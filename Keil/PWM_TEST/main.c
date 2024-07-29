#include "stm32f10x.h"


void Delay_ms(volatile unsigned int t)
{
	for(unsigned int i = 0; i < t; i++)
		for(unsigned int i = 0; i < 800; i++);
}

#define LED_ON 		*(unsigned int *)0x40011010 |= 0x01 << 29
#define LED_OFF  	*(unsigned int *)0x40011010 |= 0x01 << 13; 

void Breath_Led()
{
	int Num = 125;
	
	for(int i = 0; i < Num; i++)
	{
		LED_OFF;
		Delay_ms(i);
		LED_ON;
		Delay_ms(Num - i);
	}
	
	for(int i = 0; i < Num; i++)
	{
		LED_ON;
		Delay_ms(i);
		LED_OFF;
		Delay_ms(Num - i);
		
	}
	
}

void SystemClock_Config()
{
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));
	
	RCC->CFGR &= ~RCC_CFGR_PLLSRC;
	RCC->CFGR |=  RCC_CFGR_PLLSRC_HSE;
	
	RCC->CFGR &= ~RCC_CFGR_PLLMULL; //清除PLL倍频因子的位
	RCC->CFGR |= RCC_CFGR_PLLMULL9; //配置PLL倍频因子为9，得到72MHz的系统时钟
	
	//启动PLL
	RCC->CR |= RCC_CR_PLLON;
	while(!RCC->CR & RCC_CR_PLLRDY);
	
	//选择PLL作为系统时钟源
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
	
	//设置AHB时钟分频为1
	RCC->CFGR &= ~RCC_CFGR_HPRE;
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	
	//设置APB1总线预分频为2
	RCC->CFGR &= ~RCC_CFGR_PPRE2;
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
	
	//设置APB2总线预分频为1
	RCC->CFGR &= ~RCC_CFGR_PPRE1;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;
	
}

void TIM1_MspPostInit()
{	
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	//配置PA8为复用推挽输出模式
	GPIOA->CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_MODE8);
	GPIOA->CRH |= GPIO_CRH_CNF8_1 | GPIO_CRH_MODE8_1 | GPIO_CRH_MODE8_0;
					
}

void TIM1_Init()
{
	TIM1->CR1 &= ~TIM_CR1_CEN;
	TIM1->PSC = 7199;
	TIM1->ARR = 199;
	
	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M;
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; //PWM模式1
	
	TIM1->CCR1 = 100; 	//设置输出比较值
	
	TIM1->CCER |= TIM_CCER_CC1E;  			//使能PWM的产生
	TIM1->CCER &= ~TIM_CCER_CC1NP; 			//高电平有效
	
	TIM1->BDTR = 0;
	
	TIM1->SMCR &= ~TIM_SMCR_SMS; //启用内部时钟
	
	TIM1->CR1 |= TIM_CR1_CEN; //使能定时器
	
	TIM1_MspPostInit();
}




int main()
{		
		SystemClock_Config();
		TIM1_Init();

	
	
	*(unsigned int *)0x40021018 |= 0x01 << 4;
	*(unsigned int *)0x40011004 |= 0x01 << 20;
		
	
	while(1)
	{
		TIM1->CCR1 = 200;
		
		Breath_Led();

	}
	
}