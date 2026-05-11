/**
 * @file    Board_Config.h
 * @brief   引脚配置头文件
 * @details 定义所有外设的GPIO引脚和时钟配置
 * @author  Author
 * @date    2026-05-07
 */

#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

#include "stm32f10x.h"

#define LED_GPIO_PORT     GPIOC
#define LED_GPIO_PIN      GPIO_Pin_13
#define LED_GPIO_CLK      RCC_APB2Periph_GPIOC

#define OLED_SCL_PORT     GPIOB
#define OLED_SCL_PIN      GPIO_Pin_8
#define OLED_SCL_CLK      RCC_APB2Periph_GPIOB

#define OLED_SDA_PORT     GPIOB
#define OLED_SDA_PIN      GPIO_Pin_9
#define OLED_SDA_CLK      RCC_APB2Periph_GPIOB

#define BT_TX_PORT        GPIOB
#define BT_TX_PIN         GPIO_Pin_10
#define BT_TX_CLK         RCC_APB1Periph_USART3

#define BT_RX_PORT        GPIOB
#define BT_RX_PIN         GPIO_Pin_11
#define BT_RX_CLK         RCC_APB1Periph_USART3
#define BT_GPIO_CLK       RCC_APB2Periph_GPIOB

#define L298N_ENA_PORT    GPIOA
#define L298N_ENA_PIN     GPIO_Pin_0
#define L298N_ENA_CLK     RCC_APB2Periph_GPIOA
#define L298N_ENA_TIM     TIM2
#define L298N_ENA_CHANNEL TIM_Channel_1

#define L298N_ENB_PORT    GPIOA
#define L298N_ENB_PIN     GPIO_Pin_1
#define L298N_ENB_CLK     RCC_APB2Periph_GPIOA
#define L298N_ENB_TIM     TIM2
#define L298N_ENB_CHANNEL TIM_Channel_2

#define L298N_IN1_PORT    GPIOB
#define L298N_IN1_PIN     GPIO_Pin_0
#define L298N_IN1_CLK     RCC_APB2Periph_GPIOB

#define L298N_IN2_PORT    GPIOB
#define L298N_IN2_PIN     GPIO_Pin_1
#define L298N_IN2_CLK     RCC_APB2Periph_GPIOB

#define L298N_IN3_PORT    GPIOB
#define L298N_IN3_PIN     GPIO_Pin_5
#define L298N_IN3_CLK     RCC_APB2Periph_GPIOB

#define L298N_IN4_PORT    GPIOB
#define L298N_IN4_PIN     GPIO_Pin_6
#define L298N_IN4_CLK     RCC_APB2Periph_GPIOB

#define TRACK_S1_PORT     GPIOB
#define TRACK_S1_PIN      GPIO_Pin_10
#define TRACK_S1_CLK      RCC_APB2Periph_GPIOB

#define TRACK_S2_PORT     GPIOB
#define TRACK_S2_PIN      GPIO_Pin_11
#define TRACK_S2_CLK      RCC_APB2Periph_GPIOB

#define TRACK_S3_PORT     GPIOB
#define TRACK_S3_PIN      GPIO_Pin_12
#define TRACK_S3_CLK      RCC_APB2Periph_GPIOB

#define TRACK_S4_PORT     GPIOB
#define TRACK_S4_PIN      GPIO_Pin_13
#define TRACK_S4_CLK      RCC_APB2Periph_GPIOB

#define TRACK_S5_PORT     GPIOB
#define TRACK_S5_PIN      GPIO_Pin_14
#define TRACK_S5_CLK      RCC_APB2Periph_GPIOB

#endif