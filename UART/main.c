#include <msp430f169.h>
#include "delay.h"
#define uchar unsigned char
#define uint unsigned int
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

void init()
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
/**
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    
    //init_clk();
    TACCTL0 = CCIE;   //CCRO中断使能
    TACCR0 = 32768;
    TACTL = TASSEL_1 + MC_1;
    _EINT();

    P4SEL = 0x00;
    P4DIR = 0xff;
    P5SEL = 0x00;
    P5DIR |= BIT5 + BIT6 + BIT7;
    init();

    P3SEL |= 0x30;
    ME1 |= UTXE0 + URXE0;
    UCTL0 = CHAR;
    UTCTL0 |= SSEL0;
    UBR00 = 0x03;
    UBR10 = 0x00;
    UMCTL0 = 0x4A;
    UCTL0 &= ~SWRST;
    IE1 |= URXIE0;
    IFG1 &= ~URXIFG0;
    _ENIT();
    while(1)
    {
        while(!(IFG1 & URXIFG0));
        write_cmd(0x80);
        display(time1);
    }

}

#pragma vector = UART0RX_VECTOR
__interrupt void UART0_RXISR(void)
{
    data = RXBUF0;
    time1 = data;
    IFG1 &= ~URXIFG0;
}
