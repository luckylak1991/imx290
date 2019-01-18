//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2016  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	MISPMDPM.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<math.h>
#include	"misp100.h"
#include	"osd.h"
#include	"video.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
static PM_CTRL_PARAM		stPMCTRL;
static MD_CTRL_PARAM		stMDCTRL;

#if defined(__USE_4K60_INOUT__)
#define	PMCTRL_GetMainSrcSizeH()		GetIN_MainSrcSizeH()*2
#define	MDCTRL_GetMainSrcSizeH()		GetIN_MainSrcSizeH()*2
#else
#define	PMCTRL_GetMainSrcSizeH()		GetIN_MainSrcSizeH()
#define	MDCTRL_GetMainSrcSizeH()		GetIN_MainSrcSizeH()
#endif


#define		__USE_MD_SBOX_OSD__			// use sbox4 ~ sbox7 for md window display


// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

#if defined(__USE_MDIN_i500__) 		// MDIN-i500
static ROMDATA WORD defPM_PolyPosi[4][8]={
	// ulx,  uly, 	 urx,  ury, 	 dlx,  dly, 	 drx,  dry
	{ 512,  190, 	 808,  210, 	 464,  440, 	 864,  460},
	{1112,  190, 	1408,  210, 	1056,  440, 	1464,  460},
	{ 512,  640, 	 808,  670, 	 464,  890, 	 864,  940},
	{1112,  640, 	1408,  670, 	1056,  890, 	1464,  940},
};
#else 								// MDIN-i51X (i510, i540, i550)
static ROMDATA WORD defPM_PolyPosi[8][8]={
	// ulx,  uly, 	 urx,  ury, 	 dlx,  dly, 	 drx,  dry
	{ 176,  190, 	 432,  210, 	 144,  440, 	 448,  470},
	{ 608,  190, 	 880,  210, 	 592,  440, 	 896,  470},
	{1040,  190, 	1312,  210, 	1040,  440, 	1328,  470},
	{1488,  190, 	1744,  210, 	1472,  440, 	1776,  470},
	{ 176,  640, 	 432,  670, 	 144,  890, 	 448,  920},
	{ 608,  640, 	 880,  670, 	 592,  890, 	 896,  920},
	{1040,  640, 	1312,  670, 	1040,  890, 	1328,  920},
	{1488,  640, 	1744,  670, 	1472,  890, 	1776,  920},
};
#endif


// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
static void PMCTRL_SetRectSizePosi(BYTE no, WORD w, WORD h, SHORT x, SHORT y);
static void PMCTRL_SetRectColor(BYTE no, BYTE color);
static void PMCTRL_SetRectOnOff(BYTE no, BYTE OnOff);
static void PMCTRL_SetPolyColor(BYTE no, BYTE color);
static void PMCTRL_SetPolyOnOff(BYTE no, BYTE OnOff);
static void PMCTRL_SetPolyMinMax(BYTE no, PPM_POLY_INFO pINFO);
static void PMCTRL_SetPolySlope(BYTE no, PPM_POLY_INFO pINFO);
static void PMCTRL_SetType(BYTE type);
static void PMCTRL_SetOnOff(BYTE OnOff);
static void MDCTRL_SetSizePosi(BYTE no, BYTE w, BYTE h, BYTE lx, BYTE ly);
static void MDCTRL_SetAreaEn(BYTE no, WORD OnOff);
static void MDCTRL_SetAreaDisp(BYTE no, WORD OnOff);
static void MDCTRL_SetOnOff(WORD OnOff);
static void MDCTRL_SetMotionView(WORD OnOff);
static void MDCTRL_SetBlockSize(WORD size);
static void MDCTRL_SetThreshold(WORD val);
static void MDCTRL_SetVideoSize(WORD md_h_size, WORD md_v_size);

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
static void SetPM_Default(PPM_CTRL_PARAM pCTRL)
{
	BYTE i;
	WORD rect_w, rect_h, rect_lx, rect_ly, src_w, src_h;
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD2__)	// 4k60 slave only
	PM_POLY_INFO stPOLY;
#endif
	
	pCTRL->ctrl = (PMCTRL_DISABLE | PMCTRL_MODE_RECT); 				// pm disable & rect mode
	
	// pm rectangle default parameter
	src_w = PMCTRL_GetMainSrcSizeH(); 	src_h = GetIN_MainSrcSizeV();
	rect_w = src_w/6/32*32; 			rect_h = src_h/6/32*32; 		// make rect default size
	
	pCTRL->cam_zoom = 100; 												// pm camera zoom ratio : 100(x1.0) ~ 
	
	for(i=0;i<8;i++)	{
		pCTRL->rect[i].disp = ON; 										// pm rectangle display
		pCTRL->rect[i].color = i; 										// pm rectangle color
		pCTRL->rect[i].zoom = 100; 										// pm rectangle zoom ratio : 100(x1.0) ~ 
		
		pCTRL->rect[i].w = rect_w; 		pCTRL->rect[i].h = rect_h; 		// pm rectangle size
		
		rect_lx = src_w/4*(i%4)/32*32 + 64; 							// make rect default lx position
		rect_ly = src_h/2*(i/4)/32*32 + 160; 							// make rect default ly position
		pCTRL->rect[i].x = (rect_lx + rect_w/2) - src_w/2; 			// pm rectangle position(lx -> center.x)
		pCTRL->rect[i].y = (rect_ly + rect_h/2) - src_h/2; 			// pm rectangle position(ly -> center.y)
	}
	
	// pm polygon default parameter
	for(i=0;i<8;i++)	{
		pCTRL->poly[i].disp = ON; 										// pm polygon display
		pCTRL->poly[i].color = i+1; 									// pm polygon color
		
		memcpy(&pCTRL->poly[i].ulx, defPM_PolyPosi[i], sizeof(defPM_PolyPosi[i])); // load pm poly default position
		
		if(src_w >= 3840)	{ 											// for 4K input(poly x-posi * 2)
			pCTRL->poly[i].ulx *= 2; 	pCTRL->poly[i].urx *= 2;
			pCTRL->poly[i].dlx *= 2; 	pCTRL->poly[i].drx *= 2;
		}
		
		if(src_h >= 2160)	{ 											// for 4K input(poly y-posi * 2 )
			pCTRL->poly[i].uly *= 2; 	pCTRL->poly[i].ury *= 2;
			pCTRL->poly[i].dly *= 2; 	pCTRL->poly[i].dry *= 2;
		}
	}
	
	// pm register write
	for(i=0;i<8;i++)	{
		PMCTRL_SetRectSizePosi(i, pCTRL->rect[i].w, pCTRL->rect[i].h, pCTRL->rect[i].x, pCTRL->rect[i].y);
		PMCTRL_SetRectColor(i, pCTRL->rect[i].color);
		PMCTRL_SetRectOnOff(i, pCTRL->rect[i].disp);
		
		PMCTRL_SetPolyColor(i, pCTRL->poly[i].color);
		PMCTRL_SetPolyOnOff(i, pCTRL->poly[i].disp);
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD2__)	// 4k60 slave only
		memcpy(&stPOLY, &pCTRL->poly[i], sizeof(PM_POLY_INFO));
		stPOLY.ulx -= (stPOLY.ulx <= src_w/2)?  stPOLY.ulx:  (src_w/2);
		stPOLY.urx -= (stPOLY.urx <= src_w/2)?  stPOLY.urx:  (src_w/2);
		stPOLY.dlx -= (stPOLY.dlx <= src_w/2)?  stPOLY.dlx:  (src_w/2);
		stPOLY.drx -= (stPOLY.drx <= src_w/2)?  stPOLY.drx:  (src_w/2);
		PMCTRL_SetPolyMinMax(i, &stPOLY);
		PMCTRL_SetPolySlope(i, &stPOLY);
#else
		PMCTRL_SetPolyMinMax(i, &pCTRL->poly[i]);
		PMCTRL_SetPolySlope(i, &pCTRL->poly[i]);
#endif
	}
	
	PMCTRL_SetType((pCTRL->ctrl & PMCTRL_MODE_POLY)?  1:  0);
	PMCTRL_SetOnOff((pCTRL->ctrl & PMCTRL_ENABLE)?  ON:  OFF);
	
#if defined(__USE_MDIN_i500__)						// MDIN-i500
	MISP_RegWrite(MISP_LOCAL_ID, 0x07C1, 0x9753);	// gray1~4 level
	MISP_RegWrite(MISP_LOCAL_ID, 0x07C2, 0x00DB);	// gray5~6 level, mosaic size
#else												// MDIN-i51X (i510, i540, i550)
	MISP_RegWrite(MISP_LOCAL_ID, 0x0792, 0x9753);	// gray1~4 level
	MISP_RegWrite(MISP_LOCAL_ID, 0x0793, 0x00DB);	// gray5~6 level, mosaic size
#endif
}

//--------------------------------------------------------------------------------------------------
static void Init_PM_PARAM(PPM_CTRL_PARAM pCTRL)
{
	SetPM_Default(pCTRL);
	// load eeprom param..
}

//--------------------------------------------------------------------------------------------------
static void SetMD_Default(PMD_CTRL_PARAM pCTRL)
{
	BYTE i, mul_h, mul_v;
	
	// md default parameter
	pCTRL->ctrl = (MDCTRL_MD_OFF | MDCTRL_MOTIONVIEW_ON); 			// default : md(off), motion view(on)
	pCTRL->video_size_h = MDCTRL_GetMainSrcSizeH(); 				// set md video size
	pCTRL->video_size_v = GetIN_MainSrcSizeV();
	pCTRL->block_size = 32; 										// md block size(unit:pixel)
	pCTRL->thres = 20; 												// md threshold : 0~39
	
	switch(pCTRL->block_size)	{
		case  16:	mul_h = 4; 		mul_v = 4; 		break; 		// 16x16 block
		case  32:	mul_h = 2; 		mul_v = 2; 		break; 		// 32x32 block
		case  64:	mul_h = 1; 		mul_v = 1; 		break; 		// 64x64 block
		default:	mul_h = 1; 		mul_v = 1; 		break;
	}
	
	if(pCTRL->video_size_h >= 3840) 		mul_h *= 2; 			// 8M
	if(pCTRL->video_size_v >= 2160) 		mul_v *= 2; 			// 8M
	
	for(i=0;i<4;i++)	{ 											// position/size auto calc.
		pCTRL->area[i].en = TRUE; 								pCTRL->area[i].disp = TRUE;
		pCTRL->area[i].w = (9 * mul_h); 						pCTRL->area[i].h = (5 * mul_v);
		pCTRL->area[i].lx = (i%2)?  (17*mul_h):  (4*mul_h); 	pCTRL->area[i].ly = (i>1)?  (9*mul_v):  (2*mul_v);
	}
	
	
	// md register write
	for(i=0;i<4;i++)	{
		MDCTRL_SetSizePosi(i, pCTRL->area[i].w, pCTRL->area[i].h, pCTRL->area[i].lx, pCTRL->area[i].ly);
		MDCTRL_SetAreaEn(i, pCTRL->area[i].en);
		MDCTRL_SetAreaDisp(i, pCTRL->area[i].disp);
	}
	
	MDCTRL_SetOnOff((pCTRL->ctrl & MDCTRL_MD_ON)?  ON:  OFF); 					// md on/off
	MDCTRL_SetMotionView((pCTRL->ctrl & MDCTRL_MOTIONVIEW_ON)?  ON:  OFF); 	// md motion view on/off
	MDCTRL_SetBlockSize(pCTRL->block_size); 									// md block size(16,32,64)
	MDCTRL_SetThreshold(pCTRL->thres); 											// md threshold
	MDCTRL_SetVideoSize(pCTRL->video_size_h, pCTRL->video_size_v); 				// md video size
#if defined(__USE_4K60_INOUT__)
	MDCTRL_SetVideoSize(pCTRL->video_size_h/2, pCTRL->video_size_v); 			// md video size
#endif
}

//--------------------------------------------------------------------------------------------------
static void Init_MD_PARAM(PMD_CTRL_PARAM pCTRL)
{
	SetMD_Default(pCTRL);
	// load eeprom param..
}


// pm driver
//--------------------------------------------------------------------------------------------------
static void PMCTRL_SetType(BYTE type)
{
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	MISP_RegField(MISP_LOCAL_ID, 0x076F, 12, 1, (type)? 1: 0);		
#else 								// MDIN-i51X (i510, i540, i550)
	MISP_RegField(MISP_LOCAL_ID, 0x076C,  1, 1, (type)? 1: 0);
#endif
}

