// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

#if defined(__USE_PWMIRIS_ONLY__)
static const LONG AECTRL_YREF_LEVEL[]		= {
	12000,	12300,	12600,	12900,	13200,	13500,	13800,	14100,	14400,	14700, //00 ~ 09
	15000,	15300,	15600,	15900,	16200,	16500,	16800,	17100,	17400,	17700, //10 ~ 19
	18000,	18300,	18600,	18900,	19200,	19500,	19800,	20100,	20400,	20700, //20 ~ 29
 	21000,	21500,	22000,	22500,	23000,	23500,	24000,	24500,	25000,	25500, //30 ~ 39
	26000,	26500,	27000,	27500,	28000,	28500,	29000,	29500,	30000,	30500, //40 ~ 49
	31000,	31500,	32000,	32500,	33000,	33500,	34000,	34500,	35000,	35500, //50 ~ 59
	36000,	36500,	37000,	37500,	38000,	38500,	39000,	39500,	40000,	40500, //60 ~ 69
	41000,	42000,	43000,	44000,	45000,	46000,	47000,	48000,	49000,	50000, //70 ~ 79
	51000,	52000,	53000,	54000,	55000,	56000,	57000,	58000,	59000,	60000, //80 ~ 89
	62000,	64000,	66000,	68000,	70000,	72000,	74000,	76000,	78000,	80000, //90 ~ 99
  	85000
};
#else
static const LONG AECTRL_YREF_LEVEL[]		= {
  	// max = 103573 //ref = 36250 //1920x1080
	5500,	6000,	6500,	7000,	7500,	8000,	8500,	9000,	9500,	10000, //0 ~ 9
	11000,	11500,	12000,	12500,	13000,	13500,	14000,	14500,	15000,	15500, //10 ~ 19
	16000,	16500,	17000,	17500,	18000,	18500,	19000,	19500,	20000,	20500, //20 ~ 29
	21000,	21500,	22000,	22500,	23000,	23500,	24000,	24500,	25000,	25500, //30 ~ 39
	26000,	26500,	27000,	27500,	28000,	28500,	29000,	29500,	30000,	30500, //40 ~ 49
	31000,	31500,	32000,	32500,	33000,	33500,	34000,	34500,	35000,	35500, //50 ~ 59
	36000,	36500,	37000,	37500,	38000,	38500,	39000,	39500,	40000,	40500, //60 ~ 69
	41000,	42000,	43000,	44000,	45000,	46000,	47000,	48000,	49000,	50000, //70 ~ 79
	51000,	52000,	53000,	54000,	55000,	56000,	57000,	58000,	59000,	60000, //80 ~ 89
	62000,	64000,	66000,	68000,	70000,	72000,	74000,	76000,	78000,	80000, //90 ~ 99
  	85000	
};
#endif

static const SHORT AECTRL_GAIN_BASE[]		= {
	  0,   3,   7,  10,  13,  17,  20,  23,  27,  30,  33,  37,  40,  43,  46,  49,
	 53,  56,  59,  62,  65,  68,  72,  75,  78,  81,  84,  87,  90,  93,  96,  99,
	102, 105, 108, 111, 114, 117, 120, 123, 126, 129, 132, 135, 138, 141, 144, 146,
	149, 152, 155, 158, 161, 163, 166, 169, 172, 175, 177, 180, 183, 186, 188, 191,
	194, 197, 199, 202, 205, 207, 210, 213, 215, 218, 221, 223, 226, 228, 231, 234,
	236, 239, 241, 244, 246, 249, 252, 254, 257, 259, 262, 264, 267, 269, 272, 274,
	277, 279, 282, 284, 286, 289, 291, 294, 296, 299, 301, 303, 306, 308, 310, 313,
	315, 318, 320, 322, 325, 327, 329, 332, 334, 336, 339, 341, 343, 345, 348, 350,
	352, 354, 357, 359, 361, 363, 366, 368, 370, 372, 375, 377, 379, 381, 383, 385,
	388, 390, 392, 394, 396, 398, 401, 403, 405, 407, 409, 411, 413, 415, 418, 420,
	422, 424, 426, 428, 430, 432, 434, 436, 438, 440, 442, 444, 446, 448, 450, 452,
	454, 456, 458, 460, 462, 464, 466, 468, 470, 472, 474, 476, 478, 480, 482, 484,
	486, 488, 490, 492, 494, 496, 498, 500, 501, 503, 505, 507, 509, 511, 513, 515,
	517, 518, 520, 522, 524, 526, 528, 530, 531, 533, 535, 537, 539, 541, 542, 544,
	546, 548, 550, 551, 553, 555, 557, 559, 560, 562, 564, 566, 567, 569, 571, 573,
	574, 576, 578, 580, 581, 583, 585, 587, 588, 590, 592, 594, 595, 597, 599, 600
};

#if defined(__USE_X30X_TAMRON__)
const LONG AECTRL_CONF_F_VALUE = 4;

const LONG AECTRL_F_VALUE[8] = {
	105, // F16
	238, // F11
	484, // F8
	550, // F5.6
	645, // F4
	789, // F2.8
	827, // F2
	950, // F1.6
};

static const LONG AECTRL_BR_VALUE[18] = {
	0,	 // close
	0,	 // close
	0,	 // close
	0,	 // close
	105,	 // F16
	126, // F14
	238, // F11
	361, // F9.6
	484, // F8.0
	517, // F6.8
	550, // F5.6
	598, // F4.8
	645, // F4.0
	717, // F3.4
	789, // F2.8
	807, // F2.4
	827, // F2.0
	950, // F1.6
};

#elif defined(__USE_X10X_UNION__)
const LONG AECTRL_CONF_F_VALUE = 4;

const LONG AECTRL_F_VALUE[8] = {
	150, // F16
	200, // F11
	270, // F8
	350, // F5.6
	420, // F4
	550, // F2.8
	700, // F2
	1023, // F1.6
};

static const LONG AECTRL_BR_VALUE[18] = {
	0,	 // close
	0,	 // close
	0,	 // close
	0,	 // close
	150,	 // *F16
	170, // F14
	200, // *F11
	231, // F9.6
	270, // *F8.0
	300, // F6.8
	350, // *F5.6
	380, // F4.8
	420, // *F4.0
	470, // F3.4
	550, // *F2.8
	600, // F2.4
	700, // *F2.0
	1023, // *F1.6
};

#elif defined(__USE_X10X_ZMAX__)
const LONG AECTRL_CONF_F_VALUE = 4;

const LONG AECTRL_F_VALUE[8] = {
	180, // F16
	205, // F11
	238, // F8
	384, // F5.6
	450, // F4
	545, // F2.8
	689, // F2
	850, // F1.6
};

static const LONG AECTRL_BR_VALUE[18] = {
	0,	 // close
	0,	 // close
	0,	 // close
	0,	 // close
	180,	 // F16
	190, // F14
	205, // F11
	215, // F9.6
	238, // F8.0
	300, // F6.8
	384, // F5.6
	410, // F4.8
	450, // F4.0
	500, // F3.4
	545, // F2.8
	607, // F2.4
	689, // F2.0
	850, // F1.6
};

#elif defined(__USE_X12X_UNION__)
const LONG AECTRL_CONF_F_VALUE = 4;

const LONG AECTRL_F_VALUE[8] = {
	80+73, // F16
	80+85, // F11
	80+100, // F8
	80+120, // F5.6
	80+150, // F4
	80+200, // F2.8
	80+300, // F2
	80+370, // F1.6
};

static const LONG AECTRL_BR_VALUE[18] = {
	0,	 // close
	0,	 // close
	0,	 // close
	0,	 // close
	80+73, // F16
	80+79, // F14
	80+85, // F11
	80+92, // F9.6
	80+100, // F8.0
	80+110, // F6.8
	80+120, // F5.6
	80+135, // F4.8
	80+150, // F4.0
	80+175, // F3.4
	80+200, // F2.8
	80+250, // F2.4
	80+300, // F2.0
	80+370, // F1.8
};

#elif defined(__USE_X03X_ZMAX__)
const LONG AECTRL_CONF_F_VALUE = 4;

const LONG AECTRL_F_VALUE[8] = {
	180, // F16
	205, // F11
	238, // F8
	384, // F5.6
	450, // F4
	545, // F2.8
	689, // F2
	850, // F1.6
};

static const LONG AECTRL_BR_VALUE[18] = {
	0,	 // close
	0,	 // close
	0,	 // close
	0,	 // close
	180,	 // F16
	190, // F14
	205, // F11
	215, // F9.6
	238, // F8.0
	300, // F6.8
	384, // F5.6
	410, // F4.8
	450, // F4.0
	500, // F3.4
	545, // F2.8
	607, // F2.4
	689, // F2.0
	850, // F1.6
};

#else
const LONG AECTRL_CONF_F_VALUE = 4;

const LONG AECTRL_F_VALUE[8] = {
	105, // F16
	138, // F11
	184, // F8
	250, // F5.6
	345, // F4
	489, // F2.8
	727, // F2
	943, // F1.6
};

static const LONG AECTRL_BR_VALUE[18] = {
	0,	 // close
	0,	 // close
	0,	 // close
	0,	 // close
	105,	 // F16
	126, // F14
	138, // F11
	161, // F9.6
	184, // F8.0
	217, // F6.8
	250, // F5.6
	298, // F4.8
	345, // F4.0
	417, // F3.4
	489, // F2.8
	607, // F2.4
	727, // F2.0
	943, // F1.6
};

#endif

BYTE AE_StdDevMin = 1;
int AE_GaindB = 0;

#if defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX385__)  || defined(__USE_CMOS_IMX327__)
// std dev. min value(0dB ~ 23)
#define	maxStdDevNum	24
static BYTE StdDevMin[maxStdDevNum] = {
		15,15,16,17,20,21,22,23,25,27,
		28,30,31,32,33,35,36,38,40,42,
		44,45,46,48};
#else
#define	maxStdDevNum	24
static BYTE StdDevMin[maxStdDevNum] = {
		15,15,16,17,20,21,22,23,25,27,
		28,30,31,32,33,35,36,38,40,42,
		44,45,46,48};
#endif

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
extern BOOL dGainFlag;
#if defined(__USE_CMOS_EXT1__)
extern int dGainInterval[13] = {250, 130, 80, 60, 50, 40, 40, 40, 50, 50, 40, 30, 30};
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

//--------------------------------------------------------------------------------------------------------------------------
extern FLOAT odmRatioW, odmRatioH;
FLOAT aeWinRatio = 1.0f;

