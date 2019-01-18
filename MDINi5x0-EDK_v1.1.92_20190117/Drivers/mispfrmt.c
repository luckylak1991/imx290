//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MISPFRMT.C
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#include	"misp100.h"

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
// OUT sync control
ROMDATA MISP_OUTVIDEO_SYNC defMISPOutSync[]		= {
// TV
	{	// 720x480i60
		  20,		// hdpulse_pos
		   8,		// vdpulse_pos
		 270,	// vdpulse_pos_bot
		 858,	// htotal_size
		 120,		// hactive_start
		 840,	// hactive_end
		  63,		// hsync_start
		   1,		// hsync_end
		 525,	// vtotal_size
		  21,		// vactive_start
		 261,	// vactive_end
		   6,		// vsync_start
		   3,		// vsync_end
		 284,	// vactive_start_bot
		 524,	// vactive_end_bot
		 268,	// vsync_start_bot
		 265,	// vsync_end_bot
		 430,	// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   4,		// pre_div_vclk (P)
		  16,		// post_div_vclk (M)
		   8,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  24,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   6,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		  27,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
	#else
		  29,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
	#endif
	},
	{	// 720x576i50
		  20,		// hdpulse_pos
		   8,		// vdpulse_pos
		 320,		// vdpulse_pos_bot
		 864,		// htotal_size
		 133,		// hactive_start
		 853,		// hactive_end
		  64,		// hsync_start
		   1,		// hsync_end
		 625,		// vtotal_size
		  23,		// vactive_start
		 311,		// vactive_end
		   4,		// vsync_start
		   1,		// vsync_end
		 336,		// vactive_start_bot
		 624,		// vactive_end_bot
		 316,		// vsync_start_bot
		 313,		// vsync_end_bot
		 433,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   4,		// pre_div_vclk (P)
		  16,		// post_div_vclk (M)
		   8,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  24,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   6,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		  27,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
	#else
		  29,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
	#endif
	},
	{	// 960x480i60
		  20,		// hdpulse_pos
		   8,		// vdpulse_pos
//		 273,	// vdpulse_pos_bot		// modified
//		2200,	// htotal_size			// modified
//		 400,		// hactive_start		// modified
//		2090,	// hactive_end			// modified
//		 126,		// hsync_start		// modified
//		   2,		// hsync_end		// modified

	   272,   // vdpulse_pos_bot	  // modified
	  2288/2, 	  // htotal_size
	   160/2+33, 	  // hactive_start
	  2080/2+33, 	  // hactive_end
		46/2, 	  // hsync_start
		 2/2, 	  // hsync_end
		 525,	// vtotal_size
		  21,		// vactive_start
		 261,	// vactive_end
		   6,		// vsync_start
		   3,		// vsync_end
		 284,	// vactive_start_bot
		 524,	// vactive_end_bot
		 268,	// vsync_start_bot
		 265,	// vsync_end_bot
		 430,	// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   4,		// pre_div_vclk (P)
		  16,		// post_div_vclk (M)
		   8,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   3,		// input_div_vclk (N)
		  64,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   4,		// post_scale_vclk (S)
#endif

#if defined(SYSTEM_USE_MCLK189)
		  27,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
#else
		  29,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
#endif
	},
	{	// 960x576i50
		  20,		// hdpulse_pos
		   8,		// vdpulse_pos
		 320,		// vdpulse_pos_bot
	  2400/2, 	  // htotal_size
	   160/2+33, 	  // hactive_start
	  2080/2+33, 	  // hactive_end
		46/2, 	  // hsync_start
		 2/2, 	  // hsync_end
		 625,		// vtotal_size
		  23,		// vactive_start
		 311,		// vactive_end
		   4,		// vsync_start
		   1,		// vsync_end
		 336,		// vactive_start_bot
		 624,		// vactive_end_bot
		 316,		// vsync_start_bot
		 313,		// vsync_end_bot
		 433,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   4,		// pre_div_vclk (P)
		  16,		// post_div_vclk (M)
		   8,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   3,		// input_div_vclk (N)
		  64,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   4,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		  27,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
	#else
		  29,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
	#endif
	},
	{	// 720x480p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		 858,		// htotal_size
		 123,		// hactive_start
		 843,		// hactive_end
		  63,		// hsync_start
		   1,		// hsync_end
		 525,		// vtotal_size
		  43,		// vactive_start
		 523,		// vactive_end
		  13,		// vsync_start
		   7,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   4,		// pre_div_vclk (P)
		  16,		// post_div_vclk (M)
		   4,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  32,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   4,		// post_scale_vclk (S)
#endif

// modified for vclk=36MHz by Jay
//		   1,		// pre_div_vclk (P)
//		  12,		// post_div_vclk (M)
//		   9,		// post_scale_vclk (S)

	#if defined(SYSTEM_USE_MCLK189)
		  13,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
	#else
		  14,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
	#endif
	},
	{	// 720x576p50
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		 864,		// htotal_size
		 133,		// hactive_start
		 853,		// hactive_end
		  65,		// hsync_start
		   1,		// hsync_end
		 625,		// vtotal_size
		  45,		// vactive_start
		 621,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   4,		// pre_div_vclk (P)
		  16,		// post_div_vclk (M)
		   4,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  32,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   4,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		  13,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
	#else
		  14,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
	#endif
	},
	{	// 960x480p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1144,		// htotal_size
		 123,		// hactive_start
		1083,		// hactive_end
		  63,		// hsync_start
		   1,		// hsync_end
		 525,		// vtotal_size
		  43,		// vactive_start
		 523,		// vactive_end
		  13,		// vsync_start
		   7,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   4,		// pre_div_vclk (P)
		  16,		// post_div_vclk (M)
		   4,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   3,		// input_div_vclk (N)
		 128,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   4,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		  13,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
	#else
		  14,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
	#endif
	},
	{	// 960x576p50
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1200,		// htotal_size
		 133,		// hactive_start
		1093,		// hactive_end
		  65,		// hsync_start
		   1,		// hsync_end
		 625,		// vtotal_size
		  45,		// vactive_start
		 621,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   4,		// pre_div_vclk (P)
		  16,		// post_div_vclk (M)
		   4,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   9,		// input_div_vclk (N)
		 200,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   4,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		  13,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
	#else
		  14,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
	#endif
	},
	{	// 1280x720p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1650,		// htotal_size
		 261,		// hactive_start
		1541,		// hactive_end
		  41,		// hsync_start
		   1,		// hsync_end
		 750,		// vtotal_size
		  26,		// vactive_start
		 746,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   1,		// pre_div_vclk (P)
		  11,		// post_div_vclk (M)
		   4,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		   1,		// pre_div_vclk (P)
		   2,		// input_div_vclk (N)
		  77,		// post_div_vclk (M)
		   1,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#else
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	
#if defined(__USE_59Hz_OUTPUT__)
	{	// 1280x720p59.94
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1650,		// htotal_size
		 261,		// hactive_start
		1541,		// hactive_end
		  41,		// hsync_start
		   1,		// hsync_end
		 750,		// vtotal_size
		  26,		// vactive_start
		 746,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  91,		// pre_div_vclk (P)
		250,		// post_div_vclk (M)
		   1,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550) --- 74.1758Mhz with ext. osc(37.0879Mhz)
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  14,		// post_div_vclk (M)
		   0,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#elif defined(__USE_EXT_OSC_148M__)
		   1,		// pre_div_vclk (P)
		   4,		// input_div_vclk (N)
		  16,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#else
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  16,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  52		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		  23,		// axclk_gen_div_f (F)
		  50		// axclk_gen_div_t (T)
	#endif
	},
#endif	//#if defined(__USE_59Hz_OUTPUT__)
	
	{	// 1280x720p50
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1980,		// htotal_size
		 261,		// hactive_start
		1541,		// hactive_end
		  41,		// hsync_start
		   1,		// hsync_end
		 750,		// vtotal_size
		  26,		// vactive_start
		 746,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   1,		// pre_div_vclk (P)
		  11,		// post_div_vclk (M)
		   4,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		   1,		// pre_div_vclk (P)
		   2,		// input_div_vclk (N)
		  77,		// post_div_vclk (M)
		   1,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#else
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},

	{	// 1280x720p30
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		3300,		// htotal_size
		 261,		// hactive_start
		1541,		// hactive_end
		  41,		// hsync_start
		   1,		// hsync_end
		 750,		// vtotal_size
		  26,		// vactive_start
		 746,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   1,		// pre_div_vclk (P)
		  11,		// post_div_vclk (M)
		   4,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		   1,		// pre_div_vclk (P)
		   2,		// input_div_vclk (N)
		  77,		// post_div_vclk (M)
		   1,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#else
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	{	// 1280x720p25
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		3960,		// htotal_size
		 261,		// hactive_start
		1541,		// hactive_end
		  41,		// hsync_start
		   1,		// hsync_end
		 750,		// vtotal_size
		  26,		// vactive_start
		 746,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   1,		// pre_div_vclk (P)
		  11,		// post_div_vclk (M)
		   4,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		   1,		// pre_div_vclk (P)
		   2,		// input_div_vclk (N)
		  77,		// post_div_vclk (M)
		   1,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#else
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	{	// 1280x720p24
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		3300,	// htotal_size
		 261,		// hactive_start
		1541,		// hactive_end
		  41,		// hsync_start
		   1,		// hsync_end
		 750,		// vtotal_size
		  26,		// vactive_start
		 746,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   6,		// pre_div_vclk (P)
		  66,		// post_div_vclk (M)
		   5,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   5,		// input_div_vclk (N)
		 176,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   2,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   6,		// axclk_gen_div_s (S)
		   4,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   6,		// axclk_gen_div_s (S)
		   9,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},

	{	// 1920x1080i60
		  20,		// hdpulse_pos
		   1,		// vdpulse_pos
		 564,		// vdpulse_pos_bot
		2200,		// htotal_size
		 193,		// hactive_start
		2113,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		  21,		// vactive_start
		 561,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		 584,		// vactive_start_bot
		1124,		// vactive_end_bot
		 568,		// vsync_start_bot
		 563,		// vsync_end_bot
		1101,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   1,		// pre_div_vclk (P)
		  11,		// post_div_vclk (M)
		   4,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		   1,		// pre_div_vclk (P)
		   2,		// input_div_vclk (N)
		  77,		// post_div_vclk (M)
		   1,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#else
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	
#if defined(__USE_59Hz_OUTPUT__)
	{	// 1920x1080i59.94
		  20,		// hdpulse_pos
		   1,		// vdpulse_pos
		 564,		// vdpulse_pos_bot
		2200,		// htotal_size
		 193,		// hactive_start
		2113,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		  21,		// vactive_start
		 561,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		 584,		// vactive_start_bot
		1124,		// vactive_end_bot
		 568,		// vsync_start_bot
		 563,		// vsync_end_bot
		1101,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   91,		// pre_div_vclk (P)
		  250,		// post_div_vclk (M)
		   1,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550) --- 74.1758Mhz with ext. osc(37.0879Mhz)
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  14,		// post_div_vclk (M)
		   0,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#elif defined(__USE_EXT_OSC_148M__)
		   1,		// pre_div_vclk (P)
		   4,		// input_div_vclk (N)
		  16,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#else
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  16,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  52		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		  23,		// axclk_gen_div_f (F)
		  50		// axclk_gen_div_t (T)
	#endif
	},
#endif	//#if defined(__USE_59Hz_OUTPUT__)
	
	{	// 1920x1080i50
		  20,		// hdpulse_pos
		   0,		// vdpulse_pos
		 563,		// vdpulse_pos_bot
		2640,		// htotal_size
		 193,		// hactive_start
		2113,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		  21,		// vactive_start
		 561,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		 584,		// vactive_start_bot
		1124,		// vactive_end_bot
		 568,		// vsync_start_bot
		 563,		// vsync_end_bot
		1321,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   1,		// pre_div_vclk (P)
		  11,		// post_div_vclk (M)
		   4,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		   1,		// pre_div_vclk (P)
		   2,		// input_div_vclk (N)
		  77,		// post_div_vclk (M)
		   1,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#else
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	{	// 1920x1080p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2200,		// htotal_size
#if defined(__USE_MDIN_i500__) && defined(__M33_V101__)	// i500
		 193+32,	// hactive_start
		 2113+32,	// hactive_end
#else
		 193,		// hactive_start
		2113,		// hactive_end
#endif
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		  42,		// vactive_start
		1122,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   1,		// pre_div_vclk (P)
		  11,		// post_div_vclk (M)
		   2,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		   1,		// pre_div_vclk (P)
		   2,		// input_div_vclk (N)
		  77,		// post_div_vclk (M)
		   0,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#else
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   2,		// axclk_gen_div_s (S)
		   6,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   2,		// axclk_gen_div_s (S)
		   8,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	
#if defined(__USE_59Hz_OUTPUT__)
	{	// 1920x1080p59.94
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2200,		// htotal_size
#if defined(__USE_MDIN_i500__) && defined(__M33_V101__)	// i500
		 193+32,	// hactive_start
		 2113+32,	// hactive_end
#else
		 193,		// hactive_start
		2113,		// hactive_end
#endif
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		  42,		// vactive_start
		1122,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   91,		// pre_div_vclk (P)
		  500,		// post_div_vclk (M)
		   1,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550) --- 148.3516Mhz with ext. osc(37.0879Mhz)
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  28,		// post_div_vclk (M)
		   0,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#elif defined(__USE_EXT_OSC_148M__)
		   1,		// pre_div_vclk (P)
		   4,		// input_div_vclk (N)
		  16,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#else
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  16,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   2,		// axclk_gen_div_s (S)
		  40,		// axclk_gen_div_f (F)
		  73		// axclk_gen_div_t (T)
	#else
		   2,		// axclk_gen_div_s (S)
		  46,		// axclk_gen_div_f (F)
		  63		// axclk_gen_div_t (T)
	#endif
	},
#endif	//#if defined(__USE_59Hz_OUTPUT__)
	
	{	// 1920x1080p50
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2640,		// htotal_size
		 193,		// hactive_start
		2113,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		  42,		// vactive_start
		1122,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   1,		// pre_div_vclk (P)
		  11,		// post_div_vclk (M)
		   2,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		   1,		// pre_div_vclk (P)
		   2,		// input_div_vclk (N)
		  77,		// post_div_vclk (M)
		   0,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#else
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   2,		// axclk_gen_div_s (S)
		   6,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   2,		// axclk_gen_div_s (S)
		   8,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	{	// 1920x1080p30
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2200,		// htotal_size
#if defined(__USE_MDIN_i500__) && defined(__M33_V101__)	// i500
		 193+32,	// hactive_start
		 2113+32,	// hactive_end
#else
		 193,		// hactive_start
		2113,		// hactive_end
#endif
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		  42,		// vactive_start
		1122,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   4,		// pre_div_vclk (P)
		  22,		// post_div_vclk (M)
		   2,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		   1,		// pre_div_vclk (P)
		   2,		// input_div_vclk (N)
		  77,		// post_div_vclk (M)
		   1,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#else
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	
#if defined(__USE_59Hz_OUTPUT__)
	{	// 1920x1080p29.97
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2200,		// htotal_size
#if defined(__USE_MDIN_i500__) && defined(__M33_V101__)	// i500
		 193+32,	// hactive_start
		 2113+32,	// hactive_end
#else
		 193,		// hactive_start
		2113,		// hactive_end
#endif
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		  42,		// vactive_start
		1122,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  91,		// pre_div_vclk (P)
		 250,		// post_div_vclk (M)
		   1,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550) --- 74.1758Mhz with ext. osc(37.0879Mhz)
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  14,		// post_div_vclk (M)
		   0,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#elif defined(__USE_EXT_OSC_148M__)
		   1,		// pre_div_vclk (P)
		   4,		// input_div_vclk (N)
		  16,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#else
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  16,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  52		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		  23,		// axclk_gen_div_f (F)
		  50		// axclk_gen_div_t (T)
	#endif
	},
#endif	//#if defined(__USE_59Hz_OUTPUT__)
	
	{	// 1920x1080p25
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2640,		// htotal_size
		 193,		// hactive_start
		2113,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		  42,		// vactive_start
		1122,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   4,		// pre_div_vclk (P)
		  22,		// post_div_vclk (M)
		   2,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		   1,		// pre_div_vclk (P)
		   2,		// input_div_vclk (N)
		  77,		// post_div_vclk (M)
		   1,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#else
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},

	{	// 1920x1080p24
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2750,		// htotal_size
		 193,		// hactive_start
		2113,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		  42,		// vactive_start
		1122,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   4,		// pre_div_vclk (P)
		  22,		// post_div_vclk (M)
		   2,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		   1,		// pre_div_vclk (P)
		   2,		// input_div_vclk (N)
		  77,		// post_div_vclk (M)
		   1,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#else
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	{	// 3840x2160p30(8M)
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		4400,		// htotal_size
		 385,		// hactive_start
		4225,		// hactive_end
		  89,		// hsync_start
		   1,		// hsync_end
		2250,		// vtotal_size
		  83,		// vactive_start
		2243,		// vactive_end
		  11,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  44,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)

#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
		
#if defined(__USE_59Hz_OUTPUT__)
	{	// 3840x2160p29.97(8M)
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		4400,		// htotal_size
		 385,		// hactive_start
		4225,		// hactive_end
		  89,		// hsync_start
		   1,		// hsync_end
		2250,		// vtotal_size
		  83,		// vactive_start
		2243,		// vactive_end
		  11,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
		   
	#if defined(__USE_EXT_OSC_148M__)
		   1,		// pre_div_vclk (P) ---  not test 
		   3,		// input_div_vclk (N)
		  24,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#else	
		   1,		// pre_div_vclk (P) --- 296.7032Mhz with ext. osc(37.0879Mhz)
		   3,		// input_div_vclk (N)
		  96,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
	
	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
#endif
		
	{	// 3840x2160p25(8M)
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		5280,		// htotal_size
		 385,		// hactive_start
		4225,		// hactive_end
		  89,		// hsync_start
		   1,		// hsync_end
		2250,		// vtotal_size
		  83,		// vactive_start
		2243,		// vactive_end
		  11,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  44,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
		
	{	// 3840x2160p24(8M)
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		5500,		// htotal_size
		 385,		// hactive_start
		4225,		// hactive_end
		  89,		// hsync_start
		   1,		// hsync_end
		2250,		// vtotal_size
		  83,		// vactive_start
		2243,		// vactive_end
		  11,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  44,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	
	{	// 3840x2160p15
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		4400,		// htotal_size
		 385,		// hactive_start
		4225,		// hactive_end
		  89,		// hsync_start
		   1,		// hsync_end
		2250,		// vtotal_size
		  83,		// vactive_start
		2243,		// vactive_end
		  11,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)

#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	
{	// 3840x2160p12
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		5280,		// htotal_size
		 385,		// hactive_start
		4225,		// hactive_end
		  89,		// hsync_start
		   1,		// hsync_end
		2250,		// vtotal_size
		  83,		// vactive_start
		2243,		// vactive_end
		  11,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
		
	{	// 4096x2160p30(8M)
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		4400,		// htotal_size
		 217,		// hactive_start
		4313,		// hactive_end
		  89,		// hsync_start
		   1,		// hsync_end
		2250,		// vtotal_size
		  83,		// vactive_start
		2243,		// vactive_end
		  11,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  44,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},		
		
#if defined(__USE_59Hz_OUTPUT__)
	{	// 4096x2160p29.97(8M)
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		4400,		// htotal_size
		 217,		// hactive_start
		4313,		// hactive_end
		  89,		// hsync_start
		   1,		// hsync_end
		2250,		// vtotal_size
		  83,		// vactive_start
		2243,		// vactive_end
		  11,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
		   
	#if defined(__USE_EXT_OSC_148M__)
		   1,		// pre_div_vclk (P) ---   not test 
		   3,		// input_div_vclk (N)
		  24,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#else	
		   1,		// pre_div_vclk (P) --- 296.7032Mhz with ext. osc(37.0879Mhz)
		   3,		// input_div_vclk (N)
		  96,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
	
	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
#endif
	
	{	// 4096x2160p25(8M)
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		5280,		// htotal_size
		 217,		// hactive_start
		4313,		// hactive_end
		  89,		// hsync_start
		   1,		// hsync_end
		2250,		// vtotal_size
		  83,		// vactive_start
		2243,		// vactive_end
		  11,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  44,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},		
		
	{	// 4096x2160p24(8M)
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		5500,		// htotal_size
		 385,		// hactive_start
		4481,		// hactive_end
		  89,		// hsync_start
		   1,		// hsync_end
		2250,		// vtotal_size
		  83,		// vactive_start
		2243,		// vactive_end
		  11,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  44,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},

	{	// 2560x1440p30
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		3300,		// htotal_size
		 193,		// hactive_start
		2753,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1500,		// vtotal_size
		  42,		// vactive_start
		1482,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
			
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)

	#if defined(SYSTEM_USE_MCLK189)
		   2,		// axclk_gen_div_s (S)
		   6,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   2,		// axclk_gen_div_s (S)
		   8,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},	
		
	{	// 2560x1440p25
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		3960,		// htotal_size
		 193,		// hactive_start
		2753,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1500,		// vtotal_size
		  42,		// vactive_start
		1482,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
			
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)

	#if defined(SYSTEM_USE_MCLK189)
		   2,		// axclk_gen_div_s (S)
		   6,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   2,		// axclk_gen_div_s (S)
		   8,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},	
	
#if defined(__USE_4K60_INOUT__)
	{	// (3840/2)x2160p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2200,		// htotal_size
		 193,		// hactive_start
		2113,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		2250,		// vtotal_size
		  83,		// vactive_start
		2243,		// vactive_end
		  11,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  44,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)

#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
#endif
	},
		
#if defined(__USE_59Hz_OUTPUT__)
	{	// (3840/2)x2160p59.94
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2200,		// htotal_size
		 193,		// hactive_start
		2113,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		2250,		// vtotal_size
		  83,		// vactive_start
		2243,		// vactive_end
		  11,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	#if defined(__USE_EXT_OSC_148M__)
		   1,		// pre_div_vclk (P) ---   not test 
		   3,		// input_div_vclk (N)
		  24,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#else
		   1,		// pre_div_vclk (P) --- 296.7032Mhz with ext. osc(37.0879Mhz)
		   3,		// input_div_vclk (N)
		  96,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
#endif	// __USE_59Hz_OUTPUT__
	
	{	// (3840/2)x2160p50
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2640,		// htotal_size
		 193,		// hactive_start
		2113,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		2250,		// vtotal_size
		  83,		// vactive_start
		2243,		// vactive_end
		  11,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  44,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
		
	{	// (4096/2)x2160p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2200,		// htotal_size
		 109,		// hactive_start
		2157,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		2250,		// vtotal_size
		  83,		// vactive_start
		2243,		// vactive_end
		  11,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  44,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)

#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
#endif
	},
		
#if defined(__USE_59Hz_OUTPUT__)
	{	// (4096/2)x2160p59.94
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2200,		// htotal_size
		 109,		// hactive_start
		2157,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		2250,		// vtotal_size
		  83,		// vactive_start
		2243,		// vactive_end
		  11,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos

	#if defined(__USE_EXT_OSC_148M__)
		   1,		// pre_div_vclk (P) ---  not test 
		   3,		// input_div_vclk (N)
		  24,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#else
		   1,		// pre_div_vclk (P) --- 296.7032Mhz with ext. osc(37.0879Mhz)
		   3,		// input_div_vclk (N)
		  96,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
	
	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
#endif	// __USE_59Hz_OUTPUT__
	
	{	// (4096/2)x2160p50
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2640,		// htotal_size
		 109,		// hactive_start
		2157,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		2250,		// vtotal_size
		  83,		// vactive_start
		2243,		// vactive_end
		  11,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  44,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
#endif
#endif	//#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550

	
	// 120hz format
	{	// 1280x720p120
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1650,		// htotal_size
		 261,		// hactive_start
		1541,		// hactive_end
		  41,		// hsync_start
		   1,		// hsync_end
		 750,		// vtotal_size
		  26,		// vactive_start
		 746,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   1,		// pre_div_vclk (P)
		  11,		// post_div_vclk (M)
		   2,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		   1,		// pre_div_vclk (P)
		   2,		// input_div_vclk (N)
		  77,		// post_div_vclk (M)
		   0,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#else
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	
	{	// 1280x720p100
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1980,		// htotal_size
		 261,		// hactive_start
		1541,		// hactive_end
		  41,		// hsync_start
		   1,		// hsync_end
		 750,		// vtotal_size
		  26,		// vactive_start
		 746,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   1,		// pre_div_vclk (P)
		  11,		// post_div_vclk (M)
		   2,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		   1,		// pre_div_vclk (P)
		   2,		// input_div_vclk (N)
		  77,		// post_div_vclk (M)
		   0,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)	
	#else
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
	#endif
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	{	// 1920x1080p120
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2200,		// htotal_size
#if defined(__USE_MDIN_i500__) && defined(__M33_V101__)	// i500
		 193+32,	// hactive_start
		 2113+32,	// hactive_end
#else
		 193,		// hactive_start
		2113,		// hactive_end
#endif
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		  42,		// vactive_start
		1122,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  44,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)

	#if defined(SYSTEM_USE_MCLK189)
		   2,		// axclk_gen_div_s (S)
		   6,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   2,		// axclk_gen_div_s (S)
		   8,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},		

	{	// 1920x1080p100
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2640,		// htotal_size
		 193,		// hactive_start
		2113,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		  42,		// vactive_start
		1122,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
		   
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  44,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)

	#if defined(SYSTEM_USE_MCLK189)
		   2,		// axclk_gen_div_s (S)
		   6,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   2,		// axclk_gen_div_s (S)
		   8,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},		
#endif	// #if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)


	// PC
	{	// 640x480p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		 800,		// htotal_size
		 145,		// hactive_start
		 785,		// hactive_end
		  97,		// hsync_start
		   1,		// hsync_end
		 525,		// vtotal_size
		  36,		// vactive_start
		 516,		// vactive_end
		   3,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  26,		// pre_div_vclk (P)
		  97,		// post_div_vclk (M)
		   4,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550) --- 25.171875Mhz
		   1,		// pre_div_vclk (P)
		   8,		// input_div_vclk (N)
		 179,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   3,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		  15,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  97		// axclk_gen_div_t (T)
	#else
		  16,		// axclk_gen_div_s (S)
		   8,		// axclk_gen_div_f (F)
		  97		// axclk_gen_div_t (T)
	#endif
	},
	{	// 640x480p75
		  20,		// hdpulse_pos
		   6,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		 840,		// htotal_size
		 185,		// hactive_start
		 825,		// hactive_end
		  65,		// hsync_start
		   1,		// hsync_end
		 500,		// vtotal_size
		  19,		// vactive_start
		 499,		// vactive_end
		   3,		// vsync_start
		   0,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  27,		// pre_div_vclk (P)
		 126,		// post_div_vclk (M)
		   4,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   3,		// input_div_vclk (N)
		  56,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   2,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		  11,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
	#else
		  12,		// axclk_gen_div_s (S)
		   6,		// axclk_gen_div_f (F)
		   7		// axclk_gen_div_t (T)
	#endif
	},
	{	// 800x600p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1056,		// htotal_size
		 217,		// hactive_start
		1017,		// hactive_end
		 129,		// hsync_start
		   1,		// hsync_end
		 628,		// vtotal_size
		  27,		// vactive_start
		 627,		// vactive_end
		   4,		// vsync_start
		   0,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  27,		// pre_div_vclk (P)
		 160,		// post_div_vclk (M)
		   4,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550) --- 39.99375Mhz
		   1,		// pre_div_vclk (P)
		   5,		// input_div_vclk (N)
		 237,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   4,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   9,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   2		// axclk_gen_div_t (T)
	#else
		  10,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   8		// axclk_gen_div_t (T)
	#endif
	},
	{	// 800x600p75
		  20,		// hdpulse_pos
		   5,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1056,		// htotal_size
		 241,		// hactive_start
		1041,		// hactive_end
		  81,		// hsync_start
		   1,		// hsync_end
		 625,		// vtotal_size
		  24,		// vactive_start
		 624,		// vactive_end
		   3,		// vsync_start
		   0,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  24,		// pre_div_vclk (P)
		 176,		// post_div_vclk (M)
		   4,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   3,		// input_div_vclk (N)
		  88,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   2,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   7,		// axclk_gen_div_s (S)
		   7,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   8,		// axclk_gen_div_s (S)
		   2,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	{	// 1024x768p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1344,		// htotal_size
		 297,		// hactive_start
		1321,		// hactive_end
		 137,		// hsync_start
		   1,		// hsync_end
		 806,		// vtotal_size
		  36,		// vactive_start
		 804,		// vactive_end
		   7,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  27,		// pre_div_vclk (P)
		 130,		// post_div_vclk (M)
		   2,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550) --- 64.96875Mhz
		   1,		// pre_div_vclk (P)
		   2,		// input_div_vclk (N)
		  77,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   2,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   4,		// axclk_gen_div_f (F)
		   5		// axclk_gen_div_t (T)
	#else
		   6,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   4		// axclk_gen_div_t (T)
	#endif
	},
	{	// 1024x768p75
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1312,		// htotal_size
		 273,		// hactive_start
		1297,		// hactive_end
		  97,		// hsync_start
		   1,		// hsync_end
		 800,		// vtotal_size
		  31,		// vactive_start
		 799,		// vactive_end
		   3,		// vsync_start
		   0,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  24,		// pre_div_vclk (P)
		 140,		// post_div_vclk (M)
		   2,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550) --- 78.75Mhz
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  35,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   3,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   4,		// axclk_gen_div_s (S)
		   4,		// axclk_gen_div_f (F)
		   5		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   7		// axclk_gen_div_t (T)
	#endif
	},
	{	// 1280x960p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1712,		// htotal_size
		 353,		// hactive_start
		1633,		// hactive_end
		 137,		// hsync_start
		   1,		// hsync_end
		 994,		// vtotal_size
		  33,		// vactive_start
		 993,		// vactive_end
		   3,		// vsync_start
		   0,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  16,		// pre_div_vclk (P)
		 121,		// post_div_vclk (M)
		   2,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550) --- 102.09375Mhz
		   1,		// pre_div_vclk (P)
		   4,		// input_div_vclk (N)
		 121,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   3,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   2		// axclk_gen_div_t (T)
	#else
		   3,		// axclk_gen_div_s (S)
		   3,		// axclk_gen_div_f (F)
		   4		// axclk_gen_div_t (T)
	#endif
	},
	{	// 1280x1024p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1688,		// htotal_size
		 361,		// hactive_start
		1641,		// hactive_end
		 113,		// hsync_start
		   1,		// hsync_end
		1066,		// vtotal_size
		  41,		// vactive_start
		1065,		// vactive_end
		   3,		// vsync_start
		   0,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   4,		// pre_div_vclk (P)
		  16,		// post_div_vclk (M)
		   1,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  32,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   3,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   2		// axclk_gen_div_t (T)
	#else
		   3,		// axclk_gen_div_s (S)
		   3,		// axclk_gen_div_f (F)
		   4		// axclk_gen_div_t (T)
	#endif
	},
	{	// 1280x1024p75
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1688,		// htotal_size
		 393,		// hactive_start
		1673,		// hactive_end
		 145,		// hsync_start
		   1,		// hsync_end
		1066,		// vtotal_size
		  41,		// vactive_start
		1065,		// vactive_end
		   3,		// vsync_start
		   0,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   4,		// pre_div_vclk (P)
		  20,		// post_div_vclk (M)
		   1,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  20,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   2,		// axclk_gen_div_s (S)
		   4,		// axclk_gen_div_f (F)
		   5		// axclk_gen_div_t (T)
	#else
		   2,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
	#endif
	},
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	{	// 1280x1024p100
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1760,		// htotal_size
		 385,		// hactive_start
		1665,		// hactive_end
		 145,		// hsync_start
		   1,		// hsync_end
		1085,		// vtotal_size
		  60,		// vactive_start
		1084,		// vactive_end
		   3,		// vsync_start
		   0,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   1,		// pre_div_vclk (P)
		   7,		// input_div_vclk (N)
		 198,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)

	#if defined(SYSTEM_USE_MCLK189)
		   2,		// axclk_gen_div_s (S)
		   4,		// axclk_gen_div_f (F)
		   5		// axclk_gen_div_t (T)
	#else
		   2,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   1		// axclk_gen_div_t (T)
	#endif
	},
#endif
	{	// 1360x768p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1792,		// htotal_size
		 369,		// hactive_start
		1729,		// hactive_end
		 113,		// hsync_start
		   1,		// hsync_end
		 795,		// vtotal_size
		  25,		// vactive_start
		 793,		// vactive_end
		   7,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
				
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  27,		// pre_div_vclk (P)
		 171,		// post_div_vclk (M)
		   2,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   3,		// input_div_vclk (N)
		  76,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   4,		// axclk_gen_div_s (S)
		   8,		// axclk_gen_div_f (F)
		  19		// axclk_gen_div_t (T)
	#else
		   4,		// axclk_gen_div_s (S)
		  14,		// axclk_gen_div_f (F)
		  19		// axclk_gen_div_t (T)
	#endif
	},
	{	// 1366x768p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1792,		// htotal_size
		 357,		// hactive_start
		1723,		// hactive_end
		 144,		// hsync_start
		   1,		// hsync_end
		 798,		// vtotal_size
		  28,		// vactive_start
		 796,		// vactive_end
		   4,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
				
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  36,		// pre_div_vclk (P)
		 343,		// post_div_vclk (M)
		   3,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   3,		// input_div_vclk (N)
		  76,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)	// not exact... by sunny 120622
		   4,		// axclk_gen_div_s (S)
		   8,		// axclk_gen_div_f (F)
		  19		// axclk_gen_div_t (T)
	#else
		   4,		// axclk_gen_div_s (S)
		  14,		// axclk_gen_div_f (F)
		  19		// axclk_gen_div_t (T)
	#endif
	},
	{	// 1400x1050p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1880,		// htotal_size
		 393,		// hactive_start
		1793,		// hactive_end
		 153,		// hsync_start
		   1,		// hsync_end
		 1087,		// vtotal_size
		  36,		// vactive_start
		 1086,		// vactive_end
		   3,		// vsync_start
		   0,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
				
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  54,		// pre_div_vclk (P)
		 487,		// post_div_vclk (M)
		   2,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550) --- 122.625Mhz
		   1,		// pre_div_vclk (P)
		   3,		// input_div_vclk (N)
		 109,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   2,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)	// not exact... by sunny 120622
		   4,		// axclk_gen_div_s (S)
		   8,		// axclk_gen_div_f (F)
		  19		// axclk_gen_div_t (T)
	#else
		   4,		// axclk_gen_div_s (S)
		  14,		// axclk_gen_div_f (F)
		  19		// axclk_gen_div_t (T)
	#endif
	},
	{	// 1600x1200p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2160,		// htotal_size
		 497,		// hactive_start
		2097,		// hactive_end
		 193,		// hsync_start
		   1,		// hsync_end
		1250,		// vtotal_size
		  49,		// vactive_start
		1249,		// vactive_end
		   3,		// vsync_start
		   0,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   4,		// pre_div_vclk (P)
		  24,		// post_div_vclk (M)
		   1,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  24,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   2,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   3		// axclk_gen_div_t (T)
	#else
		   2,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		   2		// axclk_gen_div_t (T)
	#endif
	},

	{	// 1440x900p60
		  20,		// hdpulse_pos	
		  10,		// vdpulse_pos	
		   0,		// vdpulse_pos_bot	
		1904,		// htotal_size	
		 385,		// hactive_start	
		1825,		// hactive_end	
		 153,		// hsync_start	
		   1,		// hsync_end	
		 934,		// vtotal_size	
		  32,		// vactive_start	
		 932,		// vactive_end	
		   7,		// vsync_start	
		   1,		// vsync_end	
		   0,		// vactive_start_bot	
		   0,		// vactive_end_bot	
		   0,		// vsync_start_bot	
		   0,		// vsync_end_bot	
		   0,		// vsync_bot_fld_pos	
					
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  54,		// pre_div_vclk (P)	
		 213,		// post_div_vclk (M)	
		   1,		// post_scale_vclk (S)	
#else 								// MDIN-i51X (i510, i540, i550) --- 106.5Mhz
		   1,		// pre_div_vclk (P)
		   3,		// input_div_vclk (N)
		  71,		// post_div_vclk (M)
		   1,		// output_div_vclk (OD)
		   3,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   3,		// axclk_gen_div_s (S)
		  39,		// axclk_gen_div_f (F)
		  71		// axclk_gen_div_t (T)
	#else
		   3,		// axclk_gen_div_s (S)
		  57,		// axclk_gen_div_f (F)
		  71		// axclk_gen_div_t (T)
	#endif
	},
	{	// 1440x900p75
		  20,		// hdpulse_pos	
		  10,		// vdpulse_pos	
		   0,		// vdpulse_pos_bot	
		1936,		// htotal_size	
		 401,		// hactive_start	
		1841,		// hactive_end	
		 153,		// hsync_start	
		   1,		// hsync_end	
		 942,		// vtotal_size	
		  40,		// vactive_start	
		 940,		// vactive_end	
		   7,		// vsync_start	
		   1,		// vsync_end	
		   0,		// vactive_start_bot	
		   0,		// vactive_end_bot	
		   0,		// vsync_start_bot	
		   0,		// vsync_end_bot	
		   0,		// vsync_bot_fld_pos	
					
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  48,		// pre_div_vclk (P)	
		 243,		// post_div_vclk (M)	
		   1,		// post_scale_vclk (S)	
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   4,		// input_div_vclk (N)
		  81,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   2,		// axclk_gen_div_s (S)
		  62,		// axclk_gen_div_f (F)
		  81		// axclk_gen_div_t (T)
	#else
		   2,		// axclk_gen_div_s (S)
		  26,		// axclk_gen_div_f (F)
		  27		// axclk_gen_div_t (T)
	#endif
	},
	{	// 1680x1050p60
		  20,		// hdpulse_pos	
		  10,		// vdpulse_pos	
		   0,		// vdpulse_pos_bot	
		2240,		// htotal_size	
		 457,		// hactive_start	
		2137,		// hactive_end	
		 177,		// hsync_start	
		   1,		// hsync_end	
		1089,		// vtotal_size	
		  37,		// vactive_start	
		1087,		// vactive_end	
		   7,		// vsync_start	
		   1,		// vsync_end	
		   0,		// vactive_start_bot	
		   0,		// vactive_end_bot	
		   0,		// vsync_start_bot	
		   0,		// vsync_end_bot	
		   0,		// vsync_bot_fld_pos	
					
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  24,		// pre_div_vclk (P)	
		 130,		// post_div_vclk (M)	
		   1,		// post_scale_vclk (S)	
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   3,		// input_div_vclk (N)
		  65,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   2,		// axclk_gen_div_s (S)
		  38,		// axclk_gen_div_f (F)
		  65		// axclk_gen_div_t (T)
	#else
		   2,		// axclk_gen_div_s (S)
		  10,		// axclk_gen_div_f (F)
		  13		// axclk_gen_div_t (T)
	#endif
	},

	{
		// 1680x1050p60 Reduced Blanking
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1840,		// htotal_size
		 113,		// hactive_start
		1793,		// hactive_end
		  33,		// hsync_start
		   1,		// hsync_end
		1080,		// vtotal_size
		  28,		// vactive_start
		1078,		// vactive_end
		   7,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
		 
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  27,		// pre_div_vclk (P)
		 119,		// post_div_vclk (M)
		   1,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   9,		// input_div_vclk (N)
		 238,		// post_div_vclk (M)
		   1,		// output_div_vclk (OD)
		   3,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   3,		// axclk_gen_div_s (S)
		   3,		// axclk_gen_div_f (F)
		  17		// axclk_gen_div_t (T)
	#else
		   3,		// axclk_gen_div_s (S)
		  25,		// axclk_gen_div_f (F)
		  62		// axclk_gen_div_t (T)
	#endif
	},
	{
		// 1920x1080p60 Reduced Blanking
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2080,		// htotal_size
		 113,		// hactive_start
		2033,		// hactive_end
		  33,		// hsync_start
		   1,		// hsync_end
		1111,		// vtotal_size
		  30,		// vactive_start
		1110,		// vactive_end
		   7,		// vsync_start
		   2,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
		 
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  54,		// pre_div_vclk (P)
		 277,		// post_div_vclk (M)
		   1,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550) -- 138.375Mhz
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  41,		// post_div_vclk (M)
		   1,		// output_div_vclk (OD)
		   4,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   2,		// axclk_gen_div_s (S)
		  35,		// axclk_gen_div_f (F)
		  48		// axclk_gen_div_t (T)
	#else
		   2,		// axclk_gen_div_s (S)
		  61,		// axclk_gen_div_f (F)
		  66		// axclk_gen_div_t (T)
	#endif
	},
	{
		// 1920x1200p60 Reduced Blanking
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2080,		// htotal_size
		 113,		// hactive_start
		2033,		// hactive_end
		  33,		// hsync_start
		   1,		// hsync_end
		1235,		// vtotal_size
		  33,		// vactive_start
		1233,		// vactive_end
		   7,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
		 
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		  27,		// pre_div_vclk (P)
		 154,		// post_div_vclk (M)
		   1,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550) -- 154.02273Mhz
		   1,		// pre_div_vclk (P)
		  11,		// input_div_vclk (N)
		 251,		// post_div_vclk (M)
		   0,		// output_div_vclk (OD)
		   4,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   2,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   2,		// axclk_gen_div_s (S)
		  17,		// axclk_gen_div_f (F)
		  27		// axclk_gen_div_t (T)
	#endif
	},
	
	
	// SENSOR(for sensor in/out 1:1 mode)
	{	// 2048x1536p30
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2250,		// htotal_size
		 193,		// hactive_start
		2241,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1600,		// vtotal_size
		  42,		// vactive_start
		1578,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   3,		// pre_div_vclk (P)
		  12,		// post_div_vclk (M)
		   1,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  32,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},

	{	 // 2048x1536p25
			 20,		// hdpulse_pos
			 10,		// vdpulse_pos
			  0,		// vdpulse_pos_bot
		   2700,		// htotal_size
			193,		// hactive_start
		   2241,		// hactive_end
			 45,		// hsync_start
			  1,		// hsync_end
		   1600,		// vtotal_size
			 42,		// vactive_start
		   1578,		// vactive_end
			  6,		// vsync_start
			  1,		// vsync_end
			  0,		// vactive_start_bot
			  0,		// vactive_end_bot
			  0,		// vsync_start_bot
			  0,		// vsync_end_bot
			  0,		// vsync_bot_fld_pos
	   
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   3,		// pre_div_vclk (P)
		  12,		// post_div_vclk (M)
		   1,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  32,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
#endif
	
#if defined(SYSTEM_USE_MCLK189)
			  5,		// axclk_gen_div_s (S)
			  1,		// axclk_gen_div_f (F)
			 11 	   // axclk_gen_div_t (T)
#else
			  5,		// axclk_gen_div_s (S)
			  5,		// axclk_gen_div_f (F)
			 11 	   // axclk_gen_div_t (T)
#endif
	},
	
	{	// 3072x1728p15(5M)
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		3328,		// htotal_size
		 193,		// hactive_start
		3265,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1980,		// vtotal_size
		  42,		// vactive_start
		1770,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   6,		// pre_div_vclk (P)
		  22,		// post_div_vclk (M)
		   1,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   3,		// input_div_vclk (N)
		  88,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	
	{	// 2592x1944p15(5M)
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2669,		// htotal_size
		 193,		// hactive_start
		2785,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		2475,		// vtotal_size
		  42,		// vactive_start
		1986,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   6,		// pre_div_vclk (P)
		  22,		// post_div_vclk (M)
		   1,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   3,		// input_div_vclk (N)
		  88,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
		
	// DIS(aux only)
	{	// DIS, main out:2M,  aux out:2M(pip 1/16))
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2200,		// htotal_size
		1633,		// hactive_start
		2113,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		 852,		// vactive_start
		1122,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   1,		// pre_div_vclk (P)
		  11,		// post_div_vclk (M)
		   2,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   2,		// axclk_gen_div_s (S)
		   6,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   2,		// axclk_gen_div_s (S)
		   8,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	{	// DIS, main out:3M,  aux out:3M(pip 1/16))
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2250,		// htotal_size
		1729,		// hactive_start
		2241,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1600,		// vtotal_size
		1194,		// vactive_start
		1578,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   3,		// pre_div_vclk (P)	//108M
		  12,		// post_div_vclk (M)
		   1,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  32,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	
	{	// DIS, main out:8M,  aux out:2M(pip 1/16))
		20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1650,		// htotal_size
		 261,		// hactive_start
		1541,		// hactive_end
		  41,		// hsync_start
		   1,		// hsync_end
		 750,		// vtotal_size
		  26,		// vactive_start
		 746,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   1,		// pre_div_vclk (P)
		  11,		// post_div_vclk (M)
		   4,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   3,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   5,		// axclk_gen_div_s (S)
		   1,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   5,		// axclk_gen_div_s (S)
		   5,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},

	{	// DIS, main out:8M->2048x1208crop,  aux out:(512x302 pip 1/16))
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2200,		// htotal_size
		1633,		// hactive_start
		2113,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		 852,		// vactive_start
		1122,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		   1,		// pre_div_vclk (P)
		  11,		// post_div_vclk (M)
		   2,		// post_scale_vclk (S)
#else 								// MDIN-i51X (i510, i540, i550)
		   1,		// pre_div_vclk (P)
		   1,		// input_div_vclk (N)
		  22,		// post_div_vclk (M)
		   2,		// output_div_vclk (OD)
		   1,		// post_scale_vclk (S)
#endif

	#if defined(SYSTEM_USE_MCLK189)
		   2,		// axclk_gen_div_s (S)
		   6,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#else
		   2,		// axclk_gen_div_s (S)
		   8,		// axclk_gen_div_f (F)
		  11		// axclk_gen_div_t (T)
	#endif
	},
	
// USB
#if defined(__USE_USB_VOUT__)
// hd,  vd, vdb,htot, has, hae, hss, hse,vtot, vas, vae, vss, vse,vasb,vaeb,vssb,vseb,vsbf,  p,  n,  m, od,  s
{  20,  10,   0,1030, 123, 843,  63,   1, 525,  43, 523,  13,   7,   0,   0,   0,   0,   0,  1,  1, 32,  3,  4}, // 720x480p50    -- 27Mhz
{  20,  10,   0,1716, 123, 843,  63,   1, 525,  43, 523,  13,   7,   0,   0,   0,   0,   0,  1,  1, 32,  3,  4}, // 720x480p30    -- 27Mhz
{  20,  10,   0,2145, 123, 843,  63,   1, 525,  43, 523,  13,   7,   0,   0,   0,   0,   0,  1,  1, 32,  3,  4}, // 720x480p24    -- 27Mhz

{  20,  10,   0,1373, 123,1083,  63,   1, 525,  43, 523,  13,   7,   0,   0,   0,   0,   0,  1,  3,128,  3,  4}, // 960x480p50    -- 36Mhz
{  20,  10,   0,2288, 123,1083,  63,   1, 525,  43, 523,  13,   7,   0,   0,   0,   0,   0,  1,  3,128,  3,  4}, // 960x480p30    -- 36Mhz
{  20,  10,   0,2860, 123,1083,  63,   1, 525,  43, 523,  13,   7,   0,   0,   0,   0,   0,  1,  3,128,  3,  4}, // 960x480p24    -- 36Mhz

{  20,  10,   0,4950, 261,1541,  41,   1, 750,  26, 746,   6,   1,   0,   0,   0,   0,   0,  1,  1, 22,  3,  1}, // 1280x720p20   -- 74.25Mhz
{  20,  10,   0,6600, 261,1541,  41,   1, 750,  26, 746,   6,   1,   0,   0,   0,   0,   0,  1,  1, 22,  3,  1}, // 1280x720p15   -- 74.25Mhz
{  20,  10,   0,4950, 261,1541,  41,   1, 750,  26, 746,   6,   1,   0,   0,   0,   0,   0,  1,  1, 22,  3,  2}, // 1280x720p10   -- 37.125Mhz
{  20,  10,   0,6600, 261,1541,  41,   1, 750,  26, 746,   6,   1,   0,   0,   0,   0,   0,  1,  1, 22,  3,  2}, // 1280x720p7.5  -- 37.125Mhz
{  20,  10,   0,4950, 261,1541,  41,   1, 750,  26, 746,   6,   1,   0,   0,   0,   0,   0,  1,  1, 22,  3,  4}, // 1280x720p5    -- 18.5625Mhz

{  20,  10,   0,3300, 193,2113,  45,   1,1125,  42,1122,   6,   1,   0,   0,   0,   0,   0,  1,  1, 22,  3,  1}, // 1920x1080p20  -- 74.25Mhz
{  20,  10,   0,4400, 193,2113,  45,   1,1125,  42,1122,   6,   1,   0,   0,   0,   0,   0,  1,  1, 22,  3,  1}, // 1920x1080p15  -- 74.25Mhz
{  20,  10,   0,3300, 193,2113,  45,   1,1125,  42,1122,   6,   1,   0,   0,   0,   0,   0,  1,  1, 22,  3,  2}, // 1920x1080p10  -- 37.125Mhz
{  20,  10,   0,4400, 193,2113,  45,   1,1125,  42,1122,   6,   1,   0,   0,   0,   0,   0,  1,  1, 22,  3,  2}, // 1920x1080p7.5 -- 37.125Mhz
{  20,  10,   0,3300, 193,2113,  45,   1,1125,  42,1122,   6,   1,   0,   0,   0,   0,   0,  1,  1, 22,  3,  4}, // 1920x1080p5   -- 18.5625Mhz

{  20,  10,   0,6600, 385,4225,  89,   1,2250,  83,2243,  11,   1,   0,   0,   0,   0,   0,  1,  1, 22,  2,  1}, // 3840x2160p10  -- 148.5Mhz
{  20,  10,   0,4400, 385,4225,  89,   1,2250,  83,2243,  11,   1,   0,   0,   0,   0,   0,  1,  1, 22,  2,  2}, // 3840x2160p7.5 -- 74.25Mhz
{  20,  10,   0,6600, 385,4225,  89,   1,2250,  83,2243,  11,   1,   0,   0,   0,   0,   0,  1,  1, 22,  2,  2}, // 3840x2160p5   -- 74.25Mhz
#endif	// __USE_USB_VOUT__

};

// default value for in-offset using HDMI
ROMDATA WORD defDVIRXOffset[][2]		= {
//	 Hgap  Vgap
	// TV
	{  59,   30},	// 720x480i60
	{  67,   38},	// 720x576i50
	{   0,    0},	// 960x480i60
	{   0,    0},	// 960x576i50
	{  60,   30},	// 720x480p60
	{  68,   39},	// 720x576p50
	{  60,   30},	// 960x480p60
	{  68,   39},	// 960x576p50
	{ 220,   20},	// 1280x720p60
#if defined(__USE_59Hz_OUTPUT__)
	{ 220,   20},	// 1280x720p59
#endif
	{ 220,   20},	// 1280x720p50
	{ 220,   20},	// 1280x720p30
	{ 220,   20},	// 1280x720p25
	{ 220,   20},	// 1280x720p24
	{ 148,   30},	// 1920x1080i60
#if defined(__USE_59Hz_OUTPUT__)
	{ 148,   30},	// 1920x1080i59
#endif
	{ 148,   30},	// 1920x1080i50
	{ 148,   36},	// 1920x1080p60
#if defined(__USE_59Hz_OUTPUT__)
	{ 148,   36},	// 1920x1080p59
#endif
	{ 148,   15},	// 1920x1080p50
	{ 148,   15},	// 1920x1080p30
#if defined(__USE_59Hz_OUTPUT__)
	{ 148,   15},	// 1920x1080p29
#endif
	{ 148,   15},	// 1920x1080p25
	{ 148,   15},	// 1920x1080p24
	
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	{ 296,   72},	// 3840x2160p30
#if defined(__USE_59Hz_OUTPUT__)
	{ 296,   72},	// 3840x2160p29
#endif
	{ 296,   72},	// 3840x2160p25
	{ 296,   72},	// 3840x2160p24
	{ 296,   72},	// 3840x2160p20
	{ 296,   72},	// 3840x2160p15
	{ 296,   72},	// 3840x2160p12
	{ 128,   72},	// 4096x2160p30
#if defined(__USE_59Hz_OUTPUT__)
	{ 128,   72},	// 4096x2160p29
#endif
	{ 128,   72},	// 4096x2160p25
	{ 296,   72},	// 4096x2160p24
	{   0,    0},	// 2560x1440p30
	{   0,    0},	// 2560x1440p25
#if defined(__USE_4K60_INOUT__)
	{   0,    0},	// (3840/2)x2160p60
#if defined(__USE_59Hz_OUTPUT__)
	{   0,    0},	// (3840/2)x2160p59
#endif
	{   0,    0},	// (3840/2)x2160p50
	{   0,    0},	// (4096/2)x2160p60
#if defined(__USE_59Hz_OUTPUT__)
	{   0,    0},	// (4096/2)x2160p59
#endif
	{   0,    0},	// (4096/2)x2160p50
#endif
	{   0,    0},	// 1920x1080p120 -- FHD 120fps
	{   0,    0},	// 1920x1080p100 -- FHD 100fps
#endif
	
	// PC
	{  48,   33},	// 640x480p60
	{  48,   33},	// 640x480p75
	{  88,   23},	// 800x600p60
	{  88,   23},	// 800x600p75
	{ 160,   29},	// 1024x768p60
	{ 160,   29},	// 1024x768p75
	{ 248,   38},	// 1280x960p60
	{ 248,   38},	// 1280x1024p60
	{ 248,   38},	// 1280x1024p75
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	{ 240,   57},	// 1280x1024p100
#endif
	
	{ 298,   17},	// 1360x768p60
	{ 298,   17},	// 1366x768p60
	{   0,    0},	// 1400x1050p60
	{   0,    0},	// 1600x1200p60

	// ETC
	{ 314,   30},	// 1440x900p60
	{ 314,   30},	// 1440x900p75
	{   0,    0},	// 1680x1050p60
	{   0,    0},	// 1680x1050pRB
	{   0,    0},	// 1920x1080pRB
	{   0,    0},	// 1920x1200pRB
	
	{ 258,   21},	// 1280x720pRGB

	{  48,   33},	// 640x480p72
	{ 133,   22},	// 800x600p56
	{  88,   23},	// 800x600p72
	{ 160,   29},	// 1024x768p70
	{ 317,   32},	// 1152x864p75
	{ 288,   36},	// 1280x800p60
	
	// SENSOR
	{   0,    0},	// 1368x1049p60 //IMX076(1368Hx1049V, Total:1392Hx1076V)
	{   0,    0},	// 2048x153650 //IMX265 (2048Hx1536V, Total:2000Hx1650V)	
	{  70,   56},	// 2048x1536p30 //IMX124 (2048Hx1536V, Total:2250Hx1600V)
	{ 520,   56},	// 2048x1536p25 //IMX124 (2048Hx1536V, Total:(2250Hx1.2)x1600V)
	{   0,    0},	// 2592x1536p15 //MT9P031(2592Hx1944V, Total:2752Hx2004V)
	{   0,    0},	// 3072x1728p30 //imx178(5m 16:9)
	{   0,    0},	// 3072x1728p25 //imx178(5m 16:9)
	
	{   0,    0},	// 2592x1944p30 //imx326(5M)
	{   0,    0},	// 2592x1944p15 //imx326(5M)
	{   0,    0},	// 3072x2160p30 //imx326(6M)
	{   0,    0},	// 2160x2160p30 //imx326(1:1 2160p)

};

// default value for srcVideo format
ROMDATA MISP_SRCVIDEO_ATTB defMISPSrcVideo[]		= {
//	 Htot					                   H/V Polarity & ScanType                                       Hact  Vact rate
	// TV
	{ 858, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_INTR,  720,  480, 60},	// 720x480i60
	{ 864, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_INTR,  704,  576, 50},	// 720x576i50
	{1144, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_INTR,  960,  480, 60},	// 960x480i60
	{1200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_INTR,  960,  576, 50},	// 960x576i50
	{ 858, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  720,  480, 60},	// 720x480p60
	{ 864, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  720,  576, 50},	// 720x576p50
	{1144, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  960,  480, 60},	// 960x480p60
	{1200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  960,  576, 50},	// 960x576p50
	{1650, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720, 60},	// 1280x720p60
#if defined(__USE_59Hz_OUTPUT__)
	{1650, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720, 59},	// 1280x720p59.94
#endif
	{1980, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720, 50},	// 1280x720p50
	{1650, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720, 30},	// 1280x720p30
	{1980, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720, 25},	// 1280x720p25
	{1650, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720, 24},	// 1280x720p24
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_INTR, 1920, 1080, 60},	// 1920x1080i60
#if defined(__USE_59Hz_OUTPUT__)
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_INTR, 1920, 1080, 59},	// 1920x1080i59.94
#endif
	{2640, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_INTR, 1920, 1080, 50},	// 1920x1080i50
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080, 60},	// 1920x1080p60
#if defined(__USE_59Hz_OUTPUT__)
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080, 59},	// 1920x1080p59.94
#endif
	{2640, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080, 50},	// 1920x1080p50
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080, 30},	// 1920x1080p30
#if defined(__USE_59Hz_OUTPUT__)
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080, 29},	// 1920x1080p29.97
#endif
	{2640, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080, 25},	// 1920x1080p25
	{2750, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080, 24},	// 1920x1080p24
	
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3840, 2160, 30},	// 3840x2160p30
#if defined(__USE_59Hz_OUTPUT__)
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3840, 2160, 29},	// 3840x2160p29.97
#endif
	{5280, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3840, 2160, 25},	// 3840x2160p25
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3840, 2160, 24},	// 3840x2160p24
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3840, 2160, 15},	// 3840x2160p15
	{5280, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3840, 2160, 12},	// 3840x2160p12
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 4096, 2160, 30},	// 4096x2160p30
#if defined(__USE_59Hz_OUTPUT__)
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 4096, 2160, 29},	// 3840x2160p29.97
#endif
	{5280, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 4096, 2160, 25},	// 4096x2160p25
	{5500, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 4096, 2160, 24},	// 4096x2160p24
	{3300, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2560, 1440,  30},	// 2560x1440p30 -- 4M
	{3960, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2560, 1440,  25},	// 2560x1440p25 -- 4M
