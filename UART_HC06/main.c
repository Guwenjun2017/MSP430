#include <msp430f169.h>
#include "type.h"
#include "delay.h"
#include "clock.h"
#include "lcd_1602.h"
#include "UART.h"
//uchar data;
extern uchar index=0;
//*************************************************************************
//           主函数
//*************************************************************************
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           //看门狗设置
    init_clk_8M();                       //系统时钟设置
    UART_Init();                        //串口设置初始化
    _EINT();                            //开中断
    P4SEL = 0x00;
    P4DIR = 0xff;
    P5DIR |= BIT5 + BIT6 + BIT7 + BIT0 + BIT1 + BIT2;
    lcd1602_init();
    while(1)                            //无限循环
    {
//        if(data != 0)
//        {
//            Send_Byte(data);
//        }
    }
}
