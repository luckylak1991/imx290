//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MISPOUTPUT.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISPOUTPUT_H__
#define		__MISPOUTPUT_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

typedef	enum {
	// TV
	MISP_VIDOUT_720x480i60 = 0,		// 720x480i 60Hz
	MISP_VIDOUT_720x576i50,			// 720x576i 50Hz
	MISP_VIDOUT_960x480i60,			// 720x480i 60Hz
	MISP_VIDOUT_960x576i50,			// 960x576i 50Hz
	
	MISP_VIDOUT_720x480p60,			// 720x480p 60Hz
	MISP_VIDOUT_720x576p50,			// 720x576p 50Hz
	MISP_VIDOUT_960x480p60,			// 960x480p 60Hz
	MISP_VIDOUT_960x576p50,			// 960x576p 50Hz
	
	MISP_VIDOUT_1280x720p60,		// 1280x720p 60Hz
#if defined(__USE_59Hz_OUTPUT__)
	MISP_VIDOUT_1280x720p59,		// 1280x720p 59.94Hz
#endif
	MISP_VIDOUT_1280x720p50,		// 1280x720p 50Hz
	
	MISP_VIDOUT_1280x720p30,		// 1280x720p 30Hz	// not work in gv7600(SEP422)
	MISP_VIDOUT_1280x720p25,		// 1280x720p 25Hz	// not work in gv7600(SEP422)
	MISP_VIDOUT_1280x720p24,		// 1280x720p 24Hz	// not work in gv7600(SEP422)
	
	MISP_VIDOUT_1920x1080i60,		// 1920x1080i 60Hz
#if defined(__USE_59Hz_OUTPUT__)
	MISP_VIDOUT_1920x1080i59,		// 1920x1080i 59.94Hz
#endif
	MISP_VIDOUT_1920x1080i50,		// 1920x1080i 50Hz
	
	MISP_VIDOUT_1920x1080p60,		// 1920x1080p 60Hz
#if defined(__USE_59Hz_OUTPUT__)
	MISP_VIDOUT_1920x1080p59,		// 1920x1080p 59.94Hz
#endif
	MISP_VIDOUT_1920x1080p50,		// 1920x1080p 50Hz

	MISP_VIDOUT_1920x1080p30,		// 1920x1080p 30Hz
#if defined(__USE_59Hz_OUTPUT__)
	MISP_VIDOUT_1920x1080p29,		// 1920x1080p 29.97Hz
#endif
	MISP_VIDOUT_1920x1080p25,		// 1920x1080p 25Hz
	MISP_VIDOUT_1920x1080p24,		// 1920x1080p 24Hz
	
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	MISP_VIDOUT_3840x2160p30,		// 3840x2160p 30Hz
#if defined(__USE_59Hz_OUTPUT__)
	MISP_VIDOUT_3840x2160p29,		// 3840x2160p 29.97Hz
#endif
	MISP_VIDOUT_3840x2160p25,		// 3840x2160p 25Hz
	MISP_VIDOUT_3840x2160p24,		// 3840x2160p 24Hz
	MISP_VIDOUT_3840x2160p15,		// 3840x2160p 15Hz
	MISP_VIDOUT_3840x2160p12,		// 3840x2160p 12.5Hz
	MISP_VIDOUT_4096x2160p30,		// 4096x2160p 30Hz
#if defined(__USE_59Hz_OUTPUT__)
	MISP_VIDOUT_4096x2160p29,		// 4096x2160p 29.97Hz
#endif
	MISP_VIDOUT_4096x2160p25,		// 4096x2160p 25Hz
	MISP_VIDOUT_4096x2160p24,		// 4096x2160p 24Hz
	MISP_VIDOUT_2560x1440p30,		// 2560x1440p 30Hz (4M)
	MISP_VIDOUT_2560x1440p25,		// 2560x1440p 25Hz (4M)
#if defined(__USE_4K60_INOUT__)
	MISP_VIDOUT_1920x2160p60,		// (3840/2)x2160p 60Hz
#if defined(__USE_59Hz_OUTPUT__)
	MISP_VIDOUT_1920x2160p59,		// (3840/2)x2160p 59Hz
#endif
	MISP_VIDOUT_1920x2160p50,		// (3840/2)x2160p 50Hz
	MISP_VIDOUT_2048x2160p60,		// (4096/2)x2160p 60Hz
#if defined(__USE_59Hz_OUTPUT__)
	MISP_VIDOUT_2048x2160p59,		// (4096/2)x2160p 59Hz
#endif
	MISP_VIDOUT_2048x2160p50,		// (4096/2)x2160p 50Hz
#endif
#endif	

	
	// 120hz format
	MISP_VIDOUT_1280x720p120,		// HD 120fps
	MISP_VIDOUT_1280x720p100,		// HD 100fps
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	MISP_VIDOUT_1920x1080p120,		// FHD 120fps
	MISP_VIDOUT_1920x1080p100,		// FHD 100fps
#endif
	
	
	// PC
	MISP_VIDOUT_640x480p60,		// 640x480p 60Hz	//vga
	MISP_VIDOUT_640x480p75,		// 640x480p 75Hz
	MISP_VIDOUT_800x600p60,		// 800x600p 60Hz	//svga
	MISP_VIDOUT_800x600p75,		// 800x600p 75Hz
	MISP_VIDOUT_1024x768p60,		// 1024x768p 60Hz	//xga
	MISP_VIDOUT_1024x768p75,		// 1024x768p 75Hz
	MISP_VIDOUT_1280x960p60,		// 1280x960p 60Hz
	MISP_VIDOUT_1280x1024p60,		// 1280x1024p 60Hz	 //sxga
	MISP_VIDOUT_1280x1024p75,		// 1280x1024p 75Hz
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	MISP_VIDOUT_1280x1024p100,		// 1280x1024p 100Hz
#endif

	MISP_VIDOUT_1360x768p60,			// 1360x768p 60Hz
	MISP_VIDOUT_1366x768p60,			// 1366x768p 60Hz
	MISP_VIDOUT_1400x1050p60,			// 1400x1050p 60Hz
	MISP_VIDOUT_1600x1200p60,		// 1600x1200p 60Hz

	// ETC
	MISP_VIDOUT_1440x900p60,			// 1440x900p 60Hz
	MISP_VIDOUT_1440x900p75,			// 1440x900p 75Hz
	MISP_VIDOUT_1680x1050p60,		// 1680x1050p 60Hz

	MISP_VIDOUT_1680x1050pRB,		// 1680x1050pRB 60Hz
	MISP_VIDOUT_1920x1080pRB,		// 1920x1080pRB 60Hz
	MISP_VIDOUT_1920x1200pRB,		// 1920x1200pRB 60Hz
	
	// SENSOR(for sensor in/out 1:1 mode)
	MISP_VIDOUT_2048x1536p30,		// 2048x1536p 30Hz -- imx124(3M)
	MISP_VIDOUT_2048x1536p25,		// 2048x1536p 25Hz -- imx124(3M)
	MISP_VIDOUT_3072x1728p15,		// 3072x1728p 15Hz -- imx178(5M 16:9), imx326(5M)
	MISP_VIDOUT_2592x1944p15,		// 2592x1944p 15Hz -- imx326(5M)

	// DIS(aux only)
	MISP_VIDOUT_DIS2M,				// main out:2M,  aux out:2M(pip 1/16))
	MISP_VIDOUT_DIS3M,				// main out:3M,  aux out:3M(pip 1/16))
	MISP_VIDOUT_DIS8M,				// main out:8M,  aux out:3M(pip 1/16))
	MISP_VIDOUT_DIS2M_PANNING,		// main out:2048x1208,  aux out:2048x1208(pip 1/16))	

	// USB
//#if defined(__USE_USB_VOUT__)
	//MISP_VIDOUT_720x480p60,		// already defined..
	MISP_VIDOUT_720x480p50,
	MISP_VIDOUT_720x480p30,
	MISP_VIDOUT_720x480p24,
	//MISP_VIDOUT_960x480p60,		// already defined..
	MISP_VIDOUT_960x480p50,
	MISP_VIDOUT_960x480p30,
	MISP_VIDOUT_960x480p24,
	//MISP_VIDOUT_1280x720p60,		// already defined..
	//MISP_VIDOUT_1280x720p50,		// already defined..
	//MISP_VIDOUT_1280x720p30,		// already defined..
	//MISP_VIDOUT_1280x720p24,		// already defined..
	MISP_VIDOUT_1280x720p20,
	MISP_VIDOUT_1280x720p15,
	MISP_VIDOUT_1280x720p10,
	MISP_VIDOUT_1280x720p7d5,
	MISP_VIDOUT_1280x720p05,
	//MISP_VIDOUT_1920x1080p60,		// already defined..
	//MISP_VIDOUT_1920x1080p50,		// already defined..
	//MISP_VIDOUT_1920x1080p30,		// already defined..
	//MISP_VIDOUT_1920x1080p24,		// already defined..
	MISP_VIDOUT_1920x1080p20,
	MISP_VIDOUT_1920x1080p15,
	MISP_VIDOUT_1920x1080p10,
	MISP_VIDOUT_1920x1080p7d5,
	MISP_VIDOUT_1920x1080p05,
	//MISP_VIDOUT_3840x2160p15,		// already defined..
	MISP_VIDOUT_3840x2160p10,
	MISP_VIDOUT_3840x2160p7d5,
	MISP_VIDOUT_3840x2160p05,
//#endif	// __USE_USB_VOUT__
	
	MISP_VIDOUT_FORMAT_END			// output format end
	
}	MISP_OUTVIDEO_FORMAT_t;

