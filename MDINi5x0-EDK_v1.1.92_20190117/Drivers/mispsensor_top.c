// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"misp100.h"
#include	"cmos_if.h"		// for sensor dependent option

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

#if defined(__MDIN_i5XX_FPGA__)
#define	__USE_REF_SENSOR_BOARD__ 		// use fpga sensor base board rev0.5(for fpga lvds pin order)
#endif

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static SS_CTRL_PARAM stSSCTRL;

#if defined(__USE_CMOS_IMX226__) 
static WORD ClkConv_roof=0;
#endif
// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
static void Init_SS_PARAM( PSS_CTRL_PARAM pCTRL );
static void Init_SS_REGISTER( PSS_CTRL_PARAM pCTRL );
static void SSCTRL_DeserializerHandler(PSS_CTRL_PARAM pCTRL);
static void SSCTRL_ResetDeserializerFIFO(PSS_CTRL_PARAM pCTRL);
static void SSCTRL_BlackLevelHandler(PSS_CTRL_PARAM pCTRL);
static void SSCTRL_BinningHandler(PSS_CTRL_PARAM pCTRL);
static void SSCTRL_ImageStartpointHandler(PSS_CTRL_PARAM pCTRL);
static void SSCTRL_ImageSizeHandler(PSS_CTRL_PARAM pCTRL);
static void SSCTRL_WDRMemWriteHandler(PSS_CTRL_PARAM pCTRL);
static void SSCTRL_DecompEnableHandler(PSS_CTRL_PARAM pCTRL);
static void SSCTRL_SetLvdsInDelay(PSS_CTRL_PARAM pCTRL);

