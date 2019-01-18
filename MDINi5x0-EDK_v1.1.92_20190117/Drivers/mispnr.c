// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"
#include	"misp100.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

#if __NR_REV__
static ROMDATA BYTE MISP_NR_SFILTER[][14] = {
//     0     1     2     3     4     5     6     7     8     9     A     B     C     D   
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x06, 0x18 }, // sigma 0.6
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x14, 0x2D }, // sigma 0.8
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x08, 0x0D, 0x24, 0x3C }, // sigma 1
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x04, 0x06, 0x11, 0x18, 0x31, 0x46 }, // sigma 1.2	
	{0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x03, 0x07, 0x0A, 0x0C, 0x1B, 0x24, 0x3C, 0x4D }, // sigma 1.4	
	{0x00, 0x01, 0x03, 0x05, 0x06, 0x04, 0x0A, 0x11, 0x15, 0x19, 0x2A, 0x32, 0x46, 0x54 }, // sigma 1.7	
	{0x03, 0x07, 0x0C, 0x11, 0x13, 0x0F, 0x1A, 0x23, 0x27, 0x2B, 0x3B, 0x42, 0x51, 0x5A }, // sigma 2.2	
	{0x0C, 0x14, 0x1B, 0x21, 0x24, 0x1F, 0x2B, 0x34, 0x38, 0x3C, 0x48, 0x4D, 0x58, 0x5D }, // sigma 2.8
	{0x1b, 0x24, 0x2c, 0x31, 0x34, 0x2f, 0x3a, 0x42, 0x45, 0x48, 0x51, 0x54, 0x5c, 0x60 }, // sigma 3.5
};
/*
0	1	2	3	4
1	5	6	7	8
2	6	9	A	B
3	7	A	C	D
4	8	B	D	x
*/
#endif

// Bigger the table number, weaker the range LUT
static ROMDATA BYTE MISP_NR_NOISEBOUND[][5] = {
	{ 2, 4, 8,  11, 14 },
	{ 4, 8, 16, 22, 28 },
	{ 8, 16, 25, 30, 35 },
};

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static NR_CTRL_PARAM stNRCTRL;
// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
static void Init_NR_PARAM( PNR_CTRL_PARAM pCTRL ) ;
static void Init_NR_REGISTER( PNR_CTRL_PARAM pCTRL ) ;
static void NRCTRL_DemoModeHandler(PNR_CTRL_PARAM pCTRL) ;
static void NRCTRL_LowLightLevelHandler(PNR_CTRL_PARAM pCTRL) ;
static void NRCTRL_3DintensityHandler(PNR_CTRL_PARAM pCTRL) ;
static void NRCTRL_ModeHandler(PNR_CTRL_PARAM pCTRL) ;
static void NRCTRL_ImageSizeHandler(PNR_CTRL_PARAM pCTRL) ;

static void NRCTRL_IDMHandler(PNR_CTRL_PARAM pCTRL) ;
static void NRCTRL_IDMcHandler(PNR_CTRL_PARAM pCTRL) ;

static void NRCTRL_PDMHandler(PNR_CTRL_PARAM pCTRL) ;
static void NRCTRL_SRMHandler(PNR_CTRL_PARAM pCTRL) ;

static void NRCTRL_MCUDriveHandler(PNR_CTRL_PARAM pCTRL) ;
static void NRCTRL_InputImageTypeHandler(PNR_CTRL_PARAM pCTRL) ;
static void NRCTRL_DataOrderHandler ( PNR_CTRL_PARAM pCTRL ) ;
static void NRCTRL_LinememHandler ( PNR_CTRL_PARAM pCTRL ) ;

#if __NR_REV__
static void NRCTRL_2D_sFilterHandler ( PNR_CTRL_PARAM pCTRL ) ;
static void NRCTRL_2D_MFilterHandler ( PNR_CTRL_PARAM pCTRL ) ;
#endif

static void NRCTRL_2D_NBHandler ( PNR_CTRL_PARAM pCTRL ) ;

static void NRCTRL_2D_LFilterHandler ( PNR_CTRL_PARAM pCTRL ) ;

static void NRCTRL_WdrFlagControlHandler(PNR_CTRL_PARAM pCTRL) ;

static void NRCTRL_FrameStdevUpdate( PNR_CTRL_PARAM pCTRL)	;
static void NRCTRL_AUTOPEL( PNR_CTRL_PARAM pCTRL)	;

static void NRCTRL_AUTOL2DNR ( PNR_CTRL_PARAM pCTRL ) ;

#if __NR_REV__
static void NRCTRL_AUTOM2DNR ( PNR_CTRL_PARAM pCTRL ) ;
static void NRCTRL_AUTO2DNR ( PNR_CTRL_PARAM pCTRL ) ;	
#endif

static void NRCTRL_AUTOIDM ( PNR_CTRL_PARAM pCTRL ) ;	
static void NRCTRL_AUTOIDMc ( PNR_CTRL_PARAM pCTRL ) ;
static void NRCTRL_AUTOPDM ( PNR_CTRL_PARAM pCTRL ) ;	
static void NRCTRL_AUTOSRM ( PNR_CTRL_PARAM pCTRL ) ;

static void NRCTRL_AUTO3DNR ( PNR_CTRL_PARAM pCTRL ) ;	

static void NRCTRL_SetCustomBlockSize ( PNR_CTRL_PARAM pCTRL ) ;	
#if __NR_SLUT_GENFUNC__	
static void NRCTRL_SLUT_gen(float sigma_s) ;
#endif
// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------
static void NRCTRL_SetCustomBlockSizeNOTL ( PNR_CTRL_PARAM pCTRL )
{ // Special setting for image size override that is Not On The List
	WORD in_hsize ;
	WORD in_vsize ;
	
	if ( GetROT_RotationEnable() )
	{
		in_hsize = pCTRL->stLINEMEM.active_v_size + (pCTRL->stLINEMEM.bV<<1) ;
		in_vsize = pCTRL->stLINEMEM.active_h_size + (pCTRL->stLINEMEM.bH<<1) ;
	}
	else
	{
		in_hsize = pCTRL->stLINEMEM.active_h_size + (pCTRL->stLINEMEM.bH<<1) ;
		in_vsize = pCTRL->stLINEMEM.active_v_size + (pCTRL->stLINEMEM.bV<<1) ;
	}
	
	// Does not change the total input size setting but overide "INTERNAL IMAGE SIZE" for proper operation
	if( in_hsize == 636 && in_vsize == 476  )
	{
		pCTRL->stLINEMEM.bH = 3 ;
		pCTRL->stLINEMEM.bV = 0 ;
		pCTRL->stLINEMEM.active_h_size = 630 ;
		pCTRL->stLINEMEM.active_v_size = 476 ;
		pCTRL->stSRM.block_h_size = 15 ;	
		pCTRL->stSRM.block_v_size = 14;		
	}
	else if( in_hsize ==  1924 && in_vsize == 2164 )
	{ 
		pCTRL->stLINEMEM.bH = 2 ;
		pCTRL->stLINEMEM.bV = 2 ;
		pCTRL->stLINEMEM.active_h_size = 1920 ;
		pCTRL->stLINEMEM.active_v_size = 2160 ;
		pCTRL->stSRM.block_h_size = 24 ;
		pCTRL->stSRM.block_v_size = 24 ;	
	}	
	// Add to adjust abnormal  image size
	// else if( in_hsize ==  || in_vsize ==   )
	// { 
		// pCTRL->stLINEMEM.bH =  ;
		// pCTRL->stLINEMEM.bV =  ;
		// pCTRL->stLINEMEM.active_h_size = ;
		// pCTRL->stLINEMEM.active_v_size = ;
		// pCTRL->stSRM.block_h_size = ;
		// pCTRL->stSRM.block_v_size = ;	
	// }
	else
	{
		pCTRL->stSRM.block_h_size = 32 ;	
		pCTRL->stSRM.block_v_size = 36 ;		
	}
}

static void NRCTRL_SetCustomBlockSize ( PNR_CTRL_PARAM pCTRL )
{ // The minimum SRM block size is 15x15 & total block count(total blocks in the one frame) can not exceed 8192 

	WORD ref_h ;
	WORD ref_v ;
	WORD hv_swap ;	
	
#if __NR_REV__
	if ( GetROT_RotationEnable() ) {
		ref_h = pCTRL->stLINEMEM.active_v_size ; // If Rotation enabled
		ref_v = pCTRL->stLINEMEM.active_h_size ; // If Rotation enabled
	}
	else {
		ref_h = pCTRL->stLINEMEM.active_h_size ; // If Rotation disabled
		ref_v = pCTRL->stLINEMEM.active_v_size ; // If Rotation disabled
	}
#else
	ref_h = pCTRL->stLINEMEM.active_h_size ;
	ref_v = pCTRL->stLINEMEM.active_v_size ;
#endif
	
	if( ref_h == 640 ) //  480p , 512p SD
	{
#if __NR_REV__		
		pCTRL->stSRM.block_h_size = 16 ;	
		if (ref_v == 480) pCTRL->stSRM.block_v_size = 15 ;
		else pCTRL->stSRM.block_v_size = 16 ;
#else
		pCTRL->stSRM.block_h_size = 20 ;	
		if (ref_v == 480) pCTRL->stSRM.block_v_size = 15 ;
		else pCTRL->stSRM.block_v_size = 16 ;
#endif		
	}
	if(ref_h == 1280 && ref_v == 720  ) // 720p HD
	{
#if __NR_REV__		
		pCTRL->stSRM.block_h_size = 16 ;	
		pCTRL->stSRM.block_v_size = 16 ;
#else
		pCTRL->stSRM.block_h_size = 20 ;	
		pCTRL->stSRM.block_v_size = 20 ;
#endif			
	}
	else if ( ref_h == 1920 && ref_v == 1080 ) // 2Mega FHD
	{
#if __NR_REV__		
		pCTRL->stSRM.block_h_size = 16 ;	
		pCTRL->stSRM.block_v_size = 18 ;
#else
		pCTRL->stSRM.block_h_size = 32 ;	
		pCTRL->stSRM.block_v_size = 36 ;
#endif	
	}	
	else if ( ref_h == 2048 && ref_v == 1536 ) // 3Mega 2048x1536
	{
#if __NR_REV__		
		pCTRL->stSRM.block_h_size = 32 ;	
		pCTRL->stSRM.block_v_size = 24 ;
#else
		pCTRL->stSRM.block_h_size = 32 ;	
		pCTRL->stSRM.block_v_size = 32 ;	
#endif			
	}	
	else if ( ref_h == 3840 && ref_v == 2160 )  // 4K UHD
	{
		pCTRL->stSRM.block_h_size = 32 ;	
		pCTRL->stSRM.block_v_size = 36 ;	
	}	
	else{ //  ref_h is not  on the list
		NRCTRL_SetCustomBlockSizeNOTL(pCTRL);
	}	
	
#if __NR_REV__
	if ( GetROT_RotationEnable() ) // Rotation enabled
	{
		hv_swap = pCTRL->stSRM.block_v_size ;
		pCTRL->stSRM.block_v_size = pCTRL->stSRM.block_h_size ;
		pCTRL->stSRM.block_h_size = hv_swap ;
	}	
#endif
}
// ----------------------------------------------------------------------
#if __NR_SLUT_GENFUNC__	
static void NRCTRL_SLUT_gen(float sigma_s)
{
	// For generating slut only : 
	float sigma_s_square ;
	int x, y ;
	float temp;
    BYTE spatial_lut[81];
	PBYTE sLUTp;

	sigma_s_square = sigma_s * sigma_s ;

	sLUTp = spatial_lut ;
	
	for( y = -4 ; y <= 4 ; y++)
	{
		for( x = -4 ; x <= 4 ; x++)
		{
			temp=((float)((x*x)+(y*y)))/((float)sigma_s_square);
			temp=100.0*exp(-0.5*temp);
			*(sLUTp)= (BYTE)temp;
			sLUTp++ ;
		}
	}

	/*
	0	1	2	3	4
	1	5	6	7	8
	2	6	9	A	B
	3	7	A	C	D
	4	8	B	D	x
	*/	
	
	if  ( spatial_lut[20] == 0 ) spatial_lut[20] = 1 ;
	if  ( spatial_lut[22] == 0 ) spatial_lut[22] = 1 ;
	if  ( spatial_lut[38] == 0 ) spatial_lut[38] = 1 ;
	
	UARTprintf("\r\nNR SLUT gen : %2.1f\r\n",sigma_s) ;
	UARTprintf(" 0: %3d | 1: %3d | 2: %3d | 3: %3d | 4: %3d |\r\n", spatial_lut[0], spatial_lut[1], spatial_lut[2], spatial_lut[3], spatial_lut[4]) ;
	UARTprintf(" 1: %3d | 5: %3d | 6: %3d | 7: %3d | 8: %3d |\r\n", spatial_lut[9], spatial_lut[10], spatial_lut[11], spatial_lut[12], spatial_lut[13]) ;
	UARTprintf(" 2: %3d | 6: %3d | 9: %3d | A: %3d | B: %3d |\r\n", spatial_lut[18], spatial_lut[19], spatial_lut[20], spatial_lut[21], spatial_lut[22]) ;
	UARTprintf(" 3: %3d | 7: %3d | A: %3d | C: %3d | D: %3d |\r\n", spatial_lut[27], spatial_lut[28], spatial_lut[29], spatial_lut[30], spatial_lut[31]) ;
	UARTprintf(" 4: %3d | 8: %3d | B: %3d | D: %3d | X: %3d |\r\n", spatial_lut[36], spatial_lut[37], spatial_lut[38], spatial_lut[39], spatial_lut[40]) ;
	
	UARTprintf("\rCopy below code\r\n") ;
	UARTprintf("{0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x }\r\n", spatial_lut[0], spatial_lut[1], spatial_lut[2], spatial_lut[3], spatial_lut[4],spatial_lut[10], spatial_lut[11], spatial_lut[12], spatial_lut[13],spatial_lut[20], spatial_lut[21], spatial_lut[22],spatial_lut[30], spatial_lut[31]) ;

}
#endif // #if __NR_SLUT_GENFUNC__	
// ----------------------------------------------------------------------

static void Init_NR_PARAM( PNR_CTRL_PARAM pCTRL ){
	
#if __NR_SLUT_GENFUNC__	
	NRCTRL_SLUT_gen(__NR_SLUT_SIGMA_VALUE__); // For generating slut only : 
#endif	
	
	// start parameter for NR control		
	BOOL gamma_mode ;
	BOOL sensor_bit_opertion ;
	WORD sensor_frmt ;
	WORD input_h_size ;
	
#ifdef __NR_YC_IMAGE_SETUP__
	pCTRL->image_type = MISP_NR_YC_IMAGE ; // 0:MISP_NR_BAYER_IMAGE  1:MISP_NR_YC_IMAGE		
	pCTRL->stFRAMENOISE.edge_level = __NR_YC_EDGE_LEVEL__ ;
#else
	pCTRL->image_type = MISP_NR_BAYER_IMAGE ; // 0:MISP_NR_BAYER_IMAGE  1:MISP_NR_YC_IMAGE
	pCTRL->stFRAMENOISE.edge_level = __NR_BY_EDGE_LEVEL__ ;
#endif

#if defined(__USE_EXT2IPP__) ||  defined(__USE_NO__CMOS__)
	pCTRL->mode = MISP_NR_OFF ;
#elif defined(__USE_CMOS_IMX226__)||defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__) // 4K sensors start with 2DNR
	// pCTRL->mode = MISP_NR_2D ;
	pCTRL->mode = MISP_NR_2D3D ;
#else
	pCTRL->mode = MISP_NR_2D3D ;
#endif	
	
#if defined(__NR_ZOOM_LENS_SETUP__)
	#if defined (__M33_V101__)
		// pCTRL->intensity = MISP_NR3D_HIGH;
		pCTRL->intensity = MISP_NR3D_LOW;
	#else
		pCTRL->intensity = MISP_NR3D_AUTO;
	#endif
#else
	pCTRL->intensity = MISP_NR3D_AUTO;
#endif
	pCTRL->lowlight_mode = MISP_NRLOWLIGHT_OFF; // read only value
	pCTRL->demo_mode = MISP_NRDEMO_OFF;

#if defined(__USE_CMOS_PO3100K__) 
	pCTRL->stLL.ent_level=5;	pCTRL->stLL.ent_level_fr=2;	//enter level : 5.250
	pCTRL->stLL.exit_level=5;	pCTRL->stLL.exit_level_fr=1;	//exit level : 5.125	
#else
#if defined (__M33_V101__)
	pCTRL->stLL.ent_level=8;	pCTRL->stLL.ent_level_fr=2;	//enter level : 9.875
	pCTRL->stLL.exit_level=8;	pCTRL->stLL.exit_level_fr=3;	//exit level : 9.750
#else	
#if defined (__USE_CMOS_IMX185__)
	pCTRL->stLL.ent_level=7;	pCTRL->stLL.ent_level_fr=7;	//enter level : 7.875
	pCTRL->stLL.exit_level=7;	pCTRL->stLL.exit_level_fr=6;	//exit level : 7.750
#else
	pCTRL->stLL.ent_level=7;	pCTRL->stLL.ent_level_fr=7;	//enter level : 7.875
	pCTRL->stLL.exit_level=7;	pCTRL->stLL.exit_level_fr=6;	//exit level : 7.750
#endif	
#endif
#endif

	pCTRL->stLINEMEM.active_h_size = GetSS_image_active_size_h() ;
	pCTRL->stLINEMEM.active_v_size = GetSS_image_active_size_v() ;	
	
	if ( pCTRL->stLINEMEM.active_h_size <= 2048 ) pCTRL->stFRAMENOISE.corr_ratio = 0 ;		
	else pCTRL->stFRAMENOISE.corr_ratio = 1 ;		
	
	stNRCTRL.aperture = __NR_INITIAL_APERTURE__ ;  // 0~2~4
	
	if ( pCTRL->image_type == MISP_NR_BAYER_IMAGE ){
		pCTRL->stLINEMEM.bH = GetSS_image_boundary_size_h() ;
		pCTRL->stLINEMEM.bV = GetSS_image_boundary_size_v() ;
		pCTRL->stLINEMEM.y_linemem_en = 1 ;		
		pCTRL->stLINEMEM.c_linemem_en = 0 ;				
	}
	else{
		pCTRL->stLINEMEM.bH = 0 ;
		pCTRL->stLINEMEM.bV = 0 ;	
		pCTRL->stLINEMEM.y_linemem_en = 1 ;		
		if ( pCTRL->stLINEMEM.active_h_size <= 2048 ) pCTRL->stLINEMEM.c_linemem_en = 1 ;		
		else pCTRL->stLINEMEM.c_linemem_en = 0 ;		
	}	
	
	input_h_size = pCTRL->stLINEMEM.active_h_size + (pCTRL->stLINEMEM.bH<<1) ;
	
#if __NR_REV__
	gamma_mode = GetBY_gamma_mode() ;
	sensor_frmt = GetSS_SensorVoutFrmt() ;
#else
	gamma_mode = 0 ;
	sensor_frmt = 0 ;
#endif 
	sensor_bit_opertion = GetSS_sensor_bit_opertion() ;

#if defined(__USE_MDIN_i540__)||defined(__USE_MDIN_i550__)
	if ( sensor_frmt == MISP_VIDSRC_1920x1080p120 || \
		sensor_frmt == MISP_VIDSRC_1920x1080p100 || \
		sensor_frmt == MISP_VIDSRC_3840x2160p30 || \
		sensor_frmt == MISP_VIDSRC_SS3840x2160p25 || \
		sensor_frmt == MISP_VIDSRC_4096x2160p24 || \
		sensor_frmt == MISP_VIDSRC_4096x2160p20 || \
		sensor_frmt == MISP_VIDSRC_4000x3000p22 ) {
		// For pixel clock Near 300Mhz (4K sensors, 2K@120fps sensors)
		pCTRL->nr_10bit = 0 ;
	}
#if defined(__USE_4K60_INOUT__)
	else if ( sensor_frmt == MISP_VIDSRC_1920x2160p50 || \
			sensor_frmt == MISP_VIDSRC_1920x2160p60 )	{
		// For pixel clock Near 300Mhz (4K sensors, 2K@120fps sensors)
		pCTRL->nr_10bit = 0 ;
	}
#endif
	else if ( input_h_size <= 768 ) { // Under input  H size 768 	
		pCTRL->nr_10bit = 0 ;
	}
	else if ( gamma_mode == 1 || sensor_bit_opertion == 1 ) {
		pCTRL->nr_10bit = 1 ;	
	}
	else {
		pCTRL->nr_10bit = __NR_10BIT_SETUP__ ;	
	}
#else
	if ( input_h_size <= 768 )  // Under input  H size 768 	
		pCTRL->nr_10bit = 0 ;
	else if ( gamma_mode == 1 || sensor_bit_opertion == 1 ) 
		pCTRL->nr_10bit = 1 ;	
	else 
		pCTRL->nr_10bit = __NR_10BIT_SETUP__ ;	
#endif


#if defined(__USE_IMX273_720P240_MODE__)&&!defined(__USE_ISP_SCLK_OUT__)	
	pCTRL->nr_10bit = 0 ;
#endif

	pCTRL->stFRAMENOISE.bright_lower_bound = 80 ;
	pCTRL->stFRAMENOISE.dark_upper_bound = 125 ;
	
	pCTRL->stLINEMEM.active_counter = __NR_ACTIVE_COUNTER__ ;		
	
	SetNR_Bayer_data_order((MISP_SS_BAYER_PATTERN_t)GetSS_bayer_data_order()) ;
	
	pCTRL->stIDM.idm_en = 1 ;
	pCTRL->stIDM.idm_auto = 1 ;	
	
	SetNR_IDM_ratio(1,3,3);
	
#if __NR_SPLIT__==0	
	pCTRL->stIDM.id_dy_noise_limit = 1 ;
	pCTRL->stIDM.id_ll_dy_noise_limit = 1 ;
	pCTRL->stPDM.pd_dy_noise_limit = 0 ;
	pCTRL->stPDM.pd_ll_dy_noise_limit = 1 ;
	pCTRL->stSRM.sr_dy_th = 0 ;	
	pCTRL->stSRM.sr_ll_dy_th = 1 ;		
#else // No Dy_noise limit when NR split mode
	pCTRL->stIDM.id_dy_noise_limit = 0 ;
	pCTRL->stIDM.id_ll_dy_noise_limit = 0 ;
	pCTRL->stPDM.pd_dy_noise_limit = 0 ;
	pCTRL->stPDM.pd_ll_dy_noise_limit = 0 ;
	pCTRL->stSRM.sr_dy_th = 0 ;	
	pCTRL->stSRM.sr_ll_dy_th = 0 ;	
	SetNR_MCUDriveMode(1,0,0,0,0) ; // MCU drive operation
#endif	
	
#if __NR_REV__	
	pCTRL->stIDM.id_noise_source = 3 ;
#else	
	pCTRL->stIDM.id_noise_source = 1 ;
#endif

	pCTRL->stIDM.id_k_value = 32 ;
	pCTRL->stIDM.id_noise_limit = 8 ;
	pCTRL->stIDM.id_thres_3d_nr = 22 ;
	pCTRL->stIDM.id_thres_3d_2d_nr = 0 ;
	pCTRL->stIDM.id_thres_pre_3d_cu_2d_nr = 3 ;
	pCTRL->stIDM.id_thres_pre_3d_2d_cu_2d_nr = 4 ;
	
	pCTRL->stIDM.id_ll_noise_limit = 10 ;
	pCTRL->stIDM.id_ll_thres_3d_nr = 22 ;
	pCTRL->stIDM.id_ll_thres_3d_2d_nr = 0 ;
	pCTRL->stIDM.id_ll_thres_pre_3d_cu_2d_nr = 3 ;
	pCTRL->stIDM.id_ll_thres_pre_3d_2d_cu_2d_nr = 8 ;

	pCTRL->stLL.ll_en = 1 ;
	pCTRL->stLL.ll_enter_th = 10 ;
	pCTRL->stLL.ll_exit_th = 5 ;
	
	pCTRL->stIDM.c_idm_en = pCTRL->stLINEMEM.c_linemem_en ;
	pCTRL->stIDM.c_idm_auto = pCTRL->stLINEMEM.c_linemem_en ;
	pCTRL->stIDM.c_id_k_value = 32 ;
	pCTRL->stIDM.c_id_thres_ratio = 5 ;
	pCTRL->stIDM.c_id_thres_raw_pren = 5 ;
	pCTRL->stIDM.c_id_thres_c3d_pren = 5 ;
	pCTRL->stIDM.c_id_ll_thres_raw_pren = 10 ;
	pCTRL->stIDM.c_id_ll_thres_c3d_pren = 10 ;
	
	pCTRL->stPDM.pdm_en = 1 ;
	pCTRL->stPDM.pdm_auto = 1 ;

#if __NR_REV__		
	pCTRL->stPDM.pdm_noise_source = 3 ;
#else
	pCTRL->stPDM.pdm_noise_source = 0 ;
#endif

	pCTRL->stPDM.pd_motion_k = 64 ;
	pCTRL->stPDM.pd_nutral_motion_k = 64 ;
	
	pCTRL->stPDM.pd_sads_ratio = 22 ;
	pCTRL->stPDM.pd_noise_limit = 4 ;
	pCTRL->stPDM.pd_motion_th = 0 ;
	pCTRL->stPDM.pd_motion_adj = 1 ;	
	
	pCTRL->stPDM.pd_ll_motion_k = 32 ;	
	pCTRL->stPDM.pd_ll_nutral_motion_k = 20 ;
	
	pCTRL->stPDM.pd_ll_sads_ratio = 25 ;
	pCTRL->stPDM.pd_ll_noise_limit = 0 ;
	pCTRL->stPDM.pd_ll_motion_th = 3 ;
	pCTRL->stPDM.pd_ll_motion_adj = 0 ;

	pCTRL->stSRM.srm_en = 1 ;
	pCTRL->stSRM.srm_auto = 1 ;

	pCTRL->stSRM.sr_th = 3 ;	
	pCTRL->stSRM.sr_th_fractional = 7 ;	

	pCTRL->stSRM.sr_ll_th = 125 ;	
	pCTRL->stSRM.sr_ll_th_fractional = 11 ;	
		
	pCTRL->stSRM.sr_bmi_corr_en = 0 ;
	pCTRL->stSRM.sr_mi_corr_en = 1 ;
	pCTRL->stSRM.sr_mi_corr_level = 3 ;
	pCTRL->stSRM.sr_mi_corr_value = 2 ;
	pCTRL->stSRM.sr_ll_mi_corr_value = 1 ;
	
	pCTRL->stSRM.ll_still_en = __NR_LL_STILL_FUNC__ ;
	
	pCTRL->stSRM.ll_still_mblk_cnt_th = 3 ;
	// pCTRL->stSRM.ll_still_frm_th = 32 ;
	pCTRL->stSRM.ll_still_frm_th = 16 ;
	pCTRL->stSRM.ll_still_move_duration = 2 ;
	pCTRL->stSRM.ll_still_max_intensity = 5 ;
	pCTRL->stSRM.ll_still_duration = 3 ;

#if __NR_REV__	
	#if defined(__USE_CMOS_IMX290__) || defined (__USE_CMOS_IMX327__) 
		pCTRL->st2DNR.rold_impulse_init = 9 ; // i5x0 rold_impulse_init range 0~100
	#else
		#if defined(__USE_CMOS_IMXx36__)	
			pCTRL->st2DNR.rold_impulse_init = 37 ; // i5x0 rold_impulse_init range 0~100
		#else
			pCTRL->st2DNR.rold_impulse_init = 9 ; // i5x0 rold_impulse_init range 0~100
		#endif
	#endif
#else
	pCTRL->st2DNR.rold_impulse_init = 9 ; // i500 rold_impulse_init range 0~15
#endif
	
	SetNR_2D_ImpulseThresh(0,30,0,2) ; // (BOOL op_mode , BYTE ImpulseThresh , BOOL filter_select , BYTE ImpulseReplace )

#if defined(__USE_CMOS_MN34041__)   || defined(__USE_CMOS_MN34220__) || defined(__USE_CMOS_MN34420__) 
	SetNR_2D_ImpulseThresh(1,24,0,2) ;	
#else
	SetNR_2D_ImpulseThresh(1,9,0,2) ;
#endif
	
	pCTRL->st2DNR.nf_bypass_level = 0 ;
	pCTRL->st2DNR.stdev_bypass_level = 0 ;
	
	pCTRL->st2DNR.noise_bound_mode = 1 ; // 0 : normal  1 : narrow 2: wide  3: Max
	
#if __NR_REV__	
	pCTRL->stPDM.pd_pmh_en = 0 ;
	pCTRL->stPDM.pd_pmh_thres = 4 ;
	pCTRL->stPDM.pd_pmh_k00 = 49 ;
	pCTRL->stPDM.pd_pmh_k01 = 31 ;
	pCTRL->stPDM.pd_pmh_k02 = 28 ;		
	
	pCTRL->st2DNR.noise_bound_mode_df = 2 ;  // 0 : normal  1 : narrow 2: wide  3: Max
	
	pCTRL->st2DNR.pmh_2D_en = 0 ;  //
	pCTRL->st2DNR.pmh_2D_th = 2 ;  //
	pCTRL->st2DNR.pmh_2D_mode = 0 ;  // 0 : normal pmh weighting 1 : max bl 2 : gaussian
	
	pCTRL->st2DNR.pmh_2Df_en = 0 ;  //
	pCTRL->st2DNR.pmh_2Df_th = 2 ;  //
	pCTRL->st2DNR.pmh_2Df_mode = 1 ;  // 0 : normal pmh weighting 1 : max bl 2 : gaussian		

	pCTRL->st2DNR.pmh_2Dc_en = 0 ;  //
	pCTRL->st2DNR.pmh_2Dc_th = 2 ;  //
	
	SetNR_2D_ImpulseThresh(0,30,1,2) ;
	SetNR_2D_ImpulseThresh(1,9,1,2) ;
	SetNR_2D_NoiseBound(2,0,1); // 2D  : Table no , filter select , nb mode // Bigger the table number, weaker the range LUT
	SetNR_2D_NoiseBound(0,1,1); // 2Df
#else	
	#if defined (__M33_V101__)
		SetNR_2D_NoiseBound(2,0,0);
	#else
		SetNR_2D_NoiseBound(1,0,0);	
	#endif
#endif	
	pCTRL->st2DNR.c_inbound_range = 20 ;
	
	pCTRL->st3DNR.update_counter = __NR_AUTO_PERIOD__ ;
	pCTRL->st3DNR.Adaptive_en = 0 ;
	pCTRL->st3DNR.k_value_mode = 0 ;
	pCTRL->st3DNR.const_k_value = 36 ;
	pCTRL->st3DNR.const_k_value_oth = 36 ;
	pCTRL->st3DNR.k_value_bound00 = __NR_AUTO3DNR_BOUND_0__ ;
	pCTRL->st3DNR.k_value_bound01 = __NR_AUTO3DNR_BOUND_1__ ;
	pCTRL->st3DNR.k_value_bound02 = __NR_AUTO3DNR_BOUND_2__ ;
	pCTRL->st3DNR.k_value_bound03 = __NR_AUTO3DNR_BOUND_3__ ;
	pCTRL->st3DNR.k_value_bound04 = __NR_AUTO3DNR_BOUND_4__ ;
	pCTRL->st3DNR.k_value_bound05 = __NR_AUTO3DNR_BOUND_5__ ;

	
	pCTRL->stWDR.wdr_tm_only_mode = 0;
	pCTRL->stWDR.mo_his_rd_en = 0;
	pCTRL->stWDR.mo_flag_rd_en = 0;
	pCTRL->stWDR.r_wdr_mo_en = 0;
	pCTRL->stWDR.r_color_suppress_en = 0;
	pCTRL->stWDR.r_chk_surface = 0;
	pCTRL->stWDR.r_2D_wdr_mo_mode = 0;

#if __NR_REV__	
	pCTRL->strategy = __NR_STRATEGY_LEVEL__ ;	
	// 0 : M filter off 
	// 1 : M filter low 
	// 2 : M filter mid
	// 3 : M filter high 
	// 4 : M filter off + fast serial
	// 5 : M filter low + fast serial
	// 6 : M filter mid + fast serial
	// 7 : M filter high + fast serial
	
	SetNR_2D_sFilter(0,0) ; // 2D
	SetNR_2D_sFilter(0,1) ; // 2Dc
	
	SetNR_2D_sFilter(2,2) ; // 2Df		
		
	SetNR_Mittlerfilter(0,0,0) ; // filter disable with gain = 0 & offset = 0 ;
	
	// SetNR_MittlerfilterLevel(1) ; // Mittlerfilter LOW 
	SetNR_MittlerfilterLevel((pCTRL->strategy&0x03)) ; // Set Mittlerfilter 
#endif

	pCTRL->stL2DNR.update_counter = 2;

	SetNR_LetzteresfilterEN(1,1) ;
	SetNR_LetzteresfilterLevel(0) ;
	SetNR_LetzteresfilterY(1024,1) ;
	SetNR_LetzteresfilterC(1024,1,0) ;
	
// end parameter for NR control	
}

