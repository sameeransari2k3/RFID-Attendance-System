#include <LPC21xx.h>
#include <string.h>
#include "lcd.h"
#include "delay.h"
#include "uart.h"
#include "spi.h"
#include "spi_defines.h"
#include "KPM.h"
#include "interrupts_pro.h"
#include "RTC.h"


char rfid[10],r_flag=0;
int i=0;
s8 rByte[10];
s32 hour,min,sec,day,date,month,year;
int main()
{
	s8 arr[]="00307278";
	u8 ch;
	InitLCD();
	Init_UART1();
	Init_UART0();
	Init_SPI0();
	Enable_UART1();
	Enable_EINT1();
	Initkpm();
	RTC_Init();
	for(i=0;i<10;i++)
	{
	        ByteWrite(0x00+i,arr[i]);
  } 
	for(i=0;i<10;i++)
    {
      rByte[i]=ByteRead(0x00+i);
    }	  
    while(1)
		{ 
			cmdLCD(0X01);
			StrLCD("RFID PROJECT");
			cmdLCD(0XC0);
			StrLCD(rByte);
			delay_s(1);
			cmdLCD(0X01);
			while(r_flag!=2)
			{
				  GetRTCTimeInfo(&hour,&min,&sec);
				  GetRTCDateInfo(&date,&month,&year);
			   	DisplayRTCTime(hour,min,sec);
			    DisplayRTCDate(date,month,year);
				  delay_ms(1000);
			};							
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
			cmdLCD(0X01);	
			r_flag=0;
			i=0;
			if(strcmp(rfid,rByte)==0)
			{
				 StrLCD("Admin");
				 U0_TXByte('A'); 
				 U0_TXStr(rfid);
				 U0_TXByte('#');
				 U0_TXByte(',');
				 DisplayRTCDate_uart(date,month,year);
				 U0_TXByte('$');
				 delay_ms(1000);
				 i=0;
         		cmdLCD(0X01);					
			}
			else
			{
			    //StrLCD(rfid);
				U0_TXByte('S'); 
				U0_TXStr(rfid);
				U0_TXByte('#');
				U0_TXByte(',');
				DisplayRTCDate_uart(date,month,year);
				U0_TXByte('$');
				delay_ms(1000);
				i=0;
        		cmdLCD(0X01);
				ch=U0_RxByte();
				if(ch=='W')
				{
				   StrLCD("Welcome!!");
				}
				else
				{
					StrLCD("Invalid card");
				}
				delay_s(2);
			}
			 //StrLCD(U0_ReadStr());	
}		
}
void eint0_isr()__irq
{
	u8 choice,choice2;
	u32 pass,password=786,i=3;
	u32 a=0;
	s8 arr[10];
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
		StrLCD("Enter Card no.");
	    cmdLCD(GOTO_LINE2_POS0);
		for(a=0;a<9;a++)
       	{
			 arr[a]=0;
		}
		for(a=0;a<9;a++)
       	{		arr[a]=keyScan();
				if(arr[a]=='=')
				{
					arr[a]=0;
					break;
				}
				else if(arr[a]=='-')
				{
					cmdLCD(GOTO_LINE2_POS0);
					StrLCD("                ");
					arr[a]=0;
					a--;
					arr[a]=0;
					a--;
				   cmdLCD(GOTO_LINE2_POS0);
				}
				cmdLCD(GOTO_LINE2_POS0);
				StrLCD("                ");
				cmdLCD(GOTO_LINE2_POS0);
				StrLCD(arr);
                ByteWrite(0x00+a,arr[a]);
        }
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
	for(a=0;a<10;a++)
    {
      rByte[a]=ByteRead(0x00+a);
    }
	 EXTINT=1<<1;
	 VICVectAddr=0;
}
void UART1_isr() __irq
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
