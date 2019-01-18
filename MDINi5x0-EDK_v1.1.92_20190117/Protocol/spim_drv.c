//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2016  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	SPIM_DRV.C
// Description 		: spi-master driver
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
SPIm_APP_PARAM	stSPImAPP;

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
void SPIm_Initialize(void)
{
	memset(&stSPImAPP, 0, sizeof(SPIm_APP_PARAM));
}

//----------------------------------------------------------------------------------
static void MISP_CS_LOW(BYTE cID)
{
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	switch(cID)	{
		case  1:	MISP_CS_S1(LOW);	break;
		case  2:	MISP_CS_S2(LOW);	break;
		//case  3:	MISP_CS_S3(LOW);	break;
	}
#endif
}

//----------------------------------------------------------------------------------
static void MISP_CS_HIGH(BYTE cID)
{
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	switch(cID)	{
		case  1:	MISP_CS_S1(HIGH);	break;
		case  2:	MISP_CS_S2(HIGH);	break;
		//case  3:	MISP_CS_S3(HIGH);	break;
	}
#endif
}

//----------------------------------------------------------------------------------
static void SPIm_TxDword(BYTE cID, DWORD cmd)
{
	BYTE ch=1;	// spi-1
	BYTE buff[4];

	buff[0] = LOBYTE(LOWORD(cmd));		buff[1] = HIBYTE(LOWORD(cmd));	//lsb 1st
	buff[2] = LOBYTE(HIWORD(cmd));		buff[3] = HIBYTE(HIWORD(cmd));
	
	while (fSysSPILineBUSY);	// check SPI-Line status
	
	Delay_uSec(2);				// delay for zf spi control
	
	MISP_CS_LOW(cID);
	spi_wait_empty_fifo(ch);
	spi_master_xfer(ch, &buff[0], 4, NULL, 0, 0);	// send 4byte
	Delay_uSec(4);				// delay for slave
	MISP_CS_HIGH(cID);
	
	Delay_uSec(SPIM_TDLY);		// delay xxus(t_delay) -- delay : cpu cache 'on'(1us), 'off'(4us)
}


//--------------------------------------------------------------------------------
static void SPIm_RxDword(BYTE cID, PBYTE pBuff)
{
	BYTE ch=1;	// spi-1
	
	while (fSysSPILineBUSY);	// check SPI-Line status
	
	MISP_CS_LOW(cID);
	spi_wait_empty_fifo(ch);
	spi_master_xfer(ch, NULL, 0, pBuff, 4, 0);	// receive 4byte
	Delay_uSec(4);				// delay for slave
	MISP_CS_HIGH(cID);
	
	Delay_uSec(SPIM_TDLY);		// delay xxus(t_delay) -- delay : cpu cache 'on'(1us), 'off'(4us)
}

//--------------------------------------------------------------------------------
MISP_ERROR_t SPIm_Command(BYTE cID, DWORD cmd)
{
	BYTE ack[4]={0,0,0,0};
	WORD retry_cnt=0;

	while((ack[3] != 0x40) || (ack[2] != 0x10))  {  // check ack from slave
		SPIm_TxDword(cID, cmd);  // send command
		SPIm_RxDword(cID, ack);  // get ack
		
		if((ack[3] != 0x40) || (ack[2] != 0x10))	{
			if(retry_cnt++ < 10)		Delay_mSec(10);			// retry every 10ms
			else						{UARTprintf("id[%d], cmd[0x%08x], no ack ![%x%x%x%x] \r\n", cID, cmd, ack[3],ack[2],ack[1],ack[0] );	return MISP_SPI_ERROR;}
		}
	}
	
//	if(retry_cnt)		UARTprintf("id[%d], cmd[0x%08x], retry count[%d] \r\n", cID, cmd, retry_cnt);
	return MISP_NO_ERROR;
}


//---------------------------------------------------------------------------------
void SPIm_Inquiry(BYTE cID, DWORD inq, PBYTE pBuff)
{
	SPIm_TxDword(cID, inq);		// send command inquiry
	SPIm_RxDword(cID, pBuff);	// get reply
}

//---------------------------------------------------------------------------------
void SPIm_Inq3aRawData(BYTE cID, PBYTE pBuff, PBYTE pSize)
{
	BYTE  reply[4];
	BYTE  i;
	
	SPIm_Inquiry(cID, SPIM_INQ_3A_RAWDATA, &reply[0]);	// send inquiry(get af data)
	*pSize = reply[1];									// rx data size(dword) from slave
	
	for(i=0;i<*pSize;i++)		SPIm_RxDword(cID, &pBuff[i*4]);  // get raw data
}

//---------------------------------------------------------------------------------
void SPIm_InqNRRawData(BYTE cID, PBYTE pBuff, PBYTE pSize)
{
	BYTE  reply[4];
	BYTE  i;
	
	SPIm_Inquiry(cID, SPIM_INQ_NR_RAWDATA, &reply[0]);	// send inquiry(get nr raw data)
	*pSize = reply[1];									// rx data size(dword) from slave
	
	for(i=0;i<*pSize;i++)		SPIm_RxDword(cID, &pBuff[i*4]);  // get raw data
}


/*  FILE_END_HERE */
