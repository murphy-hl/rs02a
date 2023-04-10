#ifndef _AUDIO_H
#define _AUDIO_H

#include "main.h"

#define		AUDIO_BUSY_PIN		GPIO_PIN_7
#define		AUDIO_BUSY_PORT		GPIOC

#define		AUDIO_DATA_PIN		GPIO_PIN_8
#define		AUDIO_DATA_PORT		GPIOC


#define 	DATA_H				HAL_GPIO_WritePin(AUDIO_DATA_PORT,AUDIO_DATA_PIN,GPIO_PIN_SET)
#define  	DATA_L				HAL_GPIO_WritePin(AUDIO_DATA_PORT,AUDIO_DATA_PIN,GPIO_PIN_CLEAR)

#define		READ_BUSY			HAL_GPIO_ReadPin(AUDIO_BUSY_PORT,AUDIO_BUSY_PIN)

#define		AUDIO_START			0xFF
#define		AUDIO_STOP			0xFE
#define		AUDIO_PAUSE			0xFD
#define		AUDIO_volume0		0xE0
#define		AUDIO_volume1		0xE1
#define		AUDIO_volume2		0xE2
#define		AUDIO_volume3		0xE3
#define		AUDIO_volume4		0xE4
#define		AUDIO_volume5		0xE5
#define		AUDIO_volume6		0xE6
#define		AUDIO_volume7		0xE7
#define		AUDIO_volume8		0xE8
#define		AUDIO_volume9		0xE9
#define		AUDIO_volume10		0xEA
#define		AUDIO_volume11		0xEB
#define		AUDIO_volume12		0xEC
#define		AUDIO_volume13		0xED
#define		AUDIO_volume14		0xEE
#define		AUDIO_volume15		0xEF

#define		AUDIO_tempover		0x0F
#define		AUDIO_leak			0x10
#define		AUDIO_volover		0x11
#define		AUDIO_resmode		0x0C
#define		AUDIO_ressuccess	0x0D
#define		AUDIO_rescancel		0x0E
#define		AUDIO_charging		0x05

void audio_init(void);
void play_audio(uint8_t cmd);

void audio_start(void);
void audio_data1(void);
void audio_data0(void);

#endif






