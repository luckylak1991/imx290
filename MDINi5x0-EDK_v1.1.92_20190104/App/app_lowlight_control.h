#ifndef		__MDIN_LLCTRL_H__
#define		__MDIN_LLCTRL_H__

#if defined(__USE_MDIN_i510__)||defined(__USE_MDIN_i540__)||defined(__USE_MDIN_i550__)
//--------------------------------------------------------------------------------------------------------------------------
//
// 	Low light control is API base application of i5x0 series.
//  Low light Auto saturation  / Low light Color suppress / Low light Edge enhancement
//  Interaction to given MENU value
// ----------------------------------------------------------------------

// __LLCNTL_DEBUG_PRINT__ [ 0: off / 1: on ]
#define __LLCNTL_DEBUG_PRINT__ 0

// ----------------------------------------------------------------------
// define
// ----------------------------------------------------------------------
#define __LL_CONTROL_PERIOD__ 5 // WORKS EVERY 5 FRAME
#define __LL_AUTOSAT_INIT_INTENSITY__ 2 // Initial Auto sat value when EEPROM is empty
// #define __LL_AUTOSAT_INIT_INTENSITY__ 0 // Initial Auto sat value when EEPROM is empty
#define __LL_AUTOCSP_INIT_INTENSITY__ 1 // Initial Auto CSP value when EEPROM is empty
#define __LL_AUTOEDGE_INIT_INTENSITY__ 1 // Initial Auto EE value when EEPROM is empty
#define __LL_AUTOCAC_EN__ 1 // Initial Auto CAC gain control enable value when EEPROM is empty

#define __LL_AUTOSAT_MINIMUM__ 32	 // 128 centered lowest saturation limit
#define __LL_AUTOSAT_INIT_TRACE_SPEED__ 2	 // Auto saturation trace speed

#define __LL_AUTOEDGE_TRACE_SPEED__ 4	 // Auto EDGE trace speed when menu adjust 1~12. LL Auto decreasing speed will be 1 regardless of this value.

#define __LL_AUTOCAC_MINIMUM__ 400 // Auto CAC gain control min value
// ----------------
// Adjusted target EDEG = __LL_AUTOEDGE_COMP_TICK__*edge_compensation
#define __LL_AUTOEDGE_COMP_TICK__ 6 
#define __LL_AUTOEDGE_MENU_TICK__ 6 
#define __LL_AUTOEDGE_INIT_HVPFLEVEL__ 5*__LL_AUTOEDGE_MENU_TICK__ 

// --------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------
// Sensor dependant tune
// --------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------
#if defined(__USE_CMOS_IMX274__) ||defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX326__)
// --------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------

// VALUE for saturation decrese unit default 12
// Adjusted target saturation = __LL_AUTOSAT_COMP_TICK__*AutoSat_compensation
#define __LL_AUTOSAT_COMP_TICK__ 6 
// Actual working noise stdev level 
#define __LL_AUTOSAT_1__ 8 // intensity low active level
#define __LL_AUTOSAT_2__ 6 // intensity mid active level
#define __LL_AUTOSAT_3__ 4 // intensity high active level

// If to use defined gain level rather then gerneral calculation
#define __LL_USE_AUTOSAT_DEFINED_GAIN_LEVEL_ 0 

#if (__LL_USE_AUTOSAT_DEFINED_GAIN_LEVEL_==1)
#define __LL_AUTOSAT_GAIN_LEVEL_1__ 10000; // 45dB = 13824 : Check actual gain value to determine
#define __LL_AUTOSAT_GAIN_LEVEL_2__ 6000; // 35dB = 6144 : Check actual gain value to determine
#define __LL_AUTOSAT_GAIN_LEVEL_3__ 4000; // 30dB = 4096 : Check actual gain value to determine
#endif

#define __LL_AUTOCSP_EQ_1__ // EQ for UHD
// #define __LL_AUTOCSP_EQ_2__ // EQ for FHD

#define __LL_AUTOCSP_MID_NPOINT_ 6

// If to use defined gain level rather then gerneral calculation
#define __LL_USE_AUTOCSP_DEFINED_GAIN_LEVEL_ 0 

