//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MISPHOST.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISPHOST_H__
#define		__MISPHOST_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
typedef	enum {
	MISP_IRQ_VDPULSE				=0x0001,
	MISP_IRQ_TIMER					=0x0002,
	MISP_IRQ_ISP_VACT_OUT_3A		=0x0004,
	MISP_IRQ_HACT_PWM_3A			=0x0008,
	MISP_IRQ_ISP_MD_FLAG			=0x0010,
	MISP_IRQ_VACTIVE_YC2MD			=0x0020,
	MISP_IRQ_IN_SYNC_DETECT_VALID	=0x0040,
	MISP_IRQ_DIS					=0x0080,
	MISP_IRQ_WDR_INTERRUPT			=0x0100,
	MISP_IRQ_WDR_4_OR_NR_VACTIVE	=0x0200,
	MISP_IRQ_IN_SYNC_CHANGED_LOST	=0x0400,
	MISP_IRQ_FRAME_WR_SKIP			=0x0800,
	MISP_IRQ_FRAME_RD_RPT_WR_SKIP	=0x1000,
	MISP_IRQ_NR_OUT_VACTIVE			=0x2000,
	MISP_IRQ_GA_GAC_FINISH			=0x4000,
	MISP_IRQ_MRU_UNDERFLOW			=0x8000,
	
}	MISP_IRQ_INDEX_t;

typedef	struct
{
	WORD	en;						// on/off control of irq(one of MISP_IRQ_INDEX_t)
	WORD	mode;					// detect mode of irq

}	stPACKED MISP_IRQ_INFO, *PMISP_IRQ_INFO;

typedef	struct
{
	LONG	ctrl;					// host control flag
	
	MISP_IRQ_INFO	stIRQ;			// host irq info
	
}	HOST_CTRL_PARAM, *PHOST_CTRL_PARAM;

// host control parameter
#define		HOSTCTRL_TRIGGER_ISP		0x40000000		// ISP update
#define		HOSTCTRL_UPDATE_IRQ			0x00000001		// irq update
#define		HOSTCTRL_UPDATE_ALL			0x0000000F		// host all update


// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void Init_HOST(void);
void HOST_ProcessHandler(void);

void SetHOST_IRQEN(WORD val);
void SetHOST_IRQGACFinishCLR(void);
WORD ISPM GetHOST_IRQID(void);
void GetHOST_IRQGACFinishWait(void);


#endif	/* __MISPINPUT_H__ */
