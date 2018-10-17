#include <msp430f169.h>
#include "delay.h"
#include "clock.h"
#include "type.h"
//RS端口
#define set_RS P5OUT |= BIT5
#define clear_RS P5OUT &= ~BIT5
//RW端口
#define set_RW P5OUT |= BIT6
#define clear_RW P5OUT &= ~BIT6
//Enable端口
#define set_E P5OUT |= BIT7
#define clear_E P5OUT &= ~BIT7
//数据端P4.0~P4.7
#define dataout P4DIR = 0xff  //将P4端口全输出
#define dataport P4OUT

long data0, data1;
uchar tab0[] = "P6.0: ";
uchar tab1[] = "P6.1: ";

//写'命令'函数
void write_cmd(uchar cmd)
{
    clear_RW;
    clear_RS;

    set_E;
    P4OUT = cmd;
    delay_ms(1);

    clear_E;
}

//写'数据'函数
void write_data(uchar data)
{
    clear_RW;
    set_RS;

    set_E;
    P4OUT = data;
    delay_ms(1);

    clear_E;
}

void lcd_init()
{
    write_cmd(0x38);
    delay_ms(10);
    write_cmd(0x0c);
    delay_ms(10);
    write_cmd(0x06);
    delay_ms(10);
    write_cmd(0x01);
    delay_ms(10);
}

void display(unsigned long int num)
{
    uchar k;
    uchar dis_flag = 0;
    uchar table[7];

    if(num<=9 & num>0)
    {
        dis_flag = 1;
        table[0] = num%10 + '0';
    }
    else if(num<=99 & num>9)
    {
        dis_flag = 2;
        table[0] = num/10 + '0';
        table[1] = num%10 + '0';
    }
    else if(num<=999 & num>99)
    {
        dis_flag = 3;
        table[0] = num/100 + '0';
        table[1] = num/10 % 10 + '0';
        table[2] = num%10 + '0';
    }
    else if(num<9999 & num>999)
    {
        dis_flag = 4;
        table[0] = num/1000 + '0';
        table[1] = num/100 % 10 + '0';
        table[2] = num/10 % 10 + '0';
        table[3] = num%10 + '0';
    }
    for(k = 0; k < dis_flag; k++)
    {
        write_data(table[k]);
        delay_ms(1);
    }
}

void dis_str(uchar *str)
{
    while(*str)
    {
        write_data(*str);
        str++;
    }
}

/**
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    init_clk();

    P4SEL = 0x00;
    P4DIR = 0xff;
    P5SEL = 0x00;
    P5DIR |= BIT5 + BIT6 + BIT7;
    P6SEL |= 0x06;         //P6.1 P6.2双通道

    lcd_init();
    delay_ms(5);

    ADC12CTL0 = ADC12ON + SHT0_8 + MSC;
    ADC12CTL1 = SHP + CONSEQ_3;
    ADC12MCTL1 = INCH_1;
    ADC12MCTL2 = INCH_2 + EOS;
    ADC12MCTL0 = SREF_2;
    delay_ms(3);
    ADC12CTL0 |= ENC;

    while(1)
    {
        ADC12CTL0 |= ADC12SC;
        delay_ms(5);
        while((ADC12IFG & BIT1) == 0);
        data0 = (long)ADC12MEM1*33;
        data0 = data0 * 10;
        data0 = data0/4096;
        while((ADC12IFG & BIT2) == 0);
        data1 = (long)ADC12MEM2*33;
        data1 = data1 * 10;
        data1 = data1/4096;

        write_cmd(0x80);
        dis_str(tab0);
        write_data(data0/100 + '0');
        write_data('.');
        write_data(data0/10%10 + '0');
        write_data(data0%10 + '0');
        write_data('V');

        write_cmd(0x80 + 0x40);
        dis_str(tab1);
        write_data(data1/100 + '0');
        write_data('.');
        write_data(data1/10%10 + '0');
        write_data(data1%10 + '0');
        write_data('V');
    }
}

