#include<msp430f169.h>
#include"type.h"
/*
 * DS1302.h
 *
 *  Created on: 2018年10月6日
 *      Author: Jack
 */

#ifndef DS1302_H_
#define DS1302_H_

/*DS1302 端口定义与配置*/
#define DS1302_DIR P5DIR
#define DS1302_IN P5IN
#define DS1302_OUT P5OUT

#define DS1302_RST BIT0
#define DS1302_SCLK BIT1
#define DS1302_SDI BIT2

#define DS1302_RST_set DS1302_OUT |= DS1302_RST
#define DS1302_RST_clr DS1302_OUT &= ~DS1302_RST
#define DS1302_SCLK_set DS1302_OUT |= DS1302_SCLK
#define DS1302_SCLK_clr DS1302_OUT &= ~DS1302_SCLK
#define DS1302_SDI_set DS1302_DIR |= DS1302_SDI
#define DS1302_SDI_clr DS1302_DIR &= ~DS1302_SDI

/*DS1302 硬件宏定义*/
#define SEC    0x80
#define MIN    0x82
#define HOUR   0x84
#define DAY    0x86
#define MONTH  0x88
#define WEEK   0x8A
#define YEAR   0x8C
#define CTRL   0x8E

/*功能函数*/
void start();
void end();
void DS1302_write_byte(uchar);
uchar DS1302_read_byte();
void DS1302_write(uchar , uchar );
uchar DS1302_read(uchar );
void DS1302_set_time(uchar *);
void DS1302_read_time(uchar *);
void write_protect();
void write_unprotect();
void DS1302_init();

uchar readsecond();
uchar readminute();
uchar readhour();
uchar readday();
uchar readmonth();
uchar readweek();
uchar readyear();
//void readtime();
void write_second(uchar);
void write_minute(uchar);
void write_hour(uchar);
void write_day(uchar);
void write_month(uchar);
void write_week(uchar);
void write_year(uchar);

#endif /* DS1302_H_ */
