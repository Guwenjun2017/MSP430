#include <msp430f169.h>
#include"type.h"
#include"clock.h"
#include"delay.h"
#include"nRF24L01.h"
#include"Port.h"

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
	init_nRF_port();
	init_nRF24L01();
	while(1)
	{
	    char *txbuf=(unsigned char*)malloc(sizeof(char)*32);
	    for(txbuf[31]=0;txbuf[31]<32;txbuf[31]++){
	        txbuf[txbuf[31]]=txbuf[31];
	    }
	    //send
	     nRF24L01_TX(txbuf);
//	    for(txbuf[31]=0;txbuf[31]<32;txbuf[31]++){
//	        SPI_write(txbuf[txbuf[31]]);
//	        delay_us(100);
//	    }
	    //txbuf[0] = 0;
	}

}
