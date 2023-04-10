#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "delay.h"
#include "spi.h"

_lcd_dev lcddev;
		   
		   	   
void LCD_Init(void)
{
	SPI_Init();
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Alternate = GPIO_FUNCTION_0;
	
	GPIO_InitStruct.Pin = LCD_CS_PIN;
	HAL_GPIO_Init(LCD_CS_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = LCD_DC_PIN;
	HAL_GPIO_Init(LCD_DC_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = LCD_RST_PIN;
	HAL_GPIO_Init(LCD_RST_PORT, &GPIO_InitStruct);
	
//	GPIO_InitStruct.Pin = LCD_SCL_PIN;
//	HAL_GPIO_Init(LCD_SCL_PORT, &GPIO_InitStruct);
//	
//	GPIO_InitStruct.Pin = LCD_SDA_PIN;
//	HAL_GPIO_Init(LCD_SDA_PORT, &GPIO_InitStruct);
//	
	 //LCD复位 
	LCD_RST_H;
	delay_ms(1);
	LCD_RST_L;
	delay_ms(10);
	LCD_RST_H;
	delay_ms(200);

	LCD_CS_L;
	LCD_CS_H;
	//---------------------------------------------------------------------------------------------------//
	LCD_WR_REG(0x11);
	delay_ms(120); //Delay 120ms
	//------------------------------display and color format setting--------------------------------//
	LCD_WR_REG(0x36);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0x3a);
	LCD_WR_DATA(0x05);
	//--------------------------------ST7789V Frame rate setting----------------------------------//
	LCD_WR_REG(0xb2);
	LCD_WR_DATA(0x0c);
	LCD_WR_DATA(0x0c);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x33);
	LCD_WR_DATA(0x33);
	LCD_WR_REG(0xb7);
	LCD_WR_DATA(0x35);
	//---------------------------------ST7789V Power setting--------------------------------------//
	LCD_WR_REG(0xbb);
	LCD_WR_DATA(0x28);
	LCD_WR_REG(0xc0);
	LCD_WR_DATA(0x2c);
	LCD_WR_REG(0xc2);
	LCD_WR_DATA(0x01);
	LCD_WR_REG(0xc3);
	LCD_WR_DATA(0x0b);
	LCD_WR_REG(0xc4);
	LCD_WR_DATA(0x20);
	LCD_WR_REG(0xc6);
	LCD_WR_DATA(0x0f);
	LCD_WR_REG(0xd0);
	LCD_WR_DATA(0xa4);
	LCD_WR_DATA(0xa1);
	//--------------------------------ST7789V gamma setting---------------------------------------//
	LCD_WR_REG(0xe0);
	LCD_WR_DATA(0xd0);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x0f);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x2a);
	LCD_WR_DATA(0x36);
	LCD_WR_DATA(0x55);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x3a);
	LCD_WR_DATA(0x0b);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x20);
	LCD_WR_REG(0xe1);
	LCD_WR_DATA(0xd0);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x0a);
	LCD_WR_DATA(0x0b);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x43);
	LCD_WR_DATA(0x4a);
	LCD_WR_DATA(0x2b);
	LCD_WR_DATA(0x1b);
	LCD_WR_DATA(0x1c);
	LCD_WR_DATA(0x22);
	LCD_WR_DATA(0x1f);
	LCD_Clear(BLACK);
	LCD_WR_REG(0x29);

	lcddev.dir=0;	//竖屏
	lcddev.width=240;
	lcddev.height=320;

	lcddev.wramcmd=0X2C;
	lcddev.setxcmd=0X2A;
	lcddev.setycmd=0X2B;  

	LCD_Clear(BLACK);
	
}


void LCD_WR_REG(u8 REG)
{ 
	u8 reg_val=REG;
	
	LCD_CS_L;
	LCD_DC_L;
	HAL_SPI_Transmit(&hspi2,&reg_val,1,1000);
	LCD_CS_H;
}
void LCD_WR_DATA(u16 DATA)
{										    	   
	u8 data_val=DATA;
	
	LCD_CS_L;
	LCD_DC_H;
	HAL_SPI_Transmit(&hspi2,&data_val,1,1000);
	LCD_CS_H;
}		

