#include <msp430f169.h>
#include "SG90.h"
#include "my_4x4key.h"
#include "lcd_1602.h"
unsigned char key_value;
unsigned int a=1500;
void key_check(void);
/**
 * main.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	BCSCTL2 = SELS + DIVS0 + DIVS1;
	P1DIR = 0xff;
	P1OUT &= ~BIT2;
	CCR0 = 20000;            //PWM����
	CCTL1 = OUTMOD_7;        //CCR1��λ/��λ
	CCR1 = a;             //CCR1 PWMռ�ձ�5%
	TACTL = TASSEL_2 + MC_1; //TASSEL_2-->TimerAʱ��ԴΪSMCLK=8MHz��MC_1-->������ģʽ

	P2DIR &= BIT0+BIT1+BIT2+BIT3;            //P2.0~2.3����Ϊ����
	P2IES |= BIT0+BIT2+BIT2+BIT3;            //P2.0~2.3�˿��½��ش����ж�
	P2IE |= BIT0+BIT2+BIT2+BIT3;             //�ж�ʹ��
	
	P2IFG = 0x00;   //P2.0~2.3�˿��жϱ�־λ����
	_EINT();
	lcd1602_init();
	while(1)
	{
	    CCR1 = a;
	    key_check();
	    //key_value = keyscan();
	    switch(key_value)
	    {
	    case 1: key_process1();
	        break;
	    case 2: key_process2();
	        break;
	    case 3: key_process3();
	        break;
	    case 4: key_process4();
	        break;
	    case 5: key_process5();
	        break;
	    default: break;
	    }
	    key_value = 0x00;
	    P2IE |= BIT0+BIT2+BIT2+BIT3;             //�ж�ʹ��
	    P2IFG = 0x00;   //P2.0~2.3�˿��жϱ�־λ����
	    write_cmd(0x80);
	    write_data(key_value);
	}
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    switch(P2IFG)
    {
    case 0x01: key_value=1;
        break;
    case 0x02: key_value=2;
        break;
    case 0x04: key_value=3;
        break;
    case 0x08: key_value=4;
        break;
    case 0x10: key_value=5;
        break;
    default: P2IFG = 0x00;
        break;
    }
    P2IFG = 0x00;
}

void key_check(void)
{
    unsigned int i;
    for(i = 0; i<200; i++);
    if(0xff != (P2IN & 0xe0))
    {
        while(0xff != (P2IN | 0xe0));
    }
    else
        key_value=0x00;
}
