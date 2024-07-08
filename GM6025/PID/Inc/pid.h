/*
 * pid.h
 *
 *  Created on: Jul 3, 2024
 *      Author: auroranebulas
 */

#ifndef PID_H_
#define PID_H_

#include "main.h"



typedef  struct{
	struct{
		float set;
		float get;
		float err;
		float last_err;
		float previous_err;
	}input;
	struct{
		float Kp;
		float Ki;
		float Kd;
	}param;
	struct{
		float p_out;
		float i_out;
		float d_out;
		float total_out;
	}output;

	struct{
		float max_err_input;
		float max_i_out;
		float max_total_out;
	}limit;
}PID_Typedef;

 typedef struct _pid_struct_t
 {
 	float kp;
 	float ki;
 	float kd;
 	float i_max;
 	float out_max;


 	int ref;
 	int fdb;
 	int err[2];

 	float p_out;
 	float i_out;
 	float d_out;
 	float output;
 }pid_struct_t;


 typedef struct _temp_struct_t
 {
	 float ver_kp;
	 float ver_ki;
	 float ver_kd;
	 float pos_kp;
	 float pos_ki;
	 float pos_kd;
 }temp_struct_t;

 void pid_init(pid_struct_t *pid,
               float kp,
               float ki,
               float kd,
               float i_max,
               float out_max);
 float Double_Loop_Control(int ID, pid_struct_t *ver_pid, pid_struct_t *pos_pid, float current_position, float current_speed, float desired_speed, float desired_position );
 float pos_pid_calc(pid_struct_t *pid, float ref, float fdb);
 float ver_pid_calc(pid_struct_t *pid, float ref, float fdb);


#endif /* INC_PID_H_ */


