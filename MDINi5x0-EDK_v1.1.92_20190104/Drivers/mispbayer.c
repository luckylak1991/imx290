// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"misp100.h"
#include	"video.h"

#if defined(__USE_MDIN_i500__)		// MDIN-i500
#include 	"mispgammatable.h"
#else                           // MDIN-i51X (i510, i540, i550)
#include 	"mispgammatable510.h"
#endif

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static BY_CTRL_PARAM stBYCTRL;

#if __USE_GAMMA_BYTEDATA__
static BYTE gamma_current_value[32];
#else
static WORD gamma_current_value[16];
#endif

BYTE global_histo64[64];

static BYTE ghisto64[64];
//static BYTE ghmin, ghmax, ghcenter_offset, ghcenter;
//static BYTE gxl_auto, gyn_auto;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
#if defined(__USE_MDIN_i500__)
static void BYCTRL_SetGammaTable(PBY_CTRL_PARAM pCTRL);
#endif
static void BYCTRL_set_dwdr_strength_step(PBY_CTRL_PARAM pCTRL , int strength, int saturation, int init);
static void BYCTRL_set_defog_strength_step(PBY_CTRL_PARAM pCTRL , int strength, int saturation, int init);

static void BYCTRL_BYGammaEnHandler( PBY_CTRL_PARAM pCTRL );
static void BYCTRL_BYGammaExtendHandler( PBY_CTRL_PARAM pCTRL ) ;
static void BYCTRL_BYGammaValueHandler(PBY_CTRL_PARAM pCTRL);
// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------
void BYCTRL_dwdr_force_set(void);
void BYCTRL_defog_force_set(void);
void BYCTRL_defog_dwdr_all_off(void);


static void Init_BAYER_PARAM( PBY_CTRL_PARAM pCTRL ){

	
	pCTRL->input_select = 1 ;
	pCTRL->bayer_bypass = 0 ;
	pCTRL->bayer_5m_bypass = 0 ;
	
	pCTRL->bayer_pixel_id = GetSS_bayer_data_order() ;
	
//	pCTRL->bH = GetSS_image_boundary_size_h() ;
//	pCTRL->bV = GetSS_image_boundary_size_v() ;
	pCTRL->bH = 0 ;
	pCTRL->bV = 0 ;
	pCTRL->active_h_size = GetSS_image_active_size_h() + GetSS_image_boundary_size_h()*2;
	pCTRL->active_v_size = GetSS_image_active_size_v() + GetSS_image_boundary_size_v()*2;
	// pCTRL->active_h_size = GetSS_sensor_hactive_read() ;
	// pCTRL->active_v_size = GetSS_sensor_vactive_read() ;
	
	pCTRL->stLENSCORR.lens_corr_en = 0 ;
	pCTRL->stLENSCORR.outer_rad_rel = 168 ;
	pCTRL->stLENSCORR.inner_rad_rel = 70 ;
	pCTRL->stLENSCORR.lens_corr_intensity = 100 ;
	
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)	
	pCTRL->stDEFOGDWDR.defog_dwdr_mode = MISP_DEFOG_DWDR_OFF ;
#endif	
	pCTRL->stDEFOGDWDR.defog_mode = MISP_DEFOG_OFF ;
	
	//pCTRL->stDEFOGDWDR.defog_strength = 8;
	// menu 16 to 32
	pCTRL->stDEFOGDWDR.defog_strength = 16;
	pCTRL->stDEFOGDWDR.dwdr_strength = 16;
	
#if defined(__USE_MDIN_i500__)			// MDIN-i500	
	pCTRL->stDEFOGDWDR.defog_threshold = 60 ;
	pCTRL->stDEFOGDWDR.defog_skya_ratio = 46 ;
	pCTRL->stDEFOGDWDR.defog_skya_offset = 40  ;
	pCTRL->stDEFOGDWDR.defog_intensity = 100 ;
#else 									// MDIN-i51X (i510, i540, i550)
	pCTRL->stDEFOGDWDR.defog_threshold = 0 ;
#endif	
	pCTRL->stDEFOGDWDR.defog_ovr_weight = 127 ;
	pCTRL->stDEFOGDWDR.defog_weight = 127 ;
	
	pCTRL->stDEFOGDWDR.dwdr_mode = MISP_DWDR_OFF ;

#if defined(__USE_MDIN_i500__)			// MDIN-i500
	pCTRL->stDEFOGDWDR.dwdr_weight = 0 ;		// 0 and on is default.
#else 									// MDIN-i51X (i510, i540, i550)
	pCTRL->stDEFOGDWDR.dwdr_weight = 8 ;
#endif

#if __USE_GAMMA_BYTEDATA__		//i5x0	
	pCTRL->stGAMMA.gamma_default = MISP_GAMMA_045 ;	
	
	SetBY_GammaValue(8) ; // MISP_GAMMA_045
///////////////////////////////////////////
// SetBY_GammaValue
// 0 : gamma_default  | 1 : MISP_GAMMA_OFF | 2 : MISP_GAMMA_090
// 3 : MISP_GAMMA_080 | 4 : MISP_GAMMA_070 | 5 : MISP_GAMMA_060
// 6 : MISP_GAMMA_055 | 7 : MISP_GAMMA_050 | 8 : MISP_GAMMA_045
// 9 : MISP_GAMMA_040 | 10 : MISP_GAMMA_CUSTOM3
///////////////////////////////////////////	

#if defined(__USE_EXT2IPP__)
	SetBY_gamma_enable(OFF) ;
#else	//default
	SetBY_gamma_enable(ON) ;
#endif

	SetBY_BY256LUT_Extend(OFF) ;
	
	SetBY_BYLUT_Type(MISP_LUT_64x256) ;
#else							//i500
	pCTRL->stGAMMA.gamma_en = 1 ;

	#if	defined(__USE_CMOS_OV2710__)
	pCTRL->stGAMMA.gamma_r = MISP_GAMMA_050 ; 
	pCTRL->stGAMMA.gamma_g = MISP_GAMMA_050 ;
	pCTRL->stGAMMA.gamma_b = MISP_GAMMA_050 ;		
	pCTRL->stGAMMA.gamma_default = MISP_GAMMA_050 ;		
	#elif	defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__) 
	pCTRL->stGAMMA.gamma_r = MISP_GAMMA_045 ; 
	pCTRL->stGAMMA.gamma_g = MISP_GAMMA_045 ;
	pCTRL->stGAMMA.gamma_b = MISP_GAMMA_045 ;		
	pCTRL->stGAMMA.gamma_default = MISP_GAMMA_045 ;		
	#else
	pCTRL->stGAMMA.gamma_r = MISP_GAMMA_040 ; 
	pCTRL->stGAMMA.gamma_g = MISP_GAMMA_040 ;
	pCTRL->stGAMMA.gamma_b = MISP_GAMMA_040 ;		
	pCTRL->stGAMMA.gamma_default = MISP_GAMMA_040 ;		
	#endif

	pCTRL->stDEFOGDWDR.dwdr_natural_gamma=0;		//save menu status(gamma)
	pCTRL->stDEFOGDWDR.dwdr_on_gamma=0;
	pCTRL->stDEFOGDWDR.dwdr_off_gamma=0;	
	
	pCTRL->stDEFOGDWDR.dwdr_natural_aetarget=6;
	pCTRL->stDEFOGDWDR.dwdr_on_aetarget=6;
	pCTRL->stDEFOGDWDR.dwdr_off_aetarget=6;	
#endif

}

static void Init_BAYER_REGISTER( PBY_CTRL_PARAM pCTRL ) {
	
	BOOL dwdr_en = 0 ;
	BOOL dwdr_force = 0 ;
	BOOL defog_en ;
	BOOL defog_force ;
	WORD in_hsize ;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	BYTE dwdr_strength=0x00;
#endif
	
	in_hsize = pCTRL->active_h_size + (pCTRL->bH<<1) ;	
	
	switch (pCTRL->stDEFOGDWDR.dwdr_mode) {
#if defined(__USE_MDIN_i500__)												// MDIN-i500
		case MISP_DWDR_OFF :	dwdr_en = 1;	dwdr_force = 0; break;
		case MISP_DWDR_AUTO :	dwdr_en = 1;	dwdr_force = 0; break;
		case MISP_DWDR_ON :		dwdr_en = 0;	dwdr_force = 1; break;
		default :				dwdr_en = 1;	dwdr_force = 0; break;
#else																		// MDIN-i51X (i510, i540, i550)
		//case MISP_DWDR_OFF :	dwdr_en = 0;	dwdr_force = 0;	dwdr_strength=0x64; 	break;
		//case MISP_DWDR_LOW :	dwdr_en = 0;	dwdr_force = 1;	dwdr_strength=0x64;		break;
		//case MISP_DWDR_MID :	dwdr_en = 0;	dwdr_force = 1;	dwdr_strength=0x6F;		break;
		//case MISP_DWDR_HIGH :	dwdr_en = 0;	dwdr_force = 1;	dwdr_strength=0x78;		break;
		//default:				dwdr_en = 0;	dwdr_force = 0;	dwdr_strength=0x64; 	break;
#endif
	}	

	switch (pCTRL->stDEFOGDWDR.defog_mode) {
		case MISP_DEFOG_OFF :	defog_en = 0;	defog_force = 0; break;	
#if defined(__USE_MDIN_i500__)												// MDIN-i500
		case MISP_DEFOG_AUTO :	defog_en = 1;	defog_force = 0; break;			
#endif
		case MISP_DEFOG_ON :	defog_en = 0;	defog_force = 1; break;			
		default :				defog_en = 0;	defog_force = 0; break;		
	}		
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x400, (pCTRL->bayer_5m_bypass<<15)|(pCTRL->bayer_pixel_id<<13)|(pCTRL->input_select));	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x401, (in_hsize>>1));	// 
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x406, (pCTRL->stLENSCORR.lens_corr_en));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x407, (pCTRL->bH));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x408, (pCTRL->bV));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x409, (pCTRL->active_h_size));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x40A, (pCTRL->active_v_size));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x40B, (pCTRL->stLENSCORR.outer_rad_rel<<8)|(pCTRL->stLENSCORR.inner_rad_rel));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x40C, (pCTRL->stLENSCORR.lens_corr_intensity));	//
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x410, (pCTRL->bayer_bypass<<15));	//
	
#if defined(__USE_MDIN_i500__)										// MDIN-i500
	MISP_RegWrite(MISP_LOCAL_ID, 0x411, (pCTRL->bayer_pixel_id));	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x412, (defog_force<<3)|(dwdr_force<<2)|(defog_en<<1)|(dwdr_en));	//

	MISP_RegWrite(MISP_LOCAL_ID, 0x414, (pCTRL->stDEFOGDWDR.defog_threshold<<8)|(pCTRL->stDEFOGDWDR.defog_intensity));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x415, (pCTRL->stDEFOGDWDR.defog_skya_ratio<<8)|(pCTRL->stDEFOGDWDR.defog_skya_offset));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x416, (pCTRL->stDEFOGDWDR.defog_ovr_weight));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x417, (pCTRL->stDEFOGDWDR.defog_weight<<8)|(pCTRL->stDEFOGDWDR.dwdr_weight));	//
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x418, 0x0000);	//	
#else																// MDIN-i51X (i510, i540, i550)
	MISP_RegWrite(MISP_LOCAL_ID, 0x411, (0x1E00<<2) | (pCTRL->bayer_pixel_id&0x03));	// defog parameter(reg. name ?)
	MISP_RegWrite(MISP_LOCAL_ID, 0x412, (defog_force<<3)|(dwdr_force<<2)|(defog_en<<1)|(dwdr_en));	//

	//MISP_RegWrite(MISP_LOCAL_ID, 0x413, 0x7500);			// dwdr parameter(reg. name ?)
#if defined (__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__)
	MISP_RegWrite(MISP_LOCAL_ID, 0x413, 0x7480);			// dwdr parameter(reg. name ?)
#else
	//MISP_RegWrite(MISP_LOCAL_ID, 0x413, 0x7340);			// dwdr parameter(reg. name ?)
	MISP_RegWrite(MISP_LOCAL_ID, 0x413, 0x7480);			// dwdr parameter(reg. name ?)
#endif
	MISP_RegWrite(MISP_LOCAL_ID, 0x414, 0x1e10);			// dwdr parameter(reg. name ?)
	MISP_RegWrite(MISP_LOCAL_ID, 0x415, 0x0011);			// dwdr parameter(reg. name ?)
	MISP_RegWrite(MISP_LOCAL_ID, 0x416, 0xaa82);			// defog parameter(reg. name ?)
	MISP_RegWrite(MISP_LOCAL_ID, 0x417, 0x6e64);			// defog parameter(reg. name ?)
	
	//MISP_RegWrite(MISP_LOCAL_ID, 0x418, 0xf0c8 | 0x0002);	// defog & dwdr parameter(reg. name ?)
	MISP_RegWrite(MISP_LOCAL_ID, 0x418, 0xfd40 | 0x0002);	// defog & dwdr parameter(reg. name ?)
	MISP_RegWrite(MISP_LOCAL_ID, 0x419, 0x0000 | 0x007f);	// defog & dwdr parameter(reg. name ?)
	MISP_RegWrite(MISP_LOCAL_ID, 0x41D, 0x4000);			// defog parameter(reg. name ?)
	MISP_RegWrite(MISP_LOCAL_ID, 0x41E, 0x4B4B);			// 
	
	//MISP_RegWrite(MISP_LOCAL_ID, 0x40D, 0x6400);			// dwdr parameter(reg. name ?)
	MISP_RegWrite(MISP_LOCAL_ID, 0x40D, MAKEWORD(dwdr_strength, 0x00));	// dwdr parameter(reg. name ?)
		
	//MISP_RegWrite(MISP_LOCAL_ID, 0x40F, 0x0300);			// defog parameter(reg. name ?)
	MISP_RegWrite(MISP_LOCAL_ID, 0x40F, 0x0000);			// defog parameter(reg. name ?)
#endif
	MISP_RegWrite(MISP_LOCAL_ID, 0x420, 0x5050);	//
	//MISP_RegWrite(MISP_LOCAL_ID, 0x421, (pCTRL->stGAMMA.gamma_en<<15)|(pCTRL->bayer_pixel_id<<12)|(pCTRL->stGAMMA.gamma_r<<8)|(pCTRL->stGAMMA.gamma_g<<4)|(pCTRL->stGAMMA.gamma_b));	//
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	BYCTRL_SetGammaTable(pCTRL);
	MISP_RegWrite(MISP_LOCAL_ID, 0x22A, 125);	//save for misp app
