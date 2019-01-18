//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	OSD.C
// Description 		:
// Ref. Docment		: MDIN380 DDK v2.52
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"osd.h"
#include	"video.h"
#include	"mispaero.h"

//#define 	GOSD_TEST
#if defined(GOSD_TEST)
#include	"gosd_bmp.h"
#endif


#if defined(__USE_CHN_FONT__)
#include	"mispfont_24x28_outline_eng_chn.h"
#else
#include	"mispfont_12x16_outline.h"
#endif

//#if defined(__USE_VECTOR_SCOPE__)
#include	"VS.h"
//#endif

#if defined(__USE_OSD_TITLE__)
#include	"title/hdmi_384x88.h"
#include	"title/exsdi_384x88.h"
#include	"title/tvi_384x88.h"
#include	"title/camera_384x88.h"
#include	"title/blank_384x88.h"
#include	"title/o4k15p_576x88.h"
#include	"title/o4k30p_576x88.h"
#include	"title/o4k60p_576x88.h"
#include	"title/o4kuhd_576x88.h"
#include	"title/fhd60p_576x88.h"
#include	"title/fhd30p_576x88.h"
#include	"title/blank_576x88.h"
#endif

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
GACMAP_FONT_INFO stFONT;
SPRITE_CTL_INFO stOSD[12];
LAYER_CTL_INFO stLayer[3];
CURSOR_CTL_INFO stCurCTL;
SBOX_CTL_INFO stSBOX[12];

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
static void OSD_SetSprites(void)
{
	DWORD e_addr = (DWORD)DDR_MEM_SIZE -OSD_FONT_MAP_SIZE;

//sprite_0	-- main menu
	memset(&stOSD[SPRITE_INDEX0], 0, sizeof(SPRITE_CTL_INFO));
	stOSD[SPRITE_INDEX0].ctrl			= 0;
	stOSD[SPRITE_INDEX0].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEX0].x				= 512;
	stOSD[SPRITE_INDEX0].y				= 140;
	stOSD[SPRITE_INDEX0].stBMP.w		= OSD_FONT_W*28;			// 28-column
	stOSD[SPRITE_INDEX0].stBMP.h		= OSD_FONT_H*14;//OSD_FONT_H*12;			// 12-line
	stOSD[SPRITE_INDEX0].stBMP.addr		= e_addr-OSD_SPRITE0_SIZE;	
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX0], SPRITE_INDEX0);

//sprite_1	-- size,position
	memset(&stOSD[SPRITE_INDEX1], 0, sizeof(SPRITE_CTL_INFO));
	stOSD[SPRITE_INDEX1].ctrl			= 0;
	stOSD[SPRITE_INDEX1].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEX1].x				= 300;
	stOSD[SPRITE_INDEX1].y				= 140;
	stOSD[SPRITE_INDEX1].stBMP.w		= OSD_FONT_W*36;			// 36-column
	stOSD[SPRITE_INDEX1].stBMP.h		= OSD_FONT_H*2;				// 2-line
	stOSD[SPRITE_INDEX1].stBMP.addr		= stOSD[SPRITE_INDEX0].stBMP.addr - OSD_SPRITE1_SIZE;
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX1], SPRITE_INDEX1);

//sprite_2	-- info osd, cascade(CH1,2)
	memset(&stOSD[SPRITE_INDEX2], 0, sizeof(SPRITE_CTL_INFO));
	stOSD[SPRITE_INDEX2].ctrl			= 0;
	stOSD[SPRITE_INDEX2].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEX2].x				= 50;
	stOSD[SPRITE_INDEX2].y				= 20;
	//stOSD[SPRITE_INDEX2].stBMP.w		= OSD_FONT_W*40;			// 40-column
	stOSD[SPRITE_INDEX2].stBMP.w		= OSD_FONT_W*80;			// 80-column
	stOSD[SPRITE_INDEX2].stBMP.h		= OSD_FONT_H*1;				// 1-line
	stOSD[SPRITE_INDEX2].stBMP.addr		= stOSD[SPRITE_INDEX1].stBMP.addr - OSD_SPRITE2_SIZE;
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX2], SPRITE_INDEX2);

//sprite_3	-- cascade(CH3,4)
	memset(&stOSD[SPRITE_INDEX3], 0, sizeof(SPRITE_CTL_INFO));
	stOSD[SPRITE_INDEX3].ctrl			= 0;
	stOSD[SPRITE_INDEX3].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEX3].x				= 50;
	stOSD[SPRITE_INDEX3].y				= 20+540;
	//stOSD[SPRITE_INDEX3].stBMP.w		= OSD_FONT_W*40;			// 40-column
	stOSD[SPRITE_INDEX3].stBMP.w		= OSD_FONT_W*80;			// 80-column
	stOSD[SPRITE_INDEX3].stBMP.h		= OSD_FONT_H*1;				// 1-line
	stOSD[SPRITE_INDEX3].stBMP.addr		= stOSD[SPRITE_INDEX2].stBMP.addr - OSD_SPRITE3_SIZE;
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX3], SPRITE_INDEX3);

//sprite_4	-- CAM ID
	memset(&stOSD[SPRITE_INDEX4], 0, sizeof(SPRITE_CTL_INFO));
	stOSD[SPRITE_INDEX4].ctrl			= 0;
	stOSD[SPRITE_INDEX4].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEX4].x				= 1508;
	stOSD[SPRITE_INDEX4].y				= 80;
	stOSD[SPRITE_INDEX4].stBMP.w		= OSD_FONT_W*10;			// 10-column
	stOSD[SPRITE_INDEX4].stBMP.h		= OSD_FONT_H*1;				// 1-line
	stOSD[SPRITE_INDEX4].stBMP.addr		= stOSD[SPRITE_INDEX3].stBMP.addr - OSD_SPRITE4_SIZE;
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX4], SPRITE_INDEX4);
	
//sprite_5	-- message osd
	memset(&stOSD[SPRITE_INDEX5], 0, sizeof(SPRITE_CTL_INFO));
	stOSD[SPRITE_INDEX5].ctrl			= 0;
	stOSD[SPRITE_INDEX5].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEX5].x				= 670;
	stOSD[SPRITE_INDEX5].y				= 480;
	stOSD[SPRITE_INDEX5].stBMP.w		= OSD_FONT_W*20;			// 20-column
	stOSD[SPRITE_INDEX5].stBMP.h		= OSD_FONT_H*1;				// 1-line
	stOSD[SPRITE_INDEX5].stBMP.addr		= stOSD[SPRITE_INDEX4].stBMP.addr - OSD_SPRITE5_SIZE;
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX5], SPRITE_INDEX5);
	
