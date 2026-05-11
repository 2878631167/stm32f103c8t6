#ifndef __PID_H
#define __PID_H

#include "stm32f10x.h"
#include <stdint.h>

/**
 * @brief PID结构体定义
 */
typedef struct {
    float Kp;           // 比例系数
    float Ki;           // 积分系数
    float Kd;           // 微分系数
    
    float target;       // 目标值
    float actual;       // 实际值
    float error;        // 当前误差
    float last_error;   // 上次误差
    float prev_error;   // 上上次误差
    
    float integral;     // 积分累计值
    float output;       // PID输出
    
    float max_output;   // 输出最大值限制
    float min_output;   // 输出最小值限制
    float max_integral; // 积分限幅值
} PID_TypeDef;

/**
 * @brief PID初始化
 * @param pid: PID结构体指针
 * @param kp: 比例系数
 * @param ki: 积分系数
 * @param kd: 微分系数
 * @param max_output: 最大输出值
 * @param min_output: 最小输出值
 */
void PID_Init(PID_TypeDef *pid, float kp, float ki, float kd, 
              float max_output, float min_output);

/**
 * @brief 增量式PID计算
 * @param pid: PID结构体指针
 * @param target: 目标值
 * @param actual: 实际值
 * @return PID输出值
 */
float PID_Incremental(PID_TypeDef *pid, float target, float actual);

/**
 * @brief 位置式PID计算
 * @param pid: PID结构体指针
 * @param target: 目标值
 * @param actual: 实际值
 * @return PID输出值
 */
float PID_Positional(PID_TypeDef *pid, float target, float actual);

/**
 * @brief 设置PID参数
 * @param pid: PID结构体指针
 * @param kp: 比例系数
 * @param ki: 积分系数
 * @param kd: 微分系数
 */
void PID_SetParam(PID_TypeDef *pid, float kp, float ki, float kd);

/**
 * @brief 清除PID积分项
 * @param pid: PID结构体指针
 */
void PID_ClearIntegral(PID_TypeDef *pid);

/**
 * @brief 重置PID
 * @param pid: PID结构体指针
 */
void PID_Reset(PID_TypeDef *pid);

#endif /* __PID_H */
