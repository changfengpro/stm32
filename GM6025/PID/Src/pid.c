/*
 * pid.c
 *
 *  Created on: Jul 3, 2024
 *      Author: auroranebulas
 */


#include "pid.h"
#include <main.h>
#include <Bsp_can.h>
#include "stdio.h"
float Pid_Calculate(PID_Typedef *pid, float get, float set)
{
//	uint8_t flag = 0;
	//get input
	pid->input.get = get;
	pid->input.set = set;
	pid->input.last_err = pid->input.err;
	pid->input.err = set - get;

	if ((pid->limit.max_err_input != 0) && (fabs(pid->input.err) > pid->limit.max_err_input))
		return 0;

	//output calculate
	pid->output.p_out  = pid->param.Kp * pid->input.err;
	pid->output.i_out += pid->param.Ki * pid->input.err;
	pid->output.d_out  = pid->param.Kd * (pid->input.err - pid->input.last_err) * 1000;
	//output limit
	pid->output.i_out	  = limit_control(pid->output.i_out, pid->limit.max_i_out);
	pid->output.total_out = pid->output.p_out + pid->output.i_out + pid->output.d_out;
	pid->output.total_out = limit_control(pid->output.total_out, pid->limit.max_total_out);

	return pid->output.total_out;
}
