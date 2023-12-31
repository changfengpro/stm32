
#include "motor.h"
#include "tim.h"

void Set_Pwma(int motor_left)//赋值给PWM寄存器
{
	int16_t pwma_abs;
	if(motor_left>0) Motor_Left(1);
	else      Motor_Left(0);
	pwma_abs=myabs(motor_left);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pwma_abs);
}


void Motor_Left()
{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);//即01，反转
}
/*
*	函数功能：取绝对值
*	入口参数：int
*	返回值：无 unsingned int
*/
int myabs(int num)
{
	int temp;
	if(num<0)	temp=-num;
	else temp =num;
	return temp;
}
