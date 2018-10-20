/*
 * UART.h
 *
 *  Created on: 2018Äê10ÔÂ20ÈÕ
 *      Author: Jack
 */
#include<msp430f169.h>
#include"type.h"
#include"delay.h"
#include"lcd_1602.h"
#ifndef UART_H_
#define UART_H_

void UART_Init();
void Send_Byte(uchar data);



#endif /* UART_H_ */
