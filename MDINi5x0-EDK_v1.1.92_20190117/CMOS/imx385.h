//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2017  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  IMX385.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__IMX385_H__
#define		__IMX385_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_IMX385__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

//#define	__USE_IMX385_1080P120_MODE__		// support only lvds 8ch mode. must enable __USE_SENSOR_BAYER_10BIT__ mode option 
// #define	__USE_IMX385_1080P60_MODE__
#define	__USE_IMX385_1080P30_MODE__
//#define	__USE_IMX385_720P60_MODE__
//#define	__USE_IMX385_720P30_MODE__

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define	__USE_CMOS_IMX385_LVDS_S2__ // Not working
#define	__USE_CMOS_IMX385_LVDS_S4__
//#define	__USE_CMOS_IMX385_LVDS_S8__ // Not working

//#define	__USE_CMOS_IMX385_CSI2_L2__		//  Not tested
//#define	__USE_CMOS_IMX385_CSI2_L4__		// 1080P30/1080P60 working
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(__USE_CMOS_IMX385_CSI2_L2__)||defined(__USE_CMOS_IMX385_CSI2_L4__)
//#define	__USE_CMOS_IMX385_CSI2TTL__		        // For Crosslink CSI2TTL
#define	__USE_CMOS_IMX385_CSI2LVDS6CH__		// For Crosslink CSI2LVDS6CH
#endif

#define	__USE_IMX385_HCG_MODE__

#if defined(__USE_CMOS_IMX385_LVDS_S2__) || defined(__USE_CMOS_IMX385_LVDS_S4__) || defined(__USE_CMOS_IMX385_LVDS_S8__)
#define	__USE_IMX385_LVDS_MODE__		// sensor initialize lvds
#endif

#if defined(__USE_CMOS_IMX385_CSI2_L2__) || defined(__USE_CMOS_IMX385_CSI2_L4__)
#define	__USE_IMX385_CSI2_MODE__			// sensor initialize mipi
#endif

typedef	enum {
	IMX385_NORMAL=0,
	IMX385_WDR,   //Frame by Frame WDR
	IMX385_DOL2,  //DOL 2 Frame
	IMX385_DOL3,  //DOL 3 Frame
}	IMX385_Mode_t;

typedef	enum {
	IMX385_FPS_30Hz=0,
	IMX385_FPS_60Hz,
	IMX385_FPS_120Hz,	
	IMX385_FPS_30Hz_DOL2,
	IMX385_FPS_30Hz_DOL3,
	IMX385_FPS_60Hz_DOL2,
	IMX385_FPS_60Hz_DOL3,
	IMX385_FPS_120Hz_DOL2,
	IMX385_FPS_120Hz_DOL3,
	IMX385_FPS_NUM
}	IMX385_Fps_t;


// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IMX385_Initialize(void);
void IMX385_ChangeFps_Normal(BYTE nID);
void IMX385_WDR_Mode(IMX385_Mode_t Mode);

IMX385_Fps_t IMX385_GetFrameRate(void);
void IMX385_SetFrameRate(IMX385_Fps_t fps);

#endif	/* __USE_CMOS_IMX385__ */


#endif	/* __IMX385_H__ */

