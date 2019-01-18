//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  deadpix.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISP_DPC_H__  
#define		__MISP_DPC_H__  

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__CMOS_IF_H__
#include	"cmos_if.h"		// for sensor dependent option
#endif

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

//Flash sector use(i500) : 4byte(x&y 2word) x 1000ea < 4kbyte
//Flash sector use(i5x0) : 4byte(x&y 2word) x 2000ea + 2kbyte(for byr_dpc_mode) < 10kbyte

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
#define		__USE_DPD_i51X__
#endif

#if defined(__USE_DPD_i51X__)
//#define		DPD_COUNT_CUT_USE				//comment is no use
#define		NH_SIZ					2000			//nomal High - MCU DPC buffer  Size
#define		SDPC_XOFFSET			1			//static dpc x offset (dpd - offset = dpc memory)		Y-offset is 
#define		SDPD_AGC_START		0	
#define		SDPD_AGC_MAX			7	
#define		DPD_SENSOR_WAIT		10			
#define		DPD_FLASH_SECTOR_ADDR      ((FLASH_SIZE_KB/4) -3)     // flash sector addr 253,254,255  (flash size / 4) = sector size. 
#define		DPD_FLASH_TEMP_SECTOR_ADDR      ((FLASH_SIZE_KB/4) -6)     // flash sector addr 250,251,252  (flash size / 4) = sector size. 
#define		DPD_FLASH_SECTOR2_ADDR      ((FLASH_SIZE_KB/4) -9)     // flash sector addr 247,248,249  (flash size / 4) = sector size. 
#else
#define		DPD_COUNT_CUT_USE				//comment is no use
#define		NH_SIZ					1024			//nomal High - MCU DPC buffer  Size
#define		SDPC_XOFFSET			1			//static dpc x offset (dpd - offset = dpc memory)		Y-offset is 
#define		DPD_SENSOR_WAIT		20			
#define		DPD_FLASH_SECTOR_ADDR      ((FLASH_SIZE_KB/4) -3)     // dpd sector addr
#define		DPD_FLASH_TEMP_SECTOR_ADDR      ((FLASH_SIZE_KB/4) -6)     // flash sector addr 250,251,252  (flash size / 4) = sector size. 
#define		DPD_FLASH_SECTOR2_ADDR      ((FLASH_SIZE_KB/4) -9)     // flash sector addr 247,248,249  (flash size / 4) = sector size. 
#endif

#if defined(__USE_DPD_i51X__)
#define SDPD_AGC_LV0	256		//0db

#define SDPD_THHD_LV0	80		//0db
#if defined(__USE_CMOS_IMX185__)
#define SDPD_AGC_LV1	2000	//35db
#define SDPD_AGC_LV2	4000	//30db
#define SDPD_AGC_LV3	8000	//30db
#define SDPD_AGC_LV4	12000	//36db
#define SDPD_AGC_LV5	20000	//36db
#define SDPD_AGC_LV6	30000	//42db
#define SDPD_AGC_LV7	40000	//50db

#define SDPD_THHD_LV1	80		//26db
#define SDPD_THHD_LV2	100		//26db
#define SDPD_THHD_LV3	120		//30db
#define SDPD_THHD_LV4	240		//36db
#define SDPD_THHD_LV5	400		//42db
#define SDPD_THHD_LV6	750		//46db
#define SDPD_THHD_LV7	1000	//50db
#elif defined(__USE_CMOS_IMX385__)
#define SDPD_AGC_LV1	1000	//26db
#define SDPD_AGC_LV2	2000	//35db
#define SDPD_AGC_LV3	4000	//26db
#define SDPD_AGC_LV4	8000	//35db
#define SDPD_AGC_LV5	10000	//40db
#define SDPD_AGC_LV6	15000	//46db
#define SDPD_AGC_LV7	20000	//50db