#if defined(__USE_4K60_INOUT__)
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 2160,  60},	// (3840/2)x2160p60
#if defined(__USE_59Hz_OUTPUT__)
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 2160,  59},	// (3840/2)x2160p59
#endif
	{2640, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 2160,  50},	// (3840/2)x2160p50
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2048, 2160,  60},	// (4096/2)x2160p60
#if defined(__USE_59Hz_OUTPUT__)
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2048, 2160,  59},	// (4096/2)x2160p59
#endif
	{2640, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2048, 2160,  50},	// (4096/2)x2160p50
#endif
#endif	

	
	// 120hz format
	{1650, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720, 120},	// 1280x720p120
	{1980, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720, 100},	// 1280x720p100
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080, 120},	// 1920x1080p120
	{2640, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080, 100},	// 1920x1080p100
#endif
	//240hz format(About imx273, Re-check the HTOT value others)
	{1600, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720, 240},	// 1280x720p240
	{1920, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720, 200},	// 1280x720p200	
	
	// PC
	{ 800, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  640,  480, 60},	// 640x480p60
	{ 840, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  640,  480, 75},	// 640x480p75
	{1056, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG,  800,  600, 60},	// 800x600p60
	{1056, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG,  800,  600, 75},	// 800x600p75
	{1344, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG, 1024,  768, 60},	// 1024x768p60
	{1312, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1024,  768, 75},	// 1024x768p75
	{1712, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  960, 60},	// 1280x960p60
	{1688, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280, 1024, 60},	// 1280x1024p60
	{1688, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280, 1024, 75},	// 1280x1024p75
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	{1760, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280, 1024,100},	// 1280x1024p100
#endif
	{1792, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1360,  768, 60},	// 1360x768p60
	{1792, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1366,  768, 60},	// 1366x768p60
	{1864, MISP_NEGATIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1400, 1050, 60},	// 1400x1050p60
	{2160, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1600, 1200, 60},	// 1600x1200p60

	// ETC
	{1904, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1440,  900, 60},	// 1440x900p60
	{1936, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1440,  900, 75},	// 1440x900p75
	{2240, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1680, 1050, 60},	// 1680x1050p60	
	{1840, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG, 1680, 1050, 60},	// 1680x1050pRB	
	{2080, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080, 60},	// 1920x1080pRB	
	{2080, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1200, 60},	// 1920x1200pRB	
	
	{1664, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720, 60},	// 1280x720pRGB

	{ 832, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  640,  480, 72},	// 640x480p72
	{1024, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG,  800,  600, 56},	// 800x600p56
	{1040, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG,  800,  600, 72},	// 800x600p72
	{1328, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG, 1024,  768, 70},	// 1024x768p70
	{1600, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1152,  864, 75},	// 1152x864p75
	{1680, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  800, 60},	// 1280x800p60
	
	// SENSOR
	{1392, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1368, 1049, 60},	// 1368x1049p60 //imx076(1368Hx1049V, Total:1392Hx1076V)
	{2400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2048, 1536, 50},	// 2048x1536p50 //imx265 (2048Hx1536V, Total:2400Hx1650V)