typedef	enum {
	MISP_OUT_RGB444_8 = 0,		// RGB 4:4:4 8-bit with separate sync on digital out port
	MISP_OUT_RGB444_10,			// RGB 4:4:4 10-bit with separate sync on digital out port
	MISP_OUT_YUV444_8,			// YCbCr 4:4:4 8-bit with separate sync on digital out port
	MISP_OUT_YUV444_10,			// YCbCr 4:4:4 10-bit with separate sync on digital out port
	MISP_OUT_EMB422_8,			// YCbCr 4:2:2 8-bit with embedded sync on digital out port
	MISP_OUT_EMB422_10,			// YCbCr 4:2:2 10-bit with embedded sync on digital out port
	MISP_OUT_SEP422_8,			// YCbCr 4:2:2 8-bit with separate sync on digital out port
	MISP_OUT_SEP422_10,			// YCbCr 4:2:2 10-bit with separate sync on digital out port
	MISP_OUT_MUX656_8,			// Y/C-mux 4:2:2 8-bit with embedded sync on digital out port
	MISP_OUT_MUX656_10			// Y/C-mux 4:2:2 10-bit with embedded sync on digital out port

}	MISP_OUTPORT_MODE_t;


#if defined(__USE_MDIN_i500__)
typedef	enum {
	OUT_MAIN_422_20B = 0,						// out(20b) : main(422_20b)
	OUT_MAIN_444_30B,							// out(30b) : main(444_30b)
	OUT_MAIN_656_10B,							// out(10b) : main(656_10b)
	OUT_MAIN_656_10B_AUX_656_10B,				// out(20b) : main(656_10b) + aux(656_10b)
	OUT_MAIN_422_20B_AUX_656_10B,				// out(30b) : main(422_20b) + aux(656_10b)
	OUT_MAIN_422_20B_AUX_422_20B,				// out(40b) : main(422_20b) + aux(422_20b)
	OUT_MAIN_444_30B_AUX_656_10B,				// out(40b) : main(444_30b) + aux(656_10b)
	
}	MISP_OUTPUT_PINMAP_t;
#endif


#if defined(__USE_MDIN_i510__)
// cmos output mode
typedef	enum {
	OUT_MAIN_656_10B = 0,						// NCO  0,     out(10b) : main(656_10b)
	OUT_MAIN_656_10B_AUX_656_8B = 1,			// NCO  1,     out(18b) : main(656_10b) + aux(656_8b)
	OUT_MAIN_656_10B_AUX_656_10B = 1,			// NCO  1(AT), out(20b) : main(656_10b) + aux(656_10b)
	OUT_MAIN_422_20B = 3,						// NCO  3,     out(20b) : main(422_20b)
	OUT_MAIN_422_16B = 6,						// NCO  6,     out(16b) : main(422_16b)
	OUT_MAIN_422DE_10B = 11,					// NCO 11,     out(10b) : main(422de_10b)
	OUT_MAIN_422DE_10B_AUX_422DE_8B = 12,		// NCO 12,     out(18b) : main(422de_10b) + aux(422de_8b)
	OUT_MAIN_422DE_10B_AUX_422DE_10B = 12,		// NCO 12(AT), out(20b) : main(422de_10b) + aux(422de_10b)
	OUT_MAIN_444DE_12B,							// NCO 13,     out(12b) : main(444de_12b)
	OUT_MAIN_444DE_12B_AUX_422DE_8B = 14,		// NCO 14,     out(20b) : main(444de_12b) + aux(422de_8b)
	OUT_MAIN_444DE_15B,							// NCO 15,     out(15b) : main(444de_15b)
	
}	MISP_OUTPUT_CMOS_PINMAP_t;

// lvds output mode
typedef	enum {
	OUT_LVDS_10B = 0,							// NLO  0,     out(10b) : lvds(10b)
	OUT_LVDS_10B_MAIN_656_10B,					// NLO  1,     out(20b) : lvds(10b) + main(656_10b)
	OUT_LVDS_10B_AUX_656_8B = 3,				// NLO  3,     out(18b) : lvds(10b) + aux(656_8b)
	OUT_LVDS_10B_AUX_656_10B = 3,				// NLO  3(AT), out(20b) : lvds(10b) + aux(656_10b)
	OUT_LVDS_DOUBLE_20B = 5,					// NLO  5,     out(20b) : lvds(10b) + lvds(10b)
	
}	MISP_OUTPUT_LVDS_PINMAP_t;
#endif


#if defined(__USE_MDIN_i540__)
// cmos output mode
typedef	enum {
	OUT_MAIN_656_10B = 0,						// NCO  0,     out(10b) : main(656_10b)
	OUT_MAIN_656_10B_AUX_656_8B = 1,			// NCO  1,     out(18b) : main(656_10b) + aux(656_8b)
	OUT_MAIN_656_10B_AUX_656_10B = 1,			// NCO  1(AT), out(20b) : main(656_10b) + aux(656_10b)
	OUT_MAIN_656_10B_AUX_422_16B = 2,			// NCO  2,     out(26b) : main(656_10b) + aux(422_16b)
	OUT_MAIN_656_10B_AUX_422_20B = 2,			// NCO  2(AT), out(30b) : main(656_10b) + aux(422_20b)
	OUT_MAIN_422_20B,							// NCO  3,     out(20b) : main(422_20b)
	OUT_MAIN_422_20B_AUX_656_8B = 4,			// NCO  4,     out(28b) : main(422_20b) + aux(656_8b)
	OUT_MAIN_422_20B_AUX_656_10B = 4,			// NCO  4(AT), out(30b) : main(422_20b) + aux(656_10b)
	OUT_MAIN_444_30B,							// NCO  5,     out(30b) : main(444_30b)
	OUT_MAIN_422_16B,							// NCO  6,     out(16b) : main(422_16b)
	OUT_MAIN_422_16B_AUX_656_8B = 7,			// NCO  7,     out(24b) : main(422_16b) + aux(656_8b)
	OUT_MAIN_422_16B_AUX_656_10B = 7,			// NCO  7(AT), out(26b) : main(422_16b) + aux(656_10b)
	OUT_MAIN_444_24B = 9,						// NCO  9,     out(24b) : main(444_24b)
	OUT_MAIN_422DE_10B = 11,					// NCO 11,     out(10b) : main(422de_10b)
	OUT_MAIN_422DE_10B_AUX_422DE_8B = 12,		// NCO 12,     out(18b) : main(422de_10b) + aux(422de_8b)
	OUT_MAIN_422DE_10B_AUX_422DE_10B = 12,		// NCO 12(AT), out(20b) : main(422de_10b) + aux(422de_10b)
	OUT_MAIN_444DE_12B,							// NCO 13,     out(12b) : main(444de_12b)
	OUT_MAIN_444DE_12B_AUX_422DE_8B = 14,		// NCO 14,     out(20b) : main(444de_12b) + aux(422de_8b)
	OUT_MAIN_444DE_12B_AUX_422DE_10B = 14,		// NCO 14(AT), out(22b) : main(444de_12b) + aux(422de_10b)
	OUT_MAIN_444DE_15B,							// NCO 15,     out(15b) : main(444de_15b)
	OUT_MAIN_444DE_15B_AUX_422DE_8B = 16,		// NCO 16,     out(23b) : main(444de_15b) + aux(422de_8b)
	OUT_MAIN_444DE_15B_AUX_422DE_10B = 16,		// NCO 16(AT), out(25b) : main(444de_15b) + aux(422de_10b)
//	OUT_MAIN_444DW_32B_GPA_444DW_8B,			// NCO 17,     out(40b) : main(444dw_32b) + gpio-a(444dw_8b)
	
}	MISP_OUTPUT_CMOS_PINMAP_t;