#endif	
}

void Init_BayerFR(void) {	
	memset(&stBYCTRL, 0, sizeof(BY_CTRL_PARAM));
	Init_BAYER_PARAM(&stBYCTRL);
	Init_BAYER_REGISTER(&stBYCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
static void BYCTRL_InputSelectHandler(PBY_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&BYCTRL_INPUT_SEL_UPDATE)==0) return;
	pCTRL->ctrl &= (~BYCTRL_INPUT_SEL_UPDATE);	

	MISP_RegField(MISP_LOCAL_ID, 0x400, 0, 1, pCTRL->input_select);
}
//--------------------------------------------------------------------------------------------------------------------------
static void BYCTRL_BypassHandler(PBY_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&BYCTRL_BYPASS_UPDATE)==0) return;
	pCTRL->ctrl &= (~BYCTRL_BYPASS_UPDATE);	

	MISP_RegField(MISP_LOCAL_ID, 0x410, 15, 1, pCTRL->bayer_bypass);
	MISP_RegField(MISP_LOCAL_ID, 0x400, 15, 1, pCTRL->bayer_5m_bypass);
}
//--------------------------------------------------------------------------------------------------------------------------
static void BYCTRL_LensCorrEnHandler(PBY_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&BYCTRL_LENS_CORR_UPDATE)==0) return;
	pCTRL->ctrl &= (~BYCTRL_LENS_CORR_UPDATE);	

	MISP_RegWrite(MISP_LOCAL_ID, 0x406, pCTRL->stLENSCORR.lens_corr_en);
	MISP_RegWrite(MISP_LOCAL_ID, 0x40B, (pCTRL->stLENSCORR.outer_rad_rel<<8)|(pCTRL->stLENSCORR.inner_rad_rel));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x40C, (pCTRL->stLENSCORR.lens_corr_intensity));	//
}
//--------------------------------------------------------------------------------------------------------------------------
static void BYCTRL_DWDRModeHandler(PBY_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&BYCTRL_DWDR_UPDATE)==0) return;
	pCTRL->ctrl &= (~BYCTRL_DWDR_UPDATE);	
	
#if defined(__USE_MDIN_i500__)	
	switch (pCTRL->stDEFOGDWDR.dwdr_mode) {
	case MISP_DWDR_OFF :
		MISP_RegField(MISP_LOCAL_ID, 0x412, 0, 1, 1);
		MISP_RegField(MISP_LOCAL_ID, 0x412, 2, 1, 0); break;
		MISP_RegField(MISP_LOCAL_ID, 0x417, 0, 4, 0);	//		
	case MISP_DWDR_AUTO :
		MISP_RegField(MISP_LOCAL_ID, 0x412, 0, 1, 1);
		MISP_RegField(MISP_LOCAL_ID, 0x412, 2, 1, 0); break;
		MISP_RegField(MISP_LOCAL_ID, 0x417, 0, 4, 2);	//			
	case MISP_DWDR_ON :
		MISP_RegField(MISP_LOCAL_ID, 0x412, 0, 1, 0);
		MISP_RegField(MISP_LOCAL_ID, 0x412, 2, 1, 1); break;
		MISP_RegField(MISP_LOCAL_ID, 0x417, 0, 4, 4);	//		
	default :
		MISP_RegField(MISP_LOCAL_ID, 0x412, 0, 1, 0);
		MISP_RegField(MISP_LOCAL_ID, 0x412, 2, 1, 0); break; 	
	#if 0															// MDIN-i51X
	case MISP_DWDR_OFF :
		MISP_RegField(MISP_LOCAL_ID, 0x412, 0, 1, 0);
		MISP_RegField(MISP_LOCAL_ID, 0x412, 2, 1, 0);
		MISP_RegWrite(MISP_LOCAL_ID, 0x418, MAKEWORD(0xFD, 0x42));
		MISP_RegWrite(MISP_LOCAL_ID, 0x40D, MAKEWORD(0x64, 0x00));	break;
	case MISP_DWDR_LOW :  //no local
		MISP_RegField(MISP_LOCAL_ID, 0x412, 0, 1, 0);
		MISP_RegField(MISP_LOCAL_ID, 0x412, 2, 1, 1);
		//MISP_RegWrite(MISP_LOCAL_ID, 0x418, MAKEWORD(0xFD, 0x42));
		MISP_RegWrite(MISP_LOCAL_ID, 0x418, MAKEWORD(0xFD, 0x62));
		MISP_RegWrite(MISP_LOCAL_ID, 0x419, MAKEWORD(0x78, 0x7F));
		//MISP_RegWrite(MISP_LOCAL_ID, 0x40D, MAKEWORD(0x6F, 0x00));	break;
		MISP_RegWrite(MISP_LOCAL_ID, 0x40D, MAKEWORD(0x78, 0x00));	break;
	case MISP_DWDR_MID :  //pixel local
		MISP_RegField(MISP_LOCAL_ID, 0x412, 0, 1, 0);
		MISP_RegWrite(MISP_LOCAL_ID, 0x418, MAKEWORD(0xFF, 0x42));
		MISP_RegField(MISP_LOCAL_ID, 0x412, 2, 1, 1);
		MISP_RegWrite(MISP_LOCAL_ID, 0x419, MAKEWORD(0x78, 0x00));
		//MISP_RegWrite(MISP_LOCAL_ID, 0x40D, MAKEWORD(0x64, 0x00));	break;
		MISP_RegWrite(MISP_LOCAL_ID, 0x40D, MAKEWORD(0x78, 0x00));	break;
	case MISP_DWDR_HIGH :  //box local
		MISP_RegField(MISP_LOCAL_ID, 0x412, 0, 1, 0);
		MISP_RegField(MISP_LOCAL_ID, 0x412, 2, 1, 1);
		MISP_RegWrite(MISP_LOCAL_ID, 0x418, MAKEWORD(0xFD, 0x42));
		MISP_RegWrite(MISP_LOCAL_ID, 0x419, MAKEWORD(0x78, 0x00));
		MISP_RegWrite(MISP_LOCAL_ID, 0x40D, MAKEWORD(0x78, 0x00));	break;
	default :
		MISP_RegField(MISP_LOCAL_ID, 0x412, 0, 1, 0);
		MISP_RegField(MISP_LOCAL_ID, 0x412, 2, 1, 0);
		MISP_RegWrite(MISP_LOCAL_ID, 0x40D, MAKEWORD(0x64, 0x00));	break;
	#endif // if 0	
	}		

#else  // MDIN-i51X (i510, i540, i550)
		
	//UARTprintf("DWDR BYCTRL_DWDRModeHandler:%d\r\n", pCTRL->stDEFOGDWDR.dwdr_mode);
	if(pCTRL->stDEFOGDWDR.dwdr_mode == MISP_DWDR_OFF)
	{
		if(pCTRL->stDEFOGDWDR.defog_mode == MISP_DEFOG_OFF){
			pCTRL->stDEFOGDWDR.set_mode = SET_DWDR_DEFOG_ALL_OFF;
			BYCTRL_defog_dwdr_all_off();		
		}
		else if(pCTRL->stDEFOGDWDR.defog_mode == MISP_DEFOG_ON)
			pCTRL->stDEFOGDWDR.set_mode = SET_DEFOG_FORCE;
		else if(pCTRL->stDEFOGDWDR.defog_mode == MISP_DEFOG_AUTO)
			pCTRL->stDEFOGDWDR.set_mode = SET_DEFOG_AUTO;
	}
	else if(pCTRL->stDEFOGDWDR.dwdr_mode == MISP_DWDR_ON) 
	{
		//UARTprintf("MISP_DWDR_ON:stength%d\r\n", pCTRL->stDEFOGDWDR.dwdr_strength);
		pCTRL->stDEFOGDWDR.set_mode = SET_DWDR_FORCE;	
		BYCTRL_set_dwdr_strength_step(pCTRL, pCTRL->stDEFOGDWDR.dwdr_strength, pCTRL->stDEFOGDWDR.dwdr_saturation, 1);
		//BYCTRL_set_dwdr_strength_step(pCTRL, pCTRL->stDEFOGDWDR.dwdr_strength, pCTRL->stDEFOGDWDR.dwdr_saturation, 0);
		BYCTRL_dwdr_force_set();
	}
	else if(pCTRL->stDEFOGDWDR.dwdr_mode == MISP_DWDR_AUTO)
	{	
		if(pCTRL->stDEFOGDWDR.defog_mode == MISP_DEFOG_AUTO)
			pCTRL->stDEFOGDWDR.set_mode = SET_DWDR_DEFOG_FULL_AUTO;
		else
			pCTRL->stDEFOGDWDR.set_mode = SET_DWDR_AUTO;
	}
		
#endif
		

}

//--------------------------------------------------------------------------------------------------------------------------
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
static void BYCTRL_DWDRStrengthHandler(PBY_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&BYCTRL_DWDR_STRENGTH_UPDATE)==0) return;
	pCTRL->ctrl &= (~BYCTRL_DWDR_STRENGTH_UPDATE);	
	
	BYCTRL_set_dwdr_strength_step(pCTRL, (int)pCTRL->stDEFOGDWDR.dwdr_strength, (int)pCTRL->stDEFOGDWDR.dwdr_saturation_cal, 1);	
	//BYCTRL_set_dwdr_strength_step(pCTRL, (int)pCTRL->stDEFOGDWDR.dwdr_strength, (int)pCTRL->stDEFOGDWDR.dwdr_saturation_cal, 0);		
}

static void BYCTRL_DWDRSaturationHandler(PBY_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&BYCTRL_DWDR_SAT_UPDATE)==0) return;
	pCTRL->ctrl &= (~BYCTRL_DWDR_SAT_UPDATE);		
	
	BYTE val;

	val = stBYCTRL.stDEFOGDWDR.dwdr_saturation;
	
	//val *= 8;
	val *= 4;
	//if(val=128)
	if(val>=128)
		val= 127;
	//UARTprintf("SaturationHandler:val:%d\n", val);
	MISP_RegField(MISP_LOCAL_ID, 0x419, 8, 7 , val);
}

static void BYCTRL_DWDRLocalRatioHandler(PBY_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&BYCTRL_DWDR_LOCAL_RATIO_UPDATE)==0) return;
	pCTRL->ctrl &= (~BYCTRL_DWDR_LOCAL_RATIO_UPDATE);		
	
	BYTE val;

		val = stBYCTRL.stDEFOGDWDR.dwdr_local_ratio;
	val *= 8;
	if(val>=128)
		val= 127;
	MISP_RegField(MISP_LOCAL_ID, 0x419, 0, 7 , val);

}
#endif
//--------------------------------------------------------------------------------------------------------------------------
static void BYCTRL_DefogModeHandler(PBY_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&BYCTRL_DEFOG_MODE_UPDATE)==0) return;
	pCTRL->ctrl &= (~BYCTRL_DEFOG_MODE_UPDATE);	

#if defined(__USE_MDIN_i500__)	
	switch (pCTRL->stDEFOGDWDR.defog_mode) {
	case MISP_DEFOG_OFF :
		MISP_RegField(MISP_LOCAL_ID, 0x412, 1, 1, 0);
		MISP_RegField(MISP_LOCAL_ID, 0x412, 3, 1, 0); break;	
	case MISP_DEFOG_AUTO :
		MISP_RegField(MISP_LOCAL_ID, 0x412, 1, 1, 1);
		MISP_RegField(MISP_LOCAL_ID, 0x412, 3, 1, 0); break;		
	case MISP_DEFOG_ON :
		MISP_RegField(MISP_LOCAL_ID, 0x412, 1, 1, 0);
		MISP_RegField(MISP_LOCAL_ID, 0x412, 3, 1, 1); break;		
	default :
		MISP_RegField(MISP_LOCAL_ID, 0x412, 1, 1, 0);
		MISP_RegField(MISP_LOCAL_ID, 0x412, 3, 1, 0); break;		
	}
#else // MDIN-i51X (i510, i540, i550)
	
	if(pCTRL->stDEFOGDWDR.defog_mode == MISP_DEFOG_OFF) // off
	{
		//UARTprintf("DfModeHandler off\n");
		if(pCTRL->stDEFOGDWDR.dwdr_mode == MISP_DWDR_OFF){
			pCTRL->stDEFOGDWDR.set_mode = SET_DWDR_DEFOG_ALL_OFF;	
			BYCTRL_defog_dwdr_all_off();		
		}
		else if(pCTRL->stDEFOGDWDR.dwdr_mode == MISP_DWDR_ON)
			pCTRL->stDEFOGDWDR.set_mode = SET_DWDR_FORCE;
		else if(pCTRL->stDEFOGDWDR.dwdr_mode == MISP_DWDR_AUTO)
			pCTRL->stDEFOGDWDR.set_mode = SET_DWDR_AUTO;		
	}
	else if(pCTRL->stDEFOGDWDR.defog_mode == MISP_DEFOG_ON) // on
	{
		//UARTprintf("DfModeHandler on\n");
		//UARTprintf("DfModeHand:strength%d\r\n", pCTRL->stDEFOGDWDR.defog_strength);
		pCTRL->stDEFOGDWDR.set_mode = SET_DEFOG_FORCE;	
		BYCTRL_set_defog_strength_step(pCTRL, pCTRL->stDEFOGDWDR.defog_strength, pCTRL->stDEFOGDWDR.dwdr_saturation, 1);
		BYCTRL_defog_force_set();
	}
	else if(pCTRL->stDEFOGDWDR.defog_mode == MISP_DEFOG_AUTO)	// auto
	{
		//UARTprintf("DfModeHandler auto\n");
		if(pCTRL->stDEFOGDWDR.dwdr_mode == MISP_DWDR_AUTO)
			pCTRL->stDEFOGDWDR.set_mode = SET_DWDR_DEFOG_FULL_AUTO;
		else
			pCTRL->stDEFOGDWDR.set_mode = SET_DEFOG_AUTO;	
	}

	//UARTprintf("DWDR/DEFOG set mode:%d\r\n", pCTRL->stDEFOGDWDR.set_mode);

#endif
}
//--------------------------------------------------------------------------------------------------------------------------
static void BYCTRL_DefogStrengthHandler(PBY_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&BYCTRL_DEFOG_STRENGTH_UPDATE)==0) return;
	pCTRL->ctrl &= (~BYCTRL_DEFOG_STRENGTH_UPDATE);	
	
