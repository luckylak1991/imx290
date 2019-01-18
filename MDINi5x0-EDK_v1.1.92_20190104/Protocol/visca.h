//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2015  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  VISCA.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__VISCA_H__
#define		__VISCA_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#if defined(__IDEO_ZOOM_V101__)
#define 	VISCA_PACKET_MAX					20
#else
#define 	VISCA_PACKET_MAX					16
#endif

#ifdef		__USE_UART0_VISCA__
#define 	VISCA_PORT_NO						0			// visca uart port number(0:uart_0, 1:uart_1)
#else
#define 	VISCA_PORT_NO						1			// visca uart port number(0:uart_0, 1:uart_1)
#endif

#define		VISCA_VENDER_ID						0x0020		// 0x0020(sony)
#define		VISCA_MODEL_ID						0x0467		// 0x0467(FCB-EV7500)
#define		VISCA_MAX_SOCKET					0x02		// max socket

typedef	struct
{
	LONG	ctrl;				// visca control flag
	
	BYTE 	RxBuffer[VISCA_PACKET_MAX];		// visca rx data buffer
	BYTE	RxPacket[20][VISCA_PACKET_MAX];	// visca rx packet buffer
	BYTE 	TxPacket[VISCA_PACKET_MAX];		// visca tx packet buffer
	BYTE 	CmdBuffer[2][VISCA_PACKET_MAX];	// visca command buffer(0/1 : socket 1/2)
	WORD	RepeatCmd;			// visca command buffer for repeat command(zoom/focus)
	BYTE 	RxCnt;				// visca rx packet byte counter
	BYTE 	TxCnt;				// visca tx packet byte counter
	BYTE 	sn;					// visca command socket number(1,2)
	BYTE	cam_addr;			// visca camera address(1~7)
	BYTE	RxPacketPushCnt;	// visca rx packet buffer push counter
	BYTE	RxPacketPopCnt;		// visca rx packet buffer pop counter

}	VISCA_CTRL_PARAM, *PVISCA_CTRL_PARAM;


typedef	struct
{
	WORD	zoom_posi;
//	WORD	focus_nearlimit;
	WORD	focus_posi;
	BYTE	ctrl1;
	BYTE	ctrl2;
	WORD 	dummy_001;

}	VISCA_LENS_PARAM, *PVISCA_LENS_PARAM;

typedef	struct
{
	BYTE	rgain;
	BYTE	bgain;
	BYTE	wb_mode;
	BYTE	aperture_gain;
	BYTE	exposure_mode;
	BYTE	ctrl;
	BYTE	shutter_posi;
	BYTE	iris_posi;
	BYTE	gain_posi;
	BYTE	bright_posi;
	BYTE	expcomp_posi;
	BYTE	sensup_posi;
	BYTE	dummy0;
	BYTE	dummy1;
	BYTE	dummy2;

}	VISCA_CAM_PARAM, *PVISCA_CAM_PARAM;

typedef	struct
{
	BYTE	ctrl1;
	BYTE	ctrl2;
	BYTE	ctrl3;
	BYTE	pic_effect_mode;
	WORD	cam_id;
	BYTE	ctrl4;
	BYTE	dummy_001;
	WORD	dummy_002;
	
}	VISCA_OTHER_PARAM, *PVISCA_OTHER_PARAM;

typedef	struct
{
	BYTE	dzoom_posi;
//	BYTE	af_act_time;
//	BYTE	af_inter_time;
	BYTE	spotae_posi;
	BYTE	ctrl1;
	BYTE	color_gain;
	BYTE	ctrl2;
	BYTE	ae_response;
	BYTE 	gamma;
	BYTE	ctrl3;
	BYTE 	nr_level;
	BYTE 	chroma_suppress;
//	BYTE 	gain_limit;
//	BYTE	dummy_001;
	WORD	dummy_002;

}	VISCA_EFUNC1_PARAM, *PVISCA_EFUNC1_PARAM;

typedef	struct
{
	BYTE	wd_mode;
	BYTE	wd_bright_lev;
	BYTE	wd_bright_comp_sel;
	BYTE	wd_comp_lev;
	BYTE	defog_mode;
	BYTE	defog_lev;
	WORD	dummy_002;

}	VISCA_EFUNC2_PARAM, *PVISCA_EFUNC2_PARAM;

typedef	struct
{
	BYTE	color_hue;
	BYTE	dummy_001;
	WORD	dummy_002;

}	VISCA_EFUNC3_PARAM, *PVISCA_EFUNC3_PARAM;

typedef	struct
{
	LONG	mask_disp;
	BYTE	color[24];
	BYTE	w[24];
	BYTE	h[24];
	BYTE	x[24];
	BYTE	y[24];
	WORD	pan;
	WORD	tilt;
	WORD	maskpan[24];
	WORD	masktilt[24];
	WORD	maskzoom[24];

}	VISCA_PM_PARAM, *PVISCA_PM_PARAM;

