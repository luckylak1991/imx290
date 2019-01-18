//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MENU.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: MDIN380 DDK v2.52
// Revision History 	:

#ifndef		__MENU_H__
#define		__MENU_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__CMOS_IF_H__
#include	"cmos_if.h"		// for sensor dependent option
#endif

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

//configure supported sensor FPS (default, support [25,30]Hz)
#if defined(__USE_CMOS_MN34220__)||\
    defined(__USE_CMOS_MN34420__)||\
    defined(__USE_CMOS_IMXx36__)||\
    defined(__USE_IMX290_1080P60_MODE__)||\
    defined(__USE_IMX290_720P60_MODE__)||\
    defined(__USE_IMX385_1080P60_MODE__)||\
    defined(__USE_CMOS_IMX174__)||\
    defined(__USE_IMX274_1080P60_MODE__)||\
    defined(__USE_CMOS_IMX252__)||\
    defined(__USE_CMOS_IMX253__)||\
    defined(__USE_IMX265_1080P60_MODE__)||defined(__USE_IMX265_1080P30_MODE__)||defined(__USE_IMX265_ROI_MODE__)||\
    defined(__USE_CMOS_AR0134__)||\
    defined(__USE_CMOS_AR0135__)||\
    defined(__USE_CMOS_IMX273__)&&!(defined(__USE_IMX273_720P30_MODE__)||defined(__USE_IMX273_720P120_MODE__)||defined(__USE_IMX273_720P240_MODE__))||\
    defined(__USE_IMX226_1080P60_MODE__)||\
    defined(__USE_IMX226_2160P60_MODE4__)||\
	defined(__USE_IMX274_2160P60_MODE1__)
#define	__USE_SENS_60Hz_MENU__  //support [25,30,50,60]Hz
#elif defined(__USE_IMX265_1536P_ALL_SCAN_MODE__)
#define	__USE_SENS_50Hz_MENU__  //support [25,30,50]Hz
#elif defined(__USE_IMX273_720P240_MODE__)
#define	__USE_SENS_240Hz_MENU__  //support [25,30,50,60,100,120,200,240]Hz
#elif defined(__USE_IMX290_1080P120_MODE__)||\
	defined(__USE_IMX290_720P120_MODE__)||\
	defined(__USE_IMX290_720P120_ISPCROP__)||\
	defined(__USE_IMX273_720P120_MODE__)
#define	__USE_SENS_120Hz_MENU__  //support [25,30,50,60,100,120]Hz
#elif\
    (defined(__USE_IMX226_2160P30_MODE0__)||\
    defined(__USE_IMX226_2160P30_MODE1__))&&\
	!defined(__USE_MDIN_i5XX_REV__)
#define	__USE_SENS_20Hz_MENU__  //support [20,24]Hz
#endif

// enable wdr sub options
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)||\
    defined(__USE_CMOS_IMXx36__)
#define	__USE_WDR_EXT_MENU__
#endif

typedef	struct
{
#if defined(__USE_CHN_FONT__)
	PWORD		pMenu;			// menu text
	BYTE		Attb;			// menu attribute(item, value, func, text, enter, exit,,)
	PWORD		pItem;			// item text
	BYTE		Init;			// initial value(index) of menu item, defined 'osdmenu.h'(or EEPROM)
	BYTE		Max;			// menu item total q'ty(item:q'ty, value:max value)
	PWORD		pTitle;			// menu title text
#else
	PCHAR		pMenu;			// menu text
	BYTE		Attb;			// menu attribute(item, value, func, text, enter, exit,,)
	PCHAR		pItem;			// item text
	BYTE		Init;			// initial value(index) of menu item, defined 'osdmenu.h'(or EEPROM)
	BYTE		Max;			// menu item total q'ty(item:q'ty, value:max value)
	PCHAR		pTitle;			// menu title text
#endif
	
	BYTE		MenuRow;		// menu total row(except title)
	WORD		cID;			// OSDCombID
	BYTE		pos_y;			// size/positioin(Y) value
	BYTE		pos_y_max;		// size/positioin(Y) max

}	stPACKED OSD_MENU_INFO, *POSD_MENU_INFO;


// define osd menu index for external use
// Level 1
#define		WB_MENU					0x3000		// 3. white balance
#define		DNR_MENU				0x5000		// 5. dnr

