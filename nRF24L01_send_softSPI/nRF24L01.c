/*
 * nRF24L01.c
 *
 *  Created on: 2018年10月27日
 *      Author: Jack
 */
#include"nRF24L01.h"
#include<msp430f169.h>
#include"type.h"
#include"delay.h"


char SPI_write(uchar data)
{
    char i,temp=0;
    for(i=0; i<8; i++)
    {
        if((data & 0x80) == 0x80)   //逐位设置MOSI
        {
            MOSI_set;
        }
        else
        {
            MOSI_clr;
        }
        data <<= 1;
        temp <<= 1;
        SCK_set;          //配置时钟信号为高时，采样所得数据有效
        if(MISO_input_1)  //MISO端口输入1时，temp自增1
            temp++;
        SCK_clr;
    }

    return temp;
}

//function_name:  SPI_read(char regis)
//     describe:  get the value of register
char SPI_read(char regis)  //读寄存器内的数据
{
    char regis_value;
    CSN_clr;
    SPI_write(regis);      //主机->从机，写地址
    regis_value = SPI_write(0);   //从机->主机，传数据
                                  //因为要获取从机的数据，故SIP_write()本次调用传入的参数设为零
    return regis_value;
}

//function_name:  SPI_write_reg(char reg, char value)
//     describe:  send the value to the reg
char SPI_write_reg(char reg, char value)
{
    char status1;
    CSN_clr;
    status1 = SPI_write(reg);   //主机->从机，写地址
    SPI_write(value);           //主机->从机，写数据
    CSN_set;

    return status1;
}

//function_name:  SPI_read_buf(char reg, char *buf, char length)
//     describe:  get the value of reg and send it to buf
char SPI_read_buf(char reg, char *buf, char length)
{
    char status2, i;
    CSN_clr;
    status2 = SPI_write(reg);   //主机->从机，写地址
    for(i=0; i<length; i++)
    {
        buf[i] = SPI_write(0);  //从机->主机，传数据
                                //因为要获取从机的数据，故SIP_write()本次调用传入的参数设为零
    }
    CSN_set;

    return status2;
}

//function_name:  SPI_write_buf(char reg, char *buf, char length)
//     describe:  send the value of buf to reg

char SPI_write_buf(char reg, char *buf, char length)
{
    char status1, i;
    CSN_clr;
    status1 = SPI_write(reg);   //主机->从机，写地址
    for(i=0; i<length; i++)
    {
        SPI_write(*buf++);   //主机->从机，写数据
    }
    CSN_set;

    return status1;
}

//function_name:  set_RXmode(void)
//     describe:  set mode of RX
void set_RXmode(void)
{
    CE_clr;
    SPI_write_reg(WRITE_REG + CONFIG, 0x0f);
    CE_set;
    delay_us(600);    //注意不能太小
}

//function_name:  nRF24L01_RX(char *rxbuf)
//     describe:  send data to rxbuf once we get it
char nRF24L01_RX(char *rxbuf)
{
    char reg_value=0;
    char sta;
    sta = SPI_read(STATUS);  //读取nRF20L01的状态寄存器来判断数据接收情况
    if(sta & 0x40)
    {
        CE_clr;
        SPI_read_buf(RD_RX_PLOAD, rxbuf, TX_PLOAD_WIDTH);
        reg_value=1;
    }
    SPI_write_reg(WRITE_REG + STATUS, sta);  //一旦接收到数据后，RX_DR TX_DS MAS_PT都为1
                                                //重写状态寄存器以清除中断标志

    return reg_value;
}

//function_name:  nRF24L01_TX(char *txbuf)
//     describe:  send the data of txbuf
void nRF24L01_TX(char *txbuf)
{
    CE_clr;
    SPI_write_buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);
    SPI_write_buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);
    CE_set;
    //delay_us(100);
}

//function_name:  init_nRF24L01(void)
//     describe:  init this module
void init_nRF24L01(void)
{
    delay_us(10);
    CE_clr;
    CSN_clr;
    SCK_clr;
    SPI_write_buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);
    SPI_write_buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH);
    SPI_write_reg(WRITE_REG + EN_AA, 0x01);
    SPI_write_reg(WRITE_REG + EN_RXADDR, 0x01);
    SPI_write_reg(WRITE_REG + RF_CH, 0);
    SPI_write_reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);
    SPI_write_reg(WRITE_REG + RF_SETUP, 0x07);
    SPI_write_reg(WRITE_REG + CONFIG, 0x0E);
}
