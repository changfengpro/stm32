


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


int main()
{

	*(unsigned int *)0x40021018 |= 0x01 << 4;
	*(unsigned int *)0x40011004 |= 0x01 << 20;

	while(1)
	{
		
		Breath_Led();

	}
	
}