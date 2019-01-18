//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2017  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	CDCE913.C
// Description 		:	external pll ic
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"

#if defined(__USE_EXT_PLL_IC__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
static ROMDATA CDCE913_PLL defCDCE913Pll[] = {
//	N,		M,		Pdiv,	R,	Q,	P, RNG
// TV
	{3,		1,		6,		0,	24,	3,	0},	// 13.5Mhz (720x480i)
	{3,		1,		6,		0,	24,	3,	0},	// 13.5Mhz (720x576i)
	{2,		3,		1,		1,	21,	5,	1},	// 18Mhz (960x480i)
	{2,		3,		1,		1,	21,	5,	1},	// 18Mhz (960x576i)
	
	{3,		1,		3,		0,	24,	3,	0},	// 27Mhz (720x480p)
	{3,		1,		3,		0,	24,	3,	0},	// 27Mhz (720x576p)
	{4,		3,		1,		1,	21,	4,	1},	// 36Mhz (960x480p)
	{25,	18,		1,		4,	22,	4,	1},	// 37.5Mhz (960x576p)
	
	{11,	2,		2,		0,	22,	2,	1},	// 74.25Mhz (720p60)
#if defined(__USE_59Hz_OUTPUT__)
	{500,	91,		2,		89,	21,	2,	1},	// 74.1758Mhz (720p59) -- 74.175824
#endif
	{11,	2,		2,		0,	22,	2,	1},	// 74.25Mhz (720p50)
	{11,	2,		2,		0,	22,	2,	1},	// 74.25Mhz (720p30)
	{11,	2,		2,		0,	22,	2,	1},	// 74.25Mhz (720p25)
	{22,	5,		2,		3,	17,	2,	0},	// 59.4Mhz (720p24)
	
	{11,	2,		2,		0,	22,	2,	1},	// 74.25Mhz (1080i60)
#if defined(__USE_59Hz_OUTPUT__)
	{500,	91,		2,		89,	21,	2,	1},	// 74.1758Mhz (1080i59) -- 74.175824
#endif
	{11,	2,		2,		0,	22,	2,	1},	// 74.25Mhz (1080i50)
	
	{11,	2,		1,		0,	22,	2,	1},	// 148.5Mhz (1080p60)
	//{187,	34,		1,		0,	22,	2,	1},	// 148.5Mhz (1080p60)
#if defined(__USE_59Hz_OUTPUT__)
	{500,	91,		1,		89,	21,	2,	1},	// 148.3516Mhz (1080p59) -- 148.351648
#endif
	{11,	2,		1,		0,	22,	2,	1},	// 148.5Mhz (1080p50)
	{11,	2,		2,		0,	22,	2,	1},	// 74.25Mhz (1080p30)
#if defined(__USE_59Hz_OUTPUT__)
	{500,	91,		2,		89,	21,	2,	1},	// 74.1758Mhz (1080p29) -- 74.175824
#endif
	{11,	2,		2,		0,	22,	2,	1},	// 74.25Mhz (1080p25)
	{11,	2,		2,		0,	22,	2,	1},	// 74.25Mhz (1080p24)

#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	{11,	1,		1,		0,	22,	1,	3},	// 297Mhz (3840x2160p30)
	//{374,	34,		1,		0,	22,	1,	3},	// 297Mhz (3840x2160p30)
#if defined(__USE_59Hz_OUTPUT__)
	{1000,	91,		1,		89,	21,	1,	1},	// 296.7032Mhz (3840x2160p29)
#endif
	{11,	1,		1,		0,	22,	1,	3},	// 297Mhz (3840x2160p25)
	{11,	1,		1,		0,	22,	1,	3},	// 297Mhz (3840x2160p24)
	{11,	2,		1,		0,	22,	2,	1},	// 148.5Mhz (3840x2160p15)
	{11,	2,		1,		0,	22,	2,	1},	// 148.5Mhz (3840x2160p12)
	{11,	1,		1,		0,	22,	1,	3},	// 297Mhz (4096x2160p30)
#if defined(__USE_59Hz_OUTPUT__)
	{1000,	91,		1,		89,	21,	1,	1},	// 296.7032Mhz (4096x2160p29)
#endif
	{11,	1,		1,		0,	22,	1,	3},	// 297Mhz (4096x2160p25)
	{11,	1,		1,		0,	22,	1,	3},	// 297Mhz (4096x2160p24)
	{11,	2,		1,		0,	22,	2,	1},	// 148.5Mhz (2560x1440p30)
	{11,	2,		1,		0,	22,	2,	1},	// 148.5Mhz (2560x1440p25)
#if defined(__USE_4K60_INOUT__)
	{11,	1,		1,		0,	22,	1,	3},	// 297Mhz (1920x2160p60)
#if defined(__USE_59Hz_OUTPUT__)
	{1000,	91,		1,		89,	21,	1,	1},	// 296.7032Mhz (1920x2160p59)
#endif
	{11,	1,		1,		0,	22,	1,	3},	// 297Mhz (1920x2160p50)
	{11,	1,		1,		0,	22,	1,	3},	// 297Mhz (2048x2160p60)
#if defined(__USE_59Hz_OUTPUT__)
	{1000,	91,		1,		89,	21,	1,	1},	// 296.7032Mhz (2048x2160p29)
#endif
	{11,	1,		1,		0,	22,	1,	3},	// 297Mhz (2048x2160p50)
#endif
	{11,	1,		1,		0,	22,	1,	3},	// 297Mhz (1920x1080p120)
	{11,	1,		1,		0,	22,	1,	3},	// 297Mhz (1920x1080p100)
#endif
	

//PC
	{1007,	135,	8,		113,	29,	2,	3},	//25.175Mhz (640x480p60)
	{7,		2,		3,		0,		28,	3,	0},	//31.5Mhz (640x480p75)
	{80,	27,		2,		19,		23,	3,	0},	//40Mhz (800x600p60)
	{11,	3,		2,		1,		29,	3,	0},	//49.5Mhz (800x600p75)
	{130,	27,		2,		7,		19,	2,	1},	//65Mhz (1024x768p60)
 	{788,	135,	2,		47,		23,	2,	2},	//78.8Mhz (1024x768p75)
	{329,	87,		1,		22,		30,	3,	0},	//102.10345Mhz (1280x960p60)
	{4,		1,		1,		0,		16,	2,	0},	//108Mhz (1280x1024p60)
	{5,		1,		1,		0,		20,	2,	1},	//135Mhz (1280x1024p75)
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	{877,	124,	1,		36,		28,	2,	1},	//190.96Mhz (1280x1024p100) -- 190.9596
#endif
	{19,	6,		1,		2,		25,	3,	0},	//85.5Mhz (1360x768p60)
	{19,	6,		1,		2,		25,	3,	0},	//85.5Mhz (1366x768p60)
	{1712,	377,	1,		62,		18,	2,	0},	//122.61Mhz (1400x1050p60) -- 122.6100796
	{6,		1,		1,		0,		24,	2,	1},	//162Mhz (1600x1200p60)


//ETC
	{1183,	300,	1,		164,	31,	3,	0},	//106.47Mhz (1440x900p60)
	{81,	16,		1,		4,		20,	2,	1},	//136.6875Mhz (1440x900p75)
	{65,	12,		1,		8,		21,	2,	1},	//146.25Mhz (1680x1050p60)
 	{119,	27,		1,		17,		17,	2,	0},	//119Mhz (1680x1050p60RB)
 	{277,	54,		1,		28,		20,	2,	1},	//138.5Mhz (1920x1080p60RB)
	{154,	27,		1,		22,		22,	2,	2},	//154Mhz (1920x1200p60RB)
	
	
// SENSOR(for sensor in/out 1:1 mode)
	{4,		1,		1,		0,	16,	2,	0},	// 108Mhz (2048x1536p30) -- imx124(3M)
	{4,		1,		1,		0,	16,	2,	0},	// 108Mhz (2048x1536p25) -- imx124(3M)
	{11,	3,		1,		1,	29,	3,	0},	// 99Mhz (3072x1728p15) -- imx178(5M 16:9), imx326(5M)
	{11,	3,		1,		1,	29,	3,	0},	// 99Mhz (2592x1944p15) -- imx326(5M)
};


