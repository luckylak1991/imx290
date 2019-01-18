//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2017  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  CDCE913.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__CDCE913_H__
#define		__CDCE913_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
typedef	struct
{
	WORD	n;			// Divide value of PLL (range : 0~4095)
	WORD	m;			// Multiplier value of PLL (range : 1~511)
	BYTE	pdiv;		// Pre Divide value of PLL (range : 1~127)
	WORD	r;			// N' - M * Q  (range : 0~511),  N' = N x 2^P
	BYTE	q;			// int(N'/M)   (range : 16~63)
	BYTE	p;			// 4 - int(log2 N/M)   (range : 0~7)
	BYTE	rng;		// fVCO range selection(0:<125M, 1:<150M, 2:<175M, 3:>175M)
	WORD	dummy;

}	CDCE913_PLL, *PCDCE913_PLL;

typedef	struct
{
	LONG 			ctrl;	// control flag
	
	PCDCE913_PLL	pPll;	// pll parameter
	
}	EXTPLL_CTRL_PARAM, *PEXTPLL_CTRL_PARAM;

// external pll ic control parameter
#define 	EXTPLLCTRL_MODE_MASK			0x00000003		// day night mode mask

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void EXTPLL_Init(void);
void SetEXTPLL_Frmt(BYTE frmt);
void SetEXTPLL_Y1OnOff(BOOL OnOff);



#endif	/* __CDCE913_H__ */
