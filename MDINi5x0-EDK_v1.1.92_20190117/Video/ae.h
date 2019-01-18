//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  AE.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISP_AE_H__  
#define		__MISP_AE_H__  

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif
#ifndef		__CMOS_IF_H__
#include	"cmos_if.h"
#endif

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
#ifndef		__LENS_IF_H__
#include	"lens_if.h"
#endif
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

	WORD	W0;					// weighting value of ae window0
	WORD	W1;					// weighting value of ae window1
	WORD	W2;					// weighting value of ae window2
	WORD	W3;					// weighting value of ae window3
	WORD	W4;					// weighting value of ae window4

	WORD	fix;				// Y-clip-fix-value of HLC window
	WORD	val;				// Y-clipping-value of HLC window
	DWORD	max;				// Y-clip-max-value of HLC window

}	AE_WIND_PARAM, *PAE_WIND_PARAM;

typedef	struct
{
	LONG	ctrl;					// AE control flag
	LONG	band;				// AE target dead-band

	SHORT	agcDiv;				// AE agc divider(large valus is low speed), normal value (6~32), low value is high speed of agc.
	SHORT	stepDiv;

	AE_WIND_PARAM	stFIX;		// area & weight of AE FIX window
	AE_WIND_PARAM	stBLC;		// area & weight of AE BLC window
	AE_WIND_PARAM	stHLC;		// area & weight of AE HLC window
	AE_WIND_PARAM	stSPOT;		// area & weight of AE SPOT window

	LONG	Ysum;				// current Ysum value, but only use in API function
	LONG	Yref;				// current Yref value, but only use in API function
	LONG	Ydiff;				// Ysum - Yref
	LONG	YDIFF;				// ABS(Ysum-Yref)

	SHORT	mark;				// IRIS return gain, but only use in API function
	SHORT	gain;				// IRIS defined gain, but only use in API function
	SHORT	diff;				// IRIS error range, but only use in API function
	SHORT	more;				// more execution, but only use in API function

	SHORT	level;				// AE brightness level
	BYTE	blcwinden;			// AE window display
	BYTE	testmode;				// test mode

}	AE_CTRL_PARAM, *PAE_CTRL_PARAM;

typedef	struct
{
	DWORD	iris;				// IRIS-GAIN Xfer value
	DWORD	gain;				// CMOS-GAIN Xfer value
	DWORD	shut;				// CMOS-SHUT Xfer value
	DWORD	fine;				// FINE-GAIN Xfer value

#if defined(__USE_CMOS_MN34220__)
	WORD	exp1;				// CMOS-SHUT Xfer value (sht_pos_wdr : exposure period Short)
	WORD	exp2;				// CMOS-SHUT Xfer value (sht_pos_wdr : exposure period Shorter)
#elif defined(__USE_CMOS_MN34420__)
	WORD	exp1;				// CMOS-SHUT Xfer value (sht_pos_wdr : exposure period Short)
	WORD	exp2;				// CMOS-SHUT Xfer value (sht_pos_wdr : exposure period Shorter)
	WORD	gap1;				// CMOS-SHUT Xfer value (start_exp_wdr : exposure start timing Short)
	WORD	gap2;				// CMOS-SHUT Xfer value (start_exp_wdr : exposure start timing Shorter)
#else
	WORD	shs2;				// CMOS-SHUT Xfer value (Shutter setting 2)
	WORD	shs3;				// CMOS-SHUT Xfer value (Shutter setting 3)
	WORD	rhs1;				// CMOS-SHUT Xfer value (readout timing 1)
	WORD	rhs2;				// CMOS-SHUT Xfer value (readout timing 2)
#endif

#if defined(__USE_P_IRIS_ONLY__)
	DWORD	irisCur;
#endif
	WORD	fUpdateGainVBlank;	// CMOS-GAIN direct update flag
	WORD	fUpdateDGainVBlank;	// DGAIN direct update flag
	FLOAT	dB;

}	AE_XFER_PARAM, *PAE_XFER_PARAM;

