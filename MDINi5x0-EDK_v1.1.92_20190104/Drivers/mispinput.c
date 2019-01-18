//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	MISPINPUT.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"misp100.h"
#include	"cmos_if.h"		// for sensor dependent option

// ----------------------------------------------------------------------
// Extern Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
static IN_CTRL_PARAM	stINCTRL;


#if defined(__USE_4K60_OVL__)
#define		mstMrgn		8  // H-margin of source (8 means packet-size/pixel-resolution)
#endif

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
ROMDATA MISP_FNRFILT_COEF MISP_FrontNRFilter_Default[]	= {
	//0 bypass : 
  {	{0x0400, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},	// Y
	{0x0400, 0x0000, 0x0000, 0x0000}  },								// C

	//1 cut0.666 : 
  {	{0x0210, 0x013D, 0x0FF0, 0x0FAA, 0x000F, 0x0026, 0x0FFA, 0x0FF2},	// Y
	{0x0148, 0x00F5, 0x05E, 0x0009}  },									// C

	//2 cut0.563 : 
  {	{0x018A, 0x012C, 0x0067, 0x0FD9, 0x0FC5, 0x0FF1, 0x000E, 0x000B},	// Y
	{0x010C, 0x00DE, 0x0079, 0x0023}  },								// C	
	
	//3 cut0.5 : 
  {	{0x016E, 0x0123, 0x007C, 0x0FF4, 0x0FCA, 0x0FE3, 0x0001, 0x0008},	// Y
	{0x0102, 0x00D9, 0x007C, 0x002A}  },								// C

	//4 cut0.333 : 
  {	{0x00B4, 0x00A6, 0x0082, 0x0054, 0x0029, 0x000B, 0x0FFD, 0x0FF9},	// Y
	{0x00D0, 0x00B9, 0x0086, 0x0059}  },								// C

	//5 cut0.25 : 
  {	{0x0040, 0x0038, 0x0025, 0x0010, 0x0000, 0x03FA, 0x03FB, 0x03FE},	// Y
	{0x003E, 0x0036, 0x0020, 0x00B}  },									// C	
};

ROMDATA MISP_PEAKING_COEF MISP_PeakingFilter_Default	= {
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	{0x00F0, 0x008C, 0x07C0, 0x073C, 0x0000, 0x0000, 0x0000, 0x0000}, // BPF 20MHz~30Mhz
#else   							// MDIN-i51X (i510, i540, i550)
	// {0x0108, 0x005F, 0x077F, 0x079E, 0x0000, 0x0000, 0x0000, 0x0000}, // BPF 20MHz~30Mhz
		
	// {0x3AD, 0x1CF, 0x65F, 0x521, 0x6CB, 0x0AF, 0x11A, 0x044}, // BPF -	10, 20, 30, 40MHz
	// {0x3BD, 0x119, 0x57F, 0x612, 0x07C, 0x0F2, 0x01B, 0x7ED}, // BPF -	10, 20, 40, 50MHz 
	// {0x3D6, 0x06B, 0x4DA, 0x718, 0x205, 0x0EA, 0x728, 0x7A2}, // BPF -	20, 30, 40, 50MHz
	// {0x3BB, 0x7A3, 0x501, 0x0BD, 0x17F, 0x786, 0x7A9, 0x014}, // BPF -	20, 30, 50, 60MHz 	
	// {0x3C8, 0x6ED, 0x5E5, 0x166, 0x021, 0x7DC, 0x00E, 0x7D8}, // BPF -	20, 30, 50, 70MHz
	{0x1FE, 0x000, 0x701, 0x000, 0x000, 0x000, 0x000}, // BPF half gain-	No Ringing 5tab
	// {0x3F4, 0x002, 0x601, 0x002, 0x000, 0x000, 0x000}, //  BPF half gain- No Ringing 7tab
	// {0x32D, 0x09A, 0x611, 0x7BF, 0x000, 0x000, 0x000}, // BPF -	No Ringing 7tab
	// {0x3BD, 0x067, 0x5D4, 0x79E, 0x002, 0x7C7, 0x04B, 0x032}, // Fsample, Fstop1, Fpass1, Fpass2, Fstop2 = 150, 5, 25, 45, 65MHz		
/*
-	Fstop1, Fpass1, Fpass2, Fstop2 : peak_coef0 ~ peak_coef7
-	10, 20, 30, 40MHz : 0x3AD, 0x1CF, 0x65F, 0x521, 0x6CB, 0x0AF, 0x11A, 0x044
-	10, 20, 40, 50MHz : 0x3BD, 0x119, 0x57F, 0x612, 0x07C, 0x0F2, 0x01B, 0x7ED
-	20, 30, 40, 50MHz : 0x3D6, 0x06B, 0x4DA, 0x718, 0x205, 0x0EA, 0x728, 0x7A2
-	20, 30, 50, 60MHz : 0x3BB, 0x7A3, 0x501, 0x0BD, 0x17F, 0x786, 0x7A9, 0x014
-	20, 30, 50, 70MHz : 0x3C8, 0x6ED, 0x5E5, 0x166, 0x021, 0x7DC, 0x00E, 0x7D8
*/				
#endif
	// {0x0106, 0x0077, 0x079C, 0x076A, 0x0000, 0x0000, 0x0000, 0x0000}, // BPF 10MHz~40Mhz
	// {0x0200, 0x0000, 0x0700, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}, // HPF
};

ROMDATA MISP_PEAKING_COEF MISP_PeakingFilter_BPF50_7tab	= {
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	{0x3F4, 0x002, 0x601, 0x002, 0x0000, 0x0000, 0x0000, 0x0000}, // BPF 20MHz~30Mhz
#else   							// MDIN-i51X (i510, i540, i550)
	{0x3F4, 0x002, 0x601, 0x002, 0x000, 0x000, 0x000}, //  BPF half gain- No Ringing 7tab				
#endif
};

ROMDATA MISP_PEAKING_COEF MISP_PeakingFilter_BPF40_7tab	= {
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	{0x3F4, 0x002, 0x601, 0x002, 0x0000, 0x0000, 0x0000, 0x0000}, // BPF 20MHz~30Mhz
#else   							// MDIN-i51X (i510, i540, i550)
	{0x192, 0x0D0, 0x78F, 0x6D8, 0x000, 0x000, 0x000}, //  BPF lower freq. 2017.11.3	
#endif
};

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
static void Init_IN_PARAM( PIN_CTRL_PARAM pCTRL )
{
	// input pin map
#if defined(__MDIN_i500_EDK__) || defined(__MDIN_i500_REF__)
	pCTRL->stPinMap.extn_en  = 0;	// n/a
	pCTRL->stPinMap.lvds_en  = 0;	// n/a
	pCTRL->stPinMap.mode_sel = GetSS_SensorDoutFrmt()?  IN_P0_422_20B: IN_BAYER_12B_P1_656_10B;	// 1:lvds, 0:ttl
#endif
	
#if defined(__MDIN_i510_EDK__) || defined(__MDIN_i510_REF__) || defined(__MDIN_i510_IMX290__)
	pCTRL->stPinMap.extn_en  = 0;
	pCTRL->stPinMap.lvds_en  = GetSS_SensorDoutFrmt()?  1:  0;	// 1:lvds, 0:ttl
	pCTRL->stPinMap.mode_sel = GetSS_SensorDoutFrmt()?  IN_LVDS_6CH: IN_BAYER_12B;	// lvds(NLI.0), ttl(NCI.0)
#endif
	
#if defined(__MDIN_i540_EDK__) || defined(__MDIN_i540_REF__) || defined(__MDIN_i540_4K60__)
	pCTRL->stPinMap.extn_en  = 0;
	pCTRL->stPinMap.lvds_en  = GetSS_SensorDoutFrmt()?  1:  0;	// 1:lvds, 0:ttl
	pCTRL->stPinMap.mode_sel = GetSS_SensorDoutFrmt()?  IN_LVDS_8CH: IN_BAYER_12B;	// lvds(NLI.6), ttl(NCI.0)
#endif
	
#if defined(__MDIN_i550_EDK__)
	pCTRL->stPinMap.extn_en  = 1;
	pCTRL->stPinMap.lvds_en  = GetSS_SensorDoutFrmt()?  1:  0;	// 1:lvds, 0:ttl
	pCTRL->stPinMap.mode_sel = GetSS_SensorDoutFrmt()?  IN_LVDS_8CH_X4B_P1_422_20B: IN_BAYER_12B;	// lvds(ELI.10), ttl(ECI.0)
#endif
	
#if defined(__MDIN_i550_REF__)
	pCTRL->stPinMap.extn_en  = 1;
	pCTRL->stPinMap.lvds_en  = GetSS_SensorDoutFrmt()?  1:  0;	// 1:lvds, 0:ttl	
	pCTRL->stPinMap.mode_sel = GetSS_SensorDoutFrmt()?  IN_LVDS_8CH_P1_444_24B: IN_BAYER_12B;	// lvds(ELI.16), ttl(ECI.0)
	
#if defined(__MDIN_i550_USB_V100__)
	pCTRL->stPinMap.mode_sel = GetSS_SensorDoutFrmt()?  IN_LVDS_8CH: IN_BAYER_12B;		// lvds(ELI.8), ttl(ECI.0)
#endif
	
#if defined (__PANORAMA_SENSOR_SETUP__)
#if __PANORAMA_5x0__	
	pCTRL->stPinMap.mode_sel = GetSS_SensorDoutFrmt()?  IN_LVDS_8CH_X4B_P1_422_20B: IN_BAYER_12B;	// lvds(ELI.10), ttl(ECI.0)
#endif
#endif
#endif
		
#if defined(__MDIN_i5XX_FPGA__)
#if defined(__USE_2_INPUT_HDMI__)
	pCTRL->stPinMap.extn_en  = 1;
	pCTRL->stPinMap.lvds_en  = 0;
	pCTRL->stPinMap.mode_sel = IN_P0_422_20B_P1_422_20B;  // ECI.10
#else
	pCTRL->stPinMap.extn_en  = 1;
	pCTRL->stPinMap.lvds_en  = 0;
	pCTRL->stPinMap.mode_sel = GetSS_SensorDoutFrmt()?  IN_LVDS_8CH: IN_BAYER_12B;	// lvds(ELI.8), ttl(ECI.0)
#endif
#endif

	// input path
#if defined(__USE_EXT2IPP__)
	pCTRL->MainPATH = MISP_MAINPATH_SENSOR_EXT_P1;
#else
	pCTRL->MainPATH = MISP_MAINPATH_SENSOR;
#endif

	if(GetCLK_UserPLL_LockSrcPort()==0)	pCTRL->AuxPATH  = MISP_AUXPATH_MAIN_O;
	else									pCTRL->AuxPATH  = MISP_AUXPATH_EXT_P1;	

	pCTRL->EncPATH  = MISP_VENC_PATH_PORT_X;
	
#if defined(__MDIN_i5XX_FPGA__) && defined(__USE_2_INPUT_HDMI__)
	pCTRL->AuxPATH  = MISP_AUXPATH_EXT_P1;
#endif
	
#if defined(__MDIN_i550_USB_V100__)
	pCTRL->AuxPATH  = MISP_AUXPATH_SENSOR;
#endif

	//main front NR
	pCTRL->stFNR_m.enab = 1;
	pCTRL->stFNR_m.coef = 0; 
	// stFNR_m.coef
	// 0 : 1920 to 1440 ~ 1920 ==> bypass
	// 1 : 1920 to 1024 ~ 1440 ==> cut0.563
	// 2 : 1920 to 720 ~ 1024  ==> cut0.375
	// 3 : 1920 to 640 ~ 720   ==> cut0.333
	// 4 : 1920 to 480 ~ 640   ==> cut0.250	
	
	//frame count
	pCTRL->FRcfg_m.mode = 3;	// custom mode		
	pCTRL->FRcfg_x.mode = 1;	// custom mode
#if defined (__USE_DDR_FIDELIX_256M__) && (defined (__USE_CMOS_IMX226__)||defined (__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__)) // 256MB 4K sensor	
	pCTRL->FRcfg_m.Y_cnt = 1;
	pCTRL->FRcfg_m.C_cnt = 1;
	pCTRL->FRcfg_x.Y_cnt = 1;	
#else
	// default main y/c frame count changed(2 -> 3) to fix menu osd broken when change 'c' frame(3fr -> 2fr)
	pCTRL->FRcfg_m.Y_cnt = 3;
	pCTRL->FRcfg_m.C_cnt = 3;
	#if	defined(__USE_IMX226_4K3KP30_MODE0__)	//overflow memory map, 12M sensor
	pCTRL->FRcfg_x.Y_cnt = 2;
	#else	
	pCTRL->FRcfg_x.Y_cnt = 3;
	#endif
#endif	

	pCTRL->FRcfg_m.Y_dly = 0;
	pCTRL->FRcfg_m.C_dly = 0;
	pCTRL->FRcfg_x.Y_dly = 0;

	//sensor video info.
	pCTRL->stSRC_sensor.frmt = GetSS_SensorVoutFrmt();
	pCTRL->stSRC_sensor.mode = GetSS_sensor_bit_opertion()?MISP_SRC_RGB444_10:MISP_SRC_RGB444_8;
#if CSC_RANGE_CTRL  //default value ; MiscTuneMenuInfo
  #if   defined(__MDIN_i5XX_FPGA__)
	pCTRL->stSRC_sensor.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_HD_FullRange;
  #elif defined(__MDIN_i540_4K60_V10__)
	pCTRL->stSRC_sensor.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_HD_FullRange;  
  #elif defined(__M33_V101__)
	pCTRL->stSRC_sensor.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_HD_StdRange;
  #elif defined(__USE_CMOS_IMX185__) || defined(__USE_CMOS_IMX274__) || ( defined(__USE_CMOS_IMX290__) && defined(__USE_MANUAL_LENS__) || defined(__USE_CMOS_IMX326__))
	// pCTRL->stSRC_sensor.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_TestRange;
	pCTRL->stSRC_sensor.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_HD_FullRange;
  #else
	pCTRL->stSRC_sensor.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_HD_FullRange;
  #endif
#endif

	//external video port 0 info.
	pCTRL->stSRC_extp0.frmt = MISP_VIDSRC_1920x1080p60;
#if defined(__MDIN_i5XX_FPGA__)						// FPGA board only
	pCTRL->stSRC_extp0.mode = MISP_SRC_SEP422_8;
#else         // edk & ref board
	pCTRL->stSRC_extp0.mode = MISP_SRC_EMB422_8;
#endif
#if CSC_RANGE_CTRL
	pCTRL->stSRC_extp0.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_HD_StdRange;
#endif

	//external video port 1 info.
#if defined(__USE_MDIN_i550__)	
	// pCTRL->stSRC_extp1.frmt = MISP_VIDSRC_1920x1080p60;
	pCTRL->stSRC_extp1.frmt = MISP_VIDSRC_3840x2160p30;		
#else
	pCTRL->stSRC_extp1.frmt = MISP_VIDSRC_1920x1080p60;
#endif
	
#if defined(__MDIN_i5XX_FPGA__)						// FPGA board only
	pCTRL->stSRC_extp1.mode = MISP_SRC_SEP422_8;
#else         // edk & ref board
#if defined(__MDIN_i550_REF__)
	pCTRL->stSRC_extp1.mode = MISP_SRC_RGB444_8;
#else
	pCTRL->stSRC_extp1.mode = MISP_SRC_MUX656_8;
#endif

#if defined(__USE_EXT2IPP__)
	pCTRL->stSRC_extp1.mode = MISP_SRC_RGB444_8;
#endif

#endif
#if CSC_RANGE_CTRL
	pCTRL->stSRC_extp1.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_HD_StdRange;
#endif

	// pCTRL->stHPF_m.pCoef = (PMISP_PEAKING_COEF)&MISP_PeakingFilter_Default;
	// pCTRL->stHPF_m.pCoef = (PMISP_PEAKING_COEF)&MISP_PeakingFilter_BPF50_7tab;
	pCTRL->stHPF_m.pCoef = (PMISP_PEAKING_COEF)&MISP_PeakingFilter_BPF40_7tab;
	
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	pCTRL->stHPF_m.cor_value = 15;	pCTRL->stHPF_m.cor_en = ON;
	pCTRL->stHPF_m.sat_value = 3;	pCTRL->stHPF_m.sat_en = ON;
	pCTRL->stHPF_m.level = 30;		pCTRL->stHPF_m.en = ON;
#else                           // MDIN-i51X (i510, i540, i550)
	pCTRL->stHPF_m.cor_value = 4;	pCTRL->stHPF_m.cor_en = ON;
	pCTRL->stHPF_m.sat_value = 3;	pCTRL->stHPF_m.sat_en = OFF;
	// pCTRL->stHPF_m.level = 60;		
	pCTRL->stHPF_m.level = 30;		// Default value changed 170316
	pCTRL->stHPF_m.en = ON;
	pCTRL->stHPF_m.cor_half_en = OFF;		pCTRL->stHPF_m.cor_edge_dis = OFF;
	pCTRL->stHPF_m.level_depend_en = ON;	pCTRL->stHPF_m.level_depend_th = 127 ;
	pCTRL->stHPF_m.level_depend_slope = 30;	pCTRL->stHPF_m.level_depend_offset = 0 ;
#endif

#if defined(__ENABLE_INPUT_CSC__)
	pCTRL->stSRC_extp0.stTUNE.brightness = 128;		pCTRL->stSRC_extp0.stTUNE.contrast = 128;
	pCTRL->stSRC_extp0.stTUNE.saturation = 128;		pCTRL->stSRC_extp0.stTUNE.hue = 128;	

	pCTRL->stSRC_extp1.stTUNE.brightness = 128;		pCTRL->stSRC_extp1.stTUNE.contrast = 128;
	pCTRL->stSRC_extp1.stTUNE.saturation = 128;		pCTRL->stSRC_extp1.stTUNE.hue = 128;	

	pCTRL->stSRC_sensor.stTUNE.brightness = 128;	pCTRL->stSRC_sensor.stTUNE.contrast = 128;
	pCTRL->stSRC_sensor.stTUNE.saturation = 128;	pCTRL->stSRC_sensor.stTUNE.hue = 128;	
#endif

	//main/aux video info.
	switch(pCTRL->MainPATH) {
		case MISP_MAINPATH_EXT_P0:  memcpy(&pCTRL->stSRC_m, &pCTRL->stSRC_extp0, sizeof(MISP_SRCVIDEO_INFO));   break;
		case MISP_MAINPATH_EXT_P1:  memcpy(&pCTRL->stSRC_m, &pCTRL->stSRC_extp1, sizeof(MISP_SRCVIDEO_INFO));   break;
		case MISP_MAINPATH_SENSOR:  memcpy(&pCTRL->stSRC_m, &pCTRL->stSRC_sensor, sizeof(MISP_SRCVIDEO_INFO));  break;
		case MISP_MAINPATH_SENSOR_EXT_P1:  memcpy(&pCTRL->stSRC_m, &pCTRL->stSRC_sensor, sizeof(MISP_SRCVIDEO_INFO));  break;		
	}
	switch(pCTRL->AuxPATH) {
		case MISP_AUXPATH_EXT_P0:   memcpy(&pCTRL->stSRC_x, &pCTRL->stSRC_extp0, sizeof(MISP_SRCVIDEO_INFO));   break;
		case MISP_AUXPATH_EXT_P1:   memcpy(&pCTRL->stSRC_x, &pCTRL->stSRC_extp1, sizeof(MISP_SRCVIDEO_INFO));   break;
		case MISP_AUXPATH_SENSOR:   memcpy(&pCTRL->stSRC_x, &pCTRL->stSRC_sensor, sizeof(MISP_SRCVIDEO_INFO));  break;
		case MISP_AUXPATH_MAIN_O: 	pCTRL->stSRC_x.mode = MISP_OUT_SEP422_10;
									pCTRL->stSRC_x.frmt = MISP_VIDSRC_2048x1536p30; 			// set aux frame memory max(4K/4)
									break;
	}
}

