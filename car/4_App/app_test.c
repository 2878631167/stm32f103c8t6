/**
 * @file    app_test.c
 * @brief   测试框架实现
 * @details 整合所有硬件测试功能，提供统一的测试管理接口
 * @author  Author
 * @date    2026-05-13
 */

#include "app_test.h"
#include "app_car.h"
#include <string.h>

/* 当前测试项 */
static TestItem_TypeDef current_test = TEST_NONE;
static uint32_t test_start_time = 0;

/**
 * @brief 初始化测试框架
 */
void Test_Init(void)
{
    current_test = TEST_NONE;
    test_start_time = 0;
}

/**
 * @brief 设置当前测试项
 */
void Test_SetCurrent(TestItem_TypeDef item)
{
    current_test = item;
    test_start_time = SysTick_GetMillis();
}

/**
 * @brief 获取当前测试项
 */
TestItem_TypeDef Test_GetCurrent(void)
{
    return current_test;
}

/**
 * @brief 运行选定的测试
 */
void Test_RunSelected(void)
{
    uint32_t now = SysTick_GetMillis();
    
    /* 超时保护（30秒） */
    if (test_start_time != 0 && (now - test_start_time > 30000))
    {
        Car_SetMode(CAR_MODE_STOP);
        test_start_time = 0;
        BT_SendString("Test timeout\r\n");
        return;
    }
    
    /* 根据当前测试项执行 */
    switch (current_test)
    {
        case TEST_LED:
            Test_LED();
            break;
            
        case TEST_OLED:
            Test_OLED();
            break;
            
        case TEST_L298N:
            Test_L298N();
            break;
            
        case TEST_TRACK:
            Test_Track();
            break;
            
        case TEST_BT:
            Test_BT();
            break;
            
        case TEST_PID:
            Test_PID();
            break;
            
        case TEST_ALL:
            Test_All_Sequence();
            break;
            
        default:
            Car_SetMode(CAR_MODE_STOP);
            break;
    }
}

/**
 * @brief 处理测试命令
 */
void Test_HandleCommand(void)
{
    /* 解析蓝牙接收的命令 */
    /* 格式：[TEST,ITEM] 例如 [TEST,LED] 或 [TEST,ALL] */
    
    if (BT_RxPacket[0] != 'T' || 
        BT_RxPacket[1] != 'E' || 
        BT_RxPacket[2] != 'S' || 
        BT_RxPacket[3] != 'T')
    {
        return;  /* 不是测试命令 */
    }
    
    BT_RxFlag = 0;
    
    /* 解析测试项 */
    if (strstr((char*)BT_RxPacket, "LED"))
    {
        Car_SetMode(CAR_MODE_TEST);
        Test_SetCurrent(TEST_LED);
        BT_SendString("Test: LED selected\r\n");
    }
    else if (strstr((char*)BT_RxPacket, "OLED"))
    {
        Car_SetMode(CAR_MODE_TEST);
        Test_SetCurrent(TEST_OLED);
        BT_SendString("Test: OLED selected\r\n");
    }
    else if (strstr((char*)BT_RxPacket, "L298N") || strstr((char*)BT_RxPacket, "MOTOR"))
    {
        Car_SetMode(CAR_MODE_TEST);
        Test_SetCurrent(TEST_L298N);
        BT_SendString("Test: L298N selected\r\n");
    }
    else if (strstr((char*)BT_RxPacket, "TRACK"))
    {
        Car_SetMode(CAR_MODE_TEST);
        Test_SetCurrent(TEST_TRACK);
        BT_SendString("Test: TRACK selected\r\n");
    }
    else if (strstr((char*)BT_RxPacket, "BT"))
    {
        Car_SetMode(CAR_MODE_TEST);
        Test_SetCurrent(TEST_BT);
        BT_SendString("Test: BT selected\r\n");
    }
    else if (strstr((char*)BT_RxPacket, "PID"))
    {
        Car_SetMode(CAR_MODE_TEST);
        Test_SetCurrent(TEST_PID);
        BT_SendString("Test: PID selected\r\n");
    }
    else if (strstr((char*)BT_RxPacket, "ALL"))
    {
        Car_SetMode(CAR_MODE_TEST);
        Test_SetCurrent(TEST_ALL);
        BT_SendString("Test: ALL selected\r\n");
    }
    else if (strstr((char*)BT_RxPacket, "STOP"))
    {
        Car_SetMode(CAR_MODE_STOP);
        Test_SetCurrent(TEST_NONE);
        BT_SendString("Test stopped\r\n");
    }
}