#define SDPD_THHD_LV1	80		//26db
#define SDPD_THHD_LV2	100		//26db
#define SDPD_THHD_LV3	120		//26db
#define SDPD_THHD_LV4	250		//35db
#define SDPD_THHD_LV5	500		//40db
#define SDPD_THHD_LV6	750		//46db
#define SDPD_THHD_LV7	1000	//50db
#elif defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__) || defined(__USE_CMOS_IMX226__)
#define SDPD_AGC_LV1	4000	//4db
#define SDPD_AGC_LV2	8000	//6db
#define SDPD_AGC_LV3	24000	//14db
#define SDPD_AGC_LV4	50000	//14db
#define SDPD_AGC_LV5	100000	//26db
#define SDPD_AGC_LV6	120000	//25db
#define SDPD_AGC_LV7	131000	//27db

#define SDPD_THHD_LV1	90		//30db
#define SDPD_THHD_LV2	100		//30db
#define SDPD_THHD_LV3	150		//36db
#define SDPD_THHD_LV4	250		//40db
#define SDPD_THHD_LV5	500		//40db
#define SDPD_THHD_LV6	600		//46db
#define SDPD_THHD_LV7	800	//27db
#elif defined(__USE_CMOS_IMX334__) 
#define SDPD_AGC_LV1	800		//26db
#define SDPD_AGC_LV2	1500	//26db
#define SDPD_AGC_LV3	2500	//26db
#define SDPD_AGC_LV4	5000	//35db
#define SDPD_AGC_LV5	8000	//40db
#define SDPD_AGC_LV6	12000	//46db
#define SDPD_AGC_LV7	15000	//50db

#define SDPD_THHD_LV1	80		//26db
#define SDPD_THHD_LV2	100		//26db
#define SDPD_THHD_LV3	120		//26db
#define SDPD_THHD_LV4	250		//35db
#define SDPD_THHD_LV5	500		//40db
#define SDPD_THHD_LV6	750		//46db
#define SDPD_THHD_LV7	1000	//50db
#elif defined(__USE_CMOS_EXT1__)
#define SDPD_AGC_LV1	1600	 //16db
#define SDPD_AGC_LV2	2560	 //20db
#define SDPD_AGC_LV3	4096	 //24db
#define SDPD_AGC_LV4	6048	//36db
#define SDPD_AGC_LV5	8000	//36db
#define SDPD_AGC_LV6	10000	//42db
#define SDPD_AGC_LV7	12000	//42db

#define SDPD_THHD_LV1	120		//30db
#define SDPD_THHD_LV2	240		//36db
#define SDPD_THHD_LV3	400		//42db
#define SDPD_THHD_LV4	500		//46db
#define SDPD_THHD_LV5	700		//46db
#define SDPD_THHD_LV6	800		//50db
#define SDPD_THHD_LV7	1000	//50db
#elif defined(__USE_CMOS_MN34420__)
//mn34420
#define SDPD_AGC_LV1	2000	//24db
#define SDPD_AGC_LV2	4000	//24db
#define SDPD_AGC_LV3	8000	//36db
#define SDPD_AGC_LV4	10000	//42db
#define SDPD_AGC_LV5	12000	//42db
#define SDPD_AGC_LV6	15000	//46db
#define SDPD_AGC_LV7	20000	//50db

#define SDPD_THHD_LV1	80		//30db 
#define SDPD_THHD_LV2	100		//30db
#define SDPD_THHD_LV3	200		//36db
#define SDPD_THHD_LV4	300		//40db
#define SDPD_THHD_LV5	400		//40db
#define SDPD_THHD_LV6	750		//46db
#define SDPD_THHD_LV7	1000	//50db
#elif defined(__USE_IMX290_HCG_MODE__)
#define SDPD_AGC_LV1	1500	//26db
#define SDPD_AGC_LV2	3000	//26db
#define SDPD_AGC_LV3	6000	//35db
#define SDPD_AGC_LV4	9000	//40db
#define SDPD_AGC_LV5	12000	//40db
#define SDPD_AGC_LV6	15000	//46db
#define SDPD_AGC_LV7	20000	//50db

