#include "display.h"
#include "lcd.h"
#include "hlw8112.h"
#include "adc.h"

extern unsigned char gImage_logo[];
extern unsigned char gImage_car[];
extern uint8_t work_mode;
extern Date_Typedef SET_date;
extern Time_Typedef SET_time;
extern Date_Typedef wait_date;
extern Time_Typedef wait_time;
extern uint8_t datetime[12];
extern uint32_t Timer_Update_Flag;  
uint16_t Fcolor=WHITE;
uint16_t Bcolor=BLACK;


void display_para(void)
{
	
	
}

void display_main(void)
{

	RTC_GetTime(&RTC_time);
	RTC_GetDate(&RTC_data);
	display_statusbar();
	
	display_currentmode(work_mode);
	
	if(refresh_screen==0)
	{
	
		display_pic_logo();
		display_realV(Realdata.voltage);
		display_realI(Realdata.current);
		display_realP(Realdata.power);
		display_realE(Realdata.energy);
		display_plugT(Realdata.Ptemp);
		LCD_DrawStraightLine(17,247,50,0);
		LCD_DrawStraightLine(174,247,50,0);
		Draw_Circle(121,247,44);
		
	}
}


void display_currentmode(uint8_t mode)
{
	char str[10]={0};
	uint8_t current[4]={8,16,24,32};
	sprintf(str,":%2dA ",current[mode]);
	
	LCD_DispHZ(153,45,12,2,Fcolor,Bcolor);
	LCD_DispHZ(153+12*1,45,12,4,Fcolor,Bcolor);
	LCD_DispHZ(153+12*2,45,12,14,Fcolor,Bcolor);
	LCD_DispHZ(153+12*3,45,12,15,Fcolor,Bcolor);
	
	LCD_ShowString(153+12*4,45,12,str,Fcolor,Bcolor);

}

void display_chargerror(void)
{
	LCD_DispHZ(81,163,20,2,Fcolor,Bcolor);
	LCD_DispHZ(81+20,163,20,3,Fcolor,Bcolor);
	LCD_DispHZ(81+20*2,163,20,10,Fcolor,Bcolor);
	LCD_DispHZ(81+20*3,163,20,11,Fcolor,Bcolor);
	
}
void display_reservemode(void)
{
	LCD_DispHZ(81,94,20,4,Fcolor,Bcolor);
	LCD_DispHZ(81+20,94,20,5,Fcolor,Bcolor);
	LCD_DispHZ(81+20*2,94,20,6,Fcolor,Bcolor);
	LCD_DispHZ(81+20*3,94,20,7,Fcolor,Bcolor);
}
void display_reservesuccess(void)
{
	LCD_DispHZ(81,94,20,4,Fcolor,Bcolor);
	LCD_DispHZ(81+20,94,20,5,Fcolor,Bcolor);
	LCD_DispHZ(81+20*2,94,20,8,Fcolor,Bcolor);
	LCD_DispHZ(81+20*3,94,20,9,Fcolor,Bcolor);
}

void display_waitcharg(void)
{
	
	LCD_DispHZ(81,163,20,12,Fcolor,Bcolor);
	LCD_DispHZ(81+20,163,20,13,Fcolor,Bcolor);
	LCD_DispHZ(81+20*2,163,20,2,Fcolor,Bcolor);
	LCD_DispHZ(81+20*3,163,20,3,Fcolor,Bcolor);
}
void display_charging(void)
{
	
	LCD_DispHZ(81,163,20,0,Fcolor,Bcolor);
	LCD_DispHZ(81+20,163,20,1,Fcolor,Bcolor);
	LCD_DispHZ(81+20*2,163,20,2,Fcolor,Bcolor);
	LCD_DispHZ(81+20*3,163,20,3,Fcolor,Bcolor);
}