// Level 2
#define 	FCS_ZSTART_MENU			0x1200		// 1-2. zoom start
#define 	FCS_ZSTOP_MENU			0x1300		// 1-3. zoom stop
#define 	FCS_ONEPUSH_MENU		0x1300		// 1-3. zoom two motor lens one-push
#define 	FCS_INIT_MENU			0x1500		// 1-5. zoom two motor lens initial
#define 	FCS_AFINTVAL_MENU		0x1600		// 1-6. af interval
#define 	EXP_MODE_MENU			0x2100		// 2-1. ae mode
#define 	EXP_SHUT_MENU			0x2200		// 2-2. shutter speed
#define 	EXP_IRIS_MENU			0x2300		// 2-3. iris
#define 	EXP_AGC_MENU			0x2400		// 2-4. agc
#define 	EXP_AGCMAX_MENU			0x2500		// 2-5. agc max
#define 	EXP_SENSUP_MENU			0x2600		// 2-6. sens-up
#define 	EXP_BRIGHT_MENU			0x2700		// 2-7. exp(brightness)
#define 	WB_PUSH_MENU			0x3200		// 3-2. wb(one push)
#define 	WB_BLUE_MENU			0x3300		// 3-3. wb b-gain
#define 	WB_RED_MENU				0x3400		// 3-4. wb r-gain
#define 	DANT_DELAY_MENU			0x4200		// 4-2. daynight(delay)
#define 	DANT_AGCD2N_MENU		0x4300		// 4-3. daynight(day to night level)
#define 	DANT_AGCN2D_MENU		0x4400		// 4-4. daynight(night to day level)
#define		DNR_MODE_MENU			0x5100		// 5-1. dnr mode
#define		DNR_LEVEL_MENU			0x5200		// 5-2. dnr level
#define		PM_MODE_MENU			0x6100		// 6-1. pm mode
#define   	PM_TYPE_MENU			0x6200		// 6-2. pm type
#define		PM_MASKNO_MENU			0x6300		// 6-3. pm mask no.
#define		PM_DISP_MENU			0x6400		// 6-4. pm mask display
#define		PM_COLOR_MENU			0x6500		// 6-5. pm color
#define		PM_POLYSEL_MENU			0x6600		// 6-6. pm poly select
#define		PM_POLYX_MENU			0x6700		// 6-7. pm poly x position
#define		PM_POLYY_MENU			0x6800		// 6-8. pm poly y position
#define		PM_SQRX_MENU			0x6700		// 6-7. pm position(x)
#define		PM_SQRY_MENU			0x6800		// 6-8. pm position(y)
#define		PM_SQRW_MENU			0x6900		// 6-9. pm size(x)
#define		PM_SQRH_MENU			0x6A00		// 6-10. pm size(y)
#define 	PM_RETURN_MENU			0x6C00		// 6-12. pm return
#define		LSC_MENU				0x7100		// 7-1. lens shading correction
#define		GM_MENU					0x7200		// 7-2. gamma control
#define		CONTRAST_MENU			0x7300		// 7-3. contrast
#define		BRIGHTNESS_MENU			0x7400		// 7-4. bright
#define		SATURATION_MENU			0x7500		// 7-5. saturation
#define		HUE_MENU				0x7600		// 7-6. hue enhance
#define		EDGE_MENU				0x7700		// 7-7. edge enhance
#define		AUTO_SAT_MENU			0x7800		// 7-8. auto saturation menu
#define		AUTO_EDGE_MENU			0x7900		// 7-9. auto edge menu
#define		EFFECTDEFAULT_MENU		0x7A00		// 7-10. effect default
#define		DZMODE_MENU				0x8100		// 8-1. dzoom mode
#define		DEFOG_MENU				0x8200		// 8-2. defog
#define		DWDR_MENU				0x8300		// 8-3. dwdr
#define		MD_MENU					0x8400		// 8-4. md
#define 	DIS_MENU				0x8500		// 8-5. dis
#define		BLIGHT_MENU				0x8600		// 8-6. backlight
#define		DPC_MENU				0x8800		// 8-8. DPC
#define		WDR_MENU				0x8A00		// 8-10. wdr
#define		CAMTITLE_MENU			0x9100		// 9-1. cam title
#define		LANG_MENU				0x9400		// 9-4. menu language
#define		MENU_POSITION_MENU		0x9800		// 9-8. menu position
#define		OUTFRMT_MENU			0x9900		// 9-9. output format
#if defined(__USE_IR_LED_PWM__)
#define		LEDPWMLOW_MENU			0x9A00		// 9-10. led pwm low pulse width
#define		LEDPWMHIGH_MENU			0x9B00		// 9-11. led pwm high pulse width
#endif
#define		INFRMT_MENU				0xA200		// 10-2. input format
#define		SENSOR_FPS_MENU			0xA800		// 10-6-4. sensor tp default
#define		WDR_TUNE_MENU			0xAA00		// 10-10. wdr tuning
#define		DEBUGMODE_MENU			0xC100		// 12-1. debug mode


