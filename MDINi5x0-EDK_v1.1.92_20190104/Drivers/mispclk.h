//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MISPCLK.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISP_CLK_H__
#define		__MISP_CLK_H__


#define __CLK_DEBUG_PRINT__ 0 
// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
// typedef	enum {			
	// MISP_SYNC_1920x1080 = 0,
	// MISP_SYNC_1280x720,
	// MISP_SYNC_2048x1536,
// }	MISP_SYNC_MODE_t;
//-----------------------------------------------------------------------------
typedef	struct
{
	BOOL clk_invert ;
	BYTE clk_delay ;		
}	CLK_CTRL_PARAM_00, *PCLK_CTRL_PARAM_00;

typedef	struct
{
	BYTE clk_src_select ;
	BOOL clk_invert ;
	BYTE clk_delay ;		
}	CLK_CTRL_PARAM_01, *PCLK_CTRL_PARAM_01;

typedef	struct
{
	BYTE clk_src_select ;
	BYTE clk_divider ;	
	BOOL clk_invert ;
	BYTE clk_delay ;	
}	CLK_CTRL_PARAM_02, *PCLK_CTRL_PARAM_02;

typedef	struct
{
	BYTE clk_src_select ;
	BYTE clk_divider ;	
}	CLK_CTRL_PARAM_03, *PCLK_CTRL_PARAM_03;

typedef	struct
{
	BYTE clk_src_select ;
}	CLK_CTRL_PARAM_04, *PCLK_CTRL_PARAM_04;

typedef struct
{
	BOOL lvdstx_clk_sel;
	BYTE lvdstx_pll_clk_dly_sel;											
	BYTE clk_lvdstx_divider;											
	BYTE lvdstx_pll_clk_sel;											

}	CLK_LVDSTX_PARAM, *PCLK_LVDSTX_PARAM;

typedef	struct
{
 // fractional divider by dual modulus

 // ; division ratio
    // (clk_divider + clk_f/clk_t) if clk_division_mode = '0'
    // (clk_divider + (clk_t-clk_f)/clk_t) if clk_division_mode = '1'
	 
 // ; clk_synchonize = '0' : free running
   // clk_synchonize = '1' : synchonized to aux hsync

	BOOL clk_division_mode ;	// 
	BOOL clk_synchonize ;	// 
	BYTE clk_divider ; // 2~63
	WORD clk_t ; // 0~4095
	WORD clk_f ; // 0~4095
}	CLK_DIVIDER_PARAM, *PCLK_DIVIDER_PARAM;

typedef	struct
{
	CLK_DIVIDER_PARAM clk_divide_param ;
	BYTE clk_src_select ;
}	CLK_CTRL_PARAM_05, *PCLK_CTRL_PARAM_05;

//-----------------------------------------------------------------------------
typedef	struct
{
	// BOOL pll_en_n ;
	BOOL x_tal2_sel ; // [¡®0¡¯ : x_tal is selected] [¡®1¡¯ : x_tal2 is selected]

	BYTE pre_div ;
	BYTE post_div ;
	BYTE post_scale ;
}	HostCLK_MPLL_CTRL_PARAM, *PHostCLK_MPLL_CTRL_PARAM;

