//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2016  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  IMX265.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__IMX265_H__
#define		__IMX265_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_IMX265__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
#define	__USE_ISP_SCLK_OUT__

//#define	__USE_IMX265_1536P_ALL_SCAN_MODE__
#define	__USE_IMX265_1080P60_MODE__
//#define	__USE_IMX265_1080P30_MODE__
//#define	__USE_IMX265_ROI_MODE__

//#defined	__USE_IMX265_FLIP__
//#define	__USE_IMX265_SLAVE_MODE__


//---when __USE_IMX265_SLAVE_MODE__     --> sequencial trig-----------------------//
//---when not __USE_IMX265_SLAVE_MODE__ --> fast trigger mode(master mode only)---//
//#define	__USE_TRIGGER_MODE__

//#define	__USE_IMX265_MONO_SENSOR__			//0x349[7]Mono / Color Information = 1 then MONO(GRAY) Sensor

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------
typedef	enum {
	IMX265_FPS_30Hz=0,
	IMX265_FPS_60Hz,
}	IMX265_Fps_t;

typedef	enum {
	IMX265_NORMAL=0,
	IMX265_WDR   //Frame by Frame WDR
}	IMX265_Mode_t;

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IMX265_Initialize(void);
void IMX265_ChangeFps_Normal(BYTE nID);
void IMX265_ChangeTriggerMode(WORD nID);
BOOL IMX265_SensorInformation(void);
DWORD CMOS_GetVmax(void);
void IMX265_WDR_Mode(IMX265_Mode_t Mode);

#endif	/* __USE_CMOS_IMX265__ */


#endif	/* __IMX265_H__ */