EXTPLL_CTRL_PARAM	stEXTPLL;


// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//------------------------------------------------------------------------------------------
static void Init_EXTPLL_PARAM(PEXTPLL_CTRL_PARAM pCTRL)
{
	pCTRL->pPll = (PCDCE913_PLL)&defCDCE913Pll[GetOUT_MainFrmt()];
}

//--------------------------------------------------------------------------------------------------
static void EXTPLL_SetPllReg(PCDCE913_PLL pPll)
{
	BYTE pdiv = pPll->pdiv;
	
	if (pPll == NULL)			return;
	
#if defined(__USE_4K60_INOUT__)
	pdiv *= 2;
	
	if(GetOUT_MainFrmt() == MISP_VIDOUT_1920x2160p60)	pdiv /= 2;
	if(GetOUT_MainFrmt() == MISP_VIDOUT_1920x2160p50)	pdiv /= 2;
	if(GetOUT_MainFrmt() == MISP_VIDOUT_2048x2160p60)	pdiv /= 2;
	if(GetOUT_MainFrmt() == MISP_VIDOUT_2048x2160p50)	pdiv /= 2;
#if defined(__USE_59Hz_OUTPUT__)
	if(GetOUT_MainFrmt() == MISP_VIDOUT_1920x2160p59)	pdiv /= 2;
	if(GetOUT_MainFrmt() == MISP_VIDOUT_2048x2160p59)	pdiv /= 2;
#endif
	
	// for interlace out
	if(GetOUT_MainFrmt() == MISP_VIDOUT_1920x1080i60)	pdiv /= 2;
	if(GetOUT_MainFrmt() == MISP_VIDOUT_1920x1080i50)	pdiv /= 2;
#if defined(__USE_59Hz_OUTPUT__)
	if(GetOUT_MainFrmt() == MISP_VIDOUT_1920x1080i59)	pdiv /= 2;
#endif
#endif
	
	CDCE913_RegField(0x02, 0, 2, 0);					// Pdiv[9:8]
	CDCE913_RegWrite(0x03, pdiv);						// Pdiv[7:0]
	CDCE913_RegWrite(0x18, (pPll->n>>4) & 0xff);		// N[11:4]
	CDCE913_RegField(0x19, 4, 4, pPll->n & 0x0f);		// N[3:0]
	CDCE913_RegField(0x19, 0, 4, (pPll->r>>5) & 0x0f);	// R[8:5]
	CDCE913_RegField(0x1a, 3, 5, pPll->r & 0x1f);		// R[4:0]
	CDCE913_RegField(0x1a, 0, 3, (pPll->q>>3) & 0x07);	// Q[5:3]
	CDCE913_RegField(0x1b, 5, 3, pPll->q & 0x07);		// Q[2:0]
	CDCE913_RegField(0x1b, 2, 3, pPll->p & 0x07);		// P[2:0]
	CDCE913_RegField(0x1b, 0, 2, pPll->rng & 0x03);		// VCO range sel(0:<125M, 1:<150M, 2:<175M, 3:>175M)

//	UARTprintf("CDCE913 : M[%d], N[%d], Pdiv[%d], R[%d], Q[%d], P[%d], RNG[%d] \r\n",
//				pPll->m, pPll->n, pPll->pdiv, pPll->r, pPll->q, pPll->p, pPll->rng);
}

