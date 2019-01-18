// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#include	"misp100.h"

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550

// #include	"misprgb_top.h"
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static TONE_CTRL_PARAM stTONECTRL;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
#if __USE_GAMMA_BYTEDATA__
extern ROMDATA BYTE gamma_value_table[18][32] ;
extern ROMDATA BYTE gamma256_value_table[19][256] ;
#if defined(__RGBGAMMA_USE_ADJUST__)
extern ROMDATA BYTE RGBgamma_contrast_table[23][256] ;
#endif
#else
extern ROMDATA BYTE gamma_value_table[18][16] ;
#endif

extern ROMDATA BYTE YCgamma_table[10][256] ;

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
static void Init_TONE_PARAM( PTONE_CTRL_PARAM pCTRL ) ;
static void Init_TONE_REGISTER( PTONE_CTRL_PARAM pCTRL ) ;
static void TONECTRL_RGBGammaEnHandler( PTONE_CTRL_PARAM pCTRL ) ;
static void TONECTRL_RGBGammaExtendHandler( PTONE_CTRL_PARAM pCTRL ) ;
static void TONECTRL_FrameDelayHandler( PTONE_CTRL_PARAM pCTRL ) ;
static void TONECTRL_RGBGammaHandler( PTONE_CTRL_PARAM pCTRL ) ;
static void TONECTRL_BYIGammaEnHandler(PTONE_CTRL_PARAM pCTRL) ;
static void TONECTRL_BYIGammaHandler(PTONE_CTRL_PARAM pCTRL) ;
static void TONECTRL_YCGammaEnHandler(PTONE_CTRL_PARAM pCTRL) ;
static void TONECTRL_YCGammaExtendHandler(PTONE_CTRL_PARAM pCTRL) ;
static void TONECTRL_YCGammaHandler(PTONE_CTRL_PARAM pCTRL) ;
static void TONECTRL_AYCGammaEnHandler(PTONE_CTRL_PARAM pCTRL) ;
static void TONECTRL_AYCGammaExtendHandler(PTONE_CTRL_PARAM pCTRL) ;
static void TONECTRL_AYCGammaHandler(PTONE_CTRL_PARAM pCTRL) ;

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------
static void Init_TONE_PARAM( PTONE_CTRL_PARAM pCTRL ){
	// BOOL gamma_en ;
	WORD channel_select ;
	WORD gamma_table_number ;
	WORD contrast_table_number ;
	
//	gamma_en = GetBY_gamma_mode() ;
	channel_select = 0 ; // 0: all 1: G 2: B 3: R
	gamma_table_number = 0;	

	contrast_table_number = 11 ; // contrast curve for match the gray target

//	pCTRL->stRGB_GAMMA.gamma_lut = MISP_LUT_64x256;	//	pCTRL->stRGB_GAMMA.gamma_lut = 2;
//	pCTRL->stYC_GAMMA.gamma_lut = MISP_LUT_64x256;		//	
//	pCTRL->stAYC_GAMMA.gamma_lut = MISP_LUT_64x256;		//	

	SetTONE_BYIGAMMA_enable(OFF) ; // BY inverse ON/OFF
	SetTONE_BYIGAMMA_LUT(0) ; 

#if defined(__USE_EXT2IPP__) ||  defined(__USE_NO__CMOS__)
	SetTONE_RGBGAMMA_enable(OFF) ;
	SetTONE_RGB256LUT_Extend(OFF) ;
#else
	SetTONE_RGBGAMMA_enable(ON) ;
	SetTONE_RGB256LUT_Extend(OFF) ;
#endif

#if __TONE_RGB_BOOT_THERMO_ENABLE__		//set main path thermo
//	SetTONE_Thermo_RGBGAMMA(2);
	SetTONE_Thermo_RGBGAMMA(1);
	#if defined(__RGBGAMMA_USE_ADJUST__)
	pCTRL->stRGB_GAMMA.contrast = contrast_table_number;
	#endif
#else
	SetTONE_RGBLUT_Type(MISP_LUT_64x256) ;
	pCTRL->stRGB_GAMMA.thermo_mode = 0;		

	#if defined(__RGBGAMMA_USE_ADJUST__)
	SetTONE_RGBGAMMA_val(contrast_table_number);	// contrast curve mode uses contrast API
	// SetTONE_RGBGAMMA_LUT(channel_select,pCTRL->stRGB_GAMMA.contrast );
	#else
	SetTONE_RGBGAMMA_LUT(channel_select,gamma_table_number); // gamma curve mode uses gamma API
	#endif	
#endif

	SetTONE_YCLUT_Type(MISP_LUT_64x256) ;	
	SetTONE_YCGAMMA_enable(ON) ;
	SetTONE_YC256LUT_Extend(OFF) ;
	SetTONE_YCGAMMA_LUT(0,3) ; // Channel sel , table number => YCbCr - 1.0
	
	// Aux YC gamma basic lut => Thermo => YCbCr - 1.0
	SetTONE_AYCGAMMA_enable(ON) ;
	SetTONE_AYCLUT_Type(MISP_LUT_64x256) ; // YCbCr - 1.0	
	SetTONE_AYC256LUT_Extend(OFF) ;
	SetTONE_AYCGAMMA_LUT(0,3) ; // Channel sel , table number => YCbCr - 1.0	
	SetTONE_Thermo_AYCGAMMA(__TONE_AYC_BOOT_TERMO_ENABLE__) ; // Thermo
}

static void Init_TONE_REGISTER ( PTONE_CTRL_PARAM pCTRL ){
	
	TONECTRL_BYIGammaEnHandler(pCTRL) ;
	TONECTRL_BYIGammaHandler(pCTRL) ;
	
	TONECTRL_RGBGammaEnHandler(pCTRL) ;
	TONECTRL_RGBGammaExtendHandler(pCTRL);	
	TONECTRL_RGBGammaHandler(pCTRL) ;
	
	TONECTRL_YCGammaEnHandler(pCTRL) ;	
	TONECTRL_YCGammaExtendHandler(pCTRL) ;		
	TONECTRL_YCGammaHandler(pCTRL) ;

	TONECTRL_AYCGammaEnHandler(pCTRL) ;		// AYC LUT initial at main ctrl loop
	TONECTRL_AYCGammaExtendHandler(pCTRL) ;		
	TONECTRL_AYCGammaHandler(pCTRL) ;	
}

// ----------------------------------------------------------------------

void Init_TONE(void) {

	memset(&stTONECTRL, 0, sizeof(TONE_CTRL_PARAM));
	
	Init_TONE_PARAM(&stTONECTRL);
	Init_TONE_REGISTER(&stTONECTRL);
	
	// stTONECTRL.ctrl = 0 ; // For AYC LUT initial
}

// ----------------------------------------------------------------------

