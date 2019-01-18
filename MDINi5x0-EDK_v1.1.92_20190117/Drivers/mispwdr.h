//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name         :  mispwdr.h
// Description       :  This file contains typedefine for the driver files
// Ref. Docment      : 
// Revision History  :

#ifndef    __MISP_WDR_H__
#define    __MISP_WDR_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__CMOS_IF_H__
#include	"cmos_if.h"		// for sensor dependent option
#endif

// -----------------------------------------------------------------------------
// WDR revision
// -----------------------------------------------------------------------------

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
#define REV_WDR_NO_BUFF 0  //TM without frame buffer
#define REV_WDR_MANU_TM 0  //bug about TM max read
#define REV_ODM_LEV_FIX	0  //bug about odm_comp_mode 0x220[2:1]
#define REV_REG_MGR_MDS 0  //MGR motion data select
#define REV_x36_CLR_SPR 0  //Sony Built-in color suppress
#define REV_RB_SLOPE_EN 0  //WB new register rb_slope_en
#define REV_DES_DOL_ADJ 0  //Deserializer support sony DOL
#define REV_WDR_8_SPLIT 0  //WDR support 8-split mode
#define REV_WDR_NEW_MGR 0  //New Merge Algorithm
#define REV_WDR_PREV_WB 0  //Use WB'ed data for MGR input
#define REV_TM_DARK_CNT 0  //Use TM dark pixel counter
#define REV_ROT_FUNC_EN 0  //Share line-memory with Rotation Func.
#define USE_IPC_COMPRES 1  //Compress to reduce B/W usage
#define USE_DEC_POST_BT 1  //NR/MAIN 8bit mode to reduce B/W usage
#else 												// MDIN-i51X (i510, i540, i550)
#define REV_WDR_NO_BUFF 1  //TM without frame buffer
#define REV_WDR_MANU_TM 1  //bug about TM max read
#define REV_ODM_LEV_FIX	1  //bug about odm_comp_mode 0x220[2:1]
#define REV_REG_MGR_MDS 1  //MGR motion data select
#define REV_x36_CLR_SPR 1  //Sony Built-in color suppress
#define REV_RB_SLOPE_EN 1  //WB new register rb_slope_en
#define REV_DES_DOL_ADJ 1  //Deserializer support sony DOL
#define REV_WDR_8_SPLIT 1  //WDR support 8-split mode
#define REV_WDR_NEW_MGR 1  //New Merge Algorithm
#define REV_WDR_PREV_WB 1  //Use WB'ed data for MGR input
#define REV_TM_DARK_CNT 1  //Use TM dark pixel counter
#define REV_ROT_FUNC_EN 1  //Share line-memory with Rotation Func.
#define USE_IPC_COMPRES 0  //Compress to reduce B/W usage
#define USE_DEC_POST_BT 0  //NR/MAIN 8bit mode to reduce B/W usage
#endif

// jins, i5xx chip dependent bugs/fixes
#if defined(__USE_MDIN_i500__)||\
	defined(__MDIN_i5XX_FPGA__)
#define CHIP_POWER_CTRL 0  // TM only mode, clock of borrowed sram
#define CHIP_TM_ADJ_GAP 0  // TM update interval(speed)
#define CHIP_DOL_PLL_IN 0  // DOL-3 cannot generate pclk using divider
#else
#define CHIP_POWER_CTRL 1  // TM only mode, clock of borrowed sram
#define CHIP_TM_ADJ_GAP 1  // TM update interval(speed)
#define CHIP_DOL_PLL_IN 1  // DOL-3 cannot generate pclk using divider
#endif

// -----------------------------------------------------------------------------
// Sensor/Board dependent
// -----------------------------------------------------------------------------

//iris control
#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
#define WDR_IRIS_CTRL_EN  1
#else
#define WDR_IRIS_CTRL_EN  0
#endif

//sensor resolution
#if defined(__USE_CMOS_IMX123__)||\
    defined(__USE_CMOS_IMX124__)||\
    defined(__USE_IMX265_1536P_ALL_SCAN_MODE__)
