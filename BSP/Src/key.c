#include "key.h"
#include "delay.h"

EXTI_HandleTypeDef hexit0;
EXTI_HandleTypeDef hexit1;


void key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Alternate = GPIO_FUNCTION_0;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	
	GPIO_InitStruct.Pin = KEY_TIME_PIN;
	HAL_GPIO_Init(KEY_TIME_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = KEY_MODE_PIN;
	HAL_GPIO_Init(KEY_MODE_PORT, &GPIO_InitStruct);
	
	
	
//	hexit1.u32_Line    = EXTI_LINE_1;
//	hexit1.u32_Mode    = EXTI_MODE_INTERRUPT;
//	hexit1.u32_Trigger = EXTI_TRIGGER_FALLING;    
//	hexit1.u32_GPIOSel = EXTI_GPIOB;

//	HAL_EXTI_SetConfigLine(&hexit1);
//	
//	
//	hexit0.u32_Line    = EXTI_LINE_0;
//	hexit0.u32_Mode    = EXTI_MODE_INTERRUPT;
//	hexit0.u32_Trigger = EXTI_TRIGGER_FALLING;    
//	hexit0.u32_GPIOSel = EXTI_GPIOB;

//	HAL_EXTI_SetConfigLine(&hexit0);
	
	
}


typdef_keyval key_scan(void)
{
	uint16_t count1=0,count2=0;
	
//	uint16_t count0=0;
//	if((!READ_KEY_MODE)&&(!READ_KEY_TIME))
//	{
//		delay_ms(3);
//		while((!READ_KEY_MODE)&&(!READ_KEY_TIME)&&(count0<300))
//		{
//			delay_ms(10);
//			count0++;
//		}
//		if(count0<50)
//		{
//			return KEY_MT_SHORT;
//			
//		}else if(count0<350){
//			
//			return KEY_MT_LONG;
//		}else{
//			
//			return KEY_NULL;
//		}
//		
//		
//	}
	
	if(!READ_KEY_MODE)
	{
		delay_ms(3);
		while(!READ_KEY_MODE&&count1<300)
		{
			delay_ms(10);
			count1++;
			if((!READ_KEY_MODE)&&(!READ_KEY_TIME))
			{
				delay_ms(10);
				if((!READ_KEY_MODE)&&(!READ_KEY_TIME))
				{
					return KEY_MT_LONG;
				}
			}
		}
		if(count1<50)
		{
			return KEY_MODE_SHORT;
			
		}else if(count1<350){
			
			return KEY_MODE_LONG;
		}else{
			
			
			
		}
			

//		if((!READ_KEY_MODE)&&(!READ_KEY_TIME))
//		{
//			while((!READ_KEY_MODE)&&(!READ_KEY_TIME)&&(count0<300))
//			{
//				delay_ms(10);
//				count0++;
//			}
//			if(count0<50)
//			{
//				return KEY_MT_SHORT;
//				
//			}else if(count0<350){
//				
//				return KEY_MT_LONG;
//			}else{
//				
//				return KEY_NULL;
//			}
//			
//		}else{
//			
//		}
		
	}
	if(!READ_KEY_TIME)
	{
		delay_ms(3);
		while(!READ_KEY_TIME&&count2<200)
		{
			delay_ms(10);
			count2++;
		}
		if(count2<100)
		{
			return KEY_TIME_SHORT;
			
		}else {
			
			return KEY_TIME_LONG;
		}
	}
	return KEY_NULL;
}

extern typdef_keyval key_val;

extern uint16_t earth_count;

void EXTI_IRQHandler(void)
{    
//  HAL_EXTI_IRQHandler(&hexit0);
//	HAL_EXTI_IRQHandler(&hexit1);
//	HAL_EXTI_IRQHandler(&hexit8);
//	HAL_EXTI_IRQHandler(&hexit11);
	if (EXTI->PDR & EXTI_LINE_0) //模式按键
    {
        EXTI->PDR = EXTI_LINE_0;
		
    }else if(EXTI->PDR & EXTI_LINE_1) //预约时间按键
    {
        EXTI->PDR = EXTI_LINE_1;
		
    }else if(EXTI->PDR & EXTI_LINE_8) //漏电指示
    {
        EXTI->PDR = EXTI_LINE_8;
		
		
    }else if(EXTI->PDR & EXTI_LINE_11) //有效值更新
    {
        EXTI->PDR = EXTI_LINE_11;
		
		
    }else if(EXTI->PDR & EXTI_LINE_12) //有效值更新
    {
        EXTI->PDR = EXTI_LINE_12;
		earth_count++;
		
    }else{
		
	}
}



