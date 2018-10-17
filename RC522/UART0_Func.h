
/*
 * UART0_Func.h
 *
 *  Created on: 2018Äê10ÔÂ13ÈÕ
 *      Author: Jack
 */

#ifndef UART0_FUNC_H_
#define UART0_FUNC_H_

void InitUART(void);
void Send1Char(unsigned char sendchar);
unsigned char Get1Char(void);
void PutString(unsigned char *ptr);
void PutString0(unsigned char *ptr);
void DisplayConfigMenu(void);
void tochar(unsigned char id);




#endif /* UART0_FUNC_H_ */