void AECTRL_SetInstancePARAM(PAE_CTRL_PARAM pCTRL)
{
	memset((PBYTE)pCTRL, 0, sizeof(AE_CTRL_PARAM));

#if defined(__USE_DCIRIS_HALL__) || defined(__USE_DCIRIS_DAMP__) || defined(__USE_PWMIRIS_ONLY__) || defined(__USE_P_IRIS_ONLY__)
	pCTRL->ctrl = AECTRL_DCIRIS_LENS | AECTRL_PRIORITY_AUTO;
						// AECTRL_PRIORITY_AUTO ---------- (Default)
						// AECTRL_PRIORITY_SHUTTER (Shutter fixed / Aperture Control)
						// AECTRL_PRIORITY_APERTURE (Aperture fixed / Shutter Control)
						// AECTRL_PRIORITY_SHUT_GAIN (Shutter -> Gain -> Aperture)
#else
	if(LoadAELensType()==0)		{pCTRL->ctrl = AECTRL_MANUAL_LENS | AECTRL_PRIORITY_AUTO;	UARTprintf("Manual Lens\r\n");}
	else						{pCTRL->ctrl = AECTRL_DCIRIS_LENS | AECTRL_PRIORITY_AUTO;	UARTprintf("DC-IRIS Lens\r\n");}
#endif

	// set WINDOW(FIX/BLC/HLC/SPOT) factor
	pCTRL->stFIX.sX =   (BYTE)(13.0f*odmRatioW);		pCTRL->stFIX.sY =    (BYTE)(9.0f*odmRatioH);
	pCTRL->stFIX.nW =	   (BYTE)((FLOAT)AE_WND_DEF_SIZE_W*odmRatioW);		pCTRL->stFIX.nH =   (BYTE)((FLOAT)AE_WND_DEF_SIZE_H*odmRatioH);
	//UARTprintf("---------------------------------------------%d %d %d %d \r\n", pCTRL->stFIX.sX, pCTRL->stFIX.sY, pCTRL->stFIX.nW,pCTRL->stFIX.nH);
	pCTRL->stFIX.W0 =   32;
	pCTRL->stFIX.W1 =   32;
	pCTRL->stFIX.W2 =   32;
	pCTRL->stFIX.W3 =   32;
	pCTRL->stFIX.W4 =   32;

	pCTRL->stBLC.sX =  (BYTE)(12.0f*odmRatioW);		pCTRL->stBLC.sY =  (BYTE)(8.0f*odmRatioW);
	pCTRL->stBLC.nW =  (BYTE)(23.0f*odmRatioW);		pCTRL->stBLC.nH =  (BYTE)(17.0f*odmRatioW);

	pCTRL->stBLC.W0 =  32;
	pCTRL->stBLC.W1 =    0;
	pCTRL->stBLC.W2 =    0;
	pCTRL->stBLC.W3 =    0;
	pCTRL->stBLC.W4 =    0;

	pCTRL->stHLC.sX =  (BYTE)(2.0f*odmRatioW);		pCTRL->stHLC.sY =  (BYTE)(5.0f*odmRatioW);
	pCTRL->stHLC.nW =  (BYTE)(45.0f*odmRatioW);		pCTRL->stHLC.nH =  (BYTE)(25.0f*odmRatioW);

	pCTRL->stHLC.W0 =   32;
	pCTRL->stHLC.W1 =   32;
	pCTRL->stHLC.W2 =   32;
	pCTRL->stHLC.W3 =   32;
	pCTRL->stHLC.W4 =   32;

	pCTRL->stHLC.fix = 100;
	pCTRL->stHLC.max = 0xffffffff;
	pCTRL->stHLC.val = 128;

	pCTRL->stSPOT.sX =  (BYTE)(13.0f*odmRatioW);		pCTRL->stSPOT.sY =  (BYTE)(9.0f*odmRatioW);
	pCTRL->stSPOT.nW =  (BYTE)(21.0f*odmRatioW);		pCTRL->stSPOT.nH =  (BYTE)(15.0f*odmRatioW);

	pCTRL->stSPOT.W0 =   128;
	pCTRL->stSPOT.W1 =   8;
	pCTRL->stSPOT.W2 =   8;
	pCTRL->stSPOT.W3 =   8;
	pCTRL->stSPOT.W4 =   8;

	// set AGC-GAIN range factor
#if defined(__USE_CMOS_MN34220__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	AECTRL_SetSHUTRange(0, 1122);		AECTRL_SetSHUTGain(0);
	AECTRL_SensUpInit(1125);
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(60, 0x280); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(0x280); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());

#elif defined(__USE_CMOS_MN34420__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	AECTRL_SetSHUTRange(0, 1122);		AECTRL_SetSHUTGain(0);
	AECTRL_SensUpInit(1125);
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(60, 0x280); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(0x280); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());

#elif defined(__USE_CMOS_IMX124__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	#if defined(__USE_CMOS_IMX123__)
	  #if defined(__USE_IMX124_1536P30_MODE__)	
	AECTRL_SetSHUTRange(4, 1598);		AECTRL_SetSHUTGain(4);
	AECTRL_SensUpInit(1600);
	  #elif defined(__USE_IMX124_1080P30_MODE__)	||defined(__USE_IMX124_1080P60_MODE__)	
	AECTRL_SetSHUTRange(4, 1123);		AECTRL_SetSHUTGain(4);
	AECTRL_SensUpInit(1125);
	  #endif
	#else
	  #if defined(__USE_IMX124_1536P30_MODE__)	
	AECTRL_SetSHUTRange(2, 1598);		AECTRL_SetSHUTGain(2);
	AECTRL_SensUpInit(1600);
	  #elif defined(__USE_IMX124_1080P30_MODE__)	||defined(__USE_IMX124_1080P60_MODE__)	
	AECTRL_SetSHUTRange(2, 1123);		AECTRL_SetSHUTGain(2);
	AECTRL_SensUpInit(1125);
	  #endif
	#endif
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(51, 510); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(510); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());

#elif defined(__USE_CMOS_IMX174__) || defined(__USE_CMOS_IMX249__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	AECTRL_SetSHUTRange(6, 1124);		AECTRL_SetSHUTGain(6);
	AECTRL_SensUpInit(1125);
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(48, 480); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(480); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());
#elif defined(__USE_CMOS_IMX252__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);
	#if defined(__USE_TRIGGER_MODE__)
	AECTRL_SetSHUTRange(10, 1125);
	#else
	AECTRL_SetSHUTRange(10, 1124);
	#endif
	AECTRL_SetSHUTGain(10);
	AECTRL_SensUpInit(1125);
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(48, 480); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(480); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());

#elif defined(__USE_CMOS_IMX265__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);
	AECTRL_SetSHUTRange(10, CMOS_GetVmax()-1);
	AECTRL_SetSHUTGain(10);
	AECTRL_SensUpInit(CMOS_GetVmax());
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(48, 480); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(480); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());

#elif defined(__USE_CMOS_IMXx22__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	AECTRL_SetSHUTRange(0, 1123);		AECTRL_SetSHUTGain(0);
	AECTRL_SensUpInit(1125);
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(42, 140); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(140); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());

#elif defined(__USE_CMOS_IMX323__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	AECTRL_SetSHUTRange(0, 1123);		AECTRL_SetSHUTGain(0);
	AECTRL_SensUpInit(1125);
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(45, 150); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(150); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());

#elif defined(__USE_CMOS_IMX238__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	AECTRL_SetSHUTRange(0, 1090);		AECTRL_SetSHUTGain(0);
	AECTRL_SensUpInit(1125);
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(48, 160); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(160); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());

#elif defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	#if defined(__USE_IMX290_720P120_MODE__) || defined(__USE_IMX290_720P60_MODE__) || defined(__USE_IMX290_720P30_MODE__)
	AECTRL_SetSHUTRange(1, 748);		AECTRL_SetSHUTGain(1);
	AECTRL_SensUpInit(750);
	#else
	AECTRL_SetSHUTRange(1, 1123);		AECTRL_SetSHUTGain(1);
	AECTRL_SensUpInit(1125);
	#endif
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	#if defined(__USE_CMOS_IMX290__)
	SetCMOSGainMax(72, 240); //CMOS gain range dB & register value : value 10 = 3dB
	//SetCMOSGainOpt_num(240); // CMOS gain used(max value of using register value)
	SetCMOSGainOpt_num(220); // CMOS gain used(max value of using register value)
	#else
	SetCMOSGainMax(69, 230); //CMOS gain range dB & register value : value 10 = 3dB
	SetCMOSGainOpt_num(230); // CMOS gain used(max value of using register value)
	#endif
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());
#elif defined(__USE_CMOS_IMX385__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	AECTRL_SetSHUTRange(2, 1123);		AECTRL_SetSHUTGain(0);
	AECTRL_SensUpInit(1125);
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(72, 720); //CMOS gain range dB & register value : value 10 = 3dB
	//SetCMOSGainOpt_num(240); // CMOS gain used(max value of using register value)
	SetCMOSGainOpt_num(720); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());
#elif defined(__USE_CMOS_IMXx36__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	AECTRL_SetSHUTRange(0, 1124);		AECTRL_SetSHUTGain(0);
	AECTRL_SensUpInit(1125);
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(48, 480); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(480); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());

#elif defined(__USE_CMOS_IMX178__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	#if defined(__USE_IMX178_1080P60_MODE__) || defined(__USE_IMX178_1080P30_MODE__)
	AECTRL_SetSHUTRange(0, 1649);		AECTRL_SetSHUTGain(0);
	#else		//#define	__USE_IMX178_1728P30_MODE__		//3072x1728p30(5m 16:9) Window crop mode(5.3M) same lvds data rate 1080p60
	AECTRL_SetSHUTRange(0, 1979);		AECTRL_SetSHUTGain(0);
	#endif
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(48, 480); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(480); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());

#elif defined(__USE_CMOS_IMX226__)
	pCTRL->ctrl |= AECTRL_GAINCTL_DMAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	#if defined(__USE_IMX226_1080P60_MODE__)
	AECTRL_SetSHUTRange(10, 2275-4);		AECTRL_SetSHUTGain(10);
	AECTRL_SensUpInit(2275);
	#elif defined(__USE_TVI_TX__) && defined(__USE_IMX226_4K15P_MODE__)
	//AECTRL_SetSHUTRange(8, 4316);		AECTRL_SetSHUTGain(8);
	AECTRL_SetSHUTRange(1098, 2200-4);		AECTRL_SetSHUTGain(8);
	AECTRL_SensUpInit(2200);
	#elif defined(__USE_IMX226_4K3KP30_MODE0__)
	AECTRL_SetSHUTRange(10, 2275-4);		AECTRL_SetSHUTGain(10);
	AECTRL_SensUpInit(3120);
	#else		//#define	__USE_IMX178_1728P30_MODE__		//3072x1728p30(5m 16:9) Window crop mode(5.3M) same lvds data rate 1080p60
	//AECTRL_SetSHUTRange(8, 4316);		AECTRL_SetSHUTGain(8);
	AECTRL_SetSHUTRange(8, 2200-4);		AECTRL_SetSHUTGain(8);
	// AECTRL_SensUpInit(2200); // Sensup ss fps issue
	#if defined(__USE_ISP_SCLK_OUT__)	// LVDS Clock Rate = 297MHz
	AECTRL_SensUpInit(2250); //2250
	#else								// LVDS Clock Rate = 288MHz
	AECTRL_SensUpInit(2200); //2000
	#endif
	#endif
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(27, 1957); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(1957); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*32.0f/(FLOAT)GetCMOSGainMax_dB());

#elif defined(__USE_CMOS_IMX253__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);
	#if defined(__USE_TRIGGER_MODE__)
	AECTRL_SetSHUTRange(20, CMOS_GetVmax()-1);
	AECTRL_SensUpInit(CMOS_GetVmax());
	#else
	AECTRL_SetSHUTRange(20, CMOS_GetVmax()-1);
	AECTRL_SensUpInit(CMOS_GetVmax());
	#endif
	AECTRL_SetSHUTGain(20);
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(48, 480); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(480); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());

#elif defined(__USE_CMOS_IMX185__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	AECTRL_SetSHUTRange(0, 1124);		AECTRL_SetSHUTGain(0);
	AECTRL_SensUpInit(1125);
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(48, 160); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(160); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());

#elif defined(__USE_CMOS_IMX334__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	AECTRL_SetSHUTRange(5, CMOS_GetVmax()-1);		AECTRL_SetSHUTGain(5);
	AECTRL_SensUpInit(CMOS_GetVmax());
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(72, 240); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(240); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());
	UARTprintf("VMAX : %d\r\n",CMOS_GetVmax());

#elif defined(__USE_CMOS_IMX273__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	AECTRL_SetSHUTRange(15, CMOS_GetVmax()-1);		AECTRL_SetSHUTGain(15);		//minimum shut 14 --> 15 for fasttrigger mode problem
	AECTRL_SensUpInit(CMOS_GetVmax());
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(48, 480); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(480); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());


#elif defined(__USE_CMOS_IMX274__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	#if defined(__USE_IMX274_1080P60_MODE__)
	//AECTRL_SetSHUTRange(8, 1124);		AECTRL_SetSHUTGain(8);
	AECTRL_SetSHUTRange(8, 2169-4);		AECTRL_SetSHUTGain(8);
	AECTRL_SetBrightness(pCTRL, 6*5);
	AECTRL_SensUpInit(CMOS_GetVmax());
