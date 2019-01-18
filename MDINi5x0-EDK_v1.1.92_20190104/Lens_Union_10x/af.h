//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  AF.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISP_AF_H__  
#define		__MISP_AF_H__  

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif

#if defined(__USE_AFZOOM_LENS__)
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
#define AF_WND_DEF_SIZE_W		47
#define AF_WND_DEF_SIZE_H		32

#define AF_NO_MAN_FOCUS_IN_AUTO

typedef enum {
	DRV_STEP_MIN = 0x10, DRV_STEP_STD, DRV_STEP_MID, DRV_STEP_MAX, DRV_STEP_FWD,
	DRV_STEP_BWD = 0x20, DRV_STEP_TOP, DRV_STEP_DIV, DRV_STEP_REM,
	DRV_STEP_PRV = 0x30, DRV_STEP_GAP

} 	AF_DRIVING_STEP;

typedef enum {
	NORM_XX = 0x00, DARK_HL, DARK_ML, DARK_LL,
	SPOT_XX = 0x80

} 	AF_PROCESS_MODE;

typedef	struct
{
	WORD	sX;					// X start position of area window
	WORD	sY;					// Y start position of area window
	WORD	eX;					// X end position of area window
	WORD	eY;					// Y end position of area window

}	AF_WIND_PARAM, *PAF_WIND_PARAM;

typedef	struct
{
	LONG	RPFa;				// RPF data of window A
	LONG	LPFa;				// LPF data of window A
	LONG	VPFa;				// VPF data of window A
	LONG	BPFa;				// BPF data of window A
	LONG	HPFa;				// HPF data of window A
	LONG	CNTa;				// CNT data of window A

	LONG	RPFb;				// RPF data of window B
	LONG	LPFb;				// LPF data of window B
	LONG	VPFb;				// VPF data of window B
	LONG	BPFb;				// BPF data of window B
	LONG	HPFb;				// HPF data of window B
	LONG	CNTb;				// CNT data of window B

	LONG	YSUM;				// luminance information
	LONG	YPRV;				// previous information
	LONG	GAIN;				// exposure information

}	AF_FILT_PARAM, *PAF_FILT_PARAM;

typedef	struct
{
	LONG	ctrl;				// AF control flag
	SHORT	wide;				// start position of zoom
	SHORT	tele;				// end position of zoom
	SHORT	digi;				// end position of dzoom
	SHORT	near;				// near limit of focus

	WORD	tACT;				// movement time
	WORD	tINT;				// interval time

	AF_WIND_PARAM	stWNDA;		// area of AF window A
	AF_WIND_PARAM	stWNDB;		// area of AF window B

	WORD	nACT;				// movement count, but only use in API function
	WORD	nINT;				// interval count, but only use in API function
	BYTE	nMOD;				// process mode, but only use in API function
	BYTE	xMOD;				//
	BYTE	nSPT;				//
	AF_FILT_PARAM	stFILT;		// filter data, but only use in API function

}	AF_CTRL_PARAM, *PAF_CTRL_PARAM;

#define		AFCTRL_AFMODE_MASK			0x00000003		// AF mode
#define		AFCTRL_AFAUTO_MODE			0x00000000		// auto
#define		AFCTRL_INTVAL_MODE			0x00000001		// interval
#define		AFCTRL_AFZOOM_MODE			0x00000002		// zoom trigger

#define		AFCTRL_AFAUTO_MASK			0x00000008		// AUTO/MANUAL
#define		AFCTRL_MANUAL_MODE			0x00000008		// manual

#define		AFZOOM_SPEED_MASK			0x000000f0		// ZOOM speed
#define		AFZOOM_SPEED_VAR0			0x00000000		// var0
#define		AFZOOM_SPEED_VAR1			0x00000010		// var1
#define		AFZOOM_SPEED_VAR2			0x00000020		// var2
#define		AFZOOM_SPEED_VAR3			0x00000030		// var3
#define		AFZOOM_SPEED_VAR4			0x00000040		// var4
#define		AFZOOM_SPEED_VAR5			0x00000050		// var5
#define		AFZOOM_SPEED_VAR6			0x00000060		// var6
#define		AFZOOM_SPEED_VAR7			0x00000070		// var7