//sprite_6	-- status osd
	memset(&stOSD[SPRITE_INDEX6], 0, sizeof(SPRITE_CTL_INFO));
	stOSD[SPRITE_INDEX6].ctrl			= 0;
	stOSD[SPRITE_INDEX6].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEX6].x				= 128;
	stOSD[SPRITE_INDEX6].y				= 80;
	stOSD[SPRITE_INDEX6].stBMP.w		= OSD_FONT_W*36;			// 36-column(1152)
	stOSD[SPRITE_INDEX6].stBMP.h		= OSD_FONT_H*19;			// 19-line(720)
	stOSD[SPRITE_INDEX6].stBMP.addr		= stOSD[SPRITE_INDEX5].stBMP.addr - OSD_SPRITE6_SIZE;
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX6], SPRITE_INDEX6);
	
#if defined(GOSD_TEST)
//sprite_7  -- graphic osd test
	memset(&stOSD[SPRITE_INDEX7], 0, sizeof(SPRITE_CTL_INFO));
	stOSD[SPRITE_INDEX7].ctrl			= SPRITE_aRGB0565_16BIT;//SPRITE_aRGB5888_32BIT;
	stOSD[SPRITE_INDEX7].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEX7].x				= 200;//1000;
	stOSD[SPRITE_INDEX7].y				= 200;
	stOSD[SPRITE_INDEX7].stBMP.w		= ICON_W;
	stOSD[SPRITE_INDEX7].stBMP.h		= ICON_H;
	stOSD[SPRITE_INDEX7].stBMP.addr		= stOSD[SPRITE_INDEX6].stBMP.addr - sizeof(SPIROM_BMP);
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX7], SPRITE_INDEX7);
#elif defined(__USE_OSD_TITLE__)
//sprite_7  -- osd title
	memset(&stOSD[SPRITE_INDEX7], 0, sizeof(SPRITE_CTL_INFO));
	stOSD[SPRITE_INDEX7].ctrl			= SPRITE_8BITPALT_COLOR | 0x10;
	stOSD[SPRITE_INDEX7].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEX7].x				= 0;
	stOSD[SPRITE_INDEX7].y				= 0;
	stOSD[SPRITE_INDEX7].stBMP.w		= 64*15;
	stOSD[SPRITE_INDEX7].stBMP.h		= 88*2;
	stOSD[SPRITE_INDEX7].stBMP.addr		= stOSD[SPRITE_INDEX6].stBMP.addr - (64*15*88*2);
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX7], SPRITE_INDEX7);
#else
//sprite_7  -- vector scope(red/blue data)
	memset(&stOSD[SPRITE_INDEX7], 0, sizeof(SPRITE_CTL_INFO));
	stOSD[SPRITE_INDEX7].ctrl			= SPRITE_8BITPALT_COLOR | 0x10;
	stOSD[SPRITE_INDEX7].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEX7].x				= 0;
	stOSD[SPRITE_INDEX7].y				= 0;
	stOSD[SPRITE_INDEX7].stBMP.w		= 256;
	stOSD[SPRITE_INDEX7].stBMP.h		= 256;
	stOSD[SPRITE_INDEX7].stBMP.addr		= stOSD[SPRITE_INDEX6].stBMP.addr - (256*256);
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX7], SPRITE_INDEX7);
#endif

#if	OSD_AUX_EN
//sprite_8	-- main menu for aux
	memset(&stOSD[SPRITE_INDEX8], 0, sizeof(SPRITE_CTL_INFO));		// clear stOSD[SPRITE_INDEX8]
	stOSD[SPRITE_INDEX8].ctrl			= 0;						// font palette addr = 0
	stOSD[SPRITE_INDEX8].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEX8].x				= 50;
	stOSD[SPRITE_INDEX8].y				= 50;
	stOSD[SPRITE_INDEX8].stBMP.w		= OSD_FONT_W*28;			// stFONT.w * 28-column
	stOSD[SPRITE_INDEX8].stBMP.h		= OSD_FONT_H*14;//OSD_FONT_H*12;			// stFONT.h * 12-line
	stOSD[SPRITE_INDEX8].stBMP.addr		= stOSD[SPRITE_INDEX0].stBMP.addr;
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX8], SPRITE_INDEX8);

//sprite_9	-- size,position for aux
	memset(&stOSD[SPRITE_INDEX9], 0, sizeof(SPRITE_CTL_INFO));		// clear stOSD[SPRITE_INDEX1]
	stOSD[SPRITE_INDEX9].ctrl			= 0;						// font palette addr = 0
	stOSD[SPRITE_INDEX9].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEX9].x				= 50;
	stOSD[SPRITE_INDEX9].y				= 50;
	stOSD[SPRITE_INDEX9].stBMP.w		= OSD_FONT_W*36;			// stFONT.w * 36-column
	stOSD[SPRITE_INDEX9].stBMP.h		= OSD_FONT_H*2;				// stFONT.h * 2-line
	stOSD[SPRITE_INDEX9].stBMP.addr		= stOSD[SPRITE_INDEX1].stBMP.addr;
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX9], SPRITE_INDEX9);
	
//sprite_A	-- CAM ID
	memset(&stOSD[SPRITE_INDEXA], 0, sizeof(SPRITE_CTL_INFO));
	stOSD[SPRITE_INDEXA].ctrl			= 0;
	stOSD[SPRITE_INDEXA].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEXA].x				= 600;
	stOSD[SPRITE_INDEXA].y				= 10;
	stOSD[SPRITE_INDEXA].stBMP.w		= OSD_FONT_W*10;			// 10-column
	stOSD[SPRITE_INDEXA].stBMP.h		= OSD_FONT_H*1;				// 1-line
	stOSD[SPRITE_INDEXA].stBMP.addr		= stOSD[SPRITE_INDEX4].stBMP.addr;
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEXA], SPRITE_INDEXA);
	
//sprite_B	-- message osd
	memset(&stOSD[SPRITE_INDEXB], 0, sizeof(SPRITE_CTL_INFO));
	stOSD[SPRITE_INDEXB].ctrl			= 0;
	stOSD[SPRITE_INDEXB].alpha			= SPRITE_OPAQUE;
	stOSD[SPRITE_INDEXB].x				= 100;
	stOSD[SPRITE_INDEXB].y				= 100;
	stOSD[SPRITE_INDEXB].stBMP.w		= OSD_FONT_W*20;			// 20-column
	stOSD[SPRITE_INDEXB].stBMP.h		= OSD_FONT_H*1;				// 1-line
	stOSD[SPRITE_INDEXB].stBMP.addr		= stOSD[SPRITE_INDEX5].stBMP.addr;
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEXB], SPRITE_INDEXB);
#endif
}

