//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MISPIN.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISPINPUT_H__
#define		__MISPINPUT_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

typedef	enum {
	// TV
	MISP_VIDSRC_720x480i60 = 0,
	MISP_VIDSRC_720x576i50,
	MISP_VIDSRC_960x480i60,
	MISP_VIDSRC_960x576i50,
	MISP_VIDSRC_720x480p60,
	MISP_VIDSRC_720x576p50,
	MISP_VIDSRC_960x480p60,
	MISP_VIDSRC_960x576p50,
	MISP_VIDSRC_1280x720p60,
#if defined(__USE_59Hz_OUTPUT__)
	MISP_VIDSRC_1280x720p59,
#endif
	MISP_VIDSRC_1280x720p50,
	MISP_VIDSRC_1280x720p30,
	MISP_VIDSRC_1280x720p25,
	MISP_VIDSRC_1280x720p24,
	MISP_VIDSRC_1920x1080i60,
#if defined(__USE_59Hz_OUTPUT__)
	MISP_VIDSRC_1920x1080i59,
#endif
	MISP_VIDSRC_1920x1080i50,
	MISP_VIDSRC_1920x1080p60,
#if defined(__USE_59Hz_OUTPUT__)
	MISP_VIDSRC_1920x1080p59,
#endif
	MISP_VIDSRC_1920x1080p50,
	MISP_VIDSRC_1920x1080p30,
#if defined(__USE_59Hz_OUTPUT__)
	MISP_VIDSRC_1920x1080p29,
#endif
	MISP_VIDSRC_1920x1080p25,
	MISP_VIDSRC_1920x1080p24,
	
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	MISP_VIDSRC_3840x2160p30,
#if defined(__USE_59Hz_OUTPUT__)
	MISP_VIDSRC_3840x2160p29,
#endif
	MISP_VIDSRC_3840x2160p25,
	MISP_VIDSRC_3840x2160p24,
	MISP_VIDSRC_3840x2160p15,
	MISP_VIDSRC_3840x2160p12,
	MISP_VIDSRC_4096x2160p30,
#if defined(__USE_59Hz_OUTPUT__)
	MISP_VIDSRC_4096x2160p29,
#endif
	MISP_VIDSRC_4096x2160p25,
	MISP_VIDSRC_4096x2160p24,
	MISP_VIDSRC_2560x1440p30,	// 2560x1440p 30Hz (4M)
	MISP_VIDSRC_2560x1440p25,	// 2560x1440p 25Hz (4M)
#if defined(__USE_4K60_INOUT__)
	MISP_VIDSRC_1920x2160p60,	// (3840/2)x2160p60
#if defined(__USE_59Hz_OUTPUT__)
	MISP_VIDSRC_1920x2160p59,	// (3840/2)x2160p59
#endif
	MISP_VIDSRC_1920x2160p50,	// (3840/2)x2160p50
	MISP_VIDSRC_2048x2160p60,	// (4096/2)x2160p60
#if defined(__USE_59Hz_OUTPUT__)
	MISP_VIDSRC_2048x2160p59,	// (4096/2)x2160p59
#endif
	MISP_VIDSRC_2048x2160p50,	// (4096/2)x2160p50
#endif
#endif
	
	
	// 120hz format
	MISP_VIDSRC_1280x720p120,	// HD sensor 120fps
	MISP_VIDSRC_1280x720p100,	// HD sensor 100fps
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	MISP_VIDSRC_1920x1080p120,	// FHD sensor 120fps
	MISP_VIDSRC_1920x1080p100,	// FHD sensor 100fps
#endif
	// 240hz format
	MISP_VIDSRC_1280x720p240,	// HD sensor 240fps
	MISP_VIDSRC_1280x720p200,	// HD sensor 200fps	
	
	// PC
	MISP_VIDSRC_640x480p60,
	MISP_VIDSRC_640x480p75,
	MISP_VIDSRC_800x600p60,
	MISP_VIDSRC_800x600p75,
	MISP_VIDSRC_1024x768p60,
	MISP_VIDSRC_1024x768p75,
	MISP_VIDSRC_1280x960p60,
	MISP_VIDSRC_1280x1024p60,
	MISP_VIDSRC_1280x1024p75,
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	MISP_VIDSRC_1280x1024p100,
#endif
	MISP_VIDSRC_1360x768p60,
	MISP_VIDSRC_1366x768p60,
	MISP_VIDSRC_1400x1050p60,
	MISP_VIDSRC_1600x1200p60,
	
	// ETC
	MISP_VIDSRC_1440x900p60,
	MISP_VIDSRC_1440x900p75,
	MISP_VIDSRC_1680x1050p60,
	MISP_VIDSRC_1680x1050pRB,
	MISP_VIDSRC_1920x1080pRB,
	MISP_VIDSRC_1920x1200pRB,
	
	MISP_VIDSRC_1280x720pRGB,
	
	MISP_VIDSRC_640x480p72,
	MISP_VIDSRC_800x600p56,
	MISP_VIDSRC_800x600p72,
	MISP_VIDSRC_1024x768p70,
	MISP_VIDSRC_1152x864p75,
	MISP_VIDSRC_1280x800p60,
	
	// SENSOR
	MISP_VIDSRC_1368x1049p60,	// imx076
	MISP_VIDSRC_2048x1536p50,	// imx265(3M,all scan max)
	MISP_VIDSRC_2048x1536p30,	// imx124(3M)
	MISP_VIDSRC_2048x1536p25,	// imx124(3M)
	MISP_VIDSRC_2592x1536p15,	// MT9P031(5M)
	MISP_VIDSRC_3072x1728p30,	// imx178(5M 16:9), imx326(5M)
	MISP_VIDSRC_3072x1728p25,	// imx178(5M 16:9), imx326(5M)
	MISP_VIDSRC_2592x1944p30,	// imx326(5M)
	MISP_VIDSRC_2592x1944p15,	// imx326(5M)
	MISP_VIDSRC_3072x2160p30,	// imx326(6M)
	MISP_VIDSRC_2160x2160p30,	// imx326(1:1 2160p)
	MISP_VIDSRC_SS2560x1440p30,	// PS1400k(16:9, 4M )
	MISP_VIDSRC_SS3840x2160p25,	// imx226
	MISP_VIDSRC_4096x2160p20,	// imx226
	MISP_VIDSRC_4000x3000p22,	// imx226	12M 22.699fps
	MISP_VIDSRC_1440x1080p60,	// imx273	allscan
	MISP_VIDSRC_2048x1208p30,		//__USE_SENS_PANNING_MODE__
	MISP_VIDSRC_2048x1208p25,		//__USE_SENS_PANNING_MODE__
	MISP_VIDSRC_1344x1088p30,
	MISP_VIDSRC_1344x1088p25,
	
		
	MISP_VIDSRC_FORMAT_END		// input format end

}	MISP_SRCVIDEO_FORMAT_t;


