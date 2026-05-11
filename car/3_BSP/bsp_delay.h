/**
 * @file    bsp_delay.h
 * @brief   延时驱动头文件
 * @details 提供延时函数声明
 * @author  Author
 * @date    2026-05-07
 */

#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

#include <stdint.h>

void Delay_Init(void);
void Delay_Ms(uint32_t ms);
void Delay_Us(uint32_t us);

#endif