//------------------------------------------------------------------------------------------
static void INCTRL_SetMainPathCLK(void)
{
	switch(GetIN_MainPath()) {
		case MISP_MAINPATH_EXT_P0:
			SetCLK_SrcYC(0);    // i51x(0:clk_yc_tm, 1:clk_yc_tm/2, 2:clk_bayer),  i500(0:clk_b_p_hdmi, 1:clk_a_p_ttl, 2:clk2_bayer)
			SetCLK_SrcYCTm(1);  // clk_tm_yc_sel ( 0:ext_clk, 1:s_clk )
			SetCLK_InvYC(GetCLK_InvYCDef(0));
			break;
		case MISP_MAINPATH_EXT_P1:
			SetCLK_SrcYC(0);    // i51x(0:clk_yc_tm, 1:clk_yc_tm/2, 2:clk_bayer),  i500(0:clk_b_p_hdmi, 1:clk_a_p_ttl, 2:clk2_bayer)
			SetCLK_SrcYCTm(0);  // clk_tm_yc_sel ( 0:ext_clk, 1:s_clk )
			SetCLK_InvYC(GetCLK_InvYCDef(1));
			break;
		case MISP_MAINPATH_SENSOR:
		case MISP_MAINPATH_SENSOR_EXT_P1:			
			SetCLK_SrcYC(2);    // i51x(0:clk_yc_tm, 1:clk_yc_tm/2, 2:clk_bayer),  i500(0:clk_b_p_hdmi, 1:clk_a_p_ttl, 2:clk2_bayer)
			break;
	}
}

//------------------------------------------------------------------------------------------
static void INCTRL_SetAuxPathCLK(void)
{
	switch(GetIN_AuxPath()) {
		case MISP_AUXPATH_EXT_P0:
			SetCLK_SrcAux(0);    // i51x(0:clk_in_aux_tm, 1:clk_in_aux_tm/2, 2:clk_bayer),  i500(0:clk_b_p_hdmi, 1:clk_a_p_ttl, 2:clk2_bayer)
			SetCLK_SrcAuxIn(0);  // 0:clk_m_aux_int, 1:vclk_src, 2:clk_enc_int, 3:clk_aux_mclk
			SetCLK_SrcAuxlm(0);  // 0:clk_m_aux, 1:venc_clk, 2:vclk, 3:axclk
			SetCLK_SrcAuxTm(1);  // clk_tm_aux_sel ( 0:ext_clk, 1:s_clk )
			SetCLK_InvAux(GetCLK_InvAuxDef(0));
			break;
		case MISP_AUXPATH_EXT_P1:
			SetCLK_SrcAux(0);    // i51x(0:clk_in_aux_tm, 1:clk_in_aux_tm/2, 2:clk_bayer),  i500(0:clk_b_p_hdmi, 1:clk_a_p_ttl, 2:clk2_bayer)
			SetCLK_SrcAuxIn(0);  // 0:clk_m_aux_int, 1:vclk_src, 2:clk_enc_int, 3:clk_aux_mclk
			SetCLK_SrcAuxlm(0);  // 0:clk_m_aux, 1:venc_clk, 2:vclk, 3:axclk
			SetCLK_SrcAuxTm(0);  // clk_tm_aux_sel ( 0:ext_clk, 1:s_clk )
			SetCLK_InvAux(GetCLK_InvAuxDef(1));
			break;
		case MISP_AUXPATH_SENSOR:
			SetCLK_SrcAux(2);    // i51x(0:clk_in_aux_tm, 1:clk_in_aux_tm/2, 2:clk_bayer),  i500(0:clk_b_p_hdmi, 1:clk_a_p_ttl, 2:clk2_bayer)
			SetCLK_SrcAuxIn(0);  // 0:clk_m_aux_int, 1:vclk_src, 2:clk_enc_int, 3:clk_aux_mclk
			SetCLK_SrcAuxlm(0);  // 0:clk_m_aux, 1:venc_clk, 2:vclk, 3:axclk
			break;
		case MISP_AUXPATH_MAIN_O:
			SetCLK_SrcAux(GetCLK_SrcYC());
			SetCLK_SrcAuxIn(1);  // 0:clk_m_aux_int, 1:vclk_src, 2:clk_enc_int, 3:clk_aux_mclk
			SetCLK_SrcAuxlm(2);  // 0:clk_m_aux, 1:venc_clk, 2:vclk, 3:axclk
			break;
	}
	
#if defined(__USE_CMOS_AUX_SYNC_OUT__) && (defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__)) // Use AUX sync until i5x0 revision for Sensor Slave Mode
	SetCLK_SrcAuxIn(0);
#endif
}

//------------------------------------------------------------------------------------
static void INCTRL_SetPinMAP(PMISP_INP_MODE_INFO pINFO)
{
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	MISP_RegField(MISP_LOCAL_ID, 0x048,  0, 4, pINFO->mode_sel);	// input_mode_sel
#else 												// MDIN-i51X (i510, i540, i550)
	MISP_RegField(MISP_LOCAL_ID, 0x048,  0, 5, pINFO->mode_sel);	// input_mode_sel
	MISP_RegField(MISP_LOCAL_ID, 0x048, 15, 1, pINFO->extn_en);		// input_mode_sel_ext_en
	MISP_RegField(MISP_LOCAL_ID, 0x048, 14, 1, pINFO->lvds_en);		// input_mode_sel_lvds_en

#if defined(__MDIN_i550_REF_V10__)									// i550 ref v1.0
	MISP_RegField(MISP_LOCAL_ID, 0x048, 12, 2, 3);					// input_mode_sync_swap_aux, input_mode_sync_swap_main
#if defined (__PANORAMA_SENSOR_SETUP__) && (__PANORAMA_5x0__)		// panorama one board
	MISP_RegField(MISP_LOCAL_ID, 0x048, 12, 2, 0);
#endif
#endif

#if defined(__USE_EXT2IPP__)
	if ( stINCTRL.stSRC_extp1.mode == MISP_SRC_RGB444_8 || stINCTRL.stSRC_extp1.mode == MISP_SRC_RGB444_10 ) 	MISP_RegWrite(MISP_LOCAL_ID, 0x390, 0x8000) ;
	else		MISP_RegWrite(MISP_LOCAL_ID, 0x390, 0x2200) ;
	MISP_RegField(MISP_LOCAL_ID, 0x391, 12, 4, 0x0008) ;
#endif

#endif

}

//------------------------------------------------------------------------------------
static void INCTRL_SetMainPath(WORD main_inmux_sel)
{
	MISP_RegField(MISP_LOCAL_ID, 0x049, 0, 2, main_inmux_sel);	// 0:ext_p0, 1:ext_p1, 2:sensor
}

//------------------------------------------------------------------------------------
static void INCTRL_SetMainAttb(PMISP_SRCVIDEO_INFO pSRC)
{
#if defined(__USE_4K60_OVL__)
	pSRC->stATTB.Hact +=mstMrgn;
#endif
	
	MISP_RegField(MISP_LOCAL_ID, 0x006,  0, 13, pSRC->stATTB.Hact);       // hsize_a
	MISP_RegField(MISP_LOCAL_ID, 0x007,  0, 13, pSRC->stATTB.Vact);       // vsize_a

	MISP_RegField(MISP_LOCAL_ID, 0x004,  0, 10, pSRC->stFINE.offH);       // active_h_offset_a [9:0]
	MISP_RegField(MISP_LOCAL_ID, 0x006, 13,  3, pSRC->stFINE.offH>>10);   // active_h_offset_a [12:10]
	MISP_RegField(MISP_LOCAL_ID, 0x004, 10,  6, pSRC->stFINE.offV);       // active_v_offset_a [5:0]
	MISP_RegField(MISP_LOCAL_ID, 0x003,  8,  7, pSRC->stFINE.offV>>6);    // active_v_offset_a [12:6]

	MISP_RegField(MISP_LOCAL_ID, 0x000, 11,  3, pSRC->stFINE.yc_offset);  // yc_offset_a
	MISP_RegField(MISP_LOCAL_ID, 0x000,  3,  1, pSRC->stFINE.cbcr_swap);  // cb_flag_a
	MISP_RegField(MISP_LOCAL_ID, 0x001,  1,  1, pSRC->stFINE.tf_polar);   // positive_top_field_a (0:low, 1:high)
	MISP_RegField(MISP_LOCAL_ID, 0x002,  2,  1, (pSRC->stATTB.attb&MISP_POSITIVE_HSYNC)?1:0);  // positive_hsync_a (0:low, 1:high)
	MISP_RegField(MISP_LOCAL_ID, 0x002,  1,  1, (pSRC->stATTB.attb&MISP_POSITIVE_VSYNC)?1:0);  // positive_vsync_a (0:low, 1:high)

	//deint
	if(pSRC->stATTB.attb&MISP_SCANTYPE_PROG) {
		MISP_RegField(MISP_LOCAL_ID, 0x002, 0, 1, 1);  // progressive_a_i (0:inter. scan, 1:prog. scan)
		MISP_RegField(MISP_LOCAL_ID, 0x200, 3, 1, 0);  // den (0:disable, 1:enable)
		MISP_RegField(MISP_LOCAL_ID, 0x083, 6, 1, 0);  // skip_disable (0:frame skip enable. 1:frame skip disable)
		MISP_RegField(MISP_LOCAL_ID, 0x200, 6, 1, 0);  // v_size_half_en (adjust i-input top&bot position. (prevent bottom garbage 1 line))
	}
	else {
		MISP_RegField(MISP_LOCAL_ID, 0x002, 0, 1, 0);  // progressive_a_i (0:inter. scan, 1:prog. scan)
		MISP_RegField(MISP_LOCAL_ID, 0x200, 3, 1, 1);  // den (0:disable, 1:enable)
		MISP_RegField(MISP_LOCAL_ID, 0x083, 6, 1, 1);  // skip_disable (0:frame skip enable. 1:frame skip disable)
		MISP_RegField(MISP_LOCAL_ID, 0x200, 6, 1, 1);  // v_size_half_en (adjust i-input top&bot position. (prevent bottom garbage 1 line))
	}
}

