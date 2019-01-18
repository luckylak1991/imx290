//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MISP100.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISP100_H__
#define		__MISP100_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__MISPTYPE_H__
#include	"misptype.h"
#endif
#ifndef		__BOARD_H__ 
#include	"board.h"
#endif
#ifndef		__MISPBUS_H__
#include	 "mispbus.h"
#endif
#ifndef		__MISPCSC_H__
#include	"mispcsc.h"
#endif
#ifndef		__MISPFC_H__
#include	 "mispfc.h"
#endif
#ifndef		__MISPINPUT_H__
#include	 "mispinput.h"
#endif
#ifndef		__MISPOUTPUT_H__
#include	 "mispoutput.h"
#endif
#ifndef		__MISP_CLK_H__
#include	 "mispclk.h"
#endif
#ifndef		__MISPHOST_H__
#include	 "misphost.h"
#endif
#ifndef		__MISP_SENSOR_TOP_H__
#include	"mispsensor_top.h"
#endif
#ifndef		__MISP_BAYER_H__
#include	"mispbayer.h"
#endif
#ifndef		__MISP_NR_H__
#include	"mispnr.h"
#endif
#ifndef		__MISP_DEMOSAIC_H__
#include	"mispdemosaic.h"
#endif
#ifndef		__MISP_COLORCORRECTION_H__
#include	"mispcolorcorrection.h"
#endif
#ifndef		__MISP_DPC_H__
#include	"mispdpc.h"
#endif
#ifndef		__MISP_MEMORY_MAP_H__
#include	"mispmemory_map.h"
#endif
#ifndef		__MISP_WDR_H__
#include	"mispwdr.h"
#endif
#ifndef		__MISP_DIS_H__  
#include	"mispdis.h"
#endif
#ifndef		__MISPOSD_H__
#include	"misposd.h"
#endif
#ifndef		__MISPGAC_H__
#include	"mispgac.h"
#endif
#ifndef		__MISPCUR_H__
#include	"mispcur.h"
#endif
#ifndef		__MISP_FD_H__
#include	"mispfd.h"
#endif
#ifndef		__MISPADC_H__
#include	"mispadc.h"
#endif
#ifndef		__MISPMDPM_H__
#include	"mispmdpm.h"
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
#ifndef		__MISP_TONE_H__
#include	"misptone.h"
#endif
#ifndef		__MISP_CAC_H__
#include	"mispcac.h"
#endif
#ifndef		__MISP_ROTATION_H__
#include	"misprotation.h"
#endif
#ifndef		__MISP_CSP_H__
#include	"mispcsp.h"
#endif
#endif


// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
typedef	struct
{
	DWORD	hw_info;		// system hw information(board, sensor, lens, dc iris)
	DWORD	sw_ver;			// system sw version
	DWORD	boot_cnt;		// system boot counter info([31:16]boot_ok_cnt(0~0xffff), [15:0]boot_cnt(0~0xffff))

}	MISP_SYSTEM_INFO, *PMISP_SYSTEM_INFO;

#define		MISP_SYSINFO_BOARD_MASK			0x000000FF
#define		MISP_SYSINFO_I5x0FPGA				0x00000000
#define		MISP_SYSINFO_I500EDK				0x00000001
#define		MISP_SYSINFO_I500REF				0x00000002
#define		MISP_SYSINFO_I510EDK_V10			0x00000003
#define		MISP_SYSINFO_I510REF_V10			0x00000004
#define		MISP_SYSINFO_I540EDK_V10			0x00000005
#define		MISP_SYSINFO_I540REF_V10			0x00000006
#define		MISP_SYSINFO_I550EDK_V10			0x00000007
#define		MISP_SYSINFO_I550REF_V10			0x00000008
#define		MISP_SYSINFO_I510_IMX290			0x00000009
#define		MISP_SYSINFO_I510EDK_V11			0x0000000A
#define		MISP_SYSINFO_I510REF_V134			0x0000000B
#define		MISP_SYSINFO_I540EDK_V11			0x0000000C
#define		MISP_SYSINFO_I540REF_V121			0x0000000D
#define		MISP_SYSINFO_I550EDK_V11			0x0000000E
#define		MISP_SYSINFO_I550REF_V121			0x0000000F
#define		MISP_SYSINFO_I540_4K60_V10			0x00000010
#define		MISP_SYSINFO_I550_USB_V10			0x00000011