typedef	enum {
	MISP_SRC_RGB444_8 = 0,		// RGB 4:4:4 8-bit with separate sync
	MISP_SRC_RGB444_10,			// RGB 4:4:4 10-bit with separate sync
	
	MISP_SRC_YUV444_8,			// YCbCr 4:4:4 8-bit with separate sync	
	MISP_SRC_YUV444_10,			// YCbCr 4:4:4 10-bit with separate sync
	
	MISP_SRC_EMB422_8,			// YCbCr 4:2:2 8-bit with embedded sync	
	MISP_SRC_EMB422_10,			// YCbCr 4:2:2 10-bit with embedded sync
	
	MISP_SRC_SEP422_8,			// YCbCr 4:2:2 8-bit with separate sync
	MISP_SRC_SEP422_10,			// YCbCr 4:2:2 10-bit with separate sync

	MISP_SRC_MUX656_8,			// Y/C-mux 4:2:2 8-bit with embedded sync	
	MISP_SRC_MUX656_10,			// Y/C-mux 4:2:2 10-bit with embedded sync
	
	MISP_SRC_SEP656_8,			// Y/C-mux 4:2:2 8-bit with seperate sync
	MISP_SRC_SEP656_10			// Y/C-mux 4:2:2 10-bit with seperate sync
	
}	MISP_SRCPORT_MODE_t;


#if defined(__USE_MDIN_i500__)
typedef	enum {
	IN_BAYER_12B = 0,				// in(12b) : lvds(6ch) + bayer(12b)
	IN_BAYER_12B_P1_656_10B,		// in(22b) : lvds(6ch) + bayer(12b) + p1(656_10b)
	IN_BAYER_12B_P1_422_20B,		// in(32b) : lvds(6ch) + bayer(12b) + p1(422_20b)
	IN_P0_656_10B = 4,				// in(10b) : lvds(6ch) + p0(656_10b)
	IN_P0_656_10B_P1_656_10B,		// in(20b) : lvds(6ch) + p0(656_10b) + p1(656_10b)
	IN_P0_656_10B_P1_422_20B,		// in(30b) : lvds(6ch) + p0(656_10b) + p1(422_20b)
	IN_P0_422_20B = 8,				// in(20b) : lvds(6ch) + p0(422_20b)
	IN_P0_422_20B_P1_656_10B,		// in(30b) : lvds(6ch) + p0(422_20b) + p1(656_10b)
	IN_P0_444_30BIT = 12,			// in(30b) : lvds(6ch) + p0(444_30b)
	IN_P0_444_22B = 15,				// in(22b) : lvds(6ch) + p0(444_22b)
	
}	MISP_INPUT_PINMAP_t;
#endif


#if defined(__USE_MDIN_i510__)
// lvds input mode
typedef	enum {
	IN_LVDS_6CH = 0,						// NLI  0,     in(12b) : lvds(6ch_12b)
	IN_LVDS_4CH = 3,						// NLI  3,     in( 8b) : lvds(4ch_8b)
	
}	MISP_INPUT_LVDS_PINMAP_t;

// cmos input mode
typedef	enum {
	IN_BAYER_12B = 0,						// NCI  0,     in(12b) : bayer(12b)
	
}	MISP_INPUT_CMOS_PINMAP_t;
#endif


#if defined(__USE_MDIN_i540__)
// lvds input mode
typedef	enum {
	IN_LVDS_6CH = 0,						// NLI  0,     in(12b) : lvds(6ch_12b)
	IN_LVDS_6CH_P1_656_10B,					// NLI  1,     in(22b) : lvds(6ch_12b) + p1(656_10b)
	IN_LVDS_6CH_P1_422_20B,					// NLI  2,     in(32b) : lvds(6ch_12b) + p1(422_20b)
	IN_LVDS_4CH = 3,						// NLI  3,     in( 8b) : lvds(4ch_8b)    -- same as NLI(11)
	IN_LVDS_4CH_X2B_P1_656_10B,				// NLI  4,     in(20b) : lvds(4ch_8b) + dummy(2b) + p1(656_10b)
	IN_LVDS_4CH_X2B_P1_422_20B,				// NLI  5,     in(30b) : lvds(4ch_8b) + dummy(2b) + p1(422_20b)
	IN_LVDS_8CH = 6,						// NLI  6,     in(16b) : lvds(8ch_16b)    -- same as NLI(8,14,16)
	IN_LVDS_8CH_X4B_P1_656_10B,				// NLI  7,     in(30b) : lvds(8ch_16b) + dummy(4b) + p1(656_10b)
	IN_LVDS_6CH_P1_656_8B = 9,				// NLI  9,     in(20b) : lvds(6ch_12b) + p1(656_8b)
	IN_LVDS_6CH_P1_422_16B,					// NLI 10,     in(28b) : lvds(6ch_12b) + p1(422_16b)
	IN_LVDS_4CH_P1_656_8B = 12,				// NLI 12,     in(16b) : lvds(4ch_8b) + p1(656_8b)
	IN_LVDS_4CH_P1_422_16B,					// NLI 13,     in(24b) : lvds(4ch_8b) + p1(422_16b)
	IN_LVDS_8CH_P1_656_8B = 15,				// NLI 15,     in(24b) : lvds(8ch_16b) + p1(656_8b)
	IN_LVDS_8CH_P1_422_16B = 17,			// NLI 17,     in(32b) : lvds(8ch_16b) + p1(422_16b)
	
}	MISP_INPUT_LVDS_PINMAP_t;

