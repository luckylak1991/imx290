// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"
#include	"osd.h"
#include	"app_cascade_cam.h"

#if defined(__CASCADE_CAM_SETUP__)
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static MISP_CASCADE_PARAM stCASCADE;
// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------
static void SetApp_Cascade_CompMode_SD(void) ;
static void CtrlApp_Cascade_Main(void) ;
static void CtrlApp_Cascade_Aux(void) ;
// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Init_Cascade_Cam(void)
{
	SetApp_Cascade_CompMode_SD() ; // 1280x720x3 scale down
}
//--------------------------------------------------------------------------------------------------------------------------
static void SetApp_Cascade_CompMode_SD(void)
{
	stCASCADE.select_sensor = 0 ;
	
#if defined(__CASCADE_CAM_02__)
	//==================================================//
	// FHD full attach  SetApp_Cascade_CompMode_SD
	//==================================================//
	Delay_mSec(100);
	// In out format / PiP On / path select /
	// SetIN_MainSrcExtPort1() ;
	// SetIN_AuxSrcSensor() ;	

	SetIN_MainSrcSensor() ;
	SetIN_AuxSrcExtPort1() ;		
		
//	SetIN_ExtPort1Mode(MISP_SRC_SEP422_8) ;

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
	
	SetOUT_AuxMode(GetOUT_MainMode());
	SetOUT_AuxFrmt(GetOUT_MainFrmt());

	

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
//	SetIn_MainHsPolar(ON) ;
//	SetIn_MainVsPolar(ON) ;
//	SetIn_AuxHsPolar(ON) ;
//	SetIn_AuxVsPolar(ON) ;
	//============================================================//

	stCASCADE.display_mode = MISP_CASCADE_DISP_MODE_2 ;
	stCASCADE.display_position = 1 ;	
	
	stCASCADE.blend_left = 0 ;		
	stCASCADE.blend_right = 0 ;		
	
	stCASCADE.blend_l_lvl = 5 ; // aux 0~15 main
	stCASCADE.blend_r_lvl = 10 ; // aux 0~15 main	
		
	stCASCADE.in_size_h_m = GetIN_MainSrcSizeH() ;
	stCASCADE.in_size_v_m = GetIN_MainSrcSizeV() ;	

	 stCASCADE.crop_pos_x_m = 0 ;			
	 stCASCADE.crop_pos_y_m = 0 ;						
	 stCASCADE.crop_width_m = 1920  ;		
	 stCASCADE.crop_height_m = 1080 ;

	 stCASCADE.display_pos_x_m = 1920 ; // 
	 stCASCADE.display_pos_y_m = 0 ; // 	
	 stCASCADE.display_width_m = 1920 ;
	 stCASCADE.display_height_m = 2160 ;	
	
	stCASCADE.in_size_h_a = GetIN_AuxSrcSizeH() ;
	stCASCADE.in_size_v_a = GetIN_AuxSrcSizeV() ;	

	stCASCADE.crop_pos_x_a = 0 ;	//start pos
	stCASCADE.crop_pos_y_a = 0 ;		//		
	stCASCADE.crop_width_a = 1920  ;
	stCASCADE.crop_height_a = 1080 ;

	stCASCADE.display_pos_x_a = 0 ; 	
	stCASCADE.display_pos_y_a = 0 ;	
	stCASCADE.display_width_a = 1920;	
	stCASCADE.display_height_a = 2160 ;	

//	SetApp_Cascade_ReadEEPROM() ;
	
	//============================================================//
	
	CtrlApp_Cascade_Main()	;

	//============================================================//
	
	CtrlApp_Cascade_Aux() ;

	//============================================================//
//	SetOUT_PipBlend(stCASCADE.blend_l_lvl) ; // aux 0~15 main
	
	MISP_SW_Reset();
	//============================================================//
#elif defined(__CASCADE_CAM_03__) // SetApp_Cascade_CompMode_SD
	//==================================================//
	
	// In out format / PiP On / path select /
	// SetIN_MainSrcExtPort1() ;
	// SetIN_AuxSrcSensor() ;			

	SetIN_MainSrcSensor() ;
	SetIN_AuxSrcExtPort1() ;		

//	SetIN_ExtPort1Mode(MISP_SRC_SEP422_8) ;

	
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
//	SetIn_MainHsPolar(ON) ;
//	SetIn_MainVsPolar(ON) ;
//	SetIn_AuxHsPolar(ON) ;
//	SetIn_AuxVsPolar(ON) ;
	//============================================================//

	stCASCADE.display_mode = MISP_CASCADE_DISP_MODE_2 ;
	stCASCADE.display_position = 1 ;	

	stCASCADE.blend_left = 0 ;		
	stCASCADE.blend_right = 0 ;		
	
	stCASCADE.blend_l_lvl = 5 ; // aux 0~15 main	
	stCASCADE.blend_r_lvl = 10 ; // aux 0~15 main	
	
	stCASCADE.in_size_h_m = GetIN_MainSrcSizeH() ;
	stCASCADE.in_size_v_m = GetIN_MainSrcSizeV() ;	
	
	 stCASCADE.crop_pos_x_m = 0 ;			
	 stCASCADE.crop_pos_y_m = 0 ;				
	 stCASCADE.crop_width_m = 1920  ;
	 stCASCADE.crop_height_m = 1080 ;

	// stCASCADE.display_pos_x_m = 960+1920 ; 	
	// stCASCADE.display_pos_y_m = 0 ;	
	// stCASCADE.display_width_m = 960 ; 
	// stCASCADE.display_height_m = 1080 ;

//	stCASCADE.crop_pos_x_m = 13 ;			
//	stCASCADE.crop_pos_y_m = 48 ;				
//	stCASCADE.crop_width_m = 947  ;
//	stCASCADE.crop_height_m = 882 ;

	stCASCADE.display_pos_x_m = 2560 ; 	
	stCASCADE.display_pos_y_m = 540 ;	
	stCASCADE.display_width_m = 1280 ;	
	stCASCADE.display_height_m = 1080 ;			
	
	stCASCADE.in_size_h_a = GetIN_AuxSrcSizeH() ;
	stCASCADE.in_size_v_a = GetIN_AuxSrcSizeV() ;	

	// stCASCADE.crop_pos_x_a = 0 ;			
	// stCASCADE.crop_pos_y_a = 0 ;						
	// stCASCADE.crop_width_a = 960+1920  ;		
	// stCASCADE.crop_height_a = 1080 ;

	// stCASCADE.display_pos_x_a = 0 ; //	 
	// stCASCADE.display_pos_y_a = 0 ; 
	// stCASCADE.display_width_a = 960+1920 ;
	// stCASCADE.display_height_a = 1080 ;	

	stCASCADE.crop_pos_x_a = 0 ;			
	stCASCADE.crop_pos_y_a = 0 ;						
	stCASCADE.crop_width_a = 3840  ;		
	stCASCADE.crop_height_a = 2160 ;

	stCASCADE.display_pos_x_a = 0 ; // 
	stCASCADE.display_pos_y_a = 540 ; // 	
	stCASCADE.display_width_a = 2560 ;
	stCASCADE.display_height_a = 1080 ;	
	
//	SetApp_Cascade_ReadEEPROM() ;
	
	//============================================================//
	
	CtrlApp_Cascade_Main()	;

	SetCLK_InvYC(0);
	SetCLK_DlyYC(0);
	SetCLK_InvYCm(0);
	SetCLK_DlyYCm(0);	
	
	//============================================================//
	
	CtrlApp_Cascade_Aux() ;

	//============================================================//
	SetOUT_PipBlend(stCASCADE.blend_r_lvl) ; // aux 0~15 main
	
	stWBCTRL.ctrl &= (~WBCTRL_MODE_MASK);
	stWBCTRL.ctrl |= (WBCTRL_MANUAL_MODE|WBCTRL_TRIGGER_ISP);
	//============================================================//	
#elif defined(__CASCADE_CAM_01__) // SetApp_Cascade_CompMode_SD		
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
	SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p60) ;
	//SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p30) ;		//mispfrmt.c see defDVIRXOffset. test		{ 148,   36} 
#endif
	