#if defined(GOSD_TEST)
//--------------------------------------------------------------------------------------------------------------------------
static void CreateCursorInstance(void)
{
	MISP_BITMAP_INFO stBMP;
	DWORD e_addr = stOSD[SPRITE_INDEX7].stBMP.addr;
	
	// load cursor bmp
	stBMP.pBuff		= (PBYTE)misp_aero_cnv_bmp;
	stBMP.size		= sizeof(misp_aero_cnv_bmp);
	stBMP.addr		= e_addr-32*32;		// X-32*32;
	MISPOSD_SetBitmapData(stBMP.addr, stBMP.size, stBMP.pBuff);

#if __MISP100_DBGPRT__ == 1
	UARTprintf("[OSD] CUR-BMP addr = %d, size = %d\n", stBMP.addr, stBMP.size);
#endif

	memset(&stCurCTL, 0, sizeof(CURSOR_CTL_INFO));
	stCurCTL.ctrl			= CURSOR_32BIT_8COLOR;
	stCurCTL.alpha			= CURSOR_OPAQUE;
	stCurCTL.stBMP.w		= 32;
	stCurCTL.stBMP.h		= 32;
	stCurCTL.stBMP.addr		= stBMP.addr;
	stCurCTL.stPAL.pBuff	= (PBYTE)misp_aero_yuv_pal;//(PBYTE)misp_aero_rgb_pal;
	stCurCTL.stPAL.size		= sizeof(misp_aero_yuv_pal);//sizeof(misp_aero_rgb_pal);

	MISPCUR_SetCursorConfig(&stCurCTL);
	MISPCUR_SetCursorPosition(&stCurCTL, 1000, 640);
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
static void CreateSBoxInstance(void)
{
	// main sbox osd
	memset(&stSBOX[SBOX_INDEX0], 0, sizeof(SBOX_CTL_INFO));
	stSBOX[SBOX_INDEX0].ctrl		= SBOX_BORDER_OFF | SBOX_PLANE_OFF;
	stSBOX[SBOX_INDEX0].alpha		= SBOX_OPAQUE;
	stSBOX[SBOX_INDEX0].h_thk		= 1;					// 0 ~ 3
	stSBOX[SBOX_INDEX0].v_thk		= 1;					// 0 ~ 3
	stSBOX[SBOX_INDEX0].b_color		= RGB(179,165,42); 		// border color : orange(cr,y,cb)
	stSBOX[SBOX_INDEX0].p_color		= RGB(128,16,128); 		// plane color : black(cr,Y,cb)
	stSBOX[SBOX_INDEX0].stRECT.lx	= 200;
	stSBOX[SBOX_INDEX0].stRECT.ly	= 200;
	stSBOX[SBOX_INDEX0].stRECT.rx	= 250;
	stSBOX[SBOX_INDEX0].stRECT.ry	= 250;
	MISPOSD_SetSBoxConfig(&stSBOX[SBOX_INDEX0], SBOX_INDEX0);
	
	memset(&stSBOX[SBOX_INDEX1], 0, sizeof(SBOX_CTL_INFO));
	stSBOX[SBOX_INDEX1].ctrl		= SBOX_BORDER_OFF | SBOX_PLANE_OFF;
	stSBOX[SBOX_INDEX1].alpha		= SBOX_OPAQUE;
	stSBOX[SBOX_INDEX1].h_thk		= 1;					// 0 ~ 3
	stSBOX[SBOX_INDEX1].v_thk		= 1;					// 0 ~ 3
	stSBOX[SBOX_INDEX1].b_color		= RGB(110,41,240); 		// border color : blue(cr,y,cb)
	stSBOX[SBOX_INDEX1].p_color		= RGB(128,16,128); 		// plane color : black(cr,y,cb)
	stSBOX[SBOX_INDEX1].stRECT.lx	= 300;
	stSBOX[SBOX_INDEX1].stRECT.ly	= 200;
	stSBOX[SBOX_INDEX1].stRECT.rx	= 350;
	stSBOX[SBOX_INDEX1].stRECT.ry	= 250;
	MISPOSD_SetSBoxConfig(&stSBOX[SBOX_INDEX1], SBOX_INDEX1);

	memset(&stSBOX[SBOX_INDEX2], 0, sizeof(SBOX_CTL_INFO));
	stSBOX[SBOX_INDEX2].ctrl		= SBOX_BORDER_OFF | SBOX_PLANE_OFF;
	stSBOX[SBOX_INDEX2].alpha		= SBOX_OPAQUE;
	stSBOX[SBOX_INDEX2].h_thk		= 1;					// 0 ~ 3
	stSBOX[SBOX_INDEX2].v_thk		= 1;					// 0 ~ 3
	stSBOX[SBOX_INDEX2].b_color		= RGB(90,177,35); 		// border color : green(cr,y,cb)
	stSBOX[SBOX_INDEX2].p_color		= RGB(128,16,128); 		// plane color : black(cr,y,cb)
	stSBOX[SBOX_INDEX2].stRECT.lx	= 400;
	stSBOX[SBOX_INDEX2].stRECT.ly	= 200;
	stSBOX[SBOX_INDEX2].stRECT.rx	= 450;
	stSBOX[SBOX_INDEX2].stRECT.ry	= 250;
	MISPOSD_SetSBoxConfig(&stSBOX[SBOX_INDEX2], SBOX_INDEX2);

	memset(&stSBOX[SBOX_INDEX3], 0, sizeof(SBOX_CTL_INFO));
	stSBOX[SBOX_INDEX3].ctrl		= SBOX_BORDER_OFF | SBOX_PLANE_OFF;
	stSBOX[SBOX_INDEX3].alpha		= SBOX_OPAQUE;
	stSBOX[SBOX_INDEX3].h_thk		= 1;					// 0 ~ 3
	stSBOX[SBOX_INDEX3].v_thk		= 1;					// 0 ~ 3
	stSBOX[SBOX_INDEX3].b_color		= RGB(240,82,90); 		// border color : red(cr,y,cb)
	stSBOX[SBOX_INDEX3].p_color		= RGB(128,16,128); 		// plane color : black(cr,y,cb)
	stSBOX[SBOX_INDEX3].stRECT.lx	= 500;
	stSBOX[SBOX_INDEX3].stRECT.ly	= 200;
	stSBOX[SBOX_INDEX3].stRECT.rx	= 550;
	stSBOX[SBOX_INDEX3].stRECT.ry	= 250;
	MISPOSD_SetSBoxConfig(&stSBOX[SBOX_INDEX3], SBOX_INDEX3);

	memset(&stSBOX[SBOX_INDEX4], 0, sizeof(SBOX_CTL_INFO));
	stSBOX[SBOX_INDEX4].ctrl		= SBOX_BORDER_OFF | SBOX_PLANE_OFF;
	stSBOX[SBOX_INDEX4].alpha		= SBOX_OPAQUE;
	stSBOX[SBOX_INDEX4].h_thk		= 1;					// 0 ~ 3
	stSBOX[SBOX_INDEX4].v_thk		= 1;					// 0 ~ 3
	stSBOX[SBOX_INDEX4].b_color		= RGB(171,169,162);		// border color : purple(cr,y,cb)
	stSBOX[SBOX_INDEX4].p_color		= RGB(128,16,128); 		// plane color : black(cr,y,cb)
	stSBOX[SBOX_INDEX4].stRECT.lx	= 200;
	stSBOX[SBOX_INDEX4].stRECT.ly	= 400;
	stSBOX[SBOX_INDEX4].stRECT.rx	= 250;
	stSBOX[SBOX_INDEX4].stRECT.ry	= 450;
	MISPOSD_SetSBoxConfig(&stSBOX[SBOX_INDEX4], SBOX_INDEX4);

	memset(&stSBOX[SBOX_INDEX5], 0, sizeof(SBOX_CTL_INFO));
	stSBOX[SBOX_INDEX5].ctrl		= SBOX_BORDER_OFF | SBOX_PLANE_OFF;
	stSBOX[SBOX_INDEX5].alpha		= SBOX_OPAQUE;
	stSBOX[SBOX_INDEX5].h_thk		= 1;					// 0 ~ 3
	stSBOX[SBOX_INDEX5].v_thk		= 1;					// 0 ~ 3
	stSBOX[SBOX_INDEX5].b_color		= RGB(146,210,16); 		// border color : yellow(cr,y,cb)
	stSBOX[SBOX_INDEX5].p_color		= RGB(128,16,128); 		// plane color : black(cr,y,cb)
	stSBOX[SBOX_INDEX5].stRECT.lx	= 300;
	stSBOX[SBOX_INDEX5].stRECT.ly	= 400;
	stSBOX[SBOX_INDEX5].stRECT.rx	= 350;
	stSBOX[SBOX_INDEX5].stRECT.ry	= 450;
	MISPOSD_SetSBoxConfig(&stSBOX[SBOX_INDEX5], SBOX_INDEX5);

	memset(&stSBOX[SBOX_INDEX6], 0, sizeof(SBOX_CTL_INFO));
	stSBOX[SBOX_INDEX6].ctrl		= SBOX_BORDER_OFF | SBOX_PLANE_OFF;
	stSBOX[SBOX_INDEX6].alpha		= SBOX_OPAQUE;
	stSBOX[SBOX_INDEX6].h_thk		= 1;					// 0 ~ 3
	stSBOX[SBOX_INDEX6].v_thk		= 1;					// 0 ~ 3
	stSBOX[SBOX_INDEX6].b_color		= RGB(16,170,166);		// border color : skyblue(cr,y,cb)
	stSBOX[SBOX_INDEX6].p_color		= RGB(128,16,128); 		// plane color : black(cr,y,cb)
	stSBOX[SBOX_INDEX6].stRECT.lx	= 400;
	stSBOX[SBOX_INDEX6].stRECT.ly	= 400;
	stSBOX[SBOX_INDEX6].stRECT.rx	= 450;
	stSBOX[SBOX_INDEX6].stRECT.ry	= 450;
	MISPOSD_SetSBoxConfig(&stSBOX[SBOX_INDEX6], SBOX_INDEX6);

	memset(&stSBOX[SBOX_INDEX7], 0, sizeof(SBOX_CTL_INFO));
	stSBOX[SBOX_INDEX7].ctrl		= SBOX_BORDER_OFF | SBOX_PLANE_OFF;
	stSBOX[SBOX_INDEX7].alpha		= SBOX_OPAQUE;
	stSBOX[SBOX_INDEX7].h_thk		= 1;					// 0 ~ 3
	stSBOX[SBOX_INDEX7].v_thk		= 1;					// 0 ~ 3
	stSBOX[SBOX_INDEX7].b_color		= RGB(128,235,128); 	// border color : white(cr,y,cb)
	stSBOX[SBOX_INDEX7].p_color		= RGB(128,16,128); 		// plane color : black(cr,y,cb)
	stSBOX[SBOX_INDEX7].stRECT.lx	= 500;
	stSBOX[SBOX_INDEX7].stRECT.ly	= 400;
	stSBOX[SBOX_INDEX7].stRECT.rx	= 550;
	stSBOX[SBOX_INDEX7].stRECT.ry	= 450;
	MISPOSD_SetSBoxConfig(&stSBOX[SBOX_INDEX7], SBOX_INDEX7);

#if	OSD_AUX_EN
	//aux sbox osd
	memset(&stSBOX[SBOX_INDEX8], 0, sizeof(SBOX_CTL_INFO));
	stSBOX[SBOX_INDEX8].ctrl		= SBOX_BORDER_OFF | SBOX_PLANE_OFF;
	stSBOX[SBOX_INDEX8].alpha		= SBOX_OPAQUE;
	stSBOX[SBOX_INDEX8].h_thk		= 2;					// 0 ~ 3
	stSBOX[SBOX_INDEX8].v_thk		= 0;					// 0 ~ 3
	stSBOX[SBOX_INDEX8].b_color		= RGB(179,165,42); 		// border color : orange(cr,y,cb)
	stSBOX[SBOX_INDEX8].p_color		= RGB(128,16,128); 		// plane color : black(cr,y,cb)
	stSBOX[SBOX_INDEX8].stRECT.lx	= 200;
	stSBOX[SBOX_INDEX8].stRECT.ly	= 100;
	stSBOX[SBOX_INDEX8].stRECT.rx	= 300;
	stSBOX[SBOX_INDEX8].stRECT.ry	= 120;
	MISPOSD_SetSBoxConfig(&stSBOX[SBOX_INDEX8], SBOX_INDEX8);

	memset(&stSBOX[SBOX_INDEX9], 0, sizeof(SBOX_CTL_INFO));
	stSBOX[SBOX_INDEX9].ctrl		= SBOX_BORDER_OFF | SBOX_PLANE_OFF;
	stSBOX[SBOX_INDEX9].alpha		= SBOX_OPAQUE;
	stSBOX[SBOX_INDEX9].h_thk		= 2;					// 0 ~ 3
	stSBOX[SBOX_INDEX9].v_thk		= 0;					// 0 ~ 3
	stSBOX[SBOX_INDEX9].b_color		= RGB(110,41,240); 		// border color : blue(cr,y,cb)
	stSBOX[SBOX_INDEX9].p_color		= RGB(128,16,128); 		// plane color : black(cr,y,cb)
	stSBOX[SBOX_INDEX9].stRECT.lx	= 400;
	stSBOX[SBOX_INDEX9].stRECT.ly	= 100;
	stSBOX[SBOX_INDEX9].stRECT.rx	= 500;
	stSBOX[SBOX_INDEX9].stRECT.ry	= 120;
	MISPOSD_SetSBoxConfig(&stSBOX[SBOX_INDEX9], SBOX_INDEX9);

	memset(&stSBOX[SBOX_INDEXA], 0, sizeof(SBOX_CTL_INFO));
	stSBOX[SBOX_INDEXA].ctrl		= SBOX_BORDER_OFF | SBOX_PLANE_OFF;
	stSBOX[SBOX_INDEXA].alpha		= SBOX_OPAQUE;
	stSBOX[SBOX_INDEXA].h_thk		= 2;					// 0 ~ 3
	stSBOX[SBOX_INDEXA].v_thk		= 0;					// 0 ~ 3
	stSBOX[SBOX_INDEXA].b_color		= RGB(90,177,35); 		// border color : green(cr,y,cb)
	stSBOX[SBOX_INDEXA].p_color		= RGB(128,16,128); 		// plane color : black(cr,y,cb)
	stSBOX[SBOX_INDEXA].stRECT.lx	= 600;
	stSBOX[SBOX_INDEXA].stRECT.ly	= 100;
	stSBOX[SBOX_INDEXA].stRECT.rx	= 700;
	stSBOX[SBOX_INDEXA].stRECT.ry	= 120;
	MISPOSD_SetSBoxConfig(&stSBOX[SBOX_INDEXA], SBOX_INDEXA);

	memset(&stSBOX[SBOX_INDEXB], 0, sizeof(SBOX_CTL_INFO));
	stSBOX[SBOX_INDEXB].ctrl		= SBOX_BORDER_OFF | SBOX_PLANE_OFF;
	stSBOX[SBOX_INDEXB].alpha		= SBOX_OPAQUE;
	stSBOX[SBOX_INDEXB].h_thk		= 2;					// 0 ~ 3
	stSBOX[SBOX_INDEXB].v_thk		= 0;					// 0 ~ 3
	stSBOX[SBOX_INDEXB].b_color		= RGB(240,82,90); 		// border color : red(cr,y,cb)
	stSBOX[SBOX_INDEXB].p_color		= RGB(128,16,128); 		// plane color : black(cr,y,cb)
	stSBOX[SBOX_INDEXB].stRECT.lx	= 800;
	stSBOX[SBOX_INDEXB].stRECT.ly	= 100;
	stSBOX[SBOX_INDEXB].stRECT.rx	= 900;
	stSBOX[SBOX_INDEXB].stRECT.ry	= 120;
	MISPOSD_SetSBoxConfig(&stSBOX[SBOX_INDEXB], SBOX_INDEXB);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void OSD_SetLayers(void)
{
	memset(&stLayer[LAYER_INDEX0], 0, sizeof(LAYER_CTL_INFO));		// clear stLayer[LAYER_INDEX0]
	if((GetOUT_MainMode()==MISP_OUT_RGB444_8) || (GetOUT_MainMode()==MISP_OUT_RGB444_10))	{
		stLayer[LAYER_INDEX0].ctrl |= LAYER_CSC_YUV2RGB;
	}
	else	{
		stLayer[LAYER_INDEX0].ctrl |= LAYER_CSCBYPASS_ON;
	}
	MISPOSD_SetLayerConfig(&stLayer[LAYER_INDEX0], LAYER_INDEX0);

	memset(&stLayer[LAYER_INDEX1], 0, sizeof(LAYER_CTL_INFO));		// clear stLayer[LAYER_INDEX1]
	if((GetOUT_MainMode()==MISP_OUT_RGB444_8) || (GetOUT_MainMode()==MISP_OUT_RGB444_10))	{
		stLayer[LAYER_INDEX1].ctrl |= LAYER_CSC_YUV2RGB;
	}
	else	{
		stLayer[LAYER_INDEX1].ctrl |= LAYER_CSCBYPASS_ON;
	}
	MISPOSD_SetLayerConfig(&stLayer[LAYER_INDEX1], LAYER_INDEX1);

	memset(&stLayer[LAYER_INDEX2], 0, sizeof(LAYER_CTL_INFO));		// clear stLayer[LAYER_INDEX1]
	MISPOSD_SetLayerConfig(&stLayer[LAYER_INDEX2], LAYER_INDEX2);
}

//--------------------------------------------------------------------------------------------------------------------------
void OSD_SetFontLang(BYTE lang)	// 0(eng), 1(chn-1), 2(chn-2)
{
#if defined(__USE_CHN_FONT__)
	switch(lang)	{
		case  0:	stFONT.addr	= (DWORD)DDR_MEM_SIZE - sizeof(misp_font_cnv_bmp);		break;	// eng
		case  1:	stFONT.addr	= (DWORD)DDR_MEM_SIZE - (sizeof(misp_font_cnv_bmp) + sizeof(misp_font_cnv_bmp_chn1));	break;	// chn-1
		case  2:	stFONT.addr	= (DWORD)DDR_MEM_SIZE - (sizeof(misp_font_cnv_bmp) + sizeof(misp_font_cnv_bmp_chn1) + sizeof(misp_font_cnv_bmp_chn2));	break;	// chn-2
	}
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void OSD_SetFontMAP(void)
{
	MISP_BITMAP_INFO stBMP;
	DWORD e_addr = (DWORD)DDR_MEM_SIZE;		//256Mb:0x2000000, 512Mb:0x4000000

	stBMP.pBuff		= (PBYTE)misp_font_cnv_bmp;
	stBMP.size		= sizeof(misp_font_cnv_bmp);
	stBMP.addr		= e_addr-stBMP.size;

	MISPOSD_SetBitmapData(stBMP.addr, stBMP.size, stBMP.pBuff);

#if defined(__USE_CHN_FONT__)
	stBMP.pBuff		= (PBYTE)misp_font_cnv_bmp_chn1;
	stBMP.size		= sizeof(misp_font_cnv_bmp_chn1) + sizeof(misp_font_cnv_bmp_chn2);
	stBMP.addr		= stBMP.addr-stBMP.size;

	MISPOSD_SetBitmapData(stBMP.addr, stBMP.size, stBMP.pBuff);
#endif
	
#if __MISP100_DBGPRT__ == 1
	UARTprintf("[OSD] FONT-BMP addr = 0x%x ~ 0x%x, size = %d byte\n", stBMP.addr, e_addr-1, stBMP.size);
#endif

	stFONT.mode		= MISP_GAC_DRAW_XYMODE;
	stFONT.attb		= GACMAP_2BITPALT_COLOR | MISP_GAC_16Vx32H_GRID | MISP_GAC_HORI_AUTOINC;
	
	stFONT.w		= OSD_FONT_W;
	stFONT.h		= OSD_FONT_H;
	stFONT.addr		= e_addr - sizeof(misp_font_cnv_bmp);
	MISPGAC_SetFontMode(&stFONT, &stOSD[SPRITE_INDEX0]);
#if	OSD_AUX_EN
	MISPGAC_SetFontMode(&stFONT, &stOSD[SPRITE_INDEX8]);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void OSD_LoadPalette(void)
{
	PLAYER_CTL_INFO pCTL = &stLayer[LAYER_INDEX0];
	PLAYER_PAL_INFO pPAL = &stLayer[LAYER_INDEX0].stPAL;
	
	pPAL->addr		= 0;
	pPAL->size		= sizeof(misp_font_yuv_pal);
	pPAL->pBuff		= (PBYTE)misp_font_yuv_pal;
	MISPOSD_SetLayerPalette(pCTL, pPAL->addr, pPAL->size, pPAL->pBuff);
	
	pCTL = &stLayer[LAYER_INDEX1];
	pPAL = &stLayer[LAYER_INDEX1].stPAL;
	
	pPAL->addr		= 0;
	pPAL->size		= sizeof(misp_font_yuv_pal);
	pPAL->pBuff		= (PBYTE)misp_font_yuv_pal;
	MISPOSD_SetLayerPalette(pCTL, pPAL->addr, pPAL->size, pPAL->pBuff);
	
	pCTL = &stLayer[LAYER_INDEX2];
	pPAL = &stLayer[LAYER_INDEX2].stPAL;
	
	pPAL->addr		= 0;
	pPAL->size		= sizeof(mispaux_font_yuv_pal);
	pPAL->pBuff		= (PBYTE)mispaux_font_yuv_pal;
	MISPOSD_SetLayerPalette(pCTL, pPAL->addr, pPAL->size, pPAL->pBuff);
}

//--------------------------------------------------------------------------------------------------------------------------
static void OSD_LoadMenuStatus(void)
{
	SetMenuFactoryDefault(); 		// clear menu data
	SetMenuInitValue(); 			// set menu data with api parameter & eeprom
}

#if defined(GOSD_TEST)
//--------------------------------------------------------------------------------------------------------------------------
void OSD_FILL_test(void)
{
	GACFILL_BOX_INFO stFILL;
	
	memset(&stFILL, 0, sizeof(GACFILL_BOX_INFO));
	stFILL.attb		= GACMAP_16BITFULL_COLOR;//GACMAP_32BITFULL_COLOR;
	MISPGAC_SetFillMode(&stFILL, &stOSD[SPRITE_INDEX7]);

	stFILL.x = 0; stFILL.y = 0; stFILL.w = ICON_W; stFILL.h = ICON_H;
	MISPGAC_FillRectangle(&stFILL, aRGB(31, 16, 16, 16));	// black(R,G,B)
	
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX7], ON);
	UARTprintf("\r\n[OSD] %dBIT-FILL : %dx%d \r\n", ((stFILL.attb==GACMAP_16BITFULL_COLOR)? 16: 32),
				stFILL.w, stFILL.h);
}

//--------------------------------------------------------------------------------------------------------------------------
void OSD_BMP_test(void)
{
	MISP_BITMAP_INFO stBMP;	
	
	memset(&stBMP, 0, sizeof(MISP_BITMAP_INFO));
	stBMP.pBuff		= (PBYTE)SPIROM_BMP;
	stBMP.size		= (DWORD)sizeof(SPIROM_BMP);
	stBMP.addr		= stOSD[SPRITE_INDEX7].stBMP.addr;
	MISPOSD_SetBitmapData(stBMP.addr, stBMP.size, stBMP.pBuff);
	
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX7], ON);
	UARTprintf("\r\n[OSD] %dBIT-BMP : addr(0x%x)\r\n", stBMP.size/(ICON_W*ICON_H)*8, stBMP.addr);
}

