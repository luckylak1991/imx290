//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	MISPCSC.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"misp100.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
// RGB to YCbCr for input CSC
ROMDATA MISP_CSCCTRL_INFO MISP_CscRGBtoYUV_SD_FullRange	= {
	{(WORD) 301,		// coef0    
	(WORD)   58,		// coef1    
	(WORD)  153,		// coef2    
	(WORD) -174,		// coef3    
	(WORD)  262,		// coef4    
	(WORD)  -88,		// coef5    
	(WORD) -219,		// coef6    
	(WORD)  -42,		// coef7    
	(WORD)  262},		// coef8    
	{(WORD)   0,		// in_bias0 
	(WORD)    0,		// in_bias1 
	(WORD)    0},		// in_bias2 
	{(WORD)   0,		// out_bias0
	(WORD)  512,		// out_bias1
	(WORD)  512},		// out_bias2
};

ROMDATA MISP_CSCCTRL_INFO MISP_CscRGBtoYUV_HD_FullRange	= {
	{(WORD) 366,		// coef0    
	(WORD)   37,		// coef1    
	(WORD)  109,		// coef2    
	(WORD) -202,		// coef3    
	(WORD)  262,		// coef4    
	(WORD)  -60,		// coef5    
	(WORD) -238,		// coef6    
	(WORD)  -24,		// coef7    
	(WORD)  262},		// coef8    
	{(WORD)   0,		// in_bias0 
	(WORD)    0,		// in_bias1 
	(WORD)    0},		// in_bias2 
	{(WORD)   0,		// out_bias0
	(WORD)  512,		// out_bias1
	(WORD)  512},		// out_bias2
};

ROMDATA MISP_CSCCTRL_INFO MISP_CscRGBtoYUV_SD_StdRange	= {
	{(WORD) 258,		// coef0
	(WORD)   50,		// coef1
	(WORD)  132,		// coef2
	(WORD) -149,		// coef3
	(WORD)  225,		// coef4
	(WORD)  -76,		// coef5
	(WORD) -188,		// coef6
	(WORD)  -36,		// coef7
	(WORD)  225},		// coef8
	{(WORD)   0,		// in_bias0
	(WORD)    0,		// in_bias1
	(WORD)    0},		// in_bias2
	{(WORD)   0,		// out_bias0	// 64 -> 0 for cvbs out brightness(same as bypass)
	(WORD)  512,		// out_bias1
	(WORD)  512},		// out_bias2
};

ROMDATA MISP_CSCCTRL_INFO MISP_CscRGBtoYUV_HD_StdRange	= {
	{(WORD) 314,		// coef0    
	(WORD)   32,		// coef1    
	(WORD)   94,		// coef2    
	(WORD) -173,		// coef3    
	(WORD)  225,		// coef4    
	(WORD)  -52,		// coef5    
	(WORD) -204,		// coef6    
	(WORD)  -20,		// coef7    
	(WORD)  225},		// coef8    
	{(WORD)   0,		// in_bias0 
	(WORD)    0,		// in_bias1 
	(WORD)    0},		// in_bias2 
	{(WORD)  64,		// out_bias0
	(WORD)  512,		// out_bias1
	(WORD)  512},		// out_bias2
};

ROMDATA MISP_CSCCTRL_INFO MISP_CscRGBtoYUV_TestRange	= { // 16-255 with MISP_CscBypass_FullRange
	{(WORD) 343,		// coef0    
	(WORD)   34,		// coef1    
	(WORD)   103,		// coef2    
	(WORD) -173,		// coef3    
	(WORD)  225,		// coef4    
	(WORD)  -52,		// coef5    
	(WORD) -204,		// coef6    
	(WORD)  -20,		// coef7    
	(WORD)  225},		// coef8    
	{(WORD)   0,		// in_bias0 
	(WORD)    0,		// in_bias1 
	(WORD)    0},		// in_bias2 
	{(WORD)  64,		// out_bias0
	(WORD)  512,		// out_bias1
	(WORD)  512},		// out_bias2
};