#if defined (__USE_IMX124_OSC27Mhz__)
	{2250, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2048, 1536, 30},	// 2048x1536p30 //imx124 (2048Hx1536V, Total:2250Hx1600V)
	{2700, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2048, 1536, 25},	// 2048x1536p25 //imx124 (2048Hx1536V, Total:(2250Hx1.2)x1600V)
#else
	{4000, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2048, 1536, 30},	// 2048x1536p30 //imx124 (2048Hx1536V, Total:4000Hx1650V)
	{4800, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2048, 1536, 25},	// 2048x1536p25 //imx124 (2048Hx1536V, Total:(4000Hx1.2)x1650V)
#endif
	{2752, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2592, 1536, 15},	// 2592x1536p15 //MT9P031(2592Hx1944V, Total:2752Hx2004V)
	{3328, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3072, 1728, 30},	// 3072x1728p30 //imx178(5m 16:9)
	{3328, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3072, 1728, 25},	// 3072x1728p25 //imx178(5m 16:9)

	{2669, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2592, 1944, 30},	// 2592x1944p30	//imx326(5M)
	{2669, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2592, 1944, 15},	// 2592x1944p15	//imx326(5M)
	{4000, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3072, 2160, 30},	// 3072x2160p30	//imx326(6M)
	{4000, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2160, 2160, 30},	// 2160x2160p30	//imx326(1:1 2160p)