//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}	      
//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
 	LCD_WR_REG(lcddev.wramcmd);
}	 
//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM(u16 DAT)
{							    
	u8 data[2]={0};
	data[0]=(DAT>>8)&0xff;
	data[1]=DAT&0xff;
	LCD_CS_L;
	LCD_DC_H;
	HAL_SPI_Transmit(&hspi2,data,2,1000);
//	HAL_SPI_Transmit_DMA(&hspi2,data,2);
	LCD_CS_H;
}
void LCD_WriteRAMstr(u8 data[],u16 len)
{							    
	LCD_CS_L;
	LCD_DC_H;
	HAL_SPI_Transmit(&hspi2,data,len,1000);
//	HAL_SPI_Transmit_DMA(&hspi2,data,len);
	LCD_CS_H;
}


//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 
	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(Xpos>>8); 
	LCD_WR_DATA(Xpos&0XFF);	
  	LCD_WR_DATA(lcddev.width>>8); 
	LCD_WR_DATA(lcddev.width&0XFF);	
	
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(Ypos>>8); 
	LCD_WR_DATA(Ypos&0XFF);
	LCD_WR_DATA(lcddev.height>>8); 
	LCD_WR_DATA(lcddev.height&0XFF);
} 		 
     
//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	u16 color=WHITE;
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_WriteRAM(color); 
}

//清屏函数
//color:要清屏的填充色
void LCD_Clear(u16 color)
{
	u8 ram[240*2]={0};	
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 	//得到总点数
	LCD_SetCursor(0x00,0x0000);	//设置光标位置
	LCD_WriteRAM_Prepare();     //开始写入GRAM	
	
	for(uint16_t i=0;i<480;)
	{
		ram[i]=(color>>8)&0xff;
		ram[i+1]=color&0xff;
		i+=2;
	}
	for(index=0;index<320;index++)
	{
		LCD_WriteRAMstr(ram,480);	   
	}
}  
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	uint8_t ram[1024]={0};
	u16 i,j;
	u16 xlen,ylen;
	xlen=ex-sx+1;	  
	ylen=ey-sy+1;
	for(i=0;i<1024;)
	{
		ram[i]=(color>>8)&0xff;
		ram[i+1]=color&0xff;
		i+=2;
	}
	LCD_Set_Window(sx,sy,xlen,ylen);
	
	if(xlen*ylen*2<=1024)
	{
		LCD_WriteRAMstr(ram,1024);	   
		
	}else{
		for(j=0;j<xlen*ylen*2/1024;j++)
		{
			LCD_WriteRAMstr(ram,1024);
		}
		LCD_WriteRAMstr(ram,(xlen*ylen*2)%1024);
	}
}  
//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 		//得到填充的宽度
	height=ey-sy+1;		//高度
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//设置光标位置 
		LCD_WriteRAM_Prepare();     //开始写入GRAM
		for(j=0;j<width;j++)LCD_WriteRAM(color[i*height+j]);//写入数据 
	}	  
}  

/*
画直线	type 0：横线
		type 1：竖线
*/
void LCD_DrawStraightLine(uint16_t  x,uint16_t  y,uint16_t lenth,uint8_t type)
{
	uint8_t ram[640]={0};

	if(type==0)
	{
		LCD_Set_Window(x, y, lenth, 1);
	}else{
		LCD_Set_Window(x, y, 1, lenth);
	}
	
	memset(ram,0xff,lenth*2);
	LCD_WriteRAMstr(ram,lenth*2);
	
}

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawStraightLine(x1,y1,x2-x1,0);
	LCD_DrawStraightLine(x1,y2,x2-x1,0);
	LCD_DrawStraightLine(x1,y1,y2-y1,1);
	LCD_DrawStraightLine(x2,y1,y2-y1,1);
	
//	LCD_DrawLine(x1,y1,x2,y1);
//	LCD_DrawLine(x1,y1,x1,y2);
//	LCD_DrawLine(x1,y2,x2,y2);
//	LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  

