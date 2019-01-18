//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2005  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	MISP100.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"
#include	"osd.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
BYTE	pwm5_high=3, pwm5_low=5;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
MISP_SYSTEM_INFO	stSystemInfo;

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void MISP_Initialize(void)
{
//	MISP_SW_Reset();
	
	Init_Arbiter();
	Init_HostCLK();
	Init_HOST();

	Init_SensorTop();	

	CMOS_Initialize();  //must be set after Init_SensorTop()

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	Init_BayerFR();		
	Init_ROT();

	Init_DynamicDPC();
#endif
	
	Init_IN();
	
	Init_NR();	
	Init_DDRMAP() ;	
	
	MISP_SW_Reset();

	// Init_IN();
	Init_OUT();
	Init_FC();

#if defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__) // for 4k sensor slave mode
	SetSS_reset_des_fifo();
#endif

#if defined(__USE_MDIN_i500__)		// MDIN-i500
	Init_BayerFR();
#endif

#if defined(__USE_4K60_INOUT__)
	SetAPP_Set4KUartStop(ON);		// skip & backup 'uart message'
#endif
	
	Init_ODM();
	#if	defined(__USE_CMOS_IMX124__)	
//	IMX124_DCKRST_Sequence();
	#endif

	Init_Demosaic();
	Init_CCF();
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	Init_TONE();
	Init_CSP();
	Init_CAC();	
#endif
	Init_ADC();	
	Init_WDR();
#if defined(__USE_FD_PROCESS__)
	Init_FD();
#endif
	
	Init_DIS();
	Init_MDPM();


	//SetStaticDPC_init_flag(0);		//static dpc re-setting if needs

	MISP_SW_Reset();

#if defined(__USE_CMOS_IMX226__)  // for 4k sensor slave mode & clk conversion
	SetSS_reset_des_fifo();
#endif
	
#if defined(__USE_4K60_INOUT__)
	SetAPP_Set4KOutSyncLock();
	SetAPP_Set4KUartStop(OFF);		// restore 'uart message'
#endif
	
	
	SetSYSINFO_IspInitDone();
}


//--------------------------------------------------------------------------------------------------------------------------
void MISP_SW_Reset(void){

	SetDDRMAP_ddr_refresh_enable_n(1);	// disable refresh
	SetDDRMAP_ddr_access_enable(0) ;	// disable access
	
	
	MISP_RegWrite(MISP_HOST_ID,  0x070, 0x0001);	// S/W reset
	MISP_RegWrite(MISP_HOST_ID,  0x070, 0x0000);	//

	SetDDRMAP_ddr_access_enable(1) ;	// enable refresh		
	SetDDRMAP_ddr_refresh_enable_n(0);	// enable access	

}

