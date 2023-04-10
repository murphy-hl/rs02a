#include "adc.h"
#include "math.h"


ADC_HandleTypeDef ADC_Handle;
DMA_HandleTypeDef  Dma_Adc_Handle;
static volatile uint32_t gu32_ITC_Conunt = 0;    // transfer complete interrupt count
static volatile uint32_t gu32_IE_Conunt = 0;     // transfer error interrupt count

uint32_t adc_buffer[2]={0};

static void DMA_ADC_ITC_Callback(void)
{    
    gu32_ITC_Conunt++;
}

/************************************************************************
 * function   : DMA_ADC_IE_Callback
 * Description: DMA adc to memory transfer error interrupt Callback. 
 ************************************************************************/ 
static void DMA_ADC_IE_Callback(void)
{    
    gu32_IE_Conunt++;
}

void adc_init(void)
{
	
	ADC_ChannelConfTypeDef ADC_ChannelConf;
    
    ADC_Handle.Init.ClockDiv = ADC_CLOCK_DIV3;
    ADC_Handle.Init.ConConvMode = ADC_CONCONVMODE_ENABLE;
    ADC_Handle.Init.JChannelMode = ADC_JCHANNELMODE_DISABLE;
    ADC_Handle.Init.DiffMode = ADC_DIFFMODE_DISABLE;
    ADC_Handle.Init.DMAMode = ADC_DMAMODE_ENABLE;
    ADC_Handle.Init.OverMode = ADC_OVERMODE_DISABLE;
    ADC_Handle.Init.OverSampMode = ADC_OVERSAMPMODE_DISABLE;
    ADC_Handle.Init.AnalogWDGEn = ADC_ANALOGWDGEN_DISABLE;
    ADC_Handle.Init.ChannelEn = ADC_CHANNEL_15_EN | ADC_CHANNEL_14_EN;

	
    ADC_Handle.Instance = ADC;
    ADC_Handle.AdcResults = &adc_buffer[0];    
    HAL_ADC_Init(&ADC_Handle);

    /* The total adc regular channels number */
    ADC_Handle.ChannelNum = 2;
  
    ADC_ChannelConf.Channel = ADC_CHANNEL_15;
    ADC_ChannelConf.RjMode = 0;
    ADC_ChannelConf.Sq = ADC_SEQUENCE_SQ1;
    ADC_ChannelConf.Smp = ADC_SMP_CLOCK_20;   
    HAL_ADC_ConfigChannel(&ADC_Handle,&ADC_ChannelConf);
	
	ADC_ChannelConf.Channel = ADC_CHANNEL_14;
    ADC_ChannelConf.RjMode = 0;
    ADC_ChannelConf.Sq = ADC_SEQUENCE_SQ2;
    ADC_ChannelConf.Smp = ADC_SMP_CLOCK_20;   
    HAL_ADC_ConfigChannel(&ADC_Handle,&ADC_ChannelConf);
	
//	Dma_Adc_Handle.Instance = DMA_Channel2;
//    Dma_Adc_Handle.Init.Request_ID = REQ0_ADC;
//    Dma_Adc_Handle.Init.Mode = DMA_CIRCULAR;        
//    Dma_Adc_Handle.Init.Data_Flow = DMA_DATA_FLOW_P2M;
//    Dma_Adc_Handle.Init.Source_Inc       = DMA_SOURCE_ADDR_INCREASE_DISABLE;
//    Dma_Adc_Handle.Init.Desination_Inc   = DMA_DST_ADDR_INCREASE_ENABLE;
//    Dma_Adc_Handle.Init.Source_Width     = DMA_SRC_WIDTH_WORD;
//    Dma_Adc_Handle.Init.Desination_Width = DMA_DST_WIDTH_WORD;

//    /*-----------------------------------------------------------------------------------*/
//    /* Note:If user dons not apply interrupt, Set DMA_ITC_CallbackbDMA_IE_Callback NULL */
//    /*-----------------------------------------------------------------------------------*/
//    Dma_Adc_Handle.DMA_ITC_Callback = DMA_ADC_ITC_Callback;
//    Dma_Adc_Handle.DMA_IE_Callback  = DMA_ADC_IE_Callback;

//    HAL_DMA_Init(&Dma_Adc_Handle);

//    ADC_Handle.DMA_Handle = &Dma_Adc_Handle;
//	
//	ADC_DMA_Convert(&ADC_Handle, adc_buffer, ADC_Handle.ChannelNum);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Handle; 
	GPIO_Handle.Pin            = GPIO_PIN_0;
	GPIO_Handle.Mode           = GPIO_MODE_ANALOG;
	GPIO_Handle.Pull           = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_Handle);       

	GPIO_Handle.Pin            = GPIO_PIN_1;
	GPIO_Handle.Mode           = GPIO_MODE_ANALOG;
	GPIO_Handle.Pull           = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_Handle);    
    /* Enable ADC Clock */
    System_Module_Enable(EN_ADC);
    /* Clear Pending Interrupt */
    NVIC_ClearPendingIRQ(ADC_IRQn);
    /* Enable External Interrupt */
    NVIC_EnableIRQ(ADC_IRQn);
}
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{    
    /* Disable ADC Clock */
    System_Module_Disable(EN_ADC);
    /* Clear Pending Interrupt */
    NVIC_ClearPendingIRQ(ADC_IRQn);
    
    /* Disable External Interrupt */
    NVIC_DisableIRQ(ADC_IRQn);
}


