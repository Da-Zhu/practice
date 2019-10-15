#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "24l01.h" 	 
#include "my_function.h"
 
 
/************************************************
 ALIENTEK战舰STM32开发板实验33
 无线通信 实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
u8 tmp_buf[33];	

 int main(void)
 {	 
	u8 key,mode=1;
	u16 t=0;			 
		    
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键

 	NRF24L01_Init();    		//初始化NRF24L01 

//	while(NRF24L01_Check())
 
 	while(1)
	{			    

		NRF24L01_TX_Mode();  		   				 
        NRF24L01_TxPacket(tmp_buf);
		KEY_Run();		   
	}


}


