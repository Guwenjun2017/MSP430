/*
 * lcd_1602.h
 *
 *  Created on: 2018年10月5日
 *      Author: Jack
 */

#ifndef LCD_1602_H_
#define LCD_1602_H_

//RS端口
#define set_RS P5OUT |= BIT5
#define clear_RS P5OUT &= ~BIT5
//RW端口
#define set_RW P5OUT |= BIT6
#define clear_RW P5OUT &= ~BIT6
//Enable端口
#define set_E P5OUT |= BIT7
#define clear_E P5OUT &= ~BIT7
//数据端P4.0~P4.7
#define dataout P4DIR = 0xff  //将P4端口全输出
#define dataport P4OUT




#endif /* LCD_1602_H_ */
