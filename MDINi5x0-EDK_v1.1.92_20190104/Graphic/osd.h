//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  OSD.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__OSD_H__
#define		__OSD_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__MISP100_H__
#include	"misp100.h"
#endif
#ifndef		__MENU_H__
#include	"menu.h"
#endif
#ifndef		__ITEM_H__
#include	"item.h"
#endif

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#define		OSD_RGB_PALETTE		0
#define		OSD_YUV_PALETTE		1

#define		OSD_CSC_BYPASS		0
#define		OSD_CSC_YUV2RGB		1
#define		OSD_CSC_RGB2YUV		2

#if defined		(__USE_DDR_1G__)
#define 	DDR_MEM_SIZE	8192*2*8192		// col 10bit ?
#elif defined	(__USE_DDR_512M__)
#define 	DDR_MEM_SIZE	4096*2*8192		// col 9bit ?
#else
#define 	DDR_MEM_SIZE	2048*2*8192		// col 8bit
#endif

#if defined(__USE_CHN_FONT__)
#define 	OSD_FONT_W			24
#define		OSD_FONT_H			28
#define 	OSD_FONT_MAP_SIZE	((224*128) + (224*256) + (224*256))	// multi lan(eng + chn1/2)
#else
#define 	OSD_FONT_W			16
#define		OSD_FONT_H			20
#define 	OSD_FONT_MAP_SIZE	(80*128)
#endif

//#define		OSD_SPRITE0_SIZE	(OSD_FONT_W*28 * OSD_FONT_H*12 / 2)
#define		OSD_SPRITE0_SIZE	(OSD_FONT_W*28 * OSD_FONT_H*14 / 2)		// main menu
#define		OSD_SPRITE1_SIZE	(OSD_FONT_W*36 * OSD_FONT_H*2 / 2)		// sub menu
#define 	OSD_SPRITE2_SIZE	(OSD_FONT_W*80 * OSD_FONT_H*1 / 2)		// info, ch1,2
#define 	OSD_SPRITE3_SIZE	(OSD_FONT_W*80 * OSD_FONT_H*1 / 2)		// ch3,4
#define 	OSD_SPRITE4_SIZE	(OSD_FONT_W*10 * OSD_FONT_H*1 / 2)		// cam id
#define 	OSD_SPRITE5_SIZE	(OSD_FONT_W*20 * OSD_FONT_H*1 / 2)		// message box
#define 	OSD_SPRITE6_SIZE	(OSD_FONT_W*36 * OSD_FONT_H*19 / 2)		// status osd
#define 	OSD_SPRITE7_SIZE	0

#define 	OSD_CURSOR_SIZE		(32 * 32)

#if defined(__USE_USB_VOUT__)
#define 	OSD_MAIN_EN			0
#else
#define 	OSD_MAIN_EN			1
#endif

#define 	OSD_AUX_EN			1

#if defined(__MDIN_i5XX_FPGA__)
#define 	OSD_REPEAT_EN		0
#else
#define 	OSD_REPEAT_EN		1
#endif

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------
extern GACMAP_FONT_INFO stFONT;
extern SPRITE_CTL_INFO stOSD[12];
extern LAYER_CTL_INFO stLayer[3];
extern CURSOR_CTL_INFO stCurCTL;
extern SBOX_CTL_INFO stSBOX[12];

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void CreateOSDInstance(void);
void OSD_ModifyPalette_M(BOOL rgb);
void OSD_ModifyPalette_X(BOOL rgb);
void OSD_SetMainCSC(BYTE nID);
void OSD_SetFontGAC(SPRITE_INDEX_t index);
void OSD_SetFontLang(BYTE lang);
void OSD_SetFontMAP(void);	// for framebuffer map bug
void OSD_SetDemo(void);
void OSD_ViewFontTable(void);
void OSD_DrawVSboard(void);
void OSD_SetTitleDirect(BOOL OnOff, PBYTE pSTR);
void OSD_SetTitleBMP(BOOL OnOff, BYTE font_index);

#endif	/* __OSD_H__ */
