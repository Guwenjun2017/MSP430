/*
 * PCF8591.c
 *
 *  Created on: 2018��10��16��
 *      Author: Jack
 */
#include <msp430f169.h>
#include "delay.h"
#include "type.h"
#include "Port.h"
void I2C_start()
{
    SDA_OUT;
    SCL_OUT;

    SCL_set;       //SCL=1
    SDA_set;       //SDA=1
    delay_us(5);
    SDA_clr;       //SCL=1�ڼ�,SDA�����½���Ϊ�����ź�
    SCL_clr;
}

void I2C_write_byte(uchar data)
{
    SDA_OUT;
    SCL_OUT;
    SCL_clr;              //SCL=0,ʹ��SDA�ɱ�

    uchar i;
    for(i=0; i<8; i++)    //����У������λ
    {
        if(data&0x80)
        {
            SDA_set;      //Ϊ1,��λ
        }
        else
        {
            SDA_clr;
        }
        data = data<<1;
        delay_us(5);
        SCL_set;          //SCL����,�����ȶ�
        delay_us(5);
        SCL_clr;          //SCL����,�ȴ���һλУ��
        delay_us(5);
    }

    SCL_clr;
    delay_us(3);
}
uchar I2C_read_byte()
{
    SDA_IN;//��������Ϊ���뷽��
    unsigned char i,q0;
    unsigned char byte=0;
    for(i=0;i<8;i++)
    {
      SCL_set; //��ʱ��Ϊ�ߵ�ƽ,ʹ������������Ч
      delay_us(4);
      byte=byte<<1;
      SDA_IN;
      q0=(P1IN & BIT2);
      if(q0==0X01) byte=(byte|0x01); //�����ݴ���byte
      delay_us(4);
      SCL_clr;
      delay_us(4);
    }
    return byte;
}
//uchar I2C_read_byte()
//{
//    uchar data,i;
//    SDA_OUT;
//    SCL_OUT;
//
//    SCL_clr;
//    delay_us(4);
//    SDA_set;
//    SDA_IN;
//    uchar data,i;
//    for(i=0; i<8; i++)
//    {
//        SCL_set;
//        delay_us(4);
//        data <<= 1;
//        if(P1IN & BIT2)
//        {
//            data++;
//        }
//        SCL_clr;
//        delay_us(4);
//    }
//    return data;
//}
void I2C_ack()
{
    SDA_OUT;
    SCL_OUT;

    SDA_clr;      //Ӧ��ʱ,SDA����
    delay_us(2);
    SCL_set;
    delay_us(5);
    SCL_clr;
}

void I2C_not_ack()
{
    SDA_OUT;
    SCL_OUT;

    SDA_set;      //��Ӧ��ʱ,SDA���ָߵ�ƽ
    delay_us(2);
    SCL_set;
    delay_us(5);
    SCL_clr;
}

void I2C_stop()
{
    SCL_set;
    SDA_clr;
    delay_us(1);
    SDA_set;
    delay_us(1);
}
