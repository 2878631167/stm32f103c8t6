/**
 * @file    test.h
 * @brief   测试函数头文件
 * @author  Author
 * @date    2026-05-08
 */

#ifndef __TEST_H
#define __TEST_H

void Test_L298N_Forward(void);
void Test_L298N_Backward(void);
void Test_L298N_Stop(void);
void Test_L298N_Left(void);
void Test_L298N_Right(void);
void Test_L298N_Loop(void);

void Test_Track_ReadOnce(void);
void Test_Track_Loop(void);

void Test_LED_On(void);
void Test_LED_Off(void);
void Test_LED_Blink(void);
void Test_LED_BlinkFast(void);

void Test_OLED_Clear(void);
void Test_OLED_ShowChar(void);
void Test_OLED_ShowString(void);
void Test_OLED_ShowNum(void);
void Test_OLED_ShowSignedNum(void);
void Test_OLED_ShowHexNum(void);
void Test_OLED_All(void);

void Test_BT_Init(void);
void Test_BT_SendString(void);
void Test_BT_SendLoop(void);
void Test_BT_ShowRxData(void);
void Test_BT_Echo(void);

#endif