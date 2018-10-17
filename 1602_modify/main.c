#include <msp430f169.h>
#include "delay.h"
#include "type.h"
#include "clock.h"
#include "lcd_1602.h"
#include "DS1302.h"
#include "DHT11.h"

uchar second,minute,hour,year,month,day,week;
uchar *WEEK_array[7] = {"MON","TUE","WEN","THU","FRI","STA","SUN"};
uchar humdh,humdl,temph,templ,check,cal;

/**
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    init_clk();

    P4SEL = 0x00;
    P4DIR = 0xff;
    P5DIR |= BIT5 + BIT6 + BIT7 + BIT0 + BIT1 + BIT2;
    //  P6DIR |= BIT7;
    //P6OUT &= ~BIT7;
    lcd1602_init();

    DS1302_init();

    //DHT11_start();

    while(1)
    {
        second = readsecond();
        minute = readminute();
        hour = readhour();
        day = readday();
        month = readmonth();
        week = readweek();
        year = readyear();

        write_second(second);
        write_minute(minute);
        write_hour(hour);
        write_day(day);
        write_month(month);
        write_week(week);
        write_year(year);
    }
}
