//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  IMX326.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__IMX326_H__
#define		__IMX326_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_IMX326__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#define	__USE_ISP_SCLK_OUT__ // NOT using sensor OSC clock

//if no use define mode0 3072x2160 
//#define	__USE_IMX326_2592x1944_MODE2__
//#define	__USE_IMX326_3072x1728_MODE4__
//#define	__USE_IMX326_2160x2160_MODE6__

//#define	__USE_TRIGGER_MODE__
//#define	__USE_IMX326_FLIP__


#if !defined(__USE_MDIN_i5XX_REV__)	// i5xx org chip
#define	__USE_CMOS_AUX_SYNC_OUT__ // use aux sync out for 4k sensor slave mode with i5xx org chip
#endif

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IMX326_Initialize(void);

#endif	/* __USE_CMOS_IMX326__ */


#endif	/* __IMX326_H__ */
