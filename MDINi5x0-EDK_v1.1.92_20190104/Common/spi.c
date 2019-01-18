//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2005  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	SPI.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
VBYTE fSysSPILineBUSY = FALSE;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
BYTE ISPM SPI_Write(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
//	WORD i;	
	BYTE rBuff[2], ch;
	
#if defined(__MDIN_i5XX_FPGA__)		// FPGA board
	ch = SPI_CH;
#else								// EDK & REF board
	if((sAddr==SPI_AN41908_ADDR) || (sAddr==SPI_AN41919_ADDR))		ch = SPI_CH1;	//spi1(peri_spi)
	else															ch = SPI_CH0;	//spi0(sensor_spi)
#endif	// __MDIN_i5XX_FPGA__
	
	spi_wait_empty_fifo(ch);		// check spi  line is ready for use and it is not corrupted, //by hungry 2013.09.06 adc_ucom
	
	if (sAddr==SPI_MN34041_ADDR || sAddr==SPI_MN34220_ADDR || sAddr==SPI_MN34420_ADDR) {		// LSB first
		rBuff[0] = LOBYTE(rAddr)&0xff;    // LSB1st
		rBuff[1] = HIBYTE(rAddr)&0x7f;
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}
	else if (sAddr == SPI_IMX076_ADDR_1 || sAddr == SPI_IMX076_ADDR_2) {		// LSB first
		rBuff[0] = LOBYTE(sAddr);
		rBuff[1] = LOBYTE(rAddr)&0xff;	
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}
	else if (sAddr==SPI_IMX124_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)&0xff;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}
	else if (sAddr==SPI_IMX174_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)&0xff;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}	
	else if (sAddr==SPI_IMX136_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)&0xff;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}
	else if (sAddr==SPI_IMX185_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)&0xff;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}	
	else if (sAddr==SPI_IMX222_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)&0xff;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}		
	else if (sAddr==SPI_IMX238_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)&0xff;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}		
	else if (sAddr==SPI_IMX290_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)&0xff;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}
	else if (sAddr==SPI_IMX385_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)&0xff;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}	
	else if (sAddr==SPI_IMX252_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)&0xff;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}			
	else if (sAddr==SPI_IMX265_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)&0xff;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}		
	else if (sAddr==SPI_IMX178_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)&0xff;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}		
	else if (sAddr==SPI_IMX253_ADDR){	// LSB first
		rBuff[0] = HIBYTE(rAddr)&0xff;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}		
	else if (sAddr==SPI_IMX273_ADDR){	// LSB first
		rBuff[0] = HIBYTE(rAddr)&0xff;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}			
	else if (sAddr==SPI_IMX226_ADDR){	// LSB first
		rBuff[0] = 0x81;			// write reg Upper addr
		spi_master_xfer(ch, &rBuff[0], 1, NULL, 0, 1);	//send chip id
		
		rBuff[0] = HIBYTE(rAddr)&0xff;			// write reg Upper addr
		rBuff[1] = LOBYTE(rAddr)&0xff;			// write reg Lower addr
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}
	else if (sAddr==SPI_IMX274_ADDR){	// LSB first
		rBuff[0] = 0x81;			// write reg Upper addr
		spi_master_xfer(ch, &rBuff[0], 1, NULL, 0, 1);	//send chip id
		
		rBuff[0] = HIBYTE(rAddr)&0xff;			// write reg Upper addr
		rBuff[1] = LOBYTE(rAddr)&0xff;			// write reg Lower addr
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}
	else if (sAddr==SPI_EXT3_ADDR){	// LSB first	
		rBuff[0] = LOBYTE(rAddr)&0xff;			// write reg Upper addr
		rBuff[1] = HIBYTE(rAddr)&0x7f;			// write reg Lower addr
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
	}	
	else {							// LSB first
		rBuff[0] = LOBYTE(rAddr)&0x3f;
		spi_master_xfer(ch, rBuff, 1, NULL, 0, 1);
	}
	
