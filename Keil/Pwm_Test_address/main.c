

typedef unsigned int uint32_t;
typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;	


void SystemClock_Config()
{	
	//启动HSE
	*(uint32_t *)0x40021000 |= 0x01 << 16;
	
	//等待HSE稳定就绪
	while((*(uint32_t *)0x40021000 & 0x02) == 0)
	{
	}
	
	//关闭PLL
	*(uint32_t *)0x40021000 &= ~(0x01 << 24);
	while(((*(uint32_t *)0x40021000 & 0x01 << 25)) == 1 )
	{
	}
	
	//选择HSE作为PLL的时钟输入源

	*(uint32_t *)0x40021004 &= ~((0x01 << 18) | (0x01 << 19) | (0x01 << 20) | (0x01 << 21));		//清除PLL倍频因子
	*(uint32_t *)0x40021004 |= 	((0x01 << 18) | (0x01 << 19) | (0x01 << 20)); 									//PLL 9倍频输出
	
	//使能PLL
	*(uint32_t *)0x40021000 |= 	(0x01 << 24);
	
	//等待PLL稳定就绪
	while((*(uint32_t *)0x40021000 & 0x01 << 25) == 0 )
	{
	}
		
	//切换系统时钟源到PLL
	*(uint32_t *)0x40021004 &= ~(0x03); 		//清除SW位
	*(uint32_t *)0x40021004 |= (0x02);			//切换系统时钟源为PLL
	
	//等待PLL作为系统时钟源生效
	while(((*(uint32_t *)0x40021004 & (0x01 << 2)) == 0) && ((*(uint32_t *)0x40021004 & (0x01 << 3)) == 1))
	{
	}
	
	//设置AHB不分频
	*(uint32_t *)0x40021004 &= ~(0x01 << 7); 
	
	//设置APB1 2分频
	*(uint32_t *)0x40021004 &= (~((0x01) << 8) | ~((0x01) << 9)); 
	*(uint32_t *)0x40021004 |= (0x01 << 10); 
	
	//设置APB2 不分频
	*(uint32_t *)0x40021004 &= ~(0x01 << 13);
}

int main()
{
	SystemClock_Config();
	
}