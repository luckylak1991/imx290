// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"misp100.h"
#include	"video.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static DM_CTRL_PARAM stDMCTRL;
// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
extern ROMDATA WORD def_color_correction_table[14][9];

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------
static WORD  color_correction_table[14][9];			

//--------------------------------------------------------------------------------------------------------------------------
static void DM_ColorMatrixOffset(BYTE R, BYTE G, BYTE B)
{
	stDMCTRL.stDMCC.offset_r = R;	
	stDMCTRL.stDMCC.offset_g = G;	
	stDMCTRL.stDMCC.offset_b = B;		

	MISP_RegWrite(MISP_LOCAL_ID, 0x48D, (stDMCTRL.stDMCC.offset_r<<8)|(stDMCTRL.stDMCC.offset_g));
	MISP_RegWrite(MISP_LOCAL_ID, 0x48E, stDMCTRL.stDMCC.offset_b);		
}
//--------------------------------------------------------------------------------------------------------------------------
static void DM_ColorMatrix_Call(WORD rval, WORD gval, WORD bval)
{


	if ((stDMCTRL.ctrl&DMCTRL_CC_CNTL_CHANGE)==0) return;

#if __CC_BIT_EXTENSION__== 2
	stDMCTRL.stDMCC.bayer_rr_gain = color_correction_table[rval][0];
	stDMCTRL.stDMCC.bayer_gr_gain = color_correction_table[rval][1];
	stDMCTRL.stDMCC.bayer_br_gain = color_correction_table[rval][2];
	
	stDMCTRL.stDMCC.bayer_rg_gain = color_correction_table[gval][3];
	stDMCTRL.stDMCC.bayer_gg_gain = color_correction_table[gval][4];
	stDMCTRL.stDMCC.bayer_bg_gain = color_correction_table[gval][5];

	stDMCTRL.stDMCC.bayer_rb_gain = color_correction_table[bval][6];
	stDMCTRL.stDMCC.bayer_gb_gain = color_correction_table[bval][7];
	stDMCTRL.stDMCC.bayer_bb_gain = color_correction_table[bval][8];

	stDMCTRL.stDMCC.offset_r = stDMCTRL.stDMCC.CC_Brightness;	
	stDMCTRL.stDMCC.offset_g = stDMCTRL.stDMCC.CC_Brightness;	
	stDMCTRL.stDMCC.offset_b = stDMCTRL.stDMCC.CC_Brightness;	
	
#elif __CC_BIT_EXTENSION__ == 1
{
	WORD val=0;
	WORD i ;
	WORD value_table[9] ;
	WORD value ; 
	
	for ( i = 0 ; i < 9 ; i++ )
	{	
		if(i<3) val=rval;
		else if(i<6) val=gval;
		else val=bval;
	
		value = color_correction_table[val][i] ;
		
		if ( value >= (1<<(__CC_BIT_SRC__-1)) )
			value_table[i] = (1<<(__CC_BIT_DEST__-1)) + value ;
		else
			value_table[i] = value ;
	}
	
	stDMCTRL.stDMCC.bayer_rr_gain = value_table[0];
	stDMCTRL.stDMCC.bayer_gr_gain = value_table[1];
	stDMCTRL.stDMCC.bayer_br_gain = value_table[2];
	
	stDMCTRL.stDMCC.bayer_rg_gain = value_table[3];
	stDMCTRL.stDMCC.bayer_gg_gain = value_table[4];
	stDMCTRL.stDMCC.bayer_bg_gain = value_table[5];

	stDMCTRL.stDMCC.bayer_rb_gain = value_table[6];
	stDMCTRL.stDMCC.bayer_gb_gain = value_table[7];
	stDMCTRL.stDMCC.bayer_bb_gain = value_table[8];

	stDMCTRL.stDMCC.offset_r = stDMCTRL.stDMCC.CC_Brightness;	
	stDMCTRL.stDMCC.offset_g = stDMCTRL.stDMCC.CC_Brightness;	
	stDMCTRL.stDMCC.offset_b = stDMCTRL.stDMCC.CC_Brightness;	
}	
#else		
	stDMCTRL.stDMCC.bayer_rr_gain = color_correction_table[rval][0];
	stDMCTRL.stDMCC.bayer_gr_gain = color_correction_table[rval][1];
	stDMCTRL.stDMCC.bayer_br_gain = color_correction_table[rval][2];
	
	stDMCTRL.stDMCC.bayer_rg_gain = color_correction_table[gval][3];
	stDMCTRL.stDMCC.bayer_gg_gain = color_correction_table[gval][4];
	stDMCTRL.stDMCC.bayer_bg_gain = color_correction_table[gval][5];

	stDMCTRL.stDMCC.bayer_rb_gain = color_correction_table[bval][6];
	stDMCTRL.stDMCC.bayer_gb_gain = color_correction_table[bval][7];
	stDMCTRL.stDMCC.bayer_bb_gain = color_correction_table[bval][8];

	stDMCTRL.stDMCC.offset_r = stDMCTRL.stDMCC.CC_Brightness;	

	#if defined (__M33_V101__) 
		if(stDMCTRL.stDMCC.CC_Brightness>=2) stDMCTRL.stDMCC.offset_g = stDMCTRL.stDMCC.CC_Brightness-2;	
		else if(stDMCTRL.stDMCC.CC_Brightness==1)  stDMCTRL.stDMCC.offset_g =254;	
		else stDMCTRL.stDMCC.offset_g =253;	
	#else
		stDMCTRL.stDMCC.offset_g = stDMCTRL.stDMCC.CC_Brightness;	
	#endif

	stDMCTRL.stDMCC.offset_b = stDMCTRL.stDMCC.CC_Brightness;	

#endif	
}
//--------------------------------------------------------------------------------------------------------------------------
static void DM_ColorCorr_copy_coef(void)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT) && defined(__CC_USE_EEPROM__)		// check eeprom size
//	WORD addr_cc=EEPROM_CC_TABLE + (WORD)nID * sizeof(color_correction_table);
	WORD addr_cc=EEPROM_CC_TABLE;

	if(EEPROM_RegRead(EEPROM_CC_FLAG) != EEPROM_CC_DONE)	{
		EEPROM_MultiWrite(EEPROM_CC_TABLE, (PBYTE)&def_color_correction_table, sizeof(def_color_correction_table));	
		EEPROM_RegWrite(EEPROM_CC_FLAG,  EEPROM_CC_DONE);
	}

	EEPROM_MultiRead(addr_cc, (PBYTE)&color_correction_table, sizeof(color_correction_table));
	UARTprintf("Color Correction call by eeprom \r\n");