#if (__LL_USE_AUTOCSP_DEFINED_GAIN_LEVEL_==1)
#define __LL_AUTOCSP_GAIN_LEVEL_1__ 10000; // 45dB = 13824 : Check actual gain value to determine
#define __LL_AUTOCSP_GAIN_LEVEL_2__ 6000; // 35dB = 6144 : Check actual gain value to determine
#define __LL_AUTOCSP_GAIN_LEVEL_3__ 4000; // 30dB = 4096 : Check actual gain value to determine
#endif

#define __LL_AUTOEDGE_EQ_1__ // EQ for UHD
//#define __LL_AUTOEDGE_EQ_2__ // EQ for FHD 1/2" sensor
//#define __LL_AUTOEDGE_EQ_3__ // EQ for FHD 1/3" sensor

#if defined(__LL_AUTOEDGE_EQ_1__) // __LL_AUTOEDGE_EQ_1__ setting
#define __LL_AUTOEDGE_EQ_NL__ 10
#endif

#if defined(__LL_AUTOEDGE_EQ_2__) // __LL_AUTOEDGE_EQ_2__ setting
#define __LL_AUTOEDGE_EQ_NL_1_ 7
#define __LL_AUTOEDGE_EQ_NL_2_ 4
#define __LL_AUTOEDGE_EQ_NL_3_ 2
#define __LL_AUTOEDGE_EQ_COMP_1__ 2
#define __LL_AUTOEDGE_EQ_COMP_2__ 1
#define __LL_AUTOEDGE_EQ_COMP_3__ 0
#endif

#if defined(__LL_AUTOEDGE_EQ_3__) // __LL_AUTOEDGE_EQ_3__ setting
#define __LL_AUTOEDGE_EQ_NL__ 7
#define __LL_AUTOEDGE_EQ_COMP__ (__LL_AUTOEDGE_EQ_NL__-2)
#endif

#define __LL_AUTOEDGE_GAIN_LEVEL__ 23040 // 256*90

#define __LL_AUTOCAC_G_COMP_TICK_0__ 27
#define __LL_AUTOCAC_G_COMP_TICK_1__ 35
#define __LL_AUTOCAC_M_COMP_TICK_0__ 0
#define __LL_AUTOCAC_M_COMP_TICK_1__ 0
#define __LL_AUTOCAC_NPOINT_0__ 7 // The noise level of A-gain max
#define __LL_AUTOCAC_NPOINT_1__ 15 // The noise level of A-gain max
#define __LL_AUTOCAC_TRACE_SPEED__ 12 // Auto CAC gain control trace speed value

// --------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------
#elif defined(__USE_IMX290_HCG_MODE__)
// --------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------

// VALUE for saturation decrese unit default 12
// Adjusted target saturation = __LL_AUTOSAT_COMP_TICK__*AutoSat_compensation
#define __LL_AUTOSAT_COMP_TICK__ 12 

// Actual working noise stdev level 
#define __LL_AUTOSAT_1__ 8 // intensity low active level
#define __LL_AUTOSAT_2__ 6 // intensity mid active level
#define __LL_AUTOSAT_3__ 4 // intensity high active level

// If to use defined gain level rather then gerneral calculation
#define __LL_USE_AUTOSAT_DEFINED_GAIN_LEVEL_ 1

#if (__LL_USE_AUTOSAT_DEFINED_GAIN_LEVEL_==1)
#define __LL_AUTOSAT_GAIN_LEVEL_1__ 10000; // 45dB = 13824 : Check actual gain value to determine
#define __LL_AUTOSAT_GAIN_LEVEL_2__ 6000; // 35dB = 6144 : Check actual gain value to determine
#define __LL_AUTOSAT_GAIN_LEVEL_3__ 4000; // 30dB = 4096 : Check actual gain value to determine
#endif

//#define __LL_AUTOCSP_EQ_1__ // EQ for UHD
#define __LL_AUTOCSP_EQ_2__ // EQ for FHD

#define __LL_AUTOCSP_MID_NPOINT_ 6

// If to use defined gain level rather then gerneral calculation
#define __LL_USE_AUTOCSP_DEFINED_GAIN_LEVEL_ 1

