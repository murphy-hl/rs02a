#include "uart.h"

#define MAX_REC_LEN			64

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;


uint8_t arxbuf[1]={0};
uint8_t uart2_Rxbuf[MAX_REC_LEN]={0};
uint8_t uart2_rxflag=0;

/*µçÁ¿¼Æ*/
void Uart1_Init(void)
{
    huart1.Instance = UART1;
    huart1.Init.BaudRate   = 9600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits   = UART_STOPBITS_1;
    huart1.Init.Parity     = UART_PARITY_EVEN;
    huart1.Init.Mode       = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl  = UART_HWCONTROL_NONE;

    HAL_UART_Init(&huart1);
	
}
/*À¶ÑÀ*/
void Uart2_Init(void)
{
    huart2.Instance = UART2;
    huart2.Init.BaudRate   = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits   = UART_STOPBITS_1;
    huart2.Init.Parity     = UART_PARITY_NONE;
    huart2.Init.Mode       = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl  = UART_HWCONTROL_NONE;

    HAL_UART_Init(&huart2);
	HAL_UART_Receive_IT(&huart2,uart2_Rxbuf,8,UART_RX_FIFO_1_2);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef    GPIO_InitStruct;
	if (huart->Instance == UART1) 
    {
        /* Enable Clock */
        System_Module_Enable(EN_UART1);
        
        /* Initialization GPIO */
        /* A9:Tx  A10:Rx */
        GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Alternate = GPIO_FUNCTION_2;
        
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        /* NVIC Config */
        NVIC_ClearPendingIRQ(UART1_IRQn);
        NVIC_SetPriority(UART1_IRQn, (1 << __NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(UART1_IRQn);
		
    }else if(huart->Instance == UART2) 
	{
		/* Enable Clock */
        System_Module_Enable(EN_UART2);
        
        /* Initialization GPIO */
        /* A9:Tx  A10:Rx */
        GPIO_InitStruct.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Alternate = GPIO_FUNCTION_2;
        
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        /* NVIC Config */
        NVIC_ClearPendingIRQ(UART2_IRQn);
        NVIC_SetPriority(UART2_IRQn, (1 << __NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(UART2_IRQn);
		
	}
}
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    if (huart->Instance == UART1) 
    {
        /* Disable Clock */
        System_Module_Disable(EN_UART1);

        /* DeInitialization GPIO */
        /* A9:Tx  A10:Rx */
        HAL_GPIO_DeInit(GPIOA,GPIO_PIN_9 | GPIO_PIN_10);
        /* NVIC DeInit */
        NVIC_DisableIRQ(UART1_IRQn);	
    }
	else if(huart->Instance == UART2)
	{
		/* Disable Clock */
        System_Module_Disable(EN_UART2);
        /* DeInitialization GPIO */
        /* A9:Tx  A10:Rx */
        HAL_GPIO_DeInit(GPIOA,GPIO_PIN_2 | GPIO_PIN_3);
        /* NVIC DeInit */
        NVIC_DisableIRQ(UART2_IRQn);
	}
}

void UART1_IRQHandler()
{
	HAL_UART_IRQHandler(&huart1);
	
}
void UART2_IRQHandler()
{
	HAL_UART_IRQHandler(&huart2);
	HAL_UART_Receive_IT(&huart2,uart2_Rxbuf,8,UART_RX_FIFO_1_2);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==UART1)
	{
		
		
	}else if(huart->Instance==UART2)
	{
		uart2_rxflag=1;
	}
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==UART1)
	{
		
		
	}else if(huart->Instance==UART2)
	{
		
		
	}
}







