#include<msp430f169.h>
#include "delay.h"
#include"DHT11.h"
extern unsigned char humdh,humdl,temph,templ,check,cal;
#include"lcd_1602.h"

/*
 * DHT11.c
 *
 *  Created on: 2018年10月7日
 *      Author: Jack
 */
void DHT11_start()
{
    DHT11_SDA_OUT;
    DHT11_SDA_clr;
    delay_ms(18);      //延迟18ms以上
    DHT11_SDA_set;
    delay_us(30);      //延迟20~30us,等待DHT11响应

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
        //delay_us(50);     //延时50us
        while(!(P6IN&0x70));      //直到DHT11_SDA_set
        delay_us(50);       //再延迟50us后
        if(P6IN&0x70)       //采样，若DHT11_SDA电平为1
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


