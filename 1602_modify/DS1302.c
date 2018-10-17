#include "type.h"
#include "DS1302.h"
#include "lcd_1602.h"
#include "delay.h"


extern uchar *WEEK_array[7];
/*
 * DS1302.c
 *
 *  Created on: 2018��10��6��
 *      Author: Jack
 */
void start()
{
    DS1302_RST_clr;
    DS1302_SCLK_clr;
    _NOP();
    DS1302_RST_set;
}
void end()
{
    DS1302_SCLK_set;
    DS1302_RST_clr;
}
void DS1302_write_byte(uchar data)
{
    uchar i;
    DS1302_DIR |= DS1302_SDI;
    DS1302_RST_set;
    for(i=0; i<8; i++)
    {
        if(data&0x01)
            DS1302_SDI_set;
        else
            DS1302_SDI_clr;
        DS1302_SCLK_clr;
        delay_us(5);
        DS1302_SCLK_set;
        delay_us(5);
        data >>= 1;
    }
}
uchar DS1302_read_byte()
{
    uchar i;
    uchar data = 0x00;
    DS1302_DIR &= ~DS1302_SDI;
    DS1302_RST_set;
    for(i=0; i<8; i++)
    {
        DS1302_SCLK_set;
        if(DS1302_IN&DS1302_SDI)
            data |= 0x80;
        DS1302_SCLK_clr;
        data>>=1;
    }

    return data;
}
void DS1302_write(uchar addr, uchar data)
{
    start();

    DS1302_write_byte(addr);
    DS1302_write_byte(data);

    end();

}

uchar DS1302_read(uchar addr)
{
    uchar data;
    start();

    DS1302_write_byte(addr);
    data = DS1302_read_byte();

    end();
    return data;
}

void write_protect()
{
    DS1302_write(CTRL, 0x80);
}
void write_unprotect()
{
    DS1302_write(CTRL, 0x00);
}

void DS1302_init()
{
    uchar flag=DS1302_read(0x81);
    if(flag&0x80)
    {
        write_unprotect();   //�ر�д����
        DS1302_write(SEC,((50/10)<<4|(50%10)));//д��Ĵ���,д���ʼֵ55�������Զ���ʱ����
        DS1302_write(MIN,((27/10)<<4|(27%10)));//д�ּĴ�������д���ֵ59
        DS1302_write(HOUR,((20/10)<<4|(20%10)));//дСʱ�Ĵ�������д���ֵ23
        DS1302_write(DAY,((12/10)<<4|(12%10)));//д�ռĴ�������д���ֵ18
        DS1302_write(MONTH,((10/10)<<4|(10%10)));//д�¼Ĵ�������д���ֵ2
        DS1302_write(WEEK,((5/10)<<4|(5%10)));//д�ܼĴ�������д���ֵ5
        DS1302_write(YEAR,((18/10)<<4|(18%10)));//д��Ĵ�������д���ֵ12������д2012��
        write_protect();     //����д����
    }

}

uchar readsecond()
{
    uchar data;
    data = DS1302_read(SEC+1);
    data=((data&0x70)>>4)*10+(data&0x0f);
    return data;
}
uchar readminute()
{
    uchar dat;
    dat=DS1302_read(MIN+1);
    dat=((dat&0x70)>>4)*10+(dat&0x0f);
    return dat;
}
/*****************����Сʱ��ʮ������***************************/
uchar readhour()
{
    uchar dat;
    dat=DS1302_read(HOUR+1);
    dat=((dat&0x70)>>4)*10+(dat&0x0f);
    return dat;
}
/*****************�������ʮ������***************************/
uchar readday()
{
    uchar dat;
    dat=DS1302_read(DAY+1);
    dat=((dat&0x70)>>4)*10+(dat&0x0f);
    return dat;
}
/*****************�����µ�ʮ������***************************/
uchar readmonth()
{
    uchar dat;
    dat=DS1302_read(MONTH+1);
    dat=((dat&0x70)>>4)*10+(dat&0x0f);
    return dat;
}
/*****************�����ܵ�ʮ������***************************/
uchar readweek()
{
    uchar dat;
    dat=DS1302_read(WEEK+1);
    dat=((dat&0x70)>>4)*10+(dat&0x0f);
    return dat;
}
/*****************�������ʮ������***************************/
uchar readyear()
{
    uchar dat;
    dat=DS1302_read(YEAR+1);
    dat=((dat&0xf0)>>4)*10+(dat&0x0f);
    return dat;
}

/************************��������ʱ��**********************/

/*********************��1602д��ʱ��****************************/
void write_second(uchar second)
{
    uchar shi,ge;
    shi=second/10;
    ge=second%10;
    Write1602(0x80+0x40+8,0x30+shi);
    Write1602(0x80+0x40+9,0x30+ge);
}
void write_minute(uchar minute)
{
    uchar shi,ge;
    shi=minute/10;
    ge=minute%10;
    Write1602(0x80+0x40+5,0x30+shi);
    Write1602(0x80+0x40+6,0x30+ge);
}
void write_hour(uchar hour)
{
    uchar shi,ge;
    shi=hour/10;
    ge=hour%10;
    Write1602(0x80+0x40+2,0x30+shi);
    Write1602(0x80+0x40+3,0x30+ge);
}
void write_day(uchar day)
{
    uchar shi,ge;
    shi=day/10;
    ge=day%10;
    Write1602(0x80+8,0x30+shi);
    Write1602(0x80+9,0x30+ge);
}
void write_month(uchar month)
{
    uchar shi,ge;
    shi=month/10;
    ge=month%10;
    Write1602(0x80+5,0x30+shi);
    Write1602(0x80+6,0x30+ge);
}
void write_week(uchar week)
{
    dis_str(0x80+12, WEEK_array[week-1]);
}
void write_year(uchar year)
{
    uchar shi,ge;
    shi=year/10;
    ge=year%10;
    Write1602(0x80+2,0x30+shi);
    Write1602(0x80+3,0x30+ge);
}
