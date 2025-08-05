#include "contral.h"
#include <stdio.h>
#include <math.h>
#include "delay.h"

speedRampData srd;
speedRampData srd1;
speedRampData srd2;

uint8_t servo_angle1;
uint8_t servo_angle2;
uint16_t servo_angle3;

extern int16_t base_angle; 
extern int16_t code1 ;
extern int16_t code2 ; 	



int stepPosition = 0;
int stepPosition1 = 0;
int stepPosition2 = 0;

int distance=0;
uint16_t angle_temp=0;

int distance1=0;

// ��ʱ��3
/*******************************************
�������� : ����С�����̵ĳ�ʼ��
������� : ��
������� ����
********************************************/
void contral_motor_Init(void)
{
	MOTOR3_Init();
	MOTOR4_Init();
	MOTOR7_Init();
	MOTOR8_Init();
}
// ��ʱ��2
/*******************************************
�������� : ����צ��ץȡ�ĳ�ʼ��
������� : ��
������� ����
********************************************/
void claw_Init(void)
{
	MOTOR1_Init();//����
	//MOTOR5_Init();//ת��
	Servo2_Init();
	Servo3_Init();
	claw_turn0();	
	claw_open();
	claw.position_now=0;  
	claw.position_target=0;
	claw.position_temp=0;
}
/*************************
��ʱ��3 �Ŀ���
���ƶ��� С������
С������ �� ����С�������˶�
������� �� ���� ���ٶ� ���ٶ� �ٶ�
������� �� ��
**************************/
void MSD_Move(signed int step, unsigned int accel, unsigned int decel, unsigned int speed)
{
    uint16_t max_s_lim;
    uint16_t accel_lim; 
    if(step > 0)//��ʱ��
    {
        srd.dir = CCW;
    }
    if(step == 1)
    {
        srd.accel_count = -1;
        srd.run_state = DECEL;
        srd.step_delay = 1000;
        TIM_SetAutoreload(TIM3,Pulse_width);
        TIM_SetCompare1(TIM3,Pulse_width_comper); 
		TIM_SetCompare2(TIM3,Pulse_width_comper); 					
        TIM_SetCompare3(TIM3,Pulse_width_comper); 
		TIM_SetCompare4(TIM3,Pulse_width_comper); 
        TIM_Cmd(TIM3, ENABLE); 
     }
    else if(step != 0)
    {
			accel = accel*100; 
			decel = decel*100;
			speed = speed*100;
			srd.min_delay = A_T_x100 / speed;
			srd.step_delay = (T1_FREQ_148 * sqrt(A_SQ / accel))/100;
			// ������ٲ�֮��ﵽ����ٶȵ�����
			max_s_lim = (long)speed*speed/(long)(((long)A_x20000*accel)/100);
			// ����ﵽ����ٶ�С��0.5�������ǽ���������Ϊ0
			// ��ʵ�����Ǳ����ƶ�����һ�����ܴﵽ��Ҫ���ٶ�
			if(max_s_lim == 0)
			{
					max_s_lim = 1;
			}
			// ������ٲ�֮�����Ǳ��뿪ʼ����
			accel_lim = ((long)step*decel) / (accel+decel);
			// ���Ǳ����������1�����ܲ��ܿ�ʼ����.
			if(accel_lim == 0)
			{
					accel_lim = 1;
			}
			// ʹ�������������ǿ��Լ������һ�ο�ʼ���ٵ�λ��
			if(accel_lim <= max_s_lim)
			{
					srd.decel_val = accel_lim - step;
			}
			else
			{
					srd.decel_val = -(long)(max_s_lim*accel/decel);
			}
			// ��ֻʣ��һ�����Ǳ������
			if(srd.decel_val == 0)
			{
					srd.decel_val = -1;
			}
			// ���㿪ʼ����ʱ�Ĳ���
			srd.decel_start = step + srd.decel_val;
			// �������ٶȺ��������ǾͲ���Ҫ���м����˶�
			if(srd.step_delay <= srd.min_delay)
			{
					srd.step_delay = srd.min_delay;
					srd.run_state = RUN;
			}
			else
		  {
					srd.run_state = ACCEL;
			} 
			// ��λ���ٶȼ���ֵ
			srd.accel_count = 0;
			//���ö�ʱ����װֵ	
			TIM_SetAutoreload(TIM3,Pulse_width);
			TIM_SetCompare1(TIM3,Pulse_width_comper); 
			TIM_SetCompare2(TIM3,Pulse_width_comper); 					
			TIM_SetCompare3(TIM3,Pulse_width_comper); 
			TIM_SetCompare4(TIM3,Pulse_width_comper); 		
			//ʹ�ܶ�ʱ��	      
			TIM_Cmd(TIM3, ENABLE); 
    }
}
/*************************
��ʱ��2 �Ŀ���
���ƶ��� ץȡ����
���� �� ����ץȡ�������������½�
������� �� ���� ���ٶ� ���ٶ� �ٶ�
������� �� ��
**************************/
void MSD_Move1(signed int step, unsigned int accel, unsigned int decel, unsigned int speed)
{
    uint16_t max_s_lim;
    uint16_t accel_lim; 
    if(step > 0)//��ʱ��
    {
        srd1.dir = CCW;
    }
    if(step == 1)
    {
        srd1.accel_count = -1;
        srd1.run_state = DECEL;
        srd1.step_delay = 1000;
        TIM_SetAutoreload(TIM2,Pulse_width);
        TIM_SetCompare3(TIM2,Pulse_width_comper); 
				TIM_SetCompare4(TIM2,Pulse_width_comper); 
        TIM_Cmd(TIM2, ENABLE); 
     }
    else if(step != 0)
    {
				accel = accel*100; 
				decel = decel*100;
				speed = speed*100;
			srd1.min_delay = A_T_x100 / speed;
			srd1.step_delay = (T1_FREQ_148 * sqrt(A_SQ / accel))/100;
			// ������ٲ�֮��ﵽ����ٶȵ�����
			max_s_lim = (long)speed*speed/(long)(((long)A_x20000*accel)/100);
			// ����ﵽ����ٶ�С��0.5�������ǽ���������Ϊ0
			// ��ʵ�����Ǳ����ƶ�����һ�����ܴﵽ��Ҫ���ٶ�
			if(max_s_lim == 0)
			{
					max_s_lim = 1;
			}
			// ������ٲ�֮�����Ǳ��뿪ʼ����
			accel_lim = ((long)step*decel) / (accel+decel);
			// ���Ǳ����������1�����ܲ��ܿ�ʼ����.
			if(accel_lim == 0)
			{
					accel_lim = 1;
			}
			// ʹ�������������ǿ��Լ������һ�ο�ʼ���ٵ�λ��
			if(accel_lim <= max_s_lim)
			{
					srd1.decel_val = accel_lim - step;
			}
			else
			{
					srd1.decel_val = -(long)(max_s_lim*accel/decel);
			}
			// ��ֻʣ��һ�����Ǳ������
			if(srd1.decel_val == 0)
			{
					srd1.decel_val = -1;
			}
			// ���㿪ʼ����ʱ�Ĳ���
			srd1.decel_start = step + srd1.decel_val;
			// �������ٶȺ��������ǾͲ���Ҫ���м����˶�
			if(srd1.step_delay <= srd1.min_delay)
			{
					srd1.step_delay = srd1.min_delay;
					srd1.run_state = RUN;
			}
			else
		  {
					srd1.run_state = ACCEL;
			} 
			// ��λ���ٶȼ���ֵ
			srd1.accel_count = 0;
			//���ö�ʱ����װֵ	
			TIM_SetAutoreload(TIM2,Pulse_width);
			//����ռ�ձ�Ϊ50%	
			TIM_SetCompare3(TIM2,Pulse_width_comper);
			TIM_SetCompare4(TIM2,Pulse_width_comper);			
			//ʹ�ܶ�ʱ��	      
			TIM_Cmd(TIM2, ENABLE); 
    }
}
/*************************
��ʱ��4 �Ŀ���
���ƶ��� ת�̻���
���� �� ����ת�̵���ת
������� �� ���� ���ٶ� ���ٶ� �ٶ�
������� �� ��
**************************/
void MSD_Move2(signed int step, unsigned int accel, unsigned int decel, unsigned int speed)
{
    uint16_t max_s_lim;
    uint16_t accel_lim; 
    if(step > 0)//��ʱ��
    {
        srd2.dir = CCW;
    }
    if(step == 1)
    {
        srd2.accel_count = -1;
        srd2.run_state = DECEL;
        srd2.step_delay = 1000;
        TIM_SetAutoreload(TIM4,Pulse_width);
        TIM_SetCompare3(TIM4,Pulse_width_comper); 
		TIM_SetCompare4(TIM4,Pulse_width_comper); //��һ��û��Ҫ,2024/12/10
        TIM_Cmd(TIM4, ENABLE); 
     }
    else if(step != 0)
    {
				accel = accel*100; 
				decel = decel*100;
				speed = speed*100;
			srd2.min_delay = A_T_x100 / speed;
			srd2.step_delay = (T1_FREQ_148 * sqrt(A_SQ / accel))/100;
			// ������ٲ�֮��ﵽ����ٶȵ�����
			max_s_lim = (long)speed*speed/(long)(((long)A_x20000*accel)/100);
			// ����ﵽ����ٶ�С��0.5�������ǽ���������Ϊ0
			// ��ʵ�����Ǳ����ƶ�����һ�����ܴﵽ��Ҫ���ٶ�
			if(max_s_lim == 0)
			{
					max_s_lim = 1;
			}
			// ������ٲ�֮�����Ǳ��뿪ʼ����
			accel_lim = ((long)step*decel) / (accel+decel);
			// ���Ǳ����������1�����ܲ��ܿ�ʼ����.
			if(accel_lim == 0)
			{
					accel_lim = 1;
			}
			// ʹ�������������ǿ��Լ������һ�ο�ʼ���ٵ�λ��
			if(accel_lim <= max_s_lim)
			{
					srd2.decel_val = accel_lim - step;
			}
			else
			{
					srd2.decel_val = -(long)(max_s_lim*accel/decel);
			}
			// ��ֻʣ��һ�����Ǳ������
			if(srd2.decel_val == 0)
			{
					srd2.decel_val = -1;
			}
			// ���㿪ʼ����ʱ�Ĳ���
			srd2.decel_start = step + srd2.decel_val;
			// �������ٶȺ��������ǾͲ���Ҫ���м����˶�
			if(srd2.step_delay <= srd2.min_delay)
			{
					srd2.step_delay = srd2.min_delay;
					srd2.run_state = RUN;
			}
			else
		  {
					srd2.run_state = ACCEL;
			} 
			// ��λ���ٶȼ���ֵ
			srd2.accel_count = 0;
			//���ö�ʱ����װֵ	
			TIM_SetAutoreload(TIM4,Pulse_width);
			//����ռ�ձ�Ϊ50%	
			TIM_SetCompare3(TIM4,Pulse_width_comper);
			TIM_SetCompare4(TIM4,Pulse_width_comper);			
			//ʹ�ܶ�ʱ��	      
			TIM_Cmd(TIM4, ENABLE); 
    }
}

