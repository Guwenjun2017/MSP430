#include<msp430f169.h>
/*
 * clock.c
 *
 *  Created on: 2018年10月5日
 *      Author: Jack
 */

//初始化时钟
void init_clk()
{
    unsigned char i;
    BCSCTL1 &= ~XT2OFF;
    BCSCTL2 |= SELM1 + SELS;
    do
    {
        IFG1 &= ~OFIFG;
        for(i = 0; i < 100; i++)
        {
            _NOP();
        }
    }
    while((IFG1&OFIFG) != 0);
    IFG1 &= ~OFIFG;

}



