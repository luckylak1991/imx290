//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	MISPOUTPUT.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"misp100.h"
#include	"video.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
static OUT_CTRL_PARAM	stOUTCTRL;


#if defined(__USE_4K60_OVL__)
#define		mstMrgn		8  // H-margin of source (8 means packet-size/pixel-resolution)
#endif

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
#define __ENC_Y_GAIN__	   0x0200 // 10b : 0x0200 is unity
#define __ENC_SYNC_LEVEL__ 0x0004 // 6b : The level is based on zero level and its unit is 1/1024 of the full-scale DAC amplitude. 4 is recommended.
#define __ENC7__ (__ENC_SYNC_LEVEL__<<10)|(__ENC_Y_GAIN__) // Combined data must be under 16b length

// default value for Video Encoder
static ROMDATA MISP_ENCODER_COEF MISP_Encoder_Default[]		= {
//     0       1       2       3       4       5       6       7       8       9       A       B       C       D       E       F
  {{0x2000, 0x16b9, 0x84c7, 0x888e, 0x0200, 0x8070, 0xa8f0, __ENC7__, 0x1800, 0x7c1f, 0x21f0, 0x1000, 0xa021, 0x0800, 0x0000, 0x0000,	// 1E0
	0x0000, 0x0000, 0x0000 }},		// NTSC system																					// 1F0
// Added by Jay // 960 burst end position // by jwlee 20141124
  {{0x2600, 0x16e1, 0x84c7, 0x888e, 0x0200, 0x8070, 0xa8f0, __ENC7__, 0x1804, 0x5d14, 0x1974, 0x1000, 0xa021, 0x0800, 0x0000, 0x0000, // 1E0
	0x0000, 0x0000, 0x0000 }},		// NTSC 960H system

  {{0xA000, 0x16bb, 0x84c7, 0xd089, 0x0200, 0x8075, 0x00fc, __ENC7__, 0x1801, 0x8acb, 0x2a09, 0x3000, 0xa023, 0x0a00, 0x0600, 0x0000,	// 1E0
	0x0400, 0x0000, 0x0000 }}		// PAL system																					// 1F0
};

// default value for B/W extension
static ROMDATA MISP_BWPOINT_COEF defMISPBWPoint[]	= {
	{	{  16,  32,  64, 128}, { 64,  96,  132, 184}	},	// type1 [in|out] // Semi Gamma 0.45 // 20150313 jwlee
//	{	{  16,  28,  48, 70}, {  16,  50,  70, 76}	},	// type1 [in|out]
	{	{ 32,  64, 192, 224}, { 15,  64, 192, 238}	},	// type2 [in|out]
	{	{ 25,  64, 192, 228}, { 43,  64, 192, 208}	},	// type3 [in|out]
};


// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
static BYTE OUTCTRL_LoadMainFrmt(POUT_CTRL_PARAM pCTRL);

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//------------------------------------------------------------------------------------------
static void Init_OUT_PARAM( POUT_CTRL_PARAM pCTRL )
{
	// output pin map
#if defined(__MDIN_i500_EDK__) || defined(__MDIN_i500_REF__)
	pCTRL->stPinMap.extn_en  = 0;		pCTRL->stPinMap.gpio_en  = 0;
	pCTRL->stPinMap.lvds_en  = 0;
	pCTRL->stPinMap.mode_sel = OUT_MAIN_422_20B;
#endif
	
#if defined(__MDIN_i510_EDK__) || defined(__MDIN_i510_REF__) || defined(__MDIN_i510_IMX290__)
	pCTRL->stPinMap.extn_en  = 0;		pCTRL->stPinMap.gpio_en  = 0;
	pCTRL->stPinMap.lvds_en  = 0;
	pCTRL->stPinMap.mode_sel = OUT_MAIN_422_20B;		// NCO.3
#endif
	
#if defined(__MDIN_i540_EDK__) || defined(__MDIN_i540_REF__) || defined(__MDIN_i540_4K60__)
	pCTRL->stPinMap.extn_en  = 0;		pCTRL->stPinMap.gpio_en  = 0;

#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
	pCTRL->stPinMap.lvds_en  = 1;							// exchange embeded sync mode(MISP_OUT_EMB422_8 or 10) together
	pCTRL->stPinMap.mode_sel = OUT_LVDS_DOUBLE_20B;		// NLO.5 -- place(R716 ~ R688)	

#else		//use digital out(default)
	pCTRL->stPinMap.lvds_en  = 0;
#if defined(__USE_CMOS_AUX_SYNC_OUT__) && (defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__))	// Use AUX sync until i5x0 revision for Sensor Slave Mode 
	pCTRL->stPinMap.mode_sel = OUT_MAIN_422_20B_AUX_656_8B;		// NCO.4
#elif defined(__MDIN_i540_4K60__)
	pCTRL->stPinMap.mode_sel = OUT_MAIN_444_24B;		// NCO.9
#else
	pCTRL->stPinMap.mode_sel = OUT_MAIN_422_20B;		// NCO.3
#endif	

#endif	//end __USE_LVDSTX...
#endif	//end __MDIN_i540_...
	
#if defined(__MDIN_i550_EDK__) || defined(__MDIN_i550_REF__)
	pCTRL->stPinMap.extn_en  = 1;		pCTRL->stPinMap.gpio_en  = 0;
	pCTRL->stPinMap.lvds_en  = 0;

#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
	pCTRL->stPinMap.lvds_en  = 1;							// exchange embeded sync mode(MISP_OUT_EMB422_8 or 10) together
	pCTRL->stPinMap.mode_sel = OUT_MAIN_422_20B_HS_VS_LVDS_DOUBLE_20B;		// NLO.5 -- place(R716 ~ R688)	
#else	//use digital out(default)

#if defined(__MDIN_i550_USB_V100__)
	pCTRL->stPinMap.mode_sel = OUT_MAIN_444DW_32B_AUX_422_16B;	// ECO.24
#elif defined(__USE_CMOS_AUX_SYNC_OUT__) && (defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__))	// Use AUX sync until i5x0 revision for Sensor Slave Mode 
	pCTRL->stPinMap.mode_sel = OUT_MAIN_422_20B_AUX_656_8B;		// ECO.5
#else
	pCTRL->stPinMap.mode_sel = OUT_MAIN_422_20B;		// ECO.4
#endif
#endif
#endif
		
#if defined(__MDIN_i5XX_FPGA__)
#if defined(__USE_2_INPUT_HDMI__)
	pCTRL->stPinMap.extn_en  = 1;		pCTRL->stPinMap.gpio_en  = 0;
	pCTRL->stPinMap.lvds_en  = 0;
	pCTRL->stPinMap.mode_sel = OUT_MAIN_422_16B;		// ECO.12
#else
	pCTRL->stPinMap.extn_en  = 1;		pCTRL->stPinMap.gpio_en  = 0;
	pCTRL->stPinMap.lvds_en  = 0;
	pCTRL->stPinMap.mode_sel = OUT_MAIN_422_20B;		// ECO.4
#endif
#endif

	if(pCTRL->stPinMap.lvds_en == 1)	{			// lvds tx initial
#if defined(__MDIN_i550_EDK__) || defined(__MDIN_i550_REF__)
		if(pCTRL->stPinMap.mode_sel <= OUT_LVDS_DOUBLE_20B)	{	// skip 'EHO' mode
#endif
		#if defined(__USE_LVDSTX_MAIN_DUAL__)
			pCTRL->stLVDS_tx.lvds_tx_mode = 1;		//('0' : single mode	'1' : dual mode)	
			pCTRL->stLVDS_tx.lvds_tx_map = 1 ;		//('00' : 8bit VESA Map(Zhine VILM Map)	'01' : Zhine VIHM Map = Open JEIDA		'10' : 10bit VESA Map		'11' : Sony YC Map(set h/w(THC64LVDS1024) pin map VIHM))		
			pCTRL->stLVDS_tx.lvds_tx_portb_ctrl = 0;	
			pCTRL->stLVDS_tx.lvds_tx2_en = 0;
			pCTRL->stLVDS_tx.lvds_tx_evenodd_en=0;
		#elif defined(__USE_LVDSTX_EVEN_ODD__)
			pCTRL->stLVDS_tx.lvds_tx_mode = 0;		//('0' : single mode	'1' : dual mode)	
			pCTRL->stLVDS_tx.lvds_tx_map = 1 ;		//('00' : 8bit VESA Map(Zhine VILM Map)	'01' : Zhine VIHM Map = Open JEIDA		'10' : 10bit VESA Map		'11' : Sony YC Map(set h/w(THC64LVDS1024) pin map VIHM))		
			pCTRL->stLVDS_tx.lvds_tx_portb_ctrl = 2;
			pCTRL->stLVDS_tx.lvds_tx2_en = 1;
			pCTRL->stLVDS_tx.lvds_tx_evenodd_en=1;
		#elif defined(__USE_LVDSTX_MAIN_SINGLE__)
			pCTRL->stLVDS_tx.lvds_tx_mode = 0;		//('0' : single mode	'1' : dual mode)	
			pCTRL->stLVDS_tx.lvds_tx_map = 1 ;		//('00' : 8bit VESA Map(Zhine VILM Map)	'01' : Zhine VIHM Map = Open JEIDA		'10' : 10bit VESA Map			'11' : Sony YC Map(set h/w(THC64LVDS1024) pin map VIHM))		
			pCTRL->stLVDS_tx.lvds_tx_portb_ctrl = 0;
			pCTRL->stLVDS_tx.lvds_tx2_en = 0;
			pCTRL->stLVDS_tx.lvds_tx_evenodd_en=0;
		#endif
		
		pCTRL->stLVDS_tx.lvds_tx_enable = 1 ;
		pCTRL->stLVDS_tx.lvds_tx_cont1 = 0 ;
		pCTRL->stLVDS_tx.lvds_tx_cont2 = 0 ;
		pCTRL->stLVDS_tx.lvds_tx_map_option = 0;
		pCTRL->stLVDS_tx.lvds_tx_state = 0 ;
		pCTRL->stLVDS_tx.lvds_tx_state_retry = 0;
		
		#if defined(__MDIN_i550_EDK_V11__)
		pCTRL->stLVDS_tx.lvds_tx_data_swap1 = 1;
		pCTRL->stLVDS_tx.lvds_tx_data_swap2 = 1;
		#else
		pCTRL->stLVDS_tx.lvds_tx_data_swap1 = 0;
		pCTRL->stLVDS_tx.lvds_tx_data_swap2 = 0;
		#endif
		
		if(pCTRL->stLVDS_tx.lvds_tx_map == 3) pCTRL->stLVDS_tx.lvds_tx1_444_422_sel = 1;		
		else pCTRL->stLVDS_tx.lvds_tx1_444_422_sel=0;
		
		pCTRL->stLVDS_tx.lvds_tx2_444_422_sel=0;	
		pCTRL->stLVDS_tx.lvds_tx1_main_aux_sel=0;	pCTRL->stLVDS_tx.lvds_tx2_main_aux_sel=0;		
		pCTRL->stLVDS_tx.lvds_tx1_hs_invert=0;	pCTRL->stLVDS_tx.lvds_tx1_vs_invert=0;	pCTRL->stLVDS_tx.lvds_tx1_de_invert=0;	
		pCTRL->stLVDS_tx.lvds_tx2_hs_invert=0;	pCTRL->stLVDS_tx.lvds_tx2_vs_invert=0;	pCTRL->stLVDS_tx.lvds_tx2_de_invert=0;					
		
#if defined(__MDIN_i550_EDK__) || defined(__MDIN_i550_REF__)
		}
#endif
	}

	switch(GetSYSINFO_BoardType())	{
		case  MISP_SYSINFO_I540REF_V10:
		case  MISP_SYSINFO_I540REF_V121:
		case  MISP_SYSINFO_I550REF_V10:
		case  MISP_SYSINFO_I550REF_V121:
#if (defined(__USE_CMOS_IMX226__) &&  !defined(__USE_IMX226_1080P60_MODE__)) || \
    (defined(__USE_CMOS_IMX274__) &&  !defined(__USE_IMX274_1080P60_MODE__)) || \
    defined(__USE_CMOS_IMX326__) || defined(__USE_CMOS_IMX253__) || defined(__USE_CMOS_IMX334__)		// 4k sensor
					pCTRL->stOUT_m.frmt = MISP_VIDOUT_3840x2160p30; 	break;
					//pCTRL->stOUT_m.frmt = MISP_VIDOUT_1920x1080p60; 	break;
#elif (defined(__USE_CMOS_IMX273__) && !defined(__USE_IMX273_ALLSCAN_MODE__))
					pCTRL->stOUT_m.frmt = MISP_VIDOUT_1280x720p60; 	break;
#elif defined(__MN34420_1280x1024_ISPCROP__)
					pCTRL->stOUT_m.frmt = MISP_VIDOUT_1280x1024p60; 	break;
#else		// 2M sensor
		#if  (defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)) && defined(__USE_IMX290_1080P120_MODE__)
					pCTRL->stOUT_m.frmt = MISP_VIDOUT_1920x1080p120; 	break;
//					pCTRL->stOUT_m.frmt = MISP_VIDOUT_1920x1080p60; 	break;
		#else
					pCTRL->stOUT_m.frmt = MISP_VIDOUT_1920x1080p60; 	break;
		#endif				
#endif
		case  MISP_SYSINFO_I550_USB_V10:
					//pCTRL->stOUT_m.frmt = MISP_VIDOUT_3840x2160p30; 	break;
					pCTRL->stOUT_m.frmt = MISP_VIDOUT_1920x1080p24; 	break;
#if defined(__USE_4K60_INOUT__)
		case  MISP_SYSINFO_I540_4K60_V10:
				#if defined(__USE_EXT_AF_MD1__) || defined(__USE_MISP_AF_MD2__)
					pCTRL->stOUT_m.frmt = MISP_VIDOUT_3840x2160p30; 	break;
				#else
					pCTRL->stOUT_m.frmt = MISP_VIDOUT_1920x2160p60; 	break;
				#endif
#endif
		case  MISP_SYSINFO_I5x0FPGA:
					pCTRL->stOUT_m.frmt = MISP_VIDOUT_1920x1080p30; 	break;
		
		default: 	pCTRL->stOUT_m.frmt = MISP_VIDOUT_1920x1080p60; 	break;	// i500, i510 default
	}
	
#if defined(__USE_TVI_TX__)
	#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 
		pCTRL->stOUT_m.frmt = MISP_VIDOUT_3840x2160p15;	
	#else
		pCTRL->stOUT_m.frmt = MISP_VIDOUT_1920x1080p30;	
	#endif
#endif

	pCTRL->stOUT_m.frmt = OUTCTRL_LoadMainFrmt(pCTRL);	// load main format param from eeprom
	
	
#if	defined(__USE_25Hz_OUTPUT__)			// for 25hz output
	switch(pCTRL->stOUT_m.frmt)	{
		case  MISP_VIDOUT_1280x720p60: 	pCTRL->stOUT_m.frmt = MISP_VIDOUT_1280x720p50; 		break;
		case  MISP_VIDOUT_1920x1080p60: 	pCTRL->stOUT_m.frmt = MISP_VIDOUT_1920x1080p50; 	break;
		case  MISP_VIDOUT_1920x1080p30: 	pCTRL->stOUT_m.frmt = MISP_VIDOUT_1920x1080p25; 	break;
		case  MISP_VIDOUT_2048x1536p30: 	pCTRL->stOUT_m.frmt = MISP_VIDOUT_2048x1536p25; 	break;
		case  MISP_VIDOUT_1280x720p120: 	pCTRL->stOUT_m.frmt = MISP_VIDOUT_1280x720p100; 	break;
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
		case  MISP_VIDOUT_3840x2160p15: 	pCTRL->stOUT_m.frmt = MISP_VIDOUT_3840x2160p12; 	break;
		case  MISP_VIDOUT_3840x2160p30: 	pCTRL->stOUT_m.frmt = MISP_VIDOUT_3840x2160p25; 	break;
		case  MISP_VIDOUT_4096x2160p30: 	pCTRL->stOUT_m.frmt = MISP_VIDOUT_4096x2160p25; 	break;
		case  MISP_VIDOUT_1920x1080p120: 	pCTRL->stOUT_m.frmt = MISP_VIDOUT_1920x1080p100; 	break;
#if defined(__USE_4K60_INOUT__)
		case  MISP_VIDOUT_1920x2160p60: 	pCTRL->stOUT_m.frmt = MISP_VIDOUT_1920x2160p50; 	break;
		case  MISP_VIDOUT_2048x2160p60: 	pCTRL->stOUT_m.frmt = MISP_VIDOUT_2048x2160p50; 	break;
#endif
#endif
	}
#endif
	
#if defined(__MDIN_i510_IMX290__) || defined(__MDIN_i510_REF__) || defined(__USE_EN332T__)
	pCTRL->stOUT_m.mode = MISP_OUT_EMB422_8;//10;	// for SDI Tx in 'i510+290 one bd', 'i510 ref bd'
#elif defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__) 
	pCTRL->stOUT_m.mode = MISP_OUT_EMB422_8;		// for lvds tx..
//	pCTRL->stOUT_m.mode = MISP_OUT_EMB422_10;		// for lvds tx..
//	pCTRL->stOUT_m.mode = MISP_OUT_RGB444_10;		// for lvds tx..   change together :  misp_font_rgb_pal ...  at osd.c
#elif defined(__USE_4K60_INOUT__)
	pCTRL->stOUT_m.mode = MISP_OUT_RGB444_8;
#else
	pCTRL->stOUT_m.mode = MISP_OUT_SEP422_8;//10;	// 10bit : pip on noise ??
#endif

	
	// specific output mode for specific board
#if defined(__USE_TVI_TX__)
	pCTRL->stOUT_m.mode = MISP_OUT_EMB422_8;
#endif
	
#if defined(__MDIN_i5XX_FPGA__)							// FPGA board
	pCTRL->stOUT_m.mode = MISP_OUT_SEP422_8;  // because fpga setting 8bit out at h/w design
#endif
	
	
#if CSC_RANGE_CTRL
  #if defined(__MDIN_i5XX_FPGA__)
	pCTRL->stOUT_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscBypass_StdRange;	// out csc bypass for test
  #else
	if((pCTRL->stOUT_m.mode == MISP_OUT_RGB444_8) || (pCTRL->stOUT_m.mode == MISP_OUT_RGB444_10)) 	
		pCTRL->stOUT_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscYUVtoRGB_HD_StdRange;
	else
		pCTRL->stOUT_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscBypass_FullRange;
  #endif
  
#else
	pCTRL->stOUT_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscBypass_FullRange;
#endif
	pCTRL->stOUT_m.stTUNE.brightness = 128;		pCTRL->stOUT_m.stTUNE.contrast = 128;
	pCTRL->stOUT_m.stTUNE.saturation = 128;		pCTRL->stOUT_m.stTUNE.hue = 128;	

#if MISP_TUNE_RGB_GAIN_OFFSET == 1
	pCTRL->stOUT_m.stGAIN.r_gain = 128;			pCTRL->stOUT_m.stGAIN.r_offset = 128;
	pCTRL->stOUT_m.stGAIN.g_gain = 128;			pCTRL->stOUT_m.stGAIN.g_offset = 128;
	pCTRL->stOUT_m.stGAIN.b_gain = 128;			pCTRL->stOUT_m.stGAIN.b_offset = 128;
#endif

	// for CVBS out
#if defined(__USE_CMOS_AUX_SYNC_OUT__) && (defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__))	// Use AUX sync until i5x0 revision for Sensor Slave Mode
	pCTRL->stOUT_x.mode = MISP_OUT_SEP422_8;			// Use AUX sync until i5x0 revision for Sensor Slave Mode
#elif defined(__MDIN_i550_USB_V100__)
	pCTRL->stOUT_x.mode = MISP_OUT_EMB422_8;
#else
	pCTRL->stOUT_x.mode = MISP_OUT_MUX656_8;			// for CVBS out
#endif

#if defined(__MDIN_i550_USB_V100__)
	pCTRL->stOUT_x.frmt = MISP_VIDOUT_3840x2160p30;		// 4K 30P for HDMI Tx
	//pCTRL->stOUT_x.frmt = MISP_VIDOUT_1920x1080p60;		// 1080 60P for HDMI Tx
//	pCTRL->encFRMT = MISP_VID_VENC_NOTVALID;				// Do not use
#else
	pCTRL->stOUT_x.frmt = MISP_VIDOUT_720x480i60;		// CVBS : 720H
#endif
	pCTRL->encFRMT = MISP_VID_VENC_NTSC_M;				// CVBS : NTSC
	
	//pCTRL->stOUT_x.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscBypass_FullRange;
	pCTRL->stOUT_x.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscBypass_StdRange;
	pCTRL->stOUT_x.stTUNE.brightness = 128;		pCTRL->stOUT_x.stTUNE.contrast = 128;
	pCTRL->stOUT_x.stTUNE.saturation = 128;		pCTRL->stOUT_x.stTUNE.hue = 128;	

#if MISP_TUNE_RGB_GAIN_OFFSET == 1
	pCTRL->stOUT_x.stGAIN.r_gain = 128;			pCTRL->stOUT_x.stGAIN.r_offset = 128;
	pCTRL->stOUT_x.stGAIN.g_gain = 128;			pCTRL->stOUT_x.stGAIN.g_offset = 128;
	pCTRL->stOUT_x.stGAIN.b_gain = 128;			pCTRL->stOUT_x.stGAIN.b_offset = 128;
#endif

	// misc
#if defined(__INIT_CR_KEY_DEMO__)
	pCTRL->stOUT_m.stMISC.back_y=32;  pCTRL->stOUT_m.stMISC.back_cb=120;   pCTRL->stOUT_m.stMISC.back_cr=120;
#else
	pCTRL->stOUT_m.stMISC.back_y=32;  pCTRL->stOUT_m.stMISC.back_cb=0x80;  pCTRL->stOUT_m.stMISC.back_cr=0x80;
#endif
	pCTRL->stOUT_m.stMISC.dither=ON;

	pCTRL->stOUT_x.stMISC.back_y=32;  pCTRL->stOUT_x.stMISC.back_cb=0x80;  pCTRL->stOUT_x.stMISC.back_cr=0x80;
	pCTRL->stOUT_x.stMISC.dither=ON;

	// pip controls
	pCTRL->stPIPctl.blend_lev=0;  //opaque
	pCTRL->stPIPctl.key_en=0;
#if defined(__INIT_CR_KEY_DEMO__)
	pCTRL->stPIPctl.key_y_u =32;	pCTRL->stPIPctl.key_y_l =32-1;
	pCTRL->stPIPctl.key_cb_u=120;	pCTRL->stPIPctl.key_cb_l=120-1;
	pCTRL->stPIPctl.key_cr_u=120;	pCTRL->stPIPctl.key_cr_l=120-1;
#else
	pCTRL->stPIPctl.key_y_u =0x80;	pCTRL->stPIPctl.key_y_l =0x80;
	pCTRL->stPIPctl.key_cb_u=0x80;	pCTRL->stPIPctl.key_cb_l=0x80;
	pCTRL->stPIPctl.key_cr_u=0x80;	pCTRL->stPIPctl.key_cr_l=0x80;
#endif

	//misc
	pCTRL->dspFLAG = (MISP_MAIN_FREEZE_OFF | MISP_MAIN_DISPLAY_ON | MISP_AUX_FREEZE_OFF | MISP_AUX_DISPLAY_ON);	// for dis
	pCTRL->enhFLAG = (MISP_ENH_BWEXT_OFF|MISP_ENH_LTI_OFF|MISP_ENH_CTI_OFF|MISP_ENH_COLOR_OFF);
	
	//pCTRL->dspFLAG |= MISP_MAIN_OUT_TP_COLOR;	// main out testpattern
	//pCTRL->dspFLAG |= MISP_AUX_OUT_TP_COLOR;	// aux out testpattern

	pCTRL->stOUT_m.turnon_cnt=0;	pCTRL->stOUT_m.turnon_dly=0;		// main/aux video out turn-on delay(unit:16ms)
	pCTRL->stOUT_x.turnon_cnt=0;	pCTRL->stOUT_x.turnon_dly=0;
	
	// external lock
#if defined(__USE_4K60_INOUT__)
	pCTRL->stExtLock.lock_mode = 1;
	pCTRL->stExtLock.lock_source = 5;	// 5:sync signals from main sensor input
//#elif defined(__EXT_VSYNC_LOCK__)
//	pCTRL->stExtLock.lock_mode = 2;
//	pCTRL->stExtLock.lock_source = 1;	
#else
	pCTRL->stExtLock.lock_mode = 0;
	pCTRL->stExtLock.lock_source = 0;
#endif
	
	pCTRL->stExtLock.sync_mode = 0;
	pCTRL->stExtLock.hs_dly = 0;	pCTRL->stExtLock.vs_dly = 100;

	//aux input format/mode
	if(GetIN_AuxPath() == MISP_AUXPATH_MAIN_O) 	{ 					// 'main output' --> 'aux input'
		SetIN_AuxFrmt(MIN(pCTRL->stOUT_m.frmt, MISP_VIDOUT_1920x1200pRB)); 	// set aux input frmt
		SetIN_AuxMode(pCTRL->stOUT_m.mode); 							// set aux input mode
	}
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetPinMAP(PMISP_OUT_MODE_INFO pINFO)
{
	BYTE data_oen;
	WORD oen[3];
	
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	MISP_RegField(MISP_LOCAL_ID, 0x0A6,  0, 4, pINFO->mode_sel);	// dig_io_out_mode
#else 												// MDIN-i51X (i510, i540, i550)
	MISP_RegField(MISP_LOCAL_ID, 0x0A6, 15, 1, pINFO->extn_en);		// dig_io_out_mode_sel_ext_en
	MISP_RegField(MISP_LOCAL_ID, 0x0A6, 14, 1, pINFO->lvds_en);		// dig_io_out_mode_sel_lvds_en
	MISP_RegField(MISP_LOCAL_ID, 0x0A6, 11, 1, pINFO->gpio_en);		// dig_io_out_mode_sel_gpio_a_en
	MISP_RegField(MISP_LOCAL_ID, 0x0A6,  0, 5, pINFO->mode_sel);	// dig_io_out_mode

	if(pINFO->lvds_en){
		MISP_RegField(MISP_HOST_ID, 0x031, 0, 12, 0x000);		// lvds mode
		MISP_RegField(MISP_HOST_ID, 0x031, 12, 1, 1);			// lvdstx_pdn. 1: lvds mode, 0:LVTTL
	}
	
	switch(pINFO->mode_sel)	{
#if defined(__USE_MDIN_i550__)
		case OUT_MAIN_444DW_40B_AUX_656_8B:		data_oen=1;	oen[2]=0x0000;	oen[1]=0x0000;	oen[0]=0xFFFF;	break;
		case OUT_MAIN_444DW_32B_AUX_656_8B:		data_oen=1;	oen[2]=0x0000;	oen[1]=0x00FF;	oen[0]=0xFFFF;	break;
		case OUT_MAIN_444DW_32B_AUX_422_16B:	data_oen=1;	oen[2]=0x0000;	oen[1]=0x0000;	oen[0]=0xFFFF;	break;
#endif
		default:								data_oen=0;	oen[2]=0x0000;	oen[1]=0x0000;	oen[0]=0x0000;	break;
	}
	
	MISP_RegField(MISP_HOST_ID, 0x075, 15,  1, data_oen);	// r_use_data_oen
	MISP_RegField(MISP_HOST_ID, 0x075,  0, 10, oen[2]);		// r_data_oen[41:32]
	MISP_RegWrite(MISP_HOST_ID, 0x076,  oen[1]);			// r_data_oen[31:16]
	MISP_RegWrite(MISP_HOST_ID, 0x077,  oen[0]);			// r_data_oen[15:0]
#endif
}


//------------------------------------------------------------------------------------
static void OUTCTRL_SetMainMode(BYTE mode)
{
	BOOL	out_422, out_bt656, dtr_enc_en, out_8bit;
	BOOL	dig_io_8bit, dig_io_sync;
	
	switch(mode)	{
		case  MISP_OUT_RGB444_8:	out_422=0;  out_bt656=0;  dtr_enc_en=0;  out_8bit=1;  break;
		case  MISP_OUT_RGB444_10:	out_422=0;  out_bt656=0;  dtr_enc_en=0;  out_8bit=0;  break;
		case  MISP_OUT_YUV444_8:	out_422=0;  out_bt656=0;  dtr_enc_en=0;  out_8bit=1;  break;
		case  MISP_OUT_YUV444_10:	out_422=0;  out_bt656=0;  dtr_enc_en=0;  out_8bit=0;  break;
		case  MISP_OUT_EMB422_8:	out_422=1;  out_bt656=0;  dtr_enc_en=1;  out_8bit=1;  break;
		case  MISP_OUT_EMB422_10:	out_422=1;  out_bt656=0;  dtr_enc_en=1;  out_8bit=0;  break;
		case  MISP_OUT_SEP422_8:	out_422=1;  out_bt656=0;  dtr_enc_en=0;  out_8bit=1;  break;
		case  MISP_OUT_SEP422_10:	out_422=1;  out_bt656=0;  dtr_enc_en=0;  out_8bit=0;  break;
		case  MISP_OUT_MUX656_8:	out_422=0;  out_bt656=1;  dtr_enc_en=1;  out_8bit=1;  break;
		case  MISP_OUT_MUX656_10:	out_422=0;  out_bt656=1;  dtr_enc_en=1;  out_8bit=0;  break;
		default:					out_422=1;  out_bt656=0;  dtr_enc_en=1;  out_8bit=0;  break;
	}
	MISP_RegField(MISP_LOCAL_ID, 0x0A2,  0, 2, out_422);		// out_422_en (0:444, 1:422)
	MISP_RegField(MISP_LOCAL_ID, 0x0A2,  1, 2, out_bt656);		// out_bt656_en (0:422/444, 1:656)
	MISP_RegField(MISP_LOCAL_ID, 0x0A2,  5, 1, dtr_enc_en);		// s274m_en (0:SAV/EAV disable, 1:SAV/EAV enable)
	MISP_RegField(MISP_LOCAL_ID, 0x0A2,  2, 1, out_8bit);		// out_8bit_en (0:10bit, 1:8bit)
	
	dig_io_8bit = out_8bit;

	
#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__) 
	dig_io_sync = 1;
#else
	dig_io_sync = dtr_enc_en;
#endif

	
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	MISP_RegField(MISP_LOCAL_ID, 0x0A6,  5, 1, dig_io_8bit);	// dig_io_out_mode_main_bit (0:10bit, 1:8bit+2gpio)
	MISP_RegField(MISP_LOCAL_ID, 0x0A6,  4, 1, dig_io_sync);	// dig_io_out_mode_main_sync (0:sep-sync, 1:emb-sync)
#else 												// MDIN-i51X (i510, i540, i550)
	MISP_RegField(MISP_LOCAL_ID, 0x0A6,  6, 1, dig_io_8bit);	// dig_io_out_mode_main_bit (0:10bit, 1:8bit+2gpio)
	MISP_RegField(MISP_LOCAL_ID, 0x0A6,  5, 1, dig_io_sync);	// dig_io_out_mode_main_sync (0:sep-sync, 1:emb-sync)
#endif
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetMainFrmt(BYTE frmt)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_ISP_SCLK_OUT__)
	WORD n, m;
#endif
	PMISP_OUTVIDEO_INFO pOUT=&stOUTCTRL.stOUT_m;
	memcpy(&pOUT->stSYNC, &defMISPOutSync[frmt],  sizeof(MISP_OUTVIDEO_SYNC));	// copy sync info.
	memcpy(&pOUT->stATTB, &defMISPOutVideo[frmt], sizeof(MISP_OUTVIDEO_ATTB));	// copy v.out info.
	memset(&pOUT->stFINE, 0, sizeof(MISP_OUTVIDEO_FINE));

#if defined(__USE_4K60_OVL__)
	if(mstMrgn) pOUT->stATTB.Hact+=(pOUT->stATTB.Hact*mstMrgn+defMISPOutVideo[frmt].Hact-1)/defMISPOutVideo[frmt].Hact;  // round up
#endif
	
#if defined(__USE_4K60_INOUT__)
	if(IsOUT_MainFrmt12G() == FALSE)	{	// v.out is not 4k60
		pOUT->stSYNC.bgnHA += (pOUT->stSYNC.bgnHA%2)?  1:  0;
		pOUT->stSYNC.endHA += (pOUT->stSYNC.endHA%2)?  1:  0;
		pOUT->stSYNC.bgnHS += (pOUT->stSYNC.bgnHS%2)?  1:  0;
		pOUT->stSYNC.endHS += (pOUT->stSYNC.endHS%2)?  1:  0;
		
		pOUT->stSYNC.bgnHA /= 2;		pOUT->stSYNC.endHA /= 2;
		pOUT->stSYNC.bgnHS /= 2;		pOUT->stSYNC.endHS /= 2;
		pOUT->stSYNC.totHS /= 2;
		pOUT->stATTB.Htot /= 2;			pOUT->stATTB.Hact /= 2;
		pOUT->stSYNC.vclkM /= 2;
	}
	
	// for interlace out
	if(frmt == MISP_VIDOUT_1920x1080i60)	{pOUT->stSYNC.totHS	*= 2;	pOUT->stATTB.Htot *= 2;}
	if(frmt == MISP_VIDOUT_1920x1080i50)	{pOUT->stSYNC.totHS	*= 2;	pOUT->stATTB.Htot *= 2;}
#if defined(__USE_59Hz_OUTPUT__)
	if(frmt == MISP_VIDOUT_1920x1080i59)	{pOUT->stSYNC.totHS	*= 2;	pOUT->stATTB.Htot *= 2;}
#endif
#endif
	
	pOUT->stSYNC.posFLD |= (pOUT->stATTB.attb&MISP_SCANTYPE_PROG)? 0 : (1<<15);	// 1:interlace out, 0:prog.
	
#if defined(__USE_MDIN_i500__)		// mdin-i500 only
	pOUT->stSYNC.bgnHA -= 33;		pOUT->stSYNC.endHA -= 33;
#endif
	
	if (pOUT->mode==MISP_OUT_MUX656_8 || pOUT->mode==MISP_OUT_MUX656_10) {	// for 656 out
		pOUT->stSYNC.totHS *= 2;
#if	!defined(__MDIN_i5XX_FPGA__)				// EDK & REF board
		pOUT->stSYNC.vclkS /= 2;
#endif
		pOUT->stSYNC.bgnHA *= 2;		pOUT->stSYNC.endHA *= 2;
		pOUT->stSYNC.bgnHS *= 2;		pOUT->stSYNC.endHS *= 2;
	}
	
	// set sdi_rate_sel(GV7600) : 0(HD), 1(Full HD)
	switch(frmt)	{
		case MISP_VIDOUT_1920x1080p60:		SDI_RATE(1);	break;
		case MISP_VIDOUT_1920x1080p50:		SDI_RATE(1);	break;
#if defined(__USE_59Hz_OUTPUT__)
		case MISP_VIDOUT_1920x1080p59:		SDI_RATE(1);	break;
#endif
		default: 							SDI_RATE(0);	break;
	}
	
#if	defined(__MDIN_i510_REF__)			// i510 REF board
	SetDDRMAP_ddr_access_enable(0);						// disable ddr access
	SDI_RSTN(LOW);	Delay_mSec(10);	SDI_RSTN(HIGH);		// sdi-tx(gv7700) reset to setup '3G <-> 1.5G'
	MISP_SW_Reset();
#endif
	
	
	// set vpll source clock
#if defined(__USE_EXT_OSC_37M__) || defined(__USE_EXT_OSC_148M__) 
	switch(frmt)	{
		case MISP_VIDOUT_1280x720p59:		case MISP_VIDOUT_1920x1080i59:
		case MISP_VIDOUT_1920x1080p29:		case MISP_VIDOUT_1920x1080p59:
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)
		case MISP_VIDOUT_3840x2160p29:
#endif
#if	defined(__MDIN_i510_REF__)			// i510 REF board
					SDI_DIV_1001(HIGH);
#endif
					SetCLK_SrcVpllHostCLK();	break;
		default:							
#if	defined(__MDIN_i510_REF__)			// i510 REF board
					SDI_DIV_1001(LOW);
#endif
					SetCLK_SrcVpllXtal();	break;
	}
#endif
	
	
	// set usrclk_out ( host_clk_in -> userclk_out )