//	for (i=0; i<bytes; i++) {		// Transmit a buffer data
//		rBuff[0] = LSB_1ST(pBuff[i]);	
//		spi_master_xfer(ch, rBuff, 1, NULL, 0, (i==(bytes-1))? 0: 1);
//	}
	
	spi_master_xfer(ch, pBuff, bytes, NULL, 0, 0);
	
	return SPI_OK;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE SPI_Read(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
//	WORD i;	
	BYTE rBuff[2], ch;

	BYTE stat;	
	
#if defined(__MDIN_i5XX_FPGA__)		// FPGA board
	ch = SPI_CH;
#else								// EDK & REF board
	if((sAddr==SPI_AN41908_ADDR) || (sAddr==SPI_AN41919_ADDR))		ch = SPI_CH1;	//spi1(peri_spi)
	else															ch = SPI_CH0;	//spi0(sensor_spi)
#endif	// __MDIN_i5XX_FPGA__
	
	spi_wait_empty_fifo(ch);		// check spi  line is ready for use and it is not corrupted //by hungry 2013.09.07 adc_ucom
	
	if (sAddr==SPI_MN34041_ADDR || sAddr==SPI_MN34220_ADDR || sAddr==SPI_MN34420_ADDR) {		// LSB first
		rBuff[0] = LOBYTE(rAddr)&0xff;      // LSB1st
		rBuff[1] = HIBYTE(rAddr)|0x80;

#if 0
		//case selector use
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);
		//S_GPIOx(LOW);		
		spi_master_xfer(ch, NULL, 0, pBuff, bytes, 0);
		//S_GPIOx(HIGH);
#else
		//case short r9(miso, mosi and SIO short at sensor pcb)
		spi_master_xfer(ch, rBuff, 2, NULL, 0, 1);	//spi addr 1word write
		
		*R_SPI0CON(ch) = SPICON_MSTR |SPICON_CPOL |SPICON_CPHA| SPICON_WOMP | SPICON_EN;

		*R_SPI0DATA(ch) = 0xff;
		do {stat = *R_SPI0STAT(ch);}        while (!(stat & SPISTAT_SPIF));
		pBuff[0] = *R_SPI0DATA(ch);

		*R_SPI0DATA(ch) = 0xff;
		do {stat = *R_SPI0STAT(ch);}        while (!(stat & SPISTAT_SPIF));
		pBuff[1] = *R_SPI0DATA(ch);

 		*R_SSX0CON(ch) = 1;   // SPI nSS High
		*R_SPI0CON(ch) = SPICON_MSTR |SPICON_CPOL |SPICON_CPHA| SPICON_EN;
#endif

	}
	else if (sAddr == SPI_IMX076_ADDR_1 || sAddr == SPI_IMX076_ADDR_2) {		// LSB first		
		rBuff[0] = LOBYTE(sAddr)|0x80;
		rBuff[1] = LOBYTE(rAddr)|0x40;
		spi_master_xfer(ch, rBuff, 2, pBuff, bytes, 0);
	}
	else if (sAddr==SPI_IMX124_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)|0x80;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, pBuff, bytes, 0);
	}
	else if (sAddr==SPI_IMX174_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)|0x80;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, pBuff, bytes, 0);
	}	
	else if (sAddr==SPI_IMX136_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)|0x80;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, pBuff, bytes, 0);
	}
	else if (sAddr==SPI_IMX185_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)|0x80;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, pBuff, bytes, 0);
	}	
	else if (sAddr==SPI_IMX222_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)|0x80;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, pBuff, bytes, 0);
	}		
	else if (sAddr==SPI_IMX238_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)|0x80;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, pBuff, bytes, 0);
	}	
	else if (sAddr==SPI_IMX290_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)|0x80;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, pBuff, bytes, 0);
	}
	else if (sAddr==SPI_IMX385_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)|0x80;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, pBuff, bytes, 0);
	}	
	else if (sAddr==SPI_IMX252_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)|0x80;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, pBuff, bytes, 0);
	}			
	else if (sAddr==SPI_IMX265_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)|0x80;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, pBuff, bytes, 0);
	}		
	else if (sAddr==SPI_IMX178_ADDR) {	// LSB first
		rBuff[0] = HIBYTE(rAddr)|0x80;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, pBuff, bytes, 0);
	}		
	else if (sAddr==SPI_IMX253_ADDR){	// LSB first
		rBuff[0] = HIBYTE(rAddr)|0x80;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, pBuff, bytes, 0);
	}	
	else if (sAddr==SPI_IMX273_ADDR){	// LSB first
		rBuff[0] = HIBYTE(rAddr)|0x80;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, pBuff, bytes, 0);
	}		
	else if (sAddr==SPI_IMX226_ADDR){	// LSB first
//		rBuff[0] = 0x81;			// write reg Upper addr
		rBuff[0] = 0x82;			// write reg Upper addr
		spi_master_xfer(ch, &rBuff[0], 1, NULL, 0, 1);	//send chip id
	
		rBuff[0] = HIBYTE(rAddr)&0xff;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, pBuff, bytes, 0);
	}
	else if (sAddr==SPI_IMX274_ADDR){	// LSB first
//		rBuff[0] = 0x81;			// write reg Upper addr
		rBuff[0] = 0x82;			// write reg Upper addr
		spi_master_xfer(ch, &rBuff[0], 1, NULL, 0, 1);	//send chip id
	
		rBuff[0] = HIBYTE(rAddr)&0xff;
		rBuff[1] = LOBYTE(rAddr)&0xff;
		spi_master_xfer(ch, rBuff, 2, pBuff, bytes, 0);
	}
	else if (sAddr==SPI_EXT3_ADDR){	// LSB first
		rBuff[0] = LOBYTE(rAddr)&0xff;
		rBuff[1] = HIBYTE(rAddr)|0x80;
		spi_master_xfer(ch, rBuff, 2, pBuff, bytes, 0);
	}	
	else {
		rBuff[0] = LOBYTE(rAddr)|0x40;
		spi_master_xfer(ch, rBuff, 1, pBuff, bytes, 0);
	}
	
//	for(i=0; i<bytes; i++)		pBuff[i] = LSB_1ST(pBuff[i]);
	
	return SPI_OK;
}

/*  FILE_END_HERE */
