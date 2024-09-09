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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "encoder.h"
#include "PID.h"
#define GPIO_PIN_SET 1
#define GPIO_PIN_RESET 0 
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 int a=1;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t adc_vaule[3];
extern float motor1_speed;
extern int motor1_dir;
uint8_t dir_flag;
uint8_t flag_1;
uint8_t flag_2;
float kp=2100,ki=3.5,kd=0.1;
float vkp=0,vki=0;
extern int PWM;
float b;
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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  Motor_Init();
  ver_PID_Init();
  pos_PID_Init();
 b=3000;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  { 

    SET_PWM(PWM);



  //   HAL_ADCEx_Calibration_Start(&hadc1 );
  //   HAL_ADC_Start_DMA(&hadc1,adc_vaule,3);
   
   
  // if(a==1)
  // {
  //       flag_1=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1);
  //       flag_2=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2);
        
  //   if(adc_vaule[0]>0 && adc_vaule[0]<10 && motor1_dir==1 && flag_1==1 && flag_2==0)
  //   {
  //     HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_1);
  //     HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_2);
  //     OLED_ShowFloatNum(1,7,motor1_speed,1,2);
  //         OLED_ShowNum(2,12,motor1_dir,1);
  //   }
  //   else if(adc_vaule[0]>0 && adc_vaule[0]<10 && motor1_dir==0 && flag_1==0 && flag_2==1)
  //   {
  //     HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_1);
  //     HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_2);
  //     // OLED_ShowFloatNum(1,7,motor1_speed,1,2);
  //     //     OLED_ShowNum(2,12,motor1_dir,1);
  //   }
  
  //         __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,adc_vaule[2]);
          
  //     // OLED_ShowFloatNum(1,7,motor1_speed,1,2);
  //     //     OLED_ShowNum(2,12,motor1_dir,1);


  // }



    // if(a==1)
    // { OLED_ShowFloatNum(1,7,motor1_speed,1,2);
    //   if(adc_vaule[2]>0 && adc_vaule[2]<5) 
    //    {
    //      __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,3500);//全速
    //      OLED_ShowFloatNum(1,7,motor1_speed,1,2);
    //      OLED_ShowNum(2,12,motor1_dir,1);
    //    }
    //   else {
    //       __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,adc_vaule[0]);
    //       OLED_ShowFloatNum(1,7,motor1_speed,1,2);
    //       OLED_ShowNum(2,12,motor1_dir,1);
    //   }
    // }
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