#if defined(__USE_4K60_INOUT__) && defined(__USE_ISP_SCLK_OUT__)
	switch(frmt)	{
		case MISP_VIDOUT_1280x720p60:	case MISP_VIDOUT_1280x720p50:
		case MISP_VIDOUT_1280x720p30:	case MISP_VIDOUT_1280x720p25:
//		case MISP_VIDOUT_1920x1080i60:	case MISP_VIDOUT_1920x1080i50:
		case MISP_VIDOUT_1920x1080p30:	case MISP_VIDOUT_1920x1080p25:
		case MISP_VIDOUT_1920x1080p24:									n=1;	m=32;	break;	// hclk*8
		
		case MISP_VIDOUT_1280x720p24:									n=1;	m=40;	break;	// hclk*10
		
		case MISP_VIDOUT_1920x1080i60:	case MISP_VIDOUT_1920x1080i50:
		case MISP_VIDOUT_1280x720p120:	case MISP_VIDOUT_1280x720p100:
		case MISP_VIDOUT_1920x1080p60:	case MISP_VIDOUT_1920x1080p50:
		case MISP_VIDOUT_2560x1440p30:	case MISP_VIDOUT_2560x1440p25:
		case MISP_VIDOUT_3840x2160p15:	case MISP_VIDOUT_3840x2160p12:	n=2;	m=32;	break;	// hclk*4
		
		case MISP_VIDOUT_3840x2160p30:	case MISP_VIDOUT_3840x2160p25:
		case MISP_VIDOUT_3840x2160p24:	case MISP_VIDOUT_4096x2160p30:
		case MISP_VIDOUT_4096x2160p25:	case MISP_VIDOUT_4096x2160p24:
		case MISP_VIDOUT_1920x1080p120:	case MISP_VIDOUT_1920x1080p100:	n=3;	m=24;	break;	// hclk*2
		
		case MISP_VIDOUT_1920x2160p60:	case MISP_VIDOUT_1920x2160p50:
		case MISP_VIDOUT_2048x2160p60:	case MISP_VIDOUT_2048x2160p50:	n=6;	m=24;	break;	// hclk*1
		
#if defined(__USE_59Hz_OUTPUT__)
		case MISP_VIDOUT_1280x720p59:	case MISP_VIDOUT_1920x1080p29:	n=1;	m=32;	break;	// hclk*8
//		case MISP_VIDOUT_1920x1080i59:									n=1;	m=32;	break;	// hclk*8
		case MISP_VIDOUT_1920x1080i59:									n=2;	m=32;	break;	// hclk*4
		case MISP_VIDOUT_1920x1080p59:									n=2;	m=32;	break;	// hclk*4
		case MISP_VIDOUT_3840x2160p29:									n=3;	m=24;	break;	// hclk*2
		case MISP_VIDOUT_4096x2160p29:									n=3;	m=24;	break;	// hclk*2
		case MISP_VIDOUT_1920x2160p59:									n=6;	m=24;	break;	// hclk*1
		case MISP_VIDOUT_2048x2160p59:									n=6;	m=24;	break;	// hclk*1
#endif
		default:														n=6;	m=24;	break;	// hclk*1
	}
	
	SetCLK_UserPLL(1,n,m,2,1) ;	// hclk(xxM) -> usrpll(297M/4) -> usrclk_out(74.25Mhz)
#endif
	
	MISP_RegField(MISP_LOCAL_ID, 0x200, 4, 1, IsOUT_MainFrmt4K()?  1:  0);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetMainSync(PMISP_OUTVIDEO_SYNC pSYNC)
{
	// sync
	MISP_MultiWrite(MISP_LOCAL_ID, 0x084, (PBYTE)&pSYNC->posHD,  6);		// set sync reg.
	MISP_MultiWrite(MISP_LOCAL_ID, 0x088, (PBYTE)&pSYNC->totHS, 30);		// set sync reg.
	
	// vclk
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	SetCLK_VideoPLL(pSYNC->vclkP, pSYNC->vclkM, pSYNC->vclkS);				// set vclk pll
#else 								// MDIN-i51X (i510, i540, i550)
	SetCLK_VideoPLL(pSYNC->vclkP, pSYNC->vclkN, pSYNC->vclkM, pSYNC->vclkOD, pSYNC->vclkS);				// set vclk pll
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void OUTCTRL_SetMainCSC(PMISP_OUTVIDEO_INFO pOUT)
{
	MISP_CSCCTRL_INFO stCSC;
	SHORT i, coef[9];	WORD csc_ctrl;
	LONG contrast, saturation, brightness, coshue, sinhue;
	
	memcpy(&stCSC, pOUT->pCSC, sizeof(MISP_CSCCTRL_INFO));
	
	saturation = (LONG)(WORD)pOUT->stTUNE.saturation;
	contrast = (LONG)(WORD)pOUT->stTUNE.contrast;
	brightness = (LONG)(WORD)pOUT->stTUNE.brightness;
	coshue = (LONG)(SHORT)MISP_CscCosHue[pOUT->stTUNE.hue];
	sinhue = (LONG)(SHORT)MISP_CscSinHue[pOUT->stTUNE.hue];
	for (i=0; i<9; i++) coef[i] = (SHORT)stCSC.coef[i];

	stCSC.coef[0] = CLIP12((((coef[0]*contrast)>>7)))&0xfff;
	stCSC.coef[1] = CLIP12((((coef[1]*coshue+coef[2]*sinhue)*saturation)>>17))&0xfff;
	stCSC.coef[2] = CLIP12((((coef[2]*coshue-coef[1]*sinhue)*saturation)>>17))&0xfff;
	stCSC.coef[3] = CLIP12((((coef[3]*contrast)>>7)))&0xfff;
	stCSC.coef[4] = CLIP12((((coef[4]*coshue+coef[5]*sinhue)*saturation)>>17))&0xfff;
	stCSC.coef[5] = CLIP12((((coef[5]*coshue-coef[4]*sinhue)*saturation)>>17))&0xfff;
	stCSC.coef[6] = CLIP12((((coef[6]*contrast)>>7)))&0xfff;
	stCSC.coef[7] = CLIP12((((coef[7]*coshue+coef[8]*sinhue)*saturation)>>17))&0xfff;
	stCSC.coef[8] = CLIP12((((coef[8]*coshue-coef[7]*sinhue)*saturation)>>17))&0xfff;
	stCSC.in[0]	  = CLIP12((((SHORT)stCSC.in[0])+(brightness-128)*2))&0xfff;

#if MISP_TUNE_RGB_GAIN_OFFSET == 1
	if( (pOUT->mode==MISP_OUT_RGB444_8)||(pOUT->mode==MISP_OUT_RGB444_10) ) {	// only apply for RGB output
		stCSC.coef[0] = CLIP12((((coef[0]*contrast*(LONG)pOUT->stGAIN.g_gain)>>14)))&0xfff;
		stCSC.coef[3] = CLIP12((((coef[0]*contrast*(LONG)pOUT->stGAIN.b_gain)>>14)))&0xfff;
		stCSC.coef[6] = CLIP12((((coef[0]*contrast*(LONG)pOUT->stGAIN.r_gain)>>14)))&0xfff;
		stCSC.out[0]  = CLIP12((((SHORT)pOUT->stGAIN.g_offset)-128)*2+stCSC.out[0])&0xfff;
		stCSC.out[1]  = CLIP12((((SHORT)pOUT->stGAIN.b_offset)-128)*2+stCSC.out[1])&0xfff;
		stCSC.out[2]  = CLIP12((((SHORT)pOUT->stGAIN.r_offset)-128)*2+stCSC.out[2])&0xfff;
	}
#endif

	//update coefficient
	MISP_MultiWrite(MISP_LOCAL_ID, 0x072, (PBYTE)&stCSC, sizeof(MISP_CSCCTRL_INFO));

	//configure CSC
//	csc_ctrl = ((pOUT->mode==MISP_OUT_RGB444_8)||(pOUT->mode==MISP_OUT_RGB444_10))?0x01fe:0x01fc;
	csc_ctrl = 0x01fc;
	MISP_RegWrite(MISP_LOCAL_ID, 0x081, csc_ctrl);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetMainDisp(WORD flag)
{
	BOOL main_disp_en = (flag&MISP_MAIN_DISPLAY_ON)?	ON:	OFF;
	BOOL main_freeze  = (flag&MISP_MAIN_FREEZE_ON)?		ON:	OFF;
	BOOL main_wr_stop = (flag&MISP_MAIN_FREEZE_ON)?		ON:	OFF;
	BOOL main_darkscr = (flag&MISP_MAIN_DARKSCR_ON)?	ON:	OFF;
	BYTE main_testptn = (flag&MISP_MAIN_OUT_TP_MASK)>>8;

	MISP_RegField(MISP_LOCAL_ID, 0x040,  5, 1, main_disp_en);	// 0:disable, 1:enable
	MISP_RegField(MISP_LOCAL_ID, 0x040,  0, 2, main_freeze<<1 | main_wr_stop);
	MISP_RegField(MISP_LOCAL_ID, 0x043,  1, 1, main_darkscr);
	MISP_RegField(MISP_LOCAL_ID, 0x043,  2, 4, main_testptn);
}

//------------------------------------------------------------------------------------
static BYTE OUTCTRL_LoadMainFrmt(POUT_CTRL_PARAM pCTRL)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
	
	EEPROM_MultiRead(EEPROM_SETUP_OUTFRMT, (PBYTE)&buff[0], sizeof(buff));
	
	if((buff[0] == EEPROM_SAVE_DONE) && (buff[1] < MISP_VIDOUT_FORMAT_END))	return buff[1];
	else																		return	pCTRL->stOUT_m.frmt;
#else
	return		pCTRL->stOUT_m.frmt;
#endif
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetAuxFrmt(BYTE frmt)
{
	PMISP_OUTVIDEO_INFO pOUT=&stOUTCTRL.stOUT_x;
	memcpy(&pOUT->stSYNC, &defMISPOutSync[frmt],  sizeof(MISP_OUTVIDEO_SYNC));	// copy sync info.
	memcpy(&pOUT->stATTB, &defMISPOutVideo[frmt], sizeof(MISP_OUTVIDEO_ATTB));	// copy v.out info.
	memset(&pOUT->stFINE, 0, sizeof(MISP_OUTVIDEO_FINE));

	pOUT->stSYNC.posFLD |= (pOUT->stATTB.attb&MISP_SCANTYPE_PROG)? 0 : (1<<15);	// 1:interlace out, 0:prog.
	
#if defined(__USE_MDIN_i500__)		// mdin-i500 only
	pOUT->stSYNC.bgnHA -= 33;		pOUT->stSYNC.endHA -= 33;
#endif
	
	if (pOUT->mode==MISP_OUT_MUX656_8 || pOUT->mode==MISP_OUT_MUX656_10) {	// for 656 out
		pOUT->stSYNC.totHS *= 2;		pOUT->stSYNC.vclkS /= 2;
		pOUT->stSYNC.bgnHA *= 2;		pOUT->stSYNC.endHA *= 2;
		pOUT->stSYNC.bgnHS *= 2;		pOUT->stSYNC.endHS *= 2;
		pOUT->stATTB.Hact *= 2;
	}
}

#if defined(__USE_CMOS_AUX_SYNC_OUT__)
//------------------------------------------------------------------------------------
static void OUTCTRL_SetAuxCmosSync(void)
{
	WORD htot=4368, vtot=2200;
	
#if defined(__USE_CMOS_IMX226__)
#if defined(__USE_IMX226_CLK_MATCH_SCLK_)
	htot = 4368/4;
#else
	htot = 4368;
#endif
	vtot = 2200;

#elif defined(__USE_CMOS_IMX274__)
	htot = 2112;
	vtot = 4550;

#elif defined(__USE_CMOS_IMX326__)
	htot = 2000;
	vtot = 4550;
#endif
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x171, htot);		//aux_htotal_size
	MISP_RegWrite(MISP_LOCAL_ID, 0x176, vtot);		//aux_vtotal_size
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x174, 45);		//aux_hsync_start
	MISP_RegWrite(MISP_LOCAL_ID, 0x175,  1);		//aux_hsync_end
	MISP_RegWrite(MISP_LOCAL_ID, 0x179,  6);		//aux_vsync_start
	MISP_RegWrite(MISP_LOCAL_ID, 0x17A,  1);		//aux_vsync_end
	
	MISP_RegField(MISP_LOCAL_ID, 0x17F, 15, 1, 0);		//aux_interlaced_out_en
}
#endif

