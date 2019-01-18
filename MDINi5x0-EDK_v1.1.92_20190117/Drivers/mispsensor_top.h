//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  SENSOR_TOP.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISP_SENSOR_TOP_H__
#define		__MISP_SENSOR_TOP_H__

#if	defined(__USE_MDIN_i500__)		// MDIN-i500
#define __SS_SI_SETUP__ 0
#else								// MDIN-i51X (i510, i540, i550)
#define __SS_SI_SETUP__ 1
#endif
#if defined(__USE_CMOS_IMX226__)
#define __StableClkConvSens_DEBUG_PRINT__ 0 
#endif
// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
typedef	enum {
	MISP_8BIT_OPERATION = 0,		
	MISP_10BIT_OPERATION ,				
}	MISP_BIT_OPERATION_t;

typedef	enum {
	MISP_SENSOR_TTL_INPUT = 0,		
	MISP_SENSOR_LVDS_INPUT ,				
}	MISP_SENSOR_INPUT_t;

typedef	enum {  //"00" wdr disable  "01" line mode  "10" frame mode "11" HTM
	MISP_WDR_MEM_WIRTE_OFF_MODE = 0,
	MISP_WDR_MEM_WIRTE_LINE_MODE,
	MISP_WDR_MEM_WIRTE_FRAME_MODE,
	MISP_WDR_MEM_WIRTE_HTM_MODE,
}	MISP_WDR_MEM_WIRTE_MODE_t;

typedef	enum {			
	MISP_BLACK_LEVEL_MANUAL= 0,
	MISP_BLACK_LEVEL_AVERAGE,
	MISP_BLACK_LEVEL_CHANNEL,	
	MISP_BLACK_LEVEL_OFF 
}	MISP_BLACK_LEVEL_MODE_t;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
typedef	enum {
	MISP_SS_PANA_LVDS_4CH = 0 ,
	MISP_SS_SONY_LVDS_4CH ,
	MISP_SS_SONY_LVDS_2CH ,
	MISP_SS_APTINA_LVDS_4CH ,
	MISP_SS_APTINA_LVDS_2CH ,
	MISP_SS_PANA_LVDS_6CH ,
}	MISP_SENSOR_MODE_t;
#else 												// MDIN-i51X (i510, i540, i550)
typedef	enum {
	MISP_SS_LVDS_4CH = 0 ,
	MISP_SS_LVDS_2CH ,
	MISP_SS_LVDS_6CH ,
	MISP_SS_LVDS_8CH
}	MISP_SENSOR_MODE_t;

typedef	enum {
	MISP_SS_CODE_SONY = 0 ,
	MISP_SS_CODE_PANA ,
	MISP_SS_CODE_SONY_DOL ,
	MISP_SS_CODE_PANA_WDR ,
	MISP_SS_CODE_APTINA
}	MISP_SENSOR_SS_TYPE_t;
#endif

typedef	enum {			
	MISP_SS_BAYER_PATTERN_0 = 0,
	MISP_SS_BAYER_PATTERN_1,
	MISP_SS_BAYER_PATTERN_2,	
	MISP_SS_BAYER_PATTERN_3 
}	MISP_SS_BAYER_PATTERN_t;

typedef	enum {			
	MISP_SS_TP_BAYER_PATTERN_2 = 0,
	MISP_SS_TP_BAYER_PATTERN_0,
	MISP_SS_TP_BAYER_PATTERN_3,	
	MISP_SS_TP_BAYER_PATTERN_1 
}	MISP_SS_TP_BAYER_PATTERN_t;

typedef	enum {
	MISP_SS_DESFIFO_MEM_RD_ADDR_FOLLOW_WR_OTHERS_EN = 1,
	MISP_SS_DESFIFO_MEM_RD_ADDR_FOLLOW_WR_EN = 2,
	MISP_SS_DESFIFO_CE_ONE_LESS_OUT_EN = 4,
	MISP_SS_DESFIFO_CE_ONE_MORE_OUT_EN = 8,
}	MISP_SS_DESFIFO_CONTROL_t;