static void TONECTRL_RGBGammaEnHandler( PTONE_CTRL_PARAM pCTRL ) {
	if ((pCTRL->ctrl&TONECTRL_GAMMA_EN_UPDATE)==0) return ;
	pCTRL->ctrl &= (~TONECTRL_GAMMA_EN_UPDATE);
		
	MISP_RegField(MISP_LOCAL_ID, __RGBGAMMA_CTRL_ADDR__, 0, 1, pCTRL->stRGB_GAMMA.gamma_en) ;
	MISP_RegField(MISP_LOCAL_ID, __RGBGAMMA_CTRL_ADDR__, 2, 1, pCTRL->stRGB_GAMMA.gamma_en) ; // '0': output max is fixed to all ones, '1': output max can be less than all ones
//	MISP_RegField(MISP_LOCAL_ID, __RGBGAMMA_CTRL_ADDR__, 1, 1, 1) ;	//lut_acc_on_blank = access on blank		
}
// ----------------------------------------------------------------------
static void TONECTRL_RGBGammaExtendHandler( PTONE_CTRL_PARAM pCTRL ) {
	if ((pCTRL->ctrl&TONECTRL_GAMMA_EXTEND_UPDATE)==0) return ;
	pCTRL->ctrl &= (~TONECTRL_GAMMA_EXTEND_UPDATE);
		
	MISP_RegField(MISP_LOCAL_ID, __RGBGAMMA_CTRL_ADDR__, 3, 2, pCTRL->stRGB_GAMMA.gamma_extend) ;
	MISP_RegField(MISP_LOCAL_ID, __RGBGAMMA_CTRL_ADDR__, 5, 2, pCTRL->stRGB_GAMMA.gamma_extend) ;
	MISP_RegField(MISP_LOCAL_ID, __RGBGAMMA_CTRL_ADDR__, 7, 2, pCTRL->stRGB_GAMMA.gamma_extend) ;		
}

// ----------------------------------------------------------------------
static void TONECTRL_RGBGammaHandler( PTONE_CTRL_PARAM pCTRL ) {
	WORD i ;
 	PBYTE gamma_table_ptr;	
	
	if ((pCTRL->ctrl&TONECTRL_GAMMA_LUT_UPDATE)==0) return ;
	pCTRL->ctrl &= (~TONECTRL_GAMMA_LUT_UPDATE);	

	if(GetTONE_RGBGAMMA_thermo()==2){		//multy call...
		UARTprintf("***** [Register Dump : IR LUT  ]****** \r\n");
		
		gamma_table_ptr = (PBYTE)YCgamma_table[11];
		MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_COMMAND_ADDR__, 0x8000|1<<12) ; // LUT G write enable
		for ( i = 0 ; i < 256 ; i++ )
		{
			MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_DATA_ADDR__, gamma_table_ptr[i]) ; // LUT write
		}
		MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_COMMAND_ADDR__, 0x0000) ; // LUT write disable

		gamma_table_ptr = (PBYTE)YCgamma_table[12];
		MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_COMMAND_ADDR__, 0x8000|2<<12) ; // LUT B write enable
		for ( i = 0 ; i < 256 ; i++ )
		{
			MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_DATA_ADDR__, gamma_table_ptr[i]) ; // LUT write
		}
		MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_COMMAND_ADDR__, 0x0000) ; // LUT write disable

		gamma_table_ptr = (PBYTE)YCgamma_table[10];
		MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_COMMAND_ADDR__, 0x8000|4<<12) ; // LUT R write enable
		for ( i = 0 ; i < 256 ; i++ )
		{
			MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_DATA_ADDR__, gamma_table_ptr[i]) ; // LUT write
		}
		MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_COMMAND_ADDR__, 0x0000) ; // LUT write disable

		MISP_RegField(MISP_LOCAL_ID, __RGBGAMMA_CTRL_ADDR__, 15, 1, 0) ;	// LUT->CSC
		MISP_RegField(MISP_LOCAL_ID, __RGBGAMMA_CTRL_ADDR__, 10, 1, 1) ;	// dis_itp_output			
	}
	else if(GetTONE_RGBGAMMA_thermo()==1){

		UARTprintf("***** [Register Dump : Thermo LUT  ]****** \r\n");

		//register dump to test
		gamma_table_ptr = (PBYTE)YCgamma_table[7];
		MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_COMMAND_ADDR__, 0x8000|1<<12) ; // LUT G write enable
		for ( i = 0 ; i < 256 ; i++ )
		{
			MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_DATA_ADDR__, gamma_table_ptr[i]) ; // LUT write
		}
		MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_COMMAND_ADDR__, 0x0000) ; // LUT write disable

		gamma_table_ptr = (PBYTE)YCgamma_table[9];
		MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_COMMAND_ADDR__, 0x8000|2<<12) ; // LUT B write enable
		for ( i = 0 ; i < 256 ; i++ )
		{
			MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_DATA_ADDR__, gamma_table_ptr[i]) ; // LUT write
		}
		MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_COMMAND_ADDR__, 0x0000) ; // LUT write disable

		gamma_table_ptr = (PBYTE)YCgamma_table[8];
		MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_COMMAND_ADDR__, 0x8000|4<<12) ; // LUT R write enable
		for ( i = 0 ; i < 256 ; i++ )
		{
			MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_DATA_ADDR__, gamma_table_ptr[i]) ; // LUT write
		}
		MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_COMMAND_ADDR__, 0x0000) ; // LUT write disable

		MISP_RegField(MISP_LOCAL_ID, __RGBGAMMA_CTRL_ADDR__, 15, 1, 0) ;	// LUT->CSC
		MISP_RegField(MISP_LOCAL_ID, __RGBGAMMA_CTRL_ADDR__, 10, 1, 1) ;	// dis_itp_output		

	}
	else{

#if defined(__RGBGAMMA_USE_ADJUST__)
	gamma_table_ptr = (PBYTE)RGBgamma_contrast_table[pCTRL->stRGB_GAMMA.contrast];			
#else
	gamma_table_ptr = (PBYTE)gamma256_value_table[pCTRL->stRGB_GAMMA.gamma_table_number];			//if 1.0 then bypassed
#endif

	// MISP_RegField(MISP_LOCAL_ID, __RGBGAMMA_CTRL_ADDR__, 0, 1, 0) ; // Gamma off
	
	MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_COMMAND_ADDR__, 0x8000|pCTRL->stRGB_GAMMA.channel_select<<12) ; // LUT write enable
	
	
	// UARTprintf("***** [WRITING GAMMA LUT] \n");
	if(pCTRL->stRGB_GAMMA.gamma_extend)
	{
		for ( i = 0 ; i < 256 ; i++ )
		{
			if(i<128){
				if(i%2) 	MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_DATA_ADDR__, (gamma_table_ptr[i/2] + gamma_table_ptr[i/2+1])/2); // LUT write
				else		MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_DATA_ADDR__, gamma_table_ptr[i/2]); // LUT write
			}
			else if(i<192) MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_DATA_ADDR__, gamma_table_ptr[i-64]); // LUT write
			else			 MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_DATA_ADDR__, gamma_table_ptr[(i-192)*2+128]); // LUT write
		}	
	}
	else
	{
		for ( i = 0 ; i < 256 ; i++ )
		{
			MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_DATA_ADDR__, gamma_table_ptr[i]) ; // LUT write
		}
	}
	
	// UARTprintf("***** [WRITING GAMMA LUT DONE] \n");
	
	MISP_RegWrite(MISP_LOCAL_ID, __RGBGAMMA_COMMAND_ADDR__, 0x0000) ; // LUT write disable
	
//	MISP_RegField(MISP_LOCAL_ID, __RGBGAMMA_CTRL_ADDR__, 0, 1, pCTRL->stRGB_GAMMA.gamma_en) ;
//	MISP_RegField(MISP_LOCAL_ID, __RGBGAMMA_CTRL_ADDR__, 11, 2, pCTRL->stRGB_GAMMA.gamma_lut) ;

		MISP_RegField(MISP_LOCAL_ID, __RGBGAMMA_CTRL_ADDR__, 15, 1, 1) ;	// LUT->CSC
		MISP_RegField(MISP_LOCAL_ID, __RGBGAMMA_CTRL_ADDR__, 10, 1, 0) ;	// dis_itp_output		

	}
	
}



