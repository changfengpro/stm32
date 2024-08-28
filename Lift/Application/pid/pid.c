/*
 * pid.c
 *
 *  Created on: Jul 14, 2024
 *      Author: auroranebulas
 */

#include "pid.h"


pid_struct_t pid[5];


/**
  * @brief  初始化pid参数
  * @param  pid struct
  * @param  kp 比例系数
  * @param  ki 积分系数
  * @param  kd 微分系数
  * @param  i_max 积分项限幅
  * @param  out_max 输出限幅
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

