#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "oled.h"
 
/************************************************
 ALIENTEK ս��STM32F103������ʵ��12
 OLED��ʾʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
	
 int main(void)
 {	
	 u8 t;
//	char ch[5]="asdfg";
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 	LED_Init();			     //LED�˿ڳ�ʼ��
    
	 OLED_Init();
    

	
	t=100;  
	while(1) 
	{		
		
//	    OLED_Show16x16(24,0,10,0);
//        Oled_Printf_U16(0,0,t,0);
		OLED_P8x16Str(0,0,"FM:",0);
		delay_ms(500);

	}	  
	
}