/*************************
��ʱ��3 �Ŀ���
���ƶ��� С������
���� �� �ۼƵ��̵�������
������� �� ����
������� �� ��
**************************/
void MSD_StepCounter(signed char inc)
{
  //���ݷ����жϵ��λ��
  if(inc == CCW)
  {
    stepPosition++;
  }
  else
  {
    stepPosition--;
  }
}
/*************************
��ʱ��2 �Ŀ���
���ƶ��� ץȡ����
���� �� ����ץȡ�����Ĳ���
������� �� ����
������� �� ��
**************************/
void MSD_StepCounter1(signed char inc)
{
  //���ݷ����жϵ��λ��
  if(inc == CCW)
  {
    stepPosition1++;
  }
  else
  {
    stepPosition1--;
  }
}
/*************************
��ʱ��4 �Ŀ���
���ƶ��� ת�̻���
���� �� ����ת�̵Ĳ���
������� �� ����
������� �� ��
**************************/
void MSD_StepCounter2(signed char inc)
{
  //���ݷ����жϵ��λ��
  if(inc == CCW)
  {
    stepPosition2++;
  }
  else
  {
    stepPosition2--;
  }
}



/*************************
��ʱ��2 �Ŀ���
���� �� ���Ƶ��1�ķ���
������� �� ����
������� �� ��
**************************/
void Motor1_DIR(uint8_t a)
{
	if(a == CW)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	}
	else
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
	}
}
/*******************************************
�������� : ����С�����̵��ƶ�����
������� : ����
������� ����
********************************************/
// PG7    dir3
void DIR1(uint8_t a)
{
	if(a == CW)
	{
		GPIO_SetBits(GPIOG,GPIO_Pin_7);
	}
	else
	{
		GPIO_ResetBits(GPIOG,GPIO_Pin_7);
	}
}
//PG8    dir4
void DIR2(uint8_t a)
{
	if(a == CW)
	{
		GPIO_ResetBits(GPIOG,GPIO_Pin_8);
	}
	else
	{
		GPIO_SetBits(GPIOG,GPIO_Pin_8);
	}
}
// PA8    dir7
void DIR3(uint8_t a)  //�Ķ�������Ϊ���������򷴵�
{
	if(a == CW)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	}
	else
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
	}
}
// PC1    dir8
void DIR4(uint8_t a)
{
	if(a == CW)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_1);
	}
	else
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	}
}
/********************
�������� : ����С���ƶ�����ĺ���
������� : x��λ�ƣ�y��λ��
������� ����
**********************/
void MOTOR_Direction(int16_t x,int16_t y)
{
	// x��ֵ
	if(x<0) {DIR1(0); DIR2(0);DIR3(0); DIR4(0);}
	else if(x>0)  {DIR1(1); DIR2(1);DIR3(1); DIR4(1);}
	// y��ֵ	
	if(y>0) {DIR1(0); DIR2(1);DIR3(0); DIR4(1);}
	else if(y<0) {DIR1(1); DIR2(0);DIR3(1); DIR4(0);}
}
/********************
�������� : ����ֵ����
������� : ����ֵ
������� ����
**********************/
uint16_t abs_int(int16_t x)
{
	if(x>0) return x;
	else return -x;
}

