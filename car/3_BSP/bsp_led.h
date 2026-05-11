/**
 * @file    bsp_led.h
 * @brief   LED驱动头文件
 * @details 提供LED初始化和控制函数声明
 * @author  Author
 * @date    2026-05-07
 */

#ifndef __BSP_LED_H
#define __BSP_LED_H

#include <stdint.h>

void LED_Init(void);
void LED_On(void);
void LED_Off(void);

#endif