#if defined(__USE_MDIN_i500__)				// MDIN-i500
	MISP_RegWrite(MISP_LOCAL_ID, 0x414, (pCTRL->stDEFOGDWDR.defog_threshold<<8)|(pCTRL->stDEFOGDWDR.defog_intensity));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x415, (pCTRL->stDEFOGDWDR.defog_skya_ratio<<8)|(pCTRL->stDEFOGDWDR.defog_skya_offset));	//	
//	MISP_RegField(MISP_LOCAL_ID, 0x415, 0, 7, (pCTRL->stDEFOGDWDR.defog_skya_offset));	//fix by defog bright add
	MISP_RegField(MISP_LOCAL_ID, 0x417, 8, 7, (pCTRL->stDEFOGDWDR.defog_weight));	//	
		
#else 										// MDIN-i51X (i510, i540, i550)
		
	//UARTprintf("BYCTRL_DefogStrengthHandler\r\n");
	BYCTRL_set_defog_strength_step(pCTRL, (int)pCTRL->stDEFOGDWDR.defog_strength, (int)pCTRL->stDEFOGDWDR.dwdr_saturation_cal, 1);
		
#endif
	
}

//--------------------------------------------------------------------------------------------------------------------------
static void BYCTRL_DefogScreenHandler(PBY_CTRL_PARAM pCTRL)
{
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	if ((pCTRL->ctrl&BYCTRL_DEFOG_SCREEN_UPDATE)==0) return;
	pCTRL->ctrl &= (~BYCTRL_DEFOG_SCREEN_UPDATE);

	switch(pCTRL->defog_screen_ctrl)	{
		case  MISP_SCREEN_OFF   : MISP_RegField(MISP_LOCAL_ID, 0x401, 14, 2, 0);		break;	 //OFF
		case  MISP_SCREEN_L_ON   : MISP_RegField(MISP_LOCAL_ID, 0x401, 14, 2, 2);		break;	//L-On
		case  MISP_SCREEN_R_ON   : MISP_RegField(MISP_LOCAL_ID, 0x401, 14, 2, 1);		break;	//R-On
		default	: MISP_RegField(MISP_LOCAL_ID, 0x401, 14, 2, 0);		break;
	}
#endif
}

static ROMDATA BY_DWDR_STRENGTH_STEP stBY_DWDR_STRENGTH_STEP[] =
{	// index, xl, yn, yc, yd, xa, xb 

# if 0
	{0, 10, 10, 0, 230, 0, 230},
	
    {0, 15, 15, 0, 230, 0, 230},
    {0, 20, 20, 0, 230, 0, 230}, 
    {0, 25, 25, 0, 230, 0, 230}, 
    {0, 30, 30, 0, 230, 0, 230}, 
    {0, 35, 35, 0, 230, 0, 230}, 
    {0, 45, 45, 0, 230, 0, 230}, 
	
    {0, 65, 65, 0, 230, 0, 230}, 
    {0, 85, 85, 0, 230, 0, 230}, 

    {0, 115, 115, 2, 230, 2, 230}, 
    {0, 130, 130, 4, 230, 2, 230}, 
    {0, 165, 165, 6, 230, 2, 230}, 
    {0, 180, 180, 8, 230, 2, 230}, 
    {0, 195, 195, 12, 230, 2, 230}, 
    {0, 200, 200, 16, 230, 2, 230}, 
    {0, 205, 208, 24, 230, 2, 230},
    {0, 210, 214, 32, 230, 2, 230}
#else
	{0, 10, 10, 0, 230, 0, 230},		
	{0, 13, 13, 0, 230, 0, 230},		
    {0, 15, 15, 0, 230, 0, 230},
    {0, 18, 18, 0, 230, 0, 230},
    {0, 20, 20, 0, 230, 0, 230}, 

    {0, 23, 23, 0, 230, 0, 230}, 
    {0, 25, 25, 0, 230, 0, 230}, 
    {0, 28, 28, 0, 230, 0, 230}, 
    {0, 32, 32, 0, 230, 0, 230},
    {0, 36, 36, 0, 230, 0, 230},
	
    {0, 39, 39, 0, 230, 0, 230}, 
    {0, 45, 45, 0, 230, 0, 230}, 
    {0, 51, 51, 0, 230, 0, 230}, 
    {0, 60, 60, 0, 230, 0, 230}, 
    {0, 68, 68, 0, 230, 0, 230}, 
	
    {0, 70, 70, 0, 230, 0, 230}, 
    {0, 86, 86, 0, 230, 0, 230}, 
    {0, 100, 100, 1, 230, 0, 230}, 
    {0, 114, 114, 2, 230, 2, 230}, 
    {0, 122, 122, 3, 230, 2, 230}, 

    {0, 130, 130, 4, 230, 2, 230}, 
    {0, 140, 140, 5, 230, 2, 230}, 
    {0, 155, 155, 6, 230, 2, 230}, 
    {0, 163, 163, 7, 230, 2, 230}, 
    {0, 175, 175, 8, 230, 2, 230}, 

    {0, 182, 182, 10, 230, 2, 230}, 
    {0, 190, 190, 12, 230, 2, 230}, 
    {0, 194, 194, 14, 230, 2, 230}, 
    {0, 198, 198, 16, 230, 2, 230}, 
    {0, 200, 200, 20, 230, 2, 230}, 

    {0, 203, 204, 24, 230, 2, 230},
    {0, 205, 208, 24, 230, 2, 230},
	
    {0, 205, 208, 24, 230, 2, 230},
    {0, 205, 208, 24, 230, 2, 230}

//    {0, 210, 214, 32, 230, 2, 230}
#endif	
		
};

static ROMDATA BY_DWDR_SATURATION_STEP stBY_DWDR_SATURATION_STEP[] =
{	// index, ovr2
#if 0
    {0, 0},
	{0, 8},
    {0, 16}, 
    {0, 24}, 
    {0, 32}, 
    {0, 40}, 
    {0, 48}, 
    {0, 56}, 
    {0, 64}, 
    {0, 72}, 
    {0, 80}, 
    {0, 88}, 
    {0, 96}, 
    {0, 104}, 
    {0, 112}, 
    {0, 120}, 
    {0, 127}	
#else
    {0, 0},
    {0, 4},
	{0, 8},
	{0, 12},
    {0, 16}, 
	
    {0, 20}, 
    {0, 24}, 
    {0, 28}, 
    {0, 32}, 
    {0, 36}, 

    {0, 40}, 
    {0, 44}, 
    {0, 48}, 
    {0, 52}, 
    {0, 56}, 

    {0, 60}, 
    {0, 64}, 
    {0, 68}, 
    {0, 72}, 
    {0, 76}, 

    {0, 80}, 
    {0, 84}, 
    {0, 88}, 
    {0, 92}, 
    {0, 96}, 

    {0, 100}, 
    {0, 104}, 
    {0, 108}, 
    {0, 112}, 
    {0, 116}, 

    {0, 118}, 
    {0, 120},
	
    {0, 127},
    {0, 127}
#endif	
	
};

static ROMDATA BY_DEFOG_STRENGTH_STEP stBY_DEFOG_STRENGTH_STEP[] = 
{   // center_offset, low_blend, ovr2, avg_pix, hh, hl, lh, ll
#if 0
	{32, 127, 0, 0, 170, 130, 110, 100},
    {32, 120, 0, 0, 170, 130, 110, 100},
    {32, 112, 0, 0, 170, 130, 110, 100}, 
    {32, 104, 0, 0, 170, 130, 110, 100}, 
    {32, 96, 0, 0, 170, 130, 110, 100}, 
    {32, 64, 0, 0, 170, 130, 110, 100}, 
    {32, 48, 0, 0, 170, 130, 110, 100}, 
    {32, 32, 0, 1, 170, 130, 110, 90}, 
    {32, 16, 0, 1, 170, 130, 110, 80}, 
    {32, 0, 16, 1, 170, 130, 110, 70}, 
    {32, 0, 32, 1, 170, 130, 110, 60}, 
    {32, 0, 48, 1, 170, 130, 110, 50}, 
    {32, 0, 64, 1, 170, 130, 110, 40}, 
    {32, 0, 80, 1, 170, 130, 110, 30}, 
    {32, 0, 96, 1, 170, 130, 110, 20}, 
    {32, 0, 112, 1, 170, 130, 110, 10}, 
    {32, 0, 127, 1, 170, 130, 110, 0}
#else
	{32, 127, 0, 0, 170, 130, 110, 100},
	{32, 124, 0, 0, 170, 130, 110, 100},
    {32, 120, 0, 0, 170, 130, 110, 100},
    {32, 116, 0, 0, 170, 130, 110, 100},
    {32, 112, 0, 0, 170, 130, 110, 100}, 

    {32, 108, 0, 0, 170, 130, 110, 100}, 
    {32, 104, 0, 0, 170, 130, 110, 100}, 
    {32, 100, 0, 0, 170, 130, 110, 100}, 
    {32, 96, 0, 0, 170, 130, 110, 100}, 
    {32, 80, 0, 0, 170, 130, 110, 100}, 

    {32, 64, 0, 0, 170, 130, 110, 100}, 
    {32, 52, 0, 0, 170, 130, 110, 100}, 
    {32, 48, 0, 0, 170, 130, 110, 100}, 
    {32, 42, 0, 0, 170, 130, 110, 100}, 
    {32, 32, 0, 1, 170, 130, 110, 95}, 

    {32, 24, 0, 1, 170, 130, 110, 90}, 
    {32, 16, 0, 1, 170, 130, 110, 85}, 
    {32, 8, 0, 1, 170, 130, 110, 80}, 
    {32, 0, 8, 1, 170, 130, 110, 75}, 
    {32, 0, 16, 1, 170, 130, 110, 70}, 

    {32, 0, 24, 1, 170, 130, 110, 65}, 
    {32, 0, 32, 1, 170, 130, 110, 60}, 
    {32, 0, 48, 1, 170, 130, 110, 55}, 
    {32, 0, 50, 1, 170, 130, 110, 50}, 
    {32, 0, 64, 1, 170, 130, 110, 45}, 

    {32, 0, 72, 1, 170, 130, 110, 40}, 
    {32, 0, 80, 1, 170, 130, 110, 35}, 
    {32, 0, 88, 1, 170, 130, 110, 30}, 
    {32, 0, 96, 1, 170, 130, 110, 25}, 
    {32, 0, 100, 1, 170, 130, 110, 20}, 

    {32, 0, 108, 1, 170, 130, 110, 15}, 
    {32, 0, 112, 1, 170, 130, 110, 10}, 

    {32, 0, 120, 1, 170, 130, 110, 0},
    {32, 0, 127, 1, 170, 130, 110, 0}
	
#endif	
};

static ROMDATA BY_DEFOG_SATURATION_STEP stBY_DEFOG_SATURATION_STEP[] =
{   // index, high_blend
#if 0
    {0, 127},          
    {0, 120},
    {0, 112}, 
    {0, 104}, 
    {0, 96}, 
    {0, 88}, 
    {0, 80}, 
    {0, 72}, 
    {0, 64}, 
    {0, 56}, 
    {0, 48}, 
    {0, 40}, 
    {0, 32}, 
    {0, 24}, 
    {0, 16}, 
    {0, 8}, 
    {0, 0}
#else
    {0, 127},          
    {0, 124},          
    {0, 120},
    {0, 116},
    {0, 112}, 
    {0, 108}, 
    {0, 104}, 
    {0, 100}, 
    {0, 96}, 
    {0, 92}, 

    {0, 88}, 
    {0, 84}, 
    {0, 80}, 
    {0, 76}, 
    {0, 72}, 
    {0, 68}, 
    {0, 64}, 
    {0, 60}, 
    {0, 56}, 
    {0, 52}, 

    {0, 48}, 
    {0, 44}, 
    {0, 40}, 
    {0, 36}, 
    {0, 32}, 
    {0, 28}, 
    {0, 24}, 
    {0, 20}, 
    {0, 16}, 
    {0, 12}, 

    {0, 10}, 
    {0, 8},
	
    {0, 4},
    {0, 0}
#endif	
};

static BY_DWDR_STRENGTH_STEP stBY_DWDR_STRENGTH_STEP_CUR = 
{0,0,0,0,0,0,0};
static BY_DWDR_SATURATION_STEP stBY_DWDR_SATURATION_STEP_CUR = 
{0,0};
static BY_DEFOG_STRENGTH_STEP stBY_DEFOG_STRENGTH_STEP_CUR = 
{0,0,0,0,0,0,0,0};
static BY_DEFOG_SATURATION_STEP stBY_DEFOG_SATURATION_STEP_CUR = 
{0,0};


void BYCTRL_dwdr_force_set(void)
{
	WORD reg;
	MISP_RegRead(MISP_LOCAL_ID,	0x412 , &reg) ;	
	
	if(!(reg&1))
	{
		//MISP_RegField(MISP_LOCAL_ID, 0x412, 2, 2, 0);	
		//UARTprintf("BYCTRL_dwdr_force_set\r\n");
		
		//save freeze status, freeze on, 	
		MISP_RegRead(MISP_LOCAL_ID,	0x040 , &reg) ;	
		MISP_RegField(MISP_LOCAL_ID, 0x040, 1, 1, 1);
		Delay_mSec(50);
		MISP_RegField(MISP_LOCAL_ID, 0x412, 2, 2, 1);
		MISP_RegField(MISP_LOCAL_ID, 0x40f, 8, 2, 0);		
		Delay_mSec(50);
		//restore freeze state
		//MISP_RegField(MISP_LOCAL_ID, 0x040, 1, 1, 0);
		MISP_RegWrite(MISP_LOCAL_ID, 0x040 , reg) ;	
	}
}
 
void BYCTRL_defog_force_set(void)
{
	WORD reg;	
	MISP_RegRead(MISP_LOCAL_ID,	0x412 , &reg) ;	


	if(!(reg&2))
	{
		//save freeze status, freeze on, 	
		MISP_RegRead(MISP_LOCAL_ID,	0x040 , &reg) ;	
		MISP_RegField(MISP_LOCAL_ID, 0x040, 1, 1, 1);
		Delay_mSec(50);
		MISP_RegField(MISP_LOCAL_ID, 0x412, 2, 2, 2);
		MISP_RegField(MISP_LOCAL_ID, 0x40f, 8, 2, 3);			
		Delay_mSec(50);
		//restore freeze state
		MISP_RegWrite(MISP_LOCAL_ID, 0x040 , reg) ;	
	}
}