typedef	struct
{
	BYTE cntID;
	BYTE frmID;
	int	min;	
	int	max;

	LONG val;

	WORD	fUpdateSensUpVBlank;
}	AE_SENS_PARAM, *PAE_SENS_PARAM;

// remain bit  : 0x78800300
#define		AECTRL_LENS_MASK				0x00000003		// LENS mode
#define		AECTRL_MANUAL_LENS			0x00000000		// use manual lens
#define		AECTRL_DCIRIS_LENS			0x00000001		// use dc-iris lens
#define		AECTRL_V_IRIS_LENS			0x00000002		// use video-iris lens
#define		AECTRL_P_IRIS_LENS			0x00000003		// use p-iris lens

#define		AECTRL_PRIORITY_MASK			0x0000001C		// Priority mode
#define		AECTRL_PRIORITY_AUTO			0x00000004		// Shutter priority AUTO mode --(Default)
#define		AECTRL_PRIORITY_SHUTTER		0x00000008		// Shutter priority (fixed Shutter, Control Aperture)
#define		AECTRL_PRIORITY_APERTURE	0x0000000C		// Aperture priority (fixed Aperture, Control Shutter)
#define		AECTRL_PRIORITY_GAIN			0x00000010		// Gain priority (fixed Gain, Control shutter and  Aperture)
#define		AECTRL_ALL_MANUAL				0x00000014		// Gain priority (fixed Gain, Control shutter and  Aperture)

#define		AECTRL_GAIN_MASK				0x000000E0		// GAIN auto
#define		AECTRL_GAINCTL_OFF			0x00000000		// GAIN off
#define		AECTRL_GAINCTL_MIN			0x00000020		// GAIN min
#define		AECTRL_GAINCTL_MID			0x00000040		// GAIN mid
#define		AECTRL_GAINCTL_MAX			0x00000060		// GAIN max
#define		AECTRL_GAINCTL_DMIN		0x00000080		// GAIN digital gain 3x
#define		AECTRL_GAINCTL_DMID		0x000000A0		// GAIN digital gain 6x
#define		AECTRL_GAINCTL_DMAX		0x000000C0		// GAIN digital gain 9x

#define		AECTRL_BLC_MASK				0x00000300		// BLC mode
#define		AECTRL_BLC_MODE_OFF			0x00000000		// BLC off
#define		AECTRL_BLC_MODE_BLC			0x00000100		// BLC on
#define		AECTRL_BLC_MODE_HLC			0x00000200		// HSBLC on
#define		AECTRL_BLC_MODE_SPOT			0x00000300		// SPOT on

#define		AECTRL_WND_MASK				0x00000C00		// WND mode
#define		AECTRL_BLC_WIND_OFF			0x00000000		// BLC-WIND off
#define		AECTRL_BLC_WIND_BLC			0x00000400		// BLC-WIND BLC
#define		AECTRL_BLC_WIND_HLC			0x00000800		// BLC-WIND HLC

#define		AECTRL_GAINCTL_MASK			0x00004000		// GAIN mask
#define		AECTRL_GAINCTL_AUTO			0x00000000		// GAIN auto
#define		AECTRL_GAINCTL_MANU			0x00004000		// GAIN manu

#define		AECTRL_SHUTCTL_MASK			0x00008000		// SHUTTER mode
#define		AECTRL_SHUTCTL_AUTO			0x00000000		// SHUTTER auto
#define		AECTRL_SHUTCTL_MANU			0x00008000		// SHUTTER manu

