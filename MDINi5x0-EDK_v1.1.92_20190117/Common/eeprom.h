//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2015  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  EEPROM.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__EEPROM_H__
#define		__EEPROM_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
//#ifndef		__MISPTYPE_H__
//#include	"misptype.h"
//#endif

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#if defined(__MDIN_i510_IMX290__)
#define		EEPROM_I2C_CH			TWI_CH0
#else
#define		EEPROM_I2C_CH			TWI_CH1
#endif



#if defined (__USE_EEPROM__)


//
// eeprom address map
//

// system info ( 0x0000 ~ 0x001F )
#define 	EEPROM_HW_INFO 			0x0000 		// 0x0000 ~ 0x0003
#define 	EEPROM_SW_VER 			0x0004 		// 0x0004 ~ 0x0007
#define 	EEPROM_SW_DATE 			0x0008 		// 0x0008 ~ 0x0013
#define 	EEPROM_SW_TIME 			0x0014 		// 0x0014 ~ 0x001C


// pint info ( 0x0020 ~ 0x003F )
#define 	EEPROM_PINT_LENSNO		0x0020 		// 0x0020 ~ 0x0021
#define		EEPROM_PINT_ZOOM		0x0024 		// 0x0024 ~ 0x002F
#define 	EEPROM_PINT_FOCUS		0x0032 		// 0x0032 ~ 0x003D


// gamma table ( 0x0040 ~ 0x0061 )
#define		EEPROM_GAMMA_TABLE		0x0040 		// 0x0040 ~ 0x005F
#define 	EEPROM_GAMMA_FLAG		0x0060

// factory setup ( 0x0070 ~ 0x007F )
#define     EEPROM_FACTORY_SETUP        0x0070
#define     EEPROM_EXPOSURE_IRIS_OFFSET 0x0070		// IRIS offset value
#define     EEPROM_EXPOSURE_IRIS_ADC    0x0072		// IRIS ADC value
#define     EEPROM_EXPOSURE_IRISSPD     0x0074		// pwm iris speed



#if defined (EEPROM_SIZE_OVER_16KBIT)


// white balance #1 ( 0x0100 ~ 0x027F )
#define 	EEPROM_COMP_ID			0x0100
#define 	EEPROM_COMP_NDR			0x0104
#define 	EEPROM_COMP_WDR			0x0114
#define 	EEPROM_COMP_HTM			0x0124

#define 	EEPROM_POLY_ID			0x0130
#define 	EEPROM_POLY_ATW			0x0138
#define 	EEPROM_POLY_OUT			0x0198
#define 	EEPROM_POLY_IN			0x01f8
#define 	EEPROM_POLY_UV			0x0250


// cc table ( 0x0280 ~ 0x0431 )
#define		EEPROM_CC_TABLE			0x0280 		// 0x0280 ~ 0x042F
#define 	EEPROM_CC_FLAG 			0x0430 		// 0x0430 ~ 0x0431