// cmos input mode
typedef	enum {
	IN_BAYER_12B = 0,						// NCI  0,     in(12b) : bayer(12b)
	IN_BAYER_12B_P1_656_10B,				// NCI  1,     in(22b) : bayer(12b) + p1(656_10b)
	IN_BAYER_12B_P1_422_20B = 2,			// NCI  2,     in(32b) : bayer(12b) + p1(422_20b)
	IN_BAYER_12B_P1_444DE_15B = 2,			// NCI  2(DE), in(27b) : bayer(12b) + p1(444de_15b)
	IN_P0_656_10B,							// NCI  3,     in(10b) : p0(656_10b)
	IN_P0_656_10B_P1_656_10B,				// NCI  4,     in(20b) : p0(656_10b) + p1(656_10b)
	IN_P0_656_10B_P1_422_20B = 5,			// NCI  5,     in(30b) : p0(656_10b) + p1(422_20b)
	IN_P0_656_10B_P1_444DE_15B = 5,			// NCI  5(DE), in(25b) : p0(656_10b) + p1(444de_15b)
	IN_P0_422_20B = 6,						// NCI  6,     in(20b) : p0(422_20b)
	IN_P0_444DE_15B = 6,					// NCI  6(DE), in(15b) : p0(444de_15b)
	IN_P0_422_20B_P1_656_10B = 7,			// NCI  7,     in(30b) : p0(422_20b) + p1(656_10b)
	IN_P0_444DE_15B_X5B_P1_656_10B = 7,		// NCI  7(DE), in(30b) : p0(444de_15b) + dummy(5b) + p1(656_10b)
	IN_P0_444_30B = 8,						// NCI  8,     in(30b) : p0(444_30b)
	IN_P0_444DE_15B_P1_444DE_15B = 8,		// NCI  8(DE), in(30b) : p0(444de_15b) + p1(444de_15b)
	IN_BAYER_12B_P1_656_8B,					// NCI  9,     in(20b) : bayer(12b) + p1(656_8b)
	IN_BAYER_12B_P1_422_16B = 10,			// NCI 10,     in(28b) : bayer(12b) + p1(422_16b)
	IN_BAYER_12B_P1_444DE_12B = 10,			// NCI 10(DE), in(24b) : bayer(12b) + p1(444de_12b)
	IN_P0_656_8B,							// NCI 11,     in( 8b) : p0(656_8b)
	IN_P0_656_8B_P1_656_8B,					// NCI 12,     in(16b) : p0(656_8b) + p1(656_8b)
	IN_P0_656_8B_P1_422_16B = 13,			// NCI 13,     in(24b) : p0(656_8b) + p1(422_16b)
	IN_P0_656_8B_P1_444DE_12B = 13,			// NCI 13(DE), in(20b) : p0(656_8b) + p1(444de_12b)
	IN_P0_422_16B = 14,						// NCI 14,     in(16b) : p0(422_16b)
	IN_P0_444DE_12B = 14,					// NCI 14(DE), in(12b) : p0(444de_12b)
	IN_P0_422_16B_P1_656_8B = 15,			// NCI 15,     in(24b) : p0(422_16b) + p1(656_8b)
	IN_P0_444_12B_X4B_P1_656_8B = 15,		// NCI 15(DE), in(24b) : p0(444de_12b) + dummy(4b) + p1(656_8b)
	IN_P0_444_24B = 16,						// NCI 16,     in(24b) : p0(444_24b)
	IN_P0_444DE_12B_P1_444DE_12B = 16,		// NCI 16(DE), in(24b) : p0(444de_12b) + p1(444de_12b)
	IN_P0_422_16B_P1_422_16B = 17,			// NCI 17,     in(32b) : p0(422_16b) + p1(422_16b)
	IN_P0_444DE_12B_X4B_P1_444DE_12B = 17,	// NCI 17(DE), in(28b) : p0(444de_12b) + dummy(4b) + p1(444de_12b)
	
}	MISP_INPUT_CMOS_PINMAP_t;
#endif