static void Init_NR_REGISTER( PNR_CTRL_PARAM pCTRL ) {

	SetCLK_SrcNR(pCTRL->image_type);	

	MISP_RegWrite(MISP_LOCAL_ID, 0x512, 0x0000);	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x513, 0x0000);	// 
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x515, 0x0000);	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x516, 0x0011);	// 
		
	NRCTRL_ImageSizeHandler(pCTRL) ;

	MISP_RegWrite(MISP_LOCAL_ID, 0x519, 0x0000);	// 
	MISP_RegField(MISP_LOCAL_ID, 0x51B, 13, 1 , 0x01 );
		
#if __NR_REV__
	MISP_RegWrite(MISP_LOCAL_ID, 0x518,
		(pCTRL->stWDR.wdr_tm_only_mode<<15)|(pCTRL->nr_10bit<<12)|(pCTRL->stWDR.mo_his_rd_en<<11)|
		(pCTRL->stLINEMEM.c_linemem_en<<9)|(pCTRL->stLINEMEM.y_linemem_en<<8)|(pCTRL->stWDR.mo_flag_rd_en<<6)|(pCTRL->image_type<<5)|
		(pCTRL->nr_10bit<<4)|(pCTRL->stWDR.mo_his_rd_en<<3)|(pCTRL->stLINEMEM.c_linemem_en<<2)|(pCTRL->stLINEMEM.y_linemem_en<<1));
	
	MISP_RegField(MISP_LOCAL_ID, 0x523, 14, 1, 1);	//
	NRCTRL_DataOrderHandler(pCTRL) ;
	MISP_RegWrite(MISP_LOCAL_ID, 0x524, 0xbF80|(pCTRL->stIDM.idm_en<<14)|(pCTRL->stIDM.id_k_value));	// 	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x529, 0x000a);	// 		
  	MISP_RegField(MISP_LOCAL_ID, 0x531, 14, 2, 0x0001);

	MISP_RegWrite(MISP_LOCAL_ID, 0x525, 0x8000|(pCTRL->stIDM.id_dy_noise_limit<<13)|(0x0002<<10)|(pCTRL->stIDM.id_noise_source<<8)|(pCTRL->stIDM.id_noise_limit));
	MISP_RegField(MISP_LOCAL_ID, 0x532, 8, 8, (pCTRL->st2DNR.nf_bypass_level<<4)|(pCTRL->st2DNR.stdev_bypass_level));	// 		
	MISP_RegField(MISP_LOCAL_ID, 0x532, 0, 1, 0x0001);	// 		
	NRCTRL_2D_NBHandler(pCTRL) ;
	MISP_RegField(MISP_LOCAL_ID, 0x534, 14, 2, 0x0003);
	MISP_RegWrite(MISP_LOCAL_ID, 0x53a, 0xA800);	//
		
	MISP_RegWrite(MISP_LOCAL_ID, 0x54A, (pCTRL->stPDM.pd_ll_noise_limit<<8)|(pCTRL->stPDM.pd_pmh_en<<7)|(pCTRL->stPDM.pd_pmh_k00));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x54B, (0x8000)|(pCTRL->stPDM.pd_pmh_k01<<8)|(pCTRL->stPDM.pd_ll_motion_th));	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x54D, (pCTRL->stPDM.pd_pmh_k02<<8)|(pCTRL->stPDM.pd_ll_motion_adj));
	MISP_RegField(MISP_LOCAL_ID, 0x55C, 0, 15, (pCTRL->stSRM.ll_still_move_duration<<9)|(pCTRL->stSRM.ll_still_max_intensity<<6)|(pCTRL->stSRM.ll_still_duration));	// 
	
	// MISP_RegWrite(MISP_LOCAL_ID, 0x528, 0xCF80|(pCTRL->st2DNR.rold_impulse_init));	
	// MISP_RegWrite(MISP_LOCAL_ID, 0x528, 0x5380|(pCTRL->st2DNR.rold_impulse_init));	
		
	MISP_RegWrite(MISP_LOCAL_ID, 0x528, 0x4380);	
	MISP_RegField(MISP_LOCAL_ID, 0x528, 10, 3, (pCTRL->stPDM.pd_pmh_thres));	
	// MISP_RegField(MISP_LOCAL_ID, 0x528, 0, 7, (pCTRL->st2DNR.rold_impulse_init));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x535, (pCTRL->stWDR.r_2D_wdr_mo_mode<<9)|(pCTRL->st2DNR.c_inbound_range));	// 		
		
	MISP_RegWrite(MISP_LOCAL_ID, 0x540, (pCTRL->stPDM.pd_motion_th<<8)|(pCTRL->st3DNR.k_value_mode<<7)|(pCTRL->st3DNR.const_k_value_oth));	

	MISP_RegWrite(MISP_LOCAL_ID, 0x53D, (pCTRL->stPDM.pd_dy_noise_limit<<11)|0xA700);
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x53E, 0x6000|(pCTRL->stPDM.pd_noise_limit));	// 

	#if __NR_L2DNR_CONTROL__	
		MISP_RegWrite(MISP_LOCAL_ID, 0x53F, (0x0078)|(pCTRL->stIDM.c_idm_auto<<7)|(pCTRL->stSRM.srm_auto<<2)|(pCTRL->stPDM.pdm_auto<<1)|(pCTRL->stIDM.idm_auto));	// 	L M on			
	#else		
		MISP_RegWrite(MISP_LOCAL_ID, 0x53F, (0x0058)|(pCTRL->stIDM.c_idm_auto<<7)|(pCTRL->stSRM.srm_auto<<2)|(pCTRL->stPDM.pdm_auto<<1)|(pCTRL->stIDM.idm_auto));	// 	M filter only
	#endif
			
	MISP_RegWrite(MISP_LOCAL_ID, 0x552, (0x0001<<13)|(pCTRL->stSRM.srm_en<<12)|(pCTRL->stSRM.sr_dy_th<<11)|(pCTRL->stSRM.sr_th<<4)|(pCTRL->stSRM.sr_th_fractional));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x55B, (0x0003<<13)|(pCTRL->stSRM.ll_still_en<<12)|(pCTRL->stSRM.ll_still_mblk_cnt_th<<6)|(pCTRL->stSRM.ll_still_frm_th));	//
	
	//======================================================//
	MISP_RegField(MISP_LOCAL_ID, 0x7D3, 0, 1, 0 ) ; // CS block function enable
	//======================================================//
		
	NRCTRL_2D_MFilterHandler(pCTRL) ;
	NRCTRL_2D_sFilterHandler(pCTRL) ;
	
	//======================================================//
	MISP_RegField(MISP_LOCAL_ID, 0x01A, 0, 1, (pCTRL->image_type)) ; // YC NR path setup
	//======================================================//
#else
	MISP_RegWrite(MISP_LOCAL_ID, 0x518,
		(pCTRL->stWDR.wdr_tm_only_mode<<15)|(pCTRL->nr_10bit<<12)|(pCTRL->stWDR.mo_his_rd_en<<11)|
		(pCTRL->stLINEMEM.c_linemem_en<<9)|(pCTRL->stLINEMEM.y_linemem_en<<8)|(pCTRL->image_type<<5)|
		(pCTRL->nr_10bit<<4)|(pCTRL->stLINEMEM.c_linemem_en<<2)|(pCTRL->stLINEMEM.y_linemem_en<<1));	

	// MISP_RegWrite(MISP_LOCAL_ID, 0x523, pCTRL->bayer_channel);	//
	NRCTRL_DataOrderHandler(pCTRL) ;
	MISP_RegWrite(MISP_LOCAL_ID, 0x524, 0xbb80|(pCTRL->stIDM.idm_en<<14)|(pCTRL->stIDM.id_k_value));	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x525, (pCTRL->stIDM.id_dy_noise_limit<<13)|(0x0002<<10)|(pCTRL->stIDM.id_noise_source<<8)|(pCTRL->stIDM.id_noise_limit));	

	MISP_RegWrite(MISP_LOCAL_ID, 0x528, 0x0080);	// NR initial value changed : 2013.01.23 ( jhs ) 	
	MISP_RegWrite(MISP_LOCAL_ID, 0x529, (pCTRL->st2DNR.rold_impulse_init<<4)|0x000a);	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x54C, 0x0000);	//	
	
  	MISP_RegField(MISP_LOCAL_ID, 0x532, 8, 8, (pCTRL->st2DNR.nf_bypass_level<<4)|(pCTRL->st2DNR.stdev_bypass_level));	// 	

	MISP_RegWrite(MISP_LOCAL_ID, 0x535, (pCTRL->stWDR.r_2D_wdr_mo_mode<<9)|(pCTRL->st2DNR.c_inbound_range));	// 	
	
	MISP_RegField(MISP_LOCAL_ID, 0x531, 14, 2, 0x0003);
	NRCTRL_2D_NBHandler(pCTRL) ;
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x540, (pCTRL->stPDM.pd_motion_th<<8));	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x53D, (pCTRL->stPDM.pd_dy_noise_limit<<11)|0x0700);
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x53E, (pCTRL->stPDM.pd_noise_limit));	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x53F, (0x0010)|(pCTRL->stIDM.c_idm_auto<<7)|(pCTRL->stSRM.srm_auto<<2)|(pCTRL->stPDM.pdm_auto<<1)|(pCTRL->stIDM.idm_auto));	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x552, (pCTRL->stSRM.srm_en<<12)|(pCTRL->stSRM.sr_dy_th<<11)|(pCTRL->stSRM.sr_th<<4)|(pCTRL->stSRM.sr_th_fractional));		
	MISP_RegWrite(MISP_LOCAL_ID, 0x55B, (pCTRL->stSRM.ll_still_en<<12)|(pCTRL->stSRM.ll_still_mblk_cnt_th<<6)|(pCTRL->stSRM.ll_still_frm_th));	//
		
	MISP_RegWrite(MISP_LOCAL_ID, 0x54A, (pCTRL->stPDM.pd_ll_noise_limit<<8));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x54B, (pCTRL->stPDM.pd_ll_motion_th));	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x54D, (pCTRL->stPDM.pd_ll_motion_adj));		
	MISP_RegWrite(MISP_LOCAL_ID, 0x55C, (pCTRL->stSRM.ll_still_move_duration<<9)|(pCTRL->stSRM.ll_still_max_intensity<<6)|(pCTRL->stSRM.ll_still_duration));	// 	
#endif	// __NR_REV__
		
	MISP_RegWrite(MISP_LOCAL_ID, 0x526, (pCTRL->stIDM.id_thres_3d_nr<<8)|(pCTRL->stIDM.id_thres_3d_2d_nr));	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x527, (pCTRL->stIDM.id_thres_pre_3d_cu_2d_nr<<8)|(pCTRL->stIDM.id_thres_pre_3d_2d_cu_2d_nr));	// 

	MISP_RegWrite(MISP_LOCAL_ID, 0x521, 0x0500);	// color_suppress_en (control) -> will be removed (jins)
	MISP_RegWrite(MISP_LOCAL_ID, 0x522, 0x37D9);	// color_suppress_en (flag) -> will be removed (jins)
	MISP_RegWrite(MISP_LOCAL_ID, 0x543, 0x32D9);	// color_suppress_en (motion) -> will be removed (jins)

	MISP_RegWrite(MISP_LOCAL_ID, 0x544, (pCTRL->stIDM.id_ll_dy_noise_limit<<14)|(pCTRL->stIDM.id_ll_noise_limit));	

	MISP_RegWrite(MISP_LOCAL_ID, 0x545, (pCTRL->stIDM.id_ll_thres_3d_nr<<8)|(pCTRL->stIDM.id_ll_thres_3d_2d_nr));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x546, (pCTRL->stIDM.id_ll_thres_pre_3d_cu_2d_nr<<8)|(pCTRL->stIDM.id_ll_thres_pre_3d_2d_cu_2d_nr));	//

// jhs 20130621 for Chroma 2D filter ( Y/C mode )
	MISP_RegWrite(MISP_LOCAL_ID, 0x55D, (pCTRL->stIDM.c_idm_en<<15)|0x4020|(pCTRL->stIDM.c_id_k_value<<7)|(pCTRL->stIDM.c_id_thres_ratio));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x55E, (pCTRL->stIDM.c_id_thres_raw_pren<<8)|(pCTRL->stIDM.c_id_thres_c3d_pren));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x55F, (pCTRL->stIDM.c_id_ll_thres_raw_pren<<8)|(pCTRL->stIDM.c_id_ll_thres_c3d_pren));	// 	

	MISP_RegWrite(MISP_LOCAL_ID, 0x52A, (pCTRL->stFRAMENOISE.edge_level<<7)|(pCTRL->st3DNR.const_k_value));	//	

	MISP_RegWrite(MISP_LOCAL_ID, 0x52B, 0x8000|(pCTRL->st3DNR.Adaptive_en<<14)|(pCTRL->st3DNR.k_value_bound00<<7)|(pCTRL->st3DNR.k_value_bound01));	// NR initial value changed : 2013.01.23 ( jhs ) 
	
	NRCTRL_3DintensityHandler(pCTRL) ;	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x52C, (pCTRL->st3DNR.k_value_bound02<<7)|(pCTRL->st3DNR.k_value_bound03));	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x52D, (pCTRL->st3DNR.k_value_bound04));	// 

	MISP_RegWrite(MISP_LOCAL_ID, 0x536, 0x100A) ;		
	
	NRCTRL_ModeHandler(pCTRL) ;
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x538, 0x0918);	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x539, 0x06ff);	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x54E, 0x051f);	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x54F, 0x0005);	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x550, 0x0776);	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x551, 0x0433);	//
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x53B, (pCTRL->stPDM.pdm_en<<13)|0x1900|(pCTRL->stPDM.pdm_noise_source<<6)|(pCTRL->stPDM.pd_sads_ratio));
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x53C, (pCTRL->stPDM.pd_motion_k<<7)|(pCTRL->stPDM.pd_nutral_motion_k));	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x541, (pCTRL->stPDM.pd_motion_adj));	// 
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x542, (pCTRL->stLL.ll_en<<12)|(pCTRL->stLL.ll_enter_th<<6)|(pCTRL->stLL.ll_exit_th));
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x547, 3<<14|(pCTRL->stPDM.pd_ll_motion_k<<7)|(pCTRL->stPDM.pd_ll_nutral_motion_k)); 
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x548, (pCTRL->stPDM.pd_ll_dy_noise_limit<<8)|0x0180|(pCTRL->stPDM.pd_ll_sads_ratio));	
	MISP_RegWrite(MISP_LOCAL_ID, 0x549, (pCTRL->stFRAMENOISE.dark_upper_bound<<8)|pCTRL->stFRAMENOISE.bright_lower_bound);	// 	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x556, ((pCTRL->stLL.ent_level<<7)) | pCTRL->stLL.exit_level);
	MISP_RegWrite(MISP_LOCAL_ID, 0x557, ((pCTRL->stLL.ent_level_fr<<3)) | pCTRL->stLL.exit_level_fr);

	MISP_RegWrite(MISP_LOCAL_ID, 0x554, (pCTRL->stSRM.sr_ll_dy_th<<11)|(pCTRL->stSRM.sr_ll_th<<4)|(pCTRL->stSRM.sr_ll_th_fractional));
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x555, 0x8000|(pCTRL->stPDM.sr_replace_c<<13)|(pCTRL->stPDM.sr_replace<<11)|(pCTRL->stSRM.sr_bmi_corr_en<<10)|(pCTRL->stSRM.sr_mi_corr_en<<9)|(pCTRL->stSRM.sr_mi_corr_level<<6)|(pCTRL->stSRM.sr_mi_corr_value<<3)|(pCTRL->stSRM.sr_ll_mi_corr_value));	// NR inital value changed : 2012.11.27 ( jhs )

	NRCTRL_2D_LFilterHandler(pCTRL);
}

void Init_NR(void) {
	
	memset(&stNRCTRL, 0, sizeof(NR_CTRL_PARAM));
		
	stNRCTRL.ctrl |= (NRCTRL_3DINTENSITY_UPDATE|NRCTRL_MODE_UPDATE|NRCTRL_IMAGE_SIZE_UPDATE);

	Init_NR_PARAM(&stNRCTRL);
	Init_NR_REGISTER(&stNRCTRL);

#if __NR_SPLIT__==0	
	stNRCTRL.ctrl = 0 ;
#endif
	
}

//--------------------------------------------------------------------------------------------------------------------------
static void NRCTRL_DemoModeHandler(PNR_CTRL_PARAM pCTRL)
{
	WORD	nr_debug_en=0, nr_debug_sel=9;
	WORD	nr_skip_x_st=0, nr_skip_x_end=0, nr_skip_y_st=0, nr_skip_y_end=0 ;
	WORD	in_hsize=0, in_vsize=0 ;
	
	BYTE	lower_bound = 0 , upper_bound = 0  ;
	BYTE	nutral_K ;
	
	if ((pCTRL->ctrl&NRCTRL_DEMOMODE_UPDATE)==0) return;
	pCTRL->ctrl &= (~NRCTRL_DEMOMODE_UPDATE);
	
	in_hsize = pCTRL->stLINEMEM.active_h_size + (pCTRL->stLINEMEM.bH<<1) ;
	in_vsize = pCTRL->stLINEMEM.active_v_size + (pCTRL->stLINEMEM.bV<<1) ;	
	
	nr_skip_x_end = in_hsize>>1 ;
	nr_skip_y_end = in_vsize ;

	if ( pCTRL->lowlight_mode == 0 )
		nutral_K = pCTRL->stPDM.pd_nutral_motion_k ;
	else
		nutral_K = pCTRL->stPDM.pd_ll_nutral_motion_k ;	
		
	switch(pCTRL->demo_mode) {	//NR Demo mode
		case MISP_NRDEMO_OFF :	nr_debug_en=0;	break;
		case MISP_NRDEMO_LEFT_ON :	nr_debug_en=1;	nr_debug_sel=9;	nr_skip_x_st = in_hsize>>1;	nr_skip_x_end = in_hsize;	break;
		case MISP_NRDEMO_RIGHT_ON :	nr_debug_en=1;	nr_debug_sel=9;	nr_skip_x_st = 0; nr_skip_x_end = in_hsize>>1;	break;
		case MISP_NRDEMO_IDM_L :	nr_debug_en=1;	nr_debug_sel=0;	lower_bound = 1; upper_bound = 4; nr_skip_x_st = 0; nr_skip_x_end = 0;	break;
		case MISP_NRDEMO_IDM_H :	nr_debug_en=1;	nr_debug_sel=0;	lower_bound = 5; upper_bound = 8; nr_skip_x_st = 0; nr_skip_x_end = 0;	break;
		case MISP_NRDEMO_PDM_U :	nr_debug_en=1;	nr_debug_sel=8;	lower_bound = nutral_K; upper_bound = nutral_K; nr_skip_x_st = 0; nr_skip_x_end = 0;		break;
		case MISP_NRDEMO_PDM :	nr_debug_en=1;	nr_debug_sel=1;	lower_bound = 1; upper_bound = 8; nr_skip_x_st = 0; nr_skip_x_end = 0;		break;
		case MISP_NRDEMO_SRM :	nr_debug_en=1;	nr_debug_sel=10;	lower_bound = 0; upper_bound = 0; nr_skip_x_st = 0; nr_skip_x_end = 0;	break;
	}	
	
	MISP_RegField(MISP_LOCAL_ID, 0x538, 8, 4, nr_debug_sel);
	MISP_RegField(MISP_LOCAL_ID, 0x538, 14, 1, nr_debug_en);
	MISP_RegField(MISP_LOCAL_ID, 0x538 , 4 ,4 , lower_bound ) ;
	MISP_RegField(MISP_LOCAL_ID, 0x538 , 0 ,4 , upper_bound ) ;	
	MISP_RegField(MISP_LOCAL_ID, 0x539 , 8 ,8 , lower_bound ) ;
	MISP_RegField(MISP_LOCAL_ID, 0x539 , 0 ,8 , upper_bound ) ;	
	MISP_RegField(MISP_LOCAL_ID, 0x54E, 0, 13, nr_skip_x_st);
	MISP_RegField(MISP_LOCAL_ID, 0x550, 0, 13, nr_skip_x_end);
	MISP_RegField(MISP_LOCAL_ID, 0x54F, 0, 13, nr_skip_y_st);
	MISP_RegField(MISP_LOCAL_ID, 0x551, 0, 13, nr_skip_y_end);
}
//--------------------------------------------------------------------------------------------------------------------------
static void NRCTRL_LowLightLevelHandler(PNR_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&NRCTRL_FRMNOISE_UPDATE)!=0) {
		pCTRL->ctrl &= (~NRCTRL_FRMNOISE_UPDATE);
		
		MISP_RegWrite(MISP_LOCAL_ID, 0x549, (pCTRL->stFRAMENOISE.dark_upper_bound<<8)|pCTRL->stFRAMENOISE.bright_lower_bound);
	}	

	if ((pCTRL->ctrl&NRCTRL_LOWLIGHTLEVEL_UPDATE)==0) return;
	pCTRL->ctrl &= (~NRCTRL_LOWLIGHTLEVEL_UPDATE);
	
	MISP_RegField(MISP_LOCAL_ID, 0x556, 0, 14, ((pCTRL->stLL.ent_level<<7)&0x3f80) | pCTRL->stLL.exit_level);
	MISP_RegField(MISP_LOCAL_ID, 0x557, 0, 6, ((pCTRL->stLL.ent_level_fr<<3)&0x38) | pCTRL->stLL.exit_level_fr);	
}
//--------------------------------------------------------------------------------------------------------------------------
static void NRCTRL_3DintensityHandler(PNR_CTRL_PARAM pCTRL)
{
	// BYTE	NR3D_K_val;
	BYTE	NR3D_Adaptive_EN;
	
	BYTE	value_00, value_01, value_02, value_03 ,value_04, value_05 ;
	
	if ((pCTRL->ctrl&NRCTRL_3DINTENSITY_UPDATE)==0) return;
	pCTRL->ctrl &= (~NRCTRL_3DINTENSITY_UPDATE);

#if defined(__USE_CMOS_IMX226__)||defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__)	// 4K sensors	
	switch(pCTRL->intensity)	{	//NR 3D intensity
		case  MISP_NR3D_AUTO :	NR3D_Adaptive_EN = 0; value_00 = __NR_AUTO3DNR_VALUE_0__ ; value_01 = __NR_AUTO3DNR_VALUE_1__ ; value_02 = __NR_AUTO3DNR_VALUE_2__ ; 
														value_03 = __NR_AUTO3DNR_VALUE_4__ ; value_04 = __NR_AUTO3DNR_VALUE_6__ ; value_05 = __NR_AUTO3DNR_VALUE_8__ ; break;
		case  MISP_NR3D_LOW  :	NR3D_Adaptive_EN = 0; value_00 = __NR_AUTO3DNR_VALUE_5__ ; value_01 = __NR_AUTO3DNR_VALUE_6__ ; value_02 = __NR_AUTO3DNR_VALUE_7__ ; 
														value_03 = __NR_AUTO3DNR_VALUE_8__ ; value_04 = __NR_AUTO3DNR_VALUE_9__ ; value_05 = __NR_AUTO3DNR_VALUE_A__ ; break;
		case  MISP_NR3D_MID  :	NR3D_Adaptive_EN = 0; value_00 = __NR_AUTO3DNR_VALUE_3__ ; value_01 = __NR_AUTO3DNR_VALUE_4__ ; value_02 = __NR_AUTO3DNR_VALUE_5__ ; 
														value_03 = __NR_AUTO3DNR_VALUE_6__ ; value_04 = __NR_AUTO3DNR_VALUE_7__ ; value_05 = __NR_AUTO3DNR_VALUE_8__ ; break;
		case  MISP_NR3D_HIGH :	NR3D_Adaptive_EN = 0; value_00 = __NR_AUTO3DNR_VALUE_0__ ; value_01 = __NR_AUTO3DNR_VALUE_1__ ; value_02 = __NR_AUTO3DNR_VALUE_2__ ; 
														value_03 = __NR_AUTO3DNR_VALUE_3__ ; value_04 = __NR_AUTO3DNR_VALUE_4__ ; value_05 = __NR_AUTO3DNR_VALUE_5__ ;  break;
		default			 :	NR3D_Adaptive_EN = 1; value_00 = __NR_AUTO3DNR_VALUE_0__ ; value_01 = __NR_AUTO3DNR_VALUE_1__ ; value_02 = __NR_AUTO3DNR_VALUE_2__ ; 
														value_03 = __NR_AUTO3DNR_VALUE_3__ ; value_04 = __NR_AUTO3DNR_VALUE_4__ ; value_05 = __NR_AUTO3DNR_VALUE_5__ ; break;	 //RTL auto
	}
#else // 2K sensors
	switch(pCTRL->intensity)	{	//NR 3D intensity
		case  MISP_NR3D_AUTO :	NR3D_Adaptive_EN = 0; value_00 = __NR_AUTO3DNR_VALUE_0__ ; value_01 = __NR_AUTO3DNR_VALUE_1__ ; value_02 = __NR_AUTO3DNR_VALUE_2__ ; 
														value_03 = __NR_AUTO3DNR_VALUE_4__ ; value_04 = __NR_AUTO3DNR_VALUE_5__ ; value_05 = __NR_AUTO3DNR_VALUE_6__ ; break;
		case  MISP_NR3D_LOW  :	NR3D_Adaptive_EN = 0; value_00 = __NR_AUTO3DNR_VALUE_5__ ; value_01 = __NR_AUTO3DNR_VALUE_6__ ; value_02 = __NR_AUTO3DNR_VALUE_7__ ; 
														value_03 = __NR_AUTO3DNR_VALUE_8__ ; value_04 = __NR_AUTO3DNR_VALUE_9__ ; value_05 = __NR_AUTO3DNR_VALUE_A__ ; break;
		case  MISP_NR3D_MID  :	NR3D_Adaptive_EN = 0; value_00 = __NR_AUTO3DNR_VALUE_3__ ; value_01 = __NR_AUTO3DNR_VALUE_4__ ; value_02 = __NR_AUTO3DNR_VALUE_5__ ; 
														value_03 = __NR_AUTO3DNR_VALUE_6__ ; value_04 = __NR_AUTO3DNR_VALUE_7__ ; value_05 = __NR_AUTO3DNR_VALUE_8__ ; break;
		case  MISP_NR3D_HIGH :	NR3D_Adaptive_EN = 0; value_00 = __NR_AUTO3DNR_VALUE_0__ ; value_01 = __NR_AUTO3DNR_VALUE_1__ ; value_02 = __NR_AUTO3DNR_VALUE_2__ ; 
														value_03 = __NR_AUTO3DNR_VALUE_3__ ; value_04 = __NR_AUTO3DNR_VALUE_4__ ; value_05 = __NR_AUTO3DNR_VALUE_5__ ;  break;
		default			 :	NR3D_Adaptive_EN = 1; value_00 = __NR_AUTO3DNR_VALUE_0__ ; value_01 = __NR_AUTO3DNR_VALUE_1__ ; value_02 = __NR_AUTO3DNR_VALUE_2__ ; 
														value_03 = __NR_AUTO3DNR_VALUE_3__ ; value_04 = __NR_AUTO3DNR_VALUE_4__ ; value_05 = __NR_AUTO3DNR_VALUE_5__ ; break;	 //RTL auto
	}	
#endif
	// NR3D_K_val = pCTRL->st3DNR.const_k_value ;
	
	pCTRL->st3DNR.Adaptive_en = NR3D_Adaptive_EN ;
	pCTRL->st3DNR.k_value_00 = value_00 ;
	pCTRL->st3DNR.k_value_01 = value_01 ;
	pCTRL->st3DNR.k_value_02 = value_02 ;
	pCTRL->st3DNR.k_value_03 = value_03 ;
	pCTRL->st3DNR.k_value_04 = value_04 ;
	pCTRL->st3DNR.k_value_05 = value_05 ;
	
	MISP_RegField(MISP_LOCAL_ID, 0x52B, 14, 1, pCTRL->st3DNR.Adaptive_en);	//0:Constant, 1:Adaptive 3D NR
	MISP_RegField(MISP_LOCAL_ID, 0x52A, 0,  7, pCTRL->st3DNR.const_k_value); 	//K for constant 3D NR
	MISP_RegField(MISP_LOCAL_ID, 0x52E, 0, 14, (pCTRL->st3DNR.k_value_00<<7)|(pCTRL->st3DNR.k_value_01));	// 
	MISP_RegField(MISP_LOCAL_ID, 0x52F, 0, 14, (pCTRL->st3DNR.k_value_02<<7)|(pCTRL->st3DNR.k_value_03));	// 
	MISP_RegField(MISP_LOCAL_ID, 0x530, 0, 14, (pCTRL->st3DNR.k_value_04<<7)|(pCTRL->st3DNR.k_value_05));	// 	
	
#if __NR_REV__
	if ( pCTRL->st3DNR.k_value_mode == 1 )
	{
		pCTRL->st3DNR.k_value_oth_00 = MAX(__NR_AUTO3DNR_VALUE_0__,(value_00 - 5)) ; // All k_value can not be smaller then __NR_AUTO3DNR_VALUE_0__
		pCTRL->st3DNR.k_value_oth_01 = MAX(__NR_AUTO3DNR_VALUE_0__,(value_01 - 5)) ; // All k_value can not be smaller then __NR_AUTO3DNR_VALUE_0__
		pCTRL->st3DNR.k_value_oth_02 = MAX(__NR_AUTO3DNR_VALUE_0__,(value_02 - 5)) ; // All k_value can not be smaller then __NR_AUTO3DNR_VALUE_0__
		pCTRL->st3DNR.k_value_oth_03 = MAX(__NR_AUTO3DNR_VALUE_0__,(value_03 - 5)) ; // All k_value can not be smaller then __NR_AUTO3DNR_VALUE_0__
		pCTRL->st3DNR.k_value_oth_04 = MAX(__NR_AUTO3DNR_VALUE_0__,(value_04 - 5)) ; // All k_value can not be smaller then __NR_AUTO3DNR_VALUE_0__
		pCTRL->st3DNR.k_value_oth_05 = MAX(__NR_AUTO3DNR_VALUE_0__,(value_05 - 5)) ; // All k_value can not be smaller then __NR_AUTO3DNR_VALUE_0__
	}
	else
	{
		pCTRL->st3DNR.k_value_oth_00 = value_00 ;
		pCTRL->st3DNR.k_value_oth_01 = value_01 ;
		pCTRL->st3DNR.k_value_oth_02 = value_02 ; 
		pCTRL->st3DNR.k_value_oth_03 = value_03 ; 
		pCTRL->st3DNR.k_value_oth_04 = value_04 ; 
		pCTRL->st3DNR.k_value_oth_05 = value_05 ;
	}
	MISP_RegField(MISP_LOCAL_ID, 0x540, 0, 7, (pCTRL->st3DNR.const_k_value_oth));	// 
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void NRCTRL_ModeHandler(PNR_CTRL_PARAM pCTRL)
{
	BYTE c_3Dfilter ;
	
	if ((pCTRL->ctrl&NRCTRL_MODE_UPDATE)==0) return;
	pCTRL->ctrl &= (~NRCTRL_MODE_UPDATE);
	
	if (pCTRL->image_type == MISP_NR_BAYER_IMAGE ){
		switch(pCTRL->mode)	{	//NR Bayer mode
			case MISP_NR_2D:	MISP_RegField(MISP_LOCAL_ID, 0x536, 6, 4, (2<<2)|0x0);	break;	//2d on
			case MISP_NR_3D:	MISP_RegField(MISP_LOCAL_ID, 0x536, 6, 4, (1<<2)|0x0);	break;	//3d on
			case MISP_NR_2D3D:	MISP_RegField(MISP_LOCAL_ID, 0x536, 6, 4, (pCTRL->mode<<2)|0x0);	break; //2d&3d on			
			default:			MISP_RegField(MISP_LOCAL_ID, 0x536, 6, 4, 0x0);	break; //off			
		}
	}
	else
	{	
		c_3Dfilter = pCTRL->stLINEMEM.c_linemem_en ;
		
		switch(pCTRL->mode)	{	//NR Y/C mode
			case MISP_NR_2D:	MISP_RegField(MISP_LOCAL_ID, 0x536, 6, 4, (2<<2)|2);	break;	//2d on
			case MISP_NR_3D:	MISP_RegField(MISP_LOCAL_ID, 0x536, 6, 4, (1<<2)|c_3Dfilter);	break;	//3d on
			case MISP_NR_2D3D:	MISP_RegField(MISP_LOCAL_ID, 0x536, 6, 4, (pCTRL->mode<<2)|2|c_3Dfilter);	break; //2d&3d on/off
			default:			MISP_RegField(MISP_LOCAL_ID, 0x536, 6, 4, 0x0);	break; //2d&3d on/off
		}	
	}
}
//--------------------------------------------------------------------------------------------------------------------------
static void NRCTRL_ImageSizeHandler(PNR_CTRL_PARAM pCTRL)
{

	WORD in_hsize ;
	WORD in_vsize ;
	WORD col_per_line ;
	WORD count_value ;		
	WORD divide_factor ;		
	
	if ((pCTRL->ctrl&NRCTRL_IMAGE_SIZE_UPDATE)==0) return;
	pCTRL->ctrl &= (~NRCTRL_IMAGE_SIZE_UPDATE);

	in_hsize = pCTRL->stLINEMEM.active_h_size + (pCTRL->stLINEMEM.bH<<1) ;
	in_vsize = pCTRL->stLINEMEM.active_v_size + (pCTRL->stLINEMEM.bV<<1) ;
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x510, in_hsize);	// in_hsize 1928
	MISP_RegWrite(MISP_LOCAL_ID, 0x511, in_vsize);	// in_hsize 1088	

	NRCTRL_SetCustomBlockSize(pCTRL) ;
	MISP_RegWrite(MISP_LOCAL_ID, 0x553, (pCTRL->stSRM.block_h_size<<7)|pCTRL->stSRM.block_v_size );	// SBM block size 	
		
#if __NR_REV__
	MISP_RegWrite(MISP_LOCAL_ID, 0x51A, (pCTRL->stLINEMEM.bH<<8)|pCTRL->stLINEMEM.bV); // 
	MISP_RegField(MISP_LOCAL_ID, 0x51B, 0, 13 , pCTRL->stLINEMEM.active_h_size );	// internal_hsize 1920
	MISP_RegField(MISP_LOCAL_ID, 0x51C, 0, 13 , pCTRL->stLINEMEM.active_v_size );	// internal_vsize 1080	
#else
	MISP_RegWrite(MISP_LOCAL_ID, 0x51A, (pCTRL->stLINEMEM.bH<<5)|pCTRL->stLINEMEM.bV); // 
	MISP_RegWrite(MISP_LOCAL_ID, 0x51B, pCTRL->stLINEMEM.active_h_size );	// internal_hsize 1920
	MISP_RegWrite(MISP_LOCAL_ID, 0x51C, pCTRL->stLINEMEM.active_v_size );	// internal_vsize 1080
#endif
	

	if (pCTRL->nr_10bit==0) divide_factor = 8 ;
	else divide_factor = 6 ;
	
	if ( (in_hsize)%divide_factor == 0 ) col_per_line = (in_hsize)/divide_factor ;
	else col_per_line = (in_hsize)/divide_factor + 1 ;
	
//	count_value = col_per_line%32 -1 ;
//	when col_per_line is multiple of 32, count_value shall be 31.
	if (col_per_line%32) count_value = col_per_line%32 -1 ;
	else count_value = 31;
	
	// if ( in_hsize <= 768 ) MISP_RegField(MISP_LOCAL_ID, 0x517, 12,4,11);	//	 for slow video clocks
	if ( in_hsize <= 768 ) MISP_RegField(MISP_LOCAL_ID, 0x517, 12,4,15);	//	 for slow video clocks
	else MISP_RegField(MISP_LOCAL_ID, 0x517, 12,4,0);	//		
	
	MISP_RegField(MISP_LOCAL_ID, 0x517, 0, 12, 3<<10|(count_value<<5));	//		
	MISP_RegField(MISP_LOCAL_ID, 0x518, 12 , 1, pCTRL->nr_10bit );	//		
	MISP_RegField(MISP_LOCAL_ID, 0x518, 4 , 1, pCTRL->nr_10bit );	//	
}
//--------------------------------------------------------------------------------------------------------------------------

