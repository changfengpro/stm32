/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB6 PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

void MOTOR_CONTROL(uint8_t direction)
{
	static uint8_t step=0;
	if(direction == 1)//反转
	{
		if(step == 0)step=8;
		step--;
	}

	if(step == 0)//步序1
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, SET);//1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);//0
	}
	if(step == 1)//步序2
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, SET);//1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, SET);//1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);//0
	}
	if(step == 2)//步序3
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, SET);//1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);//0
	}
	if(step == 3)//步序4
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, SET);//1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);//1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);//0

	}
	if(step == 4)//步序5
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);//1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);//0
	}
	if(step == 5)//步序6
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);//1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);//1
	}
	if(step == 6)//步序7
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);//1
	}
	if(step == 7)//步序8
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, SET);//1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);//1
	}
	if(direction == 0)//正转
	{
		step++;if(step == 8)step=0;
	}
}

void Step_Motor_Start(uint16_t angle, uint8_t direction)
{
	int i = 0;
	int pulse = (int)((double)(angle/5.625)*64);
	for(i=0; i<pulse; i++){
		MOTOR_CONTROL(direction);
		HAL_Delay(2);//延时2ms,修改延时可改变电机转动速度。即修改了脉冲频率
	}
}

void Step_Motor_Stop(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);

}

/* USER CODE END 2 */
