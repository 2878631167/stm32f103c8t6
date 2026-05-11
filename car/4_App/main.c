#include "Bsp.h"

void Bsp_InitAll(void)
{
    SystemClock_Config();
    LED_Init();
    OLED_Init();
    BT_Init();
    L298N_Init();
    Track_Init();
    Delay_Init();
}

void Bsp_TestAll(void)
{
    static uint8_t test_step = 0;

    test_step++;
    if (test_step > 5) test_step = 1;

    OLED_Clear();
    switch(test_step)
    {
        case 1:
            LED_On();
            OLED_ShowString(1, 1, "Test: LED");
            OLED_ShowString(2, 1, "LED ON");
            L298N_SetMotorA(L298N_STOP, 0);
            L298N_SetMotorB(L298N_STOP, 0);
            break;
        case 2:
            LED_Off();
            OLED_ShowString(1, 1, "Test: LED");
            OLED_ShowString(2, 1, "LED OFF");
            L298N_SetMotorA(L298N_STOP, 0);
            L298N_SetMotorB(L298N_STOP, 0);
            break;
        case 3:
            OLED_ShowString(1, 1, "Test: L298N");
            OLED_ShowString(2, 1, "Motor Forward");
            L298N_SetMotorA(L298N_FORWARD, 50);
            L298N_SetMotorB(L298N_FORWARD, 50);
            break;
        case 4:
            OLED_ShowString(1, 1, "Test: L298N");
            OLED_ShowString(2, 1, "Motor Back");
            L298N_SetMotorA(L298N_BACKWARD, 50);
            L298N_SetMotorB(L298N_BACKWARD, 50);
            break;
        case 5:
            OLED_ShowString(1, 1, "Test: Track");
            OLED_ShowNum(2, 1, Track_Read(0), 1);
            OLED_ShowNum(2, 3, Track_Read(1), 1);
            OLED_ShowNum(2, 5, Track_Read(2), 1);
            OLED_ShowNum(2, 7, Track_Read(3), 1);
            OLED_ShowNum(2, 9, Track_Read(4), 1);
            L298N_SetMotorA(L298N_STOP, 0);
            L298N_SetMotorB(L298N_STOP, 0);
            break;
        default:
            break;
    }

    BT_SendString("Test Step: ");
    BT_SendByte('0' + test_step);
    BT_SendString("\r\n");
}

int main(void)
{
    Bsp_InitAll();

    OLED_Clear();
    OLED_ShowString(1, 1, "STM32 BSP Test");
    OLED_ShowString(2, 1, "Starting...");
    Delay_Ms(1500);

    BT_SendString("=== BSP Test Start ===\r\n");

    OLED_Clear();
    OLED_ShowString(1, 1, "Test: LED");
    OLED_ShowString(2, 1, "LED ON");

    LED_On();
    Delay_Ms(500);
    LED_Off();
    Delay_Ms(500);
    LED_On();
    Delay_Ms(500);
    LED_Off();

    OLED_ShowString(2, 1, "LED OK! ");
    BT_SendString("LED Test OK\r\n");
    Delay_Ms(1000);

    OLED_Clear();
    OLED_ShowString(1, 1, "Test: OLED");
    OLED_ShowString(2, 1, "Display OK!");
    BT_SendString("OLED Test OK\r\n");
    Delay_Ms(1000);

    OLED_Clear();
    OLED_ShowString(1, 1, "Test: L298N");
    OLED_ShowString(2, 1, "Motor Test...");
    L298N_SetMotorA(L298N_FORWARD, 80);
    L298N_SetMotorB(L298N_FORWARD, 80);
    Delay_Ms(1000);
    L298N_SetMotorA(L298N_STOP, 0);
    L298N_SetMotorB(L298N_STOP, 0);
    Delay_Ms(500);
    L298N_SetMotorA(L298N_BACKWARD, 80);
    L298N_SetMotorB(L298N_BACKWARD, 80);
    Delay_Ms(1000);
    L298N_SetMotorA(L298N_STOP, 0);
    L298N_SetMotorB(L298N_STOP, 0);
    OLED_ShowString(2, 1, "L298N OK!");
    BT_SendString("L298N Test OK\r\n");
    Delay_Ms(1000);

    OLED_Clear();
    OLED_ShowString(1, 1, "Test: Track");
    OLED_ShowNum(2, 1, Track_Read(0), 1);
    OLED_ShowNum(2, 3, Track_Read(1), 1);
    OLED_ShowNum(2, 5, Track_Read(2), 1);
    OLED_ShowNum(2, 7, Track_Read(3), 1);
    OLED_ShowNum(2, 9, Track_Read(4), 1);
    BT_SendString("Track Test OK\r\n");
    Delay_Ms(1000);

    OLED_Clear();
    OLED_ShowString(1, 1, "Test: BT");
    OLED_ShowString(2, 1, "RX Mode");
    BT_SendString("BT Test OK - Enter loop\r\n");
    Delay_Ms(1000);

    while(1)
    {
        Bsp_TestAll();
        Delay_Ms(2000);

        if (BT_RxFlag == 1)
        {
            BT_RxFlag = 0;
            OLED_Clear();
            OLED_ShowString(1, 1, "RX:");
            OLED_ShowString(2, 1, (char*)BT_RxPacket);
        }
    }
}