static void NRCTRL_SRMHandler(PNR_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&NRCTRL_SRM_UPDATE)==0) return;
	pCTRL->ctrl &= (~NRCTRL_SRM_UPDATE);
	
	if ( pCTRL->lowlight_mode == 0 )
	{
#if __NR_SPLIT__==0		
		MISP_RegField(MISP_LOCAL_ID, 0x552, 0, 12, (pCTRL->stSRM.sr_dy_th<<11)|(pCTRL->stSRM.sr_th<<4)|(pCTRL->stSRM.sr_th_fractional));	
#else
		MISP_RegField(MISP_LOCAL_ID, 0x552, 0, 12, (pCTRL->stSRM.sr_th<<4)|(pCTRL->stSRM.sr_th_fractional));	// No Dy_noise limit when NR split mode
#endif
	}
	else
	{
#if __NR_SPLIT__==0				
		MISP_RegWrite(MISP_LOCAL_ID, 0x554, (pCTRL->stSRM.sr_ll_dy_th<<11)|(pCTRL->stSRM.sr_ll_th<<4)|(pCTRL->stSRM.sr_ll_th_fractional));
#else
		MISP_RegWrite(MISP_LOCAL_ID, 0x554, (pCTRL->stSRM.sr_ll_th<<4)|(pCTRL->stSRM.sr_ll_th_fractional)); // No Dy_noise limit when NR split mode
#endif		
	}
	
	MISP_RegField(MISP_LOCAL_ID, 0x555, 0, 11, (pCTRL->stSRM.sr_bmi_corr_en<<10)|(pCTRL->stSRM.sr_mi_corr_en<<9)|(pCTRL->stSRM.sr_mi_corr_level<<6)|(pCTRL->stSRM.sr_mi_corr_value<<3)|(pCTRL->stSRM.sr_ll_mi_corr_value));
	
}

//--------------------------------------------------------------------------------------------------------------------------

static void NRCTRL_IDMHandler(PNR_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&NRCTRL_IDM_UPDATE)==0) return;
	pCTRL->ctrl &= (~NRCTRL_IDM_UPDATE);
	
	if ( pCTRL->lowlight_mode == 0 )
	{
		MISP_RegField(MISP_LOCAL_ID, 0x525, 0, 8, (pCTRL->stIDM.id_noise_limit));	
#if __NR_SPLIT__==0
		MISP_RegField(MISP_LOCAL_ID, 0x525, 13, 1, (pCTRL->stIDM.id_dy_noise_limit));
#else
		MISP_RegField(MISP_LOCAL_ID, 0x525, 13, 1, (0)); // No Dy_noise limit when NR split mode
#endif
		MISP_RegWrite(MISP_LOCAL_ID, 0x526, (pCTRL->stIDM.id_thres_3d_nr<<8)|(pCTRL->stIDM.id_thres_3d_2d_nr));	// 
		MISP_RegWrite(MISP_LOCAL_ID, 0x527, (pCTRL->stIDM.id_thres_pre_3d_cu_2d_nr<<8)|(pCTRL->stIDM.id_thres_pre_3d_2d_cu_2d_nr));	// 		
	}
	else
	{
#if __NR_SPLIT__==0		
		MISP_RegWrite(MISP_LOCAL_ID, 0x544, (pCTRL->stIDM.id_ll_dy_noise_limit<<14)|(pCTRL->stIDM.id_ll_noise_limit));	
#else
		MISP_RegWrite(MISP_LOCAL_ID, 0x544, (pCTRL->stIDM.id_ll_noise_limit));	 // No Dy_noise limit when NR split mode
#endif		
		MISP_RegWrite(MISP_LOCAL_ID, 0x545, (pCTRL->stIDM.id_ll_thres_3d_nr<<8)|(pCTRL->stIDM.id_ll_thres_3d_2d_nr));	//
		MISP_RegWrite(MISP_LOCAL_ID, 0x546, (pCTRL->stIDM.id_ll_thres_pre_3d_cu_2d_nr<<8)|(pCTRL->stIDM.id_ll_thres_pre_3d_2d_cu_2d_nr));	//			
	}
	
	MISP_RegField(MISP_LOCAL_ID, 0x523, 1, 10, (pCTRL->stIDM.id_t_ratio<<7)|(pCTRL->stIDM.id_y_ratio<<4)|(pCTRL->stIDM.id_b_ratio<<1));		
}
//--------------------------------------------------------------------------------------------------------------------------
static void NRCTRL_IDMcHandler(PNR_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&NRCTRL_IDMc_UPDATE)==0) return;
	pCTRL->ctrl &= (~NRCTRL_IDMc_UPDATE);

	MISP_RegField(MISP_LOCAL_ID, 0x535, 0, 8,(pCTRL->st2DNR.c_inbound_range));	// 
		
	MISP_RegWrite(MISP_LOCAL_ID, 0x55E, (pCTRL->stIDM.idc_thres_0<<8)|(pCTRL->stIDM.idc_thres_1));	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x55F, (pCTRL->stIDM.idc_ll_thres_0<<8)|(pCTRL->stIDM.idc_ll_thres_1));	//		
}
//--------------------------------------------------------------------------------------------------------------------------

static void NRCTRL_PDMHandler(PNR_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&NRCTRL_PDM_UPDATE)==0) return;
	pCTRL->ctrl &= (~NRCTRL_PDM_UPDATE);
	
	if( pCTRL->lowlight_mode == 0 )
	{
		MISP_RegField(MISP_LOCAL_ID, 0x53B, 0, 6, (pCTRL->stPDM.pd_sads_ratio));
		
		MISP_RegWrite(MISP_LOCAL_ID, 0x53C, (pCTRL->stPDM.pd_motion_k<<7)|(pCTRL->stPDM.pd_nutral_motion_k));	// 

#if __NR_SPLIT__==0			
		MISP_RegField(MISP_LOCAL_ID, 0x53D, 11, 1, (pCTRL->stPDM.pd_dy_noise_limit));
#else
		MISP_RegField(MISP_LOCAL_ID, 0x53D, 11, 1, 0); // No Dy_noise limit when NR split mode
#endif		
		MISP_RegField(MISP_LOCAL_ID, 0x53E, 0, 8, (pCTRL->stPDM.pd_noise_limit));	// 

		MISP_RegField(MISP_LOCAL_ID, 0x540, 8, 8, (pCTRL->stPDM.pd_motion_th));	// 
		MISP_RegWrite(MISP_LOCAL_ID, 0x541, (pCTRL->stPDM.pd_motion_adj));	//
	}
	else
	{
		MISP_RegField(MISP_LOCAL_ID, 0x547, 0, 14, (pCTRL->stPDM.pd_ll_motion_k<<7)|(pCTRL->stPDM.pd_ll_nutral_motion_k)); 
#if __NR_SPLIT__==0				
		MISP_RegWrite(MISP_LOCAL_ID, 0x548, (pCTRL->stPDM.pd_ll_dy_noise_limit<<8)|0x0180|(pCTRL->stPDM.pd_ll_sads_ratio));	
#else
		MISP_RegWrite(MISP_LOCAL_ID, 0x548, 0x0180|(pCTRL->stPDM.pd_ll_sads_ratio)); // No Dy_noise limit when NR split mode
#endif
		MISP_RegField(MISP_LOCAL_ID, 0x54A, 8, 8, (pCTRL->stPDM.pd_ll_noise_limit));	
		MISP_RegField(MISP_LOCAL_ID, 0x54B, 0, 8, (pCTRL->stPDM.pd_ll_motion_th));	
		
		MISP_RegField(MISP_LOCAL_ID, 0x54D, 0, 8, (pCTRL->stPDM.pd_ll_motion_adj));		
	}	

#if __NR_REV__
	MISP_RegField(MISP_LOCAL_ID, 0x555, 11 , 4, (pCTRL->stPDM.sr_replace_c<<2)|(pCTRL->stPDM.sr_replace));	
		
	if ( pCTRL->stPDM.pd_pmh_en == 1 )
	{
		MISP_RegField(MISP_LOCAL_ID, 0x528, 10, 3, (pCTRL->stPDM.pd_pmh_thres));
		MISP_RegField(MISP_LOCAL_ID, 0x54A, 0 , 7, (pCTRL->stPDM.pd_pmh_k00));	
		MISP_RegField(MISP_LOCAL_ID, 0x54B, 8 , 7, (pCTRL->stPDM.pd_pmh_k01));	
		MISP_RegField(MISP_LOCAL_ID, 0x54D, 8 , 7, (pCTRL->stPDM.pd_pmh_k02));	
	}
	
	MISP_RegField(MISP_LOCAL_ID, 0x54A, 7, 1, (pCTRL->stPDM.pd_pmh_en)) ;
#endif	
	
}

//--------------------------------------------------------------------------------------------------------------------------
static void NRCTRL_MCUDriveHandler(PNR_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&NRCTRL_MCU_DRIVE_UPDATE)==0) return;
	pCTRL->ctrl &= (~NRCTRL_MCU_DRIVE_UPDATE);	
		
	MISP_RegField(MISP_LOCAL_ID, 0x51E, 0, 1, pCTRL->stMCUDRIVE.mcu_mode_ctrl);
	MISP_RegField(MISP_LOCAL_ID, 0x51F, 0, 1, pCTRL->stMCUDRIVE.mcu_force_lowlight_mode);
	MISP_RegField(MISP_LOCAL_ID, 0x520, 0, 4, (pCTRL->stMCUDRIVE.mcu_force_still_mode<<3)|(pCTRL->stMCUDRIVE.mcu_still_intensity));	
}
//--------------------------------------------------------------------------------------------------------------------------
static void NRCTRL_MCUDriveReturn(PNR_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&NRCTRL_MCU_DRIVE_RETURN)==0) return;
		
	if ( pCTRL->stMCUDRIVE.mcu_drive_duration == 0 ){
		SetNR_MCUDriveMode(0,0,0,0,0) ;
		pCTRL->ctrl &= (~NRCTRL_MCU_DRIVE_RETURN);	
	}
	else{
		pCTRL->stMCUDRIVE.mcu_drive_duration-- ;	
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void NRCTRL_InputImageTypeHandler(PNR_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&NRCTRL_IMAGE_TYPE_UPDATE)==0) return;
	pCTRL->ctrl &= (~NRCTRL_IMAGE_TYPE_UPDATE);	
		
	MISP_RegField(MISP_LOCAL_ID, 0x518, 5, 1, (pCTRL->image_type)) ;
	MISP_RegField(MISP_LOCAL_ID, 0x01A, 0, 1, (pCTRL->image_type)) ;	
}

//--------------------------------------------------------------------------------------------------------------------------

static void NRCTRL_DataOrderHandler ( PNR_CTRL_PARAM pCTRL )
{
	if ((pCTRL->ctrl&NRCTRL_DATA_ORDER_UPDATE)==0) return;
	pCTRL->ctrl &= (~NRCTRL_DATA_ORDER_UPDATE);	
	
	MISP_RegField(MISP_LOCAL_ID, 0x523, 0, 1, (pCTRL->bayer_channel));		
}
//--------------------------------------------------------------------------------------------------------------------------
static void NRCTRL_LinememHandler ( PNR_CTRL_PARAM pCTRL )
{
	if ((pCTRL->ctrl&NRCTRL_LINEMEM_UPDATE)==0) return;
	
	if ( pCTRL->stLINEMEM.active_counter == __NR_ACTIVE_COUNTER__ )
	{
		if ( pCTRL->mode == MISP_NR_3D || pCTRL->mode == MISP_NR_2D3D )
		// if ( pCTRL->mode != MISP_NR_OFF )
		{
			if (pCTRL->image_type == MISP_NR_BAYER_IMAGE)
			{
				pCTRL->stLINEMEM.y_linemem_en = 1 ;
				pCTRL->stLINEMEM.c_linemem_en = 0 ;
			}
			else
			{
				pCTRL->stLINEMEM.y_linemem_en = 1 ;
				if ( pCTRL->stLINEMEM.active_h_size <= 2048 ) pCTRL->stLINEMEM.c_linemem_en = 1 ;		
				else pCTRL->stLINEMEM.c_linemem_en = 0 ;				
			}
			pCTRL->stLINEMEM.active_counter-- ;
		}
		else // OFF
		{
			pCTRL->stLINEMEM.y_linemem_en = 0 ;
			pCTRL->stLINEMEM.c_linemem_en = 0 ;	
			
			pCTRL->stLINEMEM.active_counter = __NR_ACTIVE_COUNTER__ ;
			pCTRL->ctrl &= (~NRCTRL_LINEMEM_UPDATE);
			pCTRL->ctrl |= (NRCTRL_MODE_UPDATE | NRCTRL_TRIGGER_ISP);
		}
				
		MISP_RegField(MISP_LOCAL_ID, 0x518, 1,2, (pCTRL->stLINEMEM.c_linemem_en<<1)|pCTRL->stLINEMEM.y_linemem_en);
		MISP_RegField(MISP_LOCAL_ID, 0x518, 8,2, (pCTRL->stLINEMEM.c_linemem_en<<1)|pCTRL->stLINEMEM.y_linemem_en);
		
#if __NR_REV__	
		if ( pCTRL->stWDR.r_wdr_mo_en == 0 )
		{
			MISP_RegField(MISP_LOCAL_ID, 0x518, 3,1, (pCTRL->stIDM.idm_hist_en));
			MISP_RegField(MISP_LOCAL_ID, 0x518, 11,1, (pCTRL->stIDM.idm_hist_en));
			
			// if ( pCTRL->stIDM.idm_hist_en == 0 )
			// {
				// pCTRL->stLINEMEM.active_counter = __NR_ACTIVE_COUNTER__ ;
				// pCTRL->ctrl &= (~NRCTRL_LINEMEM_UPDATE);
				// pCTRL->ctrl |= (NRCTRL_MODE_UPDATE | NRCTRL_TRIGGER_ISP);					
			// }
		}		
#endif		
	}
	else
	{
		if ( pCTRL->stLINEMEM.active_counter == 0 )
		{
			pCTRL->stLINEMEM.active_counter = __NR_ACTIVE_COUNTER__ ;
			pCTRL->ctrl &= (~NRCTRL_LINEMEM_UPDATE);
			pCTRL->ctrl |= (NRCTRL_MODE_UPDATE | NRCTRL_TRIGGER_ISP);
		}
		else
		{
			pCTRL->stLINEMEM.active_counter-- ;
		}
	}	
}
//--------------------------------------------------------------------------------------------------------------------------
static void NRCTRL_WdrFlagControlHandler(PNR_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&NRCTRL_WDR_FLAG_UPDATE)!=0) {
		pCTRL->ctrl &= (~NRCTRL_WDR_FLAG_UPDATE);
#if __NR_REV__
		MISP_RegField(MISP_LOCAL_ID, 0x518, 15, 1, pCTRL->stWDR.wdr_tm_only_mode);
		MISP_RegField(MISP_LOCAL_ID, 0x518, 11, 1, pCTRL->stWDR.mo_his_rd_en);
		MISP_RegField(MISP_LOCAL_ID, 0x518,  6, 1, pCTRL->stWDR.mo_flag_rd_en);
		MISP_RegField(MISP_LOCAL_ID, 0x518,  3, 1, pCTRL->stWDR.mo_his_rd_en);
		MISP_RegField(MISP_LOCAL_ID, 0x521, 15, 1, pCTRL->stWDR.r_wdr_mo_en);
		MISP_RegField(MISP_LOCAL_ID, 0x521, 14, 1, pCTRL->stWDR.r_color_suppress_en);
		MISP_RegField(MISP_LOCAL_ID, 0x521, 11, 1, pCTRL->stWDR.r_chk_surface);
		MISP_RegField(MISP_LOCAL_ID, 0x535, 9, 1, pCTRL->stWDR.r_2D_wdr_mo_mode);
#endif
	}	
}
//--------------------------------------------------------------------------------------------------------------------------
static void NRCTRL_2D_MFilterHandler(PNR_CTRL_PARAM pCTRL)
{
#if __NR_REV__	
	if ((pCTRL->ctrl&NRCTRL_2D_MFILTER_UPDATE)==0) return;
	pCTRL->ctrl &= (~NRCTRL_2D_MFILTER_UPDATE);	
		 
	MISP_RegField(MISP_LOCAL_ID, 0x7D6, 9, 7, (pCTRL->stM2DNR.th_gain<<1)|pCTRL->stM2DNR.filter_en);
	MISP_RegField(MISP_LOCAL_ID, 0x7D7, 9, 7, (pCTRL->stM2DNR.th_offset));
#endif
}
//--------------------------------------------------------------------------------------------------------------------------
	
static void NRCTRL_2D_sFilterHandler(PNR_CTRL_PARAM pCTRL)
{
#if __NR_REV__	
	WORD i ;
	BYTE value ;
	
	PBYTE pFilter ;

	const WORD cOP_FLAG = 0x2000 ;
	
	if ( ((pCTRL->ctrl&NRCTRL_2D_SFILTER_UPDATE)==0)  
		&& ((pCTRL->ctrl&NRCTRL_2Dc_SFILTER_UPDATE)==0) 
		&& ((pCTRL->ctrl&NRCTRL_2Df_SFILTER_UPDATE)==0)		
	) return;
	
	if ((pCTRL->ctrl&NRCTRL_2D_SFILTER_UPDATE)!=0)
	{
		pCTRL->ctrl &= (~NRCTRL_2D_SFILTER_UPDATE);
		
		pFilter = (PBYTE)MISP_NR_SFILTER[pCTRL->st2DNR.sFilter_sigma] ;
				
		for ( i = 0 ; i < 14 ; i++ )
		{
			value = pFilter[i]  ;
			MISP_RegWrite(MISP_LOCAL_ID, 0x54C, cOP_FLAG|0x0000|(i<<7)|value); // write_enable&channel | addr | value
		}
		MISP_RegWrite(MISP_LOCAL_ID, 0x54C, 0x0000); // 
	} 
	
	if ((pCTRL->ctrl&NRCTRL_2Dc_SFILTER_UPDATE)!=0)
	{
		pCTRL->ctrl &= (~NRCTRL_2Dc_SFILTER_UPDATE);
		
		pFilter =  (PBYTE)MISP_NR_SFILTER[pCTRL->st2DNR.c_sFilter_sigma] ;
		
		MISP_RegWrite(MISP_LOCAL_ID, 0x54C, cOP_FLAG|0x0800|(0<<7)|pFilter[2]); // write_enable&channel | addr | value
		MISP_RegWrite(MISP_LOCAL_ID, 0x54C, cOP_FLAG|0x0800|(1<<7)|pFilter[9]); // 
		MISP_RegWrite(MISP_LOCAL_ID, 0x54C, cOP_FLAG|0x0800|(2<<7)|pFilter[11]); // 
		MISP_RegWrite(MISP_LOCAL_ID, 0x54C, cOP_FLAG|0x0800|(3<<7)|pFilter[3]); // 
		MISP_RegWrite(MISP_LOCAL_ID, 0x54C, cOP_FLAG|0x0800|(4<<7)|pFilter[10]); // 
		MISP_RegWrite(MISP_LOCAL_ID, 0x54C, cOP_FLAG|0x0800|(5<<7)|pFilter[13]); // 
		MISP_RegWrite(MISP_LOCAL_ID, 0x54C, cOP_FLAG|0x0800|(6<<7)|pFilter[4]); // 
		MISP_RegWrite(MISP_LOCAL_ID, 0x54C, 0x0000); // 
	} 	
	

	if ((pCTRL->ctrl&NRCTRL_2Df_SFILTER_UPDATE)!=0)
	{
		pCTRL->ctrl &= (~NRCTRL_2Df_SFILTER_UPDATE);
		
		pFilter = (PBYTE)MISP_NR_SFILTER[pCTRL->st2DNR.sFilter_sigma_df] ;
				
		for ( i = 0 ; i < 14 ; i++ )
		{
			value = pFilter[i]  ;
			MISP_RegWrite(MISP_LOCAL_ID, 0x54C, cOP_FLAG|0x1000|(i<<7)|value); // write_enable&channel | addr | value
		}
		MISP_RegWrite(MISP_LOCAL_ID, 0x54C, 0x0000); // 
	} 		
	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x54C, 0xC000); // slut custom enable
#endif	
}
//--------------------------------------------------------------------------------------------------------------------------
static void NRCTRL_2D_NBHandler(PNR_CTRL_PARAM pCTRL)
{
#if __NR_REV__
	static BYTE sNB_2D = 255 ;
	static BYTE sNB_2Df = 255 ;
	WORD i ;
	BYTE value ;
	
	PBYTE pFilter ;
	
	if ( ((pCTRL->ctrl&NRCTRL_2D_NB_UPDATE)==0) ) return ;
		pCTRL->ctrl &= (~NRCTRL_2D_NB_UPDATE);
	
	if ( sNB_2D != pCTRL->st2DNR.noise_bound_no )
	{			
		pFilter = (PBYTE)MISP_NR_NOISEBOUND[pCTRL->st2DNR.noise_bound_no] ;
		for ( i = 0 ; i < 5 ; i++ )
		{
			value = pFilter[i]  ;
			MISP_RegWrite(MISP_LOCAL_ID, 0x533, 0x1000|(i<<8)|value); // write_enable&channel | addr | value
				
			// UARTprintf("sNB_2D : [i:%d], [value:%d]\r\n", i , value ) ;
		}		
		
		sNB_2D = pCTRL->st2DNR.noise_bound_no ;
		
		MISP_RegWrite(MISP_LOCAL_ID, 0x533, 0x0000); // 
	}
	
	if ( sNB_2Df != pCTRL->st2DNR.noise_bound_no_df )
	{			
		pFilter = (PBYTE)MISP_NR_NOISEBOUND[pCTRL->st2DNR.noise_bound_no_df] ;
		for ( i = 0 ; i < 5 ; i++ )
		{
			value = pFilter[i]  ;
			MISP_RegWrite(MISP_LOCAL_ID, 0x533, 0x1800|(i<<8)|value); // write_enable&channel | addr | value
				
			// UARTprintf("sNB_2Df : [i:%d], [value:%d]\r\n", i , value ) ;
		}		
		
		sNB_2Df = pCTRL->st2DNR.noise_bound_no_df ;
		
		MISP_RegWrite(MISP_LOCAL_ID, 0x533, 0x0000); // 
	}
	
	MISP_RegField(MISP_LOCAL_ID, 0x528, 0, 7, (pCTRL->st2DNR.rold_impulse_level));	
	MISP_RegField(MISP_LOCAL_ID, 0x531, 0, 14, (pCTRL->st2DNR.rold_impulse_th<<7)|(pCTRL->st2DNR.rold_ll_impulse_th));	//
	MISP_RegField(MISP_LOCAL_ID, 0x532, 8, 8, (pCTRL->st2DNR.nf_bypass_level<<4)|(pCTRL->st2DNR.stdev_bypass_level)) ;
	MISP_RegField(MISP_LOCAL_ID, 0x532, 4, 4, (pCTRL->st2DNR.noise_bound_mode<<2)|(pCTRL->st2DNR.noise_bound_mode_df)) ;
	MISP_RegField(MISP_LOCAL_ID, 0x532, 2, 2, (pCTRL->st2DNR.filter_mode_df));	// 	
	MISP_RegField(MISP_LOCAL_ID, 0x534, 0, 14, (pCTRL->st2DNR.rold_impulse_th_df<<7)|(pCTRL->st2DNR.rold_ll_impulse_th_df));
	MISP_RegField(MISP_LOCAL_ID, 0x535, 13, 3, ((pCTRL->st2DNR.filter_serial<<2)|(pCTRL->st2DNR.filter_mode)));
	MISP_RegField(MISP_LOCAL_ID, 0x535, 10, 3, ((pCTRL->st2DNR.c_filter_serial<<2)|(pCTRL->st2DNR.c_filter_mode)));
	MISP_RegField(MISP_LOCAL_ID, 0x537, 12, 4, ((pCTRL->st2DNR.impulse_replace<<2)|(pCTRL->st2DNR.impulse_replace)));
	
#else
	static BYTE sNB_2D = 255 ;

	PBYTE pFilter ;
	
	if ( ((pCTRL->ctrl&NRCTRL_2D_NB_UPDATE)==0) ) return ;
		pCTRL->ctrl &= (~NRCTRL_2D_NB_UPDATE);	
	
	if ( sNB_2D != pCTRL->st2DNR.noise_bound_no )
	{	
		pFilter = (PBYTE)MISP_NR_NOISEBOUND[pCTRL->st2DNR.noise_bound_no] ;

		MISP_RegField(MISP_LOCAL_ID, 0x532, 0, 8, (pFilter[0]));	// 	
		MISP_RegWrite(MISP_LOCAL_ID, 0x533, (pFilter[1]<<8)|(pFilter[2]));	// 	
		MISP_RegWrite(MISP_LOCAL_ID, 0x534, (pFilter[3]<<8)|(pFilter[4]));	// 	
		
		sNB_2D = pCTRL->st2DNR.noise_bound_no ;
	}
	
	MISP_RegField(MISP_LOCAL_ID, 0x531, 0, 14, (pCTRL->st2DNR.rold_impulse_th<<7)|(pCTRL->st2DNR.rold_ll_impulse_th));	// 	

#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void NRCTRL_2D_LFilterHandler(PNR_CTRL_PARAM pCTRL)
{
#if __NR_L2DNR_CONTROL__		
	if ((pCTRL->ctrl&NRCTRL_2D_LFILTER_UPDATE)==0) return;	
	pCTRL->ctrl &= (~NRCTRL_2D_LFILTER_UPDATE);		
	
	MISP_RegField(MISP_LOCAL_ID, 0x02A, 0, 1, pCTRL->stL2DNR.y_en);
	MISP_RegField(MISP_LOCAL_ID, 0x02A, 3, 1, pCTRL->stL2DNR.c_en);
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x022, pCTRL->stL2DNR.y_coef_00);
	MISP_RegWrite(MISP_LOCAL_ID, 0x023, pCTRL->stL2DNR.y_coef_01);
	MISP_RegWrite(MISP_LOCAL_ID, 0x024, pCTRL->stL2DNR.y_coef_02);
	MISP_RegWrite(MISP_LOCAL_ID, 0x025, pCTRL->stL2DNR.y_coef_03);
	MISP_RegWrite(MISP_LOCAL_ID, 0x026, 0x0000);
	MISP_RegWrite(MISP_LOCAL_ID, 0x027, 0x0000);
	MISP_RegWrite(MISP_LOCAL_ID, 0x028, 0x0000);
	MISP_RegWrite(MISP_LOCAL_ID, 0x029, 0x0000);
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x02C, pCTRL->stL2DNR.c_coef_00);
	MISP_RegWrite(MISP_LOCAL_ID, 0x02D, pCTRL->stL2DNR.c_coef_01);
	MISP_RegWrite(MISP_LOCAL_ID, 0x02E, pCTRL->stL2DNR.c_coef_02);
	MISP_RegWrite(MISP_LOCAL_ID, 0x02F, pCTRL->stL2DNR.c_coef_03);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------