// Level 3
#define 	FCS_CONFWINW_MENU		0x1720		// 1-7-2. conference mode win width
#define 	FCS_CONFWINH_MENU		0x1730		// 1-7-3. conference mode win height
#define 	FCS_CONFWINX_MENU		0x1740		// 1-7-4. conference mode win center x-position
#define 	FCS_CONFWINY_MENU		0x1750		// 1-7-5. conference mode win center y-position
#define 	FCS_CONFDEFAULT_MENU	0x1760		// 1-7-6. conference mode win default menu
#define		PIPMODE_MENU			0x8120		// 8-1-2. pip mode
#define		PIPSIZE_MENU			0x8130		// 8-1-3. pip size
#define		DZSAVE_MENU				0x8150		// 8-1-5. dzoom default
#define		DZDEFAULT_MENU			0x8160		// 8-1-6. dzoom default
#define 	DEFOG_SENS_MENU			0x8210		// 8-2-1. defog sensitive
#define 	DEFOG_BRIGHT_MENU		0x8220		// 8-2-2. defog bright
#define		DEFOG_DEMOSCR_MENU		0x8230		// 8-2-3. defog demo mode
#define 	DWDR_SENS_MENU			0x8310		// 8-3-1. dwdr strength
#define 	DWDR_SAT_MENUb			0x8320		// 8-3-2. dwdr saturation
#define		DWDR_LRATIO_MENU		0x8330		// 8-3-3. dwdr local ratio
#define		DWDR_SATSYNC_MENU		0x8340		// 8-3-4. dwdr saturation sync

