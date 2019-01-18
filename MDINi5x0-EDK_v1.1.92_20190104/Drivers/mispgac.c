//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	MISPGAC.C
// Description 		:
// Ref. Docment		:
// Revision History 	:

//--------------------------------------------------------------------------------------------------------------------------
// Note for GAC-char/copy/fill operation
//--------------------------------------------------------------------------------------------------------------------------

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
static WORD gridGAC, autoGAC;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPGAC_SetFontMode(PGACMAP_FONT_INFO pGAC, PSPRITE_CTL_INFO pSPT)
{
	WORD mode, bpp, unit = MISP_GetSizeOfBank()*2;

	pGAC->r = ADDR2ROW(pGAC->addr, unit);
	pGAC->c = ADDR2COL(pGAC->addr, unit)/8;				// for 8Byte-unit

	pSPT->stBMP.r = ADDR2ROW(pSPT->stBMP.addr, unit);
	pSPT->stBMP.c = ADDR2COL(pSPT->stBMP.addr, unit)/8;	// for 8Byte-unit

//#if	defined(SYSTEM_USE_MISP325)||defined(SYSTEM_USE_MISP340)
//	pGAC->c |= ((pGAC->r&1)<<7);	pGAC->r /= 2;
//	pSPT->stBMP.c |= ((pSPT->stBMP.r&1)<<7);	pSPT->stBMP.r /= 2;
//#endif

	switch (pGAC->attb&GACMAP_COLORMODE_MASK) {
		case GACMAP_1BITPALT_COLOR:		bpp = 1; mode = (5<<4)|3; break;
		case GACMAP_2BITPALT_COLOR:		bpp = 2; mode = (4<<4)|3; break;	//by hungry 2013.08.02 2bit_font
		case GACMAP_4BITPALT_COLOR:		bpp = 4; mode = (3<<4)|3; break;
		case GACMAP_8BITPALT_COLOR:		bpp = 8; mode = (2<<4)|2; break;
		default:						return MISP_INVALID_PARAM;
	}

	// gac_pixel_precision - precision
	MISP_CHK_ERR(MISP_RegField(MISP_HOST_ID, 0x058, 0, 8, mode));

	// gac_src_start_rb
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x05a, pGAC->r));

	// gac_dst_start_rb
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x05c, pSPT->stBMP.r));

	// gac_start_col
	mode = MAKEWORD(pGAC->c, pSPT->stBMP.c);
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x05e, mode));

	// gac_char_size
	mode = MAKEWORD(pGAC->w, pGAC->h);
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x06a, mode));

	// gac_src_byte_pitch
	mode = bpp*((pGAC->w<=16)? 2 : 4);
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x06b, mode));

	// gac_dst_byte_pitch
	//bpp = (bpp==1)? 4 : bpp;	mode = (pSPT->stBMP.w)*bpp/8;	mode = (mode+7)/8*8;
	bpp = (bpp<4)? 4 : bpp;	mode = (pSPT->stBMP.w)*bpp/8;	mode = (mode+7)/8*8;	//by hungry 2013.08.02 2bit_font
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x067, mode));

	// gac_process_ctrl
	gridGAC = (pGAC->attb&MISP_GAC_32Hx16V_GRID);	// set ch-grid
	autoGAC = (pGAC->attb&MISP_GAC_BOTH_AUTOINC);	// set xy-auto

	mode = ((autoGAC|gridGAC)<<6)|((pGAC->mode&MISP_GAC_DRAW_XYMODE)? 0x32 : 0x22);

	if(pGAC->w > 16)	mode |= (1 << 14);	// set gac_read_data_swap '1'	//by hungry 2013.08.02 2bit_font

	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x064, mode));

	// gac_dst_start_pos
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x068, 0x0000));	// fix 0
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x069, 0x0000));	// fix 0

	// set sprite information
	pSPT->ctrl &= ~SPRITE_COLORMODE_MASK;
	if	(bpp==4) pSPT->ctrl |= SPRITE_4BITPALT_COLOR;
	else		 pSPT->ctrl |= SPRITE_8BITPALT_COLOR;

	return MISPOSD_SetSpriteConfig(pSPT, (SPRITE_INDEX_t)pSPT->index);
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPGAC_SetDrawCHMode(BYTE y, BYTE x, PBYTE pBuff, BYTE len, BYTE attb)
{
	WORD i, addr, pChar[32];

	if 		(gridGAC==MISP_GAC_16Vx32H_GRID) addr = ((WORD)y<<5)|x;
	else if (gridGAC==MISP_GAC_32Vx16H_GRID) addr = ((WORD)y<<4)|x;
	else if (gridGAC==MISP_GAC_16Hx32V_GRID) addr = ((WORD)x<<4)|y;
	else									 addr = ((WORD)x<<5)|y;

	memset(&pChar[0], 0, sizeof(pChar));
	for (i=0; i<len; i++) pChar[i] = MAKEWORD(attb, pBuff[i]);

	MISP_CHK_ERR(MISP_MultiWrite(MISP_HOST_ID, 0x200|addr, (PBYTE)pChar, len*2));

	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPGAC_SetDrawXYMode(PGACMAP_FONT_INFO pGAC, WORD y, WORD x, PBYTE pBuff, BYTE len, BYTE attb)
{
	WORD i, pChar[100];
	WORD posix, posiy;

	if (len == 0) return MISP_NO_ERROR;
	
	posix = x * pGAC->w;
	posiy = y * pGAC->h;

	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x068, posix));
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x069, posiy));

	memset(&pChar[0], 0, sizeof(pChar));
	for (i=0; i<len; i++) pChar[i] = MAKEWORD(attb, pBuff[i]);