ROMDATA MISP_CSCCTRL_INFO MISP_CscRGBtoYUV_AdobeRGB98	= {
	{(WORD) 321,		// coef0    
	(WORD)   39,		// coef1    
	(WORD)  152,		// coef2    
	(WORD) -174,		// coef3    
	(WORD)  256,		// coef4    
	(WORD)  -82,		// coef5    
	(WORD) -229,		// coef6    
	(WORD)  -27,		// coef7    
	(WORD)  256},		// coef8    
	{(WORD)   0,		// in_bias0 
	(WORD)    0,		// in_bias1 
	(WORD)    0},		// in_bias2 
	{(WORD)   0,		// out_bias0
	(WORD)  512,		// out_bias1
	(WORD)  512},		// out_bias2
};

ROMDATA MISP_CSCCTRL_INFO MISP_CscRGBtoYUV_BT2020	= {
	{(WORD) 347,		// coef0    
	(WORD)   30,		// coef1    
	(WORD)  135,		// coef2    
	(WORD) -185,		// coef3    
	(WORD)  256,		// coef4    
	(WORD)  -71,		// coef5    
	(WORD) -235,		// coef6    
	(WORD)  -21,		// coef7    
	(WORD)  256},		// coef8    
	{(WORD)   0,		// in_bias0 
	(WORD)    0,		// in_bias1 
	(WORD)    0},		// in_bias2 
	{(WORD)   0,		// out_bias0
	(WORD)  512,		// out_bias1
	(WORD)  512},		// out_bias2
};

ROMDATA MISP_CSCCTRL_INFO MISP_CscRGBtoYUV_AppleRGB	= {
	{(WORD) 344,		// coef0    
	(WORD)   43,		// coef1    
	(WORD)  125,		// coef2    
	(WORD) -188,		// coef3    
	(WORD)  256,		// coef4    
	(WORD)  -68,		// coef5    
	(WORD) -228,		// coef6    
	(WORD)  -28,		// coef7    
	(WORD)  256},		// coef8    
	{(WORD)   0,		// in_bias0 
	(WORD)    0,		// in_bias1 
	(WORD)    0},		// in_bias2 
	{(WORD)   0,		// out_bias0
	(WORD)  512,		// out_bias1
	(WORD)  512},		// out_bias2
};

ROMDATA MISP_CSCCTRL_INFO MISP_IN_CscBypass_StdRange	= {
	{(WORD) 512,		// coef0    
	 (WORD)   0,		// coef1    
	 (WORD)   0,		// coef2    
	 (WORD)   0,		// coef3    
	 (WORD) 512,		// coef4    
	 (WORD)   0,		// coef5    
	 (WORD)   0,		// coef6    
	 (WORD)   0,		// coef7    
	 (WORD) 512 },		// coef8    
	{(WORD)   0,		// in_bias0 
	 (WORD)-512,		// in_bias1 
	 (WORD)-512 },		// in_bias2 
	{(WORD)   0,		// out_bias0
	 (WORD) 512,		// out_bias1
	 (WORD) 512 },		// out_bias2
};



//--------------------------------------------------------------------------------------------------------------------------
// YCbCr to RGB for output CSC
ROMDATA MISP_CSCCTRL_INFO MISP_CscBypass_FullRange			= {  //bypass 0-255
	{(WORD) 512,		// coef0    
	 (WORD)   0,		// coef1    
	 (WORD)   0,		// coef2    
	 (WORD)   0,		// coef3    
	 (WORD) 512,		// coef4    
	 (WORD)   0,		// coef5    
	 (WORD)   0,		// coef6    
	 (WORD)   0,		// coef7    
	 (WORD) 512 },		// coef8    
	{(WORD)   0,		// in_bias0 
	 (WORD)-512,		// in_bias1 
	 (WORD)-512 },		// in_bias2 
	{(WORD)   0,		// out_bias0		// because of 100IRE, add 64
	 (WORD) 512,		// out_bias1
	 (WORD) 512 },		// out_bias2
};

ROMDATA MISP_CSCCTRL_INFO MISP_CscBypass_StdRange	= {  //bypass 16-235
	{(WORD) 512,		// coef0    
	 (WORD)   0,		// coef1    
	 (WORD)   0,		// coef2    
	 (WORD)   0,		// coef3    
	 (WORD) 512,		// coef4    
	 (WORD)   0,		// coef5    
	 (WORD)   0,		// coef6    
	 (WORD)   0,		// coef7    
	 (WORD) 512 },		// coef8    
	{(WORD) -64,		// in_bias0 
	 (WORD)-512,		// in_bias1 
	 (WORD)-512 },		// in_bias2 
	{(WORD)  64,		// out_bias0		// because of 100IRE, add 64
	 (WORD) 512,		// out_bias1
	 (WORD) 512 },		// out_bias2
};