//	SetOUT_MainMode(MISP_OUT_SEP422_8);

	SetDDRMAP_IPC_frame_count(2) ;
	SetDDRMAP_AUX_frame_count(2) ;	
	
	SetIN_MainFRcntY(2) ;	SetIN_MainFRdlyY(0);
	SetIN_MainFRcntC(2) ;	SetIN_MainFRdlyC(0);
	SetIN_AuxFRcnt(2) ;		SetIN_AuxFRdly(0);	
	
	MISP_SW_Reset();
	
	stWBCTRL.ctrl &= (~WBCTRL_MODE_MASK);
	stWBCTRL.ctrl |= (WBCTRL_MANUAL_MODE|WBCTRL_TRIGGER_ISP);


	SetBY_Lens_corr_en(ON);		// lens shading correction 'on'
		
		
	UARTprintf("\r\n** CASCADE init done \r\n");
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void CtrlApp_Cascade_Main(void)
{
#if defined(__CASCADE_CAM_02__)
	WORD pos_x_m ;
	WORD width_m ;
	WORD b_h ;
	WORD b_v ;
	WORD crop_x ;
	WORD crop_y ;
	
	pos_x_m = stCASCADE.display_pos_x_m - stCASCADE.blend_left ;
	width_m = stCASCADE.display_width_m + stCASCADE.blend_left + stCASCADE.blend_right ;	
		
	// Main view
	// SetFC_MainFCdstPosiX(stCASCADE.display_pos_x_m);
	SetFC_MainFCdstPosiX(pos_x_m);
	SetFC_MainFCdstPosiY(stCASCADE.display_pos_y_m);
	// SetFC_MainFCdstSizeH(stCASCADE.display_width_m);
	SetFC_MainFCdstSizeH(width_m);
	SetFC_MainFCdstSizeV(stCASCADE.display_height_m);	

	// Init Crop (Main)
	// SetFC_MainFCsrcPosiX(stCASCADE.crop_pos_x_m);
	// SetFC_MainFCsrcPosiY(stCASCADE.crop_pos_y_m);
	
	b_h = GetSS_image_boundary_size_h() ;
	b_v = GetSS_image_boundary_size_v() ;	
		
	crop_x = stCASCADE.crop_pos_x_m + b_h ;
	crop_y = stCASCADE.crop_pos_y_m + b_v ;
	
	if ( crop_x > 254 )	crop_x = 254 ;
	if ( crop_y > 255 )	crop_y = 255 ;
		
	SetFC_MainFCsrcPosiX(0);
	SetFC_MainFCsrcPosiY(0);	
	
	SetDM_image_size(crop_x,crop_y,stCASCADE.crop_width_m,stCASCADE.in_size_v_m) ;	

	SetFC_MainFCsrcSizeH(stCASCADE.crop_width_m);
	SetFC_MainFCsrcSizeV(stCASCADE.crop_height_m);	
	
	if ( stCASCADE.blend_left == 0) 
	{
		SetOUT_PipGradBlendSel(0,0);
	}
	else 
	{
		SetOUT_PipGradBlendSel(0,1);
		SetOUT_PipGradBlendEn(0,1,0,0,(4095/stCASCADE.blend_left)); // horizontal PiP to Main blend
		SetOUT_PipGradBlendPos(0,pos_x_m,0,(stCASCADE.display_pos_x_m),2160) ;
	}	
	
#elif defined(__CASCADE_CAM_03__)	
	
	WORD b_h ;
	WORD b_v ;
	WORD crop_x ;
	WORD crop_y ;	
	
	// Main view
	SetFC_MainFCdstPosiX(stCASCADE.display_pos_x_m);
	SetFC_MainFCdstPosiY(stCASCADE.display_pos_y_m);
	SetFC_MainFCdstSizeH(stCASCADE.display_width_m);
	SetFC_MainFCdstSizeV(stCASCADE.display_height_m);
	
	// Init Crop (Main)
	// SetFC_MainFCsrcPosiX(stCASCADE.crop_pos_x_m);
	// SetFC_MainFCsrcPosiY(stCASCADE.crop_pos_y_m);
	
	b_h = GetSS_image_boundary_size_h() ;
	b_v = GetSS_image_boundary_size_v() ;	
		
	crop_x = stCASCADE.crop_pos_x_m + b_h ;
	crop_y = stCASCADE.crop_pos_y_m + b_v ;		

	if ( crop_x > 254 )	crop_x = 254 ;
	if ( crop_y > 255 )	crop_y = 255 ;	
	
	SetDM_image_size(crop_x,crop_y,stCASCADE.crop_width_m,stCASCADE.in_size_v_m) ;	
	
	SetFC_MainFCsrcSizeH(stCASCADE.crop_width_m);
	SetFC_MainFCsrcSizeV(stCASCADE.crop_height_m);	
#endif
}


