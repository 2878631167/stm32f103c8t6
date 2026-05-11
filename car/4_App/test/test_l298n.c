/**
 * @file    test_l298n.c
 * @brief   L298N电机驱动测试
 * @author  Author
 * @date    2026-05-08
 */

#include "Bsp.h"

void Test_L298N_Forward(void)
{
    L298N_Init();
    L298N_SetMotorA(L298N_FORWARD, 50);
    L298N_SetMotorB(L298N_FORWARD, 50);
}

void Test_L298N_Backward(void)
{
    L298N_Init();
    L298N_SetMotorA(L298N_BACKWARD, 50);
    L298N_SetMotorB(L298N_BACKWARD, 50);
}

void Test_L298N_Stop(void)
{
    L298N_Init();
    L298N_SetMotorA(L298N_STOP, 0);
    L298N_SetMotorB(L298N_STOP, 0);
}

void Test_L298N_Left(void)
{
    L298N_Init();
    L298N_SetMotorA(L298N_FORWARD, 30);
    L298N_SetMotorB(L298N_FORWARD, 70);
}

void Test_L298N_Right(void)
{
    L298N_Init();
    L298N_SetMotorA(L298N_FORWARD, 70);
    L298N_SetMotorB(L298N_FORWARD, 30);
}

void Test_L298N_Loop(void)
{
    L298N_Init();
    while(1)
    {
        L298N_SetMotorA(L298N_FORWARD, 50);
        L298N_SetMotorB(L298N_FORWARD, 50);
        Delay_Ms(2000);

        L298N_SetMotorA(L298N_STOP, 0);
        L298N_SetMotorB(L298N_STOP, 0);
        Delay_Ms(1000);

        L298N_SetMotorA(L298N_BACKWARD, 50);
        L298N_SetMotorB(L298N_BACKWARD, 50);
        Delay_Ms(2000);

        L298N_SetMotorA(L298N_STOP, 0);
        L298N_SetMotorB(L298N_STOP, 0);
        Delay_Ms(1000);
    }
}