

#ifndef		__MISP_CASCADE_CAM_H__
#define		__MISP_CASCADE_CAM_H__


// ----------------------------------------------------------------------
// define
// ----------------------------------------------------------------------


#if defined(__CASCADE_CAM_SETUP__)

//#define	__CASCADE_CAM_01__

//#define __CASCADE_CAM_02__ // __CASCADE_CAM_02__ : Sen FHD30 aux/ ExtIN FHD30 from __CASCADE_CAM_01__ main/ OutPut UHD30 => 

#define __CASCADE_CAM_03__ // __CASCADE_CAM_03__ : Sen FHD30 / ExtIN UHD30 from __CASCADE_CAM_02__ / OutPut UHD30 =>


/*
//---Activation is possible only if SPI is physically connected--//
#if defined(__CASCADE_CAM_02__)
#define __CASCADE_SPI_MASTER__ // include SPI master control fuctions
#ifdef __USE_CPU_SPI_SLAVE__
#undef __USE_CPU_SPI_SLAVE__
#endif
#endif // #if defined(__CASCADE_CAM_02__)

#if defined(__CASCADE_CAM_01__)||defined(__CASCADE_CAM_03__)
#ifndef __USE_CPU_SPI_SLAVE__
#define __USE_CPU_SPI_SLAVE__
#endif
#endif // #if defined(__CASCADE_CAM_01__)||defined(__CASCADE_CAM_03__)

#endif // #if defined(__CASCADE_CAM_SETUP__)
*/

// #ifdef __USE_EEPROM__
// #undef __USE_EEPROM__ // EEPROM disable
// #endif



// ----------------------------------------------------------------------
// Struct/Union Types
// ----------------------------------------------------------------------
typedef	enum {
	MISP_CASCADE_DISP_MODE_0 = 0,			// 1280x720 + 1280x720 + 1280x720 ==> => All sensor HV scale down
	MISP_CASCADE_DISP_MODE_1 ,				// 960x1080 + 1920x1080 + 960x1080 => Left, Right sensor H scale down
	MISP_CASCADE_DISP_MODE_2 ,				// 1280x1080 + 1280x1080 + 1280x1080 => All sensor H scale down
	MISP_CASCADE_DISP_MODE_3 ,				// 960x1080 + 960x1080 + 960x1080 =>  Left, Right sensor H crop
	MISP_CASCADE_DISP_END ,				// index end 
}	MISP_CASCADE_DISP_TYPE_t;

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
} MISP_CASCADE_PARAM, *PMISP_CASCADE_PARAM;

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
void SetApp_Init_Cascade_Cam(void) ;

// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------

void SetApp_Cascade_Control(WORD mode) ;

WORD SetApp_Cascade_SS01CtrlMode(WORD mode);
WORD SetApp_Cascade_SS02CtrlMode(WORD mode);
WORD SetApp_Cascade_SS03CtrlMode(WORD mode);

WORD SetApp_Cascade_MainCtrlMode(WORD mode) ;
WORD SetApp_Cascade_AuxCtrlMode(WORD mode) ;
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_Cascade_DisplayMode(WORD mode, WORD position) ;
void SetApp_Cascade_BlendLeftArea(WORD area) ;
void SetApp_Cascade_BlendRightArea(WORD area) ;
void SetApp_Cascade_BlendLevel(WORD level) ;

void SetApp_Cascade_WindowMoveX(WORD channel, WORD direction) ;
void SetApp_Cascade_WindowMoveY(WORD channel, WORD direction) ;
void SetApp_Cascade_WindowStretchX(WORD channel,WORD direction) ;
void SetApp_Cascade_WindowStretchY(WORD channel, WORD direction) ;

//--------------------------------------------------------------------------------------------------------------------------
void SetApp_CascadeSelectSensor(WORD sensor_num) ;
WORD GetApp_CascadeSelectSensor(void) ;

//--------------------------------------------------------------------------------------------------------------------------
// For IR ctrl
void IrCode_CascadeCtrlMain(void);
void IrCode_CascadeCtrlAux(void);

void IrCode_CascadeCtrlSelectSensor(void);
void IrCode_CascadeCtrlSelectAdjust(void);

// For MDIN APP
void IrCode_CascadeCtrlSelectSensor_MDINAPP(WORD sensor_num);
void IrCode_Cascade_Control_MDINAPP(WORD val);

// For EEPROM R/W
void SetApp_Cascade_WriteEEPROM(void) ;
void SetApp_Cascade_ReadEEPROM(void) ;
void SetApp_Cascade_ResetEEPROM(void);
void SetApp_Cascade_WB_WriteEEPROM(void);

// For wb ctrl
WORD ConvApp_Cascade_WB_GainR(WORD cID, SHORT gain);
WORD ConvApp_Cascade_WB_GainB(WORD cID, SHORT gain);
void SetAPP_Cascade_Menu_Ctrl(WORD val);
void SetAPP_Cascade_Menu_Param(WORD val);
WORD GetAPP_Cascade_Menu_Param(void);
#endif 
#endif 

/*  FILE_END_HERE */
