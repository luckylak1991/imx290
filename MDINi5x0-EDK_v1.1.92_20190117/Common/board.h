
//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name			:  BOARD.H
// Description			:  This file contains typedefine for the driver files	
// Ref. Docment			: 
// Revision History 	:

#ifndef		__BOARD_H__  
#define		__BOARD_H__  

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

//#define __ECOM1_UNION_12X__
	//#define __USE_MDIN_i540__
	//#define	__MDIN_i540_REF_V121__
	//#define	__USE_CMOS_IMX226__
	//#define __USE_AFZOOM_LENS__			// for use AFZOOM lens
	//#define	__USE_DCIRIS_HALL__			// for use DCIRIS-Hall &  AN41908 IRIS & AF controller

//#define	__USE_4K60_MD1__			// use 4k60 mdin-1
//#define	__USE_4K60_MD2__			// use 4k60 mdin-2
	//#define	__USE_MDIN_i540__
	//#define	__MDIN_i540_4K60_V10__
	//#define	__USE_EXT_PLL_IC__
	//#define	__USE_SIL9136_TX__
	//imx274.h #define	__USE_IMX274_2160P60_MODE1__

//#define	__USE_EXT_AF_MD1__			// use external CPU for AF master mdin-1(cpu slave & AF master)
	//#define	__USE_MDIN_i540__
	//#define	__MDIN_i540_4K60_V10__
	//#define	__USE_EXT_PLL_IC__
	//#define	__USE_SIL9136_TX__

	//#define	__USE_AFZOOM_LENS__			// for use AFZOOM lens
	//#define	__USE_DCIRIS_HALL__			// for use DCIRIS-Hall &  AN41908 IRIS & AF controller

//#define	__USE_MISP_AF_MD2__			// use misp for AF slave mdin-2 (cpu master & AF slave)
	//#define	__USE_MDIN_i540__
	//#define	__MDIN_i540_4K60_V10__
	//#define	__USE_EXT_PLL_IC__
	//#define	__USE_SIL9136_TX__

	//#define	__USE_MANUAL_LENS__			// for use MANUAL lens
	//#define	__USE_DCIRIS_NONE__			// for not use IRIS

//#define	__USE_IRDATA_SLAVE2MASER__				//IR data from slave to master



#define	__FW_VERSION__		0x33	// FW revision

//--------------------------------------------------------------
//-- 1. Select Target ISP
//--------------------------------------------------------------
//#define	__USE_MDIN_i500__			// use MDIN-i500
//#define	__USE_MDIN_i510__			// use MDIN-i510
#define	__USE_MDIN_i540__			// use MDIN-i540
//#define	__USE_MDIN_i550__			// use MDIN-i550


#define	__USE_MDIN_i5XX_REV__		// use MDIN-i5xx revision chip


#if defined (__USE_MDIN_i500__)
// Select one of below options
//#define	__MDIN_i5XX_FPGA__				// i5XX_FPGA board
//#define	__MDIN_i500_EDK__				// i500_EDK(socket) board
#define	__MDIN_i500_REF__				// i500_REF board
#endif


#if defined (__USE_MDIN_i510__)
//#define	__MDIN_i5XX_FPGA__				// i5XX_FPGA board
//#define	__MDIN_i510_EDK_V10__				// i510_EDK(socket) v1.0 board
//#define	__MDIN_i510_EDK_V11__				// i510_EDK(socket) v1.1 board
//#define	__MDIN_i510_REF_V10__				// i510_REF v1.0 board
#define	__MDIN_i510_REF_V134__				// i510_REF v1.34 board
//#define	__MDIN_i510_IMX290__			// i510 + IMX290 board
#endif


#if defined (__USE_MDIN_i540__)
//#define	__MDIN_i5XX_FPGA__				// i5XX_FPGA board
//#define	__MDIN_i540_EDK_V10__				// i540_EDK(socket) v1.0 board
//#define	__MDIN_i540_EDK_V11__				// i540_EDK(socket) v1.1 board
//#define	__MDIN_i540_REF_V10__				// i540_REF v1.0 board
#define	__MDIN_i540_REF_V121__				// i540_REF v1.21 board
//#define	__MDIN_i540_4K60_V10__				// i540 4k60 v1.0 board
#endif


#if defined (__USE_MDIN_i550__)
//#define	__MDIN_i5XX_FPGA__				// i5XX_FPGA board
//#define	__MDIN_i550_EDK_V10__				// i550_EDK(socket) v1.0 board
//#define	__MDIN_i550_EDK_V11__				// i550_EDK(socket) v1.1 board
//#define	__MDIN_i550_REF_V10__				// i550_REF v1.0 board
#define	__MDIN_i550_REF_V121__				// i550_REF v1.21 board
//#define	__MDIN_i550_USB_V100__				// i550_REF v1.21 board
#endif