//------------------------------------------------------------------------------------
static void OUTCTRL_SetAuxSync(PMISP_OUTVIDEO_SYNC pSYNC)
{
	// sync
	MISP_RegWrite(MISP_LOCAL_ID, 0x16E, pSYNC->posHD);						// set sync reg.
	MISP_RegField(MISP_LOCAL_ID, 0x16F, 0, 11, pSYNC->posVD);				// set sync reg.
	MISP_MultiWrite(MISP_LOCAL_ID, 0x170, (PBYTE)&pSYNC->posVB, 32);		// set sync reg.

#if defined(__USE_CMOS_AUX_SYNC_OUT__)
	OUTCTRL_SetAuxCmosSync();
#endif
	
	// vclk
#if defined(__MDIN_i550_USB_V100__)
	SetCLK_UserPLL(pSYNC->vclkP, pSYNC->vclkN, pSYNC->vclkM, pSYNC->vclkOD, pSYNC->vclkS);
#else
	SetCLK_AuxVideoPLL(pSYNC->xclkS, pSYNC->xclkF, pSYNC->xclkT);			// set vclk pll
#endif
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetAuxMode(BYTE mode)
{
	BOOL	out_444, out_bt656, dtr_enc_en, out_8bit;
	BOOL	dig_io_8bit, dig_io_sync;
	
	switch(mode)	{
		case  MISP_OUT_RGB444_8:	out_444=1;  out_bt656=0;  dtr_enc_en=0;  out_8bit=1;  break;
		case  MISP_OUT_RGB444_10:	out_444=1;  out_bt656=0;  dtr_enc_en=0;  out_8bit=0;  break;
		case  MISP_OUT_YUV444_8:	out_444=1;  out_bt656=0;  dtr_enc_en=0;  out_8bit=1;  break;
		case  MISP_OUT_YUV444_10:	out_444=1;  out_bt656=0;  dtr_enc_en=0;  out_8bit=0;  break;
		case  MISP_OUT_EMB422_8:	out_444=0;  out_bt656=0;  dtr_enc_en=1;  out_8bit=1;  break;
		case  MISP_OUT_EMB422_10:	out_444=0;  out_bt656=0;  dtr_enc_en=1;  out_8bit=0;  break;
		case  MISP_OUT_SEP422_8:	out_444=0;  out_bt656=0;  dtr_enc_en=0;  out_8bit=1;  break;
		case  MISP_OUT_SEP422_10:	out_444=0;  out_bt656=0;  dtr_enc_en=0;  out_8bit=0;  break;
		case  MISP_OUT_MUX656_8:	out_444=0;  out_bt656=1;  dtr_enc_en=1;  out_8bit=1;  break;
		case  MISP_OUT_MUX656_10:	out_444=0;  out_bt656=1;  dtr_enc_en=1;  out_8bit=0;  break;
		default:					out_444=0;  out_bt656=0;  dtr_enc_en=1;  out_8bit=0;  break;
	}
	MISP_RegField(MISP_LOCAL_ID, 0x145, 15, 1, out_444);		// aux_out_444_en (0:422, 1:444)
	MISP_RegField(MISP_LOCAL_ID, 0x145,  7, 1, out_bt656);		// aux_out_bt656_en (0:422/444, 1:656)
	MISP_RegField(MISP_LOCAL_ID, 0x145,  8, 1, dtr_enc_en);		// aux_out_insert_dtr (0:SAV/EAV disable, 1:SAV/EAV enable)
	MISP_RegField(MISP_LOCAL_ID, 0x145,  9, 1, out_8bit);		// aux_out_8bit_en (0:10bit, 1:8bit)
	
	dig_io_8bit = out_8bit;
	dig_io_sync = dtr_enc_en;
	
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	MISP_RegField(MISP_LOCAL_ID, 0x0A6,  7, 1, dig_io_8bit);	// dig_io_out_mode_aux_bit (0:10bit, 1:8bit+2gpio)
	MISP_RegField(MISP_LOCAL_ID, 0x0A6,  6, 1, dig_io_sync);	// dig_io_out_mode_aux_sync (0:sep-sync, 1:emb-sync)
#else 												// MDIN-i51X (i510, i540, i550)
#if defined(__MDIN_i550_USB_V100__)
	MISP_RegField(MISP_LOCAL_ID, 0x0A6,  12, 1, 1);	// hvf_dig_out2_use_en (Enable AUX sync output)
	MISP_RegField(MISP_LOCAL_ID, 0x0A6,  13, 1, 1);	// dual_edge_wide_out_mux_sel_inv
#endif
	MISP_RegField(MISP_LOCAL_ID, 0x0A6, 10, 1, dig_io_8bit?0:1);	// dig_io_out_aux_tenbit_en (0:8bit, 1:10bit)
	MISP_RegField(MISP_LOCAL_ID, 0x0A6,  8, 1, dig_io_8bit);	// dig_io_out_mode_aux_bit (0:10bit, 1:8bit+2gpio)
	MISP_RegField(MISP_LOCAL_ID, 0x0A6,  7, 1, dig_io_sync);	// dig_io_out_mode_aux_sync (0:sep-sync, 1:emb-sync)
#endif
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetAuxDisp(WORD flag)
{
	BOOL aux_disp_en = (flag&MISP_AUX_DISPLAY_ON)?	ON:	OFF;
	BOOL aux_freeze  = (flag&MISP_AUX_FREEZE_ON)?	ON:	OFF;
	BOOL aux_darkscr = (flag&MISP_AUX_DARKSCR_ON)?	ON:	OFF;
	BYTE aux_testptn = (flag&MISP_AUX_OUT_TP_MASK)>>12;
	BOOL aux_overlay = (flag&MISP_AUX_SYNC_PIP_EN)? ON: OFF;
	BYTE aux_clk_sel = (flag&MISP_AUX_SYNC_PIP_EN)? 0: 1;

	if(GetOUT_AuxUsedByFunc()) 		aux_clk_sel = 0; 		// aux video data used by user function(DIS, FD, etc,,)
	
	MISP_RegField(MISP_LOCAL_ID, 0x142,  0, 2, aux_freeze<<1 | aux_disp_en);
	MISP_RegField(MISP_LOCAL_ID, 0x14C,  9, 1, aux_darkscr);
	MISP_RegField(MISP_LOCAL_ID, 0x14C, 10, 4, aux_testptn);
	MISP_RegField(MISP_LOCAL_ID, 0x144,  6, 1, aux_overlay);
#if defined(__USE_CMOS_AUX_SYNC_OUT__) && (defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__))	// Use AUX sync until i5x0 revision for Sensor Slave Mode
	SetCLK_SrcAuxOut(3);  // 0:vclk, 1:venc_clk, 2:axclk, 3:aux_mclk
#elif defined(__MDIN_i550_USB_V100__)
	SetCLK_SrcAuxOut(3);  // 0:vclk, 1:venc_clk, 2:axclk, 3:aux_mclk
#else
	SetCLK_SrcAuxOut(aux_clk_sel);  // 0:vclk, 1:venc_clk, 2:axclk
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void OUTCTRL_SetAuxCSC(PMISP_OUTVIDEO_INFO pOUT)
{
	MISP_CSCCTRL_INFO stCSC;
	SHORT i, coef[9];
	LONG contrast, saturation, brightness, coshue, sinhue;
	WORD csc_ctrl;
	
	memcpy(&stCSC, pOUT->pCSC, sizeof(MISP_CSCCTRL_INFO));
#if defined(__USE_MDIN_i500__)
	for (i=0; i<3; i++) stCSC.in[i] = (SHORT)stCSC.in[i]/4;
	for (i=0; i<3; i++) stCSC.out[i]= (SHORT)stCSC.out[i]/4;
#endif

	saturation = (LONG)(WORD)pOUT->stTUNE.saturation;
	contrast = (LONG)(WORD)pOUT->stTUNE.contrast;
	brightness = (LONG)(WORD)pOUT->stTUNE.brightness;
	coshue = (LONG)(SHORT)MISP_CscCosHue[pOUT->stTUNE.hue];
	sinhue = (LONG)(SHORT)MISP_CscSinHue[pOUT->stTUNE.hue];
	for (i=0; i<9; i++) coef[i] = (SHORT)stCSC.coef[i];

	stCSC.coef[0] = CLIP12((((coef[0]*contrast)>>7)))&0xfff;
	stCSC.coef[1] = CLIP12((((coef[1]*coshue+coef[2]*sinhue)*saturation)>>17))&0xfff;
	stCSC.coef[2] = CLIP12((((coef[2]*coshue-coef[1]*sinhue)*saturation)>>17))&0xfff;
	stCSC.coef[3] = CLIP12((((coef[3]*contrast)>>7)))&0xfff;
	stCSC.coef[4] = CLIP12((((coef[4]*coshue+coef[5]*sinhue)*saturation)>>17))&0xfff;
	stCSC.coef[5] = CLIP12((((coef[5]*coshue-coef[4]*sinhue)*saturation)>>17))&0xfff;
	stCSC.coef[6] = CLIP12((((coef[6]*contrast)>>7)))&0xfff;
	stCSC.coef[7] = CLIP12((((coef[7]*coshue+coef[8]*sinhue)*saturation)>>17))&0xfff;
	stCSC.coef[8] = CLIP12((((coef[8]*coshue-coef[7]*sinhue)*saturation)>>17))&0xfff;
	stCSC.in[0]	  = CLIP12((((SHORT)stCSC.in[0])+(brightness-128)*2))&0xfff;

#if MISP_TUNE_RGB_GAIN_OFFSET == 1
	if( (pOUT->mode==MISP_OUT_RGB444_8)||(pOUT->mode==MISP_OUT_RGB444_10) ) {	// only apply for RGB output
		stCSC.coef[0] = CLIP12((((coef[0]*contrast*(LONG)pOUT->stGAIN.g_gain)>>14)))&0xfff;
		stCSC.coef[3] = CLIP12((((coef[0]*contrast*(LONG)pOUT->stGAIN.b_gain)>>14)))&0xfff;
		stCSC.coef[6] = CLIP12((((coef[0]*contrast*(LONG)pOUT->stGAIN.r_gain)>>14)))&0xfff;
		stCSC.out[0]  = CLIP12((((SHORT)pOUT->stGAIN.g_offset)-128)*2+stCSC.out[0])&0xfff;
		stCSC.out[1]  = CLIP12((((SHORT)pOUT->stGAIN.b_offset)-128)*2+stCSC.out[1])&0xfff;
		stCSC.out[2]  = CLIP12((((SHORT)pOUT->stGAIN.r_offset)-128)*2+stCSC.out[2])&0xfff;
	}
#endif

	//update coefficient
	MISP_MultiWrite(MISP_LOCAL_ID, 0x191, (PBYTE)&stCSC, sizeof(MISP_CSCCTRL_INFO));

	//configure CSC
	csc_ctrl = ((pOUT->mode==MISP_OUT_RGB444_8)||(pOUT->mode==MISP_OUT_RGB444_10))?0x7f3c:0x7f7c;
	MISP_RegWrite(MISP_LOCAL_ID, 0x190, csc_ctrl);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetEncFrmt(BYTE frmt)
{
	BYTE	nID, venc_csc_sel, venc_mod_format, venc_clk_sel, aux_frmt=stOUTCTRL.stOUT_x.frmt;
	
	switch(frmt) {
		case MISP_VID_VENC_NTSC_M:
			if(aux_frmt==MISP_VIDOUT_960x480i60)	{nID = 1; venc_csc_sel=0; venc_mod_format=4;}
			else									{nID = 0; venc_csc_sel=0; venc_mod_format=0;}
			break;	
		case MISP_VID_VENC_NTSC_J:		nID = 0; venc_csc_sel=1; venc_mod_format=0;	break;
		case MISP_VID_VENC_NTSC_443:	nID = 0; venc_csc_sel=0; venc_mod_format=1;	break;
		case MISP_VID_VENC_PAL_B:		nID = 2; venc_csc_sel=2; venc_mod_format=1;	break;
		case MISP_VID_VENC_PAL_D:		nID = 2; venc_csc_sel=2; venc_mod_format=1;	break;
		case MISP_VID_VENC_PAL_G:		nID = 2; venc_csc_sel=2; venc_mod_format=1;	break;
		case MISP_VID_VENC_PAL_H:		nID = 2; venc_csc_sel=2; venc_mod_format=1;	break;
		case MISP_VID_VENC_PAL_I:		nID = 2; venc_csc_sel=2; venc_mod_format=1;	break;
		case MISP_VID_VENC_PAL_M:		nID = 0; venc_csc_sel=0; venc_mod_format=2;	break;
		case MISP_VID_VENC_PAL_N:		nID = 2; venc_csc_sel=0; venc_mod_format=1;	break;
		case MISP_VID_VENC_PAL_Nc:		nID = 2; venc_csc_sel=2; venc_mod_format=3;	break;
		default:						nID = 0; venc_csc_sel=0; venc_mod_format=0;	break;
	}
	
	if(nID==1)		venc_clk_sel=3;		// 960H : venc clk(xtal2-36Mhz)
	else			venc_clk_sel=1;		// 720H : venc clk(xtal-27Mhz)
	
	MISP_RegField(MISP_LOCAL_ID, 0x1e0, 0, 11, MISP_Encoder_Default[nID].coef[0]);
	MISP_MultiWrite(MISP_LOCAL_ID, 0x1e1, (PBYTE)&MISP_Encoder_Default[nID].coef[1], 36);
	
	MISP_RegField(MISP_LOCAL_ID, 0x1e0, 14, 2, venc_csc_sel);		// 0:NTSC-M/443,PAL-M/N, 1:NTSC-J, 2:PAL-B/D/G/H/I/Nc
	MISP_RegField(MISP_LOCAL_ID, 0x1e8,  0, 3, venc_mod_format);	// 0:NTSC-M/J, 1:PAL-B/D/G/H/I/N, NTSC443, 2:PAL-M, 3:PAL-Nc, 4:Manual
	SetCLK_SrcENC(venc_clk_sel);			// 0:vclk, 1:xtal, 2:clk_m_enc_int, 3:xtal2
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetEncCtrl(LONG ctrl)
{
	BOOL 	venc_no_color = (ctrl & OUTCTRL_VENC_COLOR_DIS)?  1:  0;
	BOOL 	venc_burst_insert_dis = (ctrl & OUTCTRL_VENC_BURST_DIS)?  1:  0;
	BOOL	venc_blue_screen = (ctrl & OUTCTRL_VENC_BLUESCR_EN)?  1:  0;
	
	MISP_RegField(MISP_LOCAL_ID, 0x1e4, 10, 1, venc_blue_screen);		// 0:blue screen disable, 1:blue screen enable
	MISP_RegField(MISP_LOCAL_ID, 0x1e4, 13, 1, venc_no_color);			// 0:color enable, 1:color disable
	MISP_RegField(MISP_LOCAL_ID, 0x1e5, 13, 1, venc_burst_insert_dis);	// 0:burst enable, 1:burst disable
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetBWExten(BYTE flag)
{
	if(flag&MISP_ENH_BWEXT_ON)  MISP_RegField(MISP_LOCAL_ID, 0x3c0, 0, 1, 1);  // bw_ext_enable
	else                        MISP_RegField(MISP_LOCAL_ID, 0x3c0, 0, 1, 0);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetLtiCtrl(BYTE flag)
{
	if(flag&MISP_ENH_LTI_ON)  MISP_RegWrite(MISP_LOCAL_ID, 0x063, 0x02f9);  // lti_ctrl2
	else                      MISP_RegWrite(MISP_LOCAL_ID, 0x063, 0x0000);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetCtiCtrl(BYTE flag)
{
	if(flag&MISP_ENH_CTI_ON)  MISP_RegWrite(MISP_LOCAL_ID, 0x065, 0x02f9);  // cti_b_ctrl2
	else                      MISP_RegWrite(MISP_LOCAL_ID, 0x065, 0x0000);

	if(flag&MISP_ENH_CTI_ON)  MISP_RegWrite(MISP_LOCAL_ID, 0x067, 0x02f9);  // cti_r_ctrl2
	else                      MISP_RegWrite(MISP_LOCAL_ID, 0x067, 0x0000);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetColorEnh(BYTE flag)
{
	if(flag&MISP_ENH_COLOR_ON)  MISP_RegField(MISP_LOCAL_ID, 0x06c, 0, 1, 1);  // color_enh_flt_en
	else                        MISP_RegField(MISP_LOCAL_ID, 0x06c, 0, 1, 0);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetPipBlendCtrl(PMISP_PIP_CONTROL pPIP)
{
	BYTE i ;
	WORD base_addr ;
	WORD sub_addr ;
	
	for ( i = 0 ; i < 4 ; i++ )
	{
		if ( pPIP->pip_blend_window[i].ctrl_update == 0 ) continue ;
		else
		{
			if ( (pPIP->pip_blend_window[i].ctrl_update&0x01) == 1 ) base_addr = i*5 ;
			else base_addr = i*5+4 ;
				
			if ( (pPIP->pip_blend_window[i].ctrl_update&0x04) == 4 ) sub_addr = 20+i ;
			else sub_addr = 0 ;

			MISP_RegWrite(MISP_LOCAL_ID, 0x06E, 0x8000|base_addr) ;
			
			if ( base_addr == (i*5) )
			{
				MISP_RegWrite(MISP_LOCAL_ID, 0x06F, pPIP->pip_blend_window[i].grad_blend_sh) ;
				MISP_RegWrite(MISP_LOCAL_ID, 0x06F, pPIP->pip_blend_window[i].grad_blend_sv) ;
				MISP_RegWrite(MISP_LOCAL_ID, 0x06F, pPIP->pip_blend_window[i].grad_blend_eh) ;
				MISP_RegWrite(MISP_LOCAL_ID, 0x06F, pPIP->pip_blend_window[i].grad_blend_ev) ;	
			}
			
			if ( (pPIP->pip_blend_window[i].ctrl_update) > 1 )
			{
				MISP_RegWrite(MISP_LOCAL_ID, 0x06F, (pPIP->pip_blend_window[i].blend_sel<<15)|
													(pPIP->pip_blend_window[i].grad_blend_en<<14)|	 // gradtion ramp on,off				
													(pPIP->pip_blend_window[i].grad_blend_mode<<13)| // ('0'=H, '1'=V)									
													(pPIP->pip_blend_window[i].grad_blend_init)) ;
			}
			
			if 	( sub_addr == (20+i) )
			{
				MISP_RegWrite(MISP_LOCAL_ID, 0x06E, 0x8000|sub_addr) ;
				MISP_RegWrite(MISP_LOCAL_ID, 0x06F, pPIP->pip_blend_window[i].grad_blend_step) ;
			}				
			pPIP->pip_blend_window[i].ctrl_update = 0 ;
		} // if ( pPIP->pip_blend_window[i].ctrl_update == 0 ) 
	} // for ( i = 0 ; i < 4 ; i ++)
}
//------------------------------------------------------------------------------------
static void OUTCTRL_SetPipCtrl(PMISP_PIP_CONTROL pPIP)
{
	//pip blend (0:opaque, 15:transparent)
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	MISP_RegField(MISP_LOCAL_ID, 0x06C, 2, 3, pPIP->blend_lev);
#else 												// MDIN-i51X (i510, i540, i550)
	MISP_RegField(MISP_LOCAL_ID, 0x06C, 2, 4, pPIP->blend_lev);  //i51x, ext to 4bits
#endif

	//chroma key
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	MISP_RegField(MISP_LOCAL_ID, 0x2F0, 15, 1, pPIP->key_en);
	MISP_RegWrite(MISP_LOCAL_ID, 0x2F5, MAKEWORD(pPIP->key_y_u,  pPIP->key_y_l ));
	MISP_RegWrite(MISP_LOCAL_ID, 0x2F6, MAKEWORD(pPIP->key_cb_u, pPIP->key_cb_l));
	MISP_RegWrite(MISP_LOCAL_ID, 0x2F7, MAKEWORD(pPIP->key_cr_u, pPIP->key_cr_l));
	
	OUTCTRL_SetPipBlendCtrl(pPIP) ;
#endif
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetExtLock(PMISP_EXTLOCK_CTRL pExtLock)
{
	BYTE lock_source = pExtLock->lock_source ;
	BYTE lock_mode = pExtLock->lock_mode ;
	WORD sync_mode = pExtLock->sync_mode ;
	
	MISP_RegField(MISP_LOCAL_ID, 0x098, 3,  3, lock_source);
	
	MISP_RegField(MISP_LOCAL_ID, 0x097,  4, 12, pExtLock->hs_dly);	// output_sync_reset_dly
	MISP_RegField(MISP_LOCAL_ID, 0x099,  0, 13, pExtLock->vs_dly);	// output_sync_reset_thres
	
	if ( lock_mode == 1 ) // Main sync free run with single reset pulse [ Ext sync property is similar to Main sync ]
	{
		MISP_RegField(MISP_LOCAL_ID, 0x098, 0,  1, 1);	// main_free_run [free-run mode]
		MISP_RegField(MISP_LOCAL_ID, 0x098, 8,  1, 1);	// lock_to_ext_sync
		MISP_RegField(MISP_LOCAL_ID, 0x099, 15,  1, 0);	// output_sync_reset_en : 0 -> 1 sync_gen reset..
		MISP_RegField(MISP_LOCAL_ID, 0x099, 15,  1, 1);
	}
	else if ( lock_mode == 2 ) // Main sync lock mode with sync reset delay enable
	{		
		MISP_RegField(MISP_LOCAL_ID, 0x098, 0,  1, 0);	// main_free_run [frame-lock mode]
		MISP_RegField(MISP_LOCAL_ID, 0x098, 8,  2, 3);	// sync_reset_dly_en , lock_to_ext_sync
	}
	else if ( lock_mode == 3 ) // Main sync free run with single reset pulse + delay enable [ Ext sync property is similar to Main sync ]
	{
		MISP_RegField(MISP_LOCAL_ID, 0x098, 0,  1, 1);	// main_free_run [free-run mode]
		MISP_RegField(MISP_LOCAL_ID, 0x098, 8,  2, 3);	// lock_to_ext_sync
		MISP_RegField(MISP_LOCAL_ID, 0x099, 15,  1, 0);	// output_sync_reset_en : 0 -> 1 sync_gen reset..
		MISP_RegField(MISP_LOCAL_ID, 0x099, 15,  1, 1);
	}	
	else // Main sync free run [normal mode]
	{
		MISP_RegField(MISP_LOCAL_ID, 0x098, 0,  1, 1);	// main_free_run [free-run mode]
		MISP_RegField(MISP_LOCAL_ID, 0x098, 8,  2, 0);	// sync_reset_dly_en , lock_to_ext_sync
	}
	
	if ( sync_mode == 1 || sync_mode == 2 ) // Frame memory skip mode [ Direct sync from input top ]
	{
		MISP_RegField(MISP_LOCAL_ID, 0x06C, 7,  2, 3); // bypass_sync_from_input , bypass_from_input
		MISP_RegField(MISP_LOCAL_ID, 0x06C, 1,  1, 1); // color_enh_flt_444_en
		if ( sync_mode == 1 ) MISP_RegField(MISP_LOCAL_ID, 0x047, 15,  1, 1); // bypass sync from after input CSC
		else MISP_RegField(MISP_LOCAL_ID, 0x047, 15,  1, 0); // bypass sync from before input CSC
	}
	else // normal mode [ Main sync ]
	{
		MISP_RegField(MISP_LOCAL_ID, 0x06C, 7,  2, 0);
		MISP_RegField(MISP_LOCAL_ID, 0x06C, 1,  1, 0);	
	}
	
}
//-------------------------------------------------------------------------------------
static void OUTCTRL_SetLVDS_tx(PMISP_OUT_LVDS_CTRL pLVDS_tx)
{
	MISP_RegField(MISP_LOCAL_ID, 0x0A8,  0, 1, pLVDS_tx->lvds_tx_enable);	
	MISP_RegField(MISP_LOCAL_ID, 0x0A8,  1, 1, pLVDS_tx->lvds_tx_mode);	
	MISP_RegField(MISP_LOCAL_ID, 0x0A8,  2, 2, pLVDS_tx->lvds_tx_map);	
	MISP_RegField(MISP_LOCAL_ID, 0x0A8,  4, 2, pLVDS_tx->lvds_tx_portb_ctrl);	
	
	MISP_RegField(MISP_LOCAL_ID, 0x0A8,  6, 2, pLVDS_tx->lvds_tx_cont1);
	MISP_RegField(MISP_LOCAL_ID, 0x0A8,  8, 2, pLVDS_tx->lvds_tx_cont2);	
	
	MISP_RegField(MISP_LOCAL_ID, 0x0A8,  10, 1, pLVDS_tx->lvds_tx_map_option);
	MISP_RegField(MISP_LOCAL_ID, 0x0A8,  11, 1, pLVDS_tx->lvds_tx_state);	
	MISP_RegField(MISP_LOCAL_ID, 0x0A8,  12, 1, pLVDS_tx->lvds_tx_state_retry);	

	MISP_RegField(MISP_LOCAL_ID, 0x0A8,  13, 1, pLVDS_tx->lvds_tx2_en);		
	MISP_RegField(MISP_LOCAL_ID, 0x0A8,  14, 1, pLVDS_tx->lvds_tx_data_swap1);			
	MISP_RegField(MISP_LOCAL_ID, 0x0A8,  15, 1, pLVDS_tx->lvds_tx_data_swap2);			


	MISP_RegField(MISP_LOCAL_ID, 0x0A9,  0, 1, pLVDS_tx->lvds_tx_evenodd_en);	
	MISP_RegField(MISP_LOCAL_ID, 0x0A9,  1, 1, pLVDS_tx->lvds_tx1_main_aux_sel);	
	MISP_RegField(MISP_LOCAL_ID, 0x0A9,  2, 1, pLVDS_tx->lvds_tx1_444_422_sel);	
	MISP_RegField(MISP_LOCAL_ID, 0x0A9,  3, 1, pLVDS_tx->lvds_tx2_main_aux_sel);	
	MISP_RegField(MISP_LOCAL_ID, 0x0A9,  4, 1, pLVDS_tx->lvds_tx2_444_422_sel);
	MISP_RegField(MISP_LOCAL_ID, 0x0A9,  5, 1, pLVDS_tx->lvds_tx1_hs_invert);		
	MISP_RegField(MISP_LOCAL_ID, 0x0A9,  6, 1, pLVDS_tx->lvds_tx1_vs_invert);
	MISP_RegField(MISP_LOCAL_ID, 0x0A9,  7, 1, pLVDS_tx->lvds_tx1_de_invert);	
	MISP_RegField(MISP_LOCAL_ID, 0x0A9,  8, 1, pLVDS_tx->lvds_tx2_hs_invert);	
	MISP_RegField(MISP_LOCAL_ID, 0x0A9,  9, 1, pLVDS_tx->lvds_tx2_vs_invert);		
	MISP_RegField(MISP_LOCAL_ID, 0x0A9,  10, 1, pLVDS_tx->lvds_tx2_de_invert);			

	//VCKO and LVDS 2nd Port(TCLK2) pins are common. 
	//Therefore, it is necessary to set this register when connecting TCLK2 to a panel which is absolutely necessary
	if(pLVDS_tx->lvds_tx2_en)		MISP_RegField(MISP_LOCAL_ID, 0x041,  8, 1, 1);

	
}
//------------------------------------------------------------------------------------
static void OUTCTRL_SetMainMisc(PMISP_OUTVIDEO_MISC pMISC)
{
	BYTE dither;
	
	MISP_RegField(MISP_LOCAL_ID, 0x09A,  0, 8, pMISC->back_y);
	MISP_RegField(MISP_LOCAL_ID, 0x09B,  8, 8, pMISC->back_cb);
	MISP_RegField(MISP_LOCAL_ID, 0x09B,  0, 8, pMISC->back_cr);

	//dither (8bit)
	dither=((GetOUT_MainMode()%2==0)&&(pMISC->dither))?1:0;
	MISP_RegField(MISP_LOCAL_ID, 0x0A2, 10, 3, dither?8:0);  //dither_mode (8bit)
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetAuxMisc(PMISP_OUTVIDEO_MISC pMISC)
{
	BYTE dither;
	
	MISP_RegField(MISP_LOCAL_ID, 0x14C,  0, 8, pMISC->back_y);
	MISP_RegField(MISP_LOCAL_ID, 0x14D,  8, 8, pMISC->back_cb);
	MISP_RegField(MISP_LOCAL_ID, 0x14D,  0, 8, pMISC->back_cr);

	//dither (8bit)
	dither=((GetOUT_AuxMode()%2==0)&&(pMISC->dither))?1:0;
	MISP_RegField(MISP_LOCAL_ID, 0x145, 10, 1, dither?1:0);  //aux_dither_en
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetMainAttb(PMISP_OUTVIDEO_INFO pOUT)
{
	MISP_RegField(MISP_LOCAL_ID, 0x0A2,  4, 1, pOUT->stFINE.cbcr_swap);
	MISP_RegField(MISP_LOCAL_ID, 0x097,  0, 1, pOUT->stFINE.tf_polar);  //out_positive_top_field

	MISP_RegField(MISP_LOCAL_ID, 0x0A0,  5, 1, (pOUT->stATTB.attb&MISP_POSITIVE_HSYNC)?1:0);
	MISP_RegField(MISP_LOCAL_ID, 0x0A0,  4, 1, (pOUT->stATTB.attb&MISP_POSITIVE_VSYNC)?1:0);

	MISP_RegField(MISP_LOCAL_ID, 0x0A0,  8, 2, pOUT->stFINE.pin_de);
	MISP_RegField(MISP_LOCAL_ID, 0x0A0,  2, 2, pOUT->stFINE.pin_hsync);
	MISP_RegField(MISP_LOCAL_ID, 0x0A0,  0, 2, pOUT->stFINE.pin_vsync);
	
#if defined(__USE_USB_VOUT__)
	MISP_RegField(MISP_LOCAL_ID, 0x0A0,  4, 1, 0);			// always 'negative vsync' for cyusb3014
#endif
}

//------------------------------------------------------------------------------------
static void OUTCTRL_SetAuxAttb(PMISP_OUTVIDEO_INFO pOUT)
{
	MISP_RegField(MISP_LOCAL_ID, 0x145,  6, 1, pOUT->stFINE.cbcr_swap);
	MISP_RegField(MISP_LOCAL_ID, 0x144, 15, 1, pOUT->stFINE.tf_polar);  //aux_out_positive_top_field

#if defined(__USE_CMOS_AUX_SYNC_OUT__) && (defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__))	// Use AUX sync until i5x0 revision for Sensor Slave Mode
	MISP_RegField(MISP_LOCAL_ID, 0x145, 14, 1, 0);  // 0:negative, 1:positive
	MISP_RegField(MISP_LOCAL_ID, 0x145, 13, 1, 0);  // 0:negative, 1:positive
#else
	MISP_RegField(MISP_LOCAL_ID, 0x145, 14, 1, (pOUT->stATTB.attb&MISP_POSITIVE_HSYNC)?1:0);
	MISP_RegField(MISP_LOCAL_ID, 0x145, 13, 1, (pOUT->stATTB.attb&MISP_POSITIVE_VSYNC)?1:0);
#endif

//	MISP_RegField(MISP_LOCAL_ID, 0x000,  0, 0, pOUT->stFINE.pin_de);  //always zero
	MISP_RegField(MISP_LOCAL_ID, 0x2FC,  9, 2, pOUT->stFINE.pin_hsync);
	MISP_RegField(MISP_LOCAL_ID, 0x2FC,  7, 2, pOUT->stFINE.pin_vsync);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_PinMapHandler(POUT_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&OUTCTRL_UPDATE_PINMAP)==0) return;
	pCTRL->ctrl &= (~OUTCTRL_UPDATE_PINMAP);
	
	OUTCTRL_SetPinMAP(&pCTRL->stPinMap);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_MainFrmtHandler(POUT_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&OUTCTRL_UPDATE_MAIN)==0) return;
	pCTRL->ctrl &= (~OUTCTRL_UPDATE_MAIN);
	
	OUTCTRL_SetMainMode(pCTRL->stOUT_m.mode);
	OUTCTRL_SetMainSync(&pCTRL->stOUT_m.stSYNC);
	OUTCTRL_SetMainAttb(&pCTRL->stOUT_m);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_AuxFrmtHandler(POUT_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&OUTCTRL_UPDATE_AUX)==0) return;
	pCTRL->ctrl &= (~OUTCTRL_UPDATE_AUX);
	
	OUTCTRL_SetAuxMode(pCTRL->stOUT_x.mode);
	OUTCTRL_SetAuxSync(&pCTRL->stOUT_x.stSYNC);
	OUTCTRL_SetAuxAttb(&pCTRL->stOUT_x);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_EncFrmtHandler(POUT_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&OUTCTRL_UPDATE_ENCFRMT)==0) return;
	pCTRL->ctrl &= (~OUTCTRL_UPDATE_ENCFRMT);
	
	OUTCTRL_SetEncFrmt(pCTRL->encFRMT);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_EncMiscHandler(POUT_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&OUTCTRL_UPDATE_ENCMISC)==0) return;
	pCTRL->ctrl &= (~OUTCTRL_UPDATE_ENCMISC);
	
	OUTCTRL_SetEncCtrl(pCTRL->ctrl);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_MainDispHandler(POUT_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&OUTCTRL_UPDATE_MAINDISP)==0) return;
	pCTRL->ctrl |= OUTCTRL_TRIGGER_ISP;

	if( (pCTRL->stOUT_m.turnon_dly)&&
	    (pCTRL->stOUT_m.turnon_cnt < pCTRL->stOUT_m.turnon_dly) )
	    return;  // check delay

	pCTRL->ctrl &= ~OUTCTRL_UPDATE_MAINDISP;
	pCTRL->stOUT_m.turnon_dly=0;
	OUTCTRL_SetMainDisp(pCTRL->dspFLAG);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_AuxDispHandler(POUT_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&OUTCTRL_UPDATE_AUXDISP)==0) return;
	pCTRL->ctrl |= OUTCTRL_TRIGGER_ISP;

	if( (pCTRL->stOUT_x.turnon_dly)&&
	    (pCTRL->stOUT_x.turnon_cnt < pCTRL->stOUT_x.turnon_dly) )
	    return;  // check delay

	pCTRL->ctrl &= ~OUTCTRL_UPDATE_AUXDISP;
	pCTRL->stOUT_x.turnon_dly=0;
	OUTCTRL_SetAuxDisp(pCTRL->dspFLAG);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_MainCSCHandler(POUT_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&OUTCTRL_UPDATE_MAINCSC)==0) return;
	pCTRL->ctrl &= (~OUTCTRL_UPDATE_MAINCSC);
	
	OUTCTRL_SetMainCSC(&pCTRL->stOUT_m);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_AuxCSCHandler(POUT_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&OUTCTRL_UPDATE_AUXCSC)==0) return;
	pCTRL->ctrl &= (~OUTCTRL_UPDATE_AUXCSC);
	
	OUTCTRL_SetAuxCSC(&pCTRL->stOUT_x);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_EnhanceHandler(POUT_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&OUTCTRL_UPDATE_ENHANCE)==0) return;
	pCTRL->ctrl &= (~OUTCTRL_UPDATE_ENHANCE);
	
	OUTCTRL_SetBWExten(pCTRL->enhFLAG);
	OUTCTRL_SetLtiCtrl(pCTRL->enhFLAG);
	OUTCTRL_SetCtiCtrl(pCTRL->enhFLAG);
	OUTCTRL_SetColorEnh(pCTRL->enhFLAG);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_MainMiscHandler(POUT_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&OUTCTRL_UPDATE_MAINMISC)==0) return;
	pCTRL->ctrl &= (~OUTCTRL_UPDATE_MAINMISC);

	OUTCTRL_SetMainMisc(&pCTRL->stOUT_m.stMISC);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_AuxMiscHandler(POUT_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&OUTCTRL_UPDATE_AUXMISC)==0) return;
	pCTRL->ctrl &= (~OUTCTRL_UPDATE_AUXMISC);	

	OUTCTRL_SetAuxMisc(&pCTRL->stOUT_x.stMISC);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_PipHandler(POUT_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&OUTCTRL_UPDATE_PIP)==0) return;
	pCTRL->ctrl &= (~OUTCTRL_UPDATE_PIP);
	
	OUTCTRL_SetPipCtrl(&pCTRL->stPIPctl);
}