//------------------------------------------------------------------------------------
static void INCTRL_SetMainFrmt(BYTE frmt)
{
	PMISP_SRCVIDEO_FINE pFINE;
	PMISP_SRCVIDEO_ATTB pATTB;
	
	//get ATTB
	pATTB = &stINCTRL.stSRC_m.stATTB;
	memcpy(pATTB, &defMISPSrcVideo[frmt], sizeof(MISP_SRCVIDEO_ATTB));	// copy v.inp info.
	
#if defined(__MDIN_i5XX_FPGA__) // fpga b'd only
	pATTB->attb &=~MISP_POSITIVE_HSYNC;
	pATTB->attb &=~MISP_POSITIVE_VSYNC;
#endif

	switch(GetIN_MainMode())	{
		case  MISP_SRC_EMB422_8:	case  MISP_SRC_EMB422_10:	
		case  MISP_SRC_MUX656_8:	case  MISP_SRC_MUX656_10:	
		pATTB->attb &=~MISP_POSITIVE_HSYNC;
		pATTB->attb &=~MISP_POSITIVE_VSYNC;
		break;
		default:		break;
	}

	//get FINE
	pFINE = &stINCTRL.stSRC_m.stFINE;
	memset(pFINE, 0, sizeof(MISP_SRCVIDEO_FINE));
// #if defined(__MDIN_i5XX_FPGA__)
	if(GetIN_MainPath()!=MISP_MAINPATH_SENSOR) {
		pFINE->offH=defDVIRXOffset[frmt][0];
		pFINE->offV=defDVIRXOffset[frmt][1];
	}
// #endif

	switch(GetIN_MainMode())	{
		case  MISP_SRC_EMB422_8:	case  MISP_SRC_EMB422_10:	
		case  MISP_SRC_MUX656_8:	case  MISP_SRC_MUX656_10:	
		pFINE->offH=0;
		pFINE->offV=0;
		break;
		default:		break;
	}

}

//------------------------------------------------------------------------------------
static void INCTRL_SetMainMode(BYTE mode)
{
	WORD 	in_mode, dtr_dec_en, in_656, in_422, csc_set;
	BYTE mode_10bit;
	
	switch(mode)	{
		case  MISP_SRC_RGB444_8:	case  MISP_SRC_RGB444_10:
		case  MISP_SRC_YUV444_8:	case  MISP_SRC_YUV444_10:	in_mode=0; dtr_dec_en=0; in_656=0; in_422=0;	break;
		case  MISP_SRC_SEP422_8:	case  MISP_SRC_SEP422_10:	in_mode=1; dtr_dec_en=0; in_656=0; in_422=1;	break;
		case  MISP_SRC_EMB422_8:	case  MISP_SRC_EMB422_10:	in_mode=3; dtr_dec_en=5; in_656=0; in_422=1;	break;
		case  MISP_SRC_MUX656_8:	case  MISP_SRC_MUX656_10:	in_mode=2; dtr_dec_en=5; in_656=1; in_422=1;	break;
		case  MISP_SRC_SEP656_8:	case  MISP_SRC_SEP656_10:	in_mode=2; dtr_dec_en=0; in_656=1; in_422=1;	break;
		default:												in_mode=0; dtr_dec_en=0; in_656=0; in_422=0;	break;
	}
	mode_10bit = (mode%2);

	//port.A
	MISP_RegField(MISP_LOCAL_ID, 0x002,  4,  2, in_mode);		// d_in_mode_a_i (0:444, 1:422SEP, 2:656, 3:422EMB)
	MISP_RegField(MISP_LOCAL_ID, 0x020, 13,  1, mode_10bit);	// tenbit_input_a (0:8bit, 1:10bit)

	//path.main
	MISP_RegField(MISP_LOCAL_ID, 0x040,  8,  1, mode_10bit);	// tenbit_mode (0:8bit, 1:10bit)
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	if (GetIN_MainPath() != 3 ) MISP_RegField(MISP_LOCAL_ID, 0x01E,  8,  2, in_422<<1|1);	// ycbcr_input422_to_444_in_sync(9), ycbcr_input444(8)
	else { // Ext2IPP mode : only for i550
		MISP_RegRead(MISP_LOCAL_ID, 0x390, &csc_set) ;
		if ( csc_set == 0x8000 ) MISP_RegField(MISP_LOCAL_ID, 0x01E,  8,  2, in_422<<1|1); //444
		else 					 MISP_RegField(MISP_LOCAL_ID, 0x01E,  8,  2, 3); //422                    
	}
#endif

	//dtr
	MISP_RegField(MISP_LOCAL_ID, 0x040, 12,  4, dtr_dec_en);	// dtr_dec_enable (0:sync-sep, 5:sync-emb)
#if defined(__MDIN_i5XX_FPGA__)		// FPGA board only
//	MISP_RegField(MISP_LOCAL_ID, 0x04A,  8,  1, in_422);		// dtr_out_422_cb_for_cr_yc (0:422SEP 1: 656EMB or 422EMB)
//	MISP_RegField(MISP_LOCAL_ID, 0x020,  9,  1, (in_656)?1:0);	// dtr_on_chroma_a (0:DTR on Y,  1:DTR on C -- why ?????)
#endif
  
	SetCLK_DivYC((in_656)? 1: 0);
	if(GetDDRMAP_IPC_Bitmode()!=mode_10bit)  SetDDRMAP_IPC_Bitmode(mode_10bit);
}

//------------------------------------------------------------------------------------
static void INCTRL_SetMainFRconfig(PMISP_FRCONFIG_INFO pINFO)
{
	WORD	in_frame_config, in_num_frames_y, in_frame_delay_y, in_num_frames_c, in_frame_delay_c;
	WORD	y_num_frame, c_num_frame;
	
	in_frame_config = pINFO->mode;
	in_num_frames_y = pINFO->Y_cnt;		in_frame_delay_y = pINFO->Y_dly;
	in_num_frames_c = pINFO->C_cnt;		in_frame_delay_c = pINFO->C_dly;
	y_num_frame = pINFO->Y_cnt;			c_num_frame = pINFO->C_cnt;
	
	MISP_RegField(MISP_LOCAL_ID, 0x01e,  0,  2, in_frame_config);		// frame/delay, 0:2/0, 1:3/1, 2:1/0, 3:custom mode
	MISP_RegField(MISP_LOCAL_ID, 0x01f, 12,  4, in_num_frames_y);
	MISP_RegField(MISP_LOCAL_ID, 0x01f,  8,  4, in_frame_delay_y);
	MISP_RegField(MISP_LOCAL_ID, 0x01f,  4,  4, in_num_frames_c);
	MISP_RegField(MISP_LOCAL_ID, 0x01f,  0,  4, in_frame_delay_c);
	MISP_RegField(MISP_LOCAL_ID, 0x202,  4,  4, y_num_frame);			// ipc mvf_count_value
	MISP_RegField(MISP_LOCAL_ID, 0x202,  0,  4, c_num_frame);
}

//------------------------------------------------------------------------------------
static void INCTRL_SetMainHPF(PMISP_HPF_INFO pHPF)
{
	MISP_MultiWrite(MISP_LOCAL_ID, 0x380, (PBYTE)pHPF->pCoef, sizeof(MISP_PEAKING_COEF));
	MISP_RegField(MISP_LOCAL_ID, 0x389, 8, 8, pHPF->level);
	MISP_RegWrite(MISP_LOCAL_ID, 0x388, (pHPF->cor_value<<12)|(pHPF->sat_value<<8)|(pHPF->cor_en<<2)|(pHPF->sat_en<<1)|(pHPF->en));
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	MISP_RegField(MISP_LOCAL_ID, 0x388, 3, 2, (pHPF->cor_edge_dis<<1)|(pHPF->cor_half_en));
	MISP_RegField(MISP_LOCAL_ID, 0x389, 3, 1, pHPF->level_depend_en);
	MISP_RegField(MISP_LOCAL_ID, 0x38A, 0, 9, (pHPF->level_depend_slope));
	MISP_RegWrite(MISP_LOCAL_ID, 0x38B, (pHPF->level_depend_th<<8)|(pHPF->level_depend_offset));
#endif
}

//------------------------------------------------------------------------------------
static void INCTRL_SetMainFrontNR(PMISP_FRONT_NR_INFO pFNR)
{
	PMISP_FNRFILT_COEF pCoef;
	
//	Current version of API support only for 1920x1080i/p input
	// if (GetIN_MainSrcSizeH()!=1920) pFNR->coef = 0; 

	//update coefficient
	pCoef = (PMISP_FNRFILT_COEF)&MISP_FrontNRFilter_Default[pFNR->coef];
	MISP_MultiWrite(MISP_LOCAL_ID, 0x022, (PBYTE)pCoef->y, 16);
	MISP_MultiWrite(MISP_LOCAL_ID, 0x02c, (PBYTE)pCoef->c,  8);

	//configure FNR
	MISP_RegField(MISP_LOCAL_ID, 0x02A, 3, 1, pFNR->enab);  //c_lpf_on
	MISP_RegField(MISP_LOCAL_ID, 0x02A, 0, 1, pFNR->enab);  //y_lpf_on
}