//--------------------------------------------------------------------------------------------------------------------------
static void CopySPT(PSPRITE_CTL_INFO pSRC_SPT, PSPRITE_CTL_INFO pDST_SPT)
{
	GACCOPY_BMP_INFO stBMP;	MISP_AREA_BOX stSRC, stDST;

	memset(&stBMP, 0, sizeof(GACCOPY_BMP_INFO));	// clear stBMP
	//stBMP.attb		= GACMAP_2BITPALT_COLOR;
	stBMP.attb		= GACMAP_4BITPALT_COLOR;
	//stBMP.attb		= GACMAP_16BITFULL_COLOR;
	//stBMP.attb		= GACMAP_24BITFULL_COLOR;
	//stBMP.attb		= GACMAP_32BITFULL_COLOR;
	stBMP.w			= pSRC_SPT->stBMP.w;
	stBMP.h			= pSRC_SPT->stBMP.h;
	stBMP.addr		= pSRC_SPT->stBMP.addr;

	MISPGAC_SetCopyMode(&stBMP, pDST_SPT);

	stSRC.x =   0; stSRC.y =   0; stSRC.w = stBMP.w; stSRC.h = stBMP.h;
	stDST.x =   0; stDST.y =   0; stDST.w = stBMP.w; stDST.h = stBMP.h;
	MISPGAC_CopyRectangle(&stSRC, &stDST);
}

