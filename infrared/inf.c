/*
 * inf.c
 *
 *  Created on: 2018��11��9��
 *      Author: Jack
 */
#include"inf.h"
extern uchar start;
extern uchar ircode[4];
extern uchar data[33];
extern uchar VALUE;

void infrared_decode()
{
    if(start == 1)    //���ɹ����յ�һ������
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

void init_port1()  //����P1�˿ڵ��ж�����
{
    P1DIR &= ~BIT0;     //����Ϊ���뷽��
    P1IES |= BIT0;      //ѡ���½��ش���
    P1IE |= BIT0;       //���ж�����
    P1IFG = ~BIT0;      //P1IES���л�����ʹP1IFG��λ�������
}

void init_device()
{
    init_port1();
    TACCTL0 = CCIE;            //CCR0�ж�ʹ��
    TACCR0 = 33;               //�����յ�
    TACTL = TASSEL_2 + MC_1;   //TIMER_Aѡ��timerʱ��MCLK��������ģʽ
    BCSCTL2 |= SELS;
    _EINT();   //�������ж�
}



