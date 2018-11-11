/*
 * 硬件连线
 *          MSP430F149
 *       -----------------
 *      |                 |        -------
 *      |             P1.0|------>|S(OUT) |
 *      |                 |       |  红外   |
 *      |                 |       |    VCC|--->5V
 *      |                 |       |       |
 *                                |    GND|
 * 现象：
 *      红外遥控器按下不同的按键，1602第一行显示不同的数据码型，第二行
 *      显示自行设置的对应的键值，该例程只配置了遥控器后四行按键的键值。
 */
#include<msp430f169.h>
#include"inf.h"                  //红外头文件
#include"lcd_1602.h"             //lcd1602头文件
#include"Port.h"                 //端口设置
#include"clock.h"                //时钟配置
#include"delay.h"                //常用延迟

unsigned int count;              //TIMERA中断中变量
unsigned char a;                 //TIMERA中断中变量
unsigned char BIT;               //接收数据位数
unsigned char RECV_FLAG;         //接收到数据的标志位
unsigned char data[33];          //一位引导码+32位数据的数组
unsigned char ircode[4];         //存储两个用户码、一个数据码、一个数据码的反码
unsigned char VALUE;             //全局键值

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;    //关狗

    init_clk_1M();               //初始化时钟，8分频=>1MHz主频
    init_1602_port();            //设置1602端口
    init_port1();                //初始化外部中断P1.0

    lcd1602_init();              //初始化1602
    init_device();               //初始化红外接收设备和TIMERA
    _EINT();                     //开启总中断

    while(1)
    {
        infrared_decode();       //红外解码
        set_value(ircode[2]);    //配置各个按键的键值
        write_cmd_1602(0x80+0x40);    //1602第二行
        dis_num(VALUE);          //显示设置的键值
    }
}

#pragma vector=PORT1_VECTOR      //端口P1中断
__interrupt void Port1_ISR(void)
{
    if(count>32)                 //第一次接收时判断count数量，>32为引导码，
    {
        BIT=0;
    }
    data[BIT] = count;           //每次存储每位数据持续的count个数，一个count为8*33=264us，间接获得下降沿时间间隔
    count = 0;                   //存完清零count个数
    BIT++;
    if(BIT==33)                  //接收33次后（即33个下降沿触发后），清零索引BIT，并将start标志置位
    {
        BIT=0;
        RECV_FLAG=1;
    }
    P1IFG = 0;                   //清除中断标志位
}

#pragma vector=TIMERA0_VECTOR     //TIMERA中断，在达到定时要求时才会产生中断
__interrupt void Timer_A(void)    //此次设置CCR0为33，在1MHz下，约为33us产生一次中断
{
    a++;                          //一次中断，a自增一次
    if(a == 8)                    //a自增8次，即经过约33*8=264us，清零，count++
    {
        a = 0;
        count++;
    }
}
