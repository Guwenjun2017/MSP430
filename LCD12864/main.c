#include <msp430f169.h>
#include "delay.h"
#include "type.h"
#include "LM1602and12864.h"

uchar tab0[] = "九江学院";             //中文打印时循环次数=汉字数X2
uchar tab1[] = "电子工程学院";
uchar tab2[] = "ID:2016";
uchar tab3[] = "url: www";



void Clk_init()
{
    uint i;
    BCSCTL1 &= ~XT2OFF;
    do{
        IFG1 &= ~OFIFG;
        for(i=0xff; i>0; i--);
    }while((IFG1&OFIFG));
    BCSCTL2 |= SELM_2 + SELS;
}

void Port_init()
{
    P5SEL = 0x00;
    P5DIR |= BIT0 + BIT1 + BIT5 + BIT6 +BIT7;
    P4SEL = 0x00;
    dataout;
    PSB_set;
    RET_set;
}

void write(uchar argu)
{
    RW_clr;
    E_set;

    dataport = argu;
    delay_ms(5);
    delay_ms(5);
    E_clr;
}
void write_cmd(uchar cmd)
{
    RS_clr;
    write(cmd);
}

void write_data(uchar data)
{
    RS_set;
    write(data);
}

void lcd_pos(uchar x, uchar y)
{
    uchar position;
    switch(x){
        case 0: x = 0x80; break;     //确定起始行
        case 1: x = 0x90; break;
        case 2: x = 0x88; break;
        case 3: x = 0x98; break;
    }

    position = x + y;
    write_cmd(position);
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

//void display_str(uchar *str)
//{
//    uint i = 0;
//    int len = 0;
//    while(*str)
//    {
//        if(~(*str >> 8) == 0)  //
//        {
//            str+=1;      //
//            len+=2;
//        }
//        else
//        {
//            str++;
//            len+=1;
//        }
//    }
//    for(; i<len; i++)
//    {
//        write_data(str[i]);
//        delay_ms(5);
//    }
//}

//void main(void)
//{
//    WDTCTL = WDTPW | WDTHOLD;
//    Clk_init();
//
//    Port_init();
//    Lcd_init();
//    while(1)
//    {
//        lcd_pos(0, 0);
//        delay_ms(5);
//        display_str(tab0);
//
//        lcd_pos(1,0);
//        delay_ms(5);
//        display_str(tab1);
//
//        lcd_pos(2,0);
//        delay_ms(5);
//        display_str(tab2);
//
//        lcd_pos(3,0);
//        delay_ms(5);
//        display_str(tab3);
//    }
//}
void main(void)
{
     WDTCTL = WDTPW | WDTHOLD;
    Clk_init();

    Port_init();
    Lcd_init();
    while(1)
    {
        uint i;
        lcd_pos(0, 0);
        delay_ms(5);
        for(i=0; i<8; i++)
        {
            write_data(tab0[i]);
            delay_ms(5);
        }
        lcd_pos(1,0);
        delay_ms(5);
        for(i=0; i<12; i++)
        {
            write_data(tab1[i]);
            delay_ms(5);
        }
        lcd_pos(2,0);
        delay_ms(5);
        for(i=0; i<7; i++)
        {
            write_data(tab2[i]);
            delay_ms(5);
        }
        lcd_pos(3,0);
        delay_ms(5);
        for(i=0; i<8; i++)
        {
            write_data(tab3[i]);
            delay_ms(5);
        }
    }
}