typedef	struct
{
	BYTE	disp_mode;
	BYTE	det_frame;
	BYTE	thres;
	BYTE	inter_time;
	BYTE	lx[4];
	BYTE	ly[4];
	BYTE	rx[4];
	BYTE	ry[4];

}	VISCA_MD_PARAM, *PVISCA_MD_PARAM;

typedef	struct
{
//	BYTE	baud_rate;				// addr(0x00)
//	BYTE	mon_mode;				// addr(0x72)
	BYTE	out_en;					// addr(0x73)
	BYTE	lvds_mode;				// addr(0x74)
	BYTE	zoom_widelimit;			// addr(0x50)
	BYTE	zoom_telelimit;			// addr(0x51)
	BYTE	dzoom_max;				// addr(0x52)
	BYTE	stablezoom;				// addr(0x53)
	BYTE	focus_trace;			// addr(0x54)
	BYTE	focus_offset;			// addr(0x55)
	BYTE	ex_mode;				// addr(0x5f)
	BYTE	dummy_001;
	WORD	dummy_002;

}	VISCA_REG_PARAM, *PVISCA_REG_PARAM;

typedef	struct
{
	BYTE	onoff;
	BYTE	thres;
	BYTE	y_high;
	BYTE	cr_high;
	BYTE	cb_high;
	BYTE	y_low;
	BYTE	cr_low;
	BYTE	cb_low;

}	VISCA_COLENH_PARAM, *PVISCA_COLENH_PARAM;

typedef	struct
{
	WORD	display;
	BYTE	h_posi[11];
	BYTE	color[11];
	WORD	blink;
	WORD	cnt;
	BYTE	title[11][22];
	WORD	dummy;

}	VISCA_CAMTITLE_PARAM, *PVISCA_CAMTITLE_PARAM;

typedef	struct
{
	BYTE	expcomp_posi;
	BYTE	aperture_gain;
	BYTE	color_gain;
	BYTE	color_hue;
//	BYTE	autoicr_offon_thres;	// auto icr threshold level(off:day -> on:night)
//	BYTE	dummy_001;
//	WORD	dummy_002;

}	VISCA_ENLARGE_PARAM, *PVISCA_ENLARGE_PARAM;

typedef	struct
{
	BYTE	ctrl;
//	BYTE	autoicr_thres;			// auto icr threshold level(on:night -> off:day)
//	BYTE	autoicr_delay;			// auto icr time delay(on:night <-> off:day)
	BYTE	memory;					// memory no
	BYTE	zposrep_inter_time;		// zoom position reply interval time
	BYTE	fposrep_inter_time;		// focus position reply interval time
	BYTE	lens_temp;				// lens temperature
	BYTE	dscr_disptime_cnt;		// data screen display time count
	BYTE	dscr_inter_cnt;			// data screen display interval count
	BYTE	dummy_001;
//	WORD	dummy_002;
	WORD	memsave[8];				// user memory
	BYTE	maintenance_cmd[4];		// maintenance mode selection cmd

}	VISCA_USER_PARAM, *PVISCA_USER_PARAM;

typedef	struct
{
	LONG	ctrl;				// visca application control flag
	
	VISCA_LENS_PARAM		lens;
	VISCA_CAM_PARAM			cam;
	VISCA_OTHER_PARAM		other;
	VISCA_EFUNC1_PARAM		efunc1;
	VISCA_EFUNC2_PARAM		efunc2;
	VISCA_EFUNC3_PARAM		efunc3;
	VISCA_PM_PARAM			pm;				// privacy mask
	VISCA_MD_PARAM			md;				// motion detection
	VISCA_REG_PARAM			reg;			// register
	VISCA_ENLARGE_PARAM		enlarge;		// enlargement
	VISCA_COLENH_PARAM		color_enhance;	// color enhance
	VISCA_CAMTITLE_PARAM	camtitle;		// camera title
	VISCA_USER_PARAM		user;			// etc..

}	VISCA_INFO_PARAM, *PVISCA_INFO_PARAM;


// visca driver
#define		VISCA_RX_BGN						0x80
#define		VISCA_RX_END						0xFF
#define		VISCA_MSG_ACK						0x00		// acknowledge
#define		VISCA_MSG_CMD_COMPLETE				0x01		// command execute completed
#define 	VISCA_MSG_INQ_COMPLETE				0x02		// inquiry data return completed
#define		VISCA_MSG_ERR_SYNTAX				0x10		// rx command syntax error
#define		VISCA_MSG_ERR_CMDCANCEL				0x11		// command canceled
#define		VISCA_MSG_ERR_NOSOCKET				0x12		// invalid socket number
#define		VISCA_MSG_ERR_CMDNOTEXEC			0x13		// command not excuted
#define		VISCA_MSG_ERR_CMDBUFFULL			0x14		// command buffer full
#define 	VISCA_MSG_NET_CHANGE				0x20
#define 	VISCA_MSG_USER_DEFINE				0x40		// reply for command
#define		VISCA_MSG_AUTO_REPLY				0x80		// auto reply
#define		VISCA_CMD_RX_MASK					0x00000003		// visca command rx mask
#define 	VISCA_CMD_S1_PARSED					0x00000001		// visca command parsed(socket 1)
#define 	VISCA_CMD_S2_PARSED					0x00000002		// visca command parsed(socket 2)
#define 	VISCA_CMD_S1S2_PARSED				0x00000003		// visca command parsed(socket 1&2)
#define		VISCA_CMD_S1_HIGHPRIORITY			0x00000000		// visca command socket-1 high priority
#define		VISCA_CMD_S2_HIGHPRIORITY			0x00000004		// visca command socket-2 high priority
#define		VISCA_CMD_NOT_PARSED				0x00000010		// visca command not parsed(when buffer(socket 1/2) full)
#define		VISCA_INQ_NOT_EXCUTED				0x00000020		// visca inquiry not excuted(when cmd buffer not blank)
#define		VISCA_IF_ACTIVATED					0x00000100		// visca i/f activated by external equipment
#define		VISCA_TRIGGER						0x80000000		// visca process handler tirgger
#define		VISCA_VSYNC_TRIGGER					0x40000000		// visca process handler v-sync tirgger(set every v-sync)