#define		MD_AREA_MENU			0x8410		// 8-4-1. md area
#define		MD_AREA_EN_MENU			0x8420		// 8-4-2. md area enable
#define		MD_DISP_MENU			0x8430		// 8-4-3. md area display
#define		MD_MOTION_MENU			0x8460		// 8-4-6. md motion view
#define		MDDEFAULT_MENU			0x8470		// 8-4-7. md default
#define		HSBLCAREA_MENU			0x8610		// 8-6-1. hsblc area
#define		BLC_ONOFF_MENU			0x8610		// 8-6-1. blc on/off
#define		HSBLCDISP_MENU			0x8620		// 8-6-2. hsblc area display
#define		BLC_WIN_MENU			0x8630		// 8-6-3. blc display
#define		BLCDEFAULT_MENU			0x8640		// 8-6-4. blc default
#define		HSBLCBMLEV_MENU			0x8640		// 8-6-4. hsblc black mask level
#define		HSBLCBM_MENU			0x8650		// 8-6-5. hsblc black mask
#define		HSBLCDEFAULT_MENU		0x8660		// 8-6-6. hsblc default
#define		SDPC_RUN_MENU			0x8810		// 8-8-1. static dpc process
#define		SDPC_WRITE_MENU			0x8830		// 8-8-3. flash write
#define		SDPC_CLEAR_MENU			0x8840		// 8-8-4. flash clear
#define		DDPC_AUTO_MENU			0x8850		// 8-8-5. Dyanmic DPC Parameter
#define		DDPC_THHD1_MENU			0x8860		// 8-8-6. Set Dyanmic DPC Thresholds1 
#define		DDPC_THHD2_MENU			0x8870		// 8-8-7. Set Dyanmic DPC Thresholds2
#define		DDPC_SELMODE_MENU		0x8880		// 8-8-7. Set Dyanmic DPC Thresholds2
#define 	DISP_FREEZE_MENU 		0x8910 		// 8-9-1. freeze
#define 	DISP_MIRROR_MENU 		0x8920 		// 8-9-2. mirror
#define 	DISP_ROT_MENU 			0x8940 		// 8-9-4. rotation
#define		PICSTYLE_MENU			0x8950		// 8-9-5. picture style
#define		PICSTYLE_CON_MENU		0x8960		// 8-9-6. Pic contrast
#define		PICSTYLE_SAT_MENU		0x8970		// 8-9-7. Pic saturation
#define		PICSTYLE_EDE_MENU		0x8980		// 8-9-8. Pic edge enhance
#define		RS485_ID_MENU			0x9310		// 9-3-1. rs485 cam id
#define		RS485_DISP_MENU			0x9320		// 9-3-2. rs485 id display
#define		RS485_BAUD_MENU			0x9330		// 9-3-3. rs485 baud rate
#define		SYS_INFO_MENU			0x9760		// 9-7-6. system information
#define		CC_Red_MENU				0xA310		// 10-3-1. color correction
#define		CC_Green_MENU			0xA320		// 10-3-2. color correction
#define		CC_Blue_MENU			0xA330		// 10-3-3. color correction
#define		CAC_AreaSel_MENU		0xA340		// 10-3-4. color correction(Chroma area control)
#if defined(CAC_ctrl_theta)
#define		CAC_Theta_MENU			0xA350		// 10-3-5. color correction(Chroma area control)
#define		CAC_Saturation_MENU		0xA360		// 10-3-6. color correction(Chroma area control)
#define		CAC_EEPROM_MENU			0xA370		// 10-3-7. color correction(Chroma area control)
#else
#define		CAC_Cos_MENU			0xA350		// 10-3-5. color correction(Chroma area control)
#define		CAC_Sin_MENU			0xA360		// 10-3-6. color correction(Chroma area control)
#define		CAC_Saturation_MENU		0xA370		// 10-3-7. color correction(Chroma area control)
#define		CAC_EEPROM_MENU			0xA380		// 10-3-8. color correction(Chroma area control)
#endif
#define		TP_CTRL_MENU			0xA610		// 10-6-1. sensor tp select
#define		TP_ONOFF_MENU			0xA620		// 10-6-2. sensor tp onoff
#define		TPDEFAULT_MENU			0xA640		// 10-6-4. sensor tp default
#define		MISCTUNE_USER_PLL_MENU	0xA940
#define		MISCTUNE_PWMIRIS_MENU	0xC410		// 12-4-1. misc tune - pwm iris tune
#define		MISCTUNE_PANPARAM_MENU	0xA980		// 10-9-8. misc tune - pan ctrl value -> 0xA970 & 0xA980
#define		MISCTUNE_TESTMODE_MENU	0xA970		// 10-9-7. misc tune - Test mode 
#define		DNRTUNE_DEMO_MENU		0xAB10		// 10-11-1. dnr(tuning-demo mode)
#define		DNRTUNE_LOW_MENU		0xAB20		// 10-11-2. dnr(tuning-chk low light mode?)
#define		DNRTUNE_M2D_MENU		0xAB30		// 10-11-1. dnr(tuning-M2D mode)
#define		DNRTUNE_L2D_MENU		0xAB40		// 10-11-1. dnr(tuning-L2D mode)
#define		VSPOSIX_MENU			0xC320		// 12-3-2. vs position-x
#define		VSPOSIY_MENU			0xC330		// 12-3-3. vs position-y
#define		VSSIZEX_MENU			0xC340		// 12-3-4. vs size-x
#define		VSSIZEY_MENU			0xC350		// 12-3-5. vs size-y
#define		VSDRAW_MENU				0xC360		// 12-3-6. vs draw scope
#define		CCF_LineSel_MENU		0xC510		// 12-5-1. color correction(CCF Line Sel)
#define		CCF_Gain_MENU			0xC520		// 12-5-2. color correction(SetCCFGain)
#define		CCF_Balance_MENU		0xC530		// 12-5-3. color correction(SetCCFBalance)
#define		CCF_Print_MENU			0xC550		// 12-5-5. color correction(CCF result print)

