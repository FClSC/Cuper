#include "uart.h"
#include "sys.h"
#include <stdio.h>
#include <stdlib.h>
#include "stdarg.h"    
#include "string.h" 
#include "delay.h"
#include "math.h"


uint8_t Serial_Flag;
uint8_t Serial_Flag2;
uint8_t Serial_Flag5=0;
char UART5_RX_BUF[100];
uint8_t Serial_TXPacket[3];
uint8_t Serial_RXPacket[9];
uint8_t USART2_TX_BUF[100];


volatile float global_angle=0.0;
volatile uint8_t new_data_received;
volatile float angular_velocity_y;
volatile float angular_velocity_z;
uint8_t received_data_packet[11];







// PA9    TX
// PA10   RX
/********************************
�������� : ����1�ĳ�ʼ��
������� : ��
������� ����
*********************************/
void UART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
  USART_Init(USART1, &USART_InitStructure); 
	
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
  USART_Cmd(USART1, ENABLE);
}
/********************************
�������� : ����1 ����һ���ֽ�
������� : һ���ֽ�
������� ����
*********************************/
void USART1_SendBits(uint8_t data)
{
	USART1->DR = data;
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}
/********************************
�������� : ����1 ����һ������
������� : һ������
������� ����
*********************************/
void UART1_SendArray(uint8_t *array,uint8_t length)
{
	uint8_t i;
	for(i=0;i<length;i++)
	{
		USART1_SendBits(array[i]);
	}
}
/********************************
�������� : ����1 ����һ���ַ���
������� : һ���ַ���
������� ����
*********************************/
void UART1_SendString(uint8_t *String1)
{
	uint8_t i;
	for(i=0;String1[i]!='\0';i++)
	{
		USART1_SendBits(String1[i]);		
	}
}
/********************************
�������� : ����1 ����һ������
������� : һ������
������� ����
*********************************/
void UART1_SendNum(uint32_t Number,uint8_t Length)
{
	uint8_t i;
	for(i=0;i<Length;i++)
	{
		USART1_SendBits(Number/Serial_Pow(10,Length-i-1)%10 + '0');
	}
}
/********************************
�������� : x��y�η�
������� : x��y
������� ����
*********************************/
uint32_t Serial_Pow(uint32_t x,uint32_t y)
{
	uint32_t Result=1;
	while(y--)
	{
		Result *= x;
	}
	return Result;
}
/********************************
�������� : ����һ�����ݰ� ��ͷ0xff ��β0xfe
������� : ���ݰ�
������� ����
*********************************/
void UART_SendPacket(uint8_t *Serial_TXPacket)
{
	USART1_SendBits(0xFF); //��ͷ
	UART1_SendArray(Serial_TXPacket,6); //����
	USART1_SendBits(0xFE); //��β
}
/********************************
�������� : ����һ�����ݰ� ��ͷ0xff ��β0xfe
������� : һ���ֽ�
������� ����
*********************************/
void UART_SendPacket2UP(uint8_t data)
{
	USART1_SendBits(0xFF); //��ͷ
	USART1_SendBits(data);
	USART1_SendBits(0xFE); //��β
}
/********************************
�������� : ����1���жϴ�����
������� : ��
������� ����
*********************************/
void USART1_IRQHandler(void)
{
	static uint8_t RX1State=0;
	static uint8_t pRx1Packet=0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		uint8_t Rx1Data = USART_ReceiveData(USART1);
		if(RX1State == 0)
		{
			if(Rx1Data == 0xFF)
			{
					RX1State =1;
					pRx1Packet=0;
			}
		}
		else if(RX1State == 1)
		{
				Serial_RXPacket[pRx1Packet] = Rx1Data;
				pRx1Packet++;
				if(pRx1Packet>8)
				{
					RX1State=2;
				}
		}
		else if(RX1State == 2)
		{
			if(Rx1Data == 0xFE)
			{
					RX1State = 0;
					Serial_Flag = 1;
			}
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}
/********************************
�������� : ����1�Ļ�ȡ����������ɵı�־λ
������� : ��
������� ��1 �������  0 ����δ���
*********************************/
uint8_t Serial1_GetRxFlag(void)
{
	if(Serial_Flag == 1)
	{
		Serial_Flag=0;
		return 1;
	}
	return 0;
}


/******************************************************************************
PD5  TX2
PD6  RX2
******************************************************************************/
/********************************
�������� : ����2�ĳ�ʼ��
ӳ��
������� : ��
������� ����
*********************************/
void UART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
  USART_Init(USART2, &USART_InitStructure); 
	
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
  USART_Cmd(USART2, ENABLE);
	
}
/********************************
�������� : ����2 ����һ���ֽ�
������� : һ���ֽ�
������� ����
*********************************/
void USART2_SendBits(uint8_t data)
{
	USART2->DR = data;
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
}

