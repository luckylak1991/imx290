//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2010  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	DELAY.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"common.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
VWORD  usDelay, msDelay;
VDWORD usValue, msValue;

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
void Delay_mSec(WORD delay)
{
	*R_TM4CNT = msValue;								// set period to 1ms
	msDelay = delay;	*R_TM4CTRL = (1<<7 | F_TMCTRL_PFSEL_8 | F_TMCTRL_TMEN_EN);
	while (msDelay);	*R_TM4CTRL = (1<<7 | F_TMCTRL_PFSEL_8 | F_TMCTRL_TMEN_DIS);
}

/*  FILE_END_HERE */
