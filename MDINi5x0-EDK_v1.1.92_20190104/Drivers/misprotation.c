// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"misp100.h"

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
//WORD MFC_sync_h_start = 0 ;
// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static ROT_CTRL_PARAM stROTCTRL;
// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
static void Init_ROT_PARAM( PROT_CTRL_PARAM pCTRL ) ;

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------
static void Init_ROT_PARAM( PROT_CTRL_PARAM pCTRL )
{
// start parameter for Rotation control		
	
	pCTRL->rot_enable = 0 ; 
}

//--------------------------------------------------------------------------------------------------------------------------
static void ROTCTRL_RotationSetHandler( PROT_CTRL_PARAM pCTRL )
{
	WORD src_h_in ;
	WORD src_v_in ;
	WORD h_start_MSB ;
	WORD h_start_LSB ;
	WORD read_mode ;
	
	if ( (pCTRL->ctrl&ROTCTRL_ROTATION_UPDATE) == 0 ) return ;
	pCTRL->ctrl &= (~ROTCTRL_ROTATION_UPDATE);	
	
	//set register
	src_h_in = pCTRL->v_size;
	src_v_in = pCTRL->h_size;
	if ( pCTRL->bit_precision == 0 ) // 12b + 8col => '/40'
	{
		if ( src_h_in%40 == 0 )
		{
			src_h_in = src_h_in/40 ;
		}
		else
		{
			src_h_in = src_h_in/40 + 1 ;
		}
		read_mode = 1 ;		// 8 col/trans
	}
	else // 10b + 4col => '/24'
	{
		if ( src_h_in%24 == 0 )
		{
			src_h_in = src_h_in/24 ;
		}
		else
		{
			src_h_in = src_h_in/24 + 1 ;
		}
		read_mode = 0 ;		// 4 col/trans
	}
	
	h_start_MSB = (pCTRL->h_start>>3) ;
	h_start_LSB = (pCTRL->h_start&0x0007) ;	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x7E9, (pCTRL->v_start));
	MISP_RegWrite(MISP_LOCAL_ID, 0x7EA, (src_h_in<<8)|0x0004|(read_mode<<1)|(pCTRL->rot_enable));
	MISP_RegWrite(MISP_LOCAL_ID, 0x7EB, (pCTRL->bit_precision)<<15|(0x0004<<12)|(src_v_in));
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x7EC, (pCTRL->total_v_size));
	MISP_RegWrite(MISP_LOCAL_ID, 0x7ED, (pCTRL->v_end));
	MISP_RegWrite(MISP_LOCAL_ID, 0x7EE, (h_start_MSB<<13)|(pCTRL->total_h_size));
	MISP_RegWrite(MISP_LOCAL_ID, 0x7EF, (h_start_LSB<<13)|(pCTRL->h_end));	
}

//--------------------------------------------------------------------------------------------------------------------------
void Init_ROT(void)
{
	memset(&stROTCTRL, 0, sizeof(ROT_CTRL_PARAM));
	Init_ROT_PARAM(&stROTCTRL);
	
	stROTCTRL.ctrl = 0 ;	
}