//------------------------------------------------------------------------------------
static void OUTCTRL_ExtLockHandler(POUT_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&OUTCTRL_UPDATE_EXTLOCK)==0) return;
	pCTRL->ctrl &= (~OUTCTRL_UPDATE_EXTLOCK);
	
	OUTCTRL_SetExtLock(&pCTRL->stExtLock);
}

//--------------------------------------------------------------------------------------
static void Init_OUT_REGISTER( POUT_CTRL_PARAM pCTRL ) 
{
	PMISP_BWPOINT_COEF pCoef;

	MISP_RegWrite(MISP_LOCAL_ID, 0x055, 0x0000);	// set OUTPUT


	MISP_RegWrite(MISP_LOCAL_ID, 0x082, 0x0000);	// overlay_demo_ctrl

	MISP_RegWrite(MISP_LOCAL_ID, 0x0a0, 0x0000);	// out_sync_ctrl
	MISP_RegWrite(MISP_LOCAL_ID, 0x0a1, 0x0000);	// vid_act_ctrl
	MISP_RegWrite(MISP_LOCAL_ID, 0x0a3, 0x0000);	// dac_control
	MISP_RegWrite(MISP_LOCAL_ID, 0x0a4, 0x1080);	// digital_blank_data
	MISP_RegWrite(MISP_LOCAL_ID, 0x0a5, 0x0120);	// out_mux_ctrl

	//aux
	MISP_RegWrite(MISP_LOCAL_ID, 0x141, 0x0000);	// aux_etc_ctrl1(default:4002)
	MISP_RegField(MISP_LOCAL_ID, 0x141,  7,  1, 1);	// aux_digital_out_en
	MISP_RegField(MISP_LOCAL_ID, 0x141,  1,  1, 1);	// aux_rd_ext_buf
	MISP_RegWrite(MISP_LOCAL_ID, 0x143, 0xc0c8);	// aux_sync_ctrl
	MISP_RegField(MISP_LOCAL_ID, 0x144,  0,  4, 9);	// aux_rpt_chk_offset
	
	//sync info
	MISP_RegField(MISP_LOCAL_ID, 0x16F, 11, 4, 8);	// aux vdpulse_mfc_pos
	MISP_RegField(MISP_LOCAL_ID, 0x16F, 15, 1, 1);	// aux vdpulse_mfc_pos_en
	
	//sync
	MISP_RegWrite(MISP_LOCAL_ID, 0x083, 0x000a);	// out-sync // jwlee 20131016
	MISP_RegField(MISP_LOCAL_ID, 0x097,  2, 2, 1);	// vact_ipc_lead
	MISP_RegField(MISP_LOCAL_ID, 0x098, 15, 1, 1);	// main vdpulse_mfc_pos_en
	MISP_RegField(MISP_LOCAL_ID, 0x098, 11, 4, 10);	// main vdpulse_mfc_pos
	MISP_RegField(MISP_LOCAL_ID, 0x098,  9, 1, 1);	// sync_reset_dly_en, 1:enable sync delay control(hulee, 2017.02.07)
	MISP_RegField(MISP_LOCAL_ID, 0x098,  0, 1, 1);	// main_free_run
	MISP_RegWrite(MISP_LOCAL_ID, 0x09d, 0x00a4);	// main_adp_fptr_ctrl

	//color enh flt
	MISP_RegWrite(MISP_LOCAL_ID, 0x068, 0x0100);	// color_enh_flt0
	MISP_RegWrite(MISP_LOCAL_ID, 0x069, 0x0080);	// color_enh_flt1
	MISP_RegWrite(MISP_LOCAL_ID, 0x06a, 0x0000);	// color_enh_flt2
	MISP_RegWrite(MISP_LOCAL_ID, 0x06b, 0x0000);	// color_enh_flt3

	//BW extension
	pCoef = (PMISP_BWPOINT_COEF)&defMISPBWPoint[0];
	MISP_RegWrite(MISP_LOCAL_ID, 0x3d0, MAKEWORD(pCoef->src[0],pCoef->src[1]));
	MISP_RegWrite(MISP_LOCAL_ID, 0x3d1, MAKEWORD(pCoef->src[2],pCoef->src[3]));
	MISP_RegWrite(MISP_LOCAL_ID, 0x3d2, MAKEWORD(pCoef->out[0],pCoef->out[1]));
	MISP_RegWrite(MISP_LOCAL_ID, 0x3d3, MAKEWORD(pCoef->out[2],pCoef->out[3]));

	//clip always enable for sdi output.
	MISP_RegField(MISP_LOCAL_ID, 0x0A2, 3, 1, 1);	// [dig_clip_en] 1:clip to SMPTE-274M
	MISP_RegField(MISP_LOCAL_ID, 0x0A2, 8, 2, 2);	// [dig_clip_mode] 0:YCbCr mode, 2:extended range (4 ~ 1016)
	
	OUTCTRL_SetPinMAP(&pCTRL->stPinMap);

	OUTCTRL_SetMainFrmt(pCTRL->stOUT_m.frmt);		// set main(sync, attb) param
	OUTCTRL_SetMainMode(pCTRL->stOUT_m.mode);
	OUTCTRL_SetMainSync(&pCTRL->stOUT_m.stSYNC);
	OUTCTRL_SetMainAttb(&pCTRL->stOUT_m);
	OUTCTRL_SetMainDisp(pCTRL->dspFLAG);
	OUTCTRL_SetMainCSC(&pCTRL->stOUT_m);
	OUTCTRL_SetMainMisc(&pCTRL->stOUT_m.stMISC);
	
	OUTCTRL_SetAuxFrmt(pCTRL->stOUT_x.frmt);		// set aux(sync, attb) param
	OUTCTRL_SetAuxMode(pCTRL->stOUT_x.mode);
	OUTCTRL_SetAuxSync(&pCTRL->stOUT_x.stSYNC);
	OUTCTRL_SetAuxAttb(&pCTRL->stOUT_x);
	OUTCTRL_SetAuxDisp(pCTRL->dspFLAG);
	OUTCTRL_SetAuxCSC(&pCTRL->stOUT_x);
	OUTCTRL_SetAuxMisc(&pCTRL->stOUT_x.stMISC);
	
	OUTCTRL_SetEncFrmt(pCTRL->encFRMT);
	OUTCTRL_SetEncCtrl(pCTRL->ctrl);
	
	OUTCTRL_SetBWExten(pCTRL->enhFLAG);
	OUTCTRL_SetLtiCtrl(pCTRL->enhFLAG);
	OUTCTRL_SetCtiCtrl(pCTRL->enhFLAG);
	OUTCTRL_SetColorEnh(pCTRL->enhFLAG);
	
	OUTCTRL_SetPipCtrl(&pCTRL->stPIPctl);
	OUTCTRL_SetExtLock(&pCTRL->stExtLock);
	OUTCTRL_SetLVDS_tx(&pCTRL->stLVDS_tx);
	
}


//api
//----------------------------------------------------------------------------------------
void Init_OUT(void)
{
	memset(&stOUTCTRL, 0, sizeof(OUT_CTRL_PARAM));
	Init_OUT_PARAM(&stOUTCTRL);
	Init_OUT_REGISTER(&stOUTCTRL);
}

//----------------------------------------------------------------------------------------
void OUT_ProcessHandler(void)
{
	if ((stOUTCTRL.ctrl&OUTCTRL_TRIGGER_ISP)==0) return;
	stOUTCTRL.ctrl &= (~OUTCTRL_TRIGGER_ISP);
	
	OUTCTRL_PinMapHandler(&stOUTCTRL);
	OUTCTRL_MainFrmtHandler(&stOUTCTRL);
	OUTCTRL_MainDispHandler(&stOUTCTRL);
	OUTCTRL_MainCSCHandler(&stOUTCTRL);
	OUTCTRL_MainMiscHandler(&stOUTCTRL);
	
	OUTCTRL_AuxFrmtHandler(&stOUTCTRL);
	OUTCTRL_AuxDispHandler(&stOUTCTRL);
	OUTCTRL_AuxCSCHandler(&stOUTCTRL);
	OUTCTRL_AuxMiscHandler(&stOUTCTRL);
	
	OUTCTRL_EncFrmtHandler(&stOUTCTRL);
	OUTCTRL_EncMiscHandler(&stOUTCTRL);
	
	OUTCTRL_EnhanceHandler(&stOUTCTRL);
	OUTCTRL_PipHandler(&stOUTCTRL);
	OUTCTRL_ExtLockHandler(&stOUTCTRL);
}


//get/set func.
//------------------------------------------------------------------------------------
void SetOUT_MainFrmt(BYTE frmt)
{
	stOUTCTRL.stOUT_m.frmt = frmt;		OUTCTRL_SetMainFrmt(frmt);
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);		// main update
}

//------------------------------------------------------------------------------------
void SetOUT_MainMode(BYTE mode)
{
	stOUTCTRL.stOUT_m.mode = mode;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);		// main update
}

//---------------------------------------------------------------------------------------------
void SetOUT_MainDispEN(BOOL nID)
{
	stOUTCTRL.dspFLAG &= ~(MISP_MAIN_DISPLAY_ON);
	stOUTCTRL.dspFLAG |= (nID)?	 MISP_MAIN_DISPLAY_ON:  MISP_MAIN_DISPLAY_OFF;

	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAINDISP | OUTCTRL_TRIGGER_ISP);		// disp update
}

//---------------------------------------------------------------------------------------------
void SetOUT_MainDispOnOffAuto(BYTE delay)
{
	stOUTCTRL.dspFLAG &= ~(MISP_MAIN_DISPLAY_ON);
	OUTCTRL_SetMainDisp(stOUTCTRL.dspFLAG);									// main display off
	
	stOUTCTRL.stOUT_m.turnon_cnt = 0;										// clear counter
	stOUTCTRL.stOUT_m.turnon_dly = delay;									// main display turn-on delay : 16ms x delay = xx ms
	stOUTCTRL.dspFLAG |= MISP_MAIN_DISPLAY_ON;								// clear display off flag

	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAINDISP | OUTCTRL_TRIGGER_ISP);		// disp update
}

