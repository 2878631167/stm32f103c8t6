#include "Bsp.h"
#include "app_car.h"
#include "app_test.h"

/**
 * @brief 系统初始化
 */
static void System_Init(void)
{
    /* 硬件初始化 */
    SystemClock_Config();
    SysTick_Init();      /* 初始化 SysTick 定时器 */
    LED_Init();
    OLED_Init();
    BT_Init();
    L298N_Init();
    Track_Init();
    Delay_Init();
    
    /* 应用层初始化 */
    Car_Init();        /* 小车应用初始化 */
    Test_Init();       /* 测试框架初始化 */
}

/**
 * @brief 主函数
 */
int main(void)
{
    /* 1. 系统初始化 */
    System_Init();
    
    /* 2. 启动画面 */
    OLED_Clear();
    OLED_ShowString(0, 0, "Car System Ready", OLED_6X8);
    OLED_Update();
    BT_SendString("=== Car System Started ===\r\n");
    Delay_Ms(1500);
    
    /* 3. 主循环 */
    while(1)
    {
        /* 3.1 处理蓝牙命令 */
        if (BT_RxFlag == 1)
        {
            BT_ProcessPacket();
            Test_HandleCommand();  /* 测试命令处理 */
        }
        
        /* 3.2 运行应用逻辑 */
        Car_Run();     /* 小车业务逻辑（循迹、控制等） */
        
        /* 3.3 更新显示 */
        Car_UpdateDisplay();
        
        /* 3.4 延时（控制循环频率 ~100Hz） */
        Delay_Ms(10);
    }
}