void UART2_SendArray(uint8_t *array,uint8_t length)
{
	uint8_t i;
	for(i=0;i<length;i++)
	{
		USART2_SendBits(array[i]);
	}
}

void USART2_SendString(uint8_t *String1)
{
	uint8_t i;
	for(i=0;String1[i]!='\0';i++)
	{
		USART2_SendBits(String1[i]);		
	}
}

int fputc(int ch, FILE *f)
{
	USART2_SendBits(ch);
	
	return ch;
}
void USART2_IRQHandler(void)
{
//	static uint8_t RX2State=0;
//	static uint8_t pRx2Packet=0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
	{
		uint8_t Rx2Data = USART_ReceiveData(USART2);
		// if(RX2State == 0)
		// {
		// 	if(Rx2Data == 0xFF)
		// 	{
		// 			RX2State =1;
		// 			pRx2Packet=0;
		// 	}
		// }
		// else if(RX2State == 1)
		// {
		// 		Serial_RXPacket[pRx2Packet] = Rx2Data;
		// 		pRx2Packet++;
		// 		if(pRx2Packet>7)
		// 		{
		// 			RX2State=2;
		// 		}
		// }
		// else if(RX2State == 2)
		// {
		// 	if(Rx2Data == 0xFE)
		// 	{
		// 			RX2State = 0;
		// 			Serial_Flag2 = 1;
		// 	}
		// }
		if(Rx2Data == 0x02) //һ���������°�ť�����Ƭ�����Ͷ��0x02��ֻҪ�յ�һ�����ͻ�ѱ�־λ��1
		Serial_Flag2 = 1;
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}

/********************************
�������� : ����2�Ļ�ȡ����������ɵı�־λ
������� : ��
������� ��1 �������  0 ����δ���
*********************************/
uint8_t Serial2_GetRxFlag(void)
{
	if(Serial_Flag2 == 1)
	{
		Serial_Flag2=0;
		return 1;
	}
	return 0;
}



	void u2_printf(char* fmt,...)  
	{  
	 u16 i,j;
	 va_list ap;
	 va_start(ap,fmt);
	 vsprintf((char*)USART2_TX_BUF,fmt,ap);
	 va_end(ap);
	 i=strlen((const char*)USART2_TX_BUF);//�˴η������ݵĳ���
	 for(j=0;j<i;j++)//ѭ����������
	 { 
	  USART_SendData(USART2,(uint8_t)USART2_TX_BUF[j]);   //�������ݵ�����2
	   while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);  //�ȴ��ϴδ������ 
	 } 
	 USART_SendData(USART2,(uint8_t)0xff);  //���������Ϊ��ĵ�Ƭ���Ĵ��ڷ��͵��ֽں���
	 while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
	 USART_SendData(USART2,(uint8_t)0xff);  //���������Ϊ��ĵ�Ƭ���Ĵ��ڷ��͵��ֽں���
	 while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
	 USART_SendData(USART2,(uint8_t)0xff);  //���������Ϊ��ĵ�Ƭ���Ĵ��ڷ��͵��ֽں���
	 while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
	}
	

//�����ģ�������ģ��
// 0xFF, 0xAA, 0x76, 0x00, 0x00     Z��Ƕȹ���
// 0xFF, 0xAA, 0x03, 0x08, 0x00}    �����������50HZ
///////////////////////////////
void UART4_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//���ȼ���Ҫ���ߣ�ռ����ݮ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	USART_Init(UART4, &USART_InitStructure); 
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(UART4, ENABLE);
	
// 	//������������
// 	Uart4_SendArray(unlock_register,sizeof(unlock_register));
// 	//Uart4_SendArray(reset_z_axis, sizeof(reset_z_axis));
//    Uart4_SendArray(set_output_200Hz, sizeof(set_output_200Hz));
//    Uart4_SendArray(set_baudrate_115200, sizeof(set_baudrate_115200));
//    Uart4_SendArray(save_settings, sizeof(save_settings));


}

