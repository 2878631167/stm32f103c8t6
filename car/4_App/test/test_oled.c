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
}

void Test_OLED_ShowChar(void)
{
    OLED_Init();
    OLED_Clear();
    OLED_ShowChar(1, 1, 'A');
    OLED_ShowChar(2, 1, 'B');
    OLED_ShowChar(3, 1, 'C');
}

void Test_OLED_ShowString(void)
{
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(1, 1, "Hello");
    OLED_ShowString(2, 1, "STM32");
    OLED_ShowString(3, 1, "OLED Test");
}

void Test_OLED_ShowNum(void)
{
    OLED_Init();
    OLED_Clear();
    OLED_ShowNum(1, 1, 12345, 5);
    OLED_ShowNum(2, 1, 987, 3);
    OLED_ShowNum(3, 1, 0, 1);
}

void Test_OLED_ShowSignedNum(void)
{
    OLED_Init();
    OLED_Clear();
    OLED_ShowSignedNum(1, 1, 1234, 4);
    OLED_ShowSignedNum(2, 1, -567, 4);
    OLED_ShowSignedNum(3, 1, 0, 1);
}

void Test_OLED_ShowHexNum(void)
{
    OLED_Init();
    OLED_Clear();
    OLED_ShowHexNum(1, 1, 0xABCD, 4);
    OLED_ShowHexNum(2, 1, 0x1234, 4);
    OLED_ShowHexNum(3, 1, 0xFF, 2);
}

void Test_OLED_All(void)
{
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(1, 1, "OLED Test");
    OLED_ShowNum(2, 1, 2026, 4);
    OLED_ShowString(3, 1, "OK");
    Delay_Ms(2000);
    OLED_Clear();
    OLED_ShowString(1, 1, "Complete!");
}