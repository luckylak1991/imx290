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

#ifndef		__IMX185_H__
#define		__IMX185_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_IMX185__)

//#define EN774_HIDDEN_REGISTER_EN
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#define	__USE_IMX185_1080P60_MODE__
//#define	__USE_IMX185_1080P30_MODE__

//#define	__USE_IMX185_FLIP__					// if commit then IMX185 no hvflip
//#define	__USE_40PIN_HEADER__				// if user use 40pin header on imx185 sensor pcb b/d


typedef	enum {
	IMX185_NORMAL=0,
	IMX185_WDR   //Frame by Frame WDR
}	IMX185_Mode_t;

typedef	enum {
	IMX185_FPS_30Hz=0,
	IMX185_FPS_60Hz,
	IMX185_FPS_NUM
}	IMX185_Fps_t;

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IMX185_Initialize(void);
void IMX185_WDR_Mode(IMX185_Mode_t Mode);
void IMX185_ChangeFps_Normal(void);
#endif	/* __USE_CMOS_IMX185__ */


#endif	/* __IMX185_H__ */
