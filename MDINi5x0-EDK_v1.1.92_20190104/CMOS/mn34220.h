//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MN34220.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MN34220_H__
#define		__MN34220_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_MN34220__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#if defined(__PANORAMA_SENSOR_SETUP__)
#define	__USE_ISP_SCLK_OUT__ // NOT using sensor OSC clock
#endif

typedef	enum {
	MN34220_NORMAL=0,
	MN34220_WDR,   //Frame by Frame WDR
	MN34220_DOL2,  //DOL 2 Frame
	MN34220_DOL3,  //DOL 3 Frame
}	MN34220_Mode_t;

typedef	enum {
	MN34220_FPS_60Hz=0,     //Normal (60Hz)
	MN34220_FPS_50Hz,       //Normal (50Hz)
	MN34220_FPS_30Hz,       //Normal (30Hz)
	MN34220_FPS_25Hz,       //Normal (25Hz)
	MN34220_FPS_60Hz_DOL2,  //DOL2 (60->30Hz)
	MN34220_FPS_50Hz_DOL2,  //DOL2 (50->25Hz)
	MN34220_FPS_90Hz_DOL3,  //DOL3 (90->30Hz)
	MN34220_FPS_75Hz_DOL3,  //DOL3 (75->25Hz)
	MN34220_FPS_120Hz_DOL3, //DOL3 (120->30Hz)
	MN34220_FPS_100Hz_DOL3, //DOL3 (100->25Hz)
	MN34220_FPS_NUM
}	MN34220_Fps_t;

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void MN34220_Initialize(void);
void MN34220_ChangeFps_Normal(void);
void MN34220_WDR_Mode(MN34220_Mode_t mode);


#endif	/* __USE_CMOS_MN34041__ */


#endif	/* __MN34220_H__ */
