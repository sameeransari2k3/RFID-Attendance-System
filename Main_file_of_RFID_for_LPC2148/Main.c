#include <LPC21xx.h>
#include <string.h>
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "delay.h"
#include "uart.h"
#include "interrupts_pro.h"
#include "RTC.h"
#include "KPM.h"
char rfid[12],r_flag=0;
int i=0;
s32 hour,min,sec,day,date,month,year;
int main()
{
	InitLCD();
	Initkpm();
	Init_UART0();
	Init_UART1();
	Enable_UART1();
	Enable_EINT1();
	RTC_Init();
	//SetRTCTimeInfo(12,44,20);
	//SetRTCDateInfo(10,5,2025);
	//SetRTCDay(6);		  
    while(1)
		{ 
			StrLCD("Gausia");
			cmdLCD(0XC0);
			StrLCD("ARM PROJECT");
			delay_ms(1000);
			cmdLCD(0X01);
			while(r_flag!=2)
			{
				GetRTCTimeInfo(&hour,&min,&sec);
				GetRTCDateInfo(&date,&month,&year);
			   	DisplayRTCTime(hour,min,sec);
			   	DisplayRTCDate(date,month,year);
			}
			r_flag=0;
			cmdLCD(0X01);
			for(i=0;i<strlen(rfid);i++)
			{
				if(rfid[i]==0x02)
				{
					memmove(rfid+i,rfid+i+1,strlen((rfid+i+1)+1));
				}
				else if(rfid[i]==0x03)
				{
					memmove(rfid+i,rfid+i+1,strlen((rfid+i+1)+1));
				}
			}
     
			StrLCD(rfid);
			U0_TXByte('S'); 
			U0_TXStr(rfid);
			U0_TXByte('#');
			U0_TXByte(',');
			DisplayRTCDate_uart(date,month,year);
			U0_TXByte('$');
			delay_ms(1000);
			//memset(rfid, 0, sizeof(rfid));
			i=0;
          	cmdLCD(0X01);
		}
}