static void CtrlApp_Cascade_Aux(void)
{
#if defined(__CASCADE_CAM_02__)
	// Aux view
	SetFC_AuxFCdstPosiX(stCASCADE.display_pos_x_a);
	SetFC_AuxFCdstPosiY(stCASCADE.display_pos_y_a);
	SetFC_AuxFCdstSizeH(stCASCADE.display_width_a);
	SetFC_AuxFCdstSizeV(stCASCADE.display_height_a);		

	// Init Crop (Aux)
	SetFC_AuxFCsrcPosiX(stCASCADE.crop_pos_x_a);
	SetFC_AuxFCsrcPosiY(stCASCADE.crop_pos_y_a);
	SetFC_AuxFCsrcSizeH(stCASCADE.crop_width_a);
	SetFC_AuxFCsrcSizeV(stCASCADE.crop_height_a);		
#elif defined(__CASCADE_CAM_03__)
	WORD dis_width_a ;
	WORD src_width_a ;
	
	dis_width_a = stCASCADE.display_width_a + stCASCADE.blend_right ;
	src_width_a = stCASCADE.crop_width_a + stCASCADE.blend_right ;		
	
	// Aux view
	SetFC_AuxFCdstPosiX(stCASCADE.display_pos_x_a);
	SetFC_AuxFCdstPosiY(stCASCADE.display_pos_y_a);
	// SetFC_AuxFCdstSizeH(stCASCADE.display_width_a);
	SetFC_AuxFCdstSizeH(dis_width_a);
	SetFC_AuxFCdstSizeV(stCASCADE.display_height_a);		

	// Init Crop (Aux)
	SetFC_AuxFCsrcPosiX(stCASCADE.crop_pos_x_a);
	SetFC_AuxFCsrcPosiY(stCASCADE.crop_pos_y_a);
	// SetFC_AuxFCsrcSizeH(stCASCADE.crop_width_a);
	SetFC_AuxFCsrcSizeH(src_width_a);
	SetFC_AuxFCsrcSizeV(stCASCADE.crop_height_a);
	
	if ( stCASCADE.blend_right == 0) 
	{
		SetOUT_PipGradBlendSel(0,0);
	}
	else 
	{
		SetOUT_PipGradBlendSel(0,1);
		SetOUT_PipGradBlendEn(0,1,0,0,(4095/stCASCADE.blend_right)); // horizontal PiP to Main blend
		SetOUT_PipGradBlendPos(0,stCASCADE.display_width_a,0,(dis_width_a),2160) ;
	}	
	
#endif	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Cascade_DisplayMode(WORD mode, WORD position)
{
	WORD pre_mode ;
	
	pre_mode = stCASCADE.display_mode ;

//==================================================//
#if defined(__CASCADE_SPI_MASTER__) // __CASCADE_CAM_02__
	
	WORD spi_cmd_option ;
	
	spi_cmd_option = MAKEWORD(mode, position);
	SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_CASCADE_DISP,spi_cmd_option); // Cmd to __CASCADE_CAM_03__
	//SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_IR_KEY, 0x16e9);
#endif	
//==================================================//		
	
#if defined(__CASCADE_CAM_02__)
	//==================================================//	
	if ( mode == MISP_CASCADE_DISP_MODE_0 ) // 0 : 1280x720x3 full display 2 : 1280x1080x3 full display
	{
		stCASCADE.crop_pos_x_a = 0 ;
		
		// stCASCADE.display_pos_x_a = 0 ; // 

		stCASCADE.display_width_a = 1280 ;
		stCASCADE.display_height_a = 720 ;	
		
		stCASCADE.display_pos_x_m = 1280 ; // 
			
		stCASCADE.display_width_m = 1280 ;
		stCASCADE.display_height_m = 720 ;		
		// OSD_IrkeyTitleDisplay(ON, "Disp Mode0");
	}
	else if ( mode == MISP_CASCADE_DISP_MODE_1 ) // 960x1080 + 1920x1080 + 960x1080 H scale down display
	{
		stCASCADE.crop_pos_x_a = 0 ;
		
		// stCASCADE.display_pos_x_a = 0 ; // 

		stCASCADE.display_width_a = 960 ;
		stCASCADE.display_height_a = 1080 ;	
		
		stCASCADE.display_pos_x_m = 960 ; // 
			
		stCASCADE.display_width_m = 1920 ;
		stCASCADE.display_height_m = 1080 ;
		// OSD_IrkeyTitleDisplay(ON, "Disp Mode1");
	}
	else if ( mode == MISP_CASCADE_DISP_MODE_2 ) // 1280x1080 + 1280x1080 + 1280x1080 => All sensor H scale down
	{
		stCASCADE.crop_pos_x_a = 0 ;
		
		// stCASCADE.display_pos_x_a = 0 ; // 

		stCASCADE.display_width_a = 1280 ;
		stCASCADE.display_height_a = 1080 ;	
		
		stCASCADE.display_pos_x_m = 1280 ; // 
			
		stCASCADE.display_width_m = 1280 ;
		stCASCADE.display_height_m = 1080 ;	
		// OSD_IrkeyTitleDisplay(ON, "Disp Mode2");
	}	
	else if ( mode == MISP_CASCADE_DISP_MODE_3 ) // 960x1080 + 960x1080 + 960x1080 =>  Left, Right sensor H crop
	{
		// stCASCADE.crop_pos_x_a = 960 ;
		
		// stCASCADE.display_pos_x_a = 0 ; //
		
		stCASCADE.display_width_a = 960 ;
		stCASCADE.display_height_a = 1080 ;	
		
		stCASCADE.display_pos_x_m = 960 ; // 
			
		stCASCADE.display_width_m = 1920 ;
		stCASCADE.display_height_m = 1080 ;
		// OSD_IrkeyTitleDisplay(ON, "Disp Mode3");
	}
	
	if ( (pre_mode != MISP_CASCADE_DISP_MODE_3) && (mode == MISP_CASCADE_DISP_MODE_3) )		
	{
		stCASCADE.crop_pos_x_a = 960 ;	
		stCASCADE.crop_width_a = stCASCADE.crop_width_a - 960 ;	
	}
	else if ( pre_mode == MISP_CASCADE_DISP_MODE_3 && (mode != MISP_CASCADE_DISP_MODE_3)  )	
	{
		stCASCADE.crop_pos_x_a = 0 ;		
		stCASCADE.crop_width_a = stCASCADE.crop_width_a + 960 ;		
	}
		
	// if ( position == 0 ) // Top position
	// {
		// stCASCADE.display_pos_y_m = 0 ;
		// stCASCADE.display_pos_y_a = 0 ; // 	
	// }
	// else // center position
	// {
		// stCASCADE.display_pos_y_m = 0 ;
		// stCASCADE.display_pos_y_a = 0 ; // 	
	// }
#elif defined(__CASCADE_CAM_03__)
	//==================================================//
	if ( mode == MISP_CASCADE_DISP_MODE_0 ) // 0 : 1280x720x3 full display 2 : 1280x1080x3 full display
	{
		// if ( pre_mode == MISP_CASCADE_DISP_MODE_1 || pre_mode == MISP_CASCADE_DISP_MODE_3 )	
			// stCASCADE.crop_width_a = stCASCADE.crop_width_a - 320 ;
		
		// stCASCADE.crop_height_a = 720 ;
		
		// stCASCADE.display_pos_x_a = 0 ; 	

		stCASCADE.display_width_a = 1280*2 ; // 2560				
		stCASCADE.display_height_a = 720 ;		
		
		stCASCADE.display_pos_x_m = 1280*2 ; // 
			
		stCASCADE.display_width_m = 1280 ;
		stCASCADE.display_height_m = 720 ;
		OSD_IrkeyTitleDisplay(ON, "Disp Mode0");
	}
	else if ( mode == MISP_CASCADE_DISP_MODE_1 ) // 960x1080 + 1920x1080 + 960x1080 H scale down display
	{
		// if ( pre_mode == MISP_CASCADE_DISP_MODE_0 || pre_mode == MISP_CASCADE_DISP_MODE_2 )	
			// stCASCADE.crop_width_a = stCASCADE.crop_width_a + 320 ;
		
		// stCASCADE.crop_height_a = 1080 ;
		
		// stCASCADE.display_pos_x_a = 0 ; 	
			
		stCASCADE.display_width_a = 960 + 1920 ; // 2880
		stCASCADE.display_height_a = 1080 ;	
		
		stCASCADE.display_pos_x_m = 960 + 1920 ; // 
			
		stCASCADE.display_width_m = 960 ;
		stCASCADE.display_height_m = 1080 ;
		OSD_IrkeyTitleDisplay(ON, "Disp Mode1");
	}
	else if ( mode == MISP_CASCADE_DISP_MODE_2 ) // 1280x1080 + 1280x1080 + 1280x1080 => All sensor H scale down
	{
		// if ( pre_mode == MISP_CASCADE_DISP_MODE_1 || pre_mode == MISP_CASCADE_DISP_MODE_3 )	
			// stCASCADE.crop_width_a = stCASCADE.crop_width_a - 320 ;
		
		// stCASCADE.crop_height_a = 1080 ;
		
		// stCASCADE.display_pos_x_a = 0 ; 	
			
		stCASCADE.display_width_a = 1280*2 ; // 2560
		stCASCADE.display_height_a = 1080 ;	
		
		stCASCADE.display_pos_x_m = 1280*2 ; // 
			
		stCASCADE.display_width_m = 1280 ;
		stCASCADE.display_height_m = 1080 ;
		OSD_IrkeyTitleDisplay(ON, "Disp Mode2");
	}	
	else if ( mode == MISP_CASCADE_DISP_MODE_3 ) // 960x1080 + 960x1080 + 960x1080 =>  Left, Right sensor H crop
	{
		// if ( pre_mode == MISP_CASCADE_DISP_MODE_0 || pre_mode == MISP_CASCADE_DISP_MODE_2 )	
			// stCASCADE.crop_width_a = stCASCADE.crop_width_a + 320 ;
		
		// stCASCADE.crop_height_a = 1080 ;
		
		// stCASCADE.display_pos_x_a = 0 ; 	
			
		stCASCADE.display_width_a = 960 + 1920 ; // 2880
		stCASCADE.display_height_a = 1080 ;	
		
		stCASCADE.display_pos_x_m = 960 + 1920 ; // 
			
		stCASCADE.display_width_m = 960 ;
		stCASCADE.display_height_m = 1080 ;	
		OSD_IrkeyTitleDisplay(ON, "Disp Mode3");
	}
	
	if ( ((pre_mode == MISP_CASCADE_DISP_MODE_1) || (pre_mode == MISP_CASCADE_DISP_MODE_3)) &&
			((mode == MISP_CASCADE_DISP_MODE_0) || (mode == MISP_CASCADE_DISP_MODE_2)) )	
	{
		stCASCADE.crop_width_a = stCASCADE.crop_width_a - 320 ;
	}
	else if ( ((pre_mode == MISP_CASCADE_DISP_MODE_0) || (pre_mode == MISP_CASCADE_DISP_MODE_2)) &&
			((mode == MISP_CASCADE_DISP_MODE_1) || (mode == MISP_CASCADE_DISP_MODE_3)) )	
	{
		stCASCADE.crop_width_a = stCASCADE.crop_width_a + 320 ;
	}
		
	if ( mode == MISP_CASCADE_DISP_MODE_0 )	stCASCADE.crop_height_a = 720 ;
	else									stCASCADE.crop_height_a = 1080 ;
	
	if ( (pre_mode != MISP_CASCADE_DISP_MODE_3) && (mode == MISP_CASCADE_DISP_MODE_3) )		stCASCADE.crop_width_m = stCASCADE.crop_width_m - 960 ;	
	else if ( pre_mode == MISP_CASCADE_DISP_MODE_3 && (mode != MISP_CASCADE_DISP_MODE_3)  )	stCASCADE.crop_width_m = stCASCADE.crop_width_m + 960 ;	
		
	
	if ( position == 0 ) // Top position
	{
		stCASCADE.display_pos_y_m = 0 ;
		stCASCADE.display_pos_y_a = 0 ; // 
	}
	else // center position
	{
		if ( mode == MISP_CASCADE_DISP_MODE_0 )
		{
			stCASCADE.display_pos_y_m = 720 ;
			stCASCADE.display_pos_y_a = 720 ; //
		}
		else
		{
			stCASCADE.display_pos_y_m = 540 ;
			stCASCADE.display_pos_y_a = 540 ; //		
		}
	}
	//==================================================//
#endif
	
	stCASCADE.display_mode = mode ;
	
	if ( stCASCADE.display_position != position ) OSD_IrkeyTitleDisplay(OFF, "Disp   Off");
	
	stCASCADE.display_position = position ;	
		
	CtrlApp_Cascade_Main() ;
	CtrlApp_Cascade_Aux() ;	
}
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Cascade_BlendLeftArea(WORD area)
{
#if defined(__CASCADE_CAM_02__)
	//==================================================//
	// Left area setup
	//==================================================//
	WORD display_bound ;

	display_bound = stCASCADE.display_width_a ;
	
	if ( display_bound > area )
	{
		stCASCADE.blend_left = area ;
	}
	else
	{
		stCASCADE.blend_left = display_bound ;
	}
	
	CtrlApp_Cascade_Main() ;
	CtrlApp_Cascade_Aux() ;	
	//==================================================//
#elif defined(__CASCADE_CAM_03__)	
	//==================================================//
	// Left area setup
	//==================================================//
	
	// NULL
	stCASCADE.blend_left = 0 ;
	
	//==================================================//
#endif		
}

