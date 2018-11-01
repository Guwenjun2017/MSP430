#include<msp430f169.h>
#include"delay.h"
#include"type.h"
#include"Port.h"
#include"lcd_1602.h"
#include"DC_motor.h"
#include"my_4x4key.h"

uint a = 12000;         //��ʼΪ60%ռ�ձ�
uchar key_value=0;      //ȫ�ֱ�����ֵ
uchar STATUS = 0;
uchar array_run[3] = "run";
uchar array_stop[4] = "stop";
uchar array_currentspeed[5] = "speed";

void main()
{
    WDTCTL = WDTPW + WDTHOLD;
    Init_PWM();

    init_1602_port();
    lcd1602_init();
    while(1){
        CCR1 = a;
        key_value = keyscan();
        switch(key_value){  //�Լ�ֵ���д���
            case 1: start();        break;     //����
            case 2: stop();         break;     //ֹͣ
            case 3: speed_add();    break;     //���٣�ÿ������1000����ʱ������ֵ
            case 4: speed_sub();    break;     //���٣�ÿ�μ���1000����ʱ������ֵ
            default: break;
        }
        //write_cmd(0x80);    //��һ����ʾ
        if(STATUS == 1){
            write_cmd(0x01);
            dis_str(0x80, array_run);
        }
        else
            write_cmd(0x01);
            dis_str(0x80, array_stop);
        dis_str(0x80+0x40, array_currentspeed);
        write_data(':');
        display(a-10000);   //1602�۲����
        P6DIR = 0xff;       //���Գ���
        P6OUT = 0xf0;
        key_value = 0x00;   //��ֵ���
    }
}
