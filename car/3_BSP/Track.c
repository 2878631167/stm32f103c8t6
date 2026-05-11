/**
 * @file    Track.c
 * @brief   循迹传感器驱动实现
 * @details 读取5路灰度传感器状态
 * @author  Author
 * @date    2026-05-08
 */

#include "Track.h"
#include "stm32f10x.h"
#include "Board_Config.h"

void Track_Init(void)
{
    RCC_APB2PeriphClockCmd(TRACK_S1_CLK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

    GPIO_InitStructure.GPIO_Pin = TRACK_S1_PIN;
    GPIO_Init(TRACK_S1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = TRACK_S2_PIN;
    GPIO_Init(TRACK_S2_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = TRACK_S3_PIN;
    GPIO_Init(TRACK_S3_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = TRACK_S4_PIN;
    GPIO_Init(TRACK_S4_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = TRACK_S5_PIN;
    GPIO_Init(TRACK_S5_PORT, &GPIO_InitStructure);
}

uint8_t Track_Read(uint8_t index)
{
    switch(index)
    {
        case TRACK_LEFT2:
            return GPIO_ReadInputDataBit(TRACK_S1_PORT, TRACK_S1_PIN);
        case TRACK_LEFT1:
            return GPIO_ReadInputDataBit(TRACK_S2_PORT, TRACK_S2_PIN);
        case TRACK_MIDDLE:
            return GPIO_ReadInputDataBit(TRACK_S3_PORT, TRACK_S3_PIN);
        case TRACK_RIGHT1:
            return GPIO_ReadInputDataBit(TRACK_S4_PORT, TRACK_S4_PIN);
        case TRACK_RIGHT2:
            return GPIO_ReadInputDataBit(TRACK_S5_PORT, TRACK_S5_PIN);
        default:
            return 0;
    }
}