void SetApp_Cascade_BlendRightArea(WORD area)
{
	
#if defined (__CASCADE_SPI_MASTER__)		
	SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_CASCADE_BLND,area); // Cmd to __CASCADE_CAM_03__
#endif		
		
#if defined(__CASCADE_CAM_02__)
	//==================================================//
	// Right area setup
	//==================================================//
	WORD display_end_pos ;
	WORD area_bound ;

	display_end_pos = stCASCADE.display_width_m + stCASCADE.display_width_a ;
	area_bound = 3840 - display_end_pos ;	
	
	if ( area_bound > area )
	{
		stCASCADE.blend_right = area ;
	}
	else
	{
		stCASCADE.blend_right = area_bound ;
	}
	//==================================================//
#elif defined(__CASCADE_CAM_03__)	
	//==================================================//
	// Right area setup
	//==================================================//
	WORD display_end_pos ;
	WORD area_bound ;
	
	display_end_pos = stCASCADE.display_width_a ;
	area_bound = 3840 - display_end_pos ;	
	
	if ( area_bound > area )
	{
		stCASCADE.blend_right = area ;
	}
	else
	{
		stCASCADE.blend_right = area_bound ;
	}	
	
	//==================================================//
#endif
	
	CtrlApp_Cascade_Main() ;
	CtrlApp_Cascade_Aux() ;		
}
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Cascade_BlendLevel(WORD level)
{
#if defined (__CASCADE_SPI_MASTER__)		
	SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_CASCADE_BLVL,level); // Cmd to __CASCADE_CAM_03__
#endif		
	
#if defined(__CASCADE_CAM_02__)
	
	stCASCADE.blend_l_lvl = level ;
	stCASCADE.blend_r_lvl = 15-level ;
	
	SetOUT_PipBlend(stCASCADE.blend_l_lvl);

#elif defined(__CASCADE_CAM_03__)	

	stCASCADE.blend_l_lvl = level ;
	stCASCADE.blend_r_lvl = 15-level ;
	
	SetOUT_PipBlend(stCASCADE.blend_r_lvl);	
	
#endif
}
//--------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Cascade_CropLeftX(WORD channel, WORD direction)
{	
	WORD crop_pos_x_m ;
	WORD crop_width_m ;
		
	if ( channel == 1 ) // Main
	{
		crop_pos_x_m = stCASCADE.crop_pos_x_m ;
		crop_width_m = stCASCADE.crop_width_m ;
	}
	else // Aux
	{
		crop_pos_x_m = stCASCADE.crop_pos_x_a ;
		crop_width_m = stCASCADE.crop_width_a ;	
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
		stCASCADE.crop_pos_x_m = crop_pos_x_m ;
		stCASCADE.crop_width_m = crop_width_m ;
		CtrlApp_Cascade_Main() ;
	}
	else // Aux
	{
		stCASCADE.crop_pos_x_a = crop_pos_x_m ;
		stCASCADE.crop_width_a = crop_width_m ;
		CtrlApp_Cascade_Aux() ;
	}		
}

void SetApp_Cascade_CropRightX(WORD channel, WORD direction)
{
	WORD crop_width_m ;
	WORD in_size_h_m ;
	
	if ( channel == 1 ) // Main
	{
		crop_width_m = stCASCADE.crop_width_m ;
		in_size_h_m = stCASCADE.in_size_h_m - stCASCADE.crop_pos_x_m ;
	}
	else // Aux
	{
		crop_width_m = stCASCADE.crop_width_a ;	
		in_size_h_m = stCASCADE.in_size_h_a - stCASCADE.crop_pos_x_a ;
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
		stCASCADE.crop_width_m = crop_width_m ;
		CtrlApp_Cascade_Main() ;
	}
	else // Aux
	{
		stCASCADE.crop_width_a = crop_width_m ;	
		CtrlApp_Cascade_Aux() ;
	}	
}
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Cascade_CropTopY(WORD channel, WORD direction)
{
	WORD crop_pos_y_m ;
	WORD crop_height_m ;
	
	if ( channel == 1 ) // Main
	{
		crop_pos_y_m = stCASCADE.crop_pos_y_m ;
		crop_height_m = stCASCADE.crop_height_m ;
	}
	else // Aux
	{
		crop_pos_y_m = stCASCADE.crop_pos_y_a ;	
		crop_height_m = stCASCADE.crop_height_a ;
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
		stCASCADE.crop_pos_y_m = crop_pos_y_m ;
		stCASCADE.crop_height_m = crop_height_m ;	
		CtrlApp_Cascade_Main() ;		
	}
	else // Aux
	{
		stCASCADE.crop_pos_y_a = crop_pos_y_m ;	
		stCASCADE.crop_height_a = crop_height_m ;
		CtrlApp_Cascade_Aux() ;
	}
}

void SetApp_Cascade_CropBtmY(WORD channel, WORD direction)
{
	WORD crop_height_m ;
	WORD in_size_v_m ;
	
	if ( channel == 1 ) // Main
	{
		crop_height_m = stCASCADE.crop_height_m ;
		in_size_v_m = stCASCADE.in_size_v_m - stCASCADE.crop_pos_y_m ;
	}
	else // Aux
	{
		crop_height_m = stCASCADE.crop_height_a ;	
		in_size_v_m = stCASCADE.in_size_v_a - stCASCADE.crop_pos_y_a ;
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
		stCASCADE.crop_height_m = crop_height_m ;
		CtrlApp_Cascade_Main() ;
	}
	else // Aux
	{
		stCASCADE.crop_height_a = crop_height_m ;	
		CtrlApp_Cascade_Aux() ;
	}		
}

//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Cascade_WindowMoveX(WORD channel, WORD direction)
{	
	WORD display_pos_x_m ;
	WORD right_bound ;
		
	if ( channel == 1 ) // Main
	{
		display_pos_x_m = stCASCADE.display_pos_x_m ;
		right_bound = GetOUT_MainSizeHA() - stCASCADE.display_width_m -1 ;
	}
	else // Aux
	{
		display_pos_x_m = stCASCADE.display_pos_x_a ;	
		right_bound = GetOUT_MainSizeHA() - stCASCADE.display_width_a -1 ;
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
		stCASCADE.display_pos_x_m = display_pos_x_m ;
		CtrlApp_Cascade_Main() ;	
	}
	else // Aux
	{
		stCASCADE.display_pos_x_a = display_pos_x_m ;	
		CtrlApp_Cascade_Aux() ;
	}		
}

void SetApp_Cascade_WindowMoveY(WORD channel, WORD direction)
{
	WORD display_pos_y_m ;
	WORD btm_bound ;
	
	if ( channel == 1 ) // Main
	{
		display_pos_y_m = stCASCADE.display_pos_y_m ;
		btm_bound = GetOUT_MainSizeVA() - stCASCADE.display_height_m ;
	}
	else // Aux
	{
		display_pos_y_m = stCASCADE.display_pos_y_a ;	
		btm_bound = GetOUT_MainSizeVA() - stCASCADE.display_height_a ;
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
		stCASCADE.display_pos_y_m = display_pos_y_m ;
		CtrlApp_Cascade_Main() ;
	}
	else // Aux
	{
		stCASCADE.display_pos_y_a = display_pos_y_m ;	
		CtrlApp_Cascade_Aux() ;
	}		
}

//--------------------------------------------------------------------------------------------------------------------------

