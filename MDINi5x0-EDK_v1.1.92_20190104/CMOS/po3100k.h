//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  PO3100K.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__PO3100K_H__
#define		__PO3100K_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_PO3100K__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
//#define	__USE_PO3100K_60FPS__		// sensor 720p60fps, out 720p60fps
//#define	__USE_PO3100K_60to1080p30__ // sensor 720p60fps, out 1080p30fps
#define	__USE_PO3100K_30to1080p30__	// sensor 720p30fps, out 1080p30fps

#if defined (__USE_PO3100K_60to1080p30__) || defined (__USE_PO3100K_30to1080p30__)
#define	__USE_PO3100K_1080pScaleUp__	// sensor 720p30fps, out 1080p30fps
#endif

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void PO3100K_Initialize(void);

#endif	/* ____USE_CMOS_PO3100K_H____ */


#endif	/* __PO3100K_H__ */