//---------------------------------------------------------------------------------------------
void SetOUT_MainDarkScreenAuto(BYTE delay)
{
	stOUTCTRL.dspFLAG |= MISP_MAIN_DARKSCR_ON;
	OUTCTRL_SetMainDisp(stOUTCTRL.dspFLAG);									// main darkscreen on
	
	stOUTCTRL.stOUT_m.turnon_cnt = 0;										// clear counter
	stOUTCTRL.stOUT_m.turnon_dly = delay;									// main display turn-on delay : 16ms x delay = xx ms
	stOUTCTRL.dspFLAG &= ~(MISP_MAIN_DARKSCR_ON);							// clear darkscreen on flag

	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAINDISP | OUTCTRL_TRIGGER_ISP);		// disp update
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxDispOnOffAuto(BYTE delay)
{
	stOUTCTRL.dspFLAG &= ~(MISP_AUX_DISPLAY_ON);
	OUTCTRL_SetAuxDisp(stOUTCTRL.dspFLAG);									// aux display off
	
	stOUTCTRL.stOUT_x.turnon_cnt = 0;										// clear counter
	stOUTCTRL.stOUT_x.turnon_dly = delay;									// aux display turn-on delay : 16ms x delay = xx ms
	stOUTCTRL.dspFLAG |= MISP_AUX_DISPLAY_ON;								// clear display off flag

	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUXDISP | OUTCTRL_TRIGGER_ISP);		// disp update
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxDarkScreenAuto(BYTE delay)
{
	stOUTCTRL.dspFLAG |= MISP_AUX_DARKSCR_ON;
	OUTCTRL_SetAuxDisp(stOUTCTRL.dspFLAG);									// aux darkscreen on
	
	stOUTCTRL.stOUT_x.turnon_cnt = 0;										// clear counter
	stOUTCTRL.stOUT_x.turnon_dly = delay;									// aux display turn-on delay : 16ms x delay = xx ms
	stOUTCTRL.dspFLAG &= ~(MISP_AUX_DARKSCR_ON);							// clear darkscreen on flag

	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUXDISP | OUTCTRL_TRIGGER_ISP);		// disp update
}

//---------------------------------------------------------------------------------------------
void SetOUT_MainFreezeEN(BOOL nID)
{
	stOUTCTRL.dspFLAG &= ~(MISP_MAIN_FREEZE_ON);
	stOUTCTRL.dspFLAG |= (nID)?	 MISP_MAIN_FREEZE_ON:  MISP_MAIN_FREEZE_OFF;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAINDISP | OUTCTRL_TRIGGER_ISP);		// disp update
}

//------------------------------------------------------------------------------------
void SetOUT_AuxFrmt(BYTE frmt)
{
	stOUTCTRL.stOUT_x.frmt = frmt;		OUTCTRL_SetAuxFrmt(frmt);
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);		// aux update
}

//------------------------------------------------------------------------------------
void SetOUT_AuxMode(BYTE mode)
{
	stOUTCTRL.stOUT_x.mode = mode;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);		// aux update
}

//------------------------------------------------------------------------------------
void SetOUT_EncFrmt(BYTE frmt)
{
	stOUTCTRL.encFRMT = frmt;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_ENCFRMT | OUTCTRL_TRIGGER_ISP);		// enc update
}

//------------------------------------------------------------------------------------
BYTE GetOUT_EncFrmt(void)
{
	return 	stOUTCTRL.encFRMT;
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxDispEN(BOOL nID)
{
	stOUTCTRL.dspFLAG &= ~(MISP_AUX_DISPLAY_ON);
	stOUTCTRL.dspFLAG |= (nID)?	 MISP_AUX_DISPLAY_ON:  MISP_AUX_DISPLAY_OFF;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUXDISP | OUTCTRL_TRIGGER_ISP);		// disp update
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxFreezeEN(BOOL nID)
{
	stOUTCTRL.dspFLAG &= ~(MISP_AUX_FREEZE_ON);
	stOUTCTRL.dspFLAG |= (nID)?	 MISP_AUX_FREEZE_ON:  MISP_AUX_FREEZE_OFF;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUXDISP | OUTCTRL_TRIGGER_ISP);		// disp update
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxPipEN(BOOL nID)
{
	stOUTCTRL.dspFLAG &= ~(MISP_AUX_SYNC_PIP_EN);
	stOUTCTRL.dspFLAG |= (nID)?	 MISP_AUX_SYNC_PIP_EN:	MISP_AUX_SYNC_PIP_DIS;
	
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUXDISP | OUTCTRL_TRIGGER_ISP);		// disp update
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxUsedByFunc(BOOL nID)
{
	if(nID)		stOUTCTRL.ctrl |= OUTCTRL_AUX_USEDBY_FUNC;
	else 		stOUTCTRL.ctrl &= ~(OUTCTRL_AUX_USEDBY_FUNC);
	
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUXDISP | OUTCTRL_TRIGGER_ISP);		// disp update
}

//---------------------------------------------------------------------------------------------
BOOL GetOUT_AuxUsedByFunc(void)
{
	return 	(stOUTCTRL.ctrl & OUTCTRL_AUX_USEDBY_FUNC)?  1:  0;
}

//------------------------------------------------------------------------------------
void SetOUT_MaintotHS(WORD val)
{
	stOUTCTRL.stOUT_m.stSYNC.totHS = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);			// main update
}

//------------------------------------------------------------------------------------
void SetOUT_MainSizeHA(WORD val)
{
	stOUTCTRL.stOUT_m.stATTB.Hact = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);			// main update
}

//------------------------------------------------------------------------------------
void SetOUT_MainSizeVA(WORD val)
{
	stOUTCTRL.stOUT_m.stATTB.Vact = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);			// main update
}

//------------------------------------------------------------------------------------
void SetOUT_MainbgnHA(WORD val)
{
	stOUTCTRL.stOUT_m.stSYNC.bgnHA = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);			// main update
}

//------------------------------------------------------------------------------------
void SetOUT_MainendHA(WORD val)
{
	stOUTCTRL.stOUT_m.stSYNC.endHA = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);			// main update
}

//------------------------------------------------------------------------------------
void SetOUT_MainbgnVA(WORD val)
{
	stOUTCTRL.stOUT_m.stSYNC.bgnVA = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);			// main update
}

//------------------------------------------------------------------------------------
void SetOUT_MainendVA(WORD val)
{
	stOUTCTRL.stOUT_m.stSYNC.endVA = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);			// main update
}

//------------------------------------------------------------------------------------
void SetOUT_MainbgnVAB(WORD val)
{
	stOUTCTRL.stOUT_m.stSYNC.bgnVAB = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);			// main update
}

//------------------------------------------------------------------------------------
void SetOUT_MainendVAB(WORD val)
{
	stOUTCTRL.stOUT_m.stSYNC.endVAB = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);			// main update
}

//------------------------------------------------------------------------------------
void SetOUT_MainContrast(BYTE val)
{
	stOUTCTRL.stOUT_m.stTUNE.contrast = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAINCSC | OUTCTRL_TRIGGER_ISP);			// main csc update
}

//------------------------------------------------------------------------------------
void SetOUT_MainBrightness(BYTE val)
{
	stOUTCTRL.stOUT_m.stTUNE.brightness = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAINCSC | OUTCTRL_TRIGGER_ISP);			// main csc update
}

//------------------------------------------------------------------------------------
void SetOUT_MainSaturation(BYTE val)
{
	stOUTCTRL.stOUT_m.stTUNE.saturation = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAINCSC | OUTCTRL_TRIGGER_ISP);			// main csc update
}

//------------------------------------------------------------------------------------
void SetOUT_MainHue(BYTE val)
{
	stOUTCTRL.stOUT_m.stTUNE.hue = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAINCSC | OUTCTRL_TRIGGER_ISP);			// main csc update
}

//------------------------------------------------------------------------------------
void SetOUT_AuxContrast(BYTE val)
{
	stOUTCTRL.stOUT_x.stTUNE.contrast = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUXCSC | OUTCTRL_TRIGGER_ISP);			// aux csc update
}

//------------------------------------------------------------------------------------
void SetOUT_AuxBrightness(BYTE val)
{
	stOUTCTRL.stOUT_x.stTUNE.brightness = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUXCSC | OUTCTRL_TRIGGER_ISP);			// aux csc update
}

//------------------------------------------------------------------------------------
void SetOUT_AuxSaturation(BYTE val)
{
	stOUTCTRL.stOUT_x.stTUNE.saturation = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUXCSC | OUTCTRL_TRIGGER_ISP);			// aux csc update
}

//------------------------------------------------------------------------------------
void SetOUT_AuxHue(BYTE val)
{
	stOUTCTRL.stOUT_x.stTUNE.hue = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUXCSC | OUTCTRL_TRIGGER_ISP);			// aux csc update
}

//---------------------------------------------------------------------------------------------
void SetOUT_MainCSC(BYTE nID)
{
#if CSC_RANGE_CTRL
	if((stOUTCTRL.stOUT_m.mode == MISP_OUT_RGB444_8) || (stOUTCTRL.stOUT_m.mode == MISP_OUT_RGB444_10))	{
		switch(nID)	{
			case  0:  stOUTCTRL.stOUT_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscYUVtoRGB_HD_StdRange;       break;  // HD 0~255
			case  1:  stOUTCTRL.stOUT_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscYUVtoRGB_HD_FullRange;        break;  // HD 16~235
			default:  return;
		}
	}
	else	{
		switch(nID)	{
			case  0:  stOUTCTRL.stOUT_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscBypass_FullRange;       break;  // HD 0~255
			case  1:  stOUTCTRL.stOUT_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscBypass_StdRange;        break;  // HD 16~235
			case  2:  stOUTCTRL.stOUT_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscBypass_SonyRange;       break;  // HD 16~255
			case  3:  stOUTCTRL.stOUT_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscBypass_FullRange;        break;  
			default:  return;
		}
	}
#else
	switch(nID)	{
		case  0:  stOUTCTRL.stOUT_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscYUVtoRGB_SD_FullRange;  break; 	// BT.601
		case  1:  stOUTCTRL.stOUT_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscYUVtoRGB_HD_FullRange;  break; 	// BT.709
		case  2:  stOUTCTRL.stOUT_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscYUVtoRGB_HD_FullRange;  break; 	// BT.2020 (n/a)
		case  3:  stOUTCTRL.stOUT_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscBypass_FullRange;       break; 	// Bypass
		default:  return;
	}
#endif
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAINCSC | OUTCTRL_TRIGGER_ISP);			// main csc update
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_MainCSC(void)
{
#if CSC_RANGE_CTRL
	if     (stOUTCTRL.stOUT_m.pCSC == (PMISP_CSCCTRL_INFO)&MISP_CscBypass_FullRange)       return 0;  // HD 0~255
	else if(stOUTCTRL.stOUT_m.pCSC == (PMISP_CSCCTRL_INFO)&MISP_CscBypass_StdRange)        return 1;  // HD 16~235
	else if(stOUTCTRL.stOUT_m.pCSC == (PMISP_CSCCTRL_INFO)&MISP_CscBypass_SonyRange)       return 2;  // HD 16~255
	else if(stOUTCTRL.stOUT_m.pCSC == (PMISP_CSCCTRL_INFO)&MISP_CscBypass_FullRange)        return 3;  // HD 0~235
	else                                                                                   return 4;  // error
#else
	if     (stOUTCTRL.stOUT_m.pCSC == (PMISP_CSCCTRL_INFO)&MISP_CscYUVtoRGB_SD_FullRange)  return 0;  // BT.601
	else if(stOUTCTRL.stOUT_m.pCSC == (PMISP_CSCCTRL_INFO)&MISP_CscYUVtoRGB_HD_FullRange)  return 1;  // BT.709
	else if(stOUTCTRL.stOUT_m.pCSC == (PMISP_CSCCTRL_INFO)&MISP_CscYUVtoRGB_HD_FullRange)  return 2;  // BT.2020 (n/a)
	else if(stOUTCTRL.stOUT_m.pCSC == (PMISP_CSCCTRL_INFO)&MISP_CscBypass_FullRange)       return 3;  // Bypass
	else                                                                                   return 4;  // error
#endif
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxCSC(BYTE nID)
{
#if CSC_RANGE_CTRL
	switch(nID)	{
		case  0:  stOUTCTRL.stOUT_x.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscBypass_FullRange;       break;  // HD 0~255
		case  1:  stOUTCTRL.stOUT_x.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscBypass_StdRange;        break;  // HD 16~235
		case  2:  stOUTCTRL.stOUT_x.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscBypass_SonyRange;       break;  // HD 16~255
		case  3:  stOUTCTRL.stOUT_x.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscBypass_FullRange;        break;  // HD 0~235
		default:  return;
	}
#else
	switch(nID)	{
		case  0:  stOUTCTRL.stOUT_x.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscYUVtoRGB_SD_FullRange;  break; 	// BT.601
		case  1:  stOUTCTRL.stOUT_x.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscYUVtoRGB_HD_FullRange;  break; 	// BT.709
		case  2:  stOUTCTRL.stOUT_x.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscYUVtoRGB_HD_FullRange;  break; 	// BT.2020 (n/a)
		case  3:  stOUTCTRL.stOUT_x.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscBypass_FullRange;       break; 	// Bypass
		default:  return;
	}
#endif
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUXCSC | OUTCTRL_TRIGGER_ISP);			// aux csc update
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_AuxCSC(void)
{
#if CSC_RANGE_CTRL
	if     (stOUTCTRL.stOUT_x.pCSC == (PMISP_CSCCTRL_INFO)&MISP_CscBypass_FullRange)       return 0;  // HD 0~255
	else if(stOUTCTRL.stOUT_x.pCSC == (PMISP_CSCCTRL_INFO)&MISP_CscBypass_StdRange)        return 1;  // HD 16~235
	else if(stOUTCTRL.stOUT_x.pCSC == (PMISP_CSCCTRL_INFO)&MISP_CscBypass_SonyRange)       return 2;  // HD 16~255
	else if(stOUTCTRL.stOUT_x.pCSC == (PMISP_CSCCTRL_INFO)&MISP_CscBypass_FullRange)        return 3;  // HD 0~235
	else                                                                                   return 4;  // error
#else
	if     (stOUTCTRL.stOUT_x.pCSC == (PMISP_CSCCTRL_INFO)&MISP_CscYUVtoRGB_SD_FullRange)  return 0;  // BT.601
	else if(stOUTCTRL.stOUT_x.pCSC == (PMISP_CSCCTRL_INFO)&MISP_CscYUVtoRGB_HD_FullRange)  return 1;  // BT.709
	else if(stOUTCTRL.stOUT_x.pCSC == (PMISP_CSCCTRL_INFO)&MISP_CscYUVtoRGB_HD_FullRange)  return 2;  // BT.2020 (n/a)
	else if(stOUTCTRL.stOUT_x.pCSC == (PMISP_CSCCTRL_INFO)&MISP_CscBypass_FullRange)       return 3;  // Bypass
	else                                                                                   return 4;  // error
#endif
}

//------------------------------------------------------------------------------------
void SetOUT_AuxtotHS(WORD val)
{
	stOUTCTRL.stOUT_x.stSYNC.totHS = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);			// aux update
}

//------------------------------------------------------------------------------------
void SetOUT_AuxSizeHA(WORD val)
{
	stOUTCTRL.stOUT_x.stATTB.Hact = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);			// aux update
}

//------------------------------------------------------------------------------------
void SetOUT_AuxSizeVA(WORD val)
{
	stOUTCTRL.stOUT_x.stATTB.Vact = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);			// aux update
}

//------------------------------------------------------------------------------------
void SetOUT_AuxbgnHA(WORD val)
{
	stOUTCTRL.stOUT_x.stSYNC.bgnHA = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);			// aux update
}

//------------------------------------------------------------------------------------
void SetOUT_AuxendHA(WORD val)
{
	stOUTCTRL.stOUT_x.stSYNC.endHA = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);			// aux update
}

//------------------------------------------------------------------------------------
void SetOUT_AuxbgnVA(WORD val)
{
	stOUTCTRL.stOUT_x.stSYNC.bgnVA = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);			// aux update
}

//------------------------------------------------------------------------------------
void SetOUT_AuxendVA(WORD val)
{
	stOUTCTRL.stOUT_x.stSYNC.endVA = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);			// aux update
}

//------------------------------------------------------------------------------------
void SetOUT_AuxbgnVAB(WORD val)
{
	stOUTCTRL.stOUT_x.stSYNC.bgnVAB = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);			// aux update
}

//------------------------------------------------------------------------------------
void SetOUT_AuxendVAB(WORD val)
{
	stOUTCTRL.stOUT_x.stSYNC.endVAB = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);			// aux update
}

//------------------------------------------------------------------------------------
void SetOUT_CVBSNTSC720H(void)
{
	SetOUT_AuxFrmt(MISP_VIDOUT_720x480i60);	// 720H
	SetFC_AuxFCdstPosiX(0); 	SetFC_AuxFCdstSizeH(GetOUT_AuxSizeHA());
	SetFC_AuxFCdstPosiY(0); 	SetFC_AuxFCdstSizeV(GetOUT_AuxSizeVA());
	
	SetOUT_EncFrmt(MISP_VID_VENC_NTSC_M);
	SetOUT_AuxDispEN(ON);
}

//------------------------------------------------------------------------------------
void SetOUT_CVBSNTSC960H(void)
{
	SetOUT_AuxFrmt(MISP_VIDOUT_960x480i60);	// 960H
	SetFC_AuxFCdstPosiX(0); 	SetFC_AuxFCdstSizeH(GetOUT_AuxSizeHA());
	SetFC_AuxFCdstPosiY(0); 	SetFC_AuxFCdstSizeV(GetOUT_AuxSizeVA());
	
	SetOUT_EncFrmt(MISP_VID_VENC_NTSC_M);
	SetOUT_AuxDispEN(ON);
}

//------------------------------------------------------------------------------------
void SetOUT_CVBSPAL(void)
{
	SetOUT_AuxFrmt(MISP_VIDOUT_720x576i50);
	SetFC_AuxFCdstPosiX(0); 	SetFC_AuxFCdstSizeH(GetOUT_AuxSizeHA());
	SetFC_AuxFCdstPosiY(0); 	SetFC_AuxFCdstSizeV(GetOUT_AuxSizeVA());
	
	SetOUT_EncFrmt(MISP_VID_VENC_PAL_B);	// PAL-B
	SetOUT_AuxDispEN(ON);
}

//------------------------------------------------------------------------------------
void SetOUT_CVBSOutEN(void)
{
	SetIN_AuxSrcMainOut(); 		// set aux source to main output
	
	SetOUT_AuxMode(MISP_OUT_MUX656_8);
	SetOUT_AuxFrmt(MISP_VIDOUT_720x480i60);	// 720H
	SetFC_AuxFCdstPosiX(0); 	SetFC_AuxFCdstSizeH(GetOUT_AuxSizeHA());
	SetFC_AuxFCdstPosiY(0); 	SetFC_AuxFCdstSizeV(GetOUT_AuxSizeVA());
	
	SetOUT_AuxDispEN(ON);
	SetOUT_EncFrmt(MISP_VID_VENC_NTSC_M);
}

