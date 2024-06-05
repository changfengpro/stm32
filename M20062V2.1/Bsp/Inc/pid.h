/*
 * pid.h
 *
 *  Created on: May 31, 2024
 *      Author: auroranebulas
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include "main.h"





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


 float ver_pid_calc(pid_struct_t *pid, float ref, float fdb);
 void pid_init(pid_struct_t *pid,
               float kp,
               float ki,
               float kd,
               float i_max,
               float out_max);
 float pos_pid_calc(pid_struct_t *pid, float ref, float fdb);


#endif /* INC_PID_H_ */
