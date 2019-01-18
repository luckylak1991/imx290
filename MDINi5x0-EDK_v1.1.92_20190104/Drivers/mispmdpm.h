//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2016  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MISPMDPM.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISPMDPM_H__
#define		__MISPMDPM_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

// pm
typedef	struct
{
	WORD	x;			// Horizontal position of corner point
	WORD	y;			// Vertical position of corner point
}	PM_POLY_POSI, *PPM_POLY_POSI;

typedef	struct	   
{
	WORD	w;			// horizontal size of area
	WORD	h;			// vertical size of area
	SHORT	x;			// horizontal center position of area(virtual : -6144 ~ +6144)
	SHORT	y;			// vertical center position of area(virtual : -3072 ~ +3072)
	WORD	zoom;		// zoom ratio
	BYTE	disp;		// Display on/off
	BYTE	color;		// Setting Color (for Privacy mask)
	
}	PM_RECT_INFO, *PPM_RECT_INFO;

typedef	struct
{
	WORD	ulx, uly;		//top left pos
	WORD	urx, ury;		//top right pos
	WORD	dlx, dly;		//bottom left pos
	WORD	drx, dry;		//bottom right pos
	BYTE	disp;
	BYTE	color;
	WORD	dummy;
	
}	PM_POLY_INFO, *PPM_POLY_INFO;

typedef	struct
{
	LONG 	ctrl;					// control flag
	PM_RECT_INFO	rect[8];		// pm rectangle info
	PM_POLY_INFO	poly[8];		// pm polygon info
	SHORT	cam_pan;				// pm horizontal origin position(pan)
	SHORT	cam_tilt;				// pm vertical origin position(tilt)
	WORD	cam_zoom; 				// pm camera zoom position
	WORD	af_zposi; 				// pm af zoom position backup
	BYTE	gray_level[6];			// pm gray color level 1~6
	BYTE	mosaic_size;			// pm mosaic block size : (X+1)*8
	BYTE	dummy; 					// dummy
	WORD	mask_no;				// pm mask no for process([7:0]:rect 1~8, [15:8]:poly 1~8)
	WORD	noninterlock_mask;		// non-interlock mask no.
	
}	PM_CTRL_PARAM, *PPM_CTRL_PARAM;


// md
typedef	struct
{
	BYTE	w; 						// md area width
	BYTE	h; 						// md area height
	BYTE	lx; 					// md area position(start_x)
	BYTE	ly; 					// md area position(start_y)
	BYTE	en; 					// md area enable/disable
	BYTE	disp; 					// md area window display on/off
	WORD	dummy;

}	MD_AREA_INFO, *PMD_AREA_INFO;

typedef	struct
{
	LONG 			ctrl; 			// control flag
	MD_AREA_INFO 	area[4]; 		// md area info.
	WORD 			video_size_h; 	// md input video h-size
	WORD 			video_size_v; 	// md input video v-size
	WORD 			thres; 			// md threshold
	BYTE			block_size; 	// md block size(unit:pixel)
	BYTE 			dummy;

}	MD_CTRL_PARAM, *PMD_CTRL_PARAM;



// pm control parameter
#define 	PMCTRL_TRIGGER				0x80000000		// pm process handler trigger
#define		PMCTRL_DISABLE				0x00000000		// pm disable
#define		PMCTRL_ENABLE				0x00000001		// pm enable
#define		PMCTRL_MODE_RECT			0x00000000		// pm rectangle mode
#define		PMCTRL_MODE_POLY			0x00000002		// pm polygon mode

#define		PMCTRL_MASK_MAX				8				// pm mask max no.


// md control parameter
#define 	MDCTRL_TRIGGER				0x80000000		// md process handler trigger
#define		MDCTRL_MD_ON				0x00000001		// md on flag
#define		MDCTRL_MD_OFF				0x00000000		// md off flag
#define		MDCTRL_MOTIONVIEW_ON		0x00000002		// md motion view on flag
#define		MDCTRL_MOTIONVIEW_OFF		0x00000000		// md motion view off flag


// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// pm
void Init_MDPM(void);
void PM_ProcessHandler(void);