#define		AFFOKS_SPEED_MASK			0x00000f00		// FOKS speed
#define		AFFOKS_SPEED_VAR0			0x00000000		// var0
#define		AFFOKS_SPEED_VAR1			0x00000100		// var1
#define		AFFOKS_SPEED_VAR2			0x00000200		// var2
#define		AFFOKS_SPEED_VAR3			0x00000300		// var3
#define		AFFOKS_SPEED_VAR4			0x00000400		// var4
#define		AFFOKS_SPEED_VAR5			0x00000500		// var5
#define		AFFOKS_SPEED_VAR6			0x00000600		// var6
#define		AFFOKS_SPEED_VAR7			0x00000700		// var7

#define		AFFOKS_LIMIT_MASK			0x0000f000		// FOKS limit
#define		AFFOKS_LIMIT_FAR			0x00001000		// over inf
#define		AFFOKS_LIMIT_INF			0x00002000		// inf
#define		AFFOKS_LIMIT_10M			0x00003000		// 10m
#define		AFFOKS_LIMIT_6p0M			0x00004000		// 6.0m
#define		AFFOKS_LIMIT_3p0M			0x00006000		// 3.0m
#define		AFFOKS_LIMIT_2p0M			0x00008000		// 2.0m
#define		AFFOKS_LIMIT_1p5M			0x0000a000		// 1.5m
#define		AFFOKS_LIMIT_1p0M			0x0000c000		// 1.0m
#define		AFFOKS_LIMIT_0p3M			0x0000d000		// 0.3m
#define		AFFOKS_LIMIT_0p1M			0x0000e000		// 0.1m

#define		AFZOOM_DZOOM_ON				0x00000000		// DZOOM on
#define		AFZOOM_DZOOM_OFF			0x80000000		// DZOOM off

#define		AFZOOM_DZMIX_ON				0x00000000		// DZOOM combine on
#define		AFZOOM_DZMIX_OFF			0x40000000		// DZOOM combine off

#define		AFFOKS_SENSE_NRM			0x00000000		// AF sensitivity norm
#define		AFFOKS_SENSE_LOW			0x20000000		// AF sensitivity low

#define		AFFILT_IRCUT_ON				0x00000000		// IR-CUT on
#define		AFFILT_IRCUT_OFF			0x10000000		// IR-CUT off

#define		AFCTRL_RST_FRAME			0x08000000		// reset frame
#define		AFCTRL_WDR_FRAME			0x04000000		// 2frame mode

#define 	__MISP100_AF_DBG__			0
#define 	__MISP100_AF_DBG_CORE__		1
#define 	__MISP100_AF_CNV__				0

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------
extern BOOL  fSysStateCHANGE;

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
// af.c
void AFCTRL_SetInstancePARAM(PAF_CTRL_PARAM pCTRL);
FLOAT AFCTRL_CalAFRatio(void);
BYTE AFCTRL_GetZoomRatio(void);

// af_core.c
void AF_ProcessHandler(PAF_CTRL_PARAM pCTRL);
void AF_DisplayHandler(PAF_CTRL_PARAM pCTRL);
void AF_ConvertHandler(PAF_CTRL_PARAM pCTRL);
void AF_EnableProcess(BOOL OnOff);
BOOL AF_IsProcessExecuting(void);
void AF_SetFILTCoef(void);
void AF_SetAUTOInfo(PAF_CTRL_PARAM pCTRL, BOOL nCHK);
BOOL AF_GetLUMAInfo(PAF_CTRL_PARAM pCTRL, WORD nID);
BOOL AF_GetDATAInfo(PAF_CTRL_PARAM pCTRL, BOOL nID);
LONG AF_GetFILTVal(PAF_CTRL_PARAM pCTRL, BYTE nID);
void AF_SetConferenceMode(int mode); // mimi test
int AF_GetConferenceMode(void); // mimi test
void AF_SetConferenceWin(PMISP_VIDEO_WINDOW pWIN);	// mimi test
void AF_SetTrMode(BYTE val);
BYTE AF_GetTrMode(void);

