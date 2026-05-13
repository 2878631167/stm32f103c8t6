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

void BT_SendDebugInfo(char *info, int32_t value);
void BT_SendPIDStatus(void);
uint8_t BT_ParsePIDParams(void);
void BT_ProcessPacket(void);

/**
 * @brief 发送设备完整状态信息
 * @note 包括电机状态、循迹传感器、PID参数等
 */
void BT_SendDeviceStatus(void);

/**
 * @brief 发送电机状态
 */
void BT_SendMotorStatus(void);

/**
 * @brief 发送循迹传感器状态
 */
void BT_SendTrackStatus(void);

/**
 * @brief 发送系统信息
 */
void BT_SendSystemInfo(void);

#endif