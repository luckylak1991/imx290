//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  LENS_IF.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__LENS_IF_H__
#define		__LENS_IF_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__MISPTYPE_H__
#include	"misptype.h"
#endif

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
#if defined(__USE_YT3010_2MOTOR__)
#define	zeroZoom				14000
#define	zeroFocus				20000

#define	limitDigitalZoom			14000+2275+150

#define	marginZoomO			0
#define	marginFocusO			0

#define	limitWideNearZoom		14000-150
#define	limitWideNearFocus		20000-(150)*1
#define	limitTeleFarZoom			14000+2275+150
#define	limitTeleFarFocus		20000+(2935+150)*1

#define	marginZoom				0
#define	marginFocus				0

#define	diffMinMaxZoom			2275
#define	diffWideTeleFocus		(2935)*8

#define	offsetWideLimit			2275
#define	offsetTeleLimit			0

#define zoomStepRatio0			2
#define zoomStepRatio1			8

#elif defined(__USE_X10X_UNION__)
#define	zeroZoom				14000
#define	zeroFocus				20000

#define	limitDigitalZoom			14735 // 14000 + 721 + 15

#define	marginZoomO			0
#define	marginFocusO			0

#define	limitWideNearZoom		12900 // 14000 - 1000 - 20 - a(80)
#define	limitWideNearFocus		18000 // 20000 - 360*4 - c(560(140*4)) // 360 = 329 + 31
#define	limitTeleFarZoom			14900 // 14000 + 721 + 15 + b(164)
#define	limitTeleFarFocus		21500 // 20000 + 230*4 + d(580(145*4)) // 230 = 135 + 95

#define	orgPintZoom				13000 // 14000 - 1000
#define	orgPintFocus				20540 // 20000 + 135*4

#define	marginZoom				5
#define	marginFocus				20

#define	diffWideTeleFocus		340 // 85*4
#define	diffMinMaxZoom			1721 // 1000 + 721(351+370)
#define	diffPintKneeZoom		1351 // 1000 + 351
#define	diffPintKneeFocus		1582 //396*4

#define	offsetWideLimit			366
#define	offsetTeleLimit			1351

#define zoomStepRatio0			1
#define zoomStepRatio1			4

#elif defined(__USE_X10X_ZMAX__)
#define	zeroZoom				14000
#define	zeroFocus				20000

#define	limitDigitalZoom			14590 // 14000 + 567 + 23

#define	marginZoomO			0
#define	marginFocusO			20

#define	limitWideNearZoom		13200 // 14000 - 693 - 22 - a(85)
#define	limitWideNearFocus		17200 // 20000 - 527*4 - c(692(173*4)) // 527 = 222 + 305
#define	limitTeleFarZoom			14700 // 14000 + 567 + 23 + b(110)
#define	limitTeleFarFocus		24400 // 20000 + 928*4 + d(688(172*4)) // 928 = 692 + 236

#define	orgPintZoom				13307 // 13000 //---- 13307 // 14000 - 693  // mimitest
#define	orgPintFocus				21228 // 20540 //---- 21508 // 20000 + 377*4 // (21228 for imx226 / 21388 for imx274) // mimitest

#define	marginZoom				20
#define	marginFocus				80

#define	diffWideTeleFocus		1260 //315 * 4
#define	diffMinMaxZoom			1260 // 693 + 567(181+386)
#define	diffPintKneeZoom		874  // 693 + 181 //search max value in the Inf table
#define	diffPintKneeFocus		2068 // 2038 //---- //2068 // (377+222-82)*4  //search max value in the Inf table // mimitest

#define	offsetWideLimit			204 // 304 // ---- 204 = 181 + 23// mimitest
#define	offsetTeleLimit			874 // 1002 //---- 874 // mimitest

#define zoomStepRatio0			2
#define zoomStepRatio1			8

#elif defined(__USE_X12X_UNION__)
#define	zeroZoom				14000
#define	zeroFocus				20000

#define	limitDigitalZoom			14590 // 14000 + 567 + 23

#define	marginZoomO			0
#define	marginFocusO			20

#define	limitWideNearZoom		13200 // 14000 - 693 - 22 - a(85)
#define	limitWideNearFocus		17200 // 20000 - 527*4 - c(692(173*4)) // 527 = 222 + 305
#define	limitTeleFarZoom			14700 // 14000 + 567 + 23 + b(110)
#define	limitTeleFarFocus		24400 // 20000 + 928*4 + d(688(172*4)) // 928 = 692 + 236

