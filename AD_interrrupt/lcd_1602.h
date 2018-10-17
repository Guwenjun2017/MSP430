/*
 * lcd_1602.h
 *
 *  Created on: 2018��10��5��
 *      Author: Jack
 */

#ifndef LCD_1602_H_
#define LCD_1602_H_

//RS�˿�
#define set_RS P5OUT |= BIT5
#define clear_RS P5OUT &= ~BIT5
//RW�˿�
#define set_RW P5OUT |= BIT6
#define clear_RW P5OUT &= ~BIT6
//Enable�˿�
#define set_E P5OUT |= BIT7
#define clear_E P5OUT &= ~BIT7
//���ݶ�P4.0~P4.7
#define dataout P4DIR = 0xff  //��P4�˿�ȫ���
#define dataport P4OUT




#endif /* LCD_1602_H_ */