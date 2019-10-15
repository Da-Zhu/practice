#include "ads1256.h"
#include "delay.h"
#include "usart.h"

u8 results1,results2,results3;

/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_ADS1256_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable SPI1 and GPIO clocks */
  /*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO, 
       SPI_FLASH_SPI_MISO_GPIO, SPI_FLASH_SPI_DETECT_GPIO 
       and SPI_FLASH_SPI_SCK_GPIO Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);

  /*!< SPI_FLASH_SPI Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  /*!< AFIO Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  
  /*!< Configure SPI_FLASH_SPI pins: RESET */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                    			//
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;						// �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  /*!< Configure SPI_FLASH_SPI pins:SCK | DIN */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;                    //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;							// �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  /*!< Configure SPI_FLASH_SPI pins: DOUT */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                    			//
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;							//��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Deselect the FLASH: Chip Select high */

  /* SPI1 configuration */
  // ��SCLK�½��أ�ϵͳͨ��DIN��1256�������� 
  // ��SCLK�����أ�ϵͳͨ��DOUT ��ȡ1256����
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	  //����ȫ˫��
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							  //����ģʽ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						  //8λģʽ
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							  //SCLK����ʱλ�͵�ƽ
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;							  //SCLK���½��زɼ�����
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								  //�ӻ�ѡ���źţ����ģʽ��������GPIOѡ��ӻ�
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;	  //APB2����APB1����Ƶ�ʵ�1/2~1/256��Ƶ
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					  //���ݴ����MSBλ��ʼ
  SPI_InitStructure.SPI_CRCPolynomial = 7;								  //CRCֵ����Ķ���ʽ
  SPI_Init(SPI1, &SPI_InitStructure);


  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
u8 SPI_SendByte(u8 byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI1, byte);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}


/*******************************************************************
����������ʼ��DAC�ӿ�
����ԭ�ͣ�void ADC_gpio_init()
����˵������
���أ�  ��
********************************************************************/
void ADS1256_GPIO_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/////////CS  reset ��������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_4 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;	//--�������ģʽ
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_10MHz; 	//--GPIO��ת�ٶ�Ϊ50MHz

	GPIO_Init(GPIOA , &GPIO_InitStructure);	//--������д���Ӧ��GPIO�Ĵ�����	


//	GPIO_InitTypeDef GPIO_InitStructure;
    ///////// DRDY��������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING;	//--��������ģʽ
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_10MHz; 	//--GPIO��ת�ٶ�Ϊ50MHz
	GPIO_Init(GPIOA , &GPIO_InitStructure);	//--������д���Ӧ��GPIO�Ĵ�����

}

//-----------------------------------------------------------------//
//	��    �ܣ�ADS1256 д����
//	��ڲ���: /
//	���ڲ���: /
//	ȫ�ֱ���: /
//	��    ע: ��ADS1256�е�ַΪregaddr�ļĴ���д��һ���ֽ�databyte
//-----------------------------------------------------------------//

void ADS1256WREG(unsigned char regaddr,unsigned char databyte)
{
    ADS_CS_LOW();
	while(ADS_DRDY);//��ADS1256_DRDYΪ��ʱ����д�Ĵ���
	//��Ĵ���д�����ݵ�ַ
    SPI_SendByte(ADS1256_CMD_WREG | (regaddr & 0xF));
    //д�����ݵĸ���n-1
    SPI_SendByte(0);
    //��regaddr��ַָ��ļĴ���д������databyte
  	delay_us(5);
    SPI_SendByte(databyte);
	ADS_CS_HIGH();
}

//-----------------------------------------------------------------//
//	��    �ܣ�ADS1256 ���Ĵ�������
//	��ڲ���: /
//	���ڲ���: /
//	ȫ�ֱ���: /
//	��    ע: ��ADS1256�е�ַΪregaddr�ļĴ�������һ���ֽ�databyte
//-----------------------------------------------------------------//

