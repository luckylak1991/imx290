//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MISPCSP.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550

#ifndef		__MISP_CSP_H__
#define		__MISP_CSP_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
typedef	struct
{
	BYTE en ;	
	BYTE vd_source ;		// [0: Y][1: C]
	BYTE use_vd_square ;	// [0: diff][1: diff square]
	BYTE var_y_range_sel ;	// use_vd_square 0 => [0:/8][1:/4][2:/2][3:/1]
	BYTE var_ys_range_sel ;	// use_vd_square 1 => [0:/128][1:/64][2:/32][3:/16]	
	
	BYTE x0 ; // chroma_sup_vd_x0 : x0 value on the curve(0~255)
	BYTE x1 ; // chroma_sup_vd_x1 : x1 value on the curve(0~255)
	BYTE x2 ; // chroma_sup_vd_x2 : x2 value on the curve(0~255)
	BYTE x3 ; // chroma_sup_vd_x3 : x3 value on the curve(0~255)
	
	WORD y0 ; // chroma_sup_vd_y0 : y0 value on the curve(0~511. 256 corresponds to gain 1.0)
	WORD y1 ; // chroma_sup_vd_y1 : y1 value on the curve(0~511. 256 corresponds to gain 1.0)
	WORD y2 ; // chroma_sup_vd_y2 : y2 value on the curve(0~511. 256 corresponds to gain 1.0)
	WORD y3 ; // chroma_sup_vd_y3 : y3 value on the curve(0~511. 256 corresponds to gain 1.0)
	
	WORD s0 ; // chroma_sup_vd_s0 : slop between (x0,y0) to (x1,y1). (signed 13bit. -64~+64. 0x40 corresponds to 1.0)
	WORD s1 ; // chroma_sup_vd_s1 : slop between (x1,y1) to (x2,y2). (signed 13bit. -64~+64. 0x40 corresponds to 1.0)
	WORD s2 ; // chroma_sup_vd_s2 : slop between (x2,y2) to (x3,y3). (signed 13bit. -64~+64. 0x40 corresponds to 1.0)
	
}	CSP_VD_PARAM, *PCSP_VD_PARAM ;

typedef	struct
{
	BYTE en ;	
		
	BYTE x0 ; // chroma_sup_ls_x0 : x0 value on the curve(0~255)
	BYTE x1 ; // chroma_sup_ls_x1 : x1 value on the curve(0~255)
	// BYTE x2 // 255 : x2 value on the curve(0~255)
	// BYTE x3 // 255 : x3 value on the curve(0~255)
	// WORD y0 // 0 : y0 value on the curve(0~511. 256 corresponds to gain 1.0)
	// WORD y1 // 256 : y1 value on the curve(0~511. 256 corresponds to gain 1.0)
	// WORD y2 // 256 : y2 value on the curve(0~511. 256 corresponds to gain 1.0)
	// WORD y3 // 256 : y3 value on the curve(0~511. 256 corresponds to gain 1.0)
	WORD s0 ; // chroma_sup_ls_s0 : slop between (x0,y0) to (x1,y1). (signed 13bit. -64~+64. 0x40 corresponds to 1.0)
	// BYTE s1 // 0 : slop between (x1,y1) to (x2,y2). (signed 13bit. -64~+64. 0x40 corresponds to 1.0)
	// BYTE s2 // 0 : slop between (x2,y2) to (x3,y3). (signed 13bit. -64~+64. 0x40 corresponds to 1.0)	
}	CSP_LS_PARAM, *PCSP_LS_PARAM ;

typedef	struct
{
	BYTE en ;
	
	BYTE x0 ; // chroma_sup_ld_x0 : x0 value on the curve(0~255)
	BYTE x1 ; // chroma_sup_ld_x1 : x1 value on the curve(0~255)
	BYTE x2 ; // chroma_sup_ld_x2 : x2 value on the curve(0~255)
	BYTE x3 ; // chroma_sup_ld_x3 : x3 value on the curve(0~255)
	
	WORD y0 ; // chroma_sup_ld_y0 : y0 value on the curve(0~511. 256 corresponds to gain 1.0)
	WORD y1 ; // chroma_sup_ld_y1 : y1 value on the curve(0~511. 256 corresponds to gain 1.0)
	WORD y2 ; // chroma_sup_ld_y2 : y2 value on the curve(0~511. 256 corresponds to gain 1.0)
	WORD y3 ; // chroma_sup_ld_y3 : y3 value on the curve(0~511. 256 corresponds to gain 1.0)
	
	WORD s0 ; // chroma_sup_ld_s0 : slop between (x0,y0) to (x1,y1). (signed 13bit. -64~+64. 0x40 corresponds to 1.0)
	WORD s1 ; // chroma_sup_ld_s1 : slop between (x1,y1) to (x2,y2). (signed 13bit. -64~+64. 0x40 corresponds to 1.0)
	WORD s2 ; // chroma_sup_ld_s2 : slop between (x2,y2) to (x3,y3). (signed 13bit. -64~+64. 0x40 corresponds to 1.0)	
}	CSP_LD_PARAM, *PCSP_LD_PARAM ;

typedef	struct
{
	LONG 	ctrl ;			// 	
	
	CSP_VD_PARAM stCSPVD ;
	CSP_LS_PARAM stCSPLS ;
	CSP_LD_PARAM stCSPLD ;
	
}	CSP_CTRL_PARAM, *PCSP_CTRL_PARAM ;

#define	CSPCTRL_TRIGGER_ISP			0x40000000		// ISP update
#define	CSPCTRL_VD_ENABLE			0x00000001		// VD update
#define	CSPCTRL_VD_UPDATE			0x00000002		// VD update
#define	CSPCTRL_LS_ENABLE			0x00000004		// LS update
#define	CSPCTRL_LS_UPDATE			0x00000008		// LS update
#define	CSPCTRL_LD_ENABLE			0x00000010		// LD update
#define	CSPCTRL_LD_UPDATE			0x00000020		// LD update

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Device drive functions
void Init_CSP(void) ;
void CSP_ProcessHandler( void ) ;

// -----------------------------------------------------------------------------
// API Get / Set functions
void SetCSP_LS_EN(BYTE en);
void SetCSP_LS_P0(BYTE x0);
void SetCSP_LS_P1(BYTE x1);

void SetCSP_LD_EN(BYTE en);
void SetCSP_LD_P0(BYTE x0, WORD y0);
void SetCSP_LD_P1(BYTE x1, WORD y1);
void SetCSP_LD_P2(BYTE x2, WORD y2);
void SetCSP_LD_P3(BYTE x3, WORD y3);

void SetCSP_VD_EN(BYTE en);
void SetCSP_VD_P0(BYTE x0, WORD y0);
void SetCSP_VD_P1(BYTE x1, WORD y1);
void SetCSP_VD_P2(BYTE x2, WORD y2);
void SetCSP_VD_P3(BYTE x3, WORD y3);
// -----------------------------------------------------------------------------


#endif

#endif // MDIN-i51X (i510, i540, i550)