//--------------------------------------------------------------
//-- 2. Select Target CMOS Sensor
//--------------------------------------------------------------
//#define	__USE_CMOS_MT9M131__			// use aptina cmos
//#define	__USE_CMOS_MN34041__			// use panasonic cmos
//#define	__USE_CMOS_MN34220__			// use panasonic cmos
//#define	__USE_CMOS_MN34420__			// use panasonic high gain cmos
//#define	__USE_CMOS_IMX076__			// use sony cmos
//#define	__USE_CMOS_IMX124__			// use sony 123,124 3mega cmos, if 123 use, lift  __USE_CMOS_IMX123__ comment
//#define	__USE_CMOS_IMX252__				// use sony cmos 4ch lvds 1/2 (max 3M 216.2fps) sensor
//#define	__USE_CMOS_IMX265__			// use sony cmos 4ch lvds 1/2 (max 3M 55fps) sensor
//#define	__USE_CMOS_IMX174__			// use sony cmos 2,4,8ch lvds 1/1.2 fhd sensor
//#define	__USE_CMOS_IMX249__			// use sony cmos 2ch lvds 1/1.2 fhd sensor	 share 174 f/w
//#define	__USE_CMOS_IMX253__			// use sony cmos 4k GS sensor(1.1inch)
//#define	__USE_CMOS_IMX273__				// use sony cmos 1.2M GS sensor(1/2.9inch)
//#define	__USE_CMOS_IMX185__			// use sony cmos
//#define	__USE_CMOS_IMX136__			// use sony HDR cmos
//#define	__USE_CMOS_IMX222__			// use sony LowCost FHD(Max 1080p30, TTL, #V Reverse only) CMOS
//#define	__USE_CMOS_IMX322__			// use sony LowCost FHD(Max 1080p30, TTL, #V Reverse only) CMOS
//#define	__USE_CMOS_IMX323__			// use sony csp type ttl 1080p30 max cmos, similar imx322, but agc is differ  share 322 f/w
//#define	__USE_CMOS_IMX178__			// use sony HDR cmos
//#define	__USE_CMOS_IMX226__			// use sony UHD HDR cmos(max 4096x3000, 1/1.9 ),  (imx172 share imx226 f/w code, then turn on imx172 option under..), support only i5x0
//#define	__USE_CMOS_IMX326__			// use sony UHD cmos(max 3096x2218, 1/2.9 ) (similar to IMX274)
#define	__USE_CMOS_IMX274__			// use sony UHD HDR cmos(3840x2160 only, 1/2.5 )support only i5x0
//#define	__USE_CMOS_IMX236__			// use sony HDR cmos						share 136 f/w
//#define	__USE_CMOS_IMX238__			// use sony 720p(max 1280x1024 cmos)
//#define	__USE_CMOS_IMX290__			// use sony DOL WDR cmos
//#define	__USE_CMOS_IMX327__			// use sony DOL WDR cmos (similar to imx290 4ch, see imx290.c)
//#define	__USE_CMOS_IMX385__			// use sony DOL WDR cmos
//#define	__USE_CMOS_IMX334__			// use sony mipi 4k sensor(i2c communication only), The test will be going on
//#define	__USE_CMOS_AR0331__			// use Aptina HDR cmos
//#define	__USE_CMOS_AR0134__				// use Aptina 720p GS, I2C Sensor
//#define	__USE_CMOS_AR0135__			// use Aptina 720p GS, I2C Sensor
//#define	__USE_CMOS_MT9P031__			// use aptina 5M cmos
//#define	__USE_CMOS_PO3100K__			// use pxlplus cmos
//#define	__USE_CMOS_PS1210K__			// use pxlplus cmos
//#define	__USE_CMOS_PS1400K__			// use pxlplus 4M lvds 4ch sensor
//#define	__USE_CMOS_EXT1__
//#define	__USE_CMOS_EXT2__
//#define	__USE_CMOS_EXT3__				// use h_k sensor ttl
//#define	__USE_CMOS_OV2710__			// use omnivision fhd30p ttl cmos
//#define	__USE_NO__CMOS__				// To avoid menu or osd ucom code comfile error.  when setting new sensor

//#define	__USE_EXT2IPP__					// Set external input signal to sensor(bayer) path. Due to poor image quality, use only in special cases.  

//--------------------------------------------------------------
//-- 3. Select Target LENS
//--------------------------------------------------------------

//#define	__USE_GAMMA_AE__			// use Gamma for Digital AE
//#define	__USE_AWB_SPD_CTL__			// use AWB speed control by StdDev, Zoom and Brightness
//#define	__USE_AF_FAR_LIMIT__			// use AF lens far limit

#define	__USE_GET_RGB_MODE__				// get RGB data with WB grey world mode, (imx253 shuld comment)
#define	__USE_GET_AWB_RGB_INFO__			// (imx253 shuld comment)
#define	__USE_GET_AWB_RGB_INFO_SIZE_4__	// (imx253 shuld comment)
#define	__USE_AWB_RGB_WDR_MEM__			// use ddr mem wdr area for awb rgb sum calc.

//#define	__USE_AWB_WIDE_FOR_CHN__			// use wide awb for china

//------- Select Lens Type -------
//#define	__USE_AFZOOM_LENS__			// for use AFZOOM lens
//#define	__USE_2MOTOR_LENS__			// for use 2motor lens
#define	__USE_MANUAL_LENS__			// for use MANUAL lens

//------- Select Lens AE Type -------
//#define	__USE_DCIRIS_HALL__			// for use DCIRIS-Hall &  AN41908 IRIS & AF controller
//#define	__USE_DCIRIS_DAMP__			// for use DCIRIS-Damp & AN41919 IRIS controller
#define	__USE_DCIRIS_NONE__			// for not use IRIS
//#define	__USE_PWMIRIS_ONLY__		// for use PWM IRIS control only //use OP amp
//#define	__USE_P_IRIS_ONLY__		// for use P-IRIS control only

#if defined(__USE_DCIRIS_HALL__)
#define	__USE_AN41908__				// for use AN41908 IRIS&AF controller
#elif defined(__USE_DCIRIS_DAMP__)
#define	__USE_AN41919__				// for use AN41919 IRIS controller
#elif defined(__USE_PWMIRIS_ONLY__)
#define	__USE_AN41908__				// for use AN41908 AF controller
//#define	__USE_GPIO_MTC_AF__		// for use GPIO Control for AF controller
#define	__USE_PWM_ADC__				// for use ADC of pwm output
#elif defined(__USE_P_IRIS_ONLY__)
#define	__USE_AN41908__				// for use AN41908 AF controller
//#define	__USE_GPIO_MTC_AF__		// for use GPIO Control for AF controller
//#define	__USE_PWM_ADC__				// for use ADC of pwm output
#define	__USE_GPIO_MTC_AE__		// for use GPIO Control for AE controller
#endif

//------- Select Lens -------
// ******* Zoom Lens
#if defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__)
//#define	__USE_OLPF_DAIWON__			// use daiwon-zoom lens
//#define	__USE_OLPF_YOPTIC__			// use yoptic-zoom lens
//#define	__USE_X30X_TAMRON__			// use tamron-30x lens
//#define	__USE_OLPF_CANON__			// use canon-zoom lens
//#define	__USE_OLPF_SUNNY__			// use sunny-zoom lens
//#define	__USE_X33X_CHINA__			// use china-33x lens
//#define	__USE_X10X_UNION__			// use union-10x lens
//#define	__USE_X10X_ZMAX__			// use zmax-10x lens
#define	__USE_X03X_ZMAX__			// use zmax-3.3x lens
//#define	__USE_X12X_UNION__			// use union-12x lens
//#define	__USE_YT3010_2MOTOR__		// use yt3010 2motor lens
#else
// ******* C mount Manual Lens
//#define	__USE_MANU_RV4515__			// use rootvison c RV3-4.5-15 IR (imx185 i500 setting only yet)
//#define	__USE_MANU_RV0850__			// use rootvison c RV3-8-50 IR (imx290,124 setting only yet)
//#define	__USE_MANU_KAWADEN12120__		// use kawaden c 12-120mm (imx290 setting only yet, lens support by rootvision)
//#define	__USE_MANU_RV1530__			// use rootvison c RV5-1530 IR (imx124 setting only yet)
//#define	__USE_MANU_RX8FM1214C__			// use rootvison 8MP (imx274 8MP)
//#define	__USE_MANU_TAMRON_5M__			// use tamron-manu 5M 8.4-13mm F1.5 lens
// ******* CS mount Manual Lens
//#define	__USE_MANU_TAMRON__			// use tamron-manu 3M 2.8-8 F1.4 lens
//#define	__USE_MANU_TAMRON_PWM__		// use tamron-manu 3M 3.0-8 F1.0 1/3 inch lens
// ******* M12 mount Manual Lens
//#define	__USE_MANU_SUNEX115__			// use sunex-m12 DSL115 1.2M lens(imx290 setting only yet)
//#define	__USE_MANU_RV3620__				// use rootvison m12 RV2-B3620IR (1/2 reference)
//#define	__USE_MANU_RV3220__				// use rootvison m12 RV12-B3220IR-17 (1/1.8 & 4k(226,274,172) reference)
//#define	__USE_MANU_PXLPLS__				// use pxlpls-manu lens
#define	__USE_MANU_BW3M30B4000__		// use bowoon m12 1/2.7 lens(1/2.7 reference imx290)
//#define	__USE_MANU_BW4K40_1000__		//use bowoon m12 1/2 4k lens
//#define	__USE_MANU_WW_4KUHD__			//
//#define	__USE_MANU_WW_8mmIR__			//
//#define	__USE_MANU_M12_8IR__			// with mdin-i5x0 ref b/d
//#define	__USE_MANU_BW3ML43B_1000__		// use bowoon 3M for (panorama)
//#define	__USE_MANU_RV10_FM1614A1__		// use rootvision 1-inch manual lens

