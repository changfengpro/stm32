/*
 * encoder.c
 *
 *  Created on: Nov 18, 2023
 *      Author: lenovo
 */

#include "stm32f1xx.h"
int16_t Encoder_Count;

int16_t Encoder_Get(){

	int16_t Temp;
	Temp = TIM3->CNT;
	return Temp;


}