#if defined(__USE_MDIN_i550__)
// extended lvds input mode
typedef	enum {
	IN_LVDS_6CH = 0,						// ELI  0,     in(12b) : lvds(6ch_12b)
	IN_LVDS_6CH_P1_656_10B,					// ELI  1,     in(22b) : lvds(6ch_12b) + p1(656_10b)
	IN_LVDS_6CH_P1_422_20B,					// ELI  2,     in(32b) : lvds(6ch_12b) + p1(422_20b)
	IN_LVDS_6CH_P1_444_30B,					// ELI  3,     in(42b) : lvds(6ch_12b) + p1(444_30b)
	IN_LVDS_4CH,							// ELI  4,     in( 8b) : lvds(4ch_8b)
	IN_LVDS_4CH_X2B_P1_656_10B,				// ELI  5,     in(20b) : lvds(4ch_8b) + dummy(2b) + p1(656_10b)
	IN_LVDS_4CH_X2B_P1_422_20B,				// ELI  6,     in(30b) : lvds(4ch_8b) + dummy(2b) + p1(422_20b)
	IN_LVDS_4CH_X2B_P1_444_30B,				// ELI  7,     in(40b) : lvds(4ch_8b) + dummy(2b) + p1(444_30b)
	IN_LVDS_8CH,							// ELI  8,     in(16b) : lvds(8ch_16b)    -- same as ELI(12,17,19,22,23)
	IN_LVDS_8CH_X4B_P1_656_10B,				// ELI  9,     in(30b) : lvds(8ch_16b) + dummy(4b) + p1(656_10b)
	IN_LVDS_8CH_X4B_P1_422_20B,				// ELI 10,     in(40b) : lvds(8ch_16b) + dummy(4b) + p1(422_20b)
	IN_LVDS_8CH_X4B_P1_444_30B,				// ELI 11,     in(50b) : lvds(8ch_16b) + dummy(4b) + p1(444_30b)
	IN_LVDS_8CH_X14B_P1_656_10B = 13,		// ELI 13,     in(40b) : lvds(8ch_16b) + dummy(14b) + p1(656_10b)
	IN_LVDS_8CH_X14B_P1_422_20B,			// ELI 14,     in(50b) : lvds(8ch_16b) + dummy(14b) + p1(422_20b)
	IN_LVDS_8CH_X8B_P1_422_16B = 15,		// ELI 15,     in(40b) : lvds(8ch_16b) + dummy(8b) + p1(422_16b)
	IN_LVDS_8CH_X8B_P1_656_8B = 15,			// ELI 15A,    in(32b) : lvds(8ch_16b) + dummy(8b) + p1(656_8b)
	IN_LVDS_8CH_P1_444_24B = 16,			// ELI 16,     in(40b) : lvds(8ch_16b) + p1(444_24b)
	IN_LVDS_8CH_P1_422_16B = 16,			// ELI 16A,    in(32b) : lvds(8ch_16b) + p1(422_16b)
	IN_LVDS_8CH_P1_656_8B = 16,				// ELI 16B,    in(24b) : lvds(8ch_16b) + p1(656_8b)
	IN_LVDS_8CH_X24B_P1_656_8B = 18,		// ELI 18,     in(48b) : lvds(8ch_16b) + dummy(24b) + p1(656_8b)
	IN_LVDS_8CH_X16B_P1_656_8B = 20,		// ELI 20,     in(40b) : lvds(8ch_16b) + dummy(16b) + p1(656_8b)
	IN_LVDS_8CH_X16B_P1_422_16B,			// ELI 21,     in(48b) : lvds(8ch_16b) + dummy(16b) + p1(422_16b)
	IN_LVDS_8CH_X8B_P1_444_16B_GPA_444_8B = 24,		// ELI 24,     in(48b) : lvds(8ch_16b) + dummy(8b) + p1(444_16b) + gpio-a(444_8b)
	IN_LVDS_8CH_X6B,								// ELI 25,     in(22b) : lvds(8ch_16b) + dummy(6b)  -- same as ELI(31)
	IN_LVDS_8CH_X6B_OUT20B_P1_656_10B_X12B,			// ELI 26,     in(44b) : lvds(8ch_16b) + dummy(6b) + out(20b) + p1(656_10b) + dummy(12b)
	IN_LVDS_8CH_X6B_OUT20B_P1_422_20B_X2B,			// ELI 27,     in(44b) : lvds(8ch_16b) + dummy(6b) + out(20b) + p1(422_20b) + dummy(2b)  -- same as ELI(28)
	IN_LVDS_8CH_X6B_OUT20B_P1_444_22B = 29,			// ELI 29,     in(44b) : lvds(8ch_16b) + dummy(6b) + out(16b) + p1(444_22b) -- p1:ycbcr877_22b
	IN_LVDS_8CH_X6B_OUT20B_P1_444_22B_GPA_444_2B,	// ELI 30,     in(46b) : lvds(8ch_16b) + dummy(6b) + out(20b) + p1(444_22b) + gpio-a(444_2b)
	
}	MISP_INPUT_LVDS_PINMAP_t;