// lvds output mode
typedef	enum {
	OUT_LVDS_10B = 0,							// NLO  0,     out(10b) : lvds(10b)
	OUT_LVDS_10B_MAIN_656_10B,					// NLO  1,     out(20b) : lvds(10b) + main(656_10b)
	OUT_LVDS_10B_MAIN_422_20B,					// NLO  2,     out(30b) : lvds(10b) + main(422_20b)
	OUT_LVDS_10B_AUX_656_8B = 3,				// NLO  3,     out(18b) : lvds(10b) + aux(656_8b)
	OUT_LVDS_10B_AUX_656_10B = 3,				// NLO  3(AT), out(20b) : lvds(10b) + aux(656_10b)
	OUT_LVDS_10B_AUX_422_16B = 4,				// NLO  4,     out(26b) : lvds(10b) + aux(422_16b)
	OUT_LVDS_10B_AUX_422_20B = 4,				// NLO  4(AT), out(30b) : lvds(10b) + aux(422_20b)
	OUT_LVDS_DOUBLE_20B,						// NLO  5,     out(20b) : lvds(10b) + lvds(10b)
	OUT_LVDS_DOUBLE_20B_MAIN_656_10B,			// NLO  6,     out(30b) : lvds(10b) + lvds(10b) + main(656_10b)
	OUT_LVDS_DOUBLE_20B_AUX_656_8B = 7,			// NLO  7,     out(28b) : lvds(10b) + lvds(10b) + aux(656_8b)
	OUT_LVDS_DOUBLE_20B_AUX_656_10B = 7,		// NLO  7(AT), out(30b) : lvds(10b) + lvds(10b) + aux(656_10b)
	
}	MISP_OUTPUT_LVDS_PINMAP_t;
#endif


#if defined(__USE_MDIN_i550__)
// cmos output mode
typedef	enum {
	OUT_MAIN_656_10B = 0,						// ECO  0,     out(10b) : main(656_10b)
	OUT_MAIN_656_10B_AUX_656_8B = 1,			// ECO  1,     out(18b) : main(656_10b) + aux(656_8b)
	OUT_MAIN_656_10B_AUX_656_10B = 1,			// ECO  1(AT), out(20b) : main(656_10b) + aux(656_10b)
	OUT_MAIN_656_10B_AUX_422_16B = 2,			// ECO  2,     out(26b) : main(656_10b) + aux(422_16b)
	OUT_MAIN_656_10B_AUX_422_20B = 2,			// ECO  2(AT), out(30b) : main(656_10b) + aux(422_20b)
	OUT_MAIN_656_10B_AUX_444_24B = 3,			// ECO  3,     out(34b) : main(656_10b) + aux(444_24b)
	OUT_MAIN_656_10B_AUX_444_30B = 3,			// ECO  3(AT), out(40b) : main(656_10b) + aux(444_30b)
	OUT_MAIN_422_20B,							// ECO  4,     out(20b) : main(422_20b)
	OUT_MAIN_422_20B_AUX_656_8B = 5,			// ECO  5,     out(28b) : main(422_20b) + aux(656_8b)
	OUT_MAIN_422_20B_AUX_656_10B = 5,			// ECO  5(AT), out(30b) : main(422_20b) + aux(656_10b)
	OUT_MAIN_422_20B_AUX_422_16B = 6,			// ECO  6,     out(36b) : main(422_20b) + aux(422_16b)
	OUT_MAIN_422_20B_AUX_422_20B = 6,			// ECO  6(AT), out(40b) : main(422_20b) + aux(422_20b)
	OUT_MAIN_422_20B_AUX_444_24B = 7,			// ECO  7,     out(44b) : main(422_20b) + aux(422_24b)
	OUT_MAIN_422_20B_AUX_444_30B = 7,			// ECO  7(AT), out(50b) : main(422_20b) + aux(422_30b)
	OUT_MAIN_444_30B,							// ECO  8,     out(30b) : main(444_30b)
	OUT_MAIN_444_30B_AUX_656_8B = 9,			// ECO  9,     out(38b) : main(444_30b) + aux(656_8b)
	OUT_MAIN_444_30B_AUX_656_10B = 9,			// ECO  9(AT), out(40b) : main(444_30b) + aux(656_10b)
	OUT_MAIN_444_30B_AUX_422_16B = 10,			// ECO 10,     out(46b) : main(444_30b) + aux(422_16b)
	OUT_MAIN_444_30B_AUX_422_20B = 10,			// ECO 10(AT), out(50b) : main(444_30b) + aux(422_20b)
	OUT_MAIN_422_16B = 12,						// ECO 12,     out(16b) : main(422_16b)
	OUT_MAIN_422_16B_AUX_656_8B,				// ECO 13,     out(24b) : main(422_16b) + aux(656_8b)
	OUT_MAIN_422_16B_AUX_422_16B,				// ECO 14,     out(32b) : main(422_16b) + aux(422_16b)
	OUT_MAIN_422_16B_AUX_444_24B,				// ECO 15,     out(40b) : main(422_16b) + aux(444_24b)
	OUT_MAIN_444_24B,							// ECO 16,     out(24b) : main(444_24b)
	OUT_MAIN_444_24B_AUX_656_8B,				// ECO 17,     out(32b) : main(444_24b) + aux(656_8b)
	OUT_MAIN_444_24B_AUX_422_16B,				// ECO 18,     out(40b) : main(444_24b) + aux(422_16b)
	OUT_MAIN_444_24B_AUX_444_24B,				// ECO 19,     out(48b) : main(444_24b) + aux(444_24b)
	OUT_MAIN_444DW_40B,							// ECO 20,     out(40b) : main(444dw_40b)
	OUT_MAIN_444DW_40B_AUX_656_8B,				// ECO 21,     out(48b) : main(444dw_40b) + aux(656_8b)
	OUT_MAIN_444DW_32B,							// ECO 22,     out(32b) : main(444dw_32b)
	OUT_MAIN_444DW_32B_AUX_656_8B,				// ECO 23,     out(40b) : main(444dw_32b) + aux(656_8b)
	OUT_MAIN_444DW_32B_AUX_422_16B,				// ECO 24,     out(48b) : main(444dw_32b) + aux(422_16b)
	OUT_MAIN_444DW_48B,							// ECO 25,     out(48b) : main(444dw_48b)
	OUT_MAIN_422DE_10B,							// ECO 26,     out(10b) : main(422de_10b)
	OUT_MAIN_422DE_10B_AUX_422DE_8B = 27,		// ECO 27,     out(18b) : main(422de_10b) + aux(422de_8b)
	OUT_MAIN_422DE_10B_AUX_422DE_10B = 27,		// ECO 27(AT), out(20b) : main(422de_10b) + aux(422de_10b)
	OUT_MAIN_444DE_12B,							// ECO 28,     out(12b) : main(444de_12b)
	OUT_MAIN_444DE_12B_AUX_422DE_8B = 29,		// ECO 29,     out(20b) : main(444de_12b) + aux(422de_8b)
	OUT_MAIN_444DE_12B_AUX_422DE_10B = 29,		// ECO 29(AT), out(22b) : main(444de_12b) + aux(422de_10b)
	OUT_MAIN_444DE_15B,							// ECO 30,     out(15b) : main(444de_15b)
	OUT_MAIN_444DE_15B_AUX_422DE_8B = 31,		// ECO 31,     out(23b) : main(444de_15b) + aux(422de_8b)
	OUT_MAIN_444DE_15B_AUX_422DE_10B = 31,		// ECO 31(AT), out(25b) : main(444de_15b) + aux(422de_10b)
	
}	MISP_OUTPUT_CMOS_PINMAP_t;

