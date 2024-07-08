/*
 * Algorithms_Lib.h
 *
 *  Created on: Jul 3, 2024
 *      Author: auroranebulas
 */

#ifndef INC_ALGORITHMS_LIB_H_
#define INC_ALGORITHMS_LIB_H_

#include "main.h"
#include "math.h"
#include "Bsp_can.h"

#define PI ((uint16_t)360)

typedef struct{
	float output;//输出
	float filter_factor;//滤波系数，系数越小收敛越快
	float frame_period;//运算时间间隔，单位 S/秒
}LOW_PASS_FILTER_Typedef;

void low_pass_filter_init(LOW_PASS_FILTER_Typedef *filter_type, float filter_factor, float frame_period);
float low_pass_filter(LOW_PASS_FILTER_Typedef *filter_type, float input);

float Average_Filter(float *in_data, float data, uint8_t data_length);

float Angle_Calculate(float absolute_angle, float last_angle);
float invSqrt(float num);
float limit_control(float input, float limit);

#endif /* INC_ALGORITHMS_LIB_H_ */
