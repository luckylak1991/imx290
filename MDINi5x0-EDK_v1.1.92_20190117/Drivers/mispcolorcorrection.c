//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2016  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  mispcolorcorrection.c
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:


// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#include	"misp100.h"
#include	"video.h"

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
#if	defined(__USE_OLPF_DAIWON__) || defined(__USE_OLPF_YOPTIC__) || defined(__USE_X30X_TAMRON__) || defined(__USE_OLPF_CANON__)|\
	defined(__USE_X33X_CHINA__) || defined(__USE_X10X_UNION__) || defined(__USE_X10X_ZMAX__) || defined(__USE_X12X_UNION__)|\
	defined(__USE_MANU_WW_4KUHD__) || defined(__USE_MANU_WW_4KBOX__) || defined(__USE_MANU_WW_8mmIR__) || defined(__USE_MANU_RV10_FM1614A1__)
#define		__CC_USE_FILTER_RED_STYLE1__					//red filter default
#elif defined (__USE_MANU_BW3M30B4000__) || defined(__USE_MANU_RV3620__)
#define		__CC_USE_FILTER_BLUE_STYLE1__			//blue filter default
//#define		__CC_USE_FILTER_BLUE_STYLE2__				//blue filter yellow enhanced.
#else		//__USE_MANU_BW4K40_1000__ ... etc		
//#define		__CC_USE_FILTER_BLUE_STYLE1__			//blue filter default
#define		__CC_USE_FILTER_BLUE_STYLE3__				//minor customize exclude cb-cr domain grid : __CC_USE_FILTER_BLUE_STYLE1__	
//#define		__CC_USE_FILTER_BLUE_STYLE4__			//for the en332t user
#endif
// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
static CCF_APP_PARAM stCCF;
// -----------------------------------------------------------------------------
// Device drive functions
// -----------------------------------------------------------------------------
#if __CC_BIT_EXTENSION__== 2
ROMDATA WORD def_color_correction_table[14][9] =
{
	//[0] 60%
	{614, 205, 205,  205, 614, 205,  205, 205, 614}, 		//R(WB) B(WB) valid or invalid	
	//[1] 70%
	{716, 154, 154,  154, 716, 154,  154, 154, 716}, 	
	//[2] 80%
	{820, 102, 102,  102, 820, 102,  102, 102, 820}, 		
	//[3] 90%
	{920, 52, 52,  52, 920, 52,  52, 52, 920}, 	
	//[4] 100%  bypass 3x3 color correction
	{1024, 0, 0,  0, 1024, 0,  0, 0, 1024}, 	
	#if defined (__CC_USE_FILTER_BLUE_STYLE1__)				//blue filter default
	//[5] 				
	{1074, 16294, 40, 16266, 1158, 16367, 60, 16118, 1229 },			
	//[6] 					
	{1104, 16244, 60, 16158, 1294, 16340, 120, 15853, 1435 },
	//[7] (default)					
	{1124, 16204, 80, 15982, 1482, 16328, 220, 15411, 1777 },
	//[8]					
	{1208, 16054, 146, 15968, 1518, 16306, 240, 15322, 1846 },
	//[9]					
	{1224, 16024, 160, 15972, 1563, 16297, 270, 15262, 1876 },
	//[10]					
	{1284, 15944, 180, 15942, 1613, 16277, 300, 15202, 1906 },
	#elif defined (__CC_USE_FILTER_BLUE_STYLE2__)				//blue filter yellow enhanced.
	//[5]
	{1094, 16334, 16364, 16308, 1124, 16360, 64, 16180, 1164 },
	//[6] 
	{1164, 16284, 16344, 16214, 1244, 16334, 126, 15978, 1304 },
	//[7] (default)	
	{1304, 16184, 16304, 16044, 1464, 16284, 250, 15602, 1556 },
	//[8] 
	{1364, 16134, 16294, 15984, 1544, 16264, 300, 15452, 1656 },
	//[9]	
	{1424, 16084, 16284, 15944, 1604, 16244, 350, 15302, 1756 },
	//[10]
	{1484, 16044, 16264, 15904, 1664, 16224, 400, 15152, 1856 },	
	#elif defined (__CC_USE_FILTER_BLUE_STYLE3__)				//minor customize : __CC_USE_FILTER_BLUE_STYLE1__
	//[5] 				
	{1074, 16294, 40, 16266, 1158, 16367, 60, 16118, 1230 },			
	//[6] 					
	{1104, 16244, 60, 16198, 1294, 16300, 120, 15854, 1434 },
	//[7] (default)					
	{1124, 16204, 80, 16052, 1482, 16258, 200, 15482, 1726 },
	//[8]					
	{1208, 16054, 146, 16038, 1518, 16236, 240, 15362, 1806 },
	//[9]					
	{1224, 16024, 160, 16022, 1563, 16207, 270, 15302, 1836 },
	//[10]					
	{1284, 15944, 180, 15992, 1613, 16187, 300, 15242, 1866 },	
	#elif defined (__CC_USE_FILTER_BLUE_STYLE4__)				//minor customize : __CC_USE_FILTER_BLUE_STYLE1__
	//[5]
	{1165, 16277, 16, 16254, 1244, 16294,  16333, 16264, 1195}, 					
	//[6] 
	{1306, 16070, 32, 16124, 1464, 16204,  16282, 16144, 1366}, 			 
	//[7] (default)	
	{1447, 15903, 58, 15994, 1684, 16114,  16231, 16024, 1537}, 	
	//[8]
	{1490, 15798, 120, 15945, 1757, 16090,  16220, 15982, 1580}, 		
	//[9]
	{1540, 15726, 142, 15905, 1830, 16057,  16210, 15922, 1640}, 		
	//[10]
	{1588, 15656, 164, 15864, 1904, 16024,  16180, 15904, 1708}, 	
	#elif defined(__CC_USE_FILTER_RED_STYLE1__)				//red filter default
	//[5]
	{1074, 16294, 40, 16276, 1158, 16357, 60, 16118, 1229 },			
	//[6] 					
	{1124, 16204, 80, 16168, 1293, 16330, 120, 15853, 1434 },
	//[7] (default)					
	{1174, 16114, 120, 16060, 1428, 16304, 180, 15588, 1640 },
	//[8]					
	{1190, 16084, 133, 16024, 1472, 16295, 200, 15499, 1708 },	
	//[9]					
	{1207, 16054, 146, 15988, 1517, 16286, 220, 15411, 1776 },
	//[10]					
	{1224, 16024, 160, 15952, 1562, 16277, 240, 15322, 1845 },
	#elif defined(__CC_USE_FILTER_RED_STYLE2__)				//red filter for imx265 __USE_MANU_WW_4KUHD__
	{1264, 16284, 16244, 16168, 1293, 16330, 60, 16018, 1329 },			
	//[6] 					
	{1364, 16234, 16194, 16060, 1428, 16304, 120, 15753, 1534 },
	//[7] (default)					
	{1464, 16184, 16144, 15952, 1562, 16277, 180, 15488, 1740 },
	//[8]					
	{1564, 16134, 16094, 15892, 1662, 16237, 200, 15399, 1808 },	
	//[9]					
	{1664, 16084, 16044, 15832, 1762, 16197, 220, 15311, 1876 },
	//[10]					
	{1764, 16034, 15994, 15772, 1862, 16157, 240, 15222, 1945 },	
	#elif defined(__CC_USE_FILTER_RED_STYLE3__)				// mn34442, special case
	//[5]
	{1074, 16294, 40, 16276, 1158, 16357, 60, 16118, 1229 },			
	//[6] 					
	{1124, 16204, 80, 16108, 1353, 16330, 120, 15853, 1434 },
	//[7] (default)					
	{1194, 16094, 120, 16000, 1488, 16304, 180, 15588, 1640 },
	//[8]					
	{1210, 16064, 133, 15964, 1532, 16295, 200, 15499, 1708 },	
	//[9]					
	{1227, 16034, 146, 15928, 1577, 16286, 220, 15411, 1776 },
	//[10]					
	{1244, 16004, 160, 15892, 1622, 16277, 240, 15322, 1845 },
	#else
	//[5] 				
	{1074, 16294, 40, 16266, 1158, 16367, 60, 16118, 1229 },			
	//[6] 					
	{1104, 16244, 60, 16158, 1294, 16340, 120, 15853, 1435 },
	//[7] (default)					
	{1124, 16204, 80, 15982, 1482, 16328, 220, 15411, 1777 },
	//[8]					
	{1208, 16054, 146, 15968, 1518, 16306, 240, 15322, 1846 },
	//[9]					
	{1224, 16024, 160, 15972, 1563, 16297, 270, 15262, 1876 },
	//[10]					
	{1284, 15944, 180, 15942, 1613, 16277, 300, 15202, 1906 },
	#endif

	//----------------------------------------------------
	//(10 ~ 13) CC for wide dynamic range 
	//----------------------------------------------------

	#if defined(__USE_CMOS_MN34041__) || defined(__USE_CMOS_MN34220__) || defined(__USE_CMOS_MN34420__)
	//[11] WDR
	{1321, 15938, 149, 16291, 1389, 16111, 	195, 15718, 1495},	
	//[12] ToneMap	
	{1321, 15938, 149, 16291, 1389, 16111, 	195, 15718, 1495},	
	//[13] DWDR
	{1024, 0, 0,  0, 1024, 0,  0, 0, 1024}, 		
	#else
	//[11] WDR
	{1355, 16125, 16313, 15929, 1489, 16375, 	16375, 16145, 1967},
	//[12] ToneMap	
	{1321, 15938, 149, 16291, 1389, 16111, 195, 15718, 1495},
	//[13] DWDR
	{1024, 0, 0,  0, 1024, 0,  0, 0, 1024}, 	
	#endif

	
#else
ROMDATA WORD def_color_correction_table[14][9] =
{
	//[0] 60%
	{614, 205, 205,  205, 614, 205,  205, 205, 614}, 		//R(WB) B(WB) valid or invalid	
	//[1] 70%
	{716, 154, 154,  154, 716, 154,  154, 154, 716}, 	
	//[2] 80%
	{820, 102, 102,  102, 820, 102,  102, 102, 820}, 		
	//[3] 90%
	{920, 52, 52,  52, 920, 52,  52, 52, 920}, 	
	//[4] 100%  bypass 3x3 color correction
	{1024, 0, 0,  0, 1024, 0,  0, 0, 1024}, 	
	#if defined (__CC_USE_FILTER_BLUE_STYLE1__)				//blue filter default
	//[5] 				
	{1074, 8102, 40, 8074, 1158, 8175, 60, 7926, 1229 },			
	//[6] 					
	{1104, 8052, 60, 7966, 1294, 8148, 120, 7661, 1435 },
	//[7] (default)					
	{1124, 8012, 80, 7790, 1482, 8136, 220, 7219, 1777 },
	//[8]					
	{1208, 7862, 146, 7776, 1518, 8114, 240, 7130, 1846 },
	//[9]					
	{1224, 7832, 160, 7780, 1563, 8105, 270, 7070, 1876 },
	//[10]					
	{1284, 7752, 180, 7750, 1613, 8085, 300, 7010, 1906 },
	#elif defined (__CC_USE_FILTER_BLUE_STYLE2__)				//blue filter yellow enhanced.
	//[5]
	{1094, 8142, 8172, 8116, 1124, 8168, 64, 7988, 1164 },
	//[6] 
	{1164, 8092, 8152, 8022, 1244, 8142, 126, 7786, 1304 },
	//[7] (default)	
	{1304, 7992, 8112, 7852, 1464, 8092, 250, 7410, 1556 },
	//[8] 
	{1364, 7942, 8102, 7792, 1544, 8072, 300, 7260, 1656 },
	//[9]	
	{1424, 7892, 8092, 7752, 1604, 8052, 350, 7110, 1756 },
	//[10]
	{1484, 7852, 8072, 7712, 1664, 8032, 400, 6960, 1856 },	
	#elif defined (__CC_USE_FILTER_BLUE_STYLE3__)				//minor customize : __CC_USE_FILTER_BLUE_STYLE1__
	//[5] 				
	{1074, 8102, 40, 8074, 1158, 8175, 60, 7926, 1230 },			
	//[6] 					
	{1104, 8052, 60, 8006, 1294, 8108, 120, 7662, 1434 },
	//[7] (default)					
	{1124, 8012, 80, 7860, 1482, 8066, 200, 7290, 1726 },
	//[8]					
	{1208, 7862, 146, 7846, 1518, 8044, 240, 7170, 1806 },
	//[9]					
	{1224, 7832, 160, 7830, 1563, 8015, 270, 7110, 1836 },
	//[10]					
	{1284, 7752, 180, 7800, 1613, 7995, 300, 7050, 1866 },	
	#elif defined (__CC_USE_FILTER_BLUE_STYLE4__)				//minor customize : __CC_USE_FILTER_BLUE_STYLE1__
	//[5]
	{1165, 8035, 16, 8062, 1244, 8102,  8141, 8072, 1195}, 					
	//[6] 
	{1306, 7878, 32, 7932, 1464, 8012,  8090, 7952, 1366}, 			 
	//[7] (default)	
	{1447, 7711, 58, 7802, 1684, 7922,  8039, 7832, 1537}, 	
	//[8]
	{1490, 7606, 120, 7753, 1757, 7898,  8028, 7790, 1580}, 		
	//[9]
	{1540, 7534, 142, 7713, 1830, 7865,  8018, 7730, 1640}, 		
	//[10]
	{1588, 7464, 164, 7672, 1904, 7832,  7988, 7712, 1708}, 	
	#elif defined(__CC_USE_FILTER_RED_STYLE1__)				//red filter default
	{1074, 8102, 40, 8084, 1158, 8165, 60, 7926, 1229 },			
	//[6] 					
	{1124, 8012, 80, 7976, 1293, 8138, 120, 7661, 1434 },
	//[7] (default)					
	{1174, 7922, 120, 7868, 1428, 8112, 180, 7396, 1640 },
	//[8]					
	{1190, 7892, 133, 7832, 1472, 8103, 200, 7307, 1708 },	
	//[9]					
	{1207, 7862, 146, 7796, 1517, 8094, 220, 7219, 1776 },
	//[10]					
	{1224, 7832, 160, 7760, 1562, 8085, 240, 7130, 1845 },
	#elif defined(__CC_USE_FILTER_RED_STYLE2__)				//red filter for imx265 __USE_MANU_WW_4KUHD__
	{1264, 8092, 8052, 7976, 1293, 8138, 60, 7826, 1329 },			
	//[6] 					
	{1364, 8042, 8002, 7868, 1428, 8112, 120, 7561, 1534 },
	//[7] (default)					
	{1464, 7992, 7952, 7760, 1562, 8085, 180, 7296, 1740 },
	//[8]					
	{1564, 7942, 7902, 7700, 1662, 8045, 200, 7207, 1808 },	
	//[9]					
	{1664, 7892, 7852, 7640, 1762, 8005, 220, 7119, 1876 },
	//[10]					
	{1764, 7842, 7802, 7580, 1862, 7965, 240, 7030, 1945 },	
	#elif defined(__CC_USE_FILTER_RED_STYLE3__)				// mn34442, special case
	{1074, 8102, 40, 8084, 1158, 8165, 60, 7926, 1229 },			
	//[6] 					
	{1124, 8012, 80, 7916, 1353, 8138, 120, 7661, 1434 },
	//[7] (default)					
	{1194, 7902, 120, 7808, 1488, 8112, 180, 7396, 1640 },
	//[8]					
	{1210, 7872, 133, 7772, 1532, 8103, 200, 7307, 1708 },	
	//[9]					
	{1227, 7842, 146, 7736, 1577, 8094, 220, 7219, 1776 },
	//[10]					
	{1244, 7812, 160, 7700, 1622, 8085, 240, 7130, 1845 },
	#else
	//[5] 				
	{1074, 8102, 40, 8074, 1158, 8175, 60, 7926, 1229 },			
	//[6] 					
	{1104, 8052, 60, 7966, 1294, 8148, 120, 7661, 1435 },
	//[7] (default)					
	{1124, 8012, 80, 7790, 1482, 8136, 220, 7219, 1777 },
	//[8]					
	{1208, 7862, 146, 7776, 1518, 8114, 240, 7130, 1846 },
	//[9]					
	{1224, 7832, 160, 7780, 1563, 8105, 270, 7070, 1876 },
	//[10]					
	{1284, 7752, 180, 7750, 1613, 8085, 300, 7010, 1906 },
	#endif

	//----------------------------------------------------
	//(10 ~ 13) CC for wide dynamic range 
	//----------------------------------------------------

	#if defined(__USE_CMOS_MN34041__) || defined(__USE_CMOS_MN34220__) || defined(__USE_CMOS_MN34420__)
	//[11] WDR
	{1321, 7746, 149, 8099, 1389, 7919, 	195, 7526, 1495},	
	//[12] ToneMap	
	{1321, 7746, 149, 8099, 1389, 7919, 	195, 7526, 1495},	
	//[13] DWDR
	{1024, 0, 0,  0, 1024, 0,  0, 0, 1024}, 		
	#else
	//[11] WDR
	{1355, 7933, 8121, 7737, 1489, 8183, 	7953, 7488, 1967},
	//[12] ToneMap	
	{1321, 7746, 149, 8099, 1389, 7919, 	195, 7526, 1495},
	//[13] DWDR
	{1024, 0, 0,  0, 1024, 0,  0, 0, 1024}, 	
	#endif
#endif
};

static void SetCCF_Redline_Cal(BYTE nID)
{
	WORD Rgain_cal, Rbal, aRbal ;
	
	if(nID==1){
		//Red line type 1. default R1_balance_menu = 50
		Rgain_cal = CCF_Gainstep * stCCF.R1_gain_menu;
		Rbal = Rgain_cal * stCCF.R1_balance_menu /100;
		aRbal = Rgain_cal - Rbal;

		if(stCCF.R1_gain_menu>0){					
			stCCF.rr_gain = 1024 + Rgain_cal;
			stCCF.gr_gain = 16384 - Rbal;
			stCCF.br_gain = 16384 - aRbal;
		}
		else	{
			stCCF.rr_gain = 1024;
			stCCF.gr_gain = 0;
			stCCF.br_gain = 0;
		}
	}
	else{
		//Red line type 2. default R2_balance_menu = 0
		Rgain_cal = CCF_Gainstep * stCCF.R2_gain_menu;
		Rbal = (CCF_Gainstep/2) * stCCF.R2_balance_menu;
		aRbal = Rgain_cal + Rbal;

		if(stCCF.R2_balance_menu>0){					
			stCCF.rr_gain = 1024 + Rgain_cal;
			stCCF.gr_gain = 16384 - aRbal;
			stCCF.br_gain = Rbal;
		}
		else	{
			stCCF.rr_gain = 1024 + Rgain_cal;
			stCCF.gr_gain = 16384 - Rgain_cal;
			stCCF.br_gain = 0;
		}		

	}

}
static void SetCCF_Greenline_Cal(void)
{
	WORD Ggain_cal, Gbal, aGbal ;
	
	//Green line type 1
	Ggain_cal = CCF_Gainstep * stCCF.G1_gain_menu;
	Gbal = Ggain_cal * stCCF.G1_balance_menu /100;
	aGbal = Ggain_cal - Gbal;

	if(stCCF.G1_gain_menu>0){					
		stCCF.rg_gain = 16384 - aGbal;
		stCCF.gg_gain = 1024 + Ggain_cal;
		stCCF.bg_gain = 16384 - Gbal;
	}
	else	{
		stCCF.rg_gain = 0;
		stCCF.gg_gain = 1024;
		stCCF.bg_gain = 0;
	}
		
}

static void SetCCF_Blueline_Cal(BYTE nID)
{
	WORD Bgain_cal, Bbal, aBbal ;

	if(nID==1){	
		//Blue line type 1.	default B1_balance_menu = 50	
		Bgain_cal = CCF_Gainstep * stCCF.B1_gain_menu;
		Bbal = Bgain_cal * stCCF.B1_balance_menu / 100;
		aBbal = Bgain_cal - Bbal;

		if(stCCF.B1_gain_menu>0){					
			stCCF.rb_gain = 16384 - aBbal;
			stCCF.gb_gain = 16384 - Bbal;
			stCCF.bb_gain = 1024 + Bgain_cal;
		}
		else	{
			stCCF.rb_gain = 0;
			stCCF.gb_gain = 0;
			stCCF.bb_gain = 1024;
		}		
	}
	else{
		//Blue line type 2.	default B2_balance_menu = 0
		Bgain_cal = CCF_Gainstep * stCCF.B2_gain_menu;
		Bbal = (CCF_Gainstep/2) * stCCF.B2_balance_menu;
		aBbal = Bgain_cal + Bbal;

		if(stCCF.B2_balance_menu>0){					
			stCCF.rb_gain = Bbal;
			stCCF.gb_gain = 16384 - aBbal;
			stCCF.bb_gain = 1024 + Bgain_cal;
		}
		else	{
			stCCF.rb_gain = 0;
			stCCF.gb_gain = 16384 -Bgain_cal ;
			stCCF.bb_gain = 1024 + Bgain_cal;
		}		
	}
}

void SetCCF_UPDATE( PCCF_APP_PARAM pCTRL )
{
	if(pCTRL->Type_applyed == 3)	//type 3 : R2G1B2
	{
		SetCCF_Redline_Cal(2);
		SetCCF_Greenline_Cal();
		SetCCF_Blueline_Cal(2);
	}
	else if(pCTRL->Type_applyed  == 2)	//type 2 : R2G1B1
	{
		SetCCF_Redline_Cal(2);
		SetCCF_Greenline_Cal();
		SetCCF_Blueline_Cal(1);
	}
	else if(pCTRL->Type_applyed  == 1)	 //type 1 : R1G1B2
	{
		SetCCF_Redline_Cal(1);
		SetCCF_Greenline_Cal();
		SetCCF_Blueline_Cal(2);
	}
	else  //type 0 : R1G1B1(+--)
	{
		SetCCF_Redline_Cal(1);
		SetCCF_Greenline_Cal();
		SetCCF_Blueline_Cal(1);
	}


	MISP_RegWrite(MISP_LOCAL_ID, 0x484, pCTRL->rr_gain);
	MISP_RegWrite(MISP_LOCAL_ID, 0x485, pCTRL->gr_gain);
	MISP_RegWrite(MISP_LOCAL_ID, 0x486, pCTRL->br_gain);
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x487, pCTRL->rg_gain);
	MISP_RegWrite(MISP_LOCAL_ID, 0x488, pCTRL->gg_gain);
	MISP_RegWrite(MISP_LOCAL_ID, 0x489, pCTRL->bg_gain);
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x48A, pCTRL->rb_gain);
	MISP_RegWrite(MISP_LOCAL_ID, 0x48B, pCTRL->gb_gain);
	MISP_RegWrite(MISP_LOCAL_ID, 0x48C, pCTRL->bb_gain);

}

