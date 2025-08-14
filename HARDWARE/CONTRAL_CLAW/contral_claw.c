#include "contral_claw.h"

CLAW_POSITION claw;

int16_t claw_block_get1 = 103 ;   //103

/********************
�������� : צ��λ�ú���  ��λmm
������� : ��
������� ����
ֱ�� 16mm 
תһȦ  8*2*3.14=50.24 
x/mm = 3200/50.24
x = 64*mm
**********************/
void claw_position(int16_t position)
{
	uint32_t pulse=0;
	claw.position_now = claw.position_now + position; // ���µ�ǰλ��
	
	if(claw.position_now<0 || claw.position_now>claw_most_up)
	{
		claw.position_now=claw.position_now - position;
	}
	else
	{
		// ����
		if(position>0)
		{
				Motor1_DIR(1);   // ����
		}
		else
		{
				position=-position;
				Motor1_DIR(0);   // ����
		}
		pulse=(64*position);
		distance1=pulse;
		stepPosition1=0;
		MSD_Move1(pulse,170,170,220); //24 24 30
		while(1)
		{
			if(stepPosition1 == distance1)
			{
				break;
			}
		}
	}
}
/********************
�������� : צ��λ�ú���  ��λmm
������� : ��
������� ����
�Ƿ�������ⲿ����
*********/
void claw_position2(int16_t position)
{
	uint32_t pulse=0;
	claw.position_now = claw.position_now + position; // ���µ�ǰλ��
	if(claw.position_now<0 || claw.position_now>claw_most_up)
	{
		claw.position_now=claw.position_now - position;
	}
	else
	{
		// ����
		if(position>0)
		{
				Motor1_DIR(1);   // ����
		}
		else
		{
				position=-position;
				Motor1_DIR(0);   // ����
		}
		pulse=(64*position);
		distance1=pulse;
		stepPosition1=0;
		MSD_Move1(pulse,100,100,200);
	}
}

////////////////////////////////////////////////////////////צ�Ӹ߶�
/********************
�������� : ��������ͷɨ���λ��
������� : ��
������� ����
**********************/
void arrive_camera(void)
{
	claw.position_target = camera_position;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}

/********************
�������� : ������ߴ�
������� : ��
������� ����
**********************/
void arrive_most_up(void)
{
	claw.position_target = claw_most_up;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}

/********************
�������� : ������ʹ�
������� : ��
������� ����
**********************/
void arrive_most_down(void)
{
	claw.position_target = claw_most_down;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}

/********************
�������� : �������ʶ��ĸ߶�
������� : ��
������� ����
**********************/
void arrive_circle_capture(void)
{
	claw.position_target = circle_capture1;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}

/********************
�������� : ����������ʶ��ĸ߶�
������� : ��
������� ����
**********************/
void arrive_circle_capture2(void)
{
	claw.position_target = circle_capture3;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}

/********************
�������� : ����������ʶ����ɫ�ĸ߶�
������� : ��
������� ����
**********************/
void arrive_color_reco(void)
{
	claw.position_target = circle_capture2;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}


//////�������ĸ߶�

/********************
�������� : �������ץ���ĸ߶�
������� : ��
������� ����
**********************/
void arrive_block_get(void)
{
	claw.position_target = claw_block_get;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}

/********************
�������� : �����������ĸ߶�
������� : ��
������� ����
**********************/
void arrive_block_put(void)
{
	claw.position_target = put_block_down;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}

/********************
�������� : �����������ץ���ĸ߶�
������� : ��
������� ����
**********************/
void arrive_block_get1(void)
{
	claw.position_target = claw_block_get1;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}

/********************
�������� : �����������ץ���ĸ߶�
������� : ��
������� ����
**********************/
void arrive_block_putF2(void)
{
	claw.position_target = claw_block_putF2;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}



//////

/********************
�������� : �����ڳ��Ϸ����ĸ߶�
������� : ��
������� ����
**********************/
void arrive_car_put(void)
{
	claw.position_target = claw_block_put;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}


/********************
�������� : ���ﳵ��ץ���ĸ߶�
������� : ��
������� ����
**********************/
void arrive_car_get(void)
{
	claw.position_target = get_block_down;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}

/********************
�������� : ���������ڵڶ���߶�
������� : ��
������� ����
**********************/
void arrive_put_down2(void)
{
	claw.position_target = put_block_down2;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}


