#ifndef __ADS1247_h
#define __ADS1247_h
/*****************************头文件包含*****************************************/
#include "stm32f10x.h"
#include "sys.h"
#include <stdio.h>
/********************************************************************************/

/******************ads1248寄存器地址*******************/
// define commands 
#define ADS1256_CMD_WAKEUP   0x00     //完成SYNC和退出待机模式
#define ADS1256_CMD_RDATA    0x01     //读数据
#define ADS1256_CMD_RDATAC   0x03     //连续读数据
#define ADS1256_CMD_SDATAC   0x0f     //停止连续读数据 
#define ADS1256_CMD_RREG     0x10     //从寄存器度数据 
#define ADS1256_CMD_WREG     0x50     //向寄存器写数据 
#define ADS1256_CMD_SELFCAL  0xf0     //偏移和增益自动校准
#define ADS1256_CMD_SELFOCAL 0xf1     //偏移自动校准 
#define ADS1256_CMD_SELFGCAL 0xf2     //增益自动校准 
#define ADS1256_CMD_SYSOCAL  0xf3     //系统失调校准 
#define ADS1256_CMD_SYSGCAL  0xf4     //系统增益校准 
#define ADS1256_CMD_SYNC     0xfc     //同步AD转换 
#define ADS1256_CMD_STANDBY  0xfd     //待机模式开始 
#define ADS1256_CMD_REST    0xfe      //复位
 
// define the ADS1256 register values 
#define ADS1256_STATUS       0x00   
#define ADS1256_MUX          0x01   
#define ADS1256_ADCON        0x02   
#define ADS1256_DRATE        0x03   
#define ADS1256_IO           0x04   
#define ADS1256_OFC0         0x05   
#define ADS1256_OFC1         0x06   
#define ADS1256_OFC2         0x07   
#define ADS1256_FSC0         0x08   
#define ADS1256_FSC1         0x09   
#define ADS1256_FSC2         0x0A 
 
 
// define multiplexer codes 
#define ADS1256_MUXP_AIN0   0x00 
#define ADS1256_MUXP_AIN1   0x10 
#define ADS1256_MUXP_AIN2   0x20 
#define ADS1256_MUXP_AIN3   0x30 
#define ADS1256_MUXP_AIN4   0x40 
#define ADS1256_MUXP_AIN5   0x50 
#define ADS1256_MUXP_AIN6   0x60 
#define ADS1256_MUXP_AIN7   0x70 
#define ADS1256_MUXP_AINCOM 0x80 
 
#define ADS1256_MUXN_AIN0   0x00 
#define ADS1256_MUXN_AIN1   0x01 
#define ADS1256_MUXN_AIN2   0x02 
#define ADS1256_MUXN_AIN3   0x03 
#define ADS1256_MUXN_AIN4   0x04 
#define ADS1256_MUXN_AIN5   0x05 
#define ADS1256_MUXN_AIN6   0x06 
#define ADS1256_MUXN_AIN7   0x07 
#define ADS1256_MUXN_AINCOM 0x08   
 
 
// define gain codes 
#define ADS1256_GAIN_1      0x00 
#define ADS1256_GAIN_2      0x01 
#define ADS1256_GAIN_4      0x02 
#define ADS1256_GAIN_8      0x03 
#define ADS1256_GAIN_16     0x04 
#define ADS1256_GAIN_32     0x05 
#define ADS1256_GAIN_64     0x06 
//#define ADS1256_GAIN_64     0x07 
 
//define drate codes 
#define ADS1256_DRATE_30000SPS   0xF0 
#define ADS1256_DRATE_15000SPS   0xE0 
#define ADS1256_DRATE_7500SPS   0xD0 
#define ADS1256_DRATE_3750SPS   0xC0 
#define ADS1256_DRATE_2000SPS   0xB0 
#define ADS1256_DRATE_1000SPS   0xA1 
#define ADS1256_DRATE_500SPS    0x92 
#define ADS1256_DRATE_100SPS    0x82 
#define ADS1256_DRATE_60SPS     0x72 
#define ADS1256_DRATE_50SPS     0x63 
#define ADS1256_DRATE_30SPS     0x53 
#define ADS1256_DRATE_25SPS     0x43 
#define ADS1256_DRATE_15SPS     0x33 
#define ADS1256_DRATE_10SPS     0x23 
#define ADS1256_DRATE_5SPS      0x13 
#define ADS1256_DRATE_2_5SPS    0x03

/******************ads1256引脚设置*******************/

#define ADS_RESET			GPIO_Pin_4
#define ADS_DRDY			((GPIOA->IDR)&(1<<2))//GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)

#define	RESET_HIGH()	    GPIO_SetBits(GPIOA, ADS_RESET)	    
#define	RESET_LOW()	        GPIO_ResetBits(GPIOA, ADS_RESET)


#define ADS_CS              GPIO_Pin_3                  /* PC.05 */

#define ADS_CS_LOW()        GPIO_ResetBits(GPIOA, ADS_CS)
#define ADS_CS_HIGH()       GPIO_SetBits(GPIOA, ADS_CS)



extern u8  results1;
extern u8  results2;
extern u8  results3;
void SPI_ADS1256_Init(void);
void ADS1256_GPIO_init(void);
void ADS1256_Init(void);
//void ADS_sum(unsigned char road);
unsigned long ADS_sum(unsigned char road);
u8 SPI_SendByte(u8 byte);
unsigned long ADS1256ReadData(void);
void ADS1256WREG(unsigned char regaddr,unsigned char databyte);
void sendChar(unsigned char ch);


#endif