void BYCTRL_defog_dwdr_all_off(void)
{
	WORD reg;	
	//save freeze status, freeze on, 	
	MISP_RegRead(MISP_LOCAL_ID,	0x040 , &reg) ;	
	MISP_RegField(MISP_LOCAL_ID, 0x040, 1, 1, 1);	
	Delay_mSec(50);
	MISP_RegField(MISP_LOCAL_ID, 0x412, 2, 2, 0);	
	Delay_mSec(50);
	//restore freeze state
	//MISP_RegField(MISP_LOCAL_ID, 0x040, 1, 1, 0);
	MISP_RegWrite(MISP_LOCAL_ID, 0x040 , reg) ;	
}


WORD BYCTRL_set_new_value(WORD a, WORD b)
{
	WORD dif;
	
	if(a==b)
		return a;
	
	else if(a > b){
		dif = a - b;
		if(dif > 5) return a-(dif/5);
		else if(dif > 1) return a-1;
		else return a-1;
	}
	else{
		dif = b - a;
		if(dif > 5) return a+(dif/5);
		else if(dif > 1) return a+1;
		else return a+1;
	}	
}

static void BYCTRL_set_dwdr_strength_step(PBY_CTRL_PARAM pCTRL , int strength, int saturation, int init)
{
	WORD value;
	//WORD dat[3];
	
	//UARTprintf("dwdr_step:%d, %d\r\n", strength, saturation);
	
	if(init){
		stBY_DWDR_STRENGTH_STEP_CUR.xl = stBY_DWDR_STRENGTH_STEP[strength].xl;
		stBY_DWDR_STRENGTH_STEP_CUR.yn = stBY_DWDR_STRENGTH_STEP[strength].yn;
		stBY_DWDR_STRENGTH_STEP_CUR.yc = stBY_DWDR_STRENGTH_STEP[strength].yc;
		stBY_DWDR_STRENGTH_STEP_CUR.yd = stBY_DWDR_STRENGTH_STEP[strength].yd;
		stBY_DWDR_STRENGTH_STEP_CUR.xa = stBY_DWDR_STRENGTH_STEP[strength].xa;
		stBY_DWDR_STRENGTH_STEP_CUR.xb = stBY_DWDR_STRENGTH_STEP[strength].xb;
		stBY_DWDR_SATURATION_STEP_CUR.ovr2 = stBY_DWDR_SATURATION_STEP[saturation].ovr2;
	} else{
		stBY_DWDR_STRENGTH_STEP_CUR.xl = 
				BYCTRL_set_new_value(stBY_DWDR_STRENGTH_STEP_CUR.xl, stBY_DWDR_STRENGTH_STEP[strength].xl);
		stBY_DWDR_STRENGTH_STEP_CUR.yn = 
				BYCTRL_set_new_value(stBY_DWDR_STRENGTH_STEP_CUR.yn, stBY_DWDR_STRENGTH_STEP[strength].yn);
		stBY_DWDR_STRENGTH_STEP_CUR.yc = 
				BYCTRL_set_new_value(stBY_DWDR_STRENGTH_STEP_CUR.yc, stBY_DWDR_STRENGTH_STEP[strength].yc);
		stBY_DWDR_STRENGTH_STEP_CUR.yd = 
				BYCTRL_set_new_value(stBY_DWDR_STRENGTH_STEP_CUR.yd, stBY_DWDR_STRENGTH_STEP[strength].yd);
		stBY_DWDR_STRENGTH_STEP_CUR.xa = 
				BYCTRL_set_new_value(stBY_DWDR_STRENGTH_STEP_CUR.xa, stBY_DWDR_STRENGTH_STEP[strength].xa);
		stBY_DWDR_STRENGTH_STEP_CUR.xb = 
				BYCTRL_set_new_value(stBY_DWDR_STRENGTH_STEP_CUR.xb, stBY_DWDR_STRENGTH_STEP[strength].xb);
		stBY_DWDR_SATURATION_STEP_CUR.ovr2 = 
				BYCTRL_set_new_value(stBY_DWDR_SATURATION_STEP_CUR.ovr2, stBY_DWDR_SATURATION_STEP[saturation].ovr2);			
	}
	
	

	value = stBY_DWDR_STRENGTH_STEP_CUR.xl*256;
	value += stBY_DWDR_STRENGTH_STEP_CUR.yn;
	//dat[0] = value;
	//UARTprintf(" ");
	Delay_uSec(6000);
	MISP_RegWrite(MISP_LOCAL_ID, 0x41a, value);	

	value = stBY_DWDR_STRENGTH_STEP_CUR.yc*256;
	value += stBY_DWDR_STRENGTH_STEP_CUR.yd;
	//dat[1] = value;
	MISP_RegWrite(MISP_LOCAL_ID, 0x41b, value);	


	value = stBY_DWDR_STRENGTH_STEP_CUR.xa*256;
	value += stBY_DWDR_STRENGTH_STEP_CUR.xb;
	//dat[2] = value;
	MISP_RegWrite(MISP_LOCAL_ID, 0x41c, value);	
	
	
	/*dat[0] = stBY_DWDR_STRENGTH_STEP_CUR.xl;
	dat[1] = stBY_DWDR_STRENGTH_STEP_CUR.yn;
	dat[2] = stBY_DWDR_STRENGTH_STEP_CUR.yc;
	dat[3] = stBY_DWDR_STRENGTH_STEP_CUR.yd;
	dat[4] = stBY_DWDR_STRENGTH_STEP_CUR.xa;
	dat[5] = stBY_DWDR_STRENGTH_STEP_CUR.xb;
	MISP_MultiWrite(MISP_LOCAL_ID, 0x41a, (PBYTE)dat, 6);*/
	
	
	if(pCTRL->stDEFOGDWDR.set_mode == SET_DWDR_AUTO)
	{
		value = stBY_DWDR_SATURATION_STEP_CUR.ovr2;
		MISP_RegField(MISP_LOCAL_ID, 0x419, 8,7,value);		
	}
	
	
	//MISP_RegField(MISP_LOCAL_ID, 0x40f, 8, 2, 0);		
	
}

static void BYCTRL_set_defog_strength_step(PBY_CTRL_PARAM pCTRL , int strength, int saturation, int init)
{
	WORD value;
	//UARTprintf("defog_step:%d, %d\r\n", strength, saturation);
	
	if(init){
		stBY_DEFOG_STRENGTH_STEP_CUR.center_offset = stBY_DEFOG_STRENGTH_STEP[strength].center_offset;
		stBY_DEFOG_STRENGTH_STEP_CUR.hh = stBY_DEFOG_STRENGTH_STEP[strength].hh;
		stBY_DEFOG_STRENGTH_STEP_CUR.hl = stBY_DEFOG_STRENGTH_STEP[strength].hl;
		stBY_DEFOG_STRENGTH_STEP_CUR.lh = stBY_DEFOG_STRENGTH_STEP[strength].lh;
		stBY_DEFOG_STRENGTH_STEP_CUR.ll = stBY_DEFOG_STRENGTH_STEP[strength].ll;
		stBY_DEFOG_STRENGTH_STEP_CUR.avg_pix = stBY_DEFOG_STRENGTH_STEP[strength].avg_pix;
		stBY_DEFOG_STRENGTH_STEP_CUR.ovr2 = stBY_DEFOG_STRENGTH_STEP[strength].ovr2;
		stBY_DEFOG_STRENGTH_STEP_CUR.low_blend = stBY_DEFOG_STRENGTH_STEP[strength].low_blend;
		stBY_DEFOG_SATURATION_STEP_CUR.high_blend = stBY_DEFOG_SATURATION_STEP[saturation].high_blend;
	} else{
		stBY_DEFOG_STRENGTH_STEP_CUR.center_offset = 
				BYCTRL_set_new_value(stBY_DEFOG_STRENGTH_STEP_CUR.center_offset,stBY_DEFOG_STRENGTH_STEP[strength].center_offset);
		stBY_DEFOG_STRENGTH_STEP_CUR.hh = 
				BYCTRL_set_new_value(stBY_DEFOG_STRENGTH_STEP_CUR.hh,stBY_DEFOG_STRENGTH_STEP[strength].hh);
		stBY_DEFOG_STRENGTH_STEP_CUR.hl = 
				BYCTRL_set_new_value(stBY_DEFOG_STRENGTH_STEP_CUR.hl,stBY_DEFOG_STRENGTH_STEP[strength].hl);
		stBY_DEFOG_STRENGTH_STEP_CUR.lh = 
				BYCTRL_set_new_value(stBY_DEFOG_STRENGTH_STEP_CUR.lh,stBY_DEFOG_STRENGTH_STEP[strength].lh);
		stBY_DEFOG_STRENGTH_STEP_CUR.ll = 
				BYCTRL_set_new_value(stBY_DEFOG_STRENGTH_STEP_CUR.ll,stBY_DEFOG_STRENGTH_STEP[strength].ll);
		stBY_DEFOG_STRENGTH_STEP_CUR.avg_pix = 
				BYCTRL_set_new_value(stBY_DEFOG_STRENGTH_STEP_CUR.avg_pix,stBY_DEFOG_STRENGTH_STEP[strength].avg_pix);
		stBY_DEFOG_STRENGTH_STEP_CUR.ovr2 = 
				BYCTRL_set_new_value(stBY_DEFOG_STRENGTH_STEP_CUR.ovr2,stBY_DEFOG_STRENGTH_STEP[strength].ovr2);
		stBY_DEFOG_STRENGTH_STEP_CUR.low_blend  = 
                BYCTRL_set_new_value(stBY_DEFOG_STRENGTH_STEP_CUR.low_blend,stBY_DEFOG_STRENGTH_STEP[strength].low_blend);
		stBY_DEFOG_SATURATION_STEP_CUR.high_blend =
				BYCTRL_set_new_value(stBY_DEFOG_SATURATION_STEP_CUR.high_blend,stBY_DEFOG_SATURATION_STEP[saturation].high_blend);
	}
	
	//value = stBY_DEFOG_STRENGTH_STEP_CUR.center_offset*1024;
	
	//set xl, yn auto
	//value += 2*256;
	//value += 1*256;
		
	//MISP_RegWrite(MISP_LOCAL_ID, 0x40f, value);	
	
	value = stBY_DEFOG_STRENGTH_STEP_CUR.hh*256;
	value += stBY_DEFOG_STRENGTH_STEP_CUR.hl;
	MISP_RegWrite(MISP_LOCAL_ID, 0x416, value);	
	
	value = stBY_DEFOG_STRENGTH_STEP_CUR.lh*256;
	value += stBY_DEFOG_STRENGTH_STEP_CUR.ll;
	MISP_RegWrite(MISP_LOCAL_ID, 0x417, value);	
	
	value = stBY_DEFOG_STRENGTH_STEP_CUR.avg_pix;
	MISP_RegField(MISP_LOCAL_ID, 0x418, 3,1,value);	
	
	value = stBY_DEFOG_STRENGTH_STEP_CUR.ovr2;
	MISP_RegField(MISP_LOCAL_ID, 0x419, 8,7,value);		
	
	value = stBY_DEFOG_SATURATION_STEP_CUR.high_blend*256;
	value += stBY_DEFOG_STRENGTH_STEP_CUR.low_blend;
	MISP_RegWrite(MISP_LOCAL_ID, 0x41d, value);		

	//UARTprintf("defog_step:int:%d, sat:%d, ini:%d\r\n", strength, saturation, init);
	//MISP_RegField(MISP_LOCAL_ID, 0x40f, 8, 2, 1);
	
	//value = stBY_DEFOG_STRENGTH_STEP_CUR.center_offset*1024;
	
	//set xl, yn auto
	//value += 2*256;
	//value += 1*256;
		
	//MISP_RegWrite(MISP_LOCAL_ID, 0x40f, value);		
}
	



