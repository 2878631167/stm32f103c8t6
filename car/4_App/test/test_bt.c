/**
 * @file    test_bt.c
 * @brief   蓝牙驱动测试
 * @author  Author
 * @date    2026-05-08
 */

#include "Bsp.h"
#include <stdio.h>

void Test_BT_Init(void)
{
    BT_Init();
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(1, 1, "BT Ready");
    BT_SendString("BT Init OK\r\n");
}

void Test_BT_SendString(void)
{
    BT_Init();
    BT_SendString("Hello BT\r\n");
    Delay_Ms(1000);
    BT_SendString("Test Data\r\n");
    Delay_Ms(1000);
    BT_SendString("2026\r\n");
}

void Test_BT_SendLoop(void)
{
    BT_Init();
    uint8_t count = 0;
    while(1)
    {
        char buf[32];
        sprintf(buf, "Count: %d\r\n", count++);
        BT_SendString(buf);
        Delay_Ms(1000);
    }
}

void Test_BT_ShowRxData(void)
{
    BT_Init();
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(1, 1, "Waiting...");

    while(1)
    {
        if (BT_RxFlag == 1)
        {
            OLED_Clear();
            OLED_ShowString(1, 1, "RX:");
            char temp[BT_RX_BUFFER_SIZE];
            for (uint16_t i = 0; i < BT_RX_BUFFER_SIZE && BT_RxPacket[i] != '\0'; i++)
            {
                temp[i] = BT_RxPacket[i];
            }
            temp[BT_RX_BUFFER_SIZE - 1] = '\0';
            OLED_ShowString(2, 1, temp);
            BT_RxFlag = 0;
        }
        Delay_Ms(100);
    }
}

void Test_BT_Echo(void)
{
    BT_Init();
    while(1)
    {
        if (BT_RxFlag == 1)
        {
            char temp[BT_RX_BUFFER_SIZE];
            for (uint16_t i = 0; i < BT_RX_BUFFER_SIZE && BT_RxPacket[i] != '\0'; i++)
            {
                temp[i] = BT_RxPacket[i];
            }
            temp[BT_RX_BUFFER_SIZE - 1] = '\0';
            BT_SendString(temp);
            BT_RxFlag = 0;
        }
    }
}