/********************
�������� : ����ֵ����
������� : ����ֵ
������� ������
**********************/
int16_t abs_float(float x,int8_t y)
{
    int16_t z;
	if(x>y)
	{
		z=round(x-y);
		return z;
	}
	else
	{
		z=round(y-x);
		return z;
	}
}

/********************
�������� : �õ�x��y��������λ�Ƶĺ���
������� : x��λ�ƣ�y��λ��
������� ����
**********************/
uint16_t max_Return(int16_t x,int16_t y)
{
	uint16_t x1,y1;
	x1 = abs_int(x);
	y1 = abs_int(y);
	if(x1>y1) return x1;
	if(y1>x1) return y1;
	return 0;
}
/********************
�������� : ����С����ת����ĺ���
������� : ��ת�Ƕ�
������� ����
**********************/
void MOTOR_TURN(int16_t angle)
{
	if(angle>0)  {DIR1(0); DIR2(1);DIR3(1); DIR4(0);}
	else if(angle<0) {DIR1(1); DIR2(0);DIR3(0); DIR4(1);}
}
/********************
�������� : ����С��λ�ƺ���
������� : x�����cm��y�����cm����
������� ����
**********************/
void MOTOR_Displacement(int16_t x_cm,int16_t y_cm)
{
	int temp=0;
	// ���жϷ���
	MOTOR_Direction(x_cm,y_cm);
	// �˶�����
	temp=max_Return(x_cm,y_cm);
	//     3200        x/a = 78/3200     
	distance=temp*130;  //�������cm 
	MSD_Move(distance,25,25,45);  //17 17 40 
}


/********************
�������� : ����С����ת�Ƕȣ�΢����
������� : ΢���ĽǶ�
������� ����
**********************/
float adjust_float(float x, int16_t y)
{
    float z;
	z=y-x;
	return z;
}



/********************
�������� : ����С���ǶȺ���
������� : ��ʱ����ת�ĽǶ�
������� ����
**********************/
void MOTOR_Angle(int8_t angle)
{
	MOTOR_TURN(angle);
	angle_temp = abs_int(angle*50);
	MSD_Move(angle_temp,15,15,30);   //������ 14 14 25
}

/********************
�������� : ����С��΢���ǶȺ���
������� : ��ʱ����ת�ĽǶ�
������� ����
**********************/
void MOTOR_Angle_micro(float angle1)
{
	int16_t angle2;
	angle2=angle1*10;

	MOTOR_TURN(angle2);  //��ת����Ĵ���
	angle_temp = abs_int(angle2*5);   //ע������ط���*5���Ƕȱ�ø���ϸ�������ܵ���*4
	MSD_Move(angle_temp,10,10,20);   //�����ܣ� 8 8 24
}


/********************
�������� : �ڳ�����ת���������У׼
������� : ������ת�ĽǶ�
������� ����
**********************/
void MOTOR_TurnRight(int angle)  
{

	OLED_ShowFloatNum(0,0,global_angle,3,1,OLED_8X16);
	OLED_Update();
	//delay_ms(5000);
	if(determicro(angle,global_angle)==1)
	{

		stepPosition=0;
		MOTOR_Angle_micro(adjust_float(global_angle,angle));
		while(1)
		{
			if(stepPosition == angle_temp)
			{
				break;
			}
		}
	}
	//delay_ms(3000);
	OLED_ShowFloatNum(0,0,global_angle,3,1,OLED_8X16);
	OLED_Update();
	if(determicro(angle,global_angle)==1)
	{

		stepPosition=0;
		MOTOR_Angle_micro(adjust_float(global_angle,angle));
		while(1)
		{
			if(stepPosition == angle_temp)
			{
				break;
			}
		}
	}
	OLED_ShowFloatNum(0,0,global_angle,3,1,OLED_8X16);
	OLED_Update();
	//delay_ms(2000);

	
	ResetAng_Z(); //����Z�������ǣ�������ã���ֹδ����
	delay_ms(100);
	ResetAng_Z(); //����Z��������
	delay_ms(100);
	OLED_ShowFloatNum(0,0,global_angle,3,3,OLED_8X16);
	OLED_Update();

}


