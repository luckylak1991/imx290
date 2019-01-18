//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2018  Macro Image Technology Co., LTd. , All rights reserved
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

#ifndef		__APP_SYNC_LOCK_H__
#define		__APP_SYNC_LOCK_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
typedef	struct
{
	BYTE 	Lock_Time ;					//Before locking, complate initial routine
	BOOL	Lock_extsync;				//lock source external video.

}	MDIN_SYNC_LOCK_PARAM, *PMDIN_SYNC_LOCK_PARAM;

typedef	enum {
	LockTime_DontCare=0,		
	LockTime_AfterBoot,   
	LockTime_Passed,  
}	AppSyncLock_Mode_t;

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void SetApp_Init_SyncLockPARAM(void);
void SetAPP_SyncLock_Handler(void);


#endif	/* __APP_4K60_H__ */
