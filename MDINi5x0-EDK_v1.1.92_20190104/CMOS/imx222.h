//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2015  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  IMX222.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__IMX222_H__
#define		__IMX222_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if defined(__USE_CMOS_IMX222__) || defined(__USE_CMOS_IMX322__) || defined(__USE_CMOS_IMX323__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#define	__USE_CMOS_IMXx22__

//#define	__USE_IMXx22_720P30_MODE__
//#define	__USE_IMXx22_1080P15_MODE__


// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IMX222_Initialize(void);
void IMX222_ChangeFps_Normal(void);

#endif	/* __USE_CMOS_IMXx22__ */


#endif	/* __IMX222_H__ */