#define SDPD_THHD_LV1	100		//26db
#define SDPD_THHD_LV2	150		//26db
#define SDPD_THHD_LV3	250		//35db
#define SDPD_THHD_LV4	500		//35db
#define SDPD_THHD_LV5	800		//40db
#define SDPD_THHD_LV6	1000		//46db
#define SDPD_THHD_LV7	1500	//50db
#elif defined(__USE_CMOS_IMX265__) || defined(__USE_CMOS_IMX252__) || defined(__USE_CMOS_IMX253__) || defined(__USE_CMOS_IMX273__)
#define SDPD_AGC_LV1	2000	//6db
#define SDPD_AGC_LV2	4000	//6db
#define SDPD_AGC_LV3	8000	//6db
#define SDPD_AGC_LV4	24000	//14db
#define SDPD_AGC_LV5	80000	//26db
#define SDPD_AGC_LV6	100000	//25db
#define SDPD_AGC_LV7	130000	//27db

#define SDPD_THHD_LV1	80		//30db
#define SDPD_THHD_LV2	100		//30db
#define SDPD_THHD_LV3	120		//30db
#define SDPD_THHD_LV4	250		//36db
#define SDPD_THHD_LV5	500		//40db
#define SDPD_THHD_LV6	800		//46db
#define SDPD_THHD_LV7	1200	//50db
#else
#define SDPD_AGC_LV1	1000	//24db
#define SDPD_AGC_LV2	2000	//24db
#define SDPD_AGC_LV3	4000	//24db
#define SDPD_AGC_LV4	8000	//36db
#define SDPD_AGC_LV5	12000	//42db
#define SDPD_AGC_LV6	15000	//46db
#define SDPD_AGC_LV7	20000	//50db

#define SDPD_THHD_LV1	80		//30db 
#define SDPD_THHD_LV2	100		//30db
#define SDPD_THHD_LV3	200		//36db
#define SDPD_THHD_LV4	300		//40db
#define SDPD_THHD_LV5	400		//40db
#define SDPD_THHD_LV6	750		//46db
#define SDPD_THHD_LV7	1000	//50db
#endif
#else		//i500
#if defined(__USE_IMX290_HCG_MODE__)
#define SDPD_AGC_LV0	256
#define SDPD_AGC_LV1	3000	 
#define SDPD_AGC_LV2	4000	 
#define SDPD_AGC_LV3	6000	 
#define SDPD_AGC_LV4	8000	 
#define SDPD_AGC_LV5	10000	 
#define SDPD_AGC_LV6	12000	 
#define SDPD_AGC_LV7	15000	 

#define SDPD_THHD_LV0	6		 
#define SDPD_THHD_LV1	12		
#define SDPD_THHD_LV2	32		
#define SDPD_THHD_LV3	96		
#define SDPD_THHD_LV4	120	
#define SDPD_THHD_LV5	150	
#define SDPD_THHD_LV6	180	
#define SDPD_THHD_LV7	200	
#elif defined(__USE_CMOS_IMX174__) ||defined(__USE_CMOS_IMX249__)		//test end 160728 60fps mode
#define SDPD_AGC_LV0	256
#define SDPD_AGC_LV1	4208	 //4db
#define SDPD_AGC_LV2	8416	 //6db
#define SDPD_AGC_LV3	10524	 //8db
#define SDPD_AGC_LV4	12000	 //8db
#define SDPD_AGC_LV5	16000	 //8db
#define SDPD_AGC_LV6	20000	 //8db
#define SDPD_AGC_LV7	24000	 //8db

