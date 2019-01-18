//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  NR.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISP_NR_H__
#define		__MISP_NR_H__

// __NR_DEBUG_PRINT__ [ 0: off / 1: frame noise / 2: PEL ]
#define __NR_DEBUG_PRINT__ 0

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__CMOS_IF_H__
#include	"cmos_if.h"		// for sensor dependent option
#endif

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
#if	defined(__USE_MDIN_i500__)			// MDIN-i500
#define __NR_REV__ 0					// Do not change the value
#else									// MDIN-i51X (i510, i540, i550)
#define __NR_REV__ 1					// Do not change the value
#endif

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
#define __NR_ZOOM_LENS_SETUP__
#endif

#ifdef __NR_YC_PROCESS__
#define __NR_YC_IMAGE_SETUP__
#endif

#if defined(__PANORAMA_SETUP__) || defined(__USE_CMOS_IMX334__) || defined(__USE_CMOS_IMX274__) ||defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX326__) || defined(__USE_CMOS_IMX253__) 
#define __NR_IDMHIST_FUNC__ 0 // 0 : Disable IDM hist function 1 : Enable IDM hist function
#else
#define __NR_IDMHIST_FUNC__ 1 // 0 : Disable IDM hist function 1 : Enable IDM hist function
#endif

// Low light still mode : Default 0
#define __NR_LL_STILL_FUNC__ 0 // 0 : Disable still mode 1 : Enable still mode 

// L2DNR control : Default 0
#define __NR_L2DNR_CONTROL__ 0 // 0: Remove L2DNR control 1: Contain L2DNR control

// Slut gen function : Default 0
#define __NR_SLUT_GENFUNC__ 0 // 0: Remove slut gen util 1: Contain slut gen util
#define __NR_SLUT_SIGMA_VALUE__ 3.5f // Must be float type 


#if defined (__MDIN_i5XX_FPGA__) // FPGA
	#if defined  (__USE_IMX178_1080P30_MODE__) || defined (__USE_IMX178_1080P60_MODE__)
		#define __NR_10BIT_SETUP__ 0
	#else
		#define __NR_10BIT_SETUP__ 1
	#endif
#else
	#define __NR_10BIT_SETUP__ 1
	// #define __NR_10BIT_SETUP__ 0
#endif


#if defined(__USE_4K60_MD1__)
#define __NR_SPLIT__ 1 // NR split mode master
#elif defined(__USE_4K60_MD2__)
#define __NR_SPLIT__ 2 // NR split mode slave
#else
#define __NR_SPLIT__ 0 // NR normal  mode
#endif

// -----------------------------------------------------------------------------

#if __NR_SPLIT__==0

#define __NR_STRATEGY_LEVEL__  6
// #define __NR_STRATEGY_LEVEL__  2	 // 
// 0 : M filter off 
// 1 : M filter low 
// 2 : M filter mid
// 3 : M filter high 
// 4 : M filter off + fast serial
// 5 : M filter low + fast serial
// 6 : M filter mid + fast serial
// 7 : M filter high + fast serial

#define __NR_INITIAL_APERTURE__  2
// 0 : least aperture : most motion artifact
// 1 : less aperture : more motion artifact
// 2 : Normal aperture : normal motion artifact
// 3 : more aperture : less motion artifact
// 4 : most aperture : least motion artifact

#define __NR_ACTIVE_COUNTER__ 7
#define __NR_AUTO_PERIOD__ 11

#else // #if __NR_SPLIT__==1||#if __NR_SPLIT__==2

#define __NR_STRATEGY_LEVEL__  4
// #define __NR_STRATEGY_LEVEL__  2	 // 
// 0 : M filter off 
// 1 : M filter low 
// 2 : M filter mid
// 3 : M filter high 
// 4 : M filter off + fast serial
// 5 : M filter low + fast serial
// 6 : M filter mid + fast serial
// 7 : M filter high + fast serial

#define __NR_INITIAL_APERTURE__  2
// 0 : least aperture : most motion artifact
// 1 : less aperture : more motion artifact
// 2 : Normal aperture : normal motion artifact
// 3 : more aperture : less motion artifact
// 4 : most aperture : least motion artifact

#define __NR_ACTIVE_COUNTER__ 14
#define __NR_AUTO_PERIOD__ 24

#endif // #if __NR_SPLIT__==0

#define __NR_AUTO2DNR_TIMER__ (__NR_AUTO_PERIOD__-__NR_AUTO_PERIOD__*3/4)
#define __NR_AUTOPDM_TIMER__ (__NR_AUTO_PERIOD__-__NR_AUTO_PERIOD__*2/4)
#define __NR_AUTOIDM_TIMER__ (__NR_AUTO_PERIOD__-__NR_AUTO_PERIOD__/4)
#define __NR_AUTOSRM_TIMER__ (__NR_AUTO_PERIOD__-__NR_AUTO_PERIOD__/4)

