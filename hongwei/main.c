#include<msp430f169.h>
#include"lcd_1602.h"
#include"Port.h"
#include"type.h"
#include"clock.h"
#include"delay.h"

uint count;
uchar a;
uchar BIT;
uchar start;
uchar data[33];
uchar ircode[4];
void init_port1();
void init_device(void)



void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    init_clk_1M();
    init_1602_port();
    lcd1602_init();
    init_device();
    _EINT();
    while(1)
    {
        if(start == 1)
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
            uchar result=ircode[2];
            write_cmd(0x80);
            display(result);
        }
    }
}


void init_port1()
{
    P1DIR &= ~BIT0;     //����Ϊ���뷽��
    P1IES |= BIT0;      //ѡ���½��ش���
    P1IE |= BIT0;       //���ж�����
    P1IFG = ~BIT0;      //P1IES���л�����ʹP1IFG��λ�������
}

void init_device(void)
{
    init_port1();
    TACCTL0 = CCIE;            //CCR0�ж�ʹ��
    TACCR0 = 33;               //�����յ�
    TACTL = TASSEL_2 + MC_1;   //TIMER_Aѡ��timerʱ��SMCLK����������ģʽ
    BCSCTL2 |= SELS;
    _EINT();   //�������ж�
}

#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
    if(count>32)
    {
        BIT=0;
    }
    data[BIT] = count;
    count = 0;
    BIT++;
    if(BIT==33)
    {
        BIT=0;
        start=1;
    }
    P1IFG = 0;
}

#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A(void)
{
    a++;
    if(a == 8)
    {
        a = 0;
        count++;
    }
}
