//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  AWB.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISP_AWB_H__  
#define		__MISP_AWB_H__  

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif
#ifndef		__CMOS_IF_H__
#include	"cmos_if.h"		// for sensor dependent option
#endif

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

typedef	struct
{
	BYTE	sX;					// X start position of area window
	BYTE	sY;					// Y start position of area window
	BYTE	nW;					// horizontal width of area window
	BYTE	nH;					// vertical height of area window

}	WB_WIND_PARAM, *PWB_WIND_PARAM;

typedef	struct
{
	SHORT	sR;					// compensation R-component-slop
	SHORT	sB;					// compensation B-component-slop

	LONG	jR;					// compensation R-component-jeol
	LONG	jB;					// compensation B-component-jeol

}	WB_COMP_PARAM, *PWB_COMP_PARAM;

typedef	struct
{
	DWORD	cntColor;				// valid counter

	LONG	fAvgR;				// frame avg R
	LONG	fAvgG;				// frame avg G
	LONG	fAvgB;				// frame avg B

	LONG	fAvgY;				// frame avg Y
	LONG	fAvgU;				// frame avg U
	LONG	fAvgV;				// frame avg V

	WORD	U[8];	// for MDINAPP
	WORD	V[8];	// for MDINAPP

}	WB_COLOR_PARAM, *PWB_COLOR_PARAM;

typedef	struct
{
	WORD	D[44];				// polygon parameter

}	WB_POLY_PARAM, *PWB_POLY_PARAM;

typedef	struct
{
	LONG	Ro[8];				// R-component of refernece point
	LONG	Gr[8];				// Gr-component of refernece point
	LONG	Bo[8];				// B-component of refernece point
	LONG	Gb[8];				// Gb-component of refernece point

}	WB_BASE_PARAM, *PWB_BASE_PARAM;

typedef	struct
{
	CHAR	bU[8];				// upper gap of refernece point
	CHAR	bL[8];				// lower gap of refernece point

}	WB_GAP_PARAM, *PWB_GAP_PARAM;

typedef	struct
{
	WORD	R;					// R-gain according to WB-MODE
	WORD	B;					// B-gain according to WB-MODE

	WORD	L;					// boundary of low-temperature
	WORD	H;					// boundary of high-temperature

}	WB_GAIN_PARAM, *PWB_GAIN_PARAM;

typedef	struct
{
	LONG	ctrl;				// WB control flag
	SHORT	offR;				// R-offset of white point
	SHORT	offB;				// B-offset of white point

	WB_WIND_PARAM	stWIND;		// area of WB window
	WB_GAIN_PARAM	stWBHOLD;	// WB-HOLD mode gain
	WB_GAIN_PARAM	stINDOOR;	// indoor mode gain
	WB_GAIN_PARAM	stOTDOOR;	// outdoor mode gain
	WB_GAIN_PARAM	stPRESET;	// preset mode gain
	WB_GAIN_PARAM	stMANUAL;	// manual mode gain

	WB_COMP_PARAM	stWBCOMP;	// WB-COMP gain
	WB_POLY_PARAM	stWBPOLY;	// WB-POLY data
	WB_COLOR_PARAM	stWBCOLOR;	// WB-YUV values

}	WB_CTRL_PARAM, *PWB_CTRL_PARAM;

#define		WBCTRL_MODE_MASK			0x00000007		// CTRL mode
#define		WBCTRL_WBAUTO_MODE			0x00000000		// WB-AUTO mode
#define		WBCTRL_WBHOLD_MODE			0x00000001		// WB-HOLD mode
#define		WBCTRL_INDOOR_MODE			0x00000002		// indoor mode
#define		WBCTRL_OTDOOR_MODE			0x00000003		// outdoor mode
#define		WBCTRL_PRESET_MODE			0x00000004		// preset mode
#define		WBCTRL_MANUAL_MODE			0x00000005		// manual mode

#define		WBCTRL_WDR_MASK				0x00000070		// WDR mode
#define		WBCTRL_WDR_OFF				0x00000000		// normal mode
#define		WBCTRL_WDR_FRAME			0x00000010		// 2frame mode
#define		WBCTRL_WDR_4LINE			0x00000020		// 4-line mode
#define		WBCTRL_WDR_3LINE			0x00000030		// 3-line mode
#define		WBCTRL_WDR_2LINE			0x00000040		// 2-line mode
#define		WBCTRL_WDR_BUILT			0x00000050		// HTM mode