//--------------------------------------------------------------------------------------------------------------------------
static void INCTRL_SetMainCSC(PMISP_SRCVIDEO_INFO pSRC)
{
	PMISP_CSCCTRL_INFO pCSC;
	WORD csc_ctrl;
	WORD temp[6]={0,0,0,0,0,0};
	
#if defined(__ENABLE_INPUT_CSC__)
	MISP_CSCCTRL_INFO stCSC;
	SHORT i, coef[9];
	LONG contrast, saturation, brightness, coshue, sinhue;
#endif
	
	if( (pSRC->mode==MISP_SRC_RGB444_8)||
	    (pSRC->mode==MISP_SRC_RGB444_10) ) {  //csc enable
#if CSC_RANGE_CTRL
		if(pSRC->pCSC)  pCSC=pSRC->pCSC;
		else            pCSC=(PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_HD_StdRange;
#else
		if     (pSRC->stATTB.Vact>=2160)  pCSC=(PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_BT2020;  //UHD
	    else if(pSRC->stATTB.Vact>= 720)  pCSC=(PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_HD_StdRange;  //HD
		else                              pCSC=(PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_SD_StdRange;  //SD
#endif
		csc_ctrl=0x01f8;  //rgb2yuv
	}
	else {  //csc bypass
		pCSC=(PMISP_CSCCTRL_INFO)&MISP_IN_CscBypass_StdRange;  //Bypass
		csc_ctrl=0x01fc|0x0001;  //bypass
	}

#if defined(__ENABLE_INPUT_CSC__)
	memcpy(&stCSC, pCSC, sizeof(MISP_CSCCTRL_INFO));
	
	saturation = (LONG)(WORD)pSRC->stTUNE.saturation;
	contrast = (LONG)(WORD)pSRC->stTUNE.contrast;
	brightness = (LONG)(WORD)pSRC->stTUNE.brightness;
	coshue = (LONG)(SHORT)MISP_CscCosHue[pSRC->stTUNE.hue];
	sinhue = (LONG)(SHORT)MISP_CscSinHue[pSRC->stTUNE.hue];
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
	stCSC.out[0]  = CLIP12((((SHORT)stCSC.out[0])+(brightness-128)*2))&0xfff;

	//update coefficient
	MISP_MultiWrite(MISP_LOCAL_ID, 0x00a, (PBYTE)&stCSC, sizeof(MISP_CSCCTRL_INFO));  // write input csc coef. except csc_ctrl

	//configure CSC
	MISP_RegWrite(MISP_LOCAL_ID, 0x019, csc_ctrl&0xfffe);  // write csc_ctrl (clear bypass)

#else
	//update coefficient
	MISP_MultiWrite(MISP_LOCAL_ID, 0x00a, (PBYTE)pCSC, sizeof(MISP_CSCCTRL_INFO));  // write input csc coef. except csc_ctrl

	//configure CSC
	MISP_RegWrite(MISP_LOCAL_ID, 0x019, csc_ctrl);  // write csc_ctrl
#endif

	if(stINCTRL.ctrl & INCTRL_MAIN_CSC_GRAY_EN)	{  // input csc rgb to gray ??
		MISP_MultiWrite(MISP_LOCAL_ID, 0x00d, (PBYTE)&temp[0], 12);  // clear input csc cb/cr coef.
	}
}

//------------------------------------------------------------------------------------
static void INCTRL_SetAuxPath(WORD aux_inmux_sel)
{
	switch(aux_inmux_sel) {
		case MISP_AUXPATH_EXT_P0:
			MISP_RegField(MISP_LOCAL_ID, 0x049, 2, 2, 0);  // aux_inmux_sel (0:ext_p0, 1:ext_p1, 2:sensor)
			MISP_RegField(MISP_LOCAL_ID, 0x140, 0, 2, 2);  // aux_input_sel (0:main, 1:reser, 2:p_a_ext, 3:p_b_sensor)
			break;
		case MISP_AUXPATH_EXT_P1:
			MISP_RegField(MISP_LOCAL_ID, 0x049, 2, 2, 1);  // aux_inmux_sel (0:ext_p0, 1:ext_p1, 2:sensor)
			MISP_RegField(MISP_LOCAL_ID, 0x140, 0, 2, 2);  // aux_input_sel (0:main, 1:reser, 2:p_a_ext, 3:p_b_sensor)
			MISP_RegField(MISP_LOCAL_ID, 0x049, 5, 1, 1);  // slave_ext_sync_sel (settings for sync locking)						
			break;
		case MISP_AUXPATH_SENSOR:
			MISP_RegField(MISP_LOCAL_ID, 0x049, 2, 2, 2);  // aux_inmux_sel (0:ext_p0, 1:ext_p1, 2:sensor)
			MISP_RegField(MISP_LOCAL_ID, 0x140, 0, 2, 3);  // aux_input_sel (0:main, 1:reser, 2:p_a_ext, 3:p_b_sensor)
			break;
		case MISP_AUXPATH_MAIN_O:
			MISP_RegField(MISP_LOCAL_ID, 0x049, 2, 2, 0);  // aux_inmux_sel (0:ext_p0, 1:ext_p1, 2:sensor)
			MISP_RegField(MISP_LOCAL_ID, 0x140, 0, 2, 0);  // aux_input_sel (0:main, 1:reser, 2:p_a_ext, 3:p_b_sensor)
			break;
	}
}

//------------------------------------------------------------------------------------
static void INCTRL_SetAuxFrmt(BYTE frmt)
{
	PMISP_SRCVIDEO_ATTB pATTB;
	PMISP_SRCVIDEO_FINE pFINE;
	
	//get ATTB
	pATTB = &stINCTRL.stSRC_x.stATTB;
	memcpy(pATTB, &defMISPSrcVideo[frmt], sizeof(MISP_SRCVIDEO_ATTB));	// copy v.inp info.
	
#if defined(__MDIN_i5XX_FPGA__)  // fpga b'd only
	pATTB->attb &=~MISP_POSITIVE_HSYNC;
	pATTB->attb &=~MISP_POSITIVE_VSYNC;
#endif

	//get FINE
	pFINE = &stINCTRL.stSRC_x.stFINE;
	memset(pFINE, 0, sizeof(MISP_SRCVIDEO_FINE));
// #if defined(__MDIN_i5XX_FPGA__)
	if(GetIN_AuxPath()!=MISP_AUXPATH_SENSOR) {
		pFINE->offH=defDVIRXOffset[frmt][0];
		pFINE->offV=defDVIRXOffset[frmt][1];
	}
// #endif
}

//------------------------------------------------------------------------------------
static void INCTRL_SetAuxMode(BYTE mode)
{
	WORD 	in_mode, dtr_dec_en, in_656, in_422;
	BYTE mode_10bit;
	
	switch(mode)	{
		case  MISP_SRC_RGB444_8:	case  MISP_SRC_RGB444_10:
		case  MISP_SRC_YUV444_8:	case  MISP_SRC_YUV444_10:	in_mode=0; dtr_dec_en=0; in_656=0; in_422=0;	break;
		case  MISP_SRC_SEP422_8:	case  MISP_SRC_SEP422_10:	in_mode=1; dtr_dec_en=0; in_656=0; in_422=1;	break;
		case  MISP_SRC_EMB422_8:	case  MISP_SRC_EMB422_10:	in_mode=3; dtr_dec_en=5; in_656=0; in_422=1;	break;
		case  MISP_SRC_MUX656_8:	case  MISP_SRC_MUX656_10:	in_mode=2; dtr_dec_en=5; in_656=1; in_422=1;	break;
		case  MISP_SRC_SEP656_8:	case  MISP_SRC_SEP656_10:	in_mode=2; dtr_dec_en=0; in_656=1; in_422=1;	break;
		default:												in_mode=0; dtr_dec_en=0; in_656=0; in_422=0;	break;
	}
	mode_10bit = (mode%2);

	//port.B
	MISP_RegField(MISP_LOCAL_ID, 0x002, 12,  2, in_mode);		// d_in_mode_b_i (0:res, 1:422SEP, 2:656, 3:422EMB)
	MISP_RegField(MISP_LOCAL_ID, 0x021, 13,  1, mode_10bit);	// tenbit_input_b (0:8bit, 1:10bit)

	//path.aux
	MISP_RegField(MISP_LOCAL_ID, 0x140, 12,  1, mode_10bit);	// aux_tenbit_mode (0:8bit, 1:10bit)
	MISP_RegField(MISP_LOCAL_ID, 0x140,  3,  1, in_422);		// aux_c_in_422 (0:444, 1:422)

	//dtr
	if(dtr_dec_en==5)	MISP_RegField(MISP_LOCAL_ID, 0x040, 12,  4, dtr_dec_en);	// dtr_dec_enable (0:sync-sep, 5:sync-emb)
#if defined(__MDIN_i5XX_FPGA__)		// FPGA board only
	MISP_RegField(MISP_LOCAL_ID, 0x04A,  9,  1, in_422);		// dtr_out_422_cb_for_cr_aux (0:422SEP 1: 656EMB or 422EMB)
	MISP_RegField(MISP_LOCAL_ID, 0x021,  9,  1, (in_656)?1:0);	// dtr_on_chroma_b (0:DTR on Y,  1:DTR on C -- why ?????)
#endif
	
	SetCLK_DivAux((in_656)? 1: 0);
	if(GetDDRMAP_AUX_Bitmode()!=mode_10bit)  SetDDRMAP_AUX_Bitmode(mode_10bit);
}

//------------------------------------------------------------------------------------
static void INCTRL_SetAuxFRconfig(PMISP_FRCONFIG_INFO pINFO)
{
	WORD	frame_config, num_frames, frame_delay;
	
	frame_config = pINFO->mode;		num_frames = pINFO->Y_cnt;
	frame_delay = pINFO->Y_dly;
	
	MISP_RegField(MISP_LOCAL_ID, 0x14a,  8,  1, frame_config);
	MISP_RegField(MISP_LOCAL_ID, 0x14a,  4,  4, num_frames);
	MISP_RegField(MISP_LOCAL_ID, 0x14a,  0,  4, frame_delay);
}

//--------------------------------------------------------------------------------------------------------------------------
static void INCTRL_SetAuxCSC(PMISP_SRCVIDEO_INFO pSRC)
{
	PMISP_CSCCTRL_INFO pCSC;
	WORD csc_ctrl;
	WORD temp[6]={0,0,0,0,0,0};
#if defined(__ENABLE_INPUT_CSC__)
	MISP_CSCCTRL_INFO stCSC;
	SHORT i, coef[9];
	LONG contrast, saturation, brightness, coshue, sinhue;
#else
#if defined(__USE_MDIN_i500__)
	SHORT i;
#endif
#endif

	if( (pSRC->mode==MISP_SRC_RGB444_8)||
	    (pSRC->mode==MISP_SRC_RGB444_10) ) {  //csc enable
#if CSC_RANGE_CTRL
		if(pSRC->pCSC)  pCSC=pSRC->pCSC;
		else            pCSC=(PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_HD_StdRange;
#else
		if     (pSRC->stATTB.Vact>=2160)  pCSC=(PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_BT2020;  //UHD
	    else if(pSRC->stATTB.Vact>= 720)  pCSC=(PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_HD_StdRange;  //HD
		else                              pCSC=(PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_SD_StdRange;  //SD
#endif
		csc_ctrl=0x7f3c;  //rgb2yuv
	}
	else {  //csc bypass
		pCSC=(PMISP_CSCCTRL_INFO)&MISP_IN_CscBypass_StdRange;  //Bypass
		csc_ctrl=0x7f7c|0x8000;  //bypass
	}

#if defined(__ENABLE_INPUT_CSC__)
	memcpy(&stCSC, pCSC, sizeof(MISP_CSCCTRL_INFO));
	
	saturation = (LONG)(WORD)pSRC->stTUNE.saturation;
	contrast = (LONG)(WORD)pSRC->stTUNE.contrast;
	brightness = (LONG)(WORD)pSRC->stTUNE.brightness;
	coshue = (LONG)(SHORT)MISP_CscCosHue[pSRC->stTUNE.hue];
	sinhue = (LONG)(SHORT)MISP_CscSinHue[pSRC->stTUNE.hue];
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
	stCSC.out[0]  = CLIP12((((SHORT)stCSC.out[0])+(brightness-128)*2))&0xfff;

	//update coefficient
  #if defined(__USE_MDIN_i500__) 			// MDIN-i500
	for(i=0; i<9; i++) MISP_RegWrite(MISP_LOCAL_ID, 0x181+i, stCSC.coef[i]);
	for(i=0; i<3; i++) MISP_RegWrite(MISP_LOCAL_ID, 0x18A+i, stCSC.in[i]/4);
	for(i=0; i<3; i++) MISP_RegWrite(MISP_LOCAL_ID, 0x18D+i, stCSC.out[i]/4);	
  #else 									// MDIN-i51X (i510, i540, i550)
	MISP_MultiWrite(MISP_LOCAL_ID, 0x181, (PBYTE)&stCSC, sizeof(MISP_CSCCTRL_INFO));  // write input csc coef. except csc_ctrl
  #endif

	//configure CSC
	MISP_RegWrite(MISP_LOCAL_ID, 0x180, csc_ctrl&0x7fff);  // write csc_ctrl (clear bypass)

#else

	//update coefficient
  #if defined(__USE_MDIN_i500__) 			// MDIN-i500
	for(i=0; i<9; i++) MISP_RegWrite(MISP_LOCAL_ID, 0x181+i, pCSC->coef[i]);
	for(i=0; i<3; i++) MISP_RegWrite(MISP_LOCAL_ID, 0x18A+i, pCSC->in[i]/4);
	for(i=0; i<3; i++) MISP_RegWrite(MISP_LOCAL_ID, 0x18D+i, pCSC->out[i]/4);	
  #else 									// MDIN-i51X (i510, i540, i550)
	MISP_MultiWrite(MISP_LOCAL_ID, 0x181, (PBYTE)pCSC, sizeof(MISP_CSCCTRL_INFO));  // write input csc coef. except csc_ctrl
  #endif

	//configure CSC
	MISP_RegWrite(MISP_LOCAL_ID, 0x180, csc_ctrl);  // write csc_ctrl

#endif  //defined(__ENABLE_INPUT_CSC__)
	
	if(stINCTRL.ctrl & INCTRL_AUX_CSC_GRAY_EN)	{  // input csc rgb to gray ??
		MISP_MultiWrite(MISP_LOCAL_ID, 0x184, (PBYTE)&temp[0], 12);  // clear input csc cb/cr coef.
	}
}

//------------------------------------------------------------------------------------
static void INCTRL_SetAuxAttb(PMISP_SRCVIDEO_INFO pSRC)
{
	MISP_RegField(MISP_LOCAL_ID, 0x008,  0, 13, pSRC->stATTB.Hact);       // hsize_b
	MISP_RegField(MISP_LOCAL_ID, 0x009,  0, 13, pSRC->stATTB.Vact);       // vsize_b

	MISP_RegField(MISP_LOCAL_ID, 0x005,  0, 10, pSRC->stFINE.offH);       // active_h_offset_b [9:0]
	MISP_RegField(MISP_LOCAL_ID, 0x008, 13,  3, pSRC->stFINE.offH>>10);   // active_h_offset_b [12:10]
	MISP_RegField(MISP_LOCAL_ID, 0x005, 10,  6, pSRC->stFINE.offV);       // active_v_offset_b [5:0]
	MISP_RegField(MISP_LOCAL_ID, 0x003,  0,  7, pSRC->stFINE.offV>>6);    // active_v_offset_b [12:6]

	MISP_RegField(MISP_LOCAL_ID, 0x000,  8,  3, pSRC->stFINE.yc_offset);  // yc_offset_b
	MISP_RegField(MISP_LOCAL_ID, 0x000,  2,  1, pSRC->stFINE.cbcr_swap);  // cb_flag_b
	MISP_RegField(MISP_LOCAL_ID, 0x001,  0,  1, pSRC->stFINE.tf_polar);   // positive_top_field_b (0:low, 1:high)
	MISP_RegField(MISP_LOCAL_ID, 0x002, 10,  1, (pSRC->stATTB.attb&MISP_POSITIVE_HSYNC)?1:0);  // positive_hsync_b (0:low, 1:high)
	MISP_RegField(MISP_LOCAL_ID, 0x002,  9,  1, (pSRC->stATTB.attb&MISP_POSITIVE_VSYNC)?1:0);  // positive_vsync_b (0:low, 1:high)

	//deint
	if(pSRC->stATTB.attb&MISP_SCANTYPE_PROG) {
		MISP_RegField(MISP_LOCAL_ID, 0x002, 8, 1, 1);  // progressive_b_i (0:inter. scan, 1:prog. scan)
		MISP_RegField(MISP_LOCAL_ID, 0x140, 2, 1, 1);  // aux_prog_input (0:interlace, 1:prog.)
		MISP_RegField(MISP_LOCAL_ID, 0x144, 7, 1, 0);  // aux_skip_disable (0:frame skip enable 1:frame skip disable)
	}
	else {
		MISP_RegField(MISP_LOCAL_ID, 0x002, 8, 1, 0);  // progressive_b_i (0:inter. scan, 1:prog. scan)
		MISP_RegField(MISP_LOCAL_ID, 0x140, 2, 1, 0);  // aux_prog_input (0:interlace, 1:prog.)
		MISP_RegField(MISP_LOCAL_ID, 0x144, 7, 1, 1);  // aux_skip_disable (0:frame skip enable 1:frame skip disable)
	}
}

//------------------------------------------------------------------------------------
static void INCTRL_SetEncPath(WORD path)
{
	BYTE venc_input_sel, venc_src_sel;
	
	switch (path) {
		case MISP_VENC_PATH_PORT_A:		venc_input_sel=2;	venc_src_sel=0;	break;
		case MISP_VENC_PATH_PORT_B:		venc_input_sel=2;	venc_src_sel=2;	break;
		case MISP_VENC_PATH_PORT_X:
		default:						venc_input_sel=0;	venc_src_sel=0;	break;
	}
	
	MISP_RegField(MISP_LOCAL_ID, 0x01e, 12, 2, venc_src_sel);	// 0:A1, 1:A2, 2:B1, 3:B2
	MISP_RegField(MISP_LOCAL_ID, 0x1e0, 12, 2, venc_input_sel);	// 0:aux out, 1:input video(venc_src_sel)
}

//------------------------------------------------------------------------------------
static void INCTRL_PinMapHandler(PIN_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&INCTRL_UPDATE_PINMAP)==0) return;
	pCTRL->ctrl &= (~INCTRL_UPDATE_PINMAP);
	
	INCTRL_SetPinMAP(&pCTRL->stPinMap);
}

//------------------------------------------------------------------------------------
static void INCTRL_MainHandler(PIN_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&INCTRL_UPDATE_MAIN)==0) return;
	pCTRL->ctrl &= (~INCTRL_UPDATE_MAIN);
	
	INCTRL_SetMainPath(pCTRL->MainPATH);
	INCTRL_SetMainMode(pCTRL->stSRC_m.mode);
	INCTRL_SetMainAttb(&pCTRL->stSRC_m);
	INCTRL_SetMainCSC(&pCTRL->stSRC_m);
}

//------------------------------------------------------------------------------------
static void INCTRL_AuxHandler(PIN_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&INCTRL_UPDATE_AUX)==0) return;
	pCTRL->ctrl &= (~INCTRL_UPDATE_AUX);
	
	INCTRL_SetAuxPath(pCTRL->AuxPATH);
	INCTRL_SetAuxMode(pCTRL->stSRC_x.mode);
	INCTRL_SetAuxAttb(&pCTRL->stSRC_x);
	INCTRL_SetAuxCSC(&pCTRL->stSRC_x);
}

//------------------------------------------------------------------------------------
static void INCTRL_EncHandler(PIN_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&INCTRL_UPDATE_ENC)==0) return;
	pCTRL->ctrl &= (~INCTRL_UPDATE_ENC);
	
	INCTRL_SetEncPath(pCTRL->EncPATH);
}

