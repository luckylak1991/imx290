// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"misp100.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static DDRMAP_CTRL_PARAM stDDRMAPCTRL;	

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

static void Init_ARBITER_PARAM( PDDRMAP_CTRL_PARAM pCTRL ) ;
static void Init_DDRMAP_PARAM( PDDRMAP_CTRL_PARAM pCTRL ) ;
static void Init_DDRMAP_REGISTER( PDDRMAP_CTRL_PARAM pCTRL ) ;
static void Init_ARBITER_REGISTER(PDDRMAP_CTRL_PARAM pCTRL ) ;

static WORD DDRMAPCTRL_calc_col_count( WORD in_hsize , BYTE bit_precision ); // bit_precision => 0: 8b / 1: 10b / 2: 9b / 3: 12b
static WORD DDRMAPCTRL_calc_row_count( WORD col_per_line , WORD in_vsize );

static void DDRMAPCTRL_ArbiterDDRRefreshHandler( PDDRMAP_CTRL_PARAM pCTRL ) ;
static void DDRMAPCTRL_ArbiterDDRAccessHandler( PDDRMAP_CTRL_PARAM pCTRL ) ;
static void DDRMAPCTRL_DDRMAPSetHandler( PDDRMAP_CTRL_PARAM pCTRL ) ;
static void DDRMAPCTRL_DIS_MappingUpdate(PDDRMAP_CTRL_PARAM pCTRL) ;

static void SetDDRMAP_MRU0_DIS ( WORD in_hsize, WORD in_vsize, BYTE frame_cnt, BYTE bit_precision, WORD start_row_addr ) ;
static void SetDDRMAP_MRU1_IPC ( WORD in_hsize, WORD in_vsize, BYTE frame_cnt, BYTE frame_cnt_c, BYTE bit_precision, WORD start_row_addr , WORD isIPC_444) ;
static void SetDDRMAP_MRU2_AUX ( WORD in_hsize, WORD in_vsize, BYTE frame_cnt, BYTE frame_cnt_c, BYTE bit_precision, WORD start_row_addr ) ;
static void SetDDRMAP_MRU3_NR ( WORD in_hsize, WORD in_vsize, BYTE frame_cnt, BYTE nr_image_type, BYTE bit_precision, WORD start_row_addr ) ;
static void SetDDRMAP_MRU4_WDR_RW ( WORD in_hsize, WORD in_vsize, BYTE frame_cnt , BYTE bit_precision, WORD start_row_addr ) ;
static void SetDDRMAP_MRU5_WDR_SW ( WORD in_hsize, WORD in_vsize, BYTE frame_cnt , BYTE bit_precision, WORD start_row_addr ) ;

static void DDRMAPCTRL_DDRMAP_info( PDDRMAP_PARAM pCTRL) ;
// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------
extern void Delay_mSec(WORD delay);


//--------------------------------------------------------------------------------------------------------------------------
void DDRMAPCTRL_DDRMAP_info( PDDRMAP_PARAM pCTRL)
{
	UARTprintf("\r\n****DDR MAP SETUP PARAM\r\n") ;
	UARTprintf("[bounadry_hsize:%d], [bounadry_vsize:%d]\r\n", pCTRL->bounadry_hsize ,pCTRL->bounadry_vsize) ;		
	UARTprintf("[active_hsize:%d], [active_vsize:%d]\r\n", pCTRL->active_hsize , pCTRL->active_vsize) ;	
	UARTprintf("[aux_hsize:%d], [aux_vsize:%d]\r\n", pCTRL->aux_hsize , pCTRL->aux_vsize) ;	
	
#if __MEMORY_MAP_REV__		
	UARTprintf("[rotation_en:%d], [IPC_444_en:%d]\r\n[RGB_gamma_en:%d], [vertical_flip_en:%d] \r\n", pCTRL->rotation_en ,pCTRL->IPC_444_en , pCTRL->RGB_gamma_en , pCTRL->vertical_flip_en) ;	
#else
	UARTprintf("[vertical_flip_en:%d] \r\n", pCTRL->vertical_flip_en) ;
#endif
	UARTprintf("[DIS_en:%d] \r\n", pCTRL->DIS_en) ;	

	if ( pCTRL->DIS_en )
	{
		UARTprintf("**FRM count DIS OVERIDE!** \r\n") ;		
		UARTprintf("[FRM count DIS:%d] \r\n", pCTRL->DIS_frame_count) ;		
		UARTprintf("[FRM count IPC:%d,%d] \r\n", pCTRL->DIS_frame_count, pCTRL->DIS_frame_count) ;		
		UARTprintf("[FRM count AUX:%d,%d] \r\n", pCTRL->DIS_frame_count, pCTRL->DIS_frame_count) ;		
	}
	else
	{
		UARTprintf("[FRM count DIS:%d] \r\n", pCTRL->DIS_frame_count) ;		
		UARTprintf("[FRM count IPC:%d,%d] \r\n", pCTRL->IPC_frame_count, pCTRL->IPC_frame_count_c) ;		
		UARTprintf("[FRM count AUX:%d,%d] \r\n", pCTRL->AUX_frame_count, pCTRL->AUX_frame_count_c) ;		
	}
	
	UARTprintf("[FRM count NR:%d] \r\n", pCTRL->NR_frame_count) ;		
	UARTprintf("[FRM count WDR RW:%d] \r\n", pCTRL->WDRRW_frame_count) ;		
	UARTprintf("[FRM count WDR SW:%d] \r\n", pCTRL->WDRSW_frame_count) ;
#if __MEMORY_MAP_REV__	
	UARTprintf("[FRM count ROT:%d] \r\n", pCTRL->ROT_frame_count) ;
#endif
	UARTprintf("[NR_image_type:%d] \r\n", pCTRL->NR_image_type ) ;
	
#if __MEMORY_MAP_REV__		
	UARTprintf("[IPC_bit_precision:%d] \r\n[AUX_bit_precision:%d] \r\n[NR_bit_precision:%d] \r\n[WDR_SW_bit_precision:%d] \r\n[WDR_RW_bit_precision:%d] \r\n[ROT_bit_precision:%d] \r\n\r\n", pCTRL->IPC_bit_precision, pCTRL->AUX_bit_precision ,pCTRL->NR_bit_precision , pCTRL->WDR_SW_bit_precision , pCTRL->WDR_RW_bit_precision, pCTRL->ROT_bit_precision) ;
#else	
	UARTprintf("[IPC_bit_precision:%d] \r\n[NR_bit_precision:%d] \r\n[WDR_SW_bit_precision:%d] \r\n[WDR_RW_bit_precision:%d] \r\n\r\n", pCTRL->IPC_bit_precision ,pCTRL->NR_bit_precision , pCTRL->WDR_SW_bit_precision , pCTRL->WDR_RW_bit_precision) ;
#endif	
	
}	
	
//--------------------------------------------------------------------------------------------------------------------------
static WORD DDRMAPCTRL_calc_col_count( WORD in_hsize , BYTE bit_precision )
{
	WORD col_per_line ;
	BYTE divide_factor ;
	
	switch (bit_precision) {
		case MISP_DDRMAP_8b : divide_factor = 8 ; break ;
		case MISP_DDRMAP_10b : divide_factor = 6 ; break ;
		case MISP_DDRMAP_9b : divide_factor = 7 ; break ;
		case MISP_DDRMAP_12b : divide_factor = 5 ; break ;
		default : divide_factor = 8 ; break ;
	}
	
	if ( (in_hsize)%divide_factor == 0 ){
		col_per_line = (in_hsize)/divide_factor ;		
	}
	else {
		col_per_line = (in_hsize)/divide_factor + 1 ;
	}	
	
	if ( (col_per_line&0x0001) == 1 )	// If col_per_line is odd value
		col_per_line++ ; 				// Make it even
	else
		col_per_line=col_per_line+2 ; 	// guard space
	
	return col_per_line ;
}

//--------------------------------------------------------------------------------------------------------------------------
static WORD DDRMAPCTRL_calc_row_count( WORD col_per_line , WORD in_vsize )
{
	DWORD total_col_count ;
	WORD row_per_frame ;
	
	total_col_count = col_per_line * in_vsize ;
	
	if ( (total_col_count)%512 == 0 ){
		row_per_frame = (total_col_count)/512 ;		
	}
	else {
		row_per_frame = (total_col_count)/512 + 1 ;
	}		
	
	row_per_frame++ ; // guard space
	
	return row_per_frame ;
	
}

//--------------------------------------------------------------------------------------------------------------------------
static void Init_ARBITER_PARAM( PDDRMAP_CTRL_PARAM pCTRL )
{
	pCTRL->stARBITER.refresh_prog_on = 1 ;
	pCTRL->stARBITER.refresh_prog_count = 224 ;
	
	pCTRL->stARBITER.refresh_en_n = 0 ;
	pCTRL->stARBITER.refresh_mode = 0 ;
	pCTRL->stARBITER.low_speed_mode = 0 ;
	pCTRL->stARBITER.priority_scheme_n = 0 ;
	
#if __ARBITER_DDR_CONFIG__ == 11 //arbiter
	pCTRL->stARBITER.fc_pri = 8 ;
	pCTRL->stARBITER.osd_pri = 4 ;
	pCTRL->stARBITER.host_pri = 2 ;
	pCTRL->stARBITER.mcu_pri = 0 ;
	
	pCTRL->stARBITER.fc_stv = 3 ;
	pCTRL->stARBITER.osd_stv = 7 ;
	pCTRL->stARBITER.host_stv = 15 ;
	pCTRL->stARBITER.mcu_stv = 8 ;
#else //arbiter_lpddr2
	pCTRL->stARBITER.fc_pri = 3 ;	//2-bit
	pCTRL->stARBITER.osd_pri = 2 ;	//2-bit
	pCTRL->stARBITER.host_pri = 1 ;	//2-bit
	pCTRL->stARBITER.mcu_pri = 0 ;	//4-bit
	
	pCTRL->stARBITER.fc_stv = 3 ;	//3-bit
	pCTRL->stARBITER.osd_stv = 7 ;	//3-bit
	pCTRL->stARBITER.host_stv = 7 ;	//3-bit
	pCTRL->stARBITER.mcu_stv = 8 ;	//4-bit
#endif
	
	pCTRL->stARBITER.reset_req = 1 ;
	pCTRL->stARBITER.access_en = 0 ;
	pCTRL->stARBITER.drv_strength = 1 ;
}

//--------------------------------------------------------------------------------------------------------------------------
static void Init_DDRMAP_PARAM( PDDRMAP_CTRL_PARAM pCTRL )
{
#if   defined(__USE_DDR_1G__)
	pCTRL->ddr_size = 2 ;		// 0: 512M	1: 256M, 2: 1G
#elif defined(__USE_DDR_512M__)
	pCTRL->ddr_size = 0 ;		// 0: 512M	1: 256M, 2: 1G
#else
	pCTRL->ddr_size = 1 ;		// 0: 512M	1: 256M, 2: 1G
#endif
	
	//////////////////////////////////////////////////
	pCTRL->stDDRMAP.bounadry_hsize = GetSS_image_boundary_size_h() ;
	pCTRL->stDDRMAP.bounadry_vsize = GetSS_image_boundary_size_v() ;
	
	pCTRL->stDDRMAP.active_hsize = GetSS_image_active_size_h() ;
	pCTRL->stDDRMAP.active_vsize = GetSS_image_active_size_v() ;		
		
	if ( ( GetIN_MainPath() == MISP_MAINPATH_SENSOR ) || ( GetIN_MainPath() == MISP_MAINPATH_SENSOR_EXT_P1 ) )
	{		
		pCTRL->stDDRMAP.main_hsize = pCTRL->stDDRMAP.active_hsize ;
		pCTRL->stDDRMAP.main_vsize = pCTRL->stDDRMAP.active_vsize ;
	}
	else
	{
		pCTRL->stDDRMAP.main_hsize = GetIN_MainSrcSizeH() ;
		pCTRL->stDDRMAP.main_vsize = GetIN_MainSrcSizeV() ;	
	}
	
	if ( GetIN_AuxPath() == MISP_AUXPATH_SENSOR )
	{
#if defined(__MDIN_i550_USB_V100__)
		pCTRL->stDDRMAP.aux_hsize = GetIN_AuxSrcSizeH() ;
		pCTRL->stDDRMAP.aux_vsize = GetIN_AuxSrcSizeV() ;
#else
		pCTRL->stDDRMAP.aux_hsize = MAX(960, ((GetSS_image_active_size_h()>>1)) ) ;
		pCTRL->stDDRMAP.aux_vsize = MAX(540, ((GetSS_image_active_size_v()>>1)) ) ;	
#endif
	}
	else
	{
		pCTRL->stDDRMAP.aux_hsize = GetIN_AuxSrcSizeH() ;
		pCTRL->stDDRMAP.aux_vsize = GetIN_AuxSrcSizeV() ;		
	}

#if __MEMORY_MAP_REV__		
	pCTRL->stDDRMAP.rotation_en = GetROT_RotationEnable() ;
	pCTRL->stDDRMAP.IPC_444_en = 0 ;
	pCTRL->stDDRMAP.RGB_gamma_en = GetBY_gamma_mode() ;
#else
	pCTRL->stDDRMAP.rotation_en = 0 ;
	pCTRL->stDDRMAP.IPC_444_en = 0 ;	
	pCTRL->stDDRMAP.RGB_gamma_en = 0 ;
#endif
	
	pCTRL->stDDRMAP.vertical_flip_en = 0 ;
	pCTRL->stDDRMAP.DIS_en = 0 ;
	// pCTRL->stDDRMAP.NR_image_type = MISP_NR_BAYER_IMAGE ;
	pCTRL->stDDRMAP.NR_image_type = GetNR_Inputimage_type() ;
	
	pCTRL->stDDRMAP.DIS_frame_count = GETIN_MainFRcntY() ; 
	pCTRL->stDDRMAP.IPC_frame_count = GETIN_MainFRcntY()  ; 
	pCTRL->stDDRMAP.IPC_frame_count_c = GETIN_MainFRcntC()  ; 
	pCTRL->stDDRMAP.AUX_frame_count = GetIN_AuxFRcnt() ; 
	pCTRL->stDDRMAP.AUX_frame_count_c = GetIN_AuxFRcnt() ; 
	
	pCTRL->stDDRMAP.NR_frame_count = 1 ; 
	pCTRL->stDDRMAP.WDRRW_frame_count = 0 ; 
	pCTRL->stDDRMAP.WDRSW_frame_count = 0 ; 
	
	pCTRL->stDDRMAP.IPC_bit_precision = (GetIN_MainMode()%2) ; // EVEN mode for 8b , ODD mode for 10b
	pCTRL->stDDRMAP.NR_bit_precision = GetNR_BitMode() ;
		
#if __MEMORY_MAP_REV__
	pCTRL->stDDRMAP.AUX_bit_precision = (GetIN_AuxMode()%2) ; // EVEN mode for 8b , ODD mode for 10b
	pCTRL->stDDRMAP.ROT_bit_precision = GetROT_BitMode() ;
	pCTRL->stDDRMAP.ROT_frame_count = 2 ;
#else
	pCTRL->stDDRMAP.AUX_bit_precision = 0 ; // EVEN mode for 8b , ODD mode for 10b
	pCTRL->stDDRMAP.ROT_bit_precision = 0 ;
	pCTRL->stDDRMAP.ROT_frame_count = 0 ;
#endif	
	
	pCTRL->stDDRMAP.WDR_SW_bit_precision = GetSS_sensor_bit_opertion() ;
	pCTRL->stDDRMAP.WDR_RW_bit_precision = 0 ;

	//////////////////////////////////////////////////
	SetDDRMAP(&pCTRL->stDDRMAP) ;
	//////////////////////////////////////////////////
	
	// DDRMAPCTRL_DDRMAP_info(&pCTRL->stDDRMAP) ;
}