//--------------------------------------------------------------------------------------------------------------------------
//static void BYCTRL_UpdateDefogDWDRActive(PBY_CTRL_PARAM pCTRL)
static void BYCTRL_DefogDWDRHandler(PBY_CTRL_PARAM pCTRL)
{
	WORD regVal ;
	int i;

	int total=0, dwdr_strength_down, dwdr_histo_per;
	float under=0.0f, over=0.0f, sat_=0.0f;
	float alpha = 0.3;
	
	int low_max_index=0;

	static int index = 0;
	static int histo_read_cnt = 0;
	//static int defog_cnt = 0, defog_wait_cnt = 0;
	//static int dwdr_cnt = 0, dwdr_wait_cnt = 0;
	static WORD regs[8];
	//BYTE hmin, hcenter_offset, hcenter;
	static float hmin_avg=0.0;
	int hmin, hcenter_offset, hcenter;

	//BYTE dwdr_strength_cal, dwdr_saturation_cal, defog_strength_cal, defog_saturation_cal;
	int dwdr_strength_cal, dwdr_saturation_cal, defog_strength_cal, defog_saturation_cal;
	BYTE old_mode, temp_mode1, temp_mode2;
	
	int  fog, low_max = 0;

	if((pCTRL->stDEFOGDWDR.set_mode == SET_DWDR_DEFOG_ALL_OFF)&&(pCTRL->stDEFOGDWDR.op_mode > OP_DWDR_DEFOG_ALL_OFF)){
		pCTRL->stDEFOGDWDR.op_mode = OP_DWDR_DEFOG_ALL_OFF;
		BYCTRL_defog_dwdr_all_off();
	}	

	/* histogram read repeats every 16 main loops */
	/* write index at 0 and read data at 3        */
	//if(((histo_read_cnt == 0)||(histo_read_cnt == 4)||(histo_read_cnt == 8)||(histo_read_cnt == 12))
	//	&&(pCTRL->stDEFOGDWDR.set_mode > SET_DWDR_DEFOG_ALL_OFF)){
	if((histo_read_cnt == 0)||(histo_read_cnt == 4)||(histo_read_cnt == 8)||(histo_read_cnt == 12))
	{
		index = histo_read_cnt/4; 
		MISP_RegField(MISP_LOCAL_ID, 0x41f , 0,4,index);
	}
			
	/* read 1/4 portion of histogram */
	//if(((histo_read_cnt == 3)||(histo_read_cnt == 7)||(histo_read_cnt == 11)||(histo_read_cnt == 14))
	//		&&(pCTRL->stDEFOGDWDR.set_mode > SET_DWDR_DEFOG_ALL_OFF)){
	if((histo_read_cnt == 3)||(histo_read_cnt == 7)||(histo_read_cnt == 11)||(histo_read_cnt == 14))
	{
		MISP_MultiRead(MISP_LOCAL_ID, 0x420, (PBYTE)regs, 16);
		
		ghisto64[index*16+0] = regs[0]/256;
		ghisto64[index*16+1] = regs[0]&0xff;
		ghisto64[index*16+2] = regs[1]/256;
		ghisto64[index*16+3] = regs[1]&0xff;
		ghisto64[index*16+4] = regs[2]/256;
		ghisto64[index*16+5] = regs[2]&0xff;
		ghisto64[index*16+6] = regs[3]/256;
		ghisto64[index*16+7] = regs[3]&0xff;
		ghisto64[index*16+8] = regs[4]/256;
		ghisto64[index*16+9] = regs[4]&0xff;
		ghisto64[index*16+10] = regs[5]/256;
		ghisto64[index*16+11] = regs[5]&0xff;
		ghisto64[index*16+12] = regs[6]/256;
		ghisto64[index*16+13] = regs[6]&0xff;
		ghisto64[index*16+14] = regs[7]/256;
		ghisto64[index*16+15] = regs[7]&0xff;		
	}	
	
	if(histo_read_cnt == 15)
	{
		for(i=0;i<64;i++)
		{
			global_histo64[i] = ghisto64[i]; 
			//tm_printf("%d \r\n", global_histo64[i]);
		}
			
		//tm_printf("Update_g_histo\r\n");
	}			
	/* full histogram  has been read, analyze it */
		
	if((histo_read_cnt == 15)&&(pCTRL->stDEFOGDWDR.set_mode > SET_DWDR_DEFOG_ALL_OFF)){
		
		MISP_MultiRead(MISP_LOCAL_ID, 0x40e, (PBYTE)regs, 2);
		hcenter_offset = regs[1]/0x3ff;
		hcenter = regs[1]&0xff;
				
		for(i=0;i<64;i++)
		{
			total += ghisto64[i];
			global_histo64[i] = ghisto64[i]; 
		}
		//tm_printf("Update ghisto\n");			
	
		// sum of (area x weight)/total
		for(i=0;i<32;i++){
			under += ((256.0f - i*256.0f/32.0f)*ghisto64[i])/total;
			if(ghisto64[i] > low_max)
			{
				low_max = ghisto64[i];
				low_max_index = i;
			}
		}
	
		// sum of (area x weight)/total
		for(i=32;i<64;i++)
			over += (((i-32.0f)*256.0f/32.0f)*ghisto64[i])/total;	
		
		hmin = 0;
		fog = 0;
		int hmin_end = 0;
		
		for(i=0;i<12;i++){
			//if(ghisto64[i] < low_max/16)
			//	hmin += 1;
			
			if(hmin_end == 0){
				if(ghisto64[i] < (low_max/16))
				{
					//UARTprintf("hmin index: %d\r\n", i+1);
					hmin = i+1;
				}
				else{
					hmin_end = 1;
				}
			}
			
			// fog accumulate values under low_max_index
			if(i < low_max_index) 
				fog += low_max - ghisto64[i];
		}
		
		//hmin *= 4;
		//float a = pCTRL->stDEFOGDWDR.hmin * 0.9;
		//float b = hmin * 0.1;
		//float c = a + b;
		
		hmin_avg = hmin_avg * 0.9 + hmin * 0.1;
		//UARTprintf("hmin1: %d, hmin2: %f\r\n", hmin, hmin_avg);
		
		//sat += (ghisto64[60]/4.0f+ghisto64[61]/2.0f+ghisto64[62]*3/4.0f+ghisto64[63])/2.5f;	
		sat_ = (ghisto64[60]/4.0f+ghisto64[61]/2.0f+ghisto64[62]*3/4.0f+ghisto64[63])/2.5f;	
		dwdr_histo_per = total*100/(64*255); 
		
		// calc dwdr_strength_down from dwdr_histo_per
		if(dwdr_histo_per > 14)
			//dwdr_strength_down = ((dwdr_histo_per-14)*30)/100;
			dwdr_strength_down = ((dwdr_histo_per-14)*30)/51;
		else
			dwdr_strength_down = 0;
	
		//dwdr_strength_cal = (int)((under/140.0f)*16.0f);
		//if(dwdr_strength_cal > 16)
		//	dwdr_strength_cal = 16;
		dwdr_strength_cal = (int)((under/140.0f)*32.1f);
		if(dwdr_strength_cal > 33)
			dwdr_strength_cal = 33;
		
		// down dwdr strength refering to the histogram shape 
		if(dwdr_strength_cal > dwdr_strength_down)
			dwdr_strength_cal -= dwdr_strength_down;
		else
			dwdr_strength_cal = 0;

		//dwdr_saturation_cal = (int)(sat_/200.0f*16.0f);
		//defog_strength_cal = (int)(fog/(128*15/10));
		dwdr_saturation_cal = (int)(sat_/200.0f*8.1f);
		defog_strength_cal = (int)(fog/(128*15/21));
		
		// apply moving average
		pCTRL->stDEFOGDWDR.mavg_dwdr_strength_cal = 
			(BYTE)((1.0 - alpha) * pCTRL->stDEFOGDWDR.mavg_dwdr_strength_cal + alpha* dwdr_strength_cal);
		pCTRL->stDEFOGDWDR.mavg_defog_strength_cal = 
			(BYTE)((1.0 - alpha) * pCTRL->stDEFOGDWDR.mavg_defog_strength_cal + alpha* defog_strength_cal);
		pCTRL->stDEFOGDWDR.mavg_dwdr_saturation_cal = 
			(BYTE)((1.0 - alpha) * pCTRL->stDEFOGDWDR.mavg_dwdr_saturation_cal + alpha* dwdr_saturation_cal);
		
		dwdr_strength_cal = pCTRL->stDEFOGDWDR.mavg_dwdr_strength_cal;
		defog_strength_cal = pCTRL->stDEFOGDWDR.mavg_defog_strength_cal;
		dwdr_saturation_cal = pCTRL->stDEFOGDWDR.mavg_dwdr_saturation_cal;

		defog_saturation_cal = dwdr_saturation_cal;

		
		//UARTprintf("auto_level: %d, defog_strength_cal: %d\r\n", pCTRL->stDEFOGDWDR.defog_auto_level, defog_strength_cal);
		
		if(pCTRL->stDEFOGDWDR.defog_auto_level==2){
			defog_strength_cal -= 12;			
			defog_saturation_cal -= 12;
			
		}
		else if(pCTRL->stDEFOGDWDR.defog_auto_level==1){
			defog_strength_cal -= 6;						
			defog_saturation_cal -= 6;
		}
		
		if(pCTRL->stDEFOGDWDR.dwdr_auto_level==2){
			dwdr_strength_cal -= 12;			
			dwdr_saturation_cal -= 12;
		}
		else if(pCTRL->stDEFOGDWDR.dwdr_auto_level==1){
			dwdr_strength_cal -= 6;			
			dwdr_saturation_cal -= 6;
		}
	
		if(defog_strength_cal < 0)
			defog_strength_cal = 0;
		if(defog_saturation_cal < 0)
			defog_saturation_cal = 0;
		if(dwdr_strength_cal < 0)
			dwdr_strength_cal = 0;
		if(dwdr_saturation_cal < 0)
			dwdr_saturation_cal = 0;
		
		if(pCTRL->stDEFOGDWDR.dwdr_saturation_sync)
			dwdr_saturation_cal = dwdr_strength_cal;
		
		pCTRL->stDEFOGDWDR.dwdr_strength_cal = (BYTE)dwdr_strength_cal;
		pCTRL->stDEFOGDWDR.defog_strength_cal = (BYTE)defog_strength_cal;
		pCTRL->stDEFOGDWDR.dwdr_saturation_cal = (BYTE)dwdr_saturation_cal;
		pCTRL->stDEFOGDWDR.defog_saturation_cal = (BYTE)defog_saturation_cal;

        //tm_printf("dwdr_strength:%d, saturation:%d, defog:%d, hmin:%f\r\n", 
		//	dwdr_strength_cal, dwdr_saturation_cal, defog_strength_cal, hmin_avg);
		
	    old_mode = pCTRL->stDEFOGDWDR.op_mode;

        if(pCTRL->stDEFOGDWDR.set_mode == SET_DWDR_FORCE){
            pCTRL->stDEFOGDWDR.op_mode = OP_DWDR_MODE;
		}
        else if(pCTRL->stDEFOGDWDR.set_mode == SET_DEFOG_FORCE){
            pCTRL->stDEFOGDWDR.op_mode = OP_DEFOG_MODE;
		}
        else if(pCTRL->stDEFOGDWDR.set_mode == SET_DWDR_AUTO){
            if(under > GDWDR_THRESHOLD)
                pCTRL->stDEFOGDWDR.op_mode = OP_DWDR_MODE;
            else
                pCTRL->stDEFOGDWDR.op_mode = OP_DWDR_DEFOG_ALL_OFF;
		}
        else if(pCTRL->stDEFOGDWDR.set_mode == SET_DEFOG_AUTO){
            //if(hmin > (pCTRL->stDEFOGDWDR.defog_threshold+2)*4)
            //if(hmin > (pCTRL->stDEFOGDWDR.defog_threshold+2))
            if(hmin_avg > (pCTRL->stDEFOGDWDR.defog_threshold+2))
                pCTRL->stDEFOGDWDR.op_mode = OP_DEFOG_MODE;
            else
                pCTRL->stDEFOGDWDR.op_mode = OP_DWDR_DEFOG_ALL_OFF;
		}
        else if(pCTRL->stDEFOGDWDR.set_mode == SET_DWDR_DEFOG_FULL_AUTO){
				
			//if(hmin > (pCTRL->stDEFOGDWDR.defog_threshold+2)*4) 
			//if(hmin > (pCTRL->stDEFOGDWDR.defog_threshold+2)) 
			if(hmin_avg > (pCTRL->stDEFOGDWDR.defog_threshold+2)) 
				temp_mode1 = OP_DEFOG_MODE;
			else
				temp_mode1 = OP_DWDR_DEFOG_ALL_OFF;
			
			if(under > GDWDR_THRESHOLD)
				temp_mode2 = OP_DWDR_MODE;
			else
				temp_mode2 = OP_DWDR_DEFOG_ALL_OFF;

			if((temp_mode1 == OP_DEFOG_MODE)&&(temp_mode2 == OP_DWDR_MODE)){
			
				//int dif  = abs(dwdr_strength_cal - defog_strength_cal);
				
				if(hmin_avg >= pCTRL->stDEFOGDWDR.defog_threshold+2+2)
					pCTRL->stDEFOGDWDR.op_mode = OP_DEFOG_MODE;
				else{
					
					if(dwdr_strength_cal > defog_strength_cal)
						pCTRL->stDEFOGDWDR.op_mode = OP_DWDR_MODE;
					else
						pCTRL->stDEFOGDWDR.op_mode = OP_DEFOG_MODE;
				}				
				
			}
			
			/*if((temp_mode1 == OP_DEFOG_MODE)&&(temp_mode2 == OP_DWDR_MODE)){
				if((defog_strength_cal+4 > dwdr_strength_cal)||(dwdr_strength_cal < 10)){
					if((dwdr_cnt>0)&&(defog_cnt==0)){
						defog_wait_cnt++;
					}
					if(defog_wait_cnt > 32)	{
						pCTRL->stDEFOGDWDR.op_mode = OP_DEFOG_MODE;	
						defog_wait_cnt = 0;
					}
					dwdr_wait_cnt = 0;
				}else{
					if((dwdr_cnt==0)&&(defog_cnt>0)){
						dwdr_wait_cnt++;
					}
					if(dwdr_wait_cnt > 32)	{
						pCTRL->stDEFOGDWDR.op_mode = OP_DWDR_MODE;			
						dwdr_wait_cnt = 0;
					}			
					defog_wait_cnt  = 0;
				}				
			}*/
			else if(temp_mode1 == OP_DEFOG_MODE){
				pCTRL->stDEFOGDWDR.op_mode = OP_DEFOG_MODE;
				//defog_wait_cnt = 0;				
			}
			else if(temp_mode2 == OP_DWDR_MODE){
				pCTRL->stDEFOGDWDR.op_mode = OP_DWDR_MODE;
				//dwdr_wait_cnt = 0;			
			}
			else{
				pCTRL->stDEFOGDWDR.op_mode = OP_DWDR_DEFOG_ALL_OFF;
				//defog_wait_cnt = 0;				
				//dwdr_wait_cnt = 0;			
			}	
			
		}
        else{
            pCTRL->stDEFOGDWDR.op_mode = OP_DWDR_DEFOG_ALL_OFF;
		}

        if(old_mode != pCTRL->stDEFOGDWDR.op_mode){
				//UARTprintf("op_mode:%d\r\n",pCTRL->stDEFOGDWDR.op_mode);
            if(pCTRL->stDEFOGDWDR.op_mode == OP_DWDR_MODE)
                BYCTRL_dwdr_force_set();
            else if(pCTRL->stDEFOGDWDR.op_mode == OP_DEFOG_MODE)
                BYCTRL_defog_force_set();
            else
                BYCTRL_defog_dwdr_all_off();
		}

        if((pCTRL->stDEFOGDWDR.op_mode == OP_DWDR_MODE)&&(pCTRL->stDEFOGDWDR.set_mode != SET_DWDR_FORCE))
		{
            //UARTprintf("mode1:DWDRIntensity:%d, DWDRSaturaion:%d\r\n", dwdr_strength_cal, dwdr_saturation_cal);
			if(old_mode != pCTRL->stDEFOGDWDR.op_mode)
				BYCTRL_set_dwdr_strength_step(pCTRL, dwdr_strength_cal, dwdr_saturation_cal, 1); // init
				//BYCTRL_set_dwdr_strength_step(pCTRL, dwdr_strength_cal, dwdr_saturation_cal, 0); // init
			else
				BYCTRL_set_dwdr_strength_step(pCTRL, dwdr_strength_cal, dwdr_saturation_cal, 0);
		}
        else if((pCTRL->stDEFOGDWDR.op_mode == OP_DWDR_MODE)&&(pCTRL->stDEFOGDWDR.set_mode == SET_DWDR_FORCE))
		{
			BYCTRL_set_dwdr_strength_step(pCTRL, pCTRL->stDEFOGDWDR.dwdr_strength, pCTRL->stDEFOGDWDR.dwdr_saturation, 0);
		}
        else if((pCTRL->stDEFOGDWDR.op_mode == OP_DEFOG_MODE)&&(pCTRL->stDEFOGDWDR.set_mode != SET_DEFOG_FORCE))
		{		
            //UARTprintf("mode2:DefogIntensity:%d, DEFOGSaturaion:%d\r\n", defog_strength_cal, dwdr_saturation_cal);
			if(old_mode != pCTRL->stDEFOGDWDR.op_mode)
				//BYCTRL_set_defog_strength_step(pCTRL, defog_strength_cal, dwdr_saturation_cal, 1); // init
				BYCTRL_set_defog_strength_step(pCTRL, defog_strength_cal, defog_saturation_cal, 1); // init
			else
				//BYCTRL_set_defog_strength_step(pCTRL, defog_strength_cal, dwdr_saturation_cal, 0);
				BYCTRL_set_defog_strength_step(pCTRL, defog_strength_cal, defog_saturation_cal, 0);
		}		

		if(pCTRL->stDEFOGDWDR.op_mode == OP_DEFOG_MODE){
			//defog_cnt++;
			//dwdr_cnt = 0;
		}
		else if(pCTRL->stDEFOGDWDR.op_mode == OP_DWDR_MODE){
			//defog_cnt = 0;
			//dwdr_cnt++;
			
			
		}
		else{
			//defog_cnt = 0;
			//dwdr_cnt = 0;
		}		
		//UARTprintf("tmode1:%d, tmode2:%d, dfgthld:%d\r\n", temp_mode1, temp_mode2, (pCTRL->stDEFOGDWDR.defog_threshold+2)*4);		
		//UARTprintf("defog_wait_cnt:%d, dwdr_wait_cnt: %d, defog_cnt:%d, dwdr_cnt: %d\r\n", defog_wait_cnt, dwdr_wait_cnt, defog_cnt, dwdr_cnt);		
	}

	
	histo_read_cnt++;
	
	if(histo_read_cnt >=40)
		histo_read_cnt = 0;	
	
	
	if ( pCTRL->stDEFOGDWDR.defog_mode == MISP_DEFOG_OFF ) 
	{
		pCTRL->stDEFOGDWDR.defog_active = 0 ;	
		pCTRL->stDEFOGDWDR.dwdr_active = 0 ;		
		return ;
	}
	
	MISP_RegRead(MISP_LOCAL_ID,	0x412 , &regVal) ;
	
	pCTRL->stDEFOGDWDR.defog_active = (regVal>>1)&0x0001 ;	
	pCTRL->stDEFOGDWDR.dwdr_active = (regVal&0x0001) ;	
}

