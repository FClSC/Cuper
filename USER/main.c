#include "system_init.h"


/*********************************************


FClSc 2025/8/3

2025  Robcup�й������˴��� 

ɽ����̨  2025/8/17

�������ܰ���
ר����



*********************************************/

void test(void); //���Ժ���
//Ҳ��������ڳ�ʼZ��0�ȵ�ƫ��
int16_t base_angle = 0; //��׼�Ƕ�,������λ��Ҫ��Ƕȸ����ã�������λ������ת90�ȣ���׼�ǶȻ��90�ȣ���������ȥ90��

//���ɨ����Ϣ��ȫ�ֱ���
int16_t code1 =0; 
int16_t code2 =0; 	



int main(void)
{		
	system_Init();
	contral_motor_Init();
	claw_Init();
	arrive_most_down(); //������͸߶�
	delay_ms(1000);
	ResetAng_Z(); 

	// ��ʼ������
	//arrive_most_up();
	// delay_ms(200);
	// claw_turn1();
	// claw_open();

	
	arrive_most_up();
	delay_ms(1000);
	//	claw_put_block1();
	//	delay_ms(1000);
	//	claw_put_block2();
	//	delay_ms(1000);
	//	claw_put_block3();
	//	delay_ms(1000);
	//	claw_put_block4();
	//	delay_ms(1000);
	//	claw_put_block5();
	//	delay_ms(1000);
	//	claw_put_block5();
	// claw_turn1();
	// delay_ms(600);
	// claw_turn2();
	// delay_ms(600);
	// claw_turn3();
	// delay_ms(600);
	// claw_turn4();
	// delay_ms(600);
	// claw_turn5();
	// delay_ms(600);

	// arrive_car_put();

//	MOTOR_Displacement(20,0);
//			while(1)
//			{
//				if(stepPosition == distance)
//				{
//					break;
//				}
//			}

//	delay_ms(3000);
//	MOTOR_Displacement(0,20);
//			while(1)
//			{
//				if(stepPosition == distance)
//				{
//					break;
//				}
//			}
			int angle = 90;
			MOTOR_Angle(90);
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

			MOTOR_Align();

			


	

	while(1)              //������
	{





		OLED_Printf(0,0,OLED_8X16,"Angle=%.3f",global_angle);
		OLED_Printf(0,16,OLED_8X16,"BaseAngle=");
		OLED_ShowSignedNum(64,16,base_angle,3,OLED_8X16);
		OLED_Printf(0,32,OLED_8X16,"Err_Angle=%.3f",global_angle-base_angle);

		OLED_Update();
	

		if(Key_Get() == 1)//һ��������������£������ػ���������ָ�ͬʱ��Ŀ��ǶȺʹ�������ʾΪ0
		{
			base_angle = 0;
			arrive_most_up();
			delay_ms(200);
			claw_turn0();
			delay_ms(200);
			arrive_most_down(); //������͸߶�



			// arrive_camera();//��͸߶�
			// delay_ms(2000);
			// arrive_color_reco();//������ɫʶ��߶�
			// delay_ms(2000);
			// arrive_block_get1();//����ת����ץ���ĸ߶�
			// delay_ms(2000);
            
			delay_ms(500);
			UART_SendPacket2UP(0x02);
			UART_SendPacket2UP(0x02);
			UART_SendPacket2UP(0x02);
			UART_SendPacket2UP(0x02);	
		}  
     
		if(Serial1_GetRxFlag() == 1)//������ݮ����Ϣ
		{
			uart_handle();
			UART_SendPacket2UP(0x01);
		}

		if(Serial5_GetRxFlag() == 1)//������յ���ɨ�뷢�������ݾʹ���
		{

			delay_ms(300);//�ȴ����ݽ�����ȫ
			UART5_ParseCode(UART5_RX_BUF,&code1,&code2);//��������ά�����ݣ���ʱUART5_BUX����Ȼ��ŵ��Ƕ�ά������	
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

		}
		
	}	 
	

} 