#define WDR_SENS_3M_RES  1
#define WDR_SENS_1M_RES  0
#elif defined(__USE_IMX290_720P30_MODE__)||\
    defined(__USE_CMOS_IMX273__)
#define WDR_SENS_3M_RES  0
#define WDR_SENS_1M_RES  1
#else  //1080P(2M) sensor
#define WDR_SENS_3M_RES  0
#define WDR_SENS_1M_RES  0
#endif

//sony built-in : analog 4.5dB fixed
#if defined(__USE_CMOS_IMXx36__)||\
    defined(__USE_CMOS_IMX123__)
#define HTM_FIXED_GAIN  1
#else
#define HTM_FIXED_GAIN  0
#endif

//use adaptive TM gain controls
#if REV_WDR_MANU_TM
#define ADAPT_TM_GAIN_EN  1  // use adaptive TM gain controls using long to short ratio
#define ADAPT_TM_SHORTER  0  // DOL3 mode - 0:use long/short, 1:use long/shorter
#endif

//control frame mode using FID
#if defined(__USE_CMOS_IMXx36__)||\
    defined(__USE_CMOS_IMX123__)||\
    defined(__USE_CMOS_IMX124__)||\
    defined(__USE_CMOS_IMX174__)||\
    defined(__USE_CMOS_IMX249__)||\
    defined(__USE_CMOS_IMX290__)||\
    defined(__USE_CMOS_IMX327__)||\
    defined(__USE_CMOS_IMX385__)||\
    defined(__USE_CMOS_IMX185__)||\
    defined(__USE_CMOS_IMX265__)||\
    defined(__USE_CMOS_IMX273__)
#define USE_FWDR_FID_EN  1
#else
#define USE_FWDR_FID_EN  0
#endif

//global shutter
#if defined(__USE_CMOS_IMX273__) || defined(__USE_CMOS_IMX265__)
#define USE_GLOBAL_SHUT  1
#else
#define USE_GLOBAL_SHUT  0
#endif

//start with max shutter due to avoid sync loss
#if defined(__USE_CMOS_MN34220__)
#define WDR_STR_MAX_SHT  1  //sensor readout time might be changed during operation
#else
#define WDR_STR_MAX_SHT  0
#endif

//for OSD, use edge enhancement
//instead of using deblur&deghost
#if REV_WDR_NEW_MGR
#define WDR_TM_SIZE_CTRL  1
#else
#define WDR_TM_SIZE_CTRL  0
#endif

//TM strength control
//0:use bayer gamma, 1:manual TM
#if REV_WDR_MANU_TM
#define WDR_USE_MANU_TM  1
#else
#define WDR_USE_MANU_TM  0
#endif

//data output timing exceeds 1V period
#if (defined(__USE_MDIN_i500__)&&defined(__USE_CMOS_MN34220__))
#define WDR_DOL_OVER_SYN  1
#else 										// MDIN-i51X (i510, i540, i550)
#define WDR_DOL_OVER_SYN  0
#endif

//ODM fifo and Bayer clock is separated.
//in case mn34220 and imx290, pixel rate is x4 faster while h-size is only x3 wider. (6600x1500 not 8800x1125)
//which means ODM fifo have to run as x4/3 faster than bayer logic.
#if defined(__USE_MDIN_i5XX_REV__)
#define WDR_SEPA_ODM_CLK  1
#else
#define WDR_SEPA_ODM_CLK  0
#endif

//fixed shutter exp.ratio (0:variable, 1:low-gap, 2:high-gap)
#if defined(__USE_MDIN_i500__)  //i500 does not support run time conversion of shutter ratio. (sync loss might be occured)
  #define L3WDR_HOLD_RATIO  1
  #define L2WDR_HOLD_RATIO  1
#elif defined(__USE_CMOS_IMX290__)
  #define L3WDR_HOLD_RATIO  0
  #define L2WDR_HOLD_RATIO  2  // shutter(short) limit is too bright if long-short ratio is 1:16
#elif defined(__USE_CMOS_IMX385__)
  #define L3WDR_HOLD_RATIO  2  // WDRCTRL_SetShutParams is not compatible
  #define L2WDR_HOLD_RATIO  2