//--------------------------------------------------------------------------------------------------------------------------
static void PrintSystemInfo(void)
{
	UARTprintf("Board \t: ");
	switch(stSystemInfo.hw_info&MISP_SYSINFO_BOARD_MASK)	{
		case MISP_SYSINFO_I5x0FPGA:			UARTprintf("MDIN-i5x0 FPGA BD \r\n");	break;
		case MISP_SYSINFO_I500EDK:			UARTprintf("MDIN-i500 EDK V0.1 \r\n");	break;
		case MISP_SYSINFO_I500REF:			UARTprintf("MDIN-i500 REF V0.1 \r\n");	break;
		case MISP_SYSINFO_I510EDK_V10:		UARTprintf("MDIN-i510 EDK V1.0 \r\n");	break;
		case MISP_SYSINFO_I510EDK_V11:		UARTprintf("MDIN-i510 EDK V1.1 \r\n");	break;
		case MISP_SYSINFO_I510REF_V10:		UARTprintf("MDIN-i510 REF V1.0 \r\n");	break;
		case MISP_SYSINFO_I510REF_V134:		UARTprintf("MDIN-i510 REF V1.34 \r\n");	break;
		case MISP_SYSINFO_I540EDK_V10:		UARTprintf("MDIN-i540 EDK V1.0 \r\n");	break;
		case MISP_SYSINFO_I540EDK_V11:		UARTprintf("MDIN-i540 EDK V1.1 \r\n");	break;
		case MISP_SYSINFO_I540REF_V10:		UARTprintf("MDIN-i540 REF V1.0 \r\n");	break;
		case MISP_SYSINFO_I540REF_V121:		UARTprintf("MDIN-i540 REF V1.21 \r\n");	break;
		case MISP_SYSINFO_I550EDK_V10:		UARTprintf("MDIN-i550 EDK V1.0 \r\n");	break;
		case MISP_SYSINFO_I550EDK_V11:		UARTprintf("MDIN-i550 EDK V1.1 \r\n");	break;
		case MISP_SYSINFO_I550REF_V10:		UARTprintf("MDIN-i550 REF V1.0 \r\n");	break;
		case MISP_SYSINFO_I550REF_V121:		UARTprintf("MDIN-i550 REF V1.21 \r\n");	break;
		case MISP_SYSINFO_I510_IMX290:		UARTprintf("MDIN-i510 + IMX290 V1.0 \r\n");	break;
		case MISP_SYSINFO_I540_4K60_V10:	UARTprintf("MDIN-i540 4K60 V1.0 \r\n");	break;
		case MISP_SYSINFO_I550_USB_V10:		UARTprintf("MDIN-i550 USB V1.0 \r\n");	break;
		default:							UARTprintf("\r\n");	break;
	}
	
	UARTprintf("Sensor \t: ");
	switch(stSystemInfo.hw_info&MISP_SYSINFO_SENSOR_NO_MASK)	{
		case MISP_SYSINFO_SENSOR_IMX136:	UARTprintf("IMX136, ");		break;
		case MISP_SYSINFO_SENSOR_MN34220:	UARTprintf("MN34220, ");	break;
		case MISP_SYSINFO_SENSOR_IMX236:	UARTprintf("IMX236, ");		break;
		case MISP_SYSINFO_SENSOR_IMX124:	UARTprintf("IMX124, ");		break;
		case MISP_SYSINFO_SENSOR_IMX222:	UARTprintf("IMX222, ");		break;
		case MISP_SYSINFO_SENSOR_IMX185:	UARTprintf("IMX185, ");		break;
		case MISP_SYSINFO_SENSOR_IMX238:	UARTprintf("IMX238, ");		break;
		case MISP_SYSINFO_SENSOR_IMX174:	UARTprintf("IMX174, ");		break;
		case MISP_SYSINFO_SENSOR_AR0331:	UARTprintf("AR0331, ");		break;
		case MISP_SYSINFO_SENSOR_MT9P031:	UARTprintf("MT9P031, ");	break;
		case MISP_SYSINFO_SENSOR_MT9M131:	UARTprintf("MT9M131, ");	break;
		case MISP_SYSINFO_SENSOR_MN34041:	UARTprintf("MN34041, ");	break;
		case MISP_SYSINFO_SENSOR_IMX076:	UARTprintf("IMX076, ");		break;
		case MISP_SYSINFO_SENSOR_PO3100K:	UARTprintf("PO3100K, ");	break;
		case MISP_SYSINFO_SENSOR_PS1210K:	UARTprintf("PS1210K, ");	break;
		case MISP_SYSINFO_SENSOR_IMX290:	UARTprintf("IMX290, ");		break;		
		case MISP_SYSINFO_SENSOR_IMX322:	UARTprintf("IMX322, ");		break;				
		case MISP_SYSINFO_SENSOR_IMX178: 	UARTprintf("IMX178, ");		break;
		case MISP_SYSINFO_SENSOR_IMX226: 	UARTprintf("IMX226, ");		break;	
		case MISP_SYSINFO_SENSOR_IMX274: 	UARTprintf("IMX274, ");		break;
		case MISP_SYSINFO_SENSOR_EXT3: 		UARTprintf("EXT3 ");		break;				
		case MISP_SYSINFO_SENSOR_EXT2: 		UARTprintf("EXT2, ");		break;			
		case MISP_SYSINFO_SENSOR_EXT1: 		UARTprintf("EXT1, ");		break;		
		case MISP_SYSINFO_SENSOR_OV2710: 	UARTprintf("OV2710, ");		break;	
		case MISP_SYSINFO_SENSOR_IMX265:	UARTprintf("IMX265, ");		break;		
		case MISP_SYSINFO_SENSOR_IMX253:	UARTprintf("IMX253, ");		break;			
		case MISP_SYSINFO_SENSOR_IMX273:	UARTprintf("IMX273, ");		break;				
		case MISP_SYSINFO_SENSOR_MN34420:	UARTprintf("MN34420, ");	break;		
		case MISP_SYSINFO_SENSOR_PS1400K:	UARTprintf("PS1410K, ");	break;				
#if defined (__USE_CMOS_AR0134__)
		case MISP_SYSINFO_SENSOR_AR0134:	UARTprintf("AR0134, ");	break;	
#else
		case MISP_SYSINFO_SENSOR_AR0134:	UARTprintf("AR0135, ");	break;	
#endif
		case MISP_SYSINFO_SENSOR_IMX252:	UARTprintf("IMX252, ");		break;		
		case MISP_SYSINFO_SENSOR_IMX385:	UARTprintf("IMX385, ");		break;		
		case MISP_SYSINFO_SENSOR_IMX327:	UARTprintf("IMX327, ");		break;		
		case MISP_SYSINFO_SENSOR_IMX334:	UARTprintf("IMX334, ");		break;				
		case MISP_SYSINFO_SENSOR_EXT2IPP:	UARTprintf("EXT2IPP,");		break;		
		
	}
	
	switch(stSystemInfo.hw_info&MISP_SYSINFO_SENSOR_OUTPUT_MASK)	{
		case MISP_SYSINFO_SENSOR_LVDS8:	UARTprintf("LVDS Serial 8CH \r\n");		break;
		case MISP_SYSINFO_SENSOR_LVDS6:	UARTprintf("LVDS Serial 6CH \r\n");		break;		
		case MISP_SYSINFO_SENSOR_LVDS4:	UARTprintf("LVDS Serial 4CH \r\n");		break;
		case MISP_SYSINFO_SENSOR_LVDS2:	UARTprintf("LVDS Serial 2CH \r\n");		break;
		case MISP_SYSINFO_SENSOR_LVDSP:	UARTprintf("LVDS Parallel \r\n");		break;
		case MISP_SYSINFO_SENSOR_TTL:	UARTprintf("CMOS Parallel(TTL) \r\n");	break;
		default:						UARTprintf("\r\n");	break;
	}
	
	UARTprintf("Lens \t: ");
	switch(stSystemInfo.hw_info&MISP_SYSINFO_LENS_TYPE_MASK)	{
		case MISP_SYSINFO_LENS_OLPF_DAIWON:	UARTprintf("DAIWON ZOOM"); 			break;
		case MISP_SYSINFO_LENS_OLPF_YOPTIC:	UARTprintf("YOPTIC ZOOM"); 			break;
		case MISP_SYSINFO_LENS_X30X_TAMRON:	UARTprintf("TAMRON X30"); 			break;
		case MISP_SYSINFO_LENS_OLPF_CANON:	UARTprintf("CANON ZOOM"); 			break;
		case MISP_SYSINFO_LENS_OLPF_SUNNY:	UARTprintf("SUNNY ZOOM"); 			break;
		case MISP_SYSINFO_LENS_X03X_ZMAX:	UARTprintf("ZMAX X03"); 			break;
		case MISP_SYSINFO_LENS_X10X_UNION:	UARTprintf("UNION X10"); 			break;
		case MISP_SYSINFO_LENS_X10X_ZMAX:	UARTprintf("ZMAX X10"); 			break;
		
		case MISP_SYSINFO_LENS_MANU_RV4515:	UARTprintf("RV4515"); 			break;
		case MISP_SYSINFO_LENS_MANU_RV0850:	UARTprintf("RV0850"); 		break;
		case MISP_SYSINFO_LENS_MANU_KAWADEN12120:	UARTprintf("KAWADEN12120"); 			break;
		case MISP_SYSINFO_LENS_MANU_RV1530:	UARTprintf("RV1530"); 			break;
		case MISP_SYSINFO_LENS_MANU_RX8FM1214C:	UARTprintf("RX8FM1214C"); 			break;
		case MISP_SYSINFO_LENS_MANU_TAMRON_5M:	UARTprintf("TAMRON 5M"); 			break;
		case MISP_SYSINFO_LENS_MANU_TAMRON:	UARTprintf("TAMRON"); 			break;
		case MISP_SYSINFO_LENS_MANU_TAMRON_PWM: UARTprintf("TAMRON_PWM"); 		break;
		case MISP_SYSINFO_LENS_MANU_SUNEX115: UARTprintf("SUNEX115"); 		break;
		case MISP_SYSINFO_LENS_MANU_RV3620: 	UARTprintf("RV3620"); 			break;
		case MISP_SYSINFO_LENS_MANU_RV3220: 	UARTprintf("RV3220"); 			break;
		case MISP_SYSINFO_LENS_MANU_BW3M30B4000: 	UARTprintf("BW3M30B4000"); 			break;
		case MISP_SYSINFO_LENS_MANU_BW4K40_1000: 	UARTprintf("BW4K40 1000"); 			break;
		case MISP_SYSINFO_LENS_MANU_BW3ML43B_1000: 	UARTprintf("BW3ML43B 1000"); 			break;
		case MISP_SYSINFO_LENS_MANU_RV10_FM1614A1: 	UARTprintf("RV10 FM1614A1"); 			break;
		case MISP_SYSINFO_LENS_MANU_WW_4KUHD: 	UARTprintf("WW 4KUHD"); 			break;
		case MISP_SYSINFO_LENS_MANU_WW_8mmIR: 	UARTprintf("WW 8mmIR"); 			break;
	}
	UARTprintf("\r\n");

	UARTprintf("IRIS \t: ");
	switch(stSystemInfo.hw_info&MISP_SYSINFO_DCIRIS_MASK)	{
		case MISP_SYSINFO_DCIRIS_HALL:	UARTprintf("AN41908(HALL) \r\n");		break;
		case MISP_SYSINFO_DCIRIS_DAMP:	UARTprintf("AN41919(DAMP) \r\n");		break;
		case MISP_SYSINFO_DCIRIS_NONE:	UARTprintf("NONE \r\n");		break;
		case MISP_SYSINFO_PWMIRIS_ONLY:	UARTprintf("PWM only \r\n");		break;
		case MISP_SYSINFO_P_IRIS_ONLY:	UARTprintf("P IRIS only \r\n");		break;
		default:						UARTprintf("\r\n");	break;
	}

#if defined(__USE_LT8619_RX__)
	UARTprintf("Rx \t: HDMI, LT8619 \r\n");
#endif
#if defined(__USE_IT6802_RX__)
	UARTprintf("Rx \t: HDMI, IT6802 \r\n");
#endif
	
#if defined(__USE_IT66121_TX__)
	UARTprintf("Tx \t: HDMI, IT66121 \r\n");
#endif
#if defined(__USE_IT6615_TX__)
	UARTprintf("Tx \t: HDMI, ITE6615  \r\n");
#endif
#if defined(__USE_LT8618EX_TX__)
	UARTprintf("Tx \t: HDMI, LT8618EX \r\n");
#endif
#if defined(__USE_LT8618SX_TX__)
	UARTprintf("Tx \t: HDMI, LT8618SX \r\n");
#endif
#if defined(__USE_SIL9136_TX__)
	UARTprintf("Tx \t: HDMI, SIL9136(1136) \r\n");
#endif
#if defined(__USE_EN332T__)
	UARTprintf("Tx \t: EX-SDI, EN332T \r\n");
#endif
#if defined(__USE_TVI_TX__)
	UARTprintf("Tx \t: TVI, TP2912 \r\n");
#endif
	
	UARTprintf("S/W \t: %04x, Release(%01x.%01x.%02x, svn:%d), Build[%s, %s] \r\n", \
					__FW_VERSION__, HI4BIT(HIBYTE(HIWORD(stSystemInfo.sw_ver))), \
					LO4BIT(HIBYTE(HIWORD(stSystemInfo.sw_ver))), LOBYTE(HIWORD(stSystemInfo.sw_ver)),	\
					LOWORD(stSystemInfo.sw_ver), __DATE__, __TIME__);
	UARTprintf("==========================================================\r\n\n");
}