typedef	struct
{
	BOOL si_en ;
	BOOL si_sync_gen_en ;
	BOOL si_sync_lock ;
	WORD si_sync_htotal ;
	WORD si_sync_vtotal ;
	
	WORD si_sync_hrise ;
	WORD si_sync_vrise ;

	WORD si_sync_hfall ;
	WORD si_sync_vfall ;

} SS_SYNC_INTEGRITY_PARAM, *PSS_SYNC_INTEGRITY_PARAM;

typedef	struct
{
	WORD tp_hstart ;	
	WORD tp_vstart ;	
	WORD tp_hsize ;	
	WORD tp_vsize ;	
	
	BYTE tp_pix_id ;
	
	BOOL l_tp_enable ;			
	BYTE l_tp_pattern_mode ;	
	WORD l_tp_r_value ;	
	WORD l_tp_g_value ;	
	WORD l_tp_b_value ;	
	BYTE l_tp_pattern_sel1 ;	
	BYTE l_tp_pattern_sel2 ;	
	BYTE l_tp_pattern_sel3 ;	
	BYTE l_tp_pattern_sel4 ;

	BOOL s1_tp_enable ;	
	BYTE s1_tp_pattern_mode ;	
	WORD s1_tp_r_value ;	
	WORD s1_tp_g_value ;	
	WORD s1_tp_b_value ;	
	BYTE s1_tp_pattern_sel1 ;	
	BYTE s1_tp_pattern_sel2 ;	
	BYTE s1_tp_pattern_sel3 ;	
	BYTE s1_tp_pattern_sel4 ;

	BOOL s2_tp_enable ;	
	BYTE s2_tp_pattern_mode ;	
	WORD s2_tp_r_value ;	
	WORD s2_tp_g_value ;	
	WORD s2_tp_b_value ;	
	BYTE s2_tp_pattern_sel1 ;	
	BYTE s2_tp_pattern_sel2 ;	
	BYTE s2_tp_pattern_sel3 ;	
	BYTE s2_tp_pattern_sel4 ;

	BOOL s3_tp_enable ;	
	BYTE s3_tp_pattern_mode ;	
	WORD s3_tp_r_value ;	
	WORD s3_tp_g_value ;	
	WORD s3_tp_b_value ;	
	BYTE s3_tp_pattern_sel1 ;	
	BYTE s3_tp_pattern_sel2 ;	
	BYTE s3_tp_pattern_sel3 ;	
	BYTE s3_tp_pattern_sel4 ;	
} SS_TESTPATTERN_PARAM, *PSS_TESTPATTERN_PARAM;

typedef	struct
{
	BOOL blc_enable ;	
	BYTE mode ;	// 0: manual blc 1: avg blc 2:channel blc 3:no blc
	BYTE type ; // 0: use front blc / 1: use odm blc [4B0h]
	BOOL h_sync_cut_en ;	// 1: hsync cut when vactive low
	BYTE sync_mode ;
	BOOL blc_hactend_robust;	//0: h-robust off, 1: h-robust on
	
// sync_mode
// 0 : v-rise&fall can controll (range sensor from hblank falling time to 255 clk after)
// 1 : cycle  v-rise = cycle h-start / cycle  v-rise = cycle h-end
// 2 : active v-rise = active h-rise / active v-fall = cycle  h-end
// 3 : active v-rise = active h-rise / active v-fall = active  h-fall	
	
	BOOL sync_bypass_n ;
	BYTE update_frequency ;	// 0: 1f 1: 2f 2: 4f 3: 8f 4: 16f 5: 32f 6: 64f 7: 128f

	WORD manual_blc_val ;
	WORD odm_blc_val ;
	
	WORD ob_h_pos ;
	WORD ob_v_pos ;
	WORD ob_h_size ;
	WORD ob_v_size ;	

#if defined(__USE_4K60_MD1__)
	BOOL Init_OBCheck_OK;
#endif
} SS_BLACK_LEVEL_PARAM, *PSS_BLACK_LEVEL_PARAM;