void Uart4_SendByte(uint8_t byte)
{
	UART4->DR = byte;
	while(USART_GetFlagStatus(UART4,USART_FLAG_TXE) == RESET);
}

void Uart4_SendArray(uint8_t *Array,uint16_t Length)
{
	uint16_t i;
	for(i=0;i<Length;i++)
	{
		Uart4_SendByte(Array[i]);	
	}
    // Uart4_SendByte(0x0D);//���ͻ��з�
	// Uart4_SendByte(0x0A);//���ͻ��з�
	delay_ms(200);        //��ʱ����
	
}

void Uart4_SendString(char* String)
{
	uint8_t i;
	for(i=0;String[i]!='\0';i++)
	{
		Uart4_SendByte(String[i]);	
	}
}
uint32_t Uart4_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Result=1;
	while(Y--)
	{
		Result*=X;
	}
	return Result;
}

void Uart4_SendNumber(uint32_t Number,uint8_t Length)
{
	uint8_t i;
	for(i=0;i<Length;i++)
	{
		Uart4_SendByte(Number /Uart4_Pow(10,Length-i-1) % 10 + '0');
	}
	
}

// void Uart4_Printf(char* format,...)
// {
// 	char String[100];
// 	va_list arg;
// 	va_start(arg,format);
// 	vsprintf(String,format,arg);
// 	va_end(arg);
// 	Uart4_SendString(String);
	
// }


void UART4_IRQHandler(void)
{
	static uint8_t Rx_Buffer[11];
	static uint8_t Rx_Index=0;
	static uint8_t state = 0; //״̬���� 0��Ѱ�Ұ�ͷ 1����������
	
	if (USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(UART4);
		
		if(state==0)//Ѱ�Ұ�ͷ
		{
			if(RxData==0x55)
			{
				Rx_Buffer[0]=RxData;
				Rx_Index=1;
                state=1;				
			}			
		}
		
		else if (state==1)//��������
		{
			Rx_Buffer[Rx_Index++]=RxData;
			if(Rx_Index==2&&Rx_Buffer[1]!=0x53&&Rx_Buffer[1]!=0x52)
				//����ڶ����ֽڲ���0x53����0x52��Ѱ����һ��0x55
			{
				if(Rx_Buffer[1]==0x55)Rx_Index=1;
				else 
				{
					state=0;
					Rx_Index=0;
					
				}			
			}
			
			else if(Rx_Index==11)//���յ����������ݰ�
			{
        
               ParseData(Rx_Buffer,11);//��������
				Rx_Index=0;
				state=0;    //����Ѱ�Ұ�ͷ
			}					
		}
		

    USART_ClearITPendingBit(UART4, USART_IT_RXNE);			
		
	
}

}
void ParseData(uint8_t *data, uint16_t length)
{
   if (length == 11) 
		{
       uint8_t checksum = CalculateChecksum(data, length - 1, data[1]);
       if (checksum != data[length - 1]) 
		{
           //Uart4_Printf("Checksum error\r\n");
           return;
       }

       if (data[0] == 0x55 && data[1] == 0x53)//�ǶȵĽ���
		{
           // ���ݲ��ִ�����6��ʼ�����ֽ���ǰ�����ֽ��ں�
           uint8_t yaw_l = data[6];
           uint8_t yaw_h = data[7];
           int16_t yaw = (int16_t)((yaw_h << 8) | yaw_l);

           // �������������ֵת��Ϊ�Ƕ�
		//    float raw_angle = ((float)yaw) / 32768.0 * 180.0; // ԭʼ�Ƕ�
		//    filtered_angle = alpha * raw_angle + (1 - alpha) * filtered_angle; // һ�׵�ͨ�˲�
		//    global_angle = filtered_angle; // ����ȫ�ֽǶ�

           float angle = ((float)yaw / 32768.0) * 180.0;   //ֻ��Ҫ�������
           global_angle = angle;
           new_data_received = 1;
       }
        else if (data[0] == 0x55 && data[1] == 0x52) 
		{
           //�������ٶ�����
           uint8_t wy_l = data[4];
           uint8_t wy_h = data[5];
           int16_t wy = (int16_t)((wy_h << 8) | wy_l);
           angular_velocity_y = ((float)wy / 32768.0) * 2000.0;

           uint8_t wz_l = data[6];
           uint8_t wz_h = data[7];
           int16_t wz = (int16_t)((wz_h << 8) | wz_l);
           angular_velocity_z = ((float)wz / 32768.0) * 2000.0;

           new_data_received = 1;
       }
				
   }
}