static void NRCTRL_AUTOL2DNR ( PNR_CTRL_PARAM pCTRL )	
{
#if __NR_L2DNR_CONTROL__	
	const WORD chase_speed = 4 ;
	WORD rVal ;
	WORD stdev_int ;
	WORD L2Dfilter_current ;
	WORD L2Dfilter_min ;
	WORD L2Dfilter_target ;
	WORD L2Dfilter_tint_current ;
	WORD L2Dfilter_tint_target ;
	
	if ((pCTRL->mode == MISP_NR_OFF )) return ; // Runs only when MISP_NR_2D or MISP_NR_2D3D mode on MENU 
	
	if ( pCTRL->stL2DNR.update_counter == 0 )
	{	
		pCTRL->stL2DNR.update_counter = 2 ;
	}
	else
	{
		pCTRL->stL2DNR.update_counter-- ;
	}			
		
		
	if ( pCTRL->stL2DNR.update_counter == 0 )
	{
		MISP_RegRead(MISP_LOCAL_ID, 0x53F, &rVal);
		rVal = rVal&0x0020 ;				
	
		if ( rVal == 0 ) return ;
		
		stdev_int = pCTRL->stFRAMENOISE.stdev_int ;
		L2Dfilter_min = pCTRL->stL2DNR.coef_min ;		
		
		if ( stdev_int < pCTRL->st3DNR.k_value_bound00 ) {
			L2Dfilter_target = 1024 ;
		}
		else if ( stdev_int < pCTRL->st3DNR.k_value_bound01 ){
			L2Dfilter_target = ((1024+L2Dfilter_min)>>1) ;
		}
		else if ( stdev_int < pCTRL->st3DNR.k_value_bound02 ){
			L2Dfilter_target = ((((1024+L2Dfilter_min)>>1) + L2Dfilter_min)>>1) ;
		}
		else{
			L2Dfilter_target = L2Dfilter_min ;	
		}
				
		if ( stdev_int < pCTRL->st3DNR.k_value_bound02 ) {
			L2Dfilter_tint_target = 0 ;
		}
		else if ( stdev_int < pCTRL->st3DNR.k_value_bound03 ){
			L2Dfilter_tint_target = 1 ;
		}
		else if ( stdev_int < pCTRL->st3DNR.k_value_bound04 ){
			L2Dfilter_tint_target = 2 ;
		}
		else{
			L2Dfilter_tint_target = 3 ;	
		}
				
		L2Dfilter_current = pCTRL->stL2DNR.y_coef_00 ;		
		L2Dfilter_tint_current = pCTRL->stL2DNR.c_tint ;
		
		if ( L2Dfilter_current > L2Dfilter_target ) {
			if (( L2Dfilter_current - L2Dfilter_target) < chase_speed )
				L2Dfilter_current = L2Dfilter_target ;
			else
				L2Dfilter_current = L2Dfilter_current - chase_speed  ;
		}
		else if( L2Dfilter_current < L2Dfilter_target ) {
			if (( L2Dfilter_target - L2Dfilter_current) < chase_speed )
				L2Dfilter_current = L2Dfilter_target ;
			else
				L2Dfilter_current = L2Dfilter_current + chase_speed  ;
		}
		
		if ( L2Dfilter_tint_current > L2Dfilter_tint_target ) {
			L2Dfilter_tint_current-- ;
		}
		else if ( L2Dfilter_tint_current < L2Dfilter_tint_target ) {
			L2Dfilter_tint_current++ ;
		}		
		
		SetNR_LetzteresfilterY(L2Dfilter_current,2) ; // Letzteresfilter Y 
		SetNR_LetzteresfilterC(L2Dfilter_current,2,L2Dfilter_tint_current) ; // Letzteresfilter C 5 tab mode
		
		// operation_mode = pCTRL->lowlight_mode ;		
	}
#endif
}
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
static void NRCTRL_FrameStdevUpdate( PNR_CTRL_PARAM pCTRL)
{	
//-----------------------------------------------------------------------------
#if __NR_REV__
//-----------------------------------------------------------------------------
	//
	//
	// i5x0 code start : NRCTRL_FrameStdevUpdate
	//
	//
//-----------------------------------------------------------------------------
#if (__NR_SPLIT__==0)||(__NR_SPLIT__==1)
	
#if __NR_SPLIT__==0		
	int i ;
#endif // #if __NR_SPLIT__==0

	WORD stdev_int ;
	WORD stdev_frac ;
	WORD stdev ;
	WORD pstdev ;
	
	WORD stdev_int_br ;
	WORD stdev_frac_br ;
	
	WORD stdev_int_da ;
	WORD stdev_frac_da ;	

	WORD prestdev_int ;
	WORD prestdev_frac ;		
	
	// BYTE bright_lower ;		
	// BYTE dark_upper ;
	
	WORD LL_enter_int ;
	WORD LL_enter_fr ;
	WORD LL_enter ;
	WORD LL_exit_int ;
	WORD LL_exit_fr ;
	WORD LL_exit ;
	
	LONG gain ;
	LONG dgain ;
	LONG max_gain ;	
	// LONG gain_Ysum ;
	LONG ae_Ysum ;
	LONG ae_Yref ;
	LONG ae_under_exp_level_1 = 0 ;
	LONG ae_under_exp_level_1_3 = 0 ;
	LONG ae_under_exp_level_1_7 = 0 ;
	LONG ae_under_exp_level_2 = 0 ;
	LONG ae_under_exp_level_2_3 = 0 ;
	LONG ae_under_exp_level_2_7 = 0 ;
	LONG ae_under_exp_level_3 = 0 ;
	// LONG GainCTLMode ;	
	
#if __NR_SPLIT__==1
	// NR split mode slave stdev
	WORD slave_stdev_int ;
	WORD slave_stdev_frac ;	

	WORD slave_stdev_int_br ;
	WORD slave_stdev_frac_br ;
	
	WORD slave_stdev_int_da ;
	WORD slave_stdev_frac_da ;	
#endif // #if __NR_SPLIT__==1
	
	if ( pCTRL->st3DNR.update_counter == 0 )
	{	
		pCTRL->st3DNR.update_counter = __NR_AUTO_PERIOD__ ;
	}
	else
	{
		pCTRL->st3DNR.update_counter-- ;
	}	
	
	// if ( pCTRL->st3DNR.update_counter == 0 )
	if ( pCTRL->st3DNR.update_counter == __NR_AUTO2DNR_TIMER__ || 
			pCTRL->st3DNR.update_counter == __NR_AUTOPDM_TIMER__ ||
			pCTRL->st3DNR.update_counter == __NR_AUTOSRM_TIMER__ ||
			pCTRL->st3DNR.update_counter == __NR_AUTOIDM_TIMER__  )
	{		

		// GainCTLMode = AECTRL_GetGainCTLMode() ;		
		
		// pCTRL->Yref = AECTRL_GetYREFVal(pCTRL->level);
		// AECTRL_GetYREFVal
		ae_Ysum = Get_AE_Ysum() ;
		ae_Yref = AECTRL_GetBrightness() ;

		// gain =  AECTRL_GetGAINGain() ;
		gain =  AECTRL_GetGAINAGain() ;
		dgain =  Get_AE_DGain() ;
		// max_gain = AECTRL_GetGAINMax() ;
		max_gain = AECTRL_GetGAINAMax() ;
		
#if __NR_SPLIT__==0
		pCTRL->lowlight_mode = GetNR_LowLightMode();		
		
		GetNR_StDEV(&stdev_int,&stdev_frac,0);
		GetNR_StDEV_br(&stdev_int_br,&stdev_frac_br,0);
		GetNR_StDEV_da(&stdev_int_da,&stdev_frac_da,0);
		GetNR_preStDEV(&prestdev_int,&prestdev_frac,0) ;		
#else // #if __NR_SPLIT__==1
		GetNR_StDEV(&stdev_int,&stdev_frac,0);	
		GetNR_StDEV_br(&stdev_int_br,&stdev_frac_br,0);
		GetNR_StDEV_da(&stdev_int_da,&stdev_frac_da,0);
		
		prestdev_int = 0 ; // No pre Stdev data for NR split mode
		prestdev_frac = 0 ;  // No pre Stdev data for NR split mode
		
	// Inquiry  StDev data from slave
		SPImAPP_GetSlaveNRStdev(&slave_stdev_int, &slave_stdev_frac);
		SPImAPP_GetSlaveNRStdevBr(&slave_stdev_int_br, &slave_stdev_frac_br);
		SPImAPP_GetSlaveNRStdevDa(&slave_stdev_int_da, &slave_stdev_frac_da);

		// UARTprintf("-Master----------------------------------------------------------------------\r\n") ;		
		// UARTprintf("[stdev_da:%d], [frac_da:%d] | [stdev:%d], [frac:%d] | [stdev_br:%d], [frac_br:%d]\r\n", stdev_int_da,stdev_frac_da, stdev_int ,stdev_frac,stdev_int_br,stdev_frac_br) ;		
		// UARTprintf("-Slave-----------------------------------------------------------------------\r\n") ;	
		// UARTprintf("[stdev_da:%d], [frac_da:%d] | [stdev:%d], [frac:%d] | [stdev_br:%d], [frac_br:%d]\r\n", slave_stdev_int_da,slave_stdev_frac_da, slave_stdev_int ,slave_stdev_frac,slave_stdev_int_br,slave_stdev_frac_br) ;		
			
	// Averaging data	
		stdev_int = (stdev_int + ( slave_stdev_int )) ;
		if ( stdev_int%2 == 1 ) stdev_frac = ( 8 + stdev_frac + ( slave_stdev_frac )) ;
		else                    stdev_frac = ( stdev_frac + ( slave_stdev_frac )) ;
		stdev_int = (stdev_int>>1) ;
		stdev_frac = (stdev_frac>>1) ;		
		
		stdev_int_br = (stdev_int_br + ( slave_stdev_int_br )) ;
		if ( stdev_int_br%2 == 1 ) stdev_frac_br = ( 8 + stdev_frac_br + ( slave_stdev_frac_br )) ;
		else                       stdev_frac_br = (stdev_frac_br + ( slave_stdev_frac_br )) ;
		stdev_int_br = (stdev_int_br>>1) ;
		stdev_frac_br = (stdev_frac_br>>1) ;
		
		stdev_int_da = (stdev_int_da + ( slave_stdev_int_da )) ;
		if ( stdev_int_da%2 == 1 ) stdev_frac_da = ( 8 + stdev_frac_da + ( slave_stdev_frac_da )) ;
		else                       stdev_frac_da = (stdev_frac_da + ( slave_stdev_frac_da )) ;
		stdev_int_da = (stdev_int_da>>1) ;
		stdev_frac_da = (stdev_frac_da>>1) ;
		
		// UARTprintf("-AVG data--------------------------------------------------------------------\r\n") ;	
		// UARTprintf("[stdev_da:%d], [frac_da:%d] | [stdev:%d], [frac:%d] | [stdev_br:%d], [frac_br:%d]\r\n", stdev_int_da,stdev_frac_da, stdev_int ,stdev_frac,stdev_int_br,stdev_frac_br) ;		
		// UARTprintf("-----------------------------------------------------------------------------\r\n") ;		
#endif // #if __NR_SPLIT__==0
			
		stdev = (stdev_int<<3)|(stdev_frac) ;
		pstdev = (pCTRL->stFRAMENOISE.stdev_int<<3)|(pCTRL->stFRAMENOISE.stdev_frac) ;
		
		LL_enter_int = pCTRL->stLL.ent_level ;		
		LL_enter_fr = pCTRL->stLL.ent_level_fr ;
		
		LL_exit_int = pCTRL->stLL.exit_level ;
		LL_exit_fr = pCTRL->stLL.exit_level_fr ;
		
		// Calc LL exit level
		if ( LL_enter_fr == 0 )
		{
			LL_enter_int = LL_enter_int - 1 ;
			LL_enter_fr = 7 ;
		}
		else
		{
			// LL_enter_int = LL_enter_int ;
			LL_enter_fr = LL_enter_fr - 1  ;	
		}
		
		LL_enter = (LL_enter_int<<3)|LL_enter_fr ;
		LL_exit = (LL_exit_int<<3)|LL_exit_fr ;
		
		// dark_upper = pCTRL->stFRAMENOISE.dark_upper_bound ;
		// bright_lower = pCTRL->stFRAMENOISE.bright_lower_bound ;
		
		// if ( gain == max_gain && GainCTLMode != AECTRL_GAINCTL_OFF ) {
		// if ( gain == max_gain && max_gain != 256 ) // If gain reached max
		if ( (gain >= max_gain && max_gain != 256) || (dgain > 256)) // If gain reached max
		{			
			ae_under_exp_level_1 = (ae_Yref>>1) ; // -1EV of ae_Yref
			ae_under_exp_level_1_3 = (ae_under_exp_level_1<<2)/5 ; // -1.3EV of ae_Yref
			ae_under_exp_level_1_7 = (ae_under_exp_level_1_3<<2)/5 ; // -1.7EV of ae_Yref
			ae_under_exp_level_2 = (ae_Yref>>2) ; // -2EV of ae_Yref
			ae_under_exp_level_2_3 = (ae_under_exp_level_2<<2)/5 ; // -2.3EV of ae_Yref
			ae_under_exp_level_2_7 = (ae_under_exp_level_2_3<<2)/5 ; // -2.7EV of ae_Yref
			ae_under_exp_level_3 = (ae_Yref>>3) ; // -3EV of ae_Yref				
			
			if ( (ae_Ysum > ae_under_exp_level_3) || (stdev > pstdev) ) 
			{
				pCTRL->stFRAMENOISE.stdev_int = stdev_int ;
				pCTRL->stFRAMENOISE.stdev_frac = stdev_frac ;
				
				pCTRL->stFRAMENOISE.stdev_int_br = stdev_int_br ;
				pCTRL->stFRAMENOISE.stdev_frac_br = stdev_frac_br ;
				
				pCTRL->stFRAMENOISE.stdev_int_da = stdev_int_da ;
				pCTRL->stFRAMENOISE.stdev_frac_da = stdev_frac_da ;	
				
				pCTRL->stFRAMENOISE.prestdev_int = prestdev_int ;
				pCTRL->stFRAMENOISE.prestdev_frac = prestdev_frac ;					
			}
						
			pCTRL->stFRAMENOISE.agc_max = 1 ;
				
			if ( (ae_Ysum > ae_under_exp_level_1) )
			{
				if ( pCTRL->stFRAMENOISE.bound_state != 0 )	 { SetNR_PixelCategoryBound(125,80) ; }					
				
				pCTRL->stFRAMENOISE.bound_state = 0 ;
			}	
			else if ( (ae_Ysum > ae_under_exp_level_1_3) )
			{
				if ( pCTRL->stFRAMENOISE.bound_state != 1 )	 { SetNR_PixelCategoryBound(100,64) ; } 
				
				pCTRL->stFRAMENOISE.bound_state = 1 ;
			}
			else if ( (ae_Ysum > ae_under_exp_level_1_7) )
			{
				if ( pCTRL->stFRAMENOISE.bound_state != 2 )	 { SetNR_PixelCategoryBound(80,51) ; } 				
				
				pCTRL->stFRAMENOISE.bound_state = 2 ;
			}			
			else if ( (ae_Ysum > ae_under_exp_level_2) )
			{
				if ( pCTRL->stFRAMENOISE.bound_state != 3 )	 { SetNR_PixelCategoryBound(62,40) ; } 	
								
				pCTRL->stFRAMENOISE.bound_state = 3 ;
			}
			else if ( (ae_Ysum > ae_under_exp_level_2_3) )
			{
				if ( pCTRL->stFRAMENOISE.bound_state != 4 )	 { SetNR_PixelCategoryBound(50,32) ; } 	
								
				pCTRL->stFRAMENOISE.bound_state = 4 ;
			}		
			else if ( (ae_Ysum > ae_under_exp_level_2_7) )
			{
				if ( pCTRL->stFRAMENOISE.bound_state != 5 )	 { SetNR_PixelCategoryBound(40,26) ; } 	
								
				pCTRL->stFRAMENOISE.bound_state = 5 ;
			}				
			else  if ( (ae_Ysum > ae_under_exp_level_3) )
			{
				if ( pCTRL->stFRAMENOISE.bound_state != 6 )	 { SetNR_PixelCategoryBound(31,20) ; } 					
				
				pCTRL->stFRAMENOISE.bound_state = 6 ;
			}
			else
			{
				if ( pCTRL->stFRAMENOISE.bound_state != 7 )	 { SetNR_PixelCategoryBound(15,10) ; } 						
				
				pCTRL->stFRAMENOISE.bound_state = 7 ;
			}
			
			if ( (ae_Ysum > ae_under_exp_level_1) )
			{
				pCTRL->stFRAMENOISE.under_exp = 0 ;
				
				if ( LL_enter != LL_exit )
				{
					SetNR_LowLightExitLevel(LL_enter_int,LL_enter_fr) ;
				}
			}	
			else if ( (ae_Ysum > ae_under_exp_level_2) )
			{
				pCTRL->stFRAMENOISE.under_exp = 1 ;				
				if ( (LL_enter-8) != LL_exit )
				{
					SetNR_LowLightExitLevel((LL_enter_int-1),LL_enter_fr) ;
				}
			}
			else  if ( (ae_Ysum > ae_under_exp_level_3) )
			{
				pCTRL->stFRAMENOISE.under_exp = 2 ;
				if ( (LL_enter-16) != LL_exit )
				{
					SetNR_LowLightExitLevel((LL_enter_int-2),LL_enter_fr) ;
				}
			}
			else
			{
				pCTRL->stFRAMENOISE.under_exp = 3 ;	
				if ( (LL_enter-17) != LL_exit )
				{
					SetNR_LowLightExitLevel((LL_enter_int-2),(LL_enter_fr-1)) ;
				}
			}				
		}
		else{ // If gain is not max or AGC off
			// UARTprintf("Update Stdev\r\n") ;
			pCTRL->stFRAMENOISE.stdev_int = stdev_int ;
			pCTRL->stFRAMENOISE.stdev_frac = stdev_frac ;
			
			pCTRL->stFRAMENOISE.stdev_int_br = stdev_int_br ;
			pCTRL->stFRAMENOISE.stdev_frac_br = stdev_frac_br ;
			
			pCTRL->stFRAMENOISE.stdev_int_da = stdev_int_da ;
			pCTRL->stFRAMENOISE.stdev_frac_da = stdev_frac_da ;	
			
			pCTRL->stFRAMENOISE.prestdev_int = prestdev_int ;
			pCTRL->stFRAMENOISE.prestdev_frac = prestdev_frac ;
			
			pCTRL->stFRAMENOISE.agc_max = 0 ;	
			pCTRL->stFRAMENOISE.under_exp = 0 ;				
			
			if ( LL_enter != LL_exit )
			{
				pCTRL->stFRAMENOISE.bound_state = 0 ;
				SetNR_LowLightExitLevel(LL_enter_int,LL_enter_fr) ;	
				SetNR_PixelCategoryBound(125,80);
			}
		}		
		
		NRCTRL_AUTOPEL(pCTRL) ;
		
#if __NR_SPLIT__==0		
		for ( i = 3 ; i > 0 ; i-- )
		{
			pCTRL->stFRAMENOISE.mblocks[i] = pCTRL->stFRAMENOISE.mblocks[i-1] ;
			// UARTprintf("stFRAMENOISE.mblocks[%d] : %d, ratio : %d \r\n",i,pCTRL->stFRAMENOISE.mblocks[i],pCTRL->stFRAMENOISE.corr_ratio);
		}
		pCTRL->stFRAMENOISE.mblocks[0] = GetNR_mblocks() ;
#endif // #if __NR_SPLIT__==0	
	
		MISP_RegField( MISP_LOCAL_ID, 0x53F, 8, 1, pCTRL->stFRAMENOISE.agc_max) ;
		MISP_RegField( MISP_LOCAL_ID, 0x53F, 14, 2, pCTRL->stFRAMENOISE.under_exp) ;
		
#if (__NR_DEBUG_PRINT__==1)
		UARTprintf("[stdev_da:%d], [frac_da:%d] | [stdev:%d], [frac:%d] | [stdev_br:%d], [frac_br:%d]\r\n", stdev_int_da,stdev_frac_da, stdev_int ,stdev_frac,stdev_int_br,stdev_frac_br) ;		
		UARTprintf("                            [pstdev:%d],[pfrac:%d]\r\n",prestdev_int,prestdev_frac) ;		
		UARTprintf("                            [stdev_:%d],[frac_:%d]\r\n",pCTRL->stFRAMENOISE.stdev_int,pCTRL->stFRAMENOISE.stdev_frac) ;		
		UARTprintf("[aeYsum:%d], [ae_Yref:%d] | [Bound state : %d]\r\n", ae_Ysum , ae_Yref , pCTRL->stFRAMENOISE.bound_state ) ;
		
		if ( (pCTRL->stFRAMENOISE.agc_max == 1) && (gain >= max_gain) )
		{
			UARTprintf("[under_exp_level_1:%d]\r\n", ae_under_exp_level_1 ) ;
			UARTprintf("[under_exp_level_1_3:%d]\r\n", ae_under_exp_level_1_3 ) ;
			UARTprintf("[under_exp_level_1_7:%d]\r\n", ae_under_exp_level_1_7 ) ;
			UARTprintf("[under_exp_level_2:%d]\r\n", ae_under_exp_level_2 ) ;
			UARTprintf("[under_exp_level_2_3:%d]\r\n", ae_under_exp_level_2_3 ) ;
			UARTprintf("[under_exp_level_2_7:%d]\r\n", ae_under_exp_level_2_7 ) ;
			UARTprintf("[under_exp_level_3:%d]\r\n", ae_under_exp_level_3 ) ;
		}
		
		UARTprintf("[Gain:%d], [MaxGain:%d]\r\n", gain ,max_gain) ;
#endif	// #if (__NR_DEBUG_PRINT__==1)	

#if __NR_SPLIT__==1		
	// Split mode : master send command to slave
	pCTRL->lowlight_mode = GetNR_LowLightMode(); // GetNR_LowLightMode() function returns MCU drive operation mode flag when NR split mode
	
	// LL_enter & LL_exit are adjusted value
	LL_enter = (pCTRL->stLL.ent_level<<3)|(pCTRL->stLL.ent_level_fr) ;
	LL_exit = (pCTRL->stLL.exit_level<<3)|(pCTRL->stLL.exit_level_fr) ;
	
	stdev_int = (pCTRL->stFRAMENOISE.stdev_int<<3)|(pCTRL->stFRAMENOISE.stdev_frac) ;
	
	if ( pCTRL->lowlight_mode == 0 ) { // MCU drive setting is "NORMAL operation"	
		if ( stdev_int > LL_enter ) SetNR_MCUDriveMode(1,1,0,0,0) ;
	}
	else{ // MCU drive setting is "Lowlight operation"
		if ( stdev_int <= LL_exit ) SetNR_MCUDriveMode(1,0,0,0,0) ;
	}
	
	SPImAPP_SetSlaveNRMISC(pCTRL->stFRAMENOISE.agc_max, pCTRL->stFRAMENOISE.under_exp, pCTRL->stFRAMENOISE.edge_level, pCTRL->lowlight_mode);
	SPImAPP_SetSlaveNRStdev(pCTRL->stFRAMENOISE.stdev_int, pCTRL->stFRAMENOISE.stdev_frac);
	SPImAPP_SetSlaveNRStdevBr(pCTRL->stFRAMENOISE.stdev_int_br, pCTRL->stFRAMENOISE.stdev_frac_br);
	SPImAPP_SetSlaveNRStdevDa(pCTRL->stFRAMENOISE.stdev_int_da, pCTRL->stFRAMENOISE.stdev_frac_da);
#endif	
	}
	
#if __NR_SPLIT__==1		
	SPImAPP_SetSlaveNRUpCnt(pCTRL->st3DNR.update_counter);
#endif	
	
#else // __NR_SPLIT__==2
	// if ( pCTRL->st3DNR.update_counter == 0 )
	// {	
		// pCTRL->st3DNR.update_counter = __NR_AUTO_PERIOD__ ;
	// }
	// else
	// {
		// pCTRL->st3DNR.update_counter-- ;
	// }

	// if ( pCTRL->st3DNR.update_counter == __NR_AUTO2DNR_TIMER__ || 
			// pCTRL->st3DNR.update_counter == __NR_AUTOPDM_TIMER__ ||
			// pCTRL->st3DNR.update_counter == __NR_AUTOSRM_TIMER__ ||
			// pCTRL->st3DNR.update_counter == __NR_AUTOIDM_TIMER__  )
	// {
		// pCTRL->lowlight_mode = GetNR_LowLightMode(); // GetNR_LowLightMode() function returns MCU drive operation mode flag when NR split mode
	// }		
#endif // #if (__NR_SPLIT__==0)||(__NR_SPLIT__==1)		
//-----------------------------------------------------------------------------
	//
	//
	// i5x0 code end : NRCTRL_FrameStdevUpdate
	//
	//
//-----------------------------------------------------------------------------
#else	
//-----------------------------------------------------------------------------
	//
	//
	// i500 code start : NRCTRL_FrameStdevUpdate
	//
	//
//-----------------------------------------------------------------------------		
	WORD stdev_int ;
	WORD stdev_frac ;	
	WORD pstdev ;	
	WORD stdev ;	
	
	WORD LL_enter_int ;
	WORD LL_enter_fr ;
	WORD LL_enter ;
	WORD LL_exit_int ;
	WORD LL_exit_fr ;
	WORD LL_exit ;		
	
	LONG gain ;
	LONG dgain ;
	LONG max_gain ;	
	// LONG gain_Ysum ;
	LONG ae_Ysum ;
	LONG ae_Yref ;
	LONG ae_under_exp_level_1 ;
	LONG ae_under_exp_level_2 ;
	LONG ae_under_exp_level_3 ;
	// LONG GainCTLMode ;	

	if ( pCTRL->st3DNR.update_counter == 0 )
	{	
		pCTRL->st3DNR.update_counter = __NR_AUTO_PERIOD__ ;
	}
	else
	{
		pCTRL->st3DNR.update_counter-- ;
	}	
	
	// if ( pCTRL->st3DNR.update_counter == 0 )
	if ( pCTRL->st3DNR.update_counter == __NR_AUTO2DNR_TIMER__ || 
			pCTRL->st3DNR.update_counter == __NR_AUTOPDM_TIMER__ ||
			pCTRL->st3DNR.update_counter == __NR_AUTOSRM_TIMER__ ||
			pCTRL->st3DNR.update_counter == __NR_AUTOIDM_TIMER__  )
	{		
		// GainCTLMode = AECTRL_GetGainCTLMode() ;
		pCTRL->lowlight_mode = GetNR_LowLightMode();
		
		ae_Ysum = Get_AE_Ysum() ;
		ae_Yref = AECTRL_GetBrightness() ;
		ae_under_exp_level_1 = (ae_Yref>>1) ; // -1EV of ae_Yref
		ae_under_exp_level_2 = (ae_Yref>>2) ; // -2EV of ae_Yref
		ae_under_exp_level_3 = (ae_Yref>>3) ; // -3EV of ae_Yref
		// gain =  AECTRL_GetGAINGain() ;
		gain =  AECTRL_GetGAINAGain() ;
		dgain =  Get_AE_DGain() ;
		// max_gain = AECTRL_GetGAINMax() ;
		max_gain = AECTRL_GetGAINAMax() ;
		
		GetNR_StDEV(&stdev_int,&stdev_frac,0);
		
		stdev = (stdev_int<<3)|(stdev_frac);
		pstdev = (pCTRL->stFRAMENOISE.stdev_int<<3)|(pCTRL->stFRAMENOISE.stdev_frac);

		LL_enter_int = pCTRL->stLL.ent_level ;		
		LL_enter_fr = pCTRL->stLL.ent_level_fr ;
		
		LL_exit_int = pCTRL->stLL.exit_level ;
		LL_exit_fr = pCTRL->stLL.exit_level_fr ;
		
		// Calc LL exit level
		if ( LL_enter_fr == 0 )
		{
			LL_enter_int = LL_enter_int - 1 ;
			LL_enter_fr = 7 ;
		}
		else
		{
			// LL_enter_int = LL_enter_int ;
			LL_enter_fr = LL_enter_fr - 1  ;	
		}
		
		LL_enter = (LL_enter_int<<3)|LL_enter_fr ;
		LL_exit = (LL_exit_int<<3)|LL_exit_fr ;		
		
		
#if __NR_DEBUG_PRINT__	
	UARTprintf("[stdev:%d], [frac:%d]\r\n", stdev_int ,stdev_frac) ;	
	UARTprintf("[ae_Ysum:%d], [ae_Yref:%d] \r\n", ae_Ysum, ae_Yref) ;
	UARTprintf("[Gain:%d], [MaxGain:%d]\r\n", gain ,max_gain) ;		
#endif
		
		// if ( gain == max_gain && GainCTLMode != AECTRL_GAINCTL_OFF ) {
		// if ( gain == max_gain && max_gain != 256 ) {
		if ( (gain >= max_gain && max_gain != 256) || (dgain > 256)) {
			if ( (ae_Ysum > ae_under_exp_level_3) || (stdev > pstdev) ) {
				// UARTprintf("Update Stdev max_gain\r\n") ;
				pCTRL->stFRAMENOISE.stdev_int = stdev_int ;
				pCTRL->stFRAMENOISE.stdev_frac = stdev_frac ;
			}
			
			pCTRL->stFRAMENOISE.agc_max = 1 ;
			
			if ( (ae_Ysum > ae_under_exp_level_1) )
			{
				pCTRL->stFRAMENOISE.under_exp = 0 ;
				
				if ( LL_enter != LL_exit )
				{
					SetNR_LowLightExitLevel(LL_enter_int,LL_enter_fr) ;
					// SetNR_PixelCategoryBound(125,80); Not support
				}
			}			
			else if ( (ae_Ysum > ae_under_exp_level_2) )
			{
				pCTRL->stFRAMENOISE.under_exp = 1 ;				
				if ( (LL_enter-8) != LL_exit )
				{
					SetNR_LowLightExitLevel((LL_enter_int-1),LL_enter_fr) ;
					// SetNR_PixelCategoryBound(62,40);  Not support
				}
			}
			else  if ( (ae_Ysum > ae_under_exp_level_3) )
			{
				pCTRL->stFRAMENOISE.under_exp = 2 ;
				if ( (LL_enter-16) != LL_exit )
				{
					SetNR_LowLightExitLevel((LL_enter_int-2),LL_enter_fr) ;
					// SetNR_PixelCategoryBound(31,20);  Not support
				}
			}
			else
			{
				pCTRL->stFRAMENOISE.under_exp = 3 ;	
				if ( (LL_enter-16) != LL_exit )
				{
					SetNR_LowLightExitLevel((LL_enter_int-2),LL_enter_fr) ;
					// SetNR_PixelCategoryBound(15,10);  Not support
				}
			}
		}
		else{
			// UARTprintf("Update Stdev\r\n") ;
			pCTRL->stFRAMENOISE.stdev_int = stdev_int ;
			pCTRL->stFRAMENOISE.stdev_frac = stdev_frac ;
			
			pCTRL->stFRAMENOISE.agc_max = 0 ;
			pCTRL->stFRAMENOISE.under_exp = 0 ;
			
			if ( LL_enter != LL_exit )
			{
				SetNR_LowLightExitLevel(LL_enter_int,LL_enter_fr) ;
				// SetNR_PixelCategoryBound(125,80);  Not support
			}
		}
		
		MISP_RegField( MISP_LOCAL_ID, 0x53F, 8, 1, pCTRL->stFRAMENOISE.agc_max) ;
		MISP_RegField( MISP_LOCAL_ID, 0x53F, 14, 2, pCTRL->stFRAMENOISE.under_exp) ;		
		
	}
//-----------------------------------------------------------------------------
	//
	//
	// i500 code end : NRCTRL_FrameStdevUpdate
	//
	//
//-----------------------------------------------------------------------------	
#endif	
//-----------------------------------------------------------------------------
} // NRCTRL_FrameStdevUpdate
	
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
static void NRCTRL_AUTOPEL ( PNR_CTRL_PARAM pCTRL )	
{
#if (__NR_SPLIT__==0)||(__NR_SPLIT__==1)	
	DWORD max_gain,gain ;
	DWORD dgain ;
	// WORD stdev_int, stdev_frac ;

	BYTE image_type ;
	WORD edge_level_target ;
	BYTE digital_full ;
	BYTE digital_half ;
	WORD current_level ;
	
	BYTE corr_ratio ;
	
	DWORD agc_phase_0 ;
	DWORD agc_phase_1 ;
	DWORD agc_phase_2 ;	
	DWORD agc_phase_3 ;	
	DWORD agc_phase_4 ;		
	DWORD agc_phase_5 ;		
	DWORD agc_phase_6 ;		
	
	image_type = pCTRL->image_type ;
	current_level = pCTRL->stFRAMENOISE.edge_level ;
	
	// gain =  AECTRL_GetGAINGain() ;
	gain =  AECTRL_GetGAINAGain() ;
	dgain = Get_AE_DGain() ;
	// max_gain = AECTRL_GetGAINMax() ;
	max_gain = AECTRL_GetGAINAMax() ;
	
	// stdev_int = pCTRL->stFRAMENOISE.stdev_int ;
	// stdev_frac = pCTRL->stFRAMENOISE.stdev_frac ;	
	
	agc_phase_0 = max_gain>>7 ;
	agc_phase_1 = max_gain>>6 ;
	agc_phase_2 = max_gain>>5 ;
	agc_phase_3 = max_gain>>4 ;
	agc_phase_4 = max_gain>>3 ;	 
	agc_phase_5 = max_gain>>2 ;	 
	agc_phase_6 = max_gain>>1 ;	 
	
	corr_ratio = pCTRL->stFRAMENOISE.corr_ratio ;
	
	if ( image_type == MISP_NR_BAYER_IMAGE )
	{
		// if ( gain == max_gain && max_gain != 256 )
		if ( (gain >= max_gain && max_gain != 256) || (dgain > 256))
		{ // Gain max + Digital gain
			digital_full = (BYTE)(dgain>>7) ;
		
			if ( digital_full%2 == 1 ) digital_half = 1 ;
			else						digital_half = 0 ;				
				
			digital_full = (digital_full>>1) - 1 ;	
			
			if ( corr_ratio == 1 ) edge_level_target = __NR_BY_EDGE_LEVEL__ + (__NR_BY_EDGE_LEVEL__>>(3))*digital_full + (__NR_BY_EDGE_LEVEL__>>(4))*digital_half ;
			else edge_level_target = __NR_BY_EDGE_LEVEL__ + (__NR_BY_EDGE_LEVEL__>>(2))*digital_full + (__NR_BY_EDGE_LEVEL__>>(3))*digital_half ;
		}	
		else 
		{ // 
			if ( gain < agc_phase_0 ) 		edge_level_target = __NR_BY_EDGE_LEVEL_0__ ;
			else if ( gain < agc_phase_1 ) edge_level_target = __NR_BY_EDGE_LEVEL_1__ ;
			else if ( gain < agc_phase_2 )	edge_level_target = __NR_BY_EDGE_LEVEL_2__ ;
			else if ( gain < agc_phase_3 )	edge_level_target = __NR_BY_EDGE_LEVEL_3__ ;
			else if ( gain < agc_phase_4 )	edge_level_target = __NR_BY_EDGE_LEVEL_4__ ;
			else if ( gain < agc_phase_5 )	edge_level_target = __NR_BY_EDGE_LEVEL_5__ ;
			else if ( gain < agc_phase_6 )	edge_level_target = __NR_BY_EDGE_LEVEL_6__ ;
			else							edge_level_target = __NR_BY_EDGE_LEVEL__ ;
		}
	}
	else // if ( image_type == MISP_NR_YC_IMAGE )
	{
		// if ( gain == max_gain && max_gain != 256 )
		if ( (gain >= max_gain && max_gain != 256) || (dgain > 256))
		{ // Gain max + Digital gain
			digital_full = (BYTE)(dgain>>7) ;
		
			if ( digital_full%2 == 1 ) digital_half = 1 ;
			else						digital_half = 0 ;				
				
			digital_full = (digital_full>>1) - 1 ;	

			if ( corr_ratio == 1 ) edge_level_target = __NR_YC_EDGE_LEVEL__ + (__NR_YC_EDGE_LEVEL__>>(2))*digital_full + (__NR_YC_EDGE_LEVEL__>>(3))*digital_half ;
			else edge_level_target = __NR_YC_EDGE_LEVEL__ + (__NR_YC_EDGE_LEVEL__>>(1))*digital_full + (__NR_YC_EDGE_LEVEL__>>(2))*digital_half ;			
		}
		else 
		{ // 
			if ( gain < agc_phase_0 ) 		edge_level_target = __NR_YC_EDGE_LEVEL_0__ ;
			else if ( gain < agc_phase_1 ) edge_level_target = __NR_YC_EDGE_LEVEL_1__ ;
			else if ( gain < agc_phase_2 )	edge_level_target = __NR_YC_EDGE_LEVEL_2__ ;
			else if ( gain < agc_phase_3 )	edge_level_target = __NR_YC_EDGE_LEVEL_3__ ;
			else if ( gain < agc_phase_4 )	edge_level_target = __NR_YC_EDGE_LEVEL_4__ ;
			else if ( gain < agc_phase_5 )	edge_level_target = __NR_YC_EDGE_LEVEL_5__ ;
			else if ( gain < agc_phase_6 )	edge_level_target = __NR_YC_EDGE_LEVEL_6__ ;
			else							edge_level_target = __NR_YC_EDGE_LEVEL__ ;
		}	
	}
	
	if ( edge_level_target >= 100 ) edge_level_target = 100 ;
	
	if ( current_level < edge_level_target ) current_level++ ;
	else if( current_level > edge_level_target ) current_level-- ;
	
	if ( current_level != pCTRL->stFRAMENOISE.edge_level ) MISP_RegField(MISP_LOCAL_ID, 0x52A, 7 ,7 , current_level) ;		
	
	pCTRL->stFRAMENOISE.edge_level = current_level ;
	
#if (__NR_DEBUG_PRINT__==2)	
	UARTprintf("[agc_phase_0:%d][agc_phase_1:%d][agc_phase_2:%d]\r\n", agc_phase_0, agc_phase_1 ,agc_phase_2) ;
	UARTprintf("[agc_phase_3:%d][agc_phase_4:%d][agc_phase_5:%d][agc_phase_6:%d\r\n", agc_phase_3, agc_phase_4 ,agc_phase_5, agc_phase_6) ;
	UARTprintf("[gain:%d][dgain:%d][max_gain:%d][current_level:%d]\r\n", gain, dgain, max_gain, current_level) ;
#endif
	
#endif // #if (__NR_SPLIT__==0)||(__NR_SPLIT__==1)
}