//	#elif defined(__USE_IMX274_4K15P_MODE__)
//	AECTRL_SetSHUTRange(2132, 4447);		AECTRL_SetSHUTGain(12);	//4451-4? 2160x2?  //8 or 12 ~ 4451-4 or 4379-4
//	AECTRL_SetBrightness(pCTRL, 7*5);
//	AECTRL_SensUpInit(4451);
	#else		
	AECTRL_SetSHUTRange(12, 4500-4);		AECTRL_SetSHUTGain(12);	//4451-4? 2160x2?  //8 or 12 ~ 4451-4 or 4379-4
	AECTRL_SetBrightness(pCTRL, 7*5);
	AECTRL_SensUpInit(4500);
	//AECTRL_SetSHUTRange(12, 4447);		AECTRL_SetSHUTGain(12);	//4451-4? 2160x2?  //8 or 12 ~ 4451-4 or 4379-4
	//AECTRL_SetBrightness(pCTRL, 7*5);
	//AECTRL_SensUpInit(4451);
	#endif
	SetCMOSGainMax(27, 1957); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(1957); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());

#elif defined(__USE_CMOS_IMX326__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	#if defined(__USE_IMX326_1080P30_MODE__)
	AECTRL_SetSHUTRange(8, 1124);		AECTRL_SetSHUTGain(1);
	AECTRL_SetBrightness(pCTRL, 30);
	AECTRL_SensUpInit(1125);
	#else		
	AECTRL_SetSHUTRange(12, 4447);		AECTRL_SetSHUTGain(12);	//4451-4(mode 0) or 4379-4(mode 1)
	AECTRL_SetBrightness(pCTRL, 7*5);
	AECTRL_SensUpInit(4451);
	#endif
	SetCMOSGainMax(27, 1957); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(1957); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());

#elif defined(__USE_CMOS_EXT2__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x10000);	AECTRL_SetGAINGain(256);
	AECTRL_SetSHUTRange(0, 1020);		AECTRL_SetSHUTGain(0);
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	pCTRL->agcDiv = 16; // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = 750;

#elif defined(__USE_CMOS_EXT1__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x40000);	AECTRL_SetGAINGain(256);
	AECTRL_SetSHUTRange(0, 1597);		AECTRL_SetSHUTGain(0);
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 11*5);
	pCTRL->agcDiv = 16; // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = 750;

#elif defined(__USE_CMOS_OV2710__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	AECTRL_SetSHUTRange(0, 1079);		AECTRL_SetSHUTGain(0);
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 7*5);
	pCTRL->agcDiv = 16; // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = 750;

#elif defined(__USE_CMOS_PS1400K__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(256);
	AECTRL_SetSHUTRange(0, 1438);		AECTRL_SetSHUTGain(0);
	// set DEAD-BAND, Brightness factor & AGC speed
	AECTRL_SetBrightness(pCTRL, 6*5);
	SetCMOSGainMax(30, 0x3f); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(0x3f); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());

#elif defined(__USE_CMOS_AR0134__) ||defined( __USE_CMOS_AR0135__)
	pCTRL->ctrl |= AECTRL_GAINCTL_MAX;
	AECTRL_SetGAINRange(256, 0x20000);	AECTRL_SetGAINGain(0);
	AECTRL_SetSHUTRange(0, 719);		AECTRL_SetSHUTGain(0);
	AECTRL_SetBrightness(pCTRL, 30);
	AECTRL_SensUpInit(720);
	SetCMOSGainMax(64, 255); //CMOS gain range dB & register value
	SetCMOSGainOpt_num(255); // CMOS gain used(max value of using register value)
	pCTRL->agcDiv = MAX(1,MIN(32,((FLOAT)GetCMOSGainMax_dB()*16.0f/64.0f))); // age gain speed... normal value (6~32), low value is high speed of agc.
	//pCTRL->band = (LONG)(400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());
	pCTRL->band = (LONG)(4*400.0f*64.0f/(FLOAT)GetCMOSGainMax_dB());

#endif

	AECTRL_SetGAINPMax(0x40000);
	SetAEInitGainCtrlMode(pCTRL->ctrl&AECTRL_GAIN_MASK);

#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
	DCLENS_SetIRISRange(IRIS_MIN, IRIS_MAX);		DCLENS_SetIRISGain(IRIS_DEFAULT);
	DCLENS_IRISInit();
#elif defined(__USE_DCIRIS_DAMP__)
	DCLENS_SetIRISRange(4, 1012);		DCLENS_SetIRISGain(803);
#endif
	
	aeWinRatio = AECTRL_CalAERatio((pCTRL->ctrl&AECTRL_BLC_MASK), pCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetAE_ShutterFixMode(void)
{
	PAE_CTRL_PARAM pCTRL = &stAECTRL;

	pCTRL->ctrl = (pCTRL->ctrl & (~AECTRL_PRIORITY_MASK)) | AECTRL_PRIORITY_SHUTTER;
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetAE_IRISFixMode(void)
{
	PAE_CTRL_PARAM pCTRL = &stAECTRL;

	pCTRL->ctrl = (pCTRL->ctrl & (~AECTRL_PRIORITY_MASK)) | AECTRL_PRIORITY_APERTURE;
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetAE_AutoMode(void)
{
	PAE_CTRL_PARAM pCTRL = &stAECTRL;

	pCTRL->ctrl = (pCTRL->ctrl & (~AECTRL_PRIORITY_MASK)) | AECTRL_PRIORITY_AUTO;
}

void AECTRL_SetAE_GainFixMode(void)
{
	PAE_CTRL_PARAM pCTRL = &stAECTRL;

	pCTRL->ctrl = (pCTRL->ctrl & (~AECTRL_PRIORITY_MASK)) | AECTRL_PRIORITY_GAIN;
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetAE_Manual(void)
{
	PAE_CTRL_PARAM pCTRL = &stAECTRL;

	pCTRL->ctrl = (pCTRL->ctrl & (~AECTRL_PRIORITY_MASK)) | AECTRL_ALL_MANUAL;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AECTRL_IsAE_FLKMode(void)
{
	PAE_CTRL_PARAM pCTRL = &stAECTRL;

	LONG ltemp = (pCTRL->ctrl & AECTRL_PRIORITY_MASK);

#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
	if((ltemp == AECTRL_PRIORITY_AUTO)||(ltemp == AECTRL_PRIORITY_SHUTTER)||(ltemp == AECTRL_PRIORITY_GAIN)) {
		if(DCLENS_IsIRISCtrlOptiMin() == TRUE) return FALSE;
		return TRUE;
	} else {
		return FALSE;
	}
#else
	if((ltemp == AECTRL_PRIORITY_AUTO)||(ltemp == AECTRL_PRIORITY_SHUTTER)||(ltemp == AECTRL_PRIORITY_GAIN)) {
		return TRUE;
	} else {
		return FALSE;
	}
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetAE_IRISValue(SHORT gain)
{
#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
	if((gain >=0) && (gain < (sizeof(AECTRL_F_VALUE)/sizeof(AECTRL_F_VALUE[0]))))
	DCLENS_ForceSetIRISGain2(AECTRL_F_VALUE[gain]);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetAE_IRISBRValue(SHORT gain)
{
#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
	if((gain >=0) && (gain < (sizeof(AECTRL_BR_VALUE)/sizeof(AECTRL_BR_VALUE[0]))))
	DCLENS_ForceSetIRISGain2(AECTRL_BR_VALUE[gain]);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetConferIRISValue(SHORT gain)
{
#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
	if(gain == 0)
		DCLENS_ForceSetIRISGain2(AECTRL_F_VALUE[AECTRL_CONF_F_VALUE]);
	else
		DCLENS_ForceSetIRISGain2(AECTRL_F_VALUE[gain]);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE AECTRL_GetAE_IRISBRValue(LONG igain)
{
	BYTE brgain = 0;
#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
	if(igain == AECTRL_BR_VALUE[0])		brgain = 3;
	else if(igain <= AECTRL_BR_VALUE[4])	brgain = 4;
	else if(igain <= AECTRL_BR_VALUE[5])	brgain = 5;
	else if(igain <= AECTRL_BR_VALUE[6])	brgain = 6;
	else if(igain <= AECTRL_BR_VALUE[7])	brgain = 7;
	else if(igain <= AECTRL_BR_VALUE[8])	brgain = 8;
	else if(igain <= AECTRL_BR_VALUE[9])	brgain = 9;
	else if(igain <= AECTRL_BR_VALUE[10])	brgain = 10;
	else if(igain <= AECTRL_BR_VALUE[11])	brgain = 11;
	else if(igain <= AECTRL_BR_VALUE[12])	brgain = 12;
	else if(igain <= AECTRL_BR_VALUE[13])	brgain = 13;
	else if(igain <= AECTRL_BR_VALUE[14])	brgain = 14;
	else if(igain <= AECTRL_BR_VALUE[15])	brgain = 15;
	else if(igain <= AECTRL_BR_VALUE[16])	brgain = 16;
	else	brgain = 17;
#endif
		
	return brgain;
}

//--------------------------------------------------------------------------------------------------------------------------
void AEXFER_SetInstancePARAM(PAE_XFER_PARAM pXFER)
{
	memset((PBYTE)pXFER, 0, sizeof(AE_XFER_PARAM));
}

//--------------------------------------------------------------------------------------------------------------------------
//static DWORD gainBak = 0xffff;

static DWORD bakGain = 0xffff;
static DWORD bakShut = 0xffff;
static DWORD bakFine = 0xffff;

#if defined(__USE_CMOS_MN34220__)
static DWORD bakExp1 = 0xffff;
static DWORD bakExp2 = 0xffff;
#elif defined(__USE_CMOS_MN34420__)
static DWORD bakExp1 = 0xffff;
static DWORD bakExp2 = 0xffff;
static DWORD bakGap1 = 0xffff;
static DWORD bakGap2 = 0xffff;
#else
static DWORD bakShs2 = 0xffff;
static DWORD bakShs3 = 0xffff;
static DWORD bakRhs1 = 0xffff;
static DWORD bakRhs2 = 0xffff;
#endif

void AE_UpdateHandler(PAE_XFER_PARAM pXFER)
{
	if (fAECtrlODM_XFER==FALSE) return;
	fAECtrlODM_XFER = FALSE;

#if defined(__USE_DCIRIS_DAMP__)
    #if !defined(__DRIVE_VD_SHORT__)
		IRIS_VD(HIGH);	//IRIS_VD(LOW);
    #endif
		IRIS_WordWrite(SPI_AN41919_ADDR, 0x00, (SHORT)pXFER->iris);	// Xfer IRIS-gain
#elif defined(__USE_DCIRIS_HALL__)
    #if !defined(__DRIVE_VD_SHORT__)
		IRIS_VD(HIGH);	//IRIS_VD(LOW);
    #endif
    #if defined(__ECOM1_UNION_12X__)
		AECTRL_PIRIS_Handler();
    #else
	#if defined(__USE_DCIRIS_HALL__)
	#if (__AE_IRIS_XRC_TEST__ == 0)
		IRIS_WordWrite(SPI_AN41908_ADDR, 0x00, (SHORT)pXFER->iris);	// Xfer IRIS-gain
	#elif (__AE_IRIS_XRC_TEST__ == 1)
		IRIS_WordWrite(SPI_AN41908_ADDR, 0x00, 0);	// Full open
	#endif
	#endif
    #endif
#elif defined(__USE_P_IRIS_ONLY__)
	#if (__AE_IRIS_XRC_TEST__ == 1)
		pXFER->iris = 0;	// Full open
	#endif
#endif
#if !defined(__DRIVE_VD_SHORT__)
		IRIS_VD(LOW);	// iris vd temp patch
#endif

	if(bakGain != pXFER->gain) {
#if defined(__USE_CMOS_MN34041__)
		CMOS_WordWrite(SPI_MN34041_ADDR, 0x020, (WORD)pXFER->gain);
#elif defined(__USE_CMOS_MN34220__)
		CMOS_WordWrite(SPI_MN34220_ADDR, 0x0204, BYTESWAP(LOWORD(pXFER->gain)));	// analog
		CMOS_WordWrite(SPI_MN34220_ADDR, 0x3108, BYTESWAP(HIWORD(pXFER->gain)));	// digital
#elif defined(__USE_CMOS_MN34420__)
		CMOS_WordWrite(SPI_MN34420_ADDR, 0x3100, LOWORD(pXFER->gain));	// analog
		CMOS_WordWrite(SPI_MN34420_ADDR, 0x3108, LOWORD(pXFER->gain));	// analog (short)
		CMOS_WordWrite(SPI_MN34420_ADDR, 0x3110, LOWORD(pXFER->gain));	// analog (shorter)
		CMOS_WordWrite(SPI_MN34420_ADDR, 0x3102, HIWORD(pXFER->gain));	// digital
		CMOS_WordWrite(SPI_MN34420_ADDR, 0x310A, HIWORD(pXFER->gain));	// digital (short)
		CMOS_WordWrite(SPI_MN34420_ADDR, 0x3112, HIWORD(pXFER->gain));	// digital (shorter)
#elif defined(__USE_CMOS_IMX124__)
		CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0214, LOBYTE((WORD)pXFER->gain));
		CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0215, HIBYTE((WORD)pXFER->gain));
#elif defined(__USE_CMOS_IMX174__) || defined(__USE_CMOS_IMX249__)
		CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0404, LOBYTE((WORD)pXFER->gain));
		CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0405, HIBYTE((WORD)pXFER->gain));
#elif defined(__USE_CMOS_IMX252__)
		CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0404, LOBYTE((WORD)pXFER->gain));
		CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0405, HIBYTE((WORD)pXFER->gain));
#elif defined(__USE_CMOS_IMX265__)
		CMOS_ByteWrite(SPI_IMX265_ADDR, 0x0404, LOBYTE((WORD)pXFER->gain));
		CMOS_ByteWrite(SPI_IMX265_ADDR, 0x0405, HIBYTE((WORD)pXFER->gain));
#elif defined(__USE_CMOS_IMXx36__)
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0214, LOBYTE((WORD)pXFER->gain));
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0215, HIBYTE((WORD)pXFER->gain));
#elif defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__) 
		CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0214, LOBYTE((WORD)pXFER->gain));
		pXFER->dB = (FLOAT)pXFER->gain*3.0f/10.0f;
		//UARTprintf("pXFER->dB:%2.1f\r\n",pXFER->dB);
#elif defined(__USE_CMOS_IMX385__)
		CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0214, LOBYTE((WORD)pXFER->gain));
		CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0215, HIBYTE((WORD)pXFER->gain));
		pXFER->dB = (FLOAT)pXFER->gain/10.0f;
		//UARTprintf("pXFER->dB:%2.1f\r\n",pXFER->dB);
