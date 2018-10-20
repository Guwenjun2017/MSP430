/*
 * 4x4key.c
 *
 *  Created on: 2018��10��11��
 *      Author: Jack
 */

#include<msp430f169.h>
#include "lcd_1602.h"
#include "type.h"
#include "delay.h"
#include "my_4x4key.h"
#include<string.h>
//extern uchar keyvalue;
extern char disp_buff[MAX_length];
//#define ERROR 0
//#define OK 1
long num1,num2;
void calculator()
{
    uchar i;
    char expression[MAX_length];   //���ʽ����
//    uchar disp_buff[MAX_length];    //��ʾ������
    char operator_num = 0;         //�������ĸ���
    char operator_pos = 0;         //�������ڱ��ʽ�����е�λ��
    char char_pos = 0;             //�����ַ���ǰλ��
    char str1[MAX_length], str2[MAX_length];   //�����������ֵĻ�����
    char status;                   //������״̬

    //���������ʽ������
    for(i=0; i<MAX_length; i++)
    {
        expression[i] = 0x00;
        str1[i] = 0;
        str2[i] = 0;
    }

    while(1)
    {
        unsigned char keyvalue=keyscan();
        if(keyvalue != 0xff)        //���������Ч
        {
            if(keyvalue != '=' && keyvalue != 'C')    //������벻Ϊ��=�����ߡ�C��
            {
                if(keyvalue == '+' | keyvalue == '-' | keyvalue == '*' | keyvalue == '/' && operator_num == 0)
                {
                    if(char_pos != 0 && char_pos != MAX_length-1)   //���������ڵ�һλ�����һλ
                    {
                        expression[char_pos] = keyvalue;   //���������
                        operator_num++; //����������++
                        operator_pos = char_pos;   //Ԥ�����������
                        char_pos++;     //�ַ�����++
                        write_cmd(0x80);
                        calc_display(expression);   //ˢ�´�ӡ
                    }
                }
                else               //�������Ϊ����
                {
                    if(char_pos != MAX_length-1)
                    {
                        expression[char_pos] = keyvalue;  //��������
                        char_pos++;  //�ַ�����++
                        write_cmd(0x80);
                        calc_display(expression);   //ˢ�´�ӡ
                    }
                }
            }
            else if(keyvalue == '=')      //����������'='��������
            {
                if(char_pos-operator_pos > 0)  //�������Ϊ���ʽ���һ���ַ�
                {
                    strncpy(str1, expression, (operator_pos));
                    strncpy(str2, &(expression[operator_pos + 1]), (char_pos - operator_pos));
                    num1 = string_to_long(str1);
                    num2 = string_to_long(str2);
                    status = calc_operation(expression[operator_pos]);
                    for(i=0; i<MAX_length; i++)   //�����ʾ������
                    {
                        disp_buff[i] = 0x00;
                    }
                    write_cmd(0x01);        //����
                    calc_disp(status);      //��ӡ���
                }
                else
                {
                    write_cmd(0x01);
                    calc_disp('E');
                }
            }
            else if(keyvalue == 'C')
            {
                operator_num = 0;
                operator_pos = 0;
                char_pos = 0;
                for(i=0; i<MAX_length; i++)   //��ջ�����
                {
                    disp_buff[i] = 0x00;
                    expression[i] = 0x00;
                    str1[i] = 0x00;
                    str2[i] = 0x00;
                }
                write_cmd(0x01);
            }
        }
        keyvalue = 0xff;
    }
}

char string_to_long(char *buffer)
{
    long value;
    long digit;
    value = 0;
    while(*buffer != 0x00)
    {
        digit = *buffer - '0';
        value = value*10 + digit;
        buffer++;
    }

    return value;
}

int calc_operation(char token)
{
    int result;
    switch(token)
    {
    case '+':
        num1 += num2;
        result = 1;
        break;
    case '-':
        num1 -= num2;
        result = 1;
        break;
    case '*':
        num1 *= num2;
        if((num1>=-9999999) && (num2<=9999999))
            result = 1;
        else
            result = 0;
        break;
    case '/':
        if(num2 != 0)
        {
            result = 1;
            num1 = num1/num2;
        }
        else
            result = 0;
        break;
    default:
        result = 0;
    }

    return result;
}

void calc_disp(int status)
{
    switch(status)
    {
    case 1:
        calc_display(num_to_string(num1));
        break;
    case 0:
        calc_display("ERROR");
        break;
    default:
        calc_display("ERROR");
        break;
    }
}

char *num_to_string(long num)
{
    long temp=num;
    char *ptr = &disp_buff[MAX_length-1];
    long divisor = 10;
    long result;
    uchar remainder, asciival;
    char i;
    char cnt=0;
    char minus_flag = 0;
    if(!temp)
    {
        disp_buff[0] = '0';

    }
    if(num<0)
    {
        minus_flag=1;
        temp -= 2*temp;
    }
    for(i=0; i<sizeof(disp_buff)-1; i++)
    {
        remainder = temp%divisor;
        result = temp/divisor;
        if((!remainder) && (!result))
        {
            *ptr = 0x00;
            return disp_buff;
        }
        else
        {
            asciival = remainder + '0';
            *ptr = asciival;
            cnt++;
        }
        temp = temp/10;
        if(ptr != &disp_buff[1])
            ptr--;
    }
    for(i=0; i<cnt; i++)
        disp_buff[i+minus_flag] = disp_buff[MAX_length - cnt + i];
    if(minus_flag)
        disp_buff[0] = '-';
    return disp_buff;
}
unsigned char keyscan(void)
{
    unsigned char keyno=16;
    delay_ms(5);

    P2DIR=0Xf0;//����λ���������λ����
    P2OUT=0xef;        //ɨ���һ��
    switch(P2IN&0x0f)
    {
    case 0x0e:  keyno=1;
       break;
    case 0x0d:  keyno=2;
       break;
    case 0x0b:  keyno=3;
       break;
    case 0x07:  keyno=4;
       break;
    }
    P2OUT=0xDf;       //ɨ��ڶ���
    switch(P2IN&0x0f)
    {
    case 0x0e:  keyno=5;
        break;
    case 0x0d:  keyno=6;
        break;
    case 0x0b:  keyno=7;
        break;
    case 0x07:  keyno=8;
        break;
    }
    P2OUT=0xBf;       //ɨ�������
    switch(P2IN&0x0f)
    {
    case 0x0e:  keyno=9;
        break;
    case 0x0d:  keyno=10;
        break;
    case 0x0b:  keyno=11;
        break;
    case 0x07:  keyno=12;
        break;
    }
    P2OUT=0x7f;       //ɨ�������
    switch(P2IN&0x0f)
    {
    case 0x0e:  keyno=13;
        break;
    case 0x0d:  keyno=14;
        break;
    case 0x0b:  keyno=15;
        break;
    case 0x07:  keyno=16;
        break;
    }
    delay_ms(5);

    return keyno;
}
