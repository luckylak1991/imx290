//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  IMX178.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__IMX178_H__
#define		__IMX178_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_IMX178__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

//#define	__USE_CMOS_IMX178_LVDS_S8__
#define	__USE_CMOS_IMX178_LVDS_S4__

//#define	__USE_IMX178_6M29_MODE__		// 6.3M 3072x2048 29.97fps
//#define	__USE_IMX178_5M29_MODE__		// 5M 4:3 2592x1944 29.97fps
//#define	__USE_IMX178_1728P30_MODE__		//3072x1728p30(5m 16:9) Window crop mode(5.3M) same lvds data rate 1080p60
//#define	__USE_IMX178_1080P60_MODE__			//
//#define	__USE_IMX178_1728P60_MODE__		//3072x1728p60(5m 16:9) upper lvds 8ch only
#define	__USE_IMX178_1080P60_MODE__
//#define	__USE_IMX178_1080P30_MODE__

//#define	__USE_IMX178_FLIP__					// if commit then IMX174 no hvflip


#define		__USE_IMX178_OSC27Mhz__					// use 27Mhz Sensor OSC boot, comment use 37.125 OSC boot

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IMX178_Initialize(void);
void IMX178_ChangeFps_Normal(void);

#endif	/* __USE_CMOS_IMX178__ */


#endif	/* __IMX178_H__ */
