//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2018  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  IT66121.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:


#if defined(__USE_IT66121_TX__)


#ifndef		__IT66121_H__
#define		__IT66121_H__

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#define IT66121_DEV			0
#define IT66121_ADR			0x98
#define IT66121_CECADR		0x9C


typedef enum System_TimerTask_Type {
	eVideoCountingTimer,		// 0
	eAudioCountingTimer,		// 1
	eSWResetTimeOut,			// 2
	em_HPDAcceptCount,			// 3
	eusDectTxPlugTimer,			// 4
	euDelayX,					// 5
	eTxHDCPStateCounter,		// 6
	eLED1Ctrl,
	eLED2Ctrl,
	eLED3Ctrl,
	eLEDMonitor,
	eUpdateI2CSlaveReg,
	eCECTimeOut,
	SysTimerTask_non,
} System_TimerTask_Type;

typedef enum System_TimerTask_Run {
    SysTimerTask_AutoRun,
    SysTimerTask_OnceRun,
    SysTimerTask_NonRun,
} System_TimerTask_Run;

typedef struct TimerTask_Description
{
    DWORD  TickCount;
    DWORD  AimCount;
	System_TimerTask_Type  name;
    System_TimerTask_Run   AutoRun;
} TimerTask_Description,*PTimerTask_Description;

// define for xrc
#define	IT66121_MultiWrite(rAddr, pBuff, bytes)		I2C_Write(TWI_CH0, IT66121_ADR, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes)
#define	IT66121_MultiRead(rAddr, pBuff, bytes)		I2C_Read(TWI_CH0, IT66121_ADR, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes)

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void IT66121_Init(void);
void IT66121_ProcHanlder(void);
void IT66121_SetCSC(WORD val);
BYTE IT66121_GetCSC(void);
void IT66121_SetFrmt(void);
void ISPM SetIT66121_Tick(void);

void CreatTimerTask(BYTE cTimerTask, WORD Acount, BYTE AutoRun);
BOOL TimeOutCheck(BYTE cTimerTask);
void ProcessMultiTimerTask();


#endif	/* __IT66121_H__ */


#endif	// __USE_IT66121_TX__