//	{2888, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2544, 1431, 30},	// 2560x1440p30	//ps1400k(4M temp for demo)
	
	{2888, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2560, 1440, 30},	// 2560x1440p30	//ps1400k(4M org)	
	
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3840, 2160, 25},	// 3840x2160p25 // for imx226
	{5500, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 4096, 2160, 20},	// 4096x2160p20 // for imx226
	{5664, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 4000, 3000, 22},	// 4000x3000p22 // for imx226	12M
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1440, 1080, 60},	// 1440x1080p60 //imx273(1456Hx1095V, Total:2200Hx1125V)	
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2048, 1208, 30},	// 2048x1208p30 //__USE_SENS_PANNING_MODE__ 3840-->2048
	{5280, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2048, 1208, 25},	// 2048x1208p25 //__USE_SENS_PANNING_MODE__	
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1344, 1088, 30},	// 1344x1088p30 //__USE_SENS_PANNING_MODE__ 1920-->1280
	{2640, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1344, 1088, 25},	// 1344x1088p25 //__USE_SENS_PANNING_MODE__	
};

// default value for outVideo format
ROMDATA MISP_OUTVIDEO_ATTB defMISPOutVideo[]		= {
//	 Htot					                   H/V Polarity & ScanType                                       Hact  Vact
	// TV
	{ 858, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_INTR,  720,  480},	// 720x480i60
	{ 864, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_INTR,  720,  576},	// 720x576i50
	{1144, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_INTR,  960,  480},	// 960x480i60
	{1200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_INTR,  960,  576},	// 960x576i50
	{ 858, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  720,  480},	// 720x480p60
	{ 864, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  720,  576},	// 720x576p50
	{1144, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  960,  480},	// 960x480p60
	{1200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  960,  576},	// 960x576p50
	{1650, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720},	// 1280x720p60
#if defined(__USE_59Hz_OUTPUT__)
	{1650, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720},	// 1280x720p59.94
#endif
	{1980, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720},	// 1280x720p50
	
	{3300, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720},	// 1280x720p30
	{3960, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720},	// 1280x720p25
	{3300, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720},	// 1280x720p24
	
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_INTR, 1920, 1080},	// 1920x1080i60
#if defined(__USE_59Hz_OUTPUT__)
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_INTR, 1920, 1080},	// 1920x1080i59.94
#endif
	{2640, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_INTR, 1920, 1080},	// 1920x1080i50
	
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p60
#if defined(__USE_59Hz_OUTPUT__)
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p59.94
#endif
	{2640, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p50
	
#if defined(__M33_V101__)
	{2200, MISP_NEGATIVE_HACT|MISP_NEGATIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p30
#if defined(__USE_59Hz_OUTPUT__)
	{2200, MISP_NEGATIVE_HACT|MISP_NEGATIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p29.97
#endif
#else
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p30
#if defined(__USE_59Hz_OUTPUT__)
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p29.97
#endif
#endif
	{2640, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p25
	{2750, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p24
	
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3840, 2160},	// 3840x2160p30
#if defined(__USE_59Hz_OUTPUT__)
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3840, 2160},	// 3840x2160p29.97
#endif
	{5280, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3840, 2160},	// 3840x2160p25
	{5500, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3840, 2160},	// 3840x2160p24
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3840, 2160},	// 3840x2160p15
	{5280, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3840, 2160},	// 3840x2160p12
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 4096, 2160},	// 4096x2160p30
#if defined(__USE_59Hz_OUTPUT__)
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 4096, 2160},	// 4096x2160p29.97
#endif
	{5280, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 4096, 2160},	// 4096x2160p25
	{5500, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 4096, 2160},	// 4096x2160p24
	{3300, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2560, 1440},	// 2560x1440p30 -- 4M
	{3960, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2560, 1440},	// 2560x1440p25 -- 4M
