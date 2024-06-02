/*
 * pid.h
 *
 *  Created on: May 31, 2024
 *      Author: auroranebulas
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include "main.h"

int Double_Loop_Control(float position, float target_position, float current_speed, float desired_speed, int pos_I_max,int ver_I_max);
int Position_PID (int position,int target,int I_max);
float Incremental_PID (float Encoder,float Target,float I_max);
int Xianfu_voltage(int moto) ;//限制幅度的函数
void ver_PID_Init();
void pos_PID_Init();
 void I_xianfu(int max);


 typedef struct _pid_struct_t
 {
 	float kp;
 	float ki;
 	float kd;
 	float i_max;
 	float out_max;


 	float ref;
 	float fdb;
 	float err[2];

 	float p_out;
 	float i_out;
 	float d_out;
 	float output;
 }pid_struct_t;

typedef struct _PID
{
     float kp,ki,kd;

}PID;

#endif /* INC_PID_H_ */
