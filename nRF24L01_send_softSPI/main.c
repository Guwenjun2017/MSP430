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

    char tx_buf[32]="01234567890123456789012345678901";
    //char *rx_buf=(unsigned char *)malloc(sizeof(unsigned char)*32);
    P6DIR=0xff;
	while(1)
	{
	     nRF24L01_TX(tx_buf);
	     P6OUT=SPI_read(STATUS);
	     delay_ms(100);
	}

}
