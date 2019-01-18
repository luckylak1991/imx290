//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  IMX076.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__IMX174_H__
#define		__IMX174_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_IMX174__)  ||  defined(__USE_CMOS_IMX249__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

//#define	__USE_ISP_SCLK_OUT__ // NOT using sensor OSC clock

#define	__USE_IMX174_1080P60_MODE__
//#define	__USE_IMX174_1080P30_MODE__

//#define	__USE_IMX174_FLIP__					// if commit then IMX174 no hvflip
//#define	__USE_TRIGGER_MODE__				// Since mdin-i500 dosn't support trigger shutter control, i500 is operating in the slave mode.


typedef	enum {
	IMX174_NORMAL=0,
	IMX174_WDR,
	IMX174_NUM
}	IMX174_Mode_t;

typedef	enum {
	IMX174_FPS_30Hz=0,
	IMX174_FPS_60Hz,
	IMX174_FPS_NUM
}	IMX174_Fps_t;

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IMX174_Initialize(void);
void IMX174_WDR_Mode(IMX174_Mode_t Mode);
void IMX174_ChangeFps_Normal(BYTE nID);

#endif	/* __USE_CMOS_IMX174__ */


#endif	/* __IMX174_H__ */
