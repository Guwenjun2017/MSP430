#include<msp430f169.h>
#include"delay.h"
#include"type.h"
#include"Port.h"
#include"lcd_1602.h"
#include"DC_motor.h"
#include"my_4x4key.h"
#include"clock.h"

uint a = 10000;         //��ʼΪ60%ռ�ձ�
uchar key_value=0;      //ȫ�ֱ�����ֵ
uchar STATUS = 0;




void main()
{
    WDTCTL = WDTPW + WDTHOLD;
    init_clk_1M();
    Init_PWM();

    init_1602_port();
    lcd1602_init();

    while(1){
        CCR1 = a;
        P1SEL |= 0x04;  //P1.2�ڶ����� TA1 ���PWM
        TACTL= TASSEL_2 + MC_1; //��ʱ��Aʱ��ԴΪSMCLK��������ģʽ
        STATUS = 1;
        key_value = keyscan();
        switch(key_value){  //�Լ�ֵ���д���
            case 1: start();        break;     //����
            case 2: stop();         break;     //ֹͣ
            case 3: speed_add();    break;     //���٣�ÿ������1000����ʱ������ֵ
            case 4: speed_sub();    break;     //���٣�ÿ�μ���1000����ʱ������ֵ
            default: break;
        }
        show_status(STATUS, a);
        P6DIR = 0xff;       //���Գ���
        P6OUT = 0x0f;
        key_value = 0x00;   //��ֵ���
    }
}