#if defined(__USE_CMOS_IMX226__)||defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__)
	// #define __NR_AUTO3DNR_BOUND_0__ 4
	// #define __NR_AUTO3DNR_BOUND_1__ 5
	// #define __NR_AUTO3DNR_BOUND_2__ 7
	// #define __NR_AUTO3DNR_BOUND_3__ 10
	// #define __NR_AUTO3DNR_BOUND_4__ 13
	// #define __NR_AUTO3DNR_BOUND_5__ 16

	#define __NR_AUTO3DNR_BOUND_0__ 3
	#define __NR_AUTO3DNR_BOUND_1__ 5
	#define __NR_AUTO3DNR_BOUND_2__ 7
	#define __NR_AUTO3DNR_BOUND_3__ 9
	#define __NR_AUTO3DNR_BOUND_4__ 11
	#define __NR_AUTO3DNR_BOUND_5__ 13
#elif defined(__USE_CMOS_IMX185__)
	#define __NR_AUTO3DNR_BOUND_0__ 3
	#define __NR_AUTO3DNR_BOUND_1__ 5
	#define __NR_AUTO3DNR_BOUND_2__ 7
	#define __NR_AUTO3DNR_BOUND_3__ 8
	#define __NR_AUTO3DNR_BOUND_4__ 10
	#define __NR_AUTO3DNR_BOUND_5__ 12
#else
	#define __NR_AUTO3DNR_BOUND_0__ 3
	#define __NR_AUTO3DNR_BOUND_1__ 5
	#define __NR_AUTO3DNR_BOUND_2__ 7
	#define __NR_AUTO3DNR_BOUND_3__ 10
	#define __NR_AUTO3DNR_BOUND_4__ 13
	#define __NR_AUTO3DNR_BOUND_5__ 16
#endif

#if defined(__USE_CMOS_IMX226__)||defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__) ||defined(__USE_MDIN_i500__)
	#define __NR_AUTO3DNR_VALUE_0__ 7
	#define __NR_AUTO3DNR_VALUE_1__ 9
	#define __NR_AUTO3DNR_VALUE_2__ 11
	#define __NR_AUTO3DNR_VALUE_3__ 13
	#define __NR_AUTO3DNR_VALUE_4__ 15
	#define __NR_AUTO3DNR_VALUE_5__ 18
	#define __NR_AUTO3DNR_VALUE_6__ 21
	#define __NR_AUTO3DNR_VALUE_7__ 25
	#define __NR_AUTO3DNR_VALUE_8__ 29
	#define __NR_AUTO3DNR_VALUE_9__ 33
	#define __NR_AUTO3DNR_VALUE_A__ 37	
#else 											// MDIN-i5x0 (i510, i540, i550)
	#define __NR_AUTO3DNR_VALUE_0__ 7
	#define __NR_AUTO3DNR_VALUE_1__ 9
	#define __NR_AUTO3DNR_VALUE_2__ 11
	#define __NR_AUTO3DNR_VALUE_3__ 13
	#define __NR_AUTO3DNR_VALUE_4__ 15
	#define __NR_AUTO3DNR_VALUE_5__ 17
	#define __NR_AUTO3DNR_VALUE_6__ 20
	#define __NR_AUTO3DNR_VALUE_7__ 23
	#define __NR_AUTO3DNR_VALUE_8__ 26
	#define __NR_AUTO3DNR_VALUE_9__ 30
	#define __NR_AUTO3DNR_VALUE_A__ 34		
#endif
// -----------------------------------------------------------------------------
#if defined (__USE_CMOS_IMX290__) || defined (__USE_CMOS_IMX385__) || defined (__USE_CMOS_IMX327__) || defined(__USE_CMOS_IMX334__)
	#define __NR_YC_EDGE_LEVEL__ 43 
	#define __NR_YC_EDGE_LEVEL_0__ 11 
	#define __NR_YC_EDGE_LEVEL_1__ 15 
	#define __NR_YC_EDGE_LEVEL_2__ 19 
	#define __NR_YC_EDGE_LEVEL_3__ 24 
	#define __NR_YC_EDGE_LEVEL_4__ 29 
	#define __NR_YC_EDGE_LEVEL_5__ 35 
	#define __NR_YC_EDGE_LEVEL_6__ 41 

	#define __NR_BY_EDGE_LEVEL__ 64 
	#define __NR_BY_EDGE_LEVEL_0__ 11 
	#define __NR_BY_EDGE_LEVEL_1__ 14 
	#define __NR_BY_EDGE_LEVEL_2__ 19 
	#define __NR_BY_EDGE_LEVEL_3__ 25 
	#define __NR_BY_EDGE_LEVEL_4__ 35 
	#define __NR_BY_EDGE_LEVEL_5__ 54 
	#define __NR_BY_EDGE_LEVEL_6__ 61 
