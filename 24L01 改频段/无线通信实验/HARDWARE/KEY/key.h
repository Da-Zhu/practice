#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"
#include "sys.h"

#define KEY0 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define KEY1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define KEY2 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
#define KEY3 PEin(1)


#define KEY0_PRES 1
#define KEY1_PRES 2
#define KEY2_PRES 3
#define KEY3_PRES 4


void KEY_Init(void);
u8 KEY_Scan(u8);

void KEY1_Init(void);
u8 KEY1_Scan(u8);



#endif