//--------------------------------------------------------------------------------------------------
static void PMCTRL_SetOnOff(BYTE OnOff)
{
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	MISP_RegField(MISP_LOCAL_ID, 0x076F, 13, 1, (OnOff)?  ON:  OFF);
#else 								// MDIN-i51X (i510, i540, i550)
	MISP_RegField(MISP_LOCAL_ID, 0x076C,  0, 1, (OnOff)?  ON:  OFF);
#endif
}

//--------------------------------------------------------------------------------------------------
static void PMCTRL_SetRectSizePosi(BYTE no, WORD w, WORD h, SHORT x, SHORT y)
{
	SHORT  lx, rx, ly, ry, in_size_h, in_size_v, half_w, half_h;
	
	half_w = w/2;								half_h = h/2;
	in_size_h = PMCTRL_GetMainSrcSizeH();		in_size_v = GetIN_MainSrcSizeV();
	
	// calc pm size/position(lx,rx,ly,ry)
	if(x <= -in_size_h/2 + half_w)				lx = 0;							// left limit
	else										lx = MIN(x - half_w + in_size_h/2, in_size_h-1);
	
	if(y <= -in_size_v/2 + half_h)				ly = 0;							// top limit
	else										ly = MIN(y - half_h + in_size_v/2, in_size_v-1);
	
	if(x >= in_size_h/2 -half_w)				rx = in_size_h-1;				// right limit
	else										rx = MAX(x + half_w + in_size_h/2, 0);
	
	if(y >= in_size_v/2 -half_h)				ry = in_size_v-1;				// bot limit
	else										ry = MAX(y + half_h + in_size_v/2, 0);
	
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD2__)	// 4k60 slave only
	lx -= (lx <= in_size_h/2)?  lx:  (in_size_h/2);
	rx -= (rx <= in_size_h/2)?  rx:  (in_size_h/2);
#endif
	
	if(DIFF(lx, rx) == 0)	{ly=0;	ry=0;}		// mask h-size zero ?  v-start/end position set to zero.
	if(DIFF(ly, ry) == 0)	{lx=0;	rx=0;}		// mask v-size zero ?  h-start/end position set to zero.
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	MISP_RegField(MISP_LOCAL_ID, 0x0770+(no*4), 0, 11, lx);	// 770, 774,, 78C
	MISP_RegField(MISP_LOCAL_ID, 0x0771+(no*4), 0, 11, rx);
	MISP_RegField(MISP_LOCAL_ID, 0x0772+(no*4), 0, 11, ly);
	MISP_RegField(MISP_LOCAL_ID, 0x0773+(no*4), 0, 11, ry);
#else 								// MDIN-i51X (i510, i540, i550)
	MISP_RegField(MISP_LOCAL_ID, 0x0770+(no*4), 0, 13, lx);
	MISP_RegField(MISP_LOCAL_ID, 0x0771+(no*4), 0, 13, rx);
	MISP_RegField(MISP_LOCAL_ID, 0x0772+(no*4), 0, 13, ly);
	MISP_RegField(MISP_LOCAL_ID, 0x0773+(no*4), 0, 13, ry);
#endif
//	UARTprintf("[%d] : [%d*%d, %d.%d] -> [%d.%d ~ %d.%d], org[%d.%d]\r\n", no, w, h, x, y, lx, ly, rx, ry, stPMCTRL.cam_pan, stPMCTRL.cam_tilt);	//test
}

//--------------------------------------------------------------------------------------------------
static void PMCTRL_SetRectColor(BYTE no, BYTE color)
{
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	MISP_RegField(MISP_LOCAL_ID, 0x0770+(no*4), 12, 4, color); 	// [770]rect_0, [774]rect_1,,[78C]rect_7
#else 								// MDIN-i51X (i510, i540, i550)
	if(no > 3) 	MISP_RegField(MISP_LOCAL_ID, 0x076E, (no-4)*4, 4, color); 	// rect 4~7
	else 			MISP_RegField(MISP_LOCAL_ID, 0x076D, no*4, 4, color); 		// rect 0~3
#endif
}

//--------------------------------------------------------------------------------------------------
static void PMCTRL_SetRectOnOff(BYTE no, BYTE OnOff)
{
	MISP_RegField(MISP_LOCAL_ID, 0x076F, MIN(no, 15), 1, (OnOff)?  ON:  OFF);
}

//--------------------------------------------------------------------------------------------------
static void PMCTRL_SetPolyColor(BYTE no, BYTE color)
{
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	MISP_RegField(MISP_LOCAL_ID, 0x07C0, no*4, 4, color); 	// [3:0]poly_0, [7:4]poly_1, [11:8]poly_2, [15:12]poly_3
#else 								// MDIN-i51X (i510, i540, i550)
	if(no > 3) 	MISP_RegField(MISP_LOCAL_ID, 0x0791, (no-4)*4, 4, color); 	// poly 4~7
	else 			MISP_RegField(MISP_LOCAL_ID, 0x0790, no*4, 4, color); 		// poly 0~3
#endif
}

//--------------------------------------------------------------------------------------------------
static void PMCTRL_SetPolyOnOff(BYTE no, BYTE OnOff)
{
	MISP_RegField(MISP_LOCAL_ID, 0x076F, no+8, 1, (OnOff)?  ON:  OFF);
}

//--------------------------------------------------------------------------------------------
static void PMCTRL_SetPolyMinMax(BYTE no, PPM_POLY_INFO pINFO)
{
	WORD x_min, x_max, y_min, y_max;
	
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	if(no > 3) 			return; 				// i500(poly:0~3), i51x(poly:0~7)
#endif
	
	x_min = MIN(MIN(pINFO->ulx, pINFO->urx), MIN(pINFO->dlx, pINFO->drx));
	x_max = MAX(MAX(pINFO->ulx, pINFO->urx), MAX(pINFO->dlx, pINFO->drx));
	
	y_min = MIN(MIN(pINFO->uly, pINFO->ury), MIN(pINFO->dly, pINFO->dry));
	y_max = MAX(MAX(pINFO->uly, pINFO->ury), MAX(pINFO->dly, pINFO->dry));
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		MISP_RegWrite(MISP_LOCAL_ID, 0x07C3+(no*4), x_min);
		MISP_RegWrite(MISP_LOCAL_ID, 0x07C4+(no*4), x_max);
		MISP_RegWrite(MISP_LOCAL_ID, 0x07C5+(no*4), y_min);
		MISP_RegWrite(MISP_LOCAL_ID, 0x07C6+(no*4), y_max);
#else 								// MDIN-i51X (i510, i540, i550)
		MISP_RegWrite(MISP_LOCAL_ID, 0x0794+(no*4), x_min);
		MISP_RegWrite(MISP_LOCAL_ID, 0x0795+(no*4), x_max);
		MISP_RegWrite(MISP_LOCAL_ID, 0x0796+(no*4), y_min);
		MISP_RegWrite(MISP_LOCAL_ID, 0x0797+(no*4), y_max);
#endif
}

//--------------------------------------------------------------------------------------------
static void PMCTRL_SetPolySlope(BYTE no, PPM_POLY_INFO pINFO)
{
	BYTE i,j;
	FLOAT slope_1, slope_2, slope_3=0, slope_4=0, slope_L, slope_R, slope_U=0, slope_D=0;
	FLOAT slope_min=1/127, slope_max=127;
	SHORT r_slope_L, r_slope_R, r_slope_U, r_slope_D;
	LONG jeol_L=0, jeol_R=0, jeol_U=0, jeol_D=0;
	PM_POLY_POSI arr[4], temp;
	
	
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	if(no > 3) 			return; 				// i500(poly:0~3), i51X(poly:0~7)
#endif
	
	// copy polygon position data
	arr[0].x = pINFO->ulx; 			arr[0].y = pINFO->uly;
	arr[1].x = pINFO->urx; 			arr[1].y = pINFO->ury;
	arr[2].x = pINFO->dlx; 			arr[2].y = pINFO->dly;
	arr[3].x = pINFO->drx; 			arr[3].y = pINFO->dry;
	
	// sort y-position(low to high)
	for(i=0; i<3; i++)	{
		for(j=i; j<4; j++)	{
			if(arr[i].y > arr[j].y) 	{temp = arr[i];  arr[i] = arr[j];  arr[j] = temp;} 	// sort low to high
		}
	}
	
	// when (n)th Y and (n+1)th Y are same, compare X value
	for(i=0; i<3; i++)	{
		if(arr[i].y == arr[i+1].y)	{
			if(arr[i].x > arr[i+1].x)	{temp = arr[i];  arr[i] = arr[i+1];  arr[i+1] = temp;} 	// sort low to high
		}
	}
	
	// check error case
	if(arr[0].x == arr[1].x && arr[1].y == arr[2].y)	{ 			// ulx == urx, ury == dly
		if(arr[3].x < arr[1].x) 		arr[3].x = arr[1].x + 1; 	// drx < urx --> drx = urx + 1
		else if(arr[3].x > arr[2].x) 	arr[3].x = arr[1].x - 1; 	// drx > dlx --> drx = urx - 1 (?)
	}
	
	// find 4 side
	if(arr[0].y == arr[1].y) 				slope_1 = 128; 				// 90 degree(uly = ury)
	else	{
		slope_1 = ((float)arr[1].x -(float)arr[0].x) / ((float)arr[1].y -(float)arr[0].y); 	// d.ux / d.uy
		
		if(ABS(slope_1) < slope_min) 		slope_1 = (slope_1>0)?  slope_min:  -slope_min;
		if(ABS(slope_1) > slope_max) 		slope_1 = (slope_1>0)?  slope_max:  -slope_max;
	}
	
	if(arr[2].y == arr[3].y) 				slope_2 = 128; 				// 90 degree(dly = dry)
	else	{
		slope_2 = ((float)arr[3].x -(float)arr[2].x) / ((float)arr[3].y -(float)arr[2].y); 	// d.dx / d.dy
		
		if(ABS(slope_2) < slope_min)		slope_2 = (slope_2>0)?  slope_min:  -slope_min;
		if(ABS(slope_2) > slope_max)		slope_2 = (slope_2>0)?  slope_max:  -slope_max;
	}
	
	if(arr[0].x == arr[1].x)	{ 										// slope_1 is '0'(ulx == urx)
		if((arr[0].x > arr[2].x) && (arr[0].x > arr[3].x))	{ 			// ulx > dlx, ulx > drx
			slope_3 = ((float)arr[2].x -(float)arr[0].x) / ((float)arr[2].y -(float)arr[0].y); 	// d.lx / d.ly
			slope_4 = ((float)arr[3].x -(float)arr[1].x) / ((float)arr[3].y -(float)arr[1].y); 	// d.rx / d.ry
		}
		else if((arr[0].x < arr[2].x) && (arr[0].x < arr[3].x))	{ 	// ulx < dlx, ulx < drx
			slope_3 = ((float)arr[3].x -(float)arr[0].x) / ((float)arr[3].y -(float)arr[0].y); 	// d.rlx / d.rly
			slope_4 = ((float)arr[2].x -(float)arr[1].x) / ((float)arr[2].y -(float)arr[1].y); 	// d.lrx / d.lry
		}
	}
	else	{
		if(arr[0].x > arr[1].x) 	{temp = arr[0];  arr[0] = arr[1];  arr[1] = temp;} 		// ulx > urx
		if(arr[2].x > arr[3].x) 	{temp = arr[2];  arr[2] = arr[3];  arr[3] = temp;} 		// dlx > drx
		
		slope_3 = ((float)arr[2].x -(float)arr[0].x) / ((float)arr[2].y -(float)arr[0].y); 	// d.lx / d.ly
		slope_4 = ((float)arr[3].x -(float)arr[1].x) / ((float)arr[3].y -(float)arr[1].y); 	// d.rx / d.ry
	}
	
	if(ABS(slope_3) < slope_min) 		slope_3 = (slope_3>0)?  slope_min:  -slope_min;
	if(ABS(slope_3) > slope_max) 		slope_3 = (slope_3>0)?  slope_max:  -slope_max;
	
	if(ABS(slope_4) < slope_min) 		slope_4 = (slope_4>0)?  slope_min:  -slope_min;
	if(ABS(slope_4) > slope_max) 		slope_4 = (slope_4>0)?  slope_max:  -slope_max;
	
	// find L, R, U, D side
	if(slope_1 == 0) 	{	// if slope_1 is '0', slope_0 is not slope_L.
		slope_L = slope_3; 			slope_R = slope_4;
		
		if(slope_3 > 0 && slope_4 > 0) 	{
			slope_U = slope_2; 		slope_D = slope_1;
			
			// jeolpyun calculate
			jeol_L = ((arr[0].x - (slope_L * arr[0].y)) * 256);
			jeol_R = ((arr[1].x - (slope_R * arr[1].y)) * 256);
			jeol_U = ((arr[2].x - (slope_U * arr[2].y)) * 256);
			jeol_D = ((arr[0].x - (slope_D * arr[0].y)) * 256);
		}
		else if(slope_3 < 0 && slope_4 < 0) 	{
			slope_U = slope_1; 		slope_D = slope_2;
			
			// jeolpyun calculate
			jeol_L = ((arr[0].x - (slope_L * arr[0].y)) * 256);
			jeol_R = ((arr[1].x - (slope_R * arr[1].y)) * 256);
			jeol_U = ((arr[1].x - (slope_U * arr[1].y)) * 256);
			jeol_D = ((arr[2].x - (slope_D * arr[2].y)) * 256);
		}
	}
	else 	{
		slope_L = slope_1;
		slope_R = slope_2;
		slope_U = slope_4;
		slope_D = slope_3;
		
		// jeolpyun calculate
		if(slope_L == 128) 	jeol_L = arr[0].y * 256;
		else 					jeol_L = (arr[0].x - (slope_L * arr[0].y)) * 256;
		
		if(slope_R == 128) 	jeol_R = arr[2].y * 256;
		else 					jeol_R = (arr[2].x - (slope_R * arr[2].y)) * 256;
		
		jeol_U = (arr[1].x - (slope_U * arr[1].y)) * 256;
		jeol_D = (arr[0].x - (slope_D * arr[0].y)) * 256;
	}
	
	r_slope_L = (slope_L==128)?  0x7FFF:  slope_L*256;
	r_slope_R = (slope_R==128)?  0x7FFF:  slope_R*256;
	r_slope_U = slope_U*256;
	r_slope_D = slope_D*256;
	
	// for register write
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	MISP_RegWrite(MISP_LOCAL_ID, 0x0790+ no*12, r_slope_L&0xffff);
	MISP_RegWrite(MISP_LOCAL_ID, 0x0791+ no*12, jeol_L&0xffff); 		//LSB
	MISP_RegWrite(MISP_LOCAL_ID, 0x0792+ no*12, (jeol_L>>16)&0x7ff); //MSB
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x0793+ no*12, r_slope_R&0xffff);
	MISP_RegWrite(MISP_LOCAL_ID, 0x0794+ no*12, jeol_R&0xffff); 		//LSB
	MISP_RegWrite(MISP_LOCAL_ID, 0x0795+ no*12, (jeol_R>>16)&0x7ff); //MSB
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x0796+ no*12, r_slope_U&0xffff);
	MISP_RegWrite(MISP_LOCAL_ID, 0x0797+ no*12, jeol_U&0xffff); 		//LSB
	MISP_RegWrite(MISP_LOCAL_ID, 0x0798+ no*12, (jeol_U>>16)&0x7ff); //MSB
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x0799+ no*12, r_slope_D&0xffff);
	MISP_RegWrite(MISP_LOCAL_ID, 0x079A+ no*12, jeol_D&0xffff); 		//LSB
	MISP_RegWrite(MISP_LOCAL_ID, 0x079B+ no*12, (jeol_D>>16)&0x7ff); //MSB