// lvds output mode
typedef	enum {
	OUT_MAIN_656_10B_X10B_HS_VS_LVDS_DOUBLE_20B = 0,				// ELO  0,     out(42b) : main(656_10b) + dummy(10b) + h/vs + lvds(dual_20b)
	OUT_MAIN_656_10B_AUX_656_8B_X2B_HSX_VSX_LVDS_DOUBLE_20B = 1,	// ELO  1,     out(42b) : main(656_10b) + aux(656_8b) + dummy(2b) + h/vs + lvds(dual_20b)
	OUT_MAIN_656_10B_AUX_656_10B_HSX_VSX_LVDS_DOUBLE_20B = 1,		// ELO  1(AT), out(42b) : main(656_10b) + aux(656_10b) + h/vsx + lvds(dual_20b)
	OUT_MAIN_422_20B_HS_VS_LVDS_DOUBLE_20B,							// ELO  2,     out(42b) : main(422_20b) + h/vs + lvds(dual_20b)
	OUT_MAIN_422_20B_HSX_VSX_LVDS_DOUBLE_20B_AUX_656_8B = 3,		// ELO  3,     out(50b) : main(422_20b) + h/vsx + lvds(dual_20b) + aux(656_8b)
	OUT_MAIN_422_20B_HSX_VSX_LVDS_DOUBLE_20B_AUX_656_10B = 3,		// ELO  3(AT), out(52b) : main(422_20b) + h/vsx + lvds(dual_20b) + aux(656_10b)
	OUT_MAIN_422_20B_HSX_VSX_LVDS_10B_AUX_656_8B = 4,				// ELO  4,     out(40b) : main(422_20b) + h/vsx + lvds(10b) + aux(656_8b)
	OUT_MAIN_422_20B_HSX_VSX_LVDS_10B_AUX_656_10B = 4,				// ELO  4(AT), out(42b) : main(422_20b) + h/vsx + lvds(10b) + aux(656_10b)
	OUT_MAIN_422_20B_HSX_VSX_LVDS_10B_AUX_422_20B,					// ELO  5(AT), out(52b) : main(422_20b) + h/vsx + lvds(10b) + aux(422_20b)
	OUT_AUX_422_16B_X4B_HSX_VSX_LVDS_DOUBLE_20B = 6,				// ELO  6,     out(42b) : aux(422_16b) + dummy(4b) + h/vsx + lvds(double_20b)
	OUT_AUX_422_20B_HSX_VSX_LVDS_DOUBLE_20B = 6,					// ELO  6(AT), out(42b) : aux(422_20b) + h/vsx + lvds(double_20b)
	OUT_MAIN_422_16B_X4B_HS_VS_LVDS_DOUBLE_20B,						// ELO  7,     out(42b) : main(422_16b) + dummy(4b) + h/vs + lvds(double_20b)
	OUT_MAIN_444_22B_LVDS_DOUBLE_20B,								// ELO  8,     out(42b) : main(444_22b) + lvds(double_20b)  -- main:ycbcr877_22b
	OUT_MAIN_422DE_10B_X10B_HS_VS_LVDS_DOUBLE_20B,					// ELO  9,     out(42b) : main(422de_10b) + dummy(10b) + h/vs + lvds(double_20b)
	OUT_MAIN_422DE_10B_AUX_422DE_8B_X2B_HSX_VSX_LVDS_DOUBLE_20B = 10,	// ELO 10,     out(42b) : main(422de_10b) + aux(422de_8b) + dummy(2b) + h/vsx + lvds(double_20b)
	OUT_MAIN_422DE_10B_AUX_422DE_10B_HSX_VSX_LVDS_DOUBLE_20B = 10,		// ELO 10(AT), out(42b) : main(422de_10b) + aux(422de_10b) + h/vsx + lvds(double_20b)
	OUT_MAIN_444DE_12B_X8B_HS_VS_LVDS_DOUBLE_20B,						// ELO 11,     out(42b) : main(444de_12b) + dummy(8b) + h/vs + lvds(double_20b)
	OUT_MAIN_444DE_12B_AUX_422DE_8B_HSX_VSX_LVDS_DOUBLE_20B,			// ELO 12,     out(42b) : main(444de_12b) + aux(422de_8b) + h/vsx + lvds(double_20b)
	OUT_MAIN_444DE_15B_X5B_HS_VS_LVDS_DOUBLE_20B,						// ELO 13,     out(42b) : main(444de_15b) + dummy(5b) + h/vs + lvds(double_20b)
	OUT_LVDS_DOUBLE_20B,												// ELO 14,     out(20b) : lvds(double_20b)
	OUT_AUX_422_20B = 16,												// EHO 16,     out(20b) : aux(422_20b)
	OUT_AUX_422_20B_MAIN_656_8B = 17,									// EHO 17,     out(28b) : aux(422_20b) + main(656_8b)
	OUT_AUX_422_20B_MAIN_656_10B = 17,									// EHO 17(MT), out(30b) : aux(422_20b) + main(656_10b)
	OUT_AUX_422_20B_MAIN_422_16B = 18,									// EHO 18,     out(36b) : aux(422_20b) + main(422_16b)
	OUT_AUX_422_20B_MAIN_422_20B = 18,									// EHO 18(MT), out(40b) : aux(422_20b) + main(422_20b)
	OUT_AUX_422_20B_MAIN_444_24B = 19,									// EHO 19,     out(44b) : aux(422_20b) + main(444_24b)
	OUT_AUX_422_20B_MAIN_444_30B = 19,									// EHO 19(MT), out(50b) : aux(422_20b) + main(444_30b)
	OUT_AUX_444_30B,													// EHO 20,     out(30b) : aux(444_30b)
	OUT_AUX_444_30B_MAIN_656_8B = 21,									// EHO 21,     out(38b) : aux(444_30b) + main(656_8b)
	OUT_AUX_444_30B_MAIN_656_10B = 21,									// EHO 21(MT), out(40b) : aux(444_30b) + main(656_10b)
	OUT_AUX_444_30B_MAIN_422_16B = 22,									// EHO 22,     out(46b) : aux(444_30b) + main(422_16b)
	OUT_AUX_444_30B_MAIN_422_20B = 22,									// EHO 22(MT), out(50b) : aux(444_30b) + main(422_20b)
	
}	MISP_OUTPUT_LVDS_PINMAP_t;
#endif


typedef	struct
{
	WORD	coef[19];		// coefficients of video encoder

}	stPACKED MISP_ENCODER_COEF, *PMISP_ENCODER_COEF;

typedef	struct
{
	BYTE	src[4];		// input segment point for B/W extension
	BYTE	out[4];		// output segment point for B/W extension

}	stPACKED MISP_BWPOINT_COEF, *PMISP_BWPOINT_COEF;

typedef	struct
{
	WORD	posHD;		// the position of horizontal reference pulse
	WORD	posVD;		// the position of vertical reference pulse
	WORD	posVB;		// the position of vertical reference pulse for bottom field

	WORD	totHS;		// the horizontal total size of output video
	WORD	bgnHA;		// the start position of horizontal active output
	WORD	endHA;		// the end position of horizontal active output
	WORD	bgnHS;		// the start position of horizontal sync output
	WORD	endHS;		// the end position of horizontal sync output

	WORD	totVS;		// the vertical total size of output video
	WORD	bgnVA;		// the start position of vertical active output
	WORD	endVA;		// the end position of vertical active output
	WORD	bgnVS;		// the start position of vertical sync output
	WORD	endVS;		// the end position of vertical sync output

	WORD	bgnVAB;		// the start position of Vact for the bottom field
	WORD	endVAB;		// the end position of Vact for the bottom field
	WORD	bgnVSB;		// the start position of Vsync for the bottom field
	WORD	endVSB;		// the end position of Vsync for the bottom field
	WORD	posFLD;		// the transition position of Vsync for the bottom field

#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	WORD	vclkP;		// Pre-divider value of the internal PLL for video clock
	WORD	vclkM;		// Post-divider value of the internal PLL for video clock
	WORD	vclkS;		// Post-scaler value of the internal PLL for video clock
#else 								// MDIN-i51X (i510, i540, i550)
	WORD	vclkP;		// Pre-divider value of the internal PLL for video clock
	WORD	vclkN;		// Input-divider value of the internal PLL for video clock
	WORD	vclkM;		// Post-divider value of the internal PLL for video clock
	WORD	vclkOD;		// Output-divider value of the internal PLL for video clock
	WORD	vclkS;		// Post-scaler value of the internal PLL for video clock
#endif

	WORD	xclkS;		// S parameter of fractional divider in aux video clock
	WORD	xclkF;		// F parameter of fractional divider in aux video clock
	WORD	xclkT;		// T parameter of fractional divider in aux video clock

}	MISP_OUTVIDEO_SYNC, *PMISP_OUTVIDEO_SYNC;

typedef	struct
{
	WORD	Htot;		// horizontal active size of video
	WORD	attb;		// H/V sync polarity, scan type, etc
	WORD	Hact;		// horizontal active size of video
	WORD	Vact;		// verical active size of video

}	MISP_OUTVIDEO_ATTB, *PMISP_OUTVIDEO_ATTB;

