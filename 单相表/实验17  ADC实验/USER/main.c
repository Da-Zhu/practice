#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"
#include "ads1256.h"
#include "oled.h"


/************************************************
 ALIENTEK ս��STM32F103������ʵ��17
 ADC ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
#define MEDIAN_LEN  5                    //��ֱ�˲����ܳ��ȣ�һ��ѡȡ����   
#define MEDIAN      2                     //��ֵ���˲������е�λ��

#define MEDIAN_LEARN  49                    //��ֱ�˲����ܳ��ȣ�һ��ѡȡ����   
#define MEDIAN_L      24                     //��ֵ���˲������е�λ��

u8 electric_num;

long double electric_app0[50];
long double electric_app1[50];
long double electric_app2[50];
long double electric_app3[50];
long double electric_app4[50];
long double electric_app5[50];
long double electric_app6[50];
long double electric_app7[50];

long double electric_buf[8];

long double electric_middle_num;

long double electric_app_store0;
long double electric_app_store1;
long double electric_app_store2;
long double electric_app_store3;
long double electric_app_store4;
long double electric_app_store5;
long double electric_app_store6;
long double electric_app_store7;

long double judge_mid_buf[8];



unsigned long   AD_buffer[MEDIAN_LEN];    //ad�ɼ����黺��
//unsigned long   MED_buffer[MEDIAN_LEN];   //��ֵ�˲����黺��
unsigned char   medleng = 0;          //���뻺������ݸ���

long double medina_filter(long double *MED_buffer);
long double medina_filter1(long double *MED_buffer1);

u8 system_mode;
u8 learn_electric;
u8 learn_flag; 
u8 learn_finish;
u8 judge_result;
 
long double ADS_receive[5];
long double ADS_OUTPUT;
	
 int main(void)
 {	 
	u8 key,key_mode=0,i,j;
	 u32 count;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();
	OLED_Init();
	 
	SPI_ADS1256_Init();								  //SPI��ʼ��
	ADS1256_GPIO_init();							  //�˿ڳ�ʼ��
	ADS1256_Init();

	OLED_P6x8Str(0,0,"MODE:",0);
//	 OLED_P6x8Str(100,10,"ok",0);
	while(1)
	{
		key=KEY_Scan(key_mode);
		if(key)
		{
			switch(key)
			{				 
				case KEY3_PRES:	//����LED0��ת
					learn_finish=1;
				
					break;
				case KEY2_PRES:	//����LED0��ת
					if(system_mode==1)
					{
						
						learn_electric++;
						learn_flag=1;
						OLED_P6x8Str(0,10,"Learning:             ",0);
						Oled_Printf_U16(60,10,learn_electric,0);
						OLED_P6x8Str(0,13,"data:             ",0);
					}
					break;
				case KEY1_PRES:	//����LED1��ת
					if(system_mode==1)
					{
						
						learn_electric--;
						learn_flag=1;
						OLED_P6x8Str(0,10,"Learning:             ",0);
						Oled_Printf_U16(60,10,learn_electric,0);
						OLED_P6x8Str(0,13,"data:             ",0);
					}
//					system_mode=2;
//					key_mode=1;
//					OLED_P6x8Str(60,0,"Test ",0);
//					OLED_P6x8Str(0,10,"Result:             ",0);
					break;
				case KEY0_PRES:	//ͬʱ����LED0,LED1��ת 
					system_mode++;
					if(system_mode==1)
					{
						key_mode=0;
						OLED_P6x8Str(60,0,"Learn",0);
					}
					if(system_mode==2)
					{
						OLED_P6x8Str(60,0,"DirectTest ",0);
						OLED_P6x8Str(0,10,"Result:             ",0);
					}
					if(system_mode==3)
					{
						key_mode=1;
						OLED_P6x8Str(80,0,"DirectTest ",0);
						OLED_P6x8Str(0,10,"Result:             ",0);
					}
					
					break;
			}
		}
					
		/*	ADS_sum(a,b)���������ܣ�������ѹa-b
			��˵��ADS1256_MUXP_AIN��0~7������ͨ��0~7������� P��positive
				  ADS1256_MUXN_AIN��0~7������ͨ��0~7����ָ� N��negative
			
			������ֲ���ͨ��0��1��ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AIN1);    ����ͨ��0-ͨ��1�ĵ�ѹֵ
				���˲���ͨ��0��   ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);  ����ͨ��0�Եصĵ�ѹֵ
		*/
		if(system_mode==2)
		{
			for(i=0;i<5;i++)
			{
				ADS_receive[i]=ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);	//ͨ��0�Եصĵ�ѹֵ
				ADS_OUTPUT=medina_filter(ADS_receive);
				printf("ADS_OUTPUT:%lf\r\n",ADS_OUTPUT);
			}
			
//			ADS_OUTPUT=ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);
			