typedef	struct
{
	BOOL pll_en_n ;
#if defined(__USE_MDIN_i5XX_REV__)		// i5xx rev chip
	BOOL pll_sel_vclk ;
	// [¡®0¡¯ : refer pll_in_x_tal_sel value] [¡®1¡¯ : pll_in_clk_sel is used]
	
	BYTE pll_in_x_tal_sel ;	
	// Input source selection to the internal PLL for video clock when pll_sel_vclk = 0
	// [¡®00¡¯ : x_tal is used] [¡®01¡¯ : x_tal2 is used] [¡®1x¡¯ : host_clk is used ]
	
	BYTE pll_in_clk_sel ;
	// Input source selection to the internal PLL for video clock when pll_sel_vclk = 0
	// [¡®00¡¯ : CLK_B is used] [¡®01¡¯ : CLK_A is used] [¡®1x¡¯ : clk_lvds_p is used(clk from sensor)]
#else									// i5xx org chip
	BOOL pll_sel_xtal2 ; 
	// [¡®0¡¯ : refer pll_sel_xtal value] [¡®1¡¯ : x_tal2 is used]
	
	BOOL pll_sel_xtal ; 
	// [¡®0¡¯ : refer pll_sel_hclk value] [¡®1¡¯ : x_tal is used]
	
 	BOOL pll_sel_hclk ;	
	// [¡®0¡¯ : refer pll_sel_inclk value ] [¡®1¡¯ : HOST_CLK is used]
	
	BYTE pll_sel_inclk ;
	// Input source selection to the internal PLL for video clock when pll_sel_hclk = 0
	// [¡®00¡¯ : CLK_B is used] [¡®01¡¯ : CLK_A is used] [¡®10¡¯ : refer stISPCLK.clk_lvds_sel value ] [¡®11¡¯ : clk_lvds_p is used(clk from sensor)]
#endif
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	WORD pre_div ;
	WORD post_div ;
	WORD post_scale ;
#else 								// MDIN-i51X (i510, i540, i550)
	WORD pre_div ;
	WORD input_div ;
	WORD post_div ;
	WORD output_div ;
	WORD post_scale ;
#endif
	
	WORD pll_bypass ;
	
}	HostCLK_VPLL_CTRL_PARAM, *PHostCLK_VPLL_CTRL_PARAM;

typedef	struct
{
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	BOOL ab_pll_sel; // i51x : [¡®0¡¯ : CLK_B is used] [¡®1¡¯ : CLK_A is used ]
#endif	
	BYTE pll_in_sel; // i500 : [¡®00¡¯ : CLK_LVDS is used] [¡®01¡¯ : x_tal is used] [¡®10¡¯ : x_tal2 is used] [¡®11¡¯ : HOST_CLK ( from external chip pin ) is used ]
					 // i51x : [¡®00¡¯ : CLK_LVDS is used] [¡®01¡¯ : x_tal is used] [¡®10¡¯ : ab_pll_sel is used] [¡®11¡¯ : HOST_CLK ( from external chip pin ) is used ]

#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	WORD pre_div ;
	WORD post_div ;
	WORD post_scale ;
#else 								// MDIN-i51X (i510, i540, i550)
	WORD pre_div ;
	WORD input_div ;
	WORD post_div ;
	WORD output_div ;
	WORD post_scale ;
	WORD pll_bypass ;

	//Setting variables(not registers) to make locking sensors and outputs
	BYTE lock_mode;			//0 : free run		1 : Locked from standard output vclk		2 : Locked from non standard output vclk
	BYTE lock_src_port;		// 0 : Use MDIN-i5x0 internal path to user_clk out (only for standard case)		// 1 : From host clock pin to user_clk out		// 2 : From aux(ext) port pin to user_clk_out
	
#endif
	
}	HostCLK_USERPLL_CTRL_PARAM, *PHostCLK_USERPLL_CTRL_PARAM;
//-----------------------------------------------------------------------------
typedef	struct
{
	// WORD isp_clk_power_save ;		

	// When use external cpu
	// CLK_CTRL_PARAM_02 hclk ;	// ISP register access clk : default X_tal
	// When internal CPU is enabled. ISP don't need to setup hclk. Since hclk shares CPU operating clk.
	// ISP need to setup hclk only when internal CPU is disabled.
	
	CLK_CTRL_PARAM_00 clk_lvds_in ; // Deserializer clk
		
	CLK_CTRL_PARAM_02 clk_sensor_in ; // sensor_top clk
	BOOL clk_lvds_sel ;

	CLK_CTRL_PARAM_02 clk_bayer;
	// BYTE clk_wdr; // power save only
	// BYTE clk_mgr; // power save only
	// BYTE clk_tm; // power save only
	
	CLK_CTRL_PARAM_03 clk_odm_int; 
	// BYTE clk_ae; // power save only
	// BYTE clk_wb; // power save only
	// BYTE clk_af; // power save only
	
	CLK_CTRL_PARAM_01 clk_nr;
	// CLK_CTRL_PARAM_00 clk_nr_c; // power save only	
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	CLK_CTRL_PARAM_01 clk_yc_tm;
	CLK_CTRL_PARAM_04 clk_user;
	
	#if defined(__USE_MDIN_i5XX_REV__)
		CLK_CTRL_PARAM_04 clk_slave ; // Clk for sensor slave mode 2017.03.02 jhs
	#endif
	
#endif
	CLK_CTRL_PARAM_02 clk_yc;
	CLK_CTRL_PARAM_01 clk_yc_m;
	// CLK_CTRL_PARAM_00 clk_md;	// power save only
	
	CLK_CTRL_PARAM_03 clk_dis;	
}	HostCLK_ISP_PARAM, *PHostCLK_ISP_PARAM;
//-----------------------------------------------------------------------------
typedef	struct
{
	// WORD video_clk_power_save ;
	BOOL vclk_out_diable ;
		// [¡®0¡¯ : enable vclk_out&vclk_out_b pin of i500]
		// [¡®1¡¯ : disble vclk_out&vclk_out_b pin of i500]
	
#if defined(__USE_MDIN_i5XX_REV__)		
	CLK_CTRL_PARAM_00 clk_p0 ; // clk adjust for very first input
	CLK_CTRL_PARAM_00 clk_p1 ; // clk adjust for very first input
#endif	
	CLK_CTRL_PARAM_02 vclk ; 
	
	CLK_CTRL_PARAM_02 vclk_out ; 
	CLK_CTRL_PARAM_01 vclk_out_b ; 
	CLK_CTRL_PARAM_01 vclk_out_i ; 
	
	CLK_CTRL_PARAM_03 clk_m_enc_int ;
	CLK_CTRL_PARAM_04 clk_enc_int ; 	// Clock of internal video encoder and DAC. This clock split to encdac_clk&clk_in_enc
	
	BOOL DAC_video_power_down ;		// Video encoder DAC power-down mode / 0 : normal operation  / 1 : power-down mode
	CLK_CTRL_PARAM_00 encdac_clk ; 	// DAC video output clk
	CLK_CTRL_PARAM_00 clk_in_enc ;   // NTSC encoder clk		

}	HostCLK_VIDEO_PARAM, *PHostCLK_VIDEO_PARAM;
//-----------------------------------------------------------------------------
typedef	struct
{
	// WORD aux_clk_power_save ;
	
	CLK_CTRL_PARAM_03 clk_m_aux_int;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	CLK_CTRL_PARAM_01 clk_in_aux_tm;
#endif
	CLK_CTRL_PARAM_00 clk_in_aux; // clk_in_aux is an AUX video clock to video input mux
	
	CLK_CTRL_PARAM_05 clk_aux_mclk;
	
	CLK_CTRL_PARAM_01 clk_aux_in ; // clk_aux_in is an AUX video clock to video AUX input mux
	CLK_CTRL_PARAM_01 clk_aux_mid ; // clk_aux_mid is an AUX linemem clock
	CLK_CTRL_PARAM_01 clk_aux_out ; // clk_aux_out is an AUX video out clock

}	HostCLK_AUX_PARAM, *PHostCLK_AUX_PARAM;
//-----------------------------------------------------------------------------
// typedef	struct
// {
	// CLK_CTRL_PARAM_02 hclk_cpu ; 
	// CLK_CTRL_PARAM_00 pclk_cpu ; 	