#elif defined (__USE_CMOS_IMX185__)
	#define __NR_YC_EDGE_LEVEL__ 17 
	#define __NR_YC_EDGE_LEVEL_0__ 11 
	#define __NR_YC_EDGE_LEVEL_1__ 12
	#define __NR_YC_EDGE_LEVEL_2__ 13 
	#define __NR_YC_EDGE_LEVEL_3__ 14 
	#define __NR_YC_EDGE_LEVEL_4__ 15 
	#define __NR_YC_EDGE_LEVEL_5__ 16 
	#define __NR_YC_EDGE_LEVEL_6__ 17 

	#define __NR_BY_EDGE_LEVEL__ 24 
	#define __NR_BY_EDGE_LEVEL_0__ 10 
	#define __NR_BY_EDGE_LEVEL_1__ 11 
	#define __NR_BY_EDGE_LEVEL_2__ 13 
	#define __NR_BY_EDGE_LEVEL_3__ 15 
	#define __NR_BY_EDGE_LEVEL_4__ 17 
	#define __NR_BY_EDGE_LEVEL_5__ 20 
	#define __NR_BY_EDGE_LEVEL_6__ 22 
#elif defined (__USE_CMOS_IMX226__)||defined (__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__)
	#define __NR_YC_EDGE_LEVEL__ 30 
	#define __NR_YC_EDGE_LEVEL_0__ 12 
	#define __NR_YC_EDGE_LEVEL_1__ 15 
	#define __NR_YC_EDGE_LEVEL_2__ 18 
	#define __NR_YC_EDGE_LEVEL_3__ 21 
	#define __NR_YC_EDGE_LEVEL_4__ 24 
	#define __NR_YC_EDGE_LEVEL_5__ 27 
	#define __NR_YC_EDGE_LEVEL_6__ 30 

	#define __NR_BY_EDGE_LEVEL__ 18 
	#define __NR_BY_EDGE_LEVEL_0__ 10 
	#define __NR_BY_EDGE_LEVEL_1__ 10 
	#define __NR_BY_EDGE_LEVEL_2__ 10 
	#define __NR_BY_EDGE_LEVEL_3__ 11 
	#define __NR_BY_EDGE_LEVEL_4__ 13 
	#define __NR_BY_EDGE_LEVEL_5__ 14 
	#define __NR_BY_EDGE_LEVEL_6__ 16 
#else
	#define __NR_YC_EDGE_LEVEL__ 30 
	#define __NR_YC_EDGE_LEVEL_0__ 12 
	#define __NR_YC_EDGE_LEVEL_1__ 15 
	#define __NR_YC_EDGE_LEVEL_2__ 18 
	#define __NR_YC_EDGE_LEVEL_3__ 21 
	#define __NR_YC_EDGE_LEVEL_4__ 24 
	#define __NR_YC_EDGE_LEVEL_5__ 27 
	#define __NR_YC_EDGE_LEVEL_6__ 30 

	#define __NR_BY_EDGE_LEVEL__ 34 
	#define __NR_BY_EDGE_LEVEL_0__ 12 
	#define __NR_BY_EDGE_LEVEL_1__ 14 
	#define __NR_BY_EDGE_LEVEL_2__ 15 
	#define __NR_BY_EDGE_LEVEL_3__ 18 
	#define __NR_BY_EDGE_LEVEL_4__ 21 
	#define __NR_BY_EDGE_LEVEL_5__ 25 
	#define __NR_BY_EDGE_LEVEL_6__ 29 
#endif
// -----------------------------------------------------------------------------

typedef	enum {
	MISP_NR_BAYER_IMAGE = 0,// NR : bayer
	MISP_NR_YC_IMAGE ,		// NR : y/c
}	MISP_NR_IMAGE_TYPE_t;

typedef	enum {
	MISP_NR_OFF = 0,		// NR : off
	MISP_NR_2D ,			// NR : 2D
	MISP_NR_3D ,			// NR : 3D
	MISP_NR_2D3D ,			// NR : 2D/3D
}	MISP_NR_MODE_t;

