#ifndef _PWM_H
#define _PWM_H

#include "main.h"

extern TIM_HandleTypeDef htim3;

void PWM_init(uint16_t duty);

void TIM3_MSP_Post_Init(void);
	
void TIM3_PWM_Init(void);
	
	
void PWM_set_duty(uint16_t duty);
	
#endif 