//--------------------------------------------------------------------------------------------------------------------------
static void FillSPT(PSPRITE_CTL_INFO pINFO, WORD x, WORD y, WORD w, WORD h, DWORD color)
{
	GACFILL_BOX_INFO stFILL;

	memset(&stFILL, 0, sizeof(GACFILL_BOX_INFO));	// clear stBOX
	//stFILL.attb		= GACMAP_2BITPALT_COLOR;
	stFILL.attb		= GACMAP_4BITPALT_COLOR;
	//stFILL.attb		= GACMAP_16BITFULL_COLOR;
	//stFILL.attb		= GACMAP_24BITFULL_COLOR;
	//stFILL.attb		= GACMAP_32BITFULL_COLOR;
	MISPGAC_SetFillMode(&stFILL, pINFO);

	stFILL.x = x; stFILL.y = y; stFILL.w = w; stFILL.h = h;
	MISPGAC_FillRectangle(&stFILL, color);
}


#endif

//--------------------------------------------------------------------------------------------------------------------------
void CreateOSDInstance(void)
{
	OSD_SetSprites();				// configure sprites
	OSD_SetLayers();				// configure layers
	OSD_LoadPalette();				// load palette data
	OSD_SetFontMAP();				// set GAC in character mode

	CreateSBoxInstance();

#ifdef	GOSD_TEST
	CreateCursorInstance();
	MISPCUR_EnableCursor(&stCurCTL, ON);
	OSD_BMP_test();
//	OSD_FILL_test();
#endif
	
	
	OSD_LoadMenuStatus(); 		// load menu item init value
	
#if defined(__USE_OSD_TITLE__)
	DEMO_SetOSD_Title(ON);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void OSD_SetFontGAC(SPRITE_INDEX_t index)
{
	MISPGAC_SetFontMode(&stFONT, &stOSD[index]);
}

//--------------------------------------------------------------------------------------------------------------------------
void OSD_ModifyPalette_M(BOOL rgb)
{
	PLAYER_CTL_INFO pCTL = &stLayer[LAYER_INDEX0];
	PLAYER_PAL_INFO pPAL = &stLayer[LAYER_INDEX0].stPAL;

	if (rgb==OSD_RGB_PALETTE) pPAL->pBuff = (PBYTE)misp_font_rgb_pal;
	else					  pPAL->pBuff = (PBYTE)misp_font_yuv_pal;

	pPAL->addr		= 0;
	pPAL->size		= sizeof(misp_font_rgb_pal);
	MISPOSD_SetLayerPalette(pCTL, pPAL->addr, pPAL->size, pPAL->pBuff);
}

//--------------------------------------------------------------------------------------------------------------------------
void OSD_ModifyPalette_X(BOOL rgb)
{
	PLAYER_CTL_INFO pCTL = &stLayer[LAYER_INDEX2];
	PLAYER_PAL_INFO pPAL = &stLayer[LAYER_INDEX2].stPAL;

	if (rgb==OSD_RGB_PALETTE) pPAL->pBuff = (PBYTE)mispaux_font_rgb_pal;
	else					  pPAL->pBuff = (PBYTE)mispaux_font_yuv_pal;

	pPAL->addr		= 0;
	pPAL->size		= sizeof(mispaux_font_rgb_pal);
	MISPOSD_SetLayerPalette(pCTL, pPAL->addr, pPAL->size, pPAL->pBuff);
}

//--------------------------------------------------------------------------------------------------------------------------
void OSD_ViewFontTable(void)
{
	BYTE	pSTR[28], i, j;
	WORD 	font_idx, font_qty;
	
	if(stOSD[SPRITE_INDEX0].ctrl&SPRITE_DISPLAY_ON)	{
		MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX0], OFF);	return;
	}
	
	font_idx=0;	font_qty = sizeof(misp_font_cnv_bmp)/sizeof(misp_font_cnv_bmp[0]);
	OSD_SetFontGAC(SPRITE_INDEX0);
	for(j=0;j<12;j++)	{
		for(i=0;i<28;i++)		pSTR[i] = (font_idx++ % font_qty);
		MISPGAC_SetDrawXYMode(&stFONT, j, 0, pSTR, 28, 0x4f);
	}
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX0], ON);
}