#define		MISP_SYSINFO_SENSOR_NO_MASK			0x0000FF00
#define		MISP_SYSINFO_SENSOR_IMX136			0x00000000
#define		MISP_SYSINFO_SENSOR_MN34220			0x00000100
#define		MISP_SYSINFO_SENSOR_IMX236			0x00000200
#define		MISP_SYSINFO_SENSOR_IMX124			0x00000300
#define		MISP_SYSINFO_SENSOR_IMX222			0x00000400
#define		MISP_SYSINFO_SENSOR_IMX185			0x00000500
#define		MISP_SYSINFO_SENSOR_IMX238			0x00000600
#define		MISP_SYSINFO_SENSOR_IMX174			0x00000700
#define		MISP_SYSINFO_SENSOR_AR0331			0x00000800
#define		MISP_SYSINFO_SENSOR_MT9P031			0x00000900
#define		MISP_SYSINFO_SENSOR_MT9M131			0x00000A00
#define		MISP_SYSINFO_SENSOR_MN34041			0x00000B00
#define		MISP_SYSINFO_SENSOR_IMX076			0x00000C00
#define		MISP_SYSINFO_SENSOR_PO3100K			0x00000D00
#define		MISP_SYSINFO_SENSOR_PS1210K			0x00000E00
#define		MISP_SYSINFO_SENSOR_IMX290			0x00000F00
#define		MISP_SYSINFO_SENSOR_IMX322			0x00001000
#define		MISP_SYSINFO_SENSOR_IMX178			0x00001100
#define		MISP_SYSINFO_SENSOR_IMX226			0x00001200
#define		MISP_SYSINFO_SENSOR_EXT2				0x00001300
#define		MISP_SYSINFO_SENSOR_EXT1				0x00001400
#define		MISP_SYSINFO_SENSOR_OV2710			0x00001500
#define		MISP_SYSINFO_SENSOR_IMX274			0x00001600
#define		MISP_SYSINFO_SENSOR_IMX265			0x00001700
#define		MISP_SYSINFO_SENSOR_MN34420			0x00001800
#define		MISP_SYSINFO_SENSOR_IMX326			0x00001900
#define		MISP_SYSINFO_SENSOR_IMX253			0x00001A00
#define		MISP_SYSINFO_SENSOR_PS1400K			0x00001B00
#define		MISP_SYSINFO_SENSOR_AR0134			0x00001C00
#define		MISP_SYSINFO_SENSOR_EXT3				0x00001D00
#define		MISP_SYSINFO_SENSOR_IMX323			0x00001E00
#define		MISP_SYSINFO_SENSOR_IMX252			0x00001F00
#define		MISP_SYSINFO_SENSOR_IMX385			0x00002000
#define		MISP_SYSINFO_SENSOR_IMX327			0x00002100
#define		MISP_SYSINFO_SENSOR_IMX273			0x00002200
#define		MISP_SYSINFO_SENSOR_IMX334			0x00002300
#define		MISP_SYSINFO_SENSOR_EXT2IPP			0x00002400

#define		MISP_SYSINFO_SENSOR_OUTPUT_MASK		0x000F0000
#define		MISP_SYSINFO_SENSOR_LVDS4				0x00000000
#define		MISP_SYSINFO_SENSOR_LVDS2				0x00010000
#define		MISP_SYSINFO_SENSOR_LVDSP				0x00020000
#define		MISP_SYSINFO_SENSOR_TTL				0x00030000
#define		MISP_SYSINFO_SENSOR_LVDS6				0x00040000
#define		MISP_SYSINFO_SENSOR_LVDS8				0x00050000

#define		MISP_SYSINFO_DCIRIS_MASK                0x00F00000
#define		MISP_SYSINFO_DCIRIS_HALL                0x00000000
#define		MISP_SYSINFO_DCIRIS_DAMP                0x00100000
#define		MISP_SYSINFO_DCIRIS_NONE                0x00200000
#define		MISP_SYSINFO_PWMIRIS_ONLY               0x00300000
#define		MISP_SYSINFO_P_IRIS_ONLY               0x00400000