ROMDATA MISP_CSCCTRL_INFO MISP_CscBypass_SonyRange	= {  //setting 16-255
	{(WORD) 554,		// coef0    
	 (WORD)   0,		// coef1    
	 (WORD)   0,		// coef2    
	 (WORD)   0,		// coef3    
	 (WORD) 554,		// coef4    
	 (WORD)   0,		// coef5    
	 (WORD)   0,		// coef6    
	 (WORD)   0,		// coef7    
	 (WORD) 554 },		// coef8    
	{(WORD) -64,		// in_bias0 
	 (WORD)-512,		// in_bias1 
	 (WORD)-512 },		// in_bias2 
	{(WORD)  64,		// out_bias0		// because of 100IRE, add 64
	 (WORD) 512,		// out_bias1
	 (WORD) 512 },		// out_bias2
};

ROMDATA MISP_CSCCTRL_INFO MISP_CscYUVtoRGB_SD_StdRange	= {
	{(WORD) 512,		// coef0    
	 (WORD)-172,		// coef1    
	 (WORD)-357,		// coef2    
	 (WORD) 512,		// coef3    
	 (WORD) 887,		// coef4    
	 (WORD)   0,		// coef5    
	 (WORD) 512,		// coef6    
	 (WORD)   0,		// coef7    
	 (WORD) 702},		// coef8    
	{(WORD) -64,		// in_bias0 
	 (WORD)-512,		// in_bias1 
	 (WORD)-512},		// in_bias2 
	{(WORD)   0,		// out_bias0
	 (WORD)   0,		// out_bias1
	 (WORD)   0},		// out_bias2
};

ROMDATA MISP_CSCCTRL_INFO MISP_CscYUVtoRGB_HD_StdRange	= {
	{(WORD) 512,		// coef0    
	 (WORD) -96,		// coef1    
	 (WORD)-239,		// coef2    
	 (WORD) 512,		// coef3    
	 (WORD) 950,		// coef4    
	 (WORD)   0,		// coef5    
	 (WORD) 512,		// coef6    
	 (WORD)   0,		// coef7    
	 (WORD) 804},		// coef8    
	{(WORD)   0,		// in_bias0 
	 (WORD)-512,		// in_bias1 
	 (WORD)-512},		// in_bias2 
	{(WORD)   0,		// out_bias0
	 (WORD)   0,		// out_bias1
	 (WORD)   0},		// out_bias2
};

ROMDATA MISP_CSCCTRL_INFO MISP_CscYUVtoRGB_SD_FullRange	= {
	{(WORD) 596,		// coef0    
	 (WORD)-200,		// coef1    
	 (WORD)-416,		// coef2    
	 (WORD) 596,		// coef3    
	 (WORD)1033,		// coef4    
	 (WORD)   0,		// coef5    
	 (WORD) 596,		// coef6    
	 (WORD)   0,		// coef7    
	 (WORD) 817},		// coef8    
	{(WORD) -64,		// in_bias0 
	 (WORD)-512,		// in_bias1 
	 (WORD)-512},		// in_bias2 
	{(WORD)   0,		// out_bias0
	 (WORD)   0,		// out_bias1
	 (WORD)   0},		// out_bias2
};

ROMDATA MISP_CSCCTRL_INFO MISP_CscYUVtoRGB_HD_FullRange	= {
	{(WORD) 596,		// coef0    
	 (WORD)-109,		// coef1    
	 (WORD)-273,		// coef2    
	 (WORD) 596,		// coef3    
	 (WORD)1083,		// coef4    
	 (WORD)   0,		// coef5    
	 (WORD) 596,		// coef6    
	 (WORD)   0,		// coef7    
	 (WORD) 918},		// coef8    
	{(WORD) -64,		// in_bias0 
	 (WORD)-512,		// in_bias1 
	 (WORD)-512},		// in_bias2 
	{(WORD)   0,		// out_bias0
	 (WORD)   0,		// out_bias1
	 (WORD)   0},		// out_bias2
};