//--------------------------------------------------------------------------------------------------------------------------
void OSD_SetMainCSC(BYTE nID)
{
	if(nID == OSD_CSC_BYPASS)	{
		stLayer[LAYER_INDEX0].ctrl |= LAYER_CSCBYPASS_ON;
		stLayer[LAYER_INDEX1].ctrl |= LAYER_CSCBYPASS_ON;
	}
	else	{
		stLayer[LAYER_INDEX0].ctrl &= ~LAYER_CSCBYPASS_ON;
		stLayer[LAYER_INDEX1].ctrl &= ~LAYER_CSCBYPASS_ON;
		
		if(nID == OSD_CSC_YUV2RGB)	{
			stLayer[LAYER_INDEX0].ctrl |= LAYER_CSC_YUV2RGB;
			stLayer[LAYER_INDEX1].ctrl |= LAYER_CSC_YUV2RGB;
		}
		else	{
			stLayer[LAYER_INDEX0].ctrl |= LAYER_CSC_RGB2YUV;
			stLayer[LAYER_INDEX1].ctrl |= LAYER_CSC_RGB2YUV;
		}
	}
	
	MISPOSD_SetLayerConfig(&stLayer[LAYER_INDEX0], LAYER_INDEX0);
	MISPOSD_SetLayerConfig(&stLayer[LAYER_INDEX1], LAYER_INDEX1);
}