//------------------------------------------------------------------------------------
void SetOUT_CVBSColor(BOOL OnOff)
{
	stOUTCTRL.ctrl &= (~OUTCTRL_VENC_COLOR_DIS);
	stOUTCTRL.ctrl |= (OnOff)? OUTCTRL_VENC_COLOR_EN: OUTCTRL_VENC_COLOR_DIS;
	
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_ENCMISC | OUTCTRL_TRIGGER_ISP);			// enc update
}

//------------------------------------------------------------------------------------
void SetOUT_CVBSBurst(BOOL OnOff)
{
	stOUTCTRL.ctrl &= (~OUTCTRL_VENC_BURST_DIS);
	stOUTCTRL.ctrl |= (OnOff)? OUTCTRL_VENC_BURST_EN: OUTCTRL_VENC_BURST_DIS;
	
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_ENCMISC | OUTCTRL_TRIGGER_ISP);			// enc update
}

//------------------------------------------------------------------------------------
BOOL GetOUT_CVBSBurst(void)
{
	return ((stOUTCTRL.ctrl & OUTCTRL_VENC_BURST_DIS)? 0: 1);		// 0:burst-off, 1:bursr on
}

//------------------------------------------------------------------------------------
void SetOUT_CVBSBlueScreen(BOOL OnOff)
{
	stOUTCTRL.ctrl &= (~OUTCTRL_VENC_BLUESCR_EN);
	stOUTCTRL.ctrl |= (OnOff)? OUTCTRL_VENC_BLUESCR_EN: OUTCTRL_VENC_BLUESCR_DIS;
	
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_ENCMISC | OUTCTRL_TRIGGER_ISP);			// enc update
}

//------------------------------------------------------------------------------------
void SetOUT_BWExtension(BOOL OnOff)
{
	stOUTCTRL.enhFLAG &= (~MISP_ENH_BWEXT_ON);
	stOUTCTRL.enhFLAG |= (OnOff)?  MISP_ENH_BWEXT_ON:  MISP_ENH_BWEXT_OFF;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_ENHANCE | OUTCTRL_TRIGGER_ISP);	// enhance update
}

//------------------------------------------------------------------------------------
void SetOUT_EnableLTI(BOOL OnOff)
{
	stOUTCTRL.enhFLAG &= (~MISP_ENH_LTI_ON);
	stOUTCTRL.enhFLAG |= (OnOff)?  MISP_ENH_LTI_ON:  MISP_ENH_LTI_OFF;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_ENHANCE | OUTCTRL_TRIGGER_ISP);	// enhance update
}

//------------------------------------------------------------------------------------
void SetOUT_EnableCTI(BOOL OnOff)
{
	stOUTCTRL.enhFLAG &= (~MISP_ENH_CTI_ON);
	stOUTCTRL.enhFLAG |= (OnOff)?  MISP_ENH_CTI_ON:  MISP_ENH_CTI_OFF;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_ENHANCE | OUTCTRL_TRIGGER_ISP);	// enhance update
}

//------------------------------------------------------------------------------------
void SetOUT_ColorEnhEn(BOOL OnOff)
{
	stOUTCTRL.enhFLAG &= (~MISP_ENH_COLOR_ON);
	stOUTCTRL.enhFLAG |= (OnOff)?  MISP_ENH_COLOR_ON:  MISP_ENH_COLOR_OFF;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_ENHANCE | OUTCTRL_TRIGGER_ISP);	// enhance update
}

//------------------------------------------------------------------------------------
BYTE GetOUT_MainFrmt(void)
{
	return stOUTCTRL.stOUT_m.frmt;
}

//------------------------------------------------------------------------------------
BYTE GetOUT_MainMode(void)
{
	return stOUTCTRL.stOUT_m.mode;
}

//------------------------------------------------------------------------------------
WORD GetOUT_MainbgnHA(void)
{
	return stOUTCTRL.stOUT_m.stSYNC.bgnHA;
}

//------------------------------------------------------------------------------------
WORD GetOUT_MainendHA(void)
{
	return stOUTCTRL.stOUT_m.stSYNC.endHA;
}

//------------------------------------------------------------------------------------
WORD GetOUT_MainbgnVA(void)
{
	return stOUTCTRL.stOUT_m.stSYNC.bgnVA;
}

//------------------------------------------------------------------------------------
WORD GetOUT_MainendVA(void)
{
	return stOUTCTRL.stOUT_m.stSYNC.endVA;
}

//------------------------------------------------------------------------------------
BYTE GetOUT_AuxFrmt(void)
{
	return stOUTCTRL.stOUT_x.frmt;
}

//------------------------------------------------------------------------------------
BYTE GetOUT_AuxMode(void)
{
	return stOUTCTRL.stOUT_x.mode;
}

//------------------------------------------------------------------------------------
WORD GetOUT_AuxbgnHA(void)
{
	return stOUTCTRL.stOUT_x.stSYNC.bgnHA;
}

//------------------------------------------------------------------------------------
WORD GetOUT_AuxendHA(void)
{
	return stOUTCTRL.stOUT_x.stSYNC.endHA;
}

//------------------------------------------------------------------------------------
WORD GetOUT_AuxbgnVA(void)
{
	return stOUTCTRL.stOUT_x.stSYNC.bgnVA;
}

//------------------------------------------------------------------------------------
WORD GetOUT_AuxendVA(void)
{
	return stOUTCTRL.stOUT_x.stSYNC.endVA;
}

//------------------------------------------------------------------------------------
WORD GetOUT_MainSizeHA(void)
{
	return stOUTCTRL.stOUT_m.stATTB.Hact;
}

//------------------------------------------------------------------------------------
WORD GetOUT_MainSizeVA(void)
{
	return stOUTCTRL.stOUT_m.stATTB.Vact;
}

//------------------------------------------------------------------------------------
WORD GetOUT_AuxSizeHA(void)
{
	return stOUTCTRL.stOUT_x.stATTB.Hact;
}

//------------------------------------------------------------------------------------
WORD GetOUT_AuxSizeVA(void)
{
	return stOUTCTRL.stOUT_x.stATTB.Vact;
}

//------------------------------------------------------------------------------------
PMISP_OUTVIDEO_SYNC	GetOUT_MainSync(void)
{
	return &stOUTCTRL.stOUT_m.stSYNC;
}

//------------------------------------------------------------------------------------
PMISP_OUTVIDEO_SYNC	GetOUT_AuxSync(void)
{
	return &stOUTCTRL.stOUT_x.stSYNC;
}

//------------------------------------------------------------------------------------
PMISP_OUTVIDEO_ATTB	GetOUT_MainAttb(void)
{
	return &stOUTCTRL.stOUT_m.stATTB;
}

//------------------------------------------------------------------------------------
PMISP_OUTVIDEO_ATTB	GetOUT_AuxAttb(void)
{
	return &stOUTCTRL.stOUT_x.stATTB;
}

//-------------------------------------------------------------------------------------
BOOL GetOUT_MainScanTypeProg(void)
{
	return ((stOUTCTRL.stOUT_m.stATTB.attb & MISP_SCANTYPE_PROG)? 1: 0);	//0:interlace, 1:progressive
}

//-------------------------------------------------------------------------------------
BOOL GetOUT_AuxScanTypeProg(void)
{
	return ((stOUTCTRL.stOUT_x.stATTB.attb & MISP_SCANTYPE_PROG)? 1: 0);	//0:interlace, 1:progressive
}

//---------------------------------------------------------------------------------------------
BOOL GetOUT_AuxPipEN(void)
{
	return ((stOUTCTRL.dspFLAG & MISP_AUX_SYNC_PIP_EN)? 1: 0);		// 0:pip-sync off, 1:pip-sync on
}

//---------------------------------------------------------------------------------------------
void ISPM VideoTurnOnCounter(void)
{
	if(stOUTCTRL.stOUT_m.turnon_cnt<255)  stOUTCTRL.stOUT_m.turnon_cnt++;
	if(stOUTCTRL.stOUT_x.turnon_cnt<255)  stOUTCTRL.stOUT_x.turnon_cnt++;
}

//add by jins 2016.03
//-------------------------------------------------------------------------------------
BOOL GetOUT_MainFreezeEN(void)
{
	if(stOUTCTRL.dspFLAG&MISP_MAIN_FREEZE_ON)  return ON;
	else                                       return OFF;
}

//---------------------------------------------------------------------------------------------
BOOL GetOUT_AuxFreezeEN(void)
{
	if(stOUTCTRL.dspFLAG&MISP_AUX_FREEZE_ON)  return ON;
	else                                      return OFF;
}

//------------------------------------------------------------------------------------
BOOL GetOUT_BWExtension(void)
{
	if(stOUTCTRL.enhFLAG&MISP_ENH_BWEXT_ON)  return ON;
	else                                     return OFF;
}

//------------------------------------------------------------------------------------
BOOL GetOUT_EnableLTI(void)
{
	if(stOUTCTRL.enhFLAG&MISP_ENH_LTI_ON)  return ON;
	else                                   return OFF;
}

//------------------------------------------------------------------------------------
BOOL GetOUT_EnableCTI(void)
{
	if(stOUTCTRL.enhFLAG&MISP_ENH_CTI_ON)  return ON;
	else                                   return OFF;
}

//------------------------------------------------------------------------------------
BOOL GetOUT_ColorEnhEn(void)
{
	if(stOUTCTRL.enhFLAG&MISP_ENH_COLOR_ON)  return ON;
	else                                     return OFF;
}

//------------------------------------------------------------------------------------
BYTE GetOUT_MainContrast(void)
{
	return stOUTCTRL.stOUT_m.stTUNE.contrast;
}

//------------------------------------------------------------------------------------
BYTE GetOUT_MainBrightness(void)
{
	return stOUTCTRL.stOUT_m.stTUNE.brightness;
}

//------------------------------------------------------------------------------------
BYTE GetOUT_MainSaturation(void)
{
	return stOUTCTRL.stOUT_m.stTUNE.saturation;
}

//------------------------------------------------------------------------------------
BYTE GetOUT_MainHue(void)
{
	return stOUTCTRL.stOUT_m.stTUNE.hue;
}

//------------------------------------------------------------------------------------
BYTE GetOUT_AuxContrast(void)
{
	return stOUTCTRL.stOUT_x.stTUNE.contrast;
}

//------------------------------------------------------------------------------------
BYTE GetOUT_AuxBrightness(void)
{
	return stOUTCTRL.stOUT_x.stTUNE.brightness;
}

//------------------------------------------------------------------------------------
BYTE GetOUT_AuxSaturation(void)
{
	return stOUTCTRL.stOUT_x.stTUNE.saturation;
}

//------------------------------------------------------------------------------------
BYTE GetOUT_AuxHue(void)
{
	return stOUTCTRL.stOUT_x.stTUNE.hue;
}

//---------------------------------------------------------------------------------------------
BOOL GetOUT_MainDispEN(void)
{
	if(stOUTCTRL.dspFLAG&MISP_MAIN_DISPLAY_ON)  return ON;
	else                                        return OFF;
}

//---------------------------------------------------------------------------------------------
BOOL GetOUT_AuxDispEN(void)
{
	if(stOUTCTRL.dspFLAG&MISP_AUX_DISPLAY_ON)  return ON;
	else                                       return OFF;
}

//---------------------------------------------------------------------------------------------
void SetOUT_MainBackgroundY(BYTE val)
{
	stOUTCTRL.stOUT_m.stMISC.back_y = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAINMISC | OUTCTRL_TRIGGER_ISP);  // Main misc update
}

//---------------------------------------------------------------------------------------------
void SetOUT_MainBackgroundCb(BYTE val)
{
	stOUTCTRL.stOUT_m.stMISC.back_cb = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAINMISC | OUTCTRL_TRIGGER_ISP);  // Main misc update
}

//---------------------------------------------------------------------------------------------
void SetOUT_MainBackgroundCr(BYTE val)
{
	stOUTCTRL.stOUT_m.stMISC.back_cr = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAINMISC | OUTCTRL_TRIGGER_ISP);  // Main misc update
}

//---------------------------------------------------------------------------------------------
void SetOUT_MainDithering(BOOL OnOff)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveOutDither(OnOff);
#endif
	
	stOUTCTRL.stOUT_m.stMISC.dither = OnOff;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAINMISC | OUTCTRL_TRIGGER_ISP);  // Main misc update
}

//---------------------------------------------------------------------------------------------
void SetOUT_MainDarkScreen(BOOL OnOff)
{
	stOUTCTRL.dspFLAG &= ~(MISP_MAIN_DARKSCR_ON);
	stOUTCTRL.dspFLAG |= (OnOff)?	 MISP_MAIN_DARKSCR_ON:  MISP_MAIN_DARKSCR_OFF;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAINDISP | OUTCTRL_TRIGGER_ISP);  // disp update
}

//---------------------------------------------------------------------------------------------
void SetOUT_MainTestPattern(BYTE val)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveOutPattern(val);
#endif
	
	stOUTCTRL.dspFLAG &= ~(MISP_MAIN_OUT_TP_MASK);
	stOUTCTRL.dspFLAG |= ((WORD)val<<8);
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAINDISP | OUTCTRL_TRIGGER_ISP);  // disp update
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxBackgroundY(BYTE val)
{
	stOUTCTRL.stOUT_x.stMISC.back_y = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUXMISC | OUTCTRL_TRIGGER_ISP);  // Aux misc update
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxBackgroundCb(BYTE val)
{
	stOUTCTRL.stOUT_x.stMISC.back_cb = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUXMISC | OUTCTRL_TRIGGER_ISP);  // Aux misc update
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxBackgroundCr(BYTE val)
{
	stOUTCTRL.stOUT_x.stMISC.back_cr = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUXMISC | OUTCTRL_TRIGGER_ISP);  // Aux misc update
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxDithering(BOOL OnOff)
{
	stOUTCTRL.stOUT_x.stMISC.dither = OnOff;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUXMISC | OUTCTRL_TRIGGER_ISP);  // Aux misc update
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxDarkScreen(BOOL OnOff)
{
	stOUTCTRL.dspFLAG &= ~(MISP_AUX_DARKSCR_ON);
	stOUTCTRL.dspFLAG |= (OnOff)?	 MISP_AUX_DARKSCR_ON:  MISP_AUX_DARKSCR_OFF;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUXDISP | OUTCTRL_TRIGGER_ISP);  // disp update
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxTestPattern(BYTE val)
{
	stOUTCTRL.dspFLAG &= ~(MISP_AUX_OUT_TP_MASK);
	stOUTCTRL.dspFLAG |= ((WORD)val<<12);
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUXDISP | OUTCTRL_TRIGGER_ISP);  // disp update
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_MainBackgroundY(void)
{
	return stOUTCTRL.stOUT_m.stMISC.back_y;
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_MainBackgroundCb(void)
{
	return stOUTCTRL.stOUT_m.stMISC.back_cb;
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_MainBackgroundCr(void)
{
	return stOUTCTRL.stOUT_m.stMISC.back_cr;
}

//---------------------------------------------------------------------------------------------
BOOL GetOUT_MainDarkScreen(void)
{
	return (stOUTCTRL.dspFLAG&MISP_MAIN_DARKSCR_ON)?ON:OFF;
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_MainTestPattern(void)
{
	return (BYTE)((stOUTCTRL.dspFLAG&MISP_MAIN_OUT_TP_MASK)>>8);
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_AuxBackgroundY(void)
{
	return stOUTCTRL.stOUT_x.stMISC.back_y;
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_AuxBackgroundCb(void)
{
	return stOUTCTRL.stOUT_x.stMISC.back_cb;
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_AuxBackgroundCr(void)
{
	return stOUTCTRL.stOUT_x.stMISC.back_cr;
}

//---------------------------------------------------------------------------------------------
BOOL GetOUT_AuxDarkScreen(void)
{
	return (stOUTCTRL.dspFLAG&MISP_AUX_DARKSCR_ON)?ON:OFF;
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_AuxTestPattern(void)
{
	return (BYTE)((stOUTCTRL.dspFLAG&MISP_AUX_OUT_TP_MASK)>>8);
}

//---------------------------------------------------------------------------------------------
void SetOUT_MainSwapCbCr(BOOL OnOff)
{
	stOUTCTRL.stOUT_m.stFINE.cbcr_swap = OnOff;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);  // Main YC update
}

//---------------------------------------------------------------------------------------------
void SetOUT_MainHsPolar(BOOL OnOff)
{
	if(OnOff)  stOUTCTRL.stOUT_m.stATTB.attb |= MISP_POSITIVE_HSYNC;
	else       stOUTCTRL.stOUT_m.stATTB.attb &=~MISP_POSITIVE_HSYNC;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);  // Main YC update
}

//---------------------------------------------------------------------------------------------
void SetOUT_MainVsPolar(BOOL OnOff)
{
	if(OnOff)  stOUTCTRL.stOUT_m.stATTB.attb |= MISP_POSITIVE_VSYNC;
	else       stOUTCTRL.stOUT_m.stATTB.attb &=~MISP_POSITIVE_VSYNC;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);  // Main YC update
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxSwapCbCr(BOOL OnOff)
{
	stOUTCTRL.stOUT_x.stFINE.cbcr_swap = OnOff;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);  // Aux YC update
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxHsPolar(BOOL OnOff)
{
	if(OnOff)  stOUTCTRL.stOUT_x.stATTB.attb |= MISP_POSITIVE_HSYNC;
	else       stOUTCTRL.stOUT_x.stATTB.attb &=~MISP_POSITIVE_HSYNC;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);  // Aux YC update
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxVsPolar(BOOL OnOff)
{
	if(OnOff)  stOUTCTRL.stOUT_x.stATTB.attb |= MISP_POSITIVE_VSYNC;
	else       stOUTCTRL.stOUT_x.stATTB.attb &=~MISP_POSITIVE_VSYNC;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);  // Aux YC update
}

//---------------------------------------------------------------------------------------------
BOOL GetOUT_MainSwapCbCr(void)
{
	return stOUTCTRL.stOUT_m.stFINE.cbcr_swap;
}

//---------------------------------------------------------------------------------------------
BOOL GetOUT_MainHsPolar(void)
{
	return (stOUTCTRL.stOUT_m.stATTB.attb & MISP_POSITIVE_HSYNC)?1:0;
}

//---------------------------------------------------------------------------------------------
BOOL GetOUT_MainVsPolar(void)
{
	return (stOUTCTRL.stOUT_m.stATTB.attb & MISP_POSITIVE_VSYNC)?1:0;
}

//---------------------------------------------------------------------------------------------
BOOL GetOUT_AuxSwapCbCr(void)
{
	return stOUTCTRL.stOUT_x.stFINE.cbcr_swap;
}

//---------------------------------------------------------------------------------------------
BOOL GetOUT_AuxHsPolar(void)
{
	return (stOUTCTRL.stOUT_x.stATTB.attb & MISP_POSITIVE_HSYNC)?1:0;
}

//---------------------------------------------------------------------------------------------
BOOL GetOUT_AuxVsPolar(void)
{
	return (stOUTCTRL.stOUT_x.stATTB.attb & MISP_POSITIVE_VSYNC)?1:0;
}

//---------------------------------------------------------------------------------------------
void SetOUT_MainPinDe(BYTE val)
{
	// 0 : 2-D active signal 
	// 1 : composite sync signal
	// 2 : fieldid signal is output
	// 3 : hactive signal is output
	stOUTCTRL.stOUT_m.stFINE.pin_de = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);  // Main PIN update
}

//---------------------------------------------------------------------------------------------
void SetOUT_MainPinHsync(BYTE val)
{
	// 0 : hsync signal is output
	// 1 : composite sync signal
	// 2 : 2-D active signal
	// 3 : hactive signal is output
	stOUTCTRL.stOUT_m.stFINE.pin_hsync = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);  // Main PIN update
}

//---------------------------------------------------------------------------------------------
void SetOUT_MainPinVsync(BYTE val)
{
	// 0 : vsync signal is output
	// 1 : composite sync signal
	// 2 : 2-D active signal
	// 3 : vactive signal is output
	stOUTCTRL.stOUT_m.stFINE.pin_vsync = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);  // Main PIN update
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxPinHsync(BYTE val)
{
	stOUTCTRL.stOUT_x.stFINE.pin_hsync = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);  // Aux PIN update
}

//---------------------------------------------------------------------------------------------
void SetOUT_AuxPinVsync(BYTE val)
{
	stOUTCTRL.stOUT_x.stFINE.pin_vsync = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);  // Aux PIN update
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_MainPinDe(void)
{
	return stOUTCTRL.stOUT_m.stFINE.pin_de;
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_MainPinHsync(void)
{
	return stOUTCTRL.stOUT_m.stFINE.pin_hsync;
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_MainPinVsync(void)
{
	return stOUTCTRL.stOUT_m.stFINE.pin_vsync;
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_AuxPinHsync(void)
{
	return stOUTCTRL.stOUT_x.stFINE.pin_hsync;
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_AuxPinVsync(void)
{
	return stOUTCTRL.stOUT_x.stFINE.pin_vsync;
}


//---------------------------------------------------------------------------------------------
void SetOUT_PipBlend(BYTE val)
{
	// 0:opaque ~ 15:transparent
	stOUTCTRL.stPIPctl.blend_lev = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_PIP | OUTCTRL_TRIGGER_ISP);  // PIP update
}

//---------------------------------------------------------------------------------------------
void SetOUT_PipChromaKeyEn(BOOL OnOff)
{
	stOUTCTRL.stPIPctl.key_en = OnOff;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_PIP | OUTCTRL_TRIGGER_ISP);  // PIP update
}

//---------------------------------------------------------------------------------------------
void SetOUT_PipChromaKeyYUBound(BYTE val)
{
	stOUTCTRL.stPIPctl.key_y_u = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_PIP | OUTCTRL_TRIGGER_ISP);  // PIP update
}

//---------------------------------------------------------------------------------------------
void SetOUT_PipChromaKeyYLBound(BYTE val)
{
	stOUTCTRL.stPIPctl.key_y_l = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_PIP | OUTCTRL_TRIGGER_ISP);  // PIP update
}

//---------------------------------------------------------------------------------------------
void SetOUT_PipChromaKeyCbUBound(BYTE val)
{
	stOUTCTRL.stPIPctl.key_cb_u = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_PIP | OUTCTRL_TRIGGER_ISP);  // PIP update
}

