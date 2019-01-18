//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  IMX274.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__IMX274_H__
#define		__IMX274_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_IMX274__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#define	__USE_ISP_SCLK_OUT__ // NOT using sensor OSC clock

//#define	__USE_IMX274_1080P60_MODE__ //readout mode2 or 4  2x2 binning..
//#define	__USE_IMX274_2160P60_MODE1__
//#define	__USE_IMX274_4K15P_MODE__

//#define	__USE_TRIGGER_MODE__
//#define	__USE_IMX274_FLIP__
#define __USE_FHDCROP_MODE__ // Include 4K sensor 1:1 FHD crop function(ddr memory r/w)

//#define __USE_SENS_PANNING_MODE__ // 4K sensor 1:1 FHD crop function(sensor block control)

#if !defined(__USE_MDIN_i5XX_REV__)	// i5xx org chip
#define	__USE_CMOS_AUX_SYNC_OUT__ // use aux sync out for 4k sensor slave mode with i5xx org chip
#endif

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IMX274_Initialize(void);
DWORD CMOS_GetVmax(void);

#endif	/* __USE_CMOS_IMX274__ */


#endif	/* __IMX274_H__ */
