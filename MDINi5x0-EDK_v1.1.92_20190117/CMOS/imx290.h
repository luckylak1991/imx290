//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  IMX185.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__IMX290_H__
#define		__IMX290_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
//#define	__USE_ISP_SCLK_OUT__

//#define	__USE_IMX290_1080P120_MODE__		// support only lvds 8ch mode. must enable sensor_bayer 10bit mode option & not support : imx327 
//#define	__USE_IMX290_1080P60_MODE__
#define	__USE_IMX290_1080P30_MODE__
//#define	__USE_IMX290_720P120_MODE__			//  OSC : support lvds 10bit 4ch. must enable sensor_bayer 10bit mode option.      ISP_SCLK_OUT(USER CLOCK OUT USE) : 8ch lvds & 1080p120-->ISP crop.
//#define	__USE_IMX290_720P60_MODE__
//#define	__USE_IMX290_720P30_MODE__

//#define	__USE_IMX290_FLIP__					// if commit then IMX290 no hvflip
//#define	__USE_CMOS_IMX290_LVDS_S2__
#define	__USE_CMOS_IMX290_LVDS_S4__
//#define	__USE_CMOS_IMX290_LVDS_S8__		// not support : imx327 

#define	__USE_IMX290_HCG_MODE__


typedef	enum {
	IMX290_NORMAL=0,
	IMX290_WDR,   //Frame by Frame WDR
	IMX290_DOL2,  //DOL 2 Frame
	IMX290_DOL3,  //DOL 3 Frame
}	IMX290_Mode_t;

typedef	enum {
	IMX290_FPS_30Hz=0,
	IMX290_FPS_60Hz,
	IMX290_FPS_120Hz,	
	IMX290_FPS_30Hz_DOL2,
	IMX290_FPS_30Hz_DOL3,
	IMX290_FPS_60Hz_DOL2,
	IMX290_FPS_60Hz_DOL3,
	IMX290_FPS_120Hz_DOL2,
	IMX290_FPS_120Hz_DOL3,
	IMX290_FPS_NUM
}	IMX290_Fps_t;


// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IMX290_Initialize(void);
void IMX290_ChangeFps_Normal(BYTE nID);
void IMX290_WDR_Mode(IMX290_Mode_t Mode);

IMX290_Fps_t IMX290_GetFrameRate(void);
void IMX290_SetFrameRate(IMX290_Fps_t fps);

#endif	/* __USE_CMOS_IMX290__ */


#endif	/* __IMX290_H__ */
