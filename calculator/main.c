
#include <msp430f169.h>
#include <my_4x4key.h>
#include "delay.h"
#include "type.h"
#include "clock.h"
#include "lcd_1602.h"
//uchar keyvalue;
char disp_buff[MAX_length];    //显示缓存区

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
    P2DIR=0Xf0;        //高四位输出，低四位输入
    P2OUT=0xef;        //扫描第一行

    calculator();
}
