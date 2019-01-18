//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2017  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	LCMX03LF.C
// Description 		:	Lattice FPGA ( LCMX03LF-6900 )
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"

#if defined(__USE_LATTICE_LCMX03LF__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
LCMX03LF_CTRL_PARAM	stLCMX03LF;

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//------------------------------------------------------------------------------------------
static void Init_LCMX03LF_PARAM(PLCMX03LF_CTRL_PARAM pCTRL)
{
#if defined(__USE_CMOS_IMX274__)
	pCTRL->h_size = 3864;
	pCTRL->offCH1 = 0;			pCTRL->offCH1 /= 8;
	pCTRL->offCH2 = 1920;		pCTRL->offCH2 /= 8;
#endif
	
#if defined(__USE_CMOS_IMX226__)
	pCTRL->h_size = 3952;
	pCTRL->offCH1 = 0+96;		pCTRL->offCH1 /= 8;
	pCTRL->offCH2 = 1920+96;	pCTRL->offCH2 /= 8;
#endif
	
	pCTRL->offCH1_org = pCTRL->offCH1;
	pCTRL->offCH2_org = pCTRL->offCH2;
}

//--------------------------------------------------------------------------------------------------
static void Init_LCMX03LF_REGISTER(PLCMX03LF_CTRL_PARAM pCTRL)
{
	// register init
	SetLCMX03LF_sw_rst();
	
	SetLCMX03LF_hsize(pCTRL->h_size);
	SetLCMX03LF_offch1(pCTRL->offCH1);
	SetLCMX03LF_offch2(pCTRL->offCH2);
}



//--------------------------------------------------------------------------------------------
void LCMX03LF_Init(void)
{
	memset(&stLCMX03LF, 0, sizeof(LCMX03LF_CTRL_PARAM));
	
	Init_LCMX03LF_PARAM(&stLCMX03LF);
	Init_LCMX03LF_REGISTER(&stLCMX03LF);
}

//--------------------------------------------------------------------------------------------
void SetLCMX03LF_sw_rst(void)
{
	LCMX03L_RegField(0x00, 0, 1, 0x00);						// soft reset
	LCMX03L_RegField(0x00, 0, 1, 0x01);
}

//--------------------------------------------------------------------------------------------
void SetLCMX03LF_hsize(WORD val)
{
	stLCMX03LF.h_size = val;
	LCMX03L_RegField(0x01, 0, 8, LOBYTE(stLCMX03LF.h_size));	// h_size[7:0]
	LCMX03L_RegField(0x02, 0, 4, HIBYTE(stLCMX03LF.h_size));	// h_size[12:8]
}

//--------------------------------------------------------------------------------------------
void SetLCMX03LF_offch1(WORD val)
{
	stLCMX03LF.offCH1 = val;
	LCMX03L_RegField(0x03, 0, 8, LOBYTE(stLCMX03LF.offCH1));	// offset_ch1[7:0]
	LCMX03L_RegField(0x04, 0, 1, HIBYTE(stLCMX03LF.offCH1));	// offset_ch1[8]
}

//--------------------------------------------------------------------------------------------
void SetLCMX03LF_offch2(WORD val)
{
	stLCMX03LF.offCH2 = val;
	LCMX03L_RegField(0x05, 0, 8, LOBYTE(stLCMX03LF.offCH2));	// offset_ch2[7:0]
	LCMX03L_RegField(0x06, 0, 1, HIBYTE(stLCMX03LF.offCH2));	// offset_ch2[8]
}

//--------------------------------------------------------------------------------------------
WORD GetLCMX03LF_offch1(void)
{
	return stLCMX03LF.offCH1;
}

//--------------------------------------------------------------------------------------------
WORD GetLCMX03LF_offch2(void)
{
	return stLCMX03LF.offCH2;
}

//--------------------------------------------------------------------------------------------
WORD GetLCMX03LF_offch1_org(void)
{
	return stLCMX03LF.offCH1_org;
}

//--------------------------------------------------------------------------------------------
WORD GetLCMX03LF_offch2_org(void)
{
	return stLCMX03LF.offCH2_org;
}


#endif	//__USE_LATTICE_LCMX03LF__


/*  FILE_END_HERE */
