#include <msp430f169.h>
#include "type.h"
#include "delay.h"
#include "clock.h"
#include "lcd_1602.h"
#include "UART.h"
//uchar data;
extern uchar index=0;
//*************************************************************************
//           ������
//*************************************************************************
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           //���Ź�����
    init_clk_8M();                       //ϵͳʱ������
    UART_Init();                        //�������ó�ʼ��
    _EINT();                            //���ж�
    P4SEL = 0x00;
    P4DIR = 0xff;
    P5DIR |= BIT5 + BIT6 + BIT7 + BIT0 + BIT1 + BIT2;
    lcd1602_init();
    while(1)                            //����ѭ��
    {
//        if(data != 0)
//        {
//            Send_Byte(data);
//        }
    }
}
