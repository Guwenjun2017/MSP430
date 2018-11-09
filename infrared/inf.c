/*
 * inf.c
 *
 *  Created on: 2018年11月9日
 *      Author: Jack
 */
#include"inf.h"
extern uchar start;
extern uchar ircode[4];
extern uchar data[33];
extern uchar VALUE;

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

void get_value(uchar argu)
{
    switch(argu)
    {
    case  7: VALUE = 'S'; break; case 21: VALUE = 'A'; break; case  9: VALUE = 'E'; break;
    case 22: VALUE = 0; break;
    case 12: VALUE = 1; break;   case 24: VALUE = 2; break;   case 94: VALUE = 3; break;
    case  8: VALUE = 4; break;   case 28: VALUE = 5; break;   case 90: VALUE = 6; break;
    case 66: VALUE = 7; break;   case 82: VALUE = 8; break;   case 74: VALUE = 9; break;
    default: break;
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



