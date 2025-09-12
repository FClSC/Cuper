#include "servo.h"

//TIM5_CH2    PA1    servo1  

/********************************
�������� : ���1�ĳ�ʼ��
������� : ��
������� ����
*********************************/
void Servo1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=20000-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC2Init(TIM5,&TIM_OCInitStructure);
	
	TIM_OC2PreloadConfig(TIM5,ENABLE);
	TIM_ARRPreloadConfig(TIM5,ENABLE);
	
	TIM_Cmd(TIM5,ENABLE);
}
//TIM5_CH3    PA2    servo2  
/********************************
�������� : ���2�ĳ�ʼ��
������� : ��
������� ����
*********************************/
void Servo2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=20000-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC3Init(TIM5,&TIM_OCInitStructure);
	
	TIM_OC3PreloadConfig(TIM5,ENABLE);
	TIM_ARRPreloadConfig(TIM5,ENABLE);
	
	TIM_Cmd(TIM5,ENABLE);
}
//TIM5_CH4    PA3    servo3  
/********************************
�������� : ���3�ĳ�ʼ��
������� : ��
������� ����
*********************************/
void Servo3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=20000-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC4Init(TIM5,&TIM_OCInitStructure);
	
	TIM_OC4PreloadConfig(TIM5,ENABLE);
	TIM_ARRPreloadConfig(TIM5,ENABLE);
	
	TIM_Cmd(TIM5,ENABLE);
}
//TIM1_CH4    PA11    servo4  

/********************************
�������� : ���1�ĳ�ʼ��
������� : ��
������� ����
*********************************/
void Servo4_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    
    // 1. ����ʱ�� (�ؼ�����������AFIOʱ��)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); // ͬʱ����GPIOA��AFIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    // 2. ��ʼ��GPIO PA11 (TIM1_CH4)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // �����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 3. ��ʼ����ʱ��ʱ�� (������ȷ�����ֲ���)
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;     // ARR: 20ms����
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;     // PSC: 1MHz����Ƶ��
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;  // �߼���ʱ�����У���������
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
    
    // 4. ��ʼ������Ƚ�ͨ�� (������ȷ�����ֲ���)
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // ��ʼռ�ձ�
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    
    // 5. ʹ��Ԥװ�ؼĴ��� (������ȷ�����ֲ���)
    TIM_OC4PreloadConfig(TIM1, ENABLE);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    
    // 6. ����ؼ���һ����ʹ�ܸ߼���ʱ��������� --> û�������PWM��������
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    
    // 7. ʹ�ܶ�ʱ��
    TIM_Cmd(TIM1, ENABLE);
}
/*************************
270�ȶ��
�ߵ�ƽ 0.5~2.5ms ��Ӧ 270�� 
PWM���� 500~2500us ��Ӧ 0~270�� Ҳ����0.5ms ~ 2.5ms
Ĭ��������20ms

72 000 000/72             

ռ�ձ�
0.5/20   ��Ӧ0��    500/20000

2.5/20   ��Ӧ270��  2500/20000

��ô 270�ȶ�Ӧ2000������
��ôһ������ ���Զ�Ӧ0.135��
���۾��ȿ�������0.135�ȵķֱ���

��ô 1�ȶ�Ӧ7.4������

���㹫ʽΪCCR = 7.4*angle + 500


������Ҫ���Ʒ��ȣ���ֹ�𻵻�е�ṹ





*************************/
/********************************
�������� : ���1�Ŀ��ƺ�������е���ϲ����
������� : �Ƕ�
������� ����
*********************************/
void SERVO1_CONTRAL(uint8_t angle)
{
	float temp ;
	temp = 7.407 *angle + 500; 
	TIM_SetCompare2(TIM5,(uint16_t)temp);
}
/********************************
�������� : ���2�Ŀ��ƺ���
������� : �Ƕ�
������� ����
*********************************/
void SERVO2_CONTRAL(uint8_t angle)
{
	float temp ;
	temp = 7.407 *angle + 500; 
	TIM_SetCompare3(TIM5,(uint16_t)temp);
}
/********************************
�������� : ���3�Ŀ��ƺ���
������� : �Ƕ�
������� ����
*********************************/
void SERVO3_CONTRAL(uint8_t angle)
{
	float temp ;
	temp = 7.407 *angle + 500; 
	TIM_SetCompare4(TIM5,(uint16_t)temp);
}

/********************************
�������� : ���4�Ŀ��ƺ�������е���²����
������� : �Ƕ�
������� ����
*********************************/
void SERVO4_CONTRAL(uint8_t angle)
{
	float temp ;
	temp = 7.407 *angle + 500; 
	TIM_SetCompare4(TIM1,(uint16_t)temp);
}

