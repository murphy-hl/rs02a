#include "tftlcd.h"
#include "stdlib.h"
#include "font.h" 
#include "uart.h"	 
#include "delay.h"	 
#include "fmc.h"

//LCD�Ļ�����ɫ�ͱ���ɫ	   
u32 POINT_COLOR=0xFF000000;		//������ɫ
u32 BACK_COLOR =0xFFFFFFFF;  	//����ɫ 

//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;
  
//��ʼ��lcd
//�ó�ʼ���������Գ�ʼ�������ͺŵ�LCD(�����.c�ļ���ǰ�������)
void LCD_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
	
	FMC_Init();	
	delay_ms(50);
	//����9341 ID�Ķ�ȡ		
	LCD_WR_REG(0XD3);				   
	lcddev.id=LCD_RD_DATA();	//dummy read 	
	lcddev.id=LCD_RD_DATA();	//����0X00
	lcddev.id=LCD_RD_DATA();   	//��ȡ93								   
	lcddev.id<<=8;
	lcddev.id|=LCD_RD_DATA();  	//��ȡ41 	   			   
	
	printf(" LCD ID:%x\r\n",lcddev.id); //��ӡLCD ID   
	 
	LCD_WR_REG(0xCF);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x81); 
	LCD_WR_DATA(0X30);
	
	LCD_WR_REG(0xED);  			//power on sequence control 
	LCD_WR_DATA(0x64); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0X12); 
	LCD_WR_DATA(0X81);
	
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA(0x85); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x78);
	
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x34); 
	LCD_WR_DATA(0x02);
	
	LCD_WR_REG(0xF7);  			//pump retio control
	LCD_WR_DATA(0x20); 
	
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA(0x1B);   //VRH[5:0] 
	
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA(0x10);   //SAP[2:0];BT[3:0] 
	
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA(0x2D); 	 //3F
	LCD_WR_DATA(0x33); 	 //3C
	
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA(0XCF); 
	
	LCD_WR_REG(0x36);    // Memory Access Control 
	LCD_WR_DATA(0x08); 
	LCD_WR_REG(0x3A);   
	LCD_WR_DATA(0x55); 
	
	LCD_WR_REG(0xB1);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x1B); 
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0xA2); 
	
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA(0x00); 
	
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA(0x01); 
	
	LCD_WR_REG(0XE0); 
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x26);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x0E);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x54);
	LCD_WR_DATA(0xA8);
	LCD_WR_DATA(0x46);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x00);
	
	/* negative gamma correction */
	LCD_WR_REG(0XE1); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x19);
	LCD_WR_DATA(0x1B);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x2A);
	LCD_WR_DATA(0x47);
	LCD_WR_DATA(0x39);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x30);
	LCD_WR_DATA(0x38);
	LCD_WR_DATA(0x0F);

	LCD_WR_REG(0x2B); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3f);
	
	LCD_WR_REG(0x2A); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);	
	
	LCD_WR_REG(0x11); //Exit Sleep
	delay_ms(20);
	LCD_WR_REG(0x29); //display on	
	
	LCD_WR_REG(0x2c); 

//	LCD_Display_Dir(0);		//Ĭ��Ϊ����
	LCD_FillScreen(WHITE);
}  


//��LCD����
////����ֵ:������ֵ
u16 LCD_RD_DATA(void)
{		
	uint16_t temp;
	temp=(*(__IO uint16_t*)(Bank1_LCD_DATA));
	return temp;		 
}					   
//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);   		 
}	   
//���Ĵ���
//LCD_Reg:�Ĵ�����ַ
//����ֵ:����������
u16 LCD_ReadReg(u16 LCD_Reg)
{			
	LCD_WR_REG(LCD_Reg);		//д��Ҫ���ļĴ������
	delay_us(5);		  
	return LCD_RD_DATA();		//���ض�����ֵ
}   

//��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f; 
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 

	 
//LCD������ʾ
void LCD_DisplayOn(void)
{					   
	LCD_WR_REG(0X29);	//������ʾ
}	 
//LCD�ر���ʾ
void LCD_DisplayOff(void)
{	   
	LCD_WR_REG(0X28);	//�ر���ʾ
}   
		 
//����LCD���Զ�ɨ�跽��(��RGB����Ч)
//ע��:�����������ܻ��ܵ��˺������õ�Ӱ��(������9341),
//����,һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
//dir:0~7,����8������(���嶨���lcd.h)
//9341/5310/5510/1963��IC�Ѿ�ʵ�ʲ���	   	   
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	u16 temp;  
 
	switch(dir)
	{
		case L2R_U2D://������,���ϵ���
			regval|=(0<<7)|(0<<6)|(0<<5); 
			break;
		case L2R_D2U://������,���µ���
			regval|=(1<<7)|(0<<6)|(0<<5); 
			break;
		case R2L_U2D://���ҵ���,���ϵ���
			regval|=(0<<7)|(1<<6)|(0<<5); 
			break;
		case R2L_D2U://���ҵ���,���µ���
			regval|=(1<<7)|(1<<6)|(0<<5); 
			break;	 
		case U2D_L2R://���ϵ���,������
			regval|=(0<<7)|(0<<6)|(1<<5); 
			break;
		case U2D_R2L://���ϵ���,���ҵ���
			regval|=(0<<7)|(1<<6)|(1<<5); 
			break;
		case D2U_L2R://���µ���,������
			regval|=(1<<7)|(0<<6)|(1<<5); 
			break;
		case D2U_R2L://���µ���,���ҵ���
			regval|=(1<<7)|(1<<6)|(1<<5); 
			break;	 
	}
	dirreg=0X36;
	regval|=0X08;
	LCD_WriteReg(dirreg,regval);

	if(regval&0X20)
	{
		if(lcddev.width<lcddev.height)//����X,Y
		{
			temp=lcddev.width;
			lcddev.width=lcddev.height;
			lcddev.height=temp;
		}
	}else  
	{
		if(lcddev.width>lcddev.height)//����X,Y
		{
			temp=lcddev.width;
			lcddev.width=lcddev.height;
			lcddev.height=temp;
		}
	}  

	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(0);LCD_WR_DATA(0);
	LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(0);LCD_WR_DATA(0);
	LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
	
}     

//����LCD��ʾ����
//dir:0,������1,����
void LCD_Display_Dir(u8 dir)
{
	lcddev.dir=dir;		//����/���� 
	if(dir==0)			//����
	{
		lcddev.width=240;
		lcddev.height=320;

		lcddev.wramcmd=0X2C;
		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 
	}else 				//����
	{	  				 
		lcddev.width=320;
		lcddev.height=240;

		lcddev.wramcmd=0X2C;
		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	//Ĭ��ɨ�跽��
}	 



//��������
//color:Ҫ���������ɫ
void LCD_OpenWindow(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	/* coloum address set */
	LCD_WR_REG(0X2A); 
	/* coloum start */
	LCD_WR_DATA(x>>8);		/* high byte */
	LCD_WR_DATA(x&0x00FF);	/* low byte */
	/* coloum end */
	LCD_WR_DATA((x+width-1)>>8);		/* high byte */
	LCD_WR_DATA((x+width-1)&0x00FF);	/* low byte */
	
	/* page address set */
	LCD_WR_REG(0X2B); 
	/* pate start */
	LCD_WR_DATA(y>>8);			/* high byte */   
	LCD_WR_DATA(y&0x00FF);		/* low byte */
	/* pate end */
	LCD_WR_DATA((y+height-1)>>8);  	/* high byte */
	LCD_WR_DATA((y+height-1)&0x00FF);	/* low byte */
	
	/* memory write */
	LCD_WR_REG(0x2C); 
}
void LCD_FillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
	uint32_t i = 0;
	LCD_OpenWindow(x, y, height, width);
	for(i = width*height; i >0 ; --i)
	{
		LCD_WR_DATA(color);
	}
}
/*
*FuncName:	LCD_FillScreen
*Parameter:	uint16_t color	fill color
*Function:	fill whole screen
*/
void LCD_FillScreen(uint16_t color)
{
	LCD_FillRectangle(0, 0, 320, 240, color);
}

