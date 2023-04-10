#include "delay.h"

void delay_us(u16 us)
{

	while(us--!=0)
	{
		__NOP();
	}
} 

void delay_US(u16 us)
{
	u32 Delay = us * 5;
	do
	{
		__NOP();
	}
	while(Delay--);
}


void delay_ms(u16 ms)
{

	uint32_t tickstart = TICK;
	uint32_t wait = ms;

	while ((TICK - tickstart) < wait)
	{
	}
}

void delay_s(u16 s)
{
	while(s--!=0)
	{
		delay_ms(1000);
	}
}

void delay_MS(u16 ms)
{
	while(ms--!=0)
	{
		delay_us(1000);
	}
//	int i,j;
//	for(i=1;i<ms;i++)
//	{
//		for(j=0;j<128;j++);
//	}
}


/************************ (C) COPYRIGHT murphy_jiebi*****END OF FILE****/