//设置窗口
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{   
	width=sx+width-1;
	height=sy+height-1;

	LCD_WR_REG(lcddev.setxcmd);
	LCD_WR_DATA(sx>>8);  
	LCD_WR_DATA(sx&0XFF);	  
	LCD_WR_DATA(width>>8);   
	LCD_WR_DATA(width&0XFF);   
	LCD_WR_REG(lcddev.setycmd);
	LCD_WR_DATA(sy>>8);   
	LCD_WR_DATA(sy&0XFF);  
	LCD_WR_DATA(height>>8);   
	LCD_WR_DATA(height&0XFF); 
	LCD_WR_REG(lcddev.wramcmd);
}

void LCD_Dischar(uint16_t x,uint16_t y,uint8_t ch,uint8_t size,uint16_t color,uint16_t bg_color)
{
	uint8_t ram[512]={0};
	uint16_t i,j,len;
	uint16_t k=0,p=0;
	uint8_t temp;
	LCD_Set_Window(x, y, size/2, size);
	if(size/2%8==0)
	{
		len=size/2*size/8;
	}else{
		len=(size/2+(8-(size/2%8)))*size/8;
	}
	
	for(i=0;i<len;i++)
	{
		switch(size)
		{
			case 12:
				temp=ascll_1206[(ch-' ')*12+k];
				break;
			case 16:
				temp=ascll_1608[(ch-' ')*16+k];
				break;
			default:
				return;
		}
		for(j=0;j<8;)
		{
			if((temp>>j)&0x01)
			{
//				LCD_WriteRAM(fColor);
				ram[p]=(color>>8)&0xff;
				ram[p+1]=color&0xff;
			}else{
//				LCD_WriteRAM(bColor);
				ram[p]=(bg_color>>8)&0xff;
				ram[p+1]=bg_color&0xff;
			}
			p+=2;
			if(((i%(size/2/8+1))==(size/2/8))&&(size/2%8!=0))
			{
				if(j>=size/2%8-1)
				{
					break;
				}
			}
			
			j++;
		}
		k++;
	}
	LCD_WriteRAMstr(ram,size*size);
}

//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址	
void LCD_ShowString(u16 x,u16 y,u8 size,char *p,uint16_t color,uint16_t bg_color)
{         
    while((*p<='~')&&(*p>=' '))//?ж???????????!
    {       
//        LCD_ShowChar(x,y,*p,size,0,color,bg_color);
		LCD_Dischar(x,y,*p,size,color,bg_color);
        x+=(size/2);
        p++;
    }  
}


void LCD_DispHZ(uint16_t x,uint16_t y,uint8_t size,uint8_t HZ, uint16_t fColor, uint16_t bColor)
{
	uint8_t ram[800]={0};
	uint32_t i = 0, j = 0,k=0,p=0;
	uint8_t temp,len;
	LCD_Set_Window(x, y, size, size);
	if(size%8==0)
	{
		len=size*size/8;
		
	}else{
		len=(size+(8-(size%8)))*size/8;
	}
	for(i=0;i<len;i++)
	{
		switch(size)
		{
			case 12:
				temp=HZ_12[HZ*len+k];
				break;
			case 16:
				temp=HZ_16[HZ*len+k];
				break;
			case 14:
				temp=HZ_14[HZ*len+k];
				break;
			case 20:
				temp=HZ_20[HZ*len+k];
				break;
			default:
				return ;
		}
		for(j=0;j<8;)
		{
			if((temp>>j)&0x01)
			{
//				LCD_WriteRAM(fColor);
				ram[p]=(fColor>>8)&0xff;
				ram[p+1]=fColor&0xff;
			}else{
//				LCD_WriteRAM(bColor);
				ram[p]=(bColor>>8)&0xff;
				ram[p+1]=bColor&0xff;
			}
			p+=2;
			if((size%8!=0)&&((i%(size/8+1))==(size/8)))
			{
				if(j>=size%8-1)
				{
					break;
				}
			}
			j++;
		}
		k++;
	}
	
	LCD_WriteRAMstr(ram,size*size*2);
}


void LCD_DispHZ16(uint16_t x,uint16_t y,uint8_t HZ, uint16_t fColor, uint16_t bColor)
{
	uint32_t i = 0, j = 0;
	LCD_Set_Window(x, y, 16, 16);
	
	for(i=0;i<32;i++)
	{
		for(j=0;j<8;j++)
		{
			if((HZ_16[HZ*32+i]>>j)&0x01)
			{
				LCD_WriteRAM(fColor);
			}else{
				LCD_WriteRAM(bColor);
			}
		}
	}
}