#if defined(__USE_4K60_INOUT__)
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 2160},	// (3840/2)x2160p60
#if defined(__USE_59Hz_OUTPUT__)
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 2160},	// (3840/2)x2160p59.94
#endif
	{2640, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 2160},	// (3840/2)x2160p50
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2048, 2160},	// (4096/2)x2160p60
#if defined(__USE_59Hz_OUTPUT__)
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2048, 2160},	// (4096/2)x2160p59.94
#endif
	{2640, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2048, 2160},	// (4096/2)x2160p50
#endif
#endif
	
	
	// 120hz format
	{1650, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720},	// 1280x720p120
	{1980, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720},	// 1280x720p100
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p120
	{2640, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p100
#endif
	
	
	// PC
	{ 800, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  640,  480},	// 640x480p60
	{ 840, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  640,  480},	// 640x480p75
	{1056, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG,  800,  600},	// 800x600p60
	{1056, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG,  800,  600},	// 800x600p75
	{1344, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG, 1024,  768},	// 1024x768p60
	{1312, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1024,  768},	// 1024x768p75
	{1712, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  960},	// 1280x960p60
	{1688, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280, 1024},	// 1280x1024p60
	{1688, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280, 1024},	// 1280x1024p75
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	{1760, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280, 1024},	// 1280x1024p100
#endif

	{1792, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1360,  768},	// 1360x768p60
	{1792, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1366,  768},	// 1366x768p60
	{1864, MISP_NEGATIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1400, 1050},	// 1400x1050p60
	{2160, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1600, 1200},	// 1600x1200p60

	// ETC
	{1904, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1440,  900},	// 1440x900p60
	{1936, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1440,  900},	// 1440x900p75
	{2240, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1680, 1050},	// 1680x1050p60	

	{1840, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG, 1680, 1050},	// 1680x1050pRB	
	{2080, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080},	// 1920x1080pRB	
	{2080, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1200},	// 1920x1200pRB	
	
	
	// SENSOR(for sensor in/out 1:1 mode)
	{2250, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2048, 1536},	// 2048x1536p30 -- imx124(3M)
	{2700, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2048, 1536},	// 2048x1536p25 -- imx124(3M)
	{3328, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3072, 1728},	// 3072x1728p15 -- imx178(5M 16:9), imx326(5M)
	{2669, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 2592, 1944},	// 2592x1944p15 -- imx326(5M)

	
	// DIS(aux only)
	{2200, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  480,  270},	// main out:2M,  aux out:2M(pip 1/16))
	{2250, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  512,  384},	// main out:3M,  aux out:3M(pip 1/16))
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  960,  540},	// main out:8M,  aux out:8M(pip 1/16))
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  512,  302},	// main out:8M-> 2048x1208crop,  aux out:2048x1208crop(pip 1/16))	
	
	// USB
