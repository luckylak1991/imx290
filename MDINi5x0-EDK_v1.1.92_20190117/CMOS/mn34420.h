//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MN34420.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MN34420_H__
#define		__MN34420_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_MN34420__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
//#define	__USE_ISP_SCLK_OUT__ 

//#define	__USE_CMOS_MN34422__		//option 


//#define	__USE_MN34420_SLAVE_MODE__

//#define	__MN34420_1280x1024_ISPCROP__

typedef	enum {
	MN34420_NORMAL=0,
	MN34420_DOL2,  //DOL 2 Frame
	MN34420_DOL3,  //DOL 3 Frame
}	MN34420_Mode_t;

typedef	enum {
	MN34420_FPS_30Hz=0,      // 2200x1125
	MN34420_FPS_60Hz,        // 2200x1125
	MN34420_FPS_60Hz_DOL2,   // 5280x1250
	MN34420_FPS_120Hz_DOL3,  // 8800x1125
	MN34420_FPS_NUM
}	MN34420_Fps_t;

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void MN34420_Initialize(void);
void MN34420_ChangeFps_Normal(void);
void MN34420_WDR_Mode(MN34420_Mode_t Mode);


#endif	/* __USE_CMOS_MN34420__ */


#endif	/* __MN34420_H__ */