//--------------------------------------------------------------------------------------------------------------------------
void OSD_DrawVSboard(void)
{
//#if defined(__USE_VECTOR_SCOPE__)
	WORD i,j;
	BYTE buff[256];
	
	for(i=0;i<256;i++)	{
		for(j=0;j<256/8;j++)	{		// 1bit bitmap -> 8bit bitmap
			buff[j*8+0] = (VS_cnv_bmp[i*32+j] >> 7) &0x01;
			buff[j*8+1] = (VS_cnv_bmp[i*32+j] >> 6) &0x01;
			buff[j*8+2] = (VS_cnv_bmp[i*32+j] >> 5) &0x01;
			buff[j*8+3] = (VS_cnv_bmp[i*32+j] >> 4) &0x01;
			buff[j*8+4] = (VS_cnv_bmp[i*32+j] >> 3) &0x01;
			buff[j*8+5] = (VS_cnv_bmp[i*32+j] >> 2) &0x01;
			buff[j*8+6] = (VS_cnv_bmp[i*32+j] >> 1) &0x01;
			buff[j*8+7] = (VS_cnv_bmp[i*32+j] >> 0) &0x01;
		}
		MISP_MultiWrite(MISP_SDRAM_ID, stOSD[SPRITE_INDEX7].stBMP.addr + i*256, &buff[0], sizeof(buff));
	}
//#endif
}

