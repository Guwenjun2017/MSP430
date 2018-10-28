#include<msp430f169.h>
/*
 * clock.c
 *
 *  Created on: 2018��10��5��
 *      Author: Jack
 */

//��ʼ��ʱ��
void init_clk()
{
    unsigned char i;
    BCSCTL1 &= ~XT2OFF;   //��XT����
    BCSCTL2 |= SELM_2 + SELS;    //MCLKΪ8MHz,SMCLKΪ1MHz
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



