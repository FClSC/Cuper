#ifndef _CONTRAL_H
#define _CONTRAL_H

#include "system_init.h"
#include <stdio.h>
#include <math.h>


#define CW  0
#define CCW 1

#define T1_FREQ 1000000     //��ʱ��Ƶ��
#define FSPR    200         //���������Ȧ����
#define SPR     (FSPR*16)  //16ϸ�ֵĲ���
// ��ѧ������ ����MSD_Move�����ļ򻯼���
#define ALPHA (2*3.14159/SPR)                    // 2*pi/spr
#define A_T_x100 ((long)(ALPHA*T1_FREQ*100))     // (ALPHA / T1_FREQ)*100          A*f = c*w
#define T1_FREQ_148 ((int)((T1_FREQ*0.676)/100)) // divided by 100 and scaled by 0.676
#define A_SQ (long)(ALPHA*2*10000000000)         // 
#define A_x20000 (int)(ALPHA*20000)              // ALPHA*20000

//�ٶ�����״̬
#define STOP  0
#define ACCEL 1
#define DECEL 2
#define RUN   3

#define TRUE 1
#define FALSE 0

#define Pulse_width 100

#define Pulse_width_comper 50

extern int stepPosition;

extern int distance;
extern uint16_t angle_temp;

extern int distance1;

extern uint8_t servo_angle1;
extern uint8_t servo_angle2;
extern uint16_t servo_angle3;
extern uint8_t servo_angle4;

typedef struct {
  //�������״̬
  unsigned char run_state : 3;
  //������з���
  unsigned char dir : 1;
  //��һ��������ʱ���ڣ�����ʱΪ���ٶ�����
  unsigned int step_delay;
  //��ʼ���ٵ�λ��
  unsigned int decel_start;
  //���پ���
  signed int decel_val;
  //��С��ʱ��������ٶȣ�
  signed int min_delay;
  //���ٻ��߼��ټ�����
  signed int accel_count;
} speedRampData;

extern speedRampData srd;
extern speedRampData srd1;
extern speedRampData srd2;

extern int stepPosition ;
extern int stepPosition1 ;
extern int stepPosition2;

void contral_motor_Init(void);
void claw_Init(void);

void MSD_Move(signed int step, unsigned int accel, unsigned int decel, unsigned int speed);
void MSD_Move1(signed int step, unsigned int accel, unsigned int decel, unsigned int speed);
void MSD_Move2(signed int step, unsigned int accel, unsigned int decel, unsigned int speed);
void MSD_StepCounter(signed char inc);
void MSD_StepCounter1(signed char inc);
void MSD_StepCounter2(signed char inc);
void DIR1(uint8_t a);
void DIR2(uint8_t a);
void DIR3(uint8_t a);
void DIR4(uint8_t a);
void MOTOR_Direction(int16_t x,int16_t y);
uint16_t abs_int(int16_t x);
int16_t abs_float(float x,int8_t y);
uint16_t max_Return(int16_t x,int16_t y);
void MOTOR_TURN(int16_t angle);
void MOTOR_Displacement(int16_t x_cm,int16_t y_cm);
void MOTOR_Displacement_fm(int16_t x_fm,int16_t y_fm);
float adjust_float(float x, int16_t y);
void MOTOR_Angle(int8_t angle);
void MOTOR_Angle_micro(float angle1);
void MOTOR_TurnRight(int angle);
void MOTOR_Align(void);
void MOTOR_Displacement_mm(int16_t x_mm,int16_t y_mm);

void Motor1_DIR(uint8_t a);

void uart_handle(void);

void claw_up_start(void);
void claw_up(void);
void claw_up2(void);
void claw_down(void);
void claw_down2(void);
void Servo_SetAngle14(int16_t angle1,int16_t angle4);
void Servo_default(void);
void Servo_Stretch(void);
void claw_open(void);
void claw_open1(void);
void claw_close(void);
void claw_close2(void);
void claw_turn0(void);
void claw_turn129(void);
void support_turn120(void);

void claw_turn0(void);
void claw_turn1(void);
void claw_turn2(void);
void claw_turn3(void);
void claw_turn4(void);
void claw_turn5(void);

void claw_position(int16_t position);

#endif

