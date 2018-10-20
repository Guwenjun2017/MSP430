/*
 * UART.c
 *
 *  Created on: 2018年10月20日
 *      Author: Jack
 */
extern unsigned index;
#include "UART.h"
//*************************************************************************
//               MSP430串口初始化
//*************************************************************************
void UART_Init()
{
    U0CTL|=SWRST;               //复位SWRST
    U0CTL|=CHAR;                //8位数据模式
    U0TCTL|=SSEL1;              //SMCLK为串口时钟
    U0BR1=baud_h;               //BRCLK=8MHZ,Baud=BRCLK/N
    U0BR0=baud_l;               //N=UBR+(UxMCTL)/8
    U0MCTL=0x00;                //微调寄存器为0，波特率9600bps
    ME1|=UTXE0;                 //UART0发送使能
    ME1|=URXE0;                 //UART0接收使能
    U0CTL&=~SWRST;
    IE1|=URXIE0;                //接收中断使能位

    P3SEL|= BIT4;               //设置IO口为普通I/O模式
    P3DIR|= BIT4;               //设置IO口方向为输出
    P3SEL|= BIT5;
}

//*************************************************************************
//              串口0发送数据函数
//*************************************************************************

void Send_Byte(uchar data)
{
    while((IFG1&UTXIFG0)==0);          //发送寄存器空的时候发送数据
    U0TXBUF=data;

    if(data == '@')
    {
        index=0;
        write_cmd(0x01);
        return;
    }
    if(index<16)
    {
        write_cmd(0x80+index);
        write_data(data);
    }
    else
    {
        write_cmd(0x80+0x40+index-16);
        write_data(data);
    }
    index++;
}
//*************************************************************************
//               处理来自串口 0 的接收中断
//*************************************************************************

#pragma vector=USART0RX_VECTOR
__interrupt void UART0_RX_ISR(void)
{
    uchar data;
    data=U0RXBUF;                       //接收到的数据存起来
    Send_Byte(data);                    //将接收到的数据再发送出去
}

//*************************************************************************
//            处理来自串口 0 的发送中断，预留
//*************************************************************************

#pragma vector=USART0TX_VECTOR
__interrupt void UART0_TX_ISR(void)
{

}



