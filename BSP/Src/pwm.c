#include "pwm.h"

TIM_HandleTypeDef htim3;   

/*1khz PWM*/
void PWM_init(uint16_t duty)
{
	TIM_OC_InitTypeDef Tim_OC_Init_Para;   

	htim3.Instance = TIM3;
	htim3.Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;        
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP; 
	htim3.Init.RepetitionCounter = 0;  
	htim3.Init.Prescaler = 64 - 1; 
	htim3.Init.Period = 999;  // period = 10us      
	
	System_Module_Reset(RST_TIM3); 
	System_Module_Enable(EN_TIM3); 
	NVIC_ClearPendingIRQ(TIM3_IRQn);     
	NVIC_EnableIRQ(TIM3_IRQn);        
	
	HAL_TIMER_Base_Init(&htim3);  
	
	Tim_OC_Init_Para.OCMode = OUTPUT_MODE_PWM1;  
	Tim_OC_Init_Para.OCIdleState = OUTPUT_IDLE_STATE_0;
	Tim_OC_Init_Para.OCNIdleState = OUTPUT_IDLE_STATE_0;      
	Tim_OC_Init_Para.OCPolarity = OUTPUT_POL_ACTIVE_HIGH;  
	Tim_OC_Init_Para.OCNPolarity = OUTPUT_POL_ACTIVE_HIGH;    
	Tim_OC_Init_Para.OCFastMode =  OUTPUT_FAST_MODE_DISABLE;  
	Tim_OC_Init_Para.Pulse = duty;   // 50% duty cycle   	
	HAL_TIMER_Output_Config(htim3.Instance, &Tim_OC_Init_Para, TIM_CHANNEL_2);    
    
	TIM3_MSP_Post_Init();   
	HAL_TIM_PWM_Output_Start(htim3.Instance, TIM_CHANNEL_2);  
	
}

void TIM3_MSP_Post_Init(void)     
{
	GPIO_InitTypeDef gpio_init; 
	
	gpio_init.Pin = GPIO_PIN_7;  //TIM1_CH1  
	gpio_init.Mode = GPIO_MODE_AF_PP;          
	gpio_init.Pull = GPIO_NOPULL;    
	gpio_init.Alternate = GPIO_FUNCTION_2;         
	HAL_GPIO_Init(GPIOA, &gpio_init);    // PC8   	  
}  
void TIM3_PWM_Init(void)  
{
	GPIO_InitTypeDef gpio_init; 
	
	gpio_init.Pin = GPIO_PIN_7;  //TIM1_CH1  
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;          
	gpio_init.Pull = GPIO_NOPULL;    
	gpio_init.Alternate = GPIO_FUNCTION_0;         
	HAL_GPIO_Init(GPIOA, &gpio_init);    // PC8 
	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_CLEAR);
}
void PWM_set_duty(uint16_t duty)
{
	if(duty==0)
	{
		TIM3_PWM_Init();
	}else{
		PWM_init(duty);
//		htim3.Instance->CCR2 =duty;
	}
	
}