// }	HostCLK_CPU_PARAM, *PHostCLK_CPU_PARAM;
//-----------------------------------------------------------------------------
typedef	struct
{
	CLK_CTRL_PARAM_00 mclk ; 
	CLK_CTRL_PARAM_00 ix2clko ; 
	CLK_CTRL_PARAM_00 icoreclk ; 
}	HostCLK_DDRMEM_PARAM, *PHostCLK_DDRMEM_PARAM;
//-----------------------------------------------------------------------------

typedef	struct
{
	
	HostCLK_MPLL_CTRL_PARAM stMPLL ;	// Pll for making ddr memory clk ( only for external cpu )
		
	// See ../common/init.c  "static void SetPMUInit(void)" for internal CPU clk setup			
	// HostCLK_CPU_PARAM stCPUCLK ; 
	
	BOOL 	ADSTSR_UCOM_EN ;	// 0: use external cpu	 1 : use internal cpu	
	BOOL 	SYSTEM_OSC36M ;	// 0: OSC X_tal is 27MHz	 1 : OSC X_tal is 36MHz	
	
	WORD sub_lvds_disable ; // [MSB(1bit clk + 12bit data)LSB] ports active high disable 
	WORD clk_power_save_0 ;
	WORD clk_power_save_1 ;

	BYTE vclk_out_gen_sync;		//Synchronization mode of main video output clock generator (vclk_out)
	BYTE vclk_out_gen_pload;	//Preload value to the main video output clock divider This is valid only when vclk_out_gen_sync is '1'.
	
	LONG 	ctrl;				// HostCLK ctrl flag
	
	// PLL
	HostCLK_VPLL_CTRL_PARAM stVPLL ;					// Pll for making main video clk
	HostCLK_USERPLL_CTRL_PARAM stUSERPLL ;	// Pll for making user multi purpose clock
	
	// CLK gen clk group
	HostCLK_ISP_PARAM stISPCLK ;
	HostCLK_VIDEO_PARAM stVIDEOCLK ;
	
	HostCLK_AUX_PARAM stAUXCLK ;
	HostCLK_DDRMEM_PARAM stMEMCLK ;

	//LVDS TX
	CLK_LVDSTX_PARAM stLVDSTXCLK;

}	HostCLK_CTRL_PARAM, *PHostCLK_CTRL_PARAM;

