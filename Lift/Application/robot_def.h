/*
 * robot_def.h
 *
 *  Created on: Jul 14, 2024
 *      Author: auroranebulas
 */

#ifndef ROBOT_DEF_H_
#define ROBOT_DEF_H_

// 底盘模式设置
/**
 * @brief 后续考虑修改为云台跟随底盘,而不是让底盘去追云台,云台的惯量比底盘小.
 *
 */
typedef enum
{
    CHASSIS_ZERO_FORCE = 0,    // 电流零输入q
    CHASSIS_ROTATE,            // 小陀螺模式
    CHASSIS_NO_FOLLOW,         // 不跟随，允许全向平移
    CHASSIS_FOLLOW_GIMBAL_YAW, // 跟随模式，底盘叠加角度环控制
} chassis_mode_e;

typedef struct
{
    // 控制部分
    float vx;           // 前进方向速度
    float vy;           // 横移方向速度
    float wz;           // 旋转速度
    float offset_angle; // 底盘和归中位置的夹角
    chassis_mode_e chassis_mode;
    int chassis_speed_buff;
    // UI部分
    //  ...

} Chassis_Ctrl_Cmd_s;

//机器人地盘修改的参数，单位为毫米(mm)
#define WHEEL_BASE 320         				//纵向轴距（前进后退方向）
#define TRACK_WIDTH 320        				//横向轴距（左右平移方向）
#define RADIUS_WHEEL 75             		//轮子半径
#define REDUCTION_RATIO_WHEEL 1.0f			//电机减速比
#define TIMER_FREQUENCY 1000 				//定时器频率 Hz
#define TIMER_TIME (1 / TIMER_FREQUENCY)	//定时器周期
#define MOTOR_ANGLE 360.0 / 8191.0		//电机反馈值到角度的转化
#define RC_SPEED (450.0 / 660.0) 			//遥控器最大值到电机速度最大值的转化

#endif /* ROBOT_DEF_H_ */