typedef	enum {
	MISP_NR3D_AUTO = 0,		// NR3D : auto
	MISP_NR3D_LOW,			// NR3D : low
	MISP_NR3D_MID,			// NR3D : mid
	MISP_NR3D_HIGH,			// NR3D : high
}	MISP_NR3D_MODE_t;

typedef	enum {
	MISP_NRLOWLIGHT_OFF = 0,// NR low light mode : off
	MISP_NRLOWLIGHT_ON,		// NR low light mode : on
}	MISP_NRLOWLIGHT_MODE_t;

typedef	enum {
	MISP_NRDEMO_OFF = 0,	// NR demo mode : off
	MISP_NRDEMO_LEFT_ON,	// NR demo mode : left on
	MISP_NRDEMO_RIGHT_ON,	// NR demo mode : right on
	MISP_NRDEMO_IDM_L,		// NR demo mode 
	MISP_NRDEMO_IDM_H,		// NR demo mode 
	MISP_NRDEMO_PDM_U,		// NR demo mode 
	MISP_NRDEMO_PDM,		// NR demo mode 
	MISP_NRDEMO_SRM,		// NR demo mode 
}	MISP_NRDEMO_MODE_t;

typedef	struct
{
	BYTE 	bH;				// NR horizontal boundary of input image
	BYTE	bV;				// NR vertical boundary of input image
	WORD 	active_h_size;	// NR active horizontal size of input image
	WORD 	active_v_size;	// NR active vertical size of input image
	
	BYTE	active_counter ;
	BOOL	y_linemem_en ;
	BOOL	c_linemem_en ;
	WORD	dummy; 
}	NR_LINEMEM_PARAM, *PNR_LINEMEM_PARAM;

typedef	struct
{
	BOOL	mcu_mode_ctrl ;				// NR MCU drive mode ctrl
	BOOL	mcu_force_lowlight_mode ;	// NR MCU drive force lowlight
	BOOL	mcu_force_still_mode ;		// NR MCU drive force still
	BYTE 	mcu_still_intensity ;		// NR MCU drive force still intensity
	WORD 	mcu_drive_duration ;		// NR MCU drive drive duration
	WORD	dummy; 
}	NR_MCU_CTRL_PARAM, *PNR_MCU_CTRL_PARAM;

typedef	struct
{
	BOOL	idm_en ;						// Intensity difference motion detection enable
	BOOL	idm_auto ;						// Intensity difference motion detection enable	
	
	BYTE	id_t_ratio ;
	BYTE	id_y_ratio ;
	BYTE	id_b_ratio ;
	
	BOOL	id_dy_noise_limit ;
	BYTE	id_noise_source ;
	BYTE	id_k_value;						//
	BYTE	id_noise_limit;					//
	BYTE	id_thres_3d_nr;					//
	BYTE	id_thres_3d_2d_nr;				//
	BYTE	id_thres_pre_3d_cu_2d_nr;		//
	BYTE	id_thres_pre_3d_2d_cu_2d_nr;	//
	
	BOOL	id_ll_dy_noise_limit ;
	BYTE	id_ll_noise_limit;				//
	BYTE	id_ll_thres_3d_nr;				//
	BYTE	id_ll_thres_3d_2d_nr;			//
	BYTE	id_ll_thres_pre_3d_cu_2d_nr;	//
	BYTE	id_ll_thres_pre_3d_2d_cu_2d_nr;	//
	
	BOOL	c_idm_en ;						// Chroma intensity difference motion detection enable
	BOOL	c_idm_auto ;					// Intensity difference motion detection enable
	BYTE	c_id_k_value ;						
	BYTE	c_id_thres_ratio ;						
	BYTE	c_id_thres_raw_pren ;			//
	BYTE	c_id_thres_c3d_pren ;			//
	BYTE	c_id_ll_thres_raw_pren ;		//
	BYTE	c_id_ll_thres_c3d_pren ;		//
	
	BYTE	idm_hist_en ;

	BYTE	idc_thres_0;					//	
	BYTE	idc_thres_1;					//	
	
	BYTE	idc_ll_thres_0;					//	
	BYTE	idc_ll_thres_1;					//	

}	NR_IDM_PARAM, *PNR_IDM_PARAM;