#if defined(__USE_USB_VOUT__)
	{1030, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  720,  480},	// 720x480p50
	{1716, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  720,  480},	// 720x480p30
	{2145, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  720,  480},	// 720x480p24

	{1373, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  960,  480},	// 960x480p50
	{2288, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  960,  480},	// 960x480p30
	{2860, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_NEGATIVE_HSYNC|MISP_NEGATIVE_VSYNC|MISP_SCANTYPE_PROG,  960,  480},	// 960x480p24

	{4950, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720},	// 1280x720p20
	{6600, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720},	// 1280x720p15
	{4950, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720},	// 1280x720p10
	{6600, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720},	// 1280x720p7.5
	{4950, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1280,  720},	// 1280x720p5

	{3300, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p20
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p15
	{3300, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p10
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p7.5
	{3300, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p5

	{6600, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3840, 2160},	// 3840x2160p10
	{4400, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3840, 2160},	// 3840x2160p7.5
	{6600, MISP_POSITIVE_HACT|MISP_POSITIVE_VACT|MISP_POSITIVE_HSYNC|MISP_POSITIVE_VSYNC|MISP_SCANTYPE_PROG, 3840, 2160},	// 3840x2160p5
#endif	// __USE_USB_VOUT__

};

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

/*  FILE_END_HERE */
