/*
 * 4x4key.h
 *
 *  Created on: 2018Äê10ÔÂ11ÈÕ
 *      Author: Jack
 */

#ifndef MY_4X4KEY_H_
#define MY_4X4KEY_H_

unsigned char keyscan(void);
void calculator();
char string_to_long(char *buffer);
int calc_operation(char token);
void calc_disp(int status);
char *num_to_string(long num);



#endif /* 4X4KEY_H_ */
