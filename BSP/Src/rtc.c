#include "rtc.h"
#include <stdio.h>
#include "main.h"
#include "delay.h"

Date_Typedef RTC_data;
Time_Typedef RTC_time;

u8 timer[]={0x55,0x59,0x23,0x16,0x10,0x05,0x20};

void RTC_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	 GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  
    GPIO_Initure.Pull=GPIO_NOPULL;         				//上拉
	
	GPIO_Initure.Pin=RTC_RST_PIN;			
    HAL_GPIO_Init(RTC_RST_PORT,&GPIO_Initure);     		
	
	 GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;  			//推挽输出
	
	GPIO_Initure.Pin=RTC_SCK_PIN;			
    HAL_GPIO_Init(RTC_SCK_PORT,&GPIO_Initure);     		
	
	GPIO_Initure.Pin=RTC_SDA_PIN;			
    HAL_GPIO_Init(RTC_SDA_PORT,&GPIO_Initure); 

//	RTC_SCK_L;
//	RTC_SDA_H;
//	RTC_RST_H;
	
	RTC_write_reg(CTRL_REG,0x00);
	RTC_write_reg(SEC_REG,0);
//	RTC_write_reg(MIN_REG,0);
//	RTC_write_reg(HOUR_REG,0);
//	RTC_write_reg(WEEK_REG,0);
//	RTC_write_reg(DAY_REG,0);
//	RTC_write_reg(MON_REG,0);
//	RTC_write_reg(YEAR_REG,0);
	
}

void RTC_SDA_INinit(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  			//推挽输出
    GPIO_Initure.Pull=GPIO_NOPULL;         				//上拉
	

	GPIO_Initure.Pin=RTC_SDA_PIN;			
    HAL_GPIO_Init(RTC_SDA_PORT,&GPIO_Initure); 
	
}
void RTC_SDA_OUTinit(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  			//推挽输出
    GPIO_Initure.Pull=GPIO_NOPULL;         				//上拉
	

	GPIO_Initure.Pin=RTC_SDA_PIN;			
    HAL_GPIO_Init(RTC_SDA_PORT,&GPIO_Initure); 
	
}

void RTC_GetTime(Time_Typedef *time)
{
	uint8_t temp=0;
	uint8_t result=0;
	temp=RTC_read_reg(SEC_REG);
	temp=RTC_Bcd2ToByte(temp);
	if(temp>59)
	{
		time->Second=0;
	}else{
		time->Second=temp;
	}
	temp=RTC_read_reg(MIN_REG);
	temp=RTC_Bcd2ToByte(temp);
	if(temp>59)
	{
		time->Minute=0;
	}else{
		time->Minute=temp;
	}
	temp=RTC_read_reg(HOUR_REG);
	if(temp&0x80)
	{
		//12小时制
		if(temp&0x20)
		{	//PM
			result=((temp>>4)&0x01)*10+(temp&0x0f)+12;
		}else{
			//AM
			result=((temp>>4)&0x01)*10+(temp&0x0f);
		}
	}else{
		//24小时制
		if(temp&0x20)
		{	//PM
			result=(temp&0x0f)+20;
		}else{
			//AM
			result=((temp>>4)&0x01)*10+(temp&0x0f);
		}
	}
	if(result>23)
	{
		time->Hour=0;
	}else{
		time->Hour=result;
	}
	
}
void RTC_GetDate(Date_Typedef *date)
{
	uint8_t temp=0;
	temp=RTC_read_reg(DAY_REG);
	temp=RTC_Bcd2ToByte(temp);
	if(temp>31)
	{
		date->Day=0;
	}else{
		date->Day=temp;
	}
	temp=RTC_read_reg(MON_REG);
	temp=RTC_Bcd2ToByte(temp);
	if(temp>12)
	{
		date->Month=0;
	}else{
		date->Month=temp;
	}
	temp=RTC_read_reg(YEAR_REG);
	temp=RTC_Bcd2ToByte(temp);
	if(temp>99)
	{
		date->Year=0;
	}else{
		date->Year=temp;
	}
}
void RTC_SetDate(Date_Typedef date)
{
	if(date.Day>31)
	{
		date.Day=1;
	}
	
	if(date.Month>12)
	{
		date.Month=1;
	}
	if(date.Year>99)
	{
		date.Year=0;
	}
	
	RTC_write_reg(DAY_REG,RTC_ByteToBcd2(date.Day));
	
	RTC_write_reg(MON_REG,RTC_ByteToBcd2(date.Month));
	
	RTC_write_reg(YEAR_REG,RTC_ByteToBcd2(date.Year));
}
void RTC_SetTime(Time_Typedef time)
{
	uint8_t temp=0;
	if(time.Second>59)
	{
		time.Second=0;
	}
	if(time.Minute>59)
	{
		time.Minute=0;
	}
	if(time.Hour>23)
	{
		time.Hour=0;
	}
	
	RTC_write_reg(SEC_REG,RTC_ByteToBcd2(time.Second));
	RTC_write_reg(MIN_REG,RTC_ByteToBcd2(time.Minute));
	
	temp=time.Hour;
	if(temp>19)
	{
		temp=0x20|(temp-20);
	}else if(temp>9){
		temp=0x10|(temp-10);
		
	}else{
		temp=0x00|(temp);
	}
	RTC_write_reg(HOUR_REG,temp);
}