typedef	struct
{
	BOOL	pdm_en ;						// Pattern difference motion detection enable
	BOOL	pdm_auto ;						// Auto Pattern difference motion detection enable
	BYTE	pdm_noise_source ;
	BOOL	pd_dy_noise_limit ;
	BYTE	pd_motion_k ;					//
	BYTE	pd_nutral_motion_k ;			//
	BYTE	pd_sads_ratio;					//
	BYTE	pd_noise_limit;					//
	BYTE	pd_motion_th;					//
	BYTE	pd_motion_adj;					//
	
	BOOL	pd_ll_dy_noise_limit ;
	BYTE	pd_ll_motion_k ;				//
	BYTE	pd_ll_nutral_motion_k ;			//	
	BYTE	pd_ll_sads_ratio;				//
	BYTE	pd_ll_noise_limit;				//
	BYTE	pd_ll_motion_th;				//
	BYTE	pd_ll_motion_adj;				//

	BYTE	sr_replace ;
	BYTE	sr_replace_c ;	
	
	BOOL	pd_pmh_en ;
	BYTE	pd_pmh_thres ;
	BYTE	pd_pmh_k00 ;
	BYTE	pd_pmh_k01 ;
	BYTE	pd_pmh_k02 ;	
	// BYTE	dummy ;		
}	NR_PDM_PARAM, *PNR_PDM_PARAM;

typedef	struct
{
	BOOL srm_en ;								// Static region motion detection enable
	BOOL srm_auto ;								
	
	BYTE block_h_size ;
	BYTE block_v_size ;
	
	BOOL sr_dy_th ;
	BYTE sr_th ;
	BYTE sr_th_fractional ;

	BOOL sr_ll_dy_th ;
	BYTE sr_ll_th ;
	BYTE sr_ll_th_fractional ;
	
	BOOL sr_bmi_corr_en ;
	BOOL sr_mi_corr_en ;
	BYTE sr_mi_corr_level ;
	BYTE sr_mi_corr_value ;
	BYTE sr_ll_mi_corr_value ;

	BOOL ll_still_en ;
	BYTE ll_still_mblk_cnt_th ;
	BYTE ll_still_frm_th ;
	BYTE ll_still_move_duration ;
	BYTE ll_still_max_intensity ;
	BYTE ll_still_duration ;
	BYTE dummy_0; 
	WORD dummy_1; 	
}	NR_SRM_PARAM, *PNR_SRM_PARAM;

typedef	struct
{
	BYTE fs_counter ;			
	BYTE filter_serial ;			
	BYTE sFilter_sigma ;		
	
	BYTE impulse_replace ;
	BYTE rold_impulse_init ;
	BYTE rold_impulse_level ;
	BYTE rold_impulse_th ;
	BYTE rold_ll_impulse_th ;	

	BYTE nf_bypass_level ;
	BYTE stdev_bypass_level ;
	BYTE filter_mode ;	
	BYTE noise_bound_mode ;	
	BYTE noise_bound_no ;	// NB table number 
	
	BYTE c_filter_serial ;
	BYTE c_filter_mode ;
	BYTE c_sFilter_sigma ;
	
	BYTE c_inbound_range ;
	
	BYTE sFilter_sigma_df ;
	BYTE rold_impulse_th_df ;
	BYTE rold_ll_impulse_th_df ;
	BYTE filter_mode_df ;
	BYTE noise_bound_mode_df ;
		
	BYTE noise_bound_no_df ; // NB table number df
	
	BYTE pmh_2D_en ;
	BYTE pmh_2D_th ;
	BYTE pmh_2D_mode ;
	
	BYTE pmh_2Df_en ;
	BYTE pmh_2Df_th ;	
	BYTE pmh_2Df_mode ;	
	
	BYTE pmh_2Dc_en ;
	BYTE pmh_2Dc_th ;	
	BYTE dummy ;
}	NR_2DNR_PARAM, *PNR_2DNR_PARAM;

typedef	struct
{
	BYTE update_counter ;
	BYTE y_en ;
	BYTE c_en ;
	
	WORD coef_min ;
	WORD y_coef_00 ; // Unity sum :1024
	WORD y_coef_01 ;
	WORD y_coef_02 ;	
	WORD y_coef_03 ;	
	
	WORD c_coef_00 ; // Unity sum :1024
	WORD c_coef_01 ;
	WORD c_coef_02 ;	
	WORD c_coef_03 ;	
	
	WORD c_tint ;	
	WORD dummy ;
}	NR_L2DNR_PARAM, *PNR_L2DNR_PARAM;

typedef	struct
{
	BYTE filter_level ;
	BYTE filter_en ;
	BYTE th_gain ;		// filter diff thres = variance * th_gain + th_offset
	BYTE th_offset ;	// filter diff thres = variance * th_gain + th_offset
	BYTE th_offset_limit ;
}	NR_M2DNR_PARAM, *PNR_M2DNR_PARAM;