#elif defined(__USE_CMOS_MN34220__)||\
      defined(__USE_CMOS_MN34420__)
  #define L3WDR_HOLD_RATIO  2  // not allowed due to prevent sync loss
  #define L2WDR_HOLD_RATIO  2
#else
  #define L3WDR_HOLD_RATIO  0
  #define L2WDR_HOLD_RATIO  0
#endif

//support 8ch DOL
#if defined(__USE_MDIN_i500__)||\
	defined(__USE_MDIN_i510__)||\
	defined(__MDIN_i5XX_FPGA__)
#define WDR_USE_8CH_DOL  0
#else 										// MDIN-i550, MDIN-i540
#define WDR_USE_8CH_DOL  1
#endif

//supported WDR result write bit-mode
//(depends on sensor h-active size, at least one must be enabled)
#if defined(__USE_SENS_HSYNC_EXT__)
  #define WDR_WRITE_EN_08B  1  // tot.hsize>=1952(2M)
  #define WDR_WRITE_EN_09B  1  // tot.hsize>=1932(2M)
  #define WDR_WRITE_EN_10B  1  // tot.hsize>=1944(2M)
  #define WDR_WRITE_EN_12B  1  // tot.hsize>=1940(2M)
#elif defined(__USE_IMX265_1536P_ALL_SCAN_MODE__)
  #define WDR_WRITE_EN_08B  0
  #define WDR_WRITE_EN_09B  0
  #define WDR_WRITE_EN_10B  1
  #define WDR_WRITE_EN_12B  0
#elif defined(__USE_CMOS_IMX174__)||\
	defined(__USE_IMX265_1080P60_MODE__)||defined(__USE_IMX265_1080P30_MODE__)||defined(__USE_IMX265_ROI_MODE__)||\
	defined(__USE_IMX273_ALLSCAN_MODE__)||\
	defined(__USE_CMOS_IMX249__)  //max.1936
  #define WDR_WRITE_EN_08B  0
  #define WDR_WRITE_EN_09B  1
  #define WDR_WRITE_EN_10B  0
  #define WDR_WRITE_EN_12B  0
#elif defined(__USE_CMOS_IMX290__)||\
      defined(__USE_CMOS_IMX327__)||\
      defined(__USE_CMOS_IMX385__)||\
      defined(__USE_CMOS_IMX185__)  //max.1945
  #define WDR_WRITE_EN_08B  0
  #define WDR_WRITE_EN_09B  1
  #define WDR_WRITE_EN_10B  1
  #define WDR_WRITE_EN_12B  1
#else
  #define WDR_WRITE_EN_08B  1
  #define WDR_WRITE_EN_09B  1
  #define WDR_WRITE_EN_10B  1
  #define WDR_WRITE_EN_12B  1
#endif

//supported WDR mode
#if defined(__USE_CMOS_MN34220__)
  #define WDR_SENS_DOL_3FRM  1
  #define WDR_SENS_DOL_2FRM  0
  #define WDR_SENS_BUILT_IN  0
  #define WDR_SENS_MULT_EXP  0
#elif defined(__USE_CMOS_MN34420__)
  #define WDR_SENS_DOL_3FRM  REV_WDR_NEW_MGR
  #define WDR_SENS_DOL_2FRM  0
  #define WDR_SENS_BUILT_IN  0
  #define WDR_SENS_MULT_EXP  0
#elif defined(__USE_CMOS_AR0331__)
  #define WDR_SENS_DOL_3FRM  0
  #define WDR_SENS_DOL_2FRM  0
  #define WDR_SENS_BUILT_IN  1
  #define WDR_SENS_MULT_EXP  0
#elif defined(__USE_CMOS_IMXx36__)
  #define WDR_SENS_DOL_3FRM  0
  #define WDR_SENS_DOL_2FRM  0
  #define WDR_SENS_BUILT_IN  1
  #define WDR_SENS_MULT_EXP  1
