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

#ifndef		__IMX136_H__
#define		__IMX136_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if defined(__USE_CMOS_IMX136__) || defined(__USE_CMOS_IMX236__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#define	__USE_CMOS_IMXx36__

//#define	__USE_CMOS_IMXx36_TTL__
//#define	__USE_CMOS_IMXx36_LVDS_P__
//#define	__USE_CMOS_IMXx36_LVDS_S2__
#define	__USE_CMOS_IMXx36_LVDS_S4__

//#define	__USE_IMXx36_60P_MODE__
#define	__USE_IMXx36_30P_MODE__
//#define	__USE_IMXx36_120P_MODE__

//#define	__USE_CMOS_IMXx36_720pReadOut__

//#define	__USE_CMOS_IMXx36_ALLPIXEL__
//#define	__USE_CMOS_IMXx36_2X2PIXEL__
//#define	__USE_CMOS_IMXx36_HOR_1_2__
//#define	__USE_CMOS_IMXx36_VER_1_2__

#if (defined (__MDIN_i500_EDK__) || defined (__USE_CMOS_IMX136__))
#define	__USE_IMXx36_FLIP__					// if commit then IMX136 no hvflip
#endif


typedef	enum {
	IMX136_NORMAL= 0,
	IMX136_WDR,
	IMX136_HTM,
	IMX136_NUM
}	IMX136_Mode_t;

typedef	enum {
	IMX136_FPS_15Hz= 0,
	IMX136_FPS_30Hz,
	IMX136_FPS_60Hz,
	IMX136_FPS_NUM
}	IMX136_Fps_t;

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IMX136_Initialize(void);
#if 0
void IMX136_Builtin_WDRmode_comp(void);  //not in use (old ver. hdr)
void IMX136_Builtin_WDRmode(void);  //not in use (old ver. hdr)
void IMX136_Nomal_Mode_return(void);  //not in use (old ver. hdr)
#endif
void IMX136_WDR_Mode(IMX136_Mode_t Mode);
void IMX136_ChangeFps_Normal(void);

#endif	/* __USE_CMOS_IMXx36__ */


#endif	/* __IMX136_H__ */