//--------------------------------------------------------------------------------------------------------------------------
static void Init_DDRMAP_REGISTER( PDDRMAP_CTRL_PARAM pCTRL )
{
#if __ARBITER_DDR_CONFIG__ == 11 //arbiter
	MISP_RegWrite(MISP_LOCAL_ID, _MEM_CONFIG_, 0x4000);	// set DDR
#else //arbiter_lpddr2
	MISP_RegWrite(MISP_LOCAL_ID, _MEM_CONFIG_, 0x4020);	// set DDR,	[6:5]mrr_enable_delay_long=1
#endif

#if __MEMORY_MAP_REV__	

	MISP_RegWrite(MISP_LOCAL_ID, _MAP_F_COUNT_0_, (pCTRL->stMRU4_WDR_RW.mo_map.frame_cnt<<12)|(pCTRL->stMRU1_IPC.frame_cnt_y<<8)|(pCTRL->stMRU2_AUX.frame_cnt_y<<4)|(pCTRL->stMRU3_NR.frame_cnt_y));	// map count
	MISP_RegWrite(MISP_LOCAL_ID, _MAP_F_COUNT_4_, (pCTRL->stMRU4_WDR_RW.frame_cnt_y<<4)|(pCTRL->stMRU5_WDR_SW.frame_cnt_y));	//
	MISP_RegWrite(MISP_LOCAL_ID, _MAP_F_COUNT_0t_, (0<<12)|(pCTRL->stMRU1_IPC.frame_cnt_c<<8)|(pCTRL->stMRU2_AUX.frame_cnt_c<<4)|(pCTRL->stMRU3_NR.frame_cnt_c));	//
	MISP_RegWrite(MISP_LOCAL_ID, _MAP_F_COUNT_4t_, (pCTRL->stMRU4_WDR_RW.mo_map.frame_cnt<<4)|(pCTRL->stMRU5_WDR_SW.frame_cnt_c));	//

	MISP_RegWrite(MISP_LOCAL_ID, _MAP0_START_ROW_, pCTRL->stMRU4_WDR_RW.mo_map.start_row_addr_mo);	// map0 start addr -- dis
	MISP_RegWrite(MISP_LOCAL_ID, _MAP1_START_ROW_, pCTRL->stMRU1_IPC.start_row_addr_y);	// map1 start addr -- inp
	MISP_RegWrite(MISP_LOCAL_ID, _MAP2_START_ROW_, pCTRL->stMRU2_AUX.start_row_addr_y);	// map2 start addr -- aux
	MISP_RegWrite(MISP_LOCAL_ID, _MAP3_START_ROW_, pCTRL->stMRU3_NR.start_row_addr_y);	// map3 start addr -- nr
	MISP_RegWrite(MISP_LOCAL_ID, _MAP4_START_ROW_, pCTRL->stMRU4_WDR_RW.start_row_addr_y);	// map4 start addr -- WDR result : NO ALLOC
	MISP_RegWrite(MISP_LOCAL_ID, _MAP5_START_ROW_, pCTRL->stMRU5_WDR_SW.start_row_addr_y);	// map5 start addr -- WDR sensor : NO ALLOC
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAP0t_START_ROW_, pCTRL->stMRU4_WDR_RW.mo_map.start_row_addr_mohist);	// map0 start addr -- dis : NO ALLOC
	MISP_RegWrite(MISP_LOCAL_ID, _MAP1t_START_ROW_, pCTRL->stMRU1_IPC.start_row_addr_c);	// map1 start addr -- inp
	MISP_RegWrite(MISP_LOCAL_ID, _MAP2t_START_ROW_, pCTRL->stMRU2_AUX.start_row_addr_c);	// map2 start addr -- aux 
	MISP_RegWrite(MISP_LOCAL_ID, _MAP3t_START_ROW_, pCTRL->stMRU3_NR.start_row_addr_c);	// map3 start addr -- nr
	MISP_RegWrite(MISP_LOCAL_ID, _MAP4t_START_ROW_, pCTRL->stMRU4_WDR_RW.mo_map.start_row_addr_mo);	// map4 start addr -- WDR result : NO ALLOC
	MISP_RegWrite(MISP_LOCAL_ID, _MAP5t_START_ROW_, pCTRL->stMRU5_WDR_SW.start_row_addr_c);	// map5 start addr -- WDR sensor : NO ALLOC 	
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAP0_ROW_COUNT_PER_FRAME_, pCTRL->stMRU4_WDR_RW.mo_map.row_count_per_frame);	// map0_row_count_per_frame : dis
	MISP_RegWrite(MISP_LOCAL_ID, _MAP1_ROW_COUNT_PER_FRAME_, pCTRL->stMRU1_IPC.row_count_per_frame);	// map1_row_count_per_frame : inp
	MISP_RegWrite(MISP_LOCAL_ID, _MAP2_ROW_COUNT_PER_FRAME_, pCTRL->stMRU2_AUX.row_count_per_frame);	// map2_row_count_per_frame	: aux
	MISP_RegWrite(MISP_LOCAL_ID, _MAP3_ROW_COUNT_PER_FRAME_, pCTRL->stMRU3_NR.row_count_per_frame);	// map3_row_count_per_frame	: nr
	MISP_RegWrite(MISP_LOCAL_ID, _MAP4_ROW_COUNT_PER_FRAME_, pCTRL->stMRU4_WDR_RW.row_count_per_frame);	// map4_row_count_per_frame	: WDR result	   
	MISP_RegWrite(MISP_LOCAL_ID, _MAP5_ROW_COUNT_PER_FRAME_, pCTRL->stMRU5_WDR_SW.row_count_per_frame);	// map5_row_count_per_frame	: WDR sensor
																   
	MISP_RegWrite(MISP_LOCAL_ID, _MAP0_COL_COUNT_PER_LINE_, (pCTRL->stMRU4_WDR_RW.mo_map.bit_precision<<10)|(pCTRL->stMRU4_WDR_RW.mo_map.col_count_per_line));	// map0_col_count_per_line / bit_precision0 -- dis , 10bit
	MISP_RegWrite(MISP_LOCAL_ID, _MAP1_COL_COUNT_PER_LINE_, (pCTRL->stMRU1_IPC.bit_precision<<10)|(pCTRL->stMRU1_IPC.col_count_per_line));	// map1_col_count_per_line / bit_precision1 -- inp, ipc 10bit 
	MISP_RegWrite(MISP_LOCAL_ID, _MAP2_COL_COUNT_PER_LINE_, (pCTRL->stMRU2_AUX.bit_precision<<10)|(pCTRL->stMRU2_AUX.col_count_per_line));	// map2_col_count_per_line / bit_precision2 -- AUX 8bit
	MISP_RegWrite(MISP_LOCAL_ID, _MAP3_COL_COUNT_PER_LINE_, (pCTRL->stMRU3_NR.bit_precision<<10)|(pCTRL->stMRU3_NR.col_count_per_line));	// map3_col_count_per_line / bit_precision3 -- NR 10bit
	MISP_RegWrite(MISP_LOCAL_ID, _MAP4_COL_COUNT_PER_LINE_, (pCTRL->stMRU4_WDR_RW.bit_precision<<10)|(pCTRL->stMRU4_WDR_RW.col_count_per_line));	// map4_col_count_per_line / bit_precision4 -- WDR result : NO ALLOC 
	MISP_RegWrite(MISP_LOCAL_ID, _MAP5_COL_COUNT_PER_LINE_, (pCTRL->stMRU5_WDR_SW.bit_precision<<10)|(pCTRL->stMRU5_WDR_SW.col_count_per_line));	// map5_col_count_per_line / bit_precision5 -- WDR SENSOR : NO ALLOC
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_1_, (pCTRL->stMRU1_IPC.mapping_write_y<<12)|(pCTRL->stMRU1_IPC.mapping_read_y<<8)|(pCTRL->stMRU1_IPC.mapping_write_c<<4)|(pCTRL->stMRU1_IPC.mapping_read_c));	// mru 1 inp, ipc //0x0088
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_2_, (pCTRL->stMRU2_AUX.mapping_write_y<<12)|(pCTRL->stMRU2_AUX.mapping_read_y<<8)|(pCTRL->stMRU2_AUX.mapping_write_c<<4)|(pCTRL->stMRU2_AUX.mapping_read_c));	// mru 2 aux //0x44cc
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_5_, (pCTRL->stMRU5_WDR_SW.mapping_write_y<<12)|(pCTRL->stMRU5_WDR_SW.mapping_read_y<<8)|(pCTRL->stMRU5_WDR_SW.mapping_write_c<<4)|(pCTRL->stMRU5_WDR_SW.mapping_read_c));	// mru 5 wdr sonsor //0x55FF

	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_4_, (pCTRL->stMRU4_WDR_RW.mapping_write_y<<12)|(pCTRL->stMRU4_WDR_RW.mapping_read_y<<8)|(pCTRL->stMRU4_WDR_RW.mapping_write_mo<<4)|0x000F);	// mru 4 wdr result //0x11FF
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_0A_, (pCTRL->stMRU3_NR.mapping_write_y<<12)|(pCTRL->stMRU3_NR.mapping_read_y<<8)|(pCTRL->stMRU3_NR.mapping_write_c<<4)|(pCTRL->stMRU3_NR.mapping_read_c));	// mru 0 DIS //0x00FF
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_0B_, (pCTRL->stMRU4_WDR_RW.mapping_read_mo<<8)|0xF000|(pCTRL->stMRU4_WDR_RW.mapping_write_mohist<<4)|(pCTRL->stMRU4_WDR_RW.mapping_read_mohist));	// mru 0 DIS //0x00FF

	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_3_, (pCTRL->stMRU0_DIS.mapping_write_y<<12)|(pCTRL->stMRU0_DIS.mapping_read_y<<8)|(pCTRL->stMRU0_DIS.mapping_write_c<<4)|(pCTRL->stMRU0_DIS.mapping_read_c));	// mru 3 nr //0x33bb

	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_6_, 0xFFFF);	// mru 6 not using
	
	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_0_, (pCTRL->stMRU3_NR.write_pri<<12)|(pCTRL->stMRU1_IPC.write_pri<<8)|(pCTRL->stMRU2_AUX.write_pri<<4)|(pCTRL->stMRU0_DIS.write_pri));	// pri_starv_0 

	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_1_, (pCTRL->stMRU3_NR.read_pri<<12)|(pCTRL->stMRU1_IPC.read_pri<<8)|(pCTRL->stMRU2_AUX.read_pri<<4)|(pCTRL->stMRU0_DIS.read_pri));	// pri_starv_1 

	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_2_, (pCTRL->stMRU3_NR.write_stv<<12)|(pCTRL->stMRU1_IPC.write_stv<<8)|(pCTRL->stMRU2_AUX.write_stv<<4)|(pCTRL->stMRU0_DIS.write_stv));	// pri_starv_2 

	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_3_, (pCTRL->stMRU3_NR.read_stv<<12)|(pCTRL->stMRU1_IPC.read_stv<<8)|(pCTRL->stMRU2_AUX.read_stv<<4)|(pCTRL->stMRU0_DIS.read_stv));	// pri_starv_3 

	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_4_, (pCTRL->stMRU4_WDR_RW.write_pri<<12)|(pCTRL->stMRU4_WDR_RW.read_pri<<8)|(pCTRL->stMRU4_WDR_RW.write_stv<<4)|(pCTRL->stMRU4_WDR_RW.read_stv));	// pri_starv_4 

	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_5_, (pCTRL->stMRU5_WDR_SW.write_pri<<12)|(pCTRL->stMRU5_WDR_SW.read_pri<<8)|(pCTRL->stMRU5_WDR_SW.write_stv<<4)|(pCTRL->stMRU5_WDR_SW.read_stv));	// pri_starv_5

	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_6_, 0x7777);	// pri_starv_6 
#else

	MISP_RegWrite(MISP_LOCAL_ID, _MAP_F_COUNT_0_, (pCTRL->stMRU0_DIS.frame_cnt_y<<12)|(pCTRL->stMRU1_IPC.frame_cnt_y<<8)|(pCTRL->stMRU2_AUX.frame_cnt_y<<4)|(pCTRL->stMRU3_NR.frame_cnt_y));	// map count
	MISP_RegWrite(MISP_LOCAL_ID, _MAP_F_COUNT_4_, (pCTRL->stMRU4_WDR_RW.frame_cnt_y<<4)|(pCTRL->stMRU5_WDR_SW.frame_cnt_y));	//
	MISP_RegWrite(MISP_LOCAL_ID, _MAP_F_COUNT_0t_, (pCTRL->stMRU0_DIS.frame_cnt_c<<12)|(pCTRL->stMRU1_IPC.frame_cnt_c<<8)|(pCTRL->stMRU2_AUX.frame_cnt_c<<4)|(pCTRL->stMRU3_NR.frame_cnt_c));	//
	MISP_RegWrite(MISP_LOCAL_ID, _MAP_F_COUNT_4t_, (pCTRL->stMRU4_WDR_RW.frame_cnt_c<<4)|(pCTRL->stMRU5_WDR_SW.frame_cnt_c));	//	
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAP0_START_ROW_, pCTRL->stMRU0_DIS.start_row_addr_y);	// map0 start addr -- dis
	MISP_RegWrite(MISP_LOCAL_ID, _MAP1_START_ROW_, pCTRL->stMRU1_IPC.start_row_addr_y);	// map1 start addr -- inp
	MISP_RegWrite(MISP_LOCAL_ID, _MAP2_START_ROW_, pCTRL->stMRU2_AUX.start_row_addr_y);	// map2 start addr -- aux
	MISP_RegWrite(MISP_LOCAL_ID, _MAP3_START_ROW_, pCTRL->stMRU3_NR.start_row_addr_y);	// map3 start addr -- nr
	MISP_RegWrite(MISP_LOCAL_ID, _MAP4_START_ROW_, pCTRL->stMRU4_WDR_RW.start_row_addr_y);	// map4 start addr -- WDR result : NO ALLOC
	MISP_RegWrite(MISP_LOCAL_ID, _MAP5_START_ROW_, pCTRL->stMRU5_WDR_SW.start_row_addr_y);	// map5 start addr -- WDR sensor : NO ALLOC
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAP0t_START_ROW_, pCTRL->stMRU0_DIS.start_row_addr_c);	// map0 start addr -- dis : NO ALLOC
	MISP_RegWrite(MISP_LOCAL_ID, _MAP1t_START_ROW_, pCTRL->stMRU1_IPC.start_row_addr_c);	// map1 start addr -- inp
	MISP_RegWrite(MISP_LOCAL_ID, _MAP2t_START_ROW_, pCTRL->stMRU2_AUX.start_row_addr_c);	// map2 start addr -- aux 
	MISP_RegWrite(MISP_LOCAL_ID, _MAP3t_START_ROW_, pCTRL->stMRU3_NR.start_row_addr_c);	// map3 start addr -- nr
	MISP_RegWrite(MISP_LOCAL_ID, _MAP4t_START_ROW_, pCTRL->stMRU4_WDR_RW.start_row_addr_c);	// map4 start addr -- WDR result : NO ALLOC
	MISP_RegWrite(MISP_LOCAL_ID, _MAP5t_START_ROW_, pCTRL->stMRU5_WDR_SW.start_row_addr_c);	// map5 start addr -- WDR sensor : NO ALLOC 	
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAP0_ROW_COUNT_PER_FRAME_, pCTRL->stMRU0_DIS.row_count_per_frame);	// map0_row_count_per_frame : dis
	MISP_RegWrite(MISP_LOCAL_ID, _MAP1_ROW_COUNT_PER_FRAME_, pCTRL->stMRU1_IPC.row_count_per_frame);	// map1_row_count_per_frame : inp
	MISP_RegWrite(MISP_LOCAL_ID, _MAP2_ROW_COUNT_PER_FRAME_, pCTRL->stMRU2_AUX.row_count_per_frame);	// map2_row_count_per_frame	: aux
	MISP_RegWrite(MISP_LOCAL_ID, _MAP3_ROW_COUNT_PER_FRAME_, pCTRL->stMRU3_NR.row_count_per_frame);	// map3_row_count_per_frame	: nr
	MISP_RegWrite(MISP_LOCAL_ID, _MAP4_ROW_COUNT_PER_FRAME_, pCTRL->stMRU4_WDR_RW.row_count_per_frame);	// map4_row_count_per_frame	: WDR result	   
	MISP_RegWrite(MISP_LOCAL_ID, _MAP5_ROW_COUNT_PER_FRAME_, pCTRL->stMRU5_WDR_SW.row_count_per_frame);	// map5_row_count_per_frame	: WDR sensor
																   
	MISP_RegWrite(MISP_LOCAL_ID, _MAP0_COL_COUNT_PER_LINE_, (pCTRL->stMRU0_DIS.bit_precision<<10)|(pCTRL->stMRU0_DIS.col_count_per_line));	// map0_col_count_per_line / bit_precision0 -- dis , 10bit
	MISP_RegWrite(MISP_LOCAL_ID, _MAP1_COL_COUNT_PER_LINE_, (pCTRL->stMRU1_IPC.bit_precision<<10)|(pCTRL->stMRU1_IPC.col_count_per_line));	// map1_col_count_per_line / bit_precision1 -- inp, ipc 10bit 
	MISP_RegWrite(MISP_LOCAL_ID, _MAP2_COL_COUNT_PER_LINE_, (pCTRL->stMRU2_AUX.bit_precision<<10)|(pCTRL->stMRU2_AUX.col_count_per_line));	// map2_col_count_per_line / bit_precision2 -- AUX 8bit
	MISP_RegWrite(MISP_LOCAL_ID, _MAP3_COL_COUNT_PER_LINE_, (pCTRL->stMRU3_NR.bit_precision<<10)|(pCTRL->stMRU3_NR.col_count_per_line));	// map3_col_count_per_line / bit_precision3 -- NR 10bit
	MISP_RegWrite(MISP_LOCAL_ID, _MAP4_COL_COUNT_PER_LINE_, (pCTRL->stMRU4_WDR_RW.bit_precision<<10)|(pCTRL->stMRU4_WDR_RW.col_count_per_line));	// map4_col_count_per_line / bit_precision4 -- WDR result : NO ALLOC 
	MISP_RegWrite(MISP_LOCAL_ID, _MAP5_COL_COUNT_PER_LINE_, (pCTRL->stMRU5_WDR_SW.bit_precision<<10)|(pCTRL->stMRU5_WDR_SW.col_count_per_line));	// map5_col_count_per_line / bit_precision5 -- WDR SENSOR : NO ALLOC
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_0A_, (pCTRL->stMRU0_DIS.mapping_write_y<<12)|(pCTRL->stMRU0_DIS.mapping_read_y<<8)|(pCTRL->stMRU0_DIS.mapping_write_c<<4)|(pCTRL->stMRU0_DIS.mapping_read_c));	// mru 0 DIS //0x00FF
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_0B_, 0xFFFF);	// mru 0B not using
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_1_, (pCTRL->stMRU1_IPC.mapping_write_y<<12)|(pCTRL->stMRU1_IPC.mapping_read_y<<8)|(pCTRL->stMRU1_IPC.mapping_write_c<<4)|(pCTRL->stMRU1_IPC.mapping_read_c));	// mru 1 inp, ipc //0x0088
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_2_, (pCTRL->stMRU2_AUX.mapping_write_y<<12)|(pCTRL->stMRU2_AUX.mapping_read_y<<8)|(pCTRL->stMRU2_AUX.mapping_write_c<<4)|(pCTRL->stMRU2_AUX.mapping_read_c));	// mru 2 aux //0x44cc
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_3_, (pCTRL->stMRU3_NR.mapping_write_y<<12)|(pCTRL->stMRU3_NR.mapping_read_y<<8)|(pCTRL->stMRU3_NR.mapping_write_c<<4)|(pCTRL->stMRU3_NR.mapping_read_c));	// mru 3 nr //0x33bb
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_4_, (pCTRL->stMRU4_WDR_RW.mapping_write_y<<12)|(pCTRL->stMRU4_WDR_RW.mapping_read_y<<8)|(pCTRL->stMRU4_WDR_RW.mapping_write_mo<<4)|(pCTRL->stMRU4_WDR_RW.mapping_read_mo));	// mru 4 wdr result //0x11FF
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_5_, (pCTRL->stMRU5_WDR_SW.mapping_write_y<<12)|(pCTRL->stMRU5_WDR_SW.mapping_read_y<<8)|(pCTRL->stMRU5_WDR_SW.mapping_write_c<<4)|(pCTRL->stMRU5_WDR_SW.mapping_read_c));	// mru 5 wdr sonsor //0x55FF
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_6_, 0xFFFF);	// mru 6 not using
	
	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_0_, (pCTRL->stMRU0_DIS.write_pri<<12)|(pCTRL->stMRU1_IPC.write_pri<<8)|(pCTRL->stMRU2_AUX.write_pri<<4)|(pCTRL->stMRU3_NR.write_pri));	// pri_starv_0 
	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_1_, (pCTRL->stMRU0_DIS.read_pri<<12)|(pCTRL->stMRU1_IPC.read_pri<<8)|(pCTRL->stMRU2_AUX.read_pri<<4)|(pCTRL->stMRU3_NR.read_pri));	// pri_starv_1 
	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_2_, (pCTRL->stMRU0_DIS.write_stv<<12)|(pCTRL->stMRU1_IPC.write_stv<<8)|(pCTRL->stMRU2_AUX.write_stv<<4)|(pCTRL->stMRU3_NR.write_stv));	// pri_starv_2 
	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_3_, (pCTRL->stMRU0_DIS.read_stv<<12)|(pCTRL->stMRU1_IPC.read_stv<<8)|(pCTRL->stMRU2_AUX.read_stv<<4)|(pCTRL->stMRU3_NR.read_stv));	// pri_starv_3 
	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_4_, (pCTRL->stMRU4_WDR_RW.write_pri<<12)|(pCTRL->stMRU4_WDR_RW.read_pri<<8)|(pCTRL->stMRU4_WDR_RW.write_stv<<4)|(pCTRL->stMRU4_WDR_RW.read_stv));	// pri_starv_4 
	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_5_, (pCTRL->stMRU5_WDR_SW.write_pri<<12)|(pCTRL->stMRU5_WDR_SW.read_pri<<8)|(pCTRL->stMRU5_WDR_SW.write_stv<<4)|(pCTRL->stMRU5_WDR_SW.read_stv));	// pri_starv_5
	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_6_, 0x7777);	// pri_starv_6 
	
