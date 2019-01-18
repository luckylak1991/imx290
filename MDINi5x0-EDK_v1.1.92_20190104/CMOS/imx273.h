//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2016  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  IMX273.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__IMX273_H__
#define		__IMX273_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_IMX273__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

//#define	__USE_ISP_SCLK_OUT__ // NOT using sensor OSC clock

//#define	__OUT_VCLKPLL_LOCK__	//only use under sclk_out && slave mode options.

#if defined (__USE_ISP_SCLK_OUT__)
//#define	__USE_OSC_74_25MHZ__		// set Sensor Master Clock (Disable : 37.125MHz with sensor osc)
#endif

//#define	__USE_IMX273_480P60_MODE__			//use lvds s2

//#define	__USE_IMX273_720P30_MODE__		
//#define	__USE_IMX273_720P120_MODE__			// recommended Lvds s8 mode 
//#define	__USE_IMX273_720P240_MODE__		// only lvds 8ch, not support all scan mode, recommend to use sclk_out, not tunning yet.


//#define	__USE_IMX273_ALLSCAN_MODE__		// lvds s4 : max 60fps, lvds s8 max : 120fps
//#define	__USE_IMX273_ROI_MODE__		// lvds s4 : max 60fps, lvds s8 max : 120fps, OB image problem

//#define	__USE_CMOS_IMX273_LVDS_S2__		// all disable is lvds s4
//#define	__USE_CMOS_IMX273_LVDS_S8__		// all disable is lvds s4


//#defined	__USE_IMX273_FLIP__

//#define	__USE_IMX273_SLAVE_MODE__
//#define	__USE_TRIGGER_MODE__			//TRIG mode run slave mode only

typedef	enum {
	IMX273_NORMAL=0,
	IMX273_WDR   //Frame by Frame WDR
}	IMX273_Mode_t;

typedef	enum {
	IMX273_FPS_30Hz=0,
	IMX273_FPS_60Hz,
	IMX273_FPS_120Hz,	
	IMX273_FPS_240Hz,		
	IMX273_FPS_NUM
}	IMX273_Fps_t;

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IMX273_Initialize(void);
void IMX273_WDR_Mode(IMX273_Mode_t Mode);
void IMX273_ChangeFps_Normal(BYTE nID);
void IMX273_SetHiddenRegister(void);
void IMX273_SetROIMode(void);
void IMX273_SetAdBits(BYTE bit12);
BOOL IMX273_SensorInformation(void);
DWORD CMOS_GetVmax(void);
void IMX273_ChangeTriggerMode(WORD nID);

#endif	/* __USE_CMOS_IMX273__ */


#endif	/* __IMX273_H__ */