//---------------------------------------------------------------------------------------------------------------------------
static void BYCTRL_GammaTableLoad(void)
{
#if defined(__USE_MDIN_i500__) 					// MDIN-i500
	#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)		// check eeprom size

		if(EEPROM_RegRead(EEPROM_GAMMA_FLAG) != EEPROM_GAMMA_DONE)	{
		//		EEPROM_MultiWrite(EEPROM_GAMMA_TABLE, (PBYTE)&def_gamma_value_table, sizeof(def_gamma_value_table));	
			EEPROM_MultiWrite(EEPROM_GAMMA_TABLE, (PBYTE)&def_gamma_value_table[15], sizeof(def_gamma_value_table[15]));	
			EEPROM_RegWrite(EEPROM_GAMMA_FLAG,  EEPROM_GAMMA_DONE);
		}

		if((stBYCTRL.stGAMMA.gamma_r==MISP_GAMMA_CUSTOM1)||(stBYCTRL.stGAMMA.gamma_r==MISP_GAMMA_CUSTOM2)||(stBYCTRL.stGAMMA.gamma_r==MISP_GAMMA_CUSTOM3))
		{			// custom
			EEPROM_MultiRead(EEPROM_GAMMA_TABLE, (PBYTE)&gamma_current_value, sizeof(gamma_current_value));
		}
		else	{
			memcpy(&gamma_current_value[0], &def_gamma_value_table[stBYCTRL.stGAMMA.gamma_r][0], sizeof(gamma_current_value));
		}
	#else
		memcpy(&gamma_current_value[0], &def_gamma_value_table[stBYCTRL.stGAMMA.gamma_r][0], sizeof(gamma_current_value));
	#endif
#endif
}

#if defined(__USE_MDIN_i500__) 		// MDIN-i500
//--------------------------------------------------------------------------------------------------------------------------
static void BYCTRL_SetGammaTable(PBY_CTRL_PARAM pCTRL)
{
	PBYTE gamma_table_ptr;

	if(pCTRL->stGAMMA.gamma_r==12) { //to test thermography
//		#if defined(__M33_V101__)
		BYCTRL_GammaTableLoad();
		gamma_table_ptr = (PBYTE)gamma_current_value;
		
		MISP_MultiWrite(MISP_LOCAL_ID, 0x430, gamma_table_ptr, 32);
		MISP_MultiWrite(MISP_LOCAL_ID, 0x440, gamma_table_ptr, 32);
		MISP_MultiWrite(MISP_LOCAL_ID, 0x450, gamma_table_ptr, 32);

//		#else
//		MISP_MultiWrite(MISP_LOCAL_ID, 0x430, (PBYTE)gamma_table_thermo_r, 32);
//		MISP_MultiWrite(MISP_LOCAL_ID, 0x440, (PBYTE)gamma_table_thermo_g, 32);
//		MISP_MultiWrite(MISP_LOCAL_ID, 0x450, (PBYTE)gamma_table_thermo_b, 32);
//		#endif
	} 
	else {
		BYCTRL_GammaTableLoad();
		gamma_table_ptr = (PBYTE)gamma_current_value;
		
		MISP_MultiWrite(MISP_LOCAL_ID, 0x430, gamma_table_ptr, 32);
		MISP_MultiWrite(MISP_LOCAL_ID, 0x440, gamma_table_ptr, 32);
		MISP_MultiWrite(MISP_LOCAL_ID, 0x450, gamma_table_ptr, 32);
	}

	MISP_RegWrite(MISP_LOCAL_ID, 0x421, (pCTRL->stGAMMA.gamma_en<<15)|(pCTRL->bayer_pixel_id<<12));	
}
#else								// MDIN-i51X (i510, i540, i550)
//------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------
static void BYCTRL_BYGammaEnHandler(PBY_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&BYCTRL_BYGAMMA_EN_UPDATE)==0) return ;
	pCTRL->ctrl &= (~BYCTRL_BYGAMMA_EN_UPDATE);
		
	MISP_RegField(MISP_LOCAL_ID, __BYGAMMA_CTRL_ADDR__, 0, 1, pCTRL->stGAMMA.gamma_en) ;	
}
// ----------------------------------------------------------------------
static void BYCTRL_BYGammaExtendHandler(PBY_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&BYCTRL_BYGAMMA_EXTEND_UPDATE)==0) return;
	pCTRL->ctrl &= (~BYCTRL_BYGAMMA_EXTEND_UPDATE);
	
	MISP_RegField(MISP_LOCAL_ID, __BYGAMMA_CTRL_ADDR__, 3, 2, pCTRL->stGAMMA.gamma_extend) ;
}
#endif

#if defined(__USE_GAMMA_AE__)
static BYTE gamma256_var_table[256];
extern WORD digitalGainX;
#endif

static void BYCTRL_BYGammaValueHandler(PBY_CTRL_PARAM pCTRL)
{

	PBYTE gamma_table_ptr_x;
#if __USE_GAMMA_BYTEDATA__
	WORD i ;	
	PBYTE gamma_table_ptr_y;
#endif	
#if defined(__USE_GAMMA_AE__)
	FLOAT gammaGain;
	FLOAT tempF;
#endif
	if ((pCTRL->ctrl&BYCTRL_BYGAMMA_LUT_UPDATE)==0) return;
	pCTRL->ctrl &= (~BYCTRL_BYGAMMA_LUT_UPDATE);

	BYCTRL_GammaTableLoad();

#if __USE_GAMMA_BYTEDATA__ // MDIN-i5x0 (i510, i540, i550)
    #if defined(__USE_GAMMA_AE__)
	gamma_table_ptr_x = (PBYTE)gamma_current_value;
	if ((pCTRL->ctrl&BYCTRL_BYGAMMA_GAIN_UPDATE)==0) {
		gamma_table_ptr_y = (PBYTE)gamma256_value_table[pCTRL->stGAMMA.gamma_r];
	} else {
		gamma_table_ptr_y = (PBYTE)gamma256_var_table;
		pCTRL->ctrl &= (~BYCTRL_BYGAMMA_GAIN_UPDATE);

		memcpy(gamma256_var_table,gamma256_value_table[pCTRL->stGAMMA.gamma_r], sizeof(gamma256_var_table));
		gammaGain = (FLOAT)digitalGainX/32.0f + 1.0f;
		//UARTprintf("gain:%f\r\n",gammaGain);
		for ( i = 0 ; i < 256 ; i++) {
			tempF = (FLOAT)gamma256_var_table[i]*gammaGain;
			if(tempF >255.0f)
				gamma256_var_table[i] = 255;
			else
				gamma256_var_table[i] = (BYTE)tempF;
			//UARTprintf("gain:%d = %d:%d\r\n",gammaGain,i,gamma256_var_table[i]);
		}
	}
	MISP_RegWrite(MISP_LOCAL_ID, __BYGAMMA_COMMAND_ADDR__, 0xF000) ; // LUT write enable from LUT addr 0
		
	if(pCTRL->stGAMMA.gamma_extend==1) {
		for ( i = 0 ; i < 256 ; i++ ) {
			if(i<128) {
				if(i%2) 	MISP_RegWrite(MISP_LOCAL_ID, __BYGAMMA_DATA_ADDR__, (gamma_table_ptr_y[i/2] + gamma_table_ptr_y[i/2+1])/2); // LUT write
				else		MISP_RegWrite(MISP_LOCAL_ID, __BYGAMMA_DATA_ADDR__, gamma_table_ptr_y[i/2]); // LUT write
			} else if(i<192) MISP_RegWrite(MISP_LOCAL_ID, __BYGAMMA_DATA_ADDR__, gamma_table_ptr_y[i-64]); // LUT write
			else			 MISP_RegWrite(MISP_LOCAL_ID, __BYGAMMA_DATA_ADDR__, gamma_table_ptr_y[(i-192)*2+128]); // LUT write
		}	
	} else {	
		for ( i = 0 ; i < 256 ; i++ ) {
			MISP_RegWrite(MISP_LOCAL_ID, __BYGAMMA_DATA_ADDR__, gamma_table_ptr_y[i]) ; // LUT write
		}	
	}
	MISP_RegWrite(MISP_LOCAL_ID, __BYGAMMA_COMMAND_ADDR__, 0x0000) ; // LUT write disable
    #else
	gamma_table_ptr_x = (PBYTE)gamma_current_value;
	// UARTprintf("WRITING FRONT GAMMA 256LUT NO. %d \r\n",pCTRL->stGAMMA.gamma_r );	
	gamma_table_ptr_y = (PBYTE)gamma256_value_table[pCTRL->stGAMMA.gamma_r];
	// MISP_RegField(MISP_LOCAL_ID, __BYGAMMA_CTRL_ADDR__, 0, 1, 0) ; // Gamma off
	MISP_RegWrite(MISP_LOCAL_ID, __BYGAMMA_COMMAND_ADDR__, 0xF000) ; // LUT write enable from LUT addr 0
	// UARTprintf("***** [WRITING GAMMA LUT] \n");

 	if(pCTRL->stGAMMA.gamma_extend==1) {
		for ( i = 0 ; i < 256 ; i++ ) {
			if(i<128) {
				if(i%2) 	MISP_RegWrite(MISP_LOCAL_ID, __BYGAMMA_DATA_ADDR__, (gamma_table_ptr_y[i/2] + gamma_table_ptr_y[i/2+1])/2); // LUT write
				else		MISP_RegWrite(MISP_LOCAL_ID, __BYGAMMA_DATA_ADDR__, gamma_table_ptr_y[i/2]); // LUT write
			} else if(i<192) MISP_RegWrite(MISP_LOCAL_ID, __BYGAMMA_DATA_ADDR__, gamma_table_ptr_y[i-64]); // LUT write
			else			 MISP_RegWrite(MISP_LOCAL_ID, __BYGAMMA_DATA_ADDR__, gamma_table_ptr_y[(i-192)*2+128]); // LUT write
		}	
	} else {	
		for ( i = 0 ; i < 256 ; i++ ) {
			// UARTprintf("%d => value : %d \r\n",i , gamma_table_ptr_y[i]);	
			MISP_RegWrite(MISP_LOCAL_ID, __BYGAMMA_DATA_ADDR__, gamma_table_ptr_y[i]) ; // LUT write
		}	
		// UARTprintf("***** [WRITING GAMMA LUT DONE] \n");
	}
	MISP_RegWrite(MISP_LOCAL_ID, __BYGAMMA_COMMAND_ADDR__, 0x0000) ; // LUT write disable
	// MISP_RegField(MISP_LOCAL_ID, __BYGAMMA_CTRL_ADDR__, 11, 2, pCTRL->stGAMMA.gamma_lut);	
	// MISP_RegField(MISP_LOCAL_ID, __BYGAMMA_CTRL_ADDR__, 0, 1, pCTRL->stGAMMA.gamma_en) ;	
	// MISP_RegField(MISP_LOCAL_ID, __BYGAMMA_CTRL_ADDR__, 1, 1, 1) ;	//lut_acc_on_blank = access on blank	
    #endif
#else	//i500

	if(GetWDR_ProcesMode()) gamma_table_ptr_x = (PBYTE)def_gamma_value_table[pCTRL->stGAMMA.gamma_r];
	else gamma_table_ptr_x = (PBYTE)gamma_current_value;
	
	BYCTRL_SetGammaTable(pCTRL);

#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void BYCTRL_ImageSizeHandler(PBY_CTRL_PARAM pCTRL)
{
	WORD in_hsize ;	

	if ((pCTRL->ctrl&BYCTRL_IMAGE_SIZE_UPDATE)==0) return;
	pCTRL->ctrl &= (~BYCTRL_IMAGE_SIZE_UPDATE);
	
	in_hsize = pCTRL->active_h_size + (pCTRL->bH<<1) ;	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x401, (in_hsize>>1));	// 
		
	MISP_RegWrite(MISP_LOCAL_ID, 0x407, (pCTRL->bH));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x408, (pCTRL->bV));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x409, (pCTRL->active_h_size));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x40A, (pCTRL->active_v_size));	//		
}

