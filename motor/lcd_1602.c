#include<msp430f169.h>
#include"type.h"
#include"lcd_1602.h"
#include"delay.h"
/*
 * lcd_1602.c
 *
 *  Created on: 2018年10月6日
 *      Author: Jack
 */
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

void Write1602(uchar addr, uchar data)
{
    write_cmd(addr);
    write_data(data);
}

void lcd1602_init()
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

void dis_str(uchar addr, uchar *str)
{
    while(*str)
    {
        Write1602(addr, *str);
        addr++;
        str++;
    }
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
