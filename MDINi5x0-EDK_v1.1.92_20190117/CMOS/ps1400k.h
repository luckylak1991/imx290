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

#ifndef		__PS1400K_H__
#define		__PS1400K_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_PS1400K__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------
//#define __USE_FHDCROP_MODE__ // Include 4K sensor 1:1 FHD crop function


//#define	__USE_PS1400K_I2C_PORT0__		//comment use port1 MISP_SCL/SDA
// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void PS1400K_Initialize(void);

#endif	/* ____USE_CMOS_PS1400K____ */


#endif	/* ____PS1400K____ */