//--------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------------

static void BYCTRL_DataOrderHandler(PBY_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&BYCTRL_DATA_ORDER_UPDATE)==0) return;
	pCTRL->ctrl &= (~BYCTRL_DATA_ORDER_UPDATE);
	
	MISP_RegField(MISP_LOCAL_ID, 0x400, 13, 2 , pCTRL->bayer_pixel_id);
	MISP_RegField(MISP_LOCAL_ID, 0x411,  0, 2 , pCTRL->bayer_pixel_id);	// 
	MISP_RegField(MISP_LOCAL_ID, 0x421, 12, 2 , pCTRL->bayer_pixel_id);
}



//--------------------------------------------------------------------------------------------------------------------------
void BY_ProcessHandler(void)
{
	BYCTRL_DefogDWDRHandler(&stBYCTRL) ;
	
	if ((stBYCTRL.ctrl&BYCTRL_TRIGGER_ISP)==0) return;
	stBYCTRL.ctrl &= (~BYCTRL_TRIGGER_ISP);

	BYCTRL_InputSelectHandler(&stBYCTRL);
	BYCTRL_DataOrderHandler(&stBYCTRL);
	BYCTRL_ImageSizeHandler(&stBYCTRL);
	BYCTRL_BypassHandler(&stBYCTRL);
	BYCTRL_LensCorrEnHandler(&stBYCTRL);
	BYCTRL_DWDRModeHandler(&stBYCTRL);
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	BYCTRL_DWDRStrengthHandler(&stBYCTRL);
	BYCTRL_DWDRSaturationHandler(&stBYCTRL);
	BYCTRL_DWDRLocalRatioHandler(&stBYCTRL);
#endif
	BYCTRL_DefogModeHandler(&stBYCTRL);
	BYCTRL_DefogStrengthHandler(&stBYCTRL);
	BYCTRL_DefogScreenHandler(&stBYCTRL);
	BYCTRL_BYGammaValueHandler(&stBYCTRL);
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	BYCTRL_BYGammaEnHandler(&stBYCTRL);
	BYCTRL_BYGammaExtendHandler(&stBYCTRL);
#endif
}

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
void SetBY_Input_select(BOOL val)
{
	stBYCTRL.input_select = val;
	stBYCTRL.ctrl |= (BYCTRL_INPUT_SEL_UPDATE | BYCTRL_TRIGGER_ISP);
	
#if __MISP100_DBGPRT__ == 1
	UARTprintf("SetBAYER_Input_select : \r\n", (val==1) ? "From Sensor Top" : "From WDR Top");
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetBY_Bypass(BOOL val)
{
	stBYCTRL.bayer_bypass = val;
	stBYCTRL.ctrl |= (BYCTRL_BYPASS_UPDATE | BYCTRL_TRIGGER_ISP);

#if __MISP100_DBGPRT__ == 1
	UARTprintf("SetBAYER_Bypass : \r\n", (val==1) ? "Bypass ON" : "Bypass OFF");
#endif	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetBY_Lens_corr_en(BOOL val)
{
	stBYCTRL.stLENSCORR.lens_corr_en = val;
	stBYCTRL.ctrl |= (BYCTRL_LENS_CORR_UPDATE | BYCTRL_TRIGGER_ISP);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
{
	BYTE buff[2];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
	EEPROM_MultiWrite(EEPROM_EFFECT_LSC, (PBYTE)&buff[0], sizeof(buff));
}
#endif
}

void SetBY_Lens_corr_value(BYTE inner_rad, BYTE outer_rad, BYTE intensity )
{
	stBYCTRL.stLENSCORR.inner_rad_rel = inner_rad;
	stBYCTRL.stLENSCORR.outer_rad_rel = outer_rad;
	stBYCTRL.stLENSCORR.lens_corr_intensity = intensity;
	stBYCTRL.ctrl |= (BYCTRL_LENS_CORR_UPDATE | BYCTRL_TRIGGER_ISP);
	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetBY_DWDR_mode(BYTE val)
{
	stBYCTRL.stDEFOGDWDR.dwdr_mode = val;
	stBYCTRL.ctrl |= (BYCTRL_DWDR_UPDATE | BYCTRL_TRIGGER_ISP);

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveDWDRMode(val);
#endif
	
#if __MISP100_DBGPRT__ == 1
	UARTprintf("SetBAYER_DWDR : \r\n", (val==MISP_DWDR_AUTO) ? "AUTO" : ((val==MISP_DWDR_ON) ? ("ON") : ("OFF")));
#endif	
}

//--------------------------------------------------------------------------------------------------------------------------
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)
void SetBY_DefogDWDRMode(BYTE val)
{
	stBYCTRL.stDEFOGDWDR.defog_dwdr_mode = val;	
	
	switch(val){
		case 0: // off
			SetBY_DefogMode(0);
			SetBY_DWDR_mode(0);
			break;
		case 1: // auto
			SetBY_DefogMode(2);
			SetBY_DWDR_mode(2);
			break;
		case 2: // defog_on
			SetBY_DefogMode(1);
			SetBY_DWDR_mode(0);
			break;
		case 3: // dwdr_on
			SetBY_DefogMode(0);
			SetBY_DWDR_mode(1);
			break;
			
	}
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
{
	BYTE buff[2];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
	EEPROM_MultiWrite(EEPROM_SPECIAL_DEFOGDWDR, (PBYTE)&buff[0], sizeof(buff));
}
#endif	
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
void SetBY_DWDRStrength(WORD val)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveDWDRStrength(val);
#endif
	
	stBYCTRL.ctrl |= (BYCTRL_DWDR_STRENGTH_UPDATE | BYCTRL_TRIGGER_ISP);
	//stBYCTRL.stDEFOGDWDR.dwdr_strength = val;
	// menu 16 to 32 conversion
	stBYCTRL.stDEFOGDWDR.dwdr_strength = val*2;
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	{
		BYTE buff[2];
	
		buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
		EEPROM_MultiWrite(EEPROM_DWDR_STRENGTH, (PBYTE)&buff[0], sizeof(buff));
	}
#endif	
}

BYTE GetBY_DWDRStrength(void)
{
	//return  stBYCTRL.stDEFOGDWDR.dwdr_strength;
	return  stBYCTRL.stDEFOGDWDR.dwdr_strength/2;
}

void SetBY_DWDRSaturation(WORD val)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveDWDRSaturation(val);
#endif
	
	stBYCTRL.ctrl |= (BYCTRL_DWDR_SAT_UPDATE | BYCTRL_TRIGGER_ISP);
	//stBYCTRL.stDEFOGDWDR.dwdr_saturation = (BYTE)val;
	// menu set 16 to 32
	//UARTprintf("SetBY_DWDRSaturation:val:%d\n", val);
	
	stBYCTRL.stDEFOGDWDR.dwdr_saturation = (BYTE)val*2;
	//UARTprintf("SetBY_DWDRSensitivity: %d\r\n", stBYCTRL.stDEFOGDWDR.dwdr_saturation);
	
	/*val *= 8;
	if(val==128)
		val= 127;
	MISP_RegField(MISP_LOCAL_ID, 0x419, 8, 7 , val);*/
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	{
		BYTE buff[2];
	
		buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)		
		EEPROM_MultiWrite(EEPROM_DEFOGDWDR_SATURATION, (PBYTE)&buff[0], sizeof(buff));
#else		
		EEPROM_MultiWrite(EEPROM_DWDR_SATURATION, (PBYTE)&buff[0], sizeof(buff));
#endif		
	}
#endif	
	
}

BYTE GetBY_DWDRSaturation(void)
{
	//return  stBYCTRL.stDEFOGDWDR.dwdr_saturation;
	return  stBYCTRL.stDEFOGDWDR.dwdr_saturation/2;
}


void SetBY_DWDRLocalRatio(WORD val)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveDWDRLocalRatio(val);
#endif
	
	stBYCTRL.ctrl |= (BYCTRL_DWDR_LOCAL_RATIO_UPDATE | BYCTRL_TRIGGER_ISP);
	stBYCTRL.stDEFOGDWDR.dwdr_local_ratio = (BYTE)val;
	/*val *= 8;
	if(val==128)
		val= 127;
	MISP_RegField(MISP_LOCAL_ID, 0x419, 0, 7 , val);	*/
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	{
		BYTE buff[2];
	
		buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)(val);
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)		
		EEPROM_MultiWrite(EEPROM_DEFOGDWDR_LOCALRATIO, (PBYTE)&buff[0], sizeof(buff));
#else		
		EEPROM_MultiWrite(EEPROM_DWDR_LOCALRATIO, (PBYTE)&buff[0], sizeof(buff));
#endif		
	}
#endif	
	
}
#endif

BYTE GetBY_DWDRLocalRatio(void)
{
	return  stBYCTRL.stDEFOGDWDR.dwdr_local_ratio;
}


//--------------------------------------------------------------------------------------------------------------------------
void SetBY_DefogMode(BYTE val)
{
	// BYTE nr_srm_dy_th ;
	// BYTE nr_srm_th ;
	// BYTE nr_srm_th_frac ;	
	BYTE by_defog_mode ;	
	
	by_defog_mode = stBYCTRL.stDEFOGDWDR.defog_mode ;
	
	stBYCTRL.stDEFOGDWDR.defog_mode = val;
	stBYCTRL.ctrl |= (BYCTRL_DEFOG_MODE_UPDATE | BYCTRL_TRIGGER_ISP);
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveDefogMode(val);
#endif
	
	// GetNR_LowLightSRM(&nr_srm_dy_th,&nr_srm_th,&nr_srm_th_frac) ;
	
	
	// if ( val != MISP_DEFOG_OFF )  // Defog ON
	// {
		// if ( by_defog_mode == MISP_DEFOG_OFF ) 
		// {
			// SetNR_LowLightSRM(nr_srm_dy_th,(nr_srm_th+3),nr_srm_th_frac);			

			// SetNR_LowLightEnterLevel((5<<3|2));
			// SetNR_LowLightExitLevel((5<<3|1));
		// }
	// }
	// else // Defog OFF
	// {
		// SetNR_LowLightSRM(nr_srm_dy_th,(nr_srm_th-3),nr_srm_th_frac);
		
// #if defined(__USE_CMOS_IMX136__)
		// SetNR_LowLightEnterLevel((7<<3|7));
		// SetNR_LowLightExitLevel((7<<3|6));
// #else
		// SetNR_LowLightEnterLevel((9<<3|7));
		// SetNR_LowLightExitLevel((9<<3|6));		
// #endif		
	// }
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
{
	BYTE buff[2];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)	
	EEPROM_MultiWrite(EEPROM_SPECIAL_DEFOGDWDR, (PBYTE)&buff[0], sizeof(buff));
#else
	EEPROM_MultiWrite(EEPROM_SPECIAL_DEFOG, (PBYTE)&buff[0], sizeof(buff));
#endif	
	
	
}
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetBY_DefogStrength(WORD val)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveDefogStrength(val);
#endif
	
	stBYCTRL.ctrl |= (BYCTRL_DEFOG_STRENGTH_UPDATE | BYCTRL_TRIGGER_ISP);
	
	//stBYCTRL.stDEFOGDWDR.defog_strength = val;
	// conv menu 16 to 32 step
	stBYCTRL.stDEFOGDWDR.defog_strength = val*2;
	//UARTprintf("SetBY_DefogSensitive: %d\r\n", val);
	
	
#if defined(__USE_MDIN_i500__)								// MDIN-i500
	switch(val)	{
		case  MISP_DEFOG_STRN1:
			stBYCTRL.stDEFOGDWDR.defog_threshold = 60 ;
			stBYCTRL.stDEFOGDWDR.defog_intensity = 60 ;
			stBYCTRL.stDEFOGDWDR.defog_skya_offset = 70 ;
			stBYCTRL.stDEFOGDWDR.defog_weight = 127 ;

			break;	 
		case  MISP_DEFOG_STRN2:
			stBYCTRL.stDEFOGDWDR.defog_threshold = 60 ;
			stBYCTRL.stDEFOGDWDR.defog_intensity = 80;
			stBYCTRL.stDEFOGDWDR.defog_skya_offset = 50 ;
			stBYCTRL.stDEFOGDWDR.defog_weight = 127 ;

			break;	
		case  MISP_DEFOG_STRN3:
			stBYCTRL.stDEFOGDWDR.defog_threshold = 60 ;
			stBYCTRL.stDEFOGDWDR.defog_intensity = 90;
			stBYCTRL.stDEFOGDWDR.defog_skya_offset = 45 ;
			stBYCTRL.stDEFOGDWDR.defog_weight = 127 ;
			break;	
		case  MISP_DEFOG_STRN4:
			stBYCTRL.stDEFOGDWDR.defog_threshold = 60 ;
			stBYCTRL.stDEFOGDWDR.defog_intensity = 100;
			stBYCTRL.stDEFOGDWDR.defog_skya_offset = 40 ;
			stBYCTRL.stDEFOGDWDR.defog_weight = 127 ;
			break;	
		case  MISP_DEFOG_STRN5:
			stBYCTRL.stDEFOGDWDR.defog_threshold = 60 ;
			stBYCTRL.stDEFOGDWDR.defog_intensity = 118;
			stBYCTRL.stDEFOGDWDR.defog_skya_offset = 30 ;
			stBYCTRL.stDEFOGDWDR.defog_weight = 127 ;
			break;	
	}			
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
{
	BYTE buff[2];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)
	EEPROM_MultiWrite(EEPROM_DEFOGDWDR_STRENGTH, (PBYTE)&buff[0], sizeof(buff));
#else
	EEPROM_MultiWrite(EEPROM_DEFOG_STRENGTH, (PBYTE)&buff[0], sizeof(buff));
