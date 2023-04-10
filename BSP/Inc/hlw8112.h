#ifndef _HLW8112_H
#define _HLW8112_H

#include "main.h"


#define  	RESET						0X96
#define		EN_WRITE					0xE5
#define		DIS_WRITE					0xDC
#define		CHANNEL_A					0x5A
#define		CHANNEL_B					0xA5

//8112/8110 reg define
#define 	REG_SYSCON_ADDR         	0x00
#define 	REG_EMUCON_ADDR         	0x01
#define 	REG_HFCONST_ADDR        	0x02			//脉冲频率寄存器
#define 	REG_EMUCON2_ADDR        	0x13
#define 	REG_ANGLE_ADDR        		0x22			//相角寄存器
#define 	REG_UFREQ_ADDR          	0x23     	//市电线性频率地址
#define 	REG_RMSIA_ADDR          	0x24
#define 	REG_RMSIB_ADDR          	0x25
#define 	REG_RMSU_ADDR           	0x26
#define 	REG_PF_ADDR             	0x27
#define 	REG_ENERGY_PA_ADDR			0x28
#define 	REG_ENERGY_PB_ADDR			0x29
#define 	REG_POWER_PA_ADDR       	0x2C
#define 	REG_POWER_PB_ADDR       	0x2D
                                                                                                                                                                                                                                 

#define 	REG_SAGCYC_ADDR         	0x17
#define 	REG_SAGLVL_ADDR         	0x18
#define 	REG_OVLVL_ADDR          	0x19
#define 	REG_OIALVL_ADDR          	0x1a

#define 	REG_INT_ADDR          		0x1D
#define 	REG_IE_ADDR          		0x40
#define 	REG_IF_ADDR          		0x41
#define 	REG_RIF_ADDR          		0x42

#define 	REG_RDATA_ADDR          	0x44


#define 	REG_CHECKSUM_ADDR			0x6f
#define 	REG_RMS_IAC_ADDR			0x70
#define 	REG_RMS_IBC_ADDR			0x71
#define 	REG_RMS_UC_ADDR				0x72
#define 	REG_POWER_PAC_ADDR			0x73
#define 	REG_POWER_PBC_ADDR			0x74
#define 	REG_POWER_SC_ADDR			0x75
#define 	REG_ENERGY_AC_ADDR			0x76
#define 	REG_ENERGY_BC_ADDR			0x77

#define 	D_CAL_U						1000/1000		//电压校正系数
#define 	D_CAL_A_I					1000/1000		//A通道电流校正系数
#define 	D_CAL_A_P					1000/1000		//A通道功率校正系数
#define 	D_CAL_A_E					1000/1000		//A通道电能校正系数


//8112 B通道校正系数
#define 	D_CAL_B_P					1000/1000		//B通道功率校正系数
#define 	D_CAL_B_I					1000/1000		//B通道电流校正系数
#define 	D_CAL_B_E					1000/1000		//B通道电能校正系数


//宏定义
#define 	FALSE						0

typedef struct {
	
	uint16_t checksum;
	uint16_t RmsIAC;
	uint16_t RmsIBC;
	uint16_t RmsUC;
	uint16_t PowerPAC;
	uint16_t PowerPBC;
	uint16_t PowerSC;
	uint16_t EnergyAC;
	uint16_t EnergyBC;
	
}hlw8112_para_typedef;



void hlw8112_init(void);
float  hlw8112_get_RmsIA(void);
float  hlw8112_get_RmsIB(void);
float  hlw8112_get_RmsU(void);
float hlw8112_get_ufre(void);
float hlw8112_get_RmsP(void);
float hlw8112_get_RmsE(void);


void hlw8112_W_reg(uint8_t reg,uint32_t para,uint8_t len);
uint32_t  hlw8112_R_reg(uint8_t reg,uint8_t len);


uint8_t get_nsum(uint8_t *ptr,uint8_t len);
void hlw8112_special_cmd(uint8_t cmd);
void hlw8112_reset(void);
void hlw8112_enwrite(void);
void hlw8112_diswrite(void);
uint16_t hlw8112_get_para(uint8_t reg);
void hlw8112_check_para(void);


#endif
