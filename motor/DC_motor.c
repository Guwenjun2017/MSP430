/*
 * SG90.c
 *
 *  Created on: 2018年10月15日
 *      Author: Jack
 */
#include<msp430f169.h>
extern unsigned int a;
extern unsigned char STATUS;

void Init_PWM()
{
      BCSCTL2 |= SELS ;                 //SMCLK = XT2
      BCSCTL2 = DIVS0 + DIVS1;
      P1DIR = 0xff;               // P1输出
      P1OUT &= ~BIT2;
      CCR0 = 20000;                // PWM 周期
      CCTL1 = OUTMOD_7;            // CCR1复位/置位
      CCR1 = a;                // CCR1 PWM 占空比50%
      TACTL = TASSEL_2 + MC_1;     // 定时器A时钟源为SMCLK,增计数模式
}
void start()    //启动
{
    P1SEL |= 0x04;  //P1.2第二功能 TA1 输出PWM
    TACTL= TASSEL_2 + MC_1; //定时器A时钟源为SMCLK，增计数模式
    STATUS = 1;
}

void stop()    //停止
{
    P1SEL = 0x0;    //P1.2第二功能 TA1 输出PWM
    TACTL= MC_0;
    P1OUT &=  ~BIT2;
    STATUS = 0;
}

void speed_add()    //加速
{
    if(a <= 20000)
        a += 1000;
}

void speed_sub()    //减速
{
    if(a >= 12000)
        a -= 1000;
}