#else
	memcpy(&color_correction_table, &def_color_correction_table, sizeof(color_correction_table));
	UARTprintf("Color Correction call by romdata \r\n");
#endif

}

//--------------------------------------------------------------------------------------------------------------------------
static void Init_DM_PARAM( PDM_CTRL_PARAM pCTRL ){
	
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	WORD bayer_id ;
#endif
	
#if defined(__MDIN_i550_USB_V100__)
	pCTRL->input_select = 1;	// for NR block disable
#else
	pCTRL->input_select = GetNR_Inputimage_type() ;
#endif
	
	pCTRL->dm_bypass = 0 ;
	pCTRL->dm_zip_bypass = 0 ;
	
#if defined(__USE_MDIN_i500__)    // MDIN-i500
	pCTRL->bayer_pixel_id = GetSS_bayer_data_order() ;
#else                          // MDIN-i51X (i510, i540, i550)
	bayer_id = GetSS_bayer_data_order() ;
	
	switch (bayer_id)
	{
		case MISP_SS_BAYER_PATTERN_0 : pCTRL->bayer_pixel_id = MISP_BY_BAYER_PATTERN_0 ; break ;
		case MISP_SS_BAYER_PATTERN_1 : pCTRL->bayer_pixel_id = MISP_BY_BAYER_PATTERN_1 ; break ;
		case MISP_SS_BAYER_PATTERN_2 : pCTRL->bayer_pixel_id = MISP_BY_BAYER_PATTERN_2 ; break ;
		case MISP_SS_BAYER_PATTERN_3 : pCTRL->bayer_pixel_id = MISP_BY_BAYER_PATTERN_3 ; break ;
		default : pCTRL->bayer_pixel_id = MISP_BY_BAYER_PATTERN_0 ; break ;
	}			 
#endif
	
	pCTRL->bH = GetSS_image_boundary_size_h() ;
	pCTRL->bV = GetSS_image_boundary_size_v() ;
	pCTRL->active_h_size = GetSS_image_active_size_h() ;
	pCTRL->active_v_size = GetSS_image_active_size_v() ;		

	DM_ColorCorr_copy_coef();

	pCTRL->stDMCC.CC_Contrast=0;		//0% 0
	

#if defined(__PANORAMA_SETUP__)||defined(__USE_CMOS_IMX185__)
	pCTRL->stDMCC.CC_Brightness=0;		
#else
	// pCTRL->stDMCC.CC_Brightness=248;		//-8
	pCTRL->stDMCC.CC_Brightness=0;		// set 0 for tunning
#endif
	
	
	pCTRL->stDMCC.offset_r = pCTRL->stDMCC.CC_Brightness;	
	pCTRL->stDMCC.offset_g = pCTRL->stDMCC.CC_Brightness;	
	pCTRL->stDMCC.offset_b = pCTRL->stDMCC.CC_Brightness;
#if defined(__USE_EXT2IPP__) ||  defined(__USE_NO__CMOS__)|| \
	defined(__USE_IMX265_MONO_SENSOR__) 
	SetDM_ColorCCtrl(4,4,4);
	DM_ColorMatrix_Call(4,4,4);
	pCTRL->stDMCC.color_corr_table=4;		
#else
	SetDM_ColorCCtrl(7,7,7);
	DM_ColorMatrix_Call(7,7,7);
	pCTRL->stDMCC.color_corr_table=7;		
#endif

}

