

#ifndef		__MISP_PANO_H__
#define		__MISP_PANO_H__

//////////////////////////////////////////////////////////////////////////
// For __PANORAMA_01__ , __PANORAMA_03__
// Enable __USE_CPU_SPI_SLAVE__ at 'board.h' 
// For __PANORAMA_02__
// Disable __USE_CPU_SPI_SLAVE__ at 'board.h' 
//////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------
// define
// ----------------------------------------------------------------------


#if defined(__PANORAMA_SETUP__)

//
//	__PANORAMA_01__ is "__MDIN_i510_IMX290__" on panorama one board
//
//#define __PANORAMA_01__ // __PANORAMA_01__ : Sen FHD30 main / ExtIN none/ OutPut FHD30
//// Enable __USE_CPU_SPI_SLAVE__ at 'board.h' 
//
//	__PANORAMA_01__ is "__MDIN_i510_IMX290__" on panorama one board
//

#define __PANORAMA_02__ // __PANORAMA_02__ : Sen FHD30 aux/ ExtIN FHD30 from __PANORAMA_01__ main/ OutPut UHD30 => 
//// Disable __USE_CPU_SPI_SLAVE__ at 'board.h' 

//#define __PANORAMA_03__ // __PANORAMA_03__ : Sen FHD30 / ExtIN UHD30 from __PANORAMA_02__ / OutPut UHD30 =>
//// Enable __USE_CPU_SPI_SLAVE__ at 'board.h' 


#if defined(__PANORAMA_02__)
#define __PANO_SPI_MASTER__ // include SPI master control fuctions
#ifdef __USE_CPU_SPI_SLAVE__
#undef __USE_CPU_SPI_SLAVE__
#endif
#endif // #if defined(__PANORAMA_02__)

#if defined(__PANORAMA_01__)||defined(__PANORAMA_03__)
#ifndef __USE_CPU_SPI_SLAVE__
#define __USE_CPU_SPI_SLAVE__
#endif
#endif // #if defined(__PANORAMA_01__)||defined(__PANORAMA_03__)

#endif // #if defined(__PANORAMA_SETUP__)

// MN34220 OSC speed
#ifndef __CMOS_OSC_37125__
#define __CMOS_OSC_37125__
#endif

// #ifdef __USE_EEPROM__
// #undef __USE_EEPROM__ // EEPROM disable
// #endif

// #ifdef __USE_HDMI_TX__
// #undef __USE_HDMI_TX__ // HDMI_TX disable
// #endif


// ----------------------------------------------------------------------
// Struct/Union Types
// ----------------------------------------------------------------------
typedef	enum {
	MISP_PANO_DISP_MODE_0 = 0,			// 1280x720 + 1280x720 + 1280x720 ==> => All sensor HV scale down
	MISP_PANO_DISP_MODE_1 ,				// 960x1080 + 1920x1080 + 960x1080 => Left, Right sensor H scale down
	MISP_PANO_DISP_MODE_2 ,				// 1280x1080 + 1280x1080 + 1280x1080 => All sensor H scale down
	MISP_PANO_DISP_MODE_3 ,				// 960x1080 + 960x1080 + 960x1080 =>  Left, Right sensor H crop
	MISP_PANO_DISP_END ,				// index end 
}	MISP_PANO_DISP_TYPE_t;

