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

	P1DIR &= ~BIT0;      //����P1.0Ϊ����
	P1DIR &= ~BIT1;      //����P1.1Ϊ����
	P6DIR |= BIT0;       //����P6.0Ϊ���
	P6OUT |= BIT0;       //����P6.0Ϊ�ߵ�ƽ

	while(1)
	{
	    if(P1IN&BIT0)    //��P1.0Ϊ1����δ����ʱ
	        P6OUT |= BIT0;    //P6.0��Ϊ�ߵ�ƽ
	    else
  	        P6OUT &= ~BIT0;   //����ʱ���ı�P6.0Ϊ�͵�ƽ
	    if(P1IN&BIT1)    //��P1.1Ϊ1����δ����ʱ
	        ;
	    else             //���£�ִ����ˮ�Ƴ���
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
