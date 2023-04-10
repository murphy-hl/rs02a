#include "24cxx.h" 
#include "string.h"
#include "rtc.h"
#include "delay.h"

#define    SOFT_VERSION    0x03



void SYS_Param_Init(void)
{
	u8 i = 20;
	u8 result = 0;
	u8 temp_workmode=0;
	
	while(i--)
	{
		result=AT24CXX_ReadOneByte(0);
		if(result == SOFT_VERSION)
		{
			break;
		}
		else
			delay_ms(10);
	}
	if(result != SOFT_VERSION)
	{
		AT24CXX_WriteOneByte(0,SOFT_VERSION);
		AT24CXX_WriteOneByte(10,temp_workmode);
	}
	delay_ms(50);
	work_mode=AT24CXX_ReadOneByte(10);
	
}

//初始化IIC接口
void AT24CXX_Init(void)
{
	//GPIOB8,B9初始化设置
	GPIO_InitTypeDef GPIO_InitStruct = {0};	
	GPIO_InitStruct.Pin = SCL|SDA;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(IIC_PORT, &GPIO_InitStruct);
  
	IIC_SCL_H;
	IIC_SDA_H;
}
//产生IIC起始信号
void IIC_Start(void)
{
	IIC_SDA_H;	  	  
	IIC_SCL_H;
	delay_us(4);
 	IIC_SDA_L;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_L;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	IIC_SCL_L;
	IIC_SDA_L;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_H; 
	IIC_SDA_H;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0; 
	IIC_SDA_H;delay_us(1);	   
	IIC_SCL_H;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_L;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL_L;
	IIC_SDA_L;
	delay_us(2);
	IIC_SCL_H;
	delay_us(2);
	IIC_SCL_L;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL_L;
	IIC_SDA_H;
	delay_us(2);
	IIC_SCL_H;
	delay_us(2);
	IIC_SCL_L;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;    	    
    IIC_SCL_L;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
//        IIC_SDA=(txd&0x80)>>7;
		if(((txd&0x80)>>7))	
		{
				IIC_SDA_H;
		}else{   
			IIC_SDA_L;
		}
        txd<<=1; 	  
				delay_us(2);   //对TEA5767这三个延时都是必须的
				IIC_SCL_H;
				delay_us(2); 
				IIC_SCL_L;	
				delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
  for(i=0;i<8;i++ )
	{
    IIC_SCL_L; 
    delay_us(2);
		IIC_SCL_H;
    receive<<=1;
    if(READ_SDA)receive++;   
		delay_us(1); 
   }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}
//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	IIC_Send_Byte(0XA0);   //发送器件地址0XA0,写数据 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr&0xff);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA0|0x01); //进入接收模式			
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
IIC_Send_Byte(0XA0);   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr&0xff);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 
}
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}													    
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(0x0000);//避免每次开机都写AT24CXX			   
	if(temp==0X01)return 0;		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(0x0000,0X01);
	    temp=AT24CXX_ReadOneByte(0x0000);	 
		
//		AT24CXX_Write(10,Init_eeprom,100);
		
		if(temp==0X01)return 0;
	}
	return 1;											  
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}








