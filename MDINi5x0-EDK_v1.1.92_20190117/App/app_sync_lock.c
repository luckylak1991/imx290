//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2018  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	APP_SYNC_LOCK.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"
#include	"misp100.h"

static MDIN_SYNC_LOCK_PARAM stSyncLOCK ;


// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
static void SetApp_Init_SyncLock(void);
// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//------------------------------------------------------------------------------------------
void SetApp_Init_SyncLockPARAM(void)
{
#if defined(__OUT_VCLKPLL_LOCK__) && (defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__))
	stSyncLOCK.Lock_Time = LockTime_AfterBoot;
#else
	stSyncLOCK.Lock_Time = LockTime_DontCare;
#endif

	stSyncLOCK.Lock_extsync = OFF;
}
//--------------------------------------------------------------------------------------
void SetAPP_SyncLock_Handler(void)
{
	SetApp_Init_SyncLock();
}
//--------------------------------------------------------------------------------------
static void SetApp_Init_SyncLock(void)
{
	if(stSyncLOCK.Lock_Time==LockTime_AfterBoot){

		// The system stable should be preceded before lvds tx output with vout_pll lock. 
		SetCLK_UserPLL_LockMode(2);	SetCLK_UserPLL_LockSrcPort(1);	

		if(GetCLK_UserPLL_LockMode()==1)		SetCLK_UserPLLSrcSelect(4);	//vclk pll int path			//statndard v-clock out such as 37.125, 74.25...
		else if(GetCLK_UserPLL_LockMode()==2){	
			SetCLK_UserPLLSrcSelect(6);	
					
			SetCLK_UserPLL(1,4,33,3,1) ;	
			MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x0421);	// pll source ---> pll --> div 1 --> user_clock out		

			if(GetCLK_UserPLL_LockSrcPort()==1) SetCLK_UserPLLSrc(3);
			else if(GetCLK_UserPLL_LockSrcPort()==2) SetCLK_UserPLLSrc(2);		
		}

		SetOUT_ExtLockHSdelay(41) ; 
		SetOUT_ExtLockVSdelay(18) ;	
			
		SetOUT_ExtLockMode(1,1) ; // Main sync lock mode with sync reset delay enable
		SetOUT_ExtSyncMode(0) ; //  sync normal mode [ Main sync ]	

		stSyncLOCK.Lock_Time=LockTime_Passed;
	 
	}
	else if(stSyncLOCK.Lock_Time==LockTime_DontCare){	
	#if defined(__OUT_VCLKPLL_LOCK__)
//		SetOUT_ExtLockHSdelay(41) ; 
//		SetOUT_ExtLockVSdelay(18) ;	
			
		SetOUT_ExtLockMode(1,5) ; // setting value match what your system
		SetOUT_ExtSyncMode(0) ; //  sync normal mode [ Main sync ]		
		UARTprintf("### Sync Lock One time run ###\r\n");
	#endif

		stSyncLOCK.Lock_Time=LockTime_Passed;
	}
	else stSyncLOCK.Lock_Time=LockTime_Passed;

}

//--------------------------------------------------------------------------------------
/*  FILE_END_HERE */