void SetCCFGain(WORD level, WORD line)
{
	switch(line){
		case Red_Line_Type1  : stCCF.R1_gain_menu=level;		break;
		case Red_Line_Type2  : stCCF.R2_gain_menu=level; 		break;
		case Green_Line_Type1  : stCCF.G1_gain_menu=level;		break;
		case Blue_Line_Type1  :  stCCF.B1_gain_menu=level;		break;
		case Blue_Line_Type2  :  stCCF.B2_gain_menu=level;		break;
	}
	SetCCF_UPDATE(&stCCF);
}

void SetCCFBalance(WORD level, WORD line)
{
	switch(line){
		case Red_Line_Type1  : stCCF.R1_balance_menu=level;		break;
		case Red_Line_Type2  : stCCF.R2_balance_menu=level; 		break;
		case Green_Line_Type1  : stCCF.G1_balance_menu=level;		break;
		case Blue_Line_Type1  :  stCCF.B1_balance_menu=level;		break;
		case Blue_Line_Type2  :  stCCF.B2_balance_menu=level;		break;
	}
	SetCCF_UPDATE(&stCCF);	
}
WORD GetCCFGain(WORD line)
{
	switch(line){
		case Red_Line_Type1  : return  stCCF.R1_gain_menu;		break;
		case Red_Line_Type2  : return  stCCF.R2_gain_menu;		break;
		case Green_Line_Type1  : return  stCCF.G1_gain_menu;		break;
		case Blue_Line_Type1  : return  stCCF.B1_gain_menu;		break;
		case Blue_Line_Type2  : return  stCCF.B2_gain_menu;		break;
		default : return stCCF.R1_gain_menu; break;		
	}
}
WORD GetCCFBalance(WORD line)
{
	switch(line){
		case Red_Line_Type1  : return  stCCF.R1_balance_menu;		break;
		case Red_Line_Type2  : return  stCCF.R2_balance_menu;		break;
		case Green_Line_Type1  : return  stCCF.G1_balance_menu;		break;
		case Blue_Line_Type1  : return  stCCF.B1_balance_menu;		break;
		case Blue_Line_Type2  : return  stCCF.B2_balance_menu;		break;
		default : return stCCF.R1_balance_menu; break;
	}
}

