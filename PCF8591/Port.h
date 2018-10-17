/*
 * Port.h
 *
 *  Created on: 2018Äê10ÔÂ17ÈÕ
 *      Author: Jack
 */

#ifndef PORT_H_
#define PORT_H_

#define SDA_IN  P1DIR &= BIT2
#define SCL_IN  P1DIR &= BIT3
#define SDA_OUT P1DIR |= BIT2
#define SCL_OUT P1DIR |= BIT3
#define SDA_set P1OUT |= BIT2
#define SDA_clr P1OUT &= ~BIT2
#define SCL_set P1OUT |= BIT3
#define SCL_clr P1OUT &= ~BIT3





#endif /* PORT_H_ */
