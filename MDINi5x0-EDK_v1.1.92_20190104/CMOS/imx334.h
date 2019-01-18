//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2017  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  IMX334.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__IMX334_H__
#define		__IMX334_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_IMX334__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
//#define	__USE_ISP_SCLK_OUT__ // NOT using sensor OSC clock

#define	__USE_IMX334_2160P30_MODE__
//#define	__USE_IMX334_2160P60_MODE__		//only 4k60 b/d

#define	__USE_CMOS_IMX334_CSI2_L4__		//12bit 1080p30
//#define	__USE_CMOS_IMX334_CSI2_L8__		// all mode 

//#define	__USE_IMX334_SLAVE_MODE__

#define	__USE_LATTICE_LIAMD6K__     // use lattice crosslink fpga

typedef	enum {
	IMX334_NORMAL=0,
}	IMX334_Mode_t;

typedef	enum {
	IMX334_FPS_30Hz=0,
	IMX334_FPS_60Hz,
}	IMX334_Fps_t;


// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IMX334_Initialize(void);
void IMX334_ChangeFps_Normal(BYTE nID);

IMX334_Fps_t IMX334_GetFrameRate(void);
void IMX334_SetFrameRate(IMX334_Fps_t fps);

#endif	/* __USE_CMOS_IMX334__ */


#endif	/* __IMX334_H__ */

