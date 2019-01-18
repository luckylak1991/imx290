//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2017  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	APP_4K60.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"
#include	"osd.h"

#if defined(__USE_4K60_INOUT__)

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
o4K60_CTRL_PARAM	st4K60CTRL;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//------------------------------------------------------------------------------------------
void SetApp_Init_4k60(void)
{
	memset(&st4K60CTRL, 0, sizeof(o4K60_CTRL_PARAM));
	
#if defined(__USE_4K60_MD1__)
	DEMO_SetMenuPosition(1);	SetMenuStatus(MENU_POSITION_MENU, 1);	// 1:top_left
#endif
	
#if defined(__USE_4K60_MD2__)	// 4k60 slave only
	stWBCTRL.ctrl &= (~WBCTRL_MODE_MASK);
	stWBCTRL.ctrl |= (WBCTRL_MANUAL_MODE|WBCTRL_TRIGGER_ISP);
	
	SetDYNT_Mode(4);	// set day&night 'slave' mode
	//SetSS_image_start_point(1930 , GetSS_image_start_pos_v());	// 4k30 sensor out only(no need 4k60 sensor out)
#endif
}


//--------------------------------------------------------------------------------------
void SetAPP_Set4K30P60P_Mode(BOOL OnOff)
{
	if(IsOUT_MainFrmt12G() == FALSE)	{
		OSD_MessageBox(ON, "V.OUT IS NOT 4K60 !!   ");
		return;		// check v.out frmt
	}
	
#if defined(__USE_4K60_MD1__)
	SPImAPP_SetSlave4K30P60POSD(OnOff);
#if defined(__USE_OSD_TITLE__)
	OSD_SetTitleBMP(OnOff,  0x05);		// 2 : 4k30p, 5 : 4k60p
#endif
#endif

#if defined(__USE_4K60_MD2__)
	st4K60CTRL.f4K30P60P_mode = OnOff;
#endif
}

//--------------------------------------------------------------------------------------
void ISPM SetAPP_Set4K30P60P_Tick(void)
{
#if defined(__USE_4K60_MD2__)
	st4K60CTRL.f4K30P60P_tick = TRUE;
#endif
}

//--------------------------------------------------------------------------------------
void SetAPP_Set4K30P60P_Handler(void)
{
#if defined(__USE_4K60_MD2__)
	if((st4K60CTRL.f4K30P60P_mode == ON) && (st4K60CTRL.f4K30P60P_tick == TRUE))	{
		MISP_RegField(MISP_LOCAL_ID, 0x040,  1, 1, (st4K60CTRL.f4K30P60P_freeze)?  1:  0);	// freeze on/off
		
		st4K60CTRL.f4K30P60P_freeze = ~st4K60CTRL.f4K30P60P_freeze;
		st4K60CTRL.f4K30P60P_tick = FALSE;
	}
#endif
}


//--------------------------------------------------------------------------------------
void SetAPP_Set4K60OutFrmt(WORD frmt)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveOutputFormat(frmt);
#endif
	
#if defined(__USE_SIL9136_TX__)
//	SIL9136_SetPowerDown();						// hdmi-tx tmds disable
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = frmt;
	EEPROM_MultiWrite(EEPROM_SETUP_OUTFRMT, (PBYTE)&buff[0], sizeof(buff));
#endif
	
	MISP_Reboot();
}

//--------------------------------------------------------------------------------------
void SetAPP_Set4KOutSyncLock(void)
{
	Delay_mSec(8);
	MISP_RegField(MISP_LOCAL_ID, 0x099, 15,  1, 0);		// 'output_sync_reset_en' after 'soft_reset + 8ms'
	MISP_RegField(MISP_LOCAL_ID, 0x099, 15,  1, 1);
}

//--------------------------------------------------------------------------------------
void SetAPP_Set4KUartStop(BOOL stop)
{
	WORD i;
	
	if(stop)	{										// skip & backup 'uart message'
		st4K60CTRL.UartStop = TRUE;
		st4K60CTRL.UartCnt = 0;
		memset(&st4K60CTRL.UartBuff[0], 0, sizeof(st4K60CTRL.UartBuff));
	}
	else	{
		st4K60CTRL.UartStop = FALSE;
		
		for(i=0;i<st4K60CTRL.UartCnt;i++)	{			// restore 'uart message'
			UARTprintf("%c", st4K60CTRL.UartBuff[i]);
		}
	}
}

//--------------------------------------------------------------------------------------
BOOL SetAPP_Get4KUartStop(void)
{
	return	st4K60CTRL.UartStop;
}

//--------------------------------------------------------------------------------------
void SetAPP_Set4KUartBackup(char ch)
{
	if(st4K60CTRL.UartCnt < 2000)		st4K60CTRL.UartBuff[st4K60CTRL.UartCnt++] = ch;
}


#endif	//__USE_4K60_INOUT__

/*  FILE_END_HERE */