// visca application
// lens control-1
#define 	VISCA_DZOOM_COMBINE_ON				(0<<5)
#define 	VISCA_DZOOM_COMBINE_OFF				(1<<5)
#define		VISCA_AFMODE_MASK					(3<<3)
#define 	VISCA_AFMODE_NORMAL					(0<<3)
#define 	VISCA_AFMODE_INTERVAL				(1<<3)
#define 	VISCA_AFMODE_ZOOMTRIG				(2<<3)
#define 	VISCA_AFSENS_LOW					(0<<2)
#define 	VISCA_AFSENS_NORMAL					(1<<2)
#define 	VISCA_DZOOM_OFF						(0<<1)
#define 	VISCA_DZOOM_ON						(1<<1)
#define 	VISCA_FOCUS_MODE_MANUAL				(0<<0)
#define 	VISCA_FOCUS_MODE_AUTO				(1<<0)

// lens control-2
#define 	VISCA_LOWCONTRAST_DET_OFF			(0<<3)
#define 	VISCA_LOWCONTRAST_DET_ON			(1<<3)
#define 	VISCA_CAMMEM_RECALL_STOP			(0<<2)
#define 	VISCA_CAMMEM_RECALL_RUN				(1<<2)
#define 	VISCA_FOCUS_CMD_STOP				(0<<1)
#define 	VISCA_FOCUS_CMD_RUN					(1<<1)
#define 	VISCA_ZOOM_CMD_STOP					(0<<0)
#define 	VISCA_ZOOM_CMD_RUN					(1<<0)

// camera control
#define 	VISCA_HR_OFF						(0<<5)
#define 	VISCA_HR_ON							(1<<5)
#define 	VISCA_WD_OFF						(0<<4)
#define 	VISCA_WD_NOTOFF						(1<<4)
#define 	VISCA_SPOTAE_OFF					(0<<3)
#define 	VISCA_SPOTAE_ON						(1<<3)
#define 	VISCA_BACKLIGHT_OFF					(0<<2)
#define 	VISCA_BACKLIGHT_ON					(1<<2)
#define 	VISCA_EXPCOMP_OFF					(0<<1)
#define 	VISCA_EXPCOMP_ON					(1<<1)
#define 	VISCA_SLOWSHUTTER_OFF				(0<<0)
#define 	VISCA_SLOWSHUTTER_ON				(1<<0)

#define		VISCA_WBMODE_AUTO					0x00
#define		VISCA_WBMODE_INDOOR					0x01
#define		VISCA_WBMODE_OUTDOOR				0x02
#define		VISCA_WBMODE_ONEPUSH				0x03
#define		VISCA_WBMODE_ATW					0x04
#define		VISCA_WBMODE_MANUAL					0x05
#define		VISCA_WBMODE_OUTDOOR_AUTO			0x06
#define		VISCA_WBMODE_SODIUM_AUTO			0x07
#define		VISCA_WBMODE_SODIUM					0x08
#define		VISCA_WBMODE_SODIUM_OUTDOOR_AUTO	0x09

#define		VISCA_AEMODE_AUTO					0x00
#define		VISCA_AEMODE_MANUAL					0x03
#define		VISCA_AEMODE_SHUTTERFIX				0x0A
#define		VISCA_AEMODE_IRISFIX				0x0B
#define		VISCA_AEMODE_BRIGHT					0x0D

// other control-1
#define 	VISCA_AUTOICR_ALARM_OFF				(0<<3)
#define 	VISCA_AUTOICR_ALARM_ON				(1<<3)
#define 	VISCA_AUTOICR_OFF					(0<<2)
#define 	VISCA_AUTOICR_ON					(1<<2)
#define 	VISCA_POWER_OFF						(0<<0)
#define 	VISCA_POWER_ON						(1<<0)

