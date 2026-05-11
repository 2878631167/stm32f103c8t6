#include "main.h"
#include "app.h"

void Delay_ms(uint32_t ms);

int main(void)
{
  SystemInit();
  
  App_Init();
  
  while (1)
  {
    App_MainTask();
    Delay_ms(500);
  }
}

void Delay_ms(uint32_t ms)
{
  uint32_t i, j;
  for (i = 0; i < ms; i++)
  {
    for (j = 0; j < 7200; j++);
  }
}