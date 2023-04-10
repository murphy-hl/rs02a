#ifndef _EXIT_H
#define _EXIT_H

#include "main.h"

extern ADC_HandleTypeDef ADC_Handle;
extern DMA_HandleTypeDef  Dma_Adc_Handle;


extern uint32_t adc_buffer[2];


void adc_init(void);

float get_temp_mcu(void);
float get_temp_plug(void);

uint8_t ADC_DMA_Convert(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length);

#endif 



