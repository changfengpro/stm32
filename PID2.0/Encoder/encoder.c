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
float current_angle = 0;
int count = 0;
long int add_count = 0;
float tar_angle;
float diff_angle;
int diff_count;
int overflowNum = 1;
float test_angle = 720,test_speed = 2;


void Motor_Init(void)
{   
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);      //开启编码器定时器
    __HAL_TIM_ENABLE_IT(&htim2,TIM_IT_UPDATE);           //开启编码器定时器更新中断,防溢出处理
    HAL_TIM_Base_Start_IT(&htim3);                       //开启100ms定时器中断
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);            //开启PWM
    __HAL_TIM_SET_COUNTER(&htim2, 0);                //编码器定时器初始值设定为10000
    motor1.lastCount = 0;                                   //结构体内容初始化
    motor1.totalCount = 0;
    motor1.overflowNum = 0;
    motor1.lastoverflowNum=0;                                  
    motor1.speed = 0;
    motor1.direct = 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//定时器回调函数，用于计算速度
{   
    // if(HAL_GetTick > 0 && HAL_GetTick < 5000) overflowNum = 0;
    
    if(htim->Instance==htim2.Instance)//编码器输入定时器溢出中断，用于防溢出                   
    {      
        // if(COUNTERNUM < 10000) motor1.overflowNum++;       //如果是向上溢出
        // else if(COUNTERNUM >= 10000) motor1.overflowNum--; //如果是向下溢出
        // __HAL_TIM_SetCounter(&htim2, 10000);             //重新设定初始值
        // if (motor1.overflowNum != motor1.lastoverflowNum) {
        //     motor1.totalCount = COUNTERNUM + motor1.overflowNum * RELOADVALUE; // Update total count value
        //     motor1.lastoverflowNum = motor1.overflowNum; // Update last overflow count value
    // }
        if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1) == GPIO_PIN_RESET && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2) == GPIO_PIN_SET)
        {
            overflowNum--;
        }
        if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1) == GPIO_PIN_SET && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2) == GPIO_PIN_RESET)
        {
            overflowNum++;
        } 
       

    }

     if(htim->Instance==htim3.Instance)
    {   calcu_add_count();
        Read_angle();
        count = __HAL_TIM_GET_COUNTER(&htim2);
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
        PWM = Double_Loop_Control(current_angle,test_angle,motor1_speed,test_speed);
       
    }
}
void SET_PWM(int pwm)
{
    if(pwm > 0)
    {   motor_dir(0);
         __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pwm);
    }
    else 
    {
        motor_dir(1);
        __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,(-pwm));
    }
}

int motor_dir(int flag)
{
    if(flag == 0)
    {
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,1);
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,0);
    }
    if(flag == 1)
    {
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,1);
    }
    return flag;
}

void Read_angle()
{  
    current_angle = (add_count / 4) * 1.5 ;

}

void calcu_angle()
{
    diff_angle = current_angle - tar_angle;
    diff_count = (diff_angle / (360 / (MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND))) *4;
}

void calcu_add_count()
{
    
    add_count = COUNTERNUM + overflowNum * RELOADVALUE;
    
}

// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//定时器回调函数，用于计算速度
// {
//     current_time_stamp = HAL_GetTick();
//     uint32_t time_diff = current_time_stamp - last_time_stamp;
//     motor1_speed = (float)(encoder_pulse_count * )
// }

// int Read_Encoder()
// {
//     int count1 = __HAL_TIM_GET_COUNTER(&tim2);
//     int add_count += count1;

// }