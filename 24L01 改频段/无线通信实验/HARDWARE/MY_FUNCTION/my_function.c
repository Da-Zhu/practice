#include "my_function.h"
#include "sys.h" 
#include "delay.h"
#include "math.h"
#include "usart.h"



/////////////////////////////////////////////////////////////////////////////////
//将abc分开一位一位地放入数组中
//
/////////////////////////////////////////////////////////////////////////////////
u8 do_mat(u16 abc,u8 *save,u8 start,u8 end)

{
	
    u8 i;
	u16 divide_num;

	
	for(i=start;i<end+1;i++)
	{
		divide_num=pow(10,(end-i));
	    save[i]=abc/divide_num%10;
		
//		printf("%d  \r\n",divide_num);
	}
//	for(i=0;i<end+1;i++)
//	{
////	  printf("%d  ",save[i]);
//	}
//	printf("\r\n");
	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//My_KEY
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void My_Key_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_10;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE1,2,3,4
	
}


u8 My_Key_Scan(u8 mode)
{
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY4==0||KEY5==0||KEY7==0||KEY8==0||KEY9==0||KEY10==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
             if(KEY4==0)return KEY4_PRES;
		else if(KEY5==0)return KEY5_PRES;
		else if(KEY7==0)return KEY7_PRES;
	    else if(KEY8==0)return KEY8_PRES;
	    else if(KEY9==0)return KEY9_PRES;
		else if(KEY10==0)return KEY10_PRES;
	}else if(KEY4==1&&KEY5==1&&KEY7==1&&KEY8==1&&KEY9==1&&KEY10==1)key_up=1; 	    
 	return 0;// 无按键按下

}




extern u8 tmp_buf[33];	

u8 key;
u8 pwm=123;

void KEY_Run(void)
{
 
 
   key=My_Key_Scan(0);	//得到键值
	
	   	if(key)
		{						   
			switch(key)
			{				 
				case KEY4_PRES:	//控制LED0翻转
                                 pwm+=5;
					break;
				case KEY5_PRES:	//控制LED1翻转	 
                                 pwm-=5;
					break;
				case KEY7_PRES:	//同时控制LED0,LED1翻转 
                                 do_mat(pwm,tmp_buf,0,2);
				
					break;
				case KEY8_PRES:	//同时控制LED0,LED1翻转 

					break;
				case KEY9_PRES:	//同时控制LED0,LED1翻转 

					break;
				case KEY10_PRES:	//同时控制LED0,LED1翻转 

					break;
			}
		}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//easytrace
//实时跟踪识别物体
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v1)))//取最大
#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))//取最小值

typedef struct
	  {
    unsigned char  red;             // [0,255]
    unsigned char  green;           // [0,255]
    unsigned char  blue;            // [0,255]
    }COLOR_RGB;//RGB格式颜色

typedef struct
	  {
    unsigned char hue;              // [0,240]
    unsigned char saturation;       // [0,240]
    unsigned char luminance;        // [0,240]
    }COLOR_HSL;//HSL格式颜色

typedef struct
	  {
    unsigned int X_Start;              
    unsigned int X_End;
	  unsigned int Y_Start;              
    unsigned int Y_End;
    }SEARCH_AREA;//区域

//读取RBG格式颜色，唯一需要移植的函数
extern unsigned short LCD_ReadPoint(unsigned short x,unsigned short y);//读某点颜色

static void ReadColor(unsigned int x,unsigned int y,COLOR_RGB *Rgb)
	{
		unsigned short C16;

		C16 = LCD_ReadPoint(x,y);     //读某点颜色

		Rgb->red   =	 (unsigned char)((C16&0xf800)>>8);
		Rgb->green =	 (unsigned char)((C16&0x07e0)>>3);
		Rgb->blue  =   (unsigned char)((C16&0x001f)<<3);
	}


