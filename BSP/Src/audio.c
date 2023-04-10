#include "audio.h"
#include "delay.h"


void audio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Alternate = GPIO_FUNCTION_0;
	GPIO_InitStruct.Pin = AUDIO_DATA_PIN;
	HAL_GPIO_Init(AUDIO_DATA_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Alternate = GPIO_FUNCTION_0;
	GPIO_InitStruct.Pin = AUDIO_BUSY_PIN;
	HAL_GPIO_Init(AUDIO_BUSY_PORT, &GPIO_InitStruct);
	
	DATA_H;
	
}


void play_audio(uint8_t cmd)
{
	uint8_t temp=cmd;
	audio_start();
	for(uint8_t i=0;i<8;i++)
	{
		if(temp&0x80)
		{
			audio_data1();
		}else{
			audio_data0();
		}
		temp<<=1;
	}
}

void audio_start(void)
{
	DATA_L;
	delay_US(6000);
	DATA_H;
	delay_US(500);
}

void audio_data1(void)
{
	DATA_L;
	delay_US(500);
	DATA_H;
	delay_US(1500);
}


void audio_data0(void)
{
	DATA_L;
	delay_US(1500);
	DATA_H;
	delay_US(500);
}







	
	