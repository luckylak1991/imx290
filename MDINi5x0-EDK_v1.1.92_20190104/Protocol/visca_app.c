//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2015  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	VISCA_APP.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"
#include	"osd.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
extern void Reset_Start(void);

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
static void VISCAAPP_WBManGainR(BYTE gain)
{
	LONG mode = stWBCTRL.ctrl&WBCTRL_MODE_MASK;

	stWBCTRL.stMANUAL.R = 106 + gain*118/100;		// 106 ~ 406

	stWBCTRL.ctrl &= (~WBCTRL_MODE_MASK);
	stWBCTRL.ctrl |= (mode|WBCTRL_TRIGGER_ISP);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCAAPP_WBManGainB(BYTE gain)
{
	LONG mode = stWBCTRL.ctrl&WBCTRL_MODE_MASK;

	stWBCTRL.stMANUAL.B = 106 + gain*118/100;		// 106 ~ 406

	stWBCTRL.ctrl &= (~WBCTRL_MODE_MASK);
	stWBCTRL.ctrl |= (mode|WBCTRL_TRIGGER_ISP);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCAAPP_SetEdgeEnhance(BYTE val)	// 0 ~ 0xff
{
	SetApp_AutoEDGE_HVPFLevel(MIN(127, val/2));
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCAAPP_SetWDROnOff(BOOL OnOff)
{
	//WDR on/off
	SetWDR_ProcesMode(GetWDR_ProcesList(OnOff));
	
	//initial setting
	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_FRAME:
			SetWDR_BypassPath((MISP_WDR_FVIEW_t)0);   // Bypass off
#if WDR_TM_SIZE_CTRL
			SetWDR_TmStrength(8);   // TM strength
			SetWDR_TmContrast(2);   // TM local contrast
#else
			SetWDR_TmStrength(12);  // TM strength
			SetWDR_MdRegister(7);   // MGR motion threshold
#endif
			SetWDR_AutoExpose(1);   // AE on
			SetWDR_AutoTarget(10);  // AE target bright level
			SetWDR_ExposRatio(1);   // Shutter ratio (0:[1:16], 1:[1:32])
			break;
		case MISP_WDR_PROC_BUILT:
			SetWDR_BypassPath((MISP_WDR_FVIEW_t)0);   // Bypass off
			SetWDR_TmStrength(12);  // TM strength
#if WDR_TM_SIZE_CTRL
			SetWDR_TmContrast(3);   // TM local contrast
#endif
			SetWDR_AutoExpose(1);   // AE on
			SetWDR_AutoTarget(10);  // AE target bright level
			break;
		case MISP_WDR_PROC_SINGL:
			SetWDR_TmStrength(12);  // TM strength
#if WDR_TM_SIZE_CTRL
			SetWDR_TmContrast(3);   // TM local contrast
#endif
			break;
		case MISP_WDR_PROC_LINE2:
			SetWDR_BypassPath((MISP_WDR_FVIEW_t)0);   // Bypass off
#if WDR_TM_SIZE_CTRL
			SetWDR_TmStrength(8);   // TM strength
			SetWDR_TmContrast(2);   // TM local contrast
#else
			SetWDR_TmStrength(12);  // TM strength
			SetWDR_MdRegister(7);   // MGR motion threshold
#endif
			SetWDR_AutoExpose(1);   // AE on
			SetWDR_AutoTarget(10);  // AE target bright level
			SetWDR_ExposRatio(1);   // Shutter ratio (0:[1:16], 1:[1:32])
			break;
		case MISP_WDR_PROC_LINE3:
			SetWDR_BypassPath((MISP_WDR_FVIEW_t)0);   // Bypass off
#if WDR_TM_SIZE_CTRL
			SetWDR_TmStrength(8);   // TM strength
			SetWDR_TmContrast(2);   // TM local contrast
#else
			SetWDR_TmStrength(12);  // TM strength
			SetWDR_MdRegister(7);   // MGR motion threshold
#endif
			SetWDR_AutoExpose(1);   // AE on
			SetWDR_AutoTarget(10);  // AE target bright level
			SetWDR_ExposRatio(1);   // Shutter ratio (0:[1:6:36], 1:[1:8:64])
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCAAPP_DISOnOff(BOOL OnOff)
{
	Demo_DIS_OnOff(OnOff);
}

//--------------------------------------------------------------------------------------------------------------------------
WORD VISCAAPP_GetDZoomPosi(void)
{
	return		MIN(0xEB, GetDZOOM_Ratio()*426/100);	// 32/7.5=4.266
}

//--------------------------------------------------------------------------------------------------------------------------
WORD VISCAAPP_GetZoomPosi(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	return		LENS_GetVISCAPositionZOOM();
#else
	return 	0;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL VISCAAPP_GetDZoomCSMode(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	if(stAFCTRL.ctrl & AFZOOM_DZMIX_OFF)	return 	1;	// separate mode
	else									return		0;	// combine mode
#else
	return		1;											//  0:combine, 1:separate
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL VISCAAPP_GetDZoomMode(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	if(stAFCTRL.ctrl & AFZOOM_DZOOM_OFF)	return 	0;	// dzoom off
	else									return		1;	// dzoom on
#else
	return		0;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCAAPP_SetZoomDrvSpeed(BYTE val)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))

	LONG mode = AFZOOM_SPEED_VAR4;
	
	if(val==7)			mode = AFZOOM_SPEED_VAR7;
	else if(val==6)		mode = AFZOOM_SPEED_VAR6;
	else if(val==5)		mode = AFZOOM_SPEED_VAR5;
	else if(val==4)		mode = AFZOOM_SPEED_VAR4;
	else if(val==3)		mode = AFZOOM_SPEED_VAR3;
	else if(val==2)		mode = AFZOOM_SPEED_VAR2;
	else if(val==1)		mode = AFZOOM_SPEED_VAR1;
	else if(val==0)		mode = AFZOOM_SPEED_VAR0;
	
	LENS_SetZOOMDrvSPEED(&stAFCTRL, mode);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCAAPP_SetAFMode(BYTE val)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	LONG mode = AFCTRL_AFAUTO_MODE;
	
	switch(val)	{
		case 0:		mode = AFCTRL_AFAUTO_MODE;	break;
		case 1:		mode = AFCTRL_INTVAL_MODE;	break;
		case 2:		mode = AFCTRL_AFZOOM_MODE;	break;
	}

	stAFCTRL.ctrl &= (~AFCTRL_AFMODE_MASK);
	stAFCTRL.ctrl |= (mode);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetAFMode(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	switch(stAFCTRL.ctrl & AFCTRL_AFMODE_MASK)	{
		case AFCTRL_AFAUTO_MODE:	return 0;	break;
		case AFCTRL_INTVAL_MODE:	return 1;	break;
		case AFCTRL_AFZOOM_MODE:	return 2;	break;
		default:					return 0;	break;
	}
#else
	return 0;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCAAPP_SetAFActiveTime(BYTE val)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	LENS_SetAFActiveTIME(&stAFCTRL, (WORD)val);					// 0 ~ 0xff[sec]
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetAFActiveTime(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	return		(BYTE)stAFCTRL.tACT;
#else
	return		0;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCAAPP_SetAFIntervalTime(BYTE val)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	LENS_SetAFIntvalTIME(&stAFCTRL, (WORD)val);					// 0 ~ 0xff[sec]
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetAFIntervalTime(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	return		(BYTE)stAFCTRL.tINT;
#else
	return		0;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCAAPP_SetFocusNearLimit(WORD val)
{
#if defined(__USE_AFZOOM_LENS__)
	LENS_SetFOKSDrvLIMIT(&stAFCTRL, (SHORT)val);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
WORD VISCAAPP_GetFocusNearLimit(void)
{
#if defined(__USE_AFZOOM_LENS__)
	return		stAFCTRL.near;
#else
	return		0;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCAAPP_SetFocusAutoOnOff(BOOL OnOff)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	stAFCTRL.ctrl &= (~AFCTRL_AFAUTO_MASK);

	if(OnOff)	stAFCTRL.ctrl |= AFCTRL_AFAUTO_MODE;		// auto
	else		stAFCTRL.ctrl |= AFCTRL_MANUAL_MODE;		// manual
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL VISCAAPP_GetFocusMode(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	switch(stAFCTRL.ctrl & AFCTRL_AFAUTO_MASK)	{
		case  AFCTRL_AFAUTO_MODE:		return 1;	break;	// auto
		case  AFCTRL_MANUAL_MODE:		return 0;	break;	// manual
		default:						return 0;	break;
	}
#else
	return		0;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCAAPP_SetFocusDrvSpeed(BYTE val)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))

	LONG mode = AFFOKS_SPEED_VAR4;
	
	if(val==7)			mode = AFFOKS_SPEED_VAR7;
	else if(val==6)		mode = AFFOKS_SPEED_VAR6;
	else if(val==5)		mode = AFFOKS_SPEED_VAR5;
	else if(val==4)		mode = AFFOKS_SPEED_VAR4;
	else if(val==3)		mode = AFFOKS_SPEED_VAR3;
	else if(val==2)		mode = AFFOKS_SPEED_VAR2;
	else if(val==1)		mode = AFFOKS_SPEED_VAR1;
	else if(val==0)		mode = AFFOKS_SPEED_VAR0;
	
	LENS_SetFOKSDrvSPEED(&stAFCTRL, mode);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
WORD VISCAAPP_GetFocusPosi(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	return 	LENS_GetVISCAPositionFOKS();
#else
	return 	0;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCAAPP_SetFocusOnePushTrig(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	AF_EnableProcess(ON);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCAAPP_SetDZOOMTele(BYTE speed)
{
	if(stVISCAINFO.lens.ctrl1 & VISCA_DZOOM_COMBINE_OFF)	{		// enable during separate mode
		SetDZOOM_DriveTeleVar(speed);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCAAPP_SetDZOOMWide(BYTE speed)
{
	if(stVISCAINFO.lens.ctrl1 & VISCA_DZOOM_COMBINE_OFF)	{		// enable during separate mode
		SetDZOOM_DriveWideVar(speed);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCAAPP_SetRepeatCmd(WORD cmd)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	WORD z_posi=stVISCAINFO.lens.zoom_posi, f_posi=stVISCAINFO.lens.focus_posi;
#endif
	
	switch(cmd)	{
		case  0x0600:		SetMsgCommandComplete(ON);			break;	// dzoom stop
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
		case  0x0700:		SetMsgCommandComplete(ON);			break;	// zoom stop
		case  0x0702:		SetDirDriveZOOM(zTELE);				break;	// zoom tele(in)
		case  0x0703:		SetDirDriveZOOM(zWIDE);				break;	// zoom wide(out)
		case  0x0800:		SetMsgCommandComplete(ON);			break;	// focus stop
		case  0x0802:		SetDirDriveFOKS(fFAR);				break;	// focus far
		case  0x0803:		SetDirDriveFOKS(fNEAR);				break;	// focus near
		case  0x470D:		TargetDriveZOOM(z_posi);			break;	// zoom direct
		case  0x470F:		TargetDriveLENS(z_posi, f_posi);	break;	// zoom&focus direct
		case  0x480D:		TargetDriveFOKS(f_posi);			break;	// focus direct
#endif
#if defined(__USE_AFZOOM_LENS__)
		case  0xCF02:		SetDirDrivePINT(zWIDE);				break;	// wide direct
		case  0xCF03:		SetDirDrivePINT(zTELE);				break;	// tele direct
#endif
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
		case  0xCF22:		SetWBRAWViewShiftL();				break;	// move window
		case  0xCF23:		SetWBRAWViewShiftR();				break;	// move window
		case  0xCF24:		SetWBRAWViewShiftU();				break;	// move window
		case  0xCF25:		SetWBRAWViewShiftD();				break;	// move window
#endif
	}
	
	switch(cmd&0xFFF0)	{
		case  0x0620:		VISCAAPP_SetDZOOMTele(cmd&0x0F);	break;	// dzoom tele(in)
		case  0x0630:		VISCAAPP_SetDZOOMWide(cmd&0x0F);	break;	// dzoom wide(out)
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
		case  0x0720:		SetDirDriveZOOM(zTELE);				break;	// zoom tele(in)
		case  0x0730:		SetDirDriveZOOM(zWIDE);				break;	// zoom wide(out)
		case  0x0820:		SetDirDriveFOKS(fFAR);				break;	// focus far
		case  0x0830:		SetDirDriveFOKS(fNEAR);				break;	// focus near
#endif
	}
	
	// for data screen display
	switch(cmd&0xFFF0)	{
		case  0x0600:	case  0x0620:	case  0x0630:					// dzoom stop, tele(in), wide(out)
						stVISCAINFO.efunc1.dzoom_posi = VISCAAPP_GetDZoomPosi();
						stVISCAINFO.user.dscr_disptime_cnt=0;	break;	// clear dscr display time counter
		case  0x0700:	case  0x0720:	case  0x0730:					// zoom stop, tele(in), wide(out)
		case  0x4700:													// zoom direct, zoom&focus direct
						stVISCAINFO.user.dscr_disptime_cnt=0;	break;	// clear dscr display time counter
	}
}

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PowerOnOff(BOOL OnOff)
{
	if(OnOff)	stVISCAINFO.other.ctrl1 |= VISCA_POWER_ON;
	else		stVISCAINFO.other.ctrl1 &= ~VISCA_POWER_ON;
	
	// UARTprintf("[VISCA] Power On/Off[0x%x] not defined..\r\n", stVISCAINFO.other.ctrl1);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ZoomStop(void)
{
	// blank
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ZoomTeleStd(void)
{
	SetNR_NRZoomingMode();		VISCAAPP_SetZoomDrvSpeed(3);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ZoomWideStd(void)
{
	SetNR_NRZoomingMode();		VISCAAPP_SetZoomDrvSpeed(3);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ZoomTeleVar(BYTE speed)
{
	SetNR_NRZoomingMode();		VISCAAPP_SetZoomDrvSpeed(speed);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ZoomWideVar(BYTE speed)
{
	SetNR_NRZoomingMode();		VISCAAPP_SetZoomDrvSpeed(speed);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ZoomDirect(WORD posi)
{
	stVISCAINFO.lens.zoom_posi = posi;
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExeRepeatCmd(WORD cmd)
{
	VISCAAPP_SetRepeatCmd(cmd);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_DZoomStop(void)
{
	stVISCAINFO.efunc1.dzoom_posi = VISCAAPP_GetDZoomPosi();
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_DZoomOnOff(BOOL OnOff)
{
	if(OnOff)	stVISCAINFO.lens.ctrl1 |= VISCA_DZOOM_ON;			// dzoom on in combine mode
	else		stVISCAINFO.lens.ctrl1 &= (~VISCA_DZOOM_ON);		// dzoom off in combine mode
	
	if(OnOff)	if(GetDIS_OnOff())		VISCAAPP_StabilizerOnOff(OFF);		// dis off when dzoom on
	
	SetDZOOM_Status(OnOff);
	
	if(OnOff==OFF)	{
		SetDZOOM_DirectRatio(0);	stVISCAINFO.efunc1.dzoom_posi=0;	// dzoom ratio set 0 when dzoom off
	}
	
	stVISCAINFO.user.dscr_disptime_cnt=0;						// clear dscr display time counter

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	LENS_SetDIGIDrvOnOff(&stAFCTRL, (OnOff==ON)? ON : OFF);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_DZoomSwitch(void)
{
	if(stVISCAINFO.lens.ctrl1 & VISCA_DZOOM_COMBINE_OFF)	{		// enable during separate mode
		if(GetDIS_OnOff())		VISCAAPP_StabilizerOnOff(OFF);		// dis off when dzoom on
		
		if(VISCAAPP_GetDZoomPosi())		{SetDZOOM_DirectRatio(0);						stVISCAINFO.efunc1.dzoom_posi=0;}		// xx -> x1
		else							{SetDZOOM_DirectRatio(DZOOMCTRL_RATIO_MAX);		stVISCAINFO.efunc1.dzoom_posi=0xEB;}	// x1 -> Max
		
		stVISCAINFO.user.dscr_disptime_cnt=0;						// clear dscr display time counter
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_DZoomTeleVar(BYTE speed)
{
	if(GetDIS_OnOff())		VISCAAPP_StabilizerOnOff(OFF);		// dis off when dzoom in
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_DZoomWideVar(BYTE speed)
{
	if(GetDIS_OnOff())		VISCAAPP_StabilizerOnOff(OFF);		// dis off when dzoom out
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_DZoomCombine(BOOL OnOff)
{
	if(OnOff)	stVISCAINFO.lens.ctrl1 &= (~VISCA_DZOOM_COMBINE_OFF);
	else		stVISCAINFO.lens.ctrl1 |= VISCA_DZOOM_COMBINE_OFF;
	
	if(GetDIS_OnOff())			VISCAAPP_StabilizerOnOff(OFF);		// dis off when dzoom on/off
	SetDZOOM_DirectRatio(0);	stVISCAINFO.efunc1.dzoom_posi=0;	// dzoom ratio set 0 when combine on/off
	
	stVISCAINFO.user.dscr_disptime_cnt=0;						// clear dscr display time counter

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	LENS_SetDIGIMixOnOff(&stAFCTRL, (OnOff==ON)? ON : OFF);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_DZoomDirect(BYTE posi)
{
	if(stVISCAINFO.lens.ctrl1 & VISCA_DZOOM_COMBINE_OFF)	{	// enable during separate mode
		stVISCAINFO.efunc1.dzoom_posi = posi;
		
		if(GetDIS_OnOff())		VISCAAPP_StabilizerOnOff(OFF);		// dis off when dzoom direct
		SetDZOOM_DirectRatio(posi*100/426);	// x1(00), x2(128,7.5), x3(170,7.52),,x12(235, 7.48) -> 32 / 7.5 = 4.266..
		stVISCAINFO.user.dscr_disptime_cnt=0;			// clear dscr display time counter
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_FocusStop(void)
{
	// blank
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_FocusFarStd(void)
{
	VISCAAPP_SetFocusDrvSpeed(3);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_FocusNearStd(void)
{
	VISCAAPP_SetFocusDrvSpeed(3);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_FocusFarVar(BYTE speed)
{
	VISCAAPP_SetFocusDrvSpeed(speed);		// 0 ~ 7
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_FocusNearVar(BYTE speed)
{
	VISCAAPP_SetFocusDrvSpeed(speed);		// 0 ~ 7
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_FocusDirect(WORD posi)
{
	stVISCAINFO.lens.focus_posi = posi;
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_FocusAutoOnOff(BOOL OnOff)
{
	if(OnOff)	stVISCAINFO.lens.ctrl1 |= VISCA_FOCUS_MODE_AUTO;		// auto
	else		stVISCAINFO.lens.ctrl1 &= (~VISCA_FOCUS_MODE_AUTO);		// manual
	
	VISCAAPP_SetFocusAutoOnOff(OnOff);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_FocusAutoManual(void)
{
	// add function here
	// UARTprintf("[VISCA] Focus Auto/Manual not defined..\r\n");
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_FocusOnepushTrig(void)
{
	VISCAAPP_SetFocusOnePushTrig();
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_FocusNearLimit(WORD posi)
{
	VISCAAPP_SetFocusNearLimit(posi);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_AFSensitivity(BOOL flag)
{
	stVISCAINFO.lens.ctrl1 &= (~VISCA_AFSENS_NORMAL);
	stVISCAINFO.lens.ctrl1 |= (flag)?  VISCA_AFSENS_NORMAL:  VISCA_AFSENS_LOW;
	// add function here
	// UARTprintf("[VISCA] AF Sensitivity [%s] not defined..\r\n", (flag)? "Normal": "Low");
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_AFMode(BYTE mode)
{
	VISCAAPP_SetAFMode(mode);									// 0:normal, 1:interval, 2:zoom trigger
	stVISCAINFO.lens.ctrl1 &= (~VISCA_AFMODE_MASK);
	stVISCAINFO.lens.ctrl1 |= (mode<<3);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_AFIntervalTime(BYTE act, BYTE inter)
{
	VISCAAPP_SetAFActiveTime(act);							// 0 ~ 255[sec]
	VISCAAPP_SetAFIntervalTime(inter);						// 0 ~ 255[sec]
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_IRCorrectionStandard(void)
{
	// add function here
	// UARTprintf("[VISCA] IR Correction 'Standard' not defined..\r\n");
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_IRCorrectionIRLight(void)
{
	// add function here
	// UARTprintf("[VISCA] IR Correction 'IR Light' not defined..\r\n");
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ZoomFocusDirect(WORD z_posi, WORD f_posi)
{
	stVISCAINFO.lens.zoom_posi = z_posi;
	stVISCAINFO.lens.focus_posi = f_posi;
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_LensInit(void)
{
	// add function here
	// UARTprintf("[VISCA] Lens Initialize not defined..\r\n");
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_CameraInit(void)
{
	// add function here
	// UARTprintf("[VISCA] Camera Initialize not defined..\r\n");
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_WBMode(BYTE mode)
{
	switch(mode)	{
		case  0x00:		SetWBModeSetMenu(0);		break;		// auto
		case  0x01:		SetWBModeSetMenu(2);		break;		// indoor
		case  0x02:		SetWBModeSetMenu(3);		break;		// outdoor
		case  0x03:		SetWBModeSetMenu(1);		break;		// one push
		case  0x04:		SetWBModeSetMenu(0);		break;		// atw
		case  0x05:		SetWBModeSetMenu(4);		break;		// manual
		case  0x06:		// add function here					// outdoor auto
		case  0x07:		// add function here					// sodium lamp auto
		case  0x08:		// add function here					// sodium lamp
		case  0x09:		// add function here					// sodium lamp outdoor auto
						// UARTprintf("[VISCA] WB Mode[%d] not defined \r\n", mode);
						SetWBModeSetMenu(0);		break;
	}
	
	stVISCAINFO.cam.wb_mode = mode;
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_WBOnepushTrig(void)
{
	SetWBCtrlOnOff();											// one push wb trigger
	stVISCAINFO.user.ctrl |= VISCA_WB_ONEPUSH_DONE;				// set one push flag
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_WBRGainReset(void)
{
	stVISCAINFO.cam.rgain = 127;
	VISCAAPP_WBManGainR(stVISCAINFO.cam.rgain);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_WBRGainUp(void)
{
	if(stVISCAINFO.cam.rgain < 0xff)	stVISCAINFO.cam.rgain++;
	VISCAAPP_WBManGainR(stVISCAINFO.cam.rgain);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_WBRGainDown(void)
{
	if(stVISCAINFO.cam.rgain > 0x00)	stVISCAINFO.cam.rgain --;
	VISCAAPP_WBManGainR(stVISCAINFO.cam.rgain);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_WBRGainDirect(BYTE gain)
{
	stVISCAINFO.cam.rgain = gain;
	VISCAAPP_WBManGainR(stVISCAINFO.cam.rgain);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_WBBGainReset(void)
{
	stVISCAINFO.cam.bgain = 127;
	VISCAAPP_WBManGainB(stVISCAINFO.cam.bgain);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_WBBGainUp(void)
{
	if(stVISCAINFO.cam.bgain < 0xff)	stVISCAINFO.cam.bgain++;
	VISCAAPP_WBManGainB(stVISCAINFO.cam.bgain);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_WBBGainDown(void)
{
	if(stVISCAINFO.cam.bgain > 0x00)	stVISCAINFO.cam.bgain --;
	VISCAAPP_WBManGainB(stVISCAINFO.cam.bgain);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_WBBGainDirect(BYTE gain)
{
	stVISCAINFO.cam.bgain = gain;
	VISCAAPP_WBManGainB(stVISCAINFO.cam.bgain);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_AEMode(BYTE mode)
{
	if(stVISCAINFO.cam.exposure_mode != mode)	{
		switch(mode)	{
			case  VISCA_AEMODE_AUTO:		SetAECtrlOnOff(ON);		AECTRL_SetAE_AutoMode();		break;	// auto
			case  VISCA_AEMODE_MANUAL:		SetAECtrlOnOff(OFF);		AECTRL_SetAE_Manual();			break;	// manual
			case  VISCA_AEMODE_SHUTTERFIX:	SetAECtrlOnOff(ON);		AECTRL_SetAE_ShutterFixMode();	break;	// shutter priority
			case  VISCA_AEMODE_IRISFIX:		SetAECtrlOnOff(ON);		AECTRL_SetAE_IRISFixMode();		break;	// iris priority
			case  VISCA_AEMODE_BRIGHT:		SetAECtrlOnOff(OFF);										break;	// manual bright mode
		}
		
		stVISCAINFO.cam.exposure_mode = mode;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_AutoSlowShutterOnOff(BOOL OnOff)
{
	if(OnOff)	stVISCAINFO.cam.ctrl |= VISCA_SLOWSHUTTER_ON;
	else		stVISCAINFO.cam.ctrl &= (~VISCA_SLOWSHUTTER_ON);

	if(OnOff) {
		AECTRL_SetSensUpMax(stVISCAINFO.cam.sensup_posi);
	} else {
		stVISCAINFO.cam.sensup_posi = 0;
		AECTRL_SetSensUpMax(0);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_AutoSlowShutterSetLimit(BYTE val)
{
	stVISCAINFO.cam.ctrl |= VISCA_SLOWSHUTTER_ON;

	switch(val) {
		case 1 :	//AECTRL_SetSensUpMax(0);
				DEMO_SetExpSensUp(0,2);
				stVISCAINFO.cam.sensup_posi = 0;
				break; // same to sensup off
		case 2 :  //AECTRL_SetSensUpMax(1);
				stVISCAINFO.cam.sensup_posi = 1;
				DEMO_SetExpSensUp(1,2);
				break;
		case 3 :  //AECTRL_SetSensUpMax(3);
				stVISCAINFO.cam.sensup_posi = 3;
				DEMO_SetExpSensUp(3,2);
				break;
		case 4 :  //AECTRL_SetSensUpMax(5);
				stVISCAINFO.cam.sensup_posi = 5;
				DEMO_SetExpSensUp(5,2);
				break;
		case 5 :  //AECTRL_SetSensUpMax(7);
				stVISCAINFO.cam.sensup_posi = 7;
				DEMO_SetExpSensUp(7,2);
				break;
		case 6 :  //AECTRL_SetSensUpMax(9);
				stVISCAINFO.cam.sensup_posi = 9;
				DEMO_SetExpSensUp(9,2);
				break;
		default : //AECTRL_SetSensUpMax(4);
				stVISCAINFO.cam.sensup_posi = 5;
				DEMO_SetExpSensUp(5,2);
				break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ShutterReset(void)
{
	stVISCAINFO.cam.shutter_posi = 0x05;			// 1/30
	VISCAAPP_ShutterDirect(stVISCAINFO.cam.shutter_posi);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ShutterUp(void)
{
	if(stVISCAINFO.cam.shutter_posi < 0x15)		stVISCAINFO.cam.shutter_posi++;
	VISCAAPP_ShutterDirect(stVISCAINFO.cam.shutter_posi);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ShutterDown(void)
{
	if(stVISCAINFO.cam.shutter_posi > 0x00)		stVISCAINFO.cam.shutter_posi--;
	VISCAAPP_ShutterDirect(stVISCAINFO.cam.shutter_posi);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ShutterDirect(BYTE posi)
{
	switch(posi)	{
		case  0x00:										// 1/1
		case  0x01:										// 1/2
		case  0x02:										// 1/4
		case  0x03:										// 1/8
		case  0x04:										// 1/15
		case  0x05:		SetAEShutSpeed(1);	break;		// 1/30
		case  0x06:		SetAEShutSpeed(2);	break;		// 1/60
		case  0x07:										// 1/90
		case  0x08:		SetAEShutSpeed(3);	break;		// 1/100
		case  0x09:										// 1/125
		case  0x0A:		SetAEShutSpeed(4);	break;		// 1/180
		case  0x0B:		SetAEShutSpeed(6);	break;		// 1/250
		case  0x0C:										// 1/350
		case  0x0D:		SetAEShutSpeed(7);	break;		// 1/500
		case  0x0E:										// 1/725
		case  0x0F:		SetAEShutSpeed(8);	break;		// 1/1000
		case  0x10:										// 1/1500
		case  0x11:		SetAEShutSpeed(9);	break;		// 1/2000
		case  0x12:										// 1/3000
		case  0x13:		SetAEShutSpeed(10);	break;		// 1/4000
		case  0x14:										// 1/6000
		case  0x15:		SetAEShutSpeed(11);	break;		// 1/10000
	}
	
	stVISCAINFO.cam.shutter_posi = posi;
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_IrisReset(void)
{
	stVISCAINFO.cam.iris_posi = 0x11;				// F1.6
	VISCAAPP_IrisDirect(stVISCAINFO.cam.iris_posi);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_IrisUp(void)
{
	if(stVISCAINFO.cam.iris_posi < 0x11)	stVISCAINFO.cam.iris_posi++;
	if(stVISCAINFO.cam.iris_posi < 0x05)	stVISCAINFO.cam.iris_posi = 0x05;	// 0~4(close), 5(F14), 6(F11),, 0x11(F1.6)
	
	VISCAAPP_IrisDirect(stVISCAINFO.cam.iris_posi);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_IrisDown(void)
{
	if(stVISCAINFO.cam.iris_posi > 0x00)	stVISCAINFO.cam.iris_posi--;
	if(stVISCAINFO.cam.iris_posi < 0x05)	stVISCAINFO.cam.iris_posi = 0x00;	// 0~4(close), 5(F14), 6(F11),, 0x11(F1.6)
	
	VISCAAPP_IrisDirect(stVISCAINFO.cam.iris_posi);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_IrisDirect(BYTE posi)
{
	SetAEIrisBRGain(posi);
	stVISCAINFO.cam.iris_posi = posi;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetIrisPosi(void)
{
#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
	return	(DCLENS_GetIRISValue() < 5)?  0:  DCLENS_GetIRISValue();	// 0~4(close), 5(F14), 6(F11),, 0x11(F1.6)
#else
	return 0;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_GainReset(void)
{
	stVISCAINFO.cam.gain_posi = 0x01;				// 0 step
	VISCAAPP_GainDirect(stVISCAINFO.cam.gain_posi);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_GainUp(void)
{
	if(stVISCAINFO.cam.gain_posi < 0x0F)	stVISCAINFO.cam.gain_posi++;
	VISCAAPP_GainDirect(stVISCAINFO.cam.gain_posi);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_GainDown(void)
{
	if(stVISCAINFO.cam.gain_posi > 0x00)	stVISCAINFO.cam.gain_posi--;
	VISCAAPP_GainDirect(stVISCAINFO.cam.gain_posi);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_GainDirect(BYTE posi)
{
	switch(posi)	{
		case  0x01:		SetAEAGCGain(1);	break;		// 0 step
		case  0x02:		SetAEAGCGain(2);	break;		// 2 step
		case  0x03:		SetAEAGCGain(3);	break;		// 4 step
		case  0x04:		SetAEAGCGain(4);	break;		// 6 step
		case  0x05:		SetAEAGCGain(5);	break;		// 8 step
		case  0x06:		SetAEAGCGain(6);	break;		// 10 step
		case  0x07:		SetAEAGCGain(7);	break;		// 12 step
		case  0x08:		SetAEAGCGain(8);	break;		// 14 step
		case  0x09:		SetAEAGCGain(9);	break;		// 16 step
		case  0x0A:		SetAEAGCGain(10);	break;		// 18 step
		case  0x0B:		SetAEAGCGain(11);	break;		// 20 step
		case  0x0C:		SetAEAGCGain(12);	break;		// 22 step
		case  0x0D:		SetAEAGCGain(13);	break;		// 24 step
		case  0x0E:		SetAEAGCGain(14);	break;		// 26 step
		case  0x0F:		SetAEAGCGain(15);	break;		// 28 step
		default : 		SetAEAGCGain(0);		break;		// 0 step
	}
	
	stVISCAINFO.cam.gain_posi = posi;
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_GainLimit(BYTE posi)		// 4(6step, 9.2dB) ~ 0x0F(28step, 43.1dB)
{
	SetAEGainCtrlMode(MIN(3, posi));	// 0:off, 1~3:low~max, 4~0x0f:not supported yet
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetGainLimit(void)		// 4(6step, 9.2dB) ~ 0x0F(28step, 43.1dB)
{
	return		MIN(0x0f, GetAEGainCtrlMode());	// 0:off, 1~3:low~max, 4~0x0f:not supported yet
}

//--------------------------------------------------------------------------------------------------------------------------
void setManualBright(BYTE value)
{
	if(value > 0x1F) {
		// VISCAAPP_BrightReset();
		AECTRL_SetAE_IRISBRValue(17); // F1.6
		VISCAAPP_GainDirect(0); // gain = 0dB
	} else if(value < 18) {
		VISCAAPP_GainDirect(0); // gain = 0dB
		AECTRL_SetAE_IRISBRValue(value);
	} else {
		AECTRL_SetAE_IRISBRValue(17); // F1.6
		VISCAAPP_GainDirect(value - 17);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_BrightReset(void)
{
	stVISCAINFO.cam.bright_posi = 0x11;		// 0(close + 0 step) ~ 0x11(F1.6 + 0 step) ~ 0x1F(F1.6 + 28 step)
	setManualBright(stVISCAINFO.cam.bright_posi);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_BrightUp(void)
{
	if(stVISCAINFO.cam.bright_posi < 0x1F)		stVISCAINFO.cam.bright_posi++;
	setManualBright(stVISCAINFO.cam.bright_posi);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_BrightDown(void)
{
	if(stVISCAINFO.cam.bright_posi > 0x00)		stVISCAINFO.cam.bright_posi--;
	setManualBright(stVISCAINFO.cam.bright_posi);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_BrightDirect(BYTE posi)
{
	stVISCAINFO.cam.bright_posi = posi;
	setManualBright(stVISCAINFO.cam.bright_posi);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExpCompOnOff(BOOL OnOff)
{
	if(OnOff) {
		stVISCAINFO.cam.ctrl |= VISCA_EXPCOMP_ON;
		stVISCAINFO.cam.expcomp_posi = 0x07;
	} else {
		stVISCAINFO.cam.ctrl &= (~VISCA_EXPCOMP_ON);
	}

	AECTRL_SetBrightness(&stAECTRL, stVISCAINFO.cam.expcomp_posi*4 + 2);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExpCompReset(void)
{
	if((stVISCAINFO.cam.ctrl & VISCA_EXPCOMP_ON) == 0) return;

	stVISCAINFO.cam.expcomp_posi = 0x07;		// 0(-10.5dB) ~ 7(0dB) ~ 0x0E(+10.5dB)
	stVISCAINFO.enlarge.expcomp_posi = 0x80;	// 0(-10.5dB) ~ 0x80(0dB) ~ 0xFF(+10.5dB)

	AECTRL_SetBrightness(&stAECTRL, stVISCAINFO.cam.expcomp_posi*4 + 2);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExpCompUp(void)
{
	if((stVISCAINFO.cam.ctrl & VISCA_EXPCOMP_ON) == 0) return;

	if(stVISCAINFO.cam.expcomp_posi < 0x0E)		stVISCAINFO.cam.expcomp_posi++;
	stVISCAINFO.enlarge.expcomp_posi = MIN(0xFF, stVISCAINFO.cam.expcomp_posi*18);

	AECTRL_SetBrightness(&stAECTRL, stVISCAINFO.cam.expcomp_posi*4 + 2);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExpCompDown(void)
{
	if((stVISCAINFO.cam.ctrl & VISCA_EXPCOMP_ON) == 0) return;

	if(stVISCAINFO.cam.expcomp_posi > 0x00)		stVISCAINFO.cam.expcomp_posi--;
	stVISCAINFO.enlarge.expcomp_posi = MIN(0xFF, stVISCAINFO.cam.expcomp_posi*18);

	AECTRL_SetBrightness(&stAECTRL, stVISCAINFO.cam.expcomp_posi*4 + 2);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExpCompDirect(BYTE posi)
{
	if((stVISCAINFO.cam.ctrl & VISCA_EXPCOMP_ON) == 0) return;

	stVISCAINFO.cam.expcomp_posi = posi;
	stVISCAINFO.enlarge.expcomp_posi = MIN(0xFF, stVISCAINFO.cam.expcomp_posi*18);

	AECTRL_SetBrightness(&stAECTRL, posi*4 + 2);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_BacklightOnOff(BOOL OnOff)
{
	if(OnOff)		stVISCAINFO.cam.ctrl |= VISCA_BACKLIGHT_ON;
	else			stVISCAINFO.cam.ctrl &= (~VISCA_BACKLIGHT_ON);
	
	SetBLC_OnOff(OnOff);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_HLCLevel(BYTE level)
{
	// add function here(0:off, 1:low, 2:mid, 3:high)
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_HLCMaskLevel(BYTE level)
{
	WORD  hlc_max_wh, hlc_max_wv;
	
	GetHLC_SizeLimit(&hlc_max_wh, &hlc_max_wv);
	
	if(level)	{
		// api update
		SetHLC_Position(0,0);
		SetHLC_Size(hlc_max_wh, hlc_max_wv);					// set window size to full screen
		SetHLC_WinOnOff(OFF);									// window display off
		SetHLC_MaskLevel(level);								// mask level
		SetHLC_OnOff(ON);										// hlc on
		
		// menu update
		SetMenuStatus(BLIGHT_MENU, 2);				// 0:off, 1:blc, 2:hlc
		SetMenuStatus(HSBLCPOSI_MENU, 0);			SetMenuPosiY(HSBLCPOSI_MENU, 0);
		SetMenuStatus(HSBLCSIZE_MENU, hlc_max_wh);	SetMenuPosiY(HSBLCSIZE_MENU, hlc_max_wv);
		SetMenuStatus(HSBLCDISP_MENU, OFF);
		SetMenuStatus(HSBLCBMLEV_MENU, level);
	}
	else	{
		SetHLC_Default();	SetHLC_OnOff(OFF);					// clear parameter & hlc off
	}
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetHLCLevel(void)
{
	return		0x00;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetHLCMaskLevel(void)
{
	if(GetHLC_OnOff())		return		MIN((235-GetHLC_MaskLevel())/5, 0x0F);	// 0:off, 1~F:mask level
	else					return		0x00;
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_SpotAEOnOff(BOOL OnOff)
{
	if(OnOff)	stVISCAINFO.cam.ctrl |= VISCA_SPOTAE_ON;
	else		stVISCAINFO.cam.ctrl &= (~VISCA_SPOTAE_ON);

	setSpotAEOnOff(OnOff);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_SpotAEPosition(BYTE x, BYTE y)
{
	stVISCAINFO.efunc1.spotae_posi = MAKEBYTE(x,y);		// X/Y:0~0x0F

	setSportAEPostion(x, y);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_AEResponse(BYTE val)
{
	stVISCAINFO.efunc1.ae_response = val;				// 1(1sec) ~ 0x30(600sec)

#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
	AECTRL_setSlowAE(val);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_WDOnOff(BOOL OnOff)
{
	VISCAAPP_SetWDROnOff(OnOff);
	
	stVISCAINFO.efunc2.wd_mode = (OnOff)?  VISCA_WDMODE_ON:  VISCA_WDMODE_OFF;
	
	if(OnOff)	stVISCAINFO.cam.ctrl |= VISCA_WD_NOTOFF;
	else		stVISCAINFO.cam.ctrl &= (~VISCA_WD_NOTOFF);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_WDVEOnOff(BOOL OnOff)
{
	if(OnOff)	stVISCAINFO.efunc2.wd_mode = VISCA_WDMODE_VE_ON;
	if(OnOff)	stVISCAINFO.cam.ctrl |= VISCA_WD_NOTOFF;
	// add function here
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_WDSetParameter(BYTE bright_lev, BYTE comp_sel, BYTE comp_lev)
{
	stVISCAINFO.efunc2.wd_bright_lev = bright_lev;
	stVISCAINFO.efunc2.wd_bright_comp_sel = comp_sel;
	stVISCAINFO.efunc2.wd_comp_lev = comp_lev;
	// add function here
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ApertureReset(void)
{
	stVISCAINFO.cam.aperture_gain = 0x07;
	VISCAAPP_SetEdgeEnhance(stVISCAINFO.cam.aperture_gain*16);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ApertureUp(void)
{
	if(stVISCAINFO.cam.aperture_gain < 0x0F)	stVISCAINFO.cam.aperture_gain++;
	VISCAAPP_SetEdgeEnhance(stVISCAINFO.cam.aperture_gain*16);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ApertureDown(void)
{
	if(stVISCAINFO.cam.aperture_gain > 0x00)	stVISCAINFO.cam.aperture_gain--;
	VISCAAPP_SetEdgeEnhance(stVISCAINFO.cam.aperture_gain*16);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ApertureDirect(BYTE posi)
{
	stVISCAINFO.cam.aperture_gain = posi;
	VISCAAPP_SetEdgeEnhance(stVISCAINFO.cam.aperture_gain*16);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_HROnOff(BOOL OnOff)
{
	if(OnOff)	stVISCAINFO.cam.ctrl |= VISCA_HR_ON;
	else		stVISCAINFO.cam.ctrl &= (~VISCA_HR_ON);
	
	// add function here
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_NRSet(BYTE val)
{
	stVISCAINFO.efunc1.nr_level = val;
	
#if	defined(__M33_V101__)
	SetNR_Mode(MISP_NR_2D3D);
	
	switch (val){
		case 0:	SetNR_3Dintensity(MISP_NR3D_LOW);	break;	// visca off -> nr mode(2d+3d) & level(low)
		case 1:	SetNR_3Dintensity(MISP_NR3D_MID);	break;	// visca low -> nr mode(2d+3d) & level(mid)
		case 2:	SetNR_3Dintensity(MISP_NR3D_HIGH);	break;	// visca mid -> nr mode(2d+3d) & level(high)
		case 3:	SetNR_3Dintensity(MISP_NR3D_AUTO);	break;	// visca high -> nr mode(2d+3d) & level(auto)
		default:	SetNR_3Dintensity(MISP_NR3D_MID);	break;	// nr mode(2d+3d) & level(mid)
	}
#else
	switch (val){
		case 0:	SetNR_Mode(MISP_NR_OFF);										break;	// nr off
		case 1:	SetNR_Mode(MISP_NR_2D3D);	SetNR_3Dintensity(MISP_NR3D_LOW);	break;	// nr mode(2d+3d) & level(low)
		case 2:	SetNR_Mode(MISP_NR_2D3D);	SetNR_3Dintensity(MISP_NR3D_MID);	break;	// nr mode(2d+3d) & level(mid)
		case 3:	SetNR_Mode(MISP_NR_2D3D);	SetNR_3Dintensity(MISP_NR3D_HIGH);	break;	// nr mode(2d+3d) & level(high)
		case 4:	SetNR_Mode(MISP_NR_2D3D);	SetNR_3Dintensity(MISP_NR3D_AUTO);	break;	// nr mode(2d+3d) & level(auto)
		default:	SetNR_Mode((MISP_NR_MODE_t)3);	SetNR_3Dintensity(MISP_NR3D_HIGH);	break;	// nr mode(2d+3d) & level(high)
	}
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_GammaStandard(void)
{
	stVISCAINFO.efunc1.gamma = 0;		// 0:standard, 1:straight
	
	// add function here
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_GammaStraight(void)
{
	stVISCAINFO.efunc1.gamma = 1;		// 0:standard, 1:straight
	
	// add function here
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_HighSensitivityOnOff(BOOL OnOff)
{
	if(OnOff)	stVISCAINFO.efunc1.ctrl3 |= VISCA_HIGHSENS_ON;
	else		stVISCAINFO.efunc1.ctrl3 &= (~VISCA_HIGHSENS_ON);
	
	// add function here
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_LRReverseOnOff(BOOL OnOff)
{
	BYTE mode;
	
	if(OnOff)	stVISCAINFO.other.ctrl2 |= VISCA_LRREVERSE_ON;
	else		stVISCAINFO.other.ctrl2 &= (~VISCA_LRREVERSE_ON);
	
	if(stVISCAINFO.efunc1.ctrl1 & VISCA_FLIP_ON)	{
		if(OnOff)		mode=2;									// 2:V(LR + FLIP)
		else			mode=3;									// 3:H+V(FLIP)
	}
	else	{
		if(OnOff)		mode=1;									// 1:H(LR)
		else			mode=0;									// 0:off
	}
	
	DEMO_SetMirrorMode(mode);									// 0:off, 1:H, 2:V, 3:H+V
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ISPM VISCAAPP_GetLRReverseMode(void)
{
	if(stVISCAINFO.other.ctrl2 & VISCA_LRREVERSE_ON)			return 0x02;	// on
	else														return 0x03;	// off
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_FreezeOnOff(BOOL OnOff)
{
	if(OnOff)	stVISCAINFO.other.ctrl2 |= VISCA_FREEZE_ON;
	else		stVISCAINFO.other.ctrl2 &= (~VISCA_FREEZE_ON);
	
	DEMO_SetFreezeMode(OnOff);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PictureEffectOff(void)
{
	stVISCAINFO.other.pic_effect_mode = VISCA_PICEFFECT_OFF;
	DEMO_SetImageEffect(0);			// 0:off, 1:neg, 2:b/w
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PictureEffectNeg(void)
{
	stVISCAINFO.other.pic_effect_mode = VISCA_PICEFFECT_NEG;
	DEMO_SetImageEffect(1);			// 0:off, 1:neg, 2:b/w
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PictureEffectBW(void)
{
	stVISCAINFO.other.pic_effect_mode = VISCA_PICEFFECT_BW;
	DEMO_SetImageEffect(2);			// 0:off, 1:neg, 2:b/w
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_Defog(BYTE val)
{
	stVISCAINFO.efunc2.defog_mode = HI4BIT(val);
	stVISCAINFO.efunc2.defog_lev = LO4BIT(val);
	
	switch(val)	{
		case  0x00:		SetBY_DefogMode(MISP_DEFOG_OFF);							break;		// off
		case  0x10:		SetBY_DefogMode(MISP_DEFOG_AUTO);							break;		// auto
		case  0x11:		SetBY_DefogMode(MISP_DEFOG_ON);	SetBY_DefogStrength(5);		break;		// on(low)
		case  0x12:		SetBY_DefogMode(MISP_DEFOG_ON);	SetBY_DefogStrength(10);	break;		// on(mid)
		case  0x13:		SetBY_DefogMode(MISP_DEFOG_ON);	SetBY_DefogStrength(16);	break;		// on(high)
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PictureFlipOnOff(BOOL OnOff)
{
	BYTE mode;
	
	if(OnOff)	stVISCAINFO.efunc1.ctrl1 |= VISCA_FLIP_ON;
	else		stVISCAINFO.efunc1.ctrl1 &= (~VISCA_FLIP_ON);
	
	if(stVISCAINFO.other.ctrl2 & VISCA_LRREVERSE_ON)	{
		if(OnOff)		mode=2;									// 2:V(FLIP + LR)
		else			mode=1;									// 1:H(LR)
	}
	else	{
		if(OnOff)		mode=3;									// 3:H+V(FLIP)
		else			mode=0;									// 0:off
	}
	
	DEMO_SetMirrorMode(mode);									// 0:off, 1:H, 2:V, 3:H+V
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ISPM VISCAAPP_GetPictureFlipMode(void)
{
	if(stVISCAINFO.efunc1.ctrl1 & VISCA_FLIP_ON)				return 0x02;	// on
	else														return 0x03;	// off
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ICROnOff(BOOL OnOff)
{
	if(OnOff)		{stVISCAINFO.other.ctrl2 |= VISCA_ICR_ON;			SetDYNT_Mode(2);}	// icr on(night)
	else			{stVISCAINFO.other.ctrl2 &= (~VISCA_ICR_ON);		SetDYNT_Mode(1);}	// icr off(day)
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_AutoICROnOff(BOOL OnOff)
{
	if(OnOff)		{stVISCAINFO.other.ctrl1 |= VISCA_AUTOICR_ON;		SetDYNT_Mode(0);}	// auto icr on
	else			{stVISCAINFO.other.ctrl1 &= (~VISCA_AUTOICR_ON);	SetDYNT_Mode(1);}	// icr off(day)
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_AutoICROnOffThres(BYTE val)		// icr on(night) -> icr off(day)
{
	SetDYNT_AGCN2D(254 - MIN(val, 0x1c)*9);			// 0 ~ 0x1c -> 254 ~ 0
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_AutoICRDelay(BYTE val)			// icr on(night) <-> icr off(day) delay
{
	SetDYNT_AutoDly(MIN(60, val));	// 0 ~ 60 sec
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ICRAlarmReplyOnOff(BOOL OnOff)
{
	if(OnOff)	stVISCAINFO.other.ctrl1 |= VISCA_AUTOICR_ALARM_ON;
	else		stVISCAINFO.other.ctrl1 &= (~VISCA_AUTOICR_ALARM_ON);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ISPM VISCAAPP_GetICRMode(void)
{
	if(GetDYNT_Status())			return 0x02;	// icr on(night)
	else							return 0x03;	// icr off(day)
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ISPM VISCAAPP_GetAutoICRMode(void)
{
	if(GetDYNT_Mode()==0)			return 0x02;	// auto icr on
	else							return 0x03;	// auto icr off
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetAutoICRAlarm(void)			// 0:no change,  2:off->on(night),  3:on->off(day)
{
	BOOL  status = (stVISCAINFO.user.ctrl & VISCA_AUTOICR_STATUS_NIGHT)?  1:  0;			// 0:day, 1:night
	
	if(status != GetDYNT_Status())	{														// day&night mode changed ??
		if(GetDYNT_Status())	stVISCAINFO.user.ctrl |= VISCA_AUTOICR_STATUS_NIGHT;		// night
		else					stVISCAINFO.user.ctrl &= (~VISCA_AUTOICR_STATUS_NIGHT);		// day
		
		return  ((GetDYNT_Status())?  0x02:  0x03);										// 0x02:off->on(night), 0x03:on->off(day)
	}
	else	return  0x00;																	// 0:no change
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetAutoICROnOffThres(void)	// icr on -> off(night -> day)
{
	return		(254 - GetDYNT_AGCN2D()) / 9;		// 254 ~ 0 -> 0 ~ 0x1C
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetExAutoICROnOffThres(void)	// icr on -> off(night -> day) -- extended inquiry
{
	return		(254 - GetDYNT_AGCN2D());	// 254 ~ 0 -> 0 ~ 0xff
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetExAutoICROffOnThres(void)	// icr off -> on(day -> night) -- extended inquiry
{
	return		GetDYNT_AGCD2N() / 9;		// 0 ~ 255 -> 0 ~ 0x1C
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetAutoICRDelay(void)			// icr on(night) <-> icr off(day) delay
{
	return		GetDYNT_AutoDly();			// 0 ~ 60 sec
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_StabilizerOnOff(BOOL OnOff)
{
	if(OnOff)	{stVISCAINFO.other.ctrl2 |= VISCA_STABILIZER_ON;	stVISCAINFO.other.ctrl2 &= (~VISCA_STABILIZER_HOLD_ON);}
	else		{stVISCAINFO.other.ctrl2 &= (~VISCA_STABILIZER_ON);	stVISCAINFO.other.ctrl2 &= (~VISCA_STABILIZER_HOLD_ON);}
	
	if(OnOff)	{if(GetDZOOM_OnOff())	VISCAAPP_DZoomOnOff(OFF);}		// dzoom off when dis on
	
	VISCAAPP_DISOnOff(OnOff);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_StabilizerHold(void)
{
	stVISCAINFO.other.ctrl2 |= VISCA_STABILIZER_HOLD_ON;
	
	// add function here
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MemoryReset(BYTE no)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[32];
	
	memset(&buff[0], 0x00, sizeof(buff));
	EEPROM_MultiWrite(EEPROM_VISCA_MEMORY + (0x20*no), (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MemorySet(BYTE no)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[32];
	
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = HIBYTE(VISCAAPP_GetZoomPosi());				// zoom position
	buff[2] = LOBYTE(VISCAAPP_GetZoomPosi());
	buff[3] = VISCAAPP_GetDZoomMode();						// digital zoom on/off
	buff[4] = VISCAAPP_GetFocusMode();						// focus auto/manual
	buff[5] = HIBYTE(VISCAAPP_GetFocusPosi());				// focus position
	buff[6] = LOBYTE(VISCAAPP_GetFocusPosi());
	buff[7] = VISCAAPP_GetExposure_Mode();					// ae mode
	buff[8] = VISCAAPP_GetShutterPosi();					// shutter control parameters(0:1/1 ~ 0x15:1/10000)
	// buff[9] = stVISCAINFO.cam.bright_posi;				// bright control
	buff[10] = VISCAAPP_GetIrisPosi();						// iris control parameters(0:close ~ 0x11:F1.6)
	buff[11] = VISCAAPP_GetAGCGain();						// gain control parameters(1~15)
	buff[12] = VISCAAPP_GetExpCompOnOff();					// exposure compensation on/off
	buff[13] = VISCAAPP_GetExpCompLevel();					// exposure compensation level
	buff[14] = VISCAAPP_GetBacklightOnOff();				// backlight compensation on/off
	buff[15] = VISCAAPP_GetAutoSlowShutterOnOff();			// auto slow shutter on/off
	buff[16] = VISCAAPP_GetWB_Mode();						// white balance
	buff[17] = VISCAAPP_GetWB_ManGainR();					// wb r-gain(0~0xff)
	buff[18] = VISCAAPP_GetWB_ManGainB();					// wb b-gain(0~0xff)
	buff[19] = VISCAAPP_GetAperture();						// aperture control(0~15)
	buff[20] = (VISCAAPP_GetICRMode()==0x02)?  ON:  OFF;	// icr on/off
	buff[21] = VISCAAPP_GetWDROnOff();						// wd on/off
	// buff[22] = stVISCAINFO.efunc2.wd_bright_lev;			// wd parameter
	// buff[23] = stVISCAINFO.efunc2.wd_bright_comp_sel;
	// buff[24] = stVISCAINFO.efunc2.wd_comp_lev;
	buff[25] = VISCAAPP_GetDefogMode();						// defog on/off
	buff[26] = VISCAAPP_GetDefogLevel();					// defog level
	
	EEPROM_MultiWrite(EEPROM_VISCA_MEMORY + (0x20*no), (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MemoryRecall(BYTE no)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[32];
	
	EEPROM_MultiRead(EEPROM_VISCA_MEMORY + (0x20*no), (PBYTE)&buff[0], sizeof(buff));
	
	
	if(buff[0] != EEPROM_SAVE_DONE)			return;
	
	
	VISCAAPP_FocusAutoOnOff(buff[4]);						// focus auto/manual(0:manual, 1:auto)
	
	if(buff[4])	{											// auto focus
		VISCAAPP_DZoomOnOff(buff[3]);						// digital zoom on/off
		VISCAAPP_ZoomDirect(MAKEWORD(buff[1], buff[2]));	// zoom position
		VISCA_SetRepeatCmd(0x470D);
	}
	else	{												// manual focus
		VISCAAPP_ZoomFocusDirect(MAKEWORD(buff[1], buff[2]), MAKEWORD(buff[5], buff[6]));	// zoom/focus position
		VISCA_SetRepeatCmd(0x470F);
	}
	
	VISCAAPP_AEMode(buff[7]);								// ae mode
	VISCAAPP_ShutterDirect(buff[8]);						// shutter control parameters(0:1/1 ~ 0x15:1/10000)
	//VISCAAPP_BrightDirect(buff[9]);							// bright control
	VISCAAPP_IrisDirect(buff[10]);							// iris control parameters(0:close ~ 0x11:F1.6)
	VISCAAPP_GainDirect(buff[11]);							// gain control parameters(1~15)
	VISCAAPP_ExpCompOnOff(buff[12]);						// exposure compensation on/off
	VISCAAPP_ExpCompDirect(buff[13]);						// exposure compensation level
	VISCAAPP_BacklightOnOff(buff[14]);						// backlight compensation on/off
	VISCAAPP_AutoSlowShutterOnOff(buff[15]);				// auto slow shutter on/off
	VISCAAPP_WBMode(buff[16]);								// white balance
	VISCAAPP_WBRGainDirect(buff[17]);						// wb r-gain(0~0xff)
	VISCAAPP_WBBGainDirect(buff[18]);						// wb b-gain(0~0xff)
	VISCAAPP_ApertureDirect(buff[19]);						// aperture control(0~15)
	VISCAAPP_ICROnOff(buff[20]);							// icr on/off
	VISCAAPP_WDOnOff(buff[21]);								// wd on/off
	// buff[22] = stVISCAINFO.efunc2.wd_bright_lev;			// wd parameter
	// buff[23] = stVISCAINFO.efunc2.wd_bright_comp_sel;
	// buff[24] = stVISCAINFO.efunc2.wd_comp_lev;
	VISCAAPP_Defog(MAKEBYTE(buff[25], buff[26]));			// [7:4]defog on/off, [3:0]defog level
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_CustomReset(void)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[128];
	
	memset(&buff[0], 0x00, sizeof(buff));
	EEPROM_MultiWrite(EEPROM_VISCA_CUSTOM, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_CustomSet(void)
{
	// add function here
	// UARTprintf("[VISCA] Custom Set not defined..\r\n");
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_CustomRecall(void)
{
	// add function here
	// UARTprintf("[VISCA] Custom Recall not defined..\r\n");
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MemSave(BYTE addr, WORD data)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[3];
	WORD raddr = EEPROM_VISCA_MEMSAVE + (addr *3);
#endif
	
	stVISCAINFO.user.memsave[MIN(addr, 7)] = data;
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = HIBYTE(data);			buff[2] = LOBYTE(data);
	EEPROM_MultiWrite(raddr, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_DisplayOnOff(BOOL OnOff)	// data screen on/off
{
	BYTE i, color=MAKEBYTE(OSD_GRAY, OSD_NOCOLOR);
	
	if(OnOff)	stVISCAINFO.other.ctrl3 |= VISCA_DISP_ON;
	else		stVISCAINFO.other.ctrl3 &= (~VISCA_DISP_ON);
	
	if(OnOff)	{stVISCAINFO.user.dscr_inter_cnt = 0;	stVISCAINFO.user.dscr_disptime_cnt=0;}	// clear display interval count
	if(OnOff)	for(i=0;i<19;i++)	OSD_StrDispAttb(i, 0, OFF, color, "%36s", "");				// display on(clear display area)
	
	if(OnOff)	OSD_StrDispOnOff(ON);
	else	{
		if((stVISCAINFO.other.ctrl3 & VISCA_TILEDISP_ON) != VISCA_TILEDISP_ON)		OSD_StrDispOnOff(OFF);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_DisplayToggle(void)
{
	if(stVISCAINFO.other.ctrl3 & VISCA_DISP_ON)		stVISCAINFO.other.ctrl3 &= (~VISCA_DISP_ON);	// toggle on/off
	else											stVISCAINFO.other.ctrl3 |= VISCA_DISP_ON;
	
	if(stVISCAINFO.other.ctrl3 & VISCA_DISP_ON)		VISCAAPP_DisplayOnOff(ON);
	else											VISCAAPP_DisplayOnOff(OFF);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MultiLineTitleSet1(BYTE line_no, BYTE h_posi, BYTE color, BYTE blink)
{
	stVISCAINFO.camtitle.h_posi[line_no] = h_posi;
	stVISCAINFO.camtitle.color[line_no] = color;
	
	if(blink)	stVISCAINFO.camtitle.blink |= (1<<line_no);
	else		stVISCAINFO.camtitle.blink &= ~(1<<line_no);
}

//--------------------------------------------------------------------------------------------------------------------------
void GetTitle2Ascii(PBYTE pCH)	// test func.
{
	BYTE i, ch;
	
	for(i=0;i<10;i++)	{
		ch = *(pCH+i);
		
		if(ch <= 25)							ch += 'A';		// 'A' ~ 'Z'
		else if((ch >= 30) && (ch <= 38))		ch += 19;		// '1' ~ '9'
		else	{
			switch(ch)	{
				case  26:	ch = '&';	break;		case  27:	ch = ' ';	break;
				case  28:	ch = '?';	break;		case  29:	ch = '!';	break;
				case  39:	ch = '0';	break;		case  73:	ch = '"';	break;
				case  74:	ch = ':';	break;		case  75:	ch = 0x27;	break;	// '
				case  76:	ch = '.';	break;		case  77:	ch = ',';	break;
				case  78:	ch = '/';	break;		case  79:	ch = '-';	break;
				default:	ch = ' ';
			}
		}
		
		*(pCH+i) = ch;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MultiLineTitleSet2(BYTE line_no, PBYTE pTitle)
{
	GetTitle2Ascii(pTitle);
	memcpy(&stVISCAINFO.camtitle.title[line_no][0], pTitle, 10);
	
	if(stVISCAINFO.camtitle.title[line_no][10]==0x00)	{
		memset(&stVISCAINFO.camtitle.title[line_no][10], 0x20, 10);	// fill blank text
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MultiLineTitleSet3(BYTE line_no, PBYTE pTitle)
{
	GetTitle2Ascii(pTitle);
	memcpy(&stVISCAINFO.camtitle.title[line_no][10], pTitle, 10);
	
	if(stVISCAINFO.camtitle.title[line_no][0] == 0x00)	{
		memset(&stVISCAINFO.camtitle.title[line_no][0], 0x20, 10);	// fill blank text
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MultiLineTitleClear(BYTE line_no)
{
	memset(&stVISCAINFO.camtitle, 0, sizeof(stVISCAINFO.camtitle));
	
	stVISCAINFO.camtitle.display = 0x0000;								// title display off
	if((stVISCAINFO.other.ctrl3 & VISCA_DISP_ON) != VISCA_DISP_ON)		OSD_StrDispOnOff(OFF);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MultiLineTitleOn(BYTE line_no)
{
	if(line_no == 0x0F)		stVISCAINFO.camtitle.display = 0x07FF;			// all lines
	else					stVISCAINFO.camtitle.display |= (1<<line_no);	// one line on/off
	
	if(stVISCAINFO.camtitle.display)	{
		stVISCAINFO.other.ctrl3 |= VISCA_TILEDISP_ON;
		OSD_StrDispOnOff(ON);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MultiLineTitleOff(BYTE line_no)
{
	if(line_no == 0x0F)		stVISCAINFO.camtitle.display = 0x0000;				// all lines
	else					stVISCAINFO.camtitle.display &= (~(1<<line_no));	// one line on/off
	
	if(stVISCAINFO.camtitle.display==0)	{
		stVISCAINFO.other.ctrl3 &= (~VISCA_TILEDISP_ON);
		
		if((stVISCAINFO.other.ctrl3 & VISCA_DISP_ON) != VISCA_DISP_ON)		OSD_StrDispOnOff(OFF);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MuteOnOff(BOOL OnOff)
{
	if(OnOff)	stVISCAINFO.other.ctrl3 |= VISCA_MUTE_ON;
	else		stVISCAINFO.other.ctrl3 &= (~VISCA_MUTE_ON);
	
	SetOUT_MainTestPattern((stVISCAINFO.other.ctrl3 & VISCA_MUTE_ON)?  13:  0);			// blue screen
	SetOUT_AuxTestPattern((stVISCAINFO.other.ctrl3 & VISCA_MUTE_ON)?  13:  0);			// blue screen
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MuteToggle(void)
{
	if(stVISCAINFO.other.ctrl3 & VISCA_MUTE_ON)		stVISCAINFO.other.ctrl3 &= (~VISCA_MUTE_ON);
	else											stVISCAINFO.other.ctrl3 |= VISCA_MUTE_ON;
	
	SetOUT_MainTestPattern((stVISCAINFO.other.ctrl3 & VISCA_MUTE_ON)?  13:  0);			// blue screen
	SetOUT_AuxTestPattern((stVISCAINFO.other.ctrl3 & VISCA_MUTE_ON)?  13:  0);			// blue screen
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PMSetMask(BYTE mask_no, BYTE mode, BYTE w_size, BYTE h_size)
{
	stVISCAINFO.pm.w[mask_no] = w_size;				stVISCAINFO.pm.h[mask_no] = h_size;
	
	if(mode)	SetPM_RectSizeNew(mask_no, w_size*22, h_size*22);		// new
	else		SetPM_RectSizeModify(mask_no, w_size*22, h_size*22);	// modify
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PMDisplay(DWORD mask_sel)
{
	BYTE i;
	
	stVISCAINFO.pm.mask_disp = mask_sel;
	
	SetPM_OnOff(ON);
	for(i=0;i<32;i++)	{												// ch:0~5(A~F), 8~13(G~L),,
		if((i%8==6) || (i%8==7))	continue;							// skip bit-6,7,14,15,22,23,30,31
		
		if(mask_sel & (1<<i))		SetPM_RectOnOff(i-(i/8*2), ON); 	// mask display on
		else						SetPM_RectOnOff(i-(i/8*2), OFF); 	// mask display off
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PMSetMaskColor(DWORD mask_sel, BYTE color1, BYTE color2)
{
	BYTE i, color;
	
	for(i=0;i<32;i++)	{
		if((i%8==6) || (i%8==7))	continue;									// skip bit-6,7,14,15,22,23,30,31
		
		if(mask_sel & (1<<i))		stVISCAINFO.pm.color[i-(i/8*2)] = color2;	// mask sel(1) : use secondary color
		else						stVISCAINFO.pm.color[i-(i/8*2)] = color1;	// mask sel(0) : use primary color
	}
	
	for(i=0;i<24;i++)	{
		switch(stVISCAINFO.pm.color[i] & 0x0F)	{								// 'half tone' not supported..
			case  0x00:		color=0;	break;			case  0x01:		color=8;	break;	// 0(black), 1(gray1)
			case  0x02:		color=9;	break;			case  0x03:		color=10;	break;	// 2(gray2), 3(gray3)
			case  0x04:		color=11;	break;			case  0x05:		color=12;	break;	// 4(gray4), 5(gray5)
			case  0x06:		color=13;	break;			case  0x07:		color=1;	break;	// 6(gray6), 7(white)
			case  0x08:		color=4;	break;			case  0x09:		color=2;	break;	// 8(red), 9(green)
			case  0x0A:		color=3;	break;			case  0x0B:		color=5;	break;	// 10(blue), 11(cyan)
			case  0x0C:		color=7;	break;			case  0x0D:		color=6;	break;	// 12(yellow), 13(magenta)
			case  0x0F:		color=14;	break;			default:		color=0;	break;	// 14(mosaic).
		}
		SetPM_RectColor(i, color);
	}
	
//	UARTprintf("[VISCA] PM Mask Color, mask[0x%x]->color2(%d%s), other mask->color1(%d%s)..\r\n", 
//				mask_sel, color2&0x0f, (color2&0xf0)? "-Halftone": "", color1&0x0f, (color1&0xf0)? "-Halftone": "" );
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PMSetPanTiltAngle(WORD pan, WORD tilt)						// camera direction
{
	stVISCAINFO.pm.pan = pan;				stVISCAINFO.pm.tilt = tilt;
	
	SetPM_CamPanTIlt(pan, tilt);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PMSetPTZMask(BYTE mask_no, WORD pan, WORD tilt, WORD zoom)	// mask direction
{
	stVISCAINFO.pm.maskpan[mask_no] = pan;		stVISCAINFO.pm.masktilt[mask_no] = tilt;
	stVISCAINFO.pm.maskzoom[mask_no] = zoom;
	
	SetPM_MaskPTZ(mask_no, pan, tilt, zoom);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PMNonInterlockMask(BYTE mask_no, BYTE x, BYTE y, BYTE w, BYTE h)	// direct position
{
	stVISCAINFO.pm.w[mask_no] = w;				stVISCAINFO.pm.h[mask_no] = h;
	stVISCAINFO.pm.x[mask_no] = x;				stVISCAINFO.pm.y[mask_no] = y;
	
	SetPM_NonInterlockMask(mask_no, w, h, x, y);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PMCenterLineOnOff(BOOL OnOff)
{
	// add function here
	UARTprintf("[VISCA] PM CenterLine On/Off (%s) not defined..\r\n", (OnOff)? "ON": "OFF");
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_CamIDWrite(WORD cam_id)
{
	stVISCAINFO.other.cam_id = cam_id;
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MDOnOff(BOOL OnOff)
{
	if(OnOff)	stVISCAINFO.efunc1.ctrl1 |= VISCA_MD_ON;
	else		stVISCAINFO.efunc1.ctrl1 &= (~VISCA_MD_ON);
	
	SetMD_OnOff(OnOff);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MDFunctionSet(BYTE disp_mode, BYTE det_frame, BYTE thres, BYTE inter_time)
{
	BYTE i, onoff[4];
	
	stVISCAINFO.md.disp_mode = disp_mode;				stVISCAINFO.md.det_frame = det_frame;
	stVISCAINFO.md.thres = thres;						stVISCAINFO.md.inter_time = inter_time;
	
	for(i=0;i<4;i++)	{
		if(((det_frame>>i)&0x01) && (disp_mode&0x01))	onoff[i]=ON;	// check 'frame_no', 'disp'
		else											onoff[i]=OFF;
	}
	
	SetMD_AreaDisp(0, onoff[0]);				SetMD_AreaDisp(1, onoff[1]);
	SetMD_AreaDisp(2, onoff[2]);				SetMD_AreaDisp(3, onoff[3]);
	
	SetMD_MotionView((disp_mode&0x02)?  ON:  OFF);						// [1]block(motion view)
	SetMD_Threshold(MIN(40, thres/6));									// 0~ff -> 0~40
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MDWindowSet(BYTE no, BYTE lx, BYTE ly, BYTE rx, BYTE ry)
{
	BYTE step_h, step_v, w, h;
	
	stVISCAINFO.md.lx[no]=lx;		stVISCAINFO.md.rx[no]=rx;
	stVISCAINFO.md.ly[no]=ly;		stVISCAINFO.md.ry[no]=ry;
	
	step_h = GetMD_SizeMaxH() * 10 / 16;		// visca md grid(16x8)
	step_v = GetMD_SizeMaxV() * 10 / 8;
	
	w = (rx - lx) * step_h / 10;
	h = (ry - ly) * step_v / 10;
	lx = lx * step_h / 10;
	ly = ly * step_v / 10;
	
	SetMD_AreaSizePosi(no, w, h, lx, ly);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetMDAlarm(void)
{
	// add function here
	return 0x00;		// 0x00:no detection, 0x8?:detection(0x80~3:frame0~3)
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_GetMDWindow(BYTE no, PBYTE plx, PBYTE ply, PBYTE prx, PBYTE pry)
{
	BYTE step_h, step_v, w, h, lx, ly;
	
	step_h = GetMD_SizeMaxH() * 10 / 16;		// visca md grid(16x8)
	step_v = GetMD_SizeMaxV() * 10 / 8;
	
	lx = GetMD_AreaPosiX(no);
	ly = GetMD_AreaPosiY(no);
	w = GetMD_AreaWidth(no);
	h = GetMD_AreaHeight(no);
	
	*plx = lx * 10 / step_h;			*plx += ((lx * 100 / step_h %10) >= 5)?  1:  0;
	*ply = ly * 10 / step_v;			*ply += ((ly * 100 / step_v %10) >= 5)?  1:  0;
	*prx = (lx + w) * 10 / step_h;		*prx += (((lx + w) * 100 / step_h %10) >= 5)?  1:  0;
	*pry = (ly + h) * 10 / step_v;		*pry += (((ly + h) * 100 / step_v %10) >= 5)?  1:  0;
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ContZoomPosReplyOnOff(BOOL OnOff)
{
	// add function here
	// UARTprintf("[VISCA] Continuous Zoom Position Reply On/Off (%s) not defined..\r\n", (OnOff)? "ON": "OFF");
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ZoomPosReplyInterTimeSet(BYTE inter_time)
{
	// add function here
	// UARTprintf("[VISCA] Zoom Position Reply Inter time set, time/v-cycle(%d) not defined..\r\n", inter_time);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ContFocusPosReplyOnOff(BOOL OnOff)
{
	// add function here
	// UARTprintf("[VISCA] Continuous Focus Position Reply On/Off (%s) not defined..\r\n", (OnOff)? "ON": "OFF");
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_FocusPosReplyInterTimeSet(BYTE inter_time)
{
	// add function here
	// UARTprintf("[VISCA] Focus Position Reply Inter time set, time/v-cycle(%d) not defined..\r\n", inter_time);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_RegViscaBaudrate(BYTE val)
{
	int baud = 9600;

	if		(val==0) baud =   9600;
	else if (val==1) baud =  19200;
	else if (val==2) baud =  38400;
	else			 baud = 115200;

#if defined(__USE_UART0_VISCA__)
	SetUARTBaudRate(0, baud);
#else
	SetUARTBaudRate(1, baud);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_RegMonitoringMode(BYTE val)
{
	WORD	frmt;
	
	switch(val)	{
		case  0x02:		frmt = MISP_VIDOUT_1920x1080i60;	break;	// 1080i60
		case  0x03:		frmt = MISP_VIDOUT_720x480i60;		break;	// ntsc(stop digital out)
		case  0x04:		frmt = MISP_VIDOUT_1920x1080i50;	break;	// 1080i50
		case  0x05:		frmt = MISP_VIDOUT_720x576i50;		break;	// pal(stop digital out)
		case  0x07:		frmt = MISP_VIDOUT_1920x1080p30;	break;	// 108030
		case  0x08:		frmt = MISP_VIDOUT_1920x1080p25;	break;	// 1080p25
		case  0x0A:		frmt = MISP_VIDOUT_1280x720p60;		break;	// 720p60
//		case  0x0B:		frmt = ---;							break;	// reserved
		case  0x0C:		frmt = MISP_VIDOUT_1280x720p50;		break;	// 720p50
//		case  0x0D:		frmt = ---;							break;	// reserved
//		case  0x0E:		frmt = MISP_VIDOUT_1280x720p29;		break;	// 720p29 -- not supported
//		case  0x0F:		frmt = MISP_VIDOUT_1280x720p30;		break;	// 720p30 -- not supported
//		case  0x10:		frmt = ---;							break;	// reserved
//		case  0x11:		frmt = MISP_VIDOUT_1280x720p25;		break;	// 720p25 -- not supported
//		case  0x12:		frmt = ---;							break;	// reserved
		case  0x14:		frmt = MISP_VIDOUT_1920x1080p50;	break;	// 1080p50
		case  0x15:		frmt = MISP_VIDOUT_1920x1080p60;	break;	// 1080p60

		case  0x16:		frmt = MISP_VIDOUT_2048x1536p30;	break;	// 1536p29
//		case  0x17:		frmt = MISP_VIDOUT_2048x1536p30;	break;	// 1536p30
		case  0x18:		frmt = MISP_VIDOUT_2048x1536p25;	break;	// 1536p25
#if defined(__USE_59Hz_OUTPUT__)
		case  0x01:		frmt = MISP_VIDOUT_1920x1080i59;	break;	// 1080i59
		case  0x06:		frmt = MISP_VIDOUT_1920x1080p29;	break;	// 1080p29
		case  0x09:		frmt = MISP_VIDOUT_1280x720p59;		break;	// 720p59
		case  0x13:		frmt = MISP_VIDOUT_1920x1080p59;	break;	// 1080p59
#endif
		default:		frmt = MISP_VIDOUT_1920x1080p60;		break;	// 1080p60
	}
	
	DEMO_SetOutputFormat(frmt);

	stVISCAINFO.user.dscr_disptime_cnt=0;								// clear dscr display time counter
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_RegOutputEnable(BYTE val)
{
	// add function here
	// UARTprintf("[VISCA] Register Output Enable, value[0x%02x] not defined..\r\n", val);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_RegLVDSMode(BYTE val)
{
	// add function here
	// UARTprintf("[VISCA] Register LVDS Mode, value[0x%02x] not defined..\r\n", val);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_RegZoomWideLimit(BYTE val)
{
	// add function here
	// UARTprintf("[VISCA] Register Zoom Wide Limit, value[0x%02x] not defined..\r\n", val);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_RegZoomTeleLimit(BYTE val)
{
	// add function here
	// UARTprintf("[VISCA] Register Zoom Tele Limit, value[0x%02x] not defined..\r\n", val);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_RegDZoomMax(BYTE val)
{
	// add function here
	// UARTprintf("[VISCA] Register DZoom Max, value[0x%02x] not defined..\r\n", val);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_RegStableZoom(BYTE val)
{
	// add function here
	// UARTprintf("[VISCA] Register Stable Zoom, value[%s] not defined..\r\n", (val)? "ON": "OFF");
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_RegFocusTraceZD(BYTE val)
{
	// add function here
	// UARTprintf("[VISCA] Register Focus Trace @ZoomDirect, value[%s] not defined..\r\n", (val)? "ON": "OFF");
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_RegFocusOffsetDC(BYTE val)
{
	// add function here
	// UARTprintf("[VISCA] Register Focus Offset @DomeCover, value[0x%02x] not defined..\r\n", val);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_RegEnlargementMode(BYTE val)
{
	stVISCAINFO.reg.ex_mode = val;
	// add function here
	// move to eeprom ???
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_RegValue(BYTE addr, BYTE val)
{
	// add function here
	// UARTprintf("[VISCA] Register Value, addr[0x%02x], value[0x%02x] not defined..\r\n", addr, val);
	// VISCAAPP_MaintenanceMode(val);	// bh 2015.07.09 for f/w up test
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetRegViscaBaudrate(void)
{
#if defined(__USE_UART0_VISCA__)
	DWORD  baud = GetUARTBaudRate(0);
#else
	DWORD  baud = GetUARTBaudRate(1);
#endif
	
	BYTE val=0;
	
	if(baud > 9389 && baud < 9811)				val = VISCA_REG_BAUDRATE_9600;	// 9600 : 9389 ~ 9811
	else if(baud > 18778 && baud < 19622)		val = VISCA_REG_BAUDRATE_19200;	// 19200 : 18778 ~ 19622
	else if(baud > 37555 && baud < 39245)		val = VISCA_REG_BAUDRATE_38400;	// 38400 : 37555 ~ 39245
	else if(baud > 112666 && baud < 117734)	val = VISCA_REG_BAUDRATE_115200;// 115200 : 112666 ~ 117734
	
	return 	val;
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ColorEnhanceOnOff(BOOL OnOff)
{
	stVISCAINFO.color_enhance.onoff = OnOff;
	// add function here
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ColorEnhanceParam(BYTE thres, BYTE y_h, BYTE cr_h, BYTE cb_h, BYTE y_l, BYTE cr_l, BYTE cb_l)
{
	stVISCAINFO.color_enhance.thres = thres;
	stVISCAINFO.color_enhance.y_high = y_h;			stVISCAINFO.color_enhance.y_low = y_l;
	stVISCAINFO.color_enhance.cr_high = cr_h;		stVISCAINFO.color_enhance.cr_low = cr_l;
	stVISCAINFO.color_enhance.cb_high = cb_h;		stVISCAINFO.color_enhance.cb_low = cb_l;
	// add function here
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ChromaSuppress(BYTE level)
{
	stVISCAINFO.efunc1.chroma_suppress = level;		// 0(off), 1~3(level)
	// add function here
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ColorGain(BYTE val)
{
	stVISCAINFO.efunc1.color_gain = val;			// 0(60%) ~ 4(100%) ~ 0x0E(200%)
	stVISCAINFO.enlarge.color_gain = (val+6)*12+8;	// 0(0%) ~ 0x80(100%) ~ 0xFF(200%)

	// SetOUT_MainSaturation((val+6)*12+8);
	// SetOUT_AuxSaturation((val+6)*12+8);
	SetApp_AutoSat_OutSaturation((val+6)*12+8,4); // Added for autosaturation integrated control
	SetMenuStatus(SATURATION_MENU, val+6);		
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ColorHue(BYTE val)
{
	stVISCAINFO.efunc3.color_hue = val;				// 0(-14) ~ 7(0) ~ 0x0E(+14)
	stVISCAINFO.enlarge.color_hue = val*18+2;		// 0(-14) ~ 0x80(0) ~ 0xFF(+14)
	SetOUT_MainHue(val*18+2);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_GammaOffset(BYTE pol, BYTE val)
{
	// add function here
	// UARTprintf("[VISCA] Gamma Offset, polarity(%s), offset(0x%02x) not defined..\r\n", (pol)? "minus": "plus", val);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExExpCompReset(void)
{
	stVISCAINFO.enlarge.expcomp_posi = 0x80;	// 0(-10.5dB) ~ 0x80(0dB) ~ 0xFF(+10.5dB)
	stVISCAINFO.cam.expcomp_posi = 0x07;		// 0(-10.5dB) ~ 7(0dB) ~ 0x0E(+10.5dB)
	// add function here
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExExpCompUp(BYTE step)
{
	if(step==0)		step=1;
	if(stVISCAINFO.enlarge.expcomp_posi < (0xFF - step))		stVISCAINFO.enlarge.expcomp_posi += step;	// step :  0 ~ 0x7F
	stVISCAINFO.cam.expcomp_posi = MIN(0x0E, stVISCAINFO.enlarge.expcomp_posi/18);
	// add function here
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExExpCompDown(BYTE step)
{
	if(step==0)		step=1;
	if(stVISCAINFO.enlarge.expcomp_posi > step)		stVISCAINFO.enlarge.expcomp_posi -= step;
	stVISCAINFO.cam.expcomp_posi = MIN(0x0E, stVISCAINFO.enlarge.expcomp_posi/18);
	// add function here
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExExpCompDirect(BYTE level)
{
	stVISCAINFO.enlarge.expcomp_posi = level;
	stVISCAINFO.cam.expcomp_posi = MIN(0x0E, stVISCAINFO.enlarge.expcomp_posi/18);
	// add function here
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExApertureReset(void)
{
	stVISCAINFO.enlarge.aperture_gain = 0x80;		// 0 ~ 0xFF
	stVISCAINFO.cam.aperture_gain = 0x07;			// 0 ~ 0x0F
	
	VISCAAPP_SetEdgeEnhance(stVISCAINFO.enlarge.aperture_gain);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExApertureUp(BYTE step)
{
	if(step==0)		step=1;
	if(stVISCAINFO.enlarge.aperture_gain < (0xFF - step))		stVISCAINFO.enlarge.aperture_gain += step;
	stVISCAINFO.cam.aperture_gain = MIN(0x0F, stVISCAINFO.enlarge.aperture_gain/16);
	
	VISCAAPP_SetEdgeEnhance(stVISCAINFO.enlarge.aperture_gain);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExApertureDown(BYTE step)
{
	if(step==0)		step=1;
	if(stVISCAINFO.enlarge.aperture_gain > step)		stVISCAINFO.enlarge.aperture_gain -= step;
	stVISCAINFO.cam.aperture_gain = MIN(0x0F, stVISCAINFO.enlarge.aperture_gain/16);
	
	VISCAAPP_SetEdgeEnhance(stVISCAINFO.enlarge.aperture_gain);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExApertureDirect(BYTE level)
{
	stVISCAINFO.enlarge.aperture_gain = level;
	stVISCAINFO.cam.aperture_gain = MIN(0x0F, stVISCAINFO.enlarge.aperture_gain/16);
	
	VISCAAPP_SetEdgeEnhance(stVISCAINFO.enlarge.aperture_gain);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExAutoICROnOffThres(BYTE val)			// icr on(night) -> icr off(day)
{
	SetDYNT_AGCN2D(254 - MIN(val, 254));			// 0 ~ 0xff -> 254 ~ 0
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExAutoICROffOnThres(BYTE val)			// icr off(day) -> icr on(night)
{
	SetDYNT_AGCD2N(MIN(MAX(val*9, 1), 255));	// 0 ~ 0x1c -> 1 ~ 255
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExColorGain(BYTE color, BYTE val)
{
	if(color==0)	{
		stVISCAINFO.enlarge.color_gain = val;										// 0(0%) ~ 0x80(100%) ~ 0xFF(200%)
		stVISCAINFO.efunc1.color_gain = (val/12 >= 6)?  MIN(0x0E, val/12-6):  0;	// 0(60%) ~ 4(100%) ~ 0x0E(200%)
		// SetOUT_MainSaturation(val);
		// SetOUT_AuxSaturation(val);
		
		SetApp_AutoSat_OutSaturation(val,4); // Added for autosaturation integrated control
	}
	else	{
		// add function here
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_ExColorHue(BYTE color, BYTE val)
{
	if(color==0)	{
		stVISCAINFO.enlarge.color_hue = val;										// 0(-14) ~ 0x80(0) ~ 0xFF(+14)
		stVISCAINFO.efunc3.color_hue = val/18;										// 0(-14) ~ 7(0) ~ 0x0E(+14)
		SetOUT_MainHue(val);
	}
	else	{
		// add function here
	}
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetWB_ManGainR(void)
{
	WORD  gain = stWBCTRL.stMANUAL.R;
	
	if(gain < 106)			return 0;
	else					return ((gain - 106) * 85/100);	// 106~406 -> 0~ff
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetWB_ManGainB(void)
{
	WORD  gain = stWBCTRL.stMANUAL.B;
	
	if(gain < 106)			return 0;
	else					return ((gain - 106) * 85/100);	// 106~406 -> 0~ff
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetWB_Mode(void)
{
	LONG ctrl = stWBCTRL.ctrl;
	
	switch(ctrl & WBCTRL_MODE_MASK)	{
		case  WBCTRL_WBAUTO_MODE:		return VISCA_WBMODE_AUTO;
		case  WBCTRL_WBHOLD_MODE:		return VISCA_WBMODE_ONEPUSH;
		case  WBCTRL_INDOOR_MODE:		return VISCA_WBMODE_INDOOR;
		case  WBCTRL_OTDOOR_MODE:		return VISCA_WBMODE_OUTDOOR;
		case  WBCTRL_MANUAL_MODE:		return VISCA_WBMODE_MANUAL;
		default:						return VISCA_WBMODE_AUTO;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetExposure_Mode(void)
{
	switch(stAECTRL.ctrl & AECTRL_PRIORITY_MASK)	{
		case  AECTRL_PRIORITY_AUTO:			return VISCA_AEMODE_AUTO;		// auto
		case  AECTRL_PRIORITY_SHUTTER:		return VISCA_AEMODE_SHUTTERFIX;		// shut fix
		case  AECTRL_PRIORITY_APERTURE:		return VISCA_AEMODE_IRISFIX;		// iris fix
//		case  AECTRL_PRIORITY_GAIN:			return 0x0x;	break;			// gain fix
		case  AECTRL_ALL_MANUAL:			return VISCA_AEMODE_MANUAL;		// manual
		default:							return VISCA_AEMODE_AUTO;		// auto
	}
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetShutterPosi(void)
{
	switch(stAECTRL.ctrl & AECTRL_SHUT_MASK)	{
		case AECTRL_SHUTCTL_1_25:
		case AECTRL_SHUTCTL_1_30:		return 0x05;	break;
		case AECTRL_SHUTCTL_1_50:
		case AECTRL_SHUTCTL_1_60:		return 0x06;	break;
		case AECTRL_SHUTCTL_1_100:
		case AECTRL_SHUTCTL_1_120:		return 0x08;	break;
		case AECTRL_SHUTCTL_1_150:
		case AECTRL_SHUTCTL_1_180:		return 0x0A;	break;
		case AECTRL_SHUTCTL_1_200:
		case AECTRL_SHUTCTL_1_240:
		case AECTRL_SHUTCTL_1_250:
		case AECTRL_SHUTCTL_1_300:		return 0x0B;	break;
		case AECTRL_SHUTCTL_1_500:		return 0x0D;	break;
		case AECTRL_SHUTCTL_1_1000:		return 0x0F;	break;
		case AECTRL_SHUTCTL_1_2000:		return 0x11;	break;
		case AECTRL_SHUTCTL_1_5000:		return 0x13;	break;
		case AECTRL_SHUTCTL_1_10000:	return 0x15;	break;
		default:						return 0x05;	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetAGCGain(void)
{
	return MAX(0x01, MIN(0x0F, GetAEAGCGain()));
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetAutoSlowShutterOnOff(void)
{
	return (AECTRL_GetSensUpMax() > AECTRL_GetSensUpMin())?  ON:  OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetExpCompOnOff(void)
{
	return (stVISCAINFO.cam.ctrl & VISCA_EXPCOMP_ON)?  ON:  OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetExpCompLevel(void)			// 0(-10.5dB) ~ 7(0dB) ~ 0x0E(+10.5dB)
{
	if(Get_AE_BrightLevel() >= 2)		return (Get_AE_BrightLevel() - 2) / 4;
	else								return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetBacklightOnOff(void)
{
	return GetBLC_OnOff();
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetAperture(void)
{
	return  MIN(255, GetIN_MainHPFLevel()*2) / 16;		// 0 ~ 127 -> 0 ~ 15
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetExAperture(void)
{
	return  MIN(255, GetIN_MainHPFLevel()*2);			// 0 ~ 127 -> 0 ~ 255
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetWDROnOff(void)
{
	return  GetWDR_ProcesMode()?  ON:  OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetDefogMode(void)
{
	return  GetBY_DefogMode()?  ON:  OFF;				// auto/on -> on
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetDefogLevel(void)
{
	if(GetBY_DefogMode() != MISP_DEFOG_ON)		return 0;
	else if(GetBY_DefogStrength() < 6)			return 1;	// low
	else if(GetBY_DefogStrength() < 11)			return 2;	// mid
	else										return 3;	// high
}

//--------------------------------------------------------------------------------------------------------------------------
WORD VISCAAPP_GetFWVer(void)
{
	return		HIWORD(GetMenu_FirmwareVer());
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE VISCAAPP_GetVOUT_Frmt(void)
{
	BYTE frmt = GetOUT_MainFrmt();
	
	switch(frmt)	{
		case  MISP_VIDOUT_1920x1080i60:		return 0x02;	// 1080i60
		case  MISP_VIDOUT_720x480i60:		return 0x03;	// ntsc(stop digital out)
		case  MISP_VIDOUT_1920x1080i50:		return 0x04;	// 1080i50
		case  MISP_VIDOUT_720x576i50:		return 0x05;	// pal(stop digital out)
		case  MISP_VIDOUT_1920x1080p30:		return 0x07;	// 108030
		case  MISP_VIDOUT_1920x1080p25:		return 0x08;	// 1080p25
		case  MISP_VIDOUT_1280x720p60:		return 0x0A;	// 720p60
		case  MISP_VIDOUT_1280x720p50:		return 0x0C;	// 720p50
//		case  MISP_VIDOUT_1280x720p29:		return 0x0E;	// 720p29 -- not supported
//		case  MISP_VIDOUT_1280x720p30:		return 0x0F;	// 720p30 -- not supported
//		case  MISP_VIDOUT_1280x720p25:		return 0x11;	// 720p25 -- not supported
		case  MISP_VIDOUT_1920x1080p50:		return 0x14;	// 1080p50
		case  MISP_VIDOUT_1920x1080p60:		return 0x15;	// 1080p60
		case  MISP_VIDOUT_2048x1536p30:		return 0x16;	// 1536p29
//		case  MISP_VIDOUT_2048x1536p30:		return 0x17;	// 1536p30
		case  MISP_VIDOUT_2048x1536p25:		return 0x18;	// 1536p25
#if defined(__USE_59Hz_OUTPUT__)
		case  MISP_VIDOUT_1920x1080i59:		return 0x01;	// 1080i59
		case  MISP_VIDOUT_1920x1080p29:		return 0x06;	// 1080p29
		case  MISP_VIDOUT_1280x720p59:		return 0x09;	// 720p59
		case  MISP_VIDOUT_1920x1080p59:		return 0x13;	// 1080p59
#endif
		default:							return 0x15;	// 1080p60
	}
}

//-----------------------------------------------------------------------------------------
static void VISCAAPP_CnvVoutFrmtToSTR(PBYTE pSTR, WORD frmt)
{
	BYTE end;
	char vfrmt_item[][17] = {
		"720x480/60i", "720x576/50i", "960x480/60i", "960x576/60i",
		"720x480/60p", "720x576/50p", "960x480/60p", "960x576/50p", "1280x720/60p",
#if defined(__USE_59Hz_OUTPUT__)
		"1280x720/59.94p",
#endif
		"1280x720/50p", "1280x720/30p", "1280x720/25p", "1280x720/24p",	"1920x1080/60i",
#if defined(__USE_59Hz_OUTPUT__)
		"1920x1080/59.94i",
#endif
		"1920x1080/50i", "1920x1080/60p",
#if defined(__USE_59Hz_OUTPUT__)
		"1920x1080/59.94p",
#endif
		"1920x1080/50p", "1920x1080/30p",
#if defined(__USE_59Hz_OUTPUT__)
		"1920x1080/29.97p",
#endif
		"1920x1080/25p", "1920x1080/24p",
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
		"3840x2160/30p",
#if defined(__USE_59Hz_OUTPUT__)
		"3840x2160/29.97p",
#endif
		"3840x2160/25p", "3840x2160/24p", "3840x2160/15p", "3840x2160/12p",
		"4096x2160/30p",
#if defined(__USE_59Hz_OUTPUT__)
		"4096x2160/29.97p",
#endif
		"4096x2160/25p", "4096x2160/24p", "2560x1440/30p", "2560x1440/25p",
#if defined(__USE_4K60_INOUT__)
		"3840x2160/60p",
#if defined(__USE_59Hz_OUTPUT__)
		"3840x2160/59.94p",
#endif
		"3840x2160/50p",
		"4096x2160/60p",
#if defined(__USE_59Hz_OUTPUT__)
		"4096x2160/59.94p",
#endif
		"4096x2160/50p",
#endif
		"1920x1080/120p", "1920x1080/100p",
#endif	// i540, i550
		
		"----------" 
};
	
	end = sizeof(vfrmt_item) / sizeof(vfrmt_item[0]) -1;
	
	if(frmt < MISP_VIDOUT_640x480p60)	memcpy(pSTR, vfrmt_item[frmt], sizeof(vfrmt_item[frmt]));
	else								memcpy(pSTR, vfrmt_item[end], sizeof(vfrmt_item[end]));
}

//-----------------------------------------------------------------------------------------
static void VISCAAPP_CnvIrisToSTR(PBYTE pSTR, BYTE iris)
{
#if defined(__USE_X12X_UNION__)
	char iris_item[][6]={"CLOSE", "F14", "F11", "F9.6", "F8", "F6.8", "F5.6", "F4.8", "F4", "F3.4",
						"F2.8", "F2.4", "F2", "F1.8", "----"};
#else
	char iris_item[][6]={"CLOSE", "F14", "F11", "F9.6", "F8", "F6.8", "F5.6", "F4.8", "F4", "F3.4",
						"F2.8", "F2.4", "F2", "F1.6", "----"};
#endif
	BYTE iris_posi[] ={0x00, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11};
	BYTE i, ae_mode = stVISCAINFO.cam.exposure_mode;
	
	if((ae_mode == VISCA_AEMODE_MANUAL) || (ae_mode == VISCA_AEMODE_IRISFIX))	{	// manual & iris fix mode
		for(i=0;i<sizeof(iris_posi)/sizeof(iris_posi[0]);i++)	{
			if(iris <= iris_posi[i])	{memcpy(pSTR, iris_item[i], sizeof(iris_item[i]));		return;}
		}
		
		memcpy(pSTR, iris_item[i], sizeof(iris_item[i]));		// over max
	}
}

//-----------------------------------------------------------------------------------------
static void VISCAAPP_CnvAGCToSTR(PBYTE pSTR, BYTE agc)
{
	char agc_item[][7]={"0step", "2step", "4step", "6step", "8step", "10step", "12step", "14step", "16step", "18step",
						 "20step", "22step", "24step", "26step", "28step", "----"};
	BYTE ae_mode = stVISCAINFO.cam.exposure_mode;
	
	if(ae_mode == VISCA_AEMODE_MANUAL)	{												// manual mode
		if(agc <= 0x0F)		memcpy(pSTR, agc_item[agc-1], sizeof(agc_item[agc-1]));	// agc : 1~0xF(15)
		else				memcpy(pSTR, agc_item[15], sizeof(agc_item[15]));			// over max
	}
}

//-----------------------------------------------------------------------------------------
static void VISCAAPP_CnvShutToSTR(PBYTE pSTR, BYTE shut)
{
	char shut_item[][6]={"1", "2", "4", "8", "15", "30", "60", "90", "100", "125", 
						"180", "250", "350", "500", "725", "1000", "1500", "2000",	"3000", "4000",
						"6000", "10000", "----"};
	BYTE ae_mode = stVISCAINFO.cam.exposure_mode;
	
	if((ae_mode == VISCA_AEMODE_MANUAL) || (ae_mode == VISCA_AEMODE_SHUTTERFIX))	{	// manual & shut fix mode
		if(shut <= 0x15)	memcpy(pSTR, shut_item[shut], sizeof(shut_item[shut]));	// shut : 0~0x15(21)
		else				memcpy(pSTR, shut_item[22], sizeof(shut_item[22]));		// over max
	}
}

//-----------------------------------------------------------------------------------------
static void VISCAAPP_CnvMFToSTR(PBYTE pSTR, BYTE mode)
{
	char mf_item[][5]={"MF", "FAR", "NEAR"};
	
	if((stVISCAINFO.lens.ctrl1 & VISCA_FOCUS_MODE_AUTO)==0)	{	// manual focus mode
		memcpy(pSTR, mf_item[mode], sizeof(mf_item[mode]));
	}
}

//-----------------------------------------------------------------------------------------
static void VISCAAPP_CnvWBToSTR(PBYTE pSTR, BYTE mode, BOOL blink)
{
	char wb_item[][9]={" ", "INDOOR", "OUTDOOR", "ONE PUSH", "ATW", "WB-MAN", " ", " ", "SVL", " "};
	
	memcpy(pSTR, wb_item[mode], sizeof(wb_item[mode]));				// auto:blank
	
	if((stVISCAINFO.cam.wb_mode == VISCA_WBMODE_ONEPUSH) && blink)	{	// check blink when one-push mode
		if((stVISCAINFO.user.ctrl & VISCA_WB_ONEPUSH_DONE)==0)		memset(pSTR, 0, sizeof(wb_item[mode]));	// set blank
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static WORD VISCAAPP_CnvZoomPosiToRatio(WORD z_posi)
{
	WORD z_posi_item[30]={0x0000, 0x10B9, 0x1B5F, 0x224F, 0x271E, 0x2AB5, 0x2D7B, 0x2FC1, 0x31AB, 0x3352,
						0x34CE, 0x361E, 0x3751, 0x386A, 0x396C, 0x3A5B, 0x3B2B, 0x3BE9, 0x3C94, 0x3D27,
						0x3DA8, 0x3E16, 0x3E78, 0x3EC8, 0x3F11, 0x3F4E, 0x3F86, 0x3FB7, 0x3FE1, 0x4000};
	
	WORD zcomb_posi_item[12]={0x4000, 0x6000, 0x6A80, 0x7000, 0x7300, 0x7540, 0x76C0, 0x7800, 0x78C0, 0x7980,
								0x7A00, 0x7AC0};
	WORD z_ratio=0, dz_ratio=0, i;
	
	for(i=0;i<30;i++)	{
		if(z_posi < z_posi_item[i])		{
			z_ratio = (z_posi - z_posi_item[i-1]) * 10 / DIFF(z_posi_item[i], z_posi_item[i-1]);	break;	// calc decimal point
		}
	}
	z_ratio += (i*10);
	
	if(z_posi > 0x4000)		{
		for(i=0;i<12;i++)	{
			if(z_posi < zcomb_posi_item[i])	{
				dz_ratio = (z_posi - zcomb_posi_item[i-1]) * 10 / DIFF(zcomb_posi_item[i], zcomb_posi_item[i-1]);	break;	// calc decimal point
			}
		}
		dz_ratio += (i*10);
		z_ratio *= dz_ratio;	z_ratio /= 10;
	}
	
	return		z_ratio;
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_DisplayProcess(void)
{
	PVISCA_INFO_PARAM	pINFO = &stVISCAINFO;
	char vfrmt_str[17], iris_str[6], agc_str[7], shut_str[6], mf_str[5], wb_str[9];
	char wdr_str[4]="WDR", icr_str[4]="ICR", zcombi_str[4]="(C)";
	BOOL vfrmt_disp, zoom_disp, dzoom_disp, blink;
	BYTE color=MAKEBYTE(OSD_WHITE, OSD_NOCOLOR), wb_color;
	WORD z_posi, z_ratio;
	
	// calc display mode(counter, disp on/off, blink)
	if(pINFO->user.dscr_disptime_cnt++ >= 0xFF)		pINFO->user.dscr_disptime_cnt=30;	// 0 ~ 30 <-> 0xff
	
	if(pINFO->user.dscr_disptime_cnt < 6)			zoom_disp=ON;	else	zoom_disp=OFF;		// count 3sec
	if(pINFO->user.dscr_disptime_cnt < 30)			vfrmt_disp=ON;	else	vfrmt_disp=OFF;		// count 15sec
	if(pINFO->user.dscr_disptime_cnt % 2)			blink=ON;		else	blink=OFF;			// blink on/off
	if((pINFO->lens.ctrl1 & VISCA_DZOOM_COMBINE_OFF) && zoom_disp)	dzoom_disp=ON;	else	dzoom_disp=OFF;	// dzoom disp on/off
	
	// convert status value to string
	memset(vfrmt_str, 0, sizeof(vfrmt_str));	VISCAAPP_CnvVoutFrmtToSTR((PBYTE)vfrmt_str, GetOUT_MainFrmt());
	memset(iris_str, 0, sizeof(iris_str));		VISCAAPP_CnvIrisToSTR((PBYTE)iris_str, pINFO->cam.iris_posi);
	memset(agc_str, 0, sizeof(agc_str));		VISCAAPP_CnvAGCToSTR((PBYTE)agc_str, pINFO->cam.gain_posi);
	memset(shut_str, 0, sizeof(shut_str));		VISCAAPP_CnvShutToSTR((PBYTE)shut_str, pINFO->cam.shutter_posi);
	memset(mf_str, 0, sizeof(mf_str));			VISCAAPP_CnvMFToSTR((PBYTE)mf_str, 0);
	memset(wb_str, 0, sizeof(wb_str));			VISCAAPP_CnvWBToSTR((PBYTE)wb_str, pINFO->cam.wb_mode, blink);
	
	if(!(stVISCAINFO.other.ctrl2 & VISCA_ICR_ON))			memset(icr_str, 0, sizeof(icr_str));	// set blank
	if(!(stVISCAINFO.efunc2.wd_mode & VISCA_WDMODE_ON))		memset(wdr_str, 0, sizeof(wdr_str));	// set blank
	if((pINFO->lens.ctrl1 & VISCA_DZOOM_COMBINE_OFF) || !(pINFO->lens.ctrl1 & VISCA_DZOOM_ON))	{	// comb(off) or dzoom(off)
		memset(zcombi_str, 0, sizeof(zcombi_str));													// set blank
	}
	if((pINFO->cam.wb_mode == VISCA_WBMODE_ONEPUSH) && (pINFO->user.ctrl & VISCA_WB_ONEPUSH_DONE))	{
		wb_color=MAKEBYTE(OSD_BLUE, OSD_NOCOLOR);
	}
	else	wb_color = color;
	
	z_posi=VISCAAPP_GetZoomPosi();		z_ratio=VISCAAPP_CnvZoomPosiToRatio(z_posi);
	
	// write osd text
	OSD_StrDispAttb( 0,  0, OFF, color, "%36s", "");	// blank
	OSD_StrDispAttb( 1,  0, vfrmt_disp, color, "%-16s %19s", vfrmt_str, "");	// display vout format
	OSD_StrDispAttb( 2,  0, zoom_disp, color, "ZOOM%3s %04X(x%d.%d)  %16s", zcombi_str, z_posi, z_ratio/10, z_ratio%10, "");
	OSD_StrDispAttb( 3,  0, dzoom_disp, color, "DZOOM   %02X %25s", pINFO->efunc1.dzoom_posi, "");
	OSD_StrDispAttb( 3,  28, ON, color, "%3s %32s", wdr_str, "");
	OSD_StrDispAttb( 4,  0, ON, color, "%8s %3s %23s", "", icr_str, "");	// icr
	OSD_StrDispAttb( 5,  0, OFF, color, "%36s", "");	// blank
	OSD_StrDispAttb( 6,  0, OFF, color, "%36s", "");	// blank
	OSD_StrDispAttb( 7,  0, OFF, color, "%36s", "");	// blank
	OSD_StrDispAttb( 8,  0, OFF, color, "%36s", "");	// blank
	OSD_StrDispAttb( 9,  0, ON, color, "%-5s  %6s %22s", iris_str, agc_str, "");	// iris, agc
	OSD_StrDispAttb(10,  0, ON, color, "   %-4s %28s", mf_str, "");	// MF, NEAR, FAR -- enable manual focus
	OSD_StrDispAttb(11,  0, ON, color, "%5s %30s", shut_str, "");	// shut
	OSD_StrDispAttb(11,  7, ON, wb_color, "%-8s %27s", wb_str, "");	// wb
	OSD_StrDispAttb(12,  0, OFF, color, "%36s", "");	// blank
	OSD_StrDispAttb(13,  0, OFF, color, "%36s", "");	// blank
	OSD_StrDispAttb(14,  0, OFF, color, "%36s", "");	// blank
	OSD_StrDispAttb(15,  0, OFF, color, "%36s", "");	// blank
	OSD_StrDispAttb(16,  0, OFF, color, "%36s", "");	// blank
	OSD_StrDispAttb(17,  0, OFF, color, "%36s", "");	// blank
	OSD_StrDispAttb(18,  0, OFF, color, "%36s", "");	// blank
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MultiLineTitleProcess(void)
{
	PVISCA_CAMTITLE_PARAM pTITLE = &stVISCAINFO.camtitle;
	BYTE onoff[11], color[11], i;
	
	for(i=0;i<11;i++)	{
		onoff[i] = (pTITLE->display&(1<<i))?  ON:  OFF;
		if(pTITLE->cnt % 2)	{
			onoff[i] = (pTITLE->blink&(1<<i))?  OFF:  onoff[i];
		}
		
		switch(pTITLE->color[i])	{
			case  0:	color[i] = MAKEBYTE(OSD_WHITE, OSD_NOCOLOR);	break;	// white
			case  1:	color[i] = MAKEBYTE(OSD_YELLOW, OSD_NOCOLOR);	break;	// yellow
			case  2:	color[i] = MAKEBYTE(OSD_NOCOLOR, OSD_NOCOLOR);	break;	// violet -> no-color
			case  3:	color[i] = MAKEBYTE(OSD_NOCOLOR, OSD_NOCOLOR);	break;	// red -> no-color
			case  4:	color[i] = MAKEBYTE(OSD_NOCOLOR, OSD_NOCOLOR);	break;	// cyan -> no-color
			case  5:	color[i] = MAKEBYTE(OSD_NOCOLOR, OSD_NOCOLOR);	break;	// green -> no-color
			case  6:	color[i] = MAKEBYTE(OSD_BLUE, OSD_NOCOLOR);		break;	// blue
			default:	color[i] = MAKEBYTE(OSD_NOCOLOR, OSD_NOCOLOR);	break;	// no-color
		}
	}
	
	if(pTITLE->cnt == 0xff)		pTITLE->cnt = 0;	else	pTITLE->cnt++;
	
	// write osd text
	OSD_StrDispAttb( 0,  0, OFF, color[0], "%36s", "");	// blank
	OSD_StrDispAttb( 1,  pTITLE->h_posi[0], onoff[0], color[0], "%20s%16s", pTITLE->title[0], "");	// line-1
	OSD_StrDispAttb( 2,  pTITLE->h_posi[1], onoff[1], color[1], "%20s%16s", pTITLE->title[1], "");	// line-2
	OSD_StrDispAttb( 3,  pTITLE->h_posi[2], onoff[2], color[2], "%20s%16s", pTITLE->title[2], "");	// line-3
	OSD_StrDispAttb( 4,  pTITLE->h_posi[3], onoff[3], color[3], "%20s%16s", pTITLE->title[3], "");	// line-4
	OSD_StrDispAttb( 5,  pTITLE->h_posi[4], onoff[4], color[4], "%20s%16s", pTITLE->title[4], "");	// line-5
	OSD_StrDispAttb( 6,  pTITLE->h_posi[5], onoff[5], color[5], "%20s%16s", pTITLE->title[5], "");	// line-6
	OSD_StrDispAttb( 7,  pTITLE->h_posi[6], onoff[6], color[6], "%20s%16s", pTITLE->title[6], "");	// line-7
	OSD_StrDispAttb( 8,  pTITLE->h_posi[7], onoff[7], color[7], "%20s%16s", pTITLE->title[7], "");	// line-8
	OSD_StrDispAttb( 9,  pTITLE->h_posi[8], onoff[8], color[8], "%20s%16s", pTITLE->title[8], "");	// line-9
	OSD_StrDispAttb(10,  pTITLE->h_posi[9], onoff[9], color[9], "%20s%16s", pTITLE->title[9], "");	// line-10
	OSD_StrDispAttb(11,  pTITLE->h_posi[10], onoff[10], color[10], "%20s%16s", pTITLE->title[10], "");	// line-11
	OSD_StrDispAttb(12,  0, OFF, color[0], "%36s", "");	// blank
	OSD_StrDispAttb(13,  0, OFF, color[0], "%36s", "");	// blank
	OSD_StrDispAttb(14,  0, OFF, color[0], "%36s", "");	// blank
	OSD_StrDispAttb(15,  0, OFF, color[0], "%36s", "");	// blank
	OSD_StrDispAttb(16,  0, OFF, color[0], "%36s", "");	// blank
	OSD_StrDispAttb(17,  0, OFF, color[0], "%36s", "");	// blank
	OSD_StrDispAttb(18,  0, OFF, color[0], "%36s", "");	// blank
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PanTiltDriveUp(BYTE pan_speed, BYTE tilt_speed)
{
	// add function here
	// UARTprintf("VISCAAPP_PanTiltDriveUp, pan_speed(%d), tilt_speed(%d) not defined..\r\n", pan_speed, tilt_speed);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PanTiltDriveDown(BYTE pan_speed, BYTE tilt_speed)
{
	// add function here
	// UARTprintf("VISCAAPP_PanTiltDriveDown, pan_speed(%d), tilt_speed(%d) not defined..\r\n", pan_speed, tilt_speed);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PanTiltDriveLeft(BYTE pan_speed, BYTE tilt_speed)
{
	// add function here
	// UARTprintf("VISCAAPP_PanTiltDriveLeft, pan_speed(%d), tilt_speed(%d) not defined..\r\n", pan_speed, tilt_speed);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PanTiltDriveRight(BYTE pan_speed, BYTE tilt_speed)
{
	// add function here
	// UARTprintf("VISCAAPP_PanTiltDriveRight, pan_speed(%d), tilt_speed(%d) not defined..\r\n", pan_speed, tilt_speed);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PanTiltDriveUpLeft(BYTE pan_speed, BYTE tilt_speed)
{
	// add function here
	// UARTprintf("VISCAAPP_PanTiltDriveUpLeft, pan_speed(%d), tilt_speed(%d) not defined..\r\n", pan_speed, tilt_speed);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PanTiltDriveUpRight(BYTE pan_speed, BYTE tilt_speed)
{
	// add function here
	// UARTprintf("VISCAAPP_PanTiltDriveUpRight, pan_speed(%d), tilt_speed(%d) not defined..\r\n", pan_speed, tilt_speed);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PanTiltDriveDownLeft(BYTE pan_speed, BYTE tilt_speed)
{
	// add function here
	// UARTprintf("VISCAAPP_PanTiltDriveDownLeft, pan_speed(%d), tilt_speed(%d) not defined..\r\n", pan_speed, tilt_speed);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PanTiltDriveDownRight(BYTE pan_speed, BYTE tilt_speed)
{
	// add function here
	// UARTprintf("VISCAAPP_PanTiltDriveDownRight, pan_speed(%d), tilt_speed(%d) not defined..\r\n", pan_speed, tilt_speed);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PanTiltDriveStop(BYTE pan_speed, BYTE tilt_speed)
{
	// add function here
	// UARTprintf("VISCAAPP_PanTiltDriveStop, pan_speed(%d), tilt_speed(%d) not defined..\r\n", pan_speed, tilt_speed);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PanTiltDriveHome(void)
{
	// add function here
	// UARTprintf("VISCAAPP_PanTiltDriveHome not defined..\r\n");
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PanTiltDriveReset(void)
{
	// add function here
	// UARTprintf("VISCAAPP_PanTiltDriveReset not defined..\r\n");
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PanTiltDriveABSPosi(BYTE pan_speed, BYTE tilt_speed, WORD pan_posi, WORD tilt_posi)
{
	// add function here
	// UARTprintf("VISCAAPP_PanTiltDriveABSPosi, pan/tilt_speed(%d/%d), pan/tilt_position(%d/%d) not defined..\r\n", 
	//			pan_speed, tilt_speed, pan_posi, tilt_posi);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PanTiltDriveRelPosi(BYTE pan_speed, BYTE tilt_speed, WORD pan_posi, WORD tilt_posi)
{
	// add function here
	// UARTprintf("VISCAAPP_PanTiltDriveRelPosi, pan/tilt_speed(%d/%d), pan/tilt_position(%d/%d) not defined..\r\n", 
	//			pan_speed, tilt_speed, pan_posi, tilt_posi);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PanTiltLimitSetDownLeft(WORD pan_limit, WORD tilt_limit)
{
	// add function here
	// UARTprintf("VISCAAPP_PanTiltLimitSetDownLeft, pan/tilt_limit(%d/%d) not defined..\r\n", pan_limit, tilt_limit);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PanTiltLimitSetUpRight(WORD pan_limit, WORD tilt_limit)
{
	// add function here
	// UARTprintf("VISCAAPP_PanTiltLimitSetUpRight, pan/tilt_limit(%d/%d) not defined..\r\n", pan_limit, tilt_limit);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PanTiltLimitClrDownLeft(WORD pan_limit, WORD tilt_limit)
{
	// add function here
	// UARTprintf("VISCAAPP_PanTiltLimitClrDownLeft, pan/tilt_limit(%d/%d) not defined..\r\n", pan_limit, tilt_limit);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_PanTiltLimitClrUpRight(WORD pan_limit, WORD tilt_limit)
{
	// add function here
	// UARTprintf("VISCAAPP_PanTiltLimitClrUpRight, pan/tilt_limit(%d/%d) not defined..\r\n", pan_limit, tilt_limit);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MenuNavi(BYTE key)
{
	switch(key)	{
		case  0x01:		MenuMessageControl();	break;
		case  0x02:		DirLMessageControl();	break;
		case  0x03:		DirRMessageControl();	break;
		case  0x04:		DirUMessageControl();	break;
		case  0x05:		DirDMessageControl();	break;
		case  0x06:		EnterMessageControl();	break;
	}
}



//--------------------------------------------------------------------------------------------------------------------------
static void VISCAAPP_FWUpdate(void)
{
	BYTE  security_key[16] = {'+','8','2','-','0','2','-','2','1','4','2','-','4','0','0','0'};
	BYTE  app_command[8] = {'F','W','U','P','D','A','T','E'};
	DWORD  faddr = 0x4000;				// sector 4(0x4000) for bootloader <-> app communication.
	WORD  saddr = (WORD)(faddr/4096);	// flash sector address
	
	// check reset pointer for bootloader
	if(OSD_GetMenuLang()==0)	{	// eng
		if((DWORD)Reset_Start == *(volatile U32*)0)		{OSD_MessageBox(ON, " Bootloader fail");	Delay_mSec(10000);	return;}
		
		OSD_MessageBox(ON, " F/W Download.. ");
	}
	else	{ 						// chn
		if((DWORD)Reset_Start == *(volatile U32*)0)		{OSD_MessageBox(ON, (PBYTE)msg_chn_bootloader_fail);	Delay_mSec(10000);	return;}
		
		OSD_MessageBox(ON, (PBYTE)msg_chn_fw_download);
	}
	
	// screen off
	SetOUT_MainDarkScreen(ON);					// black screen
	SetOUT_AuxDarkScreen(ON);					// black screen
	Delay_mSec(100);
	
	// write firmware update command to flash.
	sf_erase_sector(saddr, 1);											// erase 1 sector(19 sector : 0x13000 ~ 0x13FFF)
	sf_write(faddr, (PBYTE)security_key, sizeof(security_key));		// write security key for bootloader.
	sf_write(faddr+16, (PBYTE)app_command, sizeof(app_command));		// write firmware update command for bootloader
	
	
	//tm_printf("Ready to reboot\n");
	
	MISP_Reboot();
}

void PrepareFWUpdate(void)
{
	VISCAAPP_FWUpdate();
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCAAPP_MaintenanceMode(BYTE val)
{
	PVISCA_USER_PARAM pINFO = &stVISCAINFO.user;
	
	if(stVISCAINFO.other.ctrl1 & VISCA_POWER_ON)			return;			// skip f/w up when power on
	
	switch(val)	{
		case  0x0C:		memset(&pINFO->maintenance_cmd[0], 0, 4);
						pINFO->maintenance_cmd[0]=0x0C;	break;
		case  0x0D:		pINFO->maintenance_cmd[1]=0x0D;	break;
		case  0x13:		pINFO->maintenance_cmd[2]=0x13;	break;
		case  0x04:		pINFO->maintenance_cmd[3]=0x04;	break;
	}
	
	if(pINFO->maintenance_cmd[0]==0x0C && pINFO->maintenance_cmd[1]==0x0D)	{	// check maintenance mode key
		if(pINFO->maintenance_cmd[2]==0x13 && pINFO->maintenance_cmd[3]==0x04 && val==0x20)	{
			VISCAAPP_FWUpdate();												// run f/w update procedure
		}
	}
}

/*  FILE_END_HERE */
