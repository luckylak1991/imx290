//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2005  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	LENS_TAMRON.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"
#include	"lens_tamron.h"

#if defined(__USE_X30X_TAMRON__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
#define	LENS_FLAGS()	((BYTE)fLensKeepPosition<<7)|((BYTE)fLensSuspendState<<6)|\
						((BYTE)fLensSavePosition<<5)|((BYTE)fLensMechPosition<<4)|\
						((BYTE)fZoomZeroNotFound<<1)|((BYTE)fFoksZeroNotFound<<0)

#define	ZOOM_FLAGS()	((BYTE)fZoomDriveDigital<<7)|((BYTE)fZoomDriveTrigger<<6)|\
						((BYTE)fZoomDrvCompleted<<5)|((BYTE)fZoomDrvDirection<<4)|\
						((BYTE)fZoomDrvTrackMode<<3)|((BYTE)fZoomDrvLimitMode<<2)|\
						((BYTE)fZoomDrvStepRatio<<1)|((BYTE)fZoomCompensation<<0)

#define	FOKS_FLAGS()	((BYTE)fFoksDriveLocking<<7)|((BYTE)fFoksDriveTrigger<<6)|\
						((BYTE)fFoksDrvCompleted<<5)|((BYTE)fFoksDrvDirection<<4)|\
						((BYTE)fFoksDrvMacroMode<<3)|((BYTE)fFoksDrvMicroMode<<2)|\
						((BYTE)fFoksDrvLimitMode<<1)|((BYTE)fFoksCompensation<<0)

typedef	struct {
	SHORT	cur;			// motor current position (absolute value)
	SHORT	peak;			// motor tracking position (absolute value)
	SHORT	goal;			// motor drive target position (absolute value)

	SHORT	mMin;			// motor mechanical minimum position (absolute value)
	SHORT	mMax;			// motor mechanical maximum position (absolute value)
	SHORT	oMin;			// motor optical minimum position (absolute value)
	SHORT	oMax;			// motor optical maximum position (absolute value)
	SHORT	pint;			// motor pint(punt) position (absolute value)
	SHORT	knee;			// motor knee-curve position (absolute value) // forcus min position

	SHORT	tMax;			// motor opt+dig maximum position (absolute value)

	WORD	ADcv;			// motor current position AD conversion value
	WORD	xfer;			// transfer motor data to drive IC

}	LENS_INFO, *PLENS_INFO;

typedef	struct {
	SHORT	wide;			// ZOOM motor start position
	SHORT	tele;			// ZOOM motor end position
	SHORT	digi;			// ZOOM digital end position
	SHORT	near;			// FOKS limit VISCA position

	SHORT	z_ID;			// ZOOM motor drive speed
	SHORT	f_ID;			// FOKS motor drive speed

	BYTE	fCTL;			// FOKS motor drive process
	BYTE	fDIR;			// FOKS motor drive direction
	SHORT	fTGT;			// FOKS motor target value

	BYTE	zCTL;			// ZOOM motor drive process
	BYTE	zDIR;			// FOKS motor drive direction
	SHORT	zTGT;			// ZOOM motor target value

}	LENS_PARAM, *PLENS_PARAM;

typedef enum {
	PRE_LENS_GOAL = 0x10, DRV_LENS_GOAL, END_LENS_GOAL,
	END_LENS_PROC = 0x00

} 	LENS_PROCESS_GOAL;

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static volatile BOOL fLensKeepPosition, fLensSuspendState, fLensSavePosition, fLensMechPosition;
static volatile BOOL fZoomZeroNotFound, fFoksZeroNotFound, fZoomDigitalOnOff, fZoomDigiMixOnOff;
static volatile BOOL fZoomDriveDigital, fZoomDriveTrigger, fZoomDrvCompleted, fZoomDrvDirection;
static volatile BOOL fZoomDrvTrackMode, fZoomDrvLimitMode, fZoomDrvStepRatio, fZoomCompensation;
static volatile BOOL fFoksDriveLocking, fFoksDriveTrigger, fFoksDrvCompleted, fFoksDrvDirection;
static volatile BOOL fFoksDrvMacroMode, fFoksDrvMicroMode, fFoksDrvLimitMode, fFoksCompensation;
static volatile BOOL fZoomDrvSpeedMode, fFoksDrvSpeedMode;

static volatile FLOAT fRatio;
static volatile WORD  wRefID, wLockCnt, wDrvTime, wOLPFCnt, dZOOMCnt;
static volatile LENS_INFO stZOOM, stFOKS;
static volatile LENS_PARAM stLENS;

static ROMDATA SHORT LENS_SpeedByZOOM[][14]		= {
//    15    14    13    12    11    10     9     8     7     6     5     4     3     2
	{   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 2489, 2540, 2640},	// 0
	{   0,    0,    0,    0,    0,    0,    0,    0,    0, 2416, 2454, 2499, 2549, 2640},	// 1
	{   0,    0,    0,    0,    0,    0,    0, 2381, 2401, 2426, 2465, 2510, 2560, 2640},	// 2
	{   0,    0,    0,    0,    0, 2356, 2372, 2389, 2411, 2438, 2476, 2519, 2570, 2640},	// 3
	{   0,    0,    0, 2340, 2351, 2365, 2380, 2399, 2420, 2451, 2487, 2528, 2578, 2640},	// 4
	{   0, 2326, 2335, 2347, 2357, 2372, 2388, 2407, 2431, 2460, 2496, 2536, 2585, 2645},	// 5
	{2326, 2333, 2341, 2353, 2365, 2378, 2394, 2417, 2439, 2469, 2502, 2543, 2591, 2651},	// 6
	{2334, 2342, 2353, 2364, 2375, 2389, 2406, 2425, 2448, 2476, 2510, 2548, 2604, 2658}	// 7
};

static ROMDATA SHORT LENS_SpeedByDZOOM[][3]		= {
	{ 2,  3,  3},	// 0
	{ 2,  3,  3},	// 1
	{ 2,  2,  3},	// 2
	{ 2,  2,  3},	// 3
	{ 2,  2,  2},	// 4
	{ 2,  2,  2},	// 5
	{ 1,  2,  2},	// 6
	{ 1,  2,  2}	// 7
};

static ROMDATA WORD LENS_LimitByFOKS[]			= {
//	  far     inf     10m     6m      3m      2m     1.5m     1m     0.3m    0.1m
	0x1000, 0x2000, 0x3000, 0x4000, 0x6000, 0x8000, 0xa000, 0xc000, 0xd000, 0xe000
};

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
static void LENSDRV_Initialize(void)
{
	WORD mode, zVal, fVal;

	LENS_VD(HIGH);	LENS_VD(LOW);

#if defined(__USE_DCIRIS_DAMP__)
	mode = (0<10)|(0<<9)|(0<<8)|(1<<7);	// PDWEN[10] = (IRIS Disable)
	LENS_WordWrite(SPI_AN41908_ADDR, 0x0b, mode);
#endif

	mode = (1<<13)|(28<<8)|(1<<0);		// PWMRES, PWMMODE, DT1
	LENS_WordWrite(SPI_AN41908_ADDR, 0x20, mode);

	mode = (1<<7)|(5<<0);				// TESTEN2, FZTEST
	LENS_WordWrite(SPI_AN41908_ADDR, 0x21, mode);

	mode = (0<<8)|(1<<0);				// PHMODAB, DT2A
	LENS_WordWrite(SPI_AN41908_ADDR, 0x22, mode);

	mode = (200<<8)|(200<<0);			// PPWB, PPWA
	LENS_WordWrite(SPI_AN41908_ADDR, 0x23, mode);

	zVal = (1<<12)|(0<<11)|(0<<10)|(0<<9)|(0<<8)|(0<<0);	// MICROAB, LEDA, ENDISAB, BREAKAB, CCWCWAB, PSUMAB
	LENS_WordWrite(SPI_AN41908_ADDR, 0x24, zVal);

	mode = (0<<8)|(1<<0);				// PHMODCD, DT2B
	LENS_WordWrite(SPI_AN41908_ADDR, 0x27, mode);

	mode = (200<<8)|(200<<0);			// PPWD, PPWC
	LENS_WordWrite(SPI_AN41908_ADDR, 0x28, mode);

	fVal = (1<<12)|(0<<11)|(0<<10)|(0<<9)|(0<<8)|(0<<0);	// MICROCD, LEDB, ENDISCD, BREAKCD, CCWCWCD, PSUMCD
	LENS_WordWrite(SPI_AN41908_ADDR, 0x29, fVal);

	mode = (296<<0);					// INTCTAB, 960pps
	LENS_WordWrite(SPI_AN41908_ADDR, 0x25, mode);	// zoom

	mode = (296<<0);					// INTCTCD, 960pps
	LENS_WordWrite(SPI_AN41908_ADDR, 0x2a, mode);	// foks
}

