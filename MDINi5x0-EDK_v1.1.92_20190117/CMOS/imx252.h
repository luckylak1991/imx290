//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2016  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  IMX252.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__IMX252_H__
#define		__IMX252_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_IMX252__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
//#define	__USE_ISP_SCLK_OUT__

//#define	__USE_IMX252_1536P_ALL_SCAN_MODE__
//#define	__USE_IMX252_1080P60_MODE__
#define	__USE_IMX252_1080P30_MODE__

//#defined	__USE_IMX252_FLIP__
//#define	__USE_IMX252_SLAVE_MODE__

#if defined(__USE_IMX252_SLAVE_MODE__)
//#define	__USE_TRIGGER_MODE__
#endif


//#define	__USE_IMX252_LVDS_S8__

//#define	__USE_IMX252_MONO_SENSOR__			//0x349[7]Mono / Color Information = 1 then MONO(GRAY) Sensor

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------
typedef	enum {
	IMX252_FPS_30Hz=0,
	IMX252_FPS_60Hz,
}	IMX252_Fps_t;

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IMX252_Initialize(void);
void IMX252_ChangeFps_Normal(BYTE nID);
BOOL IMX252_SensorInformation(void);

#endif	/* __USE_CMOS_IMX252__ */


#endif	/* __IMX252_H__ */