void eint0_isr(void)__irq
{
	u8 choice,choice2;
	u32 pass,password=786,i=3;
L4:	 cmdLCD(CLEAR_LCD);
	 cmdLCD(RET_CUR_HOME);
	 StrLCD("Enter Password:");
	 pass=Password_scan();
	 if(pass==password)	
	 {
L2:	cmdLCD(CLEAR_LCD);
	cmdLCD(RET_CUR_HOME);
	StrLCD("1:Set Admin");
	cmdLCD(GOTO_LINE2_POS0);
	StrLCD("2:Set RTC");

	choice=keyScan();
	if(choice=='1' )
	{
		cmdLCD(CLEAR_LCD);
		cmdLCD(RET_CUR_HOME);
	}
	else if(choice=='2')
	{
L1:cmdLCD(CLEAR_LCD);
   cmdLCD(RET_CUR_HOME);
   cmdLCD(GOTO_LINE1_POS0);
   StrLCD("1.H 2.M 3.S 4.D");
   cmdLCD(GOTO_LINE2_POS0);
   StrLCD("5.M 6.Y 7.D 8.E");
   choice2=keyScan();
   switch(choice2)
   {
	   case'1': cmdLCD(CLEAR_LCD);
		    cmdLCD(GOTO_LINE1_POS0);
		    StrLCD("Set Hour (00-23)");
		    hour=mykeyRead();
		    if(hour>=0 && hour<=23)
		    {
			    HOUR=hour;
		    }
		    else
		    {
			    cmdLCD(CLEAR_LCD);
			    cmdLCD(RET_CUR_HOME);
			    StrLCD("Out of Range");
			    cmdLCD(GOTO_LINE2_POS0);
			    StrLCD("Range (00 to 23)");
			    delay_ms(1000);										 
			    goto L1;
		    }
		    goto L1;

	   case'2': cmdLCD(CLEAR_LCD);
		    cmdLCD(GOTO_LINE1_POS0);
		    StrLCD("Set Min (00-59)");
		    min=mykeyRead();
		    if(min>=0 && min<=59)
		    {
			    MIN=min;
		    }
		    else
		    {
			    cmdLCD(CLEAR_LCD);
			    cmdLCD(RET_CUR_HOME);
			    StrLCD("Out of Range");
			    cmdLCD(GOTO_LINE2_POS0);
			    StrLCD("Range (00 to 59)");
			    delay_ms(1000);											 
			    goto L1;
		    }
		    goto L1;

	   case'3': cmdLCD(CLEAR_LCD);
		    cmdLCD(GOTO_LINE1_POS0);
		    StrLCD("Set Sec (00-59)");
		    sec=mykeyRead();
		    if(sec>=0 && sec<=59)
		    {
			    SEC=sec;
		    }
		    else
		    {
			    cmdLCD(CLEAR_LCD);
			    cmdLCD(RET_CUR_HOME);
			    StrLCD("Out of Range");
			    cmdLCD(GOTO_LINE2_POS0);
			    StrLCD("Range (00 to 59)");
			    delay_ms(1000);												
			    goto L1;
		    }
		    goto L1;

	   case'4': cmdLCD(CLEAR_LCD);
		    cmdLCD(GOTO_LINE1_POS0);
		    StrLCD("Set Day (0-6)");
		    day=mykeyRead();
		    if(day>=0 && day<=6)
		    {
			    DOW=day;
		    }
		    else
		    {
			    cmdLCD(CLEAR_LCD);
			    cmdLCD(RET_CUR_HOME);
			    StrLCD("Out of Range");
			    cmdLCD(GOTO_LINE2_POS0);
			    StrLCD("Range (0 to 6)");
			    delay_ms(1000); 
			    goto L1;
		    }
		    goto L1;

	   case'5': cmdLCD(CLEAR_LCD);
		    cmdLCD(GOTO_LINE1_POS0);
		    StrLCD("Set Month (1-12)");
		    month=mykeyRead();
		    if(month>=1 && month<=12)
		    {
			    MONTH=month;
		    }
		    else
		    {
			    cmdLCD(CLEAR_LCD);
			    cmdLCD(RET_CUR_HOME);
			    StrLCD("Out of Range");
			    cmdLCD(GOTO_LINE2_POS0);
			    StrLCD("Range (1 to 12)");
			    delay_ms(1000);												 
			    goto L1;
		    }
		    goto L1;

	   case'6': cmdLCD(CLEAR_LCD);
		    cmdLCD(GOTO_LINE1_POS0);
		    StrLCD("STY (2020-3000)");
		    year=mykeyRead();
		    if(year>=2020 && year<=3000)
		    {
			    YEAR=year;
		    }
		    else
		    {
			    cmdLCD(CLEAR_LCD);
			    cmdLCD(RET_CUR_HOME);
			    StrLCD("Out of Range");
			    cmdLCD(GOTO_LINE2_POS0);
			    StrLCD("(2020 to 3000)");
			    delay_ms(1000);	
			    goto L1;		
		    }
		    goto L1;

	   case'7': cmdLCD(CLEAR_LCD);
		    cmdLCD(GOTO_LINE1_POS0);
		    StrLCD("Set Date (01-31)");
		    date=mykeyRead();
		    if(date>=1 && date<=31)
		    {
			    DOM=date;
		    }
		    else
		    {
			    cmdLCD(CLEAR_LCD);
			    cmdLCD(RET_CUR_HOME);
			    StrLCD("Out of Range");
			    cmdLCD(GOTO_LINE2_POS0);
			    StrLCD("Range (1 to 31)");
			    delay_ms(1000);											 
			    goto L1;
		    }
		    goto L1;

	   case'8':break;
	   default: cmdLCD(CLEAR_LCD);
		    cmdLCD(RET_CUR_HOME);
		    StrLCD("Invalid choice");
		    cmdLCD(GOTO_LINE2_POS0);
		    StrLCD("Select 1 to 8");
		    delay_ms(1000);
		    goto L1;
   }
	}
	else
	{
		cmdLCD(CLEAR_LCD);
		cmdLCD(RET_CUR_HOME);
		StrLCD("Invalid choice");
		cmdLCD(GOTO_LINE2_POS0);
		StrLCD("select 1 or 2");
		delay_ms(1000);
		goto L2;
	}
	 }
	 else if(i==0)
	 {
		 cmdLCD(CLEAR_LCD);
		 cmdLCD(RET_CUR_HOME);
		 StrLCD("0 attempts left");
		 cmdLCD(GOTO_LINE2_POS0);
		 StrLCD("permission denied!");
		 delay_ms(1000);
		 cmdLCD(CLEAR_LCD);
	 }
	 else
	 {
		 cmdLCD(CLEAR_LCD);
		 cmdLCD(RET_CUR_HOME);
		 StrLCD("Worng Password!");
		 cmdLCD(GOTO_LINE2_POS0);
		 U32LCD(i);
		 StrLCD(" Attempts left");
		 delay_ms(1000);
		 i--;
		 goto L4;
	 }
	 cmdLCD(CLEAR_LCD);
	 EXTINT=1<<1;
	 VICVectAddr=0;
}
void UART1_isr(void) __irq
{
  char byte;

  if((U1IIR & 0x04))

  { 
   byte = U1RBR;
    if(byte==0x02)
	{ 
	  r_flag=1; 
	}
    else if ((byte != 0x03)&&(r_flag))     
       {
	  	 rfid[i++]=byte; 
	   }   

	else
     {
            rfid[i] = '\0';
			i=0;
			r_flag=2;      
     }		
     VICVectAddr = 0;
 }
}
