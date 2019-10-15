#include "pwm.h"
#include "led.h"
#include "usart.h"
 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器PWM 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//TIM1 PWM部分初始化 
//arr：自动重装值
//psc：时钟预分频数
void TIM1_PWM_Init(u16 arr,u16 psc)
{     
    GPIO_InitTypeDef  GPIO_InitStructure;        
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);    	//TIM1时钟使能  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);   	//使能PORTE时钟
        
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1);  	//GPIOE9复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);	//GPIOE11复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);	//GPIOE13复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);	//GPIOE14复用为定时器1	
     
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;          //GPIOE9，GPIOE11，GPIOE13，GPIOE14  |GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       //复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //不上下拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度100MHz
    GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化PE9,PE11,PE13,PE14
     
	TIM_TimeBaseStructure.TIM_Prescaler = psc;            			 //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   	 //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = arr;                       	 //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;        
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);          		 //初始化定时器1
                
    //初始化TIM1 Channel1,2,3,4 PWM模式        
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;     			//选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  	//比较输出使能
    TIM_OCInitStructure.TIM_Pulse = arr;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  		//输出极性高

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);    //根据T指定的参数初始化外设OC1
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);	//根据T指定的参数初始化外设OC2
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);    //根据T指定的参数初始化外设OC3
	TIM_OC4Init(TIM1, &TIM_OCInitStructure); 	//根据T指定的参数初始化外设OC4
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);   //使能TIM1在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);	//使能TIM1在CCR2上的预装载寄存器
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);	//使能TIM1在CCR3上的预装载寄存器
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);	//使能TIM1在CCR4上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);			//ARPE使能 
	TIM_CtrlPWMOutputs(TIM1,ENABLE); 			//使能TIM1的PWM输出
	TIM_Cmd(TIM1,ENABLE);						//使能TIM1                         
}

//TIM1 PWM部分初始化 
//arr：自动重装值
//psc：时钟预分频数
void TIM8_PWM_Init(u32 arr,u32 psc)
{     
    GPIO_InitTypeDef  GPIO_InitStructure;        
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);    	//TIM1时钟使能  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);   	//使能PORTE时钟
        
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);  	//GPIOE9复用为定时器1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);	//GPIOE11复用为定时器1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);	//GPIOE13复用为定时器1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);	//GPIOE14复用为定时器1	
     
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;          //GPIOE9，GPIOE11，GPIOE13，GPIOE14
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       //复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //不上下拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度100MHz
    GPIO_Init(GPIOC,&GPIO_InitStructure);              //初始化PE9,PE11,PE13,PE14
     
	TIM_TimeBaseStructure.TIM_Prescaler = psc;            			 //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   	 //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = arr;                       	 //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;        
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);          		 //初始化定时器1
                
    //初始化TIM1 Channel1,2,3,4 PWM模式        
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;     			//选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  	//比较输出使能
    TIM_OCInitStructure.TIM_Pulse = arr;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  		//输出极性高

	TIM_OC1Init(TIM8, &TIM_OCInitStructure);    //根据T指定的参数初始化外设OC1
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);	//根据T指定的参数初始化外设OC2
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);    //根据T指定的参数初始化外设OC3
	TIM_OC4Init(TIM8, &TIM_OCInitStructure); 	//根据T指定的参数初始化外设OC4
	
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);   //使能TIM1在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);	//使能TIM1在CCR2上的预装载寄存器
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);	//使能TIM1在CCR3上的预装载寄存器
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);	//使能TIM1在CCR4上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM8,ENABLE);			//ARPE使能 
	TIM_CtrlPWMOutputs(TIM8,ENABLE); 			//使能TIM1的PWM输出
	TIM_Cmd(TIM8,ENABLE);						//使能TIM1                         
}
