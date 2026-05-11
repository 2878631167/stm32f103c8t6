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

volatile char BT_RxPacket[BT_RX_BUFFER_SIZE];
volatile uint16_t BT_RxIndex = 0;
volatile uint8_t BT_RxFlag = 0;

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
        if (BT_RxIndex < BT_RX_BUFFER_SIZE - 1)
        {
            BT_RxPacket[BT_RxIndex++] = RxData;
        }
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }

    if (USART_GetITStatus(USART3, USART_IT_IDLE) == SET)
    {
        volatile uint32_t tmp;
        tmp = USART3->SR;
        tmp = USART3->DR;
        (void)tmp;

        BT_RxPacket[BT_RxIndex] = '\0';
        BT_RxFlag = 1;
        BT_RxIndex = 0;
    }
}