typedef	struct
{
	
	WORD display_mode ;		// display_mode [0: Scale down Full display] [1: Full crop display]
	WORD display_position ;	// [0: Top ] [1: Center ]
	WORD blend_left ;		// left blend area
	WORD blend_right ;		// right blend area
	WORD blend_l_lvl ;		// [0: aux ] [15: main ]
	WORD blend_r_lvl ;		// [0: aux ] [15: main ]
	
	WORD select_sensor ;	
	WORD sensor_mode_01 ;
	WORD sensor_mode_02 ;
	WORD sensor_mode_03 ;	
	
	WORD ctrl_m ;			// Main ctrl
	WORD in_size_h_m ;		// Main inputput h size
	WORD in_size_v_m ;		// Main inputput v size
	
	WORD crop_pos_x_m ;		// Main output source param
	WORD crop_pos_y_m ;		// Main output source param
	WORD crop_width_m ;		// Main output source param
	WORD crop_height_m ;	// Main output source param		
	
	WORD display_pos_x_m ;  // Main Output Window param
	WORD display_pos_y_m ;  // Main Output Window param
	WORD display_width_m ;  // Main Output Window param
	WORD display_height_m ; // Main Output Window param 
		
	WORD ctrl_a ;			// Aux ctrl
	WORD in_size_h_a ;		// Aux inputput h size
	WORD in_size_v_a ;		// Aux inputput v size
	
	WORD crop_pos_x_a ;		// Aux output source param
	WORD crop_pos_y_a ;		// Aux output source param
	WORD crop_width_a ;		// Aux output source param
	WORD crop_height_a ;	// Aux output source param		
	
	WORD display_pos_x_a ;	// Aux Output Window param
	WORD display_pos_y_a ;	// Aux Output Window param
	WORD display_width_a ;	// Aux Output Window param
	WORD display_height_a ;	// Aux Output Window param	
	
	SHORT wb_gain_r_off_s1;	// wb gain-r offset for s1
	SHORT wb_gain_b_off_s1;	// wb gain-b offset for s1
	SHORT wb_gain_r_off_s2;	// wb gain-r offset for s2
	SHORT wb_gain_b_off_s2;	// wb gain-b offset for s2
	WORD menu_ctrl_mode;	// menu control mode
	WORD dummy001;
} MISP_PANO_PARAM, *PMISP_PANO_PARAM;

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------
void SetApp_Init_Pano(void) ;

// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------

void SetApp_Pano_Control(WORD mode) ;

WORD SetApp_Pano_SS01CtrlMode(WORD mode);
WORD SetApp_Pano_SS02CtrlMode(WORD mode);
WORD SetApp_Pano_SS03CtrlMode(WORD mode);

WORD SetApp_Pano_MainCtrlMode(WORD mode) ;
WORD SetApp_Pano_AuxCtrlMode(WORD mode) ;
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Pano_DisplayMode(WORD mode, WORD position) ;
void SetApp_Pano_BlendLeftArea(WORD area) ;
void SetApp_Pano_BlendRightArea(WORD area) ;
void SetApp_Pano_BlendLevel(WORD level) ;

//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Pano_CropLeftX(WORD channel, WORD direction) ;
void SetApp_Pano_CropRightX(WORD channel, WORD direction) ;
void SetApp_Pano_CropTopY(WORD channel,WORD direction) ;
void SetApp_Pano_CropBtmY(WORD channel, WORD direction) ;

void SetApp_Pano_WindowMoveX(WORD channel, WORD direction) ;
void SetApp_Pano_WindowMoveY(WORD channel, WORD direction) ;
void SetApp_Pano_WindowStretchX(WORD channel,WORD direction) ;
void SetApp_Pano_WindowStretchY(WORD channel, WORD direction) ;

void GetApp_Pano_info(void);
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_PanoSelectSensor(WORD sensor_num) ;
WORD GetApp_PanoSelectSensor(void) ;

//--------------------------------------------------------------------------------------------------------------------------
// For IR ctrl
void IrCode_PanoCtrlMain(void);
void IrCode_PanoCtrlAux(void);

void IrCode_PanoCtrlSelectSensor(void);
void IrCode_PanoCtrlSelectAdjust(void);

// For MDIN APP
void IrCode_PanoCtrlSelectSensor_MDINAPP(WORD sensor_num);
void IrCode_Pano_Control_MDINAPP(WORD val);

// For EEPROM R/W
void SetApp_Pano_WriteEEPROM(void) ;
void SetApp_Pano_ReadEEPROM(void) ;
void SetApp_Pano_ResetEEPROM(void);
void SetApp_Pano_WB_WriteEEPROM(void);

// For wb ctrl
WORD ConvApp_Pano_WB_GainR(WORD cID, SHORT gain);
WORD ConvApp_Pano_WB_GainB(WORD cID, SHORT gain);
void SetAPP_Pano_Menu_Ctrl(WORD val);
void SetAPP_Pano_Menu_Param(WORD val);
WORD GetAPP_Pano_Menu_Param(void);
//--------------------------------------------------------------------------------------------------------------------------
#endif 


/*  FILE_END_HERE */