void test(void) //���Գ���
{
	// claw_get_block1();//ת�̷ų���
	// delay_ms(1000);
	// claw_get_block1();
	// delay_ms(1000);
	// claw_get_block1();
	// delay_ms(1000);

	// claw_get_block();//���Ϸų��ϣ��춯    1111
	// delay_ms(1000);
	// claw_get_block();
	// delay_ms(1000);
	// claw_get_block();
	// delay_ms(1000);

/////////////////////////////////////
	// claw_turn0();
	// claw_open();
	// arrive_block_get();
	// delay_ms(400);
	// claw_close();
	// delay_ms(400);
	// arrive_most_up();//�ӵ���ץ������鲢�������

	// stepPosition=0;   //��
	// stepPosition1=0;  //ץ
	// MOTOR_Displacement(10,0);//�ƶ�����һ����Ҫ�ӵ���ץȡ���ĵط�
	// claw_turn1();
	// delay_ms(600);
	// arrive_car_put();
	// while(1)
	// {
	// 	if((stepPosition == distance)&&(stepPosition1 == distance1))   //���������
	// 	{
	// 		break;
	// 	}
		
	// }

	// claw_open1();
	// delay_ms(300);
	// arrive_most_up();
	// claw_open();
	// support_turn120();

	// delay_ms(1000);

	// claw_turn0();
	// claw_open();
	// arrive_block_get();
	// delay_ms(400);
	// claw_close();
	// delay_ms(400);
	// arrive_most_up();//�ӵ���ץ������鲢�������

	// stepPosition=0;   //��
	// stepPosition1=0;  //ץ
	// MOTOR_Displacement(10,0);//�ƶ�����һ����Ҫ�ӵ���ץȡ���ĵط�
	// claw_turn1();
	// delay_ms(600);
	// arrive_car_put();
	// while(1)
	// {
	// 	if((stepPosition == distance)&&(stepPosition1 == distance1))   //���������
	// 	{
	// 		break;
	// 	}
		
	// }

	// claw_open1();
	// delay_ms(300);
	// arrive_most_up();
	// claw_open();
	// support_turn120();

	// delay_ms(1000);

	// claw_turn0();
	// claw_open();
	// arrive_block_get();
	// delay_ms(400);
	// claw_close();
	// delay_ms(400);
	// arrive_most_up();//�ӵ���ץ������鲢�������

	// stepPosition=0;   //��
	// stepPosition1=0;  //ץ
	// MOTOR_Displacement(10,0);//�ƶ�����һ����Ҫ�ӵ���ץȡ���ĵط�
	// claw_turn1();
	// delay_ms(600);
	// arrive_car_put();
	// while(1)
	// {
	// 	if((stepPosition == distance)&&(stepPosition1 == distance1))   //���������
	// 	{
	// 		break;
	// 	}
		
	// }

	// claw_open1();
	// delay_ms(300);
	// arrive_most_up();
	// claw_open();
	// support_turn120();
////////////////////////////////////

	// claw_put_block();//���Ϸŵ���  222222
	// delay_ms(1000);
	// claw_put_block();
	// delay_ms(1000);
	// claw_put_block();
	// delay_ms(1000);

	// claw_put_blockF2();//���ϷŶ���
	// delay_ms(1000);
	// claw_put_blockF2();
	// delay_ms(1000);
	// claw_put_blockF2();
	// delay_ms(1000);

	claw_put_block2();//���Ϸ�ת��
	delay_ms(1000);
	claw_put_block2();
	delay_ms(1000);
	claw_put_block2();
	delay_ms(1000);


///���������


	arrive_most_up();
	claw_open1();       
	claw_turn1();
	delay_ms(600);
	arrive_car_get();
	claw_close();
	delay_ms(300);	
	arrive_most_up(); 
	delay_ms(200);
	claw_turn0();
	delay_ms(300);
	arrive_block_putF2();//��ץ���ϵĸ߶�һ����
	delay_ms(500);	
	claw_open();
	delay_ms(300);
	arrive_most_up();
	support_turn120();

	arrive_most_up();
	claw_open1();       
	claw_turn1();
	delay_ms(600);
	arrive_car_get();
	claw_close();
	delay_ms(300);	
	arrive_most_up(); 
	delay_ms(200);
	claw_turn0();
	delay_ms(300);
	arrive_block_putF2();//��ץ���ϵĸ߶�һ����
	delay_ms(500);	
	claw_open();
	delay_ms(300);
	arrive_most_up();
	support_turn120();

	arrive_most_up();
	claw_open1();       
	claw_turn1();
	delay_ms(600);
	arrive_car_get();
	claw_close();
	delay_ms(300);	
	arrive_most_up(); 
	delay_ms(200);
	claw_turn0();
	delay_ms(300);
	arrive_block_putF2();//��ץ���ϵĸ߶�һ����
	delay_ms(500);	
	claw_open();
	delay_ms(300);
	arrive_most_up();
	support_turn120();





}
