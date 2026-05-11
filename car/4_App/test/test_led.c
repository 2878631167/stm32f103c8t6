/**
 * @file    test_led.c
 * @brief   LED驱动测试
 * @author  Author
 * @date    2026-05-08
 */

#include "Bsp.h"

void Test_LED_On(void)
{
    LED_Init();
    LED_On();
}

void Test_LED_Off(void)
{
    LED_Init();
    LED_Off();
}

void Test_LED_Blink(void)
{
    LED_Init();
    while(1)
    {
        LED_On();
        Delay_Ms(500);
        LED_Off();
        Delay_Ms(500);
    }
}

void Test_LED_BlinkFast(void)
{
    LED_Init();
    while(1)
    {
        LED_On();
        Delay_Ms(200);
        LED_Off();
        Delay_Ms(200);
    }
}