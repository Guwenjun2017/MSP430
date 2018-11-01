/*
 * 4x4key.c
 *
 *  Created on: 2018��10��11��
 *      Author: Jack
 */
#include<msp430f169.h>
#include "delay.h"

unsigned char keyscan(void)
{
    unsigned char keyno=16;
    delay_ms(5);

    P2DIR=0Xf0;//����λ���������λ����
    P2OUT=0xef;        //ɨ���һ��
    switch(P2IN&0x0f)
    {
    case 0x0e:  keyno=1;
       break;
    case 0x0d:  keyno=2;
       break;
    case 0x0b:  keyno=3;
       break;
    case 0x07:  keyno=4;
       break;
    }
    P2OUT=0xDf;       //ɨ��ڶ���
    switch(P2IN&0x0f)
    {
    case 0x0e:  keyno=5;
        break;
    case 0x0d:  keyno=6;
        break;
    case 0x0b:  keyno=7;
        break;
    case 0x07:  keyno=8;
        break;
    }
    P2OUT=0xBf;       //ɨ�������
    switch(P2IN&0x0f)
    {
    case 0x0e:  keyno=9;
        break;
    case 0x0d:  keyno=10;
        break;
    case 0x0b:  keyno=11;
        break;
    case 0x07:  keyno=12;
        break;
    }
    P2OUT=0x7f;       //ɨ�������
    switch(P2IN&0x0f)
    {
    case 0x0e:  keyno=13;
        break;
    case 0x0d:  keyno=14;
        break;
    case 0x0b:  keyno=15;
        break;
    case 0x07:  keyno=16;
        break;
    }
    delay_ms(5);

    return keyno;
}