#if defined(MISP_USE_BUS_HIF) || defined(MISP_USE_SPI_HIF)
	SetHOST_IRQGACFinishCLR(); 														// clear irq
	for(i=0;i<len;i++)	{
		MISP_RegWrite(MISP_HOST_ID, 0x65, pChar[i]);	GetHOST_IRQGACFinishWait();	// wait ga_gac_finish irq //bh
	}
#else
	MISP_CHK_ERR(MISP_MultiWrite(MISP_HOST_ID, 0x200, (PBYTE)pChar, len*2));	//osd32x48_i2c(data err), osd12x16_i2c(ok)
//	for(i=0;i<len;i++)	MISP_RegWrite(MISP_HOST_ID, 0x65, pChar[i]);							//osd32x48_spi(data err), osd12x16_spi(ok)
#endif

	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPGAC_SetCopyMode(PGACCOPY_BMP_INFO pGAC, PSPRITE_CTL_INFO pSPT)
{
	WORD mode, bpp, unit = MISP_GetSizeOfBank()*2;

	pGAC->r = ADDR2ROW(pGAC->addr, unit);
	pGAC->c = ADDR2COL(pGAC->addr, unit)/8;				// for 8Byte-unit

	pSPT->stBMP.r = ADDR2ROW(pSPT->stBMP.addr, unit);
	pSPT->stBMP.c = ADDR2COL(pSPT->stBMP.addr, unit)/8;	// for 8Byte-unit

//#if	defined(SYSTEM_USE_MISP325)||defined(SYSTEM_USE_MISP340)
//	pGAC->c |= ((pGAC->r&1)<<7);	pGAC->r /= 2;
//	pSPT->stBMP.c |= ((pSPT->stBMP.r&1)<<7);	pSPT->stBMP.r /= 2;
//#endif

	switch (pGAC->attb&GACMAP_COLORMODE_MASK) {
		case GACMAP_2BITPALT_COLOR:		bpp =  2; mode = (4<<4)|4; break;
		case GACMAP_4BITPALT_COLOR:		bpp =  4; mode = (3<<4)|3; break;
		case GACMAP_8BITPALT_COLOR:		bpp =  8; mode = (2<<4)|2; break;
		case GACMAP_16BITFULL_COLOR:	bpp = 16; mode = (1<<4)|1; break;
		case GACMAP_24BITFULL_COLOR:	bpp = 24; mode = (6<<4)|6; break;
		case GACMAP_32BITFULL_COLOR:	bpp = 32; mode = (0<<4)|0; break;
		default:						return MISP_INVALID_PARAM;
	}

	// gac_pixel_precision - precision
	MISP_CHK_ERR(MISP_RegField(MISP_HOST_ID, 0x058, 0, 8, mode));

	// gac_src_start_rb
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x05a, pGAC->r));

	// gac_dst_start_rb
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x05c, pSPT->stBMP.r));

	// gac_start_col
	mode = MAKEWORD(pGAC->c, pSPT->stBMP.c);
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x05e, mode));

	// gac_src_byte_pitch
	mode = (pGAC->w)*bpp/8;
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x06b, mode));

	// gac_dst_byte_pitch
	mode = (pSPT->stBMP.w)*bpp/8;
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x067, mode));

	// gac_process_ctrl
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x064, 0x0010));

	return MISPOSD_SetSpriteConfig(pSPT, (SPRITE_INDEX_t)pSPT->index);
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPGAC_CopyRectangle(PMISP_AREA_BOX pSRC, PMISP_AREA_BOX pDST)
{
	MISP_CHK_ERR(MISP_MultiWrite(MISP_HOST_ID, 0x06c, (PBYTE)pSRC, 8));
	MISP_CHK_ERR(MISP_MultiWrite(MISP_HOST_ID, 0x068, (PBYTE)pDST, 4));

	SetHOST_IRQGACFinishCLR(); 													// clear irq
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x064, 0x0011));	// gac_process_en
	GetHOST_IRQGACFinishWait();													// wait ga_gac_finish irq
	
	return MISP_NO_ERROR;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPGAC_SetFillMode(PGACFILL_BOX_INFO pGAC, PSPRITE_CTL_INFO pSPT)
{
	WORD mode, bpp, unit = MISP_GetSizeOfBank()*2;

	pSPT->stBMP.r = ADDR2ROW(pSPT->stBMP.addr, unit);
	pSPT->stBMP.c = ADDR2COL(pSPT->stBMP.addr, unit)/8;	// for 8Byte-unit

//#if	defined(SYSTEM_USE_MISP325)||defined(SYSTEM_USE_MISP340)
//	pSPT->stBMP.c |= ((pSPT->stBMP.r&1)<<7);	pSPT->stBMP.r /= 2;
//#endif

	switch (pGAC->attb&GACMAP_COLORMODE_MASK) {
		case GACMAP_2BITPALT_COLOR:		bpp =  2; mode = (4<<4)|4; break;
		case GACMAP_4BITPALT_COLOR:		bpp =  4; mode = (3<<4)|3; break;
		case GACMAP_8BITPALT_COLOR:		bpp =  8; mode = (2<<4)|2; break;
		case GACMAP_16BITFULL_COLOR:	bpp = 16; mode = (1<<4)|1; break;
		case GACMAP_32BITFULL_COLOR:	bpp = 32; mode = (0<<4)|0; break;
		default:						return MISP_INVALID_PARAM;
	}

	pGAC->mode = pSPT->ctrl&SPRITE_COLORMODE_MASK;	// replace attribute

	// gac_pixel_precision - precision
	MISP_CHK_ERR(MISP_RegField(MISP_HOST_ID, 0x058, 0, 8, mode));

	// gac_src_start_rb, gac_dst_start_rb
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x05a, pSPT->stBMP.r));
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x05c, pSPT->stBMP.r));

	// gac_start_col
	mode = MAKEWORD(pSPT->stBMP.c, pSPT->stBMP.c);
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x05e, mode));

	// gac_src_byte_pitch, gac_dst_byte_pitch
	mode = (pSPT->stBMP.w)*bpp/8;
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x06b, mode));
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x067, mode));

	// gac_process_ctrl
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x064, 0x0018));

	return MISPOSD_SetSpriteConfig(pSPT, (SPRITE_INDEX_t)pSPT->index);
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_ERROR_t MISPGAC_FillRectangle(PGACFILL_BOX_INFO pGAC, DWORD color)
{
	switch (pGAC->mode) {
		case SPRITE_aRGB0565_16BIT:
			color = aRGB0565(GetA(color),GetR(color),GetG(color),GetB(color));
			color = MAKEDWORD(LOWORD(color), LOWORD(color)); break;

		case SPRITE_aRGB4444_16BIT:
			color = aRGB4444(GetA(color),GetR(color),GetG(color),GetB(color));
			color = MAKEDWORD(LOWORD(color), LOWORD(color)); break;

		case SPRITE_aRGB1555_16BIT:
			color = aRGB1555(GetA(color),GetR(color),GetG(color),GetB(color));
			color = MAKEDWORD(LOWORD(color), LOWORD(color)); break;

		case SPRITE_aYUV0655_YC444:
			color = aYUV0655(GetA(color),GetR(color),GetG(color),GetB(color));
			color = MAKEDWORD(LOWORD(color), LOWORD(color)); break;

		case SPRITE_aRGB5888_32BIT:
			color = aRGB5888(GetA(color),GetR(color),GetG(color),GetB(color));
			color = MAKEDWORD(HIWORD(color), LOWORD(color)); break;

		case SPRITE_1BITPALT_COLOR:		color &= 0x01;
			color = ((color<<15)|(color<<14))+((color<<13)|(color<<12)
				  |  (color<<11)|(color<<10))+((color<< 9)|(color<< 8)
				  |  (color<< 7)|(color<< 6))+((color<< 5)|(color<< 4)
				  |  (color<< 3)|(color<< 2))+((color<< 1)|(color<< 0));
			color = MAKEDWORD(LOWORD(color), LOWORD(color)); break;

		case SPRITE_2BITPALT_COLOR:		color &= 0x03;
			color = ((color<<14)|(color<<12))+((color<<10)|(color<<8)
				  |  (color<< 6)|(color<< 4))+((color<< 2)|(color<<0));
			color = MAKEDWORD(LOWORD(color), LOWORD(color)); break;

		case SPRITE_4BITPALT_COLOR:		color &= 0x0f;
			color = ((color<<12)|(color<< 8))+((color<< 4)|(color<<0));
			color = MAKEDWORD(LOWORD(color), LOWORD(color)); break;

		case SPRITE_8BITPALT_COLOR:		color &= 0xff;
			color = ((color<<24)|(color<<16))+((color<< 8)|(color<<0));
			color = MAKEDWORD(HIWORD(color), LOWORD(color)); break;

		default:						return MISP_INVALID_PARAM;
	}

	// gac_src_window, gac_dst_window
	MISP_CHK_ERR(MISP_MultiWrite(MISP_HOST_ID, 0x06c, (PBYTE)pGAC, 8));
	MISP_CHK_ERR(MISP_MultiWrite(MISP_HOST_ID, 0x068, (PBYTE)pGAC, 4));

	// ga_bmp_data_msb, ga_bmp_data_lsb
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x04a, HIWORD(color)));
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x04b, LOWORD(color)));

	SetHOST_IRQGACFinishCLR(); 													// clear irq
	MISP_CHK_ERR(MISP_RegWrite(MISP_HOST_ID, 0x064, 0x0019));	// gac_process_en
	GetHOST_IRQGACFinishWait();													// wait ga_gac_finish irq
	
	return MISP_NO_ERROR;
}

/*  FILE_END_HERE */
