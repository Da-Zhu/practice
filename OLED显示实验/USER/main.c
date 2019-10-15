#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "oled.h"
 
/************************************************
 ALIENTEK 战舰STM32F103开发板实验12
 OLED显示实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
	
 int main(void)
 {	
	 u8 t;
//	char ch[5]="asdfg";
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 	LED_Init();			     //LED端口初始化
    
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

