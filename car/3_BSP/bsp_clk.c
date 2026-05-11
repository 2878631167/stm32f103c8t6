/**
 * @file    bsp_clk.c
 * @brief   系统时钟驱动实现
 * @details 配置HSE和PLL为72MHz系统时钟
 * @author  Author
 * @date    2026-05-07
 */

#include "bsp_clk.h"

void SystemClock_Config(void)
{
    RCC_ClocksTypeDef RCC_Clocks;

    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
    RCC_PLLCmd(DISABLE);
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    RCC_PLLCmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != 0x08);

    RCC_GetClocksFreq(&RCC_Clocks);
}