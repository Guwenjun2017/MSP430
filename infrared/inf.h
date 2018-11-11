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

void infrared_decode();
void set_value(unsigned char);
void init_port1();
void init_device();



#endif /* INF_H_ */
