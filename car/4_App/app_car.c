/**
 * @file    app_car.c
 * @brief   小车应用层实现
 * @details 实现小车业务逻辑，包括模式管理、循迹控制、速度控制等
 * @author  Author
 * @date    2026-05-13
 */

#include "app_car.h"
#include "StateMachine.h"

/* 全局状态实例 */
CarState_TypeDef CarState;

/* PID 实例（供外部模块使用） */
PID_TypeDef PID_Steer;

/* PID 更新标志（供外部模块使用） */
volatile uint8_t PID_Updated = 0;

/**
 * @brief 初始化小车应用
 */
void Car_Init(void)
{
    /* 初始化 PID 控制器 */
    PID_Init(&PID_Steer, 1.0f, 0.1f, 0.05f, 100.0f, -100.0f);
    
    /* 初始化状态 */
    CarState.mode = CAR_MODE_STOP;
    CarState.speed_left = 0;
    CarState.speed_right = 0;
    CarState.track_error = 0;
    CarState.pid_output = 0.0f;
    
    /* 停止电机 */
    L298N_SetMotorA(L298N_STOP, 0);
    L298N_SetMotorB(L298N_STOP, 0);
}

/**
 * @brief 小车主循环函数
 */
void Car_Run(void)
{
    /* 获取当前系统状态 */
    StateID_TypeDef sys_state = SM_GetCurrentState(System_GetStateMachine());
    
    /* 根据系统状态决定小车行为 */
    switch (sys_state)
    {
        case STATE_ID_IDLE:
            Car_Stop();
            break;
            
        case STATE_ID_MANUAL_CTRL:
            Car_ManualControl();
            break;
            
        case STATE_ID_AUTO_TRACK:
            Car_TrackControl();
            break;
            
        case STATE_ID_TEST:
            /* 测试模式由 app_test 模块处理 */
            break;
            
        case STATE_ID_ERROR:
        case STATE_ID_SHUTDOWN:
            Car_Stop();
            break;
            
        default:
            Car_Stop();
            break;
    }
}

/**
 * @brief 更新 OLED 显示内容
 */
void Car_UpdateDisplay(void)
{
    static uint32_t last_update = 0;
    uint32_t now = SysTick_GetMillis();
    
    /* 每 100ms 更新一次显示 */
    if (now - last_update < 100) return;
    last_update = now;
    
    OLED_Clear();
    
    /* 显示系统状态 */
    const char* state_name = SM_GetStateName(SM_GetCurrentState(System_GetStateMachine()));
    OLED_ShowString(0, 0, "State:", OLED_6X8);
    OLED_ShowString(0, 30, (char*)state_name, OLED_6X8);
    
    /* 显示速度 */
    OLED_ShowString(0, 8, "L:", OLED_6X8);
    OLED_ShowNum(0, 16, CarState.speed_left, 3, OLED_6X8);
    OLED_ShowString(0, 32, "R:", OLED_6X8);
    OLED_ShowNum(0, 40, CarState.speed_right, 3, OLED_6X8);
    
    /* 显示误差 */
    OLED_ShowString(0, 16, "Err:", OLED_6X8);
    OLED_ShowSignedNum(0, 36, CarState.track_error, 2, OLED_6X8);
    
    OLED_Update();
}

/**
 * @brief 设置小车工作模式（通过状态机事件触发）
 */
void Car_SetMode(CarMode_TypeDef mode)
{
    /* 将小车模式映射到系统状态机事件 */
    StateEvent_TypeDef event = EVENT_NONE;
    
    switch(mode)
    {
        case CAR_MODE_STOP:
            event = EVENT_STOP;
            break;
        case CAR_MODE_MANUAL:
            event = EVENT_MANUAL_CTRL;
            break;
        case CAR_MODE_TRACK:
            event = EVENT_AUTO_TRACK;
            break;
        case CAR_MODE_AVOIDANCE:
            event = EVENT_AUTO_AVOID;
            break;
        case CAR_MODE_TEST:
            event = EVENT_TEST_MODE;
            break;
        default:
            return;
    }
    
    /* 触发状态机事件 */
    SM_TriggerEvent(System_GetStateMachine(), event);
}

/**
 * @brief 设置左右轮速度
 */
void Car_SetSpeed(uint8_t left, uint8_t right)
{
    /* 限幅处理 */
    if (left > 100) left = 100;
    if (right > 100) right = 100;
    
    CarState.speed_left = left;
    CarState.speed_right = right;
    
    /* 控制电机 */
    if (left == 0 && right == 0)
    {
        L298N_SetMotorA(L298N_STOP, 0);
        L298N_SetMotorB(L298N_STOP, 0);
    }
    else
    {
        L298N_SetMotorA(L298N_FORWARD, left);
        L298N_SetMotorB(L298N_FORWARD, right);
    }
}

/**
 * @brief 停止小车
 */
void Car_Stop(void)
{
    Car_SetSpeed(0, 0);
}

/**
 * @brief 手动控制模式处理
 */
void Car_ManualControl(void)
{
    /* 手动模式下，速度由蓝牙命令设置 */
    /* 这里可以添加超时保护或其他逻辑 */
}

/**
 * @brief 循迹控制算法
 */
void Car_TrackControl(void)
{
    /* 1. 读取传感器 */
    uint8_t s[5];
    s[0] = Track_Read(TRACK_LEFT2);
    s[1] = Track_Read(TRACK_LEFT1);
    s[2] = Track_Read(TRACK_MIDDLE);
    s[3] = Track_Read(TRACK_RIGHT1);
    s[4] = Track_Read(TRACK_RIGHT2);
    
    /* 2. 计算误差（加权平均法） */
    int16_t error = 0;
    if (s[0]) error -= 4;
    if (s[1]) error -= 2;
    if (s[2]) error += 0;
    if (s[3]) error += 2;
    if (s[4]) error += 4;
    
    CarState.track_error = error;
    
    /* 3. PID 计算 */
    float output = PID_Positional(&PID_Steer, 0, error);
    CarState.pid_output = output;
    
    /* 4. 电机控制（差速转向） */
    int16_t base_speed = 50;  /* 基础速度 */
    int16_t left_speed = base_speed + (int16_t)output;
    int16_t right_speed = base_speed - (int16_t)output;
    
    /* 限幅 */
    if (left_speed > 100) left_speed = 100;
    if (left_speed < 0) left_speed = 0;
    if (right_speed > 100) right_speed = 100;
    if (right_speed < 0) right_speed = 0;
    
    Car_SetSpeed((uint8_t)left_speed, (uint8_t)right_speed);
}

/**
 * @brief 获取 PID 实例指针
 */
PID_TypeDef* Car_GetPID(void)
{
    return &PID_Steer;
}