#if __SS_SI_SETUP__
static void SSCTRL_SyncIntegrityHandler(PSS_CTRL_PARAM pCTRL);
#endif
// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
static void Init_SS_PARAM( PSS_CTRL_PARAM pCTRL ){
#if defined(__USE_WDR_PROCESS__)
	BYTE ext_hsize;  //extend active size to support WDR
#endif
  
#if defined(__USE_CMOS_MN34220__)				// use panasonic cmos	
		
	SetSS_deserializer_mn34220_s4();	
	
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
		
	pCTRL->startH = 13 + 2;
	pCTRL->startV = 21 ;

	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;
	#if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p25;
	#else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p30;
	#endif

	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 8 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 2 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1080 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 10 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 256 ;
	pCTRL->stBLACK_LEVEL.odm_blc_val = 256 ;	

	pCTRL->stHTM_DECOMP.output_range_sel = 0 ;
	pCTRL->stHTM_DECOMP.decomp_en = 0 ; // 0 : output[11:0] 1 : output[15:4] 2 : output[19:8]
	
	pCTRL->stHTM_DECOMP.decomp_p1 = 65535 ;
	pCTRL->stHTM_DECOMP.decomp_p2 = 0 ;
	pCTRL->stHTM_DECOMP.decomp_p3 = 0 ;
	
	pCTRL->stHTM_DECOMP.offset_p2 = 0 ;
	pCTRL->stHTM_DECOMP.offset_p3 = 0 ;
	
	pCTRL->stHTM_DECOMP.p1_mul_sel = 0 ; // 0: x1 1: x2 2: x4 3: x8 4: x16 5: x32 6: x64
	pCTRL->stHTM_DECOMP.p2_mul_sel = 0 ;
	pCTRL->stHTM_DECOMP.p3_mul_sel = 0 ;
	
	pCTRL->stHTM_DECOMP.bl_pre_value = 0 ;
	pCTRL->stHTM_DECOMP.bl_post_value = 256 ;
	pCTRL->stHTM_DECOMP.input_max_value = 65535 ;
#elif defined(__USE_CMOS_MN34420__)				// use panasonic cmos	
		
	SetSS_deserializer_mn34420_s4();	
	
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;

	#if defined(__MN34420_1280x1024_ISPCROP__)
	pCTRL->startH = 13 + 2 + 288;
	pCTRL->startV = 21 -4;

	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1280 + 64 ;  //1344-->1440(real for wdr)
	pCTRL->active_v_size = 1024 + 64 ;  //1088-->1092(real for wdr)
	
	#if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1344x1088p25;
	#else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1344x1088p30;
	#endif

	#else
	pCTRL->startH = 13 + 2;
	pCTRL->startV = 21 ;

	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;
	#if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p25;
	#else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p30;
	#endif
	#endif

	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 8 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 2 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1080 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 10 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 256 ;
	pCTRL->stBLACK_LEVEL.odm_blc_val = 256 ;	

	pCTRL->stHTM_DECOMP.output_range_sel = 0 ;
	pCTRL->stHTM_DECOMP.decomp_en = 0 ; // 0 : output[11:0] 1 : output[15:4] 2 : output[19:8]
	
	pCTRL->stHTM_DECOMP.decomp_p1 = 65535 ;
	pCTRL->stHTM_DECOMP.decomp_p2 = 0 ;
	pCTRL->stHTM_DECOMP.decomp_p3 = 0 ;
	
	pCTRL->stHTM_DECOMP.offset_p2 = 0 ;
	pCTRL->stHTM_DECOMP.offset_p3 = 0 ;
	
	pCTRL->stHTM_DECOMP.p1_mul_sel = 0 ; // 0: x1 1: x2 2: x4 3: x8 4: x16 5: x32 6: x64
	pCTRL->stHTM_DECOMP.p2_mul_sel = 0 ;
	pCTRL->stHTM_DECOMP.p3_mul_sel = 0 ;
	
	pCTRL->stHTM_DECOMP.bl_pre_value = 0 ;
	pCTRL->stHTM_DECOMP.bl_post_value = 256 ;
	pCTRL->stHTM_DECOMP.input_max_value = 65535 ;
		
#elif defined(__USE_CMOS_MN34041__) 
	
	SetSS_deserializer_mn34041_s4();
	
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	
	pCTRL->startH = 67 ;
	pCTRL->startV = 11 ;

	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p60;
	
	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 8 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 8 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 20 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 1080 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 256 ;
	pCTRL->stBLACK_LEVEL.odm_blc_val = 256 ;		
	
#elif defined(__USE_CMOS_IMX076__)
	
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	
	pCTRL->startH = 68 ;
	pCTRL->startV = 36 ;

	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1650 ;
	pCTRL->active_v_size = 1080 ;
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1368x1049p60;
	
	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 1 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 2 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 41 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 20 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 480 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 256 ;
	pCTRL->stBLACK_LEVEL.odm_blc_val = 256 ;		
	
#elif defined(__USE_CMOS_PO3100K__)
	
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	
	pCTRL->startH = 8 + 6;
	pCTRL->startV = 1 ;

	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1280 ;
	pCTRL->active_v_size = 720 ;
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p60;
	
	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 0 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 2 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 41 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 20 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 480 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 0 ;
	pCTRL->stBLACK_LEVEL.odm_blc_val = 0 ;		
	
#elif defined(__USE_CMOS_PS1210K__)

	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	
	pCTRL->startH = 1 + 14;
	pCTRL->startV = 2 ;

	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p60;
	
	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 0 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 0 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 0 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 0 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 0 ;
	pCTRL->stBLACK_LEVEL.odm_blc_val = 0 ;		
#elif defined(__USE_CMOS_PS1400K__)
	SetSS_deserializer_PS1400K_s4();	
	
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
		
	pCTRL->startH = 110;
	pCTRL->startV = 0 ;

	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
//	pCTRL->active_h_size = 2544 ;		//2688		//2560x1440 --> qhd. fix together mispfrmt.c..
//	pCTRL->active_v_size = 1431 ;		//1520	

	pCTRL->active_h_size = 2560 ;		//2688		//2560x1440 --> qhd
	pCTRL->active_v_size = 1440 ;		//1520		
//	pCTRL->active_h_size = 1920 ;		//2688		//2560x1440 --> qhd
//	pCTRL->active_v_size = 1080 ;		//1520		

	pCTRL->sensor_vout_frmt = MISP_VIDSRC_SS2560x1440p30;


	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 8 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 2 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1080 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 10 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 168 ;		//10b42
	pCTRL->stBLACK_LEVEL.odm_blc_val = 168 ;	

	pCTRL->stHTM_DECOMP.output_range_sel = 0 ;
	pCTRL->stHTM_DECOMP.decomp_en = 0 ; // 0 : output[11:0] 1 : output[15:4] 2 : output[19:8]
	
	pCTRL->stHTM_DECOMP.decomp_p1 = 65535 ;
	pCTRL->stHTM_DECOMP.decomp_p2 = 0 ;
	pCTRL->stHTM_DECOMP.decomp_p3 = 0 ;
	
	pCTRL->stHTM_DECOMP.offset_p2 = 0 ;
	pCTRL->stHTM_DECOMP.offset_p3 = 0 ;
	
	pCTRL->stHTM_DECOMP.p1_mul_sel = 0 ; // 0: x1 1: x2 2: x4 3: x8 4: x16 5: x32 6: x64
	pCTRL->stHTM_DECOMP.p2_mul_sel = 0 ;
	pCTRL->stHTM_DECOMP.p3_mul_sel = 0 ;
	
	pCTRL->stHTM_DECOMP.bl_pre_value = 0 ;
	pCTRL->stHTM_DECOMP.bl_post_value = 256 ;
	pCTRL->stHTM_DECOMP.input_max_value = 65535 ;

#elif defined(__USE_CMOS_IMX124__) // use sony IMX123,4 cmos
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	#if defined (__USE_CMOS_IMX123_LVDS_S8__)
	SetSS_deserializer_imx124_s8();
	#else
	SetSS_deserializer_imx124_s4();
	#endif

	#if defined(__USE_IMX124_1536P60_MODE__)||defined(__USE_IMX124_1536P30_MODE__) 
	pCTRL->startH = 4 + 2 ;
	pCTRL->startV = 18 ;	
	  #if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	  #if defined(__USE_WDR_PROCESS__)
	pCTRL->bH = 0 ;
	pCTRL->bV = 0 ;
	  #else
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	  #endif
	  #else 																						// i500
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	  #endif
	
	pCTRL->active_h_size = 2048 ;
	pCTRL->active_v_size = 1536 ;	
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_2048x1536p25;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_2048x1536p30;
	  #endif

	#elif defined(__USE_IMX124_1080P60_MODE__) 
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p50;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p60;
	  #endif		

	#else	
	pCTRL->startH = 0 ;
	pCTRL->startV = 18 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;	
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p25;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p30;
	  #endif		
	#endif

	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 2 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1024 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 240 ;
	pCTRL->stBLACK_LEVEL.odm_blc_val = 240 ;		

	pCTRL->stHTM_DECOMP.output_range_sel = 0 ;
	pCTRL->stHTM_DECOMP.decomp_en = 0 ; // 0 : output[11:0] 1 : output[15:4] 2 : output[19:8]
	
	pCTRL->stHTM_DECOMP.decomp_p1 = 2048 ;
	pCTRL->stHTM_DECOMP.decomp_p2 = 2944 ;
	pCTRL->stHTM_DECOMP.decomp_p3 = 3713 ;
	
	pCTRL->stHTM_DECOMP.offset_p2 = 16384 ;
	pCTRL->stHTM_DECOMP.offset_p3 = 0 ;
	
	pCTRL->stHTM_DECOMP.p1_mul_sel = 4 ; // 0: x1 1: x2 2: x4 3: x8 4: x16 5: x32 6: x64
	pCTRL->stHTM_DECOMP.p2_mul_sel = 6 ;
	pCTRL->stHTM_DECOMP.p3_mul_sel = 0 ;
	
	pCTRL->stHTM_DECOMP.bl_pre_value = 0 ;
	pCTRL->stHTM_DECOMP.bl_post_value = 240 ;
	pCTRL->stHTM_DECOMP.input_max_value = 3712 ;
	
#elif defined(__USE_CMOS_IMX252__) // use sony IMX252 cmos
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;

	SetSS_deserializer_imx252_s4();

	#if defined(__USE_IMX252_1080P60_MODE__) 
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p50;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p60;
	  #endif		
	#elif defined(__USE_IMX252_1080P30_MODE__) 
	pCTRL->startH = 0 ;
	pCTRL->startV = 18 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;	
 	  #if defined(__USE_25Hz_SENSOR__)
 	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p25;
 	  #else
 	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p30;
 	  #endif		
	#else
	pCTRL->startH = 4 + 2 ;
	pCTRL->startV = 18 ;	
	  #if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	  #if defined(__USE_WDR_PROCESS__)
	pCTRL->bH = 0 ;
	pCTRL->bV = 0 ;
	  #else
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	  #endif
	  #else 																						// i500
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	  #endif
	pCTRL->active_h_size = 2048 ;
	pCTRL->active_v_size = 1536 ;	
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_2048x1536p25;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_2048x1536p30;
	  #endif
	#endif
	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 2 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1024 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 240 ;
	pCTRL->stBLACK_LEVEL.odm_blc_val = 240 ;		

	pCTRL->stHTM_DECOMP.output_range_sel = 0 ;
	pCTRL->stHTM_DECOMP.decomp_en = 0 ; // 0 : output[11:0] 1 : output[15:4] 2 : output[19:8]
	
	pCTRL->stHTM_DECOMP.decomp_p1 = 2048 ;
	pCTRL->stHTM_DECOMP.decomp_p2 = 2944 ;
	pCTRL->stHTM_DECOMP.decomp_p3 = 3713 ;
	
	pCTRL->stHTM_DECOMP.offset_p2 = 16384 ;
	pCTRL->stHTM_DECOMP.offset_p3 = 0 ;
	
	pCTRL->stHTM_DECOMP.p1_mul_sel = 4 ; // 0: x1 1: x2 2: x4 3: x8 4: x16 5: x32 6: x64
	pCTRL->stHTM_DECOMP.p2_mul_sel = 6 ;
	pCTRL->stHTM_DECOMP.p3_mul_sel = 0 ;
	
	pCTRL->stHTM_DECOMP.bl_pre_value = 0 ;
	pCTRL->stHTM_DECOMP.bl_post_value = 240 ;
	pCTRL->stHTM_DECOMP.input_max_value = 3712 ;
	
#elif defined(__USE_CMOS_IMX265__) // use sony IMX265 cmos
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;

	SetSS_deserializer_imx265_s4();

	#if defined(__USE_IMX265_1080P60_MODE__) 
	#if defined(__USE_SENS_HSYNC_EXT__)
	pCTRL->startH = 14 ;
	#else
	pCTRL->startH = 4 ;
	#endif
	pCTRL->startV = 18 ;		
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p50;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p60;
	  #endif		
	#elif defined(__USE_IMX265_ROI_MODE__) 
	pCTRL->startH = 0 ;
	pCTRL->startV = 12 ;		
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p50;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p60;
	  #endif	
	#elif defined(__USE_IMX265_1080P30_MODE__) 
	#if defined(__USE_SENS_HSYNC_EXT__)
	pCTRL->startH = 14 ;
	#else
	pCTRL->startH = 4 ;
	#endif
	pCTRL->startV = 18 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;	
 	  #if defined(__USE_25Hz_SENSOR__)
 	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p25;
 	  #else
 	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p30;
 	  #endif		
	#else		//all scan mode
	  #if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	  #if defined(__USE_WDR_PROCESS__)
	pCTRL->startH = 8 ;
	pCTRL->startV = 12 ;	  
	pCTRL->bH = 0 ;
	pCTRL->bV = 0 ;
	  #else
	pCTRL->startH = 4 + 2 ;
	pCTRL->startV = 12 ;	  
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	  #endif
	  #else 																						// i500
	pCTRL->startH = 4 + 2 ;
	pCTRL->startV = 12 ;	  
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	  #endif
	pCTRL->active_h_size = 2048 ;
	pCTRL->active_v_size = 1536 ;	
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_2048x1536p25;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_2048x1536p30;
	  #endif
	#endif
	
	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 2 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1024 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 240 ;
	pCTRL->stBLACK_LEVEL.odm_blc_val = 240 ;		

	pCTRL->stHTM_DECOMP.output_range_sel = 0 ;
	pCTRL->stHTM_DECOMP.decomp_en = 0 ; // 0 : output[11:0] 1 : output[15:4] 2 : output[19:8]
	
	pCTRL->stHTM_DECOMP.decomp_p1 = 2048 ;
	pCTRL->stHTM_DECOMP.decomp_p2 = 2944 ;
	pCTRL->stHTM_DECOMP.decomp_p3 = 3713 ;
	
	pCTRL->stHTM_DECOMP.offset_p2 = 16384 ;
	pCTRL->stHTM_DECOMP.offset_p3 = 0 ;
	
	pCTRL->stHTM_DECOMP.p1_mul_sel = 4 ; // 0: x1 1: x2 2: x4 3: x8 4: x16 5: x32 6: x64
	pCTRL->stHTM_DECOMP.p2_mul_sel = 6 ;
	pCTRL->stHTM_DECOMP.p3_mul_sel = 0 ;
	
	pCTRL->stHTM_DECOMP.bl_pre_value = 0 ;
	pCTRL->stHTM_DECOMP.bl_post_value = 240 ;
	pCTRL->stHTM_DECOMP.input_max_value = 3712 ;

#elif defined(__USE_CMOS_IMX174__)  ||  defined(__USE_CMOS_IMX249__)  // use sony IMX249 cmos// use sony IMX174 cmos
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	#if defined(__USE_CMOS_IMX249__)
	SetSS_deserializer_imx249_s2();
	#else
	SetSS_deserializer_imx174_s4();
	#endif

	pCTRL->startH = 4 ;
	pCTRL->startV = 18 ;	
	pCTRL->bH = 4 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;	
	
	#if defined(__USE_IMX174_1080P60_MODE__) 
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p50;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p60;
	  #endif		
	#else	
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p25;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p30;
	  #endif		
	#endif

	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 2 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1024 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 240 ;
	pCTRL->stBLACK_LEVEL.odm_blc_val = 240 ;		
#elif defined(__USE_CMOS_IMX253__) // use sony IMX253 cmos
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	SetSS_deserializer_imx253_s8();

	pCTRL->startH = 12 ;
	pCTRL->startV = 28 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 3840 ;
	pCTRL->active_v_size = 2160 ;	
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_3840x2160p25;		
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_3840x2160p30;		
	  #endif		

	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 6 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 2048 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 6 ;

	pCTRL->stBLACK_LEVEL.manual_blc_val = 240 ;	
	pCTRL->stBLACK_LEVEL.odm_blc_val = 240 ;
#elif defined(__USE_CMOS_IMX273__) // use sony IMX273 cmos
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;

	#if defined(__USE_CMOS_IMX273_LVDS_S8__)
		SetSS_deserializer_imx273_s8();
	#elif defined(__USE_CMOS_IMX273_LVDS_S2__)
		SetSS_deserializer_imx273_s2();
	#else
		SetSS_deserializer_imx273_s4();
	#endif
	
	#if defined(__USE_IMX273_ALLSCAN_MODE__)
	pCTRL->startH = 0 ;
	pCTRL->startV = 10 ;	
	pCTRL->bH = 8 ;
	pCTRL->bV = 1 ;
	pCTRL->active_h_size = 1440 ;
	pCTRL->active_v_size = 1080 ;	

	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1440x1080p60;		
	#elif defined(__USE_IMX273_ROI_MODE__)
	pCTRL->startH = 88;
	pCTRL->startV = 12 + 30;	//for ob=12, center=30
	pCTRL->bH = 2 ;
	pCTRL->bV = 0 ;
	pCTRL->active_h_size = 1280 ;
	pCTRL->active_v_size = 720 ;	

	  #if defined(__USE_IMX273_720P240_MODE__)
		#if defined(__USE_25Hz_SENSOR__)
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p200;
			//pCTRL->startV = 12;			
		#else
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p240;
			#if defined(__USE_IMX273_SLAVE_MODE__)
			pCTRL->startV = 12;
			//pCTRL->startV = 0;	//for v=750d setting 0 or 12(see ob area) or not(708d active..)... as your wish..
			#endif
		#endif
	  #elif defined(__USE_IMX273_720P120_MODE__)
		#if defined(__USE_25Hz_SENSOR__)
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p100;
		#else
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p120;
		#endif
	  #elif defined(__USE_IMX273_720P30_MODE__)
		#if defined(__USE_25Hz_SENSOR__)		//1400x1024,	1280x1024 also ok
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p25;		
		#else
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p30;		
	 	#endif	
	  #elif defined(__USE_IMX273_480P60_MODE__)
		pCTRL->startH = 0;
		pCTRL->startV = 12;	//for ob=12, center=30
		pCTRL->bH = 2 ;
		pCTRL->bV = 2 ;
		pCTRL->active_h_size = 720 ;
		pCTRL->active_v_size = 480 ;	

		pCTRL->sensor_vout_frmt = MISP_VIDSRC_720x480p60;		
	  #else
		#if defined(__USE_25Hz_SENSOR__)		//1400x1024,	1280x1024 also ok
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p50;		
		#else
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p60;		
	 	#endif		
	  #endif
	#elif defined(__USE_IMX273_480P60_MODE__) && !defined(__USE_IMX273_ROI_MODE__)	//2x2 sub-sampling mode
		pCTRL->startH = 0;
		pCTRL->startV = 0;	
		pCTRL->bH = 2 ;
		pCTRL->bV = 2 ;
		pCTRL->active_h_size = 720 ;
		pCTRL->active_v_size = 480 ;	
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_720x480p60;		  
	#else
	pCTRL->startH = 14 + 80;
	pCTRL->startV = 28 + 180 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1280 ;
	pCTRL->active_v_size = 720 ;	

	  #if defined(__USE_IMX273_720P240_MODE__)
		#if defined(__USE_25Hz_SENSOR__)
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p200;
		#else
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p240;
		#endif
	  #elif defined(__USE_IMX273_720P120_MODE__)
		#if defined(__USE_25Hz_SENSOR__)
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p100;
		#else
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p120;
		#endif
	  #elif defined(__USE_IMX273_720P30_MODE__)
		#if defined(__USE_25Hz_SENSOR__)		//1400x1024,	1280x1024 also ok
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p25;		
		#else
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p30;		
	 	#endif			
	  #else
		#if defined(__USE_25Hz_SENSOR__)		//1400x1024,	1280x1024 also ok
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p50;		
		#else
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p60;		
	 	#endif		
	  #endif
	#endif
	

	#if defined(__USE_SENS_HSYNC_EXT__)
		stSSCTRL.stDESERIALIZER.ext_hactive = 4;
	#endif

	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	pCTRL->stBLACK_LEVEL.blc_hactend_robust = 1;
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 6 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1000 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 4;

	pCTRL->stBLACK_LEVEL.manual_blc_val = 240 ;	
	pCTRL->stBLACK_LEVEL.odm_blc_val = 240 ;
#elif defined(__USE_CMOS_IMX178__) // use sony IMX136 cmos
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	SetSS_deserializer_imx178();

	#if defined(__USE_IMX178_1080P60_MODE__) 
	pCTRL->startH = 8 ;
	pCTRL->startV = 24 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p50;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p60;
	  #endif
	#elif defined(__USE_IMX178_1080P30_MODE__) 
	pCTRL->startH = 8 ;
	pCTRL->startV = 24 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p25;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p30;
	  #endif
	#else	
	pCTRL->startH = 12 ;
	pCTRL->startV = 28 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 3072 ;
	pCTRL->active_v_size = 1728 ;	
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_3072x1728p25;		
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_3072x1728p30;		
	  #endif		
	#endif

	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 6 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 2048 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 6 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 60 ;	
	pCTRL->stBLACK_LEVEL.odm_blc_val = 60 ;		
#elif defined(__USE_CMOS_IMX185__) // use sony IMX185 cmos
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	SetSS_deserializer_imx185_s4();

	pCTRL->startH = 6 ;
	pCTRL->startV = 18 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;	
	

	#if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p25;
	#else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p30;
	#endif		

	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 2 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1024 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 240 ;
	pCTRL->stBLACK_LEVEL.odm_blc_val = 240 ;		
	
#elif defined(__USE_CMOS_IMX385__) // use sony IMX385 cmos
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	
	#if defined(__USE_IMX385_720P30_MODE__)
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p25;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p30;
	  #endif
	#endif // #if defined(__USE_IMX385_720P30_MODE__)

	#if defined(__USE_IMX385_720P60_MODE__)
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p50;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p60;
	  #endif
	#endif // #if defined(__USE_IMX385_720P60_MODE__)	

	#if defined(__USE_IMX385_1080P30_MODE__)
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p25;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p30;
	  #endif
	#endif // #if defined(__USE_IMX385_1080P30_MODE__)	

	#if defined(__USE_IMX385_1080P60_MODE__)
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p50;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p60;
	  #endif
	#endif // #if defined(__USE_IMX385_1080P60_MODE__)		

	#if defined(__USE_IMX385_1080P120_MODE__)	
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p100;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p120;
	  #endif
	#endif // #if defined(__USE_IMX385_1080P120_MODE__)		

	pCTRL->active_h_size = defMISPSrcVideo[pCTRL->sensor_vout_frmt].Hact ;
	pCTRL->active_v_size = defMISPSrcVideo[pCTRL->sensor_vout_frmt].Vact ;	
	
	#if defined(__USE_CMOS_IMX385_LVDS_S8__)
	SetSS_deserializer_imx385_s8();
	#elif defined(__USE_CMOS_IMX385_LVDS_S4__)
	SetSS_deserializer_imx385_s4();
	#elif defined(__USE_CMOS_IMX385_CSI2LVDS6CH__)
	SetSS_deserializer_imx385_s6();
	#elif defined(__USE_CMOS_IMX385_CSI2TTL__)
	SetSS_deserializer_imx385_ttl(); // 
	#endif
		
	#if defined(__USE_CMOS_IMX385_CSI2TTL__)	
		pCTRL->startH = 8 ; // re setup TTL input start point
		pCTRL->startV = 1 ;	// re setup TTL input start point		
		pCTRL->bH = 2 ;
		pCTRL->bV = 2 ;	
	#elif defined(__USE_CMOS_IMX385_CSI2LVDS6CH__)	
		pCTRL->startH = 10 ; // re setup TTL input start point
		pCTRL->startV = 18 ;	// re setup TTL input start point		
		pCTRL->bH = 2 ;
		pCTRL->bV = 2 ;			
	#else 
		pCTRL->startH = 10 ;
		pCTRL->startV = 20 ;	
		pCTRL->bH = 2 ;
		pCTRL->bV = 2 ;		
	#endif
	
	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 2 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1024 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 240 ;	

#elif defined(__USE_CMOS_IMXx22__) || defined(__USE_CMOS_IMX323__) 
	
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	SetSS_deserializer_imx222_ttl();

	#if defined(__USE_IMXx22_720P30_MODE__)
	pCTRL->startH = 154 ;
	pCTRL->startV = 13 ;			
	pCTRL->bH = 4;
	pCTRL->bV = 4 ;
	pCTRL->active_h_size = 1280 ;
	pCTRL->active_v_size = 720 ;

	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p30;
	
	#else
	pCTRL->startH = 154 ;
	pCTRL->startV = 23 ;			
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;

	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p25;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p30;
	  #endif
	
	#endif

	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	

	#if defined(__USE_IMXx22_720P30_MODE__)
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 4 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1000 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	#else
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 4 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1808 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	#endif	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 240 ;
		
	pCTRL->stHTM_DECOMP.output_range_sel = 0 ;
	pCTRL->stHTM_DECOMP.decomp_en = 0 ; // 0 : output[11:0] 1 : output[15:4] 2 : output[19:8]
	
	pCTRL->stHTM_DECOMP.decomp_p1 = 2048 ;
	pCTRL->stHTM_DECOMP.decomp_p2 = 2944 ;
	pCTRL->stHTM_DECOMP.decomp_p3 = 3713 ;
	
	pCTRL->stHTM_DECOMP.offset_p2 = 16384 ;
	pCTRL->stHTM_DECOMP.offset_p3 = 0 ;
	
	pCTRL->stHTM_DECOMP.p1_mul_sel = 4 ; // 0: x1 1: x2 2: x4 3: x8 4: x16 5: x32 6: x64
	pCTRL->stHTM_DECOMP.p2_mul_sel = 6 ;
	pCTRL->stHTM_DECOMP.p3_mul_sel = 0 ;
	
	pCTRL->stHTM_DECOMP.bl_pre_value = 0 ;
	pCTRL->stHTM_DECOMP.bl_post_value = 60 ;
	pCTRL->stHTM_DECOMP.input_max_value = 3712 ;
	
#elif defined(__USE_CMOS_IMX238__) // use sony IMX238 cmos
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	SetSS_deserializer_imx238_s4();

	pCTRL->startH = 0 ;
	pCTRL->startV = 18 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1280 ;
	pCTRL->active_v_size = 720 ;	
	

	#if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p25;
	#else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p30;
	#endif		

	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 2 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1024 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 240 ;	
#elif defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__) // use sony IMX290 cmos
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	
	#if defined(__USE_IMX290_720P30_MODE__)
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p25;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p30;
	  #endif
	pCTRL->startH = 10 ;
	pCTRL->startV = 8 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;	  
	#endif // #if defined(__USE_IMX290_720P30_MODE__)

	#if defined(__USE_IMX290_720P60_MODE__)
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p50;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p60;
	  #endif
	pCTRL->startH = 10 ;
	pCTRL->startV = 8 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;	  
	#endif // #if defined(__USE_IMX290_720P60_MODE__)	

	#if defined(__USE_IMX290_720P120_MODE__)
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p100;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p120;
	  #endif
	pCTRL->startH = 10 ;
	pCTRL->startV = 8 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;	  
	#endif // #if defined(__USE_IMX290_720P120_MODE__)		

	#if defined(__USE_IMX290_720P120_ISPCROP__)	
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p100;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p120;
	  #endif

	pCTRL->startH = 330 ;
	pCTRL->startV = 198 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;	  
	#endif // #if defined(__USE_IMX290_720P120_ISPCROP__)		

	#if defined(__USE_IMX290_1080P30_MODE__)
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p25;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p30;
	  #endif

	pCTRL->startH = 10 ;
	pCTRL->startV = 18 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	#endif // #if defined(__USE_IMX290_1080P30_MODE__)	

	#if defined(__USE_IMX290_1080P60_MODE__)
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p50;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p60;
	  #endif

	pCTRL->startH = 10 ;
	pCTRL->startV = 18 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;	  
	#endif // #if defined(__USE_IMX290_1080P60_MODE__)		

	#if defined(__USE_IMX290_1080P120_MODE__)	
	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p100;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p120;
	  #endif

	pCTRL->startH = 10 ;
	pCTRL->startV = 18 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;	  
	#endif // #if defined(__USE_IMX290_1080P120_MODE__)		

	pCTRL->active_h_size = defMISPSrcVideo[pCTRL->sensor_vout_frmt].Hact ;
	pCTRL->active_v_size = defMISPSrcVideo[pCTRL->sensor_vout_frmt].Vact ;	
	
	#if defined(__USE_CMOS_IMX290_LVDS_S8__)
	SetSS_deserializer_imx290_s8();
	#else
	SetSS_deserializer_imx290_s4();
	#endif

	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 2 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1024 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 240 ;	

#elif defined(__USE_CMOS_IMX226__) // use sony IMX290 cmos
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	#if defined(__USE_CMOS_IMX226_LVDS_S4__) || defined(__USE_IMX226_1080P60_MODE__)
	SetSS_deserializer_imx226_s4();
	#elif defined(__USE_IMX226_2160P60_MODE4__)
	SetSS_deserializer_imx226_s6();
	#else
	SetSS_deserializer_imx226_s8();
	#endif

	#if defined (__USE_IMX226_2160P30_MODE0__)
	pCTRL->startH = 94+128 ; // + (4096-3840)/2
	pCTRL->startV = 15 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 3840 ;
	pCTRL->active_v_size = 2160 ;	

	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_SS3840x2160p25;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_3840x2160p30;
	  #endif		

	#elif defined (__USE_IMX226_4K3KP30_MODE0__)
	pCTRL->startH = 94 ;
	pCTRL->startV = 15 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 4000 ;
	pCTRL->active_v_size = 3000 ;	

	pCTRL->sensor_vout_frmt = MISP_VIDSRC_4000x3000p22;
	  
	#elif defined ( __USE_IMX226_2160P30_MODE1__ )
		
	pCTRL->startH = 94 ;
	pCTRL->startV = 19 ;	
	pCTRL->bH = 0 ;
	pCTRL->bV = 0 ;
	pCTRL->active_h_size = 4096 ; // 4096 is MAX H SIZE of ISP
	pCTRL->active_v_size = 2160 ;	

	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_4096x2160p20;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_4096x2160p24;
	  #endif
	#elif defined(__USE_IMX226_2160P60_MODE4__)
	pCTRL->startH = 0 ;
	pCTRL->startV = 15 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 2160 ;	

	#if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x2160p50;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x2160p60;
	  #endif	
	  
	#elif defined(__USE_IMX226_1080P60_MODE__)
	pCTRL->startH = 46 ; 
	pCTRL->startV = 13 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;	

	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p50;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p60;
	  #endif				
	#endif
		
	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 2 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1024 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 200 ;	
	pCTRL->stBLACK_LEVEL.odm_blc_val = 200 ;		
#elif defined(__USE_CMOS_IMX326__) // use sony IMX326 cmos
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	SetSS_deserializer_imx326_s6();

	pCTRL->startH = 10 ;
	pCTRL->startV = 19 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;

	#if defined(__USE_IMX326_2592x1944_MODE2__)
	pCTRL->active_h_size = 2592 ;	
	pCTRL->active_v_size = 1944 ;	

	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_2592x1944p15;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_2592x1944p15;
	  #endif	
	  
	#elif defined(__USE_IMX326_3072x1728_MODE4__)
	pCTRL->active_h_size = 3072 ;
	pCTRL->active_v_size = 1728 ;	

	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_3072x1728p30;		
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_3072x1728p30;		
	  #endif	

	#elif defined(__USE_IMX326_2160x2160_MODE6__)	  
	pCTRL->active_h_size = 2160 ;
	pCTRL->active_v_size = 2160 ;	

	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_2160x2160p30;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_2160x2160p30;
	  #endif	
	  
	#else
	pCTRL->active_h_size = 3072 ;
	pCTRL->active_v_size = 2160 ;	

	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_3072x2160p30;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_3072x2160p30;
	  #endif	
	#endif
		
	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 2 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1024 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 200 ;	
	pCTRL->stBLACK_LEVEL.odm_blc_val = 200 ;			
#elif defined(__USE_CMOS_IMX334__)
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	SetSS_deserializer_imx334_s6();

	pCTRL->startH = 10 ;
	pCTRL->startV = 23 ;	
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;

	#if defined(__USE_IMX334_2160P30_MODE__)	  
	pCTRL->active_h_size = 3840 ;
	pCTRL->active_v_size = 2160 ;	

	  #if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_3840x2160p25;
	  #else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_3840x2160p30;
	  #endif	
	#endif
		
	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 2 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1024 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 200 ;	
	pCTRL->stBLACK_LEVEL.odm_blc_val = 200 ;	
	
#elif defined(__USE_CMOS_IMX274__)
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;

	#if defined (__USE_IMX274_1080P120_MODE__)	// 1080p120
	SetSS_deserializer_imx274_s6();	
	
	pCTRL->startH = 0 ;
	pCTRL->startV = 0 ;	
	pCTRL->bH = 0 ;
	pCTRL->bV = 0 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;	

	  #if defined(__USE_25Hz_SENSOR__)
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p100;
	  #else
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p120;
	  #endif	

	#elif defined (__USE_IMX274_1080P60_MODE__)	// 1080p60
	SetSS_deserializer_imx274_s4();	
	
	pCTRL->startH = 10 ;
	pCTRL->startV = 9 ;	
	pCTRL->bH = 0 ;
	pCTRL->bV = 0 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;	

	  #if defined(__USE_25Hz_SENSOR__)
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p50;
	  #else
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p60;
	  #endif	

	#elif defined(__USE_IMX274_2160P60_MODE1__)	// 4k60
	SetSS_deserializer_imx274_s6();
	
	pCTRL->startH = 2;
	pCTRL->startV = 19 ;	

	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 2160 ;	
	
	  #if defined(__USE_25Hz_SENSOR__)
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x2160p50;
	  #else
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x2160p60;
	  #endif
	
	#else										// 4k30
	SetSS_deserializer_imx274_s6();

		#if defined(__USE_SENS_PANNING_MODE__)
			#if 1		//Since dismiss UP-scaling acticon for DIS, resolution setting 2048x1208p30
				pCTRL->startH = 10 + 896 ;
				pCTRL->startV = 19 + 476;	
				pCTRL->bH = 2 ;
				pCTRL->bV = 2 ;
				pCTRL->active_h_size = 2048 ;
				pCTRL->active_v_size = 1208 ;	

				#if defined(__USE_25Hz_SENSOR__)
					pCTRL->sensor_vout_frmt = MISP_VIDSRC_2048x1208p25;
				#else
					pCTRL->sensor_vout_frmt = MISP_VIDSRC_2048x1208p30;
				#endif
			#else
				pCTRL->startH = 10 + 960 ;
				pCTRL->startV = 19 + 540;	
				pCTRL->bH = 2 ;
				pCTRL->bV = 2 ;
				pCTRL->active_h_size = 1920 ;
				pCTRL->active_v_size = 1080 ;	

				#if defined(__USE_25Hz_SENSOR__)
					pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p25;
				#else
					pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p30;
				#endif
			#endif
		#else	
			pCTRL->startH = 10 ;
			pCTRL->startV = 19 ;	
			pCTRL->bH = 0 ;
			pCTRL->bV = 0 ;
			pCTRL->active_h_size = 3840 ;
			pCTRL->active_v_size = 2160 ;	

			#if defined(__USE_25Hz_SENSOR__)
				pCTRL->sensor_vout_frmt = MISP_VIDSRC_SS3840x2160p25;
			#else
				pCTRL->sensor_vout_frmt = MISP_VIDSRC_3840x2160p30;
			#endif	
		#endif
		
	#endif	
	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 2 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1024 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 200 ;	
	pCTRL->stBLACK_LEVEL.odm_blc_val = 200 ;		
	
#elif defined(__USE_CMOS_IMXx36__) // use sony IMX136 cmos
	
	#if defined( __USE_CMOS_IMXx36_TTL__)	
	
	SetSS_deserializer_imx136_ttl();
	
	  #if defined( __USE_IMXx36_FLIP__)					// if commit then IMX136 no hvflip
		pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_2 ;
	  #else
		pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	  #endif

	pCTRL->startH = 22 ;
	pCTRL->startV = 17 ;		
		
	#else
	
	  #if defined(__USE_CMOS_IMXx36_LVDS_S2__)
		SetSS_deserializer_imx136_s2();
	  #endif
		
	  #if defined(__USE_CMOS_IMXx36_LVDS_S4__)
		SetSS_deserializer_imx136_s4();
	  #endif	

	  #if defined( __USE_IMXx36_FLIP__)					// if commit then IMX136 no hvflip
		pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_2 ;
	  #else
		pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	  #endif
		
	pCTRL->startH = 4 + 10;
	pCTRL->startV = 17 ;		
		
	#endif

	#if defined(__USE_CMOS_IMXx36_720pReadOut__)
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1280 ;
	pCTRL->active_v_size = 720 ;

	  #if defined(__USE_25Hz_SENSOR__)
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p50;
	  #else
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p60;
	  #endif	
	
	#else
	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;

	  #if defined(__USE_25Hz_SENSOR__)
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p25;
	  #else
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p30;
	  #endif	

	#endif

	


	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 4 ;

	#if defined(__USE_CMOS_IMXx36_720pReadOut__)	
	pCTRL->stBLACK_LEVEL.ob_h_size = 1000 ;
	#else
	pCTRL->stBLACK_LEVEL.ob_h_size = 1808 ;
	#endif
	
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 240 ;
	pCTRL->stBLACK_LEVEL.odm_blc_val = 240 ;	
		
	pCTRL->stHTM_DECOMP.output_range_sel = 0 ;
	pCTRL->stHTM_DECOMP.decomp_en = 0 ; // 0 : output[11:0] 1 : output[15:4] 2 : output[19:8]
	
	pCTRL->stHTM_DECOMP.decomp_p1 = 2048 ;
	pCTRL->stHTM_DECOMP.decomp_p2 = 2944 ;
	pCTRL->stHTM_DECOMP.decomp_p3 = 3713 ;
	
	pCTRL->stHTM_DECOMP.offset_p2 = 16384 ;
	pCTRL->stHTM_DECOMP.offset_p3 = 0 ;
	
	pCTRL->stHTM_DECOMP.p1_mul_sel = 4 ; // 0: x1 1: x2 2: x4 3: x8 4: x16 5: x32 6: x64
	pCTRL->stHTM_DECOMP.p2_mul_sel = 6 ;
	pCTRL->stHTM_DECOMP.p3_mul_sel = 0 ;
	
	pCTRL->stHTM_DECOMP.bl_pre_value = 0 ;
	pCTRL->stHTM_DECOMP.bl_post_value = 240 ;
	pCTRL->stHTM_DECOMP.input_max_value = 3712 ;
	
#elif defined(__USE_CMOS_AR0331__)	// use Aptina AR0331 cmos(sensor has manual blc)

	#if defined(__USE_CMOS_AR0331_TTL__)		// use Aptina cmos
		SetSS_deserializer_ar0331_ttl();
	#endif

	#if defined(__USE_CMOS_AR0331_LVDS_S4__)
		SetSS_deserializer_ar0331_s4();
	#endif

	#if defined(__USE_CMOS_AR0331_LVDS_S2__)
		SetSS_deserializer_ar0331_s2();
	#endif
	
	#if defined (__USE_AR0331_NORMAL_3MegaMODE__)		
	
		#if defined (__USE_CMOS_AR0331_TTL__)
			pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
		#else
			pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_0 ;
		#endif
			
		pCTRL->startH = 0;
		pCTRL->startV = 3 ;

		pCTRL->bH = 0 ;
		pCTRL->bV = 0 ;
		pCTRL->active_h_size = 2048 ;
		pCTRL->active_v_size = 1536 ;
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_2048x1536p30;
		
		pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
		pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
		pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
		pCTRL->stBLACK_LEVEL.h_sync_cut_en = 1 ;	
		pCTRL->stBLACK_LEVEL.sync_mode = 0 ;	
		pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
		pCTRL->stBLACK_LEVEL.update_frequency = 0 ;	
		
		pCTRL->stBLACK_LEVEL.ob_h_pos = 0 ;
		pCTRL->stBLACK_LEVEL.ob_v_pos = 0 ;
		pCTRL->stBLACK_LEVEL.ob_h_size = 0 ;
		pCTRL->stBLACK_LEVEL.ob_v_size = 0 ;
		
		pCTRL->stBLACK_LEVEL.manual_blc_val = 168 ;
		pCTRL->stBLACK_LEVEL.odm_blc_val = 168 ;	
	#elif defined (__USE_AR0331_NORMAL_MODE__)
		pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
		
		pCTRL->startH = 5 + 10;
		pCTRL->startV = 5 ;

		pCTRL->bH = 2 ;
		pCTRL->bV = 2 ;
		pCTRL->active_h_size = 1920 ;
		pCTRL->active_v_size = 1080 ;
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p60;
		
		pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
		pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
		pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
		pCTRL->stBLACK_LEVEL.h_sync_cut_en = 1 ;	
		pCTRL->stBLACK_LEVEL.sync_mode = 0 ;	
		pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
		pCTRL->stBLACK_LEVEL.update_frequency = 0 ;	
		
		pCTRL->stBLACK_LEVEL.ob_h_pos = 0 ;
		pCTRL->stBLACK_LEVEL.ob_v_pos = 0 ;
		pCTRL->stBLACK_LEVEL.ob_h_size = 0 ;
		pCTRL->stBLACK_LEVEL.ob_v_size = 0 ;
		
		pCTRL->stBLACK_LEVEL.manual_blc_val = 168 ;
		pCTRL->stBLACK_LEVEL.odm_blc_val = 168 ;	
	#else
		pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
			
		pCTRL->startH = 5 + 10;
		pCTRL->startV = 5 ;

		pCTRL->bH = 2 ;
		pCTRL->bV = 2 ;
		pCTRL->active_h_size = 1920 ;
		pCTRL->active_v_size = 1080 ;
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p60;
		
		pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
		pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
		pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
		pCTRL->stBLACK_LEVEL.h_sync_cut_en = 1 ;	
		pCTRL->stBLACK_LEVEL.sync_mode = 0 ;	
		pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
		pCTRL->stBLACK_LEVEL.update_frequency = 0 ;	
		
		pCTRL->stBLACK_LEVEL.ob_h_pos = 0 ;
		pCTRL->stBLACK_LEVEL.ob_v_pos = 0 ;
		pCTRL->stBLACK_LEVEL.ob_h_size = 0 ;
		pCTRL->stBLACK_LEVEL.ob_v_size = 0 ;
		
		pCTRL->stBLACK_LEVEL.manual_blc_val = 0 ;
		pCTRL->stBLACK_LEVEL.odm_blc_val = 0 ;			
	#endif


	pCTRL->stHTM_DECOMP.output_range_sel = 0 ;
	pCTRL->stHTM_DECOMP.decomp_en = 0 ; // 0 : output[11:0] 1 : output[15:4] 2 : output[19:8]
	
	pCTRL->stHTM_DECOMP.decomp_p1 = 1024 ;
	pCTRL->stHTM_DECOMP.decomp_p2 = 2569 ;
	pCTRL->stHTM_DECOMP.decomp_p3 = 3456 ;
	
	pCTRL->stHTM_DECOMP.offset_p2 = 4096 ;
	pCTRL->stHTM_DECOMP.offset_p3 = 32768 ;
	
	pCTRL->stHTM_DECOMP.p1_mul_sel = 1 ; // 0: x1 1: x2 2: x4 3: x8 4: x16 5: x32 6: x64
	pCTRL->stHTM_DECOMP.p2_mul_sel = 5 ;
	pCTRL->stHTM_DECOMP.p3_mul_sel = 6 ;
	
	pCTRL->stHTM_DECOMP.bl_pre_value = 0 ;
	pCTRL->stHTM_DECOMP.bl_post_value = 166 ;
	pCTRL->stHTM_DECOMP.input_max_value = 4095 ;		
#elif defined(__USE_CMOS_AR0134__) || defined(__USE_CMOS_AR0135__)	// use Aptina AR0134&5 

#if defined(__USE_CMOS_AR0134_TTL__)
	SetSS_deserializer_ar0134_ttl();
#else
	SetSS_deserializer_ar0134_s2();
#endif

	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;

#if defined(__USE_CMOS_AR0135__)
	#if defined(__USE_CMOS_AR0134_TTL__)
	pCTRL->startH = 96 ;
	pCTRL->startV = 19 ;
	#else
	pCTRL->startH = 96 ;
	pCTRL->startV = 16 ;
	#endif
#else
	pCTRL->startH = 1 ;
	pCTRL->startV = 19 ;
#endif

	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1280 ;
	pCTRL->active_v_size = 720 ;
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p60;
	
	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 0 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 3 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 3 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 60 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 600 ;

#if defined(__USE_CMOS_AR0135__)	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 174 ;
	pCTRL->stBLACK_LEVEL.odm_blc_val = 174 ;	
#else
	pCTRL->stBLACK_LEVEL.manual_blc_val = 300 ;
	pCTRL->stBLACK_LEVEL.odm_blc_val = 300 ;			
#endif

	pCTRL->stHTM_DECOMP.output_range_sel = 0 ;
	pCTRL->stHTM_DECOMP.decomp_en = 0 ; // 0 : output[11:0] 1 : output[15:4] 2 : output[19:8]
	
	pCTRL->stHTM_DECOMP.decomp_p1 = 1024 ;
	pCTRL->stHTM_DECOMP.decomp_p2 = 2569 ;
	pCTRL->stHTM_DECOMP.decomp_p3 = 3456 ;
	
	pCTRL->stHTM_DECOMP.offset_p2 = 4096 ;
	pCTRL->stHTM_DECOMP.offset_p3 = 32768 ;
	
	pCTRL->stHTM_DECOMP.p1_mul_sel = 1 ; // 0: x1 1: x2 2: x4 3: x8 4: x16 5: x32 6: x64
	pCTRL->stHTM_DECOMP.p2_mul_sel = 5 ;
	pCTRL->stHTM_DECOMP.p3_mul_sel = 6 ;
	
	pCTRL->stHTM_DECOMP.bl_pre_value = 0 ;
	pCTRL->stHTM_DECOMP.bl_post_value = 166 ;
	pCTRL->stHTM_DECOMP.input_max_value = 4095 ;		
		
#elif defined(__USE_CMOS_MT9P031__)	// use 5M cmos (FHD ttl mode, 5M mode)

	pCTRL->sensor_input_type = MISP_SENSOR_TTL_INPUT ;
	pCTRL->sensor_mode = 0 ;			

	pCTRL->stDESERIALIZER.user_SOF = 0 ;
	pCTRL->stDESERIALIZER.user_SOL = 0 ;
	pCTRL->stDESERIALIZER.user_EOF = 0 ;
	pCTRL->stDESERIALIZER.user_EOL = 0 ;	
	
	pCTRL->stDESERIALIZER.ref_h_size = 0 ;
	pCTRL->stDESERIALIZER.ref_v_size = 0 ;		

	pCTRL->stDESERIALIZER.ss_slave_en = 0 ;
	pCTRL->stDESERIALIZER.ss_slave_H = 0 ;
	pCTRL->stDESERIALIZER.ss_slave_V = 0 ;
	
	pCTRL->stDESERIALIZER.hd_edge_polarity = 0 ;
	pCTRL->stDESERIALIZER.vd_edge_polarity = 0 ;
	pCTRL->stDESERIALIZER.hd_delay = 0 ;
	pCTRL->stDESERIALIZER.hd_high_width = 0 ;
	pCTRL->stDESERIALIZER.vd_high_width = 0 ;
	
	pCTRL->stDESERIALIZER.data_lch_shift = 0 ;
	pCTRL->stDESERIALIZER.data_lbit_shift = 0 ;
	pCTRL->stDESERIALIZER.wdr_info_en = 0 ;
	pCTRL->stDESERIALIZER.wdr_info_bit_sel = 0 ;

	#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	pCTRL->stDESERIALIZER.wdr_info_sel = 0;
	#else 												// MDIN-i51X (i510, i540, i550)
	pCTRL->stDESERIALIZER.dol_wdr_enb = 0 ;
	pCTRL->stDESERIALIZER.dol_f_mode = 0 ;
	pCTRL->stDESERIALIZER.ext_hactive = 0;
	#endif

	pCTRL->stDESERIALIZER.error_clear_flag = 0 ;	
	
	#if defined (__USE_CMOS_MT9P031_1080p__)
		pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	//FFD ttl mode. use sensor's black level, (black level only make h,v sync overflow cutting)
		pCTRL->startH = 0 + 14;
		pCTRL->startV = 9 ;

		pCTRL->bH = 2 ;
		pCTRL->bV = 2 ;
		pCTRL->active_h_size = 1920 ;
		pCTRL->active_v_size = 1080 ;
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p60;
	#endif
	#if defined(__USE_CMOS_MT9P031_5M_PIXEL__)	// use 9p031 5M cmos mode
		pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
		pCTRL->startH = 0 + 14;
		pCTRL->startV = 1 ;

		pCTRL->bH = 0 ;
		pCTRL->bV = 0 ;
		pCTRL->active_h_size = 2592 ;
		pCTRL->active_v_size = 1536 ;	
		pCTRL->sensor_vout_frmt = MISP_VIDSRC_2592x1536p15;
	#endif

	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 0 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 4 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 4 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 4 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 1080 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 256 ;		
	pCTRL->stBLACK_LEVEL.odm_blc_val = 256 ;		

#elif defined(__USE_CMOS_EXT2__)
	
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	SetSS_deserializer_EXT2_ttl();

	pCTRL->startH = 19 ;
	pCTRL->startV = 63 ;			
	pCTRL->bH = 2;
	pCTRL->bV = 2;
	pCTRL->active_h_size = 1280 ;
	pCTRL->active_v_size = 720 ;

	#if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p25;
	#else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p30;
	#endif
	

	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	

	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 4 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1000 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;

	pCTRL->stBLACK_LEVEL.manual_blc_val = 43 ;
	pCTRL->stBLACK_LEVEL.odm_blc_val = 43 ;		
		
	pCTRL->stHTM_DECOMP.output_range_sel = 0 ;
	pCTRL->stHTM_DECOMP.decomp_en = 0 ; // 0 : output[11:0] 1 : output[15:4] 2 : output[19:8]
	
	pCTRL->stHTM_DECOMP.decomp_p1 = 2048 ;
	pCTRL->stHTM_DECOMP.decomp_p2 = 2944 ;
	pCTRL->stHTM_DECOMP.decomp_p3 = 3713 ;
	
	pCTRL->stHTM_DECOMP.offset_p2 = 16384 ;
	pCTRL->stHTM_DECOMP.offset_p3 = 0 ;
	
	pCTRL->stHTM_DECOMP.p1_mul_sel = 4 ; // 0: x1 1: x2 2: x4 3: x8 4: x16 5: x32 6: x64
	pCTRL->stHTM_DECOMP.p2_mul_sel = 6 ;
	pCTRL->stHTM_DECOMP.p3_mul_sel = 0 ;
	
	pCTRL->stHTM_DECOMP.bl_pre_value = 0 ;
	pCTRL->stHTM_DECOMP.bl_post_value = 60 ;
	pCTRL->stHTM_DECOMP.input_max_value = 3712 ;

#elif defined(__USE_CMOS_EXT1__)
	
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	SetSS_deserializer_EXT1_ttl();

	#if defined(__USE_EXT1_FLIP__)
	pCTRL->startH = 18 ;
	pCTRL->startV = 22;	
	#else
	pCTRL->startH = 19 ;
	pCTRL->startV = 87 ;	
	#endif
		

	pCTRL->bH = 2;
	pCTRL->bV = 2;
	pCTRL->active_h_size = 1280 ;
	pCTRL->active_v_size = 720 ;

	#if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p25;
	#else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1280x720p30;
	#endif
	

	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 1 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 1 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	

	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 4 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1000 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;

//	pCTRL->stBLACK_LEVEL.manual_blc_val = 28;
//	pCTRL->stBLACK_LEVEL.odm_blc_val = 28;

	pCTRL->stBLACK_LEVEL.manual_blc_val = 40;
	pCTRL->stBLACK_LEVEL.odm_blc_val = 40;
		
	pCTRL->stHTM_DECOMP.output_range_sel = 0 ;
	pCTRL->stHTM_DECOMP.decomp_en = 0 ; // 0 : output[11:0] 1 : output[15:4] 2 : output[19:8]
	
	pCTRL->stHTM_DECOMP.decomp_p1 = 2048 ;
	pCTRL->stHTM_DECOMP.decomp_p2 = 2944 ;
	pCTRL->stHTM_DECOMP.decomp_p3 = 3713 ;
	
	pCTRL->stHTM_DECOMP.offset_p2 = 16384 ;
	pCTRL->stHTM_DECOMP.offset_p3 = 0 ;
	
	pCTRL->stHTM_DECOMP.p1_mul_sel = 4 ; // 0: x1 1: x2 2: x4 3: x8 4: x16 5: x32 6: x64
	pCTRL->stHTM_DECOMP.p2_mul_sel = 6 ;
	pCTRL->stHTM_DECOMP.p3_mul_sel = 0 ;
	
	pCTRL->stHTM_DECOMP.bl_pre_value = 0 ;
	pCTRL->stHTM_DECOMP.bl_post_value = 60 ;
	pCTRL->stHTM_DECOMP.input_max_value = 3712 ;	

#elif defined(__USE_CMOS_OV2710__)
	
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	SetSS_deserializer_ov2710_ttl();

	pCTRL->startH = 1 ;
	pCTRL->startV = 0 ;			
	pCTRL->bH = 2;
	pCTRL->bV = 2;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;

	#if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p25;
	#else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p30;
	#endif
	
	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	

	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 4 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1808 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	pCTRL->stBLACK_LEVEL.manual_blc_val = 75;
		
	pCTRL->stHTM_DECOMP.output_range_sel = 0 ;
	pCTRL->stHTM_DECOMP.decomp_en = 0 ; // 0 : output[11:0] 1 : output[15:4] 2 : output[19:8]
	
	pCTRL->stHTM_DECOMP.decomp_p1 = 2048 ;
	pCTRL->stHTM_DECOMP.decomp_p2 = 2944 ;
	pCTRL->stHTM_DECOMP.decomp_p3 = 3713 ;
	
	pCTRL->stHTM_DECOMP.offset_p2 = 16384 ;
	pCTRL->stHTM_DECOMP.offset_p3 = 0 ;
	
	pCTRL->stHTM_DECOMP.p1_mul_sel = 4 ; // 0: x1 1: x2 2: x4 3: x8 4: x16 5: x32 6: x64
	pCTRL->stHTM_DECOMP.p2_mul_sel = 6 ;
	pCTRL->stHTM_DECOMP.p3_mul_sel = 0 ;
	
	pCTRL->stHTM_DECOMP.bl_pre_value = 0 ;
	pCTRL->stHTM_DECOMP.bl_post_value = 60 ;
	pCTRL->stHTM_DECOMP.input_max_value = 3712 ;
#elif defined(__USE_CMOS_EXT3__)
	
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	SetSS_deserializer_ext3_ttl();

	pCTRL->startH = 2 ;
	pCTRL->startV = 1 ;			
	pCTRL->bH = 2;
	pCTRL->bV = 2;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;

	#if defined(__USE_25Hz_SENSOR__)
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p25;
	#else
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p30;
	#endif
	
	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	

	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 4 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1808 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	pCTRL->stBLACK_LEVEL.manual_blc_val = 75;
		
	pCTRL->stHTM_DECOMP.output_range_sel = 0 ;
	pCTRL->stHTM_DECOMP.decomp_en = 0 ; // 0 : output[11:0] 1 : output[15:4] 2 : output[19:8]
	
	pCTRL->stHTM_DECOMP.decomp_p1 = 2048 ;
	pCTRL->stHTM_DECOMP.decomp_p2 = 2944 ;
	pCTRL->stHTM_DECOMP.decomp_p3 = 3713 ;
	
	pCTRL->stHTM_DECOMP.offset_p2 = 16384 ;
	pCTRL->stHTM_DECOMP.offset_p3 = 0 ;
	
	pCTRL->stHTM_DECOMP.p1_mul_sel = 4 ; // 0: x1 1: x2 2: x4 3: x8 4: x16 5: x32 6: x64
	pCTRL->stHTM_DECOMP.p2_mul_sel = 6 ;
	pCTRL->stHTM_DECOMP.p3_mul_sel = 0 ;
	
	pCTRL->stHTM_DECOMP.bl_pre_value = 0 ;
	pCTRL->stHTM_DECOMP.bl_post_value = 60 ;
	pCTRL->stHTM_DECOMP.input_max_value = 3712 ;
				
#else
	
	pCTRL->bayer_data_order = MISP_SS_BAYER_PATTERN_1 ;
	
	pCTRL->startH = 67 ;
	pCTRL->startV = 11 ;

	pCTRL->bH = 2 ;
	pCTRL->bV = 2 ;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p60;
	
	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 2 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 3 ;	
	
	pCTRL->stBLACK_LEVEL.ob_h_pos = 8 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 8 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 20 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 1080 ;
	
	pCTRL->stBLACK_LEVEL.manual_blc_val = 256 ;
	pCTRL->stBLACK_LEVEL.odm_blc_val = 256 ;		
#endif

#if defined(__USE_4K60_MD1__)
	pCTRL->stBLACK_LEVEL.Init_OBCheck_OK=0;
#endif


#if defined(__USE_WDR_PROCESS__)
{
	//table for wdr hsize
	typedef enum { HRES_2048, HRES_1920, HRES_1440, HRES_1280, HRES_NUM } HRES_t;
	typedef enum { WBIT_8bit, WBIT_9bit, WBIT_10bit, WBIT_12bit, WBIT_NUM } WBIT_t;

	//split h-size should be divided by pixel count in a packet without a remainder
	const WORD wdr_hsize[HRES_NUM][WBIT_NUM]={
	//  {  8b,   9b,  10b,  12b},
		{2048, 2072, 2064, 2080},  //2048x1536 (8-split)
		{1952, 1932, 1944, 1940},  //1920x1080 (4-split)
		{1472, 1456, 1464, 1460},  //1440x1080 (4-split)
		{1312, 1288, 1296, 1300}   //1280x720  (4-split)
	};

	HRES_t hres_id;
	WBIT_t wbit_id;

	//normal h-active size
	switch(pCTRL->active_h_size) {
		case 2048:  hres_id=HRES_2048;  break;
		case 1920:  hres_id=HRES_1920;  break;
		case 1440:  hres_id=HRES_1440;  break;
		case 1280:  hres_id=HRES_1280;  break;
		default:    hres_id=HRES_1920;  break;
	}

	//write bit mode
	switch(GetWDR_RwBitMode()) {
		case MISP_DDRMAP_8b:   wbit_id=WBIT_8bit;   break;
		case MISP_DDRMAP_9b:   wbit_id=WBIT_9bit;   break;
		case MISP_DDRMAP_10b:  wbit_id=WBIT_10bit;  break;
		default:               wbit_id=WBIT_12bit;  break;
	}

	//extend h-active size to support WDR
	ext_hsize = (wdr_hsize[hres_id][wbit_id]-(pCTRL->active_h_size))/2;
	ext_hsize = (ext_hsize>pCTRL->bH)?(ext_hsize-pCTRL->bH):0;
	
	// UARTprintf("  - Pre WDR adjust [H:%d][V:%d]\r\n", pCTRL->startH, pCTRL->startV);
	
	//validate size
	//if(pCTRL->startH < ext_hsize)  //default offset is too small
	//if(pCTRL->startH+pCTRL->active_h_size+2*pCTRL->bH < SENSOR_FORMAT_SIZE)  //out of range
	if(pCTRL->startH < ext_hsize) {
		UARTprintf("### !!! WDR configure error !!! \r\n");
		UARTprintf("### startH(%d) must bigger than ext_hsize(%d) !!\r\n",
			pCTRL->startH, ext_hsize) ;
		UARTprintf("  - wdr hsize : %d\r\n", wdr_hsize[hres_id][wbit_id]);
		UARTprintf("  - act hsize : %d\r\n", pCTRL->active_h_size);
		UARTprintf("  - dem bound : %d\r\n", pCTRL->bH);
		ext_hsize=0;
	}
	else {
		pCTRL->startH -= ext_hsize ;
		pCTRL->bH     += ext_hsize ;
	}
	// UARTprintf("  - Post WDR adjust [H:%d][V:%d]\r\n", pCTRL->startH, pCTRL->startV);
}
#endif

	pCTRL->prev_startH = pCTRL->startH;
	pCTRL->prev_startV = pCTRL->startV;
	pCTRL->init_startH = pCTRL->startH;
	pCTRL->init_startV = pCTRL->startV;	

	
#if defined(__USE_8BIT_OPERATION__)		// use 8bit pixel operation : basic	
	pCTRL->sensor_bit_operation = MISP_8BIT_OPERATION ;
#else
	pCTRL->sensor_bit_operation = MISP_10BIT_OPERATION ;
#endif 

	pCTRL->stWDR_MEM_WRITE.proc_mode = MISP_WDR_MEM_WIRTE_OFF_MODE ;
	pCTRL->stWDR_MEM_WRITE.l_bit_mode = 2 ; // "0" 8bit, "1" 10bit, "2" 9bit, "3" 12bit
	pCTRL->stWDR_MEM_WRITE.s1_bit_mode = 2 ; // "0" 8bit, "1" 10bit, "2" 9bit, "3" 12bit
	pCTRL->stWDR_MEM_WRITE.s2_bit_mode = 2 ; // "0" 8bit, "1" 10bit, "2" 9bit, "3" 12bit
	pCTRL->stWDR_MEM_WRITE.s3_bit_mode = 2 ; // "0" 8bit, "1" 10bit, "2" 9bit, "3" 12bit
	pCTRL->stWDR_MEM_WRITE.proc_frame_en = 0x00 ;
	pCTRL->stWDR_MEM_WRITE.wt_bit_scope = 1;  // "0" map[9:n], "1" map[11:n]
	pCTRL->stWDR_MEM_WRITE.odm_sync_sel = 0;	// 0: long, 1: s1, 2:s2, 3:s3

	// pCTRL->stDEAD_PIX_DETECT.dpd_en = 0 ;
	pCTRL->stBINNING.binning_en = 0 ;		


#if defined(__USE_CMOS_MN34220__)
	pCTRL->stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_2 ; 
#elif defined(__USE_CMOS_MN34420__)
	pCTRL->stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_2 ; 	
#elif defined(__USE_CMOS_MN34041__)
	pCTRL->stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_2 ; 	
#elif defined(__USE_CMOS_IMXx36__)
	pCTRL->stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_3 ; 
#elif defined(__USE_CMOS_IMX124__)
	pCTRL->stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_1 ; 
#elif defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__)
	pCTRL->stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_3 ; 
#elif defined(__USE_CMOS_IMX174__) || defined(__USE_CMOS_IMX249__)
	pCTRL->stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_2 ; 	//for slave mode
//	pCTRL->stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_0 ; 
#else
	switch (pCTRL->bayer_data_order) {
		case MISP_SS_BAYER_PATTERN_0 : pCTRL->stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_0 ; break ;
		case MISP_SS_BAYER_PATTERN_1 : pCTRL->stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_1 ; break ;
		case MISP_SS_BAYER_PATTERN_2 : pCTRL->stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_2 ; break ;
		case MISP_SS_BAYER_PATTERN_3 : pCTRL->stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_3 ; break ;
		default : pCTRL->stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_1 ; break ;	
	}
#endif
	
	pCTRL->stWDR_MEM_WRITE.in_hsize = pCTRL->active_h_size + (pCTRL->bH<<1) ;
	pCTRL->stWDR_MEM_WRITE.in_vsize = pCTRL->active_v_size + (pCTRL->bV<<1) ;		

#if __SS_SI_SETUP__	
	pCTRL->stSYNC_INTEGRITY.si_en = 0 ;
	pCTRL->stSYNC_INTEGRITY.si_sync_gen_en = 0 ;
	pCTRL->stSYNC_INTEGRITY.si_sync_lock = 0 ;
	pCTRL->stSYNC_INTEGRITY.si_sync_htotal = 4368 ;
	pCTRL->stSYNC_INTEGRITY.si_sync_vtotal = 2200 ;
	
	pCTRL->stSYNC_INTEGRITY.si_sync_hrise = 0 ;
	pCTRL->stSYNC_INTEGRITY.si_sync_vrise = 0 ;

	pCTRL->stSYNC_INTEGRITY.si_sync_hfall = 4096+214 ;
	pCTRL->stSYNC_INTEGRITY.si_sync_vfall = 2160+22 ;	
#endif
	
	SetSS_stp_imbue();

	
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	#if defined(__MDIN_i510_EDK_V10__) || defined(__MDIN_i510_REF_V10__) || \
		defined(__MDIN_i540_EDK_V10__) || defined(__MDIN_i540_REF_V10__) || \
		defined(__MDIN_i550_EDK_V10__) || defined(__MDIN_i550_REF_V10__)	// edk,ref v1.0
		
		#if defined(__USE_CMOS_MN34220__) || defined(__USE_CMOS_MN34420__)  // for line-WDR, set all channel delay as 2
			#if defined(__PANORAMA_SENSOR_SETUP__)  // panorama one board 
				pCTRL->lvds_in_inv = 0;
				pCTRL->lvds_in_delay = 0;
			#elif defined(__USE_MDIN_i510__)  // MDIN_i510REF
				pCTRL->lvds_in_inv = 1;
				pCTRL->lvds_in_delay = 0;
			#else
				pCTRL->lvds_in_inv = 1;
				pCTRL->lvds_in_delay = 2;
			#endif
		#else
			pCTRL->lvds_in_inv = 1;
			pCTRL->lvds_in_delay = 0;
		#endif
	#else																// edk,ref v1.1
		pCTRL->lvds_in_inv = 0;
		pCTRL->lvds_in_delay = 0;
	#endif
#endif



#if defined(__USE_EXT2IPP__)
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 2;

	pCTRL->startH = 0 ;
	pCTRL->startV = 0 ;			
	pCTRL->bH = 0;
	pCTRL->bV = 0;
	pCTRL->active_h_size = 1920 ;
	pCTRL->active_v_size = 1080 ;
	pCTRL->sensor_vout_frmt = MISP_VIDSRC_1920x1080p60;
	
	pCTRL->stBLACK_LEVEL.blc_enable = 1 ;
	pCTRL->stBLACK_LEVEL.mode = 0 ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	pCTRL->stBLACK_LEVEL.type = 0 ; // 0: use front blc / 1: use odm blc [4B0h]
	pCTRL->stBLACK_LEVEL.h_sync_cut_en = 0 ;	
	pCTRL->stBLACK_LEVEL.sync_mode = 1 ;	
	pCTRL->stBLACK_LEVEL.sync_bypass_n = 1 ;		
	pCTRL->stBLACK_LEVEL.update_frequency = 4 ;	

	pCTRL->stBLACK_LEVEL.ob_h_pos = 32 ;
	pCTRL->stBLACK_LEVEL.ob_v_pos = 4 ;
	pCTRL->stBLACK_LEVEL.ob_h_size = 1808 ;
	pCTRL->stBLACK_LEVEL.ob_v_size = 8 ;
	pCTRL->stBLACK_LEVEL.manual_blc_val = 0;
#endif


}
//--------------------------------------------------------------------------------------------------------------------------
static void Init_SS_REGISTER( PSS_CTRL_PARAM pCTRL ) {
	
	WORD in_hsize ;
	WORD in_vsize ;
	WORD col_per_line ;
	WORD count_value ;
	WORD count_en ;
	WORD divide_factor ;

	in_hsize = pCTRL->active_h_size + (pCTRL->bH<<1) ;
	in_vsize = pCTRL->active_v_size + (pCTRL->bV<<1) ;
	//--------------------------------------------------------------------------------------------------------------------------
	
	SSCTRL_DeserializerHandler(pCTRL);

	// hongsi 2016.10.07
	stSSCTRL.stDESERIALIZER.fifo_rd_delay = 1;
	SetSS_reset_des_fifo();
	
	SSCTRL_SetLvdsInDelay(pCTRL);	// lvds input(invert, delay)

	//--------------------------------------------------------------------------------------------------------------------------
	MISP_RegWrite(MISP_LOCAL_ID, 0x4A0, pCTRL->startH);	// active_hstart			
	MISP_RegWrite(MISP_LOCAL_ID, 0x4A1, pCTRL->startV);	// active_vstart	
	MISP_RegWrite(MISP_LOCAL_ID, 0x4A2, in_hsize);	// active_hsize
	MISP_RegWrite(MISP_LOCAL_ID, 0x4A3, in_vsize);	// active_vsize	

	MISP_RegWrite(MISP_LOCAL_ID, 0x4A4, pCTRL->stBLACK_LEVEL.ob_h_pos);	// blc_h_ob_h_pos		
	MISP_RegWrite(MISP_LOCAL_ID, 0x4A5, pCTRL->stBLACK_LEVEL.ob_v_pos);	// blc_h_ob_v_pos			
	MISP_RegWrite(MISP_LOCAL_ID, 0x4A6, pCTRL->stBLACK_LEVEL.ob_h_size);	// blc_h_ob_h			
	MISP_RegWrite(MISP_LOCAL_ID, 0x4A7, pCTRL->stBLACK_LEVEL.ob_v_size);	// blc_h_ob_v			
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x4A8, (pCTRL->stBLACK_LEVEL.blc_enable<<15)|(pCTRL->stBLACK_LEVEL.mode<<13)|
													(pCTRL->stBLACK_LEVEL.h_sync_cut_en<<11)|(pCTRL->stBLACK_LEVEL.sync_mode<<9)|
													(pCTRL->stBLACK_LEVEL.sync_bypass_n<<8)|(pCTRL->stBLACK_LEVEL.blc_hactend_robust<<6)|(pCTRL->stBLACK_LEVEL.update_frequency));	// bayer bl enable

	MISP_RegWrite(MISP_LOCAL_ID, 0x4A9, pCTRL->stBLACK_LEVEL.manual_blc_val);	// bayer bl target(manual blc value)
	MISP_RegWrite(MISP_LOCAL_ID, 0x4B0, (pCTRL->stBLACK_LEVEL.odm_blc_val<<4)|(pCTRL->stBLACK_LEVEL.type<<2)|(pCTRL->stWDR_MEM_WRITE.odm_sync_sel));

	MISP_RegWrite(MISP_LOCAL_ID, 0x4AA, 0x0000);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x4AB, 0x2F2F);	// bl_valid_rise&fall_offset 
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x4B1, pCTRL->stBINNING.binning_en);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x4B2, 0x2040);
	
	// MISP_RegWrite(MISP_LOCAL_ID, 0x4B3, pCTRL->stDEAD_PIX_DETECT.dpd_en);
	MISP_RegWrite(MISP_LOCAL_ID, 0x4B3, 0x0000);
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x57A, 0x0000);	

	switch (pCTRL->stWDR_MEM_WRITE.l_bit_mode){
		case 0 : divide_factor = 8 ; break ;	// 8b
		case 1 : divide_factor = 6 ; break ;	// 10b
		case 2 : divide_factor = 7 ; break ;	// 9b
		case 3 : divide_factor = 5 ; break ;	// 12b
		default : divide_factor = 7 ; break ;	// 9b
	}
	
	if ( (in_hsize)%divide_factor == 0 ){
		col_per_line = (in_hsize)/divide_factor ;		
	}
	else {
		col_per_line = (in_hsize)/divide_factor + 1 ;
	}

	if( col_per_line%32 ) {
		count_value = col_per_line%32 -1 ;
		count_en = 1;
	}
	else {
		count_value = 31 ;
		count_en = 0;
	}

	MISP_RegWrite(MISP_LOCAL_ID, 0x580, 
		(pCTRL->stWDR_MEM_WRITE.l_bit_mode<<14)|(pCTRL->stWDR_MEM_WRITE.s3_bit_mode<<12)|
		(pCTRL->stWDR_MEM_WRITE.s2_bit_mode<<10)|(pCTRL->stWDR_MEM_WRITE.s1_bit_mode<<8)|
		(pCTRL->stWDR_MEM_WRITE.proc_mode<<6)|count_value<<1|count_en);			

	MISP_RegField(MISP_LOCAL_ID, 0x581, 4,1, pCTRL->stWDR_MEM_WRITE.wt_bit_scope);
	MISP_RegField(MISP_LOCAL_ID, 0x581, 0,4, pCTRL->stWDR_MEM_WRITE.proc_frame_en);

	MISP_RegWrite(MISP_LOCAL_ID, 0x582, pCTRL->stWDR_MEM_WRITE.in_hsize);	// bayer wdr wt h
	MISP_RegWrite(MISP_LOCAL_ID, 0x583, pCTRL->stWDR_MEM_WRITE.in_vsize);	// bayer wdr wt v	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x584, 0x0000);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x585, 0x0000);		

	MISP_RegWrite(MISP_LOCAL_ID, 0x220, (pCTRL->stHTM_DECOMP.output_range_sel<<1)|(pCTRL->stHTM_DECOMP.decomp_en));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x221, (pCTRL->stHTM_DECOMP.decomp_p1));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x222, (pCTRL->stHTM_DECOMP.decomp_p2));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x223, (pCTRL->stHTM_DECOMP.offset_p2));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x224, (pCTRL->stHTM_DECOMP.decomp_p3));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x225, (pCTRL->stHTM_DECOMP.offset_p3));	
	
