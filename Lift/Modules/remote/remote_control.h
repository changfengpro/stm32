/*
 * remote_control.h
 *
 *  Created on: Jul 21, 2024
 *      Author: auroranebulas
 */

#ifndef REMOTE_REMOTE_CONTROL_H_
#define REMOTE_REMOTE_CONTROL_H_


#include "main.h"
#include "usart.h"

//用于遥控器数据读取，遥控器数据是一个大小为2的数组
#define LAST 1
#define TEMP 0

// 检查接收值是否出错
#define RC_CH_VALUE_MIN ((uint16_t)364)
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)

/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP ((uint16_t)1)   // 开关向上时的值
#define RC_SW_MID ((uint16_t)3)  // 开关中间时的值
#define RC_SW_DOWN ((uint16_t)2) // 开关向下时的值
// 三个判断开关状态的宏
#define switch_is_down(s) (s == RC_SW_DOWN)
#define switch_is_mid(s) (s == RC_SW_MID)
#define switch_is_up(s) (s == RC_SW_UP)


#endif /* REMOTE_REMOTE_CONTROL_H_ */