unsigned char ADS1256RREG(unsigned char regaddr)
{
    //��ADS1256�е�ַΪregaddr�ļĴ�������һ���ֽ�
    unsigned char r=0;
    ADS_CS_LOW();
	while(ADS_DRDY);//��ADS1256_DRDYΪ��ʱ����д�Ĵ���
	//д���ַ
    SPI_SendByte(ADS1256_CMD_RREG | (regaddr & 0xF));
    //д���ȡ���ݵĸ���n-1
    SPI_SendByte(0);
	  delay_us(10);  //min=50*(1/fCLKIN)=50*(1/7.68MHZ)=6500ns;max=whatever
    //����regaddr��ַָ��ļĴ���������
    r=SPI_SendByte(0);
	  ADS_CS_HIGH();

    return r;//��������
}

//-----------------------------------------------------------------//
//	��    �ܣ�ADS1256��ʼ���ӳ���
//	��ڲ���: /
//	���ڲ���: /
//	ȫ�ֱ���: /
//	��    ע: /
//-----------------------------------------------------------------//
void ADS1256_Init(void)
{     
	ADS_CS_LOW();
	RESET_HIGH();
	delay_us(100);
	SPI_SendByte(ADS1256_CMD_REST);
	delay_ms(10);
	while(ADS_DRDY);                                //��ADS1256_DRDYΪ��ʱ����д�Ĵ���
	SPI_SendByte(ADS1256_CMD_SYNC);                 //ͬ������
	SPI_SendByte(ADS1256_CMD_WAKEUP);               //ͬ������
	
	while(ADS_DRDY);
	SPI_SendByte(ADS1256_CMD_WREG | ADS1256_STATUS);//����д��4���Ĵ���
	SPI_SendByte(3);
	SPI_SendByte(0x04);                             //��λ��ǰ��ʹ���ڲ�У׼����ʹ�û���
	SPI_SendByte(ADS1256_MUXP_AIN2 | ADS1256_MUXN_AIN3);//�˿�����A2Ϊ����A3λ��
	SPI_SendByte(ADS1256_GAIN_1);                   //�Ŵ�������
	SPI_SendByte(ADS1256_DRATE_2_5SPS);             //�ɼ��ٶ�����
	delay_us(100);
	
	while(ADS_DRDY);                                //��ADS1256_DRDYΪ��ʱ����д�Ĵ���
	SPI_SendByte(ADS1256_CMD_SELFCAL);              //ƫ�ƺ������Զ�У׼
	
    ADS_CS_HIGH();
	delay_ms(100);
 
}

//-----------------------------------------------------------------//
//	��    �ܣ�
//	��ڲ���: /
//	���ڲ���: /
//	ȫ�ֱ���: /
//	��    ע:
//-----------------------------------------------------------------//

unsigned long ADS1256ReadData(void)  
{
    unsigned char i=0;
    unsigned long sum=0;
		unsigned long r=0;
		ADS_CS_LOW();

		while(ADS_DRDY);               //��ADS1256_DRDYΪ��ʱ����д�Ĵ��� 	
		SPI_SendByte(ADS1256_CMD_SYNC);
		SPI_SendByte(ADS1256_CMD_WAKEUP);
		SPI_SendByte(ADS1256_CMD_RDATA);
		delay_us(10);                  //min=50*(1/fCLKIN)=50*(1/7.68MHZ)=6500ns;max=whatever
			for(i=0;i<3;i++)
			{
				sum = sum << 8;
			r = SPI_SendByte(0);  
				sum |= r;
			}
		ADS_CS_HIGH();		 
    return sum;
}

//-----------------------------------------------------------------//
//	��    �ܣ���ȡADS1256��·����
//	��ڲ���: /
//	���ڲ���: /
//	ȫ�ֱ���: /
//	��    ע: /
//-----------------------------------------------------------------//
void sendChar(unsigned char ch)
{
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
}


unsigned char i=0;
	long ulResult;
	long double ldVolutage;

