//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2018  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	USB_APP.C
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
USB_CTRL_PARAM	stUSBCTRL;

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static volatile BOOL fUSB_CmdParsed;

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
void USBAPP_Gamma(BYTE nID)	// 0 ~ 10 : default, off(1.0), 0.9, 0.8, 0.7, 0.6, 0.55, 0.5, 0.45, 0.4, custom, 
{
	SetBY_GammaValue(nID);			// front gamma
	//SetTONE_RGBGAMMA_val(nID);	// rear gamma
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_WBMode(BYTE mode)
{
	switch(mode)	{
		case  0x00:		SetWBModeSetMenu(0);		break;		// auto
		case  0x01:		SetWBModeSetMenu(2);		break;		// indoor
		case  0x02:		SetWBModeSetMenu(3);		break;		// outdoor
		case  0x03:		SetWBModeSetMenu(4);		break;		// manual
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_WBManGainR(BYTE gain)	// 0 ~ 20
{
	DEMO_SetWBManRed(gain*5);		// 0 ~ 20 -> 0 ~ 100
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_WBManGainB(BYTE gain)	// 0 ~ 20
{
	DEMO_SetWBManBlue(gain*5);		// 0 ~ 20 -> 0 ~ 100
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_AEMode(BYTE mode)
{
	switch(mode)	{
		case  0x00:		SetAECtrlOnOff(ON);		AECTRL_SetAE_AutoMode();		break;	// auto
		case  0x01:		SetAECtrlOnOff(OFF);	AECTRL_SetAE_Manual();			break;	// manual
		case  0x02:		SetAECtrlOnOff(ON);		AECTRL_SetAE_ShutterFixMode();	break;	// shutter priority
		case  0x03:		SetAECtrlOnOff(ON);		AECTRL_SetAE_IRISFixMode();		break;	// iris priority
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_BLC(BYTE OnOff)		// off, on
{
	SetBLC_OnOff(OnOff);
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_BrightAE(BYTE val)	// 0 ~ 10
{
	SetAEBrightLvl(val);
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_AGCGain(BYTE gain)	// 0 ~ : auto, 0db, 2db, 4db,,,
{
	SetAEAGCGain(gain);
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_IRISGain(BYTE gain)	// 0 ~ 7: F16, F11, F8, F5.6, F4, F2.8, F2, F1.6
{
	SetAEIrisGain(gain);
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_ShutGain(BYTE gain)	// 0 ~ 11 : auto, 1/30, 1/60, 1/120, 1/180, 1/240, 1/300, 1/500, 1/1k, 1/2k, 1/5k, 1/10k
{
	SetAEShutSpeed(gain);
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_FocusStop(void)
{
	if(stUSBCTRL.ctrl & USB_FOCUS_MASK)	{
		stUSBCTRL.ctrl &= (~USB_FOCUS_MASK); 				// clear focus mode(near/far)
		USB_SetRepeatCmd(0xF000); 							// zoom/foks/iris drive stop command
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_FocusNear(void)
{
	stUSBCTRL.ctrl &= (~USB_FOCUS_MASK); 					// clear focus mode(near/far)
	stUSBCTRL.ctrl |= USB_FOCUS_NEAR; 						// set focus mode
	USB_SetRepeatCmd(0x4000); 								// foks drive command repeat enable
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_FocusFar(void)
{
	stUSBCTRL.ctrl &= (~USB_FOCUS_MASK); 					// clear focus mode(near/far)
	stUSBCTRL.ctrl |= USB_FOCUS_FAR; 						// set focus mode
	USB_SetRepeatCmd(0x3000); 								// foks drive command repeat enable
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_ZoomStop(void)
{
	if(stUSBCTRL.ctrl & USB_ZOOM_MASK)	{
		stUSBCTRL.ctrl &= (~USB_ZOOM_MASK); 				// clear zoom mode(tele/wide)
		USB_SetRepeatCmd(0xF000); 							// zoom/foks/iris drive stop command
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_ZoomWide(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	SetNR_NRZoomingMode(); 									// nr : zooming mode
	LENS_SetZOOMDrvSPEED(&stAFCTRL, AFZOOM_SPEED_VAR3); 	// zoom speed : normal
#endif
	
	stUSBCTRL.ctrl &= (~USB_ZOOM_MASK); 					// clear zoom mode(tele/wide)
	stUSBCTRL.ctrl |= USB_ZOOM_WIDE; 						// set zoom mode
	USB_SetRepeatCmd(0x2000); 								// zoom drive command repeat enable
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_ZoomTele(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	SetNR_NRZoomingMode(); 									// nr : zooming mode
	LENS_SetZOOMDrvSPEED(&stAFCTRL, AFZOOM_SPEED_VAR3); 	// zoom speed : normal
#endif
	
	stUSBCTRL.ctrl &= (~USB_ZOOM_MASK); 					// clear zoom mode(tele/wide)
	stUSBCTRL.ctrl |= USB_ZOOM_TELE; 						// set zoom mode
	USB_SetRepeatCmd(0x1000); 								// zoom drive command repeat enable
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_SetZoomPosition(WORD val) 					// 0 ~ 65535
{
	stUSBCTRL.zoom_posi = (val / 4); 					// 0~65535(0xFFFF) -> 0~16383(0x3FFF)
	USB_SetRepeatCmd(0x5000); 							// zoom direct drive command repeat enable
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_ManualFocusOnOff(BYTE OnOff) 				// 0:auto,  1:manual
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	stAFCTRL.ctrl &= (~AFCTRL_AFAUTO_MASK);
	
	if(OnOff) 	stAFCTRL.ctrl |= AFCTRL_MANUAL_MODE; 		// manual
	else 		stAFCTRL.ctrl |= AFCTRL_AFAUTO_MODE; 		// auto
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_SetDZoomPosition(WORD val)
{
	SetDZOOM_TargetRatio(MIN(val,50));							// 0(x1.0) ~ 50(x6.0)
}

//--------------------------------------------------------------------------------------------------------------------------
WORD USBAPP_GetDZoomPosition(void)
{
	return GetDZOOM_Ratio();
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_SetDZoomPan(WORD val)
{
	WORD pan, tilt;
	GetDZOOM_Position(&pan, &tilt);
	SetDZOOM_Position(val, tilt);								// 0(left_end) ~ 120(center) ~ 240(right_end)
}

//--------------------------------------------------------------------------------------------------------------------------
WORD USBAPP_GetDZoomPan(void)
{
	WORD pan, tilt;
	GetDZOOM_Position(&pan, &tilt);
	
	return pan;
}

//--------------------------------------------------------------------------------------------------------------------------
void USBAPP_SetDZoomTilt(WORD val)
{
	WORD pan, tilt;
	GetDZOOM_Position(&pan, &tilt);
	SetDZOOM_Position(pan, val);								// 0(top) ~ 120(center) ~ 240(bottom)
}

//--------------------------------------------------------------------------------------------------------------------------
WORD USBAPP_GetDZoomTilt(void)
{
	WORD pan, tilt;
	GetDZOOM_Position(&pan, &tilt);
	
	return tilt;
}

//--------------------------------------------------------------------------------------------------------------------------
static void USBAPP_SetRepeatCmd(WORD cmd)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	WORD z_posi=stUSBCTRL.zoom_posi;//, f_posi=stUSBCTRL.focus_posi;
#endif
	
	switch(cmd)	{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
		case  0x1000:		SetDirDriveZOOM(zTELE);				break;	// zoom tele(in)
		case  0x2000:		SetDirDriveZOOM(zWIDE);				break;	// zoom wide(out)
		case  0x3000:		SetDirDriveFOKS(fFAR);				break;	// focus far
		case  0x4000:		SetDirDriveFOKS(fNEAR);				break;	// focus near
		case  0x5000:		TargetDriveZOOM(z_posi);			break;	// zoom direct
		//case  0x6000:		TargetDriveFOKS(f_posi);			break;	// focus direct
#else
		case  0x1000:		SetDZOOM_DriveTeleVar(4);			break;	// dzoom tele(speed:0~7)
		case  0x2000:		SetDZOOM_DriveWideVar(4);			break;	// dzoom wide(speed:0~7)
#endif
		//case  0x7000:		SetDZOOM_DriveTeleVar(4);			break;	// dzoom tele(speed:0~7)
		//case  0x8000:		SetDZOOM_DriveWideVar(4);			break;	// dzoom wide(speed:0~7)
		case  0xF000:		SetMsgCommandComplete(ON);			break;	// foks/zoom stop
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM USB_PreemptMsgHooking(void)
{
	if (!IsOtherMsgPreemptive(MSG_USBCOM)) return FALSE;

	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM IsUSB_MsgHooking(void)
{
	if (USB_PreemptMsgHooking()) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM USB_MessageClassify(void)
{
	if (fUSB_CmdParsed==FALSE) return;

	fUSB_CmdParsed = FALSE;
	if (IsUSB_MsgHooking()==TRUE) return;

	SetMsgUSB_CtrlMode();
	SetMsgUSB_NewCmd(stUSBCTRL.RepeatCmd);
//	USB_CommandResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL ISPM IsUSB_CommandZOOM(void)
{
	switch (stUSBCTRL.RepeatCmd) {
		case 0x1000:	case 0x2000:	// ZOOM Standard
		case 0x5000:					// ZOOM direct
		case 0x3000:	case 0x4000:	// FOKS Standard
		//case 0x6000:					// FOKS direct
						return TRUE;
		default:		return FALSE;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void USB_MessageExecution(void)
{
	USBAPP_SetRepeatCmd(stUSBCTRL.RepeatCmd);
}

//--------------------------------------------------------------------------------------------------------------------------
void USB_SetRepeatCmd(WORD cmd)
{
	stUSBCTRL.RepeatCmd = cmd;
	fUSB_CmdParsed = TRUE;
}



/*  FILE_END_HERE */