//--------------------------------------------------------------------------------------------------------------------------
static void LENS_SetPINTInfo(void)
{
#if defined(__USE_EEPROM__)
	EEPROM_MultiRead(EEPROM_PINT_ZOOM,  (PBYTE)&stZOOM.mMin, sizeof(LENS_PINT_INFO));
	EEPROM_MultiRead(EEPROM_PINT_FOCUS, (PBYTE)&stFOKS.mMin, sizeof(LENS_PINT_INFO));

	if(stZOOM.pint==-1 || stFOKS.pint==-1) 	{	// eeprom pint info valid check..
		UARTprintf("\r\n******************************************\r\n");
		UARTprintf(    " 'LENS pint info' in eeprom not valid !!! \r\n");
		UARTprintf(    "******************************************\r\n");

#if 0 //34220
		stZOOM.mMin = 12012;	stFOKS.mMin = 19397;
		stZOOM.mMax = 14796;	stFOKS.mMax = 27547;
		stZOOM.oMin = 12077;	stFOKS.oMin = 19497;
		stZOOM.oMax = 14752;	stFOKS.oMax = 27447;
		stZOOM.pint = 12077;	stFOKS.pint = 24286;
		stZOOM.knee = 14224;	stFOKS.knee = 20509;
#else //imx290
		stZOOM.mMin = 11996;    stFOKS.mMin = 19524;
		stZOOM.mMax = 14788;    stFOKS.mMax = 27386;
		stZOOM.oMin = 12068;   stFOKS.oMin = 19624;
		stZOOM.oMax = 14743;    stFOKS.oMax = 27286;
		stZOOM.pint = 12068;    stFOKS.pint = 24540;
		stZOOM.knee = 14215;    stFOKS.knee = 20763;
#endif
	}
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetInstancePARAM(void)
{
	LENSDRV_Initialize();		// drive chip initialize
	LENS_SetPINTInfo();			// set pint information

	LENS_ResetPosition();		// find reset position
//	LENS_LimitPosition();		// for moving distance check
	LENS_OriginPosition();		// move pint position
}

//--------------------------------------------------------------------------------------------------------------------------
PLENS_PINT_INFO LENS_GetPINTInfoZOOM(void)
{
	return (PLENS_PINT_INFO)&stZOOM.mMin;
}

//--------------------------------------------------------------------------------------------------------------------------
PLENS_PINT_INFO LENS_GetPINTInfoFOKS(void)
{
	return (PLENS_PINT_INFO)&stFOKS.mMin;
}

//--------------------------------------------------------------------------------------------------
static void LENS_WaitDriveMOTOR(void)
{
	while (fZoomDrvCompleted==FALSE); fZoomDriveTrigger = OFF;
	while (fFoksDrvCompleted==FALSE); fFoksDriveTrigger = OFF;
}

//--------------------------------------------------------------------------------------------------
static void LENS_GetADcnvMOTOR(void)
{
	stZOOM.ADcv = GET_LENS_ZRST();
	stFOKS.ADcv = GET_LENS_FRST();
}
/*
//--------------------------------------------------------------------------------------------------
static void DrvMOVE_WIDE_NEAR(SHORT step, SHORT cnt)
{
	if (fZoomZeroNotFound||fFoksZeroNotFound) return;

	LENS_SetDirectionZOOM(zWIDE);	LENS_SetDirectionFOKS(fNEAR);
	LENS_XferDriveMOTOR();	Delay_mSec(4);

	while (cnt) {
		LENS_SetDriveZOOM(stZOOM.cur-step/1);
		LENS_SetDriveFOKS(stFOKS.cur-step/4);

		LENS_XferDriveMOTOR();	LENS_WaitDriveMOTOR();
		Delay_mSec(4);	cnt--;	LENS_GetADcnvMOTOR();
	}
}

//--------------------------------------------------------------------------------------------------
static void DrvMOVE_WIDE_FAR(SHORT step, SHORT cnt)
{
	if (fZoomZeroNotFound||fFoksZeroNotFound) return;

	LENS_SetDirectionZOOM(zWIDE);	LENS_SetDirectionFOKS(fFAR);
	LENS_XferDriveMOTOR();	Delay_mSec(4);

	while (cnt) {
		LENS_SetDriveZOOM(stZOOM.cur-step/1);
		LENS_SetDriveFOKS(stFOKS.cur+step/4);

		LENS_XferDriveMOTOR();	LENS_WaitDriveMOTOR();
		Delay_mSec(4);	cnt--;	LENS_GetADcnvMOTOR();
	}
}

//--------------------------------------------------------------------------------------------------
static void DrvMOVE_TELE_NEAR(SHORT step, SHORT cnt)
{
	if (fZoomZeroNotFound||fFoksZeroNotFound) return;

	LENS_SetDirectionZOOM(zTELE);	LENS_SetDirectionFOKS(fNEAR);
	LENS_XferDriveMOTOR();	Delay_mSec(4);

	while (cnt) {
		LENS_SetDriveZOOM(stZOOM.cur+step/1);
		LENS_SetDriveFOKS(stFOKS.cur-step/4);

		LENS_XferDriveMOTOR();	LENS_WaitDriveMOTOR();
		Delay_mSec(4);	cnt--;	LENS_GetADcnvMOTOR();
	}
}
*/
//--------------------------------------------------------------------------------------------------
static void DrvMOVE_TELE_FAR(SHORT step, SHORT cnt)
{
	if (fZoomZeroNotFound||fFoksZeroNotFound) return;

	LENS_SetDirectionZOOM(zTELE);	LENS_SetDirectionFOKS(fFAR);
	LENS_XferDriveMOTOR();	Delay_mSec(4);

	while (cnt) {
		LENS_SetDriveZOOM(stZOOM.cur+step/1);
		LENS_SetDriveFOKS(stFOKS.cur+step/4);

		LENS_XferDriveMOTOR();	LENS_WaitDriveMOTOR();
		Delay_mSec(4);	cnt--;	LENS_GetADcnvMOTOR();
	}
}

//--------------------------------------------------------------------------------------------------
static void DrvZERO_WIDE_NEAR(SHORT zStep, SHORT fStep)
{
	if (fZoomZeroNotFound||fFoksZeroNotFound) return;

	LENS_SetDirectionZOOM(zWIDE);	LENS_SetDirectionFOKS(fNEAR);
	LENS_XferDriveMOTOR();	Delay_mSec(4);

	while (zStep || fStep) { // find infection point from (tele(0),far(1))
		LENS_SetDriveZOOM(stZOOM.cur-zStep);
		LENS_SetDriveFOKS(stFOKS.cur-fStep);

		LENS_XferDriveMOTOR();	LENS_WaitDriveMOTOR();
		Delay_mSec(4);			LENS_GetADcnvMOTOR();

		if (stZOOM.ADcv==1)	zStep = 0;
		if (stFOKS.ADcv==0) fStep = 0;
	}
}

//--------------------------------------------------------------------------------------------------
static void DrvZERO_WIDE_FAR(SHORT zStep, SHORT fStep)
{
	if (fZoomZeroNotFound||fFoksZeroNotFound) return;

	LENS_SetDirectionZOOM(zWIDE);	LENS_SetDirectionFOKS(fFAR);
	LENS_XferDriveMOTOR();	Delay_mSec(4);	// direction confirm

	while (zStep || fStep) {
		LENS_SetDriveZOOM(stZOOM.cur-zStep);
		LENS_SetDriveFOKS(stFOKS.cur+fStep);

		LENS_XferDriveMOTOR();	LENS_WaitDriveMOTOR();
		Delay_mSec(4);			LENS_GetADcnvMOTOR();

		if (stZOOM.ADcv==1)	zStep = 0;
		if (stFOKS.ADcv==1) fStep = 0;
	}
}

//--------------------------------------------------------------------------------------------------
static void DrvZERO_TELE_NEAR(SHORT zStep, SHORT fStep)
{
	if (fZoomZeroNotFound||fFoksZeroNotFound) return;

	LENS_SetDirectionZOOM(zTELE);	LENS_SetDirectionFOKS(fNEAR);
	LENS_XferDriveMOTOR();	Delay_mSec(4);

	while (zStep || fStep) {
		LENS_SetDriveZOOM(stZOOM.cur+zStep);
		LENS_SetDriveFOKS(stFOKS.cur-fStep);

		LENS_XferDriveMOTOR();	LENS_WaitDriveMOTOR();
		Delay_mSec(4);			LENS_GetADcnvMOTOR();

		if (stZOOM.ADcv==0)	zStep = 0;
		if (stFOKS.ADcv==0) fStep = 0;
	}
}

//--------------------------------------------------------------------------------------------------
static void DrvZERO_TELE_FAR(SHORT zStep, SHORT fStep)
{
	if (fZoomZeroNotFound||fFoksZeroNotFound) return;

	LENS_SetDirectionZOOM(zTELE);	LENS_SetDirectionFOKS(fFAR);
	LENS_XferDriveMOTOR();	Delay_mSec(4);

	while (zStep || fStep) {  // find infection point from (wide(1),near(0))
		LENS_SetDriveZOOM(stZOOM.cur+zStep);
		LENS_SetDriveFOKS(stFOKS.cur+fStep);

		LENS_XferDriveMOTOR();	LENS_WaitDriveMOTOR();
		Delay_mSec(4);			LENS_GetADcnvMOTOR();

		if (stZOOM.ADcv==0)	zStep = 0;
		if (stFOKS.ADcv==1) fStep = 0;
	}
}

//--------------------------------------------------------------------------------------------------
static void DrvTRIG_WIDE_NEAR(void) // from position = tele,far to zero
{
	DrvZERO_WIDE_NEAR(12, 6);	// step(12,6)
	DrvZERO_TELE_FAR (12, 6);	// step(12,6)
	DrvMOVE_TELE_FAR (12, 3);	// step(12,13), cnt(3)
	DrvZERO_WIDE_NEAR( 1, 1);	// step(1,1)
}

//--------------------------------------------------------------------------------------------------
static void DrvTRIG_WIDE_FAR(void)
{
	DrvZERO_WIDE_FAR (12, 6);	DrvZERO_WIDE_NEAR( 0, 6);
	DrvZERO_TELE_FAR (12, 6);	DrvMOVE_TELE_FAR (12, 3);
	DrvZERO_WIDE_NEAR( 1, 1);
}

//--------------------------------------------------------------------------------------------------
static void DrvTRIG_TELE_NEAR(void)
{
	DrvZERO_TELE_NEAR(12, 6);	DrvZERO_WIDE_NEAR(12, 0);
	DrvZERO_TELE_FAR (12, 6);	DrvMOVE_TELE_FAR (12, 3);
	DrvZERO_WIDE_NEAR( 1, 1);
}

//--------------------------------------------------------------------------------------------------
static void DrvTRIG_TELE_FAR(void)
{
	DrvZERO_TELE_FAR (12, 6);
	DrvMOVE_TELE_FAR (12, 3);
	DrvZERO_WIDE_NEAR( 1, 1);
}

//--------------------------------------------------------------------------------------------------
void LENS_ZeroPosition(void) // move to wide&near
{
	fFoksDrvLimitMode = OFF;	fZoomDrvLimitMode = OFF;
	fZoomDrvTrackMode = OFF;	fZoomDriveDigital = OFF;
	fZoomDrvStepRatio = x1M;	fLensMechPosition = ON;

	LENS_GetADcnvMOTOR();		// get MOTOR-ADcv value

	if		(stZOOM.ADcv==0&&stFOKS.ADcv==1) DrvTRIG_WIDE_NEAR(); // cur position = tele,far
	else if (stZOOM.ADcv==0&&stFOKS.ADcv==0) DrvTRIG_WIDE_FAR (); // cur position = tele,near
	else if (stZOOM.ADcv==1&&stFOKS.ADcv==1) DrvTRIG_TELE_NEAR(); // cur position = wide,far
	else									 DrvTRIG_TELE_FAR (); // cur position = wide,near

	fZoomDrvStepRatio = x4M;	fLensMechPosition = ON;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_ResetPosition(void)
{
	LENS_EnableMotorZOOM(ON);		LENS_EnableMotorFOKS(ON);
	LENS_XferDriveMOTOR();			Delay_mSec(34);

	LENS_SetCurPositionZOOM(zeroZoom);	LENS_SetCurPositionFOKS(zeroFocus);
	LENS_ZeroPosition();			Delay_mSec(10);
}

//--------------------------------------------------------------------------------------------------------------------------
static SHORT LENS_SetORGPositionZOOM(SHORT val)
{
	SHORT max = LENS_GetMaxPositionZOOM();
	SHORT min = LENS_GetMinPositionZOOM();

	stZOOM.goal = (fLensKeepPosition)? val : stZOOM.pint;
	stZOOM.goal = MAX(MIN(stZOOM.goal, max), min);

	if (stZOOM.cur>stZOOM.goal) fZoomDrvDirection = zWIDE;
	if (stZOOM.cur<stZOOM.goal) fZoomDrvDirection = zTELE;
	return stZOOM.goal;
}

//--------------------------------------------------------------------------------------------------------------------------
static SHORT LENS_SetORGPositionFOKS(SHORT val)
{
	SHORT max = LENS_GetMAXPositionLOCI(stZOOM.goal);
	SHORT min = LENS_GetMINPositionLOCI(stZOOM.goal);

	stFOKS.goal = (fLensKeepPosition)? val : stFOKS.pint;
	stFOKS.goal = MAX(MIN(stFOKS.goal, max), min);

	if (stFOKS.cur>stFOKS.goal) fFoksDrvDirection = fNEAR;
	if (stFOKS.cur<stFOKS.goal) fFoksDrvDirection = fFAR;
	return stFOKS.goal;
}

//--------------------------------------------------------------------------------------------------
void LENS_OriginPosition(void)
{
	SHORT zVal, fVal;

	if (fZoomZeroNotFound||fFoksZeroNotFound) return;

	fLensKeepPosition = ON;		fLensSavePosition = OFF;
	fFoksDrvLimitMode = ON;		fZoomDrvLimitMode = ON;
	fZoomDrvTrackMode = OFF;	fZoomDriveDigital = OFF;
	fZoomDrvStepRatio = x4M;	fLensMechPosition = OFF;

	LENS_SetCurPositionZOOM(zeroZoom);	LENS_SetCurPositionFOKS(zeroFocus);

	zVal = LENS_SetORGPositionZOOM(stZOOM.pint);
	fVal = LENS_SetORGPositionFOKS(stFOKS.pint);

	LENS_XferDriveMOTOR();		Delay_mSec(17);
	LENS_SetDriveZOOM(zVal);	LENS_SetDriveFOKS(fVal);

	while (!fFoksDrvCompleted||!fZoomDrvCompleted) {
		LENS_XferDriveMOTOR();	Delay_mSec(17);
	}
	fLensSavePosition = ON;		Delay_mSec(17);

	LENS_EnableMotorZOOM(OFF);	LENS_EnableMotorFOKS(OFF);
	LENS_XferDriveMOTOR();		Delay_mSec(17);

	LENS_SetTrackingINFO((SHORT)stFOKS.cur-0);	// set inf locus
}       

//--------------------------------------------------------------------------------------------------------------------------
void ISPM LENS_EnableMotorZOOM(BOOL OnOff)	
{
	if (OnOff==ON)	stZOOM.xfer |= 0x0400;
	else			stZOOM.xfer &= 0xfbff;

	if (OnOff==OFF) fZoomDriveTrigger = OFF;
	if (OnOff==OFF) fZoomDrvTrackMode = OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM LENS_EnableMotorFOKS(BOOL OnOff)	
{
	if (OnOff==ON)	stFOKS.xfer |= 0x0400;
	else			stFOKS.xfer &= 0xfbff;

	if (OnOff==OFF) fFoksDriveTrigger = OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_EnableTrackZOOM(BOOL OnOff)
{
	fZoomDrvTrackMode = (OnOff==ON)? ON : OFF;
}

//--------------------------------------------------------------------------------------------------
void LENS_EnableSpeedZOOM(BOOL OnOff)
{
	fZoomDrvSpeedMode = (OnOff==ON)? ON : OFF;
}

//--------------------------------------------------------------------------------------------------
void LENS_EnableSpeedFOKS(BOOL OnOff)
{
	fFoksDrvSpeedMode = (OnOff==ON)? ON : OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM LENS_ForceBreakZOOM(BOOL OnOff)
{
	if (OnOff==ON) fZoomDriveTrigger = OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM LENS_ForceBreakFOKS(BOOL OnOff)
{
	if (OnOff==ON) fFoksDriveTrigger = OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetDirectionZOOM(BOOL dir)
{
	fZoomDrvDirection = (dir==zWIDE)? zWIDE : zTELE;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetDirectionFOKS(BOOL dir)
{
	fFoksDrvDirection = (dir==fNEAR)? fNEAR : fFAR;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetDriveZOOM(SHORT val)
{
	stZOOM.goal = val;
	fZoomDriveTrigger = ON;
	fZoomDrvCompleted = FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM LENS_SetDriveFOKS(SHORT val)
{
	stFOKS.goal = val;
	fFoksDriveTrigger = ON;
	fFoksDrvCompleted = FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL ISPM LENS_IsDriveEnableZOOM(void)
{
	return (stZOOM.xfer&0x0400)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL ISPM LENS_IsDriveEnableFOKS(void)
{
	return (stFOKS.xfer&0x0400)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL ISPM LENS_IsDriveCompleteZOOM(void)
{
	return (fZoomDrvCompleted)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL ISPM LENS_IsDriveCompleteFOKS(void)
{
	return (fFoksDrvCompleted)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL LENS_GetDirectionZOOM(void)
{
	return (fZoomDrvDirection==zWIDE)? zWIDE : zTELE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL LENS_GetDirectionFOKS(void)
{
	return (fFoksDrvDirection==fNEAR)? fNEAR : fFAR;
}

//--------------------------------------------------------------------------------------------------
WORD LENS_GetTrackingLOCI(void)
{
	return wRefID;
}

//--------------------------------------------------------------------------------------------------
LONG LENS_GetTrackingRATE(void)
{
	return (LONG)(fRatio*1024);
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetCurPositionZOOM(void)
{
	return stZOOM.cur;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetCurPositionFOKS(void)
{
	return stFOKS.cur;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT ISPM LENS_GetRelPositionZOOM(void)
{
	SHORT oMAX = stZOOM.oMin+stLENS.tele;
	return MIN(stZOOM.cur,oMAX)-stZOOM.pint;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT ISPM LENS_GetRelMaxPositionZOOM(void)
{
	SHORT oMAX;

	if (fZoomDigitalOnOff==OFF || fZoomDigiMixOnOff==OFF)
		 oMAX = stZOOM.oMin+stLENS.tele-stZOOM.pint;
	else oMAX = stZOOM.oMin+stLENS.tele+stLENS.digi-stZOOM.pint;

	return oMAX;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetRelPositionFOKS(void)
{
	return stFOKS.cur-(stFOKS.pint-zeroFocus);
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT ISPM LENS_GetCurPositionDZOOM(void)
{
	if (fZoomDriveDigital==OFF) return 0;
	return stZOOM.cur-(stZOOM.oMin+stLENS.tele);
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetMaxPositionZOOM(void)
{
	if (fZoomDigitalOnOff==OFF || fZoomDigiMixOnOff==OFF)
		 stZOOM.tMax = stZOOM.oMin+stLENS.tele;
	else stZOOM.tMax = stZOOM.oMin+stLENS.tele+stLENS.digi;

	return (fLensMechPosition)? stZOOM.mMax : stZOOM.tMax;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetMaxPositionFOKS(void)
{
	return (fLensMechPosition)? stFOKS.mMax : stFOKS.oMax;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetMinPositionZOOM(void)
{
	return (fLensMechPosition)? stZOOM.mMin : stZOOM.oMin+stLENS.wide;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetMinPositionFOKS(void)
{
	return (fLensMechPosition)? stFOKS.mMin : stFOKS.oMin;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetPintPositionZOOM(void)
{
	return stZOOM.pint;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetPintPositionFOKS(void)
{
	return stFOKS.pint;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetKneePositionZOOM(void)
{
	return stZOOM.knee-stZOOM.pint;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetKneePositionFOKS(void)
{
	return stFOKS.pint-stFOKS.cur;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetVISCAPositionZOOM(void)
{
	LONG zMin = LENS_GetMinPositionZOOM();
	LONG zMax = stZOOM.oMin+stLENS.tele;
	
	if (stZOOM.cur<=(SHORT)zMax) {
		LONG zCnv = (zMax-stZOOM.cur)*0x4000;
		return (SHORT)(0x4000-(FLOAT)zCnv/(zMax-zMin)+0.5);
	}
	else {
		LONG zCur = LENS_GetCurPositionDZOOM();
		LONG zCnv = (stLENS.digi-zCur)*0x3AC0;
		return (SHORT)(0x7AC0-(FLOAT)zCnv/stLENS.digi+0.5);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetVISCAPositionFOKS(void)
{
	LONG  fMIN, fMAX, fCNV, fABS;
	SHORT i, zID, rel = stFOKS.pint-zeroFocus;
	SHORT oMAX = stZOOM.oMin+stLENS.tele;

	zID = MIN(MAX(stZOOM.cur,stZOOM.oMin),oMAX)-stZOOM.pint;

	for (i=1; i<sizeof(LENS_LimitByFOKS)/2-1; i++) {
		if (stFOKS.cur>=LENS_LocusByZOOM[zID][i]+rel) break;
	}

	fMIN = MAX(LENS_LocusByZOOM[zID][i-0]+rel, stFOKS.oMin);
	fMAX = MIN(LENS_LocusByZOOM[zID][i-1]+rel, stFOKS.oMax);
	fABS = ABS(LENS_LimitByFOKS[i]-LENS_LimitByFOKS[i-1]);

	fCNV = (fMIN-stFOKS.cur)*fABS;
	fCNV = (LONG)(LENS_LimitByFOKS[i]+(FLOAT)fCNV/(fMAX-fMIN)+0.5);
	return MIN(MAX((WORD)fCNV,AFFOKS_LIMIT_FAR),(WORD)stLENS.near); //stLENS.near (normal value is 0xd000)
}

//--------------------------------------------------------------------------------------------------------------------------
static SHORT LENS_CnvVISCAtoREAL_FOKS(SHORT zVal, SHORT fVal) //fVal = stLENS.near or fMIN (normal value is 0xd000)
{
	LONG  fMIN, fMAX, fCNV, fABS;
	SHORT i, zID, rel = stFOKS.pint-zeroFocus; //24300(nomal value) - 24000
	SHORT oMAX = stZOOM.oMin+stLENS.tele; // stLENS.tele = difference btw min&max

	for (i=1; i<sizeof(LENS_LimitByFOKS)/2-1; i++) { // LENS_LimitByFOKS = 0x1000...
		if ((WORD)fVal<=LENS_LimitByFOKS[i]) break; // find i
	}

	zID  = MIN(MAX(zVal, stZOOM.oMin), oMAX)-stZOOM.pint; // if zID = 0
	fMIN = MAX(LENS_LocusByZOOM[zID][i-0]+rel, stFOKS.oMin); //24000+300
	fMAX = MIN(LENS_LocusByZOOM[zID][i-1]+rel, stFOKS.oMax); //24012+300
	fABS = ABS(LENS_LimitByFOKS[i-1]-LENS_LimitByFOKS[i]); // 0x1000(nomal value)

	fCNV = (LONG)(LENS_LimitByFOKS[i]-(WORD)fVal)*(fMAX-fMIN); //fVal = 0xe000
	return (SHORT)(fMIN+(FLOAT)fCNV/fABS+0.0);
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetMIDPositionLOCI(void)
{
	SHORT zVal = LENS_GetRelPositionZOOM();
	LONG  fInf = (LONG)LENS_LocusByZOOM[zVal][1];
	LONG  fLow = (LONG)LENS_LocusByZOOM[zVal][7];
	LONG  fRel = (LONG)stFOKS.pint-zeroFocus;

	return (SHORT)((fInf+fLow)/2+fRel);
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetGAPPositionLOCI(void)
{
	SHORT zVal = LENS_GetRelPositionZOOM();
	SHORT fInf = (SHORT)LENS_LocusByZOOM[zVal][1];
	SHORT fLow = (SHORT)LENS_LocusByZOOM[zVal][7];

	return (SHORT)ABS(fInf-fLow);
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT ISPM LENS_GetMAXPositionLOCI(SHORT zVal)
{
	SHORT fVal, rel = stFOKS.pint-zeroFocus;
	SHORT oMAX = stZOOM.oMin+stLENS.tele;

	zVal = MIN(MAX(zVal,stZOOM.oMin),oMAX)-stZOOM.pint; // calculate zVal
	fVal = MIN(LENS_LocusByZOOM[zVal][0]+rel, stFOKS.oMax); // calculate fVal from locus 
	return (fLensMechPosition)? stFOKS.mMax : fVal;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT ISPM LENS_GetMINPositionLOCI(SHORT zVal)
{
	SHORT fMIN = LENS_CnvVISCAtoREAL_FOKS(zVal, stLENS.near); // stLENS.near = min of focus
	return (fLensMechPosition)? stFOKS.mMin : fMIN;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetABVPositionLOCI(BYTE nID)
{
	SHORT zVal = LENS_GetRelPositionZOOM();
	SHORT fRel = (SHORT)stFOKS.pint-zeroFocus;

	nID = (nID==0 || nID==9)? 0 : nID-1;

	return (SHORT)LENS_LocusByZOOM[zVal][nID]+fRel;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetBLOPositionLOCI(BYTE nID)
{
	SHORT zVal = LENS_GetRelPositionZOOM();
	SHORT fRel = (SHORT)stFOKS.pint-zeroFocus;

	nID = (nID==0 || nID==9)? 0 : nID+1;

	return (SHORT)LENS_LocusByZOOM[zVal][nID]+fRel;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetCurPositionZOOM(SHORT val)
{
	stZOOM.cur = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetCurPositionFOKS(SHORT val)
{
	stFOKS.cur = val;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_CnvVISCAPositionZOOM(SHORT val)
{
	LONG zMin = LENS_GetMinPositionZOOM();
	LONG zMax = stZOOM.oMin+stLENS.tele;

	if (fZoomDigitalOnOff==ON)	// check max
		 {val = (SHORT)MIN((WORD)val,0x7ac0);}
	else {val = (SHORT)MIN((WORD)val,0x4000);}

	if ((WORD)val<=0x4000) {
		LONG zCnv = (0x4000-val)*(zMax-zMin);
		return (SHORT)(zMax-(FLOAT)zCnv/0x4000+0.5);
	}
	else {
		LONG zCur = LENS_GetMaxPositionZOOM();
		LONG zCnv = (0x7AC0-val)*stLENS.digi;
		return (SHORT)(zCur-(FLOAT)zCnv/0x3AC0+0.5);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_CnvVISCAPositionFOKS(SHORT val)
{
	SHORT fMIN = LENS_CnvVISCAtoREAL_FOKS(stZOOM.cur, stLENS.near);

	return MAX(LENS_CnvVISCAtoREAL_FOKS(stZOOM.cur, val), fMIN);
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_CnvVISCAPositionLOCI(SHORT zVal, SHORT fVal)
{
	SHORT zVAL = LENS_CnvVISCAPositionZOOM(zVal);
	SHORT fMIN = LENS_CnvVISCAtoREAL_FOKS(zVAL, stLENS.near);

	return MAX(LENS_CnvVISCAtoREAL_FOKS(zVAL, fVal), fMIN);
}

//--------------------------------------------------------------------------------------------------
void ISPM LENS_CtrlTimeCounter(void)
{
	if (wDrvTime<9*GET_FPS) wDrvTime++;
}

//--------------------------------------------------------------------------------------------------
WORD LENS_GetDrvTimeCount(void)
{
	return wDrvTime;
}

//--------------------------------------------------------------------------------------------------
void LENS_SetDrvTimeCount(WORD cnt)
{
	wDrvTime = cnt;
}

//--------------------------------------------------------------------------------------------------
static void ISPM LENS_SetDigitalModeZOOM(void)
{
	SHORT oMAX = stZOOM.oMin+stLENS.tele;
	if (fZoomDrvDirection==zWIDE) oMAX++;

	if (fZoomDrvLimitMode==ON && stZOOM.cur>=oMAX)
			fZoomDriveDigital = ON;
	else	fZoomDriveDigital = OFF;

	if (!fZoomDigiMixOnOff || !fZoomDigitalOnOff || fLensMechPosition)
			fZoomDriveDigital = OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
static SHORT ISPM LENS_GetDigitalStepZOOM(void)
{
	SHORT zVal = LENS_GetCurPositionDZOOM();
	SHORT zID = (fZoomDrvSpeedMode)? stLENS.z_ID : 7;

	if (fZoomDrvTrackMode==OFF) return 1;	dZOOMCnt++;

	if		(zVal<31) {
		if (dZOOMCnt<LENS_SpeedByDZOOM[zID][0]) return 0;
			dZOOMCnt = 0;	return 1;
	}
	else if (zVal<52) {
		if (dZOOMCnt<LENS_SpeedByDZOOM[zID][1]) return 0;
			dZOOMCnt = 0;	return 1;
	}
	else {
		if (dZOOMCnt<LENS_SpeedByDZOOM[zID][2]) return 0;
			dZOOMCnt = 0;	return 1;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static SHORT ISPM LENS_GetOpticalStepZOOM(void)
{
	SHORT zVal = LENS_GetRelPositionZOOM();
	SHORT zID = (fZoomDrvSpeedMode)? stLENS.z_ID : 7;

	if		(fZoomDrvTrackMode==OFF)			return 15;
	else if (zVal < LENS_SpeedByZOOM[zID][ 0])	return 15;
	else if (zVal < LENS_SpeedByZOOM[zID][ 1])	return 14;
	else if (zVal < LENS_SpeedByZOOM[zID][ 2])	return 13;
	else if (zVal < LENS_SpeedByZOOM[zID][ 3])	return 12;
	else if (zVal < LENS_SpeedByZOOM[zID][ 4])	return 11;
	else if (zVal < LENS_SpeedByZOOM[zID][ 5])	return 10;
	else if (zVal < LENS_SpeedByZOOM[zID][ 6])	return  9;
	else if (zVal < LENS_SpeedByZOOM[zID][ 7])	return  8;
	else if (zVal < LENS_SpeedByZOOM[zID][ 8])	return  7;
	else if (zVal < LENS_SpeedByZOOM[zID][ 9])	return  6;
	else if (zVal < LENS_SpeedByZOOM[zID][10])	return  5;
	else if (zVal < LENS_SpeedByZOOM[zID][11])	return  4;
	else if (zVal < LENS_SpeedByZOOM[zID][12])	return  3;
	else if (zVal < LENS_SpeedByZOOM[zID][13])	return  2;
	else										return  1;
}

//--------------------------------------------------------------------------------------------------------------------------
static SHORT ISPM LENS_GetStepCountZOOM(void)
{
	SHORT max;

	if (fZoomDriveTrigger==FALSE) return 0;

	LENS_SetDigitalModeZOOM();

	if (fZoomDriveDigital)	max = LENS_GetDigitalStepZOOM();
	else					max = LENS_GetOpticalStepZOOM();

	return MIN(DIFF(stZOOM.cur, stZOOM.goal), max);
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM LENS_ConfigDriveZOOM(void)
{
	SHORT step = LENS_GetStepCountZOOM();
	SHORT n = (fZoomDrvStepRatio)? 1 : 4;

	if (fZoomDrvDirection==zWIDE) stZOOM.cur -= step;
	else						  stZOOM.cur += step;

	stZOOM.xfer &= 0xfe00;	stZOOM.xfer |= (n*step);
	if (fZoomDriveDigital)	stZOOM.xfer &= (0xfe00);

	if (fZoomDrvDirection==zWIDE) stZOOM.xfer |= 0x0100;
	else						  stZOOM.xfer |= 0x0000;

	if (fZoomDriveTrigger==FALSE) fZoomDrvCompleted = TRUE;
	if (stZOOM.cur==stZOOM.goal) fZoomDrvCompleted = TRUE;
	if (fZoomDrvCompleted==TRUE) fZoomDrvSpeedMode = OFF;
}

//--------------------------------------------------------------------------------------------------
static SHORT LENS_GetTrackingZOOM(BOOL dir)
{
	SHORT step = LENS_GetStepCountZOOM();
	SHORT oMAX = stZOOM.oMin+stLENS.tele;

	stZOOM.goal = (dir==zWIDE)? stZOOM.oMin : oMAX;
	return (dir==zWIDE)? stZOOM.cur-step : stZOOM.cur+step;
}

//--------------------------------------------------------------------------------------------------
static SHORT ISPM LENS_GetTrackingFOKS(SHORT zVal)	
{
	SHORT lGap, goal, rel = stFOKS.pint-zeroFocus;  //24300(nomal value) - 24000
	SHORT *pID, oMAX = stZOOM.oMin+stLENS.tele; // stLENS.tele = difference btw min&max

	zVal = MIN(MAX(zVal, stZOOM.oMin), oMAX);
	pID = (PSHORT)LENS_LocusByZOOM[zVal-stZOOM.pint]; // range of [zVal-stZOOM.pint] is 0~xxx

	lGap = (wRefID)? pID[wRefID-1]-pID[wRefID] : 0;
	goal = (SHORT)(lGap*fRatio+0.5)+pID[wRefID]+rel;

	goal = MIN(goal, LENS_GetMAXPositionLOCI(zVal));
	goal = MAX(goal, LENS_GetMINPositionLOCI(zVal));

	return goal;
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM LENS_ConfigTrackZOOM(void)
{
	if (fZoomDrvTrackMode==OFF) return;

	stFOKS.goal = LENS_GetTrackingFOKS(stZOOM.cur);
	LENS_SetDriveFOKS(stFOKS.goal);
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM LENS_IsDriveLockFOKS(void)
{
	if (stFOKS.cur>stFOKS.goal && fFoksDrvDirection== fFAR) {
		fFoksDriveLocking = ON;		fFoksDrvDirection ^= 1;
	}

	if (stFOKS.cur<stFOKS.goal && fFoksDrvDirection==fNEAR) {
		fFoksDriveLocking = ON;		fFoksDrvDirection ^= 1;
	}

	if (fFoksDriveLocking==ON&&(++wLockCnt<2)) return TRUE;
	wLockCnt = 0; fFoksDriveLocking = OFF;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static SHORT ISPM LENS_GetStepCountFOKS(void)
{
	SHORT max, xID;

	if (fFoksDriveTrigger==FALSE) return 0;
	if (LENS_IsDriveLockFOKS()) return 0;

	xID = (stZOOM.cur<stZOOM.knee)? 1 : 2;

	if (fFoksDrvSpeedMode==OFF)	 max = 60;
	else		max = xID*(stLENS.f_ID+1);

	return MIN(DIFF(stFOKS.cur, stFOKS.goal), max);
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM LENS_ConfigDriveFOKS(void)
{
	SHORT step = LENS_GetStepCountFOKS();

	if (fFoksDrvDirection==fNEAR) stFOKS.cur -= step;
	else						  stFOKS.cur += step;

	stFOKS.xfer &= 0xfe00;	stFOKS.xfer |= (1*step);

	if (fFoksDrvDirection==fNEAR) stFOKS.xfer |= 0x0000;
	else						  stFOKS.xfer |= 0x0100;

	if (fFoksDriveTrigger==FALSE) fFoksDrvCompleted = TRUE;
	if (stFOKS.cur==stFOKS.goal) fFoksDrvCompleted = TRUE;
	if (fFoksDrvCompleted==TRUE) fFoksDrvSpeedMode = OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM LENS_XferDriveMOTOR(void)
{
	LENS_ConfigDriveZOOM();
	LENS_ConfigTrackZOOM();
	LENS_ConfigDriveFOKS();

	LENS_VD(HIGH);	//LENS_VD(LOW);
	LENS_WordWrite(SPI_AN41908_ADDR, 0x24, stZOOM.xfer);
	LENS_WordWrite(SPI_AN41908_ADDR, 0x29, stFOKS.xfer);
	LENS_VD(LOW);
}

//--------------------------------------------------------------------------------------------------
void LENS_SetTrackingPEAK(SHORT val)
{
	stFOKS.peak = (val==0)? stFOKS.cur : val;
}

//--------------------------------------------------------------------------------------------------
void LENS_SetTrackingINFO(SHORT val)
{
	SHORT fGap, *pID, rel = stFOKS.pint-zeroFocus;
	SHORT zVal, oMAX = stZOOM.oMin+stLENS.tele; // stLENS.tele = zoom end position

	LENS_SetTrackingPEAK(val);	// set tracking-peak(stFOKS.peak)
	zVal = MIN(MAX(stZOOM.cur, stZOOM.oMin), oMAX);

	// (wRefID=0) is FAR, (wRefID=1) is INF
	pID = (PSHORT)LENS_LocusByZOOM[zVal-stZOOM.pint]; // change to pID (0~xxx)
	for (wRefID=1; wRefID<sizeof(LENS_LimitByFOKS)/2-1; wRefID++) { // find wRefID
		if (stFOKS.peak>=(pID[wRefID]+rel)) break;
	}

	fGap = stFOKS.peak-(pID[wRefID]+rel);
	fRatio = (FLOAT)fGap/(pID[wRefID-1]-pID[wRefID]); // calculate fRatio
}

//--------------------------------------------------------------------------------------------------
void LENS_SetTrackingZOOM(BOOL dir)
{
	SHORT zVal = LENS_GetTrackingZOOM(dir);
	SHORT fVal = LENS_GetTrackingFOKS(zVal);

	if (stFOKS.peak>fVal) fFoksDrvDirection = fNEAR;
	if (stFOKS.peak<fVal) fFoksDrvDirection = fFAR;
}

//--------------------------------------------------------------------------------------------------
void LENS_SetMechanicMODE(BOOL OnOff)
{
	fLensMechPosition = (OnOff==ON)? ON : OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_CurvePosition(void)			// drive inflection point
{
	LENS_SetCurPositionZOOM(zeroZoom);	LENS_SetCurPositionFOKS(zeroFocus);

	LENS_SetDirectionZOOM(zWIDE);	LENS_SetDirectionFOKS(fFAR);
	LENS_SetDriveZOOM(orgPintZoom); /*omin & pint*/		LENS_SetDriveFOKS(orgPintFocus); /*pint*/

	while (!fFoksDrvCompleted||!fZoomDrvCompleted) {
		LENS_XferDriveMOTOR();		Delay_mSec(17);
	}

	LENS_EnableMotorZOOM(OFF);		LENS_EnableMotorFOKS(OFF);
	LENS_XferDriveMOTOR();			Delay_mSec(17);

	fLensKeepPosition = ON;		fLensSavePosition = OFF;
	fFoksDrvLimitMode = ON;		fZoomDrvLimitMode = ON;
	fZoomDrvTrackMode = OFF;	fZoomDriveDigital = OFF;
	fZoomDrvStepRatio = x4M;	fLensMechPosition = OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_LimitPosition(void)
{
	LENS_SetCurPositionZOOM(zeroZoom);	LENS_SetCurPositionFOKS(zeroFocus);
	LENS_SetDirectionZOOM(zWIDE);	LENS_SetDirectionFOKS(fNEAR);
	LENS_SetDriveZOOM(limitWideNearZoom);		LENS_SetDriveFOKS(limitWideNearFocus); // move to limit of (wide, far) position

	while (!fFoksDrvCompleted||!fZoomDrvCompleted) {
		LENS_XferDriveMOTOR();		Delay_mSec(17);
	}

	LENS_EnableMotorZOOM(OFF);		LENS_EnableMotorFOKS(OFF);
	LENS_XferDriveMOTOR();			Delay_mSec(17);

	LENS_ResetPosition();			Delay_mSec(17); // find zero position

	stZOOM.cur  = zeroZoom+(stZOOM.cur-zeroZoom)/4;
	stZOOM.mMin = zeroZoom-(stZOOM.cur-zeroZoom)+marginZoom;
	stFOKS.mMin = zeroFocus-(stFOKS.cur-zeroFocus)+marginFocus;

	LENS_SetCurPositionZOOM(zeroZoom);	LENS_SetCurPositionFOKS(zeroFocus);
	LENS_SetDirectionZOOM(zTELE);	LENS_SetDirectionFOKS(fFAR);
	LENS_SetDriveZOOM(limitTeleFarZoom);		LENS_SetDriveFOKS(limitTeleFarFocus); // move to limit of (tele, far) position

	while (!fFoksDrvCompleted||!fZoomDrvCompleted) {
		LENS_XferDriveMOTOR();		Delay_mSec(17);		// motor real drive
	}

	LENS_EnableMotorZOOM(OFF);		LENS_EnableMotorFOKS(OFF);
	LENS_XferDriveMOTOR();			Delay_mSec(17);		// disable channel

	LENS_ResetPosition();			Delay_mSec(17);		// find zero position

	stZOOM.cur  = zeroZoom-(zeroZoom-stZOOM.cur)/4;
	stZOOM.mMax = zeroZoom+(zeroZoom-stZOOM.cur)-marginZoom;
	stFOKS.mMax = zeroFocus+(zeroFocus-stFOKS.cur)-marginFocus;
}

//--------------------------------------------------------------------------------------------------
void LENS_SetOpticalPINT(void)
{
	stZOOM.oMin = stZOOM.cur;		stFOKS.oMin = stFOKS.mMin+marginFocus;
	stZOOM.oMax = stZOOM.cur+diffMinMaxZoom;	stFOKS.oMax = stFOKS.mMax-marginFocus;
	stZOOM.pint = stZOOM.cur;		stFOKS.pint = stFOKS.cur;
	stZOOM.knee = stZOOM.cur+diffPintKneeZoom;	stFOKS.knee = stFOKS.cur-diffPintKneeFocus;

	UARTprintf("stZOOM.mMin = %d;	stFOKS.mMin = %d;\r\n", stZOOM.mMin, stFOKS.mMin);
	UARTprintf("stZOOM.mMax = %d;	stFOKS.mMax = %d;\r\n", stZOOM.mMax, stFOKS.mMax);
	UARTprintf("stZOOM.oMin = %d;	stFOKS.oMin = %d;\r\n", stZOOM.oMin, stFOKS.oMin);
	UARTprintf("stZOOM.oMax = %d;	stFOKS.oMax = %d;\r\n", stZOOM.oMax, stFOKS.oMax);
	UARTprintf("stZOOM.pint = %d;	stFOKS.pint = %d;\r\n", stZOOM.pint, stFOKS.pint);
	UARTprintf("stZOOM.knee = %d;	stFOKS.knee = %d;\r\n", stZOOM.knee, stFOKS.knee);

	EEPROM_RegWrite(EEPROM_PINT_LENSNO,  EEPROM_PINT_DONE);
	EEPROM_MultiWrite(EEPROM_PINT_ZOOM,  (PBYTE)&stZOOM.mMin, sizeof(LENS_PINT_INFO));
	EEPROM_MultiWrite(EEPROM_PINT_FOCUS, (PBYTE)&stFOKS.mMin, sizeof(LENS_PINT_INFO));

	UARTprintf("[PINT] The Calibration data writes to EEPROM!!!\r\n");
}

//--------------------------------------------------------------------------------------------------
SHORT LENS_GetPINTDrvSTEP(BOOL mode)
{
	return (mode==fPINT)? 2 : 1;
}

//--------------------------------------------------------------------------------------------------
SHORT LENS_GetPINTDrvGOAL(BOOL mode)
{
	return (mode==fPINT)? diffWideTeleFocus : diffMinMaxZoom;
		// diffWideTeleFocus = focus difference btw wide focus to tele focus value
		// diffMinMaxZoom = zoom difference btw zoom min and zoom max
}

//--------------------------------------------------------------------------------------------------
SHORT LENS_GetPINTDrvRANGE(BOOL mode)
{
	return (mode==fPINT)? 256 : 64;
}

//--------------------------------------------------------------------------------------------------
BYTE LENS_GetAFDriveSTEP(BYTE nID)
{
	SHORT nSTD, fABS = LENS_GetGAPPositionLOCI();

	if (stZOOM.cur<=stZOOM.knee) {	nSTD = MIN(MAX(fABS/16, 3), 13);
		switch (nID) {
			case DRV_STEP_MIN: return MIN(MIN(MAX(fABS/ 32, 2),  6), (nSTD+1)/2);
			case DRV_STEP_STD: return MIN(MIN(MAX(fABS/ 16, 3), 13), (nSTD+0)*1);
			case DRV_STEP_MID: return MIN(MIN(MAX(fABS/  8, 4), 26), (nSTD+0)*2);
			case DRV_STEP_MAX: return MIN(MIN(MAX(fABS/  5, 5), 39), (nSTD+0)*3);
			case DRV_STEP_FWD: return MIN(MIN(MAX(fABS/  4, 6), 52), (nSTD+0)*4);

			case DRV_STEP_BWD: return 60;
			case DRV_STEP_TOP: return MIN(MAX(fABS/ 64, 2), 3);
		}
	}
	else {							nSTD = MIN(MAX(fABS/64, 3), 15);
		switch (nID) {
			case DRV_STEP_MIN: return MIN(MIN(MAX(fABS/ 96, 2),  8), (nSTD+1)/2);
			case DRV_STEP_STD: return MIN(MIN(MAX(fABS/ 48, 3), 15), (nSTD+0)*1);
			case DRV_STEP_MID: return MIN(MIN(MAX(fABS/ 24, 4), 30), (nSTD+0)*2);
			case DRV_STEP_MAX: return MIN(MIN(MAX(fABS/ 16, 5), 45), (nSTD+0)*3);
			case DRV_STEP_FWD: return MIN(MIN(MAX(fABS/ 12, 6),120), (nSTD+0)*8);

			case DRV_STEP_BWD: return 60;
			case DRV_STEP_TOP: return MIN(MAX(fABS/192, 2), 4);
		}
	}
	return nSTD;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetFOKSDrvLIMIT(PAF_CTRL_PARAM pCTRL, SHORT val)
{
	WORD fVal = (WORD)(val&0xff00);

	if (GetDrvOnOffPINT() || GetAgeOnOffLENS()) return;
	if (AF_IsProcessExecuting()) AF_EnableProcess(OFF);

	fVal = MIN(MAX(fVal,AFFOKS_LIMIT_INF),AFFOKS_LIMIT_0p1M);
	pCTRL->near = (SHORT)fVal;	stLENS.near = (SHORT)fVal;	//static ROMDATA WORD LENS_LimitByFOKS[]			= {
														//    0x1000, 0x2000, 0x3000, 0x4000, 0x6000, 0x8000, 0xa000, 0xc000, 0xd000, 0xe000};


	if (stFOKS.cur<LENS_GetMINPositionLOCI(stZOOM.cur))
		 stLENS.fCTL = PRE_LENS_GOAL;
	else AF_EnableProcess(ON);
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetFOKSDrvSPEED(PAF_CTRL_PARAM pCTRL, LONG val)
{
	pCTRL->ctrl &= (~AFFOKS_SPEED_MASK);
	pCTRL->ctrl |= val;	stLENS.f_ID = (SHORT)(val>>8);
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetWIDEDrvLIMIT(PAF_CTRL_PARAM pCTRL, SHORT val)
{
	FLOAT zCnv = 325.0f-((255.0f-val)/255)*325+0.5; // 0 is min

	if (GetDrvOnOffPINT() || GetAgeOnOffLENS()) return;
	if (AF_IsProcessExecuting()) AF_EnableProcess(OFF);
	LENS_SetDIGIDrvOnOff(pCTRL, OFF);	// DZOOM off

	stLENS.wide = pCTRL->wide = (SHORT)zCnv;
	stLENS.zTGT = stLENS.wide + stZOOM.pint;

	if (stZOOM.cur<stLENS.zTGT)	stLENS.zCTL = PRE_LENS_GOAL;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetTELEDrvLIMIT(PAF_CTRL_PARAM pCTRL, SHORT val)
{
	FLOAT zCnv = 2113.0f+((255.0f-val)/255)*562+0.5; //2675 = diffMinMaxZoom

	if (GetDrvOnOffPINT() || GetAgeOnOffLENS()) return;
	if (AF_IsProcessExecuting()) AF_EnableProcess(OFF);
	LENS_SetDIGIDrvOnOff(pCTRL, OFF);	// DZOOM off

	stLENS.tele = pCTRL->tele = (SHORT)zCnv;
	stLENS.zTGT = stLENS.tele + stZOOM.pint;

	if (stZOOM.cur>stLENS.zTGT)	stLENS.zCTL = PRE_LENS_GOAL;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetDIGIDrvOnOff(PAF_CTRL_PARAM pCTRL, BOOL OnOff)
{
	pCTRL->ctrl &= (~AFZOOM_DZOOM_OFF);

	if (OnOff==ON)	pCTRL->ctrl |= AFZOOM_DZOOM_ON;
	else			pCTRL->ctrl |= AFZOOM_DZOOM_OFF;

	fZoomDigitalOnOff = (OnOff==ON)? ON : OFF;

	if (OnOff==ON) return; 

	stZOOM.cur = MIN(stZOOM.cur,stZOOM.oMin+stLENS.tele);
	SetDZOOM_TargetRatio(LENS_GetCurPositionDZOOM());
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetDIGIMixOnOff(PAF_CTRL_PARAM pCTRL, BOOL OnOff)
{
	pCTRL->ctrl &= (~AFZOOM_DZMIX_OFF);

	if (OnOff==ON)	pCTRL->ctrl |= AFZOOM_DZMIX_ON;
	else			pCTRL->ctrl |= AFZOOM_DZMIX_OFF;

	fZoomDigiMixOnOff = (OnOff==ON)? ON : OFF;

	if (OnOff==ON) return; 

	stZOOM.cur = MIN(stZOOM.cur,stZOOM.oMin+stLENS.tele);
	SetDZOOM_TargetRatio(LENS_GetCurPositionDZOOM());
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetDIGIDrvLIMIT(PAF_CTRL_PARAM pCTRL, SHORT val)
{
//	if (fZoomDigitalOnOff==OFF) return;
//	if (fZoomDigiMixOnOff==OFF) return;

	pCTRL->digi = val;	stLENS.digi = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetZOOMDrvSPEED(PAF_CTRL_PARAM pCTRL, LONG val)
{
	pCTRL->ctrl &= (~AFZOOM_SPEED_MASK);
	pCTRL->ctrl |= val;	stLENS.z_ID = (SHORT)(val>>4);
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetAFIntvalTIME(PAF_CTRL_PARAM pCTRL, WORD val)
{
	pCTRL->nINT = 0;	pCTRL->tINT = MIN(val,255);
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetAFActiveTIME(PAF_CTRL_PARAM pCTRL, WORD val)
{
	pCTRL->nACT = 0;	pCTRL->tACT = MIN(val,255);
}

//--------------------------------------------------------------------------------------------------------------------------
static void LENS_PreFOKSTarget(void)
{
	if (stLENS.fCTL!=PRE_LENS_GOAL) return;

	AF_EnableProcess(OFF);	LENS_EnableMotorFOKS(ON);
	stLENS.fTGT = LENS_GetMINPositionLOCI(stZOOM.cur); // get min Focus of zoom position

	stLENS.fDIR = (stFOKS.cur>stLENS.fTGT)? fNEAR : fFAR;
	LENS_SetDirectionFOKS(stLENS.fDIR);
}

//--------------------------------------------------------------------------------------------------------------------------
static void LENS_DrvFOKSTarget(void)
{
	if (stLENS.fCTL!=DRV_LENS_GOAL) return;

	LENS_SetDriveFOKS(stLENS.fTGT);
}

//--------------------------------------------------------------------------------------------------------------------------
static void LENS_EndFOKSTarget(void)
{
	if (stLENS.fCTL!=END_LENS_GOAL) return;

	LENS_EnableMotorFOKS(OFF);
	stLENS.fCTL = END_LENS_PROC;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_DrvHandlerFOKS(PAF_CTRL_PARAM pCTRL)
{
	if (stLENS.fCTL==END_LENS_PROC)	return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	LENS_PreFOKSTarget();	// motor ON, direction
	LENS_DrvFOKSTarget();	// drive goal position
	LENS_EndFOKSTarget();	// motor OFF

	if (stLENS.fCTL!=END_LENS_PROC)	stLENS.fCTL++;
}

//--------------------------------------------------------------------------------------------------------------------------
static void LENS_PreZOOMTarget(void)
{
	if (stLENS.zCTL!=PRE_LENS_GOAL) return;

	AF_EnableProcess(OFF);		// disable AF process

	LENS_EnableMotorZOOM(ON);	// enable zoom motor channel
	LENS_EnableMotorFOKS(ON);	// enable foks motor channel

	stLENS.zDIR = (stZOOM.cur>stLENS.zTGT)? zWIDE : zTELE;
	LENS_SetDirectionZOOM(stLENS.zDIR);

	stLENS.fTGT = LENS_GetTrackingFOKS(stLENS.zTGT);
	stLENS.fDIR = (stFOKS.cur>stLENS.fTGT)? fNEAR : fFAR;
	LENS_SetDirectionFOKS(stLENS.fDIR);
}

//--------------------------------------------------------------------------------------------------------------------------
static void LENS_DrvZOOMTarget(void)
{
	if (stLENS.zCTL!=DRV_LENS_GOAL) return;

	LENS_SetDriveZOOM(stLENS.zTGT);
	LENS_SetDriveFOKS(stLENS.fTGT);
}

//--------------------------------------------------------------------------------------------------------------------------
static void LENS_EndZOOMTarget(void)
{
	if (stLENS.zCTL!=END_LENS_GOAL) return;

	LENS_EnableMotorFOKS(OFF);
	LENS_EnableMotorZOOM(OFF);
	stLENS.zCTL = END_LENS_PROC;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_DrvHandlerZOOM(PAF_CTRL_PARAM pCTRL)
{
	if (stLENS.zCTL==END_LENS_PROC)	return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
	if (LENS_IsDriveCompleteZOOM()==FALSE) return;

	LENS_PreZOOMTarget();	// motor ON, direction
	LENS_DrvZOOMTarget();	// drive goal position
	LENS_EndZOOMTarget();	// motor OFF

	if (stLENS.zCTL!=END_LENS_PROC)	stLENS.zCTL++;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL LENS_IsDriveExecuting(void)
{
	return (stLENS.fCTL || stLENS.zCTL)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void DZ_UpdateHandler(PAF_CTRL_PARAM pCTRL)
{
	if (fZoomDigiMixOnOff==OFF) return;
	if (fZoomDigitalOnOff==OFF) return;

	SetDZOOM_TargetRatio(LENS_GetCurPositionDZOOM());
}

//--------------------------------------------------------------------------------------------------
void ISPM OLPF_CtrlTimeCounter(void)
{
	if (wOLPFCnt<GET_FPS/5) wOLPFCnt++;		// 200ms
	else					return;
	
	if (wOLPFCnt == GET_FPS/5)		{OLPF_DY(HIGH);	OLPF_NT(HIGH);}		// icr driver stop
}

//--------------------------------------------------------------------------------------------------
WORD OLPF_GetDrvTimeCount(void)
{
	return wOLPFCnt;
}

//--------------------------------------------------------------------------------------------------
void OLPF_SetDrvTimeCount(WORD cnt)
{
	wOLPFCnt = cnt;
}
#endif

/*  FILE_END_HERE */