#if defined(__USE_OSD_TITLE__)
//--------------------------------------------------------------------------------------------------------------------------
static void OSD_DrawTextBMP(PSPRITE_CTL_INFO pINFO, PBYTE pBuff, WORD lx, WORD ly, WORD bmp_w, WORD bmp_h, BYTE col_idx)
{
	WORD i,j;
	BYTE buff[bmp_w];
	DWORD e_addr = pINFO->stBMP.addr + lx + pINFO->stBMP.w * ly;
	
	for(i=0;i<bmp_h;i++)	{
		for(j=0;j<bmp_w/4;j++)	{		// convert 2bit bitmap -> 8bit bitmap
			buff[j*4+0] = ((*(pBuff+i*bmp_w/4+j) >> 6) &0x03) + col_idx;
			buff[j*4+1] = ((*(pBuff+i*bmp_w/4+j) >> 4) &0x03) + col_idx;
			buff[j*4+2] = ((*(pBuff+i*bmp_w/4+j) >> 2) &0x03) + col_idx;
			buff[j*4+3] = ((*(pBuff+i*bmp_w/4+j) >> 0) &0x03) + col_idx;
		}
		
		MISP_MultiWrite(MISP_SDRAM_ID, e_addr, &buff[0], sizeof(buff));	// write bitmap data
		e_addr += pINFO->stBMP.w;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void OSD_SetTitleBMP(BOOL OnOff, BYTE font_index)
{
	BYTE font_w=64, font_h=88, h_rpt, v_rpt;
	WORD off_x=0;
	PBYTE pBuff;
	
#if (defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX334__)) && !defined(__USE_4K60_INOUT__)
	h_rpt=2;	v_rpt=2;	// repeat x4
#else
	h_rpt=1;	v_rpt=1;	// repeat x2
#endif
	
	if(OnOff==OFF)		{
		MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX7], OFF);
		MISPOSD_EnableLayerRepeat(&stLayer[LAYER_INDEX1], 1, 1);
#if defined(__USE_4K60_INOUT__)
		MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX3], OFF);
#endif
		//restore sprite6 position
		stOSD[SPRITE_INDEX6].y = 80;
		MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX6], SPRITE_INDEX6);
		return;
	}
	
	if(HI4BIT(font_index)==2)	{		// ex-sdi
		off_x = font_w*2*(1<<h_rpt);	// sprite move : ch*2
	}
	
	// adjust sprite7 size/position
#if defined(__USE_4K60_INOUT__)
	stOSD[SPRITE_INDEX7].x = 0;
	stOSD[SPRITE_INDEX7].y = 50;
	stOSD[SPRITE_INDEX7].stBMP.w		= font_w*15;
	stOSD[SPRITE_INDEX7].stBMP.h		= font_h*2;
#else
	stOSD[SPRITE_INDEX7].x = GetOUT_MainSizeHA()/2 - stOSD[SPRITE_INDEX7].stBMP.w*(1<<h_rpt)/2 +off_x;
	stOSD[SPRITE_INDEX7].y = 0;
	stOSD[SPRITE_INDEX7].stBMP.w		= font_w*15;
	stOSD[SPRITE_INDEX7].stBMP.h		= font_h*1;
#endif
	stOSD[SPRITE_INDEX7].stBMP.addr		= stOSD[SPRITE_INDEX6].stBMP.addr - (font_w*15*font_h*2);
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX7], SPRITE_INDEX7);
	MISPOSD_EnableLayerRepeat(&stLayer[LAYER_INDEX1], h_rpt, v_rpt);
	
	// adjust sprite6 y position with sprite7
	stOSD[SPRITE_INDEX6].y = stOSD[SPRITE_INDEX7].y + stOSD[SPRITE_INDEX7].stBMP.h * (v_rpt*2);
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX6], SPRITE_INDEX6);
	
	// clear sprite
	OSD_DrawTextBMP(&stOSD[SPRITE_INDEX7], (PBYTE)&BLANK384x88_BMP[0], 0, 0, 384, 88, 0x04);
	OSD_DrawTextBMP(&stOSD[SPRITE_INDEX7], (PBYTE)&BLANK384x88_BMP[0], 0, 88, 384, 88, 0x04);
	OSD_DrawTextBMP(&stOSD[SPRITE_INDEX7], (PBYTE)&BLANK576x88_BMP[0], 384, 0, 576, 88, 0x04);
	OSD_DrawTextBMP(&stOSD[SPRITE_INDEX7], (PBYTE)&BLANK576x88_BMP[0], 384, 88, 576, 88, 0x04);
	
	// vout type
	switch(HI4BIT(font_index))	{
		case 1:		pBuff = (PBYTE)&HDMI384x88_BMP[0];	break;//hdmi
		case 2:		pBuff = (PBYTE)&EXSDI384x88_BMP[0];	break;//ex-sdi
		case 3:		pBuff = (PBYTE)&TVI384x88_BMP[0];	break;//tvi
		default:	pBuff = (PBYTE)&BLANK384x88_BMP[0];	break;//blank
	}
#if defined(__USE_4K60_INOUT__)
#if defined(__USE_4K60_MD2__)
	OSD_DrawTextBMP(&stOSD[SPRITE_INDEX7], (PBYTE)&CAMERA384x88_BMP[0], 0, 0, 384, 88, 0x07);
#endif
	OSD_DrawTextBMP(&stOSD[SPRITE_INDEX7], pBuff, 0, 88, 384, 88, 0x04);
#else
	OSD_DrawTextBMP(&stOSD[SPRITE_INDEX7], pBuff, 0, 0, 384, 88, 0x04);
#endif
	
	// vout frmt
	switch(LO4BIT(font_index))	{
		case 1:		pBuff = (PBYTE)&o4K15p576x88_BMP[0];	break;//4K15p
		case 2:		pBuff = (PBYTE)&o4K30p576x88_BMP[0];	break;//4K30p
		case 3:		pBuff = (PBYTE)&FHD60p576x88_BMP[0];	break;//FHD60p
		case 4:		pBuff = (PBYTE)&FHD30p576x88_BMP[0];	break;//FHD30p
		case 5:		pBuff = (PBYTE)&o4K60p576x88_BMP[0];	break;//4K60p
		default:	pBuff = (PBYTE)&BLANK576x88_BMP[0];	break;//blank
	}
#if defined(__USE_4K60_INOUT__)
#if defined(__USE_4K60_MD1__)
	OSD_DrawTextBMP(&stOSD[SPRITE_INDEX7], (PBYTE)&o4KUHD576x88_BMP[0], 384, 0, 576, 88, 0x07);
#endif
	OSD_DrawTextBMP(&stOSD[SPRITE_INDEX7], pBuff, 384-64, 88, 576, 88, 0x04);
#else
	OSD_DrawTextBMP(&stOSD[SPRITE_INDEX7], pBuff, 384, 0, 576, 88, 0x04);
#endif
	
	
	// sbox(blue)
#if defined(__USE_4K60_INOUT__)
	MISPOSD_SetSBoxArea(&stSBOX[SBOX_INDEX3], 0, 0, GetOUT_MainSizeHA(), GetOUT_MainSizeVA());
	MISPOSD_SetSBoxBorderColor(&stSBOX[SBOX_INDEX3], RGB(110,41,240)); 			//Blue
	MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX3], ON);
#endif
	
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX7], ON);
}
#endif

/*  FILE_END_HERE */