#define	HostCLKCTRL_TRIGGER_ISP			0x40000000		// ISP update
#define	HostCLKCTRL_USPLL_UPDATE		0x01000000		// Sensor PLL update
#define	HostCLKCTRL_MPLL_UPDATE			0x02000000		// Memory PLL update
#define	HostCLKCTRL_VPLL_UPDATE			0x04000000		// Video PLL update
#define	HostCLKCTRL_CLK_ON_OFF_UPDATE	0x08000000		// CLK_POWER_SAVE update
#define	HostCLKCTRL_ISPCLK_UPDATE		0x00000001		// ISP clock update
#define	HostCLKCTRL_AUXCLK_UPDATE		0x00000002		// AUX clock update
#define	HostCLKCTRL_VIDEOCLK_UPDATE		0x00000004		// Video clock update
#define	HostCLKCTRL_MEMCLK_UPDATE		0x00000008		// Memory clock update

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Device drive functions

void Init_HostCLK(void);

void HostCLK_ProcessHandler(void); // HostCLK process handler

// -----------------------------------------------------------------------------
// API Get / Set functions

void SetCLK_SoftReset(void);

void SetCLK_SrcYCTm(WORD clk_tm_yc_sel);
void SetCLK_InvYCTm(BOOL clk_tm_yc_inv);

void SetCLK_SrcYC(WORD clk_yc_sel);
void SetCLK_InvYC(BOOL clk_yc_inv);
void SetCLK_DivYC(BOOL clk_yc_div2_sel);
void SetCLK_DlyYC(BYTE delay);

void SetCLK_InvYCm(BOOL clk_yc_m_inv);
void SetCLK_DlyYCm(BYTE delay);

void SetCLK_SrcNR(BOOL clk_nr_sel);

void SetCLK_SrcSensor(WORD clk_sensor_in_sel);
void SetCLK_SrcAux(WORD clk_m_aux_sel);
void SetCLK_InvAux(BOOL clk_aux_inv);
void SetCLK_SrcAuxOut(WORD clk_aux_out_sel);
WORD GetCLK_SrcAuxOut(void);

void SetCLK_SrcAuxTm(WORD clk_aux_tm_sel);
void SetCLK_InvAuxTm(BOOL clk_aux_tm_inv);

void SetCLK_DivAux(BOOL	clk_m_aux_div2_sel);

void SetCLK_DlyAux(BYTE delay);
void SetCLK_SrcAuxlm(WORD clk_aux_lm_sel);
void SetCLK_SrcAuxIn(WORD clk_aux_in_sel);
void SetCLK_SrcENC(WORD venc_clk_sel);

void SetCLK_ClkSensorInDivider(WORD val) ;
void SetCLK_BayerClkDivider(WORD val) ;
void SetCLK_OdmFifoClkDivider(WORD val) ;
void SetCLK_ODMClkSelect(BOOL val) ;

void SetCLK_UserPLLSrc(BYTE pll_in_sel);

