#ifndef _CONTRAL_CLAW_H
#define _CONTRAL_CLAW_H

#include "system_init.h"
#include "contral.h"

//ͨ�ø߶�
/*��������ת��ץȡʱ���80-100mm
ת��  ���5.5cm��צ�Ӹ߶�
80mm  13.5cm 99

85mm 14.0cm 105

90mm 14.5cm  111

95mm 15.0cm 117

100mm 15.5cm 123

���㹫ʽΪ��צ������߶�=99+(ת��ʵ�ʸ߶�-80)/20*24

*/
#define claw_most_up    250   //            ������ߵ�λ��            
#define camera_position  0       //               ɨ���λ��
#define claw_most_down  0       //          ���͵���͵�λ��
#define circle_capture1  80  //          һ����ð���ʶ��ĸ߶�
#define circle_capture3  25   //     ����������ʶ��ĸ߶�   160 //
#define circle_capture2  114    //��������ɫʶ��λ�ã�Ҫ���ڵȻ�ץȡ���ĸ߶� //

//��ͨ�������
#define claw_block_get    5      //      ����ץ���ĸ߶�  4

#define put_block_down   6       //      ���Ϸ����ĸ߶�  4

extern int16_t claw_block_get1 ;    //     ������ת����ץ���ĸ߶�    ��Ҫ���и��ĵģ���Ϊ������������������޸�  //

#define claw_block_put   168 //        ���Ϸ�����λ��

#define get_block_down    168   //       �ӳ���ץȡҪ��������ȥ��λ��

#define put_block_down2   87   //     ���������ڶ���ĸ߶�   //

#define claw_block_putF2  165    //���ϰ����ŵ�ת�̶���ĸ߶�   //

//�������
#define claw_block2_get    4      //      ����ץ���2�ĸ߶�  28

#define put_block2_down   4       //      ���Ϸ����2�ĸ߶�28

#define claw_block2_get1  103    //     ������ת����ץ���2�ĸ߶�

#define claw_block2_put   135 //        ���Ϸ����2��λ��

#define get_block2_down    132   //       �ӳ���ץȡҪ�����2����ȥ��λ��

#define put_block2_down2   87   //     �����2�����ڶ���ĸ߶�


typedef struct {
	int16_t position_now;             //��ǰֵ
	int16_t position_target;           //Ŀ��ֵ
	int16_t position_temp;             //��ʱֵ���м��ֵ��
}CLAW_POSITION;

extern CLAW_POSITION claw;





void claw_position(int16_t position);
void claw_position2(int16_t position);

void arrive_camera(void);
void arrive_most_up(void);
void arrive_most_down(void);
void arrive_circle_capture(void);
void arrive_circle_capture2(void);
void arrive_color_reco(void);

void arrive_block_get(void);
void arrive_block_put(void);
void arrive_block_get1(void);
void arrive_car_put(void);
void arrive_car_get(void);
void arrive_put_down2(void);
void arrive_block_putF2(void);

void arrive_block2_get(void);
void arrive_block2_put(void);
void arrive_block2_get1(void);
void arrive_car2_put(void);
void arrive_car2_get(void);
void arrive_put2_down2(void);



void claw_get_block1(void);
void claw_get_block2(void);
void claw_get_block3(void);
void claw_get_block4(void);
void claw_get_block5(void);

void claw_put_block1(void);
void claw_put_block2(void);
void claw_put_block3(void);	
void claw_put_block4(void);
void claw_put_block5(void);





void claw_home(void);



#endif