//#define	__USE_X03X_ZMAX__			// use zmax-3.3x lens
#endif

#if defined(__USE_CMOS_AR0134__) || defined(__USE_CMOS_AR0135__) 
//#define	__USE_CMOS_AR0134__INTERNAL_AE
#endif



//--------------------------------------------------------------
//-- 4. Select Target Tx Device
//--------------------------------------------------------------
#define	__USE_IT6615_TX__			// use hdmi-tx ITE6615
//#define	__USE_IT66121_TX__			// use hdmi-tx it66121
//#define	__USE_LT8618EX_TX__			// use hdmi-tx lt8618ex
//#define	__USE_LT8618SX_TX__			// use hdmi-tx lt8618sx
//#define	__USE_SIL9136_TX__			// use hdmi-tx sil9136
//#define	__USE_EN332T__				// use ex-sdi tx en332t
//#define	__USE_TVI_TX__				// use tvi tx tp2912



//--------------------------------------------------------------
//-- 5. Select Target Rx Device
//--------------------------------------------------------------
#if defined(__MDIN_i550_REF_V121__)
#define	__USE_LT8619_RX__			// use hdmi-rx lt8619
//#define	__USE_IT6802_RX__			// use hdmi-rx it6802
#endif



//--------------------------------------------------------------
//-- specific compile option(user test option)
//--------------------------------------------------------------

//when af board booting, vsync waiting must need.
#define	__SENSOR_VSYNC_BOOT__			// use sensor v_sync booting   comments is without sensor vsync booting

//#define	__USE_AF_DATA_OUT__				// use AF output data

//#define	__SENSOR_ERR_CHECK__
//#define	__P_IRIS_TEST__	// p-iris(P-IRIS)
//#define	__COLOR_S7320__			//if comment so fcb7500e not comment 7350(imx291sensor used)
//#define	__USE_SENSOR_BAYER_10BIT__		// use sensor10 bit mode boot
//#define	__NR_YC_PROCESS__				// Set YC NR mode boot
//#define	__M33_V101__

//#define	__USE_CPU_CLK_XTAL__		// use cpu clk 27M
//#define	__USE_CPU_WDT_RST__			// use cpu watchdog reset //h
//#define	__USE_UART0_VISCA__			// use uart-0 to visca
//#define	__USE_UART0_9600__			// baudrate : 9600bps
#define	__DEBUGPRT_UART1__		0	// "0" for UART0, "1" for UART1
//#define	__USE_IRKEY_DEMO__		// enable irkey for demo(menu & arrow key + some function key only)
//#define	__USE_CHN_FONT__			// use chinease font
//#define	__USE_EXT_PLL_IC__			// use external pll ic
//#define	__USE_EXT_OSC_37M__			// use external osc(37.0879mhz) for 59.94hz v.out
//#define	__USE_EXT_OSC_148M__			// use external(host clock in) osc(148.35Mhz)

//#define	__USE_FD_PROCESS__
//#define	__USE_IR_LED_PWM__			// use pwm for external ir led control
//#define	__USE_I2C_GPIO__			// use i2c using gpio for hdmi-tx
//#define	__USE_PS8409_RETIMER__			// use hdmi repeater with re-timing
//#define	__USE_WW_KEYBUTTON__
//#define	__USE_OSD_TITLE__				// use osd title for video output type(hdmi/tvi/ex-sdi,,) -- for cpse2017

#define	__USE_8BIT_OPERATION__		// use 8bit pixel operation : basic
//#define	__USE_10BIT_OPERATION__		// use 10bit pixel operation : not supported in 4K sensor


// jinsjeon 20160104, bayermapmode h-sync extension
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
// #define	__USE_SENS_HSYNC_EXT__
#endif

//25/50Hz system
//#define	__USE_25Hz_SENSOR__  //sensor input
//#define	__USE_25Hz_OUTPUT__  //video output

//extend V-total to make 25hz, else extend H-total
//#define	__VTOTAL_CTRL_PAL__

//LVDS TX Ouput. Comment is digital
//#define	__USE_LVDSTX_MAIN_SINGLE__		// output use lvds tx
//#define	__USE_LVDSTX_MAIN_DUAL__		// output use lvds tx
//#define	__USE_LVDSTX_EVEN_ODD__			// output use lvds tx even odd(h/w set : dual mode, chip set : single & even_odd mode)

#if (defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)) && !defined(__USE_MDIN_i5XX_REV__)  	// i510, i540, i550 old ver.
#define 	__USE_COMPILER_PATCH_VER__	// use compiler patch version for i51x old chip.
#endif


//#define	__PANORAMA_SENSOR_SETUP__	

#if defined (__PANORAMA_SENSOR_SETUP__)
#define	__PANORAMA_SETUP__				// Set panorama app inital : See app_pano.h for further initial
#define	__PANORAMA_5x0__ 1				// 0 : i550 ref x3 1: panorama one board
#endif

//#define	__CASCADE_CAM_SETUP__	

#if defined(__USE_MDIN_i500) || defined(__USE_EXT_OSC_37M__) || defined(__USE_EXT_OSC_148M__) || defined(__USE_EXT_PLL_IC__)
#define	__USE_59Hz_OUTPUT__			// use 59.94/29.97 output
#endif


