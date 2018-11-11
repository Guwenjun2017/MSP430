/*
 * lcd_1602.c
 *
 *  Created on: 2018年10月6日
 *      Author: Jack
 */
#include<msp430f169.h>
#include"lcd_1602.h"
#include"delay.h"
/*
 * 名    称：write_cmd_1602()
 * 功    能：1602写命令
 * 参    数：unsigned char cmd
 * 返回值：无
 */
void write_cmd_1602(unsigned char cmd)
{
    clear_RW;
    clear_RS;

    set_E;
    P4OUT = cmd;
    delay_ms(1);

    clear_E;
}

/*
 * 名    称：write_data_1602()
 * 功    能：1602写数据
 * 参    数：unsigned char data
 * 返回值：无
 */
void write_data_1602(unsigned char data)
{
    clear_RW;
    set_RS;

    set_E;
    P4OUT = data;
    delay_ms(1);

    clear_E;
}
/*
 * 名    称：lcd1602_init()
 * 功    能：初始化lcd1602
 * 参    数：无
 * 返回值：无
 */
void lcd1602_init()
{
    write_cmd_1602(0x38);
    delay_ms(10);
    write_cmd_1602(0x0c);
    delay_ms(10);
    write_cmd_1602(0x06);
    delay_ms(10);
    write_cmd_1602(0x01);
    delay_ms(10);
}
/*
 * 名    称：dis_str()
 * 功    能：显示字符串
 * 参    数：1、lcd1602显示起始地址  2、字符串首地址
 * 返回值：无
 */
void dis_str(unsigned char addr, unsigned char *str)
{
    while(*str)
    {
        write_cmd_1602(addr);
        write_data_1602(*str);
        addr++;
        str++;
    }
}
/*
 * 名    称：dis_num()
 * 功    能：显示数字
 * 参    数：unsigned long int num
 * 返回值：无
 */
void dis_num(unsigned long int num)
{
    unsigned char k;
    unsigned char dis_flag = 0;
    unsigned char table[7];

    if(num == 0)
    {
        write_data_1602('0');
    }
    else if(num<=9 & num>0)
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
        write_data_1602(table[k]);
        delay_ms(1);
    }
}
