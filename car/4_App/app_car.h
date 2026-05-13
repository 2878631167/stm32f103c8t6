/**
 * @file    app_car.h
 * @brief   小车应用层接口定义
 * @details 提供小车业务逻辑的抽象接口，包括模式管理、速度控制、循迹算法等
 * @author  Author
 * @date    2026-05-13
 */

#ifndef __APP_CAR_H
#define __APP_CAR_H

#include "Bsp.h"

/**
 * @brief 小车工作模式枚举
 */
typedef enum {
    CAR_MODE_STOP = 0,      /**< 停止模式 */
    CAR_MODE_MANUAL,        /**< 手动控制模式（通过蓝牙命令） */
    CAR_MODE_TRACK,         /**< 循迹模式 */
    CAR_MODE_AVOIDANCE,     /**< 避障模式（预留） */
    CAR_MODE_TEST           /**< 测试模式 */
} CarMode_TypeDef;

/**
 * @brief 小车状态结构体
 */
typedef struct {
    CarMode_TypeDef mode;   /**< 当前工作模式 */
    uint8_t speed_left;     /**< 左轮速度 (0-100) */
    uint8_t speed_right;    /**< 右轮速度 (0-100) */
    int16_t track_error;    /**< 循迹误差值 */
    float pid_output;       /**< PID 输出值 */
} CarState_TypeDef;

/**
 * @brief 全局小车状态实例
 */
extern CarState_TypeDef CarState;

/**
 * @brief 全局 PID 实例（供外部模块使用）
 */
extern PID_TypeDef PID_Steer;

/**
 * @brief PID 更新标志（供外部模块使用）
 */
extern volatile uint8_t PID_Updated;

/**
 * @brief 初始化小车应用
 * @note  配置 PID 参数、初始化状态、停止电机
 */
void Car_Init(void);

/**
 * @brief 小车主循环函数
 * @note  根据当前模式执行相应的业务逻辑
 */
void Car_Run(void);

/**
 * @brief 更新 OLED 显示内容
 * @note  显示当前模式、速度、误差等信息
 */
void Car_UpdateDisplay(void);

/**
 * @brief 设置小车工作模式
 * @param mode 目标模式
 */
void Car_SetMode(CarMode_TypeDef mode);

/**
 * @brief 设置左右轮速度
 * @param left  左轮速度 (0-100)
 * @param right 右轮速度 (0-100)
 */
void Car_SetSpeed(uint8_t left, uint8_t right);

/**
 * @brief 停止小车
 */
void Car_Stop(void);

/**
 * @brief 手动控制模式处理
 * @note  响应蓝牙发送的速度控制命令
 */
void Car_ManualControl(void);

/**
 * @brief 循迹控制算法
 * @note  读取传感器 → 计算误差 → PID 控制 → 差速转向
 */
void Car_TrackControl(void);

/**
 * @brief 获取 PID 实例指针（供测试模块使用）
 * @return PID 实例指针
 */
PID_TypeDef* Car_GetPID(void);

#endif /* __APP_CAR_H */
