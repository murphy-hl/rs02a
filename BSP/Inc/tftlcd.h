#ifndef __LCD_H
#define __LCD_H		

#include "main.h"	 
#include "stdlib.h" 
 
#define  LCD_WIDTH		240
#define  LCD_HEIGHT     320
//LCD��Ҫ������
typedef struct  
{		 	 
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������	
	u16	wramcmd;		//��ʼдgramָ��
	u16 setxcmd;		//����x����ָ��
	u16 setycmd;		//����y����ָ�� 
}_lcd_dev; 	  

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����
//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern u32  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u32  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ
 			    
#define Bank1_LCD_DATA 		((uint32_t)0x60020000)	/* display data address */
#define Bank1_LCD_REG		((uint32_t)0x6001FFFE)	/* display register address */

#define LCD_WR_DATA(value)	((*(__IO uint16_t*)(Bank1_LCD_DATA)) = ((uint16_t)(value)))
#define LCD_WR_REG(index)	((*(__IO uint16_t*)(Bank1_LCD_REG)) = ((uint16_t)index))
//#define LCD_RD_DATA			((uint16_t)(*(__IO uint16_t*)(Bank1_LCD_DATA)))

//ɨ�跽����
#define L2R_U2D  0 		//������,���ϵ���
#define L2R_D2U  1 		//������,���µ���
#define R2L_U2D  2 		//���ҵ���,���ϵ���
#define R2L_D2U  3 		//���ҵ���,���µ���

#define U2D_L2R  4 		//���ϵ���,������
#define U2D_R2L  5 		//���ϵ���,���ҵ���
#define D2U_L2R  6 		//���µ���,������
#define D2U_R2L  7		//���µ���,���ҵ���	 

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	    						


void LCD_Init(void);													   	//��ʼ��
void LCD_DisplayOn(void);													//����ʾ
void LCD_DisplayOff(void);													//����ʾ

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//��ʾһ���ַ�
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//��ʾһ������
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//��ʾ ����
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//��ʾһ���ַ���,12/16����

u16 LCD_RD_DATA(void);
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u16 LCD_Reg);
void LCD_Scan_Dir(u8 dir);									//������ɨ�跽��
void LCD_Display_Dir(u8 dir);								//������Ļ��ʾ����

void LCD_SetCursor(uint16_t x, uint16_t y);
void LCD_Color_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t *color);
void LCD_OpenWindow(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void LCD_FillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
void LCD_FillScreen(uint16_t color);
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_DrawLine(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, uint16_t color);
void LCD_DrawRectangle(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, uint16_t color);
void LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color);
void LCD_DispPicture1(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t* str);

void LCD_DispHZ16(uint16_t x,uint16_t y,uint8_t HZ, uint16_t fColor, uint16_t bColor);
void LCD_DispHZ24(uint16_t x,uint16_t y,uint8_t HZ, uint16_t fColor, uint16_t bColor);
void LCD_DispHZ32(uint16_t x,uint16_t y,uint8_t HZ, uint16_t fColor, uint16_t bColor);


#endif  
	 
	 



