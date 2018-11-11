/*
 * Ӳ������
 *          MSP430F149
 *       -----------------
 *      |                 |        -------
 *      |             P1.0|------>|S(OUT) |
 *      |                 |       |  ����   |
 *      |                 |       |    VCC|--->5V
 *      |                 |       |       |
 *                                |    GND|
 * ����
 *      ����ң�������²�ͬ�İ�����1602��һ����ʾ��ͬ���������ͣ��ڶ���
 *      ��ʾ�������õĶ�Ӧ�ļ�ֵ��������ֻ������ң���������а����ļ�ֵ��
 */
#include<msp430f169.h>
#include"inf.h"                  //����ͷ�ļ�
#include"lcd_1602.h"             //lcd1602ͷ�ļ�
#include"Port.h"                 //�˿�����
#include"clock.h"                //ʱ������
#include"delay.h"                //�����ӳ�

unsigned int count;              //TIMERA�ж��б���
unsigned char a;                 //TIMERA�ж��б���
unsigned char BIT;               //��������λ��
unsigned char RECV_FLAG;         //���յ����ݵı�־λ
unsigned char data[33];          //һλ������+32λ���ݵ�����
unsigned char ircode[4];         //�洢�����û��롢һ�������롢һ��������ķ���
unsigned char VALUE;             //ȫ�ּ�ֵ

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;    //�ع�

    init_clk_1M();               //��ʼ��ʱ�ӣ�8��Ƶ=>1MHz��Ƶ
    init_1602_port();            //����1602�˿�
    init_port1();                //��ʼ���ⲿ�ж�P1.0

    lcd1602_init();              //��ʼ��1602
    init_device();               //��ʼ����������豸��TIMERA
    _EINT();                     //�������ж�

    while(1)
    {
        infrared_decode();       //�������
        set_value(ircode[2]);    //���ø��������ļ�ֵ
        write_cmd_1602(0x80+0x40);    //1602�ڶ���
        dis_num(VALUE);          //��ʾ���õļ�ֵ
    }
}

#pragma vector=PORT1_VECTOR      //�˿�P1�ж�
__interrupt void Port1_ISR(void)
{
    if(count>32)                 //��һ�ν���ʱ�ж�count������>32Ϊ�����룬
    {
        BIT=0;
    }
    data[BIT] = count;           //ÿ�δ洢ÿλ���ݳ�����count������һ��countΪ8*33=264us����ӻ���½���ʱ����
    count = 0;                   //��������count����
    BIT++;
    if(BIT==33)                  //����33�κ󣨼�33���½��ش����󣩣���������BIT������start��־��λ
    {
        BIT=0;
        RECV_FLAG=1;
    }
    P1IFG = 0;                   //����жϱ�־λ
}

#pragma vector=TIMERA0_VECTOR     //TIMERA�жϣ��ڴﵽ��ʱҪ��ʱ�Ż�����ж�
__interrupt void Timer_A(void)    //�˴�����CCR0Ϊ33����1MHz�£�ԼΪ33us����һ���ж�
{
    a++;                          //һ���жϣ�a����һ��
    if(a == 8)                    //a����8�Σ�������Լ33*8=264us�����㣬count++
    {
        a = 0;
        count++;
    }
}