void SetCCFType(BYTE type)
{
	//give current cal ccf data
	stCCF.Type_applyed=type;
	//printf("type : %d	\r\n", stCCF.Type_applyed);
	//call color correction
	SetCCF_UPDATE(&stCCF);
}

BYTE GetCCFType(void)
{
	return stCCF.Type_applyed;
}

void CCFResultPrint(void)
{
	WORD Rgain_cal = (stCCF.rr_gain - 1024);	
	WORD Ggain_cal = (stCCF.gg_gain - 1024);
	WORD Bgain_cal = (stCCF.bb_gain - 1024);

	WORD bRgain_cal = (16384 - stCCF.br_gain);
	WORD gRgain_cal = (16384 - stCCF.gr_gain);	
	WORD bGgain_cal = (16384 - stCCF.bg_gain);
	WORD rGgain_cal = (16384 - stCCF.rg_gain);		
	WORD rBgain_cal = (16384 - stCCF.rb_gain);
	WORD gBgain_cal = (16384 - stCCF.gb_gain);			

	WORD rr_buf[5], gr_buf[5], br_buf[5];
	WORD rg_buf[5], gg_buf[5], bg_buf[5];
	WORD rb_buf[5], gb_buf[5], bb_buf[5];

	if(stCCF.Type_applyed>1){		//red type 2
		 rr_buf[0] = MIN(2048, 1024 + Rgain_cal /3 );		rr_buf[1] = MIN(2048, 1024 + 2*Rgain_cal /3 ); 	
		 rr_buf[2] = MIN(2048, 1024 + 5*Rgain_cal /4 ); 	rr_buf[3] = MIN(2048, 1024 + 6*Rgain_cal /4 ); 	rr_buf[4] = MIN(2048, 1024 + 7*Rgain_cal /4 ); 		 

		 br_buf[0] = MIN(1024, stCCF.br_gain /3);			br_buf[1] = MIN(1024, 2*stCCF.br_gain /3);
		 br_buf[2] = MIN(1024, 5*stCCF.br_gain /4);		br_buf[3] = MIN(1024, 6*stCCF.br_gain /4);		br_buf[4] = MIN(1024, 7*stCCF.br_gain /4);

		 gr_buf[0] = 16384 + 1024 -rr_buf[0] - br_buf[0]; 	gr_buf[1] = 16384 + 1024 -rr_buf[1] - br_buf[1]; 	
		 gr_buf[2] = 16384 + 1024 -rr_buf[2] - br_buf[2]; 	gr_buf[3] = 16384+ 1024  -rr_buf[3] - br_buf[3]; 	gr_buf[4] = 16384 + 1024 -rr_buf[4] - br_buf[4]; 	 
		 
	}
	else{	//red type 1
		 rr_buf[0] = MIN(2048, 1024 + Rgain_cal /3 );		rr_buf[1] = MIN(2048, 1024 + 2*Rgain_cal /3 ); 	
		 rr_buf[2] = MIN(2048, 1024 + 5*Rgain_cal /4 ); 	rr_buf[3] = MIN(2048, 1024 + 6*Rgain_cal /4 ); 	rr_buf[4] = MIN(2048, 1024 + 7*Rgain_cal /4 ); 	

		if(stCCF.br_gain>0){
		 br_buf[0] = (16384 - bRgain_cal / 3);	 		br_buf[1] = (16384 - 2 * bRgain_cal / 3);
		 br_buf[2] = (16384 - 5 * bRgain_cal / 4);	 		br_buf[3] = (16384 - 6 * bRgain_cal / 4);	 		br_buf[4] = (16384 - 7 * bRgain_cal / 4);
		}
		else{
		 br_buf[0] = 0;	 							br_buf[1] = 0;
		 br_buf[2] = 0;	 							br_buf[3] = 0;	 								br_buf[4] = 0;
		}
		if(stCCF.gr_gain>0){
		 gr_buf[0] = (16384 - gRgain_cal / 3);	 		gr_buf[1] = (16384 - 2 * gRgain_cal / 3);
		 gr_buf[2] = (16384 - 5 * gRgain_cal / 4);	 		gr_buf[3] = (16384 - 6 * gRgain_cal / 4);	 		gr_buf[4] = (16384 - 7 * gRgain_cal / 4);
		}
		else{
		 gr_buf[0] = 0;	 							gr_buf[1] = 0;
		 gr_buf[2] = 0;	 							gr_buf[3] = 0;	 								gr_buf[4] = 0;
		}

	}

	//green type 1
		 gg_buf[0] = MIN(2048, 1024 + Ggain_cal /3 );		gg_buf[1] = MIN(2048, 1024 + 2*Ggain_cal /3 ); 	
		 gg_buf[2] = MIN(2048, 1024 + 5*Ggain_cal /4 ); 	gg_buf[3] = MIN(2048, 1024 + 6*Ggain_cal /4 ); 	gg_buf[4] = MIN(2048, 1024 + 7*Ggain_cal /4 ); 	

		if(stCCF.bg_gain>0){
		 bg_buf[0] = (16384 - bGgain_cal / 3);	 		bg_buf[1] = (16384 - 2 * bGgain_cal / 3);
		 bg_buf[2] = (16384 - 5 * bGgain_cal / 4);	 		bg_buf[3] = (16384 - 6 * bGgain_cal / 4);	 		bg_buf[4] = (16384 - 7 * bGgain_cal / 4);
		}
		else{
		 bg_buf[0] = 0;	 							bg_buf[1] = 0;
		 bg_buf[2] = 0;	 							bg_buf[3] = 0;	 							bg_buf[4] = 0;
		}
		if(stCCF.rg_gain>0){
		 rg_buf[0] = (16384 - rGgain_cal / 3);	 			rg_buf[1] = (16384 - 2 * rGgain_cal / 3);
		 rg_buf[2] = (16384 - 5 * rGgain_cal / 4);	 		rg_buf[3] = (16384 - 6 * rGgain_cal / 4);	 		rg_buf[4] = (16384 - 7 * rGgain_cal / 4);
		}
		else{
		 rg_buf[0] = 0;	 							rg_buf[1] = 0;
		 rg_buf[2] = 0;	 							rg_buf[3] = 0;	 								rg_buf[4] = 0;
		}


	if((stCCF.Type_applyed==1) ||(stCCF.Type_applyed==3))  {		//blue type 2
		 bb_buf[0] = MIN(2048, 1024 + Bgain_cal /3 );		bb_buf[1] = MIN(2048, 1024 + 2*Bgain_cal /3 ); 	
		 bb_buf[2] = MIN(2048, 1024 + 6*Bgain_cal /5 ); 	bb_buf[3] = MIN(2048, 1024 + 7*Bgain_cal /5 ); 	bb_buf[4] = MIN(2048, 1024 + 8*Bgain_cal /5 ); 		 

		 rb_buf[0] = MIN(1024, stCCF.rb_gain /3);			rb_buf[1] = MIN(1024, 2*stCCF.rb_gain /3);
		 rb_buf[2] = MIN(1024, 6*stCCF.rb_gain /5);		rb_buf[3] = MIN(1024, 7*stCCF.rb_gain /5);		rb_buf[4] = MIN(1024, 8*stCCF.rb_gain /5);

		 gb_buf[0] = 16384 + 1024 -bb_buf[0] - rb_buf[0]; 		gb_buf[1] = 16384 + 1024 -bb_buf[1] - rb_buf[1]; 	
		 gb_buf[2] = 16384 + 1024 -bb_buf[2] - rb_buf[2]; 		gb_buf[3] = 16384 + 1024 -bb_buf[3] - rb_buf[3]; 		gb_buf[4] = 16384 + 1024 -bb_buf[4] - rb_buf[4]; 	 
		 
	}
	else{	//blue type 1
		 bb_buf[0] = MIN(2048, 1024 + Bgain_cal /3 );		bb_buf[1] = MIN(2048, 1024 + 2*Bgain_cal /3 ); 	
		 bb_buf[2] = MIN(2048, 1024 + 6*Bgain_cal /5 ); 	bb_buf[3] = MIN(2048, 1024 + 7*Bgain_cal /5 ); 	bb_buf[4] = MIN(2048, 1024 + 8*Bgain_cal /5 ); 	

		if(stCCF.rb_gain>0){
		 rb_buf[0] = (16384 - rBgain_cal / 3);	 			rb_buf[1] = (16384 - 2 * rBgain_cal / 3);
		 rb_buf[2] = (16384 - 6 * rBgain_cal / 5);	 		rb_buf[3] = (16384 - 7 * rBgain_cal / 5);	 		rb_buf[4] = (16384 - 8 * rBgain_cal / 5);
		}
		else{
		 rb_buf[0] = 0;	 							rb_buf[1] = 0;
		 rb_buf[2] = 0;	 							rb_buf[3] = 0;	 								rb_buf[4] = 0;
		}
		if(stCCF.gb_gain>0){
		 gb_buf[0] = (16384 - gBgain_cal / 3);	 		gb_buf[1] = (16384 - 2 * gBgain_cal / 3);
		 gb_buf[2] = (16384 - 6 * gBgain_cal / 5);	 		gb_buf[3] = (16384 - 7 * gBgain_cal / 5);	 		gb_buf[4] = (16384 - 8 * gBgain_cal / 5);
		}
		else{
		 gb_buf[0] = 0;	 							gb_buf[1] = 0;
		 gb_buf[2] = 0;	 							gb_buf[3] = 0;	 							gb_buf[4] = 0;
		}

	}



	//copy to the "def_color_correction_table"
	printf("	//[5]\r\n");
	printf("	{%d, %d, %d, %d, %d, %d,  %d, %d, %d},\r\n", rr_buf[0],  gr_buf[0], br_buf[0], rg_buf[0],  gg_buf[0], bg_buf[0], rb_buf[0],  gb_buf[0], bb_buf[0]);
	printf("	//[6]\r\n");
	printf("	{%d, %d, %d, %d, %d, %d,  %d, %d, %d},\r\n", rr_buf[1],  gr_buf[1], br_buf[1], rg_buf[1],  gg_buf[1], bg_buf[1], rb_buf[1],  gb_buf[1], bb_buf[1]);
	printf("	//[7]\r\n");
	printf("	{%d, %d, %d, %d, %d, %d,  %d, %d, %d},\r\n", stCCF.rr_gain,  stCCF.gr_gain, stCCF.br_gain, stCCF.rg_gain, stCCF.gg_gain, stCCF.bg_gain,stCCF.rb_gain,stCCF.gb_gain,stCCF.bb_gain);
	printf("	//[8]\r\n");
	printf("	{%d, %d, %d, %d, %d, %d,  %d, %d, %d},\r\n", rr_buf[2],  gr_buf[2], br_buf[2], rg_buf[2],  gg_buf[2], bg_buf[2], rb_buf[2],  gb_buf[2], bb_buf[2]);
	printf("	//[9]\r\n");
	printf("	{%d, %d, %d, %d, %d, %d,  %d, %d, %d},\r\n", rr_buf[3],  gr_buf[3], br_buf[3], rg_buf[3],  gg_buf[3], bg_buf[3], rb_buf[3],  gb_buf[3], bb_buf[3]);
	printf("	//[10]\r\n");	
	printf("	{%d, %d, %d, %d, %d, %d,  %d, %d, %d},\r\n", rr_buf[4],  gr_buf[4], br_buf[4], rg_buf[4],  gg_buf[4], bg_buf[4], rb_buf[4],  gb_buf[4], bb_buf[4]);	
}

