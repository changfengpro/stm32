/*
 * bsp_can.c
 *
 *  Created on: May 30, 2024
 *      Author: auroranebulas
 */
#include "bsp_can.h"

moto_info_t motor_info[MOTOR_MAX_NUM];
uint16_t can_cnt;


/*******************************************************************************************
  * @Func		my_can_filter_init
  * @Brief    CAN1和CAN2滤波器配置
  * @Param		CAN_HandleTypeDef* hcan
  * @Retval		None
  * @Date     2015/11/30
 *******************************************************************************************/
void my_can_filter_init()
{
	CAN_FilterTypeDef  can_filter;
	  can_filter.FilterBank = 0;                       // filter 0
	  can_filter.FilterMode =  CAN_FILTERMODE_IDMASK;  // mask mode
	  can_filter.FilterScale = CAN_FILTERSCALE_32BIT;
	  can_filter.FilterIdHigh = 0;
	  can_filter.FilterIdLow  = 0;
	  can_filter.FilterMaskIdHigh = 0;
	  can_filter.FilterMaskIdLow  = 0;                // set mask 0 to receive all can id
	  can_filter.FilterFIFOAssignment = CAN_RX_FIFO0; // assign to fifo0
	  can_filter.FilterActivation = ENABLE;           // enable can filter
	  can_filter.SlaveStartFilterBank  = 14;          // only meaningful in dual can mode

	  HAL_CAN_ConfigFilter(&hcan1, &can_filter);        // init can filter
	  HAL_CAN_Start(&hcan1);                          // start can1

	  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}

/**
  * @brief  can rx callback, get motor feedback info
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  CAN_RxHeaderTypeDef rx_header;
  uint8_t             rx_data[8];
  if(hcan->Instance == CAN1)
  {
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data); //receive can data
  }
  if ((rx_header.StdId >= CAN_2006Moto_ALL_ID)
   && (rx_header.StdId <  CAN_2006Moto_ALL_ID + MOTOR_MAX_NUM))                  // judge the can id
  {
    can_cnt ++;
    uint8_t index = rx_header.StdId - CAN_2006Moto_ALL_ID;                  // get motor index by can_id
    motor_info[index].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);
    motor_info[index].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);
    motor_info[index].torque_current = ((rx_data[4] << 8) | rx_data[5]);
    motor_info[index].temp           =   rx_data[6];
  }
}

/**
  * @brief  send motor control message through can bus
  * @param  id_range to select can control id 0x1ff or 0x2ff
  * @param  motor voltage 1,2,3,4 or 5,6,7
  * @retval None
  */
void set_motor_value(uint16_t txhead, int16_t v1, int16_t v2, int16_t v3, int16_t v4)
{
  CAN_TxHeaderTypeDef tx_header;
  uint8_t             tx_data[8];

  tx_header.StdId = txhead;
  tx_header.IDE   = CAN_ID_STD;
  tx_header.RTR   = CAN_RTR_DATA;
  tx_header.DLC   = 8;

  tx_data[0] = (v1>>8)&0xff;
  tx_data[1] =    (v1)&0xff;
  tx_data[2] = (v2>>8)&0xff;
  tx_data[3] =    (v2)&0xff;
  tx_data[4] = (v3>>8)&0xff;
  tx_data[5] =    (v3)&0xff;
  tx_data[6] = (v4>>8)&0xff;
  tx_data[7] =    (v4)&0xff;
  HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data,(uint32_t*)CAN_TX_MAILBOX0);
}


