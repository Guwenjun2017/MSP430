#include <msp430f169.h>
#include "meimeidexin.h"
#include<stdio.h>
#include<stdlib.h>

/**
 * main.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P6SEL = 0x00;
	P6DIR = 0xff;
	P6OUT = 0xff;
	
	unsigned char meimeixin[] = {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};
	unsigned char i,j;
	while(1)
	{
	    for(i = 0; i < 8; i++)
	    {
	        j = rand()%8;
	        P6OUT = meimeixin[j];
	        delay_ms(5);
	    }
//	    for(i = 7; i > 0; i--)
//	    {
//	        P6OUT = meimeixin[i];
//	        delay_ms(5);
//	    }
	}
}