#else 								// MDIN-i51X (i510, i540, i550)
	MISP_RegWrite(MISP_LOCAL_ID, 0x07BC+ no*3, 0x8000|0x00); 			// indirect reg. write enable, start addr(0x00)
	MISP_RegWrite(MISP_LOCAL_ID, 0x07BD+ no*3, r_slope_L); 				// slope_L
	MISP_RegWrite(MISP_LOCAL_ID, 0x07BD+ no*3, LOWORD(jeol_L)); 		// jeol_L(LSB) - 16bit
	MISP_RegWrite(MISP_LOCAL_ID, 0x07BD+ no*3, HIWORD(jeol_L)&0x1FFF); 	// jeol_L(MSB) - 13bit
	MISP_RegWrite(MISP_LOCAL_ID, 0x07BD+ no*3, r_slope_R); 				// slope_R
	MISP_RegWrite(MISP_LOCAL_ID, 0x07BD+ no*3, LOWORD(jeol_R)); 		// jeol_R(LSB)
	MISP_RegWrite(MISP_LOCAL_ID, 0x07BD+ no*3, HIWORD(jeol_R)&0x1FFF); 	// jeol_R(MSB)
	MISP_RegWrite(MISP_LOCAL_ID, 0x07BD+ no*3, r_slope_U); 				// slope_U
	MISP_RegWrite(MISP_LOCAL_ID, 0x07BD+ no*3, LOWORD(jeol_U)); 		// jeol_U(LSB)
	MISP_RegWrite(MISP_LOCAL_ID, 0x07BD+ no*3, HIWORD(jeol_U)&0x1FFF); 	// jeol_U(MSB)
	MISP_RegWrite(MISP_LOCAL_ID, 0x07BD+ no*3, r_slope_D); 				// slope_D
	MISP_RegWrite(MISP_LOCAL_ID, 0x07BD+ no*3, LOWORD(jeol_D)); 		// jeol_D(LSB)
	MISP_RegWrite(MISP_LOCAL_ID, 0x07BD+ no*3, HIWORD(jeol_D)&0x1FFF); 	// jeol_D(MSB)
	MISP_RegWrite(MISP_LOCAL_ID, 0x07BC+ no*3, 0x0000|0x00); 			// indirect reg. write disable, start addr(0x00)
#endif


#if __MISP100_DBGPRT__ == 1
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	UARTprintf("\r\n[PM] : arr, ");
	for(i=0; i<4; i++) 	UARTprintf("[%d](%d,%d), ", i, arr[i].x, arr[i].y);
#else 								// MDIN-i51X (i510, i540, i550)
	UARTprintf("\r\n[PM] : arr, ");
	for(i=0; i<4; i++) 	UARTprintf("[%d](%d,%d), ", i, arr[i].x, arr[i].y);
#endif
	UARTprintf("\r\n");
	UARTprintf("[PM] : slope, L(%f), R(%f), U(%f), D(%f) \r\n", slope_L, slope_R, slope_U, slope_D);
	UARTprintf("[PM] : r_slope, L(0x%x), R(0x%x), U(0x%x), D(0x%x) \r\n", r_slope_L, r_slope_R, r_slope_U, r_slope_D);
	UARTprintf("[PM] : jeol, L(0x%x), R(0x%x), U(0x%x), D(0x%x) \r\n", jeol_L, jeol_R, jeol_U, jeol_D);
#endif
}


// md driver
//--------------------------------------------------------------------------------------------------
static void MDCTRL_SetSizePosi(BYTE no, BYTE w, BYTE h, BYTE lx, BYTE ly)
{
#if defined(__USE_MD_SBOX_OSD__)
	WORD sbox_rx, sbox_ry, sbox_lx, sbox_ly, x, y;
	WORD h_ratio, v_ratio, dz_ratio, dz_win_x, dz_win_y;
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD2__)	// 4k60 slave only
	WORD w_off, x_off;
#endif
	
	// check mirror
	x = lx * stMDCTRL.block_size;
	y = ly * stMDCTRL.block_size;
	if(GetFC_MainMirror()&1)	x = MDCTRL_GetMainSrcSizeH() - (lx + w)*stMDCTRL.block_size;
	if(GetFC_MainMirror()&2)	y = GetIN_MainSrcSizeV() - (ly + h)*stMDCTRL.block_size;
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD2__)	// 4k60 slave only
	w_off = (x < stMDCTRL.video_size_h/2)?  (stMDCTRL.video_size_h/2 - x)/stMDCTRL.block_size:  0;
	x_off = (x < stMDCTRL.video_size_h/2)?  x:  stMDCTRL.video_size_h/2;
	w -= (w < w_off)?  w:  w_off;
	x -= x_off;
	lx -= x_off / stMDCTRL.block_size;
	
	MDCTRL_SetAreaDisp(no, stMDCTRL.area[no].disp);		// sbox on/off by position
#endif
	
	// main sbox
	h_ratio = GetOUT_MainSizeHA() * 1000 / GetIN_MainSrcSizeH();
	v_ratio = GetOUT_MainSizeVA() * 1000 / GetIN_MainSrcSizeV();
	
	sbox_lx = x;
	sbox_ly = y;
	sbox_rx = x + (w * stMDCTRL.block_size);
	sbox_ry = y + (h * stMDCTRL.block_size);
	
	if(GetDZOOM_OnOff())	{
		dz_ratio = ConvDZOOM_MenuRatio(GetDZOOM_Ratio()) * 10;		// 100:x1.00 ~ 1200:x12.0
		dz_win_x = GetDZOOM_WinPosiX() * 1000 / ((h_ratio<1000)?  h_ratio:  1000);	// adjust dz src position when ffc 'on'
		dz_win_y = GetDZOOM_WinPosiY();
		
		sbox_lx -= (sbox_lx > dz_win_x)?  dz_win_x:  sbox_lx;		// adjust x,y : full scr -> zoom window
		sbox_ly -= (sbox_ly > dz_win_y)?  dz_win_y:  sbox_ly;
		sbox_rx -= (sbox_rx > dz_win_x)?  dz_win_x:  sbox_rx;
		sbox_ry -= (sbox_ry > dz_win_y)?  dz_win_y:  sbox_ry;
		
		h_ratio = h_ratio * dz_ratio / 1000;
		v_ratio = v_ratio * dz_ratio / 1000;
	}
	
	sbox_lx = (sbox_lx * h_ratio) / 1000;
	sbox_ly = (sbox_ly * v_ratio) / 1000;
	sbox_rx = (sbox_rx * h_ratio) / 1000;
	sbox_ry = (sbox_ry * v_ratio) / 1000;
	
	if(!GetOUT_MainScanTypeProg())			{sbox_ly /=2;	sbox_ry /=2;}	//for interlace out
	
	if((DIFF(sbox_lx,sbox_rx)==0) || (DIFF(sbox_ly,sbox_ry)==0))	{		// skip 1-line box
		sbox_lx=0;	sbox_rx=0;	sbox_ly=0;	sbox_ry=0;
	}
	
	if(sbox_lx > GetOUT_MainSizeHA())		sbox_lx = GetOUT_MainSizeHA();
	if(sbox_rx > GetOUT_MainSizeHA())		sbox_rx = GetOUT_MainSizeHA();
	if(sbox_ly > GetOUT_MainSizeVA())		sbox_ly = GetOUT_MainSizeVA();
	if(sbox_ry > GetOUT_MainSizeVA())		sbox_ry = GetOUT_MainSizeVA();
	
	MISPOSD_SetSBoxArea(&stSBOX[SBOX_INDEX4+no], sbox_lx, sbox_ly, sbox_rx, sbox_ry);	// main
	
	
	// aux sbox
	h_ratio = GetOUT_AuxSizeHA() * 1000 / GetIN_AuxSrcSizeH();
	v_ratio = GetOUT_AuxSizeVA() * 1000 / GetIN_AuxSrcSizeV();
	
	sbox_lx = (sbox_lx * h_ratio) / 1000;
	sbox_ly = (sbox_ly * v_ratio) / 1000;
	sbox_rx = (sbox_rx * h_ratio) / 1000;
	sbox_ry = (sbox_ry * v_ratio) / 1000;
	
	if(GetOUT_MainScanTypeProg())			{sbox_ly /=2;	sbox_ry /=2;}	// main(p) -> aux(i)
	
	MISPOSD_SetSBoxArea(&stSBOX[SBOX_INDEX8+no], sbox_lx, sbox_ly, sbox_rx, sbox_ry);	// aux
#endif
	
	MISP_RegField(MISP_LOCAL_ID, 0x0742 + no*2, 8, 8, lx); 				// md window start-x
	MISP_RegField(MISP_LOCAL_ID, 0x0743 + no*2, 8, 7, ly); 				// md window start-y
	MISP_RegField(MISP_LOCAL_ID, 0x0742 + no*2, 0, 8, lx+w); 				// md window end-x
	MISP_RegField(MISP_LOCAL_ID, 0x0743 + no*2, 0, 7, ly+h); 				// md window end-y
}

//--------------------------------------------------------------------------------------------------
static void MDCTRL_SetAreaEn(BYTE no, WORD OnOff)
{
#if defined(__USE_MD_SBOX_OSD__)
	MDCTRL_SetAreaDisp(no, (stMDCTRL.area[no].disp)?  OnOff:  OFF);			// check md area display on/off
#endif
	
	MISP_RegField(MISP_LOCAL_ID, 0x0740, 2 + no*2, 1, (OnOff)?  ON:  OFF); 	// md area enable/disable
}

