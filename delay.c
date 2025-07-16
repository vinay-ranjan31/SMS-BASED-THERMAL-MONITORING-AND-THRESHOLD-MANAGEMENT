                       /* delay.c */
#include "types.h"
void delay_us(u32 i)
{
	i*=12;
	while(i--);
}

void delay_ms(u32 i)
{
	i*=12000;
	while(i--);
}

void delay_s(u32 i)
{
	i*=12000000;
	while(i--);
}
