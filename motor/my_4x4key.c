/*
 * 4x4key.c
 *
 *  Created on: 2018年10月11日
 *      Author: Jack
 */
#include<msp430f169.h>
#include<my_4x4key.h>
#include"type.h"
#include "delay.h"

extern char keycodes[16];

unsigned char keyscan(void)
{
    unsigned char keyno=255;

    P2DIR=0Xf0;//高四位输出，低四位输入
    P2OUT=0xef;        //扫描第一行
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

    P2OUT=0xDf;       //扫描第二行
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

    P2OUT=0xBf;       //扫描第三行
    switch(P2IN&0x0f)
    {
    case 0x0e:  keyno=9;
        break;
    case 0x0d:  keyno='a';
        break;
    case 0x0b:  keyno='a';
        break;
    case 0x07:  keyno='a';
        break;
    }

    P2OUT=0x7f;       //扫描第四行
    switch(P2IN&0x0f)
    {
    case 0x0e:  keyno='a';
        break;
    case 0x0d:  keyno='a';
        break;
    case 0x0b:  keyno='a';
        break;
    case 0x07:  keyno='a';
        break;
    }

    return keyno;
}

//void key_init()
//{
//    P2SEL = 0;
//    P2DIR = 0x0f;
//}
//
//uchar read_key()
//{
//    char row, col;  //行列变量 b
//    char mask;
//    char index;
//    char maskrow=1;
//    char key=255;    //无效键值
//    P2OUT = 0x00;
//    switch(key_state)
//    {
//	case key_state_init:
//	    maskrow = 1;
//	    if((P2IN & 0xf0) != 0xf0)
//	        key_state = key_state_press;
//	    break;
//	case key_state_press:
//	    if((P2IN & 0xf0) != 0xf0)
//	    {
//	        for(row=0; row < 4; row++, maskrow+=maskrow)  //逐行输出低电平
//	        {
//	            P2OUT = ~maskrow;   //1~1111 1110 2~1111 1101 4~1111 1011 8~1111 0111
//	            mask=1;
//	            for(col=0; col < 4; ++col, mask+=mask)
//	            {
//	                if((~((P2IN&0xf0)>>4) & mask) != 0)
//	                {
//	                    index=(row*4+col);
//	                    key = keycodes[index];
//	                    goto DONE;
//	                }
//	            }
//	        }
//	        DONE:
//		        key_state = key_state_bounce;
//	    }
//	    else
//	        key_state=key_state_init;
//	    break;
//	case key_state_bounce:
//	    P2OUT = 0x00;
//	    if((P2IN&0xf0) == 0xf0)
//	        key_state = key_state_init;
//	    break;
//	default: key_state = key_state_init;
//	    break;
//    }
//
//    return key;
//}
