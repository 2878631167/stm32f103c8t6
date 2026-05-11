#include "bsp.h"

void BSP_Init(void)
{
  LED_Init();
  USARTx_Init(115200);
}