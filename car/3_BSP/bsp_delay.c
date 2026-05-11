/**
 * @file    bsp_delay.c
 * @brief   延时驱动实现
 * @details 提供毫秒和微秒级延时功能
 * @author  Author
 * @date    2026-05-07
 */

#include "bsp_delay.h"

void Delay_Init(void)
{
}

void Delay_Ms(uint32_t ms)
{
    volatile uint32_t i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 10000; j++);
}

void Delay_Us(uint32_t us)
{
    volatile uint32_t i;
    for (i = 0; i < us * 10; i++);
}