u16 k;
//RGB转HSL
static void RGBtoHSL(const COLOR_RGB *Rgb, COLOR_HSL *Hsl)
	{
			int h,s,l,maxVal,minVal,difVal;
			int r  = Rgb->red;
			int g  = Rgb->green;
			int b  = Rgb->blue;
		
				maxVal = max3v(r, g, b);
				minVal = min3v(r, g, b);
				
				difVal = maxVal-minVal;
		
		//计算亮度
			l = (maxVal+minVal)*240/255/2;
		
		if(maxVal == minVal)//若r=g=b
			{
				h = 0; 
				s = 0;
			}
		else
		{
			//计算色调
			if(maxVal==r)
			{
				if(g>=b)
					h = 40*(g-b)/(difVal);
				else
					h = 40*(g-b)/(difVal) + 240;
			}
			else if(maxVal==g)
					h = 40*(b-r)/(difVal) + 80;
			else if(maxVal==b)
					h = 40*(r-g)/(difVal) + 160;
			//计算饱和度
			if(l == 0)
					s = (difVal)*240/(511 - (maxVal+minVal));
			else if(l<=120)
					s = (difVal)*240/(maxVal+minVal);
			else
					s = (difVal)*240/(511 - (maxVal+minVal));
		}
			Hsl->hue =        (unsigned char)(((h>240)? 240 : ((h<0)?0:h)));//色度
			Hsl->saturation = (unsigned char)(((s>240)? 240 : ((s<0)?0:s)));//饱和度
			Hsl->luminance =  (unsigned char)(((l>240)? 240 : ((l<0)?0:l)));//亮度
//            k++;
//		    if(k>1000)
//			{
//			  printf("%d  %d  %d    ",Hsl->hue,Hsl->saturation,Hsl->luminance);
//				k=0;
//			}
			
	}

//匹配颜色
static int ColorMatch(const COLOR_HSL *Hsl,const TARGET_CONDI *Condition)//HSL格式颜色、判定为的目标条件
	{
		if( 
					Hsl->hue		>	Condition->H_MIN &&
					Hsl->hue		<	Condition->H_MAX &&
					Hsl->saturation	>	Condition->S_MIN &&
					Hsl->saturation	<   Condition->S_MAX &&
					Hsl->luminance	>	Condition->L_MIN &&
					Hsl->luminance	<   Condition->L_MAX 
			)          //hue为色调，saturation为饱和度 ，luminance为亮度
				return 1;
		else
				return 0;
	}

//搜索腐蚀中心
static int SearchCentre(unsigned int *x,unsigned int *y,const TARGET_CONDI *Condition,const SEARCH_AREA *Area)
//TARGET_CONDI判定为的目标条件、SEARCH_AREA区域
	{
			unsigned int SpaceX,SpaceY,i,j,k,FailCount=0;
			COLOR_RGB Rgb;
			COLOR_HSL Hsl;
			
			SpaceX = Condition->WIDTH_MIN/3;   //目标最小宽度
			SpaceY = Condition->HIGHT_MIN/3;   //目标最小高度
		
//			SpaceX = Condition->WIDTH_MIN;   //目标最小宽度
//			SpaceY = Condition->HIGHT_MIN;   //目标最小高度

				for(i=Area->Y_Start;i<Area->Y_End;i+=SpaceY)
				{
					for(j=Area->X_Start;j<Area->X_End;j+=SpaceX)
					{
						FailCount=0;
						for(k=0;k<SpaceX+SpaceY;k++)
						{
							if(k<SpaceX)
								ReadColor(j+k,i+SpaceY/2,&Rgb);
							else
								ReadColor(j+SpaceX/2,i+(k-SpaceX),&Rgb);
							  RGBtoHSL(&Rgb,&Hsl);
							
							if(!ColorMatch(&Hsl,Condition))
								FailCount++;
							if(FailCount>((SpaceX+SpaceY)>>ALLOW_FAIL_PER))
								break;
						}
						if(k==SpaceX+SpaceY)
						{
							*x = j+SpaceX/2;
							*y = i+SpaceY/2;
							return 1;
						}
					}
				}
		return 0;
	}

