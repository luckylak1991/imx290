//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2016  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	PELCO_APP.C
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

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_AutoScan(BOOL OnOff)
{
	stPELCOINFO.scan = OnOff; 								// 0:manual, 1:auto
	// add auto/manual scan function here...
	UARTprintf("PELCOAPP_AutoScan(%s) \r\n", (OnOff)?  "Auto":  "Manual");
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_CamOnOff(BOOL OnOff)
{
	stPELCOINFO.power = OnOff; 								// 0:off, 1:on
	// add camera power on/off function here...
	UARTprintf("PELCOAPP_CamOnOff(%s) \r\n", (OnOff)?  "ON":  "OFF");
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_IrisStop(void)
{
	if(stPELCOINFO.iris)	{
		stPELCOINFO.iris = 0; 								// 0:stop, 1:open, 2:close
		PELCO_SetRepeatCmd(0xF000); 						// zoom/foks/iris drive stop command
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_IrisClose(void)
{
	stPELCOINFO.iris = 2; 									// 0:stop, 1:open, 2:close
	PELCO_SetRepeatCmd(0xA000); 							// iris drive close command repeat enable
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_IrisOpen(void)
{
	stPELCOINFO.iris = 1; 									// 0:stop, 1:open, 2:close
	PELCO_SetRepeatCmd(0x9000); 							// iris drive open command repeat enable
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_FocusStop(void)
{
	if(stPELCOINFO.foks)	{
		stPELCOINFO.foks = 0; 								// 0:stop, 1:near, 2:far
		PELCO_SetRepeatCmd(0xF000); 						// zoom/foks/iris drive stop command
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_FocusNear(void)
{
	stPELCOINFO.foks = 1; 									// 0:stop, 1:near, 2:far
	PELCO_SetRepeatCmd(0x4000); 							// foks drive wide command repeat enable
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_FocusFar(void)
{
	stPELCOINFO.foks = 2; 									// 0:stop, 1:near, 2:far
	PELCO_SetRepeatCmd(0x3000); 							// foks drive wide command repeat enable
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_ZoomStop(void)
{
	if(stPELCOINFO.zoom)	{
		stPELCOINFO.zoom = 0; 								// 0:stop, 1:tele, 2:wide
		PELCO_SetRepeatCmd(0xF000); 						// zoom/foks/iris drive stop command
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_ZoomWide(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	SetNR_NRZoomingMode(); 									// nr : zooming mode
	LENS_SetZOOMDrvSPEED(&stAFCTRL, AFZOOM_SPEED_VAR3); 	// zoom speed : normal
#endif
	
	stPELCOINFO.zoom = 2; 									// 0:stop, 1:tele, 2:wide
	PELCO_SetRepeatCmd(0x2000); 							// zoom drive wide command repeat enable
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_ZoomTele(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	SetNR_NRZoomingMode(); 									// nr : zooming mode
	LENS_SetZOOMDrvSPEED(&stAFCTRL, AFZOOM_SPEED_VAR3); 	// zoom speed : normal
#endif
	
	stPELCOINFO.zoom = 1; 									// 0:stop, 1:tele, 2:wide
	PELCO_SetRepeatCmd(0x1000); 							// zoom drive tele command repeat enable
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_PanStop(void)
{
	if(stPELCOINFO.pan)	{
		stPELCOINFO.pan = 0; 								// 0:stop, 1:right, 2:left
		// add pan_stop function here
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_PanLeft(BYTE speed)
{
	stPELCOINFO.pan = 2; 									// 0:stop, 1:right, 2:left
	// add pan_left function here
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_PanRight(BYTE speed)
{
	stPELCOINFO.pan = 1; 									// 0:stop, 1:right, 2:left
	// add pan_right function here
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_TiltStop(void)
{
	if(stPELCOINFO.tilt)	{
		stPELCOINFO.tilt = 0; 								// 0:stop, 1:up, 2:down
		// add tilt_stop function here
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_TiltDown(BYTE speed)
{
	stPELCOINFO.tilt = 2; 									// 0:stop, 1:up, 2:down
	// add tilt_down function here
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_TiltUp(BYTE speed)
{
	stPELCOINFO.tilt = 1; 									// 0:stop, 1:up, 2:down
	// add tilt_up function here
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_ZoomSpeed(BYTE val) 							// 0:slow, 1:low, 2:high, 3:highest
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))

	LONG mode = AFZOOM_SPEED_VAR4;
	
	switch(val)	{
		case  0x00:	mode = AFZOOM_SPEED_VAR0;	break; // slow
		case  0x01:	mode = AFZOOM_SPEED_VAR2;	break; // low
		case  0x02:	mode = AFZOOM_SPEED_VAR5;	break; // high
		case  0x03:	mode = AFZOOM_SPEED_VAR7;	break; // highest
	}
	
	LENS_SetZOOMDrvSPEED(&stAFCTRL, mode);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_FocusSpeed(BYTE val) 							// 0:slow, 1:low, 2:high, 3:highest
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))

	LONG mode = AFFOKS_SPEED_VAR4;
	
	switch(val)	{
		case  0x00:	mode = AFFOKS_SPEED_VAR0;	break; // slow
		case  0x01:	mode = AFFOKS_SPEED_VAR2;	break; // low
		case  0x02:	mode = AFFOKS_SPEED_VAR5;	break; // high
		case  0x03:	mode = AFFOKS_SPEED_VAR7;	break; // highest
	}
	
	LENS_SetFOKSDrvSPEED(&stAFCTRL, mode);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_AutoFocusOnOff(BYTE val) 					// 0:auto, 1:manual
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	stAFCTRL.ctrl &= (~AFCTRL_AFAUTO_MASK);
	
	if(val) 	stAFCTRL.ctrl |= AFCTRL_MANUAL_MODE; 		// manual
	else 		stAFCTRL.ctrl |= AFCTRL_AFAUTO_MODE; 		// auto
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_SetPanPosition(WORD val) 					// 0 ~ 35999(degree * 100)
{
	// add function here
	UARTprintf("set pan position function not defined.. degree(%d C) \r\n", val/100);
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_SetTiltPosition(WORD val) 					// 0 ~ 35999(degree * 100)
{
	// add function here
	UARTprintf("set tilt position function not defined.. degree(%d C) \r\n", val/100);
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_SetZoomPosition(WORD val) 					// 0 ~ 65535(posi / limit * 65535)
{
	stPELCOINFO.z_posi = (val / 4); 						// 0~65535(0xFFFF) -> 0~16383(0x3FFF)
	PELCO_SetRepeatCmd(0x5000); 							// zoom direct drive command repeat enable
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_SetFlipOnOff(void) 							// on <-> off
{
	BYTE  mode;
	
	if(stPELCOINFO.info & PELCO_FLIP_ON)	{stPELCOINFO.info &= (~PELCO_FLIP_ON);	mode=0;} 	// off
	else									{stPELCOINFO.info |= PELCO_FLIP_ON;		mode=3;} 	// H+V
	
	DEMO_SetMirrorMode(mode);	// 0:off, 1:H, 2:V, 3:H+V
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_EnterMenuMode(void) 							// on <-> off
{
	WORD  mID;
	
	if(stPELCOINFO.info & PELCO_MENU_ON)	{stPELCOINFO.info &= (~PELCO_MENU_ON);	mID=0;} 		// off
	else									{stPELCOINFO.info |= PELCO_MENU_ON;		mID=0x1000;} 	// on
	
	SetOSDMenuID(mID);	// 0:off, 0x1000:main menu on
}





//--------------------------------------------------------------------------------------------------------------------------
BYTE PELCOAPP_GetAlarm(void)
{
	// add function here
	return 	(stPELCOINFO.alarm & 0x7F);
}

//--------------------------------------------------------------------------------------------------------------------------
WORD PELCOAPP_GetPanPosition(void)
{
	// add function here
	UARTprintf("get pan position function not defined..\r\n");
	return 	0;
}

//--------------------------------------------------------------------------------------------------------------------------
WORD PELCOAPP_GetTiltPosition(void)
{
	// add function here
	UARTprintf("get tilt position function not defined..\r\n");
	return 	0;
}

//--------------------------------------------------------------------------------------------------------------------------
WORD PELCOAPP_GetZoomPosition(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	return		MIN(0xFFFF, LENS_GetVISCAPositionZOOM() * 4); 		// 0~16384(0x4000) -> 0~65535(0xFFFF)
#else
	return 	0;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
WORD PELCOAPP_GetMagVal(void)
{
	return 	(stPELCOINFO.alarm & 0x7F);
}




// motor drive function(called every v-sync)
//--------------------------------------------------------------------------------------------------------------------------
static void SetIRIS_DriveOpen(void)
{
	// add 'iris drive open' function here
	SetMsgCommandRepeatKey(0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetIRIS_DriveClose(void)
{
	// add 'iris drive close' function here
	SetMsgCommandRepeatKey(0);
}


//--------------------------------------------------------------------------------------------------------------------------
void PELCOAPP_ExeRepeatCmd(WORD cmd)
{
	switch(cmd&0xFF00)	{ 				// hi_byte(cmd), lo_byte(speed)
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
		case  0x1000:		SetDirDriveZOOM(zTELE);					break;	// zoom tele(in)
		case  0x2000:		SetDirDriveZOOM(zWIDE);					break;	// zoom wide(out)
		case  0x3000:		SetDirDriveFOKS(fFAR);					break;	// focus far
		case  0x4000:		SetDirDriveFOKS(fNEAR);					break;	// focus near
		case  0x5000:		TargetDriveZOOM(stPELCOINFO.z_posi);	break;	// zoom direct
#else
		case  0x1000:		SetDZOOM_DriveTeleVar(4);				break;	// dzoom tele(speed:0~7)
		case  0x2000:		SetDZOOM_DriveWideVar(4);				break;	// dzoom wide(speed:0~7)
#endif
		case  0x9000:		SetIRIS_DriveOpen();					break;	// iris open
		case  0xA000:		SetIRIS_DriveClose();					break;	// iris close
		case  0xB000:		SetDZOOM_DriveTeleVar(4);				break;	// dzoom tele(speed:0~7)
		case  0xC000:		SetDZOOM_DriveWideVar(4);				break;	// dzoom wide(speed:0~7)
		case  0xF000:		SetMsgCommandComplete(ON);				break;	// foks/zoom stop
	}
}


/*  FILE_END_HERE */