//------------------------------------------------------------------------------------
static void INCTRL_MainHPFHandler(PIN_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&INCTRL_UPDATE_MAIN_HPF)==0) return;
	pCTRL->ctrl &= (~INCTRL_UPDATE_MAIN_HPF);
	
	INCTRL_SetMainHPF(&pCTRL->stHPF_m);
}

//------------------------------------------------------------------------------------
static void INCTRL_MainFRConfigHandler(PIN_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&INCTRL_UPDATE_MAIN_FRAME)==0) return;
	pCTRL->ctrl &= (~INCTRL_UPDATE_MAIN_FRAME);
	
	INCTRL_SetMainFRconfig(&pCTRL->FRcfg_m);
}

//------------------------------------------------------------------------------------
static void INCTRL_AuxFRConfigHandler(PIN_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&INCTRL_UPDATE_AUX_FRAME)==0) return;
	pCTRL->ctrl &= (~INCTRL_UPDATE_AUX_FRAME);
	
	INCTRL_SetAuxFRconfig(&pCTRL->FRcfg_x);
}

//------------------------------------------------------------------------------------
static void INCTRL_MainFNRHandler(PIN_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&INCTRL_UPDATE_MAIN_NR)==0) return;
	pCTRL->ctrl &= (~INCTRL_UPDATE_MAIN_NR);
	
	INCTRL_SetMainFrontNR(&pCTRL->stFNR_m);
}

//------------------------------------------------------------------------------------
static void INCTRL_MainCSCHandler(PIN_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&INCTRL_UPDATE_MAIN_CSC)==0) return;
	pCTRL->ctrl &= (~INCTRL_UPDATE_MAIN_CSC);
	
	INCTRL_SetMainCSC(&pCTRL->stSRC_m);
}

//------------------------------------------------------------------------------------
static void INCTRL_AuxCSCHandler(PIN_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&INCTRL_UPDATE_AUX_CSC)==0) return;
	pCTRL->ctrl &= (~INCTRL_UPDATE_AUX_CSC);
	
	INCTRL_SetAuxCSC(&pCTRL->stSRC_x);
}

//--------------------------------------------------------------------------------------
static void Init_IN_REGISTER( PIN_CTRL_PARAM pCTRL ) 
{
	INCTRL_SetPinMAP(&pCTRL->stPinMap);

	INCTRL_SetMainFrmt(pCTRL->stSRC_m.frmt);
	INCTRL_SetMainPath(pCTRL->MainPATH);
	INCTRL_SetMainMode(pCTRL->stSRC_m.mode);
	INCTRL_SetMainAttb(&pCTRL->stSRC_m);
	INCTRL_SetMainCSC(&pCTRL->stSRC_m);
	INCTRL_SetMainFRconfig(&pCTRL->FRcfg_m);
	INCTRL_SetMainHPF(&pCTRL->stHPF_m);
	INCTRL_SetMainFrontNR(&pCTRL->stFNR_m);

	INCTRL_SetAuxFrmt(pCTRL->stSRC_x.frmt);
	INCTRL_SetAuxPath(pCTRL->AuxPATH);
	INCTRL_SetAuxMode(pCTRL->stSRC_x.mode);
	INCTRL_SetAuxAttb(&pCTRL->stSRC_x);
	INCTRL_SetAuxCSC(&pCTRL->stSRC_x);
	INCTRL_SetAuxFRconfig(&pCTRL->FRcfg_x);

	INCTRL_SetEncPath(pCTRL->EncPATH);

	//set clock
	INCTRL_SetMainPathCLK();
	INCTRL_SetAuxPathCLK();

	//convert to api code!!!!
	MISP_RegField(MISP_LOCAL_ID, 0x20A, 3, 2, 0);	// ipc : adjust ipc edge performance in fast move image.
	MISP_RegField(MISP_LOCAL_ID, 0x041, 0, 1, 1);	// misc : main read enable, sensor input(disable ??)
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	MISP_RegField(MISP_LOCAL_ID, 0x04f, 8, 4, 6);	// yc_hv_delay : hsync 4 clk delay .. for YCNR .. color_enh_filter...
#else   							// MDIN-i51X (i510, i540, i550)
	MISP_RegField(MISP_LOCAL_ID, 0x04f, 8, 4, 5);	// yc_hv_delay : hsync 4 clk delay .. for YCNR .. color_enh_filter...
#endif
}


//api
//----------------------------------------------------------------------------------------
void Init_IN(void)
{
	memset(&stINCTRL, 0, sizeof(IN_CTRL_PARAM));
	Init_IN_PARAM(&stINCTRL);
	Init_IN_REGISTER(&stINCTRL);
}

//----------------------------------------------------------------------------------------
void IN_ProcessHandler(void)
{
	if ((stINCTRL.ctrl&INCTRL_TRIGGER_ISP)==0) return;
	stINCTRL.ctrl &= (~INCTRL_TRIGGER_ISP);
	
	INCTRL_PinMapHandler(&stINCTRL);
	INCTRL_MainHandler(&stINCTRL);
	INCTRL_MainCSCHandler(&stINCTRL);
	INCTRL_MainFRConfigHandler(&stINCTRL);
	INCTRL_MainHPFHandler(&stINCTRL);
	INCTRL_MainFNRHandler(&stINCTRL);
	
	INCTRL_AuxHandler(&stINCTRL);
	INCTRL_AuxCSCHandler(&stINCTRL);
	INCTRL_AuxFRConfigHandler(&stINCTRL);
	
	INCTRL_EncHandler(&stINCTRL);
}



//get/set func.
//-------------------------------------------------------------------------------------
void SetIN_MainSrcExtPort0(void)
{
	if(GetIN_MainPath() == MISP_MAINPATH_EXT_P0)		return;
	
	stINCTRL.MainPATH = MISP_MAINPATH_EXT_P0;
	memcpy(&stINCTRL.stSRC_m, &stINCTRL.stSRC_extp0, sizeof(MISP_SRCVIDEO_INFO));	// main <- extp0
	
	SetIN_MainFrmt(stINCTRL.stSRC_m.frmt);
	SetFC_MainFCsrcPosiX(0);  SetFC_MainFCsrcSizeH(GetIN_MainSrcSizeH());
	SetFC_MainFCsrcPosiY(0);  SetFC_MainFCsrcSizeV(GetIN_MainSrcSizeV());
	
	SetDDRMAP_main_size(GetIN_MainSrcSizeH(),GetIN_MainSrcSizeV());
	
	INCTRL_SetMainPathCLK();
}

//-------------------------------------------------------------------------------------
void SetIN_MainSrcExtPort1(void)
{
	if(GetIN_MainPath() == MISP_MAINPATH_EXT_P1)		return;
	
	stINCTRL.MainPATH = MISP_MAINPATH_EXT_P1;
	memcpy(&stINCTRL.stSRC_m, &stINCTRL.stSRC_extp1, sizeof(MISP_SRCVIDEO_INFO));	// main <- extp1
	
	SetIN_MainFrmt(stINCTRL.stSRC_m.frmt);
	SetFC_MainFCsrcPosiX(0);  SetFC_MainFCsrcSizeH(GetIN_MainSrcSizeH());
	SetFC_MainFCsrcPosiY(0);  SetFC_MainFCsrcSizeV(GetIN_MainSrcSizeV());
	
	SetDDRMAP_main_size(GetIN_MainSrcSizeH(),GetIN_MainSrcSizeV());
	
	INCTRL_SetMainPathCLK();
}

//-------------------------------------------------------------------------------------
void SetIN_MainSrcSensor(void)
{
	if(GetIN_MainPath() == MISP_MAINPATH_SENSOR)		return;
	
	stINCTRL.MainPATH = MISP_MAINPATH_SENSOR;
	memcpy(&stINCTRL.stSRC_m, &stINCTRL.stSRC_sensor, sizeof(MISP_SRCVIDEO_INFO));	// main <- sensor
	
	SetIN_MainFrmt(stINCTRL.stSRC_m.frmt);
	SetFC_MainFCsrcPosiX(0);  SetFC_MainFCsrcSizeH(GetIN_MainSrcSizeH());
	SetFC_MainFCsrcPosiY(0);  SetFC_MainFCsrcSizeV(GetIN_MainSrcSizeV());
	
	SetDDRMAP_main_size(GetSS_image_active_size_h() ,GetSS_image_active_size_v());	
	
	INCTRL_SetMainPathCLK();
}

//-------------------------------------------------------------------------------------
void SetIN_AuxSrcExtPort0(void)
{
	if(GetIN_AuxPath() == MISP_AUXPATH_EXT_P0)		return;
	
	stINCTRL.AuxPATH = MISP_AUXPATH_EXT_P0;
	memcpy(&stINCTRL.stSRC_x, &stINCTRL.stSRC_extp0, sizeof(MISP_SRCVIDEO_INFO));	// aux <- extp0
	
	SetIN_AuxFrmt(stINCTRL.stSRC_x.frmt);
	SetFC_AuxFCsrcPosiX(0); 	SetFC_AuxFCsrcSizeH(GetIN_AuxSrcSizeH());
	SetFC_AuxFCsrcPosiY(0); 	SetFC_AuxFCsrcSizeV(GetIN_AuxSrcSizeV());
	
	SetDDRMAP_aux_size(GetIN_AuxSrcSizeH(), GetIN_AuxSrcSizeV());	
	
	INCTRL_SetAuxPathCLK();
}

//-------------------------------------------------------------------------------------
void SetIN_AuxSrcExtPort1(void)
{
	if(GetIN_AuxPath() == MISP_AUXPATH_EXT_P1)		return;
	
	stINCTRL.AuxPATH = MISP_AUXPATH_EXT_P1;
	memcpy(&stINCTRL.stSRC_x, &stINCTRL.stSRC_extp1, sizeof(MISP_SRCVIDEO_INFO));	// aux <- extp1
	
	SetIN_AuxFrmt(stINCTRL.stSRC_x.frmt);
	SetFC_AuxFCsrcPosiX(0); 	SetFC_AuxFCsrcSizeH(GetIN_AuxSrcSizeH());
	SetFC_AuxFCsrcPosiY(0); 	SetFC_AuxFCsrcSizeV(GetIN_AuxSrcSizeV());
	
	SetDDRMAP_aux_size(GetIN_AuxSrcSizeH(), GetIN_AuxSrcSizeV());	
	
	INCTRL_SetAuxPathCLK();
}

//-------------------------------------------------------------------------------------
void SetIN_AuxSrcSensor(void)
{
	if(GetIN_AuxPath() == MISP_AUXPATH_SENSOR)		return;
	
	stINCTRL.AuxPATH = MISP_AUXPATH_SENSOR;
	memcpy(&stINCTRL.stSRC_x, &stINCTRL.stSRC_sensor, sizeof(MISP_SRCVIDEO_INFO));	// aux <- sensor
	
	SetIN_AuxFrmt(stINCTRL.stSRC_x.frmt);
	SetFC_AuxFCsrcPosiX(0); 	SetFC_AuxFCsrcSizeH(GetIN_AuxSrcSizeH());
	SetFC_AuxFCsrcPosiY(0); 	SetFC_AuxFCsrcSizeV(GetIN_AuxSrcSizeV());
	
	SetDDRMAP_aux_size(GetIN_AuxSrcSizeH(), GetIN_AuxSrcSizeV());
	
	INCTRL_SetAuxPathCLK();
}

//-------------------------------------------------------------------------------------
void SetIN_AuxSrcMainOut(void)
{
	if(GetIN_AuxPath() == MISP_AUXPATH_MAIN_O)		return;
	
	stINCTRL.AuxPATH = MISP_AUXPATH_MAIN_O;
	
	SetIN_AuxFrmt(MIN(GetOUT_MainFrmt(), MISP_VIDOUT_1920x1200pRB));
	SetFC_AuxFCsrcPosiX(0); 	SetFC_AuxFCsrcSizeH(GetIN_AuxSrcSizeH());
	SetFC_AuxFCsrcPosiY(0); 	SetFC_AuxFCsrcSizeV(GetIN_AuxSrcSizeV());
	
	SetIN_AuxMode(GetOUT_MainMode());
	
	INCTRL_SetAuxPathCLK();
}