// ���ù��λ��
void LCD_SetCursor(uint16_t x, uint16_t y)
{
	/* coloum address set */
	LCD_WR_REG(0X2A); 
	/* coloum start */
	LCD_WR_DATA(x>>8);		/* high byte */
	LCD_WR_DATA(x&0x00FF);	/* low byte */
	/* coloum end */
	LCD_WR_DATA((LCD_WIDTH-1)>>8);		/* high byte */
	LCD_WR_DATA((LCD_WIDTH-1)&0x00FF);	/* low byte */
	
	/* page address set */
	LCD_WR_REG(0X2B); 
	/* pate start */
	LCD_WR_DATA(y>>8);			/* high byte */   
	LCD_WR_DATA(y&0x00FF);		/* low byte */
	/* pate end */
	LCD_WR_DATA((LCD_HEIGHT-1)>>8);  	/* high byte */
	LCD_WR_DATA((LCD_HEIGHT-1)&0x00FF);	/* low byte */
	
	/* memory write */
	LCD_WR_REG(0x2C); 
}

//��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Color_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t *color)
{  
    uint16_t i,j; 
    uint16_t height,width;     
    width = x_end - x_start + 1;          
    height = y_end - y_start + 1;   
    
    for(i=0; i<height; i++)   
    {   
        LCD_SetCursor(x_start, y_start+i);     
        for(j=0; j<width; j++)
        {
            LCD_WR_DATA(color[i*width+j]);
        }
    }  	  
} 


void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
	LCD_OpenWindow(x, y, 1, 1);
	LCD_WR_DATA(color);
}
/*
*FuncName:	LCD_DrawLine
*Parameter:	uint16_t start_x	start x coordinate
			uint16_t start_y	start y coordinate
			uint16_t end_x		end x coordinate
			uint16_t end_y		end y coordinate
			uint16_t color		line color
*Function:	draw line on LCD
*/
void LCD_DrawLine(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, uint16_t color)
{
	uint16_t x = 0, y = 0, k = 0;
	
	if((start_x == end_x) && (start_y == end_y))
	{
		LCD_DrawPoint(start_x, start_y, color);
	}
	else if(abs(end_y - start_y) > abs(end_x - start_x))
	{
		if(start_y > end_y)
		{
			k = start_y;
			start_y = end_y;
			end_y = k;
			
			k = start_x;
			start_x = end_x;
			end_x = k;
		}
		for(y = start_y; y < end_y; ++y)
		{
			x = (uint16_t)(y - start_y)*(end_x - start_x) / (end_y - start_y) + start_x;
			LCD_DrawPoint(x, y, color);
		}
	}
	else
	{
		if(start_x > end_x)
		{
			k = start_y;
			start_y = end_y;
			end_y = k;
			
			k = start_x;
			start_x = end_x;
			end_x = k;
		}
		for(x = start_x; x < end_x; ++x)
		{
			y = (uint16_t)(x - start_x)*(end_y - start_y)/ (end_x - start_x) + start_y;
			LCD_DrawPoint(x, y, color);
		}
	}
}

/*
*FuncName:	LCD_DrawRectangle
*Parameter:	uint16_t start_x	start x coordinate
			uint16_t start_y	start y coordinate
			uint16_t end_x		end x coordinate
			uint16_t end_y		end y coordinate
			uint16_t color		rectangle color
*Function:	draw rectangle on LCD
*/
void LCD_DrawRectangle(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, uint16_t color)
{
	/*
	top-left coordinate(start_x, start_y), bottom-right coordinate(end_x, end_y) 
	 '----------------'
	 '                '
	 '                '
	 '                '
	 '                '
	 '----------------'
	*/
	LCD_DrawLine(start_x, start_y, start_x, end_y, color);
	LCD_DrawLine(start_x, start_y, end_x, start_y, color);
	LCD_DrawLine(end_x, start_y, end_x, end_y, color);
	LCD_DrawLine(start_x, end_y, end_x, end_y, color);
}