#endif
	MISP_RegWrite(MISP_LOCAL_ID, _FC_MC_MON_, 0x8000);	//	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDDR_WriteCommand(BYTE mreg_ma, BYTE mreg_op) 	// for mode register writing with arbiter_lpddr2
{
	MISP_RegWrite(MISP_LOCAL_ID, 0x1AA, (U16)mreg_op<<8 | (U16)mreg_ma);
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A5, 0x0403);	//clear mode register write/read
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A5, 0x8403);	//mode register write
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A5, 0x0403);	//clear mode register write/read
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDDR_WritePhyReg(U16 addr, U32 data) 	// for ddr_phy register writing with arbiter_lpddr2
{
	MISP_RegField(MISP_LOCAL_ID, 0x1AB,  0, 11, addr); 	// addr[10:0]
	MISP_RegWrite(MISP_LOCAL_ID, 0x1AD,  LOWORD(data)); 	// data[15:0]
	MISP_RegWrite(MISP_LOCAL_ID, 0x1AE,  HIWORD(data)); 	// data[31:16]
	MISP_RegField(MISP_LOCAL_ID, 0x1AC,  8,  2,    3); 	// phy reg write access,  [9:8] = '0 -> 3'
	MISP_RegField(MISP_LOCAL_ID, 0x1AC,  8,  2,    0); 	// clear phy reg access,  [9:8] = '3 -> 0'
}

//--------------------------------------------------------------------------------------------------------------------------
static void Init_ARBITER_REGISTER(PDDRMAP_CTRL_PARAM pCTRL )
{
#if defined(__MISP_i5XX_FPGA__)						// FPGA board
#if defined(__USE_DDR_MCLK99__) || defined (__USE_DDR_MCLK108__)
	MISP_RegField(MISP_HOST_ID, 0x090, 1, 1, 1);	// dfi iusrst
	MISP_RegField(MISP_HOST_ID, 0x090, 2, 1, 1);	// dfi idllrst
#endif		

#if defined(__USE_DDR_MCLK162__) || defined (__USE_DDR_MCLK180__) 
	MISP_RegField(MISP_HOST_ID, 0x090, 1, 1, 0);	// dfi iusrst
	MISP_RegField(MISP_HOST_ID, 0x090, 2, 1, 0);	// dfi idllrst
#endif

#if defined (__USE_DDR_MCLK198__)
	MISP_RegField(MISP_HOST_ID, 0x090, 1, 1, 0);	// dfi iusrst
	MISP_RegField(MISP_HOST_ID, 0x090, 2, 1, 1);	// dfi idllrst
#endif	
	
	
#else												// EDK & REF board
	// ddr_init
	MISP_RegWrite(MISP_HOST_ID, 0x094, 0x072f);		// dfi isft
	MISP_RegField(MISP_HOST_ID, 0x090, 0, 1, 1);	// dfi ireset
	MISP_RegField(MISP_HOST_ID, 0x090, 1, 1, 1);	// dfi iusrst
	MISP_RegField(MISP_HOST_ID, 0x090, 2, 1, 1);	// dfi idllrst
	Delay_mSec(1);
	
	MISP_RegWrite(MISP_HOST_ID,  0x093, 0x2f80);	// latency init
#endif	//__MISP_i5XX_FPGA__

	
		
#if __ARBITER_DDR_CONFIG__ == 11	//initialize LPDDR1 SDRAM with arbiter
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A0, (pCTRL->stARBITER.refresh_prog_count<<8)|
										(pCTRL->stARBITER.refresh_prog_on<<7)|
										(pCTRL->stARBITER.refresh_en_n<<6)|
										(pCTRL->stARBITER.refresh_mode<<2)|
										(pCTRL->stARBITER.low_speed_mode<<1)|
										(pCTRL->stARBITER.priority_scheme_n));	// DDR	
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A1, (pCTRL->stARBITER.fc_pri<<8)|
										(pCTRL->stARBITER.osd_pri<<4)|
										(pCTRL->stARBITER.host_pri));	// DDR	
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A2, (pCTRL->stARBITER.fc_stv<<8)|
										(pCTRL->stARBITER.osd_stv<<4)|
										(pCTRL->stARBITER.host_stv));	// DDR	
										
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A3, 0x0377);	// DDR	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A5, (pCTRL->stARBITER.mcu_pri<<12)|
										(pCTRL->stARBITER.mcu_stv<<8)|0x0000);	// DDR	
										
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A6, 0x0000);	// DDR	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A7, (pCTRL->stARBITER.reset_req<<14)|(pCTRL->stARBITER.access_en<<13)|(pCTRL->stARBITER.drv_strength<<2)|0x0000);	// DDR	

#elif __ARBITER_DDR_CONFIG__ == 21	//initialize LPDDR1 SDRAM with arbiter_lpddr2
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A0, (pCTRL->stARBITER.refresh_prog_count<<8)|
										(pCTRL->stARBITER.refresh_prog_on<<7)|
										(pCTRL->stARBITER.refresh_en_n<<6)|
										(pCTRL->stARBITER.refresh_mode<<2)|
										//(pCTRL->stARBITER.low_speed_mode<<1)|	=> rd_dataen_5
										(pCTRL->stARBITER.priority_scheme_n));
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A1, 1<<15|	//precharge_per_bank_en
										6<<12|	//trp_count_val
										0<<11|	//wr_latency_4
										0<<10|	//wr_latency_4_buf
										1<<9|	//fast_next_act_en
										1<<8|	//fast_next_act_dwr_en
										1<<6|	//cab_count_val
										(pCTRL->stARBITER.fc_pri<<4)|
										(pCTRL->stARBITER.osd_pri<<2)|
										(pCTRL->stARBITER.host_pri<<0));
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A2, 0<<15|	//dfi_rddata_valid_use_en
										7<<12|	//pal_p_len_val
										0<<11|	//fast_next_act_dwr_small_wait_en
										(pCTRL->stARBITER.fc_stv<<8)|
										(pCTRL->stARBITER.osd_stv<<4)|
										(pCTRL->stARBITER.host_stv<<0));	

	MISP_RegWrite(MISP_LOCAL_ID, 0x1A3, 1<<14|	//c2pd_wr_len_val
										0<<13|	//force_col_7bit_ff
										0<<12|	//fast_col_acc_en_ff
										1<<11|	//refresh_high_pri_ff
										2<<4|	//axo_pri
										7<<0);	//axo_stv
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A6, (pCTRL->stARBITER.mcu_pri<<12)|
										(pCTRL->stARBITER.mcu_stv<<8));
	
									
	if (pCTRL->ddr_size == 1 ) { // 256Mb
		//ddr clock enable : to make ddr signal CKE active
		MISP_RegWrite(MISP_LOCAL_ID, 0x1A7, 1<<14|	//ddr_cke_enable
											0<<13|	//ddr_access_en
											1<<11|	//bank_3bit
											13);	//mreg_rd_valid_run_delay_ctrl
	}
	else { // 512Mb
		//ddr clock enable : to make ddr signal CKE active
		MISP_RegWrite(MISP_LOCAL_ID, 0x1A7, 1<<14|	//ddr_cke_enable
											0<<13|	//ddr_access_en
											0<<11|	//bank_3bit
											13);	//mreg_rd_valid_run_delay_ctrl
	}
										
	Delay_mSec(1);	//(idle time after CKE active)
	//precharge all before writing mode registers
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A5, 0x0403);	//clear pall_en(b13='0')
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A5, 0x2403);	//clear pall_en(b13='1')
	Delay_mSec(1);	//(idle time precharge)
	//write MRS (ddr_bank[1:0]&ddr_addr[12:0] = mreg_op[6:0]&mreg_ma[7:0])
	// ddr_addr : OM[12:7]-CL[6:4]-BT[3]-BL[2:0]
	//  OM : Operating Mode - all zero for Normal Operation
	//  CL : CAS Latency = "011" - CAS Latency is 3.
	//  BT : Burst Type = "0" - Sequential
	//  BL : Burst Length = "010" - Burst Length is 4.
	// OM=normal, CL=3, BT=Sequential, BL=4
	// ==> ddr_bank&ddr_addr = "00"&"000000_011_0_010" = 0x0032
	SetDDR_WriteCommand(0x32, 0x00);
	Delay_mSec(1);	//(idle time after MR set)
	// Extended Mode Register Setting
	// ddr_addr : "00000"-DS[7:5]-"00"-PASR[2:0]
	//  DS : Drive Strength = register value - "000" for Full Drive
	//  PASR : Partial Array Self Refresh = "000" - Self Refresh is not used.
	// DS=0, PASR=0
	// ==> ddr_bank&ddr_addr = "10"&"00000"&"000"&"01"&"000" = 0x4008
	SetDDR_WriteCommand(0x08, 0x40);
	Delay_mSec(1);	//(idle time after MR set)
	//ddr access enable
	//==> 0x1A7[13] will be set to '1' later

#else //initialize LPDDR2 SDRAM with arbiter_lpddr2
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A0, (pCTRL->stARBITER.refresh_prog_count<<8)|
										(pCTRL->stARBITER.refresh_prog_on<<7)|
										(pCTRL->stARBITER.refresh_en_n<<6)|
										(pCTRL->stARBITER.refresh_mode<<2)|
										//(pCTRL->stARBITER.low_speed_mode<<1)|	=> rd_dataen_5
										(pCTRL->stARBITER.priority_scheme_n));
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A1, 1<<15|	//precharge_per_bank_en
										6<<12|	//trp_count_val
										0<<11|	//wr_latency_4
										0<<10|	//wr_latency_4_buf
//										1<<9|	//fast_next_act_en
										0<<9|	//fast_next_act_en	//for gac error(retest with mask revisioned ESMT ddr mem !!)
										1<<8|	//fast_next_act_dwr_en
										1<<6|	//cab_count_val
										(pCTRL->stARBITER.fc_pri<<4)|
										(pCTRL->stARBITER.osd_pri<<2)|
										(pCTRL->stARBITER.host_pri<<0));
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A2, 1<<15|	//dfi_rddata_valid_use_en
										3<<12|	//pal_p_len_val
										0<<11|	//fast_next_act_dwr_small_wait_en
										(pCTRL->stARBITER.fc_stv<<8)|
										(pCTRL->stARBITER.osd_stv<<4)|
										(pCTRL->stARBITER.host_stv<<0));	

	MISP_RegWrite(MISP_LOCAL_ID, 0x1A3, 1<<14|	//c2pd_wr_len_val
										0<<13|	//force_col_7bit_ff
										0<<12|	//fast_col_acc_en_ff
										1<<11|	//refresh_high_pri_ff
										2<<4|	//axo_pri
										7<<0);	//axo_stv
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A6, (pCTRL->stARBITER.mcu_pri<<12)|
										(pCTRL->stARBITER.mcu_stv<<8));

	
	
// ddr phy init - new
	// phy regiser reset : dfi presetn release
	MISP_RegWrite(MISP_LOCAL_ID, 0x1AC, 0x2800); // (bit 13 : 0 -> 1)
	// phy configure register (mandatory)
	// (address 0x04 : bust length(bit 2) = '0'(defalut value is '1'), working mode(bit 1:0) = "11")
	SetDDR_WritePhyReg(0x0004, 0x00000003);
	
	// - phy configure register (optional)
	// (address 0x2C : CL(bit 7:4) = 6, AL(bit 3:0) = 0)
	SetDDR_WritePhyReg(0x002C, 0x00000060);

	SetDDR_WritePhyReg(0x0050, 0x00000006);	// CHIP_REV_DDR_TUNE_201703 : phy configure register : CK DLL delay control : vlaue 6 = 135 degree (default is value 0 = 0 degree)
	SetDDR_WritePhyReg(0x0058, 0x00000055); // CHIP_REV_DDR_TUNE_201703 : phy configure register : CK/CKB drive strength control : 0x55 (default is 0x22)

	// - phy system reset : dfi system_rstn release
	MISP_RegWrite(MISP_LOCAL_ID, 0x1AC, 0x3800); // (bit 12 : 0 -> 1)	

	// - phy soft reset : reset and release (optional)
	// (address 0x00 : bit 3:2 soft reset, active low)
	SetDDR_WritePhyReg(0x0000, 0xFFFFFFF3);
	SetDDR_WritePhyReg(0x0000, 0xFFFFFFFF);

	//	phy initialization
	MISP_RegWrite(MISP_LOCAL_ID, 0x1AC, 0x3000); // (bit 11 : 1 -> 0)
	Delay_mSec(1); // - wait 20 us (phy initialization time)

	// - ddr clock enable : to make ddr signal CKE active
#if defined(__USE_DDR_512M__)
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A7, 0x400D); // (bit 14 : 0 -> 1) -- 512Mb
#else
	MISP_RegWrite(MISP_LOCAL_ID, 0x1A7, 0x480D); // (bit 14 : 0 -> 1) -- 1Gb
#endif
	Delay_mSec(1); // - wait 200 us (idle time after CKE active)	

	// - phy calibration start (optional)
	// (address 0x08 : gating calibration control(bit 0), active high)
	SetDDR_WritePhyReg(0x0008, 0x00000001);
	Delay_mSec(1); // - wait 5 us (calibration time)

	// - phy calibration end (optional)
	// (address 0x08 : gating calibration control(bit 0), active high)
	SetDDR_WritePhyReg(0x0008, 0x00000000);
	Delay_mSec(2); // - wait 5 us (calibration time)
	
	// add by thorn, kw5210
//	- phy configure register (phy rx dll read dqs delay phase : byte 0)
// (address 0x0A0 : bit[1:0] A_DQS0, A_DQSB0)
	SetDDR_WritePhyReg(0x00A0, 0x00000002);
	Delay_mSec(2); // - wait 5 us (calibration time)
	
//	- phy configure register (phy rx dll read dqs delay phase : byte 1)
// (address 0x0A0 : bit[1:0] A_DQS0, A_DQSB0)
	SetDDR_WritePhyReg(0x00E0, 0x00000002);
	Delay_mSec(2); // - wait 5 us (calibration time)
	
//	- phy configure register (phy rx dll read dqs delay phase : byte 2)
// (address 0x0A0 : bit[1:0] A_DQS0, A_DQSB0)
	SetDDR_WritePhyReg(0x0120, 0x00000002);
	Delay_mSec(2); // - wait 5 us (calibration time)
	
//	- phy configure register (phy rx dll read dqs delay phase : byte 3)
// (address 0x0A0 : bit[1:0] A_DQS0, A_DQSB0)
	SetDDR_WritePhyReg(0x0160, 0x00000002);
	Delay_mSec(2); // - wait 5 us (calibration time)
	
