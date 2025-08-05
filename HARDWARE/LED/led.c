#include "led.h"


/********************************
LED1    PE3
**********************************/
/********************************
�������� : led�Ŀ���
������� : ��
������� ����
*********************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStruct);	

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStruct);	
	
	GPIO_SetBits(GPIOB,GPIO_Pin_5);//���
	GPIO_SetBits(GPIOE,GPIO_Pin_5);//�̵�
	GPIO_SetBits(GPIOE,GPIO_Pin_3);//LED�����
}

void LED_Turn(void)  //���ö�LED����ƽ��п��ز���
{
	if(GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_3)==1)
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_3);
	}
	else
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_3);
	}
}
