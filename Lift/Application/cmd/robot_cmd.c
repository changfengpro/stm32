/*
 * robot_cmd.c
 *
 *  Created on: Aug 26, 2024
 *      Author: auroranebulas
 */


#include "robot_cmd.h"
#include "usart.h"
#include "memory.h"
#include  "remote_control.h"
#include "robot_def.h"


static Chassis_Ctrl_Cmd_s chassis_cmd_send;        //发送给底盘的应用消息 
static uint32_t count_2;






uint8_t buffer[36];
rc RC_ctrl;
// rc_union RC_data;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
   if(huart == &huart3)
  {
//    HAL_UART_AbortReceive(huart);
    //HAL_UARTEx_ReceiveToIdle_IT(&huart3,buffer,18);
//     HAL_UART_Receive_DMA(&huart1,buffer,18);

    RC_ctrl.ch0 = (buffer[0] | buffer[1] << 8) & 0x07FF;
    RC_ctrl.ch1 = (buffer[1] >> 3 |buffer[2] << 5) & 0x07FF;
    RC_ctrl.ch2 = (buffer[2] >> 6 | buffer[3] << 2 | buffer[4] << 10) & 0x07FF;
    RC_ctrl.ch3 = (buffer [4] >> 1 | buffer[5] << 7) & 0x07FF;
    RC_ctrl.ch4 = (buffer[16] | (buffer[17] << 8));
    RC_ctrl.s1  = (buffer[5] >> 4 & 0x000C) >> 2;
    RC_ctrl.s2  = (buffer[5] >> 4 & 0x0003);



    RC_ctrl.ch0 -= RC_CH_VALUE_OFFSET;
    RC_ctrl.ch1 -= RC_CH_VALUE_OFFSET;
    RC_ctrl.ch2 -= RC_CH_VALUE_OFFSET;
    RC_ctrl.ch3 -= RC_CH_VALUE_OFFSET;
    RC_ctrl.ch4 -= RC_CH_VALUE_OFFSET;

    HAL_UARTEx_ReceiveToIdle_IT(&huart3,buffer,36);
//    HAL_UARTEx_ReceiveToIdle_DMA(&huart1,buffer,36);
  }
}
