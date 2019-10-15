#include "pwm.h"
#include "led.h"
#include "usart.h"
 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ��PWM ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//TIM1 PWM���ֳ�ʼ�� 
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM1_PWM_Init(u16 arr,u16 psc)
{     
    GPIO_InitTypeDef  GPIO_InitStructure;        
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);    	//TIM1ʱ��ʹ��  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);   	//ʹ��PORTEʱ��
        
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1);  	//GPIOE9����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);	//GPIOE11����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);	//GPIOE13����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);	//GPIOE14����Ϊ��ʱ��1	
     
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;          //GPIOE9��GPIOE11��GPIOE13��GPIOE14  |GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       //���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //�ٶ�100MHz
    GPIO_Init(GPIOE,&GPIO_InitStructure);              //��ʼ��PE9,PE11,PE13,PE14
     
	TIM_TimeBaseStructure.TIM_Prescaler = psc;            			 //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   	 //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = arr;                       	 //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;        
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);          		 //��ʼ����ʱ��1
                
    //��ʼ��TIM1 Channel1,2,3,4 PWMģʽ        
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;     			//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  	//�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_Pulse = arr;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  		//������Ը�

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);    //����Tָ���Ĳ�����ʼ������OC1
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);	//����Tָ���Ĳ�����ʼ������OC2
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);    //����Tָ���Ĳ�����ʼ������OC3
	TIM_OC4Init(TIM1, &TIM_OCInitStructure); 	//����Tָ���Ĳ�����ʼ������OC4
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);   //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);	//ʹ��TIM1��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);	//ʹ��TIM1��CCR3�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);	//ʹ��TIM1��CCR4�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);			//ARPEʹ�� 
	TIM_CtrlPWMOutputs(TIM1,ENABLE); 			//ʹ��TIM1��PWM���
	TIM_Cmd(TIM1,ENABLE);						//ʹ��TIM1                         
}

//TIM1 PWM���ֳ�ʼ�� 
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM8_PWM_Init(u32 arr,u32 psc)
{     
    GPIO_InitTypeDef  GPIO_InitStructure;        
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);    	//TIM1ʱ��ʹ��  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);   	//ʹ��PORTEʱ��
        
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);  	//GPIOE9����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);	//GPIOE11����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);	//GPIOE13����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);	//GPIOE14����Ϊ��ʱ��1	
     
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;          //GPIOE9��GPIOE11��GPIOE13��GPIOE14
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       //���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //�ٶ�100MHz
    GPIO_Init(GPIOC,&GPIO_InitStructure);              //��ʼ��PE9,PE11,PE13,PE14
     
	TIM_TimeBaseStructure.TIM_Prescaler = psc;            			 //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   	 //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = arr;                       	 //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;        
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);          		 //��ʼ����ʱ��1
                
    //��ʼ��TIM1 Channel1,2,3,4 PWMģʽ        
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;     			//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  	//�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_Pulse = arr;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  		//������Ը�

	TIM_OC1Init(TIM8, &TIM_OCInitStructure);    //����Tָ���Ĳ�����ʼ������OC1
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);	//����Tָ���Ĳ�����ʼ������OC2
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);    //����Tָ���Ĳ�����ʼ������OC3
	TIM_OC4Init(TIM8, &TIM_OCInitStructure); 	//����Tָ���Ĳ�����ʼ������OC4
	
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);   //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);	//ʹ��TIM1��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);	//ʹ��TIM1��CCR3�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);	//ʹ��TIM1��CCR4�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM8,ENABLE);			//ARPEʹ�� 
	TIM_CtrlPWMOutputs(TIM8,ENABLE); 			//ʹ��TIM1��PWM���
	TIM_Cmd(TIM8,ENABLE);						//ʹ��TIM1                         
}
