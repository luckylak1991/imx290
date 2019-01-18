// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include    "misp100.h"
#include	"cmos_if.h"		// for sensor dependent option

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
//#define TEST_DUMP_TONE  //test dump built-in input
#if	defined(__MDIN_i5XX_FPGA__)
//#define TEST_60HZ_INPUT  //test 60Hz->30Hz WDR
#endif
//#define TEST_FWDR_RESET  //test ODM reset control

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static WDR_CTRL_PARAM stWDRCTRL;  // handler to wdr control
static WDR_SAVE_PARAM stWDRSAVE;  // store value to recover normal mode

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// include ODM variable
// ----------------------------------------------------------------------
#include    "ae.h"
#include    "awb.h"
extern AE_CTRL_PARAM stAECTRL;
extern WB_CTRL_PARAM stWBCTRL;
extern AE_XFER_PARAM stAEXFER;

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
#include    "af.h"
extern AF_CTRL_PARAM stAFCTRL;
#endif

// ----------------------------------------------------------------------
// External Functions
// ----------------------------------------------------------------------
extern void DEMO_SetAutoConOnOff(BOOL OnOff);
extern void DEMO_SetAutoSaturation(BYTE nID);
extern void DEMO_SetAutoEdgeOnOff(BOOL OnOff);
extern void DEMO_SetAutoBright(BYTE nID);
extern BOOL DEMO_GetAutoConOnOff(void);
extern BYTE DEMO_GetAutoSatLevel(void);
extern BOOL DEMO_GetAutoEdgeOnOff(void);
extern BYTE DEMO_GetAutoBright(void);

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
static void stDebug_Print(const BYTE debug_lev, const char* format, ...);


// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
//init parameter for WDR control
static void Init_WDR_PARAM(PWDR_CTRL_PARAM pCTRL) {
	pCTRL->init_cntr=0xFF;  //set MAX.value to discard boot-up initialize

	pCTRL->satur_min=AUTO_SAT_MIN;  //set WDR auto.satur range (AUTO_SAT_MIN ~ nID*10+4)
	pCTRL->satur_max=104;           //when nID=10
}


//--------------------------------------------------------------------------------------------------------------------------
//init WDR registers
static void Init_WDR_REGISTER(PWDR_CTRL_PARAM pCTRL) {
	BYTE comp_port_sel;
	BYTE comp_shift;

	MISP_RegWrite(MISP_LOCAL_ID, 0x5C3, 0x0015);  // WDR wdr_sep_vdpulse
	MISP_RegWrite(MISP_LOCAL_ID, 0x5C4, 0x0404);  // wdr_sep_hsync
	MISP_RegWrite(MISP_LOCAL_ID, 0x5C6, 0x0001);  // wdr_sep_vsync_rise
	MISP_RegWrite(MISP_LOCAL_ID, 0x5C7, 0x0003);  // wdr_sep_vsync_fall
	MISP_RegWrite(MISP_LOCAL_ID, 0x5CA, 0x0101);  // wdr_total_hsync
	MISP_RegWrite(MISP_LOCAL_ID, 0x5CC, 0x2020);  // r_wdr_tot_v_rise/fall
	MISP_RegWrite(MISP_LOCAL_ID, 0x5CD, 0x0001);  // wdr_tot_vsync_rise
	MISP_RegWrite(MISP_LOCAL_ID, 0x5CE, 0x0001);  // wdr_tot_vsync_fall

	MISP_RegField(MISP_LOCAL_ID, 0x59C, 0, 2, 0);  // wdr_mergenum(0)
	MISP_RegField(MISP_LOCAL_ID, 0x59C,14, 1, 0);  // r_mvfr_fifo_ctrl_sel(0)
	MISP_RegField(MISP_LOCAL_ID, 0x59D, 8, 7, 0);  // wdr_linemem_en(0), wdr_process_en(0), wdr_proc_frame_en(0000)
	MISP_RegField(MISP_LOCAL_ID, 0x5DC,15, 1, 0);  // mgr_motiondetect_en
	MISP_RegField(MISP_LOCAL_ID, 0x5DC,14, 1, 0);  // mgr_ctrlsel
#if WDR_DOL_OVER_SYN  //tripple buffering
	MISP_RegField(MISP_LOCAL_ID, 0x5C0,14, 1, 0);  // r_wdr_dol_fid_mode (tripple)
#else
	MISP_RegField(MISP_LOCAL_ID, 0x5C0,14, 1, 1);  // r_wdr_dol_fid_mode (double)
#endif
#if WDR_USE_MANU_TM
	MISP_RegField(MISP_LOCAL_ID, 0x5E7, 1, 3, 0);  // tm_ave_fnum (0:2, 1:4, 2:8, 3:16, 4:32 frame)
	MISP_RegField(MISP_LOCAL_ID, 0x5E7, 0, 1, 1);  // tm_ctrl (0:auto, 1:manual)
#else
	MISP_RegField(MISP_LOCAL_ID, 0x5E7, 1, 3, 4);  // tm_ave_fnum (0:2, 1:4, 2:8, 3:16, 4:32 frame)
	MISP_RegField(MISP_LOCAL_ID, 0x5E7, 0, 1, 0);  // tm_ctrl (0:auto, 1:manual)
#endif
#if WDR_TM_SIZE_CTRL  //tm_mode=0
	MISP_RegField(MISP_LOCAL_ID, 0x7FE, 8, 8,32);  // tm_gain0 (Q4 format)
	MISP_RegField(MISP_LOCAL_ID, 0x7FE, 0, 8,80);  // tm_gain1 (Q4 format)
#else
	MISP_RegField(MISP_LOCAL_ID, 0x7FE, 8, 8,16);  // tm_gain0 (Q4 format)
	MISP_RegField(MISP_LOCAL_ID, 0x7FE, 0, 8,40);  // tm_gain1 (Q4 format)
#endif

	//wb_en, wb_bayersel_e, wb_bayersel_o
	switch(GetSS_bayer_data_order()) {
		case MISP_SS_BAYER_PATTERN_0:  MISP_RegWrite(MISP_LOCAL_ID, 0x5AB, 1<<4|2<<2|2);  break;  // B, Gb, Gr, R
		case MISP_SS_BAYER_PATTERN_1:  MISP_RegWrite(MISP_LOCAL_ID, 0x5AB, 1<<4|3<<2|3);  break;  // Gb, B, R, Gr
		case MISP_SS_BAYER_PATTERN_2:  MISP_RegWrite(MISP_LOCAL_ID, 0x5AB, 1<<4|0<<2|0);  break;  // Gr, R, B, Gb
		default:                       MISP_RegWrite(MISP_LOCAL_ID, 0x5AB, 1<<4|1<<2|1);  break;  // R, Gr, Gb, B
	}

	//htm compress
	comp_port_sel=3;
#if defined(__USE_IMX123_HTM_X32__)
	comp_shift=1;  //use LSB 16bit of 17bit built-in input
#else
	comp_shift=0;
#endif
	MISP_RegWrite(MISP_LOCAL_ID, 0x5D3, GetSS_decomp_p1());        // comp_p1
	MISP_RegWrite(MISP_LOCAL_ID, 0x5D4, GetSS_decomp_p2());        // comp_p2
	MISP_RegWrite(MISP_LOCAL_ID, 0x5D5, GetSS_offset_p2());        // comp_p2_offset
	MISP_RegWrite(MISP_LOCAL_ID, 0x5D6, GetSS_decomp_p3());        // comp_p3
	MISP_RegWrite(MISP_LOCAL_ID, 0x5D7, GetSS_offset_p3());        // comp_p3_offset
	MISP_RegWrite(MISP_LOCAL_ID, 0x5D8, (comp_shift<<14)|(comp_port_sel<<12)|(GetSS_p1_mul_sel()<<8)|(GetSS_p2_mul_sel()<<4)|(GetSS_p3_mul_sel()));
	MISP_RegWrite(MISP_LOCAL_ID, 0x5D9, GetSS_bl_pre_value());     // bl_pre_in
	MISP_RegWrite(MISP_LOCAL_ID, 0x5DA, GetSS_bl_post_value());    // bl_post_in
	MISP_RegWrite(MISP_LOCAL_ID, 0x5DB, GetSS_input_max_value());  // max_in_cut

	//wdr_satur_green
	MISP_RegField(MISP_LOCAL_ID, 0x5A8, 14,1,1);

	//initial value about WB(WBCTRL_SetCOREGain)
#if 1
	MISP_RegField(MISP_LOCAL_ID, 0x5ac, 0,4, 2);  // wb_coefr  (x2.0)
	MISP_RegWrite(MISP_LOCAL_ID, 0x5ad, 0x0000);
	MISP_RegField(MISP_LOCAL_ID, 0x5ae, 0,4, 1);  // wb_coefgr (x1.0)
	MISP_RegWrite(MISP_LOCAL_ID, 0x5af, 0x0000);
	MISP_RegField(MISP_LOCAL_ID, 0x5b0, 0,4, 1);  // wb_coefgb (x1.0)
	MISP_RegWrite(MISP_LOCAL_ID, 0x5b1, 0x0000);
	MISP_RegField(MISP_LOCAL_ID, 0x5b2, 0,4, 2);  // wb_coefb  (x2.0)
	MISP_RegWrite(MISP_LOCAL_ID, 0x5b3, 0x0000);

	MISP_RegWrite(MISP_LOCAL_ID, 0x5f7, 0x0000);  // wdr_rb_comp_jeol_r
	MISP_RegWrite(MISP_LOCAL_ID, 0x5f8, 0x0000);
	MISP_RegWrite(MISP_LOCAL_ID, 0x5f9, 0x0000);  // wdr_rb_comp_jeol_b
	MISP_RegWrite(MISP_LOCAL_ID, 0x5fa, 0x0000);
#endif

#if REV_x36_CLR_SPR
	//color artifact range
	MISP_RegWrite(MISP_LOCAL_ID, 0x5DD, 0x0BB8);  // supr_satur_btm (3000)  //magenta range
	MISP_RegWrite(MISP_LOCAL_ID, 0x5DE, 0x12C0);  // supr_satur_top (4800)
	MISP_RegWrite(MISP_LOCAL_ID, 0x7F0, 0x0FA0);  // supr_comp_long (4000)  //green, yellow, cyan
	MISP_RegWrite(MISP_LOCAL_ID, 0x7F1, 0x1770);  // supr_comp_shrt (6000)

	//count threshold for 3x3 window
	MISP_RegField(MISP_LOCAL_ID, 0x7F2, 8, 3, 2);  // supr_satur_cnt
	MISP_RegField(MISP_LOCAL_ID, 0x7F2, 4, 4, 3);  // supr_major_cnt
	MISP_RegField(MISP_LOCAL_ID, 0x7F2, 0, 4, 2);  // supr_minor_cnt
#endif

	//init new mgr registers
#if REV_WDR_NEW_MGR
	//registers about csp control
  #if REV_WDR_PREV_WB
	MISP_RegField(MISP_LOCAL_ID, 0x5AB, 4, 1, 0);  // wb_en
	MISP_RegField(MISP_LOCAL_ID, 0x43B, 5, 1, 0);  // r_wdr_mo_flag_en
	MISP_RegField(MISP_LOCAL_ID, 0x43B, 7, 1, 0);  // r_wdr_csp_sat_en
  #else
	MISP_RegField(MISP_LOCAL_ID, 0x5AB, 4, 1, 1);  // wb_en
	MISP_RegField(MISP_LOCAL_ID, 0x43B, 5, 1, 1);  // r_wdr_mo_flag_en
	MISP_RegField(MISP_LOCAL_ID, 0x43B, 7, 1, 1);  // r_wdr_csp_sat_en
  #endif
	MISP_RegField(MISP_LOCAL_ID, 0x43B, 2, 2, 0);  // r_demo_mode
	MISP_RegField(MISP_LOCAL_ID, 0x43B, 4, 1, 1);  // r_wdr_mo_diff_en
	MISP_RegField(MISP_LOCAL_ID, 0x43B, 6, 1, 1);  // r_wdr_csp_mix_en
	MISP_RegField(MISP_LOCAL_ID, 0x43B, 8, 3, 3);  // r_csp_med_flt_thr
	MISP_RegField(MISP_LOCAL_ID, 0x43B,11, 1, 1);  // r_csp_med_flt_en
	MISP_RegField(MISP_LOCAL_ID, 0x43B,12, 3, 1);  // r_csp_exp_flt_thr
	MISP_RegField(MISP_LOCAL_ID, 0x43B,15, 1, 1);  // r_csp_exp_flt_en

	//registers about window count
	MISP_RegField(MISP_LOCAL_ID, 0x43C, 0, 4,10);  // r_cnt_md_diff
	MISP_RegField(MISP_LOCAL_ID, 0x43C, 4, 4, 7);  // r_cnt_invalid
	MISP_RegField(MISP_LOCAL_ID, 0x43C, 8, 4, 7);  // r_cnt_bad_sat
	MISP_RegField(MISP_LOCAL_ID, 0x43C,12, 1, 1);  // r_csp_motion_use
	MISP_RegField(MISP_LOCAL_ID, 0x43C,13, 1, 1);  // r_wdr_csp_en
	MISP_RegField(MISP_LOCAL_ID, 0x43C,14, 2, 0);  // r_blend_csp_data
	MISP_RegField(MISP_LOCAL_ID, 0x43D, 0, 4, 2);  // r_cnt_mix_min
	MISP_RegField(MISP_LOCAL_ID, 0x43D, 4, 4, 8);  // r_cnt_mix_max
	MISP_RegField(MISP_LOCAL_ID, 0x43D, 8, 4,10);  // r_cnt_cmp_use

	//registers about under/over exp.
	MISP_RegField(MISP_LOCAL_ID, 0x437,12, 1, 0);  // r_comp_chk_sat
	MISP_RegField(MISP_LOCAL_ID, 0x438, 0, 8,127); // r_bias_long_over
	MISP_RegField(MISP_LOCAL_ID, 0x438, 8, 8,15);  // r_bias_sh_2_undr
	MISP_RegField(MISP_LOCAL_ID, 0x439, 0,12,32);  // r_md_thres_off
	MISP_RegField(MISP_LOCAL_ID, 0x439,12, 2, 0);  // r_blend_cmp_data
	MISP_RegField(MISP_LOCAL_ID, 0x439,14, 1, 1);  // r_comp_by_short
	MISP_RegField(MISP_LOCAL_ID, 0x439,15, 1, 0);  // r_comp_sel_exp
#endif
#if REV_TM_DARK_CNT
	MISP_RegField(MISP_LOCAL_ID, 0x43E,12, 1, 0);  // tm_dark_polar (count darker under threshold)
	MISP_RegField(MISP_LOCAL_ID, 0x43E, 0,12,255); // tm_dark_level (12bit/8)
#endif
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//WDR initialize
void Init_WDR(void) {
	memset(&stWDRCTRL, 0, sizeof(WDR_CTRL_PARAM));
	memset(&stWDRSAVE, 0, sizeof(WDR_SAVE_PARAM));
	Init_WDR_PARAM(&stWDRCTRL);
	Init_WDR_REGISTER(&stWDRCTRL);

#if USE_IPC_COMPRES  //init compress registers
	SetWDR_CompressDefault(OFF);  //force_def_quant
	SetWDR_CompressMode(MISP_COMP_QUANT_80);  //80%
#endif
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//get sensor(source) write bit mode (convert mispwdr.c type to mispmemory_map.c)
static BYTE WDRCTRL_GetSwBitMode(PWDR_CTRL_PARAM pCTRL) {
	BYTE mode;
	switch(pCTRL->ctrl&WDRCTRL_IN_RES_MASK) {
		case WDRCTRL_IN_RES_12B:  mode=(BYTE)MISP_DDRMAP_12b;  break;
		case WDRCTRL_IN_RES_10B:  mode=(BYTE)MISP_DDRMAP_10b;  break;
		case WDRCTRL_IN_RES_09B:  mode=(BYTE)MISP_DDRMAP_9b;   break;
		case WDRCTRL_IN_RES_08B:  mode=(BYTE)MISP_DDRMAP_8b;   break;
		default:                  mode=(BYTE)MISP_DDRMAP_10b;  break;
	}
	return mode;
}


//--------------------------------------------------------------------------------------------------------------------------
//if wdr is off, seize clock to save power comsumption
static void WDRCTRL_SetPowerSave(PWDR_CTRL_PARAM pCTRL) {
	//reg.033h => 0:power save, 1:enable
#if CHIP_POWER_CTRL
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:  // Frame by Frame
		case WDRCTRL_MODE_LINE2:  // Line by Line (2Frame)
		case WDRCTRL_MODE_LINE3:  // Line by Line (3Frame)
			SetCLK_ClkWDRRdOnOff(1);
			SetCLK_ClkWDROnOff(1);
			SetCLK_ClkMGROnOff(1);
			SetCLK_ClkTMOnOff(1);
			break;
		case WDRCTRL_MODE_BUILT:  // Tone-map only
		case WDRCTRL_MODE_SINGL:  // DWDR
			SetCLK_ClkWDRRdOnOff(0);
			SetCLK_ClkWDROnOff(1);
			SetCLK_ClkMGROnOff(1);
			SetCLK_ClkTMOnOff(1);
			break;
		default:
			SetCLK_ClkWDRRdOnOff(0);
			SetCLK_ClkWDROnOff(0);
			SetCLK_ClkMGROnOff(0);
			SetCLK_ClkTMOnOff(0);
			break;
	}
#else
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:  // Frame by Frame
		case WDRCTRL_MODE_LINE2:  // Line by Line (2Frame)
		case WDRCTRL_MODE_LINE3:  // Line by Line (3Frame)
			SetCLK_ClkWDROnOff(1);
			SetCLK_ClkMGROnOff(1);
			SetCLK_ClkTMOnOff (1);
			break;
		case WDRCTRL_MODE_BUILT:  // Tone-map only
		case WDRCTRL_MODE_SINGL:  // DWDR
			SetCLK_ClkWDROnOff(1);
			SetCLK_ClkMGROnOff(0);
			SetCLK_ClkTMOnOff (1);
			break;
		default:
			SetCLK_ClkWDROnOff(0);
			SetCLK_ClkMGROnOff(0);
			SetCLK_ClkTMOnOff (0);
			break;
	}
#endif
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//remap memory map about wdr source write
static void WDRCTRL_SetMemoryMap(PWDR_CTRL_PARAM pCTRL) {
	BYTE sw_bit_mode=WDRCTRL_GetSwBitMode(pCTRL);
	BYTE rw_bit_mode=GetWDR_RwBitMode();

	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:  SetDDRMAP_WDR(2, 2, sw_bit_mode, rw_bit_mode);  break;
#if (!REV_WDR_NO_BUFF)||defined(TEST_DUMP_TONE)
		case WDRCTRL_MODE_BUILT:  SetDDRMAP_WDR(2, 1, sw_bit_mode, rw_bit_mode);  break;
		case WDRCTRL_MODE_SINGL:  SetDDRMAP_WDR(2, 1, sw_bit_mode, rw_bit_mode);  break;
#endif
#if WDR_DOL_OVER_SYN  //tripple buffering
		case WDRCTRL_MODE_LINE2:  SetDDRMAP_WDR(3, 2, sw_bit_mode, rw_bit_mode);  break;
		case WDRCTRL_MODE_LINE3:  SetDDRMAP_WDR(3, 3, sw_bit_mode, rw_bit_mode);  break;
#else
		case WDRCTRL_MODE_LINE2:  SetDDRMAP_WDR(2, 2, sw_bit_mode, rw_bit_mode);  break;
		case WDRCTRL_MODE_LINE3:  SetDDRMAP_WDR(2, 3, sw_bit_mode, rw_bit_mode);  break;
#endif
		default:                  SetDDRMAP_WDR(0, 1, sw_bit_mode, rw_bit_mode);  break;
	}
}


//--------------------------------------------------------------------------------------------------------------------------
//decompress sensor built-in data (12->16bit)
static void WDRCTRL_SetHtmDecomp(PWDR_CTRL_PARAM pCTRL) {
	if(pCTRL->ctrl&WDRCTRL_MODE_BUILT)
		SetSS_decomp_enable(1);  // ODM decomp setting
	else
		SetSS_decomp_enable(0);  // disable

	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//set deserializer
static void WDRCTRL_SetDeserializer(PWDR_CTRL_PARAM pCTRL) {
	if(	(pCTRL->ctrl&WDRCTRL_MODE_LINE2)||
		(pCTRL->ctrl&WDRCTRL_MODE_LINE3) )  {  //line by line wdr
#if (!WDR_SENS_DOL_3FRM)&&(!WDR_SENS_DOL_2FRM)
		/* not a dol sensor */
#elif defined(__USE_CMOS_MN34220__)
		SetSS_deserializer_mn34220_s6();
#elif defined(__USE_CMOS_MN34420__)
		SetSS_deserializer_mn34420_s6();
#elif defined(__USE_CMOS_IMX123__)
		SetSS_deserializer_imx124_s8();
#elif defined(__USE_CMOS_IMX124__)
		SetSS_deserializer_imx124_s4();
#elif defined(__USE_CMOS_IMX290__)
  #if WDR_USE_8CH_DOL
	  if(pCTRL->ctrl&WDRCTRL_MODE_LINE3)
		SetSS_deserializer_imx290_s8();
	  else
  #endif
		SetSS_deserializer_imx290_s4();
#elif defined(__USE_CMOS_IMX385__)
  #if WDR_USE_8CH_DOL
	  if(pCTRL->ctrl&WDRCTRL_MODE_LINE3)
		SetSS_deserializer_imx385_s8();
	  else
  #endif
		SetSS_deserializer_imx385_s4();
#elif defined(__USE_CMOS_IMX265__)
		SetSS_deserializer_imx265_s4();
#elif defined(__USE_CMOS_IMX273__)
	#if defined(__USE_CMOS_IMX273_LVDS_S8__)
		SetSS_deserializer_imx273_s8();
	#else
		SetSS_deserializer_imx273_s4();
	#endif
#endif
	}
	else {
#if   defined(__USE_CMOS_MN34220__)
		SetSS_deserializer_mn34220_s4();
#elif defined(__USE_CMOS_MN34420__)
		SetSS_deserializer_mn34420_s4();
#elif defined(__USE_CMOS_IMX123__)
  #if defined(__USE_CMOS_IMX123_LVDS_S8__)
		SetSS_deserializer_imx124_s8();
  #else
		SetSS_deserializer_imx124_s4();
  #endif
#elif defined(__USE_CMOS_IMX124__)
		SetSS_deserializer_imx124_s4();
#elif defined(__USE_CMOS_IMX290__)
  #if defined(__USE_CMOS_IMX290_LVDS_S8__)
		SetSS_deserializer_imx290_s8();
  #else
		SetSS_deserializer_imx290_s4();
  #endif
#elif defined(__USE_CMOS_IMX385__)
  #if defined(__USE_CMOS_IMX385_LVDS_S8__)
		SetSS_deserializer_imx385_s8();
  #else
		SetSS_deserializer_imx385_s4();
  #endif
#elif defined(__USE_CMOS_IMX265__)
		SetSS_deserializer_imx265_s4();
#elif defined(__USE_CMOS_IMX273__)
	#if defined(__USE_CMOS_IMX273_LVDS_S8__)
		SetSS_deserializer_imx273_s8();
	#else
		SetSS_deserializer_imx273_s4();
	#endif
#endif
	}
	SSCTRL_DesErrCorrection();
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//set clock configurations
static void WDRCTRL_SetHostClk(PWDR_CTRL_PARAM pCTRL) {
	// defalut pclk source - check temp code in mispclk.c
  #if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	#if defined(__USE_CMOS_IMX290_LVDS_S8__)
	WORD clk_sensor_in_sel=2;
	#else
	WORD clk_sensor_in_sel=3;
	#endif
  #else
	WORD clk_sensor_in_sel=2;
  #endif	

	// change pclk source for the modes cannot generate pclk using divider
#if WDR_USE_8CH_DOL
  #if defined(__USE_CMOS_IMX290__)||\
  	  defined(__USE_CMOS_IMX385__)
	if(pCTRL->ctrl&WDRCTRL_MODE_LINE3)  clk_sensor_in_sel=2;
  #elif defined(__USE_CMOS_IMX123__)
	if(pCTRL->ctrl&WDRCTRL_MODE_LINE2)  clk_sensor_in_sel=2;
  #endif
#endif

	//set sensor pll (refer to mispclk.c) - sensor to pixel clock
#if defined(__USE_CMOS_MN34220__)||\
    defined(__USE_CMOS_MN34420__)
	if(	(pCTRL->ctrl&WDRCTRL_MODE_LINE2)||
		(pCTRL->ctrl&WDRCTRL_MODE_LINE3) )
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,4,4);  // 1:1  6ch
		#else 								// MDIN-i51X (i510, i540, i550)
			SetCLK_ClkSensorInDivider(0);
//			SetCLK_UserPLL(1,1,24,3,3) ;	// 27Mhz -> 27Mhz
		#endif
	else
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,4,6);  // 2/3  4ch
		#else 								// MDIN-i51X (i510, i540, i550)
			SetCLK_ClkSensorInDivider(3);
//			SetCLK_UserPLL(1,3,64,3,4) ;	// 27Mhz -> 18Mhz
		#endif

#elif defined(__USE_CMOS_IMX123__)
	if(	(pCTRL->ctrl&WDRCTRL_MODE_LINE2)||
		(pCTRL->ctrl&WDRCTRL_MODE_LINE3) )
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,8,6);  // 4/3  8ch
		#else 								// MDIN-i51X (i510, i540, i550)
			if(clk_sensor_in_sel==2)  SetCLK_UserPLL(1,3,16,2,1) ;	// 27Mhz -> 36Mhz
			else                      SetCLK_UserPLL(1,3,64,3,2) ;	// 27Mhz -> 36Mhz
		#endif
	else
  #if defined(__USE_CMOS_IMX123_LVDS_S8__)
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,8,6);  // 4/3  8ch
		#else 								// MDIN-i51X (i510, i540, i550)
			if(clk_sensor_in_sel==2)  SetCLK_UserPLL(1,3,16,2,1) ;	// 27Mhz -> 36Mhz
			else                      SetCLK_UserPLL(1,3,64,3,2) ;	// 27Mhz -> 36Mhz
		#endif
  #else
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,4,6);  // 2/3  4ch
		#else 								// MDIN-i51X (i510, i540, i550)
			if(clk_sensor_in_sel==2)  SetCLK_UserPLL(1,3, 8,2,1) ;	// 27Mhz -> 18Mhz
			else                      SetCLK_UserPLL(1,3,64,3,4) ;	// 27Mhz -> 18Mhz
		#endif
  #endif

#elif defined(__USE_CMOS_IMX290__)
  #if WDR_USE_8CH_DOL
	if(pCTRL->ctrl&WDRCTRL_MODE_LINE3)
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,8,6);  // 8ch, 10bit (when des_clk_conv_enb='1')
		#else 								// MDIN-i51X (i510, i540, i550)
		#if defined(__USE_ISP_SCLK_OUT__)
			{
				MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x2021);	// user pll clk div 8 
				SetCLK_UserPLL(1,3,132,2,1) ;		// 27M(user pll source = xtal, 297Mhz = 27*132/3/(2^2) ) -> 148.5 / 4 -> 37.125Mhz
			}
		#else		
			if(clk_sensor_in_sel==2)  SetCLK_UserPLL(1,3,16,2,1) ;	// 27Mhz -> 36Mhz
			else                      SetCLK_UserPLL(1,3,64,3,2) ;	// 27Mhz -> 36Mhz
		#endif	//__USE_ISP_SCLK_OUT__
		#endif
	else
  #endif		//WDR_USE_8CH_DOL
  #if defined(__USE_CMOS_IMX290_LVDS_S8__)
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,8,6);
		#else 								// MDIN-i51X (i510, i540, i550)
		#if defined(__USE_ISP_SCLK_OUT__)
			{
				#if defined(__USE_IMX290_1080P120_MODE__) ||defined(__USE_IMX290_720P120_ISPCROP__)
					if(pCTRL->ctrl&WDRCTRL_MODE_FRAME){
						MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x1021);	// user pll clk div 4
						SetCLK_UserPLL(1,3,66,2,1) ;		// 27M(user pll source = xtal, 148.5Mhz = 27*66/3/(2^2) ) -> 148.5 / 4 -> 37.125Mhz
					}
					else{
						MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x2021);	// user pll clk div 8 
						SetCLK_UserPLL(1,3,132,2,1) ;		// 27M(user pll source = xtal, 297Mhz = 27*132/3/(2^2) ) -> 297 / 8 -> 37.125Mhz
					}
				#else				
				MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x1021);	// user pll clk div 4 
				SetCLK_UserPLL(1,3,66,2,1) ;		// 27M(user pll source = xtal, 148.5Mhz = 27*66/3/(2^2) ) -> 148.5 / 4 -> 37.125Mhz
				#endif
			}
		#else	
			{
					if(clk_sensor_in_sel==2)  SetCLK_UserPLL(1,3,16,2,1) ;	// 27Mhz -> 36Mhz
					else                      SetCLK_UserPLL(1,3,64,3,2) ;	// 27Mhz -> 36Mhz			
			}
		#endif
		#endif
  #else
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,4,6);  // 4ch, 12bit
		#else 								// MDIN-i51X (i510, i540, i550)
		#if defined(__USE_ISP_SCLK_OUT__)
			{
				MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x1021);	// user pll clk div 4 
				SetCLK_UserPLL(1,3,66,2,1) ;		// 27M(user pll source = xtal, 148.5Mhz = 27*66/3/(2^2) ) -> 148.5 / 4 -> 37.125Mhz
			}
		#else
			if(clk_sensor_in_sel==2)  SetCLK_UserPLL(1,3, 8,2,1) ;	// 27Mhz -> 18Mhz
			else                      SetCLK_UserPLL(1,3,64,3,4) ;	// 27Mhz -> 18Mhz
		#endif
		#endif
  #endif

#elif defined(__USE_CMOS_IMX385__)
  #if WDR_USE_8CH_DOL
	if(pCTRL->ctrl&WDRCTRL_MODE_LINE3)
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,8,6);  // 8ch, 10bit (when des_clk_conv_enb='1')
		#else 								// MDIN-i51X (i510, i540, i550)
			if(clk_sensor_in_sel==2)  SetCLK_UserPLL(1,5,16,1,1) ;	// 27Mhz -> 36Mhz
			else                      SetCLK_UserPLL(1,3,64,3,2) ;	// 27Mhz -> 36Mhz
		#endif
	else
  #endif
  #if defined(__USE_CMOS_IMX385_LVDS_S8__)
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,8,6);
		#else 								// MDIN-i51X (i510, i540, i550)
			if(clk_sensor_in_sel==2)  SetCLK_UserPLL(1,3,16,2,1) ;	// 27Mhz -> 36Mhz
			else                      SetCLK_UserPLL(1,3,64,3,2) ;	// 27Mhz -> 36Mhz
		#endif
  #else
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,4,6);  // 4ch, 12bit
		#else 								// MDIN-i51X (i510, i540, i550)
			if(clk_sensor_in_sel==2)  SetCLK_UserPLL(1,3, 8,2,1) ;	// 27Mhz -> 18Mhz
			else                      SetCLK_UserPLL(1,3,64,3,4) ;	// 27Mhz -> 18Mhz
		#endif
  #endif
  
