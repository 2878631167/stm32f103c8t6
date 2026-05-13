/**
 * @file    test_oled.c
 * @brief   OLED驱动测试
 * @author  Author
 * @date    2026-05-08
 */

#include "Bsp.h"

void Test_OLED_Clear(void)
{
    OLED_Init();
    OLED_Clear();
    OLED_Update();
}

void Test_OLED_ShowChar(void)
{
    OLED_Init();
    OLED_Clear();
    OLED_ShowChar(0, 0, 'A', OLED_6X8);
    OLED_ShowChar(0, 8, 'B', OLED_6X8);
    OLED_ShowChar(0, 16, 'C', OLED_6X8);
    OLED_Update();
}

void Test_OLED_ShowString(void)
{
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0, 0, "Hello", OLED_6X8);
    OLED_ShowString(0, 8, "STM32", OLED_6X8);
    OLED_ShowString(0, 16, "OLED Test", OLED_6X8);
    OLED_Update();
}

void Test_OLED_ShowNum(void)
{
    OLED_Init();
    OLED_Clear();
    OLED_ShowNum(0, 0, 12345, 5, OLED_6X8);
    OLED_ShowNum(0, 8, 987, 3, OLED_6X8);
    OLED_ShowNum(0, 16, 0, 1, OLED_6X8);
    OLED_Update();
}

void Test_OLED_ShowSignedNum(void)
{
    OLED_Init();
    OLED_Clear();
    OLED_ShowSignedNum(0, 0, 1234, 4, OLED_6X8);
    OLED_ShowSignedNum(0, 8, -567, 4, OLED_6X8);
    OLED_ShowSignedNum(0, 16, 0, 1, OLED_6X8);
    OLED_Update();
}

void Test_OLED_ShowHexNum(void)
{
    OLED_Init();
    OLED_Clear();
    OLED_ShowHexNum(0, 0, 0xABCD, 4, OLED_6X8);
    OLED_ShowHexNum(0, 8, 0x1234, 4, OLED_6X8);
    OLED_ShowHexNum(0, 16, 0xFF, 2, OLED_6X8);
    OLED_Update();
}

void Test_OLED_All(void)
{
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0, 0, "OLED Test", OLED_6X8);
    OLED_ShowNum(0, 8, 2026, 4, OLED_6X8);
    OLED_ShowString(0, 16, "OK", OLED_6X8);
    OLED_Update();
    Delay_Ms(2000);
    OLED_Clear();
    OLED_ShowString(0, 0, "Complete!", OLED_6X8);
    OLED_Update();
}