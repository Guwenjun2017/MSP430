#include<msp430f169.h>
#include "delay.h"
#include"DHT11.h"
extern unsigned char humdh,humdl,temph,templ,check,cal;
#include"lcd_1602.h"

/*
 * DHT11.c
 *
 *  Created on: 2018��10��7��
 *      Author: Jack
 */
void DHT11_start()
{
    DHT11_SDA_OUT;
    DHT11_SDA_clr;
    delay_ms(18);      //�ӳ�18ms����
    DHT11_SDA_set;
    delay_us(30);      //�ӳ�20~30us,�ȴ�DHT11��Ӧ

    DHT11_SDA_IN;
    while(P6IN&0x80);
    if(!(P6IN&0x80))
    {
        while(!(P6IN&0x80));
        while((P6IN&0x80));
            humdh = receive();
            humdl = receive();
            temph = receive();
            templ = receive();
            check = receive();
            DHT11_end();
            cal = humdh+humdl+temph+templ;
            if(check == cal)
            {
                write_cmd(0x80+0x40+12);
                display(humdh);
            }
    }
}

unsigned char receive()
{
    unsigned char i,tmp;
    unsigned char data=0;
    for(i=0; i<8; i++)
    {
        //delay_us(50);     //��ʱ50us
        while(!(P6IN&0x70));      //ֱ��DHT11_SDA_set
        delay_us(50);       //���ӳ�50us��
        if(P6IN&0x70)       //��������DHT11_SDA��ƽΪ1
        {
            tmp = 1;
        }
        else
        {
            tmp = 0;
        }
        data = tmp<<1;
        data |= tmp;
        while(P6IN&0x80);
    }

    return data;
}

void DHT11_end()
{
    DHT11_SDA_OUT;
    DHT11_SDA_set;
}