typedef	struct
{
	BOOL decomp_en ;
	BYTE output_range_sel ;	  		// 0 : output[11:0] 1 : output[15:4] 2 : output[19:8]	  

	WORD decomp_p1 ;

	WORD decomp_p2 ;
	WORD offset_p2 ;

	WORD decomp_p3 ;
	WORD offset_p3 ;

	BYTE p1_mul_sel	;	// 0: x1 1: x2 2: x4 3: x8 4: x16 5: x32 6: x64
	BYTE p2_mul_sel	;	// 0: x1 1: x2 2: x4 3: x8 4: x16 5: x32 6: x64	
	BYTE p3_mul_sel	;	// 0: x1 1: x2 2: x4 3: x8 4: x16 5: x32 6: x64

	WORD input_max_value ;
	WORD bl_pre_value ;
	WORD bl_post_value ;
} SS_HTM_DECOMP_PARAM, *PSS_HTM_DECOMP_PARAM;

typedef	struct
{
	WORD in_hsize ;
	WORD in_vsize ;
	
	BYTE proc_mode ;	// 0 : off  / 1 : WDR 4f line mode / 2 : WDR 2f frame mode / 3 : HTM 2f mode
	BYTE l_bit_mode ;	// "0" 8bit, "1" 10bit, "2" 9bit, "3" 12bit
	BYTE s1_bit_mode ;	// "0" 8bit, "1" 10bit, "2" 9bit, "3" 12bit
	BYTE s2_bit_mode ;	// "0" 8bit, "1" 10bit, "2" 9bit, "3" 12bit
	BYTE s3_bit_mode ;	// "0" 8bit, "1" 10bit, "2" 9bit, "3" 12bit
	BYTE proc_frame_en ;   // "0011" : 2-frame / "0111" : 3-frame / "1111" : 4-frame
	BYTE wt_bit_scope ;  // "0" map[9:n], "1" map[11:n]
	
	BYTE odm_sync_sel ; // 0: long, 1: s1, 2:s2, 3:s3

} SS_WDR_MEM_WRITE_PARAM, *PSS_WDR_MEM_WRITE_PARAM;

// typedef	struct
// {
	// BOOL dpd_en ;	
// } SS_DEAD_PIX_DETECT_PARAM, *PSS_DEAD_PIX_DETECT_PARAM;

typedef	struct
{
	BOOL binning_en ;
} SS_BINNING_PARAM, *PSS_BINNING_PARAM;

typedef	struct
{
	// LVDS Sync code
	WORD user_SOF ;
	WORD user_SOL ;
	WORD user_EOF ;
	WORD user_EOL ;

	BOOL sensor_bayer_10bit;
	BYTE sensor_sel_fpga;
	BOOL des_invert_sync;
	BYTE sel_ch_port;
	BYTE sensor_mapmode;
	BYTE sync_code_type;

	// Deserializer LVDS channel selection
	BYTE des_mux_ch0_sel;
	BYTE des_mux_ch1_sel;
	BYTE des_mux_ch2_sel;
	BYTE des_mux_ch3_sel;
	BYTE des_mux_ch4_sel;
	BYTE des_mux_ch5_sel;
	BYTE des_mux_ch6_sel;
	BYTE des_mux_ch7_sel;

	// For FIFO control
	BOOL ddio_data_swap;
	BYTE fifo_rd_delay;
	BOOL dout_ctrl;
	BOOL fifo_rst_enb_reg;
	BOOL fifo_ce_rst_ctrl;
	BYTE rd_lock_cnt_max_sel;
	
	// For sensor slave mode
	BOOL ss_slave_en ;
	BYTE slave_mode_sel;
	WORD ss_slave_H ;
	WORD ss_slave_V ;
	BOOL hd_edge_polarity ;	// 0: rising, 1: falling, MN34041 = selectable, IMX136 = falling edge only
	BOOL vd_edge_polarity ;	// 0: rising, 1: falling, MN34041 = selectable, IMX136 = falling edge only
	WORD hd_delay ;	// 0: rising, 1: falling, MN34041 = selectable, IMX136 = falling edge only
	
	WORD hd_high_width ;	// 'high' state on rising edge device, 'low' state on falling edge device (relevant to vd_edge_polarity)
	WORD vd_high_width ;	// 'high' state on rising edge device, 'low' state on falling edge device (relevant to vd_edge_polarity)

	BOOL ext_trig_h_dly_enb;

	WORD sens_trig_accum_val;
	WORD trig_blank_width;
	
	// For error check
	BOOL error_clear_flag ;
	WORD ref_h_size ;
	WORD ref_v_size ;
	WORD ref_h_blank_size;
	WORD ref_v_blank_size;

	// 4K sensor clk conversion control
	BOOL des_clk_conv_enb;
	WORD des_conv_blk_size;
	
	//For SONY frame mode WDR	
	BOOL wdr_info_en ;
	BYTE wdr_info_bit_sel ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	BYTE wdr_info_sel ;	// 3
#else 												// MDIN-i51X (i510, i540, i550)
	// For SONY DOL mode WDR
	BOOL dol_wdr_enb;
	WORD dol_f_mode;
#endif	
	
	//For WDR (sensor lack of hactive)
	BYTE ext_hactive;

	BYTE data_lch_shift ; // 1: Aptina S2
	BYTE data_lbit_shift ;		
	
	BYTE fifo_ctrl;		// fifo control bit for imx226
	
} SS_SENSOR_RECIEVER, *PSS_SENSOR_RECIEVER;