void display_minstartcharge(uint16_t min)
{

	char str[10]={0};
	sprintf(str,"%3d",min);
	LCD_ShowString(58,124,16,str,Fcolor,Bcolor);
	
	LCD_DispHZ(82,124,16,0,Fcolor,Bcolor);
	LCD_DispHZ(82+16,124,16,1,Fcolor,Bcolor);
	LCD_DispHZ(82+16*2,124,16,2,Fcolor,Bcolor);
	LCD_DispHZ(82+16*3,124,16,3,Fcolor,Bcolor);
	LCD_DispHZ(82+16*4,124,16,4,Fcolor,Bcolor);
	LCD_DispHZ(82+16*5,124,16,5,Fcolor,Bcolor);
	LCD_DispHZ(82+16*6,124,16,6,Fcolor,Bcolor);
	
	
		
//	sprintf(str,"20%02d",wait_date.Year);
//	LCD_ShowString(45,152,12,str,Fcolor,Bcolor);
	sprintf(str,"%02d",wait_date.Month);
	LCD_ShowString(81-12,152,12,str,Fcolor,Bcolor);
	sprintf(str,"%02d",wait_date.Day);
	LCD_ShowString(105-12,152,12,str,Fcolor,Bcolor);
	
	sprintf(str,"%02d:%02d",wait_time.Hour,wait_time.Minute);
	LCD_ShowString(129-12,152,12,str,Fcolor,Bcolor);
	
//	LCD_DispHZ(69,152,12,18,Fcolor,Bcolor);
	LCD_DispHZ(93-12,152,12,19,Fcolor,Bcolor);
	LCD_DispHZ(117-12,152,12,20,Fcolor,Bcolor);
	LCD_DispHZ(159-12,152,12,16,Fcolor,Bcolor);
	LCD_DispHZ(159+12*1-12,152,12,17,Fcolor,Bcolor);
	LCD_DispHZ(159+12*2-12,152,12,12,Fcolor,Bcolor);
	LCD_DispHZ(159+12*3-12,152,12,2,Fcolor,Bcolor);
}

void display_timestartcharge(Date_Typedef date,Time_Typedef time)
{

}
	

void display_realV(float vol)
{
	char str[10]={0};
//	float vol;
//	
//	vol=hlw8112_get_RmsU();
	LCD_DispHZ(17,195,12,0,Fcolor,Bcolor);
	LCD_DispHZ(17+12,195,12,1,Fcolor,Bcolor);
	LCD_DispHZ(17+12*2,195,12,2,Fcolor,Bcolor);
	LCD_DispHZ(17+12*3,195,12,3,Fcolor,Bcolor);
	
//	LCD_ShowString(x,y+size,size,"          ",Fcolor,Bcolor);
	sprintf(str,"%5.1fV",vol);
	LCD_ShowString(18,216,16,str,Fcolor,Bcolor);
}
void display_realI(float current)
{
	char str[10]={0};
	
	LCD_DispHZ(181,195,12,0,Fcolor,Bcolor);
	LCD_DispHZ(181+12,195,12,1,Fcolor,Bcolor);
	LCD_DispHZ(181+12*2,195,12,2,Fcolor,Bcolor);
	LCD_DispHZ(181+12*3,195,12,4,Fcolor,Bcolor);
	
	if(current<1.0)
	{
		sprintf(str,"%4.2fA",current);
	}else{
		sprintf(str,"%4.1fA",current);
	}
	
	LCD_ShowString(181,215,16,str,Fcolor,Bcolor);

}

void display_realP(float power)
{
	char str[10]={0};
	LCD_DispHZ(17,264,12,0,Fcolor,Bcolor);
	LCD_DispHZ(17+12,264,12,1,Fcolor,Bcolor);
	LCD_DispHZ(17+12*2,264,12,5,Fcolor,Bcolor);
	LCD_DispHZ(17+12*3,264,12,6,Fcolor,Bcolor);
	
	sprintf(str,"%.1fW",power);
	LCD_ShowString(17,285,16,str,Fcolor,Bcolor);

}

