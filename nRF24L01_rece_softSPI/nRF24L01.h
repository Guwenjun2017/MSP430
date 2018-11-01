/*
 * nRF24L01.h
 *
 *  Created on: 2018年10月28日
 *      Author: Jack
 */

#ifndef NRF24L01_H_
#define NRF24L01_H_



#define CE_set P1OUT |= BIT6
#define CE_clr P1OUT &= ~BIT6

#define CSN_set P3OUT |= BIT0
#define CSN_clr P3OUT &= ~BIT0

#define SCK_set P3OUT |= BIT3
#define SCK_clr P3OUT &= ~BIT3

#define MOSI_set P3OUT |= BIT1
#define MOSI_clr P3OUT &= ~BIT1

#define MISO_set P3OUT |= BIT2
#define MISO_clr P3OUT &= ~BIT2

#define MISO_input_1 P3IN & BIT2

#define TX_ADR_WIDTH 5
#define RX_ADR_WIDTH 5
#define TX_PLOAD_WIDTH 32
#define RX_PLOAD_WIDTH 32

//nRF24L01寄存器指令
#define READ_REG   0x00   //读寄存器
#define WRITE_REG  0x20   //写寄存器
#define RD_RX_PLOAD  0x61  //读取接收数据指令
#define WR_TX_PLOAD  0xA0  //写待发数据指令
#define FLUSH_TX   0xE1   //重新发送FIFO指令
#define FLUSH_RX   0xE2   //重新接收FIFO指令
#define REUSE_TX_PL   0xE3   //定义重复装载数据指令
#define NOP1   0xFF    //保留

//SPI(nRF24L01)寄存器地址
#define CONFIG          0x00   //配置收发状态、CRC校验模式、收发状态响应方式
#define EN_AA           0x01   //自动应答功能设置
#define EN_RXADDR       0x02   //可用信道设置
#define SETUP_AW        0x03   //收发地址宽度设置
#define SETUP_RETR      0x04   //自动重发功能设置
#define RF_CH           0X05     //射频通道
#define RF_SETUP        0X06     //射频寄存器
#define STATUS          0X07     //状态寄存器
#define OBSERVE_TX      0X08     //发送检测寄存器
#define CD              0X09     //载波检测
#define RX_ADDR_P0      0X0A     //数据通道0接收地址
#define RX_ADDR_P1      0X0B     //数据通道1接收地址
#define RX_ADDR_P2      0X0C     //数据通道2接收地址
#define RX_ADDR_P3      0X0D     //数据通道3接收地址
#define RX_ADDR_P4      0X0E     //数据通道4接收地址
#define RX_ADDR_P5      0X0F     //数据通道5接收地址
#define TX_ADDR         0X10     //发送地址
#define RX_PW_P0        0X11     //接收数据通道0有效数据宽度
#define RX_PW_P1        0X12     //接收数据通道1有效数据宽度
#define RX_PW_P2        0X13     //接收数据通道2有效数据宽度
#define RX_PW_P3        0X14     //接收数据通道3有效数据宽度
#define RX_PW_P4        0X15     //接收数据通道4有效数据宽度
#define RX_PW_P5        0X16     //接收数据通道5有效数据宽度
#define FIFO_STATUS     0X17     //FIFO 状态寄存器


extern char TX_ADDRESS[5];
extern char RX_ADDRESS[5];

char sta;
char txbuf[32];
unsigned char rxbuf[32],temp[6];

void init_nRF24L01(void);
char nRF24L01_RX(char *);
void nRF24L01_TX(char *);
void set_RXmode(void);
char SPI_read(char);
char SPI_write(unsigned char);
char SPI_write_reg(char, char);
char SPI_write_buf(char, char *, char);



#endif /* NRF24L01_H_ */