typedef	struct
{
	BOOL ll_en ;
	
	BYTE ll_enter_th ;
	BYTE ll_exit_th ;

	WORD ent_level;		// NR low light mode enter level
	WORD ent_level_fr;	// NR low light mode enter level(fractional)
	WORD exit_level;	// NR low light mode exit level
	WORD exit_level_fr;	// NR low light mode exit level(fractional)	
	BYTE dummy ;
}	NR_LL_PARAM, *PNR_LL_PARAM;

typedef	struct
{
	BYTE update_counter;
	BOOL Adaptive_en;
	BYTE k_value_mode ;
	BYTE const_k_value ;
	BYTE const_k_value_oth ;
	BYTE k_value_bound00 ;
	BYTE k_value_bound01 ;
	BYTE k_value_bound02 ;
	BYTE k_value_bound03 ;
	BYTE k_value_bound04 ;
	BYTE k_value_bound05 ;
	
	BYTE k_value_00 ;
	BYTE k_value_01 ;
	BYTE k_value_02 ;
	BYTE k_value_03 ;
	BYTE k_value_04 ;
	BYTE k_value_05 ;
	
	BYTE k_value_oth_00 ;
	BYTE k_value_oth_01 ;
	BYTE k_value_oth_02 ;
	BYTE k_value_oth_03 ;
	BYTE k_value_oth_04 ;
	BYTE k_value_oth_05 ;	
	BYTE dummy ;
}	NR_3DNR_PARAM, *PNR_3DNR_PARAM;

typedef	struct
{
	BYTE	bound_state ;
	BYTE	dark_upper_bound ;
	BYTE	bright_lower_bound ;
	WORD	edge_level ;
		
	WORD	stdev_int;			// Read only parameter for frame average Stdev value ( integer part )
	WORD	stdev_frac;			// Read only parameter for frame average Stdev value ( fractional part )

	WORD	stdev_int_br;		// Read only parameter for frame average bright part Stdev value ( integer part )
	WORD	stdev_frac_br;		// Read only parameter for frame average bright part Stdev value ( fractional part )
	
	WORD	stdev_int_da;		// Read only parameter for frame average dark part Stdev value ( integer part )
	WORD	stdev_frac_da;		// Read only parameter for frame average dark part Stdev value ( fractional part )		

	WORD	prestdev_int;		// Read only parameter for frame average Stdev value ( integer part )
	WORD	prestdev_frac;		// Read only parameter for frame average Stdev value ( fractional part )	
	
	BYTE	agc_max ;			
	BYTE	under_exp ;	

#if __NR_REV__	
	BYTE	mblocks[4] ;
#endif
	BYTE	corr_ratio ;
}	NR_FRAMENOISE_PARAM, *PNR_FRAMENOISE_PARAM;

typedef	struct
{
	BYTE wdr_tm_only_mode;
	BYTE mo_his_rd_en;
	BYTE mo_flag_rd_en;
	BYTE r_wdr_mo_en;
	BYTE r_color_suppress_en;
	BYTE r_chk_surface;
	BYTE r_2D_wdr_mo_mode;
}	NR_WDR_PARAM, *PNR_WDR_PARAM;

typedef	struct
{
	LONG 	ctrl;			// NR ctrl flag
		
	BOOL	image_type;		// 0:bayer / 1:yc
	BOOL	bayer_channel;	// 0: B or R / 1: G  
	BYTE	mode;			// NR mode
	WORD 	intensity;		// NR 3D intensity mode
	WORD 	intensity_2D;	// NR 2D intensity mode
	BOOL 	lowlight_mode;	// NR lowlight mode : read only parameter
	WORD	demo_mode;		// NR demo mode flag
	BOOL	nr_10bit;		// NR 10bit mode flag
	BYTE	aperture;		// NR motion aperture 
	
	NR_MCU_CTRL_PARAM stMCUDRIVE;
	NR_LINEMEM_PARAM stLINEMEM ;
	NR_IDM_PARAM stIDM ;
	NR_PDM_PARAM stPDM ;
	NR_SRM_PARAM stSRM ;	
	NR_2DNR_PARAM st2DNR ;	
	NR_3DNR_PARAM st3DNR ;		
	NR_LL_PARAM stLL ;
	NR_FRAMENOISE_PARAM stFRAMENOISE ;
	NR_WDR_PARAM stWDR ;

#if __NR_REV__	
	WORD 	strategy;	
	NR_M2DNR_PARAM stM2DNR ;
#else
	BYTE	dummy;
#endif
	
	NR_L2DNR_PARAM stL2DNR ;	
}	NR_CTRL_PARAM, *PNR_CTRL_PARAM;