#if (__LL_USE_AUTOCSP_DEFINED_GAIN_LEVEL_==1)
#define __LL_AUTOCSP_GAIN_LEVEL_1__ 10000; // 45dB = 13824 : Check actual gain value to determine
#define __LL_AUTOCSP_GAIN_LEVEL_2__ 6000; // 35dB = 6144 : Check actual gain value to determine
#define __LL_AUTOCSP_GAIN_LEVEL_3__ 4000; // 30dB = 4096 : Check actual gain value to determine
#endif

//#define __LL_AUTOEDGE_EQ_1__ // EQ for UHD
//#define __LL_AUTOEDGE_EQ_2__ // EQ for FHD 1/2" sensor
#define __LL_AUTOEDGE_EQ_3__ // EQ for FHD 1/3" sensor

#if defined(__LL_AUTOEDGE_EQ_1__) // __LL_AUTOEDGE_EQ_1__ setting
#define __LL_AUTOEDGE_EQ_NL__ 10
#endif

#if defined(__LL_AUTOEDGE_EQ_2__) // __LL_AUTOEDGE_EQ_2__ setting
#define __LL_AUTOEDGE_EQ_NL_1_ 7
#define __LL_AUTOEDGE_EQ_NL_2_ 4
#define __LL_AUTOEDGE_EQ_NL_3_ 2
#define __LL_AUTOEDGE_EQ_COMP_1__ 2
#define __LL_AUTOEDGE_EQ_COMP_2__ 1
#define __LL_AUTOEDGE_EQ_COMP_3__ 0
#endif

#if defined(__LL_AUTOEDGE_EQ_3__) // __LL_AUTOEDGE_EQ_3__ setting
#define __LL_AUTOEDGE_EQ_NL__ 7
#define __LL_AUTOEDGE_EQ_COMP__ (__LL_AUTOEDGE_EQ_NL__-2)
#endif

#define __LL_AUTOEDGE_GAIN_LEVEL__ 2800 // 27dB = 2816

#define __LL_AUTOCAC_G_COMP_TICK_0__ 16
#define __LL_AUTOCAC_G_COMP_TICK_1__ 22
#define __LL_AUTOCAC_M_COMP_TICK_0__ 0
#define __LL_AUTOCAC_M_COMP_TICK_1__ 0
#define __LL_AUTOCAC_NPOINT_0__ 7 // The noise level of A-gain max
#define __LL_AUTOCAC_NPOINT_1__ 17 // The noise level of A-gain max knee point
#define __LL_AUTOCAC_TRACE_SPEED__ 12 // Auto CAC gain control trace speed value

// --------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------
#elif defined(__USE_CMOS_IMX185__) || defined(__USE_CMOS_IMX385__)
// --------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------

// VALUE for saturation decrese unit default 12
// Adjusted target saturation = __LL_AUTOSAT_COMP_TICK__*AutoSat_compensation
#define __LL_AUTOSAT_COMP_TICK__ 12 

// Actual working noise stdev level 
#define __LL_AUTOSAT_1__ 6 // intensity low active level
#define __LL_AUTOSAT_2__ 4 // intensity mid active level
#define __LL_AUTOSAT_3__ 2 // intensity high active level

// If to use defined gain level rather then gerneral calculation
#if defined(__USE_IMX385_HCG_MODE__)
#define __LL_USE_AUTOSAT_DEFINED_GAIN_LEVEL_ 1
#else
#define __LL_USE_AUTOSAT_DEFINED_GAIN_LEVEL_ 0	
#endif

#if (__LL_USE_AUTOSAT_DEFINED_GAIN_LEVEL_==1)
#define __LL_AUTOSAT_GAIN_LEVEL_1__ 10000; // 45dB = 13824 : Check actual gain value to determine
#define __LL_AUTOSAT_GAIN_LEVEL_2__ 6000; // 35dB = 6144 : Check actual gain value to determine
#define __LL_AUTOSAT_GAIN_LEVEL_3__ 4000; // 30dB = 4096 : Check actual gain value to determine
#endif

//#define __LL_AUTOCSP_EQ_1__ // EQ for UHD
#define __LL_AUTOCSP_EQ_2__ // EQ for FHD

#define __LL_AUTOCSP_MID_NPOINT_ 6

