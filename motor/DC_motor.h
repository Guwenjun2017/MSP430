/*
 * SG90.h
 *
 *  Created on: 2018��10��15��
 *      Author: Jack
 */
#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

void Init_PWM();
void start();
void stop();
void speed_add();
void speed_sub();
void show_status(unsigned char status, unsigned int speed);;


#endif /* DC_MOTOR_H_ */
