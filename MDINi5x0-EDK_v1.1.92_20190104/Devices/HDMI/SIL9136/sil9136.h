//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  LT8619EX.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#if defined(__USE_SIL9136_TX__)

#ifndef		__SIL9136_H__
#define		__SIL9136_H__

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#define IsHDMI_Sink()			(TRUE)

// jins, control avi infoframe
#define		MDIN_AVI_INFO_CTRL
#if defined(MDIN_AVI_INFO_CTRL)
typedef	struct
{
	BYTE VIC;
	BYTE Y;
	WORD dummy;

}	AVI_Infoframe_t;
#endif

#define		SIL9136_CHIP_NUM	1

// wrtie Tx2 register at the same time
//#define		SIL9136_CTRL_TX2	1

#if defined(__MDIN_i540_4K60_V10__) || defined(__USE_CMOS_IMX334__)
#define		SIL9136TX1_BUSID	TWI_CH1
#define		SIL9136TX2_BUSID	TWI_CH1
#else
#define		SIL9136TX1_BUSID	TWI_CH0
#define		SIL9136TX2_BUSID	TWI_CH0
#endif

#define		I2C_BusID			BYTE

//delay wrapper macros
#define DelayMS(a) Delay_mSec(a)

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------
extern BYTE Sil9136ChipSel;
extern BYTE Sil9136CSC;
#if defined(MDIN_AVI_INFO_CTRL)
extern AVI_Infoframe_t stInfoframe;
#endif

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void SIL9136_Init(void);
void SIL9136ProcHanlder(void);

#if defined(MDIN_AVI_INFO_CTRL)
void SIL9136_SetAviInfoVIC(BYTE VIC);
void SIL9136_SetAviInfoColor(BYTE Y);
#endif

void SIL9136_SetPollingStop(void);
void ISPM SIL9136_SetPollingStart(void);
void SIL9136_SetPowerDown(void);
void SIL9136_SetCSC(WORD val);
BYTE SIL9136_GetCSC(void);
void SIL9136_SetFrmt(void);
void SIL9136_ConvVICtoHDVFrmt(BYTE vic);

#endif	/* __SIL9136_H__ */
#endif	// #if defined(__USE_SIL9136_TX__)