////����ھ�������2���¶�����߶�

/********************
�������� : �������ץ���2�ĸ߶�
������� : ��
������� ����
**********************/
void arrive_block2_get(void)
{
	claw.position_target = claw_block2_get;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}

/********************
�������� : �����������2�ĸ߶�
������� : ��
������� ����
**********************/
void arrive_block2_put(void)
{
	claw.position_target = put_block2_down;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}

/********************
�������� : �����������ץ���2�ĸ߶�
������� : ��
������� ����
**********************/
void arrive_block2_get1(void)
{
	claw.position_target = claw_block2_get1;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}


/********************
�������� : �����ڳ��Ϸ����2�ĸ߶�
������� : ��
������� ����
**********************/
void arrive_car2_put(void)
{
	claw.position_target = claw_block2_put;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}


/********************
�������� : ���ﳵ��ץ���2�ĸ߶�
������� : ��
������� ����
**********************/
void arrive_car2_get(void)
{
	claw.position_target = get_block2_down;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}

/********************
�������� : �������2���ڵڶ���߶�
������� : ��
������� ����
**********************/
void arrive_put2_down2(void)
{
	claw.position_target = put_block2_down2;
	claw.position_temp = claw.position_target - claw.position_now;
	claw_position(claw.position_temp);
}


////////////////////////////////////////////////////צ�Ӿ��嶯��

////���ھ������
/********************
�������� : צ�Ӵӵ�������ų���λ��1
������� : ��
������� ����
**********************/
void claw_get_block1(void)
{
	claw_turn0();
	claw_open();
	delay_ms(300);
	arrive_block_get();
	delay_ms(300);
	claw_close();
	delay_ms(400);
	arrive_most_up();
	claw_turn1();
	delay_ms(600);
	arrive_car_put();
	claw_open1();
//	delay_ms(200);
	arrive_most_up();	
	delay_ms(300);
	claw_turn0();
	//support_turn120();
	claw_open();

}

/********************
�������� : צ�Ӵӵ�������ų���λ��2
������� : ��
������� ����
**********************/
void claw_get_block2(void)
{
	claw_turn0();
	claw_open();
	delay_ms(300);
	arrive_block_get();
	delay_ms(300);
	claw_close();
	delay_ms(400);
	arrive_most_up();
	claw_turn2();
	delay_ms(600);
	arrive_car_put();
	claw_open1();
//	delay_ms(200);
	arrive_most_up();
	delay_ms(300);
	claw_turn0();	
	//support_turn120();
	claw_open();

}

/********************
�������� : צ�Ӵӵ�������ų���λ��3
������� : ��
������� ����
**********************/
void claw_get_block3(void)
{
	claw_turn0();
	claw_open();
	delay_ms(300);
	arrive_block_get();
	delay_ms(300);
	claw_close();
	delay_ms(400);
	arrive_most_up();
	claw_turn3();
	delay_ms(600);
	arrive_car_put();
	claw_open1();
//	delay_ms(200);
	arrive_most_up();	
	delay_ms(300);
	claw_turn0();	
	//support_turn120();
	claw_open();

}

/********************
�������� : צ�Ӵӵ�������ų���λ��4
������� : ��
������� ����
**********************/
void claw_get_block4(void)
{
	claw_turn0();
	claw_open();
	delay_ms(300);
	arrive_block_get();
	delay_ms(300);
	claw_close();
	delay_ms(400);
	arrive_most_up();
	claw_turn4();
	delay_ms(800);
	arrive_car_put();
	claw_open1();
//	delay_ms(200);
	arrive_most_up();	
	delay_ms(300);
	claw_turn0();	
	//support_turn120();
	claw_open();

}

/********************
�������� : צ�Ӵӵ�������ų���λ��5
������� : ��
������� ����
**********************/
void claw_get_block5(void)
{
	claw_turn0();
	claw_open();
	delay_ms(300);
	arrive_block_get();
	delay_ms(300);
	claw_close();
	delay_ms(400);
	arrive_most_up();
	claw_turn5();
	delay_ms(1200);
	arrive_car_put();
	claw_open1();
//	delay_ms(200);
	arrive_most_up();	
	delay_ms(300);
	claw_turn0();	
	//support_turn120();
	claw_open();

}
/********************
�������� : צ�Ӵ������̰����ų��� 
������� : ��
������� ����
**********************/
// void claw_get_block1(void)
// {
// 	claw_turn0();
// 	arrive_block_get1();
// 	delay_ms(200);
// 	claw_close();
// 	delay_ms(200);
// 	arrive_most_up();
// 	claw_turn1();
// 	delay_ms(400);
// 	arrive_car_put();
// 	claw_open1();
// 	delay_ms(200);
// 	arrive_most_up();
// 	claw_turn0();
// 	claw_open();
// 	delay_ms(200);
// 	arrive_color_reco();//�ٴλص�ʶ��������ɫ�ĸ߶�
// 	support_turn120();
	