/********************
�������� : ����ʶ��/ת��ǰ�������̬����(��Ӧ��׼�Ƕ���ȫ�ֽǶȵĲ�ֵ)
������� : ��
������� �����������������㣬���ν��������ȿɴ�0.1������
**********************/
void MOTOR_Align(void)
{
    
//180/-180���
    if(base_angle==180||base_angle==-180)//(global_angle>150&&global_angle<180)||(global_angle<-150&&global_angle>-180)
   	{
        
		if(global_angle>0)  //ת���ˣ�Ҫ˳ʱ����ת��ƫ
		{
			stepPosition=0;
			MOTOR_Angle_micro(fabs(fabs(global_angle)-fabs(base_angle)));
			while(1)
			{
				if(stepPosition == angle_temp)
				{
					break;
				}
			}
		}

		else if(global_angle<0)  //ת���ˣ�Ҫ��ʱ����ת��ƫ
		{
			stepPosition=0;
			MOTOR_Angle_micro(-fabs(fabs(global_angle)-fabs(base_angle)));
			while(1)
			{
				if(stepPosition == angle_temp)
				{
					break;
				}
			}
		}
        
        delay_ms(100);


		 if(global_angle>0)  //ת���ˣ�Ҫ˳ʱ����ת��ƫ
		 {
			 stepPosition=0;
			 MOTOR_Angle_micro(fabs(fabs(global_angle)-fabs(base_angle)));
			 while(1)
			 {
				 if(stepPosition == angle_temp)
				 {
					 break;
				 }
			 }
		 }
 
		 else if(global_angle<0)  //ת���ˣ�Ҫ��ʱ����ת��ƫ
		 {
			 stepPosition=0;
			 MOTOR_Angle_micro(-fabs(fabs(global_angle)-fabs(base_angle)));
			 while(1)
			 {
				 if(stepPosition == angle_temp)
				 {
					 break;
				 }
			 }
		 }
		 delay_ms(100);
		 if(global_angle>0)  //ת���ˣ�Ҫ˳ʱ����ת��ƫ
		 {
			 stepPosition=0;
			 MOTOR_Angle_micro(fabs(fabs(global_angle)-fabs(base_angle)));
			 while(1)
			 {
				 if(stepPosition == angle_temp)
				 {
					 break;
				 }
			 }
		 }
 
		 else if(global_angle<0)  //ת���ˣ�Ҫ��ʱ����ת��ƫ
		 {
			 stepPosition=0;
			 MOTOR_Angle_micro(-fabs(fabs(global_angle)-fabs(base_angle)));
			 while(1)
			 {
				 if(stepPosition == angle_temp)
				 {
					 break;
				 }
			 }
		 }


	}

//��180/-180���
else{

	if(determicro(base_angle,global_angle)==1)
	{

		stepPosition=0;
		MOTOR_Angle_micro(adjust_float(global_angle,base_angle));
		while(1)
		{
			if(stepPosition == angle_temp)
			{
				break;
			}
		}
	}
	delay_ms(100);

	if(determicro(base_angle,global_angle)==1)
	{
		stepPosition=0;
		MOTOR_Angle_micro(adjust_float(global_angle,base_angle));
		while(1)
		{
			if(stepPosition == angle_temp)
			{
				break;
			}
		}
	}
	delay_ms(100);
	if(determicro(base_angle,global_angle)==1)
	{
		stepPosition=0;
		MOTOR_Angle_micro(adjust_float(global_angle,base_angle));
		while(1)
		{
			if(stepPosition == angle_temp)
			{
				break;
			}
		}
	}
}


}
/********************
�������� : ����С��λ�ƺ���
������� : x�����mm��y�����mm����
������� ����
**********************/
void MOTOR_Displacement_mm(int16_t x_mm,int16_t y_mm)
{
	int temp=0;
	distance=0;
	// ���жϷ���
	MOTOR_Direction(x_mm,y_mm);
	// �˶�����
	temp=max_Return(x_mm,y_mm);
	//     3200        x/a = 78/3200 
	distance=temp*13;  //�������mm 
	MSD_Move(distance,12,12,20); //�ĳɺ͹���һ�����ٶ�
}

/********************
�������� : ����С��λ�ƺ���
������� : x�����0.1mm��y�����0.1mm����
������� ����
**********************/
void MOTOR_Displacement_fm(int16_t x_fm,int16_t y_fm)
{
	int temp=0;
	distance=0;
	// ���жϷ���
	MOTOR_Direction(x_fm,y_fm);
	// �˶�����
	temp=max_Return(x_fm,y_fm);
	//     3200        x/a = 78/3200 
	distance=(int)(temp*1.3);  //�������0.1mm,���Զ����1fm
	MSD_Move(distance,5,5,10);
	 
}