#if	defined(__MDIN_i5XX_FPGA__)		// FPGA board
//#define	__USE_DDR_MCLK99__		// use ddr clk 99M  : OSC_CLOCK 49500000 -- lib_config.h
//#define	__USE_DDR_MCLK108__		// use ddr clk 108M : OSC_CLOCK 54000000 -- lib_config.h
//#define	__USE_DDR_MCLK121__		// use ddr clk 121.5M : OSC_CLOCK 27000000 -- lib_config.h
#define	__USE_DDR_MCLK162__		// use ddr clk 162M : OSC_CLOCK 40500000 -- lib_config.h
//#define	__USE_DDR_MCLK198__		// use ddr clk 198M : OSC_CLOCK 49500000 -- lib_config.h
//#define	__USE_DDR_MCLK180__		// use ddr clk 180M : OSC_CLOCK 45000000 -- lib_config.h
#else								// EDK & REF board
//#define	__USE_DDR_MCLK202__		// use ddr clk 202M  : OSC_CLOCK 101250000 -- lib_config.h
//#define	__USE_DDR_MCLK198__		// use ddr clk 198M  : OSC_CLOCK 99000000 -- lib_config.h //mdin500
//#define	__USE_DDR_MCLK324__		// use ddr clk 324M  : OSC_CLOCK 81000000 -- lib_config.h
//#define	__USE_DDR_MCLK339__		// use ddr clk 339M  : OSC_CLOCK 84750000 -- lib_config.h
//#define	__USE_DDR_MCLK360__		// use ddr clk 360M  : OSC_CLOCK 90000000 -- lib_config.h
//#define	__USE_DDR_MCLK367__		// use ddr clk 367.5M  : OSC_CLOCK 91875000 -- lib_config.h
//#define	__USE_DDR_MCLK375__		// use ddr clk 375M  : OSC_CLOCK 93750000 -- lib_config.h
//#define	__USE_DDR_MCLK383__		// use ddr clk 382.5M  : OSC_CLOCK 95625000 -- lib_config.h
#define	__USE_DDR_MCLK396__		// use ddr clk 396M  : OSC_CLOCK 99000000 -- lib_config.h
//#define	__USE_DDR_MCLK405__		// use ddr clk 405M  : OSC_CLOCK 101250000 -- lib_config.h
#endif


#if defined(__USE_MDIN_i500__) || defined(__USE_MDIN_i510__)	// i500, i510
#define	__USE_DDR_512M__			// use ddr 512M
#endif
	
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
#define	__USE_DDR_1G__				// use ddr 1G
#endif
	
#if	defined(__MDIN_i5XX_FPGA__)									// fpga board
//#define	__USE_DDR_1G__				// use ddr 1G
//#define	__USE_DDR_512M__			// use ddr 512M
#define	__USE_DDR_FIDELIX_256M__	// use ddr 256M(Fidelix)
//#define	__USE_DDR_WINBOND_256M__	// use ddr 256M(Winbond)
#endif


//configuration for arbiter & SDRAM
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
#if	defined(__MDIN_i5XX_FPGA__) 																// fpga board
#define	__ARBITER_DDR_CONFIG__	21	// 11:arbiter/DDR1, 21:arbiter_lpddr2/DDR1, 22:arbiter_lpddr2/DDR2
#else																							// edk & ref board
#define	__ARBITER_DDR_CONFIG__	22	// 11:arbiter/DDR1, 21:arbiter_lpddr2/DDR1, 22:arbiter_lpddr2/DDR2
#endif
#else 																							// i500
#define	__ARBITER_DDR_CONFIG__	11	// 11:arbiter/DDR1, 21:arbiter_lpddr2/DDR1, 22:arbiter_lpddr2/DDR2
#endif


#if defined(__USE_MDIN_i500__) || defined(__MDIN_i5XX_FPGA__)	// i500, fpga board
#define	FLASH_SIZE_KB	512			// serial flash size 512KB(4Mb)	
#else															// i510, i540, i550
#define	FLASH_SIZE_KB	1024		// serial flash size 1024KB(8Mb)
#endif



#if defined(__MDIN_i510_EDK_V10__) || defined(__MDIN_i510_EDK_V11__)
#define		__MDIN_i510_EDK__
#endif

#if defined(__MDIN_i510_REF_V10__) || defined(__MDIN_i510_REF_V134__)
#define		__MDIN_i510_REF__
#endif

#if defined(__MDIN_i540_EDK_V10__) || defined(__MDIN_i540_EDK_V11__)
#define		__MDIN_i540_EDK__
#endif

#if defined(__MDIN_i540_REF_V10__) || defined(__MDIN_i540_REF_V121__)
#define		__MDIN_i540_REF__
#endif

#if defined(__MDIN_i540_4K60_V10__)
#define		__MDIN_i540_4K60__
#endif

#if defined(__MDIN_i550_EDK_V10__) || defined(__MDIN_i550_EDK_V11__)
#define		__MDIN_i550_EDK__
#endif

#if defined(__MDIN_i550_REF_V10__) || defined(__MDIN_i550_REF_V121__) || defined(__MDIN_i550_USB_V100__)
#define		__MDIN_i550_REF__
#endif



#if defined (__MDIN_i500_REF__)
#define	__USE_EEPROM__				// use e2prom with i2c
//#define	EEPROM_SIZE_OVER_16KBIT		// e2prom size define(disable for 24C02)
#define	__USE_EEPROM_AWB__
#define	__USE_EEPROM_AF__
#endif

#if defined (__MDIN_i510_EDK__) || defined (__MDIN_i540_EDK__) || defined (__MDIN_i550_EDK__) || \
	defined (__MDIN_i510_REF__) || defined (__MDIN_i540_REF__) || defined (__MDIN_i550_REF__) || \
	defined(__MDIN_i510_IMX290__) || defined(__MDIN_i540_4K60__)
#define	__USE_EEPROM__				// use e2prom with i2c
//#define	__USE_EEPROM_AWB__
#define	__USE_EEPROM_AF__
#define	EEPROM_SIZE_OVER_16KBIT		// e2prom size define(enable for 24C256)
#endif



#if defined(__USE_IRKEY_DEMO__) || defined(__CASCADE_CAM_SETUP__)
#define	__USE_IRKEY_SWITCH__		// enable irkey switch for demo
#endif
//#define	__USE_KEY_TACK_SW__		// control key button to make sigle pulse gpio

#if defined(__MDIN_i5XX_FPGA__) || defined(__M33_V101__) || defined(__PANORAMA_SETUP__)
#define	__USE_BUTTONKEY_DISABLE__	// disable buttonkey
#endif


#if defined(__USE_4K60_MD1__) || defined(__USE_4K60_MD2__)
#define	__USE_4K60_INOUT__			// use 4k60/50 output
#endif