#if defined(__USE_MDIN_i500__) 		// MDIN-i500
void SetCLK_UserPLL(WORD P, WORD M, WORD S);
void SetCLK_VideoPLL(WORD P, WORD M, WORD S);
#else 								// MDIN-i51X (i510, i540, i550)
void SetCLK_UserPLL(WORD P, WORD N, WORD M, WORD OD, WORD S);
void SetCLK_UserPLLSrcSelect(BYTE pll_src_sel);
void SetCLK_UserPLL_LockMode(BYTE mode);
void SetCLK_UserPLL_LockSrcPort(BYTE src_port);

BYTE GetCLK_UserPLL_LockMode(void);
BYTE GetCLK_UserPLL_LockSrcPort(void);
void GetCLK_UserPLL(PWORD pP, PWORD pN, PWORD pM, PWORD pOD, PWORD pS);
void SetCLK_VideoPLL(WORD P, WORD N, WORD M, WORD OD, WORD S);

#if defined(__USE_MDIN_i5XX_REV__)
void SetCLK_SrcSensorSlave(BYTE clk_slave_sel);
#endif // __USE_MDIN_i5XX_REV__
		
#endif // MDIN-i51X (i510, i540, i550)

void SetCLK_MemoryPLL(WORD P, WORD M, WORD S); // Works only when using External cpu
void SetCLK_AuxVideoPLL(BYTE S, WORD F, WORD T);


// CLK power ON/OFF functions
void SetCLK_ClkLVDSTxOnOff(BOOL val) ;
void SetCLK_ClkLVDSInOnOff(BOOL val) ;
void SetCLK_ClkSensorInOnOff(BOOL val) ;
void SetCLK_ClkAudioOnOff(BOOL val) ;

void SetCLK_ClkAEOnOff(BOOL val) ;
void SetCLK_ClkWBOnOff(BOOL val) ;
void SetCLK_ClkAFOnOff(BOOL val) ;

void SetCLK_ClkWDRRdOnOff(BOOL val) ;
void SetCLK_ClkWDROnOff(BOOL val) ;
void SetCLK_ClkMGROnOff(BOOL val) ;
void SetCLK_ClkTMOnOff(BOOL val) ;

void SetCLK_ClkNROnOff(BOOL val) ;
void SetCLK_ClkNRcOnOff(BOOL val) ;

void SetCLK_ClkYCOnOff(BOOL val) ;
void SetCLK_ClkYCmOnOff(BOOL val) ;
void SetCLK_ClkMDOnOff(BOOL val) ;
void SetCLK_ClkDISOnOff(BOOL val) ;

void SetCLK_ClkInAUXOnOff(BOOL val) ;

void SetCLK_ClkAUXInOnOff(BOOL val) ;
void SetCLK_ClkAUXMidOnOff(BOOL val) ;
void SetCLK_ClkAUXOutOnOff(BOOL val) ;

void SetCLK_ClkVideoOnOff(BOOL val) ;
void SetCLK_ClkVideoOutOnOff(BOOL val) ;
void SetCLK_ClkEncOnOff(BOOL val) ;

void SetCLK_ClkMemoryOnOff(BOOL val) ;

void SetCLK_SrcVCLK(BYTE vclk_src_sel);
void SetCLK_VCLKDly(BYTE delay);
void SetCLK_VCLKInv(BOOL invert);

void SetCLK_VCLKOutDly(BYTE delay);
void SetCLK_VCLKOutInv(BOOL invert);
void SetCLK_VCLKOutiDly(BYTE delay);
void SetCLK_VCLKOutiInv(BOOL invert);
void SetCLK_VCLKAuxOutDly(BYTE delay);
void SetCLK_VCLKAuxOutInv(BYTE delay);
BYTE GetCLK_VCLKOutDly(void);
BYTE GetCLK_VCLKOutInv(void);
BYTE GetCLK_VCLKAuxOutDly(void);
BYTE GetCLK_VCLKAuxOutInv(void);


BYTE GetCLK_DlyYC(void);
BYTE GetCLK_DlyAux(void);
BOOL GetCLK_InvYC(void);
BOOL GetCLK_InvAux(void);
BYTE GetCLK_SrcYC(void);

BYTE GetCLK_InvYCDef(BYTE nID);
BYTE GetCLK_InvAuxDef(BYTE nID);

void SetCLK_SrcVpllHostCLK(void);
void SetCLK_SrcVpllXtal(void);

void SetCLK_SetBypassVideoPLL(void);

#endif	/* __MISP_CLK_H__ */
