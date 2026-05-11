/**
 * @file    Track.h
 * @brief   循迹传感器驱动头文件
 * @details 提供5路灰度传感器初始化和读取函数声明
 * @author  Author
 * @date    2026-05-08
 */

#ifndef __TRACK_H
#define __TRACK_H

#include <stdint.h>

void Track_Init(void);
uint8_t Track_Read(uint8_t index);

#define TRACK_LEFT2   0
#define TRACK_LEFT1   1
#define TRACK_MIDDLE  2
#define TRACK_RIGHT1  3
#define TRACK_RIGHT2  4

#endif