#if	defined(__USE_MDIN_i500__)			// MDIN-i500
	MISP_RegWrite(MISP_LOCAL_ID, 0x226, (pCTRL->stHTM_DECOMP.p3_mul_sel<<6)|(pCTRL->stHTM_DECOMP.p2_mul_sel<<3)|(pCTRL->stHTM_DECOMP.p1_mul_sel));	
#else									// MDIN-i51X (i510, i540, i550)
	MISP_RegWrite(MISP_LOCAL_ID, 0x226, (pCTRL->stHTM_DECOMP.p3_mul_sel<<8)|(pCTRL->stHTM_DECOMP.p2_mul_sel<<4)|(pCTRL->stHTM_DECOMP.p1_mul_sel));	
#endif
	MISP_RegWrite(MISP_LOCAL_ID, 0x227, (pCTRL->stHTM_DECOMP.bl_pre_value));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x228, (pCTRL->stHTM_DECOMP.bl_post_value));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x229, (pCTRL->stHTM_DECOMP.input_max_value));		
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x230, pCTRL->stTESTPATTERN.tp_hsize);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x231, pCTRL->stTESTPATTERN.tp_vsize);		
	MISP_RegWrite(MISP_LOCAL_ID, 0x232, pCTRL->stTESTPATTERN.tp_hstart);		
	MISP_RegWrite(MISP_LOCAL_ID, 0x233, pCTRL->stTESTPATTERN.tp_vstart);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x234, (pCTRL->stTESTPATTERN.tp_pix_id<<1));

#if __SS_SI_SETUP__		
	pCTRL->ctrl|= SSCTRL_SI_UPDATE ;
	SSCTRL_SyncIntegrityHandler(pCTRL);
#endif	
}
//--------------------------------------------------------------------------------------------------------------------------
void Init_SensorTop(void) {	
	memset(&stSSCTRL, 0, sizeof(SS_CTRL_PARAM));
	Init_SS_PARAM(&stSSCTRL);
	Init_SS_REGISTER(&stSSCTRL);
	stSSCTRL.ctrl = 0 ;
}

//--------------------------------------------------------------------------------------------------------------------------

static void SSCTRL_WDRMemWriteHandler(PSS_CTRL_PARAM pCTRL)
{
	WORD in_hsize ;
	WORD in_vsize ;
	WORD col_per_line ;
	WORD count_value ;	
	WORD count_en ;
	WORD divide_factor ;
	
	if ((pCTRL->ctrl&SSCTRL_WDR_MEM_WRITE_UPDATE)==0) return;
	pCTRL->ctrl &= (~SSCTRL_WDR_MEM_WRITE_UPDATE);	

	// in_hsize = pCTRL->active_h_size + (pCTRL->bH<<1) ;

	in_hsize = pCTRL->stWDR_MEM_WRITE.in_hsize ;
	in_vsize = pCTRL->stWDR_MEM_WRITE.in_vsize ;		
	
	switch (pCTRL->stWDR_MEM_WRITE.l_bit_mode){
		case 0 : divide_factor = 8 ; break ;	// 8b
		case 1 : divide_factor = 6 ; break ;	// 10b
		case 2 : divide_factor = 7 ; break ;	// 9b
		case 3 : divide_factor = 5 ; break ;	// 12b
		default : divide_factor = 7 ; break ;	// 9b
	}
	
	if ( (in_hsize)%divide_factor == 0 ){
		col_per_line = (in_hsize)/divide_factor ;		
	}
	else {
		col_per_line = (in_hsize)/divide_factor + 1 ;
	}

	if( col_per_line%32 ) {
		count_value = col_per_line%32 -1 ;
		count_en = 1;
	}
	else {
		count_value = 31 ;
		count_en = 0;
	}
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x580, 
		(pCTRL->stWDR_MEM_WRITE.l_bit_mode<<14)|(pCTRL->stWDR_MEM_WRITE.s3_bit_mode<<12)|
		(pCTRL->stWDR_MEM_WRITE.s2_bit_mode<<10)|(pCTRL->stWDR_MEM_WRITE.s1_bit_mode<<8)|
		(pCTRL->stWDR_MEM_WRITE.proc_mode<<6)|count_value<<1|count_en);		

	MISP_RegField(MISP_LOCAL_ID, 0x581, 4,1, pCTRL->stWDR_MEM_WRITE.wt_bit_scope);
	MISP_RegField(MISP_LOCAL_ID, 0x581, 0,4, pCTRL->stWDR_MEM_WRITE.proc_frame_en);

	MISP_RegWrite(MISP_LOCAL_ID, 0x582, pCTRL->stWDR_MEM_WRITE.in_hsize);	// bayer wdr wt h
	MISP_RegWrite(MISP_LOCAL_ID, 0x583, pCTRL->stWDR_MEM_WRITE.in_vsize);	// bayer wdr wt v	
}

//--------------------------------------------------------------------------------------------------------------------------
static void SSCTRL_DeserializerHandler(PSS_CTRL_PARAM pCTRL)
{
	const BYTE irq_en=1;  //invokes MISP_IRQ_WDR_INTERRUPT. register 5C0h[15] should be '0'.

	if ( (pCTRL->ctrl&SSCTRL_DESERIALIZER_UPDATE) == 0 ) return ;
	pCTRL->ctrl &= (~SSCTRL_DESERIALIZER_UPDATE);	

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	MISP_RegWrite(MISP_LOCAL_ID, 0x560, 0x0400|(pCTRL->sensor_mode)<<1|(pCTRL->sensor_input_type));	// 5:2 sensor_in_mode
#else 												// MDIN-i51X (i510, i540, i550)
	MISP_RegWrite(MISP_LOCAL_ID, 0x560, 0x0C00|(pCTRL->sensor_mode)<<1|(pCTRL->sensor_input_type));	// 5:2 sensor_in_mode
#endif	
	
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	MISP_RegWrite(MISP_LOCAL_ID, 0x561, 0x000c);	// sensor_map_mode
	MISP_RegWrite(MISP_LOCAL_ID, 0x562, pCTRL->stDESERIALIZER.user_SOF);	// sync code
#else 												// MDIN-i51X (i510, i540, i550)
//	const BYTE irq_en=1;  //invokes MISP_IRQ_WDR_INTERRUPT. register 5C0h[15] should be '0'.
	MISP_RegWrite(MISP_LOCAL_ID, 0x561, (irq_en<<13| pCTRL->stDESERIALIZER.sensor_bayer_10bit<<11|(pCTRL->stDESERIALIZER.sensor_sel_fpga)<<9)| (pCTRL->stDESERIALIZER.des_invert_sync)<<8| (pCTRL->stDESERIALIZER.sel_ch_port)<<5| (pCTRL->stDESERIALIZER.sensor_mapmode)<<2);
	MISP_RegWrite(MISP_LOCAL_ID, 0x594, (pCTRL->stDESERIALIZER.des_mux_ch4_sel)<<12|(pCTRL->stDESERIALIZER.des_mux_ch3_sel)<<9| (pCTRL->stDESERIALIZER.des_mux_ch2_sel)<<6| 
											(pCTRL->stDESERIALIZER.des_mux_ch1_sel)<<3| (pCTRL->stDESERIALIZER.des_mux_ch0_sel));
	MISP_RegWrite(MISP_LOCAL_ID, 0x595, (pCTRL->stDESERIALIZER.des_mux_ch7_sel)<<6| (pCTRL->stDESERIALIZER.des_mux_ch6_sel)<<3| (pCTRL->stDESERIALIZER.des_mux_ch5_sel));
	MISP_RegWrite(MISP_LOCAL_ID, 0x562, (pCTRL->stDESERIALIZER.sync_code_type)<<12 | pCTRL->stDESERIALIZER.user_SOF);	// sync code
#endif
	MISP_RegWrite(MISP_LOCAL_ID, 0x563,  (pCTRL->stDESERIALIZER.fifo_ctrl)<<12 |pCTRL->stDESERIALIZER.user_SOL);	// sync code
	MISP_RegWrite(MISP_LOCAL_ID, 0x564, pCTRL->stDESERIALIZER.user_EOF);	// sync code
	MISP_RegWrite(MISP_LOCAL_ID, 0x565, pCTRL->stDESERIALIZER.user_EOL);	// sync code
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x566, pCTRL->stDESERIALIZER.ref_h_size);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x567, (pCTRL->stDESERIALIZER.error_clear_flag)<<15|(pCTRL->stDESERIALIZER.ref_v_size));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x591, pCTRL->stDESERIALIZER.ref_h_blank_size);
	
	SetSS_deserializer_slave_PAL(GetSS_SensorVoutPAL()) ;
		
	// MISP_RegWrite(MISP_LOCAL_ID, 0x56A, pCTRL->stDESERIALIZER.ss_slave_H);	// h_cycle
	// MISP_RegWrite(MISP_LOCAL_ID, 0x56B, pCTRL->stDESERIALIZER.ss_slave_V);	// V_cycle
	
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	MISP_RegWrite(MISP_LOCAL_ID, 0x56C, (pCTRL->stDESERIALIZER.ss_slave_en)<<15|(pCTRL->stDESERIALIZER.vd_edge_polarity)<<13|(pCTRL->stDESERIALIZER.hd_edge_polarity)<<12|(pCTRL->stDESERIALIZER.hd_delay));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x56D, (pCTRL->stDESERIALIZER.vd_high_width)<<12|(pCTRL->stDESERIALIZER.hd_high_width));	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x56E, (pCTRL->stDESERIALIZER.wdr_info_en)<<8|(pCTRL->stDESERIALIZER.wdr_info_bit_sel)<<4|(pCTRL->stDESERIALIZER.wdr_info_sel));	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x58F, (pCTRL->stDESERIALIZER.fifo_ce_rst_ctrl)<<11|(pCTRL->stDESERIALIZER.dout_ctrl)<<9|(pCTRL->stDESERIALIZER.ddio_data_swap)<<2);	
	MISP_RegField(MISP_LOCAL_ID, 0x58F, 0, 1, 1);	// fifo_ctrl