#elif (defined(__USE_CMOS_IMXx22__) || defined(__USE_CMOS_IMX323__))
		CMOS_ByteWrite(SPI_IMX222_ADDR, 0x021E, LOBYTE((WORD)pXFER->gain));
#elif defined(__USE_CMOS_IMX178__)
		CMOS_ByteWrite(SPI_IMX178_ADDR, 0x021F, LOBYTE((WORD)pXFER->gain));
		CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0220, HIBYTE((WORD)pXFER->gain));
#elif defined(__USE_CMOS_IMX226__)
//		CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0009, LOBYTE((WORD)pXFER->gain));		
//		CMOS_ByteWrite(SPI_IMX226_ADDR, 0x000A, HIBYTE((WORD)pXFER->gain));		//use [2:0]
		pXFER->fUpdateGainVBlank = 1;								// gain update timing : isp out vdpulse -> sensor v.blank(irq)
		pXFER->dB = -20.0f*log((2048-pXFER->gain)/2048.0f);
		//UARTprintf("pXFER->dB:%2.1f\r\n",pXFER->dB);
#elif defined(__USE_CMOS_IMX253__)
		CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0404, LOBYTE((WORD)pXFER->gain));
		CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0405, 1&(HIBYTE((WORD)pXFER->gain)));
		pXFER->dB = (FLOAT)pXFER->gain/10.0f;
#elif defined(__USE_CMOS_IMX185__)
		CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0214, LOBYTE((WORD)pXFER->gain));
#elif defined(__USE_CMOS_IMX334__)
		CMOS_ByteWrite(I2C_SONY_CMOS_ADDR, 0x30E8, LOBYTE((WORD)pXFER->gain));
		CMOS_ByteWrite(I2C_SONY_CMOS_ADDR, 0x30E9, HIBYTE((WORD)pXFER->gain));
#elif defined(__USE_CMOS_IMX273__)
		CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0404, LOBYTE((WORD)pXFER->gain));
		CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0405, HIBYTE((WORD)pXFER->gain));
#elif defined(__USE_CMOS_IMX274__)
		CMOS_ByteWrite(SPI_IMX274_ADDR, 0x000A, LOBYTE((WORD)pXFER->gain));		
		CMOS_ByteWrite(SPI_IMX274_ADDR, 0x000B, HIBYTE((WORD)pXFER->gain));		//when trigger mode?
		pXFER->dB = -20.0f*log((2048-pXFER->gain)/2048.0f);
		//UARTprintf("pXFER->dB:%2.1f\r\n",pXFER->dB);
#elif defined(__USE_CMOS_AR0134__) || defined( __USE_CMOS_AR0135__)
	#if defined(__USE_CMOS_AR0134__INTERNAL_AE)
	#else
		CMOS_WordWrite(I2C_AR0134_ADDR, 0x305E, pXFER->gain);	//Global gain
		//UARTprintf("gain : %d\r\n",pXFER->gain);
	#endif

#elif defined(__USE_CMOS_IMX326__)
		CMOS_ByteWrite(SPI_IMX326_ADDR, 0x000A, LOBYTE((WORD)pXFER->gain));		
		CMOS_ByteWrite(SPI_IMX326_ADDR, 0x000B, HIBYTE((WORD)pXFER->gain));		//when trigger mode?
#elif defined(__USE_CMOS_EXT2__)
#elif defined(__USE_CMOS_EXT1__)
		if(dGainFlag == FALSE) {
			CMOS_WordWrite(I2C_EXT1_ADDR, 0x0023,	LOBYTE((WORD)pXFER->gain));
			CMOS_WordWrite(I2C_EXT1_ADDR, 0x0024,	LOBYTE((WORD)pXFER->gain));
		}
#elif defined(__USE_CMOS_OV2710__)
		WORD tmpByteL,tmpByteH;
		WORD tmpWord;
		tmpByteL = (pXFER->gain)%16;
		tmpByteH = (pXFER->gain)/16;

		if(tmpByteH == 0)
			tmpByteH = 0x00;
		else if(tmpByteH == 1)
			tmpByteH = 0x10;
		else if(tmpByteH == 2)
			tmpByteH = 0x30;
		else if(tmpByteH == 3)
			tmpByteH = 0x70;
		else if(tmpByteH == 4)
			tmpByteH = 0xF0;
		else
			tmpByteH = 0x1F0;
		tmpWord = tmpByteH + tmpByteL;
		CMOS_ByteWrite(I2C_OV2710_ADDR, 0x350B, LOBYTE(tmpWord));
		CMOS_ByteWrite(I2C_OV2710_ADDR, 0x350A, HIBYTE(tmpWord));
		//UARTprintf("gain : %3x\r\n", pXFER->gain);
#elif defined(__USE_CMOS_PS1400K__)
		CMOS_WordWrite(I2C_PS1400K_ADDR, 0x3040, (BYTE)pXFER->gain); // [6:4] : coarse [3:0]:fine   ---- 2^coarse x (1+fine/16)
		//UARTprintf("gain : %d\r\n",pXFER->gain);
#endif
		bakGain = pXFER->gain;
		//UARTprintf("gain : %d\r\n", bakGain);
	}


	if(bakFine != pXFER->fine) {
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
		SPImAPP_SetSlaveAEDGain(pXFER->fine);
#endif
#if defined(__USE_CMOS_IMX226__)
		pXFER->fUpdateDGainVBlank = 1;
#else
		MISP_RegWrite(MISP_LOCAL_ID, 0x601, LOWORD(pXFER->fine));
#endif
		//UARTprintf("%d\r\n",bakFine - pXFER->fine);
		bakFine = pXFER->fine;
	}

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	if((bakGain != pXFER->gain) || (bakFine != pXFER->fine))
		SPImAPP_SetSlaveAEGainGain(AECTRL_GetGAINGain());
#endif


	if(bakShut != pXFER->shut) {
#if defined(__USE_CMOS_MN34041__)
		CMOS_WordWrite(SPI_MN34041_ADDR, 0x0a1, (WORD)pXFER->shut);
#elif defined(__USE_CMOS_MN34220__)
		CMOS_WordWrite(SPI_MN34220_ADDR, 0x0202, BYTESWAP(LOWORD(0x0463-pXFER->shut)));
#elif defined(__USE_CMOS_MN34420__)
		CMOS_WordWrite(SPI_MN34420_ADDR, 0x3104, LOWORD(pXFER->shut));
#elif defined(__USE_CMOS_IMX124__)
		CMOS_ByteWrite(SPI_IMX124_ADDR, 0x021E, LOBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX124_ADDR, 0x021F, HIBYTE((WORD)pXFER->shut));
#elif defined(__USE_CMOS_IMX174__)  || defined(__USE_CMOS_IMX249__)
		CMOS_ByteWrite(SPI_IMX174_ADDR, 0x029A, LOBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX174_ADDR, 0x029B, HIBYTE((WORD)pXFER->shut));
#elif defined(__USE_CMOS_IMX252__)
	#if defined(__USE_TRIGGER_MODE__)
		//MISP_RegWrite(MISP_LOCAL_ID, 0x571, pCTRL->stDESERIALIZER.sens_trig_accum_val);
		//MISP_RegWrite(MISP_LOCAL_ID, 0x572, pCTRL->stDESERIALIZER.trig_blank_width);
		MISP_RegWrite(MISP_LOCAL_ID, 0x571, CMOS_GetVmax() -(WORD)pXFER->shut);
		MISP_RegWrite(MISP_LOCAL_ID, 0x572, ((WORD)pXFER->shut));
	#else
		CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028D, LOBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028E, HIBYTE((WORD)pXFER->shut));		
	#endif		
#elif defined(__USE_CMOS_IMX265__)
	#if defined(__USE_TRIGGER_MODE__)
		//MISP_RegWrite(MISP_LOCAL_ID, 0x571, pCTRL->stDESERIALIZER.sens_trig_accum_val);
		//MISP_RegWrite(MISP_LOCAL_ID, 0x572, pCTRL->stDESERIALIZER.trig_blank_width);
		MISP_RegWrite(MISP_LOCAL_ID, 0x571, CMOS_GetVmax() -(WORD)pXFER->shut);
		MISP_RegWrite(MISP_LOCAL_ID, 0x572, ((WORD)pXFER->shut));
	#else
		CMOS_ByteWrite(SPI_IMX265_ADDR, 0x028D, LOBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX265_ADDR, 0x028E, HIBYTE((WORD)pXFER->shut));		
	#endif
#elif defined(__USE_CMOS_IMXx36__)
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0220, LOBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0221, HIBYTE((WORD)pXFER->shut));
#elif defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__)
		CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0220, LOBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0221, HIBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0222, HIWORD(pXFER->shut));
#elif defined(__USE_CMOS_IMX385__)
		CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0220, LOBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0221, HIBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0222, HIWORD(pXFER->shut));
