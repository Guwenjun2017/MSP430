#include<msp430f169.h>
#include"inf.h"
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
        get_value(ircode[2]);
        write_cmd(0x80+0x40);
        display(VALUE);
    }
}

#pragma vector=PORT1_VECTOR    //�˿�P1�ж�
__interrupt void Port1_ISR(void)
{
    if(count>32)        //��һ�ν���ʱ�ж�count������>32Ϊ�����룬
    {
        BIT=0;
    }
    data[BIT] = count;  //ÿ�δ洢count��������ӻ��������ʱ����
    count = 0;          //��������count
    BIT++;
    if(BIT==33)         //����33�κ󣨼�33�������ش����󣩣���������BIT������start��־��λ
    {
        BIT=0;
        start=1;
    }
    P1IFG = 0;          //����жϱ�־λ
}

#pragma vector=TIMERA0_VECTOR     //TIMERA�жϣ��ڴﵽ��ʱҪ��ʱ�Ż�����ж�
__interrupt void Timer_A(void)    //�˴�����CCR0Ϊ33����1MHz�£�ԼΪ33us����һ���ж�
{
    a++;                          //һ���жϣ�a����һ��
    if(a == 8)                    //a����8�Σ�������Լ264us�����㣬count++
    {
        a = 0;
        count++;
    }
}