// menu item #1 - main menu ( 0x0500 ~ 0x05FF )
#define		EEPROM_MAIN_MENU		0x0500
#define		EEPROM_FOCUS_MENU		0x0500
#define		EEPROM_FOCUS_MODE		0x0500
#define		EEPROM_FOCUS_WMAX		0x0502
#define		EEPROM_FOCUS_TMAX		0x0504
#define		EEPROM_FOCUS_ZOOMSPD	0x0506
#define		EEPROM_FOCUS_FOKSLIMIT	0x0508
#define		EEPROM_FOCUS_INTERVAL	0x050A
#define		EEPROM_EXPOSURE_MENU	0x0510
#define		EEPROM_EXPOSURE_MODE	0x0510
#define		EEPROM_EXPOSURE_SHUT	0x0512		// 0x0512 ~ 0x0517
#define		EEPROM_EXPOSURE_IRIS	0x0518		// 0x0518 ~ 0x051D
#define		EEPROM_EXPOSURE_AGC		0x051E		// 0x051E ~ 0x0523
#define		EEPROM_EXPOSURE_AGCMAX	0x0524		// 0x0524 ~ 0x0529
#define		EEPROM_EXPOSURE_SENSUP	0x052A		// 0x052A ~ 0x052F
#define		EEPROM_EXPOSURE_BRIGHT	0x0530		// 0x0530 ~ 0x0535
//#define		EEPROM_EXPOSURE_IRISSPD	0x0536
#define		EEPROM_EXPOSURE_LENSMOD	0x0538
#define		EEPROM_EXPOSURE_FLKMOD	0x053A
#define		EEPROM_WB_MENU			0x0540
#define		EEPROM_WB_MODE			0x0540
#define		EEPROM_WBMAN_BLUE		0x0542
#define		EEPROM_WBMAN_RED		0x0544
#define		EEPROM_WB_SPEED			0x0546
#define		EEPROM_WBAUTO_BLUE		0x0548
#define		EEPROM_WBAUTO_RED		0x054A
#define		EEPROM_DYNT_MENU		0x0550
#define		EEPROM_DYNT_MODE		0x0550
#define		EEPROM_DYNT_DELAY		0x0552
#define		EEPROM_DYNT_D2N			0x0554
#define		EEPROM_DYNT_N2D			0x0556
#define		EEPROM_DYNT_NTCOLOR		0x0558
#define		EEPROM_DYNT_CBURST		0x055A
#define		EEPROM_PM_MENU			0x0560
#define		EEPROM_PM_MODE			0x0560
#define		EEPROM_PM_TYPE			0x0562
#define		EEPROM_PM_RECTDISP		0x0564
#define		EEPROM_PM_POLYDISP		0x0566
#define		EEPROM_PM_RECTCOLOR		0x0568		// 0x0568 ~ 0x056D
#define		EEPROM_PM_POLYCOLOR		0x056E		// 0x056E ~ 0x0573
#define		EEPROM_EFFECT_MENU 		0x0580
#define		EEPROM_EFFECT_LSC 		0x0580
#define		EEPROM_EFFECT_GAMMA 	0x0582
#define		EEPROM_EFFECT_CONTRAST 	0x0584
#define		EEPROM_EFFECT_BRIGHT 	0x0586
#define		EEPROM_EFFECT_SAT 		0x0588
#define		EEPROM_EFFECT_HUE 		0x058A
#define		EEPROM_EFFECT_EDGEENH 	0x058C
#define		EEPROM_EFFECT_AUTOSAT 	0x058E
#define		EEPROM_EFFECT_AUTOEDGE 	0x0590
#define		EEPROM_EFFECT_AUTOBRHT 	0x0592

#define		EEPROM_SPECIAL_MENU 	0x05A0
#define		EEPROM_SPECIAL_DZOOM 	0x05A0
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)
#define		EEPROM_SPECIAL_DEFOGDWDR 0x05A2
#else
#define		EEPROM_SPECIAL_DEFOG 	0x05A4
#define		EEPROM_SPECIAL_DWDR 	0x05A6
#endif
#define		EEPROM_SPECIAL_MD 		0x05A8
#define		EEPROM_SPECIAL_DIS 		0x05AA
#define		EEPROM_SPECIAL_BLIGHT 	0x05AC
#define		EEPROM_SPECIAL_BIN 		0x05AE
#define		EEPROM_SPECIAL_DPC 		0x05B0
#define		EEPROM_SPECIAL_WDR 		0x05B2
#define		EEPROM_SPECIAL_FLK		0x05B4

#define		EEPROM_SETUP_MENU 		0x05C0
#define		EEPROM_SETUP_CTITLE 	0x05C0
#define		EEPROM_SETUP_NTPAL 		0x05C2
#define		EEPROM_SETUP_LANG 		0x05C4
#define		EEPROM_SETUP_MPOSI 		0x05C6
#define		EEPROM_SETUP_OUTFRMT 	0x05C8

#define		EEPROM_TWOMOTOR_MENU	0x05D0
#define		EEPROM_TWOMOTOR_AFMODE	0x05D0
#define		EEPROM_TWOMOTOR_SCAN	0x05D2
#define		EEPROM_TWOMOTOR_SYNCTDN	0x05D4

#define		EEPROM_DNR_MENU			0x05E0
#define		EEPROM_DNR_MODE			0x05E0
#define		EEPROM_DNR_LEVEL		0x05E2
#define		EEPROM_DNR_APERTURE		0x05E4


