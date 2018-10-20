#include<msp430f169.h>
/*
 * clock.c
 *
 *  Created on: 2018年10月5日
 *      Author: Jack
 */
//***********************************************************************
//                   系统时钟初始化，外部8M晶振
//***********************************************************************
void init_clk_8M()
{
  unsigned char i;
  BCSCTL1&=~XT2OFF;                 //打开XT2振荡器
  BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清楚振荡器错误标志
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG;
}

//初始化时钟
void init_clk_1M()
{
    unsigned char i;
    BCSCTL1 &= ~XT2OFF;
    BCSCTL2 |= DIVS0 + DIVS1;
    BCSCTL2 |= SELM_2 + SELS;
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



