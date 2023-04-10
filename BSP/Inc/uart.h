#ifndef _UART_H
#define _UART_H

#include "ACM32Fxx_HAL.h"


extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

void Uart1_Init(void);
void Uart2_Init(void);


void HAL_UART_MspInit(UART_HandleTypeDef *huart);
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart);

#endif


