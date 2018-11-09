#include<msp430f169.h>
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
void init_port1();       //设置P1.0端口
void init_device();      //初始化TIMERA配置
void infrared_decode();  //红外解码函数

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
        switch(ircode[2])
        {
        case  7: VALUE = 'S'; break; case 21: VALUE = 'A'; break; case  9: VALUE = 'E'; break;
        case 22: VALUE = 0; break;
        case 12: VALUE = 1; break;   case 24: VALUE = 2; break;   case 94: VALUE = 3; break;
        case  8: VALUE = 4; break;   case 28: VALUE = 5; break;   case 90: VALUE = 6; break;
        case 66: VALUE = 7; break;   case 82: VALUE = 8; break;   case 74: VALUE = 9; break;
        default: break;
        }
        write_cmd(0x80+0x40);
        display(VALUE);
    }
}

void infrared_decode()
{
    if(start == 1)    //若成功接收到一组数据
    {
        uchar k=1, i, j,value;
        for(j=0; j<4; j++)
        {
            for(i=0; i<8; i++)
            {
                value = value>>1;
                if(data[k]>6)
                {
                    value = value|0x80;
                }
                k++;
            }
            ircode[j]=value;
            start=0;
        }
        P6DIR = 0xff;
        P6OUT = ircode[2];
        write_cmd(0x01);
        write_cmd(0x80);
        display(ircode[2]);
    }
}

void init_port1()  //配置P1端口的中断设置
{
    P1DIR &= ~BIT0;     //设置为输入方向
    P1IES |= BIT0;      //选择下降沿触发
    P1IE |= BIT0;       //打开中断允许
    P1IFG = ~BIT0;      //P1IES的切换可能使P1IFG置位，需清除
}

void init_device()
{
    init_port1();
    TACCTL0 = CCIE;            //CCR0中断使能
    TACCR0 = 33;               //计数终点
    TACTL = TASSEL_2 + MC_1;   //TIMER_A选择timer时钟MCLK和增计数模式
    BCSCTL2 |= SELS;
    _EINT();   //开启总中断
}

#pragma vector=PORT1_VECTOR    //端口P1中断
__interrupt void Port1_ISR(void)
{
    if(count>32)       //第一次接收时判断count数量，>32为引导码，
    {
        BIT=0;
    }
    data[BIT] = count; //每次存储count个数，间接获得上升沿时间间隔
    count = 0;         //存完清零count
    BIT++;
    if(BIT==33)    //接收33次后（即33个上升沿触发后），清零索引BIT，并将start标志置位
    {
        BIT=0;
        start=1;
    }
    P1IFG = 0;    //清除中断标志位
}

#pragma vector=TIMERA0_VECTOR     //TIMERA中断，在达到定时要求时才会产生中断
__interrupt void Timer_A(void)    //此次设置CCR0为33，在1MHz下，约为33us产生一次中断
{
    a++;           //一次中断，a自增一次
    if(a == 8)     //a自增8次，即经过约264us，清零，count++
    {
        a = 0;
        count++;
    }
}
