#include "bsp.h"


EXTI_HandleTypeDef hexit8;
EXTI_HandleTypeDef hexit11;
EXTI_HandleTypeDef hexit12;

extern uint32_t Timer_Update_Flag;

void bsp_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Alternate = GPIO_FUNCTION_0;
	/*¿ª»ú¼ì²â*/
	GPIO_InitStruct.Pin = L_CTL_PIN;
	HAL_GPIO_Init(L_CTL_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = N_CTL_PIN;
	HAL_GPIO_Init(N_CTL_PORT, &GPIO_InitStruct);
	
	
	GPIO_InitStruct.Pin = LEDB_PIN;
	HAL_GPIO_Init(LEDB_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = LEDG_PIN;
	HAL_GPIO_Init(LEDG_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = LEDR1_PIN;
	HAL_GPIO_Init(LEDR1_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = LEDR2_PIN;
	HAL_GPIO_Init(LEDR2_PORT, &GPIO_InitStruct);
	
	LEDB_OFF;
	LEDG_OFF;
	LEDR1_OFF;
	LEDR2_OFF;
	
	/******************************ÊäÈë*********************************************/
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Alternate = GPIO_FUNCTION_0;
	GPIO_InitStruct.Pin = EARTH_ACT_PIN;
	HAL_GPIO_Init(EARTH_ACT_PORT, &GPIO_InitStruct);
	
	hexit12.u32_Line    = EXTI_LINE_12;
	hexit12.u32_Mode    = EXTI_MODE_INTERRUPT;
	hexit12.u32_Trigger = EXTI_TRIGGER_FALLING;    
	hexit12.u32_GPIOSel = EXTI_GPIOA;

	HAL_EXTI_SetConfigLine(&hexit12);
	
	/**********************************ÖÐ¶Ï*********************************/
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	
	
	GPIO_InitStruct.Pin = ENERGY_INT1_PIN;
	HAL_GPIO_Init(ENERGY_INT1_PORT, &GPIO_InitStruct);
	hexit8.u32_Line    = EXTI_LINE_8;
	hexit8.u32_Mode    = EXTI_MODE_INTERRUPT;
	hexit8.u32_Trigger = EXTI_TRIGGER_FALLING;    
	hexit8.u32_GPIOSel = EXTI_GPIOA;

	HAL_EXTI_SetConfigLine(&hexit8);
	
	GPIO_InitStruct.Pin = ENERGY_INT2_PIN;
	HAL_GPIO_Init(ENERGY_INT2_PORT, &GPIO_InitStruct);
	hexit11.u32_Line    = EXTI_LINE_11;
	hexit11.u32_Mode    = EXTI_MODE_INTERRUPT;
	hexit11.u32_Trigger = EXTI_TRIGGER_FALLING;    
	hexit11.u32_GPIOSel = EXTI_GPIOA;

	HAL_EXTI_SetConfigLine(&hexit11);
	
	
}

//void led_blink(uint8_t mode)
//{
//	switch(mode)
//	{
//		
//		case 1: //Õ¼¿Õ±È86.67% ÂÌµÆÂýÉÁ
//			LEDB_OFF;
//			if(Timer_Update_Flag >999)
//			{
//				LEDG_ON;
//			}else{
//				LEDG_OFF;
//			}
//			break;
//		case 2:	 //Õ¼¿Õ±È73.33%£¬ÂÌµÆ¿ìÉÁ
//			LEDB_OFF;
//			if(Timer_Update_Flag%500> 249)
//			{
//				LEDG_ON;
//			}else{
//				LEDG_OFF;
//			}
//			break;
//		case 3: //Õ¼¿Õ±È60%£¬À¶µÆÂýÉÁ
//			LEDG_OFF;
//			if(Timer_Update_Flag >999)
//			{
//				LEDB_ON;
//			}else{
//				LEDB_OFF;
//			}
//			break;
//		case 4:	//Õ¼¿Õ±È46.67%£¬À¶µÆ¿ìÉÁ
//			LEDG_OFF;
//			if(Timer_Update_Flag%500> 249)
//			{
//				LEDB_ON;
//			}else{
//				LEDB_OFF;
//			}
//			break;
//		default:
//			LEDB_ON;
//			LEDG_ON;
//			break;
//	}
//}

