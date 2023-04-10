#ifndef _SPI_H
#define _SPI_H

#include "main.h"

extern SPI_HandleTypeDef hspi2;
extern DMA_HandleTypeDef DMA_SPIT_Handle;


void SPI_Init(void);

#endif

