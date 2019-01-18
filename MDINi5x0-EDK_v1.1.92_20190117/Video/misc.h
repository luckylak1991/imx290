//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2015  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MISC.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISC_H__
#define		__MISC_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
typedef	struct
{
	LONG 	ctrl;				// control flag
	BYTE 	agc_d2n;			// day -> night agc level
	BYTE 	agc_n2d;			// night -> day agc level
	BYTE 	dw_time;			// dwell time(0~60 sec)
	BYTE	dummy_001;
	WORD 	dw_cnt;				// dwell time counter(0~60 sec * (25hz or 30hz))
	WORD	wOLPFCnt;			// icr stop time counter
	
}	DYNT_CTRL_PARAM, *PDYNT_CTRL_PARAM;

typedef	struct
{
	LONG 	ctrl;				// dzoom control flag
	WORD	DZratio;			// dzoom ratio(0:x1 ~ 55:x12_1080p) -- inc/dec by direct posi command(visca,etc)
	WORD	DZtarget;			// dzoom ratio target(0:x1 ~ 55:x12_1080p) -- inc/dec by menu
	WORD	DZoffset;			// dzoom ratio offset(0:x1 ~ 55:x12_1080p) -- inc/dec by tele/wide command(visca,etc)
	WORD	DZposix;			// dzoom position-x(0~240)
	WORD	DZposiy;			// dzoom position-y(0~240)
	WORD 	PIPsize;			// pip size(0:1/4 ~ 3:1/25)
	WORD	PIPposix;			// pip position-x(0~10)
	WORD	PIPposiy;			// pip position-y(0~10)
	WORD	PIPonoff;			// pip onoff
	WORD	dummy;
	MISP_VIDEO_WINDOW	DZwin;	// dzoom window
	MISP_VIDEO_WINDOW	PIPwin;	// pip window
	
}	DZOOM_CTRL_PARAM, *PDZOOM_CTRL_PARAM;

typedef	struct
{
	LONG	ctrl;					// control flag
	WORD	x[2];					// blc window position x, [1]:default value
	WORD	y[2];					// blc window position y, [1]:default value
	WORD	w[2];					// blc window width, [1]:default value
	WORD	h[2];					// blc window height, [1]:default value
	BYTE	w_max;					// blc window width max
	BYTE	h_max;					// blc window height max
	WORD	dummy;

}	BLC_CTRL_PARAM, *PBLC_CTRL_PARAM;

typedef	struct
{
	LONG	ctrl;					// control flag
	WORD	x[2];					// hlc window position x, [1]:default value
	WORD	y[2];					// hlc window position y, [1]:default value
	WORD	w[2];					// hlc window width, [1]:default value
	WORD	h[2];					// hlc window height, [1]:default value
	BYTE	w_max;					// hlc window width limit
	BYTE	h_max;					// hlc window height limit
	WORD	dummy;
	BYTE	level[2];				// hlc level, [1]:default value
	BYTE	mask_level[2];			// hlc mask level, [1]:default value

}	HLC_CTRL_PARAM, *PHLC_CTRL_PARAM;

typedef	struct
{
	LONG	ctrl;					// control flag
	WORD	x;						// vector scope scan position x
	WORD	y;						// vector scope scan position y
	WORD	w;						// vector scope scan width
	WORD	h;						// vector scope scan height
	WORD	w_max;					// vector scope scan width limit
	WORD	h_max;					// vector scope scan height limit

}	VS_CTRL_PARAM, *PVS_CTRL_PARAM;


// day&night control parameter
#define 	DYNTCTRL_MODE_MASK			0x00000007		// day night mode mask
#define 	DYNTCTRL_MODE_AUTO			0x00000000		// day night mode : auto
#define 	DYNTCTRL_MODE_DAY			0x00000001		// day night mode : day(color)
#define 	DYNTCTRL_MODE_NIGHT			0x00000002		// day night mode : night(b/w)
#define 	DYNTCTRL_MODE_EXT			0x00000003		// day night mode : external signal(CDS, etc,,)
#define		DYNTCTRL_MODE_SLAVE			0x00000004		// day night mode : slave

