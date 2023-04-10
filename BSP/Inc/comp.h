#ifndef _COMP_H
#define _COMP_H

#include "main.h"


extern COMP_HandleTypeDef COMP1_Handle;
extern COMP_HandleTypeDef COMP2_Handle;


void COMP_Init(void);
uint8_t comp_get_level(uint8_t comp);


#endif