void LCD_DispHZ24(uint16_t x,uint16_t y,uint8_t HZ, uint16_t fColor, uint16_t bColor)
{
	uint32_t i = 0, j = 0;
	LCD_Set_Window(x, y, 24, 24);
	
	for(i=0;i<72;i++)
	{
		for(j=0;j<8;j++)
		{
			if((HZ_20[HZ*72+i]>>j)&0x01)
			{
				LCD_WriteRAM(fColor);
			}else{
				LCD_WriteRAM(bColor);
			}
		}
	}
}

void LCD_DispPic(uint16_t x,uint16_t y,uint16_t width,uint16_t height, uint8_t *p)
{
	LCD_Set_Window(x, y, width , height);
	LCD_WriteRAMstr(p,width*height*2);
}

/*
计算ram值，用color填充
*/
void cal_ram(uint8_t ram[],uint8_t m,uint8_t n,uint16_t color)
{
	uint16_t i=0,k=0;
	uint8_t len;
	if(m>5)
	{
		m=m%6;
	}
	k=136*m+n*2;
	len=(33-n)*2+1;
	for(i=k;i<k+len*2;)
	{
		ram[i]=(color>>8)&0xff;
		i++;
		ram[i]=color&0xff;
		i++;
	}
}

void pic_protect(uint16_t x,uint16_t y)
{
	uint8_t ram[816]={0};
	LCD_Set_Window(x, y, 68, 60);
	
	memset(ram,0,816);
	cal_ram(ram,0,32,RED);
	cal_ram(ram,1,30,RED);
	cal_ram(ram,2,30,RED);
	cal_ram(ram,3,29,RED);
	cal_ram(ram,4,28,RED);
	cal_ram(ram,5,28,RED);
	LCD_WriteRAMstr(ram,816);
	
	memset(ram,0,816);
	cal_ram(ram,6,27,RED);
	cal_ram(ram,7,27,RED);
	cal_ram(ram,8,26,RED);
	cal_ram(ram,9,25,RED);
	cal_ram(ram,10,25,RED);
	cal_ram(ram,11,24,RED);
	LCD_WriteRAMstr(ram,816);
	
	memset(ram,0,816);
	cal_ram(ram,12,24,RED);
	cal_ram(ram,13,23,RED);
	cal_ram(ram,14,23,RED);
	cal_ram(ram,15,22,RED);
	cal_ram(ram,16,21,RED);
	cal_ram(ram,17,21,RED);
	LCD_WriteRAMstr(ram,816);
	
	memset(ram,0,816); //**
	cal_ram(ram,18,20,RED);
	cal_ram(ram,19,20,RED);
	cal_ram(ram,20,19,RED);
	cal_ram(ram,21,18,RED);
	cal_ram(ram,22,18,RED);
	cal_ram(ram,23,17,RED);
	
	cal_ram(ram,18,32,BLACK);
	cal_ram(ram,19,31,BLACK);
	cal_ram(ram,20,31,BLACK);
	cal_ram(ram,21,31,BLACK);
	cal_ram(ram,22,31,BLACK);
	cal_ram(ram,23,31,BLACK);

	LCD_WriteRAMstr(ram,816);
	
	memset(ram,0,816);
	cal_ram(ram,24,17,RED);
	cal_ram(ram,25,16,RED);
	cal_ram(ram,26,16,RED);
	cal_ram(ram,27,15,RED);
	cal_ram(ram,28,14,RED);
	cal_ram(ram,29,14,RED);
	
	cal_ram(ram,18,31,BLACK);
	cal_ram(ram,19,31,BLACK);
	cal_ram(ram,20,31,BLACK);
	cal_ram(ram,21,31,BLACK);
	cal_ram(ram,22,31,BLACK);
	cal_ram(ram,23,31,BLACK);

	LCD_WriteRAMstr(ram,816);
	
	
	memset(ram,0,816);
	cal_ram(ram,30,13,RED);
	cal_ram(ram,31,13,RED);
	cal_ram(ram,32,12,RED);
	cal_ram(ram,33,11,RED);
	cal_ram(ram,34,11,RED);
	cal_ram(ram,35,10,RED);
	
	cal_ram(ram,18,31,BLACK);
	cal_ram(ram,19,31,BLACK);
	cal_ram(ram,20,31,BLACK);
	cal_ram(ram,21,31,BLACK);
	cal_ram(ram,22,31,BLACK);
	cal_ram(ram,23,31,BLACK);
	
	LCD_WriteRAMstr(ram,816);
	
	memset(ram,0,816);
	cal_ram(ram,36,10,RED);
	cal_ram(ram,37,9,RED);
	cal_ram(ram,38,9,RED);
	cal_ram(ram,39,8,RED);
	cal_ram(ram,40,7,RED);
	cal_ram(ram,41,7,RED);
	
	cal_ram(ram,18,31,BLACK);
	cal_ram(ram,19,31,BLACK);
	cal_ram(ram,20,31,BLACK);
	cal_ram(ram,21,31,BLACK);
	cal_ram(ram,22,31,BLACK);
	cal_ram(ram,23,31,BLACK);
	
	LCD_WriteRAMstr(ram,816);
	
	memset(ram,0,816);
	cal_ram(ram,42,6,RED);
	cal_ram(ram,43,6,RED);
	cal_ram(ram,44,5,RED);
	cal_ram(ram,45,5,RED);
	cal_ram(ram,46,4,RED);
	cal_ram(ram,47,3,RED);
	cal_ram(ram,18,31,BLACK);
	cal_ram(ram,19,31,BLACK);
	cal_ram(ram,20,32,BLACK);

	cal_ram(ram,23,32,BLACK);
	
	LCD_WriteRAMstr(ram,816);
	
	memset(ram,0,816);
	cal_ram(ram,48,3,RED);
	cal_ram(ram,49,2,RED);
	cal_ram(ram,50,2,RED);
	cal_ram(ram,51,1,RED);
	cal_ram(ram,52,1,RED);
	cal_ram(ram,53,0,RED);
	
	cal_ram(ram,18,31,BLACK);
	cal_ram(ram,19,31,BLACK);
	cal_ram(ram,20,31,BLACK);
	cal_ram(ram,21,32,BLACK);

	LCD_WriteRAMstr(ram,816);
	
	memset(ram,0,816);
	cal_ram(ram,54,0,RED);
	cal_ram(ram,55,0,RED);
	cal_ram(ram,56,1,RED);
	cal_ram(ram,57,2,RED);
	cal_ram(ram,58,4,RED);
//	cal_ram(ram,59,28,RED);

	LCD_WriteRAMstr(ram,816);
	
}