// If to use defined gain level rather then gerneral calculation
#if defined(__USE_IMX385_HCG_MODE__)
#define __LL_USE_AUTOCSP_DEFINED_GAIN_LEVEL_ 1
#else
#define __LL_USE_AUTOCSP_DEFINED_GAIN_LEVEL_ 0	
#endif

#if (__LL_USE_AUTOCSP_DEFINED_GAIN_LEVEL_==1)
#define __LL_AUTOCSP_GAIN_LEVEL_1__ 10000; // 45dB = 13824 : Check actual gain value to determine
#define __LL_AUTOCSP_GAIN_LEVEL_2__ 6000; // 35dB = 6144 : Check actual gain value to determine
#define __LL_AUTOCSP_GAIN_LEVEL_3__ 4000; // 30dB = 4096 : Check actual gain value to determine
#endif

//#define __LL_AUTOEDGE_EQ_1__ // EQ for UHD
#define __LL_AUTOEDGE_EQ_2__ // EQ for FHD 1/2" sensor
//#define __LL_AUTOEDGE_EQ_3__ // EQ for FHD 1/3" sensor

#if defined(__LL_AUTOEDGE_EQ_1__) // __LL_AUTOEDGE_EQ_1__ setting
#define __LL_AUTOEDGE_EQ_NL__ 10
#endif

#if defined(__LL_AUTOEDGE_EQ_2__) // __LL_AUTOEDGE_EQ_2__ setting
#define __LL_AUTOEDGE_EQ_NL_1_ 7
#define __LL_AUTOEDGE_EQ_NL_2_ 4
#define __LL_AUTOEDGE_EQ_NL_3_ 2
#define __LL_AUTOEDGE_EQ_COMP_1__ 2
#define __LL_AUTOEDGE_EQ_COMP_2__ 1
#define __LL_AUTOEDGE_EQ_COMP_3__ 0
#endif

#if defined(__LL_AUTOEDGE_EQ_3__) // __LL_AUTOEDGE_EQ_3__ setting
#define __LL_AUTOEDGE_EQ_NL__ 7
#define __LL_AUTOEDGE_EQ_COMP__ (__LL_AUTOEDGE_EQ_NL__-2)
#endif

#define __LL_AUTOEDGE_GAIN_LEVEL__ 2800 // 27dB = 2816

#define __LL_AUTOCAC_G_COMP_TICK_0__ 16
#define __LL_AUTOCAC_G_COMP_TICK_1__ 22
#define __LL_AUTOCAC_M_COMP_TICK_0__ 0
#define __LL_AUTOCAC_M_COMP_TICK_1__ 0
#define __LL_AUTOCAC_NPOINT_0__ 7 // The noise level of A-gain max
#define __LL_AUTOCAC_NPOINT_1__ 15 // The noise level of A-gain max knee point
#define __LL_AUTOCAC_TRACE_SPEED__ 12 // Auto CAC gain control trace speed value

// --------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------
#elif defined(__USE_CMOS_IMX334__)
// --------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------

// VALUE for saturation decrese unit default 12
// Adjusted target saturation = __LL_AUTOSAT_COMP_TICK__*AutoSat_compensation
#define __LL_AUTOSAT_COMP_TICK__ 12 

// Actual working noise stdev level 
#define __LL_AUTOSAT_1__ 8 // intensity low active level
#define __LL_AUTOSAT_2__ 6 // intensity mid active level
#define __LL_AUTOSAT_3__ 4 // intensity high active level

// If to use defined gain level rather then gerneral calculation
#define __LL_USE_AUTOSAT_DEFINED_GAIN_LEVEL_ 1

#if (__LL_USE_AUTOSAT_DEFINED_GAIN_LEVEL_==1)
#define __LL_AUTOSAT_GAIN_LEVEL_1__ 10000; // 45dB = 13824 : Check actual gain value to determine
#define __LL_AUTOSAT_GAIN_LEVEL_2__ 6000; // 35dB = 6144 : Check actual gain value to determine
#define __LL_AUTOSAT_GAIN_LEVEL_3__ 4000; // 30dB = 4096 : Check actual gain value to determine
#endif

#define __LL_AUTOCSP_EQ_1__ // EQ for UHD
//#define __LL_AUTOCSP_EQ_2__ // EQ for FHD

#define __LL_AUTOCSP_MID_NPOINT_ 6