static void Init_DM_REGISTER( PDM_CTRL_PARAM pCTRL ) {
#if defined(__USE_MDIN_i500__)    // MDIN-i500
	MISP_RegWrite(MISP_LOCAL_ID, 0x480, (pCTRL->bayer_pixel_id<<6)) ;
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x481, 0x7820);	
	
	//MISP_RegWrite(MISP_LOCAL_ID, 0x482, (pCTRL->dm_bypass<<15)|(pCTRL->dm_zip_bypass<<14)|(pCTRL->input_select<<13)|(0x080C));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x482, (pCTRL->dm_bypass<<15)|(pCTRL->dm_zip_bypass<<14)|(pCTRL->input_select<<13)|(0x0800));	
//	MISP_RegWrite(MISP_LOCAL_ID, 0x482, (pCTRL->dm_bypass<<15)|(pCTRL->dm_zip_bypass<<14)|(pCTRL->input_select<<13)|(0x08FF));	
	

	MISP_RegWrite(MISP_LOCAL_ID, 0x490, 0x001e);  //0019->001e	
	MISP_RegWrite(MISP_LOCAL_ID, 0x491, 0x282d);  //1e3c->282d	

	MISP_RegWrite(MISP_LOCAL_ID, 0x492, (pCTRL->bH<<8)|(pCTRL->bV));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x493, pCTRL->active_h_size);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x494, pCTRL->active_v_size);	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x495, 0x0000);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x496, 0x8220);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x497, 0x1914);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x498, 0x0FA0);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x499, 0xA0A0);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x49A, 0x0000);  //0050->0000	