#define		AECTRL_SHUT_MASK				0x001F0000		// SHUT manu
#define		AECTRL_SHUTCTL_1_25			0x00010000		// SHUT 1/25s
#define		AECTRL_SHUTCTL_1_30			0x00020000		// SHUT 1/30s
#define		AECTRL_SHUTCTL_1_50			0x00030000		// SHUT 1/50s
#define		AECTRL_SHUTCTL_1_60			0x00040000		// SHUT 1/60s
#define		AECTRL_SHUTCTL_1_100			0x00050000		// SHUT 1/100s
#define		AECTRL_SHUTCTL_1_120			0x00060000		// SHUT 1/120s
#define		AECTRL_SHUTCTL_1_150			0x00070000		// SHUT 1/150s
#define		AECTRL_SHUTCTL_1_180			0x00080000		// SHUT 1/180s
#define		AECTRL_SHUTCTL_1_200			0x00090000		// SHUT 1/200s
#define		AECTRL_SHUTCTL_1_240			0x000A0000		// SHUT 1/240s
#define		AECTRL_SHUTCTL_1_250			0x000B0000		// SHUT 1/250s
#define		AECTRL_SHUTCTL_1_300			0x000C0000		// SHUT 1/300s
#define		AECTRL_SHUTCTL_1_360			0x000D0000		// SHUT 1/360s
#define		AECTRL_SHUTCTL_1_480			0x000E0000		// SHUT 1/480s
#define		AECTRL_SHUTCTL_1_500			0x000F0000		// SHUT 1/500s
#define		AECTRL_SHUTCTL_1_600			0x00110000		// SHUT 1/600s
#define		AECTRL_SHUTCTL_1_1000		0x00120000		// SHUT 1/1000s
#define		AECTRL_SHUTCTL_1_2000		0x00130000		// SHUT 1/2000s
#define		AECTRL_SHUTCTL_1_5000		0x00140000		// SHUT 1/5000s
#define		AECTRL_SHUTCTL_1_10000		0x00150000		// SHUT 1/10000s

#define		AECTRL_SENS_UP_OFF			0x00000000		// sens-up off
#define		AECTRL_SENS_UP_ON			0x00200000		// sens-up on


#define		AECTRL_FLICKER_MASK				0x00400000		// flicker mode
#define		AECTRL_FLICKER_OFF			0x00000000		// flicker off
#define		AECTRL_FLICKER_ON				0x00400000		// flicker on

#define		AECTRL_PROC_MASK				0x07000000		// PROC mode
#define		AECTRL_PROC_NONE				0x00000000		// none
#define		AECTRL_PROC_SHUT				0x01000000		// SHUT mode
#define		AECTRL_PROC_IRIS				0x02000000		// IRIS mode
#define		AECTRL_PROC_GAIN				0x03000000		// GAIN mode
#define		AECTRL_PROC_SENS				0x04000000		// SENS mode

#define		AECTRL_PROCESS_RUN			0x00000000		// AE run
#define		AECTRL_PROCESS_OFF			0x80000000		// AE off
#define		AECTRL_TRIGGER_ISP			0x40000000		// ISP update
#define		AECTRL_PROCESS_PWM		0x20000000		// ISP update

#define 	__MISP100_AE_DBG__				0
#define 	__AE_IRIS_XRC_TEST__					0