#elif (defined(__USE_CMOS_IMXx22__) || defined(__USE_CMOS_IMX323__))
		CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0208, LOBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0209, HIBYTE((WORD)pXFER->shut));
#elif defined(__USE_CMOS_IMX178__)
		CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0234, LOBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0235, HIBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0236, 0);
#elif defined(__USE_CMOS_IMX226__)
//		CMOS_ByteWrite(SPI_IMX226_ADDR, 0x000B, LOBYTE((WORD)pXFER->shut));
//		CMOS_ByteWrite(SPI_IMX226_ADDR, 0x000C, HIBYTE((WORD)pXFER->shut));
		IMX226_SPIWrite(0x000B, (PBYTE)&pXFER->shut, 2);	// for shrink imx226 gain reg. communication timing : 91us -> 50us(-41us),  spec : vblank(0us) ~ 6XHS(93us)
		//UARTprintf("shut : %d\r\n", pXFER->shut);
#elif defined(__USE_CMOS_IMX253__)
	#if defined(__USE_TRIGGER_MODE__)
		//MISP_RegWrite(MISP_LOCAL_ID, 0x571, 2163-(WORD)pXFER->shut);
		MISP_RegWrite(MISP_LOCAL_ID, 0x571, CMOS_GetVmax()-(WORD)pXFER->shut);
		MISP_RegWrite(MISP_LOCAL_ID, 0x572, ((WORD)pXFER->shut));
	#else
		CMOS_ByteWrite(SPI_IMX253_ADDR, 0x028D, LOBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX253_ADDR, 0x028E, HIBYTE((WORD)pXFER->shut));
	#endif		
	pXFER->dB = (FLOAT)pXFER->gain/10.0f;
#elif defined(__USE_CMOS_IMX185__)
		CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0220, LOBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0221, HIBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0222, 0);
		pXFER->dB = (FLOAT)pXFER->gain*3.0f/10.0f;
		//UARTprintf("pXFER->dB:%2.1f\r\n",pXFER->dB);
#elif defined(__USE_CMOS_IMX334__)
		CMOS_ByteWrite(I2C_SONY_CMOS_ADDR, 0x3058, LOBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(I2C_SONY_CMOS_ADDR, 0x3059, HIBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(I2C_SONY_CMOS_ADDR, 0x305A, HIWORD(pXFER->shut));
		pXFER->dB = (FLOAT)pXFER->gain*3.0f/10.0f;
		//UARTprintf("pXFER->shut:%d\r\n",pXFER->shut);
#elif defined(__USE_CMOS_IMX273__)
	#if defined(__USE_TRIGGER_MODE__)
		MISP_RegWrite(MISP_LOCAL_ID, 0x571, CMOS_GetVmax() - (WORD)pXFER->shut);
		MISP_RegWrite(MISP_LOCAL_ID, 0x572, ((WORD)pXFER->shut));
	#else 
		CMOS_ByteWrite(SPI_IMX273_ADDR, 0x028D, LOBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX273_ADDR, 0x028E, HIBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX273_ADDR, 0x028F,  HIWORD(pXFER->shut));
	#endif
		pXFER->dB = (FLOAT)pXFER->gain/10.0f;
		//UARTprintf("pXFER->dB:%2.1f\r\n",pXFER->dB);
#elif defined(__USE_CMOS_IMX274__)
		CMOS_ByteWrite(SPI_IMX274_ADDR, 0x000C, LOBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX274_ADDR, 0x000D, HIBYTE((WORD)pXFER->shut));
#elif defined(__USE_CMOS_AR0134__) || defined( __USE_CMOS_AR0135__)
	#if defined(__USE_CMOS_AR0134__INTERNAL_AE)
	#else
		CMOS_WordWrite(I2C_AR0134_ADDR, 0x3012, 720-pXFER->shut);
		//UARTprintf("shut : %d\r\n",720-pXFER->shut);
	#endif
#elif defined(__USE_CMOS_IMX326__)
		CMOS_ByteWrite(SPI_IMX326_ADDR, 0x000C, LOBYTE((WORD)pXFER->shut));
		CMOS_ByteWrite(SPI_IMX326_ADDR, 0x000D, HIBYTE((WORD)pXFER->shut));
#elif defined(__USE_CMOS_EXT2__)
		//CMOS_WordWrite(I2C_EXT2_ADDR, 0x0001,	0x0001);
		//CMOS_WordWrite(I2C_EXT2_ADDR, 0x000F, (WORD)(pXFER->shut));
		//CMOS_WordWrite(I2C_EXT2_ADDR, 0x000F, (WORD)(0x063E - pXFER->shut));
		//CMOS_WordWrite(I2C_EXT2_ADDR, 0x000F,	0x063E);	//reg_shutter

		//CMOS_WordWrite(I2C_EXT2_ADDR, 0x0001,	0x0000);
#elif defined(__USE_CMOS_EXT1__)
		//CMOS_WordWrite(I2C_EXT1_ADDR, 0x000E, 421);
		CMOS_WordWrite(I2C_EXT1_ADDR, 0x000F, (WORD)(1598 - pXFER->shut)); //0x063E
		//UARTprintf("shut : %d\r\n", pXFER->shut);
#elif defined(__USE_CMOS_OV2710__)
		CMOS_ByteWrite(I2C_OV2710_ADDR, 0x3502, LOBYTE((WORD)((1080 - pXFER->shut)<<4)));
		CMOS_ByteWrite(I2C_OV2710_ADDR, 0x3501, HIBYTE((WORD)((1080 - pXFER->shut)<<4)));
		CMOS_ByteWrite(I2C_OV2710_ADDR, 0x3500, LOBYTE((WORD)(1080-pXFER->shut)<<12));
		//UARTprintf("shut : %4x(%4d)\r\n", pXFER->shut<<4, pXFER->shut);
#elif defined(__USE_CMOS_PS1400K__)
		CMOS_WordWrite(I2C_PS1400K_ADDR, 0x0202, 1438-pXFER->shut); // [15:0] ---- time = val*line_length_pck/2*pixel_clock_period
		//UARTprintf("shut : %d\r\n", 1438-pXFER->shut);
#endif

		bakShut = pXFER->shut;
		//UARTprintf("shut : %d\r\n", bakShut);
	}


	// registers about WDR exposure
	BOOL bSetWdr = FALSE;  // set registers only if target value is changed
#if defined(__USE_CMOS_MN34220__)
	if((bakExp1!= pXFER->exp1)||(bakExp2!= pXFER->exp2)) {
		bakExp1 = pXFER->exp1;   bakExp2 = pXFER->exp2;
		bSetWdr = TRUE;
	}
#elif defined(__USE_CMOS_MN34420__)
	if((bakExp1!= pXFER->exp1)||(bakExp2!= pXFER->exp2)||(bakGap1!= pXFER->gap1)||(bakGap2!= pXFER->gap2)) {
		bakExp1 = pXFER->exp1;   bakExp2 = pXFER->exp2;   bakGap1 = pXFER->gap1;   bakGap2 = pXFER->gap2;
		bSetWdr = TRUE;
	}
#else
	if((bakShs2!= pXFER->shs2)||(bakShs3!= pXFER->shs3)||(bakRhs1!= pXFER->rhs1)||(bakRhs2!= pXFER->rhs2)) {
		bakShs2 = pXFER->shs2;   bakShs3 = pXFER->shs3;   bakRhs1 = pXFER->rhs1;   bakRhs2 = pXFER->rhs2;
		bSetWdr = TRUE;
	}
#endif

	if(bSetWdr) {
#if defined(__USE_CMOS_MN34220__)
		CMOS_WordWrite(SPI_MN34220_ADDR, 0x312A, BYTESWAP(pXFER->exp1));
		CMOS_WordWrite(SPI_MN34220_ADDR, 0x312C, BYTESWAP(pXFER->exp2));
#elif defined(__USE_CMOS_MN34420__)
		CMOS_WordWrite(SPI_MN34420_ADDR, 0x310C, pXFER->exp1);
		CMOS_WordWrite(SPI_MN34420_ADDR, 0x3114, pXFER->exp2);
		CMOS_WordWrite(SPI_MN34420_ADDR, 0x431A, pXFER->gap1);
		CMOS_WordWrite(SPI_MN34420_ADDR, 0x431C, pXFER->gap2);
#elif defined(__USE_CMOS_IMX124__)
		CMOS_WordWrite(SPI_IMX124_ADDR,  0x0221, pXFER->shs2);
		CMOS_WordWrite(SPI_IMX124_ADDR,  0x0224, pXFER->shs3);
		CMOS_WordWrite(SPI_IMX124_ADDR,  0x022E, pXFER->rhs1);
		CMOS_WordWrite(SPI_IMX124_ADDR,  0x0231, pXFER->rhs2);
#elif defined(__USE_CMOS_IMX174__)||\
      defined(__USE_CMOS_IMX249__)
		CMOS_WordWrite(SPI_IMX174_ADDR,  0x029D, pXFER->shs2);
#elif defined(__USE_CMOS_IMXx36__)
		CMOS_WordWrite(SPI_IMX136_ADDR,  0x0223, pXFER->shs2);
#elif defined(__USE_CMOS_IMX185__)
		CMOS_WordWrite(SPI_IMX185_ADDR,  0x0223, pXFER->shs2);
#elif defined(__USE_CMOS_IMX265__)
		CMOS_ByteWrite(SPI_IMX265_ADDR,  0x0290, LOBYTE((WORD)pXFER->shs2));
		CMOS_ByteWrite(SPI_IMX265_ADDR,  0x0291, HIBYTE((WORD)pXFER->shs2));
		CMOS_ByteWrite(SPI_IMX265_ADDR,  0x0292, HIWORD(pXFER->shs2));
#elif defined(__USE_CMOS_IMX273__)
		CMOS_ByteWrite(SPI_IMX273_ADDR,  0x0290, LOBYTE((WORD)pXFER->shs2));
		CMOS_ByteWrite(SPI_IMX273_ADDR,  0x0291, HIBYTE((WORD)pXFER->shs2));
		CMOS_ByteWrite(SPI_IMX273_ADDR,  0x0292, HIWORD(pXFER->shs2));
#elif defined(__USE_CMOS_IMX290__)||\
      defined(__USE_CMOS_IMX327__)
		CMOS_WordWrite(SPI_IMX290_ADDR,  0x0224, pXFER->shs2);
		CMOS_WordWrite(SPI_IMX290_ADDR,  0x0228, pXFER->shs3);
		CMOS_WordWrite(SPI_IMX290_ADDR,  0x0230, pXFER->rhs1);
		CMOS_WordWrite(SPI_IMX290_ADDR,  0x0234, pXFER->rhs2);
#elif defined(__USE_CMOS_IMX385__)
		CMOS_WordWrite(SPI_IMX385_ADDR,  0x0223, pXFER->shs2);
		CMOS_WordWrite(SPI_IMX385_ADDR,  0x0226, pXFER->shs3);
		CMOS_WordWrite(SPI_IMX385_ADDR,  0x022C, pXFER->rhs1);
		CMOS_WordWrite(SPI_IMX385_ADDR,  0x022F, pXFER->rhs2);		
#endif
	}

}

#if defined(__USE_CMOS_IMX226__)
//--------------------------------------------------------------------------------------------------------------------------
void ISPM AE_UpdateGainVBlank(void)
{
	if(stAEXFER.fUpdateGainVBlank)	{
#if defined(__USE_IMX226_2160P60_MODE4__) || defined(__USE_IMX226_1080P60_MODE__)
		if(stAEXFER.gain < 0x147) {
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0352, 0x1e);		// APGC01[7:0] Analog Gain Setting 01			-- dependent PGC(see datasheet 37p)
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0353, 0x00);		// APGC01[8] Analog Gain Setting 01
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0356, 0x1d);		// APGC02[7:0] Analog Gain Setting 02                   -- dependent PGC(see datasheet 37p)
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0357, 0x00);		// APGC02[8] Analog Gain Setting 02
		} else if(stAEXFER.gain < 0x401) {
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0352, 0x20);
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0353, 0x00);
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0356, 0x1f);
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0357, 0x00);
		} else if(stAEXFER.gain < 0x52d) {
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0352, 0x25);
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0353, 0x00);
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0356, 0x24);
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0357, 0x00);
		} else if(stAEXFER.gain < 0x697) {
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0352, 0x27);
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0353, 0x00);
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0356, 0x26);
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0357, 0x00);
		} else if(stAEXFER.gain < 0x74c) {
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0352, 0x2b);
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0353, 0x00);
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0356, 0x2a);
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0357, 0x00);
		} else {
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0352, 0x2f);
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0353, 0x00);
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0356, 0x2e);
			CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0357, 0x00);
		}
