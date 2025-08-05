//spi_eeprom.h
#include "types.h"

void Cmd(u8 );
void ByteWrite(u16,u8);
u8 ByteRead(u16 rBuffAddr);

//spi.h
void Init_SPI0(void);
u8 spi(u8 );