void RTC_write_reg(uint8_t reg,uint8_t data)
{

//	temp=RTC_ByteToBcd2(data);
	RTC_RST_L;
	delay_us(1);
	RTC_SCK_L;
	delay_us(1);
	RTC_RST_H;
	delay_us(1);
	rtc_write_byte(reg);
	rtc_write_byte(data);
	RTC_RST_L;
}



uint8_t  RTC_read_reg(uint8_t reg)
{
	uint8_t value=0;
	
	RTC_RST_L;
	delay_us(1);
	RTC_SCK_L;
	delay_us(1);
	RTC_RST_H;
	delay_us(1);
	
	rtc_write_byte(reg|0x01);
	
	value=rtc_read_byte();
	
	RTC_RST_L;
	delay_us(1);
	RTC_SCK_L;
	delay_us(1);
	RTC_SCK_H;
	delay_us(1);
	RTC_SDA_L;
	delay_us(1);
	RTC_SDA_H;
	delay_us(1);
	
    return (value);
}

uint8_t rtc_read_byte(void)
{
	uint8_t i,value=0;
    for(i=0;i<8;i++)
    {
		RTC_SCK_L;
		delay_us(1);
		value=value>>1;
		if(RTC_READ_SDA) 
		{
			value |= 0x80;
		}  
		RTC_SCK_H;
		delay_us(1);
    }
    return (value);
}

void rtc_write_byte(uint8_t data)
{
	uint8_t i,temp = data;
	for(i=0;i<8;i++)
	{
		RTC_SCK_L;
		delay_us(1);
		if(temp & 0x01)
		{
            RTC_SDA_H;
		}
		else
		{
            RTC_SDA_L;
		}
		temp = temp >> 1;
		RTC_SCK_H;
	    delay_us(1);  	
	}
}


uint8_t RTC_ByteToBcd2(uint8_t Value)
{
  uint32_t bcdhigh = 0U;
  uint8_t temp = Value;

  while (temp >= 10U)
  {
    bcdhigh++;
    temp -= 10U;
  }

  return ((uint8_t)(bcdhigh << 4U) | temp);
}

/**
  * @brief  Convert from 2 digit BCD to Binary.
  * @param  Value BCD value to be converted
  * @retval Converted word
  */
uint8_t RTC_Bcd2ToByte(uint8_t Value)
{
  uint8_t tmp;
  tmp = ((Value & 0xF0U) >> 4U) * 10U;
  return (tmp + (Value & 0x0FU));
}

/************************ (C) COPYRIGHT murphy_jiebi*****END OF FILE****/
