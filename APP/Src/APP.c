/*
  ******************************************************************************
  * @file    APP.c
  * @author  xwl
  * @version V1.0.0
  * @date    2021  
  * @brief   GPIO demo source code.
  ******************************************************************************
*/
#include "APP.h"

GPIO_InitTypeDef GPIOA_Handle;
GPIO_InitTypeDef GPIOC_Handle;  

volatile uint32_t gu32_GPIOIRQ_Flag = false; 

void GPIO_IRQ_User_Function(void)
{
    gu32_GPIOIRQ_Flag = true;   
}

/*********************************************************************************
* Function    : GPIOAB_IRQHandler
* Description : 
* Input       : 
* Outpu       : 
* Author      : Chris_Kyle                         Date : 2021
**********************************************************************************/
void GPIOAB_IRQHandler(void)
{
    HAL_GPIO_IRQHandler(GPIOA, GPIO_PIN_0);

    GPIO_IRQ_User_Function();
}

/*********************************************************************************
* Function    : GPIOCD_IRQHandler
* Description : 
* Input       : 
* Outpu       : 
* Author      : Chris_Kyle                         Date : 2021  
**********************************************************************************/
void GPIOCD_IRQHandler(void)
{
    HAL_GPIO_IRQHandler(GPIOC, GPIO_PIN_13);
    
    GPIO_IRQ_User_Function();
}

/*********************************************************************************
* Function    : APP_GPIO_Test
* Description : 
* Input       : 
* Outpu       : 
* Author      : xwl                        Date : 2021  
**********************************************************************************/
void APP_GPIO_Test(enum_Mode_t fe_Mode)
{
    switch (fe_Mode)
    {     
        case GPIO_INT: 
        {
            printfS("This is GPIO interrupt TEST \r\n");
            
            GPIOA_Handle.Pin       = GPIO_PIN_0;
            GPIOA_Handle.Mode      = GPIO_MODE_IT_FALLING;
            GPIOA_Handle.Pull      = GPIO_PULLUP;
            GPIOA_Handle.Alternate = GPIO_FUNCTION_0;

            HAL_GPIO_Init(GPIOA, &GPIOA_Handle);
        
            /* Clear Pending Interrupt */
            NVIC_ClearPendingIRQ(GPIOAB_IRQn);
            
            /* Enable External Interrupt */
            NVIC_EnableIRQ(GPIOAB_IRQn);
            
            while (1) 
            {
                if (gu32_GPIOIRQ_Flag) 
                {
                    gu32_GPIOIRQ_Flag = false;
                    
                    printfS("Get interrupt flag!!! \r\n");
                }
            }
        }

        /* 注意：配置 GPIO PC13、PC14、PC15 管脚的数模、上下拉、驱动能力 使用PMU 域寄存器配置 */
        case GPIO_PC13:
        {
            printfS("This is GPIO PC13 interrupt TEST \r\n");

            GPIOC_Handle.Pin       = GPIO_PIN_13;
            GPIOC_Handle.Mode      = GPIO_MODE_IT_FALLING;
            GPIOC_Handle.Alternate = GPIO_FUNCTION_0;

            HAL_GPIO_Init(GPIOC, &GPIOC_Handle);

            /* RTC access enable */
            System_Enable_Disable_RTC_Domain_Access(FUNC_ENABLE);
            
            __HAL_RTC_PC13_SEL(0);  // GPIO function     
            __HAL_RTC_PC13_PULL_UP_ENABLE();
            __HAL_RTC_PC13_DIGIT();

            /* Clear Pending Interrupt */
            NVIC_ClearPendingIRQ(GPIOCD_IRQn);
            
            /* Enable External Interrupt */
            NVIC_EnableIRQ(GPIOCD_IRQn);
            
            while (1) 
            {
                if (gu32_GPIOIRQ_Flag) 
                {
                    gu32_GPIOIRQ_Flag = false;
                    
                    printfS("Get PC13 interrupt flag!!! \r\n");
                }
            }
        }
        
        case LED_BLINK: // use PA1 to drive LED 
        {
            printfS("This is LED Blinking TEST \r\n");
            
            GPIOA_Handle.Pin       = GPIO_PIN_1;
            GPIOA_Handle.Mode      = GPIO_MODE_OUTPUT_PP;
            GPIOA_Handle.Pull      = GPIO_PULLUP;
            GPIOA_Handle.Alternate = GPIO_FUNCTION_0;

            HAL_GPIO_Init(GPIOA, &GPIOA_Handle);

            while (1) 
            {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
                System_Delay_MS(500);
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_CLEAR);
                System_Delay_MS(500);
            }
        }

        default: break; 
    }
}