//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------

static void NRCTRL_AUTOM2DNR ( PNR_CTRL_PARAM pCTRL )	
{
//-----------------------------------------------------------------------------
#if __NR_REV__
//-----------------------------------------------------------------------------
	//
	//
	// i5x0 code start : NRCTRL_AUTOM2DNR
	//
	//
//-----------------------------------------------------------------------------

	WORD rVal;
	WORD stdev_int ;
	WORD stdev_frac ;
	
	WORD prestdev_int ;
	WORD prestdev_frac ;
	
	WORD strategy ;
	
	BOOL operation_mode ;
	
	BYTE gain = 0 ; // 0 ~ 63
	BYTE offset_target ;
	BYTE offset_current ;
	// BYTE under_exp ;
	BYTE agc_max ;
	BYTE filter_level ;
	BYTE th_offset_limit ;	
	
	// if ( pCTRL->st3DNR.update_counter == 0 )
	if ( pCTRL->st3DNR.update_counter == __NR_AUTO2DNR_TIMER__ )
	{
		MISP_RegRead(MISP_LOCAL_ID, 0x53F, &rVal);
		rVal = rVal&0x0040 ;
		
		if ( rVal == 0 ) return ;
		
		stdev_int = pCTRL->stFRAMENOISE.stdev_int ;
		stdev_frac = pCTRL->stFRAMENOISE.stdev_frac ;		
		
		prestdev_int = pCTRL->stFRAMENOISE.prestdev_int;
		prestdev_frac = pCTRL->stFRAMENOISE.prestdev_frac;			
		
		operation_mode = pCTRL->lowlight_mode ;		
		
		offset_current = pCTRL->stM2DNR.th_offset ;
		filter_level =  pCTRL->stM2DNR.filter_level ;
		th_offset_limit =  pCTRL->stM2DNR.th_offset_limit ;
		
		// under_exp = pCTRL->stFRAMENOISE.under_exp ;
		agc_max = pCTRL->stFRAMENOISE.agc_max ;
		
		if ( pCTRL->strategy > 4 )
		{
			strategy = (pCTRL->strategy) - 4 ;
		}
		else
		{
			strategy = 0 ;
		}
		
		if ( strategy == 0 ) // ( pCTRL->strategy 0~4 )
		{
			if ( operation_mode == 0 ) // Normal light
			{	
				offset_target = 0 ;		
			}
			else // Low light
			{				
				if ( (agc_max == 1) || (stdev_int >= pCTRL->st3DNR.k_value_bound05) ) // Noise exceed max level 290:16
				{
					switch (filter_level)
					{
						// case 0 : offset_target = 0 ; break ;
						// case 1 : offset_target = DIFF(stdev_int,prestdev_int) ; break ;
						// case 2 : offset_target = (DIFF(stdev_int,prestdev_int)) + ((DIFF(stdev_int,prestdev_int))>>1) ; break ;
						// case 3 : offset_target = (DIFF(stdev_int,prestdev_int)<<1) ; break ;
						// default : offset_target = DIFF(stdev_int,prestdev_int) ; break ;		
						case 0 : offset_target = 0 ; break ;
						case 1 : offset_target = prestdev_int ; break ;
						case 2 : offset_target = (prestdev_int<<2) ; break ;
						case 3 : offset_target = (prestdev_int<<3) ; break ;
						default : offset_target = prestdev_int ; break ;					
					}
				}
				else
				{
					offset_target = 0 ;
				}			
			}
		}
		else // ( pCTRL->strategy 5~7 )
		{	
			switch (filter_level)
			{
				// case 0 : offset_target = 0 ; break ;
				// case 1 : offset_target = DIFF(stdev_int,prestdev_int) ; break ;
				// case 2 : offset_target = (DIFF(stdev_int,prestdev_int)) + ((DIFF(stdev_int,prestdev_int))>>1) ; break ;
				// case 3 : offset_target = (DIFF(stdev_int,prestdev_int)<<1) ; break ;
				// default : offset_target = DIFF(stdev_int,prestdev_int) ; break ;		
				case 0 : offset_target = 0 ; break ;
				case 1 : offset_target = prestdev_int ; break ;
				case 2 : offset_target = (prestdev_int<<2) ; break ;
				case 3 : offset_target = (prestdev_int<<3) ; break ;
				default : offset_target = prestdev_int ; break ;					
			}
		}
		
		if (offset_target > pCTRL->stM2DNR.th_offset_limit )
			offset_target = pCTRL->stM2DNR.th_offset_limit ; // limit offset_target value
		
		if ( offset_current > offset_target )
			offset_current-- ;
		else if ( offset_current < offset_target )
			offset_current++ ;		
		
		if ( (offset_current > 0) && (pCTRL->mode != MISP_NR_OFF))
		{
			SetNR_Mittlerfilter(1,gain,offset_current) ;
		}
		else
		{
			if ( pCTRL->stM2DNR.filter_en == 1 )
				SetNR_Mittlerfilter(0,gain,offset_current) ; // filter disable with no reg control				
		}
	}
//-----------------------------------------------------------------------------
	//
	//
	// i5x0 code end : NRCTRL_AUTOM2DNR
	//
	//
//-----------------------------------------------------------------------------
#endif
} // NRCTRL_AUTOM2DNR
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------

static void NRCTRL_AUTOIDM ( PNR_CTRL_PARAM pCTRL )
{
//-----------------------------------------------------------------------------
#if __NR_REV__	
//-----------------------------------------------------------------------------
	//
	//
	// i5x0 code start : NRCTRL_AUTOIDM
	//
	//
//-----------------------------------------------------------------------------	
	WORD rVal;
	WORD stdev_int ;
	WORD stdev_frac ;
	
	LONG dgain ;
	
	WORD prestdev_int ;
	WORD prestdev_frac ;	
	BOOL operation_mode ;
	
	BYTE dy_noise_limit ;
	BYTE noise_limit ;
	BYTE TH_b0 ;
	BYTE TH_b0_plus ;
	BYTE TH_b1 ;
	BYTE TH_b1_plus ;
	BYTE TH_y0 ;
	BYTE TH_y0_plus ;
	BYTE TH_y1 ;
	BYTE TH_y1_plus ;
	
	BYTE under_exp ;
	
	// if ( pCTRL->st3DNR.update_counter == 0 )
	if ( pCTRL->st3DNR.update_counter == __NR_AUTOIDM_TIMER__ )
	{
		if ( (pCTRL->ctrl&NRCTRL_AUTO_IDM_UPDATE ) == 0)
		{
			MISP_RegRead(MISP_LOCAL_ID, 0x53F, &rVal);
			pCTRL->stIDM.idm_auto = rVal&0x0001 ;			
		}
		else
		{
			pCTRL->ctrl &= (~NRCTRL_AUTO_IDM_UPDATE);	
			rVal = pCTRL->stIDM.idm_auto ;
			MISP_RegField(MISP_LOCAL_ID, 0x53F, 0, 1, rVal);
		}
		
		if ( pCTRL->stIDM.idm_auto != 0 )
		{
			operation_mode = pCTRL->lowlight_mode ;
			
			under_exp = pCTRL->stFRAMENOISE.under_exp ;
			
			stdev_int = pCTRL->stFRAMENOISE.stdev_int ;
			stdev_frac = pCTRL->stFRAMENOISE.stdev_frac ;			

			prestdev_int = pCTRL->stFRAMENOISE.prestdev_int ;
			prestdev_frac = pCTRL->stFRAMENOISE.prestdev_frac ;					

			if ( stdev_frac >= 4 ) stdev_int++ ;
			if ( prestdev_frac >= 4 ) prestdev_int++ ;			
			
			
			
			if ( operation_mode == 0 )
			{	
			#if __NR_IDMHIST_FUNC__
				if ( stdev_int > pCTRL->st3DNR.k_value_bound01 ) // Current noise level is greater then 5
				{
					if ( pCTRL->stIDM.idm_hist_en == 0 ) SetNR_IDM_mhist(1) ; // Turn on when off
				}
				else
				{
					if ( pCTRL->stIDM.idm_hist_en == 1 ) SetNR_IDM_mhist(0) ; // Turn off when on
				}					
			#endif	
				if ( stdev_int <= pCTRL->st3DNR.k_value_bound00 ) 
				{
					dy_noise_limit = 0 ;
					stdev_int = 0  ;			
				}
				else
				{
					dy_noise_limit = 1 ;
					stdev_int = DIFF(stdev_int,prestdev_int) ;
				}			
				
				if ( pCTRL->stFRAMENOISE.corr_ratio > 0 ) stdev_int = (stdev_int>>pCTRL->stFRAMENOISE.corr_ratio) ; 				
				
				if ( prestdev_int <= pCTRL->st3DNR.k_value_bound00 )
					noise_limit = stdev_int ;
				else
					noise_limit = (stdev_int>>1) + (stdev_int>>2) + (prestdev_int>>1);
								
				TH_b0 = noise_limit>>1 ;			
				TH_y0 = noise_limit>>1 ;
				
				if ( noise_limit%2 == 1 )
				{
					TH_b1 = (noise_limit>>1) + 1 ;
					TH_y1 = (noise_limit>>1) + 1 ;
				}
				else
				{
					TH_b1 = noise_limit>>1 ;
					TH_y1 = noise_limit>>1 ;				
				}
				SetNR_IDM_ratio(1,3,3);				
			}
			else
			{
			#if __NR_IDMHIST_FUNC__
				if ( pCTRL->stIDM.idm_hist_en == 0 ) SetNR_IDM_mhist(1) ; // Turn on when off				
			#endif	
				dy_noise_limit = 1 ;
				dgain = Get_AE_DGain() ;

				if ( dgain > 256 )
				{
					dgain = dgain>>7 ;
					if ( (dgain%2) == 1 ) dgain = (dgain>>1) + 1 ;
					else dgain = (dgain>>1) ;
				}
				else dgain = 0 ;				
				
				if ( stdev_int > (pCTRL->st3DNR.k_value_bound05) )  // Noise exceed max level 290:16
				{					
					TH_b0_plus = (DIFF(stdev_int,prestdev_int)>>1) ;
					if ( DIFF(stdev_int,prestdev_int) > 0 ) TH_y0_plus = (DIFF(stdev_int,prestdev_int)) - 1 ;	
					else TH_y0_plus = 0 ; // prevent minus value		
					TH_b1_plus = (DIFF(stdev_int,prestdev_int)>>1) ;				
					TH_y1_plus = 0 ;					
				}				
				else
				{
					TH_b0_plus = (DIFF(stdev_int,prestdev_int)>>0) ;
					TH_y0_plus = (DIFF(stdev_int,prestdev_int)>>1) ;				
					TH_b1_plus = (DIFF(stdev_int,prestdev_int)>>0) ;				
					TH_y1_plus = (DIFF(stdev_int,prestdev_int)>>0) ;
				}
				
				if ( stdev_int > (pCTRL->st3DNR.k_value_bound03) ) SetNR_IDM_ratio(1,4,3);	
				else SetNR_IDM_ratio(1,3,3);	
				
				if ( prestdev_int <= pCTRL->st3DNR.k_value_bound00 )
					stdev_int = DIFF(stdev_int,prestdev_int) - under_exp ;
				else
					stdev_int = DIFF(stdev_int,prestdev_int) + (prestdev_int) ;	// 	stdev_int does not change in normal condition				
				
				stdev_int = stdev_int>>1 ;					
				
				TH_b0 = (stdev_int>>1) + TH_b0_plus ;				
				TH_y0 = (stdev_int>>1) + TH_y0_plus ;
				TH_b1 = stdev_int + TH_b1_plus;
				TH_y1 = stdev_int + TH_y1_plus;					
				
				if ( pCTRL->stFRAMENOISE.corr_ratio > 0 ) 
				{
					TH_b0 = (TH_b0>>(pCTRL->stFRAMENOISE.corr_ratio)) ;
					TH_y0 = (TH_y0>>(pCTRL->stFRAMENOISE.corr_ratio)) ;
					TH_b1 = (TH_b1>>(pCTRL->stFRAMENOISE.corr_ratio)) ;
					TH_y1 = (TH_y1>>(pCTRL->stFRAMENOISE.corr_ratio)) ;
				}
				
				if ( stdev_int%2 == 1 )				
					noise_limit = (stdev_int>>1) + 1 + dgain ;
				else
					noise_limit = (stdev_int>>1) + dgain ;						
			}
			
			if ( stdev_int >= pCTRL->st3DNR.k_value_bound01 ) 
			{
				switch ( pCTRL->aperture )
				{
					case 0 : // least aperture : most motion artifact
						TH_b0 = (TH_b0 + (TH_b0>>1)) ;
						TH_y0 = (TH_y0 + (TH_y0>>1)) ;
						TH_b1 = (TH_b1 + (TH_b1>>1)) ;
						TH_y1 = (TH_y1 + (TH_y1>>1)) ;						
						break ;
					case 1 : // less aperture : more motion artifact
						TH_b0 = (TH_b0 + (TH_b0>>2)) ;
						TH_y0 = (TH_y0 + (TH_y0>>2)) ;
						TH_b1 = (TH_b1 + (TH_b1>>2)) ;
						TH_y1 = (TH_y1 + (TH_y1>>2)) ;						
					case 3 : // more aperture : less motion artifact
						TH_b0 = (TH_b0 - (TH_b0>>2)) ;
						TH_y0 = (TH_y0 - (TH_y0>>2)) ;
						TH_b1 = (TH_b1 - (TH_b1>>2)) ;
						TH_y1 = (TH_y1 - (TH_y1>>2)) ;							
						break ;
					case 4 : // most aperture : least motion artifact
						TH_b0 = (TH_b0 - (TH_b0>>1)) ;
						TH_y0 = (TH_y0 - (TH_y0>>1)) ;
						TH_b1 = (TH_b1 - (TH_b1>>1)) ;
						TH_y1 = (TH_y1 - (TH_y1>>1)) ;							
						break ;
					default : // Normal aperture : normal motion artifact
						// do nothing
						break ;
				}		
			}		
			
			SetNR_IDM(operation_mode,dy_noise_limit,noise_limit,TH_b0,TH_y0,TH_b1,TH_y1) ;
			//UARTprintf("IDM stFRAMENOISE.corr_ratio : %d\r\n",pCTRL->stFRAMENOISE.corr_ratio);
		}		
		
	}	
//-----------------------------------------------------------------------------
	//
	//
	// i5x0 code end : NRCTRL_AUTOIDM
	//
	//
//-----------------------------------------------------------------------------
#else	
//-----------------------------------------------------------------------------
	//
	//
	// i500 code start : NRCTRL_AUTOIDM
	//
	//
//-----------------------------------------------------------------------------	
	WORD rVal;
	WORD stdev_int ;
	WORD stdev_frac ;
	BOOL operation_mode ;

	BYTE dy_noise_limit ;
	BYTE TH_b0 ;
	BYTE TH_b1 ;
	BYTE TH_y0 ;
	BYTE TH_y1 ;	
	
	// if ( pCTRL->st3DNR.update_counter == 0 )
	if ( pCTRL->st3DNR.update_counter == __NR_AUTOIDM_TIMER__ )
	{
		if ( (pCTRL->ctrl&NRCTRL_AUTO_IDM_UPDATE ) == 0)
		{
			MISP_RegRead(MISP_LOCAL_ID, 0x53F, &rVal);
			pCTRL->stIDM.idm_auto = rVal&0x0001 ;			
		}
		else
		{
			pCTRL->ctrl &= (~NRCTRL_AUTO_IDM_UPDATE);	
			rVal = pCTRL->stIDM.idm_auto ;
			MISP_RegField(MISP_LOCAL_ID, 0x53F, 0, 1, rVal);
		}
		
		if ( pCTRL->stIDM.idm_auto != 0 )
		{
			operation_mode = pCTRL->lowlight_mode ;
			
			stdev_int = pCTRL->stFRAMENOISE.stdev_int ;
			stdev_frac = pCTRL->stFRAMENOISE.stdev_frac ;			
			
			if ( stdev_frac >= 4 )
				stdev_int++ ;
						
			if ( operation_mode == 0 )
			{
				TH_b0 = (BYTE)stdev_int + (BYTE)(stdev_int>>2) ;
				// TH_b1 = (BYTE)stdev_int * 3 ;
				TH_b1 = (BYTE)stdev_int<<1 ;
				TH_y0 = 0 ;
				TH_y1 = (BYTE)stdev_int - (BYTE)(stdev_int>>2) ;				
				
				dy_noise_limit = 0 ;
			}
			else
			{				
				
				//// TH_b0 = (BYTE)(stdev_int<<1) ;
				// TH_b0 = (BYTE)(stdev_int) ;
				//// TH_b1 = (BYTE)(stdev_int<<1) ;
				// TH_b1 = (BYTE)(stdev_int) ;
				// TH_y0 = (BYTE)(stdev_int>>2) ;
				// TH_y1 = (BYTE)(stdev_int>>1) ;					
				
				if ( stNRCTRL.intensity != MISP_NR3D_AUTO )	
				{				
					if ( stdev_int < pCTRL->st3DNR.k_value_bound03 )
					{
						stdev_int = (stdev_int>>1) + (stdev_int>>2);
						dy_noise_limit = 0 ;
						// TH_b0 = (BYTE)(stdev_int<<1) ;
						TH_b0 = (BYTE)(stdev_int) ;
						// TH_b1 = (BYTE)(stdev_int<<1) ;
						TH_b1 = (BYTE)(stdev_int) ;
						TH_y0 = (BYTE)(stdev_int>>2) ;
						TH_y1 = (BYTE)(stdev_int>>1) ;	
						stdev_int = 0 ;
					}
					else
					{ 
						// stdev_int = (stdev_int>>1) + (stdev_int>>2);
						stdev_int = (stdev_int>>1) ;
						dy_noise_limit = 1 ;
						// TH_b0 = (BYTE)(stdev_int<<1) ;
						TH_b0 = (BYTE)(stdev_int) ;
						// TH_b1 = (BYTE)(stdev_int<<1) ;
						TH_b1 = (BYTE)(stdev_int) ;
						TH_y0 = (BYTE)(stdev_int>>2) ;
						TH_y1 = (BYTE)(stdev_int>>1) ;						
					}				
				}
				else
				{
						// stdev_int = (stdev_int>>1) + (stdev_int>>2);
						stdev_int = (stdev_int>>1) ;
						dy_noise_limit = 1 ;
						TH_b0 = (BYTE)(stdev_int<<1) ;
						// TH_b0 = (BYTE)(stdev_int) ;
						TH_b1 = (BYTE)(stdev_int<<1) ;
						// TH_b1 = (BYTE)(stdev_int) ;
						TH_y0 = (BYTE)(stdev_int>>2) ;
						TH_y1 = (BYTE)(stdev_int>>1) ;					
				}
			}	
			
			SetNR_IDM(operation_mode,dy_noise_limit,stdev_int,TH_b0,TH_y0,TH_b1,TH_y1) ;			
		}		
		
	}
//-----------------------------------------------------------------------------
	//
	//
	// i500 code end : NRCTRL_AUTOIDM
	//
	//
//-----------------------------------------------------------------------------		
#endif	
} // NRCTRL_AUTOIDM
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------