void pic_earth(uint16_t x,uint16_t y)
{
	uint8_t ram[816]={0};                           
	LCD_Set_Window(x, y, 68, 60);
	
	memset(ram,0,816);
	cal_ram(ram,0,32,YELLOW);
	cal_ram(ram,1,30,YELLOW);
	cal_ram(ram,2,30,YELLOW);
	cal_ram(ram,3,29,YELLOW);
	cal_ram(ram,4,28,YELLOW);
	cal_ram(ram,5,28,YELLOW);
	LCD_WriteRAMstr(ram,816);
	
	memset(ram,0,816);
	cal_ram(ram,6,27,YELLOW);
	cal_ram(ram,7,27,YELLOW);
	cal_ram(ram,8,26,YELLOW);
	cal_ram(ram,9,25,YELLOW);
	cal_ram(ram,10,25,YELLOW);
	cal_ram(ram,11,24,YELLOW);
	LCD_WriteRAMstr(ram,816);
	
	memset(ram,0,816);
	cal_ram(ram,12,24,YELLOW);
	cal_ram(ram,13,23,YELLOW);
	cal_ram(ram,14,23,YELLOW);
	cal_ram(ram,15,22,YELLOW);
	cal_ram(ram,16,21,YELLOW);
	cal_ram(ram,17,21,YELLOW);
	

	cal_ram(ram,21,32,BLACK);
	cal_ram(ram,22,32,BLACK);
	cal_ram(ram,23,32,BLACK);
	LCD_WriteRAMstr(ram,816);
	
	memset(ram,0,816); //**
	cal_ram(ram,18,20,YELLOW);
	cal_ram(ram,19,20,YELLOW);
	cal_ram(ram,20,19,YELLOW);
	cal_ram(ram,21,18,YELLOW);
	cal_ram(ram,22,18,YELLOW);
	cal_ram(ram,23,17,YELLOW);
	
	cal_ram(ram,18,32,BLACK);
	cal_ram(ram,19,32,BLACK);
	cal_ram(ram,20,32,BLACK);
	cal_ram(ram,21,32,BLACK);
	cal_ram(ram,22,32,BLACK);
	cal_ram(ram,23,32,BLACK);

	LCD_WriteRAMstr(ram,816);
	
	memset(ram,0,816);
	cal_ram(ram,24,17,YELLOW);
	cal_ram(ram,25,16,YELLOW);
	cal_ram(ram,26,16,YELLOW);
	cal_ram(ram,27,15,YELLOW);
	cal_ram(ram,28,14,YELLOW);
	cal_ram(ram,29,14,YELLOW);
	
	cal_ram(ram,18,32,BLACK);
	cal_ram(ram,19,32,BLACK);
	cal_ram(ram,20,32,BLACK);
	cal_ram(ram,21,32,BLACK);
	cal_ram(ram,22,32,BLACK);
	cal_ram(ram,23,32,BLACK);

	LCD_WriteRAMstr(ram,816);
	
	
	memset(ram,0,816);
	cal_ram(ram,30,13,YELLOW);
	cal_ram(ram,31,13,YELLOW);
	cal_ram(ram,32,12,YELLOW);
	cal_ram(ram,33,11,YELLOW);
	cal_ram(ram,34,11,YELLOW);
	cal_ram(ram,35,10,YELLOW);
	
	cal_ram(ram,18,32,BLACK);
	cal_ram(ram,19,32,BLACK);
	cal_ram(ram,20,32,BLACK);
	cal_ram(ram,21,32,BLACK);
	cal_ram(ram,22,32,BLACK);
	cal_ram(ram,23,32,BLACK);
	
	LCD_WriteRAMstr(ram,816);
	
	memset(ram,0,816);
	cal_ram(ram,36,10,YELLOW);
	cal_ram(ram,37,9,YELLOW);
	cal_ram(ram,38,9,YELLOW);
	cal_ram(ram,39,8,YELLOW);
	cal_ram(ram,40,7,YELLOW);
	cal_ram(ram,41,7,YELLOW);
	
	cal_ram(ram,18,32,BLACK);
	cal_ram(ram,19,23,BLACK);
	cal_ram(ram,20,23,BLACK);
	
	LCD_WriteRAMstr(ram,816);
	
	memset(ram,0,816);
	cal_ram(ram,42,6,YELLOW);
	cal_ram(ram,43,6,YELLOW);
	cal_ram(ram,44,5,YELLOW);
	cal_ram(ram,45,5,YELLOW);
	cal_ram(ram,46,4,YELLOW);
	cal_ram(ram,47,3,YELLOW);
	
	cal_ram(ram,18,25,BLACK);
	cal_ram(ram,19,25,BLACK);
	
	cal_ram(ram,23,27,BLACK);
	
	LCD_WriteRAMstr(ram,816);
	
	memset(ram,0,816);
	cal_ram(ram,48,3,YELLOW);
	cal_ram(ram,49,2,YELLOW);
	cal_ram(ram,50,2,YELLOW);
	cal_ram(ram,51,1,YELLOW);
	cal_ram(ram,52,1,YELLOW);
	cal_ram(ram,53,0,YELLOW);
	
	cal_ram(ram,18,27,BLACK);

	LCD_WriteRAMstr(ram,816);
	
	memset(ram,0,816);
	cal_ram(ram,54,0,YELLOW);
	cal_ram(ram,55,0,YELLOW);
	cal_ram(ram,56,1,YELLOW);
	cal_ram(ram,57,2,YELLOW);
	cal_ram(ram,58,4,YELLOW);
//	cal_ram(ram,59,28,YELLOW);

	LCD_WriteRAMstr(ram,816);
	
}