#else                             // MDIN-i51X (i510, i540, i550)
//	MISP_RegWrite(MISP_LOCAL_ID, 0x477, 0x0400) ;
	MISP_RegWrite(MISP_LOCAL_ID, 0x477, 0x0450) ;
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x480, (pCTRL->bayer_pixel_id<<14)|(pCTRL->dm_bypass<<7)|(pCTRL->input_select<<6)|0x0000);

	MISP_RegWrite(MISP_LOCAL_ID, 0x481, 0x7820) ;
	MISP_RegWrite(MISP_LOCAL_ID, 0x482, 0x0004) ;
	MISP_RegWrite(MISP_LOCAL_ID, 0x483, 0x2818) ;
	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x47C, 0x0040) ;		// demosaic chroma suppression OFF
		
	MISP_RegWrite(MISP_LOCAL_ID, 0x47E, 0x00FF) ;		// high light threshold
	MISP_RegWrite(MISP_LOCAL_ID, 0x47F, 0x0064) ;
	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x48F, 0x0082) ;	
	MISP_RegWrite(MISP_LOCAL_ID, 0x490, 0x0153) ;	
	MISP_RegWrite(MISP_LOCAL_ID, 0x491, 0x0080) ;
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x492, (pCTRL->bH<<8)|(pCTRL->bV));
	MISP_RegWrite(MISP_LOCAL_ID, 0x493, pCTRL->active_h_size);
	MISP_RegWrite(MISP_LOCAL_ID, 0x494, pCTRL->active_v_size);
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x495, 0x33CD) ;
	MISP_RegWrite(MISP_LOCAL_ID, 0x496, 0x1AE6) ;
	MISP_RegWrite(MISP_LOCAL_ID, 0x498, 0xC80A) ;
//	MISP_RegWrite(MISP_LOCAL_ID, 0x498, 0x370A) ;
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x499, 0x40CC) ;
//	MISP_RegWrite(MISP_LOCAL_ID, 0x499, 0x1030) ;
	MISP_RegWrite(MISP_LOCAL_ID, 0x49A, 0x0006) ;		// high value : night mode.....low value : day mode..
	MISP_RegWrite(MISP_LOCAL_ID, 0x49B, 0x2400) ;		// B,R correction enable, G correction enable..
//	MISP_RegWrite(MISP_LOCAL_ID, 0x49B, 0x0000) ;		// .....when low light, turn on..
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x49C, 0x4080) ;
	MISP_RegWrite(MISP_LOCAL_ID, 0x49D, 0x0400) ;
	MISP_RegWrite(MISP_LOCAL_ID, 0x49E, 0x0A00) ;
	MISP_RegWrite(MISP_LOCAL_ID, 0x49F, 0x0060) ;
#endif  //__USE_MDIN_i500__

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// for color correction
	MISP_RegWrite(MISP_LOCAL_ID, 0x484, pCTRL->stDMCC.bayer_rr_gain);
	MISP_RegWrite(MISP_LOCAL_ID, 0x485, pCTRL->stDMCC.bayer_gr_gain);
	MISP_RegWrite(MISP_LOCAL_ID, 0x486, pCTRL->stDMCC.bayer_br_gain);
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x487, pCTRL->stDMCC.bayer_rg_gain);
	MISP_RegWrite(MISP_LOCAL_ID, 0x488, pCTRL->stDMCC.bayer_gg_gain);
	MISP_RegWrite(MISP_LOCAL_ID, 0x489, pCTRL->stDMCC.bayer_bg_gain);
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x48A, pCTRL->stDMCC.bayer_rb_gain);
	MISP_RegWrite(MISP_LOCAL_ID, 0x48B, pCTRL->stDMCC.bayer_gb_gain);
	MISP_RegWrite(MISP_LOCAL_ID, 0x48C, pCTRL->stDMCC.bayer_bb_gain);
	MISP_RegWrite(MISP_LOCAL_ID, 0x48D, (pCTRL->stDMCC.offset_r<<8)|(pCTRL->stDMCC.offset_g));
	MISP_RegWrite(MISP_LOCAL_ID, 0x48E, pCTRL->stDMCC.offset_b);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void Init_Demosaic(void)
{
	memset(&stDMCTRL, 0, sizeof(DM_CTRL_PARAM));
	Init_DM_PARAM(&stDMCTRL);
	Init_DM_REGISTER(&stDMCTRL);
}

// add by kw5210 for demo
void Init_DM_REGISTER_NIGHT(void)
{
	Init_Demosaic();
	MISP_RegWrite(MISP_LOCAL_ID, 0x49D, 0x0032) ;
}