//			if(electric_num>1)
//			{
				if(ADS_OUTPUT<2650) 
				{
					judge_result=0;
					Oled_Printf_U16(60,10,judge_result,0);
				}
				if(ADS_OUTPUT>2650&&ADS_OUTPUT<3000) 
				{
					judge_result=1;
					Oled_Printf_U16(60,10,judge_result,0);
				}
				if(ADS_OUTPUT>3000&&ADS_OUTPUT<3600) 
				{
					judge_result=2;
					Oled_Printf_U16(60,10,judge_result,0);
				}
				if(ADS_OUTPUT>3600&&ADS_OUTPUT<5000) 
				{
					judge_result=3;
					Oled_Printf_U16(60,10,judge_result,0);
				}
				if(ADS_OUTPUT>5000&&ADS_OUTPUT<542800) 
				{
					judge_result=4;
					Oled_Printf_U16(60,10,judge_result,0);
				}
				if(ADS_OUTPUT>542800&&ADS_OUTPUT<judge_mid_buf[5]) 
				{
					judge_result=5;
					Oled_Printf_U16(60,10,judge_result,0);
				}
//				if(ADS_OUTPUT>judge_mid_buf[5]&&ADS_OUTPUT<judge_mid_buf[6]) 
//				{
//					judge_result=6;
//					Oled_Printf_U16(60,10,judge_result,0);
//				}
//				if(ADS_OUTPUT>judge_mid_buf[6]&&ADS_OUTPUT<judge_mid_buf[7]) 
//				{
//					judge_result=7;
//					Oled_Printf_U16(60,10,judge_result,0);
//				}
//			}
		}
		
		if(system_mode==3)
		{
			for(i=0;i<5;i++)
			{
				ADS_receive[i]=ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);	//ͨ��0�Եصĵ�ѹֵ
				ADS_OUTPUT=medina_filter(ADS_receive);
				printf("ADS_OUTPUT:%lf\r\n",ADS_OUTPUT);
			}
			