#define		MISP_SYSINFO_LENS_TYPE_MASK             0xFF000000
#define		MISP_SYSINFO_LENS_OLPF_DAIWON           0x00000000
#define		MISP_SYSINFO_LENS_OLPF_YOPTIC           0x01000000
#define		MISP_SYSINFO_LENS_X30X_TAMRON           0x02000000
#define		MISP_SYSINFO_LENS_OLPF_CANON            0x03000000
#define		MISP_SYSINFO_LENS_OLPF_SUNNY            0x04000000
#define		MISP_SYSINFO_LENS_X03X_ZMAX            0x05000000
#define		MISP_SYSINFO_LENS_X10X_UNION            0x06000000
#define		MISP_SYSINFO_LENS_X10X_ZMAX             0x07000000
#define		MISP_SYSINFO_LENS_X12X_UNION             0x08000000
#define		MISP_SYSINFO_LENS_YT3010_2MOTOR            0x09000000

#define		MISP_SYSINFO_LENS_MANU_RV4515           0x10000000
#define		MISP_SYSINFO_LENS_MANU_RV0850           0x11000000
#define		MISP_SYSINFO_LENS_MANU_KAWADEN12120     0x12000000
#define		MISP_SYSINFO_LENS_MANU_RV1530           0x13000000
#define		MISP_SYSINFO_LENS_MANU_RX8FM1214C       0x14000000
#define		MISP_SYSINFO_LENS_MANU_TAMRON_5M        0x15000000
#define		MISP_SYSINFO_LENS_MANU_TAMRON           0x16000000
#define		MISP_SYSINFO_LENS_MANU_TAMRON_PWM       0x17000000
#define		MISP_SYSINFO_LENS_MANU_SUNEX115         0x18000000
#define		MISP_SYSINFO_LENS_MANU_RV3620           0x19000000
#define		MISP_SYSINFO_LENS_MANU_RV3220           0x1A000000
#define		MISP_SYSINFO_LENS_MANU_BW3M30B4000      0x1B000000
#define		MISP_SYSINFO_LENS_MANU_BW4K40_1000      0x1C000000
#define		MISP_SYSINFO_LENS_MANU_BW3ML43B_1000    0x1D000000
#define		MISP_SYSINFO_LENS_MANU_RV10_FM1614A1    0x1E000000
#define		MISP_SYSINFO_LENS_MANU_WW_4KUHD      	0x1F000000
#define		MISP_SYSINFO_LENS_MANU_WW_8mmIR      	0x20000000



#define		MISP_SYSINFO_SW_SVN_MASK				0x0000FFFF		// svn revision no.


#define		M_SYSINFO_INIT ((volatile unsigned int*)(0x20000100))	// system debug area in dspm
#define		M_SYSINFO_HW_INFO ((volatile unsigned int*)(0x20000104))	// system h/w info
#define		M_SYSINFO_SW_VER ((volatile unsigned int*)(0x20000108))	// system s/w version

#define		MISP_SYSINFO_INIT_CPU_COMPLETE       	0x00000001		// cpu initial o.k
#define		MISP_SYSINFO_INIT_SENSOR_COMPLETE    	0x00000002		// sensor initial o.k
#define		MISP_SYSINFO_INIT_LENS_COMPLETE      	0x00000004		// lens initial o.k
#define		MISP_SYSINFO_INIT_ISP_COMPLETE       	0x00000008		// isp initial o.k


// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------
extern MISP_SYSTEM_INFO	stSystemInfo;

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void MISP_Initialize(void);
void SYSINFO_initialize(void);

void Init_ODM(void);

void MISP_SW_Reset(void) ;

DWORD GetSYSINFO_BoardType(void);
DWORD GetSYSINFO_SensorNo(void);
DWORD GetSYSINFO_SensorOutType(void);
DWORD GetSYSINFO_LensType(void);
DWORD GetSYSINFO_IrisType(void);
DWORD GetSYSINFO_SWVer(void);
DWORD GetSYSINFO_BootCnt(void);

void SetSYSINFO_CpuInitDone(void);
void SetSYSINFO_IspInitDone(void);
void SetSYSINFO_SensorInitDone(void);
void SetSYSINFO_CopyHwInfo(DWORD val);
void SetSYSINFO_CopySwVer(DWORD val);

void SetTIMER_PWM5LowWidth(BYTE ms);
void SetTIMER_PWM5HighWidth(BYTE ms);

void MISP_Reboot(void);

#endif	/* __MISP100_H__ */