#else 												// MDIN-i51X (i510, i540, i550)
	MISP_RegWrite(MISP_LOCAL_ID, 0x56C, (pCTRL->stDESERIALIZER.ss_slave_en)<<15| (pCTRL->stDESERIALIZER.vd_edge_polarity)<<13|(pCTRL->stDESERIALIZER.hd_edge_polarity)<<12|(pCTRL->stDESERIALIZER.hd_delay));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x56D, (pCTRL->stDESERIALIZER.slave_mode_sel)<<13|(pCTRL->stDESERIALIZER.hd_high_width));	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x56E, (pCTRL->stDESERIALIZER.ext_hactive)<<12|(pCTRL->stDESERIALIZER.dol_wdr_enb)<<7|(pCTRL->stDESERIALIZER.dol_f_mode)<<5|(pCTRL->stDESERIALIZER.wdr_info_en)<<4|(pCTRL->stDESERIALIZER.wdr_info_bit_sel));	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x570, (pCTRL->stDESERIALIZER.ext_trig_h_dly_enb)<<12|(pCTRL->stDESERIALIZER.vd_high_width));	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x571, pCTRL->stDESERIALIZER.sens_trig_accum_val);
	MISP_RegWrite(MISP_LOCAL_ID, 0x572, pCTRL->stDESERIALIZER.trig_blank_width);
	// MISP_RegWrite(MISP_LOCAL_ID, 0x574, (pCTRL->stDESERIALIZER.des_clk_conv_enb)<<12|(pCTRL->stDESERIALIZER.des_conv_blk_size));
	MISP_RegWrite(MISP_LOCAL_ID, 0x58F, (pCTRL->stDESERIALIZER.rd_lock_cnt_max_sel)<<13|(pCTRL->stDESERIALIZER.fifo_ce_rst_ctrl)<<11|(pCTRL->stDESERIALIZER.dout_ctrl)<<9|(pCTRL->stDESERIALIZER.fifo_rst_enb_reg)<<8|(pCTRL->stDESERIALIZER.fifo_rd_delay)<<5|(pCTRL->stDESERIALIZER.ddio_data_swap)<<2);
#endif
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x58E, (pCTRL->stDESERIALIZER.data_lch_shift)<<8|(pCTRL->stDESERIALIZER.data_lbit_shift));	// 	

	#if defined(__USE_MDIN_i500__)
	MISP_RegField(MISP_LOCAL_ID, 0x58F, 0, 1, 1);	// fifo_ctrl
	#endif
	
}
//--------------------------------------------------------------------------------------------------------------------------

static void SSCTRL_ImageSizeHandler(PSS_CTRL_PARAM pCTRL)
{
	WORD in_hsize ;
	WORD in_vsize ;
	
	if ((pCTRL->ctrl&SSCTRL_IMAGE_SIZE_UPDATE)==0) return;
	pCTRL->ctrl &= (~SSCTRL_IMAGE_SIZE_UPDATE);	

	in_hsize = pCTRL->active_h_size + (pCTRL->bH<<1) ;
	in_vsize = pCTRL->active_v_size + (pCTRL->bV<<1) ;

	MISP_RegWrite(MISP_LOCAL_ID, 0x4A2, in_hsize);	// bayer bl h
	MISP_RegWrite(MISP_LOCAL_ID, 0x4A3, in_vsize);	// bayer bl v
	
}
//--------------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------------

static void SSCTRL_ImageStartpointHandler(PSS_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&SSCTRL_START_POINT_UPDATE)==0) return;
	pCTRL->ctrl &= (~SSCTRL_START_POINT_UPDATE);	

	MISP_RegWrite(MISP_LOCAL_ID, 0x4A0, pCTRL->startH);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x4A1, pCTRL->startV);
}

//--------------------------------------------------------------------------------------------------------------------------
static void SSCTRL_BinningHandler(PSS_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&SSCTRL_BINNING_UPDATE)==0) return;
	pCTRL->ctrl &= (~SSCTRL_BINNING_UPDATE);	

	MISP_RegField(MISP_LOCAL_ID, 0x4B1, 0, 1, pCTRL->stBINNING.binning_en);
	
	#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	MISP_RegField(MISP_LOCAL_ID, 0x4B1, 3, 1, 1&pCTRL->stBINNING.binning_en);
	#else 												// MDIN-i51X (i510, i540, i550)
	MISP_RegField(MISP_LOCAL_ID, 0x4B1, 5, 1, 1&pCTRL->stBINNING.binning_en);
	#endif		
}

//--------------------------------------------------------------------------------------------------------------------------

static void SSCTRL_BlackLevelHandler(PSS_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&SSCTRL_BLACK_LEVEL_UPDATE)!=0) 
	{
		pCTRL->ctrl &= (~SSCTRL_BLACK_LEVEL_UPDATE);
		MISP_RegWrite(MISP_LOCAL_ID, 0x4A8, (pCTRL->stBLACK_LEVEL.blc_enable<<15)|(pCTRL->stBLACK_LEVEL.mode<<13)|
													(pCTRL->stBLACK_LEVEL.h_sync_cut_en<<11)|(pCTRL->stBLACK_LEVEL.sync_mode<<9)|
													(pCTRL->stBLACK_LEVEL.sync_bypass_n<<8)|(pCTRL->stBLACK_LEVEL.blc_hactend_robust<<6)|(pCTRL->stBLACK_LEVEL.update_frequency));	// bayer bl enable		
	}
	
	if ((pCTRL->ctrl&SSCTRL_BLACK_LEVEL_VALUE_UPDATE)!=0) 
	{
		pCTRL->ctrl &= (~SSCTRL_BLACK_LEVEL_VALUE_UPDATE);

		MISP_RegWrite(MISP_LOCAL_ID, 0x4A9, pCTRL->stBLACK_LEVEL.manual_blc_val);	// bayer bl target(manual blc value)
		MISP_RegWrite(MISP_LOCAL_ID, 0x4B0, (pCTRL->stBLACK_LEVEL.odm_blc_val<<4)|(pCTRL->stBLACK_LEVEL.type<<2)|(pCTRL->stWDR_MEM_WRITE.odm_sync_sel));
	}		

}
//--------------------------------------------------------------------------------------------------------------------------
static void SSCTRL_SensorTpHandler(PSS_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&SSCTRL_TP_UPDATE)!=0) 
	{
	//position&size update
	MISP_RegField(MISP_LOCAL_ID, 0x230, 0, 12, pCTRL->stTESTPATTERN.tp_hsize); 	
	MISP_RegField(MISP_LOCAL_ID, 0x231, 0, 12, pCTRL->stTESTPATTERN.tp_vsize); 		
	MISP_RegField(MISP_LOCAL_ID, 0x232, 0, 12, pCTRL->stTESTPATTERN.tp_hstart); 
	MISP_RegField(MISP_LOCAL_ID, 0x233, 0, 12, pCTRL->stTESTPATTERN.tp_vstart); 

	//mode update
	//long	
	MISP_RegWrite(MISP_LOCAL_ID, 0x234, pCTRL->stTESTPATTERN.l_tp_pattern_mode<<3 | pCTRL->stTESTPATTERN.tp_pix_id<<1 | pCTRL->stTESTPATTERN.l_tp_enable);
	MISP_RegWrite(MISP_LOCAL_ID, 0x238, pCTRL->stTESTPATTERN.l_tp_pattern_sel1 <<12 | pCTRL->stTESTPATTERN.l_tp_pattern_sel2<<8 | pCTRL->stTESTPATTERN.l_tp_pattern_sel3<<4 | pCTRL->stTESTPATTERN.l_tp_pattern_sel4);
	//s1
	MISP_RegField(MISP_LOCAL_ID, 0x239, 3, 3, pCTRL->stTESTPATTERN.s1_tp_pattern_mode);
	MISP_RegWrite(MISP_LOCAL_ID, 0x23D, pCTRL->stTESTPATTERN.s1_tp_pattern_sel1 <<12 | pCTRL->stTESTPATTERN.s1_tp_pattern_sel2<<8 | pCTRL->stTESTPATTERN.s1_tp_pattern_sel3<<4 | pCTRL->stTESTPATTERN.s1_tp_pattern_sel4);
	//s2
	MISP_RegField(MISP_LOCAL_ID, 0x23E, 3, 3, pCTRL->stTESTPATTERN.s2_tp_pattern_mode);
	MISP_RegWrite(MISP_LOCAL_ID, 0x242, pCTRL->stTESTPATTERN.s2_tp_pattern_sel1 <<12 | pCTRL->stTESTPATTERN.s2_tp_pattern_sel2<<8 | pCTRL->stTESTPATTERN.s2_tp_pattern_sel3<<4 | pCTRL->stTESTPATTERN.s2_tp_pattern_sel4);
	//s3
	MISP_RegField(MISP_LOCAL_ID, 0x243, 3, 3, pCTRL->stTESTPATTERN.s3_tp_pattern_mode);
	MISP_RegWrite(MISP_LOCAL_ID, 0x247, pCTRL->stTESTPATTERN.s3_tp_pattern_sel1 <<12 | pCTRL->stTESTPATTERN.s3_tp_pattern_sel2<<8 | pCTRL->stTESTPATTERN.s3_tp_pattern_sel3<<4 | pCTRL->stTESTPATTERN.s3_tp_pattern_sel4);
	}

}
//--------------------------------------------------------------------------------------------------------------------------
static void SSCTRL_ResetDeserializerFIFO(PSS_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&SSCTRL_RESET_DES_FIFO)==0) return ;
	pCTRL->ctrl &= (~SSCTRL_RESET_DES_FIFO);	

//	stSSCTRL.stDESERIALIZER.fifo_rst_enb_reg = 0;
//	stSSCTRL.stDESERIALIZER.fifo_rst_enb_reg = 1;
	MISP_RegField(MISP_LOCAL_ID, 0x58F, 8, 1, 0);
	MISP_RegField(MISP_LOCAL_ID, 0x58F, 8, 1, 1);
}

static void SSCTRL_SetSensorHBlankSize(PSS_CTRL_PARAM pCTRL)
{
	WORD h_blank_val = 248;

#if defined(__USE_CMOS_IMXx36__)
	if(GetSS_SensorVoutPAL()) h_blank_val = 688 ;
	else		h_blank_val = 248 ;
#elif defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__)
	if(GetSS_SensorVoutPAL()) h_blank_val = 692 ;
	else		h_blank_val = 252 ;
	// h_blank_val = 688 ;
	//	h_blank_val = 248 ;	
#elif defined(__USE_CMOS_IMX238__)
	if(GetSS_SensorVoutPAL()) h_blank_val = 225 ;
	else		h_blank_val = 188 ;
	//h_blank_val = 264 ;	
#elif defined(__USE_CMOS_IMX185__)
	if(GetSS_SensorVoutPAL()) h_blank_val = 708 ;
	else		h_blank_val = 268 ;
	// h_blank_val = 688 ;
	//	h_blank_val = 248 ;
#elif defined(__USE_CMOS_IMX174__)  ||  defined(__USE_CMOS_IMX249__)
	h_blank_val = 264 ;	
#elif defined(__USE_CMOS_IMX124__)
	#if defined(__USE_IMX124_1536P30_MODE__)
		if(GetSS_SensorVoutPAL()) h_blank_val = 636 ;
		else		h_blank_val = 180 ;
	#else
		h_blank_val = 264 ;
	#endif
#elif defined(__USE_CMOS_AR0331__)
	if(GetSS_SensorVoutPAL()) h_blank_val = 676 ;
	else		h_blank_val = 236 ;
#elif defined(__USE_CMOS_MN34041__)
	if(GetSS_SensorVoutPAL()) h_blank_val = 864 ;
	else		h_blank_val = 384 ;
#elif defined(__USE_CMOS_MN34220__)
	if(GetSS_SensorVoutPAL()) h_blank_val = 684 ;
	else		h_blank_val = 244 ;
#elif defined(__USE_CMOS_MN34420__)
	if(GetSS_SensorVoutPAL()) h_blank_val = 684 ;
	else		h_blank_val = 244 ;	
#elif defined(__USE_CMOS_IMX226__)
	if(GetSS_SensorVoutPAL()) h_blank_val = 788 ;
	else		h_blank_val = 788 ;
#elif defined(__USE_CMOS_IMX274__)
	if(GetSS_SensorVoutPAL()) h_blank_val = 432 ;
	else		h_blank_val = 360 ;
#elif defined(__USE_CMOS_IMX326__)
	if(GetSS_SensorVoutPAL()) h_blank_val = 432 ;
	else		h_blank_val = 360 ;
#endif

	stSSCTRL.stDESERIALIZER.ref_h_blank_size = h_blank_val;
}

//--------------------------------------------------------------------------------------------------------------------------
void SSCTRL_DesErrCorrection(void)
{
#if defined(__USE_CMOS_IMX124__) || defined(__USE_CMOS_IMXx36__)
//	WORD user_h, user_v, user_h_bl;
	WORD err_h, err_v, err_h_bl;
	WORD tmp_err_h, tmp_err_v, tmp_err_h_bl;
	
//	user_h = stSSCTRL.stDESERIALIZER.ref_h_size;
//	user_v = stSSCTRL.stDESERIALIZER.ref_v_size;
//	user_h_bl = stSSCTRL.stDESERIALIZER.ref_h_blank_size;
	
	MISP_RegRead(MISP_LOCAL_ID, 0x568, &tmp_err_h);
	MISP_RegRead(MISP_LOCAL_ID, 0x569, &tmp_err_v);
	MISP_RegRead(MISP_LOCAL_ID, 0x593, &tmp_err_h_bl);
	
	err_h = 0x1FFF&tmp_err_h;
	err_v = 0x0FFF&tmp_err_v;
	err_h_bl = 0x0FFF&tmp_err_h_bl;

	if(err_h !=0 ||err_v !=0 || err_h_bl !=0) {
#if defined(__USE_CMOS_IMX124__)
		IMX124_DCKRST_Sequence();	//DCKRST reset sequence
#endif
		SetSS_reset_des_fifo();

		//order logic clear & restart
		MISP_RegField(MISP_LOCAL_ID, 0x567, 15, 1, 1);  		// clear on
		MISP_RegField(MISP_LOCAL_ID, 0x567, 15, 1, 0);  		// clear off
	}
#endif
}


//--------------------------------------------------------------------------------------------------------------------------
static void SSCTRL_DecompEnableHandler(PSS_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&SSCTRL_HTM_DECOMP_EN_UPDATE)==0) return ;
	pCTRL->ctrl &= (~SSCTRL_HTM_DECOMP_EN_UPDATE);		
	
	MISP_RegField(MISP_LOCAL_ID, 0x220, 0, 1, (pCTRL->stHTM_DECOMP.decomp_en));
}

//--------------------------------------------------------------------------------------------------------------------------
static void SSCTRL_DataOrderHandler(PSS_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&SSCTRL_DATA_ORDER_UPDATE)==0) return ;
	pCTRL->ctrl &= (~SSCTRL_DATA_ORDER_UPDATE);	
	
	MISP_RegField(MISP_LOCAL_ID, 0x234, 1, 2, (pCTRL->stTESTPATTERN.tp_pix_id));	
}

//--------------------------------------------------------------------------------------------------------------------------
#if __SS_SI_SETUP__	
static void SSCTRL_SyncIntegrityHandler(PSS_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&SSCTRL_SI_UPDATE)==0) return ;
	pCTRL->ctrl &= (~SSCTRL_SI_UPDATE);	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x57A, (pCTRL->stSYNC_INTEGRITY.si_sync_gen_en<<15)|(pCTRL->stSYNC_INTEGRITY.si_sync_lock<<14)|(pCTRL->stSYNC_INTEGRITY.si_sync_htotal));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x57B, (pCTRL->stSYNC_INTEGRITY.si_sync_vtotal));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x57C, (pCTRL->stSYNC_INTEGRITY.si_en<<15));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x596, (pCTRL->stSYNC_INTEGRITY.si_sync_hrise));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x597, (pCTRL->stSYNC_INTEGRITY.si_sync_vrise));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x598, (pCTRL->stSYNC_INTEGRITY.si_sync_hfall));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x599, (pCTRL->stSYNC_INTEGRITY.si_sync_vfall));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x59A, 0x0000);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x59B, 0x0020);		
}
#endif	

//------------------------------------------------------------------------------------
static void SSCTRL_SetLvdsInDelay(PSS_CTRL_PARAM pCTRL)
{
	DWORD rVal = 0;
	DWORD inv = pCTRL->lvds_in_inv;
	DWORD dly = pCTRL->lvds_in_delay;
	
	// lvds input invert
	rVal = (inv<<23) | (inv<<20) | (inv<<17) | (inv<<14) |\
			(inv<<11) | (inv<<8) | (inv<<5) | (inv<<2);
	
	// lvds input delay
	rVal |= (dly<<21) | (dly<<18) | (dly<<15) | (dly<<12) |\
			(dly<<9) | (dly<<6) | (dly<<3) | (dly<<0);
	
#if defined(__USE_MDIN_i500__)						// MDIN-i500
	MISP_RegWrite(MISP_HOST_ID, 0x01C, 0x4000);		// LVDS port4 swap
	MISP_RegWrite(MISP_HOST_ID, 0x01D, 0x0000);
#else												// MDIN-i51X (i510, i540, i550)
	MISP_RegWrite(MISP_HOST_ID, 0x01C, LOWORD(rVal));
	MISP_RegWrite(MISP_HOST_ID, 0x01D, HIWORD(rVal));
#endif
}

//------------------------------------------------------------------------------------
static void SSCTRL_LvdsInDelayHandler(PSS_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&SSCTRL_LVDS_IN_DLY_UPDATE)==0)	return;
	pCTRL->ctrl &= (~SSCTRL_LVDS_IN_DLY_UPDATE);
	
	SSCTRL_SetLvdsInDelay(&stSSCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
// GET / SET function

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_black_level_mode(BYTE val)
{
	stSSCTRL.stBLACK_LEVEL.mode = val ;
	stSSCTRL.ctrl |= (SSCTRL_BLACK_LEVEL_UPDATE | SSCTRL_TRIGGER_ISP);
}

//--------------------------------------------------------------------------------------------------------------------------

void SS_ProcessHandler(void)
{
	if ((stSSCTRL.ctrl&SSCTRL_TRIGGER_ISP)==0) return;
	stSSCTRL.ctrl &= (~SSCTRL_TRIGGER_ISP);

	SSCTRL_ImageStartpointHandler(&stSSCTRL);
	SSCTRL_ImageSizeHandler(&stSSCTRL);
	SSCTRL_DataOrderHandler(&stSSCTRL);
	SSCTRL_DeserializerHandler(&stSSCTRL);
	SSCTRL_WDRMemWriteHandler(&stSSCTRL);
	SSCTRL_DecompEnableHandler(&stSSCTRL);
	SSCTRL_BlackLevelHandler(&stSSCTRL);
	SSCTRL_BinningHandler(&stSSCTRL);
	SSCTRL_SensorTpHandler(&stSSCTRL);	
	SSCTRL_LvdsInDelayHandler(&stSSCTRL);

#if __SS_SI_SETUP__
	SSCTRL_SyncIntegrityHandler(&stSSCTRL);	
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetSS_black_level_update_freq(BYTE val)
{
	stSSCTRL.stBLACK_LEVEL.update_frequency = val ;
	stSSCTRL.ctrl |= (SSCTRL_BLACK_LEVEL_UPDATE | SSCTRL_TRIGGER_ISP);
}	
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_black_level_sync_mode(BYTE val)
{
	stSSCTRL.stBLACK_LEVEL.sync_mode = val ;
	stSSCTRL.ctrl |= (SSCTRL_BLACK_LEVEL_UPDATE | SSCTRL_TRIGGER_ISP);
}	
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_man_black_level_target(WORD val)
{
	stSSCTRL.stBLACK_LEVEL.manual_blc_val = val ;
	stSSCTRL.ctrl |= (SSCTRL_BLACK_LEVEL_VALUE_UPDATE | SSCTRL_TRIGGER_ISP);
}
void SetSS_ODM_black_level_target(WORD val)
{
	stSSCTRL.stBLACK_LEVEL.odm_blc_val = val;
	stSSCTRL.ctrl |= (SSCTRL_BLACK_LEVEL_VALUE_UPDATE | SSCTRL_TRIGGER_ISP);
}
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_wdr_write_mode(BYTE proc_mode, BYTE bit_mode, BYTE proc_frame_en)
{
	stSSCTRL.stWDR_MEM_WRITE.proc_mode = proc_mode;  // MISP_WDR_MEM_WIRTE_MODE_t

	stSSCTRL.stWDR_MEM_WRITE.l_bit_mode  = bit_mode;	// "0" 8bit, "1" 10bit, "2" 9bit, "3" 12bit
	stSSCTRL.stWDR_MEM_WRITE.s1_bit_mode = bit_mode;
	stSSCTRL.stWDR_MEM_WRITE.s2_bit_mode = bit_mode;
	stSSCTRL.stWDR_MEM_WRITE.s3_bit_mode = bit_mode;

	stSSCTRL.stWDR_MEM_WRITE.proc_frame_en = proc_frame_en;
	
	stSSCTRL.ctrl |= (SSCTRL_WDR_MEM_WRITE_UPDATE | SSCTRL_TRIGGER_ISP);
}



#if defined(__USE_CMOS_IMX124__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx124_s4(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.sensor_mode = MISP_SS_SONY_LVDS_4CH ;	
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
  #if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #endif
#else											// edk & ref board
#if defined (__USE_CMOS_IMX123__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
#else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#endif

#endif
#endif

	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 3 ;
#else 												// MDIN-i51X (i510, i540, i550)
	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_FRAME:
			stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
			stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			break;
		case MISP_WDR_PROC_LINE2:
			stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
			stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 1 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			break;
		case MISP_WDR_PROC_LINE3:
			stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
			stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 1 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 1 ;
			break;
		default:
			stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
			stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			break;
	}
#if defined(__USE_SENS_HSYNC_EXT__)
  #if defined(__USE_IMX124_1080P60_MODE__)||\
      defined(__USE_IMX124_1080P30_MODE__)
	stSSCTRL.stDESERIALIZER.ext_hactive = 8;  //extend h-active 1936->1952
  #endif
#endif

#endif	

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;
#if defined(__USE_IMX124_1536P30_MODE__)	
	stSSCTRL.stDESERIALIZER.ref_h_size = 2064 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1565 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 85;	// 1565 + 85 = 1650
	#if defined(__USE_25Hz_SENSOR__)
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 180 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 636 ;
	#endif
#else
	stSSCTRL.stDESERIALIZER.ref_h_size = 1936 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1109 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 16;	// 1109 + 16 = 1125
	#if defined(__USE_25Hz_SENSOR__)	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 704 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 264 ;
	#endif
#endif

}
#endif



#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
#if defined(__USE_CMOS_IMX124__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx124_s8(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;
	stSSCTRL.sensor_mode = MISP_SS_LVDS_8CH ;
//	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;

	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;

	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_FRAME:
			stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
			stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
			break;
		case MISP_WDR_PROC_LINE2:
			stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
			stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 1 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY_DOL ;
			break;
		case MISP_WDR_PROC_LINE3:
			stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
			stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 1 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 1 ;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY_DOL ;
			break;
		default:
			stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
			stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
			break;
	}
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;
#if defined(__USE_IMX124_1536P30_MODE__)	
//#if defined(__USE_CMOS_IMX123_LVDS_S8__)
	stSSCTRL.stDESERIALIZER.ref_h_size = 1032 ;		// 8ch mode has 1/2 counter
//#else
//	stSSCTRL.stDESERIALIZER.ref_h_size = 2064 ;
//#endif
	stSSCTRL.stDESERIALIZER.ref_v_size = 1565 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 85;	// 1565 + 85 = 1650
	#if defined(__USE_25Hz_SENSOR__)
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 180 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 636 ;
	#endif
#else
	stSSCTRL.stDESERIALIZER.ref_h_size = 1936 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1109 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 16;	// 1109 + 16 = 1125
	#if defined(__USE_25Hz_SENSOR__)	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 704 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 264 ;
	#endif
#endif

}
#endif
#endif

#if defined(__USE_CMOS_IMX252__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx252_s4(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.sensor_mode = MISP_SS_SONY_LVDS_4CH ;	
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;
#endif

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
  #if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #endif
#else											// edk & ref board
 #if defined(__USE_MDIN_i510__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 6 ;
 #else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
 #endif
#endif


#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	#if defined(__USE_IMX252_SLAVE_MODE__)
	stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;		//only __USE_MDIN_i5XX_REV__
		#if defined(__USE_TRIGGER_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 6 ;
		stSSCTRL.stDESERIALIZER.sens_trig_accum_val = 800 ;
		stSSCTRL.stDESERIALIZER.trig_blank_width = 325 ;
		#else
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;	
		#endif
	#else
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;		// Use AUX sync until i5x0 revision for Sensor Slave Mode
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;	
	#endif
#else
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;		// Use AUX sync until i5x0 revision for Sensor Slave Mode
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;	
#endif


#if defined(__USE_IMX252_1536P_ALL_SCAN_MODE__)		//see data sheet 45p
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3384 ;			//blank 208, htotal : 2256(2048 + 208), 2256*3/2 = 3384
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1576 ;	
#elif defined(__USE_IMX252_1080P60_MODE__)
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;			//blank 264, htotal : 2200(1936 + 264)
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;	
#else //#define	__USE_IMX252_1080P30_MODE__
	stSSCTRL.stDESERIALIZER.ss_slave_H = 6600 ;			//blank 2464, htotal : 4400(1936 + 2464 = 4400)
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;	
#endif		

	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;

	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
//	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
//	stSSCTRL.stDESERIALIZER.hd_high_width = 256 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 4 ;

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;

#if defined(__USE_SENS_HSYNC_EXT__)
  #if defined(__USE_IMX252_1080P60_MODE__)||\
      defined(__USE_IMX252_1080P30_MODE__)
	stSSCTRL.stDESERIALIZER.ext_hactive = 8;  //extend h-active 1936->1952
  #endif
#endif

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;
	
#if defined(__USE_IMX252_1536P_ALL_SCAN_MODE__)	
	stSSCTRL.stDESERIALIZER.ref_h_size = 2064 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1572 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 4;	// 1572 + 4 = 1576
	#if defined(__USE_25Hz_SENSOR__)
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 180 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 208 ;
	#endif
#else
	stSSCTRL.stDESERIALIZER.ref_h_size = 1936 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1121 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 4;	// 1121 + 4 = 1125

	#if defined(__USE_IMX252_1080P60_MODE__)
		#if defined(__USE_25Hz_SENSOR__)	
			stSSCTRL.stDESERIALIZER.ref_h_blank_size = 704 ;		
		#else
			stSSCTRL.stDESERIALIZER.ref_h_blank_size = 264 ;
		#endif
	#else
		#if defined(__USE_25Hz_SENSOR__)	
			stSSCTRL.stDESERIALIZER.ref_h_blank_size = 3344 ;	
		#else
			stSSCTRL.stDESERIALIZER.ref_h_blank_size = 2464 ;
		#endif
	#endif
#endif


}
#endif


#if defined(__USE_CMOS_IMX265__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx265_s4(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.sensor_mode = MISP_SS_SONY_LVDS_4CH ;	
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;
#endif

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
  #if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #endif
#else											// edk & ref board
 #if defined(__USE_MDIN_i510__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 6 ;
 #else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
 #endif
#endif


#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	#if defined(__USE_IMX265_SLAVE_MODE__)
		#if defined(__USE_TRIGGER_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 6 ;
		stSSCTRL.stDESERIALIZER.sens_trig_accum_val = 800 ;
		stSSCTRL.stDESERIALIZER.trig_blank_width = 325 ;
		#else
		stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;		//only __USE_MDIN_i5XX_REV__		
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;	
		#endif
	#else
		#if defined(__USE_TRIGGER_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 6 ;
		stSSCTRL.stDESERIALIZER.sens_trig_accum_val = 800 ;
		stSSCTRL.stDESERIALIZER.trig_blank_width = 325 ;
		#else
		stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;		//only __USE_MDIN_i5XX_REV__		
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;	
		#endif
	#endif
#else
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;		// Use AUX sync until i5x0 revision for Sensor Slave Mode
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;	
#endif

#if defined(__USE_ISP_SCLK_OUT__)
	#if defined(__USE_IMX265_1536P_ALL_SCAN_MODE__)		//see data sheet 45p
		stSSCTRL.stDESERIALIZER.ss_slave_H = 6000 ;			//blank 208, htotal : 2256(2048 + 208), 2256*3/2 = 3384 : source clk [LVDS serial clk] 55.6fps
		stSSCTRL.stDESERIALIZER.ss_slave_V = 1650 ;			
	#elif defined(__USE_IMX265_1080P60_MODE__) || defined(__USE_IMX265_ROI_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;			//blank 264, htotal : 2200(1936 + 264),2200*3/2 = 3300 : source clk [LVDS serial clk]
		stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;	
	#else
		stSSCTRL.stDESERIALIZER.ss_slave_H = 6600 ;			//blank 2464, htotal : 4400(1936 + 2464 = 4400) 4400*3/2 = 6600 : source clk [LVDS serial clk]
		stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;	
	#endif		//#define	__USE_IMX265_1080P30_MODE__
#else
	#if defined(__USE_IMX265_1536P_ALL_SCAN_MODE__)		//see data sheet 45p
		stSSCTRL.stDESERIALIZER.ss_slave_H = 411 ;			//blank 208, htotal : 2256(2048 + 208), 2256*2/11 = 410.1818 : source clk [xtal 27Mhz] not tested
		stSSCTRL.stDESERIALIZER.ss_slave_V = 1576 ;	
	#elif defined(__USE_IMX265_1080P60_MODE__) || defined(__USE_IMX265_ROI_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 400 ;			//blank 264, htotal : 2200(1936 + 264),2200*2/11 = 400 : source clk [xtal 27Mhz]
		stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;	
	#else
		stSSCTRL.stDESERIALIZER.ss_slave_H = 800 ;			//blank 2464, htotal : 4400(1936 + 2464 = 4400) 4400*2/11 = 800 : source clk [xtal 27Mhz]
		stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;	
	#endif		//#define	__USE_IMX265_1080P30_MODE__
