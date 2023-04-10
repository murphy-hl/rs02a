#include "timer.h"
#include "bsp.h"


TIM_HandleTypeDef htim6;  
TIM_HandleTypeDef htim14;

uint8_t INT_1S=0;
volatile uint32_t TICK;    
uint32_t Timer_Update_Flag;    

uint8_t refresh_screen=0;
void TIM14_Init(void)  
{
	htim14.Instance = TIM14;
	htim14.Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;    
	htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	htim14.Init.CounterMode = TIM_COUNTERMODE_UP; 
	htim14.Init.RepetitionCounter = 0;  
	htim14.Init.Prescaler = 63;  
	htim14.Init.Period = 999;  
	
	HAL_TIMER_MSP_Init(&htim14);       
	HAL_TIMER_Base_Init(&htim14);  
	HAL_TIM_ENABLE_IT(&htim14, TIMER_INT_EN_UPD); 

	
	HAL_TIMER_Base_Start(htim14.Instance);            
}


void TIM6_Init(void)  
{
	htim6.Instance = TIM6;
	htim6.Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;    
	htim6.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP; 
	htim6.Init.RepetitionCounter = 0;  
	htim6.Init.Prescaler = 6399;  
	htim6.Init.Period = 9999;  
	
	HAL_TIMER_MSP_Init(&htim6);       
	HAL_TIMER_Base_Init(&htim6);  
	HAL_TIM_ENABLE_IT(&htim6, TIMER_INT_EN_UPD); 

	HAL_TIMER_Base_Start(htim6.Instance);            
}


void TIM14_IRQHandler(void)
{
	if (TIM14->SR & TIMER_SR_UIF)
	{
		TICK++;
		Timer_Update_Flag ++;  
		if(Timer_Update_Flag>999)
		{
			Timer_Update_Flag=0;
		}
	}
	TIM14->SR = 0;   //write 0 to clear hardware flag  
}

uint8_t count_100s=0;

void TIM6_IRQHandler(void)
{
	if (TIM6->SR & TIMER_SR_UIF)
	{
		TIM6_update_cb();
	}
	TIM6->SR = 0;   //write 0 to clear hardware flag  
}

extern uint8_t bmp_flag[2];
void  TIM6_update_cb(void)
{
	count_100s++;
	if(reser_time!=0)
	{
		reser_time--;
		if(reser_time==0)
		{
			menu_level=0;
			bmp_flag[1]=5;
		}
	}
	
	if(refresh_screen!=0)
	{
		refresh_screen--;
	}
	INT_1S++;
}





