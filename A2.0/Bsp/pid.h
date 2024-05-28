


#ifndef _PID_H
#define _PID_H

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


void pid_init(pid_struct_t *pid,
			  float kp,
			  float ki,
			  float kd,
			  float i_max,
			  float out_max);

float pid_calc(pid_struct_t *pid, float ref,float fdb);


#endif
