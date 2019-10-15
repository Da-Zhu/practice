#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"
#include "ads1256.h"


/************************************************
 ALIENTEK ս��STM32F103������ʵ��17
 ADC ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
#define MEDIAN_LEN  5                     //��ֱ�˲����ܳ��ȣ�һ��ѡȡ����   
#define MEDIAN      2                     //��ֵ���˲������е�λ��
unsigned long   AD_buffer[MEDIAN_LEN];    //ad�ɼ����黺��
//unsigned long   MED_buffer[MEDIAN_LEN];   //��ֵ�˲����黺��
unsigned char   medleng = 0;          //���뻺������ݸ���

unsigned long medina_filter(unsigned long *MED_buffer);

  
 
 int main(void)
 {	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	 	
	 
	SPI_ADS1256_Init();								  //SPI��ʼ��
	ADS1256_GPIO_init();							  //�˿ڳ�ʼ��
	ADS1256_Init();


	while(1)
	{
				/*
			ADS_sum(a,b)���������ܣ�������ѹa-b
			��˵��ADS1256_MUXP_AIN��0~7������ͨ��0~7������� P��positive
				  ADS1256_MUXN_AIN��0~7������ͨ��0~7����ָ� N��negative
			
			������ֲ���ͨ��0��1��ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AIN1);    ����ͨ��0-ͨ��1�ĵ�ѹֵ
				���˲���ͨ��0��   ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);  ����ͨ��0�Եصĵ�ѹֵ
		*/
	  ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);	//ͨ��0�Եصĵ�ѹֵ

// 		ADS_sum(ADS1256_MUXP_AIN1 | ADS1256_MUXN_AINCOM);	//ͨ��1�Եصĵ�ѹֵ
// 		sendChar(0x0d);	sendChar(0x0a);	
// 		ADS_sum(ADS1256_MUXP_AIN2 | ADS1256_MUXN_AINCOM);	//ͨ��2�Եصĵ�ѹֵ
// 		sendChar(0x0d);	sendChar(0x0a);	
// 		ADS_sum(ADS1256_MUXP_AIN3 | ADS1256_MUXN_AINCOM);	//ͨ��3�Եصĵ�ѹֵ
// 		sendChar(0x0d);	sendChar(0x0a);	
// 		ADS_sum(ADS1256_MUXP_AIN4 | ADS1256_MUXN_AINCOM);	//ͨ��4�Եصĵ�ѹֵ
// 		sendChar(0x0d);	sendChar(0x0a);	
// 		ADS_sum(ADS1256_MUXP_AIN5 | ADS1256_MUXN_AINCOM);	//ͨ��5�Եصĵ�ѹֵ
// 		sendChar(0x0d);	sendChar(0x0a);	
//   	ADS_sum(ADS1256_MUXP_AIN6 | ADS1256_MUXN_AINCOM);	//ͨ��6�Եصĵ�ѹֵ
// 		sendChar(0x0d);	sendChar(0x0a);	
// 		ADS_sum(ADS1256_MUXP_AIN7 | ADS1256_MUXN_AINCOM);	//ͨ��7�Եصĵ�ѹֵ	
// 		


		delay_ms(500);
	}
}

/**********************************************************/
// ����������ֵ�˲�����
// ����  ����ȡǰ9�βɼ������ݣ�ȥ����3����ȥ����3����Ȼ���м��
// ����  ��3��������ƽ��ֵ�����㷨�ɾ����ܵ��˵��������ݣ�����Ӱ��ɼ��ٶȡ�
// ����  ��9�����ݵ�����
// ���  ���м�3�����ݵ�ƽ��ֵ
/*********************************************************/
unsigned long medina_filter(unsigned long *MED_buffer)  //xad - ADCת��ֵ   
{   
	
	unsigned char i,j;
	unsigned long xd;
	u32 xxd;
	
	for(i = 0; i < MEDIAN_LEN; i ++)     
	{   
		for(j = 0; j < MEDIAN_LEN - i; j ++)
		{
			
			if( MED_buffer[i] > MED_buffer[i + 1]) // ��ѯ���ĵ�ǰԪ��>ADֵ,�򽻻����ǵ�ֵ   
			{ xd = MED_buffer[i]; MED_buffer[i] = MED_buffer[i + 1]; MED_buffer[i + 1] = xd;} 
		}			
	} 
	xxd = MED_buffer[MEDIAN - 1] + MED_buffer[MEDIAN] + MED_buffer[MEDIAN + 1];
	xd = xxd/3;
	return xd; //��ֵ     
}

/******************* (C) COPYRIGHT 2014 ������ӿ��������� *****END OF FILE****/
