//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  BAYER.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISP_BAYER_H__
#define		__MISP_BAYER_H__

#if defined(__USE_MDIN_i500__)		// MDIN-i500
#define __USE_GAMMA_BYTEDATA__ 0
#define __USE_INDIRECT_BYGAMMA__ 0
#else   							// MDIN-i5x0 (i510, i540, i550)
#define __USE_GAMMA_BYTEDATA__ 1 
#define __USE_INDIRECT_BYGAMMA__ 2
#endif

#define __USE_EXTENSION_BYGAMMA__ 0	// no use extension, use uniform table
//#define __USE_EXTENSION_BYGAMMA__ 1  // extension init & uniform table conversion
//#define __USE_EXTENSION_BYGAMMA__ 2  // extension init & extenstion table use
#define __BYGAMMA_CTRL_ADDR__		0x460
#define __BYGAMMA_COMMAND_ADDR__	0x461
#define __BYGAMMA_DATA_ADDR__		0x462

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
#if defined(__USE_MDIN_i500__)		// MDIN-i500
typedef	enum {
	MISP_DWDR_OFF = 0,				// DEFOG : off
	MISP_DWDR_AUTO ,				// DEFOG : sense
	MISP_DWDR_ON 					// DEFOG : force
}	MISP_DWDR_MODE_t;

typedef	enum {
	MISP_DEFOG_OFF = 0,	
	MISP_DEFOG_AUTO ,			
	MISP_DEFOG_ON 		
}	MISP_DEFOG_MODE_t;

typedef	enum {
	MISP_SCREEN_OFF = 0,		// DEMO : off
	MISP_SCREEN_L_ON ,			// DEMO : L on
	MISP_SCREEN_R_ON 				// DEMO : R on
}	MISP_DEFOG_SCREEN_t;

typedef	enum {
	MISP_DEFOG_STRN1 = 0,			// defog : strength-1
	MISP_DEFOG_STRN2,				// defog : strength-2
	MISP_DEFOG_STRN3,				// defog : strength-3
	MISP_DEFOG_STRN4,				// defog : strength-4
	MISP_DEFOG_STRN5,				// defog : strength-5
}	MISP_DEFOG_STRN_t;
#else 								// MDIN-i51X (i510, i540, i550)
//typedef	enum {
//	MISP_DWDR_OFF = 0,				// DEFOG : off
//	MISP_DWDR_LOW,					// DEFOG : sense
//	MISP_DWDR_MID,					// DEFOG : force
//	MISP_DWDR_HIGH
//}	MISP_DWDR_MODE_t;

typedef	enum {
	MISP_DEFOG_OFF = 0,	
	MISP_DEFOG_ON,	
	MISP_DEFOG_AUTO		
}	MISP_DEFOG_MODE_t;


typedef	enum {
	MISP_DWDR_OFF = 0,				
	MISP_DWDR_ON,					
	MISP_DWDR_AUTO
}	MISP_DWDR_MODE_t;


#if defined(__USE_DEFOG_DWDR_ONE_MENU__)	
typedef	enum {
	MISP_DEFOG_DWDR_OFF = 0,
	MISP_DEFOG_DWDR_AUTO,
	MISP_DEFOG_ON_DWDR_OFF,
	MISP_DEFOG_OFF_DWDR_ON	
}	MISP_DEFOG_DWDR_MODE_t;
#endif

//typedef	enum {
//	MISP_DWDR_MODE = 0,
//	MISP_DEFOG_MODE				
//}	MISP_DEFOG_DWDR_OPMODE_t;
#endif





typedef	enum {
	MISP_LIVEDPC_OFF = 0,	// Live DPC : off
	MISP_LIVEDPC_ON		// Live DPC : on
//	MISP_LIVEDPC_AUTO,		// Live DPC : Auto
//	MISP_LIVEDPC_LOW,		// Live DPC : Low		//2012.11.07
//	MISP_LIVEDPC_MID,		// Live DPC : Middle	//2012.11.07
//	MISP_LIVEDPC_HIGH		// Live DPC : High		//2012.11.07
}	MISP_LIVEDPC_MODE_t;

