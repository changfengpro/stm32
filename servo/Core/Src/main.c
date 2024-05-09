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
#include "i2c.h"
#include "tim.h"
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
  int s1,s2,s3,s4;
int distance_2,distance_1;
int keynum;
int f1=0,f2=0,f3=0,f4=0;
uint8_t i=0;

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
int k= 0,j=0;
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
  MX_TIM2_Init();
  MX_TIM1_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);//开启定时器2通道1PWM波
HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);//开启定时器2通道2PWM波
HAL_TIM_Base_Start_IT(&htim1);//开启定时器1基本中断
htim1.Instance -> CNT = 0;//将CNT的值置零
OLED_Init();//OLED初始化


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  HCsr04_1();
	  HCsr04_2();

//	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
//	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
//	  HAL_Delay(1);
//	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
//	  while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_RESET)
//		  s1 = htim1.Instance -> CNT;
//	  while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_SET)
//		  s2 = htim1.Instance -> CNT;
//	  distance = (s2 -s1) * 0.034/2;
//	  distance_1 = distance;
//	  HAL_Delay(500);
//	  OLED_ShowString(1, 1, "distance:");
//	  OLED_ShowNum(2, 1, distance_1, 3);
//	  OLED_ShowNum(3, 1, distance_2, 3);
//	  OLED_ShowNum(4, 1, htim1.Instance -> CNT, 3);
	  if(distance_1 <= 40 && distance_2 <= 40)
	  {
		  if(distance_2 > 10)
		  {
			  switch(keynum)
			  {
			  case 1:
				  	  if(f1 == 0)
				  	  {
				  	  Angle_Control(90);
				  	  HAL_Delay(1000);
				  	  Angle_Control(0);
				  	  HAL_Delay(500);
				  	  f1=1;
				  	  }


				  	  break;
			  case 2:
				  	  if(f2 == 0)
				  	  {
				  	  Angle_Control_2(90);
				  	  HAL_Delay(500);
				  	  Angle_Control(90);
				  	  HAL_Delay(500);
				  	  Angle_Control(0);
				  	  HAL_Delay(500);
				  	  Angle_Control_2(0);
				  	  f2 = 1;
				  	  }
				  	  break;
			  case 3:
				  	  if(f3 == 0)
				  	  {
				  		Angle_Control_2(180);
				  		HAL_Delay(500);
				  		Angle_Control(90);
				  		HAL_Delay(500);
				  		Angle_Control(0);
				  		HAL_Delay(500);
				  		Angle_Control_2(0);
				  		HAL_Delay(500);
				  		f3 =1;
				  	  }
				  	  break;
			  case 4:
				  	  if(f4 == 0)
				  	  {
				  	  Angle_Control_2(270);
				  	  HAL_Delay(1000);
				  	  Angle_Control(90);
				  	  HAL_Delay(1000);
				  	  Angle_Control(0);
				  	  HAL_Delay(1000);
				  	  Angle_Control_2(0);
				  	  HAL_Delay(1000);
				  	  f4 = 1;
				  	  }
				  	  break;
			  case 5:
				      f1=0;
					  f2=0;
					  f3=0;
					  f4=0;

			  }

			  	  keynum = 5;

			  OLED_ShowString(1, 1, "rubbish not full");//显示字符串
			  j=1;
			  if(k==1)
			  {
				  OLED_Clear();//清屏
				  k=0;
			  }

			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);//将PB0置高电平
		  }
		  else
		  {
			  OLED_ShowString(2, 1, "rubbish full");
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
			  k=1;
			  if(j == 1)
			  {
				  OLED_Clear();
				  j=0;
			  }
		  }
	  }

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


/**
  * @brief  超声波1距离计算
  * @param  无
  * @retval 无
  */
void HCsr04_1()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);//将PA6置低电平
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);//将PA6置高电平
		  HAL_Delay(1);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);//将PA6置低电平
		  while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_RESET)//当PA7不为低电平时退出循环
			  s1 = htim1.Instance -> CNT;
		  while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_SET)//当PA7不为高电平时退出循环
			  s2 = htim1.Instance -> CNT;
		  distance_1 = (s2 -s1) * 0.034/2;//距离计算
		  HAL_Delay(500);
}

