#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "Board_Config.h"

void USARTx_Init(uint32_t baudrate);
void USARTx_SendChar(uint8_t ch);
void USARTx_SendString(char* str);
uint8_t USARTx_ReceiveChar(void);

#endif