//--------------------------------------------------------------------------------------------------------------------------
// hue range -60~60, cos(hue) table for picture HUE control
ROMDATA WORD MISP_CscCosHue[]		= {
	0x0200, 0x0207, 0x020e, 0x0216, 0x021d, 0x0224, 0x022b, 0x0232,
	0x0239, 0x0240, 0x0247, 0x024e, 0x0254, 0x025b, 0x0262, 0x0269,
	0x026f, 0x0276, 0x027d, 0x0283, 0x028a, 0x0290, 0x0297, 0x029d,
	0x02a3, 0x02a9, 0x02b0, 0x02b6, 0x02bc, 0x02c2, 0x02c8, 0x02ce,
	0x02d4, 0x02da, 0x02e0, 0x02e6, 0x02eb, 0x02f1, 0x02f7, 0x02fc,
	0x0302, 0x0307, 0x030d, 0x0312, 0x0318, 0x031d, 0x0322, 0x0327,
	0x032c, 0x0331, 0x0336, 0x033b, 0x0340, 0x0345, 0x034a, 0x034f,
	0x0353, 0x0358, 0x035d, 0x0361, 0x0366, 0x036a, 0x036e, 0x0373,
	0x0377, 0x037b, 0x037f, 0x0383, 0x0387, 0x038b, 0x038f, 0x0393,
	0x0396, 0x039a, 0x039e, 0x03a1, 0x03a5, 0x03a8, 0x03ac, 0x03af,
	0x03b2, 0x03b5, 0x03b8, 0x03bb, 0x03be, 0x03c1, 0x03c4, 0x03c7,
	0x03ca, 0x03cc, 0x03cf, 0x03d1, 0x03d4, 0x03d6, 0x03d9, 0x03db,
	0x03dd, 0x03df, 0x03e1, 0x03e3, 0x03e5, 0x03e7, 0x03e9, 0x03eb,
	0x03ec, 0x03ee, 0x03ef, 0x03f1, 0x03f2, 0x03f4, 0x03f5, 0x03f6,
	0x03f7, 0x03f8, 0x03f9, 0x03fa, 0x03fb, 0x03fc, 0x03fd, 0x03fd,
	0x03fe, 0x03fe, 0x03ff, 0x03ff, 0x03ff, 0x0400, 0x0400, 0x0400,
	0x0400, 0x0400, 0x0400, 0x0400, 0x03ff, 0x03ff, 0x03ff, 0x03fe,
	0x03fe, 0x03fd, 0x03fd, 0x03fc, 0x03fb, 0x03fa, 0x03f9, 0x03f8,
	0x03f7, 0x03f6, 0x03f5, 0x03f4, 0x03f2, 0x03f1, 0x03ef, 0x03ee,
	0x03ec, 0x03eb, 0x03e9, 0x03e7, 0x03e5, 0x03e3, 0x03e1, 0x03df,
	0x03dd, 0x03db, 0x03d9, 0x03d6, 0x03d4, 0x03d1, 0x03cf, 0x03cc,
	0x03ca, 0x03c7, 0x03c4, 0x03c1, 0x03be, 0x03bb, 0x03b8, 0x03b5,
	0x03b2, 0x03af, 0x03ac, 0x03a8, 0x03a5, 0x03a1, 0x039e, 0x039a,
	0x0396, 0x0393, 0x038f, 0x038b, 0x0387, 0x0383, 0x037f, 0x037b,
	0x0377, 0x0373, 0x036e, 0x036a, 0x0366, 0x0361, 0x035d, 0x0358,
	0x0353, 0x034f, 0x034a, 0x0345, 0x0340, 0x033b, 0x0336, 0x0331,
	0x032c, 0x0327, 0x0322, 0x031d, 0x0318, 0x0312, 0x030d, 0x0307,
	0x0302, 0x02fc, 0x02f7, 0x02f1, 0x02eb, 0x02e6, 0x02e0, 0x02da,
	0x02d4, 0x02ce, 0x02c8, 0x02c2, 0x02bc, 0x02b6, 0x02b0, 0x02a9,
	0x02a3, 0x029d, 0x0297, 0x0290, 0x028a, 0x0283, 0x027d, 0x0276,
	0x026f, 0x0269, 0x0262, 0x025b, 0x0254, 0x024e, 0x0247, 0x0240,
	0x0239, 0x0232, 0x022b, 0x0224, 0x021d, 0x0216, 0x020e, 0x0207
};