#elif defined(__USE_CMOS_IMX123__)
  #define WDR_SENS_DOL_3FRM  0
  #define WDR_SENS_DOL_2FRM  REV_WDR_8_SPLIT&&WDR_USE_8CH_DOL  //check MEM size
  #define WDR_SENS_BUILT_IN  REV_WDR_NO_BUFF
  #define WDR_SENS_MULT_EXP  REV_WDR_8_SPLIT  //check MEM size
#elif defined(__USE_CMOS_IMX124__)
  #define WDR_SENS_DOL_3FRM  0
  #define WDR_SENS_DOL_2FRM  0
  #define WDR_SENS_BUILT_IN  0
  #define WDR_SENS_MULT_EXP  REV_WDR_8_SPLIT  //support but not implemented
#elif defined(__USE_CMOS_IMX174__)||\
      defined(__USE_CMOS_IMX249__)||\
      defined(__USE_CMOS_IMX185__)||\
      defined(__USE_CMOS_IMX273__)||\
      defined(__USE_CMOS_IMX265__)
  #define WDR_SENS_DOL_3FRM  0
  #define WDR_SENS_DOL_2FRM  0
  #define WDR_SENS_BUILT_IN  0
  #define WDR_SENS_MULT_EXP  1
#elif defined(__USE_CMOS_IMX290__)||\
      defined(__USE_CMOS_IMX385__)
  #define WDR_SENS_DOL_3FRM  REV_DES_DOL_ADJ&&WDR_USE_8CH_DOL
  #define WDR_SENS_DOL_2FRM  0/*REV_DES_DOL_ADJ*/
  #define WDR_SENS_BUILT_IN  0
  #define WDR_SENS_MULT_EXP  1
#elif defined(__USE_CMOS_IMX327__)
  #define WDR_SENS_DOL_3FRM  0
  #define WDR_SENS_DOL_2FRM  0/*REV_DES_DOL_ADJ*/
  #define WDR_SENS_BUILT_IN  0
  #define WDR_SENS_MULT_EXP  1  
#else
  #define WDR_SENS_DOL_3FRM  0
  #define WDR_SENS_DOL_2FRM  0
  #define WDR_SENS_BUILT_IN  0
  #define WDR_SENS_MULT_EXP  0
#endif
#if defined(__USE_MDIN_i500__) 			// MDIN-i500
  #define WDR_SENS_BYPAS_IN  0
#else 									// MDIN-i51X (i510, i540, i550)
  #define WDR_SENS_BYPAS_IN  0
#endif

// jins, temp for EDK B'd
#if defined(__MDIN_i550_EDK_V10__)&&\
	defined(__USE_CMOS_IMX290__)
  #define TEMP_IMX290_EDK_V10_OPT  // edk option about imx290 lvds delay
  #undef WDR_SENS_BYPAS_IN
  #define WDR_SENS_BYPAS_IN 0  // undef DWDR
  #undef WDR_SENS_DOL_3FRM
  #define WDR_SENS_DOL_3FRM 0  // undef 3-WDR
#endif
#if defined(__MDIN_i550_EDK_V11__)&&\
	defined(__USE_CMOS_IMX290__)
  #undef WDR_SENS_BYPAS_IN
  #define WDR_SENS_BYPAS_IN 0  // undef DWDR
  #undef WDR_SENS_DOL_3FRM
  #define WDR_SENS_DOL_3FRM 0  // undef 3-WDR
#endif
#if defined(__MDIN_i550_REF__)&&\
	defined(__MDIN_i550_REF_V10__)&&\
	defined(__USE_CMOS_IMX290__)
  #define TEMP_IMX290_REF_OPT  // ref option about imx290 lvds delay
#endif
#if defined(__MDIN_i550_REF__)&&\
	defined(__MDIN_i550_REF_V10__)&&\
	defined(__USE_CMOS_IMX124__)&&\
	(!defined(__USE_CMOS_IMX123__))
  #define TEMP_IMX124_REF_OPT  // ref option about imx290 lvds delay
#endif

// wdr enable process flag
#if WDR_SENS_DOL_3FRM + \
	WDR_SENS_DOL_2FRM + \
	WDR_SENS_BUILT_IN + \
	WDR_SENS_MULT_EXP + \
	WDR_SENS_BYPAS_IN
