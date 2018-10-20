/*
 * UART.c
 *
 *  Created on: 2018��10��20��
 *      Author: Jack
 */
extern unsigned index;
#include "UART.h"
//*************************************************************************
//               MSP430���ڳ�ʼ��
//*************************************************************************
void UART_Init()
{
    U0CTL|=SWRST;               //��λSWRST
    U0CTL|=CHAR;                //8λ����ģʽ
    U0TCTL|=SSEL1;              //SMCLKΪ����ʱ��
    U0BR1=baud_h;               //BRCLK=8MHZ,Baud=BRCLK/N
    U0BR0=baud_l;               //N=UBR+(UxMCTL)/8
    U0MCTL=0x00;                //΢���Ĵ���Ϊ0��������9600bps
    ME1|=UTXE0;                 //UART0����ʹ��
    ME1|=URXE0;                 //UART0����ʹ��
    U0CTL&=~SWRST;
    IE1|=URXIE0;                //�����ж�ʹ��λ

    P3SEL|= BIT4;               //����IO��Ϊ��ͨI/Oģʽ
    P3DIR|= BIT4;               //����IO�ڷ���Ϊ���
    P3SEL|= BIT5;
}

//*************************************************************************
//              ����0�������ݺ���
//*************************************************************************

void Send_Byte(uchar data)
{
    while((IFG1&UTXIFG0)==0);          //���ͼĴ����յ�ʱ��������
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
//               �������Դ��� 0 �Ľ����ж�
//*************************************************************************

#pragma vector=USART0RX_VECTOR
__interrupt void UART0_RX_ISR(void)
{
    uchar data;
    data=U0RXBUF;                       //���յ������ݴ�����
    Send_Byte(data);                    //�����յ��������ٷ��ͳ�ȥ
}

//*************************************************************************
//            �������Դ��� 0 �ķ����жϣ�Ԥ��
//*************************************************************************

#pragma vector=USART0TX_VECTOR
__interrupt void UART0_TX_ISR(void)
{

}