// ----------------------------------------------------------------------
static void TONECTRL_BYIGammaEnHandler(PTONE_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&TONECTRL_BYIGAMMA_EN_UPDATE)==0) return ;
	pCTRL->ctrl &= (~TONECTRL_BYIGAMMA_EN_UPDATE);		
	
	MISP_RegField(MISP_LOCAL_ID, 0x473, 14, 2, ((pCTRL->stBYI_GAMMA.gamma_en*3))) ;	// BYI Gamma on
}

// ----------------------------------------------------------------------

static void TONECTRL_BYIGammaHandler(PTONE_CTRL_PARAM pCTRL)
{
	WORD gamma_value ;
    PBYTE gamma_table_ptr_x;
    PBYTE gamma_table_ptr_y;
 
	if ((pCTRL->ctrl&TONECTRL_BYIGAMMA_LUT_UPDATE)==0) return ;
	pCTRL->ctrl &= (~TONECTRL_BYIGAMMA_LUT_UPDATE);	
	
#if __USE_INDIRECT_BYGAMMA__==2		//edit by sachem04 to maching 256 F.gamma table

	gamma_value = pCTRL->stBYI_GAMMA.gamma_table_number ;
	gamma_table_ptr_x = (PBYTE)gamma256_value_table[gamma_value];
	gamma_table_ptr_y = (PBYTE)gamma256_value_table[0];

	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_COMMAND_ADDR__, 0x8000) ; // LUT write enable + start addr
	
	// Wirte 8 point x ( 10bit )
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__, 0x0000) ; // LUT write offset x min 
		
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_x[4]<<2)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_x[8]<<2)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_x[16]<<2)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_x[32]<<2)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_x[64]<<2)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_x[128]<<2)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_x[192]<<2)) ; // LUT write
	
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__, 0x03FF) ; // LUT write offset x max
	
	// Wirte 8 point y ( 12bit )
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__, 0x0000) ; // LUT write offset y min 

		
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_y[4]<<4)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_y[8]<<4)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_y[16]<<4)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_y[32]<<4)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_y[64]<<4)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_y[128]<<4)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_y[192]<<4)) ; // LUT write			

	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__, 0x0FFF) ; // LUT write offset y max 

	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_COMMAND_ADDR__, 0x0000) ; // LUT write disable		

	
	MISP_RegField(MISP_LOCAL_ID, 0x473, 14, 2, ((pCTRL->stBYI_GAMMA.gamma_en*3))) ;	// BYI Gamma on

#else	//orginal

	gamma_value = GetBY_gamma_8tap() ;
	gamma_table_ptr_x = (PBYTE)gamma_value_table[gamma_value];
	gamma_table_ptr_y = (PBYTE)gamma_value_table[0];

	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_COMMAND_ADDR__, 0x8000) ; // LUT write enable + start addr

	// Wirte 8 point x ( 10bit )
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__, 0x0000) ; // LUT write offset x min 
		
	// UARTprintf("***** [WRITING GAMMA LUT X] \n");
	// UARTprintf("***** [WRITING GAMMA LUT X] : %d \n" ,  (gamma_table_ptr_x[0]<<2) );
	// UARTprintf("***** [WRITING GAMMA LUT X] : %d \n" ,  (gamma_table_ptr_x[2]<<2) );
	// UARTprintf("***** [WRITING GAMMA LUT X] : %d \n" ,  (gamma_table_ptr_x[5]<<2) );
	// UARTprintf("***** [WRITING GAMMA LUT X] : %d \n" ,  (gamma_table_ptr_x[8]<<2) );
	// UARTprintf("***** [WRITING GAMMA LUT X] : %d \n" ,  (gamma_table_ptr_x[11]<<2) );
	// UARTprintf("***** [WRITING GAMMA LUT X] : %d \n" ,  (gamma_table_ptr_x[18]<<2) );
	// UARTprintf("***** [WRITING GAMMA LUT X] : %d \n" ,  (gamma_table_ptr_x[26]<<2) );
	
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_x[0]<<2)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_x[2]<<2)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_x[5]<<2)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_x[8]<<2)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_x[11]<<2)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_x[18]<<2)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_x[26]<<2)) ; // LUT write
	
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__, 0x03FF) ; // LUT write offset x max
	
	// Wirte 8 point y ( 12bit )
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__, 0x0000) ; // LUT write offset y min 

	// UARTprintf("***** [WRITING GAMMA LUT Y] \n");
	// UARTprintf("***** [WRITING GAMMA LUT Y] : %d \n" ,  (gamma_table_ptr_y[0]<<4) );
	// UARTprintf("***** [WRITING GAMMA LUT Y] : %d \n" ,  (gamma_table_ptr_y[2]<<4) );
	// UARTprintf("***** [WRITING GAMMA LUT Y] : %d \n" ,  (gamma_table_ptr_y[5]<<4) );
	// UARTprintf("***** [WRITING GAMMA LUT Y] : %d \n" ,  (gamma_table_ptr_y[8]<<4) );
	// UARTprintf("***** [WRITING GAMMA LUT Y] : %d \n" ,  (gamma_table_ptr_y[11]<<4) );
	// UARTprintf("***** [WRITING GAMMA LUT Y] : %d \n" ,  (gamma_table_ptr_y[18]<<4) );
	// UARTprintf("***** [WRITING GAMMA LUT Y] : %d \n" ,  (gamma_table_ptr_y[26]<<4) );			
		
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_y[0]<<4)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_y[2]<<4)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_y[5]<<4)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_y[8]<<4)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_y[11]<<4)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_y[18]<<4)) ; // LUT write
	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__,  (gamma_table_ptr_y[26]<<4)) ; // LUT write			

	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_DATA_ADDR__, 0x0FFF) ; // LUT write offset y max 

	MISP_RegWrite(MISP_LOCAL_ID, __BYIGAMMA_COMMAND_ADDR__, 0x0000) ; // LUT write disable		

	// UARTprintf("***** [WRITING GAMMA LUT DONE] \n");
		
	MISP_RegField(MISP_LOCAL_ID, 0x473, 14, 2, ((pCTRL->stBYI_GAMMA.gamma_en*3))) ;	// BYI Gamma on

#endif
}

// ----------------------------------------------------------------------
static void TONECTRL_YCGammaEnHandler(PTONE_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&TONECTRL_YCGAMMA_EN_UPDATE)==0) return ;
	pCTRL->ctrl &= (~TONECTRL_YCGAMMA_EN_UPDATE);
		
	MISP_RegField(MISP_LOCAL_ID, __YCGAMMA_CTRL_ADDR__, 0, 1, pCTRL->stYC_GAMMA.gamma_en) ;	
}
// ----------------------------------------------------------------------
static void TONECTRL_YCGammaExtendHandler( PTONE_CTRL_PARAM pCTRL ) 
{
	if ((pCTRL->ctrl&TONECTRL_YCGAMMA_EXTEND_UPDATE)==0) return ;
	pCTRL->ctrl &= (~TONECTRL_YCGAMMA_EXTEND_UPDATE);
			
	MISP_RegField(MISP_LOCAL_ID, __YCGAMMA_CTRL_ADDR__, 3, 2, pCTRL->stYC_GAMMA.gamma_extend) ;
//	MISP_RegField(MISP_LOCAL_ID, __YCGAMMA_CTRL_ADDR__, 5, 2, pCTRL->stYC_GAMMA.gamma_extend) ;
//	MISP_RegField(MISP_LOCAL_ID, __YCGAMMA_CTRL_ADDR__, 7, 2, pCTRL->stYC_GAMMA.gamma_extend) ;		
}

