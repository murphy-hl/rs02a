#ifndef _LCD_H
#define _LCD_H

#include "main.h"

#define  	LCD_CS_PIN			GPIO_PIN_11
#define  	LCD_CS_PORT			GPIOB

#define  	LCD_DC_PIN			GPIO_PIN_10
#define	 	LCD_DC_PORT			GPIOB

#define  	LCD_RST_PIN			GPIO_PIN_6
#define  	LCD_RST_PORT		GPIOC

#define  	LCD_SCL_PIN			GPIO_PIN_13
#define  	LCD_SCL_PORT		GPIOB

#define  	LCD_SDA_PIN			GPIO_PIN_15
#define  	LCD_SDA_PORT		GPIOB


#define 	LCD_CS_H			HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET)
#define 	LCD_CS_L			HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_CLEAR)

#define 	LCD_DC_H			HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET)
#define 	LCD_DC_L			HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_CLEAR)

#define 	LCD_RST_H			HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET)
#define 	LCD_RST_L			HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_CLEAR)

//#define 	LCD_SDA_H			HAL_GPIO_WritePin(LCD_SDA_PORT, LCD_SDA_PIN, GPIO_PIN_SET)
//#define 	LCD_SDA_L			HAL_GPIO_WritePin(LCD_SDA_PORT, LCD_SDA_PIN, GPIO_PIN_CLEAR)

//#define 	LCD_SCL_H			HAL_GPIO_WritePin(LCD_SCL_PORT, LCD_SCL_PIN, GPIO_PIN_SET)
//#define 	LCD_SCL_L			HAL_GPIO_WritePin(LCD_SCL_PORT, LCD_SCL_PIN, GPIO_PIN_CLEAR)

#define		LCD_WIDTH			240
#define		LCD_HEIGHT			320

//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u8	wramcmd;		//开始写gram指令
	u8  setxcmd;		//设置x坐标指令
	u8  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	  

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色	   


//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	 

extern u8 DFT_SCAN_DIR;


//画笔颜色
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
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
void LCD_Init(void);													   	//初始化
void LCD_DisplayOn(void);													//开显示
void LCD_DisplayOff(void);													//关显示
void LCD_Clear(u16 Color);	 												//清屏
void LCD_SetCursor(u16 Xpos, u16 Ypos);										//设置光标
void LCD_DrawPoint(u16 x,u16 y);											//画点
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);								//快速画点
u16  LCD_ReadPoint(u16 x,u16 y); 											//读点 
void Draw_Circle(u16 x0,u16 y0,u8 r);										//画圆
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);							//画线
void LCD_DrawStraightLine(uint16_t  x,uint16_t  y,uint16_t lenth,uint8_t type);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   				//画矩形
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);		   				//填充单色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				//填充指定颜色
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//显示一个字符
void LCD_Dischar(uint16_t x,uint16_t y,uint8_t ch,uint8_t size,uint16_t color,uint16_t bg_color);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//显示一个数字
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//显示 数字

void LCD_ShowString(u16 x,u16 y,u8 size,char *p,uint16_t color,uint16_t bg_color);
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);
void IO_init(void);


void LCD_WR_REG(u8);
void LCD_WR_DATA(u16);
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u8 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
void LCD_WriteRAMstr(u8 data[],u16 len);
void LCD_Scan_Dir(u8 dir);							//设置屏扫描方向
void LCD_Display_Dir(u8 dir);						//设置屏幕显示方向
 					   					
void LCD_DispHZ16(uint16_t x,uint16_t y,uint8_t HZ, uint16_t fColor, uint16_t bColor);
void LCD_DispHZ24(uint16_t x,uint16_t y,uint8_t HZ, uint16_t fColor, uint16_t bColor);
void LCD_DispHZ32(uint16_t x,uint16_t y,uint8_t HZ, uint16_t fColor, uint16_t bColor);
void LCD_DispHZ(uint16_t x,uint16_t y,uint8_t size,uint8_t HZ, uint16_t fColor, uint16_t bColor);
void LCD_DispPic(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t *p);
				
void pic_protect(uint16_t x,uint16_t y);
void pic_earth(uint16_t x,uint16_t y);

#endif