void SetPM_OnOff(BYTE OnOff);
void SetPM_Type(BYTE type);
void SetPM_RectOnOff(BYTE no, BYTE OnOff);
void SetPM_RectColor(BYTE no, BYTE color);
void SetPM_RectSizeNew(BYTE no, WORD width, WORD height);
void SetPM_RectSizeModify(BYTE no, WORD width, WORD height);
void SetPM_RectSizeDirect(BYTE no, WORD width, WORD height);
void SetPM_RectPosiDirect(BYTE no, WORD lx, WORD ly);
void SetPM_PolyOnOff(BYTE no, BYTE OnOff);
void SetPM_PolyColor(BYTE no, BYTE color);
void SetPM_PolyPosi(BYTE no, BYTE edge, WORD x, WORD y);
void SetPM_CamPanTIlt(WORD pan, WORD tilt);
void SetPM_MaskPTZ(BYTE no, WORD pan, WORD tilt, WORD zoom);
void SetPM_NonInterlockMask(BYTE no, WORD w, WORD h, WORD x, WORD y);
void SetPM_OnOffMenu(WORD nID);
void SetPM_TypeMenu(WORD nID);
void SetPM_MaskNoMenu(WORD nID);
void SetPM_MaskOnOffMenu(WORD nID);
void SetPM_ColorMenu(WORD nID);
void SetPM_RectPosiXMenu(WORD nID);
void SetPM_RectPosiYMenu(WORD nID);
void SetPM_RectSizeWMenu(WORD nID);
void SetPM_RectSizeHMenu(WORD nID);
void SetPM_PolyEdgeSelMenu(WORD nID);
void SetPM_PolyXposiMenu(WORD nID);
void SetPM_PolyYposiMenu(WORD nID);
void SetPM_FactoryDefaultMenu(void);
BYTE GetPM_OnOff(void);
BYTE GetPM_Type(void);
BYTE GetPM_RectOnOff(BYTE no);
BYTE GetPM_RectColor(BYTE no);
void GetPM_RectSize(BYTE no, PWORD pw, PWORD ph);
void GetPM_RectPosi(BYTE no, PWORD px, PWORD py);
BYTE GetPM_PolyOnOff(BYTE no);
BYTE GetPM_PolyColor(BYTE no);
void GetPM_PolyPosi(BYTE no, BYTE edge, PWORD px, PWORD py);
BYTE GetPM_MaskOnOffMenu(void);
BYTE GetPM_ColorMenu(void);
BYTE GetPM_PosiXMenu(void);
BYTE GetPM_PosiYMenu(void);
BYTE GetPM_WidthMenu(void);
BYTE GetPM_HeightMenu(void);


// md
void SetMD_OnOff(WORD OnOff);
void SetMD_MotionView(WORD OnOff);
void SetMD_BlockSize(WORD size);
void SetMD_Threshold(WORD val);
void SetMD_VideoSize(WORD md_h_size, WORD md_v_size);
void SetMD_AreaEn(BYTE no, WORD OnOff);
void SetMD_AreaDisp(BYTE no, WORD OnOff);
void SetMD_AreaSizePosi(BYTE no, BYTE w, BYTE h, BYTE lx, BYTE ly);
void SetMD_OnOffMenu(WORD nID);
void SetMD_AreaSelMenu(WORD nID);
void SetMD_AreaOnOffMenu(WORD nID);
void SetMD_AreaDispMenu(WORD nID);
void SetMD_PosiMenu(WORD nID);
void SetMD_SizeMenu(WORD nID);
void SetMD_FactoryDefaultMenu(void);
BYTE GetMD_OnOff(void);
BYTE GetMD_AreaEn(BYTE no);
BYTE GetMD_AreaDisp(BYTE no);
BYTE GetMD_AreaWidth(BYTE no);
BYTE GetMD_AreaHeight(BYTE no);
BYTE GetMD_AreaPosiX(BYTE no);
BYTE GetMD_AreaPosiY(BYTE no);
BYTE GetMD_SizeMaxH(void);
BYTE GetMD_SizeMaxV(void);
WORD GetMD_MotionView(void);
WORD GetMD_Threshold(void);


#endif	/* __MISPMDPM_H__ */