/********************
�������� : ������λ�����͵���Ϣ
������� : ��
������� ����
**********************/
void uart_handle(void)
{
	uint8_t mode=0x00;
	mode = Serial_RXPacket[0];   //����ģʽ
	uint8_t x_dis1 = 0;  //�������ֳ���λ���д���,x_dis1��x_dis2
	uint8_t x_dis2 = 0;
	uint16_t temp_dis = 0; //ת���м����
	uint8_t y_dis1 = 0;	
	uint8_t y_dis2 = 0;
    int16_t x_dis  = 0;
	int16_t y_dis  = 0;
	int8_t x_mdis = 0;
	int8_t y_mdis = 0;
	int8_t angle = 0;		
	uint8_t position_camera = 0;
	uint8_t claw_mode=0;
	int8_t move_mode=0;

	
/*�յ���ݮ�ɷ��͵����ݰ��ĸ�ʽ��
0xFF 
0[ģʽ] 
1[x��yλ�Ƶĸ߰�λ]  2[x��yλ�ƵĵͰ�λ]     
3[��ת�Ƕ�] 4[��ά��ǰ��λ�߰�λ] 5[��ά��ǰ��λ�Ͱ�λ]
6[��ά�����λ�߰�λ] 7[��ά�����λ�Ͱ�λ]
0xFE
                                                     */
	switch(mode)
	{
		case 0x01:  //xλ��
		{
			x_dis1 = Serial_RXPacket[1];
			x_dis2 = Serial_RXPacket[2];
			temp_dis =  (x_dis1 << 8) | x_dis2 ;
			x_dis  = (int16_t)temp_dis;
			stepPosition=0;
			MOTOR_Displacement(x_dis,0);
			while(1)
			{
				if(stepPosition == distance)
				{
					break;
				}
			}
			break;
		}
		case 0x02:  //yλ��
		{
            y_dis1 = Serial_RXPacket[1];
			y_dis2 = Serial_RXPacket[2];
			temp_dis =  (y_dis1 << 8) | y_dis2 ;
			y_dis  = (int16_t)temp_dis;
			stepPosition=0;			
			MOTOR_Displacement(0,y_dis);
			while(1)
			{
				if(stepPosition == distance)
				{
					break;
				}
			}
			break;
		}		
		case 0x03:  //�Ƕ�
		{
			angle = Serial_RXPacket[3];
			stepPosition=0;		
			MOTOR_Angle(angle);
			while(1)
			{
				if(stepPosition == angle_temp)
				{
					break;
				}
			}	


			if(angle>0)base_angle+=90; 
			else base_angle-=90;

			if(base_angle>180)//270
			{
				base_angle=-90;
			}
			else if(base_angle<-180)//-270
			{
				base_angle=90; 
			}

            delay_ms(100);

			//������΢��������������Z�������ǣ�ʹ�û�׼�Ƕȸ�����λ�����õĽǶȱ仯��΢����Ϊ����ת��
			//����λ������Motor_Align()������ʵ��
			//MOTOR_Align();

			break;
		}			
		case 0x04:  //�õ�Ƭ��ɨ��
		{
			UART5_Start_Scan();//����ɨ��ָ��
			// while(1)//���û�н��յ����ݾ�һֱ�ȴ�
			// {
			// 	if(Serial5_GetRxFlag() == 1)//���յ������ݾʹ���
			// 	{
			// 		int16_t code1 =0; 
			// 		int16_t code2 =0; 	
			// 		delay_ms(300);//�ȴ����ݽ�����ȫ
			// 		UART5_ParseCode(UART5_RX_BUF,&code1,&code2);//��������ά�����ݣ���ʱUART5_BUX����Ȼ��ŵ��Ƕ�ά������	
			// 		u2_printf("tt3.txt=\"%d+%d\"",code1,code2);//��η��͸�������
			// 		delay_ms(10);
			// 		u2_printf("tt3.txt=\"%d+%d\"",code1,code2);
			// 		delay_ms(10);
			// 		u2_printf("t3.txt=\"%d+%d\"",code1,code2);
			// 		delay_ms(10);
			// 		u2_printf("t3.txt=\"%d+%d\"",code1,code2);
			// 		delay_ms(10);
			// 		u2_printf("tt3.txt=\"%d+%d\"",code1,code2);
			// 		delay_ms(10);			
			// 		break;
			// 	}
			// }


			break;
		}	

		case 0x28:  //������λ����������ɨ����Ϣ������ͷ��
		{
			code1 = (Serial_RXPacket[4]<<8) + Serial_RXPacket[5];
			code2 = (Serial_RXPacket[6]<<8) + Serial_RXPacket[7];
			u2_printf("t3.txt=\"%d+%d\"",code1,code2);
			delay_ms(10);
			u2_printf("tt3.txt=\"%d+%d\"",code1,code2);
			delay_ms(10);
			u2_printf("tt3.txt=\"%d+%d\"",code1,code2);
			delay_ms(10);
			u2_printf("t3.txt=\"%d+%d\"",code1,code2);
			delay_ms(10);
			u2_printf("t3.txt=\"%d+%d\"",code1,code2);
			delay_ms(10);

			break;
		}


		case 0x05:  //����ʶ��x����
		{
			x_mdis = Serial_RXPacket[1];
			stepPosition=0;
			MOTOR_Displacement_mm(x_mdis,0);
			while(1)
			{
				if(stepPosition == distance)
				{
					break;
				}
			}
			break;
		}		
		case 0x06:  //����ʶ��y����
		{
			y_mdis = Serial_RXPacket[2];
			stepPosition=0;
			MOTOR_Displacement_mm(0,y_mdis);
			while(1)
			{
				if(stepPosition == distance)
				{
					break;
				}
			}
			break;
		}	
		case 0x15:  //��ϸ����ʶ��x����-0.1mm
		{
			x_dis1 = Serial_RXPacket[1];
			x_dis2 = Serial_RXPacket[2];
			temp_dis =  (x_dis1 << 8) | x_dis2 ;
			x_dis  = (int16_t)temp_dis;
			stepPosition=0;
			MOTOR_Displacement_fm(x_dis,0);
			while(1)
			{
				if(stepPosition == distance)
				{
					break;
				}
			}
			break;
		}		
		case 0x16:  //��ϸ����ʶ��y����-0.1mm
		{
            y_dis1 = Serial_RXPacket[1];
			y_dis2 = Serial_RXPacket[2];
			temp_dis =  (y_dis1 << 8) | y_dis2 ;
			y_dis  = (int16_t)temp_dis;
			stepPosition=0;
			MOTOR_Displacement_fm(0,y_dis);
			while(1)
			{
				if(stepPosition == distance)
				{
					break;
				}
			}
			break;
		}	
		//
		case 0x07:     //���ӵ��Ϸų���λ��1
		{
            claw_get_block1();
			break;
		}

        case 0x08:     //���ӵ��Ϸŵ�����λ��2
		{
            claw_get_block2();
			break;
		}

		case 0x09:     //���ӵ��Ϸŵ�����λ��3
		{
            claw_get_block3();   
			break;
		}
        case 0x0A:   //���ӵ��Ϸŵ�����λ��4
		{
			claw_get_block4();
		  break;
		}
		case 0x0C:  //���ӵ��Ϸų���λ��5
		{
			claw_get_block5();
			break;
		}
		case 0x0E:  //���ӳ��Ϸŵ�����λ��1
		{
			claw_put_block1();
			break;
		}
		case 0x0F:  //���ӳ��Ϸŵ�����λ��2
		{
			claw_put_block2();
			break;
		}
		case 0x10:  //���ӳ��Ϸŵ�����λ��3
		{
			claw_put_block3();
			break;
		}
		case 0x11:  //���ӳ��Ϸŵ�����λ��4
		{
			claw_put_block4();
			break;
		}
		case 0x12:  //���ӳ��Ϸŵ�����λ��5
		{
			claw_put_block5();
			break;
		}

		

		case 0x0B: // ����ָ���߶�
		{
			position_camera=Serial_RXPacket[1];
			switch (position_camera)
			{
				case 0x01:
				{
					arrive_camera();   //�����ά�룬��ͣ�ɨ��߶�
					  break;
				}
				case 0x02:
				{
					arrive_color_reco();   //����ת�̶�ȡ��ɫ
					  break;					
				}
				case 0x03:
				{
					arrive_circle_capture();   //ʶ��ŵ��ϵĸ߶ȣ�Ҳ���Ǵ�еĸ߶�
					  break;					
				}	
				case 0x04:
				{
					//arrive_circle_capture2();   //ʶ��������ĸ߶�
					break;
				}
				case 0x05:
				{
					arrive_most_up();   //�������
					break;
				}
				default :
				{
                    break ;
				}
			}
			break;
		}

		case 0X0D:  //�춯ģʽ
		{
			claw_mode=Serial_RXPacket[1];   //צ��ģʽ
			move_mode=Serial_RXPacket[2];   //�ƶ�����
			
			switch(claw_mode)  //�춯ģʽ
			{
				case 0x01:  //�����ά�룬���ڲ���Ҫ���������4/26���Ҿ����б�Ҫ������ͷɨ����
				{	
					stepPosition=0;   //��
					stepPosition1=0;  //ץ
					MOTOR_Displacement(move_mode,0);
					claw_turn0();
					delay_ms(200);  //��ִ���ܵ���ִ��������
					arrive_camera();//Ӧ�������
					while(1)
					{
						if((stepPosition == distance)&&(stepPosition1 == distance1))   //���������
						{
							break;
						}
					}	
					break;
				}
				case 0x02:     //��������̨
				{
					stepPosition=0;   //��
					stepPosition1=0;  //ץ
					MOTOR_Displacement(move_mode,0);
					delay_ms(200);  //��ִ���ܵ���ִ��������
					arrive_color_reco();  //�������ɫʶ��߶�
					while(1)
					{
						if((stepPosition == distance)&&(stepPosition1 == distance1))   //���������
						{
							break;
						}
					}						
					break;
				}

				case 0x03:  // �������ʶ��1��Ӧ���ǿ�Ҫ������ʶ��ĵط�����������ܣ��߽��͵�����߶�
				{
					stepPosition=0;   //��
					stepPosition1=0;  //ץ
					MOTOR_Displacement(move_mode,0);
					delay_ms(200);  //��ִ���ܵ���ִ��������
					arrive_circle_capture();
					while(1)
					{
						if((stepPosition == distance)&&(stepPosition1 == distance1))   //���������
						{
							break;
						}
					}
					break;
				}

				case 0x04:  // �������߶�
				{
					stepPosition=0;   //��
					stepPosition1=0;  //ץ
					MOTOR_Displacement(move_mode,0);
					delay_ms(200);  //��ִ���ܵ���ִ��������
					arrive_circle_capture2();
					while(1)
					{
						if((stepPosition == distance)&&(stepPosition1 == distance1))   //���������
						{
							break;
						}
					}
					break;
				}
				case 0x05:  // ת���������ߣ��½���������̨ʶ��߶�
				{
					stepPosition=0;   //��
					stepPosition1=0;  //ץ
					MOTOR_Displacement(move_mode,0);
					claw_turn0();
					delay_ms(200);  //��ִ���ܵ���ִ��������
					arrive_color_reco();
					while(1)
					{
						if((stepPosition == distance)&&(stepPosition1 == distance1))   //���������
						{
							break;
						}
					}	
					break;
				}				
				case 0x06:  // ���߱�ת��������ʶ��1�ĸ߶�
				{
					stepPosition=0;   //��
					stepPosition1=0;  //ץ
					MOTOR_Displacement(move_mode,0);
					claw_turn0();
					delay_ms(200);  //��ִ���ܵ���ִ��������
					arrive_circle_capture();
					while(1)
					{
						if((stepPosition == distance)&&(stepPosition1 == distance1))   //���������
						{
							break;
						}
					}	
					break;
				}
				case 0x07:  //�ڰ����1�ӵ��Ϸų��ϵĹ����б��߱߷�
				{

					claw_turn0();
					claw_open();
					arrive_block_get();
					delay_ms(400);
					claw_close();
					delay_ms(400);
					arrive_most_up();//�ӵ���ץ������鲢�������

					stepPosition=0;   //��
					stepPosition1=0;  //ץ
					MOTOR_Displacement(move_mode,0);//�ƶ�����һ����Ҫ�ӵ���ץȡ���ĵط�
					claw_turn1();
					delay_ms(600);
					arrive_car_put();
					while(1)
					{
						if((stepPosition == distance)&&(stepPosition1 == distance1))   //���������
						{
							break;
						}
						
					}
		
					claw_open1();
					delay_ms(300);
					arrive_most_up();
					claw_open();
					support_turn120();

					break;
				}
				
				case 0x08:  //��Բ��ץ�����ֱ���ջ�צ���뿪
				{

					claw_turn0();
					arrive_block_get1();
					delay_ms(200);
					claw_close();
					delay_ms(200);

					stepPosition=0;   //��
					stepPosition1=0;  //ץ
					arrive_most_up();  //�������
					MOTOR_Displacement(move_mode,0);  //�����׼�����ƶ�
					claw_turn1();   //�ջ�צ��
					delay_ms(400);
					arrive_car_put();
					claw_open1();
					delay_ms(200);
					arrive_most_up();
					support_turn120();
					while(1)
					{
						if((stepPosition == distance)&&(stepPosition1 == distance1))   //���������
						{
							break;
						}
					}

					break;

				}

				case 0x09:  //������������һ����ֱ���ջ�צ���뿪--���ڵڶ��������
				{
		
					claw_open1();       
					claw_turn1();
					delay_ms(500);
					arrive_car_get();
					claw_close();
					delay_ms(300);	
					arrive_most_up(); 
					delay_ms(200);
					claw_turn0();
					support_turn120();
					delay_ms(300);
					arrive_put_down2();
					delay_ms(200);	
					claw_open1();
					delay_ms(300);
					stepPosition=0;  //��
					stepPosition1=0; //ץ 
					arrive_most_up();//�����������
					MOTOR_Displacement(move_mode,0); //�����׼�����ƶ�
					claw_open1();
					claw_turn1();  //�ջ�צ��					
					delay_ms(500);					
					arrive_car_get();
					
					
					while(1)
					{
						if((stepPosition == distance)&&(stepPosition1 == distance1))   //���������
						{
							break;
						}
					}

					break;
				}

				case 0x0A:  //  �ӵ��ϰ����2�ŵ����ϱ�ץ����
				{
					claw_turn0();
					claw_open();
					arrive_block2_get();
					delay_ms(200);
					claw_close2();
					delay_ms(200);
					arrive_most_up();//�ӵ���ץ������鲢�������

					stepPosition=0;   //��
					stepPosition1=0;  //ץ
					MOTOR_Displacement(move_mode,0);//�ƶ�����һ����Ҫ�ӵ���ץȡ���ĵط�
					claw_turn1();
					delay_ms(500);
					arrive_car2_put();
					while(1)
					{
						if((stepPosition == distance)&&(stepPosition1 == distance1))   //���������
						{
							break;
						}
						
					}
		
					claw_open1();
					delay_ms(200);
					arrive_most_up();
					claw_open();
					support_turn120();

				}
				


				default:
				{
					break;
				}
			}
			break;
		}


		case 0x13:
		{
            ResetAng_Z(); //����Z��������
			break;
		}	
		case 0x14:
		{
           MOTOR_Align();  //����ʶ����ת��ǰ/�������̬����
			break;
		}	
		case 0x21:  
		{

			break;
		}
		case 0x22: 
		{

			break;
		}
		case 0x23:  
		{

			break;
		}
		case 0x24: 
		{

			 break;
		}
		case 0x25:  
		{

			 break;
		}
		case 0x26:  //����ת�߰�צ��ת���������½�������ʶ��߶ȣ�������Ҫ��ƫ
		{
			angle = Serial_RXPacket[3];
			stepPosition=0;   //ת
			stepPosition1=0;  //����
			MOTOR_Angle(angle);
			claw_turn0();
			delay_ms(200);
			claw_open();
			delay_ms(200);  //��ִ���ܵ���ִ��������
			arrive_circle_capture();  //����ǰ���ʶ��߶�
			while(1)
			{
				if((stepPosition == angle_temp)&&(stepPosition1 == distance1))   //���������
				{
					break;
				}
			}

			if(angle>0)base_angle+=90; 
			else base_angle-=90;

			if(base_angle>180)//270
			{
				base_angle=-90;
			}
			else if(base_angle<-180)//-270
			{
				base_angle=90; 
			}

            delay_ms(100);

			 break;
		
		}
		case 0x29:     //����ת�߰�צ��ת���������½���ת��ʶ��߶ȣ�������Ҫ��ƫ
		{
			angle = Serial_RXPacket[3];
			stepPosition=0;   //ת
			stepPosition1=0;  //����
			MOTOR_Angle(angle);
			claw_turn0();
			delay_ms(200);
			claw_open();
			delay_ms(200);  //��ִ���ܵ���ִ��������
			arrive_color_reco();  //�����ת��ʶ����ɫ
			while(1)
			{
				if((stepPosition == angle_temp)&&(stepPosition1 == distance1))   //���������
				{
					break;
				}
			}

			if(angle>0)base_angle+=90; 
			else base_angle-=90;

			if(base_angle>180)//270
			{
				base_angle=-90;
			}
			else if(base_angle<-180)//-270
			{
				base_angle=90; 
			}

            delay_ms(100);


			break;

		}

        case 0x27:   
		{

			
			break;
		}
		case 0x30 :
		{

			break;
		}
		case 0x31 :
		{

				break;
		}
		case 0x32 :
		{
			claw_turn0();
				break;
		}

		case 0x36 ://54
		{
			UART1_SendString((uint8_t*)UART5_RX_BUF);  //����ݮ�ɷ��Ͷ�ά����Ϣ

			UART5_ParseCode(UART5_RX_BUF,&code1,&code2);//��������ά�����ݣ���ʱUART5_BUX����Ȼ��ŵ��Ƕ�ά������	

			// unsigned char data[8] ;
			// data[0]=0xAB;
			// data[1]= code1/100;
			// data[2]= (code1/10)%10;
			// data[3]= (code1)%10;
			// data[4]= code2/100;
			// data[5]= (code2/10)%10;
			// data[6]= (code2)%10;
			// data[7]=0xCD;
			// UART1_SendArray(data,8);

			u2_printf("tt3.txt=\"%d+%d\"",code1,code2);//��η��͸�������
			delay_ms(10);
			u2_printf("tt3.txt=\"%d+%d\"",code1,code2);
			delay_ms(10);
			u2_printf("t3.txt=\"%d+%d\"",code1,code2);
			delay_ms(10);
			u2_printf("t3.txt=\"%d+%d\"",code1,code2);
			delay_ms(10);
			u2_printf("tt3.txt=\"%d+%d\"",code1,code2);
			delay_ms(10);		

			break;
		}
		case 0x34 :
		{



			break;
		}
		case 0x33 :
		{
		    claw_home();
			break;
		}
				
		
		
		default:
			break;
	}
}
	

