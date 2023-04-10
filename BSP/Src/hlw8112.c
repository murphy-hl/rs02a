#include "hlw8112.h"
#include "uart.h"
#include "delay.h"


hlw8112_para_typedef hlw8112_para;


void hlw8112_init(void)
{
	//地址00 写入0f274
	hlw8112_enwrite();
	delay_ms(10);
	hlw8112_special_cmd(CHANNEL_A);
	delay_ms(10);

	hlw8112_W_reg(REG_SYSCON_ADDR,0x0f04,2);		//通道A、B、电压 开启  电压增益为1 电流增益为16
	hlw8112_W_reg(REG_EMUCON_ADDR,0x1001,2);		// 
	hlw8112_W_reg(REG_EMUCON2_ADDR,0x04e5,2);		// 
	hlw8112_W_reg(REG_INT_ADDR,0x3282,2);			// INT2 有效值更新  INT1 漏电指示信号
	hlw8112_W_reg(REG_IE_ADDR,0x8001,2);			//使能漏电中断和均值更新
	
	hlw8112_diswrite();
	hlw8112_check_para();
}


float  hlw8112_get_RmsIA(void)
{
	uint32_t rmsIA=0;
	float val;
	rmsIA=hlw8112_R_reg(REG_RMSIA_ADDR,3);
	
	if((rmsIA & 0x800000) == 0x800000)
	{
		return 0;
	}else{
		val=rmsIA*1.00*hlw8112_para.RmsIAC;
		val/=0x800000;
		val/=1000;		//mA转化为A
		val/=0.5;	//K1=0.5
	}
	return val;
}
float  hlw8112_get_RmsIB(void)
{
	uint32_t rmsIB=0;
	float val;
	rmsIB=hlw8112_R_reg(REG_RMSIB_ADDR,3);
	if((rmsIB & 0x800000) == 0x800000)
	{
		return 0;
	}else{
		val=rmsIB*1.00*hlw8112_para.RmsIBC;
		val/=0x800000;
		val/=1000;		//mA转化为A
		val/=5;	//K1=0.5
	}
	
	return val;
}
float  hlw8112_get_RmsU(void)
{
	uint32_t rmsU=0;
	float val;
	rmsU=hlw8112_R_reg(REG_RMSU_ADDR,3);
	if((rmsU & 0x800000) == 0x800000)
	{
		return 0;
	}else{
		val=rmsU*1.00*hlw8112_para.RmsUC;
		val/=0x400000;
		val/=100;	//mV转换为V
		val/=0.05;//K2=0.05
	}
	
	return val;
}
float hlw8112_get_ufre(void)
{
	uint16_t fre_reg;
	float fre;
	fre_reg=hlw8112_R_reg(REG_UFREQ_ADDR,2);
	fre=3579000/8.00/fre_reg;
	return fre;
}
float hlw8112_get_RmsP(void)
{
	uint32_t rmsP=0;
	float val;
	rmsP=hlw8112_R_reg(REG_POWER_PA_ADDR,4);
	
	if(rmsP > 0x80000000)
	{
		val=~rmsP;
	}else{
		val=rmsP;
	}
	
	val=rmsP*1.00*hlw8112_para.PowerPAC;
	val/=0x80000000;
	val/=0.5;
	val/=0.05;
	return val;
	
}

float hlw8112_get_RmsE(void)
{
	uint32_t rmsE=0;
	uint32_t HFconst=0;
	float val;
	
	rmsE=hlw8112_R_reg(REG_ENERGY_PA_ADDR,3);
	HFconst=hlw8112_R_reg(REG_HFCONST_ADDR,2);
//	if((rmsE & 0x800000) == 0x800000)
//	{
//		return 0;
//	}else{
//		
//	}
	
	val=rmsE*1.00*hlw8112_para.EnergyAC*HFconst;
	val=val/0x200000000/4096/0.05/0.5;			//单位是度
	val/=1;
	
	return val;
	
}


void U32ToU8(uint32_t data,uint8_t *dat)
{
	dat[0]=data;
	dat[1]=data>>8;
	dat[2]=data>>16;
	dat[3]=data>>24;
}

void hlw8112_W_reg(uint8_t reg,uint32_t para,uint8_t len)
{
	uint8_t tx_buf[10]={0xa5};
	uint8_t frame_len=2;
	uint8_t data[4]={0};
	
	tx_buf[1]=reg|0x80;
	U32ToU8(para,data);
	
	for(uint8_t i=0;i<len;i++)
	{
		
		tx_buf[i+2]=data[len-1-i];
	}
	
	frame_len+=len;
	
	tx_buf[frame_len]=get_nsum(tx_buf,frame_len);
	HAL_UART_Transmit(&huart1,tx_buf,frame_len+1,1000);
}

