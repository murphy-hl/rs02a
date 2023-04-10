/*
  ******************************************************************************
  * @file    APP.h
  * @author  Chris_Kyle
  * @version V1.0.0
  * @date    2020
  * @brief   GPIO demo Header file.
  ******************************************************************************
*/
#ifndef __APP_H__
#define __APP_H__

#include "ACM32Fxx_HAL.h"

typedef enum
{
    GPIO_OUTPUT,
    GPIO_INT,
    GPIO_PC13,
    LED_BLINK,
}enum_Mode_t;

/* APP_GPIO_Test */
void APP_GPIO_Test(enum_Mode_t fe_Mode);

#endif
