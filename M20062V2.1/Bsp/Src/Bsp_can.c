/*
 * Bsp_can.c
 *
 *  Created on: May 31, 2024
 *      Author: auroranebulas
 */

#include "Bsp_can.h"


moto_info_t motor_info[MOTOR_MAX_NUM];
uint16_t can_cnt;


/**
  * @brief  初始化CAN过滤器，启动CAN并启用CAN RX FIFO 0消息挂起中断
  * @param  hcan1 指向第一个CAN_HandleTypeDef结构体的指针
  * @param  hcan2 指向第二个CAN_HandleTypeDef结构体的指针
  * @retval None
  */
void can_dual_init(CAN_HandleTypeDef* hcan1, CAN_HandleTypeDef* hcan2)
{
  CAN_FilterTypeDef  can_filter;

  // 配置CAN1过滤器
  can_filter.FilterBank = 0;                       // 过滤器0
  can_filter.FilterMode =  CAN_FILTERMODE_IDMASK;  // 掩码模式
  can_filter.FilterScale = CAN_FILTERSCALE_32BIT;  // 32位宽度
  can_filter.FilterIdHigh = 0;
  can_filter.FilterIdLow  = 0;
  can_filter.FilterMaskIdHigh = 0;
  can_filter.FilterMaskIdLow  = 0;                // 接收所有CAN ID的消息
  can_filter.FilterFIFOAssignment = CAN_RX_FIFO0; // 分配到FIFO0
  can_filter.FilterActivation = ENABLE;           // 启用过滤器
  can_filter.SlaveStartFilterBank  = 14;         // 在双CAN模式下，仅在CAN2上配置过滤器时才有意义

  // 初始化CAN1过滤器
  HAL_CAN_ConfigFilter(hcan1, &can_filter);

  // 启动CAN1
  HAL_CAN_Start(hcan1);

  // 启用CAN1接收FIFO0消息挂起中断
  HAL_CAN_ActivateNotification(hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

  // 配置CAN2过滤器
  can_filter.FilterBank = 15;                      // 过滤器15
  // 其他配置与CAN1相同

  // 初始化CAN2过滤器
  HAL_CAN_ConfigFilter(hcan2, &can_filter);

  // 启动CAN2
  HAL_CAN_Start(hcan2);

  // 启用CAN2接收FIFO0消息挂起中断
  HAL_CAN_ActivateNotification(hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
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
  if ((rx_header.StdId >= FEEDBACK_ID_BASE)
     && (rx_header.StdId <  FEEDBACK_ID_BASE + MOTOR_MAX_NUM))                  // judge the can id
    {
      can_cnt ++;
      uint8_t index = rx_header.StdId - FEEDBACK_ID_BASE;                  // get motor index by can_id
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
void set_motor_vaule(uint16_t tx_ID, int16_t v1, int16_t v2, int16_t v3, int16_t v4)
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
