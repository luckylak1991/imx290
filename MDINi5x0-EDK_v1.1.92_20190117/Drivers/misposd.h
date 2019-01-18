//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MISPOSD.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		:
// Revision History 	:

#ifndef		__MISPOSD_H__
#define		__MISPOSD_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#define RGB(r,g,b)		((MISP_COLORRGB)MAKEDWORD(r,MAKEWORD(g,b)))
#define GetR(rgb)		(LOBYTE(HIWORD(rgb)))
#define GetG(rgb)		(HIBYTE(LOWORD(rgb)))
#define GetB(rgb)		(LOBYTE(LOWORD(rgb)))

#define YBR(y,b,r)		((MISP_COLORYBR)MAKEDWORD(y,MAKEWORD(b,r)))
#define GetY(ybr)		(LOBYTE(HIWORD(ybr)))
#define GetCb(ybr)		(HIBYTE(LOWORD(ybr)))
#define GetCr(ybr)		(LOBYTE(LOWORD(ybr)))

#define aRGB(a,r,g,b)	((MISP_COLORRGB)MAKEDWORD(MAKEWORD(a,r),MAKEWORD(g,b)))
#define aYBR(a,y,b,r)	((MISP_COLORYBR)MAKEDWORD(MAKEWORD(a,y),MAKEWORD(b,r)))
#define GetA(color)		(HIBYTE(HIWORD(color)))

#define aRGB0565(a,r,g,b) (WORD)(((WORD)(r&0xf8)<<8)|((WORD)(g&0xfc)<<3)|((WORD)(b&0xf8)>>3))
#define aRGB4444(a,r,g,b) (WORD)(((WORD)(a)<<12)|((WORD)(r&0xf0)<<4)|((WORD)(g&0xf0)<<0)|((WORD)(b&0xf0)>>4))
#define aRGB1555(a,r,g,b) (WORD)(((WORD)(a)<<15)|((WORD)(r&0xf8)<<7)|((WORD)(g&0xf8)<<2)|((WORD)(b&0xf8)>>3))
#define aYUV0655(a,y,b,r) (WORD)(((WORD)(y&0xfc)<<8)|((WORD)(b&0xf8)<<2)|((WORD)(r&0xf8)>>3))
#define aRGB5888(a,r,g,b) (DWORD)(((DWORD)(a)<<27)|((DWORD)r<<16)|((DWORD)g<<8)|((DWORD)b>>0))

typedef struct {
	WORD	lx;			// the x position of upper-left of rectangle
	WORD	ly;			// the y position of upper-left of rectangle
	WORD	rx;			// the x position of lower-right of rectangle
	WORD	ry;			// the y position of lower-right of rectangle

}	stPACKED MISP_RECT_BOX, *PMISP_RECT_BOX;

typedef struct {
	WORD	x;			// the x position of upper-left of rectangle
	WORD	y;			// the y position of upper-left of rectangle
	WORD	w;			// the horizontal size of rectangle
	WORD	h;			// the vertical size of rectangle

}	stPACKED MISP_AREA_BOX, *PMISP_AREA_BOX;

typedef enum {
	BGBOX_INDEX0 = 0, BGBOX_INDEX1, BGBOX_INDEX2, BGBOX_INDEX3,
	BGBOX_INDEX4, BGBOX_INDEX5, BGBOX_INDEX6, BGBOX_INDEX7
	
}	BGBOX_INDEX_t;

typedef struct {
	DWORD	addr;		// the start address of bitmap data in MISP-sdram
	DWORD	size;		// the size of bitmap data, in bytes
	PBYTE	pBuff;		// the pointer to bitmap data

}	stPACKED MISP_BITMAP_INFO, *PMISP_BITMAP_INFO;

typedef struct {
	WORD	w;			// the width of sprite bitmap, in pixels
	WORD	h;			// the height of sprite bitmap, in pixels
	DWORD	addr;		// the start address of sprite bitmap in MISP-sdram

	WORD	r;			// the row+bank of sprite bitmap, is used only internal operation
	WORD	c;			// the column of sprite bitmap, is used only internal operation

}	stPACKED SPRITE_BMP_INFO, *PSPRITE_BMP_INFO;

typedef struct {
	WORD	ctrl;		// the display control of sprite bitmap
	WORD	x;			// the left corner of sprite display position
	WORD	y;			// the top corner of sprite display position
	BYTE	alpha;		// the alpha-blending of sprite layer
	BYTE	index;		// the number of index of sprite layer

	SPRITE_BMP_INFO stBMP;

}	stPACKED SPRITE_CTL_INFO, *PSPRITE_CTL_INFO;

typedef enum {
	SPRITE_INDEX0 = 0, SPRITE_INDEX1, SPRITE_INDEX2, SPRITE_INDEX3,
	SPRITE_INDEX4, SPRITE_INDEX5, SPRITE_INDEX6, SPRITE_INDEX7,
	SPRITE_INDEX8, SPRITE_INDEX9, SPRITE_INDEXA, SPRITE_INDEXB
	
}	SPRITE_INDEX_t;