// other control-2
#define 	VISCA_STABILIZER_OFF				(0<<6)
#define 	VISCA_STABILIZER_ON					(1<<6)
#define 	VISCA_STABILIZER_HOLD_OFF			(0<<5)
#define 	VISCA_STABILIZER_HOLD_ON			(1<<5)
#define 	VISCA_ICR_OFF						(0<<4)
#define 	VISCA_ICR_ON						(1<<4)
#define 	VISCA_FREEZE_OFF					(0<<3)
#define 	VISCA_FREEZE_ON						(1<<3)
#define 	VISCA_LRREVERSE_OFF					(0<<2)
#define 	VISCA_LRREVERSE_ON					(1<<2)

// other control-3
#define 	VISCA_PM_OFF						(0<<5)
#define 	VISCA_PM_ON							(1<<5)
#define 	VISCA_MUTE_OFF						(0<<4)
#define 	VISCA_MUTE_ON						(1<<4)
#define 	VISCA_TILEDISP_OFF					(0<<3)
#define 	VISCA_TILEDISP_ON					(1<<3)
#define 	VISCA_DISP_OFF						(0<<2)
#define 	VISCA_DISP_ON						(1<<2)

// other control-4
#define 	VISCA_MEM_NOTPROVIDED				(0<<4)
#define 	VISCA_MEM_PROVIDED					(1<<4)
#define 	VISCA_ICR_NOTPROVIDED				(0<<2)
#define 	VISCA_ICR_PROVIDED					(1<<2)
#define 	VISCA_STABILIZER_NOTPROVIDED		(0<<1)
#define 	VISCA_STABILIZER_PROVIDED			(1<<1)
#define 	VISCA_SYSTEM_60_30HZ				(0<<0)
#define 	VISCA_SYSTEM_50_25HZ				(1<<0)

// other picture effect mode
#define		VISCA_PICEFFECT_OFF					0x00
#define		VISCA_PICEFFECT_NEG					0x02
#define		VISCA_PICEFFECT_BW					0x04

// enlargement function1 control-1
#define 	VISCA_MD_OFF						(0<<2)
#define 	VISCA_MD_ON							(1<<2)
#define 	VISCA_FLIP_OFF						(0<<0)
#define 	VISCA_FLIP_ON						(1<<0)

// enlargement function1 control-2
#define 	VISCA_ADVPM_NOTPROVIDED				(0<<2)
#define 	VISCA_ADVPM_PROVIDED				(1<<2)
#define 	VISCA_FLIP_NOTPROVIDED				(0<<0)
#define 	VISCA_FLIP_PROVIDED					(1<<0)

// enlargement function1 control-3
#define 	VISCA_HIGHSENS_OFF					(0<<3)
#define 	VISCA_HIGHSENS_ON					(1<<3)

// enlargement function2
#define 	VISCA_WDMODE_OFF					(0<<0)
#define 	VISCA_WDMODE_ON						(1<<0)
#define 	VISCA_WDMODE_VE_ON					(2<<0)

#define		VISCA_DEFOG_OFF						(0<<0)
#define		VISCA_DEFOG_ON						(1<<0)

#define		VISCA_DEFOG_defMID					(0<<0)
#define		VISCA_DEFOG_LOW						(1<<0)
#define		VISCA_DEFOG_MID						(2<<0)
#define		VISCA_DEFOG_HIGH					(3<<0)

// pm mask display
#define		VISCA_PM_MASK_OFF					0x00000000
#define		VISCA_PM_MASK_A_ON					0x00000001
#define		VISCA_PM_MASK_B_ON					0x00000002
#define		VISCA_PM_MASK_C_ON					0x00000004
#define		VISCA_PM_MASK_D_ON					0x00000008
#define		VISCA_PM_MASK_E_ON					0x00000010
#define		VISCA_PM_MASK_F_ON					0x00000020
#define		VISCA_PM_MASK_G_ON					0x00000100
#define		VISCA_PM_MASK_H_ON					0x00000200
#define		VISCA_PM_MASK_I_ON					0x00000400
#define		VISCA_PM_MASK_J_ON					0x00000800
#define		VISCA_PM_MASK_K_ON					0x00001000
#define		VISCA_PM_MASK_L_ON					0x00002000
#define		VISCA_PM_MASK_M_ON					0x00010000
#define		VISCA_PM_MASK_N_ON					0x00020000
#define		VISCA_PM_MASK_O_ON					0x00040000
#define		VISCA_PM_MASK_P_ON					0x00080000
#define		VISCA_PM_MASK_Q_ON					0x00100000
#define		VISCA_PM_MASK_R_ON					0x00200000
#define		VISCA_PM_MASK_S_ON					0x01000000
#define		VISCA_PM_MASK_T_ON					0x02000000
#define		VISCA_PM_MASK_U_ON					0x04000000
#define		VISCA_PM_MASK_V_ON					0x08000000
#define		VISCA_PM_MASK_W_ON					0x10000000
#define		VISCA_PM_MASK_X_ON					0x20000000

// md - display mode
#define		VISCA_MD_DISP_OFF					0x00
#define		VISCA_MD_DISP_ON					0x01