#elif defined(__USE_CMOS_IMX273__)	// MDIN-i51X (i510, i540, i550)
		#if defined(__USE_ISP_SCLK_OUT__)
			{
			#if defined(__OUT_VCLKPLL_LOCK__)
				#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
				MISP_RegField(MISP_HOST_ID,  0x02F, 0, 4, 14);	//from 74.25 x 7 vpll to 37.125 user clock out Mhz
				#else
				MISP_RegField(MISP_HOST_ID,  0x02F, 0, 4, 2);	//from 74.25 vpll to 37.125 user clock out Mhz
				#endif
			#else
				MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x1021);	// user pll clk div 4
				SetCLK_UserPLL(1,3,66,2,1) ;		// 27M(user pll source = xtal, 148.5Mhz = 27*66/3/(2^2) ) -> 148.5 / 4 -> 37.125Mhz
			#endif
			}
		#else	
			{
				if(clk_sensor_in_sel==2)  SetCLK_UserPLL(1,3,16,2,1) ;	// 27Mhz -> 36Mhz
				else                      SetCLK_UserPLL(1,3,64,3,2) ;	// 27Mhz -> 36Mhz			
			}
		#endif

#elif defined(__USE_CMOS_IMX265__)	// MDIN-i51X (i510, i540, i550)
		#if defined(__USE_ISP_SCLK_OUT__)
			{
				MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x1021);	// user pll clk div 4
				SetCLK_UserPLL(1,3,66,2,1) ;		// 27M(user pll source = xtal, 148.5Mhz = 27*66/3/(2^2) ) -> 148.5 / 4 -> 37.125Mhz
			}
		#else	
			{
				if(clk_sensor_in_sel==2)  SetCLK_UserPLL(1,3,16,2,1) ;	// 27Mhz -> 36Mhz
				else                      SetCLK_UserPLL(1,3,64,3,2) ;	// 27Mhz -> 36Mhz			
			}
		#endif	
#endif

#if CHIP_DOL_PLL_IN
	SetCLK_SrcSensor(clk_sensor_in_sel);
#endif


#if defined(TEMP_IMX290_EDK_OPT)
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:
		case WDRCTRL_MODE_LINE2:  // invert, 3 delay
			SetSS_lvds_in_delay(1, 3);	// invert, 3 delay
			//MISP_RegWrite(MISP_HOST_ID, 0x01C, 0xffff);  // lvds data delay1
			//MISP_RegWrite(MISP_HOST_ID, 0x01D, 0x00ff);  // lvds data delay2
			break;
		default:  // invert, no delay
			SetSS_lvds_in_delay(1, 0);	// invert, no delay
			//MISP_RegWrite(MISP_HOST_ID, 0x01C, 0x4924);  // lvds data delay1
			//MISP_RegWrite(MISP_HOST_ID, 0x01D, 0x0092);  // lvds data delay2
			break;
	}
#elif defined(TEMP_IMX290_REF_OPT)
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_LINE3:  // invert, 3 delay
			SetSS_lvds_in_delay(1, 3);	// invert, 3 delay
			//MISP_RegWrite(MISP_HOST_ID, 0x01C, 0xffff);  // lvds data delay1
			//MISP_RegWrite(MISP_HOST_ID, 0x01D, 0x00ff);  // lvds data delay2
			break;
		default:  // invert, no delay
			SetSS_lvds_in_delay(1, 0);	// invert, no delay
			//MISP_RegWrite(MISP_HOST_ID, 0x01C, 0x4924);  // lvds data delay1
			//MISP_RegWrite(MISP_HOST_ID, 0x01D, 0x0092);  // lvds data delay2
			break;
	}
#elif defined(TEMP_IMX124_REF_OPT)
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:  // invert, 1 delay
			SetSS_lvds_in_delay(1, 1);	// invert, 1 delay
			//MISP_RegWrite(MISP_HOST_ID, 0x01C, 0xdb6d);  // lvds data delay1
			//MISP_RegWrite(MISP_HOST_ID, 0x01D, 0x00b6);  // lvds data delay2
			break;
		default:  // invert, no delay
			SetSS_lvds_in_delay(1, 0);	// invert, no delay
			//MISP_RegWrite(MISP_HOST_ID, 0x01C, 0x4924);  // lvds data delay1
			//MISP_RegWrite(MISP_HOST_ID, 0x01D, 0x0092);  // lvds data delay2
			break;
	}
#endif

	//set bayer clock - pixel to bayer clock
	// reg 0x0024 - clk2_bayer_ctrl (clk2_bayer_dly_sel, clk2_bayer_divider, clk2_bayer_div2_sel)
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:  SetCLK_BayerClkDivider(2);  break;  //Frame by Frame
		case WDRCTRL_MODE_LINE2:  SetCLK_BayerClkDivider(2);  break;  //DOL2 : 60hz->30hz (2200x1125x2 -> 2200x1125)
#if defined(__USE_CMOS_MN34220__)||\
    defined(__USE_CMOS_IMX290__)||\
    defined(__USE_CMOS_IMX385__)
  #if   WDR_DOL_OVER_SYN
		case WDRCTRL_MODE_LINE3:  SetCLK_BayerClkDivider(3);  break;  //DOL3 : 90hz->30hz (2200x1125x3 -> 2200x1125)
  #elif WDR_SEPA_ODM_CLK
		case WDRCTRL_MODE_LINE3:  SetCLK_BayerClkDivider(4);  break;  //DOL3 :120hz->30hz (2200x1500x3 -> 2200x1125)
  #else
		case WDRCTRL_MODE_LINE3:  SetCLK_BayerClkDivider(3);  break;  //DOL3 :120hz->30hz (2200x1500x3 -> 2200x1125), use 4/3 clk
  #endif
#else
		case WDRCTRL_MODE_LINE3:  SetCLK_BayerClkDivider(4);  break;  //DOL3 :120hz->30hz (2200x1500x3 -> 2200x1125)
#endif
		default:                  SetCLK_BayerClkDivider(1);  break;  //Tonemap, DWDR, WDR off
	}

	//set ODM FIFO clock - pixel to odm analyzer
#if WDR_SEPA_ODM_CLK
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_LINE2:  SetCLK_OdmFifoClkDivider(2);  break;  //DOL2 : 60hz->30hz (2200x1125x2 -> 2200x1125)
#if defined(__USE_CMOS_MN34220__)||\
    defined(__USE_CMOS_IMX290__)
		case WDRCTRL_MODE_LINE3:  SetCLK_OdmFifoClkDivider(3);  break;  //DOL3 :120hz->30hz (2200x1500x3 -> 2200x1125), use 4/3 clk
#elif defined(__USE_CMOS_IMX385__)  // since there's no odm-clock delay logic, fine-tune clock phase via clock speed controls.
		case WDRCTRL_MODE_LINE3:  SetCLK_OdmFifoClkDivider(2);  break;  // actually '3' is right
#else
		case WDRCTRL_MODE_LINE3:  SetCLK_OdmFifoClkDivider(4);  break;  //DOL3 :120hz->30hz (2200x1500x3 -> 2200x1125)
#endif
		default:                  SetCLK_OdmFifoClkDivider(1);  break;  //n/a
	}
	//ODM clock setting
	if(	(pCTRL->ctrl&WDRCTRL_MODE_LINE2)||
		(pCTRL->ctrl&WDRCTRL_MODE_LINE3) )  {  //line by line wdr
		//SetCLK_ODMClkSelect(0);  //use clk_bayer
		SetCLK_ODMClkSelect(2);  //use OdmFifoClkDivider clk
	}
	else {
		SetCLK_ODMClkSelect(1);  //use clk_sensor_in
	}	
	
#else
	//ODM clock setting
	if(	(pCTRL->ctrl&WDRCTRL_MODE_LINE2)||
		(pCTRL->ctrl&WDRCTRL_MODE_LINE3) )  {  //line by line wdr
		SetCLK_ODMClkSelect(0);  //use clk_bayer
	}
	else {
		SetCLK_ODMClkSelect(1);  //use clk_sensor_in
	}		
#endif


	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//set odm data path
static void WDRCTRL_SetODM(PWDR_CTRL_PARAM pCTRL) {
	//ODM FIFO
	if(	(pCTRL->ctrl&WDRCTRL_MODE_LINE2)||
		(pCTRL->ctrl&WDRCTRL_MODE_LINE3) )  {  //line by line wdr
		MISP_RegField(MISP_LOCAL_ID, 0x600, 9, 1, 1);  // ODM FIFO ON
	}
	else {
		MISP_RegField(MISP_LOCAL_ID, 0x600, 9, 1, 0);  // ODM FIFO OFF
	}

	//ODM set path
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_BUILT:  //built-in
#if REV_RB_SLOPE_EN
			//MISP_RegField(MISP_LOCAL_ID, 0x60C, 0,8, 0x0F);  // ODM WDR WB (HTM mode)
			WBCTRL_SetAWBCtrlPath(1,1,1,1,1);
#else
			//MISP_RegField(MISP_LOCAL_ID, 0x60C, 0,8, 0x07);  // ODM WDR WB (HTM mode)
			WBCTRL_SetAWBCtrlPath(1,0,1,1,1);
#endif
			break;
		case WDRCTRL_MODE_FRAME:  //frame by frame
		case WDRCTRL_MODE_LINE2:  //line by line (2line)
		case WDRCTRL_MODE_LINE3:  //line by line (3line)
#if REV_WDR_PREV_WB  //use ODM WB
  #if REV_RB_SLOPE_EN
			//MISP_RegField(MISP_LOCAL_ID, 0x60C, 0, 8, 0xF0);  // ODM WDR WB (WDR mode)
			WBCTRL_SetAWBCtrlPath(0,1,1,1,1);
  #else
			//MISP_RegField(MISP_LOCAL_ID, 0x60C, 0, 8, 0x70);  // ODM WDR WB (WDR mode)
			WBCTRL_SetAWBCtrlPath(0,0,1,1,1);
  #endif
#else  //use MGR WB
			//MISP_RegField(MISP_LOCAL_ID, 0x60C, 0, 8, 0x20);  // ODM WDR WB (WDR mode)
			WBCTRL_SetAWBCtrlPath(0,0,0,0,1);
#endif
			break;
		default:  //normal mode
#if REV_RB_SLOPE_EN
			//MISP_RegField(MISP_LOCAL_ID, 0x60C, 0, 8, 0xF0);  // ODM normal WB
			WBCTRL_SetAWBCtrlPath(0,1,1,1,1);
#else
			//MISP_RegField(MISP_LOCAL_ID, 0x60C, 0, 8, 0x70);  // ODM normal WB
			WBCTRL_SetAWBCtrlPath(0,0,1,1,1);
#endif
			break;
	}

	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//set bayer data path
static void WDRCTRL_SetBayer(PWDR_CTRL_PARAM pCTRL) {
	//bayer top data path select (nomal or "wdr")
	if(pCTRL->ctrl&WDRCTRL_PROCESS_ON)
		SetBY_Input_select(0);  //From WDR Top
	else
		SetBY_Input_select(1);  //From Sensor Top

	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//compress wdr result to reduce bandwidth usage
static void WDRCTRL_SetCompress(PWDR_CTRL_PARAM pCTRL) {
#if (USE_IPC_COMPRES)&&(!WDR_SENS_1M_RES)&&(!WDR_SENS_3M_RES)  //Compress supports 2M only!!
	//enables 1080p only
	BOOL Compress_en;
	switch(GetOUT_MainFrmt()) {
		case MISP_VIDOUT_1920x1080p60:
		case MISP_VIDOUT_1920x1080p50:
		case MISP_VIDOUT_1920x1080p30:
		case MISP_VIDOUT_1920x1080p25:
		case MISP_VIDOUT_1920x1080p24:
#if defined(__USE_59Hz_OUTPUT__)
		case MISP_VIDOUT_1920x1080p59:
		case MISP_VIDOUT_1920x1080p29:
#endif
			Compress_en=TRUE;
			break;
		default:
			Compress_en=FALSE;
			break;
	}

	//set compress
	if(	(pCTRL->ctrl&WDRCTRL_MODE_LINE2)||
		(pCTRL->ctrl&WDRCTRL_MODE_LINE3) ) {
		if(Compress_en)
			SetWDR_CompressCtrl(MISP_COMP_ON);   //compress on
		else
			//SetWDR_CompressCtrl(MISP_COMP_420);  //420 mode
			SetWDR_CompressCtrl(MISP_COMP_OFF);  //compress off
	}
	else {
		SetWDR_CompressCtrl(MISP_COMP_OFF);  //compress off
	}
#endif
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//set mgr_offst, mgr_pixelmax with sensor BLC
static void WDRCTRL_SetSensorBLC(PWDR_CTRL_PARAM pCTRL) {
	//set mgr_offst
#if REV_WDR_PREV_WB
	pCTRL->mgr_offst=0;
#else
    pCTRL->mgr_offst=stWDRSAVE.bl_mval;   //blc by mgr (need to check about blc_type)
#endif
    pCTRL->mgr_offst>>=pCTRL->inp_shift;  //input shift
	MISP_RegWrite(MISP_LOCAL_ID, 0x5A1, pCTRL->mgr_offst);  //mgr_offset

	//set mgr_pixelmax
	MISP_RegRead(MISP_LOCAL_ID, 0x60B, &pCTRL->mgr_pxmax);  //refer to init_ODM.c (WB data cut)
	pCTRL->mgr_pxmax>>=4;  //base 16->12bit

	#if defined(__USE_CMOS_IMX273__) //|| defined(__USE_IMX265_1536P_ALL_SCAN_MODE__)
	pCTRL->mgr_pxmax = pCTRL->mgr_pxmax/10 * 8;		//reduce err : non linear region on imx273 sensor 
	#endif
	
	pCTRL->mgr_pxmax>>=pCTRL->inp_shift;  //input shift
	MISP_RegWrite(MISP_LOCAL_ID, 0x5A2, pCTRL->mgr_pxmax);  // mgr_pixelmax

#if (!REV_WDR_MANU_TM)
	//set for initial TM max
	pCTRL->tma_level=pCTRL->mgr_pxmax/10*7;  //statr with 70% level
	MISP_RegField(MISP_LOCAL_ID, 0x5A8, 0,12, pCTRL->tma_level);  // wdr_satur_level
#endif

#if (REV_REG_MGR_MDS)&&(!REV_WDR_NEW_MGR)
	//set divide motion area
	WORD mgr_mlvl0 = pCTRL->mgr_pxmax;  //motion saturation : use shorter
	WORD mgr_mlvl1 = 14;      //motion under-exp. : force 2D-NR
	MISP_RegField(MISP_LOCAL_ID, 0x7F3, 0,12, mgr_mlvl0);  // mgr_mlvl0 (high thres)
	MISP_RegField(MISP_LOCAL_ID, 0x7F4, 0,12, mgr_mlvl1);  // mgr_mlvl1 (low thres)
	MISP_RegField(MISP_LOCAL_ID, 0x7F5,12, 4, 4);          // mgr_mo_flag_sel (0100 : still,low,mid,high) for 2D-NR

	//set MDS
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:
		case WDRCTRL_MODE_LINE2:
			MISP_RegField(MISP_LOCAL_ID, 0x7F5, 8, 3, 2);  // mgr_mds0 (high level) - 2:short, 3:short+shorter 4:shorter
			MISP_RegField(MISP_LOCAL_ID, 0x7F5, 5, 3, 2);  // mgr_mds1 (mid  level) - 2:short, 3:short+shorter 4:shorter
			MISP_RegField(MISP_LOCAL_ID, 0x7F5, 2, 3, 2);  // mgr_mds2 (low  level) - 2:short, 3:short+shorter 4:shorter
			break;
		case WDRCTRL_MODE_LINE3:
			MISP_RegField(MISP_LOCAL_ID, 0x7F5, 8, 3, 4);  // mgr_mds0 (high level) - 2:short, 3:short+shorter 4:shorter
			MISP_RegField(MISP_LOCAL_ID, 0x7F5, 5, 3, 4);  // mgr_mds1 (mid  level) - 2:short, 3:short+shorter 4:shorter
			MISP_RegField(MISP_LOCAL_ID, 0x7F5, 2, 3, 4);  // mgr_mds2 (low  level) - 2:short, 3:short+shorter 4:shorter
			break;
		default:
			break;
	}
#endif

	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//set sensor memory write mode
static void WDRCTRL_SetSensorWrite(PWDR_CTRL_PARAM pCTRL) {
	BYTE sw_bit_mode=WDRCTRL_GetSwBitMode(pCTRL);
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:  SetSS_wdr_write_mode(MISP_WDR_MEM_WIRTE_FRAME_MODE, sw_bit_mode, 0x03);  break;
#if (!REV_WDR_NO_BUFF)||defined(TEST_DUMP_TONE)
		case WDRCTRL_MODE_BUILT:  SetSS_wdr_write_mode(MISP_WDR_MEM_WIRTE_HTM_MODE,   sw_bit_mode, 0x01);  break;
		case WDRCTRL_MODE_SINGL:  SetSS_wdr_write_mode(MISP_WDR_MEM_WIRTE_HTM_MODE,   sw_bit_mode, 0x01);  break;
#endif
		case WDRCTRL_MODE_LINE2:  SetSS_wdr_write_mode(MISP_WDR_MEM_WIRTE_LINE_MODE,  sw_bit_mode, 0x03);  break;
		case WDRCTRL_MODE_LINE3:  SetSS_wdr_write_mode(MISP_WDR_MEM_WIRTE_LINE_MODE,  sw_bit_mode, 0x07);  break;
		default:                  SetSS_wdr_write_mode(MISP_WDR_MEM_WIRTE_OFF_MODE,   sw_bit_mode, 0x00);  break;
	}
}


//--------------------------------------------------------------------------------------------------------------------------
//motion flag control
static void WDRCTRL_SetMotionFlag(PWDR_CTRL_PARAM pCTRL) {
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
#if (REV_REG_MGR_MDS)&&(!REV_WDR_NEW_MGR)
		case WDRCTRL_MODE_LINE2:
		case WDRCTRL_MODE_LINE3:
			// motion filtering
			MISP_RegField(MISP_LOCAL_ID, 0x7F5,11, 1, 1);  // mgr_mo_use_en
			SetNR_UseWdrMotionFlag(1);  //0:disable, 1:motion, 2:suppress (2D filtering to MGR motion area)
			break;
#endif
#if REV_x36_CLR_SPR
		case WDRCTRL_MODE_BUILT:
			// color suppression
			MISP_RegField(MISP_LOCAL_ID, 0x7F5,11, 1, 1);  // mgr_mo_use_en
			SetNR_UseWdrMotionFlag(2);  //0:disable, 1:motion, 2:suppress (Color Suppression to Artifact assumed area)
			break;
#endif
		case WDRCTRL_MODE_FRAME:
		case WDRCTRL_MODE_SINGL:
		default:
			// off
			MISP_RegField(MISP_LOCAL_ID, 0x7F5,11, 1, 0);  // mgr_mo_use_en
			SetNR_UseWdrMotionFlag(0);  //0:disable, 1:motion, 2:suppress (Color Suppression to Artifact assumed area)
			break;
	}
}


//--------------------------------------------------------------------------------------------------------------------------
//set wdr core registers about control
static void WDRCTRL_SetWDRCtrl(PWDR_CTRL_PARAM pCTRL) {
	BYTE sw_bit_mode;

	//reset r_wdr_sep_fid_mode
	MISP_RegField(MISP_LOCAL_ID, 0x5C2,14,1, 0);  //disable single read/write

	//sync trigger vact
	if     (pCTRL->ctrl&WDRCTRL_MODE_LINE2)  MISP_RegField(MISP_LOCAL_ID, 0x5C2, 12,2,3);  // r_wdr_s_vact_sel (3:s1)
	else if(pCTRL->ctrl&WDRCTRL_MODE_LINE3)  MISP_RegField(MISP_LOCAL_ID, 0x5C2, 12,2,2);  // r_wdr_s_vact_sel (2:s2)
	else                                     MISP_RegField(MISP_LOCAL_ID, 0x5C2, 12,2,0);  // r_wdr_s_vact_sel (0:long)

	//tm_only_en
	if(	(pCTRL->ctrl&WDRCTRL_MODE_BUILT)||
		(pCTRL->ctrl&WDRCTRL_MODE_SINGL) )
		MISP_RegField(MISP_LOCAL_ID, 0x5B5, 0, 1, 1);  // tm_only_en (on)
	else
		MISP_RegField(MISP_LOCAL_ID, 0x5B5, 0, 1, 0);  // tm_only_en (off)

	//input data bit shift
	if(pCTRL->ctrl&WDRCTRL_PROCESS_ON) {  //WDR on
		MISP_RegField(MISP_LOCAL_ID, 0x5B4, 0, 3, pCTRL->inp_shift);  // wdr_bit_shift_l
		MISP_RegField(MISP_LOCAL_ID, 0x5B4, 3, 3, pCTRL->inp_shift);  // wdr_bit_shift_s1
		MISP_RegField(MISP_LOCAL_ID, 0x5B4, 6, 3, pCTRL->inp_shift);  // wdr_bit_shift_s2
		MISP_RegField(MISP_LOCAL_ID, 0x5B4, 9, 3, pCTRL->inp_shift);  // wdr_bit_shift_s3
	}

	//set frame id
	if(pCTRL->ctrl&WDRCTRL_MODE_FRAME) {
		MISP_RegField(MISP_LOCAL_ID, 0x5DF,  8,8, 0xE0);  // wdr_imagesel : 3,2,x,x
		MISP_RegField(MISP_LOCAL_ID, 0x5C9,  8,2, 1);     // r_wdr_num_frame(1:2frame)
#if USE_FWDR_FID_EN  //enable frame mode fid control
	  if(GetSS_sensor_wdr_info_en()) {
		MISP_RegField(MISP_LOCAL_ID, 0x5C9, 10,1, 1);     // r_wdr_use_ext_fid
	  }
#endif
	}
	else {
		MISP_RegField(MISP_LOCAL_ID, 0x5DF,  8,8, 0xE4);  // wdr_imagesel : 3,2,1,0
		MISP_RegField(MISP_LOCAL_ID, 0x5C9,  8,2, 0);     // r_wdr_num_frame(x)
		MISP_RegField(MISP_LOCAL_ID, 0x5C9, 10,1, 0);     // r_wdr_use_ext_fid(x)
	}

	//sensor read bit mode (reg.59C)
	sw_bit_mode=WDRCTRL_GetSwBitMode(pCTRL);
	MISP_RegField(MISP_LOCAL_ID, 0x59C,10, 2, sw_bit_mode);  // wdr_s3_bit_mode
	MISP_RegField(MISP_LOCAL_ID, 0x59C, 8, 2, sw_bit_mode);  // wdr_s2_bit_mode
	MISP_RegField(MISP_LOCAL_ID, 0x59C, 6, 2, sw_bit_mode);  // wdr_s1_bit_mode
	MISP_RegField(MISP_LOCAL_ID, 0x59C, 4, 2, sw_bit_mode);  // wdr_l_bit_mode

	//Set wdr ctrl and enable (reg.59C, reg.59D)
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:
			MISP_RegField(MISP_LOCAL_ID, 0x59C, 0, 2, 1);     // wdr_mergenum(1:2frame)
			MISP_RegField(MISP_LOCAL_ID, 0x59D, 8, 4, 0x03);  // wdr_proc_frame_en(0011)
			MISP_RegField(MISP_LOCAL_ID, 0x59D, 0, 8, 0xC2);  // mgr_imagesel(3->x->x->2)
			MISP_RegField(MISP_LOCAL_ID, 0x59D,14, 1, 1);     // svalid_cut_sel(1)
			MISP_RegField(MISP_LOCAL_ID, 0x59D,13, 1, 1);     // linemem enable(1)
			if(pCTRL->fview_sel==MISP_WDR_FVIEW_WDR)
			  MISP_RegField(MISP_LOCAL_ID, 0x59D,12, 1, 1);   // wdr_process_en(1)
			else
			  MISP_RegField(MISP_LOCAL_ID, 0x59D,12, 1, 0);   // wdr_process_en(0) : bypass
  #if REV_WDR_NEW_MGR
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,15, 1, 1);     // wdr_satur_scope(1) : use range[9:0]
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,12, 2, 2);     // wdr_sat_frm_exp(2) : use short frame
			MISP_RegField(MISP_LOCAL_ID, 0x5A3, 4, 2, 2);     // wdr_sat_frm_sat(2) : use short frame
  #else
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,15, 1, 1);     // wdr_satur_scope(1) : use range[9:0]
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,12, 2, 2);     // wdr_satur_frame(2) : use short frame (TM max)
  #endif
			break;
#if (!REV_WDR_NO_BUFF)||defined(TEST_DUMP_TONE)
		case WDRCTRL_MODE_BUILT:
		case WDRCTRL_MODE_SINGL:
			MISP_RegField(MISP_LOCAL_ID, 0x59C, 0, 2, 0);     // wdr_mergenum(x)
			MISP_RegField(MISP_LOCAL_ID, 0x59D, 8, 4, 0x01);  // wdr_proc_frame_en(0001)
			MISP_RegField(MISP_LOCAL_ID, 0x59D, 0, 8, 0x00);  // mgr_imagesel(x)
			MISP_RegField(MISP_LOCAL_ID, 0x59D,14, 1, 1);     // svalid_cut_sel(1)
			MISP_RegField(MISP_LOCAL_ID, 0x59D,13, 1, 1);     // linemem enable(1)
			if(pCTRL->fview_sel==MISP_WDR_FVIEW_WDR)
			  MISP_RegField(MISP_LOCAL_ID, 0x59D,12, 1, 1);   // wdr_process_en(1)
			else
			  MISP_RegField(MISP_LOCAL_ID, 0x59D,12, 1, 0);   // wdr_process_en(0) : bypass
  #if REV_WDR_NEW_MGR
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,15, 1, 0);     // wdr_satur_scope(x)
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,12, 2, 0);     // wdr_sat_frm_exp(x)
			MISP_RegField(MISP_LOCAL_ID, 0x5A3, 4, 2, 0);     // wdr_sat_frm_sat(x)
  #else
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,15, 1, 0);     // wdr_satur_scope(x)
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,12, 2, 0);     // wdr_satur_frame(x)
  #endif
			break;
#endif
		case WDRCTRL_MODE_LINE2:
			MISP_RegField(MISP_LOCAL_ID, 0x59C, 0, 2, 1);     // wdr_mergenum(1:2frame)
			MISP_RegField(MISP_LOCAL_ID, 0x59D, 8, 4, 0x03);  // wdr_proc_frame_en(0011)
			MISP_RegField(MISP_LOCAL_ID, 0x59D, 0, 8, 0xC2);  // mgr_imagesel(3->x->x->2)
			MISP_RegField(MISP_LOCAL_ID, 0x59D,14, 1, 0);     // svalid_cut_sel(0)
			MISP_RegField(MISP_LOCAL_ID, 0x59D,13, 1, 1);     // linemem enable(1)
			if(pCTRL->fview_sel==MISP_WDR_FVIEW_WDR)
			  MISP_RegField(MISP_LOCAL_ID, 0x59D,12, 1, 1);   // wdr_process_en(1)
			else
			  MISP_RegField(MISP_LOCAL_ID, 0x59D,12, 1, 0);   // wdr_process_en(0) : bypass
  #if REV_WDR_NEW_MGR
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,15, 1, 1);     // wdr_satur_scope(1) : use range[9:0]
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,12, 2, 2);     // wdr_sat_frm_exp(2) : use short frame
			MISP_RegField(MISP_LOCAL_ID, 0x5A3, 4, 2, 2);     // wdr_sat_frm_sat(2) : use short frame
  #else
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,15, 1, 1);     // wdr_satur_scope(1) : use range[9:0]
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,12, 2, 2);     // wdr_satur_frame(2) : use short frame
  #endif
			break;
		case WDRCTRL_MODE_LINE3:
			MISP_RegField(MISP_LOCAL_ID, 0x59C, 0, 2, 2);     // wdr_mergenum(2:3frame)
			MISP_RegField(MISP_LOCAL_ID, 0x59D, 8, 4, 0x07);  // wdr_proc_frame_en(0111)
			MISP_RegField(MISP_LOCAL_ID, 0x59D, 0, 8, 0xC9);  // mgr_imagesel(3->x->2->1)
			MISP_RegField(MISP_LOCAL_ID, 0x59D,14, 1, 0);     // svalid_cut_sel(0)
			MISP_RegField(MISP_LOCAL_ID, 0x59D,13, 1, 1);     // linemem enable(1)
			if(pCTRL->fview_sel==MISP_WDR_FVIEW_WDR)
			  MISP_RegField(MISP_LOCAL_ID, 0x59D,12, 1, 1);   // wdr_process_en(1)
			else
			  MISP_RegField(MISP_LOCAL_ID, 0x59D,12, 1, 0);   // wdr_process_en(0) : bypass
  #if REV_WDR_NEW_MGR
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,15, 1, 1);     // wdr_satur_scope(1) : use range[9:0]
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,12, 2, 1);     // wdr_sat_frm_exp(3) : use shorter frame
			MISP_RegField(MISP_LOCAL_ID, 0x5A3, 4, 2, 1);     // wdr_sat_frm_sat(1) : use shorter frame
  #else
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,15, 1, 1);     // wdr_satur_scope(1) : use range[9:0]
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,12, 2, 1);     // wdr_satur_frame(1) : use shorter frame
  #endif
			break;
		default:
			MISP_RegField(MISP_LOCAL_ID, 0x59C, 0, 2, 0);     // wdr_mergenum(x)
			MISP_RegField(MISP_LOCAL_ID, 0x59D, 8, 4, 0x00);  // wdr_proc_frame_en(x)
			MISP_RegField(MISP_LOCAL_ID, 0x59D, 0, 8, 0x00);  // mgr_imagesel(x)
			MISP_RegField(MISP_LOCAL_ID, 0x59D,14, 1, 0);     // svalid_cut_sel(x)
			MISP_RegField(MISP_LOCAL_ID, 0x59D,13, 1, 0);     // linemem enable(0)
			MISP_RegField(MISP_LOCAL_ID, 0x59D,12, 1, 0);     // wdr_process_en(0)
  #if REV_WDR_NEW_MGR
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,15, 1, 0);     // wdr_satur_scope(X)
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,12, 2, 0);     // wdr_sat_frm_exp(x)
			MISP_RegField(MISP_LOCAL_ID, 0x5A3, 4, 2, 0);     // wdr_sat_frm_sat(x)
  #else
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,15, 1, 0);     // wdr_satur_scope(X)
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,12, 2, 0);     // wdr_satur_frame(x)
  #endif
			break;
	}
}