#define 	DYNTCTRL_NT_COLOR_DIS		0x00000000		// day night night color disable flag
#define 	DYNTCTRL_NT_COLOR_EN		0x00000010		// day night night color enable flag
#define 	DYNTCTRL_COLOR_BURST_DIS	0x00000000		// day night color burst disable flag
#define 	DYNTCTRL_COLOR_BURST_EN		0x00000020		// day night color burst enable flag
#define 	DYNTCTRL_IS_DAY				0x00000000		// day night agc check result : day
#define 	DYNTCTRL_IS_NIGHT			0x00000040		// day night agc check result : night
#define 	DYNTCTRL_BW_DIS				0x00000000		// day night b/w disable
#define 	DYNTCTRL_BW_EN				0x00000080		// day night b/w enable
//#define 	DYNTCTRL_PARAM_INIT_DONE	0x80000000		// day night parameter initialize done flag

#define		DYNTCTRL_AGC_OFFSET			256				// day night agc offset


// dzoom control parameter
#define 	DZOOMCTRL_TRIGGER			0x80000000		// dzoom process handler trigger
#define		DZOOMCTRL_DZ_ON				0x00000100		// dzoom on flag
#define		DZOOMCTRL_DZ_OFF			0x00000000		// dzoom off flag
#define		DZOOMCTRL_PROC_MASK			0x000000FF		// dzoom process mask
#define		DZOOMCTRL_ZOOM_MASK			0x0000000F		// dzoom zooming mask
#define 	DZOOMCTRL_TELE_IN			0x00000001		// dzoom tele(in)
#define 	DZOOMCTRL_WIDE_OUT			0x00000002		// dzoom wide(out)
#define		DZOOMCTRL_DIRECTRATIO		0x00000004		// dzoom direct ratio
#define		DZOOMCTRL_TARGETRATIO		0x00000008		// dzoom target ratio
#define		DZOOMCTRL_POSI				0x00000010		// dzoom move position
#define		PIPCTRL_PROC_MASK			0x00030000		// pip process mask
#define		PIPCTRL_PROC_EN				0x00010000		// pip process enable
#define		PIPCTRL_PROC_DIS			0x00020000		// pip process disable
#define		PIPCTRL_BOXOSD_ON			0x00100000		// pip box osd on
#define		PIPCTRL_BOXOSD_OFF			0x00000000		// pip box osd off


#if defined(__M33_V101__)
#define		DZOOMCTRL_RATIO_MAX			59				// 
#else
#define		DZOOMCTRL_RATIO_MAX			55				// 
#endif


// blc control parameter
#define		BLCCTRL_MODE_ON				0x00000001		// blc on
#define		BLCCTRL_MODE_OFF			0x00000000		// blc off
#define		BLCCTRL_WIND_ON				0x00000002		// blc window display on
#define		BLCCTRL_WIND_OFF			0x00000000		// blc window display off


// hlc control parameter
#define		HLCCTRL_MODE_ON				0x00000001		// hlc on
#define		HLCCTRL_MODE_OFF			0x00000000		// hlc off
#define		HLCCTRL_WIND_ON				0x00000002		// hlc window display on
#define		HLCCTRL_WIND_OFF			0x00000000		// hlc window display off
#define		HLCCTRL_MASK_ON				0x00000004		// hlc mask display on
#define		HLCCTRL_MASK_OFF			0x00000000		// hlc mask display off

// vs control parameter
#define 	VSCTRL_DRAW_TRIG			0x40000000		// vector scope draw trigger
#define		VSCTRL_MODE_ON				0x00000001		// vector scope on
#define		VSCTRL_MODE_OFF				0x00000000		// vector scope off
#define		VSCTRL_SCAN_FULL			0x00000000		// vector scope scan full
#define		VSCTRL_SCAN_USER			0x00000010		// vector scope scan user area

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void MISC_Initialize(void);
void DZOOM_ProcessHandler(void);
void DYNT_ProcessHandler(PWB_CTRL_PARAM pWBCTRL);