static void NRCTRL_AUTOPDM ( PNR_CTRL_PARAM pCTRL )
{
//-----------------------------------------------------------------------------
#if __NR_REV__	
//-----------------------------------------------------------------------------
	//
	//
	// i5x0 code start : NRCTRL_AUTOPDM
	//
	//
//-----------------------------------------------------------------------------	
	WORD rVal;
	WORD stdev_int ;
	WORD stdev_frac ;
	
	WORD stdev_int_d ;
	WORD stdev_int_t ;
	WORD stdev_int_b ;	
	
	WORD prestdev_int ;
	WORD prestdev_frac ;	
	BOOL operation_mode ;
	
	BOOL dy_noise_limit ;
	WORD noise_limit ;
	
	BYTE motion_plus ;
	BYTE motion_th ;
	BYTE motion_adj ;
	
	BYTE current_K ;
	BYTE motion_K ;
	BYTE motion_neutral_K ;
	
	BYTE mhist_thres ;
	BYTE mhist_k00 ;
	BYTE mhist_k01 ;
	BYTE mhist_k02 ;
	
	BYTE under_exp ;
	LONG dgain ;
	
	// if ( pCTRL->st3DNR.update_counter == 0 )
	if ( pCTRL->st3DNR.update_counter == __NR_AUTOPDM_TIMER__ )
	{
		if ( (pCTRL->ctrl&NRCTRL_AUTO_PDM_UPDATE ) == 0)
		{
			MISP_RegRead(MISP_LOCAL_ID, 0x53F, &rVal);
			pCTRL->stPDM.pdm_auto = rVal&0x0002 ;			
		}
		else
		{
			pCTRL->ctrl &= (~NRCTRL_AUTO_PDM_UPDATE);	
			rVal = pCTRL->stPDM.pdm_auto ;
			MISP_RegField(MISP_LOCAL_ID, 0x53F, 1, 1, rVal);
		}

		if ( pCTRL->stPDM.pdm_auto != 0 )
		{
			operation_mode = pCTRL->lowlight_mode ;			
			under_exp = pCTRL->stFRAMENOISE.under_exp ;
			motion_adj = 0 ;

			if ( operation_mode == 0 )
			{
				stdev_int = pCTRL->stFRAMENOISE.stdev_int ;
				stdev_frac = pCTRL->stFRAMENOISE.stdev_frac ;	
			}
			else
			{
				stdev_int_d = (pCTRL->stFRAMENOISE.stdev_int_da<<3)|(pCTRL->stFRAMENOISE.stdev_frac_da);
				stdev_int_t = (pCTRL->stFRAMENOISE.stdev_int<<3)|(pCTRL->stFRAMENOISE.stdev_frac);
				stdev_int_b = (pCTRL->stFRAMENOISE.stdev_int_br<<3)|(pCTRL->stFRAMENOISE.stdev_frac_br);
								
				if ( under_exp > 0 ) { // under exp state
					if ( stdev_int_d > stdev_int_t ) {
						stdev_int = pCTRL->stFRAMENOISE.stdev_int_da ;
						stdev_frac = pCTRL->stFRAMENOISE.stdev_frac_da ;
					}
					else{
						stdev_int = pCTRL->stFRAMENOISE.stdev_int ;
						stdev_frac = pCTRL->stFRAMENOISE.stdev_frac ;					
					}
				}
				else{
					stdev_int = pCTRL->stFRAMENOISE.stdev_int_da ;
					stdev_frac = pCTRL->stFRAMENOISE.stdev_frac_da ;				
				}					
			}
			
			prestdev_int = pCTRL->stFRAMENOISE.prestdev_int ;
			prestdev_frac = pCTRL->stFRAMENOISE.prestdev_frac ;			
			current_K = GetNR_3Dconst_k() ;
			
			if ( stdev_frac >= 4 ) stdev_int++ ;
			if ( prestdev_frac >= 4 ) prestdev_int++ ;			
			
			if ( stdev_int < pCTRL->st3DNR.k_value_bound00 ) // 3
				{ motion_K = 34 ; motion_neutral_K = 32 ; mhist_k00 = 64 ; mhist_k01 = current_K ; mhist_k02 = current_K ; mhist_thres = 4 ;}
			else if ( stdev_int < pCTRL->st3DNR.k_value_bound01 ) // 5
				{ motion_K = 32 ; motion_neutral_K = 30 ; mhist_k00 = 64 ; mhist_k01 = current_K ; mhist_k02 = current_K ; mhist_thres = 4 ;}				
			else if ( stdev_int < pCTRL->st3DNR.k_value_bound02 ) // 7
				{ motion_K = 32 ; motion_neutral_K = 30 ; mhist_k00 = 64 ; mhist_k01 = current_K ; mhist_k02 = current_K ; mhist_thres = 4 ;}	
			else if ( stdev_int < pCTRL->st3DNR.k_value_bound03 ) // 10
				{ motion_K = 30 ; motion_neutral_K = 28 ; mhist_k00 = 64 ; mhist_k01 = current_K ; mhist_k02 = current_K ; mhist_thres = 5 ;}	
			else if ( stdev_int < pCTRL->st3DNR.k_value_bound04 ) // 13
				{ motion_K = 28 ; motion_neutral_K = 26 ; mhist_k00 = 30 ; mhist_k01 = current_K ; mhist_k02 = current_K ; mhist_thres = 5 ;}	
			else if ( stdev_int < pCTRL->st3DNR.k_value_bound05 ) // 16
				{ motion_K = 28 ; motion_neutral_K = 26 ; mhist_k00 = 28 ; mhist_k01 = current_K ; mhist_k02 = current_K ; mhist_thres = 6 ;}	
			else
				{ motion_K = 28 ; motion_neutral_K = 26 ; mhist_k00 = 20 ; mhist_k01 = current_K ; mhist_k02 = current_K ; mhist_thres = 6 ;}		
			
			SetNR_PDMmotionK(operation_mode,motion_K,motion_neutral_K) ;
			SetNR_PDM_mhist_K(mhist_k00, mhist_k01, mhist_k02) ;
			SetNR_PDM_mhistTH(mhist_thres);	
			
			if ( operation_mode == 0 )
			{
				
				dy_noise_limit = 1 ; // Chip Auto control enable
				
				if ( stdev_int <= pCTRL->st3DNR.k_value_bound00 )  // 3
				{
					noise_limit = (stdev_int) ;
					motion_th = 2 ;
				}
				else
				{
					if ( prestdev_int <= pCTRL->st3DNR.k_value_bound00 ) 
					{
						noise_limit = ((DIFF(stdev_int,prestdev_int))>>1) ;
						motion_plus = 3 + (prestdev_int>>1) ;
						if ( motion_plus > (noise_limit>>1) ) motion_th = motion_plus - (noise_limit>>1) ;
						else motion_th = 0 ;
						// motion_th = 3 - (noise_limit>>1) + (prestdev_int>>1) ;
					}
					else
					{
						noise_limit = ((DIFF(stdev_int,prestdev_int))>>1) + (prestdev_int>>1);
						motion_plus = 3 + (prestdev_int>>1) ;
						if ( motion_plus > (noise_limit>>1) ) motion_th = motion_plus - (noise_limit>>1) ;
						else motion_th = 0 ;						
						// motion_th = 3 - (noise_limit>>1) + (prestdev_int>>1) ;
					}					
				}				
				
				if ( pCTRL->stFRAMENOISE.corr_ratio > 0 )
				{
					noise_limit = (noise_limit>>pCTRL->stFRAMENOISE.corr_ratio) ;
					motion_th = (motion_th>>pCTRL->stFRAMENOISE.corr_ratio) ;
				}
								
				// if ( (motion_th&0x80) == 1 ) // if motion_th is minus value
					// motion_th = 0 ;
				
				SetNR_PDM_replace(0,0) ;
				
			}
			else
			{
				dgain = Get_AE_DGain() ;
				if ( dgain > 256 )
				{
					dgain = dgain>>7 ;
					if ( (dgain%2) == 1 ) dgain = (dgain>>1) + 1 ;
					else dgain = (dgain>>1) ;
				}
				else dgain = 0 ;					
				
				dy_noise_limit = 1 ; // Chip Auto control enable	
				
				if ( prestdev_int <= pCTRL->st3DNR.k_value_bound00 )
					noise_limit = ((DIFF(stdev_int,prestdev_int))>>1) + dgain ;
				else
					noise_limit = ((DIFF(stdev_int,prestdev_int))>>1) + (prestdev_int>>1) + dgain ;
				
				if  ( pCTRL->image_type == MISP_NR_YC_IMAGE )
				{
					if ( noise_limit%2 == 1 )
						noise_limit = (noise_limit>>1) + 1 ;
					else
						noise_limit = (noise_limit>>1) ;
					
					motion_th = 0 ;
				}
				else
				{
					if ( stdev_int > (pCTRL->st3DNR.k_value_bound05) )  // Noise exceed max level 290:16
					{
						// motion_th = 4 + (noise_limit>>1) + (noise_limit>>2);	
						motion_th = 2 + (noise_limit>>1) + (stdev_int>>1);	
					}
					else
					{
						// if ( noise_limit > 3 )
							// motion_th = 3 ;	
						if ( noise_limit > 4 )
							motion_th = 4 ;							
						else
							motion_th = noise_limit ;					
					}				
				}
				
				if ( pCTRL->stFRAMENOISE.corr_ratio > 0 )
				{
					noise_limit = (noise_limit>>pCTRL->stFRAMENOISE.corr_ratio) ;
					motion_th = (motion_th>>pCTRL->stFRAMENOISE.corr_ratio) ;	
				}
				
				if ( stdev_int >= pCTRL->st3DNR.k_value_bound03 )  // 10
					SetNR_PDM_replace(3,3) ;
				else
					SetNR_PDM_replace(0,0) ;				
			}	
			
			if ( stdev_int >= pCTRL->st3DNR.k_value_bound01 ) 
			{
				switch ( pCTRL->aperture )
				{
					case 0 : // least aperture : most motion artifact
						motion_th = (motion_th + (motion_th>>1)) ;
						break ;
					case 1 : // less aperture : more motion artifact
						motion_th = (motion_th + (motion_th>>2)) ;
					case 3 : // more aperture : less motion artifact
						motion_th = (motion_th - (motion_th>>2)) ;
						break ;
					case 4 : // most aperture : least motion artifact
						motion_th = (motion_th - (motion_th>>1)) ;
						break ;
					default : // Normal aperture : normal motion artifact
						// do nothing
						break ;
				}				
			}			
			
			SetNR_PDMratio(operation_mode, MIN((24+operation_mode+stdev_int),32)) ;
			SetNR_PDM(operation_mode,dy_noise_limit,noise_limit,motion_th,motion_adj);
			//UARTprintf("PDM stFRAMENOISE.corr_ratio : %d\r\n",pCTRL->stFRAMENOISE.corr_ratio);
		}
	}
//-----------------------------------------------------------------------------
	//
	//
	// i5x0 code end : NRCTRL_AUTOPDM
	//
	//
//-----------------------------------------------------------------------------	
#else
//-----------------------------------------------------------------------------
	//
	//
	// i500 code start : NRCTRL_AUTOPDM
	//
	//
//-----------------------------------------------------------------------------	
	
	WORD rVal;
	WORD stdev_int ;
	WORD stdev_frac ;
	WORD stdev_noise_limit ;
	BOOL operation_mode ;
	
	BOOL dy_noise_limit ;
	WORD noise_limit ;
	
	BYTE motion_th ;
	BYTE motion_adj ;
	
	BYTE motion_K ;
	BYTE motion_nuetral_K ;

	// if ( pCTRL->st3DNR.update_counter == 0 )
	if ( pCTRL->st3DNR.update_counter == __NR_AUTOPDM_TIMER__ )
	{
		if ( (pCTRL->ctrl&NRCTRL_AUTO_PDM_UPDATE ) == 0)
		{
			MISP_RegRead(MISP_LOCAL_ID, 0x53F, &rVal);
			pCTRL->stPDM.pdm_auto = rVal&0x0002 ;			
		}
		else
		{
			pCTRL->ctrl &= (~NRCTRL_AUTO_PDM_UPDATE);	
			rVal = pCTRL->stPDM.pdm_auto ;
			MISP_RegField(MISP_LOCAL_ID, 0x53F, 1, 1, rVal);
		}

		if ( pCTRL->stPDM.pdm_auto != 0 )
		{
			operation_mode = pCTRL->lowlight_mode ;
			
			stdev_int = pCTRL->stFRAMENOISE.stdev_int ;
			stdev_frac = pCTRL->stFRAMENOISE.stdev_frac ;			
			motion_adj = 0 ;
			
			if ( stdev_int < pCTRL->st3DNR.k_value_bound00 ) // 3
				{ motion_K = 34 ; motion_nuetral_K = 32 ; }
			else if ( stdev_int < pCTRL->st3DNR.k_value_bound01 ) // 5
				{ motion_K = 32 ; motion_nuetral_K = 30 ; }				
			else if ( stdev_int < pCTRL->st3DNR.k_value_bound02 ) // 7
				{ motion_K = 32 ; motion_nuetral_K = 30 ; }	
			else if ( stdev_int < pCTRL->st3DNR.k_value_bound03 ) // 10
				{ motion_K = 30 ; motion_nuetral_K = 28 ; }	
			else if ( stdev_int < pCTRL->st3DNR.k_value_bound04 ) // 13
				{ motion_K = 28 ; motion_nuetral_K = 26 ; }	
			else
				{ motion_K = 28 ; motion_nuetral_K = 26 ;}		
			
			SetNR_PDMmotionK(operation_mode,motion_K,motion_nuetral_K) ;
				
			if ( operation_mode == 0 )
			{
				
				if ( stdev_frac >= 3 || (stdev_int&0x0001) == 1 ) motion_th = 1 ;
				else motion_th = 0 ;
				
				dy_noise_limit = 0 ;
				noise_limit = (stdev_int>>1) ;
				
				if ( stdev_frac >= 4 ) stdev_int++ ;
		
			}
			else
			{
				if ( stdev_frac >= 4 || (stdev_int&0x0001) == 1 ) motion_th = 1 ;
				else motion_th = 0 ;	
				
				dy_noise_limit = 1 ;
				
				noise_limit = (stdev_int>>2) + (stdev_int>>3) ;
				
				if ( stdev_frac >= 4 ) stdev_int++ ;
			}
			
			SetNR_PDMratio(operation_mode, MIN((24+operation_mode+stdev_int),32)) ;
			SetNR_PDM(operation_mode,dy_noise_limit,noise_limit,motion_th,motion_adj);
		}
	}	
//-----------------------------------------------------------------------------
	//
	//
	// i500 code end : NRCTRL_AUTOPDM
	//
	//
//-----------------------------------------------------------------------------		
#endif	
} // NRCTRL_AUTOPDM
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
	
static void NRCTRL_AUTOSRM ( PNR_CTRL_PARAM pCTRL )
{	
//-----------------------------------------------------------------------------
#if __NR_REV__	
//-----------------------------------------------------------------------------
	//
	//
	// i5x0 code start : NRCTRL_AUTOSRM
	//
	//
//-----------------------------------------------------------------------------
	WORD rVal;
	// WORD stdev ;
	WORD stdev_int ;
	WORD stdev_frac ;
	// WORD prestdev ;
	WORD prestdev_int ;
	WORD prestdev_frac ;
	WORD abs_stdev ;
	WORD abs_stdev_frac ;
	BOOL operation_mode ;
	
	BOOL pdm_corr_en ;
	
	BYTE motion_th_int ;
	BYTE motion_th_frac ;
	BYTE mi_corr_value ;
	BYTE under_exp ;
	
	// if ( pCTRL->st3DNR.update_counter == 0 )
	if ( pCTRL->st3DNR.update_counter == __NR_AUTOSRM_TIMER__ )
	{
		if ( (pCTRL->ctrl&NRCTRL_AUTO_SRM_UPDATE ) == 0)
		{
			MISP_RegRead(MISP_LOCAL_ID, 0x53F, &rVal);
			pCTRL->stSRM.srm_auto = rVal&0x0004 ;			
		}
		else
		{
			pCTRL->ctrl &= (~NRCTRL_AUTO_SRM_UPDATE);	
			rVal = pCTRL->stSRM.srm_auto ;
			MISP_RegField(MISP_LOCAL_ID, 0x53F, 3, 1, rVal);
		}
		
		if ( pCTRL->stSRM.srm_auto != 0 )
		{
			operation_mode = pCTRL->lowlight_mode ;
			
			stdev_int = pCTRL->stFRAMENOISE.stdev_int ;
			stdev_frac = pCTRL->stFRAMENOISE.stdev_frac ;
			prestdev_int = pCTRL->stFRAMENOISE.prestdev_int ;
			prestdev_frac = pCTRL->stFRAMENOISE.prestdev_frac ;					
			under_exp = pCTRL->stFRAMENOISE.under_exp ;
			
			if ( operation_mode == 0 ) 
			{

				if ( stdev_int == prestdev_int ) 
				{
					abs_stdev = 0 ;
					if ( stdev_frac > prestdev_frac )
					{
						abs_stdev_frac = stdev_frac - prestdev_frac ;
					}
					else
					{
						abs_stdev_frac = prestdev_frac - stdev_frac ;
					}					
				}
				else if ( stdev_int > prestdev_int ) 
				{
					if ( stdev_frac > prestdev_frac )
					{
						abs_stdev = stdev_int - prestdev_int ;
						abs_stdev_frac = stdev_frac - prestdev_frac ;
					}
					else
					{
						abs_stdev = stdev_int - prestdev_int -1 ;
						abs_stdev_frac = 7 + stdev_frac - prestdev_frac ;
					}
				}
				else                    
				{
					if ( prestdev_frac > stdev_frac )
					{
						abs_stdev = prestdev_int - stdev_int -1 ;
						abs_stdev_frac = 7 + prestdev_frac - stdev_frac ;
					}
					else
					{
						abs_stdev = prestdev_int - stdev_int ;
						abs_stdev_frac = stdev_frac - prestdev_frac ;
					}
				}

								
				if ( abs_stdev < pCTRL->st3DNR.k_value_bound00 )
				{
					motion_th_int = (abs_stdev>>1) ;
					motion_th_frac = (abs_stdev_frac<<1) ;
				}
				else
				{
					motion_th_int = abs_stdev - pCTRL->st3DNR.k_value_bound00 ;
					motion_th_frac = (abs_stdev_frac<<1) ;
				}					
				
				// motion_th_int = 0 ;
				// motion_th_int = (stdev_int) ;
								
				// motion_th_frac = stdev_frac ;				
				// motion_th_frac = (stdev_frac<<1) ;
				
				if ( pCTRL->stFRAMENOISE.corr_ratio > 0 ) stdev_int = (stdev_int>>pCTRL->stFRAMENOISE.corr_ratio) ;
				
				if ( stdev_int < pCTRL->st3DNR.k_value_bound01 ) mi_corr_value = 2 ; // 5
				else mi_corr_value = 1 ;
				
				pdm_corr_en = 0 ;
				
			}
			else // When Low light mode
			{
				if ( under_exp > 0 )
				{	
					motion_th_int = stdev_int - pCTRL->stLL.ent_level ;			
					
					if ( motion_th_int > 2 )
						motion_th_frac = (stdev_frac<<1) ;						
					else
						motion_th_frac = 0 ;					
					
					if ( motion_th_int < 3 && stdev_frac > 4 )
						motion_th_int++ ;
					else
						motion_th_int = 3 ;	
				
					if ( motion_th_int < 2 )
						mi_corr_value = motion_th_int + 1 ;
					else
						mi_corr_value = 2 ;				
				}
				else
				{
					if ( prestdev_int <= pCTRL->st3DNR.k_value_bound00 )
					{
						motion_th_int = 0 ;
						motion_th_frac = prestdev_frac ;
						
						mi_corr_value = 1 ;	
					}
					else
					{
						motion_th_int = prestdev_int>>1 ;
						motion_th_frac = prestdev_frac ;
						
						mi_corr_value = 1 ;						
					}
				}
				
				if ( stdev_int >= pCTRL->st3DNR.k_value_bound04 || under_exp > 0 ) // 13
					pdm_corr_en = 1 ;
				else
					pdm_corr_en = 0 ;
				
				
				if ( stdev_frac >= 4 ) stdev_int++ ;
				if ( stdev_int > (pCTRL->st3DNR.k_value_bound05) )  // Noise exceed max level 290:16
				{
					stdev_int = stdev_int - 16 ;
					stdev_int = (stdev_int>>2);
					motion_th_int = DIFF(motion_th_int,stdev_int) ;
				}
				
			} // if ( operation_mode == 1 ) // When Low light mode
			
			SetNR_SRM_corr(operation_mode,pdm_corr_en,1,mi_corr_value);
			SetNR_SRM(operation_mode,1,motion_th_int,motion_th_frac) ;
			
			//UARTprintf("SRM stFRAMENOISE.corr_ratio : %d\r\n",pCTRL->stFRAMENOISE.corr_ratio);
			
		}		
	}
//-----------------------------------------------------------------------------
	//
	//
	// i5x0 code end : NRCTRL_AUTOSRM
	//
	//
//-----------------------------------------------------------------------------	
#else
//-----------------------------------------------------------------------------
	//
	//
	// i500 code start : NRCTRL_AUTOSRM
	//
	//
//-----------------------------------------------------------------------------	

	WORD rVal;
	WORD stdev_int ;
	WORD stdev_frac ;
	BOOL operation_mode ;
	
	BYTE motion_th_int ;
	BYTE motion_th_frac ;
	
	// if ( pCTRL->st3DNR.update_counter == 0 )
	if ( pCTRL->st3DNR.update_counter == __NR_AUTOSRM_TIMER__ )
	{
		if ( (pCTRL->ctrl&NRCTRL_AUTO_SRM_UPDATE ) == 0)
		{
			MISP_RegRead(MISP_LOCAL_ID, 0x53F, &rVal);
			pCTRL->stSRM.srm_auto = rVal&0x0004 ;			
		}
		else
		{
			pCTRL->ctrl &= (~NRCTRL_AUTO_SRM_UPDATE);	
			rVal = pCTRL->stSRM.srm_auto ;
			MISP_RegField(MISP_LOCAL_ID, 0x53F, 3, 1, rVal);
		}
		
		if ( pCTRL->stSRM.srm_auto != 0 )
		{
			operation_mode = pCTRL->lowlight_mode ;
			
			stdev_int = pCTRL->stFRAMENOISE.stdev_int ;
			stdev_frac = pCTRL->stFRAMENOISE.stdev_frac ;
			
			if ( operation_mode == 1 ) // When Low light mode
			{
				motion_th_int = stdev_int - pCTRL->stLL.ent_level ;				
				
				if ( motion_th_int == 0 )
				{
					motion_th_frac = stdev_frac>>1 ;
					
					SetNR_SRM(operation_mode,1,motion_th_int,motion_th_frac) ;
				}
				else 
				{
					motion_th_int = motion_th_int>>1 ;
					
					motion_th_frac = stdev_frac ;
					
					if ( motion_th_int >= 3 )
						SetNR_SRM(operation_mode,1,(128-(3)),motion_th_frac) ;
					else
						SetNR_SRM(operation_mode,1,(128-(motion_th_int)),motion_th_frac) ;
				}
			} // if ( operation_mode == 1 ) // When Low light mode

		}		
	}	
//-----------------------------------------------------------------------------
	//
	//
	// i500 code end : NRCTRL_AUTOSRM
	//
	//
//-----------------------------------------------------------------------------		
#endif	
} // NRCTRL_AUTOSRM	
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------

static void NRCTRL_AUTO2DNR ( PNR_CTRL_PARAM pCTRL )	
{
//-----------------------------------------------------------------------------
#if __NR_REV__
//-----------------------------------------------------------------------------
	//
	//
	// i5x0 code start : NRCTRL_AUTO2DNR
	//
	//
//-----------------------------------------------------------------------------	
	WORD rVal ;
	WORD is_auto_2Dsfilter ;
	WORD stdev_int ;
	WORD stdev_frac ;
	WORD prestdev_int ;
	WORD prestdev_frac ;
	WORD sfilter_target ;
	WORD sfilter_target_df = 0 ;
	
	WORD strategy ;
	
	BOOL operation_mode ;
	BYTE filter_serial ;
	BYTE c_filter_serial ;
	BYTE under_exp ;
	BYTE imp_level_init ;
	BYTE imp_level ;
	BYTE imp_thresh ;
	BYTE imp_replace ;
	BYTE fs_counter ;
	
	BYTE nf_bypass_level ;
	BYTE stdev_bypass_level ;
	BYTE noise_bound_table ;
	BYTE noise_bound_mode ;
	BYTE noise_bound_mode_df ;
	BYTE filter_mode = 0 ;
	
	if ((pCTRL->mode == MISP_NR_OFF )) return ; // Runs only when NR_ON on MENU 
	
	if ( pCTRL->st3DNR.update_counter == __NR_AUTO2DNR_TIMER__ )
	{
		MISP_RegRead(MISP_LOCAL_ID, 0x53F, &rVal);
		is_auto_2Dsfilter = rVal&0x0008 ;				
	
		if ( is_auto_2Dsfilter == 0 )
		{	// 2DNR manual control
		
			sfilter_target = ((rVal&0x0E00)>>9) ;
			rVal = ((rVal&0x3000)>>12) ;
			
			if ( sfilter_target > 7 )
				sfilter_target = 7 ;			
			
			if ( rVal == 0 )
			{
				if ( pCTRL->st2DNR.sFilter_sigma != sfilter_target )
				{
					SetNR_2D_sFilter(sfilter_target,rVal) ;
				}
			}
			else if ( rVal == 2 )
			{
				if ( pCTRL->st2DNR.sFilter_sigma_df != sfilter_target )
				{
					SetNR_2D_sFilter(sfilter_target,rVal) ;
				}				
			}
			else
			{
				if ( pCTRL->st2DNR.c_sFilter_sigma != sfilter_target )
				{
					SetNR_2D_sFilter(sfilter_target,rVal) ;
				}		
			}
		}
		else
		{	// 2DNR auto control
			operation_mode = pCTRL->lowlight_mode ;
			stdev_int = pCTRL->stFRAMENOISE.stdev_int ;
			stdev_frac = pCTRL->stFRAMENOISE.stdev_frac ;
			prestdev_int = pCTRL->stFRAMENOISE.prestdev_int ;
			prestdev_frac = pCTRL->stFRAMENOISE.prestdev_frac ;
			under_exp = pCTRL->stFRAMENOISE.under_exp ;
			fs_counter = pCTRL->st2DNR.fs_counter ;
			imp_level_init = pCTRL->st2DNR.rold_impulse_init ;
			
			if ( pCTRL->strategy >= 4)
			{
				strategy = (pCTRL->strategy>>1) + ((pCTRL->strategy>>1)%2) ;
			}
			else
			{
				strategy = 0  ;			
			}
			
			if ( stdev_frac >= 4  ) stdev_int++ ;			
			if ( prestdev_frac >= 4  ) prestdev_int++ ;			
			
			if ( pCTRL->image_type == MISP_NR_YC_IMAGE ) 
			{
				if ( stdev_int < pCTRL->st3DNR.k_value_bound00 ) { sfilter_target = 1 ; sfilter_target_df = 2 ; noise_bound_table = 2 ; noise_bound_mode = 1 ; noise_bound_mode_df = 2 ; }	// 3
				else if ( stdev_int < pCTRL->st3DNR.k_value_bound01 ) { sfilter_target = 2 ; sfilter_target_df = 3 ; noise_bound_table = 2 ; noise_bound_mode = 1 ; noise_bound_mode_df = 2 ;}	// 5
				else if ( stdev_int < pCTRL->st3DNR.k_value_bound02 ) { sfilter_target = 2 ; sfilter_target_df = 4 ; noise_bound_table = 2 ; noise_bound_mode = 1 ; noise_bound_mode_df = 2 ;}	// 7
				else if ( stdev_int < pCTRL->st3DNR.k_value_bound03 ) { sfilter_target = 3 ; sfilter_target_df = 4 ; noise_bound_table = 2 ; noise_bound_mode = 0 ; noise_bound_mode_df = 3 ;}	// 10
				else if ( stdev_int < pCTRL->st3DNR.k_value_bound04 ) { sfilter_target = 3 ; sfilter_target_df = 5 ; noise_bound_table = 2 ; noise_bound_mode = 0 ; noise_bound_mode_df = 3 ;}	// 13
				else if ( stdev_int < pCTRL->st3DNR.k_value_bound05 ) { sfilter_target = 4 ; sfilter_target_df = 5 ; noise_bound_table = 2 ; noise_bound_mode = 0 ; noise_bound_mode_df = 3 ;}	// 16
				else 											{ sfilter_target = 4 ; sfilter_target_df = 6 ; noise_bound_table = 2 ; noise_bound_mode = 0 ; noise_bound_mode_df = 3 ;}
			}
			else // pCTRL->image_type == MISP_NR_BAYER_IMAGE
			{
				if ( stdev_int < pCTRL->st3DNR.k_value_bound00 ) { sfilter_target = 1 ; sfilter_target_df = 1 ; noise_bound_table = 2 ; noise_bound_mode = 1 ;noise_bound_mode_df = 1 ; }	// 3
				else if ( stdev_int < pCTRL->st3DNR.k_value_bound01 ) { sfilter_target = 2 ; sfilter_target_df = 2 ; noise_bound_table = 2 ; noise_bound_mode = 1 ; noise_bound_mode_df = 1 ;}	// 5
				else if ( stdev_int < pCTRL->st3DNR.k_value_bound02 ) { sfilter_target = 3 ; sfilter_target_df = 3 ; noise_bound_table = 1 ; noise_bound_mode = 1 ; noise_bound_mode_df = 2 ;}	// 7
				else if ( stdev_int < pCTRL->st3DNR.k_value_bound03 ) { sfilter_target = 4 ; sfilter_target_df = 4 ; noise_bound_table = 0 ; noise_bound_mode = 2 ; noise_bound_mode_df = 2 ;}	// 10
				else if ( stdev_int < pCTRL->st3DNR.k_value_bound04 ) { sfilter_target = 6 ; sfilter_target_df = 6 ; noise_bound_table = 0 ; noise_bound_mode = 2 ; noise_bound_mode_df = 3 ;}	// 13
				else if ( stdev_int < pCTRL->st3DNR.k_value_bound05 ) { sfilter_target = 7 ; sfilter_target_df = 7 ; noise_bound_table = 0 ; noise_bound_mode = 2 ; noise_bound_mode_df = 3 ;}	// 16
				else 											{ sfilter_target = 7 ; sfilter_target_df = 8 ; noise_bound_table = 0 ; noise_bound_mode = 2 ; noise_bound_mode_df = 3 ;}			
			}

			if ( stdev_int < 10 ) imp_thresh = 30-(3*stdev_int) ;
			else imp_thresh = 3 ;	// if imp_thresh is less then 3 or minus value			
			
			// Noise exceed max level 290:16
			if ( stdev_int > (pCTRL->st3DNR.k_value_bound05) ) // 16
			{
				imp_level = imp_level_init - (stdev_int>>2) ;				
			}
			else 
			{
				imp_level = imp_level_init ;				
			}				
			
			// if ( stdev_int > (pCTRL->st3DNR.k_value_bound04) ) filter_mode = 1 ; // 10
			// else filter_mode = 0 ;
			
			SetNR_2D_Impulselevel(imp_level) ;
			
			// The impulse signal 3D replace
			if ( stdev_int < pCTRL->st3DNR.k_value_bound02 )	imp_replace = 2 ;
			else 										imp_replace = 3 ;				
			
			if ( imp_replace == pCTRL->st2DNR.impulse_replace ) {
				if ( operation_mode == 0 && imp_thresh != pCTRL->st2DNR.rold_impulse_th ) SetNR_2D_ImpulseThresh(operation_mode,imp_thresh,0,imp_replace) ;				
				else if ( operation_mode == 1 && imp_thresh != pCTRL->st2DNR.rold_ll_impulse_th ) SetNR_2D_ImpulseThresh(operation_mode,imp_thresh,0,imp_replace) ;
			}
			else{
				SetNR_2D_ImpulseThresh(operation_mode,imp_thresh,0,imp_replace) ;
			}
			
			if ( stdev_int < pCTRL->st3DNR.k_value_bound01 ) // 5
			{
				nf_bypass_level = 0 ;
				stdev_bypass_level = 0 ;
			}
			else
			{
				nf_bypass_level = 1 ;
				stdev_bypass_level = 1 ;
			}			
			
			if ( nf_bypass_level != pCTRL->st2DNR.nf_bypass_level ||  
				stdev_bypass_level != pCTRL->st2DNR.stdev_bypass_level ) SetNR_2D_Bypasslevel(nf_bypass_level,stdev_bypass_level) ;
			
		
			if ( (pCTRL->mode == MISP_NR_2D3D ) || (pCTRL->mode == MISP_NR_3D))
			{
				if ( sfilter_target != pCTRL->st2DNR.sFilter_sigma ) SetNR_2D_sFilter(sfilter_target,0) ; // 2D
				if ( sfilter_target_df != pCTRL->st2DNR.sFilter_sigma_df ) SetNR_2D_sFilter(sfilter_target_df,2) ; // 2Df	
				
				if ( (noise_bound_mode != pCTRL->st2DNR.noise_bound_mode) || (noise_bound_table != pCTRL->st2DNR.noise_bound_no)) SetNR_2D_NoiseBound(noise_bound_table,0,noise_bound_mode); // 2D : Table no , filter select , nb mode // Bigger the table number, weaker the range LUT		
				// if ( noise_bound_mode != pCTRL->st2DNR.noise_bound_mode ) SetNR_2D_NoiseBound(0,0,noise_bound_mode); // 2D : Table no , filter select , nb mode // Bigger the table number, weaker the range LUT		
				if ( noise_bound_mode_df != pCTRL->st2DNR.noise_bound_mode_df ) SetNR_2D_NoiseBound(0,1,noise_bound_mode_df); // 2Df	
			}
			else
			{
				switch( pCTRL->intensity_2D ) 
				{
					case 0 :   sfilter_target = (sfilter_target*2 + sfilter_target_df*2 +1 )/4 +1 ; break ; // auto
					case 1 :   sfilter_target = sfilter_target -1 ; break ; // weak
					case 2 :   sfilter_target = sfilter_target_df -1 ; break ; // mid
					case 3 :   sfilter_target = (sfilter_target*2 + sfilter_target_df*2 +1 )/4 +2 ; break ; // high
					default : sfilter_target = (sfilter_target*2 + sfilter_target_df*2 +1 )/4 ; break ;
				}				
				
				if (sfilter_target > 7 ) sfilter_target = 7 ;
				
				if ( sfilter_target != pCTRL->st2DNR.sFilter_sigma ) SetNR_2D_sFilter(sfilter_target,0) ;		// 2D
				if ( sfilter_target != pCTRL->st2DNR.sFilter_sigma_df ) SetNR_2D_sFilter(sfilter_target,2) ;		// 2Df		
				
				if ( (noise_bound_mode != pCTRL->st2DNR.noise_bound_mode) || (noise_bound_table != pCTRL->st2DNR.noise_bound_no)) SetNR_2D_NoiseBound(noise_bound_table,0,noise_bound_mode); // 2D
				if ( noise_bound_mode != pCTRL->st2DNR.noise_bound_mode_df ) SetNR_2D_NoiseBound(1,1,noise_bound_mode); // 2Df						
			}

			if ( pCTRL->image_type == MISP_NR_YC_IMAGE )
				if ( sfilter_target_df != pCTRL->st2DNR.c_sFilter_sigma ) SetNR_2D_sFilter(sfilter_target_df,1) ;		// 2Dc
				
			
			// Filter serialize option		
			if ( (pCTRL->mode == MISP_NR_2D3D ) && sfilter_target_df >= (4-strategy)) 
			{
				filter_serial = 1 ;				
				c_filter_serial = pCTRL->stLINEMEM.c_linemem_en ;
			}
			else
			{
				filter_serial = 0 ;
				c_filter_serial = 0 ;
			}	
			
			if ( filter_serial == 1 )
			{
				fs_counter++ ;
			}
			else
			{
				if ( fs_counter > 0 )
					fs_counter-- ;			
			}
			
			if ( fs_counter > 4 )
				fs_counter = 4 ;
			
			pCTRL->st2DNR.fs_counter = fs_counter ;
			
			if ( fs_counter == 0 || fs_counter == 4 )
			{
				if ( (filter_serial != pCTRL->st2DNR.filter_serial) || (filter_mode != pCTRL->st2DNR.filter_mode)) 
				{
					// SetNR_2D_FilterMode(0,filter_serial,0) ;	// 2D : mode / serialize / filter_select
					SetNR_2D_FilterMode(filter_mode,filter_serial,0) ;	// 2D : mode / serialize / filter_select
					// SetNR_2D_FilterMode(0,0,2) ; 	// 2Df : mode / serialize / filter_select => 2Df do not have serialize option
					SetNR_2D_FilterMode(filter_mode,0,2) ; 	// 2Df : mode / serialize / filter_select => 2Df do not have serialize option
				}
				
				if ( c_filter_serial != pCTRL->st2DNR.c_filter_serial )
				{
					SetNR_2D_FilterMode(c_filter_serial,c_filter_serial,1) ; // 2Dc : mode / serialize / filter_select
				}
			}					
		}		
	}
//-----------------------------------------------------------------------------
	//
	//
	// i5x0 code end : NRCTRL_AUTO2DNR
	//
	//
//-----------------------------------------------------------------------------		
#else
//-----------------------------------------------------------------------------
	//
	//
	// i500 code start : NRCTRL_AUTO2DNR
	//
	//
//-----------------------------------------------------------------------------
	WORD rVal ;
	WORD is_auto_2Dsfilter ;
	WORD stdev_int ;
	WORD stdev_frac ;
	
	BOOL operation_mode ;

	BYTE under_exp ;
	BYTE imp_thresh ;
	BYTE imp_replace ;
	
	if ((pCTRL->mode == MISP_NR_OFF )) return ; // Runs only when NR_ON on MENU 
		
	if ( pCTRL->st3DNR.update_counter == __NR_AUTO2DNR_TIMER__ )
	{	
		MISP_RegRead(MISP_LOCAL_ID, 0x53F, &rVal);
		is_auto_2Dsfilter = rVal&0x0008 ;			
		
		if ( is_auto_2Dsfilter == 0 )
		{	// 2DNR manual control
			// Do nothing
		}
		else
		{	// 2DNR auto control
			operation_mode = pCTRL->lowlight_mode ;
			stdev_int = pCTRL->stFRAMENOISE.stdev_int ;
			stdev_frac = pCTRL->stFRAMENOISE.stdev_frac ;

			under_exp = pCTRL->stFRAMENOISE.under_exp ;
			
			if ( stdev_frac >= 4  ) stdev_int++ ;			
			
			if ( stdev_int < 10 ) imp_thresh = 30-(3*stdev_int) ;
			else imp_thresh = 3 ;	// if imp_thresh is less then 3 or minus value
			
			// The impulse signal 3D replace
			if ( stdev_int < pCTRL->st3DNR.k_value_bound02 )	imp_replace = 2 ;
			else 										imp_replace = 3 ;				
			
			if ( imp_replace == pCTRL->st2DNR.impulse_replace ) {
				if ( operation_mode == 0 && imp_thresh != pCTRL->st2DNR.rold_impulse_th ) SetNR_2D_ImpulseThresh(operation_mode,imp_thresh,0,imp_replace) ;				
				else if ( operation_mode == 1 && imp_thresh != pCTRL->st2DNR.rold_ll_impulse_th ) SetNR_2D_ImpulseThresh(operation_mode,imp_thresh,0,imp_replace) ;
			}
			else{
				SetNR_2D_ImpulseThresh(operation_mode,imp_thresh,0,imp_replace) ;
			}
		}
	}	
//-----------------------------------------------------------------------------
	//
	//
	// i500 code end : NRCTRL_AUTO2DNR
	//
	//
//-----------------------------------------------------------------------------			
#endif
}//NRCTRL_AUTO2DNR	
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------

