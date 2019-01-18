//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	MISPHOST.C
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
HOST_CTRL_PARAM		stHOSTCTRL;

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
extern VBYTE fIRQGAfinish;

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
	
// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// driver
//------------------------------------------------------------------------------------
static WORD ISPM HOSTCTRL_GetIRQID(void)
{
	WORD	irq_status;
	
	MISP_RegRead(MISP_HOST_ID, 0x010, &irq_status);		// get irq status
	MISP_RegWrite(MISP_HOST_ID, 0x010, irq_status);		// clear status reg.
	
	return (irq_status & stHOSTCTRL.stIRQ.en);
}

//------------------------------------------------------------------------------------
static void HOSTCTRL_SetIRQEN(WORD int_enable)
{
	MISP_RegWrite(MISP_HOST_ID, 0x00E, int_enable);
}
/*
//------------------------------------------------------------------------------------
static void HOSTCTRL_IrqHandler(PHOST_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&HOSTCTRL_UPDATE_IRQ)==0) return;
	pCTRL->ctrl &= (~HOSTCTRL_UPDATE_IRQ);
	
//	HOSTCTRL_SetIRQMode(pCTRL->stIRQ.mode);
	HOSTCTRL_SetIRQEN(pCTRL->stIRQ.en);
}
*/
//------------------------------------------------------------------------------------------
static void Init_HOST_PARAM( PHOST_CTRL_PARAM pCTRL )
{
//	pCTRL->stIRQ.en = MISP_IRQ_VDPULSE | MISP_IRQ_ISP_VACT_OUT_3A | MISP_IRQ_DIS;
	pCTRL->stIRQ.en = MISP_IRQ_VDPULSE | MISP_IRQ_ISP_VACT_OUT_3A | MISP_IRQ_DIS | MISP_IRQ_GA_GAC_FINISH;
}

//--------------------------------------------------------------------------------------
static void Init_HOST_REGISTER( PHOST_CTRL_PARAM pCTRL ) 
{
//	MISP_RegWrite(MISP_HOST_ID,  0x092, 0x0600);	// c4 ddr 90MHz
	MISP_RegWrite(MISP_HOST_ID,  0x092, 0x0500);	// c4 ddr 108MHz
	MISP_RegWrite(MISP_HOST_ID, 0x005, 0x1900);	//bank2K, bus wr byte/32b swap[12:11]
#if defined(__USE_MDIN_i500__)					// MDIN-i500
	MISP_RegWrite(MISP_HOST_ID, 0x006, 0x8000);	//bus rd byte swap[15]
	MISP_RegWrite(MISP_HOST_ID, 0x008, 0x0800);	//bus rd 32b swap[11]
#else											// MDIN-i51X (i510, i540, i550)
	MISP_RegWrite(MISP_HOST_ID, 0x008, 0x1800);	//bus rd '32b swap'[11], 'byte swap'[12] -- for DDR 1Gb
#endif
	MISP_RegWrite(MISP_HOST_ID, 0x012, 0x0003);	//endian_swap 3
	MISP_RegWrite(MISP_HOST_ID, 0x010, 0xffff);	//int_status : clear status reg.
	
	HOSTCTRL_SetIRQEN(pCTRL->stIRQ.en);
}



//api
//----------------------------------------------------------------------------------------
void Init_HOST(void)
{
	memset(&stHOSTCTRL, 0, sizeof(HOST_CTRL_PARAM));
	Init_HOST_PARAM(&stHOSTCTRL);
	Init_HOST_REGISTER(&stHOSTCTRL);
}

//----------------------------------------------------------------------------------------
void HOST_ProcessHandler(void)
{
//	if ((stHOSTCTRL.ctrl&HOSTCTRL_TRIGGER_ISP)==0) return;
//	stHOSTCTRL.ctrl &= (~HOSTCTRL_TRIGGER_ISP);
//	
//	HOSTCTRL_IrqHandler(&stHOSTCTRL);
}



//get/set func.
//-------------------------------------------------------------------------------------
void SetHOST_IRQEN(WORD  val)
{
	stHOSTCTRL.stIRQ.en |= val;
	HOSTCTRL_SetIRQEN(stHOSTCTRL.stIRQ.en);
}

//--------------------------------------------------------------------------------------------------
WORD ISPM GetHOST_IRQID()
{
	return		HOSTCTRL_GetIRQID();	
}

//--------------------------------------------------------------------------------------------------
void SetHOST_IRQGACFinishCLR(void)
{
	fIRQGAfinish=0;
}

//--------------------------------------------------------------------------------------------------
void GetHOST_IRQGACFinishWait(void)
{
	DWORD  Timeout=0xffffff;	// about 1sec
	
//	while(!fIRQGAfinish);
	while(!fIRQGAfinish)	{	// for test
		if(Timeout-- == 0)		{UARTprintf("GetHOST_IRQGACFinishWait,, timeout error !!!!\r\n");	return;}
	}
	fIRQGAfinish=0;
}

/*  FILE_END_HERE */
