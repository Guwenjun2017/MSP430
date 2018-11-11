/*
 * lcd_1602.c
 *
 *  Created on: 2018��10��6��
 *      Author: Jack
 */
#include<msp430f169.h>
#include"lcd_1602.h"
#include"delay.h"
/*
 * ��    �ƣ�write_cmd_1602()
 * ��    �ܣ�1602д����
 * ��    ����unsigned char cmd
 * ����ֵ����
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
 * ��    �ƣ�write_data_1602()
 * ��    �ܣ�1602д����
 * ��    ����unsigned char data
 * ����ֵ����
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
 * ��    �ƣ�lcd1602_init()
 * ��    �ܣ���ʼ��lcd1602
 * ��    ������
 * ����ֵ����
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
 * ��    �ƣ�dis_str()
 * ��    �ܣ���ʾ�ַ���
 * ��    ����1��lcd1602��ʾ��ʼ��ַ  2���ַ����׵�ַ
 * ����ֵ����
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
 * ��    �ƣ�dis_num()
 * ��    �ܣ���ʾ����
 * ��    ����unsigned long int num
 * ����ֵ����
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