/**
 * @brief LED 测试
 */
void Test_LED(void)
{
    static uint32_t last_toggle = 0;
    uint32_t now = SysTick_GetMillis();
    
    if (now - last_toggle > 500)  /* 每 500ms 翻转 */
    {
        last_toggle = now;
        
        static uint8_t led_state = 0;
        led_state = !led_state;
        
        if (led_state)
            LED_On();
        else
            LED_Off();
        
        OLED_Clear();
        OLED_ShowString(0, 0, "Test: LED", OLED_6X8);
        OLED_ShowString(0, 8, led_state ? "ON" : "OFF", OLED_6X8);
        OLED_Update();
    }
}

/**
 * @brief OLED 测试
 */
void Test_OLED(void)
{
    static uint8_t step = 0;
    static uint32_t last_change = 0;
    uint32_t now = SysTick_GetMillis();
    
    if (now - last_change > 1000)  /* 每秒切换显示内容 */
    {
        last_change = now;
        step++;
        if (step > 3) step = 1;
        
        OLED_Clear();
        switch(step)
        {
            case 1:
                OLED_ShowString(0, 0, "Test: OLED", OLED_6X8);
                OLED_ShowString(0, 8, "Hello!", OLED_6X8);
                break;
            case 2:
                OLED_ShowString(0, 0, "Test: OLED", OLED_6X8);
                OLED_ShowNum(0, 8, 2026, 4, OLED_6X8);
                break;
            case 3:
                OLED_ShowString(0, 0, "Test: OLED", OLED_6X8);
                OLED_ShowString(0, 8, "OK!", OLED_6X8);
                break;
        }
        OLED_Update();
    }
}

/**
 * @brief L298N 电机测试
 */
void Test_L298N(void)
{
    static uint8_t step = 0;
    static uint32_t last_change = 0;
    uint32_t now = SysTick_GetMillis();
    
    if (now - last_change > 2000)  /* 每 2 秒切换 */
    {
        last_change = now;
        step++;
        if (step > 4) step = 1;
        
        OLED_Clear();
        switch(step)
        {
            case 1:
                OLED_ShowString(0, 0, "Test: Motor", OLED_6X8);
                OLED_ShowString(0, 8, "Forward", OLED_6X8);
                L298N_SetMotorA(L298N_FORWARD, 50);
                L298N_SetMotorB(L298N_FORWARD, 50);
                break;
            case 2:
                OLED_ShowString(0, 0, "Test: Motor", OLED_6X8);
                OLED_ShowString(0, 8, "Backward", OLED_6X8);
                L298N_SetMotorA(L298N_BACKWARD, 50);
                L298N_SetMotorB(L298N_BACKWARD, 50);
                break;
            case 3:
                OLED_ShowString(0, 0, "Test: Motor", OLED_6X8);
                OLED_ShowString(0, 8, "Left", OLED_6X8);
                L298N_SetMotorA(L298N_FORWARD, 30);
                L298N_SetMotorB(L298N_FORWARD, 70);
                break;
            case 4:
                OLED_ShowString(0, 0, "Test: Motor", OLED_6X8);
                OLED_ShowString(0, 8, "Right", OLED_6X8);
                L298N_SetMotorA(L298N_FORWARD, 70);
                L298N_SetMotorB(L298N_FORWARD, 30);
                break;
        }
        OLED_Update();
    }
}

