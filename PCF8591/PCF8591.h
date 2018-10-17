/*
 * PCF8591.h
 *
 *  Created on: 2018Äê10ÔÂ17ÈÕ
 *      Author: Jack
 */

#ifndef PCF8591_H_
#define PCF8591_H_

void I2C_start();
void I2C_write_byte(unsigned char data);
unsigned char I2C_read_byte();
void I2C_ack();
void I2C_not_ack();
void I2C_stop();





#endif /* PCF8591_H_ */