// menu item #2 - sub menu ( 0x0600 ~ 0x06FF )
#define		EEPROM_SUB_MENU			0x0600
#define		EEPROM_CONFER_MENU		0x0600
#define		EEPROM_CONFER_MODE		0x0600
#define		EEPROM_CONFER_WIDTH		0x0602
#define		EEPROM_CONFER_HEIGHT	0x0604
#define		EEPROM_CONFER_POSIX		0x0606
#define		EEPROM_CONFER_POSIY		0x0608
#define		EEPROM_DZOOM_MENU		0x0610
#define		EEPROM_DZOOM_DZRATIO	0x0610
#define		EEPROM_DZOOM_DZPOSI		0x0612		// 0x0612 ~ 0x0615
#define		EEPROM_DZOOM_PIPONOFF	0x0616
#define		EEPROM_DZOOM_PIPSIZE	0x0618
#define		EEPROM_DZOOM_PIPPOSI	0x061A		// 0x061A ~ 0x061D
#define		EEPROM_DEFOG_MENU		0x0620
#define		EEPROM_DEFOG_STRENGTH	0x0620
#define		EEPROM_DEFOG_BRIGHT		0x0622
#define		EEPROM_BLC_MENU			0x0624
#define		EEPROM_BLC_MODE			0x0624
#define		EEPROM_BLC_WINDISP		0x0626
#define		EEPROM_BLC_WINPOSI		0x0628		// 0x0628 ~ 0x062B
#define		EEPROM_BLC_WINSIZE		0x062C		// 0x062C ~ 0x062F
#define		EEPROM_HLC_MENU			0x0630
#define		EEPROM_HLC_WINDISP		0x0630
#define		EEPROM_HLC_MASKLEVEL	0x0632
#define		EEPROM_HLC_MASKDISP		0x0634
#define		EEPROM_HLC_WINPOSI		0x0636		// 0x0636 ~ 0x0639
#define		EEPROM_HLC_WINSIZE		0x063A		// 0x063A ~ 0x063D
#define		EEPROM_DPC_MENU			0x0640
#define		EEPROM_DPC_SDPDGAIN		0x0640
#define		EEPROM_DISP_MENU		0x0642
#define		EEPROM_DISP_FREEZE		0x0642
#define		EEPROM_DISP_MIRROR		0x0644
#define		EEPROM_DISP_EFFECT		0x0646
#define		EEPROM_CTITLE_MENU		0x0650
#define		EEPROM_CTITLE_TEXT		0x0650		// text : 0x0650 ~ 0x065A,  reserved : 0x065B ~ 0x066F
#define		EEPROM_CTITLE_COLOR		0x0670
#define		EEPROM_MCOLOR_MENU		0x0672
#define		EEPROM_MCOLOR_TEXT		0x0672
#define		EEPROM_MCOLOR_TEXTH		0x0674
#define		EEPROM_MCOLOR_BG		0x0676
#define		EEPROM_MCOLOR_BGH		0x0678


#if defined(__USE_DEFOG_DWDR_ONE_MENU__)	
#define		EEPROM_DEFOGDWDR_MENU		0x0680
#define		EEPROM_DEFOGDWDR_STRENGTH	0x0680
#define		EEPROM_DEFOGDWDR_BRIGHT		0x0682
#define		EEPROM_DEFOGDWDR_SATURATION	0x0684
#define		EEPROM_DEFOGDWDR_LOCALRATIO	0x0686
#define		EEPROM_DEFOGDWDR_SATSYNC	0x0688

#else

#define		EEPROM_DWDR_MENU		0x0690
#define		EEPROM_DWDR_STRENGTH    0x0690
#define		EEPROM_DWDR_SATURATION  0x0692
#define		EEPROM_DWDR_LOCALRATIO  0x0694
#define		EEPROM_DWDR_SATSYNC     0x0696
#define		EEPROM_DWDR_AUTO_LEVEL  0x0698
#define		EEPROM_DEFOG_AUTO_LEVEL 0x069A

#endif


#define		EEPROM_MD_MENU			0x06A0
#define		EEPROM_MD_AREAEN		0x06A0
#define		EEPROM_MD_AREADISP		0x06A2
#define		EEPROM_MD_THRESHOLD		0x06A4
#define		EEPROM_MD_MOTIONVIEW	0x06A6
#define		EEPROM_MD_SIZEPOSI		0x06A8		// 0x06A8 ~ 0x06BF