#if defined(__USE_4K60_MD2__)
#define	__USE_CPU_SPI_SLAVE__
#define	__USE_AF_SLAVE__
#elif defined(__USE_MISP_AF_MD2__)
#define	__USE_CPU_SPI_SLAVE__
#define	__USE_AF_SLAVE__
#else
//#define	__USE_CPU_SPI_SLAVE__		// use cpu spi-slave mode
//#define	__USE_AF_SLAVE__				// for use AF slave mode
#endif


#if defined(__MDIN_i550_USB_V100__)
#define		__USE_USB_VOUT__
#endif


#if	defined(__MDIN_i500_EDK__) || defined(__MDIN_i500_REF__)						// i500 EDK & REF board

#define	TVI_RESET(x)	(x)?	(*R_POHIGH(0) = 1<<0) : (*R_POLOW(0) = 1<<0)	// 0.0(TVI_RESET)
#define	DE_485(x)		(x)?	(*R_POHIGH(0) = 1<<1) : (*R_POLOW(0) = 1<<1)	// 0.1(485_DE)
#define	MD_OUT(x)		(x)?	(*R_POHIGH(0) = 1<<2) : (*R_POLOW(0) = 1<<2)	// 0.2(ISP_MD_OUT)
#define	SDI_RATE(x)		(x)?	(*R_POHIGH(0) = 1<<4) : (*R_POLOW(0) = 1<<4)	// 0.4(SDI_RATE_SEL)
#define	SENSOR_RSTN(x)	(x)?	(*R_POHIGH(0) = 1<<6) : (*R_POLOW(0) = 1<<6)	// 0.6(SENSOR_RESETN)
#define	OLPF_DY(x)		(x)?	(*R_POHIGH(1) = 1<<0) : (*R_POLOW(1) = 1<<0)	// 1.0(DAY_CTRL)
#define	OLPF_NT(x)		(x)?	(*R_POHIGH(1) = 1<<1) : (*R_POLOW(1) = 1<<1)	// 1.1(NIGHT_CTRL)
#define	LENS_VD(x)		(x)?	(*R_POHIGH(1) = 1<<2) : (*R_POLOW(1) = 1<<2)	// 1.2(LENS_VD)
#define	IRIS_VD(x)		(x)?	(*R_POHIGH(1) = 1<<3) : (*R_POLOW(1) = 1<<3)	// 1.3(IRIS_VD)
#define	GET_LENS_FRST()			((*R_PILEV(1))&1<<4) >>4						// 1.4(FOCUS_RESET)
#define	GET_LENS_ZRST()			((*R_PILEV(1))&1<<5) >>5						// 1.5(ZOOM_RESET)
#define	LENS_CS(x)		(x)?	(*R_POHIGH(2) = 1<<1) : (*R_POLOW(2) = 1<<1)	// 2.1(ZF_CS)
#if defined(__USE_AN41908__)
#define	IRIS_CS(x)		(x)?	(*R_POHIGH(2) = 1<<1) : (*R_POLOW(2) = 1<<1)	// 2.1(ZF_CS) -- 41908
#elif defined(__USE_AN41919__)
#define	IRIS_CS(x)		(x)?	(*R_POHIGH(2) = 1<<2) : (*R_POLOW(2) = 1<<2)	// 2.2(IRIS_CS) -- 41919
#endif
#define	CMOS_CS(x)																//cpu_spi0_ssx

#endif	// __MDIN_i500_EDK__, __MDIN_i500_REF__



#if	defined(__MDIN_i510_EDK__)														// i510 EDK(socket) board
#define	SENSOR_RSTN(x)	(x)?	(*R_POHIGH(1) = 1<<0) : (*R_POLOW(1) = 1<<0)	// 1.0(SENSOR_RESETN)
#define	LENS_VD(x)		(x)?	(*R_POHIGH(1) = 1<<2) : (*R_POLOW(1) = 1<<2)	// 1.2(LENS_VD)
#define	IRIS_VD(x)		(x)?	(*R_POHIGH(1) = 1<<3) : (*R_POLOW(1) = 1<<3)	// 1.3(IRIS_VD)
#define	GET_LENS_FRST()			((*R_PILEV(1))&1<<4) >>4						// 1.4(FOCUS_RESET)
#define	GET_LENS_ZRST()			((*R_PILEV(1))&1<<5) >>5						// 1.5(ZOOM_RESET)
#define	LENS_CS(x)		(x)?	(*R_POHIGH(2) = 1<<1) : (*R_POLOW(2) = 1<<1)	// 2.1(ZF_CS)
#define	IRIS_CS(x)		(x)?	(*R_POHIGH(2) = 1<<1) : (*R_POLOW(2) = 1<<1)	// 2.1(ZF_CS) -- 41908
#define	CMOS_CS(x)																// cpu_spi0_ssx
#define	OLPF_DY(x)																// DAY_CTRL
#define	OLPF_NT(x)																// NIGHT_CTRL
#define	DE_485(x)																// 485_DE
#define	SDI_RATE(x)																// SDI_RATE_SEL
#endif