// ----------------------------------------------------------------------
static void TONECTRL_YCGammaHandler(PTONE_CTRL_PARAM pCTRL)
{

	WORD i, j ;
   	PBYTE gamma_table_ptr;
	
	BYTE update_case ;
	BYTE update_queue ;
	BYTE table_y ;
	BYTE table_cb ;
	BYTE table_cr ;
	BYTE command_queue[3] ;
	BYTE table_queue[3] ;
		
	if ((pCTRL->ctrl&TONECTRL_YCGAMMA_UPDATE)==0) return;
	pCTRL->ctrl &= (~TONECTRL_YCGAMMA_UPDATE);	
	
	update_case =  pCTRL->stYC_GAMMA.channel_select ;
	table_y =  pCTRL->stYC_GAMMA.Y_gamma_table_number ;
	table_cb =  pCTRL->stYC_GAMMA.Cb_gamma_table_number ;
	table_cr =  pCTRL->stYC_GAMMA.Cr_gamma_table_number ;
	
	// switch (channel_sel){
	// case 0 : stTONECTRL.stYC_GAMMA.channel_select |= 7 ; break ; // all
	// case 1 : stTONECTRL.stYC_GAMMA.channel_select |= 1 ; break ; // Y
	// case 2 : stTONECTRL.stYC_GAMMA.channel_select |= 2 ; break ; // Cb
	// case 3 : stTONECTRL.stYC_GAMMA.channel_select |= 4 ; break ; // Cr
	// default: stTONECTRL.stYC_GAMMA.channel_select |= 7 ; // all
	// }		
	
	switch(update_case)
	{
		case 3 :  // Y, Cb update
			if ( table_y == table_cb ) 
			{ 
				update_queue = 1 ; 
				command_queue[0] = update_case ; 
				table_queue[0] = table_y ; 
			}
			else 
			{ 
				update_queue = 2 ; 
				command_queue[1] = ((update_case&0x02)) ;	command_queue[0] = ((update_case&0x01)) ; 
				table_queue[1] = table_cb ; 				table_queue[0] = table_y ;
			}

			break ;
		case 5 : // Y, Cr update
			if ( table_y == table_cr ) 
			{ 
				update_queue = 1 ; 
				command_queue[0] = update_case ; 
				table_queue[0] = table_y ; 
			}
			else 
			{ 
				update_queue = 2 ; 
				command_queue[1] = ((update_case&0x04)) ; 	command_queue[0] = ((update_case&0x01)) ; 
				table_queue[1] = table_cr ; 					table_queue[0] = table_y ;
			}

			break ;			
		case 6 : // Cb, Cr update
			if ( table_cb == table_cr ) 
			{ 
				update_queue = 1 ; 
				command_queue[0] = update_case ; 
				table_queue[0] = table_cb ; 
			}
			else 
			{ 
				update_queue = 2 ; 
				command_queue[1] = ((update_case&0x04)) ; 	command_queue[0] = ((update_case&0x02)) ; 
				table_queue[1] = table_cr ; 				table_queue[0] = table_cb ;
			}

			break ;			
		case 7 : 
			if ( table_y == table_cb ) 
			{	// same 
				if ( table_y == table_cr ) 
				{ 
					update_queue = 1 ; 
					command_queue[0] = update_case ; 
					table_queue[0] = table_y ; 
				}					
				else 
				{ 
					update_queue = 2 ; 
					command_queue[1] = ((update_case&0x04)) ; 	command_queue[0] = ((update_case&0x03)) ; 
					table_queue[1] = table_cr ; 				table_queue[0] = table_y ;
				}
			}
			else if ( table_y == table_cr )
			{
				update_queue = 2 ; 
				command_queue[1] = ((update_case&0x02)) ; 	command_queue[0] = ((update_case&0x06)) ; 
				table_queue[1] = table_cb ; 				table_queue[0] = table_y ;
			}
			else
			{
				if ( table_cb == table_cr ) 
				{ 
					update_queue = 2 ; 
					command_queue[1] = ((update_case&0x06)) ; 	command_queue[0] = ((update_case&0x01)) ; 
					table_queue[1] = table_cb ; 					table_queue[0] = table_y ;
				}
				else 
				{ 
					update_queue = 3 ; 
					command_queue[2] = ((update_case&0x04)) ; 	command_queue[1] = ((update_case&0x02)) ; 	command_queue[0] = ((update_case&0x01)) ; 
					table_queue[2] = table_cr ; 				table_queue[1] = table_cb ; 				table_queue[0] = table_y ;
				}
			}
			break ;
		default : 
			update_queue = 1 ; 
			command_queue[0] = update_case ; 
			if ( update_case == 1 ) table_queue[0] = table_y ;
			else if ( update_case == 2 ) table_queue[0] = table_cb ;
			else table_queue[0] = table_cr ;
			
			break ;
	}
	
	// UARTprintf("***** [update_case :%d] \n" , update_case );
	// UARTprintf("***** [update_queue :%d] \n" , update_queue );
	// UARTprintf("***** [command_queue[0] :%d] \n" , command_queue[0] );
	// UARTprintf("***** [command_queue[1] :%d] \n" , command_queue[1] );
	// UARTprintf("***** [command_queue[2] :%d] \n" , command_queue[2] );
	
	// UARTprintf("***** [table_queue[0] :%d] \n" , table_queue[0] );
	// UARTprintf("***** [table_queue[1] :%d] \n" , table_queue[1] );
	// UARTprintf("***** [table_queue[2] :%d] \n" , table_queue[2] );
	
	for ( i = 0 ; i < update_queue ; i++ )
	{
		// gamma_table_ptr = (PBYTE)gamma256_value_table[table_queue[i]];
		gamma_table_ptr = (PBYTE)YCgamma_table[table_queue[i]];

		MISP_RegWrite(MISP_LOCAL_ID, __YCGAMMA_COMMAND_ADDR__, 0x8000|(command_queue[i])<<12) ; // LUT write enable
		
		// UARTprintf("***** [YC CMD addr %d] :%d ,%d \r\n", __YCGAMMA_COMMAND_ADDR__,(0x8000|(command_queue[i])<<12),command_queue[i]);
		// UARTprintf("***** [WRITING GAMMA LUT] \n");
		
		for ( j = 0 ; j < 256 ; j++ )
		{
			MISP_RegWrite(MISP_LOCAL_ID, __YCGAMMA_DATA_ADDR__, gamma_table_ptr[j]) ; // LUT write
		}
		
		// UARTprintf("***** [WRITING GAMMA LUT DONE] \n");
		
		MISP_RegField(MISP_LOCAL_ID, __YCGAMMA_COMMAND_ADDR__, 12, 4, 0x0) ; // LUT write disable		
	}	
	
//	MISP_RegField(MISP_LOCAL_ID, __YCGAMMA_CTRL_ADDR__, 0, 1, pCTRL->stYC_GAMMA.gamma_en) ;			
	
	pCTRL->stYC_GAMMA.channel_select = 0 ; // clear the value	

}

