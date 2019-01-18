//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2015  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  IMX226.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__IMX226_H__
#define		__IMX226_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_IMX226__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#define	__USE_ISP_SCLK_OUT__ // NOT using sensor OSC clock

//#define __USE_IMX226_1080P60_MODE__ 		//(sensor 2x2 binning mode5, 10bit adbit, format 12bit)
#define	__USE_IMX226_2160P30_MODE0__ // 3840x2160 		:	APGC fixed(mode 0,2,3)
//#define	__USE_IMX226_2160P30_MODE1__ // 4096x2160	:	APGC moving(mode 1,4,5), see datasheet 37p
//#define	__USE_IMX226_2160P60_MODE4__ //4096x2160 59.94fps 10bit	:	APGC moving(mode 1,4,5), see datasheet 37p
//#define __USE_IMX226_4K3KP30_MODE0__		// 4000x3000 8ch test

//#define	__USE_CMOS_IMX226_LVDS_S4__		// use lvds s4 15fps mode


#if defined( __USE_IMX226_2160P30_MODE0__)||defined(__USE_IMX226_2160P30_MODE1__)
//if not define then 4k30p mode. use user_clk_out(=sclk_out) mode
//#define	__USE_IMX226_4K15P_MODE__		//__USE_TVI_TX__ && sensor 15fps with 8M15p out...
//#define	__USE_IMX226_4K20P_MODE__
//#define	__USE_IMX226_4K25P_MODE__

//#define	__USE_IMX226_CLK_CONV__
#endif 

//#define	__USE_IMX226_CLK_MATCH_SCLK_		//aux clock make 1/4, and sync count make 1/4 size (ex : 288Mhz/4 --> 72Mhz)

//#define	__USE_CMOS_IMX172__			// share 226 f/w, but nr and lowlight etc fuction need to be tune..., 172 both GSVR and Rolling shutter
#define __USE_FHDCROP_MODE__ // Include 4K sensor 1:1 FHD crop function

#if !defined(__USE_MDIN_i5XX_REV__)	// i5xx org chip
#define	__USE_CMOS_AUX_SYNC_OUT__ // use aux sync out for 4k sensor slave mode with i5xx org chip
#endif

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IMX226_Initialize(void);

#endif	/* __USE_CMOS_IMX226__ */

#endif	/* __IMX226_H__ */
