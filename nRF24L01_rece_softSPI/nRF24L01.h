/*
 * nRF24L01.h
 *
 *  Created on: 2018��10��28��
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

//nRF24L01�Ĵ���ָ��
#define READ_REG   0x00   //���Ĵ���
#define WRITE_REG  0x20   //д�Ĵ���
#define RD_RX_PLOAD  0x61  //��ȡ��������ָ��
#define WR_TX_PLOAD  0xA0  //д��������ָ��
#define FLUSH_TX   0xE1   //���·���FIFOָ��
#define FLUSH_RX   0xE2   //���½���FIFOָ��
#define REUSE_TX_PL   0xE3   //�����ظ�װ������ָ��
#define NOP1   0xFF    //����

//SPI(nRF24L01)�Ĵ�����ַ
#define CONFIG          0x00   //�����շ�״̬��CRCУ��ģʽ���շ�״̬��Ӧ��ʽ
#define EN_AA           0x01   //�Զ�Ӧ��������
#define EN_RXADDR       0x02   //�����ŵ�����
#define SETUP_AW        0x03   //�շ���ַ�������
#define SETUP_RETR      0x04   //�Զ��ط���������
#define RF_CH           0X05     //��Ƶͨ��
#define RF_SETUP        0X06     //��Ƶ�Ĵ���
#define STATUS          0X07     //״̬�Ĵ���
#define OBSERVE_TX      0X08     //���ͼ��Ĵ���
#define CD              0X09     //�ز����
#define RX_ADDR_P0      0X0A     //����ͨ��0���յ�ַ
#define RX_ADDR_P1      0X0B     //����ͨ��1���յ�ַ
#define RX_ADDR_P2      0X0C     //����ͨ��2���յ�ַ
#define RX_ADDR_P3      0X0D     //����ͨ��3���յ�ַ
#define RX_ADDR_P4      0X0E     //����ͨ��4���յ�ַ
#define RX_ADDR_P5      0X0F     //����ͨ��5���յ�ַ
#define TX_ADDR         0X10     //���͵�ַ
#define RX_PW_P0        0X11     //��������ͨ��0��Ч���ݿ��
#define RX_PW_P1        0X12     //��������ͨ��1��Ч���ݿ��
#define RX_PW_P2        0X13     //��������ͨ��2��Ч���ݿ��
#define RX_PW_P3        0X14     //��������ͨ��3��Ч���ݿ��
#define RX_PW_P4        0X15     //��������ͨ��4��Ч���ݿ��
#define RX_PW_P5        0X16     //��������ͨ��5��Ч���ݿ��
#define FIFO_STATUS     0X17     //FIFO ״̬�Ĵ���


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
