#ifndef _TIMER_H
#define _TIMER_H

#include "main.h"

extern TIM_HandleTypeDef htim6;  
extern TIM_HandleTypeDef htim14;


void TIM14_Init(void) ;
void TIM6_Init(void);

void  TIM6_update_cb(void);

#endif 