typedef	struct
{
	BOOL	cbcr_swap;	// video CbCr-swap
	BOOL	tf_polar;	// top-field polarity
	BYTE	pin_de;		// DE_OUT pin mux
	BYTE	pin_hsync;	// HSYNC_OUT pin mux
	BYTE	pin_vsync;	// VSYNC_OUT pin mux
	BYTE	dummy0;		// dummy byte for 32-bit alignment
	WORD	dummy1;		// dummy byte for 32-bit alignment

}	MISP_OUTVIDEO_FINE, *PMISP_OUTVIDEO_FINE;

typedef	struct
{
	BYTE	brightness;		// default = 128, range: from 0 to 255
	BYTE	contrast;		// default = 128, range: from 0 to 255
	BYTE	saturation;		// default = 128, range: from 0 to 255
	BYTE	hue;			// default = 128, range: from 0 to 255

}	MISP_OUTVIDEO_TUNE, *PMISP_OUTVIDEO_TUNE;

#if MISP_TUNE_RGB_GAIN_OFFSET == 1
typedef	struct
{
	BYTE	r_gain;			// default = 128, range: 0~255, only used to RGB-out mode
	BYTE	g_gain;			// default = 128, range: 0~255, only used to RGB-out mode
	BYTE	b_gain;			// default = 128, range: 0~255, only used to RGB-out mode
	BYTE	r_offset;		// default = 128, range: 0~255, only used to RGB-out mode
	BYTE	g_offset;		// default = 128, range: 0~255, only used to RGB-out mode
	BYTE	b_offset;		// default = 128, range: 0~255, only used to RGB-out mode
	WORD	dummy;			// dummy byte for 32-bit alignment

}	MISP_OUTVIDEO_GAIN, *PMISP_OUTVIDEO_GAIN;
#endif

typedef	struct
{
	BYTE	back_y;			// BG color Y
	BYTE	back_cb;		// BG color Cb
	BYTE	back_cr;		// BG color Cr
	BYTE	dither;			// dither

}	MISP_OUTVIDEO_MISC, *PMISP_OUTVIDEO_MISC;

typedef	struct
{
	BYTE	frmt;			// video format of output (one of MISP_OUTVIDEO_FORMAT_t)
	BYTE	mode;			// video mode of output port (one of MISP_OUTPORT_MODE_t)

	BYTE	turnon_dly;		// video output turn-on delay mode delay
	BYTE	turnon_cnt;		// video output turn-on delay mode counter

	MISP_OUTVIDEO_SYNC	stSYNC;		// sync-info of out video
	MISP_OUTVIDEO_ATTB	stATTB;		// attribute of out video
	MISP_OUTVIDEO_FINE	stFINE;		// fine controls not in stATTB
	MISP_OUTVIDEO_TUNE	stTUNE;		// CSC tune of input video
#if MISP_TUNE_RGB_GAIN_OFFSET == 1
	MISP_OUTVIDEO_GAIN	stGAIN;		// CSC tune of input video (RGB gain/offset)
#endif
	MISP_OUTVIDEO_MISC	stMISC;		// background color, dither mode
	PMISP_CSCCTRL_INFO	pCSC;		// output CSC control

}	MISP_OUTVIDEO_INFO, *PMISP_OUTVIDEO_INFO;

typedef	struct
{
	BYTE	extn_en;		// enable Dig_io_ext output (i51x)
	BYTE	lvds_en;		// enable LVDS output (i51x)
	BYTE	gpio_en;		// set GPIO.A port as video pin
	BYTE	mode_sel;		// pin mapping of output port ( i500:MISP_OUTPUT_PINMAP_t, i51x:MISP_OUTPUT_LVDS/CMOS_PINMAP_t )
	
}	MISP_OUT_MODE_INFO, *PMISP_OUT_MODE_INFO;

typedef	struct
{	
	BYTE	ctrl_update ;	// Set 1/3/4 for update : auto set by API
	BOOL	blend_sel ;		// '0' use total_soild_blend , '1'use gradation_blend_window
	BOOL	grad_blend_en;	// '0' solid blend , '1' gradation blend
	BOOL	grad_blend_mode;// '0' H direction , '1' V direction
	WORD	grad_blend_init;// grad_blend_en '0' 0(PiP) ~ 15(Main) ,'1' 0(PiP) ~ 4095(Main)
	WORD	grad_blend_step; // gradation step size (13b signed)
	
	WORD	grad_blend_sh;	// gradation window start h position
	WORD	grad_blend_sv;	// gradation window start v position
	WORD	grad_blend_eh;	// gradation window end h position
	WORD	grad_blend_ev;	// gradation window end v position		
}	MISP_PIP_GRAD_WINDOW, *PMISP_PIP_GRAD_WINDOW;

typedef	struct
{	
	BYTE	blend_lev;		// pip blend level : total_soild_blend

	BOOL	key_en;			// color key enable
	BYTE	key_y_u;		// color key upper bound Y
	BYTE	key_y_l;		// color key lower bound Y
	BYTE	key_cb_u;		// color key upper bound Cb
	BYTE	key_cb_l;		// color key lower bound Cb
	BYTE	key_cr_u;		// color key upper bound Cr
	BYTE	key_cr_l;		// color key lower bound Cr
	
	MISP_PIP_GRAD_WINDOW	pip_blend_window[4] ; // Pip gradiation_blend_window : 4ea
	
}	MISP_PIP_CONTROL, *PMISP_PIP_CONTROL;

typedef	struct
{
	BYTE	lock_mode;		// external lock mode
	BYTE	lock_source;		// external lock_source 
		// Only for lock_mode is set to 1 or 2.
		// 1 : separate sync signals on the main input port
		// 2 : separate sync signals on the aux input port
		// 3 : selected sync signals for the main input path
		// 4 : selected sync signals for the aux input path
		// 5 : sync signals from main input DTR decoder
		// 6 : sync signals from main sensor input
		// 7 : sync signals from aux input DTR decoder
		// 8 : sync signals from aux sensor input			
	
	WORD	sync_mode;		// external sync mode

	WORD	hs_dly;			// external sync lock h_sync delay
	WORD	vs_dly;			// external sync lock v_sync delay

}	MISP_EXTLOCK_CTRL, *PMISP_EXTLOCK_CTRL;

typedef struct
{
	BOOL lvds_tx_enable;			// LVDS TX1 Enable
	BOOL lvds_tx_mode;				// LVDS TX1 mode select		('0' : single mode	'1' : dual mode)
	BYTE lvds_tx_map;				// LVDS bit map select		('00' : Zhine VILM Map	'01' : Zhine VIHM Map = Open JEIDA		'10' : VESA Map		'11' : Sony YC Map(set h/w(THC64LVDS1024) pin map VIHM))
	BYTE lvds_tx_portb_ctrl;			// LVDS TX2 control. When single lvds tx output mode. 	('00' : B port disabled	'01' : B port output set same A port signal		'10' : B port stand alone mode(external input out)	'11' : A port and B port output swap)
	BYTE lvds_tx_cont1;				// LVDS TX1 user program'd data
	BYTE lvds_tx_cont2;				// LVDS TX2 user program'd data
	BOOL lvds_tx_map_option;		// DE port selection when Sony YC Map mode	('0' : C6	'1' : DE)
	BOOL lvds_tx_state;				// Internal state machine. 				('0' : use toggle video clock	'1' : use pll counter)
	BOOL lvds_tx_state_retry;		// This register value move 1 to 0, then reset pll counter. 
	BOOL lvds_tx2_en;				// Disable LVDS TX2 clock ('0' : TX2 disable	'1' : TX2 enable )
	BOOL lvds_tx_data_swap1;		// LVDS TX1 data LSB MSB swap
	BOOL lvds_tx_data_swap2;		// LVDS TX2 data LSB MSB swap
	
	BOOL lvds_tx_evenodd_en;		//LVDS TX wide mode enable
	BOOL lvds_tx1_main_aux_sel;	//LVDS TX1 out source select
	BOOL lvds_tx1_444_422_sel;		//LVDS TX1 out format select
	BOOL lvds_tx2_main_aux_sel;	//LVDS TX2 out source select
	BOOL lvds_tx2_444_422_sel;		//LVDS TX2 out format select
	BOOL lvds_tx1_hs_invert;			//LVDS TX1 hsync invert
	BOOL lvds_tx1_vs_invert;			//LVDS TX1 vsync invert
	BOOL lvds_tx1_de_invert;		//LVDS TX1 DE invert
	BOOL lvds_tx2_hs_invert;			//LVDS TX2 hsync invert
	BOOL lvds_tx2_vs_invert;			//LVDS TX2 vsync invert
	BOOL lvds_tx2_de_invert;		//LVDS TX2 DE invert

	BOOL lvds_tclk1_pn_swap;		//LVDS TX1 data PN swap( tclk1 inv )
	BOOL lvds_te1_pn_swap;			//LVDS TX1 data PN swap( Port te1 inv )
	BOOL lvds_td1_pn_swap;			//LVDS TX1 data PN swap( Port td1 inv )
	BOOL lvds_tc1_pn_swap;			//LVDS TX1 data PN swap( Port tc1 inv )
	BOOL lvds_tb1_pn_swap;			//LVDS TX1 data PN swap( Port tb1 inv )
	BOOL lvds_ta1_pn_swap;			//LVDS TX1 data PN swap( Port ta1 inv )

	BOOL lvds_tclk2_pn_swap;		//LVDS TX2 data PN swap( tclk2 inv )
	BOOL lvds_te2_pn_swap;			//LVDS TX2 data PN swap( Port te2 inv )
	BOOL lvds_td2_pn_swap;			//LVDS TX2 data PN swap( Port td2 inv )
	BOOL lvds_tc2_pn_swap;			//LVDS TX2 data PN swap( Port tc2 inv )
	BOOL lvds_tb2_pn_swap;			//LVDS TX2 data PN swap( Port tb2 inv )
	BOOL lvds_ta2_pn_swap;			//LVDS TX2 data PN swap( Port ta2 inv )



}	MISP_OUT_LVDS_CTRL, *PMISP_OUT_LVDS_CTRL;