#if	defined(__MDIN_i510_REF__)														// i510 REF board
#define	SENSOR_RSTN(x)	(x)?	(*R_POHIGH(1) = 1<<0) : (*R_POLOW(1) = 1<<0)	// 1.0(SENSOR_RESETN)
#define	LENS_VD(x)		(x)?	(*R_POHIGH(1) = 1<<2) : (*R_POLOW(1) = 1<<2)	// 1.2(LENS_VD)
#define	IRIS_VD(x)		(x)?	(*R_POHIGH(1) = 1<<3) : (*R_POLOW(1) = 1<<3)	// 1.3(IRIS_VD)
#define	LENS_CS(x)		(x)?	(*R_POHIGH(2) = 1<<1) : (*R_POLOW(2) = 1<<1)	// 2.1(ZF_CS)
#define	IRIS_CS(x)		(x)?	(*R_POHIGH(2) = 1<<1) : (*R_POLOW(2) = 1<<1)	// 2.1(ZF_CS) -- 41908
#define	GET_LENS_FRST()			((*R_PILEV(1))&1<<4) >>4						// 1.4(FOCUS_RESET)
#define	GET_LENS_ZRST()			((*R_PILEV(1))&1<<5) >>5						// 1.5(ZOOM_RESET)
#define	CMOS_CS(x)																// cpu_spi0_ssx
// I/O expander #1(U4) -- addr(0x00 : port_read,  0x01 : port_write)
#define	SDI_DIV_1001(x)			IOExp_Field(0, 0x01, 0, 1, x);					// U4.P0(SDI_DIV_1001)
#define	SDI_DET_TRS(x)			IOExp_Field(0, 0x01, 1, 1, x);					// U4.P1(SDI_DETECT_TRS)
#define	SDI_20B_10B(x)			IOExp_Field(0, 0x01, 2, 1, x);					// U4.P2(SDI_20BIT_10BIT)
#define	SDI_RATE(x)				IOExp_Field(0, 0x01, 3, 1, x);					// U4.P3(SDI_RATE_SEL)
#define	SDI_TX_CS(x)			IOExp_Field(0, 0x01, 4, 1, x);					// U4.P4(SDI_TX_CS)
#define	SDI_RSTN(x)				IOExp_Field(0, 0x01, 5, 1, x);					// U4.P5(SDI_RESET_N)
// I/O expander #2(U6) -- addr(0x00 : port_read,  0x01 : port_write)
#define	S_GPIO0(x)				IOExp_Field(1, 0x01, 0, 1, x);					// U6.P0(S_GPIO0)
#define	DE_485(x)				IOExp_Field(1, 0x01, 1, 1, x);					// U6.P1(485_DE)
#define	S_GPIO1(x)				IOExp_Field(1, 0x01, 4, 1, x);					// U6.P4(S_GPIO1)
#define	MCU_CS(x)				IOExp_Field(1, 0x01, 5, 1, x);					// U6.P5(MCU_CS)
#define	OLPF_DY(x)				IOExp_Field(1, 0x01, 6, 1, x);					// U6.P6(DAY_CTRL)
#define	OLPF_NT(x)				IOExp_Field(1, 0x01, 7, 1, x);					// U6.P7(NIGHT_CTRL)
#define	GET_HDMI_TX_INT()		(IOExp_Read(1, 0x00)&1<<2) >>2					// U6.P2(HDMI_TX_INT)
#define	GET_SWITCH()			(IOExp_Read(1, 0x00)&1<<3) >>3					// U6.P3(SWITCH)
#define	TVI_RESET(x)
#define	HDTX_RESET(x)
#endif

#if	defined(__MDIN_i510_IMX290__)													// i510 + IMX290 board
#define	SENSOR_RSTN(x)	(x)?	(*R_POHIGH(1) = 1<<0) : (*R_POLOW(1) = 1<<0)	// 1.0(SENSOR_RESETN)
#define	SDI_DIV_1001(x)	(x)?	(*R_POHIGH(1) = 1<<2) : (*R_POLOW(1) = 1<<2)	// 1.2(SDI_DIV_1001)
#define	DE_485(x)		(x)?	(*R_POHIGH(1) = 1<<3) : (*R_POLOW(1) = 1<<3)	// 1.3(485_DE)
#define	OLPF_DY(x)		(x)?	(*R_POHIGH(1) = 1<<4) : (*R_POLOW(1) = 1<<4)	// 1.4(DAY_CTRL) **
#define	OLPF_NT(x)		(x)?	(*R_POHIGH(1) = 1<<5) : (*R_POLOW(1) = 1<<5)	// 1.5(NIGHT_CTRL) **
#define	SDI_DET_TRS(x)	(x)?	(*R_POHIGH(1) = 1<<6) : (*R_POLOW(1) = 1<<6)	// 1.6(SDI_DETECT_TRS)
#define	SDI_20B_10B(x)	(x)?	(*R_POHIGH(1) = 1<<7) : (*R_POLOW(1) = 1<<7)	// 1.7(SDI_20BIT_10BIT)
#define	SDI_RATE(x)		(x)?	(*R_POHIGH(2) = 1<<1) : (*R_POLOW(2) = 1<<1)	// 2.1(SDI_RATE_SEL)
#define	SDI_TX_CS(x)	(x)?	(*R_POHIGH(2) = 1<<2) : (*R_POLOW(2) = 1<<2)	// 2.2(SDI_TX_CS)
#define	CMOS_CS(x)																// cpu_spi0_ssx
#define	LENS_VD(x)
#define	IRIS_VD(x)
#define	LENS_CS(x)
#define	IRIS_CS(x)
#endif



#if	defined(__MDIN_i540_EDK__) || defined(__MDIN_i540_REF__)						// i540 EDK & REF board
#define	SET_SCL_GPIO(x)	(x)?	(*R_POHIGH(0) = 1<<0) : (*R_POLOW(0) = 1<<0)	// 0.0(SCL_GPIO)
#define	GET_SCL_GPIO()			((*R_PILEV(0))&1<<0) >>0						// 0.0(SCL_GPIO)
#define	DE_485(x)			(x)?	(*R_POHIGH(0) = 1<<1) : (*R_POLOW(0) = 1<<1)	// 0.1(485_DE)
//#define	MD_OUT(x)		(x)?	(*R_POHIGH(0) = 1<<2) : (*R_POLOW(0) = 1<<2)	// 0.2(ISP_MD_OUT)
#define	SET_SDA_GPIO(x)	(x)?	(*R_POHIGH(0) = 1<<4) : (*R_POLOW(0) = 1<<4)	// 0.4(SDA_GPIO)
#define	GET_SDA_GPIO()			((*R_PILEV(0))&1<<4) >>4						// 0.4(SDA_GPIO)
#define	HDTX_RESET(x)		(x)?	(*R_POHIGH(0) = 1<<5) : (*R_POLOW(0) = 1<<5)	// 0.5(TX_RESETN_MDx)
#define	OLPF_DY(x)			(x)?	(*R_POHIGH(0) = 1<<6) : (*R_POLOW(0) = 1<<6)	// 0.6(DAY_CTRL) **
#define	OLPF_NT(x)			(x)?	(*R_POHIGH(0) = 1<<7) : (*R_POLOW(0) = 1<<7)	// 0.7(NIGHT_CTRL) **
#define	SENSOR_RSTN(x)		(x)?	(*R_POHIGH(1) = 1<<0) : (*R_POLOW(1) = 1<<0)	// 1.0(SENSOR_RESETN) **
#define	LENS_VD(x)			(x)?	(*R_POHIGH(1) = 1<<2) : (*R_POLOW(1) = 1<<2)	// 1.2(LENS_VD)
#define	IRIS_VD(x)			(x)?	(*R_POHIGH(1) = 1<<3) : (*R_POLOW(1) = 1<<3)	// 1.3(IRIS_VD)
#define	GET_LENS_FRST()		((*R_PILEV(1))&1<<4) >>4						// 1.4(FOCUS_RESET)
#define	GET_LENS_ZRST()		((*R_PILEV(1))&1<<5) >>5						// 1.5(ZOOM_RESET)