#define SDPD_THHD_LV0	2
#define SDPD_THHD_LV1	3		
#define SDPD_THHD_LV2	4		
#define SDPD_THHD_LV3	6
#define SDPD_THHD_LV4	10
#define SDPD_THHD_LV5	20
#define SDPD_THHD_LV6	30
#define SDPD_THHD_LV7	40
#else
#define SDPD_AGC_LV0	256
#define SDPD_AGC_LV1	4000	 
#define SDPD_AGC_LV2	6000	 
#define SDPD_AGC_LV3	8000	 
#define SDPD_AGC_LV4	10000	
#define SDPD_AGC_LV5	12000	
#define SDPD_AGC_LV6	14000	
#define SDPD_AGC_LV7	16000

#define SDPD_THHD_LV0	6		 
#define SDPD_THHD_LV1	16		
#define SDPD_THHD_LV2	48		
#define SDPD_THHD_LV3	96
#define SDPD_THHD_LV4	120
#define SDPD_THHD_LV5	150
#define SDPD_THHD_LV6	180
#define SDPD_THHD_LV7	200
#endif
#endif





typedef enum
{
       Start,
	ID,
       Xbuf,
       Ybuf,
       Mbuf
}FLASHADDR_NAME;

typedef enum
{
		white_thhd,
		white_median,
		dark_thhd,
		dark_median
}DPDSEARCHMODE_NAME;

typedef	enum {
	MISP_SDPD_OFF = 0,		// static dpc/dpd off
	MISP_SDPD_SETUP,		// static dpc/dpd sensor, iris setting
	MISP_SDPD_CHK ,		// static dpc/dpd check
	MISP_SDPD_RST,			// isp internal static dpc memory rst	
	MISP_SDPD_READ ,		// static dpd read processing
	MISP_SDPD_T_FLASH ,	// static dpd temp flash update
	MISP_SDPD_DECIDE,		// waiting for flash update or not
	MISP_SDPD_END
}	MISP_SDPD_MODE_t;

#if defined(__USE_DPD_i51X__)	
typedef	struct
{
	BOOL	init;				// init at boot sequence flag
	BOOL	auto_ddpc;		// auto dynamic dpc thhd contral enable or not
	BOOL	ae_backflag;	// AE return(back) flag 
	BOOL	end_flag;		// dpd routine end flag 
	BOOL	median_ineqsel; //only median dark mode set
	LONG 	ctrl;				// SDPC ctrl flag
	WORD	mode;			// SDPC running control mode
	BYTE	search_type;	// 
	BYTE	agc_lv;			// dpd ae agc level setting(sensor)
	BYTE	shut_lv;			// dpd ae agc level setting(sensor)	
	BYTE	dpc_mode;		// current byr_dpc_mode[NH_SIZ];	

	BYTE	fwait_cnt;			//sensor transition waiting frame
	BYTE	agc_backup;		//dpd before parameter mode save
	BYTE	iris_backup;		//dpd before parameter mode save

	WORD	upper_thhd;		//hot pixel threshold
	WORD	lower_thhd;		//dead(dark) pixel threshold

	BOOL	dpd_check_bypass;	//check bypass
	BYTE	buff_page_use;	//max buffer page use
	BYTE	buff_page_sel;	//now buffer accece page
	WORD	frame_id;		//dpd_frame_id
	
	BYTE	scnt;			//dpd_check_para cnt	
	BYTE	rp_nfs;			//dpd repeat counter number for setup(parameter check)
	WORD	scope[4];		//dpd_check_para

	BYTE	debug_disp;		//display debug message

}	SDPC_CTRL_PARAM, *PSDPC_CTRL_PARAM;
#else
typedef	struct
{
	BOOL	init;				// init at boot sequence flag
	BOOL	auto_ddpc;		// auto dynamic dpc thhd contral enable or not	
	BYTE 	dpc_th_val0 ;
	BYTE 	dpc_th_val1 ;
	BYTE 	dpc_th_val2 ;

	BOOL	ae_backflag;	// AE return(back) flag 
	BOOL	end_flag;		// dpd routine end flag 
	LONG 	ctrl;				// SDPC ctrl flag
	WORD	mode;			// SDPC running control mode
	BYTE	agc_lv;			// dpd ae agc level setting(sensor)
	BYTE	shut_lv;			// dpd ae agc level setting(sensor)	
	BYTE	dpc_mode;		// current byr_dpc_mode[NH_SIZ];
	
	BYTE	fwait_cnt;			//sensor transition waiting frame
	BYTE	agc_backup;		//dpd before parameter mode save
	BYTE	iris_backup;		//dpd before parameter mode save
	
	BYTE	hot_thhd;		//hot pixel threshold
	BYTE	dead_thhd;		//dead(dark) pixel threshold
	BYTE	rp_n;			//dpd repeat counter number
	BYTE	rp_nfs;			//dpd repeat counter number for setup(parameter check)

	BYTE	cut;				//dpd counter cutting point
	BYTE	scnt;			//dpd_check_para cnt	
	WORD	cyl;				//pixel count length 1 cycle
	WORD	scope[4];		//dpd_check_para

	BYTE	debug_disp;		//display debug message
}	SDPC_CTRL_PARAM, *PSDPC_CTRL_PARAM;
#endif