#endif

	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;

//	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ; //  source clk [LVDS serial clk]
	stSSCTRL.stDESERIALIZER.hd_high_width = 37 ; // source clk [xtal 27Mhz]
//	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ; 
//	stSSCTRL.stDESERIALIZER.hd_high_width = 256 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 4 ;

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;

#if defined(__USE_SENS_HSYNC_EXT__)
  #if defined(__USE_IMX265_1080P60_MODE__)||\
      defined(__USE_IMX265_1080P30_MODE__)
	stSSCTRL.stDESERIALIZER.ext_hactive = 8;  //extend h-active 1936->1952
  #endif
#endif

	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_FRAME:
			stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
			stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
			break;
		default:
			stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
			stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
			break;
	}

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;
	stSSCTRL.stDESERIALIZER.fifo_ctrl = MISP_SS_DESFIFO_MEM_RD_ADDR_FOLLOW_WR_OTHERS_EN ;
	
#if defined(__USE_IMX265_1536P_ALL_SCAN_MODE__)	
	stSSCTRL.stDESERIALIZER.ref_h_size = 2064 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1555 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 21;
	#if defined(__USE_25Hz_SENSOR__)
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 2336 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 1936 ;
	#endif
#else
	stSSCTRL.stDESERIALIZER.ref_h_size = 1936 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1107;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 18;	// 1107 + 18 = 1125

	#if defined(__USE_IMX265_1080P60_MODE__) || defined(__USE_IMX265_ROI_MODE__)
		#if defined(__USE_25Hz_SENSOR__)	
			stSSCTRL.stDESERIALIZER.ref_h_blank_size = 704 ;		
		#else
			stSSCTRL.stDESERIALIZER.ref_h_blank_size = 264 ;
		#endif
	#else
		#if defined(__USE_25Hz_SENSOR__)	
			stSSCTRL.stDESERIALIZER.ref_h_blank_size = 3344 ;	
		#else
			stSSCTRL.stDESERIALIZER.ref_h_blank_size = 2464 ;
		#endif
	#endif
#endif


}
#endif



#if defined(__USE_CMOS_IMX174__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx174_s4(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.sensor_mode = MISP_SS_SONY_LVDS_4CH ;	
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
  #if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #endif
#else											// edk & ref board
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
#endif
#endif

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
  #if defined(__USE_TRIGGER_MODE__)
  	stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;

  #else 
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
  #endif

#else 												// MDIN-i51X (i510, i540, i550)
  #if defined(__USE_TRIGGER_MODE__)
	stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 6 ;
  #else
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7 ;
  #endif
	stSSCTRL.stDESERIALIZER.sens_trig_accum_val = 800 ;
	stSSCTRL.stDESERIALIZER.trig_blank_width = 325 ;
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif

	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 3 ;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
  #if defined(__USE_SENS_HSYNC_EXT__)
	stSSCTRL.stDESERIALIZER.ext_hactive = 8;  //extend h-active 1936->1952
  #endif
#endif


	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 1936 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1107 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 18;	// 1107 + 18 = 1125
	
	#if defined(__USE_25Hz_SENSOR__)	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 704 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 264 ;
	#endif
}
#endif



#if defined(__USE_CMOS_IMX249__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx249_s2(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.sensor_mode = MISP_SS_SONY_LVDS_2CH ;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.sensor_mode = MISP_SS_LVDS_2CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;	
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
  #if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #endif
#else											// edk & ref board
	#if defined(__USE_MDIN_i510__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 6 ;
	#else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
	#endif
#endif
	
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
		
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
	stSSCTRL.stDESERIALIZER.ss_slave_H = 6600 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 3 ;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;
#endif

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 1936 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1107 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 18;	// 1107 + 18 = 1125
	
	#if defined(__USE_25Hz_SENSOR__)	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 704 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 264 ;
	#endif
}
#endif

#if defined(__USE_CMOS_IMX253__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx253_s8(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;

	stSSCTRL.sensor_mode = MISP_SS_LVDS_8CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;	

//	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;

	stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 1 ;		// clock conversion enable
	stSSCTRL.stDESERIALIZER.des_conv_blk_size = 248 ;	// H blank size = r_4k_conv_blk_size /2 *(8/6) *2

	stSSCTRL.stDESERIALIZER.fifo_ctrl = MISP_SS_DESFIFO_MEM_RD_ADDR_FOLLOW_WR_EN;
	
	stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__USE_SENSOR_BAYER_10BIT__)
	stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 1 ;
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;
#else
	stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 0 ;
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
#endif

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
	
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;

#if defined(__USE_IMX253_SLAVE_MODE__)		
	stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
#else
	stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_H = 4400 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 2250 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
	
	stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;

	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 2056 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 2208 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 42;
	
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 877 ;
}
#endif

#if defined(__USE_CMOS_IMX273__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx273_s4(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;

	stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;	

//	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;

	stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 0 ;		// clock conversion enable
	stSSCTRL.stDESERIALIZER.des_conv_blk_size = 248 ;	// H blank size = r_4k_conv_blk_size /2 *(8/6) *2

	stSSCTRL.stDESERIALIZER.fifo_ctrl = MISP_SS_DESFIFO_MEM_RD_ADDR_FOLLOW_WR_EN;
	
	stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__USE_SENSOR_BAYER_10BIT__)
	stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 1 ;
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;
#else
	stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 0 ;
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
#endif

#if 0	//use 50pin header sensor imx273 pcb v1.0
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#else	//use 50pin header sensor imx273 pcb v2.0
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;	
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	#if defined(__USE_IMX273_SLAVE_MODE__)
		#if defined(__USE_TRIGGER_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 6 ;
		#else
		stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;		//only __USE_MDIN_i5XX_REV__		
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;	
		#endif
	#else
		#if defined(__USE_TRIGGER_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 6 ;
		#else
		stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;		//only __USE_MDIN_i5XX_REV__		
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;	
		#endif
	#endif
#else
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;		// Use AUX sync until i5x0 revision for Sensor Slave Mode
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;	
#endif

#if defined(__USE_IMX273_ALLSCAN_MODE__)
	#if defined(__USE_IMX273_720P30_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 6600 ;
	#else
		stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	#endif

	stSSCTRL.stDESERIALIZER.ss_slave_V = 1500 ;

	stSSCTRL.stDESERIALIZER.sens_trig_accum_val = 1400 ;
	stSSCTRL.stDESERIALIZER.trig_blank_width = 100 ;	
	
#elif defined(__USE_IMX273_ROI_MODE__)
	#if defined(__USE_IMX273_720P120_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 3000 ;
		stSSCTRL.stDESERIALIZER.ss_slave_V = 825 ;
	#elif defined(__USE_IMX273_720P30_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 12000 ;
		stSSCTRL.stDESERIALIZER.ss_slave_V = 825 ;
	#else
		stSSCTRL.stDESERIALIZER.ss_slave_H = 6000 ;	// H-Total=2200pixel. Orig. = 2112pixel(3168d)
		stSSCTRL.stDESERIALIZER.ss_slave_V = 825 ;
	#endif

	stSSCTRL.stDESERIALIZER.sens_trig_accum_val = 800 ;
	stSSCTRL.stDESERIALIZER.trig_blank_width = 25 ;		
#else
	#if defined(__USE_IMX273_720P120_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 2250 ;
		stSSCTRL.stDESERIALIZER.ss_slave_V = 1100 ;
	#elif defined(__USE_IMX273_720P30_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 9000 ;
		stSSCTRL.stDESERIALIZER.ss_slave_V = 1100 ;
	#else
		stSSCTRL.stDESERIALIZER.ss_slave_H = 4500 ;	// H-Total=2200pixel. Orig. = 2112pixel(3168d)
		stSSCTRL.stDESERIALIZER.ss_slave_V = 1100 ;
	#endif

	stSSCTRL.stDESERIALIZER.sens_trig_accum_val = 1000 ;
	stSSCTRL.stDESERIALIZER.trig_blank_width = 100 ;		
#endif

	
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
	

	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_FRAME:
			stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
			stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			break;
		default:
			stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
			stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			break;
	}
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 1456 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1015 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 18;
	
//	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 656 ;
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 120 ;
}

void SetSS_deserializer_imx273_s8(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;

	stSSCTRL.sensor_mode = MISP_SS_LVDS_8CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;	

//	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;

#if (defined(__USE_IMX273_720P240_MODE__) && defined(__USE_IMX273_ROI_MODE__))
	stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 1 ;		// clock conversion enable
	#if defined(__USE_25Hz_SENSOR__)
	stSSCTRL.stDESERIALIZER.des_conv_blk_size = 600 ;	// 1800 (lvds clock) : 2400 - 600 = 1800(pixel clock)
	SetSS_ClkConvSetup(stSSCTRL.stDESERIALIZER.des_clk_conv_enb,1200,stSSCTRL.stDESERIALIZER.des_conv_blk_size,0,40);
	#else
	stSSCTRL.stDESERIALIZER.des_conv_blk_size = 500 ;	// 1500 (lvds clock) : 2000 - 500 = 1500(pixel clock)
	SetSS_ClkConvSetup(stSSCTRL.stDESERIALIZER.des_clk_conv_enb,1000,stSSCTRL.stDESERIALIZER.des_conv_blk_size,0,40);	
	#endif

#else
	stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 0 ;		// clock conversion enable
	stSSCTRL.stDESERIALIZER.des_conv_blk_size = 248 ;	// H blank size = r_4k_conv_blk_size /2 *(8/6) *2
#endif

	stSSCTRL.stDESERIALIZER.fifo_ctrl = MISP_SS_DESFIFO_MEM_RD_ADDR_FOLLOW_WR_EN;
	
	stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__USE_SENSOR_BAYER_10BIT__)
	stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 1 ;
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;
#else
	stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 0 ;
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
#endif

#if 0	//use 50pin header sensor imx273 pcb v1.0
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#else	//use 50pin header sensor imx273 pcb v2.0
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;	
#endif

	if(stSSCTRL.stDESERIALIZER.slave_mode_sel==5)	stSSCTRL.stDESERIALIZER.slave_mode_sel = 5;
	else if(stSSCTRL.stDESERIALIZER.slave_mode_sel==4)	stSSCTRL.stDESERIALIZER.slave_mode_sel = 4;
	else		stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;


#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	#if defined(__USE_IMX273_SLAVE_MODE__)
		#if defined(__USE_TRIGGER_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 6 ;
		#else
		stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;		//only __USE_MDIN_i5XX_REV__		
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;	
		#endif
	#else
		#if defined(__USE_TRIGGER_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 6 ;
		#else
		stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;		//only __USE_MDIN_i5XX_REV__		
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;	
		#endif
	#endif
#else
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;		// Use AUX sync until i5x0 revision for Sensor Slave Mode
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;	
#endif



#if defined(__USE_IMX273_ALLSCAN_MODE__)
#if 1
	#if defined(__USE_IMX273_720P120_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 1650 ;	
	#elif defined(__USE_IMX273_720P30_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 6600 ;
	#else
		stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	#endif

	stSSCTRL.stDESERIALIZER.ss_slave_V = 1500 ;
	stSSCTRL.stDESERIALIZER.sens_trig_accum_val = 1400 ;
	stSSCTRL.stDESERIALIZER.trig_blank_width = 100 ;		
#else
	#if defined(__USE_IMX273_720P240_MODE__)
		#if defined(__USE_ISP_SCLK_OUT__) && !defined(__USE_25Hz_SENSOR__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 1320 ;
		#else
		stSSCTRL.stDESERIALIZER.ss_slave_H = 1100 ;
		#endif	//240hz extra case : __USE_ISP_SCLK_OUT__ && 30hz
	#elif defined(__USE_IMX273_720P120_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 2200 ;
	#elif defined(__USE_IMX273_720P30_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 8800 ;
	#else
		stSSCTRL.stDESERIALIZER.ss_slave_H = 4400 ;
	#endif

	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
#endif
#elif defined(__USE_IMX273_ROI_MODE__)
	#if defined(__USE_IMX273_720P240_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 1650 ;	// use clock conversion. 2000d-->1500
		stSSCTRL.stDESERIALIZER.ss_slave_V = 750 ;
//		stSSCTRL.stDESERIALIZER.ss_slave_H = 1500 ;		// for 200hz, 1500 x 1.2 = 1800 --> 2400d and clock conversion need 600d then 316? 318?
//		stSSCTRL.stDESERIALIZER.ss_slave_V = 825 ;
	#elif defined(__USE_IMX273_720P120_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 3000 ;
		stSSCTRL.stDESERIALIZER.ss_slave_V = 825 ;
	#elif defined(__USE_IMX273_720P30_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 12000 ;
		stSSCTRL.stDESERIALIZER.ss_slave_V = 825 ;
	#else
		stSSCTRL.stDESERIALIZER.ss_slave_H = 6000 ;	// H-Total=2200pixel. Orig. = 2112pixel(3168d)
		stSSCTRL.stDESERIALIZER.ss_slave_V = 825 ;
	#endif

	stSSCTRL.stDESERIALIZER.sens_trig_accum_val = 800 ;
	stSSCTRL.stDESERIALIZER.trig_blank_width = 25 ;		
#else
	#if defined(__USE_IMX273_720P120_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 2250 ;
		stSSCTRL.stDESERIALIZER.ss_slave_V = 1100 ;
	#elif defined(__USE_IMX273_720P30_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 9000 ;
		stSSCTRL.stDESERIALIZER.ss_slave_V = 1100 ;
	#else
		stSSCTRL.stDESERIALIZER.ss_slave_H = 4500 ;
		stSSCTRL.stDESERIALIZER.ss_slave_V = 1100 ;
	#endif

	stSSCTRL.stDESERIALIZER.sens_trig_accum_val = 1000 ;
	stSSCTRL.stDESERIALIZER.trig_blank_width = 100 ;		
#endif

	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
	
	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_FRAME:
			stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
			stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			break;
		default:
			stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
			stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			break;
	}
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 1456 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1015 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 18;
	
//	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 656 ;
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 120 ;
}

void SetSS_deserializer_imx273_s2(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;

	stSSCTRL.sensor_mode = MISP_SS_LVDS_2CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;	

//	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;

	stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 0 ;		// clock conversion enable
	stSSCTRL.stDESERIALIZER.des_conv_blk_size = 248 ;	// H blank size = r_4k_conv_blk_size /2 *(8/6) *2

	stSSCTRL.stDESERIALIZER.fifo_ctrl = MISP_SS_DESFIFO_MEM_RD_ADDR_FOLLOW_WR_EN;
	
	stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__USE_SENSOR_BAYER_10BIT__)
	stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 1 ;
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;
#else
	stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 0 ;
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
#endif

#if 0	//use 50pin header sensor imx273 pcb v1.0
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#else	//use 50pin header sensor imx273 pcb v2.0
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;	
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	#if defined(__USE_IMX273_SLAVE_MODE__)
		#if defined(__USE_TRIGGER_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 6 ;
		#else
		stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;		//only __USE_MDIN_i5XX_REV__		
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;	
		#endif
	#else
		#if defined(__USE_TRIGGER_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 6 ;
		#else
		stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;		//only __USE_MDIN_i5XX_REV__		
		stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;	
		#endif
	#endif
#else
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;		// Use AUX sync until i5x0 revision for Sensor Slave Mode
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;	
#endif

#if defined(__USE_IMX273_ALLSCAN_MODE__)
	#if defined(__USE_IMX273_720P30_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 6600 ;
	#else
		stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	#endif

	stSSCTRL.stDESERIALIZER.ss_slave_V = 1500 ;

	stSSCTRL.stDESERIALIZER.sens_trig_accum_val = 1400 ;
	stSSCTRL.stDESERIALIZER.trig_blank_width = 100 ;		
	
#elif defined(__USE_IMX273_ROI_MODE__)
	#if defined(__USE_IMX273_720P30_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 12000 ;
		stSSCTRL.stDESERIALIZER.ss_slave_V = 825 ;
	#else
		stSSCTRL.stDESERIALIZER.ss_slave_H = 6000 ;	// H-Total=2200pixel. Orig. = 2112pixel(3168d)
		stSSCTRL.stDESERIALIZER.ss_slave_V = 825 ;
	#endif

	stSSCTRL.stDESERIALIZER.sens_trig_accum_val = 800 ;
	stSSCTRL.stDESERIALIZER.trig_blank_width = 25 ;			
#else
	#if defined(__USE_IMX273_720P30_MODE__)
		stSSCTRL.stDESERIALIZER.ss_slave_H = 9000 ;
		stSSCTRL.stDESERIALIZER.ss_slave_V = 1100 ;
	#else
		stSSCTRL.stDESERIALIZER.ss_slave_H = 4500 ;	// H-Total=2200pixel. Orig. = 2112pixel(3168d)
		stSSCTRL.stDESERIALIZER.ss_slave_V = 1100 ;
	#endif

	stSSCTRL.stDESERIALIZER.sens_trig_accum_val = 1000;
	stSSCTRL.stDESERIALIZER.trig_blank_width = 100 ;			
#endif

	
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
	

	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_FRAME:
			stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
			stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			break;
		default:
			stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
			stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			break;
	}
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 1456 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1015 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 18;
	
//	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 656 ;
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 120 ;
}

#endif

//#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
#if defined(__USE_CMOS_IMX178__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx178(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.sensor_mode = 1 ;	// sony 4ch
#else 												// MDIN-i51X (i510, i540, i550)
	#if defined (__USE_CMOS_IMX178_LVDS_S8__)
		stSSCTRL.sensor_mode = MISP_SS_LVDS_8CH ;
	#else
		stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;
	#endif
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
#endif

	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
 #if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #endif
#else											// edk & ref board
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
#endif

	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
#endif
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

#if defined(__USE_IMX178_1080P60_MODE__)
	stSSCTRL.stDESERIALIZER.ss_slave_H = 2000 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1650 ;
	
	stSSCTRL.stDESERIALIZER.ref_h_size = 1944 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1117 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 533;	// 1117 + 533 = 1650
	
	#if defined(__USE_25Hz_SENSOR__)	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 704 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 56 ;
	#endif

#elif defined (__USE_IMX178_6M29_MODE__)
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3200 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 3003 ;

#if defined (__USE_CMOS_IMX178_LVDS_S8__)	
	stSSCTRL.stDESERIALIZER.ref_h_size = 1548 ;
#else
	stSSCTRL.stDESERIALIZER.ref_h_size = 3096 ;
#endif
	stSSCTRL.stDESERIALIZER.ref_v_size = 3003 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 0;	// 3003 + 0 = 3003(?)

	#if defined(__USE_25Hz_SENSOR__)	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 704 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 48 ;
	#endif

#elif defined (__USE_IMX178_5M29_MODE__)
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3200 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 3003 ;

#if defined (__USE_CMOS_IMX178_LVDS_S8__)	
	stSSCTRL.stDESERIALIZER.ref_h_size = 1308 ;
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 78 ;	// actually 157 pixel
#else
	stSSCTRL.stDESERIALIZER.ref_h_size = 2616 ;
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 53 ;
#endif
	stSSCTRL.stDESERIALIZER.ref_v_size = 1991 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 484;	// 1991 + 484 = 2475

#else	//__USE_IMX178_1728P30_MODE__
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3333 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1980 ;
	
#if defined (__USE_CMOS_IMX178_LVDS_S8__)
	stSSCTRL.stDESERIALIZER.ref_h_size = 1548 ;
#else
	stSSCTRL.stDESERIALIZER.ref_h_size = 3096 ;
#endif
	stSSCTRL.stDESERIALIZER.ref_v_size = 1775 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 205;	// 1775 + 205 = 1980
	
	#if defined(__USE_25Hz_SENSOR__)	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 704 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 237 ;
	#endif

#endif

}
#endif
//#endif



#if defined(__USE_CMOS_IMX185__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx185_s4(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.sensor_mode = MISP_SS_SONY_LVDS_4CH ;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
  #if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #endif
#else											// edk & ref board
	#if defined(__USE_40PIN_HEADER__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
	#else
		#if defined(__USE_MDIN_i510__)
		stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 3 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 2 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 0 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 7 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 6 ;
		#else
		stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
		#endif
	#endif
#endif
#endif

	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3960 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif

	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
  #if defined(__USE_SENS_HSYNC_EXT__)
	stSSCTRL.stDESERIALIZER.ext_hactive = 4;  //extend h-active 1945->1953
  #endif
#else 																							// i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 3 ;
#endif

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 1952 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1116 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 9;	// 1116 + 9 = 1125
	
	#if defined(__USE_25Hz_SENSOR__)	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 708 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 268 ;	
	//	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 688 ;
	#endif
}
#endif



#if defined(__USE_CMOS_IMXx22__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx222_ttl(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_TTL_INPUT ;
	stSSCTRL.sensor_mode = 0 ;	

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
		
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
	stSSCTRL.stDESERIALIZER.ss_slave_H = 2200 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 0 ;

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;	

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;
#if defined(__USE_IMXx22_720P30_MODE__)
	stSSCTRL.stDESERIALIZER.ref_h_size = 1644 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 749 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 1;	// 749 + 1 = 750
#else
	stSSCTRL.stDESERIALIZER.ref_h_size = 1984 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1105 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 20;	// 1105 + 20 = 1125
#endif
#if defined(__USE_25Hz_SENSOR__)
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 688 ;
#else
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 192 ;
#endif
}
#endif



#if defined(__USE_CMOS_OV2710__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_ov2710_ttl(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_TTL_INPUT ;
	stSSCTRL.sensor_mode = 0 ;	
		
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
	stSSCTRL.stDESERIALIZER.ss_slave_H = 2200 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 0 ;
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;	

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 1984 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1105 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 20;	// 1105 + 20 = 1125

#if defined(__USE_25Hz_SENSOR__)
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 688 ;
#else
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 192 ;
#endif
}
#endif


#if defined(__USE_CMOS_EXT3__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_ext3_ttl(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_TTL_INPUT ;
	stSSCTRL.sensor_mode = 0 ;	
		
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
	stSSCTRL.stDESERIALIZER.ss_slave_H = 2200 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 0 ;
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;	

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 1984 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1105 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 20;	// 1105 + 20 = 1125

#if defined(__USE_25Hz_SENSOR__)
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 688 ;
#else
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 192 ;
#endif
}
#endif

#if defined(__USE_CMOS_IMX238__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx238_s4(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.sensor_mode = MISP_SS_SONY_LVDS_4CH ;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
  #if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #endif
#else											// edk & ref board
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
#endif
#endif
		
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_H = 2200 ;
//	stSSCTRL.stDESERIALIZER.ss_slave_H = 2640 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
  #if defined(__USE_SENS_HSYNC_EXT__)
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;  //extend h-active
  #endif
#else 																							// i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 3 ;
#endif	

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 1312 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1069 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 31;	// 1069 + 31 = 1100
	
	#if defined(__USE_25Hz_SENSOR__)	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 225 ;
	#else
	//	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 248 ;	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 188 ;
	#endif

}
#endif

#if defined(__USE_CMOS_IMX385__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx385_s4(void)
{
	UARTprintf("IMX385 sLVDS CH4\r\n");
	const WORD h_tot=GetSS_SensorVoutPAL()?2640:2200;
	
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.sensor_mode = MISP_SS_SONY_LVDS_4CH ;
#else 												// MDIN-i51X (i510, i540, i550)
  #if defined(__USE_CMOS_IMX385_LVDS_S2__)	
	stSSCTRL.sensor_mode = MISP_SS_LVDS_2CH ;
  #elif defined(__USE_CMOS_IMX385_LVDS_S4__)	
	stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;
  #endif
  	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
  #if defined(__USE_SENSOR_BAYER_10BIT__)
	stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 1 ;
	stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 1 ;		// clock conversion enable
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;
  #else
	stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 0 ;
	stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 0 ;		// clock conversion enable
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;	
  #endif
	stSSCTRL.stDESERIALIZER.des_conv_blk_size = 233 ;	// r_4k_conv_blk_size = real H blank size - 20
	
	SetSS_ClkConvSetup(stSSCTRL.stDESERIALIZER.des_clk_conv_enb,h_tot,2200,stSSCTRL.stDESERIALIZER.des_conv_blk_size,40);
		
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
  #if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #endif
#else											// edk & ref board
	#if defined(__USE_MDIN_i510__)				// i510
		#if defined(__MDIN_i510_EDK_V10__)								// 510 edk v1.0(jumper)
			stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 3 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 2 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 7 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 6 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
		#elif defined(__MDIN_i510_IMX290__)								// 510 + imx290 oneboard
			stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
		#else															// 510 ref, edk v1.1
			stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 3 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 2 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 0 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 7 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 6 ;
		#endif
	#else										// i540, i550
		stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
	#endif
#endif
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
//	stSSCTRL.stDESERIALIZER.ss_slave_H = 2640 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;	
	stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_LINE3:
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 1 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 1 ; 
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY_DOL ;
			break;  //DOL 3
		case MISP_WDR_PROC_LINE2:
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 1 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY_DOL ;
			break;  //DOL 2
		case MISP_WDR_PROC_DISBL:
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
			break;
		default:
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;  
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
			break;  //N/A
	}
  #if defined(__USE_SENS_HSYNC_EXT__)
	stSSCTRL.stDESERIALIZER.ext_hactive = 4;  //extend h-active 1945->1953 (or 1305->1313)
  #endif
#else 																							// i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 3 ;
#endif

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 1948 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1110 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 15;	// 1110 + 15 = 1125

	#if defined(__USE_25Hz_SENSOR__)	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 692 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 252 ;
	#endif
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetSS_deserializer_imx385_s6(void)
{
	UARTprintf("IMX385 CSI2LVDS 6CH\r\n");
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;
	stSSCTRL.sensor_mode = MISP_SS_LVDS_6CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;

	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;

	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;	
	//stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;	
	stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;		
	
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

//	stSSCTRL.stDESERIALIZER.fifo_ctrl = MISP_SS_DESFIFO_MEM_RD_ADDR_FOLLOW_WR_EN;

	stSSCTRL.stDESERIALIZER.ref_h_size = 1962 ; // 1957 + 5(garbage)
	stSSCTRL.stDESERIALIZER.ref_v_size = 1110 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 15;	// 1110 + 15 = 1125			
										
	#if defined(__USE_25Hz_SENSOR__)
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 264;		//220 * 1.2 = 264
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 220;
	#endif
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
void SetSS_deserializer_imx385_s8(void)
{
	UARTprintf("IMX385 sLVDS CH8\r\n");
	const WORD h_tot=GetSS_SensorVoutPAL()?2640:2200;
	// const WORD h_act=1952;
	const WORD h_off=40;  //8ch:40, 4ch:20
	
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;
	stSSCTRL.sensor_mode = MISP_SS_LVDS_8CH ;

	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
#if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#endif
#else											// edk & ref board
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;	
	//stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;	
	stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;	

	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_LINE3:
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 1 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 1 ;  
			stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 1 ;
			stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;
			stSSCTRL.stDESERIALIZER.fifo_ctrl = 0;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY_DOL ;			
			break;  //DOL 3
		case MISP_WDR_PROC_LINE2:
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 1 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 1 ;
			stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;
			stSSCTRL.stDESERIALIZER.fifo_ctrl = 0;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY_DOL ;
			break;  //DOL 2
		case MISP_WDR_PROC_DISBL:
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			#if defined(__USE_SENSOR_BAYER_10BIT__)
				stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 1 ;
				stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;
				stSSCTRL.stDESERIALIZER.fifo_ctrl = MISP_SS_DESFIFO_MEM_RD_ADDR_FOLLOW_WR_EN;
			#else
				stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 0 ;
				stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
				stSSCTRL.stDESERIALIZER.fifo_ctrl = 0;
			#endif	
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
			break; //WDR Disable
		default: //N/A
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;  
			stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 0 ;
			stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
			break;  //N/A
	}

	//clock conversion (10bit DOL mode)
	
	stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 0 ;
	stSSCTRL.stDESERIALIZER.des_conv_blk_size = 0 ;
	
	SetSS_ClkConvSetup(stSSCTRL.stDESERIALIZER.des_clk_conv_enb,h_tot,2200,stSSCTRL.stDESERIALIZER.des_conv_blk_size,h_off);
	
#if defined(__USE_SENS_HSYNC_EXT__)
	stSSCTRL.stDESERIALIZER.ext_hactive = 4;  //extend h-active 1945->1953 (or 1305->1313)
#endif

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 976 ; 	// active size is 1952(not 1948!!) last 4pixel is garbage data. 
	stSSCTRL.stDESERIALIZER.ref_v_size = 1110 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 15;	// 1110 + 15 = 1125
	
	#if defined(__USE_25Hz_SENSOR__)	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 681 ;
	#else
	//	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 248 ;	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 252 ;
	#endif
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetSS_deserializer_imx385_ttl(void)
{
	UARTprintf("IMX385 CSI2TTL\r\n");
	const WORD h_tot=GetSS_SensorVoutPAL()?2640:2200;
	// const WORD h_act=1952;
	const WORD h_off=40;  //8ch:40, 4ch:20
	
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_TTL_INPUT ;
	stSSCTRL.sensor_mode = 0 ;

	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
#if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#endif
#else											// edk & ref board
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;	
	//stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;	
	stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;	

	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_LINE3:
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 1 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 1 ;  
			stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 1 ;
			stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;
			stSSCTRL.stDESERIALIZER.fifo_ctrl = 0;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY_DOL ;			
			break;  //DOL 3
		case MISP_WDR_PROC_LINE2:
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 1 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 1 ;
			stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;
			stSSCTRL.stDESERIALIZER.fifo_ctrl = 0;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY_DOL ;
			break;  //DOL 2
		case MISP_WDR_PROC_DISBL:
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			#if defined(__USE_SENSOR_BAYER_10BIT__)
				stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 1 ;
				stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;
				stSSCTRL.stDESERIALIZER.fifo_ctrl = MISP_SS_DESFIFO_MEM_RD_ADDR_FOLLOW_WR_EN;
			#else
				stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 0 ;
				stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
				stSSCTRL.stDESERIALIZER.fifo_ctrl = 0;
			#endif	
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
			break; //WDR Disable
		default: //N/A
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;  
			stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 0 ;
			stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
			break;  //N/A
	}

	//clock conversion (10bit DOL mode)
	
	stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 0 ;
	stSSCTRL.stDESERIALIZER.des_conv_blk_size = 0 ;
	
	SetSS_ClkConvSetup(stSSCTRL.stDESERIALIZER.des_clk_conv_enb,h_tot,2200,stSSCTRL.stDESERIALIZER.des_conv_blk_size,h_off);
	
#if defined(__USE_SENS_HSYNC_EXT__)
	stSSCTRL.stDESERIALIZER.ext_hactive = 4;  //extend h-active 1945->1953 (or 1305->1313)
#endif

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 976 ; 	// active size is 1952(not 1948!!) last 4pixel is garbage data. 
	stSSCTRL.stDESERIALIZER.ref_v_size = 1110 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 15;	// 1110 + 15 = 1125
	
	#if defined(__USE_25Hz_SENSOR__)	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 681 ;
	#else
	//	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 248 ;	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 252 ;
	#endif
}

