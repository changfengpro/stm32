#include "PID.h"
#include "stdio.h"
#define TARGET_SPEED_MAX    3    // 目标速度的最大值
#define ANGLE_DEADZONE 1 // 角度死区阈值，单位：度

PID ver_PID;
PID pos_PID;

float pos_Bias,ver_Bias,velocity_sum;
int pwm_output,ver_Pwm =0;
float target_speed;
extern float motor1_speed;
float pos_Pwm = 0;
void ver_PID_Init()
{
	ver_PID.kp = 300;
	ver_PID.ki = 0;
	ver_PID.kd = 0;
}
void pos_PID_Init()
{
	pos_PID.kp = 0.1;
	pos_PID.ki = 0;
	pos_PID.kd = 0;
}

int Double_Loop_Control(float position, float target_position, float current_speed, float desired_speed)
{
	
    
int position_error = target_position - position;

    // 角度死区处理
    if (position_error < ANGLE_DEADZONE && position_error > -ANGLE_DEADZONE) 
	{
        target_speed = 0; // 在死区内，目标速度置零
    } 
	else {
		
		// 位置环控制
		target_speed = Position_PID(position, target_position);
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
    pwm_output = Incremental_PID(current_speed, target_speed);

	
    return pwm_output;
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
int Position_PID (int position,int target)
{ 
	static float Pwm,Integral_bias,Last_Bias;
	pos_Bias=target-position;                                  //计算偏差
	Integral_bias+=pos_Bias;	                                 //求出偏差的积分
	if(Integral_bias>400)Integral_bias=400;				//对积分 限幅
	if(Integral_bias<-400)Integral_bias=-400;		//积分限幅 防止到达目标位置后过冲
	
	Last_Bias=pos_Bias;                                       //保存上一次偏差 
	// if(pos_Bias < 10)
	// {
	// 	Pwm = 0;
	// }
	pos_Pwm=pos_PID.kp*pos_Bias+pos_PID.ki*Integral_bias+pos_PID.kd*(pos_Bias-Last_Bias);       //位置式PID控制器
	return pos_Pwm;                                           //增量输出
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
int Incremental_PID (float Encoder,float Target)
{ 	
		
	static float Last_bias,llast_bias,Bias_sum;
	ver_Bias=Target-Encoder;                //计算偏差
	Bias_sum += ver_Bias;
	llast_bias = Last_bias;
		                   //保存上一次偏差 
	// if(pos_Bias < 10)
	// {
	// 	Pwm = 0;
	// }
	// if(ver_Bias < 0.1)
	// {
	// 	ver_Bias = 0;
	// 	Last_bias = 0;
	// 	llast_bias = 0;
	// }
	I_xianfu(8000);
	ver_Pwm = ver_PID.kp*ver_Bias+ver_PID.ki*Bias_sum +ver_PID.kd*(ver_Bias - Last_bias);   //增量式PI控制器
	Last_bias=ver_Bias;
	return ver_Pwm;                         //增量输出
}


/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
int Xianfu_Pwm(int moto) //限制幅度的函数
 {
   int Amplitude=3500;  //===PWM满幅是8400 限制7000
	 if(moto<-Amplitude)  moto = -Amplitude;
	 if(moto>Amplitude)   moto =  Amplitude;
	 return moto;
 }
 void I_xianfu(int max)
{
	if(velocity_sum>max)  velocity_sum=max;
	if(velocity_sum<-max) velocity_sum=-max;
}
