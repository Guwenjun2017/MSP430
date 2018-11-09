#include<msp430f169.h>
#include"inf.h"
#include"lcd_1602.h"
#include"Port.h"
#include"type.h"
#include"clock.h"
#include"delay.h"

uint count;              //TIMERA中断中变量
uchar a;                 //TIMERA中断中变量
uchar BIT;               //接收数据位数
uchar start;             //接收到数据的标志位
uchar data[33];          //一位引导码+32位数据
uchar ircode[4];         //存储两个用户码、一个数据码、一个数据码的反码
uchar VALUE;             //全局键值

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;  //关狗
    init_clk_1M();          //初始化时钟，8分频=>1MHz主频
    init_1602_port();       //设置1602端口
    lcd1602_init();         //初始化1602
    init_device();          //初始化红外接收设备
    _EINT();                //开启总中断
    while(1)
    {
        infrared_decode();
        get_value(ircode[2]);
        write_cmd(0x80+0x40);
        display(VALUE);
    }
}

#pragma vector=PORT1_VECTOR    //端口P1中断
__interrupt void Port1_ISR(void)
{
    if(count>32)        //第一次接收时判断count数量，>32为引导码，
    {
        BIT=0;
    }
    data[BIT] = count;  //每次存储count个数，间接获得上升沿时间间隔
    count = 0;          //存完清零count
    BIT++;
    if(BIT==33)         //接收33次后（即33个上升沿触发后），清零索引BIT，并将start标志置位
    {
        BIT=0;
        start=1;
    }
    P1IFG = 0;          //清除中断标志位
}

#pragma vector=TIMERA0_VECTOR     //TIMERA中断，在达到定时要求时才会产生中断
__interrupt void Timer_A(void)    //此次设置CCR0为33，在1MHz下，约为33us产生一次中断
{
    a++;                          //一次中断，a自增一次
    if(a == 8)                    //a自增8次，即经过约264us，清零，count++
    {
        a = 0;
        count++;
    }
}