/********************
�������� : צ�ӵĳ�ʼ����
������� : ��
������� ����
**********************/
void claw_up_start(void)
{
	Motor1_DIR(1);
	distance1=9800;
	stepPosition1=0;
	MSD_Move1(9800,12,12,16);
	while(1)
	{
		if(stepPosition1 == distance1)
		{
			break;
		}
	}
}



/********************
�������� : צ�ӵ�����
������� : ��
������� ����
**********************/
void claw_up(void)
{
	Motor1_DIR(1);
	distance1=9600;
	stepPosition1=0;
	MSD_Move1(9600,12,12,16);
	while(1)
	{
		if(stepPosition1 == distance1)
		{
			break;
		}
	}
}
/********************
�������� : צ�ӵ��½�
������� : ��
������� ����
**********************/
void claw_down(void)
{
	Motor1_DIR(0);
	distance1=9600+1500;
	stepPosition1=0;
	MSD_Move1(11100,12,12,16);  // 9600+1500            3600+1500= 5100
	while(1)
	{
		if(stepPosition1 == distance1)
		{
			break;
		}
	}
}
void claw_down2(void)
{
	Motor1_DIR(0);
	distance1=1500;
	stepPosition1=0;
	MSD_Move1(1500,12,12,16);
	while(1)
	{
		if(stepPosition1 == distance1)
		{
			break;
		}
	}
}
///********************
//�������� : צ�ӵ�����  �ŵڶ��㣨1��
//������� : ��
//������� ����
//**********************/
//void claw_down3(void)
//{
//	Motor1_DIR(0);
//	distance1=6000;
//	stepPosition1=0;
//	MSD_Move1(6000,12,12,16);
//	while(1)
//	{
//		if(stepPosition1 == distance1)
//		{
//			break;
//		}
//	}
//}
///********************
//�������� : צ�ӵ�����  �ŵڶ��㣨2��
//������� : ��
//������� ����
//**********************/
//void claw_down4(void)
//{
//	Motor1_DIR(0);
//	distance1=5100;
//	stepPosition1=0;
//	MSD_Move1(5100,12,12,16);
//	while(1)
//	{
//		if(stepPosition1 == distance1)
//		{
//			break;
//		}
//	}
//}