#define	orgPintZoom				13307 // 13000 //---- 13307 // 14000 - 693  // mimitest
#define	orgPintFocus				21228 // 20540 //---- 21508 // 20000 + 377*4 // (21228 for imx226 / 21388 for imx274) // mimitest

#define	marginZoom				20
#define	marginFocus				80

#define	diffWideTeleFocus		1260 //315 * 4
#define	diffMinMaxZoom			1260 // 693 + 567(181+386)
#define	diffPintKneeZoom		874  // 693 + 181 //search max value in the Inf table
#define	diffPintKneeFocus		2068 // 2038 //---- //2068 // (377+222-82)*4  //search max value in the Inf table // mimitest

#define	offsetWideLimit			204 // 304 // ---- 204 = 181 + 23// mimitest
#define	offsetTeleLimit			874 // 1002 //---- 874 // mimitest

#define zoomStepRatio0			2
#define zoomStepRatio1			8

#elif defined(__USE_X03X_ZMAX__)
#define	zeroZoom				14000
#define	zeroFocus				20000

#define	limitDigitalZoom			14590 // 14000 + 567 + 23

#define	marginZoomO			0
#define	marginFocusO			20

#define	limitWideNearZoom		13200 // 14000 - 693 - 22 - a(85)
#define	limitWideNearFocus		17200 // 20000 - 527*4 - c(692(173*4)) // 527 = 222 + 305
#define	limitTeleFarZoom			14700 // 14000 + 567 + 23 + b(110)
#define	limitTeleFarFocus		24400 // 20000 + 928*4 + d(688(172*4)) // 928 = 692 + 236

#define	orgPintZoom				13307 // 13000 //---- 13307 // 14000 - 693  // mimitest
#define	orgPintFocus				21228 // 20540 //---- 21508 // 20000 + 377*4 // (21228 for imx226 / 21388 for imx274) // mimitest

#define	marginZoom				20
#define	marginFocus				80

#define	diffWideTeleFocus		1260 //315 * 4
#define	diffMinMaxZoom			1260 // 693 + 567(181+386)
#define	diffPintKneeZoom		874  // 693 + 181 //search max value in the Inf table
#define	diffPintKneeFocus		2068 // 2038 //---- //2068 // (377+222-82)*4  //search max value in the Inf table // mimitest

#define	offsetWideLimit			204 // 304 // ---- 204 = 181 + 23// mimitest
#define	offsetTeleLimit			874 // 1002 //---- 874 // mimitest

#define zoomStepRatio0			2
#define zoomStepRatio1			8

#elif defined(__USE_X30X_TAMRON__)
#define	zeroZoom				14000
#define	zeroFocus				24000

#define	limitWideNearZoom		11800
#define	limitWideNearFocus		18880
#define	limitTeleFarZoom		15000
#define	limitTeleFarFocus		27840

#define	orgPintZoom				12065
#define	orgPintFocus			24464

#define	marginZoom				20
#define	marginFocus				100

#define	diffWideTeleFocus		2106
#define	diffMinMaxZoom			2675

#define	diffPintKneeZoom			2147
#define	diffPintKneeFocus			3777

#endif

#define	x4M						0
#define	x1M						1


typedef	struct {
	SHORT	mMin;			// motor mechanical minimum position (absolute value)
	SHORT	mMax;			// motor mechanical maximum position (absolute value)
	SHORT	oMin;			// motor optical minimum position (absolute value)
	SHORT	oMax;			// motor optical maximum position (absolute value)
	SHORT	pint;			// motor pint(punt) position (absolute value)
	SHORT	knee;			// motor knee-curve position (absolute value)

}	LENS_PINT_INFO, *PLENS_PINT_INFO;

#define		zWIDE					1
#define		zTELE					0
#define		fNEAR					1
#define		fFAR					0
#define		fPINT					1
#define		zPINT					0

#define		SPI_AN41908_ADDR		0x94

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
// lens_if.c
void LENS_SetInstancePARAM(void);

BYTE ISPM LENS_MultiWrite(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);
BYTE ISPM LENS_WordWrite(BYTE sAddr, WORD rAddr, WORD wData);
BYTE LENS_MultiRead(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);
WORD LENS_WordRead(BYTE sAddr, WORD rAddr);
BYTE LENS_WordField(BYTE sAddr, WORD rAddr, WORD bPos, WORD bCnt, WORD bData);

