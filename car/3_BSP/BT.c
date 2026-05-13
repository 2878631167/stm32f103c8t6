/**
 * @file    BT.c
 * @brief   蓝牙串口驱动实现
 * @details 使用USART3实现蓝牙透传功能
 * @author  Author
 * @date    2026-05-07
 */

#include "BT.h"
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "Board_Config.h"
#include "PID.h"
#include "L298N.h"
#include "bsp_led.h"
#include <stdlib.h>
#include <stdio.h>

char BT_RxPacket[BT_RX_BUFFER_SIZE];
uint8_t BT_RxFlag = 0;

static uint8_t RxState = 0;
static uint8_t pRxPacket = 0;

extern PID_TypeDef PID_Steer;
extern volatile uint8_t PID_Updated;

void BT_Init(void)
{
    RCC_APB1PeriphClockCmd(BT_TX_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(BT_GPIO_CLK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = BT_TX_PIN;
    GPIO_Init(BT_TX_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = BT_RX_PIN;
    GPIO_Init(BT_RX_PORT, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3, &USART_InitStructure);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART3, ENABLE);
}

void BT_SendByte(uint8_t Byte)
{
    USART_SendData(USART3, Byte);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

void BT_SendString(char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        BT_SendByte(String[i]);
    }
}

void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
    {
        uint8_t RxData = USART_ReceiveData(USART3);

        if (RxState == 0)
        {
            if (RxData == '[' && BT_RxFlag == 0)
            {
                RxState = 1;
                pRxPacket = 0;
            }
        }
        else if (RxState == 1)
        {
            if (RxData == ']')
            {
                RxState = 0;
                BT_RxPacket[pRxPacket] = '\0';
                BT_RxFlag = 1;
            }
            else
            {
                BT_RxPacket[pRxPacket] = RxData;
                pRxPacket++;
            }
        }

        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}

void BT_SendDebugInfo(char *info, int32_t value)
{
    BT_SendString(info);
    BT_SendString(": ");
    
    if (value < 0)
    {
        BT_SendByte('-');
        value = -value;
    }
    
    char buffer[12];
    uint8_t i = 0;
    if (value == 0)
    {
        BT_SendByte('0');
    }
    else
    {
        while (value > 0)
        {
            buffer[i++] = '0' + (value % 10);
            value /= 10;
        }
        while (i > 0)
        {
            BT_SendByte(buffer[--i]);
        }
    }
    
    BT_SendString("\r\n");
}

void BT_SendPIDStatus(void)
{
    BT_SendString("=== PID Status ===\r\n");
    BT_SendString("Send format: PID,Kp,Ki,Kd\r\n");
    BT_SendString("Example: PID,1.0,0.1,0.05\r\n");
}

static float BT_ParseFloat(char *str, uint16_t *pos)
{
    float result = 0.0f;
    float fraction = 0.1f;
    uint8_t has_dot = 0;
    uint8_t negative = 0;
    uint16_t i = *pos;
    
    if (str[i] == '-')
    {
        negative = 1;
        i++;
    }
    
    while (str[i] != '\0' && str[i] != ',')
    {
        if (str[i] == '.')
        {
            has_dot = 1;
            i++;
            continue;
        }
        
        if (str[i] >= '0' && str[i] <= '9')
        {
            if (!has_dot)
            {
                result = result * 10.0f + (str[i] - '0');
            }
            else
            {
                result += (str[i] - '0') * fraction;
                fraction *= 0.1f;
            }
        }
        i++;
    }
    
    *pos = i + 1;
    return negative ? -result : result;
}

uint8_t BT_ParsePIDParams(void)
{
    if (BT_RxFlag != 1)
    {
        return 0;
    }

    BT_RxFlag = 0;

    if (BT_RxPacket[0] != 'P' ||
        BT_RxPacket[1] != 'I' ||
        BT_RxPacket[2] != 'D' ||
        BT_RxPacket[3] != ',')
    {
        return 0;
    }

    uint16_t pos = 4;
    float kp = BT_ParseFloat((char*)BT_RxPacket, &pos);
    float ki = BT_ParseFloat((char*)BT_RxPacket, &pos);
    float kd = BT_ParseFloat((char*)BT_RxPacket, &pos);

    PID_SetParam(&PID_Steer, kp, ki, kd);
    PID_Updated = 1;

    BT_SendString("PID Updated: Kp=");
    char buf[16];

    return 1;
}

static uint8_t BT_ParseInt(char *str, int32_t *result)
{
    *result = 0;
    uint8_t i = 0;
    uint8_t negative = 0;

    if (str[i] == '-')
    {
        negative = 1;
        i++;
    }

    while (str[i] >= '0' && str[i] <= '9')
    {
        *result = *result * 10 + (str[i] - '0');
        i++;
    }

    if (i == 0) return 0;

    *result = negative ? -*result : *result;
    return i;
}

void BT_ProcessPacket(void)
{
    if (BT_RxFlag != 1) return;

    BT_RxFlag = 0;

    char *packet = (char*)BT_RxPacket;
    uint16_t len = 0;
    while (packet[len] != '\0') len++;

    if (len == 0)
    {
        BT_SendString("ERR: Empty packet\r\n");
        return;
    }

    uint16_t pos = 0;
    while (packet[pos] != ',' && packet[pos] != '\0') pos++;

    if (packet[pos] == '\0')
    {
        BT_SendString("ERR: No command\r\n");
        return;
    }

    packet[pos] = '\0';
    char *cmd = packet;
    char *args = packet + pos + 1;

    if (cmd[0] == 'P' && cmd[1] == 'I' && cmd[2] == 'D')
    {
        float kp = 0, ki = 0, kd = 0;
        int32_t val;

        pos = 0;
        while (args[pos] >= '0' && args[pos] <= '9' || args[pos] == '.')
            pos++;
        args[pos] = '\0';
        kp = atof(args);
        args = args + pos + 1;

        pos = 0;
        while (args[pos] >= '0' && args[pos] <= '9' || args[pos] == '.')
            pos++;
        args[pos] = '\0';
        ki = atof(args);
        args = args + pos + 1;

        kd = atof(args);

        PID_SetParam(&PID_Steer, kp, ki, kd);
        PID_Updated = 1;

        BT_SendString("OK: PID updated\r\n");
    }
    else if (cmd[0] == 'M' && cmd[1] == 'O' && cmd[2] == 'T' && cmd[3] == 'O' && cmd[4] == 'R')
    {
        uint8_t motor = 0;
        if (args[0] == 'A') motor = 0;
        else if (args[0] == 'B') motor = 1;
        else
        {
            BT_SendString("ERR: Motor A/B\r\n");
            return;
        }

        args += 2;
        if (args[0] == 'F') args[0] = 0;
        else if (args[0] == 'B') args[0] = 1;
        else
        {
            BT_SendString("ERR: Direction F/B\r\n");
            return;
        }
        uint8_t dir = args[0];

        args += 2;
        int32_t speed = atoi(args);

        if (motor == 0)
            L298N_SetMotorA(dir ? L298N_BACKWARD : L298N_FORWARD, speed);
        else
            L298N_SetMotorB(dir ? L298N_BACKWARD : L298N_FORWARD, speed);

        BT_SendString("OK: Motor set\r\n");
    }
    else if (cmd[0] == 'L' && cmd[1] == 'E' && cmd[2] == 'D')
    {
        if (args[0] == 'O' && args[1] == 'N')
        {
            LED_On();
            BT_SendString("OK: LED ON\r\n");
        }
        else if (args[0] == 'O' && args[1] == 'F' && args[2] == 'F')
        {
            LED_Off();
            BT_SendString("OK: LED OFF\r\n");
        }
        else
        {
            BT_SendString("ERR: LED ON/OFF\r\n");
        }
    }
    else if (cmd[0] == 'H' && cmd[1] == 'E' && cmd[2] == 'L' && cmd[3] == 'P')
    {
        BT_SendString("=== Commands ===\r\n");
        BT_SendString("PID,Kp,Ki,Kd\r\n");
        BT_SendString("MOTOR,A/B,F/B,0-100\r\n");
        BT_SendString("LED,ON/OFF\r\n");
        BT_SendString("STATUS\r\n");
        BT_SendString("HELP\r\n");
    }
    else if (cmd[0] == 'S' && cmd[1] == 'T' && cmd[2] == 'A' && cmd[3] == 'T' && cmd[4] == 'U' && cmd[5] == 'S')
    {
        BT_SendString("=== Status ===\r\n");
        BT_SendString("OK: Running\r\n");
    }
    else
    {
        BT_SendString("ERR: Unknown cmd\r\n");
    }
}