// 20170327 : lpddr2 sdram mode register setting order is changed from before phy calibration to after that.
	// - RESET command for lpddr2 sdram mode register : indirect mode register write
	SetDDR_WriteCommand(0x3F, 0x00);
	Delay_mSec(1); // - wait 10 us (idle time after RESET command)	

	// - MR10 set for lpddr2 sdram mode register : indirect mode register write
	// (bit[7:0] - ZQ calibration code 0xFF(after init), 0xAB(long), 0x56(short), 0xC3(ZQ reset))
	SetDDR_WriteCommand(0x0A, 0xFF);
	Delay_mSec(1); // - wait 1 us (idle time after calibration command)

	// - MR1 set for lpddr2 sdram mode register : indirect mode register write
	// (bit[7:5] - nWR(=6) - b'100, bit4 - Wrap(=0) - b'0, bit3 - Seq(=0) - b'0, bit[2:0] - BL(=4) - b'010)
	SetDDR_WriteCommand(0x01, 0x82);
	
	// - MR2 set for lpddr2 sdram mode register : indirect mode register write
	// (bit[7:4] - RFU - b'0000, bit[3:0] - RL(=6) & WL(=3) - b'0100)
	SetDDR_WriteCommand(0x02, 0x04);

	// - MR3 set for lpddr2 sdram mode register : indirect mode register write
	// (bit[7:4] - RFU - b'0000, bit[3:0] - DS(=40 ohm) = b'0020)
	SetDDR_WriteCommand(0x03, 0x02);	// CHIP_REV_DDR_TUNE_201703 : DS : changed from value 4(60 ohm) to 2(40 ohm)
	Delay_mSec(1); // - wait 10 us (idle time after MR set)	

	// - ddr access enable
	//MISP_RegWrite(MISP_LOCAL_ID, 0x1A7, 0x680D); // (bit 13 : 0 -> 1)
	//==> 0x1A7[13] will be set to '1' later
#endif
}
//--------------------------------------------------------------------------------------------------------------------------
void Init_DDRMAP ( void )
{
	Init_DDRMAP_PARAM(&stDDRMAPCTRL);
	Init_DDRMAP_REGISTER(&stDDRMAPCTRL);
	
	stDDRMAPCTRL.ctrl = 0 ;
}

void Init_Arbiter ( void )
{
	memset(&stDDRMAPCTRL, 0, sizeof(DDRMAP_CTRL_PARAM));
	Init_ARBITER_PARAM(&stDDRMAPCTRL);	
	Init_ARBITER_REGISTER(&stDDRMAPCTRL);
}
//--------------------------------------------------------------------------------------------------------------------------

static void DDRMAPCTRL_ArbiterDDRRefreshHandler( PDDRMAP_CTRL_PARAM pCTRL )
{
	if ( (pCTRL->ctrl&DDRMAPCTRL_DDR_REFRESH_EN_N_UPDATE)== 0 ) return ;
	
	pCTRL->ctrl &= (~DDRMAPCTRL_DDR_REFRESH_EN_N_UPDATE);	
	
	MISP_RegField(MISP_LOCAL_ID, 0x1A0, 6, 1, (pCTRL->stARBITER.refresh_en_n));
}

//--------------------------------------------------------------------------------------------------------------------------

static void DDRMAPCTRL_ArbiterDDRAccessHandler( PDDRMAP_CTRL_PARAM pCTRL )
{
	if ( (pCTRL->ctrl&DDRMAPCTRL_DDR_ACCESS_EN_UPDATE) == 0 ) return ;
	
	pCTRL->ctrl &= (~DDRMAPCTRL_DDR_ACCESS_EN_UPDATE);	
	
	MISP_RegField(MISP_LOCAL_ID, 0x1A7, 13, 1, (pCTRL->stARBITER.access_en));
}


//--------------------------------------------------------------------------------------------------------------------------
static void DDRMAPCTRL_DIS_MappingUpdate(PDDRMAP_CTRL_PARAM pCTRL) 
{
	
#if __MEMORY_MAP_REV__
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_3_, (pCTRL->stMRU0_DIS.mapping_write_y<<12)|(pCTRL->stMRU0_DIS.mapping_read_y<<8)|(pCTRL->stMRU0_DIS.mapping_write_c<<4)|(pCTRL->stMRU0_DIS.mapping_read_c));	// mru 3 nr //0x33bb
#else
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_0A_, (pCTRL->stMRU0_DIS.mapping_write_y<<12)|(pCTRL->stMRU0_DIS.mapping_read_y<<8)|(pCTRL->stMRU0_DIS.mapping_write_c<<4)|(pCTRL->stMRU0_DIS.mapping_read_c));	
#endif

}


//--------------------------------------------------------------------------------------------------------------------------

static void DDRMAPCTRL_DDRMAPSetHandler( PDDRMAP_CTRL_PARAM pCTRL )
{
	
	if ( (pCTRL->ctrl&DDRMAPCTRL_FCMRU_UPDATE) == 0 ) return ;
	
	pCTRL->ctrl &= (~DDRMAPCTRL_FCMRU_UPDATE);
	
#if __MEMORY_MAP_REV__	

	MISP_RegWrite(MISP_LOCAL_ID, _MAP_F_COUNT_0_, (pCTRL->stMRU4_WDR_RW.mo_map.frame_cnt<<12)|(pCTRL->stMRU1_IPC.frame_cnt_y<<8)|(pCTRL->stMRU2_AUX.frame_cnt_y<<4)|(pCTRL->stMRU3_NR.frame_cnt_y));	// map count
	MISP_RegWrite(MISP_LOCAL_ID, _MAP_F_COUNT_4_, (pCTRL->stMRU4_WDR_RW.frame_cnt_y<<4)|(pCTRL->stMRU5_WDR_SW.frame_cnt_y));	//
	MISP_RegWrite(MISP_LOCAL_ID, _MAP_F_COUNT_0t_, (0<<12)|(pCTRL->stMRU1_IPC.frame_cnt_c<<8)|(pCTRL->stMRU2_AUX.frame_cnt_c<<4)|(pCTRL->stMRU3_NR.frame_cnt_c));	//
	MISP_RegWrite(MISP_LOCAL_ID, _MAP_F_COUNT_4t_, (pCTRL->stMRU4_WDR_RW.mo_map.frame_cnt<<4)|(pCTRL->stMRU5_WDR_SW.frame_cnt_c));	//

	MISP_RegWrite(MISP_LOCAL_ID, _MAP0_START_ROW_, pCTRL->stMRU4_WDR_RW.mo_map.start_row_addr_mo);	// map0 start addr -- dis
	MISP_RegWrite(MISP_LOCAL_ID, _MAP1_START_ROW_, pCTRL->stMRU1_IPC.start_row_addr_y);	// map1 start addr -- inp
	MISP_RegWrite(MISP_LOCAL_ID, _MAP2_START_ROW_, pCTRL->stMRU2_AUX.start_row_addr_y);	// map2 start addr -- aux
	MISP_RegWrite(MISP_LOCAL_ID, _MAP3_START_ROW_, pCTRL->stMRU3_NR.start_row_addr_y);	// map3 start addr -- nr
	MISP_RegWrite(MISP_LOCAL_ID, _MAP4_START_ROW_, pCTRL->stMRU4_WDR_RW.start_row_addr_y);	// map4 start addr -- WDR result : NO ALLOC
	MISP_RegWrite(MISP_LOCAL_ID, _MAP5_START_ROW_, pCTRL->stMRU5_WDR_SW.start_row_addr_y);	// map5 start addr -- WDR sensor : NO ALLOC
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAP0t_START_ROW_, pCTRL->stMRU4_WDR_RW.mo_map.start_row_addr_mohist);	// map0 start addr -- dis : NO ALLOC
	MISP_RegWrite(MISP_LOCAL_ID, _MAP1t_START_ROW_, pCTRL->stMRU1_IPC.start_row_addr_c);	// map1 start addr -- inp
	MISP_RegWrite(MISP_LOCAL_ID, _MAP2t_START_ROW_, pCTRL->stMRU2_AUX.start_row_addr_c);	// map2 start addr -- aux 
	MISP_RegWrite(MISP_LOCAL_ID, _MAP3t_START_ROW_, pCTRL->stMRU3_NR.start_row_addr_c);	// map3 start addr -- nr
	MISP_RegWrite(MISP_LOCAL_ID, _MAP4t_START_ROW_, pCTRL->stMRU4_WDR_RW.mo_map.start_row_addr_mo);	// map4 start addr -- WDR result : NO ALLOC
	MISP_RegWrite(MISP_LOCAL_ID, _MAP5t_START_ROW_, pCTRL->stMRU5_WDR_SW.start_row_addr_c);	// map5 start addr -- WDR sensor : NO ALLOC 	
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAP0_ROW_COUNT_PER_FRAME_, pCTRL->stMRU4_WDR_RW.mo_map.row_count_per_frame);	// map0_row_count_per_frame : dis
	MISP_RegWrite(MISP_LOCAL_ID, _MAP1_ROW_COUNT_PER_FRAME_, pCTRL->stMRU1_IPC.row_count_per_frame);	// map1_row_count_per_frame : inp
	MISP_RegWrite(MISP_LOCAL_ID, _MAP2_ROW_COUNT_PER_FRAME_, pCTRL->stMRU2_AUX.row_count_per_frame);	// map2_row_count_per_frame	: aux
	MISP_RegWrite(MISP_LOCAL_ID, _MAP3_ROW_COUNT_PER_FRAME_, pCTRL->stMRU3_NR.row_count_per_frame);	// map3_row_count_per_frame	: nr
	MISP_RegWrite(MISP_LOCAL_ID, _MAP4_ROW_COUNT_PER_FRAME_, pCTRL->stMRU4_WDR_RW.row_count_per_frame);	// map4_row_count_per_frame	: WDR result	   
	MISP_RegWrite(MISP_LOCAL_ID, _MAP5_ROW_COUNT_PER_FRAME_, pCTRL->stMRU5_WDR_SW.row_count_per_frame);	// map5_row_count_per_frame	: WDR sensor
																   
	MISP_RegWrite(MISP_LOCAL_ID, _MAP0_COL_COUNT_PER_LINE_, (pCTRL->stMRU4_WDR_RW.mo_map.bit_precision<<10)|(pCTRL->stMRU4_WDR_RW.mo_map.col_count_per_line));	// map0_col_count_per_line / bit_precision0 -- dis , 10bit
	MISP_RegWrite(MISP_LOCAL_ID, _MAP1_COL_COUNT_PER_LINE_, (pCTRL->stMRU1_IPC.bit_precision<<10)|(pCTRL->stMRU1_IPC.col_count_per_line));	// map1_col_count_per_line / bit_precision1 -- inp, ipc 10bit 
	MISP_RegWrite(MISP_LOCAL_ID, _MAP2_COL_COUNT_PER_LINE_, (pCTRL->stMRU2_AUX.bit_precision<<10)|(pCTRL->stMRU2_AUX.col_count_per_line));	// map2_col_count_per_line / bit_precision2 -- AUX 8bit
	MISP_RegWrite(MISP_LOCAL_ID, _MAP3_COL_COUNT_PER_LINE_, (pCTRL->stMRU3_NR.bit_precision<<10)|(pCTRL->stMRU3_NR.col_count_per_line));	// map3_col_count_per_line / bit_precision3 -- NR 10bit
	MISP_RegWrite(MISP_LOCAL_ID, _MAP4_COL_COUNT_PER_LINE_, (pCTRL->stMRU4_WDR_RW.bit_precision<<10)|(pCTRL->stMRU4_WDR_RW.col_count_per_line));	// map4_col_count_per_line / bit_precision4 -- WDR result : NO ALLOC 
	MISP_RegWrite(MISP_LOCAL_ID, _MAP5_COL_COUNT_PER_LINE_, (pCTRL->stMRU5_WDR_SW.bit_precision<<10)|(pCTRL->stMRU5_WDR_SW.col_count_per_line));	// map5_col_count_per_line / bit_precision5 -- WDR SENSOR : NO ALLOC
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_1_, (pCTRL->stMRU1_IPC.mapping_write_y<<12)|(pCTRL->stMRU1_IPC.mapping_read_y<<8)|(pCTRL->stMRU1_IPC.mapping_write_c<<4)|(pCTRL->stMRU1_IPC.mapping_read_c));	// mru 1 inp, ipc //0x0088
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_2_, (pCTRL->stMRU2_AUX.mapping_write_y<<12)|(pCTRL->stMRU2_AUX.mapping_read_y<<8)|(pCTRL->stMRU2_AUX.mapping_write_c<<4)|(pCTRL->stMRU2_AUX.mapping_read_c));	// mru 2 aux //0x44cc
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_5_, (pCTRL->stMRU5_WDR_SW.mapping_write_y<<12)|(pCTRL->stMRU5_WDR_SW.mapping_read_y<<8)|(pCTRL->stMRU5_WDR_SW.mapping_write_c<<4)|(pCTRL->stMRU5_WDR_SW.mapping_read_c));	// mru 5 wdr sonsor //0x55FF

	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_4_, (pCTRL->stMRU4_WDR_RW.mapping_write_y<<12)|(pCTRL->stMRU4_WDR_RW.mapping_read_y<<8)|(pCTRL->stMRU4_WDR_RW.mapping_write_mo<<4)|0x000F);	// mru 4 wdr result //0x11FF
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_0A_, (pCTRL->stMRU3_NR.mapping_write_y<<12)|(pCTRL->stMRU3_NR.mapping_read_y<<8)|(pCTRL->stMRU3_NR.mapping_write_c<<4)|(pCTRL->stMRU3_NR.mapping_read_c));	// mru 0 DIS //0x00FF
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_0B_, (pCTRL->stMRU4_WDR_RW.mapping_read_mo<<8)|0xF000|(pCTRL->stMRU4_WDR_RW.mapping_write_mohist<<4)|(pCTRL->stMRU4_WDR_RW.mapping_read_mohist));	// mru 0 DIS //0x00FF

	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_3_, (pCTRL->stMRU0_DIS.mapping_write_y<<12)|(pCTRL->stMRU0_DIS.mapping_read_y<<8)|(pCTRL->stMRU0_DIS.mapping_write_c<<4)|(pCTRL->stMRU0_DIS.mapping_read_c));	// mru 3 nr //0x33bb

	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_6_, 0xFFFF);	// mru 6 not using
	
	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_0_, (pCTRL->stMRU3_NR.write_pri<<12)|(pCTRL->stMRU1_IPC.write_pri<<8)|(pCTRL->stMRU2_AUX.write_pri<<4)|(pCTRL->stMRU0_DIS.write_pri));	// pri_starv_0 

	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_1_, (pCTRL->stMRU3_NR.read_pri<<12)|(pCTRL->stMRU1_IPC.read_pri<<8)|(pCTRL->stMRU2_AUX.read_pri<<4)|(pCTRL->stMRU0_DIS.read_pri));	// pri_starv_1 

	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_2_, (pCTRL->stMRU3_NR.write_stv<<12)|(pCTRL->stMRU1_IPC.write_stv<<8)|(pCTRL->stMRU2_AUX.write_stv<<4)|(pCTRL->stMRU0_DIS.write_stv));	// pri_starv_2 

	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_3_, (pCTRL->stMRU3_NR.read_stv<<12)|(pCTRL->stMRU1_IPC.read_stv<<8)|(pCTRL->stMRU2_AUX.read_stv<<4)|(pCTRL->stMRU0_DIS.read_stv));	// pri_starv_3 

	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_4_, (pCTRL->stMRU4_WDR_RW.write_pri<<12)|(pCTRL->stMRU4_WDR_RW.read_pri<<8)|(pCTRL->stMRU4_WDR_RW.write_stv<<4)|(pCTRL->stMRU4_WDR_RW.read_stv));	// pri_starv_4 

	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_5_, (pCTRL->stMRU5_WDR_SW.write_pri<<12)|(pCTRL->stMRU5_WDR_SW.read_pri<<8)|(pCTRL->stMRU5_WDR_SW.write_stv<<4)|(pCTRL->stMRU5_WDR_SW.read_stv));	// pri_starv_5

	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_6_, 0x7777);	// pri_starv_6 