#define	LENS_CS(x)			(x)?	(*R_POHIGH(2) = 1<<1) : (*R_POLOW(2) = 1<<1)	// 2.1(ZF_CS)
#define	IRIS_CS(x)			(x)?	(*R_POHIGH(2) = 1<<1) : (*R_POLOW(2) = 1<<1)	// 2.1(ZF_CS) -- 41908
#define	MISP_CS_S2(x)		(x)?	(*R_POHIGH(2) = 1<<2) : (*R_POLOW(2) = 1<<2)	// 2.2(MISP_CS_S2)
#define	CMOS_CS(x)																// cpu_spi0_ssx
#define	SDI_RATE(x)																// SDI_RATE_SEL
#define	TVI_RESET(x)
#define	MISP_CS_S1(x)

#define	WR_SDA_GPIO()			{*R_PODIR(0) |= 1<<4;}							// 0.4 gpio direction 'write'
#define	RD_SDA_GPIO()			{*R_PIDIR(0) |= 1<<4;}							// 0.4 gpio direction 'read'
#if defined(__USE_GPIO_MTC_AF__)
#define GPIO_MFC_AP(x)		(x)? (*R_POHIGH(2) = 1<<0) : (*R_POLOW(2) = 1<<0)	// Motor Focus Control A+
#define GPIO_MFC_AM(x)		(x)? (*R_POHIGH(2) = 1<<1) : (*R_POLOW(2) = 1<<1)	// "				   A-
#define GPIO_MFC_BP(x)		(x)? (*R_POHIGH(2) = 1<<2) : (*R_POLOW(2) = 1<<2)	// "				   B+
#define GPIO_MFC_BM(x)		(x)? (*R_POHIGH(2) = 1<<3) : (*R_POLOW(2) = 1<<3)	// "				   B-

#define GPIO_MZC_AP(x)		(x)? (*R_POHIGH(2) = 1<<4) : (*R_POLOW(2) = 1<<4)	// Motor Zoom Control  A+
#define GPIO_MZC_AM(x)		(x)? (*R_POHIGH(2) = 1<<5) : (*R_POLOW(2) = 1<<5)	// "				   A-
#define GPIO_MZC_BP(x)		(x)? (*R_POHIGH(2) = 1<<6) : (*R_POLOW(2) = 1<<6)	// "				   B+
#define GPIO_MZC_BM(x)		(x)? (*R_POHIGH(2) = 1<<7) : (*R_POLOW(2) = 1<<7)	// "				   B-
#endif

#if defined(__USE_GPIO_MTC_AE__)
#define GPIO_MEC_AP(x)		(x)? (*R_POHIGH(2) = 1<<0) : (*R_POLOW(2) = 1<<0)	// Motor AE Control A+
#define GPIO_MEC_AM(x)		(x)? (*R_POHIGH(2) = 1<<1) : (*R_POLOW(2) = 1<<1)	// "				A-
#define GPIO_MEC_BP(x)		(x)? (*R_POHIGH(2) = 1<<2) : (*R_POLOW(2) = 1<<2)	// "				B+
#define GPIO_MEC_BM(x)		(x)? (*R_POHIGH(2) = 1<<3) : (*R_POLOW(2) = 1<<3)	// "				B-
#endif

#if defined(__ECOM1_UNION_12X__)	
#define OSC_Select(x)		(x)?	(*R_POHIGH(0) = 1<<5) : (*R_POLOW(0) = 1<<5)	// 0.6(OSC SEL) ** LOW = 27Mhz, HIGH = 26.9865Mhz			
#define	P_IRIS_AP_T(x)		(x)?	(*R_POHIGH(0) = 1<<4) : (*R_POLOW(0) = 1<<4)	// 0.4 (P IRIS A+)
#define	P_IRIS_AM_T(x)		(x)?	(*R_POHIGH(0) = 1<<5) : (*R_POLOW(0) = 1<<5)	// 0.4 (P IRIS A-)
#define	P_IRIS_BP_T(x)		(x)?	(*R_POHIGH(0) = 1<<6) : (*R_POLOW(0) = 1<<6)	// 0.4 (P IRIS B+)
#define	P_IRIS_BM_T(x)		(x)?	(*R_POHIGH(0) = 1<<7) : (*R_POLOW(0) = 1<<7)	// 0.4 (P IRIS B-)
#endif
	
#endif	// __MDIN_i540_EDK__, __MDIN_i540_REF__



#if	defined(__MDIN_i540_4K60__)													// i540 4K60
#define	DE_485(x)			(x)?	(*R_POHIGH(0) = 1<<1) : (*R_POLOW(0) = 1<<1)	// 0.1(485_DE)
#define	HDTX_RESET(x)		(x)?	(*R_POHIGH(0) = 1<<3) : (*R_POLOW(0) = 1<<3)	// 0.3(TX_RESETN_MDx)
#define	ZF_MISO_EN(x)		(x)?	(*R_POHIGH(0) = 1<<5) : (*R_POLOW(0) = 1<<5)	// 0.5(41908_SPI_EN)
#define	OLPF_DY(x)			(x)?	(*R_POHIGH(0) = 1<<6) : (*R_POLOW(0) = 1<<6)	// 0.6(DAY_CTRL)
#define	OLPF_NT(x)			(x)?	(*R_POHIGH(0) = 1<<7) : (*R_POLOW(0) = 1<<7)	// 0.7(NIGHT_CTRL)
#define	SENSOR_RSTN(x)		(x)?	(*R_POHIGH(1) = 1<<0) : (*R_POLOW(1) = 1<<0)	// 1.0(SENSOR_RESETN)
#define	LENS_VD(x)			(x)?	(*R_POHIGH(1) = 1<<2) : (*R_POLOW(1) = 1<<2)	// 1.2(LENS_VD)
#define	IRIS_VD(x)			(x)?	(*R_POHIGH(1) = 1<<3) : (*R_POLOW(1) = 1<<3)	// 1.3(IRIS_VD)
#define	GET_LENS_FRST()		((*R_PILEV(1))&1<<4) >>4						// 1.4(FOCUS_RESET)
#define	GET_LENS_ZRST()		((*R_PILEV(1))&1<<5) >>5						// 1.5(ZOOM_RESET)
#define	ZF_CS(x)			(x)?	(*R_POHIGH(2) = 1<<1) : (*R_POLOW(2) = 1<<1)	// 2.1(ZF_CS)
#define	MISP_CS_S2(x)		(x)?	(*R_POHIGH(2) = 1<<2) : (*R_POLOW(2) = 1<<2)	// 2.2(MISP_CS_S2)

