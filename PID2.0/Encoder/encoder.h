#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "stm32f1xx.h"
#include "tim.h"
extern float motor1_speed;


#define MOTOR_SPEED_RERATIO 20u    //电机减速比
#define PULSE_PRE_ROUND 12 //一圈多少个脉冲
#define RADIUS_OF_TYRE 34 //轮胎半径，单位毫米
#define LINE_SPEED_C RADIUS_OF_TYRE * 2 * 3.14
#define RELOADVALUE __HAL_TIM_GetAutoreload(&htim2)    //获取自动装载值,本例中为20000
#define COUNTERNUM __HAL_TIM_GetCounter(&htim2)        //获取编码器定时器中的计数值

typedef struct _Motor
{
    int32_t lastCount;   //上一次计数值
    int32_t totalCount;  //总计数值
    int16_t overflowNum; //溢出次数
    int16_t lastoverflowNum;
    float speed;         //电机转速
    uint8_t direct;      //旋转方向
}Motor;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);//定时器回调函数，用于计算速度
void Motor_Init(void);
void SET_PWM(int pwm);
float calculate_filtered_speed(float raw_speed);
int motor_dir(int flag);
void Read_angle();
void calcu_angle();
void calcu_add_count();


#endif