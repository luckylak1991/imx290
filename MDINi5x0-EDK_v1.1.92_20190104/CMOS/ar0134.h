//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  AR0331.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__AR0134_H__
#define		__AR0134_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_AR0134__) || defined( __USE_CMOS_AR0135__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#define	__USE_CMOS_AR0134_TTL__
//#define	__USE_CMOS_AR0134_LVDS_S2__


#if defined(__USE_CMOS_AR0135__) 
#define	__USE_AR0134_HVFLIP__
#else
#define	__USE_AR0134_VFLIP__	
#endif
// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------



void AR0134_Initialize(void);

#endif	/* __USE_CMOS_AR0134__ */ /* __USE_CMOS_AR0135__ */


#endif	/* __AR0134_H__ */