/*
*FuncName:	LCD_DrawCircle
*Parameter:	uint16_t x		center x coordinate
			uint16_t y		center y coordinate
			uint16_t radius	circle radius
			uint16_t color	rectangle color
*Function:	draw circle on LCD
*/
void LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color)
{
	uint16_t a,b;
	int16_t di;
	a = 0;
	b = radius;          
	di = 3 - (radius << 1); 
	while(a <= b)
	{
		LCD_DrawPoint(x - b, y - a, color);             //3           
		LCD_DrawPoint(x + b, y - a, color);             //0           
		LCD_DrawPoint(x - a, y + b, color);             //1              
		LCD_DrawPoint(x - a, y - b, color);             //2             
		LCD_DrawPoint(x + b, y + a, color);             //4               
		LCD_DrawPoint(x + a, y - b, color);             //5
		LCD_DrawPoint(x + a, y + b, color);             //6 
		LCD_DrawPoint(x - b, y + a, color);             
		a++;
		/* Bresenham algorithm */ 
		if(di < 0)
		{
			di += 4*a + 6;   
		}			
		else
		{
			di += 10 + 4 * (a - b);   
			b--;
		} 
		LCD_DrawPoint(x + a, y + b, color);
	}
}

/*
*FuncName:	LCD_DispPicture1
*Parameter:	uint16_t x			x coordinate
			uint16_t y			y coordinate
			uint16_t width		the picture width
			uint16_t height		the picture height
			uint8_t* str		the pointer of data array
*Function:	display picture on LCD
*/
void LCD_DispPicture1(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t* str)
{
	uint32_t i = 0, j = 0;
	
	LCD_OpenWindow(x, y, width - x , height -y);
	uint16_t tmp;
	j = (width - x)  * (height -y);
	for (i = 0;i < j; i++)
	{
		tmp = (str[i*2] | str[i*2+1]<<8);
		LCD_WR_DATA(tmp);
	}
}


//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24/32
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else if(size==32)temp=asc2_3216[num][t];	//����3216����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=LCD_HEIGHT)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=LCD_WIDTH)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

void LCD_DispHZ16(uint16_t x,uint16_t y,uint8_t HZ, uint16_t fColor, uint16_t bColor)
{
	uint32_t i = 0, j = 0, k = 0;
	LCD_OpenWindow(x, y, 16, 16);
//	LCD_FillRectangle(x, y, 16, 16, bColor);
	
	for(i=0;i<32;i++)
	{
		for(j=0;j<8;j++)
		{
			if((HZ_16[HZ*32+i]>>j)&0x01)
			{
				LCD_WR_DATA(fColor);
			}else{
				LCD_WR_DATA(bColor);
			}
		}
	}
}

void LCD_DispHZ24(uint16_t x,uint16_t y,uint8_t HZ, uint16_t fColor, uint16_t bColor)
{
	uint32_t i = 0, j = 0, k = 0;
	LCD_OpenWindow(x, y, 24, 24);
//	LCD_FillRectangle(x, y, 16, 16, bColor);
	
	for(i=0;i<72;i++)
	{
		for(j=0;j<8;j++)
		{
			if((HZ_24[HZ*72+i]>>j)&0x01)
			{
				LCD_WR_DATA(fColor);
			}else{
				LCD_WR_DATA(bColor);
			}
		}
	}
}