typedef struct {

	WORD	dpd_cycleendflag;
	WORD	dpd_errcnt_frame;
	WORD	dpd_scope_cnt;

	#ifndef __USE_DPD_i51X__

	WORD	dpd_repeat_cnt_chk;
	
	WORD	x_pos_buf_0;			// static dead pixel x position 0
	WORD	x_pos_buf_1;			// static dead pixel x position 1
	WORD	x_pos_buf_2;			// static dead pixel x position 2
	WORD	x_pos_buf_3;			// static dead pixel x position 3
	WORD	y_pos_buf_0;			// static dead pixel x position 0
	WORD	y_pos_buf_1;			// static dead pixel x position 1
	WORD	y_pos_buf_2;			// static dead pixel x position 2
	WORD	y_pos_buf_3;			// static dead pixel x position 3
	#endif

	
}	MISP_DPD_BUFFER, *PMISP_DPD_BUFFER;

typedef struct {
	WORD	th;				// count thhd
	WORD	l;				// final data length 
	WORD	x[NH_SIZ];			// static dead pixel x position(normal cond)
	WORD	y[NH_SIZ];			// static dead pixel y position(normal cond)
	BYTE	byr_dpc_mode[NH_SIZ];
	#if defined (DPD_COUNT_CUT_USE)	
	BYTE	find[NH_SIZ];		// static dead pixel find counter save
	#endif
}	NOR_HDPC_EEPROM, *PNOR_HDPC_EEPROM;

typedef struct {
	BYTE	pwr0_threshold1;
	BYTE	pwr0_threshold2;
	BYTE	pwr0_selmode;

	BYTE	pwr1_threshold1;
	BYTE	pwr1_threshold2;
	BYTE	pwr1_selmode;
	
	BYTE	pwr2_threshold1;
	BYTE	pwr2_threshold2;
	BYTE	pwr2_selmode;

	BYTE	pwr3_threshold1;
	BYTE	pwr3_threshold2;
	BYTE	pwr3_selmode;	

	BYTE	pwr4_threshold1;
	BYTE	pwr4_threshold2;
	BYTE	pwr4_selmode;	

	BYTE	pwr5_threshold1;
	BYTE	pwr5_threshold2;
	BYTE	pwr5_selmode;	

	BYTE	pwr6_threshold1;
	BYTE	pwr6_threshold2;
	BYTE	pwr6_selmode;	

	BYTE	pwr7_threshold1;
	BYTE	pwr7_threshold2;
	BYTE	pwr7_selmode;		
	
	BYTE	fixed_threshold1;
	BYTE	fixed_threshold2;
	BYTE	fixed_selmode;		
	
}	DDPC_PARAM, *PDDPC_PARAM;


