//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  SPI.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__UCOMSPI_H__
#define		__UCOMSPI_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__MISPTYPE_H__
#include	"misptype.h"
#endif

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
#define		SPI_OK					0
#define		SPI_NOT_FREE			1
#define		SPI_ACK_DUMMY			0x89ABCDEF		// spi-slave return dummy data

#define		SPI_AN41919_ADDR		0x96
#define		SPI_AN41908_ADDR		0x94

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// spi.c
BYTE ISPM SPI_Write(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);
BYTE SPI_Read(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);

#endif	/* __UCOMSPI_H__ */