// extended cmos input mode
typedef	enum {
	IN_BAYER_12B = 0,						// ECI  0,     in(12b) : bayer(12b)
	IN_BAYER_12B_P1_656_10B,				// ECI  1,     in(22b) : bayer(12b) + p1(656_10b)
	IN_BAYER_12B_P1_422_20B = 2,			// ECI  2,     in(32b) : bayer(12b) + p1(422_20b)
	IN_BAYER_12B_P1_444DE_15B = 2,			// ECI  2(DE), in(27b) : bayer(12b) + p1(444de_15b)
	IN_BAYER_12B_P1_444_30B,				// ECI  3,     in(42b) : bayer(12b) + p1(444_30b)
	IN_P0_656_10B,							// ECI  4,     in(10b) : p0(656_10b)
	IN_P0_656_10B_P1_656_10B,				// ECI  5,     in(20b) : p0(656_10b) + p1(656_10b)
	IN_P0_656_10B_P1_422_20B = 6,			// ECI  6,     in(30b) : p0(656_10b) + p1(422_20b)
	IN_P0_656_10B_P1_444DE_15B = 6,			// ECI  6(DE), in(25b) : p0(656_10b) + p1(444de_15b)
	IN_P0_656_10B_P1_444_30B,				// ECI  7,     in(40b) : p0(656_10b) + p1(444_30b)
	IN_P0_422_20B = 8,						// ECI  8,     in(20b) : p0(422_20b)
	IN_P0_444DE_15B = 8,					// ECI  8(DE), in(15b) : p0(444de_15b)
	IN_P0_422_20B_P1_656_10B = 9,			// ECI  9,     in(30b) : p0(422_20b) + p1(656_10b)
	IN_P0_444DE_15B_X5B_P1_656_10B = 9,		// ECI  9(DE), in(30b) : p0(444de_15b) + dummy(5b) + p1(656_10b)
	IN_P0_422_20B_P1_422_20B = 10,			// ECI 10,     in(40b) : p0(422_20b) + p1(422_20b)
	IN_P0_422_20B_P1_444DE_15B = 10,		// ECI 10(DE), in(35b) : p0(422_20b) + p1(444de_15b)
	IN_P0_422_20B_P1_444_30B,				// ECI 11,     in(50b) : p0(422_20b) + p1(444_30b)
	IN_P0_444_30B = 12,						// ECI 12,     in(30b) : p0(444_30b)
	IN_P0_444DE_15B_P1_444DE_15B = 12,		// ECI 12(DE), in(30b) : p0(444de_15b) + p1(444de_15b)
	IN_P0_444_30B_P1_656_10B = 13,			// ECI 13,     in(40b) : p0(444_30b) + p1(656_10b)
	IN_P0_444DE_15B_X15B_P1_656_10B = 13,	// ECI 13(DE), in(40b) : p0(444de_15b) + dummy(15b) + p1(656_10b)
	IN_P0_444_30B_P1_422_20B = 14,			// ECI 14,     in(50b) : p0(444_30b) + p1(422_20b)
	IN_P0_444DE_15B_X15B_P1_444DE_15B = 14,	// ECI 14(DE), in(45b) : p0(444de_15b) + dummy(15b) + p1(444de_15b)
	IN_P0_444_24B_P1_422_16B = 15,			// ECI 15,     in(40b) : p0(444_24b) + p1(422_16b)
	IN_P0_444_24B_P1_656_8B = 15,			// ECI 15A,    in(32b) : p0(444_24b) + p1(656_8b)
	IN_P0_444DE_15B_X9B_P1_444DE_15B = 15,	// ECI 15(DE), in(39b) : p0(444de_15b) + dummy(9b) + p1(444de_15b)
	IN_P0_422_16B_P1_444_24B = 16,			// ECI 16,     in(40b) : p0(422_16b) + p1(444_24b)
	IN_P0_422_16B_P1_422_16B = 16,			// ECI 16A,    in(32b) : p0(422_16b) + p1(422_16b)
	IN_P0_422_16B_P1_656_8B = 16,			// ECI 16B,    in(24b) : p0(422_16b) + p1(656_8b)
	IN_P0_444DE_15B_X1B_P1_444DE_15B = 16,	// ECI 16(DE), in(31b) : p0(444de_15b) + dummy(1b) + p1(444de_15b)
	IN_P0_422DW_40B,						// ECI 17,     in(40b) : p0(422dw_40b)
	IN_P0_422DW_40B_P1_656_8B,				// ECI 18,     in(48b) : p0(422dw_40b) + p1(656_8b)
	IN_P0_422DW_32B,						// ECI 19,     in(32b) : p0(422dw_32b)
	IN_P0_422DW_32B_P1_656_8B,				// ECI 20,     in(40b) : p0(422dw_32b) + p1(656_8b)
	IN_P0_422DW_32B_P1_422_16B,				// ECI 21,     in(48b) : p0(422dw_32b) + p1(422_16b)
	IN_P0_422DW_48B,						// ECI 22,     in(48b) : p0(422dw_48b)
	IN_P0_422DW2_48B,						// ECI 23,     in(48b) : p0(422dw_48b)
	IN_P0_444_24B_P1_444_16B_GPA_444_8B,	// ECI 24,     in(48b) : p0(444_24b) + p1(444_16B) + gpio-a(444_8b)
	IN_P0_444_22B,							// ECI 25,     in(22b) : p0(444_22b)
	IN_P0_422_20B_X2B_OUT20B_P1_656_10B_X12B,				// ECI 26,     in(44b) : p0(422_20b) + dummy(2b) + out(20b) + p1(656_10b) + dummy(12b)
	IN_P0_422_20B_X2B_OUT20B_P1_422_20B_X2B = 27,			// ECI 27,     in(44b) : p0(422_20b) + dummy(2b) + out(20b) + p1(422_20b) + dummy(2b)
	IN_P0_444DE_15B_X7B_OUT20B_P1_444DE_15B_X7B = 27,		// ECI 27(DE), in(44b) : p0(444de_15b) + dummy(7b) + out(20b) + p1(444de_15b) + dummy(7b)
	IN_P0_444_22B_OUT18B_P1_422_20B_X2B,					// ECI 28,     in(44b) : p0(444_22b) + out(18b) + p1(422_20b) + dummy(2b)  -- p0:ycbcr877_22b
	IN_P0_444_22B_OUT16B_P1_444_22B,						// ECI 29,     in(44b) : p0(444_22b) + out(16b) + p1(444_22b)  -- p0/1:ycbcr877_22b
	IN_P0_444_22B_GPA_444_2B_OUT20B_P1_444_22B_GPA_444_2B,	// ECI 30,     in(48b) : p0(444_22b) + gpio-a(444_2b) + out(16b) + p1(444_22b) + gpio-a(444_2b)
	IN_P0_444_16B_GPA_444_8B_X6B,							// ECI 31,     in(30b) : p0(444_16b) + gpio-a(444_8b) + dummy(6b)
	
}	MISP_INPUT_CMOS_PINMAP_t;
#endif

typedef enum {
	MISP_MAINPATH_EXT_P0 = 0,			// main-src is ext_in, port 0
	MISP_MAINPATH_EXT_P1,				// main-src is ext_in, port 1
	MISP_MAINPATH_SENSOR,				// main-src is sensor, port 0
	MISP_MAINPATH_SENSOR_EXT_P1,		// main-src is sensor, port 1

}	MISP_SRCVIDEO_PATH_MAIN_t;

typedef enum {
	MISP_AUXPATH_EXT_P0 = 0,			// aux-src is ext_in, port 0
	MISP_AUXPATH_EXT_P1,				// aux-src is ext_in, port 1
	MISP_AUXPATH_SENSOR,				// aux-src is sensor, port 0
	MISP_AUXPATH_MAIN_O					// aux-src is main output

}	MISP_SRCVIDEO_PATH_AUX_t;

