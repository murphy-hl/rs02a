#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "main.h"
#include "rtc.h"


void display_currentmode(uint8_t mode);
void display_reservemode(void);
void display_reservesuccess(void);
void display_chargerror(void);
void display_statusbar(void);
void display_minstartcharge(uint16_t min);
void display_timestartcharge(Date_Typedef date,Time_Typedef time);


void display_realV(float vol);
void display_realI(float current);
void display_realP(float power);
void display_realE(float energy);
void display_mcuT(float temp);
void display_plugT(float temp);

void display_charging(void);
void display_waitcharg(void);

void display_pic_logo(void);
void display_pic_car(void);
void display_pic_noearth(void);
void display_pic_overtemp(void);
void display_pic_leak(void);

void display_main(void);

void display_set_calender(uint16_t x,uint16_t y);
	
#endif  