void display_realE(float energy)
{
	
	char str[10]={0};
	LCD_DispHZ(98,253,12,11,Fcolor,Bcolor);
	LCD_DispHZ(98+12,253,12,12,Fcolor,Bcolor);
	LCD_DispHZ(98+12*2,253,12,2,Fcolor,Bcolor);
	LCD_DispHZ(98+12*3,253,12,13,Fcolor,Bcolor);
	
//	LCD_ShowString(x,y+size,size,"          ",Fcolor,Bcolor);
	sprintf(str,"%.1fkW*h",energy);
	LCD_ShowString(95,231,16,str,Fcolor,Bcolor);


}

void display_mcuT(float temp)
{
	
}

void display_plugT(float temp)
{
	char str[10]={0};
	
	LCD_DispHZ(181,264,12,7,Fcolor,Bcolor);
	LCD_DispHZ(181+12,264,12,8,Fcolor,Bcolor);
	LCD_DispHZ(181+12*2,264,12,9,Fcolor,Bcolor);
	LCD_DispHZ(181+12*3,264,12,10,Fcolor,Bcolor);
	
//	LCD_ShowString(x,y+size,size,"          ",Fcolor,Bcolor);
	sprintf(str,"%4.1f",temp);
	LCD_ShowString(182,285,16,str,Fcolor,Bcolor);

	LCD_DispHZ(182+16*2,285,16,7,Fcolor,Bcolor);

}

void display_statusbar(void)
{
	char str[10]={0};
	sprintf(str,"%2d",RTC_data.Month);
	LCD_ShowString(9,10,12,str,Fcolor,Bcolor);
	LCD_DispHZ(9+12,10,12,19,Fcolor,Bcolor);
	sprintf(str,"%2d",RTC_data.Day);
	LCD_ShowString(9+12*2,10,12,str,Fcolor,Bcolor);
	LCD_DispHZ(9+12*3,10,12,20,Fcolor,Bcolor);
	
	sprintf(str,"%02u:%02u",RTC_time.Hour,RTC_time.Minute);
	LCD_ShowString(157,10,12,str,Fcolor,Bcolor);
	
	sprintf(str,"%3.1f",get_temp_mcu());
	LCD_ShowString(210-12,10,12,str,Fcolor,Bcolor);
	
	LCD_DispHZ(210+12,10,12,21,Fcolor,Bcolor);
}




void display_pic_logo()
{
	LCD_DispPic(9,40,30,22,gImage_logo);
	LCD_DispHZ(42,45,14,0,WHITE,BLACK);
	LCD_DispHZ(42+14,45,14,1,WHITE,BLACK);
}

void display_pic_car(void)
{
	LCD_DispPic(62,79,116,52,gImage_car);
}
void display_pic_noearth(void)
{
	pic_earth(87,61);
	
	LCD_DispHZ(96,127,12,22,Fcolor,Bcolor);
	LCD_DispHZ(96+12,127,12,23,Fcolor,Bcolor);
	LCD_DispHZ(96+12*2,127,12,24,Fcolor,Bcolor);
	LCD_DispHZ(96+12*3,127,12,25,Fcolor,Bcolor);
	LCD_DispHZ(96,127+13,12,26,Fcolor,Bcolor);
	LCD_DispHZ(96+12,127+13,12,27,Fcolor,Bcolor);
	LCD_DispHZ(96+12*2,127+13,12,28,Fcolor,Bcolor);
	LCD_DispHZ(96+12*3,127+13,12,29,Fcolor,Bcolor);
	
}
void display_pic_overtemp(void)
{
	pic_protect(87,61);
	
	LCD_DispHZ(96,127,12,30,Fcolor,Bcolor);
	LCD_DispHZ(96+12,127,12,31,Fcolor,Bcolor);
	LCD_DispHZ(96+12*2,127,12,32,Fcolor,Bcolor);
	LCD_DispHZ(96+12*3,127,12,33,Fcolor,Bcolor);
	
}

void display_pic_leak(void)
{
	pic_protect(87,61);
	
	LCD_DispHZ(96,127,12,34,Fcolor,Bcolor);
	LCD_DispHZ(96+12,127,12,35,Fcolor,Bcolor);
	LCD_DispHZ(96+12*2,127,12,32,Fcolor,Bcolor);
	LCD_DispHZ(96+12*3,127,12,33,Fcolor,Bcolor);
	
}