void SetApp_Cascade_WindowStretchX(WORD channel, WORD direction)
{
	WORD window_right_pos ;
	WORD right_bound ;
	WORD display_width_m ;
	WORD display_pos_x_m ;

	right_bound = GetOUT_MainSizeHA() ;
	
	if ( channel == 1 ) // Main
	{			
		window_right_pos = stCASCADE.display_pos_x_m + stCASCADE.display_width_m;
		display_width_m = stCASCADE.display_width_m ;
		display_pos_x_m = stCASCADE.display_pos_x_m ;
	}
	else // Aux
	{
		window_right_pos = stCASCADE.display_pos_x_a + stCASCADE.display_width_a ;	
		display_width_m = stCASCADE.display_width_a ;
		display_pos_x_m = stCASCADE.display_pos_x_a ;
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
		stCASCADE.display_width_m = display_width_m ;
		CtrlApp_Cascade_Main() ;	
	}
	else // Aux
	{
		stCASCADE.display_width_a = display_width_m ;
		CtrlApp_Cascade_Aux() ;
	}		
}

void SetApp_Cascade_WindowStretchY(WORD channel, WORD direction)
{
	WORD display_height_m ;
	WORD display_pos_y_m ;
	WORD window_btm_pos ;
	WORD btm_bound ;
	
	btm_bound = GetOUT_MainSizeVA() ;

	if ( channel == 1 ) // Main
	{
		window_btm_pos = stCASCADE.display_pos_y_m + stCASCADE.display_height_m ;
		display_pos_y_m = stCASCADE.display_pos_y_m ;
		display_height_m = stCASCADE.display_height_m ;
	}
	else // Aux
	{
		window_btm_pos = stCASCADE.display_pos_y_a + stCASCADE.display_height_a ;	
		display_pos_y_m = stCASCADE.display_pos_y_a ;
		display_height_m = stCASCADE.display_height_a ;
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
		stCASCADE.display_height_m = display_height_m ;
		CtrlApp_Cascade_Main() ;
	}
	else // Aux
	{
		stCASCADE.display_height_a = display_height_m ;
		CtrlApp_Cascade_Aux() ;
	}		
}
//--------------------------------------------------------------------------------------------------------------------------
void GetApp_Cascade_info(void)
{
	UARTprintf("\r\n**Cascade parameter\r\n") ;
	UARTprintf("***Pano Mode : M [%d] , P [%d]\r\n",stCASCADE.display_mode,stCASCADE.display_position,stCASCADE.blend_left) ;
	UARTprintf("***BLND Area : L [%d] , R [%d]\r\n", stCASCADE.blend_left, stCASCADE.blend_right) ;
	UARTprintf("***WB offset1: R [%d] , B [%d]\r\n", stCASCADE.wb_gain_r_off_s1, stCASCADE.wb_gain_b_off_s1) ;
	UARTprintf("***WB offset2: R [%d] , B [%d]\r\n", stCASCADE.wb_gain_r_off_s2, stCASCADE.wb_gain_b_off_s2) ;
	UARTprintf("****Main\r\n") ;
	UARTprintf("[Input H:%d], [Input V:%d]\r\n", stCASCADE.in_size_h_m ,stCASCADE.in_size_v_m) ;		
	UARTprintf("[Source pos X:%d], [Source pos Y:%d]\r\n", stCASCADE.crop_pos_x_m ,stCASCADE.crop_pos_y_m) ;	
	UARTprintf("[Source width:%d], [Source height:%d]\r\n", stCASCADE.crop_width_m ,stCASCADE.crop_height_m) ;	
	
	UARTprintf("[Window pos X:%d], [Window pos Y:%d]\r\n", stCASCADE.display_pos_x_m ,stCASCADE.display_pos_y_m) ;	
	UARTprintf("[Window width:%d], [Window height:%d]\r\n\r\n", stCASCADE.display_width_m ,stCASCADE.display_height_m) ;		
	
	UARTprintf("****Aux\r\n") ;
	UARTprintf("[Input H:%d], [Input V:%d]\r\n", stCASCADE.in_size_h_a ,stCASCADE.in_size_v_a) ;		
	UARTprintf("[Source pos X:%d], [Source pos Y:%d]\r\n", stCASCADE.crop_pos_x_a ,stCASCADE.crop_pos_y_a) ;	
	UARTprintf("[Source width:%d], [Source height:%d]\r\n", stCASCADE.crop_width_a ,stCASCADE.crop_height_a) ;	
	
	UARTprintf("[Window pos X:%d], [Window pos Y:%d]\r\n", stCASCADE.display_pos_x_a ,stCASCADE.display_pos_y_a) ;	
	UARTprintf("[Window width:%d], [Window height:%d]\r\n\r\n", stCASCADE.display_width_a ,stCASCADE.display_height_a) ;		
}
//--------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
WORD SetApp_Cascade_SS01CtrlMode(WORD mode)
{
#if defined (__CASCADE_CAM_02__) || defined (__CASCADE_CAM_03__) 	
	if (mode > 4 ) 
	{
		if ( stCASCADE.sensor_mode_01 < 4 ) stCASCADE.sensor_mode_01 = stCASCADE.sensor_mode_01 + 1 ;
		else stCASCADE.sensor_mode_01 = 0 ;
	}
	else
	{
		stCASCADE.sensor_mode_01 = mode ;
	}	
	
	stCASCADE.sensor_mode_02 = 0 ;
	stCASCADE.sensor_mode_03 = 0 ;
	
#if defined (__CASCADE_SPI_MASTER__)
	SetApp_CASCADE_AuxCtrlMode(stCASCADE.sensor_mode_01) ;
#endif
	
	return stCASCADE.sensor_mode_01 ;
	
#else	// defined (__CASCADE_CAM_01__) 	
	return mode ;
#endif	
}
//--------------------------------------------------------------------------------------------------------------------------
WORD SetApp_Cascade_SS02CtrlMode(WORD mode)
{
#if defined (__CASCADE_CAM_02__) || defined (__CASCADE_CAM_03__) 	
	if (mode > 4 ) 
	{
		if ( stCASCADE.sensor_mode_02 < 4 ) stCASCADE.sensor_mode_02 = stCASCADE.sensor_mode_02 + 1 ;
		else stCASCADE.sensor_mode_02 = 0 ;
	}
	else
	{
		stCASCADE.sensor_mode_02 = mode ;
	}	

	stCASCADE.sensor_mode_01 = 0 ;
	stCASCADE.sensor_mode_03 = 0 ;	
	
#if defined (__CASCADE_SPI_MASTER__)	
	SetApp_Cascade_MainCtrlMode(stCASCADE.sensor_mode_02) ;
#endif	
	
	return stCASCADE.sensor_mode_02 ;
	
#else	// defined (__CASCADE_CAM_01__) 	
	return mode ;
#endif
	
}

//--------------------------------------------------------------------------------------------------------------------------
WORD SetApp_Cascade_SS03CtrlMode(WORD mode)
{
#if defined (__CASCADE_CAM_02__) || defined (__CASCADE_CAM_03__) 	
	if (mode > 4 ) 
	{
		if ( stCASCADE.sensor_mode_03 < 4 ) stCASCADE.sensor_mode_03 = stCASCADE.sensor_mode_03 + 1 ;
		else stCASCADE.sensor_mode_03 = 0 ;
	}
	else
	{
		stCASCADE.sensor_mode_03 = mode ;
	}	
	
	stCASCADE.sensor_mode_01 = 0 ;
	stCASCADE.sensor_mode_02 = 0 ;	
	
#if !defined (__CASCADE_SPI_MASTER__)	
	SetApp_Cascade_MainCtrlMode(stCASCADE.sensor_mode_03) ;
#endif	
	
	return stCASCADE.sensor_mode_03 ;
	
#else	// defined (__CASCADE_CAM_01__) 	
	return mode ;
#endif	
}
//--------------------------------------------------------------------------------------------------------------------------

WORD SetApp_Cascade_MainCtrlMode(WORD mode)
{
#if defined (__CASCADE_CAM_02__) 
	if (mode > 4 ) 
	{
		if ( stCASCADE.ctrl_m < 4 ) stCASCADE.ctrl_m = stCASCADE.ctrl_m + 1 ;
		else stCASCADE.ctrl_m = 0 ;
	}
	else
	{
		stCASCADE.ctrl_m = mode ;
	}
#elif defined (__CASCADE_CAM_03__)	
	if (mode > 4 ) 
	{
		if ( stCASCADE.ctrl_m < 4 ) stCASCADE.ctrl_m = stCASCADE.ctrl_m + 1 ;
		else stCASCADE.ctrl_m = 0 ;
	}
	else
	{
		stCASCADE.ctrl_m = mode ;
	}		
#endif
	stCASCADE.ctrl_a = 0 ; // one by one control
	
	return stCASCADE.ctrl_m ;
}