// hue range -60~60, sin(hue) table for picture HUE control
ROMDATA WORD MISP_CscSinHue[]		= {
	0xfc89, 0xfc8d, 0xfc92, 0xfc96, 0xfc9a, 0xfc9f, 0xfca3, 0xfca8,
	0xfcad, 0xfcb1, 0xfcb6, 0xfcbb, 0xfcc0, 0xfcc5, 0xfcca, 0xfccf,
	0xfcd4, 0xfcd9, 0xfcde, 0xfce3, 0xfce8, 0xfcee, 0xfcf3, 0xfcf9,
	0xfcfe, 0xfd04, 0xfd09, 0xfd0f, 0xfd15, 0xfd1a, 0xfd20, 0xfd26,
	0xfd2c, 0xfd32, 0xfd38, 0xfd3e, 0xfd44, 0xfd4a, 0xfd50, 0xfd57,
	0xfd5d, 0xfd63, 0xfd69, 0xfd70, 0xfd76, 0xfd7d, 0xfd83, 0xfd8a,
	0xfd91, 0xfd97, 0xfd9e, 0xfda5, 0xfdac, 0xfdb2, 0xfdb9, 0xfdc0,
	0xfdc7, 0xfdce, 0xfdd5, 0xfddc, 0xfde3, 0xfdea, 0xfdf2, 0xfdf9,
	0xfe00, 0xfe07, 0xfe0f, 0xfe16, 0xfe1d, 0xfe25, 0xfe2c, 0xfe34,
	0xfe3b, 0xfe43, 0xfe4a, 0xfe52, 0xfe59, 0xfe61, 0xfe69, 0xfe70,
	0xfe78, 0xfe80, 0xfe88, 0xfe8f, 0xfe97, 0xfe9f, 0xfea7, 0xfeaf,
	0xfeb7, 0xfebf, 0xfec7, 0xfecf, 0xfed7, 0xfedf, 0xfee7, 0xfeef,
	0xfef7, 0xfeff, 0xff07, 0xff0f, 0xff17, 0xff20, 0xff28, 0xff30,
	0xff38, 0xff40, 0xff49, 0xff51, 0xff59, 0xff61, 0xff6a, 0xff72,
	0xff7a, 0xff83, 0xff8b, 0xff93, 0xff9c, 0xffa4, 0xffac, 0xffb5,
	0xffbd, 0xffc5, 0xffce, 0xffd6, 0xffdf, 0xffe7, 0xffef, 0xfff8,
	0x0000, 0x0008, 0x0011, 0x0019, 0x0021, 0x002a, 0x0032, 0x003b,
	0x0043, 0x004b, 0x0054, 0x005c, 0x0064, 0x006d, 0x0075, 0x007d,
	0x0086, 0x008e, 0x0096, 0x009f, 0x00a7, 0x00af, 0x00b7, 0x00c0,
	0x00c8, 0x00d0, 0x00d8, 0x00e0, 0x00e9, 0x00f1, 0x00f9, 0x0101,
	0x0109, 0x0111, 0x0119, 0x0121, 0x0129, 0x0131, 0x0139, 0x0141,
	0x0149, 0x0151, 0x0159, 0x0161, 0x0169, 0x0171, 0x0178, 0x0180,
	0x0188, 0x0190, 0x0197, 0x019f, 0x01a7, 0x01ae, 0x01b6, 0x01bd,
	0x01c5, 0x01cc, 0x01d4, 0x01db, 0x01e3, 0x01ea, 0x01f1, 0x01f9,
	0x0200, 0x0207, 0x020e, 0x0216, 0x021d, 0x0224, 0x022b, 0x0232,
	0x0239, 0x0240, 0x0247, 0x024e, 0x0254, 0x025b, 0x0262, 0x0269,
	0x026f, 0x0276, 0x027d, 0x0283, 0x028a, 0x0290, 0x0297, 0x029d,
	0x02a3, 0x02a9, 0x02b0, 0x02b6, 0x02bc, 0x02c2, 0x02c8, 0x02ce,
	0x02d4, 0x02da, 0x02e0, 0x02e6, 0x02eb, 0x02f1, 0x02f7, 0x02fc,
	0x0302, 0x0307, 0x030d, 0x0312, 0x0318, 0x031d, 0x0322, 0x0327,
	0x032c, 0x0331, 0x0336, 0x033b, 0x0340, 0x0345, 0x034a, 0x034f,
	0x0353, 0x0358, 0x035d, 0x0361, 0x0366, 0x036a, 0x036e, 0x0373
};



// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
	
// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

/*  FILE_END_HERE */
