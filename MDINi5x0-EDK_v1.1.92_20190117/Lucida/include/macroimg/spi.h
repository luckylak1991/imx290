#pragma once

#define SPI_CH0	0
#define SPI_CH1	1

#define SPI_CH	SPI_CH0
//#define SPI_CH	SPI_CH1

#define SPI_MAX_WFIFO   8
#define SPI_MAX_RFIFO   8
#define SPI_BASE_ADDR	0x80010C00
#define R_SPI0CON(N)      ((volatile unsigned int*)(SPI_BASE_ADDR+(N*0x40)+0))
#define SPICON_SPISIZE8B   (0 << 0)
#define SPICON_SPISIZE16B  (1 << 0)
#define SPICON_SPISIZE32B  (2 << 0)
#define SPICON_MSBF        (1 << 2)
#define SPICON_CPHA        (1 << 3)
#define SPICON_CPOL        (1 << 4)
#define SPICON_MSTR        (1 << 5)
#define SPICON_WOMP        (1 << 6)
#define SPICON_EN          (1 << 7)

#define	R_SPI0BAUD(N)	((volatile unsigned int*)(SPI_BASE_ADDR+(N*0x40)+4))

#define R_SPI0STAT(N)	((volatile unsigned int*)(SPI_BASE_ADDR+(N*0x40)+8))
#define SPISTAT_SRXE     (1 << 0)
#define SPISTAT_SRXF     (1 << 1)
#define SPISTAT_STXE     (1 << 2)
#define SPISTAT_STXF     (1 << 3)
#define SPISTAT_SSX      (1 << 4)
#define SPISTAT_MODF     (1 << 5)
#define SPISTAT_WCOL     (1 << 6)
#define SPISTAT_SPIF     (1 << 7)

#define R_SPI0DATA(N)     ((volatile unsigned int*)(SPI_BASE_ADDR+(N*0x40)+0xc))

#define R_SSX0CON(N)      ((volatile unsigned int*)(SPI_BASE_ADDR+(N*0x40)+0x10))
#define SSXCR_SSXCON    (1 << 0)

#define R_SPI0INT(N)	    ((volatile unsigned int*)(SPI_BASE_ADDR+(N*0x40)+0x14))
#define SIMK_SRXEE     (1 << 0)
#define SIMK_SRXFE     (1 << 1)
#define SIMK_STXEE     (1 << 2)
#define SIMK_STXFE     (1 << 3)
#define SIMK_SSXE      (1 << 4)
#define SIMK_MODFE     (1 << 6)
#define SIMK_SPIFE     (1 << 7)

#define SPI_FIFOSIZE    64

#define SPI_BAUD(clk)    ((get_apb_clock()  / (2*clk))-1)
#define SPI_SCK(baud)    (get_apb_clock()  / (2*(baud+1)))

#define SPI_MASTER      0
#define SPI_SLAVE       1

#define SPI_MASTER_MAXFREQ      (get_apb_clock()/2)
#define SPI_SLAVE_MAXFREQ       (get_apb_clock()/4)

#define SPI_CS_LOW(ch)   
#define SPI_CS_HIGH(ch)  

void spi_master_init(int ch);
int spi_set_freq (int ch,int mode, U32 freq);
//void spi_master_xfer (int ch,U8 *wbuf, int wlength, U8 *rbuf, int rlength, int continue_xfer);
//void spi_wait_empty_fifo (int ch);
void ISPM spi_master_xfer (int ch,U8 *wbuf, int wlength, U8 *rbuf, int rlength, int continue_xfer);
void ISPM spi_wait_empty_fifo (int ch);
