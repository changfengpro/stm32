/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
uint8_t TxData[10]="hongying";
uint8_t RxData[15];
uint32_t angle; uint8_t direction; float speed;
uint32_t global_i = 1;
uint32_t pulse;
uint32_t read_angle;
float angular_velocity;
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UARTEx_ReceiveToIdle_IT(&huart1, RxData, 15);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

while(1)
{
	read_angle= (global_i/64)*5.625;
			    HAL_Delay(2000);
			    if(read_angle != angle)
			    {Step_Motor_Start(angle, direction, speed);
			    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);}
			    if(read_angle == angle)
			    		{
			    	Step_Motor_Stop();
			    		}


}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void ms_Delay(uint16_t t_ms)
{
	uint32_t t=t_ms*3127;
	while(t--);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	  		if(huart == &huart1)
	  		{
	  			HAL_UARTEx_ReceiveToIdle_IT(&huart1, RxData, 15);
	  			sscanf(RxData,"%d %d %f",&angle,&direction,&angular_velocity);
	  			speed = 125/(1152*angular_velocity);

	  		}
}


void MOTOR_CONTROL(uint8_t direction,float speed)
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
		HAL_Delay(speed);
	}
	if(step == 1)//步序2
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, SET);//1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, SET);//1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);//0
		HAL_Delay(speed);
	}
	if(step == 2)//步序3
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, SET);//1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);//0
		HAL_Delay(speed);
	}
	if(step == 3)//步序4
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, SET);//1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);//1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);//0
		HAL_Delay(speed);

	}
	if(step == 4)//步序5
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);//1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);//0
		HAL_Delay(speed);
	}
	if(step == 5)//步序6
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);//1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);//1
		HAL_Delay(speed);
	}
	if(step == 6)//步序7
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);//1
		HAL_Delay(speed);
	}
	if(step == 7)//步序8
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, SET);//1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);//0
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);//1
		HAL_Delay(speed);
	}
	if(direction == 0)//正转
	{
		step++;if(step == 8)step=0;
	}
}

void Step_Motor_Start(uint16_t angle, uint8_t direction,float speed)
{
	 pulse = (uint32_t)((double)((uint32_t)angle/5.625)*64);
	for(global_i=0; global_i<pulse; global_i++){
		MOTOR_CONTROL(direction,speed);
		HAL_Delay(speed);//延时2ms,修改延时可改变电机转动�?�度。即修改了脉冲频�?
	}
}

void Step_Motor_Stop(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);

}
void Read_angle(uint8_t read_angle)
{
	read_angle= (global_i/64)*5.625;
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