//-------------------------------------------------------------------------------------
void SetIN_MainFrmt(BYTE frmt)
{
	stINCTRL.stSRC_m.frmt = frmt;		INCTRL_SetMainFrmt(frmt);
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN | INCTRL_TRIGGER_ISP);	// main update
	
	SetDDRMAP_main_size(GetIN_MainSrcSizeH(),GetIN_MainSrcSizeV());
}

//-------------------------------------------------------------------------------------
BYTE GetIN_MainFrmt(void)
{
	return stINCTRL.stSRC_m.frmt;
}

//-------------------------------------------------------------------------------------
void SetIN_MainMode(BYTE mode)
{
	stINCTRL.stSRC_m.mode = mode;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN | INCTRL_TRIGGER_ISP);	// main update
}

//-------------------------------------------------------------------------------------
void SetIN_ExtPort0Frmt(WORD frmt)
{
	stINCTRL.stSRC_extp0.frmt = frmt ;
	
	if(GetIN_MainPath() == MISP_MAINPATH_EXT_P0)	{
		SetIN_MainFrmt(frmt);
		SetFC_MainFCsrcPosiX(0);  SetFC_MainFCsrcSizeH(GetIN_MainSrcSizeH());
		SetFC_MainFCsrcPosiY(0);  SetFC_MainFCsrcSizeV(GetIN_MainSrcSizeV());
	}
	
	if(GetIN_AuxPath() == MISP_AUXPATH_EXT_P0)	{
		SetIN_AuxFrmt(frmt);
		SetFC_AuxFCsrcPosiX(0); 	SetFC_AuxFCsrcSizeH(GetIN_AuxSrcSizeH());
		SetFC_AuxFCsrcPosiY(0); 	SetFC_AuxFCsrcSizeV(GetIN_AuxSrcSizeV());
	}
}	
	
//-------------------------------------------------------------------------------------	
void SetIN_ExtPort0Mode(WORD mode)
{
	stINCTRL.stSRC_extp0.mode = mode ;
	
	if ( GetIN_MainPath() == MISP_MAINPATH_EXT_P0 ) SetIN_MainMode(mode) ;
	if ( GetIN_AuxPath() == MISP_AUXPATH_EXT_P0 ) SetIN_AuxMode(mode) ;
}	
	
//-------------------------------------------------------------------------------------	
void SetIN_ExtPort1Frmt(WORD frmt)
{
	stINCTRL.stSRC_extp1.frmt = frmt ;
	
	if(GetIN_MainPath() == MISP_MAINPATH_EXT_P1)	{
		SetIN_MainFrmt(frmt);
		SetFC_MainFCsrcPosiX(0);  SetFC_MainFCsrcSizeH(GetIN_MainSrcSizeH());
		SetFC_MainFCsrcPosiY(0);  SetFC_MainFCsrcSizeV(GetIN_MainSrcSizeV());
	}
	
	if(GetIN_AuxPath() == MISP_AUXPATH_EXT_P1)	{
		SetIN_AuxFrmt(frmt);
		SetFC_AuxFCsrcPosiX(0); 	SetFC_AuxFCsrcSizeH(GetIN_AuxSrcSizeH());
		SetFC_AuxFCsrcPosiY(0); 	SetFC_AuxFCsrcSizeV(GetIN_AuxSrcSizeV());
	}
}	
	
//-------------------------------------------------------------------------------------	
void SetIN_ExtPort1Mode(WORD mode)
{
	stINCTRL.stSRC_extp1.mode = mode ;
	
	if (( GetIN_MainPath() == MISP_MAINPATH_EXT_P1 ) || ( GetIN_MainPath() == MISP_MAINPATH_SENSOR_EXT_P1 )) SetIN_MainMode(mode) ;
	if ( GetIN_AuxPath() == MISP_AUXPATH_EXT_P1 ) SetIN_AuxMode(mode) ;
}	
	
//-------------------------------------------------------------------------------------
void SetIN_MainFRconfig(WORD val)
{
	stINCTRL.FRcfg_m.mode = val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN_FRAME | INCTRL_TRIGGER_ISP);
}

//-------------------------------------------------------------------------------------
void SetIN_MainFRcntY(WORD val)
{
	stINCTRL.FRcfg_m.Y_cnt = val;
	
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN_FRAME | INCTRL_TRIGGER_ISP);
}

//-------------------------------------------------------------------------------------
void SetIN_MainFRdlyY(WORD val)
{
	stINCTRL.FRcfg_m.Y_dly = val;
	
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN_FRAME | INCTRL_TRIGGER_ISP);
}

//-------------------------------------------------------------------------------------
WORD GETIN_MainFRcntY(void)
{
	return stINCTRL.FRcfg_m.Y_cnt ;
}
//-------------------------------------------------------------------------------------
void SetIN_MainFRcntC(WORD val)
{
	stINCTRL.FRcfg_m.C_cnt = val;
	
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN_FRAME | INCTRL_TRIGGER_ISP);
}

//-------------------------------------------------------------------------------------
void SetIN_MainFRdlyC(WORD val)
{
	stINCTRL.FRcfg_m.C_dly = val;

	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN_FRAME | INCTRL_TRIGGER_ISP);
}

//-------------------------------------------------------------------------------------
WORD GETIN_MainFRcntC(void)
{
	return stINCTRL.FRcfg_m.C_cnt ;
}

//-------------------------------------------------------------------------------------
BOOL GetIN_MainHoriPeaking(void)
{
	return stINCTRL.stHPF_m.en ;
}
//-------------------------------------------------------------------------------------
void SetIN_MainHoriPeaking(BOOL OnOff)
{
	stINCTRL.stHPF_m.en = OnOff ;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN_HPF | INCTRL_TRIGGER_ISP);	// main-hpf update
}
//-------------------------------------------------------------------------------------
void SetIN_MainHPFLevel(WORD val)
{
	stINCTRL.stHPF_m.level = (BYTE)val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN_HPF | INCTRL_TRIGGER_ISP);	// main-hpf update
}
//-------------------------------------------------------------------------------------
void SetIN_MainHPFCoeff(WORD val)
{
	if ( val == 0 ) stINCTRL.stHPF_m.pCoef = (PMISP_PEAKING_COEF)&MISP_PeakingFilter_Default;
	else if ( val == 1 ) stINCTRL.stHPF_m.pCoef = (PMISP_PEAKING_COEF)&MISP_PeakingFilter_BPF50_7tab;
	else stINCTRL.stHPF_m.pCoef = (PMISP_PEAKING_COEF)&MISP_PeakingFilter_BPF40_7tab;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN_HPF | INCTRL_TRIGGER_ISP);	// main-hpf update
}
//-------------------------------------------------------------------------------------
BYTE GetIN_MainHPFLevel(void)
{
	return 	stINCTRL.stHPF_m.level;
}

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
//-------------------------------------------------------------------------------------
void SetIN_MainHPFLDthresh(WORD val)
{
	stINCTRL.stHPF_m.level_depend_th = val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN_HPF | INCTRL_TRIGGER_ISP);	// main-hpf update
}	
	
void SetIN_MainHPFLDslope(WORD val)
{
	stINCTRL.stHPF_m.level_depend_slope = val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN_HPF | INCTRL_TRIGGER_ISP);	// main-hpf update
}
#endif

//-------------------------------------------------------------------------------------
void SetIN_MainSrcSizeH(WORD size)
{
	stINCTRL.stSRC_m.stATTB.Hact = size;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN | INCTRL_TRIGGER_ISP);
}

//-------------------------------------------------------------------------------------
void SetIN_MainSrcSizeV(WORD size)
{
	stINCTRL.stSRC_m.stATTB.Vact = size;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN | INCTRL_TRIGGER_ISP);
}

//-------------------------------------------------------------------------------------
WORD GetIN_MainSrcSizeH(void)
{
	return 	stINCTRL.stSRC_m.stATTB.Hact;
}
//-------------------------------------------------------------------------------------
WORD GetIN_MainSrcSizeV(void)
{
	return 	stINCTRL.stSRC_m.stATTB.Vact;
}
//-------------------------------------------------------------------------------------
WORD GetIN_MainSrcSizeHtot(void)
{
	return 	stINCTRL.stSRC_m.stATTB.Htot;
}
//-------------------------------------------------------------------------------------
BOOL GetIN_MainScanTypeProg(void)
{
	return ((stINCTRL.stSRC_m.stATTB.attb & MISP_SCANTYPE_PROG)? 1: 0);	//0:interlace, 1:progressive
}

//-------------------------------------------------------------------------------------
BYTE GetIN_MainPath(void)
{
	return		stINCTRL.MainPATH;
}

//-------------------------------------------------------------------------------------
BYTE GetIN_MainMode(void)
{
	return stINCTRL.stSRC_m.mode ;
}

//-------------------------------------------------------------------------------------
BYTE GetIN_AuxMode(void)
{
	return stINCTRL.stSRC_x.mode ;
}

//-------------------------------------------------------------------------------------
BOOL GetIN_MainBitmode(void)
{
	BYTE rVal ;
	rVal = stINCTRL.stSRC_m.mode%2 ; 
		
	return (BOOL)rVal ;
}	

//-------------------------------------------------------------------------------------
BOOL GetIN_AuxBitmode(void)
{
	BYTE rVal = stINCTRL.stSRC_x.mode%2;
	return (BOOL)rVal ;
}	

//-------------------------------------------------------------------------------------
BYTE GetIN_AuxPath(void)
{
	return	stINCTRL.AuxPATH;
}

//-------------------------------------------------------------------------------------
void SetIN_AuxFrmt(BYTE frmt)
{
	stINCTRL.stSRC_x.frmt = frmt;		INCTRL_SetAuxFrmt(frmt);
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX | INCTRL_TRIGGER_ISP);	// aux update
		
	SetDDRMAP_aux_size(GetIN_AuxSrcSizeH(), GetIN_AuxSrcSizeV());
}

//-------------------------------------------------------------------------------------
BYTE GetIN_AuxFrmt(void)
{
	return stINCTRL.stSRC_x.frmt;
}

//-------------------------------------------------------------------------------------
void SetIN_AuxMode(BYTE mode)
{
	stINCTRL.stSRC_x.mode = mode;
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX | INCTRL_TRIGGER_ISP);	// aux update
}

//-------------------------------------------------------------------------------------
void SetIN_AuxSrcSizeH(WORD size)
{
	stINCTRL.stSRC_x.stATTB.Hact = size;
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX | INCTRL_TRIGGER_ISP);
}

//-------------------------------------------------------------------------------------
void SetIN_AuxSrcSizeV(WORD size)
{
	stINCTRL.stSRC_x.stATTB.Vact = size;
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX | INCTRL_TRIGGER_ISP);
}

//-------------------------------------------------------------------------------------
WORD GetIN_AuxSrcSizeH(void)
{
	return 	stINCTRL.stSRC_x.stATTB.Hact;
}

//-------------------------------------------------------------------------------------
WORD GetIN_AuxSrcSizeV(void)
{
	return 	stINCTRL.stSRC_x.stATTB.Vact;
}

//-------------------------------------------------------------------------------------
void SetIN_AuxFRconfig(WORD val)
{
	stINCTRL.FRcfg_x.mode = val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX_FRAME | INCTRL_TRIGGER_ISP);
}

//-------------------------------------------------------------------------------------
void SetIN_AuxFRcnt(WORD val)
{
	stINCTRL.FRcfg_x.Y_cnt = val;
	
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX_FRAME | INCTRL_TRIGGER_ISP);
}

//-------------------------------------------------------------------------------------
void SetIN_AuxFRdly(WORD val)
{
	stINCTRL.FRcfg_x.Y_dly = val;

	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX_FRAME | INCTRL_TRIGGER_ISP);
}

//-------------------------------------------------------------------------------------
WORD GetIN_AuxFRcnt(void)
{
	return stINCTRL.FRcfg_x.Y_cnt ;
}

//-------------------------------------------------------------------------------------
void SetIN_EncPATH(WORD val)
{
	stINCTRL.EncPATH = val;										// 0:in_a, 1:in_b, 2:aux
	stINCTRL.ctrl |= (INCTRL_UPDATE_ENC | INCTRL_TRIGGER_ISP);	// enc update
}

//-------------------------------------------------------------------------------------
BOOL GetIN_AuxScanTypeProg(void)
{
	return ((stINCTRL.stSRC_x.stATTB.attb & MISP_SCANTYPE_PROG)? 1: 0);	//0:interlace, 1:progressive
}


