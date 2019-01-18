//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2005  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	SIL9136.c
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include "common.h"
#include "sil9136.h"

#include "defs.h"
#include "TypeDefs.h"
#include "AV_Config.h"
#include "Constants.h"
#include "TPI.h"
#include "TPI_Access.h"
#include "TPI_Regs.h"
#include "VideoModeTable.h"
#include "Externals.h"



#if defined(__USE_SIL9136_TX__)

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
#if defined(__MDIN_i550_USB_V100__)
#define		GetOUT_IspFrmt()	GetOUT_AuxFrmt()
#else
#define		GetOUT_IspFrmt()	GetOUT_MainFrmt()
#endif

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
BYTE Sil9136ChipSel=0;
BYTE Sil9136CSC=0;
#if defined(MDIN_AVI_INFO_CTRL)
//AVI_Infoframe_t stInfoframe={97, 0};  // 3840x2160p60, RGB
AVI_Infoframe_t stInfoframe={95, 0};  // 3840x2160p30, RGB
#endif

static BOOL bHdmiTxInit=FALSE;
static BOOL bUpdateInfo=FALSE;
static BOOL bHdmiTxPollStop=FALSE;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
void SIL9136_Init(void)
{
	bHdmiTxInit=TRUE;

//	for(Sil9136ChipSel=0; Sil9136ChipSel<SIL9136_CHIP_NUM; Sil9136ChipSel++)
		TPI_Init();
		SIL9136_SetFrmt();
}

// ----------------------------------------------------------------------
void SIL9136ProcHanlder(void)
{
	if(!bHdmiTxInit) return;

//	for(Sil9136ChipSel=0; Sil9136ChipSel<SIL9136_CHIP_NUM; Sil9136ChipSel++) {
		if(!bHdmiTxPollStop)	{TPI_Poll();	bHdmiTxPollStop=TRUE;}
		if(bUpdateInfo) SetAVI_InfoFrames();
//	}
	if(bUpdateInfo) bUpdateInfo=FALSE;
}

// ----------------------------------------------------------------------
#if defined(MDIN_AVI_INFO_CTRL)
void SIL9136_SetAviInfoVIC(BYTE VIC) {
	stInfoframe.VIC=VIC;
	bUpdateInfo=TRUE;
}

// ----------------------------------------------------------------------
void SIL9136_SetAviInfoColor(BYTE Y) {
	stInfoframe.Y=Y;
	bUpdateInfo=TRUE;
}
#endif

// ----------------------------------------------------------------------
void SIL9136_SetPollingStop(void)
{
	bHdmiTxPollStop = TRUE;
	TPI_DEBUG_PRINT (("TX Polling Stop..\n"));
}

// ----------------------------------------------------------------------
void ISPM SIL9136_SetPollingStart(void)
{
	bHdmiTxPollStop = FALSE;
//	TPI_DEBUG_PRINT (("TX Polling Enable..\n"));
}

// ----------------------------------------------------------------------
void SIL9136_SetPowerDown(void)
{
	DisableTMDS();
}

// ----------------------------------------------------------------------
void SIL9136_SetCSC(WORD val)
{
	BYTE csc, out_mode, Y;
	
	switch(val)	{
		case 0:	out_mode = BITS_OUT_RGB;		csc = BIT_BT_709;	Y=0;	break;	// RGB444(bt709)
		case 1:	out_mode = BITS_OUT_RGB;		csc = BIT_BT_601;	Y=0;	break;	// RGB444(bt601)
		case 2:	out_mode = BITS_OUT_YCBCR422;	csc = BIT_BT_709;	Y=1;	break;	// YC422(for M400_quad)
		default:								return;				break;
	}
	
	ReadModifyWriteTPI(TPI_OUTPUT_FORMAT_REG, BITS_1_0, out_mode);
	ReadModifyWriteTPI(TPI_OUTPUT_FORMAT_REG, BIT_4, csc);
	
	SIL9136_SetAviInfoColor(Y);
	
	Sil9136CSC = val;
	
	bUpdateInfo=TRUE;
}

// ----------------------------------------------------------------------
BYTE SIL9136_GetCSC(void)
{
	return	Sil9136CSC;
}

