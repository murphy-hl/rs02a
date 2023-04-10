#ifndef _RTC_H
#define _RTC_H

#include "main.h"

#define		SEC_REG					0x80
#define		MIN_REG					0x82
#define		HOUR_REG				0x84
#define		WEEK_REG				0X8A
#define		DAY_REG					0X86
#define		MON_REG					0X88
#define		YEAR_REG				0X8C
#define		CTRL_REG				0X8E
/*
	注：0x80~0xD0为闹钟寄存器地址，这里不使用
*/
#define		CFG1_REG				0XE0
#define		CFG2_REG				0XF0

#define   	INT_ENABLE  			0x03    	//RTC中断使能
#define   	INT_DISABLE 			0x01		//RTC中断不使能(寄存器1配置为INTRA输出低)


#define 	RTC_RST_PIN				GPIO_PIN_7
#define 	RTC_RST_PORT			GPIOB

#define 	RTC_SCK_PIN				GPIO_PIN_8
#define 	RTC_SCK_PORT			GPIOB

#define 	RTC_SDA_PIN				GPIO_PIN_9
#define 	RTC_SDA_PORT			GPIOB

#define		RTC_RST_H				HAL_GPIO_WritePin(RTC_RST_PORT,RTC_RST_PIN,GPIO_PIN_SET)
#define		RTC_RST_L				HAL_GPIO_WritePin(RTC_RST_PORT,RTC_RST_PIN,GPIO_PIN_CLEAR)

#define		RTC_SCK_H				HAL_GPIO_WritePin(RTC_SCK_PORT,RTC_SCK_PIN,GPIO_PIN_SET)
#define		RTC_SCK_L				HAL_GPIO_WritePin(RTC_SCK_PORT,RTC_SCK_PIN,GPIO_PIN_CLEAR)

#define		RTC_SDA_H				HAL_GPIO_WritePin(RTC_SDA_PORT,RTC_SDA_PIN,GPIO_PIN_SET)
#define		RTC_SDA_L				HAL_GPIO_WritePin(RTC_SDA_PORT,RTC_SDA_PIN,GPIO_PIN_CLEAR)

#define		RTC_READ_SDA			/*HAL_GPIO_ReadPin(RTC_SDA_PORT,RTC_SDA_PIN)*/((GPIOAB->IDATA & (RTC_SDA_PIN<<16)) != 0)	


typedef struct
{
	uint8_t Year;
	uint8_t Month;
	uint8_t Day;
}Date_Typedef;

typedef struct
{
	uint8_t Hour; 
	uint8_t Minute;
	uint8_t Second;
}Time_Typedef;

extern Date_Typedef RTC_data;
extern Time_Typedef RTC_time;

//RTC所有操作函数
void RTC_Init(void);                //初始化RTC的IO口
void RTC_SDA_INinit(void);
void RTC_SDA_OUTinit(void);
/**************RTC配置****************/
void RTC_GetTime(Time_Typedef *time);
void RTC_GetDate(Date_Typedef *date);
void RTC_SetDate(Date_Typedef date);
void RTC_SetTime(Time_Typedef time);

void RTC_INT_Config(u8 rtc_int);


uint8_t  RTC_read_reg(uint8_t reg);
void RTC_write_reg(uint8_t reg,uint8_t data);

/*************************************/

uint8_t rtc_read_byte(void);
void rtc_write_byte(uint8_t data);


uint8_t RTC_ByteToBcd2(uint8_t Value);
uint8_t RTC_Bcd2ToByte(uint8_t Value);
/*************************************/
#endif

/************************ (C) COPYRIGHT murphy_jiebi*****END OF FILE****/
