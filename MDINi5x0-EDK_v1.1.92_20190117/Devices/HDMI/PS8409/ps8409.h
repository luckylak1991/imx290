//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2017  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  PS8409.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__PS8409_H__
#define		__PS8409_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

typedef	struct
{
	LONG 			ctrl;		// control flag
	
	BYTE			did[7];		// device id
	BYTE			tmds_oe;	// tmds output enable flag
	
}	RETIMER_CTRL_PARAM, *PRETIMER_CTRL_PARAM;


// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void RETIMER_Init(void);
void SetRETIMER_TMDSOutEnable(BOOL OnOff);


#endif	/* __PS8409_H__ */
