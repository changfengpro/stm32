/*
 * Bsp_can.c
 *
 *  Created on: Jun 14, 2024
 *      Author: auroranebulas
 */
#include "Bsp_can.h"


Motor_6025_Typedef motor_array[6] = { 0 }; //前四位为底盘，后两位为云台



void can_user_init(CAN_HandleTypeDef* hcan )
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

  HAL_CAN_ConfigFilter(hcan, &can_filter);        // init can filter
  HAL_CAN_Start(&hcan1);                          // start can1
  HAL_CAN_Start(hcan);
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING); // enable can1 rx interrupt
}



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  CAN_RxHeaderTypeDef rx_header;
  uint8_t             rx_data[8];
  if(hcan->Instance == CAN1)
  {
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data); //receive can data
  }
  if ((rx_header.StdId >= MOTOR_3508_FEEDBACK_ID_BASE)
   && (rx_header.StdId <  MOTOR_3508_FEEDBACK_ID_BASE + MOTOR_CHASSIS_MAX_NUM + MOTOR_GIMBAL_MAX_NUM))                  // judge the can id
  {

    uint8_t index = rx_header.StdId - MOTOR_3508_FEEDBACK_ID_BASE  ;                  // get motor index by can_id
    motor_array[index].angle    = ((rx_data[0] << 8) | rx_data[1]);
    motor_array[index].current_get    = ((rx_data[2] << 8) | rx_data[3]);
    motor_array[index].current_set = ((rx_data[4] << 8) | rx_data[5]);
    motor_array[index].hall           =   rx_data[6];
  }

}

void set_motor_value(uint16_t tx_ID, int16_t v1, int16_t v2, int16_t v3, int16_t v4)
{
  CAN_TxHeaderTypeDef tx_header;
  uint8_t             tx_data[8];

  tx_header.StdId = tx_ID;
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