typedef enum {
	MISP_VENC_PATH_PORT_A = 0,			// VENC-source is port A
	MISP_VENC_PATH_PORT_B,				// VENC-source is port B
	MISP_VENC_PATH_PORT_X				// VENC-source is aux video

}	MISP_VENC_SRCPATH_t;		//venc_src_sel[0x01E, in_frame_cfg1]

typedef	struct
{
	WORD	y[8];			// coefficients of front NR filter for Y
	WORD	c[4];			// coefficients of front NR filter for C

}	stPACKED MISP_FNRFILT_COEF, *PMISP_FNRFILT_COEF;

typedef	struct
{
	WORD	coef[8];		// coefficients of horizontal peaking filter

}	stPACKED MISP_PEAKING_COEF, *PMISP_PEAKING_COEF;

typedef	struct
{
	PMISP_PEAKING_COEF 	pCoef;		// coefficients of horizontal peaking filter
	BYTE				level;		// output gain of hpf, 0~64(100%)
	BOOL				cor_en;		// filter coring_en on/off
	BYTE				cor_value;	// filter coring_en value
	BOOL				sat_en;		// filter saturation on/off
	BYTE				sat_value;	// filter saturation value
	BOOL				en;			// filter on/off
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	BOOL				cor_half_en;
	BOOL				cor_edge_dis;
	BOOL				level_depend_en;
	BYTE				level_depend_th;
	WORD				level_depend_slope;
	BYTE				level_depend_offset;
#endif

}	stPACKED MISP_HPF_INFO, *PMISP_HPF_INFO;

typedef	struct
{
	WORD	Htot;		// horizontal active size of video
	WORD	attb;		// H/V sync polarity, scan type, etc
	WORD	Hact;		// horizontal active size of video
	WORD	Vact;		// verical active size of video
	WORD	rate;		// frame per second of video

}	stPACKED MISP_SRCVIDEO_ATTB, *PMISP_SRCVIDEO_ATTB;

typedef	struct
{
	WORD	offH;		// pixel offset between HACT and video active
	WORD	offV;		// line offset between VACT and video active
	BYTE	yc_offset;	// video YC offset [0:off] [1:C earlier 1px] [2:C earlier 2px] [3:C earlier 3px] [4:Y earlier 1px] [5:Y earlier 2px] [6:Y earlier 3px]
	BOOL	cbcr_swap;	// video CbCr-swap
	BOOL	tf_polar;	// top-field polarity
	BYTE	dummy;

}	stPACKED MISP_SRCVIDEO_FINE, *PMISP_SRCVIDEO_FINE;

#if defined(__ENABLE_INPUT_CSC__)
typedef	struct
{
	BYTE	brightness;		// default = 128, range: from 0 to 255
	BYTE	contrast;		// default = 128, range: from 0 to 255
	BYTE	saturation;		// default = 128, range: from 0 to 255
	BYTE	hue;			// default = 128, range: from 0 to 255

}	stPACKED MISP_SRCVIDEO_TUNE, *PMISP_SRCVIDEO_TUNE;
#endif

typedef	struct
{
	BYTE	mode;		// frame buffer config mode(custom mode : 3(main), 1(aux))
	BYTE	Y_cnt;		// Y frame count on custom mode
	BYTE	C_cnt;		// C frame count on custom mode
	BYTE	Y_dly;		// Y frame delay on custom mode
	BYTE	C_dly;		// C frame delay on custom mode
	
}	MISP_FRCONFIG_INFO, *PMISP_FRCONFIG_INFO;

typedef	struct
{
	BYTE	enab;		// enable main front NR
	BYTE	coef;		// FNR coefficient
	WORD	dummy;
	
}	MISP_FRONT_NR_INFO, *PMISP_FRONT_NR_INFO;

typedef	struct
{
	BOOL	extn_en;		// enable Dig_io_ext input (i51x)
	BOOL	lvds_en;		// enable LVDS input (i51x)
	BYTE	mode_sel;		// pin mapping of input port0/1 (i500:MISP_INPUT_PINMAP_t, i51x:MISP_INPUT_LVDS/CMOS_PINMAP_t)
	BYTE	dummy;			// dummy byte for 32-bit alignment
	
}	MISP_INP_MODE_INFO, *PMISP_INP_MODE_INFO;

typedef	struct
{
	BYTE	frmt;			// video format of input (one of MISP_SRCVIDEO_FORMAT_t)
	BYTE	mode;			// video mode of input port (one of MISP_SRCPORT_MODE_t)

	MISP_SRCVIDEO_ATTB	stATTB;		// attribute of input video
	MISP_SRCVIDEO_FINE	stFINE;		// fine controls not in stATTB
#if defined(__ENABLE_INPUT_CSC__)
	MISP_SRCVIDEO_TUNE	stTUNE;		// CSC tune of input video
#endif
#if CSC_RANGE_CTRL
	PMISP_CSCCTRL_INFO	pCSC;		// input CSC control
#endif
}	MISP_SRCVIDEO_INFO, *PMISP_SRCVIDEO_INFO;

typedef	struct
{
	LONG	ctrl;					// input control flag
	
	WORD	MainPATH;				// path of main input video (one of MISP_SRCVIDEO_PATH_MAIN_t)
	WORD 	AuxPATH;				// path of aux input video ( one of MISP_SRCVIDEO_PATH_AUX_t)
	WORD	EncPATH;				// path of enc input video ( one of MISP_VENC_SRCPATH_t)
	WORD	dummy;					// dummy word for 32-bit alignment

	MISP_INP_MODE_INFO	stPinMap;		// input pin mode
	MISP_FRONT_NR_INFO	stFNR_m;		// main front NR

	MISP_FRCONFIG_INFO	FRcfg_m;		// frame count for main
	MISP_FRCONFIG_INFO	FRcfg_x;		// frame count for aux
	
	MISP_SRCVIDEO_INFO	stSRC_extp0;	// source video info of port 0
	MISP_SRCVIDEO_INFO	stSRC_extp1;	// source video info of port 1
	MISP_SRCVIDEO_INFO	stSRC_sensor;	// source video info of sensor
	MISP_SRCVIDEO_INFO	stSRC_x;		// source video info of aux-path
	MISP_SRCVIDEO_INFO	stSRC_m;		// source video info of main path, but only use in API function
	
	MISP_HPF_INFO		stHPF_m;		// h-peaking filter info of main
	
}	IN_CTRL_PARAM, *PIN_CTRL_PARAM;