//--------------------------------------------------------------------------------------------------
static void Init_EXTPLL_REGISTER(PEXTPLL_CTRL_PARAM pCTRL)
{
	// register init
	CDCE913_RegField(0x05, 3, 5, 0x12);		// Crystal Load cap(18pF)
 	CDCE913_RegField(0x14, 7, 1, 0);		// 0:PLL1 enable, 1:PLL1 bypass(power down)
	CDCE913_RegField(0x14, 6, 1, 0);		// Y2 Mux,  0:Pdiv1, 1:Pdiv2
 	CDCE913_RegField(0x16, 0, 7, 0);		// Pdiv2 0:reset and stand-by, 1~127:divider value
 	CDCE913_RegField(0x17, 0, 7, 0);		// Pdiv3 0:reset and stand-by, 1~127:divider value
	
	EXTPLL_SetPllReg(pCTRL->pPll);
}



//--------------------------------------------------------------------------------------------
void EXTPLL_Init(void)
{
	memset(&stEXTPLL, 0, sizeof(EXTPLL_CTRL_PARAM));
	
	Init_EXTPLL_PARAM(&stEXTPLL);
	Init_EXTPLL_REGISTER(&stEXTPLL);
}

//--------------------------------------------------------------------------------------------
void SetEXTPLL_Frmt(BYTE frmt)
{
	if(frmt >= MISP_VIDOUT_FORMAT_END)			return;
	
	stEXTPLL.pPll = (PCDCE913_PLL)&defCDCE913Pll[frmt];
	EXTPLL_SetPllReg(stEXTPLL.pPll);
}

//--------------------------------------------------------------------------------------------
void SetEXTPLL_Y1OnOff(BOOL OnOff)
{
	CDCE913_RegField(0x04, 1, 1, (OnOff)?  1:  0);		// 1: Y1 enable(default), 0: Y1 disable
	//UARTprintf("EXT. PLL (%s) \r\n", (OnOff)? "Y1 enable": "Y1 disable");
}

#endif	//__USE_EXT_PLL_IC__


/*  FILE_END_HERE */