// add by kw5210 for demo
void Init_DM_REGISTER_DAY(void)
{
	Init_Demosaic();
	MISP_RegWrite(MISP_LOCAL_ID, 0x49D, 0x0000) ;
}

//--------------------------------------------------------------------------------------------------------------------------
static void DMCTRL_InputSelectHandler(PDM_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&DMCTRL_INPUT_SEL_UPDATE)==0) return;
	pCTRL->ctrl &= (~DMCTRL_INPUT_SEL_UPDATE);	

#if defined(__USE_MDIN_i500__)    // MDIN-i500
	MISP_RegField(MISP_LOCAL_ID, 0x482, 13, 1, pCTRL->input_select);
#else                          // MDIN-i51X (i510, i540, i550)
	MISP_RegField(MISP_LOCAL_ID, 0x480, 6, 1, pCTRL->input_select);
#endif
}
//--------------------------------------------------------------------------------------------------------------------------
static void DMCTRL_BypassHandler(PDM_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&DMCTRL_BYPASS_UPDATE)==0) return;
	pCTRL->ctrl &= (~DMCTRL_BYPASS_UPDATE);	

#if defined(__USE_MDIN_i500__)    // MDIN-i500
	MISP_RegField(MISP_LOCAL_ID, 0x482, 14, 2, (pCTRL->dm_bypass<<1)|(pCTRL->dm_zip_bypass));
#else                          // MDIN-i51X (i510, i540, i550)
	MISP_RegField(MISP_LOCAL_ID, 0x480, 7, 1, (pCTRL->dm_bypass));
	MISP_RegField(MISP_LOCAL_ID, 0x473, 0, 1, (pCTRL->stDMCC.color_corr_bypass));
#endif
}
//--------------------------------------------------------------------------------------------------------------------------
static void DMCTRL_ColorCCtrlHandler(PDM_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&DMCTRL_CC_CNTL_CHANGE)==0) return;
	pCTRL->ctrl &= (~DMCTRL_CC_CNTL_CHANGE);

	MISP_RegWrite(MISP_LOCAL_ID, 0x484, pCTRL->stDMCC.bayer_rr_gain + pCTRL->stDMCC.CC_Contrast);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x485, pCTRL->stDMCC.bayer_gr_gain);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x486, pCTRL->stDMCC.bayer_br_gain);	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x487, pCTRL->stDMCC.bayer_rg_gain);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x488, pCTRL->stDMCC.bayer_gg_gain + pCTRL->stDMCC.CC_Contrast);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x489, pCTRL->stDMCC.bayer_bg_gain);	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x48A, pCTRL->stDMCC.bayer_rb_gain);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x48B, pCTRL->stDMCC.bayer_gb_gain);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x48C, pCTRL->stDMCC.bayer_bb_gain + pCTRL->stDMCC.CC_Contrast);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x48D, (pCTRL->stDMCC.offset_r<<8)|(pCTRL->stDMCC.offset_g));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x48E, pCTRL->stDMCC.offset_b);		
	
}

//--------------------------------------------------------------------------------------------------------------------------
static void DMCTRL_ImageSizeHandler(PDM_CTRL_PARAM pCTRL)
{
	if ((stDMCTRL.ctrl&DMCTRL_IMAGE_SIZE_UPDATE)==0) return;
	stDMCTRL.ctrl &= (~DMCTRL_IMAGE_SIZE_UPDATE);

	MISP_RegWrite(MISP_LOCAL_ID, 0x492, (pCTRL->bH<<8)|(pCTRL->bV));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x493, pCTRL->active_h_size);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x494, pCTRL->active_v_size);		
	
}