// }


/********************
�������� : צ�Ӵӳ���λ��1�����ŵ���
������� : ��
������� ����
**********************/
void claw_put_block1(void)
{
    
	arrive_most_up(); 
	delay_ms(200);
	claw_open1();   
	claw_turn1();
	delay_ms(600);
	arrive_car_get();
//	delay_ms(300);
	claw_close();
	delay_ms(200);
	arrive_most_up(); 
    delay_ms(200);
	claw_turn0();
    delay_ms(200);
	arrive_block_put();
	delay_ms(200);	
	claw_open();
	arrive_circle_capture();
	//support_turn120();
	
}

/********************
�������� : צ�Ӵӳ���λ��2�����ŵ���
������� : ��
������� ����
**********************/
void claw_put_block2(void)
{
    
	arrive_most_up(); 
	delay_ms(200);
	claw_open1();   
	claw_turn2();
	delay_ms(600);
	arrive_car_get();
//	delay_ms(300);
	claw_close();
	delay_ms(200);
	arrive_most_up(); 
    delay_ms(200);
	claw_turn0();
    delay_ms(200);
	arrive_block_put();
	delay_ms(200);	
	claw_open();
	arrive_circle_capture();
	//support_turn120();
	
}


/********************
�������� : צ�Ӵӳ���λ��3�����ŵ���
������� : ��
������� ����
**********************/
void claw_put_block3(void)
{
    
	arrive_most_up(); 
	delay_ms(200);
	claw_open1();   
	claw_turn3();
	delay_ms(600);
	arrive_car_get();
//	delay_ms(300);
	claw_close();
	delay_ms(200);
	arrive_most_up(); 
    delay_ms(200);
	claw_turn0();
    delay_ms(500);
	arrive_block_put();
	delay_ms(200);	
	claw_open();
	arrive_circle_capture();
	//support_turn120();
	
}


/********************
�������� : צ�Ӵӳ���λ��4�����ŵ���
������� : ��
������� ����
**********************/
void claw_put_block4(void)
{
    
	arrive_most_up(); 
	delay_ms(200);
	claw_open1();   
	claw_turn4();
	delay_ms(1200);
	arrive_car_get();
//	delay_ms(300);
	claw_close();
	delay_ms(200);
	arrive_most_up(); 
    delay_ms(200);
	claw_turn0();
    delay_ms(500);
	arrive_block_put();
	delay_ms(200);	
	claw_open();
	arrive_circle_capture();
	//support_turn120();
	
}


/********************
�������� : צ�Ӵӳ���λ��5�����ŵ���
������� : ��
������� ����
**********************/
void claw_put_block5(void)
{
    
	arrive_most_up(); 
	delay_ms(200);
	claw_open1();   
	claw_turn5();
	delay_ms(1200);
	arrive_car_get();
//	delay_ms(300);
	claw_close();
	delay_ms(200);
	arrive_most_up(); 
    delay_ms(200);
	claw_turn0();
    delay_ms(500);
	arrive_block_put();
	delay_ms(200);	
	claw_open();
	arrive_circle_capture();
	//support_turn120();
	
}


/********************
�������� : צ�Ӱ����ӳ��Ϸŵ�����
������� : ��
������� ����
**********************/
// void claw_put_blockF2(void)
// {

// 	arrive_most_up();
// 	claw_open1();       
// 	claw_turn1();
// 	delay_ms(600);
// 	arrive_car_get();
// 	claw_close();
// 	delay_ms(300);	
// 	arrive_most_up(); 
//     delay_ms(200);
// 	claw_turn0();
// 	delay_ms(300);
// 	arrive_put_down2();
// 	delay_ms(200);	
// 	claw_open();
// 	delay_ms(300);
// 	arrive_most_up();
// 	support_turn120();

// }