#define	__USE_WDR_PROCESS__
#endif

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
typedef	enum {  //WDR processing mode
	MISP_WDR_PROC_DISBL=0,  // WDR off
	MISP_WDR_PROC_FRAME,    // Frame by Frame
	MISP_WDR_PROC_BUILT,    // Built-in (Tonemap only)
	MISP_WDR_PROC_SINGL,    // DWDR
	MISP_WDR_PROC_LINE2,    // Line by Line (2 Frame)
	MISP_WDR_PROC_LINE3,    // Line by Line (3 Frame)
	MISP_WDR_PROC_NUM
} MISP_WDR_PROC_t;

typedef	enum {  //WDR bypass
	MISP_WDR_FVIEW_WDR=0,  // WDR on (bypass off)
	MISP_WDR_FVIEW_LONG,   // Bypass Long Image
	MISP_WDR_FVIEW_SH_1,   // Bypass Short Image
	MISP_WDR_FVIEW_SH_2,   // Bypass Shorter Image
	MISP_WDR_FVIEW_NUM
} MISP_WDR_FVIEW_t;

typedef	enum {  //WDR auto control
	MISP_WDR_AE_MANU=0,  // AE manual
	MISP_WDR_AE_AUTO,    // AE auto
	MISP_WDR_AE_NUM
} MISP_WDR_AE_t;

typedef	enum {  //WDR compress mode
	MISP_COMP_OFF=0,  // Compress off
	MISP_COMP_ON,     // Compress on
	MISP_COMP_420,    // Compress 420 mode
	MISP_COMP_NUM
} MISP_COMP_MODE_t;

typedef	enum {  //WDR compress mode
	MISP_COMP_QUANT_80=0,  // Quantize 80%
	MISP_COMP_QUANT_60,    // Quantize 60%
	MISP_COMP_QUANT_NUM
} MISP_COMP_QUANT_t;

typedef	enum {  //WDR AE direction
	MISP_AE_DIR_INC=0,  // Increase brightness
	MISP_AE_DIR_DEC,    // Decrease brightness
	MISP_AE_DIR_NUM
} MISP_AE_DIR_t;

typedef	enum {  //WDR engineer functions
	MISP_WDR_TEST_SHUT_MODE=0,  //WDR exposure Shut/Iris
	MISP_WDR_TEST_DEBUG_MODE,   //WDR debug level
	MISP_WDR_TEST_HOLD_SS_WR,   //WDR hold sensor write
	MISP_WDR_TEST_FORCE_TM,     //WDR bypass TM
	MISP_WDR_TEST_INPUT_SHIFT,  //WDR input bit mode
	MISP_WDR_TEST_DISP_FLAG,    //WDR display flag
	MISP_WDR_TEST_HOLD_AE,      //WDR hold AE proc.
	MISP_WDR_TEST_HOLD_TM,      //WDR hold TM proc.
	MISP_WDR_TEST_CSP_EN,       //WDR enhanced algo.
	MISP_WDR_TEST_INPUT_BIT,    //WDR input bit-mode
	MISP_WDR_TEST_MERGE_NUM,    //WDR merge num
	MISP_WDR_TEST_CSP_USE_HIS,  //WDR CSP use motion history
	MISP_WDR_TEST_CSP_MD_THR,   //WDR CSP motion threshold
	MISP_WDR_TEST_HIS_MINUS,    //WDR CSP motion history minus
	MISP_WDR_TEST_WB_POS,       //WDR WB adjust position
	MISP_WDR_TEST_TOGGLE_CC,    //WDR toggle CC
	MISP_WDR_TEST_TM_LSIZE,     //WDR TM Local size
	MISP_WDR_TEST_TM_LGAIN,     //WDR TM Local gain
	MISP_WDR_TEST_MGR_BLEND,    //WDR MGR blend
	MISP_WDR_TEST_MGR_EXPAND,   //WDR MGR expand selection
	MISP_WDR_TEST_DISP_FRAME,   //WDR MGR use frame
	MISP_WDR_TEST_TM_OFFSET,    //WDR TM offset control
	MISP_WDR_TEST_NUM
} MISP_TEST_FUNC_t;

