/**
 * @file    bsp_delay.c
 * @brief   延时驱动实现
 * @details 提供毫秒和微秒级延时功能（基于 SysTick）
 * @author  Author
 * @date    2026-05-07
 */

#include "bsp_delay.h"
#include "bsp_SysTick.h"

void Delay_Init(void)
{
    /* 现在由 SysTick 提供延时功能 */
}

void Delay_Ms(uint32_t ms)
{
    /* 使用 SysTick 实现精确延时 */
    SysTick_DelayMs(ms);
}

void Delay_Us(uint32_t us)
{
    /* 使用 SysTick 实现精确延时 */
    SysTick_DelayUs(us);
}
