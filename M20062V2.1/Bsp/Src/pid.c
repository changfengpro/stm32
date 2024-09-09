/*
 * pid.c
 *
 *  Created on: May 31, 2024
 *      Author: auroranebulas
 */


#include "pid.h"
#include <main.h>
#include <Bsp_can.h>
#include "stdio.h"
#define TARGET_SPEED_MAX    200    // 目标速度的最大值
#define ANGLE_DEADZONE 1 // 角度死区阈值，单位：度
#define LIMIT_MIN_MAX(x,min,max) (x) = (((x)<=(min))?(min):(((x)>=(max))?(max):(x)))


extern int target_speed[5];
extern int target_position[5];
extern int desired_speed[5];
float Double_output[7];

/**
  * @brief  双环控制
  * @param  pid struct
    @param  parameter
  * @retval None
  */

float Double_Loop_Control(int ID, pid_struct_t *ver_pid, pid_struct_t *pos_pid, float current_position, float current_speed, float desired_speed, float target_position )
{




			// 位置环控制
			desired_speed = pos_pid_calc(pos_pid, target_position, current_position);
			/* 目标速度上限处理 */
			if (current_speed > desired_speed)// 目标速度的最大值
			{
				current_speed = desired_speed;
			}
			else if (current_speed < -desired_speed)
			{
				current_speed = -desired_speed;
			}



	    // 速度环控制
	    Double_output[ID] = ver_pid_calc(ver_pid, desired_speed, current_speed);


	    return Double_output[ID];
}





/**
  * @brief  初始化pid参数
  * @param  pid struct
    @param  parameter
  * @retval None
  */
void pid_init(pid_struct_t *pid,
              float kp,
              float ki,
              float kd,
              float i_max,
              float out_max)
{
  pid->kp      = kp;
  pid->ki      = ki;
  pid->kd      = kd;
  pid->i_max   = i_max;
  pid->out_max = out_max;
}


/**
  * @brief  角度pid
  * @param  pid struct
    @param  reference value
    @param  feedback value
  * @retval calculation result
  */
float pos_pid_calc(pid_struct_t *pid, float ref, float fdb)
{
	pid->ref = ref;
	  pid->fdb = fdb;
	  pid->err[1] = pid->err[0];
	  pid->err[0] = pid->ref - pid->fdb;

	  pid->p_out  = pid->kp * pid->err[0];
	  pid->i_out += pid->ki * pid->err[0];
	  pid->d_out  = pid->kd * (pid->err[0] - pid->err[1]);
	  LIMIT_MIN_MAX(pid->i_out, -pid->i_max, pid->i_max);

	  pid->output = pid->p_out + pid->i_out + pid->d_out;
	  LIMIT_MIN_MAX(pid->output, -pid->out_max, pid->out_max);
	  return pid->output;
}


/**
  * @brief  速度pid
  * @param  pid struct
    @param  reference value
    @param  feedback value
  * @retval calculation result
  */
float ver_pid_calc(pid_struct_t *pid, float ref, float fdb)
{
  pid->ref = ref;
  pid->fdb = fdb;
  pid->err[1] = pid->err[0];
  pid->err[0] = pid->ref - pid->fdb;

  pid->p_out  = pid->kp * pid->err[0];
  pid->i_out += pid->ki * pid->err[0];
  pid->d_out  = pid->kd * (pid->err[0] - pid->err[1]);
  LIMIT_MIN_MAX(pid->i_out, -pid->i_max, pid->i_max);

  pid->output = pid->p_out + pid->i_out + pid->d_out;
  LIMIT_MIN_MAX(pid->output, -pid->out_max, pid->out_max);
  return pid->output;
}
