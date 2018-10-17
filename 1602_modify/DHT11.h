/*
 * DHT11.h
 *
 *  Created on: 2018Äê10ÔÂ7ÈÕ
 *      Author: Jack
 */

#ifndef DHT11_H_
#define DHT11_H_
#define DHT11_SDA_OUT P6OUT |= BIT7
#define DHT11_SDA_IN  P6OUT &= ~BIT7

#define DHT11_SDA_set P6OUT |= BIT7
#define DHT11_SDA_clr P6OUT &= ~BIT7

void DHT11_start();
unsigned char receive();
void DHT11_end();

#endif /* DHT11_H_ */
