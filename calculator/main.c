
#include <msp430f169.h>
#include <my_4x4key.h>
#include "delay.h"
#include "type.h"
#include "clock.h"
#include "lcd_1602.h"
//uchar keyvalue;
char disp_buff[MAX_length];    //��ʾ������

/**
 * main.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    init_clk();

    P4SEL = 0x00;
    P4DIR = 0xff;
    P5DIR |= BIT0 + BIT1 + BIT2 + BIT5 + BIT6 + BIT7;

    lcd1602_init();
    P2DIR=0Xf0;        //����λ���������λ����
    P2OUT=0xef;        //ɨ���һ��

    calculator();
}
