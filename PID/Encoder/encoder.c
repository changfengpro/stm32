#include "encoder.h"
#include "tim.h"
#include "gpio.h"
#include "stdlib.h"
#include "PID.h"
#define SPEED_THRESHOLD 3

Motor motor1;
float motor1_speed;
int motor1_dir;
int PWM;
extern float b;



void Motor_Init(void)
{
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);      //开启编码器定时器
    __HAL_TIM_ENABLE_IT(&htim2,TIM_IT_UPDATE);           //开启编码器定时器更新中断,防溢出处理
    HAL_TIM_Base_Start_IT(&htim3);                       //开启100ms定时器中断
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);            //开启PWM
    __HAL_TIM_SET_COUNTER(&htim2, 10000);                //编码器定时器初始值设定为10000
    motor1.lastCount = 0;                                   //结构体内容初始化
    motor1.totalCount = 0;
    motor1.overflowNum = 0;
    motor1.lastoverflowNum=0;                                  
    motor1.speed = 0;
    motor1.direct = 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//定时器回调函数，用于计算速度
{   
    if(htim->Instance==&htim2.Instance)//编码器输入定时器溢出中断，用于防溢出                   
    {      
        if(COUNTERNUM < 10000) motor1.overflowNum++;       //如果是向上溢出
        else if(COUNTERNUM >= 10000) motor1.overflowNum--; //如果是向下溢出
        __HAL_TIM_SetCounter(&htim2, 10000);             //重新设定初始值
        if (motor1.overflowNum != motor1.lastoverflowNum) {
            motor1.totalCount = COUNTERNUM + motor1.overflowNum * RELOADVALUE; // Update total count value
            motor1.lastoverflowNum = motor1.overflowNum; // Update last overflow count value
    }
    }
     if(htim->Instance==htim3.Instance)
    {   
        
            motor1.direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2);//如果向上计数（正转），返回值为0，否则返回值为1
        motor1.totalCount = COUNTERNUM + motor1.overflowNum * RELOADVALUE;//一个周期内的总计数值等于目前计数值加上溢出的计数值
        if((fabs(motor1.totalCount - motor1.lastCount))>2000)
        {   float speed_change = (float)(motor1.totalCount - motor1.lastCount+20000) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10;
        // 检查速度变化率是否超过阈值
        if (fabs(speed_change) > SPEED_THRESHOLD) {
            speed_change = speed_change > 0 ? SPEED_THRESHOLD : -SPEED_THRESHOLD;
        }
        motor1.speed = speed_change;//算得每秒多少转
            //motor1.speed = (float)(motor1.totalCount - motor1.totalCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10 * LINE_SPEED_C//算得车轮线速度每秒多少毫米
        }
        else{
            float speed_change = (float)(motor1.totalCount - motor1.lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10;
        // 检查速度变化率是否超过阈值
        if (fabs(speed_change) > SPEED_THRESHOLD) {
            speed_change = speed_change > 0 ? SPEED_THRESHOLD : -SPEED_THRESHOLD;
        }
        motor1.speed = speed_change;//算得每秒多少转
            //motor1.speed = (float)(motor1.totalCount - motor1.totalCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10 * LINE_SPEED_C//算得车轮线速度每秒多少毫米
        }
        motor1.lastCount = motor1.totalCount; //记录这一次的计数值
            
        motor1_speed=fabs(motor1.speed);
        motor1_dir=abs(motor1.direct);
        PWM = velocity_PID_vaule(b)+vertical_PID_value(motor1.speed,b);
    }
}
void SETPWM(int pwm)
{
    if(pwm > 0) __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pwm);
    else __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,(-pwm));
}