// md - detect frame
#define		VISCA_MD_FRAME0_DET_OFF				0x00
#define		VISCA_MD_FRAME0_DET_ON				0x01
#define		VISCA_MD_FRAME1_DET_OFF				0x00
#define		VISCA_MD_FRAME1_DET_ON				0x02
#define		VISCA_MD_FRAME2_DET_OFF				0x00
#define		VISCA_MD_FRAME2_DET_ON				0x04
#define		VISCA_MD_FRAME3_DET_OFF				0x00
#define		VISCA_MD_FRAME3_DET_ON				0x08

// register baudrate
#define		VISCA_REG_BAUDRATE_9600				0x00
#define		VISCA_REG_BAUDRATE_19200			0x01
#define		VISCA_REG_BAUDRATE_38400			0x02
#define		VISCA_REG_BAUDRATE_115200			0x03

// register monitor mode
#define		VISCA_REG_MON_1080I59				0x01
//#define		VISCA_REG_MON_RESERVED				0x02
#define		VISCA_REG_MON_NTSCONLY				0x03
#define		VISCA_REG_MON_1080I50				0x04
#define		VISCA_REG_MON_PALONLY				0x05
#define		VISCA_REG_MON_1080P29				0x06
//#define		VISCA_REG_MON_RESERVED				0x07
#define		VISCA_REG_MON_1080P25				0x08
#define		VISCA_REG_MON_720P59				0x09
//#define		VISCA_REG_MON_RESERVED				0x0A
//#define		VISCA_REG_MON_RESERVED				0x0B
#define		VISCA_REG_MON_720P50				0x0C
//#define		VISCA_REG_MON_RESERVED				0x0D
#define		VISCA_REG_MON_720P29				0x0E
//#define		VISCA_REG_MON_RESERVED				0x0F
//#define		VISCA_REG_MON_RESERVED				0x10
#define		VISCA_REG_MON_720P25				0x11
#define		VISCA_REG_MON_RESERVED				0x12
#define		VISCA_REG_MON_1080P59				0x13
#define		VISCA_REG_MON_1080P50				0x14

#define		VISCA_REG_OUTEN_ANALOG_EN			0x01
#define		VISCA_REG_OUTEN_DIGITAL_EN			0x02
#define		VISCA_REG_OUTEN_BOTH_EN				0x03

#define		VISCA_REG_LVDS_SINGLE				0x00
#define		VISCA_REG_LVDS_DUAL					0x01

// register enlargement(ex mode)
#define		VISCA_REG_EX_EXPCOMP256LVL_OFF		(0<<0)
#define		VISCA_REG_EX_EXPCOMP256LVL_ON		(1<<0)
#define		VISCA_REG_EX_APER256LVL_OFF			(0<<1)
#define		VISCA_REG_EX_APER256LVL_ON			(1<<1)
#define		VISCA_REG_EX_COLORGAINHUE256LVL_OFF	(0<<2)
#define		VISCA_REG_EX_COLORGAINHUE256LVL_ON	(1<<2)
#define		VISCA_REG_EX_AUTOICROFFONSET_OFF	(0<<3)
#define		VISCA_REG_EX_AUTOICROFFONSET_ON		(1<<3)


// user control
#define 	VISCA_IRCORRECTION_OFF				(0<<7)
#define 	VISCA_IRCORRECTION_ON				(1<<7)
#define		VISCA_ZOOMPOS_REPLY_OFF				(0<<6)
#define		VISCA_ZOOMPOS_REPLY_ON				(1<<6)
#define		VISCA_FOCUSPOS_REPLY_OFF			(0<<5)
#define		VISCA_FOCUSPOS_REPLY_ON				(1<<5)
#define		VISCA_AUTOICR_STATUS_DAY			(0<<4)
#define		VISCA_AUTOICR_STATUS_NIGHT			(1<<4)
#define		VISCA_TRANS_DZ_TO_ZOOM				(1<<3)
#define		VISCA_WB_ONEPUSH_DONE				(1<<2)

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------
extern VISCA_INFO_PARAM	stVISCAINFO;
extern VISCA_CTRL_PARAM	stVISCACTRL;
extern volatile BOOL fVISCA_CmdReceive, fSerialCmdReceive;

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// visca_drv.c
void VISCA_Initialize(void);
void VISCA_ProcessHandler(void);
void VISCA_MessageExecution(void);
void ISPM VISCA_RxHandler(BYTE ch);
void ISPM VISCA_MessageClassify(void);
void ISPM VISCA_VSyncTrigger(void);
BOOL ISPM IsVISCA_CommandZOOM(void);
void VISCA_SetRepeatCmd(WORD cmd);
BOOL VISCA_GetActStatus(void);

// visca_app.c
void VISCAAPP_PowerOnOff(BOOL OnOff);
void VISCAAPP_ZoomStop(void);
void VISCAAPP_ZoomTeleStd(void);
void VISCAAPP_ZoomWideStd(void);
void VISCAAPP_ZoomTeleVar(BYTE speed);
void VISCAAPP_ZoomWideVar(BYTE speed);
void VISCAAPP_ZoomDirect(WORD posi);
void VISCAAPP_ExeRepeatCmd(WORD cmd);
WORD VISCAAPP_GetZoomPosi(void);

