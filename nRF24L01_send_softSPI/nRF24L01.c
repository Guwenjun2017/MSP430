/*
 * nRF24L01.c
 *
 *  Created on: 2018��10��27��
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
        if((data & 0x80) == 0x80)   //��λ����MOSI
        {
            MOSI_set;
        }
        else
        {
            MOSI_clr;
        }
        data <<= 1;
        temp <<= 1;
        SCK_set;          //����ʱ���ź�Ϊ��ʱ����������������Ч
        if(MISO_input_1)  //MISO�˿�����1ʱ��temp����1
            temp++;
        SCK_clr;
    }

    return temp;
}

//function_name:  SPI_read(char regis)
//     describe:  get the value of register
char SPI_read(char regis)  //���Ĵ����ڵ�����
{
    char regis_value;
    CSN_clr;
    SPI_write(regis);      //����->�ӻ���д��ַ
    regis_value = SPI_write(0);   //�ӻ�->������������
                                  //��ΪҪ��ȡ�ӻ������ݣ���SIP_write()���ε��ô���Ĳ�����Ϊ��
    return regis_value;
}

//function_name:  SPI_write_reg(char reg, char value)
//     describe:  send the value to the reg
char SPI_write_reg(char reg, char value)
{
    char status1;
    CSN_clr;
    status1 = SPI_write(reg);   //����->�ӻ���д��ַ
    SPI_write(value);           //����->�ӻ���д����
    CSN_set;

    return status1;
}

//function_name:  SPI_read_buf(char reg, char *buf, char length)
//     describe:  get the value of reg and send it to buf
char SPI_read_buf(char reg, char *buf, char length)
{
    char status2, i;
    CSN_clr;
    status2 = SPI_write(reg);   //����->�ӻ���д��ַ
    for(i=0; i<length; i++)
    {
        buf[i] = SPI_write(0);  //�ӻ�->������������
                                //��ΪҪ��ȡ�ӻ������ݣ���SIP_write()���ε��ô���Ĳ�����Ϊ��
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
    status1 = SPI_write(reg);   //����->�ӻ���д��ַ
    for(i=0; i<length; i++)
    {
        SPI_write(*buf++);   //����->�ӻ���д����
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
    delay_us(600);    //ע�ⲻ��̫С
}

//function_name:  nRF24L01_RX(char *rxbuf)
//     describe:  send data to rxbuf once we get it
char nRF24L01_RX(char *rxbuf)
{
    char reg_value=0;
    char sta;
    sta = SPI_read(STATUS);  //��ȡnRF20L01��״̬�Ĵ������ж����ݽ������
    if(sta & 0x40)
    {
        CE_clr;
        SPI_read_buf(RD_RX_PLOAD, rxbuf, TX_PLOAD_WIDTH);
        reg_value=1;
    }
    SPI_write_reg(WRITE_REG + STATUS, sta);  //һ�����յ����ݺ�RX_DR TX_DS MAS_PT��Ϊ1
                                                //��д״̬�Ĵ���������жϱ�־

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
