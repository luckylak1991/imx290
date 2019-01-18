//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MISPRRGB_TOP.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550

#ifndef		__MISP_TONE_H__
#define		__MISP_TONE_H__


#define __TONE_ACTIVE_COUNTER__ 2

#define __TONE_AYC_HANDLER_ENABLE__ 1
#define __TONE_AYC_BOOT_TERMO_ENABLE__ 0  // Load Termo table on AYC gamma on boot(aux path)
#define __TONE_RGB_BOOT_THERMO_ENABLE__ 0  // Load Termo table on RGB gamma on boot(main path)

#define __BYIGAMMA_CTRL_ADDR__		0x470 // 8lv Bayer inv-gamma
#define __BYIGAMMA_COMMAND_ADDR__	0x471
#define __BYIGAMMA_DATA_ADDR__		0x472

#define __RGBGAMMA_CTRL_ADDR__		0x474 // 256lv rgb-gamma
#define __RGBGAMMA_COMMAND_ADDR__	0x475
#define __RGBGAMMA_DATA_ADDR__		0x476
#define __RGBGAMMA_USE_ADJUST__			//Use RGB gamma curve for default tunning
#define __RGBGAMMA_USE_CONTRAST__		//Activated : Use RGB gamma curve as CONTRAST , Comment : USE YC linear contrast(The same type contrast function as MDIN 3xx scaler series) 

#define __YCGAMMA_CTRL_ADDR__		0x281 // 256lv yc-gamma
#define __YCGAMMA_COMMAND_ADDR__	0x282
#define __YCGAMMA_DATA_ADDR__		0x283

#define __AYCGAMMA_CTRL_ADDR__		0x2F9 // 256lv ayc-gamma
#define __AYCGAMMA_COMMAND_ADDR__	0x2FA
#define __AYCGAMMA_DATA_ADDR__		0x2FB
// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

typedef	struct
{
	BOOL gamma_en ;		// '1': enable LUT, '0': bypass LUT

	WORD gamma_table_number ;
}	BYIGAMMA_PARAM, *PBYIGAMMA_PARAM;

typedef	struct
{
	BOOL gamma_en ;		// '1': enable LUT, '0': bypass LUT
	BYTE thermo_mode ;	
	BOOL gamma_extend;
	
	WORD channel_select ;
	WORD gamma_table_number ;
	WORD contrast;
	WORD prev_gamma_table_number ;
	
	BYTE gamma_lut; 	// about 256 LUT '0' :  '1' : 128x256 lut / '2'  : 64x256lut(default) / '3' : 32x256lut	
}	RGBGAMMA_PARAM, *PRGBGAMMA_PARAM;

typedef	struct
{
	BOOL gamma_en ;		// '1': enable LUT, '0': bypass LUT
	BOOL gamma_extend;
	
	WORD channel_select ;
	WORD Y_gamma_table_number ;
	WORD Cb_gamma_table_number ;
	WORD Cr_gamma_table_number ;
	BYTE gamma_lut; 	// about 256 LUT '0' :  '1' : 128x256 lut / '2'  : 64x256lut(default) / '3' : 32x256lut		
}	YC_GAMMA_PARAM, *PYC_GAMMA_PARAM;

typedef	struct
{
	BOOL gamma_en ;		// '1': enable LUT, '0': bypass LUT
	BOOL thermo_mode ;
	BOOL gamma_extend ;
	
	WORD channel_select ;
	WORD Y_gamma_table_number ;
	WORD Cb_gamma_table_number ;
	WORD Cr_gamma_table_number ;
	WORD pY_gamma_table_number ;
	WORD pCb_gamma_table_number ;
	WORD pCr_gamma_table_number ;	
	BYTE gamma_lut; 	// about 256 LUT '0' :  '1' : 128x256 lut / '2'  : 64x256lut(default) / '3' : 32x256lut		
}	AYC_GAMMA_PARAM, *PAYC_GAMMA_PARAM;

typedef	struct
{
	LONG 	ctrl ;			// 	
	
	BYTE	active_counter ;
		
	BYIGAMMA_PARAM stBYI_GAMMA ;
	RGBGAMMA_PARAM stRGB_GAMMA ;
	YC_GAMMA_PARAM stYC_GAMMA ;
	AYC_GAMMA_PARAM stAYC_GAMMA ;

}	TONE_CTRL_PARAM, *PTONE_CTRL_PARAM ;

#define	TONECTRL_DELAY_UPDATE			0x20000000		// ISP update
#define	TONECTRL_TRIGGER_ISP			0x40000000		// ISP update

#define	TONECTRL_GAMMA_EN_UPDATE		0x00000001		// ISP update
#define	TONECTRL_GAMMA_LUT_UPDATE		0x00000002		// ISP update
#define	TONECTRL_GAMMA_EXTEND_UPDATE	0x00000004		// ISP update

#define	TONECTRL_BYIGAMMA_EN_UPDATE		0x00000010		// ISP update
#define	TONECTRL_BYIGAMMA_LUT_UPDATE	0x00000020		// ISP update

#define	TONECTRL_YCGAMMA_EN_UPDATE		0x00000100		// ISP update
#define	TONECTRL_YCGAMMA_UPDATE			0x00000200		// ISP update
#define	TONECTRL_YCGAMMA_EXTEND_UPDATE	0x00000400		// ISP update

#define	TONECTRL_AYCGAMMA_EN_UPDATE		0x00001000		// ISP update
#define	TONECTRL_AYCGAMMA_UPDATE		0x00002000		// ISP update
#define	TONECTRL_AYCGAMMA_EXTEND_UPDATE	0x00004000		// ISP update

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------
/*
#if __USE_GAMMA_BYTEDATA__
extern ROMDATA BYTE gamma_value_table[18][32] ;
#else
extern ROMDATA BYTE gamma_value_table[18][16] ;
#endif
*/
// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Device drive functions

void Init_TONE(void) ;
void TONE_ProcessHandler( void ) ;

// -----------------------------------------------------------------------------
// API Get / Set functions

void SetTONE_RGBGAMMA_enable(BOOL val);
void SetTONE_RGBGAMMA_LUT(WORD channel_sel , WORD val) ;

void SetTONE_BYIGAMMA_enable(BOOL val);
void SetTONE_BYIGAMMA_LUT(WORD val) ;

void SetTONE_YCGAMMA_enable(BOOL val);
void SetTONE_YCGAMMA_LUT(WORD channel_sel , WORD val);

BYTE SetTONE_RGBGAMMA_val(BYTE  val);

void SetTONE_Thermo_RGBGAMMA(BYTE nID);
void SetTONE_Thermo_AYCGAMMA(BOOL OnOff);

void SetTONE_AYCGAMMA_enable(BOOL val);
void SetTONE_AYCGAMMA_LUT(WORD channel_sel , WORD val);

// -----------------------------------------------------------------------------
BOOL GetTONE_RGBGAMMA_enable(void);
BOOL GetTONE_AYCGAMMA_enable(void);
BYTE GetTONE_RGBGAMMA_val(void);
BOOL GetTONE_AYCGAMMA_thermo(void);
BYTE GetTONE_RGBGAMMA_thermo(void);

void SetTONE_RGB256LUT_Extend(BOOL OnOff);		
void SetTONE_YC256LUT_Extend(BOOL OnOff);	
void SetTONE_AYC256LUT_Extend(BOOL OnOff);	
void SetTONE_RGBLUT_Type(BYTE nID);
void SetTONE_YCLUT_Type(BYTE nID);
void SetTONE_AYCLUT_Type(BYTE nID);

#endif

#endif // MDIN-i51X (i510, i540, i550)