// If to use defined gain level rather then gerneral calculation
#define __LL_USE_AUTOCSP_DEFINED_GAIN_LEVEL_ 1

#if (__LL_USE_AUTOCSP_DEFINED_GAIN_LEVEL_==1)
#define __LL_AUTOCSP_GAIN_LEVEL_1__ 10000; // 45dB = 13824 : Check actual gain value to determine
#define __LL_AUTOCSP_GAIN_LEVEL_2__ 6000; // 35dB = 6144 : Check actual gain value to determine
#define __LL_AUTOCSP_GAIN_LEVEL_3__ 4000; // 30dB = 4096 : Check actual gain value to determine
#endif

#define __LL_AUTOEDGE_EQ_1__ // EQ for UHD
//#define __LL_AUTOEDGE_EQ_2__ // EQ for FHD 1/2" sensor
//#define __LL_AUTOEDGE_EQ_3__ // EQ for FHD 1/3" sensor

#if defined(__LL_AUTOEDGE_EQ_1__) // __LL_AUTOEDGE_EQ_1__ setting
#define __LL_AUTOEDGE_EQ_NL__ 10
#endif

#if defined(__LL_AUTOEDGE_EQ_2__) // __LL_AUTOEDGE_EQ_2__ setting
#define __LL_AUTOEDGE_EQ_NL_1_ 7
#define __LL_AUTOEDGE_EQ_NL_2_ 4
#define __LL_AUTOEDGE_EQ_NL_3_ 2
#define __LL_AUTOEDGE_EQ_COMP_1__ 2
#define __LL_AUTOEDGE_EQ_COMP_2__ 1
#define __LL_AUTOEDGE_EQ_COMP_3__ 0
#endif

#if defined(__LL_AUTOEDGE_EQ_3__) // __LL_AUTOEDGE_EQ_3__ setting
#define __LL_AUTOEDGE_EQ_NL__ 7
#define __LL_AUTOEDGE_EQ_COMP__ (__LL_AUTOEDGE_EQ_NL__-2)
#endif

#define __LL_AUTOEDGE_GAIN_LEVEL__ 2800 // 27dB = 2816

#define __LL_AUTOCAC_G_COMP_TICK_0__ 16
#define __LL_AUTOCAC_G_COMP_TICK_1__ 22
#define __LL_AUTOCAC_M_COMP_TICK_0__ 0
#define __LL_AUTOCAC_M_COMP_TICK_1__ 0
#define __LL_AUTOCAC_NPOINT_0__ 7 // The noise level of A-gain max
#define __LL_AUTOCAC_NPOINT_1__ 17 // The noise level of A-gain max knee point
#define __LL_AUTOCAC_TRACE_SPEED__ 12 // Auto CAC gain control trace speed value

// --------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------
#else
// --------------------------------------------------------------------------------------------------------------------------------	
// --------------------------------------------------------------------------------------------------------------------------------	
// General setting
// --------------------------------------------------------------------------------------------------------------------------------	
// --------------------------------------------------------------------------------------------------------------------------------

// VALUE for saturation decrese unit default 12
// Adjusted target saturation = __LL_AUTOSAT_COMP_TICK__*AutoSat_compensation
#define __LL_AUTOSAT_COMP_TICK__ 12 

// Actual working noise stdev level 
#define __LL_AUTOSAT_1__ 6 // intensity low active level
#define __LL_AUTOSAT_2__ 4 // intensity mid active level
#define __LL_AUTOSAT_3__ 2 // intensity high active level

// If to use defined gain level rather then gerneral calculation
#define __LL_USE_AUTOSAT_DEFINED_GAIN_LEVEL_ 0 

#if (__LL_USE_AUTOSAT_DEFINED_GAIN_LEVEL_==1)
#define __LL_AUTOSAT_GAIN_LEVEL_1__ 10000; // 45dB = 13824 : Check actual gain value to determine
#define __LL_AUTOSAT_GAIN_LEVEL_2__ 6000; // 35dB = 6144 : Check actual gain value to determine
#define __LL_AUTOSAT_GAIN_LEVEL_3__ 4000; // 30dB = 4096 : Check actual gain value to determine
#endif

