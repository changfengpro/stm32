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
#define LIMIT_MIN_MAX(x,min,max) (x) = (((x)<=(min))?(min):(((x)>=(max))?(max):(x)))
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
#include "bsp_can.h"
#include "pid.h"
#include "pid_2.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern moto_info_t motor_info[MOTOR_MAX_NUM];
int16_t led_cnt;
pid_struct_t motor_pid[7];
float target_speed;
float target_position;
float desired_speed;
float last_angle;
int overflowNum = 0;
float angle_sum;
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
  MX_CAN2_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
can_user_init(&hcan1);
can_user_init(&hcan2);
pid_init(&motor_pid[2], 40, 3, 0, 15000, 15000);
pos_PID_Init();
ver_PID_Init();
//HAL_TIM_Base_Start_IT(&htim2);
//htim2.Instance->CNT = 0;
HAL_TIM_Base_Start_IT(&htim2);
int temp;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {   LED_SET();
//  	  htim2.Instance->CNT = motor_info[2].rotor_angle;
  	  angle_sum = angle_calc(motor_info[2].rotor_angle);

//   LED_Control(50);
	  motor_info[2].set_voltage = Double_Loop_Control(angle_sum, target_position, motor_info[2].rotor_speed, desired_speed, 20000, 20000);
//	   motor_info[2].set_voltage = pid_calc(&motor_pid[2],target_speed, motor_info[2].rotor_speed);
//	  motor_info[0].set_voltage = Double_Loop_Control(motor_info[0].rotor_angle, target_position, motor_info[0].rotor_speed, desired_speed, 20000, 20000);
//	  set_motor_voltage(0, 0, 0, motor_info[2].set_voltage, 0);
	  set_motor_voltage(0, 0, 0, motor_info[2].set_voltage, 0);
//	  set_motor_voltage(0, 0, 0, 0, 0);
	  HAL_Delay(10);
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
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if(htim->Instance == htim2.Instance)
  {

    if((motor_info[2].rotor_angle - last_angle) <  -180)
    {
    	 overflowNum++;
    }
    if((motor_info[2].rotor_angle - last_angle) >  180)
    {
    	overflowNum--;
    }

    last_angle = motor_info[2].rotor_angle;
  }

}



int angle_calc( int CNT )
{
	float angle;

	angle = overflowNum * 360 + CNT * (360.0 / 8192.0);

	return angle;
}

void LED_1(void)
{
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,RESET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,SET);

}
void LED_2(void)
{

  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,RESET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,SET);

}
void LED_3(void)
{
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,RESET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,SET);
}
void LED_4(void)
{
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,RESET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,SET);

}
void LED_5(void)
{
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,RESET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,SET);

}
void LED_6(void)
{
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,RESET);

}

void LED_SET(void)
{
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,SET);
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,SET);

}

void LED_Control(int ms)
{
  LED_1();
  HAL_Delay(ms);
  LED_2();
  HAL_Delay(ms);
  LED_3();
  HAL_Delay(ms);
  LED_4();
  HAL_Delay(ms);
  LED_5();
  HAL_Delay(ms);
  LED_6();
  HAL_Delay(ms);
  LED_5();
  HAL_Delay(ms);
  LED_4();
  HAL_Delay(ms);
  LED_3();
  HAL_Delay(ms);
  LED_2();
  HAL_Delay(ms);
  LED_1();
  HAL_Delay(ms);
}
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