#else

	MISP_RegWrite(MISP_LOCAL_ID, _MAP_F_COUNT_0_, (pCTRL->stMRU0_DIS.frame_cnt_y<<12)|(pCTRL->stMRU1_IPC.frame_cnt_y<<8)|(pCTRL->stMRU2_AUX.frame_cnt_y<<4)|(pCTRL->stMRU3_NR.frame_cnt_y));	// map count
	MISP_RegWrite(MISP_LOCAL_ID, _MAP_F_COUNT_4_, (pCTRL->stMRU4_WDR_RW.frame_cnt_y<<4)|(pCTRL->stMRU5_WDR_SW.frame_cnt_y));	//
	MISP_RegWrite(MISP_LOCAL_ID, _MAP_F_COUNT_0t_, (pCTRL->stMRU0_DIS.frame_cnt_c<<12)|(pCTRL->stMRU1_IPC.frame_cnt_c<<8)|(pCTRL->stMRU2_AUX.frame_cnt_c<<4)|(pCTRL->stMRU3_NR.frame_cnt_c));	//
	MISP_RegWrite(MISP_LOCAL_ID, _MAP_F_COUNT_4t_, (pCTRL->stMRU4_WDR_RW.frame_cnt_c<<4)|(pCTRL->stMRU5_WDR_SW.frame_cnt_c));	//	
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAP0_START_ROW_, pCTRL->stMRU0_DIS.start_row_addr_y);	// map0 start addr -- dis
	MISP_RegWrite(MISP_LOCAL_ID, _MAP1_START_ROW_, pCTRL->stMRU1_IPC.start_row_addr_y);	// map1 start addr -- inp
	MISP_RegWrite(MISP_LOCAL_ID, _MAP2_START_ROW_, pCTRL->stMRU2_AUX.start_row_addr_y);	// map2 start addr -- aux
	MISP_RegWrite(MISP_LOCAL_ID, _MAP3_START_ROW_, pCTRL->stMRU3_NR.start_row_addr_y);	// map3 start addr -- nr
	MISP_RegWrite(MISP_LOCAL_ID, _MAP4_START_ROW_, pCTRL->stMRU4_WDR_RW.start_row_addr_y);	// map4 start addr -- WDR result : NO ALLOC
	MISP_RegWrite(MISP_LOCAL_ID, _MAP5_START_ROW_, pCTRL->stMRU5_WDR_SW.start_row_addr_y);	// map5 start addr -- WDR sensor : NO ALLOC
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAP0t_START_ROW_, pCTRL->stMRU0_DIS.start_row_addr_c);	// map0 start addr -- dis : NO ALLOC
	MISP_RegWrite(MISP_LOCAL_ID, _MAP1t_START_ROW_, pCTRL->stMRU1_IPC.start_row_addr_c);	// map1 start addr -- inp
	MISP_RegWrite(MISP_LOCAL_ID, _MAP2t_START_ROW_, pCTRL->stMRU2_AUX.start_row_addr_c);	// map2 start addr -- aux 
	MISP_RegWrite(MISP_LOCAL_ID, _MAP3t_START_ROW_, pCTRL->stMRU3_NR.start_row_addr_c);	// map3 start addr -- nr
	MISP_RegWrite(MISP_LOCAL_ID, _MAP4t_START_ROW_, pCTRL->stMRU4_WDR_RW.start_row_addr_c);	// map4 start addr -- WDR result : NO ALLOC
	MISP_RegWrite(MISP_LOCAL_ID, _MAP5t_START_ROW_, pCTRL->stMRU5_WDR_SW.start_row_addr_c);	// map5 start addr -- WDR sensor : NO ALLOC 	
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAP0_ROW_COUNT_PER_FRAME_, pCTRL->stMRU0_DIS.row_count_per_frame);	// map0_row_count_per_frame : dis
	MISP_RegWrite(MISP_LOCAL_ID, _MAP1_ROW_COUNT_PER_FRAME_, pCTRL->stMRU1_IPC.row_count_per_frame);	// map1_row_count_per_frame : inp
	MISP_RegWrite(MISP_LOCAL_ID, _MAP2_ROW_COUNT_PER_FRAME_, pCTRL->stMRU2_AUX.row_count_per_frame);	// map2_row_count_per_frame	: aux
	MISP_RegWrite(MISP_LOCAL_ID, _MAP3_ROW_COUNT_PER_FRAME_, pCTRL->stMRU3_NR.row_count_per_frame);	// map3_row_count_per_frame	: nr
	MISP_RegWrite(MISP_LOCAL_ID, _MAP4_ROW_COUNT_PER_FRAME_, pCTRL->stMRU4_WDR_RW.row_count_per_frame);	// map4_row_count_per_frame	: WDR result	   
	MISP_RegWrite(MISP_LOCAL_ID, _MAP5_ROW_COUNT_PER_FRAME_, pCTRL->stMRU5_WDR_SW.row_count_per_frame);	// map5_row_count_per_frame	: WDR sensor
																   
	MISP_RegWrite(MISP_LOCAL_ID, _MAP0_COL_COUNT_PER_LINE_, (pCTRL->stMRU0_DIS.bit_precision<<10)|(pCTRL->stMRU0_DIS.col_count_per_line));	// map0_col_count_per_line / bit_precision0 -- dis , 10bit
	MISP_RegWrite(MISP_LOCAL_ID, _MAP1_COL_COUNT_PER_LINE_, (pCTRL->stMRU1_IPC.bit_precision<<10)|(pCTRL->stMRU1_IPC.col_count_per_line));	// map1_col_count_per_line / bit_precision1 -- inp, ipc 10bit 
	MISP_RegWrite(MISP_LOCAL_ID, _MAP2_COL_COUNT_PER_LINE_, (pCTRL->stMRU2_AUX.bit_precision<<10)|(pCTRL->stMRU2_AUX.col_count_per_line));	// map2_col_count_per_line / bit_precision2 -- AUX 8bit
	MISP_RegWrite(MISP_LOCAL_ID, _MAP3_COL_COUNT_PER_LINE_, (pCTRL->stMRU3_NR.bit_precision<<10)|(pCTRL->stMRU3_NR.col_count_per_line));	// map3_col_count_per_line / bit_precision3 -- NR 10bit
	MISP_RegWrite(MISP_LOCAL_ID, _MAP4_COL_COUNT_PER_LINE_, (pCTRL->stMRU4_WDR_RW.bit_precision<<10)|(pCTRL->stMRU4_WDR_RW.col_count_per_line));	// map4_col_count_per_line / bit_precision4 -- WDR result : NO ALLOC 
	MISP_RegWrite(MISP_LOCAL_ID, _MAP5_COL_COUNT_PER_LINE_, (pCTRL->stMRU5_WDR_SW.bit_precision<<10)|(pCTRL->stMRU5_WDR_SW.col_count_per_line));	// map5_col_count_per_line / bit_precision5 -- WDR SENSOR : NO ALLOC
	
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_0A_, (pCTRL->stMRU0_DIS.mapping_write_y<<12)|(pCTRL->stMRU0_DIS.mapping_read_y<<8)|(pCTRL->stMRU0_DIS.mapping_write_c<<4)|(pCTRL->stMRU0_DIS.mapping_read_c));	// mru 0 DIS //0x00FF
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_0B_, 0xFFFF);	// mru 0B not using
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_1_, (pCTRL->stMRU1_IPC.mapping_write_y<<12)|(pCTRL->stMRU1_IPC.mapping_read_y<<8)|(pCTRL->stMRU1_IPC.mapping_write_c<<4)|(pCTRL->stMRU1_IPC.mapping_read_c));	// mru 1 inp, ipc //0x0088
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_2_, (pCTRL->stMRU2_AUX.mapping_write_y<<12)|(pCTRL->stMRU2_AUX.mapping_read_y<<8)|(pCTRL->stMRU2_AUX.mapping_write_c<<4)|(pCTRL->stMRU2_AUX.mapping_read_c));	// mru 2 aux //0x44cc
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_3_, (pCTRL->stMRU3_NR.mapping_write_y<<12)|(pCTRL->stMRU3_NR.mapping_read_y<<8)|(pCTRL->stMRU3_NR.mapping_write_c<<4)|(pCTRL->stMRU3_NR.mapping_read_c));	// mru 3 nr //0x33bb
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_4_, (pCTRL->stMRU4_WDR_RW.mapping_write_y<<12)|(pCTRL->stMRU4_WDR_RW.mapping_read_y<<8)|(pCTRL->stMRU4_WDR_RW.mapping_write_mo<<4)|(pCTRL->stMRU4_WDR_RW.mapping_read_mo));	// mru 4 wdr result //0x11FF
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_5_, (pCTRL->stMRU5_WDR_SW.mapping_write_y<<12)|(pCTRL->stMRU5_WDR_SW.mapping_read_y<<8)|(pCTRL->stMRU5_WDR_SW.mapping_write_c<<4)|(pCTRL->stMRU5_WDR_SW.mapping_read_c));	// mru 5 wdr sonsor //0x55FF
	MISP_RegWrite(MISP_LOCAL_ID, _MAPPING_6_, 0xFFFF);	// mru 6 not using
	
	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_0_, (pCTRL->stMRU0_DIS.write_pri<<12)|(pCTRL->stMRU1_IPC.write_pri<<8)|(pCTRL->stMRU2_AUX.write_pri<<4)|(pCTRL->stMRU3_NR.write_pri));	// pri_starv_0 
	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_1_, (pCTRL->stMRU0_DIS.read_pri<<12)|(pCTRL->stMRU1_IPC.read_pri<<8)|(pCTRL->stMRU2_AUX.read_pri<<4)|(pCTRL->stMRU3_NR.read_pri));	// pri_starv_1 
	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_2_, (pCTRL->stMRU0_DIS.write_stv<<12)|(pCTRL->stMRU1_IPC.write_stv<<8)|(pCTRL->stMRU2_AUX.write_stv<<4)|(pCTRL->stMRU3_NR.write_stv));	// pri_starv_2 
	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_3_, (pCTRL->stMRU0_DIS.read_stv<<12)|(pCTRL->stMRU1_IPC.read_stv<<8)|(pCTRL->stMRU2_AUX.read_stv<<4)|(pCTRL->stMRU3_NR.read_stv));	// pri_starv_3 
	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_4_, (pCTRL->stMRU4_WDR_RW.write_pri<<12)|(pCTRL->stMRU4_WDR_RW.read_pri<<8)|(pCTRL->stMRU4_WDR_RW.write_stv<<4)|(pCTRL->stMRU4_WDR_RW.read_stv));	// pri_starv_4 
	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_5_, (pCTRL->stMRU5_WDR_SW.write_pri<<12)|(pCTRL->stMRU5_WDR_SW.read_pri<<8)|(pCTRL->stMRU5_WDR_SW.write_stv<<4)|(pCTRL->stMRU5_WDR_SW.read_stv));	// pri_starv_5
	MISP_RegWrite(MISP_LOCAL_ID, _PRI_STARV_6_, 0x7777);	// pri_starv_6 
	
#endif

	
	// UARTprintf("\r\n****DDR MAP SETUP PARAM\r\n") ;
	//DDRMAPCTRL_DDRMAP_info(&pCTRL->stDDRMAP) ;	
}


//--------------------------------------------------------------------------------------------------------------------------


void DDRMAP_ProcessHandler(void)
{
	if ( (stDDRMAPCTRL.ctrl&DDRMAPCTRL_TRIGGER_ISP) == 0 ) return ;
	
	stDDRMAPCTRL.ctrl &= (~DDRMAPCTRL_TRIGGER_ISP) ;
	
	DDRMAPCTRL_DDRMAPSetHandler(&stDDRMAPCTRL);

}

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------

void SetDDRMAP_ddr_access_enable ( BOOL val )
{
	stDDRMAPCTRL.stARBITER.access_en = val ;
	
	stDDRMAPCTRL.ctrl |= (DDRMAPCTRL_DDR_ACCESS_EN_UPDATE) ;
	
	DDRMAPCTRL_ArbiterDDRAccessHandler(&stDDRMAPCTRL) ;
}

//--------------------------------------------------------------------------------------------------------------------------

