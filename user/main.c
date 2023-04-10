/*
  ******************************************************************************
  作者					日期					版本
  
  murphy_jiebi			2022/09/09				version 1.0
  ******************************************************************************
*/
#include "main.h"
#include "uart.h"
#include "24cxx.h" 
#include "timer.h"
#include "bsp.h"
#include "lcd.h"
#include "pwm.h"
#include "sys.h"
#include "audio.h"
#include "hlw8112.h"
#include "comp.h"
#include "display.h"
#include "adc.h"
#include "spi.h"
#include "bt.h"
#include "key.h"
#include "delay.h"

extern uint8_t INT_1S;
Electric_data_typedef Realdata;

const uint16_t pwm_duty[4]={864,733,600,467};				//PWM频率
const uint8_t audio_current[4]={0x06,0x09,0x0A,0x0B};		//喇叭播放音频序号
const uint16_t res_time[25]={0,30,60,90,120,150,180,210,240,270,300,		//预约时间表
330,360,390,420,450,480,510,540,570,600,630,660,690,720};


uint16_t earth_count=0;
uint8_t work_mode=0;

uint32_t reser_time=0;
uint16_t earth_timer[5]={0};
typdef_keyval key_val;
uint8_t run_timers=0;
uint8_t bmp_flag[2]={0};
uint8_t menu_level=0;
uint8_t menu_one=0;
uint8_t menu_two=0;
Date_Typedef SET_date;
Time_Typedef SET_time;
Date_Typedef wait_date;
Time_Typedef wait_time;
uint8_t datetime[12]={0};
uint8_t adc_p=0;
uint32_t test=0;
uint8_t temp[7];

uint8_t menu_index=0;

int main(void)
{
	System_Init();
	TIM14_Init();
	TIM6_Init();
	Uart1_Init();
	AT24CXX_Init();
	SYS_Param_Init();
	key_Init();
	bsp_init();
	LCD_Init();
	RTC_Init();
	audio_init();
	play_audio(0x00);
	hlw8112_init();
	PWM_set_duty(0);
	COMP_Init();
	adc_init();
	display_pic_logo();
	LCD_DrawStraightLine(17,247,50,0);
	LCD_DrawStraightLine(174,247,50,0);
	Draw_Circle(121,247,44);
	
    while(1)
    {	

		#if 1
		display_main();
		if(INT_1S>1)
		{
			Readpara();
			INT_1S=0;
		}
		
		
		if(Realdata.leak_current>0.03f)
		{
			menu_index=0;
		}else if(get_temp_plug()>85.0f)
		{
			menu_index=1;
		}else{
			menu_index=2;
		}
		
		if(bmp_flag[0]!=bmp_flag[1])
		{
			LCD_Fill(87,61,240-90,70,BLACK);
			LCD_Fill(0,70,240,170,BLACK);
		}
		
		switch(menu_index)
		{
			case 0:		//漏电
				display_pic_leak();
				bmp_flag[0]=bmp_flag[1];
				bmp_flag[1]=1;
				if(count_100s%3==0)
				{
					play_audio(0x10);
				}
				LN_CTL_OFF;
				PWM_set_duty(0);
				break;
			case 1:		//过温
				display_pic_overtemp();
				bmp_flag[0]=bmp_flag[1];
				bmp_flag[1]=1;
				if(count_100s%3==0)
				{
					play_audio(0x0f);
				}
				LN_CTL_OFF;
				PWM_set_duty(0);
				break;
			case 2:
				run_timers++;
				key_val=key_scan();
				if(key_val!=KEY_NULL)
				{
					refresh_screen=SLEEP_TIME;
				}
				if(!comp_get_level(1))
				{
					key_handle((uint8_t)key_val);
				}
				earth_timer[run_timers%5]=earth_count;

				switch(menu_level)
				{
					case 0:		//正常运行模式
						if(!comp_get_level(1))
						{
							PWM_set_duty(0);
							display_waitcharg();
							LN_CTL_OFF;
						}else{
							if(get_temp_plug()>60.0f)
							{
								work_mode=0;
							}
							PWM_set_duty(pwm_duty[work_mode]);
							if(!comp_get_level(2))
							{
								LN_CTL_OFF;
								display_waitcharg();
							}else{
								LN_CTL_ON;  
								display_charging();
							}
						}
						if(READ_EARTH)
						{
							display_pic_car();
							bmp_flag[0]=bmp_flag[1];
							bmp_flag[1]=2;
						}else{
							display_pic_noearth();
							bmp_flag[0]=bmp_flag[1];
							bmp_flag[1]=3;
						}
						break;
					case 1:		//设置时间日期
						LN_CTL_OFF;
						bmp_flag[0]=bmp_flag[1];
						display_set_calender(32,110);
						break;
					case 2:		//预约
						bmp_flag[0]=bmp_flag[1];
						display_reservemode();
						display_minstartcharge(res_time[menu_two]);
						PWM_set_duty(0);
						break;
					case 3:		//预约成功，等待充电
						bmp_flag[0]=bmp_flag[1];
						display_reservesuccess();
						display_minstartcharge(reser_time/60);
						PWM_set_duty(0);
						break;
					default:
						
						break;
					
				}	

				break;
			default:
				break;

		}			
		

		
		#endif
		
    }
}