// ----------------------------------------------------------------------
static void TONECTRL_AYCGammaEnHandler(PTONE_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&TONECTRL_AYCGAMMA_EN_UPDATE)==0) return ;
	pCTRL->ctrl &= (~TONECTRL_AYCGAMMA_EN_UPDATE);
	
	MISP_RegField(MISP_LOCAL_ID, __AYCGAMMA_CTRL_ADDR__, 0, 1, pCTRL->stAYC_GAMMA.gamma_en) ;	
}
// ----------------------------------------------------------------------
static void TONECTRL_AYCGammaExtendHandler( PTONE_CTRL_PARAM pCTRL ) 
{
	if ((pCTRL->ctrl&TONECTRL_AYCGAMMA_EXTEND_UPDATE)==0) return ;
	pCTRL->ctrl &= (~TONECTRL_AYCGAMMA_EXTEND_UPDATE);
	
	MISP_RegField(MISP_LOCAL_ID, __AYCGAMMA_CTRL_ADDR__, 3, 2, pCTRL->stAYC_GAMMA.gamma_extend) ; // Y
//	MISP_RegField(MISP_LOCAL_ID, __AYCGAMMA_CTRL_ADDR__, 5, 2, pCTRL->stAYC_GAMMA.gamma_extend) ; // Cb
//	MISP_RegField(MISP_LOCAL_ID, __AYCGAMMA_CTRL_ADDR__, 7, 2, pCTRL->stAYC_GAMMA.gamma_extend) ; // Cr
}
// ----------------------------------------------------------------------
static void TONECTRL_AYCGammaHandler(PTONE_CTRL_PARAM pCTRL)
{

	WORD i, j ;
   	PBYTE gamma_table_ptr;
	
	BYTE update_case ;
	BYTE update_queue ;
	BYTE table_y ;
	BYTE table_cb ;
	BYTE table_cr ;
	BYTE command_queue[3] ;
	BYTE table_queue[3] ;
	
	if ((pCTRL->ctrl&TONECTRL_AYCGAMMA_UPDATE)==0) return;
	pCTRL->ctrl &= (~TONECTRL_AYCGAMMA_UPDATE);	
	
	update_case =  pCTRL->stAYC_GAMMA.channel_select ;
	table_y =  pCTRL->stAYC_GAMMA.Y_gamma_table_number ;
	table_cb =  pCTRL->stAYC_GAMMA.Cb_gamma_table_number ;
	table_cr =  pCTRL->stAYC_GAMMA.Cr_gamma_table_number ;
	
	// switch (channel_sel){
	// case 0 : stTONECTRL.stYC_GAMMA.channel_select |= 7 ; break ; // all
	// case 1 : stTONECTRL.stYC_GAMMA.channel_select |= 1 ; break ; // Y
	// case 2 : stTONECTRL.stYC_GAMMA.channel_select |= 2 ; break ; // Cb
	// case 3 : stTONECTRL.stYC_GAMMA.channel_select |= 4 ; break ; // Cr
	// default: stTONECTRL.stYC_GAMMA.channel_select |= 7 ; // all
	// }		
	
	switch(update_case)
	{
		case 3 :  // Y, Cb update
			if ( table_y == table_cb ) 
			{ 
				update_queue = 1 ; 
				command_queue[0] = update_case ; 
				table_queue[0] = table_y ; 
			}
			else 
			{ 
				update_queue = 2 ; 
				command_queue[1] = ((update_case&0x02)) ;	command_queue[0] = ((update_case&0x01)) ; 
				table_queue[1] = table_cb ; 				table_queue[0] = table_y ;
			}

			break ;
		case 5 : // Y, Cr update
			if ( table_y == table_cr ) 
			{ 
				update_queue = 1 ; 
				command_queue[0] = update_case ; 
				table_queue[0] = table_y ; 
			}
			else 
			{ 
				update_queue = 2 ; 
				command_queue[1] = ((update_case&0x04)) ; 	command_queue[0] = ((update_case&0x01)) ; 
				table_queue[1] = table_cr ; 					table_queue[0] = table_y ;
			}

			break ;			
		case 6 : // Cb, Cr update
			if ( table_cb == table_cr ) 
			{ 
				update_queue = 1 ; 
				command_queue[0] = update_case ; 
				table_queue[0] = table_cb ; 
			}
			else 
			{ 
				update_queue = 2 ; 
				command_queue[1] = ((update_case&0x04)) ; 	command_queue[0] = ((update_case&0x02)) ; 
				table_queue[1] = table_cr ; 				table_queue[0] = table_cb ;
			}

			break ;			
		case 7 : 
			if ( table_y == table_cb ) 
			{	// same 
				if ( table_y == table_cr ) 
				{ 
					update_queue = 1 ; 
					command_queue[0] = update_case ; 
					table_queue[0] = table_y ; 
				}					
				else 
				{ 
					update_queue = 2 ; 
					command_queue[1] = ((update_case&0x04)) ; 	command_queue[0] = ((update_case&0x03)) ; 
					table_queue[1] = table_cr ; 				table_queue[0] = table_y ;
				}
			}
			else if ( table_y == table_cr )
			{
				update_queue = 2 ; 
				command_queue[1] = ((update_case&0x02)) ; 	command_queue[0] = ((update_case&0x06)) ; 
				table_queue[1] = table_cb ; 				table_queue[0] = table_y ;
			}
			else
			{
				if ( table_cb == table_cr ) 
				{ 
					update_queue = 2 ; 
					command_queue[1] = ((update_case&0x06)) ; 	command_queue[0] = ((update_case&0x01)) ; 
					table_queue[1] = table_cb ; 					table_queue[0] = table_y ;
				}
				else 
				{ 
					update_queue = 3 ; 
					command_queue[2] = ((update_case&0x04)) ; 	command_queue[1] = ((update_case&0x02)) ; 	command_queue[0] = ((update_case&0x01)) ; 
					table_queue[2] = table_cr ; 				table_queue[1] = table_cb ; 				table_queue[0] = table_y ;
				}
			}
			break ;
		default : 
			update_queue = 1 ; 
			command_queue[0] = update_case ; 
			if ( update_case == 1 ) table_queue[0] = table_y ;
			else if ( update_case == 2 ) table_queue[0] = table_cb ;
			else table_queue[0] = table_cr ;
			
			break ;
	}
	
	//UARTprintf("***** [update_case :%d] \r\n" , update_case );
	//UARTprintf("***** [update_queue :%d] \r\n" , update_queue );
	//UARTprintf("***** [command_queue[0] :%d] \r\n" , command_queue[0] );
	//UARTprintf("***** [command_queue[1] :%d] \r\n" , command_queue[1] );
	//UARTprintf("***** [command_queue[2] :%d] \r\n" , command_queue[2] );
	
	//UARTprintf("***** [table_queue[0] :%d] \r\n" , table_queue[0] );
	//UARTprintf("***** [table_queue[1] :%d] \r\n" , table_queue[1] );
	//UARTprintf("***** [table_queue[2] :%d] \r\n" , table_queue[2] );
	
	for ( i = 0 ; i < update_queue ; i++ )
	{
		// gamma_table_ptr = (PBYTE)gamma256_value_table[table_queue[i]];
		gamma_table_ptr = (PBYTE)YCgamma_table[table_queue[i]];

		MISP_RegWrite(MISP_LOCAL_ID, __AYCGAMMA_COMMAND_ADDR__, 0x8000|(command_queue[i])<<12) ; // LUT write enable
		
		// UARTprintf("***** [AYCCMD addr %d] :%d ,%d \r\n", __AYCGAMMA_COMMAND_ADDR__,(0x8000|(command_queue[i])<<12),command_queue[i]);
			
		//UARTprintf("***** [WRITING GAMMA LUT] :%d \r\n", i);
		
		for ( j = 0 ; j < 256 ; j++ )
		{			
			MISP_RegWrite(MISP_LOCAL_ID, __AYCGAMMA_DATA_ADDR__, gamma_table_ptr[j]) ; // LUT write	
		}
		
		// UARTprintf("***** [WRITING GAMMA LUT DONE] :%d \r\n", i);
		
		MISP_RegField(MISP_LOCAL_ID, __AYCGAMMA_COMMAND_ADDR__, 12 , 4, 0x0) ; // LUT write disable		
	}	
	
	// if thermo table => disable output interpolation
	// if ( table_y == 17 )	MISP_RegField(MISP_LOCAL_ID, __AYCGAMMA_CTRL_ADDR__, 10, 1, 1) ;	// dis_itp_output
	if ( table_y == 8 )	
	{
		MISP_RegField(MISP_LOCAL_ID, __AYCGAMMA_CTRL_ADDR__, 15, 1, 0) ;	// LUT->CSC
		MISP_RegField(MISP_LOCAL_ID, __AYCGAMMA_CTRL_ADDR__, 10, 1, 1) ;	// dis_itp_output
	}
	else					
	{
		MISP_RegField(MISP_LOCAL_ID, __AYCGAMMA_CTRL_ADDR__, 15, 1, 1) ;	// CSC->LUT
		MISP_RegField(MISP_LOCAL_ID, __AYCGAMMA_CTRL_ADDR__, 10, 1, 0) ;	// en_itp_output
	}
	
	pCTRL->stAYC_GAMMA.channel_select = 0 ; // clear the value	
}


