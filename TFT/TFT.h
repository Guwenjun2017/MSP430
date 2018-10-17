/*
 * TFT.h
 *
 *  Created on: 2018年9月24日
 *      Author: Jack
 */

#ifndef TFT_H_
#define TFT_H_
#include"type.h"

#define RED       0xf800
#define YELLOW    0xffe0
#define GREEN     0x07e0
#define CYEN      0x07ff
#define BLUE      0x001f
#define PURPLE    0xf81f
#define BLACK     0x0000
#define WHITE     0xffff
#define GRAY      0x7bef
#define Lcd_Width    240
#define Lcd_Height   320

#define DataPort P4OUT
#define High_DataPort P6OUT //高8位数据口
#define Low_DataPort  P5OUT //低8为数据口

#define RS_set   P5OUT |= BIT5
#define RS_clr   P5OUT &= ~BIT5

#define RW_set   P5OUT |= BIT6
#define RW_clr   P5OUT &= ~BIT6

#define RD_set   P5OUT |= BIT7
#define RD_clr   P5OUT &= ~BIT7

#define CS_set   P5OUT |= BIT0
#define CS_clr   P5OUT &= ~BIT0

#define RST_set    P5OUT |= BIT3
#define RST_clr    P5OUT &= ~BIT3

#define LE_set   P5OUT |= BIT1
#define LE_clr   P5OUT &= ~BIT1

void write(uint);
void TFT_write_cmd(uint);
void TFT_write_data(uint);
void TFT_write_cmd_data(uint, uint);
void TFT_address_set(uint, uint, uint, uint);
void TFT_clear(uint color);
void TFT_Fill(uint xstart, uint ystart, uint xend, uint yend, uint color);
void TFT_draw_point(uint x, uint y, uint color);
void TFT_init();


#endif /* TFT_H_ */