//void claw_up2(void)
//{
//	Motor1_DIR(1);
//	distance1=3000;
//	stepPosition1=0;
//	MSD_Move1(3000,12,12,16);
//	while(1)
//	{
//		if(stepPosition1 == distance1)
//		{
//			break;
//		}
//	}
//}
/********************
�������� : צ�ӵ��ſ�����Ƕ�
������� : ��
������� ����
**********************/

void claw_open(void)
{
		servo_angle2=15;
		SERVO2_CONTRAL(servo_angle2);
		delay_ms(25);
		SERVO2_CONTRAL(servo_angle2);
		delay_ms(25);	
}

/********************
�������� : צ�ӵĹرգ����ھ������
������� : ��
������� ����
**********************/
void claw_close(void)
{
		servo_angle2=95;
		SERVO2_CONTRAL(servo_angle2);
		delay_ms(25);
		SERVO2_CONTRAL(servo_angle2);
		delay_ms(25);	
}

/********************
�������� : צ�ӵĹرգ����ھ������
������� : ��
������� ����
**********************/
void claw_close2(void)
{
		servo_angle2=98;
		SERVO2_CONTRAL(servo_angle2);
		delay_ms(25);
		SERVO2_CONTRAL(servo_angle2);
		delay_ms(25);	
}