#define		SPRITE_DISPLAY_ON			0x1000
#define		SPRITE_DISPLAY_OFF			0x0000

#define		SPRITE_1BITPALT_COLOR		0x0000
#define		SPRITE_2BITPALT_COLOR		0x0100
#define		SPRITE_4BITPALT_COLOR		0x0200
#define		SPRITE_8BITPALT_COLOR		0x0300
#define		SPRITE_aRGB0565_16BIT		0x0400
#define		SPRITE_aRGB4444_16BIT		0x0500
#define		SPRITE_aRGB1555_16BIT		0x0600
#define		SPRITE_aYBYR088_YC422		0x0700
#define		SPRITE_aYBYR277_YC422		0x0800
#define		SPRITE_aYBYR187_YC422		0x0900
#define		SPRITE_aYUV0655_YC444		0x0a00
#define		SPRITE_aRGB0888_24BIT		0x0b00
#define		SPRITE_aRGB5888_32BIT		0x0c00
#define		SPRITE_COLORMODE_MASK		0x0f00

#define		SPRITE_PALT_ADDR_MASK		0x00ff

#define		SPRITE_OPAQUE				31
#define		SPRITE_TRANSPARENT			0

typedef struct {
	WORD	addr;		// the start address of palette entry
	WORD	size;		// the size of palette data, in bytes
	PBYTE	pBuff;		// the pointer to palette entry

}	stPACKED LAYER_PAL_INFO, *PLAYER_PAL_INFO;

typedef struct {
	WORD	ctrl;		// the display control of OSD layer
	BYTE	index;		// the number of index of OSD layer
	BYTE	dummy;		// dummy for 32-bit alignment

	LAYER_PAL_INFO stPAL;

}	stPACKED LAYER_CTL_INFO, *PLAYER_CTL_INFO;

typedef enum {
	LAYER_INDEX0 = 0, LAYER_INDEX1, LAYER_INDEX2
	
}	LAYER_INDEX_t;

#define		LAYER_CSCBYPASS_ON			0x0002
#define		LAYER_CSCBYPASS_OFF			0x0000

#define		LAYER_CSC_YUV2RGB			0x0001
#define		LAYER_CSC_RGB2YUV			0x0000

#define		LAYER_REPEAT_MASK			0x0F00
#define		LAYER_REPEAT_H_OFF			0x0000
#define		LAYER_REPEAT_H_2X			0x0100
#define		LAYER_REPEAT_H_4X			0x0200
#define		LAYER_REPEAT_V_OFF			0x0000
#define		LAYER_REPEAT_V_2X			0x0400
#define		LAYER_REPEAT_V_4X			0x0800

#define		LAYER_TPKEY_RGB_ON			0x0010
#define		LAYER_TPKEY_RGB_OFF			0x0000

#define		LAYER_TPKEY_YUV_ON			0x0020
#define		LAYER_TPKEY_YUV_OFF			0x0000

#define		LAYER_AUXPAL_16BIT			0x0040
#define		LAYER_AUXPAL_32BIT			0x0000

typedef struct {
	WORD	ctrl;		// the global blink control of S-BOX
	
}	stPACKED SBOX_BLK_INFO, *PSBOX_BLK_INFO;

#define		SBOX_BORDER_BLINK_ON		0x0010
#define		SBOX_BORDER_BLINK_OFF		0x0000

#define		SBOX_PLANE_BLINK_ON			0x0020
#define		SBOX_PLANE_BLINK_OFF		0x0000

#define		SBOX_BLINK_INVERSE			0x0040
#define		SBOX_BLINK_BLACK			0x0000

#define		SBOX_BLINK_CYCLE_MASK		0x000f

typedef struct {
	WORD	ctrl;		// the display control of sbox layer
	BYTE	alpha;		// the alpha-blending of sbox layer
	BYTE	index;		// the number of index of sbox layer

	BYTE	h_thk;		// the h-thickness of border of sbox
	BYTE	v_thk;		// the v-thickness of border of sbox

	MISP_RECT_BOX	stRECT;		// the coordinates of sbox
	MISP_COLORRGB	b_color;	// the color of border of sbox
	MISP_COLORRGB	p_color;	// the color of plain of sbox
	
}	stPACKED SBOX_CTL_INFO, *PSBOX_CTL_INFO;

typedef enum {
	SBOX_INDEX0 = 0, SBOX_INDEX1, SBOX_INDEX2, SBOX_INDEX3,
	SBOX_INDEX4, SBOX_INDEX5, SBOX_INDEX6, SBOX_INDEX7,
	SBOX_INDEX8, SBOX_INDEX9, SBOX_INDEXA, SBOX_INDEXB
	
}	SBOX_INDEX_t;

#define		SBOX_BORDER_ON			0x0001
#define		SBOX_BORDER_OFF			0x0000

#define		SBOX_PLANE_ON			0x0002
#define		SBOX_PLANE_OFF			0x0000