#endif


#if defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx290_s4(void)
{
	const WORD h_tot=GetSS_SensorVoutPAL()?2640:2200;
	
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.sensor_mode = MISP_SS_SONY_LVDS_4CH ;
#else 												// MDIN-i51X (i510, i540, i550)
  #if defined(__USE_CMOS_IMX290_LVDS_S2__)	
	stSSCTRL.sensor_mode = MISP_SS_LVDS_2CH ;
  #elif defined(__USE_CMOS_IMX290_LVDS_S4__)	
	stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;
  #endif
  	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
  #if defined(__USE_SENSOR_BAYER_10BIT__)
	stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 1 ;
	stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 0 ;		// clock conversion enable
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;
  #else
	stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 0 ;
	stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 0 ;		// clock conversion enable
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;	
  #endif
	stSSCTRL.stDESERIALIZER.des_conv_blk_size = 233 ;	// r_4k_conv_blk_size = real H blank size - 20
	
	SetSS_ClkConvSetup(stSSCTRL.stDESERIALIZER.des_clk_conv_enb,h_tot,2200,stSSCTRL.stDESERIALIZER.des_conv_blk_size,40);
		
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
  #if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #endif
#else											// edk & ref board
	#if defined(__USE_MDIN_i510__)				// i510
		#if defined(__MDIN_i510_EDK_V10__)								// 510 edk v1.0(jumper)
			stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 3 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 2 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 7 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 6 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
		#elif defined(__MDIN_i510_IMX290__)								// 510 + imx290 oneboard
			stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
		#else															// 510 ref, edk v1.1
			stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 3 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 2 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 0 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 7 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 6 ;
		#endif
	#else										// i540, i550
		stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
	#endif
#endif
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
//	stSSCTRL.stDESERIALIZER.ss_slave_H = 2640 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;	
	stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_LINE3:
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 1 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 1 ; 
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY_DOL ;
			break;  //DOL 3
		case MISP_WDR_PROC_LINE2:
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 1 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY_DOL ;
			break;  //DOL 2
		case MISP_WDR_PROC_DISBL:
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
			break;
		default:
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;  
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
			break;  //N/A
	}
  #if defined(__USE_SENS_HSYNC_EXT__)
	stSSCTRL.stDESERIALIZER.ext_hactive = 4;  //extend h-active 1945->1953 (or 1305->1313)
  #endif
#else 																							// i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 3 ;
#endif

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 1948 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1110 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 15;	// 1110 + 15 = 1125

	#if defined(__USE_25Hz_SENSOR__)	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 692 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 252 ;
	#endif
}
#endif



#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
#if defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx290_s8(void)
{
	const WORD h_tot=GetSS_SensorVoutPAL()?2640:2200;
	// const WORD h_act=1952;
	const WORD h_off=40;  //8ch:40, 4ch:20
	
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;
	stSSCTRL.sensor_mode = MISP_SS_LVDS_8CH ;

	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
#if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#endif
#else											// edk & ref board
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;	
	//stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;	
	stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;	

	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_LINE3:
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 1 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 1 ;  
			stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 1 ;
			stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;
			stSSCTRL.stDESERIALIZER.fifo_ctrl = 0;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY_DOL ;			
			break;  //DOL 3
		case MISP_WDR_PROC_LINE2:
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 1 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 1 ;
			stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;
			stSSCTRL.stDESERIALIZER.fifo_ctrl = 0;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY_DOL ;
			break;  //DOL 2
		case MISP_WDR_PROC_DISBL:
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
			#if defined(__USE_SENSOR_BAYER_10BIT__)
				stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 1 ;
				stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;
				stSSCTRL.stDESERIALIZER.fifo_ctrl = MISP_SS_DESFIFO_MEM_RD_ADDR_FOLLOW_WR_EN;
			#else
				stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 0 ;
				stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
				stSSCTRL.stDESERIALIZER.fifo_ctrl = 0;
			#endif	
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
			break; //WDR Disable
		default: //N/A
			stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
			stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;  
			stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 0 ;
			stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
			stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;
			break;  //N/A
	}

	//clock conversion (10bit DOL mode)
	if(stSSCTRL.stDESERIALIZER.sensor_bayer_10bit) {
		if(GetWDR_ProcesMode()==MISP_WDR_PROC_DISBL)		stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 0 ;	
		else		stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 1 ;			
		// stSSCTRL.stDESERIALIZER.des_conv_blk_size = h_tot-h_act-h_off;
		
	}
	else {
		stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 0 ;
		// stSSCTRL.stDESERIALIZER.des_conv_blk_size = 0 ;
	}
	
	stSSCTRL.stDESERIALIZER.des_conv_blk_size = 210;
	
	SetSS_ClkConvSetup(stSSCTRL.stDESERIALIZER.des_clk_conv_enb,h_tot,2200,stSSCTRL.stDESERIALIZER.des_conv_blk_size,h_off);
	
#if defined(__USE_SENS_HSYNC_EXT__)
	stSSCTRL.stDESERIALIZER.ext_hactive = 4;  //extend h-active 1945->1953 (or 1305->1313)
#endif

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 976 ; 	// active size is 1952(not 1948!!) last 4pixel is garbage data. 
	stSSCTRL.stDESERIALIZER.ref_v_size = 1110 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 15;	// 1110 + 15 = 1125
	
	#if defined(__USE_25Hz_SENSOR__)	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 681 ;
	#else
	//	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 248 ;	
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 252 ;
	#endif
}
#endif
#endif



#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
#if defined(__USE_CMOS_IMX226__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx226_s4(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;
	stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;

	#if defined(__USE_IMX226_CLK_CONV__)
		stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 1 ;		// clock conversion enable
		stSSCTRL.stDESERIALIZER.des_conv_blk_size = 80 ;	// r_4k_conv_blk_size = real H blank size - (20*2)
	#endif

	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
	#if defined(__USE_REF_SENSOR_BOARD__)
		stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
	  #else
		stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 2 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 3 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 4 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 5 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 6 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 7 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 0 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 1 ;
	#endif
#else											// edk & ref board
	#if defined (__ECOM1_UNION_12X__)
		#if defined(__USE_IMX226_1080P60_MODE__)
			stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
		#else
			stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
		#endif
	#else
		#if defined(__USE_IMX226_1080P60_MODE__)
			stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 3 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 2 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 0 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 7 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 6 ;
		#else
			stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
			stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
		#endif
	#endif
#endif
		
	stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;

#if defined(__USE_IMX226_1080P60_MODE__)	
	//use 72Mhz clock to sensor( SCLKOUT : SetCLK_UserPLL(1,3,64,3,4) )
	stSSCTRL.stDESERIALIZER.ss_slave_H = 2112 ;		 //4224 : 29.97...fps, 2112 : 59.940059..fps
	stSSCTRL.stDESERIALIZER.ss_slave_V = 2275 ;
	//use 74.25Mhz clock to sensor( SCLKOUT : SetCLK_UserPLL(1,3,66,3,4) )
//	stSSCTRL.stDESERIALIZER.ss_slave_H = 2200 ;		 	//2200 : 60hz, 4400 : 30hz
//	stSSCTRL.stDESERIALIZER.ss_slave_V = 2250 ;
	
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

//	stSSCTRL.stDESERIALIZER.fifo_ctrl = MISP_SS_DESFIFO_MEM_RD_ADDR_FOLLOW_WR_EN;

	stSSCTRL.stDESERIALIZER.ref_h_size = 2124 ;		
	stSSCTRL.stDESERIALIZER.ref_v_size = 1098 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 39;	// 2275/2 - 1098
	
	#if defined(__USE_25Hz_SENSOR__)
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 788 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 692 ;
	#endif
#else
	stSSCTRL.stDESERIALIZER.ss_slave_H = 8736 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 2200 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

//	stSSCTRL.stDESERIALIZER.fifo_ctrl = MISP_SS_DESFIFO_MEM_RD_ADDR_FOLLOW_WR_EN;

	stSSCTRL.stDESERIALIZER.ref_h_size = 4248 ;		
	stSSCTRL.stDESERIALIZER.ref_v_size = 2182 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 17;	// 2182 + 17 = 2199
	
	#if defined(__USE_25Hz_SENSOR__)
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 788 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 788 ;
	#endif
#endif

}
#endif
#endif



#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
#if defined(__USE_CMOS_IMX226__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx226_s8(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;
	stSSCTRL.sensor_mode = MISP_SS_LVDS_8CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;

	#if defined(__USE_IMX226_CLK_CONV__)
		stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 1 ;		// clock conversion enable
		// stSSCTRL.stDESERIALIZER.des_conv_blk_size = 80 ;	// H blank size = r_4k_conv_blk_size /2 *(8/6) *2
		#if defined (__USE_IMX226_4K3KP30_MODE0__)
			stSSCTRL.stDESERIALIZER.des_conv_blk_size = 112 ;	// H blank size = r_4k_conv_blk_size /2 *(8/6) *2
		#else
			stSSCTRL.stDESERIALIZER.des_conv_blk_size = 192 ;	// H blank size = r_4k_conv_blk_size /2 *(8/6) *2
		#endif
	#endif

	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
#if defined(__MDIN_i540_REF_V121__) || defined(__MDIN_i550_REF_V121__) || defined(__MDIN_i540_4K60__)
	stSSCTRL.stDESERIALIZER.sel_ch_port = 6;
#else
	stSSCTRL.stDESERIALIZER.sel_ch_port = 0;
#endif

	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#else											// edk & ref board
	#if defined(__ECOM1_UNION_12X__)
		stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
	#else
		stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
		stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
	#endif
#endif

#if !defined (__USE_IMX226_4K3KP30_MODE0__)
	stSSCTRL.stDESERIALIZER.rd_lock_cnt_max_sel = 1;
#endif

#if !defined(__USE_CMOS_AUX_SYNC_OUT__)
	stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
#else
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;	// Use AUX sync until i5x0 revision for Sensor Slave Mode
#endif
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
	// stSSCTRL.stDESERIALIZER.ss_slave_H = 4368 ;
	// stSSCTRL.stDESERIALIZER.ss_slave_V = 2200 ;
#if defined (__USE_IMX226_4K3KP30_MODE0__)
	stSSCTRL.stDESERIALIZER.ss_slave_H = 4400 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 3120 ;
#else
#if defined(__USE_ISP_SCLK_OUT__)	// LVDS Clock Rate = 297MHz
	stSSCTRL.stDESERIALIZER.ss_slave_H = 4400 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 2250 ;
#else								// LVDS Clock Rate = 288MHz
	stSSCTRL.stDESERIALIZER.ss_slave_H = 4400 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 2200 ;
#endif
#endif
	
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.fifo_ctrl = MISP_SS_DESFIFO_MEM_RD_ADDR_FOLLOW_WR_EN;

	stSSCTRL.stDESERIALIZER.ref_h_size = 2124 ;		// 8ch mode has 1/2 counter(4248/2)
//	stSSCTRL.stDESERIALIZER.ref_h_size = 4248 ;		// 8ch mode has 1/2 counter(4248/2)
	stSSCTRL.stDESERIALIZER.ref_v_size = 2182 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 17+1;	// 2182 + 17 = 2199 then + 1 = 2200(vtotal)
													
	
	#if defined(__USE_25Hz_SENSOR__)
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 945 ;		//788 * 1.2 = 945.6
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 788 ;
	#endif

}

#if defined(__USE_IMX226_2160P60_MODE4__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx226_s6(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;
	stSSCTRL.sensor_mode = MISP_SS_LVDS_6CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;

	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;

	stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#if defined(__USE_ISP_SCLK_OUT__)	// LVDS Clock Rate = 297MHz
	stSSCTRL.stDESERIALIZER.ss_slave_H = 2200 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 2250 ;
#else								// LVDS Clock Rate = 288MHz
	stSSCTRL.stDESERIALIZER.ss_slave_H = 2184 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 2200 ;
#endif
	
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

//	stSSCTRL.stDESERIALIZER.fifo_ctrl = MISP_SS_DESFIFO_MEM_RD_ADDR_FOLLOW_WR_EN;

	stSSCTRL.stDESERIALIZER.ref_h_size = 1980;
	stSSCTRL.stDESERIALIZER.ref_v_size = 2182 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 17+1;	// 2182 + 17 = 2199 then + 1 = 2200(vtotal)
													
	#if defined(__USE_25Hz_SENSOR__)
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 264;		//220 * 1.2 = 264
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 220;
	#endif
}
#endif	// defined(__USE_IMX226_2160P60_MODE4__)

#endif
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
#if defined(__USE_CMOS_IMX274__)
//--------------------------------------------------------------------------------------------------------------------------
void	SetSS_deserializer_imx274_s4(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;					//0x560
	stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;	//Mode 2					//0x560		
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;			//0x562[12]~


	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 2 ;	
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0;			//stSSCTRL.stDESERIALIZER.sel_ch_port = 1;	//L¬ç LVDS CH 0?8? ??¬ï VSYNC u??? ¬ï!!!! L¬ï ??? O? ??: ?w? ?: ??.
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;							//0x561

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 4 ;					
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 3 ;					//2  
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;					//4  
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;					
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;					
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 7 ;					
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;   				
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 5 ;   				

#if !defined(__USE_CMOS_AUX_SYNC_OUT__)
	stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
#else
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;	// Use AUX sync until i5x0 revision for Sensor Slave Mode
#endif
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;							//0x56D


//	stSSCTRL.stDESERIALIZER.ss_slave_H = 4284;		// for 29.94hz
//	stSSCTRL.stDESERIALIZER.ss_slave_H = 4161;		//2774 x 3 / 2 (for 4ch) = 
#if 1
//	stSSCTRL.stDESERIALIZER.ss_slave_H = 2142;		// for 59.94hz with 72Mhz OSC
//	stSSCTRL.stDESERIALIZER.ss_slave_V = 2310;
	
	stSSCTRL.stDESERIALIZER.ss_slave_H = 2250;		// for 59.94hz
	stSSCTRL.stDESERIALIZER.ss_slave_V = 2200;		// see also ae.c AECTRL_SensUpInit(____)
#else
	stSSCTRL.stDESERIALIZER.ss_slave_H = 2200;		// for 59.94hz
	stSSCTRL.stDESERIALIZER.ss_slave_V = 2250;		// see also ae.c AECTRL_SensUpInit(____)
	stSSCTRL.stDESERIALIZER.fifo_ctrl = MISP_SS_DESFIFO_MEM_RD_ADDR_FOLLOW_WR_OTHERS_EN;
#endif

	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 1 ;	
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 1 ;	
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;	
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;	
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;	
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;	
	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;


	stSSCTRL.stDESERIALIZER.ref_h_size = 1932;		//  2774 - 1932 = 842
	stSSCTRL.stDESERIALIZER.ref_v_size = 1102 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 53 ;	// 1102 + 53 = 1155  2310/2XHS = 1155 = Vtotal

	#if defined(__USE_25Hz_SENSOR__)
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 148 ;		//not exact..
	#else		
//		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 842 ;
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 3615 ;
	#endif


}
//--------------------------------------------------------------------------------------------------------------------------
void	SetSS_deserializer_imx274_s6(void)
{

	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;					//0x560
	stSSCTRL.sensor_mode = MISP_SS_LVDS_6CH ;							//0x560
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;			//0x562[12]~

	#if defined(__USE_SENSOR_BAYER_10BIT__)
		stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 1 ;
		stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
		stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;
//		stSSCTRL.stDESERIALIZER.fifo_ctrl = MISP_SS_DESFIFO_MEM_RD_ADDR_FOLLOW_WR_EN;
	#else
		stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = 0 ;
		stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
		stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	#endif	
	
#if defined (__USE_IMX274_1080P120_MODE__)	// mode3 1080p120, clock conversion

	if(stSSCTRL.stDESERIALIZER.sensor_bayer_10bit) {
		stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 1 ;
		// stSSCTRL.stDESERIALIZER.des_conv_blk_size = h_tot-h_act-h_off;
		
	}
	else {
		stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 0 ;
		// stSSCTRL.stDESERIALIZER.des_conv_blk_size = 0 ;
	}
	
	stSSCTRL.stDESERIALIZER.des_conv_blk_size = 440;
	
	SetSS_ClkConvSetup(stSSCTRL.stDESERIALIZER.des_clk_conv_enb,h_tot,2200,stSSCTRL.stDESERIALIZER.des_conv_blk_size,h_off);
	
#endif

	
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 2 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;							//0x561

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
  	#if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
	#else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
	#endif
#elif defined (__MDIN_i540_4K60__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#else											// edk & ref board
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#endif

#if !defined(__USE_CMOS_AUX_SYNC_OUT__)
	stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
#else
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;	// Use AUX sync until i5x0 revision for Sensor Slave Mode
#endif
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;							//0x56D
//	stSSCTRL.stDESERIALIZER.ss_slave_H = 4224 ;							//PAL=4608?
#if defined(__USE_ISP_SCLK_OUT__)
  #if defined (__USE_IMX274_1080P120_MODE__)	// 1080p120
	stSSCTRL.stDESERIALIZER.ss_slave_H = 1320 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 2250 ;  
  #elif defined(__USE_IMX274_4K15P_MODE__)	
	stSSCTRL.stDESERIALIZER.ss_slave_H = 4400 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 4500 ;  
  #elif defined(__USE_IMX274_2160P60_MODE1__)  
	stSSCTRL.stDESERIALIZER.ss_slave_H = 1100 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 4500 ;  
  #else
	stSSCTRL.stDESERIALIZER.ss_slave_H = 2200 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 4500 ;  
  #endif
#else
  #if defined(__USE_IMX274_2160P60_MODE1__)
	stSSCTRL.stDESERIALIZER.ss_slave_H = 1100 ;
  #elif defined(__USE_IMX274_4K15P_MODE__)	
	stSSCTRL.stDESERIALIZER.ss_slave_H = 4224 ;  
  #else
	stSSCTRL.stDESERIALIZER.ss_slave_H = 2112 ;
  #endif
	stSSCTRL.stDESERIALIZER.ss_slave_V = 4550 ;
#endif
	
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

#if defined(__USE_IMX274_2160P60_MODE1__) && defined(__MDIN_i540_4K60__)
	stSSCTRL.stDESERIALIZER.ref_h_size = 1932 ;
//	stSSCTRL.stDESERIALIZER.ref_h_size = 2124 ;		// 8ch mode has 1/2 counter
	stSSCTRL.stDESERIALIZER.ref_v_size = 2218 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 57 ;	// 2218 + 57 = 2275
#elif defined (__USE_IMX274_1080P120_MODE__)	// mode3 1080p120
	stSSCTRL.stDESERIALIZER.ref_h_size = 1932 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1102 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 23 ;	// 2218 + 57 = 2275	
#else
	stSSCTRL.stDESERIALIZER.ref_h_size = 3864 ;	
//	stSSCTRL.stDESERIALIZER.ref_h_size = 2124 ;		// 8ch mode has 1/2 counter
	stSSCTRL.stDESERIALIZER.ref_v_size = 2218 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 57 ;	// 2218 + 57 = 2275
#endif


	
	#if defined(__USE_IMX274_2160P60_MODE1__) && defined(__MDIN_i540_4K60__)
		#if defined(__USE_25Hz_SENSOR__)
			stSSCTRL.stDESERIALIZER.ref_h_blank_size = 321 ;
		#else
			stSSCTRL.stDESERIALIZER.ref_h_blank_size = 268 ;
		#endif
	#elif defined(__USE_ISP_SCLK_OUT__)
		#if defined(__USE_25Hz_SENSOR__)
			stSSCTRL.stDESERIALIZER.ref_h_blank_size = 518 ;
		#else
			stSSCTRL.stDESERIALIZER.ref_h_blank_size = 536 ;
		#endif
	#else
		#if defined(__USE_25Hz_SENSOR__)
			stSSCTRL.stDESERIALIZER.ref_h_blank_size = 432 ;
		#else
			stSSCTRL.stDESERIALIZER.ref_h_blank_size = 360 ;
		#endif
	#endif
}
#endif

#if defined(__USE_CMOS_IMX326__)
//--------------------------------------------------------------------------------------------------------------------------
void	SetSS_deserializer_imx326_s6(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;					//0x560
	stSSCTRL.sensor_mode = MISP_SS_LVDS_6CH ;							//0x560
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;			//0x562[12]~

//	#if defined(__USE_IMX274_CLK_CONV__)
//		stSSCTRL.stDESERIALIZER.des_clk_conv_enb = 1 ;		// clock conversion enable
//		stSSCTRL.stDESERIALIZER.des_conv_blk_size = 80 ;	// r_4k_conv_blk_size = real H blank size - (20*2)
//	#endif

	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 2 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;							//0x561

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
  	#if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
	#else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
	#endif
#else											// edk & ref board
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#endif

	#if !defined(__USE_CMOS_AUX_SYNC_OUT__)
		stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
	#else
		stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;	// Use AUX sync until i5x0 revision for Sensor Slave Mode
	#endif
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;							//0x56D
	stSSCTRL.stDESERIALIZER.ss_slave_H = 2000 ;	//500(INCK) * 8(CH) / 2(1/2counter) = 2000
	stSSCTRL.stDESERIALIZER.ss_slave_V = 4800 ;	//XVS 4800
	
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;
#if defined(__USE_IMX326_2592x1944_MODE2__)
	stSSCTRL.stDESERIALIZER.ref_h_size = 2616 ;		// See datasheet : Image Data Output Format (Sub-LVDS), // 8ch mode has 1/2 counter
	stSSCTRL.stDESERIALIZER.ref_v_size = 2002 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 398 ;	// 2002 + 398 = 2400
	
	#if defined(__USE_25Hz_SENSOR__)
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 1660 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 1384 ;
	#endif
#else
	stSSCTRL.stDESERIALIZER.ref_h_size = 3096 ;		// See datasheet : Image Data Output Format (Sub-LVDS), // 8ch mode has 1/2 counter
	stSSCTRL.stDESERIALIZER.ref_v_size = 2218 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 182 ;	// 2218 + 182 = 2400
	
	#if defined(__USE_25Hz_SENSOR__)
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 1084 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 904 ;
	#endif
#endif
}
#endif

#endif

#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
#if defined(__USE_CMOS_IMX334__)
void SetSS_deserializer_imx334_s6(void) 
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;					//0x560
	stSSCTRL.sensor_mode = MISP_SS_LVDS_6CH ;							//0x560
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;			//0x562[12]~

	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 2 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	stSSCTRL.stDESERIALIZER.sel_ch_port = 0;

	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;							//0x561

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
	#if defined(__USE_IMX334_SLAVE_MODE__)
		UARTprintf("IMX334 CSI2LVDS 6CH, slave mode\r\n");
		stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
	#else
		UARTprintf("IMX334 CSI2LVDS 6CH, master mode\r\n");	
		stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;							//0x56D
	stSSCTRL.stDESERIALIZER.ss_slave_H = 400 ;	
	stSSCTRL.stDESERIALIZER.ss_slave_V = 2250 ;	
	
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 1 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;
	
	stSSCTRL.stDESERIALIZER.ref_h_size = 3864 ;		
	stSSCTRL.stDESERIALIZER.ref_v_size = 2202 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 48 ;	
	
	#if defined(__USE_25Hz_SENSOR__)
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 644 ;
	#else
		stSSCTRL.stDESERIALIZER.ref_h_blank_size = 536 ; 
	#endif

}
#endif // #if defined(__USE_CMOS_IMX334__)
#endif


#if defined(__USE_CMOS_IMXx36__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx136_ttl(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_TTL_INPUT ;
	stSSCTRL.sensor_mode = 0 ;	

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
		
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
	stSSCTRL.stDESERIALIZER.ss_slave_H = 2200 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;
#else 																							// i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 3 ;
#endif

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;
	stSSCTRL.stDESERIALIZER.ref_h_size = 1952 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1117 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 8;	// 1117 + 8 = 1125
	
#if defined(__USE_25Hz_SENSOR__)
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 688 ;
#else
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 248 ;
#endif
}
#endif



#if defined(__USE_CMOS_IMXx36__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx136_s2(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.sensor_mode = MISP_SS_SONY_LVDS_2CH ;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.sensor_mode = MISP_SS_LVDS_2CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;	
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
  #if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #endif
#else											// edk & ref board
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
		
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
	stSSCTRL.stDESERIALIZER.ss_slave_H = 6600 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 3 ;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;
#endif

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

#if defined(__USE_CMOS_IMXx36_720pReadOut__)
	stSSCTRL.stDESERIALIZER.ref_h_size = 1312 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 741 ;
	#if defined(__USE_25Hz_SENSOR__)
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 688 ;
	#else
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 338 ;
	#endif
#else
	stSSCTRL.stDESERIALIZER.ref_h_size = 1952 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1117 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 8;	// 1117 + 8 = 1125
	
	#if defined(__USE_25Hz_SENSOR__)
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 668 ;
	#else
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 248 ;
	#endif
#endif

}
#endif



#if defined(__USE_CMOS_IMXx36__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_imx136_s4(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.sensor_mode = MISP_SS_SONY_LVDS_4CH ;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_SONY ;	
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
  #if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #endif
#else											// edk & ref board
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 7 ;  // n/a
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 6 ;  // n/a
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 1 ;  // n/a
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 0 ;  // n/a
#endif
	
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif		
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;
#else 																							// i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 3 ;
#endif

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

#if defined(__USE_CMOS_IMXx36_720pReadOut__)
	stSSCTRL.stDESERIALIZER.ref_h_size = 1312 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 741 ;
	#if defined(__USE_25Hz_SENSOR__)
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 668 ;
	#else
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 338 ;
	#endif
#else
	stSSCTRL.stDESERIALIZER.ref_h_size = 1952 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1117 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 8;	// 1117 + 8 = 1125
	
	#if defined(__USE_25Hz_SENSOR__)
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 688 ;
	#else
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 248 ;
	#endif
#endif

}
#endif



#if defined(__USE_CMOS_AR0331__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_ar0331_ttl(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	
	stSSCTRL.sensor_input_type = MISP_SENSOR_TTL_INPUT ;
	stSSCTRL.sensor_mode = 0 ;	

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 2 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#endif
	
	stSSCTRL.stDESERIALIZER.user_SOF = 3072 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2560 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 3584 ;	
	
	stSSCTRL.stDESERIALIZER.ref_h_size = 1964 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1108 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 17;	// 1108 + 17 = 1125
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 236 ;

	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 0;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;
#endif

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;	
}
#endif



