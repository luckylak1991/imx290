//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  mispfd.h
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:
#ifndef		__MISP_FD_H__
#define		__MISP_FD_H__

#if defined(__USE_FD_PROCESS__)
// -----------------------------------------------------------------------------
// Sensor/Board dependent
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
#define USE_OSD_BOX		1
#define OSD_START		SBOX_INDEX4
#define OSD_LIMIT		SBOX_INDEX7

#if defined(__USE_MDIN_i500__)
#define REV_BYTE_SWAP	0
#else
#define REV_BYTE_SWAP	1
#endif

//integral frame size
#define FD_DECI_FACTOR	14
#define FD_FRAME_HSIZE	(1920/FD_DECI_FACTOR)
#define FD_FRAME_VSIZE	(1080/FD_DECI_FACTOR)
#define FD_FRAME_LSIZE	(FACADE_VSIZE+1)  //24+1

//bucket size
#define FD_SLOT_NUM		10

//read decimal y position
#define FD_BILINE_READ	1

//aux memory
#if defined(__USE_MDIN_i500__)
#define FD_AUX_MM_ADDR       0x0FD8	 // 0x1B3[14:0] : map2_start_row
#define FD_COL_CNT_PER_LINE  0x0080	 // 0x1C5[ 9:0] : map2_col_count_per_line
#else
#define FD_AUX_MM_ADDR       0x0FF0	 // 0x1B3[14:0] : map2_start_row
#define FD_COL_CNT_PER_LINE  0x0081	 // 0x1C5[ 9:0] : map2_col_count_per_line
#endif

typedef	struct
{
	WORD	lx;
	WORD	ly;
	WORD	rx;
	WORD	ry;
} POS_t;

typedef	struct
{
	WORD	cont;    // continue iteration
	WORD	scle;    // scale iteration
	WORD	vert;    // vertical iteration
	WORD	hori;    // horizontal iteration
} FD_ITER_PARAM, *PFD_ITER_PARAM;

typedef	struct
{
	BYTE	ctrl;                 // FD control flag
	BYTE	proc_step;            // FD state
	BYTE	req_count;            // waiting requested integral count
	BYTE	fd_num;               // FD found count
	POS_t	fd_pos[FD_SLOT_NUM];  // FD found position
	DWORD	mem_start;            // AUX start address
	DWORD	mem_count;            // AUX column count per one line
} FD_CTRL_PARAM, *PFD_CTRL_PARAM;


// WDR control flags
#define	FDCTRL_TRIGGER_ISP	0x01  // On/Off control
#define FDCTRL_PROCESS_ON	0x02  // FD process on

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------
extern VBYTE fFDCtrlProcCtrl;
#define FD_ITER_START  0x01  // start FD iteration
#define FD_ITER_SEIZE  0x02  // seize FD iteration

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
// mispfd.c
void Init_FD(void);
void FD_ProcessHandler(void);

//api controls
void SetFD_ProcesMode(BOOL OnOff);
BOOL GetFD_ProcesMode(void);

#endif  //#if defined(__USE_FD_PROCESS__)

#endif	/* __MISP_FD_H__ */