/**
 * @brief 循迹传感器测试
 */
void Test_Track(void)
{
    /* 实时显示循迹传感器状态 */
    uint8_t s[5];
    s[0] = Track_Read(TRACK_LEFT2);
    s[1] = Track_Read(TRACK_LEFT1);
    s[2] = Track_Read(TRACK_MIDDLE);
    s[3] = Track_Read(TRACK_RIGHT1);
    s[4] = Track_Read(TRACK_RIGHT2);
    
    OLED_Clear();
    OLED_ShowString(0, 0, "Track Test", OLED_6X8);
    OLED_ShowString(0, 8, "L2:", OLED_6X8);
    OLED_ShowNum(0, 16, s[0], 1, OLED_6X8);
    OLED_ShowString(0, 24, "L1:", OLED_6X8);
    OLED_ShowNum(0, 32, s[1], 1, OLED_6X8);
    OLED_ShowString(0, 40, "M:", OLED_6X8);
    OLED_ShowNum(0, 48, s[2], 1, OLED_6X8);
    OLED_ShowString(0, 56, "R1:", OLED_6X8);
    OLED_ShowNum(0, 64, s[3], 1, OLED_6X8);
    OLED_ShowString(0, 72, "R2:", OLED_6X8);
    OLED_ShowNum(0, 80, s[4], 1, OLED_6X8);
    OLED_Update();
}

/**
 * @brief 蓝牙测试
 */
void Test_BT(void)
{
    OLED_Clear();
    OLED_ShowString(0, 0, "Test: BT", OLED_6X8);
    OLED_ShowString(0, 8, "Send CMD", OLED_6X8);
    OLED_Update();
    
    /* 蓝牙测试主要通过命令交互，这里仅显示提示 */
}

/**
 * @brief PID 测试
 */
void Test_PID(void)
{
    static float target = 100.0f;
    static float actual = 0.0f;
    static float output = 0.0f;
    static uint32_t last_calc = 0;
    uint32_t now = SysTick_GetMillis();
    
    if (now - last_calc > 100)  /* 每 100ms 计算一次 */
    {
        last_calc = now;
        
        /* 模拟 PID 控制过程 */
        PID_TypeDef* pid = Car_GetPID();
        output = PID_Positional(pid, target, actual);
        actual += output * 0.1f;
        
        /* 显示结果 */
        OLED_Clear();
        OLED_ShowString(0, 0, "Test: PID", OLED_6X8);
        OLED_ShowString(0, 8, "Tar:", OLED_6X8);
        OLED_ShowFloatNum(0, 20, target, 3, 1, OLED_6X8);
        OLED_ShowString(0, 32, "Act:", OLED_6X8);
        OLED_ShowFloatNum(0, 44, actual, 3, 1, OLED_6X8);
        OLED_Update();
    }
}

/**
 * @brief 按顺序执行所有测试
 */
void Test_All_Sequence(void)
{
    static uint32_t test_start = 0;
    uint32_t now = SysTick_GetMillis();
    
    /* 初始化 */
    if (test_start == 0)
    {
        test_start = now;
    }
    
    /* 每个测试运行 5 秒 */
    uint8_t current_step = (now - test_start) / 5000;
    
    if (current_step >= 6)
    {
        /* 所有测试完成 */
        Car_SetMode(CAR_MODE_STOP);
        test_start = 0;
        BT_SendString("All tests completed\r\n");
        return;
    }
    
    /* 根据阶段执行不同测试 */
    switch(current_step)
    {
        case 0:
            Test_LED();
            break;
        case 1:
            Test_OLED();
            break;
        case 2:
            Test_L298N();
            break;
        case 3:
            Test_Track();
            break;
        case 4:
            Test_BT();
            break;
        case 5:
            Test_PID();
            break;
    }
}