typedef enum {
	MISP_VID_VENC_NTSC_M = 0,			// the monochrome standard for NTSC
	MISP_VID_VENC_NTSC_J,				// NTSC system in japan
	MISP_VID_VENC_NTSC_443,				// NTSC system in 4.43MHz
	MISP_VID_VENC_PAL_B,					// (B) PAL system
	MISP_VID_VENC_PAL_D,					// (D) PAL system
	MISP_VID_VENC_PAL_G,					// (G) PAL system
	MISP_VID_VENC_PAL_H,					// (H) PAL system
	MISP_VID_VENC_PAL_I,					// (I) PAL system
	MISP_VID_VENC_PAL_M,					// (M) PAL system
	MISP_VID_VENC_PAL_N,					// (N) PAL system
	MISP_VID_VENC_PAL_Nc,				// (Nc) PAL system
	MISP_VID_VENC_PAL_60,				// (60) PAL system
	MISP_VID_VENC_SECAM,				// SECAM system
	MISP_VID_VENC_AUTO,					// Auto Detection
	MISP_VID_VENC_NOTVALID,			// not Valid
	MISP_VID_VENC_FMT_END				// source format end
}	MISP_VENC_FORMAT_t;


typedef struct {
	LONG	ctrl;					// input control flag

	WORD	dspFLAG;				// the flag of display of video(main, aux)
	BYTE	encFRMT;				// video format of VENC (one of MISP_VENC_FORMAT_t)
	BYTE	enhFLAG;				// the flag of enhancement (BWEXT, LTI, CTI, COLOR)
	BYTE	dummy;					// dummy byte for 32-bit alignment

	MISP_OUT_MODE_INFO	stPinMap;	// output pin mode
	MISP_PIP_CONTROL	stPIPctl;		// pip blend/key control
	MISP_EXTLOCK_CTRL	stExtLock;	// external sync lock control
	MISP_OUT_LVDS_CTRL	stLVDS_tx;	// lvds tx control	

	MISP_OUTVIDEO_INFO	stOUT_m;	// output video info of main path
	MISP_VIDEO_WINDOW	stCROP_m;	// crop window of main path for image cut
	MISP_VIDEO_WINDOW	stZOOM_m;	// zoom window of main path for overscan
	MISP_VIDEO_WINDOW	stVIEW_m;	// view window of main path for aspect ratio

	MISP_OUTVIDEO_INFO	stOUT_x;	// output video info of aux path
	MISP_VIDEO_WINDOW	stCROP_x;	// crop window of aux path for image cut
	MISP_VIDEO_WINDOW	stZOOM_x;	// zoom window of aux path for overscan
	MISP_VIDEO_WINDOW	stVIEW_x;	// view window of aux path for aspect ratio

//	MISP_FRAMEMAP_INFO	stMAP_m;	// frame buffer map of main&aux path
//	MISP_DEINTCTL_INFO	stIPC_m;	// deinterlacer of main path only

}	OUT_CTRL_PARAM, *POUT_CTRL_PARAM;

// control parameter
#define		OUTCTRL_TRIGGER_ISP			0x40000000		// ISP update
#define		OUTCTRL_UPDATE_PINMAP		0x00000001		// output main source update
#define		OUTCTRL_UPDATE_MAIN			0x00000002		// output main source update
#define		OUTCTRL_UPDATE_AUX			0x00000004		// output aux source update
#define 	OUTCTRL_UPDATE_ENCFRMT		0x00000008		// output encoder format update
#define 	OUTCTRL_UPDATE_ENCMISC		0x00000010		// output encoder control update
#define 	OUTCTRL_UPDATE_MAINDISP		0x00000020		// output main display update
#define 	OUTCTRL_UPDATE_AUXDISP		0x00000040		// output aux display update
#define 	OUTCTRL_UPDATE_MAINCSC		0x00000080		// output main csc update
#define 	OUTCTRL_UPDATE_AUXCSC 		0x00000100		// output aux csc update
#define 	OUTCTRL_UPDATE_ENHANCE		0x00000200		// output enhancement update
#define 	OUTCTRL_UPDATE_MAINMISC		0x00000400		// output main background, dither update
#define 	OUTCTRL_UPDATE_AUXMISC 		0x00000800		// output aux background, dither update
#define 	OUTCTRL_UPDATE_PIP			0x00001000		// output PIP control
#define 	OUTCTRL_UPDATE_EXTLOCK		0x00002000		// output external sync lock control
#define 	OUTCTRL_UPDATE_ALL 			0x0000FFFF		// output all(main,aux,enc) update

#define 	OUTCTRL_VENC_CTRL_MASK		0x00F00000		// v.enc control mask
#define 	OUTCTRL_VENC_COLOR_DIS		0x00100000		// v.enc color disable
#define 	OUTCTRL_VENC_COLOR_EN		0x00000000		// v.enc color enable
#define 	OUTCTRL_VENC_BURST_DIS		0x00200000		// v.enc burst disable
#define 	OUTCTRL_VENC_BURST_EN		0x00000000		// v.enc burst enable
#define 	OUTCTRL_VENC_BLUESCR_EN		0x00400000		// v.enc blue screen enable
#define 	OUTCTRL_VENC_BLUESCR_DIS	0x00000000		// v.enc blue screen disable

#define		OUTCTRL_AUX_USEDBY_FUNC		0x00010000		// aux video data used by user function(pip & v.enc disable)


// user define for fine control of output video
//#define	MISP_CbCrSWAP_ON			0x0010	// pre-defined at source video
//#define	MISP_CbCrSWAP_OFF			0x0000	//
//#define		MISP_PbPrSWAP_ON			0x0020	// PbPr are swapped on DAC output
//#define		MISP_PbPrSWAP_OFF			0x0000	// PbPr are not swapped on DAC output
//#define		MISP_SYNC_FREERUN			0x0040	// out-sync runs freely regardless of in-sync
//#define		MISP_SYNC_FRMLOCK			0x0000	// out-sync is frame-locked to the in-sync
//#define		MISP_POSITIVE_DE			0x0080	// DE data region is high level
//#define		MISP_NEGATIVE_DE			0x0000	// DE data region is low level
//#define		MISP_DEOUT_IS_DE			0x0000	// DE_OUT pin is data enable signal
//#define		MISP_DEOUT_IS_COMP			0x0400	// DE_OUT pin is composite sync
//#define		MISP_DEOUT_IS_FLDID			0x0800	// DE_OUT pin is field-id signal
//#define		MISP_DEOUT_IS_HACT			0x0c00	// DE_OUT pin is Hactive signal
//#define		MISP_VSYNC_IS_VSYNC			0x0000	// VSYNC_OUT pin is Vsync signal
//#define		MISP_VSYNC_IS_COMP			0x1000	// VSYNC_OUT pin is composite sync
//#define		MISP_VSYNC_IS_DE			0x2000	// VSYNC_OUT pin is data enable signal
//#define		MISP_VSYNC_IS_VACT			0x3000	// VSYNC_OUT pin is Vactive signal
//#define		MISP_HSYNC_IS_HSYNC			0x0000	// HSYNC_OUT pin is Hsync signal
//#define		MISP_HSYNC_IS_COMP			0x4000	// HSYNC_OUT pin is composite sync
//#define		MISP_HSYNC_IS_DE			0x8000	// HSYNC_OUT pin is data enable signal
//#define		MISP_HSYNC_IS_HACT			0xc000	// HSYNC_OUT pin is Hactive signal

