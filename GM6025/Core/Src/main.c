/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "main.h"
#include "can.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Bsp_can.h"
#include "Chassis_Control.h"
#include "pid.h"
#include "Algorithms_Lib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RC_CH_VALUE_MIN 364
#define RC_CH_VALUE_OFFSET 1024
#define RC_CH_VALUE_MAX 1684
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
typedef struct
 {
 int ch0;
 int ch1;
 int ch2;
 int ch3;
 int ch4;
 uint8_t s1;
 uint8_t s2;
 uint8_t last_s1;
 uint8_t last_ch4;
 } rc;

uint8_t buffer[36];
rc RC_ctrl;
int back;
uint8_t state;
uint8_t error;

extern Motor_6025_Typedef motor_array[6]; //前四位为底盘，后两位为云�?
extern value_set_chassis[4];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
can_user_init(&hcan1);
HAL_TIM_Base_Start_IT(&htim2);
HAL_UARTEx_ReceiveToIdle_IT(&huart3 , buffer, sizeof(buffer));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
   if(huart == &huart3)
//  if(__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE))
  {
//    HAL_UART_AbortReceive(huart);
    //HAL_UARTEx_ReceiveToIdle_IT(&huart3,buffer,18);
//     HAL_UART_Receive_DMA(&huart1,buffer,18);
	  HAL_UARTEx_ReceiveToIdle_IT(&huart3,buffer,18);
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
//    HAL_UARTEx_ReceiveToIdle_DMA(&huart1,buffer,36);
  }
}


void RC_Chassis_DataUpdate(int16_t *chassis_set_array)
{
	static LOW_PASS_FILTER_Typedef motor_chassis_filter[4];
	static uint8_t i = 1;
	const static float RATE = 1;
	if(i == 1)
	{
		const float K_LPF_CHASSIS = 0.12;
		const float T_LPF_CHASSIS = 0.002;
		for(i = 0; i < 4; i++)
		{
			low_pass_filter_init(&motor_chassis_filter[i], K_LPF_CHASSIS, T_LPF_CHASSIS);
		}
	}
	chassis_set_array[0] = -RATE * low_pass_filter(&motor_chassis_filter[0], RC_ctrl.ch1 - RC_ctrl.ch0 - RC_ctrl.ch2);
	chassis_set_array[1] =  RATE * low_pass_filter(&motor_chassis_filter[1], RC_ctrl.ch1 + RC_ctrl.ch0 + RC_ctrl.ch2);
	chassis_set_array[2] =  RATE * low_pass_filter(&motor_chassis_filter[2], RC_ctrl.ch1 - RC_ctrl.ch0 + RC_ctrl.ch2);
	chassis_set_array[3] = -RATE * low_pass_filter(&motor_chassis_filter[3], RC_ctrl.ch1 + RC_ctrl.ch0 - RC_ctrl.ch2);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	static uint8_t flag = 0;
	if (htim->Instance == TIM2)
	{
		if (flag == 0)
		{
			Chassis_Control(motor_array);
			set_motor_value(0x200, value_set_chassis[0], value_set_chassis[1], value_set_chassis[2], value_set_chassis[3]);

		}

		flag ^= 1;
	}
}

















/****************************************************************************/
//#define DATA_LENGTH_CHASSIS ((uint8_t) 20)
//int16_t speed_set_chassis[4];
//static float speed_filtered_chassis[4] = {0};
//int16_t value[4];
//static uint8_t init_flag = 1;
//		uint8_t i;
//		static PID_Typedef pid_speed_motor_chassis[4];
//		static float relative_angle_chassis[4] ={0, 0, 0 ,0};
//		static float in_data[4][DATA_LENGTH_CHASSIS] = {0};
//		float angle_increment_chassis[4] = {0};
//		float speed_chassis[4] = {0};
//		static int16_t value_set_chassis[4] ={ 0 };
//
//void Chassis_Control(Motor_6025_Typedef *motor_chassis_array)
//{
////	static uint8_t init_flag = 1;
////		uint8_t i;
////		static PID_Typedef pid_speed_motor_chassis[4];
////		static float relative_angle_chassis[4] ={0, 0, 0 ,0};
////		static float in_data[4][DATA_LENGTH_CHASSIS] = {0};
////		float angle_increment_chassis[4] = {0};
////		float speed_chassis[4] = {0};
////		static int16_t value_set_chassis[4] ={ 0 };
//
//		if(init_flag == 1)
//		{
//			Pid_Init_Speed_Chassis(&pid_speed_motor_chassis[0]);
//			for(i = 0; i < 4; i++)
//			{
//					motor_chassis_array[i].last_angle = motor_chassis_array[i].angle;
//			}
//			init_flag = 0;
//		}
//		RC_Chassis_DataUpdate(speed_set_chassis);
//		for(i = 0; i < 4; i++)
//		{
//			angle_increment_chassis[i] = Angle_Calculate(motor_chassis_array[i].angle / 8191.0 * 360.0 ,
//													motor_chassis_array[i].last_angle / 8191.0 * 360.0);
//			relative_angle_chassis[i] += angle_increment_chassis[i];
//			speed_chassis[i] = angle_increment_chassis[i] * 60000.0 / 360.0;
//			speed_filtered_chassis[i] = Average_Filter(&in_data[i][0], speed_chassis[i], DATA_LENGTH_CHASSIS);
//
//			value_set_chassis[i] = Pid_Calculate(&pid_speed_motor_chassis[i], speed_filtered_chassis[i], speed_set_chassis[i]);
//			}
//			for(i = 0; i < 4; i++)
//			{
//
//			motor_chassis_array[i].last_angle = motor_chassis_array[i].angle;
//		}
//		for(int i=0;i<4;i++)
//		{
//			value[i] = value_set_chassis[i];
//
//		}
//
//}
//
//
//void Pid_Init_Speed_Chassis(PID_Typedef *pid)
//{
//	for(uint8_t i = 0; i < 4; i++)
//	{
//		pid[i].param.Kd = 0.04;
//		pid[i].param.Ki = 0.09;
//		pid[i].param.Kp = 10.00;
//
//		pid[i].limit.max_err_input = 3300;
//		pid[i].limit.max_i_out = 4000;
//		pid[i].limit.max_total_out = 5000;
//	}
//}
//

/**************************************************************************************************/

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