//	// Optimal Min & Max
// range should be under 1023;
#if defined(__USE_X10X_ZMAX__)
#define IRIS_MAX				950
#define IRIS_MIN				100	// Active min
#define IRIS_OMAX				700	// F2.0
#define IRIS_OMIN				230	// F8
#define IRIS_PMAX				1023
#define IRIS_PMIN				0	// Pysical Min
#define IRIS_DEFAULT			700
#elif defined(__USE_X12X_UNION__)
#define IRIS_MAX				450
#define IRIS_MIN				80	// Active min
#define IRIS_OMAX			450
#define IRIS_OMIN				150	// Opt. min
#define IRIS_PMAX				525
#define IRIS_PMIN				0	// Pysical Min
#define IRIS_DEFAULT			280
#elif defined(__USE_X03X_ZMAX__)
#define IRIS_MAX				1000
#define IRIS_MIN				200	// Active min
#define IRIS_OMAX			900
#define IRIS_OMIN				300
#define IRIS_PMAX				1023
#define IRIS_PMIN				0	// Pysical Min
#define IRIS_DEFAULT			700
#elif defined(__USE_X10X_UNION__)
#define IRIS_MAX				950
#define IRIS_MIN					10	// Active min
#define IRIS_OMAX				700	// F2
#define IRIS_OMIN				200	// F8
#define IRIS_PMAX				1023
#define IRIS_PMIN				0	// Pysical Min
#define IRIS_DEFAULT			700 //700
#elif defined(__USE_X30X_TAMRON__)
#define IRIS_MAX				950
#define IRIS_MIN					50	// Active min
#define IRIS_OMAX				400	// F2.8
#define IRIS_OMIN				125	// F13
#define IRIS_PMAX				950
#define IRIS_PMIN				50	// Pysical Min
#define IRIS_DEFAULT			250
#else
#define IRIS_MAX				1023
#define IRIS_MIN					0	// Active min
#define IRIS_OMAX				IRIS_MAX
#define IRIS_OMIN				IRIS_MIN
#define IRIS_PMAX				1023
#define IRIS_PMIN				0	// Pysical Min
#define IRIS_DEFAULT			600
#endif

#define ODM_WND_DEF_MUL_W	40
#define ODM_WND_DEF_MUL_H	32
#define AE_WND_DEF_SIZE_W		21
#define AE_WND_DEF_SIZE_H		15
// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------
extern VBYTE fAECtrlODM_XFER;
extern int USE_INNER_DGAIN;

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// ae.c
void AECTRL_SetInstancePARAM(PAE_CTRL_PARAM pCTRL);
void AEXFER_SetInstancePARAM(PAE_XFER_PARAM pXFER);
void AE_UpdateHandler(PAE_XFER_PARAM pXFER);
void ISPM AE_UpdateGainVBlank(void);
void ISPM AE_UpdateDGainVBlank(void);
DWORD AECTRL_CnvGAINtoCMOS(DWORD gain);
DWORD AECTRL_CnvGAINtoFINE(DWORD gain);
LONG AECTRL_CnvCMOStoSHUT(DWORD shut);
DWORD AECTRL_CnvSHUTtoCMOS(LONG gain);
LONG AECTRL_GetYREFVal(BYTE nID);
FLOAT AECTRL_CalAERatio(LONG mode,PAE_CTRL_PARAM pCTRL);
void AECTRL_ViewBLCWIND(BOOL en);
void AECTRL_MoveBLCWIND(PAE_CTRL_PARAM pCTRL, BYTE x, BYTE y, BYTE w, BYTE h);
void BLC_HLC_Display(void);

void AECTRL_SetAE_ShutterFixMode(void);
void AECTRL_SetAE_IRISFixMode(void);
void AECTRL_SetAE_AutoMode(void);
void AECTRL_SetAE_GainFixMode(void);
void AECTRL_SetAE_Manual(void);
BOOL AECTRL_IsAE_FLKMode(void);

void AECTRL_SetAE_IRISValue(SHORT gain);
void AECTRL_SetAE_IRISBRValue(SHORT gain);
BYTE AECTRL_GetAE_IRISBRValue(LONG igain);
void AECTRL_SetConferIRISValue(SHORT gain);
void AECTRL_SetTestMode(void);

// ae_core.c
void AE_YsumHandler(PAE_CTRL_PARAM pCTRL);
void AE_ProcessHandler(PAE_CTRL_PARAM pCTRL);
void AE_DisplayHandler(PAE_CTRL_PARAM pCTRL);
void AECTRL_SetBrightness(PAE_CTRL_PARAM pCTRL, SHORT val);
void AECTRL_SetGAINCtrl(PAE_CTRL_PARAM pCTRL, LONG val, LONG ctrl, BOOL fmode); // fmode(0) : gain is within (min, max).... fmode(1) : just set gain
void AECTRL_SetSHUTCtrl(PAE_CTRL_PARAM pCTRL, SHORT val, LONG ctrl);
LONG AECTRL_GetBrightness(void);
void ODM_SetDEBUGMode(BYTE mode);
BYTE ODM_GetDEBUGMode(void);
void AECTRL_setSlowAE(BYTE val);
BYTE AECTRL_getSlowAE(void);