/********************
�������� : צ�ӵ�С�Ƕ��ſ�����ֹ������
������� : ��
������� ����
**********************/
void claw_open1(void)
{
		servo_angle2=70;    //71
		SERVO2_CONTRAL(servo_angle2);
		delay_ms(25);
		SERVO2_CONTRAL(servo_angle2);
		delay_ms(25);	
}
// ת��ǰ��
/********************
�������� : צ����ת��0��
������� : ��
������� ����
**********************/
void claw_turn0(void)
{
		servo_angle3=9;					//56
		SERVO3_CONTRAL(servo_angle3);      //56
}

// ת������
/********************
�������� : צ����ת��129 = (90 + 38.97)��
������� : ��
������� ����
**********************/
void claw_turn129(void)
{
		servo_angle3=135;
		SERVO3_CONTRAL(servo_angle3);			//230
}

/********************
�������� : צ����ת��ת�����Ϸ�
������� : ��
������� ����
**********************/
void claw_turn1(void)
{
		servo_angle3 = 132;   //   ����ĳ�161
		SERVO3_CONTRAL(servo_angle3);
}
/********************
�������� : צ����ת��λ�ö�
������� : ��
������� ����
**********************/
void claw_turn2(void)
{
		servo_angle3 = 160;
		SERVO3_CONTRAL(servo_angle3);
}
/********************
�������� : צ����ת��λ����
������� : ��
������� ����
**********************/
void claw_turn3(void)
{
		servo_angle3=186;
		SERVO3_CONTRAL(servo_angle3);
}
/********************
�������� : צ����ת��λ����
������� : ��
������� ����
**********************/
void claw_turn4(void)
{
		servo_angle3=214;
		SERVO3_CONTRAL(215);
}
/********************
�������� : צ����ת��λ����
������� : ��
������� ����
**********************/
void claw_turn5(void)
{
		servo_angle3=242;
		SERVO3_CONTRAL(250);
}


/********************
�������� : ����̨����ת
������� : ��
������� ����
**********************/
void support_turn120(void)
{
	MSD_Move2(1067,6,6,12);
}
void support_turn35(void)
{
	MSD_Move2(700,1,1,2);
}
void support_turn67(void)
{
	MSD_Move2(400,1,1,2);
}
///********************
//�������� : צ�Ӱ����ŵ�����̨
//������� : ��
//������� ����
//**********************/
//void claw_in(void)
//{
//	claw_close();
//	claw_up();          // 9600
//	claw_turn129();
//	delay_ms(1200);
//	delay_ms(1200);
//	claw_down2();       // -1500
//	claw_open();
//	claw_up2();         //  3000
//	claw_turn0();
//	support_turn120();
//	delay_ms(1200);
//	delay_ms(1200);
//	claw_turn0();
//	claw_down();        // -11100
//	delay_ms(1200);	
//	delay_ms(1200);
//}
///********************
//�������� : צ�Ӱ�����ó�����̨ �ŵ���һ��
//������� : ��
//������� ����
//**********************/
//void claw_out(void)
//{
//	claw_open();
//	claw_up();          //   9600
//	claw_turn129();
//	delay_ms(1200);
//	delay_ms(1200);
//	claw_down2();       //  -1500
//	claw_close();
//	claw_up2();         //   3000
//	claw_turn0();
//	support_turn120();
//	delay_ms(1200);
//	delay_ms(1200);
//	claw_turn0();
//	claw_down();       //   -11100
//	delay_ms(1200);	
//	delay_ms(1200);
//	claw_open();
//}
///********************
//�������� : צ�Ӱ�����ó�����̨ �ŵ��ڶ���
//������� : ��
//������� ����
// //    �߶�5100
//**********************/
//void claw_out2(void)
//{
//	claw_open();
//	claw_up();         //    9600
//	claw_turn129();
//	delay_ms(1200);
//	delay_ms(1200);
//	claw_down2();     //    -1500
//	claw_close();
//	claw_up2();       //     3000   
//	claw_turn0();
//	support_turn120();
//	delay_ms(1200);
//	delay_ms(1200);
//	claw_turn0();
//	claw_down3();     //    -6000
//	delay_ms(1200);
//	delay_ms(1200);	
//	claw_open();
////  12600-1500-6000 = 5100
//}

///***********************
//����ͷλ��
//**********************/
//void task1()
//{
//	
//	
//}