//从腐蚀中心向外腐蚀，得到新的腐蚀中心
static int Corrode(unsigned int oldx,unsigned int oldy,const TARGET_CONDI *Condition,RESULT *Resu)
{
	
	unsigned int Xmin,Xmax,Ymin,Ymax,i,FailCount=0;
	COLOR_RGB Rgb;
	COLOR_HSL Hsl;
	
	for(i=oldx;i>IMG_X;i--)
		{
				ReadColor(i,oldy,&Rgb);
				RGBtoHSL(&Rgb,&Hsl);
				if(!ColorMatch(&Hsl,Condition))
					FailCount++;
				if(FailCount>(((Condition->WIDTH_MIN+Condition->WIDTH_MAX)>>2)>>ALLOW_FAIL_PER))
					break;	
		}
	Xmin=i;
	FailCount=0;
	
	for(i=oldx;i<IMG_X+IMG_W;i++)
		{
				ReadColor(i,oldy,&Rgb);
				RGBtoHSL(&Rgb,&Hsl);
				if(!ColorMatch(&Hsl,Condition))
					FailCount++;
				if(FailCount>(((Condition->WIDTH_MIN+Condition->WIDTH_MAX)>>2)>>ALLOW_FAIL_PER))
					break;	
		}
	Xmax=i;
	FailCount=0;
	
	for(i=oldy;i>IMG_Y;i--)
		{
				ReadColor(oldx,i,&Rgb);
				RGBtoHSL(&Rgb,&Hsl);
				if(!ColorMatch(&Hsl,Condition))
					FailCount++;
				if(FailCount>(((Condition->HIGHT_MIN+Condition->HIGHT_MAX)>>2)>>ALLOW_FAIL_PER))
					break;	
		}
	Ymin=i;
	FailCount=0;
	
	for(i=oldy;i<IMG_Y+IMG_H;i++)
		{
				ReadColor(oldx,i,&Rgb);
				RGBtoHSL(&Rgb,&Hsl);
				if(!ColorMatch(&Hsl,Condition))
					FailCount++;
				if(FailCount>(((Condition->HIGHT_MIN+Condition->HIGHT_MAX)>>2)>>ALLOW_FAIL_PER))
					break;	
		}
	Ymax=i;
	FailCount=0;
	
	Resu->x	= (Xmin+Xmax)/2;
	Resu->y	= (Ymin+Ymax)/2;
	Resu->w	= Xmax-Xmin;
	Resu->h	= Ymax-Ymin;

	if(
			 ((Xmax-Xmin)>(Condition->WIDTH_MIN)) && ((Ymax-Ymin)>(Condition->HIGHT_MIN)) &&\
			 ((Xmax-Xmin)<(Condition->WIDTH_MAX)) && ((Ymax-Ymin)<(Condition->HIGHT_MAX))
	   )
		  return 1;	
	else
		  return 0;	
}


void read_color(void)
{
	COLOR_RGB Rgb;
	COLOR_HSL Hsl;
    ReadColor(5,5,&Rgb);
	RGBtoHSL(&Rgb,&Hsl);

}

//唯一的API，用户将识别条件写入Condition指向的结构体中，该函数将返回目标的x，y坐标和长宽
//返回1识别成功，返回1识别失败
int Trace(const TARGET_CONDI *Condition,RESULT *Resu)  //TARGET_CONDI识别条件  RESULT识别结果
{
	unsigned int i;
	static unsigned int x0,y0,flag=0;  //静态变量
	static SEARCH_AREA Area={IMG_X,IMG_X+IMG_W,IMG_Y,IMG_Y+IMG_H};  //x坐标 y坐标  w宽度  h高度
	RESULT Result;	//RESULT识别结果
	

	if(flag==0)    //已经定义
		{
			if(SearchCentre(&x0,&y0,Condition,&Area))  //搜索腐蚀中心   &是取地址运算符
				 flag=1;
			else
				{
						Area.X_Start= IMG_X	       ;
						Area.X_End  = IMG_X+IMG_W  ;
						Area.Y_Start= IMG_Y		     ;
						Area.Y_End  = IMG_Y+IMG_H  ;

						if(SearchCentre(&x0,&y0,Condition,&Area))	
						{
							flag=0;
							return 0;
						}	
				}
		}
		
	Result.x = x0;
	Result.y = y0;
	
	for(i=0;i<ITERATE_NUM;i++)  //进行迭代计算
		 Corrode(Result.x,Result.y,Condition,&Result);
		
	if(Corrode(Result.x,Result.y,Condition,&Result))//从腐蚀中心向外腐蚀
		{
			x0=Result.x;
			y0=Result.y;
			Resu->x=Result.x;
			Resu->y=Result.y;
			Resu->w=Result.w;
			Resu->h=Result.h;
			flag=1;

			Area.X_Start= Result.x - ((Result.w)>>1);
			Area.X_End  = Result.x + ((Result.w)>>1);
			Area.Y_Start= Result.y - ((Result.h)>>1);
			Area.Y_End  = Result.y + ((Result.h)>>1);


			return 1;
		}
	else
		{
			flag=0;
			return 0;
		}

}