#if defined(__USE_CMOS_AR0331__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_ar0331_s2(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.sensor_mode = MISP_SS_LVDS_2CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_APTINA ;
#else 																							// i500
	stSSCTRL.sensor_mode = MISP_SS_APTINA_LVDS_2CH ;
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 2 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#endif

	stSSCTRL.stDESERIALIZER.user_SOF = 3072 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2560 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 3584 ;	
	
	stSSCTRL.stDESERIALIZER.ref_h_size = 1964 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1108 ;	
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 17;	// 1108 + 17 = 1125
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 236 ;

	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_H = 6600 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 0;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;
#endif

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;	
}
#endif



#if defined(__USE_CMOS_AR0331__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_ar0331_s4(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_APTINA ;
#else 																							// i500
	stSSCTRL.sensor_mode = MISP_SS_APTINA_LVDS_4CH ;
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 2 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#endif
	
	stSSCTRL.stDESERIALIZER.user_SOF = 3072 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2560 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 3584 ;	
	
	stSSCTRL.stDESERIALIZER.ref_h_size = 1964 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1108 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 17;	// 1108 + 17 = 1125
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 236 ;

	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 0;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;
#endif

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;	
}
#endif

#if defined(__USE_CMOS_AR0134_TTL__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_ar0134_ttl(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	
	stSSCTRL.sensor_input_type = MISP_SENSOR_TTL_INPUT ;
	stSSCTRL.sensor_mode = 0 ;	

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 2 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#endif
	
	stSSCTRL.stDESERIALIZER.user_SOF = 0 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 0 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 0 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 0 ;	
	
	stSSCTRL.stDESERIALIZER.ref_h_size = 1412 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1028 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 17;	// 1108 + 17 = 1125
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 236 ;

	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 0;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;
#endif

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;	
}
#endif

#if defined(__USE_CMOS_AR0134_LVDS_S2__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_ar0134_s2(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550

	stSSCTRL.sensor_mode = MISP_SS_LVDS_2CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_APTINA ;	
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
	
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
		
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
	stSSCTRL.stDESERIALIZER.ss_slave_H = 6600 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 1 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;

	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;

	stSSCTRL.stDESERIALIZER.user_SOF = 3072 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2560 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 3584 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 1284 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 741 ;
	#if defined(__USE_25Hz_SENSOR__)
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 688 ;
	#else
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 338 ;
	#endif

}
#endif

#if defined(__USE_CMOS_MN34041__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_mn34041_s4(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_PANA ;
#else 																							// i500
	stSSCTRL.sensor_mode = MISP_SS_PANA_LVDS_4CH ;
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 1 ;
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#endif

	stSSCTRL.stDESERIALIZER.user_SOF = 2 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 0 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 1 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 3 ;		

	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif

	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif

	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 0;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;
#endif

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;
	stSSCTRL.stDESERIALIZER.ref_h_size = 2016 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1108 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 17;	// 1108 + 17 = 1125
	
#if defined(__USE_25Hz_SENSOR__)
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 864 ;
#else
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 384 ;
#endif
}
#endif



#if defined(__USE_CMOS_MN34220__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_mn34220_s4(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_PANA ;
#else 																							// i500
	stSSCTRL.sensor_mode = MISP_SS_PANA_LVDS_4CH ;
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
  #if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #else
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 1 ;

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #endif
#else											// edk & ref board
   #if defined(__PANORAMA_SENSOR_SETUP__)
   	#if __PANORAMA_5x0__ // panorama one board 
   	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
	#else
   	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;	
	#endif
   #else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 6 ;
   #endif
#endif
#endif

	stSSCTRL.stDESERIALIZER.user_SOF = 2 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 0 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 1 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 3 ;		

	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif

	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 0;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;
#endif
	
	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;
	stSSCTRL.stDESERIALIZER.ref_h_size = 1956 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1108 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 17;	// 1108 + 17 = 1125
	
#if defined(__USE_25Hz_SENSOR__)	
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 684 ;
#else
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 244 ;
#endif
}
#endif



#if defined(__USE_CMOS_MN34220__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_mn34220_s6(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.sensor_mode = MISP_SS_LVDS_6CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_PANA_WDR ;
#else 																							// i500
	stSSCTRL.sensor_mode = MISP_SS_PANA_LVDS_6CH ;
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
  #if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #else
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 1 ;

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #endif
#else											// edk & ref board
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#endif
#endif
	stSSCTRL.stDESERIALIZER.user_SOF = 2 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 0 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 1 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 3 ;		

	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;  //ADC 10bit
	stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 0;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;
#endif

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;
	stSSCTRL.stDESERIALIZER.ref_h_size = 1956 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1108 ;
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 244 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 17;	// 1108 + 17 = 1125
}
#endif

#if defined(__USE_CMOS_MN34420__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_mn34420_s4(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_PANA ;
#else 																							// i500
	stSSCTRL.sensor_mode = MISP_SS_PANA_LVDS_4CH ;
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
  #if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #else
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 1 ;

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #endif
#else											// edk & ref board
   #if defined(__PANORAMA_SENSOR_SETUP__)
   	#if __PANORAMA_5x0__ // panorama one board 
   	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
	#else
   	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;	
	#endif
   #else
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 7 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 6 ;
   #endif
#endif
#endif

	stSSCTRL.stDESERIALIZER.user_SOF = 2 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 0 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 1 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 3 ;		

#if defined(__USE_MN34420_SLAVE_MODE__)
	stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
#else
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
#endif
	
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif

#if defined(__USE_25Hz_SENSOR__) 
	#if defined(__VTOTAL_CTRL_PAL__)
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 * 1.2 ;
	#else
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 * 1.2 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	#endif
#else
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
#endif
	
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif

	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 0;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;
#endif
	
	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;
	stSSCTRL.stDESERIALIZER.ref_h_size = 1952 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1266 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 17;	// 1108 + 17 = 1125
	
#if defined(__USE_25Hz_SENSOR__)	
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 684 ;
#else
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 244 ;
#endif
}
#endif

#if defined(__USE_CMOS_MN34420__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_mn34420_s6(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.sensor_mode = MISP_SS_LVDS_6CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_PANA_WDR ;
#else 																							// i500
	stSSCTRL.sensor_mode = MISP_SS_PANA_LVDS_6CH ;
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

#if defined(__MDIN_i5XX_FPGA__)					// fpga board
  #if defined(__USE_REF_SENSOR_BOARD__)
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 0 ;

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #else
	stSSCTRL.stDESERIALIZER.sensor_sel_fpga = 1 ;

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
  #endif
#else											// edk & ref board
	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#endif
#endif
	stSSCTRL.stDESERIALIZER.user_SOF = 2 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 0 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 1 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 3 ;		

#if defined(__USE_MN34420_SLAVE_MODE__)
	stSSCTRL.stDESERIALIZER.ss_slave_en = 1 ;
#else
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
#endif
	
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif

//size for wdr
#if defined(__USE_25Hz_SENSOR__) 
	#if defined(__VTOTAL_CTRL_PAL__)
	stSSCTRL.stDESERIALIZER.ss_slave_H = 8800 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 * 1.2 ;
	#else
	stSSCTRL.stDESERIALIZER.ss_slave_H = 8800 * 1.2 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	#endif
#else
	stSSCTRL.stDESERIALIZER.ss_slave_H = 8800 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
//	stSSCTRL.stDESERIALIZER.ss_slave_H = 6600 ;
//	stSSCTRL.stDESERIALIZER.ss_slave_V = 1500 ;
#endif
	
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
//	stSSCTRL.stDESERIALIZER.data_lbit_shift = 2 ;  //ADC 10bit
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 0;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;
#endif

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;
	stSSCTRL.stDESERIALIZER.ref_h_size = 1952 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1266 ;
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 244 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 17;	// 1108 + 17 = 1125
}
#endif

#if defined(__USE_CMOS_EXT2__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_EXT2_ttl(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_TTL_INPUT ;
	stSSCTRL.sensor_mode = 0 ;	

	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
		
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_H = 2200 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;
#else 																							// i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 0;
#endif

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 1644 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 749 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 386;	// 749 + 386 = 1135(?)

#if defined(__USE_25Hz_SENSOR__)
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 688 ;
#else
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 192 ;
#endif
}
#endif



#if defined(__USE_CMOS_EXT1__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_EXT1_ttl(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.sensor_input_type = MISP_SENSOR_TTL_INPUT ;
	stSSCTRL.sensor_mode = 0 ;	
		
	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
	stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 0 ;

	
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_H = 2200 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 0;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;
#endif

	stSSCTRL.stDESERIALIZER.user_SOF = 2912 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 2048 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 2736 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 2512 ;		

	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;

	stSSCTRL.stDESERIALIZER.ref_h_size = 1644 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 749 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 802;	// 749 + 802 = 1551(?)

#if defined(__USE_25Hz_SENSOR__)
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 688 ;
#else
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 192 ;
#endif
}
#endif

#if defined(__USE_CMOS_PS1400K__)
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_PS1400K_s4(void)
{
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
	
	stSSCTRL.sensor_input_type = MISP_SENSOR_LVDS_INPUT ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.sensor_mode = MISP_SS_LVDS_4CH ;
	stSSCTRL.stDESERIALIZER.sync_code_type = MISP_SS_CODE_PANA ;
#else 																							// i500
	stSSCTRL.sensor_mode = MISP_SS_PANA_LVDS_4CH ;
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.des_invert_sync = 0 ;
	#if defined(__MDIN_i510_EDK__)
		stSSCTRL.stDESERIALIZER.sel_ch_port = 3 ;//for i510 socket board jumper
	#else
		stSSCTRL.stDESERIALIZER.sel_ch_port = 0 ;
	#endif
	stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ;

	stSSCTRL.stDESERIALIZER.des_mux_ch0_sel = 0 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch1_sel = 1 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch2_sel = 2 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch3_sel = 3 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch4_sel = 4 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch5_sel = 5 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch6_sel = 6 ;
	stSSCTRL.stDESERIALIZER.des_mux_ch7_sel = 7 ;
#endif


	//panasonic
	stSSCTRL.stDESERIALIZER.user_SOF = 2 ;
	stSSCTRL.stDESERIALIZER.user_SOL = 0 ;
	stSSCTRL.stDESERIALIZER.user_EOF = 1 ;
	stSSCTRL.stDESERIALIZER.user_EOL = 3 ;		

	stSSCTRL.stDESERIALIZER.ss_slave_en = 0 ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.slave_mode_sel = 7;
#endif
	stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;
	stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;
	stSSCTRL.stDESERIALIZER.hd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.vd_edge_polarity = 0 ;
	stSSCTRL.stDESERIALIZER.hd_delay = 0 ;
	stSSCTRL.stDESERIALIZER.hd_high_width = 200 ;
	stSSCTRL.stDESERIALIZER.vd_high_width = 50 ;
	
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stSSCTRL.stDESERIALIZER.ext_trig_h_dly_enb = 0 ;
#endif
	stSSCTRL.stDESERIALIZER.data_lch_shift = 0 ;
	stSSCTRL.stDESERIALIZER.data_lbit_shift = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_en = 0 ;
	stSSCTRL.stDESERIALIZER.wdr_info_bit_sel = 0 ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	stSSCTRL.stDESERIALIZER.wdr_info_sel = 0;
#else 												// MDIN-i51X (i510, i540, i550)
	stSSCTRL.stDESERIALIZER.dol_wdr_enb = 0 ;
	stSSCTRL.stDESERIALIZER.dol_f_mode = 0 ;
	stSSCTRL.stDESERIALIZER.ext_hactive = 0;
#endif
	
	stSSCTRL.stDESERIALIZER.error_clear_flag = 0 ;
	stSSCTRL.stDESERIALIZER.ref_h_size = 1956 ;
	stSSCTRL.stDESERIALIZER.ref_v_size = 1108 ;
	stSSCTRL.stDESERIALIZER.ref_v_blank_size = 17;	// 1108 + 17 = 1125
	
#if defined(__USE_25Hz_SENSOR__)	
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 684 ;
#else
	stSSCTRL.stDESERIALIZER.ref_h_blank_size = 244 ;
#endif
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
void SetSS_deserializer_slave_PAL(BYTE en)
{ // PAL or NTSC setting for sensor slave mode
	WORD ss_slave_H ;
	WORD ss_slave_V ;
	
	if ( en == 1 ) // PAL setting slave sync total count 
	{
#if defined(__VTOTAL_CTRL_PAL__) || defined(__USE_IMX226_1080P60_MODE__)
		ss_slave_V = stSSCTRL.stDESERIALIZER.ss_slave_V * 1.2f ;
		ss_slave_H = stSSCTRL.stDESERIALIZER.ss_slave_H ;
#else
		ss_slave_V = stSSCTRL.stDESERIALIZER.ss_slave_V ;
		ss_slave_H = stSSCTRL.stDESERIALIZER.ss_slave_H * 1.2f ;
#endif	
	}
	else // Slave sync total NTSC setting
	{
		ss_slave_V = stSSCTRL.stDESERIALIZER.ss_slave_V ;
		ss_slave_H = stSSCTRL.stDESERIALIZER.ss_slave_H ;
	}	

	if ( (stSSCTRL.stDESERIALIZER.ss_slave_en==1) ) // For slave mode clock conversion
		SetSS_ClkConvSetup(stSSCTRL.stDESERIALIZER.des_clk_conv_enb,ss_slave_H,stSSCTRL.stDESERIALIZER.ss_slave_H,stSSCTRL.stDESERIALIZER.des_conv_blk_size,40);
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x56B, ss_slave_V);	// V_cycle
	MISP_RegWrite(MISP_LOCAL_ID, 0x56A, ss_slave_H);	// h_cycle


		UARTprintf("*****0x56A (nID: %d)  *****\r\n",ss_slave_H);
	
}
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_bayer_data_order(BYTE val)
{
	stSSCTRL.ctrl |= (SSCTRL_DATA_ORDER_UPDATE | SSCTRL_TRIGGER_ISP);

	stSSCTRL.bayer_data_order = val ;
		
	switch (val) {
		case MISP_SS_BAYER_PATTERN_0 : stSSCTRL.stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_0 ; break ;
		case MISP_SS_BAYER_PATTERN_1 : stSSCTRL.stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_1 ; break ;
		case MISP_SS_BAYER_PATTERN_2 : stSSCTRL.stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_2 ; break ;
		case MISP_SS_BAYER_PATTERN_3 : stSSCTRL.stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_3 ; break ;
		default : stSSCTRL.stTESTPATTERN.tp_pix_id = MISP_SS_TP_BAYER_PATTERN_1 ; break ;	
	}	
	
	SetODM_Bayer_data_order(val) ;
	SetBY_Bayer_data_order(val) ;
	SetNR_Bayer_data_order((MISP_SS_BAYER_PATTERN_t)val) ;
	SetDM_Bayer_data_order(val) ;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetSS_image_start_point(WORD valH , WORD valV)
{
	stSSCTRL.startH = valH;
	stSSCTRL.startV = valV;
	stSSCTRL.ctrl |= (SSCTRL_START_POINT_UPDATE | SSCTRL_TRIGGER_ISP);
}
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_image_size(BYTE bH , BYTE bV, WORD valH , WORD valV)
{
	stSSCTRL.bH = bH;
	stSSCTRL.bV = bV;
	stSSCTRL.active_h_size = valH;
	stSSCTRL.active_v_size = valV;	
	
	SetSS_WDRSWimage_size(bH,bV,valH,valV) ;
	
	SetBY_image_size(bH,bV,valH,valV) ;
	SetNR_image_size(bH,bV,valH,valV) ;
	SetDM_image_size(bH,bV,valH,valV) ;	
	
	stSSCTRL.ctrl |= (SSCTRL_IMAGE_SIZE_UPDATE | SSCTRL_TRIGGER_ISP);
}
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_image_panning(WORD valH , WORD valV)
{
#if defined(__USE_SENS_PANNING_MODE__)
	stSSCTRL.startH = valH;
	stSSCTRL.startV = valV;
#endif	
}
//--------------------------------------------------------------------------------------------------------------------------
BOOL SS_image_panning_update(void)
{
#if defined(__USE_SENS_PANNING_MODE__)
	if((stSSCTRL.startH == stSSCTRL.prev_startH) && (stSSCTRL.startV == stSSCTRL.prev_startV))		return 0;
	else{
		MISP_RegWrite(MISP_LOCAL_ID, 0x4A0, stSSCTRL.startH );	
		MISP_RegWrite(MISP_LOCAL_ID, 0x4A1, stSSCTRL.startV );

		stSSCTRL.prev_startH = stSSCTRL.startH ;
		stSSCTRL.prev_startV = stSSCTRL.startV ;
		return 1;
	}
#else
		return 0;
#endif
}
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_WDRSWimage_size(BYTE bH , BYTE bV, WORD valH , WORD valV)
{
	stSSCTRL.stWDR_MEM_WRITE.in_hsize = (bH<<1) + valH ;
	stSSCTRL.stWDR_MEM_WRITE.in_vsize = (bV<<1) + valV ;
	
	stSSCTRL.ctrl |= (SSCTRL_WDR_MEM_WRITE_UPDATE | SSCTRL_TRIGGER_ISP);
}



//--------------------------------------------------------------------------------------------------------------------------
void SetSS_binning_enable(BOOL val)
{
	stSSCTRL.stBINNING.binning_en = val ;
	stSSCTRL.ctrl |= (SSCTRL_BINNING_UPDATE | SSCTRL_TRIGGER_ISP);


			

	if( (stSSCTRL.stBINNING.binning_en == 1) ) {		
		if(GetSS_image_start_pos_h()==0) {
//		if ( stSSCTRL.bH == 16 ) {
			// SetSS_image_size( (stSSCTRL.bH-1), stSSCTRL.bV , stSSCTRL.active_h_size, stSSCTRL.active_v_size);
			// SetSS_image_start_point( (stSSCTRL.startH+2) , (stSSCTRL.startV));
			SetSS_black_level_sync_mode(0) ;			
		}
	}
	else {// if ( (stSSCTRL.stBINNING.binning_en == 0)){
		if(GetSS_image_start_pos_h()==0) {
//		if ( stSSCTRL.bH == 16 ) {
			// SetSS_image_size( (stSSCTRL.bH+1), stSSCTRL.bV , stSSCTRL.active_h_size, stSSCTRL.active_v_size);
			// SetSS_image_start_point( (stSSCTRL.startH-2) , (stSSCTRL.startV));
			SetSS_black_level_sync_mode(1) ;
		}		
	}

	
}
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_decomp_enable(BOOL val)
{
	stSSCTRL.stHTM_DECOMP.decomp_en = val ;

	stSSCTRL.ctrl |= (SSCTRL_HTM_DECOMP_EN_UPDATE | SSCTRL_TRIGGER_ISP);
}
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_reset_des_fifo(void)
{
	stSSCTRL.ctrl |= (SSCTRL_RESET_DES_FIFO);
	
	SSCTRL_ResetDeserializerFIFO(&stSSCTRL) ;
}

void SetSS_HBlankSize(void)
{
	stSSCTRL.ctrl |= (SSCTRL_RESET_DES_FIFO);
	
	SSCTRL_SetSensorHBlankSize(&stSSCTRL) ;
}
//-------------------------------------------------------------------------------------------------------------------------
WORD GetSS_stDESERIALIZER_ext_hactive(void)
{
	return stSSCTRL.stDESERIALIZER.ext_hactive;
}
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_sensor_bayer_10bit(BOOL val)
{
	stSSCTRL.stDESERIALIZER.sensor_bayer_10bit = val ;		// enable 10bit = 1, 12bit = 0
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
}
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_data_lbit_shift(BYTE val)
{
	stSSCTRL.stDESERIALIZER.data_lbit_shift = val ;		//move left..
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);
}
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_black_level_type(BOOL val)
{
	stSSCTRL.stBLACK_LEVEL.type = val;
	stSSCTRL.ctrl |= (SSCTRL_BLACK_LEVEL_VALUE_UPDATE | SSCTRL_TRIGGER_ISP);
}
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_SensorVoutFrmt(BYTE val)	//jins 2015.01.12
{
	stSSCTRL.sensor_vout_frmt = val;
}
//--------------------------------------------------------------------------------------------------------------------------
void SetSS_SensorSlaveModeSel(BYTE val)	
{
	stSSCTRL.stDESERIALIZER.slave_mode_sel = val;
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);	
}
//--------------------------------------------------------------------------------------------------------------------------
BYTE GetSS_SensorSlaveModeSel(void)
{
	return stSSCTRL.stDESERIALIZER.slave_mode_sel;
}
//--------------------------------------------------------------------------------------------------------------------------
WORD GetSS_SensorSlave_Vsize(void)
{
	return stSSCTRL.stDESERIALIZER.ss_slave_V;
}
//--------------------------------------------------------------------------------------------------------------------------
BYTE GetSS_black_level_type(void)  //jins 2015.12.01
{
	return stSSCTRL.stBLACK_LEVEL.type;
}
//--------------------------------------------------------------------------------------------------------------------------
BOOL GetSS_sensor_bit_opertion(void)
{
	return stSSCTRL.sensor_bit_operation ;
}
//--------------------------------------------------------------------------------------------------------------------------
BYTE GetSS_black_level_mode(void)
{
	return stSSCTRL.stBLACK_LEVEL.mode;
}
//--------------------------------------------------------------------------------------------------------------------------
WORD GetSS_man_black_level_target(void)
{
	return stSSCTRL.stBLACK_LEVEL.manual_blc_val ;
}
//--------------------------------------------------------------------------------------------------------------------------
WORD GetSS_ODM_black_level_target(void)
{
	return stSSCTRL.stBLACK_LEVEL.odm_blc_val;
}
//--------------------------------------------------------------------------------------------------------------------------
WORD GetSS_sensor_hactive_read(void)
{
	return stSSCTRL.stDESERIALIZER.ref_h_size;
}
//--------------------------------------------------------------------------------------------------------------------------
BOOL GetSS_sensor_wdr_info_en(void)
{
	return stSSCTRL.stDESERIALIZER.wdr_info_en;
}
//--------------------------------------------------------------------------------------------------------------------------
WORD GetSS_sensor_vactive_read(void)
{
	return stSSCTRL.stDESERIALIZER.ref_v_size;
}
//--------------------------------------------------------------------------------------------------------------------------
BYTE GetSS_bayer_data_order(void)
{
	return stSSCTRL.bayer_data_order ;
}
//--------------------------------------------------------------------------------------------------------------------------
BYTE GetSS_image_boundary_size_h(void)
{
	return stSSCTRL.bH ;
}
//--------------------------------------------------------------------------------------------------------------------------
BYTE GetSS_image_boundary_size_v(void)
{
	return stSSCTRL.bV ;
}
//--------------------------------------------------------------------------------------------------------------------------
WORD GetSS_image_start_pos_h(void)
{
	return stSSCTRL.startH ;
}
//--------------------------------------------------------------------------------------------------------------------------
WORD GetSS_image_start_pos_v(void)
{
	return stSSCTRL.startV ;
}
//--------------------------------------------------------------------------------------------------------------------------
BYTE GetSS_init_start_pos_h(void)
{
	return stSSCTRL.init_startH ;
}
//--------------------------------------------------------------------------------------------------------------------------
BYTE GetSS_init_start_pos_v(void)
{
	return stSSCTRL.init_startV ;
}
//--------------------------------------------------------------------------------------------------------------------------
WORD GetSS_image_active_size_h(void)
{
	return stSSCTRL.active_h_size ;
}
//--------------------------------------------------------------------------------------------------------------------------
WORD GetSS_image_active_size_v(void)
{
	return stSSCTRL.active_v_size ;
}
//--------------------------------------------------------------------------------------------------------------------------
WORD GetSS_decomp_p1(void)
{
	return stSSCTRL.stHTM_DECOMP.decomp_p1 ;
}

WORD GetSS_decomp_p2(void)
{
	return stSSCTRL.stHTM_DECOMP.decomp_p2 ;
}

WORD GetSS_offset_p2(void)
{
	return stSSCTRL.stHTM_DECOMP.offset_p2 ;
}

WORD GetSS_decomp_p3(void)
{
	return stSSCTRL.stHTM_DECOMP.decomp_p3 ;
}

WORD GetSS_offset_p3(void)
{
	return stSSCTRL.stHTM_DECOMP.offset_p3 ;
}

BYTE GetSS_p1_mul_sel(void)
{
	return stSSCTRL.stHTM_DECOMP.p1_mul_sel ;
}

BYTE GetSS_p2_mul_sel(void)
{
	return stSSCTRL.stHTM_DECOMP.p2_mul_sel ;
}

BYTE GetSS_p3_mul_sel(void)
{
	return stSSCTRL.stHTM_DECOMP.p3_mul_sel ;
}

WORD GetSS_input_max_value(void)
{
	return stSSCTRL.stHTM_DECOMP.input_max_value ;
}

WORD GetSS_bl_pre_value(void)
{
	return stSSCTRL.stHTM_DECOMP.bl_pre_value ;
}

WORD GetSS_bl_post_value(void)
{
	return stSSCTRL.stHTM_DECOMP.bl_post_value ;
}

BYTE GetSS_SensorDoutFrmt(void)
{
	return stSSCTRL.sensor_input_type;		// 0:ttl, 1:lvds
}

BYTE GetSS_SensorVoutFrmt(void)
{
	return stSSCTRL.sensor_vout_frmt;
}

BOOL GetSS_SensorVoutPAL(void)	//0:ntsc, 1:pal system
{
	if(	(GetSS_SensorVoutFps()==25)||(GetSS_SensorVoutFps()==50)||(GetSS_SensorVoutFps()==100)||(GetSS_SensorVoutFps()==200))
		return TRUE;   //PAL
	else
		return FALSE;  //NTSC
}

WORD GetSS_SensorVoutFps(void)
{
	return (WORD)(defMISPSrcVideo[stSSCTRL.sensor_vout_frmt].rate);
}

BOOL GetSS_binning_enable(void)
{
	return stSSCTRL.stBINNING.binning_en;
}


