//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name			:  COLORCORRECTION.H
// Description			:  This file contains typedefine for the driver files	
// Ref. Docment			: 
// Revision History 	:

#ifndef		__MISP_COLORCORRECTION_H__
#define		__MISP_COLORCORRECTION_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
#define		CCF_Gainstep			8

typedef	struct
{
	//To preserve osd menu parameter
	WORD	R1_gain_menu;	
	WORD	R2_gain_menu;		
	WORD	G1_gain_menu;		
	WORD	B1_gain_menu;		
	WORD	B2_gain_menu;	

	WORD	R1_balance_menu;	
	WORD	R2_balance_menu;		
	WORD	G1_balance_menu;		
	WORD	B1_balance_menu;		
	WORD	B2_balance_menu;	

	WORD	rr_gain;
	WORD	gr_gain;	
	WORD	br_gain;	

	WORD	rg_gain;
	WORD	gg_gain;	
	WORD	bg_gain;		

	WORD	rb_gain;
	WORD	gb_gain;	
	WORD	bb_gain;	

	BYTE	Type_applyed;
}	CCF_APP_PARAM, *PCCF_APP_PARAM ;

typedef	enum
{
	Red_Line_Type1=0,
	Red_Line_Type2,
	Green_Line_Type1,	
	Blue_Line_Type1,		
	Blue_Line_Type2
}CCF_Type_Num;

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void SetCCFGain(WORD level, WORD line);	
void SetCCFBalance(WORD level, WORD line);
WORD GetCCFGain(WORD line);	
WORD GetCCFBalance(WORD line);
BYTE GetCCFType(void);

void SetCCFType(BYTE type);		
void CCFResultPrint(void);
void Init_CCF(void);
// -----------------------------------------------------------------------------

#endif	/* __MISPCOLORCORRECTION_H__ */

