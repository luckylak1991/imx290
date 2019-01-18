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

#ifndef		__IMX076_H__
#define		__IMX076_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_IMX076__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

//#define	__USE_CMOS_IMX076_TTL__
//#define	__USE_CMOS_IMX076_LVDS__
//#define	__USE_CMOS_IMX076_ALLPIXEL__
//#define	__USE_CMOS_IMX076_2X2PIXEL__
//#define	__USE_CMOS_IMX076_HOR_1_2__
//#define	__USE_CMOS_IMX076_VER_1_2__


// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IMX076_Initialize(void);

#endif	/* __USE_CMOS_IMX076__ */


#endif	/* __IMX076_H__ */
