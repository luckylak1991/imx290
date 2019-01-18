//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2018  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	IT66121.c
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include "it66121_inc.h"
#include "video.h"


#if defined(__USE_IT66121_TX__)


// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
BYTE IT66121CSC=0;						// 0:rgb444(709), 1:rgb444(601), 2:yc422
BYTE IT66121Frmt=HDMI_1080p60;
BYTE IT66121Tick=0;
TimerTask_Description TimerTask;
HDMI_Colorimetry UserColorimetry=HDMI_ITU709;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
void IT66121_Init(void)
{
	InitHDMITX_Variable();		// load default parameter
	InitHDMITX();				// init register value
	IT66121_SetFrmt();			// set hdtx format with isp v.out frmt
}

// ----------------------------------------------------------------------
void IT66121_ProcHanlder(void)
{
	if(IT66121Tick)	{
		HDMITX_DevLoopProc();
		IT66121Tick = FALSE;
	}
}

// ----------------------------------------------------------------------
void ISPM SetIT66121_Tick(void)
{
	IT66121Tick = TRUE;			// tick it66121 loop
}

// ----------------------------------------------------------------------
void IT66121_SetCSC(WORD val)	// 0:rgb444(709), 1:rgb444(601), 2:yc422
{
	IT66121CSC = val;
	UserColorimetry = (val==1)?  HDMI_ITU601:  HDMI_ITU709;
	
	HDMITX_ChangeDisplayOption(IT66121Frmt, (IT66121CSC<2)?  HDMI_RGB444:  HDMI_YUV422);
}

// ----------------------------------------------------------------------
BYTE IT66121_GetCSC(void)
{
	return	IT66121CSC;
}

//--------------------------------------------------------------------------------------
void IT66121_SetFrmt(void)
{
	switch(GetOUT_MainFrmt()) {
		case MISP_VIDOUT_720x480i60:    IT66121Frmt = HDMI_480i60;	break;
		case MISP_VIDOUT_720x576i50:    IT66121Frmt = HDMI_576i50;	break;
//		case MISP_VIDOUT_960x480i60:    break;
//		case MISP_VIDOUT_960x576i50:    break;
		case MISP_VIDOUT_720x480p60:    IT66121Frmt = HDMI_480p60;	break;
		case MISP_VIDOUT_720x576p50:    IT66121Frmt = HDMI_576p50;	break;
		case MISP_VIDOUT_1280x720p60:   IT66121Frmt = HDMI_720p60;	break;
		case MISP_VIDOUT_1280x720p50:   IT66121Frmt = HDMI_720p50;	break;
		case MISP_VIDOUT_1280x720p30:   IT66121Frmt = HDMI_720p30;	break;
		case MISP_VIDOUT_1280x720p25:   IT66121Frmt = HDMI_720p25;	break;
		case MISP_VIDOUT_1280x720p24:   IT66121Frmt = HDMI_720p24;	break;
		case MISP_VIDOUT_1920x1080i60:  IT66121Frmt = HDMI_1080i60;	break;
		case MISP_VIDOUT_1920x1080i50:  IT66121Frmt = HDMI_1080i50;	break;
		case MISP_VIDOUT_1920x1080p60:  IT66121Frmt = HDMI_1080p60;	break;
		case MISP_VIDOUT_1920x1080p50:  IT66121Frmt = HDMI_1080p50;	break;
		case MISP_VIDOUT_1920x1080p30:  IT66121Frmt = HDMI_1080p30;	break;
		case MISP_VIDOUT_1920x1080p25:  IT66121Frmt = HDMI_1080p25;	break;
		case MISP_VIDOUT_1920x1080p24:  IT66121Frmt = HDMI_1080p24;	break;
		case MISP_VIDOUT_1280x720p120:  IT66121Frmt = HDMI_720p120;	break;
		case MISP_VIDOUT_1280x720p100:  IT66121Frmt = HDMI_720p100;	break;
#if defined(__USE_59Hz_OUTPUT__)
		case MISP_VIDOUT_1280x720p59:   IT66121Frmt = HDMI_720p60;	break;
		case MISP_VIDOUT_1920x1080i59:  IT66121Frmt = HDMI_1080i60;	break;
		case MISP_VIDOUT_1920x1080p59:  IT66121Frmt = HDMI_1080p60;	break;
		case MISP_VIDOUT_1920x1080p29:  IT66121Frmt = HDMI_1080p30;	break;
#endif
		default:                   	IT66121Frmt = HDMI_1080p60;	break;
	}
	
	HDMITX_ChangeDisplayOption(IT66121Frmt, (IT66121CSC<2)?  HDMI_RGB444:  HDMI_YUV422);
}

//--------------------------------------------------------------------------------------
void CreatTimerTask(BYTE cTimerTask, WORD Acount, BYTE AutoRun)
{
    TimerTask.AimCount=Acount;
	TimerTask.TickCount=get_ms();
	TimerTask.name=cTimerTask;
	TimerTask.AutoRun=AutoRun;
}

//--------------------------------------------------------------------------------------
static DWORD CalTimer(DWORD SetupCnt)
{
    if(SetupCnt > get_ms())	{
        return (0xffffffff - (SetupCnt - get_ms()));
    }
    else	{
        return (get_ms() - SetupCnt);
    }
}

//--------------------------------------------------------------------------------------
static BOOL DestructTimerTask(BYTE cTimerTask)
{
	TimerTask.name=SysTimerTask_non;
   	return TRUE;
}

//--------------------------------------------------------------------------------------
BOOL TimeOutCheck(BYTE cTimerTask)
{
	if(TimerTask.name==SysTimerTask_non)
		return TRUE;
	else
		return FALSE;
}

//--------------------------------------------------------------------------------------
void ProcessMultiTimerTask()
{
    if(TimerTask.name==SysTimerTask_non)	return;
    
    if(CalTimer(TimerTask.TickCount) > TimerTask.AimCount)	{	// time out
        if(TimerTask.AutoRun==SysTimerTask_AutoRun)
            TimerTask.TickCount=get_ms();
        else
            DestructTimerTask(TimerTask.name);
    }
}


#endif	// __USE_IT66121_TX__

