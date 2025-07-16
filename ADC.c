#include <LPC214x.h>

#include "types.h"
#include "adc_defines.h"
#include "delay.h"

#define ADC_FUNC 0x01 

void Init_ADC(void)
{
	PINSEL1 |= (ADC_FUNC<<24);  //configure P0.28 as ADC input
  AD0CR=PDN_BIT|CLKDIV|CHANNEL_SEL;	
}

f32 Read_ADC(u8 chNo)
{
  u16 adcVal=0;
	f32 eAR;
	//WRITEBYTE(AD0CR,0,chNo);
	AD0CR&=0XFFFFFF00;
	AD0CR|=1<<chNo;
	AD0CR|=1<<ADC_START_BIT;
	//SETBIT(AD0CR,ADC_START_BIT);
	delay_us(3);
	while(((AD0GDR>>DONE_BIT)&1)==0);
	//CLRBIT(AD0CR,ADC_START_BIT);
	AD0CR&=~(1<<ADC_START_BIT);
	adcVal=(AD0GDR>>6)&0x3FF;
	eAR=((adcVal*3.3)/1023);
	return eAR;
}
