//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	MISPCUR.C
// Description 		:
// Ref. Docment		:
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"misp100.h"

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
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPCUR_CursorColorMode(PCURSOR_CTL_INFO pINFO)
{
	WORD mode, attb;

	switch (pINFO->ctrl&CURSOR_COLORMODE_MASK) {
		case CURSOR_32BIT_4COLOR: mode = 0; attb = 1; break;
		case CURSOR_32BIT_8COLOR: mode = 0; attb = 2; break;
		default:				  mode = 1; attb = 2; break;
	}
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x301, 6, 1, mode));
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x33c, 4, 3, attb));
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPCUR_CursorDisplay(PCURSOR_CTL_INFO pINFO)
{
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x300, 11, 1, MBIT(pINFO->ctrl,CURSOR_DISPLAY_ON)));		// display on/off
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPCUR_CursorPosition(PCURSOR_CTL_INFO pINFO)
{
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x33d, pINFO->x));				// x position
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x33e, pINFO->y));				// y position
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPCUR_CursorRepeat(PCURSOR_CTL_INFO pINFO)
{
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x301,  5, 1, MBIT(pINFO->ctrl,CURSOR_REPEAT_H_ON)));	// H-repeat on/off
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x301,  4, 1, MBIT(pINFO->ctrl,CURSOR_REPEAT_V_ON)));	// V-repeat on/off
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPCUR_CursorAlpha(PCURSOR_CTL_INFO pINFO)
{
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x33c,  7, 5, pINFO->alpha));		// alpha value
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPCUR_CursorBlink(PCURSOR_CTL_INFO pINFO)
{
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x33c, 12, 1, MBIT(pINFO->ctrl,CURSOR_BLINK_ON)));		// blink on/off
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPCUR_CursorBlinkInverse(PCURSOR_CTL_INFO pINFO)
{
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x33c, 13, 1, MBIT(pINFO->ctrl,CURSOR_BLINK_INVERSE)));	// blink inverse
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPCUR_CursorBlinkCycle(PCURSOR_CTL_INFO pINFO)
{
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x342,  9, 7, pINFO->cycle));		// blink cycle
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPCUR_CursorColorKey(PCURSOR_CTL_INFO pINFO)
{
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x357, 10, 1, MBIT(pINFO->ctrl,CURSOR_COLORKEY_ON)));	// colorkey on/off
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPCUR_CursorBMPInfo(PCURSOR_CTL_INFO pINFO)
{
	WORD unit = MISP_GetSizeOfBank()*2;

	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x33f, pINFO->stBMP.w));			// width
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x340, pINFO->stBMP.h));			// height

	pINFO->stBMP.r = ADDR2ROW(pINFO->stBMP.addr, unit);
	pINFO->stBMP.c = ADDR2COL(pINFO->stBMP.addr, unit)/8;		// for 8Byte-unit
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x341, pINFO->stBMP.r));			// row+bank
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x342, 0, 8, pINFO->stBMP.c));	// column
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPCUR_CursorPalette(PCURSOR_CTL_INFO pINFO)
{
	if (pINFO->stPAL.pBuff==NULL||pINFO->stPAL.size==0) return MISP_INVALID_PARAM;
	MISP_CHK_ERR(MISP_MultiWrite(MISP_LOCAL_ID, 0x343, pINFO->stPAL.pBuff, pINFO->stPAL.size));
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPCUR_SetCursorConfig(PCURSOR_CTL_INFO pINFO)
{
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x33c, 14, 1, 1));	// fix blink in vsync

	MISP_CHK_ERR(MISPCUR_CursorColorMode(pINFO));		// set color mode
	MISP_CHK_ERR(MISPCUR_CursorDisplay(pINFO));		// display on/off
	MISP_CHK_ERR(MISPCUR_CursorPosition(pINFO));		// x,y position
	MISP_CHK_ERR(MISPCUR_CursorRepeat(pINFO));			// h,v repeat on/off
	MISP_CHK_ERR(MISPCUR_CursorAlpha(pINFO));			// alpha value
	MISP_CHK_ERR(MISPCUR_CursorBlink(pINFO));			// blink on/off
	MISP_CHK_ERR(MISPCUR_CursorBlinkInverse(pINFO));	// blink inverse on/off
	MISP_CHK_ERR(MISPCUR_CursorBlinkCycle(pINFO));		// set blink cycle
	MISP_CHK_ERR(MISPCUR_CursorColorKey(pINFO));		// colorkey on/off
	MISP_CHK_ERR(MISPCUR_CursorBMPInfo(pINFO));		// set BMP info
	MISP_CHK_ERR(MISPCUR_CursorPalette(pINFO));		// set cursor palette
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPCUR_EnableCursor(PCURSOR_CTL_INFO pINFO, BOOL OnOff)
{
	if (OnOff)	pINFO->ctrl |=  CURSOR_DISPLAY_ON;
	else		pINFO->ctrl &= ~CURSOR_DISPLAY_ON;
	return MISPCUR_CursorDisplay(pINFO);		// display on/off
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPCUR_SetCursorPosition(PCURSOR_CTL_INFO pINFO, WORD x, WORD y)
{
	pINFO->x = x; pINFO->y = y;
	return MISPCUR_CursorPosition(pINFO);		// x,y position
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPCUR_EnableCursorRepeat(PCURSOR_CTL_INFO pINFO, BOOL h_rpt, BOOL v_rpt)
{
	if (h_rpt)	pINFO->ctrl |=  CURSOR_REPEAT_H_ON;
	else		pINFO->ctrl &= ~CURSOR_REPEAT_H_ON;

	if (v_rpt)	pINFO->ctrl |=  CURSOR_REPEAT_V_ON;
	else		pINFO->ctrl &= ~CURSOR_REPEAT_V_ON;

	return MISPCUR_CursorRepeat(pINFO);			// h,v repeat on/off
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPCUR_SetCursorAlpha(PCURSOR_CTL_INFO pINFO, BYTE alpha)
{
	pINFO->alpha = alpha;
	return MISPCUR_CursorAlpha(pINFO);			// alpha value
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPCUR_EnableCursorBlink(PCURSOR_CTL_INFO pINFO, BOOL OnOff)
{
	if (OnOff)	pINFO->ctrl |=  CURSOR_BLINK_ON;
	else		pINFO->ctrl &= ~CURSOR_BLINK_ON;
	return MISPCUR_CursorBlink(pINFO);			// blink on/off
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPCUR_SetCursorBlinkInverse(PCURSOR_CTL_INFO pINFO, BOOL OnOff)
{
	if (OnOff)	pINFO->ctrl |=  CURSOR_BLINK_INVERSE;
	else		pINFO->ctrl &= ~CURSOR_BLINK_INVERSE;
	return MISPCUR_CursorBlinkInverse(pINFO);	// blink inverse on/off
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPCUR_SetCursorBlinkCycle(PCURSOR_CTL_INFO pINFO, BYTE cycle)
{
	pINFO->cycle = cycle;
	return MISPCUR_CursorBlinkCycle(pINFO);		// set blink cycle
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPCUR_EnableCursorColorKey(PCURSOR_CTL_INFO pINFO, BOOL OnOff)
{
	if (OnOff)	pINFO->ctrl |=  CURSOR_COLORKEY_ON;
	else		pINFO->ctrl &= ~CURSOR_COLORKEY_ON;
	return MISPCUR_CursorColorKey(pINFO);		// colorkey on/off
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPCUR_SetCursorBMPInfo(PCURSOR_CTL_INFO pINFO, BYTE w, BYTE h, DWORD addr)
{
	pINFO->stBMP.w		= w;
	pINFO->stBMP.h		= h;
	pINFO->stBMP.addr	= addr;
	return MISPCUR_CursorBMPInfo(pINFO);		// set BMP info
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPCUR_SetCursorPalette(PCURSOR_CTL_INFO pINFO, BYTE size, PBYTE pBuff)
{
	if (pBuff==NULL||size==0) return MISP_INVALID_PARAM;

	pINFO->stPAL.size	= size;
	pINFO->stPAL.pBuff	= pBuff;
	return MISPCUR_CursorPalette(pINFO);		// set cursor palette
}

/*  FILE_END_HERE */