// lens_xxx.c
void LENS_ZeroPosition(void);
void LENS_ResetPosition(void);
void LENS_OriginPosition(void);
void ISPM LENS_EnableMotorZOOM(BOOL OnOff);
void ISPM LENS_EnableMotorFOKS(BOOL OnOff);
void LENS_EnableTrackZOOM(BOOL OnOff);
void LENS_EnableSpeedZOOM(BOOL OnOff);
void LENS_EnableSpeedFOKS(BOOL OnOff);
void ISPM LENS_ForceBreakZOOM(BOOL OnOff);
void ISPM LENS_ForceBreakFOKS(BOOL OnOff);
void LENS_SetDirectionZOOM(BOOL dir);
void LENS_SetDirectionFOKS(BOOL dir);
void LENS_SetDriveZOOM(SHORT val);
void ISPM LENS_SetDriveFOKS(SHORT val);
BOOL ISPM LENS_IsDriveEnableZOOM(void);
BOOL ISPM LENS_IsDriveEnableFOKS(void);
BOOL ISPM LENS_IsDriveCompleteZOOM(void);
BOOL ISPM LENS_IsDriveCompleteFOKS(void);
BOOL LENS_GetDirectionZOOM(void);
BOOL LENS_GetDirectionFOKS(void);
WORD LENS_GetTrackingLOCI(void);
LONG LENS_GetTrackingRATE(void);
SHORT LENS_GetCurPositionZOOM(void);
SHORT LENS_GetCurPositionFOKS(void);
SHORT ISPM LENS_GetRelPositionZOOM(void);
SHORT ISPM LENS_GetRelMaxPositionZOOM(void);
SHORT LENS_GetRelPositionFOKS(void);
SHORT ISPM LENS_GetCurPositionDZOOM(void);
SHORT LENS_GetMaxPositionZOOM(void);
SHORT LENS_GetMaxPositionFOKS(void);
SHORT LENS_GetMinPositionZOOM(void);
SHORT LENS_GetMinPositionFOKS(void);
SHORT LENS_GetPintPositionZOOM(void);
SHORT LENS_GetPintPositionFOKS(void);
SHORT LENS_GetKneePositionZOOM(void);
SHORT LENS_GetVISCAPositionZOOM(void);
SHORT LENS_GetVISCAPositionFOKS(void);
SHORT LENS_GetMIDPositionLOCI(void);
SHORT LENS_GetGAPPositionLOCI(void);
SHORT ISPM LENS_GetMAXPositionLOCI(SHORT zVal);
SHORT ISPM LENS_GetMINPositionLOCI(SHORT zVal);
SHORT ISPM LENS_GetMAXPositionModeLOCI(SHORT zVal,BOOL mode);
SHORT ISPM LENS_GetMINPositionModeLOCI(SHORT zVal,BOOL mode);
SHORT LENS_GetABVPositionLOCI(BYTE nID);
SHORT LENS_GetBLOPositionLOCI(BYTE nID);
void LENS_SetCurPositionZOOM(SHORT val);
void LENS_SetCurPositionFOKS(SHORT val);
SHORT LENS_CnvVISCAPositionZOOM(SHORT val);
SHORT LENS_CnvVISCAPositionFOKS(SHORT val);
SHORT LENS_CnvVISCAPositionLOCI(SHORT zVal, SHORT fVal);
void ISPM LENS_CtrlSPILineBUSY(void);
void ISPM LENS_CtrlSPILineFREE(void);
void ISPM LENS_CtrlTimeCounter(void);
WORD LENS_GetDrvTimeCount(void);
void LENS_SetDrvTimeCount(WORD cnt);
void ISPM LENS_XferDriveMOTOR(void);
void LENS_SetTrackingPEAK(SHORT val);
void LENS_SetTrackingINFO(SHORT val);
void LENS_SetTrackingZOOM(BOOL dir);
void LENS_SetMechanicMODE(BOOL OnOff);

void LENS_CurvePosition(void);
void LENS_LimitPosition(void);
BYTE LENS_GetAFDriveSTEP(BYTE nID);
void ISPM LENS_SetCurPositionDZOOM(SHORT val);

void ISPM OLPF_CtrlTimeCounter(void);
WORD OLPF_GetDrvTimeCount(void);
void OLPF_SetDrvTimeCount(WORD cnt);

void focusPickSearch(void);


#endif	/* __LENS_IF_H__ */
