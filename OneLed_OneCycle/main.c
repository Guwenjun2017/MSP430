#include <msp430f169.h>
#include <stdio.h>
#include "delay.h"

unsigned int cycle[] = {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};
/**
 * main.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	P1DIR &= ~BIT0;      //定义P1.0为输入
	P1DIR &= ~BIT1;      //定义P1.1为输入
	P6DIR |= BIT0;       //定义P6.0为输出
	P6OUT |= BIT0;       //定义P6.0为高电平

	while(1)
	{
	    if(P1IN&BIT0)    //若P1.0为1，即未按下时
	        P6OUT |= BIT0;    //P6.0仍为高电平
	    else
  	        P6OUT &= ~BIT0;   //按下时，改变P6.0为低电平
	    if(P1IN&BIT1)    //若P1.1为1，即未按下时
	        ;
	    else             //按下，执行流水灯程序
	    {
	        unsigned char i;
	        for(i = 0; i < 10;i++)
	        {
	            unsigned char j
	            ;

	            P6DIR |= 0xff;
	            for(j = 0; j < 8; j++)
	            {
	                P6OUT = cycle[j];
	                delay_ms(300);
	            }
	            for(j = 6; j > 0; j--)
	            {
	                P6OUT = cycle[j];
	                delay_ms(100);

	            }
	        }
	    }
	    P6OUT = 0xff;
	}
}