#define		SBOX_BLINK_ON			0x0004
#define		SBOX_BLINK_OFF			0x0000

#define		SBOX_OPAQUE				31
#define		SBOX_TRANSPARENT		0

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

MISP_ERROR_t MISPOSD_SetBGLayerColor(MISP_COLORRGB rgb);

MISP_ERROR_t MISPOSD_SetBGBoxColor(MISP_COLORRGB rgb);
MISP_ERROR_t MISPOSD_SetBGBoxArea(BGBOX_INDEX_t nID, WORD x, WORD y, WORD w, WORD h);
MISP_ERROR_t MISPOSD_EnableBGBox(BGBOX_INDEX_t nID, BOOL OnOff);
MISP_ERROR_t MISPOSD_BGBoxDisplay(BOOL OnOff);

MISP_ERROR_t MISPOSD_SetSpriteConfig(PSPRITE_CTL_INFO pINFO, SPRITE_INDEX_t nID);
MISP_ERROR_t MISPOSD_EnableSprite(PSPRITE_CTL_INFO pINFO, BOOL OnOff);
MISP_ERROR_t MISPOSD_SetSpritePosition(PSPRITE_CTL_INFO pINFO, WORD x, WORD y);
MISP_ERROR_t MISPOSD_SetSpriteAlpha(PSPRITE_CTL_INFO pINFO, BYTE alpha);
MISP_ERROR_t MISPOSD_SetSpritePaletteAddr(PSPRITE_CTL_INFO pINFO, BYTE addr);
MISP_ERROR_t MISPOSD_SetSpriteBMPInfo(PSPRITE_CTL_INFO pINFO, WORD w, WORD h, DWORD addr);

MISP_ERROR_t MISPOSD_SetLayerConfig(PLAYER_CTL_INFO pINFO, LAYER_INDEX_t nID);
MISP_ERROR_t MISPOSD_EnableLayerRepeat(PLAYER_CTL_INFO pINFO, BYTE h_rpt, BYTE v_rpt);
MISP_ERROR_t MISPOSD_GetLayerRepeat(PLAYER_CTL_INFO pINFO, PBYTE ph_rpt, PBYTE pv_rpt);
MISP_ERROR_t MISPOSD_EnableLayerCSCBypass(PLAYER_CTL_INFO pINFO, BOOL OnOff);
MISP_ERROR_t MISPOSD_SetLayerCSCControl(PLAYER_CTL_INFO pINFO, BOOL yuv2rgb);
MISP_ERROR_t MISPOSD_EnableLayerTPKeyRGB(PLAYER_CTL_INFO pINFO, BOOL OnOff);
MISP_ERROR_t MISPOSD_EnableLayerTPKeyYUV(PLAYER_CTL_INFO pINFO, BOOL OnOff);
MISP_ERROR_t MISPOSD_SetLayerPalette(PLAYER_CTL_INFO pINFO, WORD addr, WORD size, PBYTE pBuff);

MISP_ERROR_t MISPOSD_SetBitmapData(DWORD addr, DWORD size, PBYTE pBuff);
MISP_ERROR_t MISPOSD_SetGlobalColorKeyRGB(MISP_COLORRGB rgb);
MISP_ERROR_t MISPOSD_SetGlobalColorKeyYUV(MISP_COLORYBR ybr);
MISP_ERROR_t MISPAUX_SetGlobalColorKeyRGB(MISP_COLORRGB rgb);

MISP_ERROR_t MISPOSD_SetSBoxConfig(PSBOX_CTL_INFO pINFO, SBOX_INDEX_t nID);
MISP_ERROR_t MISPOSD_EnableSBoxBorder(PSBOX_CTL_INFO pINFO, BOOL OnOff);
MISP_ERROR_t MISPOSD_EnableSBoxPlane(PSBOX_CTL_INFO pINFO, BOOL OnOff);
MISP_ERROR_t MISPOSD_EnableSBoxBlink(PSBOX_CTL_INFO pINFO, BOOL OnOff);
MISP_ERROR_t MISPOSD_SetSBoxAlpha(PSBOX_CTL_INFO pINFO, BYTE alpha);
MISP_ERROR_t MISPOSD_SetSBoxBorderThickH(PSBOX_CTL_INFO pINFO, BYTE h_thk);
MISP_ERROR_t MISPOSD_SetSBoxBorderThickV(PSBOX_CTL_INFO pINFO, BYTE v_thk);
MISP_ERROR_t MISPOSD_SetSBoxArea(PSBOX_CTL_INFO pINFO, WORD lx, WORD ly, WORD rx, WORD ry);
MISP_ERROR_t MISPOSD_SetSBoxBorderColor(PSBOX_CTL_INFO pINFO, MISP_COLORRGB rgb);
MISP_ERROR_t MISPOSD_SetSBoxPlaneColor(PSBOX_CTL_INFO pINFO, MISP_COLORRGB rgb);
MISP_ERROR_t MISPOSD_SetGlobalSBoxBlink(SBOX_BLK_INFO stBLK);


#endif	/* __MISPOSD_H__ */