#define	NRCTRL_TRIGGER_ISP			0x40000000		// ISP update
#define NRCTRL_MCU_DRIVE_UPDATE		0x10000000		// NR mcu drive mode update
#define NRCTRL_MCU_DRIVE_RETURN		0x20000000		// NR mcu drive auto return

#define NRCTRL_IMAGE_TYPE_UPDATE	0x00000001		// NR image type update
#define NRCTRL_MODE_UPDATE			0x00000002		// NR mode update
#define	NRCTRL_IMAGE_SIZE_UPDATE	0x00000004		// NR image size update
#define NRCTRL_DEMOMODE_UPDATE		0x00000008		// NR demo mode update
#define	NRCTRL_3DINTENSITY_UPDATE	0x00000010		// NR 3D intensity update
#define	NRCTRL_LOWLIGHTLEVEL_UPDATE	0x00000020		// NR low light level update
#define	NRCTRL_DATA_ORDER_UPDATE	0x00000040		// NR bayer data order update ( Only for bayer NR )
	
#define	NRCTRL_LINEMEM_UPDATE		0x00000080		// NR linemem update
	
#define NRCTRL_IDM_UPDATE			0x00000100		// NR IDM update
#define NRCTRL_AUTO_IDM_UPDATE		0x00000200		// NR AUTO IDM update
#define NRCTRL_IDMc_UPDATE			0x00000400		// NR IDMc update
#define NRCTRL_AUTO_IDMc_UPDATE		0x00000800		// NRAUTO  IDMc update
	
#define NRCTRL_PDM_UPDATE			0x00001000		// NR PDM update
#define NRCTRL_AUTO_PDM_UPDATE		0x00002000		// NR AUTO PDM update
	
#define NRCTRL_SRM_UPDATE			0x00010000		// NR SRM update
#define NRCTRL_AUTO_SRM_UPDATE		0x00020000		// NR AUTO SRM update
	
#define NRCTRL_FRMNOISE_UPDATE		0x00100000		// NR Bright Dark bound update
#define NRCTRL_WDR_FLAG_UPDATE		0x00200000		// NR WDR flag control update
	
#define NRCTRL_2D_SFILTER_UPDATE	0x00400000		// NR 2D_SLUT_UPDATE
#define NRCTRL_2Dc_SFILTER_UPDATE	0x00800000		// NR 2Dc_SLUT_UPDATE
#define NRCTRL_2Df_SFILTER_UPDATE	0x01000000		// NR 2Df_SLUT_UPDATE
	
#define NRCTRL_2D_NB_UPDATE			0x02000000		// NR 2D_noise_bound_UPDATE
	
#define NRCTRL_2D_LFILTER_UPDATE	0x04000000		// NR 2D_L filter
#define NRCTRL_2D_MFILTER_UPDATE	0x08000000		// NR 2D_M filter
// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Device drive functions

// nr.c

void Init_NR(void) ;

void NR_ProcessHandler(void) ;

// -----------------------------------------------------------------------------
// API Get / Set functions

void SetNR_Toggle_Inputimage_type(void) ;	
void SetNR_Mode( MISP_NR_MODE_t val );
void SetNR_intensity( MISP_NR3D_MODE_t val );
void SetNR_Aperture( BYTE aperture );
void SetNR_3Dintensity( MISP_NR3D_MODE_t val );
void SetNR_2Dintensity( MISP_NR3D_MODE_t val );
void SetNR_3Dconst_k(WORD val, WORD val_oth);
void SetNR_LowLightEnterLevel( WORD val_int , WORD val_fr );
void SetNR_LowLightExitLevel( WORD val_int , WORD val_fr );
void SetNR_Bayer_data_order(MISP_SS_BAYER_PATTERN_t val);
void SetNR_DemoMode(MISP_NRDEMO_MODE_t val);
void SetNR_image_size(BYTE bH , BYTE bV, WORD valH , WORD valV);
void SetNR_BitMode(BOOL mode);

void SetNR_MCUDriveMode(BOOL drive_en, BOOL force_lowlight_en , BOOL force_still_en , BYTE still_intensity ,BYTE drive_duration );

void SetNR_IDMAuto(BOOL auto_en ); 
void SetNR_IDMcAuto(BOOL auto_en ); 
void SetNR_IDM(BOOL op_mode , BOOL dy_noise_limit , BYTE noise_limit , BYTE mo_th_0 , BYTE mo_th_1 , BYTE mo_th_2 , BYTE mo_th_3 ) ;
void SetNR_IDM_ratio( BYTE t_ratio, BYTE y_ratio, BYTE b_ratio);