//#define __LL_AUTOCSP_EQ_1__ // EQ for UHD
#define __LL_AUTOCSP_EQ_2__ // EQ for FHD

#define __LL_AUTOCSP_MID_NPOINT_ 6

// If to use defined gain level rather then gerneral calculation
#define __LL_USE_AUTOCSP_DEFINED_GAIN_LEVEL_ 0 

#if (__LL_USE_AUTOCSP_DEFINED_GAIN_LEVEL_==1)
#define __LL_AUTOCSP_GAIN_LEVEL_1__ 10000; // 45dB = 13824 : Check actual gain value to determine
#define __LL_AUTOCSP_GAIN_LEVEL_2__ 6000; // 35dB = 6144 : Check actual gain value to determine
#define __LL_AUTOCSP_GAIN_LEVEL_3__ 4000; // 30dB = 4096 : Check actual gain value to determine
#endif

//#define __LL_AUTOEDGE_EQ_1__ // EQ for UHD
//#define __LL_AUTOEDGE_EQ_2__ // EQ for FHD 1/2" sensor
#define __LL_AUTOEDGE_EQ_3__ // EQ for FHD 1/3" sensor

#if defined(__LL_AUTOEDGE_EQ_1__) // __LL_AUTOEDGE_EQ_1__ setting
#define __LL_AUTOEDGE_EQ_NL__ 10
#endif

#if defined(__LL_AUTOEDGE_EQ_2__) // __LL_AUTOEDGE_EQ_2__ setting
#define __LL_AUTOEDGE_EQ_NL_1_ 7
#define __LL_AUTOEDGE_EQ_NL_2_ 4
#define __LL_AUTOEDGE_EQ_NL_3_ 2
#define __LL_AUTOEDGE_EQ_COMP_1__ 2
#define __LL_AUTOEDGE_EQ_COMP_2__ 1
#define __LL_AUTOEDGE_EQ_COMP_3__ 0
#endif

#if defined(__LL_AUTOEDGE_EQ_3__) // __LL_AUTOEDGE_EQ_3__ setting
#define __LL_AUTOEDGE_EQ_NL__ 7
#define __LL_AUTOEDGE_EQ_COMP__ (__LL_AUTOEDGE_EQ_NL__-2)
#endif

#define __LL_AUTOEDGE_GAIN_LEVEL__ 2800 // 27dB = 2816

#define __LL_AUTOCAC_G_COMP_TICK_0__ 27
#define __LL_AUTOCAC_G_COMP_TICK_1__ 35
#define __LL_AUTOCAC_M_COMP_TICK_0__ 0
#define __LL_AUTOCAC_M_COMP_TICK_1__ 0
#define __LL_AUTOCAC_NPOINT_0__ 7 // The noise level of A-gain max
#define __LL_AUTOCAC_NPOINT_1__ 15 // The noise level of A-gain max knee point
#define __LL_AUTOCAC_TRACE_SPEED__ 12 // Auto CAC gain control trace speed value

#endif // #if defined(__USE_CMOS_IMX274__) ||defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX326__)
// ----------------
// ----------------
// End define
// ----------------
// ----------------
// ----------------

#if defined(__USE_4K60_MD1__)
#define __LLC_SPLIT__ 1 // LLC split mode master
#elif defined(__USE_4K60_MD2__)
#define __LLC_SPLIT__ 2 // LLC split mode slave
#else
#define __LLC_SPLIT__ 0 // LLC normal  mode
#endif

// ----------------------------------------------------------------------
// Struct/Union Types
// ----------------------------------------------------------------------

typedef	struct
{
	BYTE period_counter ;
	BYTE is_gain_max ;
	
	LONG GainCTLMode ;	
	LONG gain ;
	LONG dgain ;
	LONG max_gain ;
	LONG gain_factor ;
	
	LONG gain_Ysum ;	
	LONG gain_Yref ;

	WORD stdev_int ;
	WORD stdev_frac ;
	WORD pre_stdev_int ;
	WORD pre_stdev_frac ;				
	
} MDIN_LL_STATUS, *PMDIN_LL_STATUS;

