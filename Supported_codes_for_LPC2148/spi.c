//spi.c
#include <LPC21xx.h>
#include "types.h"
#include "spi_defines.h"
#include "spi.h"
#include "delay.h"

void Init_SPI0(void)

{
//Cfg p0.4,p0.5,p0.6 as sck0,miso0,mosi0 pins
//	CfgPortPinFunc(0,4,SCK0_PIN);
//	CfgPortPinFunc(0,5,MISO0_PIN);
//	CfgPortPinFunc(0,6,MOSI0_PIN);
	PINSEL0|=0x00001500;
//Cfg SPI settings
//Mode3,MSBF

	S0SPCR=(1<<CPHA_BIT)|(1<<CPOL_BIT)|(1<<MSTR_BIT);

	//cfg spi clock freq

	S0SPCCR=14;

	//cfg slave select pin as gpio out pin
	//& ensuring pin is initially high

	IOSET0=1<<SS0_PIN;
	IODIR0|=1<<SS0_PIN;	
}


u8 spi(u8 sByte)

{
	//write to tx/rx buffer
	//which initiates serialization via mosi & miso 

	S0SPDR=sByte;

	//wait until serialization complete

	while(((S0SPSR>>SPIF_BIT)&1)==0);

	//read & return recvd byte

	return S0SPDR;	

}
//spi_eeprom.c

void Cmd(u8 cmdByte)

{
	IOCLR0=1<<SS0_PIN;
	spi(cmdByte);
	IOSET0=1<<SS0_PIN;
}

void ByteWrite(u16 wBuffAddr,u8 wByte)

{
	Cmd(WREN);
	IOCLR0=1<<SS0_PIN;
	spi(WRITE);
	spi(wBuffAddr>>8);
	spi(wBuffAddr);
	spi(wByte);
	IOSET0=1<<SS0_PIN;
	delay_ms(5);
	Cmd(WRDI);
}


u8 ByteRead(u16 rBuffAddr)
{
	u8 rByte=0;
	IOCLR0=1<<SS0_PIN;
	spi(READ);
	spi(rBuffAddr>>8);
	spi(rBuffAddr);
	rByte=spi(0x00);
	IOSET0=1<<SS0_PIN;
	return rByte;
}