// user define for display of video (dspFLAG)
#define		MISP_AUX_DISPLAY_ON			0x0001		// display on of aux-video
#define		MISP_AUX_DISPLAY_OFF		0x0000		// display off of aux-video
#define		MISP_AUX_FREEZE_ON			0x0002		// freeze on of aux-video
#define		MISP_AUX_FREEZE_OFF			0x0000		// freeze off of aux-video
#define		MISP_AUX_MAINOSD_ON			0x0004		// main-osd on of aux-video
#define		MISP_AUX_MAINOSD_OFF		0x0000		// main-osd off of aux-video
#define		MISP_AUX_SYNC_PIP_EN		0x0008		// aux sync enable for pip
#define		MISP_AUX_SYNC_PIP_DIS		0x0000		// aux sync disable for pip
#define		MISP_MAIN_DISPLAY_ON		0x0010		// display on of main-video
#define		MISP_MAIN_DISPLAY_OFF		0x0000		// display on of main-video
#define		MISP_MAIN_FREEZE_ON			0x0020		// freeze on of main-video
#define		MISP_MAIN_FREEZE_OFF		0x0000		// freeze off of main-video
#define		MISP_MAIN_DARKSCR_ON		0x0040		// dark-screen on of main-video
#define		MISP_MAIN_DARKSCR_OFF		0x0000		// dark-screen off of main-video
#define		MISP_AUX_DARKSCR_ON			0x0080		// dark-screen on of aux-video
#define		MISP_AUX_DARKSCR_OFF		0x0000		// dark-screen off of aux-video

#define		MISP_MAIN_OUT_TP_MASK		0x0F00		// testpattern of main-video
#define		MISP_MAIN_OUT_TP_OFF		0x0000		// tp off
#define		MISP_MAIN_OUT_TP_WHITE		0x0100		// white test pattern
#define		MISP_MAIN_OUT_TP_CROSS		0x0200		// cross
#define		MISP_MAIN_OUT_TP_CRHATCH	0x0300		// cross hatch
#define		MISP_MAIN_OUT_TP_COLOR		0x0400		// colorbar
#define		MISP_MAIN_OUT_TP_GRAY		0x0500		// graybar
#define		MISP_MAIN_OUT_TP_WINWHITE	0x0600		// white window
#define		MISP_MAIN_OUT_TP_HRAMP		0x0700		// horizental ramp
#define		MISP_MAIN_OUT_TP_WIDEHRAMP	0x0800		// wide horizental ramp
#define		MISP_MAIN_OUT_TP_WIDEVRAMP	0x0900		// wide vertical ramp
#define		MISP_MAIN_OUT_TP_DIAGONAL	0x0A00		// graybar
#define		MISP_MAIN_OUT_TP_RED		0x0B00		// red
#define		MISP_MAIN_OUT_TP_GREEN		0x0C00		// green
#define		MISP_MAIN_OUT_TP_BLUE		0x0D00		// blue
#define		MISP_MAIN_OUT_TP_RESERVED	0x0E00		// reserved
#define		MISP_MAIN_OUT_TP_MULTICH	0x0F00		// multi channel test pattern

#define		MISP_AUX_OUT_TP_MASK		0xF000		// testpattern of aux-video
#define		MISP_AUX_OUT_TP_OFF			0x0000		// tp off
#define		MISP_AUX_OUT_TP_WHITE		0x1000		// white test pattern
#define		MISP_AUX_OUT_TP_CROSS		0x2000		// cross
#define		MISP_AUX_OUT_TP_CRHATCH		0x3000		// cross hatch
#define		MISP_AUX_OUT_TP_COLOR		0x4000		// colorbar
#define		MISP_AUX_OUT_TP_GRAY		0x5000		// graybar
#define		MISP_AUX_OUT_TP_WINWHITE	0x6000		// white window
#define		MISP_AUX_OUT_TP_HRAMP		0x7000		// horizental ramp
#define		MISP_AUX_OUT_TP_WIDEHRAMP	0x8000		// wide horizental ramp
#define		MISP_AUX_OUT_TP_WIDEVRAMP	0x9000		// wide vertical ramp
#define		MISP_AUX_OUT_TP_DIAGONAL	0xA000		// graybar
#define		MISP_AUX_OUT_TP_RED			0xB000		// red
#define		MISP_AUX_OUT_TP_GREEN		0xC000		// green
#define		MISP_AUX_OUT_TP_BLUE		0xD000		// blue
#define		MISP_AUX_OUT_TP_RP1981		0xE000		// rp198 test pattern 1
#define		MISP_AUX_OUT_TP_RP1982		0xF000		// rp198 test pattern 2


// enhancement flag (enhFLAG)
#define 	MISP_ENH_BWEXT_ON			0x01	// b/w extension enable
#define 	MISP_ENH_BWEXT_OFF			0x00	// b/w extension disable
#define 	MISP_ENH_LTI_ON				0x02	// LTI enable
#define 	MISP_ENH_LTI_OFF			0x00	// LTI disable
#define 	MISP_ENH_CTI_ON				0x04	// CTI enable
#define 	MISP_ENH_CTI_OFF			0x00	// CTI disable
#define 	MISP_ENH_COLOR_ON			0x08	// color enhancement enable
#define 	MISP_ENH_COLOR_OFF			0x00	// color enhancement disable



#if defined(__MDIN_i550_USB_V100__)
#define		GetOUT_IspFrmt()	GetOUT_AuxFrmt()
#define		GetOUT_IspMode()	GetOUT_AuxMode()
#define		SetOUT_IspFrmt(x)	SetOUT_AuxFrmt(x)
#else
#define		GetOUT_IspFrmt()	GetOUT_MainFrmt()
#define		GetOUT_IspMode()	GetOUT_MainMode()
#define		SetOUT_IspFrmt(x)	SetOUT_MainFrmt(x)
#endif


// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------
extern ROMDATA MISP_OUTVIDEO_SYNC defMISPOutSync[];
extern ROMDATA MISP_OUTVIDEO_ATTB defMISPOutVideo[];

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void Init_OUT(void);
void OUT_ProcessHandler(void);

void SetOUT_MainFrmt(BYTE frmt);
void SetOUT_MainMode(BYTE mode);
void SetOUT_MaintotHS(WORD val);
void SetOUT_MainSizeHA(WORD val);
void SetOUT_MainSizeVA(WORD val);
void SetOUT_MainbgnHA(WORD val);
void SetOUT_MainendHA(WORD val);
void SetOUT_MainbgnVA(WORD val);
void SetOUT_MainendVA(WORD val);
void SetOUT_MainbgnVAB(WORD val);
void SetOUT_MainendVAB(WORD val);
void SetOUT_MainContrast(BYTE val);
void SetOUT_MainBrightness(BYTE val);
void SetOUT_MainSaturation(BYTE val);
void SetOUT_MainHue(BYTE val);
void SetOUT_MainDispEN(BOOL nID);
void SetOUT_MainDispOnOffAuto(BYTE delay);
void SetOUT_MainDarkScreenAuto(BYTE delay);
void SetOUT_AuxDispOnOffAuto(BYTE delay);
void SetOUT_AuxDarkScreenAuto(BYTE delay);
void SetOUT_MainFreezeEN(BOOL nID);
void SetOUT_MainCSC(BYTE nID);
BYTE GetOUT_MainCSC(void);
void SetOUT_AuxCSC(BYTE nID);
BYTE GetOUT_AuxCSC(void);

void SetOUT_AuxFrmt(BYTE frmt);
void SetOUT_AuxMode(BYTE mode);
void SetOUT_AuxtotHS(WORD val);
void SetOUT_AuxSizeHA(WORD val);
void SetOUT_AuxSizeVA(WORD val);
void SetOUT_AuxbgnHA(WORD val);
void SetOUT_AuxendHA(WORD val);
void SetOUT_AuxbgnVA(WORD val);
void SetOUT_AuxendVA(WORD val);
void SetOUT_AuxbgnVAB(WORD val);
void SetOUT_AuxendVAB(WORD val);
void SetOUT_AuxDispEN(BOOL nID);
void SetOUT_AuxFreezeEN(BOOL nID);
void SetOUT_AuxPipEN(BOOL nID);
void SetOUT_AuxUsedByFunc(BOOL nID);
BOOL GetOUT_AuxUsedByFunc(void);
void SetOUT_AuxContrast(BYTE val);
void SetOUT_AuxBrightness(BYTE val);
void SetOUT_AuxSaturation(BYTE val);
void SetOUT_AuxHue(BYTE val);