//--------------------------------------------------------------------------------------------------------------------------
static void DMCTRL_DataOrderHandler(PDM_CTRL_PARAM pCTRL)
{
	if ((stDMCTRL.ctrl&DMCTRL_DATA_ORDER_UPDATE)==0) return;
	stDMCTRL.ctrl &= (~DMCTRL_DATA_ORDER_UPDATE);
	
#if defined(__USE_MDIN_i500__)    // MDIN-i500
	MISP_RegWrite(MISP_LOCAL_ID, 0x480, (pCTRL->bayer_pixel_id<<6)) ;
#else                          // MDIN-i51X (i510, i540, i550)
	MISP_RegField(MISP_LOCAL_ID, 0x480, 14, 2, pCTRL->bayer_pixel_id);	
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void DM_ProcessHandler(void)
{
	if ((stDMCTRL.ctrl&DMCTRL_TRIGGER_ISP)==0) return;
	stDMCTRL.ctrl &= (~DMCTRL_TRIGGER_ISP);

	CC_ProcessHandler();						
	DMCTRL_InputSelectHandler(&stDMCTRL);
	DMCTRL_ImageSizeHandler(&stDMCTRL);
	DMCTRL_DataOrderHandler(&stDMCTRL);
	DMCTRL_BypassHandler(&stDMCTRL);

}
//--------------------------------------------------------------------------------------------------------------------------
void CC_ProcessHandler(void)
{
	DM_ColorMatrix_Call(stDMCTRL.stDMCC.color_corr_ctrl_red, stDMCTRL.stDMCC.color_corr_ctrl_green, stDMCTRL.stDMCC.color_corr_ctrl_blue);	
	DMCTRL_ColorCCtrlHandler(&stDMCTRL);
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
// GET / SET function

//--------------------------------------------------------------------------------------------------------------------------
void SetDM_Input_select(BOOL val)
{
	stDMCTRL.input_select = val;
	stDMCTRL.ctrl |= (DMCTRL_INPUT_SEL_UPDATE | DMCTRL_TRIGGER_ISP);
	
#if __MISP100_DBGPRT__ == 1
	UARTprintf("SetDM_Input_select : \r\n", (val==1) ? "From NR Top" : "From Bayer Top");
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDM_Bypass(BOOL val)
{
	stDMCTRL.dm_bypass = val;
	stDMCTRL.ctrl |= (DMCTRL_BYPASS_UPDATE | DMCTRL_TRIGGER_ISP);

#if __MISP100_DBGPRT__ == 1
	UARTprintf("SetDM_Bypass : \r\n", (val==1) ? "Bypass ON" : "Bypass OFF");
#endif	
}
//--------------------------------------------------------------------------------------------------------------------------
void SetDM_ZipBypass(BOOL val)
{
	stDMCTRL.dm_zip_bypass = val;
	stDMCTRL.ctrl |= (DMCTRL_BYPASS_UPDATE | DMCTRL_TRIGGER_ISP);

#if __MISP100_DBGPRT__ == 1
	UARTprintf("SetDM_Bypass : \r\n", (val==1) ? "Bypass ON" : "Bypass OFF");
#endif	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDM_Bayer_data_order (BYTE val )
{
	stDMCTRL.ctrl |= (DMCTRL_DATA_ORDER_UPDATE | DMCTRL_TRIGGER_ISP);

#if defined(__USE_MDIN_i500__)    // MDIN-i500
	stDMCTRL.bayer_pixel_id = val ;
#else                          // MDIN-i51X (i510, i540, i550)
	switch (val)
	{
		case MISP_SS_BAYER_PATTERN_0 : stDMCTRL.bayer_pixel_id = MISP_BY_BAYER_PATTERN_0 ; break ;
		case MISP_SS_BAYER_PATTERN_1 : stDMCTRL.bayer_pixel_id = MISP_BY_BAYER_PATTERN_1 ; break ;
		case MISP_SS_BAYER_PATTERN_2 : stDMCTRL.bayer_pixel_id = MISP_BY_BAYER_PATTERN_2 ; break ;
		case MISP_SS_BAYER_PATTERN_3 : stDMCTRL.bayer_pixel_id = MISP_BY_BAYER_PATTERN_3 ; break ;
		default : stDMCTRL.bayer_pixel_id = MISP_BY_BAYER_PATTERN_0 ; break ;
	}			 
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDM_image_size(BYTE bH , BYTE bV, WORD valH , WORD valV)
{
	stDMCTRL.ctrl |= (DMCTRL_IMAGE_SIZE_UPDATE | DMCTRL_TRIGGER_ISP);

	stDMCTRL.bH = bH;
	stDMCTRL.bV = bV;
	stDMCTRL.active_h_size = valH;
	stDMCTRL.active_v_size = valV;

}
//--------------------------------------------------------------------------------------------------------------------------
void SetDM_ColorCCtrl(WORD rval, WORD gval, WORD bval)
{
	SetDM_ColorCCtrl_Red(rval);
	SetDM_ColorCCtrl_Green(gval);
	SetDM_ColorCCtrl_Blue(bval);	
}
void SetDM_ColorCCtrl_Red(WORD val)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveCCRed(val);
#endif
	
	stDMCTRL.ctrl |= (DMCTRL_TRIGGER_ISP | DMCTRL_CC_CNTL_CHANGE);
	stDMCTRL.stDMCC.color_corr_ctrl_red=val;
}
void SetDM_ColorCCtrl_Green(WORD val)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveCCGreen(val);
#endif
	
	stDMCTRL.ctrl |= (DMCTRL_TRIGGER_ISP | DMCTRL_CC_CNTL_CHANGE);
	stDMCTRL.stDMCC.color_corr_ctrl_green=val;
}
void SetDM_ColorCCtrl_Blue(WORD val)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveCCBlue(val);
#endif
	
	stDMCTRL.ctrl |= (DMCTRL_TRIGGER_ISP | DMCTRL_CC_CNTL_CHANGE);
	stDMCTRL.stDMCC.color_corr_ctrl_blue=val;
}
//--------------------------------------------------------------------------------------------------------------------------
void SetDM_ColorCC_Brightness(WORD val)
{
	stDMCTRL.stDMCC.CC_Brightness=val;
	#if defined(__M33_V101__)
	if(val>=2) stDMCTRL.stDMCC.offset_g = stDMCTRL.stDMCC.CC_Brightness-2;	
	else if(val==1)  stDMCTRL.stDMCC.offset_g =254;	
	else stDMCTRL.stDMCC.offset_g =253;	

	DM_ColorMatrixOffset(stDMCTRL.stDMCC.CC_Brightness, stDMCTRL.stDMCC.offset_g, stDMCTRL.stDMCC.CC_Brightness);
	#else
	DM_ColorMatrixOffset(stDMCTRL.stDMCC.CC_Brightness, stDMCTRL.stDMCC.CC_Brightness, stDMCTRL.stDMCC.CC_Brightness);
	#endif
}
//--------------------------------------------------------------------------------------------------------------------------
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
//--------------------------------------------------------------------------------------------------------------------------
void SetDM_ColorCCEn(WORD val)
{
	if ( val == 0 )	stDMCTRL.stDMCC.color_corr_bypass = 1 ;	// Bypass
	else	stDMCTRL.stDMCC.color_corr_bypass = 0 ;			// Enable
		
	stDMCTRL.ctrl |= (DMCTRL_BYPASS_UPDATE | DMCTRL_TRIGGER_ISP);
}
#endif
//--------------------------------------------------------------------------------------------------------------------------
BYTE GetDM_ColorCCtrl_table(void)
{
	return stDMCTRL.stDMCC.color_corr_table;
}
WORD GetDM_ColorCC_Brightness (void)
{
	return stDMCTRL.stDMCC.CC_Brightness ;
}
//--------------------------------------------------------------------------------------------------------------------------
WORD GetDM_ColorCCtrl_Red(void)
{
	return stDMCTRL.stDMCC.color_corr_ctrl_red;
}
WORD GetDM_ColorCCtrl_Green(void)
{
	return stDMCTRL.stDMCC.color_corr_ctrl_green;
}
WORD GetDM_ColorCCtrl_Blue(void)
{
	return stDMCTRL.stDMCC.color_corr_ctrl_blue;
}

/*  FILE_END_HERE */