typedef	enum {
	MISP_LUT_256x256 = 0,			// defog : strength-1
	MISP_LUT_128x256,				// defog : strength-2
	MISP_LUT_64x256,				// defog : strength-3
	MISP_LUT_32x256				// defog : strength-4
}	MISP_LUT_STYLE_t;

typedef	enum {
	MISP_GAMMA_OFF = 0,			// GAMMA bypass = off
	MISP_GAMMA_095 ,				// 1
	MISP_GAMMA_090 ,				// 2
	MISP_GAMMA_085 ,				// 3
	MISP_GAMMA_080 , 				// 4
	MISP_GAMMA_075 ,				// 5
	MISP_GAMMA_070 ,				// 6
	MISP_GAMMA_065 ,				// 7
	MISP_GAMMA_060 ,				// 8
	MISP_GAMMA_055 ,				// 9
	MISP_GAMMA_050 ,				// 10
	MISP_GAMMA_045 ,				// 11
	MISP_GAMMA_040 ,				// 12(sometimes mapped thremo mode)
	MISP_GAMMA_CUSTOM1 ,			// 13
	MISP_GAMMA_CUSTOM2 ,			// 14
	MISP_GAMMA_CUSTOM3 			// 15	
}	MISP_Gamma_t; 

typedef	struct
{
	BOOL lens_corr_en ;
	
	BYTE outer_rad_rel ;
	BYTE inner_rad_rel ;
	BYTE lens_corr_intensity ;
} BY_LENS_CORR_PARAM, *PDM_LENS_CORR_PARAM;

typedef	struct
{
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)		
	BYTE defog_dwdr_mode;
#endif	
	BYTE defog_mode;
	BYTE defog_strength;
	BOOL defog_active;

	BYTE defog_threshold;
	BYTE defog_auto_level;
	BYTE defog_ovr_weight;
	BYTE defog_weight;
	
	BYTE dwdr_mode;
	BOOL dwdr_active;
	BYTE dwdr_weight;
		
	BYTE set_mode;
	BYTE op_mode;
	
	BYTE dwdr_strength_cal;
	BYTE defog_strength_cal;
	BYTE dwdr_saturation_cal;
	BYTE defog_saturation_cal;

	BYTE mavg_dwdr_strength_cal;
	BYTE mavg_defog_strength_cal;
	BYTE mavg_dwdr_saturation_cal;
	BYTE mavg_defog_saturation_cal;
	
	BYTE dwdr_strength;
	BYTE dwdr_saturation;
	BYTE dwdr_local_ratio;
	BYTE dwdr_saturation_sync;
	BYTE dwdr_auto_level;
	
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	BYTE defog_skya_ratio ;
	BYTE defog_skya_offset ;

	BYTE dwdr_natural_gamma;
	BYTE dwdr_on_gamma;
	BYTE dwdr_off_gamma;
	
	BYTE dwdr_natural_aetarget;
	BYTE dwdr_on_aetarget;
	BYTE dwdr_off_aetarget;	
	BYTE defog_intensity;
#endif
} BY_DEFOG_DWDR_CORR_PARAM, *PBY_DEFOG_DWDR_CORR_PARAM;

typedef	struct
{	
	BOOL gamma_en ;

	BYTE gamma_r ;
	BYTE gamma_g ;
	BYTE gamma_b ;
	BYTE gamma_default ;	
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	BOOL gamma_mode ; // '0' : Bayer gamma / '1' : RGB gamma
	BYTE gamma_8tap;	// for nr and demosic - 8tap f&inv gamma table

	BYTE gamma_extend; // about 256 LUT '0' : Uniform / '1' : Extend Fine to Course / '2' Course to fine
	BYTE gamma_lut; 	// about 256 LUT '0' :  '1' : 128x256 lut / '2'  : 64x256lut(default) / '3' : 32x256lut
#endif
} BY_GAMMA_CORR_PARAM, *PBY_GAMMA_CORR_PARAM;

