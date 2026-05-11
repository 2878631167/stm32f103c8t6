#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

#include "stm32f10x.h"

#define BOARD_NAME "STM32F103C8T6"

#define LED_PORT GPIOC
#define LED_PIN GPIO_Pin_13
#define LED_RCC RCC_APB2Periph_GPIOC

#define KEY_PORT GPIOA
#define KEY_PIN GPIO_Pin_0
#define KEY_RCC RCC_APB2Periph_GPIOA

#define USARTx USART1
#define USARTx_RCC RCC_APB2Periph_USART1
#define USARTx_GPIO_RCC RCC_APB2Periph_GPIOA
#define USARTx_TX_PIN GPIO_Pin_9
#define USARTx_RX_PIN GPIO_Pin_10
#define USARTx_GPIO GPIOA

#define SPIx SPI1
#define SPIx_RCC RCC_APB2Periph_SPI1
#define SPIx_GPIO_RCC RCC_APB2Periph_GPIOA
#define SPIx_SCK_PIN GPIO_Pin_5
#define SPIx_MISO_PIN GPIO_Pin_6
#define SPIx_MOSI_PIN GPIO_Pin_7
#define SPIx_GPIO GPIOA

#define I2Cx I2C1
#define I2Cx_RCC RCC_APB1Periph_I2C1
#define I2Cx_GPIO_RCC RCC_APB2Periph_GPIOB
#define I2Cx_SCL_PIN GPIO_Pin_6
#define I2Cx_SDA_PIN GPIO_Pin_7
#define I2Cx_GPIO GPIOB

#define PWM_TIM TIM1
#define PWM_TIM_RCC RCC_APB2Periph_TIM1
#define PWM_PIN GPIO_Pin_11
#define PWM_GPIO_PORT GPIOA
#define PWM_GPIO_RCC RCC_APB2Periph_GPIOA
#define PWM_CHANNEL TIM_Channel_4

#endif