//--------------------------------------------------------------------------------------------------------------------------
void ROT_ProcessHandler(void)
{
	if ((stROTCTRL.ctrl&ROTCTRL_TRIGGER_ISP)==0) return;
	stROTCTRL.ctrl &= (~ROTCTRL_TRIGGER_ISP);

//	ROTCTRL_TEMPMFCSetHandler(&stROTCTRL); // Temp code for output window setting 
	ROTCTRL_RotationSetHandler(&stROTCTRL);	
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
void SetROT_RotationEnable( WORD val)	
{
	DWORD ratio_v, scaled_h_size;
	
	stROTCTRL.ctrl |= ( ROTCTRL_ROTATION_UPDATE | ROTCTRL_TRIGGER_ISP);

	if ( GetSS_image_active_size_v() > 1600 ) stROTCTRL.bit_precision = 1; // 0: 12b 1: 10b [V over 1600 cannot do faster mode at 12b]
	else stROTCTRL.bit_precision = 0; // 0: 12b 1: 10b [V under 1600 can do faster mode at 12b]	
	
	stROTCTRL.rot_enable = ((val > 0) && (val < 10))?  ON:  OFF;
	stROTCTRL.rot_mode = val;						// 0,10:rot_off, 1,2:rot_left, 3,4:rot_right
	
	stROTCTRL.h_size = MIN(GetSS_image_active_size_v(), GetOUT_MainSizeHA());	// sensor v_size(limit:v.out h_size)
	stROTCTRL.v_size = GetSS_image_active_size_h();
	
	MISP_RegField(MISP_LOCAL_ID, 0x5D0, 12, 1, 0);	// read input sync, 0:vsync, 1:hsync
	MISP_RegRead(MISP_LOCAL_ID, 0x5D1, &stROTCTRL.total_h_size);	// v_total[12:0] -> rot_h_total
	
	MISP_RegField(MISP_LOCAL_ID, 0x5D0, 12, 1, 1);	// read input sync, 0:vsync, 1:hsync
	MISP_RegRead(MISP_LOCAL_ID, 0x5D1, &stROTCTRL.total_v_size);	// h_total[12:0] -> rot_v_total
	
	stROTCTRL.total_h_size &= 0x1FFF;			stROTCTRL.total_v_size &= 0x1FFF;
	
	if(stROTCTRL.total_h_size < (stROTCTRL.h_size + 76))	stROTCTRL.total_h_size = stROTCTRL.h_size + 76;	// check h total low limit.
	
	stROTCTRL.h_start = 0;
	stROTCTRL.h_end = stROTCTRL.h_start + stROTCTRL.h_size;
	
	stROTCTRL.v_start = 0;
	stROTCTRL.v_end = stROTCTRL.v_start + stROTCTRL.v_size ;
	stROTCTRL.v_end -= (stROTCTRL.v_end > stROTCTRL.total_v_size)?  stROTCTRL.total_v_size:  0;
	
	SetOUT_MainDarkScreenAuto(60);	// main display turn-on delay = 16ms x 60 = 960ms

	if (stROTCTRL.rot_enable)
	{
		// clk power on
		SetCLK_ClkWDROnOff(1);
		SetCLK_ClkMGROnOff(1);
		SetCLK_ClkTMOnOff(1);
		
#if defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__)  ||  defined(__USE_CMOS_IMX226__)		// for 4k sensor frame rate limit(10.1fr_375Mhz, 10.8fr_399Mhz)
		if(stROTCTRL.rot_prev_mode==0 || stROTCTRL.rot_prev_mode==10)	{	// when rotation 'off' -> 'on'
			GetCLK_UserPLL(&stROTCTRL.spll_p, &stROTCTRL.spll_n, &stROTCTRL.spll_m, &stROTCTRL.spll_od, &stROTCTRL.spll_s);	// backup org parameter
			//SetCLK_UserPLL(1,3,86,3,1) ;		// 30frame(288Mhz) -> 10.1frame(96.75Mhz) -- 375Mhz imx274 fail safe 
			SetCLK_UserPLL(1,3,78,3,1) ;		// 30frame(288Mhz) -> 9.1frame(103.5Mhz) -- 396Mhz - imx274 fail safe 
			//SetCLK_UserPLL(1,3,92,3,1) ;		// 30frame(288Mhz) -> 10.8frame(103.5Mhz) -- 399Mhz
			stROTCTRL.spll_backup = TRUE;
		}
#endif
		
		stROTCTRL.bound_h_size = GetSS_image_boundary_size_h();			
		stROTCTRL.bound_v_size = GetSS_image_boundary_size_v();			
		stROTCTRL.active_h_size = GetSS_image_active_size_h();			
		stROTCTRL.active_v_size = GetSS_image_active_size_v();	
		
		SetDDRMAP_rotation_mode(1,0,0, stROTCTRL.v_size, stROTCTRL.h_size) ;
		SetSS_WDRSWimage_size(0,0, stROTCTRL.v_size, stROTCTRL.h_size) ;
		SetWDR_Rotation_mode(stROTCTRL.rot_enable, stROTCTRL.h_size, stROTCTRL.v_size, stROTCTRL.bit_precision) ;
		// SetNR_image_size(stROTCTRL.bound_v_size,stROTCTRL.bound_h_size,stROTCTRL.active_v_size,stROTCTRL.active_h_size);
		SetNR_image_size(0,0, stROTCTRL.h_size, stROTCTRL.v_size);
		SetNR_BitMode(0);
		// SetDM_image_size(stROTCTRL.bound_v_size,stROTCTRL.bound_h_size,stROTCTRL.active_v_size,stROTCTRL.active_h_size);
		// SetDM_image_size(0,0,stROTCTRL.active_v_size,stROTCTRL.active_h_size);
		SetDM_image_size(0,0, stROTCTRL.h_size, stROTCTRL.v_size);
		// SetDM_Bayer_data_order(MISP_SS_BAYER_PATTERN_2) ;
		
		if ( MISP_SS_BAYER_PATTERN_1 == GetSS_bayer_data_order() )
			SetDM_Bayer_data_order(MISP_SS_BAYER_PATTERN_2) ;
		else
			SetDM_Bayer_data_order(MISP_SS_BAYER_PATTERN_1) ;
		
		// SetNR_Mode(MISP_NR_OFF) ;
		SetBY_Input_select(0) ; // From WDR top
		
		// set mfc
		ratio_v = GetOUT_MainSizeVA() * 1000 /stROTCTRL.v_size;					// calc v_size scale ratio
		scaled_h_size = stROTCTRL.h_size * ratio_v / 1000;							// for h_size scale up/down
		scaled_h_size += ((stROTCTRL.h_size * ratio_v % 1000) >= 500)?  1:  0;	// for round up
		scaled_h_size &= 0xFFFFFFF0;												// for even pixel size
		
		SetIN_MainSrcSizeH(stROTCTRL.h_size);	SetIN_MainSrcSizeV(stROTCTRL.v_size);
		SetFC_MainFCsrcPosiX(0);				SetFC_MainFCsrcSizeH(stROTCTRL.h_size);
		SetFC_MainFCsrcPosiY(0);				SetFC_MainFCsrcSizeV(stROTCTRL.v_size);
		
		if((stROTCTRL.rot_mode == 2) || (stROTCTRL.rot_mode == 4))	{		// 1:1 mode
			SetFC_MainFCdstPosiX((GetOUT_MainSizeHA() - stROTCTRL.h_size) /2);			// position(center)
			SetFC_MainFCdstSizeH(stROTCTRL.h_size);
			SetFC_MainFCdstPosiY(0);
			SetFC_MainFCdstSizeV(stROTCTRL.v_size);
		}
		else	{															// full screen mode
			SetFC_MainFCdstPosiX((GetOUT_MainSizeHA() - scaled_h_size) /2);			// position(center)
			SetFC_MainFCdstSizeH(scaled_h_size);
			SetFC_MainFCdstPosiY(0);
			SetFC_MainFCdstSizeV(GetOUT_MainSizeVA());
		}
	}
	else // rotation off
	{
		// clk power off
		SetCLK_ClkWDROnOff(0);
		SetCLK_ClkMGROnOff(0);
		SetCLK_ClkTMOnOff(0);
		
#if defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__)  ||  defined(__USE_CMOS_IMX226__)		// for 4k sensor frame rate limit(10.1fr_375Mhz, 10.8fr_399Mhz)
		if(stROTCTRL.spll_backup)	{
			SetCLK_UserPLL(stROTCTRL.spll_p, stROTCTRL.spll_n, stROTCTRL.spll_m, stROTCTRL.spll_od, stROTCTRL.spll_s);	// restore org parameter
			stROTCTRL.spll_backup = FALSE;
		}
#endif
		
		SetDDRMAP_rotation_mode(0,stROTCTRL.bound_h_size,stROTCTRL.bound_v_size,stROTCTRL.active_h_size,stROTCTRL.active_v_size) ;
		SetSS_image_size(stROTCTRL.bound_h_size,stROTCTRL.bound_v_size,stROTCTRL.active_h_size,stROTCTRL.active_v_size) ; // "SetSS_image_size" calls "SetSS_WDRSWimage_size"
		SetWDR_Rotation_mode(stROTCTRL.rot_enable, stROTCTRL.active_h_size, stROTCTRL.active_v_size, stROTCTRL.bit_precision) ;
		// SetDM_Bayer_data_order(MISP_SS_BAYER_PATTERN_1) ;
#if !(defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX226__)  ||  defined(__USE_CMOS_IMX326__))
		SetNR_BitMode(1);
#endif	
		SetDM_Bayer_data_order(GetSS_bayer_data_order()) ;		
		// SetNR_Mode(MISP_NR_2D3D) ;
		SetBY_Input_select(1) ; // From Sensor top
		
		// set mfc
		SetIN_MainSrcSizeH(GetSS_image_active_size_h());
		SetIN_MainSrcSizeV(GetSS_image_active_size_v());
		SetFC_MainFCsrcPosiX(0);				SetFC_MainFCsrcSizeH(GetIN_MainSrcSizeH());
		SetFC_MainFCsrcPosiY(0);				SetFC_MainFCsrcSizeV(GetIN_MainSrcSizeV());
		SetFC_MainFCdstPosiX(0);				SetFC_MainFCdstSizeH(GetOUT_MainSizeHA());
		SetFC_MainFCdstPosiY(0);				SetFC_MainFCdstSizeV(GetOUT_MainSizeVA());
	}
	
	stROTCTRL.rot_prev_mode = stROTCTRL.rot_mode;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL GetROT_RotationEnable( void )
{
	return stROTCTRL.rot_enable ;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE GetROT_BitMode( void )
{
	if ( stROTCTRL.bit_precision == 1 ) return MISP_DDRMAP_10b ;
	else return MISP_DDRMAP_12b ;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE GetROT_RotationMode(void)
{
	return stROTCTRL.rot_mode;
}

#endif // MDIN-i51X (i510, i540, i550)

/*  FILE_END_HERE */