typedef	struct {  //WDR control structure
	LONG  ctrl;         // WDR control flag

	//process mode
	BYTE  state_wdr;    // current state for normal mode restoring
	BYTE  inp_shift;    // input data bitwise right shift

	//for Frame by Frame mode
	BYTE  fwdr_cntr;    // current fid
	BYTE  fwdr_fnum;    // max fid
	BYTE  fwdr_lfid;    // long frame fid
	BYTE  fwdr_init;    // long fid updated

	//for MGR
	BYTE  rate_long;    // exposure ratio long
	BYTE  rate_sh_1;    // exposure ratio sh_1
	BYTE  rate_sh_2;    // exposure ratio sh_2

	//counters
	BYTE  init_cntr;    // for initialize
	BYTE  stbl_wait;    // for AE (wait count for the slow step)
	BYTE  dark_cntr;    // for MGR (low light state counter)

	//for BLC
	WORD  mgr_offst;    // mgr offset   (reg.0x5A1)
	WORD  mgr_pxmax;    // mgr pixelmax (reg.0x5A2)

	//for AE
	WORD  cur_Y_sum;    // current Ysum value
	WORD  targt_min;    // AE target level (min)
	WORD  targt_mid;    // AE target level (cnt)
	WORD  targt_max;    // AE target level (max)

	//for Auto Saturation
	DWORD avg_Y_sum;    // average Ysum value
	BYTE  satur_min;    // min satur. level
	BYTE  satur_max;    // max satur. level
	BYTE  dummy_s00;    // dummy to align pointer
	BYTE  dummy_s01;    // dummy to align pointer

	//for TM
	DWORD tmr_avg_0;    // tm read : mean_0
	DWORD tmr_avg_1;    // tm read : mean_1
	DWORD tmr_max_0;    // tm read : max_0
	DWORD tmr_max_1;    // tm read : max_1
	DWORD tmi_avg_0;    // tm internal : mean_0
	DWORD tmi_avg_1;    // tm internal : mean_1
	DWORD tmi_max_0;    // tm internal : max_0
	DWORD tmi_max_1;    // tm internal : max_1
	DWORD tmw_avg_0;    // tm write : mean_0
	DWORD tmw_avg_1;    // tm write : mean_1
	DWORD tmw_max_0;    // tm write : max_0
	DWORD tmw_max_1;    // tm write : max_1

	//for TM (i500 - assume TM max with short input)
#if (!REV_WDR_MANU_TM)
	BYTE  tma_phase;    // assume phase
	BYTE  dummy_000;    // dummy to align pointer
	BYTE  dummy_001;    // dummy to align pointer
	BYTE  dummy_002;    // dummy to align pointer
	WORD  tma_level;    // count threshold
	WORD  tma_m_cnt;    // pixel count (over tma_level)
	WORD  tma_l_cnt;    // pixel count (over tma_level-1)
#endif

	//for TM (i51x - adaptive TM offset)
#if REV_WDR_MANU_TM
	BYTE  tma_phase;    // adjust phase
	BYTE  dummy_000;    // dummy to align pointer
	BYTE  dummy_001;    // dummy to align pointer
	BYTE  dummy_002;    // dummy to align pointer
	WORD  psum_long;    // wdr_satur_pxsum(long)
	WORD  psum_shrt;    // wdr_satur_pxsum(short(er))
	WORD  cur_ratio;    // curr. long-short Ysum ratio
	DWORD pre_ratio;    // prev. long-short Ysum ratio (Q6 format)
	WORD  tone_offs;    // tone map gain offset result
#endif

	//index based configure value
	BYTE  fview_sel;    // frame select index
	BYTE  bayr_gamm;    // (i500) bayer gamma index
	BYTE  mgr_md_th;    // (i500) MD threshold index
	BYTE  auto_mode;    // auto gain/exposure index
	BYTE  brightnes;    // auto target index
	BYTE  deghst_en;    // (i500) enalbe mgr deghost
	BYTE  comp_mode;    // (i500) compress mode
	BYTE  debug_lev;    // debug level via uart
	WORD  shut_hold;    // temporary store to avoid fid sync loss
	WORD  gain_ctrl;    // sensor gain index
	WORD  shut_ctrl;    // sensor shutter index
	WORD  iris_ctrl;    // lens iris index

	//control variables
	WORD  sns_again;    // cmos gain value (analog)
	WORD  sns_dgain;    // cmos gain value (digital)
	WORD  tone_gain;    // misp tone map gain value
	WORD  expos_min;    // base exposure min.
	WORD  expos_max;    // base exposure max.
	WORD  accm_long;    // accumulation line (exposure time) - (long)
	WORD  accm_sh_1;    // accumulation line (exposure time) - (short)
	WORD  accm_sh_2;    // accumulation line (exposure time) - (shorter)
	WORD  accm_sh_3;    // accumulation line (exposure time) - (shortest)
	WORD  readout_1;    // DOL readout timing (exposure time) - (short)
	WORD  readout_2;    // DOL readout timing (exposure time) - (shorter)
	WORD  lens_iris;    // misp lens iris gain
}	WDR_CTRL_PARAM, *PWDR_CTRL_PARAM;