typedef	struct
{
	LONG 	ctrl;				// Bayer ctrl flag
	
	BOOL	input_select ;
	BOOL	bayer_bypass ;
	BOOL	bayer_5m_bypass ;
	
	BYTE	bayer_pixel_id;		// 0: GR / 1: RG / 2: BG / 3: GB 
	BYTE	defog_screen_ctrl ;
	BYTE	gamma_screen_ctrl ;
	
	WORD 	bH;				// Bayer horizontal boundary of input image
	WORD	bV;				// Bayer vertical boundary of input image
	WORD 	active_h_size;	// Bayer output active horizontal size of input image
	WORD 	active_v_size;	// Bayer output active vertical size of input image	
	
	BY_LENS_CORR_PARAM stLENSCORR ;
	BY_DEFOG_DWDR_CORR_PARAM stDEFOGDWDR ;
	BY_GAMMA_CORR_PARAM stGAMMA ;
	
}	BY_CTRL_PARAM, *PBY_CTRL_PARAM;

		
typedef struct
{
	BYTE 	index;
	BYTE 	xl;
	BYTE 	yn;
	BYTE 	yc;
	BYTE 	yd;
	BYTE 	xa;
	BYTE 	xb;
} BY_DWDR_STRENGTH_STEP, *PBY_DWDR_STRENGTH_STEP;

typedef struct
{
	BYTE 	index;
	BYTE 	ovr2;
} BY_DWDR_SATURATION_STEP, *PBY_DWDR_SATURATION_STEP;


typedef struct
{
	BYTE	center_offset;
	BYTE	low_blend;
	BYTE 	ovr2;
	BYTE 	avg_pix;
	BYTE 	hh;
	BYTE 	hl;
	BYTE 	lh;
	BYTE 	ll;
} BY_DEFOG_STRENGTH_STEP, *PBY_DEFOG_STRENGTH_STEP;

typedef struct
{
	BYTE 	index;
	BYTE 	high_blend;
} BY_DEFOG_SATURATION_STEP, *PBY_DEFOG_SATURATION_STEP;
	
typedef	enum {
	SET_DWDR_DEFOG_ALL_OFF = 0,
	SET_DWDR_FORCE,
	SET_DEFOG_FORCE,
	SET_DWDR_AUTO,
	SET_DEFOG_AUTO,
	SET_DWDR_DEFOG_FULL_AUTO
}	DWDR_DEFOG_SET_MODE_t;

typedef	enum {
	OP_DWDR_DEFOG_ALL_OFF = 0,
	OP_DWDR_MODE,
	OP_DEFOG_MODE
}	DWDR_DEFOG_OP_MODE_t;


#define GDWDR_THRESHOLD 	80

#define	BYCTRL_TRIGGER_ISP			0x40000000		// ISP update
#define	BYCTRL_BYPASS_UPDATE		0x20000000		// Bayer bypass

#define	BYCTRL_INPUT_SEL_UPDATE		0x00000001		// Bayer input port select
#define	BYCTRL_IMAGE_SIZE_UPDATE	0x00000002		// Bayer image size
#define	BYCTRL_DATA_ORDER_UPDATE		0x00000004		// Bayer data order
#define	BYCTRL_LENS_CORR_UPDATE		0x00000010		// Bayer lens corr
#define	BYCTRL_DWDR_UPDATE			0x00000100		// Bayer dwdr
#define	BYCTRL_DWDR_STRENGTH_UPDATE	0x00000200		// Bayer dwdr strength
#define	BYCTRL_DWDR_SAT_UPDATE		0x00000400		// Bayer dwdr saturation
#define	BYCTRL_DWDR_LOCAL_RATIO_UPDATE	0x00000800		// Bayer dwdr local
#define	BYCTRL_DWDR_SAT_SYNC_UPDATE	0x00001000		// Bayer dwdr local
	
#define	BYCTRL_DEFOG_MODE_UPDATE		0x00010000		// Bayer defog mode
#define	BYCTRL_DEFOG_STRENGTH_UPDATE	0x00020000		// Bayer defog sensitivity
#if defined(__USE_MDIN_i500__)		// MDIN-i500
#define	BYCTRL_DEFOG_SCREEN_UPDATE		0x00040000		// Bayer defog screen control
#else
#define	BYCTRL_DEFOG_THRESHOLD_UPDATE	0x00040000		// Bayer defog sensitivity
#endif


