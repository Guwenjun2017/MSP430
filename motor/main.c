#include<msp430f169.h>
#include"delay.h"
#include"type.h"
#include"Port.h"
#include"lcd_1602.h"
#include"DC_motor.h"
#include"my_4x4key.h"

uint a = 12000;         //初始为60%占空比
uchar key_value=0;      //全局变量键值
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
        switch(key_value){  //对键值进行处理
            case 1: start();        break;     //启动
            case 2: stop();         break;     //停止
            case 3: speed_add();    break;     //加速，每次增加1000个定时器计数值
            case 4: speed_sub();    break;     //减速，每次减少1000个定时器计数值
            default: break;
        }
        //write_cmd(0x80);    //第一行显示
        if(STATUS == 1){
            write_cmd(0x01);
            dis_str(0x80, array_run);
        }
        else
            write_cmd(0x01);
            dis_str(0x80, array_stop);
        dis_str(0x80+0x40, array_currentspeed);
        write_data(':');
        display(a-10000);   //1602观察调试
        P6DIR = 0xff;       //调试程序
        P6OUT = 0xf0;
        key_value = 0x00;   //键值清除
    }
}