//------------------------------------------Sensor Test Pattern---------------------------------------------------------------
void SetSS_stp_mode(WORD val)
{
	
	if(val<12){
		stSSCTRL.stTESTPATTERN.l_tp_pattern_mode = 0; 
		stSSCTRL.stTESTPATTERN.s1_tp_pattern_mode = 0; 
		stSSCTRL.stTESTPATTERN.s2_tp_pattern_mode = 0; 
		stSSCTRL.stTESTPATTERN.s3_tp_pattern_mode = 0; 
		
		stSSCTRL.stTESTPATTERN.l_tp_pattern_sel1 = val+1;
		stSSCTRL.stTESTPATTERN.s1_tp_pattern_sel1 = val+1;
		stSSCTRL.stTESTPATTERN.s2_tp_pattern_sel1 = val+1;
		stSSCTRL.stTESTPATTERN.s3_tp_pattern_sel1 = val+1;				
	}
	else if(val<21){
		stSSCTRL.stTESTPATTERN.l_tp_pattern_mode = 1;	
		stSSCTRL.stTESTPATTERN.s1_tp_pattern_mode = 1;	
		stSSCTRL.stTESTPATTERN.s2_tp_pattern_mode = 1;	
		stSSCTRL.stTESTPATTERN.s3_tp_pattern_mode = 1;	
		
		stSSCTRL.stTESTPATTERN.l_tp_pattern_sel2 = val-12;
		stSSCTRL.stTESTPATTERN.s1_tp_pattern_sel2 = val-12;
		stSSCTRL.stTESTPATTERN.s2_tp_pattern_sel2 = val-12;
		stSSCTRL.stTESTPATTERN.s3_tp_pattern_sel2 = val-12;
	}
	else if(val<35){
		stSSCTRL.stTESTPATTERN.l_tp_pattern_mode = 2;	
		stSSCTRL.stTESTPATTERN.s1_tp_pattern_mode = 2; 
		stSSCTRL.stTESTPATTERN.s2_tp_pattern_mode = 2; 
		stSSCTRL.stTESTPATTERN.s3_tp_pattern_mode = 2; 
		
		stSSCTRL.stTESTPATTERN.l_tp_pattern_sel3 = val-21;
		stSSCTRL.stTESTPATTERN.s1_tp_pattern_sel3 = val-21;
		stSSCTRL.stTESTPATTERN.s2_tp_pattern_sel3 = val-21;
		stSSCTRL.stTESTPATTERN.s3_tp_pattern_sel3 = val-21;
	}
	else if(val<51){
		stSSCTRL.stTESTPATTERN.l_tp_pattern_mode = 3; 
		stSSCTRL.stTESTPATTERN.s1_tp_pattern_mode = 3; 
		stSSCTRL.stTESTPATTERN.s2_tp_pattern_mode = 3; 
		stSSCTRL.stTESTPATTERN.s3_tp_pattern_mode = 3; 
		
		stSSCTRL.stTESTPATTERN.l_tp_pattern_sel4 = val-35;
		stSSCTRL.stTESTPATTERN.s1_tp_pattern_sel4 = val-35;
		stSSCTRL.stTESTPATTERN.s2_tp_pattern_sel4 = val-35;
		stSSCTRL.stTESTPATTERN.s3_tp_pattern_sel4 = val-35;	
	}

}
void SetSS_stp_onoff(BOOL OnOff)
{
	stSSCTRL.stTESTPATTERN.l_tp_enable=OnOff;

	stSSCTRL.stTESTPATTERN.s1_tp_enable=OnOff;
	stSSCTRL.stTESTPATTERN.s2_tp_enable=OnOff;
	stSSCTRL.stTESTPATTERN.s3_tp_enable=OnOff;	
}
void SetSS_stp_imbue(void)
{
	//Fill testpattern data to all of sensor active region. use reset, default, initial
	SetSS_stp_xpos(0);
	SetSS_stp_ypos(0);
	#if defined(__USE_IMX226_2160P30_MODE0__)||defined(__USE_IMX226_2160P30_MODE1__)
	SetSS_stp_xsize(4100);	
	#else
	SetSS_stp_xsize(GetSS_sensor_hactive_read());	
	#endif
	SetSS_stp_ysize(GetSS_sensor_vactive_read());	

	//Fill testpattern data active region(ex FHD:1920x1080) use reset, default, initial
//	SetSS_stp_xpos(GetSS_image_start_pos_h);
//	SetSS_stp_ypos(GetSS_image_start_pos_v);
//	SetSS_stp_xsize(GetSS_image_active_size_h());	
//	SetSS_stp_ysize(GetSS_image_active_size_v());		
}
void SetSS_stp_xpos(WORD val)
{
	stSSCTRL.ctrl |= (SSCTRL_TP_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.stTESTPATTERN.tp_hstart=val;
}
void SetSS_stp_ypos(WORD val)
{
	stSSCTRL.ctrl |= (SSCTRL_TP_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.stTESTPATTERN.tp_vstart=val;
}
void SetSS_stp_xsize(WORD val)
{
	stSSCTRL.ctrl |= (SSCTRL_TP_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.stTESTPATTERN.tp_hsize=val;
}
void SetSS_stp_ysize(WORD val)
{
	stSSCTRL.ctrl |= (SSCTRL_TP_UPDATE | SSCTRL_TRIGGER_ISP);
	stSSCTRL.stTESTPATTERN.tp_vsize=val;
}

BYTE GetSS_stp_pix_id(void)
{
	return stSSCTRL.stTESTPATTERN.tp_pix_id ;
}
BOOL GetSS_stp_onoff(void)
{
	return stSSCTRL.stTESTPATTERN.l_tp_enable;
}
//--------------------------------------------------------------------------------------------------------------------------

void SetSS_virtual_sync_mode(BOOL si_sync_en , BOOL si_sync_lock , BOOL si_check , WORD si_htotal , WORD si_vtotal , WORD si_hact , WORD si_vact )
{
#if __SS_SI_SETUP__	
	stSSCTRL.ctrl |= (SSCTRL_SI_UPDATE | SSCTRL_TRIGGER_ISP);
	
	stSSCTRL.stSYNC_INTEGRITY.si_en = si_check ;
	stSSCTRL.stSYNC_INTEGRITY.si_sync_gen_en = si_sync_en ;
	stSSCTRL.stSYNC_INTEGRITY.si_sync_lock = si_sync_lock ;
	stSSCTRL.stSYNC_INTEGRITY.si_sync_htotal = si_htotal ;
	stSSCTRL.stSYNC_INTEGRITY.si_sync_vtotal = si_vtotal ;
	
	stSSCTRL.stSYNC_INTEGRITY.si_sync_hfall = si_hact ;
	stSSCTRL.stSYNC_INTEGRITY.si_sync_vfall = si_vact ;		
#endif	
}

void SetSS_virtual_sync_4K ( void )
{
#if __SS_SI_SETUP__		
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	SetSS_virtual_sync_mode(1,0,0,4368,2200,4096+214,2160+22) ;
	SetSS_image_start_point(0,0) ;
	SetSS_image_size(0,0,4096,2160) ;
	SetDDRMAP_image_size(0,0,4096,2160) ;
	
	SetIN_MainFrmt(MISP_VIDSRC_4096x2160p24);
	SetFC_MainFCsrcPosiX(0); 	SetFC_MainFCsrcSizeH(GetIN_MainSrcSizeH());
	SetFC_MainFCsrcPosiY(0); 	SetFC_MainFCsrcSizeV(GetIN_MainSrcSizeV());
	SetDDRMAP_main_size(4096,2160) ;
	
	SetOUT_MainFrmt(MISP_VIDOUT_4096x2160p24) ;
	SetFC_MainFCdstPosiX(0); 	SetFC_MainFCdstSizeH(GetOUT_MainSizeHA());
	SetFC_MainFCdstPosiY(0); 	SetFC_MainFCdstSizeV(GetOUT_MainSizeVA());
	
	if(GetIN_AuxPath() == MISP_AUXPATH_MAIN_O) 	{ 					// 'main output' --> 'aux input'
		SetIN_AuxFrmt(MISP_VIDOUT_4096x2160p24); 						// set aux input frmt
		SetFC_AuxFCsrcPosiX(0); 	SetFC_AuxFCsrcSizeH(GetIN_AuxSrcSizeH());
		SetFC_AuxFCsrcPosiY(0); 	SetFC_AuxFCsrcSizeV(GetIN_AuxSrcSizeV());
		SetDDRMAP_aux_size(4096,2160) ;
	}
	
	MISP_RegField(MISP_LOCAL_ID, 0x561, 2 ,2 , 1); // bayer map mode : 0 : TTL 1 : SI 2:  in2sentop 3: lvds
		
	SetBY_gamma_enable(0) ; // Gamma off
	MISP_RegField(MISP_LOCAL_ID , 0x473 , 0 , 1 , 1 ) ;	// CC disable
	MISP_RegField(MISP_LOCAL_ID , 0x47C , 15 , 1 , 0 ) ; // Demosaic False color off
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x234, 0x0001) ; // Sensor tp ON
	MISP_RegWrite(MISP_LOCAL_ID, 0x230, 4096) ; // Sensor sensor_in_tp_hsize
	MISP_RegWrite(MISP_LOCAL_ID, 0x231, 2160) ; // Sensor sensor_in_tp_vsize
	MISP_RegWrite(MISP_LOCAL_ID, 0x232, 0) ; // Sensor sensor_in_tp_hstart
	MISP_RegWrite(MISP_LOCAL_ID, 0x233, 0) ; // Sensor sensor_in_tp_vstart
	
	MISP_RegField(MISP_LOCAL_ID, 0x238, 12, 3, 2) ; // Sensor sensor_in_tp_sel1 => cross	
#endif	//#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
#endif	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetSS_ResetSensor(BYTE nID) {  //reset sensor to change fps
#if defined(__USE_CMOS_MN34220__)
	MN34220_ChangeFps_Normal();
#elif defined(__USE_CMOS_MN34420__)
	#if defined(__USE_MN34420_SLAVE_MODE__)		//see also stMN34420_SetRate() function
		#if defined(__VTOTAL_CTRL_PAL__)	
			if((GetSS_SensorVoutFps()==25)||(GetSS_SensorVoutFps()==50))
					stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 * 1.2 ;
			else		stSSCTRL.stDESERIALIZER.ss_slave_V = 1125 ;	
		#else
			if((GetSS_SensorVoutFps()==25)||(GetSS_SensorVoutFps()==50))
					stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 * 1.2 ;
			else		stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;	
			
		#endif

		SetSS_deserializer_slave_PAL(GetSS_SensorVoutPAL()) ;	
	#endif
	
		MN34420_ChangeFps_Normal();	

#elif defined(__USE_CMOS_IMXx36__)
	IMX136_ChangeFps_Normal();
#elif defined(__USE_CMOS_IMX124__)
	IMX124_ChangeFps_Normal();
#elif defined(__USE_CMOS_IMX174__) || defined(__USE_CMOS_IMX249__)
	IMX174_ChangeFps_Normal(nID);
#elif defined(__USE_CMOS_IMX185__)
	IMX185_ChangeFps_Normal();
#elif defined(__USE_CMOS_IMXx22__) || defined(__USE_CMOS_IMX323__) 
	IMX222_ChangeFps_Normal();
#elif defined(__USE_CMOS_IMX238__)
	IMX238_ChangeFps_Normal();
#elif defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__)
	IMX290_ChangeFps_Normal(nID);
#elif defined(__USE_CMOS_IMX385__)
	IMX385_ChangeFps_Normal(nID);
#elif defined(__USE_CMOS_IMX252__)
	#if defined(__USE_IMX252_SLAVE_MODE__)
		//slave mode sensor configuration
		// slave source clk [LVDS serial clock]
		if((GetSS_SensorVoutFps()==25) || (GetSS_SensorVoutFps()==30)) 	stSSCTRL.stDESERIALIZER.ss_slave_H = 6600 ;
		else		stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;	
		SetSS_deserializer_slave_PAL(GetSS_SensorVoutPAL()) ;
	#else
		//master mode sensor configuration
		IMX252_ChangeFps_Normal(nID);
	#endif
#elif defined(__USE_CMOS_IMX265__)
	//slave mode and trigger(both master and slave ) sensor configuration 
	#if defined(__USE_ISP_SCLK_OUT__)
		// slave source clk [LVDS serial clock]
		#if defined(__USE_IMX265_1536P_ALL_SCAN_MODE__)
			if((GetSS_SensorVoutFps()==25) || (GetSS_SensorVoutFps()==30)) 	stSSCTRL.stDESERIALIZER.ss_slave_H = 6000 ;
			else		stSSCTRL.stDESERIALIZER.ss_slave_H = 3000 ;	

			stSSCTRL.stDESERIALIZER.ss_slave_V=1650;
		#else
			if((GetSS_SensorVoutFps()==25) || (GetSS_SensorVoutFps()==30)) 	stSSCTRL.stDESERIALIZER.ss_slave_H = 6600 ;
			else		stSSCTRL.stDESERIALIZER.ss_slave_H = 3300 ;	

			stSSCTRL.stDESERIALIZER.ss_slave_V=1125;			
		#endif
	#else
		// slave source clk [xtal 27MHz]
		if((GetSS_SensorVoutFps()==25) || (GetSS_SensorVoutFps()==30)) 	stSSCTRL.stDESERIALIZER.ss_slave_H = 800 ;
		else		stSSCTRL.stDESERIALIZER.ss_slave_H = 400 ;		
	#endif

	SetSS_deserializer_slave_PAL(GetSS_SensorVoutPAL()) ;
		
	#if !defined(__USE_IMX265_SLAVE_MODE__)
		//master mode sensor configuration
		IMX265_ChangeFps_Normal(nID);
	#endif
#elif defined(__USE_CMOS_IMX253__)
	#if defined(__USE_IMX253_SLAVE_MODE__)

	#else
		//master mode sensor configuration
		IMX253_ChangeFps_Normal();
	#endif	
#elif defined(__USE_CMOS_IMX273__)
	#if defined(__USE_IMX273_SLAVE_MODE__)
		// slave source clk [LVDS serial clock]
		#if defined(__USE_IMX273_ALLSCAN_MODE__)
				if((GetSS_SensorVoutFps()==25) ||(GetSS_SensorVoutFps()==30))	stSSCTRL.stDESERIALIZER.ss_slave_H=6600;	
				else if((GetSS_SensorVoutFps()==100) ||(GetSS_SensorVoutFps()==120))		stSSCTRL.stDESERIALIZER.ss_slave_H=1650;					
				else		stSSCTRL.stDESERIALIZER.ss_slave_H=3300;	
		
				stSSCTRL.stDESERIALIZER.ss_slave_V=1500;
				stSSCTRL.stDESERIALIZER.des_clk_conv_enb=0;
		#elif defined(__USE_IMX273_ROI_MODE__)
			if(GetSS_SensorVoutFps()==240){		
				stSSCTRL.stDESERIALIZER.ss_slave_H=1650;		stSSCTRL.stDESERIALIZER.ss_slave_V=750;	
				stSSCTRL.stDESERIALIZER.des_clk_conv_enb=1;		
				SetSS_image_start_point(88,12);				
			}		
			else if(GetSS_SensorVoutFps()==200){	
				stSSCTRL.stDESERIALIZER.ss_slave_H=1500;		stSSCTRL.stDESERIALIZER.ss_slave_V=825;	
				stSSCTRL.stDESERIALIZER.des_clk_conv_enb=1;			
				SetSS_image_start_point(88,12+30);						
			}
			else	{
				if((GetSS_SensorVoutFps()==25) ||(GetSS_SensorVoutFps()==30))	stSSCTRL.stDESERIALIZER.ss_slave_H=12000;		
				else if((GetSS_SensorVoutFps()==100) ||(GetSS_SensorVoutFps()==120))		stSSCTRL.stDESERIALIZER.ss_slave_H=3000;	
				else 	stSSCTRL.stDESERIALIZER.ss_slave_H=6000;			//50hz 60hz

				stSSCTRL.stDESERIALIZER.ss_slave_V=825;	
				stSSCTRL.stDESERIALIZER.des_clk_conv_enb=0;				
				SetSS_image_start_point(88,12+30);						
			}		
		#else		
				if((GetSS_SensorVoutFps()==25) ||(GetSS_SensorVoutFps()==30))	stSSCTRL.stDESERIALIZER.ss_slave_H=9000;		
				else if((GetSS_SensorVoutFps()==100) ||(GetSS_SensorVoutFps()==120))		stSSCTRL.stDESERIALIZER.ss_slave_H=2250;	
				else 	stSSCTRL.stDESERIALIZER.ss_slave_H=4500;			//50hz 60hz

				stSSCTRL.stDESERIALIZER.ss_slave_V=1100;	
				stSSCTRL.stDESERIALIZER.des_clk_conv_enb=0;				
		#endif	
			
		UARTprintf("***** __USE_IMX273_SLAVE_MODE__ sensor fps change (ss_slave_H: %d)  *****\r\n",stSSCTRL.stDESERIALIZER.ss_slave_H);
		UARTprintf("***** __USE_IMX273_SLAVE_MODE__ sensor fps change (nID: %d)  *****\r\n",nID);
		
		SetSS_deserializer_slave_PAL(GetSS_SensorVoutPAL()) ;
	#else
		// trigger(master) sensor configuration 
		#if defined(__USE_IMX273_ALLSCAN_MODE__)
				if((GetSS_SensorVoutFps()==25) ||(GetSS_SensorVoutFps()==30))	stSSCTRL.stDESERIALIZER.ss_slave_H=6600;	
				else if((GetSS_SensorVoutFps()==100) ||(GetSS_SensorVoutFps()==120))		stSSCTRL.stDESERIALIZER.ss_slave_H=1650;					
				else		stSSCTRL.stDESERIALIZER.ss_slave_H=3300;	
				
				stSSCTRL.stDESERIALIZER.ss_slave_V=1500;
		#elif defined(__USE_IMX273_ROI_MODE__)
			if(GetSS_SensorVoutFps()==240){		
				stSSCTRL.stDESERIALIZER.ss_slave_H=1650;		stSSCTRL.stDESERIALIZER.ss_slave_V=750;	
			}		
			else if(GetSS_SensorVoutFps()==200){	
				stSSCTRL.stDESERIALIZER.ss_slave_H=1500;		stSSCTRL.stDESERIALIZER.ss_slave_V=825;					
			}
			else	{
				if((GetSS_SensorVoutFps()==25) ||(GetSS_SensorVoutFps()==30))	stSSCTRL.stDESERIALIZER.ss_slave_H=12000;		
				else if((GetSS_SensorVoutFps()==100) ||(GetSS_SensorVoutFps()==120))		stSSCTRL.stDESERIALIZER.ss_slave_H=3000;	
				else 	stSSCTRL.stDESERIALIZER.ss_slave_H=6000;			//50hz 60hz

				stSSCTRL.stDESERIALIZER.ss_slave_V=825;	
			}		
		#else		
				if((GetSS_SensorVoutFps()==25) ||(GetSS_SensorVoutFps()==30))	stSSCTRL.stDESERIALIZER.ss_slave_H=9000;		
				else if((GetSS_SensorVoutFps()==100) ||(GetSS_SensorVoutFps()==120))		stSSCTRL.stDESERIALIZER.ss_slave_H=2250;	
				else 	stSSCTRL.stDESERIALIZER.ss_slave_H=4500;			//50hz 60hz

				stSSCTRL.stDESERIALIZER.ss_slave_V=1100;				
		#endif	

		// master mode sensor configuration
		#if defined(__USE_IMX273_ROI_MODE__)		
			if(GetSS_SensorVoutFps()==240){
				stSSCTRL.stDESERIALIZER.des_clk_conv_enb=1;
				stSSCTRL.stDESERIALIZER.des_conv_blk_size = 600 ;	// 1800 (lvds clock) : 2400 - 600 = 1800(pixel clock)		
				SetSS_image_start_point(88,12);
				SetSS_ClkConvSetup(stSSCTRL.stDESERIALIZER.des_clk_conv_enb,1200,stSSCTRL.stDESERIALIZER.des_conv_blk_size,0,40);	
			}
			else if(GetSS_SensorVoutFps()==200){
				stSSCTRL.stDESERIALIZER.des_clk_conv_enb=1;
				stSSCTRL.stDESERIALIZER.des_conv_blk_size = 500 ;	// 1500 (lvds clock) : 2000 - 500 = 1500(pixel clock)
				SetSS_image_start_point(88,12+30);				
				SetSS_ClkConvSetup(stSSCTRL.stDESERIALIZER.des_clk_conv_enb,1000,stSSCTRL.stDESERIALIZER.des_conv_blk_size,0,40);	
			}
			else{
				stSSCTRL.stDESERIALIZER.des_clk_conv_enb=0;
				SetSS_image_start_point(88,12+30);					
				SetSS_ClkConvSetup(stSSCTRL.stDESERIALIZER.des_clk_conv_enb,2400,1800,stSSCTRL.stDESERIALIZER.des_conv_blk_size,40);
			}
		#endif

			SetSS_deserializer_slave_PAL(GetSS_SensorVoutPAL()) ;		
			IMX273_ChangeFps_Normal(nID);

	#endif		
#elif defined(__USE_CMOS_IMX226__)
	//slave mode sensor configuration
	#if defined(__USE_IMX226_1080P60_MODE__)
		if((GetSS_SensorVoutFps()==25) ||(GetSS_SensorVoutFps()==30))	stSSCTRL.stDESERIALIZER.ss_slave_H=4224;		
		else																stSSCTRL.stDESERIALIZER.ss_slave_H=2112;		
	#elif defined(__USE_IMX226_2160P60_MODE4__) && defined(__USE_ISP_SCLK_OUT__)
		if((GetSS_SensorVoutFps()==25) ||(GetSS_SensorVoutFps()==30))	stSSCTRL.stDESERIALIZER.ss_slave_H=4400;		
		else																stSSCTRL.stDESERIALIZER.ss_slave_H=2200;		
	#elif defined(__USE_IMX226_2160P60_MODE4__) && !defined(__USE_ISP_SCLK_OUT__)
		if((GetSS_SensorVoutFps()==25) ||(GetSS_SensorVoutFps()==30))	stSSCTRL.stDESERIALIZER.ss_slave_H=4368;		
		else																stSSCTRL.stDESERIALIZER.ss_slave_H=2184;		
	#endif
	
	SetSS_deserializer_slave_PAL(GetSS_SensorVoutPAL()) ;
#elif defined(__USE_CMOS_IMX274__)
	//slave mode sensor configuration
	// if(GetSS_SensorVoutFps()==25)
		// MISP_RegWrite(MISP_LOCAL_ID, 0x56B, 5460) ;		// v-total
	// else
		// MISP_RegWrite(MISP_LOCAL_ID, 0x56B, 4550) ;		// v-total
	SetSS_deserializer_slave_PAL(GetSS_SensorVoutPAL()) ;
#elif defined(__USE_CMOS_IMX326__)
	//slave mode sensor configuration
	// if(GetSS_SensorVoutFps()==25)
		// MISP_RegWrite(MISP_LOCAL_ID, 0x56B, 5460) ;		// v-total
	// else
		// MISP_RegWrite(MISP_LOCAL_ID, 0x56B, 4550) ;		// v-total
	SetSS_deserializer_slave_PAL(GetSS_SensorVoutPAL()) ;
#elif defined(__USE_CMOS_IMX334__)
	#if defined(__USE_IMX334_SLAVE_MODE__)
		SetSS_deserializer_slave_PAL(GetSS_SensorVoutPAL()) ;
	#else
		IMX334_ChangeFps_Normal(nID);
	#endif
#endif

	Delay_mSec(40);  //little delay before deserializer fifo reset
	SetSS_reset_des_fifo();  //reset deserializer fifo
	SetSS_HBlankSize();
}

void SetSS_deserializer_slave_en(BOOL OnOff)
{
	stSSCTRL.stDESERIALIZER.ss_slave_en = OnOff;
	MISP_RegWrite(MISP_LOCAL_ID, 0x56C, (stSSCTRL.stDESERIALIZER.ss_slave_en)<<15| (stSSCTRL.stDESERIALIZER.vd_edge_polarity)<<13|(stSSCTRL.stDESERIALIZER.hd_edge_polarity)<<12|(stSSCTRL.stDESERIALIZER.hd_delay));
}
//--------------------------------------------------------------------------------------------------------------------------
BOOL GetSS_deserializer_slave_en(void)
{
	return stSSCTRL.stDESERIALIZER.ss_slave_en ;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE GetSS_mw_wdr_proc_mode(void)
{
	return stSSCTRL.stWDR_MEM_WRITE.proc_mode;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetSS_lvds_in_delay(BOOL invert, BYTE delay)
{
	stSSCTRL.lvds_in_inv = invert;
	stSSCTRL.lvds_in_delay = delay;
	
	stSSCTRL.ctrl |= (SSCTRL_LVDS_IN_DLY_UPDATE | SSCTRL_TRIGGER_ISP);
}

//--------------------------------------------------------------------------------------------------------------------------
void StableClkConvSens(void)
{
#if defined(__USE_CMOS_IMX226__) 
	WORD BLC_Hsync_rd;
	WORD BLC_Vsync_rd;	

#if defined(__USE_IMX226_CLK_MATCH_SCLK_)
	WORD spll_p, spll_n, spll_m, spll_od, spll_s;
#endif
	
	
	MISP_RegRead(MISP_LOCAL_ID, 0x4AC, &BLC_Hsync_rd);
	MISP_RegRead(MISP_LOCAL_ID, 0x4AD, &BLC_Vsync_rd);

	if((BLC_Hsync_rd==4248)&&(BLC_Vsync_rd==2182))	return;

	if((BLC_Hsync_rd==4248)&&(BLC_Vsync_rd==2183)){
		
		#if __StableClkConvSens_DEBUG_PRINT__		//for debug
		UARTprintf("***** Waiting Stable (Vsync: %d)  *****\r\n",BLC_Vsync_rd);
		#endif	//__StableClkConvSens_DEBUG_PRINT__		

		return;
	}


	if(ClkConv_roof<6) 	ClkConv_roof++;
	else if(ClkConv_roof<16){
		SetSS_reset_des_fifo();	
		ClkConv_roof++;

		#if __StableClkConvSens_DEBUG_PRINT__		//for debug
		UARTprintf("***** FIFO RST H: %d   V: %d  *****\r\n",BLC_Hsync_rd,BLC_Vsync_rd);
		#else
		Delay_mSec(2);	
		#endif	//__StableClkConvSens_DEBUG_PRINT__		
	}
	else	{
#if defined(__USE_IMX226_CLK_MATCH_SCLK_)
		// sensor pll = org freq / 4
		GetCLK_UserPLL(&spll_p, &spll_n, &spll_m, &spll_od, &spll_s);
		SetCLK_UserPLL(spll_p, spll_n, spll_m, spll_od, spll_s*4);
#endif
		
		ClkConv_roof=6;	

		#if __StableClkConvSens_DEBUG_PRINT__		//for debug
		UARTprintf("***** PLL RST H: %d   V: %d  *****\r\n",BLC_Hsync_rd,BLC_Vsync_rd);
		#else
		Delay_mSec(2);			
		#endif	//__StableClkConvSens_DEBUG_PRINT__		
	}
	
#endif	//__USE_CMOS_IMX226__

}

void SetSS_ClkConvSetup(BYTE en , WORD tgt_h_tot, WORD ref_h_tot, WORD ref_h_blank, WORD ref_h_offset )
{
////=======================================================================================//
//	en : Clock conversion enable flag
//	tgt_h_tot : Target H total size
//	ref_h_tot : Current H total size
//	ref_h_blank : Current H blank size ( Register value )
//	ref_h_offset : H blank offset value
////=======================================================================================//	
	WORD conv_blank_size ;
	
	if ( en == 1 ) conv_blank_size = tgt_h_tot - (ref_h_tot -(ref_h_blank+ref_h_offset)) - ref_h_offset ;
	else conv_blank_size = ref_h_blank ;		
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x574, (en<<12)|(conv_blank_size));
}

void SetSS_deserializer_Ext2IPP(BOOL en)
{
	if ( en == 1 ) 
	{
		SetCLK_SrcSensor(0) ; // Set sensor clk to ext clk : Select ¡®01¡¯ if system ext clk is linked to CLK_A
		// [¡®00¡¯ : CLK_B is used] : HDMI CLK
		// [¡®01¡¯ : CLK_A is used] : TTL CLK
		// [¡®10¡¯ : refer to stISPCLK.clk_lvds_sel] // clk_lvds_int
		// [¡®11¡¯ : LVDS serial clk divider clk is used] : See stISPCLK.clk_sensor_in.clk_divider		
		stSSCTRL.stDESERIALIZER.sensor_mapmode = 2 ; // Ext2IPP			
	}
	else {
		if ( stSSCTRL.sensor_input_type == MISP_SENSOR_LVDS_INPUT )
		{
			SetCLK_SrcSensor(3) ;
			stSSCTRL.stDESERIALIZER.sensor_mapmode = 3 ; // LVDS
		}
		else
		{
			SetCLK_SrcSensor(1) ;
			stSSCTRL.stDESERIALIZER.sensor_mapmode = 0 ; // TTL
		}
	}
	
	stSSCTRL.ctrl |= (SSCTRL_DESERIALIZER_UPDATE | SSCTRL_TRIGGER_ISP);	
}

void SetSS_Ext2IPP(BOOL en)
{
	static WORD startH ;
	static WORD startV ;
	static WORD active_h_size ;
	static WORD active_v_size ;
	
	static BYTE bH ;
	static BYTE bV ;
	
	static BYTE blc_mode ;
	static WORD blc_man_target ;
	static WORD blc_ODM_target ;	
	
	if ( en == 1 )
	{
		startH = GetSS_image_start_pos_h() ;
		startV = GetSS_image_start_pos_v() ;	

		bH = GetSS_image_boundary_size_h() ;
		bV = GetSS_image_boundary_size_v() ;	
		active_h_size = GetSS_image_active_size_h() ;	
		active_v_size = GetSS_image_active_size_v() ;	
		blc_mode = GetSS_black_level_mode() ;
		blc_man_target = GetSS_man_black_level_target() ;
		blc_ODM_target = GetSS_ODM_black_level_target() ;
		
		SetSS_image_start_point(0,0);
		SetSS_image_size(0,0,GetIN_MainSrcSizeH(),GetIN_MainSrcSizeV());
		SetSS_black_level_mode(0) ;
		SetSS_man_black_level_target(0);
		SetSS_ODM_black_level_target(0);	
	}
	else if ( (stSSCTRL.stDESERIALIZER.sensor_mapmode == 2) && (en == 0) )
	{
		SetSS_image_start_point(startH,startV);
		SetSS_image_size(bH,bV,active_h_size,active_v_size);
		SetSS_black_level_mode(blc_mode) ;
		SetSS_man_black_level_target(blc_man_target);
		SetSS_ODM_black_level_target(blc_ODM_target);	
	}
	
	SetSS_deserializer_Ext2IPP(en) ;
}

#if defined(__USE_4K60_MD1__)
void Check_FPGA_OB(void)
{
	//Check LCMX03LF Optical Black Value for power on boot
	WORD OB_black_level_val;
	

	if(stSSCTRL.stBLACK_LEVEL.Init_OBCheck_OK == 1)		return;

	MISP_RegRead(MISP_LOCAL_ID, 0x4AE, &OB_black_level_val);
	OB_black_level_val = (0x0FFF & OB_black_level_val);
	
#if defined(__USE_4K60_MD1__) 
	if(OB_black_level_val<196){

		SENSOR_RSTN(LOW);
		Delay_mSec(1);
		SENSOR_RSTN(HIGH);

		CMOS_Initialize();

		UARTprintf("***** Retry Sensor Reset *****\r\n");		
	}
#endif

	stSSCTRL.stBLACK_LEVEL.Init_OBCheck_OK = 1;


}
#endif

//--------------------------------------------------------------------------------------------------------------------------


/*  FILE_END_HERE */