typedef	struct
{
	LONG 	ctrl;			// SS ctrl flag
	
	BOOL	sensor_bit_operation ; // Sensor bit opration information
	BOOL	sensor_input_type;	// 0: TTL input / 1: LVDS input	
	BOOL	lvds_in_inv;		// 0:normal, 1:invert
	BYTE	lvds_in_delay;		// lvds input delay(0~3)
	BYTE	sensor_mode;		// 000: 4ch, 001: 2ch, 010: 6ch, 011: 8ch
	BYTE	sensor_vout_frmt;	// video format of input (one of MISP_SRCVIDEO_FORMAT_t)
	BYTE	bayer_data_order ;
		
	WORD 	startH;			// SS horizontal start point of output image
	WORD	startV;			// SS vertical start point of output image	
	BYTE 	bH;				// SS horizontal boundary of output image
	BYTE	bV;				// SS vertical boundary of output image
	WORD 	active_h_size;	// SS output active horizontal size of input image
	WORD 	active_v_size;	// SS output active vertical size of input image	


	WORD 	prev_startH;			// SS privious horizontal start point of output image 
	WORD	prev_startV;			// SS privious vertical start point of output image		
	WORD 	init_startH;			// SS initial horizontal start point of output image 
	WORD	init_startV;			// SS initial vertical start point of output image	

	
	SS_SENSOR_RECIEVER stDESERIALIZER ;
	SS_TESTPATTERN_PARAM stTESTPATTERN ;	// --
	SS_BLACK_LEVEL_PARAM stBLACK_LEVEL ;
	SS_HTM_DECOMP_PARAM stHTM_DECOMP ;	
	SS_WDR_MEM_WRITE_PARAM stWDR_MEM_WRITE ;
	// SS_DEAD_PIX_DETECT_PARAM stDEAD_PIX_DETECT ;
	SS_BINNING_PARAM stBINNING ;
	
#if __SS_SI_SETUP__	
	SS_SYNC_INTEGRITY_PARAM stSYNC_INTEGRITY ;	
#endif	
	
}	SS_CTRL_PARAM, *PSS_CTRL_PARAM;

#define	SSCTRL_TRIGGER_ISP					0x40000000		// ISP update
#define	SSCTRL_IMAGE_SIZE_UPDATE			0x00000001		// image size update
#define	SSCTRL_START_POINT_UPDATE			0x00000002		// image start point update
#define	SSCTRL_DATA_ORDER_UPDATE			0x00000004		// WDR_MEM_WRITE update
#define	SSCTRL_WDR_MEM_WRITE_UPDATE			0x00000008		// WDR_MEM_WRITE update
#define	SSCTRL_BLACK_LEVEL_UPDATE			0x00000010		// BLACK_LEVEL update
#define	SSCTRL_BLACK_LEVEL_VALUE_UPDATE		0x00000020		// BLACK_LEVEL update
#define	SSCTRL_HTM_DECOMP_EN_UPDATE			0x00000100		// HTM_DECOMP update
#define	SSCTRL_HTM_DECOMP_UPDATE			0x00000200		// HTM_DECOMP update
#define	SSCTRL_BINNING_UPDATE				0x00000400		// BINNING update
#define	SSCTRL_RESET_DES_FIFO				0x00001000		// Reset deserializer fifo
#define	SSCTRL_DESERIALIZER_UPDATE			0x00002000		// Deserializer update
#define	SSCTRL_LVDS_IN_DLY_UPDATE			0x00004000		// LVDS input delay & invert
#define	SSCTRL_TP_UPDATE					0x04000000		// TP update
#define	SSCTRL_SI_UPDATE					0x08000000		// SI update

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Device drive functions

