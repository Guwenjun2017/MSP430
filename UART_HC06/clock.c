#include<msp430f169.h>
/*
 * clock.c
 *
 *  Created on: 2018��10��5��
 *      Author: Jack
 */
//***********************************************************************
//                   ϵͳʱ�ӳ�ʼ�����ⲿ8M����
//***********************************************************************
void init_clk_8M()
{
  unsigned char i;
  BCSCTL1&=~XT2OFF;                 //��XT2����
  BCSCTL2|=SELM1+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
  do{
    IFG1&=~OFIFG;                   //������������־
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
  IFG1&=~OFIFG;
}

//��ʼ��ʱ��
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



