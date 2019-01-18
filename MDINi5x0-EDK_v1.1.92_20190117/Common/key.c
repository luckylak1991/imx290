//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	KEY.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<stdio.h>
#include	"video.h"
#include	"osd.h"

//#if defined(__USE_UCOM_XRCV01__)
// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
#define		BUTTON_MENU			0x1e
#define		BUTTON_DIRU			0x3f
#define		BUTTON_DIRD			0x7f
#define		BUTTON_DIRL			0x1d
#define		BUTTON_DIRR			0x1b
#define		BUTTON_ENTER		0x17
#define		BUTTON_OPEN			0x1f

static volatile BYTE KeyCmdCode, Key1stCode, Key2ndCode, KeyScanCnt;
static volatile BOOL fButtonCmdParsed, fButtonCompleted;
static volatile BOOL fButtonUARTprintf;

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
void ButtonDebugHandler(void)
{
	if (fButtonUARTprintf==FALSE) return;
	
	fButtonUARTprintf = FALSE;
	UARTprintf("KEY code = 0x%02X\r\n", KeyCmdCode);
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE ISPM ButtonADCtoKEY(WORD val)	  			// convert adc to keycode
{
#if defined(__USE_WW_KEYBUTTON__)
	if(550 > val && val > 500)			return BUTTON_DIRR; //519
	else if(400 > val && val > 350)		return BUTTON_DIRL; //389
	else if(290 > val && val > 230)		return BUTTON_DIRD; //263
	else if(190 > val && val > 100)		return BUTTON_DIRU; //146
	else if(50 > val && val > 10)		return BUTTON_ENTER; //19
	else								return BUTTON_OPEN;
#else
	#if defined (__ECOM1_UNION_12X__)
		return;
	#else
		if(val > 800)				return BUTTON_MENU;	// SW2(980), n-time(980), 1-time(830)
		else if(val > 690)			return BUTTON_DIRU;	// SW3(742)
		else if(val > 550)			return BUTTON_DIRD;	// SW4(599)
		else if(val > 400)			return BUTTON_DIRL;	// SW5(452)
		else if(val > 250)			return BUTTON_DIRR;	// SW6(305)
		else if(val > 100)			return BUTTON_ENTER;	// SW7(153)
		else						return BUTTON_OPEN;	// OPEN(0~100)
	#endif
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM ButtonCommandParse(void)
{
//	KEY_SCAN(Key1stCode);
	Key1stCode = ButtonADCtoKEY(GetADC_ConvData(ADC_SRC_VIN1));			// get adc_vin1 value

	if (Key1stCode==Key2ndCode) KeyScanCnt++;
	else			 Key2ndCode = Key1stCode;

	if (Key1stCode!=Key2ndCode) KeyScanCnt = 0;

	if (KeyScanCnt<6) return;

	KeyScanCnt = 0; KeyCmdCode = Key1stCode;

	if (KeyCmdCode==BUTTON_OPEN)
		 SetMsgButtonNewCmd(KeyCmdCode);
	else fButtonCmdParsed = TRUE;

	fButtonUARTprintf = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM ButtonPreemptMsgHooking(void)
{
	return (IsOtherMsgPreemptive(MSG_BUTTON))? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM IsButtonMsgHooking(void)
{
	if (ButtonPreemptMsgHooking()) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM ButtonMessageClassify(void)
{
	if (fButtonCmdParsed==FALSE) return;
	fButtonCmdParsed = FALSE;

	if (IsButtonMsgHooking()) return;

	SetMsgButtonCtrlMode();
	SetMsgButtonNewCmd(KeyCmdCode);
}

//--------------------------------------------------------------------------------------------------------------------------
static void MenuButtonControl(void)
{
	MenuMessageControl();
}

//--------------------------------------------------------------------------------------------------------------------------
static void DirLButtonControl(void)
{
#if defined (__USE_CMOS_MT9P031__)
	if		(fZOOMMove && (GetOSDMenuID()==0))	{IrCodeZoomMoveDirL(); SetDZOOM_RepeatKeyEnable(); }
//	else if (Turn2MegaPan) {IrCode2MegaMoveDirL(); SetDZOOM_RepeatKeyEnable();}
	else if (fMainCrop)	IrCodeCropMoveDirL();
	else if (GetWBRAWOnOffMODE()) SetWBRAWViewShiftL();
	else					DirLMessageControl();
#else
	if		(fZOOMMove && (GetOSDMenuID()==0))	{IrCodeZoomMoveDirL(); SetDZOOM_RepeatKeyEnable(); }
	else if (fMainCrop)	IrCodeCropMoveDirL();
	else if (fDWindowMove)	{IrCodeDWindowMoveDirL();	SetDZOOM_RepeatKeyEnable();}
	else if (GetWBRAWOnOffMODE()) SetWBRAWViewShiftL();
	else					DirLMessageControl();
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void DirRButtonControl(void)
{
#if defined (__USE_CMOS_MT9P031__)
	if		(fZOOMMove && (GetOSDMenuID()==0))	{IrCodeZoomMoveDirR(); SetDZOOM_RepeatKeyEnable();}
//	else if (Turn2MegaPan) {IrCode2MegaMoveDirR(); SetDZOOM_RepeatKeyEnable();}
	else if (fMainCrop)	IrCodeCropMoveDirR();
	else if (GetWBRAWOnOffMODE()) SetWBRAWViewShiftR();
	else					DirRMessageControl();
#else
	if		(fZOOMMove && (GetOSDMenuID()==0))	{ IrCodeZoomMoveDirR(); SetDZOOM_RepeatKeyEnable(); }
	else if (fMainCrop)	IrCodeCropMoveDirR();
	else if (fDWindowMove)	{IrCodeDWindowMoveDirR();	SetDZOOM_RepeatKeyEnable();}
	else if (GetWBRAWOnOffMODE()) SetWBRAWViewShiftR();
	else					DirRMessageControl();
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void EnterButtonControl(void)
{
	if(GetOSDMenuID()==0x0000)		MenuMessageControl();
	else							EnterMessageControl();
}

//--------------------------------------------------------------------------------------------------------------------------
static void DirUButtonControl(void)
{
#if defined (__USE_CMOS_MT9P031__)
	if (fZOOMMove && (GetOSDMenuID()==0))	{IrCodeZoomMoveDirU(); SetDZOOM_RepeatKeyEnable();}
//	else if (Turn2MegaPan) {IrCode2MegaMoveDirU(); SetDZOOM_RepeatKeyEnable(); }
	else if (fMainCrop)	IrCodeCropMoveDirU();
	else if (GetWBRAWOnOffMODE()) SetWBRAWViewShiftU();
	else					DirUMessageControl();  
#else
	if (fZOOMMove && (GetOSDMenuID()==0))	{ IrCodeZoomMoveDirU(); SetDZOOM_RepeatKeyEnable(); }
	else if (fMainCrop)	IrCodeCropMoveDirU();
	else if (GetWBRAWOnOffMODE()) SetWBRAWViewShiftU();
	else					DirUMessageControl();  
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void DirDButtonControl(void)
{
#if defined (__USE_CMOS_MT9P031__)
	if		(fZOOMMove && (GetOSDMenuID()==0)) {IrCodeZoomMoveDirD(); SetDZOOM_RepeatKeyEnable();}
//	else if (Turn2MegaPan){IrCode2MegaMoveDirD();SetDZOOM_RepeatKeyEnable();}
	else if (fMainCrop)	IrCodeCropMoveDirD();
	else if (GetWBRAWOnOffMODE()) SetWBRAWViewShiftD();
	else					DirDMessageControl();
#else
	if		(fZOOMMove && (GetOSDMenuID()==0)) { IrCodeZoomMoveDirD();SetDZOOM_RepeatKeyEnable();}
	else if (fMainCrop)	IrCodeCropMoveDirD();
	else if (GetWBRAWOnOffMODE()) SetWBRAWViewShiftD();
	else					DirDMessageControl();
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void ButtonMessageExecution(void)
{
//	UARTprintf("KEY code = 0x%02X\r\n", KeyCmdCode);

	switch (KeyCmdCode) {
		case BUTTON_MENU:		MenuButtonControl(); break;
		case BUTTON_DIRU:		DirUButtonControl(); break;
		case BUTTON_DIRD:		DirDButtonControl(); break;
		case BUTTON_DIRL:		DirLButtonControl(); break;
		case BUTTON_DIRR:		DirRButtonControl(); break;
		case BUTTON_ENTER:		EnterButtonControl(); break;
		default:				SetMsgCommandComplete(ON); break;
	}
}
//#endif	/* defined(__USE_UCOM_XRCV01__) */

/*  FILE_END_HERE */
