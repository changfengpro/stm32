/*
 * pid.h
 *
 *  Created on: Jul 14, 2024
 *      Author: auroranebulas
 */

#ifndef PID_PID_H_
#define PID_PID_H_

#define LIMIT_MIN_MAX(x,min,max) (x) = (((x)<=(min))?(min):(((x)>=(max))?(max):(x)))


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

 void pid_init(pid_struct_t *pid,
               float kp,
               float ki,
               float kd,
               float i_max,
               float out_max);
 float ver_pid_calc(pid_struct_t *pid, float ref, float fdb);

#endif /* PID_PID_H_ */
