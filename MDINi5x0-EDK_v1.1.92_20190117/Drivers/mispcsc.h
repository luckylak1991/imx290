//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name			:  MISPCSC.H
// Description			:  This file contains typedefine for the driver files	
// Ref. Docment			: 
// Revision History 	:

#ifndef		__MISPCSC_H__
#define		__MISPCSC_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
#define CSC_RANGE_CTRL  1  // 0:CSC depends on resolution, 1:CSC depends on range

typedef	struct
{
	WORD	coef[9];		// CSC coefficient 0~8
	WORD	in[3];			// added to G/Y,B/Cb,R/Cr channel before CSC
	WORD	out[3];			// added to G/Y,B/Cb,R/Cr channel after CSC

}	stPACKED MISP_CSCCTRL_INFO, *PMISP_CSCCTRL_INFO;

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------
extern ROMDATA MISP_CSCCTRL_INFO MISP_CscRGBtoYUV_SD_StdRange;
extern ROMDATA MISP_CSCCTRL_INFO MISP_CscRGBtoYUV_HD_StdRange;
extern ROMDATA MISP_CSCCTRL_INFO MISP_CscRGBtoYUV_SD_FullRange;
extern ROMDATA MISP_CSCCTRL_INFO MISP_CscRGBtoYUV_HD_FullRange;
extern ROMDATA MISP_CSCCTRL_INFO MISP_CscRGBtoYUV_TestRange;
extern ROMDATA MISP_CSCCTRL_INFO MISP_CscRGBtoYUV_AdobeRGB98;
extern ROMDATA MISP_CSCCTRL_INFO MISP_CscRGBtoYUV_BT2020;
extern ROMDATA MISP_CSCCTRL_INFO MISP_CscRGBtoYUV_AppleRGB;
extern ROMDATA MISP_CSCCTRL_INFO MISP_IN_CscBypass_StdRange;

extern ROMDATA MISP_CSCCTRL_INFO MISP_CscBypass_FullRange;
extern ROMDATA MISP_CSCCTRL_INFO MISP_CscBypass_StdRange;
extern ROMDATA MISP_CSCCTRL_INFO MISP_CscBypass_SonyRange;
extern ROMDATA MISP_CSCCTRL_INFO MISP_CscYUVtoRGB_SD_StdRange;
extern ROMDATA MISP_CSCCTRL_INFO MISP_CscYUVtoRGB_HD_StdRange;
extern ROMDATA MISP_CSCCTRL_INFO MISP_CscYUVtoRGB_SD_FullRange;
extern ROMDATA MISP_CSCCTRL_INFO MISP_CscYUVtoRGB_HD_FullRange;

extern ROMDATA WORD MISP_CscCosHue[];
extern ROMDATA WORD MISP_CscSinHue[];

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

#endif	/* __MISPCSC_H__ */

