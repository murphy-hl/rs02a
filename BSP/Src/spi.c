#include "spi.h"

SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef DMA_SPIT_Handle;

void SPI_Init(void)
{
    hspi2.Instance                 = SPI2;
    hspi2.Init.SPI_Mode            = SPI_MODE_MASTER;
    hspi2.Init.SPI_Work_Mode       = SPI_WORK_MODE_0;
    hspi2.Init.X_Mode              = SPI_1X_MODE;
    hspi2.Init.First_Bit           = SPI_FIRSTBIT_MSB;
    hspi2.Init.BaudRate_Prescaler  = SPI_BAUDRATE_PRESCALER_4;

    HAL_SPI_Init(&hspi2);

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(spiHandle->Instance==SPI2)
	{
		System_Module_Enable(EN_SPI2);
		
		GPIO_InitStruct.Pin            = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
        GPIO_InitStruct.Mode           = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull           = GPIO_PULLUP;
        GPIO_InitStruct.Alternate      = GPIO_FUNCTION_4;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(SPI2_IRQn);
        /* Enable External Interrupt */
        NVIC_EnableIRQ(SPI2_IRQn);
	}
}
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{
	if(spiHandle->Instance==SPI2)
	{
		System_Module_Disable(EN_SPI2);
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
		
		NVIC_ClearPendingIRQ(SPI2_IRQn);
		/* Disable External Interrupt */
		NVIC_DisableIRQ(SPI2_IRQn);
	}
}
void SPI2_IRQHandler(void)
{
    HAL_SPI_IRQHandler(&hspi2);
}

void DMA_SPITransmit_Init(void)
{
    
}





