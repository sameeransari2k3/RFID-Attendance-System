#define WRITE 0x02
#define READ 0x03
#define WRDI 0x04
#define WREN 0x06

//spi_defines.h

#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK (CCLK/4)


//defines for SxSPCR sfr

#define CPHA_BIT 3
#define CPOL_BIT 4
#define MSTR_BIT 5


//defines for SxSPSR sfr

#define SPIF_BIT 7

//defines for SPI0 pins

#define SCK0_PIN  PIN_FUNC2  //@p0.4
#define MISO0_PIN PIN_FUNC2  //@p0.5
#define MOSI0_PIN PIN_FUNC2  //@p0.6
#define SS0_PIN   7          //@p0.7

