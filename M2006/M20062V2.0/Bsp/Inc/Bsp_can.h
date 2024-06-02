/*
 * Bsp_can.h
 *
 *  Created on: May 31, 2024
 *      Author: auroranebulas
 */

#ifndef INC_BSP_CAN_H_
#define INC_BSP_CAN_H_

#include "can.h"

#define MOTOR_MAX_NUM 7
#define M2006 10000
#define GM6020 30000


/*CAN发送或是接收的ID*/
typedef enum
{

	CAN_2006Moto_ALL_ID = 0x200,
	CAN_2006Moto1_ID = 0x201,
	CAN_2006Moto2_ID = 0x202,
	CAN_2006Moto3_ID = 0x203,
	CAN_2006Moto4_ID = 0x204,

}CAN_Message_ID;

typedef struct
{
    uint16_t can_id;
    int16_t  set_current;
    uint16_t rotor_angle;
    int16_t  rotor_speed;
    int16_t  torque_current;
    uint8_t  temp;
}moto_info_t;

void set_motor_current(uint16_t tx_ID, int16_t v1, int16_t v2, int16_t v3, int16_t v4);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);


#endif /* INC_BSP_CAN_H_ */