static void NRCTRL_AUTO3DNR ( PNR_CTRL_PARAM pCTRL )
{
//-----------------------------------------------------------------------------
#if __NR_REV__
//-----------------------------------------------------------------------------
	//
	//
	// i5x0 code start : NRCTRL_AUTO3DNR
	//
	//
//-----------------------------------------------------------------------------
	WORD rVal ;
	WORD stdev_int ;
	// WORD stdev_frac ;
	BYTE k_value_target ;
	BYTE k_value_target_oth ;
	BYTE cur_k_value ;
	BYTE cur_k_value_oth ;
	BYTE under_exp ;
	
	
	// if ((pCTRL->intensity!=MISP_NR3D_AUTO)) return ; // Runs only when MISP_NR3D_AUTO mode on MENU 
	if ((pCTRL->mode == MISP_NR_OFF )) return ;
	
	if ( pCTRL->st3DNR.update_counter == 0 )
	{
		
		MISP_RegRead(MISP_LOCAL_ID, 0x53F, &rVal);
		rVal = rVal&0x0010 ;				
	
		if ( rVal == 0 ) return ;	
		
		stdev_int = pCTRL->stFRAMENOISE.stdev_int ;
		under_exp = pCTRL->stFRAMENOISE.under_exp ;
		
		// UARTprintf("[stdev_int:%d], [stdev_frac:%d]\r\n", stdev_int ,stdev_frac) ;	
		
		cur_k_value = pCTRL->st3DNR.const_k_value ;
		cur_k_value_oth = pCTRL->st3DNR.const_k_value_oth ;
		
		if ( stdev_int < pCTRL->st3DNR.k_value_bound00 )
			k_value_target = pCTRL->st3DNR.k_value_05 ;
		else if ( stdev_int < pCTRL->st3DNR.k_value_bound01 )
			k_value_target = pCTRL->st3DNR.k_value_04 ;
		else if ( stdev_int < pCTRL->st3DNR.k_value_bound02 )
			k_value_target = pCTRL->st3DNR.k_value_03 ;
		else if ( stdev_int < pCTRL->st3DNR.k_value_bound03 )
			k_value_target = pCTRL->st3DNR.k_value_02 ;
		else if ( stdev_int < pCTRL->st3DNR.k_value_bound04 )
			k_value_target = pCTRL->st3DNR.k_value_01 ;
		else
			k_value_target = pCTRL->st3DNR.k_value_00 ;		
		
	
		if ( under_exp > 1 ) 
		{
			k_value_target = k_value_target - under_exp ;
		}
#if __NR_SPLIT__==0	
		else
		{
			if ( pCTRL->intensity == MISP_NR3D_AUTO )
			{
				if ( pCTRL->stFRAMENOISE.mblocks[0] > 20 && pCTRL->stFRAMENOISE.mblocks[1] > 20 )
					k_value_target = k_value_target + 1 ;
				else if ( pCTRL->stFRAMENOISE.mblocks[0] <= 3 && pCTRL->stFRAMENOISE.mblocks[1] <= 3 )
					k_value_target = k_value_target - 1 ;
				
				if ( pCTRL->stFRAMENOISE.mblocks[1] > 20 && pCTRL->stFRAMENOISE.mblocks[2] > 20 )
					k_value_target = k_value_target + 1 ;			
				else if ( pCTRL->stFRAMENOISE.mblocks[1] <= 3 && pCTRL->stFRAMENOISE.mblocks[2] <= 3 )
					k_value_target = k_value_target - 1 ;
				
				if ( pCTRL->stFRAMENOISE.mblocks[2] > 20 && pCTRL->stFRAMENOISE.mblocks[3] > 20 )
					k_value_target = k_value_target + 1 ;
			}
		}
#endif // #if __NR_SPLIT__==0		
	

		if ( k_value_target < pCTRL->st3DNR.k_value_00 )
			k_value_target = pCTRL->st3DNR.k_value_00 ;			
		
		if ( cur_k_value > k_value_target ) cur_k_value-- ;			
		else if ( cur_k_value < k_value_target ) cur_k_value++ ; // For prevent hunting
		
		//UARTprintf("[k_value_target:%d], [cur_k_value:%d]\r\n", k_value_target ,cur_k_value) ;		

		if ( stdev_int < pCTRL->st3DNR.k_value_bound00 )
			k_value_target_oth = pCTRL->st3DNR.k_value_oth_05 ;
		else if ( stdev_int < pCTRL->st3DNR.k_value_bound01 )
			k_value_target_oth = pCTRL->st3DNR.k_value_oth_04 ;
		else if ( stdev_int < pCTRL->st3DNR.k_value_bound02 )
			k_value_target_oth = pCTRL->st3DNR.k_value_oth_03 ;
		else if ( stdev_int < pCTRL->st3DNR.k_value_bound03 )
			k_value_target_oth = pCTRL->st3DNR.k_value_oth_02 ;
		else if ( stdev_int < pCTRL->st3DNR.k_value_bound04 )
			k_value_target_oth = pCTRL->st3DNR.k_value_oth_01 ;
		else
			k_value_target_oth = pCTRL->st3DNR.k_value_oth_00 ;				
						
		if ( cur_k_value_oth > k_value_target_oth ) cur_k_value_oth-- ;			
		else if ( cur_k_value_oth < k_value_target_oth ) cur_k_value_oth++ ; // For prevent hunting			
						
		SetNR_3Dconst_k(cur_k_value, cur_k_value_oth) ;
		
	}
//-----------------------------------------------------------------------------
	//
	//
	// i5x0 code end : NRCTRL_AUTO3DNR
	//
	//
//-----------------------------------------------------------------------------	
//-----------------------------------------------------------------------------
#else	
//-----------------------------------------------------------------------------
	//
	//
	// i500 code start : NRCTRL_AUTO3DNR
	//
	//
//-----------------------------------------------------------------------------	
	WORD rVal ;
	WORD stdev_int ;
	// WORD stdev_frac ;
	BYTE k_value_target ;
	BYTE cur_k_value ;
	BYTE cur_k_value_oth ;
	BYTE under_exp ;
	
	// UARTprintf("Starting auto3DNR \r\n") ;
	
	if ((pCTRL->mode == MISP_NR_OFF )) return ;
	
	if ( pCTRL->st3DNR.update_counter == 0 )
	{
		
		MISP_RegRead(MISP_LOCAL_ID, 0x53F, &rVal);
		rVal = rVal&0x0010 ;				
		
		// UARTprintf("Checking auto3DNR : %d \r\n", rVal) ;	
		
		if ( rVal == 0 ) return ;	
	
		stdev_int = pCTRL->stFRAMENOISE.stdev_int ;
		under_exp = pCTRL->stFRAMENOISE.under_exp ;
		// UARTprintf("[stdev_int:%d], [stdev_frac:%d]\r\n", stdev_int ,stdev_frac) ;	
		
		cur_k_value = pCTRL->st3DNR.const_k_value ;
		
		if ( stdev_int < pCTRL->st3DNR.k_value_bound00 )
			k_value_target = pCTRL->st3DNR.k_value_05 ;
		else if ( stdev_int < pCTRL->st3DNR.k_value_bound01 )
			k_value_target = pCTRL->st3DNR.k_value_04 ;
		else if ( stdev_int < pCTRL->st3DNR.k_value_bound02 )
			k_value_target = pCTRL->st3DNR.k_value_03 ;
		else if ( stdev_int < pCTRL->st3DNR.k_value_bound03 )
			k_value_target = pCTRL->st3DNR.k_value_02 ;
		else if ( stdev_int < pCTRL->st3DNR.k_value_bound04 )
			k_value_target = pCTRL->st3DNR.k_value_01 ;
		else
			k_value_target = pCTRL->st3DNR.k_value_00 ;

		if ( under_exp > 0 ) 
		{
			if ( k_value_target > pCTRL->st3DNR.k_value_00 )
				k_value_target = k_value_target - under_exp ;
		}			
		
		// if ( k_value_target < __NR_AUTO3DNR_VALUE_0__ )
			// k_value_target = __NR_AUTO3DNR_VALUE_0__ ;			

		if ( k_value_target < pCTRL->st3DNR.k_value_00 )
			k_value_target = pCTRL->st3DNR.k_value_00 ;			
		
		// UARTprintf("[k_value_target:%d], [k_value_00:%d]\r\n", k_value_target ,pCTRL->st3DNR.k_value_00) ;	
		
		if ( cur_k_value > k_value_target ) cur_k_value-- ;			
		else if ( cur_k_value < k_value_target ) cur_k_value++ ; // For prevent hunting
		
		cur_k_value_oth = 0 ; // i500 does not support cur_k_value_oth
		SetNR_3Dconst_k(cur_k_value, cur_k_value_oth) ;
		
	}	
//-----------------------------------------------------------------------------
	//
	//
	// i500 code end : NRCTRL_AUTO3DNR
	//
	//
//-----------------------------------------------------------------------------		
#endif
} // NRCTRL_AUTO3DNR()
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------------

static void NRCTRL_AUTOIDMc ( PNR_CTRL_PARAM pCTRL )
{
#if __NR_REV__
//-----------------------------------------------------------------------------
	//
	//
	// i5x0 code start : NRCTRL_AUTOIDMc
	//
	//
//-----------------------------------------------------------------------------		
	
	WORD rVal;
	WORD stdev_int ;
	WORD stdev_frac ;
	WORD prestdev_int ;
	WORD prestdev_frac ;
	BOOL operation_mode ;

	BYTE cbound ;
	BYTE TH_0 ;
	BYTE TH_1 ;

	// if ( pCTRL->st3DNR.update_counter == 0 && (pCTRL->image_type == MISP_NR_YC_IMAGE) )
	if ( pCTRL->st3DNR.update_counter == __NR_AUTOIDM_TIMER__ && (pCTRL->image_type == MISP_NR_YC_IMAGE) )
	{
		if ( (pCTRL->ctrl&NRCTRL_AUTO_IDMc_UPDATE ) == 0)
		{
			MISP_RegRead(MISP_LOCAL_ID, 0x53F, &rVal);
			rVal = rVal&0x0080 ;			
		}
		else
		{
			pCTRL->ctrl &= (~NRCTRL_AUTO_IDMc_UPDATE);	
			rVal = pCTRL->stIDM.c_idm_auto ;
			MISP_RegField(MISP_LOCAL_ID, 0x53F, 7, 1, rVal);
		}
		
		if ( rVal == 0 ) return ;	
				
		operation_mode = pCTRL->lowlight_mode ;
		stdev_int = pCTRL->stFRAMENOISE.stdev_int ;
		stdev_frac = pCTRL->stFRAMENOISE.stdev_frac ;			

		prestdev_int = pCTRL->stFRAMENOISE.prestdev_int ;
		prestdev_frac = pCTRL->stFRAMENOISE.prestdev_frac ;				
		
		if ( operation_mode == 0 )
		{			
			if ( stdev_frac >= 4 )
				stdev_int++ ;

			if ( prestdev_frac >= 4 )
				prestdev_int++ ;			
			
			cbound = 15 + (stdev_int<<1) ;
			
			stdev_int = ( DIFF(stdev_int,prestdev_int)>>1 ) ;			
			
			TH_0 = stdev_int + (stdev_int>>1) ;
			TH_1 = stdev_int + (stdev_int>>2) ;
		}
		else
		{
			cbound = 15 + (stdev_int<<1) ;
			
			stdev_int = ( DIFF(stdev_int,prestdev_int)>>1 ) ;
			
			TH_0 = stdev_int + (stdev_int>>2) ;
			TH_1 = stdev_int + (stdev_int>>2) ;		
		}
		
		SetNR_IDMc(operation_mode,cbound,TH_0,TH_1);
	}
		
//-----------------------------------------------------------------------------
	//
	//
	// i5x0 code end : NRCTRL_AUTOIDMc
	//
	//
//-----------------------------------------------------------------------------	
#else		
//-----------------------------------------------------------------------------
	//
	//
	// i500 code start : NRCTRL_AUTOIDMc
	//
	//
//-----------------------------------------------------------------------------			
	
	WORD rVal;
	WORD stdev_int ;
	WORD stdev_frac ;
	BOOL operation_mode ;

	BYTE cbound ;
	BYTE TH_0 ;
	BYTE TH_1 ;

	// if ( pCTRL->st3DNR.update_counter == 0 && (pCTRL->image_type == MISP_NR_YC_IMAGE) )
	if ( pCTRL->st3DNR.update_counter == __NR_AUTOIDM_TIMER__ && (pCTRL->image_type == MISP_NR_YC_IMAGE) )
	{
		MISP_RegRead(MISP_LOCAL_ID, 0x53F, &rVal);
		rVal = rVal&0x0080 ;
		
		if ( rVal == 0 ) return ;	
				
		operation_mode = pCTRL->lowlight_mode ;
		stdev_int = pCTRL->stFRAMENOISE.stdev_int ;
		stdev_frac = pCTRL->stFRAMENOISE.stdev_frac ;			
		
		if ( operation_mode == 0 )
		{			
			if ( stdev_frac >= 4 )
				stdev_int++ ;
			
			cbound = 15 + (stdev_int<<1) ;
			TH_0 = stdev_int + (stdev_int>>1) ;
			TH_1 = stdev_int + (stdev_int>>2) ;
		}
		else
		{
			cbound = 15 + (stdev_int<<1) ;
			TH_0 = stdev_int + (stdev_int>>2) ;
			TH_1 = stdev_int + (stdev_int>>2) ;		
		}
		
		SetNR_IDMc(operation_mode,cbound,TH_0,TH_1);
	}
//-----------------------------------------------------------------------------
	//
	//
	// i500 code end : NRCTRL_AUTOIDMc
	//
	//
//-----------------------------------------------------------------------------	
#endif
}
//--------------------------------------------------------------------------------------------------------------------------

	

//--------------------------------------------------------------------------------------------------------------------------

