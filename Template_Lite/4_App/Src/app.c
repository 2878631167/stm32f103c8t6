#include "app.h"
#include "bsp_led.h"
#include "bsp_usart.h"

void App_Init(void)
{
  LED_Init();
  USARTx_Init(115200);
}

void App_MainTask(void)
{
  LED_Toggle();
  USARTx_SendString("Hello STM32F103C8T6!\r\n");
}