/**
 * @file    app_test.h
 * @brief   测试框架接口定义
 * @details 提供统一的测试管理接口，整合所有硬件测试功能
 * @author  Author
 * @date    2026-05-13
 */

#ifndef __APP_TEST_H
#define __APP_TEST_H

#include "Bsp.h"

/**
 * @brief 测试项枚举
 */
typedef enum {
    TEST_NONE = 0,
    TEST_LED,
    TEST_OLED,
    TEST_L298N,
    TEST_TRACK,
    TEST_BT,
    TEST_PID,
    TEST_ALL
} TestItem_TypeDef;

/**
 * @brief 初始化测试框架
 */
void Test_Init(void);

/**
 * @brief 运行选定的测试
 */
void Test_RunSelected(void);

/**
 * @brief 处理测试命令（从蓝牙接收）
 */
void Test_HandleCommand(void);

/**
 * @brief 设置当前测试项
 * @param item 测试项
 */
void Test_SetCurrent(TestItem_TypeDef item);

/**
 * @brief 获取当前测试项
 * @return 当前测试项
 */
TestItem_TypeDef Test_GetCurrent(void);

/* 单个测试函数声明 */
void Test_LED(void);
void Test_OLED(void);
void Test_L298N(void);
void Test_Track(void);
void Test_BT(void);
void Test_PID(void);
void Test_All_Sequence(void);

#endif /* __APP_TEST_H */