#endif
		IMX226_SPIWrite(0x0009, (PBYTE)&stAEXFER.gain, 2);
		stAEXFER.fUpdateGainVBlank = 0;
	}
}
#endif

#if defined(__USE_CMOS_IMX226__)
//--------------------------------------------------------------------------------------------------------------------------
void ISPM AE_UpdateDGainVBlank(void)
{
	if(stAEXFER.fUpdateDGainVBlank)	{
		//UARTprintf("%d\r\n",stAEXFER.fine);
		MISP_RegWrite(MISP_LOCAL_ID, 0x601, stAEXFER.fine);

		stAEXFER.fUpdateDGainVBlank = 0;
	}
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
DWORD AECTRL_CnvGAINtoCMOS(DWORD gain)
{
	WORD nID, x, senVal;

	if (gain<256*  2)		nID =  0;
	else if (gain<256*   4)	nID =  1;
	else if (gain<256*   8)	nID =  2;
	else if (gain<256* 16)	nID =  3;
	else if (gain<256* 32)	nID =  4;
	else if (gain<256* 64)	nID =  5;
	else if (gain<256*128)	nID =  6;
	else if (gain<256*256)	nID =  7;
	else if (gain<256*512)	nID =  8;
	else					nID =  9;

 	x = MAX(0,(LONG)(gain>>nID)-256);	if (x>255) x = 255;

#if defined(__USE_CMOS_MN34041__)
	if 		(gain>=256*  1 && gain<256*2)	return 0x80;	// 0dB
	else									return 0xc0;	// 6dB

#elif defined(__USE_CMOS_MN34220__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/8.4375f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = senVal*60/640; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;

	if (senVal < 0x140) {
		return MAKEDWORD(0x100, MIN(0x100+senVal,0x240)); // 0x240 - 0x100 = 0x140= 320  320*2 = 640
	} else {
		senVal = senVal - 0x140;
		return MAKEDWORD(MIN(0x100+senVal,0x240), 0x240);
	}

#elif defined(__USE_CMOS_MN34420__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/8.4375f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = senVal*60/640; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;

	if (senVal < 0x140) {
		return MAKEDWORD(0x120, MIN(0x120+senVal,0x260)); // 0x240 - 0x100 = 0x140= 320  320*2 = 640
	} else {
		senVal = senVal - 0x140;
		return MAKEDWORD(MIN(0x120+senVal,0x260), 0x260);
	}

#elif defined(__USE_CMOS_IMX124__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/10.6f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = senVal/10; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;

	return (DWORD)MIN(senVal,GetCMOSGainOpt_num());

#elif defined(__USE_CMOS_IMX174__)  || defined(__USE_CMOS_IMX249__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/11.25f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = senVal/10; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;

	return (DWORD)MIN(senVal,GetCMOSGainOpt_num());

#elif defined(__USE_CMOS_IMX252__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/11.25f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = senVal/10; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;

	return (DWORD)MIN(senVal,GetCMOSGainOpt_num());

#elif defined(__USE_CMOS_IMX265__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/11.25f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = senVal/10; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;

	return (DWORD)MIN(senVal,GetCMOSGainOpt_num());

#elif defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__)
	#if defined(__USE_CMOS_IMX290__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/22.5f+0.5);
	#else
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/23.53f+0.5);
	#endif

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = senVal*3/10; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;

	return (DWORD)MIN(senVal,GetCMOSGainOpt_num());

#elif defined(__USE_CMOS_IMX385__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/7.505f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = senVal/10; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;

	//UARTprintf("senVal:%4d / ", senVal);
	return (DWORD)MIN(senVal,GetCMOSGainOpt_num());
#elif defined(__USE_CMOS_IMXx36__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/11.25f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = senVal/10; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;

	return (DWORD)MIN(senVal,GetCMOSGainOpt_num());

#elif defined(__USE_CMOS_IMXx22__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/38.57f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = senVal*42/140; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;

	return (DWORD)MIN(senVal,140);

#elif defined(__USE_CMOS_IMX323__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/36.12f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB(); // 8
	else					AE_GaindB = senVal*45/150; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;

	return (DWORD)MIN(senVal,150);

#elif defined(__USE_CMOS_IMX178__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/11.24f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = senVal/10; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;

	return (DWORD)MIN(senVal,GetCMOSGainOpt_num());
	
#elif defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX326__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/2.759f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = (WORD)(-20.0f*log10((2048.0f-(FLOAT)senVal)/2048.0f)); // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;

	return (DWORD)MAX(0,MIN(senVal,GetCMOSGainOpt_num()));

#elif defined(__USE_CMOS_IMX253__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/11.25f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = senVal/10; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;

	return (DWORD)MAX(0,MIN(senVal,GetCMOSGainOpt_num()));

#elif defined(__USE_CMOS_IMX185__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/33.75f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = senVal*3/10; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;

	return (DWORD)MIN(senVal,GetCMOSGainOpt_num());

#elif defined(__USE_CMOS_IMX334__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/22.5f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = senVal*3/10; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;

	return (DWORD)MIN(senVal,GetCMOSGainOpt_num());

#elif defined(__USE_CMOS_IMX273__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/11.25f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = senVal/10; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;

	return (DWORD)MAX(0,MIN(senVal,GetCMOSGainOpt_num()));

#elif defined(__USE_CMOS_AR0134__) || defined( __USE_CMOS_AR0135__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/21.2f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = senVal*3/10; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;
		
	return (DWORD)MAX(32,MIN(senVal,GetCMOSGainOpt_num()));

#elif defined(__USE_CMOS_EXT2__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/505.0f+2.5);

	return (DWORD)MIN(senVal,12);

#elif defined(__USE_CMOS_EXT1__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/23.166f+0.5);

	return (DWORD)MIN(senVal/10,13);

#elif defined(__USE_CMOS_OV2710__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/57.14f+0.5);

	return (DWORD)MIN(senVal,95);

#elif defined(__USE_CMOS_PS1400K__)
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/42.69f+0.5);

	if(gain >= 256*512)	AE_GaindB = GetCMOSGainMax_dB();
	else					AE_GaindB = senVal/10; // calculate dB

	if((AE_GaindB>=0) && (AE_GaindB < maxStdDevNum))	AE_StdDevMin = StdDevMin[(BYTE)AE_GaindB];
	else												AE_StdDevMin = 100;
	//UARTprintf("senVal : %4d\r\n",senVal);

	return (DWORD)MIN(senVal,GetCMOSGainOpt_num());

#endif
}

//--------------------------------------------------------------------------------------------------------------------------
// using CMOS Digital Gain
//--------------------------------------------------------------------------------------------------------------------------
#if defined(__USE_GAMMA_AE__)
WORD digitalGainX=0;
#endif

#if __MISP100_AE_DBG__ == 1
extern BOOL AE_GAIN_TEST;
extern BOOL AE_DGAIN_TEST;
#endif
DWORD AECTRL_CnvGAINtoFINE(DWORD gain)
{
#if __MISP100_AE_DBG__ == 1
	if(AE_DGAIN_TEST)
		return 256*2;
#endif

#if defined(__USE_CMOS_MN34041__)
	if 		(gain>=256*  1 && gain<256*2)	return (gain>> 0);	// 0dB
	else									return (gain>> 1);	// 6dB

#elif defined(__USE_CMOS_MN34220__) || defined(__USE_CMOS_MN34420__)
	WORD x = MAX(0,(LONG)(gain>>9)-256);	if (x>255) x = 255;

	if (gain<256* 512)
		return 256;
	else
		return (256 + x*8);

#elif defined(__USE_CMOS_IMXx36__) || defined(__USE_CMOS_IMX238__)
	return (DWORD)(256);

#elif defined(__USE_CMOS_IMX185__) || defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__) || \
	defined(__USE_CMOS_IMX274__)  || defined(__USE_CMOS_IMX326__) || defined(__USE_CMOS_IMX323__) || \
	defined(__USE_CMOS_IMX385__) || defined(__USE_CMOS_IMX253__)  || defined(__USE_CMOS_IMX273__) || \
	defined(__USE_CMOS_IMX334__)
	WORD x = MAX(0,(LONG)(gain>>9)-256);	if (x>255) x = 255; // x = 0 ~ 255
    #if defined(__USE_GAMMA_AE__)
	if(x != digitalGainX) {
		digitalGainX = x;
		SetBY_GammaGainValue();
	}
    #endif
	if (gain<256* 512) {
		//UARTprintf("dGain:%d\r\n",256);
		return 256;
	} else {
		//UARTprintf("dGain:%d\r\n",(256 + x*8));
		return (256 + x*8);
	}
#elif defined(__USE_CMOS_IMX226__)
	WORD x = MAX(0,(LONG)(gain>>9)-256);	if (x>255) x = 255; // x = 0 ~ 255
	x = (x*x) / 255;
    #if defined(__USE_GAMMA_AE__)
	if(x != digitalGainX) {
		digitalGainX = x;
		SetBY_GammaGainValue();
	}
    #endif
	if (gain<256* 512) {
		//UARTprintf("dGain:%d\r\n",256);
		return 256;
	} else {
		//UARTprintf("dGain:%d\r\n",(256 + x*12));
		return (256 + x*32);
	}
#elif defined(__USE_CMOS_PS1400K__)
	WORD x = MAX(0,(LONG)(gain>>9)-256);	if (x>255) x = 255; // x = 0 ~ 255
    #if defined(__USE_GAMMA_AE__)
	if(x != digitalGainX) {
		digitalGainX = x;
		SetBY_GammaGainValue();
	}
    #endif
	if (gain<256* 512)
		return 256;
	else
		return (256 + x*8);
#elif defined(__USE_CMOS_EXT1__)
  #if defined(__EXT_INNER_ANALOG_GAIN_ONLY__)
	return (DWORD)(256);
  #else
	WORD nID, x, senVal;

	if (gain<256*  2)		nID =  0;
	else if (gain<256*   4)	nID =  1;
	else if (gain<256*   8)	nID =  2;
	else if (gain<256* 16)	nID =  3;
	else if (gain<256* 32)	nID =  4;
	else if (gain<256* 64)	nID =  5;
	else if (gain<256*128)	nID =  6;
	else if (gain<256*256)	nID =  7;
	else if (gain<256*512)	nID =  8;
	else					nID =  9;

	x = MAX(0,(LONG)(gain>>nID)-256);	if (x>255) x = 255;
	senVal = (WORD)((FLOAT)(600*nID+AECTRL_GAIN_BASE[x])/23.166f+0.5);
    #if defined(__USE_DIGITAL_GAIN__)
	if(senVal >= 130) {
		return  (DWORD)(256 + 48*(senVal-130));
	} else {
		return (DWORD) (256+(senVal%10)*dGainInterval[MIN(senVal/10,12)]/10);
	}
    #else
	if(senVal >= 130) {
		return  (DWORD)(256);
	} else {
		return (DWORD) 256+(senVal%10)*10;
	}
    #endif
  #endif

#else

	return (DWORD)(256);

#endif
}

