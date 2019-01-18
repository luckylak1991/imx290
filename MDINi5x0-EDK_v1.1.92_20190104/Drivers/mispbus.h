//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2013  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MISPBUS.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISPBUS_H__
#define		__MISPBUS_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
//#define		MISP100_BASE_ADDR		((VPWORD)0x70000000)	// cpu dependent
#define		MISP100_BASE_ADDR		0x70000000	// cpu dependent
#define		MISP_HOST_BASE_ADDR		(MISP100_BASE_ADDR | 0x2000000)
#define		MISP_LOCAL_BASE_ADDR	(MISP100_BASE_ADDR | 0x0000000)

#if defined(__USE_MDIN_i500__)		// MDIN-i500
#define		MISP_SDRAM_BASE_ADDR	(MISP100_BASE_ADDR | 0x4000000)		// addr bit(26bit, 0~512Mbit)
#else   							// MDIN-i51X (i510, i540, i550)
#define		MISP_SDRAM_BASE_ADDR	(MISP100_BASE_ADDR | 0x8000000)		// addr bit(27bit, 0~1Gbit)
#endif

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

MISP_ERROR_t ISPM MISPBUS_Write(BYTE nID, DWORD rAddr, PBYTE pBuff, DWORD bytes);
MISP_ERROR_t ISPM MISPBUS_RegWrite(BYTE nID, DWORD rAddr, WORD wData);
MISP_ERROR_t ISPM MISPBUS_Read(BYTE nID, DWORD rAddr, PBYTE pBuff, DWORD bytes);
MISP_ERROR_t ISPM MISPBUS_RegRead(BYTE nID, DWORD rAddr, PWORD rData);
MISP_ERROR_t ISPM MISPBUS_RegField(BYTE nID, DWORD rAddr, WORD bPos, WORD bCnt, WORD bData);

#endif	/* __MISPTYPE_H__ */