#endif	
	
}
#endif
}

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)	// i510, i540, i550
void SetBY_DefogThreshold(WORD val)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveDefogThreshold(val);
#endif
	
	// defog threshold has no register control
	stBYCTRL.ctrl |= (BYCTRL_DEFOG_THRESHOLD_UPDATE | BYCTRL_TRIGGER_ISP);
	stBYCTRL.stDEFOGDWDR.defog_threshold = val;
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
{
	BYTE buff[2];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)	
	EEPROM_MultiWrite(EEPROM_DEFOGDWDR_BRIGHT, (PBYTE)&buff[0], sizeof(buff));
#else	
	EEPROM_MultiWrite(EEPROM_DEFOG_BRIGHT, (PBYTE)&buff[0], sizeof(buff));
#endif	
}
#endif
}
#endif

BYTE GetBY_DefogThreshold(void)
{
	return  stBYCTRL.stDEFOGDWDR.defog_threshold;
}

void SetBY_DefogAutoLevel(WORD val)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveDefogAutoLevel(val);
#endif
	
	stBYCTRL.ctrl |= (BYCTRL_DEFOG_THRESHOLD_UPDATE | BYCTRL_TRIGGER_ISP);
	stBYCTRL.stDEFOGDWDR.defog_auto_level = val;
	
	//UARTprintf("SetBY_DefogAutoLevel: %d\r\n", val);	
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
{
	BYTE buff[2];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
	EEPROM_MultiWrite(EEPROM_DEFOG_AUTO_LEVEL, (PBYTE)&buff[0], sizeof(buff));
}
#endif
}

BYTE GetBY_DefogAutoLevel(void)
{
	return  stBYCTRL.stDEFOGDWDR.defog_auto_level;
}

void SetBY_DWDRAutoLevel(WORD val)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveDWDRAutoLevel(val);
#endif
	
	stBYCTRL.ctrl |= (BYCTRL_DEFOG_THRESHOLD_UPDATE | BYCTRL_TRIGGER_ISP);
	stBYCTRL.stDEFOGDWDR.dwdr_auto_level = val;

	//UARTprintf("SetBY_DefogAutoLevel: %d\r\n", val);	
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
{
	BYTE buff[2];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
	EEPROM_MultiWrite(EEPROM_DWDR_AUTO_LEVEL, (PBYTE)&buff[0], sizeof(buff));
}
#endif
}

BYTE GetBY_DWDRAutoLevel(void)
{
	return  stBYCTRL.stDEFOGDWDR.dwdr_auto_level;
}


#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
void SetBY_DWDRSatSync(WORD val)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveDWDRSatSync(val);
#endif
	
	stBYCTRL.ctrl |= (BYCTRL_DWDR_SAT_SYNC_UPDATE | BYCTRL_TRIGGER_ISP);
	stBYCTRL.stDEFOGDWDR.dwdr_saturation_sync = val;
	//UARTprintf("SetBY_DWDRSatSync: %d\r\n", val);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)
	EEPROM_MultiWrite(EEPROM_DEFOGDWDR_SATSYNC, (PBYTE)&buff[0], sizeof(buff));
#else	
	EEPROM_MultiWrite(EEPROM_DWDR_SATSYNC, (PBYTE)&buff[0], sizeof(buff));
#endif
	
#endif	
	
}

BYTE GetBY_DWDRSatSync(void)
{
	return  stBYCTRL.stDEFOGDWDR.dwdr_saturation_sync;
}

#endif

//--------------------------------------------------------------------------------------------------------------------------
#if defined(__USE_MDIN_i500__)	
void SetBY_DefogBright(WORD val)
{
	stBYCTRL.ctrl |= (BYCTRL_DEFOG_STRENGTH_UPDATE | BYCTRL_TRIGGER_ISP);
	stBYCTRL.stDEFOGDWDR.defog_skya_ratio = 60 - (14 + val*2);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)
	BYTE buff[2];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
	EEPROM_MultiWrite(EEPROM_DEFOGDWDR_BRIGHT, (PBYTE)&buff[0], sizeof(buff));
#endif
#endif
}


//--------------------------------------------------------------------------------------------------------------------------
BYTE GetBY_DefogBright(void)
{
	return  (60 - (14 + stBYCTRL.stDEFOGDWDR.defog_skya_ratio)) / 2;
}
#endif

#if defined(__USE_MDIN_i500__)	
//--------------------------------------------------------------------------------------------------------------------------
void SetBY_DefogDispSel(WORD val)
{
	stBYCTRL.ctrl |= (BYCTRL_DEFOG_SCREEN_UPDATE | BYCTRL_TRIGGER_ISP);		
	stBYCTRL.defog_screen_ctrl = val ;	
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
BOOL GetBY_DefogActive(void)
{
	return stBYCTRL.stDEFOGDWDR.defog_active ;
}
//--------------------------------------------------------------------------------------------------------------------------
BOOL GetBY_DWDRActive(void)
{
	return stBYCTRL.stDEFOGDWDR.dwdr_active ;
}
//--------------------------------------------------------------------------------------------------------------------------
BYTE GetBY_DefogMode(void)
{
	return stBYCTRL.stDEFOGDWDR.defog_mode ;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE GetBY_DefogStrength(void)
{
	//return stBYCTRL.stDEFOGDWDR.defog_strength;
	return stBYCTRL.stDEFOGDWDR.defog_strength/2;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE GetBY_DefogDispSel(void)
{
	return stBYCTRL.defog_screen_ctrl;	
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE GetBY_dwdr_mode(void)
{
	return stBYCTRL.stDEFOGDWDR.dwdr_mode;
}

#if defined(__USE_DEFOG_DWDR_ONE_MENU__)
BYTE GetBY_DefogDWDRMode(void)
{
	return stBYCTRL.stDEFOGDWDR.defog_dwdr_mode;
}
#endif

void SetBY_GammaGainValue(void)
{
	BYTE gamma_value;
	

	stBYCTRL.ctrl |= (BYCTRL_BYGAMMA_LUT_UPDATE | BYCTRL_TRIGGER_ISP | BYCTRL_BYGAMMA_GAIN_UPDATE);		

	gamma_value = stBYCTRL.stGAMMA.gamma_default;

	stBYCTRL.stGAMMA.gamma_r = gamma_value;
	stBYCTRL.stGAMMA.gamma_g = gamma_value;
	stBYCTRL.stGAMMA.gamma_b = gamma_value;		

#if __MISP100_DBGPRT__ == 1
	UARTprintf("SetBAYER_GAMMA GAIN : %d \r\n", gamma_value);
#endif	
}

void SetBY_GammaValue(BYTE val)
{
	BYTE gamma_value = 11 ;
	

	stBYCTRL.ctrl |= (BYCTRL_BYGAMMA_LUT_UPDATE | BYCTRL_TRIGGER_ISP);		

	if ( val == 0 ){ 	//default
		gamma_value = stBYCTRL.stGAMMA.gamma_default;
	}		
	else if ( val == 1 ) {	gamma_value = MISP_GAMMA_OFF ;	}	//off(1.0)
	else if ( val == 2 ) {	gamma_value = MISP_GAMMA_090 ;	}	//0.9
	else if ( val == 3 ) {	gamma_value = MISP_GAMMA_080 ;	}	//0.8
	else if ( val == 4 ) {	gamma_value = MISP_GAMMA_070 ;	}	//0.7
	else if ( val == 5 ) {	gamma_value = MISP_GAMMA_060 ;	}	//0.6
	else if ( val == 6 ) {	gamma_value = MISP_GAMMA_055 ;	}	//0.55	
	else if ( val == 7 ) {	gamma_value = MISP_GAMMA_050 ;	}	//0.5
	else if ( val == 8 ) {	gamma_value = MISP_GAMMA_045 ;	}	//0.45	
	else if ( val == 9 ) {	gamma_value = MISP_GAMMA_040 ;	}	//0.4	
	else if ( val == 10 ) { 				
		#if defined(__COLOR_S7320__)
			if(GetOUT_MainCSC()==2)	gamma_value = MISP_GAMMA_CUSTOM2 ;	
			else if(GetOUT_MainCSC()==1)	gamma_value = MISP_GAMMA_CUSTOM3 ;					
			else				gamma_value = MISP_GAMMA_CUSTOM1 ;				
		#else
			gamma_value = MISP_GAMMA_CUSTOM3 ;	
		#endif
	}		

	stBYCTRL.stGAMMA.gamma_r = gamma_value;
	stBYCTRL.stGAMMA.gamma_g = gamma_value;
	stBYCTRL.stGAMMA.gamma_b = gamma_value;		

#if __MISP100_DBGPRT__ == 1
	UARTprintf("SetBAYER_GAMMA : %d \r\n", gamma_value);
#endif	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetBY_Bayer_data_order ( BYTE val )
{
	stBYCTRL.bayer_pixel_id = val ;
	
	stBYCTRL.ctrl |= (BYCTRL_DATA_ORDER_UPDATE | SSCTRL_TRIGGER_ISP);
}
//--------------------------------------------------------------------------------------------------------------------------
void SetBY_image_size(BYTE bH , BYTE bV, WORD valH , WORD valV)
{
	stBYCTRL.ctrl |= (BYCTRL_IMAGE_SIZE_UPDATE | SSCTRL_TRIGGER_ISP);
	
	stBYCTRL.bH = bH;
	stBYCTRL.bV = bV;
	stBYCTRL.active_h_size = valH;
	stBYCTRL.active_v_size = valV;	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetBY_5MBypass(BOOL val)
{
	stBYCTRL.bayer_5m_bypass = val;
	stBYCTRL.ctrl |= (BYCTRL_BYPASS_UPDATE | BYCTRL_TRIGGER_ISP);
	
#if __MISP100_DBGPRT__ == 1
	UARTprintf("SetBY_5MBypass : %s \r\n", (val==1) ? "Bypass ON" : "Bypass OFF");
#endif	
}


//--------------------------------------------------------------------------------------------------------------------------
#if defined(__USE_MDIN_i500__)
void SetBY_DWDR_gamma(BYTE mode, BYTE val) 
{
	if(mode==3) 	stBYCTRL.stDEFOGDWDR.dwdr_natural_gamma=val;
	else if(mode>0)	stBYCTRL.stDEFOGDWDR.dwdr_on_gamma=val;
	else stBYCTRL.stDEFOGDWDR.dwdr_off_gamma=val;
}
BYTE GetBY_DWDR_gamma(BYTE mode)  
{
	if(mode==3) return stBYCTRL.stDEFOGDWDR.dwdr_natural_gamma;
	else if(mode>0) return stBYCTRL.stDEFOGDWDR.dwdr_on_gamma;
	else return stBYCTRL.stDEFOGDWDR.dwdr_off_gamma;
}
void SetBY_DWDR_AEtarget(BYTE mode, BYTE val) 
{
	if(mode==3) 	stBYCTRL.stDEFOGDWDR.dwdr_natural_aetarget=val;
	else if(mode>0)	stBYCTRL.stDEFOGDWDR.dwdr_on_aetarget=val;
	else stBYCTRL.stDEFOGDWDR.dwdr_off_aetarget=val;
}
BYTE GetBY_DWDR_AEtarget(BYTE mode)  
{
	if(mode==3) return stBYCTRL.stDEFOGDWDR.dwdr_natural_aetarget;
	else if(mode>0) return stBYCTRL.stDEFOGDWDR.dwdr_on_aetarget;
	else return stBYCTRL.stDEFOGDWDR.dwdr_off_aetarget;
}
//--------------------------------------------------------------------------------------------------------------------------
#else	// MDIN-i51X (i510, i540, i550)
BOOL GetBY_gamma_mode(void) 
{
	return stBYCTRL.stGAMMA.gamma_mode;
}
void SetBY_gamma_8tap(BYTE nID)
{
	stBYCTRL.stGAMMA.gamma_8tap = nID;
}
BYTE GetBY_gamma_8tap(void) 
{
	return stBYCTRL.stGAMMA.gamma_8tap;
}
void SetBY_BY256LUT_Extend(BOOL OnOff)
{
	stBYCTRL.ctrl |= (BYCTRL_BYGAMMA_LUT_UPDATE| BYCTRL_BYGAMMA_EXTEND_UPDATE | BYCTRL_TRIGGER_ISP);
	stBYCTRL.stGAMMA.gamma_extend=OnOff;
}
void SetBY_BYLUT_Type(BYTE nID)
{
	stBYCTRL.stGAMMA.gamma_lut=nID;
	MISP_RegField(MISP_LOCAL_ID, __BYGAMMA_CTRL_ADDR__, 11, 2, stBYCTRL.stGAMMA.gamma_lut) ;
}
#endif
//--------------------------------------------------------------------------------------------------------------------------
BYTE GetBY_gamma_value(void)
{
	return stBYCTRL.stGAMMA.gamma_r;
}
void SetBY_gamma_value(BYTE val)
{
	stBYCTRL.stGAMMA.gamma_r = val;
	stBYCTRL.stGAMMA.gamma_g = val;
	stBYCTRL.stGAMMA.gamma_b = val;
	stBYCTRL.ctrl |= (BYCTRL_BYGAMMA_LUT_UPDATE | BYCTRL_TRIGGER_ISP);
}
BOOL GetBY_gamma_enable(void)
{
	return stBYCTRL.stGAMMA.gamma_en;
}
void SetBY_gamma_enable(BOOL val)
{
	stBYCTRL.stGAMMA.gamma_en = val;
	stBYCTRL.ctrl |= (BYCTRL_BYGAMMA_EN_UPDATE | BYCTRL_TRIGGER_ISP);
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL GetBY_Lens_corr_en(void)
{
	return  (stBYCTRL.stLENSCORR.lens_corr_en)?  ON:  OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE GetBY_GammaValue(void)
{
	BYTE val;
	
	switch(stBYCTRL.stGAMMA.gamma_r)	{
		case  0: 		val=1;	break; // off(1.0)
		case  2: 		val=2;	break; // 0.9
		case  4: 		val=3;	break; // 0.8
		case  6: 		val=4;	break; // 0.7
		case  8: 		val=5;	break; // 0.6
		case  9: 		val=6;	break; // 0.55
		case  10: 		val=7;	break; // 0.5
		case  11: 		val=8;	break; // 0.45
		case  12: 		val=9;	break; // 0.4
		default: 		val=10;	break; // custom
	}
	
	if(stBYCTRL.stGAMMA.gamma_r == stBYCTRL.stGAMMA.gamma_default)		val=0;	// on(default)
	
	return  val;
}


/*  FILE_END_HERE */