//dpc running
#define		SDPC_PROCESS_MASK		0xF0000000		// DPC mask
#define		SDPC_PROCESS_RUN			0x00000000		// DPC run
#define		SDPC_PROCESS_STATIC		0x10000000		// DPC run
#define		SDPC_PROCESS_FIXED		0x20000000		// DPC run
#define		SDPC_PROCESS_OFF			0x80000000		// DPC disabled
#define		SDPC_TRIGGER_ISP			0x40000000		// DPC memory at ISP update 

#define		SDPC_MEMORY_MASK			0x0000000F		// DPC mask
#define		SDPC_MEMORY_RELOAD		0x00000008		// DPC read memory rewrite from external memory
#define		SDPC_MEMORY_RESET		0x00000004		// DPC read memory at ISP update 
#define		SDPC_MEMORY_READ			0x00000002		// DPC read memory at ISP update 
#define		SDPC_MEMORY_WRITE		0x00000001		// DPC Write memory at ISP update 
#define		SDPC_MEMORY_HOLD			0x00000000		// Do not change MEMORY

#define		SDPC_STATE_MASK			0x0F000000		// DPC sensor gain check status mask
#define		SDPC_NORMAL_COND			0x01000000		//see : now normal cond dpc


#define		SDPC_FLASH_MASK			0x00F00000		//DPC flash controll mask
#define		SDPC_FLASH_PRINT			0x00A00000		//debugprint flash dpc region
#define		SDPC_FLASH_UPDATE		0x00800000		//update flash dpc region
#define		SDPC_FLASH_CLEAR			0x00400000		//reset flash dpc region
#define		SDPC_FLASH_TEMP_READ		0x00300000		//DPC temp sector read
#define		SDPC_FLASH_READ			0x00200000		//read uartprint for debug. if write isp memory use SDPC_MEMORY_RELOAD flag
#define		SDPC_FLASH_WRITE			0x00100000		//write to flash 
#define		SDPC_FLASH_HOLD			0x00000000		//see : now normal cond dpc

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
#define		reg_sdpd_parameter 		0x4B3			//[0] dpd_enable	[1] dpd_mode 	[8:4] dpd_repeat_counter
#define		reg_sdpd_pixel_thhd			0x4B4			//[7:0]hot_pixel_thhd, [15:8]dead_pixel_thhd
#define		reg_sdpd_rd_status			0x4B5			//[9:5]dpd_repeat_cnt_chk, [4:1] dpd_errcnt_frame	Send error pixel count value duaring one frame   [0] Dead Pixel Detecting One Cycle end flag
#define		reg_sdpd_scope_cnt			0x4B9			//[15:0] dead pixel scope cnt

#define		reg_sdpd_memory_rd_addr	0x428			//[9:0] Set read address for the static defective pixel memory.
#define		reg_sdpd_memory_rd_xpos	0x429			//[11:0] X position value from the static defective pixel memory for the given address.
#define		reg_sdpd_memory_rd_ypos	0x42A			//[11:0] Y position value from the static defective pixel memory for the given address.
#define		reg_sdpd_memory_wr_addr	0x42B			//[15] Write strobe for the static defective pixel memory. [9:0] Set write address for the static defective pixel memory.
#define		reg_sdpd_memory_wr_xpos	0x42C			//[11:0] X position value written to the static defective pixel memory for the given address.
#define		reg_sdpd_memory_wr_ypos	0x42D			//[11:0] Y position value written to the static defective pixel memory for the given address.
#else 												// MDIN-i51X (i510, i540, i550)
#define		reg_sdpd_parameter 		0x4B3			//[0] dpd_enable	[1] dpd_mode 	[8:4] dpd_repeat_counter
#define		reg_sdpd_upper_thhd		0x4B4			//[11:0] reg_sdpd_lower_thhd< detect < reg_sdpd_upper_thhd
#define		reg_sdpd_lower_thhd		0x4B5			//[11:0] reg_sdpd_lower_thhd< detect < reg_sdpd_upper_thhd
#define		reg_sdpd_rd_status			0x4B6			//[9:5]dpd_repeat_cnt_chk, [4:1] dpd_errcnt_frame	Send error pixel count value duaring one frame   [0] Dead Pixel Detecting One Cycle end flag
#define		reg_sdpd_scope_cnt			0x4BA			//[15:0] dead pixel scope cnt
#define		reg_sdpd_buff_page			0x4BB	