// sensor_top.c

void Init_SensorTop(void);

void SS_ProcessHandler(void); // Sensor_top process handler

// -----------------------------------------------------------------------------
	
void SetODM_Bayer_data_order ( BYTE val ) ; // TEMP declaration	
		
// -----------------------------------------------------------------------------

void SSCTRL_DesErrCorrection(void);

// API Get / Set functions
	
void SetSS_reset_des_fifo(void);
void SetSS_HBlankSize(void);
void SetSS_deserializer_imx124_s4(void);
void SetSS_deserializer_imx124_s8(void);
void SetSS_deserializer_imx252_s4(void);
void SetSS_deserializer_imx265_s4(void);
void SetSS_deserializer_imx174_s4(void);
void SetSS_deserializer_imx249_s2(void);
void SetSS_deserializer_imx253_s8(void);
void SetSS_deserializer_imx273_s2(void);
void SetSS_deserializer_imx273_s4(void);
void SetSS_deserializer_imx273_s8(void);
void SetSS_deserializer_imx178(void);
void SetSS_deserializer_imx185_s4(void);
void SetSS_deserializer_imx222_ttl(void);
void SetSS_deserializer_imx238_s4(void);
void SetSS_deserializer_imx290_s4(void);
void SetSS_deserializer_imx290_s8(void);
void SetSS_deserializer_imx385_s4(void);
void SetSS_deserializer_imx385_s8(void);
void SetSS_deserializer_imx226_s4(void);

void SetSS_deserializer_imx385_ttl(void); // For CSI2 to TTL
void SetSS_deserializer_imx385_s6(void); // For CSI2 to LVDS 6CH

void SetSS_deserializer_imx226_s8(void);
void SetSS_deserializer_imx226_s6(void);
void SetSS_deserializer_imx274_s4(void);
void SetSS_deserializer_imx274_s6(void);
void SetSS_deserializer_imx326_s6(void);
void SetSS_deserializer_imx334_s6(void); // For CSI2 to LVDS 6CH
void SetSS_deserializer_imx136_ttl(void);
void SetSS_deserializer_imx136_s2(void);
void SetSS_deserializer_imx136_s4(void);
void SetSS_deserializer_ar0331_ttl(void);
void SetSS_deserializer_ar0331_s2(void);
void SetSS_deserializer_ar0331_s4(void);
void SetSS_deserializer_ar0134_ttl(void);
void SetSS_deserializer_ar0134_s2(void);
void SetSS_deserializer_mn34041_s4(void);
void SetSS_deserializer_mn34220_s4(void);
void SetSS_deserializer_mn34220_s6(void);
void SetSS_deserializer_mn34420_s4(void);
void SetSS_deserializer_mn34420_s6(void);
void SetSS_deserializer_ext3_ttl(void);
void SetSS_deserializer_EXT2_ttl(void);
void SetSS_deserializer_EXT1_ttl(void);
void SetSS_deserializer_ov2710_ttl(void);
void SetSS_deserializer_PS1400K_s4(void);
void SetSS_deserializer_slave_PAL(BYTE en); // PAL or NTSC setting for sensor slave mode

void SetSS_deserializer_slave_en(BOOL OnOff);	
BOOL GetSS_deserializer_slave_en(void) ;	
	
void SetSS_bayer_data_order(BYTE val);

void SetSS_black_level_mode(BYTE val);
void SetSS_black_level_update_freq(BYTE val);
void SetSS_man_black_level_target(WORD val);
void SetSS_ODM_black_level_target(WORD val);
void SetSS_black_level_sync_mode(BYTE val);
void SetSS_SensorSlaveModeSel(BYTE val);

