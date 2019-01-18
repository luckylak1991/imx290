//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  IRIS_IF.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__IRIS_IF_H__
#define		__IRIS_IF_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__MISPTYPE_H__
#include	"misptype.h"
#endif

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IRISDRV_Initialize(void);

BYTE IRIS_MultiWrite(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);
BYTE IRIS_WordWrite(BYTE sAddr, WORD rAddr, WORD wData);
BYTE IRIS_MultiRead(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);
WORD IRIS_WordRead(BYTE sAddr, WORD rAddr);
BYTE IRIS_WordField(BYTE sAddr, WORD rAddr, WORD bPos, WORD bCnt, WORD bData);


#endif	/* __IRIS_IF_H__ */