//			ADS_OUTPUT=ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);
			
			if(electric_num>1)
			{
				if(ADS_OUTPUT<judge_mid_buf[0]) 
				{
					judge_result=0;
					Oled_Printf_U16(60,10,judge_result,0);
				}
				if(ADS_OUTPUT>judge_mid_buf[0]&&ADS_OUTPUT<judge_mid_buf[1]) 
				{
					judge_result=1;
					Oled_Printf_U16(60,10,judge_result,0);
				}
				if(ADS_OUTPUT>judge_mid_buf[1]&&ADS_OUTPUT<judge_mid_buf[2]) 
				{
					judge_result=2;
					Oled_Printf_U16(60,10,judge_result,0);
				}
				if(ADS_OUTPUT>judge_mid_buf[2]&&ADS_OUTPUT<judge_mid_buf[3]) 
				{
					judge_result=3;
					Oled_Printf_U16(60,10,judge_result,0);
				}
				if(ADS_OUTPUT>judge_mid_buf[3]&&ADS_OUTPUT<judge_mid_buf[4]) 
				{
					judge_result=4;
					Oled_Printf_U16(60,10,judge_result,0);
				}
				if(ADS_OUTPUT>judge_mid_buf[4]&&ADS_OUTPUT<judge_mid_buf[5]) 
				{
					judge_result=5;
					Oled_Printf_U16(60,10,judge_result,0);
				}
				if(ADS_OUTPUT>judge_mid_buf[5]&&ADS_OUTPUT<judge_mid_buf[6]) 
				{
					judge_result=6;
					Oled_Printf_U16(60,10,judge_result,0);
				}
				if(ADS_OUTPUT>judge_mid_buf[6]&&ADS_OUTPUT<judge_mid_buf[7]) 
				{
					judge_result=7;
					Oled_Printf_U16(60,10,judge_result,0);
				}
			}
		}
		if(system_mode==1)
		{
			count++;
			if(count>200000)
			{
				ADS_OUTPUT=ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);
				printf("ADS_OUTPUT:%lf\r\n",ADS_OUTPUT);
				count=0;
			}
			if(learn_electric&&(learn_flag==1)) 
			{
				switch(learn_electric)
				{				 
					case 1:	//���Ʒ�����
						for(i=0;i<50;i++)
						{
							electric_app0[i]=ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);	//ͨ��0�Եصĵ�ѹֵ
							printf("electric_app0:%lf\r\n",electric_app0[i]);
						}
						electric_app_store0=medina_filter1(electric_app0);
						learn_flag=0;
						OLED_P6x8Str(100,10,"ok",0);
						printf("electric_app_store0:%lf\r\n",electric_app_store0);
						Oled_Printf_U16(60,13,(u16)electric_app_store0/100,0);
						
//						delay_ms(500);
//						OLED_P6x8Str(100,10,"  ",0);
						break;
					case 2:	//����LED0��ת
						for(i=0;i<50;i++)
						{
							electric_app1[i]=ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);	//ͨ��0�Եصĵ�ѹֵ
							printf("electric_app1:%lf\r\n",electric_app1[i]);
						}
						electric_app_store1=medina_filter1(electric_app1);
						learn_flag=0;
						OLED_P6x8Str(100,10,"ok",0);
						printf("electric_app_store1:%lf\r\n",electric_app_store1);
						Oled_Printf_U16(60,13,(u16)electric_app_store1/100,0);
						break;
					case 3:	//����LED1��ת	 
						for(i=0;i<50;i++)
						{
							electric_app2[i]=ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);	//ͨ��0�Եصĵ�ѹֵ
							printf("electric_app2:%lf\r\n",electric_app2[i]);
						}
						electric_app_store2=medina_filter1(electric_app2);
						learn_flag=0;
						OLED_P6x8Str(100,10,"ok",0);
						printf("electric_app_store2:%lf\r\n",electric_app_store2);
						Oled_Printf_U16(60,13,(u16)electric_app_store2/100,0);
						break;
					case 4:	//ͬʱ����LED0,LED1��ת 
						for(i=0;i<50;i++)
						{
							electric_app3[i]=ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);	//ͨ��0�Եصĵ�ѹֵ
							printf("electric_app3:%lf\r\n",electric_app3[i]);
						}
						electric_app_store3=medina_filter1(electric_app3);
						learn_flag=0;
						OLED_P6x8Str(100,10,"ok",0);
						printf("electric_app_store3:%lf\r\n",electric_app_store3);
						Oled_Printf_U16(60,13,(u16)electric_app_store3/100,0);
						break;
					case 5:	//���Ʒ�����
						for(i=0;i<50;i++)
						{
							electric_app4[i]=ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);	//ͨ��0�Եصĵ�ѹֵ
							printf("electric_app4:%lf\r\n",electric_app4[i]);
						}
						electric_app_store4=medina_filter1(electric_app4);
						learn_flag=0;
						OLED_P6x8Str(100,10,"ok",0);
						printf("electric_app_store4:%lf\r\n",electric_app_store4);
						Oled_Printf_U16(60,13,(u16)electric_app_store4/100,0);
						break;
					case 6:	//���Ʒ�����
						for(i=0;i<50;i++)
						{
							electric_app5[i]=ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);	//ͨ��0�Եصĵ�ѹֵ
							printf("electric_app5:%lf\r\n",electric_app5[i]);
						}
						electric_app_store5=medina_filter1(electric_app5);
						learn_flag=0;
						OLED_P6x8Str(100,10,"ok",0);
						printf("electric_app_store5:%lf\r\n",electric_app_store5);
						Oled_Printf_U16(60,13,(u16)electric_app_store5/100,0);
						break;
					case 7:	//���Ʒ�����
						for(i=0;i<50;i++)
						{
							electric_app6[i]=ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);	//ͨ��0�Եصĵ�ѹֵ
							printf("electric_app6:%lf\r\n",electric_app6[i]);
						}
						electric_app_store6=medina_filter1(electric_app6);
						learn_flag=0;
						OLED_P6x8Str(100,10,"ok",0);
						printf("electric_app_store6:%lf\r\n",electric_app_store6);
						Oled_Printf_U16(60,13,(u16)electric_app_store6/100,0);
						break;
					case 8:	//���Ʒ�����
						for(i=0;i<50;i++)
						{
							electric_app7[i]=ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);	//ͨ��0�Եصĵ�ѹֵ
							printf("electric_app7:%lf\r\n",electric_app7[i]);
						}
						electric_app_store7=medina_filter1(electric_app7);
						learn_flag=0;
						OLED_P6x8Str(100,10,"ok",0);
						printf("electric_app_store7:%lf\r\n",electric_app_store7);
						Oled_Printf_U16(60,13,(u16)electric_app_store7/100,0);
						break;
				}
				
			}
		}
		if(learn_finish==1)
		{
			electric_buf[0]=electric_app_store0;
			electric_buf[1]=electric_app_store1;
			electric_buf[2]=electric_app_store2;
			electric_buf[3]=electric_app_store3;
			electric_buf[4]=electric_app_store4;
			electric_buf[5]=electric_app_store5;
			electric_buf[6]=electric_app_store6;
			electric_buf[7]=electric_app_store7;
			
			for(i=0;i<8;i++)     //��С��������
			{   
				for(j=0;j<8;j++)
				{
					if(electric_buf[j-1] >= electric_buf[j]) // ��ѯ���ĵ�ǰԪ��>ADֵ,�򽻻����ǵ�ֵ   
					{ electric_middle_num = electric_buf[j-1]; electric_buf[j-1] = electric_buf[j]; electric_buf[j] = electric_middle_num;} 
				}			
			}
			
			if(electric_buf[0]==0)
			{
				for(i=0;i<8;i++)   //ȥ����ֵ��0��
				{
					if(electric_buf[i]!=0)
					{
						for(j=0;j<8;j++)
						{
							electric_buf[j]=electric_buf[i+j];
						}
						electric_num=8-i;
						break;
					}
				}
			}
			for(i=0;i<8;i++)
			{
				printf("electric_buf:%lf\r\n",electric_buf[i]);
			}
			if(electric_num>1)
			{
				for(i=0;i<(electric_num-1);i++)
				{
					judge_mid_buf[i]=(electric_buf[i]+electric_buf[i+1])/2;
				}
				judge_mid_buf[electric_num-1]=electric_buf[electric_num-1]+5000;
			}
//			judge_mid_buf[0]=(electric_buf[0]+electric_buf[1])/2;
//			judge_mid_buf[1]=(electric_buf[1]+electric_buf[2])/2;
//			judge_mid_buf[2]=(electric_buf[2]+electric_buf[3])/2;
//			judge_mid_buf[3]=(electric_buf[3]+electric_buf[4])/2;
//			judge_mid_buf[4]=(electric_buf[4]+electric_buf[5])/2;
//			judge_mid_buf[5]=(electric_buf[5]+electric_buf[6])/2;
//			judge_mid_buf[6]=(electric_buf[6]+electric_buf[7])/2;
			
			
			
			
//			printf("electric_num:%d\r\n",electric_num);
//			
//			printf("range:%lf\r\n",judge_mid_buf[0]);
//			printf("range:%lf\r\n",judge_mid_buf[1]);
//			delay_ms(50000);
//			for(i=0;i<8;i++)
//			{
//				printf("range:%lf\r\n",electric_buf[i]);
//			}
			learn_finish=0;
		}
		
		
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
//		delay_ms(10);
		
		
		
		
	}
}