void VISCAAPP_DZoomStop(void);
void VISCAAPP_DZoomOnOff(BOOL OnOff);
void VISCAAPP_DZoomSwitch(void);
void VISCAAPP_DZoomTeleVar(BYTE speed);
void VISCAAPP_DZoomWideVar(BYTE speed);
void VISCAAPP_DZoomCombine(BOOL OnOff);
void VISCAAPP_DZoomDirect(BYTE posi);
WORD VISCAAPP_GetDZoomPosi(void);
BOOL VISCAAPP_GetDZoomCSMode(void);
BOOL VISCAAPP_GetDZoomMode(void);

void VISCAAPP_FocusStop(void);
void VISCAAPP_FocusFarStd(void);
void VISCAAPP_FocusNearStd(void);
void VISCAAPP_FocusFarVar(BYTE speed);
void VISCAAPP_FocusNearVar(BYTE speed);
void VISCAAPP_FocusDirect(WORD posi);
void VISCAAPP_FocusAutoOnOff(BOOL OnOff);
void VISCAAPP_FocusAutoManual(void);
void VISCAAPP_FocusOnepushTrig(void);
void VISCAAPP_FocusNearLimit(WORD posi);
WORD VISCAAPP_GetFocusPosi(void);
BOOL VISCAAPP_GetFocusMode(void);
BYTE VISCAAPP_GetAFActiveTime(void);
BYTE VISCAAPP_GetAFIntervalTime(void);
WORD VISCAAPP_GetFocusNearLimit(void);

void VISCAAPP_AFSensitivity(BOOL flag);
void VISCAAPP_AFMode(BYTE mode);
void VISCAAPP_AFIntervalTime(BYTE act, BYTE inter);
BYTE VISCAAPP_GetAFMode(void);

void VISCAAPP_IRCorrectionStandard(void);
void VISCAAPP_IRCorrectionIRLight(void);

void VISCAAPP_ZoomFocusDirect(WORD z_posi, WORD f_posi);

void VISCAAPP_LensInit(void);
void VISCAAPP_CameraInit(void);

void VISCAAPP_WBMode(BYTE mode);
void VISCAAPP_WBOnepushTrig(void);
void VISCAAPP_WBRGainReset(void);
void VISCAAPP_WBRGainUp(void);
void VISCAAPP_WBRGainDown(void);
void VISCAAPP_WBRGainDirect(BYTE gain);
void VISCAAPP_WBBGainReset(void);
void VISCAAPP_WBBGainUp(void);
void VISCAAPP_WBBGainDown(void);
void VISCAAPP_WBBGainDirect(BYTE gain);

void VISCAAPP_AEMode(BYTE mode);
void VISCAAPP_AutoSlowShutterOnOff(BOOL OnOff);
void VISCAAPP_AutoSlowShutterSetLimit(BYTE val);

void VISCAAPP_ShutterReset(void);
void VISCAAPP_ShutterUp(void);
void VISCAAPP_ShutterDown(void);
void VISCAAPP_ShutterDirect(BYTE posi);

void VISCAAPP_IrisReset(void);
void VISCAAPP_IrisUp(void);
void VISCAAPP_IrisDown(void);
void VISCAAPP_IrisDirect(BYTE posi);
BYTE VISCAAPP_GetIrisPosi(void);


void VISCAAPP_GainReset(void);
void VISCAAPP_GainUp(void);
void VISCAAPP_GainDown(void);
void VISCAAPP_GainDirect(BYTE posi);
void VISCAAPP_GainLimit(BYTE posi);
BYTE VISCAAPP_GetGainLimit(void);

void VISCAAPP_BrightReset(void);
void VISCAAPP_BrightUp(void);
void VISCAAPP_BrightDown(void);
void VISCAAPP_BrightDirect(BYTE posi);

void VISCAAPP_ExpCompOnOff(BOOL OnOff);
void VISCAAPP_ExpCompReset(void);
void VISCAAPP_ExpCompUp(void);
void VISCAAPP_ExpCompDown(void);
void VISCAAPP_ExpCompDirect(BYTE posi);

void VISCAAPP_BacklightOnOff(BOOL OnOff);
void VISCAAPP_HLCLevel(BYTE level);
void VISCAAPP_HLCMaskLevel(BYTE level);
BYTE VISCAAPP_GetHLCLevel(void);
BYTE VISCAAPP_GetHLCMaskLevel(void);

void VISCAAPP_SpotAEOnOff(BOOL OnOff);
void VISCAAPP_SpotAEPosition(BYTE x, BYTE y);

void VISCAAPP_AEResponse(BYTE val);

void VISCAAPP_WDOnOff(BOOL OnOff);
void VISCAAPP_WDVEOnOff(BOOL OnOff);
void VISCAAPP_WDSetParameter(BYTE bright_lev, BYTE comp_sel, BYTE comp_lev);

void VISCAAPP_ApertureReset(void);
void VISCAAPP_ApertureUp(void);
void VISCAAPP_ApertureDown(void);
void VISCAAPP_ApertureDirect(BYTE posi);