void NR_ProcessHandler(void)
{
	NRCTRL_LinememHandler(&stNRCTRL);	
	
#if (__NR_SPLIT__==0)||(__NR_SPLIT__==1)	
	NRCTRL_FrameStdevUpdate(&stNRCTRL);
	
	NRCTRL_AUTOIDM(&stNRCTRL);	
	NRCTRL_AUTOPDM(&stNRCTRL);	
	NRCTRL_AUTOSRM(&stNRCTRL);		
	
	NRCTRL_AUTOIDMc(&stNRCTRL);
	
	NRCTRL_MCUDriveReturn(&stNRCTRL);
	NRCTRL_AUTO3DNR(&stNRCTRL);	
		
#if __NR_REV__
	NRCTRL_AUTO2DNR(&stNRCTRL) ;
	NRCTRL_AUTOM2DNR(&stNRCTRL) ;
#endif
	
	NRCTRL_AUTOL2DNR(&stNRCTRL) ;
		
	if ((stNRCTRL.ctrl&NRCTRL_TRIGGER_ISP)==0) return;
	stNRCTRL.ctrl &= (~NRCTRL_TRIGGER_ISP);
	
	NRCTRL_MCUDriveHandler(&stNRCTRL);
	
	NRCTRL_InputImageTypeHandler(&stNRCTRL);
	NRCTRL_DataOrderHandler(&stNRCTRL);
	NRCTRL_ModeHandler(&stNRCTRL);
	NRCTRL_ImageSizeHandler(&stNRCTRL);
	NRCTRL_3DintensityHandler(&stNRCTRL);
	NRCTRL_LowLightLevelHandler(&stNRCTRL);
	NRCTRL_DemoModeHandler(&stNRCTRL);	
	
	NRCTRL_IDMHandler(&stNRCTRL);	
	NRCTRL_IDMcHandler(&stNRCTRL);
	
	NRCTRL_PDMHandler(&stNRCTRL) ;
	NRCTRL_SRMHandler(&stNRCTRL) ;	
	
#if __NR_REV__
	NRCTRL_2D_sFilterHandler(&stNRCTRL);
	NRCTRL_2D_MFilterHandler(&stNRCTRL);
#endif	
	
	NRCTRL_2D_NBHandler (&stNRCTRL) ;
		
	NRCTRL_2D_LFilterHandler(&stNRCTRL);
	
	NRCTRL_WdrFlagControlHandler(&stNRCTRL);
#endif // #if (__NR_SPLIT__==0)||(__NR_SPLIT__==1)
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
void SetNR_Toggle_Inputimage_type(void)	
{

	stNRCTRL.ctrl |= (NRCTRL_IMAGE_TYPE_UPDATE | NRCTRL_LINEMEM_UPDATE | NRCTRL_TRIGGER_ISP);
	
	if ( stNRCTRL.image_type == MISP_NR_BAYER_IMAGE )
	{
		stNRCTRL.image_type = MISP_NR_YC_IMAGE ;
		stNRCTRL.stFRAMENOISE.edge_level = __NR_YC_EDGE_LEVEL__ ;
		SetNR_image_size(0,0,GetIN_MainSrcSizeH(),GetIN_MainSrcSizeV()); // y/c image size set. Follow Main input active size		
	}
	else
	{
		stNRCTRL.image_type = MISP_NR_BAYER_IMAGE ;
		stNRCTRL.stFRAMENOISE.edge_level = __NR_BY_EDGE_LEVEL__ ;
		SetNR_image_size(GetSS_image_boundary_size_h(),GetSS_image_boundary_size_v(),GetSS_image_active_size_h(),GetSS_image_active_size_v()); // bayer image size set
	}
	
	SetDM_Input_select(stNRCTRL.image_type) ; // Set demosaic input data : [0] NR / [1] Bayer top
	SetCLK_SrcNR(stNRCTRL.image_type);
		// [ '0' : clk_bayer is used ]
		// [ '1' : clk_yc_m is used ]	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetNR_Mode( MISP_NR_MODE_t val)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];	
#endif	
	stNRCTRL.mode = val;
	stNRCTRL.ctrl |= (NRCTRL_LINEMEM_UPDATE);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	// BYTE buff[2];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
	EEPROM_MultiWrite(EEPROM_DNR_MODE, (PBYTE)&buff[0], sizeof(buff));
#endif
	
#if __NR_SPLIT__==1	
	// Split mode master send command to slave
	SPImAPP_SetSlaveNRMode(val);
#endif
	
#if __MISP100_DBGPRT__ == 1
	UARTprintf("SetNR_Mode : mode(%d) \r\n", val);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetNR_Bayer_data_order( MISP_SS_BAYER_PATTERN_t val )
{
	stNRCTRL.ctrl |= (NRCTRL_DATA_ORDER_UPDATE | NRCTRL_TRIGGER_ISP);
	
	switch (val)
	{
		case MISP_SS_BAYER_PATTERN_0 : stNRCTRL.bayer_channel = 0 ; break ;
		case MISP_SS_BAYER_PATTERN_1 : stNRCTRL.bayer_channel = 1 ; break ;
		case MISP_SS_BAYER_PATTERN_2 : stNRCTRL.bayer_channel = 1 ; break ;
		case MISP_SS_BAYER_PATTERN_3 : stNRCTRL.bayer_channel = 0 ; break ;
		default : stNRCTRL.bayer_channel = 1 ; break ;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SetNR_image_size(BYTE bH , BYTE bV, WORD valH , WORD valV)
{
	BOOL sensor_bit_opertion ;
	WORD input_h_size ;
	
	stNRCTRL.ctrl |= (NRCTRL_IMAGE_SIZE_UPDATE | NRCTRL_TRIGGER_ISP);
	
	stNRCTRL.stLINEMEM.bH = bH ;
	stNRCTRL.stLINEMEM.bV = bV ;
	stNRCTRL.stLINEMEM.active_h_size = valH ;
	stNRCTRL.stLINEMEM.active_v_size = valV ;
	
	input_h_size = valH + (bH<<1) ;
	
	sensor_bit_opertion = GetSS_sensor_bit_opertion() ;	

	if ( input_h_size <= 768 ) {
		stNRCTRL.stFRAMENOISE.corr_ratio = 0 ;	
		SetNR_BitMode(0) ; // Under input  H size 768 	
	}
	else if ( input_h_size <= 2048 ) {
		stNRCTRL.stFRAMENOISE.corr_ratio = 0 ;	
		if ( sensor_bit_opertion == MISP_8BIT_OPERATION )
			SetNR_BitMode(__NR_10BIT_SETUP__) ; // follow mispnr.h setup bit mode for under H 2048
		else
			SetNR_BitMode(1) ; // 10b operation bit mode for under H 2048
	}
	else {
		stNRCTRL.stFRAMENOISE.corr_ratio = 1 ;	
		SetNR_BitMode(0) ;  // 8b NR for over H 2048
	}	
	
	if ( GetROT_RotationEnable() == 0 ) SetDDRMAP_image_size(bH,bV,valH,valV) ; // If Rotation disabled
}
//--------------------------------------------------------------------------------------------------------------------------
MISP_NR_MODE_t GetNR_Mode(void)
{
	return 	stNRCTRL.mode;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetNR_intensity( MISP_NR3D_MODE_t val)
{
	if ( stNRCTRL.mode == MISP_NR_3D || stNRCTRL.mode == MISP_NR_2D3D )
	{
		SetNR_3Dintensity(val) ;
	}
	else if ( stNRCTRL.mode == MISP_NR_2D  )
	{
		SetNR_2Dintensity(val) ;
	}
	
#if __NR_SPLIT__==1	
	// Split mode master send command to slave
	SPImAPP_SetSlaveNRIntensity(val);
#endif
	
}

MISP_NR3D_MODE_t GetNR_intensity()
{
	if ( stNRCTRL.mode == MISP_NR_3D || stNRCTRL.mode == MISP_NR_2D3D )
	{
		return GetNR_3Dintensity() ;
	}
	else if ( stNRCTRL.mode == MISP_NR_2D  )
	{
		return GetNR_2Dintensity() ;
	}
	else
	{
		return 0 ;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SetNR_Aperture ( BYTE aperture )
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];	
#endif	
	
	stNRCTRL.aperture = aperture ;
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	// BYTE buff[2];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)aperture;
	EEPROM_MultiWrite(EEPROM_DNR_APERTURE, (PBYTE)&buff[0], sizeof(buff));
#endif	
	
#if __NR_SPLIT__==1	
	// Split mode master send command to slave
	SPImAPP_SetSlaveNRAperture(aperture);
#endif	
}
//--------------------------------------------------------------------------------------------------------------------------
BYTE GetNR_Aperture( void )
{
	return stNRCTRL.aperture ;	
}
//--------------------------------------------------------------------------------------------------------------------------
void SetNR_3Dintensity( MISP_NR3D_MODE_t val)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];	
#endif

	stNRCTRL.intensity = val;
	stNRCTRL.ctrl |= (NRCTRL_3DINTENSITY_UPDATE | NRCTRL_TRIGGER_ISP);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	// BYTE buff[2];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
	EEPROM_MultiWrite(EEPROM_DNR_LEVEL, (PBYTE)&buff[0], sizeof(buff));
#endif
	
#if __MISP100_DBGPRT__ == 1
	UARTprintf("SetNR_3Dintensity : val(%d) \r\n", val);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_NR3D_MODE_t GetNR_3Dintensity(void)
{
	return  (MISP_NR3D_MODE_t)stNRCTRL.intensity;
}

MISP_NR3D_MODE_t GetNR_2Dintensity(void)
{
	return  (MISP_NR3D_MODE_t)stNRCTRL.intensity_2D;
}
//--------------------------------------------------------------------------------------------------------------------------
void SetNR_2Dintensity( MISP_NR3D_MODE_t val)
{
	stNRCTRL.intensity_2D = val ; // SetNR_2Dintensity does not have handler
}
//--------------------------------------------------------------------------------------------------------------------------
void SetNR_3Dconst_k(WORD val, WORD val_oth)
{
	stNRCTRL.ctrl |= (NRCTRL_3DINTENSITY_UPDATE | NRCTRL_TRIGGER_ISP);
	stNRCTRL.st3DNR.const_k_value = val ;
	stNRCTRL.st3DNR.const_k_value_oth = val_oth ;
}

//--------------------------------------------------------------------------------------------------------------------------
MISP_NRLOWLIGHT_MODE_t GetNR_LowLightMode(void)
{
	WORD	rVal;
	
	if ( stNRCTRL.stMCUDRIVE.mcu_mode_ctrl == 0 ){
		MISP_RegRead(MISP_LOCAL_ID, 0x542, &rVal);			//read low light mode register.
		rVal = (rVal >> 13) & 0x01;
	}
	else
	{
		rVal = stNRCTRL.stMCUDRIVE.mcu_force_lowlight_mode ;
	}

	return (MISP_NRLOWLIGHT_MODE_t)rVal;
}

//--------------------------------------------------------------------------------------------------------------------------
WORD GetNR_LowLightStillIntensity(void)
{
	WORD	rVal;
	
	MISP_RegRead(MISP_LOCAL_ID, 0x55A, &rVal);			//read low light still_intensity register.	
		
	return	rVal ;
}

//--------------------------------------------------------------------------------------------------------------------------
void GetNR_StDEV(PWORD StDev_int, PWORD StDev_frac, BYTE target)
{
	WORD	rVal;
	
	if ( target == 0 ) {	
		MISP_RegRead(MISP_LOCAL_ID, 0x559, &rVal);			//read Frame stdev register.	
		
		*StDev_int = rVal>>3 ;
		*StDev_frac = rVal&0x0007 ;
	}
	else
	{
		*StDev_int = stNRCTRL.stFRAMENOISE.stdev_int ;
		*StDev_frac = stNRCTRL.stFRAMENOISE.stdev_frac ;	
	}
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE GetNR_mblocks(void)
{
	WORD	rVal;

	MISP_RegRead(MISP_LOCAL_ID, 0x52D, &rVal);			//read from register.	
	
	rVal = (rVal>>10) ;

	return (BYTE)rVal ;
}

//--------------------------------------------------------------------------------------------------------------------------
void GetNR_preStDEV(PWORD StDev_int, PWORD StDev_frac, BYTE target )
{
#if __NR_REV__	
#if __NR_SPLIT__==0	
	WORD	rVal;
	if ( target == 0 ) 
	{	
		MISP_RegRead(MISP_LOCAL_ID, 0x51E, &rVal);			//read Frame stdev register.	
		
		rVal = rVal>>6 ;
			
		*StDev_int = rVal>>3 ;
		*StDev_frac = rVal&0x0007 ;
	}
	else
	{
		*StDev_int = stNRCTRL.stFRAMENOISE.prestdev_int ;
		*StDev_frac = stNRCTRL.stFRAMENOISE.prestdev_frac ;	
	}
#else // #if __NR_SPLIT__==1 || #if __NR_SPLIT__==2
		// NR split mode does not refer pre stdev value
		*StDev_int = 0 ;
		*StDev_frac = 0 ;		
#endif // #if __NR_SPLIT__==0	
#endif // #if __NR_REV__	
}
//--------------------------------------------------------------------------------------------------------------------------

void GetNR_StDEV_br(PWORD StDev_int, PWORD StDev_frac, BYTE target)
{
#if __NR_REV__		
	WORD	rVal;
	
	if ( target == 0 ) 
	{	
		MISP_RegRead(MISP_LOCAL_ID, 0x55A, &rVal);			//read bright Frame stdev register.	
		
		*StDev_int = rVal>>3 ;
		*StDev_frac = rVal&0x0007 ;	
	}
	else
	{
		*StDev_int = stNRCTRL.stFRAMENOISE.stdev_int_br ;
		*StDev_frac = stNRCTRL.stFRAMENOISE.stdev_frac_br ;		
	}
#endif
}
//--------------------------------------------------------------------------------------------------------------------------

void GetNR_StDEV_da(PWORD StDev_int, PWORD StDev_frac, BYTE target)
{
#if __NR_REV__	
	WORD	rVal;
	
	if ( target == 0 ) 
	{
		MISP_RegRead(MISP_LOCAL_ID, 0x558, &rVal);			//read dark Frame stdev register.	
		
		*StDev_int = rVal>>3 ;
		*StDev_frac = rVal&0x0007 ;
	}
	else
	{
		*StDev_int = stNRCTRL.stFRAMENOISE.stdev_int_da ;
		*StDev_frac = stNRCTRL.stFRAMENOISE.stdev_frac_da ;		
	}		
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetNR_MCUDriveMode(BOOL drive_en, BOOL force_lowlight_en , BOOL force_still_en , BYTE still_intensity , BYTE drive_duration)
{
	stNRCTRL.stMCUDRIVE.mcu_mode_ctrl = drive_en ;
	stNRCTRL.stMCUDRIVE.mcu_force_lowlight_mode = force_lowlight_en ;
	stNRCTRL.stMCUDRIVE.mcu_force_still_mode = force_still_en ;
	stNRCTRL.stMCUDRIVE.mcu_still_intensity = still_intensity ;
	stNRCTRL.stMCUDRIVE.mcu_drive_duration = drive_duration ;
	
	if ( drive_duration == 0 )
		stNRCTRL.ctrl |= (NRCTRL_MCU_DRIVE_UPDATE | NRCTRL_TRIGGER_ISP);
	else
		stNRCTRL.ctrl |= (NRCTRL_MCU_DRIVE_RETURN|NRCTRL_MCU_DRIVE_UPDATE | NRCTRL_TRIGGER_ISP);
}

//--------------------------------------------------------------------------------------------------------------------------

void SetNR_NRZoomingMode(void)
{
#ifdef __NR_ZOOM_LENS_SETUP__
	
#if __NR_SPLIT__==0		
	BOOL operation_mode ;

	stNRCTRL.lowlight_mode = GetNR_LowLightMode() ; // Update LL flag
	
	operation_mode = stNRCTRL.lowlight_mode ;	
	
	SetNR_MCUDriveMode(1,operation_mode,1,0,__NR_ACTIVE_COUNTER__) ; // Force exit NR LL still mode with auto control return
#endif // #if __NR_SPLIT__==0		

#if __NR_SPLIT__==1	
	// Split mode master send command to slave
	SPImAPP_SetSlaveNRZoomMode();
#endif
	// NR split mode is alredy MCU drive enabled
	stNRCTRL.st3DNR.update_counter = MAX(4,__NR_ACTIVE_COUNTER__-4) ; // AUTO 3DNR counter reset
	
	SetNR_3Dconst_k(stNRCTRL.st3DNR.k_value_04,stNRCTRL.st3DNR.k_value_04); // Set 3DNR strength low
#endif // #ifdef __NR_ZOOM_LENS_SETUP__
}

//--------------------------------------------------------------------------------------------------------------------------
void SetNR_DemoMode(MISP_NRDEMO_MODE_t val)
{
	stNRCTRL.demo_mode = val;
	stNRCTRL.ctrl |= (NRCTRL_DEMOMODE_UPDATE | NRCTRL_TRIGGER_ISP);
	
#if __MISP100_DBGPRT__ == 1
	UARTprintf("SetNR_DemoMode : NR(%s) ON !!! \r\n", (val==1)? "LEFT": ((val==2)? "RIGHT": "ALL"));
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
WORD GetNR_DemoMode(void)
{
	return stNRCTRL.demo_mode;
}
//--------------------------------------------------------------------------------------------------------------------------
WORD GetNR_Inputimage_type(void)
{
	return stNRCTRL.image_type ;
}
//--------------------------------------------------------------------------------------------------------------------------
void SetNR_LowLightEnterLevel( WORD val_int , WORD val_fr )	
{
	stNRCTRL.stLL.ent_level = val_int ;			//0~100
	stNRCTRL.stLL.ent_level_fr = val_fr&0x07;		//0(0%)~7(0.875%) 
	stNRCTRL.ctrl |= (NRCTRL_LOWLIGHTLEVEL_UPDATE | NRCTRL_TRIGGER_ISP);
	
#if __MISP100_DBGPRT__ == 1
	UARTprintf("SetNR_3DEnterLevel : level(%d) -> ent_level(%d.%d) \r\n", val, stNRCTRL.stLL.ent_level, stNRCTRL.stLL.ent_level_fr*125);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetNR_LowLightExitLevel( WORD val_int , WORD val_fr )
{
	stNRCTRL.stLL.exit_level = val_int ;		//0~100
	stNRCTRL.stLL.exit_level_fr = val_fr&0x07;		//0(0%)~7(0.875%) 
	stNRCTRL.ctrl |= (NRCTRL_LOWLIGHTLEVEL_UPDATE | NRCTRL_TRIGGER_ISP);
	
#if __MISP100_DBGPRT__ == 1
	UARTprintf("SetNR_3DExitLevel : level(%d) -> ent_level(%d.%d) \r\n", val, stNRCTRL.stLL.exit_level, stNRCTRL.stLL.exit_level_fr*125);
#endif
}

//---------------------------------------------------------------------------------------------------------------------------
void SetNR_PDMratio(BOOL op_mode, BYTE ratio)
{
	if ( op_mode == 0 ) stNRCTRL.stPDM.pd_sads_ratio = ratio ;
	else				stNRCTRL.stPDM.pd_ll_sads_ratio = ratio ;
	
	stNRCTRL.ctrl |= (NRCTRL_PDM_UPDATE | NRCTRL_TRIGGER_ISP);	
}

//---------------------------------------------------------------------------------------------------------------------------
void SetNR_PDMmotionK(BOOL op_mode, BYTE motion_k, BYTE neutral_motion_k )
{
	if ( op_mode == 0 )
	{
		stNRCTRL.stPDM.pd_motion_k = motion_k ;
		stNRCTRL.stPDM.pd_nutral_motion_k = neutral_motion_k ;	
	}
	else
	{
		stNRCTRL.stPDM.pd_ll_motion_k = motion_k ;
		stNRCTRL.stPDM.pd_ll_nutral_motion_k = neutral_motion_k ;		
	}
	
	stNRCTRL.ctrl |= (NRCTRL_PDM_UPDATE | NRCTRL_TRIGGER_ISP);	
}

//---------------------------------------------------------------------------------------------------------------------------
void SetNR_IDMAuto(BOOL auto_en )
{
	stNRCTRL.stIDM.idm_auto = auto_en ;
	
	stNRCTRL.ctrl |= (NRCTRL_AUTO_IDM_UPDATE);	
}

void SetNR_IDMcAuto(BOOL auto_en )
{
	stNRCTRL.stIDM.c_idm_auto = auto_en ;
	
	stNRCTRL.ctrl |= (NRCTRL_AUTO_IDMc_UPDATE);	
}
//---------------------------------------------------------------------------------------------------------------------------
void SetNR_IDM(BOOL op_mode , BOOL dy_noise_limit , BYTE noise_limit , BYTE mo_th_0 , BYTE mo_th_1 , BYTE mo_th_2 , BYTE mo_th_3 )
{
	if ( op_mode == 0 )
	{
		stNRCTRL.stIDM.id_dy_noise_limit = dy_noise_limit ;
		stNRCTRL.stIDM.id_noise_limit = noise_limit ;	
		
		stNRCTRL.stIDM.id_thres_3d_nr = mo_th_0 ;
		stNRCTRL.stIDM.id_thres_3d_2d_nr = mo_th_1 ;
		stNRCTRL.stIDM.id_thres_pre_3d_cu_2d_nr = mo_th_2 ;
		stNRCTRL.stIDM.id_thres_pre_3d_2d_cu_2d_nr = mo_th_3 ;		
	}
	else
	{
		stNRCTRL.stIDM.id_ll_dy_noise_limit = dy_noise_limit ;
		stNRCTRL.stIDM.id_ll_noise_limit = noise_limit ;	
		
		stNRCTRL.stIDM.id_ll_thres_3d_nr = mo_th_0 ;
		stNRCTRL.stIDM.id_ll_thres_3d_2d_nr = mo_th_1 ;
		stNRCTRL.stIDM.id_ll_thres_pre_3d_cu_2d_nr = mo_th_2 ;
		stNRCTRL.stIDM.id_ll_thres_pre_3d_2d_cu_2d_nr = mo_th_3 ;		
	}
	
	stNRCTRL.ctrl |= (NRCTRL_IDM_UPDATE|NRCTRL_TRIGGER_ISP);
}
//--------------------------------------------------------------------------------------------------------------------------
void SetNR_IDM_ratio( BYTE t_ratio, BYTE y_ratio, BYTE b_ratio)
{
	stNRCTRL.ctrl |= (NRCTRL_IDM_UPDATE | NRCTRL_TRIGGER_ISP);
	
	stNRCTRL.stIDM.id_t_ratio = t_ratio ;
	stNRCTRL.stIDM.id_y_ratio = y_ratio ;
	stNRCTRL.stIDM.id_b_ratio = b_ratio ;	
}
//---------------------------------------------------------------------------------------------------------------------------
void SetNR_IDM_mhist(BOOL en)
{
#if __NR_REV__
	if ( stNRCTRL.stWDR.r_wdr_mo_en == 0 )
	{
		stNRCTRL.stIDM.idm_hist_en = en ;	
		// SetNR_2D_mhist(en) ;
		SetNR_PDM_mhist(en) ;	
	}
	else
	{
		stNRCTRL.stIDM.idm_hist_en = 0 ;	
		// SetNR_2D_mhist(0) ;
		SetNR_PDM_mhist(0) ;	
	}

	stNRCTRL.ctrl |= (NRCTRL_LINEMEM_UPDATE | NRCTRL_TRIGGER_ISP);	
#endif	
}
//---------------------------------------------------------------------------------------------------------------------------

void SetNR_2D_mhist(BOOL en)
{
#if __NR_REV__
	
#endif
}

//---------------------------------------------------------------------------------------------------------------------------

void SetNR_PDM_mhist(BOOL en)
{
#if __NR_REV__
	stNRCTRL.stPDM.pd_pmh_en = en ;	
	stNRCTRL.ctrl |= (NRCTRL_PDM_UPDATE | NRCTRL_TRIGGER_ISP);		
#endif
}
//---------------------------------------------------------------------------------------------------------------------------

void SetNR_PDM_mhistTH(BYTE pmh_thres)
{
#if __NR_REV__
	stNRCTRL.stPDM.pd_pmh_thres = pmh_thres ;	
	stNRCTRL.ctrl |= (NRCTRL_PDM_UPDATE | NRCTRL_TRIGGER_ISP);		
#endif
}
//---------------------------------------------------------------------------------------------------------------------------

void SetNR_PDM_mhist_K(BYTE pmh_k00 , BYTE pmh_k01, BYTE pmh_k02)
{
#if __NR_REV__
	stNRCTRL.stPDM.pd_pmh_k00 = pmh_k00 ;	
	stNRCTRL.stPDM.pd_pmh_k01 = pmh_k01 ;	
	stNRCTRL.stPDM.pd_pmh_k02 = pmh_k02 ;	
	
	stNRCTRL.ctrl |= (NRCTRL_PDM_UPDATE | NRCTRL_TRIGGER_ISP);	
#endif
}

//---------------------------------------------------------------------------------------------------------------------------
void SetNR_IDMc(BOOL op_mode, BYTE CBound, BYTE th_0 , BYTE th_1)
{
	if ( op_mode == 0 )
	{		
		stNRCTRL.stIDM.idc_thres_0 = th_0 ;
		stNRCTRL.stIDM.idc_thres_1 = th_1 ;
	}
	else
	{
		stNRCTRL.stIDM.idc_ll_thres_0 = th_0 ;
		stNRCTRL.stIDM.idc_ll_thres_1 = th_1 ;	
	}
	
	stNRCTRL.st2DNR.c_inbound_range = CBound ;
	
	
	stNRCTRL.ctrl |= (NRCTRL_IDMc_UPDATE | NRCTRL_TRIGGER_ISP);
}

//---------------------------------------------------------------------------------------------------------------------------
void SetNR_PDMAuto(BOOL auto_en )
{
	stNRCTRL.stPDM.pdm_auto = auto_en ;
	
	stNRCTRL.ctrl |= (NRCTRL_AUTO_PDM_UPDATE);
	
}
//---------------------------------------------------------------------------------------------------------------------------

void SetNR_PDM(BOOL op_mode, BOOL dy_noise_limit, BYTE noise_limit , BYTE motion_th, BYTE motion_adj )
{
	if ( op_mode == 0 )
	{
		stNRCTRL.stPDM.pd_dy_noise_limit = dy_noise_limit ;
		stNRCTRL.stPDM.pd_noise_limit = noise_limit ;	
		
		stNRCTRL.stPDM.pd_motion_th = motion_th ;
		stNRCTRL.stPDM.pd_motion_adj = motion_adj ;
	}
	else
	{
		stNRCTRL.stPDM.pd_ll_dy_noise_limit = dy_noise_limit ;
		stNRCTRL.stPDM.pd_ll_noise_limit = noise_limit ;	
		
		stNRCTRL.stPDM.pd_ll_motion_th = motion_th ;
		stNRCTRL.stPDM.pd_ll_motion_adj = motion_adj ;	
	}
	
	stNRCTRL.ctrl |= (NRCTRL_PDM_UPDATE | NRCTRL_TRIGGER_ISP);	
}

//--------------------------------------------------------------------------------------------------------------------------

void SetNR_PDM_replace( BYTE pdm_replace , BYTE pdm_replace_c )
{
#if __NR_REV__	
	stNRCTRL.stPDM.sr_replace = pdm_replace ;
	stNRCTRL.stPDM.sr_replace_c = pdm_replace_c ;
	
	stNRCTRL.ctrl |= (NRCTRL_PDM_UPDATE | NRCTRL_TRIGGER_ISP);
#endif
}

//---------------------------------------------------------------------------------------------------------------------------
void SetNR_SRMAuto(BOOL auto_en )
{
	stNRCTRL.stSRM.srm_auto = auto_en ;
	
	stNRCTRL.ctrl |= (NRCTRL_AUTO_SRM_UPDATE);
	
}
//--------------------------------------------------------------------------------------------------------------------------
void SetNR_SRM(BOOL op_mode, BOOL dy_th, BYTE motion_th , BYTE motion_th_frac )
{
	if ( op_mode == 0 )
	{
		stNRCTRL.stSRM.sr_dy_th = dy_th ;	
		stNRCTRL.stSRM.sr_th = motion_th ;	
		stNRCTRL.stSRM.sr_th_fractional = motion_th_frac ;
	}
	else
	{
		stNRCTRL.stSRM.sr_ll_dy_th = dy_th ;	
		stNRCTRL.stSRM.sr_ll_th = motion_th ;	
		stNRCTRL.stSRM.sr_ll_th_fractional = motion_th_frac ;			
	}
	
	stNRCTRL.ctrl |= (NRCTRL_SRM_UPDATE | NRCTRL_TRIGGER_ISP);	

}

//--------------------------------------------------------------------------------------------------------------------------

void SetNR_SRM_corr( BOOL op_mode, BOOL pdm_en , BOOL mi_en ,  BYTE mi_corr_value )
{
#if __NR_REV__	
	stNRCTRL.stSRM.sr_bmi_corr_en = pdm_en ;
#else
	stNRCTRL.stSRM.sr_bmi_corr_en = 0 ; // i500 do not support sr_bmi_corr_en
#endif
	stNRCTRL.stSRM.sr_mi_corr_en = mi_en ;	
	
	if ( op_mode == 0 ) stNRCTRL.stSRM.sr_mi_corr_value = mi_corr_value ;	
	else stNRCTRL.stSRM.sr_ll_mi_corr_value = mi_corr_value ;	

	stNRCTRL.ctrl |= (NRCTRL_SRM_UPDATE | NRCTRL_TRIGGER_ISP);	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetNR_PixelCategoryBound(BYTE dark_upper_bound , BYTE bright_lower_bound)
{
	stNRCTRL.stFRAMENOISE.dark_upper_bound = dark_upper_bound ;
	stNRCTRL.stFRAMENOISE.bright_lower_bound = bright_lower_bound ;
	
	stNRCTRL.ctrl |= (NRCTRL_FRMNOISE_UPDATE | NRCTRL_TRIGGER_ISP);	
}
//--------------------------------------------------------------------------------------------------------------------------

BYTE GetNR_3Dconst_k(void)
{
	return stNRCTRL.st3DNR.const_k_value ;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetNR_2D_sFilter(WORD table_no , BYTE filter_select)
{
#if __NR_REV__		
	if ( filter_select == 0 )
	{
		stNRCTRL.st2DNR.sFilter_sigma = table_no ;	
		stNRCTRL.ctrl |= (NRCTRL_2D_SFILTER_UPDATE | NRCTRL_TRIGGER_ISP);	
	}
	else if ( filter_select == 2 )
	{
		stNRCTRL.st2DNR.sFilter_sigma_df = table_no ;	
		stNRCTRL.ctrl |= (NRCTRL_2Df_SFILTER_UPDATE | NRCTRL_TRIGGER_ISP);			
	}
	else
	{
		stNRCTRL.st2DNR.c_sFilter_sigma = table_no ;
		stNRCTRL.ctrl |= (NRCTRL_2Dc_SFILTER_UPDATE | NRCTRL_TRIGGER_ISP);	
	}
	
#endif	
}
//--------------------------------------------------------------------------------------------------------------------------
void SetNR_2D_FilterMode(BYTE mode , BYTE serialize, BYTE filter_select)
{
#if __NR_REV__		
	if ( filter_select == 0 )
	{
		stNRCTRL.st2DNR.filter_mode = mode ;	
		stNRCTRL.st2DNR.filter_serial = serialize ;	
		
	}
	else if ( filter_select == 2 )
	{
		stNRCTRL.st2DNR.filter_mode_df = mode ;	
	}
	else
	{
		stNRCTRL.st2DNR.c_filter_mode = mode ;
		stNRCTRL.st2DNR.c_filter_serial = serialize ;
	}
	
	stNRCTRL.ctrl |= (NRCTRL_2D_NB_UPDATE | NRCTRL_TRIGGER_ISP);	
#endif	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetNR_2D_Bypasslevel(BYTE nf_level , BYTE stdev_level)
{

	stNRCTRL.st2DNR.nf_bypass_level = nf_level ;				
	stNRCTRL.st2DNR.stdev_bypass_level = stdev_level ;	
		
	stNRCTRL.ctrl |= (NRCTRL_2D_NB_UPDATE | NRCTRL_TRIGGER_ISP);
}


//--------------------------------------------------------------------------------------------------------------------------
void SetNR_2D_NoiseBound(WORD table_no , BOOL filter_select , BYTE nb_bound )
{

	if ( filter_select == 0 )
	{
		stNRCTRL.st2DNR.noise_bound_no = table_no ;				
		stNRCTRL.st2DNR.noise_bound_mode = nb_bound ;				
	}
#if __NR_REV__
	else
	{
		stNRCTRL.st2DNR.noise_bound_no_df = table_no ;
		stNRCTRL.st2DNR.noise_bound_mode_df = nb_bound ;
	}	
#endif
	
	stNRCTRL.ctrl |= (NRCTRL_2D_NB_UPDATE | NRCTRL_TRIGGER_ISP);
}
//--------------------------------------------------------------------------------------------------------------------------
void SetNR_2D_Impulselevel(BYTE imp_level)
{
	stNRCTRL.st2DNR.rold_impulse_level = imp_level ;
	stNRCTRL.ctrl |= (NRCTRL_2D_NB_UPDATE | NRCTRL_TRIGGER_ISP);
}
//--------------------------------------------------------------------------------------------------------------------------
void SetNR_2D_ImpulseThresh(BOOL op_mode , BYTE ImpulseThresh , BOOL filter_select , BYTE ImpulseReplace )
{
	
	if ( filter_select == 0 )
	{
		if ( op_mode == 0 ) { stNRCTRL.st2DNR.rold_impulse_th = ImpulseThresh ; }
		else { stNRCTRL.st2DNR.rold_ll_impulse_th = ImpulseThresh ; }
	}
#if __NR_REV__	
	else
	{
		if ( op_mode == 0 ) { stNRCTRL.st2DNR.rold_impulse_th_df = ImpulseThresh ; }
		else { stNRCTRL.st2DNR.rold_ll_impulse_th_df = ImpulseThresh ; }
	}
#endif
	
	stNRCTRL.st2DNR.impulse_replace = ImpulseReplace ;
	
	stNRCTRL.ctrl |= (NRCTRL_2D_NB_UPDATE | NRCTRL_TRIGGER_ISP);
}
//--------------------------------------------------------------------------------------------------------------------------
void SetNR_UseWdrMotionFlag(BYTE mode)
{
#if __NR_REV__	
	switch(mode) {
		case 1:  // use as motion flag
			stNRCTRL.stWDR.wdr_tm_only_mode = 0;     // even line write
			stNRCTRL.stWDR.mo_his_rd_en = 0;         // enable read
			stNRCTRL.stWDR.mo_flag_rd_en = 1;         // enable read
			stNRCTRL.stWDR.r_wdr_mo_en = 1;          // use 1-bit as motion flag
			stNRCTRL.stWDR.r_color_suppress_en = 0;  // N/A
			stNRCTRL.stWDR.r_chk_surface = 0;        // N/A
			stNRCTRL.stWDR.r_2D_wdr_mo_mode = 1;     // 2D filter to the MGR motion area
			SetNR_IDM_mhist(0) ;
			break;
		case 2:  // use as suppress flag
			stNRCTRL.stWDR.wdr_tm_only_mode = 1;     // full line write
			stNRCTRL.stWDR.mo_his_rd_en = 1;         // enable read
			stNRCTRL.stWDR.mo_flag_rd_en = 1;         // enable read
			stNRCTRL.stWDR.r_wdr_mo_en = 1;          // use 1-bit as suppress flag
			stNRCTRL.stWDR.r_color_suppress_en = 1;  // enable color suppress
			stNRCTRL.stWDR.r_chk_surface = 1;        // suppress to the surface only
			stNRCTRL.stWDR.r_2D_wdr_mo_mode = 0;     // N/A
			SetNR_IDM_mhist(0) ;
			break;
		default:  // disable
			stNRCTRL.stWDR.wdr_tm_only_mode = 0;     // N/A
			stNRCTRL.stWDR.mo_his_rd_en = 0;         // disable read
			stNRCTRL.stWDR.mo_flag_rd_en = 0;         // disable read
			stNRCTRL.stWDR.r_wdr_mo_en = 0;          // N/A
			stNRCTRL.stWDR.r_color_suppress_en = 0;  // N/A
			stNRCTRL.stWDR.r_chk_surface = 0;        // N/A
			stNRCTRL.stWDR.r_2D_wdr_mo_mode = 0;     // N/A
			SetNR_IDM_mhist(0) ;
			break;
	}
	
	stNRCTRL.ctrl |= (NRCTRL_WDR_FLAG_UPDATE | NRCTRL_TRIGGER_ISP);	
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetNR_BitMode(BOOL mode)
{
	stNRCTRL.nr_10bit = mode ;
	stNRCTRL.ctrl |= (NRCTRL_IMAGE_SIZE_UPDATE | NRCTRL_TRIGGER_ISP);
	
	SetDDRMAP_NRImagetype_mode(stNRCTRL.image_type,stNRCTRL.nr_10bit); 	
}

BOOL GetNR_BitMode(void)
{
	return stNRCTRL.nr_10bit ;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetNR_LetzteresfilterEN(BYTE y_en, BYTE c_en ) 
{
#if __NR_L2DNR_CONTROL__	
	stNRCTRL.stL2DNR.y_en = y_en ;
	stNRCTRL.stL2DNR.c_en = c_en ;
	stNRCTRL.ctrl |= (NRCTRL_2D_LFILTER_UPDATE | NRCTRL_TRIGGER_ISP); 
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetNR_LetzteresfilterY(WORD value, WORD width ) 
{	
#if __NR_L2DNR_CONTROL__	
	WORD coef_00 ;
	WORD coef_01 ;
	WORD coef_02 ;
	WORD coef_03 ;
	
	if ( value > 1023 || width == 0 )	coef_00 = 1024 ;
	else								coef_00 = value ;
	
	if ( width == 0 ) {
		coef_01 = 0 ;
		coef_02 = 0 ;
		coef_03 = 0 ;
	}
	else if ( width == 1 ) { // 3-tab mode 		
		coef_01 = (1024 - coef_00)>>1 ;
		coef_02 = 0 ;
		coef_03 = 0 ;
	}
	else if ( width == 2 ){ // 5 tab mode
		coef_01 = (1024 - coef_00)>>2 ;
		coef_02 = (1024 - coef_00)>>2 ;
		coef_03 = 0 ;
	}
	else{ // 7tab mode  - not supported yet
		coef_01 = 0 ;
		coef_02 = 0 ;
		coef_03 = 0 ;		
	}

	stNRCTRL.stL2DNR.y_coef_00 = coef_00 ;
	stNRCTRL.stL2DNR.y_coef_01 = coef_01 ;
	stNRCTRL.stL2DNR.y_coef_02 = coef_02 ;
	stNRCTRL.stL2DNR.y_coef_03 = coef_03 ;		
	
	stNRCTRL.ctrl |= (NRCTRL_2D_LFILTER_UPDATE | NRCTRL_TRIGGER_ISP);
#endif
}

void SetNR_LetzteresfilterC(WORD value, WORD width ,WORD tint_adjust)
{
#if __NR_L2DNR_CONTROL__		
	WORD coef_01 ;
	WORD coef_02 ;
	WORD coef_03 ;
	
	if ( value > 1023 || width == 0 )	stNRCTRL.stL2DNR.c_coef_00 = 1024 ;
	else								stNRCTRL.stL2DNR.c_coef_00 = value ;
	
	if ( width == 0 ) {
		coef_01 = tint_adjust ;
		coef_02 = 0 ;
		coef_03 = 0 ;
	}
	else if ( width == 1 ) { // 3-tab mode 		
		coef_01 = (1024 - value)>>1 ;
		coef_02 = tint_adjust ;
		coef_03 = 0 ;
	}
	else if ( width == 2 ){ // 5 tab mode
		coef_01 = (1024 - value)>>2 ;
		coef_02 = (1024 - value)>>2 ;
		coef_03 = tint_adjust ;
	}
	else{ // 7tab mode  - not supported yet
		coef_01 = 0 ;
		coef_02 = 0 ;
		coef_03 = 0 ;		
	}

	stNRCTRL.stL2DNR.c_coef_01 = coef_01 ;
	stNRCTRL.stL2DNR.c_coef_02 = coef_02 ;
	stNRCTRL.stL2DNR.c_coef_03 = coef_03 ;		
	stNRCTRL.stL2DNR.c_tint = tint_adjust ;		

	stNRCTRL.ctrl |= (NRCTRL_2D_LFILTER_UPDATE | NRCTRL_TRIGGER_ISP); 
#endif
}
//--------------------------------------------------------------------------------------------------------------------------
void SetNR_LetzteresfilterLevel(BYTE level)
{
#if __NR_L2DNR_CONTROL__		
	switch(level)
	{
		case 0 : stNRCTRL.stL2DNR.coef_min = 1024  ; break ;	// OFF
		case 1 : stNRCTRL.stL2DNR.coef_min = 852 ; break ;	// low
		case 2 : stNRCTRL.stL2DNR.coef_min = 682 ; break ;	// mid
		case 3 : stNRCTRL.stL2DNR.coef_min = 512 ; break ;	// high
		default :  stNRCTRL.stL2DNR.coef_min = 1024 ; break ;	
	}	
#endif	
}

//--------------------------------------------------------------------------------------------------------------------------

void SetNR_Mittlerfilter(BYTE en, BYTE gain , BYTE offset)
{
#if	__NR_REV__
	stNRCTRL.stM2DNR.filter_en = en ;
	stNRCTRL.stM2DNR.th_gain = gain ;
	stNRCTRL.stM2DNR.th_offset = offset ;	
	
	stNRCTRL.ctrl |= (NRCTRL_2D_MFILTER_UPDATE | NRCTRL_TRIGGER_ISP); 
#endif	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetNR_MittlerfilterLevel(BYTE level)
{
#if __NR_REV__
#if __NR_SPLIT__==0	
	stNRCTRL.stM2DNR.filter_level = level ;
	
	switch(level)
	{
		case 0 : stNRCTRL.stM2DNR.th_offset_limit = 0  ; break ;	// OFF
		case 1 : stNRCTRL.stM2DNR.th_offset_limit = 10 ; break ;	// low
		case 2 : stNRCTRL.stM2DNR.th_offset_limit = 20 ; break ;	// mid
		case 3 : stNRCTRL.stM2DNR.th_offset_limit = 40 ; break ;	// high
		default :  stNRCTRL.stM2DNR.th_offset_limit = 10 ; break ;	
	}
	
	// UARTprintf("filter_level : [%d], [%d]\r\n", stNRCTRL.stM2DNR.filter_level , stNRCTRL.stM2DNR.th_offset_limit ) ;
#endif // #if __NR_SPLIT__==0
#endif
}

WORD GetNR_Strategy(void)
{
#if __NR_REV__
	return stNRCTRL.strategy ;
#endif
}
//--------------------------------------------------------------------------------------------------------------------------

void SetNRS_FNMisc(BYTE agc_max, BYTE under_exp, BYTE edge_level, BYTE lowlight_mode )
{
#if __NR_SPLIT__==2	
	stNRCTRL.stFRAMENOISE.agc_max = agc_max ;
	stNRCTRL.stFRAMENOISE.under_exp = under_exp ;
	
	MISP_RegField( MISP_LOCAL_ID, 0x53F, 8, 1, stNRCTRL.stFRAMENOISE.agc_max) ;
	MISP_RegField( MISP_LOCAL_ID, 0x53F, 14, 2, stNRCTRL.stFRAMENOISE.under_exp) ;
	
	if ( edge_level != stNRCTRL.stFRAMENOISE.edge_level ) MISP_RegField(MISP_LOCAL_ID, 0x52A, 7 ,7 , edge_level) ;		
	
	stNRCTRL.stFRAMENOISE.edge_level = edge_level ;	
	
	// GetNR_LowLightMode() function returns MCU drive operation mode flag when NR split mode
	if ( GetNR_LowLightMode() != lowlight_mode ) 
	{
		SetNR_MCUDriveMode(1,lowlight_mode,0,0,0); 
		stNRCTRL.lowlight_mode = GetNR_LowLightMode(); // GetNR_LowLightMode() function returns MCU drive operation mode flag when NR split mode
	}
	
#endif // #if __NR_SPLIT__==2	
}
//-------------------------------------------------
void SetNRS_FNUpdateCounter(BYTE update_counter)
{
#if __NR_SPLIT__==2		
	stNRCTRL.st3DNR.update_counter = update_counter ;	
	NR_ProcessSlaveHandler() ;
#endif // #if __NR_SPLIT__==2		
}
//-------------------------------------------------
void NR_ProcessSlaveHandler(void)
{
#if __NR_SPLIT__==2		
	
	NRCTRL_AUTOIDM(&stNRCTRL);	
	NRCTRL_AUTOPDM(&stNRCTRL);	
	NRCTRL_AUTOSRM(&stNRCTRL);		
	
	NRCTRL_AUTOIDMc(&stNRCTRL);
	
	// NRCTRL_MCUDriveReturn(&stNRCTRL);
	NRCTRL_AUTO3DNR(&stNRCTRL);	
		
#if __NR_REV__
	NRCTRL_AUTO2DNR(&stNRCTRL) ;
	NRCTRL_AUTOM2DNR(&stNRCTRL) ;
#endif
	
	// NRCTRL_AUTOL2DNR(&stNRCTRL) ;
		
	if ((stNRCTRL.ctrl&NRCTRL_TRIGGER_ISP)==0) return;
	stNRCTRL.ctrl &= (~NRCTRL_TRIGGER_ISP);
	
	NRCTRL_MCUDriveHandler(&stNRCTRL);
	
	// NRCTRL_InputImageTypeHandler(&stNRCTRL);
	// NRCTRL_DataOrderHandler(&stNRCTRL);
	NRCTRL_ModeHandler(&stNRCTRL);
	NRCTRL_ImageSizeHandler(&stNRCTRL);
	NRCTRL_3DintensityHandler(&stNRCTRL);
	NRCTRL_LowLightLevelHandler(&stNRCTRL);
	NRCTRL_DemoModeHandler(&stNRCTRL);	
	
	NRCTRL_IDMHandler(&stNRCTRL);	
	NRCTRL_IDMcHandler(&stNRCTRL);
	
	NRCTRL_PDMHandler(&stNRCTRL) ;
	NRCTRL_SRMHandler(&stNRCTRL) ;	
	
#if __NR_REV__
	NRCTRL_2D_sFilterHandler(&stNRCTRL);
	NRCTRL_2D_MFilterHandler(&stNRCTRL);
#endif	
	
	NRCTRL_2D_NBHandler (&stNRCTRL) ;
		
	NRCTRL_2D_LFilterHandler(&stNRCTRL);
	
	NRCTRL_WdrFlagControlHandler(&stNRCTRL);	
	
#endif // #if __NR_SPLIT__==2		
}

//-------------------------------------------------
void SetNRS_FNStdev(BYTE StDev_int, BYTE StDev_frac )
{ // NR split mode slave function. Frame noise & AE status data from Master(__NR_SPLIT__==1)
#if __NR_SPLIT__==2	
	stNRCTRL.stFRAMENOISE.stdev_int = (WORD)StDev_int ;
	stNRCTRL.stFRAMENOISE.stdev_frac = (WORD)StDev_frac ;
#endif // #if __NR_SPLIT__==2	
}
//-------------------------------------------------
void SetNRS_FNStdevBr(BYTE StDev_int, BYTE StDev_frac )
{
#if __NR_SPLIT__==2		
	stNRCTRL.stFRAMENOISE.stdev_int_br = (WORD)StDev_int ;
	stNRCTRL.stFRAMENOISE.stdev_frac_br = (WORD)StDev_frac ;	
#endif // #if __NR_SPLIT__==2		
}
//-------------------------------------------------
void SetNRS_FNStdevDa(BYTE StDev_int, BYTE StDev_frac )
{
#if __NR_SPLIT__==2		
	stNRCTRL.stFRAMENOISE.stdev_int_da = (WORD)StDev_int ;
	stNRCTRL.stFRAMENOISE.stdev_frac_da = (WORD)StDev_frac ;
#endif // #if __NR_SPLIT__==2		
}
//--------------------------------------------------------------------------------------------------------------------------
/*  FILE_END_HERE */