static void Init_CCF_PARAM( PCCF_APP_PARAM pCTRL )
{
	MISP_RegRead(MISP_LOCAL_ID, 0x484, &pCTRL->rr_gain);
	MISP_RegRead(MISP_LOCAL_ID, 0x485, &pCTRL->gr_gain);
	MISP_RegRead(MISP_LOCAL_ID, 0x486, &pCTRL->br_gain);
	
	MISP_RegRead(MISP_LOCAL_ID, 0x487, &pCTRL->rg_gain);
	MISP_RegRead(MISP_LOCAL_ID, 0x488, &pCTRL->gg_gain);
	MISP_RegRead(MISP_LOCAL_ID, 0x489, &pCTRL->bg_gain);
	
	MISP_RegRead(MISP_LOCAL_ID, 0x48A, &pCTRL->rb_gain);
	MISP_RegRead(MISP_LOCAL_ID, 0x48B, &pCTRL->gb_gain);
	MISP_RegRead(MISP_LOCAL_ID, 0x48C, &pCTRL->bb_gain);

	//menu를 읽어서 444보다 작은것이 하나라도... exsist default...
	//sum = 1.0 인근이 아니면 pass 

	if((pCTRL->br_gain<1024) && (pCTRL->rb_gain<1024)) //CCF type3 :  R2G1B2
	{
		pCTRL->R1_gain_menu=0;	
		pCTRL->R1_balance_menu=50;
		pCTRL->B1_gain_menu=0;
		pCTRL->B1_balance_menu=50;

		// case R type2
		pCTRL->R2_gain_menu=(pCTRL->rr_gain-1024) /CCF_Gainstep;
		pCTRL->R2_balance_menu=pCTRL->br_gain * 2 / CCF_Gainstep;		
		// case G type1
		pCTRL->G1_gain_menu=(pCTRL->gg_gain-1024) /CCF_Gainstep;	
		pCTRL->G1_balance_menu=((16384 - pCTRL->bg_gain) * 100) / (pCTRL->gg_gain-1024);
		//case B type2
		pCTRL->B2_gain_menu=(pCTRL->bb_gain-1024)/CCF_Gainstep;	
		pCTRL->B2_balance_menu=pCTRL->rb_gain  * 2 /CCF_Gainstep;

		pCTRL->Type_applyed 	= 3;
	
	}
	else if((pCTRL->br_gain<1024)  && (pCTRL->rb_gain>8192)) //CCF type2 : R2G1B1
	{
		pCTRL->R1_gain_menu=0;	
		pCTRL->R1_balance_menu=50;
		pCTRL->B2_gain_menu=0;
		pCTRL->B2_balance_menu=0;

		// case R type2
		pCTRL->R2_gain_menu=(pCTRL->rr_gain-1024) /CCF_Gainstep;
		pCTRL->R2_balance_menu=pCTRL->br_gain * 2 / CCF_Gainstep;
		// case G type1
		pCTRL->G1_gain_menu=(pCTRL->gg_gain-1024) /CCF_Gainstep;	
		pCTRL->G1_balance_menu=((16384 - pCTRL->bg_gain) * 100) / (pCTRL->gg_gain-1024);
		//case B type1
		pCTRL->B1_gain_menu=(pCTRL->bb_gain-1024)/CCF_Gainstep;	
		pCTRL->B1_balance_menu=(16384 -pCTRL->gb_gain) * 100 / (pCTRL->bb_gain - 1024);

		pCTRL->Type_applyed 	= 2;
	}	
	else if((pCTRL->br_gain>8192)  && (pCTRL->rb_gain<1024)) //CCF type1 : R1G1B2	
	{
		pCTRL->R2_gain_menu=0;	
		pCTRL->R2_balance_menu=0;
		pCTRL->B1_gain_menu=0;
		pCTRL->B1_balance_menu=50;

		// case R type1
		pCTRL->R1_gain_menu=(pCTRL->rr_gain-1024) /CCF_Gainstep;
		pCTRL->R1_balance_menu=(16384 - pCTRL->gr_gain) * 100 / (pCTRL->rr_gain - 1024);		
		// case G type1
		pCTRL->G1_gain_menu=(pCTRL->gg_gain-1024) /CCF_Gainstep;	
		pCTRL->G1_balance_menu=((16384 - pCTRL->bg_gain) * 100) / (pCTRL->gg_gain-1024);
		//case B type2
		pCTRL->B2_gain_menu=(pCTRL->bb_gain-1024)/CCF_Gainstep;	
		pCTRL->B2_balance_menu=pCTRL->rb_gain  * 2 /CCF_Gainstep;		

		pCTRL->Type_applyed 	= 1;	
	}	
	else if((pCTRL->br_gain>8192) && (pCTRL->rb_gain>8192)) //CCF type0 : R1G1B1
	{
		pCTRL->R2_gain_menu=0;	
		pCTRL->R2_balance_menu=0;
		pCTRL->B2_gain_menu=0;
		pCTRL->B2_balance_menu=0;
		
		// case R type1
		pCTRL->R1_gain_menu=(pCTRL->rr_gain-1024) /CCF_Gainstep;
		pCTRL->R1_balance_menu=(16384 - pCTRL->gr_gain) * 100 / (pCTRL->rr_gain - 1024);		
		// case G type1
		pCTRL->G1_gain_menu=(pCTRL->gg_gain-1024) /CCF_Gainstep;	
		pCTRL->G1_balance_menu=((16384 - pCTRL->bg_gain) * 100) / (pCTRL->gg_gain-1024);
		//case B type1
		pCTRL->B1_gain_menu=(pCTRL->bb_gain-1024)/CCF_Gainstep;	
		pCTRL->B1_balance_menu=(16384 -pCTRL->gb_gain) * 100 / (pCTRL->bb_gain - 1024);

		pCTRL->Type_applyed 	= 0;				
	}
	else{		//all defalut
		pCTRL->R1_gain_menu=0;	
		pCTRL->R1_balance_menu=50;
		pCTRL->G1_gain_menu=0;
		pCTRL->G1_balance_menu=50;		
		pCTRL->B1_gain_menu=0;
		pCTRL->B1_balance_menu=50;

		pCTRL->R2_gain_menu=0;	
		pCTRL->R2_balance_menu=0;
		pCTRL->B2_gain_menu=0;
		pCTRL->B2_balance_menu=0;

		pCTRL->Type_applyed 	= 0;	
	}

	
}

void Init_CCF(void)
{
	Init_CCF_PARAM(&stCCF);
}
	
/*  FILE_END_HERE */

