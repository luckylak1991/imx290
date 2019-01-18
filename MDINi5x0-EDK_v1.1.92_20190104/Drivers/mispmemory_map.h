//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MEMORY_MAP.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISP_MEMORY_MAP_H__  
#define		__MISP_MEMORY_MAP_H__  

#if	defined(__USE_MDIN_i500__)			// MDIN-i500
#define	__MEMORY_MAP_REV__ 0
#else									// MDIN-i51X (i510, i540, i550)
#define	__MEMORY_MAP_REV__ 1
#endif
// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#define _MEM_CONFIG_ 					0x1b0
#define _MAP0_START_ROW_ 				0x1B1
#define _MAP1_START_ROW_ 				0x1B2
#define _MAP2_START_ROW_ 				0x1B3
#define _MAP3_START_ROW_ 				0x1B4
#define _MAP4_START_ROW_ 				0x1B5
#define _MAP5_START_ROW_ 				0x1B6
#define _MAP0t_START_ROW_ 				0x1B7
#define _MAP1t_START_ROW_ 				0x1B8
#define _MAP2t_START_ROW_ 				0x1B9
#define _MAP3t_START_ROW_ 				0x1BA
#define _MAP4t_START_ROW_ 				0x1BB
#define _MAP5t_START_ROW_ 				0x1BC
#define _MAP0_ROW_COUNT_PER_FRAME_ 		0x1BD
#define _MAP1_ROW_COUNT_PER_FRAME_ 		0x1BE
#define _MAP2_ROW_COUNT_PER_FRAME_ 		0x1BF
#define _MAP3_ROW_COUNT_PER_FRAME_ 		0x1C0
#define _MAP4_ROW_COUNT_PER_FRAME_ 		0x1C1
#define _MAP5_ROW_COUNT_PER_FRAME_ 		0x1C2
#define _MAP0_COL_COUNT_PER_LINE_ 		0x1C3
#define _MAP1_COL_COUNT_PER_LINE_ 		0x1C4
#define _MAP2_COL_COUNT_PER_LINE_ 		0x1C5
#define _MAP3_COL_COUNT_PER_LINE_ 		0x1C6
#define _MAP4_COL_COUNT_PER_LINE_ 		0x1C7
#define _MAP5_COL_COUNT_PER_LINE_ 		0x1C8
#define _MAP_F_COUNT_0_ 				0x1D7
#define _MAP_F_COUNT_4_ 				0x1D8
#define _MAP_F_COUNT_0t_ 				0x1D9
#define _MAP_F_COUNT_4t_ 				0x1DA
#define _MAPPING_0A_ 					0x1C9
#define _MAPPING_0B_ 					0x1CA
#define _MAPPING_1_ 					0x1CB
#define _MAPPING_2_ 					0x1CC
#define _MAPPING_3_ 					0x1CD
#define _MAPPING_4_ 					0x1DD
#define _MAPPING_5_ 					0x1D2
#define _MAPPING_6_ 					0x1D4
#define _PRI_STARV_0_ 					0x1CE
#define _PRI_STARV_1_ 					0x1CF
#define _PRI_STARV_2_ 					0x1D0
#define _PRI_STARV_3_ 					0x1D1
#define _PRI_STARV_4_ 					0x1DE
#define _PRI_STARV_5_ 					0x1D3
#define _PRI_STARV_6_ 					0x1D5
#define _FC_MC_MON_						0x1D6
#define _ARBITER_CTRL_ 					0x1A0
#define _ARBITER_PRI_ 					0x1A1
#define _ARBITER_STARV_ 				0x1A2
#define _AXO_PRI_STARV_ 				0x1A3
#define _MCU_PRI_VALUE_FF_ 				0x1A5
#define _DIAG_TEST_FF_ 					0x1A6	

typedef	enum {
	MISP_DDRMAP_8b = 0,		
	MISP_DDRMAP_10b ,				
	MISP_DDRMAP_9b ,				
	MISP_DDRMAP_12b ,			
}	MISP_DDRMAP_BIT_MODE_t;

typedef	struct
{
	BOOL	refresh_prog_on ;
	BYTE	refresh_prog_count ;
	
	BOOL	refresh_en_n ;
	BOOL	refresh_mode ;
	BOOL	low_speed_mode ;
	BOOL	priority_scheme_n ;

	BYTE	fc_pri  ;	
	BYTE	osd_pri  ;	
	BYTE	host_pri  ;	
	BYTE	mcu_pri  ;	
	
	BYTE	fc_stv  ;	
	BYTE	osd_stv  ;	
	BYTE	host_stv  ;	
	BYTE	mcu_stv  ;	
	
	BOOL	reset_req  ;	
	BOOL	access_en  ;	
	BOOL	drv_strength  ;	
	
}	DDRMAP_ARBITER_PARAM, *PDDRMAP_ARBITER_PARAM;

