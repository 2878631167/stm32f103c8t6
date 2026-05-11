#include "PID.h"
#include <string.h>

/**
 * @brief PID初始化
 */
void PID_Init(PID_TypeDef *pid, float kp, float ki, float kd, 
              float max_output, float min_output)
{
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    
    pid->target = 0.0f;
    pid->actual = 0.0f;
    pid->error = 0.0f;
    pid->last_error = 0.0f;
    pid->prev_error = 0.0f;
    
    pid->integral = 0.0f;
    pid->output = 0.0f;
    
    pid->max_output = max_output;
    pid->min_output = min_output;
    pid->max_integral = (max_output - min_output) / 2.0f;
}

/**
 * @brief 增量式PID计算
 * @note 适用于执行机构带积分部件的对象（如步进电机）
 *       输出的是增量值，需要累加到执行机构
 */
float PID_Incremental(PID_TypeDef *pid, float target, float actual)
{
    pid->target = target;
    pid->actual = actual;
    
    // 计算当前误差
    pid->error = pid->target - pid->actual;
    
    // 增量式PID公式: Δu(k) = Kp*[e(k)-e(k-1)] + Ki*e(k) + Kd*[e(k)-2e(k-1)+e(k-2)]
    float p_term = pid->Kp * (pid->error - pid->last_error);
    float i_term = pid->Ki * pid->error;
    float d_term = pid->Kd * (pid->error - 2.0f * pid->last_error + pid->prev_error);
    
    // 计算增量输出
    float delta_output = p_term + i_term + d_term;
    
    // 更新误差历史
    pid->prev_error = pid->last_error;
    pid->last_error = pid->error;
    
    // 累加到输出
    pid->output += delta_output;
    
    // 输出限幅
    if (pid->output > pid->max_output) {
        pid->output = pid->max_output;
    } else if (pid->output < pid->min_output) {
        pid->output = pid->min_output;
    }
    
    return pid->output;
}

/**
 * @brief 位置式PID计算
 * @note 适用于执行机构不带积分部件的对象（如直流电机PWM控制）
 *       直接输出控制量
 */
float PID_Positional(PID_TypeDef *pid, float target, float actual)
{
    pid->target = target;
    pid->actual = actual;
    
    // 计算当前误差
    pid->error = pid->target - pid->actual;
    
    // 积分项（带抗饱和）
    pid->integral += pid->error;
    if (pid->integral > pid->max_integral) {
        pid->integral = pid->max_integral;
    } else if (pid->integral < -pid->max_integral) {
        pid->integral = -pid->max_integral;
    }
    
    // 微分项
    float derivative = pid->error - pid->last_error;
    
    // 位置式PID公式: u(k) = Kp*e(k) + Ki*Σe(k) + Kd*[e(k)-e(k-1)]
    pid->output = pid->Kp * pid->error + 
                  pid->Ki * pid->integral + 
                  pid->Kd * derivative;
    
    // 更新误差历史
    pid->last_error = pid->error;
    
    // 输出限幅
    if (pid->output > pid->max_output) {
        pid->output = pid->max_output;
    } else if (pid->output < pid->min_output) {
        pid->output = pid->min_output;
    }
    
    return pid->output;
}

/**
 * @brief 设置PID参数
 */
void PID_SetParam(PID_TypeDef *pid, float kp, float ki, float kd)
{
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
}

/**
 * @brief 清除PID积分项
 */
void PID_ClearIntegral(PID_TypeDef *pid)
{
    pid->integral = 0.0f;
}

/**
 * @brief 重置PID
 */
void PID_Reset(PID_TypeDef *pid)
{
    pid->error = 0.0f;
    pid->last_error = 0.0f;
    pid->prev_error = 0.0f;
    pid->integral = 0.0f;
    pid->output = 0.0f;
}
