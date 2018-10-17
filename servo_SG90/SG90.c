/*
 * SG90.c
 *
 *  Created on: 2018Äê10ÔÂ15ÈÕ
 *      Author: Jack
 */
#include<msp430f169.h>
extern unsigned int a;
void key_process1(void)
{
    P1SEL |= 0x04;
    TACTL = TASSEL_2 + MC_1;
}

void key_process2(void)
{
    P1SEL = 0x00;
    TACTL = MC_0;
    P1OUT &= ~BIT2;
}
void key_process3(void)
{
    a+=100;
    if(a >= 2500)
        a = 2500;
}

void key_process4(void)
{
    a -= 100;
    if(a <= 1500)
        a = 500;
}

void key_process5(void)
{
    a=1500;
}