void display_set_calender(uint16_t x,uint16_t y)
{

	uint8_t y2=y+20;
	
	
	if(Timer_Update_Flag>499)
	{
		
		switch(menu_one)
		{
			case 0:
				LCD_Dischar(x+16*2,y,' ',16,Fcolor,Bcolor);
				break;
			case 1:
				LCD_Dischar(x+16*3,y,' ',16,Fcolor,Bcolor);
				break;
			case 2:
				LCD_Dischar(x+16*5,y,' ',16,Fcolor,Bcolor);
				break;
			case 3:
				LCD_Dischar(x+16*6,y,' ',16,Fcolor,Bcolor);
				break;
			case 4:
				LCD_Dischar(x+16*8,y,' ',16,Fcolor,Bcolor);
				break;
			case 5:
				LCD_Dischar(x+16*9,y,' ',16,Fcolor,Bcolor);
				break;
			case 6:
				LCD_Dischar(x+16*1,y2,' ',16,Fcolor,Bcolor);
				break;
			case 7:
				LCD_Dischar(x+16*2,y2,' ',16,Fcolor,Bcolor);
				break;
			case 8:
				LCD_Dischar(x+16*4,y2,' ',16,Fcolor,Bcolor);
				break;
			case 9:
				LCD_Dischar(x+16*5,y2,' ',16,Fcolor,Bcolor);
				break;
			case 10:
				LCD_Dischar(x+16*7,y2,' ',16,Fcolor,Bcolor);
				break;
			case 11:
				LCD_Dischar(x+16*8,y2,' ',16,Fcolor,Bcolor);
				break;
			
			default:
				break;

		}
		
	}else{
		
		LCD_Dischar(x,y,'2',16,Fcolor,Bcolor);
		LCD_Dischar(x+16*1,y,'0',16,Fcolor,Bcolor);
		LCD_Dischar(x+16*2,y,datetime[0]%10+0x30,16,Fcolor,Bcolor);
		LCD_Dischar(x+16*3,y,datetime[1]%10+0x30,16,Fcolor,Bcolor);
		LCD_DispHZ(x+16*4,y,16,8,Fcolor,Bcolor);
		LCD_Dischar(x+16*5,y,datetime[2]%10+0x30,16,Fcolor,Bcolor);
		LCD_Dischar(x+16*6,y,datetime[3]%10+0x30,16,Fcolor,Bcolor);
		LCD_DispHZ(x+16*7,y,16,9,Fcolor,Bcolor);
		LCD_Dischar(x+16*8,y,datetime[4]%10+0x30,16,Fcolor,Bcolor);
		LCD_Dischar(x+16*9,y,datetime[5]%10+0x30,16,Fcolor,Bcolor);
		LCD_DispHZ(x+16*10,y,16,10,Fcolor,Bcolor);
		
		
		LCD_Dischar(x+16*1,y2,datetime[6]%10+0x30,16,Fcolor,Bcolor);
		LCD_Dischar(x+16*2,y2,datetime[7]%10+0x30,16,Fcolor,Bcolor);
		LCD_DispHZ(x+16*3,y2,16,11,Fcolor,Bcolor);
		LCD_Dischar(x+16*4,y2,datetime[8]%10+0x30,16,Fcolor,Bcolor);
		LCD_Dischar(x+16*5,y2,datetime[9]%10+0x30,16,Fcolor,Bcolor);
		LCD_DispHZ(x+16*6,y2,16,12,Fcolor,Bcolor);
		LCD_Dischar(x+16*7,y2,datetime[10]%10+0x30,16,Fcolor,Bcolor);
		LCD_Dischar(x+16*8,y2,datetime[11]%10+0x30,16,Fcolor,Bcolor);
		LCD_DispHZ(x+16*9,y2,16,13,Fcolor,Bcolor);
	}
	
	
}




