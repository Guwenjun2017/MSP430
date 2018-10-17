#include <msp430f169.h>
#include "type.h"
#include "delay.h"
#include "LM1602and12864.h"

uchar tab0[] = "九江学院";
uchar tab1[] = "电子工程学院";
uchar tab2[] = "ID:2016";
uchar tab3[] = "url: www";


void Clk_init()
{
    uint i;
    BCSCTL1 &= ~XT2OFF;
    do{
        IFG1 &= ~OFIFG;
        for(i = 0xff; i>0; i--);
    }while((IFG1&OFIFG));
    BCSCTL2 |= SELM_2 + SELS;
}

void Port_init()
{
    P5SEL = 0x00;
    P5DIR |= BIT0 + BIT1 + BIT5 +BIT6 + BIT7;
    P5OUT = 0xff;
//    P4SEL = 0x00;
//    dataout;
    PSB_clr;
    //RET_set;
}

void send_byte(uchar data)
{
    uint i;
    SCLK_clr;
    for (i=0; i<8; i++)
    {
        if((data<<i) & 0x80)
        {
            SID_set;
        }
        else
        {
            SID_clr;
        }
        delay_ms(5);
        //SCLK_clr;
        delay_ms(5);
        SCLK_set;                //一个上升沿，传一个数据
        delay_ms(5);
    }
}

void write_cmd(uchar cmd)
{
    CS_clr;
    send_byte(0xf8);           //写指令
    send_byte(cmd&0xf0);
    send_byte((cmd<<4)&0xf0);
}

void write_data(uchar data)
{
    CS_set;
    send_byte(0xf8);
    send_byte(data&0xf0);
    send_byte((data<<4)&0xf0);

}

void lcd_pos(uchar x, uchar y)
{
    uchar position;
    switch(x){
        case 0: x = 0x80; break;
        case 1: x = 0x90; break;
        case 2: x = 0x88; break;
        case 3: x = 0x98; break;
    }
    position = x + y;
    write_cmd(position);
}

void display(uchar *string)
{
    while(*string)
    {
        write_data(*string);
        string++;
    }
}

void Lcd_init()
{
    write_cmd(0x30);
    delay_ms(5);
    write_cmd(0x0c);
    delay_ms(5);
    write_cmd(0x01);
    delay_ms(5);
    write_cmd(0x02);
    delay_ms(5);
}
/**
 * main.c
 */

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Clk_init();
	Port_init();
	Lcd_init();

	while(1)
	{
	    lcd_pos(0, 0);
	    display(tab0);
        lcd_pos(1, 0);
        display(tab1);
        lcd_pos(2, 0);
        display(tab2);
        lcd_pos(3, 0);
        display(tab3);
	}
}