WORD SetApp_Cascade_AuxCtrlMode(WORD mode)
{
#if defined (__CASCADE_CAM_02__) 
	if (mode > 4 ) 
	{	
		if ( stCASCADE.ctrl_a < 4 ) stCASCADE.ctrl_a = stCASCADE.ctrl_a + 1 ;
		else stCASCADE.ctrl_a = 0 ;
	}
	else
	{
		stCASCADE.ctrl_a = mode ;
	}	
#elif defined (__CASCADE_CAM_03__)
	if (mode > 4 ) 
	{	
		if ( stCASCADE.ctrl_a < 4 ) stCASCADE.ctrl_a = stCASCADE.ctrl_a + 1 ;
		else stCASCADE.ctrl_a = 0 ;
	}
	else
	{
		stCASCADE.ctrl_a = mode ;
	}	
#endif	
	stCASCADE.ctrl_m = 0 ; // one by one control
		
	return stCASCADE.ctrl_a ;
}
//--------------------------------------------------------------------------------------------------------------------------

void SetApp_Cascade_Control( WORD direction )
{
#if defined (__CASCADE_CAM_02__) || defined (__CASCADE_CAM_03__) 	
	
	WORD control ;
	WORD channel ;
	WORD mode ;
	WORD blend_area ;
	WORD blend_lvl ;
	WORD position ;

#if defined (__CASCADE_SPI_MASTER__)		
	
	if ( stCASCADE.select_sensor == 1 || stCASCADE.ctrl_a > 0 )
	{
		channel = 0 ;					// aux
		control = stCASCADE.ctrl_a ;	
	}
	else if ( stCASCADE.select_sensor == 2  || stCASCADE.ctrl_m > 0 )
	{
		channel = 1 ;					// main
		control = stCASCADE.ctrl_m ;	
	}
	else if ( stCASCADE.select_sensor == 3 )
	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_CASCADE_CNTRL,direction); 
		channel = 0 ;
		control = 0 ;		
	}
	else
	{
		channel = 0 ;
		control = 0 ;		
	}

#else

	if ( stCASCADE.select_sensor == 3 || stCASCADE.ctrl_m > 0 )
	{
		channel = 1 ;					// main
		control = stCASCADE.ctrl_m ;		
	}	
	else if ( stCASCADE.ctrl_a > 0 )
	{
		channel = 0 ;					// aux
		control = stCASCADE.ctrl_a ;		
	}
	else
	{
		channel = 0 ;
		control = 0 ;		
	}		
	
#endif
		
	mode = stCASCADE.display_mode ;
	position = stCASCADE.display_position ;
	blend_area = stCASCADE.blend_left ;
	blend_lvl = stCASCADE.blend_l_lvl ;
		
	if ( stCASCADE.select_sensor == 0 ) 
	{		
		if ( direction == 1 ) 
		{
			control = 5 ;
			if ( mode > MISP_CASCADE_DISP_END-2 )	mode = 0 ;
			else								mode = mode + 1 ;
		}
		else 
		{
			control = 5 ;
			if ( position == 1 ) 	position = 0 ;
			else					position = 1 ;
		}
	}
	
	if ( stCASCADE.select_sensor == 4 ) 
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
		case 1 : SetApp_Cascade_CropLeftX(channel,direction) ; break ; // x left crop
		case 2 : SetApp_Cascade_CropRightX(channel,direction) ; break ; // x right crop
		case 3 : SetApp_Cascade_CropTopY(channel,direction) ; break ; // y top crop
		case 4 : SetApp_Cascade_CropBtmY(channel,direction) ; break ; // y btm crop
		case 5 : SetApp_Cascade_DisplayMode(mode,position) ; break ; // 
		case 6 : SetApp_Cascade_BlendLeftArea(blend_area) ; SetApp_Cascade_BlendRightArea(blend_area) ; break ; // 
		// case 6 : SPImAPP_CmdStd(SPIM_MISP_CID_S1, SPIM_CMD_IR_KEY, 0x16e9) ; break ; // for S1 SPIs test
		case 7 : SetApp_Cascade_BlendLevel(blend_lvl) ; break ; // 	
		// case 8 : break ; // 	
		default : break ;	// Do nothing
	}
	
#endif // #if defined (__CASCADE_CAM_02__) || defined (__CASCADE_CAM_03__) 	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetApp_CascadeSelectSensor(WORD sensor_num)
{
	stCASCADE.select_sensor = sensor_num ;	
	
	stCASCADE.ctrl_m = 0 ; // Reset internal ctrl mode
	stCASCADE.ctrl_a = 0 ;
}

WORD GetApp_CascadeSelectSensor(void)
{
	return stCASCADE.select_sensor ;
}
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
void IrCode_CascadeCtrlSelectSensor(void)
{
#if defined (__CASCADE_CAM_02__) || defined (__CASCADE_CAM_03__)	

	WORD sensor_num ;
	
	sensor_num = GetApp_CascadeSelectSensor() ;
	
	if ( sensor_num >= 4 ) 	sensor_num = 0 ;
	else 					sensor_num = sensor_num + 1 ;	
	
	SetApp_CascadeSelectSensor(sensor_num) ;
	
#if defined (__CASCADE_SPI_MASTER__) // Cmd to __CASCADE_CAM_03__	
	SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_CASCADE_SELSS,0); 
#else // Cmd Exe __CASCADE_CAM_03__
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

void IrCode_CascadeCtrlSelectAdjust(void)
{
#if defined (__CASCADE_CAM_02__) || defined (__CASCADE_CAM_03__)	
	
	WORD sensor_num ;
	WORD mode ;
	
	sensor_num = GetApp_CascadeSelectSensor() ;	
	
	if ( sensor_num == 1 ) // Aux input of S3 i550 => SS1
	{
		mode = SetApp_Cascade_SS01CtrlMode(9);
	}	
	else if ( sensor_num == 2 ) // Main input of S3 i550 => SS2
	{
		mode = SetApp_Cascade_SS02CtrlMode(9);
	}	
	else if ( sensor_num == 3 ) 
	{
		mode = SetApp_Cascade_SS03CtrlMode(9);
	}
	else
	{
		mode = 0 ;
	}

#if defined (__CASCADE_SPI_MASTER__) // Cmd to __CASCADE_CAM_03__		
	
	SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_CASCADE_SSADJ,mode); 

#else // Cmd exe __CASCADE_CAM_03__ [OSD display]
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
#endif // #if defined (__CASCADE_SPI_MASTER__) // Cmd to __CASCADE_CAM_03__		
	
#endif		
}

//--------------------------------------------------------------------------------------------------------------------------

void IrCode_CascadeCtrlMain(void)
{
	WORD mode ;
	mode = SetApp_Cascade_MainCtrlMode(9);

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

void IrCode_CascadeCtrlAux(void)
{
	WORD mode ;
	mode = SetApp_Cascade_AuxCtrlMode(9);
	
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
void IrCode_CascadeCtrlSelectSensor_MDINAPP(WORD sensor_num)
{
#if defined (__CASCADE_CAM_02__) || defined (__CASCADE_CAM_03__)	
	
	SetApp_CascadeSelectSensor(sensor_num) ;
	
#if defined (__CASCADE_SPI_MASTER__) // Cmd to __CASCADE_CAM_03__	
	SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_CASCADE_SS0 | sensor_num,0); 
#else // Cmd Exe __CASCADE_CAM_03__
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


void IrCode_Cascade_Control_MDINAPP(WORD val){
#if defined (__CASCADE_CAM_02__) || defined (__CASCADE_CAM_03__) 	
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

#if defined (__CASCADE_SPI_MASTER__)		
	
	if ( stCASCADE.select_sensor == 1 )
	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CASCADE_CRP_LX_U, val + 32); 
		channel = 0 ;					// aux
		control = ctrl ;	
	}
	else if ( stCASCADE.select_sensor == 2 )
	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CASCADE_CRP_LX_U, val + 16); 
		channel = 1 ;					// main
		control = ctrl ;	
	}
	else if ( stCASCADE.select_sensor == 3 )
	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CASCADE_CRP_LX_U, val); 
		channel = 0 ;
		control = 0 ;
	}
	else
	{
		channel = 0 ;
		control = 0 ;
	}