//--------------------------------------------------------------------------------------------------
static void MDCTRL_SetAreaDisp(BYTE no, WORD OnOff)
{
#if defined(__USE_MD_SBOX_OSD__)
	if((stMDCTRL.ctrl & MDCTRL_MD_ON) != MDCTRL_MD_ON)		OnOff = OFF;	// check md on/off
	if(stMDCTRL.area[no].en == OFF)							OnOff = OFF;	// check md area enable on/off
	
#if defined(__USE_4K60_INOUT__)
#if defined(__USE_4K60_MD2__)	// 4k60 slave only
	if((stMDCTRL.area[no].lx + stMDCTRL.area[no].w) <= stMDCTRL.video_size_h/2/stMDCTRL.block_size)
		OnOff = OFF;			// hide left side sbox in slave
#endif
	MISPOSD_SetSBoxAlpha(&stSBOX[SBOX_INDEX4+no], 8);	//alpha
	MISPOSD_SetSBoxPlaneColor(&stSBOX[SBOX_INDEX4+no], RGB(128,235,128));
	MISPOSD_SetSBoxPlaneColor(&stSBOX[SBOX_INDEX8+no], RGB(128,235,128));
	MISPOSD_EnableSBoxPlane(&stSBOX[SBOX_INDEX4+no], OnOff);
	MISPOSD_EnableSBoxPlane(&stSBOX[SBOX_INDEX8+no], OnOff);
#endif
	
	MISPOSD_SetSBoxBorderColor(&stSBOX[SBOX_INDEX4+no], RGB(128,235,128));	// white
	MISPOSD_SetSBoxBorderColor(&stSBOX[SBOX_INDEX8+no], RGB(128,235,128));	// white
	MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX4+no], OnOff);
	MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX8+no], OnOff);
#else
	MISP_RegField(MISP_LOCAL_ID, 0x0740, 3 + no*2, 1, (OnOff)?  ON:  OFF); 	// md area window display on/off
#endif
}

//--------------------------------------------------------------------------------------------------
static void MDCTRL_SetOnOff(WORD OnOff)
{
#if defined(__USE_MD_SBOX_OSD__)
	BYTE i;
	
	for(i=0;i<4;i++)	{
		MDCTRL_SetSizePosi(i, stMDCTRL.area[i].w, stMDCTRL.area[i].h, stMDCTRL.area[i].lx, stMDCTRL.area[i].ly);
		MDCTRL_SetAreaDisp(i, stMDCTRL.area[i].disp);
	}
#endif
	
	MISP_RegField(MISP_LOCAL_ID, 0x0740, 0, 1, (OnOff)?  ON:  OFF); 			// md on/off
}

//--------------------------------------------------------------------------------------------------
static void MDCTRL_SetMotionView(WORD OnOff)
{
	MISP_RegField(MISP_LOCAL_ID, 0x0740, 1, 1, (OnOff)?  ON:  OFF);
}

//--------------------------------------------------------------------------------------------------
static void MDCTRL_SetBlockSize(WORD size)
{
	MISP_RegField(MISP_LOCAL_ID, 0x0741, 12, 2, (size/32)); 	// md block size : 0(16x16), 1(32x32), 2(64x64)
}

//--------------------------------------------------------------------------------------------------
static void MDCTRL_SetThreshold(WORD val)
{
	MISP_RegField(MISP_LOCAL_ID, 0x0741, 0, 10, val); 			// md threshold : 1(most sens) ~ 1023(most insens)
}

//--------------------------------------------------------------------------------------------------
static void MDCTRL_SetVideoSize(WORD md_h_size, WORD md_v_size)
{
	MISP_RegField(MISP_LOCAL_ID, 0x074D, 0, 13, md_h_size);
	MISP_RegField(MISP_LOCAL_ID, 0x074E, 0, 13, md_v_size);
}


#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
//--------------------------------------------------------------------------------------------------
static WORD PMCTRL_ConvZPosiToRatio(WORD zoom)
{
	WORD  i, unit=0, ratio, tenths_ratio;
	WORD  zposi_tbl[30] = {
// fcb-ev7500
		0x0000, 0x16A1, 0x2063, 0x2628, 0x2A1D, 0x2D13, 0x2F6D, 0x3161, 0x330D, 0x3486,	// x1 ~ x10
		0x35D7, 0x3709, 0x3820, 0x3920, 0x3A0A, 0x3ADD, 0x3B9C, 0x3C46, 0x3CDC, 0x3D60,	// x11 ~ x20
		0x3DD4, 0x3E39, 0x3E90, 0x3EDC, 0x3F1E, 0x3F57, 0x3F8A, 0x3FB6, 0x3FDC, 0x4000,	// x21 ~ x30
// tamron-30x
//		0x0000, 0x10B9, 0x1B5F, 0x224F, 0x271E, 0x2AB5, 0x2D7B, 0x2FC1, 0x31AB, 0x3352,	// x1 ~ x10(step:x1)
//		0x34CE, 0x361E, 0x3751, 0x386A, 0x396C, 0x3A5B, 0x3B2B, 0x3BE9, 0x3C94, 0x3D27,	// x11 ~ x20(step:x1)
//		0x3DA8, 0x3E16, 0x3E78, 0x3EC8, 0x3F11, 0x3F4E, 0x3F86, 0x3FB7, 0x3FE1, 0x4000,	// x21 ~ x30(step:x1)
	};
	
	zoom = MIN(zoom, 0x4000);
	
	for(i=0;i<30;i++)	{ 																	// calc zoom ratio
		if(zoom <= zposi_tbl[i])	{
			unit = DIFF(zposi_tbl[i], zposi_tbl[(i)? i-1: 0]); 	break; 				// calc zoom position gap
		}
	}
	
	tenths_ratio = ((zoom - zposi_tbl[(i)? i-1: 0]) * 10000 / unit);
	tenths_ratio = sqrt((tenths_ratio * tenths_ratio) / 10000 * tenths_ratio); 	// .ratio^(3/2) for small tolerance(mask vs zoom)
	ratio = (((i)*10000) + tenths_ratio) / 100;	// ratio : x1 ~ x30 --> 100 ~ 30000
	
	return  MIN(MAX(ratio, 100), 30000);
}
#endif

//--------------------------------------------------------------------------------------------------
static void PMCTRL_ZoomProcess(PPM_CTRL_PARAM pCTRL)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	if(pCTRL->af_zposi == LENS_GetVISCAPositionZOOM()) 	return; 	// check zoom 'position change..'
	pCTRL->af_zposi = LENS_GetVISCAPositionZOOM();
	
	stPMCTRL.cam_zoom = PMCTRL_ConvZPosiToRatio(pCTRL->af_zposi); 		// set camera direction zoom position
	
	pCTRL->mask_no |= (0xff);		pCTRL->ctrl |= PMCTRL_TRIGGER;		// enable all rect(0~7) mask process
#endif
}

//--------------------------------------------------------------------------------------------------
static void PMCTRL_MainProcess(PPM_CTRL_PARAM pCTRL)
{
	LONG posi_x, posi_y;
	DWORD width, height, i;
	
	if((pCTRL->ctrl & PMCTRL_TRIGGER)==0)					return;	// check trigger
	pCTRL->ctrl &= (~PMCTRL_TRIGGER);
	
	for(i=0;i<16;i++)	{
		if(pCTRL->mask_no & (1<<i))		{
			pCTRL->mask_no &= ~(1<<i);
			
			// set mask size/position with visca ptz
			width = pCTRL->rect[i].w*100 / pCTRL->rect[i].zoom;
			height = pCTRL->rect[i].h*100 / pCTRL->rect[i].zoom;
			
			if(pCTRL->noninterlock_mask & (1<<i))	{					// non-interlock mode
				posi_x = pCTRL->rect[i].x;
				posi_y = pCTRL->rect[i].y;
			}
			else	{													// interlock mode(pan_tilt, ptz)
				posi_x = pCTRL->rect[i].x - pCTRL->cam_pan;
				posi_y = pCTRL->rect[i].y - pCTRL->cam_tilt;
			}
			
			if(posi_x < -6141)		posi_x += 12288;					// move left max : -6141(-6141 ~ +6144)
			if(posi_x > 6144)		posi_x -= 12288;					// move right max : +6144(-6141 ~ +6144)
			if(posi_y < -3069)		posi_y += 6144;						// move up max : -3069(-3069 ~ +3072)
			if(posi_y > 3072)		posi_y -= 6144;						// move dn max : -3069(-3069 ~ +3072)
			
			
			// set mask size/position with af zoom
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
			width = width * pCTRL->cam_zoom / 100;
			height = height * pCTRL->cam_zoom / 100;
			posi_x = posi_x * pCTRL->cam_zoom / 100;
			posi_y = posi_y * pCTRL->cam_zoom / 100;
			
			if(posi_x < -32768)  	posi_x = -32768;
			if(posi_x > 32767) 	posi_x = 32767;
			if(posi_y < -32768)  	posi_y = -32768;
			if(posi_y > 32767) 	posi_y = 32767;
#endif
			
			PMCTRL_SetRectSizePosi(i, (WORD)width, (WORD)height, (SHORT)posi_x, (SHORT)posi_y);
		}
	}
}


//api
//----------------------------------------------------------------------------------------
void Init_MDPM(void)
{
	memset(&stPMCTRL, 0, sizeof(PM_CTRL_PARAM));
	memset(&stMDCTRL, 0, sizeof(MD_CTRL_PARAM));
	
	Init_PM_PARAM(&stPMCTRL);
	Init_MD_PARAM(&stMDCTRL);
}

//--------------------------------------------------------------------------------------------------
void PM_ProcessHandler(void)
{
	PMCTRL_ZoomProcess(&stPMCTRL); 					// check af zoom position & set mask zoom position
	PMCTRL_MainProcess(&stPMCTRL); 					// pm mask main process
}


