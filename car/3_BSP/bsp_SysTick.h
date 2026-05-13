/**
 * @file    bsp_systick.h
 * @brief   SysTick 系统滴答定时器驱动头文件
 * @details 提供系统时间戳获取和延时功能
 * @author  Author
 * @date    2026-05-13
 */

#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H

#include <stdint.h>

/**
 * @brief 初始化 SysTick 定时器
 * @note  配置 SysTick 为 1ms 中断周期
 */
void SysTick_Init(void);

/**
 * @brief 获取系统运行时间（毫秒）
 * @return 系统运行时间（ms）
 * @note  从系统启动开始计时，约 49.7 天后溢出归零
 */
uint32_t SysTick_GetMillis(void);

/**
 * @brief 获取系统运行时间（微秒）
 * @return 系统运行时间（us）
 * @note  精度较低，适合短时间测量
 */
uint32_t SysTick_GetMicros(void);

/**
 * @brief 精确延时（毫秒）
 * @param ms 延时时间（毫秒）
 */
void SysTick_DelayMs(uint32_t ms);

/**
 * @brief 精确延时（微秒）
 * @param us 延时时间（微秒）
 * @note  基于 CPU 主频计算循环次数
 */
void SysTick_DelayUs(uint32_t us);

#endif /* __BSP_SYSTICK_H */
