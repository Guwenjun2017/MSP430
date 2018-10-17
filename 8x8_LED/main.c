#include<msp430f169.h>
#include "delay.h"
const unsigned char  tab[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//led
const unsigned char  digittab[3][8]=
                         {
                             {0x00,0x00,0x00,0xfe,0xfe,0x00,0x00,0x00},  //"I"
                             {0x1C,0x3e,0x7e,0xfc,0xfc,0x7e,0x3e,0x1c},  //"xin"
                             {0x00,0x3e,0x7e,0x60,0x60,0x7e,0x3e,0x00}   //"U"
//                             {0x00,0x00,0x82,0xfe,0x82,0x00,0x00,0x00},
//                             {0x1C,0x22,0x42,0x84,0x84,0x42,0x22,0x1c},
//                             {0x00,0x02,0x04,0xf8,0x04,0x02,0x00,0x00}
                         };

unsigned int timecount = 0;
unsigned char cnta = 0;
unsigned char cntb = 0;
#define SD_H      P1OUT|=BIT4;        //串行数据输入
#define SD_L      P1OUT&=~BIT4;
#define ST_H    P1OUT|=BIT5;        //存储寄存器时钟输入
#define ST_L    P1OUT&=~BIT5;
#define SH_H    P1OUT|=BIT6;   //移位寄存器时钟输入
#define SH_L    P1OUT&=~BIT6;

void Ser_IN(unsigned char Data)
{
    unsigned char i;
    for(i=0; i < 8; i++)
    {
        SH_L;                        //CLOCK_MAX=100MHz
        if(Data&0x80)
        {
            SD_H;
        }
        else
        {
            SD_L;
        }
        Data <<= 1;
        SH_H;
    }
}
void Par_OUT(void)
{
    ST_L;
    ST_H;
}
void sysclock()
{
    unsigned int i;
    BCSCTL1 &= ~XT2OFF;
    do
    {
        IFG1 &= ~OFIFG;
        for(i=0xff; i>0; i--);
    }while((IFG1&OFIFG));
    BCSCTL2 |= SELM_2 + SELS;
}

void main()
{
    WDTCTL=WDTPW+WDTHOLD;
    P1SEL&=~(BIT4+BIT5+BIT6);
    P1DIR|=BIT4+BIT5+BIT6;
    sysclock();

    while(1){
        unsigned long i=0;
        for(;i<12000;i++)
        {
            Ser_IN(tab[cnta]);//8X8点阵列扫描
            Ser_IN(digittab[cntb%3][cnta]);//8X8点阵行送扫描数据
            Par_OUT();//74HC595输出显示
            cnta++;\
            //delay_ms(10000);
            if(cnta==8)
            {
                cnta=0;
            }
        }
        cntb++;
    }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timeA0(void)
{
//        Ser_IN(tab[cnta]);//8X8点阵列扫描
//        Ser_IN(digittab[cntb][cnta]);//8X8点阵行送扫描数据
//        Par_OUT();//74HC595输出显示
//        cnta++;

    timecount++;
    if(timecount==1)
    {
        timecount=0;
        cntb++;
        if(cntb==6)
        {
          cntb=0;
        }
    }
}
