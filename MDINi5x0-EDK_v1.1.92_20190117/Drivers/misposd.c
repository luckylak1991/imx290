//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	MISPOSD.C
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

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetBGLayerColor(MISP_COLORRGB rgb)
{
	WORD r = GetR(rgb), g = GetG(rgb), b = GetB(rgb);

	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x09a, g));
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x09b, MAKEWORD(b,r)));
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetBGBoxColor(MISP_COLORRGB rgb)
{
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x302, 0, 8, GetR(rgb)));
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x303, LOWORD(rgb)));
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetBGBoxArea(BGBOX_INDEX_t nID, WORD x, WORD y, WORD w, WORD h)
{
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x366, x));	// x
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x367, nID*4+0x80));
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x366, y));	// y
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x367, nID*4+0x81));
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x366, w));	// w
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x367, nID*4+0x82));
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x366, h));	// h
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x367, nID*4+0x83));
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_EnableBGBox(BGBOX_INDEX_t nID, BOOL OnOff)
{
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x367, 0x00|0x20));
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x366, nID, 1, MBIT(OnOff,1)));
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x367, 0x80|0x20));
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_BGBoxDisplay(BOOL OnOff)
{
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x300, 12, 1, MBIT(OnOff,1)));
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPAUX_SpriteColorMode(PSPRITE_CTL_INFO pINFO, BYTE nID)
{
	WORD mode;

	switch (pINFO->ctrl&SPRITE_COLORMODE_MASK) {
		case SPRITE_1BITPALT_COLOR: mode = 0; break;
		case SPRITE_2BITPALT_COLOR: mode = 1; break;
		case SPRITE_4BITPALT_COLOR: mode = 2; break;
		default:					mode = 0; break;
	}
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x2a1+(nID*7), 4, 3, mode));
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPOSD_SpriteColorMode(PSPRITE_CTL_INFO pINFO, BYTE nID)
{
	WORD mode, attb, p = ((nID%4)/2)*8+((nID%4)%2)*3;

	if (nID>SPRITE_INDEX7) return MISPAUX_SpriteColorMode(pINFO, nID%SPRITE_INDEX8);

	switch (pINFO->ctrl&SPRITE_COLORMODE_MASK) {
		case SPRITE_1BITPALT_COLOR: mode = 0; attb = 0; break;
		case SPRITE_2BITPALT_COLOR: mode = 1; attb = 0; break;
		case SPRITE_4BITPALT_COLOR: mode = 2; attb = 0; break;
		case SPRITE_8BITPALT_COLOR: mode = 3; attb = 0; break;

		case SPRITE_aRGB0565_16BIT: mode = 4; attb = 0; break;
		case SPRITE_aRGB4444_16BIT: mode = 4; attb = 1; break;
		case SPRITE_aRGB1555_16BIT: mode = 4; attb = 2; break;
		case SPRITE_aYBYR088_YC422: mode = 4; attb = 4; break;
		case SPRITE_aYBYR277_YC422: mode = 4; attb = 5; break;
		case SPRITE_aYBYR187_YC422: mode = 4; attb = 6; break;
		case SPRITE_aYUV0655_YC444: mode = 4; attb = 7; break;

		case SPRITE_aRGB0888_24BIT: mode = 6; attb = 0; break;
		default:					mode = 5; attb = 0; break;
	}
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x304+(nID*7), 4, 3, mode));
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x355+(nID/4), MIN(p, 15), 3, attb));
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPAUX_SpriteDisplay(PSPRITE_CTL_INFO pINFO, BYTE nID)
{
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x2a0, MIN(0+nID, 15), 1, MBIT(pINFO->ctrl,SPRITE_DISPLAY_ON)));	// display on/off
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPOSD_SpriteDisplay(PSPRITE_CTL_INFO pINFO, BYTE nID)
{
	if (nID>SPRITE_INDEX7) return MISPAUX_SpriteDisplay(pINFO, nID%SPRITE_INDEX8);

	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x300, 3+nID, 1, MBIT(pINFO->ctrl,SPRITE_DISPLAY_ON)));	// display on/off
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPAUX_SpritePosition(PSPRITE_CTL_INFO pINFO, BYTE nID)
{
	WORD rVal, x, y;

	MISP_CHK_ERR(MISP_RegRead(MISP_LOCAL_ID, 0x145, &rVal));
	x = (rVal&0x0080)? pINFO->x*2 : pINFO->x;	// if YC-muxed then x = x*2

	MISP_CHK_ERR(MISP_RegRead(MISP_LOCAL_ID, 0x17f, &rVal));
	y = (rVal&0x8000)? pINFO->y/2 : pINFO->y;	// if interlace then y = y/2

	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2a2+nID*7, x));			// x position
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2a3+nID*7, y));			// y position
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPOSD_SpritePosition(PSPRITE_CTL_INFO pINFO, BYTE nID)
{
	if (nID>SPRITE_INDEX7) return MISPAUX_SpritePosition(pINFO, nID%SPRITE_INDEX8);

	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x305+nID*7, pINFO->x));		// x position
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x306+nID*7, pINFO->y));		// y position
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPAUX_SpriteAlpha(PSPRITE_CTL_INFO pINFO, BYTE nID)
{
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x2a1+nID*7, 7, 5, pINFO->alpha));	// alpha value
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPOSD_SpriteAlpha(PSPRITE_CTL_INFO pINFO, BYTE nID)
{
	if (nID>SPRITE_INDEX7) return MISPAUX_SpriteAlpha(pINFO, nID%SPRITE_INDEX8);

	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x304+nID*7, 7, 5, pINFO->alpha));	// alpha value
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPOSD_SpritePaletteAddr(PSPRITE_CTL_INFO pINFO, BYTE nID)
{
	WORD idx = (pINFO->ctrl&SPRITE_PALT_ADDR_MASK)>>4;

	if (nID>SPRITE_INDEX7) return MISP_NO_ERROR;

	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x304+nID*7, 0, 4, idx));				// palette addr
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPAUX_SpriteBMPInfo(PSPRITE_CTL_INFO pINFO, BYTE nID)
{
	WORD unit = MISP_GetSizeOfBank()*2;

	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2a4+nID*7, pINFO->stBMP.w));		// width
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2a5+nID*7, pINFO->stBMP.h));		// height

	pINFO->stBMP.r = ADDR2ROW(pINFO->stBMP.addr, unit);
	pINFO->stBMP.c = ADDR2COL(pINFO->stBMP.addr, unit)/8;			// for 8Byte-unit
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2a6+nID*7, pINFO->stBMP.r));		// row+bank
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x2a7+nID*7, 0, 8, pINFO->stBMP.c));	// column
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPOSD_SpriteBMPInfo(PSPRITE_CTL_INFO pINFO, BYTE nID)
{
	WORD unit = MISP_GetSizeOfBank()*2;

	if (nID>SPRITE_INDEX7) return MISPAUX_SpriteBMPInfo(pINFO, nID%SPRITE_INDEX8);

	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x307+nID*7, pINFO->stBMP.w));		// width
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x308+nID*7, pINFO->stBMP.h));		// height

	pINFO->stBMP.r = ADDR2ROW(pINFO->stBMP.addr, unit);
	pINFO->stBMP.c = ADDR2COL(pINFO->stBMP.addr, unit)/8;			// for 8Byte-unit
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x309+nID*7, pINFO->stBMP.r));		// row+bank
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x30a+nID*7, 0, 8, pINFO->stBMP.c));	// column
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetSpriteConfig(PSPRITE_CTL_INFO pINFO, SPRITE_INDEX_t nID)
{
	pINFO->index = nID;		// set sprite index

	MISP_CHK_ERR(MISPOSD_SpriteColorMode(pINFO, pINFO->index));		// set color mode
	MISP_CHK_ERR(MISPOSD_SpriteDisplay(pINFO, pINFO->index));			// display on/off
	MISP_CHK_ERR(MISPOSD_SpritePosition(pINFO, pINFO->index));			// x,y position
	MISP_CHK_ERR(MISPOSD_SpriteAlpha(pINFO, pINFO->index));			// alpha value
	MISP_CHK_ERR(MISPOSD_SpritePaletteAddr(pINFO, pINFO->index));		// palette addr
	MISP_CHK_ERR(MISPOSD_SpriteBMPInfo(pINFO, pINFO->index));			// set BMP info
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_EnableSprite(PSPRITE_CTL_INFO pINFO, BOOL OnOff)
{
	if (OnOff)	pINFO->ctrl |=  SPRITE_DISPLAY_ON;
	else		pINFO->ctrl &= ~SPRITE_DISPLAY_ON;
	return MISPOSD_SpriteDisplay(pINFO, pINFO->index);			// display on/off
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetSpritePosition(PSPRITE_CTL_INFO pINFO, WORD x, WORD y)
{
	pINFO->x = x; pINFO->y = y;
	return MISPOSD_SpritePosition(pINFO, pINFO->index);		// x,y position
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetSpriteAlpha(PSPRITE_CTL_INFO pINFO, BYTE alpha)
{
	pINFO->alpha = alpha;
	return MISPOSD_SpriteAlpha(pINFO, pINFO->index);		// alpha value
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetSpritePaletteAddr(PSPRITE_CTL_INFO pINFO, BYTE addr)
{
	pINFO->ctrl &= ~SPRITE_PALT_ADDR_MASK;
	pINFO->ctrl |= (addr&SPRITE_PALT_ADDR_MASK);
	return MISPOSD_SpritePaletteAddr(pINFO, pINFO->index);	// palette addr
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetSpriteBMPInfo(PSPRITE_CTL_INFO pINFO, WORD w, WORD h, DWORD addr)
{
	pINFO->stBMP.w		= w;
	pINFO->stBMP.h		= h;
	pINFO->stBMP.addr	= addr;
	return MISPOSD_SpriteBMPInfo(pINFO, pINFO->index);		// set BMP info
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPAUX_LayerRepeat(PLAYER_CTL_INFO pINFO, WORD nID)
{
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x2a0, 8, 1, MBIT(pINFO->ctrl,LAYER_REPEAT_H_2X)));	// H-repeat(2x) on/off
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x2a0, 9, 1, MBIT(pINFO->ctrl,LAYER_REPEAT_V_2X)));	// V-repeat(2x) on/off

	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x2a0, 12, 1, MBIT(pINFO->ctrl,LAYER_REPEAT_H_4X)));	// H-repeat(4x) on/off
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x2a0, 13, 1, MBIT(pINFO->ctrl,LAYER_REPEAT_V_4X)));	// V-repeat(4x) on/off

	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPOSD_LayerRepeat(PLAYER_CTL_INFO pINFO, WORD nID)
{
	if (nID>LAYER_INDEX1) return MISPAUX_LayerRepeat(pINFO, nID%LAYER_INDEX2);

	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x301, 0+nID*2, 1, MBIT(pINFO->ctrl,LAYER_REPEAT_H_2X)));	// H-repeat(2x) on/off
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x301, 1+nID*2, 1, MBIT(pINFO->ctrl,LAYER_REPEAT_V_2X)));	// V-repeat(2x) on/off

	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x301, 7+nID*2, 1, MBIT(pINFO->ctrl,LAYER_REPEAT_H_4X)));	// H-repeat(4x) on/off
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x301, 8+nID*2, 1, MBIT(pINFO->ctrl,LAYER_REPEAT_V_4X)));	// V-repeat(4x) on/off

	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPOSD_LayerCSCBypass(PLAYER_CTL_INFO pINFO, WORD nID)
{
	if (nID>LAYER_INDEX1) return MISP_NO_ERROR;

	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x355, (nID)? 14: 15, 1, MBIT(pINFO->ctrl,LAYER_CSCBYPASS_ON)));	// csc-bypass on/off
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPOSD_LayerCSCControl(PLAYER_CTL_INFO pINFO, WORD nID)
{
	if (nID>LAYER_INDEX1) return MISP_NO_ERROR;

	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x353+nID, MBIT(pINFO->ctrl,LAYER_CSC_YUV2RGB)? 0x9240: 0x1008));	// csc control
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPAUX_LayerRGBKey(PLAYER_CTL_INFO pINFO, WORD nID)
{
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x2d0, 1, 1, MBIT(pINFO->ctrl,LAYER_TPKEY_RGB_ON)));	// rgb-key on/off
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPOSD_LayerRGBKey(PLAYER_CTL_INFO pINFO, WORD nID)
{
	if (nID>LAYER_INDEX1) return MISPAUX_LayerRGBKey(pINFO, nID%LAYER_INDEX2);

	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x357, 8+nID*1, 1, MBIT(pINFO->ctrl,LAYER_TPKEY_RGB_ON)));	// rgb-key on/off
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPOSD_LayerYUVKey(PLAYER_CTL_INFO pINFO, WORD nID)
{
	if (nID>LAYER_INDEX1) return MISP_NO_ERROR;

	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x359, 8+nID*1, 1, MBIT(pINFO->ctrl,LAYER_TPKEY_YUV_ON)));	// yuv-key on/off
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPAUX_LayerPalette(PLAYER_CTL_INFO pINFO, WORD nID)
{
	if (MBIT(pINFO->ctrl,LAYER_AUXPAL_16BIT)==1&&pINFO->stPAL.size>64) return MISP_INVALID_PARAM;
	if (MBIT(pINFO->ctrl,LAYER_AUXPAL_16BIT)==0&&pINFO->stPAL.size>32) return MISP_INVALID_PARAM;

	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x2bd, 0, 1, MBIT(pINFO->ctrl,LAYER_AUXPAL_16BIT)));
	MISP_CHK_ERR(MISP_MultiWrite(MISP_LOCAL_ID, 0x2c0, pINFO->stPAL.pBuff, pINFO->stPAL.size));
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPOSD_LayerPalette(PLAYER_CTL_INFO pINFO, WORD nID)
{
	WORD i;

	if (pINFO->stPAL.pBuff==NULL||pINFO->stPAL.size==0) return MISP_INVALID_PARAM;
	if (nID>LAYER_INDEX1) return MISPAUX_LayerPalette(pINFO, nID%LAYER_INDEX2);

	for (i=0; i<pINFO->stPAL.size/4; i++) {
		MISP_CHK_ERR(MISP_MultiWrite(MISP_LOCAL_ID, 0x360+nID*3, pINFO->stPAL.pBuff+i*4, 4));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x362+nID*3, 0x0000+i+pINFO->stPAL.addr));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x362+nID*3, 0x0100+i+pINFO->stPAL.addr));
	}
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetLayerConfig(PLAYER_CTL_INFO pINFO, LAYER_INDEX_t nID)
{
	pINFO->index = nID;		// set layer index

	MISP_CHK_ERR(MISPOSD_LayerRepeat(pINFO, pINFO->index));		// H,V repeat on/off
	MISP_CHK_ERR(MISPOSD_LayerCSCBypass(pINFO, pINFO->index));		// csc-bypass on/off
	MISP_CHK_ERR(MISPOSD_LayerCSCControl(pINFO, pINFO->index));	// csc control
	MISP_CHK_ERR(MISPOSD_LayerRGBKey(pINFO, pINFO->index));		// rgb-key on/off
	MISP_CHK_ERR(MISPOSD_LayerYUVKey(pINFO, pINFO->index));		// yuv-key on/off
	MISP_CHK_ERR(MISPOSD_LayerPalette(pINFO, pINFO->index));		// set palette entry
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_EnableLayerRepeat(PLAYER_CTL_INFO pINFO, BYTE h_rpt, BYTE v_rpt)
{
	pINFO->ctrl &= ~LAYER_REPEAT_MASK;
	
	switch(h_rpt)	{
		case 0:	pINFO->ctrl |= LAYER_REPEAT_H_OFF;	break;
		case 1:	pINFO->ctrl |= LAYER_REPEAT_H_2X;	break;
		case 2:	pINFO->ctrl |= LAYER_REPEAT_H_4X;	break;
	}
	
	switch(v_rpt)	{
		case 0:	pINFO->ctrl |= LAYER_REPEAT_V_OFF;	break;
		case 1:	pINFO->ctrl |= LAYER_REPEAT_V_2X;	break;
		case 2:	pINFO->ctrl |= LAYER_REPEAT_V_4X;	break;
	}

	return MISPOSD_LayerRepeat(pINFO, pINFO->index);	// H,V repeat on/off
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_GetLayerRepeat(PLAYER_CTL_INFO pINFO, PBYTE ph_rpt, PBYTE pv_rpt)
{
	if(pINFO->ctrl & LAYER_REPEAT_H_2X)			*ph_rpt=1;
	else if(pINFO->ctrl & LAYER_REPEAT_H_4X)	*ph_rpt=2;
	else										*ph_rpt=0;
	
	if(pINFO->ctrl & LAYER_REPEAT_V_2X)			*pv_rpt=1;
	else if(pINFO->ctrl & LAYER_REPEAT_V_4X)	*pv_rpt=2;
	else										*pv_rpt=0;
	
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_EnableLayerCSCBypass(PLAYER_CTL_INFO pINFO, BOOL OnOff)
{
	if (OnOff)	pINFO->ctrl |=  LAYER_CSCBYPASS_ON;
	else		pINFO->ctrl &= ~LAYER_CSCBYPASS_ON;
	return MISPOSD_LayerCSCBypass(pINFO, pINFO->index);	// csc-bypass on/off
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetLayerCSCControl(PLAYER_CTL_INFO pINFO, BOOL yuv2rgb)
{
	if (yuv2rgb)	pINFO->ctrl |=  LAYER_CSC_YUV2RGB;
	else			pINFO->ctrl &= ~LAYER_CSC_YUV2RGB;
	return MISPOSD_LayerCSCControl(pINFO, pINFO->index);	// csc control
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_EnableLayerTPKeyRGB(PLAYER_CTL_INFO pINFO, BOOL OnOff)
{
	if (OnOff)	pINFO->ctrl |=  LAYER_TPKEY_RGB_ON;
	else		pINFO->ctrl &= ~LAYER_TPKEY_RGB_ON;
	return MISPOSD_LayerRGBKey(pINFO, pINFO->index);	// rgb-key on/off
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_EnableLayerTPKeyYUV(PLAYER_CTL_INFO pINFO, BOOL OnOff)
{
	if (OnOff)	pINFO->ctrl |=  LAYER_TPKEY_YUV_ON;
	else		pINFO->ctrl &= ~LAYER_TPKEY_YUV_ON;
	return MISPOSD_LayerYUVKey(pINFO, pINFO->index);	// yuv-key on/off
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetLayerPalette(PLAYER_CTL_INFO pINFO, WORD addr, WORD size, PBYTE pBuff)
{
	if (pBuff==NULL||size==0) return MISP_INVALID_PARAM;

	pINFO->stPAL.addr	= addr;
	pINFO->stPAL.size	= size;
	pINFO->stPAL.pBuff	= pBuff;
	return MISPOSD_LayerPalette(pINFO, pINFO->index);	// set palette entry
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetBitmapData(DWORD addr, DWORD size, PBYTE pBuff)
{
	if (pBuff==NULL||size==0) return MISP_INVALID_PARAM;

//	MISP_CHK_ERR(MISP_SetPriorityHIF(MISP_PRIORITY_HIGH));

	MISP_CHK_ERR(MISP_MultiWrite(MISP_SDRAM_ID, addr, pBuff, size));

//	MISP_CHK_ERR(MISP_SetPriorityHIF(MISP_PRIORITY_NORM));
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetGlobalColorKeyRGB(MISP_COLORRGB rgb)
{
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x357, 0, 8, GetR(rgb)));		// R component of RGB color-key
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x358, 8, 8, GetG(rgb)));		// G component of RGB color-key
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x358, 0, 8, GetB(rgb)));		// B component of RGB color-key
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetGlobalColorKeyYUV(MISP_COLORYBR ybr)
{
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x359, 0, 8, GetY(ybr)));		// Y  component of YUV color-key
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x35a, 8, 8, GetCb(ybr)));	// Cb component of YUV color-key
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x35a, 0, 8, GetCr(ybr)));	// Cr component of YUV color-key
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPAUX_SetGlobalColorKeyRGB(MISP_COLORRGB rgb)
{
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x2d1, 0, 8, GetR(rgb)));		// R component of RGB color-key
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x2d2, 8, 8, GetG(rgb)));		// G component of RGB color-key
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x2d2, 0, 8, GetB(rgb)));		// B component of RGB color-key
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPOSD_SBoxCtrl(PSBOX_CTL_INFO pINFO, WORD nID)
{
	WORD mode = (pINFO->alpha<<4)|(pINFO->h_thk<<2)|(pINFO->v_thk<<0);

	mode |= (pINFO->ctrl&SBOX_BORDER_ON)?	(1<<11) : 0;
	mode |= (pINFO->ctrl&SBOX_PLANE_ON)?	(1<<10) : 0;
	mode |= (pINFO->ctrl&SBOX_BLINK_ON)?	(1<< 9) : 0;

	if(nID < SBOX_INDEX8)	{	// main sbox osd
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x368, mode));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x369, 0x0080|(0+nID*8)));
	}
	else	{	// aux sbox osd
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2DE, mode));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2DF, 0x0080|(0+(nID-8)*8)));
	}
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPOSD_SBoxRect(PSBOX_CTL_INFO pINFO, WORD nID)
{
	if(nID < SBOX_INDEX8)	{	// main sbox osd
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x368, pINFO->stRECT.lx));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x369, 0x0080|(1+nID*8)));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x368, pINFO->stRECT.rx));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x369, 0x0080|(2+nID*8)));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x368, pINFO->stRECT.ly));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x369, 0x0080|(3+nID*8)));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x368, pINFO->stRECT.ry));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x369, 0x0080|(4+nID*8)));
	}
	else	{	// aux sbox osd
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2DE, pINFO->stRECT.lx));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2DF, 0x0080|(1+(nID-8)*8)));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2DE, pINFO->stRECT.rx));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2DF, 0x0080|(2+(nID-8)*8)));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2DE, pINFO->stRECT.ly));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2DF, 0x0080|(3+(nID-8)*8)));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2DE, pINFO->stRECT.ry));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2DF, 0x0080|(4+(nID-8)*8)));
	}
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
static MISP_ERROR_t MISPOSD_SBoxColor(PSBOX_CTL_INFO pINFO, WORD nID)
{
	if(nID < SBOX_INDEX8)	{	// main sbox osd
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x368, MAKEWORD(GetR(pINFO->b_color),GetG(pINFO->b_color))));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x369, 0x0080|(5+nID*8)));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x368, MAKEWORD(GetB(pINFO->b_color),GetR(pINFO->p_color))));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x369, 0x0080|(6+nID*8)));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x368, MAKEWORD(GetG(pINFO->p_color),GetB(pINFO->p_color))));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x369, 0x0080|(7+nID*8)));
	}
	else	{
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2DE, MAKEWORD(GetR(pINFO->b_color),GetG(pINFO->b_color))));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2DF, 0x0080|(5+(nID-8)*8)));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2DE, MAKEWORD(GetB(pINFO->b_color),GetR(pINFO->p_color))));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2DF, 0x0080|(6+(nID-8)*8)));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2DE, MAKEWORD(GetG(pINFO->p_color),GetB(pINFO->p_color))));
		MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x2DF, 0x0080|(7+(nID-8)*8)));
	}
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetSBoxConfig(PSBOX_CTL_INFO pINFO, SBOX_INDEX_t nID)
{
	pINFO->index = nID;		// set sprite index

	MISP_CHK_ERR(MISPOSD_SBoxCtrl(pINFO, pINFO->index));		// sbox control
	MISP_CHK_ERR(MISPOSD_SBoxRect(pINFO, pINFO->index));		// sbox rectangle
	MISP_CHK_ERR(MISPOSD_SBoxColor(pINFO, pINFO->index));		// sbox color
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_EnableSBoxBorder(PSBOX_CTL_INFO pINFO, BOOL OnOff)
{
	if (OnOff)	pINFO->ctrl |=  SBOX_BORDER_ON;
	else		pINFO->ctrl &= ~SBOX_BORDER_ON;
	return MISPOSD_SBoxCtrl(pINFO, pINFO->index);		// border on/off
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_EnableSBoxPlane(PSBOX_CTL_INFO pINFO, BOOL OnOff)
{
	if (OnOff)	pINFO->ctrl |=  SBOX_PLANE_ON;
	else		pINFO->ctrl &= ~SBOX_PLANE_ON;
	return MISPOSD_SBoxCtrl(pINFO, pINFO->index);		// plane on/off
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_EnableSBoxBlink(PSBOX_CTL_INFO pINFO, BOOL OnOff)
{
	if (OnOff)	pINFO->ctrl |=  SBOX_BLINK_ON;
	else		pINFO->ctrl &= ~SBOX_BLINK_ON;
	return MISPOSD_SBoxCtrl(pINFO, pINFO->index);		// blink on/off
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetSBoxAlpha(PSBOX_CTL_INFO pINFO, BYTE alpha)
{
	pINFO->alpha = alpha;
	return MISPOSD_SBoxCtrl(pINFO, pINFO->index);		// alpha value
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetSBoxBorderThickH(PSBOX_CTL_INFO pINFO, BYTE h_thk)
{
	pINFO->h_thk = h_thk;
	return MISPOSD_SBoxCtrl(pINFO, pINFO->index);		// H-thick
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetSBoxBorderThickV(PSBOX_CTL_INFO pINFO, BYTE v_thk)
{
	pINFO->v_thk = v_thk;
	return MISPOSD_SBoxCtrl(pINFO, pINFO->index);		// V-thick
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetSBoxArea(PSBOX_CTL_INFO pINFO, WORD lx, WORD ly, WORD rx, WORD ry)
{
	pINFO->stRECT.lx	= lx;
	pINFO->stRECT.ly	= ly;
	pINFO->stRECT.rx	= rx;
	pINFO->stRECT.ry	= ry;
	return MISPOSD_SBoxRect(pINFO, pINFO->index);		// rectangle
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetSBoxBorderColor(PSBOX_CTL_INFO pINFO, MISP_COLORRGB rgb)
{
	pINFO->b_color = rgb;
	return MISPOSD_SBoxColor(pINFO, pINFO->index);		// boundary color
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetSBoxPlaneColor(PSBOX_CTL_INFO pINFO, MISP_COLORRGB rgb)
{
	pINFO->p_color = rgb;
	return MISPOSD_SBoxColor(pINFO, pINFO->index);		// plane color
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPOSD_SetGlobalSBoxBlink(SBOX_BLK_INFO stBLK)
{
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x369, 0x0000|0));
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x368, 12, 4, LO4BIT(stBLK.ctrl)));
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x369, 0x0080|0));

	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x369, 0x0000|8));
	MISP_CHK_ERR(MISP_RegField(MISP_LOCAL_ID, 0x368, 12, 3, HI4BIT(stBLK.ctrl)));
	MISP_CHK_ERR(MISP_RegWrite(MISP_LOCAL_ID, 0x369, 0x0080|8));
	return MISP_NO_ERROR;
}

/*  FILE_END_HERE */