void SetOUT_EncFrmt(BYTE frmt);
BYTE GetOUT_EncFrmt(void);

void SetOUT_CVBSNTSC720H(void);
void SetOUT_CVBSNTSC960H(void);
void SetOUT_CVBSPAL(void);
void SetOUT_CVBSOutEN(void);
void SetOUT_CVBSColor(BOOL OnOff);
void SetOUT_CVBSBurst(BOOL OnOff);
void SetOUT_CVBSBlueScreen(BOOL OnOff);
BOOL GetOUT_CVBSBurst(void);

void SetOUT_BWExtension(BOOL OnOff);
void SetOUT_EnableLTI(BOOL OnOff);
void SetOUT_EnableCTI(BOOL OnOff);

void ISPM VideoTurnOnCounter(void);

BYTE GetOUT_MainFrmt(void);
BYTE GetOUT_MainMode(void);
WORD GetOUT_MainbgnHA(void);
WORD GetOUT_MainendHA(void);
WORD GetOUT_MainbgnVA(void);
WORD GetOUT_MainendVA(void);
WORD GetOUT_MainSizeHA(void);
WORD GetOUT_MainSizeVA(void);
BOOL GetOUT_MainScanTypeProg(void);
PMISP_OUTVIDEO_SYNC	GetOUT_MainSync(void);
PMISP_OUTVIDEO_ATTB	GetOUT_MainAttb(void);

BYTE GetOUT_AuxFrmt(void);
BYTE GetOUT_AuxMode(void);
WORD GetOUT_AuxbgnHA(void);
WORD GetOUT_AuxendHA(void);
WORD GetOUT_AuxbgnVA(void);
WORD GetOUT_AuxendVA(void);
WORD GetOUT_AuxSizeHA(void);
WORD GetOUT_AuxSizeVA(void);
BOOL GetOUT_AuxScanTypeProg(void);
BOOL GetOUT_AuxPipEN(void);
PMISP_OUTVIDEO_SYNC	GetOUT_AuxSync(void);
PMISP_OUTVIDEO_ATTB	GetOUT_AuxAttb(void);

BOOL GetOUT_MainFreezeEN(void);
BOOL GetOUT_AuxFreezeEN(void);
BOOL GetOUT_BWExtension(void);
BOOL GetOUT_EnableLTI(void);
BOOL GetOUT_EnableCTI(void);
BYTE GetOUT_MainContrast(void);
BYTE GetOUT_MainBrightness(void);
BYTE GetOUT_MainSaturation(void);
BYTE GetOUT_MainHue(void);
BYTE GetOUT_AuxContrast(void);
BYTE GetOUT_AuxBrightness(void);
BYTE GetOUT_AuxSaturation(void);
BYTE GetOUT_AuxHue(void);
BOOL GetOUT_MainDispEN(void);
BOOL GetOUT_AuxDispEN(void);

void SetOUT_MainBackgroundY(BYTE val);
void SetOUT_MainBackgroundCb(BYTE val);
void SetOUT_MainBackgroundCr(BYTE val);
void SetOUT_MainDithering(BOOL OnOff);
void SetOUT_MainDarkScreen(BOOL OnOff);
void SetOUT_MainTestPattern(BYTE val);
void SetOUT_AuxBackgroundY(BYTE val);
void SetOUT_AuxBackgroundCb(BYTE val);
void SetOUT_AuxBackgroundCr(BYTE val);
void SetOUT_AuxDithering(BOOL OnOff);
void SetOUT_AuxDarkScreen(BOOL OnOff);
void SetOUT_AuxTestPattern(BYTE val);
BYTE GetOUT_MainBackgroundY(void);
BYTE GetOUT_MainBackgroundCb(void);
BYTE GetOUT_MainBackgroundCr(void);
BOOL GetOUT_MainDarkScreen(void);
BYTE GetOUT_MainTestPattern(void);
BYTE GetOUT_AuxBackgroundY(void);
BYTE GetOUT_AuxBackgroundCb(void);
BYTE GetOUT_AuxBackgroundCr(void);
BOOL GetOUT_AuxDarkScreen(void);
BYTE GetOUT_AuxTestPattern(void);

void SetOUT_MainSwapCbCr(BOOL OnOff);
void SetOUT_MainHsPolar(BOOL OnOff);
void SetOUT_MainVsPolar(BOOL OnOff);
void SetOUT_AuxSwapCbCr(BOOL OnOff);
void SetOUT_AuxHsPolar(BOOL OnOff);
void SetOUT_AuxVsPolar(BOOL OnOff);
BOOL GetOUT_MainSwapCbCr(void);
BOOL GetOUT_MainHsPolar(void);
BOOL GetOUT_MainVsPolar(void);
BOOL GetOUT_AuxSwapCbCr(void);
BOOL GetOUT_AuxHsPolar(void);
BOOL GetOUT_AuxVsPolar(void);

void SetOUT_MainPinDe(BYTE val);
void SetOUT_MainPinHsync(BYTE val);
void SetOUT_MainPinVsync(BYTE val);
void SetOUT_AuxPinHsync(BYTE val);
void SetOUT_AuxPinVsync(BYTE val);
void SetOUT_ColorEnhEn(BOOL OnOff);
BYTE GetOUT_MainPinDe(void);
BYTE GetOUT_MainPinHsync(void);
BYTE GetOUT_MainPinVsync(void);
BYTE GetOUT_AuxPinHsync(void);
BYTE GetOUT_AuxPinVsync(void);
BOOL GetOUT_ColorEnhEn(void);

void SetOUT_PipBlend(BYTE val);
void SetOUT_PipChromaKeyEn(BOOL OnOff);
void SetOUT_PipChromaKeyYUBound(BYTE val);
void SetOUT_PipChromaKeyYLBound(BYTE val);
void SetOUT_PipChromaKeyCbUBound(BYTE val);
void SetOUT_PipChromaKeyCbLBound(BYTE val);
void SetOUT_PipChromaKeyCrUBound(BYTE val);
void SetOUT_PipChromaKeyCrLBound(BYTE val);
BYTE GetOUT_PipBlend(void);
BOOL GetOUT_PipChromaKeyEn(void);
BYTE GetOUT_PipChromaKeyYUBound(void);
BYTE GetOUT_PipChromaKeyYLBound(void);
BYTE GetOUT_PipChromaKeyCbUBound(void);
BYTE GetOUT_PipChromaKeyCbLBound(void);
BYTE GetOUT_PipChromaKeyCrUBound(void);
BYTE GetOUT_PipChromaKeyCrLBound(void);

void SetOUT_PipGradBlendSel(BYTE window_no, BYTE Sel);
void SetOUT_PipGradBlendEn(BYTE window_no, BYTE en, BYTE hv_mode, WORD initial, WORD step);
void SetOUT_PipGradBlendPos(BYTE window_no, WORD sh, WORD sv, WORD eh, WORD ev);


void SetOUT_PinMapExtn(BOOL OnOff);
BYTE GetOUT_PinMapExtn(void);
void SetOUT_PinMapLvds(BOOL OnOff);
BYTE GetOUT_PinMapLvds(void);
void SetOUT_PinMapGpio(BOOL OnOff);
BYTE GetOUT_PinMapGpio(void);
void SetOUT_PinMapMode(BYTE mode);
BYTE GetOUT_PinMapMode(void);

void SetOUT_MainTfPolar(BOOL OnOff);
void SetOUT_AuxTfPolar(BOOL OnOff);
BOOL GetOUT_MainTfPolar(void);
BOOL GetOUT_AuxTfPolar(void);

void SetOUT_ExtLockHSdelay(WORD dly);
void SetOUT_ExtLockVSdelay(WORD dly);
WORD GetOUT_ExtLockHSdelay(void);
WORD GetOUT_ExtLockVSdelay(void);
void SetOUT_ExtLockMode(BYTE lock_mode, BYTE lock_source);
void SetOUT_ExtSyncMode(WORD sync_mode);

BOOL IsOUT_MainFrmt4K(void);
BOOL IsOUT_MainFrmt4M(void);
BOOL IsOUT_MainFrmt3G(void);
BOOL IsOUT_MainFrmt12G(void);
BOOL IsOUT_MainSyncEmb(void);
WORD GetOUT_MainFps(void);


#endif	/* __MISPOUTPUT_H__ */
