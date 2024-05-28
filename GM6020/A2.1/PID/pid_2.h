#ifndef __PID_H
#define __PID_H
#include "main.h"
int Double_Loop_Control(float position, float target_position, float current_speed, float desired_speed, int pos_I_max,int ver_I_max);
int Position_PID (int position,int target,int I_max);
float Incremental_PID (float Encoder,float Target,float I_max);
int Xianfu_voltage(int moto) ;//限制幅度的函数
void ver_PID_Init();
void pos_PID_Init();
 void I_xianfu(int max);

typedef struct _PID
{
     float kp,ki,kd;
       
}PID;


#endif
