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

#ifndef		__IMX124_H__
#define		__IMX124_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_IMX124__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

//#define	__USE_CMOS_IMX123__

#define	__USE_CMOS_IMX123_LVDS_S4__		// use sony cmos imx123 lvds s4, imx124 lvds s4 boot
//#define	__USE_CMOS_IMX123_LVDS_S8__		// use sony cmos imx123 lvds s8, imx124 lvds s4 boot


//#define	__USE_IMX124_1536P60_MODE__			// use sony cmos imx123,124 boot 1536p60
#define	__USE_IMX124_1536P30_MODE__			// use sony cmos imx123,124 boot 1536p30
//#define	__USE_IMX124_1080P60_MODE__		// use sony cmos imx123,124 boot 1080p60
//#define	__USE_IMX124_1080P30_MODE__		// use sony cmos imx123,124 boot 1080p30
//if you use 25,50Hz set eSensFPS==SENSOR_25HZ or 50Hz
//#define	__USE_IMX124_FLIP__					// if commit then IMX123,124 no hvflip

//#define	__USE_IMX123_HTM_X32__	//built-in long to short ratio. comment out to set as x32.

// -----------------------------------------------------------------------------
// Sensor QXGA mode booting : 27Mhz OSC use(Check both H/W OSC device and F/W build option)
// Sensor 1080p-HD mode booting : 37.125 OSC use 

// TIP : After that QXGA mode booting with 27MHz OSC, make cropping image what user want resolution..
// -----------------------------------------------------------------------------
#define		__USE_IMX124_OSC27Mhz__					// use 27Mhz Sensor OSC boot, comment use 37.125 OSC boot

//#define __USE_IMX124_HTM_OLD_COMP__  //HTM old type compensation about long saturation

#if defined(__USE_IMX124_1536P60_MODE__)||\
    defined(__USE_IMX124_1536P30_MODE__)
#define __USE_IMX124_RES_QXGA__
#endif

typedef	enum {
	IMX124_NORMAL=0,
	IMX124_WDR,   //Frame by Frame WDR
	IMX124_HTM,   //Built-in WDR
	IMX124_DOL2,  //DOL 2 Frame
	IMX124_DOL3,  //DOL 3 Frame (QXGA prohibited)
}	IMX124_Mode_t;

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IMX124_Initialize(void);
void IMX124_DCKRST_Sequence(void);
void IMX124_ChangeFps_Normal(void);
void IMX124_WDR_Mode(IMX124_Mode_t Mode);

#endif	/* __USE_CMOS_IMX124__ */


#endif	/* __IMX124_H__ */