//--------------------------------------------------------------------------------------
void SIL9136_SetFrmt(void)
{
	// set AVI InfoFrame
	BYTE VIC;
//	BYTE Y;

	// video format identification code
	switch(GetOUT_IspFrmt()) {
		case MISP_VIDOUT_720x480i60:    VIC=  7;  break;  // 6 for 4:3
		case MISP_VIDOUT_720x576i50:    VIC= 22;  break;  // 21 for 4:3
		case MISP_VIDOUT_960x480i60:    VIC=  7;  break;  // unknown, substitute with 720x480i60
		case MISP_VIDOUT_960x576i50:    VIC= 22;  break;  // unknown, substitute with 720x576i50
		case MISP_VIDOUT_720x480p60:    VIC=  3;  break;  // 2 for 4:3
		case MISP_VIDOUT_720x576p50:    VIC= 18;  break;  // 17 for 4:3
		case MISP_VIDOUT_1280x720p60:   VIC=  4;  break;
		case MISP_VIDOUT_1280x720p50:   VIC= 19;  break;
		case MISP_VIDOUT_1280x720p30:   VIC= 62;  break;
		case MISP_VIDOUT_1280x720p25:   VIC= 61;  break;
		case MISP_VIDOUT_1280x720p24:   VIC= 60;  break;
		case MISP_VIDOUT_1920x1080i60:  VIC=  5;  break;
		case MISP_VIDOUT_1920x1080i50:  VIC= 20;  break;
		case MISP_VIDOUT_1920x1080p60:  VIC= 16;  break;
		case MISP_VIDOUT_1920x1080p50:  VIC= 31;  break;
		case MISP_VIDOUT_1920x1080p30:  VIC= 34;  break;
		case MISP_VIDOUT_1920x1080p25:  VIC= 33;  break;
		case MISP_VIDOUT_1920x1080p24:  VIC= 32;  break;
		case MISP_VIDOUT_1280x720p120:  VIC= 47;  break;
		case MISP_VIDOUT_1280x720p100:  VIC= 41;  break;
#if defined(__USE_59Hz_OUTPUT__)
		case MISP_VIDOUT_1280x720p59:   VIC=  4;  break;
		case MISP_VIDOUT_1920x1080i59:  VIC=  5;  break;
		case MISP_VIDOUT_1920x1080p59:  VIC= 16;  break;
		case MISP_VIDOUT_1920x1080p29:  VIC= 34;  break;
#endif
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)
		case MISP_VIDOUT_3840x2160p30:  VIC= 95;  break;
		case MISP_VIDOUT_3840x2160p25:  VIC= 94;  break;
		case MISP_VIDOUT_3840x2160p24:  VIC= 93;  break;
		case MISP_VIDOUT_4096x2160p30:  VIC=100;  break;
		case MISP_VIDOUT_4096x2160p25:  VIC= 99;  break;
		case MISP_VIDOUT_4096x2160p24:  VIC= 98;  break;
		case MISP_VIDOUT_1920x1080p120: VIC= 63;  break;
		case MISP_VIDOUT_1920x1080p100: VIC= 64;  break;
#if defined(__USE_59Hz_OUTPUT__)
		case MISP_VIDOUT_3840x2160p29:  VIC= 95;  break;
		case MISP_VIDOUT_4096x2160p29:  VIC=100;  break;
#endif
#endif
		default:                        VIC= 16;  break;  // unknown, substitute with 1920x1080p60
	}

	// RGB or YCbCr
/*	switch(stVideo[0].stOUT_m.mode) {
		case MDIN_OUT_SEP422_8:  Y=1;  break;
		case MDIN_OUT_YUV444_8:  Y=2;  break;
		case MDIN_OUT_RGB444_8:  Y=0;  break;
		default:                 Y=0;  break;
	}*/

	SIL9136_SetAviInfoVIC(VIC);
//	SIL9136_SetAviInfoColor(Y);
	
	SIL9136_ConvVICtoHDVFrmt(VIC);
}

//--------------------------------------------------------------------------------------
void SIL9136_ConvVICtoHDVFrmt(BYTE vic)
{
	if(vic <= 64)	{		// 1 ~ 64
		VideoModeDescription[Sil9136ChipSel].HDMIVideoFormat = VMD_HDMIFORMAT_CEA_VIC;
		VideoModeDescription[Sil9136ChipSel].VIC = vic;
	}
	else if((vic >= 93) && (vic <= 100))	{	// 93 ~ 95, 98 ~ 100
		vic -= (vic >= 98)?  94:  92;		// 3840h(93,94,95 -> 1,2,3), 4096h(98,99,100 -> 4,5,6)
		VideoModeDescription[Sil9136ChipSel].HDMIVideoFormat = VMD_HDMIFORMAT_HDMI_VIC;
		VideoModeDescription[Sil9136ChipSel].VIC = vic;
	}
	else	{
		VideoModeDescription[Sil9136ChipSel].HDMIVideoFormat = VMD_HDMIFORMAT_3D;
		VideoModeDescription[Sil9136ChipSel].VIC = vic;
	}
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#endif	//__USE_SIL9136_TX__
