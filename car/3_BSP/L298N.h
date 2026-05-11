/**
 * @file    L298N.h
 * @brief   L298N双H桥电机驱动模块
 * @details 使用TIM2产生PWM控制电机转速，IN1-IN4控制方向
 * @author  Author
 * @date    2026-05-08
 */

#ifndef __L298N_H
#define __L298N_H

#include <stdint.h>

#define L298N_FORWARD  1   /*!< 电机正转 */
#define L298N_BACKWARD 2   /*!< 电机反转 */
#define L298N_STOP     0   /*!< 电机停止 */

/**
 * @brief 初始化L298N驱动
 */
void L298N_Init(void);

/**
 * @brief 设置电机A方向和速度
 * @param dir 方向: L298N_FORWARD/L298N_BACKWARD/L298N_STOP
 * @param speed 速度: 0-100
 */
void L298N_SetMotorA(uint8_t dir, uint8_t speed);

/**
 * @brief 设置电机B方向和速度
 * @param dir 方向: L298N_FORWARD/L298N_BACKWARD/L298N_STOP
 * @param speed 速度: 0-100
 */
void L298N_SetMotorB(uint8_t dir, uint8_t speed);

/**
 * @brief 设置电机A速度
 * @param speed 速度: 0-100
 */
void L298N_SetSpeedA(uint8_t speed);

/**
 * @brief 设置电机B速度
 * @param speed 速度: 0-100
 */
void L298N_SetSpeedB(uint8_t speed);

#endif