//---------------------------------------------------------------------------------------------
void SetOUT_PipChromaKeyCbLBound(BYTE val)
{
	stOUTCTRL.stPIPctl.key_cb_l = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_PIP | OUTCTRL_TRIGGER_ISP);  // PIP update
}

//---------------------------------------------------------------------------------------------
void SetOUT_PipChromaKeyCrUBound(BYTE val)
{
	stOUTCTRL.stPIPctl.key_cr_u = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_PIP | OUTCTRL_TRIGGER_ISP);  // PIP update
}

//---------------------------------------------------------------------------------------------
void SetOUT_PipChromaKeyCrLBound(BYTE val)
{
	stOUTCTRL.stPIPctl.key_cr_l = val;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_PIP | OUTCTRL_TRIGGER_ISP);  // PIP update
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_PipBlend(void)
{
	return stOUTCTRL.stPIPctl.blend_lev;
}

//---------------------------------------------------------------------------------------------
BOOL GetOUT_PipChromaKeyEn(void)
{
	return stOUTCTRL.stPIPctl.key_en;
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_PipChromaKeyYUBound(void)
{
	return stOUTCTRL.stPIPctl.key_y_u;
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_PipChromaKeyYLBound(void)
{
	return stOUTCTRL.stPIPctl.key_y_l;
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_PipChromaKeyCbUBound(void)
{
	return stOUTCTRL.stPIPctl.key_cb_u;
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_PipChromaKeyCbLBound(void)
{
	return stOUTCTRL.stPIPctl.key_cb_l;
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_PipChromaKeyCrUBound(void)
{
	return stOUTCTRL.stPIPctl.key_cr_u;
}

//---------------------------------------------------------------------------------------------
BYTE GetOUT_PipChromaKeyCrLBound(void)
{
	return stOUTCTRL.stPIPctl.key_cr_l;
}

//---------------------------------------------------------------------------------------------
void SetOUT_PipGradBlendSel(BYTE window_no, BYTE Sel)
{
	stOUTCTRL.stPIPctl.pip_blend_window[window_no].ctrl_update |= 0x02 ;
	stOUTCTRL.stPIPctl.pip_blend_window[window_no].blend_sel = Sel ;
	
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_PIP | OUTCTRL_TRIGGER_ISP);  // PIP update
}
//---------------------------------------------------------------------------------------------
void SetOUT_PipGradBlendEn(BYTE window_no, BYTE en, BYTE hv_mode, WORD initial, WORD step)
{
	stOUTCTRL.stPIPctl.pip_blend_window[window_no].ctrl_update |= 0x04 ;
	stOUTCTRL.stPIPctl.pip_blend_window[window_no].grad_blend_en = en ;
	stOUTCTRL.stPIPctl.pip_blend_window[window_no].grad_blend_mode = hv_mode ;
	stOUTCTRL.stPIPctl.pip_blend_window[window_no].grad_blend_init = initial ;
	stOUTCTRL.stPIPctl.pip_blend_window[window_no].grad_blend_step = step ;
	
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_PIP | OUTCTRL_TRIGGER_ISP);  // PIP update
}
//---------------------------------------------------------------------------------------------
void SetOUT_PipGradBlendPos(BYTE window_no, WORD sh, WORD sv, WORD eh, WORD ev)
{
	stOUTCTRL.stPIPctl.pip_blend_window[window_no].ctrl_update |= 0x01 ;
	stOUTCTRL.stPIPctl.pip_blend_window[window_no].grad_blend_sh = sh ;
	stOUTCTRL.stPIPctl.pip_blend_window[window_no].grad_blend_sv = sv ;
	stOUTCTRL.stPIPctl.pip_blend_window[window_no].grad_blend_eh = eh ;
	stOUTCTRL.stPIPctl.pip_blend_window[window_no].grad_blend_ev = ev ;
	
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_PIP | OUTCTRL_TRIGGER_ISP);  // PIP update
}

//------------------------------------------------------------------------------------
void SetOUT_PinMapExtn(BOOL OnOff) {
	stOUTCTRL.stPinMap.extn_en = OnOff;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_PINMAP | OUTCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
BYTE GetOUT_PinMapExtn(void) {
	return stOUTCTRL.stPinMap.extn_en;
}

//------------------------------------------------------------------------------------
void SetOUT_PinMapLvds(BOOL OnOff) {
	stOUTCTRL.stPinMap.lvds_en = OnOff;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_PINMAP | OUTCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
BYTE GetOUT_PinMapLvds(void) {
	return stOUTCTRL.stPinMap.lvds_en;
}

//------------------------------------------------------------------------------------
void SetOUT_PinMapGpio(BOOL OnOff) {
	stOUTCTRL.stPinMap.gpio_en = OnOff;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_PINMAP | OUTCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
BYTE GetOUT_PinMapGpio(void) {
	return stOUTCTRL.stPinMap.gpio_en;
}

//------------------------------------------------------------------------------------
void SetOUT_PinMapMode(BYTE mode) {
	stOUTCTRL.stPinMap.mode_sel = mode;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_PINMAP | OUTCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
BYTE GetOUT_PinMapMode(void) {
	return stOUTCTRL.stPinMap.mode_sel;
}

//------------------------------------------------------------------------------------
void SetOUT_MainTfPolar(BOOL OnOff)
{
	stOUTCTRL.stOUT_m.stFINE.tf_polar = OnOff;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
void SetOUT_AuxTfPolar(BOOL OnOff)
{
	stOUTCTRL.stOUT_x.stFINE.tf_polar = OnOff;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
BOOL GetOUT_MainTfPolar(void)
{
	return stOUTCTRL.stOUT_m.stFINE.tf_polar;
}

//------------------------------------------------------------------------------------
BOOL GetOUT_AuxTfPolar(void)
{
	return stOUTCTRL.stOUT_x.stFINE.tf_polar;
}
//------------------------------------------------------------------------------------
void SetOUT_ExtLockMode(BYTE lock_mode, BYTE lock_source)
{
	stOUTCTRL.stExtLock.lock_mode = lock_mode;
		// 0 : Main sync free run [normal mode]
		// 1 : Main sync free run with single reset pulse [ Ext sync property is similar to Main sync ]
		// 2 : Main sync lock mode with sync reset delay enable
		// 3 : Main sync free run with single reset pulse + delay enable [ Ext sync property is similar to Main sync ]
	
	stOUTCTRL.stExtLock.lock_source = lock_source;
		// Only for lock_mode is set to 1 or 2.
		// 0 : separate sync signals on the main input port
		// 1 : separate sync signals on the aux input port
		// 2 : selected sync signals for the main input path
		// 3 : selected sync signals for the aux input path
		// 4 : sync signals from MAIN input DTR decoder 
		// 5 : sync signals from main sensor input	
		// 6:  sync signals from AUX input DTR decoder
		// 7 : sync signals from aux sensor input
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_EXTLOCK | OUTCTRL_TRIGGER_ISP);
}
//------------------------------------------------------------------------------------
void SetOUT_ExtSyncMode(WORD sync_mode)
{
	stOUTCTRL.stExtLock.sync_mode = sync_mode;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_EXTLOCK | OUTCTRL_TRIGGER_ISP);
}
//------------------------------------------------------------------------------------
void SetOUT_ExtLockHSdelay(WORD dly)
{
	stOUTCTRL.stExtLock.hs_dly = dly;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_EXTLOCK | OUTCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
void SetOUT_ExtLockVSdelay(WORD dly)
{
	stOUTCTRL.stExtLock.vs_dly = dly;
	stOUTCTRL.ctrl |= (OUTCTRL_UPDATE_EXTLOCK | OUTCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
WORD GetOUT_ExtLockHSdelay(void)
{
	return stOUTCTRL.stExtLock.hs_dly;
}

//------------------------------------------------------------------------------------
WORD GetOUT_ExtLockVSdelay(void)
{
	return stOUTCTRL.stExtLock.vs_dly;
}

//------------------------------------------------------------------------------------
BOOL IsOUT_MainFrmt4K(void)
{
	switch(stOUTCTRL.stOUT_m.frmt)	{
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
		case MISP_VIDOUT_3840x2160p30:		return TRUE;
		case MISP_VIDOUT_3840x2160p25:		return TRUE;
		case MISP_VIDOUT_3840x2160p24:		return TRUE;
		case MISP_VIDOUT_3840x2160p15:		return TRUE;
		case MISP_VIDOUT_3840x2160p12:		return TRUE;
		case MISP_VIDOUT_4096x2160p30:		return TRUE;
		case MISP_VIDOUT_4096x2160p25:		return TRUE;
		case MISP_VIDOUT_4096x2160p24:		return TRUE;
#if defined(__USE_59Hz_OUTPUT__)
		case MISP_VIDOUT_3840x2160p29:		return TRUE;
		case MISP_VIDOUT_4096x2160p29:		return TRUE;
#endif
#endif
		default:							return FALSE;
	}
}

//------------------------------------------------------------------------------------
BOOL IsOUT_MainFrmt4M(void)
{
	switch(stOUTCTRL.stOUT_m.frmt)	{
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
		case MISP_VIDOUT_2560x1440p30:		return TRUE;
		case MISP_VIDOUT_2560x1440p25:		return TRUE;
#endif
		default:							return FALSE;
	}
}

//------------------------------------------------------------------------------------
BOOL IsOUT_MainFrmt3G(void)
{
	switch(stOUTCTRL.stOUT_m.frmt)	{
		case MISP_VIDOUT_1920x1080p60:		return TRUE;
		case MISP_VIDOUT_1920x1080p50:		return TRUE;
#if defined(__USE_59Hz_OUTPUT__)
		case MISP_VIDOUT_1920x1080p59:		return TRUE;
#endif
		default:							return FALSE;
	}
}

//------------------------------------------------------------------------------------
BOOL IsOUT_MainFrmt12G(void)
{
	switch(stOUTCTRL.stOUT_m.frmt)	{
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
#if defined(__USE_4K60_INOUT__)
		case MISP_VIDOUT_1920x2160p60:		return TRUE;
		case MISP_VIDOUT_1920x2160p50:		return TRUE;
		case MISP_VIDOUT_2048x2160p60:		return TRUE;
		case MISP_VIDOUT_2048x2160p50:		return TRUE;
#if defined(__USE_59Hz_OUTPUT__)
		case MISP_VIDOUT_1920x2160p59:		return TRUE;
		case MISP_VIDOUT_2048x2160p59:		return TRUE;
#endif
#endif
#endif
		default:							return FALSE;
	}
}

//------------------------------------------------------------------------------------
BOOL IsOUT_MainSyncEmb(void)
{
	if((stOUTCTRL.stOUT_m.mode == MISP_OUT_EMB422_8) || (stOUTCTRL.stOUT_m.mode == MISP_OUT_EMB422_10))
		return  TRUE;
	else
		return  FALSE;
}

//--------------------------------------------------------------------------------------------------
WORD GetOUT_MainFps(void)
{
	WORD fps;
	
	switch(stOUTCTRL.stOUT_m.frmt)	{
		case  MISP_VIDOUT_720x480i60:	case  MISP_VIDOUT_960x480i60:
		case  MISP_VIDOUT_720x480p60:	case  MISP_VIDOUT_960x480p60:
		case  MISP_VIDOUT_1280x720p60:	case  MISP_VIDOUT_1920x1080i60:
		case  MISP_VIDOUT_1920x1080p60:
				fps = 60;	break;
		
		case  MISP_VIDOUT_720x480p50:	case  MISP_VIDOUT_960x480p50:
		case  MISP_VIDOUT_720x576i50:	case  MISP_VIDOUT_960x576i50:
		case  MISP_VIDOUT_720x576p50:	case  MISP_VIDOUT_960x576p50:
		case  MISP_VIDOUT_1280x720p50:	case  MISP_VIDOUT_1920x1080i50:
		case  MISP_VIDOUT_1920x1080p50:
				fps = 50;	break;
		
		case  MISP_VIDOUT_720x480p30:	case  MISP_VIDOUT_960x480p30:
		case  MISP_VIDOUT_1280x720p30:	case  MISP_VIDOUT_1920x1080p30:
				fps = 30;	break;
		
		case  MISP_VIDOUT_1280x720p25:	case  MISP_VIDOUT_1920x1080p25:
				fps = 25;	break;
		
		case  MISP_VIDOUT_720x480p24:	case  MISP_VIDOUT_960x480p24:
		case  MISP_VIDOUT_1280x720p24:	case  MISP_VIDOUT_1920x1080p24:
				fps = 24;	break;
		
		case  MISP_VIDOUT_1280x720p20:	case  MISP_VIDOUT_1920x1080p20:
				fps = 20;	break;
		
		case  MISP_VIDOUT_1280x720p15:	case  MISP_VIDOUT_1920x1080p15:
				fps = 15;	break;
		
		case  MISP_VIDOUT_1280x720p10:	case  MISP_VIDOUT_1920x1080p10:
				fps = 10;	break;
		
		case  MISP_VIDOUT_1280x720p7d5:	case  MISP_VIDOUT_1920x1080p7d5:
				fps = 7;	break;
		
		case  MISP_VIDOUT_1280x720p05:	case  MISP_VIDOUT_1920x1080p05:
				fps = 5;	break;
		
		case  MISP_VIDOUT_1280x720p100:	fps = 100;	break;
		case  MISP_VIDOUT_1280x720p120:	fps = 120;	break;
		
#if defined(__USE_59Hz_OUTPUT__)
		case  MISP_VIDOUT_1280x720p59:	case  MISP_VIDOUT_1920x1080i59:
		case  MISP_VIDOUT_1920x1080p59:
				fps = 59;	break;
		case  MISP_VIDOUT_1920x1080p29:		fps = 29;	break;
#endif
		
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)
		case  MISP_VIDOUT_3840x2160p30:	case  MISP_VIDOUT_4096x2160p30:
		case  MISP_VIDOUT_2560x1440p30:
				fps = 30;	break;
		
		case  MISP_VIDOUT_3840x2160p25:	case  MISP_VIDOUT_4096x2160p25:
		case  MISP_VIDOUT_2560x1440p25:
				fps = 25;	break;
		
		case  MISP_VIDOUT_3840x2160p24:	case  MISP_VIDOUT_4096x2160p24:
				fps = 24;	break;
		
		case  MISP_VIDOUT_3840x2160p15:		fps = 15;	break;
		case  MISP_VIDOUT_3840x2160p12:		fps = 12;	break;
		case  MISP_VIDOUT_3840x2160p10:		fps = 10;	break;
		case  MISP_VIDOUT_3840x2160p7d5:	fps = 7;	break;
		case  MISP_VIDOUT_3840x2160p05:		fps = 5;	break;
		case  MISP_VIDOUT_1920x1080p100:	fps = 100;	break;
		case  MISP_VIDOUT_1920x1080p120:	fps = 120;	break;
#if defined(__USE_59Hz_OUTPUT__)
		case  MISP_VIDOUT_3840x2160p29:		fps = 29;	break;
		case  MISP_VIDOUT_4096x2160p29:		fps = 29;	break;
#endif
#if defined(__USE_4K60_INOUT__)
		case  MISP_VIDOUT_1920x2160p60:		fps = 60;	break;
		case  MISP_VIDOUT_1920x2160p50:		fps = 50;	break;
		case  MISP_VIDOUT_2048x2160p60:		fps = 60;	break;
		case  MISP_VIDOUT_2048x2160p50:		fps = 50;	break;
#if defined(__USE_59Hz_OUTPUT__)
		case  MISP_VIDOUT_1920x2160p59:		fps = 59;	break;
		case  MISP_VIDOUT_2048x2160p59:		fps = 59;	break;
#endif
#endif
#endif
		default:							fps = 60;	break;
	}
	
	return fps;
}


/*  FILE_END_HERE */
