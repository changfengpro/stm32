/*
 * djimotor.h
 *
 *  Created on: Jul 14, 2024
 *      Author: auroranebulas
 */

#ifndef DJIMOTOR_DJIMOTOR_H_
#define DJIMOTOR_DJIMOTOR_H_

#include "Bsp_can.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void Velocity_Calclate(int ID);
void AngleSumCalc(int ID);
void DJIMotorStop();

#endif /* DJIMOTOR_DJIMOTOR_H_ */
