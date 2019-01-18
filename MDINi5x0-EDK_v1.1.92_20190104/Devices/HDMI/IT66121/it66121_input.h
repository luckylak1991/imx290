///*****************************************
//  Copyright (C) 2009-2017
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <hdmitx_input.h>
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2017/12/20
//   @fileversion: ITE_HDMITX_SAMPLE_3.25
//******************************************/


#if defined(__USE_IT66121_TX__)


#ifndef _HDMITX_DEBUG_H_
#define _HDMITX_DEBUG_H_


#ifdef HDMITX_INPUT_INFO
LONG CalcPCLK();
LONG CalcAudFS();
LONG CalcRCLK();
BYTE hdmitx_getAudioCount() ;

USHORT hdmitx_getInputHTotal();
USHORT hdmitx_getInputVTotal();
BOOL hdmitx_isInputInterlace();
#endif

#endif


#endif	// __USE_IT66121_TX__