/********************
�������� : צ�Ӱ����ӳ��Ϸŵ�ת�̸߶�
������� : ��
������� ����
**********************/
// void claw_put_block2(void)
// {

// 	arrive_most_up();
// 	claw_open1();       
// 	claw_turn1();
// 	delay_ms(600);
// 	arrive_car_get();
// 	claw_close();
// 	delay_ms(300);	
// 	arrive_most_up(); 
//     delay_ms(200);
// 	claw_turn0();
// 	delay_ms(300);
// 	arrive_block_get1();//��ץ���ϵĸ߶�һ����
// 	delay_ms(500);	
// 	claw_open();
// 	delay_ms(300);
// 	arrive_most_up();
// 	support_turn120();

// }


///���ھ������2��צ�Ӿ��嶯��


/********************
�������� : צ�Ӵӵ�������2�ų��� 
������� : ��
������� ����
**********************/
// void claw_get2_block(void)
// {
// 	claw_turn0();
// 	claw_open();
// 	arrive_block2_get();
// 	delay_ms(200);
// 	claw_close2();
// 	delay_ms(200);
// 	arrive_most_up();
// 	claw_turn1();
// 	delay_ms(600);
// 	arrive_car2_put();
// 	claw_open1();
// 	delay_ms(200);
// 	arrive_most_up();	
// 	support_turn120();
// 	claw_open();

// }

/********************
�������� : צ�Ӵ������̰����2�ų��� 
������� : ��
������� ����
**********************/
// void claw_get2_block1(void)
// {
// 	claw_turn0();
// 	arrive_block2_get1();
// 	delay_ms(200);
// 	claw_close2();
// 	delay_ms(200);
// 	arrive_most_up();
// 	claw_turn1();
// 	delay_ms(800);
// 	arrive_car2_put();
// 	claw_open1();
// 	delay_ms(200);
// 	arrive_most_up();
// 	claw_turn0();
// 	claw_open();
// 	delay_ms(200);
// 	arrive_color_reco();//�ٴλص�ʶ��������ɫ�ĸ߶�
// 	support_turn120();
	
// }


/********************
�������� : צ�Ӵӳ��ϰ����2�ŵ���
������� : ��
������� ����
**********************/
// void claw_put2_block(void)
// {
    
// 	arrive_most_up(); 
// 	claw_open1();   
// 	claw_turn1();
// 	delay_ms(600);
// 	arrive_car2_get();
// //	delay_ms(300);
// 	claw_close2();
// 	delay_ms(200);
// 	arrive_most_up(); 
//     delay_ms(100);
// 	claw_turn0();
//     delay_ms(200);
// 	arrive_block2_put();
// 	delay_ms(200);	
// 	claw_open();
// 	arrive_most_up();
// 	support_turn120();
	
// }

/********************
�������� : צ�Ӱ����2�ӳ��Ϸŵ�����
������� : ��
������� ����
**********************/
// void claw_put2_blockF2(void)
// {

// 	arrive_most_up();
// 	claw_open1();       
// 	claw_turn1();
// 	delay_ms(600);
// 	arrive_car2_get();
// 	claw_close2();
// 	delay_ms(300);	
// 	arrive_most_up(); 
//     delay_ms(200);
// 	claw_turn0();
// 	delay_ms(300);
// 	arrive_put2_down2();
// 	delay_ms(500);	
// 	claw_open();
// 	delay_ms(300);
// 	arrive_most_up();
// 	support_turn120();

// }


/********************
�������� : צ�Ӱ����2�ӳ��Ϸŵ�ת�̸߶�
������� : ��
������� ����
**********************/
// void claw_put2_block2(void)
// {

// 	arrive_most_up();
// 	claw_open1();       
// 	claw_turn1();
// 	delay_ms(600);
// 	arrive_car2_get();
// 	claw_close2();
// 	delay_ms(300);	
// 	arrive_most_up(); 
//     delay_ms(200);
// 	claw_turn0();
// 	delay_ms(300);
// 	arrive_block2_get1();//��ץ���ϵĸ߶�һ����
// 	delay_ms(500);	
// 	claw_open();
// 	delay_ms(300);
// 	arrive_most_up();
// 	support_turn120();

// }










/********************
�������� : צ�ӹ�λ
������� : ��
������� ����
**********************/
void claw_home(void)
{
	arrive_most_up();
	claw_open();
	delay_ms(300);  
	claw_turn3();
}