// ----------------------------------------------------------------------
static void TONECTRL_FrameDelayHandler(PTONE_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&TONECTRL_DELAY_UPDATE)==0) return;
	
	if ( pCTRL->active_counter == __TONE_ACTIVE_COUNTER__)
	{
		pCTRL->active_counter = 0 ;
		pCTRL->ctrl |= (TONECTRL_TRIGGER_ISP);
		pCTRL->ctrl &= (~TONECTRL_DELAY_UPDATE);
	}
	else
	{
		pCTRL->active_counter = pCTRL->active_counter + 1 ;
	}	
}
// ----------------------------------------------------------------------
void TONE_ProcessHandler( void )
{
	TONECTRL_FrameDelayHandler(&stTONECTRL) ;
	
	if ((stTONECTRL.ctrl&TONECTRL_TRIGGER_ISP)==0) return;
	stTONECTRL.ctrl &= (~TONECTRL_TRIGGER_ISP);	
		
	TONECTRL_BYIGammaEnHandler(&stTONECTRL) ;
	TONECTRL_BYIGammaHandler(&stTONECTRL) ;
	
	TONECTRL_RGBGammaEnHandler(&stTONECTRL) ;
	TONECTRL_RGBGammaExtendHandler(&stTONECTRL);	
	TONECTRL_RGBGammaHandler(&stTONECTRL) ;
	
	TONECTRL_YCGammaEnHandler(&stTONECTRL) ;	
	TONECTRL_YCGammaExtendHandler(&stTONECTRL) ;		
	TONECTRL_YCGammaHandler(&stTONECTRL) ;

#if __TONE_AYC_HANDLER_ENABLE__	// Temporary block code : Double Fault
	TONECTRL_AYCGammaEnHandler(&stTONECTRL) ;	
	TONECTRL_AYCGammaExtendHandler(&stTONECTRL) ;		
	TONECTRL_AYCGammaHandler(&stTONECTRL) ;
#endif
}

// ----------------------------------------------------------------------
// Get / Set functions
// ----------------------------------------------------------------------

void SetTONE_RGBGAMMA_enable(BOOL val)
{
	
	stTONECTRL.stRGB_GAMMA.gamma_en = val ;
	stTONECTRL.ctrl |= (TONECTRL_GAMMA_EN_UPDATE | TONECTRL_TRIGGER_ISP);

}

// ----------------------------------------------------------------------
void SetTONE_RGBGAMMA_LUT(WORD channel_sel , WORD val)
{
	switch (channel_sel){
		case 0 : stTONECTRL.stRGB_GAMMA.channel_select = 7 ; break ; // all
		case 1 : stTONECTRL.stRGB_GAMMA.channel_select = 1 ; break ; // G
		case 2 : stTONECTRL.stRGB_GAMMA.channel_select = 2 ; break ; // B
		case 3 : stTONECTRL.stRGB_GAMMA.channel_select = 4 ; break ; // R
		default: stTONECTRL.stRGB_GAMMA.channel_select = 7 ; // all
	}

#if !defined(__RGBGAMMA_USE_ADJUST__)
	stTONECTRL.stRGB_GAMMA.gamma_table_number = val ;
#endif
	
	// stTONECTRL.ctrl |= (TONECTRL_GAMMA_LUT_UPDATE | TONECTRL_TRIGGER_ISP);
	stTONECTRL.ctrl |= (TONECTRL_GAMMA_LUT_UPDATE | TONECTRL_DELAY_UPDATE);
}


// ----------------------------------------------------------------------

void  SetTONE_YCGAMMA_enable(BOOL val) 
{
	stTONECTRL.stYC_GAMMA.gamma_en = val  ;
	
	// stTONECTRL.ctrl |= ( TONECTRL_YCGAMMA_EN_UPDATE | TONECTRL_TRIGGER_ISP) ;
	stTONECTRL.ctrl |= ( TONECTRL_YCGAMMA_EN_UPDATE | TONECTRL_DELAY_UPDATE) ;
}

// ----------------------------------------------------------------------

void SetTONE_YCGAMMA_LUT(WORD channel_sel , WORD val)
{
	
	switch (channel_sel){
		case 0 : stTONECTRL.stYC_GAMMA.channel_select |= 7 ; break ; // all
		case 1 : stTONECTRL.stYC_GAMMA.channel_select |= 1 ; break ; // Y
		case 2 : stTONECTRL.stYC_GAMMA.channel_select |= 2 ; break ; // Cb
		case 3 : stTONECTRL.stYC_GAMMA.channel_select |= 4 ; break ; // Cr
		default: stTONECTRL.stYC_GAMMA.channel_select |= 7 ; // all
	}	
	
	switch (channel_sel){
		case 0 : // all
			stTONECTRL.stYC_GAMMA.Y_gamma_table_number = val  ;
			stTONECTRL.stYC_GAMMA.Cb_gamma_table_number = val  ;
			stTONECTRL.stYC_GAMMA.Cr_gamma_table_number = val  ;
			break ;
		case 1 : // Y
			stTONECTRL.stYC_GAMMA.Y_gamma_table_number = val  ;			
			break ;
		case 2 : // Cb
			stTONECTRL.stYC_GAMMA.Cb_gamma_table_number = val  ;
			break ;		
		case 3 : // Cr
			stTONECTRL.stYC_GAMMA.Cr_gamma_table_number = val  ;
			break ;		
		default: 
			stTONECTRL.stYC_GAMMA.Y_gamma_table_number = val  ;
			stTONECTRL.stYC_GAMMA.Cb_gamma_table_number = val  ;
			stTONECTRL.stYC_GAMMA.Cr_gamma_table_number = val  ;
			break ;
	}
	
	// stTONECTRL.ctrl |= ( TONECTRL_YCGAMMA_UPDATE | TONECTRL_TRIGGER_ISP) ;
	stTONECTRL.ctrl |= ( TONECTRL_YCGAMMA_UPDATE | TONECTRL_DELAY_UPDATE) ;
}


