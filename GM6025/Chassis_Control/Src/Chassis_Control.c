/*
 * Chassis_Control.c
 *
 *  Created on: Jul 3, 2024
 *      Author: auroranebulas
 */

#include "Chassis_Control.h"

#define DATA_LENGTH_CHASSIS ((uint8_t) 20)
int16_t speed_set_chassis[4];
static float speed_filtered_chassis[4] = {0};
int16_t value[4];
int16_t value_set_chassis[4] ={ 0 };

void Chassis_Control(Motor_6025_Typedef *motor_chassis_array)
{
	static uint8_t init_flag = 1;
		uint8_t i;
		static PID_Typedef pid_speed_motor_chassis[4];
		static float relative_angle_chassis[4] ={0, 0, 0 ,0};
		static float in_data[4][DATA_LENGTH_CHASSIS] = {0};
		float angle_increment_chassis[4] = {0};
		float speed_chassis[4] = {0};


		if(init_flag == 1)
		{
			Pid_Init_Speed_Chassis(&pid_speed_motor_chassis[0]);
			for(i = 0; i < 4; i++)
			{
					motor_chassis_array[i].last_angle = motor_chassis_array[i].angle;
			}
			init_flag = 0;
		}
		RC_Chassis_DataUpdate(speed_set_chassis);
		for(i = 0; i < 4; i++)
		{
			angle_increment_chassis[i] = Angle_Calculate(motor_chassis_array[i].angle / 8191.0 * 360.0 ,
													motor_chassis_array[i].last_angle / 8191.0 * 360.0);
			relative_angle_chassis[i] += angle_increment_chassis[i];
			speed_chassis[i] = angle_increment_chassis[i] * 60000.0 / 360.0;
			speed_filtered_chassis[i] = Average_Filter(&in_data[i][0], speed_chassis[i], DATA_LENGTH_CHASSIS);

			value_set_chassis[i] = Pid_Calculate(&pid_speed_motor_chassis[i], speed_filtered_chassis[i], speed_set_chassis[i]);
			}
			for(i = 0; i < 4; i++)
			{

			motor_chassis_array[i].last_angle = motor_chassis_array[i].angle;
		}
		for(int i=0;i<4;i++)
		{
			value[i] = value_set_chassis[i];

		}

}


void Pid_Init_Speed_Chassis(PID_Typedef *pid)
{
	for(uint8_t i = 0; i < 4; i++)
	{
		pid[i].param.Kd = 0.04;
		pid[i].param.Ki = 0.09;
		pid[i].param.Kp = 10.00;

		pid[i].limit.max_err_input = 3300;
		pid[i].limit.max_i_out = 4000;
		pid[i].limit.max_total_out = 5000;
	}
}
