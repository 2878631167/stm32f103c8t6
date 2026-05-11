/**
 * @file    L298N.c
 * @brief   L298N双H桥电机驱动实现
 * @details 使用TIM2产生PWM控制电机转速，IN1-IN4控制方向
 * @author  Author
 * @date    2026-05-08
 */

#include "L298N.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "Board_Config.h"

/**
 * @brief 初始化L298N驱动
 * @details 配置GPIO和TIM2为PWM输出模式
 */
void L298N_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = L298N_IN1_PIN;
    GPIO_Init(L298N_IN1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = L298N_IN2_PIN;
    GPIO_Init(L298N_IN2_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = L298N_IN3_PIN;
    GPIO_Init(L298N_IN3_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = L298N_IN4_PIN;
    GPIO_Init(L298N_IN4_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = L298N_ENA_PIN;
    GPIO_Init(L298N_ENA_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = L298N_ENB_PIN;
    GPIO_Init(L298N_ENB_PORT, &GPIO_InitStructure);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 100 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM2, ENABLE);

    TIM_Cmd(TIM2, ENABLE);

    L298N_SetMotorA(L298N_STOP, 0);
    L298N_SetMotorB(L298N_STOP, 0);
}

void L298N_SetMotorA(uint8_t dir, uint8_t speed)
{
    switch(dir)
    {
        case L298N_FORWARD:
            GPIO_SetBits(L298N_IN1_PORT, L298N_IN1_PIN);
            GPIO_ResetBits(L298N_IN2_PORT, L298N_IN2_PIN);
            break;
        case L298N_BACKWARD:
            GPIO_ResetBits(L298N_IN1_PORT, L298N_IN1_PIN);
            GPIO_SetBits(L298N_IN2_PORT, L298N_IN2_PIN);
            break;
        case L298N_STOP:
        default:
            GPIO_ResetBits(L298N_IN1_PORT, L298N_IN1_PIN);
            GPIO_ResetBits(L298N_IN2_PORT, L298N_IN2_PIN);
            break;
    }
    L298N_SetSpeedA(speed);
}

void L298N_SetMotorB(uint8_t dir, uint8_t speed)
{
    switch(dir)
    {
        case L298N_FORWARD:
            GPIO_SetBits(L298N_IN3_PORT, L298N_IN3_PIN);
            GPIO_ResetBits(L298N_IN4_PORT, L298N_IN4_PIN);
            break;
        case L298N_BACKWARD:
            GPIO_ResetBits(L298N_IN3_PORT, L298N_IN3_PIN);
            GPIO_SetBits(L298N_IN4_PORT, L298N_IN4_PIN);
            break;
        case L298N_STOP:
        default:
            GPIO_ResetBits(L298N_IN3_PORT, L298N_IN3_PIN);
            GPIO_ResetBits(L298N_IN4_PORT, L298N_IN4_PIN);
            break;
    }
    L298N_SetSpeedB(speed);
}

void L298N_SetSpeedA(uint8_t speed)
{
    if(speed > 100) speed = 100;
    TIM_SetCompare1(TIM2, speed);
}

void L298N_SetSpeedB(uint8_t speed)
{
    if(speed > 100) speed = 100;
    TIM_SetCompare2(TIM2, speed);
}