//--------------------------------------------------------------------------------------------------------------------------
LONG AECTRL_CnvCMOStoSHUT(DWORD shut)
{
#if defined(__USE_CMOS_MN34041__)
	return (LONG)(65536*(1123.00f-shut)/1123.00f);

#elif defined(__USE_CMOS_MN34220__)
	return (LONG)(65536*(1123.00f-shut)/1123.00f);

#elif defined(__USE_CMOS_MN34420__)
	return (LONG)(65536*(1123.00f-shut)/1123.00f);

#elif defined(__USE_CMOS_IMX124__)
	#if defined(__USE_IMX124_1536P30_MODE__)
	return (LONG)(65536*(1600.00f-shut)/1600.00f);
	#else
	return (LONG)(65536*(1125.00f-shut)/1125.00f);
	#endif

#elif defined(__USE_CMOS_IMXx36__)
	return (LONG)(65536*(1124.75f-shut)/1124.75f);
 	
#elif defined(__USE_CMOS_IMX174__) || defined(__USE_CMOS_IMX249__) || defined(__USE_CMOS_IMX185__)
	return (LONG)(65536*(1124.54f-shut)/1124.54f);

#elif defined(__USE_CMOS_IMX334__)
	return (LONG)(65536*((FLOAT)CMOS_GetVmax()-shut)/(FLOAT)CMOS_GetVmax());

#elif defined(__USE_CMOS_IMX273__)
	return (LONG)(65536*((FLOAT)CMOS_GetVmax()-shut)/(FLOAT)CMOS_GetVmax());

#elif defined(__USE_CMOS_IMX252__)
	return (LONG)(65536*(1124.54f-shut)/1124.54f);

#elif defined(__USE_CMOS_IMX265__)
	return (LONG)(65536*((FLOAT)CMOS_GetVmax()-shut)/(FLOAT)CMOS_GetVmax());
//	return (LONG)(65536*(1124.54f-shut)/1124.54f);

#elif (defined(__USE_CMOS_IMXx22__) || defined(__USE_CMOS_IMX323__))
	return (LONG)(65536*(1124.90f-shut)/1124.90f);

#elif defined(__USE_CMOS_EXT2__)
	return (LONG)(65536*(1023.90f-shut)/1023.90f);

#elif defined(__USE_CMOS_EXT1__)
	return (LONG)(65536*(1449.0f-shut)/1449.0f);

#elif defined(__USE_CMOS_IMX238__)
	return (LONG)(65536*(1124.54f-shut)/1124.54f);
		
#elif defined(__USE_CMOS_IMX290__)||defined(__USE_CMOS_IMX327__)
	#if defined(__USE_IMX290_720P120_MODE__) || defined(__USE_IMX290_720P60_MODE__) || defined(__USE_IMX290_720P30_MODE__)
	return (LONG)(65536*(750.0f-shut)/750.0f);
	#else
	return (LONG)(65536*(1124.0f-shut)/1124.0f);
	#endif

#elif defined(__USE_CMOS_IMX385__)
	return (LONG)(65536*(1124.0f-shut)/1124.0f);

#elif defined(__USE_CMOS_AR0134__) || defined( __USE_CMOS_AR0135__)
	return (LONG)(65536*(720.0f-shut)/720.0f);

#elif defined(__USE_CMOS_IMX178__)
	#if defined(__USE_IMX178_1080P60_MODE__) || defined(__USE_IMX178_1080P30_MODE__)
		return (LONG)(65536*(1650.0f-shut)/1650.0f);
	#else
		return (LONG)(65536*(1980.0f-shut)/1980.0f);
	#endif
		
#elif defined(__USE_CMOS_IMX226__)
	#if defined(__USE_IMX226_1080P60_MODE__)
//		return (LONG)(65536*(1124.0f-shut)/1124.0f);
		return (LONG)(65536*(2275.0f-shut)/2275.0f);
	#else
		return (LONG)(65536*(2200.0f-shut)/2200.0f);
	#endif

#elif defined(__USE_CMOS_IMX253__)
	return (LONG)(65536*((FLOAT)CMOS_GetVmax()-shut)/(FLOAT)CMOS_GetVmax());

#elif defined(__USE_CMOS_IMX274__)
	#if defined(__USE_IMX274_1080P60_MODE__)
		//return (LONG)(65536*(1124.0f-shut)/1124.0f);
		return (LONG)(65536*(2219.0f-shut)/2219.0f);
	#else
		return (LONG)(65536*(4500.0f-shut)/4500.0f);
	#endif

#elif defined(__USE_CMOS_IMX326__)
	#if defined(__USE_IMX326_1080P30_MODE__)
		return (LONG)(65536*(1124.0f-shut)/1124.0f);
	#else
		return (LONG)(65536*(4451.0f-shut)/4451.0f);
	#endif

#elif defined(__USE_CMOS_OV2710__)
	return (LONG)(65536*(1080.0f-shut)/1080.0f);

#elif defined(__USE_CMOS_PS1400K__)
	return (LONG)(65536*(1440.0f-shut)/1440.0f);

#endif
}

//--------------------------------------------------------------------------------------------------------------------------
DWORD AECTRL_CnvSHUTtoCMOS(LONG gain)
{
#if defined(__USE_CMOS_MN34041__)
	return (DWORD)(1123-(DWORD)(1123.00f*gain/65536+0.5));

#elif defined(__USE_CMOS_MN34220__)
	return (DWORD)(1123-(DWORD)(1123.00f*gain/65536+0.5));

#elif defined(__USE_CMOS_MN34420__)
	return (DWORD)(1123-(DWORD)(1123.00f*gain/65536+0.5));

#elif defined(__USE_CMOS_IMX124__)
	#if defined(__USE_IMX124_1536P30_MODE__)
	return (DWORD)(1600-(DWORD)(1600.00f*gain/65536+0.5));	
	#else
	return (DWORD)(1125-(DWORD)(1123.00f*gain/65536+0.5));
	#endif

#elif defined(__USE_CMOS_IMXx36__)
	return (DWORD)(1125-(DWORD)(1124.75f*gain/65536+0.5));
	
#elif defined(__USE_CMOS_IMX174__)  || defined(__USE_CMOS_IMX249__)
	return (DWORD)(1125-(DWORD)(1124.54f*gain/65536+0.5));

#elif defined(__USE_CMOS_IMX252__)
	return (DWORD)(1125-(DWORD)(1124.54f*gain/65536+0.5));

#elif defined(__USE_CMOS_IMX265__)
	return (DWORD)((FLOAT)CMOS_GetVmax()-(DWORD)((FLOAT)CMOS_GetVmax()*gain/65536+0.5));
//	return (DWORD)(1125-(DWORD)(1124.54f*gain/65536+0.5));	

#elif defined(__USE_CMOS_IMX185__)
	return (DWORD)(1125-(DWORD)(1124.54f*gain/65536+0.5));

#elif defined(__USE_CMOS_IMX334__)
	return (DWORD)((FLOAT)CMOS_GetVmax()-(DWORD)((FLOAT)CMOS_GetVmax()*gain/65536+0.5));

#elif defined(__USE_CMOS_IMX273__)
	return (DWORD)((FLOAT)CMOS_GetVmax()-(DWORD)((FLOAT)CMOS_GetVmax()*gain/65536+0.5));

#elif (defined(__USE_CMOS_IMXx22__) || defined(__USE_CMOS_IMX323__))
	return (DWORD)(1125-(DWORD)(1124.90f*gain/65536+0.5));
		
#elif defined(__USE_CMOS_IMX238__)
	return (DWORD)(1090-(DWORD)(1090.00f*gain/65536+0.5));
		
#elif defined(__USE_CMOS_IMX290__)||defined(__USE_CMOS_IMX327__)
	#if defined(__USE_IMX290_720P120_MODE__) || defined(__USE_IMX290_720P60_MODE__) || defined(__USE_IMX290_720P30_MODE__)
	return (DWORD)(750.0-(DWORD)(750.0f*gain/65536+0.5));
	#else
	return (DWORD)(1124.0-(DWORD)(1124.0f*gain/65536+0.5));
	#endif

#elif defined(__USE_CMOS_IMX385__)
	return (DWORD)(1124.0-(DWORD)(1124.0f*gain/65536+0.5));

#elif defined(__USE_CMOS_AR0134__) || defined( __USE_CMOS_AR0135__)
	return (DWORD)(720.0-(DWORD)(720.0f*gain/65536+0.5));

#elif defined(__USE_CMOS_EXT2__)
	return (DWORD)(1020-(DWORD)(1023.90f*gain/65536+0.5));

#elif defined(__USE_CMOS_EXT1__)
	return (DWORD)(1449-(DWORD)(1449.0f*gain/65536+0.5));
 
#elif defined(__USE_CMOS_IMX178__)
	#if defined(__USE_IMX178_1080P60_MODE__) || defined(__USE_IMX178_1080P30_MODE__)
	return (DWORD)(1650-(DWORD)(1650.00f*gain/65536+0.5));	
	#else
	return (DWORD)(1980-(DWORD)(1980.00f*gain/65536+0.5));
	#endif

#elif defined(__USE_CMOS_IMX226__)
	#if defined(__USE_IMX226_1080P60_MODE__)
//	return (DWORD)(1125-(DWORD)(1124.54f*gain/65536+0.5));
	return (DWORD)(2275-(DWORD)(2275.0f*gain/65536+0.5));
	#else
	return (DWORD)(2200-(DWORD)(2200.0f*gain/65536+0.5));		//data sheet 48page
	#endif

#elif defined(__USE_CMOS_IMX253__)
	return (DWORD)((FLOAT)CMOS_GetVmax()-(DWORD)((FLOAT)CMOS_GetVmax()*gain/65536+0.5));

#elif defined(__USE_CMOS_IMX274__)
	#if defined(__USE_IMX274_1080P60_MODE__)
	//return (DWORD)(1125-(DWORD)(1124.54f*gain/65536+0.5));
	return (DWORD)(2219-(DWORD)(2219.0f*gain/65536+0.5));
	#else
	return (DWORD)(4500-(DWORD)(4500.00f*gain/65536+0.5));		//data sheet 74,83 page
	#endif

#elif defined(__USE_CMOS_IMX326__)
	#if defined(__USE_IMX326_1080P60_MODE__)
	return (DWORD)(1125-(DWORD)(1124.54f*gain/65536+0.5));
	#else
	return (DWORD)(4451-(DWORD)(4451.00f*gain/65536+0.5));		//data sheet 74,83 page
	#endif

#elif defined(__USE_CMOS_OV2710__)
	return (DWORD)(1080-(DWORD)(1080.0f*gain/65536+0.5));

#elif defined(__USE_CMOS_PS1400K__)
	return (DWORD)(1440-(DWORD)(1440.0f*gain/65536+0.5));

#endif	
}