void VISCAAPP_HROnOff(BOOL OnOff);

void VISCAAPP_NRSet(BYTE val);

void VISCAAPP_GammaStandard(void);
void VISCAAPP_GammaStraight(void);

void VISCAAPP_HighSensitivityOnOff(BOOL OnOff);

void VISCAAPP_LRReverseOnOff(BOOL OnOff);
BYTE ISPM VISCAAPP_GetLRReverseMode(void);

void VISCAAPP_FreezeOnOff(BOOL OnOff);

void VISCAAPP_PictureEffectOff(void);
void VISCAAPP_PictureEffectNeg(void);
void VISCAAPP_PictureEffectBW(void);

void VISCAAPP_Defog(BYTE val);

void VISCAAPP_PictureFlipOnOff(BOOL OnOff);
BYTE ISPM VISCAAPP_GetPictureFlipMode(void);

void VISCAAPP_ICROnOff(BOOL OnOff);
void VISCAAPP_AutoICROnOff(BOOL OnOff);
void VISCAAPP_AutoICROnOffThres(BYTE val);
void VISCAAPP_AutoICRDelay(BYTE val);
void VISCAAPP_ICRAlarmReplyOnOff(BOOL OnOff);
BYTE ISPM VISCAAPP_GetICRMode(void);
BYTE ISPM VISCAAPP_GetAutoICRMode(void);
BYTE VISCAAPP_GetAutoICRAlarm(void);
BYTE VISCAAPP_GetAutoICROnOffThres(void);
BYTE VISCAAPP_GetExAutoICROnOffThres(void);
BYTE VISCAAPP_GetExAutoICROffOnThres(void);
BYTE VISCAAPP_GetAutoICRDelay(void);

void VISCAAPP_StabilizerOnOff(BOOL OnOff);
void VISCAAPP_StabilizerHold(void);

void VISCAAPP_MemoryReset(BYTE no);
void VISCAAPP_MemorySet(BYTE no);
void VISCAAPP_MemoryRecall(BYTE no);

void VISCAAPP_CustomReset(void);
void VISCAAPP_CustomSet(void);
void VISCAAPP_CustomRecall(void);

void VISCAAPP_MemSave(BYTE addr, WORD data);

void VISCAAPP_DisplayOnOff(BOOL OnOff);
void VISCAAPP_DisplayToggle(void);

void VISCAAPP_MultiLineTitleSet1(BYTE line_no, BYTE h_posi, BYTE color, BYTE blink);
void VISCAAPP_MultiLineTitleSet2(BYTE line_no, PBYTE pTitle);
void VISCAAPP_MultiLineTitleSet3(BYTE line_no, PBYTE pTitle);
void VISCAAPP_MultiLineTitleClear(BYTE line_no);
void VISCAAPP_MultiLineTitleOn(BYTE line_no);
void VISCAAPP_MultiLineTitleOff(BYTE line_no);

void VISCAAPP_MuteOnOff(BOOL OnOff);
void VISCAAPP_MuteToggle(void);

void VISCAAPP_PMSetMask(BYTE mask_no, BYTE mode, BYTE w_size, BYTE h_size);
void VISCAAPP_PMDisplay(DWORD mask_onoff);
void VISCAAPP_PMSetMaskColor(DWORD mask_sel, BYTE color1, BYTE color2);
void VISCAAPP_PMSetPanTiltAngle(WORD pan, WORD tilt);
void VISCAAPP_PMSetPTZMask(BYTE mask_no, WORD pan, WORD tilt, WORD zoom);
void VISCAAPP_PMNonInterlockMask(BYTE mask_no, BYTE x, BYTE y, BYTE w, BYTE h);
void VISCAAPP_PMCenterLineOnOff(BOOL OnOff);

void VISCAAPP_CamIDWrite(WORD cam_id);

void VISCAAPP_MDOnOff(BOOL OnOff);
void VISCAAPP_MDFunctionSet(BYTE disp_mode, BYTE det_frame, BYTE thres, BYTE inter_time);
void VISCAAPP_MDWindowSet(BYTE no, BYTE lx, BYTE ly, BYTE rx, BYTE ry);
BYTE VISCAAPP_GetMDAlarm(void);
void VISCAAPP_GetMDWindow(BYTE no, PBYTE plx, PBYTE ply, PBYTE prx, PBYTE pry);

void VISCAAPP_ContZoomPosReplyOnOff(BOOL OnOff);
void VISCAAPP_ZoomPosReplyInterTimeSet(BYTE inter_time);
void VISCAAPP_ContFocusPosReplyOnOff(BOOL OnOff);
void VISCAAPP_FocusPosReplyInterTimeSet(BYTE inter_time);

