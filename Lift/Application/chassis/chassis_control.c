/*
 * chassis_control.c
 *
 *  Created on: Jul 14, 2024
 *      Author: auroranebulas
 */


#include "chassis_control.h"
#include "robot_def.h"
#include "arm_math.h"
#include "Bsp_can.h"
#include "robot_cmd.h"
#include "pid.h"
#include "djimotor.h"

#define HALF_WHEEL_BASE (WHEEL_BASE / 2.0f)    	//半轴距
#define HALF_TRACK_WIDTH (TRACK_WIDTH / 2.0f)	//半轮距
#define PERIMETER_WHEEL （RADIUS_WHEEL  * 2 * PI)
#define DEGREE_2_RAD 0.01745329252f // pi/180

#define LF_CENTER ((HALF_TRACK_WIDTH + HALF_WHEEL_BASE) * DEGREE_2_RAD)
#define RF_CENTER ((HALF_TRACK_WIDTH + HALF_WHEEL_BASE) * DEGREE_2_RAD)
#define LB_CENTER ((HALF_TRACK_WIDTH + HALF_WHEEL_BASE) * DEGREE_2_RAD)
#define RB_CENTER ((HALF_TRACK_WIDTH + HALF_WHEEL_BASE) * DEGREE_2_RAD)
Chassis_Ctrl_Cmd_s chassis_cmd_recv; //底盘接收到的控制命令
float vt_lf, vt_rf, vt_lb, vt_rb;
extern rc RC_ctrl;
extern pid_struct_t pid[5];
float pid_ref[5];
extern float Velocity[5];
static uint32_t count;
static uint32_t count_3;

void MecanumCalculate()
{
    chassis_cmd_recv.vx = -RC_ctrl.ch0 * 0.68f;
	chassis_cmd_recv.vy = -RC_ctrl.ch1 * 0.68f;
	chassis_cmd_recv.wz = -RC_ctrl.ch2 * 0.68f;


    vt_lf = -chassis_cmd_recv.vx - chassis_cmd_recv.vy - chassis_cmd_recv.wz * LF_CENTER;
	vt_rf = -chassis_cmd_recv.vx + chassis_cmd_recv.vy - chassis_cmd_recv.wz * RF_CENTER;
	vt_lb =  chassis_cmd_recv.vx - chassis_cmd_recv.vy - chassis_cmd_recv.wz * LB_CENTER;
	vt_rb =  chassis_cmd_recv.vx + chassis_cmd_recv.vy - chassis_cmd_recv.wz * RB_CENTER;
}

void ChassisTask()
{
    MecanumCalculate();

    
}