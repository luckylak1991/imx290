//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2017  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	PS8409.C
// Description 		:	HDMI Repeater with re-timing (PS8409A)
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"

#if defined(__USE_PS8409_RETIMER__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
RETIMER_CTRL_PARAM	stRETIMER;

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//------------------------------------------------------------------------------------------
static void Init_PS8409_PARAM(PRETIMER_CTRL_PARAM pCTRL)
{
	pCTRL->tmds_oe = ON;
}

//--------------------------------------------------------------------------------------------------
static void Init_PS8409_REGISTER(PRETIMER_CTRL_PARAM pCTRL)
{
	// register init
//	PS8409_RegField(0x20, 0, 1, (pCTRL->tmds_oe)?  0:  1);	// 'tmds_oe'
	
	// check device id
//	pCTRL->did[0] = PS8409_RegRead(0x14);
//	pCTRL->did[1] = PS8409_RegRead(0x15);
//	pCTRL->did[2] = PS8409_RegRead(0x16);
//	pCTRL->did[3] = PS8409_RegRead(0x17);
//	pCTRL->did[4] = PS8409_RegRead(0x18);
//	pCTRL->did[5] = PS8409_RegRead(0x19);
//	pCTRL->did[6] = '\0';
	
//	UARTprintf("HDMI Retimer : %s \r\n", &pCTRL->did[0]);
}



//--------------------------------------------------------------------------------------------
void RETIMER_Init(void)
{
	memset(&stRETIMER, 0, sizeof(RETIMER_CTRL_PARAM));
	
	Init_PS8409_PARAM(&stRETIMER);
	Init_PS8409_REGISTER(&stRETIMER);
}

//--------------------------------------------------------------------------------------------
void SetRETIMER_TMDSOutEnable(BOOL OnOff)
{
	stRETIMER.tmds_oe = OnOff;
	PS8409_RegField(0x20, 0, 1, (OnOff)?  0:  1);			// enable 'tmds_oe'
}


#endif	//__USE_PS8409_RETIMER__


/*  FILE_END_HERE */