void key_handle(uint8_t key_val)
{
	switch(key_val)
	{
		case KEY_NULL:
			break;
		case KEY_MODE_SHORT:
			if(menu_level==0)
			{
				work_mode++;
				if(work_mode>3)
				{
					work_mode=0;
				}
				AT24CXX_WriteOneByte(10,work_mode);
				play_audio(audio_current[work_mode]);
			}else if(menu_level==1){
				//时间日期设置
				menu_one++;
				if(menu_one>11)
				{
					menu_one=0;
				}
			}else if(menu_level==2){
				if(comp_get_level(1))
				{
					
				}else{
					if(menu_two!=0)
					{
						menu_two--;
					}else{
						menu_two=24;
					}
					TimeGetDate(res_time[menu_two]);
				}
			}
			break;
		case KEY_MODE_LONG:
			break;
		case KEY_TIME_SHORT:
			if(menu_level==2)
			{
				menu_two++;
				if(menu_two>24)
				{
					menu_two=0;
				}
				TimeGetDate(res_time[menu_two]);
			}
			if(menu_level==1)
			{
				switch(menu_one)
				{
					case 0:
						datetime[0]++;
						if(datetime[0]>9)
						{
							datetime[0]=0;
						}
						break;
					case 1:
						datetime[1]++;
						if(datetime[1]>9)
						{
							datetime[1]=0;
						}
						break;
					case 2:
						datetime[2]++;
						if(datetime[2]>1)
						{
							datetime[2]=0;
						}
						break;
					case 3:
						datetime[3]++;
						if(datetime[2]==0)
						{
							if(datetime[3]>9)
							{
								datetime[3]=0;
							}
						}else if(datetime[2]==1)
						{
							if(datetime[3]>2)
							{
								datetime[3]=0;
							}
						}
						break;
					case 4:
						if(datetime[2]*10+datetime[3]==2)
						{
							datetime[4]++;
							if(datetime[4]>2)
							{
								datetime[4]=0;
							}
						}else{
							datetime[4]++;
							if(datetime[4]>3)
							{
								datetime[4]=0;
							}
						}					
						break;
					case 5:
						datetime[5]++;
						if(datetime[2]*10+datetime[3]==2)
						{
							if(datetime[4]==2)
							{
								if((((datetime[0]*10+datetime[1]+2000)%4==0)&&((datetime[0]*10+datetime[1]+2000)%100!=0))||((datetime[0]*10+datetime[1]+2000)%100==0))
								{
									if(datetime[5]>9)
									{
										datetime[5]=0;
									}
								}else{
									if(datetime[5]>8)
									{
										datetime[5]=0;
									}
								}
							}else{
								if(datetime[5]>9)
								{
									datetime[5]=0;
								}
							}
						}else if((datetime[2]*10+datetime[3]==4)||(datetime[2]*10+datetime[3]==6)||(datetime[2]*10+datetime[3]==9)||(datetime[2]*10+datetime[3]==11)){
							if(datetime[4]==3)
							{
								datetime[5]=0;
							}else{
								if(datetime[5]>9)
								{
									datetime[5]=0;
								}
							}
						}else{
							if(datetime[4]==3)
							{
								if(datetime[5]>1)
								{
									datetime[5]=0;
								}
							}else{
								if(datetime[5]>9)
								{
									datetime[5]=0;
								}
							}
						}
						
						break;
					case 6:
						datetime[6]++;
						if(datetime[6]>2)
						{
							datetime[6]=0;
						}
						break;
					case 7:
						datetime[7]++;
						if(datetime[6]==2)
						{
							if(datetime[7]>3)
							{
								datetime[7]=0;
							}
						}else{
							if(datetime[7]>9)
							{
								datetime[7]=0;
							}
						}
						break;
					case 8:
						datetime[8]++;
						if(datetime[8]>5)
						{
							datetime[8]=0;
						}
						break;
					case 9:
						datetime[9]++;
						if(datetime[9]>9)
						{
							datetime[9]=0;
						}
						break;
					case 10:
						datetime[10]++;
						if(datetime[10]>5)
						{
							datetime[10]=0;
						}
						break;
					case 11:
						datetime[11]++;
						if(datetime[11]>9)
						{
							datetime[11]=0;
						}
						break;	
					default:
						break;
				}
				
			}
			break;
		case KEY_TIME_LONG:
			LCD_Fill(87,61,87+60,79,BLACK);
			LCD_Fill(62,79,192,163+20,BLACK);
			
			TimeGetDate(res_time[menu_two]);
			if(menu_level==2)
			{
				if(menu_two!=0)
				{
					menu_level=3;
					reser_time=res_time[menu_two]*60;
					
					play_audio(0x0d);
				}else{
					menu_level=0;
					play_audio(0x0e);
				}
			}else if(menu_level==1)
			{
				
			}else {
				if(comp_get_level(1))
				{
					
				}else{
					if(menu_two!=0)
					{
						menu_two=0;
						menu_level=0;
						play_audio(0x0e);
						bmp_flag[1]=5;
					}else{
						menu_level=2;
						play_audio(0x0c);
						bmp_flag[1]=5;
					}
					
				}
				
			}

			break;
		case KEY_MT_SHORT:
			
			break;
		case KEY_MT_LONG:
			if(menu_level==1)
			{
				menu_level=0;
				SET_date.Year=datetime[0]*10+datetime[1];
				SET_date.Month=datetime[2]*10+datetime[3];
				SET_date.Day=datetime[4]*10+datetime[5];
				
				SET_time.Hour=datetime[6]*10+datetime[7];
				SET_time.Minute=datetime[8]*10+datetime[9];
				SET_time.Second=datetime[10]*10+datetime[11];
				RTC_SetTime(SET_time);
				RTC_SetDate(SET_date);
				LCD_Fill(87,61,240-90,70,BLACK);
				LCD_Fill(0,70,240,185,BLACK);
			}else{
				menu_level=1;
				RTC_GetTime(&SET_time);
				RTC_GetDate(&SET_date);
				datetime[0]=SET_date.Year%100/10;
				datetime[1]=SET_date.Year%10;
				datetime[2]=SET_date.Month%100/10;
				datetime[3]=SET_date.Month%10;
				datetime[4]=SET_date.Day%100/10;
				datetime[5]=SET_date.Day%10;
				
				datetime[6]=SET_time.Hour%100/10;
				datetime[7]=SET_time.Hour%10;
				datetime[8]=SET_time.Minute%100/10;
				datetime[9]=SET_time.Minute%10;
				datetime[10]=SET_time.Second%100/10;
				datetime[11]=SET_time.Second%10;
				LCD_Fill(87,61,240-90,70,BLACK);
				LCD_Fill(0,70,240,185,BLACK);
			}
			break;
		default:
			break;
		
	}
}


