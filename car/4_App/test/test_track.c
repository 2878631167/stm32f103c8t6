/**
 * @file    test_track.c
 * @brief   循迹传感器驱动测试
 * @author  Author
 * @date    2026-05-08
 */

#include "Bsp.h"

void Test_Track_ReadOnce(void)
{
    Track_Init();
    OLED_Init();
    OLED_Clear();

    uint8_t s1 = Track_Read(TRACK_LEFT2);
    uint8_t s2 = Track_Read(TRACK_LEFT1);
    uint8_t s3 = Track_Read(TRACK_MIDDLE);
    uint8_t s4 = Track_Read(TRACK_RIGHT1);
    uint8_t s5 = Track_Read(TRACK_RIGHT2);

    OLED_ShowString(0, 0, "L2:", OLED_6X8);
    OLED_ShowNum(0, 12, s1, 1, OLED_6X8);
    OLED_ShowString(0, 20, "L1:", OLED_6X8);
    OLED_ShowNum(0, 32, s2, 1, OLED_6X8);
    OLED_ShowString(0, 40, "Mid:", OLED_6X8);
    OLED_ShowNum(0, 52, s3, 1, OLED_6X8);
    OLED_ShowString(0, 64, "R1:", OLED_6X8);
    OLED_ShowNum(0, 76, s4, 1, OLED_6X8);
    OLED_ShowString(0, 84, "R2:", OLED_6X8);
    OLED_ShowNum(0, 96, s5, 1, OLED_6X8);
    OLED_Update();
}

void Test_Track_Loop(void)
{
    Track_Init();
    OLED_Init();

    while(1)
    {
        OLED_Clear();
        OLED_ShowString(0, 0, "Track:", OLED_6X8);

        uint8_t s1 = Track_Read(TRACK_LEFT2);
        uint8_t s2 = Track_Read(TRACK_LEFT1);
        uint8_t s3 = Track_Read(TRACK_MIDDLE);
        uint8_t s4 = Track_Read(TRACK_RIGHT1);
        uint8_t s5 = Track_Read(TRACK_RIGHT2);

        OLED_ShowNum(0, 32, s1, 1, OLED_6X8);
        OLED_ShowNum(0, 40, s2, 1, OLED_6X8);
        OLED_ShowNum(0, 48, s3, 1, OLED_6X8);
        OLED_ShowNum(0, 56, s4, 1, OLED_6X8);
        OLED_ShowNum(0, 64, s5, 1, OLED_6X8);
        OLED_Update();

        Delay_Ms(200);
    }
}