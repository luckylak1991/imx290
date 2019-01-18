//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2016  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  IMX253.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__IMX253_H__
#define		__IMX253_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_IMX253__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

//#define	__USE_ISP_SCLK_OUT__ // NOT using sensor OSC clock

#if defined (__USE_ISP_SCLK_OUT__)
#define	__USE_OSC_74_25MHZ__		// set Sensor Master Clock (Disable : 37.125MHz)
#endif

//#defined	__USE_IMX253_FLIP__

//#define	__USE_IMX253_SLAVE_MODE__
//#define	__USE_TRIGGER_MODE__			//TRIG mode run slave mode only


// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IMX253_Initialize(void);
void IMX253_ChangeFps_Normal(void);
void IMX253_SetHiddenRegister(void);
void IMX253_SetROIMode(void);
BOOL IMX253_SensorInformation(void);
DWORD CMOS_GetVmax(void);

#endif	/* __USE_CMOS_IMX253__ */


#endif	/* __IMX253_H__ */
