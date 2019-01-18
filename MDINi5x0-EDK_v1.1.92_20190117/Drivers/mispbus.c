//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2013  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MISPBUS.C
// Description 		:  Host interface Protocol handling
// Ref. Docment		: 
// Revision History 	:
// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"misp100.h"

#if		defined(MISP_USE_BUS_HIF)

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Global functions
// ----------------------------------------------------------------------

#if 0 //functions for single address memory access can be replaced by inline macro
//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t ISPM MISPBUS_RegWrite(BYTE nID, DWORD rAddr, WORD wData)
{
	switch ((nID)&0xfe) {
		case MISP_HOST_ID:	*((volatile WORD*)(MISP_HOST_BASE_ADDR+(rAddr)*4)) = (wData); break;
		case MISP_LOCAL_ID:	*((volatile WORD*)(MISP_LOCAL_BASE_ADDR+(rAddr)*4)) = (wData); break;
		case MISP_SDRAM_ID:	*((volatile WORD*)(MISP_SDRAM_BASE_ADDR+(rAddr))) = (wData); break;
	}
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t ISPM MISPBUS_RegRead(BYTE nID, DWORD rAddr, PWORD rData)
{
	switch ((nID)&0xfe) {
		case MISP_HOST_ID:	*(rData) = *((volatile WORD*)(MISP_HOST_BASE_ADDR+(rAddr)*4)); break;
		case MISP_LOCAL_ID:	*(rData) = *((volatile WORD*)(MISP_LOCAL_BASE_ADDR+(rAddr)*4)); break;
		case MISP_SDRAM_ID:	*(rData) = *((volatile WORD*)(MISP_SDRAM_BASE_ADDR+(rAddr))); break;
	}
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t ISPM MISPBUS_RegField(BYTE nID, DWORD rAddr, WORD bPos, WORD bCnt, WORD bData)
{
	WORD temp;

	if (bPos>15||bCnt==0||bCnt>16||(bPos+bCnt)>16) return MISP_INVALID_PARAM;
	MISP_RegRead(nID, rAddr, &temp);
	bCnt = ~(0xffff<<bCnt);
	temp &= ~(bCnt<<bPos);
	temp |= ((bData&bCnt)<<bPos);
	MISP_RegWrite(nID, rAddr, temp);
	return MISP_NO_ERROR;
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
// Drive Function for BUS read & BUS write
// You must make functions which is defined below.
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t ISPM MISPBUS_Write(BYTE nID, DWORD rAddr, PBYTE pBuff, DWORD bytes)
{
	DWORD	b_addr, i;
	b_addr=0;
	
	switch(nID&0xfe)
	{
		case MISP_HOST_ID:	b_addr = MISP_HOST_BASE_ADDR + rAddr*4; break;
		case MISP_LOCAL_ID:	b_addr = MISP_LOCAL_BASE_ADDR + rAddr*4; break;
		case MISP_SDRAM_ID:	b_addr = MISP_SDRAM_BASE_ADDR + rAddr; break;
	}
	
	if (nID==MISP_SDRAM_ID) {
		memcpy((void *)(b_addr), &pBuff[0], bytes);			// 8b access
	}
	else {													// 16b access
		for (i=0; i<bytes/2; i++) {
			*((volatile WORD*)(b_addr + i*4)) = ((PWORD)pBuff)[i];
		}
	}
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t ISPM MISPBUS_Read(BYTE nID, DWORD rAddr, PBYTE pBuff, DWORD bytes)
{
	DWORD	b_addr, i;
	b_addr=0;
	
	switch(nID&0xfe)
	{
		case MISP_HOST_ID:	b_addr = MISP_HOST_BASE_ADDR + rAddr*4; break;
		case MISP_LOCAL_ID:	b_addr = MISP_LOCAL_BASE_ADDR + rAddr*4; break;
		case MISP_SDRAM_ID:	b_addr = MISP_SDRAM_BASE_ADDR + rAddr; break;
	}
	
	if (nID==MISP_SDRAM_ID) {
		memcpy(&pBuff[0], (void *)b_addr, bytes);					//8b access
	}
	else {															//16b access
		for (i=0; i<bytes/2; i++) {
			((PWORD)pBuff)[i]  = *((volatile WORD*)(b_addr + i*4));
		}
	}
	return MISP_NO_ERROR;
}
#endif	//#if		defined(MISP_USE_BUS_HIF)
/*  FILE_END _HERE */
