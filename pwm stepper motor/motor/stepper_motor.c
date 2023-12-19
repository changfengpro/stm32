#include "gpio.h"
#include "main.h"
#include "tim.h"
#include "stepper_motor.h"
float angle;
uint8_t direction;
float RPM;
void stepperMotor_Init(void)
{
    HAL_TIM_Base_Start(&htim2);
}


void delay_us(uint16_t us)
{
    __HAL_TIM_SetCounter(&htim2, 0);
    while (__HAL_TIM_GetCounter(&htim2) < us)
        ;
}

void set_RPM_4096(float RPM)
{
    delay_us(1000000 / 4096 / RPM);
}

void stepperMotor_halfDrive(uint8_t step)
{
	 switch (step)
	 {case 0:
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, SET);//1
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET);//0
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);//0
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);//0
		 	break;
	 case 1:
		    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, SET);//1
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, SET);//1
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);//0
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);//0
		 	break;
	 case 2:
		    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);//0
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, SET);//1
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);//0
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);//0
		 	break;
	 case 3:
		    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);//0
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, SET);//1
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);//1
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);//0
		   break;
	 case 4:
		    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);//0
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET);//0
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);//1
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);//0
	 		   break;
	 case 5:
		    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);//0
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET);//0
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);//1
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);//1
	 		   break;
	 case 6:
		    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);//0
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET);//0
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);//0
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);//1
	 		   break;
	 case 7:
		    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, SET);//1
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET);//0
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);//0
		 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);//1
	 		   break;

	 }
}


void halfDrive_angle_diretion_RPM(float angle, uint8_t direction, float RPM)
{
    float anglepersequence = 0.703125; // 360 = 512 sequences
    uint16_t numberofsequences = (angle/anglepersequence);
    for (uint8_t seq = 0; seq < numberofsequences; seq++)
    {
        if (direction == 0) // for clockwise
        {
            for (uint8_t step = 7; step > 0; step--)
            {
                stepperMotor_halfDrive(step);
                set_RPM_4096(RPM);
            }
        }

        else if (direction == 1) // for anti-clockwise
        {
            for (uint8_t step = 0; step < 7; step++)
            {
                stepperMotor_halfDrive(step);
                set_RPM_4096(RPM);
            }
        }
    }
}

