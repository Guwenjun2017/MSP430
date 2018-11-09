/*
 * inf.h
 *
 *  Created on: 2018Äê11ÔÂ9ÈÕ
 *      Author: Jack
 */

#ifndef INF_H_
#define INF_H_
#include<msp430f169.h>
#include"lcd_1602.h"
#include"type.h"

void infrared_decode();
void get_value(uchar argu);
void init_port1();
void init_device();



#endif /* INF_H_ */
