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

    OLED_ShowString(1, 1, "L2:");
    OLED_ShowNum(1, 4, s1, 1);
    OLED_ShowString(1, 6, "L1:");
    OLED_ShowNum(1, 9, s2, 1);
    OLED_ShowString(2, 1, "Mid:");
    OLED_ShowNum(2, 5, s3, 1);
    OLED_ShowString(2, 7, "R1:");
    OLED_ShowNum(2, 10, s4, 1);
    OLED_ShowString(3, 1, "R2:");
    OLED_ShowNum(3, 4, s5, 1);
}

void Test_Track_Loop(void)
{
    Track_Init();
    OLED_Init();

    while(1)
    {
        OLED_Clear();
        OLED_ShowString(1, 1, "Track:");

        uint8_t s1 = Track_Read(TRACK_LEFT2);
        uint8_t s2 = Track_Read(TRACK_LEFT1);
        uint8_t s3 = Track_Read(TRACK_MIDDLE);
        uint8_t s4 = Track_Read(TRACK_RIGHT1);
        uint8_t s5 = Track_Read(TRACK_RIGHT2);

        OLED_ShowNum(1, 7, s1, 1);
        OLED_ShowNum(2, 1, s2, 1);
        OLED_ShowNum(2, 4, s3, 1);
        OLED_ShowNum(2, 7, s4, 1);
        OLED_ShowNum(3, 1, s5, 1);

        Delay_Ms(200);
    }
}