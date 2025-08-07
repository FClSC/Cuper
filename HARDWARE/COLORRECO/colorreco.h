#ifndef _COLORRECO_H
#define _COLORRECO_H
#include "stm32f10x.h"                  // Device header
#include "delay.h"

#define I2C_SCL_PIN  GPIO_Pin_12  // SCL ����  PC12
#define I2C_SDA_PIN  GPIO_Pin_2  // SDA ����   PD2
#define I2C_SCL_PORT GPIOC       // ʹ�õ� GPIO �˿�
#define I2C_SDA_PORT GPIOD       // ʹ�õ� GPIO �˿�

#define I2C_SCL_HIGH() GPIO_SetBits(I2C_SCL_PORT, I2C_SCL_PIN)
#define I2C_SCL_LOW()  GPIO_ResetBits(I2C_SCL_PORT, I2C_SCL_PIN)
#define I2C_SDA_HIGH() GPIO_SetBits(I2C_SDA_PORT, I2C_SDA_PIN)
#define I2C_SDA_LOW()  GPIO_ResetBits(I2C_SDA_PORT, I2C_SDA_PIN)
#define I2C_SDA_READ() GPIO_ReadInputDataBit(I2C_SDA_PORT, I2C_SDA_PIN)

extern uint8_t color_value[3];

void I2C_GPIO_Init(void);
void I2C_Delay(void);
void I2C_Stop(void);
void I2C_Start(void);
void I2C_SendBit(uint8_t bit);
uint8_t I2C_ReadBit(void);
void I2C_SendByte(uint8_t byte);
uint8_t I2C_ReadByte(uint8_t ack);
void I2C_Read_Sensor(uint8_t *recv_value);

int getClosestColor(uint8_t hsl[3]);



#endif