void SetDDRMAP_ddr_refresh_enable_n ( BOOL val )
{
	stDDRMAPCTRL.stARBITER.refresh_en_n = val ;
	
	stDDRMAPCTRL.ctrl |= (DDRMAPCTRL_DDR_REFRESH_EN_N_UPDATE) ;
	
	DDRMAPCTRL_ArbiterDDRRefreshHandler(&stDDRMAPCTRL) ;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_MRU0_DIS ( WORD in_hsize, WORD in_vsize, BYTE frame_cnt, BYTE bit_precision, WORD start_row_addr )
{
	stDDRMAPCTRL.stMRU0_DIS.frame_cnt_y = frame_cnt ;
	stDDRMAPCTRL.stMRU0_DIS.frame_cnt_c = 0 ;

	stDDRMAPCTRL.stMRU0_DIS.bit_precision = bit_precision ;
	stDDRMAPCTRL.stMRU0_DIS.col_count_per_line = DDRMAPCTRL_calc_col_count( in_hsize, bit_precision ) ;
	stDDRMAPCTRL.stMRU0_DIS.row_count_per_frame = DDRMAPCTRL_calc_row_count( stDDRMAPCTRL.stMRU0_DIS.col_count_per_line, in_vsize) ;
		
	stDDRMAPCTRL.stMRU0_DIS.start_row_addr_y = start_row_addr ;
	stDDRMAPCTRL.stMRU0_DIS.start_row_addr_c = stDDRMAPCTRL.stMRU0_DIS.start_row_addr_y + (stDDRMAPCTRL.stMRU0_DIS.row_count_per_frame*stDDRMAPCTRL.stMRU0_DIS.frame_cnt_y) ;
	stDDRMAPCTRL.stMRU0_DIS.end_row_addr = stDDRMAPCTRL.stMRU0_DIS.start_row_addr_c + (stDDRMAPCTRL.stMRU0_DIS.row_count_per_frame*stDDRMAPCTRL.stMRU0_DIS.frame_cnt_c) ;
#if __MEMORY_MAP_REV__	
	stDDRMAPCTRL.stMRU0_DIS.mapping_write_y = 1 ;
#else
	stDDRMAPCTRL.stMRU0_DIS.mapping_write_y = 0 ;
#endif
	stDDRMAPCTRL.stMRU0_DIS.mapping_read_y = stDDRMAPCTRL.stMRU0_DIS.mapping_write_y ;
	stDDRMAPCTRL.stMRU0_DIS.mapping_write_c = stDDRMAPCTRL.stMRU0_DIS.mapping_write_y+8 ;
	stDDRMAPCTRL.stMRU0_DIS.mapping_read_c = stDDRMAPCTRL.stMRU0_DIS.mapping_write_c ;	
	
	stDDRMAPCTRL.stMRU0_DIS.write_pri = 13 ;	
	stDDRMAPCTRL.stMRU0_DIS.read_pri = 14 ;
	
	stDDRMAPCTRL.stMRU0_DIS.write_stv = 5 ;
//	stDDRMAPCTRL.stMRU0_DIS.read_stv = 7 ;		// kw5210
	stDDRMAPCTRL.stMRU0_DIS.read_stv = 1 ;
	
	stDDRMAPCTRL.ctrl |= (DDRMAPCTRL_FCMRU_UPDATE|DDRMAPCTRL_TRIGGER_ISP);
}
//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_MRU1_IPC ( WORD in_hsize, WORD in_vsize, BYTE frame_cnt, BYTE frame_cnt_c, BYTE bit_precision, WORD start_row_addr , WORD isIPC_444)
{
	stDDRMAPCTRL.stMRU1_IPC.frame_cnt_y = frame_cnt ;	
	stDDRMAPCTRL.stMRU1_IPC.frame_cnt_c = (frame_cnt_c<<isIPC_444) ;
	
	stDDRMAPCTRL.stMRU1_IPC.bit_precision = bit_precision ;
	stDDRMAPCTRL.stMRU1_IPC.col_count_per_line = DDRMAPCTRL_calc_col_count( in_hsize, bit_precision ) ;
	stDDRMAPCTRL.stMRU1_IPC.row_count_per_frame = DDRMAPCTRL_calc_row_count( stDDRMAPCTRL.stMRU1_IPC.col_count_per_line, in_vsize) ;	
	
	stDDRMAPCTRL.stMRU1_IPC.start_row_addr_y = start_row_addr ; 
	stDDRMAPCTRL.stMRU1_IPC.start_row_addr_c = stDDRMAPCTRL.stMRU1_IPC.start_row_addr_y + (stDDRMAPCTRL.stMRU1_IPC.row_count_per_frame*stDDRMAPCTRL.stMRU1_IPC.frame_cnt_y) ;
	stDDRMAPCTRL.stMRU1_IPC.end_row_addr = stDDRMAPCTRL.stMRU1_IPC.start_row_addr_c + (stDDRMAPCTRL.stMRU1_IPC.row_count_per_frame*stDDRMAPCTRL.stMRU1_IPC.frame_cnt_c) ;	
		
	stDDRMAPCTRL.stMRU1_IPC.mapping_write_y = 1 ;
	stDDRMAPCTRL.stMRU1_IPC.mapping_read_y = stDDRMAPCTRL.stMRU1_IPC.mapping_write_y ;
	stDDRMAPCTRL.stMRU1_IPC.mapping_write_c = stDDRMAPCTRL.stMRU1_IPC.mapping_write_y+8 ;
	stDDRMAPCTRL.stMRU1_IPC.mapping_read_c = stDDRMAPCTRL.stMRU1_IPC.mapping_write_c ;
	
	// stDDRMAPCTRL.stMRU1_IPC.write_pri = 5 ;	
	// stDDRMAPCTRL.stMRU1_IPC.read_pri = 6 ;

	stDDRMAPCTRL.stMRU1_IPC.write_pri = 6 ;	
	stDDRMAPCTRL.stMRU1_IPC.read_pri = 4 ;	
	
	stDDRMAPCTRL.stMRU1_IPC.write_stv = 5 ;
	stDDRMAPCTRL.stMRU1_IPC.read_stv = 7 ;	
	
	stDDRMAPCTRL.ctrl |= (DDRMAPCTRL_FCMRU_UPDATE|DDRMAPCTRL_TRIGGER_ISP);
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_MRU2_AUX ( WORD in_hsize, WORD in_vsize, BYTE frame_cnt, BYTE frame_cnt_c, BYTE bit_precision, WORD start_row_addr )
{
	stDDRMAPCTRL.stMRU2_AUX.frame_cnt_y = frame_cnt ;
	stDDRMAPCTRL.stMRU2_AUX.frame_cnt_c = frame_cnt_c ;

	stDDRMAPCTRL.stMRU2_AUX.bit_precision = bit_precision ;
	stDDRMAPCTRL.stMRU2_AUX.col_count_per_line = DDRMAPCTRL_calc_col_count( in_hsize , bit_precision ) ;
	stDDRMAPCTRL.stMRU2_AUX.row_count_per_frame = DDRMAPCTRL_calc_row_count( stDDRMAPCTRL.stMRU2_AUX.col_count_per_line, in_vsize) ;
		
	stDDRMAPCTRL.stMRU2_AUX.start_row_addr_y = start_row_addr ; 
	stDDRMAPCTRL.stMRU2_AUX.start_row_addr_c = stDDRMAPCTRL.stMRU2_AUX.start_row_addr_y + (stDDRMAPCTRL.stMRU2_AUX.row_count_per_frame*stDDRMAPCTRL.stMRU2_AUX.frame_cnt_y) ;	
	stDDRMAPCTRL.stMRU2_AUX.end_row_addr = stDDRMAPCTRL.stMRU2_AUX.start_row_addr_c + (stDDRMAPCTRL.stMRU2_AUX.row_count_per_frame*stDDRMAPCTRL.stMRU2_AUX.frame_cnt_c)	;
	
	stDDRMAPCTRL.stMRU2_AUX.mapping_write_y = 2 ;
	stDDRMAPCTRL.stMRU2_AUX.mapping_read_y = stDDRMAPCTRL.stMRU2_AUX.mapping_write_y ;
	stDDRMAPCTRL.stMRU2_AUX.mapping_write_c = stDDRMAPCTRL.stMRU2_AUX.mapping_write_y+8 ;
	stDDRMAPCTRL.stMRU2_AUX.mapping_read_c = stDDRMAPCTRL.stMRU2_AUX.mapping_write_c ;
	
	stDDRMAPCTRL.stMRU2_AUX.write_pri = 9 ;	// 7->9 for cvbs output noise on ddr clock 383Mhz
#if defined (__USE_2_INPUT_HDMI__)
	stDDRMAPCTRL.stMRU2_AUX.read_pri = 7 ;
#else
	stDDRMAPCTRL.stMRU2_AUX.read_pri = 8 ;
#endif
	
	stDDRMAPCTRL.stMRU2_AUX.write_stv = 5 ;
	stDDRMAPCTRL.stMRU2_AUX.read_stv = 7 ;		
	
	stDDRMAPCTRL.ctrl |= (DDRMAPCTRL_FCMRU_UPDATE|DDRMAPCTRL_TRIGGER_ISP);
}
//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_MRU3_NR ( WORD in_hsize, WORD in_vsize,BYTE frame_cnt, BYTE nr_image_type , BYTE bit_precision, WORD start_row_addr )
{
	stDDRMAPCTRL.stMRU3_NR.frame_cnt_y = frame_cnt ;
	
	if ( nr_image_type == MISP_NR_BAYER_IMAGE )	stDDRMAPCTRL.stMRU3_NR.frame_cnt_c = 0 ;
	else stDDRMAPCTRL.stMRU3_NR.frame_cnt_c = frame_cnt ;

	stDDRMAPCTRL.stMRU3_NR.bit_precision = bit_precision ;
	stDDRMAPCTRL.stMRU3_NR.col_count_per_line = DDRMAPCTRL_calc_col_count( in_hsize , bit_precision ) ;
	stDDRMAPCTRL.stMRU3_NR.row_count_per_frame = DDRMAPCTRL_calc_row_count( stDDRMAPCTRL.stMRU3_NR.col_count_per_line, in_vsize) ;
		
	stDDRMAPCTRL.stMRU3_NR.start_row_addr_y = start_row_addr ; 
	stDDRMAPCTRL.stMRU3_NR.start_row_addr_c = stDDRMAPCTRL.stMRU3_NR.start_row_addr_y + (stDDRMAPCTRL.stMRU3_NR.row_count_per_frame*stDDRMAPCTRL.stMRU3_NR.frame_cnt_y) ;
	stDDRMAPCTRL.stMRU3_NR.end_row_addr = stDDRMAPCTRL.stMRU3_NR.start_row_addr_c + (stDDRMAPCTRL.stMRU3_NR.row_count_per_frame*stDDRMAPCTRL.stMRU3_NR.frame_cnt_c)	;
	
	stDDRMAPCTRL.stMRU3_NR.mapping_write_y = 3 ;
	stDDRMAPCTRL.stMRU3_NR.mapping_read_y = stDDRMAPCTRL.stMRU3_NR.mapping_write_y ;
	stDDRMAPCTRL.stMRU3_NR.mapping_write_c = stDDRMAPCTRL.stMRU3_NR.mapping_write_y+8 ;
	stDDRMAPCTRL.stMRU3_NR.mapping_read_c = stDDRMAPCTRL.stMRU3_NR.mapping_write_c ;
	
#if defined(__USE_CMOS_IMX265__)	
	if ( stDDRMAPCTRL.stDDRMAP.rotation_en == 1 )
		stDDRMAPCTRL.stMRU3_NR.write_pri = 13 ;	
	else
		stDDRMAPCTRL.stMRU3_NR.write_pri = 9 ;	
#else	
	stDDRMAPCTRL.stMRU3_NR.write_pri = 9 ;	
#endif
	stDDRMAPCTRL.stMRU3_NR.read_pri = 10 ;
	
	stDDRMAPCTRL.stMRU3_NR.write_stv = 5 ;
	stDDRMAPCTRL.stMRU3_NR.read_stv = 7 ;
	
	stDDRMAPCTRL.ctrl |= (DDRMAPCTRL_FCMRU_UPDATE|DDRMAPCTRL_TRIGGER_ISP);
}
//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_MRU4_WDR_RW ( WORD in_hsize, WORD in_vsize, BYTE frame_cnt , BYTE bit_precision, WORD start_row_addr )
{
	stDDRMAPCTRL.stMRU4_WDR_RW.frame_cnt_y = frame_cnt ;
	stDDRMAPCTRL.stMRU4_WDR_RW.frame_cnt_c = 0 ;

	stDDRMAPCTRL.stMRU4_WDR_RW.bit_precision = bit_precision ;
	
	stDDRMAPCTRL.stMRU4_WDR_RW.col_count_per_line = DDRMAPCTRL_calc_col_count( in_hsize , bit_precision ) ;
	stDDRMAPCTRL.stMRU4_WDR_RW.row_count_per_frame = DDRMAPCTRL_calc_row_count( stDDRMAPCTRL.stMRU4_WDR_RW.col_count_per_line, in_vsize) ;

	stDDRMAPCTRL.stMRU4_WDR_RW.start_row_addr_y = start_row_addr ; 
	stDDRMAPCTRL.stMRU4_WDR_RW.start_row_addr_c = stDDRMAPCTRL.stMRU4_WDR_RW.start_row_addr_y + (stDDRMAPCTRL.stMRU4_WDR_RW.row_count_per_frame*stDDRMAPCTRL.stMRU4_WDR_RW.frame_cnt_y) ;
	stDDRMAPCTRL.stMRU4_WDR_RW.end_row_addr = stDDRMAPCTRL.stMRU4_WDR_RW.start_row_addr_c + (stDDRMAPCTRL.stMRU4_WDR_RW.row_count_per_frame*stDDRMAPCTRL.stMRU4_WDR_RW.frame_cnt_c)	;
	
	stDDRMAPCTRL.stMRU4_WDR_RW.mapping_write_y = 4 ;
	stDDRMAPCTRL.stMRU4_WDR_RW.mapping_read_y = stDDRMAPCTRL.stMRU4_WDR_RW.mapping_write_y ;
	
	stDDRMAPCTRL.stMRU4_WDR_RW.write_pri = 11 ;	
	stDDRMAPCTRL.stMRU4_WDR_RW.read_pri = 12 ;
	
	stDDRMAPCTRL.stMRU4_WDR_RW.write_stv = 5 ;
	stDDRMAPCTRL.stMRU4_WDR_RW.read_stv = 7 ;	

	// WDR motion flag map setting : 
	stDDRMAPCTRL.stMRU4_WDR_RW.mapping_write_mo = 0 ;
	stDDRMAPCTRL.stMRU4_WDR_RW.mapping_read_mo = 0 ;
	
	stDDRMAPCTRL.stMRU4_WDR_RW.mapping_write_mohist = stDDRMAPCTRL.stMRU4_WDR_RW.mapping_write_mo+8 ;
	stDDRMAPCTRL.stMRU4_WDR_RW.mapping_read_mohist = stDDRMAPCTRL.stMRU4_WDR_RW.mapping_write_mohist ;
		
	stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.frame_cnt = frame_cnt ;
	
	stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.bit_precision = MISP_DDRMAP_8b ;	
	// stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.col_count_per_line = DDRMAPCTRL_calc_col_count(((in_hsize+64)>>2), stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.bit_precision ) ;
	// stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.col_count_per_line = DDRMAPCTRL_calc_col_count(in_hsize, stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.bit_precision ) ;
	stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.col_count_per_line = DDRMAPCTRL_calc_col_count(((in_hsize+64)>>2), stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.bit_precision ) ;
	stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.row_count_per_frame = DDRMAPCTRL_calc_row_count(stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.col_count_per_line, in_vsize ) ;
	
	stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.start_row_addr_mo = stDDRMAPCTRL.stMRU4_WDR_RW.end_row_addr ;
	stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.start_row_addr_mohist = stDDRMAPCTRL.stMRU4_WDR_RW.end_row_addr + (stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.row_count_per_frame*stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.frame_cnt) ;
	stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.end_row_addr = stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.start_row_addr_mohist + (stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.row_count_per_frame*1) ;
	
	stDDRMAPCTRL.ctrl |= (DDRMAPCTRL_FCMRU_UPDATE|DDRMAPCTRL_TRIGGER_ISP);
}
//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_MRU5_WDR_SW ( WORD in_hsize, WORD in_vsize, BYTE frame_cnt , BYTE bit_precision, WORD start_row_addr )
{
	stDDRMAPCTRL.stMRU5_WDR_SW.frame_cnt_y = frame_cnt ;
	stDDRMAPCTRL.stMRU5_WDR_SW.frame_cnt_c = 0 ;

	stDDRMAPCTRL.stMRU5_WDR_SW.bit_precision = bit_precision ;
	stDDRMAPCTRL.stMRU5_WDR_SW.col_count_per_line = DDRMAPCTRL_calc_col_count( in_hsize , bit_precision ) ;
	stDDRMAPCTRL.stMRU5_WDR_SW.row_count_per_frame = DDRMAPCTRL_calc_row_count( stDDRMAPCTRL.stMRU5_WDR_SW.col_count_per_line, in_vsize) ;

	stDDRMAPCTRL.stMRU5_WDR_SW.start_row_addr_y = start_row_addr ; 
	stDDRMAPCTRL.stMRU5_WDR_SW.start_row_addr_c = stDDRMAPCTRL.stMRU5_WDR_SW.start_row_addr_y + (stDDRMAPCTRL.stMRU5_WDR_SW.row_count_per_frame*stDDRMAPCTRL.stMRU5_WDR_SW.frame_cnt_y) ;
	stDDRMAPCTRL.stMRU5_WDR_SW.end_row_addr = stDDRMAPCTRL.stMRU5_WDR_SW.start_row_addr_c + (stDDRMAPCTRL.stMRU5_WDR_SW.row_count_per_frame*stDDRMAPCTRL.stMRU5_WDR_SW.frame_cnt_c)	;
	
	stDDRMAPCTRL.stMRU5_WDR_SW.mapping_write_y = 5 ;
	stDDRMAPCTRL.stMRU5_WDR_SW.mapping_read_y = stDDRMAPCTRL.stMRU5_WDR_SW.mapping_write_y ;
	stDDRMAPCTRL.stMRU5_WDR_SW.mapping_write_c = stDDRMAPCTRL.stMRU5_WDR_SW.mapping_write_y+8 ;
	stDDRMAPCTRL.stMRU5_WDR_SW.mapping_read_c = stDDRMAPCTRL.stMRU5_WDR_SW.mapping_write_c ;	
	
	stDDRMAPCTRL.stMRU5_WDR_SW.write_pri = 11 ;	
	stDDRMAPCTRL.stMRU5_WDR_SW.read_pri = 12 ;
	
	stDDRMAPCTRL.stMRU5_WDR_SW.write_stv = 5 ;
	stDDRMAPCTRL.stMRU5_WDR_SW.read_stv = 7 ;		
	
	stDDRMAPCTRL.ctrl |= (DDRMAPCTRL_FCMRU_UPDATE|DDRMAPCTRL_TRIGGER_ISP);
}

//--------------------------------------------------------------------------------------------------------------------------
WORD GetDDRMAP_IPC_Y_start_row(void)
{
	return stDDRMAPCTRL.stMRU1_IPC.start_row_addr_y ;
}

//--------------------------------------------------------------------------------------------------------------------------
WORD GetDDRMAP_IPC_C_start_row(void)
{
	return stDDRMAPCTRL.stMRU1_IPC.start_row_addr_c ;
}

//--------------------------------------------------------------------------------------------------------------------------
WORD GetDDRMAP_WDRSW_start_row(void)
{
	return stDDRMAPCTRL.stMRU5_WDR_SW.start_row_addr_y ;
}

//--------------------------------------------------------------------------------------------------------------------------
WORD GetDDRMAP_NR_start_row(void)
{
	return stDDRMAPCTRL.stMRU3_NR.start_row_addr_y ;
}

//--------------------------------------------------------------------------------------------------------------------------
WORD GetDDRMAP_IPC_end_row(void)
{
	return stDDRMAPCTRL.stMRU1_IPC.end_row_addr ;
}
WORD GetDDRMAP_AUX_end_row(void)
{
	return stDDRMAPCTRL.stMRU2_AUX.end_row_addr ;
}
WORD GetDDRMAP_NR_end_row(void)
{
	return stDDRMAPCTRL.stMRU3_NR.end_row_addr ;
}
WORD GetDDRMAP_WDRRW_end_row(void)
{
#if	__MEMORY_MAP_REV__
	return stDDRMAPCTRL.stMRU4_WDR_RW.mo_map.end_row_addr ;
#else
	return stDDRMAPCTRL.stMRU4_WDR_RW.end_row_addr ;
#endif
}
WORD GetDDRMAP_WDRSW_end_row(void)
{
	return stDDRMAPCTRL.stMRU5_WDR_SW.end_row_addr ;
}
//--------------------------------------------------------------------------------------------------------------------------
void GetDDRMAP_WDRSW_MemCRPF(PBYTE bit_mode, PWORD CPF,PWORD RPF)
{
	*bit_mode = stDDRMAPCTRL.stMRU5_WDR_SW.bit_precision ;
	*CPF = stDDRMAPCTRL.stMRU5_WDR_SW.col_count_per_line ;
	*RPF = stDDRMAPCTRL.stMRU5_WDR_SW.row_count_per_frame ;
}

void GetDDRMAP_NR_MemCRPF(PBYTE bit_mode, PWORD CPF,PWORD RPF)
{
	*bit_mode = stDDRMAPCTRL.stMRU3_NR.bit_precision ;
	*CPF = stDDRMAPCTRL.stMRU3_NR.col_count_per_line ;
	*RPF = stDDRMAPCTRL.stMRU3_NR.row_count_per_frame ;
}

void GetDDRMAP_IPC_MemCRPF(PBYTE bit_mode, PWORD CPF,PWORD RPF)
{
	*bit_mode = stDDRMAPCTRL.stMRU1_IPC.bit_precision ;
	*CPF = stDDRMAPCTRL.stMRU1_IPC.col_count_per_line ;
	*RPF = stDDRMAPCTRL.stMRU1_IPC.row_count_per_frame ;
}

//--------------------------------------------------------------------------------------------------------------------------

void SetDDRMAP( PDDRMAP_PARAM pCtrl ) 
{
	//////////////////////////////////////////////////
	
	WORD bounadry_hsize ;
	WORD bounadry_vsize ;
	WORD active_hsize ;
	WORD active_vsize ;

	WORD in_hsize ;
	WORD in_vsize ;

	WORD in_main_hsize ;
	WORD in_main_vsize ;		
	
	WORD in_aux_hsize ;
	WORD in_aux_vsize ;	
	
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
	
	
	//////////////////////////////////////////////////

	bounadry_hsize = pCtrl->bounadry_hsize ;
	bounadry_vsize = pCtrl->bounadry_vsize ;
	active_hsize = pCtrl->active_hsize ;
	active_vsize = pCtrl->active_vsize ;
	
	in_hsize = active_hsize + (bounadry_hsize<<1) ;
	in_vsize = active_vsize + (bounadry_vsize<<1) ;
	
	in_main_hsize = pCtrl->main_hsize ;
	in_main_vsize = pCtrl->main_vsize ;
	
	in_aux_hsize = pCtrl->aux_hsize ;
	in_aux_vsize = pCtrl->aux_vsize ;
	
	//////////////////////////////////////////////////
	rotation_en = pCtrl->rotation_en ;
	IPC_444_en = pCtrl->IPC_444_en ;
	RGB_gamma_en = pCtrl->RGB_gamma_en ;
	vertical_flip_en = pCtrl->vertical_flip_en ;	
	DIS_en = pCtrl->DIS_en ;	
	
	NR_image_type = pCtrl->NR_image_type ;	
	
	//////////////////////////////////////////////////

	if ( vertical_flip_en == 0 && DIS_en == 0 ) 
	{
		DIS_frame_count = pCtrl->DIS_frame_count ;	
		IPC_frame_count = pCtrl->IPC_frame_count ;	
		IPC_frame_count_c = pCtrl->IPC_frame_count_c ;	
	}
	else if ( vertical_flip_en == 1 && DIS_en == 0 )  // Vertical flip needs minimum triple buffering 
	{
		// DIS_frame_count = MAX(3,pCtrl->DIS_frame_count) ;
		DIS_frame_count = pCtrl->DIS_frame_count ;
		IPC_frame_count = MAX(3,pCtrl->IPC_frame_count) ;
		IPC_frame_count_c = MAX(3,pCtrl->IPC_frame_count_c) ;
		
		// pCtrl->DIS_frame_count = DIS_frame_count ;	
		pCtrl->IPC_frame_count = IPC_frame_count ;		
		pCtrl->IPC_frame_count_c = IPC_frame_count_c ;		
	}
	else
	{ 
		if ( DIS_en == 1 ) // DIS_en == 1 : DIS needs minimum QUAD buffering 
		{
			DIS_frame_count = MAX(4,pCtrl->DIS_frame_count) ;
			
			IPC_frame_count = MAX(4,pCtrl->DIS_frame_count) ;
			IPC_frame_count_c = MAX(4,pCtrl->DIS_frame_count) ;
			
			// IPC_frame_count = MAX(4,pCtrl->IPC_frame_count) ;			
			// IPC_frame_count_c = MAX(4,pCtrl->IPC_frame_count_c) ;
			
			pCtrl->DIS_frame_count = DIS_frame_count ;	
			// pCtrl->IPC_frame_count = IPC_frame_count ;		
			// pCtrl->IPC_frame_count_c = IPC_frame_count_c ;		
		}
		else // DIS_en == 2 : DIS limited tripple buffering 
		{
			DIS_frame_count = MAX(3,pCtrl->DIS_frame_count) ;
			IPC_frame_count = MAX(3,pCtrl->DIS_frame_count) ;
			IPC_frame_count_c = MAX(3,pCtrl->DIS_frame_count) ;
			
			// IPC_frame_count = MAX(3,pCtrl->IPC_frame_count) ;
			// IPC_frame_count_c = MAX(3,pCtrl->IPC_frame_count_c) ;
			
			pCtrl->DIS_frame_count = DIS_frame_count ;	
			// pCtrl->IPC_frame_count = IPC_frame_count ;
			// pCtrl->IPC_frame_count_c = IPC_frame_count_c ;			
		}
		
/*		// default main y/c frame count changed(2 -> 3) to fix menu osd broken when change 'c' frame(3fr -> 2fr)
		if ( vertical_flip_en == 1 ){
			pCtrl->IPC_frame_count = 3 ;
			pCtrl->IPC_frame_count_c = 3 ;
		}
		else{		
			pCtrl->IPC_frame_count = 2 ;
			pCtrl->IPC_frame_count_c = 2 ;
		}
*/
	}
	
	if ( DIS_en == 0 ) 
	{
		AUX_frame_count = pCtrl->AUX_frame_count ;	
		AUX_frame_count_c = pCtrl->AUX_frame_count ;	
	}
	else{  
		if ( DIS_en == 1 ) // DIS_en == 1 : DIS needs minimum QUAD buffering 
		{		
			AUX_frame_count = MAX(4,pCtrl->DIS_frame_count) ;
			AUX_frame_count_c = MAX(4,pCtrl->DIS_frame_count) ;
			
			// AUX_frame_count = MAX(4,pCtrl->AUX_frame_count) ;
			// AUX_frame_count_c = MAX(4,pCtrl->AUX_frame_count_c) ;			
		
			// pCtrl->AUX_frame_count = AUX_frame_count ;
			// pCtrl->AUX_frame_count_c = AUX_frame_count_c ;
		}
		else // DIS_en == 2 : DIS limited tripple buffering 
		{
			AUX_frame_count = MAX(3,pCtrl->DIS_frame_count) ;
			AUX_frame_count_c = MAX(3,pCtrl->DIS_frame_count) ;
			
			// AUX_frame_count = MAX(3,pCtrl->AUX_frame_count) ;
			// AUX_frame_count_c = MAX(3,pCtrl->AUX_frame_count_c) ;
		
			// pCtrl->AUX_frame_count = AUX_frame_count ;
			// pCtrl->AUX_frame_count_c = AUX_frame_count_c ;		
		}
/*		// default main y/c frame count changed(2 -> 3) to fix menu osd broken when change 'c' frame(3fr -> 2fr)
		if ( vertical_flip_en == 1 ){
			pCtrl->AUX_frame_count = 3 ;
			pCtrl->AUX_frame_count_c = 3 ;
		}
		else{
			pCtrl->AUX_frame_count = 2 ;
			pCtrl->AUX_frame_count_c = 2 ;			
		}
*/
	}
	
	NR_frame_count = pCtrl->NR_frame_count ;
	
	
	WDRRW_frame_count = pCtrl->WDRRW_frame_count ;	
	WDRSW_frame_count = pCtrl->WDRSW_frame_count ;
	ROT_frame_count = pCtrl->ROT_frame_count ;

	
	//////////////////////////////////////////////////
	IPC_bit_precision = pCtrl->IPC_bit_precision ;	
	AUX_bit_precision = pCtrl->AUX_bit_precision ;	
	
	if ( RGB_gamma_en == 0 )
	{	
		NR_bit_precision = pCtrl->NR_bit_precision ;
	}
	else
	{			
		NR_bit_precision = MISP_DDRMAP_10b ; 
		pCtrl->NR_bit_precision = NR_bit_precision ;
	}
	
	WDR_RW_bit_precision = pCtrl->WDR_RW_bit_precision ;	
	WDR_SW_bit_precision = pCtrl->WDR_SW_bit_precision ;	
	ROT_bit_precision = pCtrl->ROT_bit_precision ;	

	//////////////////////////////////////////////////
	
	// DDRMAPCTRL_DDRMAP_info(pCtrl) ;
	
	//////////////////////////////////////////////////
	
	if ( rotation_en == 0 ) // rotation disabled
	{
		SetDDRMAP_MRU0_DIS ( in_main_hsize, in_main_vsize, DIS_frame_count, IPC_bit_precision, 0 ) ;	
		SetDDRMAP_MRU1_IPC ( in_main_hsize, in_main_vsize, IPC_frame_count, IPC_frame_count_c, IPC_bit_precision, 0 , IPC_444_en) ;
		SetDDRMAP_MRU2_AUX ( in_aux_hsize , in_aux_vsize, AUX_frame_count, AUX_frame_count_c, AUX_bit_precision, GetDDRMAP_IPC_end_row()) ;
		SetDDRMAP_MRU3_NR ( in_hsize, in_vsize, NR_frame_count, NR_image_type, NR_bit_precision, GetDDRMAP_AUX_end_row()) ;	
		
		SetDDRMAP_MRU4_WDR_RW(in_hsize, in_vsize, WDRRW_frame_count, WDR_RW_bit_precision, GetDDRMAP_NR_end_row());		

#if defined(__USE_GET_RGB_MODE__)
		SetDDRMAP_MRU5_WDR_SW(in_hsize, in_vsize, WDRSW_frame_count, WDR_SW_bit_precision, GetDDRMAP_WDRRW_end_row());			
#else
    #if defined(__USE_GET_AWB_RGB_INFO__) && (defined(__USE_CMOS_IMX226__) || defined (__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX334__))
      #if defined(__USE_GET_AWB_RGB_INFO_SIZE_4__) && (defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__)|| defined(__USE_CMOS_IMX334__))
		SetDDRMAP_MRU5_WDR_SW(in_hsize/4, in_vsize/64, WDRSW_frame_count, WDR_SW_bit_precision, GetDDRMAP_WDRRW_end_row());	// shring size for mem bandwidth
      #else
		SetDDRMAP_MRU5_WDR_SW(in_hsize/8, in_vsize/8, WDRSW_frame_count, WDR_SW_bit_precision, GetDDRMAP_WDRRW_end_row());	// shring size for mem bandwidth
      #endif
    #else
		SetDDRMAP_MRU5_WDR_SW(in_hsize, in_vsize, WDRSW_frame_count, WDR_SW_bit_precision, GetDDRMAP_WDRRW_end_row());			
    #endif
#endif
	}
	else // rotation enabled
	{
		SetDDRMAP_MRU0_DIS ( in_main_vsize, in_main_hsize, DIS_frame_count, IPC_bit_precision, 0 ) ;
		SetDDRMAP_MRU1_IPC ( in_main_vsize, in_main_hsize, IPC_frame_count, IPC_frame_count_c, IPC_bit_precision, 0 , IPC_444_en) ;	
//		SetDDRMAP_MRU2_AUX ( in_aux_vsize , in_aux_hsize, AUX_frame_count, AUX_frame_count_c, AUX_bit_precision, GetDDRMAP_IPC_end_row()) ;
		if(in_aux_hsize > 2048)		// 4K(over 3M) rotation => map size over 1Gb DDR.
			SetDDRMAP_MRU2_AUX ( 0 , 0, 0, 0, AUX_bit_precision, GetDDRMAP_IPC_end_row()) ;// aux map disable.
		else						
			SetDDRMAP_MRU2_AUX ( in_aux_vsize , in_aux_hsize, AUX_frame_count, AUX_frame_count_c, AUX_bit_precision, GetDDRMAP_IPC_end_row()) ;
		SetDDRMAP_MRU3_NR ( in_vsize, in_hsize, NR_frame_count, NR_image_type, NR_bit_precision, GetDDRMAP_AUX_end_row()) ;	
		
		SetDDRMAP_MRU4_WDR_RW(in_vsize, in_hsize, ROT_frame_count, ROT_bit_precision, GetDDRMAP_NR_end_row());		
		SetDDRMAP_MRU5_WDR_SW(in_hsize, in_vsize, ROT_frame_count, ROT_bit_precision, GetDDRMAP_WDRRW_end_row());			
	}
}


//--------------------------------------------------------------------------------------------------------------------------
void GetDDRMAP( PDDRMAP_PARAM pCtrl ) 
{
	memcpy( pCtrl, &stDDRMAPCTRL.stDDRMAP ,  sizeof(DDRMAP_PARAM) ) ;
	
	UARTprintf("Copied current DDR map\r\n") ;
	
	DDRMAPCTRL_DDRMAP_info(pCtrl) ;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE GetDDRMAP_vflip_mode(void)
{
	return stDDRMAPCTRL.stDDRMAP.vertical_flip_en ;
}


//--------------------------------------------------------------------------------------------------------------------------

void SetDDRMAP_RGBgamma_mode(BYTE val)
{
#if __MEMORY_MAP_REV__	
	stDDRMAPCTRL.stDDRMAP.RGB_gamma_en = val ;

	SetDDRMAP(&stDDRMAPCTRL.stDDRMAP) ;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_image_size(WORD bh, WORD bv, WORD ah, WORD av)
{	
	stDDRMAPCTRL.stDDRMAP.bounadry_hsize = bh ;
	stDDRMAPCTRL.stDDRMAP.bounadry_vsize = bv ;
	stDDRMAPCTRL.stDDRMAP.active_hsize = ah ;
	stDDRMAPCTRL.stDDRMAP.active_vsize = av ;		
	
	SetDDRMAP(&stDDRMAPCTRL.stDDRMAP) ;
}
//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_main_size(WORD ah, WORD av)
{	
	stDDRMAPCTRL.stDDRMAP.main_hsize = ah ;
	stDDRMAPCTRL.stDDRMAP.main_vsize = av ;		
	
	SetDDRMAP(&stDDRMAPCTRL.stDDRMAP) ;
}
//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_aux_size(WORD ah, WORD av)
{	
	stDDRMAPCTRL.stDDRMAP.aux_hsize = ah ;
	stDDRMAPCTRL.stDDRMAP.aux_vsize = av ;		
	
	SetDDRMAP(&stDDRMAPCTRL.stDDRMAP) ;
}
//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_IPC_frame_count(BYTE frm_count)
{
	stDDRMAPCTRL.stDDRMAP.IPC_frame_count = frm_count ;
	stDDRMAPCTRL.stDDRMAP.IPC_frame_count_c = frm_count ;	
		
	SetDDRMAP(&stDDRMAPCTRL.stDDRMAP) ;
}
//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_AUX_frame_count(BYTE frm_count)
{
	stDDRMAPCTRL.stDDRMAP.AUX_frame_count = frm_count ;	
	stDDRMAPCTRL.stDDRMAP.AUX_frame_count_c = frm_count ;
	
	SetDDRMAP(&stDDRMAPCTRL.stDDRMAP) ;
}
//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_rotation_mode(BYTE val, WORD bh, WORD bv, WORD ah, WORD av)
{
#if __MEMORY_MAP_REV__
	stDDRMAPCTRL.stDDRMAP.rotation_en = val ;
	
	stDDRMAPCTRL.stDDRMAP.bounadry_hsize = bh ;
	stDDRMAPCTRL.stDDRMAP.bounadry_vsize = bv ;
	stDDRMAPCTRL.stDDRMAP.active_hsize = ah ;
	stDDRMAPCTRL.stDDRMAP.active_vsize = av ;		
	
	stDDRMAPCTRL.stDDRMAP.ROT_bit_precision = GetROT_BitMode() ;		
	
	SetDDRMAP(&stDDRMAPCTRL.stDDRMAP) ;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_vflip_mode(BYTE val , BYTE frm_count)
{
	stDDRMAPCTRL.stDDRMAP.vertical_flip_en = val ;
	
	// stDDRMAPCTRL.stDDRMAP.DIS_frame_count = frm_count ;
	stDDRMAPCTRL.stDDRMAP.IPC_frame_count = frm_count ;
	stDDRMAPCTRL.stDDRMAP.IPC_frame_count_c = frm_count ;	
	stDDRMAPCTRL.stDDRMAP.AUX_frame_count = frm_count ;	
	stDDRMAPCTRL.stDDRMAP.AUX_frame_count_c = frm_count ;	
	
	SetDDRMAP(&stDDRMAPCTRL.stDDRMAP) ;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_DIS_mode(BYTE val)
{
	stDDRMAPCTRL.stDDRMAP.DIS_en = val ;
	
	SetDDRMAP(&stDDRMAPCTRL.stDDRMAP) ;	

}

//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_IPC_Bitmode(BYTE Bitmode)
{	
	stDDRMAPCTRL.stDDRMAP.IPC_bit_precision = Bitmode ;
	
	SetDDRMAP(&stDDRMAPCTRL.stDDRMAP) ;
}
//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_AUX_Bitmode(BYTE Bitmode)
{	
#if __MEMORY_MAP_REV__	
	stDDRMAPCTRL.stDDRMAP.AUX_bit_precision = Bitmode ;
	
	SetDDRMAP(&stDDRMAPCTRL.stDDRMAP) ;
#endif
}
//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_IPC444_mode(BYTE val)
{
#if __MEMORY_MAP_REV__		
	stDDRMAPCTRL.stDDRMAP.IPC_444_en = val ;
	
	SetDDRMAP(&stDDRMAPCTRL.stDDRMAP) ;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_NRImagetype_mode(BYTE val,BYTE Bitmode)
{
	stDDRMAPCTRL.stDDRMAP.NR_image_type = val ;
	stDDRMAPCTRL.stDDRMAP.NR_bit_precision = Bitmode ;
	
	SetDDRMAP(&stDDRMAPCTRL.stDDRMAP) ;	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_WDR(BYTE buffing, BYTE frame_num, BYTE sw_bit_mode, BYTE rw_bit_mode)
{
	// WORD in_hsize ;
	// WORD in_vsize ;
	
	// in_hsize = GetSS_image_active_size_h() + (GetSS_image_boundary_size_h()<<1) ;
	// in_vsize = GetSS_image_active_size_v() + (GetSS_image_boundary_size_v()<<1) ;

	//////////////////////////////////////////////////
	stDDRMAPCTRL.stDDRMAP.bounadry_hsize = GetSS_image_boundary_size_h() ;
	stDDRMAPCTRL.stDDRMAP.bounadry_vsize = GetSS_image_boundary_size_v() ;
	
	stDDRMAPCTRL.stDDRMAP.active_hsize = GetSS_image_active_size_h() ; // mimitest
	stDDRMAPCTRL.stDDRMAP.active_vsize = GetSS_image_active_size_v() ;
	
	if ( frame_num != 0 ) stDDRMAPCTRL.stDDRMAP.WDRRW_frame_count = 2 ; 
	else stDDRMAPCTRL.stDDRMAP.WDRRW_frame_count = 0 ; 
	
	stDDRMAPCTRL.stDDRMAP.WDRSW_frame_count = (buffing*frame_num) ; 
	stDDRMAPCTRL.stDDRMAP.WDR_SW_bit_precision = sw_bit_mode ;
	stDDRMAPCTRL.stDDRMAP.WDR_RW_bit_precision = rw_bit_mode ;
	//////////////////////////////////////////////////
	
// #if   defined(__USE_MEMORY_MAP1__)		// (10bit) 256M DIS : No map for WDR core
	// SetDDRMAP_MAP1();
// #elif defined(__USE_MEMORY_MAP2__)		// (10bit) 512M WDR : No map for DIS
	// SetDDRMAP_MRU5_WDR_SW(in_hsize, in_vsize, (buffing*frame_num), bit_precision, GetDDRMAP_WDRRW_end_row());
// #elif defined(__USE_MEMORY_MAP3__)		// (8bit) 256M DIS : No map for WDR core 
	// SetDDRMAP_MAP3();
// #elif defined(__USE_MEMORY_MAP4__)		// (8bit) 512M WDR + DIS
	// SetDDRMAP_MRU5_WDR_SW(in_hsize, in_vsize, (buffing*frame_num), bit_precision, GetDDRMAP_WDRRW_end_row());
// #elif defined(__USE_MEMORY_MAP5__)		// (8bit) 256M aptina HTM / Frame by Frame WDR
	// SetDDRMAP_MRU5_WDR_SW(in_hsize, in_vsize, (buffing*frame_num), bit_precision, GetDDRMAP_WDRRW_end_row());
// #endif
	
	SetDDRMAP(&stDDRMAPCTRL.stDDRMAP) ;	
	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_DIS_mapping( BYTE val )
{
	stDDRMAPCTRL.stMRU0_DIS.mapping_write_y = val ;
	stDDRMAPCTRL.stMRU0_DIS.mapping_read_y = val ;
	stDDRMAPCTRL.stMRU0_DIS.mapping_write_c = val+8 ;
	stDDRMAPCTRL.stMRU0_DIS.mapping_read_c = val+8 ;
	
	DDRMAPCTRL_DIS_MappingUpdate(&stDDRMAPCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------

BYTE GetDDRMAP_IPC_mapping(void)
{
	return stDDRMAPCTRL.stMRU1_IPC.mapping_read_y ;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE GetDDRMAP_IPC_frame_count(void)
{
	// return stDDRMAPCTRL.stMRU1_IPC.frame_cnt_y ;
	return stDDRMAPCTRL.stDDRMAP.IPC_frame_count ;
}

//--------------------------------------------------------------------------------------------------------------------------

BYTE GetDDRMAP_AUX_frame_count(void)
{
	// return stDDRMAPCTRL.stMRU2_AUX.frame_cnt_y ;
	return stDDRMAPCTRL.stDDRMAP.AUX_frame_count ;
}
//--------------------------------------------------------------------------------------------------------------------------
BYTE GetDDRMAP_AUX_mapping(void)
{
	return stDDRMAPCTRL.stMRU2_AUX.mapping_read_y ;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDDRMAP_SetAudioDelay(WORD AudioDelayMS, MISP_AUD_DELAY_MODE_t AudioDelayMode)
{
	// AudioDelayMS Range : 0 ~ 500(ms) for one audio stream delay
	//	mdin380 defined 
	//	mdin380 video.c : MDINHTX_SetAudioDelay(OFF, 300, AUDIO_DELAY_MODE_128_ONE); // range: 0~500(ms)
	WORD AudioDelayAmount = 0;

	#if defined		(__USE_DDR_1G__)
	WORD StartRowAddr=16000-AudioDelayMS;
	MISP_RegField(MISP_LOCAL_ID, 0x1fb, 7, 1, 1);		//audio_col_8bit
	#elif defined	(__USE_DDR_512M__)
	WORD StartRowAddr=8000-AudioDelayMS;
	MISP_RegField(MISP_LOCAL_ID, 0x1fb, 7, 1, 1);		//audio_col_8bit
	#else
	WORD StartRowAddr=4000-AudioDelayMS;
	MISP_RegField(MISP_LOCAL_ID, 0x1fb, 7, 1, 1);		//audio_col_8bit
	#endif	

       	// calculate audio delay amount : input is msec unit for each delay mode
		// SCK = 64*fs = 64*48kHz = 3,072kHz, MDIN380: 4kByte/row, MDIN340: 2kByte/row

	MISP_RegField(MISP_LOCAL_ID, 0x1fb, 0, 2, 0);
	MISP_RegField(MISP_LOCAL_ID, 0x1fc, 0, 1, 0);
		
	switch (AudioDelayMode) {
		// for one audio stream (SD_IN(0) selected)
		case MISP_AUD_DELAY_MODE_128_ONE:
			MISP_RegField(MISP_LOCAL_ID, 0x1fb, 3, 2, 0);
			MISP_RegField(MISP_LOCAL_ID, 0x1fb, 5, 2, 3);
			if (AudioDelayMS>500)	AudioDelayAmount = 500 * 24;			// 24(=3,072kHz/128)
			else					AudioDelayAmount = AudioDelayMS * 24;
			break;
		// for all audio stream (SD_IN(3:0))
		case MISP_AUD_DELAY_MODE_32_ALL:
			MISP_RegField(MISP_LOCAL_ID, 0x1fb, 5, 2, 2);
			if (AudioDelayMS>125)	AudioDelayAmount = 125 * 96;			// 96(=3,072kHz/32)
			else					AudioDelayAmount = AudioDelayMS * 96;
			break;
		// for one audio stream (SD_IN(0) selected)
		case MISP_AUD_DELAY_MODE_64_ONE:
			MISP_RegField(MISP_LOCAL_ID, 0x1fb, 3, 2, 0);
			MISP_RegField(MISP_LOCAL_ID, 0x1fb, 5, 2, 1);
			if (AudioDelayMS>250)	AudioDelayAmount = 250 * 48;			// 48(=3,072kHz/64)
			else					AudioDelayAmount = AudioDelayMS * 48;
			break;
		// for all audio stream (SD_IN(3:0))
		case MISP_AUD_DELAY_MODE_16_ALL:
			MISP_RegField(MISP_LOCAL_ID, 0x1fb, 5, 2, 0);
			if (AudioDelayMS>62)		AudioDelayAmount = 62 * 192;			// 192(=3,072kHz/16)
			else					AudioDelayAmount = AudioDelayMS * 192;
			break;
		default: break;
	}



	// Row Addr Start/End Configuration
  // 	MISP_RegRead(MISP_LOCAL_ID, 0x1f8, &StartRowAddr);
	MISP_RegWrite(MISP_LOCAL_ID, 0x1f8, StartRowAddr);

	MISP_RegWrite(MISP_LOCAL_ID, 0x1f9, StartRowAddr+48); 		// 500ms = 192kByte = 48row (4kByte/row)
	MISP_RegWrite(MISP_LOCAL_ID, 0x1fa, AudioDelayAmount);		 // 0x1fa[15:0] = (0~65535)

	// enable audio delay & enable audio output
	MISP_RegField(MISP_LOCAL_ID, 0x1fb, 0, 2, 3);
	MISP_RegField(MISP_LOCAL_ID, 0x1fc, 0, 1, 1);

	 // enable audio delay
	MISP_RegField(MISP_HOST_ID, 0x03e, 15, 1, 0);	

	MISP_RegField(MISP_HOST_ID, 0x08F, 0, 8, 16+1);  	//audio_bypass  <= slv_regs(16#03E#)(15);  -- ¡®1¡¯ bypass  ¡®0¡¯ normal
	MISP_RegField(MISP_HOST_ID, 0x08F, 8, 8, 32+2);  	//audio_bypass  <= slv_regs(16#03E#)(15);  -- ¡®1¡¯ bypass  ¡®0¡¯ normal	
			
}

//add by jins 2016.04
//--------------------------------------------------------------------------------------------------------------------------
BYTE GetDDRMAP_IPC_Bitmode(void)
{	
	return stDDRMAPCTRL.stDDRMAP.IPC_bit_precision;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE GetDDRMAP_AUX_Bitmode(void)
{	
	return stDDRMAPCTRL.stDDRMAP.AUX_bit_precision;
}


//--------------------------------------------------------------------------------------------------------------------------
void SetMemoryMAP_480i(void){
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b0, 0x4000);	// set DDR
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b1, 0x0000);	// map0 start addr -- inp -- bit reesolution NR 8bit, WDR 8bit, INP(FRC) 8bit
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b2, 0x0AA0);	// map1 start addr -- WDR accmo -- map NR 8bit, WDR 8bit, INP(FRC) 8bit
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b3, 0x10E0);	// map2 start addr -- fey
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b4, 0x10E0);	// map3 start addr -- nr
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b5, 0x1518);	// map4 start addr -- aux
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b6, 0x19B4);	// map5 start addr -- wdr 

	MISP_RegWrite(MISP_LOCAL_ID, 0x1b7, 0x0550);	// map0 start addr -- inp
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b8, 0x0FF0);	// map1 start addr -- x
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b9, 0x1518);	// map2 start addr -- fey 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1ba, 0x1518);	// map3 start addr -- nr
	MISP_RegWrite(MISP_LOCAL_ID, 0x1bb, 0x1518);	// map4 start addr -- aux
	MISP_RegWrite(MISP_LOCAL_ID, 0x1bc, 0x19B4);	// map5 start addr -- x 

	MISP_RegWrite(MISP_LOCAL_ID, 0x1bd, 0x023A);	// map0_row_count_per_frame
	MISP_RegWrite(MISP_LOCAL_ID, 0x1be, 0x021C);	// 1
	MISP_RegWrite(MISP_LOCAL_ID, 0x1bf, 0x02A8);	// 2
	MISP_RegWrite(MISP_LOCAL_ID, 0x1c0, 0x021C);	// 3
	MISP_RegWrite(MISP_LOCAL_ID, 0x1c1, 0x02A8);	// 4		   
	MISP_RegWrite(MISP_LOCAL_ID, 0x1c2, 0x021C);	// 5

	MISP_RegWrite(MISP_LOCAL_ID, 0x1c3, 0x00f0);	// map0_col_count_per_line / bit_precision -- inp, ipc 8bit
	MISP_RegWrite(MISP_LOCAL_ID, 0x1c4, 0x00f0);	// 1 -- WDR acc mo 8bit
	MISP_RegWrite(MISP_LOCAL_ID, 0x1c5, 0x00f0);	// 2 -- not used
	MISP_RegWrite(MISP_LOCAL_ID, 0x1c6, 0x00f0);	// 3 -- NR 8bit
	MISP_RegWrite(MISP_LOCAL_ID, 0x1c7, 0x00f0);	// 4 -- AUX 8bit
	MISP_RegWrite(MISP_LOCAL_ID, 0x1c8, 0x00f0);	// 5 -- WDR 8bit

	MISP_RegWrite(MISP_LOCAL_ID, 0x1d7, 0x2000);	// map count
	MISP_RegWrite(MISP_LOCAL_ID, 0x1d8, 0x0000);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x1d9, 0x2000);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x1da, 0x0000);	//

	MISP_RegWrite(MISP_LOCAL_ID, 0x1c9, 0x2233);	// mapping 0 fisheye //0x22aa 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1ca, 0x44ff);	// mapping 1 fisheye //0xffff
	MISP_RegWrite(MISP_LOCAL_ID, 0x1cb, 0x0011);	// mapping 2 inp, ipc //0x0088
	MISP_RegWrite(MISP_LOCAL_ID, 0x1cc, 0xcccc);	// mapping 3 aux //0x44cc
	MISP_RegWrite(MISP_LOCAL_ID, 0x1cd, 0xbbbb);	// mapping 4 nr //0x33bb
	MISP_RegWrite(MISP_LOCAL_ID, 0x1dd, 0xdddd);	// mapping 5 wdr//0x5511
//	MISP_RegWrite(MISP_LOCAL_ID, 0x1ce, 0xd579);	// pri_starv_0 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1ce, 0x7777);	// pri_starv_0 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1cf, 0xE68A);	// pri_starv_1 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1d0, 0x5555);	// pri_starv_2 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1d1, 0x7777);	// pri_starv_3 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1de, 0xbc57);	// pri_starv_4 

	MISP_RegWrite(MISP_LOCAL_ID, 0x1d2, 0x0200);	// mapping 6 dis //0x0000
	MISP_RegWrite(MISP_LOCAL_ID, 0x1d3, 0x0356);	// pri_starv_6 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1d4, 0x0300);	// mapping 7 //0x0000
	MISP_RegWrite(MISP_LOCAL_ID, 0x1d5, 0x3c57);	// pri_starv_7 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1d6, 0x9B00);	//


}

void SetMemoryMAP_1080i(void){
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b0, 0x4000);	// set DDR
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b1, 0x0000);	// map0 start addr -- inp -- bit reesolution NR 8bit, WDR 8bit, INP(FRC) 8bit
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b2, 3780);	// map1 start addr -- WDR accmo -- map NR 8bit, WDR 8bit, INP(FRC) 8bit
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b3, 4392);	// map2 start addr -- fey
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b4, 4392);	// map3 start addr -- nr
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b5, 5472);	// map4 start addr -- aux
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b6, 7500);	// map5 start addr -- wdr 

	MISP_RegWrite(MISP_LOCAL_ID, 0x1b7, 2700);	// map0 start addr -- inp
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b8, 0);		// map1 start addr -- x
	MISP_RegWrite(MISP_LOCAL_ID, 0x1b9, 4392);	// map2 start addr -- fey 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1ba, 4932);	// map3 start addr -- nr
	MISP_RegWrite(MISP_LOCAL_ID, 0x1bb, 6486);	// map4 start addr -- aux
	MISP_RegWrite(MISP_LOCAL_ID, 0x1bc, 0);		// map5 start addr -- x 

	MISP_RegWrite(MISP_LOCAL_ID, 0x1bd, 540);	// map0_row_count_per_frame
	MISP_RegWrite(MISP_LOCAL_ID, 0x1be, 612);	// 1
	MISP_RegWrite(MISP_LOCAL_ID, 0x1bf, 540);	// 2
	MISP_RegWrite(MISP_LOCAL_ID, 0x1c0, 540);	// 3
	MISP_RegWrite(MISP_LOCAL_ID, 0x1c1, 507);	// 4		   
	MISP_RegWrite(MISP_LOCAL_ID, 0x1c2, 540);	// 5

	MISP_RegWrite(MISP_LOCAL_ID, 0x1c3, 240);		// map0_col_count_per_line / bit_precision -- inp, ipc 8bit
	MISP_RegWrite(MISP_LOCAL_ID, 0x1c4, 272);		// 1 -- WDR acc mo 8bit
	MISP_RegWrite(MISP_LOCAL_ID, 0x1c5, 240);	// 2 -- not used
	MISP_RegWrite(MISP_LOCAL_ID, 0x1c6, 240);		// 3 -- NR 8bit
	MISP_RegWrite(MISP_LOCAL_ID, 0x1c7, 240);		// 4 -- AUX 8bit
	MISP_RegWrite(MISP_LOCAL_ID, 0x1c8, 240);	// 5 -- WDR 8bit

	MISP_RegWrite(MISP_LOCAL_ID, 0x1d7, 0x5101);	// map count
	MISP_RegWrite(MISP_LOCAL_ID, 0x1d8, 0x0020);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x1d9, 0x2001);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x1da, 0x0020);	//

	MISP_RegWrite(MISP_LOCAL_ID, 0x1c9, 0x22AA);	// mapping 0 fisheye //0x22aa 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1ca, 0xffff);	// mapping 1 fisheye //0xffff
	MISP_RegWrite(MISP_LOCAL_ID, 0x1cb, 0x0088);	// mapping 2 inp, ipc //0x0088
	MISP_RegWrite(MISP_LOCAL_ID, 0x1cc, 0x44cc);	// mapping 3 aux //0x44cc
	MISP_RegWrite(MISP_LOCAL_ID, 0x1cd, 0x33bb);	// mapping 4 nr //0x33bb
	MISP_RegWrite(MISP_LOCAL_ID, 0x1dd, 0x5511);	// mapping 5 wdr//0x5511