//--------------------------------------------------------------------------------------------------------------------------
void SYSINFO_initialize(void)
{
#if defined (__USE_EEPROM__)
	LONG eep_hw_info=0, eep_sw_ver=0;
	CHAR eep_sw_date[12], eep_sw_time[9];
#endif
	
	memset(&stSystemInfo, 0, sizeof(MISP_SYSTEM_INFO));
	
	// board info
#if defined (__MDIN_i500_EDK__)
	stSystemInfo.hw_info |= MISP_SYSINFO_I500EDK;
#elif defined (__MDIN_i500_REF__)
	stSystemInfo.hw_info |= MISP_SYSINFO_I500REF;
#elif defined (__MDIN_i510_EDK_V10__)
	stSystemInfo.hw_info |= MISP_SYSINFO_I510EDK_V10;
#elif defined (__MDIN_i510_EDK_V11__)
	stSystemInfo.hw_info |= MISP_SYSINFO_I510EDK_V11;
#elif defined (__MDIN_i510_REF_V10__)
	stSystemInfo.hw_info |= MISP_SYSINFO_I510REF_V10;
#elif defined (__MDIN_i510_REF_V134__)
	stSystemInfo.hw_info |= MISP_SYSINFO_I510REF_V134;
#elif defined (__MDIN_i510_IMX290__)
	stSystemInfo.hw_info |= MISP_SYSINFO_I510_IMX290;
#elif defined (__MDIN_i540_EDK_V10__)
	stSystemInfo.hw_info |= MISP_SYSINFO_I540EDK_V10;
#elif defined (__MDIN_i540_EDK_V11__)
	stSystemInfo.hw_info |= MISP_SYSINFO_I540EDK_V11;
#elif defined (__MDIN_i540_REF_V10__)
	stSystemInfo.hw_info |= MISP_SYSINFO_I540REF_V10;
#elif defined (__MDIN_i540_REF_V121__)
	stSystemInfo.hw_info |= MISP_SYSINFO_I540REF_V121;
#elif defined (__MDIN_i540_4K60_V10__)
	stSystemInfo.hw_info |= MISP_SYSINFO_I540_4K60_V10;
#elif defined (__MDIN_i550_EDK_V10__)
	stSystemInfo.hw_info |= MISP_SYSINFO_I550EDK_V10;
#elif defined (__MDIN_i550_EDK_V11__)
	stSystemInfo.hw_info |= MISP_SYSINFO_I550EDK_V11;
#elif defined (__MDIN_i550_REF_V10__)
	stSystemInfo.hw_info |= MISP_SYSINFO_I550REF_V10;
#elif defined (__MDIN_i550_REF_V121__)
	stSystemInfo.hw_info |= MISP_SYSINFO_I550REF_V121;
#elif defined (__MDIN_i550_USB_V100__)
	stSystemInfo.hw_info |= MISP_SYSINFO_I550_USB_V10;
#else
	stSystemInfo.hw_info |= MISP_SYSINFO_I5x0FPGA;
#endif
	
	// sensor no
#if defined (__USE_CMOS_IMX136__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX136;
#elif defined (__USE_CMOS_MN34220__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_MN34220;
#elif defined (__USE_CMOS_MN34420__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_MN34420;
#elif defined (__USE_CMOS_IMX236__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX236;
#elif defined (__USE_CMOS_IMX124__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX124;
#elif defined (__USE_CMOS_IMX222__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX222;
#elif defined (__USE_CMOS_IMX185__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX185;
#elif defined (__USE_CMOS_IMX334__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX334;
#elif defined (__USE_CMOS_IMX238__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX238;
#elif defined (__USE_CMOS_IMX174__)  || defined(__USE_CMOS_IMX249__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX174;
#elif defined (__USE_CMOS_AR0331__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_AR0331;
#elif defined (__USE_CMOS_AR0134__)  || defined(__USE_CMOS_AR0135__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_AR0134;
#elif defined (__USE_CMOS_MT9P031__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_MT9P031;
#elif defined (__USE_CMOS_MT9M131__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_MT9M131;
#elif defined (__USE_CMOS_MN34041__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_MN34041;
#elif defined (__USE_CMOS_IMX076__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX076;
#elif defined (__USE_CMOS_PO3100K__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_PO3100K;
#elif defined (__USE_CMOS_PS1210K__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_PS1210K;
#elif defined (__USE_CMOS_IMX290__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX290;
#elif defined (__USE_CMOS_IMX322__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX322;
#elif defined(__USE_CMOS_IMX323__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX323;
#elif defined (__USE_CMOS_IMX178__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX178;
#elif defined (__USE_CMOS_IMX226__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX226;
#elif defined (__USE_CMOS_IMX274__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX274;
#elif defined (__USE_CMOS_IMX326__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX326;
#elif defined (__USE_CMOS_EXT3__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_EXT3;
#elif defined (__USE_CMOS_EXT2__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_EXT2;
#elif defined (__USE_CMOS_EXT1__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_EXT1;
#elif defined (__USE_CMOS_OV2710__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_OV2710;
#elif defined (__USE_CMOS_IMX265__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX265;
#elif defined (__USE_CMOS_IMX252__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX252;
#elif defined (__USE_CMOS_IMX253__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX253;
#elif defined (__USE_CMOS_IMX273__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX273;
#elif defined (__USE_CMOS_PS1400K__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_PS1400K;
#elif defined (__USE_CMOS_IMX385__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX385;
#elif defined (__USE_CMOS_IMX327__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_IMX327;
#endif
	
	// sensor output type
#if defined (__USE_CMOS_IMXx36_TTL__) || defined (__USE_CMOS_IMXx22__) || defined(__USE_CMOS_EXT3__) || defined(__USE_CMOS_EXT2__) || defined(__USE_CMOS_EXT1__) || defined(__USE_CMOS_OV2710__) || defined (__USE_CMOS_IMX323__) ||  defined(__USE_CMOS_AR0134_TTL__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_TTL;
#elif defined(__USE_CMOS_IMXx36_LVDS_P__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_LVDSP;
#elif defined(__USE_CMOS_IMXx36_LVDS_S2__) || defined(__USE_CMOS_AR0134_LVDS_S2__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_LVDS2;
#elif defined(__USE_CMOS_IMXx36_LVDS_S4__) || defined(__USE_IMX274_1080P60_MODE__) || defined(__USE_CMOS_IMX290_LVDS_S4__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_LVDS4;
#elif (defined(__USE_CMOS_IMX274__) && !defined(__USE_IMX274_1080P60_MODE__))
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_LVDS6;
#elif defined(__USE_CMOS_IMX290_LVDS_S8__) || defined(__USE_IMX226_2160P30_MODE0__)
	stSystemInfo.hw_info |= MISP_SYSINFO_SENSOR_LVDS8;
#endif
	
	// lens type
#if defined (__USE_OLPF_DAIWON__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_OLPF_DAIWON;
#elif defined (__USE_OLPF_YOPTIC__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_OLPF_YOPTIC;
#elif defined (__USE_X30X_TAMRON__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_X30X_TAMRON;
#elif defined (__USE_X33X_CHINA__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_OLPF_CANON;
#elif defined (__USE_OLPF_CANON__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_OLPF_SUNNY;
#elif defined (__USE_X03X_ZMAX__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_X03X_ZMAX;
#elif defined (__USE_YT3010_2MOTOR__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_YT3010_2MOTOR;
#elif defined (__USE_X10X_UNION__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_X10X_UNION;
#elif defined (__USE_X10X_ZMAX__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_X10X_ZMAX;
#elif defined (__USE_X12X_UNION__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_X12X_UNION;
#elif defined (__USE_MANU_RV4515__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_MANU_RV4515;
#elif defined (__USE_MANU_RV0850__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_MANU_RV0850;
#elif defined (__USE_MANU_KAWADEN12120__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_MANU_KAWADEN12120;
#elif defined (__USE_MANU_RV1530__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_MANU_RV1530;
#elif defined (__USE_MANU_RX8FM1214C__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_MANU_RX8FM1214C;
#elif defined (__USE_MANU_TAMRON_5M__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_MANU_TAMRON_5M;
#elif defined (__USE_MANU_TAMRON__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_MANU_TAMRON;
#elif defined (__USE_MANU_TAMRON_PWM__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_MANU_TAMRON_PWM;
#elif defined (__USE_MANU_SUNEX115__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_MANU_SUNEX115;
#elif defined (__USE_MANU_RV3620__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_MANU_RV3620;
#elif defined (__USE_MANU_RV3220__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_MANU_RV3220;
#elif defined (__USE_MANU_BW3M30B4000__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_MANU_BW3M30B4000;
#elif defined (__USE_MANU_BW4K40_1000__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_MANU_BW4K40_1000;
#elif defined (__USE_MANU_BW3ML43B_1000__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_MANU_BW3ML43B_1000;
#elif defined (__USE_MANU_RV10_FM1614A1__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_MANU_RV10_FM1614A1;
#elif defined (__USE_MANU_WW_4KUHD__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_MANU_WW_4KUHD;
#elif defined (__USE_MANU_WW_8mmIR__)
	stSystemInfo.hw_info |= MISP_SYSINFO_LENS_MANU_WW_8mmIR;
#endif

  	// iris info
#if defined (__USE_DCIRIS_HALL__)
	stSystemInfo.hw_info |= MISP_SYSINFO_DCIRIS_HALL;
#elif defined (__USE_DCIRIS_DAMP__)
	stSystemInfo.hw_info |= MISP_SYSINFO_DCIRIS_DAMP;
#elif defined (__USE_DCIRIS_NONE__)
	stSystemInfo.hw_info |= MISP_SYSINFO_DCIRIS_NONE;
#elif defined (__USE_PWMIRIS_ONLY__)
	stSystemInfo.hw_info |= MISP_SYSINFO_PWMIRIS_ONLY;
#elif defined(__USE_P_IRIS_ONLY__)
	stSystemInfo.hw_info |= MISP_SYSINFO_P_IRIS_ONLY;
#endif
	
	
	// s/w info
	stSystemInfo.sw_ver = GetMenu_FirmwareVer();
	
	
	
	SetSYSINFO_CopyHwInfo(stSystemInfo.hw_info);	// copy h/w info to debug area in dspm
	SetSYSINFO_CopySwVer(stSystemInfo.sw_ver);		// copy s/w version to debug area in dspm
	
	PrintSystemInfo();
	
	
#if defined (__USE_EEPROM__)
	// check eeprom h/w & s/w info
	memset(&eep_sw_date[0], 0, sizeof(eep_sw_date));
	memset(&eep_sw_time[0], 0, sizeof(eep_sw_time));
	
	EEPROM_MultiRead(EEPROM_HW_INFO, (PBYTE)&eep_hw_info, 4); 				// read h/w info
	EEPROM_MultiRead(EEPROM_SW_VER, (PBYTE)&eep_sw_ver, 4); 				// read s/w ver
	EEPROM_MultiRead(EEPROM_SW_DATE, (PBYTE)&eep_sw_date[0], 12); 			// read s/w build date
	EEPROM_MultiRead(EEPROM_SW_TIME, (PBYTE)&eep_sw_time[0], 9); 			// read s/w build time
	
	if(eep_hw_info != stSystemInfo.hw_info)		goto EEPROM_VER_NG;		// check h/w info
	if(eep_sw_ver != stSystemInfo.sw_ver)		goto EEPROM_VER_NG;		// check s/w ver
	if(strcmp(eep_sw_date, __DATE__))			goto EEPROM_VER_NG;		// check s/w build date
	if(strcmp(eep_sw_time, __TIME__))			goto EEPROM_VER_NG;		// check s/w build time
 	
	goto EEPROM_CHK_BOOT_CNT;
	
	
	
EEPROM_VER_NG:
	UARTprintf("New s/w downloaded...  Erase all eeprom data.. \r\n");
	EraseE2promMenuStatus();
	EEPROM_MultiWrite(EEPROM_HW_INFO, (PBYTE)&stSystemInfo.hw_info, 4); 	// write h/w info
	EEPROM_MultiWrite(EEPROM_SW_VER, (PBYTE)&stSystemInfo.sw_ver, 4); 		// write s/w ver
	EEPROM_MultiWrite(EEPROM_SW_DATE, (PBYTE)&__DATE__, 12); 				// write s/w build date
	EEPROM_MultiWrite(EEPROM_SW_TIME, (PBYTE)&__TIME__, 9); 				// write s/w build time


EEPROM_CHK_BOOT_CNT:
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	// check boot counter
	if(EEPROM_RegRead(EEPROM_BOOT_CNT) == EEPROM_SAVE_DONE)	{
		EEPROM_MultiRead(EEPROM_BOOT_CNT+2, (PBYTE)&stSystemInfo.boot_cnt, 4); 		// read boot counter([31:16]boot_ok_cnt, [15:0]boot_cnt)
		if(LOWORD(stSystemInfo.boot_cnt) != 0xffff)		stSystemInfo.boot_cnt++;	// increase boot counter
		EEPROM_MultiWrite(EEPROM_BOOT_CNT+2, (PBYTE)&stSystemInfo.boot_cnt, 4); 		// write boot counter
	}
#endif
	return;
	
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
DWORD GetSYSINFO_BoardType(void)
{
	return	(stSystemInfo.hw_info & MISP_SYSINFO_BOARD_MASK);
}

//--------------------------------------------------------------------------------------------------------------------------
DWORD GetSYSINFO_SensorNo(void)
{
	return	(stSystemInfo.hw_info & MISP_SYSINFO_SENSOR_NO_MASK);
}

//--------------------------------------------------------------------------------------------------------------------------
DWORD GetSYSINFO_SensorOutType(void)
{
	return	(stSystemInfo.hw_info & MISP_SYSINFO_SENSOR_OUTPUT_MASK);	// 
}

//--------------------------------------------------------------------------------------------------------------------------
DWORD GetSYSINFO_LensType(void)
{
	return	(stSystemInfo.hw_info & MISP_SYSINFO_LENS_TYPE_MASK);	// lens model no
}

//--------------------------------------------------------------------------------------------------------------------------
DWORD GetSYSINFO_IrisType(void)
{
	return	(stSystemInfo.hw_info & MISP_SYSINFO_DCIRIS_MASK);		// Hall, Damp
}

//--------------------------------------------------------------------------------------------------------------------------
DWORD GetSYSINFO_SWVer(void)
{
	return	stSystemInfo.sw_ver;
}

//--------------------------------------------------------------------------------------------------------------------------
DWORD GetSYSINFO_BootCnt(void)
{
	return	stSystemInfo.boot_cnt;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetTIMER_PWM5LowWidth(BYTE ms)
{
	pwm5_low = ms;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetTIMER_PWM5HighWidth(BYTE ms)
{
	pwm5_high = ms;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetSYSINFO_CpuInitDone(void)
{
	*M_SYSINFO_INIT = MISP_SYSINFO_INIT_CPU_COMPLETE;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetSYSINFO_SensorInitDone(void)
{
	*M_SYSINFO_INIT |= MISP_SYSINFO_INIT_SENSOR_COMPLETE;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetSYSINFO_LensInitDone(void)
{
	*M_SYSINFO_INIT |= MISP_SYSINFO_INIT_LENS_COMPLETE;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetSYSINFO_IspInitDone(void)
{
	*M_SYSINFO_INIT |= MISP_SYSINFO_INIT_ISP_COMPLETE;
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	if(EEPROM_RegRead(EEPROM_BOOT_CNT) == EEPROM_SAVE_DONE)	{
		if(HIWORD(stSystemInfo.boot_cnt) != 0xffff)		stSystemInfo.boot_cnt += 0x10000;	// increase boot ok counter
		EEPROM_MultiWrite(EEPROM_BOOT_CNT+2, (PBYTE)&stSystemInfo.boot_cnt, 4); 			// write boot ok counter
	}
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetSYSINFO_CopyHwInfo(DWORD val)
{
	*M_SYSINFO_HW_INFO = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetSYSINFO_CopySwVer(DWORD val)
{
	*M_SYSINFO_SW_VER = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void MISP_Reboot(void)
{
	*R_WDTCTRL &= (~F_WDTCTRL_WDTEN_EN);		// watchdog disable
	
	INTERRUPT_DISABLE;							// interrupt disable
	
	// timer disable
	*R_TM0CTRL = F_TMCTRL_TMEN_DIS;
	*R_TM1CTRL = F_TMCTRL_TMEN_DIS;
	*R_TM2CTRL = F_TMCTRL_TMEN_DIS;
	*R_TM3CTRL = F_TMCTRL_TMEN_DIS;
	*R_TM4CTRL = F_TMCTRL_TMEN_DIS;
	*R_TM5CTRL = F_TMCTRL_TMEN_DIS;
	
	// wdt enable for reboot
	*R_WDTCNT = 0;												// set wdt counter for reboot
	*R_WDTCTRL = F_WDTCTRL_WDTMOD_RST | F_WDTCTRL_WDTEN_EN;		// wdt enable
}


/*  FILE_END_HERE */
