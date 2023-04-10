#ifndef _KEY_H
#define _KEY_H

#include "main.h"


#define		KEY_TIME_PIN		GPIO_PIN_1
#define		KEY_TIME_PORT		GPIOB

#define		KEY_MODE_PIN		GPIO_PIN_0
#define		KEY_MODE_PORT		GPIOB


#define		READ_KEY_TIME			HAL_GPIO_ReadPin(KEY_TIME_PORT,KEY_TIME_PIN)
#define		READ_KEY_MODE			HAL_GPIO_ReadPin(KEY_MODE_PORT,KEY_MODE_PIN)

typedef enum key_val{
	KEY_NULL=0,
	KEY_MODE_SHORT=1,
	KEY_MODE_LONG,
	KEY_MODE_DOUBLE,
	KEY_TIME_SHORT,
	KEY_TIME_LONG,
	KEY_TIME_DOUBLE,
	KEY_MT_SHORT,
	KEY_MT_LONG,
}typdef_keyval;





extern EXTI_HandleTypeDef hexit0;
extern EXTI_HandleTypeDef hexit1;

void key_Init(void);
typdef_keyval key_scan(void);

#endif



