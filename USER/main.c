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
	delay_ms(500);
	arrive_most_down(); //������͸߶�
	delay_ms(500);
	ResetAng_Z(); 

	// ��ʼ������
	
	
	arrive_most_up();
	delay_ms(300);
	claw_turn3();
	delay_ms(200);
	claw_open();

	// delay_ms(600);
	// claw_turn0();
	// delay_ms(200);
	// arrive_most_down(); //������͸߶�

	//test();



	




	while(1)              //������
	{


		// I2C_Read_Sensor(color_value);
		// color = getClosestColor(color_value);


		OLED_ClearArea(0, 48, 128, 16); //���OLED��Ļ
		OLED_Printf(0,0,OLED_8X16,"Angle=%.3f",global_angle);
		OLED_Printf(0,16,OLED_8X16,"BaseAngle=");
		OLED_ShowSignedNum(64,16,base_angle,3,OLED_8X16);
		//	OLED_Printf(0,32,OLED_8X16,"Err_Angle=%.3f",global_angle-base_angle);

		OLED_Printf(0,48,OLED_8X16,"%d %d %d %d",color_value[0], color_value[1], color_value[2],color);
		
		// delay_ms(1000);
		OLED_Update();

	

		if(Key_Get() == 1)//һ��������������£������ػ���������ָ�ͬʱ��Ŀ��ǶȺʹ�������ʾΪ0
		{
			base_angle = 0;
			// arrive_most_up();
			// delay_ms(200);
			// claw_turn0();
			// delay_ms(400);
			// arrive_most_down(); //������͸߶�



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

		// if(Serial5_GetRxFlag() == 1)//������յ���ɨ�뷢�������ݾʹ���
		// {

		// 	delay_ms(300);//�ȴ����ݽ�����ȫ
		// 	UART5_ParseCode(UART5_RX_BUF,&code1,&code2);//��������ά�����ݣ���ʱUART5_BUX����Ȼ��ŵ��Ƕ�ά������	
		// 	u2_printf("tt3.txt=\"%d+%d\"",code1,code2);//��η��͸�������
		// 	delay_ms(10);
		// 	u2_printf("tt3.txt=\"%d+%d\"",code1,code2); 
		// 	delay_ms(10);
		// 	u2_printf("t3.txt=\"%d+%d\"",code1,code2);
		// 	delay_ms(10);
		// 	u2_printf("t3.txt=\"%d+%d\"",code1,code2);
		// 	delay_ms(10);
		// 	u2_printf("tt3.txt=\"%d+%d\"",code1,code2);
		// 	delay_ms(10);			

		// }
		
	}	 
	

} 


void test(void) //���Գ���
{




	claw_get_block1();
	delay_ms(2000);
	claw_get_block2();
	delay_ms(2000);
	claw_get_block3();
	delay_ms(2000);
	claw_get_block4();
	delay_ms(2000);
	claw_get_block5();
	delay_ms(2000);
	claw_put_block1();
	delay_ms(2000);
	claw_put_block2();
	delay_ms(2000);
	claw_put_block3();
	delay_ms(2000);
	claw_put_block4();
	delay_ms(2000);
	claw_put_block5();



}
