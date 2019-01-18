//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2005  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	lens_yt3010_2motor.c
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"
#include	"lens_yt3010_2motor.h"

#if defined(__USE_YT3010_2MOTOR__)
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
	SHORT	margin;

	SHORT	tMax;			// motor opt+dig maximum position (absolute value)

	WORD	xfer;			// transfer motor data to drive IC

}	LENS_INFO, *PLENS_INFO;

typedef	struct {
	SHORT	digi;			// ZOOM digital end position

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
BOOL fDZ_UpdateHandler = 0;
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

static SHORT CONV_LocusByZOOM;

static ROMDATA SHORT LENS_SpeedByZOOM[][13]		= {
	{   0,	0,		0,		0,		0,		0,		0,		0,		0,		0,		500,		1500,	2275},	// 0
	{   0,	0,		0,		0,		0,		0,		0,		0,		500,		1000,	2000,	2250,	2275},	// 1
	{   0,	0,		0,		0,		0,		0,		0,		400,		800,		1200,	2000,	2250,	2275},	// 2
	{   0,	0,		0,		0,		0,		300,		600,		900,		1200,	1500,	2000,	2250,	2275},	// 3
	{   0,	0,		0,		0,		250,		500,		750,		1000,	1250,	1500,	2000,	2250,	2275},	// 4 fast
	{   0,	0,		0,		400,		800,		1000,	1200, 	1400,	1600,	1800,	2000,	2250,	2275},	// 5
	{   0,	0,		200,		400,		800,		1000,	1200,	1400,	1600,	1800,	2000,	2250,	2275},	// 6
	{   0,	0,		200,		400,		800,		1000,	1200,	1400,	1600,	1800,	2000,	2250,	2275}	// 7
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

#if defined(__USE_AN41919__)
	mode = (0<10)|(0<<9)|(0<<8)|(1<<7);	// PDWEN[10] = (IRIS Disable)
	LENS_WordWrite(SPI_AN41908_ADDR, 0x0B, mode);
#endif

	mode = (2<<13)|(14<<8)|(1<<0);		// PWMRES, PWMMODE, DT1
	LENS_WordWrite(SPI_AN41908_ADDR, 0x20, mode);

	mode = (1<<7)|(5<<0);				// TESTEN2, FZTEST
	LENS_WordWrite(SPI_AN41908_ADDR, 0x21, mode);


	mode = (0<<8)|(1<<0);				// PHMODAB, DT2A
	LENS_WordWrite(SPI_AN41908_ADDR, 0x22, mode);

	mode = (224<<8)|(224<<0);			// PPWB, PPWA
	LENS_WordWrite(SPI_AN41908_ADDR, 0x23, mode);

	zVal = (1<<12)|(0<<11)|(0<<10)|(0<<9)|(0<<8)|(0<<0);	// MICROAB, LEDA, ENDISAB, BREAKAB, CCWCWAB, PSUMAB
	LENS_WordWrite(SPI_AN41908_ADDR, 0x24, zVal);



	mode = (0<<8)|(1<<0);				// PHMODCD, DT2B
	LENS_WordWrite(SPI_AN41908_ADDR, 0x27, mode);

	mode = (224<<8)|(224<<0);			// PPWD, PPWC
	LENS_WordWrite(SPI_AN41908_ADDR, 0x28, mode);

	fVal = (1<<12)|(0<<11)|(0<<10)|(0<<9)|(0<<8)|(0<<0);	// MICROCD, LEDB, ENDISCD, BREAKCD, CCWCWCD, PSUMCD
	LENS_WordWrite(SPI_AN41908_ADDR, 0x29, fVal);


	mode = (177<<0);					// INTCTAB, 800pps
	LENS_WordWrite(SPI_AN41908_ADDR, 0x25, mode);	// zoom

	//mode = (353<<0);					// INTCTCD, 800pps
	mode = (177<<0);					// INTCTCD, 800pps
	LENS_WordWrite(SPI_AN41908_ADDR, 0x2A, mode);	// foks
}

//--------------------------------------------------------------------------------------------------------------------------
static void LENS_SetLensInfo(void)
{
	stZOOM.mMin = zeroZoom-150;			stFOKS.mMin = zeroFocus-150*1;
	stZOOM.mMax = zeroZoom+2275+150;	stFOKS.mMax = zeroFocus+(2935+150)*1;
	stZOOM.oMin = zeroZoom;				stFOKS.oMin = zeroFocus;
	stZOOM.oMax = zeroZoom+2275;		stFOKS.oMax = zeroFocus+2935*1;
	stZOOM.margin = 150;					stFOKS.margin = 150*1;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetInstancePARAM(void)
{
	LENSDRV_Initialize();		// drive chip initialize
	LENS_SetLensInfo();			// set lens information

	LENS_ResetPosition();		// find reset position
}

//--------------------------------------------------------------------------------------------------
static void LENS_WaitDriveMOTOR(void)
{
	while (fZoomDrvCompleted==FALSE); fZoomDriveTrigger = OFF;
	while (fFoksDrvCompleted==FALSE); fFoksDriveTrigger = OFF;
}

//--------------------------------------------------------------------------------------------------
void LENS_ZeroPosition(void) // mimitest
{
	fFoksDrvLimitMode = OFF;	fZoomDrvLimitMode = OFF;
	fZoomDrvTrackMode = OFF;	fZoomDriveDigital = OFF;
	fZoomDrvStepRatio = x4M;	fLensMechPosition = ON;

	LENS_SetCurPositionZOOM(stZOOM.mMax);
	LENS_SetCurPositionFOKS(stFOKS.mMin);
	//LENS_SetCurPositionFOKS(stFOKS.mMax);
	do {
		LENS_EnableMotorZOOM(ON);
		LENS_EnableMotorFOKS(ON);
		LENS_SetDirectionZOOM(zWIDE);
		LENS_SetDirectionFOKS(fFAR);
		//LENS_SetDirectionFOKS(fNEAR);

#if defined(__USE_GPIO_MTC_AF__)
		if(stZOOM.cur > stZOOM.mMin)
			LENS_SetDriveZOOM(stZOOM.cur-1);
		if(stFOKS.cur < stFOKS.mMax)
			LENS_SetDriveFOKS(stFOKS.cur+1);
#else
		if(stZOOM.cur > stZOOM.mMin)
			LENS_SetDriveZOOM(stZOOM.cur-4);
		if(stFOKS.cur < stFOKS.mMax)
			LENS_SetDriveFOKS(stFOKS.cur+4);
#endif
		//if(stFOKS.cur > stFOKS.mMin)
		//	LENS_SetDriveFOKS(stFOKS.cur-16);

		//UARTprintf("stZOOM.cur:%d / stFOKS.cur:%d\r\n",stZOOM.cur,stFOKS.cur);
		LENS_XferDriveMOTOR();
		LENS_WaitDriveMOTOR(); Delay_mSec(4);
	} while((stZOOM.cur > stZOOM.mMin)||(stFOKS.cur < stFOKS.mMax));
	//} while((stZOOM.cur > stZOOM.mMin)||(stFOKS.cur > stFOKS.mMin));
	UARTprintf("lens reset position...\r\n");
	//UARTprintf("stZOOM.cur:%d / stFOKS.cur:%d\r\n",stZOOM.cur,stFOKS.cur);


	LENS_SetDirectionZOOM(zTELE);
	LENS_SetDirectionFOKS(fNEAR);
	//LENS_SetDirectionFOKS(fFAR);
	do {
#if defined(__USE_GPIO_MTC_AF__)
		if(stZOOM.cur < stZOOM.oMin)
			LENS_SetDriveZOOM(stZOOM.cur+1);
		if(stFOKS.cur > LENS_LocusTable[0])
			LENS_SetDriveFOKS(stFOKS.cur-1);
#else
		if(stZOOM.cur < stZOOM.oMin)
			LENS_SetDriveZOOM(stZOOM.cur+2);
		if(stFOKS.cur > LENS_LocusTable[0])
			LENS_SetDriveFOKS(stFOKS.cur-2);
#endif
		//if(stFOKS.cur < LENS_LocusTable[0])
		//	LENS_SetDriveFOKS(stFOKS.cur+8);
		//UARTprintf("stZOOM.cur:%d / stFOKS.cur:%d\r\n",stZOOM.cur,stFOKS.cur);
		LENS_XferDriveMOTOR();
		LENS_WaitDriveMOTOR(); Delay_mSec(4);
	} while((stZOOM.cur < stZOOM.oMin)||(stFOKS.cur > LENS_LocusTable[0]));
	//} while((stZOOM.cur < stZOOM.oMin)||(stFOKS.cur < LENS_LocusTable[0]));
	UARTprintf("lens zero position...\r\n");
	//UARTprintf("stZOOM.cur:%d / stFOKS.cur:%d\r\n",stZOOM.cur,stFOKS.cur);

	fZoomDrvStepRatio = x4M;
	fLensMechPosition = OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_ResetPosition(void)
{
	LENS_ZeroPosition();
	LENS_SetCurPositionZOOM(stZOOM.oMin);	LENS_SetCurPositionFOKS(LENS_LocusTable[0]);

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

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetCurPositionZOOM(void)
{
	return stZOOM.cur;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetGoalPositionZOOM(void)
{
	return stZOOM.goal;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetCurPositionFOKS(void)
{
	return stFOKS.cur;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT ISPM LENS_GetRelPositionZOOM(void)
{
	return MIN(stZOOM.cur,stZOOM.oMax)-stZOOM.oMin;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT ISPM LENS_GetRelMaxPositionZOOM(void)
{
	if (fZoomDigitalOnOff==OFF || fZoomDigiMixOnOff==OFF)		return (stZOOM.oMax-stZOOM.oMin);
	else 													return (stZOOM.oMax-stZOOM.oMin+stLENS.digi);
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetRelPositionFOKS(void)
{
	return stFOKS.cur;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT ISPM LENS_GetCurPositionDZOOM(void)
{
	if (fZoomDriveDigital==OFF) return 0;
	return stZOOM.cur-stZOOM.oMax;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM LENS_SetCurPositionDZOOM(SHORT val)
{
	if (fZoomDriveDigital==OFF) return;
	stZOOM.cur = stZOOM.oMax + val;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetMaxPositionZOOM(void)
{
	if (fZoomDigitalOnOff==OFF || fZoomDigiMixOnOff==OFF)
		 stZOOM.tMax = stZOOM.oMax;
	else stZOOM.tMax = stZOOM.oMax+stLENS.digi;

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
	return (fLensMechPosition)? stZOOM.mMin : stZOOM.oMin;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetMinPositionFOKS(void)
{
	return (fLensMechPosition)? stFOKS.mMin : stFOKS.oMin;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetVISCAPositionZOOM(void)
{
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
static SHORT LENS_GetConvLocusByZOOM(SHORT zID)
{
	return LENS_LocusTable[zID];
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetVISCAPositionFOKS(void)
{
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
static SHORT LENS_CnvVISCAtoREAL_FOKS(SHORT zVal, SHORT fVal)
{
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetCnvPositionLOCI(void)
{
	SHORT zVal = LENS_GetRelPositionZOOM();
	CONV_LocusByZOOM = LENS_GetConvLocusByZOOM(zVal);
	return zVal;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetMIDPositionLOCI(void)
{
	LENS_GetCnvPositionLOCI();
	return CONV_LocusByZOOM;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetGAPPositionLOCI(void)
{
	SHORT zVal = LENS_GetCurPositionZOOM();
	SHORT fMAX = LENS_GetMAXPositionLOCI(zVal);
	SHORT fMIN = LENS_GetMINPositionLOCI(zVal);

	return (fMAX-fMIN);
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT ISPM LENS_GetMAXPositionLOCI(SHORT zVal)
{
	SHORT fVal;

	zVal = MIN(MAX(zVal, stZOOM.oMin), stZOOM.oMax) - stZOOM.oMin;
	CONV_LocusByZOOM = LENS_GetConvLocusByZOOM(zVal);
	fVal = MIN(MAX(CONV_LocusByZOOM+500, stFOKS.oMin), stFOKS.oMax);
	return (fLensMechPosition)? stFOKS.mMax : fVal;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT ISPM LENS_GetMINPositionLOCI(SHORT zVal)
{
	SHORT fVal;

	zVal = MIN(MAX(zVal, stZOOM.oMin), stZOOM.oMax) - stZOOM.oMin;
	CONV_LocusByZOOM = LENS_GetConvLocusByZOOM(zVal);
	fVal = MIN(MAX(CONV_LocusByZOOM-500, stFOKS.oMin), stFOKS.oMax);
	return (fLensMechPosition)? stFOKS.mMin : fVal;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT ISPM LENS_GetMAXPositionModeLOCI(SHORT zVal,BOOL mode)
{
	SHORT fVal;

	zVal = MIN(MAX(zVal, stZOOM.oMin), stZOOM.oMax) - stZOOM.oMin;
	CONV_LocusByZOOM = LENS_GetConvLocusByZOOM(zVal);
	if(mode == 0)
		fVal = MIN(MAX(CONV_LocusByZOOM+256, stFOKS.oMin), stFOKS.oMax);
	else
		fVal = MIN(MAX(CONV_LocusByZOOM+512, stFOKS.oMin), stFOKS.oMax);
	return (fLensMechPosition)? stFOKS.mMax : fVal;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT ISPM LENS_GetMINPositionModeLOCI(SHORT zVal,BOOL mode)
{
	SHORT fVal;

	zVal = MIN(MAX(zVal, stZOOM.oMin), stZOOM.oMax) - stZOOM.oMin;
	CONV_LocusByZOOM = LENS_GetConvLocusByZOOM(zVal);
	if(mode == 0)
		fVal = MIN(MAX(CONV_LocusByZOOM-256, stFOKS.oMin), stFOKS.oMax);
	else
		fVal = MIN(MAX(CONV_LocusByZOOM-512, stFOKS.oMin), stFOKS.oMax);
	return (fLensMechPosition)? stFOKS.mMin : fVal;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetABVPositionLOCI(BYTE nID)
{
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_GetBLOPositionLOCI(BYTE nID)
{
	return 0;
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
	LONG zMax = LENS_GetMaxPositionZOOM();

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
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT LENS_CnvVISCAPositionLOCI(SHORT zVal, SHORT fVal)
{
	return 0;
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
	SHORT oMAX = stZOOM.oMax;

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
	SHORT i, zVal = LENS_GetRelPositionZOOM();
	SHORT zID = (fZoomDrvSpeedMode)? stLENS.z_ID : 7;

	for (i=0; i<13-1; i++) {
		if (zVal<LENS_SpeedByZOOM[zID][i]) break;
	}

	return (fZoomDrvTrackMode==OFF)? 13 : (13-i);
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

#if defined(__USE_GPIO_MTC_AF__)
//--------------------------------------------------------------------------------------------------
void ISPM GpioMoterCtrlFoks(SHORT step) 
{
	switch(step) { // Focus Driving
		case 0: GPIO_MFC_AP(HIGH); GPIO_MFC_AM(LOW); GPIO_MFC_BP(LOW); GPIO_MFC_BM(LOW); break;
		case 1: GPIO_MFC_AP(HIGH); GPIO_MFC_AM(LOW); GPIO_MFC_BP(LOW); GPIO_MFC_BM(HIGH); break;
		case 2: GPIO_MFC_AP(LOW); GPIO_MFC_AM(LOW); GPIO_MFC_BP(LOW); GPIO_MFC_BM(HIGH); break;
		case 3: GPIO_MFC_AP(LOW); GPIO_MFC_AM(HIGH); GPIO_MFC_BP(LOW); GPIO_MFC_BM(HIGH); break;
		case 4: GPIO_MFC_AP(LOW); GPIO_MFC_AM(HIGH); GPIO_MFC_BP(LOW); GPIO_MFC_BM(LOW); break;
		case 5: GPIO_MFC_AP(LOW); GPIO_MFC_AM(HIGH); GPIO_MFC_BP(HIGH); GPIO_MFC_BM(LOW); break;
		case 6: GPIO_MFC_AP(LOW); GPIO_MFC_AM(LOW); GPIO_MFC_BP(HIGH); GPIO_MFC_BM(LOW); break;
		case 7: GPIO_MFC_AP(HIGH); GPIO_MFC_AM(LOW); GPIO_MFC_BP(HIGH); GPIO_MFC_BM(LOW); break;
	}
}

//--------------------------------------------------------------------------------------------------
void ISPM GpioMoterCtrlZoom(SHORT step) 
{
	switch(step){ // Zoom Driving
		case 0: GPIO_MZC_AP(HIGH); GPIO_MZC_AM(LOW); GPIO_MZC_BP(LOW); GPIO_MZC_BM(LOW); break;
		case 1: GPIO_MZC_AP(HIGH); GPIO_MZC_AM(LOW); GPIO_MZC_BP(LOW); GPIO_MZC_BM(HIGH); break;
		case 2: GPIO_MZC_AP(LOW); GPIO_MZC_AM(LOW); GPIO_MZC_BP(LOW); GPIO_MZC_BM(HIGH); break;
		case 3: GPIO_MZC_AP(LOW); GPIO_MZC_AM(HIGH); GPIO_MZC_BP(LOW); GPIO_MZC_BM(HIGH); break;
		case 4: GPIO_MZC_AP(LOW); GPIO_MZC_AM(HIGH); GPIO_MZC_BP(LOW); GPIO_MZC_BM(LOW); break;
		case 5: GPIO_MZC_AP(LOW); GPIO_MZC_AM(HIGH); GPIO_MZC_BP(HIGH); GPIO_MZC_BM(LOW); break;
		case 6: GPIO_MZC_AP(LOW); GPIO_MZC_AM(LOW); GPIO_MZC_BP(HIGH); GPIO_MZC_BM(LOW); break;
		case 7: GPIO_MZC_AP(HIGH); GPIO_MZC_AM(LOW); GPIO_MZC_BP(HIGH); GPIO_MZC_BM(LOW); break;
	}
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM LENS_ConfigDriveZOOM(void)
{
#if defined(__USE_GPIO_MTC_AF__)
	SHORT step = MIN(LENS_GetStepCountZOOM(),1);

	//if(step == 0) return;

	BYTE tmpB;

	if(step != 0) {
		tmpB = stZOOM.xfer&0xf;
		
		if (fZoomDrvDirection==zWIDE) {
			stZOOM.cur --;
			if(tmpB == 0)	tmpB = 7;
			else			tmpB--;
		} else {
			stZOOM.cur ++;
			if(tmpB == 7)	tmpB = 0;
			else			tmpB++;
		}
		//UARTprintf("z-cur:%d / xfer:%d\r\n",stZOOM.cur,(stZOOM.xfer&0xf));

		GpioMoterCtrlZoom(tmpB);
		stZOOM.xfer = (stZOOM.xfer&0xfff0) | tmpB;
	}


	if (fZoomDrvDirection==zWIDE) stZOOM.xfer |= 0x0100;
	else						  stZOOM.xfer |= 0x0000;

	if (fZoomDriveTrigger==FALSE) fZoomDrvCompleted = TRUE;
	if (stZOOM.cur==stZOOM.goal) fZoomDrvCompleted = TRUE;
	if (fZoomDrvCompleted==TRUE) fZoomDrvSpeedMode = OFF;
#else
	SHORT step = LENS_GetStepCountZOOM();
	SHORT n = (fZoomDrvStepRatio)? 2 : 8;

	if (fZoomDrvDirection==zWIDE) stZOOM.cur -= step;
	else						  stZOOM.cur += step;

	stZOOM.xfer &= 0xfe00;	stZOOM.xfer |= (n*step);
	if (fZoomDriveDigital)	stZOOM.xfer &= (0xfe00);

	if (fZoomDrvDirection==zWIDE) stZOOM.xfer |= 0x0100;
	else						  stZOOM.xfer |= 0x0000;

	if (fZoomDriveTrigger==FALSE) fZoomDrvCompleted = TRUE;
	if (stZOOM.cur==stZOOM.goal) fZoomDrvCompleted = TRUE;
	if (fZoomDrvCompleted==TRUE) fZoomDrvSpeedMode = OFF;
#endif
}

//--------------------------------------------------------------------------------------------------
static SHORT LENS_GetTrackingZOOM(BOOL dir)
{
	SHORT step = LENS_GetStepCountZOOM();
	SHORT oMAX = stZOOM.oMax;

	stZOOM.goal = (dir==zWIDE)? stZOOM.oMin : oMAX;
	return (dir==zWIDE)? stZOOM.cur-step : stZOOM.cur+step;
}

//--------------------------------------------------------------------------------------------------
static SHORT ISPM LENS_GetTrackingFOKS(SHORT zVal)	
{
	SHORT goal;

	zVal = MIN(MAX(zVal, stZOOM.oMin), stZOOM.oMax)-stZOOM.oMin;

	CONV_LocusByZOOM = LENS_GetConvLocusByZOOM(zVal);

	goal = CONV_LocusByZOOM;

	return goal;
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM LENS_ConfigTrackZOOM(void)
{
	if (fZoomDrvTrackMode==OFF) return;

	stFOKS.goal = LENS_GetTrackingFOKS(stZOOM.cur);
	LENS_SetDriveFOKS(stFOKS.goal);
	//UARTprintf("stFOKS.goal:%d\r\n",stFOKS.goal);
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

	xID = 1;

	if (fFoksDrvSpeedMode==OFF)	 max = 13;
	else		max = xID*(stLENS.f_ID+1);

	return MIN(DIFF(stFOKS.cur, stFOKS.goal), max);
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM LENS_ConfigDriveFOKS(void)
{
#if defined(__USE_GPIO_MTC_AF__)
	SHORT step = MIN(LENS_GetStepCountFOKS(),1);

	BYTE tmpB;

	tmpB = stFOKS.xfer&0x7;

	if(step != 0) {
		if (fFoksDrvDirection==fNEAR) {
			stFOKS.cur --;
			if(tmpB == 0)	tmpB == 7;
			else			tmpB--;
		} else {
			stFOKS.cur ++;
			if(tmpB == 7)	tmpB = 0;
			else			tmpB ++;
		}

		GpioMoterCtrlFoks(tmpB);
		stFOKS.xfer = (stFOKS.xfer&0xfff8) | tmpB;
	}

	if (fFoksDrvDirection==fNEAR) stFOKS.xfer |= 0x0000;
	else						  stFOKS.xfer |= 0x0100;

	if (fFoksDriveTrigger==FALSE) fFoksDrvCompleted = TRUE;
	if (stFOKS.cur==stFOKS.goal) fFoksDrvCompleted = TRUE;
	if (fFoksDrvCompleted==TRUE) fFoksDrvSpeedMode = OFF;
#else
	SHORT step = LENS_GetStepCountFOKS();
	SHORT n = (fZoomDrvStepRatio)? 2 : 8;

	if (fFoksDrvDirection==fNEAR) stFOKS.cur -= step;
	else						  stFOKS.cur += step;

	stFOKS.xfer &= 0xfe00;	stFOKS.xfer |= (n*step);

	if (fFoksDrvDirection==fNEAR) stFOKS.xfer |= 0x0000;
	else						  stFOKS.xfer |= 0x0100;

	if (fFoksDriveTrigger==FALSE) fFoksDrvCompleted = TRUE;
	if (stFOKS.cur==stFOKS.goal) fFoksDrvCompleted = TRUE;
	if (fFoksDrvCompleted==TRUE) fFoksDrvSpeedMode = OFF;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM LENS_XferDriveMOTOR(void) // mimitest
{
	LENS_ConfigDriveZOOM();
	LENS_ConfigTrackZOOM();
	LENS_ConfigDriveFOKS();

#if !defined(__USE_GPIO_MTC_AF__)
	LENS_VD(HIGH);	//LENS_VD(LOW);
	LENS_WordWrite(SPI_AN41908_ADDR, 0x24, stZOOM.xfer);
	LENS_WordWrite(SPI_AN41908_ADDR, 0x29, stFOKS.xfer);
	LENS_VD(LOW);
#endif
}

//--------------------------------------------------------------------------------------------------
void LENS_SetTrackingPEAK(SHORT val)
{
	stFOKS.peak = (val==0)? stFOKS.cur : val;
}

//--------------------------------------------------------------------------------------------------
void LENS_SetTrackingINFO(SHORT val)
{
	SHORT fGap, *pID;
	SHORT zVal;

	LENS_SetTrackingPEAK(val);
	zVal = MIN(MAX(stZOOM.cur, stZOOM.oMin), stZOOM.oMax)-stZOOM.oMin;

	CONV_LocusByZOOM = LENS_GetConvLocusByZOOM(zVal);

	fRatio = 0;
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

//--------------------------------------------------------------------------------------------------
BYTE LENS_GetAFDriveSTEP(BYTE nID)
{
	SHORT nSTD, fABS = LENS_GetGAPPositionLOCI();
	SHORT tSTD = 0;

	nSTD = MIN(MAX(fABS/16, 3), 15);
	switch (nID) {
		case DRV_STEP_MIN:	tSTD = MIN(MIN(MAX(fABS/24, 2),  8), (nSTD+1)/2);	break;
		case DRV_STEP_STD:	tSTD = MIN(MIN(MAX(fABS/16, 3), 16), (nSTD+0)*1);	break;
		case DRV_STEP_MID:	tSTD = MIN(MIN(MAX(fABS/10, 4), 24), (nSTD+0)*2);	break;
		case DRV_STEP_MAX:	tSTD = MIN(MIN(MAX(fABS/10, 4), 24), (nSTD+0)*3);	break;
		case DRV_STEP_FWD:	tSTD = MIN(MIN(MAX(fABS/ 8, 6), 32), (nSTD+0)*4);	break;
		case DRV_STEP_BWD:	tSTD = 56;											break;
		case DRV_STEP_TOP:	tSTD = MIN(MAX(fABS/ 64, 2), 2);					break;
		default :																break;
	}
	return tSTD;
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetFOKSDrvSPEED(PAF_CTRL_PARAM pCTRL, LONG val)
{
	pCTRL->ctrl &= (~AFFOKS_SPEED_MASK);
	pCTRL->ctrl |= val;	stLENS.f_ID = (SHORT)(val>>8);
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetDIGIDrvOnOff(PAF_CTRL_PARAM pCTRL, BOOL OnOff)
{
	pCTRL->ctrl &= (~AFZOOM_DZOOM_OFF);

	if (OnOff==ON)	pCTRL->ctrl |= AFZOOM_DZOOM_ON;
	else			pCTRL->ctrl |= AFZOOM_DZOOM_OFF;

	fZoomDigitalOnOff = (OnOff==ON)? ON : OFF;

	if (OnOff==ON) return; 

	stZOOM.cur = MIN(stZOOM.cur,stZOOM.oMax);
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

	stZOOM.cur = MIN(stZOOM.cur,stZOOM.oMax);
	SetDZOOM_TargetRatio(LENS_GetCurPositionDZOOM());
}

//--------------------------------------------------------------------------------------------------------------------------
void LENS_SetDIGIDrvLIMIT(PAF_CTRL_PARAM pCTRL, SHORT val)
{
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
	BOOL mode;
	stLENS.fTGT = LENS_GetMINPositionLOCI(stZOOM.cur);

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
static SHORT dzVbak = 0;
void DZ_UpdateHandler(PAF_CTRL_PARAM pCTRL)
{
	short dzV;
	//char dzC[11];

	if (fZoomDigiMixOnOff==OFF) return;
	if (fZoomDigitalOnOff==OFF) return;

	if(fDZ_UpdateHandler == 1) {
		dzV = LENS_GetCurPositionDZOOM();
		//if((dzV > 0) && (dzVbak==0)) Delay_mSec(100);
		SetDZOOM_TargetRatio(dzV);
		//if(dzV>0) {
		//	sprintf(dzC,"DZoom : %2d",dzV);
		//	OSD_IrkeyTitleDisplay(ON, dzC);
		//} else {
		//	OSD_IrkeyTitleDisplay(OFF, "          ");
		//}
		dzVbak = dzV;
	}
	fDZ_UpdateHandler = 0;
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

typedef	struct {
	SHORT	startFocus;
	SHORT	endFocus;
	SHORT	centerFocus;

	SHORT	step;
	SHORT	topFocusLoc;
	LONG	topFocusVal;
}	SEARCH_CTL, *PSEARCH_CTL;

static volatile SEARCH_CTL tmSEARCH;

BOOL startFocusPickSearch = FALSE;

//--------------------------------------------------------------------------------------------------
void LENS_FocusMove(BOOL mode,BOOL dir) // mimitest
{
	SHORT zoomLoc, focusLoc;
	SHORT relZoomLoc;

	if(mode == 1) { // ZOOM
#if 0
		if (IsMsgPreExecCommand()) {
			SetMsgCommandRepeatKey(0);		// set repeater MsgFlag
			SetMsgCommandDriveFOKS(0);		// set drvMotor-dir flag
			LENS_EnableMotorFOKS(ON);
			LENS_SetDirectionFOKS(dir);
		}

		fFoksDriveTrigger = ON;
		fFoksDrvCompleted = FALSE;

		if(dir) {
			stFOKS.goal = 20000;
		} else {
			stFOKS.goal = 20000+(2935+300)*1;
		}

		LENS_WaitDriveMOTOR();

		UARTprintf("stFOKS.cur:%5d /",stFOKS.cur);
		UARTprintf("stFOKS.goal:%5d\r\n",stFOKS.goal);
#else
		if (IsMsgPreExecCommand()) {
			SetMsgCommandRepeatKey(0);		// set repeater MsgFlag
			SetMsgCommandDriveZOOM(0);		// set drvMotor-dir flag
			LENS_EnableMotorZOOM(ON);
			LENS_EnableMotorFOKS(ON);
			LENS_SetDirectionZOOM(dir);
		}

		if(dir)	zoomLoc = MIN(MAX((stZOOM.cur-2),stZOOM.oMin),stZOOM.oMax);
		else		zoomLoc = MIN(MAX((stZOOM.cur+2),stZOOM.oMin),stZOOM.oMax);
		relZoomLoc = zoomLoc - stZOOM.oMin;
		
		if(LENS_LocusTable[relZoomLoc] > stFOKS.cur)
			LENS_SetDirectionFOKS(fFAR);
		else
			LENS_SetDirectionFOKS(fNEAR);
		//UARTprintf("LENS_LocusTable[%d] = %d -- ",relZoomLoc,LENS_LocusTable[relZoomLoc]);
		LENS_SetDriveFOKS(LENS_LocusTable[relZoomLoc]);
		
		LENS_SetDriveZOOM(zoomLoc);
		LENS_WaitDriveMOTOR(); Delay_mSec(1);

		//UARTprintf("stFOKS.cur:%5d /",stFOKS.cur);
		//UARTprintf("stFOKS.goal:%5d /",stFOKS.goal);
		//UARTprintf("stZOOM.cur:%5d /",stZOOM.cur);
		//UARTprintf("stZOOM.goal:%5d\r\n",stZOOM.goal);
#endif
	} else { // FOCUS

#if 1
#if 0
		UARTprintf("start focus pick search...\r\n");
		tmSEARCH.centerFocus = LENS_LocusTable[stZOOM.cur - stZOOM.oMin];
		tmSEARCH.startFocus = MAX(tmSEARCH.centerFocus - 700*1, stFOKS.oMin);
		tmSEARCH.endFocus = MIN(tmSEARCH.centerFocus + 700*1, stFOKS.oMax);
		tmSEARCH.step = 2;
		UARTprintf("start:%d / end:%d / step:%d\r\n",tmSEARCH.startFocus,tmSEARCH.endFocus,tmSEARCH.step);

		LENS_EnableMotorFOKS(ON);
		if(tmSEARCH.startFocus > stFOKS.cur)
			LENS_SetDirectionFOKS(fFAR);
		else
			LENS_SetDirectionFOKS(fNEAR);
		LENS_SetDriveFOKS(tmSEARCH.startFocus);
		LENS_WaitDriveMOTOR();
		UARTprintf("ready focus pick search...\r\n");
		startFocusPickSearch = TRUE;
		tmSEARCH.topFocusVal = 0;
		tmSEARCH.topFocusLoc = tmSEARCH.centerFocus;
#else
		if (IsMsgPreExecCommand()) {
			SetMsgCommandRepeatKey(0);		// set repeater MsgFlag
			SetMsgCommandDriveFOKS(0);		// set drvMotor-dir flag
			LENS_EnableMotorFOKS(ON);
			LENS_SetDirectionFOKS(dir);
		}

		if(dir)	focusLoc = MAX((stFOKS.cur-2),stFOKS.oMin);
		else		focusLoc = MIN((stFOKS.cur+2),stFOKS.oMax);
		LENS_SetDriveFOKS(focusLoc);
		LENS_WaitDriveMOTOR(); //Delay_mSec(1);

		//UARTprintf("stFOKS.cur:%5d /",stFOKS.cur);
		//UARTprintf("stFOKS.goal:%5d\r\n",stFOKS.goal);
#endif
#else
		if (IsMsgPreExecCommand()) {
			SetMsgCommandRepeatKey(0);		// set repeater MsgFlag
			SetMsgCommandDriveZOOM(0);		// set drvMotor-dir flag
			LENS_EnableMotorZOOM(ON);
			LENS_SetDirectionZOOM(dir);
		}

		fZoomDriveTrigger = ON;
		fZoomDrvCompleted = FALSE;

		if(dir) {
			stZOOM.goal = 14000;
		} else {
			stZOOM.goal = 14000+2275+300;
		}

		LENS_WaitDriveMOTOR();

		UARTprintf("stZOOM.cur:%5d /",stZOOM.cur);
		UARTprintf("stZOOM.goal:%5d\r\n",stZOOM.goal);
#endif
	}
}

//--------------------------------------------------------------------------------------------------
void LENS_FocusResearch(void)
{
	if((stAFCTRL.ctrl&AFCTRL_AFAUTO_MASK) != 0) return;
	startFocusPickSearch = TRUE;
	stAFCTRL.autoFcnt = 60;
}

static BOOL fInitFocusPickSearch = FALSE;
//--------------------------------------------------------------------------------------------------
static InitFocusPickSearch(void)
{
	if(fInitFocusPickSearch == FALSE) {
		LENS_FocusResearch();
		fInitFocusPickSearch = TRUE;
	}
}

//--------------------------------------------------------------------------------------------------
void focusPickSearch(void) // mimitest
{
	InitFocusPickSearch();

	if (LENS_IsDriveExecuting() || GetAgeOnOffLENS()) return;
	if(fFoksDrvCompleted == FALSE) return;
	if(startFocusPickSearch == FALSE) {
		stAFCTRL.autoFcnt = 0;
		return;
	} else {
		if(stAFCTRL.autoFcnt <= 60) {
			if(stAFCTRL.autoFcnt == 60) {
				UARTprintf("start focus pick search...\r\n");
				tmSEARCH.centerFocus = LENS_LocusTable[stZOOM.cur - stZOOM.oMin];
				tmSEARCH.startFocus = MAX(tmSEARCH.centerFocus - 700*1, stFOKS.oMin);
				tmSEARCH.endFocus = MIN(tmSEARCH.centerFocus + 700*1, stFOKS.oMax);
				tmSEARCH.step = 2;
				UARTprintf("start:%d / end:%d / step:%d\r\n",tmSEARCH.startFocus,tmSEARCH.endFocus,tmSEARCH.step);

				LENS_EnableMotorFOKS(ON);
				if(tmSEARCH.startFocus > stFOKS.cur)
					LENS_SetDirectionFOKS(fFAR);
				else
					LENS_SetDirectionFOKS(fNEAR);
				LENS_SetDriveFOKS(tmSEARCH.startFocus);
				LENS_WaitDriveMOTOR();
				UARTprintf("ready focus pick search...\r\n");
				startFocusPickSearch = TRUE;
				tmSEARCH.topFocusVal = 0;
				tmSEARCH.topFocusLoc = tmSEARCH.centerFocus;
			}
			stAFCTRL.autoFcnt++;
			return;
		}
	}

	LONG AFdata = GetAFOPDWIND(0);
	if(AFdata > tmSEARCH.topFocusVal) {
		tmSEARCH.topFocusVal = AFdata;
		tmSEARCH.topFocusLoc = stFOKS.cur;
		//UARTprintf("TOP ----- afdata : %d / stFOKS.cur\r\n", AFdata,stFOKS.cur);
	} else {
		//UARTprintf("NOR ----- afdata : %d / stFOKS.cur\r\n", AFdata,stFOKS.cur);
	}

	char diffs[50];
	sprintf(diffs,"searching wait...");
	OSD_StrDispOnOff(OFF);
	OSD_MessageBox(ON, diffs);

	if(stFOKS.cur >= tmSEARCH.endFocus) {
		LENS_EnableMotorFOKS(ON);
		LENS_SetDirectionFOKS(fNEAR);
		LENS_SetDriveFOKS(tmSEARCH.topFocusLoc);
		//UARTprintf("stFOKS.cur:%5d /",stFOKS.cur);
		//UARTprintf("stFOKS.goal:%5d\r\n",stFOKS.goal);
		startFocusPickSearch = FALSE;
		UARTprintf("end focus pick search...%d/%d/%d\r\n",stZOOM.cur,stFOKS.cur,stFOKS.goal);
		OSD_StrDispOnOff(OFF);
	} else {
		LENS_EnableMotorFOKS(ON);
		LENS_SetDirectionFOKS(fFAR);
		LENS_SetDriveFOKS(stFOKS.cur + tmSEARCH.step);
		//UARTprintf("stFOKS.cur:%5d /",stFOKS.cur);
		//UARTprintf("stFOKS.goal:%5d\r\n",stFOKS.goal);
	}
}

#endif

/*  FILE_END_HERE */