void SetNR_IDM_mhist(BOOL en) ;
void SetNR_2D_mhist(BOOL en) ;
void SetNR_PDM_mhist(BOOL en) ;
void SetNR_PDM_mhistTH(BYTE pmh_thres) ;
void SetNR_PDM_mhist_K(BYTE pmh_k00 , BYTE pmh_k01, BYTE pmh_k02) ;

void SetNR_IDMc(BOOL op_mode, BYTE CBound, BYTE th_0 , BYTE th_1) ;

void SetNR_PDMAuto(BOOL auto_en );

void SetNR_PDMratio(BOOL op_mode, BYTE ratio) ;
void SetNR_PDM_replace( BYTE pdm_replace , BYTE pdm_replace_c ) ;

void SetNR_PDM(BOOL op_mode, BOOL dy_noise_limit, BYTE noise_limit , BYTE motion_th, BYTE motion_adj );
void SetNR_PDMmotionK(BOOL op_mode, BYTE motion_k, BYTE neutral_motion_k ) ;

void SetNR_SRMAuto(BOOL auto_en );
void SetNR_SRM(BOOL op_mode, BOOL dy_th, BYTE motion_th , BYTE motion_th_frac );
void SetNR_SRM_corr( BOOL op_mode, BOOL pdm_en , BOOL mi_en ,  BYTE mi_corr_value ) ;

void SetNR_NRZoomingMode(void);

void SetNR_PixelCategoryBound(BYTE dark_upper_bound , BYTE bright_lower_bound); // __NR_REV__ 1

void SetNR_UseWdrMotionFlag(BYTE mode);

void SetNR_2D_ImpulseThresh(BOOL op_mode , BYTE ImpulseThresh , BOOL filter_select , BYTE ImpulseReplace );
void SetNR_2D_Impulselevel(BYTE imp_level);
void SetNR_2D_Bypasslevel(BYTE nf_level , BYTE stdev_level);
void SetNR_2D_sFilter(WORD table_no , BYTE filter_select);
void SetNR_2D_FilterMode(BYTE mode , BYTE serialize, BYTE filter_select);
void SetNR_2D_NoiseBound(WORD table_no , BOOL filter_select , BYTE nb_mode);
void SetNR_LetzteresfilterEN(BYTE y_en, BYTE c_en ) ;
void SetNR_LetzteresfilterY(WORD value, WORD width ) ;
void SetNR_LetzteresfilterC(WORD value, WORD width , WORD tint_adjust) ;
void SetNR_LetzteresfilterLevel(BYTE level) ;
void SetNR_Mittlerfilter(BYTE en, BYTE gain , BYTE offset) ;
void SetNR_MittlerfilterLevel(BYTE level) ;

// -----------------------------------------------------------------------------

WORD GetNR_Inputimage_type(void);
WORD GetNR_DemoMode(void);
MISP_NR_MODE_t GetNR_Mode(void);
MISP_NR3D_MODE_t GetNR_intensity(void);
MISP_NR3D_MODE_t GetNR_3Dintensity(void);
MISP_NR3D_MODE_t GetNR_2Dintensity(void);
MISP_NRLOWLIGHT_MODE_t GetNR_LowLightMode(void);
BYTE GetNR_3Dconst_k(void);
BYTE GetNR_mblocks(void);
BYTE GetNR_Aperture(void);

WORD GetNR_LowLightStillIntensity(void);
void GetNR_preStDEV(PWORD StDev_int, PWORD StDev_frac, BYTE target ) ;
void GetNR_StDEV(PWORD StDev_int, PWORD StDev_frac, BYTE target ) ;

void GetNR_StDEV_br(PWORD StDev_int, PWORD StDev_frac, BYTE target ) ; // __NR_REV__ 1
void GetNR_StDEV_da(PWORD StDev_int, PWORD StDev_frac, BYTE target ) ; // __NR_REV__ 1

BOOL GetNR_BitMode(void);
WORD GetNR_Strategy(void);

// -----------------------------------------------------------------------------
// Function for NR split slave
// #if __NR_SPLIT__==2
void SetNRS_FNMisc(BYTE agc_max, BYTE under_exp, BYTE edge_level, BYTE lowlight_mode ) ;
void SetNRS_FNUpdateCounter(BYTE update_counter) ;
void SetNRS_FNStdev(BYTE StDev_int, BYTE StDev_frac ) ;
void SetNRS_FNStdevBr(BYTE StDev_int, BYTE StDev_frac ) ;
void SetNRS_FNStdevDa(BYTE StDev_int, BYTE StDev_frac ) ;

void NR_ProcessSlaveHandler(void) ;
// #endif

// -----------------------------------------------------------------------------

#endif	/* __MISP_NR_H__ */
