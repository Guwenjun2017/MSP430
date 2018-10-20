/*
 * PCF8591.c
 *
 *  Created on: 2018年10月16日
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
    SDA_clr;       //SCL=1期间,SDA产生下降沿为启动信号
    SCL_clr;
}

void I2C_write_byte(uchar data)
{
    SDA_OUT;
    SCL_OUT;
    SCL_clr;              //SCL=0,使得SDA可变

    uchar i;
    for(i=0; i<8; i++)    //依次校验数据位
    {
        if(data&0x80)
        {
            SDA_set;      //为1,置位
        }
        else
        {
            SDA_clr;
        }
        data = data<<1;
        delay_us(5);
        SCL_set;          //SCL拉高,数据稳定
        delay_us(5);
        SCL_clr;          //SCL拉低,等待下一位校验
        delay_us(5);
    }

    SCL_clr;
    delay_us(3);
}
uchar I2C_read_byte()
{
    SDA_IN;//置数据线为输入方向
    unsigned char i,q0;
    unsigned char byte=0;
    for(i=0;i<8;i++)
    {
      SCL_set; //置时钟为高电平,使数据线数据有效
      delay_us(4);
      byte=byte<<1;
      SDA_IN;
      q0=(P1IN & BIT2);
      if(q0==0X01) byte=(byte|0x01); //将数据存入byte
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

    SDA_clr;      //应答时,SDA拉低
    delay_us(2);
    SCL_set;
    delay_us(5);
    SCL_clr;
}

void I2C_not_ack()
{
    SDA_OUT;
    SCL_OUT;

    SDA_set;      //非应答时,SDA保持高电平
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