// af_norm.c
void AFNORM_ProcessHandler(PAF_CTRL_PARAM pCTRL);
void AFNORM_EnableProcess(BOOL OnOff);
BOOL AFNORM_IsProcessExecuting(void);
void AFNORM_ChangeAFCtlVal(int init,int cur,int val);

// af_dark.c
void AFDARK_ProcessHandler(PAF_CTRL_PARAM pCTRL);
void AFDARK_EnableProcess(BOOL OnOff);
BOOL AFDARK_IsProcessExecuting(void);

// af_spot.c
void AFSPOT_ProcessHandler(PAF_CTRL_PARAM pCTRL);
void AFSPOT_EnableProcess(BOOL OnOff);
BOOL AFSPOT_IsProcessExecuting(void);

// af_pint.c
void AX_ProcessHandler(PAF_CTRL_PARAM pCTRL);
void AX_EnableProcess(BOOL OnOff);
BOOL AX_IsProcessExecuting(void);

void SetDirDrivePINT(BOOL dir);
void SetDrvOnOffPINT(void);
BOOL GetDrvOnOffPINT(void);
void SetDrvOffPINT(void);


// af_age.c
void AA_ProcessHandler(PAF_CTRL_PARAM pCTRL);
void SetAgeOnOffLENS(void);
BOOL GetAgeOnOffLENS(void);
void AA_ZOOMHandler(BOOL dir);
void AA_FOKSHandler(BOOL dir);
void AA_IRISHandler(BOOL OnOff);
void AA_OLPFHandler(BOOL OnOff);


// af_aux.c
void AF_ExtractHandler(PAF_CTRL_PARAM pCTRL);
void SetAFExtractTrigger(void);
BOOL AF_IsExtractExecuting(void);

void AF_PanTiltHandler(PAF_CTRL_PARAM pCTRL);
void SetAFPanTiltTrigger(void);

// af_focus.c
void SetDirDriveFOKS(BOOL dir);
void CoarseFineDriveFOKS(BOOL CoarseFine, BOOL dir);
void CoarseDriveFOKS(BOOL CoarseFine, BOOL dir);
void CompleDriveFOKS(BOOL dir);
void TargetDriveFOKS(WORD val);

// af_zoom.c
void SetDirDriveZOOM(BOOL dir);
void CoarseDriveZOOM(BOOL dir);
void CompleDriveZOOM(BOOL dir);
void TargetDriveZOOM(WORD val);
void TargetDriveLENS(WORD zVal, WORD fVal);

// lens_xxx.c
void LENS_SetFOKSDrvLIMIT(PAF_CTRL_PARAM pCTRL, SHORT val);
void LENS_SetFOKSDrvSPEED(PAF_CTRL_PARAM pCTRL, LONG val);
void LENS_SetWIDEDrvLIMIT(PAF_CTRL_PARAM pCTRL, SHORT val);
void LENS_SetTELEDrvLIMIT(PAF_CTRL_PARAM pCTRL, SHORT val);
void LENS_SetDIGIDrvOnOff(PAF_CTRL_PARAM pCTRL, BOOL OnOff);
void LENS_SetDIGIMixOnOff(PAF_CTRL_PARAM pCTRL, BOOL OnOff);
void LENS_SetDIGIDrvLIMIT(PAF_CTRL_PARAM pCTRL, SHORT val);
void LENS_SetZOOMDrvSPEED(PAF_CTRL_PARAM pCTRL, LONG val);
void LENS_SetAFIntvalTIME(PAF_CTRL_PARAM pCTRL, WORD val);
void LENS_SetAFActiveTIME(PAF_CTRL_PARAM pCTRL, WORD val);

void LENS_DrvHandlerFOKS(PAF_CTRL_PARAM pCTRL);
void LENS_DrvHandlerZOOM(PAF_CTRL_PARAM pCTRL);
BOOL LENS_IsDriveExecuting(void);

void DZ_UpdateHandler(PAF_CTRL_PARAM pCTRL);

#endif


#endif	/* __MISP_AF_H__ */