// ----------------------------------------------------------------------
void SetTONE_BYIGAMMA_enable(BOOL val )
{
	stTONECTRL.stBYI_GAMMA.gamma_en = val ;
	
	// stTONECTRL.ctrl |= (TONECTRL_BYIGAMMA_EN_UPDATE | TONECTRL_TRIGGER_ISP);
	stTONECTRL.ctrl |= (TONECTRL_BYIGAMMA_EN_UPDATE | TONECTRL_DELAY_UPDATE);
}

// ----------------------------------------------------------------------
void SetTONE_BYIGAMMA_LUT(WORD val)
{
	stTONECTRL.stBYI_GAMMA.gamma_table_number = val ;
	
	// stTONECTRL.ctrl |= (TONECTRL_BYIGAMMA_LUT_UPDATE | TONECTRL_TRIGGER_ISP);	
	stTONECTRL.ctrl |= (TONECTRL_BYIGAMMA_LUT_UPDATE | TONECTRL_DELAY_UPDATE);	
}

// ----------------------------------------------------------------------
BOOL GetTONE_RGBGAMMA_enable(void)
{
	return stTONECTRL.stRGB_GAMMA.gamma_en ;
}
// ----------------------------------------------------------------------
BOOL GetTONE_AYCGAMMA_enable(void)
{
	return stTONECTRL.stAYC_GAMMA.gamma_en ;
}
// ----------------------------------------------------------------------
void  SetTONE_AYCGAMMA_enable(BOOL val) 
{
	stTONECTRL.stAYC_GAMMA.gamma_en = val  ;
	
	// stTONECTRL.ctrl |= ( TONECTRL_AYCGAMMA_EN_UPDATE | TONECTRL_TRIGGER_ISP) ;
	stTONECTRL.ctrl |= ( TONECTRL_AYCGAMMA_EN_UPDATE | TONECTRL_DELAY_UPDATE) ;
}

// ----------------------------------------------------------------------
void SetTONE_AYCGAMMA_LUT(WORD channel_sel , WORD val)
{
	
	switch (channel_sel){
		case 0 : stTONECTRL.stAYC_GAMMA.channel_select |= 7 ; break ; // all
		case 1 : stTONECTRL.stAYC_GAMMA.channel_select |= 1 ; break ; // Y
		case 2 : stTONECTRL.stAYC_GAMMA.channel_select |= 2 ; break ; // Cb
		case 3 : stTONECTRL.stAYC_GAMMA.channel_select |= 4 ; break ; // Cr
		default: stTONECTRL.stAYC_GAMMA.channel_select |= 7 ; // all
	}	
	
	switch (channel_sel){
		case 0 : // all
			stTONECTRL.stAYC_GAMMA.Y_gamma_table_number = val  ;
			stTONECTRL.stAYC_GAMMA.Cb_gamma_table_number = val  ;
			stTONECTRL.stAYC_GAMMA.Cr_gamma_table_number = val  ;
			break ;
		case 1 : // Y
			stTONECTRL.stAYC_GAMMA.Y_gamma_table_number = val  ;			
			break ;
		case 2 : // Cb
			stTONECTRL.stAYC_GAMMA.Cb_gamma_table_number = val  ;
			break ;		
		case 3 : // Cr
			stTONECTRL.stAYC_GAMMA.Cr_gamma_table_number = val  ;
			break ;		
		default: 
			stTONECTRL.stAYC_GAMMA.Y_gamma_table_number = val  ;
			stTONECTRL.stAYC_GAMMA.Cb_gamma_table_number = val  ;
			stTONECTRL.stAYC_GAMMA.Cr_gamma_table_number = val  ;
			break ;
	}
	
	// stTONECTRL.ctrl |= ( TONECTRL_AYCGAMMA_UPDATE | TONECTRL_TRIGGER_ISP) ;
	stTONECTRL.ctrl |= ( TONECTRL_AYCGAMMA_UPDATE | TONECTRL_DELAY_UPDATE) ;
}
// ----------------------------------------------------------------------
BYTE SetTONE_RGBGAMMA_val(BYTE val)
{
#if defined(__RGBGAMMA_USE_ADJUST__)
//---------------------------------------------------------------//
// here is mapping rear lut contrast for i5x0
// add define lens -> sensor  -> pcb & extra option to reduce define depth
// see both mispgammatable510.h & DEMO_SetContrast() at demo.c
// lens define naming at board.h
//---------------------------------------------------------------//
	stTONECTRL.stRGB_GAMMA.contrast = val;

	WORD gamma_table_number;
	#if defined(__USE_MANU_BW3M30B4000__)
		#if defined(__USE_CMOS_IMX124__)
			if(val<9)		gamma_table_number = stTONECTRL.stRGB_GAMMA.contrast+2;		//val-> max(10)
			else if(val==9)	gamma_table_number = stTONECTRL.stRGB_GAMMA.contrast+3;		//val=12
			else 			gamma_table_number = stTONECTRL.stRGB_GAMMA.contrast+4;		//val=14,15...
		#else
			gamma_table_number = stTONECTRL.stRGB_GAMMA.contrast;						//val = 2,3...12,13...22
		#endif
	#else		//default
		gamma_table_number = stTONECTRL.stRGB_GAMMA.contrast;
	#endif

	//----------------------------------------------------------------//
	SetTONE_RGBGAMMA_LUT(0,gamma_table_number);	

	//return stTONECTRL.stRGB_GAMMA.contrast;
	return gamma_table_number;
#else
	BYTE gamma_table_no = 0;
	
	if ( val == 0 )	 		{	SetTONE_RGBGAMMA_LUT(0,0);	gamma_table_no=0;	}
	else if ( val == 1 )	{	SetTONE_RGBGAMMA_LUT(0,0);	gamma_table_no=0;		}	//1.0
	else if ( val == 2 ) 	{	SetTONE_RGBGAMMA_LUT(0,2);	gamma_table_no=2;		}	//0.9
	else if ( val == 3 ) 	{	SetTONE_RGBGAMMA_LUT(0,4);	gamma_table_no=4;		}	//0.8
	else if ( val == 4 ) 	{	SetTONE_RGBGAMMA_LUT(0,6);	gamma_table_no=6;		}	//0.7
	else if ( val == 5 ) 	{	SetTONE_RGBGAMMA_LUT(0,8);	gamma_table_no=8;		}	//0.6
	else if ( val == 6 ) 	{	SetTONE_RGBGAMMA_LUT(0,9);	gamma_table_no=9;		}	//0.55
	else if ( val == 7 ) 	{	SetTONE_RGBGAMMA_LUT(0,10);	gamma_table_no=10;		}	//0.5
	else if ( val == 8 ) 	{	SetTONE_RGBGAMMA_LUT(0,11);	gamma_table_no=11;		}	//0.45
	else if ( val == 9 )	{	SetTONE_RGBGAMMA_LUT(0,12);	gamma_table_no=12;		}	//0.4
	else if ( val == 10 ) 	{	SetTONE_RGBGAMMA_LUT(0,2);	gamma_table_no=2;		}	//0.50(custum)

	return gamma_table_no;
#endif
}

// ----------------------------------------------------------------------
BYTE GetTONE_RGBGAMMA_val(void)
{
#if defined(__RGBGAMMA_USE_ADJUST__)
		// if(stTONECTRL.stRGB_GAMMA.gamma_table_number > 2) 		return  (stTONECTRL.stRGB_GAMMA.gamma_table_number-2);
		// else 													return  0;	
		
		//return stTONECTRL.stRGB_GAMMA.gamma_table_number ;
		return stTONECTRL.stRGB_GAMMA.contrast;
#else
	switch(stTONECTRL.stRGB_GAMMA.gamma_table_number)	{
		case  0: 		return  1; 	// 1.0
		case  2: 		return  2; 	// 0.9
		case  4: 		return  3; 	// 0.8
		case  6: 		return  4; 	// 0.7
		case  8: 		return  5; 	// 0.6
		case  9: 		return  6; 	// 0.55
		case  10: 		return  7; 	// 0.5
		case  11: 		return  8; 	// 0.45
		case  12: 		return  9; 	// 0.4
		default: 		return  0;
	}
#endif
}
//-----------------------------------------------------------------------
void SetTONE_Thermo_RGBGAMMA( BYTE nID )
{
	if ( nID > 0 )
	{
		SetTONE_RGBLUT_Type(MISP_LUT_256x256) ; // Thermo
		
		// Save current setting	
#if defined(__RGBGAMMA_USE_ADJUST__)		
		stTONECTRL.stRGB_GAMMA.prev_gamma_table_number = stTONECTRL.stRGB_GAMMA.contrast;
#else
		stTONECTRL.stRGB_GAMMA.prev_gamma_table_number = stTONECTRL.stRGB_GAMMA.gamma_table_number;
#endif
		// Update to thermo		
		if(nID == 1){
			stTONECTRL.stRGB_GAMMA.thermo_mode = 1;
			
			SetTONE_RGBGAMMA_LUT(1, 8);		//set G
			SetTONE_RGBGAMMA_LUT(2, 9);		//set B
			SetTONE_RGBGAMMA_LUT(3, 7);		//set R
			UARTprintf("***** [Order Thermo LUT ] prev : %d \r\n",stTONECTRL.stRGB_GAMMA.prev_gamma_table_number);			
		}
		else //if(nID == 2)
		{
			stTONECTRL.stRGB_GAMMA.thermo_mode = 2;
			stTONECTRL.ctrl |= (TONECTRL_GAMMA_LUT_UPDATE | TONECTRL_DELAY_UPDATE);
		}

	}
	else
	{	
		SetTONE_RGBLUT_Type(MISP_LUT_64x256) ; // normal
			
		if ( stTONECTRL.stRGB_GAMMA.thermo_mode > 0 )
		{			
			// Restore setting
			SetTONE_RGBGAMMA_LUT(7, stTONECTRL.stRGB_GAMMA.prev_gamma_table_number);		//set Y  G

			UARTprintf("***** [Return to Normal LUT ]****** \r\n");
		}

		stTONECTRL.stRGB_GAMMA.thermo_mode = 0;		
	}
	

}

//-----------------------------------------------------------------------
BYTE GetTONE_RGBGAMMA_thermo(void)
{
	return stTONECTRL.stRGB_GAMMA.thermo_mode ;
}
//-----------------------------------------------------------------------
void SetTONE_Thermo_AYCGAMMA( BOOL OnOff )
{
	if ( OnOff == ON )
	{
		SetTONE_AYCLUT_Type(MISP_LUT_256x256) ; // Thermo
		
		// Save current setting		
		stTONECTRL.stAYC_GAMMA.pY_gamma_table_number = stTONECTRL.stAYC_GAMMA.Y_gamma_table_number ;
		stTONECTRL.stAYC_GAMMA.pCb_gamma_table_number = stTONECTRL.stAYC_GAMMA.Cb_gamma_table_number ;
		stTONECTRL.stAYC_GAMMA.pCr_gamma_table_number = stTONECTRL.stAYC_GAMMA.Cr_gamma_table_number ;		
		
		SetTONE_AYCGAMMA_LUT(1, 8);		//set Y  G
		SetTONE_AYCGAMMA_LUT(2, 9);		//set Cb B
		SetTONE_AYCGAMMA_LUT(3, 7);		//set Cr R	
	}
	else
	{	
		SetTONE_AYCLUT_Type(MISP_LUT_64x256) ; // normal
			
		if ( stTONECTRL.stAYC_GAMMA.thermo_mode == ON )
		{			
			// Restore setting
			SetTONE_AYCGAMMA_LUT(1, stTONECTRL.stAYC_GAMMA.pY_gamma_table_number);		//set Y  G
			SetTONE_AYCGAMMA_LUT(2, stTONECTRL.stAYC_GAMMA.pCb_gamma_table_number);		//set Cb B
			SetTONE_AYCGAMMA_LUT(3, stTONECTRL.stAYC_GAMMA.pCr_gamma_table_number);		//set Cr R	
		}
	}
	
	stTONECTRL.stAYC_GAMMA.thermo_mode = OnOff ;
}

//-----------------------------------------------------------------------
BOOL GetTONE_AYCGAMMA_thermo(void)
{
	return stTONECTRL.stAYC_GAMMA.thermo_mode ;
}

//-----------------------------------------------------------------------
void SetTONE_RGB256LUT_Extend(BOOL OnOff)
{
	stTONECTRL.stRGB_GAMMA.gamma_extend = OnOff;
	// stTONECTRL.ctrl |= (TONECTRL_GAMMA_LUT_UPDATE | TONECTRL_GAMMA_EXTEND_UPDATE | TONECTRL_TRIGGER_ISP);	
	stTONECTRL.ctrl |= (TONECTRL_GAMMA_LUT_UPDATE | TONECTRL_GAMMA_EXTEND_UPDATE | TONECTRL_DELAY_UPDATE);	
}
void SetTONE_YC256LUT_Extend(BOOL OnOff)
{
	stTONECTRL.stYC_GAMMA.gamma_extend = OnOff;
	// stTONECTRL.ctrl |= (TONECTRL_YCGAMMA_EXTEND_UPDATE | TONECTRL_TRIGGER_ISP);	
	stTONECTRL.ctrl |= (TONECTRL_YCGAMMA_EXTEND_UPDATE | TONECTRL_DELAY_UPDATE);	
}
void SetTONE_AYC256LUT_Extend(BOOL OnOff)
{
	stTONECTRL.stAYC_GAMMA.gamma_extend = OnOff;
	// stTONECTRL.ctrl |= (TONECTRL_AYCGAMMA_EXTEND_UPDATE | TONECTRL_TRIGGER_ISP);	
	stTONECTRL.ctrl |= (TONECTRL_AYCGAMMA_EXTEND_UPDATE | TONECTRL_DELAY_UPDATE);	
}
//-----------------------------------------------------------------------
void SetTONE_RGBLUT_Type(BYTE nID)
{
	stTONECTRL.stRGB_GAMMA.gamma_lut=nID;
	MISP_RegField(MISP_LOCAL_ID, __RGBGAMMA_CTRL_ADDR__, 11, 2, stTONECTRL.stRGB_GAMMA.gamma_lut) ;
}
void SetTONE_YCLUT_Type(BYTE nID)
{
	stTONECTRL.stYC_GAMMA.gamma_lut=nID;
	MISP_RegField(MISP_LOCAL_ID, __YCGAMMA_CTRL_ADDR__, 11, 2, stTONECTRL.stYC_GAMMA.gamma_lut) ;
}

void SetTONE_AYCLUT_Type(BYTE nID)
{
	stTONECTRL.stAYC_GAMMA.gamma_lut=nID;
	MISP_RegField(MISP_LOCAL_ID, __AYCGAMMA_CTRL_ADDR__, 11, 2, stTONECTRL.stAYC_GAMMA.gamma_lut) ;	
}

//-----------------------------------------------------------------------
#endif // MDIN-i51X (i510, i540, i550)

