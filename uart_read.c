#include "types.h"
#include "delay.h"
#include "lcd.h"
#include "uart0.h"
#include <string.h>

extern char buff[100],i;
extern s8 s1[100];
char *p,cnt;
void read_sms(void)
{
	u32 j=0;
	Write_CMD_LCD(0x01);
	Write_str_LCD("recieving sms....");
	delay_ms(500);
	i=0;memset(buff,'\0',200);
	UART_str("AT+CMGR=1\r\n");
	delay_ms(1000);
	while(i<2);
	buff[i] = '\0';
	if(strstr(buff,"OK"))
	{
		Write_CMD_LCD(0x01);
		Write_str_LCD("Message recieve..");
		delay_ms(500);
		p=buff;
		p=strchr(p,0x0a);
		while(p)
		{
			cnt++;
			if(cnt==2)
			{
				cnt=0;
				p++;
				break;
			}
			p++;
			p=strchr(p,0x0a);
		}
		i=p-buff;
		for(;buff[i]!=0x0d;i++)
		{
			s1[j++]=buff[i];
		}
		s1[j++]=' ';
		s1[j]='\0';
	}
	else
	{
		Write_CMD_LCD(0x01);
		Write_str_LCD("Message failed..");
		delay_ms(500);
	}
}
