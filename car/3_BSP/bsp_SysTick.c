/**
 * @file    bsp_systick.c
 * @brief   SysTick 系统滴答定时器驱动实现
 * @details 基于 Cortex-M3 SysTick 定时器实现系统时间戳和延时功能
 * @author  Author
 * @date    2026-05-13
 */

#include "bsp_systick.h"
#include "stm32f10x.h"

/* 系统主频（Hz） */
#define SYSCLK_FREQ     72000000UL

/* SysTick 计数值（1ms） */
#define SYSTICK_RELOAD_VAL  (SYSCLK_FREQ / 1000)

/* 全局时间计数器 */
static volatile uint32_t s_sysTickCounter = 0;

/**
 * @brief SysTick 中断服务函数
 * @note  每 1ms 触发一次中断，计数器加 1
 */
void SysTick_Handler(void)
{
    s_sysTickCounter++;
}

/**
 * @brief 初始化 SysTick 定时器
 */
void SysTick_Init(void)
{
    /* 配置 SysTick 为 1ms 中断周期 */
    /* 使用内核时钟作为时钟源 */
    SysTick_Config(SYSTICK_RELOAD_VAL);
    
    /* 设置优先级 */
    NVIC_SetPriority(SysTick_IRQn, 0);
}

/**
 * @brief 获取系统运行时间（毫秒）
 */
uint32_t SysTick_GetMillis(void)
{
    return s_sysTickCounter;
}

/**
 * @brief 获取系统运行时间（微秒）
 */
uint32_t SysTick_GetMicros(void)
{
    uint32_t ms = s_sysTickCounter;
    uint32_t ticks = SysTick->VAL;
    uint32_t load = SysTick->LOAD + 1;
    
    /* 计算当前毫秒内的微秒数 */
    uint32_t us_in_ms = ((load - ticks) * 1000) / load;
    
    return (ms * 1000) + us_in_ms;
}

/**
 * @brief 精确延时（毫秒）
 */
void SysTick_DelayMs(uint32_t ms)
{
    uint32_t start = s_sysTickCounter;
    while ((s_sysTickCounter - start) < ms)
    {
        __WFI();  /* 进入低功耗模式等待中断 */
    }
}

/**
 * @brief 精确延时（微秒）
 */
void SysTick_DelayUs(uint32_t us)
{
    uint32_t start_us = SysTick_GetMicros();
    while ((SysTick_GetMicros() - start_us) < us)
    {
        /* 忙等待 */
    }
}