typedef	struct
{
	BYTE	frame_cnt ;
	
	WORD	start_row_addr_mo ;
	WORD	start_row_addr_mohist ;
	WORD	end_row_addr ;
	
	BYTE	bit_precision ;
	WORD	row_count_per_frame ;
	WORD	col_count_per_line ;
	
}	DDRMAP_WDR_MO_MAP, *PDDRMAP_WDR_MO_MAP;

typedef	struct
{
	BYTE	frame_cnt_y ;
	BYTE	frame_cnt_c ;
	
	WORD	start_row_addr_y ;
	WORD	start_row_addr_c ;
	WORD	end_row_addr ;
	
	BYTE	bit_precision ;
	WORD	row_count_per_frame ;
	WORD	col_count_per_line ;
	
	BYTE	mapping_write_y  ;
	BYTE	mapping_read_y  ;
	BYTE	mapping_write_c  ;	
	BYTE	mapping_read_c  ;	
	
	BYTE	read_pri  ;	
	BYTE	write_pri  ;	
	BYTE	read_stv  ;	
	BYTE	write_stv  ;	
	
}	DDRMAP_FCMRU_PARAM00, *PDDRMAP_FCMRU_PARAM00;

typedef	struct
{
	BYTE	frame_cnt_y ;
	BYTE	frame_cnt_c ;
	
	WORD	start_row_addr_y ;
	WORD	start_row_addr_c ;
	WORD	end_row_addr ;
	
	BYTE	bit_precision ;
	WORD	row_count_per_frame ;
	WORD	col_count_per_line ;
	
	BYTE	mapping_write_y  ;
	BYTE	mapping_read_y  ;
	BYTE	mapping_write_mo  ;	
	BYTE	mapping_read_mo  ;	
	BYTE	mapping_read_mohist  ;	
	BYTE	mapping_write_mohist  ;	
	
	BYTE	read_pri  ;	
	BYTE	write_pri  ;	
	BYTE	read_stv  ;	
	BYTE	write_stv  ;	
	
	DDRMAP_WDR_MO_MAP mo_map ;
	
}	DDRMAP_FCMRU_PARAM01, *PDDRMAP_FCMRU_PARAM01;

typedef	struct
{
	WORD bounadry_hsize ;
	WORD bounadry_vsize ;
	WORD active_hsize ;
	WORD active_vsize ;

	WORD main_hsize ;
	WORD main_vsize ;

	WORD aux_hsize ;
	WORD aux_vsize ;	
	
	BYTE rotation_en ;
	BYTE IPC_444_en ;
	BYTE RGB_gamma_en ;	
	BYTE vertical_flip_en ;	
	BYTE DIS_en ;	
	BYTE NR_image_type ;
	
	BYTE DIS_frame_count ;	
	BYTE IPC_frame_count ;
	BYTE IPC_frame_count_c ;
	BYTE AUX_frame_count ;
	BYTE AUX_frame_count_c ;
	BYTE NR_frame_count ;	
	BYTE WDRRW_frame_count ;
	BYTE WDRSW_frame_count ;
	BYTE ROT_frame_count ;
	
	BYTE IPC_bit_precision ;
	BYTE AUX_bit_precision ;
	BYTE NR_bit_precision ;
	BYTE ROT_bit_precision ;
	BYTE WDR_SW_bit_precision ;
	BYTE WDR_RW_bit_precision ;
	
}	DDRMAP_PARAM, *PDDRMAP_PARAM;


typedef	struct
{
	LONG 	ctrl;			// DDRMAP ctrl flag

	BYTE	ddr_size;		// 0: 512M	1: 256M, 2: 1G
	
	DDRMAP_ARBITER_PARAM stARBITER ;
	
	DDRMAP_PARAM		 stDDRMAP ; 	
	
	DDRMAP_FCMRU_PARAM00 stMRU0_DIS ;
	DDRMAP_FCMRU_PARAM00 stMRU1_IPC ;
	DDRMAP_FCMRU_PARAM00 stMRU2_AUX ;
	DDRMAP_FCMRU_PARAM00 stMRU3_NR ;
	DDRMAP_FCMRU_PARAM01 stMRU4_WDR_RW ; // WDR result write
	DDRMAP_FCMRU_PARAM00 stMRU5_WDR_SW ; // WDR source write
	
}	DDRMAP_CTRL_PARAM, *PDDRMAP_CTRL_PARAM;

typedef enum {
	MISP_AUD_DELAY_MODE_16_ALL = 0,
	MISP_AUD_DELAY_MODE_64_ONE,
	MISP_AUD_DELAY_MODE_32_ALL,
	MISP_AUD_DELAY_MODE_128_ONE
	
}	MISP_AUD_DELAY_MODE_t;
		


