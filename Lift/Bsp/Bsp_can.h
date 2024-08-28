/*
 * Bsp_can.h
 *
 *  Created on: Jul 14, 2024
 *      Author: auroranebulas
 */

#ifndef BSP_CAN_H_
#define BSP_CAN_H_

#include "can.h"


#define MOTOR_3508_FEEDBACK_ID_BASE 	0x200
#define MOTOR_CHASSIS_MAX_NUM     	4
#define MOTOR_GIMBAL_MAX_NUM     	2
#define MOTOR_MAX_NUM         9

typedef struct{
	int16_t  	angle;//rang:0~8191(0x1FFF)
	int16_t  	last_angle;
	int16_t  	current_get;//rang:-13000 ~ 13000
	int16_t 	current_set;//rang:-13000 ~ 13000
	uint8_t     hall;//rang:0 ~ 6
	uint16_t  	null;
}Motor_6025_Typedef;

typedef struct
{
    uint16_t can_id;
    int16_t  set_current;
    uint16_t rotor_angle;
    int16_t  rotor_speed;
    int16_t  torque_current;
    uint8_t  temp;
}moto_info_t;


void can_user_init(CAN_HandleTypeDef* hcan );
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void set_motor_value_CAN1(uint16_t tx_ID, int16_t v1, int16_t v2, int16_t v3, int16_t v4);
void set_motor_value_CAN2(uint16_t tx_ID, int16_t v1, int16_t v2, int16_t v3, int16_t v4);

#endif /* BSP_CAN_H_ */