// pm api
//--------------------------------------------------------------------------------------------------
void SetPM_OnOff(BYTE OnOff)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	if(OnOff) 		stPMCTRL.ctrl |= PMCTRL_ENABLE; 		// pm enable
	else 			stPMCTRL.ctrl &= ~(PMCTRL_ENABLE); 		// pm disable
	
	PMCTRL_SetOnOff(OnOff);
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlavePMOnOff((OnOff)?  ON:  OFF);
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)OnOff;
	EEPROM_MultiWrite(EEPROM_PM_MODE, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE GetPM_OnOff(void)
{
	if(stPMCTRL.ctrl & PMCTRL_ENABLE) 		return ON;
	else 									return OFF;
}

//--------------------------------------------------------------------------------------------------
void SetPM_Type(BYTE type) 								// 0:rect, 1:poly
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	if(type) 		stPMCTRL.ctrl |= PMCTRL_MODE_POLY; 		// pm type(polygon)
	else 			stPMCTRL.ctrl &= ~(PMCTRL_MODE_POLY); 	// pm type(rectangle)
	
	PMCTRL_SetType(type);
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlavePMType(type);
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)type;
	EEPROM_MultiWrite(EEPROM_PM_TYPE, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE GetPM_Type(void)
{
	if(stPMCTRL.ctrl & PMCTRL_MODE_POLY) 		return 1; 	// polygon
	else 										return 0; 	// rectangle
}

//--------------------------------------------------------------------------------------------------
void SetPM_RectOnOff(BYTE no, BYTE OnOff)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2], i;
#endif
	
	if(no > 7) 			return; 					// no:0~7(rect), 8~15(poly)
	
	stPMCTRL.rect[no].disp = OnOff;
	
	PMCTRL_SetRectOnOff(no, OnOff);
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlavePMRectOnOff(no, (OnOff)?  ON:  OFF);
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = 0x00;
	for(i=0;i<8;i++) 		buff[1] |= (stPMCTRL.rect[i].disp)?  (1<<i):  0;
	
	EEPROM_MultiWrite(EEPROM_PM_RECTDISP, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE GetPM_RectOnOff(BYTE no)
{
	if(no > 7) 			return 0; 					// no:0~7(rect), 8~15(poly)
	
	return 	stPMCTRL.rect[no].disp;
}

//--------------------------------------------------------------------------------------------------
void SetPM_RectColor(BYTE no, BYTE color)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[5];
#endif
	
	if(no > 7) 			return; 					// no:0~7(rect), 8~15(poly)
	
	stPMCTRL.rect[no].color = color;
	
	PMCTRL_SetRectColor(no, color);
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlavePMRectColor(no, color);
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = MAKEBYTE(stPMCTRL.rect[1].color,  stPMCTRL.rect[0].color);
	buff[2] = MAKEBYTE(stPMCTRL.rect[3].color,  stPMCTRL.rect[2].color);
	buff[3] = MAKEBYTE(stPMCTRL.rect[5].color,  stPMCTRL.rect[4].color);
	buff[4] = MAKEBYTE(stPMCTRL.rect[7].color,  stPMCTRL.rect[6].color);
	
	EEPROM_MultiWrite(EEPROM_PM_RECTCOLOR, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE GetPM_RectColor(BYTE no)
{
	if(no > 7) 			return 0; 					// no:0~7(rect), 8~15(poly)
	
	return 	stPMCTRL.rect[no].color;
}

//--------------------------------------------------------------------------------------------------
void SetPM_RectSizeNew(BYTE no, WORD width, WORD height)
{
	if(no > 7)				return;					// no:0~7(rect), 8~15(poly)
	
	stPMCTRL.rect[no].w = width;
	stPMCTRL.rect[no].h = height;
	stPMCTRL.rect[no].x = stPMCTRL.cam_pan;				// set position to center with cam offset x,y
	stPMCTRL.rect[no].y = stPMCTRL.cam_tilt;
	
	stPMCTRL.rect[no].zoom = 100;						// reset ptz(zoom)
	
	stPMCTRL.noninterlock_mask &= ~(1 << no);			// clear non-interlock mask flag
	
	stPMCTRL.mask_no |= (1 << no);		stPMCTRL.ctrl |= PMCTRL_TRIGGER;		// write pm reg. on v-blank time
}

//--------------------------------------------------------------------------------------------------
void SetPM_RectSizeModify(BYTE no, WORD width, WORD height)
{
	if(no > 7)				return;					// no:0~7(rect), 8~15(poly)
	
	stPMCTRL.rect[no].w = width;
	stPMCTRL.rect[no].h = height;
	
	// check non-interlock mode
	if(stPMCTRL.noninterlock_mask & (1 << no))	{		// reset ptx & center position when non-interlock mode
		stPMCTRL.rect[no].x = stPMCTRL.cam_pan;			// set position to center with cam offset x,y
		stPMCTRL.rect[no].y = stPMCTRL.cam_tilt;
		
		stPMCTRL.rect[no].zoom = 100;					// reset ptz(zoom)
	}
	
	stPMCTRL.noninterlock_mask &= ~(1 << no);			// clear non-interlock mask flag
	
	stPMCTRL.mask_no |= (1 << no);		stPMCTRL.ctrl |= PMCTRL_TRIGGER;		// write pm reg. on v-blank time
}

//--------------------------------------------------------------------------------------------------
void SetPM_RectSizeDirect(BYTE no, WORD width, WORD height)
{
	if(no > 7)				return;					// no:0~7(rect), 8~15(poly)
	
	stPMCTRL.rect[no].w = width;
	stPMCTRL.rect[no].h = height;
	
	stPMCTRL.mask_no |= (1 << no);		stPMCTRL.ctrl |= PMCTRL_TRIGGER;		// write pm reg. on v-blank time
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlavePMRectSize(no, width, height);
#endif
}

//--------------------------------------------------------------------------------------------------
void SetPM_RectPosiDirect(BYTE no, WORD lx, WORD ly)
{
	if(no > 7)				return;					// no:0~7(rect), 8~15(poly)
	
	stPMCTRL.rect[no].x = (lx - PMCTRL_GetMainSrcSizeH()/2) + stPMCTRL.rect[no].w/2;
	stPMCTRL.rect[no].y = (ly - GetIN_MainSrcSizeV()/2) + stPMCTRL.rect[no].h/2;
	
	stPMCTRL.mask_no |= (1 << no);		stPMCTRL.ctrl |= PMCTRL_TRIGGER;		// write pm reg. on v-blank time
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlavePMRectPosi(no, lx, ly);
#endif
}

//--------------------------------------------------------------------------------------------------
void GetPM_RectSize(BYTE no, PWORD pw, PWORD ph)
{
	if(no > 7)				return;					// no:0~7(rect), 8~15(poly)
	
	*pw = stPMCTRL.rect[no].w; 			*ph = stPMCTRL.rect[no].h;
}

//--------------------------------------------------------------------------------------------------
void GetPM_RectPosi(BYTE no, PWORD px, PWORD py)
{
	if(no > 7)				return;					// no:0~7(rect), 8~15(poly)
	
	*px = (stPMCTRL.rect[no].x + PMCTRL_GetMainSrcSizeH()/2) - stPMCTRL.rect[no].w/2;
	*py = (stPMCTRL.rect[no].y + GetIN_MainSrcSizeV()/2) - stPMCTRL.rect[no].h/2;
	// when position is '-6000', need protection ??????????????????????????????????
}

//--------------------------------------------------------------------------------------------------
void SetPM_PolyOnOff(BYTE no, BYTE OnOff)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2], i;
#endif
	
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	if(no > 3) 			return; 				// i500(poly:0~3), i51X(poly:0~7)
#endif
	
	stPMCTRL.poly[no].disp = OnOff;
	
	PMCTRL_SetPolyOnOff(no, OnOff);
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlavePMPolyOnOff(no, OnOff);
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = 0x00;
	for(i=0;i<8;i++) 		buff[1] |= (stPMCTRL.poly[i].disp)?  (1<<i):  0;
	
	EEPROM_MultiWrite(EEPROM_PM_POLYDISP, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE GetPM_PolyOnOff(BYTE no)
{
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	if(no > 3) 			return 0; 				// i500(poly:0~3), i51X(poly:0~7)
#endif
	
	return 	stPMCTRL.poly[no].disp;
}

//--------------------------------------------------------------------------------------------------
void SetPM_PolyColor(BYTE no, BYTE color)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[5];
#endif
	
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	if(no > 3) 			return; 				// i500(poly:0~3), i51X(poly:0~7)
#endif
	
	stPMCTRL.poly[no].color = color;
	
	PMCTRL_SetPolyColor(no, color);
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlavePMPolyColor(no, color);
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = MAKEBYTE(stPMCTRL.poly[1].color,  stPMCTRL.poly[0].color);
	buff[2] = MAKEBYTE(stPMCTRL.poly[3].color,  stPMCTRL.poly[2].color);
	buff[3] = MAKEBYTE(stPMCTRL.poly[5].color,  stPMCTRL.poly[4].color);
	buff[4] = MAKEBYTE(stPMCTRL.poly[7].color,  stPMCTRL.poly[6].color);
	
	EEPROM_MultiWrite(EEPROM_PM_POLYCOLOR, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE GetPM_PolyColor(BYTE no)
{
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	if(no > 3) 			return 0; 				// i500(poly:0~3), i51X(poly:0~7)
#endif
	
	return 	stPMCTRL.poly[no].color;
}

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD2__)	// 4k60 slave only
//--------------------------------------------------------------------------------------------------
static void SetPM_SlavePolyPosi(BYTE edge, PPM_POLY_INFO pSRC, PPM_POLY_INFO pDST)
{
	DWORD h, w, w_hid, h_off;
	WORD in_size_h = PMCTRL_GetMainSrcSizeH();
	
	memcpy(pDST, pSRC, sizeof(PM_POLY_INFO));
	
	//UARTprintf("org[%4d.%4d], [%4d.%4d], ", pDST->ulx, pDST->uly, pDST->urx, pDST->ury);
	//UARTprintf("[%4d.%4d], [%4d.%4d]\r\n", pDST->dlx, pDST->dly, pDST->drx, pDST->dry);
	
	// check new x position(limit)
	if(pDST->ulx >= in_size_h/2)	pDST->ulx -= in_size_h/2;	// md2 side
	else							pDST->ulx=0;				// md1 side
	
	if(pDST->urx >= in_size_h/2)	pDST->urx -= in_size_h/2;
	else							pDST->urx=0;
	
	if(pDST->dlx >= in_size_h/2)	pDST->dlx -= in_size_h/2;
	else							pDST->dlx=0;
	
	if(pDST->drx >= in_size_h/2)	pDST->drx -= in_size_h/2;
	else							pDST->drx=0;
	
	//UARTprintf("lmt[%4d.%4d], [%4d.%4d], ", pDST->ulx, pDST->uly, pDST->urx, pDST->ury);
	//UARTprintf("[%4d.%4d], [%4d.%4d]\r\n", pDST->dlx, pDST->dly, pDST->drx, pDST->dry);
	
	// check unavailable case
	if((pSRC->ulx > in_size_h/2) && (pSRC->dlx < in_size_h/2))	{		// case : ulx(md2), dlx(md1)
		if(edge==0)		{pSRC->dlx = in_size_h/2;	pDST->dlx = 0;}		// move dlx to center.
		if(edge==2)		{pSRC->ulx = in_size_h/2;	pDST->ulx = 0;}		// move ulx to center.
	}
	
	if((pSRC->ulx < in_size_h/2) && (pSRC->dlx > in_size_h/2))	{		// case : ulx(md1), dlx(md2)
		if(edge==0)		{pSRC->dlx = in_size_h/2;	pDST->dlx = 0;}		// move dlx to center.
		if(edge==2)		{pSRC->ulx = in_size_h/2;	pDST->ulx = 0;}		// move ulx to center.
	}
	
	if((pSRC->urx > in_size_h/2) && (pSRC->drx <= in_size_h/2))	{		// case : urx(md2), drx(md1)
		if(edge==1)		{pSRC->drx = in_size_h/2+16;	pDST->drx=16;}	// move drx to right
		if(edge==3)		{pSRC->urx = in_size_h/2;		pDST->urx=0;}	// move urx to center
	}
	
	if((pSRC->urx <= in_size_h/2) && (pSRC->drx > in_size_h/2))	{		// case : urx(md1), drx(md2)
		if(edge==1)		{pSRC->drx = in_size_h/2;		pDST->drx=0;}	// move drx to center
		if(edge==3)		{pSRC->urx = in_size_h/2+16;	pDST->urx=16;}	// move urx to right
	}
	
	
	// move 'uly,dly' up/down
	if((pSRC->ulx < in_size_h/2) && (pSRC->urx > in_size_h/2))	{
		w = DIFF(pSRC->ulx, pSRC->urx);			h = DIFF(pSRC->uly, pSRC->ury);
		w_hid = in_size_h/2 - pSRC->ulx;		h_off = h * (w_hid * 1000 / w) / 1000;
		
		if(pSRC->uly < pSRC->ury)				pDST->uly += h_off;	// 'uly' move down
		else									pDST->uly -= h_off;	// 'uly' move up
	}
	
	if((pSRC->dlx < in_size_h/2) && (pSRC->drx > in_size_h/2))	{
		w = DIFF(pSRC->dlx, pSRC->drx);			h = DIFF(pSRC->dly, pSRC->dry);
		w_hid = in_size_h/2 - pSRC->dlx;		h_off = h * (w_hid * 1000 / w) / 1000;
		
		if(pSRC->dly < pSRC->dry)				pDST->dly += h_off;	// 'dly' move down
		else									pDST->dly -= h_off;	// 'dly' move up
	}
	
	//UARTprintf("end[%4d.%4d], [%4d.%4d], ", pDST->ulx, pDST->uly, pDST->urx, pDST->ury);
	//UARTprintf("[%4d.%4d], [%4d.%4d]\r\n\n", pDST->dlx, pDST->dly, pDST->drx, pDST->dry);
}

//--------------------------------------------------------------------------------------------------
static WORD GetPM_PolyPosiX(BYTE edge, PPM_POLY_INFO pINFO)
{
	switch(edge)	{
		case  0: 		return pINFO->ulx;	break;
		case  1: 		return pINFO->urx;	break;
		case  2: 		return pINFO->dlx;	break;
		case  3: 		return pINFO->drx;	break;
	}
	
	return 0;
}
#endif

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
//--------------------------------------------------------------------------------------------------
static void SetPM_MasterPolyPosi(BYTE edge, PPM_POLY_INFO pINFO)
{
	WORD in_size_h = PMCTRL_GetMainSrcSizeH();
	
	//UARTprintf("org[%4d.%4d], [%4d.%4d], ", pINFO->ulx, pINFO->uly, pINFO->urx, pINFO->ury);
	//UARTprintf("[%4d.%4d], [%4d.%4d]\r\n", pINFO->dlx, pINFO->dly, pINFO->drx, pINFO->dry);
	
	// check unavailable case
	if((pINFO->ulx > in_size_h/2) && (pINFO->dlx < in_size_h/2))	{	// case : ulx(md2), dlx(md1)
		if(edge==0)		pINFO->dlx = in_size_h/2;						// move dlx to center.
		if(edge==2)		pINFO->ulx = in_size_h/2;						// move ulx to center.
	}
	
	if((pINFO->ulx < in_size_h/2) && (pINFO->dlx > in_size_h/2))	{	// case : ulx(md1), dlx(md2)
		if(edge==0)		pINFO->dlx = in_size_h/2;						// move dlx to center.
		if(edge==2)		pINFO->ulx = in_size_h/2;						// move ulx to center.
	}
	
	if((pINFO->urx > in_size_h/2) && (pINFO->drx <= in_size_h/2))	{	// case : urx(md2), drx(md1)
		if(edge==1)		pINFO->drx = in_size_h/2+16;					// move drx to right
		if(edge==3)		pINFO->urx = in_size_h/2;						// move urx to center
	}
	
	if((pINFO->urx <= in_size_h/2) && (pINFO->drx > in_size_h/2))	{	// case : urx(md1), drx(md2)
		if(edge==1)		pINFO->drx = in_size_h/2;						// move drx to center
		if(edge==3)		pINFO->urx = in_size_h/2+16;					// move urx to right
	}

	//UARTprintf("end[%4d.%4d], [%4d.%4d], ", pINFO->ulx, pINFO->uly, pINFO->urx, pINFO->ury);
	//UARTprintf("[%4d.%4d], [%4d.%4d]\r\n\n", pINFO->dlx, pINFO->dly, pINFO->drx, pINFO->dry);
}
#endif

//--------------------------------------------------------------------------------------------------
void SetPM_PolyPosi(BYTE no, BYTE edge, WORD x, WORD y)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD2__)	// 4k60 slave only
	PM_POLY_INFO	stPOLY;
#endif
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[6];
#endif
	
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	if(no > 3) 			return; 				// i500(poly:0~3), i51X(poly:0~7)
#endif
	
	switch(edge)	{
		case  0: 		stPMCTRL.poly[no].ulx = x;	stPMCTRL.poly[no].uly = y;	break;
		case  1: 		stPMCTRL.poly[no].urx = x;	stPMCTRL.poly[no].ury = y;	break;
		case  2: 		stPMCTRL.poly[no].dlx = x;	stPMCTRL.poly[no].dly = y;	break;
		case  3: 		stPMCTRL.poly[no].drx = x;	stPMCTRL.poly[no].dry = y;	break;
	}
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlavePMPolyPosi(no, edge, x, y);
	SetPM_MasterPolyPosi(edge, &stPMCTRL.poly[no]);
#endif

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD2__)	// 4k60 slave only
	SetPM_SlavePolyPosi(edge, &stPMCTRL.poly[no], &stPOLY);
	x = GetPM_PolyPosiX(edge, &stPOLY);
	PMCTRL_SetPolyMinMax(no, &stPOLY);
	PMCTRL_SetPolySlope(no, &stPOLY);