uint32_t  hlw8112_R_reg(uint8_t reg,uint8_t len)
{
	uint8_t tx_buf[10]={0xa5};
	uint8_t rx_buf[10]={0};
//	uint8_t frame_len=2;
//	uint8_t data[4]={0};
	uint32_t para=0;
	tx_buf[1]=reg;
	
	HAL_UART_Transmit(&huart1,tx_buf,2,1000);
	if(HAL_UART_Receive(&huart1,rx_buf,len+1,0xffff)!=HAL_OK)
	{
		return 0;
	}
	for(uint8_t i=0;i<len;i++)
	{
		para=para<<8;
//		para&=0xffffff00;
		para|=rx_buf[i];
	}
	return para;
}


void hlw8112_special_cmd(uint8_t cmd)
{
	uint8_t tx_temp[]={0xA5,0xEA,0,0};
	tx_temp[2]=cmd;
	tx_temp[3]=get_nsum(tx_temp,3);
	HAL_UART_Transmit(&huart1,tx_temp,4,1000);
}

void hlw8112_reset(void)
{
	uint8_t tx_temp[]={0xA5,0xEA,0x96,0xDA};
	HAL_UART_Transmit(&huart1,tx_temp,4,1000);
	
}
void hlw8112_enwrite(void)
{
	uint8_t tx_temp[]={0xA5,0xEA,0xE5,0xDA};
	tx_temp[3]=get_nsum(tx_temp,3);
	HAL_UART_Transmit(&huart1,tx_temp,4,1000);
	
}

void hlw8112_diswrite(void)
{
	uint8_t tx_temp[]={0xA5,0xEA,0xDC,0xDA};
	tx_temp[3]=get_nsum(tx_temp,3);
	HAL_UART_Transmit(&huart1,tx_temp,4,1000);
	
}

uint8_t get_nsum(uint8_t *ptr,uint8_t len)
{
	uint8_t sum=ptr[0];
	for(uint8_t i=1;i<len;i++)
	{
		sum+=ptr[i];
	}
	return ~sum;
}


uint16_t hlw8112_get_para(uint8_t reg)
{
	uint8_t tx_buf[10]={0xa5};
	uint8_t rx_buf[10]={0};
	tx_buf[1]=reg;
	HAL_UART_Transmit(&huart1,tx_buf,2,1000);
	HAL_UART_Receive(&huart1,rx_buf,3,1000);
	return (rx_buf[0]<<8)|(rx_buf[1]);
}

void hlw8112_check_para(void)
{
	uint32_t checksum=0;
	
	hlw8112_para.RmsIAC=hlw8112_R_reg(REG_RMS_IAC_ADDR,2);
	hlw8112_para.RmsIBC=hlw8112_R_reg(REG_RMS_IBC_ADDR,2);
	hlw8112_para.RmsUC=hlw8112_R_reg(REG_RMS_UC_ADDR,2);
	hlw8112_para.PowerPAC=hlw8112_R_reg(REG_POWER_PAC_ADDR,2);
	hlw8112_para.PowerPBC=hlw8112_R_reg(REG_POWER_PBC_ADDR,2);
	hlw8112_para.PowerSC=hlw8112_R_reg(REG_POWER_SC_ADDR,2);
	hlw8112_para.EnergyAC=hlw8112_R_reg(REG_ENERGY_AC_ADDR,2);
	hlw8112_para.EnergyBC=hlw8112_R_reg(REG_ENERGY_BC_ADDR,2);
	hlw8112_para.checksum=hlw8112_R_reg(REG_CHECKSUM_ADDR,2);

	#if 1
	checksum=~(0xffff+hlw8112_para.EnergyAC+hlw8112_para.EnergyBC+hlw8112_para.PowerPAC+hlw8112_para.PowerPBC+hlw8112_para.PowerSC+hlw8112_para.RmsIAC+hlw8112_para.RmsIBC+hlw8112_para.RmsUC);
	checksum&=0xffff;
	if(hlw8112_para.checksum==checksum)
	{
		if(1)
		{
		}//校验和正确
	}else{
		if(0)
		{
			
		}
		//校验和错误
	}
	#endif
}


