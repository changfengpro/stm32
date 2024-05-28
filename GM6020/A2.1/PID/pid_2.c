#include <pid_2.h>
#include <main.h>
#include <bsp_can.h>
#include "stdio.h"
#define TARGET_SPEED_MAX    200    // 目标速度的最大值
#define ANGLE_DEADZONE 1 // 角度死区阈值，单位：度


PID ver_PID;
PID pos_PID;

float pos_Bias,ver_Bias,velocity_sum;
int voltage_output,ver_voltage =0;
extern float target_speed;
extern float motor1_speed;
float pos_voltage = 0;
void ver_PID_Init()
{
	ver_PID.kp = 45;
	ver_PID.ki = 3;
	ver_PID.kd = 0.05;
}
void pos_PID_Init()
{
	pos_PID.kp = 0.1;
	pos_PID.ki = 0;
	pos_PID.kd = 0.;
}

int Double_Loop_Control(float position, float target_position, float current_speed, float desired_speed, int pos_I_max,int ver_I_max)
{
	
    
int position_error = target_position - position;

    // 角度死区处理
    if (position_error < ANGLE_DEADZONE && position_error > -ANGLE_DEADZONE) 
	{
        target_speed = 0; // 在死区内，目标速度置零
    } 
	else {
		
		// 位置环控制
		target_speed = Position_PID(position, target_position,pos_I_max);
		/* 目标速度上限处理 */
		if (target_speed > desired_speed)// 目标速度的最大值
		{
			target_speed = desired_speed;
		}
		else if (target_speed < -desired_speed)
		{
			target_speed = -desired_speed;
		}
	}
	

    // 速度环控制
    voltage_output = Incremental_PID(current_speed, target_speed,ver_I_max);

	
    return voltage_output;
}


/*											多环控制  										*/
/*
*	函数功能：位置式PID控制器
*	入口参数：编码器测量位置信息，目标位置
*	返回  值：电机PWM
*	根据位置式离散PID公式 
*	pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
*	e(k)代表本次偏差 
*	e(k-1)代表上一次的偏差  
*	∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
*	pwm代表输出
**************************************************************************/
int Position_PID (int position,int target,int I_max)
{   
	static float Integral_bias,Last_Bias;
	pos_Bias=target-position;                                  //计算偏差
	Integral_bias+=pos_Bias;	                                 //求出偏差的积分
	if(Integral_bias>I_max)Integral_bias=I_max;				//对积分 限幅
	if(Integral_bias<-I_max)Integral_bias=-I_max;		//积分限幅 防止到达目标位置后过冲
	
	Last_Bias=pos_Bias;                                       //保存上一次偏差 
	// if(pos_Bias < 10)
	// {
	// 	Pwm = 0;
	// }
	pos_voltage = pos_PID.kp*pos_Bias+pos_PID.ki*Integral_bias+pos_PID.kd*(pos_Bias-Last_Bias);       //位置式PID控制器
	return pos_voltage;                                           //增量输出
}


/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
// int Incremental_PID (int Encoder,int Target)
// {
// 	float filt_velocity,velocity,last_filt_velocity;
//     float a=0.3;
//     filt_velocity = a*velocity+(1-a)*last_filt_velocity;
//     velocity_sum += filt_velocity;
//     I_xianfu(3500);
//     last_filt_velocity = filt_velocity;

//     return  ver_PID.kp*filt_velocity + ver_PID.ki*velocity_sum + ver_PID.kd * (filt_velocity - last_filt_velocity);
// }
float Incremental_PID (float Encoder,float Target,float I_max)
{ 	
		
	static float last_bias,Bias_sum,P_out,I_out,d_out;
	last_bias = ver_Bias;
	ver_Bias=Target-Encoder;                //计算偏差
	
	P_out = ver_PID.kp *ver_Bias;
	I_out += ver_PID.ki *ver_Bias;
	LIMIT_MIN_MAX(I_out,-I_max,I_max);
	d_out = ver_PID.kd *(ver_Bias - last_bias);

//	ver_voltage = ver_PID.kp*ver_Bias+ver_PID.ki*Bias_sum +ver_PID.kd*(ver_Bias - Last_bias);   //增量式PI控制器
	ver_voltage = P_out + I_out + d_out;
	return ver_voltage;                         //增量输出
}


/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
int Xianfu_voltage(int moto) //限制幅度的函数
 {
   int Amplitude=30000;  //===PWM满幅是8400 限制7000
	 if(moto<-Amplitude)  moto = -Amplitude;
	 if(moto>Amplitude)   moto =  Amplitude;
	 return moto;
 }
 void I_xianfu(int max)
{
	if(velocity_sum>max)  velocity_sum=max;
	if(velocity_sum<-max) velocity_sum=-max;
}


 void motor_angle_sum_calc(int Now_angle)
 {



 }