unsigned long ADS_sum(unsigned char road)
{
	unsigned long results=0;
	unsigned long Result_sum=0;
	unsigned long fVoltage;
	unsigned char buf[10],i,rod;

	ADS1256WREG(ADS1256_MUX,road);		//����ͨ��
	SPI_SendByte(ADS1256_CMD_SELFCAL);    //ƫ�ƺ������Զ�У׼
	
	rod = (road >> 4)  ;
//	printf("��");
//	sendChar(rod);
//	printf("·��");

	Result_sum = ADS1256ReadData();//��ȡADֵ������24λ���ݡ�
    ulResult= ADS1256ReadData();//��ȡADֵ������24λ���ݡ�
//	return results;

	if(Result_sum<0x800000)    //��õ�ѹֵΪ�������
	{
//		fVoltage=Result_sum*985;  //954=2*2.5/2^19*100000000   2.5Ϊ��׼��ѹ  100000000ΪΪ�˼����׼ȷ��ת��Ϊ���μ��㡣����Ϊ954��ʵ������Ϊ985
//		buf[0]=(unsigned char)(fVoltage/100000000);
//		buf[0]=buf[0]+0x30;
//		buf[1]='.';
//		buf[2]=(unsigned char)((fVoltage%100000000)/10000000);
//		buf[2]=buf[2]+0x30;
//		buf[3]=(unsigned char)((fVoltage%10000000)/1000000);
//		buf[3]=buf[3]+0x30;
//		buf[4]=(unsigned char)(fVoltage%1000000/100000);
//		buf[4]=buf[4]+0x30;
//		buf[5]=(unsigned char)(fVoltage%100000/10000);
//		buf[5]=buf[5]+0x30;
//		buf[6]=(unsigned char)(fVoltage%10000/1000);
//		buf[6]=buf[6]+0x30; 
//		buf[7]=(unsigned char)(fVoltage%1000/100);
//		buf[7]=buf[7]+0x30;       
//		buf[8]='V';			//V       
//		buf[9]=0x20;
		
		if( ulResult & 0x800000 )
			{
			 	ulResult = ~(unsigned long)ulResult;
				ulResult &= 0x7fffff;
				ulResult += 1;
				ulResult = -ulResult;
			}
		
			ldVolutage = (long double)ulResult*0.59604644775390625;

			printf("��%dͨ��:",rod);
			printf("%lf",ldVolutage); 	//double
			printf("uV\r\n");


	 }
	 else					 //��õ�ѹֵΪ�������
	 {
		//buf[0]='-';					       //ȡ��-����
		Result_sum=0xfffff-Result_sum;	   //ȡ������ 
		fVoltage=Result_sum*985;  //954=2*2.5/2^19*100000000   2.5Ϊ��׼��ѹ  100000000ΪΪ�˼����׼ȷ��ת��Ϊ���μ���
		buf[0]=(unsigned char)(fVoltage/100000000);
		buf[0]=buf[0]+0x30;
		buf[1]='.';
		buf[2]=(unsigned char)((fVoltage%100000000)/10000000);
		buf[2]=buf[2]+0x30;
		buf[3]=(unsigned char)((fVoltage%10000000)/1000000);
		buf[3]=buf[3]+0x30;
		buf[4]=(unsigned char)(fVoltage%1000000/100000);
		buf[4]=buf[4]+0x30;
		buf[5]=(unsigned char)(fVoltage%100000/10000);
		buf[5]=buf[5]+0x30;
		buf[6]=(unsigned char)(fVoltage%10000/1000);
		buf[6]=buf[6]+0x30; 
		buf[7]=(unsigned char)(fVoltage%1000/100);
		buf[7]=buf[7]+0x30;       
		buf[8]='V';			//V         
		buf[9]='-';
	 }
	
//	   i=0;
//	   while(i<10)
//	   {
//		 sendChar(buf[i]);
//		 i++;
//	   }
			return results; 
////	ADS1256WREG(ADS1256_MUX,road);		//����ͨ��
//	results = ADS1256ReadData();        //��ȡADֵ������24λ���ݡ�
//	results1 = (results >> 16) & 0x0000ff; //�������λ	
//	results2 = (results >> 8) & 0x0000ff;  //�����м�λ
//    results3 = results & 0x0000ff;		   //���͵�λ
}