#define		EEPROM_RS485_ID			0x06C0
#define		EEPROM_RS485_DISP		0x06C2
#define		EEPROM_RS485_BAUD		0x06C4

//#define		EEPROM_OUT_FRMT			0x06F0
#define		EEPROM_SS_FPS			0x06F2
#define		EEPROM_AE_LENS			0x06F4
#define		EEPROM_HDMITX_MODE		0x06F6
#define		EEPROM_BOOT_CNT			0x06F8		// 0x06F8 ~ 0x06FD


// menu item #3 - pm size/posi ( 0x0700 ~ 0x07FF )
#define		EEPROM_PM_SIZEPOSI		0x0700
#define		EEPROM_PM_RECTPOSI		0x0700 		// 0x0700 ~ 0x071E
#define		EEPROM_PM_RECTSIZE		0x0720 		// 0x0720 ~ 0x073E
#define		EEPROM_PM_POLYPOSI		0x0740 		// 0x0740 ~ 0x07FF


// menu item #4 - misc ( 0x0800 ~ 0x08FF )
#define		EEPROM_DDPC_PARAM		0x0800		// 0x0800 ~ 0x081F
#define		EEPROM_PICTURE_STYLE	0x0820		// 0x0820 ~ 0x0832
#define		EEPROM_CAC				0x0840		// 0x0840 ~ 0x087F


// menu item #5 - reserved ( 0x0900 ~ 0x0AFF )
#define		EEPROM_RESERVED_AREA	0x0900		// 0x0900 ~ 0x0AFF


// RGB gamma table ( 0x0B00 ~ 0x0FFF )
#define		EEPROM_RGBGAMMA_TABLE	0x0B00 		// 0x0B00 ~ 0x0EFF
#define 	EEPROM_RGBGAMMA_FLAG	0x0F00		// 0x0F00 ~ 0x0F01

#if defined(__CASCADE_CAM_SETUP__)
#define		EEPROM_USER_AREA			0x1000		// 0x1000 ~ 0x1FFF
#define		EEPROM_CASCADE_MAIN		0x1000		// 0x1000 ~ 0x1010
#define		EEPROM_CASCADE_AUX		0x1020		// 0x1020 ~ 0x103F
#define		EEPROM_CASCADE_BLEND		0x1040		// 0x1040 ~ 0x104F
#define		EEPROM_CASCADE_WB		0x1050		// 0x1050 ~ 0x105F
#else
// user area - application parameter ( 0x1000 ~ 0x6FFF )
#define		EEPROM_USER_AREA		0x1000		// 0x1000 ~ 0x1FFF
#define		EEPROM_PANO_MAIN		0x1000		// 0x1000 ~ 0x1010
#define		EEPROM_PANO_AUX			0x1020		// 0x1020 ~ 0x103F
#define		EEPROM_PANO_BLEND		0x1040		// 0x1040 ~ 0x104F
#define		EEPROM_PANO_WB			0x1050		// 0x1050 ~ 0x105F
#endif

// visca area ( 0x7000 ~ 0x73FF )
#define		EEPROM_VISCA_AREA		0x7000		// 0x7000 ~ 0x73FF
#define		EEPROM_VISCA_MEMORY		0x7000		// 0x7000 ~ 0x71FF(32 x 16)
#define		EEPROM_VISCA_MEMSAVE	0x7200		// 0x7200 ~ 0x7217(3 x 8)
#define		EEPROM_VISCA_CUSTOM		0x7220		// 0x7220 ~ 0x729F(128 ?)


// white balance #2
#define		EEPROM_POLY_RAW			0x7780		// 0x7780 ~ 0x77FF
#define 	EEPROM_TIME_UV			0x7800 		// 0x7800 ~ 0x7FFF


#endif	// EEPROM_SIZE_OVER_16KBIT



// eeprom flag
#define 	EEPROM_PINT_DONE		0xAF01
#define 	EEPROM_COMP_DONE		0xAC02
#define 	EEPROM_POLY_DONE		0xAC03

#define 	EEPROM_GAMMA_DONE		0xA511
#define 	EEPROM_CC_DONE			0xA50A

#define 	EEPROM_SAVE_DONE		0xB4


#endif	// __USE_EEPROM__

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

#endif	/* __EEPROM_H__ */