#define	DDRMAPCTRL_TRIGGER_ISP		0x40000000	// trigger ISP
#define	DDRMAPCTRL_FCMRU_UPDATE		0x00000001	// FCMRU update
#define	DDRMAPCTRL_DDR_ACCESS_EN_UPDATE		0x00000002	// Arbiter update
#define	DDRMAPCTRL_DDR_REFRESH_EN_N_UPDATE		0x00000004	// Arbiter update
// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Device drive functions

	
// memory_map.c	
void Init_Arbiter ( void ) ;
void Init_DDRMAP ( void ) ;	

	
void DDRMAP_ProcessHandler(void) ;


// -----------------------------------------------------------------------------
// API Get / Set functions
void SetDDRMAP_ddr_access_enable ( BOOL val ) ;
void SetDDRMAP_ddr_refresh_enable_n ( BOOL val ) ;

// ---------------------------------------------------------

void SetDDRMAP ( PDDRMAP_PARAM pCtrl ) ;

// ---------------------------------------------------------

// ---------------------------------------------------------
void SetDDRMAP_DIS_mapping ( BYTE val ) ;
void SetDDRMAP_WDR(BYTE buffing, BYTE frame_num, BYTE sw_bit_mode, BYTE rw_bit_mode);

void SetDDRMAP_image_size(WORD bh, WORD bv, WORD ah, WORD av );
void SetDDRMAP_main_size(WORD ah, WORD av);
void SetDDRMAP_aux_size(WORD ah, WORD av);
void SetDDRMAP_RGBgamma_mode(BYTE val);
void SetDDRMAP_rotation_mode(BYTE val , WORD bh, WORD bv, WORD ah, WORD av );
void SetDDRMAP_vflip_mode(BYTE val, BYTE frm_count);
void SetDDRMAP_DIS_mode(BYTE val);
void SetDDRMAP_IPC444_mode(BYTE val);
void SetDDRMAP_IPC_Bitmode(BYTE Bitmode) ;
void SetDDRMAP_AUX_Bitmode(BYTE Bitmode) ;
void SetDDRMAP_NRImagetype_mode(BYTE val, BYTE Bitmode) ;

void SetDDRMAP_IPC_frame_count(BYTE frm_count) ;
void SetDDRMAP_AUX_frame_count(BYTE frm_count) ;

BYTE GetDDRMAP_IPC_Bitmode(void);
BYTE GetDDRMAP_AUX_Bitmode(void);

// ---------------------------------------------------------

void SetMemoryMAP_480i(void);
void SetMemoryMAP_1080i(void);

#if defined(__USE_CMOS_MT9P031__)		// use 5M Sensor	
void Set5MegaMemoryMap(BOOL OnOff);
#endif


// -----------------------------------------------------------------------------

BYTE GetDDRMAP_IPC_mapping(void) ;
BYTE GetDDRMAP_IPC_frame_count(void) ;

BYTE GetDDRMAP_AUX_mapping(void) ;
BYTE GetDDRMAP_AUX_frame_count(void) ;

WORD GetDDRMAP_IPC_Y_start_row(void);
WORD GetDDRMAP_IPC_C_start_row(void);
WORD GetDDRMAP_WDRSW_start_row(void);
WORD GetDDRMAP_NR_start_row(void);
WORD GetDDRMAP_IPC_end_row(void) ;
WORD GetDDRMAP_AUX_end_row(void) ;
WORD GetDDRMAP_NR_end_row(void) ;
WORD GetDDRMAP_WDRRW_end_row(void) ;
WORD GetDDRMAP_WDRSW_end_row(void) ;

void GetDDRMAP_WDRSW_MemCRPF(PBYTE bit_mode, PWORD CPF,PWORD RPF) ;
void GetDDRMAP_NR_MemCRPF(PBYTE bit_mode, PWORD CPF,PWORD RPF) ;
void GetDDRMAP_IPC_MemCRPF(PBYTE bit_mode, PWORD CPF,PWORD RPF) ;

void GetDDRMAP ( PDDRMAP_PARAM pCtrl ) ;

// -----------------------------------------------------------------------------
BYTE GetDDRMAP_vflip_mode(void);


// -----------------------------------------------------------------------------

void SetDDRMAP_SetAudioDelay(WORD AudioDelayMS, MISP_AUD_DELAY_MODE_t AudioDelayMode);


void SetDDR_WritePhyReg(U16 addr, U32 data);
void SetDDR_WriteCommand(BYTE mreg_ma, BYTE mreg_op);

#endif	/* __MISP_MEMORY_MAP_H__ */
