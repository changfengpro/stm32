/*
 * motor.h
 *
 *  Created on: 2023年7月17日
 *      Author: Qi520503
 */

#ifndef MOTOR_H_
#define MOTOR_H_
#include "main.h"

extern	int myabs(int num);
void Set_Pwmb(int motor_right);
void Set_Pwma(int motor_left);
int myabs(int num);
void Motor_Right();
void Motor_Left();

#endif /* MOTOR_H_ */
