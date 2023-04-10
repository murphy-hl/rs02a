#ifndef _MAIN_H
#define _MAIN_H

#include "ACM32Fxx_HAL.h"

typedef		unsigned 		char  		u8;
typedef		unsigned short 	int			u16;
typedef		unsigned 	   	int  		u32;
		
#define		READ_EARTH		(earth_timer[0]|earth_timer[1]|earth_timer[2]|earth_timer[3]|earth_timer[4])

#define		SLEEP_TIME		10

typedef struct {
	float voltage;
	float current;
	float leak_current;
	float power;
	float energy;
	float Mtemp;
	float Ptemp;
	float Fre;
	float power_cal;
}Electric_data_typedef;

extern uint32_t adc_value[10];

extern uint8_t work_mode;
extern volatile uint32_t TICK;
extern uint8_t menu_level;
extern uint8_t menu_one;
extern uint8_t count_100s;
extern uint32_t reser_time;

extern uint8_t refresh_screen;
extern Electric_data_typedef Realdata;


void led_blink(uint8_t mode);
void key_handle(uint8_t key_val);

void Readpara(void);
void TimeGetDate(uint16_t min);

#endif


