#include "bsp_usart.h"

void USARTx_Init(uint32_t baudrate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  RCC_APB2PeriphClockCmd(USARTx_RCC | USARTx_GPIO_RCC, ENABLE);

  GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(USARTx_GPIO, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(USARTx_GPIO, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USARTx, &USART_InitStructure);

  USART_Cmd(USARTx, ENABLE);
}

void USARTx_SendChar(uint8_t ch)
{
  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
  USART_SendData(USARTx, ch);
}

void USARTx_SendString(char* str)
{
  while (*str)
  {
    USARTx_SendChar(*str++);
  }
}

uint8_t USARTx_ReceiveChar(void)
{
  while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
  return USART_ReceiveData(USARTx);
}