/*
 * Bsp_can.h
 *
 *  Created on: Jun 14, 2024
 *      Author: auroranebulas
 */

#ifndef INC_BSP_CAN_H_
#define INC_BSP_CAN_H_

#include "can.h"


#define MOTOR_3508_FEEDBACK_ID_BASE 	0x200
#define MOTOR_CHASSIS_MAX_NUM     	4
#define MOTOR_GIMBAL_MAX_NUM     	2


typedef struct{
	uint16_t  	angle;//rang:0~8191(0x1FFF)
	uint16_t  	last_angle;
	int16_t  	current_get;//rang:-13000 ~ 13000
	int16_t 	current_set;//rang:-13000 ~ 13000
	uint8_t     hall;//rang:0 ~ 6
	uint16_t  	null;
}Motor_6025_Typedef;


void can_user_init(CAN_HandleTypeDef* hcan );
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void set_motor_value(uint16_t tx_ID, int16_t v1, int16_t v2, int16_t v3, int16_t v4);


#endif /* INC_BSP_CAN_H_ */
