#ifndef __LCD_H
#define __LCD_H		

#include "main.h"	 
#include "stdlib.h" 
 
#define  LCD_WIDTH		240
#define  LCD_HEIGHT     320
//LCD重要参数集
typedef struct  
{		 	 
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wramcmd;		//开始写gram指令
	u16 setxcmd;		//设置x坐标指令
	u16 setycmd;		//设置y坐标指令 
}_lcd_dev; 	  

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色	   
extern u32  POINT_COLOR;//默认红色    
extern u32  BACK_COLOR; //背景颜色.默认为白色
 			    
#define Bank1_LCD_DATA 		((uint32_t)0x60020000)	/* display data address */
#define Bank1_LCD_REG		((uint32_t)0x6001FFFE)	/* display register address */

#define LCD_WR_DATA(value)	((*(__IO uint16_t*)(Bank1_LCD_DATA)) = ((uint16_t)(value)))
#define LCD_WR_REG(index)	((*(__IO uint16_t*)(Bank1_LCD_REG)) = ((uint16_t)index))
//#define LCD_RD_DATA			((uint16_t)(*(__IO uint16_t*)(Bank1_LCD_DATA)))

//扫描方向定义
#define L2R_U2D  0 		//从左到右,从上到下
#define L2R_D2U  1 		//从左到右,从下到上
#define R2L_U2D  2 		//从右到左,从上到下
#define R2L_D2U  3 		//从右到左,从下到上

#define U2D_L2R  4 		//从上到下,从左到右
#define U2D_R2L  5 		//从上到下,从右到左
#define D2U_L2R  6 		//从下到上,从左到右
#define D2U_R2L  7		//从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

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
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
	    						


void LCD_Init(void);													   	//初始化
void LCD_DisplayOn(void);													//开显示
void LCD_DisplayOff(void);													//关显示

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//显示一个数字
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//显示 数字
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//显示一个字符串,12/16字体

u16 LCD_RD_DATA(void);
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u16 LCD_Reg);
void LCD_Scan_Dir(u8 dir);									//设置屏扫描方向
void LCD_Display_Dir(u8 dir);								//设置屏幕显示方向

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
	 
	 



