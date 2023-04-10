#include "sys.h"

extern uint32_t gu32_SystemClock;
extern uint32_t gu32_APBClock;
extern uint32_t gu32_Clocksrc ;  
/*********************************************************************************
* Function    : System_Clock_Init
* Description : Clock init
* Input       : fu32_Clock: System core clock
* Outpu       : 0: success, other value: fail reason
* Author      : xwl                         Date : 2021  
**********************************************************************************/
bool System_Clock_config(uint32_t fu32_Clock)
{
    uint32_t lu32_DIV, lu32_system_clk_source, lu32_result, lu32_timeout;        

    lu32_system_clk_source = CLK_SRC_XTH_PLL;  
    
    SET_EFC_RD_WAIT(RD_WAIT_SET_DEFAULT)    
    
    switch (fu32_Clock)
    {
        /* 64MHz */
        case 64000000: lu32_DIV = 1; break;
            
        /* 32MHz */
        case 32000000: lu32_DIV = 2; break;
            
        /* 16MHz */
        case 16000000: lu32_DIV = 4; break;

        /* 8MHz */
        case 8000000:  lu32_DIV = 8; break;

        default: return false;
    }
    
    lu32_result = 0; 
    
    if (lu32_system_clk_source == CLK_SRC_XTH_PLL)   
    {
        lu32_timeout = 0;  
               
        SCU->XTHCR = SCU_XTHCR_XTH_EN /*| SCU_XTHCR_READYTIME_32768*/;
        while (0 == (SCU->XTHCR & SCU_XTHCR_XTHRDY))
        {     
            if (lu32_timeout == SYSTEM_TIMEOUT)
            {
                lu32_result = 1;  
                break;     
            }  
            lu32_timeout++;    
        } 
        
        if (0 == lu32_result)
        {
            SCU->PLLCR |=  SCU_PLLCR_PLL_EN;  
            SCU->PLLCR &= ~(SCU_PLLCR_PLL_SLEEP);   
            while(!(SCU->PLLCR & (SCU_PLLCR_PLL_FREE_RUN) )) {}  
                
#ifdef XTH_8M_CRYSTAL            
            SCU->PLLCR = (SCU->PLLCR &(~(0x1FFFFU << 3))) | (15U << 3) | (1U << 12) | (0U << 16);   
#endif 
                
#ifdef XTH_12M_CRYSTAL 
            SCU->PLLCR = (SCU->PLLCR &(~(0x1FFFFU << 3))) | (15U << 3) | (2U << 12) | (0U << 16);  
#endif   
         
            SCU->PLLCR = (SCU->PLLCR & (~(0x3U << 1)) ) | (3 << 1);   
            SCU->PLLCR |= SCU_PLLCR_PLL_UPDATE_EN;   
            while(!(SCU->PLLCR & (SCU_PLLCR_PLL_FREE_RUN) ) );   
                
            /* Division Config */
            SCU->CCR2 = (SCU->CCR2 & (~0xFF)) | APB_CLK_DIV_0 | (lu32_DIV - 1);  
            while((SCU->CCR2 & (1UL << 31)) == 0x00);   
        
            /* Clock Select PLL */
            SCU->CCR1 = SYS_CLK_SRC_PLLCLK;    
            gu32_Clocksrc = CLK_SRC_XTH_PLL;     
        }
        else
        {
            SCU->XTHCR &= (~SCU_XTHCR_XTH_EN);  
        }
    }
    
    if ( (lu32_system_clk_source == CLK_SRC_RC64M) || (0 != lu32_result) )     
    {
        /* Division Config */
        SCU->CCR2 = (SCU->CCR2 & (~0xFF)) | APB_CLK_DIV_0 | (lu32_DIV - 1);  
        while((SCU->CCR2 & (1UL << 31)) == 0x00);   
        
        /* Clock Select RCH */
        SCU->CCR1 = SYS_CLK_SRC_RCH;  
        gu32_Clocksrc = CLK_SRC_RC64M;  
    }
    
    gu32_SystemClock = fu32_Clock;
    gu32_APBClock    = fu32_Clock;    
    
    /* Eflash Config */
    HAL_EFlash_Init(gu32_SystemClock);    
    
    return true;
}

extern DMA_HandleTypeDef DMA_SPIT_Handle;
extern ADC_HandleTypeDef ADC_Handle;

void DMA_IRQHandler(void)
{
//	HAL_DMA_IRQHandler(&DMA_SPIT_Handle);
	HAL_DMA_IRQHandler(ADC_Handle.DMA_Handle);
}