#define		b_CONSTANT		3450		//w温度转换系数
/********************************************************
R=100V/(33-20V)				R:电阻值 单位K   V：电压值 单位V

********************************************************/

#define Epsilon 10e-6

long double ln(long double c)
{
	long double s2=0.0,delta;int i=1;
	if(c==1) 	
	{
		return 0;
	}
	c=(c-1.0)/(c+1.0);//Marclurin??????
	delta=c;i=0;
	do{ 
		s2+=delta/(2*i+1);
		delta*=c*c;i++;
	}while(fabs((2*delta)/(2*i+1))>Epsilon);

	return 2*s2;
}

float get_temp_plug(void)
{
	float adc_val;
	float vol,res,temp=0;


	 HAL_ADC_Polling(&ADC_Handle, adc_buffer, ADC_Handle.ChannelNum, 0);
	
	adc_val= adc_buffer[0]&0xfff;
	vol =adc_val*3.2f/4096.00f;
	res=vol*100/(32.0-20*vol);
	if(res==0)
	{
		return 0;
	}
	temp=1/(1/298.15+ln(res/10.00f)/b_CONSTANT)-273.15;

	return temp;		
}

float get_temp_mcu(void)
{
	float adc_val;
	float vol,res,temp=0;


	HAL_ADC_Polling(&ADC_Handle, adc_buffer, ADC_Handle.ChannelNum, 0);
	adc_val= adc_buffer[1]&0xfff;
	vol =adc_val*3.2f/4096.00f;
	res=vol*100/(32.0-20*vol);
	if(res==0)
	{
		return 0;
	}
	temp=1/(1/298.15+ln(res/10.00f)/b_CONSTANT)-273.15;
	
	return temp;
}
/************************************************************************
 * function   : ADC_IRQHandler
 * Description: adc interrupt service routine. 
 * input : 
 *         none
 * return: none
 ************************************************************************/
void ADC_IRQHandler(void)
{
    HAL_ADC_IRQHandler(&ADC_Handle);
    NVIC_ClearPendingIRQ(ADC_IRQn); 
}

uint8_t ADC_DMA_Convert(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length)
{
    uint8_t tmp_hal_status;
    
    if(!hadc->AdcResults)  return HAL_ERROR;

    if(HAL_ADC_Start(hadc) != HAL_OK)  return HAL_ERROR; 
    
    if(!pData)  return HAL_ERROR;
    
    hadc->AdcResults = pData;
    
    tmp_hal_status = HAL_ADC_Start_DMA(&ADC_Handle,hadc->AdcResults,Length);
    
    if(tmp_hal_status != HAL_OK) return HAL_ERROR; 

    while(!gu32_ITC_Conunt){}
        
    gu32_ITC_Conunt--;
        
    if(hadc->Init.ConConvMode == ADC_CONCONVMODE_DISABLE)
        HAL_ADC_Stop_DMA(hadc); 
    
    return tmp_hal_status;
}