#else

	if ( stCASCADE.select_sensor == 3 )
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

	mode = stCASCADE.display_mode ;
	position = stCASCADE.display_position ;
	blend_area = stCASCADE.blend_left ;
	blend_lvl = stCASCADE.blend_l_lvl ;
		
	if ( stCASCADE.select_sensor == 0 && ctrl == 5) 
	{		
		if ( direction == 1 ) 
		{
			if ( mode > MISP_CASCADE_DISP_END-2 )	mode = 0 ;
			else								mode = mode + 1 ;
		}
		else 
		{
			if ( position == 1 ) 	position = 0 ;
			else					position = 1 ;
		}
		SetApp_Cascade_DisplayMode(mode,position) ;  
	}

	if ( stCASCADE.select_sensor == 4  && ctrl > 5) 
	{
		
		if ( direction == 1 ) 
		{

			// if ( blend_area > 15 )	blend_area = 0 ;
			if ( blend_area > 23 )	blend_area = 0 ;
			else 					blend_area = blend_area + 8 ; // must be even

			SetApp_Cascade_BlendLeftArea(blend_area) ; SetApp_Cascade_BlendRightArea(blend_area) ;  // 
		}
		else 
		{

			if ( blend_lvl > 14 )	blend_lvl = 0 ;
			else					blend_lvl = blend_lvl + 5 ;

			SetApp_Cascade_BlendLevel(blend_lvl) ;  // 	
		}
	}

	switch ( control )
	{
		case 1 : SetApp_Cascade_CropLeftX(channel,direction) ; 	break ; // x left crop
		case 2 : SetApp_Cascade_CropRightX(channel,direction) ;	break ; // x right crop
		case 3 : SetApp_Cascade_CropTopY(channel,direction) ; 		break ; // y top crop
		case 4 : SetApp_Cascade_CropBtmY(channel,direction) ; 		break ; // y btm crop
		default : break ;	// Do nothing
	}
#endif

}


//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Cascade_WriteEEPROM(void)
{
#if defined (__CASCADE_CAM_02__) || defined (__CASCADE_CAM_03__)	
	
//#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
    BYTE buff[21];
	BYTE buff_b[9];
	 
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = LOBYTE(stCASCADE.crop_pos_x_m);       buff[2] = HIBYTE(stCASCADE.crop_pos_x_m);
	buff[3] = LOBYTE(stCASCADE.crop_pos_y_m);       buff[4] = HIBYTE(stCASCADE.crop_pos_y_m);
	buff[5] = LOBYTE(stCASCADE.crop_width_m);       buff[6] = HIBYTE(stCASCADE.crop_width_m);
	buff[7] = LOBYTE(stCASCADE.crop_height_m);      buff[8] = HIBYTE(stCASCADE.crop_height_m);
	buff[9] = LOBYTE(stCASCADE.display_pos_x_m);    buff[10] = HIBYTE(stCASCADE.display_pos_x_m);
	buff[11] = LOBYTE(stCASCADE.display_pos_y_m);   buff[12] = HIBYTE(stCASCADE.display_pos_y_m);
	buff[13] = LOBYTE(stCASCADE.display_width_m);   buff[14] = HIBYTE(stCASCADE.display_width_m);
	buff[15] = LOBYTE(stCASCADE.display_height_m);  buff[16] = HIBYTE(stCASCADE.display_height_m);		
	buff[17] = LOBYTE(stCASCADE.display_mode); 	 buff[18] = HIBYTE(stCASCADE.display_mode);		
	buff[19] = LOBYTE(stCASCADE.display_position);  buff[20] = HIBYTE(stCASCADE.display_position);	
	
	EEPROM_MultiWrite(EEPROM_CASCADE_MAIN, (PBYTE)&buff[0], sizeof(buff));
	
	// buff[0] = EEPROM_SAVE_DONE;	
	buff[1] = LOBYTE(stCASCADE.crop_pos_x_a);       buff[2] = HIBYTE(stCASCADE.crop_pos_x_a);
	buff[3] = LOBYTE(stCASCADE.crop_pos_y_a);       buff[4] = HIBYTE(stCASCADE.crop_pos_y_a);
	buff[5] = LOBYTE(stCASCADE.crop_width_a);       buff[6] = HIBYTE(stCASCADE.crop_width_a);
	buff[7] = LOBYTE(stCASCADE.crop_height_a);      buff[8] = HIBYTE(stCASCADE.crop_height_a);
	buff[9] = LOBYTE(stCASCADE.display_pos_x_a);    buff[10] = HIBYTE(stCASCADE.display_pos_x_a);
	buff[11] = LOBYTE(stCASCADE.display_pos_y_a);   buff[12] = HIBYTE(stCASCADE.display_pos_y_a);
	buff[13] = LOBYTE(stCASCADE.display_width_a);   buff[14] = HIBYTE(stCASCADE.display_width_a);
	buff[15] = LOBYTE(stCASCADE.display_height_a);  buff[16] = HIBYTE(stCASCADE.display_height_a);	
	buff[17] = LOBYTE(stCASCADE.display_mode); 	 buff[18] = HIBYTE(stCASCADE.display_mode);		
	buff[19] = LOBYTE(stCASCADE.display_position);  buff[20] = HIBYTE(stCASCADE.display_position);	
	
	EEPROM_MultiWrite(EEPROM_CASCADE_AUX, (PBYTE)&buff[0], sizeof(buff));	
	
	buff_b[0] = EEPROM_SAVE_DONE;
	buff_b[1] = LOBYTE(stCASCADE.blend_left);       buff_b[2] = HIBYTE(stCASCADE.blend_left);
	buff_b[3] = LOBYTE(stCASCADE.blend_right);      buff_b[4] = HIBYTE(stCASCADE.blend_right);
	buff_b[5] = LOBYTE(stCASCADE.blend_l_lvl);      buff_b[6] = HIBYTE(stCASCADE.blend_l_lvl);
	buff_b[7] = LOBYTE(stCASCADE.blend_r_lvl);      buff_b[8] = HIBYTE(stCASCADE.blend_r_lvl);	
		
	EEPROM_MultiWrite(EEPROM_CASCADE_BLEND, (PBYTE)&buff_b[0], sizeof(buff_b));	

	UARTprintf("\r\n**Panorama setup write to EEPROM\r\n") ;
//#endif
#endif // #if defined (__CASCADE_CAM_02__) && defined (__CASCADE_CAM_03__)	
	
#if defined (__CASCADE_SPI_MASTER__)		
	SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_CASCADE_SAVE,0); // Cmd to __CASCADE_CAM_03__
#else
	OSD_IrkeyTitleDisplay(ON, "Save done ");
#endif	
	
}

void SetApp_Cascade_ReadEEPROM(void)
{
#if defined (__CASCADE_CAM_02__) || defined (__CASCADE_CAM_03__)	
	
//#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
    BYTE buff[21];
	BYTE buff_b[9];
           
	EEPROM_MultiRead(EEPROM_CASCADE_MAIN, (PBYTE)&buff[0], sizeof(buff));

	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.crop_pos_x_m = MAKEWORD(buff[2], buff[1]);
	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.crop_pos_y_m = MAKEWORD(buff[4], buff[3]);
	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.crop_width_m = MAKEWORD(buff[6], buff[5]);
	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.crop_height_m = MAKEWORD(buff[8], buff[7]);
	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.display_pos_x_m = MAKEWORD(buff[10], buff[9]);
	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.display_pos_y_m = MAKEWORD(buff[12], buff[11]);
	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.display_width_m = MAKEWORD(buff[14], buff[13]);
	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.display_height_m = MAKEWORD(buff[16], buff[15]);
	
	EEPROM_MultiRead(EEPROM_CASCADE_AUX, (PBYTE)&buff[0], sizeof(buff));

	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.crop_pos_x_a = MAKEWORD(buff[2], buff[1]);
	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.crop_pos_y_a = MAKEWORD(buff[4], buff[3]);
	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.crop_width_a = MAKEWORD(buff[6], buff[5]);
	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.crop_height_a = MAKEWORD(buff[8], buff[7]);
	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.display_pos_x_a = MAKEWORD(buff[10], buff[9]);
	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.display_pos_y_a = MAKEWORD(buff[12], buff[11]);
	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.display_width_a = MAKEWORD(buff[14], buff[13]);
	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.display_height_a = MAKEWORD(buff[16], buff[15]);	
	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.display_mode = MAKEWORD(buff[18], buff[17]);	
	if(buff[0] == EEPROM_SAVE_DONE)           stCASCADE.display_position = MAKEWORD(buff[20], buff[19]);	
	
	EEPROM_MultiRead(EEPROM_CASCADE_BLEND, (PBYTE)&buff_b[0], sizeof(buff_b));
	
	if(buff_b[0] == EEPROM_SAVE_DONE)           stCASCADE.blend_left = MAKEWORD(buff_b[2], buff_b[1]);
	if(buff_b[0] == EEPROM_SAVE_DONE)           stCASCADE.blend_right = MAKEWORD(buff_b[4], buff_b[3]);
	if(buff_b[0] == EEPROM_SAVE_DONE)           stCASCADE.blend_l_lvl = MAKEWORD(buff_b[6], buff_b[5]);
	if(buff_b[0] == EEPROM_SAVE_DONE)           stCASCADE.blend_r_lvl = MAKEWORD(buff_b[8], buff_b[7]);	
	
	EEPROM_MultiRead(EEPROM_CASCADE_WB, (PBYTE)&buff_b[0], sizeof(buff_b));
	
	if(buff_b[0] == EEPROM_SAVE_DONE)           stCASCADE.wb_gain_r_off_s1 = (SHORT)MAKEWORD(buff_b[2], buff_b[1]);
	if(buff_b[0] == EEPROM_SAVE_DONE)           stCASCADE.wb_gain_b_off_s1 = (SHORT)MAKEWORD(buff_b[4], buff_b[3]);
	if(buff_b[0] == EEPROM_SAVE_DONE)           stCASCADE.wb_gain_r_off_s2 = (SHORT)MAKEWORD(buff_b[6], buff_b[5]);
	if(buff_b[0] == EEPROM_SAVE_DONE)           stCASCADE.wb_gain_b_off_s2 = (SHORT)MAKEWORD(buff_b[8], buff_b[7]);

	if(buff[0] == EEPROM_SAVE_DONE)  	UARTprintf("\r\n**Panorama setup from EEPROM\r\n") ;	
	
	GetApp_Cascade_info() ;
	
//#endif
	
#endif // #if defined (__CASCADE_CAM_02__) && defined (__CASCADE_CAM_03__)	
}