void VISCAAPP_RegViscaBaudrate(BYTE val);
void VISCAAPP_RegMonitoringMode(BYTE val);
void VISCAAPP_RegOutputEnable(BYTE val);
void VISCAAPP_RegLVDSMode(BYTE val);
void VISCAAPP_RegZoomWideLimit(BYTE val);
void VISCAAPP_RegZoomTeleLimit(BYTE val);
void VISCAAPP_RegDZoomMax(BYTE val);
void VISCAAPP_RegStableZoom(BYTE val);
void VISCAAPP_RegFocusTraceZD(BYTE val);
void VISCAAPP_RegFocusOffsetDC(BYTE val);
void VISCAAPP_RegEnlargementMode(BYTE val);
void VISCAAPP_RegValue(BYTE addr, BYTE val);
BYTE VISCAAPP_GetRegViscaBaudrate(void);

void VISCAAPP_ColorEnhanceOnOff(BOOL OnOff);
void VISCAAPP_ColorEnhanceParam(BYTE thres, BYTE y_h, BYTE cr_h, BYTE cb_h, BYTE y_l, BYTE cr_l, BYTE cb_l);

void VISCAAPP_ChromaSuppress(BYTE level);

void VISCAAPP_ColorGain(BYTE val);
void VISCAAPP_ColorHue(BYTE val);

void VISCAAPP_GammaOffset(BYTE pol, BYTE val);

void VISCAAPP_ExExpCompReset(void);
void VISCAAPP_ExExpCompUp(BYTE step);
void VISCAAPP_ExExpCompDown(BYTE step);
void VISCAAPP_ExExpCompDirect(BYTE level);

void VISCAAPP_ExApertureReset(void);
void VISCAAPP_ExApertureUp(BYTE step);
void VISCAAPP_ExApertureDown(BYTE step);
void VISCAAPP_ExApertureDirect(BYTE level);

void VISCAAPP_ExAutoICROnOffThres(BYTE val);
void VISCAAPP_ExAutoICROffOnThres(BYTE val);

void VISCAAPP_ExColorGain(BYTE color, BYTE val);
void VISCAAPP_ExColorHue(BYTE color, BYTE val);

BYTE VISCAAPP_GetWB_ManGainR(void);
BYTE VISCAAPP_GetWB_ManGainB(void);
BYTE VISCAAPP_GetWB_Mode(void);
BYTE VISCAAPP_GetExposure_Mode(void);
BYTE VISCAAPP_GetShutterPosi(void);
BYTE VISCAAPP_GetAGCGain(void);
BYTE VISCAAPP_GetAutoSlowShutterOnOff(void);
BYTE VISCAAPP_GetExpCompOnOff(void);
BYTE VISCAAPP_GetExpCompLevel(void);
BYTE VISCAAPP_GetBacklightOnOff(void);
BYTE VISCAAPP_GetAperture(void);
BYTE VISCAAPP_GetExAperture(void);
BYTE VISCAAPP_GetWDROnOff(void);
BYTE VISCAAPP_GetDefogMode(void);
BYTE VISCAAPP_GetDefogLevel(void);

WORD VISCAAPP_GetFWVer(void);
BYTE VISCAAPP_GetVOUT_Frmt(void);

void VISCAAPP_DisplayProcess(void);
void VISCAAPP_MultiLineTitleProcess(void);

void VISCAAPP_PanTiltDriveUp(BYTE pan_speed, BYTE tilt_speed);
void VISCAAPP_PanTiltDriveDown(BYTE pan_speed, BYTE tilt_speed);
void VISCAAPP_PanTiltDriveLeft(BYTE pan_speed, BYTE tilt_speed);
void VISCAAPP_PanTiltDriveRight(BYTE pan_speed, BYTE tilt_speed);
void VISCAAPP_PanTiltDriveUpLeft(BYTE pan_speed, BYTE tilt_speed);
void VISCAAPP_PanTiltDriveUpRight(BYTE pan_speed, BYTE tilt_speed);
void VISCAAPP_PanTiltDriveDownLeft(BYTE pan_speed, BYTE tilt_speed);
void VISCAAPP_PanTiltDriveDownRight(BYTE pan_speed, BYTE tilt_speed);
void VISCAAPP_PanTiltDriveStop(BYTE pan_speed, BYTE tilt_speed);
void VISCAAPP_PanTiltDriveHome(void);
void VISCAAPP_PanTiltDriveReset(void);
void VISCAAPP_PanTiltDriveABSPosi(BYTE pan_speed, BYTE tilt_speed, WORD pan_posi, WORD tilt_posi);
void VISCAAPP_PanTiltDriveRelPosi(BYTE pan_speed, BYTE tilt_speed, WORD pan_posi, WORD tilt_posi);
void VISCAAPP_PanTiltLimitSetDownLeft(WORD pan_limit, WORD tilt_limit);
void VISCAAPP_PanTiltLimitSetUpRight(WORD pan_limit, WORD tilt_limit);
void VISCAAPP_PanTiltLimitClrDownLeft(WORD pan_limit, WORD tilt_limit);
void VISCAAPP_PanTiltLimitClrUpRight(WORD pan_limit, WORD tilt_limit);

void VISCAAPP_MenuNavi(BYTE key);
void VISCAAPP_MaintenanceMode(BYTE val);


#endif	/* __VISCA_H__ */