// input control parameter
#define		INCTRL_TRIGGER_ISP			0x40000000		// ISP update
#define		INCTRL_UPDATE_PINMAP		0x00000001		// input pin map
#define		INCTRL_UPDATE_MAIN			0x00000002		// Input main source update
#define		INCTRL_UPDATE_AUX			0x00000004		// Input aux source update
#define		INCTRL_UPDATE_ENC			0x00000008		// Input enc source update
#define		INCTRL_UPDATE_MAIN_CSC		0x00000010		// input main csc update
#define		INCTRL_UPDATE_AUX_CSC		0x00000020		// input aux csc update
#define		INCTRL_UPDATE_MAIN_FRAME	0x00000040		// input main frame buffer count update
#define		INCTRL_UPDATE_AUX_FRAME		0x00000080		// input aux frame buffer count update
#define		INCTRL_UPDATE_MAIN_HPF		0x00000100		// input main h-peaking filter update
#define 	INCTRL_UPDATE_MAIN_NR		0x00000200		// input main front NR update
//#define		INCTRL_UPDATE_ALL			0x0000007F		// Input all(main,aux,enc) source update

#define		INCTRL_MAIN_CSC_GRAY_EN		0x00010000		// input main csc gray mode : y(0), cb/cr(x)
#define		INCTRL_AUX_CSC_GRAY_EN		0x00020000		// input aux csc gray mode : y(0), cb/cr(x)
//#define		INCTRL_MAIN_CROP_EN			0x00040000		// input main source video crop enable
//#define		INCTRL_AUX_CROP_EN			0x00080000		// input aux source video crop enable

// user define for attribute of source & output video
#define		MISP_NEGATIVE_HACT			0x0001	// high level contains active data region
#define		MISP_POSITIVE_HACT			0x0000	// low level contains active data region
#define		MISP_NEGATIVE_VACT			0x0002	// high level contains active data region
#define		MISP_POSITIVE_VACT			0x0000	// low level contains active data region
#define		MISP_POSITIVE_HSYNC			0x0004	// polarity of Hsync is rising edge
#define		MISP_NEGATIVE_HSYNC			0x0000	// polarity of Hsync is falling edge
#define		MISP_POSITIVE_VSYNC			0x0008	// polarity of Vsync is rising edge
#define		MISP_NEGATIVE_VSYNC			0x0000	// polarity of Vsync is falling edge
#define		MISP_SCANTYPE_PROG			0x0010	// progressive scan
#define		MISP_SCANTYPE_INTR			0x0000	// interlaced scan

// define for attribute of source & output video, but only use in API function
//#define		MISP_WIDE_RATIO				0x0020	// aspect ratio is 16:9
//#define		MISP_NORM_RATIO				0x0000	// aspect ratio is 4:3
//#define		MISP_QUALITY_HD				0x0100	// HD quality
//#define		MISP_QUALITY_SD				0x0000	// SD quality
//#define		MISP_PRECISION_8			0x0200	// 8-bit precision per color component
//#define		MISP_PRECISION_10			0x0000	// 10-bit precision per color component
//#define		MISP_PIXELSIZE_422			0x0400	// pixel-size is 4:2:2 format
//#define		MISP_PIXELSIZE_444			0x0000	// pixel-size is 4:4:4 format
//#define		MISP_COLORSPACE_YUV			0x0800	// color space is YCbCr domain
//#define		MISP_COLORSPACE_RGB			0x0000	// color space is RGB domain
//#define		MISP_VIDEO_SYSTEM			0x1000	// video system
//#define		MISP_PCVGA_SYSTEM			0x0000	// PC-VGA system
//#define		MISP_USE_INPORT_A			0x2000	// input port A is selected for input video
//#define		MISP_USE_INPORT_B			0x0000	// input port B is selected for input video
//#define		MISP_USE_INPORT_M			0x4000	// main video is selected for input video

// user define for fine control of source video
//#define		MISP_CbCrSWAP_ON			0x0010	// swap the ordering of chominance data
//#define		MISP_CbCrSWAP_OFF			0x0000	// swap the ordering of chominance data
//#define		MISP_FIELDID_INPUT			0x0020	// FIELDID input is used as a field-id signal
//#define		MISP_FIELDID_BYPASS			0x0000	// FIELDID is generated from H/V active signal
//#define		MISP_HIGH_IS_TOPFLD			0x0040	// high level of field-id represents top field
//#define		MISP_LOW_IS_TOPFLD			0x0000	// low level of field-id represents top field
//#define		MISP_HACT_IS_CSYNC			0x0008	// HACTIVE_A or B input is composite sync
//#define		MISP_HACT_IS_HSYNC			0x0000	// HACTIVE_A or B input is Hsync
//#define		MISP_YCOFFSET_MASK			0x0007	// masking of YC offset value

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------
extern ROMDATA MISP_SRCVIDEO_ATTB defMISPSrcVideo[];
extern ROMDATA WORD defDVIRXOffset[][2];


// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void Init_IN(void);
void IN_ProcessHandler(void);

void SetIN_MainSrcExtPort0(void);
void SetIN_MainSrcExtPort1(void);
void SetIN_MainSrcSensor(void);
void SetIN_MainFrmt(BYTE frmt);
BYTE GetIN_MainFrmt(void);
void SetIN_MainMode(BYTE mode);
void SetIN_MainFRconfig(WORD val);
void SetIN_MainFRcntY(WORD val);
void SetIN_MainFRcntC(WORD val);
void SetIN_MainFRdlyY(WORD val);
void SetIN_MainFRdlyC(WORD val);
void SetIN_MainHoriPeaking(BOOL OnOff);
void SetIN_MainHPFLevel(WORD val);
void SetIN_MainHPFCoeff(WORD val);
BYTE GetIN_MainHPFLevel(void);
BOOL GetIN_MainHoriPeaking(void);
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
void SetIN_MainHPFLDthresh(WORD val);
void SetIN_MainHPFLDslope(WORD val);
#endif

