//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2017  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  APP_4K60.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__APP_4K60_H__
#define		__APP_4K60_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
typedef	struct
{
	LONG	ctrl;					// control flag
	
	BOOL	f4K30P60P_mode;			// 4k30/60 demo mode on/off flag
	BOOL	f4K30P60P_tick;			// 4k30 freeze loop trigger
	BOOL	f4K30P60P_freeze;		// 4k30 freeze on/off flag
	BYTE	UartStop;				// uart message stop on/off flag
	WORD	UartCnt;				// uart message buffer counter
	char	UartBuff[2000];			// uart message buffer

}	o4K60_CTRL_PARAM, *Po4K60_CTRL_PARAM;


#define	__USE_LATTICE_LCMX03LF__		// use lattice machx03 fpga


// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void SetApp_Init_4k60(void);
void SetAPP_Set4K30P60P_Mode(BOOL OnOff);
void ISPM SetAPP_Set4K30P60P_Tick(void);
void SetAPP_Set4K30P60P_Handler(void);
void SetAPP_Set4K60OutFrmt(WORD frmt);
void SetAPP_Set4KOutSyncLock(void);
void SetAPP_Set4KUartStop(BOOL stop);
BOOL SetAPP_Get4KUartStop(void);
void SetAPP_Set4KUartBackup(char ch);


#endif	/* __APP_4K60_H__ */
