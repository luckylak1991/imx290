//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  ROTATION.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550

#ifndef		__MISP_ROTATION_H__
#define		__MISP_ROTATION_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

typedef	struct
{
	LONG 	ctrl;			// Rotation ctrl flag
	
	BOOL	rot_enable ;
	BOOL	rot_demo;
	BOOL	spll_backup;	// sensor pll backup flag.
	BYTE	rot_mode;
	BYTE	rot_prev_mode;
	BYTE	bit_precision ; // 0: 12b 1: 10b
	WORD	rot_cnt;
	WORD	h_size;
	WORD	h_start;			
	WORD	h_end;			
	WORD	v_size;
	WORD	v_start;		
	WORD	v_end;		
	
	WORD	bound_h_size;			
	WORD	bound_v_size;	
	
	WORD	active_h_size;			
	WORD	active_v_size;
	
	WORD	total_h_size;			
	WORD	total_v_size;	
	
	WORD	spll_p;			// sensor pll 'p' -- for 4k sensor frame rate limit(10.1fr_375Mhz, 10.8fr_399Mhz)
	WORD	spll_n;			// sensor pll 'n'
	WORD	spll_m;			// sensor pll 'm'
	WORD	spll_od;		// sensor pll 'od'
	WORD	spll_s;			// sensor pll 's'
	
	DDRMAP_PARAM stDDRMAP_normal ;
	
}	ROT_CTRL_PARAM, *PROT_CTRL_PARAM;

#define	ROTCTRL_TRIGGER_ISP			0x40000000		// ISP update
#define ROTCTRL_ROTATION_UPDATE		0x10000000		// rotation update
#define ROTCTRL_TEMPMFC_UPDATE		0x20000000		// rotation update

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Device drive functions

// rotation.c
	
void Init_ROT(void) ;
	
void ROT_ProcessHandler( void ) ;

// -----------------------------------------------------------------------------
// API Get / Set functions

void SetROT_RotationEnable( WORD val ) ;	
BOOL GetROT_RotationEnable( void ) ;	
BYTE GetROT_BitMode( void ) ;	
BYTE GetROT_RotationMode(void);



#endif	/* __MISP_ROTATION_H__ */

#endif // MDIN-i51X (i510, i540, i550)