//add by jins 2016.03
//-------------------------------------------------------------------------------------
void SetIn_MainOffHA(WORD val) {
	stINCTRL.stSRC_m.stFINE.offH = val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
void SetIn_MainOffVA(WORD val) {
	stINCTRL.stSRC_m.stFINE.offV = val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
void SetIn_AuxOffHA(WORD val) {
	stINCTRL.stSRC_x.stFINE.offH = val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
void SetIn_AuxOffVA(WORD val) {
	stINCTRL.stSRC_x.stFINE.offV = val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
WORD GetIn_MainOffHA(void) {
	return stINCTRL.stSRC_m.stFINE.offH;
}

//------------------------------------------------------------------------------------
WORD GetIn_MainOffVA(void) {
	return stINCTRL.stSRC_m.stFINE.offV;
}

//------------------------------------------------------------------------------------
WORD GetIn_AuxOffHA(void) {
	return stINCTRL.stSRC_x.stFINE.offH;
}

//------------------------------------------------------------------------------------
WORD GetIn_AuxOffVA(void) {
	return stINCTRL.stSRC_x.stFINE.offV;
}

//------------------------------------------------------------------------------------
void SetIn_PinMapExtn(BOOL OnOff) {
	stINCTRL.stPinMap.extn_en = OnOff;
	stINCTRL.ctrl |= (INCTRL_UPDATE_PINMAP | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
BOOL GetIn_PinMapExtn(void) {
	return stINCTRL.stPinMap.extn_en;
}

//------------------------------------------------------------------------------------
void SetIn_PinMapLvds(BOOL OnOff) {
	stINCTRL.stPinMap.lvds_en = OnOff;
	stINCTRL.ctrl |= (INCTRL_UPDATE_PINMAP | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
BOOL GetIn_PinMapLvds(void) {
	return stINCTRL.stPinMap.lvds_en;
}

//------------------------------------------------------------------------------------
void SetIn_PinMapMode(BYTE mode) {
	stINCTRL.stPinMap.mode_sel = mode;
	stINCTRL.ctrl |= (INCTRL_UPDATE_PINMAP | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
BYTE GetIn_PinMapMode(void) {
	return stINCTRL.stPinMap.mode_sel;
}

//------------------------------------------------------------------------------------
void SetIn_MainSwapCbCr(BOOL OnOff)
{
	stINCTRL.stSRC_m.stFINE.cbcr_swap=OnOff;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
void SetIn_MainYcOffset(BYTE val)
{
	stINCTRL.stSRC_m.stFINE.yc_offset=val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
void SetIn_MainTfPolar(BOOL OnOff)
{
	stINCTRL.stSRC_m.stFINE.tf_polar = OnOff;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
void SetIn_MainHsPolar(BOOL OnOff)
{
	if(OnOff)  stINCTRL.stSRC_m.stATTB.attb |= MISP_POSITIVE_HSYNC;
	else       stINCTRL.stSRC_m.stATTB.attb &=~MISP_POSITIVE_HSYNC;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
void SetIn_MainVsPolar(BOOL OnOff)
{
	if(OnOff)  stINCTRL.stSRC_m.stATTB.attb |= MISP_POSITIVE_VSYNC;
	else       stINCTRL.stSRC_m.stATTB.attb &=~MISP_POSITIVE_VSYNC;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
void SetIn_AuxSwapCbCr(BOOL OnOff)
{
	stINCTRL.stSRC_x.stFINE.cbcr_swap=OnOff;
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
void SetIn_AuxYcOffset(BYTE val)
{
	stINCTRL.stSRC_x.stFINE.yc_offset=val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
void SetIn_AuxTfPolar(BOOL OnOff)
{
	stINCTRL.stSRC_x.stFINE.tf_polar = OnOff;
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
void SetIn_AuxHsPolar(BOOL OnOff)
{
	if(OnOff)  stINCTRL.stSRC_x.stATTB.attb |= MISP_POSITIVE_HSYNC;
	else       stINCTRL.stSRC_x.stATTB.attb &=~MISP_POSITIVE_HSYNC;
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
void SetIn_AuxVsPolar(BOOL OnOff)
{
	if(OnOff)  stINCTRL.stSRC_x.stATTB.attb |= MISP_POSITIVE_VSYNC;
	else       stINCTRL.stSRC_x.stATTB.attb &=~MISP_POSITIVE_VSYNC;
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX | INCTRL_TRIGGER_ISP);
}

//---------------------------------------------------------------------------------------------
BOOL GetIn_MainSwapCbCr(void)
{
	return stINCTRL.stSRC_m.stFINE.cbcr_swap;
}

//------------------------------------------------------------------------------------
BYTE GetIn_MainYcOffset(void)
{
	return stINCTRL.stSRC_m.stFINE.yc_offset;
}

//------------------------------------------------------------------------------------
BOOL GetIn_MainTfPolar(void)
{
	return stINCTRL.stSRC_m.stFINE.tf_polar;
}

//------------------------------------------------------------------------------------
BOOL GetIn_MainHsPolar(void)
{
	return (stINCTRL.stSRC_m.stATTB.attb & MISP_POSITIVE_HSYNC)?1:0;
}

//------------------------------------------------------------------------------------
BOOL GetIn_MainVsPolar(void)
{
	return (stINCTRL.stSRC_m.stATTB.attb & MISP_POSITIVE_VSYNC)?1:0;
}

//------------------------------------------------------------------------------------
BOOL GetIn_AuxSwapCbCr(void)
{
	return stINCTRL.stSRC_x.stFINE.cbcr_swap;
}

//------------------------------------------------------------------------------------
BYTE GetIn_AuxYcOffset(void)
{
	return stINCTRL.stSRC_x.stFINE.yc_offset;
}

//------------------------------------------------------------------------------------
BOOL GetIn_AuxTfPolar(void)
{
	return stINCTRL.stSRC_x.stFINE.tf_polar;
}

//------------------------------------------------------------------------------------
BOOL GetIn_AuxHsPolar(void)
{
	return (stINCTRL.stSRC_x.stATTB.attb & MISP_POSITIVE_HSYNC)?1:0;
}

//------------------------------------------------------------------------------------
BOOL GetIn_AuxVsPolar(void)
{
	return (stINCTRL.stSRC_x.stATTB.attb & MISP_POSITIVE_VSYNC)?1:0;
}

//------------------------------------------------------------------------------------
void SetIn_MainFnrEn(BOOL OnOff)
{
	stINCTRL.stFNR_m.enab = OnOff;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN_NR | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
void SetIn_MainFnrCoef(BYTE nID)
{
	stINCTRL.stFNR_m.coef = nID;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN_NR | INCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------------
BOOL GetIn_MainFnrEn(void)
{
	return stINCTRL.stFNR_m.enab;
}

//------------------------------------------------------------------------------------
BYTE GetIn_MainFnrCoef(void)
{
	return stINCTRL.stFNR_m.coef;
}


#if defined(__ENABLE_INPUT_CSC__)
//------------------------------------------------------------------------------------
void SetIn_MainContrast(BYTE val)
{
	stINCTRL.stSRC_m.stTUNE.contrast = val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);	// main update
}

//------------------------------------------------------------------------------------
void SetIn_MainBrightness(BYTE val)
{
	stINCTRL.stSRC_m.stTUNE.brightness = val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);	// main update
}

//------------------------------------------------------------------------------------
void SetIn_MainSaturation(BYTE val)
{
	stINCTRL.stSRC_m.stTUNE.saturation = val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);	// main update
}

//------------------------------------------------------------------------------------
void SetIn_MainHue(BYTE val)
{
	stINCTRL.stSRC_m.stTUNE.hue = val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN | OUTCTRL_TRIGGER_ISP);	// main update
}

//------------------------------------------------------------------------------------
void SetIn_AuxContrast(BYTE val)
{
	stINCTRL.stSRC_x.stTUNE.contrast = val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);		// aux update
}

//------------------------------------------------------------------------------------
void SetIn_AuxBrightness(BYTE val)
{
	stINCTRL.stSRC_x.stTUNE.brightness = val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);		// aux update
}

//------------------------------------------------------------------------------------
void SetIn_AuxSaturation(BYTE val)
{
	stINCTRL.stSRC_x.stTUNE.saturation = val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);		// aux update
}

//------------------------------------------------------------------------------------
void SetIn_AuxHue(BYTE val)
{
	stINCTRL.stSRC_x.stTUNE.hue = val;
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX | OUTCTRL_TRIGGER_ISP);		// aux update
}

//------------------------------------------------------------------------------------
BYTE GetIn_MainContrast(void)
{
	return stINCTRL.stSRC_m.stTUNE.contrast;
}

//------------------------------------------------------------------------------------
BYTE GetIn_MainBrightness(void)
{
	return stINCTRL.stSRC_m.stTUNE.brightness;
}

//------------------------------------------------------------------------------------
BYTE GetIn_MainSaturation(void)
{
	return stINCTRL.stSRC_m.stTUNE.saturation;
}

//------------------------------------------------------------------------------------
BYTE GetIn_MainHue(void)
{
	return stINCTRL.stSRC_m.stTUNE.hue;
}

//------------------------------------------------------------------------------------
BYTE GetIn_AuxContrast(void)
{
	return stINCTRL.stSRC_x.stTUNE.contrast;
}

//------------------------------------------------------------------------------------
BYTE GetIn_AuxBrightness(void)
{
	return stINCTRL.stSRC_x.stTUNE.brightness;
}

//------------------------------------------------------------------------------------
BYTE GetIn_AuxSaturation(void)
{
	return stINCTRL.stSRC_x.stTUNE.saturation;
}

//------------------------------------------------------------------------------------
BYTE GetIn_AuxHue(void)
{
	return stINCTRL.stSRC_x.stTUNE.hue;
}
#endif  //#if defined(__ENABLE_INPUT_CSC__)


//-------------------------------------------------------------------------------------
void SetIN_MainCSCgray(BOOL nID)
{
	if(nID)		stINCTRL.ctrl |= INCTRL_MAIN_CSC_GRAY_EN;
	else		stINCTRL.ctrl &= (~INCTRL_MAIN_CSC_GRAY_EN);
	
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN_CSC | INCTRL_TRIGGER_ISP);	// main-csc update
}

//-------------------------------------------------------------------------------------
void SetIN_MainCSC(BYTE nID)
{
#if CSC_RANGE_CTRL
	switch(nID)	{
		case  0:	stINCTRL.stSRC_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_HD_FullRange;		break; 	// HD 0~255
		case  1:	stINCTRL.stSRC_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_HD_StdRange;		break; 	// HD 16~235
		case  2:	stINCTRL.stSRC_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_BT2020;			break; 	// UHD BT2020
		case  3:	stINCTRL.stSRC_m.pCSC = (PMISP_CSCCTRL_INFO)&MISP_CscRGBtoYUV_AppleRGB;			break; 	// Adobe rgb
		default:	return;
	}
	stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN_CSC | INCTRL_TRIGGER_ISP);	// main-csc update
#endif
}

//-------------------------------------------------------------------------------------
void SetIN_AuxCSCgray(BOOL nID)
{
	if(nID)		stINCTRL.ctrl |= INCTRL_AUX_CSC_GRAY_EN;
	else		stINCTRL.ctrl &= (~INCTRL_AUX_CSC_GRAY_EN);
	
	stINCTRL.ctrl |= (INCTRL_UPDATE_AUX_CSC | INCTRL_TRIGGER_ISP);	// aux-csc update
}


//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
void SetIN_SensorFrmt(WORD val)
{
	stINCTRL.stSRC_sensor.frmt = val;
}

//-------------------------------------------------------------------------------------
void SetIN_ExtFrmt(WORD val)
{
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	if(GetSS_SensorDoutFrmt())		stINCTRL.stSRC_extp0.frmt = val;		// lvds + ext_p0
	else							stINCTRL.stSRC_extp1.frmt = val;		// ttl + ext_p1
#else                           // MDIN-i51X (i510, i540, i550)
	stINCTRL.stSRC_extp1.frmt = val;		// lvds + ext_p1
#endif
}

//-------------------------------------------------------------------------------------
void SetIN_MainSrcExt(void)
{
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	if(GetSS_SensorDoutFrmt())		SetIN_MainSrcExtPort0();	// lvds+ext_p0
	else							SetIN_MainSrcExtPort1();	// ttl+ext_p1
#else                           // MDIN-i51X (i510, i540, i550)
	SetIN_MainSrcExtPort1();	// lvds + ext_p1
#endif
}

//-------------------------------------------------------------------------------------
void SetIN_AuxSrcExt(void)
{
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	if(GetSS_SensorDoutFrmt())		SetIN_AuxSrcExtPort0();		// lvds+ext_p0
	else							SetIN_AuxSrcExtPort1();		// ttl+ext_p1
#else                           // MDIN-i51X (i510, i540, i550)
	SetIN_AuxSrcExtPort1();		// lvds + ext_p1
#endif
}
//-------------------------------------------------------------------------------------
void SetIN_Ext2IPP(BOOL en)
{
#if defined(__USE_MDIN_i550__)&&defined(__USE_MDIN_i5XX_REV__)
	static WORD ext_mode ;
	WORD csc_set ;
	static MISP_SRCVIDEO_PATH_MAIN_t path ;
	if ( en == 1 )
	{
		ext_mode = stINCTRL.stSRC_extp1.mode ;
		path = GetIN_MainPath() ;
		
		if ( ext_mode == MISP_SRC_RGB444_8 || ext_mode == MISP_SRC_RGB444_10 ) csc_set = 0x8000 ; // Ext is RGB
		else csc_set = 0x2200 ; // Ext is YC
		
		SetIN_ExtPort1Mode(MISP_SRC_RGB444_8) ; // FORCE change EXT_P1 mode to RGB because IPP data is RGB
			
		if ( path == MISP_MAINPATH_EXT_P1 ){ 
			// if MainPATH is alreay EXT_P1, just ignite IPP path
			stINCTRL.MainPATH |= MISP_MAINPATH_SENSOR ;
			stINCTRL.ctrl |= (INCTRL_UPDATE_MAIN | INCTRL_TRIGGER_ISP);
		}
		else{
			// if MainPATH is other then EXT_P1, set MainPATH to EXT_P1 and ignite IPP path
			SetIN_MainSrcExtPort1();	
			stINCTRL.MainPATH |= MISP_MAINPATH_SENSOR ;
		}
		
		MISP_RegWrite(MISP_LOCAL_ID, 0x390, csc_set) ;
		MISP_RegField(MISP_LOCAL_ID, 0x391, 12, 4, 0x0008) ;
	}
	else{
		// Get back to previous path when Ext2IPP disabled
		SetIN_ExtPort1Mode(ext_mode) ; 
				
		if ( path == MISP_MAINPATH_EXT_P1 ) SetIN_MainSrcExtPort1();	
		else SetIN_MainSrcSensor();	
	}

#endif
}

//------------------------------------------------------------------------------------
WORD SetIN_ExtPort0ModeRoll(void)
{
	// FOR IO test
	WORD cur_mode ;
	
	cur_mode = stINCTRL.stSRC_extp0.mode ;
	
	if ( cur_mode == MISP_SRC_SEP656_10 ) cur_mode = MISP_SRC_RGB444_8 ;		
	else cur_mode++ ;

	SetIN_ExtPort0Mode(cur_mode) ;
	
	return cur_mode ;
}

//------------------------------------------------------------------------------------
WORD SetIN_ExtPort1ModeRoll(void)
{
	// FOR IO test
	WORD cur_mode ;
	
	cur_mode = stINCTRL.stSRC_extp1.mode ;
	
	if ( cur_mode == MISP_SRC_SEP656_10 ) cur_mode = MISP_SRC_RGB444_8 ;		
	else cur_mode++ ;

	SetIN_ExtPort1Mode(cur_mode) ;
	
	return cur_mode ;
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
void SetIN_MainNoFCMode(BOOL en)
{
#if (defined(__USE_MDIN_i510__)||defined(__USE_MDIN_i540__)||defined(__USE_MDIN_i550__))&&defined(__USE_MDIN_i5XX_REV__)
	if ( en == 1 )
	{
		SetFC_StopMainFC(2) ; // Stop Main FC & IPC
		
		SetCLK_SrcVCLK(5) ;
			// [¡®xx0¡¯ : VPLL out clock is used] : VPLL CLK
			// [¡®001¡¯ : CLK_B is used] : HDMI CLK
			// [¡®011¡¯ : CLK_A is used] : TTL CLK
			// [¡®101¡¯ : refer to stISPCLK.clk_lvds_sel value] // clk_lvds_int
			// [¡®111¡¯ : LVDS serial clk is used]		
		SetCLK_VCLKInv(0) ; // invert vclk ( May differ for some boards )
#if (defined(__USE_MDIN_i540__)||defined(__USE_MDIN_i550__))
		if( GetSS_SensorVoutFrmt() == MISP_VIDSRC_3840x2160p30 )
		{
			SetOUT_ExtLockHSdelay(107) ; //107(IMX226) 
			SetOUT_ExtLockVSdelay(7) ;	//7(IMX226)
		}
		else
		{
			SetOUT_ExtLockHSdelay(19) ; //19(IMX290)
			SetOUT_ExtLockVSdelay(3) ;	//3(IMX290)		
		}
#else
		{
			SetOUT_ExtLockHSdelay(19) ; //19(IMX290)
			SetOUT_ExtLockVSdelay(3) ;	//3(IMX290)		
		}
#endif
		
		SetOUT_ExtLockMode(2,5) ; // Main sync lock mode with sync reset delay enable
		SetOUT_ExtSyncMode(1) ; // Frame memory skip mode [ Direct sync from input top ]
		
#if defined(__USE_ISP_SCLK_OUT__)
		if ( GetSS_deserializer_slave_en() == 1 )
		{ // Sensor slave mode : ISP path is alredy using PLL 
			SetCLK_UserPLLSrc(1) ; // Just for Refresh PLL
				// [¡®00¡¯ : LVDS serial clk is used is used] 
				// [¡®01¡¯ : X_tal is used] 
				// [¡®10¡¯ : X_tal2 is used] 
				// [¡®11¡¯ : HOST_CLK is used]
		}
		else
		{ // Sensor master mode : ISP path might not using PLL 
			SetCLK_UserPLLSrc(0) ; // NoFCMode need PLL
				// [¡®00¡¯ : LVDS serial clk is used is used] 
				// [¡®01¡¯ : X_tal is used] 
				// [¡®10¡¯ : X_tal2 is used] 
				// [¡®11¡¯ : HOST_CLK is used]		
		}
#else
		SetCLK_UserPLLSrc(0) ; // NoFCMode need PLL : Refresh PLL
#endif
		// UARTprintf("SetIN_MainNoFCMode(1)\r\n");
	}
	else
	{

		SetOUT_ExtLockMode(0,0) ; // Main sync free run [normal mode]
		SetOUT_ExtSyncMode(0) ; // normal mode [ Main sync ]
			
		SetCLK_SrcVCLK(0) ;
			// [¡®xx0¡¯ : VPLL out clock is used] : VPLL CLK
			// [¡®001¡¯ : CLK_B is used] : HDMI CLK
			// [¡®011¡¯ : CLK_A is used] : TTL CLK
			// [¡®101¡¯ : refer to stISPCLK.clk_lvds_sel value] // clk_lvds_int
			// [¡®111¡¯ : LVDS serial clk is used]		
		SetCLK_VCLKInv(0) ; // normal vclk
			
		SetOUT_ExtLockHSdelay(100) ;
		SetOUT_ExtLockVSdelay(0) ;
		
		SetFC_StopMainFC(0) ; // RUN Main FC & IPC
#if defined(__USE_ISP_SCLK_OUT__)
		if ( GetSS_deserializer_slave_en() == 1 )
		{ // Sensor slave mode			
			SetCLK_UserPLLSrc(1) ; // Just for Refresh PLL
				// [¡®00¡¯ : LVDS serial clk is used is used] 
				// [¡®01¡¯ : X_tal is used] 
				// [¡®10¡¯ : X_tal2 is used] 
				// [¡®11¡¯ : HOST_CLK is used]
		}
		else
		{ // Sensor master mode
			SetCLK_UserPLLSrc(0) ; // Just for Refresh PLL
				// [¡®00¡¯ : LVDS serial clk is used is used] 
				// [¡®01¡¯ : X_tal is used] 
				// [¡®10¡¯ : X_tal2 is used] 
				// [¡®11¡¯ : HOST_CLK is used]		
		}		
#else
		SetCLK_UserPLLSrc(0) ; // Refresh PLL
#endif		
		// UARTprintf("SetIN_MainNoFCMode(0)\r\n");
	}
#endif
}

//------------------------------------------------------------------------------------
void SetIN_MainNoDelayMode(BYTE en)
{
#if (defined(__USE_MDIN_i510__)||defined(__USE_MDIN_i540__)||defined(__USE_MDIN_i550__))&&defined(__USE_MDIN_i5XX_REV__)
	if ( en == 1 )
	{
		// SetCLK_SrcVCLK(5) ;
			// [¡®xx0¡¯ : VPLL out clock is used] : VPLL CLK
			// [¡®001¡¯ : CLK_B is used] : HDMI CLK
			// [¡®011¡¯ : CLK_A is used] : TTL CLK
			// [¡®101¡¯ : refer to stISPCLK.clk_lvds_sel value] // clk_lvds_int
			// [¡®111¡¯ : LVDS serial clk is used]		
		// SetCLK_VCLKInv(0) ; // invert vclk ( May differ for some boards )
#if (defined(__USE_MDIN_i540__)||defined(__USE_MDIN_i550__))
		if( GetSS_SensorVoutFrmt() == MISP_VIDSRC_3840x2160p30 )
		{
			SetOUT_ExtLockHSdelay(113) ; 
			SetOUT_ExtLockVSdelay(18) ;	
		}
		else
		{
			SetOUT_ExtLockHSdelay(41) ; 
			SetOUT_ExtLockVSdelay(18) ;		
		}
#else
		{
			SetOUT_ExtLockHSdelay(41) ; 
			SetOUT_ExtLockVSdelay(18) ;	
		}
#endif

		SetIN_MainFRcntY(1) ;	SetIN_MainFRdlyY(0);
		SetIN_MainFRcntC(1) ;	SetIN_MainFRdlyC(0);		
		
		MISP_SW_Reset();
		
		SetOUT_ExtLockMode(2,5) ; // Main sync lock mode with sync reset delay enable
		SetOUT_ExtSyncMode(0) ; //  sync normal mode [ Main sync ]
		
// #if defined(__USE_ISP_SCLK_OUT__)
		// if ( GetSS_deserializer_slave_en() == 1 )
		// { // Sensor slave mode : ISP path is alredy using PLL 
			// SetCLK_UserPLLSrc(1) ; // Just for Refresh PLL
				// [¡®00¡¯ : LVDS serial clk is used is used] 
				// [¡®01¡¯ : X_tal is used] 
				// [¡®10¡¯ : X_tal2 is used] 
				// [¡®11¡¯ : HOST_CLK is used]
		// }
		// else
		// { // Sensor master mode : ISP path might not using PLL 
			// SetCLK_UserPLLSrc(0) ; // NoFCMode need PLL
				// [¡®00¡¯ : LVDS serial clk is used is used] 
				// [¡®01¡¯ : X_tal is used] 
				// [¡®10¡¯ : X_tal2 is used] 
				// [¡®11¡¯ : HOST_CLK is used]		
		// }
// #else
		// SetCLK_UserPLLSrc(0) ; // NoFCMode need PLL : Refresh PLL
// #endif

	}
	else if ( en == 2 )	
	{
		SetCLK_SrcVCLK(5) ;
			// [¡®xx0¡¯ : VPLL out clock is used] : VPLL CLK
			// [¡®001¡¯ : CLK_B is used] : HDMI CLK
			// [¡®011¡¯ : CLK_A is used] : TTL CLK
			// [¡®101¡¯ : refer to stISPCLK.clk_lvds_sel value] // clk_lvds_int
			// [¡®111¡¯ : LVDS serial clk is used]		
		SetCLK_VCLKInv(0) ; // invert vclk ( May differ for some boards )
#if (defined(__USE_MDIN_i540__)||defined(__USE_MDIN_i550__))
		if( GetSS_SensorVoutFrmt() == MISP_VIDSRC_3840x2160p30 )
		{
			SetOUT_ExtLockHSdelay(113) ; 
			SetOUT_ExtLockVSdelay(18) ;	
		}
		else
		{
			SetOUT_ExtLockHSdelay(41) ; 
			SetOUT_ExtLockVSdelay(18) ;		
		}
#else
		{
			SetOUT_ExtLockHSdelay(41) ; 
			SetOUT_ExtLockVSdelay(18) ;	
		}
#endif

		SetIN_MainFRcntY(1) ;	SetIN_MainFRdlyY(0);
		SetIN_MainFRcntC(1) ;	SetIN_MainFRdlyC(0);		
		
		MISP_SW_Reset();
		
		SetOUT_ExtLockMode(2,5) ; // Main sync lock mode with sync reset delay enable
		SetOUT_ExtSyncMode(0) ; //  sync normal mode [ Main sync ]
		
#if defined(__USE_ISP_SCLK_OUT__)
		if ( GetSS_deserializer_slave_en() == 1 )
		{ // Sensor slave mode : ISP path is alredy using PLL 
			SetCLK_UserPLLSrc(1) ; // Just for Refresh PLL
				// [¡®00¡¯ : LVDS serial clk is used is used] 
				// [¡®01¡¯ : X_tal is used] 
				// [¡®10¡¯ : X_tal2 is used] 
				// [¡®11¡¯ : HOST_CLK is used]
		}
		else
		{ // Sensor master mode : ISP path might not using PLL 
			SetCLK_UserPLLSrc(0) ; // NoFCMode need PLL
				// [¡®00¡¯ : LVDS serial clk is used is used] 
				// [¡®01¡¯ : X_tal is used] 
				// [¡®10¡¯ : X_tal2 is used] 
				// [¡®11¡¯ : HOST_CLK is used]		
		}
#else
		SetCLK_UserPLLSrc(0) ; // NoFCMode need PLL : Refresh PLL
#endif
	}
	else if ( en == 3 )	
	{
	//-----------------------------------------------
	// same setting [¡®111¡¯ : LVDS serial clk is used]	
	// if comment [¡®xx0¡¯ : VPLL out clock is used] : VPLL CLK
	// for vclk bypass..
//	MISP_RegField(MISP_HOST_ID, 0x020, 0, 1, 1);	// disable PLL	
//	MISP_RegField(MISP_HOST_ID,  0x02a, 0, 1, 1);
//	MISP_RegField(MISP_HOST_ID,  0x042, 4, 1, 1);
//	MISP_RegField(MISP_HOST_ID, 0x020, 0, 1, 0);	// disable PLL	
	//------------------------------------------------
	SetOUT_ExtLockHSdelay(41) ; 
	SetOUT_ExtLockVSdelay(18) ;	

#if defined(__OUT_VCLKPLL_LOCK__) && (defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__))
	MISP_RegField(MISP_LOCAL_ID, 0x049, 5, 1, 1);  // slave_ext_sync_sel (settings for sync locking)			
//	MISP_RegField(MISP_LOCAL_ID, 0x56D, 13, 3, 0x0005) ;
//	SetSS_SensorSlaveModeSel(5);

	// The system stable should be preceded before lvds tx output with vout_pll lock. 
	SetCLK_UserPLL_LockMode(2);	SetCLK_UserPLL_LockSrcPort(1);	

	if(GetCLK_UserPLL_LockMode()==1)		SetCLK_UserPLLSrcSelect(4);	//vclk pll int path			//statndard v-clock out such as 37.125, 74.25...
	else if(GetCLK_UserPLL_LockMode()==2){	
		SetCLK_UserPLLSrcSelect(6);	
				
		SetCLK_UserPLL(1,4,33,3,1) ;	
		MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x0421);	// pll source ---> pll --> div 1 --> user_clock out		

		if(GetCLK_UserPLL_LockSrcPort()==1) SetCLK_UserPLLSrc(3);
		else if(GetCLK_UserPLL_LockSrcPort()==2) SetCLK_UserPLLSrc(2);		
	}
#endif
	
//	MISP_SW_Reset();

	SetOUT_ExtLockMode(1,1) ; // Main sync lock mode with sync reset delay enable
	SetOUT_ExtSyncMode(0) ; //  sync normal mode [ Main sync ]	

	}	
	else
	{

		SetOUT_ExtLockMode(0,0) ; // Main sync free run [normal mode]
		SetOUT_ExtSyncMode(0) ; // normal mode [ Main sync ]
			
		SetCLK_SrcVCLK(0) ;
			// [¡®xx0¡¯ : VPLL out clock is used] : VPLL CLK
			// [¡®001¡¯ : CLK_B is used] : HDMI CLK
			// [¡®011¡¯ : CLK_A is used] : TTL CLK
			// [¡®101¡¯ : refer to stISPCLK.clk_lvds_sel value] // clk_lvds_int
			// [¡®111¡¯ : LVDS serial clk is used]		
		SetCLK_VCLKInv(0) ; // normal vclk

		SetIN_MainFRcntY(3) ;	SetIN_MainFRdlyY(0);
		SetIN_MainFRcntC(3) ;	SetIN_MainFRdlyC(0);					
		
		MISP_SW_Reset();
		
		SetOUT_ExtLockHSdelay(100) ;
		SetOUT_ExtLockVSdelay(0) ;

#if defined(__USE_ISP_SCLK_OUT__)
		if ( GetSS_deserializer_slave_en() == 1 )
		{ // Sensor slave mode			
			SetCLK_UserPLLSrc(1) ; // Just for Refresh PLL
				// [¡®00¡¯ : LVDS serial clk is used is used] 
				// [¡®01¡¯ : X_tal is used] 
				// [¡®10¡¯ : X_tal2 is used] 
				// [¡®11¡¯ : HOST_CLK is used]
		}
		else
		{ // Sensor master mode
			SetCLK_UserPLLSrc(0) ; // Just for Refresh PLL
				// [¡®00¡¯ : LVDS serial clk is used is used] 
				// [¡®01¡¯ : X_tal is used] 
				// [¡®10¡¯ : X_tal2 is used] 
				// [¡®11¡¯ : HOST_CLK is used]		
		}		
#else
		SetCLK_UserPLLSrc(0) ; // Refresh PLL
#endif		

	}
#endif
}

//------------------------------------------------------------------------------------

/*  FILE_END_HERE */