void SetSS_image_start_point(WORD valH , WORD valV);
void SetSS_image_panning(WORD valH , WORD valV);
BOOL SS_image_panning_update(void);
void SetSS_image_size(BYTE bH , BYTE bV, WORD valH , WORD valV);
void SetSS_WDRSWimage_size(BYTE bH , BYTE bV, WORD valH , WORD valV);
void SetSS_wdr_write_mode(BYTE proc_mode, BYTE bit_mode, BYTE proc_frame_en);
void SetSS_binning_enable(BOOL val); 
void SetSS_decomp_enable(BOOL val);
void SetSS_sensor_bayer_10bit(BOOL val);
void SetSS_data_lbit_shift(BYTE val);
void SetSS_black_level_type(BOOL val);
void SetSS_SensorVoutFrmt(BYTE val);	//jins 2015.01.12
void SetSS_lvds_in_delay(BOOL invert, BYTE delay);
void StableClkConvSens(void);
void SetSS_ClkConvSetup(BYTE en , WORD tgt_h_tot, WORD ref_h_tot, WORD ref_h_blank, WORD ref_h_offset );

#if defined(__USE_4K60_MD1__)
void Check_FPGA_OB(void);
#endif

void SetSS_Ext2IPP(BOOL en) ;

BYTE GetSS_SensorSlaveModeSel(void);
WORD GetSS_SensorSlave_Vsize(void);
WORD GetSS_stDESERIALIZER_ext_hactive(void);
BYTE GetSS_black_level_type(void);  //jins 2015.12.01
BOOL GetSS_sensor_bit_opertion(void);
BYTE GetSS_black_level_mode(void);
WORD GetSS_man_black_level_target(void);
WORD GetSS_ODM_black_level_target(void);
BYTE GetSS_bayer_data_order(void);
BYTE GetSS_image_boundary_size_h(void);
BYTE GetSS_image_boundary_size_v(void);
WORD GetSS_image_start_pos_h(void);
WORD GetSS_image_start_pos_v(void);
WORD GetSS_image_active_size_h(void);
WORD GetSS_image_active_size_v(void);
WORD GetSS_sensor_hactive_read(void);
WORD GetSS_sensor_vactive_read(void);
BOOL GetSS_sensor_wdr_info_en(void);	//jins 2015.06.10
BYTE GetSS_mw_wdr_proc_mode(void);

// Sensor Test pattern
void SetSS_stp_onoff(BOOL OnOff);
void SetSS_stp_mode(WORD val);
void SetSS_stp_xpos(WORD val);
void SetSS_stp_ypos(WORD val);
void SetSS_stp_xsize(WORD val);
void SetSS_stp_ysize(WORD val);
void SetSS_stp_imbue(void);

BYTE GetSS_stp_pix_id(void);
BOOL GetSS_stp_onoff(void);

void SetSS_virtual_sync_mode(BOOL si_sync_en , BOOL si_sync_lock , BOOL si_check , WORD si_htotal , WORD si_vtotal , WORD si_hact , WORD si_vact );
void SetSS_virtual_sync_4K(void) ;

void SetSS_ResetSensor(BYTE nID);

// Get Decomp value
WORD GetSS_stDESERIALIZER_ext_hactive(void);
WORD GetSS_decomp_p1(void);
WORD GetSS_decomp_p2(void);
WORD GetSS_offset_p2(void);
WORD GetSS_decomp_p3(void);
WORD GetSS_offset_p3(void);
BYTE GetSS_p1_mul_sel(void);
BYTE GetSS_p2_mul_sel(void);
BYTE GetSS_p3_mul_sel(void);
WORD GetSS_input_max_value(void);
WORD GetSS_bl_pre_value(void);
WORD GetSS_bl_post_value(void);

BYTE GetSS_SensorDoutFrmt(void);
BYTE GetSS_SensorVoutFrmt(void);
BOOL GetSS_SensorVoutPAL(void);
WORD GetSS_SensorVoutFps(void);
BOOL GetSS_binning_enable(void);

#endif	/* __MISP_SENSOR_TOP_H__ */
