#include <msp430f169.h>
#include"type.h"
#include"clock.h"
#include"delay.h"
#include"nRF24L01.h"
#include"Port.h"
#include"lcd_1602.h"

char TX_ADDRESS[5]={0x34,0x43,0x10,0x10,0x01};
char RX_ADDRESS[5]={0x34,0x43,0x10,0x10,0x01};
/**
 * main.c
 */
#include<stdlib.h>
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	init_clk();
	//init_1602_port();
    init_nRF_port();

	//lcd1602_init();
	init_nRF24L01();
	char *rxbuf=(char *)malloc(sizeof(char)*32);
	char status1;
	P6DIR = 0xff;
	while(1)
	{
	    //status1 = 0;
	    set_RXmode();
	    status1 = nRF24L01_RX(rxbuf);
	    P6OUT = status1;
//	    if(status1)
//	    {
//	        write_cmd(0x80);
//	        for(i=0; i<16; i++)
//	        {
//	            write_data(rxbuf[i]);
//	        }
//	        write_cmd(0x80+0x40);
//	        for(i=16; i<32; i++)
//	        {
//	            write_data(rxbuf[i]);
//	        }
//	    }
	    delay_ms(500);
	}

}
