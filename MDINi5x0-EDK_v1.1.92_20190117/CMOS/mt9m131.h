//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MT9M131.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MT9M131_H__
#define		__MT9M131_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_MT9M131__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void MT9M131_Initialize(void);

#endif	/* __USE_CMOS_MT9M131__ */


#endif	/* __MT9M131_H__ */