typedef	struct
{
	BOOL is_initialized ;
	BYTE AutoSat_intensity ; // Value for menu matching
	
	BYTE AutoSat_normal_value ; // value indicate before compensation
	
	BYTE AutoSat_active_level ; // Actual noise level of AutoSat to work
	LONG AutoSat_gain_level ; // Actual gain level of AutoSat to work
	BYTE AutoSat_compensation ; // saturation decrease value

	BYTE AutoSat_trace_value ; // value indicate actual moving sat value 
	BYTE AutoSat_trace_speed ; // value for trace speed
	BYTE AutoSat_comp_value ; // value indicate after final compensation	
} MDIN_LL_SATURATION, *PMDIN_LL_SATURATION;

typedef	struct
{		
	BYTE AutoCSP_intensity ;
	LONG AutoCSP_gain_level ;
	
	BYTE csp_ls_x0 ;
	BYTE csp_ls_x1 ;

	BYTE x0_target ;
	BYTE x1_target ;	
	
} MDIN_LL_COLORSUPPRESS, *PMDIN_LL_COLORSUPPRESS;

typedef	struct
{
	BOOL is_initialized ;
	BYTE AutoEDGE_intensity ;
	LONG AutoEDGE_gain_level ;
	
	BYTE AutoEDGE_normal_value ;
	
	BYTE AutoEDGE_trace_value ;
	BYTE AutoEDGE_trace_speed ;
	BYTE AutoEDGE_comp_value ;	
} MDIN_LL_EDGE, *PMDIN_LL_EDGE;

typedef	struct
{		
	BYTE AutoCAC_en ;
	
	WORD AutoCAC_g_normal ; // CAC g normal state gain
	WORD AutoCAC_m_normal ; // CAC m normal state gain

	WORD AutoCAC_g_trace_value ; // CAC g actual gain
	WORD AutoCAC_m_trace_value ; // CAC m actual gain	

	WORD AutoCAC_g_trace_speed ; // CAC g actual gain
	WORD AutoCAC_m_trace_speed ; // CAC m actual gain	
	
	WORD CAC_compensation ; 
	WORD AutoCAC_g_comp_tick ; 	// CAC g decresing slope
	WORD AutoCAC_m_comp_tick ;	// CAC m decresing slope
	
	WORD AutoCAC_g_comp_value ; // CAC g ll state gain
	WORD AutoCAC_m_comp_value ;	// CAC m ll state gain
	
} MDIN_LL_CAC, *PMDIN_LL_CAC;

typedef	struct
{
	LONG 					ctrl ;
	
	MDIN_LL_STATUS 			stSTAT;
	
	MDIN_LL_SATURATION 		stSAT ;
	MDIN_LL_COLORSUPPRESS 	stCSP ;
	MDIN_LL_EDGE 			stEE ;
	
	MDIN_LL_CAC				stCAC ;	
} MDIN_LL_CONTROL_PARAM, *PMDIN_LL_CONTROL_PARAM;

#define	LLCTRL_TRIGGER_ISP			0x40000000		// ISP update

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------
void SetApp_initLLcontrol(void);
void LLApp_ProcessHandler(void);
// ----------------------------------------------------------------------
void SetApp_AutoSat_Intensity(BYTE intensity) ;
void SetApp_AutoSat_OutSaturation(BYTE val, BYTE trace_speed) ;
void SetApp_AutoSat_TraceSpeed(BYTE trace_speed) ;
void SetApp_AutoSat_ValueUpdate(void) ; // Avoid to use this function
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_AutoCSP_Intensity(BYTE intensity) ;
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_AutoEDGE_Intensity(BYTE intensity) ;
void SetApp_AutoEDGE_HVPFLevel(BYTE val) ;
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_AutoCAC_GreenUpdate(void) ; 
void SetApp_AutoCAC_MagentaUpdate(void) ; 
//--------------------------------------------------------------------------------------------------------------------------
BYTE GetApp_AutoSat_Intensity(void);
BYTE GetApp_AutoEDGE_Intensity(void);
BYTE GetApp_AutoEDGE_HVPFLevel(void);
//--------------------------------------------------------------------------------------------------------------------------
#endif // #if defined(__USE_MDIN_i510__)||defined(__USE_MDIN_i540__)||defined(__USE_MDIN_i550__)
//--------------------------------------------------------------------------------------------------------------------------

#endif // #define		__MDIN_LLCOLOR_H__


