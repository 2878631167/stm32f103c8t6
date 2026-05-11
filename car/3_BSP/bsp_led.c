/**
 * @file    bsp_led.c
 * @brief   LED驱动实现
 * @details 控制板载LED (PC13)
 * @author  Author
 * @date    2026-05-07
 */

#include "bsp.h"

/* ============================================
 * 模块名称: LED驱动
 * 功能: LED初始化和控制
 * 硬件: PC13
 * ============================================ */

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
}

void LED_On(void)
{
    GPIO_ResetBits(LED_GPIO_PORT, LED_GPIO_PIN);
}

void LED_Off(void)
{
    GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN);
}