typedef struct {  //WDR restore structure
	BYTE  data_en;    // stored data exist
	BYTE  dummy_0;    // dummy to align pointer
	BYTE  dummy_1;    // dummy to align pointer
	BYTE  dummy_2;    // dummy to align pointer
	LONG  ae_mode;    // ae process on/off
	LONG  ae_gain;    // ae gain value
	SHORT ae_shut;    // ae shutter value
	SHORT ae_iris;    // ae iris value
	BYTE  dummy_3;    // dummy to align pointer
	BYTE  dummy_4;    // dummy to align pointer
	BYTE  bl_type;    // BLC type (0:Front, 1:ODM)
	BYTE  bl_mode;    // BLC mode
	WORD  bl_mval;    // BLC manual target
	WORD  bl_oval;    // BLC ODM target
	BYTE  gm_bayr;    // gamma - bayer(front)
	BYTE  dummy_5;    // dummy to align pointer
#if USE_DEC_POST_BT
	BYTE  dnr_bit;    // NR bit mode
	BYTE  inp_bit;    // Main bit mode
#else
	BYTE  dummy_6;    // dummy to align pointer
	BYTE  dummy_7;    // dummy to align pointer
#endif
	BYTE  at_satu;    // AutoSat_Level
	BYTE  at_brig;    // AutoBright_Level
	BOOL  at_edge;    // AutoEdge_OnOff
	BOOL  at_cont;    // AutoCon_OnOff
} WDR_SAVE_PARAM, *PWDR_SAVE_PARAM;  //restore values for non-WDR mode


// WDR control flags
#define	WDRCTRL_TRIGGER_ISP   0x00000001  // Init ISP
#define WDRCTRL_PROCESS_ON    0x00000010  // WDR process on

#define WDRCTRL_MODE_MASK     0x00001F00  // WDR mode
#define WDRCTRL_MODE_FRAME    0x00000100  //  - Frame by Frame
#define WDRCTRL_MODE_BUILT    0x00000200  //  - Tone-map only
#define WDRCTRL_MODE_LINE2    0x00000400  //  - Line by Line (2Frame)
#define WDRCTRL_MODE_LINE3    0x00000800  //  - Line by Line (3Frame)
#define WDRCTRL_MODE_SINGL    0x00001000  //  - DWDR

#define WDRCTRL_AE_PROC_ON    0x00010000  // WDR AE enable
#define WDRCTRL_AE_IRIS_EN    0x00020000  // WDR AE - use iris, else use shutter
#define WDRCTRL_TM_CTRL_ON    0x00040000  // (i51x) ISP Controls TM level
#define WDRCTRL_TM_OFFS_ON    0x00080000  // (i51x) TM offset control
#define WDRCTRL_MD_CTRL_ON    0x00100000  // (i500) Deghost/Deblur control
#define WDRCTRL_AT_SATU_ON    0x00200000  // WDR - use auto saturation