#else
	PMCTRL_SetPolyMinMax(no, &stPMCTRL.poly[no]);
	PMCTRL_SetPolySlope(no, &stPMCTRL.poly[no]);
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 			buff[1] = 0x00;
	buff[2] = LOBYTE(x); 					buff[3] = HIBYTE(x);
	buff[4] = LOBYTE(y); 					buff[5] = HIBYTE(y);
	
	EEPROM_MultiWrite(EEPROM_PM_POLYPOSI + (24*no) + (6*edge), (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void GetPM_PolyPosi(BYTE no, BYTE edge, PWORD px, PWORD py)
{
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	if(no > 3) 			return; 				// i500(poly:0~3), i51X(poly:0~7)
#endif
	
	switch(edge)	{
		case  0: 		*px = stPMCTRL.poly[no].ulx;	*py = stPMCTRL.poly[no].uly;	break;
		case  1: 		*px = stPMCTRL.poly[no].urx;	*py = stPMCTRL.poly[no].ury;	break;
		case  2: 		*px = stPMCTRL.poly[no].dlx;	*py = stPMCTRL.poly[no].dly;	break;
		case  3: 		*px = stPMCTRL.poly[no].drx;	*py = stPMCTRL.poly[no].dry;	break;
	}
}

//--------------------------------------------------------------------------------------------------
void SetPM_CamPanTIlt(WORD pan, WORD tilt)								// camera direction
{
	BYTE  i;
	
	// calc origin_x/y with pan/tilt
	if(pan >= 0x800)		stPMCTRL.cam_pan = -((0x1000 - pan) * 3);	// angle(-) : cam move left -> mask move right, 960 / 320(28.1C) = 3
	else					stPMCTRL.cam_pan = pan * 3;					// angle(+) : cam move right -> mask move left
	
	if(tilt >= 0xC00)		stPMCTRL.cam_tilt = (0x1000 - tilt) * 3;	// angle(-) : cam move down -> mask move up, 540 / 180(15.9C) = 3
	else					stPMCTRL.cam_tilt = -(tilt * 3);			// angle(+) : cam move down -> mask move up
	
	for(i=0;i<8;i++)	{												// no:0~7(rect), 8~15(poly)
		if((stPMCTRL.noninterlock_mask & (1<<i)) == 0)		{			// skip when non-interlock mode
			stPMCTRL.mask_no |= (1 << i);
			stPMCTRL.ctrl |= PMCTRL_TRIGGER;							// write pm reg. on v-blank time
		}
	}
}

//--------------------------------------------------------------------------------------------------
void SetPM_MaskPTZ(BYTE no, WORD pan, WORD tilt, WORD zoom)				// mask direction
{
	WORD  i, unit=0, tenths_ratio;
	SHORT offx, offy;
	WORD  zposi_tbl[30] = { 	// fcb-ev7500
		0x0000, 0x16A1, 0x2063, 0x2628, 0x2A1D, 0x2D13, 0x2F6D, 0x3161, 0x330D, 0x3486,	// x1 ~ x10
		0x35D7, 0x3709, 0x3820, 0x3920, 0x3A0A, 0x3ADD, 0x3B9C, 0x3C46, 0x3CDC, 0x3D60,	// x11 ~ x20
		0x3DD4, 0x3E39, 0x3E90, 0x3EDC, 0x3F1E, 0x3F57, 0x3F8A, 0x3FB6, 0x3FDC, 0x4000,	// x21 ~ x30
	};
	
	if(no > 7)				return;					// no:0~7(rect), 8~15(poly)
	
	// calc zoom ratio
	zoom = MIN(zoom, 0x4000);
	for(i=0;i<30;i++)	{
		if(zoom <= zposi_tbl[i])	{
			unit = DIFF(zposi_tbl[i], zposi_tbl[(i)? i-1: 0]); 	break; 				// calc zoom position gap
		}
	}
	
	tenths_ratio = ((zoom - zposi_tbl[(i)? i-1: 0]) * 10000 / unit);
	tenths_ratio = sqrt((tenths_ratio * tenths_ratio) / 10000 * tenths_ratio); 	// .ratio^(3/2) for small tolerance(mask vs zoom)
	stPMCTRL.rect[no].zoom = (((i)*10000) + tenths_ratio) / 100;	// ratio : x1 ~ x30 --> 100 ~ 30000
	stPMCTRL.rect[no].zoom = MIN(MAX(stPMCTRL.rect[no].zoom, 100), 30000);
//	UARTprintf("SetPM_MaskPTZ, ratio(%d.%d)\r\n", stPMCTRL.rect[no].zoom/100, stPMCTRL.rect[no].zoom%100);
	
	// calc origin_x/y with pan/tilt -- mask direction
	if(pan >= 0x800)		offx = -(0x1000 - pan) * 3;					// angle(-) : mask move left, 960 / 320(28.1C) = 3
	else					offx = pan * 3;								// angle(+) : mask move right
	
	if(tilt >= 0xC00)		offy = (0x1000 - tilt) * 3;					// angle(-) : mask move down, 540 / 180(15.9C) = 3
	else					offy = -(tilt * 3);							// angle(+) : mask move up
	
	// check non-interlock mode
	if((stPMCTRL.noninterlock_mask & (1 << no)) == 0)	{
		stPMCTRL.rect[no].x = offx;										// set position offset when normal mode
		stPMCTRL.rect[no].y = offy;
	}
	else	{
		stPMCTRL.rect[no].x = -stPMCTRL.cam_pan;						// disable position offset when non-interlock mode
		stPMCTRL.rect[no].y = -stPMCTRL.cam_tilt;
	}
	
	stPMCTRL.mask_no |= (1 << no);		stPMCTRL.ctrl |= PMCTRL_TRIGGER;		// write pm reg. on v-blank time
}

//--------------------------------------------------------------------------------------------------
void SetPM_NonInterlockMask(BYTE no, WORD w, WORD h, WORD x, WORD y)
{
	SHORT  posi_x=0, posi_y=0;
	
	if(no > 7)				return;					// no:0~7(rect), 8~15(poly)
	
	stPMCTRL.noninterlock_mask |= (1 << no);			// set non-interlock mask flag
	
	// calc position offset x,y
	if(x >= 0xB0)		posi_x -= (0x100-x)*13;			// pan(B0h ~ FFh)	--> -1040 ~ -13
	else				posi_x += x * 13;				// pan(0 ~ 50h)		--> 0 ~ +1040
	
	if(y >= 0xC4)		posi_y += (0x100-y)*13;			// tilt(C4h ~ FFh)	--> -780 ~ -13
	else				posi_y -= y * 13;				// tilt(0 ~ 3Ch)	--> 0 ~ +780
	
	// set position x,y
	stPMCTRL.rect[no].x = posi_x;	stPMCTRL.rect[no].y = posi_y;
	stPMCTRL.rect[no].w = w*22;		stPMCTRL.rect[no].h = h*22;
	
	stPMCTRL.mask_no |= (1 << no);		stPMCTRL.ctrl |= PMCTRL_TRIGGER;		// write pm reg. on v-blank time
}


// pm menu application
//--------------------------------------------------------------------------------------------------
void SetPM_OnOffMenu(WORD nID)
{
	WORD mask_no = GetMenuStatus(PM_MASKNO_MENU); 	// 0:MASK1,,7:MASK8
	
	SetPM_OnOff((nID)?  ON:  OFF); 				// pm enable/disable
	
	if(nID == ON) 	SetPM_MaskNoMenu(mask_no); 		// update menu info(on/off, color, size, position)
}

//--------------------------------------------------------------------------------------------
void SetPM_TypeMenu(WORD nID)
{
	SetPM_Type(nID); 					// 0:rect, 1:poly
	
	SetMenuStatus(PM_MASKNO_MENU, 0); 	// set mask no. default : 0
	SetMenuStatus(PM_POLYSEL_MENU, 0); 	// set edge default : L-TOP
	SetPM_MaskNoMenu(0); 				// update menu info(on/off, color, size, position)
}

//--------------------------------------------------------------------------------------------
void SetPM_MaskNoMenu(WORD nID)
{
	WORD type = GetMenuStatus(PM_TYPE_MENU); 			// 0:rect, 1:poly
	WORD edge = GetMenuStatus(PM_POLYSEL_MENU); 		// 0:L-TOP, 1:R-TOP, 2:L-BOT, 3:R-BOT
	WORD x, y, w, h;
	WORD div_x = (PMCTRL_GetMainSrcSizeH() >= 3840)?  16:  8;
	WORD div_y = (GetIN_MainSrcSizeV() >= 2160)?  10:  5;
	
	// menu update(on/off, color, size/position)
	if(type==1) 	{ 				// poly
#if defined(__USE_MDIN_i500__)		// MDIN-i500
		if(nID > 3) 				{nID -= 4; 		SetMenuStatus(PM_MASKNO_MENU, nID);} 	// i500(poly:0~3), i51x(poly:0~7)
#endif
		
		SetMenuStatus(PM_DISP_MENU, GetPM_PolyOnOff(nID));
		SetMenuStatus(PM_COLOR_MENU, GetPM_PolyColor(nID));
		
		GetPM_PolyPosi(nID, edge, &x, &y);
		SetMenuStatus(PM_POLYX_MENU, x/div_x); 			SetMenuStatus(PM_POLYY_MENU, y/div_y);
	}
	else	{ 						// rect
		SetMenuStatus(PM_DISP_MENU, GetPM_RectOnOff(nID));
		SetMenuStatus(PM_COLOR_MENU, GetPM_RectColor(nID));
		
		GetPM_RectPosi(nID, &x, &y); 				GetPM_RectSize(nID, &w, &h);
		SetMenuStatus(PM_SQRX_MENU, x/32); 			SetMenuStatus(PM_SQRY_MENU, y/32);
		SetMenuStatus(PM_SQRW_MENU, w/32); 			SetMenuStatus(PM_SQRH_MENU, h/32);
	}
}

//--------------------------------------------------------------------------------------------
void SetPM_MaskOnOffMenu(WORD nID)
{
	WORD type = GetMenuStatus(PM_TYPE_MENU); 				// 0:rect, 1:poly
	WORD mask_no = GetMenuStatus(PM_MASKNO_MENU);
	
	if(type==1) 	SetPM_PolyOnOff(mask_no, nID); 			// polygon
	else 			SetPM_RectOnOff(mask_no, nID); 			// rectangle
}

//--------------------------------------------------------------------------------------------
void SetPM_ColorMenu(WORD nID)
{
	WORD type = GetMenuStatus(PM_TYPE_MENU); 				// 0:rect, 1:poly
	WORD mask_no = GetMenuStatus(PM_MASKNO_MENU);
	
	if(type==1) 	SetPM_PolyColor(mask_no, nID); 			// polygon
	else 			SetPM_RectColor(mask_no, nID); 			// rectangle
}

//--------------------------------------------------------------------------------------------
void SetPM_RectPosiXMenu(WORD nID)
{
	WORD mask_no = GetMenuStatus(PM_MASKNO_MENU); 				// 0:MASK1,,7:MASK8
	WORD new_lx = nID * 32;
	WORD lx, ly, w, h;
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[3];
#endif
	
	GetPM_RectPosi(mask_no, &lx, &ly); 			GetPM_RectSize(mask_no, &w, &h);
	
	// check h-posi limit
	if((new_lx + w) > PMCTRL_GetMainSrcSizeH()) 	{SetMenuStatus(PM_SQRX_MENU, lx/32);	new_lx = lx;}	//restore old value
	
	SetPM_RectSizeDirect(mask_no, w, h);
	SetPM_RectPosiDirect(mask_no, new_lx, ly);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = new_lx / 32; 			buff[2] = ly / 32;
	
	EEPROM_MultiWrite(EEPROM_PM_RECTPOSI + (4*mask_no), (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------
void SetPM_RectPosiYMenu(WORD nID)
{
	WORD mask_no = GetMenuStatus(PM_MASKNO_MENU); 				// 0:MASK1,,7:MASK8
	WORD new_ly = nID * 32;
	WORD lx, ly, w, h;
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[3];
#endif
	
	GetPM_RectPosi(mask_no, &lx, &ly); 			GetPM_RectSize(mask_no, &w, &h);
	
	// check v-posi limit
	if((new_ly + h) > GetIN_MainSrcSizeV()) 	{SetMenuStatus(PM_SQRY_MENU, ly/32);	new_ly = ly;}	//restore old value
	
	SetPM_RectSizeDirect(mask_no, w, h);
	SetPM_RectPosiDirect(mask_no, lx, new_ly);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = lx / 32; 				buff[2] = new_ly / 32;
	
	EEPROM_MultiWrite(EEPROM_PM_RECTPOSI + (4*mask_no), (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------
void SetPM_RectSizeWMenu(WORD nID)
{
	WORD mask_no = GetMenuStatus(PM_MASKNO_MENU); 				// 0:MASK1,,7:MASK8
	WORD new_w = nID * 32;
	WORD lx, ly, w, h;
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[3];
#endif
	
	GetPM_RectPosi(mask_no, &lx, &ly); 			GetPM_RectSize(mask_no, &w, &h);
	
	// check h-size limit
	if((lx + new_w) > PMCTRL_GetMainSrcSizeH()) 	{SetMenuStatus(PM_SQRW_MENU, w/32);		new_w = w;}	//restore old value
	
	SetPM_RectSizeDirect(mask_no, new_w, h);
	SetPM_RectPosiDirect(mask_no, lx, ly);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = new_w / 32; 				buff[2] = h / 32;
	
	EEPROM_MultiWrite(EEPROM_PM_RECTSIZE + (4*mask_no), (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------
void SetPM_RectSizeHMenu(WORD nID)
{
	WORD mask_no = GetMenuStatus(PM_MASKNO_MENU); 				// 0:MASK1,,7:MASK8
	WORD new_h = nID * 32;
	WORD lx, ly, w, h;
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[3];
#endif
	
	GetPM_RectPosi(mask_no, &lx, &ly); 			GetPM_RectSize(mask_no, &w, &h);
	
	// check v-size limit
	if((ly + new_h) > GetIN_MainSrcSizeV()) 	{SetMenuStatus(PM_SQRH_MENU, h/32);		new_h = h;}	// restore old value
	
	SetPM_RectSizeDirect(mask_no, w, new_h);
	SetPM_RectPosiDirect(mask_no, lx, ly);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = w / 32; 					buff[2] = new_h / 32;
	
	EEPROM_MultiWrite(EEPROM_PM_RECTSIZE + (4*mask_no), (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------
void SetPM_PolyEdgeSelMenu(WORD nID)
{
	WORD mask_no = GetMenuStatus(PM_MASKNO_MENU); 				// 0:MASK1,,3:MASK4	
	WORD x, y;
	WORD div_x = (PMCTRL_GetMainSrcSizeH() >= 3840)?  16:  8;
	WORD div_y = (GetIN_MainSrcSizeV() >= 2160)?  10:  5;
	
	GetPM_PolyPosi(mask_no, nID, &x, &y);
	SetMenuStatus(PM_POLYX_MENU, x/div_x);
	SetMenuStatus(PM_POLYY_MENU, y/div_y);
}

//--------------------------------------------------------------------------------------------
void SetPM_PolyXposiMenu(WORD nID)
{
	WORD mask_no = GetMenuStatus(PM_MASKNO_MENU); 			// 0:MASK1,,3:MASK4
	WORD edge = GetMenuStatus(PM_POLYSEL_MENU); 			// 0:L-TOP,,3:R-BOTTOM
	WORD old_x, old_y, tmp_y, ulx, urx, dlx, drx;
	WORD div = (PMCTRL_GetMainSrcSizeH() >= 3840)?  16:  8;
	
	GetPM_PolyPosi(mask_no, edge, &old_x, &old_y); 			// load prev. posi value
	
	switch(edge)	{	//pm x overflow check
		case 0: 	ulx = nID * div; 						// calc new 'ulx'
					GetPM_PolyPosi(mask_no, 1, &urx, &tmp_y); 	// load 'urx'
					if(ulx < urx) 		{SetPM_PolyPosi(mask_no, edge, ulx, old_y); 	break;}
					else 				{SetMenuStatus(PM_POLYX_MENU, old_x/div);	return;}
					
		case 1: 	urx = nID * div; 						// calc new 'urx'
					GetPM_PolyPosi(mask_no, 0, &ulx, &tmp_y); 	// load 'ulx'
					if(urx > ulx) 		{SetPM_PolyPosi(mask_no, edge, urx, old_y); 	break;}
					else 				{SetMenuStatus(PM_POLYX_MENU, old_x/div);	return;}
					
		case 2: 	dlx = nID * div; 						// calc new 'dlx'
					GetPM_PolyPosi(mask_no, 3, &drx, &tmp_y); 	// load 'drx'
					if(dlx < drx) 		{SetPM_PolyPosi(mask_no, edge, dlx, old_y); 	break;}
					else 				{SetMenuStatus(PM_POLYX_MENU, old_x/div);	return;}
					
		case 3: 	drx = nID * div; 						// calc new 'drx'
					GetPM_PolyPosi(mask_no, 2, &dlx, &tmp_y); 	// load 'dlx'
					if(drx > dlx) 		{SetPM_PolyPosi(mask_no, edge, drx, old_y); 	break;}
					else 				{SetMenuStatus(PM_POLYX_MENU, old_x/div);	return;}
	}
	
	//converting osd_parameter to rtl pixel by pixel parameter	odd: (nID-1)*8 + 7 , even (nID*8)
//	stPM_poly[area][edge].x = nID*8 - (nID%2); 	// -- need ????
}

//--------------------------------------------------------------------------------------------
void SetPM_PolyYposiMenu(WORD nID)
{
	WORD mask_no = GetMenuStatus(PM_MASKNO_MENU); 			// 0:MASK1,,3:MASK4
	WORD edge = GetMenuStatus(PM_POLYSEL_MENU); 			// 0:L-TOP,,3:R-BOTTOM
	WORD old_x, old_y, tmp_x, uly, ury, dly, dry;
	WORD div = (GetIN_MainSrcSizeV() >= 2160)?  10:  5;
	
	GetPM_PolyPosi(mask_no, edge, &old_x, &old_y); 			// load prev. posi value
	
	// check y_position overflow
	switch(edge)	{
		case 0: 	uly = nID * div; 							// calc new 'uly'
					GetPM_PolyPosi(mask_no, 2, &tmp_x, &dly); 	// load 'dly, dry'
					GetPM_PolyPosi(mask_no, 3, &tmp_x, &dry);
					if((uly < dly) && (uly < dry)) 		{SetPM_PolyPosi(mask_no, edge, old_x, uly); 	break;}
					else 									{SetMenuStatus(PM_POLYY_MENU, old_y/div);	return;}
					
		case 1: 	ury = nID * div; 							// calc new 'ury'
					GetPM_PolyPosi(mask_no, 2, &tmp_x, &dly); 	// load 'dly, dry'
					GetPM_PolyPosi(mask_no, 3, &tmp_x, &dry);
					if((ury < dly) && (ury < dry)) 		{SetPM_PolyPosi(mask_no, edge, old_x, ury); 	break;}
					else 									{SetMenuStatus(PM_POLYY_MENU, old_y/div);	return;}
					
		case 2: 	dly = nID * div; 							// calc new 'dly'
					GetPM_PolyPosi(mask_no, 0, &tmp_x, &uly); 	// load 'uly, ury'
					GetPM_PolyPosi(mask_no, 1, &tmp_x, &ury);
					if((dly > uly) && (dly > ury)) 		{SetPM_PolyPosi(mask_no, edge, old_x, dly); 	break;}
					else 									{SetMenuStatus(PM_POLYY_MENU, old_y/div);	return;}
					
		case 3: 	dry = nID * div; 							// calc new 'dry'
					GetPM_PolyPosi(mask_no, 0, &tmp_x, &uly); 	// load 'uly, ury'
					GetPM_PolyPosi(mask_no, 1, &tmp_x, &ury);
					if((dry > uly) && (dry > ury)) 		{SetPM_PolyPosi(mask_no, edge, old_x, dry); 	break;}
					else 									{SetMenuStatus(PM_POLYY_MENU, old_y/div);	return;}
	}
}

//--------------------------------------------------------------------------------------------------
void SetPM_FactoryDefaultMenu(void)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	WORD i;
	WORD buff=0xffff;
#endif
	// set pm parameter to factory default
	memset(&stPMCTRL, 0, sizeof(PM_CTRL_PARAM));
	SetPM_Default(&stPMCTRL);
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlavePMDefault();
#endif
	
	// erase eeprom
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	for(i=0;i<20;i+=2) 		EEPROM_RegWrite(EEPROM_PM_MODE+i, buff); 		// erase pm menu item(20byte)
	for(i=0;i<256;i+=2) 	EEPROM_RegWrite(EEPROM_PM_SIZEPOSI+i, buff); 	// erase pm size/posi param(256byte)
#endif
	
	// set menu value with pm parameter
	SetMenuStatus(PM_MODE_MENU, GetPM_OnOff()); 			// pm mode
	SetMenuStatus(PM_TYPE_MENU, GetPM_Type()); 				// pm type
	SetMenuStatus(PM_DISP_MENU, GetPM_MaskOnOffMenu()); 	// pm mask display on/off
	SetMenuStatus(PM_COLOR_MENU, GetPM_ColorMenu()); 		// pm mask color
	SetMenuStatus(PM_SQRX_MENU, GetPM_PosiXMenu()); 		// pm position-x
	SetMenuStatus(PM_SQRY_MENU, GetPM_PosiYMenu()); 		// pm position-y
	SetMenuStatus(PM_SQRW_MENU, GetPM_WidthMenu()); 		// pm width
	SetMenuStatus(PM_SQRH_MENU, GetPM_HeightMenu()); 		// pm height
}

//--------------------------------------------------------------------------------------------------
BYTE GetPM_MaskOnOffMenu(void)
{
	return  (BYTE)(GetPM_Type()?  GetPM_PolyOnOff(0):  GetPM_RectOnOff(0)); 	// type(0:rect, 1:poly)
}

//--------------------------------------------------------------------------------------------------
BYTE GetPM_ColorMenu(void)
{
	return  (BYTE)(GetPM_Type()?  GetPM_PolyColor(0):  GetPM_RectColor(0)); 	// type(0:rect, 1:poly)
}

//--------------------------------------------------------------------------------------------------
BYTE GetPM_PosiXMenu(void)
{
	WORD x, y, div;
	
	if(GetPM_Type()) 	{GetPM_PolyPosi(0, 0, &x, &y); 		div = (PMCTRL_GetMainSrcSizeH() >= 3840)?  16:  8;} 	// poly
	else 				{GetPM_RectPosi(0, &x, &y); 		div = 32;} 											// rect
	
	return  (BYTE)(x / div);
}

//--------------------------------------------------------------------------------------------------
BYTE GetPM_PosiYMenu(void)
{
	WORD x, y, div;
	
	if(GetPM_Type()) 	{GetPM_PolyPosi(0, 0, &x, &y); 		div = (GetIN_MainSrcSizeV() >= 2160)?  10:  5;} 	// poly
	else 				{GetPM_RectPosi(0, &x, &y); 		div = 32;} 											// rect
	
	return  (BYTE)(y / div);
}

//--------------------------------------------------------------------------------------------------
BYTE GetPM_WidthMenu(void)
{
	WORD w, h, div;
	
	if(GetPM_Type()) 	{w=0;  h=0;  div = 0;} 						// poly
	else 				{GetPM_RectSize(0, &w, &h); 	div = 32;} 		// rect
	
	return  (BYTE)(w / div);
}

//--------------------------------------------------------------------------------------------------
BYTE GetPM_HeightMenu(void)
{
	WORD w, h, div;
	
	if(GetPM_Type()) 	{w=0;  h=0;  div = 0;} 						// poly
	else 				{GetPM_RectSize(0, &w, &h); 	div = 32;} 		// rect
	
	return  (BYTE)(h / div);
}


// md api
//--------------------------------------------------------------------------------------------------
void SetMD_OnOff(WORD OnOff)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	if(OnOff) 		stMDCTRL.ctrl |= MDCTRL_MD_ON;
	else 			stMDCTRL.ctrl &= (~MDCTRL_MD_ON);
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveMDOnOff(OnOff);
#endif

	MDCTRL_SetOnOff(OnOff);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)OnOff;
	EEPROM_MultiWrite(EEPROM_SPECIAL_MD, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void SetMD_MotionView(WORD OnOff)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveMDMotionView(OnOff);
#endif
	
	if(OnOff) 		stMDCTRL.ctrl |= MDCTRL_MOTIONVIEW_ON;
	else 			stMDCTRL.ctrl &= (~MDCTRL_MOTIONVIEW_ON);
	
	MDCTRL_SetMotionView(OnOff);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)OnOff;
	EEPROM_MultiWrite(EEPROM_MD_MOTIONVIEW, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void SetMD_BlockSize(WORD size)
{
	stMDCTRL.block_size = size;
	
	MDCTRL_SetBlockSize(size);
}

//--------------------------------------------------------------------------------------------------
void SetMD_Threshold(WORD val)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveMDThreshold(val);
#endif
	
	stMDCTRL.thres = val;
	
	if(val > 19) 	val = 2*val - 18; 							// 20~40 -> 22~62
	else 			val = val + 1; 							// 0~19 -> 1~20
	
	MDCTRL_SetThreshold(val);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)stMDCTRL.thres;
	EEPROM_MultiWrite(EEPROM_MD_THRESHOLD, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void SetMD_VideoSize(WORD md_h_size, WORD md_v_size)
{
	stMDCTRL.video_size_h = md_h_size; 			stMDCTRL.video_size_v = md_v_size;
	
	MDCTRL_SetVideoSize(md_h_size, md_v_size);
}

//--------------------------------------------------------------------------------------------------
void SetMD_AreaEn(BYTE no, WORD OnOff)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2], i;
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveMDAreaOnOff(no, OnOff);
#endif
	
	stMDCTRL.area[no].en = (OnOff)?  ON:  OFF;
	
	MDCTRL_SetAreaEn(no, OnOff);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = 0x00;
	for(i=0;i<4;i++) 		buff[1] |= (stMDCTRL.area[i].en)?  (1<<i):  0;
	EEPROM_MultiWrite(EEPROM_MD_AREAEN, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void SetMD_AreaDisp(BYTE no, WORD OnOff)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2], i;
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveMDAreaDisp(no, OnOff);
#endif
	
	stMDCTRL.area[no].disp = (OnOff)?  ON:  OFF;
	
	MDCTRL_SetAreaDisp(no, OnOff);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = 0x00;
	for(i=0;i<4;i++) 		buff[1] |= (stMDCTRL.area[i].disp)?  (1<<i):  0;
	EEPROM_MultiWrite(EEPROM_MD_AREADISP, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void SetMD_AreaSizePosi(BYTE no, BYTE w, BYTE h, BYTE lx, BYTE ly)
{
	BYTE  old_lx, old_ly, old_w, old_h;
	BYTE  max_h = stMDCTRL.video_size_h / stMDCTRL.block_size;
	BYTE  max_v = stMDCTRL.video_size_v / stMDCTRL.block_size;
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[6];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveMDSizePosi(no, w, h, lx, ly);
#endif
	
	old_lx = GetMD_AreaPosiX(no); 		old_ly = GetMD_AreaPosiY(no); 		// load prev. size/posi parameter
	old_w = GetMD_AreaWidth(no); 		old_h = GetMD_AreaHeight(no);
	
	if(lx + w > max_h) 				{lx = old_lx; 	w = old_w;} 		// check size/posi limit
	if(ly + h > max_v) 				{ly = old_ly; 	h = old_h;}
	if(w==0) 	w = 1;
	if(h==0) 	h = 1;
	
	stMDCTRL.area[no].lx = lx; 			stMDCTRL.area[no].ly = ly; 			// update size/posi parameter
	stMDCTRL.area[no].w = w; 			stMDCTRL.area[no].h = h;
	
	MDCTRL_SetSizePosi(no, w, h, lx, ly);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = w; 				buff[2] = h;
	buff[3] = lx; 				buff[4] = ly;
	
	EEPROM_MultiWrite(EEPROM_MD_SIZEPOSI + (6*no), (PBYTE)&buff[0], sizeof(buff));
#endif
	
#if __MISP100_DBGPRT__ == 1
	UARTprintf("MDCTRL_SetSizePosi [%d] : [%d.%d, %d.%d] -> [%d.%d, %d.%d]\r\n", no, lx, ly, lx+w, ly+h, 
				lx*stMDCTRL.block_size, ly*stMDCTRL.block_size, (lx+w)*stMDCTRL.block_size, (ly+h)*stMDCTRL.block_size);	//test
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE GetMD_OnOff(void)
{
	return  (stMDCTRL.ctrl & MDCTRL_MD_ON)?  ON:  OFF;
}

//--------------------------------------------------------------------------------------------------
BYTE GetMD_AreaEn(BYTE no)
{
	return  (stMDCTRL.area[no].en)?  ON:  OFF;
}

//--------------------------------------------------------------------------------------------------
BYTE GetMD_AreaDisp(BYTE no)
{
	return  (stMDCTRL.area[no].disp)?  ON:  OFF;
}

//--------------------------------------------------------------------------------------------------
BYTE GetMD_AreaWidth(BYTE no)
{
	return  stMDCTRL.area[no].w;
}

//--------------------------------------------------------------------------------------------------
BYTE GetMD_AreaHeight(BYTE no)
{
	return  stMDCTRL.area[no].h;
}

//--------------------------------------------------------------------------------------------------
BYTE GetMD_AreaPosiX(BYTE no)
{
	return  stMDCTRL.area[no].lx;
}

//--------------------------------------------------------------------------------------------------
BYTE GetMD_AreaPosiY(BYTE no)
{
	return  stMDCTRL.area[no].ly;
}

//--------------------------------------------------------------------------------------------------
BYTE GetMD_SizeMaxH(void)
{
	return  stMDCTRL.video_size_h / stMDCTRL.block_size;
}

//--------------------------------------------------------------------------------------------------
BYTE GetMD_SizeMaxV(void)
{
	return  stMDCTRL.video_size_v / stMDCTRL.block_size;
}

//--------------------------------------------------------------------------------------------------
WORD GetMD_MotionView(void)
{
	if(stMDCTRL.ctrl & MDCTRL_MOTIONVIEW_ON)	return TRUE;
	else										return FALSE;
}

//--------------------------------------------------------------------------------------------------
WORD GetMD_Threshold(void)
{
	return	stMDCTRL.thres;
}

// md menu app
//--------------------------------------------------------------------------------------------------
void SetMD_OnOffMenu(WORD nID)
{
	SetMD_OnOff((nID)?  ON:  OFF);
	SetMD_AreaSelMenu(0);
}

//--------------------------------------------------------------------------------------------------
void SetMD_AreaSelMenu(WORD nID)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveMDAreaSel(nID);
#endif
	
	SetMenuStatus(MD_AREA_MENU, nID); 						// nID : 0(area-1) ~ 3(area-4)		
	
	SetMenuStatus(MD_AREA_EN_MENU, GetMD_AreaEn(nID)); 		SetMenuStatus(MD_DISP_MENU, GetMD_AreaDisp(nID));
	SetMenuStatus(MD_POSI_MENU, GetMD_AreaPosiX(nID)); 		SetMenuPosiY(MD_POSI_MENU, GetMD_AreaPosiY(nID));
	SetMenuStatus(MD_SIZE_MENU, GetMD_AreaWidth(nID)); 		SetMenuPosiY(MD_SIZE_MENU, GetMD_AreaHeight(nID));
}

//--------------------------------------------------------------------------------------------------
void SetMD_AreaOnOffMenu(WORD nID)
{
	WORD  area = GetMenuStatus(MD_AREA_MENU);
	
	SetMD_AreaEn(area, nID);
}


//--------------------------------------------------------------------------------------------------
void SetMD_AreaDispMenu(WORD nID)
{
	WORD  area = GetMenuStatus(MD_AREA_MENU);
	
	SetMD_AreaDisp(area, (nID)?  ON:  OFF);
}

//--------------------------------------------------------------------------------------------------
void SetMD_PosiMenu(WORD nID)
{
	WORD  area = GetMenuStatus(MD_AREA_MENU);
	WORD  lx = GetMenuStatus(MD_POSI_MENU),	ly = GetMenuPosiY(MD_POSI_MENU); 
	WORD  w = GetMenuStatus(MD_SIZE_MENU),	h = GetMenuPosiY(MD_SIZE_MENU); 
	
	SetMD_AreaSizePosi(area, w, h, lx, ly);
	
	// update menu value with size/posi limit calc.
	SetMenuStatus(MD_POSI_MENU, GetMD_AreaPosiX(area));	SetMenuPosiY(MD_POSI_MENU, GetMD_AreaPosiY(area));
	SetMenuStatus(MD_SIZE_MENU, GetMD_AreaWidth(area));	SetMenuPosiY(MD_SIZE_MENU, GetMD_AreaHeight(area));
}

//--------------------------------------------------------------------------------------------------
void SetMD_SizeMenu(WORD nID)
{
	WORD  area = GetMenuStatus(MD_AREA_MENU);
	WORD  lx = GetMenuStatus(MD_POSI_MENU),	ly = GetMenuPosiY(MD_POSI_MENU); 
	WORD  w = GetMenuStatus(MD_SIZE_MENU),	h = GetMenuPosiY(MD_SIZE_MENU); 
	
	SetMD_AreaSizePosi(area, w, h, lx, ly);
	
	// update menu value with size/posi limit calc.
	SetMenuStatus(MD_POSI_MENU, GetMD_AreaPosiX(area));	SetMenuPosiY(MD_POSI_MENU, GetMD_AreaPosiY(area));
	SetMenuStatus(MD_SIZE_MENU, GetMD_AreaWidth(area));	SetMenuPosiY(MD_SIZE_MENU, GetMD_AreaHeight(area));
}

//--------------------------------------------------------------------------------------------------
void SetMD_FactoryDefaultMenu(void)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	WORD i;
	WORD buff=0xffff;
	
	for(i=0;i<32;i+=2) 		EEPROM_RegWrite(EEPROM_MD_MENU+i, buff); 	// erase md param(32byte)
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveMDDefault();
#endif
	
	memset(&stMDCTRL, 0, sizeof(MD_CTRL_PARAM)); 			// md parameter clear
	SetMD_Default(&stMDCTRL); 								// md parameter set default
	
	SetMD_OnOff(ON); 										// md on/off set 'on'
	SetMD_AreaSelMenu(0); 									// md area set '0'
}


/*  FILE_END_HERE */
