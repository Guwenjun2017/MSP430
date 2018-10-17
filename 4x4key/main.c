
#include <msp430f169.h>
#include <my_4x4key.h>
#include "delay.h"
#include "type.h"
#include "clock.h"
#include "lcd_1602.h"
#include "SG90.h"
unsigned int a=1500;
uchar keyvalue;

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    //init_clk();
    BCSCTL2 = SELS + DIVS0 + DIVS1;
    P1DIR = 0xff;
    P1OUT &= ~BIT2;
    CCR0 = 20000;             //PWM周期
    CCTL1 = OUTMOD_7;         //CCR1复位/置位
    CCR1 = a;                 //CCR1 PWM占空比5%
    TACTL = TASSEL_2 + MC_1;  //TASSEL_2-->TimerA时钟源为SMCLK=8MHz,MC_1-->增计数模式

    P4SEL = 0x00;
    P4DIR = 0xff;
    P5DIR |= BIT5 + BIT6 + BIT7 + BIT0 + BIT1 + BIT2;

    lcd1602_init();

    while(1)
    {
        keyvalue = keyscan();
        if(keyvalue != 16)
        {
            write_cmd(0x01);
            write_cmd(0x80);
            display(keyvalue);
        }
        CCR1 = a;
                //key_check();
                //key_value = keyscan();
                switch(keyvalue)
                {
                case 1: key_process1();
                    break;
                case 2: key_process2();
                    break;
                case 3: key_process3();
                    break;
                case 4: key_process4();
                    break;
                case 5: key_process5();
                    break;
                default: break;
                }
                keyvalue = 0x00;
    }

}
