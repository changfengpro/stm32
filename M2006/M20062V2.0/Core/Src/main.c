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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Bsp_can.h"
#include "pid.h"
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
pid_struct_t motor_pid[7];
int target_speed[5];
int target_position[5];
int desired_speed[5];
int last_angle[5];
int overflowNum[5] = {0,0,0,0,0};
int angle_sum[5];
int err;
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
  MX_CAN1_Init();
  MX_TIM2_Init();
  MX_CAN2_Init();
  /* USER CODE BEGIN 2 */
can_dual_init(&hcan1, &hcan2);
ver_PID_Init();
pos_PID_Init();
HAL_TIM_Base_Start_IT(&htim2);
//target_position = (motor_info[3].rotor_angle *(360.0/8191.0))/36;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


//	  set_motor_current(0x200, 0, 0, 500, 0);
//	  HAL_Delay(1);

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
int flag = 1;
int flag_2 = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{	if(flag_2 == 0)
	{
//		motor_reset();
		flag_2 = 1;
	}

	if(htim->Instance == htim2.Instance)
	{
//		target_position[3] = 5000;
//		desired_speed[3]=80;
    motor_calc(3,&angle_sum[3],&target_position[3],&target_speed[3],&desired_speed[3]);

		if(flag == 2)
		{
//			target_position[3] = 2000;
//			desired_speed[3] = 80;
			motor_info[3].set_current = Double_Loop_Control(angle_sum[3], target_position[3],target_speed[3] , desired_speed[3], M2006, M2006);
			flag = 1;
		}


		set_motor_current(0x200, 0, 0, motor_info[3].set_current, motor_info[4].set_current);

		flag++;
		err = angle_sum -last_angle;

	}
}

void motor_calc(int ID, int **angle_sumEx, int **target_positionEx, int **target_speedEx, int **desired_speedEx)
{
		*target_speedEx = motor_info[ID].rotor_speed / 36 ;

		*angle_sumEx = angle_calc(ID,motor_info[ID].rotor_angle);


		/**********************************************************/
		//过零处理
		if((motor_info[ID].rotor_angle - last_angle[ID]) <  -4096)
		    {
		    	 overflowNum[ID]++;
		    }
		    if((motor_info[ID].rotor_angle - last_angle[ID]) >  4096)
		    {
		    	overflowNum[ID]--;
		    }

		    last_angle[ID] = motor_info[ID].rotor_angle;

		/**********************************************************/
}

int angle_calc(int ID,int CNT)
{
	float  angle;

	angle = (overflowNum[ID] *360 + CNT * (360.0/8191.0))/36.0;

	return angle;
}

void motor_reset() {
    // 设置期望速度为 {10, 10, 10, 10, 10}
    desired_speed[0] = 10;
    desired_speed[1] = 10;
    desired_speed[2] = 10;
    desired_speed[3] = 10;
    desired_speed[4] = 10;

    // 延时 500 毫秒
    delay_ms(500);

    // 设置期望速度为 {0, 0, 0, 0, 0}
    desired_speed[0] = 0;
    desired_speed[1] = 0;
    desired_speed[2] = 0;
    desired_speed[3] = 0;
    desired_speed[4] = 0;

    overflowNum[3] = 0;
}

void delay_ms(uint32_t milliseconds) {
    // 假设 CPU 频率为 180 MHz，每个循环大约需要 5.56 纳秒
    // 这个假设需要根据实际的硬件平台进行调整

    // 180 MHz 下的每个循环执行的指令数
    // (1 / 180 MHz) * 1000 = 5.56 纳秒
    // (1 ms / 5.56 纳秒) ≈ 180000 循环
    // 调整为适应不同的频率，这个值需要重新计算

    for (uint32_t i = 0; i < milliseconds; ++i) {
        for (volatile uint32_t j = 0; j < 180000; ++j) {
            // 空循环，消耗一些时间
        }
    }
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
