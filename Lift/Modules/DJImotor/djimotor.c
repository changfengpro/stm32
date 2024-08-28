/*
 * djimotor.c
 *
 *  Created on: Jul 14, 2024
 *      Author: auroranebulas
 */

#include "chassis_control.h"
#include "pid.h"
#include "Bsp_can.h"
#include "robot_cmd.h"
#include "chassis_control.h"


extern Motor_6025_Typedef motor_array[6]; //前四位为底盘，后两位为云台
extern rc RC_ctrl;
static float position[5];
static float last_position[5];
float Velocity[5];
extern float vt_lf, vt_rf, vt_lb, vt_rb;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim ->Instance == TIM2)
	{

		ChassisTask();

	}
}


void AngleSumCalc(int ID)
{
//	position[ID] = motor_array[ID].angle;
	float temp =   motor_array[ID].angle - last_position[ID];
	position[ID] +=  temp;


}

void Velocity_Calclate(int ID)
{
	AngleSumCalc(ID);

	Velocity[ID] = ((position[ID] - last_position[ID]) * (360.0 / 8192.0) ) * 166.66666667;

	last_position[ID] = motor_array[ID].angle;
}


void DJIMotorStop()
{
	set_motor_value(0x200, 0, 0, 0, 0);
}


