/*
 * SG90.c
 *
 *  Created on: 2018��10��15��
 *      Author: Jack
 */
#include<msp430f169.h>
#include"lcd_1602.h"
extern unsigned int a;
extern unsigned char STATUS;

void Init_PWM()
{
      BCSCTL2 |= SELS ;                 //SMCLK = XT2
      BCSCTL2 = DIVS0 + DIVS1;
      P1DIR = 0xff;               // P1���
      P1OUT &= ~BIT2;
      CCR0 = 20000;                // PWM ����
      CCTL1 = OUTMOD_7;            // CCR1��λ/��λ
      CCR1 = a;                // CCR1 PWM ռ�ձ�50%
      TACTL = TASSEL_2 + MC_1;     // ��ʱ��Aʱ��ԴΪSMCLK,������ģʽ
}
void start()    //����
{
    P1SEL |= 0x04;  //P1.2�ڶ����� TA1 ���PWM
    TACTL= TASSEL_2 + MC_1; //��ʱ��Aʱ��ԴΪSMCLK��������ģʽ
    STATUS = 1;
}

void stop()    //ֹͣ
{
    P1SEL = 0x0;    //P1.2�ڶ����� TA1 ���PWM
    TACTL= MC_0;
    P1OUT &=  ~BIT2;
    STATUS = 0;
}

void speed_add()    //����
{
    if(a <= 20000)
        a += 1000;
}

void speed_sub()    //����
{
    if(a >= 12000)
        a -= 1000;
}
void show_status(unsigned char status, unsigned int speed)
{
    if(STATUS == 1){
        dis_str(0x80+8, "RUN ");
    }
    else
        dis_str(0x80+8, "STOP");
    write_cmd(0x80+0x40+8);
    display(speed-10000);   //1602�۲����
}
