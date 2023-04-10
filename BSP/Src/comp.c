#include "comp.h"

COMP_HandleTypeDef COMP1_Handle;
COMP_HandleTypeDef COMP2_Handle;

/************************************************************************
 * function   : COMP_Init
 * Description: COMP Initiation. 
 ************************************************************************/ 
void COMP_Init(void)
{    
    
    /* COMP1 use the GPIO as VINM*/
    COMP1_Handle.Instance = COMP;
    COMP1_Handle.Init.Comparator = COMP1;
    
    COMP1_Handle.Init.InPSel = COMP_CR_INPSEL_0;
    COMP1_Handle.Init.InMSel = COMP_CR_INMSEL_0;
    
    HAL_COMP_Init(&COMP1_Handle); 

    /* COMP2 use the VDDA Crv as VINM*/
    COMP2_Handle.Instance = COMP;
    COMP2_Handle.Init.Comparator = COMP2;
    
    COMP2_Handle.Init.InPSel = COMP_CR_INPSEL_0;
    COMP2_Handle.Init.InMSel = COMP_CR_INMSEL_0;
    
    HAL_COMP_Init(&COMP2_Handle); 
}
void HAL_COMP_MspInit(COMP_HandleTypeDef* hcomp)
{
    /* 
      NOTE : This function should be modified by the user.
    */
    
    /* For Example */
    GPIO_InitTypeDef GPIO_Handle; 
    
    System_Module_Enable(EN_COMP);
    
    if(hcomp->Init.Comparator == COMP1 )
    {
        /* COMP1 GPIO inition VINP:PA5(INP_0)*/
        /* COMP1 GPIO inition VINM:PA4(INM_0)*/
        /* COMP1 GPIO inition VOUT:PA0(FUNCTION_7)*/       
        GPIO_Handle.Pin            = GPIO_PIN_4 | GPIO_PIN_5;
        GPIO_Handle.Mode           = GPIO_MODE_ANALOG;
        GPIO_Handle.Pull           = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_Handle);
        
        GPIO_Handle.Pin            = GPIO_PIN_0;
        GPIO_Handle.Mode           = GPIO_MODE_AF_PP;
        GPIO_Handle.Alternate      = GPIO_FUNCTION_7;
        GPIO_Handle.Pull           = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_Handle);            
    }
    else if(hcomp->Init.Comparator == COMP2 )
    {
        /* COMP2 GPIO inition VINP:PB4(INP_1)*/
        /* COMP2 GPIO inition VOUT:PA2(FUNCTION_7)*/
        GPIO_Handle.Pin            = GPIO_PIN_4|GPIO_PIN_3;
        GPIO_Handle.Mode           = GPIO_MODE_ANALOG;
        GPIO_Handle.Pull           = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOB, &GPIO_Handle);
        
        GPIO_Handle.Pin            = GPIO_PIN_5;
        GPIO_Handle.Mode           = GPIO_MODE_AF_PP;
        GPIO_Handle.Alternate      = GPIO_FUNCTION_7;
        GPIO_Handle.Pull           = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOB, &GPIO_Handle);
    }
}
void HAL_COMP_MspDeInit(COMP_HandleTypeDef* hcomp)
{
    /* 
      NOTE : This function should be modified by the user.
    */
    
    /* For Example */
    System_Module_Reset(RST_COMP);
    System_Module_Enable(EN_COMP);
}

uint8_t comp_get_level(uint8_t comp)
{
	uint8_t level=0;
	if(comp==1)
	{
		HAL_COMP_GetOutputLevel(&COMP1_Handle);
		level=COMP1_Handle.OutputLevel;
	}
	if(comp==2)
	{
		HAL_COMP_GetOutputLevel(&COMP2_Handle);
		level=COMP2_Handle.OutputLevel;
	}
	
	return level;	
}



