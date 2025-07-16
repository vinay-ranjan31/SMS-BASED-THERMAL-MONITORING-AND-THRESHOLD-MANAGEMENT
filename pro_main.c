#include<lpc214x.h>
#include<stdlib.h>
#include<string.h>

#include "types.h"
#include "lcd.h"
#include "spi.h"
#include "spi_eeprom.h"
#include "delay.h"
#include "adc.h"
#include "gsm.h"
#include "uart0.h"

#define TEMP_ADDR  0X00
#define M_ADDR    0X10

s8 s1[100];
extern char buff[100];
extern unsigned char i,r_flag;
u8 cur_temp,m_no[]="9391025330",syntax[]="1234",r_temp,rm_no[11],j;

void extract_msg(u8 *p)
{
	u8 ch,k,t[]="TEMP:";
	//l=strlen((const char *)syntax);
	if(strstr((const char *)p,(const char *)syntax))
	{
		ch=*(p+4);
		switch(ch)
		{
			case 'T':k=atoi((const char *)(p+4+1));
					Write_CMD_LCD(0x01);
					Write_int_LCD(k);
					delay_ms(2000);
					ByteWrite_25LC512(TEMP_ADDR,k);
					r_temp=ByteRead_25LC512(TEMP_ADDR);
					Write_CMD_LCD(0x01);
					Write_str_LCD("SPOINT CHANGED");
					delay_ms(2000);
					Write_CMD_LCD(0x01);
					break;

			case 'M':p=p+4+1;
					for(k=0;k<10;k++)
					{
						m_no[k]=*p++;
					}
					m_no[k]='\0';
					Write_CMD_LCD(0x01);
					Write_str_LCD((char *)m_no);
					delay_ms(2000);	
					Write_CMD_LCD(0x01);
					for(j=0;m_no[j];j++)
					{
						ByteWrite_25LC512(M_ADDR+j,m_no[j]);
					}
					for(j=0;j<10;j++)
					{
						rm_no[j]=ByteRead_25LC512(M_ADDR+j);
					}
					rm_no[j]='\0';
					Write_str_LCD("PHNO CHANGED");
					delay_ms(2000);
					Write_CMD_LCD(0x01);
					break;	
			case 'I':for(j=0;j<5;j++)
					{
						p[j]=t[j];
					}
					j=j+1;
					k=cur_temp;
					while(k)
					{
						p[j--]=k%10+48;
						k=k/10;
					}
					p[j+2]='\0';
					send_sms((char *)m_no,(char *)p);
					break;
		}
	}
	else
	{
		//Error Msg
		Write_CMD_LCD(0x01);
		Write_str_LCD("INVALID SYNTAX");
		delay_ms(2000);	
		Write_CMD_LCD(0x01);
	}
}

int main()
{
	LCD_Init();
	Write_str_LCD(" WELCOME TO ");
	Write_CMD_LCD(0XC0);
	Write_str_LCD("MAJOR PROJECT");
	delay_ms(2000);
	Write_CMD_LCD(0x01);
	Init_ADC();
	InitUART0();
	Init_SPI0();
	GSM_init();
	//ByteWrite_25LC512(TEMP_ADDR,40);
	r_temp=ByteRead_25LC512(TEMP_ADDR);
	/*for(j=0;m_no[j];j++)
	{
		ByteWrite_25LC512(M_ADDR+j,m_no[j]);
	} */
	for(j=0;j<10;j++)
	{
		rm_no[j]=ByteRead_25LC512(M_ADDR+j);
	}
	rm_no[j]='\0';
	Write_CMD_LCD(0x01);
	Write_str_LCD("OWNER NUMBER");
	Write_CMD_LCD(0XC0);
	Write_str_LCD((char *)rm_no);
	delay_ms(2000);
	Write_CMD_LCD(0x01);
	while(1)
	{
		do
		{
			Write_CMD_LCD(0x80);
			cur_temp=Read_ADC(1)*100;
			Write_str_LCD("TEMP: ");
			Write_int_LCD(cur_temp);
			Write_DAT_LCD(223);
			Write_DAT_LCD('C');
			Write_CMD_LCD(0xC0);
			Write_str_LCD("SET POINT: ");
			Write_int_LCD(r_temp);
			Write_DAT_LCD(223);
			Write_DAT_LCD('C');
			if(cur_temp>r_temp)
			{
				Write_CMD_LCD(0x01);
				Write_str_LCD("ALERT! ");
				delay_ms(1000);
				send_sms((char *)rm_no,"ALERT! TEMPERATURE EXCEED THE SETPOINT");
				Write_CMD_LCD(0x01);	
			}
			delay_ms(1000);
		}while(r_flag==0);
		r_flag=0;
		read_sms();
		i=0;memset(buff,'\0',100);
		UART_str("AT+CMGD=1\r\n");
		while(i<2);
		delay_ms(500);
		buff[i] = '\0';
		if(strstr(buff,"OK"))
		{		
			Write_CMD_LCD(0x80);
			Write_str_LCD("OK");
			delay_ms(500);
			Write_CMD_LCD(0x80);
			Write_str_LCD("  ");
		}
		else
		{
			Write_CMD_LCD(0x80);
			Write_str_LCD("ERROR");
			delay_ms(500);
			Write_CMD_LCD(0x80);
			Write_str_LCD("     ");
		}
		extract_msg((u8 *)s1);
	}
}