// Level 4
#define		DZRATIO_MENU			0x8111		// 8-1-1-1. dzoom ratio
#define		DZPOSI_MENU				0x8112		// 8-1-1-2. dzoom position
#define 	PIPPOSI_MENU			0x8141		// 8-1-4-1. pip position
#define		MD_POSI_MENU			0x8441		// 8-4-4-1. md position
#define		MD_SIZE_MENU			0x8442		// 8-4-4-2. md size
#define		BLC_POSI_MENU			0x8621		// 8-6-2-1. blc position(start)
#define		BLC_SIZE_MENU 			0x8622		// 8-6-2-2. blc position(end)
#define		HSBLCPOSI_MENU			0x8631		// 8-6-3-1. hsblc position
#define		HSBLCSIZE_MENU			0x8632		// 8-6-3-2. hsblc size
#define		TP_POSI_MENU			0xA631		// 10-6-3-1. sensor tp position
#define		TP_SIZE_MENU			0xA632		// 10-6-3-2. sensor tp size

// menu osd color
#define		OSD_BLACK		0		//#000000
#define		OSD_BLUE		1		//#0000FF
#define		OSD_GRAY		2		//#808080
#define 	OSD_YELLOW		3		//#FFFF00
#define		OSD_WHITE		4		//#FFFFFF
#define		OSD_BLACK_OP	5		//#000000(1)
#define		OSD_WHITE_OP	6		//#FFFFFF(1)
#define		OSD_NOCOLOR		7		//#000000(0)

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------
// message osd(chinese)
extern ROMDATA WORD msg_chn_turn_off_wdr_compr[];
extern ROMDATA WORD msg_chn_turn_off_cascade[];
extern ROMDATA WORD msg_chn_turn_off_dzoom[];
extern ROMDATA WORD msg_chn_turn_off_dis[];
extern ROMDATA WORD msg_chn_turn_off_binning[];
extern ROMDATA WORD msg_chn_turn_off_rotation[];
extern ROMDATA WORD msg_chn_set_sensor_input[];
extern ROMDATA WORD msg_chn_bootloader_fail[];
extern ROMDATA WORD msg_chn_fw_download[];
extern ROMDATA WORD msg_chn_factory_reset[];
extern ROMDATA WORD title_chn_dzoom_on[];
extern ROMDATA WORD title_chn_dzoom_off[];
extern ROMDATA WORD msg_chn_not_available[];


// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void MenuDisplayHandler(void);
WORD GetMenuPosiY(WORD mID);
WORD GetMenuStatus(WORD mID);
void SetMenuStatus(WORD mID, BYTE val);
void SetMenuPosiY(WORD mID, BYTE val);
WORD GetMenuItemMax(WORD mID);
WORD GetOSDMenuID(void);
void SetOSDMenuID(WORD nID);
WORD GetOSDCombID(void);
void SetOSDCombID(WORD nID);
void SetMenuDefaultItem(WORD mID);
void SetMenuInitValue(void);
void SetMenuFactoryDefault(void);
DWORD GetMenu_FirmwareVer(void);
void SetMenuRedraw(void);

void MenuMessageControl(void);
BYTE MenuMessageExit(void);
void DirUMessageControl(void);
void DirDMessageControl(void);
void DirRMessageControl(void);
void DirLMessageControl(void);
void EnterMessageControl(void);
void OSD_CamTitleDisplay(BOOL OnOff);
BYTE OSD_GetCamTitleDisplay(void);
void OSD_IrkeyTitleDisplay(BOOL OnOff, PBYTE pSTR1);
void OSD_InfoDisplay(BYTE mode, PBYTE pSTR1, PBYTE pSTR2);
void EraseE2promMenuStatus(void);
void OSD_CHNoDisplay(BOOL OnOff, WORD w, WORD h);
void OSD_DrawOutlineQuad(BOOL OnOff, WORD w, WORD h);
void OSD_DrawOutlineDouble(BOOL OnOff, WORD w, WORD h);
void OSD_MessageBox(BOOL OnOff, PBYTE pSTR1);
void OSD_StrDispAttb(BYTE y, BYTE x, BOOL OnOff, BYTE color, const char* format, ...);
void OSD_StrDispOnOff(BOOL OnOff);
BYTE OSD_GetMenuLang(void);
void OSD_WaitMessageBox(BOOL OnOff);

#endif	/* __MENU_H__ */