/**
  * @brief  超声波2距离计算
  * @param  无
  * @retval 无
  */
void HCsr04_2()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
			  HAL_Delay(1);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
			  while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_RESET)
				  s3 = htim1.Instance -> CNT;
			  while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_SET)
				  s4 = htim1.Instance -> CNT;
			  distance_2 = (s4 -s3) * 0.034/2;
			  HAL_Delay(500);
}

//void Servo_Control(uint16_t angle)
//{
//	float temp;
//	temp = angle / 9*10 +50;
//	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,temp);
//}

/**
  * @brief  舵机1角度控制
  * @param  angle 需要控制的角度
  * @retval 无
  */
void Angle_Control(int angle)
{
	if(angle == 0)  __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,1);
	if(angle == 180) __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,248);//�??????????????178°
	if(angle == 90)  __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,150);//�??????????????91.6°
	if(angle == 270)  __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,400);//�??????????????270°
}

/**
  * @brief  舵机2角度控制
  * @param  angle 需要控制的角度
  * @retval 无
  */
void Angle_Control_2(int angle)
{
	if(angle == 0)  __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,1);
	if(angle == 180) __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,248);//�??????????????178°
	if(angle == 90)  __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,150);//�??????????????91.6°
	if(angle == 270)  __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,400);//�??????????????270°
}


//void button_1(void)
//{
//    static int flag = 0;
//
//    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET)
//    {
//
//        if(flag == 0)
//        {
//            flag = 1;
//        }
//        else
//        {
//            flag = 0;
//        }
//    }
//
//
//    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET && flag == 1)
//    {
//        Angle_Control(0);
//    }
//}


//void delay_us(uint16_t us)
//{
//	__HAL_TIM_SET_COUNTER (&htim1 ,0);
//	while(__HAL_TIM_GET_COUNTER (&htim1 )<us );
//
//}

/**
  * @brief  外部中断回调函数
  * @param  GPIO_Pin gpio口
  * @retval 无
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		static int flag_1 = 0;
		static int flag_2 = 0;
		static int flag_3 = 0;
		static int flag_4 = 0;

	if(GPIO_Pin == GPIO_PIN_6)//判断是否是PIN6
	{


		    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_SET)//读取PB6的电平进行判断
		    {
		        if(flag_1 == 0)
		        {
		            flag_1 = 1;
		        }
		        else
		        {
		            flag_1 = 0;
		        }
		    }

		    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_RESET && flag_1 == 1)
		    {
//		        Angle_Control(0);
		        flag_1 = 0;
		        keynum=1;
		    }
	}

	if(GPIO_Pin == GPIO_PIN_7)
		{


			    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_SET)
			    {
			        if(flag_2 == 0)
			        {
			        	flag_2 = 1;
			        }
			        else
			        {
			        	flag_2= 0;
			        }
			    }

			    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_RESET && flag_2 == 1)
			    {
//			        Angle_Control(90);
			        flag_2 = 0;
			        keynum=2;
			    }
		}

	if(GPIO_Pin == GPIO_PIN_8)
		{
			static int flag_3 = 0;

			    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_SET)
			    {
			        if(flag_3 == 0)
			        {
			        	flag_3 = 1;
			        }
			        else
			        {
			        	flag_3 = 0;
			        }
			    }

			    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_RESET && flag_3 == 1)
			    {
//			        Angle_Control(180);
			        flag_3 = 0;
			        keynum=3;
			    }
		}
	if(GPIO_Pin == GPIO_PIN_9)
		{
			static int flag_4 = 0;

			    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_SET)
			    {
			        if(flag_4 == 0)
			        {
			        	flag_4 = 1;
			        }
			        else
			        {
			        	flag_4 = 0;
			        }
			    }

			    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_RESET && flag_4 == 1)
			    {
//			        Angle_Control(270);
			        flag_4 = 0;
			        keynum=4;
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