void SetIN_ExtPort0Frmt(WORD frmt);
void SetIN_ExtPort0Mode(WORD mode);

void SetIN_ExtPort1Frmt(WORD frmt);
void SetIN_ExtPort1Mode(WORD mode);

void SetIN_MainSrcSizeH(WORD size);
void SetIN_MainSrcSizeV(WORD size);
WORD GetIN_MainSrcSizeH(void);
WORD GetIN_MainSrcSizeV(void);
WORD GetIN_MainSrcSizeHtot(void);
BOOL GetIN_MainScanTypeProg(void);
BYTE GetIN_MainPath(void);
BYTE GetIN_AuxPath(void);
BYTE GetIN_MainMode(void);
BYTE GetIN_AuxMode(void);
BOOL GetIN_MainBitmode(void);
BOOL GetIN_AuxBitmode(void);
WORD GETIN_MainFRcntY(void);
WORD GETIN_MainFRcntC(void);

void SetIN_AuxSrcExtPort0(void);
void SetIN_AuxSrcExtPort1(void);
void SetIN_AuxSrcSensor(void);
void SetIN_AuxSrcMainOut(void);
void SetIN_AuxFrmt(BYTE frmt);
BYTE GetIN_AuxFrmt(void);
void SetIN_AuxMode(BYTE mode);
void SetIN_AuxFRconfig(WORD val);
void SetIN_AuxFRcnt(WORD val);
void SetIN_AuxFRdly(WORD val);
void SetIN_AuxSrcSizeH(WORD size);
void SetIN_AuxSrcSizeV(WORD size);
WORD GetIN_AuxSrcSizeH(void);
WORD GetIN_AuxSrcSizeV(void);
WORD GetIN_AuxFRcnt(void);
void SetIN_EncPATH(WORD val);
BOOL GetIN_AuxScanTypeProg(void);

void SetIn_MainOffHA(WORD val);
void SetIn_MainOffVA(WORD val);
void SetIn_AuxOffHA(WORD val);
void SetIn_AuxOffVA(WORD val);
WORD GetIn_MainOffHA(void);
WORD GetIn_MainOffVA(void);
WORD GetIn_AuxOffHA(void);
WORD GetIn_AuxOffVA(void);

void SetIn_PinMapExtn(BOOL OnOff);
BOOL GetIn_PinMapExtn(void);
void SetIn_PinMapLvds(BOOL OnOff);
BOOL GetIn_PinMapLvds(void);
void SetIn_PinMapMode(BYTE mode);
BYTE GetIn_PinMapMode(void);

void SetIn_MainSwapCbCr(BOOL OnOff);
void SetIn_MainYcOffset(BYTE val);
void SetIn_MainTfPolar(BOOL OnOff);
void SetIn_MainHsPolar(BOOL OnOff);
void SetIn_MainVsPolar(BOOL OnOff);
void SetIn_AuxSwapCbCr(BOOL OnOff);
void SetIn_AuxYcOffset(BYTE val);
void SetIn_AuxTfPolar(BOOL OnOff);
void SetIn_AuxHsPolar(BOOL OnOff);
void SetIn_AuxVsPolar(BOOL OnOff);
BOOL GetIn_MainSwapCbCr(void);
BYTE GetIn_MainYcOffset(void);
BOOL GetIn_MainTfPolar(void);
BOOL GetIn_MainHsPolar(void);
BOOL GetIn_MainVsPolar(void);
BOOL GetIn_AuxSwapCbCr(void);
BYTE GetIn_AuxYcOffset(void);
BOOL GetIn_AuxTfPolar(void);
BOOL GetIn_AuxHsPolar(void);
BOOL GetIn_AuxVsPolar(void);

void SetIn_MainFnrEn(BOOL OnOff);
void SetIn_MainFnrCoef(BYTE nID);
BOOL GetIn_MainFnrEn(void);
BYTE GetIn_MainFnrCoef(void);

#if defined(__ENABLE_INPUT_CSC__)
void SetIn_MainContrast(BYTE val);
void SetIn_MainBrightness(BYTE val);
void SetIn_MainSaturation(BYTE val);
void SetIn_MainHue(BYTE val);
void SetIn_AuxContrast(BYTE val);
void SetIn_AuxBrightness(BYTE val);
void SetIn_AuxSaturation(BYTE val);
void SetIn_AuxHue(BYTE val);
BYTE GetIn_MainContrast(void);
BYTE GetIn_MainBrightness(void);
BYTE GetIn_MainSaturation(void);
BYTE GetIn_MainHue(void);
BYTE GetIn_AuxContrast(void);
BYTE GetIn_AuxBrightness(void);
BYTE GetIn_AuxSaturation(void);
BYTE GetIn_AuxHue(void);
#endif

void SetIN_MainCSCgray(BOOL nID);
void SetIN_MainCSC(BYTE nID);
void SetIN_AuxCSCgray(BOOL nID);

void SetIN_SensorFrmt(WORD val);
void SetIN_ExtFrmt(WORD val);
void SetIN_MainSrcExt(void);
void SetIN_AuxSrcExt(void);

void SetIN_Ext2IPP(BOOL en);
void SetIN_MainNoFCMode(BOOL en) ;
void SetIN_MainNoDelayMode(BYTE en);

//IO test functions 
WORD SetIN_ExtPort0ModeRoll(void) ;
WORD SetIN_ExtPort1ModeRoll(void) ;

#endif	/* __MISPINPUT_H__ */