#define		reg_sdpd_memory_rd_addr	0x428			//[10:0] Set read address for the static defective pixel memory.
#define		reg_sdpd_memory_rd_xpos	0x429			//[12:0] X position value from the static defective pixel memory for the given address.
#define		reg_sdpd_memory_rd_ypos	0x42A			//[12] dpd_buf_rd_nflg [11:0] Y position value from the static defective pixel memory for the given address.
#define		reg_sdpd_memory_wr_addr	0x42B			//[15] Write strobe for the static defective pixel memory. [10:0] Set write address for the static defective pixel memory.
#define		reg_sdpd_memory_wr_xpos	0x42C			//[12:0] X position value written to the static defective pixel memory for the given address.
#define		reg_sdpd_memory_wr_ypos	0x42D			//[12] dpd_buf_wr_nflg [11:0] Y position value written to the static defective pixel memory for the given address.
#endif


// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// deadpix.c
BOOL StaticDPC_Process(void);
BOOL StaticDPD_Busy(void);
void SetStaticDPC_init_flag(BOOL en);

void DPD_RUN(void);
void SetSDPC_OnOff(BYTE nID);
BYTE GetSDPC_OnOff(void);
void SetSDPC_MemWrite_Order(void);
void SetSDPC_MemRead_Order(void);
void SetSDPC_MemReset_Order(void);
void SetSDPC_MemReload_Order(void);

void SetSDPD_AGC_Level(BYTE nID);
BYTE GetSDPD_AGC_Level(void);
void SetSDPD_Shutter_level(BYTE nID);
BYTE GetSDPD_Shutter_level(void);

void SetSDPD_thhd(BYTE nID);
void SetSDPD_AE_BackFlag(BOOL OnOff);
BOOL GetSDPD_AE_BackFlag(void);
void SetDPC_DebugDisp(BYTE nID);
BYTE GetDPC_DebugDisp(void);

void SetSDPC_FlashPrint_Order(void);
void SetSDPC_FlashUpdate_Order(void);
void SetSDPC_FlashWrite_Order(void);
void SetSDPC_FlashRead_Order(void);
void SetSDPC_FlashTempRead_Order(void);
void SetSDPC_FlashClear_Order(void);
void SetDynamicDPC_Auto(BOOL OnOff);
BOOL GetDynamicDPC_Auto(void);
void DPD_MemPrint(void);

//for debug func
//void dpdbuf_wr_test(void);
//void dpdbuf_rd_test(void);	
//void dpdbuf_wrd_test(void);
//void DPC_MemoryReset(void);
void dpd_pixel_val_print(void);

#if defined(__USE_DPD_i51X__)
void SetSDPC_Search_type(WORD nID);
BYTE GetSDPC_Search_type(void);
void dpd_buff_page_use(BYTE nID);

#endif

void Init_DynamicDPC(void);
void DynamicDPC_Process(BYTE pwr);

void SetDDPC_Threshold1(WORD pwr, WORD nID);
void SetDDPC_Threshold2(WORD pwr, WORD nID);
void SetDDPC_SelMode(WORD pwr, WORD nID);
BYTE GetDDPC_Threshold1(WORD pwr);
BYTE GetDDPC_Threshold2(WORD pwr);
BYTE GetDDPC_SelMode(WORD pwr);

void SetDDPC_WriteEEPROM(void);
#endif	/* __MISP_DPC_H__   */