//--------------------------------------------------------------------------------------------------------------------------
//get sensor input total size (wdr read size)
static void WDRCTRL_GetTotalSize(PWDR_CTRL_PARAM pCTRL, PWORD h_total, PWORD v_total) {
#if defined(__USE_CMOS_AR0331__)
	if(pCTRL->ctrl&WDRCTRL_MODE_BUILT) { *h_total=2240;  *v_total=2209; }
	else                               { *h_total=2200;  *v_total=1125; }

#elif defined(__USE_CMOS_PO3100K__)  //720P
    *h_total=1648;
    *v_total=750;

#elif defined(__USE_CMOS_MN34041__)
	*h_total=2400;
	*v_total=1125;

#elif defined(__USE_CMOS_IMX123__)||\
      defined(__USE_CMOS_IMX124__)
  #if WDR_SENS_3M_RES
	if(pCTRL->ctrl&WDRCTRL_MODE_BUILT) { *h_total=4500; *v_total=1600; }  // 4500=(750*4)*1.5
	else                               { *h_total=2250; *v_total=1600; }  // 2250=(750*2)*1.5
  #else  //1080P sensor
	if(pCTRL->ctrl&WDRCTRL_MODE_BUILT) { *h_total=4400; *v_total=1125; }
	else                               { *h_total=2200; *v_total=1125; }
  #endif

#elif defined(__USE_IMX290_720P30_MODE__)
	*h_total=2200;
	*v_total= 750;

#elif defined(__USE_CMOS_MN34220__)||\
      defined(__USE_CMOS_IMX290__)||\
      defined(__USE_CMOS_IMX327__)||\
      defined(__USE_CMOS_IMX385__)
 #if defined(__USE_IMX290_720P120_MODE__) || defined(__USE_IMX290_720P60_MODE__) || defined(__USE_IMX290_720P30_MODE__)
   #if (!WDR_SEPA_ODM_CLK)&&(!WDR_DOL_OVER_SYN)
	if(pCTRL->ctrl&WDRCTRL_MODE_LINE3) { *h_total=3300; *v_total=1000; }  // 750x4/3
	else                               { *h_total=3300; *v_total=750; }
   #else
	*h_total=3300;
	*v_total=750;
  #endif
 #else
   #if (!WDR_SEPA_ODM_CLK)&&(!WDR_DOL_OVER_SYN)
	if(pCTRL->ctrl&WDRCTRL_MODE_LINE3) { *h_total=2200; *v_total=1500; }  // 1125x4/3
	else                               { *h_total=2200; *v_total=1125; }
   #else
	*h_total=2200;
	*v_total=1125;
  #endif 
 #endif
#elif defined(__USE_CMOS_IMX273__)
	#if defined(__USE_IMX273_ALLSCAN_MODE__)
	*h_total=2200;
	*v_total=1500;
	#else
	*h_total=3000;	//3000x12/8
	*v_total=1100;
	#endif
#elif defined(__USE_CMOS_IMX265__)
	if(GetSS_image_active_size_v() > 1080){
		if(GetSS_SensorVoutPAL())	{	*h_total=2000;	*v_total=1650;	}
		else 	{	*h_total=4000;	*v_total=1650;	}
	}
	else		{	*h_total=2200;	*v_total=1125;	}

#else  //normal 1080P sensor (imx136, imx174, mn34420)
	*h_total=2200;
	*v_total=1125;

#endif

	//extend for PAL system
#if defined(__VTOTAL_CTRL_PAL__)
	if(GetSS_SensorVoutPAL())  *v_total *= 1.2f;
#else
	if(GetSS_SensorVoutPAL())  *h_total *= 1.2f;
#endif

	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//set wdr core registers about image size
static void WDRCTRL_SetWDRSize(PWDR_CTRL_PARAM pCTRL) {
	WORD tot_hsize;
	WORD tot_vsize;
#if WDR_SENS_3M_RES
	DWORD tot_pixel;
#endif

	WORD wdr_hsize;
	WORD wdr_vsize;
	WORD wdr_hblnk;
	WORD wdr_vblnk;
	WORD spl_hsize;
	WORD spl_vsize;
	WORD spl_hblnk;
	WORD spl_vblnk;

	BYTE packet_size;

	BYTE mgr_hsize_offset;
	BYTE wdr_wt_count_value;

	//set active size
	if(pCTRL->ctrl&WDRCTRL_PROCESS_ON) {  //WDR on
		//update split mode
		BYTE trim_en;
		WORD spl_h_num;
		WORD spl_margn;
		switch(pCTRL->ctrl&WDRCTRL_SPLIT_MASK) {
			case WDRCTRL_SPLIT_NUM4:  trim_en=1;  spl_h_num=4;  spl_margn=12;  break;
			case WDRCTRL_SPLIT_NUM8:  trim_en=1;  spl_h_num=8;  spl_margn=12;  break;
			default:                  trim_en=0;  spl_h_num=1;  spl_margn= 0;  break;
		}

		//set split size
		WDRCTRL_GetTotalSize(pCTRL, &tot_hsize, &tot_vsize);  //get sensor total h/v size
#if WDR_SENS_3M_RES
  //for the 8-split mode, lack of h-total might be occurs. (refer to register 5A0h, 5C4h)
  //so, extension of total h-size is necessary.
  #if defined(__USE_CMOS_IMX123__)||\
      defined(__USE_CMOS_IMX124__)
		if(pCTRL->ctrl&WDRCTRL_SPLIT_NUM8) {  //split mode
			MISP_RegField(MISP_LOCAL_ID, 0x5C2, 15, 1, 1);  // r_wdr_slave_sync (reset sync by short)
			tot_pixel=(DWORD)tot_hsize*tot_vsize;
			tot_hsize=2320;  //extend total hsize to make blank margin
			tot_vsize=(tot_pixel+tot_hsize-1)/tot_hsize;  //round up
		}
  #endif
#endif
		wdr_hsize = GetSS_image_active_size_h()+2*GetSS_image_boundary_size_h();
		wdr_vsize = GetSS_image_active_size_v()+2*GetSS_image_boundary_size_v();
		wdr_hblnk = tot_hsize-wdr_hsize;
		wdr_vblnk = tot_vsize-wdr_vsize;
		spl_hsize = wdr_hsize/spl_h_num;
		spl_vsize = wdr_vsize;
		spl_hblnk = wdr_hblnk/spl_h_num;
		spl_vblnk = wdr_vblnk;

		MISP_RegField(MISP_LOCAL_ID, 0x5DF, 0, 8, spl_h_num);   // mgr_snum
		MISP_RegField(MISP_LOCAL_ID, 0x5B9, 0, 8, spl_h_num);   // trim_snum
		MISP_RegField(MISP_LOCAL_ID, 0x5B9, 8, 1, trim_en);     // trim_en
#if REV_WDR_NO_BUFF
		MISP_RegField(MISP_LOCAL_ID, 0x5B5, 4, 1, (!trim_en));  // tm_only_no_fbuf_en
		MISP_RegField(MISP_LOCAL_ID, 0x5FF, 0, 14, tot_hsize);  // no_fbuf_tot_hsize

	  if(pCTRL->ctrl&WDRCTRL_MODE_SINGL)
	  	MISP_RegField(MISP_LOCAL_ID, 0x5B5, 5, 1, (!trim_en));  // tm_only_no_fbuf_16bit_en
	  else
	  	MISP_RegField(MISP_LOCAL_ID, 0x5B5, 5, 1, 0);           // tm_only_no_fbuf_16bit_en
#endif

		MISP_RegWrite(MISP_LOCAL_ID, 0x59E, spl_hsize);  // WDR mgr_hsize
		MISP_RegWrite(MISP_LOCAL_ID, 0x59F, spl_vsize);  // WDR mgr_vsize
		MISP_RegWrite(MISP_LOCAL_ID, 0x5A0, spl_margn);  // WDR mgr_hmergin

		MISP_RegWrite(MISP_LOCAL_ID, 0x5B6, spl_hsize);  // WDR tm_hsize
		MISP_RegWrite(MISP_LOCAL_ID, 0x5B7, spl_vsize);  // WDR tm_vsize
		MISP_RegWrite(MISP_LOCAL_ID, 0x5B8, spl_margn);  // WDR tm_hmergin

#if REV_WDR_NEW_MGR
		MISP_RegField(MISP_LOCAL_ID, 0x5C0, 0, 13, wdr_hsize);  // WDR in_hsize
		MISP_RegField(MISP_LOCAL_ID, 0x5C1, 0, 13, wdr_vsize);  // WDR in_vsize
#else
		MISP_RegWrite(MISP_LOCAL_ID, 0x5C0, wdr_hsize);  // WDR in_hsize
		MISP_RegWrite(MISP_LOCAL_ID, 0x5C1, wdr_vsize);  // WDR in_vsize
#endif

		MISP_RegWrite(MISP_LOCAL_ID, 0x5CB, wdr_hblnk);  // r_wdr_tot_h_blank
		MISP_RegWrite(MISP_LOCAL_ID, 0x5CF, wdr_vblnk);  // r_wdr_tot_v_blank

		MISP_RegWrite(MISP_LOCAL_ID, 0x5C5, spl_hblnk);  // r_wdr_sep_h_blank
		MISP_RegWrite(MISP_LOCAL_ID, 0x5C8, spl_vblnk);  // r_wdr_sep_v_blank

		//result write option
		switch(GetWDR_RwBitMode()) {  //mru packet size
			case MISP_DDRMAP_8b:  packet_size=8;  break;  // 8bitx8
			case MISP_DDRMAP_9b:  packet_size=7;  break;  // 9bitx7
			case MISP_DDRMAP_10b: packet_size=6;  break;  //10bitx6
			default:              packet_size=5;  break;  //12bitx5
		}
		mgr_hsize_offset=(wdr_hsize/spl_h_num)%2;
		wdr_wt_count_value=(wdr_hsize/spl_h_num/packet_size)%32+mgr_hsize_offset;
		wdr_wt_count_value=wdr_wt_count_value?(wdr_wt_count_value-1):/*0*/31;
		MISP_RegField(MISP_LOCAL_ID, 0x59C, 12, 2, GetWDR_RwBitMode());
		MISP_RegField(MISP_LOCAL_ID, 0x59E, 12, 3, mgr_hsize_offset);
		MISP_RegField(MISP_LOCAL_ID, 0x5BC,  5, 5, wdr_wt_count_value);
	}
}


//--------------------------------------------------------------------------------------------------------------------------
//configure MISP about shutter
static void WDRCTRL_SetExpRatio(PWDR_CTRL_PARAM pCTRL) {
#if REV_WDR_NEW_MGR
	WORD input_max;
	DWORD r_wb_cut_off;
	WORD r_wb_cut_off_lsb;
	WORD r_wb_cut_off_msb;

	WORD r_sh_1_to_long;
	WORD r_sh_2_to_long;
	WORD r_long_to_sh_2;
	WORD r_sh_1_to_sh_2;
	WORD r_under_lev;
	WORD r_satur_lev;
	WORD r_exp_sh_1_satur;
	WORD r_exp_long_satur;

	//mux source expose
	WORD accum_long, accum_sh_1, accum_sh_2;
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:
		case WDRCTRL_MODE_LINE2:  //2->1 frame
			accum_long=pCTRL->accm_long;
			accum_sh_1=0;  //n/a
			accum_sh_2=pCTRL->accm_sh_1;
			break;
		default:  //3->1 frame
			accum_long=pCTRL->accm_long;
			accum_sh_1=pCTRL->accm_sh_1;
			accum_sh_2=pCTRL->accm_sh_2;
			break;
	}

	//WB registers
	input_max=pCTRL->mgr_pxmax-pCTRL->mgr_offst;
	r_wb_cut_off=(accum_sh_2)?(input_max*accum_long/accum_sh_2):input_max;  //18bit. wb cut-off level
	r_wb_cut_off_lsb=(WORD)((r_wb_cut_off    )&0x0000FFFF);
	r_wb_cut_off_msb=(WORD)((r_wb_cut_off>>16)&0x00000003);
	MISP_RegField(MISP_LOCAL_ID, 0x43A, 0,16, r_wb_cut_off_lsb);
	MISP_RegField(MISP_LOCAL_ID, 0x43B, 0, 2, r_wb_cut_off_msb);

	//WDR regsiters
	r_sh_1_to_long=accum_sh_1?(64*accum_long/accum_sh_1):0;  //sh_1 to long ratio (Q6)
	r_sh_2_to_long=accum_sh_2?(64*accum_long/accum_sh_2):0;  //sh_2 to long ratio (Q6)
	r_long_to_sh_2=accum_long?(64*accum_sh_2/accum_long):0;  //long to sh_2 ratio (Q6)
	r_sh_1_to_sh_2=accum_sh_1?(64*accum_sh_2/accum_sh_1):0;  //sh_1 to sh_2 ratio (Q6)
	r_under_lev=10;                                    //12bit. under-exp. level
	r_satur_lev=pCTRL->mgr_pxmax-pCTRL->mgr_offst-10;  //12bit. saturation level
	r_exp_sh_1_satur=accum_sh_1?(r_satur_lev*accum_sh_2/accum_sh_1):0;  //with sh_2, expect sh_1 saturation
	r_exp_long_satur=accum_long?(r_satur_lev*accum_sh_2/accum_long):0;  //with sh_2, expect long saturation
	MISP_RegField(MISP_LOCAL_ID, 0x430, 0,14, r_sh_1_to_long);
	MISP_RegField(MISP_LOCAL_ID, 0x431, 0,14, r_sh_2_to_long);
	MISP_RegField(MISP_LOCAL_ID, 0x432, 0,14, r_long_to_sh_2);
	MISP_RegField(MISP_LOCAL_ID, 0x433, 0,14, r_sh_1_to_sh_2);
	MISP_RegField(MISP_LOCAL_ID, 0x434, 0,12, r_under_lev);
	MISP_RegField(MISP_LOCAL_ID, 0x435, 0,12, r_satur_lev);
	MISP_RegField(MISP_LOCAL_ID, 0x436, 0,12, r_exp_sh_1_satur);
	MISP_RegField(MISP_LOCAL_ID, 0x437, 0,12, r_exp_long_satur);

	//todo : control mergenum (if long-short exopsure ratio is too big, give up merge)
	/*
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:
		case WDRCTRL_MODE_LINE2:  //2->1 frame
			if     (accum_long>=accum_sh_2*128)  MISP_RegField(MISP_LOCAL_ID, 0x59C, 0, 2, 0);  //long only
			else                                 MISP_RegField(MISP_LOCAL_ID, 0x59C, 0, 2, 1);  //2->1 frame
			break;
		default:  //3->1 frame
			if     (accum_long>=accum_sh_1*128)  MISP_RegField(MISP_LOCAL_ID, 0x59C, 0, 2, 0);  //long only
			else if(accum_long>=accum_sh_2*128)  MISP_RegField(MISP_LOCAL_ID, 0x59C, 0, 2, 1);  //2->1 frame
			else                                 MISP_RegField(MISP_LOCAL_ID, 0x59C, 0, 2, 2);  //3->1 frame
			break;
	}
	*/

#else
	//configure MISP about shutter (Q.12)
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:
		case WDRCTRL_MODE_LINE2:  //2->1 frame
			MISP_RegWrite(MISP_LOCAL_ID, 0x5A4, pCTRL->accm_sh_1);  // mgr_ex_time0 (short)
			MISP_RegWrite(MISP_LOCAL_ID, 0x5A5, 0x0000);            // mgr_ex_time1 (don't care)
			MISP_RegWrite(MISP_LOCAL_ID, 0x5A6, 0x0000);            // mgr_ex_time2 (don't care)
			MISP_RegWrite(MISP_LOCAL_ID, 0x5A7, pCTRL->accm_long);  // mgr_ex_time3 (long)
			break;
		default:  //3->1 frame
			MISP_RegWrite(MISP_LOCAL_ID, 0x5A4, pCTRL->accm_sh_2);  // mgr_ex_time0 (shorter)
			MISP_RegWrite(MISP_LOCAL_ID, 0x5A5, pCTRL->accm_sh_1);  // mgr_ex_time1 (short)
			MISP_RegWrite(MISP_LOCAL_ID, 0x5A6, 0x0000);            // mgr_ex_time2 (don't care)
			MISP_RegWrite(MISP_LOCAL_ID, 0x5A7, pCTRL->accm_long);  // mgr_ex_time3 (long)
			break;
	}
#endif
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//set parameters about shutter
static void WDRCTRL_SetShutParams(PWDR_CTRL_PARAM pCTRL) {
	#define WNA 0  // word value for n/a

	typedef enum { MODE_DOL_3, MODE_DOL_2, MODE_BUILT, MODE_FRAME, MODE_NUM } MODE_t;
	typedef enum { SHUT_E_MIN, SHUT_E_MAX, SHUT_RHS_1, SHUT_RHS_2, SHUT_NUM } SHUT_t;

	//DOL3 : [1:6:36], [1:8:64]
	//DOL2 : [1:16], [1:32]

	//set shutter range
	static ROMDATA WORD shut_range[MODE_NUM][SHUT_NUM]={
#if defined(__USE_CMOS_MN34220__)
  #if WDR_DOL_OVER_SYN  //i500, [1:16], [1:3:16]
		{ 256, 894, WNA, WNA},  //long : 256~ 880H (max.total : 1118=1125-(3+2*2))
		{ 128,1054, WNA, WNA},  //long : 128~1040H (max.total : 1120=1125-(3+1*2))
  #elif 0  //i51x, [1:16], [1:6:36] (based on VMAX.1500)
		{ 256,1250, WNA, WNA},  //long : 256~1248H (max.total : 1493=1500-(3+2*2), max.s1+s2 : 1500-1125)
		{ 128,1407, WNA, WNA},  //long : 128~1400H (max.total : 1495=1500-(3+1*2), max.s1+s2 : 1500-1125)
  #else  //i51x, [1:16], [1:6:36] (based on VMAX.1125)
		{ 256, 935, WNA, WNA},  //long : 256~ 935H (max.total : 1118=1125-(3+2*2))
		{ 128, 959, WNA, WNA},  //long : 128~ 959H (max.total : 1120=1125-(3+1*2))
  #endif
		{ WNA, WNA, WNA, WNA},
		{ 128,1120, WNA, WNA},  //long : 128~1120H (when 1:32, sh_1 : 4~ 35H)
#elif defined(__USE_CMOS_MN34420__)
		{  64, 960, 122, 139},  //long :  64~ 960H (when 1:64, sh_2 : 1~ 15H)
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},
#elif defined(__USE_CMOS_AR0331__)
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},  //support, but not impelemnted..
		{ WNA, WNA, WNA, WNA},
#elif defined(__USE_CMOS_IMXx36__)
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},
		{ 128,1120, WNA, WNA},  //long : 128~1120H (when 1:16, sh_1 : 8~ 70H)
		{ 128,1120, WNA, WNA},  //long : 128~1120H (when 1:32, sh_1 : 4~ 35H)
#elif defined(__USE_CMOS_IMX123__)
		{ 448,5504, 704, 808},  //long : 448~5504H (when 1:64, sh_2 : 7~ 86H)
		{ 160,1504,  58, WNA},  //long : 160~1504H (when 1:32, sh_1 : 5~ 47H)
		{  80,3184, WNA, WNA},  //long :  80~3184H (when 1:16, sh_1 : 5~199H)
		{ 128,1568, WNA, WNA},  //long : 128~1536H (when 1:32, sh_1 : 4~ 49H)
#elif defined(__USE_CMOS_IMX124__)
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},
		{ 128,1568, WNA, WNA},  //long : 128~1536H (when 1:32, sh_1 : 4~ 49H)
#elif defined(__USE_CMOS_IMX174__)||\
      defined(__USE_CMOS_IMX249__)
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},
		{ 128,1119, WNA, WNA},  //long : 128~1119H (imx174 shutter limit to 1119)
#elif defined(__USE_IMX290_720P30_MODE__)
		{  64,2560, 325, 371},  //long :  64~2560H (when 1:64, sh_2 : 1~40H)
		{  32, 192,   9, WNA},  //long :  32~ 192H (when 1:32, sh_1 : 1~ 6H)
		{ WNA, WNA, WNA, WNA},
		{ 128, 740, WNA, WNA},  //long : 128~ 740H (740H : 750-alpha)
#elif defined(__USE_CMOS_IMX290__)||\
	defined(__USE_CMOS_IMX327__)
		{  64,3904, 493, 560},  //long :  64~3904H (when 1:64, sh_2 : 1~61H)
		{  32, 256,  11, WNA},  //long :  32~ 256H (when 1:32, sh_1 : 1~ 8H)
		{ WNA, WNA, WNA, WNA},
		{ 128,1120, WNA, WNA},  //long : 128~1120H (when 1:32, sh_1 : 4~35H)
#elif defined(__USE_CMOS_IMX385__)
		{  64,3935, 493, 560},  //long :  64~3904H (when 1:64, sh_2 : 1~61H)
		{  32,1248,  43, WNA},  //long :  32~1248H (when 1:32, sh_1 : 1~39H)
		{ WNA, WNA, WNA, WNA},
		{ 128,1120, WNA, WNA},  //long : 128~1120H (when 1:32, sh_1 : 4~35H)
#elif defined(__USE_CMOS_IMX185__)
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},
		{ 128,1120, WNA, WNA},  //long : 128~1120H (when 1:32, sh_1 : 4~35H)
#elif defined(__USE_CMOS_IMX265__)
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},
	#if defined(__USE_IMX265_1536P_ALL_SCAN_MODE__)
		{ 128,1568, WNA, WNA},  //long : 128~1536H (when 1:32, sh_1 : 4~ 49H)
	#else
		{ 128,1119, WNA, WNA},  //long : 128~1119H (imx174 shutter limit to 1119)
	#endif
#elif defined(__USE_CMOS_IMX273__)
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},
		{ 128,1056, WNA, WNA},  //long : 128~1120H (when 1:32, sh_1 : 4~33H)
#else
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},
		{ WNA, WNA, WNA, WNA},