void LCD_DispHZ32(uint16_t x,uint16_t y,uint8_t HZ, uint16_t fColor, uint16_t bColor)
{
	uint32_t i = 0, j = 0, k = 0;
	LCD_OpenWindow(x, y, 32, 32);
//	LCD_FillRectangle(x, y, 16, 16, bColor);
	
	for(i=0;i<128;i++)
	{
		for(j=0;j<8;j++)
		{
			if((HZ_32[HZ*72+i]>>j)&0x01)
			{
				LCD_WR_DATA(fColor);
			}else{
				LCD_WR_DATA(bColor);
			}
		}
	}
}


#if 0
/*
*FuncName:	LCD_DispASCIICharacter
*Parameter:	uint16_t x			x coordinate
			uint16_t y			y coordinate
			uint8_t character	character
			uint16_t fColor		front color
			uint16_t bColor		background color
			uint8_t font		the macro of font size
*Function:	display ASCII character on LCD
*/
void LCD_DispASCIICharacter(uint16_t x, uint16_t y, uint8_t character, uint16_t fColor, uint16_t bColor, uint8_t font)
{
	uint16_t width = 0, height = 0;
	uint32_t i = 0, j = 0, k = 0;
	
	height = GetFontWidth(font, ASCII_CHARACTER);
	width = GetFontHeight(font, ASCII_CHARACTER);
	
	LCD_OpenWindow(x, y, x + width - 1, y + height - 1);
	LCD_FillRectangle(x, y, width, height, bColor);
	
	switch(font)
	{
		case FONT_SIZE_24:
			for(k = 0; k < sizeof(codeASCII_24) / sizeof(codeASCII_24[0]); ++k) //����
			{
				if(codeASCII_24[k].Index == character)
				{
					for(i = 0; i < 48; ++i) 	// ��������
					{
						uint8_t m = codeASCII_24[k].Msk[i];
						if(i%2)
						{
							for(j = 0; j < 4; ++j)	
							{
								if(m & 0x80)
								{
									LCD_WR_DATA(fColor);
								}
								else 
								{
									LCD_WR_DATA(bColor);
								}
								m <<= 1;
							}
						}
						else
						{
							for(j = 0; j< 8; ++j)
							{
								if(m & 0x80)
								{
									LCD_WR_DATA(fColor);
								}
								else
								{
									LCD_WR_DATA(bColor);
								}
								m <<= 1;
							}
						}
					}
				}
			}
			break;
		default:
			break;
	}
}

/*
*FuncName:	LCD_DispHZCharacter
*Parameter:	uint16_t x			x coordinate
			uint16_t y			y coordinate
			uint8_t index[2]	HZ character
			uint16_t fColor		front color
			uint16_t bColor		background color
			uint8_t font		the macro of font size
*Function:	display HZ character on LCD
*/
void LCD_DispHZCharacter(uint16_t x, uint16_t y, uint8_t index[2], uint16_t fColor, uint16_t bColor, uint8_t font)
{
	uint16_t width = 0, height = 0;
	uint32_t i = 0, j = 0, k = 0;
	
	width = GetFontWidth(font, HZ_CHARACTER);
	height = GetFontHeight(font, HZ_CHARACTER);
	LCD_OpenWindow(x, y, x + width - 1, y + height - 1);
	LCD_FillRectangle(x, y, width, height, bColor);
	
	switch(font)
	{
		case FONT_SIZE_24:
			for(k = 0; k < sizeof(codeHZ_24) / sizeof(codeHZ_24[0]); ++k)
			{
				if((codeHZ_24[k].Index[0] == index[0]) && (codeHZ_24[k].Index[1] == index[1]))
				{
					for(i = 0; i < 72; ++i)
					{
						uint8_t m = codeHZ_24[k].Msk[i];
						for(j = 0; j < 8; ++j)
						{
							if(m & 0x80)
							{
								LCD_WR_DATA(fColor);
							}
							else
							{
								LCD_WR_DATA(bColor);
							}
							m <<= 1;
						}
					}
					break;
				}
			}
			break;
		default:
			break;
	}
}




#endif






















