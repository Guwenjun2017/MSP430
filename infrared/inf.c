/*
 * inf.c
 *
 *  Created on: 2018��11��9��
 *      Author: Jack
 */
#include"inf.h"
extern unsigned char RECV_FLAG;
extern unsigned char ircode[4];
extern unsigned char data[33];
extern unsigned char VALUE;

/*
 * ��    �ƣ�infrared_decode()
 * ��    �ܣ���������
 * ��    ������
 * ����ֵ����
 */
void infrared_decode()
{
    if(RECV_FLAG == 1)    //���ɹ����յ�һ�����ݣ�33���½��ش�����
    {
        unsigned char k=1, i, j,value;    //k����Ϊ1����Ϊ��һλΪ�����룬�����
        for(j=0; j<4; j++)                //��4�飬ÿ��8λ�����ν���
        {
            for(i=0; i<8; i++)
            {
                value = value>>1;
                if(data[k]>6)             //����λ���ݳ�����count��>6,��Ϊ��1���룬����Ϊ��0����
                {
                    value = value|0x80;
                }
                k++;
            }
            ircode[j]=value;              //ircode�������δ洢�����û��롢һ�������롢һ�������뷴�롢
        }
        RECV_FLAG=0;                      //���ձ�־λ����
        P6DIR = 0xff;                     //38 39����������ã����ڹ۲�ÿ��������Ӧ�����ݲ�ͬ
        P6OUT = ircode[2];                //ircode[2]Ϊ������
        write_cmd_1602(0x01);                  //lcd1602����
        write_cmd_1602(0x80);                  //�ڵ�һ����ʾ
        dis_num(ircode[2]);               //��ʾircode[2]
    }
}
/*
 * ��    �ƣ�set_value()
 * ��    �ܣ����ݲ�ͬ������������֮��Ӧ������ļ�ֵ
 * ��    ����ÿ��������Ӧ�Ĳ�ͬ��������
 * ����ֵ����
 */
void set_value(unsigned char argu)
{
    switch(argu)    //����ֻ������ң���������еİ���ֵ���ɸ�����Ҫ�޸�
    {
    case  7: VALUE = 'S'; break; case 21: VALUE = 'A'; break; case  9: VALUE = 'E'; break;
    case 22: VALUE = 0; break;
    case 12: VALUE = 1; break;   case 24: VALUE = 2; break;   case 94: VALUE = 3; break;
    case  8: VALUE = 4; break;   case 28: VALUE = 5; break;   case 90: VALUE = 6; break;
    case 66: VALUE = 7; break;   case 82: VALUE = 8; break;   case 74: VALUE = 9; break;
    default: break;
    }
}
/*
 * ��    �ƣ�init_port1()
 * ��    �ܣ�����P1.0�˿ڵ��ж�
 * ��    ������
 * ����ֵ����
 */
void init_port1()
{
    P1DIR &= ~BIT0;     //����Ϊ���뷽��
    P1IES |= BIT0;      //ѡ���½��ش���
    P1IE |= BIT0;       //���ж�����
    P1IFG = ~BIT0;      //P1IES���л�����ʹP1IFG��λ�������
}
/*
 * ��    �ƣ�init_devi()
 * ��    �ܣ�����TIMERA
 * ��    ������
 * ����ֵ����
 */
void init_device()
{
    TACCTL0 = CCIE;            //CCR0�ж�ʹ��
    TACCR0 = 33;               //�����յ�
    TACTL = TASSEL_2 + MC_1;   //TIMER_Aѡ��timerʱ��MCLK��������ģʽ
    BCSCTL2 |= SELS;
    _EINT();                   //�������ж�
}
