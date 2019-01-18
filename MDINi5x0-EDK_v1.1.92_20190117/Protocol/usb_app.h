//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2018  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  USB_APP.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__USB_APP_H__
#define		__USB_APP_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
typedef	struct
{
	LONG	ctrl;					// control flag
	
	WORD	zoom_posi;
	WORD	focus_posi;
	WORD	RepeatCmd;
	WORD	dummy;

}	USB_CTRL_PARAM, *PUSB_CTRL_PARAM;

// ctrl param
#define		USB_TRIGGER							0x80000000		// usb process handler tirgger

#define		USB_FOCUS_STOP 						0x00000000 		// usb focus mode(stop)
#define		USB_FOCUS_NEAR 						0x00000001 		// usb focus mode(near)
#define		USB_FOCUS_FAR 						0x00000002 		// usb focus mode(far)
#define		USB_FOCUS_MASK 						0x00000003 		// usb focus mode mask

#define		USB_ZOOM_STOP 						0x00000000 		// usb zoom mode(stop)
#define		USB_ZOOM_TELE 						0x00000010 		// usb zoom mode(tele)
#define		USB_ZOOM_WIDE 						0x00000020 		// usb zoom mode(wide)
#define		USB_ZOOM_MASK 						0x00000030 		// usb zoom mode mask

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

void USBAPP_Gamma(BYTE nID);
void USBAPP_WBMode(BYTE mode);
void USBAPP_WBManGainR(BYTE gain);
void USBAPP_WBManGainB(BYTE gain);
void USBAPP_AEMode(BYTE mode);
void USBAPP_BLC(BYTE OnOff);
void USBAPP_BrightAE(BYTE val);
void USBAPP_AGCGain(BYTE gain);
void USBAPP_IRISGain(BYTE gain);
void USBAPP_ShutGain(BYTE gain);

void USBAPP_FocusStop(void);
void USBAPP_FocusNear(void);
void USBAPP_FocusFar(void);
void USBAPP_ZoomStop(void);
void USBAPP_ZoomWide(void);
void USBAPP_ZoomTele(void);
void USBAPP_SetZoomPosition(WORD val);
void USBAPP_ManualFocusOnOff(BYTE OnOff);
void USBAPP_SetDZoomPosition(WORD val);
WORD USBAPP_GetDZoomPosition(void);
void USBAPP_SetDZoomPan(WORD val);
WORD USBAPP_GetDZoomPan(void);
void USBAPP_SetDZoomTilt(WORD val);
WORD USBAPP_GetDZoomTilt(void);


void ISPM USB_MessageClassify(void);
BOOL ISPM IsUSB_CommandZOOM(void);
void USB_MessageExecution(void);
void USB_SetRepeatCmd(WORD cmd);


#endif	/* __USB_APP_H__ */
