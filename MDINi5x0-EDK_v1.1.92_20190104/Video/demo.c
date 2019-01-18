// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"
#include	"osd.h"

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
extern const LONG AECTRL_F_VALUE[8];

typedef	enum {
#if defined(__USE_SENS_240Hz_MENU__)
	MISP_FPS_25Hz=0,
	MISP_FPS_30Hz,
	MISP_FPS_50Hz,
	MISP_FPS_60Hz,
	MISP_FPS_100Hz,
	MISP_FPS_120Hz,
	MISP_FPS_200Hz,
	MISP_FPS_240Hz,
	MISP_FPS_20Hz,
	MISP_FPS_24Hz,	
	MISP_FPS_NUM,
#elif defined(__USE_SENS_120Hz_MENU__)
	MISP_FPS_25Hz=0,
	MISP_FPS_30Hz,
	MISP_FPS_50Hz,
	MISP_FPS_60Hz,
	MISP_FPS_100Hz,
	MISP_FPS_120Hz,
	MISP_FPS_20Hz,
	MISP_FPS_24Hz,
	MISP_FPS_200Hz,
	MISP_FPS_240Hz,	
	MISP_FPS_NUM,
#elif defined(__USE_SENS_20Hz_MENU__)
	MISP_FPS_20Hz=0,
	MISP_FPS_24Hz,
	MISP_FPS_25Hz,
	MISP_FPS_30Hz,
	MISP_FPS_50Hz,
	MISP_FPS_60Hz,
	MISP_FPS_100Hz,
	MISP_FPS_120Hz,
	MISP_FPS_200Hz,
	MISP_FPS_240Hz,	
	MISP_FPS_NUM,
#else
	MISP_FPS_25Hz=0,
	MISP_FPS_30Hz,
	MISP_FPS_50Hz,
	MISP_FPS_60Hz,
	MISP_FPS_20Hz,
	MISP_FPS_24Hz,
	MISP_FPS_100Hz,
	MISP_FPS_120Hz,
	MISP_FPS_200Hz,
	MISP_FPS_240Hz,	
	MISP_FPS_NUM,
#endif
} MISP_FPS_t;

typedef	struct	   
{
	WORD	w;			// Horizontal size of video area
	WORD	h;			// Vertical size of video area
	WORD	x;			// Horizontal position of the top-left corner of video area
	WORD	y;			// Vertical position of the top-left corner of video area
	BOOL	EN;			// Enable Area(sub block)
	BOOL	OnOff;		// Display on/off
	BOOL	Init;		// value init(0:not init, 1:init)
}	stPACKED MISP_DEMOWIN_INFO, *PMISP_DEMOWIN_INFO;


#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
typedef	struct	   
{
	WORD	ld_x[4];			// 
	WORD	ld_y[4];			// 
	WORD	ld_s[4];			// 
	WORD	ls_x[2];			// 
	WORD	ls_s;				// 
	WORD	vd_x[4];			// 
	WORD	vd_y[4];			// 
	WORD	vd_s[3];			// 
	
}	CSUP_THRES, *PCSUP_THRES;

typedef	struct
{
	LONG	ctrl;				// flicker control flag
	
	WORD 	raw[6][24];			// flicker raw data(horizontal accumulation data)
	BYTE	min_p[24];		// flicker raw minimum position
	BYTE	max_p[24];	// flicker raw maximum position
	BYTE	raw_limit;			// flicker raw data line limit(25hz:24 line, 30hz:20 line)
	BYTE	fr_limit;			// flicker raw data frame number limit
	BYTE	p_err_limit;			// flicker raw pattern error limit
	BYTE	dev_min_fr;			// flicker raw deviation min frame number
	DWORD	min_mean;			// flicker raw data min mean
	DWORD	max_mean;			// flicker raw data max mean
	WORD	mean_thres1;		// flicker raw data mean threshold 1
	WORD	mean_thres2;		// flicker raw data mean threshold 2
	BOOL	p_flk;
	BYTE	dummy_000;
	WORD	dummy_001;

}	FLK_CTRL_PARAM, *PFLK_CTRL_PARAM;

#define		FLKCTRL_PROCESS_ON			0x10000000		// flicker process enable
#define		FLKCTRL_PROCESS_OFF			0x00000000		// flicker process disable

#define		FLKCTRL_SS_FPS_NONE			0x00000000
#define		FLKCTRL_SS_FPS_25HZ			0x00000001
#define		FLKCTRL_SS_FPS_30HZ			0x00000002
#define		FLKCTRL_SS_FPS_MASK			0x0000000F


#endif
// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

#if defined(__USE_MDIN_i500__) 		// MDIN-i500
static BYTE move_in_LR = 5, move_in_UD = 5;	  
#else 								// MDIN-i51X (i510, i540, i550)
BYTE move_in_LR = 15, move_in_UD = 15;	  
#endif

MISP_DEMOWIN_INFO	stTP_Bayer[50];
static BOOL DIS_OVERSCAN;

#if defined(__USE_CMOS_MN34220__) || defined(__USE_CMOS_MN34420__)
static ROMDATA LONG  AGCGain[] = {
//	auto,	0dB,		3dB,		6dB,		9dB,		12dB,	15dB,	18dB,	21dB,	24dB,	27dB
	256,		256,		384,		512,		700,		896,		1280,	1792,	2432,	3200,	4480,

//	30dB,	33dB,	36dB,	39dB,	42dB,	45dB	48dB	51dB	54dB	57dB	60dB
	5888,	7680,	11520,	14720,	20480,	28800,	38400,	54400,	0x11500,	0x18800,	0x20000,

//	d2X,						d3X,						d4X						d5X
	0x20000+0x20000*1/8,		0x20000+0x20000*2/8,		0x20000+0x20000*3/8,		0x20000+0x20000*4/8,
//	d6X,						d7X,						d8X						d9X
	0x20000+0x20000*5/8,		0x20000+0x20000*6/8,		0x20000+0x20000*7/8,		0x20000+0x20000*8/8,
};

static ROMDATA LONG  AGCMax[] = {
//	OFF,		LOW,		MID,			HIGH
	256,		0x1fff,		0x10000,		0x20000
};

#elif	defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__)
static ROMDATA LONG  AGCGain[] = {
//	auto,	0dB,		2dB,		4dB,		6dB,		8dB,		10dB,	12dB,	14dB,	16dB,	18dB
	256,		256,		0x400,	0xc00,	0x1b00,	0x3400,	0x5700,	0x8500,	0xba00,	0xf300,	0x12d00,

//	20dB,	22dB,	24dB,	26dB,	27dB,
	0x16500,	0x19700,	0x1c500,	0x1ee00,	0x20000,

//	d2X,						d3X,						d4X						d5X
	0x20000+0x20000/8*1,		0x20000+0x20000/8*2,		0x20000+0x20000/8*3,		0x20000+0x20000/8*4,
//	d6X,						d7X,						d8X						d9X
	0x20000+0x20000/8*5,		0x20000+0x20000/8*6,		0x20000+0x20000/8*7,		0x20000+0x20000/8*8,
};

static ROMDATA LONG  AGCMax[] = {
//	OFF,		LOW,		MID,			HIGH
	256,		0x1fff,		0x04fff,		0x20000
};

#elif	defined(__USE_CMOS_IMX226__)
static ROMDATA LONG  AGCGain[] = {
//	auto,	0dB,		2dB,		4dB,		6dB,		8dB,		10dB,	12dB,	14dB,	16dB,	18dB
	256,		256,		0x400,	0xc00,	0x1b00,	0x3400,	0x5700,	0x8500,	0xba00,	0xf300,	0x12d00,

//	20dB,	22dB,	24dB,	26dB,	27dB,
	0x16500,	0x19700,	0x1c500,	0x1ee00,	0x20000,

//	d3X,						d5X,						d7X						d9X
	0x20000+0x20000/32*2,		0x20000+0x20000/32*4,		0x20000+0x20000/32*6,		0x20000+0x20000/32*8,
//	d11X,					d13X,					d15X						d17X
	0x20000+0x20000/32*10,		0x20000+0x20000/32*12,		0x20000+0x20000/32*14,		0x20000+0x20000/32*16,
//	d19X,					d21X,					d23X						d25X
	0x20000+0x20000/32*18,		0x20000+0x20000/32*20,		0x20000+0x20000/32*22,		0x20000+0x20000/32*24,
//	d27X,					d29X,					d31X						d33X
	0x20000+0x20000/32*26,		0x20000+0x20000/32*28,		0x20000+0x20000/32*30,		0x20000+0x20000/32*32,
};

static ROMDATA LONG  AGCMax[] = {
//	OFF,		LOW,		MID,			HIGH
	256,		0x1fff,		0x04fff,		0x20000
};

#elif defined(__USE_CMOS_IMX253__)
static ROMDATA LONG  AGCGain[] = {
//	auto,	0dB,		3dB,		6dB,		9dB,		12dB,	15dB,	18dB,	21dB,	24dB,
	256,		256,		0x200,	0x2a0,	0x400,	0x580,	0x800,	0xb80,	0x1000,	0x1800,

//	27dB	30dB,	33dB,	36dB,	39dB,	42dB,	45dB	48dB
	0x2400,	0x3400,	0x5000,	0x7000,	0xa000,	0xf000,	0x18000,	0x20000,

//	d2X,						d3X,						d4X						d5X
	0x20000+0x20000*1/8,		0x20000+0x20000*2/8,		0x20000+0x20000*3/8,		0x20000+0x20000*4/8,
//	d6X,						d7X,						d8X						d9X
	0x20000+0x20000*5/8,		0x20000+0x20000*6/8,		0x20000+0x20000*7/8,		0x20000+0x20000*8/8,
};

static ROMDATA LONG  AGCMax[] = {
//	OFF,		LOW,		MID,			HIGH
	256,		0x1fff,		0x04fff,		0x20000
};

#elif defined(__USE_CMOS_IMX124__)
static ROMDATA LONG  AGCGain[] = {
//	auto,	0dB,		3dB,		7dB,		10dB,	14dB,	17dB,	21dB,	24dB,	28dB,	31dB
	256,		256,		368,		600,		866,		1412,	2036,	3328,	4816,	7840,	11328,

//	35dB,	38dB,	42dB,	45dB,	51dB,
	18496,	26688,	43648,	62848,	129536,
};
static ROMDATA LONG  AGCMax[] = {
//	OFF,		LOW,		MID,			HIGH
	256,		0x1fff,		0x10000,		0x20000
};

#elif defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__) || defined(__USE_CMOS_IMX334__)
static ROMDATA LONG  AGCGain[] = {
//	auto,	0dB,		5dB,		10dB,	15dB,	20dB,	25dB,	30dB,	35dB,	40dB,	45dB
	256,		256,		0x1a0,	0x280,	0x600,	0x800,	0xb00,	0x1000,	0x1800,	0x2400,	0x3600,

//	50dB,	55dB,	60dB,	65dB,	72dB(69dB),
	0x5000,	0x8000,	0xc600,	0x12e00,	0x20000,

//	d2X,						d3X,						d4X						d5X
	0x20000+0x20000*1/8,		0x20000+0x20000*2/8,		0x20000+0x20000*3/8,		0x20000+0x20000*4/8,
//	d6X,						d7X,						d8X						d9X
	0x20000+0x20000*5/8,		0x20000+0x20000*6/8,		0x20000+0x20000*7/8,		0x20000+0x20000*8/8,
};

static ROMDATA LONG  AGCMax[] = {
//	OFF,		LOW,		MID,			HIGH
	256,		0x1800,		0x5000,		0x20000
};

#elif defined(__USE_CMOS_IMX385__)
static ROMDATA LONG  AGCGain[] = {
//	auto,	0dB,		5dB,		10dB,	15dB,	20dB,	25dB,	30dB,	35dB,	40dB,	45dB
	256,		256,		0x1a0,	0x280,	0x600,	0x800,	0xb00,	0x1000,	0x1800,	0x2400,	0x3600,

//	50dB,	55dB,	60dB,	65dB,	72dB,
	0x5000,	0x8000,	0xc600,	0x12e00,	0x20000,

//	d2X,						d3X,						d4X						d5X
	0x20000+0x20000*1/8,		0x20000+0x20000*2/8,		0x20000+0x20000*3/8,		0x20000+0x20000*4/8,
//	d6X,						d7X,						d8X						d9X
	0x20000+0x20000*5/8,		0x20000+0x20000*6/8,		0x20000+0x20000*7/8,		0x20000+0x20000*8/8,
};

static ROMDATA LONG  AGCMax[] = {
//	OFF,		LOW,		MID,			HIGH
	256,		0x1800,		0x5000,		0x20000
};

#elif defined(__USE_CMOS_IMXx36__)
static ROMDATA LONG  AGCGain[] = {
//	auto,	0dB,		3dB,		6dB,		9dB,		12dB,	15dB,	18dB,	21dB,	24dB,	27dB
	256,		256,		360,		508,		718,		1015,	1432,	2027,	2856,	4040,	5712,

//	30dB,	33dB,	36dB,	39dB,	42dB,
	8048,	11392,	16064,	22720,	32064,
};

static ROMDATA LONG  AGCMax[] = {
//	OFF,		LOW,		MID,			HIGH
	256,		0x1fff,		0x10000,		0x20000
};

#elif defined(__USE_CMOS_IMX174__) || defined(__USE_CMOS_IMX249__) || defined(__USE_CMOS_IMX265__) || defined(__USE_CMOS_IMX252__)	// agc module same between imx265 and imx249
static ROMDATA LONG  AGCGain[] = {
//	auto,	0dB,		2dB,		4dB,		6dB,		8dB,		10dB,	12dB,	14dB,	16dB,	18dB
	256,		256,	330,		428,		556,		720,		932,		1212,	1568,	2032,	2640,

//	20dB,	22dB,	24dB,	d26dB,	d28dB,	d30dB,	d33dB,	d36dB,	d39dB,	d42dB,	d45dB,
	3424,	4448,	5760,	7456,	9696,	12544,	18560,	27392,	40448,	59648,	88064,

//	d48dB,
	130048,
};

static ROMDATA LONG  AGCMax[] = {
//	OFF,		LOW,		MID,			HIGH
	256,		7456,		27392,		0x20000
};

#elif defined(__USE_CMOS_IMX185__) || defined(__USE_CMOS_IMX273__)
// AE_DARK_TEST
//32768 = 48 32703 = 47 29887 = 46 27263 = 45 24191 = 44 22079 = 43 20159 = 42 17855 = 41
//16255 = 40 14847 = 39 13151 = 38 11999 = 37 10943 = 36 9727 = 35 8863 = 34 8079 = 33 
//7151 = 32 6527 = 31 5967 = 30 5279 = 29 4815 = 28 4399 = 27 3887 = 26 3551 = 25 
//3247 = 24 2871 = 23 2623 = 22 2399 = 21 2119 = 20 1931 = 19 1763 = 18 1563 = 17 
//1427 = 16 1303 = 15 1155 = 14 1055 = 13 959 = 12 851 = 11 777 = 10 709 = 9 
//627 = 8 573 = 7 523 = 6 462 = 5 422 = 4 385 = 3 341 = 2 311 = 1 256 = 0
 
//static ROMDATA LONG  AGCGain[] = {
////	auto,	0dB,		3dB,		6dB,		9dB,		12dB,	15dB,	18dB,	21dB,	24db,	27db	30db,	
//	256,		256,		356,		486,		690,		959,		1384,	1956,	2768,	3247,	4399,	5967,
//
////	33dB	36dB,	39dB,	40dB,	41dB,	42dB,	43dB	44dB	45db	46db	47db	48db
//	8079,	10943,	14847,	16255,	17855,	20159,	22079,	24191,	27263,	29887,	32703,	32768,
////	50dB	51dB	52dB	53dB	54dB	55dB	56dB	57dB	58dB	59dB
//	36044,	39320,	42596,	45872,	49148,	52424,	55700,	58976,	62252,	65536
//};

static ROMDATA LONG  AGCGain[] = {
//	auto,	0dB,		3dB,		6dB,		9dB,		12dB,	15dB,	18dB,	21dB,	24dB,
	256,		256,		0x200,	0x2a0,	0x400,	0x580,	0x800,	0xb80,	0x1000,	0x1800,

//	27dB	30dB,	33dB,	36dB,	39dB,	42dB,	45dB	48dB
	0x2400,	0x3400,	0x5000,	0x7000,	0xa000,	0xf000,	0x18000,	0x20000,

//	d2X,						d3X,						d4X						d5X
	0x20000+0x20000*1/8,		0x20000+0x20000*2/8,		0x20000+0x20000*3/8,		0x20000+0x20000*4/8,
//	d6X,						d7X,						d8X						d9X
	0x20000+0x20000*5/8,		0x20000+0x20000*6/8,		0x20000+0x20000*7/8,		0x20000+0x20000*8/8,
};

static ROMDATA LONG  AGCMax[] = {
//	OFF,		LOW,		MID,			HIGH
	256,		0x1fff,		0x10000,		0x20000
};

#elif defined(__USE_CMOS_IMX323__)
static ROMDATA LONG  AGCGain[] = {
//	auto,	0dB,		3dB,		6dB,		9dB,		12dB,	15dB,	18dB,	21dB,	24dB,
	256,		256,		0x200,	0x2a0,	0x400,	0x580,	0x800,	0xb80,	0x1000,	0x1800,

//	27dB		30dB,	33dB,	36dB,	39dB,	42dB,	45dB
	0x2400,	0x3400,	0x5000,	0x7000,	0xa000,	0xf000,	0x20000,

//	d2X,						d3X,						d4X						d5X
	0x20000+0x20000*1/8,		0x20000+0x20000*2/8,		0x20000+0x20000*3/8,		0x20000+0x20000*4/8,
//	d6X,						d7X,						d8X						d9X
	0x20000+0x20000*5/8,		0x20000+0x20000*6/8,		0x20000+0x20000*7/8,		0x20000+0x20000*8/8,
};

static ROMDATA LONG  AGCMax[] = {
//	OFF,		LOW,		MID,			HIGH
	256,		0x1fff,		0x10000,		0x20000
};

#else

static ROMDATA LONG  AGCGain[] = {
//	auto,	0dB,	2dB,	4dB,	6dB,	8dB,	10dB,	12dB,	14dB,	16dB,	18dB
	256,	256,	322,	406,	512,	643,	810,	1024,	1283,	1615,	2048,

//	20dB,	22dB,	24dB,	26dB,	28dB,
	2560,	3223,	4096,	5108,	6430,
};

 #if defined(__USE_CMOS_EXT2__)
 static ROMDATA LONG  AGCMax[] = {
 //	OFF,		LOW,		MID,			HIGH
 	256,		0x01fff,		0x04fff,		0x10000
 };
 
 #elif defined(__USE_CMOS_EXT1__)
 static ROMDATA LONG  AGCMax[] = {
   #if defined(__EXT_INNER_ANALOG_GAIN_ONLY__)
  	256,		0x01fff,		0x10000,		0x20000
   #else
    #if defined(__USE_DIGITAL_GAIN__)
	256,		0x01fff,		0x20000,		0x40000
    #else
	256,		0x01fff,		0x10000,		0x20000
    #endif
   #endif
 };
 
 #else
 static ROMDATA LONG  AGCMax[] = {
 //	OFF,		LOW,		MID,			HIGH
 	256,		0x1fff,		0x10000,		0x20000
 };
 #endif

#endif



// shutter max
//#define		ROUNDUP_FLOAT(a)	((DWORD)(a*10)%10 >= 5)?  (DWORD)(a+1):  (DWORD)(a)

AE_MENU_PARAM	stAEMENU;
MISP_VIDEO_WINDOW	stCONF_WIN;
extern MISP_VIDEO_WINDOW	stAF_WIN;

BYTE target_con=128, target_sat=152, p_target_con=0, p_target_sat=0;
BYTE p_menu_con=0, con_bound=0, sat_bound=0, WBPreset_cnt_0=0;
BYTE p_menu_edge=0, target_edge=10, p_target_edge=0, edge_bound=0 ;
BYTE p_menu_bright=0, target_bright=0, p_target_bright=0;
BYTE target_ddpc=0, p_target_ddpc=0;
LONG sat_compensation = 0 ;
LONG edge_compensation = 0 ;
LONG bright_compensation = 0;
LONG ddpc_compensation = 0;

LONG saved_stAECTRL = 0;
BYTE AutoSat_Init=LowLight_Mid, AutoSat_Level=LowLight_Mid;
// BYTE AutoSat_Init=LowLight_Off, AutoSat_Level=LowLight_Off; //  Set OFF for tunning

BOOL AutoEdge_Init=ON, AutoEdge_OnOff=ON;

#if !defined(__PANORAMA_SETUP__)
// BYTE AutoBright_Init=LowLight_Mid, AutoBright_Level=LowLight_Mid;

BYTE AutoBright_Init=LowLight_Off, AutoBright_Level=LowLight_Off; // Set OFF for tunning

#else
BYTE AutoBright_Init=LowLight_Off, AutoBright_Level=LowLight_Off;
#endif

#if defined (__RGBGAMMA_USE_ADJUST__)
BOOL AutoCon_Init=OFF, AutoCon_Enable=OFF, AutoCon_OnOff=OFF;
#else
BOOL AutoCon_Init=OFF, AutoCon_Enable=OFF, AutoCon_OnOff=OFF;
#endif


#if defined(__SENSOR_ERR_CHECK__)
static BYTE sensor_wait_cnt=0;
static WORD sensor_h_err_song, sensor_v_err_song =0;
static WORD sensor_err_hong=0;
static WORD frame_cnt=0;
static WORD sensor_vcnt=0;
BOOL sensor_err_en=OFF;
WORD pre_sensor_herr=0, pre_sensor_verr=0;
#endif

WORD sensor_hcnt_old, sensor_vcnt_old, sensor_err_flag=0;

BOOL  MainSrc=0, PIPSrc=0;					//0:Sensor, 1:EXT_IN(HDMI)
	
static BOOL defog_act_before, defog_act = OFF;		//0: OFF, 1: auto, 2: ON
	
WORD  DWindowPos=0;
volatile BOOL fZOOMMove, fDWindowMove;
volatile BOOL fMainCrop = FALSE ;
volatile BOOL fCropMode = FALSE ;
#if defined(__USE_SENS_PANNING_MODE__)
volatile BOOL fSensorPanningMove = TRUE ;
#endif
	
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
static int fConferenceMode = 0;
#endif

BOOL AutoFLKdetected = TRUE;
BOOL AutoFLKremove = FALSE;

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
FLK_CTRL_PARAM	stFLKCTRL;

static int AutoFLKcnt = 0;

static ROMDATA CSUP_THRES ChromaSup_thres[3] ={
// ld_x0, x1, x2, x3,  y0, y1, y2, y3,  s0, s1, s2, ls_x0,x1,  s0,   vd_x0,x1,x2,x3,   y0, y1, y2, y3,   s0, s1, s2
	{{16,32,216,232}, {32,256,256,32}, {1023,0,1024}, { 0,16}, 1023, {32,64,128,192}, {256,240,176,48}, {464,448,384}},	// weak
	{{32,48,208,224}, { 0,255,  0, 0}, {1023,0,1024}, { 0,16}, 1023, {32,64,128,160}, {256,240,144,16}, {464,416,256}},	// normal
	{{40,56,208,224}, { 0,256,256, 0}, {1023,0,1024}, {20,36}, 1023, {32,64, 96,128}, {256,240,128, 0}, {464,288,256}},	// strong
};

#endif


#if defined (__USE_CMOS_MT9P031_5M_PIXEL__)
BYTE	CIS_FPS = CIS_15FPS;
#else
BYTE	CIS_FPS = CIS_30FPS;
#endif

#if (defined(__USE_CMOS_IMX265__) && !defined(__USE_IMX265_SLAVE_MODE__))
BYTE	GS_Sensor_Status=Trig_Stby_Off;
WORD	pulse2sensor_width=100;		//for manu-shut control
BYTE	pulse2sensor_counter=0;
BYTE	pulse2sensor_num=1;

extern VBYTE max_mscnt_trig;
extern VBYTE reset_mscnt_trig;
#endif

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
//extern WORD Dis_return_val;
//extern BYTE DIS_Val_ref_x, DIS_Val_ref_x_1d, DIS_Val_ref_x_2d, DIS_Val_ref_x_3d, DIS_Val_ref_y;
extern SPRITE_CTL_INFO stOSD[12];		

extern WB_CTRL_PARAM stWBCTRL;
extern AE_CTRL_PARAM stAECTRL;

extern DIS_ROI stDISCoor;		// DIS coordinate x,y
	
extern BOOL f_dGain;

#if	defined(__MDIN_i510_REF__)				// i510 REF board
extern BOOL fIOExpIRQ;
#endif
	
// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void DEMO_SetContrast(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveEffectContrast(nID);
#endif
	
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	{SetOUT_MainContrast(nID*12+8); 		SetOUT_AuxContrast(nID*12+8);}
#else 												// MDIN-i51X (i510, i540, i550)
	#if defined(__RGBGAMMA_USE_ADJUST__) && defined(__RGBGAMMA_USE_CONTRAST__)
		SetTONE_RGBGAMMA_val(nID);
	#else
		{SetOUT_MainContrast(nID*12+8); 	SetOUT_AuxContrast(nID*12+8);}
	#endif
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_EFFECT_CONTRAST, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE DEMO_GetContrast(void)
{
	BYTE  val;
	
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	if(GetOUT_MainContrast() > 8) 			val = (GetOUT_MainContrast() - 8) /12;
	else 									val = 0;
#else 												// MDIN-i51X (i510, i540, i550)
	#if defined(__RGBGAMMA_USE_ADJUST__) && defined(__RGBGAMMA_USE_CONTRAST__)
		val = GetTONE_RGBGAMMA_val();
	#else
		if(GetOUT_MainContrast() > 8) 		val = (GetOUT_MainContrast() - 8) /12;
		else 								val = 0;
	#endif
#endif
	
	return  val;
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetBrightness(WORD nID)			
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	#define	Bright_gap	8
	#define	Bright_bypass_id	11
	WORD 	Bright_ref;
	WORD 	Bright_result;

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveEffectBrightness(nID);
#endif
	
	if(nID<Bright_bypass_id) 	Bright_ref = 256-(Bright_bypass_id-nID)*Bright_gap;
	else 					Bright_ref = (nID-Bright_bypass_id)*Bright_gap;

	Bright_result=Bright_ref+(WORD)target_bright;

	if(Bright_result>255) Bright_result=Bright_result-256;
		
	SetDM_ColorCC_Brightness(Bright_result);
	
//signed value, 0~127 positive, 128~255 negative
//	if(nID<12) 	SetDM_ColorCC_Brightness(256-(12-nID)*8);
//	else 		SetDM_ColorCC_Brightness((nID-12)*8);

//	SetOUT_MainBrightness(nID*12+8); 		SetOUT_AuxBrightness(nID*12+8);

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_EFFECT_BRIGHT, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE DEMO_GetBrightness(void)
{
	WORD  bright_bypass_id = 11, bright_gap = 8;
	WORD  val = GetDM_ColorCC_Brightness() % 256;
	
	// remove 'target_bright'
	if(val < target_bright) 		val = (val + 256) - target_bright;
	else 							val = val - target_bright;
	
	// remove 'gap & bypass id'
	if(val < 73) 					val = (val / bright_gap) + bright_bypass_id; 		// val : 0~72 -> 11~20
	else if(val < 128) 			val = 20; 											// val : 73~127 -> 20(max)
	else if(val < 168) 			val = 0; 											// val : 128~167 -> 0(min)
	else 				 			val = bright_bypass_id - (256 - val)/bright_gap; 	// val : 168~255 -> 0~10
	
	return  (BYTE)val;
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetAECTRL_Brightness(WORD nID)		
{

	stAECTRL.level = nID*5;

	AECTRL_SetBrightness(&stAECTRL, stAECTRL.level);

	#if defined (__USE_CMOS_PO3100K__)		//this is po3100k inner isp ae mode
	CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
	switch(nID){
		case 0  : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x10);		break;
		case 1  : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x1A);		break;
		case 2  : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x20);		break;
		case 3  : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x25);		break;
		case 4  : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x2A);		break;
		case 5  : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x30);		break;
		case 6  : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x35);		break;
		case 7  : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x3A);		break;
		case 8  : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x40);		break;
		case 9  : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x45);		break;
		case 10 : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x4A);		break;
		case 11 : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x50);		break;
		case 12 : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x5A);		break;
		case 13 : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x60);		break;
		case 14 : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x6A);		break;
		case 15 : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x70);		break;
		case 16 : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x7A);		break;
		case 17 : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x80);		break;
		case 18 : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x8A);		break;
		case 19 : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x90);		break;
		case 20 : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0xA0);		break;
		default : CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0xB0);		break;
	}
	

	#endif
	
	return;
}
//--------------------------------------------------------------------------------------------------
void DEMO_SetExpSensUp(WORD nID, int mode) // mode : 0 = max value, 1 = set max value & cmos sensup when manual mode, 2 = set max value & cmos sensup always 
{
	WORD	rVal	= 0;
//	WORD	rVal2	= 0;
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[6];
#endif

#if defined (__USE_CMOS_MN34041__)
	switch(nID){
		case 0 : rVal = 0x0000; break;		// off
		case 1 : rVal = 0x0001; break;		// x2
		case 2 : rVal = 0x0002; break;		// x3
		case 3 : rVal = 0x0003; break;		// x4
		case 4 : rVal = 0x0004; break;		// x5
		case 5 : rVal = 0x0005; break;		// x6
		case 6 : rVal = 0x0006; break;		// x7
		case 7 : rVal = 0x0007; break;		// x8
		case 8 : rVal = 0x0008; break;		// x9
		case 9 : rVal = 0x0009; break;		// x10
		case 10 : rVal = 0x0013; break;		// x20
		case 11 : rVal = 0x001d; break;		// x30
		default : rVal = 0x0000; break;		// off
		}
#elif defined (__USE_CMOS_MN34220__) || defined (__USE_CMOS_MN34420__)
	switch(nID){
		case 0 : rVal = 0x0000; break;		// off
		case 1 : rVal = 0x0100; break;		// x2
		case 2 : rVal = 0x0200; break;		// x3
		case 3 : rVal = 0x0300; break;		// x4
		case 4 : rVal = 0x0400; break;		// x5
		case 5 : rVal = 0x0500; break;		// x6
		case 6 : rVal = 0x0600; break;		// x7
		case 7 : rVal = 0x0700; break;		// x8
		case 8 : rVal = 0x0800; break;		// x9
		case 9 : rVal = 0x0900; break;		// x10
		case 10 : rVal = 0x1300; break;		// x20
		case 11 : rVal = 0x1d00; break;		// x30
		default : rVal = 0x0000; break;		// off
		}
#elif defined (__USE_CMOS_IMXx36__)
	switch(nID){
		case 0 : rVal = 0x0465; break;		// off
		case 1 : rVal = 0x08CA; break;		// x2
		case 2 : rVal = 0x0D2F; break;		// x3
		case 3 : rVal = 0x1194; break;		// x4
		case 4 : rVal = 0x15F9; break;		// x5
		case 5 : rVal = 0x1A5E; break;		// x6
		case 6 : rVal = 0x1EC3; break;		// x7
		case 7 : rVal = 0x2328; break;		// x8
		case 8 : rVal = 0x278D; break;		// x9
		case 9 : rVal = 0x2Bf2; break;		// x10
		case 10 : rVal = 0x57E4; break;		// x20
		case 11 : rVal = 0x83D6; break;		// x30
		default : rVal = 0x0465; break;		// off
	}
#elif defined (__USE_CMOS_IMX124__)
	#if defined(__USE_IMX124_1536P30_MODE__)
	switch(nID){
		case 0 : rVal = 0x0640; break;		// off
		case 1 : rVal = 0x0C80; break;		// x2
		case 2 : rVal = 0x12C0; break;		// x3
		case 3 : rVal = 0x1900; break;		// x4
		case 4 : rVal = 0x1F40; break;		// x5
		case 5 : rVal = 0x2580; break;		// x6
		case 6 : rVal = 0x2BC0; break;		// x7
		case 7 : rVal = 0x3200; break;		// x8
		case 8 : rVal = 0x3840; break;		// x9
		case 9 : rVal = 0x3E80; break;		// x10
		case 10 : rVal = 0x7D00; break;		// x20
		case 11 : rVal = 0x83D6; break;		// x30
		default : rVal = 0xBB80; break;		// off
	}	
	#else
	switch(nID){
		case 0 : rVal = 0x0465; break;		// off
		case 1 : rVal = 0x08CA; break;		// x2
		case 2 : rVal = 0x0D2F; break;		// x3
		case 3 : rVal = 0x1194; break;		// x4
		case 4 : rVal = 0x15F9; break;		// x5
		case 5 : rVal = 0x1A5E; break;		// x6
		case 6 : rVal = 0x1EC3; break;		// x7
		case 7 : rVal = 0x2328; break;		// x8
		case 8 : rVal = 0x278D; break;		// x9
		case 9 : rVal = 0x2Bf2; break;		// x10
		case 10 : rVal = 0x57E4; break;		// x20
		case 11 : rVal = 0x83D6; break;		// x30
		default : rVal = 0x0465; break;		// off
	}
	#endif
#elif defined (__USE_CMOS_IMX252__)
	#if defined(__USE_IMX252_1536P_ALL_SCAN_MODE__)
	switch(nID){
		case 0 : rVal = 0x0640; break;		// off
		case 1 : rVal = 0x0C80; break;		// x2
		case 2 : rVal = 0x12C0; break;		// x3
		case 3 : rVal = 0x1900; break;		// x4
		case 4 : rVal = 0x1F40; break;		// x5
		case 5 : rVal = 0x2580; break;		// x6
		case 6 : rVal = 0x2BC0; break;		// x7
		case 7 : rVal = 0x3200; break;		// x8
		case 8 : rVal = 0x3840; break;		// x9
		case 9 : rVal = 0x3E80; break;		// x10
		case 10 : rVal = 0x7D00; break;		// x20
		case 11 : rVal = 0x83D6; break;		// x30
		default : rVal = 0xBB80; break;		// off
	}	
	#else
	switch(nID){
		case 0 : rVal = 0x0465; break;		// off
		case 1 : rVal = 0x08CA; break;		// x2
		case 2 : rVal = 0x0D2F; break;		// x3
		case 3 : rVal = 0x1194; break;		// x4
		case 4 : rVal = 0x15F9; break;		// x5
		case 5 : rVal = 0x1A5E; break;		// x6
		case 6 : rVal = 0x1EC3; break;		// x7
		case 7 : rVal = 0x2328; break;		// x8
		case 8 : rVal = 0x278D; break;		// x9
		case 9 : rVal = 0x2Bf2; break;		// x10
		case 10 : rVal = 0x57E4; break;		// x20
		case 11 : rVal = 0x83D6; break;		// x30
		default : rVal = 0x0465; break;		// off
	}
	#endif		
#elif defined (__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX385__)  || defined(__USE_CMOS_IMX327__) \
	|| defined (__USE_CMOS_IMX253__) || defined(__USE_CMOS_IMX334__) || defined(__USE_CMOS_IMX265__)
	switch(nID){
 		case 0 : rVal = AECTRL_GetSensUpMin()*1; break;		// off
		case 1 : rVal = AECTRL_GetSensUpMin()*2; break;		// x2
		case 2 : rVal = AECTRL_GetSensUpMin()*3; break;		// x3
		case 3 : rVal = AECTRL_GetSensUpMin()*4; break;		// x4
		case 4 : rVal = AECTRL_GetSensUpMin()*5; break;		// x5
		case 5 : rVal = AECTRL_GetSensUpMin()*6; break;		// x6
		case 6 : rVal = AECTRL_GetSensUpMin()*7; break;		// x7
		case 7 : rVal = AECTRL_GetSensUpMin()*8; break;		// x8
		case 8 : rVal = AECTRL_GetSensUpMin()*9; break;		// x9
		case 9 : rVal = AECTRL_GetSensUpMin()*10; break;		// x10
		case 10 : rVal = AECTRL_GetSensUpMin()*12; break;		// x12
		case 11 : rVal = AECTRL_GetSensUpMin()*15; break;		// x15
		default : rVal = AECTRL_GetSensUpMin()*1; break;		// off
	}
#elif defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX273__)
	switch(nID){
 		case 0 : rVal = AECTRL_GetSensUpMin()*1; break;		// off
		case 1 : rVal = AECTRL_GetSensUpMin()*2; break;		// x2
		case 2 : rVal = AECTRL_GetSensUpMin()*3; break;		// x3
		case 3 : rVal = AECTRL_GetSensUpMin()*4; break;		// x4
		case 4 : rVal = AECTRL_GetSensUpMin()*5; break;		// x5
		case 5 : rVal = AECTRL_GetSensUpMin()*6; break;		// x6
		case 6 : rVal = AECTRL_GetSensUpMin()*7; break;		// x7
		case 7 : rVal = AECTRL_GetSensUpMin()*8; break;		// x8
		case 8 : rVal = AECTRL_GetSensUpMin()*9; break;		// x9
		case 9 : rVal = AECTRL_GetSensUpMin()*10; break;		// x10
		case 10 : rVal = AECTRL_GetSensUpMin()*12; break;		// x12
		default : rVal = AECTRL_GetSensUpMin()*1; break;		// off
	}
#elif defined (__USE_CMOS_IMX185__)
	switch(nID){
		case 0 : rVal = 0x0465; break;		// off
		case 1 : rVal = 0x08CA; break;		// x2
		case 2 : rVal = 0x0D2F; break;		// x3
		case 3 : rVal = 0x1194; break;		// x4
		case 4 : rVal = 0x15F9; break;		// x5
		case 5 : rVal = 0x1A5E; break;		// x6
		case 6 : rVal = 0x1EC3; break;		// x7
		case 7 : rVal = 0x2328; break;		// x8
		case 8 : rVal = 0x278D; break;		// x9
		case 9 : rVal = 0x2Bf2; break;		// x10
		case 10 : rVal = 0x57E4; break;		// x20
		case 11 : rVal = 0x83D6; break;		// x30
		default : rVal = 0x0465; break;		// off
	}
#elif defined (__USE_CMOS_IMXx22__)  || defined (__USE_CMOS_IMX323__)
	switch(nID){
		case 0 : rVal = 0x0465; break;		// off
		case 1 : rVal = 0x08CA; break;		// x2
		case 2 : rVal = 0x0D2F; break;		// x3
		case 3 : rVal = 0x1194; break;		// x4
		case 4 : rVal = 0x15F9; break;		// x5
		case 5 : rVal = 0x1A5E; break;		// x6
		case 6 : rVal = 0x1EC3; break;		// x7
		case 7 : rVal = 0x2328; break;		// x8
		case 8 : rVal = 0x278D; break;		// x9
		case 9 : rVal = 0x2Bf2; break;		// x10
		case 10 : rVal = 0x57E4; break;		// x20
		case 11 : rVal = 0x83D6; break;		// x30
		default : rVal = 0x0465; break;		// off
	}
#elif defined (__USE_CMOS_EXT2__) || defined(__USE_CMOS_EXT1__)
	switch(nID){
		case 0 : rVal = 0x0465; break;		// off
		case 1 : rVal = 0x08CA; break;		// x2
		case 2 : rVal = 0x0D2F; break;		// x3
		case 3 : rVal = 0x1194; break;		// x4
		case 4 : rVal = 0x15F9; break;		// x5
		case 5 : rVal = 0x1A5E; break;		// x6
		case 6 : rVal = 0x1EC3; break;		// x7
		case 7 : rVal = 0x2328; break;		// x8
		case 8 : rVal = 0x278D; break;		// x9
		case 9 : rVal = 0x2Bf2; break;		// x10
		case 10 : rVal = 0x57E4; break;		// x20
		case 11 : rVal = 0x83D6; break;		// x30
		default : rVal = 0x0465; break;		// off
	}
#endif
	if(mode == 2) {
		AECTRL_SetSensUpVal(rVal);
		AECTRL_SetSensUpMax(rVal);
		//UARTprintf("nID0:%d\r\n",nID);



#if defined(__USE_CMOS_IMX226__)
		AE_UpdateSensUpVal(1,rVal);
#else
		AE_UpdateSensUpVal(0,rVal);
#endif
	} else {
#if defined(__USE_MANUAL_LENS__) || defined(__USE_2MOTOR_LENS__)
		if((GetMenuStatus(EXP_MODE_MENU) == 1) || (GetMenuStatus(EXP_MODE_MENU) == 3)) { // manual mode
#else
		if((GetMenuStatus(EXP_MODE_MENU) == 1) || (GetMenuStatus(EXP_MODE_MENU) == 4)) { // manual mode
#endif
			AECTRL_SetSensUpVal(rVal);
			AECTRL_SetSensUpMax(rVal);
#if defined(__USE_CMOS_IMX226__)
			AE_UpdateSensUpVal(1,rVal);
#else
			AE_UpdateSensUpVal(0,rVal);
#endif
		} else {
			if(mode == 0) {
				if(rVal < AECTRL_GetSensUpVal()) {
					AECTRL_SetSensUpVal(rVal);
#if defined(__USE_CMOS_IMX226__)
					AE_UpdateSensUpVal(1,rVal);
#else
					AE_UpdateSensUpVal(0,rVal);
#endif
				}					

				AECTRL_SetSensUpMax(rVal);	// auto, shut/iris/agc fix
				//UARTprintf("nID2:%d\r\n",nID);
			} else {
				if(rVal < AECTRL_GetSensUpVal())
					AECTRL_SetSensUpVal(rVal);
				//UARTprintf("nID3:%d\r\n",nID);
#if defined(__USE_CMOS_IMX226__)
				AE_UpdateSensUpVal(1,rVal);
#else
				AE_UpdateSensUpVal(0,rVal);
#endif
			}
		}
	}
	
	stAEMENU.sensup[stAEMENU.mode] = nID;
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = stAEMENU.sensup[0]; 		// auto
	buff[2] = stAEMENU.sensup[1]; 		// shut-fix
	buff[3] = stAEMENU.sensup[2]; 		// iris-fix
	buff[4] = stAEMENU.sensup[3]; 		// agc-fix
	buff[5] = stAEMENU.sensup[4]; 		// manual
	EEPROM_MultiWrite(EEPROM_EXPOSURE_SENSUP, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
WORD DEMO_GetExpSensUp(void)
{
	return  stAEMENU.sensup[stAEMENU.mode];
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetImageEffect(WORD nID)		
{
	WORD	ColorTone, effect_lv;
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveImageEffect(nID);
#endif
	
	if((nID >= 3) && (nID < 7))				{effect_lv = nID-3;		ColorTone = 3;}		// red
	else if((nID >= 7) && (nID < 11))		{effect_lv = nID-7;		ColorTone = 4;}		// blue
	else if((nID >= 11) && (nID < 15))		{effect_lv = nID-11;	ColorTone = 5;}		// green
	else 									{effect_lv = 0;			ColorTone = nID;}	// off, neg, grey

	MISP_RegField(MISP_LOCAL_ID, 0x767, 0, 2, effect_lv);
	MISP_RegField(MISP_LOCAL_ID, 0x767, 2, 3, ColorTone);
	MISP_RegField(MISP_LOCAL_ID, 0x767, 5, 1, ((nID==0)? OFF: ON));
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_DISP_EFFECT, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE DEMO_GetImageEffect(void)
{
	WORD	color_tone, effect_lv, effect_en, temp;
	
	MISP_RegRead(MISP_LOCAL_ID, 0x767, &temp);
	
	effect_lv = (temp >> 0) & 0x03;
	color_tone = (temp >> 2) & 0x07;
	effect_en = (temp >> 5) & 0x01;
	
	if(effect_en)	{
		switch(color_tone)	{
			case  1:  return  1; 					// negative
			case  2:  return  2; 					// grey-mode
			case  3:  return  (effect_lv +3); 	// reddish-1 ~ 4
			case  4:  return  (effect_lv +7); 	// bluish-1 ~ 4
			case  5:  return  (effect_lv +11); 	// greenish-1 ~ 4
			default:	return  0; 				// off
		}
	}
	else 				return  0; 				// off
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetImageSize(void)
{
	BYTE bH ;
	BYTE bV ;
	WORD active_H ;
	WORD active_V ;
	
	bH = GetSS_image_boundary_size_h();
	bV = GetSS_image_boundary_size_v();
	active_H = GetSS_image_active_size_h();
	active_V = GetSS_image_active_size_v();
	
	SetBY_image_size(bH,bV,active_H,active_V) ;
	SetNR_image_size(bH,bV,active_H,active_V) ;
	SetDM_image_size(bH,bV,active_H,active_V) ;
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetSaturation(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveEffectSaturation(nID);
#endif
	
	// if(nID==0) 		{SetOUT_MainSaturation(0); 			SetOUT_AuxSaturation(0);}
	// else			{SetOUT_MainSaturation(nID*12+8); 	SetOUT_AuxSaturation(nID*12+8);}
	
	if(nID==0) 		{SetApp_AutoSat_OutSaturation(0,4);}
	else			{SetApp_AutoSat_OutSaturation((nID*12+8),4);}	
	
	
#if defined(__USE_WDR_PROCESS__)  // set WDR max.satur as weaker than normal
	if(nID==0) 		SetWDR_SaturLevel(0);
	else			SetWDR_SaturLevel(nID*10+4);
#endif

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_EFFECT_SAT, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE DEMO_GetSaturation(void)
{
	if(GetOUT_MainSaturation() < 8) 		return  0;
	else 									return  (GetOUT_MainSaturation() - 8) /12;
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetHue(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveEffectHue(nID);
#endif
	
	if(nID==0) 		{SetOUT_MainHue(0); 			SetOUT_AuxHue(0);}
	else			{SetOUT_MainHue(nID*12+8); 		SetOUT_AuxHue(nID*12+8);}

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_EFFECT_HUE, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE DEMO_GetHue(void)
{
	if(GetOUT_MainHue() < 8) 		return  0;
	else 							return  (GetOUT_MainHue() - 8) /12;
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetSharpness(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveEffectEdgeEnhance(nID);
#endif
	
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	WORD VPF_gain_level ;

	VPF_gain_level = nID*6+16 ;
	
	if ( VPF_gain_level > 127 ) VPF_gain_level = 127 ;
	else if ( VPF_gain_level == 16 ) VPF_gain_level = 0 ;
	
	SetFC_MainVPFLevel(VPF_gain_level);
	
#if defined(__M33_V101__)
	SetIN_MainHPFLevel(nID*4);
#else
	SetIN_MainHPFLevel(nID*3);
#endif
#else 								// MDIN-i51X (i510, i540, i550), VPF is BPF, use the same peaking gain as HPF
	// #if defined(__USE_MDIN_i5XX_REV__)
	// SetFC_MainVPFLevel(nID*6);	
	// #endif
	
	// SetIN_MainHPFLevel(nID*6);
	
	// #if defined(__USE_MDIN_i5XX_REV__)
	// SetFC_MainVPFLDslope(nID*6) ; 
	// #endif
	
	// SetIN_MainHPFLDslope(nID*6) ;		
	SetApp_AutoEDGE_HVPFLevel(nID*6); //for test
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_EFFECT_EDGEENH, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE DEMO_GetSharpness(void)
{
#if defined(__USE_MDIN_i500__)		// MDIN-i500
#if defined(__M33_V101__)
	return  GetIN_MainHPFLevel() /4;
#else
	return  GetIN_MainHPFLevel() /3;
#endif
#else 								// MDIN-i51X (i510, i540, i550)
	// return  GetIN_MainHPFLevel() /4;
	// return  GetIN_MainHPFLevel() /6; // Match with the DEMO_SetSharpness()
	return  GetApp_AutoEDGE_HVPFLevel() /6; // For test // Match with the DEMO_SetSharpness()
#endif
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetAutoConOnOff(BOOL OnOff)
{
	AutoCon_OnOff=OnOff;
	AutoCon_Enable=OnOff;

	//if visca control then call visca menu defalut..(code will fix..)
	if(OnOff==OFF)	SetOUT_MainContrast(GetMenuStatus(CONTRAST_MENU)*12 + 8);
	
}

//--------------------------------------------------------------------------------------------------
BOOL DEMO_GetAutoConOnOff(void)
{
	return AutoCon_OnOff;
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetAutoSaturation(BYTE nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveEffectAutoSaturate(nID);
#endif
	
	if(nID==0)	AutoSat_Level=LowLight_Off;
	else if(nID==1)	AutoSat_Level=LowLight_Low;
	else if(nID==2)	AutoSat_Level=LowLight_Mid;
	else if(nID==3)	AutoSat_Level=LowLight_High;	
	
	SetApp_AutoSat_Intensity(nID);

	//if visca control then call visca menu defalut..(code will fix..)
	// if(AutoSat_Level==LowLight_Off){
		// target_sat = GetMenuStatus(SATURATION_MENU)*12 + 8;
		// p_target_sat = target_sat;
		// SetOUT_MainSaturation(target_sat);
		// SetOUT_AuxSaturation(target_sat);  
	// }
	
	
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_EFFECT_AUTOSAT, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE DEMO_GetAutoSatLevel(void)
{
	// switch(AutoSat_Level)	{
		// case  LowLight_Off: 	return  0;
		// case  LowLight_Low: 	return  1;
		// case  LowLight_Mid: 	return  2;
		// case  LowLight_High: 	return  3;
		// default: 				return  0;
	// }
	
	return GetApp_AutoSat_Intensity() ;
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetAutoEdgeOnOff(BOOL OnOff)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveEffectAutoEdge(OnOff);
#endif
	
	// AutoEdge_OnOff=OnOff;

	//if visca control then call visca menu defalut..(code will fix..)
	// if(OnOff==OFF){
		// target_edge = GetMenuStatus(EDGE_MENU);
		// p_target_edge = target_edge;			
		// DEMO_SetSharpness(target_edge);
	// }
	
	SetApp_AutoEDGE_Intensity(OnOff); // for test
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)OnOff;
	EEPROM_MultiWrite(EEPROM_EFFECT_AUTOEDGE, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BOOL DEMO_GetAutoEdgeOnOff(void)
{
	// return  AutoEdge_OnOff;
	return  GetApp_AutoEDGE_Intensity() ;
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetAutoBright(BYTE nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	if(nID==0)	AutoBright_Level=LowLight_Off;
	else if(nID==1)	AutoBright_Level=LowLight_Low;
	else if(nID==2)	AutoBright_Level=LowLight_Mid;
	else if(nID==3)	AutoBright_Level=LowLight_High;	

	if(AutoBright_Level==LowLight_Off){
		target_bright=0;
		p_target_bright=target_bright;	
		DEMO_SetBrightness(GetMenuStatus(BRIGHTNESS_MENU));
	}
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_EFFECT_AUTOBRHT, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE DEMO_GetAutoBright(void)
{
	switch(AutoBright_Level)	{
		case  LowLight_Off: 	return  0;
		case  LowLight_Low: 	return  1;
		case  LowLight_Mid: 	return  2;
		case  LowLight_High: 	return  3;
		default: 				return  0;
	}
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetZoomMode(WORD nID)
{
	WORD DZRatio=0, DZPosiX=120, DZPosiY=120; 
	WORD PIPOnOff = GetPIP_DispOnOff();
	WORD PIPSize = GetMenuStatus(PIPSIZE_MENU);
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[5];
	
	EEPROM_MultiRead(EEPROM_DZOOM_MENU, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE)		DZRatio = buff[1];
	if(buff[2] == EEPROM_SAVE_DONE)		{DZPosiX = buff[3];		DZPosiY = buff[4];}
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveDZOOMMode(nID);
#endif
	
	if (nID)	{
		// dzoom <-> dis/wdr/rotation can't running at same time.
		if(GetDIS_OnOff()) 			{Demo_DIS_OnOff(OFF); 		SetMenuStatus(DIS_MENU, OFF);} 			// dis 'off'
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		if(GetROT_RotationEnable())	{SetROT_RotationEnable(OFF);	SetMenuStatus(DISP_ROT_MENU, OFF);}	// rotation 'off'
#endif
#if defined(__USE_MDIN_i500__)
		if(GetWDR_ProcesMode())		{DEMO_SetWDR_MODE(OFF); 	SetMenuStatus(WDR_MENU, OFF);} 			// wdr 'off'
#endif

		if(DEMO_GetMirrorMode()&1)		DZPosiX = 240 - DZPosiX;	// h-mirror
		if(DEMO_GetMirrorMode()&2)		DZPosiY = 240 - DZPosiY;	// v-mirror
		
		SetDZOOM_Status(ON);
		SetDZOOM_DirectRatio(DZRatio);			// dzoom (x1 -> xx)
		SetMenuStatus(DZRATIO_MENU, DZRatio);
		SetDZOOM_Position(DZPosiX, DZPosiY);
		SetPIPSource(MainSrc); 					// set aux source to main source. 0:sensor, 1:ext(hdmi)
		SetPIP_Process(ON);			SetPIP_BoxOSDOnOff(PIPOnOff); 		// pip box osd move
		DEMO_SetPIPSize(PIPSize);
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
		LENS_SetDIGIMixOnOff(&stAFCTRL, OFF);
		LENS_SetDIGIDrvOnOff(&stAFCTRL, OFF);
#endif
	}
	else		{
		SetDZOOM_Status(OFF);
		SetDZOOM_DirectRatio(0);				// dzoom (xx -> x1)
		SetMenuStatus(DZRATIO_MENU, 0);
		SetPIP_Process(OFF);			SetPIP_BoxOSDOnOff(OFF); 		// pip box osd move
		fZOOMMove = FALSE; 
		MISP_RegField(MISP_LOCAL_ID, 0x480, 11, 1, 1);
		MISP_RegWrite(MISP_LOCAL_ID, 0x09E, 0x0000);	//overrun check disable
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
		LENS_SetDIGIMixOnOff(&stAFCTRL, ON);
		LENS_SetDIGIDrvOnOff(&stAFCTRL, ON);
#endif
	}
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_SPECIAL_DZOOM, (PBYTE)&buff[0], 2);
#endif
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetZoomRatio(WORD nID)			//0:x1.0,,69:x7.9,,,126:x64
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveDZOOMRatio(nID);
#endif
	
	SetDZOOM_TargetRatio(nID);							// dzoom ratio control
	SetPIP_Process(ON);									// pip box osd move
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetZoomPosition(WORD nID)
{
	WORD Posi_x = GetMenuStatus(DZPOSI_MENU);
	WORD Posi_y = GetMenuPosiY(DZPOSI_MENU);
	
#if defined(__USE_4K60_INOUT__)
	return;
#endif
	
	SetDZOOM_Position(Posi_x, Posi_y);					// dzoom position move
	SetPIP_Process(ON);									// pip box osd move
}

//--------------------------------------------------------------------------------------------------
void DEMO_SaveZoomRatioPosi(void)
{
	WORD DZRatio, DZPosiX, DZPosiY; 

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[5];
#endif
	
	DZRatio = GetMenuStatus(DZRATIO_MENU);
	DZPosiX = GetMenuStatus(DZPOSI_MENU);
	DZPosiY = GetMenuPosiY(DZPOSI_MENU);
	
	if(DEMO_GetMirrorMode()&1)		DZPosiX = 240 - DZPosiX;	// h-mirror
	if(DEMO_GetMirrorMode()&2)		DZPosiY = 240 - DZPosiY;	// v-mirror
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)DZRatio;
	buff[2] = EEPROM_SAVE_DONE;			buff[3] = (BYTE)DZPosiX;
	buff[4] = (BYTE)DZPosiY;
	EEPROM_MultiWrite(EEPROM_DZOOM_MENU, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
static void DEMO_SetDZOOMPosiX(BOOL dir)
{
	WORD Posi_x = GetMenuStatus(DZPOSI_MENU);
	WORD Posi_y = GetMenuPosiY(DZPOSI_MENU);
	
	if(dir)		{if(Posi_x < 240)		Posi_x++;}		// move right
	else		{if(Posi_x > 0)			Posi_x--;}		// move left
	
	SetMenuStatus(DZPOSI_MENU, Posi_x);					// save menu status
	SetDZOOM_Position(Posi_x, Posi_y);					// dzoom position move
	SetPIP_Process(ON);									// pip box osd move
}

//--------------------------------------------------------------------------------------------------
static void DEMO_SetDZOOMPosiY(BOOL dir)
{
	WORD Posi_x = GetMenuStatus(DZPOSI_MENU);
	WORD Posi_y = GetMenuPosiY(DZPOSI_MENU);
	
	if(dir)		{if(Posi_y < 240)		Posi_y++;}		// move down
	else		{if(Posi_y > 0)			Posi_y--;}		// move up
	
	SetMenuPosiY(DZPOSI_MENU, Posi_y);					// save menu status
	SetDZOOM_Position(Posi_x, Posi_y);					// dzoom position move
	SetPIP_Process(ON);									// pip box osd move
}

//--------------------------------------------------------------------------------------------------
void IrCodeZoomMoveDirL(void)
{
	DEMO_SetDZOOMPosiX(NEGA);		// move left
}

//--------------------------------------------------------------------------------------------------
void IrCodeZoomMoveDirR(void)
{
	DEMO_SetDZOOMPosiX(POSI);		// move right
}

//--------------------------------------------------------------------------------------------------
void IrCodeZoomMoveDirU(void)
{
	DEMO_SetDZOOMPosiY(NEGA);		// move up
}

//--------------------------------------------------------------------------------------------------
void IrCodeZoomMoveDirD(void)
{
	DEMO_SetDZOOMPosiY(POSI);		// move down
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetCascade(BYTE mode)
{
	WORD  hact_m = GetIN_MainSrcSizeH(), vact_m = GetIN_MainSrcSizeV();	// input video
	WORD  hact_a = GetIN_AuxSrcSizeH(),  vact_a = GetIN_AuxSrcSizeV();	// input video
	WORD  hact_o = GetOUT_MainSizeHA(),  vact_o = GetOUT_MainSizeVA();	// output video
	BYTE  frmt_m = GetOUT_MainFrmt();

	//refresh aux input to update clock configures
	switch(GetIN_AuxPath()) {
		case MISP_AUXPATH_EXT_P0:  SetIN_AuxSrcExtPort0();  break;
		case MISP_AUXPATH_EXT_P1:  SetIN_AuxSrcExtPort1();  break;
		case MISP_AUXPATH_SENSOR:  SetIN_AuxSrcSensor();    break;
	}

	if(mode==1)	{				// main/aux 1:1
		DWindowPos=(hact_o/2);			// center

		//main
		SetFC_MainFCsrcSizeH(hact_m-DWindowPos);	SetFC_MainFCsrcSizeV(vact_m);	// mfc source
		SetFC_MainFCdstSizeH(hact_o-DWindowPos);	SetFC_MainFCdstSizeV(vact_o);	// mfc dest.
		SetFC_MainFCsrcPosiX(DWindowPos);			SetFC_MainFCsrcPosiY(0);		// mfc src position
		SetFC_MainFCdstPosiX(0);					SetFC_MainFCdstPosiY(0);		// mfc dst position
		
		//aux
		SetFC_AuxFCsrcSizeH(DWindowPos);			SetFC_AuxFCsrcSizeV(vact_a);	// aux source
		SetFC_AuxFCdstSizeH(DWindowPos);			SetFC_AuxFCdstSizeV(vact_o);	// aux dest.		
		SetFC_AuxFCsrcPosiX(0);						SetFC_AuxFCsrcPosiY(0);			// aux src position
		SetFC_AuxFCdstPosiX(hact_o-DWindowPos);		SetFC_AuxFCdstPosiY(0);			// aux dst position

		//misc
		SetOUT_AuxMode(GetOUT_MainMode());			SetOUT_AuxFrmt(frmt_m);			// set aux frmt/mode with main frmt/mode
		SetOUT_AuxPipEN(ON);						SetOUT_AuxDispEN(ON);			// aux pip enable

		fDWindowMove=TRUE;
		OSD_DrawOutlineQuad(OFF, hact_o, vact_o);	OSD_CHNoDisplay(OFF, hact_o, vact_o);
		OSD_IrkeyTitleDisplay(ON,  "CASCADE ON");
	}
	else if((mode==2) || (mode==3))	{		// 4CH
		//main
		SetFC_MainFCsrcSizeH(hact_m);				SetFC_MainFCsrcSizeV(vact_m);	// mfc source
		SetFC_MainFCdstSizeH(hact_o/2);				SetFC_MainFCdstSizeV(vact_o/2);	// mfc dest.
		SetFC_MainFCsrcPosiX(0);					SetFC_MainFCsrcPosiY(0);		// mfc src position
		SetFC_MainFCdstPosiX(0);					SetFC_MainFCdstPosiY((mode==2)? (vact_o/2): 0);  // mfc dst position

		//aux
		SetFC_AuxFCsrcSizeH(hact_a);				SetFC_AuxFCsrcSizeV(vact_a);	// aux source
		SetFC_AuxFCdstSizeH(hact_o/2);				SetFC_AuxFCdstSizeV(vact_o/2);	// aux dest.
		SetFC_AuxFCsrcPosiX(0);						SetFC_AuxFCsrcPosiY(0);			// aux src position
		SetFC_AuxFCdstPosiX(hact_o/2);				SetFC_AuxFCdstPosiY((mode==2)? (vact_o/2): 0);  // aux dst position

		//misc
		SetOUT_AuxMode(GetOUT_MainMode());			SetOUT_AuxFrmt(frmt_m);			// set aux frmt/mode with main frmt/mode
		SetOUT_AuxPipEN(ON);						SetOUT_AuxDispEN(ON);			// aux pip enable

		fDWindowMove=FALSE;
		OSD_IrkeyTitleDisplay(OFF, "          ");
		OSD_DrawOutlineQuad(ON, hact_o, vact_o);	OSD_CHNoDisplay(ON, hact_o, vact_o);
	}
	else	{					// off
		SetFC_MainFCsrcSizeH(hact_m);				SetFC_MainFCsrcSizeV(vact_m);	// mfc source
		SetFC_MainFCdstSizeH(hact_o);				SetFC_MainFCdstSizeV(vact_o);	// mfc dest.
		SetFC_MainFCsrcPosiX(0);					SetFC_MainFCsrcPosiY(0);		// mfc src position
		SetFC_MainFCdstPosiX(0);					SetFC_MainFCdstPosiY(0);		// mfc dst position
		
		SetOUT_AuxPipEN(OFF);						SetOUT_CVBSOutEN();				// aux pip off & cvbs on
		
		fDWindowMove=FALSE;
		OSD_IrkeyTitleDisplay(OFF, "          ");
		OSD_DrawOutlineQuad(OFF, hact_o, vact_o);	OSD_CHNoDisplay(OFF, hact_o, vact_o);
	}
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetPIPOnOff(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlavePIPMode(nID);
#endif
	
	SetPIP_DispOnOff(nID);
	SetPIP_BoxOSDOnOff((nID)? ON: OFF);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_DZOOM_PIPONOFF, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetPIPMode(WORD nID)
{
	if(nID) 	SetPIPSource(MainSrc); 	// pip on : aux src <- main src(sensor/ext_in),  pip off : aux src <- main out
	
	DEMO_SetPIPOnOff(nID);
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetPIPSize(WORD nID)							//0:1/4, 1:1/9, 2:1/16, 3:1/25
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlavePIPSize(nID);
#endif

#if defined(__USE_CMOS_IMX226__)||defined(__USE_CMOS_IMX274__)||defined(__USE_CMOS_IMX326__)||defined(__USE_CMOS_IMX334__) // 4K sensors restrict PiP size
	if(nID < 2)		{nID += 2;		SetMenuStatus(PIPSIZE_MENU, nID);}
#endif

	SetPIP_Size(nID);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_DZOOM_PIPSIZE, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetPIPPosition(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[3];
#endif
	
	WORD Posi_x = GetMenuStatus(PIPPOSI_MENU);		// 0~10
	WORD Posi_y = GetMenuPosiY(PIPPOSI_MENU);		// 0~10
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	Posi_x = 10;						// fix x position
	SPImAPP_SetSlavePIPPosi(Posi_y);
#endif
	
	SetPIP_Position(Posi_x, Posi_y);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = (BYTE)Posi_x;
	buff[2] = (BYTE)Posi_y;
	EEPROM_MultiWrite(EEPROM_DZOOM_PIPPOSI, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void Demo_DIS_InitOverScan(BOOL OnOff)
{
	if(OnOff){
		SetFC_MainFCsrcOverscanH(stDISCoor.DIS_OFFSET_X*2);
		SetFC_MainFCsrcOverscanV(stDISCoor.DIS_OFFSET_Y*2);
		SetFC_MainFCsrcPosiX(stDISCoor.DIS_OFFSET_X);
		SetFC_MainFCsrcPosiY(stDISCoor.DIS_OFFSET_Y);
	}
	else{
		SetFC_MainFCsrcOverscanH(0);
		SetFC_MainFCsrcOverscanV(0);
		SetFC_MainFCsrcPosiX(0);
		SetFC_MainFCsrcPosiY(0);
	}
}

//--------------------------------------------------------------------------------------------------
void Demo_DIS_OnOff(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_SPECIAL_DIS, (PBYTE)&buff[0], sizeof(buff));
#endif

	if (nID==GetDIS_OnOff())		return;		// If same mode, just return
	
	//this is do not us front format convertor case, this is init
	if(nID){
		
//		if(GetIN_MainSrcSizeH() != GetOUT_MainSizeHA()) {
//			SetMenuStatus(DIS_MENU, OFF);
//			OSD_MessageBox(ON, " In/Out Res. is Diff");
//			return;
//		}
		if(GetNR_LowLightMode()) {			// If 'LOW LIGHT' mode, just return.
			SetMenuStatus(DIS_MENU, OFF);
			OSD_MessageBox(ON, " LOW LIGHT MODE !!  ");
			return;
		}
		
		// dis <-> dzoom/wdr/mirror/rotation can't running at same time
		// dis <-> dzoom/wdr/rotation can't running at same time
//		if(GetPIP_DispOnOff())			{DEMO_SetPIPOnOff(OFF);		SetMenuStatus(PIPMODE_MENU, OFF);}		// pip 'off'
		if(GetDZOOM_OnOff()) 			{DEMO_SetZoomMode(OFF); 	SetMenuStatus(DZMODE_MENU, OFF);} 		// dzoom 'off'
//		if(DEMO_GetMirrorMode()) 		{DEMO_SetMirrorMode(OFF); 	SetMenuStatus(DISP_MIRROR_MENU, OFF);} 	// mirror 'off'
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		if(GetROT_RotationEnable())	{SetROT_RotationEnable(OFF);	SetMenuStatus(DISP_ROT_MENU, OFF);}	// rotation 'off'
#endif
		if(GetWDR_ProcesMode()) 		{DEMO_SetWDR_MODE(OFF); 	SetMenuStatus(WDR_MENU, OFF);} 			// wdr 'off'
		
		// set NR mode (2D only)	// delete by kw5210 20170306
		SetNR_Mode(MISP_NR_2D);		SetMenuStatus(DNR_MODE_MENU, MISP_NR_2D);	// NR(3D-off, 2D-on)
//		SetNR_Mode(MISP_NR_OFF);		SetMenuStatus(DNR_MODE_MENU, MISP_NR_OFF);	// NR(off)
		
		// set DIS interest area. set overscan area.
		Set_DIS_Interesting_Area();
		
		SetOUT_MainDarkScreenAuto(30);	// main display turn-on delay = 16ms x 30 = 480ms
		
		

	#if defined( __USE_SENS_PANNING_MODE__)
		// no overscan
			DIS_OVERSCAN=OFF;	
	#elif defined(__MN34420_1280x1024_ISPCROP__)
		// no overscan
			DIS_OVERSCAN=OFF;			
	#else
		// overscan mode on.
		if(DIS_OVERSCAN==OFF) {
			DIS_OVERSCAN=ON;
			Demo_DIS_InitOverScan(ON);
			UARTprintf("From now on, running overscan for dis \r\n");					
		}
	#endif

		//----------------- AUX -----------------------------------------------------------
		// set AUX.
		SetOUT_AuxDispEN(OFF);
		SetOUT_AuxMode(MISP_OUT_EMB422_8);
		SetOUT_AuxUsedByFunc(ON);
		
		if(GetIN_MainSrcSizeH() == 3840)		SetOUT_AuxFrmt(MISP_VIDOUT_DIS8M);	// 8M
		else if((GetIN_MainSrcSizeH() == 2048)&&(GetIN_MainSrcSizeV() == 1208))	SetOUT_AuxFrmt(MISP_VIDOUT_DIS2M_PANNING);	// 8Mcrop-->2M
		else if(GetIN_MainSrcSizeH() == 2048)		SetOUT_AuxFrmt(MISP_VIDOUT_DIS3M);	// 3M
		else									SetOUT_AuxFrmt(MISP_VIDOUT_DIS2M);	// 2M
		
		SetFC_AuxFCdstPosiX(0); 	SetFC_AuxFCdstSizeH(GetOUT_AuxSizeHA());
		SetFC_AuxFCdstPosiY(0); 	SetFC_AuxFCdstSizeV(GetOUT_AuxSizeVA());
		
//		UARTprintf("aux (%d, %d) -> (%d, %d)\n", 
//						GetIN_AuxSrcSizeH(), GetIN_AuxSrcSizeV(), GetOUT_AuxSizeHA(), GetOUT_AuxSizeVA());
		
		SetPIPSource(MainSrc); 					// set aux source to main source. 0:sensor, 1:ext(hdmi)
		
		//----------------- DIS MEMORY MAP Change ------------------------------
		// (must be 4 frame mode.)
#if defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX334__)
	#if defined(__USE_SENS_PANNING_MODE__)
		SetDDRMAP_DIS_mode(1) ;		// 0 :off / 1:4f / 2:3f 
		SetIN_MainFRcntY(4);	SetIN_MainFRdlyY(2);	// 4 frame + 2 delay
		SetIN_MainFRcntC(4);	SetIN_MainFRdlyC(2);	// 4 frame + 2 delay
		SetIN_AuxFRcnt(4);		SetIN_AuxFRdly(2);		// 4 frame + 2 delay
	#else
		SetDDRMAP_DIS_mode(2) ;		// 0 :off / 1:4f / 2:3f 
		SetIN_MainFRcntY(3);	SetIN_MainFRdlyY(1);	// 3 frame + 1 delay
		SetIN_MainFRcntC(3);	SetIN_MainFRdlyC(1);	// 3 frame + 1 delay
		SetIN_AuxFRcnt(3);		SetIN_AuxFRdly(1);		// 3 frame + 1 delay
	#endif
#else
		SetDDRMAP_DIS_mode(1) ;		// 0 :off / 1:4f / 2:3f 
		SetIN_MainFRcntY(4);	SetIN_MainFRdlyY(2);	// 4 frame + 2 delay
		SetIN_MainFRcntC(4);	SetIN_MainFRdlyC(2);	// 4 frame + 2 delay
		SetIN_AuxFRcnt(4);		SetIN_AuxFRdly(2);		// 4 frame + 2 delay
#endif
		
		
		MISP_SW_Reset();			// s/w rst to fix dis hold 'wait' when repeat dis on/off
			
		// DIS ON
		SetDIS_OnOff(nID);			// remote control DIS enalble !!
	}
	else {
//#if defined(__USE_MDIN_i500__)		// MDIN-i500		// delete by kw5210 20170306
//		SetNR_Mode(MISP_NR_2D3D);	SetMenuStatus(DNR_MODE_MENU, MISP_NR_2D3D);	// NR(3D-on)
//#endif
		SetNR_Mode(MISP_NR_2D3D);		SetMenuStatus(DNR_MODE_MENU, MISP_NR_2D3D);	// NR(on)
			
		SetOUT_MainDarkScreenAuto(30);	// main display turn-on delay = 16ms x 30 = 480ms


	#if defined( __USE_SENS_PANNING_MODE__) 
		// no overscan
			DIS_OVERSCAN=OFF;		
	#elif defined(__MN34420_1280x1024_ISPCROP__)
		// no overscan
			DIS_OVERSCAN=OFF;		
	#else	
		if(DIS_OVERSCAN==ON){
			DIS_OVERSCAN=OFF;
			Demo_DIS_InitOverScan(OFF);
			UARTprintf("From now on, disable overscan for dis \r\n");		
		}
	#endif
	
		// default main y/c frame count changed(2 -> 3) to fix menu osd broken when change 'c' frame(3fr -> 2fr)
		SetIN_MainFRcntY(3);	SetIN_MainFRdlyY(0);	// 3 frame + 0 delay
		SetIN_MainFRcntC(3);	SetIN_MainFRdlyC(0);	// 3 frame + 0 delay
		SetIN_AuxFRcnt(3);		SetIN_AuxFRdly(0);		// 3 frame + 0 delay
		
		SetDDRMAP_DIS_mode(0) ; // 0 :off / 1:4f / 2:3f
		MISP_SW_Reset();		// s/w rst to fix menu osd broken when dis off(aux 4fr -> 2fr) -- 2Msen to 4Kout
	
		SetDIS_OnOff(OFF);
//		SetDIS_wait(0);
		DIS_OVERSCAN=OFF;
		
		// AUX
//		SetOUT_AuxFreezeEN(OFF);
//		SetOUT_AuxDispEN(ON);
//		SetOUT_AuxUsedByFunc(OFF);
//		SetOUT_CVBSOutEN();
	}
}
//--------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
void DEMO_SetWBAutoBlue(WORD nID)
{
	LONG mode = stWBCTRL.ctrl&WBCTRL_MODE_MASK;
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	//stWBCTRL.offB  = 50-nID;
	stWBCTRL.offB  = nID-50;

	stWBCTRL.ctrl &= (~WBCTRL_MODE_MASK);
	stWBCTRL.ctrl |= (mode|WBCTRL_TRIGGER_ISP);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_WBAUTO_BLUE, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//---------------------------------------------------------------------------------------------
void DEMO_SetWBAutoRed(WORD nID)
{
	LONG mode = stWBCTRL.ctrl&WBCTRL_MODE_MASK;
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif

	//stWBCTRL.offR = 50-nID;
	stWBCTRL.offR = nID-50;

	stWBCTRL.ctrl &= (~WBCTRL_MODE_MASK);
	stWBCTRL.ctrl |= (mode|WBCTRL_TRIGGER_ISP);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_WBAUTO_RED, (PBYTE)&buff[0], sizeof(buff));
#endif
}


//---------------------------------------------------------------------------------------------
void DEMO_SetWBManBlue(WORD nID)
{
	LONG mode = stWBCTRL.ctrl&WBCTRL_MODE_MASK;
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	stWBCTRL.stMANUAL.B  = 106 + 3*nID;

	stWBCTRL.ctrl &= (~WBCTRL_MODE_MASK);
	stWBCTRL.ctrl |= (mode|WBCTRL_TRIGGER_ISP);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_WBMAN_BLUE, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//---------------------------------------------------------------------------------------------
void DEMO_SetWBManRed(WORD nID)
{
	LONG mode = stWBCTRL.ctrl&WBCTRL_MODE_MASK;
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif

	stWBCTRL.stMANUAL.R = 106 + 3*nID;

	stWBCTRL.ctrl &= (~WBCTRL_MODE_MASK);
	stWBCTRL.ctrl |= (mode|WBCTRL_TRIGGER_ISP);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_WBMAN_RED, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//---------------------------------------------------------------------------------------------
void DEMO_SetWBSpeed(WORD nID)
{
	LONG speed;
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	switch(nID)	{
		case 0:	speed = WBCTRL_SPEED_VAR0;	break;
		case 1:	speed = WBCTRL_SPEED_VAR1;	break;
		case 2:	speed = WBCTRL_SPEED_VAR2;	break;
		case 3:	speed = WBCTRL_SPEED_VAR3;	break;
		case 4:	speed = WBCTRL_SPEED_VAR4;	break;
		case 5:	speed = WBCTRL_SPEED_VAR5;	break;
		case 6:	speed = WBCTRL_SPEED_VAR6;	break;
		case 7:	speed = WBCTRL_SPEED_VAR7;	break;
		default: speed = WBCTRL_SPEED_VAR6;	break;
	}
	
	WB_SetAWBDrvSPEED(&stWBCTRL, speed);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_WB_SPEED, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//---------------------------------------------------------------------------------------------
void DEMO_SetMenuPosition(WORD nID)	
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	WORD Hact=GetOUT_MainSizeHA(), Vact=GetOUT_MainSizeVA();
	WORD MenuW=OSD_FONT_W*28, MenuH=OSD_FONT_H*14;
	WORD offx=40, offy=80;
	WORD MenuPos[5][2];
	
#if OSD_REPEAT_EN
	MenuW *= (MenuW*2 <= Hact)?  2:  1;		//MenuW *= (MenuW*2 <= Hact)?  2:  1;
	MenuH *= (MenuH*2 <= Vact)?  2:  1;		//MenuH *= (MenuH*2 <= Vact)?  2:  1;
#endif
	
	MenuPos[0][0]=(Hact-MenuW)/2;		MenuPos[0][1]=140;					// default
	MenuPos[1][0]=offx;					MenuPos[1][1]=offy;					// Top-Left
	MenuPos[2][0]=(Hact-MenuW)-offx;	MenuPos[2][1]=offy;					// Top-Right
	MenuPos[3][0]=(Hact-MenuW)-offx;	MenuPos[3][1]=(Vact-MenuH)-offy;	// Bottom-Right
	MenuPos[4][0]=offx;					MenuPos[4][1]=(Vact-MenuH)-offy;	// Bottom-Left
	
	stOSD[SPRITE_INDEX0].x = MenuPos[nID][0];
	stOSD[SPRITE_INDEX0].y = MenuPos[nID][1];
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX0], SPRITE_INDEX0);
	
#if OSD_AUX_EN
	Hact=GetOUT_AuxSizeHA();	Vact=GetOUT_AuxSizeVA();
	offx=100;					offy=50;
	
	MenuW=OSD_FONT_W*28; 	MenuH=OSD_FONT_H*14;
	MenuW *= (MenuW*2 <= Hact)?  2:  1;		//MenuW *= (MenuW*2 <= Hact)?  2:  1;
	MenuH *= (MenuH*2 <= Vact)?  2:  1;		//MenuH *= (MenuH*2 <= Vact)?  2:  1;
	
	MenuPos[0][0]=(Hact-MenuW)/2;		MenuPos[0][1]=100;					// default
	MenuPos[1][0]=offx;					MenuPos[1][1]=offy;					// Top-Left
	MenuPos[2][0]=(Hact-MenuW)-offx;	MenuPos[2][1]=offy;					// Top-Right
	MenuPos[3][0]=(Hact-MenuW)-offx;	MenuPos[3][1]=(Vact-MenuH)-offy;	// Bottom-Right
	MenuPos[4][0]=offx;					MenuPos[4][1]=(Vact-MenuH)-offy;	// Bottom-Left
	
	stOSD[SPRITE_INDEX8].x = MenuPos[nID][0];
	stOSD[SPRITE_INDEX8].y = MenuPos[nID][1];
	stOSD[SPRITE_INDEX8].x /= ((GetOUT_AuxMode()==MISP_OUT_MUX656_8) || (GetOUT_AuxMode()==MISP_OUT_MUX656_10))?  2:  1;	// aux sprite position(mux656 : *2)
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX8], SPRITE_INDEX8);
#endif
	
#if defined(__USE_4K60_INOUT__)
	stOSD[SPRITE_INDEX5].x = (Hact-MenuW)/2;
	stOSD[SPRITE_INDEX5].y = Vact/3;
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX5], SPRITE_INDEX5);
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_SETUP_MPOSI, (PBYTE)&buff[0], sizeof(buff));
#endif
	
#if __MISP100_DBGPRT__ == 1
	UARTprintf("DEMO_SetMenuPosition : (%d,%d) \n", stOSD[SPRITE_INDEX0].x, stOSD[SPRITE_INDEX0].y );
#endif
}

//---------------------------------------------------------------------------------------------
WORD DEMO_GetMenuPosition(void)
{
	WORD Hact=GetOUT_MainSizeHA(), Vact=GetOUT_MainSizeVA();
	WORD MenuW=OSD_FONT_W*28, MenuH=OSD_FONT_H*14;
	WORD offx=40, offy=80, i;
	WORD MenuPos[5][2];
	
#if OSD_REPEAT_EN
	MenuW *= (MenuW*2 <= Hact)?  2:  1;		//MenuW *= (MenuW*2 <= Hact)?  2:  1;
	MenuH *= (MenuH*2 <= Vact)?  2:  1;		//MenuH *= (MenuH*2 <= Vact)?  2:  1;
#endif
	
	MenuPos[0][0]=(Hact-MenuW)/2;		MenuPos[0][1]=140;					// default
	MenuPos[1][0]=offx;					MenuPos[1][1]=offy;					// Top-Left
	MenuPos[2][0]=(Hact-MenuW)-offx;	MenuPos[2][1]=offy;					// Top-Right
	MenuPos[3][0]=(Hact-MenuW)-offx;	MenuPos[3][1]=(Vact-MenuH)-offy;	// Bottom-Right
	MenuPos[4][0]=offx;					MenuPos[4][1]=(Vact-MenuH)-offy;	// Bottom-Left
	
	for(i=0;i<5;i++)	{
		if((stOSD[SPRITE_INDEX0].x == MenuPos[i][0]) && (stOSD[SPRITE_INDEX0].y == MenuPos[i][1]))	{
			return  i;
		}
	}
	
	return  0;
}

//---------------------------------------------------------------------------------------------
void DEMO_SetSensorFps(BYTE nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif

#if   defined(__USE_SENS_240Hz_MENU__)	
	ROMDATA MISP_FPS_t fps_set[]={MISP_FPS_25Hz, MISP_FPS_30Hz, MISP_FPS_50Hz, MISP_FPS_60Hz,MISP_FPS_100Hz,MISP_FPS_120Hz,MISP_FPS_200Hz,MISP_FPS_240Hz};
#elif   defined(__USE_SENS_120Hz_MENU__)	
	ROMDATA MISP_FPS_t fps_set[]={MISP_FPS_25Hz, MISP_FPS_30Hz, MISP_FPS_50Hz, MISP_FPS_60Hz,MISP_FPS_100Hz,MISP_FPS_120Hz};
#elif   defined(__USE_SENS_60Hz_MENU__)
	ROMDATA MISP_FPS_t fps_set[]={MISP_FPS_25Hz, MISP_FPS_30Hz, MISP_FPS_50Hz, MISP_FPS_60Hz};
#elif   defined(__USE_SENS_50Hz_MENU__)
	ROMDATA MISP_FPS_t fps_set[]={MISP_FPS_25Hz, MISP_FPS_30Hz, MISP_FPS_50Hz};
#elif defined(__USE_SENS_20Hz_MENU__)
	ROMDATA MISP_FPS_t fps_set[]={MISP_FPS_20Hz, MISP_FPS_24Hz};
#else
	ROMDATA MISP_FPS_t fps_set[]={MISP_FPS_25Hz, MISP_FPS_30Hz};
#endif

	MISP_FPS_t target;
	BYTE maxID;
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveSensorFps(nID);
#endif
	
	//set target fps
	maxID=sizeof(fps_set)/sizeof(MISP_FPS_t)-1;
	if(nID>maxID) {
		UARTprintf("DEMO_SetSensorFps : bad param. %d (max. %d)\r\n", nID, maxID);
		return;
	}
	target=fps_set[nID];

	//set sensor format
	switch(target) {
		case MISP_FPS_25Hz:   //set to 25Hz
			switch(GetSS_image_active_size_v()) {
				case  720:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1280x720p25 );  break;
				case 1080:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x1080p25);  break;
				case 1536:  SetSS_SensorVoutFrmt(MISP_VIDSRC_2048x1536p25);  break;
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
				case 2160:  SetSS_SensorVoutFrmt(MISP_VIDSRC_SS3840x2160p25);  break;
#endif
				default:    UARTprintf("DEMO_SetSensorFps : N/A 25Hz\r\n");  break;
			}
			break;
		case MISP_FPS_30Hz:   //set to 30Hz
			switch(GetSS_image_active_size_v()) {
				case  720:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1280x720p30 );  break;
				case 1080:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x1080p30);  break;
				case 1536:  SetSS_SensorVoutFrmt(MISP_VIDSRC_2048x1536p30);  break;
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
				case 2160:  SetSS_SensorVoutFrmt(MISP_VIDSRC_3840x2160p30);  break;
#endif
				default:    UARTprintf("DEMO_SetSensorFps : N/A 30Hz\r\n");  break;
			}
			break;
		case MISP_FPS_50Hz:   //set to 50Hz
			switch(GetSS_image_active_size_v()) {
				case  720:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1280x720p50 );  break;
				case 1080:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x1080p50);  break;
				case 1536:  SetSS_SensorVoutFrmt(MISP_VIDSRC_2048x1536p50);  break;				
#if defined(__MDIN_i540_4K60_V10__)
				case 2160:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x2160p50);  break;				
#endif
				default:    UARTprintf("DEMO_SetSensorFps : N/A 50Hz\r\n");  break;
			}
			break;
		case MISP_FPS_60Hz:   //set to 60Hz
			switch(GetSS_image_active_size_v()) {
				case  720:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1280x720p60 );  break;
				case 1080:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x1080p60);  break;
#if defined(__MDIN_i540_4K60_V10__)				
				case 2160:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x2160p60);  break;							
#endif
				default:    UARTprintf("DEMO_SetSensorFps : N/A 60Hz\r\n");  break;
			}
			break;
		case MISP_FPS_20Hz:   //set to 20Hz
			switch(GetSS_image_active_size_v()) {
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
				case 2160:  SetSS_SensorVoutFrmt(MISP_VIDSRC_4096x2160p20);  break;
#endif
				default:    UARTprintf("DEMO_SetSensorFps : N/A 10Hz\r\n");  break;
			}
			break;
		case MISP_FPS_24Hz:   //set to 24Hz
			switch(GetSS_image_active_size_v()) {
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
				case 2160:  SetSS_SensorVoutFrmt(MISP_VIDSRC_4096x2160p24);  break;
#endif
				default:    UARTprintf("DEMO_SetSensorFps : N/A 12Hz\r\n");  break;
			}
			break;
		case MISP_FPS_100Hz:   //set to 100Hz
			switch(GetSS_image_active_size_v()) {		
				case  720:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1280x720p100 );  break;
//				case 1080:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x1080p100);  break;
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
				case 1080:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x1080p100);  break;
#endif
				default:    UARTprintf("DEMO_SetSensorFps : N/A 100Hz\r\n");  break;
			}
			break;
		case MISP_FPS_120Hz:   //set to 120Hz
			switch(GetSS_image_active_size_v()) {		
				case  720:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1280x720p120 );  break;
//				case 1080:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x1080p120);  break;
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
				case 1080:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x1080p120);  break;
#endif				
				default:    UARTprintf("DEMO_SetSensorFps : N/A 120Hz\r\n");  break;
			}
			break;		
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550			
		case MISP_FPS_200Hz:   //set to 200Hz
			switch(GetSS_image_active_size_v()) {		
				case  720:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1280x720p200 );  break;
				default:    UARTprintf("DEMO_SetSensorFps : N/A 200Hz\r\n");  break;		
			}
			break;
		case MISP_FPS_240Hz:   //set to 240Hz		
			switch(GetSS_image_active_size_v()) {		
				case  720:  SetSS_SensorVoutFrmt(MISP_VIDSRC_1280x720p240 );  break;
				default:    UARTprintf("DEMO_SetSensorFps : N/A 240Hz\r\n");  break;		
			}
#endif		
		default:
			break;
	}

	//set sensor configurations
	if(GetWDR_ProcesMode()) {
		//reset WDR also to protect split FID
		//WDR_ProcessHandler() includes sensor control
		SetWDR_ProcessRst();
	}
	else {
		//reset sensor to adjust target fps
		SetSS_ResetSensor(DEMO_GetSensorFps());

		//setting clk : special case
	#if defined(__USE_ISP_SCLK_OUT__) 
		#if defined(__USE_IMX290_1080P120_MODE__) || defined(__USE_IMX290_720P120_ISPCROP__)
			//720p120 with user clock out : 1080p-->720p isp crop		
			switch(target){
			case MISP_FPS_25Hz: 		case MISP_FPS_30Hz: 
			case MISP_FPS_50Hz:		case MISP_FPS_60Hz: 
				MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x1021);	// user pll clk div 4
				SetCLK_UserPLL(1,3,66,2,1) ;		// 27M(user pll source = xtal, 148.5Mhz = 27*66/3/(2^2) ) -> 148.5 / 4 -> 37.125Mhz		
				SetSS_sensor_bayer_10bit(0);
				SetSS_data_lbit_shift(0);
			break;
			
			case MISP_FPS_100Hz: 	case MISP_FPS_120Hz: 		
				MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x2021);	// user pll clk div 8 
				SetCLK_UserPLL(1,3,132,2,1) ;		// 27M(user pll source = xtal, 297Mhz = 27*132/3/(2^2) ) -> 297 / 8 -> 37.125Mhz
				SetSS_sensor_bayer_10bit(1);
				SetSS_data_lbit_shift(2);		
			break;

			default:			break;		
			}
		#endif
	#else	//no use __USE_ISP_SCLK_OUT__
		#if defined(__USE_IMX290_1080P120_MODE__) || defined(__USE_IMX290_720P120_MODE__)
		
			switch(target){
			case MISP_FPS_25Hz: 		case MISP_FPS_30Hz: 
			case MISP_FPS_50Hz:		case MISP_FPS_60Hz: 
				SetSS_sensor_bayer_10bit(0);
				SetSS_data_lbit_shift(0);
			#if defined(__USE_IMX290_720P120_MODE__)
				SetCLK_SrcSensor(3) ;		
			#endif				
			break;
			
			case MISP_FPS_100Hz: 	case MISP_FPS_120Hz: 		
				SetSS_sensor_bayer_10bit(1);
				SetSS_data_lbit_shift(2);		
			#if defined(__USE_IMX290_720P120_MODE__)
				SetCLK_SrcSensor(2) ;		
			#endif								
			break;

			default:			break;		
			}		
		#elif defined(__USE_IMX273_720P240_MODE__)
			switch(target){
			case MISP_FPS_25Hz: 		case MISP_FPS_30Hz: 
			case MISP_FPS_50Hz:		case MISP_FPS_60Hz: 
			case MISP_FPS_100Hz: 	case MISP_FPS_120Hz: 					
				SetSS_sensor_bayer_10bit(0);
				SetSS_data_lbit_shift(0);	

				SetCLK_SrcSensor(3) ;					
			break;
			
			case MISP_FPS_200Hz: 	case MISP_FPS_240Hz: 		
				SetSS_sensor_bayer_10bit(1);
				SetSS_data_lbit_shift(2);	

				SetCLK_SrcSensor(2) ;					
			break;

			default:			break;				
			}
		#endif
	#endif
	}
	
	if((target==MISP_FPS_50Hz) || (target==MISP_FPS_60Hz) || (target==MISP_FPS_100Hz) || (target==MISP_FPS_120Hz) || (target==MISP_FPS_200Hz) || (target==MISP_FPS_240Hz))	{
		if(GetROT_RotationEnable())	{
			Demo_SetRotation(OFF);	SetMenuStatus(DISP_ROT_MENU, OFF);	// rotation 'off'
		}
	}

#ifndef __VTOTAL_CTRL_PAL__
	//set manual shutter speed
	//if sensor fps changes, exposure line must be re-calculated
	if(GetWDR_ProcesMode()==0) {
		if((stAECTRL.ctrl&AECTRL_SHUTCTL_MASK)==AECTRL_SHUTCTL_MANU)
			SetAEShutSpeed(GetMenuStatus(EXP_SHUT_MENU));
	}
#endif

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = nID;
	EEPROM_MultiWrite(EEPROM_SS_FPS, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//---------------------------------------------------------------------------------------------
BYTE DEMO_GetSensorFps(void)
{

	switch(GetSS_SensorVoutFps())	{
		case  25: 		return  MISP_FPS_25Hz;
		case  30: 		return  MISP_FPS_30Hz;
		case  50: 		return  MISP_FPS_50Hz;
		case  60: 		return  MISP_FPS_60Hz;
		case  20: 		return  MISP_FPS_20Hz;
		case  24: 		return  MISP_FPS_24Hz;
		case  100:		return  MISP_FPS_100Hz;
		case  120:		return  MISP_FPS_120Hz;
		case  200:		return  MISP_FPS_200Hz;
		case  240:		return  MISP_FPS_240Hz;		
		default: 		return  MISP_FPS_30Hz;
	}
}

//---------------------------------------------------------------------------------------------
void DEMO_SetSensorFpsToggle(void)
{
	switch(GetSS_image_active_size_v()) {
		case  720:
			switch(GetSS_SensorVoutFps())	{
				case  25: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1280x720p30);	break;
				case  30: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1280x720p25);	break;
				case  50: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1280x720p60);	break;
				case  60: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1280x720p50);	break;
				case  100: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1280x720p100);	break;
				case  120: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1280x720p120);	break;			
				case  200: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1280x720p200);	break;
				case  240: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1280x720p240);	break;					
				default: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1280x720p30);	break;
			}
			break;
			
		case 1080:
			switch(GetSS_SensorVoutFps())	{
				case  25: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x1080p30);	break;
				case  30: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x1080p25);	break;
				case  50: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x1080p60);	break;
				case  60: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x1080p50);	break;
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550				
				case  100: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x1080p60);	break;
				case  120: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x1080p50);	break;				
#endif
				default: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x1080p30);	break;
			}
			break;
			
		case 1536:
			switch(GetSS_SensorVoutFps())	{
				case  25: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_2048x1536p30);	break;
				case  30: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_2048x1536p25);	break;
				default: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_2048x1536p30);	break;
			}
			break;
			
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
		case 2160:
			switch(GetSS_SensorVoutFps())	{
				case  25: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_3840x2160p30);	break;
				case  30: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_3840x2160p25);	break;
#if defined(__USE_4K60_INOUT__)
				case  50: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x2160p60);	break;
				case  60: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_1920x2160p50);	break;
#endif
				default: 	SetSS_SensorVoutFrmt(MISP_VIDSRC_3840x2160p30);	break;
			}
			break;
#endif
	}
	
	//set sensor configurations
	if(GetWDR_ProcesMode()) {
		//reset WDR also to protect split FID
		//WDR_ProcessHandler() includes sensor control
		SetWDR_ProcessRst();
	}
	else {
		//reset sensor to adjust target fps
		SetSS_ResetSensor(DEMO_GetSensorFps());
	}

#ifndef __VTOTAL_CTRL_PAL__
	//set manual shutter speed
	//if sensor fps changes, exposure line must be re-calculated
	if(GetWDR_ProcesMode()==0) {
		if((stAECTRL.ctrl&AECTRL_SHUTCTL_MASK)==AECTRL_SHUTCTL_MANU)
			SetAEShutSpeed(GetMenuStatus(EXP_SHUT_MENU));
	}
#endif
}

//-----------------------------Live DPC   when dpc on - some bug is exsist ---------------------------------
void DEMO_SetLiveDPCMode(WORD nID)		  
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	SetSDPC_OnOff(nID);

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	if(nID==1)	DEMO_SetAutoDynamicDPCEn(ON);
	else if(nID==3)	DEMO_SetAutoDynamicDPCEn(ON);	
	else			DEMO_SetAutoDynamicDPCEn(OFF);
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_SPECIAL_DPC, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//-----------------------------------------------------------------------------------------
void DEMO_StatDPCRun(WORD nID)		
{
#if !defined(__USE_4K60_MD2__)
	// AE Stop
	saved_stAECTRL=stAECTRL.ctrl;
	SetAECtrlOnOff(OFF);

	//IRIS Close
	#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
	DCLENS_ForceSetIRISGain(0);		//AN41908 iris full close
	UARTprintf("[SDPD]iris force close\r\n");	
	#elif defined(__USE_PWMIRIS_ONLY__)
	AECTRL_IRISPWM_SetFullClose();		//DC(pwm) iris full close
	#endif		

	//AE faster shutter setting
	if(GetSDPD_Shutter_level()==2){
		#if defined(__USE_CMOS_IMX226__)
		AECTRL_SetSHUTCtrl(&stAECTRL, (SHORT)2196, AECTRL_SHUTCTL_MANU);			//imx226 datasheet official shutter limit	
		#else
		AECTRL_SetSHUTCtrl(&stAECTRL, (SHORT)1123, AECTRL_SHUTCTL_MANU);			
		#endif
	}
	else if(GetSDPD_Shutter_level()==1)	AECTRL_SetSHUTCtrl(&stAECTRL, (SHORT)700, AECTRL_SHUTCTL_MANU);			
	else 								AECTRL_SetSHUTCtrl(&stAECTRL, (SHORT)12, AECTRL_SHUTCTL_MANU);	
		
	UARTprintf("[SDPD]AE Shutter level : %d \r\n",GetSDPD_Shutter_level());	
#endif


#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveSDPCRun(nID);
#endif


	SetSDPD_AE_BackFlag(OFF);
	SetSDPD_AGC_Level(SDPD_AGC_START);
	SetSDPD_thhd(SDPD_AGC_START);		//set dpc thhd level

#if !defined(__USE_4K60_MD2__)	
	//AE gain setting
		switch (SDPD_AGC_START) {
			case 0:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV0, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[SDPD]LV0 agc_target %d\r\n", SDPD_AGC_LV0);	break;
			case 1:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV1, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[SDPD]LV1 agc_target %d\r\n", SDPD_AGC_LV1);	break;
			case 2:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV2, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[SDPD]LV2 agc_target %d\r\n", SDPD_AGC_LV2);	break;
			case 3:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV3, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[SDPD]LV3 agc_target %d\r\n", SDPD_AGC_LV3);	break;
			case 4:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV4, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[SDPD]LV3 agc_target %d\r\n", SDPD_AGC_LV4);	break;			
			case 5:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV5, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[SDPD]LV3 agc_target %d\r\n", SDPD_AGC_LV5);	break;						
			case 6:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV6, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[SDPD]LV3 agc_target %d\r\n", SDPD_AGC_LV6);	break;		
			case 7:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV7, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[SDPD]LV3 agc_target %d\r\n", SDPD_AGC_LV7);	break;		
			default:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV0, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[SDPD]Def agc_target %d\r\n", SDPD_AGC_LV0);	break;
		}	
#endif

	DPD_RUN();

}

void DEMO_SetAutoDynamicDPCEn(BOOL OnOff)
{
	SetDynamicDPC_Auto(OnOff);

	if(OnOff==OFF){
		p_target_ddpc=0;
		target_ddpc=p_target_ddpc;
		DynamicDPC_Process(target_ddpc);
	}
	else				DynamicDPC_Process(target_ddpc);
		
}
//------------------------------------------------------------------------------------------
void SDPC_Return(void)
{
	

	if(GetSDPD_AE_BackFlag()){
		//return sequence
		UARTprintf("[SDPD]AE parameter return. serching dp end\r\n\r\n");		
		
		stAECTRL.ctrl = saved_stAECTRL;

	#if !defined(__USE_4K60_MD2__)		
		#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
		DCLENS_SetIRISGain(IRIS_DEFAULT);
		#elif defined(__USE_PWMIRIS_ONLY__)
		DCLENS_IRISAutoMode(1,&stAECTRL);
		#endif

		
		SetAEShutSpeed(stAEMENU.shut[stAEMENU.mode]);
		SetAEAGCGain(stAEMENU.agc[stAEMENU.mode]);
		
		SetMenuStatus(SDPC_RUN_MENU, 0);	
		SetOSDMenuID(SDPC_RUN_MENU);

		OSD_MessageBox(ON, "DP DETECT PROCESS END  ");		
	#endif

	}
	else{
		UARTprintf("[SDPD]AGC parameter change\r\n");	

	#if !defined(__USE_4K60_MD2__)
		//AE gain setting
		switch (GetSDPD_AGC_Level()) {
			case 0:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV0, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[DPD]agc_target %d\r\n", SDPD_AGC_LV0);	break;
			case 1:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV1, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[DPD]agc_target %d\r\n", SDPD_AGC_LV1);	break;
			case 2:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV2, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[DPD]agc_target %d\r\n", SDPD_AGC_LV2);	break;
			case 3:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV3, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[DPD]agc_target %d\r\n", SDPD_AGC_LV3);	break;
			case 4:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV4, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[DPD]agc_target %d\r\n", SDPD_AGC_LV4);	break;
			case 5:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV5, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[DPD]agc_target %d\r\n", SDPD_AGC_LV5);	break;			
			case 6:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV6, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[DPD]agc_target %d\r\n", SDPD_AGC_LV6);	break;			
			case 7:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV7, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[DPD]agc_target %d\r\n", SDPD_AGC_LV7);	break;			
			default:	AECTRL_SetGAINCtrl(&stAECTRL, (LONG) SDPD_AGC_LV0, AECTRL_GAINCTL_MANU, 0);			UARTprintf("[DPD]agc_target %d\r\n", SDPD_AGC_LV0);	break;
		}	
	#endif

		SetSDPD_thhd(GetSDPD_AGC_Level());		//set dpc thhd level		

		DPD_RUN();
	}
		


}

//-----------------------------Live DPC, LSC is same registor controll---------------------------------------
void DEMO_SetDWDRMode(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_MDIN_i500__)		// MDIN-i500
#if defined(__M33_V101__)
	SetBY_DWDR_mode(nID);
#else
	SetBY_DWDR_gamma(GetBY_dwdr_mode(),GetMenuStatus(GM_MENU));	
	SetBY_DWDR_AEtarget(GetBY_dwdr_mode(), GetMenuStatus(EXP_BRIGHT_MENU));	

	//dwdr mode setting
	SetBY_DWDR_mode(nID);
	//set AE setting
	SetAEBrightLvl(GetBY_DWDR_AEtarget(nID));
	SetMenuStatus(EXP_BRIGHT_MENU, GetBY_DWDR_AEtarget(nID));
	//gamma setting
	SetBY_GammaValue(GetBY_DWDR_gamma(nID));		
	SetMenuStatus(GM_MENU, GetBY_DWDR_gamma(nID));
#endif

#else       						// MDIN-i51X (i510, i540, i550)
	SetBY_DWDR_mode(nID) ;
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_SPECIAL_DWDR, (PBYTE)&buff[0], sizeof(buff));
#endif
}

/*
//-----------------------------------------------------------------------------------------
void DEMO_SetExpShutter(WORD nID)	//if long shutter mode is add this function should be changed
{
	LONG shut, mode = stAECTRL.ctrl&AECTRL_SHUT_MASK;

	//need shut variable mapping for sensor(same defCISShutStep)
		switch (nID) {
			case 0:	shut =    	defCISShutStep[CIS_FPS][nID];	mode = AECTRL_SHUTCTL_AUTO;	break;
			case 1:	shut =    	defCISShutStep[CIS_FPS][nID];	mode = AECTRL_SHUTCTL_1_30;		break;
			case 2:	shut =  	defCISShutStep[CIS_FPS][nID];	mode = AECTRL_SHUTCTL_1_50;		break;
			case 3:	shut =  	defCISShutStep[CIS_FPS][nID];	mode = AECTRL_SHUTCTL_1_60;		break;
			case 4:	shut =  	defCISShutStep[CIS_FPS][nID];	mode = AECTRL_SHUTCTL_1_100;	break;
			case 5:	shut =  	defCISShutStep[CIS_FPS][nID];	mode = AECTRL_SHUTCTL_1_120;	break;
			case 6:	shut =  	defCISShutStep[CIS_FPS][nID];	mode = AECTRL_SHUTCTL_1_250;	break;
			case 7:	shut = 	defCISShutStep[CIS_FPS][nID];	mode = AECTRL_SHUTCTL_1_500;	break;
			case 8:	shut = 	defCISShutStep[CIS_FPS][nID];	mode = AECTRL_SHUTCTL_1_1000;	break;
			case 9:	shut = 	defCISShutStep[CIS_FPS][nID];	mode = AECTRL_SHUTCTL_1_2000;	break;
			case 10:	shut = 	defCISShutStep[CIS_FPS][nID];	mode = AECTRL_SHUTCTL_1_5000;	break;
			case 11:	shut = 	defCISShutStep[CIS_FPS][nID];	mode = AECTRL_SHUTCTL_1_10000;	break;
			default:	shut =    0;								mode = AECTRL_SHUTCTL_AUTO;	break;
		}
		
	#if defined(__USE_CMOS_IMXx36__) || defined(__USE_CMOS_MN34041__) ||  defined(__USE_CMOS_MN34220__) || defined (__USE_CMOS_IMX124__)		
		if (mode==AECTRL_SHUTCTL_AUTO)	 AECTRL_SetSHUTCtrl(&stAECTRL, (SHORT)shut, AECTRL_SHUTCTL_AUTO);
//		else AECTRL_SetSHUTCtrl(&stAECTRL, (SHORT)shut, AECTRL_SHUTCTL_MANU);
		else {	AECTRL_SetSHUTCtrl(&stAECTRL, (SHORT)shut, AECTRL_SHUTCTL_MANU);	
				CMOS_SetShutter(shut);
		}
		stAECTRL.ctrl &= (~AECTRL_SHUT_MASK);
		stAECTRL.ctrl |= (mode);
		
	#elif defined(__USE_CMOS_PO3100K__) 
		if (mode==AECTRL_SHUTCTL_AUTO){		//use po3100k internal isp ae logic
				CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
				CMOS_ByteWrite(I2C_PO3100K_ADDR,0x04,0x9C);	//auto controll 1		
		}
		else{
			CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
			CMOS_ByteWrite(I2C_PO3100K_ADDR,0x04,0x9F);	//auto controll 1	

			CMOS_SetShutter(shut);
		}	
	#else
		CMOS_SetShutter(shut);
	#endif
}
*/
//--------------------------------------------------------------------------------------------------
void DEMO_SetExpAGC(WORD nID)	  
{

	LONG max, mode;
	
	if		(nID==0)	mode = AECTRL_GAINCTL_OFF;
	else if (nID==1)		mode = AECTRL_GAINCTL_MIN;
	else if (nID==2)		mode = AECTRL_GAINCTL_MID;
	else	 if (nID==3)		mode = AECTRL_GAINCTL_MAX;
	else	 if (nID==4)		mode = AECTRL_GAINCTL_DMIN;
	else	 if (nID==5)		mode = AECTRL_GAINCTL_DMID;
	else	 if (nID==6)		mode = AECTRL_GAINCTL_DMAX;
	else		mode = AECTRL_GAINCTL_MAX;

	stAECTRL.ctrl &= (~AECTRL_GAIN_MASK);
	stAECTRL.ctrl |= (mode);

	switch (mode) {
		case AECTRL_GAINCTL_OFF: max = AGCMax[0]; break;
		case AECTRL_GAINCTL_MIN: max = AGCMax[1]; break;
		case AECTRL_GAINCTL_MID: max = AGCMax[2]; break;
		case AECTRL_GAINCTL_MAX: max = AGCMax[3]; break;
		case AECTRL_GAINCTL_DMIN: max = AGCMax[3]+AGCMax[3]*2/8; break;
		case AECTRL_GAINCTL_DMID: max = AGCMax[3]+AGCMax[3]*5/8; break;
		case AECTRL_GAINCTL_DMAX: max = AGCMax[3]+AGCMax[3]*8/8; break;
		default:				max = AGCMax[3]; break;
	}
				
	#if defined (__USE_CMOS_AR0331__)
		switch(nID)	{
			case  1:	CMOS_SetGain(121);	break;
			case  2:	CMOS_SetGain(242);	break;
			case  3:	CMOS_SetGain(2000);	break;
			case  0:	CMOS_SetGain(0);	break;
			default:	CMOS_SetGain(0);	break;	   //HIGH
		}
	#elif defined (__USE_CMOS_MT9P031__)
		switch(nID)	{
			case  1:	CMOS_SetGain(23);	break;
			case  2:	CMOS_SetGain(39);	break;
			case  3:	CMOS_SetGain(159);	break;
			case  0:	CMOS_SetGain(0);	break;
			default:	CMOS_SetGain(0);	break;	   //HIGH
		}					
	#elif defined (__USE_CMOS_PO3100K__)
		switch(nID)	{
			case  1:	CMOS_SetGain(24);	break;
			case  2:	CMOS_SetGain(48);	break;
			case  3:	CMOS_SetGain(95);	break;
			case  0:	CMOS_SetGain(0);	break;
			default:	CMOS_SetGain(0);	break;	   //HIGH
		}

	#else

		AECTRL_SetGAINMax(max);	// set GAIN max

	#endif
}

//--------------------------------------------------------------------------------------------------------------------------
const DEMO_func DEMO_SetWDR_FUNC[] = {
	&DEMO_SetWDR_MODE,
	&DEMO_SetWDR_VIEW,
	&DEMO_SetWDR_STRN,
	&DEMO_SetWDR_CONT,
	&DEMO_SetWDR_MDGR,
	&DEMO_SetWDR_AUTO,
	&DEMO_SetWDR_GOAL,
	&DEMO_SetWDR_GAIN,
	&DEMO_SetWDR_IRIS,
	&DEMO_SetWDR_SHUT,
	&DEMO_SetWDR_SRTO
};

typedef	enum {
	DEMO_WDR_FUNC_MODE = 0,
	DEMO_WDR_FUNC_VIEW,
	DEMO_WDR_FUNC_STRN,
	DEMO_WDR_FUNC_CONT,
	DEMO_WDR_FUNC_MDGR,
	DEMO_WDR_FUNC_AUTO,
	DEMO_WDR_FUNC_GOAL,
	DEMO_WDR_FUNC_GAIN,
	DEMO_WDR_FUNC_IRIS,
	DEMO_WDR_FUNC_SHUT,
	DEMO_WDR_FUNC_SRTO,
	DEMO_WDR_FUNC_NUM
} WDR_FUNC_t;

static BYTE func_tbl_disbl[]={  // WDR off
	DEMO_WDR_FUNC_MODE,
};

static BYTE func_tbl_frame[]={  // Frame by Frame
	DEMO_WDR_FUNC_MODE, DEMO_WDR_FUNC_VIEW, DEMO_WDR_FUNC_STRN,
#if WDR_TM_SIZE_CTRL
	DEMO_WDR_FUNC_CONT,
#else
	DEMO_WDR_FUNC_MDGR,
#endif
	DEMO_WDR_FUNC_AUTO, DEMO_WDR_FUNC_GOAL, DEMO_WDR_FUNC_GAIN,
	DEMO_WDR_FUNC_IRIS, DEMO_WDR_FUNC_SHUT, DEMO_WDR_FUNC_SRTO
};

static BYTE func_tbl_built[]={  // Built-in (Tonemap only)
	DEMO_WDR_FUNC_MODE, DEMO_WDR_FUNC_VIEW, DEMO_WDR_FUNC_STRN,
#if WDR_TM_SIZE_CTRL
	DEMO_WDR_FUNC_CONT,
#endif
	DEMO_WDR_FUNC_AUTO, DEMO_WDR_FUNC_GOAL, DEMO_WDR_FUNC_GAIN,
	DEMO_WDR_FUNC_IRIS, DEMO_WDR_FUNC_SHUT
};

static BYTE func_tbl_singl[]={  // DWDR
	DEMO_WDR_FUNC_MODE, DEMO_WDR_FUNC_STRN,
#if WDR_TM_SIZE_CTRL
	DEMO_WDR_FUNC_CONT,
#endif
};

static BYTE func_tbl_line2[]={  // Line by Line (2 Frame)
	DEMO_WDR_FUNC_MODE, DEMO_WDR_FUNC_VIEW, DEMO_WDR_FUNC_STRN,
#if WDR_TM_SIZE_CTRL
	DEMO_WDR_FUNC_CONT,
#else
	DEMO_WDR_FUNC_MDGR,
#endif
	DEMO_WDR_FUNC_AUTO, DEMO_WDR_FUNC_GOAL, DEMO_WDR_FUNC_GAIN,
	DEMO_WDR_FUNC_IRIS, DEMO_WDR_FUNC_SHUT, DEMO_WDR_FUNC_SRTO
};

static BYTE func_tbl_line3[]={  // Line by Line (3 Frame)
	DEMO_WDR_FUNC_MODE, DEMO_WDR_FUNC_VIEW, DEMO_WDR_FUNC_STRN,
#if WDR_TM_SIZE_CTRL
	DEMO_WDR_FUNC_CONT,
#else
	DEMO_WDR_FUNC_MDGR,
#endif
	DEMO_WDR_FUNC_AUTO, DEMO_WDR_FUNC_GOAL, DEMO_WDR_FUNC_GAIN,
	DEMO_WDR_FUNC_IRIS, DEMO_WDR_FUNC_SHUT, DEMO_WDR_FUNC_SRTO
};

//----------------------------------------------------
PBYTE DEMO_GetWDR_FTBL(void) {  //get wdr function table
	PBYTE func_tbl;
	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_FRAME:  func_tbl=func_tbl_frame;  break;
		case MISP_WDR_PROC_BUILT:  func_tbl=func_tbl_built;  break;
		case MISP_WDR_PROC_SINGL:  func_tbl=func_tbl_singl;  break;
		case MISP_WDR_PROC_LINE2:  func_tbl=func_tbl_line2;  break;
		case MISP_WDR_PROC_LINE3:  func_tbl=func_tbl_line3;  break;
		default:                   func_tbl=func_tbl_disbl;  break;
	}
	return func_tbl;
}

//----------------------------------------------------
BYTE DEMO_GetWDR_FNUM(void) {  //get wdr function number
	BYTE func_num;
	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_FRAME:  func_num=sizeof(func_tbl_frame)/sizeof(BYTE);  break;
		case MISP_WDR_PROC_BUILT:  func_num=sizeof(func_tbl_built)/sizeof(BYTE);  break;
		case MISP_WDR_PROC_SINGL:  func_num=sizeof(func_tbl_singl)/sizeof(BYTE);  break;
		case MISP_WDR_PROC_LINE2:  func_num=sizeof(func_tbl_line2)/sizeof(BYTE);  break;
		case MISP_WDR_PROC_LINE3:  func_num=sizeof(func_tbl_line3)/sizeof(BYTE);  break;
		default:                   func_num=sizeof(func_tbl_disbl)/sizeof(BYTE);  break;
	}
	return func_num;
}


//----------------------------------------------------
void DEMO_SetWDR_MODE(WORD nID)
{
	PBYTE func_tbl;
	BYTE i, fID;
	WORD mPos;
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	// wdr <-> dis/dzoom/mirror/rotation can't running at same time.
	if(nID)	{
		if(GetDIS_OnOff()) 			{Demo_DIS_OnOff(OFF); 		SetMenuStatus(DIS_MENU, OFF);} 			// dis 'off'
#if USE_IPC_COMPRES		
		if(GetDZOOM_OnOff()) 		{DEMO_SetZoomMode(OFF); 	SetMenuStatus(DZMODE_MENU, OFF);} 		// dzoom 'off'
		if(DEMO_GetMirrorMode()) 	{DEMO_SetMirrorMode(OFF); 	SetMenuStatus(DISP_MIRROR_MENU, OFF);} 	// mirror 'off'
#endif
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		if(GetROT_RotationEnable())	{SetROT_RotationEnable(OFF);	SetMenuStatus(DISP_ROT_MENU, OFF);}	// rotation 'off'
#endif
	}
	
	//update menu status
	SetMenuStatus(WDR_MENU, (nID)? 1: 0);
	SetMenuStatus(WDR_TUNE_MENU, nID);

	//WDR on/off
	SetWDR_ProcesMode(GetWDR_ProcesList(nID));

	//configure sub-menu items
	func_tbl=DEMO_GetWDR_FTBL();
	for(i=0; i<DEMO_GetWDR_FNUM(); i++) {
		fID=func_tbl[i];
		switch(fID) {
			case DEMO_WDR_FUNC_MODE:  //recursive function
			case DEMO_WDR_FUNC_GAIN:  //init by DEMO_WDR_FUNC_AUTO
			case DEMO_WDR_FUNC_IRIS:  //init by DEMO_WDR_FUNC_AUTO
			case DEMO_WDR_FUNC_SHUT:  //init by DEMO_WDR_FUNC_AUTO
				break;  //skip
			default:
				mPos=WDR_TUNE_MENU+(i<<4);
				DEMO_SetWDR_FUNC[fID](GetMenuStatus(mPos));
				break;
		}
	}

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_SPECIAL_WDR, (PBYTE)&buff[0], sizeof(buff));
#endif
	return;	
}

void DEMO_SetWDR_VIEW(WORD nID)
{
	//set frame view
	SetWDR_BypassPath((MISP_WDR_FVIEW_t)nID);
	return;
}

void DEMO_SetWDR_STRN(WORD nID)
{
	//set tone-map strength
	SetWDR_TmStrength(nID);
	return;
}

void DEMO_SetWDR_CONT(WORD nID)
{
	//set tone-map contrast
	SetWDR_TmContrast(nID);
	return;
}

void DEMO_SetWDR_AUTO(WORD nID)
{
	//set auto gain mode
	SetWDR_AutoExpose(nID);

	//set manual shutter, gain
	if(nID==MISP_WDR_AE_MANU) {
		//parse menu status
		PBYTE func_tbl=DEMO_GetWDR_FTBL();
		WORD mPosGain=0, mPosIris=0, mPosShut=0;
		BYTE i;
		for(i=0; i<DEMO_GetWDR_FNUM(); i++) {
			if(func_tbl[i]==DEMO_WDR_FUNC_GAIN)  mPosGain=WDR_TUNE_MENU+(i<<4);
			if(func_tbl[i]==DEMO_WDR_FUNC_IRIS)  mPosIris=WDR_TUNE_MENU+(i<<4);
			if(func_tbl[i]==DEMO_WDR_FUNC_SHUT)  mPosShut=WDR_TUNE_MENU+(i<<4);
		}

		//set shutter, gain
		if(mPosGain)  DEMO_SetWDR_GAIN(GetMenuStatus(mPosGain));
		if(mPosIris)  DEMO_SetWDR_IRIS(GetMenuStatus(mPosIris));
		if(mPosShut)  DEMO_SetWDR_SHUT(GetMenuStatus(mPosShut));
	}

	return;
}

void DEMO_SetWDR_GOAL(WORD nID)
{
	//set target brightness
	WORD goal=nID;  //nID : 0~19
	SetWDR_AutoTarget(goal);
	return;
}

void DEMO_SetWDR_MDGR(WORD nID)
{
	//set MGR motion threshold
	SetWDR_MdRegister(nID);
	return;
}

void DEMO_SetWDR_GAIN(WORD nID)
{
	//set sensor gain
	WORD range=GetWDR_GainMax()-GetWDR_GainMin();
	WORD offst=GetWDR_GainMin();
	WORD nGain=nID*range/19+offst;  //nID : 0~19
	SetWDR_SensorGain(nGain);
	return;
}

void DEMO_SetWDR_IRIS(WORD nID)
{
	//set lens iris
	WORD range=GetWDR_IrisMax()-GetWDR_IrisMin();
	WORD offst=GetWDR_IrisMin();
	WORD nIris=nID*range/19+offst;  //nID : 0~19
	SetWDR_LensDcIris(nIris);
	return;
}

void DEMO_SetWDR_SHUT(WORD nID)
{
	//set sensor shutter
	WORD range=GetWDR_ShutMax()-GetWDR_ShutMin();
	WORD offst=GetWDR_ShutMin();
	WORD nShut=nID*range/19+offst;  //nID : 0~19
	SetWDR_SensorShut(nShut);
	return;
}

void DEMO_SetWDR_SRTO(WORD nID)
{
	//set exposure ratio
	SetWDR_ExposRatio(nID);
	return;
}


#if 0  //substitute with SetGMCtrlVal
void DEMO_SetGamma(WORD nID)
{
	WORD TEMP;

	if (nID>0) TEMP = (1<<15)+ (1<<12) +(nID<<8)+(nID<<4)+nID;
	else	 TEMP=0;
	MISP_RegWrite(MISP_LOCAL_ID, 0x421, TEMP);
}
#endif

//--------------------------------------------------------------------------------------------------
void DEMO_LoadNR3DLowMode(WORD nID)
{
	if(GetNR_LowLightMode() == MISP_NRLOWLIGHT_ON)		SetMenuStatus(DNRTUNE_LOW_MENU, ON);
	else												SetMenuStatus(DNRTUNE_LOW_MENU, OFF);
}

void DEMO_SetSensorHDMI_PIPSEL(WORD nID)			
{

	switch(nID)	{
		case 0 :	SetMainSource(0);	SetPIPSource(0);	DEMO_SetPIPOnOff(OFF);	SetMenuStatus(PIPMODE_MENU, OFF);	break;//sensor out	 sen_pip out		
		case 1 :	SetMainSource(1);	SetPIPSource(1);	DEMO_SetPIPOnOff(OFF);	SetMenuStatus(PIPMODE_MENU, OFF);	break;//hdmi out	hdmi_pip out							
		case 2 :	SetMainSource(0);	SetPIPSource(1);	DEMO_SetPIPOnOff(ON);	SetMenuStatus(PIPMODE_MENU, ON);	break;//sensor_main hdmi_pip out	
		case 3 :	SetMainSource(1);	SetPIPSource(0);	DEMO_SetPIPOnOff(ON);	SetMenuStatus(PIPMODE_MENU, ON);	break;//hdmi_main sen_pip out				
		default :												break;//same sensor out
	}
}



//--------------------------------------------------------------------------------------------------
void DEMO_SetInputFormat(WORD nID)
{
	if (MainSrc==1)		SetIN_ExtFrmt(nID);		// hdmi
	else				SetIN_SensorFrmt(nID);	// sensor
	
	SetIN_MainFrmt(nID);
	SetFC_MainFCsrcPosiX(0);  SetFC_MainFCsrcSizeH(GetIN_MainSrcSizeH());
	SetFC_MainFCsrcPosiY(0);  SetFC_MainFCsrcSizeV(GetIN_MainSrcSizeV());
	
	if(nID==MISP_VIDSRC_2592x1536p15)	{
		SetBY_5MBypass(1);
		SetDM_Input_select(1);
		//SetSS_image_size(0, 0, 2592, 1536);	// 
		//Set5MegaMemoryMap(ON);				// 
		//Bayer order							// 
	}
	else	{
		SetBY_5MBypass(0);
		SetDM_Input_select(0);
		//SetSS_image_size(4, 4, 1920, 1080);	// 
		//Set5MegaMemoryMap(OFF);				// 
		//Bayer order							// 
	}
	
	SetDZOOM_Window(MIN(GetFC_MainFCsrcSizeH(), GetFC_MainFCdstSizeH()), GetFC_MainFCsrcSizeV());
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetOutputFormat(WORD frmt)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	if(GetOUT_IspFrmt() == frmt)		return;
	
	
	// turn off 'mfc using function'
	if(GetDIS_OnOff()) 			{Demo_DIS_OnOff(OFF);	SetMenuStatus(DIS_MENU, OFF);} 				// dis 'off'
	if(GetDZOOM_OnOff()) 		{DEMO_SetZoomMode(OFF);	SetMenuStatus(DZMODE_MENU, OFF);} 			// dzoom 'off'
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	if(GetROT_RotationEnable())	{SetROT_RotationEnable(OFF);	SetMenuStatus(DISP_ROT_MENU, OFF);}	// rotation 'off'
#endif
	
#if defined(__USE_EXT_PLL_IC__)
	if(GetWDR_ProcesMode()) 	{DEMO_SetWDR_MODE(OFF); 	SetMenuStatus(WDR_MENU, OFF);}
#endif

	
	// skip v.out format
#if defined(__USE_SIL9136_TX__)
	if((GetOUT_IspMode() == MISP_OUT_EMB422_8) || (GetOUT_IspMode() == MISP_OUT_EMB422_10))	{
		switch(frmt)	{
			case  MISP_VIDOUT_1280x720p30:	case  MISP_VIDOUT_1280x720p25:	case  MISP_VIDOUT_1280x720p24:	// HFP : 30/24p(1760), 25p(2420), sil9136 limit(1~1023)
#if (defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)) && defined(__USE_SIL9136_TX__)
			case  MISP_VIDOUT_3840x2160p25:	case  MISP_VIDOUT_3840x2160p24:	// HFP : 25p(1056), 24p(1276), sil9136 limit(1~1023)
#endif
					frmt = GetOUT_IspFrmt();
					SetMenuStatus(OUTFRMT_MENU, frmt-MISP_VIDOUT_1280x720p60);
					OSD_MessageBox(ON, "Not supported format");	Delay_mSec(2000);
					return;
				break;
		}
	}
#endif
	
#if defined(__USE_IT66121_TX__) || defined(__USE_IT6615_TX__) || defined(__USE_LT8618EX_TX__) || defined(__USE_SIL9136_TX__)
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)
	switch(frmt)	{
		case  MISP_VIDOUT_3840x2160p15:	case  MISP_VIDOUT_3840x2160p12:	// 15p
		case  MISP_VIDOUT_2560x1440p30:	case  MISP_VIDOUT_2560x1440p25:	// 4M
					frmt = GetOUT_IspFrmt();
					SetMenuStatus(OUTFRMT_MENU, frmt-MISP_VIDOUT_1280x720p60);
					OSD_MessageBox(ON, "Not supported format");	Delay_mSec(2000);
					return;
				break;
	}
#endif
#endif
	
#if defined(__USE_TVI_TX__)
	switch(frmt)	{
		case  MISP_VIDOUT_1280x720p30:	case  MISP_VIDOUT_1280x720p25:									// table(o), shake
		case  MISP_VIDOUT_1280x720p24:	case  MISP_VIDOUT_1920x1080i60:	case  MISP_VIDOUT_1920x1080i50:	// table(x)
		case  MISP_VIDOUT_1920x1080p60:	case  MISP_VIDOUT_1920x1080p50:	case  MISP_VIDOUT_1920x1080p24:	// table(x)
	#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)		
		case  MISP_VIDOUT_3840x2160p30:	case  MISP_VIDOUT_3840x2160p25:	case  MISP_VIDOUT_3840x2160p24:	// table(x)
		case  MISP_VIDOUT_4096x2160p30:	case  MISP_VIDOUT_4096x2160p25:	case  MISP_VIDOUT_4096x2160p24:	// table(x)
	#endif
#if defined(__USE_59Hz_OUTPUT__)
		case  MISP_VIDOUT_1920x1080i59:	case  MISP_VIDOUT_1920x1080p59:	case  MISP_VIDOUT_3840x2160p29:	// table(x)
		case  MISP_VIDOUT_4096x2160p29:	// table(x)
#endif
				frmt = GetOUT_IspFrmt();
				SetMenuStatus(OUTFRMT_MENU, frmt-MISP_VIDOUT_1280x720p60);
				OSD_MessageBox(ON, "Not supported format");	Delay_mSec(2000);
				return;
			break;
	}
	
	#if (defined(__USE_CMOS_IMX226__) && defined(__USE_ISP_SCLK_OUT__) && defined(__USE_IMX226_4K15P_MODE__))
		if(frmt==MISP_VIDOUT_3840x2160p15){
			// run 15fps && shut(min) = 30fps
			DEMO_SetLVDSPLL(0);		SetMenuStatus(MISCTUNE_USER_PLL_MENU, 0);	
			AECTRL_SetSHUTRange(1098, 2200-4);	
		}
		else{ 			
			//run 30fps			
			DEMO_SetLVDSPLL(5);		SetMenuStatus(MISCTUNE_USER_PLL_MENU, 5);	
			AECTRL_SetSHUTRange(8, 2200-4);	
		}
	#endif
#endif
	
	
#if defined(__USE_4K60_INOUT__)
	switch(frmt)	{
		case MISP_VIDOUT_1920x1080i60:	case MISP_VIDOUT_1920x1080i59:
		case MISP_VIDOUT_1920x1080i50:
		case MISP_VIDOUT_2560x1440p30:	case MISP_VIDOUT_2560x1440p25:
		case MISP_VIDOUT_3840x2160p15:	case MISP_VIDOUT_3840x2160p12:
				frmt = GetOUT_MainFrmt();
				SetMenuStatus(OUTFRMT_MENU, frmt-MISP_VIDOUT_1280x720p60);
				OSD_MessageBox(ON, "Not supported format");	Delay_mSec(2000);
				return;
			break;
	}
	
	SetAPP_Set4K60OutFrmt(frmt);
#endif

	

	// change out v.format & mfc parameter
	SetOUT_IspFrmt(frmt);
	
#if defined(__USE_EXT_PLL_IC__)			// for external pll ic
	SetEXTPLL_Frmt(frmt);
#endif
	
#if defined(__USE_EN332T__)				// set ex-sdi tx format
	SetEN332T_Frmt();
#endif
	
#if defined(__USE_TVI_TX__)				// set tvi-tx format
	SetTVITx_Frmt();
#endif
	
#if defined(__USE_IT66121_TX__)			// set it66121 format
	IT66121_SetFrmt();
#endif

#if defined(__USE_IT6615_TX__)			// set ite6615 format
	ITE6615_SetFrmt();
#endif
	
#if defined(__USE_SIL9136_TX__)			// set sil9136 format
	SIL9136_SetFrmt();
#endif
	
#if defined(__USE_LT8618EX_TX__)		// set lt8618ex format
	LT8618EX_SetFrmt();
#endif
	
	
	SetFC_MainFCsrcPosiX(0);	SetFC_MainFCsrcPosiY(0);
	SetFC_MainFCdstPosiX(0); 	SetFC_MainFCdstSizeH(GetOUT_MainSizeHA());
	SetFC_MainFCdstPosiY(0); 	SetFC_MainFCdstSizeV(GetOUT_MainSizeVA());
	SetFC_MainFCsrcOverscanH(0);	SetFC_MainFCsrcOverscanV(0);
	SetDZOOM_Window(MIN(GetFC_MainFCsrcSizeH(), GetFC_MainFCdstSizeH()), GetFC_MainFCsrcSizeV());
	
	if(GetIN_AuxPath() == MISP_AUXPATH_MAIN_O) 	{ 					// 'main output' --> 'aux input'
		SetIN_AuxFrmt(MIN(GetOUT_MainFrmt(), MISP_VIDOUT_1920x1200pRB)); 		// set aux input frmt
		SetFC_AuxFCsrcPosiX(0); 	SetFC_AuxFCsrcSizeH(GetIN_AuxSrcSizeH());
		SetFC_AuxFCsrcPosiY(0); 	SetFC_AuxFCsrcSizeV(GetIN_AuxSrcSizeV());
	}
	
#if defined(__MDIN_i550_USB_V100__)
	SetFC_AuxFCdstPosiX(0); 	SetFC_AuxFCdstSizeH(GetOUT_AuxSizeHA());
	SetFC_AuxFCdstPosiY(0); 	SetFC_AuxFCdstSizeV(GetOUT_AuxSizeVA());
#endif
	
	// refresh func.
	OSD_CamTitleDisplay(GetMenuStatus(CAMTITLE_MENU));			// refresh cam title
	DEMO_SetMenuPosition(GetMenuStatus(MENU_POSITION_MENU));	// adj menu position
	SetMD_OnOff(GetMenuStatus(MD_MENU));						// refresh md window
	
#if defined(__USE_WDR_PROCESS__)
	//compress control about Line mode WDR
	if(	(GetWDR_ProcesMode()==MISP_WDR_PROC_LINE2)||
		(GetWDR_ProcesMode()==MISP_WDR_PROC_LINE3) ) {  //WDR on
		switch(frmt) {  //compress works on 1080P only
			case MISP_VIDOUT_1920x1080p60:
			case MISP_VIDOUT_1920x1080p50:
			case MISP_VIDOUT_1920x1080p30:
			case MISP_VIDOUT_1920x1080p25:
			case MISP_VIDOUT_1920x1080p24:
#if defined(__USE_59Hz_OUTPUT__)
			case MISP_VIDOUT_1920x1080p59:
			case MISP_VIDOUT_1920x1080p29:
#endif
				SetWDR_CompressCtrl(MISP_COMP_ON);  //compress on
				break;
			default:
				//SetWDR_CompressCtrl(MISP_COMP_420);  //420 mode
				SetWDR_CompressCtrl(MISP_COMP_OFF);  //compress off
				break;
		}
	}
	else if(GetWDR_CompressCtrl()!=MISP_COMP_OFF) {
		SetWDR_CompressCtrl(MISP_COMP_OFF);  //compress off
	}
#endif  //#if defined(__USE_WDR_PROCESS__)
	
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = frmt;
	EEPROM_MultiWrite(EEPROM_SETUP_OUTFRMT, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
WORD DEMO_GetOutputFormat(void)
{
	return  (GetOUT_IspFrmt() - MISP_VIDOUT_1280x720p60);		// skip 480i60 ~ 576p50(0~4);
}

//--------------------------------------------------------------------------------------------------
/* do not delete this comment
void DEMO_SDI_OUT_TEST(void)
{

	//clock-sdi
	MISP_RegField(MISP_HOST_ID, 0x01E, 3, 1, 1);		//di_clk_in (vclk2x)
	MISP_RegWrite(MISP_HOST_ID, 0x030, 4);	// in_size_v	
	MISP_RegField(MISP_HOST_ID, 0x042, 0, 5, 5);		//di_clk_in (vclk2x)

	MISP_RegWrite(MISP_HOST_ID, 0x020, 1);		//vclk_pll_dis toggle on
	MISP_RegWrite(MISP_HOST_ID, 0x020, 0);		//vclk_pll_dis toggle off
	
	//sdi-top set
	MISP_RegWrite(MISP_LOCAL_ID, 0x240, 0x0107);		// CSC_Down_sample_Sync		
	MISP_RegWrite(MISP_LOCAL_ID, 0x241, 1920);		// in_size_h		
	MISP_RegWrite(MISP_LOCAL_ID, 0x242, 1080);		// in_size_v	
	MISP_RegWrite(MISP_LOCAL_ID, 0x243, 0x2467);		// in_active_offset	
	MISP_RegWrite(MISP_LOCAL_ID, 0x244, 0x0104);		// SDI control(0x0104 1080p30, 0x0107 720p60		
	MISP_RegWrite(MISP_LOCAL_ID, 0x245, 0);			// Audio Mode & S352 payload	
	MISP_RegWrite(MISP_LOCAL_ID, 0x246, 0x3300);		// Audio Channel mode	
	MISP_RegWrite(MISP_LOCAL_ID, 0x247, 0);			// for emulation	
	
	//out set	
	MISP_RegWrite(MISP_LOCAL_ID, 0x0A7, 0x4000);	// sdi_dig_io_ctrl1	
	
	//audio setting(now audio top path through)
	MISP_RegField(MISP_HOST_ID, 0x03E, 15, 1, OFF);		//audio top by pass off
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x1FB, 0x00A1);		// ad_control			
	MISP_RegWrite(MISP_LOCAL_ID, 0x1FC, 0x0001);		// audio_up_control 0	
	
}
*/
//--------------------------------------------------------------------------------------------------
void IrCodePanningMoveDirL(void)	  		
{
	WORD MOVE_HSIZE = 12;	//user tunning
	WORD Org_startH = 10;	//see misp_sensortop.c
	WORD Dis_safe_boundary = 64;
	
	//check limitation : (GetSS_image_start_pos_h -MOVE_HSIZE) > (Org_startH + bH + DIS safe boundary)
	if(GetSS_image_start_pos_h() > Org_startH + GetSS_image_boundary_size_h() + Dis_safe_boundary + MOVE_HSIZE)
	{
		if(GetDIS_OnOff())	SetDIS_Wait();	
		SetSS_image_panning(GetSS_image_start_pos_h() - MOVE_HSIZE, GetSS_image_start_pos_v());
	}


}

//--------------------------------------------------------------------------------------------------
void IrCodePanningMoveDirR(void)	  		
{
	WORD MOVE_HSIZE = 12;		//user tunning
	WORD Org_startH = 10;		//see misp_sensortop.c	
	WORD Org_activeH = 3840;		//see misp_sensortop.c
	WORD Dis_safe_boundary = 64;
	
	//check limitation : (GetSS_image_start_pos_h + MOVE_HSIZE) < (Org_startH + Org_activeH + bH -activeH - DIS safe boundary)
	if(GetSS_image_start_pos_h() <Org_startH + Org_activeH + GetSS_image_boundary_size_h() - GetSS_image_active_size_h() - Dis_safe_boundary - MOVE_HSIZE)
	{
		if(GetDIS_OnOff())	SetDIS_Wait();	
		SetSS_image_panning(GetSS_image_start_pos_h() + MOVE_HSIZE, GetSS_image_start_pos_v());
	}

}

//--------------------------------------------------------------------------------------------------
void IrCodePanningMoveDirU(void)	   		
{
	WORD MOVE_VSIZE = 8;	//user tunning
	WORD Org_startV = 19;		//see misp_sensortop.c
	WORD Dis_safe_boundary=64;
	
	//check limitation : (GetSS_image_start_pos_v -MOVE_VSIZE) > (Org_startV + bV + DIS safe boundary)
	if(GetSS_image_start_pos_v() > Org_startV + GetSS_image_boundary_size_v()+ Dis_safe_boundary + MOVE_VSIZE)
	{
		if(GetDIS_OnOff())	SetDIS_Wait();	
		SetSS_image_panning(GetSS_image_start_pos_h(), GetSS_image_start_pos_v() - MOVE_VSIZE);
	}


}
//--------------------------------------------------------------------------------------------------
void IrCodePanningMoveDirD(void)			
{
	WORD MOVE_VSIZE = 8;	//user tunning
	WORD Org_startV = 19;		//see misp_sensortop.c
	WORD Org_activeV = 2160;		//see misp_sensortop.c	
	WORD Dis_safe_boundary=64;
	
	//check limitation : (GetSS_image_start_pos_h + MOVE_HSIZE) < (Org_startV + Org_activeV + bV -activeV - DIS safe boundary)
	if(GetSS_image_start_pos_v() < Org_startV + Org_activeV + GetSS_image_boundary_size_v() - GetSS_image_active_size_v() - Dis_safe_boundary - MOVE_VSIZE)
	{
		if(GetDIS_OnOff())	SetDIS_Wait();	
		SetSS_image_panning(GetSS_image_start_pos_h(), GetSS_image_start_pos_v() + MOVE_VSIZE);
	}
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetCropPosiCenter(BOOL OnOff)	  		
{
	if(OnOff)	{move_in_LR = 15;	move_in_UD = 15;}
	else		{move_in_LR = 0;	move_in_UD = 0;}
	
	SetFC_MainFCsrcPosiX(move_in_LR*64);	SetFC_MainFCsrcPosiY(move_in_UD*36);
}

//--------------------------------------------------------------------------------------------------
void IrCodeCropMoveDirL(void)	  		
{
	if(move_in_LR<1) move_in_LR=0;
	else move_in_LR--;

	SetFC_MainFCsrcPosiX(move_in_LR*64);
}

//--------------------------------------------------------------------------------------------------
void IrCodeCropMoveDirR(void)	  		
{
	if(move_in_LR>=30) move_in_LR=30;
	else move_in_LR++;

	SetFC_MainFCsrcPosiX(move_in_LR*64);
}

//--------------------------------------------------------------------------------------------------
void IrCodeCropMoveDirU(void)	   		
{
	if(move_in_UD<1) move_in_UD=0;
	else move_in_UD--;

	SetFC_MainFCsrcPosiY(move_in_UD*36);
}

//--------------------------------------------------------------------------------------------------
void IrCodeDWindowMoveDirL(void)
{
	WORD out_ha = GetOUT_MainSizeHA();
	
	if(DWindowPos >= 64)		DWindowPos-=64;	// step:64
	
	SetOUT_AuxbgnHA(DWindowPos+160);		SetOUT_AuxendHA(DWindowPos+160 + out_ha);	//aux hactive start/end
	SetFC_MainFCsrcPosiX(out_ha - DWindowPos);
}

//--------------------------------------------------------------------------------------------------
void IrCodeDWindowMoveDirR(void)
{
	WORD out_ha = GetOUT_MainSizeHA();
	
	if(DWindowPos <= (out_ha-64))		DWindowPos+=64;	// step:64
	
	SetOUT_AuxbgnHA(DWindowPos+160);		SetOUT_AuxendHA(DWindowPos+160 + out_ha);	//aux hactive start/end
	SetFC_MainFCsrcPosiX(out_ha - DWindowPos);
}

//--------------------------------------------------------------------------------------------------
void IrCodeCropMoveDirD(void)			
{
	if(move_in_UD>=30) move_in_UD=30;
	else move_in_UD++;

	SetFC_MainFCsrcPosiY(move_in_UD*36);
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetNRInputMode(void)
{
	SetNR_Toggle_Inputimage_type() ;
}

//--------------------------------------------------------------------------------------------
void DEMO_Set485CamID(WORD nID)	
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	PELCO_SetCamID(nID);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_RS485_ID, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------
void DEMO_Set485IDDisp(WORD nID)	
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	PELCO_SetIDDisp((nID)?  ON:  OFF);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_RS485_DISP, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------
void DEMO_Set485Baudrate(WORD nID)	
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	PELCO_SetBaudRate(nID);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_RS485_BAUD, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------
void DEMO_SetNTPal(WORD nID)	
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif

	switch(GetOUT_AuxFrmt())	{
		case MISP_VIDOUT_720x480i60 :	case MISP_VIDOUT_720x576i50 :
		case MISP_VIDOUT_960x480i60 :	case MISP_VIDOUT_960x576i50 :
		break;
		
		default : 
		UARTprintf("DEMO_SetNTPal did not work \n"); 
		return;	
	}

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveNTPAL(nID);
#endif
	
	switch(nID)	{
		case  0:	SetOUT_CVBSNTSC720H();	break;
		case  1:	SetOUT_CVBSPAL();		break;
	}
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_SETUP_NTPAL, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------
WORD DEMO_GetNTPal(void)	// 0:ntsc, 1:pal
{
	switch(GetOUT_EncFrmt())	{
		case MISP_VID_VENC_PAL_B:
		case MISP_VID_VENC_PAL_D:
		case MISP_VID_VENC_PAL_G:
		case MISP_VID_VENC_PAL_H:
		case MISP_VID_VENC_PAL_I:
		case MISP_VID_VENC_PAL_M:
		case MISP_VID_VENC_PAL_N:
		case MISP_VID_VENC_PAL_Nc:
		case MISP_VID_VENC_PAL_60:
		case MISP_VID_VENC_SECAM:
			return	1;	// pal
		case MISP_VID_VENC_NTSC_M:
		case MISP_VID_VENC_NTSC_J:
		case MISP_VID_VENC_NTSC_443:
		default:
			return	0;	// ntsc
	}
}

//--------------------------------------------------------------------------------------------
void DEMO_SetLang(WORD nID)	
{
#if defined(__USE_CHN_FONT__)
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	SetMenuStatus(LANG_MENU, (nID)?  1:  0);		// 0:eng, 1:chn
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_SETUP_LANG, (PBYTE)&buff[0], sizeof(buff));
#endif
#endif
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetFreezeMode(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveFreezeMode(nID);
#endif
	
	// MISP_RegField(MISP_LOCAL_ID, 0x040, 1, 1, nID);
	SetFC_StopMainFC(nID) ;
	SetFC_StopAuxFC(nID);

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_DISP_FREEZE, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
static void DEMO_SetDZOOMMirror(WORD nID)
{
	switch(nID)	{
		case 1: SetMenuStatus(DZPOSI_MENU, 240 - GetMenuStatus(DZPOSI_MENU));	break;	// posi-x mirror
		case 2: SetMenuPosiY(DZPOSI_MENU, 240 - GetMenuPosiY(DZPOSI_MENU));		break;	// posi-y mirror
	}
	
	if(GetDZOOM_OnOff())		DEMO_SetZoomPosition(0);								// dz position refresh
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetMirrorMode(WORD nID)    //0:off, 1:H, 2:V, 3:H+V
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
#if 0	//swiching disable to make swap image after i5x0
	nID = (nID==1)?  2:  (nID==3)?  0:  nID;
	SetMenuStatus(DISP_MIRROR_MENU, nID);
	SetMenuRedraw();
#else	//switching fpga
	WORD offCH1 = GetLCMX03LF_offch1_org();
	WORD offCH2 = GetLCMX03LF_offch2_org();
	
	
	SetLCMX03LF_offch1((nID&1)?  offCH2:  offCH1);
	SetLCMX03LF_offch2((nID&1)?  offCH1:  offCH2);
#endif	
	SPImAPP_SetSlaveMirrorMode(nID);
#endif
	
	// for mirror + dzoom
	if(((nID&1)==1) && ((GetFC_MainMirror()&1)==0))	{					// mirror 'h' off -> on
		DEMO_SetDZOOMMirror(1);
	}
	if(((nID&1)==0) && ((GetFC_MainMirror()&1)==1))	{					// mirror 'h' on -> off
		DEMO_SetDZOOMMirror(1);
	}
	if(((nID&2)==2) && ((GetFC_MainMirror()&2)==0))	{					// mirror 'v' off -> on
		DEMO_SetDZOOMMirror(2);
	}
	if(((nID&2)==0) && ((GetFC_MainMirror()&2)==2))	{					// mirror 'v' on -> off
		DEMO_SetDZOOMMirror(2);
	}
	
	SetFC_MainMirror(nID);
	SetFC_AuxMirror(nID);

	if(nID&2)	{SetIN_MainFRdlyY(1);	SetIN_MainFRdlyC(1);	SetIN_AuxFRdly(1);}	// v-flip 'on' : frame delay '0' -> '1'
	else		{SetIN_MainFRdlyY(0);	SetIN_MainFRdlyC(0);	SetIN_AuxFRdly(0);}	// v-flip 'off' : frame delay '1' -> '0'
	

	// reset Frame-WDR to prevent long/short ID missing.
	if(GetWDR_ProcesMode()==MISP_WDR_PROC_FRAME)
		SetWDR_ProcessRst();
	
	SetMD_OnOff(GetMenuStatus(MD_MENU));		// refresh md window
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_DISP_MIRROR, (PBYTE)&buff[0], sizeof(buff));
#endif
	
#if __MISP100_DBGPRT__ == 1
	UARTprintf("DEMO_SetMirrorMode : 0:OFF, 1:H, 2:V, 3:ALL (%d) \n", nID); 
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE DEMO_GetMirrorMode(void)    //0:off, 1:H, 2:V, 3:H+V
{
	return	GetFC_MainMirror();
}

//--------------------------------------------------------------------------------------------------			
void DEMO_SetFactoryReset(void)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveFactoryReset();
#endif
	
	SetMenuFactoryDefault();	// clear menu data
	
#ifdef	__USE_EEPROM__
	EraseE2promMenuStatus();		//erase e2prom menu data
#endif

#if __MISP100_DBGPRT__ == 1
	UARTprintf("DEMO_SetFactoryReset() !!! \n\n\n");
#endif
	
	Delay_mSec(100);
	
	MISP_Reboot();
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetTPCtrl(WORD nID)		 
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveTPSel(nID);
#endif
	
	SetSS_stp_mode(nID);
	
	SetMenuStatus(TP_POSI_MENU, stTP_Bayer[nID].x );
	SetMenuPosiY(TP_POSI_MENU,  stTP_Bayer[nID].y ); 
	SetMenuStatus(TP_SIZE_MENU, stTP_Bayer[nID].w );
	SetMenuPosiY(TP_SIZE_MENU,  stTP_Bayer[nID].h ); 
	
	DEMO_SetTPPosition(0);
	DEMO_SetTPSize(0);
	
	UARTprintf("Ctrl_TPPosition : size(%d,%d) \n",	 stTP_Bayer[nID].w, stTP_Bayer[nID].h);
	UARTprintf("Ctrl_SetTPSize : pos(%d,%d) \n",	 stTP_Bayer[nID].x, stTP_Bayer[nID].y);
	
}

void DEMO_SetTPOnOffCtrl(BOOL OnOFF)
{
	BYTE i=0;
	WORD	byr_blc_mode, byr_blc_val;
	BOOL	edm_rb_slop_en, sns_rb_comp_en, sns_dc_gain_en, sns_wb_gain_en;

	static WORD byr_blc_val_org;
	static WORD byr_blc_mode_org ;
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveTPOnOff(OnOFF);
#endif
	
	if (stTP_Bayer[0].Init == 0){			
		SetMenuStatus(TP_POSI_MENU,0);
		SetMenuPosiY(TP_POSI_MENU, 0); 
		SetMenuStatus(TP_SIZE_MENU,60);
		SetMenuPosiY(TP_SIZE_MENU, 34 ); 	

		for(i=0; i<50;i++)	{
			if (stTP_Bayer[i].Init == 0)	{
				stTP_Bayer[i].OnOff =  OFF;

				stTP_Bayer[i].x = 0;
				stTP_Bayer[i].y = 0;										
				stTP_Bayer[i].w = 60;			
				stTP_Bayer[i].h = 34;
				stTP_Bayer[i].Init = 1;
			}
		}
	}	

	if(OnOFF){	for(i=0;i<50;i++){stTP_Bayer[i].OnOff=ON;}	}
	else		{ 	for(i=0;i<50;i++){stTP_Bayer[i].OnOff=OFF;}	}
	
	//WB Stop, AE, AS Off
	if(OnOFF){		
		stWBCTRL.ctrl &= (~WBCTRL_PROCESS_OFF);
		stWBCTRL.ctrl |= (WBCTRL_PROCESS_OFF);
		stAECTRL.ctrl &= (~AECTRL_PROCESS_OFF);
		stAECTRL.ctrl |= (AECTRL_PROCESS_OFF);
		
		byr_blc_mode = 0;	byr_blc_val = 0;						//0:manual, 1:avg
		edm_rb_slop_en=0, sns_rb_comp_en=0;	sns_dc_gain_en=0;	sns_wb_gain_en=0;	//odm
		SetSS_stp_onoff(ON);

		byr_blc_mode_org = GetSS_black_level_mode();
		byr_blc_val_org = GetSS_man_black_level_target() ;
		//MISP_RegRead(MISP_LOCAL_ID, 0x4A9, &byr_blc_val_org);
		//byr_blc_val_org &= 0x0FFF;
		
		SetBY_gamma_enable(0);
		SetTONE_RGBGAMMA_enable(0);
		SetTONE_YCGAMMA_enable(0);
		SetTONE_AYCGAMMA_enable(0);
		
		SetIN_MainHoriPeaking(OFF) ;
		SetFC_MainVertPeaking(OFF) ;		
	}
	else{
		stWBCTRL.ctrl &= (~WBCTRL_PROCESS_OFF);
		stWBCTRL.ctrl |= (WBCTRL_PROCESS_RUN);
		stAECTRL.ctrl &= (~AECTRL_PROCESS_OFF);
		stAECTRL.ctrl |= (AECTRL_PROCESS_RUN);	
		
		byr_blc_mode = byr_blc_mode_org;	byr_blc_val = byr_blc_val_org;			//0:manual, 1:avg
		edm_rb_slop_en=1, sns_rb_comp_en=1;	sns_dc_gain_en=1;	sns_wb_gain_en=1;	//odm
		SetSS_stp_onoff(OFF);
	
		SetBY_gamma_enable(1);
		SetTONE_RGBGAMMA_enable(1);
		SetTONE_YCGAMMA_enable(1);
		SetTONE_AYCGAMMA_enable(1);	
		
		SetIN_MainHoriPeaking(ON) ;
		SetFC_MainVertPeaking(ON) ;		
	}
	
	SetSS_black_level_mode(byr_blc_mode);
	SetSS_man_black_level_target(byr_blc_val);
	SetSS_ODM_black_level_target(byr_blc_val);
	//MISP_RegField(MISP_LOCAL_ID, 0x4A8, 13, 2, byr_blc_mode); 		//bayer_blc_mode
	//MISP_RegField(MISP_LOCAL_ID, 0x4A9, 0, 12, byr_blc_val); 		//manual_blc_val
	//MISP_RegField(MISP_LOCAL_ID, 0x60C, 4, 4, edm_rb_slop_en<<3 | sns_rb_comp_en<<2 | sns_dc_gain_en<<1 | sns_wb_gain_en); 	//ext_block_ctrl	
	WBCTRL_SetAWBCtrlPath(0,edm_rb_slop_en,sns_rb_comp_en,sns_dc_gain_en,sns_wb_gain_en) ;
	//DPC on/off
	if(OnOFF==ON) DEMO_SetLiveDPCMode(OFF);
	else DEMO_SetLiveDPCMode(GetMenuStatus(DPC_MENU));
	
	if((OnOFF==ON) && (GetMenuStatus(TP_CTRL_MENU)==0))		DEMO_SetTPCtrl(0);	//for first ON.
	
	//set pos, size	
	DEMO_SetTPPosition(0);
	DEMO_SetTPSize(0);

}

//--------------------------------------------------------------------------------------------------------------------------
void DEMO_SetAWBHold(BOOL holdf)
{
	if(holdf == TRUE) {
		stWBCTRL.ctrl |= (WBCTRL_PROCESS_HOLD);
		//UARTprintf("AWB hold...\r\n");
		MISP_RegWrite(MISP_LOCAL_ID, 0x608, 256);
		MISP_RegWrite(MISP_LOCAL_ID, 0x60A, 256);
	} else {
		//UARTprintf("AWB auto...\r\n");
		stWBCTRL.ctrl  &= (~WBCTRL_PROCESS_HOLD);
	}
}

//--------------------------------------------------------------------------------------------
void DEMO_SetTPPosition(WORD nID)
{

	WORD	number = GetMenuStatus(TP_CTRL_MENU);	
	WORD	Posi_x = GetMenuStatus(TP_POSI_MENU),	Posi_y = GetMenuPosiY(TP_POSI_MENU); 
	WORD	Size_w = GetMenuStatus(TP_SIZE_MENU),	Size_h = GetMenuPosiY(TP_SIZE_MENU); 
	WORD	unit = 32;

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveTPPosi(Posi_x, Posi_y);
#endif
	
	UARTprintf("number : nID(%d) number(%d) \n", nID,number);
	UARTprintf("BeforePosition : position(%d,%d) \n",	 Posi_x, Posi_y);
	UARTprintf("BeforeTPSize : size(%d,%d) \n",	 Size_w, Size_h);

	if(GetSS_image_active_size_h()==3840)	unit=64;
	else if(GetSS_image_active_size_h()==3072) unit = 52;
	else if(GetSS_image_active_size_h()==2592) unit = 44;		
	else unit=32;
	
// for limit calulate(at osdmenu.h, single variable over case limited) 
	if((Posi_x+Size_w)>60)	 	SetMenuStatus(TP_POSI_MENU, --Posi_x);
	if((Posi_y+Size_h)>34) 	 SetMenuPosiY(TP_POSI_MENU, --Posi_y);
	
// store size value	
	stTP_Bayer[number].x= Posi_x;			stTP_Bayer[number].y =  Posi_y;

	SetMenuStatus(TP_POSI_MENU, Posi_x);	SetMenuPosiY(TP_POSI_MENU, Posi_y);
	
// for register write

	SetSS_stp_xpos(stTP_Bayer[number].x * unit);
	SetSS_stp_ypos(stTP_Bayer[number].y * unit);
	
	UARTprintf("after : nID(%d) number(%d) \n", nID,number);
	UARTprintf("afterPosition : position(%d,%d) \n", Posi_x, Posi_y);
	UARTprintf("afterTPSize : size(%d,%d) \n", Size_w, Size_h);

}

//--------------------------------------------------------------------------------------------
void DEMO_SetTPSize(WORD nID)
{

	WORD	number = GetMenuStatus(TP_CTRL_MENU);	
	WORD	Posi_x = GetMenuStatus(TP_POSI_MENU),	Posi_y = GetMenuPosiY(TP_POSI_MENU); 
	WORD	Size_w = GetMenuStatus(TP_SIZE_MENU),	Size_h = GetMenuPosiY(TP_SIZE_MENU); 
	WORD	sensor_in_tp_hsize, sensor_in_tp_vsize;
	WORD	unit = 32;

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveTPSize(Size_w, Size_h);
#endif
	
	UARTprintf("number : nID(%d) number(%d) \n", nID,number);
	UARTprintf("BeforePosition : position(%d,%d) \n",	 Posi_x, Posi_y);
	UARTprintf("BeforeTPSize : size(%d,%d) \n",	 Size_w, Size_h);

	if(GetSS_image_active_size_h()==3840)	unit=64;
	else if(GetSS_image_active_size_h()==3072) unit = 52;
	else if(GetSS_image_active_size_h()==2592) unit = 44;		
	else unit=32;

// for limit calulate(at osdmenu.h, single variable over case limited) 
	if((Posi_x+Size_w)>60)	 	SetMenuStatus(TP_SIZE_MENU, --Size_w);
	if((Posi_y+Size_h)>34) 	 SetMenuPosiY(TP_SIZE_MENU, --Size_h);

// store size value		
	stTP_Bayer[number].w= Size_w;			stTP_Bayer[number].h = Size_h;
	SetMenuStatus(TP_SIZE_MENU, Size_w);		SetMenuPosiY(TP_SIZE_MENU, Size_h);


// for register write
#if defined(__USE_CMOS_AR0331__) && defined(__USE_AR0331_NORMAL_3MegaMODE__)
	sensor_in_tp_hsize = stTP_Bayer[number].w * 34+8;
	sensor_in_tp_vsize = stTP_Bayer[number].h * 45+6;
#else
	if((Posi_x+Size_w)>=60)		sensor_in_tp_hsize = GetSS_sensor_hactive_read();
	else						sensor_in_tp_hsize = stTP_Bayer[number].w * unit;
	
	if((Posi_y+Size_h)>=34)		sensor_in_tp_vsize = GetSS_sensor_vactive_read();
	else						sensor_in_tp_vsize = stTP_Bayer[number].h * unit;
#endif

	SetSS_stp_xsize(sensor_in_tp_hsize);
	SetSS_stp_ysize(sensor_in_tp_vsize);
	
	UARTprintf("after : nID(%d) number(%d) \n", nID,number);
	UARTprintf("afterPosition : position(%d,%d) \n", Posi_x, Posi_y);
	UARTprintf("afterTPSize : size(%d,%d) \n", Size_w, Size_h);
	
}

void DEMO_SetTPDefault(void)	
{

	BYTE i;

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveTPDefault();
#endif
	
	for(i=0; i<40; i++)		//display, position, size set to default.
	{

				stTP_Bayer[i].OnOff = OFF;		
				stTP_Bayer[i].x = 0;
				stTP_Bayer[i].y = 0;										
				stTP_Bayer[i].w = 60;			
				stTP_Bayer[i].h = 34;
				stTP_Bayer[i].Init = 1;

	UARTprintf("DEMO_SetTPPosition : size(%d,%d) \n",	 stTP_Bayer[i].w, stTP_Bayer[i].h);
	UARTprintf("DEMO_SetTPSize : pos(%d,%d) \n",	 stTP_Bayer[i].x, stTP_Bayer[i].y);
	}

	DEMO_SetTPOnOffCtrl(OFF);
	DEMO_SetTPCtrl(0);			
	SetMenuStatus(TP_CTRL_MENU, 0);	
	SetMenuStatus(TP_ONOFF_MENU, 0);
	
	SetMenuStatus(TP_POSI_MENU,0);
	SetMenuPosiY(TP_POSI_MENU, 0); 
	
	SetMenuStatus(TP_SIZE_MENU,60);
	SetMenuPosiY(TP_SIZE_MENU, 34); 		
	
	SetSS_stp_imbue();
}


void DEMO_SetGammaValue(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveGammaOnOff(nID);
#endif
	
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	SetBY_gamma_enable(ON);
	SetBY_GammaValue(nID) ;
#else   							// MDIN-i51X (i510, i540, i550)
	if(nID==1){
		SetBY_gamma_enable(OFF);
	#if !defined(__RGBGAMMA_USE_ADJUST__)
		SetTONE_RGBGAMMA_enable(OFF);		// If NOT use RGB gamma curve as CONTRAST 
	#endif
	}
	else{
		if(!GetWDR_ProcesMode()) SetBY_gamma_enable(ON);
	#if !defined(__RGBGAMMA_USE_ADJUST__)
		SetTONE_RGBGAMMA_val(nID);			// If NOT use RGB gamma curve as CONTRAST 
		SetTONE_RGBGAMMA_enable(ON);		// If NOT use RGB gamma curve as CONTRAST 
	#endif
	}


//		SetBY_GammaValue((BYTE)nID);

	//RGB Mode
//	if(GetBY_gamma_mode()){
//		SetTONE_RGBGAMMA_val((BYTE)nID);
//		UARTprintf("Call RGB Gamma LUT \r\n");
//	}
	//Bayer Mode
//	else{
		
//		if((nID==13)||(nID==14)||(nID==15)){		//When Custum gamma mode
//			SetTONE_RGBGAMMA_enable(ON);		//Use Front gamma and Read Gamma
//			SetTONE_RGBGAMMA_val(11);
//		}
//		else{									//When Custum gamma mode
//			SetTONE_RGBGAMMA_enable(OFF);		//RGB gamma OFF	
//		}
		
//		SetBY_GammaValue((BYTE)nID);
//		UARTprintf("Call Bayer Gamma LUT \r\n");		
//	}

#endif
	
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_EFFECT_GAMMA, (PBYTE)&buff[0], sizeof(buff));
#endif
}

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
//--------------------------------------------------------------------------------------------------
void DEMO_SetFrontGammaEn(BYTE nID)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveGammaFrontEn(nID);
#endif
	
	SetBY_GammaValue(nID);
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetRearGammaEn(BYTE nID)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveGammaRearEn(nID);
#endif
	
	SetTONE_RGBGAMMA_val(nID);
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetGamma8tap(WORD nID)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveGamma8tap(nID);
#endif
	
#if 0
	//when default inv gamma off
	if(nID<2){
		SetTONE_BYIGAMMA_LUT(0);		
		SetTONE_BYIGAMMA_enable(OFF);		
	}
	else{	
		
		if (nID == 2 ) SetTONE_BYIGAMMA_LUT(2);		//0.9's inv
		else if (nID == 3 ) SetTONE_BYIGAMMA_LUT(4);	//0.8's inv
		else if (nID == 4 ) SetTONE_BYIGAMMA_LUT(6);	//0.7's inv
		else if (nID == 5 ) SetTONE_BYIGAMMA_LUT(8);	//0.6's inv
		else if (nID == 6 ) SetTONE_BYIGAMMA_LUT(9);	//0.55's inv	
		else if (nID == 7 ) SetTONE_BYIGAMMA_LUT(10);	//0.5's inv
		else if (nID == 8 ) SetTONE_BYIGAMMA_LUT(11);	//0.45's inv	
		else if (nID == 9 ) SetTONE_BYIGAMMA_LUT(12);	//0.4's inv	
		else if (nID == 10 ) { 				
			if(GetOUT_MainCSC()==2)		SetTONE_BYIGAMMA_LUT(14);
			else if(GetOUT_MainCSC()==1)	SetTONE_BYIGAMMA_LUT(15);			
			else									SetTONE_BYIGAMMA_LUT(13);				
		}	

		SetTONE_BYIGAMMA_enable(ON);
	}	
#else
	
	if(nID>=2 && nID<=5){
		SetTONE_BYIGAMMA_LUT((nID-2));		
		SetTONE_BYIGAMMA_enable(ON);		
	}
	else{
		// SetTONE_BYIGAMMA_LUT(0);		
		SetTONE_BYIGAMMA_enable(OFF);			
	}
#endif
}
//--------------------------------------------------------------------------------------------------
void DEMO_SetBAYER_LUT_Extend(BYTE nID)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveGammaBYLUTExt(nID);
#endif
	
	SetBY_BY256LUT_Extend(nID);		//non-uniform input segment nID>0
	//see lut256xWx3_transform.vhd
}

void DEMO_SetRGB_LUT_Extend(BYTE nID)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveGammaRGBLUTExt(nID);
#endif
	
	if(nID==1)	SetTONE_RGB256LUT_Extend(ON);		//non-uniform input segment <fine-to-coarse>
	else			SetTONE_RGB256LUT_Extend(OFF);	//uniform input segment
	//see lut256xWx3_transform.vhd
}

void DEMO_SetYC_LUT_Extend(BYTE nID)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveGammaYCLUTExt(nID);
#endif
	
	if(nID==1)	SetTONE_YC256LUT_Extend(ON);		//non-uniform input segment <fine-to-coarse>
	else			SetTONE_YC256LUT_Extend(OFF);	//uniform input segment
	//see lut256xWx3_transform.vhd
}
void DEMO_SetBYLUT_Xresolution(BYTE nID)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveGammaBYLUTXresol(nID);
#endif
	
	SetBY_BYLUT_Type(nID);
}
void DEMO_SetRGBLUT_Xresolution(BYTE nID)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveGammaRGBLUTXresol(nID);
#endif
	
	SetTONE_RGBLUT_Type(nID);
}
void DEMO_SetRGBLUT_ThermoEN(BYTE nID)
{
	SetTONE_Thermo_RGBGAMMA(nID);	
}
//--------------------------------------------------------------------------------------------------
#endif

//--------------------------------------------------------------------------------------------------
void DEMO_SetBackLightMode(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveBackLightMode(nID);
#endif
	
	// md & pm & blc can't running same time
	if(nID) 	{
		if(GetMD_OnOff()) 			{SetMD_OnOffMenu(OFF);	SetMenuStatus(MD_MENU, OFF);} 		// md 'off'
		if(GetPM_OnOff()) 			{SetPM_OnOffMenu(OFF);	SetMenuStatus(PM_MODE_MENU, OFF);} 	// pm 'off'
	}
	
	SetMenuStatus(BLIGHT_MENU, nID); 		// b.light menu
	
	switch(nID)	{
		case  0:	SetHLC_OnOffMenu(OFF);	SetBLC_OnOffMenu(OFF);		break; 	// 'off'
		case  1:	SetHLC_OnOffMenu(OFF);	SetBLC_OnOffMenu(ON);		break; 	// 'blc'
		case  2:	SetBLC_OnOffMenu(OFF);	SetHLC_OnOffMenu(ON);		break; 	// 'hlc'
	}
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_SPECIAL_BLIGHT, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE DEMO_GetBackLightMode(void)
{
	return 	GetBLC_OnOff()?  1:  (GetHLC_OnOff()?  2:  0);
}

//--------------------------------------------------------------------------------------------------
void DEMO_BINNING_MODE(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveBinning(nID);
#endif
	
	switch(nID){	
		case 0 :
				SetSS_binning_enable(OFF);
				stWBCTRL.ctrl &= (~WBCTRL_PROCESS_OFF);
				stWBCTRL.ctrl |= (WBCTRL_PROCESS_RUN);
				break;
	
		case 1 : 
				SetSS_binning_enable(ON);
				stWBCTRL.ctrl |= (WBCTRL_PROCESS_OFF);
				break;
	}
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_SPECIAL_BIN, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetB_BYPASSCtrl(WORD nID)		 
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveByrBypass(nID);
#endif
	
	switch(nID){	
		case 0 : SetBY_Bypass(OFF); SetDM_Bypass(OFF); break;
		case 1 : SetBY_Bypass(ON); 	SetDM_Bypass(OFF); break;
		case 2 : SetBY_Bypass(OFF); SetDM_Bypass(ON); break;	
		case 3 : SetBY_Bypass(ON); SetDM_Bypass(ON); break;			
	}	
	
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetYCConv(WORD nID)		
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveYCConv(nID);
#endif
	
	//read Eng.menu(YCConv) setting gamma curve both when use  __COLOR_S7320__ and gamma custom(14 or 15 case)
	SetIN_MainCSC(nID);

}
void DEMO_SetOUTCSCDefine(WORD nID)		
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveOutCSC(nID);
#endif
	
	SetOUT_MainCSC(nID);
	UARTprintf("DEMO_SetOUTCSCDefine!!! %d \n", GetOUT_MainCSC());

	//call saturation..
	DEMO_SetSaturation(GetMenuStatus(SATURATION_MENU));
}

//----------------------------------------------------------------------------------------
void DEMO_SetEffectDefault(void)		
{
	SetBY_Lens_corr_en(OFF);	// LSC(Off, On)
//	DEMO_SetGammaValue(0);
//	DEMO_SetYCConv(0);			// YCConverion test
	DEMO_SetContrast(11);		// Contrast(0~20)
	DEMO_SetBrightness(11);		// Brightness(0~20)
	DEMO_SetSaturation(10);		// Saturation(0~20)
	DEMO_SetHue(10);			// Hue(0~20)
	DEMO_SetSharpness(5);		// Sharpness(0~20)

	DEMO_SetAutoConOnOff(AutoCon_Init);
	DEMO_SetAutoSaturation(AutoSat_Init);
	DEMO_SetAutoEdgeOnOff(AutoEdge_Init);	
//	DEMO_SetAutoBright(AutoBright_Init);	
	
	
	// set menu value with application parameter
	SetMenuStatus(LSC_MENU, GetBY_Lens_corr_en()); 				// lens shading correction
	SetMenuStatus(GM_MENU, (GetBY_gamma_enable())?  0:  1); 	// gamma
	SetMenuStatus(CONTRAST_MENU, DEMO_GetContrast()); 			// contrast
	SetMenuStatus(BRIGHTNESS_MENU, DEMO_GetBrightness()); 		// brightness
	SetMenuStatus(SATURATION_MENU, DEMO_GetSaturation()); 		// saturation
	SetMenuStatus(HUE_MENU, DEMO_GetHue()); 					// hue
	SetMenuStatus(EDGE_MENU, DEMO_GetSharpness()); 				// edge enhance
	SetMenuStatus(AUTO_SAT_MENU, DEMO_GetAutoSatLevel()); 		// auto-saturation
	SetMenuStatus(AUTO_EDGE_MENU, DEMO_GetAutoEdgeOnOff()); 	// auto-edge
//	SetMenuStatus(AUTO_BRIGHT_MENU, DEMO_GetAutoBright()); 		// auto-bright
	
#if __MISP100_DBGPRT__ == 1
	UARTprintf("DEMO_SetEffectDefault!!! \n");
#endif
}


void DEMO_HTMOnOFF(WORD nID)
{
	if(nID){
	MISP_RegWrite(MISP_LOCAL_ID, 0x460, 0x0001);
	MISP_RegWrite(MISP_LOCAL_ID, 0x461, 0x0003);
	MISP_RegWrite(MISP_LOCAL_ID, 0x462, 0x0001);
	MISP_RegWrite(MISP_LOCAL_ID, 0x463, 0x0003);
	MISP_RegWrite(MISP_LOCAL_ID, 0x464, 0x01AE);
	MISP_RegWrite(MISP_LOCAL_ID, 0x465, 0x0100);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x466, 0x01CC);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x467, 0x0780);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x468, 0x0438);

	MISP_RegField(MISP_LOCAL_ID, 0x600, 6, 1, 0);				//disable R/B Comapansation

	}
	else{
	MISP_RegWrite(MISP_LOCAL_ID, 0x460, 0);
	MISP_RegField(MISP_LOCAL_ID, 0x600, 6, 1, 1);				//disable R/B Comapansation

	}
	
}

void  DEMO_HTMCurveOption(WORD nID)
{
	switch(nID){
		case 0: MISP_RegWrite(MISP_LOCAL_ID, 0x461, 0x0002); break;
		case 1: MISP_RegWrite(MISP_LOCAL_ID, 0x461, 0x0003); break;
		case 2: MISP_RegWrite(MISP_LOCAL_ID, 0x461, 0x000f); break;
		case 3: MISP_RegWrite(MISP_LOCAL_ID, 0x461, 0x000b); break;
	}
}
//----------------------------------------------------------------------------------------------------
#if 1  // do not delete this comment
void DEMO_SET_ADCCON(WORD nID)
{
	if(nID==0){
		//Power Down Mode
		*R_ADCCON=F_ADC_OFF;
	}
	else if(nID==1){
		//One time Send 
		*R_ADCCON = (F_ADC_ON| F_ADC_STC); 
	}
	else if(nID==2){
		//N time Send
		*R_ADCCON = (F_ADC_ON| F_ADC_NSTC); 			
	}
}

void DEMO_SET_ADCSAM(WORD nID)
{

	*R_ADCSAM = (F_ADCCLK_SEL_X(nID) | F_STC_4W);		


//	*R_ADCSAM = F_ADCCLK_SEL_0;	//default : 2 ADC Clock Width, ADC_Sampling Clock : off
//	*R_ADCSAM = (F_ADCCLK_SEL_4 | F_STC_4W);
//	*R_ADCSAM = F_ADCCLK_SEL_8;
//	*R_ADCSAM = F_ADCCLK_SEL_16;
//	*R_ADCSAM = F_ADCCLK_SEL_32;
//	*R_ADCSAM = F_ADCCLK_SEL_64;
//	*R_ADCSAM = F_ADCCLK_SEL_128;
//	*R_ADCSAM = F_ADCCLK_SEL_256;
//	*R_ADCSAM = (F_ADCCLK_SEL_512 | F_STC_4W);

	
}

void DEMO_SET_ADC_Channel(WORD nID)
{
	//ADC Channel Register 
	switch(nID){
		case 0  : *R_ADCCH = F_ADCCH0;	break;	
		case 1  : *R_ADCCH = F_ADCCH1;	break;	
		case 2  : *R_ADCCH = F_ADCCH2;	break;
		case 3  : *R_ADCCH = F_ADCCH3;	break;	
		case 4  : *R_ADCCH = F_ADCCH4;	break;	
		case 5  : *R_ADCCH = F_ADCCH5;	break;		
		case 6  : *R_ADCCH = F_ADCCH6;	break;	
		case 7  : *R_ADCCH = F_ADCCH7;	break;	
		case 8  : *R_ADCCH = F_ADCCH8;	break;
		case 9  : *R_ADCCH = F_ADCCH9;	break;	
		case 10 : *R_ADCCH = F_ADCCH10;	break;	
		case 11 : *R_ADCCH = F_ADCCH11;	break;			
		case 12 : *R_ADCCH = F_ADCCH12;	break;	
		case 13 : *R_ADCCH = F_ADCCH13;	break;				
		case 14 : *R_ADCCH = F_ADCCH14;	break;			
		case 15 : *R_ADCCH = F_ADCCH15;	break;	
		case 16 : *R_ADCCH = F_ADCCH16;	break;			
	}
}

BOOL DEMO_ADC_EOC_Check(void)
{
	DWORD TEMP=0;
	WORD EOC_DATA=0;
	
	TEMP=*R_ADCCON;
	
	EOC_DATA=((WORD)TEMP)&0x0010;

	if(EOC_DATA>1) return 1;
	else  return 0;
}

WORD DEMO_ADC_Data_Read(BOOL EOC)
{
	DWORD DATA;
	WORD  A_DATA;

	if(EOC==OFF) { UARTprintf("return \n\r ");	return 0; }

	DATA = *R_ADCDATA;	
	A_DATA = (((WORD)DATA)&0x03ff);
		
	UARTprintf("\n\r ADC_Data_Read : (%d) \n\r ", A_DATA);	
	
	return A_DATA;

	
}


#endif

//----------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void DEMO_SetInputPattern(WORD nID)
{
	BOOL	in_test_rgb=1;			//0:YCbCr, 1:RGB
	BOOL	in_test_main_en=0, in_test_aux_en=0;		//0:OFF, 1:ON
	WORD	in_test_ptrn_aux=0, in_test_ptrn=0;
	WORD	in_test_format_aux=0;		//aux:480p, main:1080i
	WORD	htot_m = GetIN_MainSrcSizeHtot(),	hact_m = GetIN_MainSrcSizeH(),	vact_m = GetIN_MainSrcSizeV();	// input video
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveInPattern(nID);
#endif
	
	if(nID)	{
#if defined(__MDIN_i550_USB_V100__)
		if(nID<8)	{in_test_aux_en=1;	in_test_main_en=0;	in_test_ptrn_aux=nID;	in_test_format_aux=3;}
#else
		if(nID>9)	{in_test_aux_en=1;	in_test_main_en=0;	in_test_ptrn_aux=(nID-9);}	//aux test pattern
		else		{in_test_aux_en=0;	in_test_main_en=1;	in_test_ptrn=nID;}			//main test pattern
#endif
	}

	MISP_RegField(MISP_LOCAL_ID, 0x042, 6, 2, in_test_main_en*2+in_test_aux_en);

	if(in_test_main_en){		//set main
		MISP_RegField(MISP_LOCAL_ID, 0x280, 5, 1, 0);	//0:tp_prog, 1:tp_interlace	
		MISP_RegField(MISP_LOCAL_ID, 0x280, 4, 1, in_test_rgb);
		MISP_RegField(MISP_LOCAL_ID, 0x280, 0, 4, in_test_ptrn);
	}
	
	if(in_test_aux_en){			//set aux
		MISP_RegField(MISP_LOCAL_ID, 0x042, 10, 5, in_test_ptrn_aux*4 + in_test_format_aux);	
		MISP_RegField(MISP_LOCAL_ID, 0x042, 15, 1, in_test_rgb);
	}

	MISP_RegWrite(MISP_LOCAL_ID, 0x270, htot_m); 			//in_tp_h_total
	MISP_RegWrite(MISP_LOCAL_ID, 0x272, 160); 			//in_tpp_h_active_start
	MISP_RegWrite(MISP_LOCAL_ID, 0x273, 160+hact_m); 	//in_tpp_h_active_end

	if(vact_m>1080)	MISP_RegWrite(MISP_LOCAL_ID, 0x271, 2250); 	//in_tp_v_total
	else		MISP_RegWrite(MISP_LOCAL_ID, 0x271, 1125); 			//in_tp_v_total
	MISP_RegWrite(MISP_LOCAL_ID, 0x274, 42); 						//in_tpp_v_active_start
	MISP_RegWrite(MISP_LOCAL_ID, 0x275, 42+vact_m); 				//in_tpp_v_active_end	
	
	//------------------------------------
	//moving test pattern setting
	if(in_test_ptrn==8){
	MISP_RegWrite(MISP_LOCAL_ID, 0x276, 100); 
	MISP_RegWrite(MISP_LOCAL_ID, 0x277, 100); 
	MISP_RegWrite(MISP_LOCAL_ID, 0x27B, 490); 	
	MISP_RegWrite(MISP_LOCAL_ID, 0x27E, 1); 	
	
	}

	if(in_test_ptrn==9){
	MISP_RegWrite(MISP_LOCAL_ID, 0x276, 50); 
	MISP_RegWrite(MISP_LOCAL_ID, 0x277, 50); 
	MISP_RegWrite(MISP_LOCAL_ID, 0x27B, 0); 	
	MISP_RegWrite(MISP_LOCAL_ID, 0x27E, 1); 	
	MISP_RegWrite(MISP_LOCAL_ID, 0x27F, 1); 			

	}
	//------------------------------------

		

	
}

//--------------------------------------------------------------------------------------------------
void DEMO_SetBNRCtrl(WORD nID)
{
	MISP_RegField(MISP_LOCAL_ID, 0x213, 14, 1, (nID)? 1: 0);			// 'r_bnr_en'
	
#if __MISP100_DBGPRT__ == 1
	UARTprintf("DEMO_SetBNRCtrl(%s), ", (nID)? "ON": "OFF");
#endif
}
//--------------------------------------------------------------------------------------------------
void DEMO_SetCompressCtrl(WORD nID)	
{
	if(nID)  SetWDR_CompressCtrl(MISP_COMP_ON);
	else     SetWDR_CompressCtrl(MISP_COMP_OFF);

#if __MISP100_DBGPRT__ == 1
	UARTprintf("DEMO_SetCompressCtrl(%s), ", (nID)? "ON": "OFF");
#endif
}
//--------------------------------------------------------------------------------------------------
void DEMO_SetCompressDefault(WORD nID)
{
	if(nID)  SetWDR_CompressDefault(ON);  //force_def_quant
	else     SetWDR_CompressDefault(OFF);

#if __MISP100_DBGPRT__ == 1
	UARTprintf("DEMO_SetCompressDefault(%s), ", (nID)? "ON": "OFF");
#endif
}
//--------------------------------------------------------------------------------------------------
void DEMO_SetCompressMode(WORD nID)		//0:80%, 1:60%
{
	switch(nID) {
		case 0:  SetWDR_CompressMode(MISP_COMP_QUANT_80);  break;
		default: SetWDR_CompressMode(MISP_COMP_QUANT_60);  break;
	}

#if __MISP100_DBGPRT__ == 1
	UARTprintf("DEMO_SetCompressMode(%s), ", (nID)? "60%": "80%");
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
#if defined(__SENSOR_ERR_CHECK__)
void sensor_check_print(void)
{
	WORD temp1,temp2;

	if(sensor_err_en==OFF) return;

//	static WORD sensor_err_tot_hong=0;

	//wait_cnt
	if(sensor_wait_cnt<5){ 
		sensor_wait_cnt++; 
		return; 
	}

	//part A : song's
	MISP_RegRead(MISP_LOCAL_ID, 0x57C, &temp1);
	MISP_RegRead(MISP_LOCAL_ID, 0x57D, &temp2);

	if((temp1&0x2000)>0){

		sensor_wait_cnt = 0;
		sensor_h_err_song ++;
	
		MISP_RegField(MISP_LOCAL_ID, 0x57A, 14, 1, 0);  		//Song's restart	
		MISP_RegField(MISP_LOCAL_ID, 0x57A, 14, 1, 1);  		//Song's restart	
		
		UARTprintf("num : %d <hsync_err> [herr_cnt] : %d [hpixel point] : %d \r\n", sensor_vcnt, sensor_h_err_song, temp1&0x1fff);
	}

	if((temp2&0x1000)>0){

		sensor_wait_cnt = 0;
		sensor_v_err_song ++;
	
		MISP_RegField(MISP_LOCAL_ID, 0x57A, 14, 1, 0);  		//Song's restart	
		MISP_RegField(MISP_LOCAL_ID, 0x57A, 14, 1, 1);  		//Song's restart	
		
		UARTprintf("num : %d <hsync_err> [verr_cnt] : %d [vpixel point] : %d \r\n", sensor_vcnt, sensor_v_err_song, temp2&0x0fff);
	}
	
}

void sensor_check_print2(void)
{
	WORD temp1,temp2;
	WORD temp1val, temp2val;


	if(sensor_err_en==OFF) return;

	MISP_RegRead(MISP_LOCAL_ID, 0x56F, &temp1);
	MISP_RegRead(MISP_LOCAL_ID, 0x57E, &temp2);

	temp1=temp1&0xFFF0;
	temp2=temp2&0x0FFF;

	if(temp1!=pre_sensor_verr){
		UARTprintf("<v_err_cnt> : %d  ", temp1);

		MISP_RegRead(MISP_LOCAL_ID, 0x569, &temp1val);
		UARTprintf("<v_err_val> : %d  \r\n", temp1val&0x0FFF);		
	}

	if(temp2!=pre_sensor_herr){
		UARTprintf("<h_err_cnt> : %d  ", temp2);

		MISP_RegRead(MISP_LOCAL_ID, 0x568, &temp2val);
		UARTprintf("<h_err_val> : %d  \r\n", temp2val&0x1FFF);		
	}	
	
	pre_sensor_verr=temp1;
	pre_sensor_herr=temp2;

	
/*
	if(frame_cnt == 255)
		frame_cnt = 0;
	else
		frame_cnt ++;
	


	if(((temp1&0x1FFF)!=0)||((temp2&0x0FFF)!=0)){
		sensor_err_hong ++;

		UARTprintf("[frame_cnt] : %d ", frame_cnt );
		UARTprintf("[tot_cnt] : %d ", sensor_err_hong );
		UARTprintf("[bits_phase] : %d ", (temp1&0x2000>0) ? 1 : 0);
		UARTprintf("<sof_err> : %d  ", (temp2&0x8000>0) ? 1 : 0);
		UARTprintf("<sol_err> : %d  ", (temp2&0x4000>0) ? 1 : 0);
		UARTprintf("<eof_err> : %d  ", (temp2&0x2000>0) ? 1 : 0);
		UARTprintf("<eol_err> : %d  ", (temp2&0x1000>0) ? 1 : 0);
		UARTprintf("<h_err_val> : %d  ", temp1&0x1FFF);
		UARTprintf("<v_err_val> : %d  \r\n", temp2&0x0FFF);

		//order logic clear & restart
//		MISP_RegField(MISP_LOCAL_ID, 0x567, 15, 1, 1);  		// clear on
//		Delay_mSec(10);
//		MISP_RegField(MISP_LOCAL_ID, 0x567, 15, 1, 0);  		// clear off
	}
*/	
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
void sensor_check_osd(void)
{
	WORD v_size_err_cnt, h_size_err_cnt;
	
	MISP_RegRead(MISP_LOCAL_ID, 0x56F, &v_size_err_cnt);
	MISP_RegRead(MISP_LOCAL_ID, 0x57E, &h_size_err_cnt);
	
	v_size_err_cnt &= 0xFFF0;
	h_size_err_cnt &= 0x0FFF;
	
	if((v_size_err_cnt != sensor_vcnt_old) || (h_size_err_cnt != sensor_hcnt_old))	{
		sensor_vcnt_old = v_size_err_cnt;
		sensor_hcnt_old = h_size_err_cnt;
		
		if(sensor_err_flag == 0)	{	// sync ok -> no sync
			SetOUT_MainFreezeEN(ON);
			OSD_IrkeyTitleDisplay(ON, "SS No Sync");
			sensor_err_flag = 1;
			
			//UARTprintf("sync error \r\n");
		}
	}
	else	{
		if(sensor_err_flag == 1)	{	// no sync -> sync ok
			SetOUT_MainFreezeEN(OFF);
			OSD_IrkeyTitleDisplay(OFF, NULL);
			sensor_err_flag = 0;
			
			//UARTprintf("sync ok \r\n");
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SetMainSource(BOOL src)
{
	BYTE menu_con;
	BYTE menu_sat;

	MainSrc = src;
	
	if (MainSrc==1){	
		SetIN_MainSrcExt();	
		
		// Sensor Auto contrast saturation adjust stop.		
		AutoSat_Level = OFF ; 
		AutoCon_OnOff = OFF ;
		menu_con = GetMenuStatus(CONTRAST_MENU);
		menu_sat = GetMenuStatus(SATURATION_MENU);	
		SetOUT_MainContrast((12*menu_con+8));
//		SetOUT_MainContrast((12*menu_con+8));
		SetOUT_MainSaturation((12*menu_sat+8));
		
		SetNR_LetzteresfilterEN(0,0) ; // disable sensor base nr_flt control
	} // hdmi
	else{	
		SetIN_MainSrcSensor();	
		AutoSat_Level = AutoSat_Init ;

		if(AutoCon_Enable) AutoCon_OnOff = ON;
		else AutoCon_OnOff = OFF;
		
#if defined(__USE_MDIN_i500__)			// MDIN-i500
		SetNR_LetzteresfilterEN(0,0) ; // enable sensor base nr_flt control
#else   								// MDIN-i51X (i510, i540, i550)
		SetNR_LetzteresfilterEN(1,1) ;  // enable sensor base nr_flt control
#endif
	} // sensor
		 
	//SetNR_Inputimage_type(src) ;
	
	SetDZOOM_Window(MIN(GetFC_MainFCsrcSizeH(), GetFC_MainFCdstSizeH()), GetFC_MainFCsrcSizeV());
}

//------------------------------------------------------------------------------------------
void SetPIPSource(BOOL src)
{
	PIPSrc = src;
	
	if(PIPSrc==1)		SetIN_AuxSrcExt();		// hdmi
	else				SetIN_AuxSrcSensor();	// sensor
}

//-------------------------------------------------------------------------------------------------------------------------
void SetToneMappingOnoff(BOOL OnOff)
{
#if defined(__USE_CMOS_IMXx36__)
	WORD vstart;

	if(OnOff){		//full hd, ttl 30fs settiing and full hd, lvds 30fs, setting
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY	

		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x020C, 0x02);		// WDMODE[1:0]
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x020F, 0x05);		// WDC_CMPEN[2]
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0210, 0x00);		// 
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0212, 0x2D);		// 
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0: Operation,  1: STANDBY

		//kamukim's ae logic stop
		stAECTRL.ctrl &= (~AECTRL_PROCESS_OFF);
		stAECTRL.ctrl |= (AECTRL_PROCESS_OFF);

		//black level bypass
		MISP_RegWrite(MISP_LOCAL_ID, 0x0581,0x9100);
		MISP_RegWrite(MISP_LOCAL_ID, 0x0582,0);

		MISP_RegRead(MISP_LOCAL_ID, 0x599, &vstart);
		MISP_RegWrite(MISP_LOCAL_ID, 0x599,vstart-1);
		
		MISP_RegField(MISP_LOCAL_ID, 0x0463, 7, 1, 1);		
		MISP_RegField(MISP_LOCAL_ID, 0x0563, 2, 3, 2);			//bayer map mode htm
		
		
	}
	else{
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY			
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x020C, 0x00);		// WDMODE[1:0]		
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x020F, 0x01);		//[0] is fixed '1'		
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0210, 0x01);	
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0212, 0x00);				
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0: Operation,  1: STANDBY

		//kamukim's ae logic run
		stAECTRL.ctrl &= (~AECTRL_PROCESS_OFF);
		stAECTRL.ctrl |= (AECTRL_PROCESS_RUN);	

		//black level redo
		MISP_RegWrite(MISP_LOCAL_ID, 0x0581,0xB100);
		MISP_RegWrite(MISP_LOCAL_ID, 0x0582,256);

		MISP_RegRead(MISP_LOCAL_ID, 0x599, &vstart);
		MISP_RegWrite(MISP_LOCAL_ID, 0x599,vstart+1);		

		MISP_RegField(MISP_LOCAL_ID, 0x0463, 7, 1, 0);				
		MISP_RegField(MISP_LOCAL_ID, 0x0563, 2, 3, 3);			//bayer map mode nomal
		
		
	}
		
#endif		
}

//--------------------------------------------------------------------------------------------------
void LowLight_AutoGamma(BOOL OnOff)
{
	LONG get_Ysum ;
	LONG get_Ytarget ;
	LONG cal_Ydistance ;

	if(OnOff)		return;
	
	get_Ysum = Get_AE_Ysum() ;
	get_Ytarget = AECTRL_GetBrightness() ;

	if(get_Ysum>=get_Ytarget) cal_Ydistance = get_Ysum - get_Ytarget;
	
	//UARTprintf("Y_sum = %d Y_target = %d  Y_distance = %d  \r\n", get_Ysum, get_Ytarget, cal_Ydistance);

	
}

//--------------------------------------------------------------------------------------------------
void LowLight_CSP(void)
{
	static BYTE csp_ls_x0 = 0 ;
	static BYTE csp_ls_x1 = 4 ;

	static BYTE x0_target = 0 ;
	static BYTE x1_target = 4 ;	
	
	LONG gain ;
	LONG dgain ;
	LONG max_gain ;
	LONG gain_factor ;

	WORD stdev_int ;
	WORD stdev_frac ;
	WORD pre_stdev_int ;
	WORD pre_stdev_frac ;		
	
	LONG gain_Ysum, gain_Yref ;	
	LONG GainCTLMode ;
	
	gain_Yref =  Get_AE_Yref();	
	gain_Ysum = Get_AE_Ysum() ;
	gain = AECTRL_GetGAINGain() ;
	dgain = Get_AE_DGain() ;
	max_gain = AECTRL_GetGAINPMax() ;
	GetNR_StDEV(&pre_stdev_int,&pre_stdev_frac,1); // From struct
	GetNR_StDEV(&stdev_int,&stdev_frac,0); // From reg

#if defined(__USE_IMX290_HCG_MODE__) || defined(__USE_IMX385_HCG_MODE__)
	if(AutoSat_Level==LowLight_Low) 	gain_factor = 10000; // 45dB = 13824 
	else if(AutoSat_Level==LowLight_Mid) 	gain_factor = 6000; // 35dB = 6144
	// else if(AutoSat_Level==LowLight_High) 	gain_factor = 4000; //30dB = 4096
	else gain_factor = 4000; //30dB = 4096
#elif defined(__USE_CMOS_IMX174__) || defined(__USE_CMOS_IMX249__) || defined(__USE_CMOS_IMX265__) || defined(__USE_CMOS_IMX252__) || defined(__USE_CMOS_IMX253__) 
	if(AutoSat_Level==LowLight_Low) 	gain_factor = 10000;
	else if(AutoSat_Level==LowLight_Mid) 	gain_factor = 6000;
	// else if(AutoSat_Level==LowLight_High) 	gain_factor = 3000;
	else gain_factor = 3000;
#else
	if(AutoSat_Level==LowLight_Low) 	gain_factor = max_gain/4;
	else if(AutoSat_Level==LowLight_Mid) 	gain_factor = max_gain/8;
	// else if(AutoSat_Level==LowLight_High) 	gain_factor = max_gain/16;
	else gain_factor = max_gain/16;
#endif	
	
	if ( gain == max_gain && max_gain == 256 )	GainCTLMode = AECTRL_GAINCTL_OFF ;
	else										GainCTLMode = AECTRL_GAINCTL_MAX ;	

	if ( WBPreset_cnt_0 == 5 )
	{ 		
		if ((GainCTLMode == AECTRL_GAINCTL_OFF) || (AutoSat_Level ==LowLight_Off) ){
			x0_target = 0 ;		
			x1_target = 4 ;		
		}
		else{
			if (stdev_frac >= 4) stdev_int++ ;
			if (pre_stdev_frac >= 4) pre_stdev_int++ ;			
			
			if ( (gain >= max_gain) || (dgain > 256)){ // gain max 
				if ((pre_stdev_int >= stdev_int)){ // When scene is not under exposed
				#if defined(__USE_CMOS_IMX274__) ||defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX326__)
					x0_target = stdev_int + (stdev_int>>1) ; 
					x1_target = stdev_int + x0_target ; 				
				#else
					x0_target = stdev_int>>2 ; 
					x1_target = stdev_int>>1 ; 
				#endif
				}
			}
			else if ( gain > gain_factor ) {						
				#if defined(__USE_CMOS_IMX274__) ||defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX326__)
					if (stdev_int >= 5)
					{
						x0_target = stdev_int ; 
						x1_target = stdev_int + x0_target ;
					}
					else
					{
						x0_target = stdev_int>>1 ; 
						x1_target = stdev_int + x0_target ;		
					}				
				#else
					if (stdev_int >= 5)
					{
						x0_target = stdev_int>>1 ; 
						x1_target = stdev_int + x0_target ;
					}
					else
					{
						x0_target = 0 ;
						x1_target = stdev_int ; 		
					}
				#endif
			}			
		}
		// UARTprintf("[Ysum:%d], [Get_AE_Ysum():%d], [Yref:%d]\r\n", gain_Ysum ,Get_AE_Ysum(), Get_AE_Yref() ) ;
		// UARTprintf("[Gain:%d], [dGain:%d], [Gain_factor:%d] [MaxGain:%d]\r\n", gain , dgain, gain_factor, max_gain) ;
		// UARTprintf("[x0_target:%d], [x1_target:%d]\r\n", x0_target ,x1_target) ;			
		// UARTprintf("[pstdev:%d], [pfrac:%d]\r\n", pre_stdev_int ,pre_stdev_frac) ;			
		// UARTprintf("[stdev:%d],  [frac:%d]\r\n", stdev_int ,stdev_frac) ;			
	}

	if ( csp_ls_x0 < x0_target )			csp_ls_x0++ ;
	else if ( csp_ls_x0 > x0_target )		csp_ls_x0-- ;	
	
	if ( csp_ls_x1 < x1_target )			csp_ls_x1++ ;
	else if ( csp_ls_x1 > x1_target )		csp_ls_x1-- ;	
	
	if ((GainCTLMode == AECTRL_GAINCTL_OFF) || (AutoSat_Level ==LowLight_Off)){
		if ( (csp_ls_x0 != x0_target) || (csp_ls_x1 != x1_target) ){
			SetCSP_LS_P0(csp_ls_x0);
			SetCSP_LS_P1(csp_ls_x1);			
		}		
	}
	else{
		SetCSP_LS_P0(csp_ls_x0);
		SetCSP_LS_P1(csp_ls_x1);		
	}	
}
//--------------------------------------------------------------------------------------------------
void LowLight_AutoSat(BYTE nID)
{
	LONG gain ;
	LONG dgain ;
	LONG max_gain ;
	LONG gain_factor ;

	WORD stdev_int ;
	WORD stdev_frac ;
	WORD pre_stdev_int ;
	WORD pre_stdev_frac ;	
	
	BYTE menu_sat;
	static BYTE pmenu_sat;
	BYTE AutoSat_power;	
	
	LONG gain_Ysum, gain_Yref ;
	LONG GainCTLMode ;
	
	
	menu_sat = GetMenuStatus(SATURATION_MENU);	
	if ( menu_sat != pmenu_sat )
	{		
		pmenu_sat = menu_sat ;
		target_sat = menu_sat*12 + 8 ;
	}
	
	if (AutoSat_Level==LowLight_Off ) 
	{
		if ( WBPreset_cnt_0 == 0 ) WBPreset_cnt_0 = 5 ;
		else WBPreset_cnt_0-- ;
		return ;
	}
	else if(AutoSat_Level==LowLight_Low) AutoSat_power=6;
	else if(AutoSat_Level==LowLight_Mid) AutoSat_power=4;	
	else if(AutoSat_Level==LowLight_High) AutoSat_power=2; 
	else AutoSat_power = 0 ; 
	
	gain_Yref =  Get_AE_Yref();	
	gain_Ysum = Get_AE_Ysum() ;
	gain =  AECTRL_GetGAINGain() ;
	dgain = Get_AE_DGain() ;
	max_gain = AECTRL_GetGAINPMax() ;
	GetNR_StDEV(&pre_stdev_int,&pre_stdev_frac,1);
	GetNR_StDEV(&stdev_int,&stdev_frac,0);

#if defined(__USE_IMX290_HCG_MODE__) || defined(__USE_IMX385_HCG_MODE__)
	if(AutoSat_Level==LowLight_Low) 	gain_factor = 10000; // 45dB = 13824 
	else if(AutoSat_Level==LowLight_Mid) 	gain_factor = 6000; // 35dB = 6144
	// else if(AutoSat_Level==LowLight_High) 	gain_factor = 4000; //30dB = 4096
	else gain_factor = 4000; //30dB = 4096
#elif defined(__USE_CMOS_IMX174__) || defined(__USE_CMOS_IMX249__) || defined(__USE_CMOS_IMX265__) || defined(__USE_CMOS_IMX252__) || defined(__USE_CMOS_IMX253__)
	if(AutoSat_Level==LowLight_Low) 	gain_factor = 10000;
	else if(AutoSat_Level==LowLight_Mid) 	gain_factor = 6000;
	// else if(AutoSat_Level==LowLight_High) 	gain_factor = 3000;
	else gain_factor = 3000;
#else
	if(AutoSat_Level==LowLight_Low) 	gain_factor = max_gain/4;
	else if(AutoSat_Level==LowLight_Mid) 	gain_factor = max_gain/8;
	// else if(AutoSat_Level==LowLight_High) 	gain_factor = max_gain/16;
	else gain_factor = max_gain/16;
#endif

	if ( gain == max_gain && max_gain == 256 )	GainCTLMode = AECTRL_GAINCTL_OFF ;
	else										GainCTLMode = AECTRL_GAINCTL_MAX ;

	//CAL sat_compensation
	if (GainCTLMode == AECTRL_GAINCTL_OFF ) 	sat_compensation = 0  ;
	else{
		if ( (gain >= max_gain) || (dgain > 256) ){ // gain max
			if (stdev_frac >= 4) stdev_int++ ;
			if (pre_stdev_frac >= 4) pre_stdev_int++ ;	
			
			if ((pre_stdev_int >= stdev_int)) // when scene is not under exposed
			{
			#if defined(__USE_CMOS_IMX185__) || defined(__USE_CMOS_IMX385__)
				if( stdev_int > AutoSat_power)	sat_compensation = stdev_int - AutoSat_power;
				else				sat_compensation = 0;
			#elif defined(__USE_IMX290_HCG_MODE__)
				if( stdev_int > AutoSat_power+2)	sat_compensation = stdev_int - AutoSat_power -3;
				else				sat_compensation = 0;
			#elif defined(__USE_CMOS_IMX274__) ||defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX326__)
				if( pre_stdev_int > ((AutoSat_power+1)))	sat_compensation = stdev_int - (AutoSat_power+1) -1 ;
				else				sat_compensation = 0;				
			#else
				if ( pre_stdev_int > 8 ) sat_compensation = pre_stdev_int - AutoSat_power ;
				else sat_compensation = 0;						
			#endif
			}
		}
		else if ( gain > gain_factor ) {
			#if defined(__USE_CMOS_IMX185__) || defined(__USE_CMOS_IMX385__)
				if( stdev_int > AutoSat_power)	sat_compensation = stdev_int - AutoSat_power;
				else				sat_compensation = 0;
			#elif defined(__USE_IMX290_HCG_MODE__) 
				if( stdev_int > AutoSat_power+2)	sat_compensation = stdev_int - AutoSat_power -3;
				else				sat_compensation = 0;	
			#elif defined(__USE_CMOS_IMX274__) ||defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX326__)
				// sat_compensation = 0;
				if( stdev_int > ((AutoSat_power+1)))	sat_compensation = stdev_int - (AutoSat_power+1) -1 ;
			#else
				if ( stdev_int > 8 ) sat_compensation = stdev_int - AutoSat_power ;
				else sat_compensation = 0;		
			#endif
		}
		else sat_compensation = 0;
	}

	//CAL sat_bound(to target_sat..)
	if ( WBPreset_cnt_0 == 0 )
	{ 
		if (gain == max_gain && GainCTLMode != AECTRL_GAINCTL_OFF ) {
			//When, AGC max && insufficient Ysum
			if(gain_Yref*0.2f>gain_Ysum)	  sat_bound = MAX(0, menu_sat - MIN(menu_sat,sat_compensation + 5));	
			else if(gain_Yref*0.3f>gain_Ysum) sat_bound = MAX(0, menu_sat - MIN(menu_sat,sat_compensation + 4));		
			else if(gain_Yref*0.4f>gain_Ysum) sat_bound = MAX(0, menu_sat - MIN(menu_sat,sat_compensation + 3));
			else if(gain_Yref*0.6f>gain_Ysum) sat_bound = MAX(0, menu_sat - MIN(menu_sat,sat_compensation + 2));	
			else if(gain_Yref*0.8f>gain_Ysum) sat_bound = MAX(0, menu_sat - MIN(menu_sat,sat_compensation + 1));				
			else sat_bound = MAX(0, menu_sat - sat_compensation);			
		}
		else {			
			//When, AGC gain factor~AGC Max
			if ( gain > gain_factor ) 		sat_bound = MAX(0, menu_sat - sat_compensation);				
			else 						sat_bound = menu_sat ;			
		}

		//auto saturation low limit and transform(x*12+8) value
		if ( menu_sat > 2 )		sat_bound = MAX(2*12+8, sat_bound*12+8);			
		else					sat_bound = menu_sat*12+8 ;
				
		WBPreset_cnt_0 = 5 ;			 
		
		// UARTprintf("[Ysum:%d], [Get_AE_Ysum():%d], [Yref:%d]\r\n", gain_Ysum ,Get_AE_Ysum(), Get_AE_Yref() ) ;
		// UARTprintf("[Gain:%d], [dGain:%d], [Gain_factor:%d] [MaxGain:%d]\r\n", gain, dgain, gain_factor, max_gain) ;
		// UARTprintf("[target_sat:%d], [sat_compensation:%d]\r\n", target_sat,sat_compensation) ;	
		// UARTprintf("[pstdev:%d], [pfrac:%d]\r\n", pre_stdev_int ,pre_stdev_frac) ;			
		// UARTprintf("[stdev:%d],  [frac:%d]\r\n", stdev_int ,stdev_frac) ;			
	}
	else 	WBPreset_cnt_0-- ;	

	if ( target_sat < sat_bound )			target_sat++ ;
	else if ( target_sat > sat_bound )		target_sat-- ;


	//b. adjust fix
	if(target_sat != p_target_sat)		{
		// SetOUT_MainSaturation(target_sat);	// temp test
		// SetOUT_AuxSaturation(target_sat);  	// temp test
		p_target_sat=target_sat;	
	}
}
//--------------------------------------------------------------------------------------------------
void LowLight_AutoEdge(BOOL OnOff)
{
	LONG gain ;
	LONG dgain ;
	LONG max_gain ;
	#if defined(__USE_IMX290_HCG_MODE__) || defined(__USE_CMOS_IMX185__)
	//only use HCG mode
	// LONG gain_factor = 4000 ;			//30dB = 4096
	LONG gain_factor = 2800 ;				//27dB = 2816
	#else
	LONG gain_factor = 256*90 ;			//23040
	#endif	

	WORD stdev_int=0;
	WORD stdev_frac = 0;
	
	// WORD pre_stdev_int=0;
	// WORD pre_stdev_frac = 0;	
	
	BYTE menu_edge;
	static BYTE pmenu_edge;

	LONG gain_Ysum ;
	LONG GainCTLMode ;
	
	menu_edge = GetMenuStatus(EDGE_MENU);	
	if ( menu_edge != pmenu_edge )
	{
		pmenu_edge = menu_edge ;
		target_edge = menu_edge ;	
	}
	
	if ( AutoEdge_OnOff==OFF ) return ;
	
	gain_Ysum = Get_AE_Ysum() ;
	gain =  AECTRL_GetGAINGain() ;
	dgain = Get_AE_DGain() ;
	max_gain = AECTRL_GetGAINPMax() ;

	// GetNR_StDEV(&pre_stdev_int,&pre_stdev_frac,1);
	GetNR_StDEV(&stdev_int,&stdev_frac,0);

	if ( stdev_frac >3 ) stdev_int = stdev_int +1 ;
	
	// GainCTLMode = AECTRL_GetGainCTLMode() ;	
	if ( gain == max_gain && max_gain == 256 )	GainCTLMode = AECTRL_GAINCTL_OFF ;
	else										GainCTLMode = AECTRL_GAINCTL_MAX ;
	

	if (GainCTLMode == AECTRL_GAINCTL_OFF ) edge_compensation = 0  ;
	else{
		if ( (gain >= max_gain) || (dgain > 256)){
			#if defined(__USE_CMOS_IMX185__)
				if( stdev_int > 7)	edge_compensation = stdev_int + 2;			
				else if( stdev_int > 4)	edge_compensation = stdev_int + 1;			
				else if( stdev_int > 2)	edge_compensation = stdev_int ;
				else 			edge_compensation = 0;
			#elif defined(__USE_CMOS_IMX274__) ||defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX326__)
				if ( stdev_int > 10 ) edge_compensation = stdev_int - 10 ;
				else edge_compensation = 0;	
			#else
				if ( stdev_int > 7 ) edge_compensation = stdev_int - 5 ;
				else edge_compensation = 0;				
			#endif
		}
		else if ( gain > gain_factor ) {	
			#if defined(__USE_CMOS_IMX185__)	
				if( stdev_int > 7)	edge_compensation = stdev_int + 2;			
				else if( stdev_int > 4)	edge_compensation = stdev_int + 1;			
				else if( stdev_int > 2)	edge_compensation = stdev_int ;
				else 			edge_compensation = 0;
			#elif defined(__USE_CMOS_IMX274__) ||defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX326__)
				edge_compensation = 0;
			#else		
				if ( stdev_int > 7 ) edge_compensation = stdev_int - 5 ;
				else edge_compensation = 0;			
			#endif	
		}				
		else edge_compensation = 0;			
	}

	
	if ( WBPreset_cnt_0 == 1 )
	{ 	
		if (GainCTLMode != AECTRL_GAINCTL_OFF ) {		
			if ( menu_edge > edge_compensation )
			{
				edge_bound = menu_edge - edge_compensation ;
			}
			else
			{
				edge_bound = 0 ;
			}						
		}
		else {				
			edge_bound = menu_edge ;
		}	 
		
		 // UARTprintf("[Ysum:%d], [Get_AE_Ysum():%d], [Yref:%d]\r\n", gain_Ysum ,Get_AE_Ysum(), Get_AE_Yref() ) ;
		 // UARTprintf("[Gain:%d], [MaxGain:%d]\r\n", gain ,max_gain) ;
		 // UARTprintf("[target_edge:%d], [menu_edge:%d], [edge_compensation:%d]\r\n", target_edge ,menu_edge,edge_compensation) ;	
		 // UARTprintf("[p_target_edge:%d][edge_bound:%d], [stdev:%d]\r\n", p_target_edge, edge_bound, stdev_int) ;	

		if ( target_edge < edge_bound )		target_edge++ ;
		else if ( target_edge > edge_bound )	target_edge--;


		if(target_edge != p_target_edge)
		{
			// DEMO_SetSharpness(target_edge);	
		
			SetIN_MainHPFLevel(target_edge*6); // Avoid DEMO_SetSharpness() to preserve EEPROM value
			SetIN_MainHPFLDslope(target_edge*6) ; // Avoid DEMO_SetSharpness() to preserve EEPROM value		

			SetFC_MainVPFLevel(target_edge*6); // Avoid DEMO_SetSharpness() to preserve EEPROM value
			SetFC_MainVPFLDslope(target_edge*6) ; // Avoid DEMO_SetSharpness() to preserve EEPROM value		
			p_target_edge=target_edge;					
		}				 
	}
}
//-----------------------------------------------------------------------------
void LowLight_AutoCon(BOOL OnOff)
{
	//divid code auto contrast 15.11.10 (i500 FA)
	//This function should be call before LowLight_AutoSat func( WBPreset_cnt_0..)
	//function divid to debug image dynaymic range, wash out..
	LONG gain ;
	LONG max_gain ;
	
	BYTE menu_con;

	LONG gain_Ysum ;
	LONG GainCTLMode ;
	
	menu_con = GetMenuStatus(CONTRAST_MENU);


	if ( menu_con != p_menu_con )	target_con = menu_con*12 + 8 ;
	
	p_menu_con = menu_con ; 

	if ( AutoCon_OnOff==OFF ) return ;		
	
	gain_Ysum = Get_AE_Ysum() ;
	gain =  AECTRL_GetGAINGain() ;
	max_gain = AECTRL_GetGAINPMax() ;


	if ( WBPreset_cnt_0 == 2 )
	{ 
		if ( gain == max_gain && max_gain == 256 )	GainCTLMode = AECTRL_GAINCTL_OFF ;
		else										GainCTLMode = AECTRL_GAINCTL_MAX ;
	
		// UARTprintf("[Ysum:%d], [Get_AE_Ysum():%d], [Yref:%d]\r\n", gain_Ysum ,Get_AE_Ysum(), Get_AE_Yref() ) ;
		// UARTprintf("[Gain:%d], [MaxGain:%d]\r\n", gain ,max_gain) ;
		// UARTprintf("[target_con:%d] \r\n", target_con) ;		
		// UARTprintf("[stdev:%d], [frac:%d]\r\n", stdev_int ,stdev_frac) ;	
		
		if (gain == max_gain && GainCTLMode != AECTRL_GAINCTL_OFF ) {
			if ( gain_Ysum < 4000 )	con_bound = menu_con + 7 ;			
			else if ( gain_Ysum < 6500 ) con_bound = menu_con + 5 ;
			else if ( gain_Ysum < 9000 ) con_bound = menu_con + 3 ;
			else 						con_bound = menu_con + 1 ;
		}
		else {			
			if ( gain > 256*90 ) con_bound = menu_con ;		
			else 				con_bound = menu_con ;
		}

		if ( con_bound > 20 )	con_bound = 20*12+8 ;
		else				  	con_bound = con_bound*12+8 ;
	}

	if ( target_con < con_bound )		target_con++ ;
	else if ( target_con > con_bound )	target_con-- ;

	//b. adjust fix
	if(target_con != p_target_con)	
	{
		SetOUT_MainContrast(target_con);	
		p_target_con=target_con;
	}
}
void LowLight_AutoBrightness(BYTE nID)
{
	LONG gain ;
	LONG max_gain ;
	LONG gain_Ysum, gain_Yref ;
	LONG GainCTLMode ;
	
	BYTE menu_bright;
	BYTE brightness_power=0;

	if(nID==LowLight_Off) return;
	else if(nID==LowLight_Low) brightness_power=1;
	else if(nID==LowLight_Mid) brightness_power=2;	
	else if(nID==LowLight_High) brightness_power=3;	
	
	menu_bright = GetMenuStatus(BRIGHTNESS_MENU);	
	gain_Ysum = Get_AE_Ysum() ;
	gain_Yref =  Get_AE_Yref();
	gain =  AECTRL_GetGAINGain() ;
	max_gain = AECTRL_GetGAINPMax() ;

	if ( gain == max_gain && max_gain == 256 )	GainCTLMode = AECTRL_GAINCTL_OFF ;
	else										GainCTLMode = AECTRL_GAINCTL_MAX ;


	//moving 0~max(8*brightness_power)
	if (GainCTLMode == AECTRL_GAINCTL_OFF ) bright_compensation = 0  ;
	else{
		if (gain == max_gain) {	
			if(gain_Yref*0.1f>gain_Ysum) bright_compensation = brightness_power*8;		
			else if(gain_Yref*0.2f>gain_Ysum) bright_compensation = brightness_power*7;		
			else if(gain_Yref*0.3f>gain_Ysum) bright_compensation = brightness_power*6;			
			else if(gain_Yref*0.4f>gain_Ysum) bright_compensation = brightness_power*5;			
			else if(gain_Yref*0.5f>gain_Ysum) bright_compensation = brightness_power*4;					
			else if(gain_Yref*0.6f>gain_Ysum) bright_compensation = brightness_power*3;			
			else if(gain_Yref*0.7f>gain_Ysum) bright_compensation = brightness_power*2;					
			else if(gain_Yref*0.8f>gain_Ysum) bright_compensation = brightness_power;				
			else bright_compensation = 0  ;
								
		}				
		else bright_compensation = 0;			
	}

	
	if ( WBPreset_cnt_0 == 3 )
	{ 	
		if ( target_bright < bright_compensation )			target_bright++ ;
		else if ( target_bright > bright_compensation )		target_bright--;


		if(target_bright == p_target_bright)	return;

		p_target_bright=target_bright;	

		DEMO_SetBrightness(menu_bright);
//		UARTprintf("[Ysum:%d], [Get_AE_Ysum():%d], [Yref:%d]\r\n", gain_Ysum ,Get_AE_Ysum(), Get_AE_Yref() ) ;
//		UARTprintf("[Gain:%d], [MaxGain:%d]\r\n", gain ,max_gain) ;
//		UARTprintf("[target_bright:%d], [bright_compensation:%d]\r\n", target_bright ,bright_compensation) ;	
//		UARTprintf("bright change \n");					
		 
	}

		

}
void LowLight_AutoDyanmicDPC(void)
{
	LONG gain ;
	LONG max_gain ;
	LONG gain_Ysum, gain_Yref ;
	LONG GainCTLMode ;
	

	if(GetDynamicDPC_Auto()==OFF) return;

	gain_Ysum = Get_AE_Ysum() ;
	gain_Yref =  Get_AE_Yref();
	gain =  AECTRL_GetGAINGain() ;
	max_gain = AECTRL_GetGAINPMax() ;

	if ( gain == max_gain && max_gain == 256 )	GainCTLMode = AECTRL_GAINCTL_OFF ;
	else										GainCTLMode = AECTRL_GAINCTL_MAX ;

	if (GainCTLMode == AECTRL_GAINCTL_OFF ) ddpc_compensation = 0  ;
	else{
#if defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX226__)		
		if (gain > 0x35000) {	
			if(gain_Yref*0.4f>gain_Ysum) ddpc_compensation = 7;		
			else if(gain_Yref*0.7f>gain_Ysum) ddpc_compensation = 6;							
			else ddpc_compensation = 5  ;								
		}	
		else if ( gain > 0x22000 ) ddpc_compensation = 5;					// Digital Gain Start > 0x20000
		else if ( gain > 0x12000 ) ddpc_compensation = 4;					
		else if ( gain > 0xB000) ddpc_compensation = 3;						
		else if ( gain > 0x4000) ddpc_compensation = 2;							
		else if ( gain > 0x1000) ddpc_compensation = 1;								
		else ddpc_compensation = 0;	
#elif defined(__USE_CMOS_IMX385__)	//temp setting
		if (gain > 0x35000) {	
			if(gain_Yref*0.4f>gain_Ysum) ddpc_compensation = 7;		
			else if(gain_Yref*0.7f>gain_Ysum) ddpc_compensation = 6;							
			else ddpc_compensation = 5  ;								
		}	
		else if ( gain > 0x22000 ) ddpc_compensation = 5;					// Digital Gain Start > 0x20000
		else if ( gain > 0x12000 ) ddpc_compensation = 4;							
		else if ( gain > 0xB000 ) ddpc_compensation = 3;					//
		else if ( gain > 0x3000 ) ddpc_compensation = 2;					//
		else if ( gain > 0x500) ddpc_compensation = 1;						//
		else ddpc_compensation = 0;			
#else
		if (gain == max_gain) {	
			if(gain_Yref*0.2f>gain_Ysum) ddpc_compensation = 7;		
			else if(gain_Yref*0.4f>gain_Ysum) ddpc_compensation = 6;			
			else if(gain_Yref*0.6f>gain_Ysum) ddpc_compensation = 5;					
			else if(gain_Yref*0.8f>gain_Ysum) ddpc_compensation = 4;				
			else ddpc_compensation = 3  ;								
		}	
		else if ( gain > (SDPD_AGC_LV3) ) ddpc_compensation = 3;				
		else if ( gain > (SDPD_AGC_LV2) ) ddpc_compensation = 2;			
		else if ( gain > (SDPD_AGC_LV1) ) ddpc_compensation = 1;	
		else ddpc_compensation = 0;	
#endif
	}

	if ( target_ddpc < ddpc_compensation )			target_ddpc++ ;
	else if ( target_ddpc > ddpc_compensation )		target_ddpc--;


	if(target_ddpc != p_target_ddpc)
	{
	//------------------------------------Parameter change------------------------------------------------
		p_target_ddpc=target_ddpc;	
		DynamicDPC_Process(target_ddpc);
	//------------------------------------Parameter change------------------------------------------------	

	//---------------------------------------For debug----------------------------------------------------
		if(GetDPC_DebugDisp()==1)
		{
			#if defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX226__)	
				if (gain > 0x35000) {	
					if(gain_Yref*0.4f>gain_Ysum) OSD_IrkeyTitleDisplay(ON, "DDPC PWR7 ");
					else if(gain_Yref*0.7f>gain_Ysum) OSD_IrkeyTitleDisplay(ON, "DDPC PWR6 ");				
					else OSD_IrkeyTitleDisplay(ON, "DDPC PWR5 ");									
				}	
				else if ( gain > 0x22000 ) OSD_IrkeyTitleDisplay(ON, "DDPC PWR5 ");				
				else if ( gain > 0x12000 ) OSD_IrkeyTitleDisplay(ON, "DDPC PWR4 ");		
				else if ( gain > 0xB000 ) OSD_IrkeyTitleDisplay(ON, "DDPC PWR3 ");
				else if ( gain > 0x4000 ) OSD_IrkeyTitleDisplay(ON, "DDPC PWR2 ");
				else if ( gain > 0x1000 ) OSD_IrkeyTitleDisplay(ON, "DDPC PWR1 ");				
				else OSD_IrkeyTitleDisplay(ON, "DDPC PWR0 ");		
			#elif defined(__USE_CMOS_IMX385__)	//temp setting
				if (gain > 0x35000) {	
					if(gain_Yref*0.4f>gain_Ysum) OSD_IrkeyTitleDisplay(ON, "DDPC PWR7 ");
					else if(gain_Yref*0.7f>gain_Ysum) OSD_IrkeyTitleDisplay(ON, "DDPC PWR6 ");				
					else OSD_IrkeyTitleDisplay(ON, "DDPC PWR5 ");									
				}	
				else if ( gain > 0x22000 ) OSD_IrkeyTitleDisplay(ON, "DDPC PWR5 ");				
				else if ( gain > 0x12000 ) OSD_IrkeyTitleDisplay(ON, "DDPC PWR4 ");		
				else if ( gain > 0xB000 ) OSD_IrkeyTitleDisplay(ON, "DDPC PWR3 ");
				else if ( gain > 0x3000 ) OSD_IrkeyTitleDisplay(ON, "DDPC PWR2 ");
				else if ( gain > 0x500 ) OSD_IrkeyTitleDisplay(ON, "DDPC PWR1 ");	
			#else
				if (gain == max_gain) {	
					if(gain_Yref*0.2f>gain_Ysum) OSD_IrkeyTitleDisplay(ON, "DDPC PWR5 ");
					else if(gain_Yref*0.4f>gain_Ysum) OSD_IrkeyTitleDisplay(ON, "DDPC PWR4 ");				
					else if(gain_Yref*0.6f>gain_Ysum) OSD_IrkeyTitleDisplay(ON, "DDPC PWR3 ");							
					else if(gain_Yref*0.8f>gain_Ysum) OSD_IrkeyTitleDisplay(ON, "DDPC PWR2 ");				
					else OSD_IrkeyTitleDisplay(ON, "DDPC PWR1 ");							
				}	
				else if ( gain > (SDPD_AGC_LV1) /2 ) OSD_IrkeyTitleDisplay(ON, "DDPC PWR1 ");	
				else OSD_IrkeyTitleDisplay(ON, "DDPC PWR0 ");	
			#endif

			UARTprintf("[Ysum:%d], [Get_AE_Ysum():%d], [Yref:%d]\r\n", gain_Ysum ,Get_AE_Ysum(), Get_AE_Yref() ) ;
			UARTprintf("[Gain:%d], [MaxGain:%d]\r\n", gain ,max_gain) ;	
			UARTprintf("dynamic dpc change \n");	
			
		}
	//---------------------------------------For debug----------------------------------------------------
	}
	
}
//-----------------------------------------------------------------------------
void CAC_prevent_escape(void)
{
	LONG gain ;
	LONG gain_Ysum, gain_Yref, Y_diff ;
	LONG AE_MODE;
	LONG exit_level;
	
	gain_Ysum = Get_AE_Ysum() ;
	gain_Yref =  Get_AE_Yref();
	gain =  AECTRL_GetGAINGain() ;
	Y_diff = Get_AE_YDIFF();
	AE_MODE=Get_AE_Mode();	

	if((AE_MODE && AECTRL_PRIORITY_MASK)==AECTRL_ALL_MANUAL)		exit_level=20000;
	else																exit_level=3000;	


	if(gain>0x20000){
		if(Get_CAC_Enable())		return;	
		else	 {	
			Set_CAC_Enable(ON);		//digital gain high region, enable return.
			return;		
		}		
	}

	if(gain_Ysum > gain_Yref){
		if(Y_diff>exit_level)	Set_CAC_Enable(OFF);
		else	{
			if(Get_CAC_Enable())		return;	
			else	 {	
				Set_CAC_Enable(ON);		
				return;		
			}			
		}
	}
	else	{
		if(Get_CAC_Enable())		return;	
		else	 {	
			Set_CAC_Enable(ON);		
			return;		
		}	
	}

	//--------------------debug message-------------------------------------------

/*
	if(AE_MODE==AECTRL_ALL_MANUAL)
		if(gain_Ysum > gain_Yref){
			if(Y_diff>exit_level)	OSD_IrkeyTitleDisplay(ON, "CAC MANUAL");	
			else					OSD_IrkeyTitleDisplay(OFF, "          ");	
		}
		else	{
			OSD_IrkeyTitleDisplay(OFF, "          ");	
		}
	else	{
		if(gain_Ysum > gain_Yref){
			if(Y_diff>exit_level)	OSD_IrkeyTitleDisplay(ON, "CAC ESCAPE");	
			else					OSD_IrkeyTitleDisplay(OFF, "          ");	
		}
		else	{
			OSD_IrkeyTitleDisplay(OFF, "          ");	
		}
	}
*/
	//-----------------------------------------------------------------------------

}
//-----------------------------------------------------------------------------
void AUTODefogCCtrl(void)
{
#if defined(__USE_MDIN_i500__)
	BYTE DefogMode=GetBY_DefogMode();
	WORD ColorCCtrl_Red=GetDM_ColorCCtrl_Red();
	WORD ColorCCtrl_Green=GetDM_ColorCCtrl_Green();	
	WORD ColorCCtrl_Blue=GetDM_ColorCCtrl_Blue();		
#endif

	defog_act_before = defog_act;
	defog_act = GetBY_DefogActive() ;	//auto : read register check, on : return on, off return off


	if(defog_act_before==defog_act) return;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	if(DefogMode==MISP_DEFOG_OFF){
		if(defog_act_before){
			// AE return
			DEMO_SetExpAGC(3);
			SetAEBrightLvl(6);
			SetMenuStatus(EXP_BRIGHT_MENU, 6);
			SetMenuStatus(EXP_AGCMAX_MENU, 3);			// high
			SetDM_ZipBypass(0);
			DEMO_SetSharpness(GetMenuStatus(EDGE_MENU));					
			// GM return
			SetBY_GammaValue(0);
			// Color return
			//VISCAAPP_ColorGain(GetMenuStatus(CC_MENU));
			//Defog auto parameter return
			defog_act_before=OFF;
			defog_act=OFF;
//			UARTprintf("defog reset sequence\r\n") ;				
//			UARTprintf("defog off\r\n") ;	
		}			
		return;
	}
	else{	
		if (defog_act==ON){									//auto or N
			if(defog_act_before==OFF){	
				DEMO_SetExpAGC(1);
				SetAEBrightLvl(7);
				SetMenuStatus(EXP_BRIGHT_MENU, 7);
				SetMenuStatus(EXP_AGCMAX_MENU, 1);			// low
				
				SetDM_ZipBypass(1);
				DEMO_SetSharpness(0);						//user additional sharpneess tuning..
			}
		}
		else{
			if(defog_act_before==ON){				
				DEMO_SetExpAGC(3);
				SetAEBrightLvl(6);
				SetMenuStatus(EXP_BRIGHT_MENU, 6);
				SetMenuStatus(EXP_AGCMAX_MENU, 3);			// high
				
				SetDM_ZipBypass(0);
				DEMO_SetSharpness(GetMenuStatus(EDGE_MENU));		
			}
		}
	}
	//--------exposure ctrl end----------//

	#ifndef __USE_IMX290_HCG_MODE__
	if (defog_act>0)			SetBY_gamma_value(17);
	else						SetBY_GammaValue(0);		//function difference. naming will be change
	#endif
	
	//--------gm ctrl end--------------//	
	if (defog_act>0){
		if(ColorCCtrl_Red>5){
			SetDM_ColorCCtrl_Red(ColorCCtrl_Red-5);	
		}
		else {
			SetDM_ColorCCtrl_Red(0);	
		}	

		if(ColorCCtrl_Green>5){
			SetDM_ColorCCtrl_Green(ColorCCtrl_Green-5);	
		}
		else {
			SetDM_ColorCCtrl_Green(0);	
		}	

		if(ColorCCtrl_Blue>5){
			SetDM_ColorCCtrl_Blue(ColorCCtrl_Blue-5);	
		}
		else {
			SetDM_ColorCCtrl_Blue(0);	
		}	
		
	}
	else	{
		SetDM_ColorCCtrl_Red(GetMenuStatus(CC_Red_MENU));
		SetDM_ColorCCtrl_Green(GetMenuStatus(CC_Green_MENU));
		SetDM_ColorCCtrl_Blue(GetMenuStatus(CC_Blue_MENU));
	}

	//--------color ctrl end----------//
#else 												// MDIN-i51X (i510, i540, i550)
	if (defog_act==1)	SetDM_ZipBypass(1);
	else		SetDM_ZipBypass(0);
#endif
}


//--------------------------------------------------------------------------------------------------------------------------
void SetWBModeSetMenu(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	LONG mode = (LONG)nID&WBCTRL_MODE_MASK;
	
	if(mode==WBCTRL_PRESET_MODE)	mode = WBCTRL_MANUAL_MODE;		// skip 'preset mode' for customer
	
	stWBCTRL.ctrl &= (~WBCTRL_MODE_MASK);
	stWBCTRL.ctrl |= (mode|WBCTRL_TRIGGER_ISP);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_WB_MODE, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
WORD GetWBModeSetMenu(void)
{
	switch(stWBCTRL.ctrl & WBCTRL_MODE_MASK)	{
		case  WBCTRL_WBAUTO_MODE:	return 0;
		case  WBCTRL_WBHOLD_MODE:	return 1;
		case  WBCTRL_INDOOR_MODE:	return 2;
		case  WBCTRL_OTDOOR_MODE:	return 3;
		//case  WBCTRL_PRESET_MODE:	return 4;
		case  WBCTRL_MANUAL_MODE:	return 4;
		default: 					return 0;
	}
}

//---------------------------------------------------------------------------------------------
WORD GetWBAutoBGainMenu(void)
{
	//if(stWBCTRL.offB < 0) 		return 0;
	//else								return (stWBCTRL.offB+50);
	return (stWBCTRL.offB+50);
}

//---------------------------------------------------------------------------------------------
WORD GetWBAutoRGainMenu(void)
{
	//if(stWBCTRL.offR < 0) 		return 0;
	//else								return (stWBCTRL.offR+50);
	return (stWBCTRL.offR+50);
}

//---------------------------------------------------------------------------------------------
WORD GetWBManualBGainMenu(void)
{
	if(stWBCTRL.stMANUAL.B < 106) 		return 0;
	else								return (stWBCTRL.stMANUAL.B - 106)/3;
}

//---------------------------------------------------------------------------------------------
WORD GetWBManualRGainMenu(void)
{
	if(stWBCTRL.stMANUAL.R < 106) 		return 0;
	else								return (stWBCTRL.stMANUAL.R - 106)/3;
}

//---------------------------------------------------------------------------------------------
WORD GetWBSpeedMenu(void)
{
	switch(stWBCTRL.ctrl & WBCTRL_SPEED_MASK)	{
		case  WBCTRL_SPEED_VAR0:	return 0;
		case  WBCTRL_SPEED_VAR1:	return 1;
		case  WBCTRL_SPEED_VAR2:	return 2;
		case  WBCTRL_SPEED_VAR3:	return 3;
		case  WBCTRL_SPEED_VAR4:	return 4;
		case  WBCTRL_SPEED_VAR5:	return 5;
		case  WBCTRL_SPEED_VAR6:	return 6;
		case  WBCTRL_SPEED_VAR7:	return 7;
		default: 					return 6;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SetAFModeOnOffMenu(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))

	LONG mode = AFCTRL_AFAUTO_MODE;
	
	switch(nID)	{
		case 0:		mode = AFCTRL_AFAUTO_MODE;	break;
		case 1:		mode = AFCTRL_INTVAL_MODE;	break;
		case 2:		mode = AFCTRL_AFZOOM_MODE;	break;
		case 3:		mode = AFCTRL_MANUAL_MODE;	break;
	}

	stAFCTRL.ctrl &= (~(AFCTRL_AFAUTO_MASK|AFCTRL_AFMODE_MASK));
	stAFCTRL.ctrl |= (mode);
#endif

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = nID;
	EEPROM_MultiWrite(EEPROM_FOCUS_MODE, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetFOKSDrvLIMITMenu(WORD nID)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_X30X_TAMRON__)
	LONG mode = AFFOKS_LIMIT_0p3M;
	
	switch(nID)	{
		case 0:		mode = AFFOKS_LIMIT_0p1M;	break;
		case 1:		mode = AFFOKS_LIMIT_0p3M;	break;
		case 2:		mode = AFFOKS_LIMIT_1p0M;	break;
		case 3:		mode = AFFOKS_LIMIT_1p5M;	break;
		case 4:		mode = AFFOKS_LIMIT_2p0M;	break;
		case 5:		mode = AFFOKS_LIMIT_3p0M;	break;
		case 6:		mode = AFFOKS_LIMIT_6p0M;	break;
		case 7:		mode = AFFOKS_LIMIT_10M;	break;
		case 8:		mode = AFFOKS_LIMIT_INF;	break;
	}
	LENS_SetFOKSDrvLIMIT(&stAFCTRL, (SHORT)mode);

#elif defined(__USE_X10X_UNION__)
	LONG mode = AFFOKS_LIMIT_0p3M;
	
	switch(nID)	{
		case 0:		mode = AFFOKS_LIMIT_0p1M;	break;
		case 1:		mode = AFFOKS_LIMIT_0p3M;	break;
		case 2:		mode = AFFOKS_LIMIT_1p0M;	break;
		case 3:		mode = AFFOKS_LIMIT_1p5M;	break;
		case 4:		mode = AFFOKS_LIMIT_2p0M;	break;
		case 5:		mode = AFFOKS_LIMIT_3p0M;	break;
		case 6:		mode = AFFOKS_LIMIT_6p0M;	break;
		case 7:		mode = AFFOKS_LIMIT_10M;	break;
		case 8:		mode = AFFOKS_LIMIT_INF;	break;
	}
	LENS_SetFOKSDrvLIMIT(&stAFCTRL, (SHORT)mode);

#elif defined(__USE_X10X_ZMAX__)
	LONG mode = AFFOKS_LIMIT_0p5M;
	
	switch(nID)	{
		case 0:		mode = AFFOKS_LIMIT_0p1M;	break;
		case 1:		mode = AFFOKS_LIMIT_0p5M;	break;
		case 2:		mode = AFFOKS_LIMIT_1p0M;	break;
		case 3:		mode = AFFOKS_LIMIT_1p5M;	break;
		case 4:		mode = AFFOKS_LIMIT_2p0M;	break;
		case 5:		mode = AFFOKS_LIMIT_3p0M;	break;
		case 6:		mode = AFFOKS_LIMIT_5p0M;	break;
		case 7:		mode = AFFOKS_LIMIT_10M;	break;
		case 8:		mode = AFFOKS_LIMIT_INF;	break;
	}
	LENS_SetFOKSDrvLIMIT(&stAFCTRL, (SHORT)mode);

#elif defined(__USE_X12X_UNION__)
	LONG mode = AFFOKS_LIMIT_0p5M;
	
	switch(nID)	{
		case 0:		mode = AFFOKS_LIMIT_0p1M;	break;
		case 1:		mode = AFFOKS_LIMIT_0p5M;	break;
		case 2:		mode = AFFOKS_LIMIT_1p0M;	break;
		case 3:		mode = AFFOKS_LIMIT_1p5M;	break;
		case 4:		mode = AFFOKS_LIMIT_2p0M;	break;
		case 5:		mode = AFFOKS_LIMIT_3p0M;	break;
		case 6:		mode = AFFOKS_LIMIT_5p0M;	break;
		case 7:		mode = AFFOKS_LIMIT_10M;	break;
		case 8:		mode = AFFOKS_LIMIT_INF;	break;
	}
	LENS_SetFOKSDrvLIMIT(&stAFCTRL, (SHORT)mode);

#elif defined(__USE_X03X_ZMAX__)
	LONG mode = AFFOKS_LIMIT_1p0M;
	
	switch(nID)	{
		case 0:		mode = AFFOKS_LIMIT_0p5M;	break;
		case 1:		mode = AFFOKS_LIMIT_1p0M;	break;
		case 2:		mode = AFFOKS_LIMIT_1p5M;	break;
		case 3:		mode = AFFOKS_LIMIT_2p0M;	break;
		case 4:		mode = AFFOKS_LIMIT_2p5M;	break;
		case 5:		mode = AFFOKS_LIMIT_3p0M;	break;
		case 6:		mode = AFFOKS_LIMIT_5p0M;	break;
		case 7:		mode = AFFOKS_LIMIT_10M;	break;
		case 8:		mode = AFFOKS_LIMIT_INF;	break;
	}
	LENS_SetFOKSDrvLIMIT(&stAFCTRL, (SHORT)mode);

#elif defined(__USE_YT3010_2MOTOR__)
	return;
#endif

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_FOCUS_FOKSLIMIT, (PBYTE)&buff[0], sizeof(buff));
#endif

#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetZOOMDrvSPEEDMenu(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))

	LONG mode = AFZOOM_SPEED_VAR2;
	
	switch(nID)	{
		case 0:		mode = AFZOOM_SPEED_VAR0;	break;
		case 1:		mode = AFZOOM_SPEED_VAR2;	break;
		case 2:		mode = AFZOOM_SPEED_VAR4;	break;
		case 3:		mode = AFZOOM_SPEED_VAR7;	break;
	}

	LENS_SetZOOMDrvSPEED(&stAFCTRL, mode);
#endif

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_FOCUS_ZOOMSPD, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetWIDEDrvLIMITMenu(SHORT val)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_AFZOOM_LENS__)
	LENS_SetWIDEDrvLIMIT(&stAFCTRL, MIN(MAX(val*16,0),255));	// 0 ~ 255
#endif

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
	EEPROM_MultiWrite(EEPROM_FOCUS_WMAX, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetTELEDrvLIMITMenu(SHORT val)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_AFZOOM_LENS__)
	LENS_SetTELEDrvLIMIT(&stAFCTRL, MIN(MAX(val*16,0),255));	// 0 ~ 255
#endif

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
	EEPROM_MultiWrite(EEPROM_FOCUS_TMAX, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetAFIntvalTIMEMenu(WORD val)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	LENS_SetAFIntvalTIME(&stAFCTRL, val);
#endif

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
	EEPROM_MultiWrite(EEPROM_FOCUS_INTERVAL, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
WORD GetAFModeOnOffMenu(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	if ((stAFCTRL.ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_MANUAL_MODE) return 3;
	if ((stAFCTRL.ctrl&AFCTRL_AFMODE_MASK)==AFCTRL_AFAUTO_MODE) return 0;
	if ((stAFCTRL.ctrl&AFCTRL_AFMODE_MASK)==AFCTRL_INTVAL_MODE) return 1;
	if ((stAFCTRL.ctrl&AFCTRL_AFMODE_MASK)==AFCTRL_AFZOOM_MODE) return 2;
	return 0;
#else
	return 0;
#endif
}
//--------------------------------------------------------------------------------------------------------------------------
WORD GetWIDEDrvLIMITMenu(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
#if defined(__USE_X30X_TAMRON__)
	return	(255.0f - (325.0f - stAFCTRL.wide -0.5)/325*255+0.5) /255*16; 	//zCnv = 325.0f-((255.0f-val)/255)*325+0.5;
#elif defined(__USE_X10X_UNION__)
	return	(255.0f - (366.0f - stAFCTRL.wide -0.5)/366*255+0.5) /255*16; 	//zCnv = 366.0f-((255.0f-val)/255)*366+0.5;
#elif defined(__USE_X10X_ZMAX__)
	return	(255.0f - (304.0f - stAFCTRL.wide -0.5)/304*255+0.5) /255*16; 	//zCnv = 366.0f-((255.0f-val)/255)*366+0.5;
#elif defined(__USE_X12X_UNION__)
	return	(255.0f - (304.0f - stAFCTRL.wide -0.5)/304*255+0.5) /255*16; 	//zCnv = 366.0f-((255.0f-val)/255)*366+0.5;
#elif defined(__USE_X03X_ZMAX__)
	return	(255.0f - (146.0f - stAFCTRL.wide -0.5)/146*255+0.5) /255*16; 	//zCnv = 366.0f-((255.0f-val)/255)*366+0.5;
#endif
#endif
	return 0;
}
	
//--------------------------------------------------------------------------------------------------------------------------
WORD GetTELEDrvLIMITMenu(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
#if defined(__USE_X30X_TAMRON__)
	return ((stAFCTRL.tele < 2113)?  0:  255.0f - (stAFCTRL.tele - 2113.0f -0.5)/562*255) /255*16; 	//zCnv = 2113.0f+((255.0f-val)/255)*562+0.5;
#elif defined(__USE_X10X_UNION__)
	return ((stAFCTRL.tele < 1351)?  0:  255.0f - (stAFCTRL.tele - 1351.0f -0.5)/370*255) /255*16; 	//zCnv = 1351.0f+((255.0f-val)/255)*370+0.5;
#elif defined(__USE_X10X_ZMAX__)
	return ((stAFCTRL.tele < 1002)?  0:  255.0f - (stAFCTRL.tele - 1002.0f -0.5)/258*255) /255*16; 	//zCnv = 1351.0f+((255.0f-val)/255)*370+0.5;
#elif defined(__USE_X12X_UNION__)
	return ((stAFCTRL.tele < 1002)?  0:  255.0f - (stAFCTRL.tele - 1002.0f -0.5)/258*255) /255*16; 	//zCnv = 1351.0f+((255.0f-val)/255)*370+0.5;
#elif defined(__USE_X03X_ZMAX__)
	return ((stAFCTRL.tele <  284)?  0:  255.0f - (stAFCTRL.tele -  284.0f -0.5)/150*255) /255*16; 	//zCnv = 1351.0f+((255.0f-val)/255)*370+0.5;
#endif
#endif
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
WORD GetZOOMDrvSPEEDMenu(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))

	switch(stAFCTRL.ctrl&AFZOOM_SPEED_MASK)	{
		case AFZOOM_SPEED_VAR0:
		case AFZOOM_SPEED_VAR1:		return 0;	break;
		case AFZOOM_SPEED_VAR2:
		case AFZOOM_SPEED_VAR3:		return 1;	break;
		case AFZOOM_SPEED_VAR4:
		case AFZOOM_SPEED_VAR5:		return 2;	break;
		case AFZOOM_SPEED_VAR6:
		default:					return 3;	break;
	}
#else
	return 0;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
WORD GetFOKSDrvLIMITMenu(void)
{
#if defined(__USE_AFZOOM_LENS__)

#if defined(__USE_X30X_TAMRON__)
	switch(stAFCTRL.near & AFFOKS_LIMIT_MASK)	{
		case AFFOKS_LIMIT_0p1M:		return 0;	break;
		case AFFOKS_LIMIT_0p3M:		return 1;	break;
		case AFFOKS_LIMIT_1p0M:		return 2;	break;
		case AFFOKS_LIMIT_1p5M:		return 3;	break;
		case AFFOKS_LIMIT_2p0M:		return 4;	break;
		case AFFOKS_LIMIT_3p0M:		return 5;	break;
		case AFFOKS_LIMIT_6p0M:		return 6;	break;
		case AFFOKS_LIMIT_10M:		return 7;	break;
		case AFFOKS_LIMIT_INF:		return 8;	break;
		default:					return 1;	break;
	}

#elif defined(__USE_X10X_UNION__)
	switch(stAFCTRL.near & AFFOKS_LIMIT_MASK)	{
		case AFFOKS_LIMIT_0p1M:		return 0;	break;
		case AFFOKS_LIMIT_0p3M:		return 1;	break;
		case AFFOKS_LIMIT_1p0M:		return 2;	break;
		case AFFOKS_LIMIT_1p5M:		return 3;	break;
		case AFFOKS_LIMIT_2p0M:		return 4;	break;
		case AFFOKS_LIMIT_3p0M:		return 5;	break;
		case AFFOKS_LIMIT_6p0M:		return 6;	break;
		case AFFOKS_LIMIT_10M:		return 7;	break;
		case AFFOKS_LIMIT_INF:		return 8;	break;
		default:					return 1;	break;
	}

#elif defined(__USE_X10X_ZMAX__)
	switch(stAFCTRL.near & AFFOKS_LIMIT_MASK)	{
		case AFFOKS_LIMIT_0p1M:		return 0;	break;
		case AFFOKS_LIMIT_0p5M:		return 1;	break;
		case AFFOKS_LIMIT_1p0M:		return 2;	break;
		case AFFOKS_LIMIT_1p5M:		return 3;	break;
		case AFFOKS_LIMIT_2p0M:		return 4;	break;
		case AFFOKS_LIMIT_3p0M:		return 5;	break;
		case AFFOKS_LIMIT_5p0M:		return 6;	break;
		case AFFOKS_LIMIT_10M:		return 7;	break;
		case AFFOKS_LIMIT_INF:		return 8;	break;
		default:					return 1;	break;
	}

#elif defined(__USE_X12X_UNION__)
	switch(stAFCTRL.near & AFFOKS_LIMIT_MASK)	{
		case AFFOKS_LIMIT_0p1M:		return 0;	break;
		case AFFOKS_LIMIT_0p5M:		return 1;	break;
		case AFFOKS_LIMIT_1p0M:		return 2;	break;
		case AFFOKS_LIMIT_1p5M:		return 3;	break;
		case AFFOKS_LIMIT_2p0M:		return 4;	break;
		case AFFOKS_LIMIT_3p0M:		return 5;	break;
		case AFFOKS_LIMIT_5p0M:		return 6;	break;
		case AFFOKS_LIMIT_10M:		return 7;	break;
		case AFFOKS_LIMIT_INF:		return 8;	break;
		default:					return 1;	break;
	}

#elif defined(__USE_X03X_ZMAX__)
	switch(stAFCTRL.near & AFFOKS_LIMIT_MASK)	{
		case AFFOKS_LIMIT_0p5M:		return 0;	break;
		case AFFOKS_LIMIT_1p0M:		return 1;	break;
		case AFFOKS_LIMIT_1p5M:		return 2;	break;
		case AFFOKS_LIMIT_2p0M:		return 3;	break;
		case AFFOKS_LIMIT_2p5M:		return 4;	break;
		case AFFOKS_LIMIT_3p0M:		return 5;	break;
		case AFFOKS_LIMIT_5p0M:		return 6;	break;
		case AFFOKS_LIMIT_10M:		return 7;	break;
		case AFFOKS_LIMIT_INF:		return 8;	break;
		default:					return 1;	break;
	}

#elif defined(__USE_YT3010_2MOTOR__)
	switch(stAFCTRL.near & AFFOKS_LIMIT_MASK)	{
		case AFFOKS_LIMIT_0p5M:		return 0;	break;
		case AFFOKS_LIMIT_1p0M:		return 1;	break;
		case AFFOKS_LIMIT_1p5M:		return 2;	break;
		case AFFOKS_LIMIT_2p0M:		return 3;	break;
		case AFFOKS_LIMIT_2p5M:		return 4;	break;
		case AFFOKS_LIMIT_3p0M:		return 5;	break;
		case AFFOKS_LIMIT_5p0M:		return 6;	break;
		case AFFOKS_LIMIT_10M:		return 7;	break;
		case AFFOKS_LIMIT_INF:		return 8;	break;
		default:					return 1;	break;
	}

#else
	switch(stAFCTRL.near & AFFOKS_LIMIT_MASK)	{
		case AFFOKS_LIMIT_0p5M:		return 0;	break;
		case AFFOKS_LIMIT_1p0M:		return 1;	break;
		case AFFOKS_LIMIT_1p5M:		return 2;	break;
		case AFFOKS_LIMIT_2p0M:		return 3;	break;
		case AFFOKS_LIMIT_2p5M:		return 4;	break;
		case AFFOKS_LIMIT_3p0M:		return 5;	break;
		case AFFOKS_LIMIT_5p0M:		return 6;	break;
		case AFFOKS_LIMIT_10M:		return 7;	break;
		case AFFOKS_LIMIT_INF:		return 8;	break;
		default:					return 1;	break;
	}
#endif

#endif
	return 0;
}
//--------------------------------------------------------------------------------------------------------------------------
WORD GetAFIntvalTIMEMenu(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	return (WORD)(stAFCTRL.tINT);
#else
	return 0;
#endif
}

//-----------------------------------------------------------------------------------------
BYTE GetAEShutSpeed(void)
{
	LONG mode = stAECTRL.ctrl&AECTRL_SHUTCTL_MASK;
	LONG shut = stAECTRL.ctrl&AECTRL_SHUT_MASK;
	BYTE mVal;
	
	if(mode == AECTRL_SHUTCTL_AUTO)		return 0;
	
	switch(shut)	{
		case AECTRL_SHUTCTL_1_25:
		case AECTRL_SHUTCTL_1_30:		mVal =  1;	break;
		case AECTRL_SHUTCTL_1_50:
		case AECTRL_SHUTCTL_1_60:		mVal =  2;	break;
		case AECTRL_SHUTCTL_1_100:
		case AECTRL_SHUTCTL_1_120:		mVal =  3;	break;
		case AECTRL_SHUTCTL_1_150:
		case AECTRL_SHUTCTL_1_180:		mVal =  4;	break;
		case AECTRL_SHUTCTL_1_200:
		case AECTRL_SHUTCTL_1_240:		mVal =  5;	break;
		case AECTRL_SHUTCTL_1_250:		
		case AECTRL_SHUTCTL_1_300:		mVal =  6;	break;
		case AECTRL_SHUTCTL_1_500:		mVal =  7;	break;
		case AECTRL_SHUTCTL_1_1000:		mVal = 8;	break;
		case AECTRL_SHUTCTL_1_2000:		mVal = 9;	break;
		case AECTRL_SHUTCTL_1_5000:		mVal = 10;	break;
		case AECTRL_SHUTCTL_1_10000:	mVal = 11;	break;	
		default:						mVal =  1;	break;
	}
	
	return mVal;
}

//-----------------------------------------------------------------------------------------
void SetAEShutSpeed(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[6];
#endif
	
	LONG mode, shut;
	FLOAT sens_fps=1.0f*GetSS_SensorVoutFps();
	LONG  shut_den;  //denominator
	
#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
	BYTE manual_mode = 4; 				// ae mode(zoom lens) : auto(0), shut(1), iris(2), agc(3), manual(4)
#else
	BYTE manual_mode = 3; 				// ae mode(manual lens) : auto(0), shut(1), agc(2), manual(3)
#endif
	
	if((stAEMENU.mode == manual_mode) && (nID == 0))	{ 						// manual mode : skip shut 'auto'
		if(GetAEShutSpeed()==1) 	nID = GetMenuItemMax(EXP_SHUT_MENU)-1; 		// 1/25 -> auto(skip) -> 1/10000
		else 						nID = 1; 									// 1/10000 -> auto(skip) -> 1/25
		
		SetMenuStatus(EXP_SHUT_MENU, nID); 										// menu update
	}

	if(GetSS_SensorVoutPAL()) {
		switch (nID) {
			case  0:    mode = AECTRL_SHUTCTL_AUTO;       shut_den =    25;    break;
			case  1:    mode = AECTRL_SHUTCTL_1_25;       shut_den =    25;    break;
			case  2:    mode = AECTRL_SHUTCTL_1_50;       shut_den =    50;    break;
			case  3:    mode = AECTRL_SHUTCTL_1_100;      shut_den =   100;    break;
			case  4:    mode = AECTRL_SHUTCTL_1_150;      shut_den =   150;    break;
			case  5:    mode = AECTRL_SHUTCTL_1_200;      shut_den =   200;    break;
			case  6:    mode = AECTRL_SHUTCTL_1_250;      shut_den =   250;    break;
			case  7:    mode = AECTRL_SHUTCTL_1_500;      shut_den =   500;    break;
			case  8:    mode = AECTRL_SHUTCTL_1_1000;     shut_den =  1000;    break;
			case  9:    mode = AECTRL_SHUTCTL_1_2000;     shut_den =  2000;    break;
			case 10:    mode = AECTRL_SHUTCTL_1_5000;     shut_den =  5000;    break;
			case 11:    mode = AECTRL_SHUTCTL_1_10000;    shut_den = 10000;    break;
			default:    mode = AECTRL_SHUTCTL_AUTO;       shut_den =    25;    break;
		}
	}
	else {
		switch (nID) {
			case  0:    mode = AECTRL_SHUTCTL_AUTO;       shut_den =    30;    break;
			case  1:    mode = AECTRL_SHUTCTL_1_30;       shut_den =    30;    break;
			case  2:    mode = AECTRL_SHUTCTL_1_60;       shut_den =    60;    break;
			case  3:    mode = AECTRL_SHUTCTL_1_120;      shut_den =   120;    break;
			case  4:    mode = AECTRL_SHUTCTL_1_180;      shut_den =   180;    break;
			case  5:    mode = AECTRL_SHUTCTL_1_240;      shut_den =   240;    break;
			case  6:    mode = AECTRL_SHUTCTL_1_300;      shut_den =   300;    break;
			case  7:    mode = AECTRL_SHUTCTL_1_500;      shut_den =   500;    break;
			case  8:    mode = AECTRL_SHUTCTL_1_1000;     shut_den =  1000;    break;
			case  9:    mode = AECTRL_SHUTCTL_1_2000;     shut_den =  2000;    break;
			case 10:    mode = AECTRL_SHUTCTL_1_5000;     shut_den =  5000;    break;
			case 11:    mode = AECTRL_SHUTCTL_1_10000;    shut_den = 10000;    break;
			default:    mode = AECTRL_SHUTCTL_AUTO;       shut_den =    30;    break;
		}
	}

	//update shutter value
	shut = AECTRL_GetSHUTMax() - (sens_fps*AECTRL_GetSHUTMax()/shut_den);
	shut = MIN((LONG)AECTRL_GetSHUTMax(), MAX(0, shut));

//	UARTprintf("shut : %d\r\n", shut);

	if(mode==AECTRL_SHUTCTL_AUTO)
		AECTRL_SetSHUTCtrl(&stAECTRL, (SHORT)shut, AECTRL_SHUTCTL_AUTO);
	else
		AECTRL_SetSHUTCtrl(&stAECTRL, (SHORT)shut, AECTRL_SHUTCTL_MANU);

	stAECTRL.ctrl &= (~AECTRL_SHUT_MASK);
	stAECTRL.ctrl |= (mode);
	
	stAEMENU.shut[stAEMENU.mode] = nID;


#if (defined(__USE_CMOS_IMX265__) && !defined(__USE_IMX265_SLAVE_MODE__))
	pulse2sensor_width=(WORD)AECTRL_GetSHUTGain();
#endif

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = stAEMENU.shut[0]; 		// auto
	buff[2] = stAEMENU.shut[1]; 		// shut-fix
	buff[3] = stAEMENU.shut[2]; 		// iris-fix
	buff[4] = stAEMENU.shut[3]; 		// agc-fix
	buff[5] = stAEMENU.shut[4]; 		// manual
	EEPROM_MultiWrite(EEPROM_EXPOSURE_SHUT, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//-----------------------------------------------------------------------------------------
void SetAEShutSpdFlk(WORD nID) // nID:0=1/100 1=1/60
{
	
	LONG shut;
	FLOAT sens_fps=1.0f*GetSS_SensorVoutFps();
	LONG  shut_den;  //denominator

	if(nID == 0)	shut_den =   100;
	else	shut_den =    60;

	shut = AECTRL_GetSHUTMax() - (sens_fps*AECTRL_GetSHUTMax()/shut_den);
	shut = MIN((LONG)AECTRL_GetSHUTMax(), MAX(0, shut));
	AECTRL_SetSHUTGain(shut);

}

//-----------------------------------------------------------------------------------------
BYTE GetAEIrisGain(void)
{
#if defined(__USE_DCIRIS_HALL__) ||defined(__USE_DCIRIS_DAMP__	) || defined(__USE_P_IRIS_ONLY__)
	SHORT iris_gain = DCLENS_GetIRISGain();
	BYTE i;
	
	for(i=0;i<sizeof(AECTRL_F_VALUE)/sizeof(AECTRL_F_VALUE[0]);i++)	if(iris_gain <= AECTRL_F_VALUE[i])		break;
	return MIN(i, sizeof(AECTRL_F_VALUE)/sizeof(AECTRL_F_VALUE[0])-1);
#else
	return 0;
#endif
}

//-----------------------------------------------------------------------------------------
void SetAEIrisGain(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[6];
#endif
	
	AECTRL_SetAE_IRISValue(nID);

	stAEMENU.iris[stAEMENU.mode] = nID;
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = stAEMENU.iris[0]; 		// auto
	buff[2] = stAEMENU.iris[1]; 		// shut-fix
	buff[3] = stAEMENU.iris[2]; 		// iris-fix
	buff[4] = stAEMENU.iris[3]; 		// agc-fix
	buff[5] = stAEMENU.iris[4]; 		// manual
	EEPROM_MultiWrite(EEPROM_EXPOSURE_IRIS, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//-----------------------------------------------------------------------------------------
void SetAEIrisBRGain(WORD nID)
{
	AECTRL_SetAE_IRISBRValue(nID);

	//stAEMENU.iris[stAEMENU.mode] = nID;
}

//-----------------------------------------------------------------------------------------
BYTE GetAEAGCGain(void)
{
	LONG val = AECTRL_GetGAINGain();
	BYTE i;

	//UARTprintf("%s\r\n", __FUNCTION__);
	for(i=0;i<sizeof(AGCGain)/sizeof(AGCGain[0]);i++)		if(val <= AGCGain[i])	break;
	
	return MIN(i, sizeof(AGCGain)/sizeof(AGCGain[0])-1);
}

//-----------------------------------------------------------------------------------------
void SetAEAGCGain(WORD nID)
{
	//UARTprintf("%s\r\n", __FUNCTION__);
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[6];
#endif

	LONG gain;
	
#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
	BYTE manual_mode = 4; 				// ae mode(zoom lens) : auto(0), shut(1), iris(2), agc(3), manual(4)
#else
	BYTE manual_mode = 3; 				// ae mode(manual lens) : auto(0), shut(1), agc(2), manual(3)
#endif
	
	if((stAEMENU.mode == manual_mode) && (nID == 0))	{ 						// manual mode : skip agc 'auto'
		if(GetAEAGCGain()==0) 		nID = GetMenuItemMax(EXP_AGC_MENU)-1; 		// 0dB -> auto(skip) -> maxdB
		else 						nID = 1; 									// maxdB -> auto(skip) -> 0dB
		
		SetMenuStatus(EXP_AGC_MENU, nID); 										// menu update
	}
	
	gain = AGCGain[nID];
	//UARTprintf("gain : %d\r\n", gain);
	
	if(nID != 0)		AECTRL_SetGAINCtrl(&stAECTRL, gain, AECTRL_GAINCTL_MANU, 1); // set direct
	else				AECTRL_SetGAINCtrl(&stAECTRL, gain, AECTRL_GAINCTL_AUTO, 0); // set btw(min,max)
	
	// set agc-max(high) in agc manual mode
	if(nID != 0)		{
		//UARTprintf("%s : 3\r\n", __FUNCTION__);
		SetAEGainCtrlMode(6);	SetMenuStatus(EXP_AGCMAX_MENU, 3);
	}
	
	
	stAEMENU.agc[stAEMENU.mode] = nID;
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = stAEMENU.agc[0]; 		// auto
	buff[2] = stAEMENU.agc[1]; 		// shut-fix
	buff[3] = stAEMENU.agc[2]; 		// iris-fix
	buff[4] = stAEMENU.agc[3]; 		// agc-fix
	buff[5] = stAEMENU.agc[4]; 		// manual
	EEPROM_MultiWrite(EEPROM_EXPOSURE_AGC, (PBYTE)&buff[0], sizeof(buff));
#endif
}


//-----------------------------------------------------------------------------------------
BYTE GetAEGainCtrlMode(void)
{
	LONG mode = AECTRL_GetGainCTLMode();
	BYTE mVal;
	
	switch(mode)	{
		case AECTRL_GAINCTL_OFF:	mVal =  0;	break;
		case AECTRL_GAINCTL_MIN:	mVal =  1;	break;
		case AECTRL_GAINCTL_MID:	mVal =  2;	break;
		case AECTRL_GAINCTL_MAX:	mVal =  3;	break;
		case AECTRL_GAINCTL_DMIN:	mVal =  4;	break;
		case AECTRL_GAINCTL_DMID:	mVal =  5;	break;
		case AECTRL_GAINCTL_DMAX:	mVal =  6;	break;
		default:					mVal =  3;	break;
	}
	
	return mVal;
}

//-----------------------------------------------------------------------------------------
void SetAEInitGainCtrlMode(LONG mode)
{
	LONG max;

	switch(mode)	{
		case  AECTRL_GAINCTL_OFF:	max = AGCMax[0];	break;
		case  AECTRL_GAINCTL_MIN:	max = AGCMax[1];	break;
		case  AECTRL_GAINCTL_MID:	max = AGCMax[2];	break;
		case  AECTRL_GAINCTL_MAX:	max = AGCMax[3];	break;
		case  AECTRL_GAINCTL_DMIN:	max =  AGCMax[3] + AGCMax[3]*2/8;	break;
		case  AECTRL_GAINCTL_DMID:	max =  AGCMax[3] + AGCMax[3]*5/8;	break;
		case  AECTRL_GAINCTL_DMAX:	max =  AGCMax[3] + AGCMax[3]*8/8;	break;
		default:	max = AGCMax[3];	break;
	}
	
	AECTRL_SetGAINMax(max);	// set GAIN max
}

//-----------------------------------------------------------------------------------------
void SetAEGainCtrlMode(WORD nID)
{
	LONG mode, max=AGCMax[nID];
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[6];
#endif

	switch(nID)	{
		case  0:	mode = AECTRL_GAINCTL_OFF;	break;
		case  1:	mode = AECTRL_GAINCTL_MIN;	break;
		case  2:	mode = AECTRL_GAINCTL_MID;	break;
		case  3:	mode = AECTRL_GAINCTL_MAX;	break;
		case  4:	mode = AECTRL_GAINCTL_DMIN;
					max =  AGCMax[3] + AGCMax[3]*2/8;
					break;
		case  5:	mode = AECTRL_GAINCTL_DMID;
					max =  AGCMax[3] + AGCMax[3]*5/8;
					break;
		case  6:	mode = AECTRL_GAINCTL_DMAX;
					max =  AGCMax[3] + AGCMax[3]*8/8;
					break;
		default:	mode = AECTRL_GAINCTL_OFF;	break;
	}

	stAECTRL.ctrl &= (~AECTRL_GAIN_MASK);
	stAECTRL.ctrl |= (mode);

	if(AECTRL_GetGAINGain() > max) {
		AECTRL_SetGAINGain(max);
	}

	AECTRL_SetGAINMax(max);	// set GAIN max
	
	stAEMENU.agcmax[stAEMENU.mode] = nID;

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = stAEMENU.agcmax[0]; 		// auto
	buff[2] = stAEMENU.agcmax[1]; 		// shut-fix
	buff[3] = stAEMENU.agcmax[2]; 		// iris-fix
	buff[4] = stAEMENU.agcmax[3]; 		// agc-fix
	buff[5] = stAEMENU.agcmax[4]; 		// manual
	EEPROM_MultiWrite(EEPROM_EXPOSURE_AGCMAX, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//-----------------------------------------------------------------------------------------
BYTE GetAEBrightLvl(void)
{
	LONG val = AECTRL_GetBrightness();
	BYTE i;
	
	for(i=0;i<100;i++)		if(val <= AECTRL_GetYREFVal(i))		break;	// yref -> bright(0~100)
	
	return (BYTE)(i / 5);		// bright(0~100) -> menu val(0~20)
}

//-----------------------------------------------------------------------------------------
BYTE GetAEIrisSpeed(void)
{
#if defined(__USE_PWMIRIS_ONLY__)
	return	stAEMENU.irisspeed;
#else
	return 0;
#endif
}

//-----------------------------------------------------------------------------------------
BYTE GetAELensMode(void)
{
#if defined(__USE_PWMIRIS_ONLY__)
	return	stAEMENU.lensmode;
#else
	return 0;
#endif
}

//-----------------------------------------------------------------------------------------
BYTE GetAEFlkMode(void)
{
	return	stAEMENU.flkmode;
}

//-----------------------------------------------------------------------------------------
void SetAEBrightLvl(WORD val)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[6];
#endif
	
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	if(GetBY_dwdr_mode()) AECTRL_SetBrightness(&stAECTRL, val*2);						
	else AECTRL_SetBrightness(&stAECTRL, val*5);	
#else   							// MDIN-i51X (i510, i540, i550)
	AECTRL_SetBrightness(&stAECTRL, val*5);		// 0~20 -> 0~100
#endif
	
	stAEMENU.bright[stAEMENU.mode] = val;
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = stAEMENU.bright[0]; 		// auto
	buff[2] = stAEMENU.bright[1]; 		// shut-fix
	buff[3] = stAEMENU.bright[2]; 		// iris-fix
	buff[4] = stAEMENU.bright[3]; 		// agc-fix
	buff[5] = stAEMENU.bright[4]; 		// manual
	EEPROM_MultiWrite(EEPROM_EXPOSURE_BRIGHT, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//-----------------------------------------------------------------------------------------
void SetAEIrisSpeed(WORD val)
{
#if defined(__USE_PWMIRIS_ONLY__)

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	stAEMENU.irisspeed = (BYTE)val;
	DCLENS_IrisSpeed(val);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = (BYTE)val;	// 0~5
	EEPROM_MultiWrite(EEPROM_EXPOSURE_IRISSPD, (PBYTE)&buff[0], sizeof(buff));
#endif
#endif
}

//-----------------------------------------------------------------------------------------
void AEFlkModeSet(WORD val)
{
	switch(val) {
		case 0 :	FLK_ProcessOnOff(TRUE);
				AECTRL_SetSHUTGain(0);
			break;
		case 1 :	FLK_ProcessOnOff(FALSE);
				if(GetSS_SensorVoutPAL())
					SetAEShutSpdFlk(1); // 1/60
				else
					SetAEShutSpdFlk(0); // 1/100
			break;
		case 2 :	FLK_ProcessOnOff(FALSE);
				AECTRL_SetSHUTGain(0);
			break;
	}
}

//-----------------------------------------------------------------------------------------
void SetAELensMode(WORD val)
{
#if defined(__USE_PWMIRIS_ONLY__)

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	stAEMENU.lensmode = (BYTE)val;
	DCLENS_LensMode(val);
	AEFlkModeSet(GetAEFlkMode());
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = (BYTE)val;	// 0~5
	EEPROM_MultiWrite(EEPROM_EXPOSURE_LENSMOD, (PBYTE)&buff[0], sizeof(buff));
#endif
#endif
}

//-----------------------------------------------------------------------------------------
void SetAEFlkMode(WORD val)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	stAEMENU.flkmode = (BYTE)val;
	AEFlkModeSet(val);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = (BYTE)val;	// 0~2
	EEPROM_MultiWrite(EEPROM_EXPOSURE_FLKMOD, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//-----------------------------------------------------------------------------------------
void SetAESetShutDefault(void)
{
	LONG  shut_den;
	LONG shut;
	FLOAT sens_fps=1.0f*GetSS_SensorVoutFps();

	if(GetSS_SensorVoutPAL())		shut_den =    25;
	else							shut_den =    30;

	shut = AECTRL_GetSHUTMax() - (sens_fps*AECTRL_GetSHUTMax()/shut_den);
	shut = MIN((LONG)AECTRL_GetSHUTMax(), MAX(0, shut));

	AECTRL_SetSHUTCtrl(&stAECTRL, (SHORT)shut, AECTRL_SHUTCTL_AUTO);
}

//-----------------------------------------------------------------------------------------
void SetAECtrlMode(WORD nID)
{
	// backup old parameter
	BYTE  shut_old = GetMenuStatus(EXP_SHUT_MENU);
	BYTE  iris_old = GetMenuStatus(EXP_IRIS_MENU);
	BYTE  agc_old = GetMenuStatus(EXP_AGC_MENU);
	BYTE  sensup_old = GetMenuStatus(EXP_SENSUP_MENU);
	BYTE  agcmax_old = GetMenuStatus(EXP_AGCMAX_MENU);
	BYTE  bright_old = GetMenuStatus(EXP_BRIGHT_MENU);
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	f_dGain = 0; // digital gain off
	
	stAEMENU.mode = nID; 												// set ae control mode
	
	// set ae parameter for ae control mode
	switch(nID)	{
#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
		case 0:														// auto mode
			stAEMENU.shut[stAEMENU.mode] = 0; 	stAEMENU.iris[stAEMENU.mode] = GetAEIrisGain();
			stAEMENU.agc[stAEMENU.mode] = 0;
			AECTRL_SetAE_AutoMode();
			SetAECtrlOnOff (ON);
			break;
		case 1:														// shut fix
			stAEMENU.iris[stAEMENU.mode] = GetAEIrisGain();
			stAEMENU.agc[stAEMENU.mode] = 0; 		//stAEMENU.sensup[stAEMENU.mode] = 0;
			AECTRL_SetAE_ShutterFixMode();
			SetAECtrlOnOff (ON);
			break;
		case 2:														// iris fix
			stAEMENU.shut[stAEMENU.mode] = 0;		
			stAEMENU.agc[stAEMENU.mode] = 0; 		stAEMENU.sensup[stAEMENU.mode] = 0;
			AECTRL_SetAE_IRISFixMode();
			SetAECtrlOnOff (ON);
			break;
		case 3:														// agc fix
			stAEMENU.shut[stAEMENU.mode] = 0; 	stAEMENU.sensup[stAEMENU.mode] = 0;
			AECTRL_SetAE_GainFixMode();
			SetAECtrlOnOff (ON);
			break;
		default:								// manual
			SetAECtrlOnOff (OFF);
			AECTRL_SetAE_Manual();
			f_dGain = 1;
			
			if(stAEMENU.shut[stAEMENU.mode]==0) 	{ 										// shut auto mode
				stAEMENU.shut[stAEMENU.mode] = 1; 											// auto -> 1/30 or 1/25
			}
			
			if(stAEMENU.agc[stAEMENU.mode]==0) 		{ 										// agc auto mode
				if(GetAEAGCGain()==0) 		stAEMENU.agc[stAEMENU.mode] = 1; 				// auto -> 0dB
				else 						stAEMENU.agc[stAEMENU.mode] = GetAEAGCGain(); 	// xxdB -> xxdB
			}
			
 			break;
#else
//		AECTRL_SetAE_Manual();
		case 0:														// auto mode
			stAEMENU.shut[stAEMENU.mode] = 0; 	stAEMENU.iris[stAEMENU.mode] = 7;
			stAEMENU.agc[stAEMENU.mode] = 0;
			SetAESetShutDefault();
			AECTRL_SetAE_AutoMode();
			#if defined(__USE_PWMIRIS_ONLY__)
			DCLENS_LensMode(GetAELensMode());
			#endif
			AEFlkModeSet(GetAEFlkMode());
			SetAECtrlOnOff (ON);
			break;
		case 1:														// shut fix
			stAEMENU.iris[stAEMENU.mode] = 7;
			stAEMENU.agc[stAEMENU.mode] = 0; 		//stAEMENU.sensup[stAEMENU.mode] = 0;
			AECTRL_SetAE_ShutterFixMode();
			SetAECtrlOnOff (ON);
			break;
		case 2:														// agc fix
			stAEMENU.shut[stAEMENU.mode] = 0; 	stAEMENU.sensup[stAEMENU.mode] = 0;
			AECTRL_SetAE_GainFixMode();
			#if defined(__USE_PWMIRIS_ONLY__)
			DCLENS_LensMode(GetAELensMode());
			#endif
			AEFlkModeSet(GetAEFlkMode());
			SetAECtrlOnOff (ON);
			break;
		default:								// manual
			SetAECtrlOnOff (OFF);
			AECTRL_SetAE_Manual();
			AECTRL_IRISPWM_SetFullOpen();
			f_dGain = 1;
			
			if(stAEMENU.shut[stAEMENU.mode]==0) 	{ 										// shut auto mode
				stAEMENU.shut[stAEMENU.mode] = 1; 											// auto -> 1/30 or 1/25
			}
			
			if(stAEMENU.agc[stAEMENU.mode]==0) 		{ 										// agc auto mode
				if(GetAEAGCGain()==0) 		stAEMENU.agc[stAEMENU.mode] = 1; 				// auto -> 0dB
				else 						stAEMENU.agc[stAEMENU.mode] = GetAEAGCGain(); 	// xxdB -> xxdB
			}
			
 			break;
#endif
	}
	
	// compare old & new parameter, and update ae api value
	if((GetAEFlkMode() != 1) || (nID == 1)){
		if(stAEMENU.shut[stAEMENU.mode] != shut_old) 		SetAEShutSpeed(stAEMENU.shut[stAEMENU.mode]);
	}
	if(stAEMENU.iris[stAEMENU.mode] != iris_old) 		SetAEIrisGain(stAEMENU.iris[stAEMENU.mode]);
	if(stAEMENU.agc[stAEMENU.mode] != agc_old) 			SetAEAGCGain(stAEMENU.agc[stAEMENU.mode]);
	if(stAEMENU.agcmax[stAEMENU.mode] != agcmax_old) 	SetAEGainCtrlMode(stAEMENU.agcmax[stAEMENU.mode]);

	if(stAEMENU.sensup[stAEMENU.mode] != sensup_old) 	DEMO_SetExpSensUp(stAEMENU.sensup[stAEMENU.mode], 0);

#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
	if(stAEMENU.mode != 4)	{	// skip ae bright setting on manual mode
		if(stAEMENU.bright[stAEMENU.mode] != bright_old) 	SetAEBrightLvl(stAEMENU.bright[stAEMENU.mode]);
	}
#else	// manual lens
	if(stAEMENU.mode != 3)	{	// skip ae bright setting on manual mode
		if(stAEMENU.bright[stAEMENU.mode] != bright_old) 	SetAEBrightLvl(stAEMENU.bright[stAEMENU.mode]);
	}
#endif
	
	// update menu osd item value with new parameter
	SetMenuStatus(EXP_SHUT_MENU, stAEMENU.shut[stAEMENU.mode]);
	SetMenuStatus(EXP_IRIS_MENU, stAEMENU.iris[stAEMENU.mode]);
	SetMenuStatus(EXP_AGC_MENU, stAEMENU.agc[stAEMENU.mode]);
	SetMenuStatus(EXP_AGCMAX_MENU, stAEMENU.agcmax[stAEMENU.mode]);
	SetMenuStatus(EXP_SENSUP_MENU, stAEMENU.sensup[stAEMENU.mode]);
	SetMenuStatus(EXP_BRIGHT_MENU, stAEMENU.bright[stAEMENU.mode]);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_EXPOSURE_MODE, (PBYTE)&buff[0], sizeof(buff));
#endif
}
//-----------------------------------------------------------------------------------------
void SetGSTriggerStandby(WORD nID)
{
#if (defined(__USE_CMOS_IMX265__) && !defined(__USE_IMX265_SLAVE_MODE__))
	BOOL ss_slave_en, vd_edge_polarity, hd_edge_polarity;
	BYTE hd_delay;
	BYTE slave_mode_sel=6;

	CMOS_ByteWrite(SPI_IMX265_ADDR, 0x020A, 0x01);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop		
	CMOS_ByteWrite(SPI_IMX265_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY	
	
	if(nID==1){
		ss_slave_en=0;
		vd_edge_polarity=1;	hd_edge_polarity=1;	hd_delay=0;
		
		MISP_RegWrite(MISP_LOCAL_ID, 0x56C, ss_slave_en<<15| vd_edge_polarity<<13|hd_edge_polarity<<12|hd_delay);		//slave mode off 
		MISP_RegWrite(MISP_LOCAL_ID, 0x56D, slave_mode_sel<<13|37);	// 
		
		#if defined(__USE_CMOS_IMX265__) || defined(__USE_CMOS_IMX273__)
		MISP_RegWrite(MISP_LOCAL_ID, 0x571, CMOS_GetVmax() -pulse2sensor_width);		
		MISP_RegWrite(MISP_LOCAL_ID, 0x572, pulse2sensor_width);

	#ifndef __USE_TRIGGER_MODE__		
		#if defined(__USE_CMOS_IMX265__)
		IMX265_ChangeTriggerMode(3);		
		#elif defined(__USE_CMOS_IMX273__)
		IMX273_ChangeTriggerMode(3);		
		#endif
	#endif

		//UARTprintf("-----Set Fast Trigger Mode Standby----\r\n");				
		#endif

	// re-setting parameter
		GS_Sensor_Status=Trig_Stby_Wait;
		pulse2sensor_counter=0;
	}
	else{
		ss_slave_en=1;
		vd_edge_polarity=1;	hd_edge_polarity=1;	hd_delay=0;
		
	#ifndef __USE_TRIGGER_MODE__		
		ss_slave_en=0;
		vd_edge_polarity=1;	hd_edge_polarity=1;	hd_delay=0;
	
		#if defined(__USE_CMOS_IMX265__)
		IMX265_ChangeTriggerMode(0);		
		#elif defined(__USE_CMOS_IMX273__)
		IMX273_ChangeTriggerMode(0);		
		#endif
	#endif

	//return normal shutter mode
		MISP_RegWrite(MISP_LOCAL_ID, 0x56C, ss_slave_en<<15| vd_edge_polarity<<13|hd_edge_polarity<<12|hd_delay);		//slave mode off 
		MISP_RegWrite(MISP_LOCAL_ID, 0x570, (0<<12|4));

	// re-setting parameter
		GS_Sensor_Status=Trig_Stby_Off;
		pulse2sensor_counter=0;		
	}

	CMOS_ByteWrite(SPI_IMX265_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0: Operation,  1: STANDBY		
	CMOS_ByteWrite(SPI_IMX265_ADDR, 0x020A, 0x00);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop	

#endif
}
void SetGSTriggerPushPulse(void)
{
#if (defined(__USE_CMOS_IMX265__) && !defined(__USE_IMX265_SLAVE_MODE__))
	BOOL hd_edge_polarity=1;
	BOOL vd_edge_polarity=1;
	BOOL ss_slave_en=1;
	BYTE hd_delay=0;
	
	if(GS_Sensor_Status==Trig_Stby_Off) return;
	if(GS_Sensor_Status==Trig_Stby_Running) return;
								
	GS_Sensor_Status=Trig_Stby_Running;			//other trigger pulse should be prohibited

	//send trigger signal what user want
#if defined(__USE_CMOS_IMX265__) || defined(__USE_CMOS_IMX273__)
	MISP_RegWrite(MISP_LOCAL_ID, 0x571, CMOS_GetVmax() -pulse2sensor_width);		
	MISP_RegWrite(MISP_LOCAL_ID, 0x572, pulse2sensor_width);
#endif	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x56C, ss_slave_en<<15| vd_edge_polarity<<13|hd_edge_polarity<<12|hd_delay);		

	if(pulse2sensor_num==1){
#if defined(__USE_CMOS_IMX265__)		
		if( GetSS_SensorVoutFrmt() == MISP_VIDSRC_1920x1080p50 ) {	max_mscnt_trig=20-5;		reset_mscnt_trig	=0;	}
		else if(GetSS_SensorVoutFrmt() == MISP_VIDSRC_1920x1080p60 ) {	max_mscnt_trig=16-1;		reset_mscnt_trig	=0;	}
		else if(GetSS_SensorVoutFrmt() == MISP_VIDSRC_1920x1080p25 ) {	max_mscnt_trig=40-8;		reset_mscnt_trig	=0;	}
		else		{	max_mscnt_trig=33-1;		reset_mscnt_trig	=0;	}	//FHD30

#elif defined(__USE_CMOS_IMX273__)
	max_mscnt_trig=16-1;		reset_mscnt_trig	=0;	
#endif
		
	}
	else		max_mscnt_trig=0;	

	pulse2sensor_counter++;	
	//UARTprintf("-----SetGSTriggerPushPulse : GetSS_SensorVoutFrmt=%d----\r\n",GetSS_SensorVoutFrmt());		
	//UARTprintf("-----SetGSTriggerPushPulse : pulse2sensor_counter=%d----\r\n",pulse2sensor_counter);		
#endif
}

void SetGSTriggerFrameCounter(void)
{
#if (defined(__USE_CMOS_IMX265__) && !defined(__USE_IMX265_SLAVE_MODE__))
	BOOL hd_edge_polarity=1;
	BOOL vd_edge_polarity=1;
	BOOL ss_slave_en=0;
	BYTE hd_delay=0;
	
	if(GS_Sensor_Status==Trig_Stby_Running)
	{
		if( pulse2sensor_counter >= pulse2sensor_num )
		{
			//retrun trigger signal high	
			MISP_RegWrite(MISP_LOCAL_ID, 0x56C, ss_slave_en<<15| vd_edge_polarity<<13|hd_edge_polarity<<12|hd_delay);		//slave mode off 
		
			pulse2sensor_counter=0;	
			max_mscnt_trig=0;
			GS_Sensor_Status=Trig_Stby_Wait;		
			UARTprintf("SetGSTriggerPushPulse end(%d)\r\n",reset_mscnt_trig);		
		}
		else{
			pulse2sensor_counter++;
		}
	}
#endif
}

void SetGSTriggerOffHandler(void)
{
#if (defined(__USE_CMOS_IMX265__) && !defined(__USE_IMX265_SLAVE_MODE__))
	if(max_mscnt_trig==0) return;

	if(max_mscnt_trig==reset_mscnt_trig)
	{
		SetGSTriggerFrameCounter();
		UARTprintf("-----SetGSTriggerOffHandler : max_mscnt_trig=%d----\r\n",max_mscnt_trig);	
	}
#endif
}


//-----------------------------------------------------------------------------------------
BYTE GetAECtrlMode(void)
{
	return  stAEMENU.mode;
}

//-----------------------------------------------------------------------------------------
void SetAEModeDefault(void)
{
	BYTE i;
	
	stAEMENU.mode = 0; 						// ae control mode default 'auto'
	stAEMENU.irisspeed = 3; 				// iris speed default '3'
	stAEMENU.lensmode = 0; 					// lens mode default '0'(indoor), '1'(outdoor)
	stAEMENU.flkmode = 0; 					// flickr mode default '0'(auo), '1'(off), '2'(on)	
	// set ae menu parameter with ae api
	for(i=0;i<5;i++)	{
		stAEMENU.shut[i] = GetAEShutSpeed();
		stAEMENU.iris[i] = GetAEIrisGain();
		stAEMENU.agc[i] = GetAEAGCGain();
		stAEMENU.agcmax[i] = GetAEGainCtrlMode();
		stAEMENU.sensup[i] = OFF;
		stAEMENU.bright[i] = GetAEBrightLvl();
	}
}

//------------------------- mimi test start ------
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
extern FLOAT afWinRatio;
void AF_SetConferenceWin(PMISP_VIDEO_WINDOW pWIN)
{
	MISP_RegField(MISP_LOCAL_ID, 0x6A0, 8, 6, pWIN->x);	// afw-a start x
	MISP_RegField(MISP_LOCAL_ID, 0x6A0, 0, 6, pWIN->y);	// afw-a start y
	MISP_RegField(MISP_LOCAL_ID, 0x6A1, 8, 6, pWIN->w);	// afw-a width
	MISP_RegField(MISP_LOCAL_ID, 0x6A1, 0, 6, pWIN->h);	// afw-a height

	MISP_RegField(MISP_LOCAL_ID, 0x6A2, 8, 6, pWIN->x+pWIN->w/4);	// afw-b start x
	MISP_RegField(MISP_LOCAL_ID, 0x6A2, 0, 6, pWIN->y+pWIN->h/4);	// afw-b start y
	MISP_RegField(MISP_LOCAL_ID, 0x6A3, 8, 6, pWIN->w/2);				// afw-b width
	MISP_RegField(MISP_LOCAL_ID, 0x6A3, 0, 6, pWIN->h/2);				// afw-b height

	afWinRatio = AFCTRL_CalAFRatio();
	//UARTprintf("conference : afWinRatio : %f\r\n", afWinRatio);
}

void AF_SetConferenceMode(int mode)
{
	if(mode == 0) {
		fConferenceMode = 0;
		MISP_RegWrite(MISP_LOCAL_ID, 0x752, 0x0000);
	} else {
		fConferenceMode = 1;
		MISP_RegWrite(MISP_LOCAL_ID, 0x752, 0x0008);
	}
}

int AF_GetConferenceMode(void)
{
	return( fConferenceMode );
}

#endif

//-----------------------------------------------------------------------------------------
static BOOL ChkAFConferenceWinSizeOver(PMISP_VIDEO_WINDOW pWIN)
{
	BOOL  err=0;
	WORD  Hact = GetIN_MainSrcSizeH()/40, Vact = GetIN_MainSrcSizeV()/32;
	
	if(pWIN->w + pWIN->x > Hact)		err=1;
	if(pWIN->h + pWIN->y > Vact)		err=1;
	
	return		err;
}

//-----------------------------------------------------------------------------------------
void SetAFConferenceMode(WORD nID)
{
	PMISP_VIDEO_WINDOW pWIN;
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	if(nID) {
		pWIN = &stCONF_WIN;	// conference win
		AECTRL_SetAE_IRISFixMode(); //set AE mode to IRIS fix
		AECTRL_SetConferIRISValue(0);
	} else {
		pWIN = &stAF_WIN;		// original AF win
	}
	//memcpy(&stCONF_WIN, pWIN, sizeof(MISP_VIDEO_WINDOW));

	if((stCONF_WIN.w == 0) || (stCONF_WIN.h == 0))	{ // blank check
		stCONF_WIN.x = stAF_WIN.x+stAF_WIN.w/4;
		stCONF_WIN.y = stAF_WIN.y+stAF_WIN.h/4;
		stCONF_WIN.w = stAF_WIN.w/2;
		stCONF_WIN.h = stAF_WIN.h/2;
		SetMenuStatus(FCS_CONFWINW_MENU, stCONF_WIN.w/2);
		SetMenuStatus(FCS_CONFWINH_MENU, stCONF_WIN.h/2);
		SetMenuStatus(FCS_CONFWINX_MENU, stCONF_WIN.x + stCONF_WIN.w/2);
		SetMenuStatus(FCS_CONFWINY_MENU, stCONF_WIN.y + stCONF_WIN.h/2);
	}

	//UARTprintf("conference : %d %d %d %d\r\n", stCONF_WIN.x,stCONF_WIN.y,stCONF_WIN.w,stCONF_WIN.h);
	//UARTprintf("original : %d %d %d %d\r\n", stAF_WIN.x,stAF_WIN.y,stAF_WIN.w,stAF_WIN.h);
	
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	AF_SetConferenceWin(pWIN);
	AF_SetConferenceMode((nID)?  ON:  OFF);
#endif
	
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_CONFER_MODE, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//-----------------------------------------------------------------------------------------
void SetAFConferenceWinWidth(WORD nID)
{
	PMISP_VIDEO_WINDOW  pWIN = &stCONF_WIN;
	WORD  old_w, old_x;
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	old_w = pWIN->w;		old_x = pWIN->x;
	pWIN->w = nID * 2;
	
	if(pWIN->w > old_w)		pWIN->x -= DIFF(pWIN->w, old_w)/2;	// width +1 ? -> x-position shift left
	else					pWIN->x += DIFF(pWIN->w, old_w)/2;	// winth -1 ? -> x-position shift right
	
	if(ChkAFConferenceWinSizeOver(pWIN))	{					// window size > video size ?
		pWIN->w = old_w;	pWIN->x = old_x;					// revert previous menu value..
		SetMenuStatus(FCS_CONFWINW_MENU, pWIN->w/2); 	SetMenuRedraw();return;//SetOSDMenuID(FCS_CONFWINW_MENU);	return;
	}
	
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	if(fConferenceMode) AF_SetConferenceWin(pWIN);
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_CONFER_WIDTH, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//-----------------------------------------------------------------------------------------
void SetAFConferenceWinHeight(WORD nID)
{
	PMISP_VIDEO_WINDOW  pWIN = &stCONF_WIN;
	WORD  old_h, old_y;
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	old_h = pWIN->h;		old_y = pWIN->y;
	pWIN->h = nID * 2;
	
	if(pWIN->h > old_h)		pWIN->y -= DIFF(pWIN->h, old_h)/2;	// height +1 ? -> y-position shift up
	else					pWIN->y += DIFF(pWIN->h, old_h)/2;	// height -1 ? -> y-position shift down
	
	if(ChkAFConferenceWinSizeOver(pWIN))	{
		pWIN->h = old_h;	pWIN->y = old_y;
		SetMenuStatus(FCS_CONFWINH_MENU, pWIN->h/2); 	SetMenuRedraw();return;//SetOSDMenuID(FCS_CONFWINH_MENU);	return;
	}
	
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	if(fConferenceMode) AF_SetConferenceWin(pWIN);
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_CONFER_HEIGHT, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//-----------------------------------------------------------------------------------------
void SetAFConferenceWinXPos(WORD nID)
{
	PMISP_VIDEO_WINDOW  pWIN = &stCONF_WIN;
	WORD  old_x;
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	old_x = pWIN->x;
	pWIN->x = nID - pWIN->w/2;									// center position(nID) -> x start position(x)
	
	if(ChkAFConferenceWinSizeOver(pWIN))		{
		pWIN->x = old_x;
		SetMenuStatus(FCS_CONFWINX_MENU, pWIN->x + pWIN->w/2);	SetOSDMenuID(FCS_CONFWINX_MENU);	return;
	}
	
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	if(fConferenceMode) AF_SetConferenceWin(pWIN);
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_CONFER_POSIX, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//-----------------------------------------------------------------------------------------
void SetAFConferenceWinYPos(WORD nID)
{
	PMISP_VIDEO_WINDOW  pWIN = &stCONF_WIN;
	WORD  old_y;
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	old_y = pWIN->y;
	pWIN->y = nID - pWIN->h/2;									// center position(nID) -> y start position(y)
	
	if(ChkAFConferenceWinSizeOver(pWIN))		{
		pWIN->y = old_y;
		SetMenuStatus(FCS_CONFWINY_MENU, pWIN->y + pWIN->h/2);	SetOSDMenuID(FCS_CONFWINY_MENU);	return;
	}
	
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	if(fConferenceMode) AF_SetConferenceWin(pWIN);
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_CONFER_POSIY, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//-----------------------------------------------------------------------------------------
BYTE GetAFConferenceMode(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	return  AF_GetConferenceMode();
#else
	return  0;
#endif
}

//-----------------------------------------------------------------------------------------
WORD GetAFConferenceWinWidth(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	return  (stCONF_WIN.w /2);
#else
	return  0;
#endif
}

//-----------------------------------------------------------------------------------------
WORD GetAFConferenceWinHeight(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	return  (stCONF_WIN.h /2);
#else
	return  0;
#endif
}

//-----------------------------------------------------------------------------------------
WORD GetAFConferenceWinXPos(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	return  (stCONF_WIN.x + stCONF_WIN.w/2);
#else
	return  0;
#endif
}

//-----------------------------------------------------------------------------------------
WORD GetAFConferenceWinYPos(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	return  (stCONF_WIN.y + stCONF_WIN.h/2);
#else
	return  0;
#endif
}

//-----------------------------------------------------------------------------------------
void DEMO_SetLEDPWMLowWidth(BYTE val)
{
#if defined(__USE_IR_LED_PWM__)
	BYTE high_width = GetMenuStatus(LEDPWMHIGH_MENU);
	
	if((val < 1) || (val+high_width > 33))	{
		if(val < 1)		val = 1;
		else			val = 33 - high_width;
		
		SetMenuStatus(LEDPWMLOW_MENU, val);		SetOSDMenuID(LEDPWMLOW_MENU);
	}
	
	SetTIMER_PWM5LowWidth(val);			// led pwm low pulse width
#endif
}

//-----------------------------------------------------------------------------------------
void DEMO_SetLEDPWMHighWidth(BYTE val)
{
#if defined(__USE_IR_LED_PWM__)
	BYTE low_width = GetMenuStatus(LEDPWMLOW_MENU);
	
	if((val < 1) || (val+low_width > 33))	{
		if(val < 1)		val = 1;
		else			val = 33 - low_width;
		
		SetMenuStatus(LEDPWMHIGH_MENU, val);	SetOSDMenuID(LEDPWMHIGH_MENU);
	}
	
	SetTIMER_PWM5HighWidth(val);		// led pwm high pulse width
#endif
}

#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
//-----------------------------------------------------------------------------------------
static void DEMO_CnvIrisGainToSTR(PBYTE pSTR, SHORT gain)
{
#if defined(__USE_X12X_UNION__)
	char iris_item[][5]={"F16", "F11", "F8", "F5.6", "F4", "F2.8", "F2", "F1.8", "----"};
#else
	char iris_item[][5]={"F16", "F11", "F8", "F5.6", "F4", "F2.8", "F2", "F1.6", "----"};
#endif
	BYTE i;
	
	for(i=0;i<8;i++)	if(gain <= AECTRL_F_VALUE[i])		{memcpy(pSTR, iris_item[i], sizeof(iris_item[i]));		return;}
	memcpy(pSTR, iris_item[i], sizeof(iris_item[i]));		// over max
}
#endif

//-----------------------------------------------------------------------------------------
static void DEMO_CnvShutGainToSTR(PBYTE pSTR, LONG gain)
{
#if defined(__USE_IMX274_4K15P_MODE__)
	char shut_item[][8]={"AUTO", "1/25", "1/30", "1/50 ", "1/60 ", "1/75 ", "1/90 ", "1/100", "1/120", "1/125", 
						"1/150", "1/180", "1/240", "1/250", "1/300", "1/500", "1/1k", "1/2.5k",	"1/5k", "-------"};
#else
	char shut_item[][8]={"AUTO", "1/50", "1/60", "1/100", "1/120", "1/150", "1/180", "1/200", "1/240", "1/250", 
						"1/300", "1/360", "1/480", "1/500", "1/600", "1/1k", "1/2k", "1/5k",	"1/10k", "-------"};
#endif
	BYTE i;
	LONG  shut, shut_den[19]={30, 50, 60, 100, 120, 150, 180, 200, 240, 250, 300, 360, 480, 500, 600, 1000, 2000, 5000, 10000};
	FLOAT sens_fps;

#if defined(__USE_IMX274_4K15P_MODE__)
	if(GetSS_SensorVoutPAL())	{sens_fps = 25.0f;		shut_den[0] = 25;		memcpy(shut_item[0], "1/12.5", 4);}
	else						{sens_fps = 30.0f;		shut_den[0] = 30;		memcpy(shut_item[0], "1/15", 4);}
#else
	if(GetSS_SensorVoutPAL())	{sens_fps = 25.0f;		shut_den[0] = 25;		memcpy(shut_item[0], "1/25", 4);}
	else						{sens_fps = 30.0f;		shut_den[0] = 30;		memcpy(shut_item[0], "1/30", 4);}
#endif	
	for(i=0;i<19;i++)	{
		shut = AECTRL_GetSHUTMax() - (sens_fps*AECTRL_GetSHUTMax()/shut_den[i]);
		shut = MIN((LONG)AECTRL_GetSHUTMax(), MAX(AECTRL_GetSHUTMax()-(sens_fps*AECTRL_GetSHUTMax()/shut_den[1])-1, shut));
		
		if(gain <= shut)		{memcpy(pSTR, shut_item[i], sizeof(shut_item[i]));		return;}
	}
	memcpy(pSTR, shut_item[i], sizeof(shut_item[i]));	// over max

}

//-----------------------------------------------------------------------------------------
extern int AE_GaindB;
extern DYNT_CTRL_PARAM stDYNTCTRL;
void DEMO_StatusDisplay(BOOL OnOff)
{
#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
	char iris_str[5];
#endif
	char shut_str[8];
	// LONG iris_gain;
	WORD nr_stdev_int, nr_stdev_frac, nr_3d_intensity;
	BYTE i, color=MAKEBYTE(OSD_WHITE, OSD_BLACK);

	WORD bright_now=GetDM_ColorCC_Brightness();
	DWORD boot_cnt = GetSYSINFO_BootCnt();
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	if(EEPROM_RegRead(EEPROM_BOOT_CNT) != EEPROM_SAVE_DONE)	{
		boot_cnt = 0;												// clear boot counter
		EEPROM_RegWrite(EEPROM_BOOT_CNT, EEPROM_SAVE_DONE);
		EEPROM_MultiWrite(EEPROM_BOOT_CNT+2, (PBYTE)&boot_cnt, 4);
	}
#endif

	if(OnOff==OFF)			{OSD_StrDispOnOff(OFF);		return;}			// disable osd sprite
	
	// get status
	GetNR_StDEV(&nr_stdev_int,&nr_stdev_frac,0);
	nr_3d_intensity  = GetNR_3Dconst_k();
	
	// convert status value to string
#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
	memset(iris_str, 0, sizeof(iris_str));		DEMO_CnvIrisGainToSTR((PBYTE)iris_str, DCLENS_GetIRISGain());
#endif
	memset(shut_str, 0, sizeof(shut_str));		DEMO_CnvShutGainToSTR((PBYTE)shut_str, AECTRL_GetSHUTGain());
	
	// display status osd
	for(i=0;i<19;i++)	OSD_StrDispAttb(i, 0, OFF, color, "%36s", ""); // clear osd

#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
	OSD_StrDispAttb( 0,  0, ON, color, "IRIS : %5s(%3d)|%s(Ver%X)", iris_str, DCLENS_GetIRISGain(), __DATE__, __FW_VERSION__);
#elif defined(__USE_PWMIRIS_ONLY__)
	extern WORD offsetIRIS;
	WORD hval;
	MISP_RegRead(MISP_LOCAL_ID, 0x67E,&hval);
	OSD_StrDispAttb( 0,  0, ON, color, "PWM :  %4d(%4d)|%s(Ver%X)", hval, offsetIRIS, __DATE__, __FW_VERSION__);
#else
	OSD_StrDispAttb( 0,  0, ON, color, "Date : %s(ver. %X)",  __DATE__, __FW_VERSION__);
#endif
	OSD_StrDispAttb( 1,  0, ON, color, "AGC  : %3ddB(%5Xh) + DiG  : %4.1fx", AE_GaindB, AECTRL_GetGAINGain(), (FLOAT)Get_AE_DGain()/256.0f);
	OSD_StrDispAttb( 2,  0, ON, color, "Shut : %5s(%6d) + SenUp: %4.1fx", shut_str, AECTRL_GetSHUTGain(), (FLOAT)AECTRL_GetSensUpVal()/(FLOAT)AECTRL_GetSensUpMin());
	OSD_StrDispAttb( 3,  0, ON, color, "YSum : %5d(%6d) = %5d", Get_AE_Ysum(), Get_AE_Yref(), Get_AE_Ysum() - Get_AE_Yref());
#if 1
	if(bright_now<128)
		OSD_StrDispAttb( 4,  0, ON, color, "D2N:%5Xh/%5Xh(%1d) | BRGHT: %5d",( stDYNTCTRL.agc_d2n*0x40000/256 + 256),( stDYNTCTRL.agc_n2d*0x40000/256 + 256),GetDYNT_Status(),bright_now);
	else
		OSD_StrDispAttb( 4,  0, ON, color, "D2N:%5Xh/%5Xh(%1d) | BRGHT: %5d",( stDYNTCTRL.agc_d2n*0x40000/256 + 256),( stDYNTCTRL.agc_n2d*0x40000/256 + 256),GetDYNT_Status(),-(256-bright_now));
#else
	if(bright_now<128)
		OSD_StrDispAttb( 4,  0, ON, color, "T.SAT: %5d         | BRGHT: %5d", target_sat, bright_now);
	else
		OSD_StrDispAttb( 4,  0, ON, color, "T.SAT: %5d         | BRGHT: %5d", target_sat, -(256-bright_now));
#endif

#if (defined(__USE_AFZOOM_LENS__	)||defined(__USE_2MOTOR_LENS__))
	if (stAFCTRL.nMOD&SPOT_XX)
		OSD_StrDispAttb( 5,  0, ON, color, "Focus: %5d(%6s)", LENS_GetRelPositionFOKS(), "Spot");
	else if (stAFCTRL.nMOD==NORM_XX)
		OSD_StrDispAttb( 5,  0, ON, color, "Focus: %5d(%6s)", LENS_GetRelPositionFOKS(), "Normal");
	else
		OSD_StrDispAttb( 5,  0, ON, color, "Focus: %5d(%6s)", LENS_GetRelPositionFOKS(), "Dark");

	OSD_StrDispAttb( 6,  0, ON, color, "Zoom : %5d(DZ: %2d)", LENS_GetRelPositionZOOM(),LENS_GetCurPositionDZOOM());
#else
	OSD_StrDispAttb( 5,  0, ON, color, "DDPC  : PWR-%d NOW", target_ddpc);
	//OSD_StrDispAttb( 5,  0, ON, color, "Test : %5d)", 100);
	//OSD_StrDispAttb( 6,  0, ON, color, "Test : %5d)", 100);
#endif

	//OSD_StrDispAttb( 7,  0, ON, color, "Test : %5d)", 100);
	OSD_StrDispAttb( 7,  0, ON, color, " -Boot cnt: ok(%5d) / total(%5d)", HIWORD(boot_cnt), LOWORD(boot_cnt));

	OSD_StrDispAttb( 8,  0, ON, color, "NR STDEV : %d.%d", nr_stdev_int, nr_stdev_frac);
	OSD_StrDispAttb( 9,  0, ON, color, "NR 3D Intensity : %d[7-40]", nr_3d_intensity);

#if defined(__USE_WDR_PROCESS__)
	OSD_StrDispAttb(10,  0, ON, color, GetWdr_DebugStr(0));
	OSD_StrDispAttb(11,  0, ON, color, GetWdr_DebugStr(1));
	OSD_StrDispAttb(12,  0, ON, color, GetWdr_DebugStr(2));
	OSD_StrDispAttb(13,  0, ON, color, GetWdr_DebugStr(3));
	OSD_StrDispAttb(14,  0, ON, color, GetWdr_DebugStr(4));
	OSD_StrDispAttb(15,  0, ON, color, GetWdr_DebugStr(5));
	OSD_StrDispAttb(16,  0, ON, color, GetWdr_DebugStr(6));
	OSD_StrDispAttb(17,  0, ON, color, GetWdr_DebugStr(7));
	OSD_StrDispAttb(18,  0, ON, color, GetWdr_DebugStr(8));
#endif

	OSD_StrDispOnOff(ON);				// enable osd sprite

#if 1

	UARTprintf("------------------------------------\r\n");
#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
	UARTprintf("IRIS : %5s(%3d)|%s(Ver%X)\r\n", iris_str, DCLENS_GetIRISGain(), __DATE__, __FW_VERSION__);
#elif defined(__USE_PWMIRIS_ONLY__)
	MISP_RegRead(MISP_LOCAL_ID, 0x67E,&hval);
	UARTprintf("PWM : %4d(%4d)|%s(Ver%X)\r\n", hval, offsetIRIS, __DATE__, __FW_VERSION__);
#else
	UARTprintf("Date : %s(ver. %X)",  __DATE__, __FW_VERSION__);
#endif
	UARTprintf("AGC  : %3ddB(%6d) + DiG  : %5d\r\n", AE_GaindB, AECTRL_GetGAINGain(), Get_AE_DGain());
	UARTprintf("Shut : %5s(%6d) + SenUp: %3.2f\r\n", shut_str, AECTRL_GetSHUTGain(), (FLOAT)AECTRL_GetSensUpVal()/(FLOAT)AECTRL_GetSensUpMin());
	UARTprintf("YSum : %5d(%6d) = %5d\r\n", Get_AE_Ysum(), Get_AE_Yref(), Get_AE_Ysum() - Get_AE_Yref());
#if 1
	if(bright_now<128)
		UARTprintf("D2N:%5Xh/%5Xh(%1d) | BRGHT: %5d\r\n",( stDYNTCTRL.agc_d2n*0x40000/256 + 256),( stDYNTCTRL.agc_n2d*0x40000/256 + 256),GetDYNT_Status(),bright_now);
	else
		UARTprintf("D2N:%5Xh/%5Xh(%1d) | BRGHT: %5d\r\n",( stDYNTCTRL.agc_d2n*0x40000/256 + 256),( stDYNTCTRL.agc_n2d*0x40000/256 + 256),GetDYNT_Status(),-(256-bright_now));
#else
	if(bright_now<128)
		UARTprintf("T.SAT: %5d         | BRGHT: %5d\r\n", target_sat, bright_now);
	else
		UARTprintf("T.SAT: %5d         | BRGHT: %5d\r\n", target_sat, -(256-bright_now));
#endif

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	if (stAFCTRL.nMOD&SPOT_XX)
		UARTprintf("Focus: %5d(%6s)\r\n", LENS_GetRelPositionFOKS(), "Spot");
	else if (stAFCTRL.nMOD==NORM_XX)
		UARTprintf("Focus: %5d(%6s)\r\n", LENS_GetRelPositionFOKS(), "Normal");
	else
		UARTprintf("Focus: %5d(%6s)\r\n", LENS_GetRelPositionFOKS(), "Dark");

	UARTprintf("Zoom : %5d(DZ: %2d)\r\n", LENS_GetRelPositionZOOM(),LENS_GetCurPositionDZOOM());
#endif

	//UARTprintf("Test : %5d)", 100);
	UARTprintf(" -Boot cnt: ok(%5d) / total(%5d) \r\n", HIWORD(boot_cnt), LOWORD(boot_cnt));

	UARTprintf("NR STDEV : %d.%d\r\n", nr_stdev_int, nr_stdev_frac);
	UARTprintf("NR 3D Intensity : %d[7-28]\r\n", nr_3d_intensity);

#if defined(__USE_WDR_PROCESS__)
	UARTprintf("%s\r\n",GetWdr_DebugStr(0));
	UARTprintf("%s\r\n",GetWdr_DebugStr(1));
	UARTprintf("%s\r\n",GetWdr_DebugStr(2));
	UARTprintf("%s\r\n",GetWdr_DebugStr(3));
	UARTprintf("%s\r\n",GetWdr_DebugStr(4));
	UARTprintf("%s\r\n",GetWdr_DebugStr(5));
	UARTprintf("%s\r\n",GetWdr_DebugStr(6));
	UARTprintf("%s\r\n",GetWdr_DebugStr(7));
	UARTprintf("%s\r\n",GetWdr_DebugStr(8));
#endif
	UARTprintf("------------------------------------\r\n");
#endif
}

//------------------------------------------------------------------------------------
BYTE LoadAELensType(void)		// 0:manual, 1:dc-iris
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
	
	EEPROM_MultiRead(EEPROM_AE_LENS, (PBYTE)&buff[0], sizeof(buff));
	
	if((buff[0] == EEPROM_SAVE_DONE) && (buff[1] < 2))		return buff[1];	// 0:manual, 1:dc-iris
#endif
	return		0;
}

//------------------------------------------------------------------------------------
void SaveAELensType(BYTE type)		// 0:manual, 1:dc-iris
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = type;
	EEPROM_MultiWrite(EEPROM_AE_LENS, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//------------------------------------------------------------------------------------
void SaveMenuColor(WORD id, WORD val)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
	switch(id)	{
		case  1:  EEPROM_MultiWrite(EEPROM_MCOLOR_TEXT, (PBYTE)&buff[0], sizeof(buff));	break; 	// text color
		case  2:  EEPROM_MultiWrite(EEPROM_MCOLOR_TEXTH, (PBYTE)&buff[0], sizeof(buff));	break; 	// text(h) color
		case  3:  EEPROM_MultiWrite(EEPROM_MCOLOR_BG, (PBYTE)&buff[0], sizeof(buff));	break; 		// bg color
		case  4:  EEPROM_MultiWrite(EEPROM_MCOLOR_BGH, (PBYTE)&buff[0], sizeof(buff));	break; 	// bg(h) color
	}
#endif
}

//------------------------------------------------------------------------------------
void SaveCamTitleColor(BYTE val)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)val;
	EEPROM_MultiWrite(EEPROM_CTITLE_COLOR, (PBYTE)&buff[0], sizeof(buff));
#endif
}

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
//--------------------------------------------------------------------------------------------------------------------------
/*
BYTE circular_shift(BYTE range, BYTE pos, S8 shift)
{
	S8 sum = pos + shift;
	BYTE shifted_pos;
	if (range <= 0){
		UARTprintf("Invalid range. (range <= 0)\n");
		return -1;
	}
	if (sum > range)	shifted_pos = sum % range;
	else if(0 > sum)	shifted_pos = (range - (-sum % range)) % range;
	else	shifted_pos = sum;
	return shifted_pos;
}
*/

static void FLKDET_pattern_cal(PFLK_CTRL_PARAM pCTRL)		// 151001 FLK_pattern_detection
{
	BYTE i, j;
	BYTE pattern;
	BYTE min_err, max_err;
	BYTE min_err_dev, max_err_dev;
	BYTE p_err_min_temp;
	BYTE p_err_min = 63;
	BYTE p_err_mean;	// pattern error mean without minimum pattern error
	
	WORD min_err_acc, max_err_acc;	// min,max error accumulated value
	BOOL min_max_sel_temp;
	BOOL min_max_sel = FALSE;
	min_err_acc = 0;
	max_err_acc = 0;
	for(j=0;j<pCTRL->fr_limit-1;j++)	{
		min_err = 0;
		max_err = 0;
		if (GetSS_SensorVoutPAL()){		// fps: 25Hz, light: 60Hz (power)
			for(i=0;i<pCTRL->raw_limit;i++)	{
			//	pattern = circular_shift(pCTRL->fr_limit-1, j, (S8)(-i));
				pattern = (pCTRL->raw_limit-i-j)%(pCTRL->fr_limit-1);
				min_err_dev = ABS(pCTRL->min_p[i] - pattern);
				max_err_dev = ABS(pCTRL->max_p[i] - pattern);
				min_err += min_err_dev > 1 ? 2 : min_err_dev;
				max_err += max_err_dev > 1 ? 2 : max_err_dev;
			}
		}else{							// fps: 30Hz, light: 50Hz (power)
			for(i=0;i<pCTRL->raw_limit;i+=2){
			//	pattern = circular_shift(pCTRL->fr_limit-1, j, (S8)(-i));
				pattern = (pCTRL->raw_limit-i-j)%(pCTRL->fr_limit-1);
				min_err += pCTRL->min_p[i] == pattern ? 0: 1;
				max_err += pCTRL->max_p[i] == pattern ? 0: 1;
			}
			for(i=1;i<pCTRL->raw_limit;i+=2){
			//	pattern = circular_shift(pCTRL->fr_limit-1, j, (S8)(-i));
				pattern = (pCTRL->raw_limit-i-j)%(pCTRL->fr_limit-1);
				min_err += pCTRL->max_p[i] == pattern ? 0: 1;
				max_err += pCTRL->min_p[i] == pattern ? 0: 1;
			}
		}
		min_err_acc += min_err;
		max_err_acc += max_err;

		if(max_err > min_err){
			p_err_min_temp = min_err;
			min_max_sel_temp = FALSE;
		}else{
			p_err_min_temp = max_err;
			min_max_sel_temp = TRUE;
		}

		if (p_err_min > p_err_min_temp)	{
			p_err_min = p_err_min_temp;
			min_max_sel = min_max_sel_temp;
		}
	//	UARTprintf("Min: %3d, Max: %3d (Min/Max:%d, Min of min: %d(%d))\n",min_err, max_err, min_max_sel_temp, p_err_min, min_max_sel);	// test
		//UARTprintf("Min: %3d, Max: %3d \r\n",min_err, max_err);	// test
	}

	p_err_mean = min_max_sel ? (max_err_acc - p_err_min)/(pCTRL->fr_limit - 2) : (min_err_acc - p_err_min)/(pCTRL->fr_limit - 2);
	//UARTprintf("Min acc.: %d, Max acc.: %d\r\n", min_err_acc, max_err_acc);	// test
	
	pCTRL->p_flk = p_err_mean > (p_err_min + pCTRL->p_err_limit) ? TRUE : FALSE;
	//UARTprintf("Average: %d, Min error: %d\r\n",p_err_mean, p_err_min);	// test
}

static void FLKDET_calc_raw(PFLK_CTRL_PARAM pCTRL)
{
	BYTE i, j;
	BYTE min_p = 0;
	BYTE max_p = 0;
	WORD min[24], max[24];
	DWORD dev_min[6];
	
	// calc min, max, mean
	pCTRL->min_mean = 0;	pCTRL->max_mean = 0;		// clear min/max mean
	
	for(i=0;i<pCTRL->raw_limit;i++)	{
		for(j=0;j<pCTRL->fr_limit;j++)	{
			if(j==0)	{
				min[i] = pCTRL->raw[j][i];
				max[i] = pCTRL->raw[j][i];
			}
			else	{
				if(min[i] > pCTRL->raw[j][i])	{
					min[i] = pCTRL->raw[j][i];		// min
					min_p = j;						// min position
				}
				if(max[i] < pCTRL->raw[j][i]){
					max[i] = pCTRL->raw[j][i];		// max
					max_p = j;						// max position
				}
			}
		}
		pCTRL->min_mean += min[i];													// mean(total)
		pCTRL->max_mean += max[i];
		pCTRL->min_p[i] = min_p;			// position
		pCTRL->max_p[i] = max_p;	
	}
	pCTRL->min_mean /= pCTRL->raw_limit;											// mean
	pCTRL->max_mean /= pCTRL->raw_limit;
	
	
	// calc dev-min
	memset(&dev_min[0], 0, sizeof(dev_min));			// clear buffer
	
	for(i=0;i<pCTRL->raw_limit;i++)	{
		for(j=1;j<pCTRL->fr_limit;j++)	{
			dev_min[j] += DIFF(pCTRL->raw[0][i], pCTRL->raw[j][i]);								// dev min
		}
	}
	
	// search dev-min minimum frame number
	dev_min[0] = 0xFFFFFFFF;	pCTRL->dev_min_fr = 0;
	
	for(j=1;j<pCTRL->fr_limit;j++)	{
		if(dev_min[0] > dev_min[j])		{dev_min[0] = dev_min[j];	pCTRL->dev_min_fr=j;}
	}
	
	FLKDET_pattern_cal(pCTRL);	// pattern detection
}

//--------------------------------------------------------------------------------------------------------------------------
//static SHORT FLKshut = 0;
static BOOL AutoFLKflag = FALSE;
static void FLKDET_Process(PFLK_CTRL_PARAM	pCTRL)
{
	int i;

	if(AECTRL_IsAE_FLKMode() == FALSE) return;

#if 1
	if(AutoFLKflag == TRUE) {
		DEMO_SetSensorFpsToggle();
		AutoFLKflag = FALSE;
	}
#else
	FLOAT sens_fps=1.0f*GetSS_SensorVoutFps();
	LONG  shut_den;  //denominator
	LONG shut;

	if(GetSS_SensorVoutPAL())
		shut_den =   100;
	else 
		shut_den =   120;

	//update shutter value
	shut = AECTRL_GetSHUTMax() - (sens_fps*AECTRL_GetSHUTMax()/shut_den);
	shut = MIN((LONG)AECTRL_GetSHUTMax(), MAX(0, shut));

	if(AutoFLKflag == TRUE) {
		if(FLKshut > shut) {
			FLKshut -= 5;
			AECTRL_SetSHUTGain(FLKshut);
		} else {
			AutoFLKflag = FALSE;
		}
		//UARTprintf("AutoFLKremove:%d\r\n",AutoFLKremove);
	} else {
		FLKshut = AECTRL_GetSHUTGain();
	}

	if(AECTRL_GetSHUTGain() <= shut) {
		if(AECTRL_GetGAINGain() > AECTRL_GetGAINPMax()/32) {
			AutoFLKdetected = FALSE;
			//UARTprintf("%d:FLICKER : detect false...%d/%d\r\n",AutoFLKdetected,shut,AECTRL_GetSHUTGain());
			//OSD_MessageBox(ON, "Flicker Off#1");
		}
		return;
	}
#endif

	if(pCTRL->mean_thres1==0)		pCTRL->mean_thres1 = 100;
	if(pCTRL->mean_thres2==0)		pCTRL->mean_thres2 = 5000;

	// get sensor info.
	switch(pCTRL->ctrl & FLKCTRL_SS_FPS_MASK)	{
		case  FLKCTRL_SS_FPS_25HZ:	
			if(GetSS_SensorVoutPAL()==0)	{
				if(GetSS_image_active_size_v() == 720){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x2313);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_30HZ;
				}else if(GetSS_image_active_size_v() == 1536){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x4b13);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_30HZ;
				}else if(GetSS_image_active_size_v() == 1728){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x5513);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_30HZ;
				}else if(GetSS_image_active_size_v() == 1944){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x6013);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_30HZ;					
				}else if(GetSS_image_active_size_v() == 2048){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x6513);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_30HZ;					
				}else if(GetSS_image_active_size_v() == 2160){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x6b13);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_30HZ;					
				}else{
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x3513);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_30HZ;				
				}
			}
			break;
		case  FLKCTRL_SS_FPS_30HZ:	
			if(GetSS_SensorVoutPAL()==1)	{
				if(GetSS_image_active_size_v() == 720){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x1d17);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_25HZ;
				}else if(GetSS_image_active_size_v() == 1536){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x3F17);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_25HZ;
				}else if(GetSS_image_active_size_v() == 1728){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x4717);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_25HZ;
				}else if(GetSS_image_active_size_v() == 1944){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x5017);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_25HZ;					
				}else if(GetSS_image_active_size_v() == 2048){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x5517);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_25HZ;					
				}else if(GetSS_image_active_size_v() == 2160){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x5917);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_25HZ;					
				}else{
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x2c17);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_25HZ;
				}
			}
			break;
		default:					
			if(GetSS_SensorVoutPAL()==1)	{
				if(GetSS_image_active_size_v() == 720){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x1d17);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_25HZ;
				}else if(GetSS_image_active_size_v() == 1536){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x3F17);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_25HZ;
				}else if(GetSS_image_active_size_v() == 1728){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x4717);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_25HZ;
				}else if(GetSS_image_active_size_v() == 1944){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x5017);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_25HZ;					
				}else if(GetSS_image_active_size_v() == 2048){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x5517);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_25HZ;					
				}else if(GetSS_image_active_size_v() == 2160){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x5917);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_25HZ;					
				}else{
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x2c17);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_25HZ;
				}
			}
			else	{
				if(GetSS_image_active_size_v() == 720){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x2313);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_30HZ;
				}else if(GetSS_image_active_size_v() == 1536){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x4b13);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_30HZ;
				}else if(GetSS_image_active_size_v() == 1728){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x5513);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_30HZ;
				}else if(GetSS_image_active_size_v() == 1944){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x6013);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_30HZ;					
				}else if(GetSS_image_active_size_v() == 2048){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x6513);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_30HZ;					
				}else if(GetSS_image_active_size_v() == 2160){
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x6b13);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_30HZ;					
				}else{
					MISP_RegWrite(MISP_LOCAL_ID, 0x028C, 0x3513);
					pCTRL->ctrl &= (~FLKCTRL_SS_FPS_MASK);	pCTRL->ctrl |= FLKCTRL_SS_FPS_30HZ;				
				}
			}
			break;
	}
	
	pCTRL->raw_limit = GetSS_SensorVoutPAL()? 24: 20;			// 25hz(24 line), 30hz(20 line)
	pCTRL->fr_limit = GetSS_SensorVoutPAL()? 6: 4;				// 25hz(6 frame), 30hz(4 frame)
	pCTRL->p_err_limit = GetSS_SensorVoutPAL()? 18: 7;			// error threshold: 25hz(20), 30hz(8)
	
	// raw data buffer shifting
	for(i=(pCTRL->fr_limit-1); i>0; i--)	{
		memcpy(&pCTRL->raw[i][0], &pCTRL->raw[i-1][0], pCTRL->raw_limit*2);	// raw data[0] -> 1,2,3,4,5
	}
	
	// read flicker raw data(horizontal accumulation data)
	for(i=0;i<pCTRL->raw_limit;i++)	{
		MISP_RegWrite(MISP_LOCAL_ID, 0x028E, i);				// indirect reg. read mode, start addr(0x00)
		MISP_RegRead(MISP_LOCAL_ID, 0x028F, &pCTRL->raw[0][i]);
//		UARTprintf("%04x ", pCTRL->raw[0][i]);
	}
//	UARTprintf("\r\n\n");
	
	
	// calc flicker raw data(min/max mean, dev min frame no)
	FLKDET_calc_raw(pCTRL);	// 140us
	
	// check flicker detection
	if((DIFF(pCTRL->min_mean, pCTRL->max_mean) > pCTRL->mean_thres1) && \
		(DIFF(pCTRL->min_mean, pCTRL->max_mean) < pCTRL->mean_thres2))	{				// criteria 1
			//UARTprintf("FLICKER : criteria 1, %d > %d > %d !!! \r\n", 
			//		pCTRL->mean_thres2, DIFF(pCTRL->min_mean, pCTRL->max_mean), pCTRL->mean_thres1);
			//UARTprintf("FLICKER : Valid boundary.(%d) \r\n", DIFF(pCTRL->min_mean, pCTRL->max_mean));
		if(pCTRL->dev_min_fr == pCTRL->fr_limit-1 && pCTRL->p_flk)	{									// criteria 2
			//UARTprintf("FLICKER : detected, criteria 2, dev min fr(%d), valid flicker pattern. \r\n", pCTRL->dev_min_fr);
			//OSD_MessageBox(ON, "Flicker detected");
			if(AutoFLKcnt < 30)
				AutoFLKcnt++; // 5 seconds
			else {
				//OSD_MessageBox(ON, "Flicker On   ");
				AutoFLKdetected = TRUE;
				AutoFLKremove = TRUE;
				AutoFLKflag = TRUE;
				//AECTRL_SetSHUTGain((SHORT)shut);
				//UARTprintf("%d:FLICKER : detected...%d/%d\r\n",AutoFLKdetected,shut,AECTRL_GetSHUTGain());
			}
		}
		else{
			//UARTprintf("%d:FLICKER : detection fail...%d/%d\r\n",AutoFLKdetected,shut,AECTRL_GetSHUTGain());
		}
	} else {
		if(AutoFLKcnt > 0)
			AutoFLKcnt--;
		else {
			AutoFLKdetected = FALSE;
			//OSD_MessageBox(ON, "Flicker Off#2");
			//UARTprintf("%d:FLICKER : detect off...%d/%d\r\n",AutoFLKdetected,shut,AECTRL_GetSHUTGain());
		}
	}
	//UARTprintf("AutoFLKcnt : %d\r\n",AutoFLKcnt);
}

//-----------------------------------------------------------------------------------------
void FLK_ProcessHandler(void)
{
	if ((stFLKCTRL.ctrl&FLKCTRL_PROCESS_ON)==0) return;	// check flicker process enable ?
	
	FLKDET_Process(&stFLKCTRL);
	
//	WORD  val;
//	MISP_RegRead(MISP_LOCAL_ID, 0x297, &val);	// flicker detect result
//	if(val & 1)		OSD_MessageBox(ON, "Flicker detected");
}

//-----------------------------------------------------------------------------------------
void FLK_ProcessOnOff(BOOL OnOff)
{
	if(OnOff)		stFLKCTRL.ctrl |= FLKCTRL_PROCESS_ON;
	else			stFLKCTRL.ctrl &= (~FLKCTRL_PROCESS_ON);
}

//--------------------------------------------------------------------------------------------------
void DEMO_FLKDET_MODE(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif

	switch(nID){	
		case 0 :
				FLK_ProcessOnOff(OFF);
				break;
	
		case 1 : 
				FLK_ProcessOnOff(ON);
				break;
	}
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_SPECIAL_FLK, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BYTE Get_FLKDet_Mode(void)
{
	LONG ltemp;

	ltemp = stFLKCTRL.ctrl&FLKCTRL_PROCESS_ON;

	if(ltemp == 0)	return 0;
	else			return 1;
}

//-----------------------------------------------------------------------------------------
void Demo_SetChromaSuppress(WORD nID)
{
	PCSUP_THRES	pThres;
	
	if(nID==OFF)	{
		MISP_RegField(MISP_LOCAL_ID, 0x7D3,   0,  1, 1);	// chroma suppress bypass(disable)
		return;
	}
	else	{
		MISP_RegField(MISP_LOCAL_ID, 0x7D3,   3,  1, 1);		// chroma suppress vd enable
		MISP_RegField(MISP_LOCAL_ID, 0x7D3,   2,  1, 1);		// chroma suppress ls enable
		MISP_RegField(MISP_LOCAL_ID, 0x7D3,   1,  1, 1);		// chroma suppress ld enable
		MISP_RegField(MISP_LOCAL_ID, 0x7D3,   0,  1, 0);		// chroma suppress enable
	}
	
	pThres = (PCSUP_THRES)&ChromaSup_thres[nID-1];	// suppress enable & set thres
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x7D4, MAKEWORD(pThres->ld_x[1], pThres->ld_x[0]));	// ld_x0,1
	MISP_RegWrite(MISP_LOCAL_ID, 0x7D5, MAKEWORD(pThres->ld_x[3], pThres->ld_x[2]));	// ld_x2,3
	MISP_RegField(MISP_LOCAL_ID, 0x7D6,   0,  9, pThres->ld_y[0]);						// ld_y0
	MISP_RegField(MISP_LOCAL_ID, 0x7D7,   0,  9, pThres->ld_y[1]);						// ld_y1
	MISP_RegField(MISP_LOCAL_ID, 0x7D8,   0,  9, pThres->ld_y[2]);						// ld_y2
	MISP_RegField(MISP_LOCAL_ID, 0x7D9,   0,  9, pThres->ld_y[3]);						// ld_y3
	MISP_RegField(MISP_LOCAL_ID, 0x7DA,   0, 13, pThres->ld_s[0]);						// ld_s0
	MISP_RegField(MISP_LOCAL_ID, 0x7DB,   0, 13, pThres->ld_s[1]);						// ld_s1
	MISP_RegField(MISP_LOCAL_ID, 0x7DC,   0, 13, pThres->ld_s[2]);						// ld_s2
	MISP_RegWrite(MISP_LOCAL_ID, 0x7DD, MAKEWORD(pThres->ls_x[1], pThres->ls_x[0]));	// ls_x0,1
	MISP_RegField(MISP_LOCAL_ID, 0x7DE,   0, 13, pThres->ls_s);							// ls_s
	MISP_RegWrite(MISP_LOCAL_ID, 0x7DF, MAKEWORD(pThres->vd_x[1], pThres->vd_x[0]));	// vd_x0,1
	MISP_RegWrite(MISP_LOCAL_ID, 0x7E0, MAKEWORD(pThres->vd_x[3], pThres->vd_x[2]));	// vd_x2,3
	MISP_RegField(MISP_LOCAL_ID, 0x7E1,   0,  9, pThres->vd_y[0]);						// vd_y0
	MISP_RegField(MISP_LOCAL_ID, 0x7E2,   0,  9, pThres->vd_y[1]);						// vd_y1
	MISP_RegField(MISP_LOCAL_ID, 0x7E3,   0,  9, pThres->vd_y[2]);						// vd_y2
	MISP_RegField(MISP_LOCAL_ID, 0x7E4,   0,  9, pThres->vd_y[3]);						// vd_y3
	MISP_RegField(MISP_LOCAL_ID, 0x7E5,   0, 13, pThres->vd_s[0]);						// vd_s0
	MISP_RegField(MISP_LOCAL_ID, 0x7E6,   0, 13, pThres->vd_s[1]);						// vd_s1
	MISP_RegField(MISP_LOCAL_ID, 0x7E7,   0, 13, pThres->vd_s[2]);						// vd_s2
	
//	UARTprintf("Demo_SetChromaSuppress[%s], lev(%d) \r\n", (nID)? "ON": "OFF", nID);	//test
}


void Demo_SetDemosaicChromaSuppress(WORD nID)
{
	if(nID==OFF)	{
		MISP_RegWrite(MISP_LOCAL_ID, 0x47C, 0x0040) ;		// demosaic chroma suppression OFF
		return;
	}
	else	{
		MISP_RegWrite(MISP_LOCAL_ID, 0x47C, 0x8040) ;		// demosaic chroma suppression ON
	}
}

//-----------------------------------------------------------------------------------------
void Demo_SetRotation(WORD nID)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveRotation(nID);
#endif
	
	SetROT_RotationEnable(nID);
	
	// mirror process for rotate left/right
	switch(nID)	{
		case  0:  DEMO_SetMirrorMode(0);	SetMenuStatus(DISP_MIRROR_MENU, OFF);	break;	// mirror off
		case  1:  DEMO_SetMirrorMode(2);	SetMenuStatus(DISP_MIRROR_MENU, 2);		break;	// rotate_left_full : rotate + v_mirror + full screen
		case  2:  DEMO_SetMirrorMode(2);	SetMenuStatus(DISP_MIRROR_MENU, 2);		break;	// rotate_left_1:1 : rotate + v_mirror + 1:1
		case  3:  DEMO_SetMirrorMode(1);	SetMenuStatus(DISP_MIRROR_MENU, 1);		break;	// rotate_right_full : rotate + h_mirror + full screen
		case  4:  DEMO_SetMirrorMode(1);	SetMenuStatus(DISP_MIRROR_MENU, 1);		break;	// rotate_right_1:1 : rotate + h_mirror + 1:1
	}
}

//-----------------------------------------------------------------------------------------
void DEMO_SetLVDSPLL(WORD nID)	// for test
{
	switch(nID)	{
		case  0:		SetCLK_UserPLL(1,3, 66,2,1);	break;	// 15frame(144Mhz)
//		case  0:		SetCLK_UserPLL(1,3, 64,2,1);	break;	// 15frame(144Mhz)
		case  1:		SetCLK_UserPLL(1,3, 74,2,1);	break;	// 17frame(166Mhz)
		case  2:		SetCLK_UserPLL(1,3, 86,2,1);	break;	// 20frame(193Mhz)
		case  3:		SetCLK_UserPLL(1,3, 94,2,1);	break;	// 22frame(211Mhz)
		case  4:		SetCLK_UserPLL(1,3,107,2,1);	break;	// 25frame(241Mhz)
		case  5:		SetCLK_UserPLL(1,3,128,2,1);	break;	// 30frame(288Mhz)
	}
}
//-----------------------------------------------------------------------------------------
void Demo_Set_OutFHD_Crop(void)
{
#if defined(__USE_FHDCROP_MODE__)
	SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p60) ;	// output sync 1080p60
	
	SetFC_MainFCsrcPosiX(move_in_LR*64); // H source position 1/4 of 4K
	SetFC_MainFCsrcPosiY(move_in_UD*36); // V source position 1/4 of 4K	
	
	SetFC_MainFCsrcSizeH(GetOUT_MainSizeHA());	  // Same as output H size 1920
	SetFC_MainFCsrcSizeV(GetOUT_MainSizeVA());	  // Same as output V size 1080	
	
	SetFC_MainFCdstPosiX(0);	
	SetFC_MainFCdstPosiY(0);
	SetFC_MainFCdstSizeH(GetOUT_MainSizeHA());	  // Set FC H window size 1920
	SetFC_MainFCdstSizeV(GetOUT_MainSizeVA());	  // Set FC V window size 1080

	fMainCrop = TRUE ;
	fCropMode = TRUE ;
#elif defined(__USE_SENS_PANNING_MODE__)
	SetFC_MainFCsrcPosiX(64); // 1920+128=2048--> 0x007 in_size_h_main, 128/2 = 64
	SetFC_MainFCsrcPosiY(64); // 1080+128=1208--> 0x008 in_size_v_main,, 128/2 = 64
	SetFC_MainFCsrcSizeH(1920);	  
	SetFC_MainFCsrcSizeV(1080);	  	
	
	SetFC_MainFCdstPosiX(0);	
	SetFC_MainFCdstPosiY(0);
	SetFC_MainFCdstSizeH(1920);	  // Set FC H window size 1920
	SetFC_MainFCdstSizeV(1080);	  // Set FC V window size 1080
#endif
}
//-----------------------------------------------------------------------------------------
void Demo_Set_OutFHD_Scale(void)
{
#if defined(__USE_SENS_PANNING_MODE__)
	SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p30) ;	// output sync 1080p60	

	SetFC_MainFCsrcPosiX(0); 
	SetFC_MainFCsrcPosiY(0); 
	SetFC_MainFCsrcSizeH(GetIN_MainSrcSizeH());	  // Set FC Src size
	SetFC_MainFCsrcSizeV(GetIN_MainSrcSizeV());	  // Set FC Src size	

	SetFC_MainFCdstPosiX(0);	
	SetFC_MainFCdstPosiY(0);
	SetFC_MainFCdstSizeH(GetOUT_MainSizeHA());	  // Set FC H window size 1920
	SetFC_MainFCdstSizeV(GetOUT_MainSizeVA());	  // Set FC V window size 1080	
#else
	SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p60) ;	// output sync 1080p60	

	SetFC_MainFCsrcPosiX(0); 
	SetFC_MainFCsrcPosiY(0); 
	SetFC_MainFCsrcSizeH(GetIN_MainSrcSizeH());	  // Set FC Src size
	SetFC_MainFCsrcSizeV(GetIN_MainSrcSizeV());	  // Set FC Src size
	
	SetFC_MainFCdstPosiX(0);	
	SetFC_MainFCdstPosiY(0);
	SetFC_MainFCdstSizeH(GetOUT_MainSizeHA());	  // Set FC H window size 1920
	SetFC_MainFCdstSizeV(GetOUT_MainSizeVA());	  // Set FC V window size 1080

	fMainCrop = FALSE ;
	fCropMode = FALSE ;
#endif


}
//-----------------------------------------------------------------------------------------
void Demo_Set_OutUHD(void)
{
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)
	SetOUT_MainFrmt(MISP_VIDOUT_3840x2160p30) ;	// output sync 2160p

	SetFC_MainFCsrcPosiX(0); 
	SetFC_MainFCsrcPosiY(0); 
	SetFC_MainFCsrcSizeH(GetIN_MainSrcSizeH());	  // Set FC Src size 3840
	SetFC_MainFCsrcSizeV(GetIN_MainSrcSizeV());	  // Set FC Src size 2160
	
	SetFC_MainFCdstPosiX(0);	
	SetFC_MainFCdstPosiY(0);
	SetFC_MainFCdstSizeH(GetOUT_MainSizeHA());	  // Set FC H window size 3840
	SetFC_MainFCdstSizeV(GetOUT_MainSizeVA());	  // Set FC V window size 2160

	fMainCrop = FALSE ;
	fCropMode = FALSE ;
#endif
}
//-----------------------------------------------------------------------------------------
#endif	// MDIN-i5x0 (i510, i540, i550)


void DEMO_SetSensorWrite(BOOL OnOff)	// for test
{
	// target bitwidth
	const BYTE sw_bit_mode=MISP_DDRMAP_12b;

	// check WDR state
	if(GetWDR_ProcesMode()) {
		UARTprintf("### Sensor Write Error!! Turn off WDR\r\n");
		return;
	}
	if(OnOff)  UARTprintf("### Sensor Write on\r\n");
	else       UARTprintf("### Sensor Write off\r\n");

	// static WORD blc_trgt=0;
	// if(GetSS_mw_wdr_proc_mode()==MISP_WDR_MEM_WIRTE_OFF_MODE) {
		// blc_trgt=GetSS_man_black_level_target();
	// }

	// set BLC, WB gain
	if(OnOff) {  // turn off
		// SetSS_man_black_level_target(0);  // 0x4A9
		// WBCTRL_SetAWBCtrlPath(0,0,0,0,0);
		SetDDRMAP_WDR(0, 0, sw_bit_mode, MISP_DDRMAP_8b); // 0 buffer 0 frame sww 12b		
	}
	// else {  // restore
		// SetSS_man_black_level_target(blc_trgt);
		// WBCTRL_SetAWBCtrlPath(0,1,1,1,1);
	// }

	// set memory map
	

	// set sensor write mode
	if(OnOff)  SetSS_wdr_write_mode(MISP_WDR_MEM_WIRTE_HTM_MODE, sw_bit_mode, 0x01);
	else       SetSS_wdr_write_mode(MISP_WDR_MEM_WIRTE_OFF_MODE, sw_bit_mode, 0x00);
}

void DEMO_SetSensorWstop(BOOL OnOff)	// for test
{
	// check SSW state
	if(GetSS_mw_wdr_proc_mode()!=MISP_WDR_MEM_WIRTE_HTM_MODE) {
		UARTprintf("### error!! check ssw mode : %d\r\n", GetSS_mw_wdr_proc_mode());
		return;
	}

	// freeze
	static WORD proc_mode=0;
	if(OnOff) {
		UARTprintf("### Sensor freeze on\r\n");
		MISP_RegRead(MISP_LOCAL_ID, 0x580, &proc_mode);  // read [7:6]
		proc_mode = (proc_mode>>6)&0x0003;
		MISP_RegField(MISP_LOCAL_ID, 0x580, 6, 2, 0);  //ss_mw_wdr_proc_mode (off)
	}
	else {
		UARTprintf("### Sensor freeze off\r\n");
		MISP_RegField(MISP_LOCAL_ID, 0x580, 6, 2, proc_mode);
	}

	// print offset
	WORD map5_start_row;
	WORD map5_row_count;
	MISP_RegRead(MISP_LOCAL_ID, 0x1B6, &map5_start_row);
	MISP_RegRead(MISP_LOCAL_ID, 0x1C2, &map5_row_count);
	UARTprintf("### map5_start_row : %d\r\n", map5_start_row);
	UARTprintf("### map5_row_count : %d\r\n", map5_row_count);

	// print H-size
	WORD rVal;
	WORD col_per_line;
	WORD sw_bit_mode;
	MISP_RegRead(MISP_LOCAL_ID, 0x1C8, &rVal);
	col_per_line = (rVal>>0 )&0x03FF;  // read [9:0]
	sw_bit_mode  = (rVal>>10)&0x0003;  // read [11:10]

	BYTE bit_count=0;
	switch(sw_bit_mode) {
		case MISP_DDRMAP_8b:   bit_count=8;   break;
		case MISP_DDRMAP_9b:   bit_count=9;   break;
		case MISP_DDRMAP_10b:  bit_count=10;  break;
		case MISP_DDRMAP_12b:  bit_count=12;  break;
	}

	WORD pxl_per_pack=64/bit_count;
	UARTprintf("### write H-size : %d (%dbit mode)\r\n", col_per_line*pxl_per_pack, bit_count);
	UARTprintf("### write V-size : %d\r\n", GetSS_image_active_size_v()+2*GetSS_image_boundary_size_v());

	// print bayer order (600h[1:0])
	UARTprintf("### bayer order : ");
	MISP_RegRead(MISP_LOCAL_ID, 0x600, &rVal);
	switch(rVal&0x0003) {
		case 0:  UARTprintf("BGGR\r\n");  break;
		case 1:  UARTprintf("GBRG\r\n");  break;
		case 2:  UARTprintf("RGGB\r\n");  break;
		case 3:  UARTprintf("GRBG\r\n");  break;
		default: UARTprintf("????\r\n");  break;
	}
}

//-----------------------------------------------------------------------------------------
void DEMO_fbcap_full(void)
{
	DWORD	i, e_addr_y, e_addr_c;
	WORD	temp, ddr_size_h, in_size_h, in_size_v;
	BYTE	rData[8];
	
	// load capture size & address info.
	MISP_RegRead(MISP_LOCAL_ID, 0x1C4, &temp);	// map1 col count per line
	ddr_size_h = (temp&0x3FF) * 8;
	in_size_h = GetSS_image_active_size_h();
	in_size_v = GetSS_image_active_size_v();
	
	e_addr_y = (DWORD)GetDDRMAP_IPC_Y_start_row() << 12;	// map1 start row address
	e_addr_c = (DWORD)GetDDRMAP_IPC_C_start_row() << 12;	// map1t start row address
	
//	UARTprintf("Y_main, row(%d), addr[%07x ~ %07x], %d bytes \r\n", e_addr_y>>12, e_addr_y, e_addr_y+(ddr_size_h*in_size_v-1), in_size_h*in_size_v);
//	UARTprintf("C_main, row(%d), addr[%07x ~ %07x], %d bytes \r\n\n", e_addr_c>>12, e_addr_c, e_addr_c+(ddr_size_h*in_size_v-1), in_size_h*in_size_v);
	
	MISP_RegField(MISP_LOCAL_ID, 0x040, 0, 2, 1); // enable 'main_wr_stop'(0:disable, 1:enable)
	
	
	// capture 'y'
//	UARTprintf("[Y]\r\n-------------------------------\r\n");
	for(i=0;i<ddr_size_h*in_size_v;i=i+8)
	{
		if((i%ddr_size_h < in_size_h) && (i/ddr_size_h < in_size_v))	{	// skip read guard mem area
				MISP_MultiRead(MISP_SDRAM_ID, e_addr_y+i, rData, 8);
				// P7P6P5P4   P3P2P1P0   P15P14P13P12   P11P10P9P8 ,,
				UARTprintf("%02x%02x%02x%02x%02x%02x%02x%02x\r\n",	rData[0], rData[1], rData[2], rData[3], rData[4], rData[5], rData[6], rData[7]);
		}
		
	}
//	UARTprintf("-------------------------------\r\n");
	
	
	// capture 'c'
//	UARTprintf("[CbCr]\r\n-------------------------------\r\n");
	for(i=0;i<ddr_size_h*in_size_v;i=i+8)
	{
		if((i%ddr_size_h < in_size_h) && (i/ddr_size_h < in_size_v))	{	// skip read guard mem area
			MISP_MultiRead(MISP_SDRAM_ID, e_addr_c+i, rData, 8);
			UARTprintf("%02x%02x%02x%02x%02x%02x%02x%02x\r\n",	rData[0], rData[1], rData[2], rData[3], rData[4], rData[5], rData[6], rData[7]);
		}
		
	}
//	UARTprintf("-------------------------------\r\n");
	
	
	MISP_RegField(MISP_LOCAL_ID, 0x040, 0, 2, 0); // disable 'main_wr_stop'(0:disable, 1:enable)
}

//-----------------------------------------------------------------------------------------
void DEMO_fbcap_area(WORD lx, WORD ly, WORD w, WORD h)
{
	DWORD	i, e_addr_y, e_addr_c, e_addr_rd, cap_size, cnt_rd=0, cnt_prt=0;
	WORD	rVal, cnt_col, rd_x, rd_y, cnt_push=0, cnt_pop=0;
	BYTE	rd_buff[8], raw_buff[16], bytes, idx, j;
	
	MISP_RegRead(MISP_LOCAL_ID, 0x1C4, &rVal);	cnt_col = (rVal&0x3FF) * 8;		// map1 col count per line
	e_addr_y = (DWORD)GetDDRMAP_IPC_Y_start_row() << 12;	// map1 start row address
	e_addr_c = (DWORD)GetDDRMAP_IPC_C_start_row() << 12;	// map1t start row address
	
//	UARTprintf("Y_main, row(%d), addr[%07x ~ %07x], %d bytes \r\n", e_addr_y>>12, e_addr_y, e_addr_y+(cnt_col*h-1), w*h);
//	UARTprintf("C_main, row(%d), addr[%07x ~ %07x], %d bytes \r\n\n", e_addr_c>>12, e_addr_c, e_addr_c+(cnt_col*h-1), w*h);
	
	MISP_RegField(MISP_LOCAL_ID, 0x040, 0, 2, 1); // enable 'main_wr_stop'(0:disable, 1:enable)
	
	// init value
	rd_x = lx;				rd_y = ly;
	cap_size = w * h * 2;	// Y + C
	e_addr_rd = e_addr_y;
	
	// capture func.
//	UARTprintf("[Y]\r\n-------------------------------\r\n");
	for(i=0;i<cap_size/8;i++)	{
		
		while(cnt_rd < (cnt_prt + 8))	{
			// calc ddr address
			if(rd_x == lx)	{	// every new line
				if(cnt_rd < cap_size/2)	{
					e_addr_rd = e_addr_y + ly * cnt_col + lx;		// x,y start offset
					e_addr_rd += (rd_y - ly) * cnt_col;				// calc y offset
				}
				else	{
					e_addr_rd = e_addr_c + ly * cnt_col + lx;		// x,y start offset
					e_addr_rd += (rd_y - h - ly) * cnt_col;			// calc y offset
				}
				
				e_addr_rd -= (lx % 8);		// read 8bytes from prev. -- new line x(not 8x)
			}
			else	e_addr_rd += 8;
			
			// read ddr frame buffer
			if((rd_x % 8 == 0))	{
				if(rd_x + 8 < lx + w)	{			// normal
					bytes = 8;							idx = 0;
					rd_x += 8;
				}
				else if(rd_x + 8 == lx + w)	{	// last bytes(8x)
					bytes = 8;							idx = 0;
					rd_x = lx;	rd_y++;
				}
				else	{											// last bytes(not 8x)
					bytes = (lx + w) % 8;	idx = 8 - bytes;
					rd_x = lx;	rd_y++;
				}
			}														// new line x(not 8x)
			else	{
				bytes = 8 - (rd_x % 8);			idx = 0;				
				rd_x += 8 - (rd_x % 8);
			}
			
			// [0] [1] [2] [3]    [4] [5] [6] [7] ,, -- read buff
			// P7  P6  P5  P4     P3  P2  P1  P0     P15  P14  P13  P12 ,,
			MISP_MultiRead(MISP_SDRAM_ID, e_addr_rd, &rd_buff[0], 8);	// ddr read
			for(j=0;j<bytes;j++)	{
				raw_buff[cnt_push] = rd_buff[bytes-1-j+idx];		// move(read buffer -> raw buffer, P0,P1,P2,,,)
				if(cnt_push >= 15)	cnt_push=0;		else	cnt_push++;
			}
			
			cnt_rd += bytes;
		};
		
		
		// print data [P7 P6 P5 P4 P3 P2 P1 P0,,]
		UARTprintf("%02x", raw_buff[cnt_pop+7 + ((cnt_pop>8)?  (-16):  0)]);
		UARTprintf("%02x", raw_buff[cnt_pop+6 + ((cnt_pop>9)?  (-16):  0)]);
		UARTprintf("%02x", raw_buff[cnt_pop+5 + ((cnt_pop>10)?  (-16):  0)]);
		UARTprintf("%02x", raw_buff[cnt_pop+4 + ((cnt_pop>11)?  (-16):  0)]);
		UARTprintf("%02x", raw_buff[cnt_pop+3 + ((cnt_pop>12)?  (-16):  0)]);
		UARTprintf("%02x", raw_buff[cnt_pop+2 + ((cnt_pop>13)?  (-16):  0)]);
		UARTprintf("%02x", raw_buff[cnt_pop+1 + ((cnt_pop>14)?  (-16):  0)]);
		UARTprintf("%02x", raw_buff[cnt_pop+0 + ((cnt_pop>15)?  (-16):  0)]);
		UARTprintf("\r\n");
		
		if(cnt_pop > 8)		cnt_pop -= 8;
		else				cnt_pop += 8;
		
		cnt_prt += 8;
		
//		if(cnt_prt % cap_size == cap_size/2)	{
//			UARTprintf("-------------------------------\r\n");
//			UARTprintf("[CbCr]\r\n-------------------------------\r\n");
//		}
	}

//	UARTprintf("-------------------------------\r\n");
	
	
	MISP_RegField(MISP_LOCAL_ID, 0x040, 0, 2, 0); // disable 'main_wr_stop'(0:disable, 1:enable)
}

//-----------------------------------------------------------------------------------------
static void DEMO_CnvByr64bit(PBYTE pBuff, PBYTE pByr64b, BYTE bit_res, BYTE pix_cnt, BYTE bit_cut)
{
// [0] [1] [2] [3] [4] [5] [6] [7]   [8] [9],,, -- ddr mem
// x|_P4_| |_P3_|_P2_| |_P1_|_P0_|   x|_P9_|,,, -- bayer 12bit, x : dummy bit

	DWORD	byr_hi32b, byr_lo32b;
	BYTE	i;
	
	byr_hi32b = MAKEDWORD(MAKEWORD(pByr64b[0],pByr64b[1]),MAKEWORD(pByr64b[2],pByr64b[3])) << bit_cut;
	byr_lo32b = MAKEDWORD(MAKEWORD(pByr64b[4],pByr64b[5]),MAKEWORD(pByr64b[6],pByr64b[7]));
	
	// x bit -> 8bit
	for(i=0;i<pix_cnt;i++)	{
		if(bit_cut+bit_res <= 32)	{								// hi
			pBuff[pix_cnt-1-i] = HIBYTE(HIWORD(byr_hi32b));
			byr_hi32b = byr_hi32b << bit_res;
		}
		else if(bit_cut < 32)	{									// hi+lo
			pBuff[pix_cnt-1-i] = HIBYTE(HIWORD(byr_hi32b));
			pBuff[pix_cnt-1-i] |= HIBYTE(HIWORD(byr_lo32b)) >> (32-bit_cut);
			byr_lo32b = byr_lo32b << (bit_cut+bit_res - 32);
		}
		else	{
			pBuff[pix_cnt-1-i] = HIBYTE(HIWORD(byr_lo32b));
			byr_lo32b = byr_lo32b << bit_res;
		}
		
		bit_cut += bit_res;
	}
}

//-----------------------------------------------------------------------------------------
// read time : 64 x 64 = 4KB --> 7ms
void DEMO_byrcap_area(WORD lx, WORD ly, WORD w, WORD h, PBYTE pRavg, PBYTE pGavg, PBYTE pBavg)
{
	//DWORD cnt=0;
	DWORD	i, j, cap_size, mem_w, e_addr, e_addr_2nd=0, e_addr_1st=0, rsum=0, gsum=0, bsum=0;
	WORD	cnt_row, cnt_col, rd_x, rd_y, c_cnt=0;
	BYTE	rd_buff[32], byr_buff[32], g[16], b[16], r[16], c_old[w];
	BYTE	bit_mode, bit_res, bit_cut, pix_cnt;
	
//	DEMO_SetSensorWstop(ON);
	
#if defined(__USE_AWB_RGB_WDR_MEM__)
	GetDDRMAP_WDRSW_MemCRPF(&bit_mode, &cnt_col, &cnt_row);		// get wdr-sw mem info
	e_addr = (DWORD)GetDDRMAP_WDRSW_start_row() << 12;			// map5(wdr-sw) start row address
#else
	GetDDRMAP_NR_MemCRPF(&bit_mode, &cnt_col, &cnt_row);		// get nr mem info
	e_addr = (DWORD)GetDDRMAP_NR_start_row() << 12;				// map3(nr) start row address
#endif
	
	switch(bit_mode)	{			// mem bit ratio, ex)ssw[12b] vs ipc[8b] = 5 vs 8 [pixel/8B] = 1.6
		case  MISP_DDRMAP_8b:	bit_res = 8;	pix_cnt = 8;	bit_cut = 0;	break;
		case  MISP_DDRMAP_9b:	bit_res = 9;	pix_cnt = 7;	bit_cut = 1;	break;
		case  MISP_DDRMAP_10b:	bit_res = 10;	pix_cnt = 6;	bit_cut = 4;	break;
		case  MISP_DDRMAP_12b:
		default:				bit_res = 12;	pix_cnt = 5;	bit_cut = 4;	break;
	}
	
	lx -= (lx % pix_cnt*2);			// *2 -> for pixel order
	w -= (w % (pix_cnt*2));			// *2 -> for read 16 bytes
	ly -= (ly % 2);
	h -= (h % 2);
	
	rd_x = lx;
	rd_y = ly;
	cap_size = w * h;
	mem_w = cnt_col * 8;

	for(i=0;i<cap_size/(pix_cnt*2);i++)	{
		// calc ddr address
		if(rd_x == lx)	{										// every new line
			e_addr_1st = e_addr + (DWORD)ly * mem_w + (DWORD)lx/pix_cnt*8;	// x,y start offset
			e_addr_1st += (DWORD)(rd_y - ly) * mem_w;			// 1st line : y
			e_addr_2nd = e_addr_1st + mem_w;					// 2nd line : y+1
			c_cnt = 0;
		}
		else	{
			e_addr_1st += 16;
			e_addr_2nd += 16;
		}
		
		// ddr read
		if(rd_y % 2 == 0)	{
			MISP_MultiRead(MISP_SDRAM_ID, e_addr_1st, &rd_buff[0], 16);			// 1st line
			MISP_MultiRead(MISP_SDRAM_ID, e_addr_2nd, &rd_buff[16], 16);		// 2nd line
		}
		else	{
			MISP_MultiRead(MISP_SDRAM_ID, e_addr_1st, &rd_buff[0], 16);			// 1st line
		}
		
		// convert bayer x bit to 8bit
		DEMO_CnvByr64bit(&byr_buff[pix_cnt*0], &rd_buff[8*0], bit_res, pix_cnt, bit_cut);
		DEMO_CnvByr64bit(&byr_buff[pix_cnt*1], &rd_buff[8*1], bit_res, pix_cnt, bit_cut);
		DEMO_CnvByr64bit(&byr_buff[pix_cnt*2], &rd_buff[8*2], bit_res, pix_cnt, bit_cut);
		DEMO_CnvByr64bit(&byr_buff[pix_cnt*3], &rd_buff[8*3], bit_res, pix_cnt, bit_cut);
		
		// convert bayer raw data to rgb -- order : GBRG
		if(rd_y % 2 == 0)	{
			for(j=0;j<pix_cnt*2;j+=2)	{
				g[j] = g[j+1] = byr_buff[j];					// 0,2,4,6,8
				b[j] = b[j+1] = byr_buff[j+1];					// 1,3,5,7,9
				r[j] = r[j+1] = byr_buff[j+pix_cnt*2];			// 10,12,14,16,18
			}
			memcpy(&c_old[c_cnt], &b[0], pix_cnt*2);		c_cnt += pix_cnt*2;	// color backup
		}
		else	{
			for(j=0;j<pix_cnt*2;j+=2)	{
				r[j] = r[j+1] = byr_buff[j];					// 0,2,4,6,8
				g[j] = g[j+1] = byr_buff[j+1];					// 1,3,5,7,9
			}
			memcpy(&b[0], &c_old[c_cnt], pix_cnt*2);		c_cnt += pix_cnt*2;	// color restore
		}
		
		// calc rgb sum
		for(j=0;j<pix_cnt*2;j++)	{
			rsum += r[j];		gsum += g[j];			bsum += b[j];
			//pBuff[cnt++]=b[j];pBuff[cnt++]=g[j];pBuff[cnt++]=r[j];
		}
		
		rd_x += pix_cnt*2;
		if(rd_x >= lx + w)		{rd_x = lx;		rd_y++;}
	}
	
	*pRavg = rsum / cap_size;
	*pGavg = gsum / cap_size;
	*pBavg = bsum / cap_size;

//	DEMO_SetSensorWstop(OFF);
}

//-----------------------------------------------------------------------------------------
void DEMO_SetCVBSAspectCrop(void)
{
	WORD src_ratio, crop_size, offset_x;
	
	src_ratio = GetIN_AuxSrcSizeH() * 10 / GetIN_AuxSrcSizeV();
	crop_size = GetIN_AuxSrcSizeH() * 75 / 100;
	offset_x = (GetIN_AuxSrcSizeH() - crop_size) / 2;
	
	if(src_ratio < 16)			return;		// exit when video source aspect ratio is 4:3
	
	SetFC_AuxFCsrcSizeH(crop_size);
	SetFC_AuxFCsrcPosiX(offset_x);
	
	MISP_SW_Reset();
}

//-----------------------------------------------------------------------------------------
void DEMO_SetCVBSAspectFull(void)
{
	SetFC_AuxFCsrcSizeH(GetIN_AuxSrcSizeH());
	SetFC_AuxFCsrcPosiX(0);
	
	MISP_SW_Reset();
}
//-----------------------------------------------------------------------------------------
void DEMO_SetExt2IPP(void)
{
#if defined(__USE_MDIN_i550__)&&defined(__USE_MDIN_i5XX_REV__)
	static BOOL ext2ipp = 1 ;
	LONG mode = stAECTRL.ctrl&AECTRL_PROCESS_OFF;
	
	if ( ext2ipp == 1 )
	{ // Ext2IPP mode 
		SetIN_Ext2IPP(1) ;
		SetSS_Ext2IPP(1) ;
		
		SetWBModeSetMenu(WBCTRL_MANUAL_MODE) ; // If AWB is alreay stopped then don't need to execute this line

		WBCTRL_SetAWBCtrlPath(0,0,0,0,0);
		SetBY_gamma_enable(0);
		SetDM_ColorCCEn(0);
		
		SetTONE_RGBGAMMA_enable(0);
		
		ext2ipp = 0 ;
		
		if (mode!=AECTRL_PROCESS_OFF)	mode = AECTRL_PROCESS_OFF; // If AE is alreay stopped then don't need to execute this line

		stAECTRL.ctrl &= (~AECTRL_PROCESS_OFF); // If AE is alreay stopped then don't need to execute this line
		stAECTRL.ctrl |= (mode); // If AE is alreay stopped then don't need to execute this line		
	}
	else
	{	// Normal IPP mode 
		SetIN_Ext2IPP(0) ;		
		SetSS_Ext2IPP(0) ;	
		
		SetWBModeSetMenu(WBCTRL_WBAUTO_MODE) ; // If system is NO SENSOR mode then don't need to execute this line
		
		WBCTRL_SetAWBCtrlPath(0,1,1,1,1);
		SetBY_gamma_enable(1);
		SetDM_ColorCCEn(1);
		
		SetTONE_RGBGAMMA_enable(1);
		
		ext2ipp = 1 ;
		
		if (mode!=AECTRL_PROCESS_RUN)	mode = AECTRL_PROCESS_RUN; // If system is NO SENSOR mode then don't need to execute this line

		stAECTRL.ctrl &= (~AECTRL_PROCESS_OFF); // If system is NO SENSOR mode then don't need to execute this line
		stAECTRL.ctrl |= (mode); // If system is NO SENSOR mode then don't need to execute this line			
	}

#endif
}

//--------------------------------------------------------------------------------------------------------------------------
// Drive Function for Auto-Sync Detection
//--------------------------------------------------------------------------------------------------------------------------
BOOL DEMO_GetSyncDone(void)
{
	WORD rVal = 0, count = 100;

	while (count&&(rVal==0)) {
		MISP_RegRead(MISP_LOCAL_ID, 0x0d0, &rVal);
		rVal &= 0x8000; count--;	Delay_mSec(10);	// delay 10ms
	}
	//if (count==0) mdinERR = 5;
	return (count)?  0:  1;		// 0:no error,  1:timeout error
}

//--------------------------------------------------------------------------------------------------------------------------
void DEMO_GetSyncInfo(void)
{
	BYTE	port = 1;	// the selection of input port

	//BYTE	err;		// auto sync error (MDIN_AUTOSYNC_ERROR_t)
	WORD	sync;		// the status of change and lost of input video sync, scan type
	WORD	Hfrq;		// the horizontal frequency of input video sync
	WORD	Htot;		// the total pixels of input video sync
	WORD	Vfrq;		// the vertical frequency of input video sync
	WORD	Vtot;		// the total lines of input video sync
	WORD	Hact;		// the active pixels of input video data
	WORD	Vact;		// the active lines of input video data
	
	WORD	rBuff[6];
	WORD	hclk = 104;	// host clock frequency [Mhz]

	// check input test pattern
	MISP_RegRead(MISP_LOCAL_ID, 0x042, rBuff);
	if (rBuff[0]&0x0080) UARTprintf("[AUTO-SYNC] error, AUTO_SYNC_SRC_PTRN \r\n");
	if (rBuff[0]&0x0080) return;

	// enable auto-detection
	MISP_RegWrite(MISP_LOCAL_ID, 0x0d0, 0x1001);
	MISP_RegWrite(MISP_LOCAL_ID, 0x0d9, 0x0001);

	if (DEMO_GetSyncDone()) {UARTprintf("TIMEOUT \r\n");	return;}	// check done flag

	// disable auto-detection
	MISP_RegWrite(MISP_LOCAL_ID, 0x0d0, 0x1000);
	MISP_RegWrite(MISP_LOCAL_ID, 0x0d9, 0x0000);

	// get sync_chg and sync_lost
	MISP_RegRead(MISP_LOCAL_ID, 0xe0, rBuff);
	sync = (port)? HIBYTE(rBuff[0])&3 : LOBYTE(rBuff[0])&3;

	MISP_MultiRead(MISP_LOCAL_ID, 0xe4+((port)?  0x10:  0), (PBYTE)rBuff, 12);
	if (rBuff[1]&0x0001) sync |= 4;						// get scan-type, 4:MDIN_AUTOSYNC_INTR
	Hfrq = ((DWORD)hclk*100000/(rBuff[0]&0x7fff));		// get H-freq
	Htot = (rBuff[2]&0x7fff)+(rBuff[3]&0x0fff);			// get H-total
	Vtot = (rBuff[4]&0x0fff)+(rBuff[5]&0x0fff);			// get V-total
	Vfrq = ((DWORD)Hfrq)*10/Vtot;			// get V-freq

	if ((rBuff[0]&0x8000)==0) UARTprintf("[AUTO-SYNC] error, AUTO_SYNC_BAD_HFRQ \r\n");
	if ((rBuff[1]&0x8000)==0) UARTprintf("[AUTO-SYNC] error, AUTO_SYNC_BAD_SCAN \r\n");
	if ((rBuff[2]&0x8000)==0) UARTprintf("[AUTO-SYNC] error, AUTO_SYNC_BAD_HTOT \r\n");
	if ((rBuff[3]&0x8000)==0) UARTprintf("[AUTO-SYNC] error, AUTO_SYNC_BAD_HTOT \r\n");
	if ((rBuff[4]&0x8000)==0) UARTprintf("[AUTO-SYNC] error, AUTO_SYNC_BAD_VTOT \r\n");
	if ((rBuff[5]&0x8000)==0) UARTprintf("[AUTO-SYNC] error, AUTO_SYNC_BAD_VTOT \r\n");

	//UARTprintf("[AUTO-SYNC] error is 0x%02X\n", err);
	UARTprintf("[AUTO-SYNC] sync status is 0x%02X\n", sync);
	UARTprintf("[AUTO-SYNC] H-freq is %d/100 Hz\n", Hfrq);
	UARTprintf("[AUTO-SYNC] H-total is %d\n", Htot);
	UARTprintf("[AUTO-SYNC] V-freq is %d Hz\n", Vfrq);
	UARTprintf("[AUTO-SYNC] V-total is %d\n", Vtot);

	if (port) return;	// if port-1, active info is not exist.

	MISP_MultiRead(MISP_LOCAL_ID, 0xee, (PBYTE)rBuff, 8);
	Hact = (rBuff[1]&0x0fff)-(rBuff[0]&0x0fff);			// get H-active
	Vact = (rBuff[3]&0x0fff)-(rBuff[2]&0x0fff);			// get V-active

	if ((rBuff[0]&0x8000)==0) UARTprintf("[AUTO-SYNC] error, AUTO_SYNC_BAD_HACT \r\n");
	if ((rBuff[1]&0x8000)==0) UARTprintf("[AUTO-SYNC] error, AUTO_SYNC_BAD_HACT \r\n");
	if ((rBuff[2]&0x8000)==0) UARTprintf("[AUTO-SYNC] error, AUTO_SYNC_BAD_VACT \r\n");
	if ((rBuff[3]&0x8000)==0) UARTprintf("[AUTO-SYNC] error, AUTO_SYNC_BAD_VACT \r\n");

	UARTprintf("[AUTO-SYNC] H-active is %d\n", Hact);
	UARTprintf("[AUTO-SYNC] V-active is %d\n", Vact);
}

//--------------------------------------------------------------------------------------------------------------------------
void DEMO_GetAuxSyncInfo(void)							
{							
	BYTE	port =0;	// the selection of input port				
							
	//BYTE	err;		// auto sync error (MDIN_AUTOSYNC_ERROR_t)			
	WORD	sync;		// the status of change and lost of input video sync, scan type			
	WORD	Hfrq;		// the horizontal frequency of input video sync			
	WORD	Htot;		// the total pixels of input video sync			
	WORD	Vfrq;		// the vertical frequency of input video sync			
	WORD	Vtot;		// the total lines of input video sync			
	WORD	Hact;		// the active pixels of input video data			
	WORD	Vact;		// the active lines of input video data			
							
	WORD	rBuff[6];					
	WORD	hclk = 104;	// host clock frequency [Mhz]				
							
	// check input test pattern						
	MISP_RegRead(MISP_LOCAL_ID, 0x042, rBuff);						
	if (rBuff[0]&0x0040) UARTprintf("[AUTO-AUX SYNC] error, AUTO_SYNC_SRC_PTRN \r\n");						
	if (rBuff[0]&0x0040) return;						
							
	// enable auto-detection						
	MISP_RegWrite(MISP_LOCAL_ID, 0x0d0, 0x0001);						
	MISP_RegWrite(MISP_LOCAL_ID, 0x0d9, 0x1001);						
							
	if (DEMO_GetSyncDone()) {UARTprintf("(AUX) TIMEOUT \r\n");	return;}	// check done flag				
							
	// disable auto-detection						
	MISP_RegWrite(MISP_LOCAL_ID, 0x0d0, 0x0000);						
	MISP_RegWrite(MISP_LOCAL_ID, 0x0d9, 0x1000);						
							
	// get sync_chg and sync_lost						
	MISP_RegRead(MISP_LOCAL_ID, 0xe0, rBuff);						
	sync = (port)? HIBYTE(rBuff[0])&3 : LOBYTE(rBuff[0])&3;						
							
	MISP_MultiRead(MISP_LOCAL_ID, 0xe4+((port)?  0x10:  0), (PBYTE)rBuff, 12);						
	if (rBuff[1]&0x0001) sync |= 4;						// get scan-type, 4:MDIN_AUTOSYNC_INTR
	Hfrq = ((DWORD)hclk*100000/(rBuff[0]&0x7fff));		// get H-freq				
	Htot = (rBuff[2]&0x7fff)+(rBuff[3]&0x0fff);			// get H-total			
	Vtot = (rBuff[4]&0x0fff)+(rBuff[5]&0x0fff);			// get V-total			
	Vfrq = ((DWORD)Hfrq)*10/Vtot;			// get V-freq			
							
	if ((rBuff[0]&0x8000)==0) UARTprintf("[AUTO-AUX SYNC] error, AUTO_SYNC_BAD_HFRQ \r\n");						
	if ((rBuff[1]&0x8000)==0) UARTprintf("[AUTO-AUX SYNC] error, AUTO_SYNC_BAD_SCAN \r\n");						
	if ((rBuff[2]&0x8000)==0) UARTprintf("[AUTO-AUX SYNC] error, AUTO_SYNC_BAD_HTOT \r\n");						
	if ((rBuff[3]&0x8000)==0) UARTprintf("[AUTO-AUX SYNC] error, AUTO_SYNC_BAD_HTOT \r\n");						
	if ((rBuff[4]&0x8000)==0) UARTprintf("[AUTO-AUX SYNC] error, AUTO_SYNC_BAD_VTOT \r\n");						
	if ((rBuff[5]&0x8000)==0) UARTprintf("[AUTO-AUX SYNC] error, AUTO_SYNC_BAD_VTOT \r\n");						
							
	//UARTprintf("[AUTO-SYNC] error is 0x%02X\n", err);						
	UARTprintf("[AUTO-AUX SYNC] sync status is 0x%02X\n", sync);						
	UARTprintf("[AUTO-AUX SYNC] H-freq is %d/100 Hz\n", Hfrq);						
	UARTprintf("[AUTO-AUX SYNC] H-total is %d\n", Htot);						
	UARTprintf("[AUTO-AUX SYNC] V-freq is %d Hz\n", Vfrq);						
	UARTprintf("[AUTO-AUX SYNC] V-total is %d\n", Vtot);						
							
	if (port) return;	// if port-1, active info is not exist.					
							
	MISP_MultiRead(MISP_LOCAL_ID, 0xee, (PBYTE)rBuff, 8);						
	Hact = (rBuff[1]&0x0fff)-(rBuff[0]&0x0fff);			// get H-active			
	Vact = (rBuff[3]&0x0fff)-(rBuff[2]&0x0fff);			// get V-active			
							
	if ((rBuff[0]&0x8000)==0) UARTprintf("[AUTO-AUX SYNC] error, AUTO_SYNC_BAD_HACT \r\n");						
	if ((rBuff[1]&0x8000)==0) UARTprintf("[AUTO-AUX SYNC] error, AUTO_SYNC_BAD_HACT \r\n");						
	if ((rBuff[2]&0x8000)==0) UARTprintf("[AUTO-AUX SYNC] error, AUTO_SYNC_BAD_VACT \r\n");						
	if ((rBuff[3]&0x8000)==0) UARTprintf("[AUTO-AUX SYNC] error, AUTO_SYNC_BAD_VACT \r\n");						
							
	UARTprintf("[AUTO-AUX SYNC] H-active is %d\n", Hact);						
	UARTprintf("[AUTO-AUX SYNC] V-active is %d\n", Vact);						
}							

void DEMO_RawDataDumpSetup( BYTE process )
{
	BYTE bit_mode ;
	WORD col_count_per_line ;
	WORD row_count_per_frame ;
	
	WORD image_width ;
	WORD image_hight ;
	
	WORD ipc_fc ;
	
	if ( process == 1 )
	{
		// freeze IPC
		SetFC_StopMainFC(1) ;
		
		// Setup SSW
		DEMO_SetSensorWrite(ON) ;
		
		// NR Off
		SetNR_Mode(MISP_NR_OFF) ;
		UARTprintf("MemDump process START.\r\n");	
		OSD_IrkeyTitleDisplay(ON, "DDRWriting");
	}
	else if ( process == 2 )
	{
		// Hold SSW
		DEMO_SetSensorWrite(OFF) ;		
		
		GetDDRMAP_WDRSW_MemCRPF(&bit_mode,&col_count_per_line,&row_count_per_frame);		
		image_width = (GetSS_image_boundary_size_h()<<1) + GetSS_image_active_size_h()  ; 
		image_hight = (GetSS_image_boundary_size_v()<<1) + GetSS_image_active_size_v()  ; 
			
		UARTprintf("\r\n") ;
		UARTprintf("\r\n") ;
		UARTprintf(":::Batch0 file starts from HERE:::::::::::::::::::::::::::::::::::::::::\r\n") ;	
		UARTprintf(":::Copy & Execute below script :::::::::::::::::::::::::::::::::::::::::\r\n") ;	
		
		UARTprintf("@echo off\r\n") ;
		UARTprintf("pushd %%~dp0\r\n") ;
		UARTprintf("set econ=\"C:\\ADChips\\EISC Studio 3\\econ\"\r\n") ;
		UARTprintf("SET PATH=%%PATH%%;%%econ%%\r\n") ;
		UARTprintf("\r\n") ;		
		
		UARTprintf("set map5_start_row=%d\r\n",GetDDRMAP_WDRRW_end_row()) ;
		UARTprintf("set map5_row_count=%d\r\n\r\n",(row_count_per_frame-1)) ;
		UARTprintf("set base_addr=491520\r\n") ;
		UARTprintf("set /a addr_result = (%%base_addr%%+%%map5_start_row%%+(%%map5_row_count%%*0))*4096\r\n") ;
		UARTprintf("set /a size_result = (%%map5_row_count%%)*4096\r\n") ;
		
		UARTprintf("EConMan.exe -target -sysinit -jtags 5 -fr %%addr_result%% %%size_result%% sram_rslt_ssw.Y -c -exit\r\n") ;
		UARTprintf("\r\n") ;
		
		UARTprintf("set dir=.\r\n") ;		

		UARTprintf("set col_count_per_line=%d\r\n",col_count_per_line) ;		
		UARTprintf("set image_width=%d\r\n",image_width) ;
		UARTprintf("set image_hight=%d\r\n",image_hight) ;	
		
		UARTprintf("set bit_mode=%d\r\n",bit_mode) ;		

		UARTprintf("sram_to_raw.exe %%dir%%\\sram_rslt_ssw.Y sram_rslt_ssw.raw %%bit_mode%% %%col_count_per_line%% %%image_width%% %%image_hight%%\r\n") ;
	
		UARTprintf("goto :EOF\r\n");
		UARTprintf(":EOF\r\n");	
		
		UARTprintf("\r\n:::Batch0 file ends HERE:::::::::::::::::::::::::::::::::::::::::::::::\r\n") ;
		
		// Setup NR		
		SetNR_Mode(MISP_NR_2D3D) ;
		
		// RUN IPC	
		SetFC_StopMainFC(0) ;
		OSD_IrkeyTitleDisplay(ON, "DDRdumpExS");
	}
	else if ( process == 3 )
	{
		// Hold NR
		SetNR_Mode(MISP_NR_OFF) ;

		GetDDRMAP_NR_MemCRPF(&bit_mode,&col_count_per_line,&row_count_per_frame);		
		image_width = (GetSS_image_boundary_size_h()<<1) + GetSS_image_active_size_h()  ; 
		image_hight = (GetSS_image_boundary_size_v()<<1) + GetSS_image_active_size_v()  ; 		
		
		UARTprintf("\r\n") ;
		UARTprintf("\r\n") ;
		UARTprintf(":::Batch1 file starts from HERE:::::::::::::::::::::::::::::::::::::::::\r\n") ;	
		UARTprintf(":::Copy & Execute below script :::::::::::::::::::::::::::::::::::::::::\r\n") ;	
		
		UARTprintf("@echo off\r\n") ;
		UARTprintf("pushd %%~dp0\r\n") ;
		UARTprintf("set econ=\"C:\\ADChips\\EISC Studio 3\\econ\"\r\n") ;
		UARTprintf("SET PATH=%%PATH%%;%%econ%%\r\n") ;
		UARTprintf("\r\n") ;		
		
		UARTprintf("set map3_start_row=%d\r\n",GetDDRMAP_AUX_end_row()) ;
		UARTprintf("set map3_row_count=%d\r\n\r\n",(row_count_per_frame-1)) ;
		UARTprintf("set base_addr=491520\r\n") ;
		UARTprintf("set /a addr_result = (%%base_addr%%+%%map3_start_row%%+(%%map3_row_count%%*0))*4096\r\n") ;
		UARTprintf("set /a size_result = (%%map3_row_count%%)*4096\r\n") ;
		
		UARTprintf("EConMan.exe -target -sysinit -jtags 5 -fr %%addr_result%% %%size_result%% sram_rslt_nr.Y -c -exit\r\n") ;
		UARTprintf("\r\n") ;
		
		UARTprintf("set dir=.\r\n") ;		

		UARTprintf("set col_count_per_line=%d\r\n",col_count_per_line) ;		
		UARTprintf("set image_width=%d\r\n",image_width) ;
		UARTprintf("set image_hight=%d\r\n",image_hight) ;	
		
		UARTprintf("set bit_mode=%d\r\n",bit_mode) ;		

		UARTprintf("sram_to_raw.exe %%dir%%\\sram_rslt_nr.Y sram_rslt_nr.raw %%bit_mode%% %%col_count_per_line%% %%image_width%% %%image_hight%%\r\n") ;
	
		UARTprintf("goto :EOF\r\n");
		UARTprintf(":EOF\r\n");	
		
		UARTprintf("\r\n:::Batch1 file ends HERE:::::::::::::::::::::::::::::::::::::::::::::::\r\n") ;	
		OSD_IrkeyTitleDisplay(ON, "DDRdumpExN");
	}
	else if ( process == 4 )
	{
		// Setup NR		
		SetNR_Mode(MISP_NR_2D3D) ;
		// Hold IPC	
		SetFC_StopMainFC(1) ;

		GetDDRMAP_IPC_MemCRPF(&bit_mode,&col_count_per_line,&row_count_per_frame);		
		image_width = GetSS_image_active_size_h()  ; 
		image_hight = GetSS_image_active_size_v()  ;			
		ipc_fc = GetDDRMAP_IPC_frame_count() ;
		
		UARTprintf("\r\n") ;
		UARTprintf("\r\n") ;
		UARTprintf(":::Batch2 file starts from HERE:::::::::::::::::::::::::::::::::::::::::\r\n") ;	
		UARTprintf(":::Copy & Execute below script :::::::::::::::::::::::::::::::::::::::::\r\n") ;	
		
		UARTprintf("@echo off\r\n") ;
		UARTprintf("pushd %%~dp0\r\n") ;
		UARTprintf("set econ=\"C:\\ADChips\\EISC Studio 3\\econ\"\r\n") ;
		UARTprintf("SET PATH=%%PATH%%;%%econ%%\r\n") ;
		UARTprintf("\r\n") ;		
		
		UARTprintf("set map1_start_row=%d\r\n",0) ;
		UARTprintf("set map1_row_count=%d\r\n\r\n",(row_count_per_frame-1)) ;
		UARTprintf("set base_addr=491520\r\n") ;
		UARTprintf("set /a addr_result = (%%base_addr%%+%%map1_start_row%%+(%%map1_row_count%%*0))*4096\r\n") ;
		UARTprintf("set /a size_result = (%%map1_row_count%%)*4096\r\n") ;
		
		UARTprintf("EConMan.exe -target -sysinit -jtags 5 -fr %%addr_result%% %%size_result%% sram_rslt_ipc_y.Y -c -exit\r\n") ;
		UARTprintf("\r\n") ;
		
		UARTprintf("set dir=.\r\n") ;		

		UARTprintf("set col_count_per_line=%d\r\n",col_count_per_line) ;		
		UARTprintf("set image_width=%d\r\n",image_width) ;
		UARTprintf("set image_hight=%d\r\n",image_hight) ;	
		
		UARTprintf("set bit_mode=%d\r\n",bit_mode) ;		

		UARTprintf("sram_to_raw.exe %%dir%%\\sram_rslt_ipc_y.Y sram_rslt_ipc_y.raw %%bit_mode%% %%col_count_per_line%% %%image_width%% %%image_hight%%\r\n") ;
	
		UARTprintf("set map1t_start_row=%d\r\n",(row_count_per_frame*ipc_fc)) ;
		UARTprintf("set map1t_row_count=%d\r\n\r\n",(row_count_per_frame-1)) ;		
		UARTprintf("set /a addr_result = (%%base_addr%%+%%map1t_start_row%%+(%%map1t_row_count%%*0))*4096\r\n") ;
		UARTprintf("set /a size_result = (%%map1t_row_count%%)*4096\r\n") ;	
		
		UARTprintf("EConMan.exe -target -sysinit -jtags 5 -fr %%addr_result%% %%size_result%% sram_rslt_ipc_c.Y -c -exit\r\n") ;
		UARTprintf("\r\n") ;		
		
		UARTprintf("sram_to_raw.exe %%dir%%\\sram_rslt_ipc_c.Y sram_rslt_ipc_c.raw %%bit_mode%% %%col_count_per_line%% %%image_width%% %%image_hight%%\r\n") ;
		
		UARTprintf("goto :EOF\r\n");
		UARTprintf(":EOF\r\n");	
		
		UARTprintf("\r\n:::Batch2 file ends HERE:::::::::::::::::::::::::::::::::::::::::::::::\r\n") ;	
		OSD_IrkeyTitleDisplay(ON, "DDRdumpExI");
	}
	else
	{
		// RUN IPC
		SetFC_StopMainFC(0) ;
		// Setup NR	
		SetNR_Mode(MISP_NR_2D3D) ;		
		
		UARTprintf("MemDump process DONE.\r\n");	
		OSD_IrkeyTitleDisplay(OFF, "          ");
	}
}

void DEMO_RawDataDumpProcess(void)
{
	static BYTE process = 0 ;
	
	process++ ;
	DEMO_RawDataDumpSetup(process);
		
	if ( process > 4 ) process = 0 ;
}

//--------------------------------------------------------------------------------------
void DEMO_SetDebugMode(WORD val)
{
	// add function here..
	UARTprintf("DEMO_SetDebugMode[%d] \r\n", val);
}

//--------------------------------------------------------------------------------------
void DEMO_SetAgingTest(WORD val)
{
	// add function here..
	UARTprintf("DEMO_SetAgingTest[%d] \r\n", val);
}

#if	defined(__MDIN_i510_REF__)				// i510 REF board
//--------------------------------------------------------------------------------------
void IOExp_ProcessHandler(void)
{
	if(!fIOExpIRQ)		return;
	fIOExpIRQ=0;
	
#if defined(__USE_IRKEY_SWITCH__)
	if(GET_SWITCH()==LOW)			IrkeyEnableToggle();
#endif
}
#endif

//--------------------------------------------------------------------------------------
void DEMO_SetHdmiTxMode(WORD val)	// 0:RGB444(bt709), 1:RGB444(bt601), 2:YC422(for M400_quad) 
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveHdmiTxMode(val);
#endif
	
#if defined(__USE_IT66121_TX__)
	IT66121_SetCSC(val);
#endif

#if defined(__USE_IT6615_TX__)
	ITE6615_SetOutColorSpace(val);
#endif

#if defined(__USE_LT8618EX_TX__)
	LT8618EX_SetCSC(val);
#endif

#if defined(__USE_LT8618SX_TX__)
	LT8618SX_SetCSC(val);
#endif

#if defined(__USE_SIL9136_TX__)
	SIL9136_SetCSC(val);
#endif

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;			buff[1] = val;
	EEPROM_MultiWrite(EEPROM_HDMITX_MODE, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------
BYTE DEMO_GetHdmiTxMode(void)	// 0:RGB444(bt709), 1:RGB444(bt601), 2:YC422(for M400_quad) 
{
#if defined(__USE_IT66121_TX__)
	return IT66121_GetCSC();
#endif

#if defined(__USE_IT6615_TX__)
	return ITE6615_GetOutColorSpace();
#endif

#if defined(__USE_LT8618EX_TX__)
	return LT8618EX_GetCSC();
#endif

#if defined(__USE_LT8618SX_TX__)
	return LT8618SX_GetCSC();
#endif

#if defined(__USE_SIL9136_TX__)
	return SIL9136_GetCSC();
#endif
	
	return 0;
}

#if defined(__USE_OSD_TITLE__)
//--------------------------------------------------------------------------------------
void DEMO_SetOSD_Title(BOOL OnOff)
{
#if !defined(__USE_4K60_INOUT__)
	BYTE vtype=0, vfrmt=0;
	
// v.out type
#if defined(__USE_IT66121_TX__) || defined(__USE_IT6615_TX__) || defined(__USE_LT8618EX_TX__) || defined(__USE_LT8618SX_TX__) || defined(__USE_SIL9136_TX__)
		vtype=1;	// hdmi
#endif
#if defined(__USE_EN332T__)
		vtype=2;	// ex-sdi
#endif
#if defined(__USE_TVI_TX__)
		vtype=3;	// tvi
#endif
	
// v.out frmt
	switch(GetOUT_MainFrmt())	{
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)
		case  MISP_VIDOUT_3840x2160p15:
		case  MISP_VIDOUT_3840x2160p12:		vfrmt=1;	break;	// 4K15p
		case  MISP_VIDOUT_3840x2160p30:
		case  MISP_VIDOUT_3840x2160p25:		vfrmt=2;	break;	// 4K30p
#endif
		case  MISP_VIDOUT_1920x1080p60:
		case  MISP_VIDOUT_1920x1080p50:		vfrmt=3;	break;	// FHD60p
		case  MISP_VIDOUT_1920x1080p30:
		case  MISP_VIDOUT_1920x1080p25:
		case  MISP_VIDOUT_1920x1080p24:		vfrmt=4;	break;	// FHD30p
	}
	
	OSD_SetTitleBMP(OnOff, MAKEBYTE(vtype, vfrmt));
#endif
}
#endif

#if defined(__USE_2MOTOR_LENS__)
//--------------------------------------------------------------------------------------
void SetTwoMotorAfMode(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	LONG mode = AFCTRL_AFAUTO_MODE;
	
	switch(nID)	{
		case 0:		mode = AFCTRL_AFAUTO_MODE;	break;
		case 1:		mode = AFCTRL_MANUAL_MODE;	break;
	}

	stAFCTRL.ctrl &= (~(AFCTRL_AFAUTO_MASK));
	stAFCTRL.ctrl |= (mode);

//	UARTprintf("afmode : %x\r\n",(stAFCTRL.ctrl & AFCTRL_AFAUTO_MASK));
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_TWOMOTOR_AFMODE, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------
void SetTwoMotorScanMode(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif

	LONG mode = AFCTRL_AFSCAN_HALF;

	switch(nID)	{
		case 0:		mode = AFCTRL_AFSCAN_HALF;	break;
		case 1:		mode = AFCTRL_AFSCAN_FULL;	break;
	}

	stAFCTRL.ctrl &= (~(AFCTRL_AFSCAN_FULL));
	stAFCTRL.ctrl |= (mode);

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_TWOMOTOR_SCAN, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------
void SetTwoMotorOnePush(void)
{
	LENS_FocusResearch();
}

//--------------------------------------------------------------------------------------
void SetTwoMotorSyncTDN(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	// add function here
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_TWOMOTOR_SYNCTDN, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------
void SetTwoMotorInitial(void)
{
	LENS_ResetPosition();
}
#endif

//--------------------------------------------------------------------------------------
BYTE GetTwoMotorAfMode(void)
{
	//return api_param;
	return 0;//temp
}

//--------------------------------------------------------------------------------------
BYTE GetTwoMotorScanMode(void)
{
	//return api_param;
	return 0;//temp
}

//--------------------------------------------------------------------------------------
BYTE GetTwoMotorSyncTDN(void)
{
	//return api_param;
	return 0;//temp
}

//--------------------------------------------------------------------------------------
void SetPwmIrisTune(void)
{
#if defined(__USE_PWMIRIS_ONLY__)
	DCLENS_IRIS_Tune();
#endif
	Delay_mSec(1000);
	SetMenuStatus(MISCTUNE_PWMIRIS_MENU, 0);
	SetMenuRedraw();
}

//--------------------------------------------------------------------------------------
void ChangeTestMode(WORD nID)
{
	AECTRL_SetTestMode();
	Delay_mSec(500);
	SetMenuStatus(MISCTUNE_TESTMODE_MENU, 0);
	SetMenuRedraw();
}


/*  FILE_END_HERE */ 