#define	MISP_CS_S1(x)															// MISP_CS_S1
#define	CMOS_CS(x)																// cpu_spi0_ssx
#define	SDI_RATE(x)																// SDI_RATE_SEL
//#define	TVI_RESET(x)
#define	WR_SDA_GPIO()															// gpio direction 'write'
#define	RD_SDA_GPIO()															// gpio direction 'read'
#define	SET_SCL_GPIO(x)															// SCL_GPIO
#define	GET_SCL_GPIO()			0												// SCL_GPIO
#define	SET_SDA_GPIO(x)															// SDA_GPIO
#define	GET_SDA_GPIO()			0												// SDA_GPIO
#define	LENS_CS(x)				{ZF_MISO_EN(x);		ZF_CS(x);}
#define	IRIS_CS(x)				{ZF_MISO_EN(x);		ZF_CS(x);}
#endif	// __MDIN_i540_4K60__



#if	defined(__MDIN_i550_EDK__) || defined(__MDIN_i550_REF__)						// i550 EDK & REF board
#define	SET_SCL_GPIO(x)	(x)?	(*R_POHIGH(0) = 1<<0) : (*R_POLOW(0) = 1<<0)	// 0.0(SCL_GPIO)
#define	GET_SCL_GPIO()			((*R_PILEV(0))&1<<0) >>0						// 0.0(SCL_GPIO)
#define	DE_485(x)		(x)?	(*R_POHIGH(0) = 1<<1) : (*R_POLOW(0) = 1<<1)	// 0.1(485_DE)
//#define	MD_OUT(x)		(x)?	(*R_POHIGH(0) = 1<<2) : (*R_POLOW(0) = 1<<2)	// 0.2(ISP_MD_OUT)
#define	SET_SDA_GPIO(x)	(x)?	(*R_POHIGH(0) = 1<<4) : (*R_POLOW(0) = 1<<4)	// 0.4(SDA_GPIO)
#define	GET_SDA_GPIO()			((*R_PILEV(0))&1<<4) >>4						// 0.4(SDA_GPIO)
#define	MISP_CS_S1(x)	(x)?	(*R_POHIGH(0) = 1<<5) : (*R_POLOW(0) = 1<<5)	// 0.5(MISP_CS_S1)
#define	OLPF_DY(x)		(x)?	(*R_POHIGH(0) = 1<<6) : (*R_POLOW(0) = 1<<6)	// 0.6(DAY_CTRL) **
#define	OLPF_NT(x)		(x)?	(*R_POHIGH(0) = 1<<7) : (*R_POLOW(0) = 1<<7)	// 0.7(NIGHT_CTRL) **
#define	SENSOR_RSTN(x)	(x)?	(*R_POHIGH(1) = 1<<0) : (*R_POLOW(1) = 1<<0)	// 1.0(SENSOR_RESETN) **
#define	LENS_VD(x)		(x)?	(*R_POHIGH(1) = 1<<2) : (*R_POLOW(1) = 1<<2)	// 1.2(LENS_VD)
#define	IRIS_VD(x)		(x)?	(*R_POHIGH(1) = 1<<3) : (*R_POLOW(1) = 1<<3)	// 1.3(IRIS_VD)
#define	GET_LENS_FRST()			((*R_PILEV(1))&1<<4) >>4						// 1.4(FOCUS_RESET)
#define	GET_LENS_ZRST()			((*R_PILEV(1))&1<<5) >>5						// 1.5(ZOOM_RESET)
#define	LENS_CS(x)		(x)?	(*R_POHIGH(2) = 1<<1) : (*R_POLOW(2) = 1<<1)	// 2.1(ZF_CS)
#define	IRIS_CS(x)		(x)?	(*R_POHIGH(2) = 1<<1) : (*R_POLOW(2) = 1<<1)	// 2.1(ZF_CS) -- 41908
#define	MISP_CS_S2(x)	(x)?	(*R_POHIGH(2) = 1<<2) : (*R_POLOW(2) = 1<<2)	// 2.2(MISP_CS_S2)
#define	CMOS_CS(x)																//cpu_spi0_ssx
#define	SDI_RATE(x)																// SDI_RATE_SEL
#define	TVI_RESET(x)
#define	HDTX_RESET(x)

#define	WR_SDA_GPIO()			{*R_PODIR(0) |= 1<<4;}							//0.4 gpio direction 'write'
#define	RD_SDA_GPIO()			{*R_PIDIR(0) |= 1<<4;}							//0.4 gpio direction 'read'
#endif	// __MDIN_i550_EDK__, __MDIN_i550_REF__



#if	defined(__MDIN_i5XX_FPGA__)														// FPGA board
#define	OLPF_DY(x)		(x)?	(*R_POHIGH(0) = 1<<4) : (*R_POLOW(0) = 1<<4)	//0.4(DAY_CTRL)
#define	OLPF_NT(x)		(x)?	(*R_POHIGH(0) = 1<<5) : (*R_POLOW(0) = 1<<5)	//0.5(NIGHT_CTRL)
#define	MD_OUT(x)		(x)?	(*R_POHIGH(0) = 1<<6) : (*R_POLOW(0) = 1<<6)	//0.6(FPGA_MD_OUT)
#if defined(__USE_AN41908__)
#define	IRIS_CS(x)		(x)?	(*R_POHIGH(0) = 1<<7) : (*R_POLOW(0) = 1<<7)	// 0.7(ZF_CS)
#elif defined(__USE_AN41919__)
#define	IRIS_CS(x)		(x)?	(*R_POHIGH(2) = 1<<2) : (*R_POLOW(2) = 1<<2)	// 2.2(IRIS_CS)
#endif
#define	IRIS_VD(x)		(x)?	(*R_POHIGH(1) = 1<<5) : (*R_POLOW(1) = 1<<5)	// 1.5(VD_IN)
#define	CMOS_CS(x)		(x)?	(*R_POHIGH(2) = 1<<3) : (*R_POLOW(2) = 1<<3)	// 2.3(MICOM_SENSOR_CS)
#define	LENS_VD(x)		(x)?	(*R_POHIGH(2) = 1<<5) : (*R_POLOW(2) = 1<<5)	// 2.5(MTR_VD)
#define	TRIG_OUT(x)
#define	LENS_CS(x)		(x)?	(*R_POHIGH(0) = 1<<7) : (*R_POLOW(0) = 1<<7)	// 0.7(ZF_CS)
#define	DE_485(x)		(x)?	(*R_POHIGH(2) = 1<<4) : (*R_POLOW(2) = 1<<4)	// 2.4(485_DE)
#define	SDI_RATE(x)
#define	GET_LENS_FRST()			((*R_PILEV(0))&1<<1) >>1						// 0.1(PD2, R111)
#define	GET_LENS_ZRST()			((*R_PILEV(2))&1<<1) >>1						// 2.1(N.C, R106)
#endif	// __MDIN_i5XX_FPGA__


// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------


#endif	/* __BOARD_H__ */
