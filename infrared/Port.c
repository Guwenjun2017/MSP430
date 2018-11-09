/*
 * Port.c
 *
 *  Created on: 2018Äê10ÔÂ28ÈÕ
 *      Author: Jack
 */
#include<msp430f169.h>

void init_1602_port()
{
    P4DIR = 0xff;
    P4OUT = 0x00;
    P5DIR |= BIT5 + BIT6 + BIT7;
    P5OUT |= BIT5 + BIT6 + BIT7;
}

void init_nRF_port()
{
    P1DIR |= BIT6;
    P1DIR &= ~BIT7;
    P3DIR |= BIT3 + BIT0 + BIT1;
    P3DIR &= ~BIT2;
}