void SetApp_Cascade_ResetEEPROM(void)
{
#if defined (__CASCADE_CAM_02__) || defined (__CASCADE_CAM_03__)	
	
//#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)	
	BYTE buff[2];
	
	buff[0] = EEPROM_RegRead(EEPROM_CASCADE_MAIN) ;
	
	if (buff[0] == EEPROM_SAVE_DONE) 	
	{
		buff[1] = 0x00;	
		EEPROM_RegWrite(EEPROM_CASCADE_MAIN, (WORD)buff[1]);
		EEPROM_RegWrite(EEPROM_CASCADE_AUX, (WORD)buff[1]);		
		EEPROM_RegWrite(EEPROM_CASCADE_BLEND, (WORD)buff[1]);	
		EEPROM_RegWrite(EEPROM_CASCADE_WB, (WORD)buff[1]);
		
#if defined (__CASCADE_SPI_MASTER__)		
	SPImAPP_CmdStd(SPIM_MISP_CID_S2,SPIM_CMD_CASCADE_DEL,0); // Cmd to __CASCADE_CAM_03__
#else
	OSD_IrkeyTitleDisplay(ON, "DeleteDone");
#endif		
				
	}
	else								
	{
		SetApp_Cascade_WriteEEPROM();
	}	

//#endif
	UARTprintf("\r\n**Panorama Reset EEPROM\r\n") ;
#endif // #if defined (__CASCADE_CAM_02__) && defined (__CASCADE_CAM_03__)		
}

void SetApp_Cascade_Menu_WriteEEPROM(void)
{
#if defined (__CASCADE_CAM_02__)
//#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)	
	BYTE buff_b[9];
	
	buff_b[0] = EEPROM_SAVE_DONE;
	buff_b[1] = LOBYTE(stCASCADE.wb_gain_r_off_s1);      buff_b[2] = HIBYTE(stCASCADE.wb_gain_r_off_s1);
	buff_b[3] = LOBYTE(stCASCADE.wb_gain_b_off_s1);      buff_b[4] = HIBYTE(stCASCADE.wb_gain_b_off_s1);
	buff_b[5] = LOBYTE(stCASCADE.wb_gain_r_off_s2);      buff_b[6] = HIBYTE(stCASCADE.wb_gain_r_off_s2);
	buff_b[7] = LOBYTE(stCASCADE.wb_gain_b_off_s2);      buff_b[8] = HIBYTE(stCASCADE.wb_gain_b_off_s2);	
	
	EEPROM_MultiWrite(EEPROM_CASCADE_WB, (PBYTE)&buff_b[0], sizeof(buff_b));
//#endif
#endif // #if defined (__CASCADE_CAM_02__)
}

#if defined (__CASCADE_CAM_02__)
static void SetApp_Cascade_WB_GainR_offset(WORD cID, WORD val)
{
	if(cID==SPIM_MISP_CID_S1)		stCASCADE.wb_gain_r_off_s1 = (val < 125)?  -(125-val):  val-125;	// -125 ~ +125
	if(cID==SPIM_MISP_CID_S2)		stCASCADE.wb_gain_r_off_s2 = (val < 125)?  -(125-val):  val-125;	// -125 ~ +125
}
#endif

#if defined (__CASCADE_CAM_02__)
static void SetApp_Cascade_WB_GainB_offset(WORD cID, WORD val)
{
	if(cID==SPIM_MISP_CID_S1)		stCASCADE.wb_gain_b_off_s1 = (val < 125)?  -(125-val):  val-125;	// -125 ~ +125
	if(cID==SPIM_MISP_CID_S2)		stCASCADE.wb_gain_b_off_s2 = (val < 125)?  -(125-val):  val-125;	// -125 ~ +125
}
#endif

WORD ConvApp_Cascade_WB_GainR(WORD cID, SHORT gain)
{
#if defined (__CASCADE_CAM_02__)
	if(cID==SPIM_MISP_CID_S1)		return  (gain + stCASCADE.wb_gain_r_off_s1 > 0)?  gain + stCASCADE.wb_gain_r_off_s1:  0;
	if(cID==SPIM_MISP_CID_S2)		return  (gain + stCASCADE.wb_gain_r_off_s2 > 0)?  gain + stCASCADE.wb_gain_r_off_s2:  0;
#endif
	
	return  0;
}

WORD ConvApp_Cascade_WB_GainB(WORD cID, SHORT gain)
{
#if defined (__CASCADE_CAM_02__)
	if(cID==SPIM_MISP_CID_S1)		return  (gain + stCASCADE.wb_gain_b_off_s1 > 0)?  gain + stCASCADE.wb_gain_b_off_s1:  0;
	if(cID==SPIM_MISP_CID_S2)		return  (gain + stCASCADE.wb_gain_b_off_s2 > 0)?  gain + stCASCADE.wb_gain_b_off_s2:  0;
#endif
	
	return  0;
}

void SetAPP_Cascade_Menu_Ctrl(WORD val)
{
#if defined (__CASCADE_CAM_02__)
	stCASCADE.menu_ctrl_mode = val;
#endif
}

void SetAPP_Cascade_Menu_Param(WORD val)
{
#if defined (__CASCADE_CAM_02__)
	switch(stCASCADE.menu_ctrl_mode)	{
		case  0:	return;
		case  1:	SetApp_Cascade_WB_GainR_offset(SPIM_MISP_CID_S1, val);		break;	//gain-r offset s1
		case  2:	SetApp_Cascade_WB_GainB_offset(SPIM_MISP_CID_S1, val);		break;	//gain-b offset s1
		case  3:	SetApp_Cascade_WB_GainR_offset(SPIM_MISP_CID_S2, val);		break;	//gain-r offset s2
		case  4:	SetApp_Cascade_WB_GainB_offset(SPIM_MISP_CID_S2, val);		break;	//gain-b offset s2
	}
	
	if(stCASCADE.menu_ctrl_mode != 0)		SetApp_Cascade_Menu_WriteEEPROM();
#endif
}

WORD GetAPP_Cascade_Menu_Param(void)
{
#if defined (__CASCADE_CAM_02__)
	switch(stCASCADE.menu_ctrl_mode)	{
		case  0:	return  125;
		case  1:	return  ConvApp_Cascade_WB_GainR(SPIM_MISP_CID_S1, 125);	//gain-r offset s1
		case  2:	return  ConvApp_Cascade_WB_GainB(SPIM_MISP_CID_S1, 125);	//gain-b offset s1
		case  3:	return  ConvApp_Cascade_WB_GainR(SPIM_MISP_CID_S2, 125);	//gain-r offset s2
		case  4:	return  ConvApp_Cascade_WB_GainB(SPIM_MISP_CID_S2, 125);	//gain-b offset s2
	}
#endif
	
	return  0;
}





//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------


#endif	//__CASCADE_CAM_SETUP__

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
/*  FILE_END_HERE */
