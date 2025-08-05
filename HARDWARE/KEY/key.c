#include "key.h"

/********************************
�������� : �����Ŀ��ƣ�һ������
������� : ��
������� ����
*********************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//�������룬�������ڵ͵�ƽ�����¿ɶ�ȡ�ߵ�ƽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);

	GPIO_ResetBits(GPIOE,GPIO_Pin_6);

	
}
/********************************
�������� : ����KEY�Ķ�ȡ
������� : ��
������� �����·���1��û�а��·���0
*********************************/
uint8_t Key_Get(void)
{
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6) == 1)
	{
		delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6) == 1);
		delay_ms(20);
		KeyNum = 1;
	}
	
	return KeyNum;
}