void AECTRL_SensHandler(PAE_CTRL_PARAM pCTRL);
WORD AECTRL_GetSensUpMax(void);
void AECTRL_SetSensUpMax(WORD val);
WORD AECTRL_GetSensUpMin(void);
void AECTRL_SetSensUpMin(WORD val);
WORD AECTRL_GetSensUpVal(void);
void AECTRL_SetSensUpVal(WORD val);
void AECTRL_SensUpInit(LONG min);
void AE_UpdateSensUpVBlank(void);
void AE_UpdateSensUpVal(BOOL mode,LONG val);
BOOL AECTRL_IsSensUpMin(void);
BOOL AECTRL_IsSensUpMax(void);

// ae_iris.c & ae_hall.c
void DCLENS_AECtrlProcess(PAE_CTRL_PARAM pCTRL);
void DCLENS_ClearModeIRIS(void);
BOOL DCLENS_IsIRISCtrlMode(void);
BOOL DCLENS_IsIRISCtrlOptiMax(void);
BOOL DCLENS_IsIRISCtrlOptiMin(void);
void DCLENS_SetIRISRange(SHORT min, SHORT max);
void DCLENS_SetIRISOptiRange(SHORT omin, SHORT omax);
void DCLENS_SetIRISPhyRange(SHORT pmin, SHORT pmax);
void DCLENS_SetIRISOptiMax(SHORT omax);
void DCLENS_SetIRISOptiMax(SHORT omax);
void DCLENS_SetIRISOptiMin(SHORT omin);
void DCLENS_SetIRISGain(SHORT gain);
void DCLENS_ForceSetIRISGain(SHORT gain); // use after stop AE
void DCLENS_ForceSetIRISGain2(SHORT gain);
SHORT DCLENS_GetIRISGain(void);
SHORT DCLENS_GetIRISMax(void);
SHORT DCLENS_GetIRISMin(void);
WORD DCLENS_GetIRISState(void);
BYTE DCLENS_GetIRISValue(void);
void DCLENS_IRISInit(void);
SHORT DCLENS_GetIRISOptiMax(void);
SHORT DCLENS_GetIRISOptiMin(void);

#ifdef __P_IRIS_TEST__
void AE_P_IRIS_Open(void);
void AE_P_IRIS_Close(void);
#endif

// ae_agc.c
void AECTRL_GAINHandler(PAE_CTRL_PARAM pCTRL);
void AECTRL_ClearModeGAIN(void);
BOOL AECTRL_IsGAINCtrlMode(PAE_CTRL_PARAM pCTRL);
BOOL AECTRL_IsGAINCtrlMax(void);
BOOL AECTRL_IsGAINCtrlHistMax(void);
BOOL AECTRL_IsGAINCtrlMin(void);
void AECTRL_SetGAINRange(LONG min, LONG max);
void AECTRL_SetGAINMax(LONG max);
void AECTRL_SetGAINMin(LONG min);
void AECTRL_SetGAINPMax(LONG pmax);
void AECTRL_SetGAINGain(LONG gain);
void AECTRL_SetGAINGainDirect(LONG gain);
LONG AECTRL_GetGAINGain(void);
LONG AECTRL_GetGAINAGain(void); // analog gain
LONG AECTRL_GetGAINMin(void);
LONG AECTRL_GetGAINMax(void); // chang by MENU
LONG AECTRL_GetGAINAMax(void); // analog gain max
LONG AECTRL_GetGAINPMax(void); // 0x40000
BYTE AECTRL_GetGAINCnt(void);
LONG AECTRL_GetGAINPrev(void);
void AECTRL_GAINCheck(void);
LONG AECTRL_GetGainCTLMode(void);

