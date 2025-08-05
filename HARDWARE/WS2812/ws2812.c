#include "ws2812.h"
/*
WS2812

PWM+DMA

PWM: TIM1_CH4  PA11

ʱ������72 MHz=7200 0000 HZ
��Ƶϵ�� 1
��Ƶ��Ƶ�� = 7200 0000/1=7200 0000 HZ
��Ƶ��һ�μ�����ʱ�� 1 / 7200 0000 s
��90������ 1.25us
�� PSC=1 ARR=90

0��
�ߵ�ƽ+�͵�ƽ=0.125us
����CCR=25
�ߵ�ƽʱ��0.347us
�͵�ƽʱ��0.903us



1��ʱ��
�ߵ�ƽ+�͵�ƽ=0.125us
����CCR=55
�ߵ�ƽʱ��0.76us
�͵�ƽʱ��0.49us


*/


uint16_t ws2812_buffer[WS2812_BUFFER_SIZE]; // DMA���仺����

// ��ʼ������������ԭ������ȷ����TIM1��DMA��
void WS2812_Init(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period = 89;        // 800kHz (72MHz/(89+1))
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Disable);

    DMA_DeInit(DMA1_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM1->CCR4;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ws2812_buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = WS2812_BUFFER_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);

    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_DMACmd(TIM1, TIM_DMA_CC4, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    GPIO_ResetBits(GPIOA, GPIO_Pin_11);
    delay_us(100);
}

// ���õ���LED��ɫ��GRB��ʽ��
void WS2812_SetColor(uint8_t led_num, uint32_t grb_color) 
{
    uint16_t *p = &ws2812_buffer[led_num * 24];
    for (int i = 0; i < 24; i++) {
        p[i] = (grb_color & (1 << (23 - i))) ? WS2812_1 : WS2812_0;
    }
}

// ����DMA���䲢���ɸ�λ�ź�
void WS2812_Update(void)
{
    TIM_Cmd(TIM1, DISABLE);
    DMA_Cmd(DMA1_Channel4, DISABLE);

    DMA_SetCurrDataCounter(DMA1_Channel4, WS2812_BUFFER_SIZE);
    DMA_Cmd(DMA1_Channel4, ENABLE);
    TIM_Cmd(TIM1, ENABLE);

    // �ȴ�DMA�������
    while (!DMA_GetFlagStatus(DMA1_FLAG_TC4));
    DMA_ClearFlag(DMA1_FLAG_TC4);

    // �ؼ��޸������׹ر� PWM �� DMA�������� GPIO
    TIM_Cmd(TIM1, DISABLE);
    DMA_Cmd(DMA1_Channel4, DISABLE);
    GPIO_ResetBits(GPIOA, GPIO_Pin_11); // ǿ������������
    delay_us(100);  // �ӳ���λʱ���� 100��s��ȷ���ɿ���
}

//0xFFFFFF ȫ��
// 0xFF0000 ��ɫ
// 0x00FF00 ��ɫ
// 0x0000FF ��ɫ
// 0x7F7F7F ���
// ʾ������������LEDΪ��ɫ
void ws2812_ON(void) 
{
    for (int i = 0; i < WS2812_NUM_LEDS; i++) {
        WS2812_SetColor(i, 0x7F7F7F); // GRB��ʽ����ɫ��
    }
    WS2812_Update();
    delay_ms(10);
}
// �ر����� LED����ƣ�
void ws2812_OFF(void) 
{
    for (int i = 0; i < WS2812_NUM_LEDS; i++) {
        WS2812_SetColor(i, 0x000000); // GRB ȫ 0���ر� LED
    }
    WS2812_Update(); // �������ݵ��ƴ�
    delay_ms(10);    // ��ѡ��ȷ�����ݷ������
}


// DMA��������жϷ���������ѡ��
void DMA1_Channel4_IRQHandler(void) 
{
    if (DMA_GetITStatus(DMA1_IT_TC4)) {
        DMA_ClearITPendingBit(DMA1_IT_TC4);
        // ���ڴ���Ӹ�λ�źŴ���
    }
}
