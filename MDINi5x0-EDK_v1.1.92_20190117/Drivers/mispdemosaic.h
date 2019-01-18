//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  DEMOSAIC.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISP_DEMOSAIC_H__
#define		__MISP_DEMOSAIC_H__

#if	defined(__USE_MDIN_i500__)		// MDIN-i500
#define __CC_BIT_EXTENSION__ 0
#define __CC_BIT_SRC__ 0
#define __CC_BIT_DEST__ 0
#else								// MDIN-i51X (i510, i540, i550)
#define __CC_BIT_EXTENSION__ 1
#define __CC_BIT_SRC__ 13
#define __CC_BIT_DEST__ 14
#endif

//#define	__CC_USE_EEPROM__
// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
typedef	enum {			
	MISP_BY_BAYER_PATTERN_2 = 0,
	MISP_BY_BAYER_PATTERN_3,
	MISP_BY_BAYER_PATTERN_0,
	MISP_BY_BAYER_PATTERN_1,		 
}	MISP_BY_BAYER_PATTERN_t;
#endif
typedef	struct
{
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	BYTE	color_corr_bypass ;
#endif
	BYTE	color_corr_ctrl_red;
	BYTE	color_corr_ctrl_green;
	BYTE	color_corr_ctrl_blue;	
	
	BYTE	color_corr_table;	
	WORD	bayer_rr_gain ;
	WORD	bayer_gr_gain ;
	WORD	bayer_br_gain ;
	
	WORD	bayer_rg_gain ;
	WORD	bayer_gg_gain ;
	WORD	bayer_bg_gain ;
	
	WORD	bayer_rb_gain ;
	WORD	bayer_gb_gain ;
	WORD	bayer_bb_gain ;
	
	BYTE	offset_r ;
	BYTE	offset_g ;
	BYTE	offset_b ;

	WORD	CC_Contrast;
	WORD	CC_Brightness;	

	WORD	save_Rgain;
	WORD	save_Bgain;
  
} DM_COLOR_CORR_PARAM, *PDM_COLOR_CORR_PARAM;


typedef	struct
{
	LONG 	ctrl;				// DM ctrl flag
	
	BOOL	input_select ;
	BOOL	dm_bypass ;
	BOOL	dm_zip_bypass ;
	
	BYTE	bayer_pixel_id;		// 0: GR / 1: RG / 2: BG / 3: GB 
	
	BYTE 	bH;				// DM horizontal boundary of input image
	BYTE	bV;				// DM vertical boundary of input image
	WORD 	active_h_size;	// DM output active horizontal size of input image
	WORD 	active_v_size;	// DM output active vertical size of input image	
	
	DM_COLOR_CORR_PARAM stDMCC ;
	
}	DM_CTRL_PARAM, *PDM_CTRL_PARAM;

#define	DMCTRL_TRIGGER_ISP			0x40000000		// ISP update
#define	DMCTRL_BYPASS_UPDATE		0x20000000		// Demosaic bypass

#define	DMCTRL_INPUT_SEL_UPDATE		0x00000001		// Demosaic input port select
#define	DMCTRL_IMAGE_SIZE_UPDATE	0x00000002		// Demosaic image size
#define	DMCTRL_DATA_ORDER_UPDATE	0x00000004		// Demosaic image size

#define	DMCTRL_CC_CNTL_STABLE		0x00000000		// Demosaic Color correction -
#define	DMCTRL_CC_CNTL_CHANGE		0x00000010		// Demosaic Color correction -
#define	DMCTRL_CC_CNTL_RESET		0x00000020		// Demosaic Color correction -
// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Device drive functions

// demosaic.c
	
void Init_DM_REGISTER_DAY(void);
void Init_DM_REGISTER_NIGHT(void);

void Init_Demosaic(void);

void DM_ProcessHandler(void); // Demosaic process handler
void CC_ProcessHandler(void); // For dynamic Color control

// -----------------------------------------------------------------------------
// API Get / Set functions

void SetDM_Input_select(BOOL val);
void SetDM_Bayer_data_order(BYTE val);
void SetDM_image_size(BYTE bH , BYTE bV, WORD valH , WORD valV);
void SetDM_Bypass(BOOL val);
void SetDM_ZipBypass(BOOL val);
void SetDM_ColorCCtrl(WORD rval, WORD gval, WORD bval);
void SetDM_ColorCCtrl_Red(WORD val);
void SetDM_ColorCCtrl_Green(WORD val);
void SetDM_ColorCCtrl_Blue(WORD val);
void SetDM_ColorCC_Brightness(WORD val);

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
void SetDM_ColorCCEn(WORD val);
#endif
BYTE GetDM_ColorCCtrl_table(void);
WORD GetDM_ColorCC_Brightness(void);
WORD GetDM_ColorCCtrl_Red(void);
WORD GetDM_ColorCCtrl_Green(void);
WORD GetDM_ColorCCtrl_Blue(void);

#endif	/* __MISP_DEMOSAIC_H__ */