// ae_sht.c
void AECTRL_SHUTHandler(PAE_CTRL_PARAM pCTRL);
void AECTRL_ClearModeSHUT(void);
BOOL AECTRL_IsSHUTCtrlMode(PAE_CTRL_PARAM pCTRL);
void AECTRL_SetSHUTDefaultMax(void);
//BOOL AECTRL_IsSHUTCtrlMin(PAE_CTRL_PARAM pCTRL);
//BOOL AECTRL_IsSHUTCtrlMax(PAE_CTRL_PARAM pCTRL);
BOOL AECTRL_IsSHUTCtrlGmin(void);
BOOL AECTRL_IsSHUTCtrlGmax(void);
void AECTRL_SetSHUTRange(SHORT min, SHORT max);
void AECTRL_SetSHUTMax(SHORT max);
void AECTRL_SetSHUTGain(SHORT gain);
void AECTRL_SetSHUTOmax(SHORT omax);
SHORT AECTRL_GetSHUTGain(void);
SHORT AECTRL_GetSHUTMin(void);
SHORT AECTRL_GetSHUTMax(void);
SHORT AECTRL_GetSHUTOmax(void);
LONG AECTRL_GetSHUTGMin(void);
LONG AECTRL_GetSHUTGMax(void);

// ae_aux.c
void AE_DBGIrisHandler(PAE_CTRL_PARAM pCTRL);
void SetAEDBGIrisTrigger(void);
void AE_DBGGainHandler(PAE_CTRL_PARAM pCTRL);
void SetAEDBGGainTrigger(void);
void AE_DBGShutHandler(PAE_CTRL_PARAM pCTRL);
void SetAEDBGShutTrigger(void);
void SetAEDBGCleanAll(void);


// ae_blc.c
void AECTRL_BLCModeHandler(PAE_CTRL_PARAM pCTRL);

// sht.c
void AECTRL_SetDSPSHTMode(BYTE val);
void AECTRL_SetDSPSlowSHTMode(BYTE val);
void AEDBG_SetSHTGain(void);

// dn.c
void AECTRL_DANHandler(void);
void AECTRL_SetDSPDANMode(BYTE val);
void AECTRL_SetDSPFILTMode(BYTE val);

//ae_hall.c
void DCLENS_ForceIRISCtl(BYTE val); // IRIS open/close
void DCLENS_ForceIRISUpDown(BOOL updn); // IRIS set value up/down

// etc

void AECTRL_DirectSetTest(void);
void AE_DigitalGainCtl(PAE_CTRL_PARAM pCTRL);

BYTE AECTRL_GetGAINRatio(void);
BYTE AECTRL_GetSTDRatio(void);

void AE_IRIS_Full_Open(BOOL onoff);
void AE_IRIS_Full_Close(BOOL onoff);
//void AF_Var_Filter_Select(void);
void Histogram_Read(void);

//ae_pwm.c
void DCLENS_IRISAutoMode(SHORT mode,PAE_CTRL_PARAM pCTRL);
void DCLENS_IrisSpeed(BYTE val);
void DCLENS_LensMode(BYTE val);
BYTE DCLENS_getIrisSpeed(void);
void DCLENS_IRIS_SetOffset(WORD offset);
WORD DCLENS_IRIS_GetOffset(void);
void DCLENS_IRIS_SetADC(WORD offset);
WORD DCLENS_IRIS_GetADC(void);
void DCLENS_IRIS_Tune(void);

//ae_piris.c
#if defined (__ECOM1_UNION_12X__)
void AECTRL_PIRIS_Handler(void);
void AECTRL_PIRIS_Init(void);
void AECTRL_PIRIS_ForceSetPosition(SHORT ipos);
void AECTRL_PIRIS_SetPosition(SHORT ipos);
SHORT AECTRL_PIRIS_GetCurPosition(void);
SHORT AECTRL_PIRIS_GetMaxPosition(void);
SHORT AECTRL_PIRIS_GetMinPosition(void);
#endif

#endif	/* __MISP_AE_H__ */