#endif
	};

	//update variables about shutter
	PWORD p_reg_set=NULL;
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_LINE3:  p_reg_set=(PWORD)(shut_range[MODE_DOL_3]);  break;
		case WDRCTRL_MODE_LINE2:  p_reg_set=(PWORD)(shut_range[MODE_DOL_2]);  break;
		case WDRCTRL_MODE_BUILT:  p_reg_set=(PWORD)(shut_range[MODE_BUILT]);  break;
		case WDRCTRL_MODE_FRAME:  p_reg_set=(PWORD)(shut_range[MODE_FRAME]);  break;
	}
	if(p_reg_set) {
		pCTRL->expos_min=p_reg_set[SHUT_E_MIN];
		pCTRL->expos_max=p_reg_set[SHUT_E_MAX];
		pCTRL->readout_1=p_reg_set[SHUT_RHS_1];
		pCTRL->readout_2=p_reg_set[SHUT_RHS_2];
	}
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//set sensor gain
static void WDRCTRL_SetSensGain(PWDR_CTRL_PARAM pCTRL) {
	FLOAT ratio;
	FLOAT dB;

	if(pCTRL->gain_ctrl>GetWDR_GainMax())  pCTRL->gain_ctrl=GetWDR_GainMax();
	if(pCTRL->gain_ctrl<GetWDR_GainMin())  pCTRL->gain_ctrl=GetWDR_GainMin();
#if HTM_FIXED_GAIN  //sony built-in : fixed to analog 4.5dB
	if(pCTRL->ctrl&WDRCTRL_MODE_BUILT)     pCTRL->gain_ctrl=GetWDR_GainMin();
#endif

	//calculate target dB
	ratio=(0.025f*pCTRL->gain_ctrl);
	dB=20.0f*log10(ratio);

	//update sensor gain value
#if defined(__USE_CMOS_MN34220__)
	pCTRL->sns_again=0x0100+(WORD)(dB*16/1.5f);  //1.5dB per 16
	pCTRL->sns_dgain=0x0100;  //n/a(0dB)

#elif defined(__USE_CMOS_MN34420__)
	pCTRL->sns_again=0x0120+(WORD)(dB*16/1.5f);  //1.5dB per 16
	pCTRL->sns_dgain=0x0120;  //n/a(0dB)

#elif defined(__USE_CMOS_IMXx36__)||\
      defined(__USE_CMOS_IMX174__)||\
      defined(__USE_CMOS_IMX249__)||\
      defined(__USE_CMOS_IMX123__)||\
      defined(__USE_CMOS_IMX124__)||\
      defined(__USE_CMOS_IMX385__)||\
      defined(__USE_CMOS_IMX265__)||\
      defined(__USE_CMOS_IMX273__)
	pCTRL->sns_again=(WORD)(dB*10);  //step pitch 0.1dB
	pCTRL->sns_dgain=0x0000;  //n/a(0dB)

#elif defined(__USE_CMOS_IMX290__)||\
      defined(__USE_CMOS_IMX327__)||\
      defined(__USE_CMOS_IMX185__)
	pCTRL->sns_again=(WORD)(dB*10/3);  //step pitch 0.3dB
	pCTRL->sns_dgain=0x0000;  //n/a(0dB)

#elif defined(__USE_CMOS_MN34041__)  //not implemented.. (set fixed 0dB)
	pCTRL->sns_again=0x0080;  //n/a(0dB)
	pCTRL->sns_dgain=0x0080;  //n/a(0dB)

#elif defined(__USE_CMOS_PO3100K__)  //not implemented.. (set fixed 0dB)
	pCTRL->sns_again=0x0000;  //n/a(0dB)
	pCTRL->sns_dgain=0x0040;  //n/a(0dB)

#endif
	stDebug_Print(2, "WDRCTRL_SetSensGain : %d (ratio:%.3f, dB:%.2f)\r\n", pCTRL->gain_ctrl, ratio, dB);

	//configure MISP about gain
#if (!REV_WDR_NEW_MGR)
	WORD r_gain=(WORD)(sqrt(ratio)*256+0.5);
	MISP_RegField(MISP_LOCAL_ID, 0x5FB, 10,1,1);  //hold adjust
	MISP_RegWrite(MISP_LOCAL_ID, 0x5E3, r_gain);  //mgr_sensor_again ((int)(sqrt(pow(10,dB/20)) * 256+0.5))
	MISP_RegWrite(MISP_LOCAL_ID, 0x5E4, 0x0100);  //mgr_sensor_dgain ((int)(sqrt(pow(10,dB/20)) * 256+0.5))
	MISP_RegField(MISP_LOCAL_ID, 0x5FB, 10,1,0);  //flush
#endif
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//set sensor shutter
static void WDRCTRL_SetSensShut(PWDR_CTRL_PARAM pCTRL) {
	WORD div_offs;

	if(pCTRL->shut_ctrl>GetWDR_ShutMax())  pCTRL->shut_ctrl=GetWDR_ShutMax();
	if(pCTRL->shut_ctrl<GetWDR_ShutMin())  pCTRL->shut_ctrl=GetWDR_ShutMin();

	//set each frame accumulation time
	div_offs=pCTRL->rate_long/2;  //offset for round division
	if(pCTRL->ctrl&WDRCTRL_MODE_LINE3) {
		pCTRL->accm_long=(pCTRL->shut_ctrl);
		pCTRL->accm_sh_1=(pCTRL->shut_ctrl*pCTRL->rate_sh_1+div_offs)/pCTRL->rate_long;
		pCTRL->accm_sh_2=(pCTRL->shut_ctrl*pCTRL->rate_sh_2+div_offs)/pCTRL->rate_long;
		pCTRL->accm_sh_3=0;  //(N/A)
	}
	else {  //2-frame merge
		pCTRL->accm_long=(pCTRL->shut_ctrl);
		pCTRL->accm_sh_1=(pCTRL->shut_ctrl*pCTRL->rate_sh_1+div_offs)/pCTRL->rate_long;
		pCTRL->accm_sh_2=0;  //(N/A)
		pCTRL->accm_sh_3=0;  //(N/A)
	}

	//todo : adjust shutter boundary
	/*
	pCTRL->accm_sh_1=MAX(pCTRL->sh_1_min,MIN(pCTRL->sh_1_max,pCTRL->accm_sh_1));
	pCTRL->accm_sh_2=MAX(pCTRL->sh_2_min,MIN(pCTRL->sh_2_max,pCTRL->accm_sh_2));
	*/

	//configure MISP about shutter
	WDRCTRL_SetExpRatio(pCTRL);
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//set lens iris
static void WDRCTRL_SetLensIris(PWDR_CTRL_PARAM pCTRL) {
	if(pCTRL->iris_ctrl>GetWDR_IrisMax())  pCTRL->iris_ctrl=GetWDR_IrisMax();
	if(pCTRL->iris_ctrl<GetWDR_IrisMin())  pCTRL->iris_ctrl=GetWDR_IrisMin();

	pCTRL->lens_iris=pCTRL->iris_ctrl;
	stDebug_Print(2, "WDRCTRL_SetLensIris : %d (%4d (0x%03x))\r\n", pCTRL->iris_ctrl, pCTRL->lens_iris, pCTRL->lens_iris);
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//set AE step speed (Analog Gain)
static void WDRCTRL_SetStepGain(PWDR_CTRL_PARAM pCTRL, MISP_AE_DIR_t dir) {
	WORD refer=pCTRL->gain_ctrl;
	WORD targt_quo=(pCTRL->targt_mid*refer)/pCTRL->cur_Y_sum;
	WORD targt_rem=(pCTRL->targt_mid*refer)%pCTRL->cur_Y_sum;
	WORD targt=(targt_rem>=(pCTRL->cur_Y_sum/2))?(targt_quo+1):targt_quo;

	WORD diff=DIFF(targt,refer);
	WORD step=(diff+AE_STEP_OFF)/AE_STEP_DIV;  //round up
	if(dir==MISP_AE_DIR_INC)
		pCTRL->gain_ctrl=(refer<GetWDR_GainMax()-step)?(refer+step):GetWDR_GainMax();
	else
		pCTRL->gain_ctrl=(refer>step+GetWDR_GainMin())?(refer-step):GetWDR_GainMin();
	pCTRL->stbl_wait=(diff<GAIN_ADJ_DLY)?GAIN_ADJ_DLY:0;
	stDebug_Print(3, "### WDRCTRL_SetStepGain : refer:%d, targt:%d, step:%d (w:%d)\r\n", refer, targt, step, pCTRL->stbl_wait);
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//set AE step speed (Shutter)
static void WDRCTRL_SetStepShut(PWDR_CTRL_PARAM pCTRL, MISP_AE_DIR_t dir) {
	WORD refer=pCTRL->shut_ctrl;
	WORD targt_quo=(pCTRL->targt_mid*refer)/pCTRL->cur_Y_sum;
	WORD targt_rem=(pCTRL->targt_mid*refer)%pCTRL->cur_Y_sum;
	WORD targt=(targt_rem>=(pCTRL->cur_Y_sum/2))?(targt_quo+1):targt_quo;

	WORD diff=DIFF(targt,refer);
	WORD step=(diff+AE_STEP_OFF)/AE_STEP_DIV;  //round up
	if(dir==MISP_AE_DIR_INC)
		pCTRL->shut_ctrl=(refer<GetWDR_ShutMax()-step)?(refer+step):GetWDR_ShutMax();
	else
		pCTRL->shut_ctrl=(refer>step+GetWDR_ShutMin())?(refer-step):GetWDR_ShutMin();
	pCTRL->stbl_wait=(diff<SHUT_ADJ_DLY)?SHUT_ADJ_DLY:0;
	stDebug_Print(3, "### WDRCTRL_SetStepShut : refer:%d, targt:%d, step:%d (w:%d)\r\n", refer, targt, step, pCTRL->stbl_wait);
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//set AE step speed (Iris)
static void WDRCTRL_SetStepIris(PWDR_CTRL_PARAM pCTRL, MISP_AE_DIR_t dir) {
	WORD refer=pCTRL->iris_ctrl;
	WORD targt_quo=(pCTRL->targt_mid*refer)/pCTRL->cur_Y_sum;
	WORD targt_rem=(pCTRL->targt_mid*refer)%pCTRL->cur_Y_sum;
	WORD targt=(targt_rem>=(pCTRL->cur_Y_sum/2))?(targt_quo+1):targt_quo;

	WORD diff=DIFF(targt,refer);
	WORD step=(diff+AE_STEP_OFF)/AE_STEP_DIV;  //round up
	if(dir==MISP_AE_DIR_INC)
		pCTRL->iris_ctrl=(refer<GetWDR_IrisMax()-step)?(refer+step):GetWDR_IrisMax();
	else
		pCTRL->iris_ctrl=(refer>step+GetWDR_IrisMin())?(refer-step):GetWDR_IrisMin();
	pCTRL->stbl_wait=(diff<IRIS_ADJ_DLY)?IRIS_ADJ_DLY:0;
	stDebug_Print(3, "### WDRCTRL_SetStepIris : refer:%d, targt:%d, step:%d (w:%d)\r\n", refer, targt, step, pCTRL->stbl_wait);
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//increase brightness with sensor control
static void WDRCTRL_IncreAeGain(PWDR_CTRL_PARAM pCTRL) {
	if(pCTRL->ctrl&WDRCTRL_AE_IRIS_EN) {  //use shutter, iris, gain
		if( ((pCTRL->shut_ctrl!=GetWDR_ShutMax())&&(pCTRL->iris_ctrl!=GetWDR_IrisMin()))||
			((pCTRL->shut_ctrl!=GetWDR_ShutMax())&&(pCTRL->gain_ctrl!=GetWDR_GainMin()))||
		    ((pCTRL->iris_ctrl!=GetWDR_IrisMax())&&(pCTRL->gain_ctrl!=GetWDR_GainMin())) ) {  //check abnormal state
			pCTRL->shut_ctrl=GetWDR_ShutMax();  WDRCTRL_SetSensShut(pCTRL);
			pCTRL->iris_ctrl=GetWDR_IrisMax();  WDRCTRL_SetLensIris(pCTRL);
			pCTRL->gain_ctrl=GetWDR_GainMin();  WDRCTRL_SetSensGain(pCTRL);
			stDebug_Print(2, "WDRCTRL_IncreAeGain.. recover initial state\r\n");
		}
		else if(pCTRL->shut_ctrl<GetWDR_ShutMax()) {  //increase shutter first
			WDRCTRL_SetStepShut(pCTRL, MISP_AE_DIR_INC);
			WDRCTRL_SetSensShut(pCTRL);
		}
		else if(pCTRL->iris_ctrl<GetWDR_IrisMax()) {  //increase iris second
			WDRCTRL_SetStepIris(pCTRL, MISP_AE_DIR_INC);
			WDRCTRL_SetLensIris(pCTRL);
		}
		else if(pCTRL->gain_ctrl<GetWDR_GainMax()) {  //increase gain last
			WDRCTRL_SetStepGain(pCTRL, MISP_AE_DIR_INC);
			WDRCTRL_SetSensGain(pCTRL);
		}
	}
	else {  //use shutter, gain
		if( (pCTRL->shut_ctrl!=GetWDR_ShutMax())&&(pCTRL->gain_ctrl!=GetWDR_GainMin()) ) {  //check abnormal state
			pCTRL->shut_ctrl=GetWDR_ShutMax();  WDRCTRL_SetSensShut(pCTRL);
			pCTRL->gain_ctrl=GetWDR_GainMin();  WDRCTRL_SetSensGain(pCTRL);
			stDebug_Print(2, "WDRCTRL_IncreAeGain.. recover initial state\r\n");
		}
		else if(pCTRL->shut_ctrl<GetWDR_ShutMax()) {  //increase shutter first
			WDRCTRL_SetStepShut(pCTRL, MISP_AE_DIR_INC);
			WDRCTRL_SetSensShut(pCTRL);
		}
		else if(pCTRL->gain_ctrl<GetWDR_GainMax()) {  //increase gain last
			WDRCTRL_SetStepGain(pCTRL, MISP_AE_DIR_INC);
			WDRCTRL_SetSensGain(pCTRL);
		}
	}

	stDebug_Print(2, "### AE increase (shut:%d(%d~%d), iris:%d(%d~%d), gain:%d(%d~%d))\r\n",
		pCTRL->shut_ctrl, GetWDR_ShutMin(), GetWDR_ShutMax(),
		pCTRL->iris_ctrl, GetWDR_IrisMin(), GetWDR_IrisMax(),
		pCTRL->gain_ctrl, GetWDR_GainMin(), GetWDR_GainMax());

	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//decrease brightness with sensor control
static void WDRCTRL_DecreAeGain(PWDR_CTRL_PARAM pCTRL) {
	if(pCTRL->ctrl&WDRCTRL_AE_IRIS_EN) {  //use shutter, iris, gain
		if( ((pCTRL->shut_ctrl!=GetWDR_ShutMax())&&(pCTRL->iris_ctrl!=GetWDR_IrisMin()))||
			((pCTRL->shut_ctrl!=GetWDR_ShutMax())&&(pCTRL->gain_ctrl!=GetWDR_GainMin()))||
		    ((pCTRL->iris_ctrl!=GetWDR_IrisMax())&&(pCTRL->gain_ctrl!=GetWDR_GainMin())) ) {  //check abnormal state
			pCTRL->shut_ctrl=GetWDR_ShutMax();  WDRCTRL_SetSensShut(pCTRL);
			pCTRL->iris_ctrl=GetWDR_IrisMax();  WDRCTRL_SetLensIris(pCTRL);
			pCTRL->gain_ctrl=GetWDR_GainMin();  WDRCTRL_SetSensGain(pCTRL);
			stDebug_Print(2, "WDRCTRL_DecreAeGain.. recover initial state\r\n");
		}
		else if(pCTRL->gain_ctrl>GetWDR_GainMin()) {  //decrease gain first
			WDRCTRL_SetStepGain(pCTRL, MISP_AE_DIR_DEC);
			WDRCTRL_SetSensGain(pCTRL);
		}
		else if(pCTRL->iris_ctrl>GetWDR_IrisMin()) {  //decrease iris second
			WDRCTRL_SetStepIris(pCTRL, MISP_AE_DIR_DEC);
			WDRCTRL_SetLensIris(pCTRL);
		}
		else if(pCTRL->shut_ctrl>GetWDR_ShutMin()) {  //decrease shutter last
			WDRCTRL_SetStepShut(pCTRL, MISP_AE_DIR_DEC);
			WDRCTRL_SetSensShut(pCTRL);
		}
	}
	else {  //use shutter, gain
		if( (pCTRL->shut_ctrl!=GetWDR_ShutMax())&&(pCTRL->gain_ctrl!=GetWDR_GainMin()) ) {  //check abnormal state
			pCTRL->shut_ctrl=GetWDR_ShutMax();  WDRCTRL_SetSensShut(pCTRL);
			pCTRL->gain_ctrl=GetWDR_GainMin();  WDRCTRL_SetSensGain(pCTRL);
			stDebug_Print(2, "WDRCTRL_DecreAeGain.. recover initial state\r\n");
		}
		else if(pCTRL->gain_ctrl>GetWDR_GainMin()) {  //decrease gain first
			WDRCTRL_SetStepGain(pCTRL, MISP_AE_DIR_DEC);
			WDRCTRL_SetSensGain(pCTRL);
		}
		else if(pCTRL->shut_ctrl>GetWDR_ShutMin()) {  //decrease shutter last
			WDRCTRL_SetStepShut(pCTRL, MISP_AE_DIR_DEC);
			WDRCTRL_SetSensShut(pCTRL);
		}
	}

	stDebug_Print(2, "### AE decrease (shut:%d(%d~%d), iris:%d(%d~%d), gain:%d(%d~%d))\r\n",
		pCTRL->shut_ctrl, GetWDR_ShutMin(), GetWDR_ShutMax(),
		pCTRL->iris_ctrl, GetWDR_IrisMin(), GetWDR_IrisMax(),
		pCTRL->gain_ctrl, GetWDR_GainMin(), GetWDR_GainMax());

	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//write registers about tone-map
static void WDRCTRL_SetTmTarget(PWDR_CTRL_PARAM pCTRL) {
	WORD wVal[5];

	//update write value
	pCTRL->tmw_avg_0 = (pCTRL->tmi_avg_0)/TM_BLEND_DIV;
	pCTRL->tmw_avg_1 = (pCTRL->tmi_avg_1)/TM_BLEND_DIV;
	pCTRL->tmw_max_0 = (pCTRL->tmi_max_0)/TM_BLEND_DIV;
	pCTRL->tmw_max_1 = (pCTRL->tmi_max_1)/TM_BLEND_DIV;

	//adjust TM strength
	pCTRL->tmw_avg_0 = (pCTRL->tmw_avg_0*pCTRL->tone_gain/100);

	//cut-off (max. 20bit)
	pCTRL->tmw_avg_0 = (pCTRL->tmw_avg_0>0x000FFFFF)?0x000FFFFF:pCTRL->tmw_avg_0;
	pCTRL->tmw_avg_1 = (pCTRL->tmw_avg_1>0x000FFFFF)?0x000FFFFF:pCTRL->tmw_avg_1;
	pCTRL->tmw_max_0 = (pCTRL->tmw_max_0>0x000FFFFF)?0x000FFFFF:pCTRL->tmw_max_0;
	pCTRL->tmw_max_1 = (pCTRL->tmw_max_1>0x000FFFFF)?0x000FFFFF:pCTRL->tmw_max_1;

	//write register (tonemap mean, max)
	wVal[0]=(WORD)(pCTRL->tmw_avg_0&0x0000FFFF);
	wVal[1]=(WORD)(pCTRL->tmw_avg_1&0x0000FFFF);
	wVal[2]=(WORD)(pCTRL->tmw_max_0&0x0000FFFF);
	wVal[3]=(WORD)(pCTRL->tmw_max_1&0x0000FFFF);
	wVal[4]=(WORD)(
		((pCTRL->tmw_avg_0&0x000F0000)>>16)+
		((pCTRL->tmw_avg_1&0x000F0000)>>12)+
		((pCTRL->tmw_max_0&0x000F0000)>> 8)+
		((pCTRL->tmw_max_1&0x000F0000)>> 4));

	//adjust TM register
	MISP_RegField(MISP_LOCAL_ID, 0x5FB, 10,1,1);  //hold adjust
	MISP_MultiWrite(MISP_LOCAL_ID, 0x5E8, (PBYTE)wVal, 10);
	MISP_RegField(MISP_LOCAL_ID, 0x5FB, 10,1,0);  //flush

	stDebug_Print(7, "### TM wr(%ld, %ld), it(%ld, %ld), rd(%ld, %ld)\r\n",
		pCTRL->tmw_avg_0, pCTRL->tmw_max_0,
		pCTRL->tmi_avg_0/TM_BLEND_DIV, pCTRL->tmi_max_0/TM_BLEND_DIV,
		pCTRL->tmr_avg_0, pCTRL->tmr_max_0);
}


//--------------------------------------------------------------------------------------------------------------------------
//turn off wdr core process
static void WDRCTRL_ResetProcMode(PWDR_CTRL_PARAM pCTRL) {
	//wdr off
	MISP_RegField(MISP_LOCAL_ID, 0x5BA, 8, 2, 0);  // r_wdr_proc_mode(0:off)

	//set maximum shutter (set before WDRCTRL_SetProcMode)
	//shutter must start with the maximum value to avoid sync loss
#if WDR_STR_MAX_SHT
	if(	(pCTRL->ctrl&WDRCTRL_MODE_LINE2)||
		(pCTRL->ctrl&WDRCTRL_MODE_LINE3) ) {
		pCTRL->shut_hold=MAX(MIN(pCTRL->shut_ctrl,GetWDR_ShutMax()),GetWDR_ShutMin());
		pCTRL->shut_ctrl=GetWDR_ShutMax();
		WDRCTRL_SetSensShut(pCTRL);
	}
#endif
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//turn on wdr core process
static void WDRCTRL_SetProcMode(PWDR_CTRL_PARAM pCTRL) {
	//set wdr proc mode
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:  //Frame by Frame
			MISP_RegField(MISP_LOCAL_ID, 0x5BA, 8, 2, 2);  // r_wdr_proc_mode(2:frame mode)
			break;
#if (!REV_WDR_NO_BUFF)||defined(TEST_DUMP_TONE)
		case WDRCTRL_MODE_BUILT:  //Tone-mapping only
		case WDRCTRL_MODE_SINGL:  //DWDR
			MISP_RegField(MISP_LOCAL_ID, 0x5BA, 8, 2, 3);  // r_wdr_proc_mode(3:double buffer)
			break;
#endif
		case WDRCTRL_MODE_LINE2:  //90hz->30hz
		case WDRCTRL_MODE_LINE3:  //90hz->30hz
			MISP_RegField(MISP_LOCAL_ID, 0x5BA, 8, 2, 1);  // r_wdr_proc_mode(1:linemode)
			break;
		default:  //WDR off
			MISP_RegField(MISP_LOCAL_ID, 0x5BA, 8, 2, 0);  // r_wdr_proc_mode(0:off)
			break;
	}

	//revert shutter (set after WDRCTRL_SetProcMode)
	//shutter must start with the maximum value to avoid sync loss
#if WDR_STR_MAX_SHT
	if(	(pCTRL->ctrl&WDRCTRL_MODE_LINE2)||
		(pCTRL->ctrl&WDRCTRL_MODE_LINE3) ) {
		pCTRL->shut_ctrl=pCTRL->shut_hold;
		WDRCTRL_SetSensShut(pCTRL);
	}
#endif
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//WDR on/off
static void WDRCTRL_ConfigureMisp(PWDR_CTRL_PARAM pCTRL) {
	// Configure MISP Registers
	WDRCTRL_SetPowerSave(pCTRL);
	WDRCTRL_SetMemoryMap(pCTRL);
	WDRCTRL_SetHtmDecomp(pCTRL);
	WDRCTRL_SetDeserializer(pCTRL);
	WDRCTRL_SetHostClk(pCTRL);
	WDRCTRL_SetODM(pCTRL);
	WDRCTRL_SetBayer(pCTRL);
	WDRCTRL_SetCompress(pCTRL);
	WDRCTRL_SetSensorBLC(pCTRL);
	WDRCTRL_SetSensorWrite(pCTRL);
	WDRCTRL_SetMotionFlag(pCTRL);
	WDRCTRL_SetWDRCtrl(pCTRL);
	WDRCTRL_SetWDRSize(pCTRL);
	WDRCTRL_SetExpRatio(pCTRL);
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//store normal(wdr off) mode configurations
static void WDRCTRL_StoreNormalMode(PWDR_SAVE_PARAM pSAVE) {
	//store current AE, gamma
	pSAVE->data_en = TRUE;
	pSAVE->ae_mode = stAECTRL.ctrl&AECTRL_PROCESS_OFF;
	pSAVE->ae_gain = AECTRL_GetGAINGain();
	pSAVE->ae_shut = AECTRL_GetSHUTGain();
#if WDR_IRIS_CTRL_EN
	pSAVE->ae_iris = DCLENS_GetIRISGain();
#endif
	pSAVE->bl_type = GetSS_black_level_type();
	pSAVE->bl_mode = GetSS_black_level_mode();
	pSAVE->bl_mval = GetSS_man_black_level_target();
	pSAVE->bl_oval = GetSS_ODM_black_level_target();
#if WDR_USE_MANU_TM
	pSAVE->gm_bayr = GetBY_gamma_enable();  //store front gamma (on/off)
#else
	pSAVE->gm_bayr = GetBY_gamma_value();   //store front gamma (value)
#endif
#if USE_DEC_POST_BT
	pSAVE->dnr_bit = GetNR_BitMode();
	pSAVE->inp_bit = GetIN_MainMode();
#endif
	pSAVE->at_satu = DEMO_GetAutoSatLevel();
	pSAVE->at_brig = DEMO_GetAutoBright();
	pSAVE->at_edge = DEMO_GetAutoEdgeOnOff();
	pSAVE->at_cont = DEMO_GetAutoConOnOff();
}


//--------------------------------------------------------------------------------------------------------------------------
//substitute normal(wdr off) mode configurations
static void WDRCTRL_SubstNormalMode(PWDR_CTRL_PARAM pCTRL) {
	if(GetWDR_AEProcMode())  stAECTRL.ctrl |= AECTRL_PROCESS_OFF;  //use WDR AE
	else                     stAECTRL.ctrl &=~AECTRL_PROCESS_OFF;  //use ODM AE
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {  //set BLC
#if (!REV_WDR_PREV_WB)
		case WDRCTRL_MODE_FRAME:
		case WDRCTRL_MODE_LINE2:
		case WDRCTRL_MODE_LINE3:
			SetSS_black_level_type(1);  //BLC adjust to ODM only, and WDR uses mgr_offset register
			SetSS_black_level_mode(MISP_BLACK_LEVEL_MANUAL);
			SetSS_man_black_level_target(0);  //n/a
			SetSS_ODM_black_level_target(stWDRSAVE.bl_mval);
			break;
#endif
		case WDRCTRL_MODE_BUILT:
			SetSS_black_level_type(stWDRSAVE.bl_type);
			SetSS_black_level_mode(MISP_BLACK_LEVEL_OFF);
			SetSS_man_black_level_target(0);  //HTM BLC uses bl_pre_in(227h, 5D9h are set in initial code)
			SetSS_ODM_black_level_target(0);  //HTM BLC uses bl_pre_in(227h, 5D9h are set in initial code)
			break;
		default:
			SetSS_black_level_type(stWDRSAVE.bl_type);
			SetSS_black_level_mode(stWDRSAVE.bl_mode);
			SetSS_man_black_level_target(stWDRSAVE.bl_mval);
			SetSS_ODM_black_level_target(stWDRSAVE.bl_oval);
			break;
	}
#if WDR_USE_MANU_TM
	SetBY_gamma_enable(OFF);  //gamma off
#else
	SetBY_gamma_value(pCTRL->bayr_gamm);  //set by SetWDR_TmStrength
#endif
#if USE_DEC_POST_BT
	SetNR_BitMode(0);  //NR 8-bit mode
	SetIN_MainMode(GetIN_MainMode()/2*2);  //Main input 8-bit mode (refer to MISP_SRCPORT_MODE_t)
#endif
	DEMO_SetAutoSaturation(0);  // LowLight_Off
	DEMO_SetAutoBright(0);  // LowLight_Off
	DEMO_SetAutoEdgeOnOff(OFF);
//	DEMO_SetAutoConOnOff(OFF);
}


//--------------------------------------------------------------------------------------------------------------------------
//recall normal(wdr off) mode configurations
static void WDRCTRL_RestrNormalMode(PWDR_SAVE_PARAM pSAVE) {
	if(!pSAVE->data_en)  return;  //no stored data

	//revert AE, gamma
	stAECTRL.ctrl &=~AECTRL_PROCESS_OFF;  //AE on
	stAECTRL.ctrl |= pSAVE->ae_mode;
	AECTRL_SetGAINGain(pSAVE->ae_gain);
	AECTRL_SetSHUTGain(pSAVE->ae_shut);
#if WDR_IRIS_CTRL_EN
	DCLENS_SetIRISGain(pSAVE->ae_iris);
#endif
	SetSS_black_level_type(pSAVE->bl_type);
	SetSS_black_level_mode(pSAVE->bl_mode);
	SetSS_man_black_level_target(pSAVE->bl_mval);
	SetSS_ODM_black_level_target(pSAVE->bl_oval);
#if WDR_USE_MANU_TM
	SetBY_gamma_enable(pSAVE->gm_bayr);    //revert front gamma (on/off)
#else
	SetBY_gamma_value(pSAVE->gm_bayr);     //revert front gamma (value)
#endif
#if USE_DEC_POST_BT
	SetNR_BitMode(pSAVE->dnr_bit);   //NR bit mode
	SetIN_MainMode(pSAVE->inp_bit);  //Main input bit mode
#endif
	// return to off-mode value before restore
	DEMO_SetAutoSaturation(0);  // LowLight_Off
	DEMO_SetAutoBright(0);  // LowLight_Off
	DEMO_SetAutoEdgeOnOff(OFF);
//	DEMO_SetAutoConOnOff(OFF);

	// restore auto-csc func.
	DEMO_SetAutoSaturation(pSAVE->at_satu);
	DEMO_SetAutoBright(pSAVE->at_brig);
	DEMO_SetAutoEdgeOnOff(pSAVE->at_edge);
//	DEMO_SetAutoConOnOff(pSAVE->at_cont);
}


//--------------------------------------------------------------------------------------------------------------------------
//restart AF process
static void WDRCTRL_RestartAFProc(PWDR_CTRL_PARAM pCTRL) {
#if defined(TEST_FWDR_RESET)&&(defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	//set AF frame flag
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:  stAFCTRL.ctrl |= AFCTRL_WDR_FRAME;  break;  //frame by frame
		default:                  stAFCTRL.ctrl &=~AFCTRL_WDR_FRAME;  break;  //other mode
	}

	//release AF process
	stAFCTRL.ctrl &=~AFCTRL_RST_FRAME;
#endif
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//restart WB process
static void WDRCTRL_RestartWBProc(PWDR_CTRL_PARAM pCTRL) {
	//set WB frame flag
	stWBCTRL.ctrl &=~WBCTRL_WDR_MASK;
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
#if defined(TEST_FWDR_RESET)
		case WDRCTRL_MODE_FRAME:  stWBCTRL.ctrl |= WBCTRL_WDR_FRAME;  break;  //frame by frame
#endif
		case WDRCTRL_MODE_BUILT:  stWBCTRL.ctrl |= WBCTRL_WDR_BUILT;  break;  //built in
		case WDRCTRL_MODE_SINGL:  stWBCTRL.ctrl |= WBCTRL_WDR_OFF;    break;  //normal mode
		case WDRCTRL_MODE_LINE2:  stWBCTRL.ctrl |= WBCTRL_WDR_2LINE;  break;  //line by line (2line)
		case WDRCTRL_MODE_LINE3:  stWBCTRL.ctrl |= WBCTRL_WDR_3LINE;  break;  //line by line (3line)
		default:                  stWBCTRL.ctrl |= WBCTRL_WDR_OFF;    break;  //normal mode
	}

	//restart WB process
	stWBCTRL.ctrl |= WBCTRL_PROCESS_RST;
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//restart ODM process
static void WDRCTRL_RestartODMProc(PWDR_CTRL_PARAM pCTRL) {
	WDRCTRL_RestartAFProc(pCTRL);
	WDRCTRL_RestartWBProc(pCTRL);
}


//--------------------------------------------------------------------------------------------------------------------------
//initialize ODM process
static void WDRCTRL_InitODMProc(PWDR_CTRL_PARAM pCTRL) {
	pCTRL->fwdr_cntr=fSysIRQCntODMVD;  //init internal counter
	pCTRL->fwdr_lfid=fSysIRQCntODMVD;  //set long fid
	pCTRL->fwdr_init=1;                //update done
	WDRCTRL_RestartODMProc(pCTRL);  //restart ODM process
}


//--------------------------------------------------------------------------------------------------------------------------
//assume TM input max value
#if (!REV_WDR_MANU_TM)  //bug about TM max read -> assume TM max using satur_level (tma_level)
static void WDRCTRL_AssumeMax(PWDR_CTRL_PARAM pCTRL) {
	//read satur count
	if(pCTRL->tma_phase)  MISP_RegRead(MISP_LOCAL_ID, 0x5A9, &pCTRL->tma_m_cnt);  // read over 'tma_level'
	else                  MISP_RegRead(MISP_LOCAL_ID, 0x5A9, &pCTRL->tma_l_cnt);  // read over 'tma_level-1'

	//decide satur level
	const BYTE step= 5;  //level step size
	const BYTE pthr=50;  //count threshold to determine satur.pixel is exist
	const BYTE nthr=30;  //count threshold to determine satur.pixel is not exist
	if(pCTRL->tma_phase) {
		if((pCTRL->tma_l_cnt>=pthr)&&(pCTRL->tma_m_cnt>=pthr))     // increase threhold
			pCTRL->tma_level=(pCTRL->tma_level<0x0fff-step)?(pCTRL->tma_level+step):0x0fff;
		else if((pCTRL->tma_l_cnt<nthr)&&(pCTRL->tma_m_cnt<nthr))  // decrease threhold
			pCTRL->tma_level=(pCTRL->tma_level>step)?(pCTRL->tma_level-step):0;
	}

	//set satur level
	if(pCTRL->tma_phase)  MISP_RegField(MISP_LOCAL_ID, 0x5A8, 0,12, pCTRL->tma_level-1);  // set tma_l_cnt threshold
	else                  MISP_RegField(MISP_LOCAL_ID, 0x5A8, 0,12, pCTRL->tma_level  );  // set tma_m_cnt threshold

	//for next process
	pCTRL->tma_phase=pCTRL->tma_phase?0:1;
}
#endif


//--------------------------------------------------------------------------------------------------------------------------
//read brightness of current input image
static void WDRCTRL_GetAEYsum(PWDR_CTRL_PARAM pCTRL) {
	BYTE use_odm_data;  //0:use WDR, 1:use ODM data
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_BUILT:  use_odm_data=1;  break;
		case WDRCTRL_MODE_FRAME:  use_odm_data=1;  break;
#if REV_WDR_NEW_MGR  //use odm data to fasten AE response
		case WDRCTRL_MODE_LINE2:  use_odm_data=1;  break;
		case WDRCTRL_MODE_LINE3:  use_odm_data=1;  break;
#endif
		default:                  use_odm_data=0;  break;
	}

	//read Y sum
	if(use_odm_data) pCTRL->cur_Y_sum = (WORD)(MIN(0xFFFF, stAECTRL.Ysum/16));  //from AECTRL_GetAEOPDWIND(), LONG->WORD
	else             MISP_RegRead(MISP_LOCAL_ID, 0x5AA, &pCTRL->cur_Y_sum);     //frame level average (wdr_satur_frame)

	//blend Y sum
	pCTRL->avg_Y_sum = ((AE_BLEND_DIV-1)*(pCTRL->avg_Y_sum/AE_BLEND_DIV))+pCTRL->cur_Y_sum;

	stDebug_Print(3, "### Ysum:%d\r\n", pCTRL->cur_Y_sum);
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//read registers about tone-map
static void WDRCTRL_GetTMmean(PWDR_CTRL_PARAM pCTRL) {  //return auto mean 1st register
	//read register (tonemap mean, max)
	WORD rVal[5];
#if !REV_WDR_MANU_TM
	DWORD shrt_max;
#endif

	MISP_MultiRead(MISP_LOCAL_ID, 0x5ED, (PBYTE)rVal, 10);

	//update read value
	pCTRL->tmr_avg_0 = (DWORD)rVal[0]+(((DWORD)rVal[4]&0x0000000F)<<16);
	pCTRL->tmr_avg_1 = (DWORD)rVal[1]+(((DWORD)rVal[4]&0x000000F0)<<12);
#if REV_WDR_MANU_TM
	pCTRL->tmr_max_0 = (DWORD)rVal[2]+(((DWORD)rVal[4]&0x00000F00)<< 8);
	pCTRL->tmr_max_1 = (DWORD)rVal[3]+(((DWORD)rVal[4]&0x0000F000)<< 4);
#else  //use assumed TM max
	shrt_max = (pCTRL->tma_level+0x03ff)/2;  //estimated short max
	shrt_max = MIN(0x03ff,MAX(0,shrt_max-pCTRL->mgr_offst));  //adjust blc
	pCTRL->tmr_max_0 = shrt_max*pCTRL->rate_long;  //assumed TM max
	pCTRL->tmr_max_1 = 0;  //n/a
#endif
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//read odm data
static void WDRCTRL_GetODMHandler(PWDR_CTRL_PARAM pCTRL) {
#if (!REV_WDR_MANU_TM)  //bug about TM max read -> assume TM max using satur_level
	WDRCTRL_AssumeMax(pCTRL);
#endif

	if( (pCTRL->ctrl&WDRCTRL_MODE_FRAME)&&
	    (pCTRL->fwdr_lfid!=fSysIRQCntODMVD) )  //skip long frame (short data phase)
		return;

	WDRCTRL_GetAEYsum(pCTRL);  //read current Ysum (brightness)
	WDRCTRL_GetTMmean(pCTRL);  //read current tm mean, max
}


//--------------------------------------------------------------------------------------------------------------------------
//auto-exposure handler
static void WDRCTRL_AeGainHandler(PWDR_CTRL_PARAM pCTRL) {
	if(	(!(pCTRL->ctrl&WDRCTRL_AE_PROC_ON))||  //process disable
		(pCTRL->init_cntr<16) )  //not stable
		return;

	if( (pCTRL->ctrl&WDRCTRL_MODE_FRAME)&&
	    (pCTRL->fwdr_lfid!=fSysIRQCntODMVD) )  //skip long frame (short data phase)
		return;

	//update iris/shutter/gain
	if(pCTRL->stbl_wait) {  //wait for stable
		stDebug_Print(3, "ae_wait : %d\r\n", pCTRL->stbl_wait);
	}
	else if(pCTRL->cur_Y_sum< pCTRL->targt_min) {  //increase gain
		stDebug_Print(2, "ae_incr : %d/%d\r\n", pCTRL->cur_Y_sum, pCTRL->targt_mid);
		WDRCTRL_IncreAeGain(pCTRL);
	}
	else if(pCTRL->cur_Y_sum>=pCTRL->targt_max) {  //decrease gain
		stDebug_Print(2, "ae_decr : %d/%d\r\n", pCTRL->cur_Y_sum, pCTRL->targt_mid);
		WDRCTRL_DecreAeGain(pCTRL);
	}
	else {  //hold
		stDebug_Print(3, "ae_hold : %d/%d\r\n", pCTRL->cur_Y_sum, pCTRL->targt_mid);
	}
	if(pCTRL->stbl_wait)  pCTRL->stbl_wait--;
}


//--------------------------------------------------------------------------------------------------------------------------
//tone-map offset handler (i51x only)
static void WDRCTRL_TmOffsHandler(PWDR_CTRL_PARAM pCTRL) {
#if ADAPT_TM_GAIN_EN
	WORD exp_ratio;
	WORD rec_ratio;
	WORD ratio_dist;

	if(	(!(pCTRL->ctrl&WDRCTRL_TM_OFFS_ON))||  //process disable
		(pCTRL->init_cntr<16) ) {  //not stable
		pCTRL->tone_offs=64;  //Q6 format
		return;
	}

	if( (!pCTRL->ctrl&WDRCTRL_MODE_FRAME)&&
		(!pCTRL->ctrl&WDRCTRL_MODE_LINE2)&&
		(!pCTRL->ctrl&WDRCTRL_MODE_LINE3) ) {  //Frame, Line WDR only
		pCTRL->tone_offs=64;  //Q6 format
		return;
	}

	if( (pCTRL->ctrl&WDRCTRL_MODE_FRAME)&&
	    (pCTRL->fwdr_lfid!=fSysIRQCntODMVD) )  //skip long frame (short data phase)
		return;

	//read long-short data (Ysum)
	switch(pCTRL->tma_phase) {
		case 0:  //read short, configure long
			MISP_RegRead(MISP_LOCAL_ID, 0x5AA, &pCTRL->psum_shrt);  // wdr_satur_pxsum (short)
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,12, 2, 3);   // wdr_sat_frm_exp(3) : long frame
			break;
		case 1:  //wait long update
		case 2:
			break;
		case 3:  //read long, configure short
			MISP_RegRead(MISP_LOCAL_ID, 0x5AA, &pCTRL->psum_long);  // wdr_satur_pxsum (long)
#if ADAPT_TM_SHORTER
		  if(pCTRL->ctrl&WDRCTRL_MODE_LINE3)
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,12, 2, 1);   // wdr_sat_frm_exp(1) : shorter frame
		  else
#endif
			MISP_RegField(MISP_LOCAL_ID, 0x5A8,12, 2, 2);   // wdr_sat_frm_exp(2) : short frame
			break;
		case 4:  //wait short update
		case 5:
			break;
	}

	//get long-short data ratio
#if ADAPT_TM_SHORTER
  if(pCTRL->ctrl&WDRCTRL_MODE_LINE3)
	exp_ratio=100*pCTRL->rate_long/pCTRL->rate_sh_2;  //expected ratio (long/shorter)
  else
#endif
	exp_ratio=100*pCTRL->rate_long/pCTRL->rate_sh_1;  //expected ratio (long/short)

	if(pCTRL->tma_phase==6) {  //update offset
		DWORD cur_ratio=100*pCTRL->psum_long/pCTRL->psum_shrt;
		pCTRL->cur_ratio=MIN(100*128,cur_ratio);  //max.ratio 128
	}

	//rectifier (15:1)
	#define rec_blend 16
	pCTRL->pre_ratio=((rec_blend-1)*pCTRL->pre_ratio/rec_blend)+pCTRL->cur_ratio;
	rec_ratio=(WORD)(pCTRL->pre_ratio/rec_blend);
	rec_ratio=MAX(100,MIN(exp_ratio,rec_ratio));

	//decide TM offset
	#define rto_min 40   // ratio lower rto_min : regard as hdr scene
	#define rto_max 100  // ratio upper rto_max : regard as normal scene
	#define off_min 16   // tm max gain : 64(min)~off_min(max)
	ratio_dist=100*rec_ratio/exp_ratio;
	ratio_dist=MIN(rto_max,MAX(rto_min,ratio_dist));  //limit ratio
	pCTRL->tone_offs=(64-off_min)*(ratio_dist-rto_min)/(rto_max-rto_min)+off_min;  //Q6 format (max:64)

	stDebug_Print(6, "### TM cur:%4d(%5d,%5d), ratio:%3d->%3d\r\n",
		pCTRL->cur_ratio, pCTRL->psum_long, pCTRL->psum_shrt, 100*pCTRL->cur_ratio/exp_ratio, ratio_dist);

	//for next process (phase 0~6)
	pCTRL->tma_phase=(pCTRL->tma_phase>=6)?0:(pCTRL->tma_phase+1);
#endif
}


//--------------------------------------------------------------------------------------------------------------------------
//tone-map gain handler
static void WDRCTRL_TmGainHandler(PWDR_CTRL_PARAM pCTRL) {
	if(	(!(pCTRL->ctrl&WDRCTRL_TM_CTRL_ON))||  //process disable
		(pCTRL->fview_sel!=MISP_WDR_FVIEW_WDR) )
		return;

	if( (pCTRL->ctrl&WDRCTRL_MODE_FRAME)&&
	    (pCTRL->fwdr_lfid!=fSysIRQCntODMVD) )  //skip long frame (short data phase)
		return;

	//adjust TM adaptive bias
#if ADAPT_TM_GAIN_EN
	if(pCTRL->ctrl&WDRCTRL_TM_OFFS_ON)
		pCTRL->tmr_avg_0=pCTRL->tmr_avg_0*pCTRL->tone_offs/64;
#endif

	//rectify TM recursive value
	if(pCTRL->init_cntr<20) {  //follows fastly - 2:1 average (20bit)
		BYTE alpha=TM_BLEND_DIV/3;
		pCTRL->tmi_avg_0 = ((TM_BLEND_DIV-alpha)*(pCTRL->tmi_avg_0/TM_BLEND_DIV))+(alpha*pCTRL->tmr_avg_0);
		pCTRL->tmi_avg_1 = ((TM_BLEND_DIV-alpha)*(pCTRL->tmi_avg_1/TM_BLEND_DIV))+(alpha*pCTRL->tmr_avg_1);
		pCTRL->tmi_max_0 = ((TM_BLEND_DIV-alpha)*(pCTRL->tmi_max_0/TM_BLEND_DIV))+(alpha*pCTRL->tmr_max_0);
		pCTRL->tmi_max_1 = ((TM_BLEND_DIV-alpha)*(pCTRL->tmi_max_1/TM_BLEND_DIV))+(alpha*pCTRL->tmr_max_1);
	}
	else {  //follows slowly - 31:1 average (20bit)
		pCTRL->tmi_avg_0 = ((TM_BLEND_DIV-1)*(pCTRL->tmi_avg_0/TM_BLEND_DIV))+pCTRL->tmr_avg_0;
		pCTRL->tmi_avg_1 = ((TM_BLEND_DIV-1)*(pCTRL->tmi_avg_1/TM_BLEND_DIV))+pCTRL->tmr_avg_1;
		pCTRL->tmi_max_0 = ((TM_BLEND_DIV-1)*(pCTRL->tmi_max_0/TM_BLEND_DIV))+pCTRL->tmr_max_0;
		pCTRL->tmi_max_1 = ((TM_BLEND_DIV-1)*(pCTRL->tmi_max_1/TM_BLEND_DIV))+pCTRL->tmr_max_1;
	}

	//update TM target
	WDRCTRL_SetTmTarget(pCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
//auto output saturation handler
static void WDRCTRL_OutCscHandler(PWDR_CTRL_PARAM pCTRL) {
	if(!(pCTRL->ctrl&WDRCTRL_AT_SATU_ON))  //process disable
		return;

	if( (pCTRL->ctrl&WDRCTRL_MODE_FRAME)&&
	    (pCTRL->fwdr_lfid!=fSysIRQCntODMVD) )  //skip long frame (short data phase)
		return;

	//get Ysum
	WORD sum_min=64;   //input min(dark)
	WORD sum_max=420;  //input max(bright)
#if   WDR_SENS_3M_RES  // (2048x1536)/(1920x1080)
	sum_min=sum_min*3/2;  sum_max=sum_max*3/2;
#elif WDR_SENS_1M_RES  // (1280x720)/(1920x1080)
	sum_min=sum_min*4/9;  sum_max=sum_max*4/9;
#endif

	//get satur range ('sum_min~sum_max' to 'sat_min~sat_max')
	BYTE sat_val;
	if(pCTRL->satur_max>pCTRL->satur_min) {
		DWORD ae_ysum=pCTRL->avg_Y_sum/AE_BLEND_DIV;
		DWORD tm_bias=(ae_ysum>sum_min)?(ae_ysum-sum_min):0;
		DWORD sat_cal=tm_bias*(pCTRL->satur_max-pCTRL->satur_min)/(sum_max-sum_min)+pCTRL->satur_min;
		sat_val=MIN(pCTRL->satur_max,sat_cal);
	}
	else {  // abnormal case
		sat_val=pCTRL->satur_min;
	}

	//update output CSC
//	UARTprintf("sat_val:%d\r\n", sat_val);
	if(GetOUT_MainSaturation()!=sat_val)  SetOUT_MainSaturation(sat_val);
	if(GetOUT_AuxSaturation ()!=sat_val)  SetOUT_AuxSaturation (sat_val);
}

//--------------------------------------------------------------------------------------------------------------------------
//update deblur/deghost state (i500 only)
static void WDRCTRL_DeblurHandler(PWDR_CTRL_PARAM pCTRL) {
#if (!REV_WDR_NEW_MGR)
	if(!(pCTRL->ctrl&WDRCTRL_MD_CTRL_ON))  //process disable
		return;

	if( (pCTRL->ctrl&WDRCTRL_MODE_FRAME)&&
	    (pCTRL->fwdr_lfid!=fSysIRQCntODMVD) )  //skip long frame (short data phase)
		return;

	//threshold to decide low light condition
	const WORD low_input_thr=1400;

	//decide MD on/off
	//count low light frame
	if(	(pCTRL->ctrl&WDRCTRL_AE_PROC_ON)&&
		(pCTRL->cur_Y_sum<low_input_thr) ) {  //low input brightness
		pCTRL->dark_cntr=(pCTRL->dark_cntr<BLUR_MAX_CNT)?pCTRL->dark_cntr+1:pCTRL->dark_cntr;
	}
	else {
		pCTRL->dark_cntr=(pCTRL->dark_cntr>0)?pCTRL->dark_cntr-1:0;
	}
	stDebug_Print(4, "!!! Low light : %d\r\n", pCTRL->dark_cntr);

	//turn off MD in the low light state
	WORD rval;
	MISP_RegRead(MISP_LOCAL_ID, 0x5DC, &rval);
	if(rval&0x8000) {  //mgr_motiondetect_en : bit.15
		if(	(pCTRL->mgr_md_th==0)||
			(pCTRL->dark_cntr==BLUR_MAX_CNT) ) {  //low light
			stDebug_Print(1, "!!! Deghost off\r\n");
			pCTRL->deghst_en=0;
			MISP_RegField(MISP_LOCAL_ID, 0x5DC,15, 1, 0);  // mgr_motiondetect_en
		}
	}
	else {
		if(	(pCTRL->mgr_md_th!=0)&&
			(pCTRL->dark_cntr==0) ) {  //normal light
			stDebug_Print(1, "!!! Deghost on\r\n");
			pCTRL->deghst_en=1;
			MISP_RegField(MISP_LOCAL_ID, 0x5DC,15, 1, 1);  // mgr_motiondetect_en
		}
	}

	//control mergenum
	if(	(pCTRL->deghst_en==0)&&(pCTRL->mgr_md_th!=0)&&  //auto deghost off
		(pCTRL->fview_sel==MISP_WDR_FVIEW_WDR) ) {  //wdr on
		//turn to 2-frame merge
		if(pCTRL->ctrl&WDRCTRL_MODE_LINE3)
			MISP_RegField(MISP_LOCAL_ID, 0x59C, 0, 2, 1);  // wdr_mergenum(2->1) : exclude shorter (use long+short)
	}
	else {
		//revert to 3-frame merge
		if(pCTRL->ctrl&WDRCTRL_MODE_LINE3)
			MISP_RegField(MISP_LOCAL_ID, 0x59C, 0, 2, 2);  // wdr_mergenum(3->1) : use shoter
	}
#endif
}


//--------------------------------------------------------------------------------------------------------------------------
//check FID and reset AF/WB
static void WDRCTRL_OdmFidHandler(PWDR_CTRL_PARAM pCTRL) {
	//init(restart) ODM process
	BYTE init_odm=0;
#if USE_FWDR_FID_EN  //enable frame mode fid control
	WORD rFid;
#endif
	BYTE reset_fid;

//issue a delay (wdr_info updates in the ODM vsync timing)
#if defined(__USE_IMX265_1536P_ALL_SCAN_MODE__)
	const WORD fid_dly = 2;
#else
	const WORD fid_dly = 1;
#endif

#if USE_FWDR_FID_EN  //enable frame mode fid control
	if(pCTRL->ctrl&WDRCTRL_MODE_FRAME) {
		if(pCTRL->init_cntr==8) {
			//caution!! prohibited read wdr_info right after ODM vsync.
			Delay_mSec(fid_dly);  //issue a delay (wdr_info updates in the ODM vsync timing)
			MISP_RegRead(MISP_LOCAL_ID, 0x56F, &rFid);  //read wdr_info (frame id)
#if defined(__USE_CMOS_IMX273__)
	#if defined(__USE_IMX273_ROI_MODE__) || defined(__OUT_VCLKPLL_LOCK__)
			if((rFid&0x0001)==0x0001)  init_odm=1;
	#else
			if((rFid&0x0001)!=0x0001)  init_odm=1;
	#endif
#elif (defined(__USE_CMOS_IMX249__) || defined(__USE_CMOS_IMX174__))|\
	 (defined(__USE_IMX265_1536P_ALL_SCAN_MODE__) && defined(__USE_IMX265_SLAVE_MODE__))
			if((rFid&0x0001)!=0x0001)  init_odm=1;
#else
			if((rFid&0x0001)==0x0001)  init_odm=1;
#endif
		}
		else if((pCTRL->init_cntr==9)&&(pCTRL->fwdr_init==0)) {  //not initialized yet (WDRCTRL_InitODMProc() not called)
			init_odm=1;
		}
	}
	else
#endif
	if(pCTRL->init_cntr==8) {
		init_odm=1;
	}

	//sync FID : compare inter, system FID
	if( (pCTRL->init_cntr>=12)&&                 //wait for stable
		(pCTRL->ctrl&WDRCTRL_MODE_FRAME)&&       //frame mode
		(pCTRL->fwdr_cntr!=fSysIRQCntODMVD)&&    //sync fail !!
	    (pCTRL->fwdr_lfid==fSysIRQCntODMVD) ) {  //reset at long frame
		reset_fid=1;
	}
	else {
		reset_fid=0;
	}

	//init, reset fid
	if(init_odm) {  //init counter, reset odm
		stDebug_Print(1, "### init ODM process !! \r\n");
		WDRCTRL_InitODMProc(pCTRL);
	}
	else if(reset_fid) {  //reset counter, odm
		stDebug_Print(1, "### restart ODM process !! \r\n");
		pCTRL->fwdr_cntr=fSysIRQCntODMVD;
		WDRCTRL_RestartODMProc(pCTRL);
	}

	//increase for next process
	if(pCTRL->fwdr_cntr>=pCTRL->fwdr_fnum)
		pCTRL->fwdr_cntr=0;
	else
		pCTRL->fwdr_cntr++;
}


//--------------------------------------------------------------------------------------------------------------------------
//set sensor shutter
static void WDRCTRL_UpdateShutVal(PWDR_CTRL_PARAM pCTRL) {
//================================================================//
// [pana DOL style (mn34220)]                                     //
//                           shut                                 //
//                 |2|      |3|                                   //
//         ##(e1)##   #(e2)#  ##########(long)############        //
//  |---------|------------|-----------------------------|        //
//     FSC-long-3-e2-2   FSC-long-3                  FSC(fixed)   //
//================================================================//
// [pana DOL style (mn34420)]                                     //
//                           shut                                 //
//  |s1|      | s2|           |                                   //
//     ##(e1)##   #(e2)#      ##########(long)############        //
//  |---------|--------|---------------------------------|        //
//         s1+e1  s1+e1+s2+e2                        FSC(fixed)   //
//================================================================//
// [sony DOL style]                                               //
//    shut       shs2        shs3                                 //
//     |          |           |                                   //
//     ##(s1)##   #(s2)#      ##########(long)############        //
//  |---------|--------|---------------------------------|        //
//           rhs1     rhs2                           FSC(fixed)   //
//================================================================//
	WORD shut_max;

	if(!(stAECTRL.ctrl&AECTRL_PROCESS_OFF))  //use normal AE
		return;

	//shutter max
#if defined(__USE_CMOS_MN34220__)||\
    defined(__USE_CMOS_MN34420__)
	shut_max=0x0463;  //refer to AE_UpdateHandler()
#elif defined(__USE_CMOS_IMX273__)
  #if defined(__USE_IMX273_ALLSCAN_MODE__)
	shut_max=1500;
  #elif defined(__USE_IMX273_ROI_MODE__)
	shut_max=750;  
  #else
	shut_max=1100;
  #endif
#elif defined(__USE_IMX265_1536P_ALL_SCAN_MODE__)
	shut_max=1650;
#else
  #if   WDR_SENS_3M_RES
	shut_max=0x0640;  //vmax:1600
  #elif WDR_SENS_1M_RES
	shut_max=0x02EE;  //vmax:750
  #else
	shut_max=0x0465;  //vmax:1125
  #endif
#endif
#if defined(__VTOTAL_CTRL_PAL__)
	if(GetSS_SensorVoutPAL())  shut_max*=1.2f;
#endif

	//set shutter
#if defined(__USE_CMOS_MN34220__)||\
    defined(__USE_CMOS_MN34041__)
	//PANA sensor
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_LINE2:
		case WDRCTRL_MODE_LINE3:  //line mode
			stAEXFER.shut = shut_max-pCTRL->accm_long;
			stAEXFER.exp1 = pCTRL->accm_sh_1;
			stAEXFER.exp2 = pCTRL->accm_sh_2;
			break;
		case WDRCTRL_MODE_FRAME:  //toggle shutter (shutter reflects in the n+2 frame)
			if(pCTRL->fwdr_cntr==pCTRL->fwdr_lfid)
				stAEXFER.shut = shut_max-pCTRL->accm_sh_1;
			else
				stAEXFER.shut = shut_max-pCTRL->accm_long;
			break;
		default:
			break;
	}

#elif defined(__USE_CMOS_MN34420__)
	//PANA sensor
	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_LINE3:  //line mode
			stAEXFER.shut = shut_max-pCTRL->accm_long;      //shutter long
			stAEXFER.exp1 = pCTRL->accm_sh_1;  //exposure short
			stAEXFER.exp2 = pCTRL->accm_sh_2;  //exposure shorter
			stAEXFER.gap1 = pCTRL->readout_1-pCTRL->accm_sh_1;  //exposure gap (long-short)
			stAEXFER.gap2 = pCTRL->readout_2-pCTRL->readout_1-pCTRL->accm_sh_2;  //exposure gap (short-shorter)
			break;
		default:
			break;
	}

#elif defined(__USE_CMOS_IMXx36__)||\
      defined(__USE_CMOS_IMX174__)||\
      defined(__USE_CMOS_IMX249__)||\
      defined(__USE_CMOS_IMX123__)||\
      defined(__USE_CMOS_IMX124__)||\
      defined(__USE_CMOS_IMX290__)||\
      defined(__USE_CMOS_IMX327__)||\
      defined(__USE_CMOS_IMX385__)||\
      defined(__USE_CMOS_IMX185__)||\
      defined(__USE_CMOS_IMX265__)||\
      defined(__USE_CMOS_IMX273__)

  #if defined(__USE_CMOS_IMX123__)||\
      defined(__USE_CMOS_IMX124__)||\
      defined(__USE_IMX265_1536P_ALL_SCAN_MODE__)      
  	BYTE shs_mrg=0;
  #else
  	BYTE shs_mrg=1;
  #endif

	switch(pCTRL->ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_LINE3:  //3frame
			stAEXFER.rhs1 = pCTRL->readout_1;                             //readout short
			stAEXFER.shut = pCTRL->readout_1-(pCTRL->accm_sh_1+shs_mrg);  //shutter short
			stAEXFER.rhs2 = pCTRL->readout_2;                             //readout shorter
			stAEXFER.shs2 = pCTRL->readout_2-(pCTRL->accm_sh_2+shs_mrg);  //shutter shorter
			stAEXFER.shs3 = (shut_max*4)-(pCTRL->accm_long+shs_mrg);      //shutter long
			break;
		case WDRCTRL_MODE_LINE2:  //2frame
			stAEXFER.rhs1 = pCTRL->readout_1;                             //readout short
			stAEXFER.shut = pCTRL->readout_1-(pCTRL->accm_sh_1+shs_mrg);  //shutter short
			stAEXFER.rhs2 = 0;                                            //(N/A)
			stAEXFER.shs2 = (shut_max*2)-(pCTRL->accm_long+shs_mrg);      //shutter long
			stAEXFER.shs3 = 0;                                            //(N/A)
			break;
		case WDRCTRL_MODE_FRAME:
			stAEXFER.shut = shut_max-pCTRL->accm_sh_1;
			stAEXFER.shs2 = shut_max-pCTRL->accm_long;
			//printf("### shut. max:%d, shut:%d, shs2:%d, xfer_shut:%ld, xfer_shs2:%d, func_max:%ld\n", shut_max, pCTRL->accm_sh_1, pCTRL->accm_long, stAEXFER.shut, stAEXFER.shs2, CMOS_GetVmax());
			break;
		case WDRCTRL_MODE_BUILT:  //2frame
  #if defined(__USE_CMOS_IMX123__)||\
      defined(__USE_CMOS_IMX124__)
			stAEXFER.shut = (shut_max*2)-pCTRL->accm_sh_1;
			stAEXFER.shs2 = (shut_max*2)-pCTRL->accm_long;
  #else
			stAEXFER.shut = shut_max-pCTRL->accm_sh_1;
			stAEXFER.shs2 = shut_max-pCTRL->accm_long;
  #endif
			break;
		default:
			break;
	}

#else  //toggle shutter
	switch(pCTRL->fwdr_cntr) {
		case 0:  stAEXFER.shut = shut_max-pCTRL->accm_sh_1;  break;
		case 1:  stAEXFER.shut = shut_max-pCTRL->accm_sh_2;  break;
		case 2:  stAEXFER.shut = shut_max-pCTRL->accm_sh_3;  break;
		default: stAEXFER.shut = shut_max-pCTRL->accm_long;  break;
	}

#endif

	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//set sensor gain
static void WDRCTRL_UpdateGainVal(PWDR_CTRL_PARAM pCTRL) {
	if(!(stAECTRL.ctrl&AECTRL_PROCESS_OFF))  //use normal AE
		return;

	//set sensor gain
	stAEXFER.gain = MAKEDWORD(pCTRL->sns_dgain, pCTRL->sns_again);
	stAEXFER.fine = MAKEDWORD(0x0000, 0x0100);  //odm_dc_gain x1.0
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//set lens iris
static void WDRCTRL_UpdateIrisVal(PWDR_CTRL_PARAM pCTRL) {
	if(!(stAECTRL.ctrl&AECTRL_PROCESS_OFF))  //use normal AE
		return;

	//set iris gain (refer to DCLENS_IRISCtrlGainConv()!!)
#if defined(__USE_X30X_TAMRON__)||defined(__USE_X33X_CHINA__)||defined(__USE_X10X_UNION__)||defined(__USE_OLPF_SUNNY__)||defined(__USE_X10X_ZMAX__)||defined(__USE_X03X_ZMAX__)||defined(__USE_X12X_UNION__)
	stAEXFER.iris = MAKEDWORD(0x0000, 1023-pCTRL->lens_iris);
#else
	stAEXFER.iris = MAKEDWORD(0x0000, pCTRL->lens_iris);
#endif
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//control i2c sensor
static void WDRCTRL_UpdateI2cSens(PWDR_CTRL_PARAM pCTRL) {
#if defined(__USE_CMOS_PO3100K__)
	BYTE data_long[3];
	BYTE data_shrt[3];

	data_long[0]=HIBYTE(pCTRL->accm_long);  //msb
	data_long[1]=LOBYTE(pCTRL->accm_long);  //lsb
	data_long[2]=0;  //fraction

	data_shrt[0]=HIBYTE(pCTRL->accm_sh_1);  //msb
	data_shrt[1]=LOBYTE(pCTRL->accm_sh_1);  //lsb
	data_shrt[2]=0;  //fraction

	//set shutter
	CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x01);  //page b
	switch(pCTRL->fwdr_cntr) {
		case 0:  CMOS_MultiWrite(I2C_PO3100K_ADDR,0xC0,data_long,3);  break;
		default: CMOS_MultiWrite(I2C_PO3100K_ADDR,0xC0,data_shrt,3);  break;
	}

	//set gain
	CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x01);  //page b
	CMOS_ByteWrite(I2C_PO3100K_ADDR,0xC3,pCTRL->sns_again);   //a.gain
	CMOS_ByteWrite(I2C_PO3100K_ADDR,0xC4,pCTRL->sns_dgain);   //d.gain

#elif defined(__USE_CMOS_AR0331__)
	//temp code.. exposure(accumulation) time
	//maximum coarse_integration_time = minimum(70 x T1/T2, frame_length_lines - 71)
	//  T1/T2 : 16x (0x3082)
	//  frame_length_lines : 1125 (0x465)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3012, 1054);

#endif
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//configure Sensor WDR on/off
static void WDRCTRL_ConfigureSens(PWDR_CTRL_PARAM pCTRL) {
#if defined(__USE_CMOS_MN34220__)
	if     (pCTRL->ctrl&WDRCTRL_MODE_LINE2)  MN34220_WDR_Mode(MN34220_DOL3);  //deserializer does not support 4ch wdr
	else if(pCTRL->ctrl&WDRCTRL_MODE_LINE3)  MN34220_WDR_Mode(MN34220_DOL3);
	else if(pCTRL->ctrl&WDRCTRL_MODE_FRAME)  MN34220_WDR_Mode(MN34220_WDR);
	else                                     MN34220_WDR_Mode(MN34220_NORMAL);

#elif defined(__USE_CMOS_MN34420__)
	if     (pCTRL->ctrl&WDRCTRL_MODE_LINE3)  MN34420_WDR_Mode(MN34420_DOL3);
	else                                     MN34420_WDR_Mode(MN34420_NORMAL);

#elif defined(__USE_CMOS_IMXx36__)
	if     (pCTRL->ctrl&WDRCTRL_MODE_FRAME)  IMX136_WDR_Mode(IMX136_WDR);
	else if(pCTRL->ctrl&WDRCTRL_MODE_BUILT)  IMX136_WDR_Mode(IMX136_HTM);
	else                                     IMX136_WDR_Mode(IMX136_NORMAL);

#elif defined(__USE_CMOS_IMX123__)||\
      defined(__USE_CMOS_IMX124__)
	if     (pCTRL->ctrl&WDRCTRL_MODE_LINE3)  IMX124_WDR_Mode(IMX124_DOL3);
	else if(pCTRL->ctrl&WDRCTRL_MODE_LINE2)  IMX124_WDR_Mode(IMX124_DOL2);
	else if(pCTRL->ctrl&WDRCTRL_MODE_BUILT)  IMX124_WDR_Mode(IMX124_HTM);
	else if(pCTRL->ctrl&WDRCTRL_MODE_FRAME)  IMX124_WDR_Mode(IMX124_WDR);
	else                                     IMX124_WDR_Mode(IMX124_NORMAL);

#elif defined(__USE_CMOS_IMX290__)
	if     (pCTRL->ctrl&WDRCTRL_MODE_LINE3)  IMX290_WDR_Mode(IMX290_DOL3);
	else if(pCTRL->ctrl&WDRCTRL_MODE_LINE2)  IMX290_WDR_Mode(IMX290_DOL2);
	else if(pCTRL->ctrl&WDRCTRL_MODE_FRAME)  IMX290_WDR_Mode(IMX290_WDR);
	else                                     IMX290_WDR_Mode(IMX290_NORMAL);

#elif defined(__USE_CMOS_IMX327__)
	if(pCTRL->ctrl&WDRCTRL_MODE_FRAME)  IMX290_WDR_Mode(IMX290_WDR);
	else                                     IMX290_WDR_Mode(IMX290_NORMAL);
	
#elif defined(__USE_CMOS_IMX385__)
	if     (pCTRL->ctrl&WDRCTRL_MODE_LINE3)  IMX385_WDR_Mode(IMX385_DOL3);
	else if(pCTRL->ctrl&WDRCTRL_MODE_LINE2)  IMX385_WDR_Mode(IMX385_DOL2);
	else if(pCTRL->ctrl&WDRCTRL_MODE_FRAME)  IMX385_WDR_Mode(IMX385_WDR);
	else                                     IMX385_WDR_Mode(IMX385_NORMAL);

#elif defined(__USE_CMOS_IMX174__)||\
      defined(__USE_CMOS_IMX249__)
	if     (pCTRL->ctrl&WDRCTRL_MODE_FRAME)  IMX174_WDR_Mode(IMX174_WDR);
	else                                     IMX174_WDR_Mode(IMX174_NORMAL);

#elif defined(__USE_CMOS_IMX185__)
	if     (pCTRL->ctrl&WDRCTRL_MODE_FRAME)  IMX185_WDR_Mode(IMX185_WDR);
	else                                     IMX185_WDR_Mode(IMX185_NORMAL);
#elif defined(__USE_CMOS_IMX265__)
	if     (pCTRL->ctrl&WDRCTRL_MODE_FRAME)  IMX265_WDR_Mode(IMX265_WDR);
	else                                     IMX265_WDR_Mode(IMX265_NORMAL);
#elif defined(__USE_CMOS_IMX273__)
	if     (pCTRL->ctrl&WDRCTRL_MODE_FRAME)  IMX273_WDR_Mode(IMX273_WDR);
	else                                     IMX273_WDR_Mode(IMX273_NORMAL);

#elif defined(__USE_CMOS_AR0331__)
	if     (pCTRL->ctrl&WDRCTRL_MODE_BUILT)  AR0331_WDR_Initialize();
	else                                     AR0331_Initialize();
#endif

	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//select frame view select, tone-map strength
static void WDRCTRL_SetTonemapReg(PWDR_CTRL_PARAM pCTRL) {
#if REV_WDR_NEW_MGR
	BYTE wdr_thsel;  //0:long, 1:sh_1, 2:sh_2
#endif
	//set mgr
	if(pCTRL->fview_sel==MISP_WDR_FVIEW_WDR) {  //WDR enable
		MISP_RegField(MISP_LOCAL_ID, 0x59D, 12,1,1);  //wdr_process_en
		MISP_RegField(MISP_LOCAL_ID, 0x5B4, 12,3,0);  //tm_out_bit_scope
	}
	else {  //bypass
		MISP_RegField(MISP_LOCAL_ID, 0x59D, 12,1,0);  //wdr_process_en=0
		if(	(pCTRL->ctrl&WDRCTRL_MODE_BUILT)||
			(pCTRL->ctrl&WDRCTRL_MODE_SINGL) ) {  //tone-mapping mode
			switch(pCTRL->fview_sel) {  //display 12bit of 20bit TM bypass output
				case 1:  MISP_RegField(MISP_LOCAL_ID, 0x5B4, 12,3,7);  break;  //tm_out_bit_scope : 7[11:0]
				case 2:  MISP_RegField(MISP_LOCAL_ID, 0x5B4, 12,3,5);  break;  //tm_out_bit_scope : 5[13:2]
				case 3:  MISP_RegField(MISP_LOCAL_ID, 0x5B4, 12,3,4);  break;  //tm_out_bit_scope : 4[15:4]
			}
		}
		else {  //select mgr_input
			MISP_RegField(MISP_LOCAL_ID, 0x5B4, 12,3,pCTRL->inp_shift);  //tm_out_bit_scope (bitwise left shift to 12bit)
		}
	}

	//select bypass frame
#if REV_WDR_NEW_MGR
	if(pCTRL->fview_sel==MISP_WDR_FVIEW_WDR)  wdr_thsel=0;  //normal WDR
	else if (pCTRL->ctrl&WDRCTRL_MODE_FRAME)  wdr_thsel=(pCTRL->fview_sel==1)?0:2;
	else if (pCTRL->ctrl&WDRCTRL_MODE_LINE2)  wdr_thsel=(pCTRL->fview_sel==1)?0:2;
	else if (pCTRL->ctrl&WDRCTRL_MODE_LINE3)  wdr_thsel=(pCTRL->fview_sel==1)?0:(pCTRL->fview_sel==2)?1:2;
	else                                      wdr_thsel=0;
	MISP_RegField(MISP_LOCAL_ID, 0x59C, 2,2, wdr_thsel);
#else
	if     (pCTRL->ctrl&WDRCTRL_MODE_LINE2)  MISP_RegField(MISP_LOCAL_ID, 0x59C, 2,2, 1-pCTRL->fview_sel+1);  //wdr_thsel (x, 1, 0)
	else if(pCTRL->ctrl&WDRCTRL_MODE_LINE3)  MISP_RegField(MISP_LOCAL_ID, 0x59C, 2,2, 2-pCTRL->fview_sel+1);  //wdr_thsel (x, 2, 1, 0)
	else                                     MISP_RegField(MISP_LOCAL_ID, 0x59C, 2,2, pCTRL->fwdr_fnum-pCTRL->fview_sel+1);  //wdr_thsel (x, 1, 0)
#endif

	//set tm
	if(pCTRL->fview_sel==MISP_WDR_FVIEW_WDR) {  //WDR enable
		pCTRL->ctrl |= WDRCTRL_TM_CTRL_ON;  //release TM
#if REV_WDR_MANU_TM
		MISP_RegField(MISP_LOCAL_ID, 0x5B9, 9,2,3);  //tm_en:3 (use TM_0,1)
#else
		MISP_RegField(MISP_LOCAL_ID, 0x5B9, 9,2,2);  //tm_en:2 (use TM_0) - bug about TM max read
#endif
		WDRCTRL_SetTmTarget(pCTRL);  //update TM target
	}
	else {  //bypass
		pCTRL->ctrl &=~WDRCTRL_TM_CTRL_ON;  //hold TM
		MISP_RegField(MISP_LOCAL_ID, 0x5B9, 9,2,0);  //tm_en:0
	}

	//set gamma
#if WDR_USE_MANU_TM
	if(pCTRL->fview_sel==MISP_WDR_FVIEW_WDR)  SetBY_gamma_enable(OFF);
	else                                      SetBY_gamma_enable(stWDRSAVE.gm_bayr);
#else
	if(pCTRL->fview_sel==MISP_WDR_FVIEW_WDR)  SetBY_gamma_value(pCTRL->bayr_gamm);  //by SetWDR_TmStrength
	else                                      SetBY_gamma_value(stWDRSAVE.gm_bayr);
#endif

	return;
}


// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
//wdr loop handler
void WDR_ProcessHandler(void) {
	//WDR initialize
	if(stWDRCTRL.ctrl&WDRCTRL_TRIGGER_ISP) {  //init 1st step
		stDebug_Print(1, "WDRCTRL_TRIGGER_ISP:0x%08x\r\n", stWDRCTRL.ctrl);
		stWDRCTRL.ctrl &=~WDRCTRL_TRIGGER_ISP;  //clear flag
		stWDRCTRL.init_cntr=0;  //reset counter
		stWDRCTRL.fwdr_init=0;  //reset flag

		SetOUT_MainDarkScreen(ON);          //close curtain to hide transition
		WDRCTRL_ResetProcMode(&stWDRCTRL);  //standby proc_mode
		WDRCTRL_ConfigureSens(&stWDRCTRL);  //set sensor registers
		WDRCTRL_ConfigureMisp(&stWDRCTRL);  //set misp registers
	}
	if(stWDRCTRL.init_cntr== 3)  SetSS_reset_des_fifo();  //reset deserializer fifo
	if(stWDRCTRL.init_cntr== 4)  WDRCTRL_SetProcMode(&stWDRCTRL);  //set proc_mode (+2frame after shutter set)
#if REV_WDR_NEW_MGR
	if(stWDRCTRL.init_cntr==18)  SetOUT_MainDarkScreen(OFF);  //open curtain
#else
	if(stWDRCTRL.init_cntr==24)  SetOUT_MainDarkScreen(OFF);  //open curtain
#endif

	//WDR process
	WDRCTRL_OdmFidHandler(&stWDRCTRL);  //check FID and reset AF/WB
	if( (stWDRCTRL.init_cntr>=2)&&
	    (stWDRCTRL.ctrl&WDRCTRL_PROCESS_ON) ) {  //WDR normal process
		WDRCTRL_GetODMHandler(&stWDRCTRL);  //read odm data
		WDRCTRL_AeGainHandler(&stWDRCTRL);  //update ae(shutter/iris/gain) value
		WDRCTRL_DeblurHandler(&stWDRCTRL);  //update deblur/deghost state (i500 only)
		WDRCTRL_TmOffsHandler(&stWDRCTRL);  //update tonemap offset value (i51x only)
		WDRCTRL_TmGainHandler(&stWDRCTRL);  //update tonemap gain value
		WDRCTRL_OutCscHandler(&stWDRCTRL);  //update CSC(saturation) controls
		WDRCTRL_UpdateShutVal(&stWDRCTRL);  //set stAEXFER about shutter
		WDRCTRL_UpdateGainVal(&stWDRCTRL);  //set stAEXFER about gain
		WDRCTRL_UpdateIrisVal(&stWDRCTRL);  //set stAEXFER about iris
		WDRCTRL_UpdateI2cSens(&stWDRCTRL);  //control i2c bus sensor
	}
	else {  // turn off WDR shutters explicitly
#if defined(__USE_CMOS_MN34220__)
		stAEXFER.exp1=stAEXFER.exp2=0;
#elif defined(__USE_CMOS_MN34420__)
		stAEXFER.exp1=stAEXFER.exp2=stAEXFER.gap1=stAEXFER.gap2=0;
#else
		stAEXFER.shs2=stAEXFER.shs3=stAEXFER.rhs1=stAEXFER.rhs2=0;
#endif
	}

	//add counter
	if(stWDRCTRL.init_cntr <63)  stWDRCTRL.init_cntr++;
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//set auto exposure mode
BYTE SetWDR_AutoExpose(WORD nID) {
	stWDRCTRL.auto_mode=nID;

	//check current mode
	if(!GetWDR_AEProcMode())
		return 1;  //use normal AE

	//set auto mode initial shutter/gain/iris
	if(stWDRCTRL.auto_mode==MISP_WDR_AE_AUTO) {
		WDRCTRL_SetSensGain(&stWDRCTRL);
		WDRCTRL_SetSensShut(&stWDRCTRL);
		WDRCTRL_SetLensIris(&stWDRCTRL);
	}

	//set mode
	if(stWDRCTRL.auto_mode==MISP_WDR_AE_AUTO)
		stWDRCTRL.ctrl |= WDRCTRL_AE_PROC_ON;
	else
		stWDRCTRL.ctrl &=~WDRCTRL_AE_PROC_ON;

	return 0;
}


//--------------------------------------------------------------------------------------------------------------------------
//set input brightness target
BYTE SetWDR_AutoTarget(WORD nID) {
	FLOAT offset;
	BYTE rto_mul;
	BYTE rto_den;
	
	//set gain target
	stWDRCTRL.brightnes=MIN(19,nID);

	//set target offset : pow(10,(offset+0.03*nID))
	if(stWDRCTRL.ctrl&WDRCTRL_MODE_BUILT) {  //built-in mode
#if REV_ODM_LEV_FIX
		offset=2.83f;  //use [ 9:0](odm) data (center is nearby 1440)
#else
		offset=2.35f;  //use [11:2](odm) data (center is nearby 480)
#endif
	}
	else if(stWDRCTRL.ctrl&WDRCTRL_MODE_FRAME) {  //frame mode
		offset=2.83f;  //use long(odm) frame (center is nearby 1440)
	}
	else {  //line mode
#if REV_WDR_NEW_MGR
		offset=2.83f;  //use long(odm) frame (center is nearby 1440)
#else
		offset=2.89f;  //use short(wdr) frame (center is nearby 1660)
#endif
	}

	//set auto mode target (FHD base)
	stWDRCTRL.targt_min=(WORD)pow(10,offset+0.03f*(stWDRCTRL.brightnes+0));
	stWDRCTRL.targt_mid=(WORD)pow(10,offset+0.03f*(stWDRCTRL.brightnes+1));
	stWDRCTRL.targt_max=(WORD)pow(10,offset+0.03f*(stWDRCTRL.brightnes+2));

	//adjust resolution dependent bias
#if   WDR_SENS_3M_RES  // (2048x1536)/(1920x1080)
	rto_mul=3; rto_den=2;
#elif WDR_SENS_1M_RES  // (1280x720)/(1920x1080)
	rto_mul=4; rto_den=9;
#else
	rto_mul=1; rto_den=1;
#endif
	stWDRCTRL.targt_min=stWDRCTRL.targt_min*rto_mul/rto_den;
	stWDRCTRL.targt_mid=stWDRCTRL.targt_mid*rto_mul/rto_den;
	stWDRCTRL.targt_max=stWDRCTRL.targt_max*rto_mul/rto_den;

	stDebug_Print(1, "set ae target : %d ~ %d\r\n", stWDRCTRL.targt_min, stWDRCTRL.targt_max);
	return 0;
}


//--------------------------------------------------------------------------------------------------------------------------
//set sensor gain
BYTE SetWDR_SensorGain(WORD nID) {
	if(stWDRCTRL.auto_mode==MISP_WDR_AE_MANU) {
		stWDRCTRL.gain_ctrl=nID;
		stDebug_Print(1, "SetWDR_SensorGain (%d)\r\n", nID);
		WDRCTRL_SetSensGain(&stWDRCTRL);
	}
	return 0;
}


//--------------------------------------------------------------------------------------------------------------------------
//set sensor shutter
BYTE SetWDR_SensorShut(WORD nID) {
	if(stWDRCTRL.auto_mode==MISP_WDR_AE_MANU) {
		stWDRCTRL.shut_ctrl=nID;
		stDebug_Print(1, "WDRCTRL_SetSensShut (%d)\r\n", nID);
		WDRCTRL_SetSensShut(&stWDRCTRL);
	}
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
//set lens iris
BYTE SetWDR_LensDcIris(WORD nID) {
	if(stWDRCTRL.auto_mode==MISP_WDR_AE_MANU) {
		stWDRCTRL.iris_ctrl=nID;
		stDebug_Print(1, "WDRCTRL_SetLensIris (%d)\r\n", nID);
		WDRCTRL_SetLensIris(&stWDRCTRL);
	}
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
//set exposure ratio
BYTE SetWDR_ExposRatio(WORD nID) {
	switch(stWDRCTRL.ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:
			if(nID==0) { stWDRCTRL.rate_long=16;  stWDRCTRL.rate_sh_1=1;  stWDRCTRL.rate_sh_2=0; }
			else       { stWDRCTRL.rate_long=32;  stWDRCTRL.rate_sh_1=1;  stWDRCTRL.rate_sh_2=0; }
			break;
		case WDRCTRL_MODE_LINE2:
			nID=L2WDR_HOLD_RATIO?(L2WDR_HOLD_RATIO-1):nID;  // check fixed ratio
			if(nID==0) { stWDRCTRL.rate_long=16;  stWDRCTRL.rate_sh_1=1;  stWDRCTRL.rate_sh_2=0; }
			else       { stWDRCTRL.rate_long=32;  stWDRCTRL.rate_sh_1=1;  stWDRCTRL.rate_sh_2=0; }
			break;
		case WDRCTRL_MODE_LINE3:
			nID=L3WDR_HOLD_RATIO?(L3WDR_HOLD_RATIO-1):nID;  // check fixed ratio
			if(nID==0) { stWDRCTRL.rate_long=36;  stWDRCTRL.rate_sh_1=6;  stWDRCTRL.rate_sh_2=1; }
			else       { stWDRCTRL.rate_long=64;  stWDRCTRL.rate_sh_1=8;  stWDRCTRL.rate_sh_2=1; }
			break;
		default:
			break;
	}
	WDRCTRL_SetSensShut(&stWDRCTRL);
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
//set auto-saturation level
BYTE SetWDR_SaturLevel(BYTE val) {
	// set WDR saturation range (AUTO_SAT_MIN~val)
	stWDRCTRL.satur_max=val;
	stWDRCTRL.satur_min=(val>=AUTO_SAT_MIN)?AUTO_SAT_MIN:(val/4);
	stDebug_Print(1, "SetWDR_SaturLevel (%d~%d)\r\n", stWDRCTRL.satur_min, stWDRCTRL.satur_max);
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
//set mgr md threshold
BYTE SetWDR_MdRegister(WORD nID) {
#if (!REV_WDR_NEW_MGR)
	const WORD thres_set[15][2]={
		//cam(14bit.Q12), off(16bit,Q10)
		{0x3FFF, 0xFFFF},  //MD off
		{0x319A, 0x0100},  //3.1, 256
		{0x2E66, 0x00E0},  //2.9, 224
		{0x2B33, 0x00C0},  //2.7, 192
		{0x2800, 0x00A0},  //2.5, 160
		{0x24CD, 0x0080},  //2.3, 128
		{0x219A, 0x0060},  //2.1,  96
		{0x1E66, 0x0040},  //1.9,  64
		{0x1B33, 0x0020},  //1.7,  32  (recommend)
		{0x1800, 0x0010},  //1.5,  16
		{0x14CD, 0x0000},  //1.3,   0  (default)
		{0x119A, 0x0000},  //1.1,   0
		{0x0E66, 0x0000},  //0.9,   0
		{0x0B33, 0x0000},  //0.7,   0
		{0x0800, 0x0000}}; //0.5,   0

	stWDRCTRL.mgr_md_th=nID;

	//md_enable
//	if(stWDRCTRL.mgr_md_th)  MISP_RegField(MISP_LOCAL_ID, 0x5DC,15, 1, 1);  //md_en=1
//	else                     MISP_RegField(MISP_LOCAL_ID, 0x5DC,15, 1, 0);  //md_en=0

	//md threshold
	MISP_RegWrite(MISP_LOCAL_ID, 0x5E5, thres_set[stWDRCTRL.mgr_md_th][0]);  //mgr_cam_param
	MISP_RegWrite(MISP_LOCAL_ID, 0x5E6, thres_set[stWDRCTRL.mgr_md_th][1]);  //nlut_boft
	return 0;
#else
	return 1;  //failure : i500 only
#endif
}


//--------------------------------------------------------------------------------------------------------------------------
//set wdr processing mode (on/off)
BYTE SetWDR_ProcesMode(MISP_WDR_PROC_t mode) {
	//current state
	stWDRCTRL.state_wdr=GetWDR_ProcesMode();

	//set WDR control flags
	switch(mode) {
		case MISP_WDR_PROC_FRAME:  //Frame by Frame
			stWDRCTRL.ctrl=WDRCTRL_PROCESS_ON+WDRCTRL_MODE_FRAME;
			stDebug_Print(1, "### WDR on (Frame by Frame)       \r\n");
			break;
		case MISP_WDR_PROC_BUILT:  //Tone-map only
			stWDRCTRL.ctrl=WDRCTRL_PROCESS_ON+WDRCTRL_MODE_BUILT;
			stDebug_Print(1, "### WDR on (Tone-map only)        \r\n");
			break;
		case MISP_WDR_PROC_SINGL:  //DWDR
			stWDRCTRL.ctrl=WDRCTRL_PROCESS_ON+WDRCTRL_MODE_SINGL;
			stDebug_Print(1, "### WDR on (DWDR)                 \r\n");
			break;
		case MISP_WDR_PROC_LINE2:  //Line by Line (2-Frame)
			stWDRCTRL.ctrl=WDRCTRL_PROCESS_ON+WDRCTRL_MODE_LINE2;
			stDebug_Print(1, "### WDR on (Line by Line - 2Frame)\r\n");
			break;
		case MISP_WDR_PROC_LINE3:  //Line by Line (3-Frame)
			stWDRCTRL.ctrl=WDRCTRL_PROCESS_ON+WDRCTRL_MODE_LINE3;
			stDebug_Print(1, "### WDR on (Line by Line - 3Frame)\r\n");
			break;
		default:  //clear all flags
			stWDRCTRL.ctrl=0;
			stDebug_Print(1, "### WDR off                       \r\n");
			break;
	}

	//configure H-split image
	switch(stWDRCTRL.ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:
		case WDRCTRL_MODE_LINE2:
		case WDRCTRL_MODE_LINE3:
#if (!REV_WDR_NO_BUFF)
		case WDRCTRL_MODE_BUILT:
		case WDRCTRL_MODE_SINGL:
#endif
#if WDR_SENS_3M_RES
			stWDRCTRL.ctrl |= WDRCTRL_SPLIT_NUM8;
#else
			stWDRCTRL.ctrl |= WDRCTRL_SPLIT_NUM4;
#endif
			break;
		default:
			break;
	}

	//store normal mode AE/Gamma/etc..
	if(	(stWDRCTRL.state_wdr==MISP_WDR_PROC_DISBL)&&
		(stWDRCTRL.ctrl&WDRCTRL_PROCESS_ON) ) {  //off->on
		//store normal mode
		WDRCTRL_StoreNormalMode(&stWDRSAVE);
		WDRCTRL_SubstNormalMode(&stWDRCTRL);
	}
	else if( (stWDRCTRL.state_wdr!=MISP_WDR_PROC_DISBL)&&
		(!(stWDRCTRL.ctrl&WDRCTRL_PROCESS_ON)) ) {  //on->off
		//recall normal mode
		WDRCTRL_RestrNormalMode(&stWDRSAVE);
	}
	else if( (stWDRCTRL.state_wdr!=MISP_WDR_PROC_DISBL)&&
		(stWDRCTRL.ctrl&WDRCTRL_PROCESS_ON) ) { //on->on
		//substitute normal mode functions
		WDRCTRL_SubstNormalMode(&stWDRCTRL);
	}

	//set frame count (for frame mode)
	if(stWDRCTRL.ctrl&WDRCTRL_MODE_FRAME)
		stWDRCTRL.fwdr_fnum=1;  //2->1 merge
	else
		stWDRCTRL.fwdr_fnum=0;  //n/a

	//control TM by uCom
#if WDR_USE_MANU_TM
	if(stWDRCTRL.ctrl&WDRCTRL_PROCESS_ON) {
		stWDRCTRL.ctrl |= WDRCTRL_TM_CTRL_ON;
	}
#endif

	//TM offset control
#if ADAPT_TM_GAIN_EN
	if( (stWDRCTRL.ctrl&WDRCTRL_MODE_FRAME)||
		(stWDRCTRL.ctrl&WDRCTRL_MODE_LINE2)||
		(stWDRCTRL.ctrl&WDRCTRL_MODE_LINE3) ) {
#if USE_GLOBAL_SHUT  // disable for global shutter (not tested)
		stWDRCTRL.ctrl &=~WDRCTRL_TM_OFFS_ON;
#else
		stWDRCTRL.ctrl |= WDRCTRL_TM_OFFS_ON;
#endif
	}
#endif

	//deghost control
#if (!REV_WDR_NEW_MGR)
	if(	(stWDRCTRL.ctrl&WDRCTRL_MODE_FRAME)||
		(stWDRCTRL.ctrl&WDRCTRL_MODE_LINE2)||
		(stWDRCTRL.ctrl&WDRCTRL_MODE_LINE3) ) {
		stWDRCTRL.ctrl |= WDRCTRL_MD_CTRL_ON;
	}
#endif

	//auto saturtion control
	if( (stWDRCTRL.ctrl&WDRCTRL_MODE_FRAME)||
		(stWDRCTRL.ctrl&WDRCTRL_MODE_LINE2)||
		(stWDRCTRL.ctrl&WDRCTRL_MODE_LINE3) ) {
#if USE_GLOBAL_SHUT  // disable for global shutter
		stWDRCTRL.ctrl &=~WDRCTRL_AT_SATU_ON;
#else
		stWDRCTRL.ctrl |= WDRCTRL_AT_SATU_ON;
#endif
	}

	//set mgr input bit mode
	switch(stWDRCTRL.ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:
		case WDRCTRL_MODE_LINE2:
		case WDRCTRL_MODE_LINE3:
			stWDRCTRL.ctrl |= WDRCTRL_IN_RES_10B;
			stWDRCTRL.inp_shift=2;  //bitwise right shift to remove dummy LSB
			break;
		default:
			stWDRCTRL.ctrl |= WDRCTRL_IN_RES_12B;
			stWDRCTRL.inp_shift=0;  //tone-map input
			break;
	}

	//set shutter/iris control mode
#if WDR_IRIS_CTRL_EN
	if((stAECTRL.ctrl&AECTRL_LENS_MASK)==AECTRL_DCIRIS_LENS)
		stWDRCTRL.ctrl |= WDRCTRL_AE_IRIS_EN;
#endif

	//set shutter ratio (non-built-in mode would be set by SetWDR_ExposRatio())
	if(stWDRCTRL.ctrl&WDRCTRL_MODE_BUILT) {
#if defined(__USE_IMX123_HTM_X32__)
		stWDRCTRL.rate_long=32;  stWDRCTRL.rate_sh_1=1;  stWDRCTRL.rate_sh_2=0;
#else
		stWDRCTRL.rate_long=16;  stWDRCTRL.rate_sh_1=1;  stWDRCTRL.rate_sh_2=0;
#endif
	}

	//check initial AE value
	if( (stWDRCTRL.ctrl&WDRCTRL_MODE_LINE3)||
	    (stWDRCTRL.ctrl&WDRCTRL_MODE_LINE2)||
	    (stWDRCTRL.ctrl&WDRCTRL_MODE_BUILT)||
	    (stWDRCTRL.ctrl&WDRCTRL_MODE_FRAME) ) {
		WDRCTRL_SetShutParams(&stWDRCTRL);  //set shutter params

		//init for the [on->on] case, else hold previous value
		stWDRCTRL.gain_ctrl = (stWDRCTRL.state_wdr)?GetWDR_GainMin():stWDRCTRL.gain_ctrl;
		stWDRCTRL.shut_ctrl = (stWDRCTRL.state_wdr)?GetWDR_ShutMax():stWDRCTRL.shut_ctrl;
		stWDRCTRL.iris_ctrl = (stWDRCTRL.state_wdr)?GetWDR_IrisMax():stWDRCTRL.iris_ctrl;
	}

	//seize AF process before sensor mode transitions
#if defined(TEST_FWDR_RESET)&&(defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	stAFCTRL.ctrl |= AFCTRL_RST_FRAME;
#endif

	//seize sensor write before configure
	SetSS_wdr_write_mode(MISP_WDR_MEM_WIRTE_OFF_MODE, 0, 0x00);

#if defined(TEST_60HZ_INPUT)
	//use 8bit input
	stWDRCTRL.ctrl &=~WDRCTRL_IN_RES_MASK;
	stWDRCTRL.ctrl |= WDRCTRL_IN_RES_08B;

	//turn off aux to get bandwidth margin
	if(stWDRCTRL.ctrl&WDRCTRL_PROCESS_ON) {  //AUX off
		SetOUT_AuxFreezeEN(ON);
		SetOUT_AuxDispEN(OFF);
	}
	else {  //AUX on
		SetOUT_AuxFreezeEN(OFF);
		SetOUT_AuxDispEN(ON);
	}
#endif

	//throw trigger to WDR_ProcessHandler()
	stWDRCTRL.ctrl |= WDRCTRL_TRIGGER_ISP;

	return 0;
}


//--------------------------------------------------------------------------------------------------------------------------
//bypass wdr input (0:wdr, 1~:bypass)
BYTE SetWDR_BypassPath(MISP_WDR_FVIEW_t nID) {
	stWDRCTRL.fview_sel=nID;
	WDRCTRL_SetTonemapReg(&stWDRCTRL);
	return 0;
}


//--------------------------------------------------------------------------------------------------------------------------
//set tone-map strength
BYTE SetWDR_TmStrength(WORD nID) {
#if WDR_USE_MANU_TM
	//S/W strength (0:weak ~ 29:strong)
  #if REV_WDR_MANU_TM
	WORD tgain_tbl[30]={  //mid 200%(0x0C8), step x1.1
		0x343, 0x2F7, 0x2B2, 0x273, 0x23A, 0x206, 0x1D7, 0x1AC,
		0x185, 0x162, 0x142, 0x124, 0x10A, 0x0F2, 0x0DC, 0x0C8,
		0x0B5, 0x0A5, 0x096, 0x088, 0x07C, 0x070, 0x066, 0x05D,
		0x054, 0x04D, 0x046, 0x03F, 0x039, 0x034 };
  #else
	WORD tgain_tbl[30]={  //mid  72%(0x048), step x1.1
		0x12C, 0x111, 0x0F8, 0x0E1, 0x0CD, 0x0BA, 0x0A9, 0x09A,
		0x08C, 0x07F, 0x073, 0x069, 0x05F, 0x057, 0x04F, 0x048,
		0x041, 0x03B, 0x036, 0x031, 0x02C, 0x028, 0x024, 0x021,
		0x01E, 0x01B, 0x019, 0x016, 0x014, 0x012 };
  #endif
	stWDRCTRL.tone_gain=tgain_tbl[nID];

#else
	//H/W strength (0:weak ~ 19:strong)
	stWDRCTRL.bayr_gamm=nID+3;  //set bayer gamma (3~11)

#endif

	WDRCTRL_SetTonemapReg(&stWDRCTRL);
	return 0;
}


//--------------------------------------------------------------------------------------------------------------------------
//set tone-map contrast using local window size control
BYTE SetWDR_TmContrast(WORD nID) {
#if WDR_TM_SIZE_CTRL
	BYTE lsize0, lsize1;  //1,5,9
	BYTE lgain0, lgain1;  //0~8
	switch(nID) {
		case 0:   lsize0=5;  lsize1=1;  lgain0=lgain1=3;  break;
		case 1:   lsize0=5;  lsize1=1;  lgain0=lgain1=4;  break;
		case 2:   lsize0=9;  lsize1=9;  lgain0=lgain1=4;  break;  //IP default
		case 3:   lsize0=9;  lsize1=9;  lgain0=lgain1=5;  break;
		case 4:   lsize0=9;  lsize1=9;  lgain0=lgain1=6;  break;
		default:  lsize0=9;  lsize1=9;  lgain0=lgain1=7;  break;
	}
	MISP_RegField(MISP_LOCAL_ID, 0x5AE,12, 4, lsize0);  //tm_lsize_0
	MISP_RegField(MISP_LOCAL_ID, 0x5AE, 8, 4, lsize1);  //tm_lsize_1
	MISP_RegField(MISP_LOCAL_ID, 0x5B0,12, 4, lgain0);  //tm_lgain_0
	MISP_RegField(MISP_LOCAL_ID, 0x5B0, 8, 4, lgain1);  //tm_lgain_1
	return 0;
#else
	return 1;  //failure : i500 n/a
#endif
}


//--------------------------------------------------------------------------------------------------------------------------
//restart WDR process
BYTE SetWDR_ProcessRst(void) {
	stWDRCTRL.state_wdr=GetWDR_ProcesMode();  //to restore normal mode
	stWDRCTRL.ctrl |= WDRCTRL_TRIGGER_ISP;
	return 0;
}


//--------------------------------------------------------------------------------------------------------------------------
//get processing mode (for menu)
MISP_WDR_PROC_t GetWDR_ProcesMode(void) {
	BYTE Mode;
	switch(stWDRCTRL.ctrl&WDRCTRL_MODE_MASK) {
		case WDRCTRL_MODE_FRAME:  Mode=MISP_WDR_PROC_FRAME;  break;
		case WDRCTRL_MODE_BUILT:  Mode=MISP_WDR_PROC_BUILT;  break;
		case WDRCTRL_MODE_SINGL:  Mode=MISP_WDR_PROC_SINGL;  break;
		case WDRCTRL_MODE_LINE2:  Mode=MISP_WDR_PROC_LINE2;  break;
		case WDRCTRL_MODE_LINE3:  Mode=MISP_WDR_PROC_LINE3;  break;
		default:                  Mode=MISP_WDR_PROC_DISBL;  break;
	}
	return (MISP_WDR_PROC_t)Mode;
}


//--------------------------------------------------------------------------------------------------------------------------
//get WDR process mode list
MISP_WDR_PROC_t GetWDR_ProcesList(BYTE nID) {
	MISP_WDR_PROC_t mode_tbl[]={
		MISP_WDR_PROC_DISBL,
#if WDR_SENS_DOL_3FRM
		MISP_WDR_PROC_LINE3,
#endif
#if WDR_SENS_DOL_2FRM
		MISP_WDR_PROC_LINE2,
#endif
#if WDR_SENS_BUILT_IN
		MISP_WDR_PROC_BUILT,
#endif
#if WDR_SENS_MULT_EXP
		MISP_WDR_PROC_FRAME,
#endif
#if WDR_SENS_BYPAS_IN
		MISP_WDR_PROC_SINGL,
#endif
	};
	BYTE mode_num=sizeof(mode_tbl)/sizeof(MISP_WDR_PROC_t);

	BYTE mode;
	if(nID>=mode_num) {
		mode=mode_tbl[mode_num-1];
		stDebug_Print(1, "### GetWDR_ProcesList : out of range (%d / %d)\r\n", nID, mode_num-1);
	}
	else {
		mode=mode_tbl[nID];
	}
	return (MISP_WDR_PROC_t)mode;
}


//--------------------------------------------------------------------------------------------------------------------------
//get AE possession (for menu)
BOOL GetWDR_AEProcMode(void) {
	//ae control
	if(	(stWDRCTRL.ctrl&WDRCTRL_MODE_FRAME)||
		(stWDRCTRL.ctrl&WDRCTRL_MODE_BUILT)||
		(stWDRCTRL.ctrl&WDRCTRL_MODE_LINE2)||
		(stWDRCTRL.ctrl&WDRCTRL_MODE_LINE3) )
		return TRUE;   //AE by WDR
	else
		return FALSE;  //AE by ODM
}


//--------------------------------------------------------------------------------------------------------------------------
//gain control min/max index (refer to WDRCTRL_SetSensGain())
WORD GetWDR_GainMin(void) {
	return (  40);  //min: 0dB = 20*log10(0.025*[40])
}
WORD GetWDR_GainMax(void) {
#if defined(__USE_CMOS_MN34220__)||\
    defined(__USE_CMOS_MN34420__)
  #if WDR_DOL_OVER_SYN
	//if the analog gain changes as runtime, transaction error will be occurred in short frame.
	//because short frame output timing would be exceeded 1V period.
	//optical level varies if analog gain is over 7.5dB. and it causes white-balance problem.
	return (  94);  //max:7.5dB
  #else
	return (1264);  //max:30dB
  #endif
#elif defined(__USE_CMOS_IMXx36__)||\
      defined(__USE_CMOS_IMX174__)||\
      defined(__USE_CMOS_IMX249__)||\
      defined(__USE_CMOS_IMX185__)||\
      defined(__USE_CMOS_IMX265__)||\
      defined(__USE_CMOS_IMX273__)
	return ( 633);  //max:24dB
#elif defined(__USE_CMOS_IMX123__)||\
      defined(__USE_CMOS_IMX327__)||\
      defined(__USE_CMOS_IMX124__)
	return ( 895);  //max:27dB
#elif defined(__USE_CMOS_IMX290__)||\
      defined(__USE_CMOS_IMX385__)
	return (1264);  //max:30dB
#else
	return GetWDR_GainMin();  //not implemented.. 40=(pow(10,0dB/20)/0.025)
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
//shutter control min/max index
WORD GetWDR_ShutMin(void) {
	return stWDRCTRL.expos_min;
}
WORD GetWDR_ShutMax(void) {
	return stWDRCTRL.expos_max;
}

//--------------------------------------------------------------------------------------------------------------------------
//iris control min/max index
WORD GetWDR_IrisMin(void) {
#if WDR_IRIS_CTRL_EN
	return MIN(450,DCLENS_GetIRISMax());  //linear range (450~DCLENS_GetIRISMax())
#else
	return 0 ;
#endif
}
WORD GetWDR_IrisMax(void) {
#if WDR_IRIS_CTRL_EN
	return DCLENS_GetIRISMax();
#else
	return 0;
#endif
}


//--------------------------------------------------------------------------------------------------------------------------
//wdr result write bit mode (depends on sensor h-active size)
BYTE GetWDR_RwBitMode(void) {
#if defined(__USE_10BIT_OPERATION__)
  #if   WDR_WRITE_EN_10B
	const BYTE mode=MISP_DDRMAP_10b;
  #elif WDR_WRITE_EN_09B
	const BYTE mode=MISP_DDRMAP_9b;
  #else
	const BYTE mode=MISP_DDRMAP_8b;
  #endif
#else  //8bit operation
  #if   WDR_WRITE_EN_08B
	const BYTE mode=MISP_DDRMAP_8b;
  #elif WDR_WRITE_EN_09B
	const BYTE mode=MISP_DDRMAP_9b;
  #else
	const BYTE mode=MISP_DDRMAP_10b;
  #endif
#endif
	return mode;
}


//--------------------------------------------------------------------------------------------------------------------------
//set MGR demo mode
BYTE SetWDR_DemoMode(BYTE mode, BYTE fsel) {
#if REV_WDR_NEW_MGR
	if(mode)  stWDRCTRL.ctrl &=~WDRCTRL_TM_CTRL_ON;   // hold TM
	else      stWDRCTRL.ctrl |= WDRCTRL_TM_CTRL_ON;   // release TM
	MISP_RegField(MISP_LOCAL_ID, 0x43B, 2, 2, mode);  // r_demo_mode
	MISP_RegField(MISP_LOCAL_ID, 0x59C, 2, 2, fsel);  // wdr_thsel
	return 0;
#else
	return 1;  //failure : i500 n/a
#endif
}


//--------------------------------------------------------------------------------------------------------------------------
//get frame state : returns '1' if the aquired ODM data(T-1) is valid(long)
BOOL GetWDR_ODMValidFrame(void) {
	BOOL odm_en;
	if(!(stWDRCTRL.ctrl&WDRCTRL_MODE_FRAME))       odm_en=TRUE;  //no frame WDR
	else if(stWDRCTRL.fwdr_init==0)                odm_en=TRUE;  //fid is not updated yet (WDRCTRL_OdmFidHandler)
	else if(stWDRCTRL.fwdr_lfid==fSysIRQCntODMVD)  odm_en=TRUE;  //long frame is similar to normal brightness
	else                                           odm_en=FALSE;

#if 0
	WORD WND_sum = (WORD)(MIN(0xFFFF, stAECTRL.Ysum/16));  //LONG->WORD
	stDebug_Print(0, "### ODM (%d) %4d\r\n", odm_en, WND_sum);
#endif

	return odm_en;
}


//--------------------------------------------------------------------------------------------------------------------------
//compress on/off
BYTE SetWDR_CompressCtrl(MISP_COMP_MODE_t mode) {
#if USE_IPC_COMPRES
	BOOL cmpr_en, c420_en;
	switch(mode) {
		case MISP_COMP_ON:   cmpr_en=TRUE;   c420_en=TRUE;   break;
		case MISP_COMP_420:  cmpr_en=FALSE;  c420_en=TRUE;   break;
		default:             cmpr_en=FALSE;  c420_en=FALSE;  break;
	}
	stWDRCTRL.comp_mode=mode;

	//set decomp register
	MISP_RegField(MISP_LOCAL_ID, 0x213, 13, 1, c420_en);  //r_420mode_en
	MISP_RegField(MISP_LOCAL_ID, 0x213, 12, 1, cmpr_en);  //r_compress_en

	//set compress register
	MISP_RegField(MISP_LOCAL_ID, 0x05D,  1, 1, c420_en);  //c420_en
	MISP_RegField(MISP_LOCAL_ID, 0x05D,  0, 1, cmpr_en);  //compress_en
	MISP_RegField(MISP_LOCAL_ID, 0x05D,  3, 1, cmpr_en);  //cbcr_block_mux
	return 0;
#else
	return 1;  //failure : i500 only
#endif
}


//--------------------------------------------------------------------------------------------------------------------------
//compress on/off
MISP_COMP_MODE_t GetWDR_CompressCtrl(void) {
#if USE_IPC_COMPRES
	WORD rVal;
	MISP_RegRead(MISP_LOCAL_ID, 0x05D, &rVal);
	if     (rVal&0x0001)  return MISP_COMP_ON;   //bit.0 (compress_en)
	else if(rVal&0x0002)  return MISP_COMP_420;  //bit.1 (c420_en)
	else                  return MISP_COMP_OFF;
#else
	return MISP_COMP_OFF;
#endif
}


//--------------------------------------------------------------------------------------------------------------------------
//force_def_quant
BYTE SetWDR_CompressDefault(BOOL On) {
#if USE_IPC_COMPRES
	MISP_RegField(MISP_LOCAL_ID, 0x05D,  5, 1, (On)? 1: 0);  // 'force_def_quant'
	return 0;
#else
	return 1;  //failure : i500 only
#endif
}


//--------------------------------------------------------------------------------------------------------------------------
//set compress ratio
BYTE SetWDR_CompressMode(WORD nID) {  //0:80%, 1:60%
#if USE_IPC_COMPRES
	ROMDATA WORD defQuantBit80[]={
		0x5666, 0x6778, 0x5556, 0x6778
	};
	ROMDATA WORD LumQuantBit80[]={
		0x8888, 0x8777, 0x5677, 0x7887,	0x5677, 0x7887, 0x2466, 0x6787,  //T00, T01, T02, T03
		0x2334, 0x6787, 0x0022, 0x4788,	0x0002, 0x2489, 0x0000, 0x2249,  //T04, T05, T06, T07
		0x7888, 0x8766, 0x6667, 0x7776,	0x2466, 0x6787, 0x2246, 0x6787,  //T10, T11, T12, T13
		0x0224, 0x6788, 0x0022, 0x4788, 0x0002, 0x2489, 0x0000, 0x2249,  //T14, T15, T16, T17
		0x6677, 0x7665, 0x5567, 0x7776, 0x2466, 0x7787, 0x2246, 0x6787,  //T20, T21, T22, T23
		0x0224, 0x6788, 0x0022, 0x4788,	0x0002, 0x2489, 0x0000, 0x2249,  //T24, T25, T26, T27
	};
	ROMDATA WORD defQuantBit60[]={
		0x2334, 0x5678, 0x2224, 0x5678
	};
	ROMDATA WORD LumQuantBit60[]={
		0x7777, 0x7666, 0x5677, 0x7776,	0x5566, 0x6776, 0x3455, 0x5776,  //T00, T01, T02, T03
		0x2334, 0x5687, 0x0023, 0x4687,	0x0002, 0x3488, 0x0000, 0x2249,  //T04, T05, T06, T07
		0x7766, 0x6666, 0x5666, 0x6776,	0x5566, 0x6666, 0x3455, 0x5666,  //T10, T11, T12, T13
		0x2334, 0x5676, 0x0023, 0x4676, 0x0002, 0x3477, 0x0000, 0x2248,  //T14, T15, T16, T17
		0x6666, 0x6666, 0x5666, 0x6666, 0x5555, 0x5666, 0x3455, 0x5566,  //T20, T21, T22, T23
		0x2334, 0x5566, 0x0023, 0x4566,	0x0002, 0x3467, 0x0000, 0x2247,  //T24, T25, T26, T27
	};

	WORD h_size=GetSS_image_active_size_h();  //1920
	switch(nID)	{
		case 0:		//80%
			MISP_RegField(MISP_LOCAL_ID, 0x05E,  0, 14, h_size*8 * 8/10);  // comp_max_bits_y [input]
			MISP_RegField(MISP_LOCAL_ID, 0x05F,  0, 14, h_size*8 * 6/10);  // comp_max_bits_c 
			MISP_RegField(MISP_LOCAL_ID, 0x213,  0, 12, h_size   * 8/10);  // r_hsize_compress [ipc]
			MISP_RegField(MISP_LOCAL_ID, 0x214,  0, 12, h_size   * 6/10);  // r_hsize_compress_c 
			
			MISP_MultiWrite(MISP_HOST_ID, 0x1F0, (PBYTE)defQuantBit80, sizeof(defQuantBit80));  // defquantbit [host]
			MISP_MultiWrite(MISP_HOST_ID, 0x1C0, (PBYTE)LumQuantBit80, sizeof(LumQuantBit80));  // lumquantbit
			break;
		case 1:		//60%
			MISP_RegField(MISP_LOCAL_ID, 0x05E,  0, 14, h_size*8 * 6/10);  // comp_max_bits_y [input]
			MISP_RegField(MISP_LOCAL_ID, 0x05F,  0, 14, h_size*8 * 6/10);  // comp_max_bits_c 
			MISP_RegField(MISP_LOCAL_ID, 0x213,  0, 12, h_size   * 6/10);  // r_hsize_compress [ipc]
			MISP_RegField(MISP_LOCAL_ID, 0x214,  0, 12, h_size   * 6/10);  // r_hsize_compress_c 
			
			MISP_MultiWrite(MISP_HOST_ID, 0x1F0, (PBYTE)defQuantBit60, sizeof(defQuantBit60));  // defquantbit [host]
			MISP_MultiWrite(MISP_HOST_ID, 0x1C0, (PBYTE)LumQuantBit60, sizeof(LumQuantBit60));  // lumquantbit
			break;
	}
	return 0;
#else
	return 1;  //failure : i500 only
#endif
}


//--------------------------------------------------------------------------------------------------------------------------
//get current state
BOOL GetWDR_ComprState(void) {
#if USE_IPC_COMPRES
	if(stWDRCTRL.comp_mode)  return TRUE;
	else                     return FALSE;
#else
	return FALSE;
#endif
}


//--------------------------------------------------------------------------------------------------------------------------
//toggle debugging print level
static void WDRTEST_WdrDebugMode(PCHAR pStr) {
	int i;

	stWDRCTRL.debug_lev=(stWDRCTRL.debug_lev>=7)?0:(stWDRCTRL.debug_lev+1);
	stDebug_Print(0, "\nDEBUG_WDR:%d\r\n", stWDRCTRL.debug_lev);
	if(stWDRCTRL.debug_lev) sprintf(pStr, "WDR.MSG:%2d", stWDRCTRL.debug_lev);

	//print current state
	stDebug_Print(1, "########################################## \r\n");
	for(i=0; i<9; i++)  stDebug_Print(1, "### %s\r\n", GetWdr_DebugStr(i));
	stDebug_Print(1, "########################################## \r\n");
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
//test code : tonemap to the bypass frame
static void WDRTEST_WdrBypassTM(PCHAR pStr) {
	static BYTE OnOff=OFF;
	if(stWDRCTRL.fview_sel==MISP_WDR_FVIEW_WDR) {
		//wdr normal state..
		//do nothing..
		strcpy(pStr, "N/A       ");
	}
	else {
		OnOff = (OnOff)?OFF:ON;
		if(OnOff) {  //TM en
			strcpy(pStr, "TM enable ");
			MISP_RegField(MISP_LOCAL_ID, 0x5B9,  9,2, 3);  // tm_en
			MISP_RegField(MISP_LOCAL_ID, 0x5B4, 12,3, 0);  // tm_out_bit_scope
			MISP_RegField(MISP_LOCAL_ID, 0x5E7,  0,1, 0);  // tm_ctrl (auto)
		}
		else {  //return TM
			//strcpy(pStr, "TM disable");
			MISP_RegField(MISP_LOCAL_ID, 0x5B9,  9,2, 0);  // tm_en
			MISP_RegField(MISP_LOCAL_ID, 0x5B4, 12,3, stWDRCTRL.inp_shift);  // tm_out_bit_scope
			MISP_RegField(MISP_LOCAL_ID, 0x5E7,  0,1, 1);  // tm_ctrl (manual)
		}
	}
}


//--------------------------------------------------------------------------------------------------------------------------
//test code : toggle new algo.
static void WDRTEST_WdrEnhance(PCHAR pStr) {
	//static BYTE nID=0;
	WORD rVal;

	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_FRAME:
		case MISP_WDR_PROC_LINE2:
		case MISP_WDR_PROC_LINE3:
#if REV_WDR_NEW_MGR
			MISP_RegRead(MISP_LOCAL_ID, 0x43C, &rVal);
			if(rVal&0x2000) {  //check 43C[13] : r_wdr_csp_en
				strcpy(pStr, "CSP OFF   ");
				MISP_RegField(MISP_LOCAL_ID, 0x43C,13, 1, 0);  //r_wdr_csp_en
			}
			else {
				strcpy(pStr, "CSP ON    ");
				MISP_RegField(MISP_LOCAL_ID, 0x43C,13, 1, 1);  //r_wdr_csp_en
			}
#elif REV_REG_MGR_MDS
			MISP_RegRead(MISP_LOCAL_ID, 0x5E0, &rVal);  //use reserved register 5E0h[15:12]
			rVal>>=12;
			rVal = (rVal>=3)?0:(rVal+1);
			MISP_RegField(MISP_LOCAL_ID, 0x5E0, 12, 4, rVal);
			switch(rVal) {
				case 0:
					strcpy(pStr, "Sht'er    ");
					stDebug_Print(0, "### MDS:Shorter, Filter:Disable\r\n");
					MISP_RegField(MISP_LOCAL_ID, 0x7F5, 2, 9, 4<<6|4<<3|4);  // mds : shorter
					MISP_RegField(MISP_LOCAL_ID, 0x7F5,11, 1, 0);  // mgr_mo_use_en
					SetNR_UseWdrMotionFlag(0);  //0:disable, 1:motion, 2:suppress
					break;
				case 1:
					strcpy(pStr, "Sht'er+FLT");
					stDebug_Print(0, "### MDS:Shorter, Filter:Enable\r\n");
					MISP_RegField(MISP_LOCAL_ID, 0x7F5, 2, 9, 4<<6|4<<3|4);  // mds : short
					MISP_RegField(MISP_LOCAL_ID, 0x7F5,11, 1, 1);  // mgr_mo_use_en
					SetNR_UseWdrMotionFlag(1);  //0:disable, 1:motion, 2:suppress
					break;
				case 2:
					strcpy(pStr, "Short     ");
					stDebug_Print(0, "### MDS:Short, Filter:Disable\r\n");
					MISP_RegField(MISP_LOCAL_ID, 0x7F5, 2, 9, 4<<6|2<<3|2);  // mds : short
					MISP_RegField(MISP_LOCAL_ID, 0x7F5,11, 1, 0);  // mgr_mo_use_en
					SetNR_UseWdrMotionFlag(0);  //0:disable, 1:motion, 2:suppress
					break;
				default:
					strcpy(pStr, "Short+FLT ");
					stDebug_Print(0, "### MDS:Short, Filter:Enable\r\n");
					MISP_RegField(MISP_LOCAL_ID, 0x7F5, 2, 9, 4<<6|2<<3|2);  // mds : short
					MISP_RegField(MISP_LOCAL_ID, 0x7F5,11, 1, 1);  // mgr_mo_use_en
					SetNR_UseWdrMotionFlag(1);  //0:disable, 1:motion, 2:suppress
					break;
			}
#endif
			break;
		case MISP_WDR_PROC_BUILT:
#if REV_x36_CLR_SPR
			MISP_RegRead(MISP_LOCAL_ID, 0x7F5, &rVal);
			if(rVal&0x0800) {  //check 7F5[11] : mgr_mo_use_en
				strcpy(pStr, "Supr. OFF ");
				stDebug_Print(0, "### color suppression off\r\n");
				MISP_RegField(MISP_LOCAL_ID, 0x7F5,11, 1, 0);  // mgr_mo_use_en
				SetNR_UseWdrMotionFlag(0);  //0:disable, 1:motion, 2:suppress
			}
			else {
				strcpy(pStr, "Supr. ON  ");
				stDebug_Print(0, "### color suppression on\r\n");
				MISP_RegField(MISP_LOCAL_ID, 0x7F5,11, 1, 1);  // mgr_mo_use_en
				SetNR_UseWdrMotionFlag(2);  //0:disable, 1:motion, 2:suppress
			}
#endif
			break;
		default:
			rVal=0;  //just touch to remove compile warning
			strcpy(pStr, "N/A       ");
			break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
//test code : mgr_debugmode
static void WDRTEST_WdrDebugDisp(PCHAR pStr) {
	static BYTE mode=0;
	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_FRAME:
		case MISP_WDR_PROC_LINE2:
		case MISP_WDR_PROC_LINE3:
#if REV_WDR_NEW_MGR
			mode=(mode)?0:1;
			if(mode) {
				stWDRCTRL.ctrl &=~WDRCTRL_TM_CTRL_ON;  //hold TM
				MISP_RegField(MISP_LOCAL_ID, 0x5A3, 8,2, 3);  // r_debug_mode
				MISP_RegField(MISP_LOCAL_ID, 0x5A3,10,5, 2);  // r_debug_flag
				strcpy(pStr, "DISP.CSP  ");
			}
			else {
				stWDRCTRL.ctrl |= WDRCTRL_TM_CTRL_ON;  //release TM
				MISP_RegField(MISP_LOCAL_ID, 0x5A3, 8,2, 0);  // r_debug_mode
				MISP_RegField(MISP_LOCAL_ID, 0x5A3,10,5, 0);  // r_debug_flag
			}
#elif REV_REG_MGR_MDS
			mode=(mode>=2)?0:(mode+1);
		#if 1  //use NR display
			if(mode==1) {  //debug display on (2D filter)
				MISP_RegField(MISP_LOCAL_ID, 0x7F5,12,4, 4);  // mgr_mo_flag_sel (0100 : still,low,mid,high) for 2D-NR
				MISP_RegField(MISP_LOCAL_ID, 0x538,14,1, 1);  // r_NR_2D_DEBUG_EN
				MISP_RegField(MISP_LOCAL_ID, 0x538,12,2, 0);  // r_NR_2D_DEBUG_OVERLAY_SEL
				MISP_RegField(MISP_LOCAL_ID, 0x538, 8,4,11);  // r_NR_2D_DEBUG_SEL
				strcpy(pStr, "MO : FLT  ");
			}
			else if(mode==2) {  //debug display on (MGR all)
				MISP_RegField(MISP_LOCAL_ID, 0x7F5,12,4, 7);  // mgr_mo_flag_sel (0100 : still,low,mid,high) for 2D-NR
				MISP_RegField(MISP_LOCAL_ID, 0x538,14,1, 1);  // r_NR_2D_DEBUG_EN
				MISP_RegField(MISP_LOCAL_ID, 0x538,12,2, 0);  // r_NR_2D_DEBUG_OVERLAY_SEL
				MISP_RegField(MISP_LOCAL_ID, 0x538, 8,4,11);  // r_NR_2D_DEBUG_SEL
				strcpy(pStr, "MO : ALL  ");
			}
			else {  //debug display off
				MISP_RegField(MISP_LOCAL_ID, 0x7F5,12,4, 4);  // mgr_mo_flag_sel (0100 : still,low,mid,high) for 2D-NR
				MISP_RegField(MISP_LOCAL_ID, 0x538,14,1, 0);  // r_NR_2D_DEBUG_EN
				MISP_RegField(MISP_LOCAL_ID, 0x538,12,2, 0);  // r_NR_2D_DEBUG_OVERLAY_SEL
				MISP_RegField(MISP_LOCAL_ID, 0x538, 8,4, 0);  // r_NR_2D_DEBUG_SEL
			}
		#else  //use MGR display
			if(mode) {  //debug display on (mgr motion)
				stWDRCTRL.ctrl &=~WDRCTRL_TM_CTRL_ON;  //hold TM
				MISP_RegField(MISP_LOCAL_ID, 0x5AB, 4,1, 0);  // wb_en
				MISP_RegField(MISP_LOCAL_ID, 0x5B9, 9,2, 0);  // tm_en
				MISP_RegField(MISP_LOCAL_ID, 0x7F5, 0,2, 1);  // mgr_debugmode
				MISP_RegField(MISP_LOCAL_ID, 0x480,14,2, 0);  // dm_pix_id
			}
			else {  //debug display off
				stWDRCTRL.ctrl |= WDRCTRL_TM_CTRL_ON;  //revert TM
				MISP_RegField(MISP_LOCAL_ID, 0x5AB, 4,1, 1);  // wb_en
				MISP_RegField(MISP_LOCAL_ID, 0x5B9, 9,2, 3);  // tm_en
				MISP_RegField(MISP_LOCAL_ID, 0x7F5, 0,2, 0);  // mgr_debugmode
				MISP_RegField(MISP_LOCAL_ID, 0x480,14,2, 3);  // dm_pix_id
			}
		#endif
#else
			strcpy(pStr, "N/A       ");
#endif
			break;
		case MISP_WDR_PROC_BUILT:
			mode=(mode>=4)?0:(mode+1);
#if REV_x36_CLR_SPR
			if(mode) {  //debug display on (color suppress)
				MISP_RegField(MISP_LOCAL_ID, 0x538,14,1, 1);  // r_NR_2D_DEBUG_EN
				MISP_RegField(MISP_LOCAL_ID, 0x538,12,2, 0);  // r_NR_2D_DEBUG_OVERLAY_SEL
				MISP_RegField(MISP_LOCAL_ID, 0x538, 8,4,11);  // r_NR_2D_DEBUG_SEL
			}
			else {  //debug display off
				MISP_RegField(MISP_LOCAL_ID, 0x538,14,1, 0);  // r_NR_2D_DEBUG_EN
				MISP_RegField(MISP_LOCAL_ID, 0x538,12,2, 0);  // r_NR_2D_DEBUG_OVERLAY_SEL
				MISP_RegField(MISP_LOCAL_ID, 0x538, 8,4, 0);  // r_NR_2D_DEBUG_SEL
			}

			switch(mode) {  //r_color_motion_sel
				case 1:  MISP_RegField(MISP_LOCAL_ID, 0x522, 14,2, 0);  strcpy(pStr, "SUPR:ALL  ");  break;
				case 2:  MISP_RegField(MISP_LOCAL_ID, 0x522, 14,2, 3);  strcpy(pStr, "SUPR:FLAG ");  break;
				case 3:  MISP_RegField(MISP_LOCAL_ID, 0x522, 14,2, 2);  strcpy(pStr, "SUPR:CU.MO");  break;
				case 4:  MISP_RegField(MISP_LOCAL_ID, 0x522, 14,2, 1);  strcpy(pStr, "SUPR:PR.MO");  break;
				default: MISP_RegField(MISP_LOCAL_ID, 0x522, 14,2, 0);  break;
			}
#else
			strcpy(pStr, "N/A       ");
#endif
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrHoldTMProc(PCHAR pStr) {
	if(!(stWDRCTRL.ctrl&WDRCTRL_PROCESS_ON))
		return;

#if WDR_USE_MANU_TM
	if(stWDRCTRL.ctrl&WDRCTRL_TM_CTRL_ON) {
		strcpy(pStr, "Hold TM   ");
		stWDRCTRL.ctrl &=~WDRCTRL_TM_CTRL_ON;
	}
	else {
		//strcpy(pStr, "Auto TM   ");
		stWDRCTRL.ctrl |= WDRCTRL_TM_CTRL_ON;
	}
#else
	strcpy(pStr, "N/A       ");
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrHoldAEProc(PCHAR pStr) {
	if(!(stWDRCTRL.ctrl&WDRCTRL_PROCESS_ON))
		return;

	if(stWDRCTRL.ctrl&WDRCTRL_AE_PROC_ON) {
		strcpy(pStr, "Hold AE   ");
		stWDRCTRL.ctrl &=~WDRCTRL_AE_PROC_ON;
	}
	else {
		//strcpy(pStr, "Auto AE   ");
		stWDRCTRL.ctrl |= WDRCTRL_AE_PROC_ON;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrHoldSensorWr(PCHAR pStr) {
	WORD rVal;
	MISP_RegRead(MISP_LOCAL_ID, 0x580, &rVal);
	if(rVal&0x00C0) {  //check 580[7:6] : ss_mw_wdr_proc_mode
		MISP_RegField(MISP_LOCAL_ID, 0x580, 6,2, 0);  //ss_mw_wdr_proc_mode (off)
		MISP_RegField(MISP_LOCAL_ID, 0x5C2,14,1, 1);  //r_wdr_sep_fid_mode (single frame)
		stDebug_Print(0, "IrCode_WdrHoldSensorWr : seize\r\n");
		strcpy(pStr, ">> FREEZE ");
	}
	else {
		WDRCTRL_SetSensorWrite(&stWDRCTRL);  //ss_mw_wdr_proc_mode (revert)
		MISP_RegField(MISP_LOCAL_ID, 0x5C2,14,1, 0);  //r_wdr_sep_fid_mode (tripple frame)
		stDebug_Print(0, "IrCode_WdrHoldSensorWr : release\r\n");
		//strcpy(pStr, ">> RELEASE");
	}
	return;
}

//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrInputBitShift(PCHAR pStr) {
	switch(stWDRCTRL.inp_shift) {
		case 0:  stWDRCTRL.inp_shift=1;  strcpy(pStr, ">> 1 BIT  ");  break;
		case 1:  stWDRCTRL.inp_shift=2;  strcpy(pStr, ">> 2 BIT  ");  break;
		case 2:  stWDRCTRL.inp_shift=3;  strcpy(pStr, ">> 3 BIT  ");  break;
		case 3:  stWDRCTRL.inp_shift=0;  strcpy(pStr, ">> 0 BIT  ");  break;
	}
	stDebug_Print(0, "shift %d bit\r\n", stWDRCTRL.inp_shift);

	//read input data
	MISP_RegField(MISP_LOCAL_ID, 0x5B4, 0, 3, stWDRCTRL.inp_shift);  // wdr_bit_shift_l
	MISP_RegField(MISP_LOCAL_ID, 0x5B4, 3, 3, stWDRCTRL.inp_shift);  // wdr_bit_shift_s1
	MISP_RegField(MISP_LOCAL_ID, 0x5B4, 6, 3, stWDRCTRL.inp_shift);  // wdr_bit_shift_s2
	MISP_RegField(MISP_LOCAL_ID, 0x5B4, 9, 3, stWDRCTRL.inp_shift);  // wdr_bit_shift_s3

	//set mgr_offst, mgr_pxmax
	WDRCTRL_SetSensorBLC(&stWDRCTRL);
	return;
}

//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrShutterMode(PCHAR pStr) {
	if(stWDRCTRL.ctrl&WDRCTRL_AE_IRIS_EN) {  //iris->shutter
		strcpy(pStr, "SHUT mode ");
		stWDRCTRL.ctrl &=~WDRCTRL_AE_IRIS_EN;
	}
	else {  //shutter->iris
		strcpy(pStr, "IRIS mode ");
		stWDRCTRL.ctrl |= WDRCTRL_AE_IRIS_EN;
	}

	//initial shutter(iris)/gain
	stWDRCTRL.gain_ctrl=GetWDR_GainMin();  //min
	stWDRCTRL.shut_ctrl=GetWDR_ShutMax();
	stWDRCTRL.iris_ctrl=GetWDR_IrisMax();
	WDRCTRL_SetSensGain(&stWDRCTRL);
	WDRCTRL_SetSensShut(&stWDRCTRL);
	WDRCTRL_SetLensIris(&stWDRCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
/*static void Test_SetWdrNoiseLUT(BYTE nID) {
	#define LUT_SIZE 128
	const WORD nlut_wdr[][LUT_SIZE]={
		{  //mn34220
			0x0F0C, 0x1513, 0x1A17, 0x1D1C, 0x211F, 0x2422, 0x2725, 0x2928,
			0x2C2B, 0x2E2D, 0x302F, 0x3231, 0x3433, 0x3635, 0x3837, 0x3A39,
			0x3C3B, 0x3E3D, 0x3F3E, 0x4140, 0x4342, 0x4443, 0x4645, 0x4746,
			0x4948, 0x4A49, 0x4B4B, 0x4D4C, 0x4E4D, 0x4F4F, 0x5150, 0x5251,
			0x5353, 0x5554, 0x5655, 0x5756, 0x5858, 0x5959, 0x5B5A, 0x5C5B,
			0x5D5C, 0x5E5D, 0x5F5E, 0x6060, 0x6161, 0x6262, 0x6363, 0x6464,
			0x6565, 0x6666, 0x6767, 0x6868, 0x6969, 0x6A6A, 0x6B6B, 0x6C6C,
			0x6D6D, 0x6E6E, 0x6F6F, 0x7070, 0x7171, 0x7272, 0x7373, 0x7473,
			0x7574, 0x7675, 0x7776, 0x7777, 0x7878, 0x7979, 0x7A7A, 0x7B7A,
			0x7C7B, 0x7D7C, 0x7D7D, 0x7E7E, 0x7F7F, 0x8080, 0x8180, 0x8281,
			0x8282, 0x8383, 0x8484, 0x8584, 0x8685, 0x8686, 0x8787, 0x8887,
			0x8988, 0x8989, 0x8A8A, 0x8B8B, 0x8C8B, 0x8C8C, 0x8D8D, 0x8E8E,
			0x8F8E, 0x8F8F, 0x9090, 0x9190, 0x9291, 0x9292, 0x9393, 0x9493,
			0x9494, 0x9595, 0x9695, 0x9696, 0x9797, 0x9898, 0x9998, 0x9999,
			0x9A9A, 0x9B9A, 0x9B9B, 0x9C9C, 0x9D9C, 0x9D9D, 0x9E9E, 0x9F9E,
			0x9F9F, 0xA0A0, 0xA1A0, 0xA1A1, 0xA2A2, 0xA3A2, 0xA3A3, 0xA4A4},
		{  //dummy test (test)
			0x2828, 0x2928, 0x2929, 0x2A2A, 0x2B2A, 0x2C2B, 0x2D2C, 0x2E2D,
			0x2F2E, 0x302F, 0x3130, 0x3231, 0x3433, 0x3635, 0x3837, 0x3A39,
			0x3C3B, 0x3E3D, 0x3F3E, 0x4140, 0x4342, 0x4443, 0x4645, 0x4746,
			0x4948, 0x4A49, 0x4B4B, 0x4D4C, 0x4E4D, 0x4F4F, 0x5150, 0x5251,
			0x5353, 0x5554, 0x5655, 0x5756, 0x5858, 0x5959, 0x5B5A, 0x5C5B,
			0x5D5C, 0x5E5D, 0x5F5E, 0x6060, 0x6161, 0x6262, 0x6363, 0x6464,
			0x6565, 0x6666, 0x6767, 0x6868, 0x6969, 0x6A6A, 0x6B6B, 0x6C6C,
			0x6D6D, 0x6E6E, 0x6F6F, 0x7070, 0x7171, 0x7272, 0x7373, 0x7473,
			0x7574, 0x7675, 0x7776, 0x7777, 0x7878, 0x7979, 0x7A7A, 0x7B7A,
			0x7C7B, 0x7D7C, 0x7D7D, 0x7E7E, 0x7F7F, 0x8080, 0x8180, 0x8281,
			0x8282, 0x8383, 0x8484, 0x8584, 0x8685, 0x8686, 0x8787, 0x8887,
			0x8988, 0x8989, 0x8A8A, 0x8B8B, 0x8C8B, 0x8C8C, 0x8D8D, 0x8E8E,
			0x8F8E, 0x8F8F, 0x9090, 0x9190, 0x9291, 0x9292, 0x9393, 0x9493,
			0x9494, 0x9595, 0x9695, 0x9696, 0x9797, 0x9898, 0x9998, 0x9999,
			0x9A9A, 0x9B9A, 0x9B9B, 0x9C9C, 0x9D9C, 0x9D9D, 0x9E9E, 0x9F9E,
			0x9F9F, 0xA0A0, 0xA1A0, 0xA1A1, 0xA2A2, 0xA3A2, 0xA3A3, 0xA4A4},
		{  //mit test (OLPF_hallo - 0.1193, 4.434)
			0x0b09, 0x0d0c, 0x0f0e, 0x1110, 0x1312, 0x1514, 0x1615, 0x1717,
			0x1918, 0x1a19, 0x1b1a, 0x1c1c, 0x1d1d, 0x1e1e, 0x1f1f, 0x2020,
			0x2121, 0x2222, 0x2322, 0x2423, 0x2524, 0x2525, 0x2626, 0x2727,
			0x2827, 0x2928, 0x2929, 0x2a2a, 0x2b2a, 0x2b2b, 0x2c2c, 0x2d2d,
			0x2e2d, 0x2e2e, 0x2f2f, 0x2f2f, 0x3030, 0x3130, 0x3131, 0x3232,
			0x3332, 0x3333, 0x3434, 0x3434, 0x3535, 0x3635, 0x3636, 0x3736,
			0x3737, 0x3837, 0x3838, 0x3939, 0x3939, 0x3a3a, 0x3a3a, 0x3b3b,
			0x3b3b, 0x3c3c, 0x3d3c, 0x3d3d, 0x3e3d, 0x3e3e, 0x3e3e, 0x3f3f,
			0x3f3f, 0x4040, 0x4040, 0x4141, 0x4141, 0x4242, 0x4242, 0x4343,
			0x4343, 0x4443, 0x4444, 0x4544, 0x4545, 0x4545, 0x4646, 0x4646,
			0x4747, 0x4747, 0x4847, 0x4848, 0x4848, 0x4949, 0x4949, 0x4a49,
			0x4a4a, 0x4b4a, 0x4b4b, 0x4b4b, 0x4c4c, 0x4c4c, 0x4d4c, 0x4d4d,
			0x4d4d, 0x4e4e, 0x4e4e, 0x4f4e, 0x4f4f, 0x4f4f, 0x504f, 0x5050,
			0x5050, 0x5151, 0x5151, 0x5251, 0x5252, 0x5252, 0x5352, 0x5353,
			0x5353, 0x5454, 0x5454, 0x5554, 0x5555, 0x5555, 0x5655, 0x5656,
			0x5656, 0x5756, 0x5757, 0x5757, 0x5858, 0x5858, 0x5858, 0x5959},
		{  //mit test (OLPF_hallo_x4 - 0.4771, 70.944)
			0x2423, 0x2826, 0x2b29, 0x2d2c, 0x302f, 0x3231, 0x3534, 0x3736,
			0x3938, 0x3b3a, 0x3d3c, 0x3f3e, 0x4140, 0x4342, 0x4544, 0x4746,
			0x4847, 0x4a49, 0x4c4b, 0x4d4c, 0x4f4e, 0x5050, 0x5251, 0x5353,
			0x5554, 0x5655, 0x5857, 0x5958, 0x5a5a, 0x5c5b, 0x5d5c, 0x5e5e,
			0x605f, 0x6160, 0x6261, 0x6363, 0x6564, 0x6665, 0x6766, 0x6868,
			0x6969, 0x6a6a, 0x6c6b, 0x6d6c, 0x6e6d, 0x6f6e, 0x7070, 0x7171,
			0x7272, 0x7373, 0x7474, 0x7575, 0x7676, 0x7777, 0x7878, 0x7979,
			0x7a7a, 0x7b7b, 0x7c7c, 0x7d7d, 0x7e7e, 0x7f7f, 0x8080, 0x8181,
			0x8282, 0x8383, 0x8484, 0x8585, 0x8685, 0x8786, 0x8887, 0x8988,
			0x8a89, 0x8a8a, 0x8b8b, 0x8c8c, 0x8d8d, 0x8e8d, 0x8f8e, 0x908f,
			0x9090, 0x9191, 0x9292, 0x9393, 0x9493, 0x9594, 0x9595, 0x9696,
			0x9797, 0x9897, 0x9998, 0x9999, 0x9a9a, 0x9b9b, 0x9c9b, 0x9d9c,
			0x9d9d, 0x9e9e, 0x9f9f, 0xa09f, 0xa0a0, 0xa1a1, 0xa2a2, 0xa3a2,
			0xa3a3, 0xa4a4, 0xa5a5, 0xa6a5, 0xa6a6, 0xa7a7, 0xa8a8, 0xa9a8,
			0xa9a9, 0xaaaa, 0xabaa, 0xacab, 0xacac, 0xadad, 0xaead, 0xaeae,
			0xafaf, 0xb0af, 0xb0b0, 0xb1b1, 0xb2b1, 0xb2b2, 0xb3b3, 0xb4b4},
	};

	int i;
 	WORD wData;

	//preset
	MISP_RegWrite(MISP_LOCAL_ID, 0x5E0, 0x0000);  //wdr_pana_lut_init, r_wdr_pana_lut_use_en
	MISP_RegWrite(MISP_LOCAL_ID, 0x5E0, 0x0005);
	MISP_RegRead(MISP_LOCAL_ID, 0x5E0, &wData);   //read just to issue a little delay

	//write lut
	if(wData==0x0005) {
		for(i=0; i<LUT_SIZE; i++)
			MISP_RegWrite(MISP_LOCAL_ID, 0x5E1, nlut_wdr[nID][i]);
	}

	return;
}
*/

//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrMergeNum(PCHAR pStr) {
	WORD rVal;
	BYTE mNum;

	MISP_RegRead(MISP_LOCAL_ID, 0x59C, &rVal);
	rVal=rVal&0x07;
	mNum = (rVal==2)?1:2;
	switch(mNum) {
		case 1:   // 2->1 frame
			MISP_RegField(MISP_LOCAL_ID, 0x59C, 0, 2, 1);     // wdr_mergenum(1:2frame)
			MISP_RegField(MISP_LOCAL_ID, 0x59D, 8, 4, 0x03);  // wdr_proc_frame_en(0011)
			MISP_RegField(MISP_LOCAL_ID, 0x59D, 0, 8, 0xC2);  // mgr_imagesel(3->x->x->2)
			strcpy(pStr, "2->1 MERGE");
			break;
		default:  // 3->1 frame
			MISP_RegField(MISP_LOCAL_ID, 0x59C, 0, 2, 2);     // wdr_mergenum(2:3frame)
			MISP_RegField(MISP_LOCAL_ID, 0x59D, 8, 4, 0x07);  // wdr_proc_frame_en(0111)
			MISP_RegField(MISP_LOCAL_ID, 0x59D, 0, 8, 0xC9);  // mgr_imagesel(3->x->2->1)
			strcpy(pStr, "3->1 MERGE");
			break;
	}
	return;
}

//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrToggleBitMode(PCHAR pStr) {
	if(stWDRCTRL.ctrl&WDRCTRL_IN_RES_10B) {
		stDebug_Print(0, "### use 9bit input\r\n");
		stWDRCTRL.ctrl &=~WDRCTRL_IN_RES_MASK;
		stWDRCTRL.ctrl |= WDRCTRL_IN_RES_09B;
		stWDRCTRL.inp_shift=3;
		strcpy(pStr, "INP_9BIT  ");
	}
	else {
		stDebug_Print(0, "### use 10bit input\r\n");
		stWDRCTRL.ctrl &=~WDRCTRL_IN_RES_MASK;
		stWDRCTRL.ctrl |= WDRCTRL_IN_RES_10B;
		stWDRCTRL.inp_shift=2;
		strcpy(pStr, "INP_10BIT ");
	}
	SetWDR_ProcessRst();
}

//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrCspMotionHist(PCHAR pStr) {
	//Color suppresstion for Sony Built-in Artifact
	WORD rVal;
	MISP_RegRead(MISP_LOCAL_ID, 0x528, &rVal);

	if(rVal&0x0300) {  //r_mo_his_minus
		stDebug_Print(0, "### r_mo_his_minus:0\r\n");
		MISP_RegField(MISP_LOCAL_ID, 0x528, 8, 2, 0);
		strcpy(pStr, "W/O MO.HIS");
	}
	else {
		stDebug_Print(0, "### r_mo_his_minus:1\r\n");
		MISP_RegField(MISP_LOCAL_ID, 0x528, 8, 2, 1);
		strcpy(pStr, "USE MO.HIS");
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrCspMotionThr(PCHAR pStr) {
	//Color suppresstion for Sony Built-in Artifact
	WORD rVal;
	MISP_RegRead(MISP_LOCAL_ID, 0x543, &rVal);  //read 0x543[11:8]
	rVal=(rVal>>8)&0x000F;
	rVal=(rVal==15)?0:(rVal+1);

	MISP_RegField(MISP_LOCAL_ID, 0x543,8,4, rVal);
	stDebug_Print(0, "### r_diff_offset:%d\r\n", rVal);
	sprintf(pStr, "THRES:%2d  ", rVal);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrCspHistMinus(PCHAR pStr) {
	//Color suppresstion for Sony Built-in Artifact
	WORD rVal;
	MISP_RegRead(MISP_LOCAL_ID, 0x528, &rVal);  //read 0x528[9:8]
	rVal=(rVal>>8)&0x0003;
	rVal=(rVal==3)?0:(rVal+1);

	MISP_RegField(MISP_LOCAL_ID, 0x528,8,2, rVal);
	stDebug_Print(0, "### r_mo_his_minus:%d\r\n", rVal);
	sprintf(pStr, "MINUS:%d   ", rVal);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrMgrPrevWB(PCHAR pStr) {
	WORD inp_off=GetSS_man_black_level_target();
	WORD mgr_off=inp_off>>stWDRCTRL.inp_shift;
	WORD inp_max=0x0FFF>>stWDRCTRL.inp_shift;

	//test MGR prev.WB
	WORD rVal;
	MISP_RegRead(MISP_LOCAL_ID, 0x5AB, &rVal);  //read 0x5AB[4]
	if(rVal&0x0010) {  //->prev.WB
		sprintf(pStr, "WB.PREV    ");
		stWDRCTRL.mgr_offst=0;
		stWDRCTRL.mgr_pxmax=inp_max-mgr_off;

		MISP_RegField(MISP_LOCAL_ID, 0x5AB, 4, 1, 0);        //wb_en
		MISP_RegField(MISP_LOCAL_ID, 0x5A1, 0,12, stWDRCTRL.mgr_offst);  //mgr_offset
		MISP_RegField(MISP_LOCAL_ID, 0x5A2, 0,12, stWDRCTRL.mgr_pxmax);  //mgr_pixelmax

		MISP_RegField(MISP_LOCAL_ID, 0x43B, 7, 1, 0);        //r_wdr_csp_sat_en
		MISP_RegField(MISP_LOCAL_ID, 0x43B, 5, 1, 0);        //r_wdr_mo_flag_en

		//MISP_RegField(MISP_LOCAL_ID, 0x60C, 7, 1, 1);        //sns_rb_slope_en
		//MISP_RegField(MISP_LOCAL_ID, 0x60C, 6, 1, 1);        //sns_rb_comp_en
		//MISP_RegField(MISP_LOCAL_ID, 0x60C, 5, 1, 1);        //sns_dc_gain_en
		//MISP_RegField(MISP_LOCAL_ID, 0x60C, 4, 1, 1);        //sns_wb_gain_en
		WBCTRL_SetAWBCtrlPath(0,1,1,1,1);
		MISP_RegField(MISP_LOCAL_ID, 0x4A9, 0,12, inp_off);  //blc_target
	}
	else {  //->post.WB
		sprintf(pStr, "WB.POST    ");
		stWDRCTRL.mgr_offst=mgr_off;
		stWDRCTRL.mgr_pxmax=inp_max;

		MISP_RegField(MISP_LOCAL_ID, 0x5AB, 4, 1, 1);        //wb_en
		MISP_RegField(MISP_LOCAL_ID, 0x5A1, 0,12, stWDRCTRL.mgr_offst);  //mgr_offset
		MISP_RegField(MISP_LOCAL_ID, 0x5A2, 0,12, stWDRCTRL.mgr_pxmax);  //mgr_pixelmax

		MISP_RegField(MISP_LOCAL_ID, 0x43B, 7, 1, 1);        //r_wdr_csp_sat_en
		MISP_RegField(MISP_LOCAL_ID, 0x43B, 5, 1, 1);        //r_wdr_mo_flag_en

		//MISP_RegField(MISP_LOCAL_ID, 0x60C, 7, 1, 0);        //sns_rb_slope_en
		//MISP_RegField(MISP_LOCAL_ID, 0x60C, 6, 1, 0);        //sns_rb_comp_en
		//MISP_RegField(MISP_LOCAL_ID, 0x60C, 5, 1, 1);        //sns_dc_gain_en
		//MISP_RegField(MISP_LOCAL_ID, 0x60C, 4, 1, 0);        //sns_wb_gain_en
		WBCTRL_SetAWBCtrlPath(0,0,0,1,0);
		MISP_RegField(MISP_LOCAL_ID, 0x4A9, 0,12, 0);        //blc_target
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrToggleCC(PCHAR pStr) {
	if(!(stWDRCTRL.ctrl&WDRCTRL_PROCESS_ON))
		return;

	if(GetDM_ColorCCtrl_Red()>4) {  //on->off
		SetDM_ColorCCtrl_Red(4);				// 4 is bypass
		sprintf(pStr, "CC RED OFF ");
	}
	else {  //off->on
		if     (stWDRCTRL.ctrl&WDRCTRL_MODE_BUILT)  SetDM_ColorCCtrl_Red(12);  //Tonemap
		else if(stWDRCTRL.ctrl&WDRCTRL_MODE_SINGL)  SetDM_ColorCCtrl_Red(13);  //DWDR
		else                                        SetDM_ColorCCtrl_Red(11);  //WDR
		sprintf(pStr, "CC RED ON  ");
	}

	if(GetDM_ColorCCtrl_Green()>4) {  //on->off
		SetDM_ColorCCtrl_Green(4);				// 4 is bypass
		sprintf(pStr, "CC GR  OFF ");
	}
	else {  //off->on
		if     (stWDRCTRL.ctrl&WDRCTRL_MODE_BUILT)  SetDM_ColorCCtrl_Green(12);  //Tonemap
		else if(stWDRCTRL.ctrl&WDRCTRL_MODE_SINGL)  SetDM_ColorCCtrl_Green(13);  //DWDR
		else                                        SetDM_ColorCCtrl_Green(11);  //WDR
		sprintf(pStr, "CC GR  ON  ");
	}

	if(GetDM_ColorCCtrl_Blue()>4) {  //on->off
		SetDM_ColorCCtrl_Blue(4);				// 4 is bypass
		sprintf(pStr, "CC BLUEOFF");
	}
	else {  //off->on
		if     (stWDRCTRL.ctrl&WDRCTRL_MODE_BUILT)  SetDM_ColorCCtrl_Blue(12);  //Tonemap
		else if(stWDRCTRL.ctrl&WDRCTRL_MODE_SINGL)  SetDM_ColorCCtrl_Blue(13);  //DWDR
		else                                        SetDM_ColorCCtrl_Blue(11);  //WDR
		sprintf(pStr, "CC BLUEON");
	}	
	
}

//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrSetTmLsize(PCHAR pStr) {
	//gain ROI about local tone-map
	WORD rVal;

	if(!(stWDRCTRL.ctrl&WDRCTRL_PROCESS_ON))
		return;

	MISP_RegRead(MISP_LOCAL_ID, 0x5AE, &rVal);  //read 0x5AE[15:12]
	rVal=(rVal>>12)&0x000F;
	rVal=(rVal==1)?5:(rVal==5)?9:1;  //toggle 1,5,9
	MISP_RegField(MISP_LOCAL_ID, 0x5AE,12, 4, rVal);  //tm_lsize_0
	MISP_RegField(MISP_LOCAL_ID, 0x5AE, 8, 4, rVal);  //tm_lsize_1
	sprintf(pStr, "LSIZE %dx%d ", rVal, rVal);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrSetTmLgain(PCHAR pStr) {
	//gain table about local tone-map
	WORD rVal;

	if(!(stWDRCTRL.ctrl&WDRCTRL_PROCESS_ON))
		return;

	MISP_RegRead(MISP_LOCAL_ID, 0x5B0, &rVal);  //read 0x5B0[15:12]
	rVal=(rVal>>12)&0x000F;
	rVal=(rVal>=7)?0:(rVal+1);  //range 0~7
	MISP_RegField(MISP_LOCAL_ID, 0x5B0,12, 4, rVal);  //tm_lgain_0
	MISP_RegField(MISP_LOCAL_ID, 0x5B0, 8, 4, rVal);  //tm_lgain_1
	sprintf(pStr, "LGAIN : %d ", rVal);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrSetMgrBlend(PCHAR pStr) {
	//blend short data with long data to weaken blob artifacts for the low light condition
	WORD rVal;

	if(!(stWDRCTRL.ctrl&WDRCTRL_PROCESS_ON))
		return;

	MISP_RegRead(MISP_LOCAL_ID, 0x439, &rVal);  //read 0x439[13:12] - r_blend_cmp_data
	rVal=(rVal>>12)&0x0003;
	//rVal=(rVal>=3)?0:(rVal+1);  //range 0~3
	rVal=(rVal==3)?0:3;  //toggle
	MISP_RegField(MISP_LOCAL_ID, 0x439,12, 2, rVal);
	switch(rVal) {
		case 0:   sprintf(pStr, "NO BLEND   ");  break;
		case 1:   sprintf(pStr, "BLEND 1:1  ");  break;
		case 2:   sprintf(pStr, "BLEND 3:1  ");  break;
		default:  sprintf(pStr, "BLEND 7:1  ");  break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrSetMgrExpSel(PCHAR pStr) {
	//refer to neighbor 3x3 data to decide merge data
	WORD rVal;

	if(!(stWDRCTRL.ctrl&WDRCTRL_PROCESS_ON))
		return;

	MISP_RegRead(MISP_LOCAL_ID, 0x439, &rVal);  //read 0x439[15] - r_comp_sel_exp
	rVal=(rVal>>15)&0x0001;
	rVal=rVal?0:1;  //toggle
	MISP_RegField(MISP_LOCAL_ID, 0x439,15, 1, rVal);
	switch(rVal) {
		case 0:   sprintf(pStr, "MGR SNG.SEL");  break;
		default:  sprintf(pStr, "MGR EXP.SEL");  break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrDispMgrSel(PCHAR pStr) {
	//decision rule about source merging
	WORD rVal;

	if(!(stWDRCTRL.ctrl&WDRCTRL_PROCESS_ON))
		return;

	//get current state (r_debug_flag)
	MISP_RegRead(MISP_LOCAL_ID, 0x5A3, &rVal);  //read 0x5A3[14:10]
	rVal=(rVal>>10)&0x001F;

	//set next state
	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_FRAME:
		case MISP_WDR_PROC_LINE2:  // off->14->16
			if     (rVal==14)  { rVal=16; sprintf(pStr, "USE SHORT  "); }
			else if(rVal==16)  { rVal= 0; }
			else               { rVal=14; sprintf(pStr, "USE LONG   "); }
			break;
		case MISP_WDR_PROC_LINE3:  // off->14->15->16
			if     (rVal==14)  { rVal=15; sprintf(pStr, "USE SH_1   "); }
			else if(rVal==15)  { rVal=16; sprintf(pStr, "USE SH_2   "); }
			else if(rVal==16)  { rVal= 0; }
			else               { rVal=14; sprintf(pStr, "USE LONG   "); }
			break;
		default:
			return;
	}

	if(rVal) {  //disp enable
		MISP_RegField(MISP_LOCAL_ID, 0x5A3,10, 5,rVal);
		MISP_RegField(MISP_LOCAL_ID, 0x5A3, 8, 2, 1);
		stWDRCTRL.ctrl &=~WDRCTRL_TM_CTRL_ON;  //hold TM
	}
	else {
		MISP_RegField(MISP_LOCAL_ID, 0x5A3,10, 5, 0);
		MISP_RegField(MISP_LOCAL_ID, 0x5A3, 8, 2, 0);
		stWDRCTRL.ctrl |= WDRCTRL_TM_CTRL_ON;  //release TM
	}
}


//--------------------------------------------------------------------------------------------------------------------------
static void WDRTEST_WdrTmOffset(PCHAR pStr) {
	if(!(stWDRCTRL.ctrl&WDRCTRL_PROCESS_ON))
		return;

	if(stWDRCTRL.ctrl&WDRCTRL_TM_OFFS_ON) {
		strcpy(pStr, "FIXED BIAS");
		stWDRCTRL.ctrl &=~WDRCTRL_TM_OFFS_ON;
	}
	else {
		strcpy(pStr, "TM ADAPT. ");
		stWDRCTRL.ctrl |= WDRCTRL_TM_OFFS_ON;
	}
}


//--------------------------------------------------------------------------------------------------------------------------
//function for debug (returns string for OSD)
static CHAR pStr[12];
PBYTE SetWDR_TestFunc(BYTE nID) {
	pStr[0]=0;
	switch(nID) {
		case MISP_WDR_TEST_SHUT_MODE:    WDRTEST_WdrShutterMode(pStr);    break;  //WDR exposure Shut/Iris
		case MISP_WDR_TEST_DEBUG_MODE:   WDRTEST_WdrDebugMode(pStr);      break;  //WDR debug level
		case MISP_WDR_TEST_HOLD_SS_WR:   WDRTEST_WdrHoldSensorWr(pStr);   break;  //WDR hold sensor write
		case MISP_WDR_TEST_FORCE_TM:     WDRTEST_WdrBypassTM(pStr);       break;  //WDR bypass TM
		case MISP_WDR_TEST_INPUT_SHIFT:  WDRTEST_WdrInputBitShift(pStr);  break;  //WDR input bit mode
		case MISP_WDR_TEST_DISP_FLAG:    WDRTEST_WdrDebugDisp(pStr);      break;  //WDR display flag
		case MISP_WDR_TEST_HOLD_AE:      WDRTEST_WdrHoldAEProc(pStr);     break;  //WDR hold AE proc.
		case MISP_WDR_TEST_HOLD_TM:      WDRTEST_WdrHoldTMProc(pStr);     break;  //WDR hold TM proc.
		case MISP_WDR_TEST_CSP_EN:       WDRTEST_WdrEnhance(pStr);        break;  //WDR enhanced algo.
		case MISP_WDR_TEST_INPUT_BIT:    WDRTEST_WdrToggleBitMode(pStr);  break;  //WDR input bit-mode
		case MISP_WDR_TEST_MERGE_NUM:    WDRTEST_WdrMergeNum(pStr);       break;  //WDR merge num
		case MISP_WDR_TEST_CSP_USE_HIS:  WDRTEST_WdrCspMotionHist(pStr);  break;  //WDR CSP use motion history
		case MISP_WDR_TEST_CSP_MD_THR:   WDRTEST_WdrCspMotionThr(pStr);   break;  //WDR CSP motion threshold
		case MISP_WDR_TEST_HIS_MINUS:    WDRTEST_WdrCspHistMinus(pStr);   break;  //WDR CSP motion history minus
		case MISP_WDR_TEST_WB_POS:       WDRTEST_WdrMgrPrevWB(pStr);      break;  //WDR WB adjust position
		case MISP_WDR_TEST_TOGGLE_CC:    WDRTEST_WdrToggleCC(pStr);       break;  //WDR toggle CC
		case MISP_WDR_TEST_TM_LSIZE:     WDRTEST_WdrSetTmLsize(pStr);     break;  //WDR TM Local size
		case MISP_WDR_TEST_TM_LGAIN:     WDRTEST_WdrSetTmLgain(pStr);     break;  //WDR TM Local gain
		case MISP_WDR_TEST_MGR_BLEND:    WDRTEST_WdrSetMgrBlend(pStr);    break;  //WDR MGR blend
		case MISP_WDR_TEST_MGR_EXPAND:   WDRTEST_WdrSetMgrExpSel(pStr);   break;  //WDR MGR expand selection
		case MISP_WDR_TEST_DISP_FRAME:   WDRTEST_WdrDispMgrSel(pStr);     break;  //WDR MGR use frame
		case MISP_WDR_TEST_TM_OFFSET:    WDRTEST_WdrTmOffset(pStr);       break;  //WDR TM offset control
		default:                         strcpy(pStr, "N/A       ");      break;  //N/A
	}
	return (PBYTE)pStr;
}


//--------------------------------------------------------------------------------------------------------------------------
//for debug OSD
static CHAR debugStr[64];  //max.64 (refer to OSD_StrDispAttb())
PCHAR GetWdr_DebugStr(BYTE nID) {

	WORD exp_ratio;
#if ADAPT_TM_SHORTER
  if(stWDRCTRL.ctrl&WDRCTRL_MODE_LINE3)
	exp_ratio=100*stWDRCTRL.rate_long/stWDRCTRL.rate_sh_2;  //expected ratio (long/shorter)
  else
#endif
	exp_ratio=100*stWDRCTRL.rate_long/stWDRCTRL.rate_sh_1;  //expected ratio (long/short)

	switch(nID) {
		case 0:  sprintf(debugStr, "WDR ctrl       : [0x%08lx] Pre.%d", stWDRCTRL.ctrl, stWDRCTRL.state_wdr);       break;
		case 1:  sprintf(debugStr, "WDR auto satur : %4d (%3d~%4d)", GetOUT_MainSaturation(), stWDRCTRL.satur_min, stWDRCTRL.satur_max);  break;
#if REV_WDR_MANU_TM
		case 2:  sprintf(debugStr, "TM pxsum ratio : %4d (%5d/%5d)", stWDRCTRL.cur_ratio, stWDRCTRL.psum_long, stWDRCTRL.psum_shrt);  break;
		case 3:  sprintf(debugStr, "TM gain offset : %4ld/%4d => %2d/64", stWDRCTRL.pre_ratio/16, exp_ratio, stWDRCTRL.tone_offs);  break;
#else
		case 2:  sprintf(debugStr, "WDR deghost_en : %d (%2d)",      stWDRCTRL.deghst_en, stWDRCTRL.dark_cntr);  break;
		case 3:  sprintf(debugStr, "WDR short max. : %4d (x%2d)",    stWDRCTRL.tma_level, stWDRCTRL.rate_long);  break;
#endif
		case 4:  sprintf(debugStr, "WDR curr Y sum : %4d (%5d~%5d)", stWDRCTRL.cur_Y_sum, stWDRCTRL.targt_min, stWDRCTRL.targt_max);  break;
		case 5:  sprintf(debugStr, "WDR gain index : %4d (%3d~%4d)", stWDRCTRL.gain_ctrl, GetWDR_GainMin(), GetWDR_GainMax());  break;
#if WDR_IRIS_CTRL_EN
		case 6:  sprintf(debugStr, "WDR iris index : %4d (%3d~%4d)", stWDRCTRL.iris_ctrl, GetWDR_IrisMin(), GetWDR_IrisMax());  break;
#else
		case 6:  sprintf(debugStr, "WDR iris index : ----");  break;
#endif
		case 7:  sprintf(debugStr, "WDR shut index : %4d (%3d~%4d)", stWDRCTRL.shut_ctrl, GetWDR_ShutMin(), GetWDR_ShutMax());  break;
		default: sprintf(debugStr, " ");  break;
	}
	return (PCHAR)debugStr;
}


//--------------------------------------------------------------------------------------------------------------------------
//for rotation setting
BYTE SetWDR_Rotation_mode(BYTE mode, WORD h_size, WORD v_size, BYTE bit_precision) {
#if REV_ROT_FUNC_EN
	BYTE bt_mode=bit_precision?MISP_DDRMAP_10b:MISP_DDRMAP_12b;  // bit precision - 0:12bit, 1:10bit
	BYTE wr_mode=mode?MISP_WDR_MEM_WIRTE_HTM_MODE:MISP_WDR_MEM_WIRTE_OFF_MODE;  //sensor write mode

	SetSS_wdr_write_mode(wr_mode, bt_mode, mode);            // Sensor Write mode
	MISP_RegField(MISP_LOCAL_ID, 0x59C, 12, 2, bt_mode);     // WDR RW MRU setting
	MISP_RegWrite(MISP_LOCAL_ID, 0x59D, (mode<<13)|0x0000);  // WDR linemem enable
	MISP_RegField(MISP_LOCAL_ID, 0x5C0, 0, 13, h_size);      // WDR in_hsize = Rotated h size
	MISP_RegField(MISP_LOCAL_ID, 0x5C1, 0, 13, v_size);      // WDR in_vsize = Rotated v size
	return 0;
#else
	return 1;  //failure : i500 n/a
#endif
}


//--------------------------------------------------------------------------------------------------------------------------
static void stDebug_Print(const BYTE debug_lev, const char* format, ...) {
#if WDR_DBG_PRINT
	char dbgString[255];
	va_list arg_ptr;

	//[debug level]
	//  0 : TEST func.
	//  1 : API call
	//  2 : AE ctrl
	//  3 : AE more (hold)
	//  4 : Sepcial (low light)
	//  5 : Ysum
	//  6 : TM ctrl (adjust)
	//  7 : TM ctrl (manual)
	if(debug_lev>stWDRCTRL.debug_lev)
		return;

	//parse debug string
	va_start(arg_ptr, format);
	vsprintf(dbgString, format, arg_ptr);
	UARTprintf("%s", dbgString);
	va_end(arg_ptr);
#endif
	return;
}



//--------------------------------------------------------------------------------------------------------------------------


/*  FILE_END_HERE */