#define WDRCTRL_IN_RES_MASK   0x0F000000  // Source (sensor write) bit resolution
#define WDRCTRL_IN_RES_12B    0x01000000  //  - 12bit write (built-in mode)
#define WDRCTRL_IN_RES_10B    0x02000000  //  - 10bit write (normal WDR)
#define WDRCTRL_IN_RES_09B    0x04000000  //  -  9bit write
#define WDRCTRL_IN_RES_08B    0x08000000  //  -  8bit write

#define WDRCTRL_SPLIT_MASK    0x30000000  // Split WDR input image
#define WDRCTRL_SPLIT_NUM4    0x10000000  //  - Split number 4
#define WDRCTRL_SPLIT_NUM8    0x20000000  //  - Split number 8

// debug print
#define WDR_DBG_PRINT    1

// control response speed (accumulation frame)
#define SHUT_ADJ_DLY    6  // shutter adjust delay
#define GAIN_ADJ_DLY    6  // gain adjust delay
#define IRIS_ADJ_DLY   14  // iris adjust delay

// AE step speed
#define AE_STEP_DIV     32   // step divider
#define AE_STEP_OFF     (AE_STEP_DIV-1)  // offset for round-up divide

// TM response speed
#if defined(CHIP_TM_ADJ_GAP)
#define TM_BLEND_DIV    (128/4)  // TM speed
#else
#define TM_BLEND_DIV    (128/8)  // TM speed (FPGA)
#endif

// Output CSC controls (auto saturation)
#define AE_BLEND_DIV    (128/4)  // Output CSC reponse speed
#define AUTO_SAT_MIN    16       // minimum saturation value

// threshold to determine low light condition
#define BLUR_MAX_CNT    63

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

//wdr process
void Init_WDR(void);
void WDR_ProcessHandler(void);

//api controls
BYTE SetWDR_ProcesMode(MISP_WDR_PROC_t mode);
BYTE SetWDR_BypassPath(MISP_WDR_FVIEW_t nID);
BYTE SetWDR_TmStrength(WORD nID);
BYTE SetWDR_TmContrast(WORD nID);  //(i51x)
BYTE SetWDR_MdRegister(WORD nID);  //(i500)
BYTE SetWDR_AutoExpose(WORD nID);
BYTE SetWDR_AutoTarget(WORD nID);
BYTE SetWDR_SensorGain(WORD nID);
BYTE SetWDR_SensorShut(WORD nID);
BYTE SetWDR_LensDcIris(WORD nID);
BYTE SetWDR_ExposRatio(WORD nID);
BYTE SetWDR_SaturLevel(BYTE val);
BYTE SetWDR_ProcessRst(void);
MISP_WDR_PROC_t GetWDR_ProcesMode(void);
MISP_WDR_PROC_t GetWDR_ProcesList(BYTE nID);
BOOL GetWDR_AEProcMode(void);

//drivers
WORD GetWDR_GainMin(void);
WORD GetWDR_GainMax(void);
WORD GetWDR_ShutMin(void);
WORD GetWDR_ShutMax(void);
WORD GetWDR_IrisMin(void);
WORD GetWDR_IrisMax(void);
BYTE GetWDR_RwBitMode(void);

//misc functions
BYTE SetWDR_DemoMode(BYTE mode, BYTE fsel);  //(i51x)
BOOL GetWDR_ODMValidFrame(void);

//compress (i500)
BYTE SetWDR_CompressCtrl(MISP_COMP_MODE_t mode);
MISP_COMP_MODE_t GetWDR_CompressCtrl(void);
BYTE SetWDR_CompressDefault(BOOL On);
BYTE SetWDR_CompressMode(WORD nID);
BOOL GetWDR_ComprState(void);

//test function
PBYTE SetWDR_TestFunc(BYTE nID);
PCHAR GetWdr_DebugStr(BYTE nID);

//for rotation setting
BYTE SetWDR_Rotation_mode(BYTE mode, WORD h_size, WORD v_size, BYTE bit_precision);

#endif  /* __MISP_WDR_H__ */
