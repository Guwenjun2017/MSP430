#include<msp430f169.h>
#include"lcd_1602.h"
#include"Port.h"
#include"type.h"
#include"clock.h"
#include"delay.h"

uint count;              //TIMERA�ж��б���
uchar a;                 //TIMERA�ж��б���
uchar BIT;               //��������λ��
uchar start;             //���յ����ݵı�־λ
uchar data[33];          //һλ������+32λ����
uchar ircode[4];         //�洢�����û��롢һ�������롢һ��������ķ���
uchar VALUE;             //ȫ�ּ�ֵ
void init_port1();       //����P1.0�˿�
void init_device();      //��ʼ��TIMERA����
void infrared_decode();  //������뺯��

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;  //�ع�
    init_clk_1M();          //��ʼ��ʱ�ӣ�8��Ƶ=>1MHz��Ƶ
    init_1602_port();       //����1602�˿�
    lcd1602_init();         //��ʼ��1602
    init_device();          //��ʼ����������豸
    _EINT();                //�������ж�
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

#pragma vector=PORT1_VECTOR    //�˿�P1�ж�
__interrupt void Port1_ISR(void)
{
    if(count>32)       //��һ�ν���ʱ�ж�count������>32Ϊ�����룬
    {
        BIT=0;
    }
    data[BIT] = count; //ÿ�δ洢count��������ӻ��������ʱ����
    count = 0;         //��������count
    BIT++;
    if(BIT==33)    //����33�κ󣨼�33�������ش����󣩣���������BIT������start��־��λ
    {
        BIT=0;
        start=1;
    }
    P1IFG = 0;    //����жϱ�־λ
}

#pragma vector=TIMERA0_VECTOR     //TIMERA�жϣ��ڴﵽ��ʱҪ��ʱ�Ż�����ж�
__interrupt void Timer_A(void)    //�˴�����CCR0Ϊ33����1MHz�£�ԼΪ33us����һ���ж�
{
    a++;           //һ���жϣ�a����һ��
    if(a == 8)     //a����8�Σ�������Լ264us�����㣬count++
    {
        a = 0;
        count++;
    }
}
