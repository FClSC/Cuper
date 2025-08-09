#include "colorreco.h"

uint8_t color_value[3] = {0, 0, 0}; // �洢��ɫֵ
int color = 0;

void I2C_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  // ���� GPIOC ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  // ���� GPIOD ʱ��


    GPIO_InitStruct.GPIO_Pin = I2C_SCL_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;          // ��©���
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;        // �ٶ�����
    GPIO_Init(I2C_SCL_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = I2C_SDA_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;          // ��©���
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;        // �ٶ�����
    GPIO_Init(I2C_SDA_PORT, &GPIO_InitStruct);

    I2C_SCL_HIGH();
    I2C_SDA_HIGH();
}



void I2C_Delay(void)
{
    for (volatile int i = 0; i < 100; i++);
}

void I2C_Start(void)
{
    I2C_SDA_HIGH();
    I2C_SCL_HIGH();
    I2C_Delay();
    I2C_SDA_LOW();  // SDA ���ͣ���ʼ�ź�
    I2C_Delay();
    I2C_SCL_LOW();  // SCL ����
}

void I2C_Stop(void)
{
    I2C_SDA_LOW();
    I2C_SCL_HIGH();
    I2C_Delay();
    I2C_SDA_HIGH();  // SDA ���ߣ�ֹͣ�ź�
    I2C_Delay();
}

void I2C_SendBit(uint8_t bit)
{
    if (bit)
        I2C_SDA_HIGH();
    else
        I2C_SDA_LOW();
    
    I2C_SCL_HIGH();
    I2C_Delay();
    I2C_SCL_LOW();
    I2C_Delay();
}

uint8_t I2C_ReadBit(void)
{
    uint8_t bit;
    I2C_SDA_HIGH();  // �ͷ� SDA �Ա��ȡ����
    I2C_SCL_HIGH();
    I2C_Delay();
    bit = I2C_SDA_READ();
    I2C_SCL_LOW();
    I2C_Delay();
    return bit;
}

void I2C_SendByte(uint8_t byte)
{
    for (int i = 0; i < 8; i++)
    {
        I2C_SendBit((byte & 0x80) != 0);  // �������λ
        byte <<= 1;
    }
    // ����Ӧ��λ
    I2C_ReadBit();
}

uint8_t I2C_ReadByte(uint8_t ack)
{
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++)
    {
        byte <<= 1;
        byte |= I2C_ReadBit();  // ��ȡÿһλ
    }
    // ����Ӧ��λ
    I2C_SendBit(ack ? 0 : 1);  // ���� ACK �� NACK
    return byte;
}



void I2C_Read_Sensor(uint8_t *recv_value)
{
    I2C_Start();  // ������ʼ�ź�
                              //                                Ĭ�� ����ñ����  ��дλ
    I2C_SendByte(0x4F << 1);  // ���ʹӻ���ַ��дģʽ�� �ӻ���ַΪ 1001 111        X      0x48Ϊ 0100 1111
    I2C_SendByte(0xD1);       // �������� 0xD1,��ȡHSL���ݵ�����ͨ��ֵ��0xD0�Ƕ�ȡRGB����
    I2C_Stop();               // ֹͣ�ź�

    I2C_Delay();  // �ȴ�һС��ʱ��

    I2C_Start();  // ���������������ģʽ
    I2C_SendByte((0x4F << 1) | 1);  // ���ʹӻ���ַ����ģʽ��

    // ��������
    recv_value[0] = I2C_ReadByte(1);  // ����1���ֽڣ����� ACK
    recv_value[1] = I2C_ReadByte(1);  // ����2���ֽڣ����� ACK
    recv_value[2] = I2C_ReadByte(0);  // ����3���ֽڣ����� NACK

    I2C_Stop();  // ֹͣ�ź�
}

#include <stdint.h>
#include <math.h>

/**
 * @brief ���������HSLֵ��0-240��Χ��������ӽ�����ɫ
 * @param hsl �����HSL���飬hsl[0]=H, hsl[1]=S, hsl[2]=L
 * @return 1���죩��2���̣���3��������4���ף���5���ڣ�
 */
int getClosestColor(uint8_t hsl[3]) 
{
    float h = hsl[0] / 240.0f * 360.0f;  // H��׼���� [0, 360)
    float s = hsl[1] / 240.0f;           // S��׼���� [0, 1]
    float l = hsl[2] / 240.0f;           // L��׼���� [0, 1]

    // 1. ����ɫ�������ȣ�
    if (l <= 0.1f) {
        return 5; // ��ɫ
    }

    // 2. ����ɫ�������� + �ͱ��Ͷȣ�
    if (l >= 0.9f && s <= 0.1f) {
        return 4; // ��ɫ
    }

    // 3. ���㵱ǰH��졢�̡�����ɫ���
    float hue = h;
    if (hue < 0) hue += 360;
    if (hue >= 360) hue -= 360;

    // ��׼��ɫ��Hֵ
    float red_hue = 0.0f;    // ��ɫH=0��
    float green_hue = 120.0f; // ��ɫH=120��
    float blue_hue = 240.0f;  // ��ɫH=240��

    // ���㵱ǰH��졢�̡�������Сɫ���
    float diff_red = fminf(fabsf(hue - red_hue), 360 - fabsf(hue - red_hue));
    float diff_green = fminf(fabsf(hue - green_hue), 360 - fabsf(hue - green_hue));
    float diff_blue = fminf(fabsf(hue - blue_hue), 360 - fabsf(hue - blue_hue));

    // �����ӽ��ı�׼��ɫƫ�� ��30�㣬��ֱ�ӷ���
    if (diff_red <= 30.0f) return 1;   // ��
    if (diff_green <= 30.0f) return 2; // ��
    if (diff_blue <= 30.0f) return 3;  // ��

    // 4. ������ͶȽϸߣ�ѡ����ӽ���RGB��ɫ
    if (s >= 0.5f) {
        if (diff_red <= diff_green && diff_red <= diff_blue) return 1; // ��
        if (diff_green <= diff_red && diff_green <= diff_blue) return 2; // ��
        return 3; // ��
    }

    // 5. �ͱ��Ͷ�������������ȷ��ذ׻��
    if (l >= 0.5f) {
        return 4; // ��
    } else {
        return 5; // ��
    }
}