#define		WBCTRL_SPEED_MASK			0x00000f00		// AWB speed
#define		WBCTRL_SPEED_VAR0			0x00000000		// var0
#define		WBCTRL_SPEED_VAR1			0x00000100		// var1
#define		WBCTRL_SPEED_VAR2			0x00000200		// var2
#define		WBCTRL_SPEED_VAR3			0x00000300		// var3
#define		WBCTRL_SPEED_VAR4			0x00000400		// var4
#define		WBCTRL_SPEED_VAR5			0x00000500		// var5
#define		WBCTRL_SPEED_VAR6			0x00000600		// var6
#define		WBCTRL_SPEED_VAR7			0x00000700		// var7

#define		WBCTRL_WDRCORE_OFF			0x00000000		// disable WDR
#define		WBCTRL_WDRCORE_RUN			0x10000000		// enable WDR

#define		WBCTRL_PROCESS_RUN			0x00000000		// AWB run
#define		WBCTRL_PROCESS_OFF			0x80000000		// AWB off
#define		WBCTRL_PROCESS_RST			0x40000000		// AWB reset
#define		WBCTRL_TRIGGER_ISP			0x20000000		// ISP update
#define		WBCTRL_PROCESS_HOLD			0x10000000		// AWB hold

#if defined(__USE_CMOS_IMX124__)|| defined(__USE_IMX265_1536P_ALL_SCAN_MODE___) 	// 3-million
#define		WBVIEW_HACTIVE_LEN			2048
#define		WBVIEW_VACTIVE_LEN			1536
#endif

#if defined(__USE_CMOS_IMX226__)		// 8-million
#if defined(__USE_IMX226_1080P60_MODE__)
#define		WBVIEW_HACTIVE_LEN			1920
#define		WBVIEW_VACTIVE_LEN			1080
#else
#define		WBVIEW_HACTIVE_LEN			3840
#define		WBVIEW_VACTIVE_LEN			2160
#endif
#endif

#ifndef		WBVIEW_HACTIVE_LEN  		// 2-million
#define		WBVIEW_HACTIVE_LEN			1920
#define		WBVIEW_VACTIVE_LEN			1080
#endif

#define 	__MISP100_AWB_DBG__				1

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// awb.c
void WBCTRL_SetInstancePARAM(PWB_CTRL_PARAM pCTRL);
void WBCTRL_GetCOMPGain(PWB_CTRL_PARAM pCTRL, BYTE nID);
void WBCTRL_GetPOLYInfo(PWB_CTRL_PARAM pCTRL, BYTE nID);

// awb_core.c
void WB_ProcessHandler(PWB_CTRL_PARAM pCTRL);
void WB_DisplayHandler(PWB_CTRL_PARAM pCTRL);
void WB_SetAWBDrvSPEED(PWB_CTRL_PARAM pCTRL, LONG val);
void WB_SetAWBDrvSHIFT(PWB_CTRL_PARAM pCTRL, SHORT offR, SHORT offB);
void WB_GetAWBSumVALUE(PLONG R, PLONG G, PLONG B, PLONG W);
void WB_GetAWBGryVALUE(PLONG R, PLONG G, PLONG B, PLONG W);
void WB_GetAWBCtlVALUE(PLONG R, PLONG G, PLONG B, PLONG W);

void WBCTRL_SetAWBCtrlPath(BOOL ishtm, BOOL RBslope, BOOL RBscomp, BOOL DCgain, BOOL WBgain );

// awb_aux.c
void WD_ProcessHandler(PWB_CTRL_PARAM pCTRL);
void WD_TriggerHandler(PWB_CTRL_PARAM pCTRL);
void SetWBRAWViewTrigger(BOOL mode);
void SetWBRAWOnOffCOMP(void);
void SetWBRAWOnOffPOLY(void);
BOOL GetWBRAWOnOffMODE(void);
void SetWBRAWOnOffTIME(void);
BOOL GetWBRAWOnOffTIME(void);
void SetWBRAWViewShiftL(void);
void SetWBRAWViewShiftR(void);
void SetWBRAWViewShiftU(void);
void SetWBRAWViewShiftD(void);

// awb_color.c
void ColorRAW_ProcessInit(void);
void SetColorRAWViewTrigger(void);
void Color_ProcessHandler(PWB_CTRL_PARAM pCTRL);
void AWB_ModeChange(BOOL mode);

#endif	/* __MISP_AWB_H__ */
