//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2017  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  LCMX03LF.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__LCMX03LF_H__
#define		__LCMX03LF_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

typedef	struct
{
	LONG 			ctrl;	// control flag
	
	WORD			h_size;
	WORD			offCH1;
	WORD			offCH2;
	WORD			offCH1_org;
	WORD			offCH2_org;
	WORD			dummy;
	
}	LCMX03LF_CTRL_PARAM, *PLCMX03LF_CTRL_PARAM;


// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void LCMX03LF_Init(void);
void SetLCMX03LF_sw_rst(void);
void SetLCMX03LF_hsize(WORD val);
void SetLCMX03LF_offch1(WORD val);
void SetLCMX03LF_offch2(WORD val);
WORD GetLCMX03LF_offch1(void);
WORD GetLCMX03LF_offch2(void);
WORD GetLCMX03LF_offch1_org(void);
WORD GetLCMX03LF_offch2_org(void);


#endif	/* __LCMX03LF_H__ */
