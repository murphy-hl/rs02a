#include <stdio.h>
#include "main.h"
#include "delay.h"

#define RTC_RST_PIN		GPIO_PIN_1
#define RTC_RST_PORT	GPIOA

#define RTC_SCK_PIN		GPIO_PIN_1
#define RTC_SCK_PORT	GPIOA

#define RTC_SDA_PIN		GPIO_PIN_1
#define RTC_SDA_PORT	GPIOA

#define	RTC_RST_H		HAL_GPIO_WritePin(RTC_RST_PORT,RTC_RST_PIN,GPIO_PIN_SET)
#define	RTC_RST_L		HAL_GPIO_WritePin(RTC_RST_PORT,RTC_RST_PIN,GPIO_PIN_RESET)

#define	RTC_SCK_H		HAL_GPIO_WritePin(RTC_SCK_PORT,RTC_SCK_PIN,GPIO_PIN_SET)
#define	RTC_SCK_L		HAL_GPIO_WritePin(RTC_SCK_PORT,RTC_SCK_PIN,GPIO_PIN_RESET)

#define	RTC_SDA_H		HAL_GPIO_WritePin(RTC_SDA_PORT,RTC_SDA_PIN,GPIO_PIN_SET)
#define	RTC_SDA_L		HAL_GPIO_WritePin(RTC_SDA_PORT,RTC_SDA_PIN,GPIO_PIN_RESET)

#define	RTC_READ_SDA	((RTC_SDA_PORT->IDR & RTC_SDA_PIN) != 0)	

void rtc_init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOC_CLK_ENABLE();						//开启GPIOB时钟
	
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  			//推挽输出
    GPIO_Initure.Pull=GPIO_NOPULL;         				//上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  			//高速
	
	GPIO_Initure.Pin=RTC_RST_PIN;			
    HAL_GPIO_Init(RTC_RST_PORT,&GPIO_Initure);     		
	
	GPIO_Initure.Pin=RTC_SCK_PIN;			
    HAL_GPIO_Init(RTC_SCK_PORT,&GPIO_Initure);     		
	
	GPIO_Initure.Pin=RTC_SDA_PIN;			
    HAL_GPIO_Init(RTC_SDA_PORT,&GPIO_Initure);     		
	
}
uint8_t rtc_read_byte(void)
{
	uint8_t i,value=0;
	
    for(i=0;i<8;i++)
    {
		value=value<<1;
		if(RTC_READ_SDA) 
		{
			value |= 0x01;
		}  
		else 
		{
			value &= 0xfe;
		} 
		RTC_SCK_L;
		delay_us(1);
		RTC_SCK_H;
		delay_us(1);
    }
    return (value);
}
void rtc_write_byte(uint8_t data)
{
	uint8_t i,temp;
	temp = data;
	for(i=0;i<8;i++)
	{
		if(temp & 0x80)
		{
            RTC_SDA_H;
		}
		else
		{
            RTC_SDA_L;
		}
		temp = temp << 1;
        RTC_SCK_L;
		delay_us(1);	
        RTC_SCK_H;
	    delay_us(1);	
	}
	
}