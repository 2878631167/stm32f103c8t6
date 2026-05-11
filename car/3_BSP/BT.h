/**
 * @file    BT.h
 * @brief   蓝牙串口驱动头文件
 * @details 提供蓝牙模块初始化和发送函数声明
 * @author  Author
 * @date    2026-05-07
 */

#ifndef __BT_H
#define __BT_H

#include <stdint.h>

#define BT_RX_BUFFER_SIZE 256

extern volatile char BT_RxPacket[];
extern volatile uint8_t BT_RxFlag;

void BT_Init(void);
void BT_SendString(char *String);
void BT_SendByte(uint8_t Byte);

#endif