void SetDYNT_Mode(WORD nID);
void SetDYNT_AutoDly(WORD nID);
void SetDYNT_AGCD2N(WORD nID);
void SetDYNT_AGCN2D(WORD nID);
void SetDYNT_NTColor(WORD nID);
void SetDYNT_CVBSBurst(WORD nID);
void SetDYNT_ICR(BOOL OnOff);
WORD GetDYNT_Mode(void);
BOOL GetDYNT_Status(void);
WORD GetDYNT_AGCD2N(void);
WORD GetDYNT_AGCN2D(void);
BYTE GetDYNT_AutoDly(void);
BOOL GetDYNT_NTColor(void);
BOOL GetDYNT_CVBSBurst(void);

void SetDZOOM_Status(BOOL OnOff);
void ISPM SetDZOOM_Tick(void);
void SetDZOOM_DriveTele(void);
void SetDZOOM_DriveWide(void);
void SetDZOOM_DriveTeleVar(BYTE speed);
void SetDZOOM_DriveWideVar(BYTE speed);
void SetDZOOM_DirectRatio(WORD val);
void SetDZOOM_TargetRatio(WORD val);
void SetDZOOM_RepeatKeyEnable(void);
void SetDZOOM_Window(WORD wh, WORD wv);
void SetDZOOM_Position(WORD x, WORD y);
void SetDZOOM_FactoryDefaultMenu(void);
void SetDZOOM_WriteEEPROM(void);
BOOL GetDZOOM_OnOff(void);
WORD GetDZOOM_Ratio(void);
WORD ConvDZOOM_MenuRatio(WORD ratio);
BOOL IsDZOOM_WideEND(void);
void GetDZOOM_Position(PWORD px, PWORD py);
WORD GetDZOOM_WinPosiX(void);
WORD GetDZOOM_WinPosiY(void);
void SetPIP_BoxOSDOnOff(BOOL OnOff);
void SetPIP_DispOnOff(BOOL OnOff);
void SetPIP_Size(WORD size);
void SetPIP_Position(WORD x, WORD y);
void SetPIP_Process(BOOL OnOff);
BYTE GetPIP_DispOnOff(void);
WORD GetPIP_Size(void);
void GetPIP_Position(PWORD px, PWORD py);
BOOL GetPIP_BoxOSDOnOff(void);


void SetBLC_OnOff(WORD OnOff);
void SetBLC_WinOnOff(WORD OnOff);
void SetBLC_Position(WORD x, WORD y);
void SetBLC_Size(WORD width, WORD height);
void SetBLC_Default(void);
void GetBLC_Position(PWORD px, PWORD py);
void GetBLC_Size(PWORD pwidth, PWORD pheight);
void GetBLC_SizeLimit(PWORD pw_max, PWORD ph_max);
void SetBLC_OnOffMenu(WORD nID);
void SetBLC_PosiMenu(WORD nID);
void SetBLC_SizeMenu(WORD nID);
void SetBLC_FactoryDefaultMenu(BOOL OnOff);
BOOL GetBLC_WinOnOff(void);
BOOL GetBLC_OnOff(void);

void SetHLC_Position(WORD x, WORD y);
void SetHLC_Size(WORD width, WORD height);
void SetHLC_WinOnOff(WORD OnOff);
void SetHLC_MaskOnOff(WORD OnOff);
void SetHLC_MaskLevel(WORD level);
void SetHLC_OnOff(WORD OnOff);
void SetHLC_Default(void);
void GetHLC_Position(PWORD px, PWORD py);
void GetHLC_Size(PWORD pwidth, PWORD pheight);
void GetHLC_SizeLimit(PWORD pw_max, PWORD ph_max);
void SetHLC_OnOffMenu(WORD nID);
void SetHLC_PosiMenu(WORD nID);
void SetHLC_SizeMenu(WORD nID);
void SetHLC_FactoryDefaultMenu(BOOL OnOff);
BOOL GetHLC_WinOnOff(void);
BOOL GetHLC_MaskOnOff(void);
BYTE GetHLC_MaskLevel(void);
BOOL GetHLC_OnOff(void);

void SetVS_OnOffMenu(WORD OnOff);
void SetVS_ScanAreaMenu(WORD mode);
void SetVS_DrawMenu(void);
void SetVS_PosiXMenu(WORD x);
void SetVS_PosiYMenu(WORD y);
void SetVS_SizeXMenu(WORD w);
void SetVS_SizeYMenu(WORD h);

#endif	/* __MISC_H__ */
