#include<msp430f169.h>
#include"delay.h"
#include"type.h"
#include"Port.h"
#include"lcd_1602.h"
#include"DC_motor.h"
#include"my_4x4key.h"
#include"clock.h"

uint a = 10000;         //初始为60%占空比
uchar key_value=0;      //全局变量键值
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
        P1SEL |= 0x04;  //P1.2第二功能 TA1 输出PWM
        TACTL= TASSEL_2 + MC_1; //定时器A时钟源为SMCLK，增计数模式
        STATUS = 1;
        key_value = keyscan();
        switch(key_value){  //对键值进行处理
            case 1: start();        break;     //启动
            case 2: stop();         break;     //停止
            case 3: speed_add();    break;     //加速，每次增加1000个定时器计数值
            case 4: speed_sub();    break;     //减速，每次减少1000个定时器计数值
            default: break;
        }
        show_status(STATUS, a);
        P6DIR = 0xff;       //调试程序
        P6OUT = 0x0f;
        key_value = 0x00;   //键值清除
    }
}

