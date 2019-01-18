//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MISPGAC.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		:
// Revision History 	:

#ifndef		__MISPGAC_H__
#define		__MISPGAC_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

typedef	struct
{
	WORD	dummy;		// dummy for 32-bit alignment
	WORD	attb;		// color mode

	WORD	w;			// size of src bitmap in horizontal direction, in pixel
	WORD	h;			// size of src bitmap in vertical direction, in pixel
	DWORD	addr;		// the start address of src bitmap in MISP-sdram

	WORD	r;			// the row+bank of src bitmap, is used only internal operation
	WORD	c;			// the column of src bitmap, is used only internal operation

}	stPACKED GACCOPY_BMP_INFO, *PGACCOPY_BMP_INFO;

typedef	struct
{
	WORD	x;			// the x position of upper-left of rectangle
	WORD	y;			// the y position of upper-left of rectangle
	WORD	w;			// the horizontal size of rectangle
	WORD	h;			// the vertical size of rectangle

	WORD	attb;		// color mode
	WORD	mode;		// sprite mode, is used only internal operation

}	stPACKED GACFILL_BOX_INFO, *PGACFILL_BOX_INFO;

typedef	struct
{
	BYTE	mode;		// ch-mode or xy-mode
	BYTE	attb;		// color mode & grid or auto-inc

	BYTE	w;			// size of 1-font in horizontal direction, in pixel
	BYTE	h;			// size of 1-font in vertical direction, in pixel
	DWORD	addr;		// the start address of font bitmap in MISP-sdram

	WORD	r;			// the row+bank of font bitmap, is used only internal operation
	WORD	c;			// the column of font bitmap, is used only internal operation

}	stPACKED GACMAP_FONT_INFO, *PGACMAP_FONT_INFO;

// user define for mode of gac-draw
#define		MISP_GAC_DRAW_CHMODE		0x00
#define		MISP_GAC_DRAW_XYMODE		0x01

// user define for attribute of color precision of gac
#define		GACMAP_1BITPALT_COLOR		0x00
#define		GACMAP_2BITPALT_COLOR		0x10
#define		GACMAP_4BITPALT_COLOR		0x20
#define		GACMAP_8BITPALT_COLOR		0x30
#define		GACMAP_16BITFULL_COLOR		0x40
#define		GACMAP_24BITFULL_COLOR		0x50
#define		GACMAP_32BITFULL_COLOR		0x60
#define		GACMAP_COLORMODE_MASK		0xf0

// user define for attribute of grid of gac in character mode
#define		MISP_GAC_16Vx32H_GRID		0x00
#define		MISP_GAC_32Vx16H_GRID		0x01
#define		MISP_GAC_16Hx32V_GRID		0x02
#define		MISP_GAC_32Hx16V_GRID		0x03

// user define for attribute of increment of gac in xy mode
#define		MISP_GAC_NONE_AUTOINC		0x00
#define		MISP_GAC_HORI_AUTOINC		0x04
#define		MISP_GAC_VERT_AUTOINC		0x08
#define		MISP_GAC_BOTH_AUTOINC		0x0c

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

MISP_ERROR_t MISPGAC_SetFontMode(PGACMAP_FONT_INFO pGAC, PSPRITE_CTL_INFO pSPT);
MISP_ERROR_t MISPGAC_SetDrawCHMode(BYTE y, BYTE x, PBYTE pBuff, BYTE len, BYTE attb);
MISP_ERROR_t MISPGAC_SetDrawXYMode(PGACMAP_FONT_INFO pGAC, WORD y, WORD x, PBYTE pBuff, BYTE len, BYTE attb);

MISP_ERROR_t MISPGAC_SetCopyMode(PGACCOPY_BMP_INFO pGAC, PSPRITE_CTL_INFO pSPT);
MISP_ERROR_t MISPGAC_CopyRectangle(PMISP_AREA_BOX pSRC, PMISP_AREA_BOX pDST);

MISP_ERROR_t MISPGAC_SetFillMode(PGACFILL_BOX_INFO pGAC, PSPRITE_CTL_INFO pSPT);
MISP_ERROR_t MISPGAC_FillRectangle(PGACFILL_BOX_INFO pGAC, DWORD color);


#endif	/* __MISPGAC_H__ */