//	MISP_RegWrite(MISP_LOCAL_ID, 0x1ce, 0xd579);	// pri_starv_0 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1ce, 0x7777);	// pri_starv_0 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1cf, 0x7788);	// pri_starv_1 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1d0, 0x5555);	// pri_starv_2 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1d1, 0x7777);	// pri_starv_3 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1de, 0xbc57);	// pri_starv_4 

	MISP_RegWrite(MISP_LOCAL_ID, 0x1d2, 0x20a8);	// mapping 6 dis //0x0000
	MISP_RegWrite(MISP_LOCAL_ID, 0x1d3, 0x9755);	// pri_starv_6 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1d4, 0x0000);	// mapping 7 //0x0000
	MISP_RegWrite(MISP_LOCAL_ID, 0x1d5, 0xBc57);	// pri_starv_7 
	MISP_RegWrite(MISP_LOCAL_ID, 0x1d6, 0x8000);	//


}

#if defined(__USE_CMOS_MT9P031__)		// use 5M Sensor	
void Set5MegaMemoryMap(BOOL OnOff)			//2012.11.02
{
	switch(OnOff)	{
	case ON :
	//do not use 3DNR, Frame WDR 						// copy from misp100.c
		MISP_RegWrite(MISP_LOCAL_ID, 0x1b0, 0x4000);	// set DDR
		MISP_RegWrite(MISP_LOCAL_ID, 0x1b1, 0x0000);	// map0 start addr -- inp -- bit reesolution NR 8bit, WDR 8bit, INP(FRC) 8bit
		MISP_RegWrite(MISP_LOCAL_ID, 0x1b2, 4924);	// map1 start addr -- WDR accmo -- map NR 8bit, WDR 8bit, INP(FRC) 8bit
		MISP_RegWrite(MISP_LOCAL_ID, 0x1b3, 4924);	// map2 start addr -- fey
		MISP_RegWrite(MISP_LOCAL_ID, 0x1b4, 4924);	// map3 start addr -- nr
		MISP_RegWrite(MISP_LOCAL_ID, 0x1b5, 4924);	// map4 start addr -- aux
		MISP_RegWrite(MISP_LOCAL_ID, 0x1b6, 5464);	// map5 start addr -- wdr 
		MISP_RegWrite(MISP_LOCAL_ID, 0x1b7, 2462);	// map0 start addr -- inp
		MISP_RegWrite(MISP_LOCAL_ID, 0x1b8, 0);	// map1 start addr -- x
		MISP_RegWrite(MISP_LOCAL_ID, 0x1b9, 4924);	// map2 start addr -- fey 
		MISP_RegWrite(MISP_LOCAL_ID, 0x1ba, 4924);	// map3 start addr -- nr
		MISP_RegWrite(MISP_LOCAL_ID, 0x1bb, 5194);	// map4 start addr -- aux
		MISP_RegWrite(MISP_LOCAL_ID, 0x1bc, 0x0000);	// map5 start addr -- x 
		MISP_RegWrite(MISP_LOCAL_ID, 0x1bd, 1231);	// map0_row_count_per_frame
		MISP_RegWrite(MISP_LOCAL_ID, 0x1be, 1352);	// 1
		MISP_RegWrite(MISP_LOCAL_ID, 0x1bf, 1231);	// 2
		MISP_RegWrite(MISP_LOCAL_ID, 0x1c0, 0x02EE);	// 3
		MISP_RegWrite(MISP_LOCAL_ID, 0x1c1, 135);	// 4		   
		MISP_RegWrite(MISP_LOCAL_ID, 0x1c2, 1231);	// 5
		MISP_RegWrite(MISP_LOCAL_ID, 0x1c3, 0x0144);	// map0_col_count_per_line / bit_precision -- inp, ipc 8bit
		MISP_RegWrite(MISP_LOCAL_ID, 0x1c4, 0x0164);	// 1 -- WDR acc mo 8bit
		MISP_RegWrite(MISP_LOCAL_ID, 0x1c5, 0x0144);	// 2 -- not used
		MISP_RegWrite(MISP_LOCAL_ID, 0x1c6, 0x0144);	// 3 -- NR 8bit
		MISP_RegWrite(MISP_LOCAL_ID, 0x1c7, 0x0078);	// 4 -- AUX 8bit
		MISP_RegWrite(MISP_LOCAL_ID, 0x1c8, 0x0144);	// 5 -- WDR 8bit
		MISP_RegWrite(MISP_LOCAL_ID, 0x1d7, 0x2000);	// map count
		MISP_RegWrite(MISP_LOCAL_ID, 0x1d8, 0x0030);	//
		MISP_RegWrite(MISP_LOCAL_ID, 0x1d9, 0x2000);	//
		MISP_RegWrite(MISP_LOCAL_ID, 0x1da, 0x0030);	//
		break;
	case OFF :	default : 
//		SetMemoryMAP4();
		break;	
	}
}
#endif


/*  FILE_END _HERE */