uint8_t CalculateChecksum(uint8_t *data, uint16_t length, uint8_t type) 
{
   uint8_t checksum = 0;
   for (uint16_t i = 0; i < length; i++) 
	{
       checksum += data[i];
   }
   return checksum;
}

void ResetAng_Z(void)
{
	Uart4_SendByte(0xFF);
	Uart4_SendByte(0xAA);
	Uart4_SendByte(0x76);
	Uart4_SendByte(0x00);
	Uart4_SendByte(0x00);
	delay_ms(200);
	
}

int16_t determicro(int target,float real)  //�����Ƿ�΢��
{
	//�����ת��ĽǶ���85-95��֮�����-85��-95֮�䣬����1������΢��
     if((fabs(target-real)<20)&&(fabs(target-real))>0.1)
     {
		 return 1;
	 }
	 else
	 {
		 return 0;
	 }

}

















//���UART5_RX_BUF��0-6λ������Ҫ��  ��  123+321
//������    ɨ��ģ��   ����USART5 
//��Ƭ��   PC12 TX    PD2 RX   
void UART5_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);//����UART5��APB1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	//PC12   TX 
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//PD2   RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(UART5, &USART_InitStructure);
	
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);


	
	USART_Cmd(UART5, ENABLE);
}

/********************************
�������� : ����5 ����һ���ֽ�
������� : һ���ֽ�
������� ����
*********************************/
void USART5_SendBits(uint8_t data)
{
	UART5->DR = data;
	while(USART_GetFlagStatus(UART5,USART_FLAG_TXE) == RESET);
}
/********************************
�������� : ����5 ����һ������
������� : һ������
������� ����
*********************************/
void UART5_SendArray(uint8_t *array,uint8_t length)
{
	uint8_t i;
	for(i=0;i<length;i++)
	{
		USART5_SendBits(array[i]);
	}
}
/********************************
�������� : ����5 ����һ���ַ���
������� : һ���ַ���
������� ����
*********************************/
void UART5_SendString(uint8_t *String1)
{
	uint8_t i;
	for(i=0;String1[i]!='\0';i++)
	{
		USART5_SendBits(String1[i]);		
	}
}


//����5��ñ�־λ
uint8_t Serial5_GetRxFlag(void)
{
	if(Serial_Flag5 == 1)
	{
		Serial_Flag5=0;
		return 1;
	}
	return 0;
}

//  @123+321#$  @123+321#$

void UART5_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;	
    if (USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
	{
        uint8_t RxData = USART_ReceiveData(UART5);
        if(RxState==0)
		{
			if(RxData=='@'&&Serial_Flag5==0)
			{
				RxState=1;
				pRxPacket=0;		
			}			
		}
		else if (RxState==1)
		{
			if(RxData=='#')
			{
				RxState=2;
			}
			else
			{
				UART5_RX_BUF[pRxPacket]=RxData;
				pRxPacket++;
			}
		}
		else if(RxState==2)
		{
			if(RxData=='$')
			{
				RxState=0;
				UART5_RX_BUF[pRxPacket]='\0';
				Serial_Flag5=1;				
			}
			
			
		}
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
  }
}


void UART5_Start_Scan(void)//����ɨ��ָ��,������ó�һֱɨ��ֱ��ɨ��ֹͣ
{

   uint8_t Order[9]={0x7E,0x00,0x08,0x01,0x00,0x02,0x01,0xAB,0xCD};

   UART5_SendArray(Order,9);


}


void UART5_ParseCode(const char *Buf, int16_t *code1, int16_t *code2)
{
    // ֱ�ӽ�����һ����λ��
    *code1 = (Buf[0] - '0') * 100 + 
            (Buf[1] - '0') * 10 + 
            (Buf[2] - '0');

    // ���� '+' �Ž����ڶ�����λ��
    *code2 = (Buf[4] - '0') * 100 + 
            (Buf[5] - '0') * 10 + 
            (Buf[6] - '0');
	
}
//�ѽ���������ݷ�����ݮ�ɣ��п��ܻ��һ�����з���







