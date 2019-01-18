// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"
#include	"osd.h"
#include	"app_pano.h"

#if defined(__PANORAMA_SENSOR_SETUP__)
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

// // // spi master command set
// // // inquiry
// // #define SPIM_INQ_3A_RAWDATA				0x09070000
// // #define SPIM_INQ_REG_READ					0x09A00000
// // #define SPIM_INQ_USR0						0x09B00000
// // #define SPIM_INQ_USR1						0x09B10000
// // #define SPIM_INQ_USR2						0x09B20000


// // // command
// // #define SPIM_CMD_REG_WRITE				0x01A00000
// // #define SPIM_CMD_REG_ADDR					0x01A10000
// // #define SPIM_CMD_IR_KEY					0x01AF0000
// // #define SPIM_CMD_USR0						0x01B00000
// // #define SPIM_CMD_USR1						0x01B10000
// // #define SPIM_CMD_USR2						0x01B20000

// // #define SPIM_CMD_PANO_DISP				0x01C00000
// // #define SPIM_CMD_PANO_SAVE				0x01C10000
// // #define SPIM_CMD_PANO_DEL 				0x01C20000
// // #define SPIM_CMD_PANO_BLND 				0x01C30000
// // #define SPIM_CMD_PANO_BLVL 				0x01C40000
// // #define SPIM_CMD_PANO_SELSS 				0x01C50000
// // #define SPIM_CMD_PANO_SSADJ 				0x01C60000
// // #define SPIM_CMD_PANO_CNTRL 				0x01C70000
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
// Board number 1 
/*
	__PANORAMA_02__
**Panorama parameter
***Pano Mode : M [3] , P [1]
***BLND Area : L [6] , R [6]
****Main
[Input H:1920], [Input V:1080]
[Source pos X:12], [Source pos Y:106]
[Source width:1850], [Source height:836]
[Window pos X:960], [Window pos Y:0]
[Window width:1920], [Window height:1080]

****Aux
[Input H:1920], [Input V:1080]
[Source pos X:960], [Source pos Y:144]
[Source width:910], [Source height:821]
[Window pos X:0], [Window pos Y:0]
[Window width:960], [Window height:1080]

	stPANO.crop_pos_x_m = 12 ;			
	stPANO.crop_pos_y_m = 106 ;						
	stPANO.crop_width_m = 1850  ;		
	stPANO.crop_height_m = 836 ;

	stPANO.display_pos_x_m = 960 ;  
	stPANO.display_pos_y_m = 0 ;  	
	stPANO.display_width_m = 1920 ;
	stPANO.display_height_m = 1080 ;	

	stPANO.crop_pos_x_a = 960 ;			
	stPANO.crop_pos_y_a = 144 ;				
	stPANO.crop_width_a = 910  ;
	stPANO.crop_height_a = 821 ;

	stPANO.display_pos_x_a = 0 ; 	
	stPANO.display_pos_y_a = 0 ;	
	stPANO.display_width_a = 960 ;	
	stPANO.display_height_a = 1080 ;		
	
	__PANORAMA_03__
**Panorama parameter
***Pano Mode : M [3] , P [1]
***BLND Area : L [0] , R [6]
****Main
[Input H:1920], [Input V:1080]
[Source pos X:45], [Source pos Y:74]
[Source width:915], [Source height:842]
[Window pos X:2880], [Window pos Y:540]
[Window width:960], [Window height:1080]

****Aux
[Input H:3840], [Input V:2160]
[Source pos X:0], [Source pos Y:0]
[Source width:2880], [Source height:1080]
[Window pos X:0], [Window pos Y:540]
[Window width:2880], [Window height:1080]
	
	stPANO.crop_pos_x_m = 45 ;			
	stPANO.crop_pos_y_m = 74 ;						
	stPANO.crop_width_m = 915  ;		
	stPANO.crop_height_m = 842 ;

	stPANO.display_pos_x_m = 2880 ; // 
	stPANO.display_pos_y_m = 540 ; // 	
	stPANO.display_width_m = 960 ;
	stPANO.display_height_m = 1080 ;	

	stPANO.crop_pos_x_a = 0 ;			
	stPANO.crop_pos_y_a = 0 ;				
	stPANO.crop_width_a = 2880  ;
	stPANO.crop_height_a = 1080 ;

	stPANO.display_pos_x_a = 0 ; 	
	stPANO.display_pos_y_a = 540 ;	
	stPANO.display_width_a = 2880 ;	
	stPANO.display_height_a = 1080 ;			
*/
// ----------------------------------------------------------------------
// Board number 2
/*

*/
// ----------------------------------------------------------------------
// Board number 4
/*

*/
// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static MISP_PANO_PARAM stPANO ;
// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------
#if __PANORAMA_5x0__
static void SetApp_Pano_One_Board_Init(void) ;
#else
static void SetApp_Pano_CompMode_SD(void) ;
#endif
static void CtrlApp_Pano_Main(void) ;
static void CtrlApp_Pano_Aux(void) ;
static void SetApp_Pano_Main420en(WORD enable) ;
// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Init_Pano(void)
{
#if __PANORAMA_5x0__
	SetApp_Pano_One_Board_Init() ; // 1280x720x3 scale down

#elif defined(__PANORAMA_01__)|defined(__PANORAMA_02__)|defined(__PANORAMA_03__)
	SetApp_Pano_CompMode_SD() ; // 1280x720x3 scale down
	
	// SetApp_Pano_Main420en(1) ;	// 420 en
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetApp_Pano_Main420en(WORD enable)
{
	MISP_RegField(MISP_LOCAL_ID, 0x05D, 1, 1 , enable) ; // input 420
	MISP_RegField(MISP_LOCAL_ID, 0x213, 13, 1 , enable) ; // output 420
}


#if __PANORAMA_5x0__
static void SetApp_Pano_One_Board_Init(void)
{
	//stPANO.select_sensor = 0 ;
	
	IrCode_PanoCtrlSelectSensor_MDINAPP(2);

	//==================================================//
	// FHD full attach  SetApp_Pano_CompMode_SD
	//==================================================//
	Delay_mSec(100);
	// In out format / PiP On / path select /
	// SetIN_MainSrcExtPort1() ;
	// SetIN_AuxSrcSensor() ;	

	SetIN_MainSrcSensor() ;
		
	SetIN_ExtPort1Mode(MISP_SRC_SEP422_8) ;

#if defined(__USE_25Hz_SENSOR__)	
	SetIN_MainFrmt(MISP_VIDSRC_1920x1080p25);
	// SetIN_AuxFrmt(MISP_VIDSRC_1920x1080p50);  // for fine sync tune 	
#else
	SetIN_MainFrmt(MISP_VIDSRC_1920x1080p30);
#endif		
	
#if defined(__USE_25Hz_OUTPUT__)
	SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p25) ;	
#else
	SetOUT_MainFrmt(GetOUT_MainFrmt()) ;	
#endif	
	
	SetDDRMAP_IPC_frame_count(3) ;
	
	SetIN_MainFRcntY(3) ;	SetIN_MainFRdlyY(1);
	SetIN_MainFRcntC(3) ;	SetIN_MainFRdlyC(1);

	//============================================================//
	SetIn_MainHsPolar(ON) ;
	SetIn_MainVsPolar(ON) ;
	//============================================================//

	stPANO.display_mode = MISP_PANO_DISP_MODE_3 ;
	stPANO.display_position = 1 ;	
	
	//stPANO.blend_left = 0 ;		
	//stPANO.blend_right = 0 ;		
	
	//stPANO.blend_l_lvl = 5 ; // aux 0~15 main
	//stPANO.blend_r_lvl = 10 ; // aux 0~15 main	
		
	stPANO.in_size_h_m = GetIN_MainSrcSizeH() ;
	stPANO.in_size_v_m = GetIN_MainSrcSizeV() ;	

	
	stPANO.crop_pos_x_m = 10 ;			
	stPANO.crop_pos_y_m = 10 ;						
	stPANO.crop_width_m = 1910  ;
	stPANO.crop_height_m = 1070 ;

	stPANO.display_pos_x_m = 0 ;  
	stPANO.display_pos_y_m = 0 ;  	
	stPANO.display_width_m = 1920 ;
	stPANO.display_height_m = 1080 ;	
	
	SetApp_Pano_ReadEEPROM() ;
	
	//============================================================//
	
	CtrlApp_Pano_Main()	;

	//============================================================//
	//SetOUT_PipBlend(stPANO.blend_l_lvl) ; // aux 0~15 main
	
	MISP_SW_Reset();
	//============================================================//

	//==================================================//

	SetBY_Lens_corr_en(ON);		// lens shading correction 'on'

	UARTprintf("\r\n** Panorama init done \r\n");
}

#else
//--------------------------------------------------------------------------------------------------------------------------
static void SetApp_Pano_CompMode_SD(void)
{
	stPANO.select_sensor = 0 ;
	
#if defined(__PANORAMA_02__)
	//==================================================//
	// FHD full attach  SetApp_Pano_CompMode_SD
	//==================================================//
	Delay_mSec(100);
	// In out format / PiP On / path select /
	// SetIN_MainSrcExtPort1() ;
	// SetIN_AuxSrcSensor() ;	

	SetIN_MainSrcSensor() ;
	SetIN_AuxSrcExtPort1() ;		
		
	SetIN_ExtPort1Mode(MISP_SRC_SEP422_8) ;

#if defined(__USE_25Hz_SENSOR__)	
	SetIN_MainFrmt(MISP_VIDSRC_1920x1080p25);
	// SetIN_AuxFrmt(MISP_VIDSRC_1920x1080p50);  // for fine sync tune 	
	SetIN_AuxFrmt(MISP_VIDSRC_1920x1080p60);  // for fine sync tune 	
#else
	SetIN_MainFrmt(MISP_VIDSRC_1920x1080p30);
	SetIN_AuxFrmt(MISP_VIDSRC_1920x1080p60); // for fine sync tune
#endif		

#if defined(__USE_25Hz_OUTPUT__)
	SetOUT_MainFrmt(MISP_VIDOUT_3840x2160p25) ;	
#else
	SetOUT_MainFrmt(MISP_VIDOUT_3840x2160p30) ;	
#endif	
	
	//SetOUT_AuxMode(GetOUT_MainMode());
	//SetOUT_AuxFrmt(GetOUT_MainFrmt());

	// SetDDRMAP_IPC_frame_count(2) ;
	// SetDDRMAP_AUX_frame_count(3) ;
	
	// SetIN_MainFRcntY(2) ;	SetIN_MainFRdlyY(0);
	// SetIN_MainFRcntC(2) ;	SetIN_MainFRdlyC(0);
	// SetIN_AuxFRcnt(3) ;		SetIN_AuxFRdly(1);

	SetDDRMAP_IPC_frame_count(3) ;
	SetDDRMAP_AUX_frame_count(2) ;
	
	SetIN_MainFRcntY(3) ;	SetIN_MainFRdlyY(1);
	SetIN_MainFRcntC(3) ;	SetIN_MainFRdlyC(1);
	SetIN_AuxFRcnt(2) ;		SetIN_AuxFRdly(0);
	
	SetOUT_AuxPipEN(ON);
	SetOUT_AuxDispOnOffAuto(8) ;	

	//============================================================//
	SetIn_MainHsPolar(ON) ;
	SetIn_MainVsPolar(ON) ;
	SetIn_AuxHsPolar(ON) ;
	SetIn_AuxVsPolar(ON) ;
	//============================================================//

	stPANO.display_mode = MISP_PANO_DISP_MODE_3 ;
	stPANO.display_position = 1 ;	
	
	stPANO.blend_left = 0 ;		
	stPANO.blend_right = 0 ;		
	
	stPANO.blend_l_lvl = 5 ; // aux 0~15 main
	stPANO.blend_r_lvl = 10 ; // aux 0~15 main	
		
	stPANO.in_size_h_m = GetIN_MainSrcSizeH() ;
	stPANO.in_size_v_m = GetIN_MainSrcSizeV() ;	

	// stPANO.crop_pos_x_m = 0 ;			
	// stPANO.crop_pos_y_m = 0 ;						
	// stPANO.crop_width_m = 1920  ;		
	// stPANO.crop_height_m = 1080 ;

	// stPANO.display_pos_x_m = 960 ; // 
	// stPANO.display_pos_y_m = 0 ; // 	
	// stPANO.display_width_m = 1920 ;
	// stPANO.display_height_m = 1080 ;	
	
	stPANO.crop_pos_x_m = 12 ;			
	stPANO.crop_pos_y_m = 106 ;						
	stPANO.crop_width_m = 1850  ;		
	stPANO.crop_height_m = 836 ;

	stPANO.display_pos_x_m = 960 ;  
	stPANO.display_pos_y_m = 0 ;  	
	stPANO.display_width_m = 1920 ;
	stPANO.display_height_m = 1080 ;	
	
	stPANO.in_size_h_a = GetIN_AuxSrcSizeH() ;
	stPANO.in_size_v_a = GetIN_AuxSrcSizeV() ;	

	// stPANO.crop_pos_x_a = 960 ;			
	// stPANO.crop_pos_y_a = 0 ;				
	// stPANO.crop_width_a = 960  ;
	// stPANO.crop_height_a = 1080 ;

	// stPANO.display_pos_x_a = 0 ; 	
	// stPANO.display_pos_y_a = 0 ;	
	// stPANO.display_width_a = 960 ;	
	// stPANO.display_height_a = 1080 ;	

	stPANO.crop_pos_x_a = 960 ;			
	stPANO.crop_pos_y_a = 144 ;				
	stPANO.crop_width_a = 910  ;
	stPANO.crop_height_a = 821 ;

	stPANO.display_pos_x_a = 0 ; 	
	stPANO.display_pos_y_a = 0 ;	
	stPANO.display_width_a = 960 ;	
	stPANO.display_height_a = 1080 ;		
	
	SetApp_Pano_ReadEEPROM() ;
	
	//============================================================//
	
	CtrlApp_Pano_Main()	;

	//============================================================//
	
	CtrlApp_Pano_Aux() ;

	//============================================================//
	SetOUT_PipBlend(stPANO.blend_l_lvl) ; // aux 0~15 main
	
	MISP_SW_Reset();
	//============================================================//
#elif defined(__PANORAMA_03__) // SetApp_Pano_CompMode_SD
	//==================================================//
	
	// In out format / PiP On / path select /
	// SetIN_MainSrcExtPort1() ;
	// SetIN_AuxSrcSensor() ;			

	SetIN_MainSrcSensor() ;
	SetIN_AuxSrcExtPort1() ;		

	SetIN_ExtPort1Mode(MISP_SRC_SEP422_8) ;

	
#if defined(__USE_25Hz_SENSOR__)
	// SetIN_MainFrmt(MISP_VIDSRC_3840x2160p25);
	// SetIN_AuxFrmt(MISP_VIDSRC_1920x1080p25);	
	SetIN_MainFrmt(MISP_VIDSRC_1920x1080p25);
	SetIN_AuxFrmt(MISP_VIDSRC_3840x2160p25);		
#else
	// SetIN_MainFrmt(MISP_VIDSRC_3840x2160p30);
	// SetIN_AuxFrmt(MISP_VIDSRC_1920x1080p30);	
	SetIN_MainFrmt(MISP_VIDSRC_1920x1080p30);
	SetIN_AuxFrmt(MISP_VIDSRC_3840x2160p30);		
#endif
	
#if defined(__USE_25Hz_OUTPUT__)
	SetOUT_MainFrmt(MISP_VIDOUT_3840x2160p25) ;	
#else
	SetOUT_MainFrmt(MISP_VIDOUT_3840x2160p30) ;	
#endif
	
	SetOUT_AuxMode(GetOUT_MainMode());
	SetOUT_AuxFrmt(GetOUT_MainFrmt());
	
	// SetDDRMAP_IPC_frame_count(2) ;
	// SetDDRMAP_AUX_frame_count(4) ;
	
	// SetIN_MainFRcntY(2) ;	SetIN_MainFRdlyY(0);
	// SetIN_MainFRcntC(2) ;	SetIN_MainFRdlyC(0);
	// SetIN_AuxFRcnt(4) ;		SetIN_AuxFRdly(2);

	SetDDRMAP_IPC_frame_count(4) ;
	SetDDRMAP_AUX_frame_count(2) ;
	
	SetIN_MainFRcntY(4) ;	SetIN_MainFRdlyY(2);
	SetIN_MainFRcntC(4) ;	SetIN_MainFRdlyC(2);
	SetIN_AuxFRcnt(2) ;		SetIN_AuxFRdly(0);
	
	SetOUT_AuxPipEN(ON);
	SetOUT_AuxDispOnOffAuto(8) ;
	
	//============================================================//
	SetIn_MainHsPolar(ON) ;
	SetIn_MainVsPolar(ON) ;
	SetIn_AuxHsPolar(ON) ;
	SetIn_AuxVsPolar(ON) ;
	//============================================================//

	stPANO.display_mode = MISP_PANO_DISP_MODE_3 ;
	stPANO.display_position = 1 ;	

	stPANO.blend_left = 0 ;		
	stPANO.blend_right = 0 ;		
	
	stPANO.blend_l_lvl = 5 ; // aux 0~15 main	
	stPANO.blend_r_lvl = 10 ; // aux 0~15 main	
	
	stPANO.in_size_h_m = GetIN_MainSrcSizeH() ;
	stPANO.in_size_v_m = GetIN_MainSrcSizeV() ;	
	
	// stPANO.crop_pos_x_m = 0 ;			
	// stPANO.crop_pos_y_m = 0 ;				
	// stPANO.crop_width_m = 960  ;
	// stPANO.crop_height_m = 1080 ;

	// stPANO.display_pos_x_m = 960+1920 ; 	
	// stPANO.display_pos_y_m = 0 ;	
	// stPANO.display_width_m = 960 ; 
	// stPANO.display_height_m = 1080 ;

	stPANO.crop_pos_x_m = 13 ;			
	stPANO.crop_pos_y_m = 48 ;				
	stPANO.crop_width_m = 947  ;
	stPANO.crop_height_m = 882 ;

	stPANO.display_pos_x_m = 2880 ; 	
	stPANO.display_pos_y_m = 540 ;	
	stPANO.display_width_m = 960 ;	
	stPANO.display_height_m = 1080 ;			
	
	stPANO.in_size_h_a = GetIN_AuxSrcSizeH() ;
	stPANO.in_size_v_a = GetIN_AuxSrcSizeV() ;	

	// stPANO.crop_pos_x_a = 0 ;			
	// stPANO.crop_pos_y_a = 0 ;						
	// stPANO.crop_width_a = 960+1920  ;		
	// stPANO.crop_height_a = 1080 ;

	// stPANO.display_pos_x_a = 0 ; //	 
	// stPANO.display_pos_y_a = 0 ; 
	// stPANO.display_width_a = 960+1920 ;
	// stPANO.display_height_a = 1080 ;	

	stPANO.crop_pos_x_a = 0 ;			
	stPANO.crop_pos_y_a = 0 ;						
	stPANO.crop_width_a = 2880  ;		
	stPANO.crop_height_a = 1080 ;

	stPANO.display_pos_x_a = 0 ; // 
	stPANO.display_pos_y_a = 540 ; // 	
	stPANO.display_width_a = 2880 ;
	stPANO.display_height_a = 1080 ;	
	
	SetApp_Pano_ReadEEPROM() ;
	
	//============================================================//
	
	CtrlApp_Pano_Main()	;

	SetCLK_InvYC(0);
	SetCLK_DlyYC(0);
	SetCLK_InvYCm(0);
	SetCLK_DlyYCm(0);	
	
	//============================================================//
	
	CtrlApp_Pano_Aux() ;

	//============================================================//
	SetOUT_PipBlend(stPANO.blend_r_lvl) ; // aux 0~15 main
	
	stWBCTRL.ctrl &= (~WBCTRL_MODE_MASK);
	stWBCTRL.ctrl |= (WBCTRL_MANUAL_MODE|WBCTRL_TRIGGER_ISP);
	//============================================================//	
#elif defined(__PANORAMA_01__) // SetApp_Pano_CompMode_SD		
	//============================================================//
	Delay_mSec(200);		
		
	SetIN_MainSrcSensor() ;
	SetIN_AuxSrcSensor() ;			

	
#if defined(__USE_25Hz_SENSOR__)	
	SetIN_AuxFrmt(MISP_VIDSRC_1920x1080p25);
	SetIN_MainFrmt(MISP_VIDSRC_1920x1080p25);	
#else
	SetIN_AuxFrmt(MISP_VIDSRC_1920x1080p30);
	SetIN_MainFrmt(MISP_VIDSRC_1920x1080p30);	
#endif
	
#if defined(__USE_25Hz_OUTPUT__)
	// SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p50) ;	
	SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p25) ;	
#else
	// SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p60) ;
	SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p30) ;
#endif
	
	SetOUT_MainMode(MISP_OUT_SEP422_8);

	SetDDRMAP_IPC_frame_count(2) ;
	SetDDRMAP_AUX_frame_count(2) ;	
	
	SetIN_MainFRcntY(2) ;	SetIN_MainFRdlyY(0);
	SetIN_MainFRcntC(2) ;	SetIN_MainFRdlyC(0);
	SetIN_AuxFRcnt(2) ;		SetIN_AuxFRdly(0);	
	
	MISP_SW_Reset();
	
	stWBCTRL.ctrl &= (~WBCTRL_MODE_MASK);
	stWBCTRL.ctrl |= (WBCTRL_MANUAL_MODE|WBCTRL_TRIGGER_ISP);
#endif	
	//==================================================//

	SetBY_Lens_corr_en(ON);		// lens shading correction 'on'
		
		
	UARTprintf("\r\n** Panorama init done \r\n");
}
#endif	

//--------------------------------------------------------------------------------------------------------------------------
static void CtrlApp_Pano_Main(void)
{
#if defined(__PANORAMA_02__)
	WORD pos_x_m ;
	WORD width_m ;
	WORD b_h ;
	WORD b_v ;
	WORD crop_x ;
	WORD crop_y ;
	
	pos_x_m = stPANO.display_pos_x_m - stPANO.blend_left ;
	width_m = stPANO.display_width_m + stPANO.blend_left + stPANO.blend_right ;	
		
	// Main view
	// SetFC_MainFCdstPosiX(stPANO.display_pos_x_m);
	SetFC_MainFCdstPosiX(pos_x_m);
	SetFC_MainFCdstPosiY(stPANO.display_pos_y_m);
	// SetFC_MainFCdstSizeH(stPANO.display_width_m);
	SetFC_MainFCdstSizeH(width_m);
	SetFC_MainFCdstSizeV(stPANO.display_height_m);	

	// Init Crop (Main)
	// SetFC_MainFCsrcPosiX(stPANO.crop_pos_x_m);
	// SetFC_MainFCsrcPosiY(stPANO.crop_pos_y_m);
	
	b_h = GetSS_image_boundary_size_h() ;
	b_v = GetSS_image_boundary_size_v() ;	
		
	crop_x = stPANO.crop_pos_x_m + b_h ;
	crop_y = stPANO.crop_pos_y_m + b_v ;
	
	if ( crop_x > 254 )	crop_x = 254 ;
	if ( crop_y > 255 )	crop_y = 255 ;
		
	SetFC_MainFCsrcPosiX(0);
	SetFC_MainFCsrcPosiY(0);	
	
	SetDM_image_size(crop_x,crop_y,stPANO.crop_width_m,stPANO.in_size_v_m) ;	

	SetFC_MainFCsrcSizeH(stPANO.crop_width_m);
	SetFC_MainFCsrcSizeV(stPANO.crop_height_m);	
	
	if ( stPANO.blend_left == 0) 
	{
		SetOUT_PipGradBlendSel(0,0);
	}
	else 
	{
		SetOUT_PipGradBlendSel(0,1);
		SetOUT_PipGradBlendEn(0,1,0,0,(4095/stPANO.blend_left)); // horizontal PiP to Main blend
		SetOUT_PipGradBlendPos(0,pos_x_m,0,(stPANO.display_pos_x_m),2160) ;
	}	
	
#elif defined(__PANORAMA_03__)	
	
	WORD b_h ;
	WORD b_v ;
	WORD crop_x ;
	WORD crop_y ;	
	
	// Main view
	SetFC_MainFCdstPosiX(stPANO.display_pos_x_m);
	SetFC_MainFCdstPosiY(stPANO.display_pos_y_m);
	SetFC_MainFCdstSizeH(stPANO.display_width_m);
	SetFC_MainFCdstSizeV(stPANO.display_height_m);
	
	// Init Crop (Main)
	// SetFC_MainFCsrcPosiX(stPANO.crop_pos_x_m);
	// SetFC_MainFCsrcPosiY(stPANO.crop_pos_y_m);
	
	b_h = GetSS_image_boundary_size_h() ;
	b_v = GetSS_image_boundary_size_v() ;	
		
	crop_x = stPANO.crop_pos_x_m + b_h ;
	crop_y = stPANO.crop_pos_y_m + b_v ;		

	if ( crop_x > 254 )	crop_x = 254 ;
	if ( crop_y > 255 )	crop_y = 255 ;	
	
	SetDM_image_size(crop_x,crop_y,stPANO.crop_width_m,stPANO.in_size_v_m) ;	
	
	SetFC_MainFCsrcSizeH(stPANO.crop_width_m);
	SetFC_MainFCsrcSizeV(stPANO.crop_height_m);	
#endif
}


static void CtrlApp_Pano_Aux(void)
{
#if defined(__PANORAMA_02__)
	// Aux view
	SetFC_AuxFCdstPosiX(stPANO.display_pos_x_a);
	SetFC_AuxFCdstPosiY(stPANO.display_pos_y_a);
	SetFC_AuxFCdstSizeH(stPANO.display_width_a);
	SetFC_AuxFCdstSizeV(stPANO.display_height_a);		

	// Init Crop (Aux)
	SetFC_AuxFCsrcPosiX(stPANO.crop_pos_x_a);
	SetFC_AuxFCsrcPosiY(stPANO.crop_pos_y_a);
	SetFC_AuxFCsrcSizeH(stPANO.crop_width_a);
	SetFC_AuxFCsrcSizeV(stPANO.crop_height_a);		
#elif defined(__PANORAMA_03__)
	WORD dis_width_a ;
	WORD src_width_a ;
	
	dis_width_a = stPANO.display_width_a + stPANO.blend_right ;
	src_width_a = stPANO.crop_width_a + stPANO.blend_right ;		
	
	// Aux view
	SetFC_AuxFCdstPosiX(stPANO.display_pos_x_a);
	SetFC_AuxFCdstPosiY(stPANO.display_pos_y_a);
	// SetFC_AuxFCdstSizeH(stPANO.display_width_a);
	SetFC_AuxFCdstSizeH(dis_width_a);
	SetFC_AuxFCdstSizeV(stPANO.display_height_a);		

	// Init Crop (Aux)
	SetFC_AuxFCsrcPosiX(stPANO.crop_pos_x_a);
	SetFC_AuxFCsrcPosiY(stPANO.crop_pos_y_a);
	// SetFC_AuxFCsrcSizeH(stPANO.crop_width_a);
	SetFC_AuxFCsrcSizeH(src_width_a);
	SetFC_AuxFCsrcSizeV(stPANO.crop_height_a);
	
	if ( stPANO.blend_right == 0) 
	{
		SetOUT_PipGradBlendSel(0,0);
	}
	else 
	{
		SetOUT_PipGradBlendSel(0,1);
		SetOUT_PipGradBlendEn(0,1,0,0,(4095/stPANO.blend_right)); // horizontal PiP to Main blend
		SetOUT_PipGradBlendPos(0,stPANO.display_width_a,0,(dis_width_a),2160) ;
	}	
	
#endif	
}
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Pano_DisplayMode(WORD mode, WORD position)
{
	WORD pre_mode ;
	
	pre_mode = stPANO.display_mode ;

//==================================================//
#if defined(__PANO_SPI_MASTER__) // __PANORAMA_02__
	
	WORD spi_cmd_option ;
	
	spi_cmd_option = MAKEWORD(mode, position);
	SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_PANO_DISP,spi_cmd_option); // Cmd to __PANORAMA_03__
	//SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_IR_KEY, 0x16e9);
#endif	
//==================================================//		
	
#if defined(__PANORAMA_02__)
	//==================================================//	
	if ( mode == MISP_PANO_DISP_MODE_0 ) // 0 : 1280x720x3 full display 2 : 1280x1080x3 full display
	{
		stPANO.crop_pos_x_a = 0 ;
		
		// stPANO.display_pos_x_a = 0 ; // 

		stPANO.display_width_a = 1280 ;
		stPANO.display_height_a = 720 ;	
		
		stPANO.display_pos_x_m = 1280 ; // 
			
		stPANO.display_width_m = 1280 ;
		stPANO.display_height_m = 720 ;		
		// OSD_IrkeyTitleDisplay(ON, "Disp Mode0");
	}
	else if ( mode == MISP_PANO_DISP_MODE_1 ) // 960x1080 + 1920x1080 + 960x1080 H scale down display
	{
		stPANO.crop_pos_x_a = 0 ;
		
		// stPANO.display_pos_x_a = 0 ; // 

		stPANO.display_width_a = 960 ;
		stPANO.display_height_a = 1080 ;	
		
		stPANO.display_pos_x_m = 960 ; // 
			
		stPANO.display_width_m = 1920 ;
		stPANO.display_height_m = 1080 ;
		// OSD_IrkeyTitleDisplay(ON, "Disp Mode1");
	}
	else if ( mode == MISP_PANO_DISP_MODE_2 ) // 1280x1080 + 1280x1080 + 1280x1080 => All sensor H scale down
	{
		stPANO.crop_pos_x_a = 0 ;
		
		// stPANO.display_pos_x_a = 0 ; // 

		stPANO.display_width_a = 1280 ;
		stPANO.display_height_a = 1080 ;	
		
		stPANO.display_pos_x_m = 1280 ; // 
			
		stPANO.display_width_m = 1280 ;
		stPANO.display_height_m = 1080 ;	
		// OSD_IrkeyTitleDisplay(ON, "Disp Mode2");
	}	
	else if ( mode == MISP_PANO_DISP_MODE_3 ) // 960x1080 + 960x1080 + 960x1080 =>  Left, Right sensor H crop
	{
		// stPANO.crop_pos_x_a = 960 ;
		
		// stPANO.display_pos_x_a = 0 ; //
		
		stPANO.display_width_a = 960 ;
		stPANO.display_height_a = 1080 ;	
		
		stPANO.display_pos_x_m = 960 ; // 
			
		stPANO.display_width_m = 1920 ;
		stPANO.display_height_m = 1080 ;
		// OSD_IrkeyTitleDisplay(ON, "Disp Mode3");
	}
	
	if ( (pre_mode != MISP_PANO_DISP_MODE_3) && (mode == MISP_PANO_DISP_MODE_3) )		
	{
		stPANO.crop_pos_x_a = 960 ;	
		stPANO.crop_width_a = stPANO.crop_width_a - 960 ;	
	}
	else if ( pre_mode == MISP_PANO_DISP_MODE_3 && (mode != MISP_PANO_DISP_MODE_3)  )	
	{
		stPANO.crop_pos_x_a = 0 ;		
		stPANO.crop_width_a = stPANO.crop_width_a + 960 ;		
	}
		
	// if ( position == 0 ) // Top position
	// {
		// stPANO.display_pos_y_m = 0 ;
		// stPANO.display_pos_y_a = 0 ; // 	
	// }
	// else // center position
	// {
		// stPANO.display_pos_y_m = 0 ;
		// stPANO.display_pos_y_a = 0 ; // 	
	// }
#elif defined(__PANORAMA_03__)
	//==================================================//
	if ( mode == MISP_PANO_DISP_MODE_0 ) // 0 : 1280x720x3 full display 2 : 1280x1080x3 full display
	{
		// if ( pre_mode == MISP_PANO_DISP_MODE_1 || pre_mode == MISP_PANO_DISP_MODE_3 )	
			// stPANO.crop_width_a = stPANO.crop_width_a - 320 ;
		
		// stPANO.crop_height_a = 720 ;
		
		// stPANO.display_pos_x_a = 0 ; 	

		stPANO.display_width_a = 1280*2 ; // 2560				
		stPANO.display_height_a = 720 ;		
		
		stPANO.display_pos_x_m = 1280*2 ; // 
			
		stPANO.display_width_m = 1280 ;
		stPANO.display_height_m = 720 ;
		OSD_IrkeyTitleDisplay(ON, "Disp Mode0");
	}
	else if ( mode == MISP_PANO_DISP_MODE_1 ) // 960x1080 + 1920x1080 + 960x1080 H scale down display
	{
		// if ( pre_mode == MISP_PANO_DISP_MODE_0 || pre_mode == MISP_PANO_DISP_MODE_2 )	
			// stPANO.crop_width_a = stPANO.crop_width_a + 320 ;
		
		// stPANO.crop_height_a = 1080 ;
		
		// stPANO.display_pos_x_a = 0 ; 	
			
		stPANO.display_width_a = 960 + 1920 ; // 2880
		stPANO.display_height_a = 1080 ;	
		
		stPANO.display_pos_x_m = 960 + 1920 ; // 
			
		stPANO.display_width_m = 960 ;
		stPANO.display_height_m = 1080 ;
		OSD_IrkeyTitleDisplay(ON, "Disp Mode1");
	}
	else if ( mode == MISP_PANO_DISP_MODE_2 ) // 1280x1080 + 1280x1080 + 1280x1080 => All sensor H scale down
	{
		// if ( pre_mode == MISP_PANO_DISP_MODE_1 || pre_mode == MISP_PANO_DISP_MODE_3 )	
			// stPANO.crop_width_a = stPANO.crop_width_a - 320 ;
		
		// stPANO.crop_height_a = 1080 ;
		
		// stPANO.display_pos_x_a = 0 ; 	
			
		stPANO.display_width_a = 1280*2 ; // 2560
		stPANO.display_height_a = 1080 ;	
		
		stPANO.display_pos_x_m = 1280*2 ; // 
			
		stPANO.display_width_m = 1280 ;
		stPANO.display_height_m = 1080 ;
		OSD_IrkeyTitleDisplay(ON, "Disp Mode2");
	}	
	else if ( mode == MISP_PANO_DISP_MODE_3 ) // 960x1080 + 960x1080 + 960x1080 =>  Left, Right sensor H crop
	{
		// if ( pre_mode == MISP_PANO_DISP_MODE_0 || pre_mode == MISP_PANO_DISP_MODE_2 )	
			// stPANO.crop_width_a = stPANO.crop_width_a + 320 ;
		
		// stPANO.crop_height_a = 1080 ;
		
		// stPANO.display_pos_x_a = 0 ; 	
			
		stPANO.display_width_a = 960 + 1920 ; // 2880
		stPANO.display_height_a = 1080 ;	
		
		stPANO.display_pos_x_m = 960 + 1920 ; // 
			
		stPANO.display_width_m = 960 ;
		stPANO.display_height_m = 1080 ;	
		OSD_IrkeyTitleDisplay(ON, "Disp Mode3");
	}
	
	if ( ((pre_mode == MISP_PANO_DISP_MODE_1) || (pre_mode == MISP_PANO_DISP_MODE_3)) &&
			((mode == MISP_PANO_DISP_MODE_0) || (mode == MISP_PANO_DISP_MODE_2)) )	
	{
		stPANO.crop_width_a = stPANO.crop_width_a - 320 ;
	}
	else if ( ((pre_mode == MISP_PANO_DISP_MODE_0) || (pre_mode == MISP_PANO_DISP_MODE_2)) &&
			((mode == MISP_PANO_DISP_MODE_1) || (mode == MISP_PANO_DISP_MODE_3)) )	
	{
		stPANO.crop_width_a = stPANO.crop_width_a + 320 ;
	}
		
	if ( mode == MISP_PANO_DISP_MODE_0 )	stPANO.crop_height_a = 720 ;
	else									stPANO.crop_height_a = 1080 ;
	
	if ( (pre_mode != MISP_PANO_DISP_MODE_3) && (mode == MISP_PANO_DISP_MODE_3) )		stPANO.crop_width_m = stPANO.crop_width_m - 960 ;	
	else if ( pre_mode == MISP_PANO_DISP_MODE_3 && (mode != MISP_PANO_DISP_MODE_3)  )	stPANO.crop_width_m = stPANO.crop_width_m + 960 ;	
		
	
	if ( position == 0 ) // Top position
	{
		stPANO.display_pos_y_m = 0 ;
		stPANO.display_pos_y_a = 0 ; // 
	}
	else // center position
	{
		if ( mode == MISP_PANO_DISP_MODE_0 )
		{
			stPANO.display_pos_y_m = 720 ;
			stPANO.display_pos_y_a = 720 ; //
		}
		else
		{
			stPANO.display_pos_y_m = 540 ;
			stPANO.display_pos_y_a = 540 ; //		
		}
	}
	//==================================================//
#endif
	
	stPANO.display_mode = mode ;
	
	if ( stPANO.display_position != position ) OSD_IrkeyTitleDisplay(OFF, "Disp   Off");
	
	stPANO.display_position = position ;	
		
	CtrlApp_Pano_Main() ;
	CtrlApp_Pano_Aux() ;	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Pano_BlendLeftArea(WORD area)
{
#if defined(__PANORAMA_02__)
	//==================================================//
	// Left area setup
	//==================================================//
	WORD display_bound ;

	display_bound = stPANO.display_width_a ;
	
	if ( display_bound > area )
	{
		stPANO.blend_left = area ;
	}
	else
	{
		stPANO.blend_left = display_bound ;
	}
	
	CtrlApp_Pano_Main() ;
	CtrlApp_Pano_Aux() ;	
	//==================================================//
#elif defined(__PANORAMA_03__)	
	//==================================================//
	// Left area setup
	//==================================================//
	
	// NULL
	stPANO.blend_left = 0 ;
	
	//==================================================//
#endif		
}

void SetApp_Pano_BlendRightArea(WORD area)
{
	
#if defined (__PANO_SPI_MASTER__)		
	SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_PANO_BLND,area); // Cmd to __PANORAMA_03__
#endif		
		
#if defined(__PANORAMA_02__)
	//==================================================//
	// Right area setup
	//==================================================//
	WORD display_end_pos ;
	WORD area_bound ;

	display_end_pos = stPANO.display_width_m + stPANO.display_width_a ;
	area_bound = 3840 - display_end_pos ;	
	
	if ( area_bound > area )
	{
		stPANO.blend_right = area ;
	}
	else
	{
		stPANO.blend_right = area_bound ;
	}
	//==================================================//
#elif defined(__PANORAMA_03__)	
	//==================================================//
	// Right area setup
	//==================================================//
	WORD display_end_pos ;
	WORD area_bound ;
	
	display_end_pos = stPANO.display_width_a ;
	area_bound = 3840 - display_end_pos ;	
	
	if ( area_bound > area )
	{
		stPANO.blend_right = area ;
	}
	else
	{
		stPANO.blend_right = area_bound ;
	}	
	
	//==================================================//
#endif
	
	CtrlApp_Pano_Main() ;
	CtrlApp_Pano_Aux() ;		
}
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Pano_BlendLevel(WORD level)
{
#if defined (__PANO_SPI_MASTER__)		
	SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_PANO_BLVL,level); // Cmd to __PANORAMA_03__
#endif		
	
#if defined(__PANORAMA_02__)
	
	stPANO.blend_l_lvl = level ;
	stPANO.blend_r_lvl = 15-level ;
	
	SetOUT_PipBlend(stPANO.blend_l_lvl);

#elif defined(__PANORAMA_03__)	

	stPANO.blend_l_lvl = level ;
	stPANO.blend_r_lvl = 15-level ;
	
	SetOUT_PipBlend(stPANO.blend_r_lvl);	
	
#endif
}
//--------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Pano_CropLeftX(WORD channel, WORD direction)
{	
	WORD crop_pos_x_m ;
	WORD crop_width_m ;
		
	if ( channel == 1 ) // Main
	{
		crop_pos_x_m = stPANO.crop_pos_x_m ;
		crop_width_m = stPANO.crop_width_m ;
	}
	else // Aux
	{
		crop_pos_x_m = stPANO.crop_pos_x_a ;
		crop_width_m = stPANO.crop_width_a ;	
	}
	
	if ( direction == 1 )
	{	// Increase left crop area 
		crop_pos_x_m = crop_pos_x_m + 2 ;
		crop_width_m = crop_width_m - 2 ;		
	}
	else 
	{	// decrease left crop area 
		if ( crop_pos_x_m > 0 )	 
		{	
			crop_pos_x_m =  crop_pos_x_m - 2 ;
			crop_width_m = crop_width_m + 2 ;
		}
		else					 
		{
			crop_pos_x_m = 0 ;
		}
		
	}
	
	if ( channel == 1 ) // Main
	{
		stPANO.crop_pos_x_m = crop_pos_x_m ;
		stPANO.crop_width_m = crop_width_m ;
		CtrlApp_Pano_Main() ;
	}
	else // Aux
	{
		stPANO.crop_pos_x_a = crop_pos_x_m ;
		stPANO.crop_width_a = crop_width_m ;
		CtrlApp_Pano_Aux() ;
	}		
}

void SetApp_Pano_CropRightX(WORD channel, WORD direction)
{
	WORD crop_width_m ;
	WORD in_size_h_m ;
	
	if ( channel == 1 ) // Main
	{
		crop_width_m = stPANO.crop_width_m ;
		in_size_h_m = stPANO.in_size_h_m - stPANO.crop_pos_x_m ;
	}
	else // Aux
	{
		crop_width_m = stPANO.crop_width_a ;	
		in_size_h_m = stPANO.in_size_h_a - stPANO.crop_pos_x_a ;
	}	
		
	if ( direction == 1 )
	{	// Increase left crop area 
		if ( crop_width_m > 2 )		crop_width_m = crop_width_m - 2 ;		
		else	 					crop_width_m = 2 ;		
	}
	else 
	{	// decrease left crop area 
		if (crop_width_m < in_size_h_m )	crop_width_m = crop_width_m + 2 ;
		else								crop_width_m = in_size_h_m ;
		
	}
		
	if ( channel == 1 ) // Main
	{
		stPANO.crop_width_m = crop_width_m ;
		CtrlApp_Pano_Main() ;
	}
	else // Aux
	{
		stPANO.crop_width_a = crop_width_m ;	
		CtrlApp_Pano_Aux() ;
	}	
}
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Pano_CropTopY(WORD channel, WORD direction)
{
	WORD crop_pos_y_m ;
	WORD crop_height_m ;
	
	if ( channel == 1 ) // Main
	{
		crop_pos_y_m = stPANO.crop_pos_y_m ;
		crop_height_m = stPANO.crop_height_m ;
	}
	else // Aux
	{
		crop_pos_y_m = stPANO.crop_pos_y_a ;	
		crop_height_m = stPANO.crop_height_a ;
	}	
		
	if ( direction == 1 )
	{	// Increase Top crop area 
		crop_pos_y_m = crop_pos_y_m + 1 ;
		crop_height_m = crop_height_m - 1 ;		
	}
	else 
	{	// decrease Top crop area 
		if (crop_pos_y_m >0 )	crop_pos_y_m = crop_pos_y_m - 1 ;
		else					crop_pos_y_m = 0 ;
		crop_height_m = crop_height_m + 1 ;
	}
	
	if ( channel == 1 ) // Main
	{
		stPANO.crop_pos_y_m = crop_pos_y_m ;
		stPANO.crop_height_m = crop_height_m ;	
		CtrlApp_Pano_Main() ;		
	}
	else // Aux
	{
		stPANO.crop_pos_y_a = crop_pos_y_m ;	
		stPANO.crop_height_a = crop_height_m ;
		CtrlApp_Pano_Aux() ;
	}
}

void SetApp_Pano_CropBtmY(WORD channel, WORD direction)
{
	WORD crop_height_m ;
	WORD in_size_v_m ;
	
	if ( channel == 1 ) // Main
	{
		crop_height_m = stPANO.crop_height_m ;
		in_size_v_m = stPANO.in_size_v_m - stPANO.crop_pos_y_m ;
	}
	else // Aux
	{
		crop_height_m = stPANO.crop_height_a ;	
		in_size_v_m = stPANO.in_size_v_a - stPANO.crop_pos_y_a ;
	}		
	
	if ( direction == 1 )
	{	// Increase Btm crop area 
		if (crop_height_m > 0 )	crop_height_m = crop_height_m - 1 ;
		else					crop_height_m = 1 ;	
	}
	else 
	{	// decrease Btm crop area 
		if ( crop_height_m < in_size_v_m )	crop_height_m = crop_height_m + 1 ;		
		else	 							crop_height_m = in_size_v_m ;			
	}
	
	if ( channel == 1 ) // Main
	{
		stPANO.crop_height_m = crop_height_m ;
		CtrlApp_Pano_Main() ;
	}
	else // Aux
	{
		stPANO.crop_height_a = crop_height_m ;	
		CtrlApp_Pano_Aux() ;
	}		
}

//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Pano_WindowMoveX(WORD channel, WORD direction)
{	
	WORD display_pos_x_m ;
	WORD right_bound ;
		
	if ( channel == 1 ) // Main
	{
		display_pos_x_m = stPANO.display_pos_x_m ;
		right_bound = GetOUT_MainSizeHA() - stPANO.display_width_m -1 ;
	}
	else // Aux
	{
		display_pos_x_m = stPANO.display_pos_x_a ;	
		right_bound = GetOUT_MainSizeHA() - stPANO.display_width_a -1 ;
	}	
	
	if ( direction == 1 )
	{	// Increase window x
		if (display_pos_x_m < right_bound )	display_pos_x_m = display_pos_x_m + 2 ;
		else								display_pos_x_m = right_bound + 1 ;	
	}
	else 
	{	// decrease Btm crop area 
		if ( display_pos_x_m > 1 )	display_pos_x_m	= display_pos_x_m - 2 ;		
		else	 					display_pos_x_m	= 0 ;			
	}
	
	
	if ( channel == 1 ) // Main
	{
		stPANO.display_pos_x_m = display_pos_x_m ;
		CtrlApp_Pano_Main() ;	
	}
	else // Aux
	{
		stPANO.display_pos_x_a = display_pos_x_m ;	
		CtrlApp_Pano_Aux() ;
	}		
}

void SetApp_Pano_WindowMoveY(WORD channel, WORD direction)
{
	WORD display_pos_y_m ;
	WORD btm_bound ;
	
	if ( channel == 1 ) // Main
	{
		display_pos_y_m = stPANO.display_pos_y_m ;
		btm_bound = GetOUT_MainSizeVA() - stPANO.display_height_m ;
	}
	else // Aux
	{
		display_pos_y_m = stPANO.display_pos_y_a ;	
		btm_bound = GetOUT_MainSizeVA() - stPANO.display_height_a ;
	}		
	
	if ( direction == 1 )
	{	// Increase window x
		if (display_pos_y_m < btm_bound )	display_pos_y_m = display_pos_y_m + 1 ;
		else								display_pos_y_m = btm_bound ;	
	}
	else 
	{	// decrease Btm crop area 
		if ( display_pos_y_m > 0 )	display_pos_y_m	= display_pos_y_m - 1 ;		
		else	 					display_pos_y_m	= 0 ;			
	}
	
	if ( channel == 1 ) // Main
	{
		stPANO.display_pos_y_m = display_pos_y_m ;
		CtrlApp_Pano_Main() ;
	}
	else // Aux
	{
		stPANO.display_pos_y_a = display_pos_y_m ;	
		CtrlApp_Pano_Aux() ;
	}		
}

//--------------------------------------------------------------------------------------------------------------------------

void SetApp_Pano_WindowStretchX(WORD channel, WORD direction)
{
	WORD window_right_pos ;
	WORD right_bound ;
	WORD display_width_m ;
	WORD display_pos_x_m ;

	right_bound = GetOUT_MainSizeHA() ;
	
	if ( channel == 1 ) // Main
	{			
		window_right_pos = stPANO.display_pos_x_m + stPANO.display_width_m;
		display_width_m = stPANO.display_width_m ;
		display_pos_x_m = stPANO.display_pos_x_m ;
	}
	else // Aux
	{
		window_right_pos = stPANO.display_pos_x_a + stPANO.display_width_a ;	
		display_width_m = stPANO.display_width_a ;
		display_pos_x_m = stPANO.display_pos_x_a ;
	}
	
	if ( direction == 1 )
	{	// Increase stretch x
		if ( window_right_pos < right_bound )	display_width_m = display_width_m + 1 ;
	}
	else 
	{	// decrease stretch x 
		if ( window_right_pos > display_pos_x_m )	display_width_m	= display_width_m - 1 ;		
		else	 									display_width_m	= 1 ;			
	}
	
	if ( channel == 1 ) // Main
	{			
		stPANO.display_width_m = display_width_m ;
		CtrlApp_Pano_Main() ;	
	}
	else // Aux
	{
		stPANO.display_width_a = display_width_m ;
		CtrlApp_Pano_Aux() ;
	}		
}

void SetApp_Pano_WindowStretchY(WORD channel, WORD direction)
{
	WORD display_height_m ;
	WORD display_pos_y_m ;
	WORD window_btm_pos ;
	WORD btm_bound ;
	
	btm_bound = GetOUT_MainSizeVA() ;

	if ( channel == 1 ) // Main
	{
		window_btm_pos = stPANO.display_pos_y_m + stPANO.display_height_m ;
		display_pos_y_m = stPANO.display_pos_y_m ;
		display_height_m = stPANO.display_height_m ;
	}
	else // Aux
	{
		window_btm_pos = stPANO.display_pos_y_a + stPANO.display_height_a ;	
		display_pos_y_m = stPANO.display_pos_y_a ;
		display_height_m = stPANO.display_height_a ;
	}	
	
	if ( direction == 1 )
	{	// Increase window x
		if ( window_btm_pos < btm_bound )	display_height_m = display_height_m + 1 ;
	}
	else 
	{	// decrease Btm crop area 
		if ( window_btm_pos > display_pos_y_m )	display_height_m = display_height_m - 1 ;		
		else	 								display_height_m = 1 ;			
	}
	
	if ( channel == 1 ) // Main
	{
		stPANO.display_height_m = display_height_m ;
		CtrlApp_Pano_Main() ;
	}
	else // Aux
	{
		stPANO.display_height_a = display_height_m ;
		CtrlApp_Pano_Aux() ;
	}		
}
//--------------------------------------------------------------------------------------------------------------------------
void GetApp_Pano_info(void)
{
	UARTprintf("\r\n**Panorama parameter\r\n") ;
	UARTprintf("***Pano Mode : M [%d] , P [%d]\r\n",stPANO.display_mode,stPANO.display_position,stPANO.blend_left) ;
	UARTprintf("***BLND Area : L [%d] , R [%d]\r\n", stPANO.blend_left, stPANO.blend_right) ;
	UARTprintf("***WB offset1: R [%d] , B [%d]\r\n", stPANO.wb_gain_r_off_s1, stPANO.wb_gain_b_off_s1) ;
	UARTprintf("***WB offset2: R [%d] , B [%d]\r\n", stPANO.wb_gain_r_off_s2, stPANO.wb_gain_b_off_s2) ;
	UARTprintf("****Main\r\n") ;
	UARTprintf("[Input H:%d], [Input V:%d]\r\n", stPANO.in_size_h_m ,stPANO.in_size_v_m) ;		
	UARTprintf("[Source pos X:%d], [Source pos Y:%d]\r\n", stPANO.crop_pos_x_m ,stPANO.crop_pos_y_m) ;	
	UARTprintf("[Source width:%d], [Source height:%d]\r\n", stPANO.crop_width_m ,stPANO.crop_height_m) ;	
	
	UARTprintf("[Window pos X:%d], [Window pos Y:%d]\r\n", stPANO.display_pos_x_m ,stPANO.display_pos_y_m) ;	
	UARTprintf("[Window width:%d], [Window height:%d]\r\n\r\n", stPANO.display_width_m ,stPANO.display_height_m) ;		
	
	UARTprintf("****Aux\r\n") ;
	UARTprintf("[Input H:%d], [Input V:%d]\r\n", stPANO.in_size_h_a ,stPANO.in_size_v_a) ;		
	UARTprintf("[Source pos X:%d], [Source pos Y:%d]\r\n", stPANO.crop_pos_x_a ,stPANO.crop_pos_y_a) ;	
	UARTprintf("[Source width:%d], [Source height:%d]\r\n", stPANO.crop_width_a ,stPANO.crop_height_a) ;	
	
	UARTprintf("[Window pos X:%d], [Window pos Y:%d]\r\n", stPANO.display_pos_x_a ,stPANO.display_pos_y_a) ;	
	UARTprintf("[Window width:%d], [Window height:%d]\r\n\r\n", stPANO.display_width_a ,stPANO.display_height_a) ;		
}



//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
WORD SetApp_Pano_SS01CtrlMode(WORD mode)
{
#if defined (__PANORAMA_02__) || defined (__PANORAMA_03__) 	
	if (mode > 4 ) 
	{
		if ( stPANO.sensor_mode_01 < 4 ) stPANO.sensor_mode_01 = stPANO.sensor_mode_01 + 1 ;
		else stPANO.sensor_mode_01 = 0 ;
	}
	else
	{
		stPANO.sensor_mode_01 = mode ;
	}	
	
	stPANO.sensor_mode_02 = 0 ;
	stPANO.sensor_mode_03 = 0 ;
	
#if defined (__PANO_SPI_MASTER__)
	SetApp_Pano_AuxCtrlMode(stPANO.sensor_mode_01) ;
#endif
	
	return stPANO.sensor_mode_01 ;
	
#else	// defined (__PANORAMA_01__) 	
	return mode ;
#endif	
}
//--------------------------------------------------------------------------------------------------------------------------
WORD SetApp_Pano_SS02CtrlMode(WORD mode)
{
#if defined (__PANORAMA_02__) || defined (__PANORAMA_03__) 	
	if (mode > 4 ) 
	{
		if ( stPANO.sensor_mode_02 < 4 ) stPANO.sensor_mode_02 = stPANO.sensor_mode_02 + 1 ;
		else stPANO.sensor_mode_02 = 0 ;
	}
	else
	{
		stPANO.sensor_mode_02 = mode ;
	}	

	stPANO.sensor_mode_01 = 0 ;
	stPANO.sensor_mode_03 = 0 ;	
	
#if defined (__PANO_SPI_MASTER__)	
	SetApp_Pano_MainCtrlMode(stPANO.sensor_mode_02) ;
#endif	
	
	return stPANO.sensor_mode_02 ;
	
#else	// defined (__PANORAMA_01__) 	
	return mode ;
#endif
	
}

//--------------------------------------------------------------------------------------------------------------------------
WORD SetApp_Pano_SS03CtrlMode(WORD mode)
{
#if defined (__PANORAMA_02__) || defined (__PANORAMA_03__) 	
	if (mode > 4 ) 
	{
		if ( stPANO.sensor_mode_03 < 4 ) stPANO.sensor_mode_03 = stPANO.sensor_mode_03 + 1 ;
		else stPANO.sensor_mode_03 = 0 ;
	}
	else
	{
		stPANO.sensor_mode_03 = mode ;
	}	
	
	stPANO.sensor_mode_01 = 0 ;
	stPANO.sensor_mode_02 = 0 ;	
	
#if !defined (__PANO_SPI_MASTER__)	
	SetApp_Pano_MainCtrlMode(stPANO.sensor_mode_03) ;
#endif	
	
	return stPANO.sensor_mode_03 ;
	
#else	// defined (__PANORAMA_01__) 	
	return mode ;
#endif	
}
//--------------------------------------------------------------------------------------------------------------------------

WORD SetApp_Pano_MainCtrlMode(WORD mode)
{
#if defined (__PANORAMA_02__) 
	if (mode > 4 ) 
	{
		if ( stPANO.ctrl_m < 4 ) stPANO.ctrl_m = stPANO.ctrl_m + 1 ;
		else stPANO.ctrl_m = 0 ;
	}
	else
	{
		stPANO.ctrl_m = mode ;
	}
#elif defined (__PANORAMA_03__)	
	if (mode > 4 ) 
	{
		if ( stPANO.ctrl_m < 4 ) stPANO.ctrl_m = stPANO.ctrl_m + 1 ;
		else stPANO.ctrl_m = 0 ;
	}
	else
	{
		stPANO.ctrl_m = mode ;
	}		
#endif
	stPANO.ctrl_a = 0 ; // one by one control
	
	return stPANO.ctrl_m ;
}

WORD SetApp_Pano_AuxCtrlMode(WORD mode)
{
#if defined (__PANORAMA_02__) 
	if (mode > 4 ) 
	{	
		if ( stPANO.ctrl_a < 4 ) stPANO.ctrl_a = stPANO.ctrl_a + 1 ;
		else stPANO.ctrl_a = 0 ;
	}
	else
	{
		stPANO.ctrl_a = mode ;
	}	
#elif defined (__PANORAMA_03__)
	if (mode > 4 ) 
	{	
		if ( stPANO.ctrl_a < 4 ) stPANO.ctrl_a = stPANO.ctrl_a + 1 ;
		else stPANO.ctrl_a = 0 ;
	}
	else
	{
		stPANO.ctrl_a = mode ;
	}	
#endif	
	stPANO.ctrl_m = 0 ; // one by one control
		
	return stPANO.ctrl_a ;
}
//--------------------------------------------------------------------------------------------------------------------------

void SetApp_Pano_Control( WORD direction )
{
#if defined (__PANORAMA_02__) || defined (__PANORAMA_03__) 	
	
	WORD control ;
	WORD channel ;
	WORD mode ;
	WORD blend_area ;
	WORD blend_lvl ;
	WORD position ;

#if defined (__PANO_SPI_MASTER__)		
	
	if ( stPANO.select_sensor == 1 || stPANO.ctrl_a > 0 )
	{
		channel = 0 ;					// aux
		control = stPANO.ctrl_a ;	
	}
	else if ( stPANO.select_sensor == 2  || stPANO.ctrl_m > 0 )
	{
		channel = 1 ;					// main
		control = stPANO.ctrl_m ;	
	}
	else if ( stPANO.select_sensor == 3 )
	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_PANO_CNTRL,direction); 
		channel = 0 ;
		control = 0 ;		
	}
	else
	{
		channel = 0 ;
		control = 0 ;		
	}

#else

	if ( stPANO.select_sensor == 3 || stPANO.ctrl_m > 0 )
	{
		channel = 1 ;					// main
		control = stPANO.ctrl_m ;		
	}	
	else if ( stPANO.ctrl_a > 0 )
	{
		channel = 0 ;					// aux
		control = stPANO.ctrl_a ;		
	}
	else
	{
		channel = 0 ;
		control = 0 ;		
	}		
	
#endif
		
	mode = stPANO.display_mode ;
	position = stPANO.display_position ;
	blend_area = stPANO.blend_left ;
	blend_lvl = stPANO.blend_l_lvl ;
		
	if ( stPANO.select_sensor == 0 ) 
	{		
		if ( direction == 1 ) 
		{
			control = 5 ;
			if ( mode > MISP_PANO_DISP_END-2 )	mode = 0 ;
			else								mode = mode + 1 ;
		}
		else 
		{
			control = 5 ;
			if ( position == 1 ) 	position = 0 ;
			else					position = 1 ;
		}
	}
	
	if ( stPANO.select_sensor == 4 ) 
	{
		
		if ( direction == 1 ) 
		{
			control = 6 ;
			
			// if ( blend_area > 15 )	blend_area = 0 ;
			if ( blend_area > 23 )	blend_area = 0 ;
			else 					blend_area = blend_area + 8 ; // must be even
		}
		else 
		{
			control = 7 ;
			
			if ( blend_lvl > 14 )	blend_lvl = 0 ;
			else					blend_lvl = blend_lvl + 5 ;
		}
	}
	
	switch ( control )
	{
		case 1 : SetApp_Pano_CropLeftX(channel,direction) ; break ; // x left crop
		case 2 : SetApp_Pano_CropRightX(channel,direction) ; break ; // x right crop
		case 3 : SetApp_Pano_CropTopY(channel,direction) ; break ; // y top crop
		case 4 : SetApp_Pano_CropBtmY(channel,direction) ; break ; // y btm crop
		case 5 : SetApp_Pano_DisplayMode(mode,position) ; break ; // 
		case 6 : SetApp_Pano_BlendLeftArea(blend_area) ; SetApp_Pano_BlendRightArea(blend_area) ; break ; // 
		// case 6 : SPImAPP_CmdStd(SPIM_MISP_CID_S1, SPIM_CMD_IR_KEY, 0x16e9) ; break ; // for S1 SPIs test
		case 7 : SetApp_Pano_BlendLevel(blend_lvl) ; break ; // 	
		// case 8 : break ; // 	
		default : break ;	// Do nothing
	}
	
#endif // #if defined (__PANORAMA_02__) || defined (__PANORAMA_03__) 	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetApp_PanoSelectSensor(WORD sensor_num)
{
	stPANO.select_sensor = sensor_num ;	
	
	stPANO.ctrl_m = 0 ; // Reset internal ctrl mode
	stPANO.ctrl_a = 0 ;
}

WORD GetApp_PanoSelectSensor(void)
{
	return stPANO.select_sensor ;
}
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
void IrCode_PanoCtrlSelectSensor(void)
{
#if defined (__PANORAMA_02__) || defined (__PANORAMA_03__)	

	WORD sensor_num ;
	
	sensor_num = GetApp_PanoSelectSensor() ;
	
	if ( sensor_num >= 4 ) 	sensor_num = 0 ;
	else 					sensor_num = sensor_num + 1 ;	
	
	SetApp_PanoSelectSensor(sensor_num) ;
	
#if defined (__PANO_SPI_MASTER__) // Cmd to __PANORAMA_03__	
	SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_PANO_SELSS,0); 
#else // Cmd Exe __PANORAMA_03__
	switch ( sensor_num )
	{
		case 1 : OSD_IrkeyTitleDisplay(ON, "Select SS1"); break ;
		case 2 : OSD_IrkeyTitleDisplay(ON, "Select SS2"); break ;
		case 3 : OSD_IrkeyTitleDisplay(ON, "Select SS3"); break ;	
		case 4 : OSD_IrkeyTitleDisplay(ON, "Blend Mode"); break ;	
		default : OSD_IrkeyTitleDisplay(OFF, "Select SS0"); break ;
	}
#endif	
	
#endif
}

void IrCode_PanoCtrlSelectAdjust(void)
{
#if defined (__PANORAMA_02__) || defined (__PANORAMA_03__)	
	
	WORD sensor_num ;
	WORD mode ;
	
	sensor_num = GetApp_PanoSelectSensor() ;	
	
	if ( sensor_num == 1 ) // Aux input of S3 i550 => SS1
	{
		mode = SetApp_Pano_SS01CtrlMode(9);
	}	
	else if ( sensor_num == 2 ) // Main input of S3 i550 => SS2
	{
		mode = SetApp_Pano_SS02CtrlMode(9);
	}	
	else if ( sensor_num == 3 ) 
	{
		mode = SetApp_Pano_SS03CtrlMode(9);
	}
	else
	{
		mode = 0 ;
	}

#if defined (__PANO_SPI_MASTER__) // Cmd to __PANORAMA_03__		
	
	SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_PANO_SSADJ,mode); 

#else // Cmd exe __PANORAMA_03__ [OSD display]
	if ( sensor_num == 1 ) // Aux input of S3 i550 => SS1
	{
		switch (mode)
		{
			case 1 : OSD_IrkeyTitleDisplay(ON, "SS1 CrpLX "); break ;
			case 2 : OSD_IrkeyTitleDisplay(ON, "SS1 CrpRX "); break ;
			case 3 : OSD_IrkeyTitleDisplay(ON, "SS1 CrpTY "); break ;
			case 4 : OSD_IrkeyTitleDisplay(ON, "SS1 CrpBY "); break ;
			default : OSD_IrkeyTitleDisplay(OFF, "Select SS1"); break ;
		}		
	}	
	else if ( sensor_num == 2 ) // Main input of S3 i550 => SS2
	{
		switch (mode)
		{
			case 1 : OSD_IrkeyTitleDisplay(ON, "SS2 CrpLX "); break ;
			case 2 : OSD_IrkeyTitleDisplay(ON, "SS2 CrpRX "); break ;
			case 3 : OSD_IrkeyTitleDisplay(ON, "SS2 CrpTY "); break ;
			case 4 : OSD_IrkeyTitleDisplay(ON, "SS2 CrpBY "); break ;
			default : OSD_IrkeyTitleDisplay(OFF, "Select SS2"); break ;
		}	
	}		
	else if ( sensor_num == 3 ) // Main input of S2 i550 => SS3
	{
		switch (mode)
		{
			case 1 : OSD_IrkeyTitleDisplay(ON, "SS3 CrpLX "); break ;
			case 2 : OSD_IrkeyTitleDisplay(ON, "SS3 CrpRX "); break ;
			case 3 : OSD_IrkeyTitleDisplay(ON, "SS3 CrpTY "); break ;
			case 4 : OSD_IrkeyTitleDisplay(ON, "SS3 CrpBY "); break ;
			default : OSD_IrkeyTitleDisplay(OFF, "Select SS3"); break ;
		}	
	}	
#endif // #if defined (__PANO_SPI_MASTER__) // Cmd to __PANORAMA_03__		
	
#endif		
}

//--------------------------------------------------------------------------------------------------------------------------

void IrCode_PanoCtrlMain(void)
{
	WORD mode ;
	mode = SetApp_Pano_MainCtrlMode(9);

	switch (mode)
	{
		case 1 : OSD_IrkeyTitleDisplay(ON, "Main CrpLX"); break ;
		case 2 : OSD_IrkeyTitleDisplay(ON, "Main CrpRX"); break ;
		case 3 : OSD_IrkeyTitleDisplay(ON, "Main CrpTY"); break ;
		case 4 : OSD_IrkeyTitleDisplay(ON, "Main CrpBY"); break ;
		case 5 : OSD_IrkeyTitleDisplay(ON, "Disp Mode "); break ;
		case 6 : OSD_IrkeyTitleDisplay(ON, "Disp Pos  "); break ;
		case 7 : OSD_IrkeyTitleDisplay(ON, "Blend Mode"); break ;
		case 8 : OSD_IrkeyTitleDisplay(ON, "NaN       "); break ;
		default : OSD_IrkeyTitleDisplay(OFF, "MainctrlOF"); break ;
	}	
}

void IrCode_PanoCtrlAux(void)
{
	WORD mode ;
	mode = SetApp_Pano_AuxCtrlMode(9);
	
	switch (mode)
	{
		case 1 : OSD_IrkeyTitleDisplay(ON, "Aux CrpLX "); break ;
		case 2 : OSD_IrkeyTitleDisplay(ON, "Aux CrpRX "); break ;
		case 3 : OSD_IrkeyTitleDisplay(ON, "Aux CrpTY "); break ;
		case 4 : OSD_IrkeyTitleDisplay(ON, "Aux CrpBY "); break ;
		case 5 : OSD_IrkeyTitleDisplay(ON, "Disp Mode "); break ;
		case 6 : OSD_IrkeyTitleDisplay(ON, "Disp Pos  "); break ;
		case 7 : OSD_IrkeyTitleDisplay(ON, "Aux CrpTY "); break ;
		case 8 : OSD_IrkeyTitleDisplay(ON, "Aux CrpBY "); break ;
		default : OSD_IrkeyTitleDisplay(OFF, "Aux ctrlOF"); break ;
	}	
}

// MDIN APP
void IrCode_PanoCtrlSelectSensor_MDINAPP(WORD sensor_num)
{
#if defined (__PANORAMA_02__) || defined (__PANORAMA_03__)	
	
	SetApp_PanoSelectSensor(sensor_num) ;
	
#if defined (__PANO_SPI_MASTER__) // Cmd to __PANORAMA_03__	
	SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_PANO_SS0 | sensor_num,0); 
#else // Cmd Exe __PANORAMA_03__
	switch ( sensor_num )
	{
		case 1 : OSD_IrkeyTitleDisplay(ON, "Select SS1"); break ;
		case 2 : OSD_IrkeyTitleDisplay(ON, "Select SS2"); break ;
		case 3 : OSD_IrkeyTitleDisplay(ON, "Select SS3"); break ;	
		case 4 : OSD_IrkeyTitleDisplay(ON, "Blend Mode"); break ;	
		default : OSD_IrkeyTitleDisplay(OFF, "Select SS0"); break ;
	}
#endif	
	
#endif
}


void IrCode_Pano_Control_MDINAPP(WORD val){
#if defined (__PANORAMA_02__) || defined (__PANORAMA_03__) 	
	WORD ctrl ;
	WORD direction ;
	WORD control ;
	WORD channel ;

	WORD mode ;
	WORD blend_area ;
	WORD blend_lvl ;
	WORD position ;

	switch ( val )
	{
		case 0 : ctrl = 1 ;	direction = 0 ;	break ; // x left crop
		case 1 : ctrl = 1 ;	direction = 1 ;	break ; // x left crop
		case 2 : ctrl = 2 ;	direction = 0 ;	break ; // x right crop
		case 3 : ctrl = 2 ;	direction = 1 ;	break ; // x right crop
		case 4 : ctrl = 3 ;	direction = 0 ;	break ; // y top crop
		case 5 : ctrl = 3 ;	direction = 1 ;	break ; // y top crop
		case 6 : ctrl = 4 ;	direction = 0 ;	break ; // y btm crop
		case 7 : ctrl = 4 ;	direction = 1 ;	break ; // y btm crop
		case 8 : ctrl = 5 ;	direction = 0 ;   break ; // display mode : position change only
		case 9 : ctrl = 6 ;	direction = 1 ;   break ; // blend area
		case 10 : ctrl = 7 ;	direction = 0 ;	break ; // blend level
		default : ctrl =0 ; direction = 0 ; break ;	// Do nothing
	}

#if defined (__PANO_SPI_MASTER__)		
	
	if ( stPANO.select_sensor == 1 )
	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PANO_CRP_LX_U, val + 32); 
		channel = 0 ;					// aux
		control = ctrl ;	
	}
	else if ( stPANO.select_sensor == 2 )
	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PANO_CRP_LX_U, val + 16); 
		channel = 1 ;					// main
		control = ctrl ;	
	}
	else if ( stPANO.select_sensor == 3 )
	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PANO_CRP_LX_U, val); 
		channel = 0 ;
		control = 0 ;
	}
	else
	{
		channel = 0 ;
		control = 0 ;
	}

#else

	if ( stPANO.select_sensor == 3 )
	{
		channel = 1 ;					// main
		control = ctrl ;
	}	
	else 
	{
		channel = 0 ;					// aux
		control = ctrl ;
	}

	switch ( val )
	{
		case 32 : OSD_IrkeyTitleDisplay(ON, "SS1 CrpLX"); return;
		case 33 : OSD_IrkeyTitleDisplay(ON, "SS1 CrpLX"); return; 
		case 34 : OSD_IrkeyTitleDisplay(ON, "SS1 CrpRX"); return; 
		case 35 : OSD_IrkeyTitleDisplay(ON, "SS1 CrpRX"); return; 
		case 36 : OSD_IrkeyTitleDisplay(ON, "SS1 CrpTY"); return; 
		case 37 : OSD_IrkeyTitleDisplay(ON, "SS1 CrpTY"); return; 
		case 38 : OSD_IrkeyTitleDisplay(ON, "SS1 CrpTY"); return; 
		case 39 : OSD_IrkeyTitleDisplay(ON, "SS1 CrpBY"); return; 
		case 40 : OSD_IrkeyTitleDisplay(ON, "SS1 CrpBY"); return; 

		case 16 : OSD_IrkeyTitleDisplay(ON, "SS2 CrpLX"); return; 
		case 17 : OSD_IrkeyTitleDisplay(ON, "SS2 CrpLX"); return; 
		case 18 : OSD_IrkeyTitleDisplay(ON, "SS2 CrpRX"); return; 
		case 19 : OSD_IrkeyTitleDisplay(ON, "SS2 CrpRX"); return;
		case 20 : OSD_IrkeyTitleDisplay(ON, "SS2 CrpTY"); return; 
		case 21 : OSD_IrkeyTitleDisplay(ON, "SS2 CrpTY"); return; 
		case 22 : OSD_IrkeyTitleDisplay(ON, "SS2 CrpTY"); return; 
		case 23 : OSD_IrkeyTitleDisplay(ON, "SS2 CrpBY"); return; 
		case 24 : OSD_IrkeyTitleDisplay(ON, "SS2 CrpBY"); return; 

		case 0 : OSD_IrkeyTitleDisplay(ON, "SS3 CrpLX"); break;
		case 1 : OSD_IrkeyTitleDisplay(ON, "SS3 CrpLX"); break;
		case 2 : OSD_IrkeyTitleDisplay(ON, "SS3 CrpRX"); break;
		case 3 : OSD_IrkeyTitleDisplay(ON, "SS3 CrpRX"); break;
		case 4 : OSD_IrkeyTitleDisplay(ON, "SS3 CrpTY"); break;
		case 5 : OSD_IrkeyTitleDisplay(ON, "SS3 CrpTY"); break;
		case 6 : OSD_IrkeyTitleDisplay(ON, "SS3 CrpTY"); break;
		case 7 : OSD_IrkeyTitleDisplay(ON, "SS3 CrpBY"); break;
		case 8 : OSD_IrkeyTitleDisplay(ON, "SS3 CrpBY"); break;
		default : OSD_IrkeyTitleDisplay(OFF, "Select SS0"); 
	}

#endif

	mode = stPANO.display_mode ;
	position = stPANO.display_position ;
	blend_area = stPANO.blend_left ;
	blend_lvl = stPANO.blend_l_lvl ;
		
	if ( stPANO.select_sensor == 0 && ctrl == 5) 
	{		
		if ( direction == 1 ) 
		{
			if ( mode > MISP_PANO_DISP_END-2 )	mode = 0 ;
			else								mode = mode + 1 ;
		}
		else 
		{
			if ( position == 1 ) 	position = 0 ;
			else					position = 1 ;
		}
		SetApp_Pano_DisplayMode(mode,position) ;  
	}

	if ( stPANO.select_sensor == 4  && ctrl > 5) 
	{
		
		if ( direction == 1 ) 
		{

			// if ( blend_area > 15 )	blend_area = 0 ;
			if ( blend_area > 23 )	blend_area = 0 ;
			else 					blend_area = blend_area + 8 ; // must be even

			SetApp_Pano_BlendLeftArea(blend_area) ; SetApp_Pano_BlendRightArea(blend_area) ;  // 
		}
		else 
		{

			if ( blend_lvl > 14 )	blend_lvl = 0 ;
			else					blend_lvl = blend_lvl + 5 ;

			SetApp_Pano_BlendLevel(blend_lvl) ;  // 	
		}
	}

	switch ( control )
	{
		case 1 : SetApp_Pano_CropLeftX(channel,direction) ; 	break ; // x left crop
		case 2 : SetApp_Pano_CropRightX(channel,direction) ;	break ; // x right crop
		case 3 : SetApp_Pano_CropTopY(channel,direction) ; 		break ; // y top crop
		case 4 : SetApp_Pano_CropBtmY(channel,direction) ; 		break ; // y btm crop
		default : break ;	// Do nothing
	}
#endif

}


//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Pano_WriteEEPROM(void)
{
#if defined (__PANORAMA_02__) || defined (__PANORAMA_03__)	
	
//#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
    BYTE buff[21];
	BYTE buff_b[9];
	 
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = LOBYTE(stPANO.crop_pos_x_m);       buff[2] = HIBYTE(stPANO.crop_pos_x_m);
	buff[3] = LOBYTE(stPANO.crop_pos_y_m);       buff[4] = HIBYTE(stPANO.crop_pos_y_m);
	buff[5] = LOBYTE(stPANO.crop_width_m);       buff[6] = HIBYTE(stPANO.crop_width_m);
	buff[7] = LOBYTE(stPANO.crop_height_m);      buff[8] = HIBYTE(stPANO.crop_height_m);
	buff[9] = LOBYTE(stPANO.display_pos_x_m);    buff[10] = HIBYTE(stPANO.display_pos_x_m);
	buff[11] = LOBYTE(stPANO.display_pos_y_m);   buff[12] = HIBYTE(stPANO.display_pos_y_m);
	buff[13] = LOBYTE(stPANO.display_width_m);   buff[14] = HIBYTE(stPANO.display_width_m);
	buff[15] = LOBYTE(stPANO.display_height_m);  buff[16] = HIBYTE(stPANO.display_height_m);		
	buff[17] = LOBYTE(stPANO.display_mode); 	 buff[18] = HIBYTE(stPANO.display_mode);		
	buff[19] = LOBYTE(stPANO.display_position);  buff[20] = HIBYTE(stPANO.display_position);	
	
	EEPROM_MultiWrite(EEPROM_PANO_MAIN, (PBYTE)&buff[0], sizeof(buff));
	
	// buff[0] = EEPROM_SAVE_DONE;	
	buff[1] = LOBYTE(stPANO.crop_pos_x_a);       buff[2] = HIBYTE(stPANO.crop_pos_x_a);
	buff[3] = LOBYTE(stPANO.crop_pos_y_a);       buff[4] = HIBYTE(stPANO.crop_pos_y_a);
	buff[5] = LOBYTE(stPANO.crop_width_a);       buff[6] = HIBYTE(stPANO.crop_width_a);
	buff[7] = LOBYTE(stPANO.crop_height_a);      buff[8] = HIBYTE(stPANO.crop_height_a);
	buff[9] = LOBYTE(stPANO.display_pos_x_a);    buff[10] = HIBYTE(stPANO.display_pos_x_a);
	buff[11] = LOBYTE(stPANO.display_pos_y_a);   buff[12] = HIBYTE(stPANO.display_pos_y_a);
	buff[13] = LOBYTE(stPANO.display_width_a);   buff[14] = HIBYTE(stPANO.display_width_a);
	buff[15] = LOBYTE(stPANO.display_height_a);  buff[16] = HIBYTE(stPANO.display_height_a);	
	buff[17] = LOBYTE(stPANO.display_mode); 	 buff[18] = HIBYTE(stPANO.display_mode);		
	buff[19] = LOBYTE(stPANO.display_position);  buff[20] = HIBYTE(stPANO.display_position);	
	
	EEPROM_MultiWrite(EEPROM_PANO_AUX, (PBYTE)&buff[0], sizeof(buff));	
	
	buff_b[0] = EEPROM_SAVE_DONE;
	buff_b[1] = LOBYTE(stPANO.blend_left);       buff_b[2] = HIBYTE(stPANO.blend_left);
	buff_b[3] = LOBYTE(stPANO.blend_right);      buff_b[4] = HIBYTE(stPANO.blend_right);
	buff_b[5] = LOBYTE(stPANO.blend_l_lvl);      buff_b[6] = HIBYTE(stPANO.blend_l_lvl);
	buff_b[7] = LOBYTE(stPANO.blend_r_lvl);      buff_b[8] = HIBYTE(stPANO.blend_r_lvl);	
		
	EEPROM_MultiWrite(EEPROM_PANO_BLEND, (PBYTE)&buff_b[0], sizeof(buff_b));	

	UARTprintf("\r\n**Panorama setup write to EEPROM\r\n") ;
//#endif
#endif // #if defined (__PANORAMA_02__) && defined (__PANORAMA_03__)	
	
#if defined (__PANO_SPI_MASTER__)		
	SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_PANO_SAVE,0); // Cmd to __PANORAMA_03__
#else
	OSD_IrkeyTitleDisplay(ON, "Save done ");
#endif	
	
}

void SetApp_Pano_ReadEEPROM(void)
{
#if defined (__PANORAMA_02__) || defined (__PANORAMA_03__)	
	
//#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
    BYTE buff[21];
	BYTE buff_b[9];
           
	EEPROM_MultiRead(EEPROM_PANO_MAIN, (PBYTE)&buff[0], sizeof(buff));

	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.crop_pos_x_m = MAKEWORD(buff[2], buff[1]);
	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.crop_pos_y_m = MAKEWORD(buff[4], buff[3]);
	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.crop_width_m = MAKEWORD(buff[6], buff[5]);
	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.crop_height_m = MAKEWORD(buff[8], buff[7]);
	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.display_pos_x_m = MAKEWORD(buff[10], buff[9]);
	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.display_pos_y_m = MAKEWORD(buff[12], buff[11]);
	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.display_width_m = MAKEWORD(buff[14], buff[13]);
	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.display_height_m = MAKEWORD(buff[16], buff[15]);
	
	EEPROM_MultiRead(EEPROM_PANO_AUX, (PBYTE)&buff[0], sizeof(buff));

	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.crop_pos_x_a = MAKEWORD(buff[2], buff[1]);
	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.crop_pos_y_a = MAKEWORD(buff[4], buff[3]);
	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.crop_width_a = MAKEWORD(buff[6], buff[5]);
	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.crop_height_a = MAKEWORD(buff[8], buff[7]);
	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.display_pos_x_a = MAKEWORD(buff[10], buff[9]);
	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.display_pos_y_a = MAKEWORD(buff[12], buff[11]);
	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.display_width_a = MAKEWORD(buff[14], buff[13]);
	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.display_height_a = MAKEWORD(buff[16], buff[15]);	
	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.display_mode = MAKEWORD(buff[18], buff[17]);	
	if(buff[0] == EEPROM_SAVE_DONE)           stPANO.display_position = MAKEWORD(buff[20], buff[19]);	
	
	EEPROM_MultiRead(EEPROM_PANO_BLEND, (PBYTE)&buff_b[0], sizeof(buff_b));
	
	if(buff_b[0] == EEPROM_SAVE_DONE)           stPANO.blend_left = MAKEWORD(buff_b[2], buff_b[1]);
	if(buff_b[0] == EEPROM_SAVE_DONE)           stPANO.blend_right = MAKEWORD(buff_b[4], buff_b[3]);
	if(buff_b[0] == EEPROM_SAVE_DONE)           stPANO.blend_l_lvl = MAKEWORD(buff_b[6], buff_b[5]);
	if(buff_b[0] == EEPROM_SAVE_DONE)           stPANO.blend_r_lvl = MAKEWORD(buff_b[8], buff_b[7]);	
	
	EEPROM_MultiRead(EEPROM_PANO_WB, (PBYTE)&buff_b[0], sizeof(buff_b));
	
	if(buff_b[0] == EEPROM_SAVE_DONE)           stPANO.wb_gain_r_off_s1 = (SHORT)MAKEWORD(buff_b[2], buff_b[1]);
	if(buff_b[0] == EEPROM_SAVE_DONE)           stPANO.wb_gain_b_off_s1 = (SHORT)MAKEWORD(buff_b[4], buff_b[3]);
	if(buff_b[0] == EEPROM_SAVE_DONE)           stPANO.wb_gain_r_off_s2 = (SHORT)MAKEWORD(buff_b[6], buff_b[5]);
	if(buff_b[0] == EEPROM_SAVE_DONE)           stPANO.wb_gain_b_off_s2 = (SHORT)MAKEWORD(buff_b[8], buff_b[7]);

	if(buff[0] == EEPROM_SAVE_DONE)  	UARTprintf("\r\n**Panorama setup from EEPROM\r\n") ;	
	
	GetApp_Pano_info() ;
	
//#endif
	
#endif // #if defined (__PANORAMA_02__) && defined (__PANORAMA_03__)	
}

void SetApp_Pano_ResetEEPROM(void)
{
#if defined (__PANORAMA_02__) || defined (__PANORAMA_03__)	
	
//#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)	
	BYTE buff[2];
	
	buff[0] = EEPROM_RegRead(EEPROM_PANO_MAIN) ;
	
	if (buff[0] == EEPROM_SAVE_DONE) 	
	{
		buff[1] = 0x00;	
		EEPROM_RegWrite(EEPROM_PANO_MAIN, (WORD)buff[1]);
		EEPROM_RegWrite(EEPROM_PANO_AUX, (WORD)buff[1]);		
		EEPROM_RegWrite(EEPROM_PANO_BLEND, (WORD)buff[1]);	
		EEPROM_RegWrite(EEPROM_PANO_WB, (WORD)buff[1]);
		
#if defined (__PANO_SPI_MASTER__)		
	SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_PANO_DEL,0); // Cmd to __PANORAMA_03__
#else
	OSD_IrkeyTitleDisplay(ON, "DeleteDone");
#endif		
				
	}
	else								
	{
		SetApp_Pano_WriteEEPROM();
	}	

//#endif
	UARTprintf("\r\n**Panorama Reset EEPROM\r\n") ;
#endif // #if defined (__PANORAMA_02__) && defined (__PANORAMA_03__)		
}

void SetApp_Pano_Menu_WriteEEPROM(void)
{
#if defined (__PANORAMA_02__)
//#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)	
	BYTE buff_b[9];
	
	buff_b[0] = EEPROM_SAVE_DONE;
	buff_b[1] = LOBYTE(stPANO.wb_gain_r_off_s1);      buff_b[2] = HIBYTE(stPANO.wb_gain_r_off_s1);
	buff_b[3] = LOBYTE(stPANO.wb_gain_b_off_s1);      buff_b[4] = HIBYTE(stPANO.wb_gain_b_off_s1);
	buff_b[5] = LOBYTE(stPANO.wb_gain_r_off_s2);      buff_b[6] = HIBYTE(stPANO.wb_gain_r_off_s2);
	buff_b[7] = LOBYTE(stPANO.wb_gain_b_off_s2);      buff_b[8] = HIBYTE(stPANO.wb_gain_b_off_s2);	
	
	EEPROM_MultiWrite(EEPROM_PANO_WB, (PBYTE)&buff_b[0], sizeof(buff_b));
//#endif
#endif // #if defined (__PANORAMA_02__)
}

#if defined (__PANORAMA_02__)
static void SetApp_Pano_WB_GainR_offset(WORD cID, WORD val)
{
	if(cID==SPIM_MISP_CID_S1)		stPANO.wb_gain_r_off_s1 = (val < 125)?  -(125-val):  val-125;	// -125 ~ +125
	if(cID==SPIM_MISP_CID_S2)		stPANO.wb_gain_r_off_s2 = (val < 125)?  -(125-val):  val-125;	// -125 ~ +125
}
#endif

#if defined (__PANORAMA_02__)
static void SetApp_Pano_WB_GainB_offset(WORD cID, WORD val)
{
	if(cID==SPIM_MISP_CID_S1)		stPANO.wb_gain_b_off_s1 = (val < 125)?  -(125-val):  val-125;	// -125 ~ +125
	if(cID==SPIM_MISP_CID_S2)		stPANO.wb_gain_b_off_s2 = (val < 125)?  -(125-val):  val-125;	// -125 ~ +125
}
#endif

WORD ConvApp_Pano_WB_GainR(WORD cID, SHORT gain)
{
#if defined (__PANORAMA_02__)
	if(cID==SPIM_MISP_CID_S1)		return  (gain + stPANO.wb_gain_r_off_s1 > 0)?  gain + stPANO.wb_gain_r_off_s1:  0;
	if(cID==SPIM_MISP_CID_S2)		return  (gain + stPANO.wb_gain_r_off_s2 > 0)?  gain + stPANO.wb_gain_r_off_s2:  0;
#endif
	
	return  0;
}

WORD ConvApp_Pano_WB_GainB(WORD cID, SHORT gain)
{
#if defined (__PANORAMA_02__)
	if(cID==SPIM_MISP_CID_S1)		return  (gain + stPANO.wb_gain_b_off_s1 > 0)?  gain + stPANO.wb_gain_b_off_s1:  0;
	if(cID==SPIM_MISP_CID_S2)		return  (gain + stPANO.wb_gain_b_off_s2 > 0)?  gain + stPANO.wb_gain_b_off_s2:  0;
#endif
	
	return  0;
}

void SetAPP_Pano_Menu_Ctrl(WORD val)
{
#if defined (__PANORAMA_02__)
	stPANO.menu_ctrl_mode = val;
#endif
}

void SetAPP_Pano_Menu_Param(WORD val)
{
#if defined (__PANORAMA_02__)
	switch(stPANO.menu_ctrl_mode)	{
		case  0:	return;
		case  1:	SetApp_Pano_WB_GainR_offset(SPIM_MISP_CID_S1, val);		break;	//gain-r offset s1
		case  2:	SetApp_Pano_WB_GainB_offset(SPIM_MISP_CID_S1, val);		break;	//gain-b offset s1
		case  3:	SetApp_Pano_WB_GainR_offset(SPIM_MISP_CID_S2, val);		break;	//gain-r offset s2
		case  4:	SetApp_Pano_WB_GainB_offset(SPIM_MISP_CID_S2, val);		break;	//gain-b offset s2
	}
	
	if(stPANO.menu_ctrl_mode != 0)		SetApp_Pano_Menu_WriteEEPROM();
#endif
}

WORD GetAPP_Pano_Menu_Param(void)
{
#if defined (__PANORAMA_02__)
	switch(stPANO.menu_ctrl_mode)	{
		case  0:	return  125;
		case  1:	return  ConvApp_Pano_WB_GainR(SPIM_MISP_CID_S1, 125);	//gain-r offset s1
		case  2:	return  ConvApp_Pano_WB_GainB(SPIM_MISP_CID_S1, 125);	//gain-b offset s1
		case  3:	return  ConvApp_Pano_WB_GainR(SPIM_MISP_CID_S2, 125);	//gain-r offset s2
		case  4:	return  ConvApp_Pano_WB_GainB(SPIM_MISP_CID_S2, 125);	//gain-b offset s2
	}
#endif
	
	return  0;
}

#endif	//__PANORAMA_SENSOR_SETUP__

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
/*  FILE_END_HERE */
