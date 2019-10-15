#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"
#include "ads1256.h"


/************************************************
 ALIENTEK 战舰STM32F103开发板实验17
 ADC 实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
#define MEDIAN_LEN  5                     //中直滤波的总长度，一般选取奇数   
#define MEDIAN      2                     //中值在滤波数组中的位置
unsigned long   AD_buffer[MEDIAN_LEN];    //ad采集数组缓存
//unsigned long   MED_buffer[MEDIAN_LEN];   //中值滤波数组缓存
unsigned char   medleng = 0;          //存入缓存的数据个数

unsigned long medina_filter(unsigned long *MED_buffer);

  
 
 int main(void)
 {	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			     //LED端口初始化
	 	
	 
	SPI_ADS1256_Init();								  //SPI初始化
	ADS1256_GPIO_init();							  //端口初始化
	ADS1256_Init();


	while(1)
	{
				/*
			ADS_sum(a,b)：函数功能，测量电压a-b
			解说：ADS1256_MUXP_AIN（0~7）代表通道0~7，差分正 P：positive
				  ADS1256_MUXN_AIN（0~7）代表通道0~7，差分负 N：negative
			
			例：差分测量通道0和1：ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AIN1);    返回通道0-通道1的电压值
				单端测量通道0：   ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);  返回通道0对地的电压值
		*/
	  ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);	//通道0对地的电压值

// 		ADS_sum(ADS1256_MUXP_AIN1 | ADS1256_MUXN_AINCOM);	//通道1对地的电压值
// 		sendChar(0x0d);	sendChar(0x0a);	
// 		ADS_sum(ADS1256_MUXP_AIN2 | ADS1256_MUXN_AINCOM);	//通道2对地的电压值
// 		sendChar(0x0d);	sendChar(0x0a);	
// 		ADS_sum(ADS1256_MUXP_AIN3 | ADS1256_MUXN_AINCOM);	//通道3对地的电压值
// 		sendChar(0x0d);	sendChar(0x0a);	
// 		ADS_sum(ADS1256_MUXP_AIN4 | ADS1256_MUXN_AINCOM);	//通道4对地的电压值
// 		sendChar(0x0d);	sendChar(0x0a);	
// 		ADS_sum(ADS1256_MUXP_AIN5 | ADS1256_MUXN_AINCOM);	//通道5对地的电压值
// 		sendChar(0x0d);	sendChar(0x0a);	
//   	ADS_sum(ADS1256_MUXP_AIN6 | ADS1256_MUXN_AINCOM);	//通道6对地的电压值
// 		sendChar(0x0d);	sendChar(0x0a);	
// 		ADS_sum(ADS1256_MUXP_AIN7 | ADS1256_MUXN_AINCOM);	//通道7对地的电压值	
// 		


		delay_ms(500);
	}
}

/**********************************************************/
// 函数名：中值滤波函数
// 描述  ：提取前9次采集的数据，去掉高3个，去掉低3个，然后中间的
// 描述  ：3个数据求平均值，该算法可尽可能的滤掉干扰数据，并不影响采集速度。
// 输入  ：9个数据的数组
// 输出  ：中间3个数据的平均值
/*********************************************************/
unsigned long medina_filter(unsigned long *MED_buffer)  //xad - ADC转换值   
{   
	
	unsigned char i,j;
	unsigned long xd;
	u32 xxd;
	
	for(i = 0; i < MEDIAN_LEN; i ++)     
	{   
		for(j = 0; j < MEDIAN_LEN - i; j ++)
		{
			
			if( MED_buffer[i] > MED_buffer[i + 1]) // 轮询到的当前元素>AD值,则交换它们的值   
			{ xd = MED_buffer[i]; MED_buffer[i] = MED_buffer[i + 1]; MED_buffer[i + 1] = xd;} 
		}			
	} 
	xxd = MED_buffer[MEDIAN - 1] + MED_buffer[MEDIAN] + MED_buffer[MEDIAN + 1];
	xd = xxd/3;
	return xd; //中值     
}

/******************* (C) COPYRIGHT 2014 三峰电子开发工作室 *****END OF FILE****/
