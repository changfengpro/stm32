#include "PID.h"
#include "encoder.h"
/// @brief 

extern float kp, ki, kd;  // 直立环参数
float err;                // 此次误差
float last_err = 0;       // 上次误差，初始值设为0
float err_sum = 0;        // 误差累积值，初始值设为0
float err_difference;     // 误差变化量

extern float vkp,vki;    
float filt_velocity;     
float last_filt_velocity;
float velocity_sum=0;    

int vertical_PID_value(float measure, float calcu)
{
    err = measure - calcu; // 计算当前误差
    err_sum += err;
    err_difference =err - last_err;
    last_err = err;
    return kp*err + ki*err_sum + kd*err_difference;
   
}
int velocity_PID_vaule(int velocity)
{
    float a=0.3;
    filt_velocity = a*velocity+(1-a)*last_filt_velocity;
    velocity_sum += filt_velocity;
    I_xianfu(3000);
    last_filt_velocity = filt_velocity;

    return  vkp*filt_velocity + vki*velocity_sum;
}

void I_xianfu(int max)
{
	if(velocity_sum>max)  velocity_sum=max;
	if(velocity_sum<-max) velocity_sum=-max;
}