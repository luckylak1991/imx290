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

#ifndef		__AR0331_H__
#define		__AR0331_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if	defined(__USE_CMOS_AR0331__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

//#define	__USE_CMOS_AR0331_TTL__
//#define	__USE_CMOS_AR0331_LVDS_S4__
#define	__USE_CMOS_AR0331_LVDS_S2__
//#define	__USE_CMOS_AR0331_LVDS_S1__

#if defined (__USE_CMOS_AR0331_LVDS_S4__)
//#define	__USE_CMOS_AR0331_LVDS_S4_60P__
#define	__USE_CMOS_AR0331_LVDS_S4_30P__
#endif

#define	__USE_AR0331_NORMAL_MODE__
//#define	__USE_AR0331_NORMAL_3MegaMODE__
//#define	__USE_AR0331_NORMAL_ALTM_MODE__
//#define	__USE_AR0331_HDR_ALTM_MODE__


// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void PLL_settings_LVDS(BOOL mode);
void HDR_Mode(WORD nID);
void Motion_2D_Compensation (BOOL mode);
void ALTM_Mode (BOOL mode);
void ADACD_Mode (BOOL mode);
void Companding_Mode (BOOL mode);
void Embedded_Data_n_Stats_Mode (BOOL mode);
void AR0331_Analog_Settings(void);
void AR0331_Linear_Sequencer_Load(void);
void AR0331_HDR_Sequencer_Load(void);

void AR0331_Parallel_Linear_Nomal_1080p30(void);
void AR0331_Parallel_HDR_ALTM_1080p30(void);
void AR0331_Parallel_Linear_ALTM_1080p30(void);
void AR0331_Parallel_HDR_COMP_1080p30(void);


void AR0331_Initialize(void);
void AR0331_WDR_Initialize(void);

#endif	/* __USE_CMOS_AR0331__ */


#endif	/* __AR0331_H__ */