#define	BYCTRL_BYGAMMA_LUT_UPDATE		0x00100000		// Bayer Gamma control
#define	BYCTRL_BYGAMMA_EXTEND_UPDATE		0x00200000		// Bayer Gamma control
#define	BYCTRL_BYGAMMA_GAIN_UPDATE		0x00300000		// Bayer Gamma control		
#define	BYCTRL_BYGAMMA_EN_UPDATE			0x00400000		// Bayer Gamma control		


// -----------------------------------------------------------------------------
// External Variables declaration
// ------------------------------------------------------------
extern BYTE global_histo64[64];
// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Device drive functions

// demosaic.c

void Init_BayerFR(void);

void BY_ProcessHandler(void); // Demosaic process handler

// -----------------------------------------------------------------------------
// API Get / Set functions
void SetBY_Bayer_data_order(BYTE val);
void SetBY_image_size(BYTE bH , BYTE bV, WORD valH , WORD valV);
void SetBY_Input_select(BOOL val);
void SetBY_Bypass(BOOL val);

void SetBY_Lens_corr_en(BOOL val);
void SetBY_Lens_corr_value(BYTE inner_rad, BYTE outer_rad, BYTE intensity ) ;
void SetBY_DWDR_mode(BYTE val);
void SetBY_DWDRStrength(WORD val);
void SetBY_DWDRSaturation(WORD val);
void SetBY_DWDRLocalRatio(WORD val);
void SetBY_DWDRSatSync(WORD val);
void SetBY_DWDRAutoLevel(WORD val);
void SetBY_DefogDWDRMode(BYTE val);
void SetBY_DefogMode(BYTE val);
void SetBY_DefogStrength(WORD val);
void SetBY_DefogThreshold(WORD val);
void SetBY_DefogAutoLevel(WORD val);
void SetBY_GammaValue(BYTE val);
void SetBY_GammaGainValue(void);
void SetBY_5MBypass(BOOL val);
#if defined(__USE_MDIN_i500__)		// MDIN-i500
void SetBY_DefogBright(WORD val);
void SetBY_DefogDispSel(WORD val);
void SetBY_DWDR_gamma(BYTE mode, BYTE val);
void SetBY_DWDR_AEtarget(BYTE mode, BYTE val);
#endif
	
BOOL GetBY_DefogActive(void);
BOOL GetBY_DWDRActive(void);
BYTE GetBY_DefogDWDRMode(void);
BYTE GetBY_DefogMode(void);
BYTE GetBY_DefogStrength(void);
BYTE GetBY_DefogBright(void);
BYTE GetBY_DefogDispSel(void);
BYTE GetBY_DefogThreshold(void);
BYTE GetBY_DefogAutoLevel(void);
BYTE GetBY_dwdr_mode(void);
BYTE GetBY_DWDRStrength(void);
BYTE GetBY_DWDRSaturation(void);
BYTE GetBY_DWDRLocalRatio(void);
BYTE GetBY_DWDRSatSync(void);
BYTE GetBY_DWDRAutoLevel(void);

#if defined(__USE_MDIN_i500__)		// MDIN-i500
BYTE GetBY_DWDR_gamma(BYTE mode) ;
BYTE GetBY_DWDR_AEtarget(BYTE mode) ;
#endif

//--------------------------------------------------------------------------------------------------------------------------
BYTE GetBY_gamma_value(void);
void SetBY_gamma_value(BYTE val);
BOOL GetBY_gamma_enable(void);
void SetBY_gamma_enable(BOOL val);
BOOL GetBY_Lens_corr_en(void);
BYTE GetBY_GammaValue(void);

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
BOOL GetBY_gamma_mode(void);

void SetBY_gamma_8tap(BYTE nID);
BYTE GetBY_gamma_8tap(void);

void SetBY_BY256LUT_Extend(BOOL OnOff);
void SetBY_BYLUT_Type(BYTE nID);
#endif
// -----------------------------------------------------------------------------

#endif	/* __MISP_BAYER_H__ */