void Readpara(void)
{
	
	Realdata.voltage=hlw8112_get_RmsU();
	Realdata.current=hlw8112_get_RmsIA();
	Realdata.leak_current=hlw8112_get_RmsIB();
	Realdata.power=hlw8112_get_RmsP();
//			Realdata.energy=hlw8112_get_RmsE();
	Realdata.energy+=Realdata.power/1000.00f/3600.00f*INT_1S;
	Realdata.Mtemp=get_temp_mcu();
	Realdata.Ptemp=get_temp_plug();
	Realdata.Fre=hlw8112_get_ufre();
	Realdata.power_cal=Realdata.voltage*Realdata.current;
	
}


void TimeGetDate(uint16_t min)
{
	Date_Typedef date;
	Time_Typedef time;
	
	RTC_GetTime(&time);
	RTC_GetDate(&date);
	if(RTC_time.Minute+min%60>59)
	{
		time.Minute=RTC_time.Minute+min%60-60;
		time.Hour+=1;
	}else{
		time.Minute=RTC_time.Minute+min%60;
	}
	if(time.Hour+min/60>23)
	{
		time.Hour+=min/60-24;
		date.Day+=1;
	}else{
		time.Hour+=min/60;
	}
	if(date.Month==1||date.Month==3||date.Month==5||date.Month==7||date.Month==8||date.Month==10||date.Month==12)
	{
		if(date.Day>31)
		{
			date.Day=1;
			date.Month++;
		}
	}else if(date.Month==4||date.Month==6||date.Month==9||date.Month==11)
	{
		if(date.Day>30)
		{
			date.Day=1;
			date.Month++;
		}
	}else if(date.Month==2)
	{
		if((((date.Year+2000)%4==0)&&((date.Year+2000)%100!=0))||((date.Year+2000)%100==0))
		{
			if(date.Day>29)
			{
				date.Day=1;
				date.Month++;
			}
		}else{
			if(date.Day>28)
			{
				date.Day=1;
				date.Month++;
			}
		}
	}else{
		date.Day=1;
	}
	if(date.Month>12)
	{
		date.Month=1;
	}	
	
	memcpy(&wait_date,&date,sizeof(Date_Typedef));
	memcpy(&wait_time,&time,sizeof(Time_Typedef));
	
}