//--------------------------------------------------------------------------------------------------------------------------
LONG AECTRL_GetYREFVal(BYTE nID)
{
#if defined(__USE_CMOS_AR0134__INTERNAL_AE)
	return 32*nID;
#else
	return AECTRL_YREF_LEVEL[nID];
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
FLOAT AECTRL_CalAERatio(LONG mode,PAE_CTRL_PARAM pCTRL)
{
#if 0
	WORD tmpBuf;
	WORD tmpW, tmpH, tmpNW, tmpNH;
	FLOAT retF;

	LONG mode;
	WORD windW, windH;

	MISP_RegRead(MISP_LOCAL_ID, 0x60F, &tmpBuf);
	tmpW =(tmpBuf >> 8);
	tmpH =(tmpBuf&0xff);
	MISP_RegRead(MISP_LOCAL_ID, 0x681, &tmpBuf);
	tmpNW =(tmpBuf >> 8);
	tmpNH =(tmpBuf&0xff);

	//UARTprintf("W:%d H:%d NW:%d NH:%d(%d x %d)\r\n",tmpW,tmpH,tmpNW,tmpNH,(tmpW*tmpNW),(tmpH*tmpNH));

	mode = pCTRL->ctrl&AECTRL_WND_MASK;

	if(mode==AECTRL_BLC_WIND_BLC) {
		//windW = pCTRL->stHLC.nW*tmpW;
		//windH = pCTRL->stHLC.nH*tmpH;
		//retF = (FLOAT)(AE_WND_DEF_SIZE_W*ODM_WND_DEF_MUL_W*AE_WND_DEF_SIZE_H*ODM_WND_DEF_MUL_H) / (FLOAT)(tmpW*tmpH*tmpNH*tmpNW); // 840 x 480 / 1624x912
		//retF = (FLOAT)(1920*1080)/(windW*windH);
		retF = (FLOAT)(1920*1080)/(GetSS_image_active_size_h()*GetSS_image_active_size_v());
	} else if(mode==AECTRL_BLC_WIND_HLC) {
		//windW = pCTRL->stHLC.nW*tmpW;
		//windH = pCTRL->stHLC.nH*tmpH;
		retF = (FLOAT)(AE_WND_DEF_SIZE_W*ODM_WND_DEF_MUL_W*AE_WND_DEF_SIZE_H*ODM_WND_DEF_MUL_H) / (FLOAT)(tmpW*tmpH*tmpNH*tmpNW); // 840 x 480 / 1624x912
		//retF = (FLOAT)(1920*1080)/(windW*windH);
	} else {
		retF = (FLOAT)(AE_WND_DEF_SIZE_W*ODM_WND_DEF_MUL_W*AE_WND_DEF_SIZE_H*ODM_WND_DEF_MUL_H) / (FLOAT)(tmpW*tmpH*tmpNH*tmpNW); // 840 x 480 / 1624x912
	}

	//UARTprintf("AE Window Ratio : %f\r\n", retF);
#else
	FLOAT retF;
	WORD blk_w,blk_h;
	WORD temp;
	WORD windW, windH;

	MISP_RegRead(MISP_LOCAL_ID, 0x60F, &temp);
	blk_w = HIBYTE(temp);	blk_h = LOBYTE(temp);

	if(mode==AECTRL_BLC_MODE_BLC) {
		windW = pCTRL->stBLC.nW*blk_w;
		windH = pCTRL->stBLC.nH*blk_h;
	} else {
		windW = GetSS_image_active_size_h();
		windH = GetSS_image_active_size_v();
	}
	retF = (FLOAT)(1920*1080)/(windW*windH);
#endif

	return retF;
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_ViewBLCWIND(BOOL en)
{
	PAE_CTRL_PARAM pCTRL = &stAECTRL;
	
	pCTRL->blcwinden = en;
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_MoveBLCWIND(PAE_CTRL_PARAM pCTRL, BYTE x, BYTE y, BYTE w, BYTE h)
{
	LONG mode = pCTRL->ctrl&AECTRL_WND_MASK;

	if (mode==AECTRL_BLC_WIND_OFF) return;

	if (mode==AECTRL_BLC_WIND_BLC) {
		pCTRL->stBLC.sX = x;	pCTRL->stBLC.sY = y;
		pCTRL->stBLC.nW = w;	pCTRL->stBLC.nH = h;
	}
	else {
		pCTRL->stHLC.sX = x;	pCTRL->stHLC.sY = y;
		pCTRL->stHLC.nW = w;	pCTRL->stHLC.nH = h;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void BLC_HLC_Display(void)
{
	PAE_CTRL_PARAM pCTRL = &stAECTRL;
	WORD rODM[3], wVal[8];

	MISP_MultiRead(MISP_LOCAL_ID, 0x60d, (PBYTE)&rODM, 6); // odm_enwin_bgn_x~

	if((pCTRL->ctrl&AECTRL_BLC_MASK) == AECTRL_BLC_MODE_BLC)	{	// blc window
		wVal[0] = rODM[0] + pCTRL->stBLC.sX * HIBYTE(rODM[2]);		// rODM[0] : odm win sx
		wVal[1] = rODM[1] + pCTRL->stBLC.sY * LOBYTE(rODM[2]);		// rODM[1] : odm win sy
		wVal[2] = wVal[0] + pCTRL->stBLC.nW * HIBYTE(rODM[2]);		// rODM[2] : odm block w/h
		wVal[3] = wVal[1] + pCTRL->stBLC.nH * LOBYTE(rODM[2]);
	}
	else	{														// hlc window
		wVal[0] = rODM[0] + pCTRL->stHLC.sX * HIBYTE(rODM[2]);		// rODM[0] : odm win sx
		wVal[1] = rODM[1] + pCTRL->stHLC.sY * LOBYTE(rODM[2]);		// rODM[1] : odm win sy
		wVal[2] = wVal[0] + pCTRL->stHLC.nW * HIBYTE(rODM[2]);		// rODM[2] : odm block w/h
		wVal[3] = wVal[1] + pCTRL->stHLC.nH * LOBYTE(rODM[2]);
	}
	
	wVal[4] = wVal[0]+1;	wVal[5] = wVal[1]+1;
	wVal[6] = wVal[2]-1;	wVal[7] = wVal[3]-1;

	MISP_MultiWrite(MISP_LOCAL_ID, 0x753, (PBYTE)&wVal[0], 8);		// odm window #1 position
	MISP_RegField(MISP_LOCAL_ID, 0x752, 0, 1, pCTRL->blcwinden);	// odm window #1 en/disable
	MISP_MultiWrite(MISP_LOCAL_ID, 0x757, (PBYTE)&wVal[4], 8);		// odm window #2 position
	MISP_RegField(MISP_LOCAL_ID, 0x752, 2, 1, pCTRL->blcwinden);	// odm window #2 en/disable
}


//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_DirectSetTest()
{
#if defined(__USE_CMOS_IMX185__)		// use IMX185
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0214, GetCMOSGainMax_num()); // gain max
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0220, 0); // shutter min.
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0221, 0);
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0222, 0);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE sDGcnt = 0;
extern BOOL fDGen;
void AE_DigitalGainCtl(PAE_CTRL_PARAM pCTRL)
{
	WORD rVal[8];

	if(fDGen == TRUE) {
		if(sDGcnt == 0) {
			rVal[0] = 0;
			MISP_RegWrite(MISP_LOCAL_ID, 0x41F, 0);
			MISP_MultiRead(MISP_LOCAL_ID, 0x420, (PBYTE)rVal, 16);
			UARTprintf("%02x %02x %02x %02x %02x %02x %02x %02x\n",
				rVal[0], rVal[1], rVal[2], rVal[3], rVal[4], rVal[5], rVal[6], rVal[7]);
			
			rVal[0] = 1;
			MISP_RegWrite(MISP_LOCAL_ID, 0x41F, 1);
			MISP_MultiRead(MISP_LOCAL_ID, 0x420, (PBYTE)rVal, 16);
			UARTprintf("%02x %02x %02x %02x %02x %02x %02x %02x\n",
				rVal[0], rVal[1], rVal[2], rVal[3], rVal[4], rVal[5], rVal[6], rVal[7]);

			rVal[0] = 2;
			MISP_RegWrite(MISP_LOCAL_ID, 0x41F, 2);
			MISP_MultiRead(MISP_LOCAL_ID, 0x420, (PBYTE)rVal, 16);
			UARTprintf("%02x %02x %02x %02x %02x %02x %02x %02x\n",
				rVal[0], rVal[1], rVal[2], rVal[3], rVal[4], rVal[5], rVal[6], rVal[7]);

			rVal[0] = 3;
			MISP_RegWrite(MISP_LOCAL_ID, 0x41F, 3);
			MISP_MultiRead(MISP_LOCAL_ID, 0x420, (PBYTE)rVal, 16);
			UARTprintf("%02x %02x %02x %02x %02x %02x %02x %02x\n",
				rVal[0], rVal[1], rVal[2], rVal[3], rVal[4], rVal[5], rVal[6], rVal[7]);
			UARTprintf("\n");
		}

		if(sDGcnt == 60)
			sDGcnt = 0;
		else
			sDGcnt ++;
	}
}


#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
//--------------------------------------------------------------------------------------------------------------------------
void AE_IRIS_Full_Open(BOOL onoff)
{
	if(onoff) {
		AECTRL_SetAE_IRISFixMode();
		DCLENS_ForceIRISCtl(2);
	} else {
		AECTRL_SetAE_AutoMode();
		DCLENS_ForceIRISCtl(0);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void AE_IRIS_Full_Close(BOOL onoff)
{
	if(onoff) {
		AECTRL_SetAE_IRISFixMode();
		DCLENS_ForceIRISCtl(1);
	} else {
		AECTRL_SetAE_AutoMode();
		DCLENS_ForceIRISCtl(0);
	}
}

#endif



//--------------------------------------------------------------------------------------------------------------------------
extern BYTE global_histo64[64];
void Histogram_Read(void)
{
	static BYTE histFrameCnt = 0;
	
	int i;
	WORD sHist[4];
	WORD totalHist;
	FLOAT fHist[3];

	histFrameCnt--;
	if(histFrameCnt > 0) return;
	histFrameCnt = 40;

	//UARTprintf("histogram : ");

	sHist[0] = 0;
	for(i=0;i<16;i++) {
		//UARTprintf("%3d:",global_histo64[i]);
		sHist[0] += global_histo64[i];
	}
	//UARTprintf("%4d:",sHist[0]);

	sHist[1] = 0;
	for(i=16;i<32;i++) {
		//UARTprintf("%3d:",global_histo64[i]);
		sHist[1] += global_histo64[i];
	}
	//UARTprintf("%4d:",sHist[1]);

	sHist[2] = 0;
	for(i=32;i<48;i++) {
		//UARTprintf("%3d:",global_histo64[i]);
		sHist[2] += global_histo64[i];
	}
	//UARTprintf("%4d:",sHist[2]);

	sHist[3] = 0;
	for(i=48;i<64;i++) {
		//UARTprintf("%3d:",global_histo64[i]);
		sHist[3] += global_histo64[i];
	}
	//UARTprintf("%4d:",sHist[3]);

	totalHist = sHist[0]+sHist[1]+sHist[2]+sHist[3];
	//UARTprintf("-------%4d:",totalHist);

	fHist[0] = 2.0*(FLOAT)sHist[0] + (FLOAT)sHist[1];
	fHist[1] = 1.5*(FLOAT)sHist[1] + 1.5*(FLOAT)sHist[2];
	fHist[2] = (FLOAT)sHist[2] + 2.0*(FLOAT)sHist[3];
	//UARTprintf("%8.1f:",fHist[0]);
	//UARTprintf("%8.1f:",fHist[1]);
	//UARTprintf("%8.1f:",fHist[2]);
	//UARTprintf("\r\n");
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE AECTRL_GetGAINRatio(void) // for AWB
{
	//UARTprintf("Ydiff : %d\r\n", Get_AE_Ydiff());
	if (Get_AE_YDIFF() > 2000) return 1;
	else return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE AECTRL_GetSTDRatio(void) // for AWB
{
	WORD StDev_int, StDev_frac;
	WORD StDev;
	int divStd;

#if defined(__USE_4K60_MD1__) || defined(__USE_4K60_MD2__)
	GetNR_StDEV(&StDev_int, &StDev_frac, 1);
#else
	GetNR_StDEV(&StDev_int, &StDev_frac, 0);
#endif
	StDev = (WORD)((FLOAT)StDev_int*10.0f+(FLOAT)StDev_frac*1.25f);
	if(AE_StdDevMin < StDev)
		divStd = 8-2*(StDev -AE_StdDevMin);
	else
		divStd = 8;

	//UARTprintf("AE_StdDevMin : %d / StDev : %d / div : %d\r\n",AE_StdDevMin,StDev,MAX(divStd,1));
	return (BYTE)MAX(divStd,1);
}

//AFCTRL_GetZoomRatio() // for AWB

void AECTRL_SetTestMode(void)
{
	if(stAECTRL.testmode == 1)
		stAECTRL.testmode = 0;
	else
		stAECTRL.testmode++;
}
/*  FILE_END _HERE */
