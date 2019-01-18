//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2017  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name			:  MISPCAC.H
// Description			:  This file contains typedefine for the driver files	
// Ref. Docment			: 
// Revision History 	:

#ifndef		__MISP_CAC_H__
#define		__MISP_CAC_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
#define CAC_ctrl_theta
#define CAC_counterclockwise

#define CAC_cos_pwr		6
#define CAC_sin_pwr		6
#define CAC_gain_pwr	4		//saturation power : do not over 4

typedef	struct
{
	WORD	gain;		//for saturation ctrl. unsigned 12bit. 0x400 is 1.0

	WORD	cos;		//for hue ctrl. signed 12bit. 0x400 is 1.0 0xC00 is -1.0
	WORD	sin;		//for hue ctrl. signed 12bit. 0x400 is 1.0 0xC00 is -1.0

	//Cb out = (Cb in * cos theta) + (Cr in * sin theta)
	//Cr out = (Cr in * cos theta) - (Cb in * sin theta)
}	CAC_AREA_PARAM, *PCAC_AREA_PARAM ;

typedef	struct
{
	LONG 	ctrl ;			// 	
	
	BOOL	bypass ;			// 0x440[0] fuction bypass
	BOOL	enable ;			// 0x440[1] function enable
	BOOL	disp_en ;		// 0x440[2] Display the chroma area controlled pixels. ¡®0¡¯ : no display, ¡®1¡¯ : display
	BYTE	disp_sel ;		// 0x440[5:3] Select the display for the chroma area controlled pixels. 0~5 : chroma area number, 6 : chroma value changed pixel 7 : no display
	BYTE	disp_col_sel ;	// 0x440[7:6]	0:cyan 1:magenta 2:yellow 3:mosaic

	CAC_AREA_PARAM stAREA_Magenta;	//Magenta area. Cb > 0 and Cr > 0
	CAC_AREA_PARAM stAREA_Red;		//Red area. Cb < 0, Cr > 0 and abs(Cb) < abs(Cr)
	CAC_AREA_PARAM stAREA_Yellow;		//Yellow area. Cb < 0, Cr > 0 and abs(Cb) > abs(Cr)
	CAC_AREA_PARAM stAREA_Green;		//Green area. Cb < 0 and Cr < 0
	CAC_AREA_PARAM stAREA_Cyan;		//Cyan area. Cb > 0, Cr < 0 and abs(Cb) < abs(Cr)
	CAC_AREA_PARAM stAREA_Blue;		//Blue area. Cb > 0, Cr < 0 and abs(Cb) > abs(Cr)
	
}	CAC_CTRL_PARAM, *PCAC_CTRL_PARAM ;

typedef	struct
{
	//To preserve osd menu parameter
	WORD	Magenta_gain_menu;	
	WORD	Red_gain_menu;		
	WORD	Yellow_gain_menu;		
	WORD	Green_gain_menu;		
	WORD	Cyan_gain_menu;	
	WORD	Blue_gain_menu;		

	WORD	Magenta_cos_menu;	
	WORD	Red_cos_menu;		
	WORD	Yellow_cos_menu;		
	WORD	Green_cos_menu;		
	WORD	Cyan_cos_menu;	
	WORD	Blue_cos_menu;		

	WORD	Magenta_sin_menu;	
	WORD	Red_sin_menu;		
	WORD	Yellow_sin_menu;		
	WORD	Green_sin_menu;		
	WORD	Cyan_sin_menu;	
	WORD	Blue_sin_menu;		

	WORD	Magenta_theta_menu;	
	WORD	Red_theta_menu;		
	WORD	Yellow_theta_menu;		
	WORD	Green_theta_menu;		
	WORD	Cyan_theta_menu;	
	WORD	Blue_theta_menu;			

}	CAC_APP_PARAM, *PCAC_APP_PARAM ;

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void Init_CAC(void);

void Set_CAC_Bypass(BOOL OnOff);
void Set_CAC_Enable(BOOL OnOff);

void Set_CAC_debugdisp_en(BOOL OnOff);
void Set_CAC_debugdisp_sel(BYTE sel, BYTE col);

void Set_CAC_Magenta_gain(WORD gain);
void Set_CAC_Magenta_sin(WORD sin);
void Set_CAC_Magenta_cos(WORD cos);
void Set_CAC_Red_gain(WORD gain);
void Set_CAC_Red_sin(WORD sin);
void Set_CAC_Red_cos(WORD cos);
void Set_CAC_Yellow_gain(WORD gain);
void Set_CAC_Yellow_sin(WORD sin);
void Set_CAC_Yellow_cos(WORD cos);
void Set_CAC_Green_gain(WORD gain);
void Set_CAC_Green_sin(WORD sin);
void Set_CAC_Green_cos(WORD cos);
void Set_CAC_Cyan_gain(WORD gain);
void Set_CAC_Cyan_sin(WORD sin);
void Set_CAC_Cyan_cos(WORD cos);
void Set_CAC_Blue_gain(WORD gain);
void Set_CAC_Blue_sin(WORD sin);
void Set_CAC_Blue_cos(WORD cos);

BOOL Get_CAC_Enable(void);
WORD Get_CAC_Magenta_gain(void);
WORD Get_CAC_Magenta_sin(void);
WORD Get_CAC_Magenta_cos(void);
WORD Get_CAC_Red_gain(void);
WORD Get_CAC_Red_sin(void);
WORD Get_CAC_Red_cos(void);
WORD Get_CAC_Yellow_gain(void);
WORD Get_CAC_Yellow_sin(void);
WORD Get_CAC_Yellow_cos(void);
WORD Get_CAC_Green_gain(void);
WORD Get_CAC_Green_sin(void);
WORD Get_CAC_Green_cos(void);
WORD Get_CAC_Cyan_gain(void);
WORD Get_CAC_Cyan_sin(void);
WORD Get_CAC_Cyan_cos(void);
WORD Get_CAC_Blue_gain(void);
WORD Get_CAC_Blue_sin(void);
WORD Get_CAC_Blue_cos(void);
//app
void SetApp_CACSaturation(WORD level, WORD area);
WORD GetApp_CACSaturation(WORD area);

void SetApp_CACCos(WORD level, WORD area);
WORD GetApp_CACCos(WORD area);

void SetApp_CACSin(WORD level, WORD area);
WORD GetApp_CACSin(WORD area);

void SetApp_CACTheta(WORD level, WORD area);
WORD GetApp_CACTheta(WORD area);

void SetApp_CAC_WriteEEPROM(void);
void SetApp_CAC_ReadEEPROM(void);
void SetApp_CAC_DefaultEEPROM(void);
#endif	/* __MISPCAC_H__ */