/**********************************************************/
// ����������ֵ�˲�����
// ����  ����ȡǰ9�βɼ������ݣ�ȥ����3����ȥ����3����Ȼ���м��
// ����  ��3��������ƽ��ֵ�����㷨�ɾ����ܵ��˵��������ݣ�����Ӱ��ɼ��ٶȡ�
// ����  ��9�����ݵ�����
// ���  ���м�3�����ݵ�ƽ��ֵ
/*********************************************************/
long double medina_filter(long double *MED_buffer)  //xad - ADCת��ֵ   
{   
	
	unsigned char i,j;
	long double xd;
	long double xxd;
	
	for(i = 0; i < MEDIAN_LEN; i ++)     
	{   
		for(j = 0; j < MEDIAN_LEN; j ++)
		{
			if( MED_buffer[j] > MED_buffer[j + 1]) // ��ѯ���ĵ�ǰԪ��>ADֵ,�򽻻����ǵ�ֵ   
			{ xd = MED_buffer[j]; MED_buffer[j] = MED_buffer[j + 1]; MED_buffer[j + 1] = xd;} 
		}			
	} 
	
	xxd = MED_buffer[MEDIAN-1]+MED_buffer[MEDIAN]+MED_buffer[MEDIAN+1];
	xd = xxd/3;
	xxd=0;
	return xd; //��ֵ     
}

/******************* (C) COPYRIGHT 2014 ������ӿ��������� *****END OF FILE****/

/**********************************************************/
// ����������ֵ�˲�����
// ����  ����ȡǰ9�βɼ������ݣ�ȥ����3����ȥ����3����Ȼ���м��
// ����  ��3��������ƽ��ֵ�����㷨�ɾ����ܵ��˵��������ݣ�����Ӱ��ɼ��ٶȡ�
// ����  ��9�����ݵ�����
// ���  ���м�3�����ݵ�ƽ��ֵ
/*********************************************************/
long double medina_filter1(long double *MED_buffer1)  //xad - ADCת��ֵ   
{   
	
	unsigned char i,j;
	long double xd;
	long double xxd;
	
	for(i = 0; i < MEDIAN_LEARN; i ++)     
	{   
		for(j = 0; j < MEDIAN_LEARN; j ++)
		{
			if( MED_buffer1[j] > MED_buffer1[j + 1]) // ��ѯ���ĵ�ǰԪ��>ADֵ,�򽻻����ǵ�ֵ   
			{ xd = MED_buffer1[j]; MED_buffer1[j] = MED_buffer1[j + 1]; MED_buffer1[j + 1] = xd;} 
		}			
	} 
	for(i=MEDIAN_L-15;i<MEDIAN_L+15;i++)
	{
		xxd+=MED_buffer1[i];
	}
	xd = xxd/31;
	xxd=0;
	return xd; //��ֵ     
}

/******************* (C) COPYRIGHT 2014 ������ӿ��������� *****END OF FILE****/
