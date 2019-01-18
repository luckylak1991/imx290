//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  OSDMENU.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__OSDMENU_H__
#define		__OSDMENU_H__

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

// menu text row, col
#define 	MenuTotalRow	14			// menu total row max
#define		MenuTitleRow	2			// menu title row max
#define		MenuTitleCol	26			// menu title text length
#define		MenuNameCol		14			// menu name text length
#define 	MenuItemCol		10			// menu item text length
#define		MenuMargin		1			// Menu margin from start&end.(Left:1_14_1, Right:1_10_1)

// menu attribute
#define		MENU_ITEM		0x01		//Item selection
#define		MENU_VALUE		0x02		//value adjust
#define		MENU_FUNC		0x04		//func call
#define		MENU_CTITLE		0x08		//cam title
#define		MENU_SUB		0x10		//enter sub menu
#define		MENU_SKIP		0x20		//skip menu
#define 	MENU_TEXT		0x40		//print text
#define		MENU_EXIT		0x80		//exit menu or return prev. menu
#define 	MENU_NULL		0x00		//blank

#define 	GetArrayMax(pArray)		sizeof(pArray)/sizeof(pArray[0])


// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

static ROMDATA char FirmwareVer[][MenuItemCol]		= {{"1.1.925095"}};
static ROMDATA char FirmwareDate[][MenuItemCol]		= {{"2019.01.04"}};

// define common menu & Item
static ROMDATA char RetMenu[][MenuNameCol]			= {{"RETURN        "}};
static ROMDATA char DefaultMenu[][MenuNameCol]		= {{"DEFAULT       "}};
static ROMDATA char SizePosition[][MenuNameCol]	= {{"SIZE/POSITION "}};
static ROMDATA char AreaMenu[][MenuNameCol]		= {{"AREA          "}};
static ROMDATA char AreaDisplay[][MenuNameCol]		= {{"AREA DISPLAY  "}};
static ROMDATA char LevelMenu[][MenuNameCol]		= {{"LEVEL         "}};
static ROMDATA char PositionMenu[][MenuNameCol]	= {{"POSITION      "}};
static ROMDATA char SizeMenu[][MenuNameCol]		= {{"SIZE          "}};

static ROMDATA char OnOffItem[][MenuItemCol]	= {"OFF       ",	"ON        "};
static ROMDATA char OnOffSub[][MenuItemCol]	= {"OFF       ",	"ON       >"};
static ROMDATA char WaitOkItem[][MenuItemCol]	= {"PRESS OK  ",	"WAIT...   "};
static ROMDATA char TestOkItem[][MenuItemCol]	= {"PRESS OK  ",	"TEST...   "};
static ROMDATA char SubItem[][MenuItemCol]		= {"         >"};
static ROMDATA char BlankItem[][MenuItemCol]	= {"          "};
static ROMDATA char SkipItem[][MenuItemCol] 	= {"----      "};
static ROMDATA char ColorItem[][MenuItemCol] 	= {
	"BLACK     ",	"BLUE      ",	"GRAY      ",	"YELLOW    ",
	"WHITE     ",	"BLACK OP  ",	"WHITE OP  ",	"NO COLOR  ",
};



//--------------------------------------------------------------------------------------------------------------------------
// 0. MAIN MENU  
//--------------------------------------------------------------------------------------------------------------------------

// main menu title
static ROMDATA char MainMenuTitle[][MenuTitleCol]	= { 
	"MAIN MENU                 ", "                          ", 
};

// main menu
static ROMDATA char FocusMenu[][MenuNameCol]		= {{"FOCUS         "}};
static ROMDATA char ExposureMenu[][MenuNameCol]	= {{"EXPOSURE      "}};
static ROMDATA char WBMenu[][MenuNameCol]			= {{"WHITE BALANCE "}};
static ROMDATA char DayNMenu[][MenuNameCol]		= {{"DAY & NIGHT   "}};
static ROMDATA char DNRMenu[][MenuNameCol]			= {{"DNR           "}};
static ROMDATA char PMMenu[][MenuNameCol]			= {{"PRIVACY       "}};
static ROMDATA char Effect[][MenuNameCol]			= {{"EFFECT        "}};
static ROMDATA char Special[][MenuNameCol]			= {{"SPECIAL       "}};
static ROMDATA char Setup[][MenuNameCol]			= {{"SETUP         "}};
static ROMDATA char EngMenu[][MenuNameCol]			= {{"ENG. MENU     "}};
static ROMDATA char ExitMenu[][MenuNameCol]		= {{"EXIT          "}};

// main menu info
static ROMDATA OSD_MENU_INFO MainMenuInfo[] = {
	//menu,					attb,			Item,				init,	max,					title
	{(PCHAR)FocusMenu,		MENU_SUB,		(PCHAR)SubItem,		0,		GetArrayMax(SubItem),	(PCHAR)MainMenuTitle	},
	{(PCHAR)ExposureMenu,	MENU_SUB,		(PCHAR)SubItem,		0,		GetArrayMax(SubItem),	(PCHAR)MainMenuTitle	},
	{(PCHAR)WBMenu,			MENU_SUB,		(PCHAR)SubItem,		0,		GetArrayMax(SubItem),	(PCHAR)MainMenuTitle	},
	{(PCHAR)DayNMenu,		MENU_SUB,		(PCHAR)SubItem,		0,		GetArrayMax(SubItem),	(PCHAR)MainMenuTitle	},
#if defined(__MDIN_i550_USB_V100__)
	{(PCHAR)DNRMenu,		MENU_SKIP,		(PCHAR)SubItem,		0,		GetArrayMax(SubItem),	(PCHAR)MainMenuTitle	},
	{(PCHAR)PMMenu,			MENU_SKIP,		(PCHAR)SubItem,		0,		GetArrayMax(SubItem),	(PCHAR)MainMenuTitle	},
#else
	{(PCHAR)DNRMenu,		MENU_SUB,		(PCHAR)SubItem,		0,		GetArrayMax(SubItem),	(PCHAR)MainMenuTitle	},
	{(PCHAR)PMMenu,			MENU_SUB,		(PCHAR)SubItem,		0,		GetArrayMax(SubItem),	(PCHAR)MainMenuTitle	},
#endif
	{(PCHAR)Effect,			MENU_SUB,		(PCHAR)SubItem,		0,		GetArrayMax(SubItem),	(PCHAR)MainMenuTitle	},
#if defined(__USE_CMOS_PS1400K__)	
	{(PCHAR)Special,		MENU_SKIP,		(PCHAR)SubItem,		0,		GetArrayMax(SubItem),	(PCHAR)MainMenuTitle	},
#else
	{(PCHAR)Special,		MENU_SUB,		(PCHAR)SubItem,		0,		GetArrayMax(SubItem),	(PCHAR)MainMenuTitle	},
#endif
	{(PCHAR)Setup,			MENU_SUB,		(PCHAR)SubItem,		0,		GetArrayMax(SubItem),	(PCHAR)MainMenuTitle	},
	{(PCHAR)EngMenu,		MENU_SUB,		(PCHAR)SubItem,		0,		GetArrayMax(SubItem),	(PCHAR)MainMenuTitle	},
	{(PCHAR)ExitMenu,		MENU_EXIT,		(PCHAR)BlankItem,	0,		GetArrayMax(BlankItem),	(PCHAR)MainMenuTitle	},
};

#if !defined(__USE_2MOTOR_LENS__)
//--------------------------------------------------------------------------------------------------------------------------
// 1-1.Focus Menu - normal
//--------------------------------------------------------------------------------------------------------------------------

// focus menu title
static ROMDATA char FocusMenuTitle[][MenuTitleCol]	= {
	"FOCUS                     ", "                          "
};

// focus menu
static ROMDATA char FocusMode[][MenuNameCol]		= {{"FOCUS MODE    "}};
static ROMDATA char ZoomStart[][MenuNameCol]		= {{"ZOOM START    "}};
static ROMDATA char ZoomStop[][MenuNameCol]		= {{"ZOOM STOP     "}};
static ROMDATA char ZoomSpeed[][MenuNameCol]		= {{"ZOOM SPEED    "}};
static ROMDATA char FocusLimit[][MenuNameCol]		= {{"FOCUS LIMIT   "}};
static ROMDATA char AFInterval[][MenuNameCol]		= {{"AF INTERVAL   "}};
static ROMDATA char ConferMenu[][MenuNameCol]		= {{"CONFERENCE    "}};

// focus menu item
static ROMDATA char FocusModeItem[][MenuItemCol] = {
	"AUTO      ",	"INTERVAL  ",	"ZOOM TRIG ",	"MANUAL    "
};

static ROMDATA char ZoomSpeedItem[][MenuItemCol] = {
	"SLOW      ",	"NORMAL    ",	"FAST      ",	"V.FAST    "
};

static ROMDATA char FocusLimitItem[][MenuItemCol] = {
#if defined(__USE_AFZOOM_LENS__)
#if defined(__USE_X30X_TAMRON__)
	"10CM      ",	"30CM      ",	"1M        ",	"1.5M      ",	"2M        ",
	"3M        ",	"6M        ",	"10M       ",	"INF       ",

#elif defined(__USE_X33X_CHINA__)
	"10CM      ",	"50CM      ",	"1M        ",	"1.5M      ",	"2M        ",
	"3M        ",	"5M        ",	"8M        ",	"20M       ",	"INF       ",

#elif defined(__USE_X10X_UNION__)
	"10CM      ",	"30CM      ",	"1M        ",	"1.5M      ",	"2M        ",
	"3M        ",	"6M        ",	"10M       ",	"INF       ",

#elif defined(__USE_X10X_ZMAX__)
	"50CM      ",	"1M        ",	"2M        ",	"4M        ",	"7M        ",
	"10M       ",	"15M       ",	"20M       ",	"INF       ",

#elif defined(__USE_X12X_UNION__)
	"50CM      ",	"1M        ",	"2M        ",	"4M        ",	"7M        ",
	"10M       ",	"15M       ",	"20M       ",	"INF       ",

#elif defined(__USE_X03X_ZMAX__)
	"50CM      ",	"1M        ",	"1.5M      ",	"2M        ",	"2.5M      ",
	"3M        ",	"5M        ",	"10M       ",	"INF       ",

#elif defined(__USE_YT3010_2MOTOR__)
	"50CM      ",	"1M        ",	"1.5M      ",	"2M        ",	"2.5M      ",
	"3M        ",	"5M        ",	"10M       ",	"INF       ",


#endif

#else
	"10CM      ",	"30CM      ",	"1M        ",	"1.5M      ",	"2M        ",
	"3M        ",	"6M        ",	"10M       ",	"INF       ",
#endif
};

#if defined(__USE_AFZOOM_LENS__)
#define		FOCUS1_ATTB				MENU_ITEM
#define		FOCUS2_ATTB				MENU_VALUE
#define		FOCUS3_ATTB				MENU_VALUE
#define		FOCUS4_ATTB				MENU_ITEM
#define		FOCUS5_ATTB				MENU_ITEM
#define		FOCUS6_ATTB				MENU_VALUE
#define		FOCUS7_ATTB				MENU_SUB
#else
#define		FOCUS1_ATTB				(MENU_ITEM | MENU_SKIP)
#define		FOCUS2_ATTB				(MENU_VALUE | MENU_SKIP)
#define		FOCUS3_ATTB				(MENU_VALUE | MENU_SKIP)
#define		FOCUS4_ATTB				(MENU_ITEM | MENU_SKIP)
#define		FOCUS5_ATTB				(MENU_ITEM | MENU_SKIP)
#define		FOCUS6_ATTB				(MENU_VALUE | MENU_SKIP)
#define		FOCUS7_ATTB				(MENU_SUB | MENU_SKIP)
#endif

// focus menu info
static ROMDATA OSD_MENU_INFO FocusMenuInfo[] = {
	// menu,			attb,			Item,					init,		max,						title
	{(PCHAR)FocusMode,	FOCUS1_ATTB,	(PCHAR)FocusModeItem,	0,			GetArrayMax(FocusModeItem),	(PCHAR)FocusMenuTitle	},
	{(PCHAR)ZoomStart,	FOCUS2_ATTB,	NULL,					0,			16,							(PCHAR)FocusMenuTitle	},
	{(PCHAR)ZoomStop,	FOCUS3_ATTB,	NULL,					0,			16,							(PCHAR)FocusMenuTitle	},
	{(PCHAR)ZoomSpeed,	FOCUS4_ATTB,	(PCHAR)ZoomSpeedItem,	3,			GetArrayMax(ZoomSpeedItem),	(PCHAR)FocusMenuTitle	},
	{(PCHAR)FocusLimit,	FOCUS5_ATTB,	(PCHAR)FocusLimitItem,	1,			GetArrayMax(FocusLimitItem),(PCHAR)FocusMenuTitle	},
	{(PCHAR)AFInterval,	FOCUS6_ATTB,	NULL,					5,			255,						(PCHAR)FocusMenuTitle	},
	{(PCHAR)ConferMenu,	FOCUS7_ATTB,	(PCHAR)SubItem,			0,			GetArrayMax(SubItem),		(PCHAR)FocusMenuTitle	},
	{(PCHAR)RetMenu,	MENU_EXIT,		(PCHAR)BlankItem,		0,			GetArrayMax(BlankItem),		(PCHAR)FocusMenuTitle	},
};
#else	//__USE_2MOTOR_LENS__
//--------------------------------------------------------------------------------------------------------------------------
// 1-1'.Focus Menu - two motor lens
//--------------------------------------------------------------------------------------------------------------------------

// two motor menu title
static ROMDATA char FocusMenuTitle[][MenuTitleCol]	= {
	"FOCUS - 2 MOTOR LENS      ", "                          "
};

// two motor menu
static ROMDATA char TwoMotorAfMode[][MenuNameCol]		= {{"AF MODE       "}};
static ROMDATA char TwoMotorScan[][MenuNameCol]			= {{"SCANNING      "}};
static ROMDATA char TwoMotorOnePush[][MenuNameCol]		= {{"ONE PUSH      "}};
static ROMDATA char TwoMotorSyncTDN[][MenuNameCol]		= {{"SYNC TDN      "}};
static ROMDATA char TwoMotorInitial[][MenuNameCol]		= {{"INITIAL       "}};

// two motor menu item
static ROMDATA char TwoMotorAfItem[][MenuItemCol] = {
	"AUTO      ",	"MANUAL    "
};

static ROMDATA char TwoMotorScanItem[][MenuItemCol] = {
	"HALF      ",	"FULL      "
};

// two motor menu info
static ROMDATA OSD_MENU_INFO FocusMenuInfo[] = {
	// menu,				attb,		Item,					init,	max,							title
	{(PCHAR)TwoMotorAfMode,	MENU_ITEM,	(PCHAR)TwoMotorAfItem,	0,		GetArrayMax(TwoMotorAfItem),	(PCHAR)FocusMenuTitle	},
	{(PCHAR)TwoMotorScan,	MENU_ITEM,	(PCHAR)TwoMotorScanItem,0,		GetArrayMax(TwoMotorScanItem),	(PCHAR)FocusMenuTitle	},
	{(PCHAR)TwoMotorOnePush,MENU_FUNC,	(PCHAR)WaitOkItem,		0,		GetArrayMax(WaitOkItem),		(PCHAR)FocusMenuTitle	},
	{(PCHAR)TwoMotorSyncTDN,MENU_ITEM,	(PCHAR)OnOffItem,		0,		GetArrayMax(OnOffItem),			(PCHAR)FocusMenuTitle	},
	{(PCHAR)TwoMotorInitial,MENU_FUNC,	(PCHAR)WaitOkItem,		0,		GetArrayMax(WaitOkItem),		(PCHAR)FocusMenuTitle	},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),			(PCHAR)FocusMenuTitle	},
};
#endif

//--------------------------------------------------------------------------------------------------------------------------
// 1-7-1. Focus - Conference Menu
//--------------------------------------------------------------------------------------------------------------------------

// conference menu title
static ROMDATA char ConferMenuTitle[][MenuTitleCol]	= {
	"CONFERENCE MODE           ", "                          "
};

// conference menu
static ROMDATA char ConferMode[][MenuNameCol]		= {{"MODE          "}};
static ROMDATA char ConferWidth[][MenuNameCol]		= {{" WINDOW WIDTH "}};
static ROMDATA char ConferHeight[][MenuNameCol]	= {{" WINDOW HEIGHT"}};
static ROMDATA char ConferXPos[][MenuNameCol]		= {{" WINDOW X-POS "}};
static ROMDATA char ConferYPos[][MenuNameCol]		= {{" WINDOW Y-POS "}};

// conference menu item

// conference menu info
static ROMDATA OSD_MENU_INFO ConferMenuInfo[] = {
	// menu,				attb,			Item,					init,		max,					title
	{(PCHAR)ConferMode,		MENU_ITEM,		(PCHAR)OnOffItem,		0,			GetArrayMax(OnOffItem),	(PCHAR)ConferMenuTitle	},
	{(PCHAR)ConferWidth,	MENU_VALUE,		NULL,					8,			32,						(PCHAR)ConferMenuTitle	},
	{(PCHAR)ConferHeight,	MENU_VALUE,		NULL,					6,			32,						(PCHAR)ConferMenuTitle	},
	{(PCHAR)ConferXPos,		MENU_VALUE,		NULL,					24,			64,						(PCHAR)ConferMenuTitle	},
	{(PCHAR)ConferYPos,		MENU_VALUE,		NULL,					16,			64,						(PCHAR)ConferMenuTitle	},
	{(PCHAR)RetMenu,		MENU_EXIT,		(PCHAR)BlankItem,		0,			GetArrayMax(BlankItem),	(PCHAR)ConferMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 2-1. Exposure Menu
//--------------------------------------------------------------------------------------------------------------------------

// exposure menu title
static ROMDATA char ExpMenuTitle[][MenuTitleCol]	= { 
	"EXPOSURE                  ", "                          "
};

// exposure menu
static ROMDATA char AEModeMenu[][MenuNameCol]		= {{"AE MODE       "}};
static ROMDATA char ExpShutMenu[][MenuNameCol]		= {{" SHUTTER      "}};
static ROMDATA char ExpIrisMenu[][MenuNameCol]		= {{" IRIS         "}};
static ROMDATA char ExpAGCMenu[][MenuNameCol]		= {{" AGC          "}};
static ROMDATA char ExpAGCMaxMenu[][MenuNameCol]	= {{" AGC MAX      "}};
static ROMDATA char SensUpMenu[][MenuNameCol]		= {{" SENS UP      "}};
static ROMDATA char ExpBrightMenu[][MenuNameCol]	= {{" AE BRIGHT    "}};
static ROMDATA char ExpLensMode[][MenuNameCol]		= {{"LENS MODE     "}};
static ROMDATA char ExpFlkMode[][MenuNameCol]		= {{"FLK CONTROL   "}};
static ROMDATA char ExpTrigStandbyMode[][MenuNameCol]		= {{"TRIG STANDBY  "}};

// exposure menu item
static ROMDATA char AEModeItem[][MenuItemCol]	= {
#if	defined(__USE_DCIRIS_HALL__) | defined(__USE_P_IRIS_ONLY__)
	"AUTO      ",	"SHUT FIX  ",	"IRIS FIX  ",	"AGC FIX   ",//	"BRIGHT    ",
#else
	"AUTO      ",	"SHUT FIX  ",	"AGC FIX   ",//	"BRIGHT    ",
#endif
	"MANUAL    ",
};

static ROMDATA char ExpShutItemPal[][MenuItemCol]	= {
	"AUTO      ",	"1/25      ",	"1/50      ",	"1/100     ",	"1/150     ",
	"1/200     ",	"1/250     ",	"1/500     ",	"1/1000    ",	"1/2000    ",
	"1/5000    ",	"1/10000   "
};

static ROMDATA char ExpShutItemNtsc[][MenuItemCol]	= {
#if defined(__USE_IMX274_4K15P_MODE__)
	"AUTO      ",	"1/15      ",	"1/30      ",	"1/60      ",	"1/90      ",
	"1/120     ",	"1/150     ",	"1/250     ",	"1/500     ",	"1/1000    ",
	"1/2500    ",	"1/5000    "
#else
	"AUTO      ",	"1/30      ",	"1/60      ",	"1/120     ",	"1/180     ",
	"1/240     ",	"1/300     ",	"1/500     ",	"1/1000    ",	"1/2000    ",
	"1/5000    ",	"1/10000   "
#endif
};

#if defined(__USE_X12X_UNION__)
static ROMDATA char ExpIrisItem[][MenuItemCol]		= {
	"F16       ",	"F11       ",	"F8        ",	"F5.6      ",	"F4        ",
	"F2.8      ",	"F2        ",	"F1.8      ",
};
#else
static ROMDATA char ExpIrisItem[][MenuItemCol]		= {
	"F16       ",	"F11       ",	"F8        ",	"F5.6      ",	"F4        ",
	"F2.8      ",	"F2        ",	"F1.6      ",
};
#endif

#if defined(__USE_CMOS_MN34220__) || defined(__USE_CMOS_MN34420__)
	static ROMDATA char ExpAGCItem[][MenuItemCol]={
		"AUTO      ",	"0dB       ",	"3dB       ",	"6dB       ",	"9dB       ",	"12dB      ",	"15dB      ",	"18dB      ",	"21dB      ",
		"24dB      ",	"27dB     ",	"d30dB     ",	"d33dB     ",	"d36dB     ",	"d39dB     ",	"d42dB     ",	"d45dB     ",	"d48dB     ",
		"d51dB     ",	"d54dB    ",	"d57dB     ",	"d60dB     ",
		"d2X       ",	"d3X       ",	"d4X       ",	"d5X       ",	"d6X       ",	"d7X       ",	"d8X       ",	"d9X       "};
#elif	defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__)
	static ROMDATA char ExpAGCItem[][MenuItemCol]={
		"AUTO      ",	"0dB       ",	"2dB       ",	"4dB       ",	"6dB       ",	"8dB       ",	"10dB      ",	"12dB      ",
		"14dB      ",	"16dB      ",	"18dB      ",	"20dB      ",	"22dB      ",	"24dB      ",	"26dB      ",	"27dB      ",
		"d2X       ",	"d3X       ",	"d4X       ",	"d5X       ",	"d6X       ",	"d7X       ",	"d8X       ",	"d9X       "};
#elif	defined(__USE_CMOS_IMX226__)
	static ROMDATA char ExpAGCItem[][MenuItemCol]={
		"AUTO      ",	"0dB       ",	"2dB       ",	"4dB       ",	"6dB       ",	"8dB       ",	"10dB      ",	"12dB      ",
		"14dB      ",	"16dB      ",	"18dB      ",	"20dB      ",	"22dB      ",	"24dB      ",	"26dB      ",	"27dB      ",
		"d3X       ",	"d5X       ",	"d7X       ",	"d9X       ",	"d11X      ",	"d13X      ",	"d15X      ",	"d17X      ",
		"d19X      ",	"d21X      ",	"d23X      ",	"d25X      ",	"d27X      ",	"d29X      ",	"d31X      ",	"d33X      "};
#elif defined(__USE_CMOS_IMX124__)
	static ROMDATA char ExpAGCItem[][MenuItemCol]={
		"AUTO      ",	"0dB       ",	"3dB       ",	"7dB       ",	"10dB      ",	"14dB      ",	"17dB      ",	"21dB      ",
		"24dB      ",	"d28dB     ",	"d31dB     ",	"d35dB     ",	"d38dB     ",	"d42dB     ",	"d45dB     ",	"d51dB     "};
#elif defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX334__)
	static ROMDATA char ExpAGCItem[][MenuItemCol]={
		"AUTO      ",	"0dB       ",	"5dB       ",	"10dB      ",	"15dB      ",	"20dB      ",	"25dB      ",	"30dB      ",
		"d35dB     ",	"d40dB     ",	"d45dB     ",	"d50dB     ",	"d55dB     ",	"d60dB     ",	"d65dB     ",	"d72dB     ",
		"d2X       ",	"d3X       ",	"d4X       ",	"d5X       ",	"d6X       ",	"d7X       ",	"d8X       ",	"d9X       "};
#elif defined(__USE_CMOS_IMX327__)
	static ROMDATA char ExpAGCItem[][MenuItemCol]={
		"AUTO      ",	"0dB       ",	"5dB       ",	"10dB      ",	"15dB      ",	"20dB      ",	"25dB      ",	"30dB      ",
		"d35dB     ",	"d40dB     ",	"d45dB     ",	"d50dB     ",	"d55dB     ",	"d60dB     ",	"d65dB     ",	"d69dB     ",
		"d2X       ",	"d3X       ",	"d4X       ",	"d5X       ",	"d6X       ",	"d7X       ",	"d8X       ",	"d9X       "};
#elif defined(__USE_CMOS_IMX385__)
	static ROMDATA char ExpAGCItem[][MenuItemCol]={
		"AUTO      ",	"0dB       ",	"5dB       ",	"10dB      ",	"15dB      ",	"20dB      ",	"25dB      ",	"30dB      ",
		"d35dB     ",	"d40dB     ",	"d45dB     ",	"d50dB     ",	"d55dB     ",	"d60dB     ",	"d65dB     ",	"d72dB     ",
		"d2X       ",	"d3X       ",	"d4X       ",	"d5X       ",	"d6X       ",	"d7X       ",	"d8X       ",	"d9X       "};
#elif defined(__USE_CMOS_IMXx36__)
	static ROMDATA char ExpAGCItem[][MenuItemCol]={
		"AUTO      ",	"0dB       ",	"3dB       ",	"6dB       ",	"9dB       ",	"12dB      ",	"15dB      ",	"18dB      ",
		"21dB      ",	"24dB      ",	"d27dB     ",	"d30dB     ",	"d33dB     ",	"d36dB     ",	"d39dB     ",	"d42dB     "};
#elif defined(__USE_CMOS_IMX185__) || defined(__USE_CMOS_IMX273__)
	static ROMDATA char ExpAGCItem[][MenuItemCol]={
		"AUTO      ",	"0dB       ",	"3dB       ",	"6dB       ",	"9dB       ",	"12dB      ",	"15dB      ",	"18dB      ",	"21dB      ",
		"24dB      ",	"d27dB     ",	"d30dB     ",	"d33dB     ",	"d36dB     ",	"d39dB     ",	"d42dB     ",	"d45dB     ",	"d48dB     ",
		"d2X       ",	"d3X       ",	"d4X       ",	"d5X       ",	"d6X       ",	"d7X       ",	"d8X       ",	"d9X       "};
#elif defined(__USE_CMOS_IMX253__) 
	static ROMDATA char ExpAGCItem[][MenuItemCol]={
		"AUTO      ",	"0dB       ",	"3dB       ",	"6dB       ",	"9dB       ",	"12dB      ",	"15dB      ",	"18dB      ",	"21dB      ",
		"24dB      ",	"d27dB     ",	"d30dB     ",	"d33dB     ",	"d36dB     ",	"d39dB     ",	"d42dB     ",	"d45dB     ",	"d48dB     ",
		"d2X       ",	"d3X       ",	"d4X       ",	"d5X       ",	"d6X       ",	"d7X       ",	"d8X       ",	"d9X       "};
#elif defined(__USE_CMOS_IMX273__) 
	static ROMDATA char ExpAGCItem[][MenuItemCol]={
		"AUTO      ",	"0dB       ",	"3dB       ",	"6dB       ",	"9dB       ",	"12dB      ",	"15dB      ",	"18dB      ",	"21dB      ",
		"24dB      ",	"d27dB     ",	"d30dB     ",	"d33dB     ",	"d36dB     ",	"d39dB     ",	"d42dB     ",	"d45dB     ",	"d48dB     ",
		"d2X       ",	"d3X       ",	"d4X       ",	"d5X       ",	"d6X       ",	"d7X       ",	"d8X       ",	"d9X       "};		
#elif defined(__USE_CMOS_IMX323__)
	static ROMDATA char ExpAGCItem[][MenuItemCol]={
		"AUTO      ",	"0dB       ",	"3dB       ",	"6dB       ",	"9dB       ",	"12dB      ",	"15dB      ",	"18dB      ",	"21dB      ",
		"d24dB      ",	"d27dB     ",	"d30dB     ",	"d33dB     ",	"d36dB     ",	"d39dB     ",	"d42dB     ",	"d45dB     ",
		"d2X       ",	"d3X       ",	"d4X       ",	"d5X       ",	"d6X       ",	"d7X       ",	"d8X       ",	"d9X       "};
#elif defined(__USE_CMOS_IMX174__) || defined(__USE_CMOS_IMX249__) || defined(__USE_CMOS_IMX265__) || defined(__USE_CMOS_IMX252__)
static ROMDATA char ExpAGCItem[][MenuItemCol]		= {	
	"AUTO      ",	"0dB       ",	"2dB       ",	"4dB       ",	"6dB       ",	"8dB       ",	"10dB      ",	"12dB      ",
	"14dB      ",	"16dB      ",	"18dB      ",	"20dB      ",	"22dB      ",	"24dB      ",	"d26dB     ",	"d28dB     ",
	"d30dB     ",	"d33dB     ",	"d36dB     ",	"d39dB     ",	"d42dB     ",	"d45dB     ",	"d48dB     "
};	
#else
static ROMDATA char ExpAGCItem[][MenuItemCol]		= {
	"AUTO      ",	"0dB       ",	"2dB       ",	"4dB       ",	"6dB       ",	"8dB       ",	"10dB      ",	"12dB      ",
	"14dB      ",	"16dB      ",	"18dB      ",	"20dB      ",	"22dB      ",	"24dB      ",	"26dB      ",	"28dB      ",
};
#endif

static ROMDATA char ExpAGCMaxItem[][MenuItemCol]		= {
	"OFF       ",	"LOW       ",	"MIDDLE    ",	"HIGH      ",	"DMIN      ",	"DMID      ",	"DMAX      ",
};

#if defined (__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__) \
	|| defined(__USE_CMOS_IMX385__) || defined(__USE_CMOS_IMX253__) || defined(__USE_CMOS_IMX265__)
static ROMDATA char SensUpItem[][MenuItemCol]		= {
	"OFF       ",	"x2        ",	"x3        ",	"x4        ",
	"x5        ",	"x6        ",	"x7        ",	"x8        ",
	"x9        ",	"x10       ",	"x12       ",	"x15       ",
};
#elif defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX273__)
static ROMDATA char SensUpItem[][MenuItemCol]		= {
	"OFF       ",	"x2        ",	"x3        ",	"x4        ",
	"x5        ",	"x6        ",	"x7        ",	"x8        ",
	"x9        ",	"x10       ",	"x12       ",
};
#else
static ROMDATA char SensUpItem[][MenuItemCol]		= {
	"OFF       ",	"x2        ",	"x3        ",	"x4        ",
	"x5        ",	"x6        ",	"x7        ",	"x8        ",
	"x9        ",	"x10       ",	"x20       ",	"x30       ",
};
#endif

static ROMDATA char ExpLensModeItem[][MenuItemCol]		= {
	"INDOOR    ",	"OUTDOOR   ",		"MANUAL    ",
};

static ROMDATA char ExpFlkModeItem[][MenuItemCol]		= {
	"AUTO      ",	"ON        ",	"OFF       ",
};

static ROMDATA char ExpTrigStandbyModeItem[][MenuItemCol]		= {
	"NORMAL    ",	"TRIG-STBY "
};

#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
#define 	MENU_ATTB_IRIS			MENU_ITEM
#else
#define		MENU_ATTB_IRIS			(MENU_ITEM | MENU_SKIP)
#endif

// exposure menu info
static ROMDATA OSD_MENU_INFO ExpMenuInfo[] = {
	// menu,				attb,		Item,					init,	max,						title
	{(PCHAR)AEModeMenu,		MENU_ITEM,	(PCHAR)AEModeItem,		0,		GetArrayMax(AEModeItem),	(PCHAR)ExpMenuTitle	},
#if defined(__USE_25Hz_SENSOR__)
	{(PCHAR)ExpShutMenu,	MENU_ITEM,	(PCHAR)ExpShutItemPal,	0,		GetArrayMax(ExpShutItemPal),	(PCHAR)ExpMenuTitle	},
#else
	{(PCHAR)ExpShutMenu,	MENU_ITEM,	(PCHAR)ExpShutItemNtsc,	0,		GetArrayMax(ExpShutItemNtsc),	(PCHAR)ExpMenuTitle	},
#endif
	{(PCHAR)ExpIrisMenu,	MENU_ATTB_IRIS,	(PCHAR)ExpIrisItem,	7,		GetArrayMax(ExpIrisItem),	(PCHAR)ExpMenuTitle	},
	{(PCHAR)ExpAGCMenu,		MENU_ITEM,	(PCHAR)ExpAGCItem,		0,		GetArrayMax(ExpAGCItem),	(PCHAR)ExpMenuTitle	},
	{(PCHAR)ExpAGCMaxMenu,	MENU_ITEM,	(PCHAR)ExpAGCMaxItem,	3,		GetArrayMax(ExpAGCMaxItem),	(PCHAR)ExpMenuTitle	},
	{(PCHAR)SensUpMenu,		MENU_ITEM,	(PCHAR)SensUpItem,		0,		GetArrayMax(SensUpItem),	(PCHAR)ExpMenuTitle	},
	{(PCHAR)ExpBrightMenu,	MENU_VALUE,	NULL,					6,		10,							(PCHAR)ExpMenuTitle	}, // brightness max
	{(PCHAR)ExpFlkMode,		MENU_ITEM,	(PCHAR)ExpFlkModeItem,	0,		GetArrayMax(ExpFlkModeItem),(PCHAR)ExpMenuTitle	},
#if defined(__USE_PWMIRIS_ONLY__)
	{(PCHAR)ExpLensMode,	MENU_ITEM,	(PCHAR)ExpLensModeItem,	0,		GetArrayMax(ExpLensModeItem),(PCHAR)ExpMenuTitle},
#endif
#if	defined(__USE_CMOS_IMX265__) && !defined(__USE_IMX265_SLAVE_MODE__)
	{(PCHAR)ExpTrigStandbyMode,	MENU_ITEM,	(PCHAR)ExpTrigStandbyModeItem,	0,		GetArrayMax(ExpTrigStandbyModeItem),(PCHAR)ExpMenuTitle},
#endif
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),		(PCHAR)ExpMenuTitle	},
};


//--------------------------------------------------------------------------------------------------------------------------
// 3-1.White Balance
//--------------------------------------------------------------------------------------------------------------------------

// WB menu title
static ROMDATA char WBMenuTitle[][MenuTitleCol]	= {
	"WHITE BALANCE             ", "                          "
};

// WB menu
static ROMDATA char WBModeMenu[][MenuNameCol]		= {{"MODE          "}};
static ROMDATA char WBPushMenu[][MenuNameCol]		= {{"PUSH          "}};
static ROMDATA char WBBlueMenu[][MenuNameCol]		= {{"BLUE          "}};
static ROMDATA char WBRedMenu[][MenuNameCol]		= {{"RED           "}};
static ROMDATA char WBSpeedMenu[][MenuNameCol]		= {{"SPEED         "}};
static ROMDATA char WBOffBMenu[][MenuNameCol]		= {{"OFFSET-B      "}};
static ROMDATA char WBOffRMenu[][MenuNameCol]		= {{"OFFSET-R      "}};

// WB menu item
static ROMDATA char WBModeItem[][MenuItemCol] = {
	"ATW       ",	"ONE PUSH  ",	"INDOOR    ",	"OUTDOOR   ",//	"PRESET    ",
	"MANUAL    ",	
};

// WB menu info
static ROMDATA OSD_MENU_INFO WBMenuInfo[] = {
	// menu,			attb,		Item,				init,	max,						title
	{(PCHAR)WBModeMenu,	MENU_ITEM,	(PCHAR)WBModeItem,	0,		GetArrayMax(WBModeItem),	(PCHAR)WBMenuTitle	},
	{(PCHAR)WBPushMenu,	MENU_FUNC,	(PCHAR)WaitOkItem,	0,		GetArrayMax(WaitOkItem),	(PCHAR)WBMenuTitle	},
	{(PCHAR)WBBlueMenu,	MENU_VALUE,	NULL,				50,		100,						(PCHAR)WBMenuTitle	},
	{(PCHAR)WBRedMenu,	MENU_VALUE,	NULL,				50,		100,						(PCHAR)WBMenuTitle	},
	{(PCHAR)WBSpeedMenu,MENU_VALUE,	NULL,				6,		7,							(PCHAR)WBMenuTitle	},
	{(PCHAR)WBOffBMenu,	MENU_VALUE,	NULL,				50,		100,						(PCHAR)WBMenuTitle	},
	{(PCHAR)WBOffRMenu,	MENU_VALUE,	NULL,				50,		100,						(PCHAR)WBMenuTitle	},
	{(PCHAR)RetMenu,	MENU_EXIT,	(PCHAR)BlankItem,	0,		GetArrayMax(BlankItem),		(PCHAR)WBMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 4-1. Day & Night
//--------------------------------------------------------------------------------------------------------------------------

// daynight menu title
static ROMDATA char DYNTMenuTitle[][MenuTitleCol]	= {
	"DAY & NIGHT               ", "                          "
};

// daynight menu
static ROMDATA char DYNTModeMenu[][MenuNameCol]	= {{"MODE          "}};
static ROMDATA char DYNTDelayMenu[][MenuNameCol]	= {{"DELAY[SEC]    "}};
static ROMDATA char AGCD2NMenu[][MenuNameCol]		= {{"D->N LEVEL    "}};
static ROMDATA char AGCN2DMenu[][MenuNameCol]		= {{"N->D LEVEL    "}};
static ROMDATA char NColorMenu[][MenuNameCol]		= {{"NIGHT COLOR   "}};
static ROMDATA char ColorBurstMenu[][MenuNameCol]	= {{"COLOR BURST   "}};
static ROMDATA char SaveMenu[][MenuNameCol]			= {{"SAVE          "}};

// daynight menu item
static ROMDATA char DYNTModeItem[][MenuItemCol] = {
	"AUTO      ",	"DAY(COLOR)",	"NIGHT(B/W)",	"EXT       "
};

// daynight menu info
static ROMDATA OSD_MENU_INFO DYNTMenuInfo[] = {
	// menu,				attb,		Item,					init,	max,						title
	{(PCHAR)DYNTModeMenu,	MENU_ITEM,	(PCHAR)DYNTModeItem,	0,		GetArrayMax(DYNTModeItem),	(PCHAR)DYNTMenuTitle	},
	{(PCHAR)DYNTDelayMenu,	MENU_VALUE,	NULL,					5,		60,							(PCHAR)DYNTMenuTitle	},
	{(PCHAR)AGCD2NMenu,		MENU_VALUE,	NULL,					165,	255,						(PCHAR)DYNTMenuTitle	},
	{(PCHAR)AGCN2DMenu,		MENU_VALUE,	NULL,					65,		254,						(PCHAR)DYNTMenuTitle	},
	{(PCHAR)NColorMenu,		MENU_ITEM,	(PCHAR)OnOffItem,		0,		GetArrayMax(OnOffItem),		(PCHAR)DYNTMenuTitle	},
	{(PCHAR)ColorBurstMenu,	MENU_ITEM,	(PCHAR)OnOffItem,		0,		GetArrayMax(OnOffItem),		(PCHAR)DYNTMenuTitle	},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),		(PCHAR)DYNTMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
//  5-1. Noise Reduction
//--------------------------------------------------------------------------------------------------------------------------

// dnr menu title
static ROMDATA char DNRMenuTitle[][MenuTitleCol]	= {
	"NOISE REDUCTION           ", "                          "
};

// dnr menu
static ROMDATA char DNRModeMenu[][MenuNameCol]			= {{"DNR MODE      "}};
static ROMDATA char DNRLevelMenu[][MenuNameCol]		= {{"DNR LEVEL     "}};
static ROMDATA char DNRApertureMenu[][MenuNameCol]		= {{"DNR APERTURE  "}};

// dnr menu item
static ROMDATA char DNRModeItem[][MenuItemCol] = {
	"OFF       ",	"2D        ",	"3D        ",	"2D+3D     "
};

static ROMDATA char DNRLevelItem[][MenuItemCol]	= {
	"AUTO      ",	"LOW       ",	"MIDDLE    ",	"HIGH      "
};

static ROMDATA char DNRApertureItem[][MenuItemCol]	= {
	"0         ",	"1         ",	"2         ",	"3         ",	"4         "
};

// dnr menu info
static ROMDATA OSD_MENU_INFO DNRMenuInfo[] = {
	// menu,				attb,		Item,					init,	max,						title
	{(PCHAR)DNRModeMenu,	MENU_ITEM,	(PCHAR)DNRModeItem,		3,		GetArrayMax(DNRModeItem),	(PCHAR)DNRMenuTitle	},
	{(PCHAR)DNRLevelMenu,	MENU_ITEM,	(PCHAR)DNRLevelItem,	0,		GetArrayMax(DNRLevelItem),	(PCHAR)DNRMenuTitle	},
	{(PCHAR)DNRApertureMenu,MENU_ITEM,	(PCHAR)DNRApertureItem,	2,		GetArrayMax(DNRApertureItem),(PCHAR)DNRMenuTitle	},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),		(PCHAR)DNRMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 6-1. Privacy Mask
//--------------------------------------------------------------------------------------------------------------------------

// pm menu title
static ROMDATA char PMMenuTitle[][MenuTitleCol]	= {
	"PRIVACY MASK              ", "                          "
};

// pm menu
static ROMDATA char PMModeMenu[][MenuNameCol]			= {{"MODE          "}};
static ROMDATA char PMTypeMenu[][MenuNameCol]			= {{"TYPE          "}};
static ROMDATA char PMMaskNoMenu[][MenuNameCol]		= {{"MASK NO.      "}};
static ROMDATA char PMDisplayMenu[][MenuNameCol]		= {{" DISPLAY      "}};
static ROMDATA char ColorMenu[][MenuNameCol]			= {{" COLOR        "}};
static ROMDATA char PolySelect[][MenuNameCol]			= {{" POLY SELECT  "}};
static ROMDATA char PMPosiXMenu[][MenuNameCol]			= {{" POSITION-X   "}};
static ROMDATA char PMPosiYMenu[][MenuNameCol]			= {{" POSITION-Y   "}};
static ROMDATA char PMSizeXMenu[][MenuNameCol]			= {{" SIZE-X       "}};
static ROMDATA char PMSizeYMenu[][MenuNameCol]			= {{" SIZE-Y       "}};

// pm menu item
static ROMDATA char PMTypeItem[][MenuItemCol]	= {
	"SQUARE    ",	"POLYGON   "
};

static ROMDATA char PMMaskNoItem[][MenuItemCol]	= {
	"MASK 1    ",	"MASK 2    ",	"MASK 3    ",	"MASK 4    ", 
	"MASK 5    ",	"MASK 6    ",	"MASK 7    ",	"MASK 8    "
};

static ROMDATA char PMPolyPosiItem[][MenuItemCol] = {
	"L-TOP     ",	"R-TOP     ",	"L-BOTTOM  ",	"R-BOTTOM  ",

};

static ROMDATA char PMColorItem[][MenuItemCol] = {
	"BLACK     ",	"WHITE     ",	"GREEN     ",	"BLUE      ",
	"RED       ",	"CYAN      ",	"MAGENTA   ",	"YELLOW    ",
	"GRAY-1    ",	"GRAY-2    ",	"GRAY-3    ",	"GRAY-4    ",
	"GRAY-5    ",	"GRAY-6    ",	"MOSAIC    ",
};

// pm menu info
static ROMDATA OSD_MENU_INFO PMMenuInfo[] = {
	// menu,				attb,		Item,					init,	max,						title
	{(PCHAR)PMModeMenu,		MENU_ITEM,	(PCHAR)OnOffItem,		0,		GetArrayMax(OnOffItem),		(PCHAR)PMMenuTitle	},
	{(PCHAR)PMTypeMenu,		MENU_ITEM,	(PCHAR)PMTypeItem,		0,		GetArrayMax(PMTypeItem),	(PCHAR)PMMenuTitle	},
	{(PCHAR)PMMaskNoMenu,	MENU_ITEM,	(PCHAR)PMMaskNoItem,	0,		GetArrayMax(PMMaskNoItem),	(PCHAR)PMMenuTitle	},
	{(PCHAR)PMDisplayMenu,	MENU_ITEM,	(PCHAR)OnOffItem,		1,		GetArrayMax(OnOffItem),		(PCHAR)PMMenuTitle	},
	{(PCHAR)ColorMenu,		MENU_ITEM,	(PCHAR)PMColorItem,		0,		GetArrayMax(PMColorItem),	(PCHAR)PMMenuTitle	},
	{(PCHAR)PolySelect,		MENU_ITEM,	(PCHAR)PMPolyPosiItem,	0,		GetArrayMax(PMPolyPosiItem),(PCHAR)PMMenuTitle	},
	{(PCHAR)PMPosiXMenu,	MENU_VALUE,	NULL,					0,		255,						(PCHAR)PMMenuTitle	},
	{(PCHAR)PMPosiYMenu,	MENU_VALUE,	NULL,					1,		216,						(PCHAR)PMMenuTitle	},
	{(PCHAR)PMSizeXMenu,	MENU_VALUE,	NULL,					0,		120,							(PCHAR)PMMenuTitle	},
	{(PCHAR)PMSizeYMenu,	MENU_VALUE,	NULL,					0,		68,							(PCHAR)PMMenuTitle	},
	{(PCHAR)DefaultMenu,	MENU_FUNC,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),		(PCHAR)PMMenuTitle	},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),		(PCHAR)PMMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 7-1.Effect
//--------------------------------------------------------------------------------------------------------------------------

// effect menu title
static ROMDATA char EffectMenuTitle[][MenuTitleCol]	= {
	"EFFECT                    ", "                          "
};

// effect menu
static ROMDATA char LensShading[][MenuNameCol]			= {{"LENS SHADING  "}};
static ROMDATA char GMCorrect[][MenuNameCol]			= {{"GAMMA CORRECT "}};
static ROMDATA char ContrastMenu[][MenuNameCol]		= {{"CONTRAST      "}};
static ROMDATA char ColBrightMenu[][MenuNameCol]		= {{"BRIGHTNESS    "}};
static ROMDATA char SaturationMenu[][MenuNameCol]		= {{"SATURATION    "}};
static ROMDATA char HueMenu[][MenuNameCol]				= {{"HUE           "}};
static ROMDATA char EdgeEnhance[][MenuNameCol]			= {{"EDGE ENHANCE  "}};

static ROMDATA char AutoSatEn[][MenuNameCol]		= {{"AUTO SATURATE "}};
static ROMDATA char AutoEdgeEn[][MenuNameCol]		= {{"AUTO EDGE     "}};
//static ROMDATA char AutoBrightEn[][MenuNameCol]	= {{"AUTO BRIGHT   "}};
// effect menu item
static ROMDATA char GammaItem[][MenuItemCol] = {
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	"ON        ", "OFF(1.0)  ", "0.9       ", "0.8       ", 
	"0.7       ",  "0.6       ", "0.55      ", "0.5       ", "0.45      ", "0.4       ", "CUSTOM    "
#else 								// MDIN-i51X (i510, i540, i550)
	"ON       >", "OFF(1.0) >"
#endif
};

static ROMDATA char AutoAdjustItem[][MenuItemCol]		= {
	"OFF       ",	"LOW       ",	"MIDDLE    ",	"HIGH      ",
};


// effect menu info
static ROMDATA OSD_MENU_INFO EffectMenuInfo[] = {
	// menu,				attb,		Item,					init,	max,						title
#if defined(__USE_4K60_INOUT__)
	{(PCHAR)LensShading,	MENU_SKIP,	(PCHAR)OnOffItem,		0,		GetArrayMax(OnOffItem),		(PCHAR)EffectMenuTitle	},
#else
	{(PCHAR)LensShading,	MENU_ITEM,	(PCHAR)OnOffItem,		0,		GetArrayMax(OnOffItem),		(PCHAR)EffectMenuTitle	},
#endif
	#if defined(__USE_MDIN_i500__)			// MDIN-i500
	{(PCHAR)GMCorrect,		MENU_ITEM,	(PCHAR)GammaItem,		0,		GetArrayMax(GammaItem),		(PCHAR)EffectMenuTitle	},
	#else									// MDIN-i51X (i510, i540, i550)
//	{(PCHAR)GMCorrect,		MENU_ITEM,	(PCHAR)GammaItem,		0,		GetArrayMax(GammaItem),		(PCHAR)EffectMenuTitle	},
	{(PCHAR)GMCorrect,		MENU_SUB,	(PCHAR)GammaItem,		0,		GetArrayMax(GammaItem),		(PCHAR)EffectMenuTitle	},		
	#endif
	{(PCHAR)ContrastMenu,	MENU_VALUE,	NULL,					10,		20,							(PCHAR)EffectMenuTitle	},
	{(PCHAR)ColBrightMenu,	MENU_VALUE,	NULL,					10,		20,							(PCHAR)EffectMenuTitle	},
	{(PCHAR)SaturationMenu,	MENU_VALUE,	NULL,					10,		20,							(PCHAR)EffectMenuTitle	},
	{(PCHAR)HueMenu,		MENU_VALUE,	NULL,					10,		20,							(PCHAR)EffectMenuTitle	},
#if defined(__USE_EXT2IPP__) || defined(__USE_NO__CMOS__)
	{(PCHAR)EdgeEnhance,	MENU_SKIP,	NULL,					10,		20,							(PCHAR)EffectMenuTitle	},
	{(PCHAR)AutoSatEn,		MENU_SKIP,	(PCHAR)AutoAdjustItem,	2,		GetArrayMax(AutoAdjustItem),(PCHAR)EffectMenuTitle	},	
	{(PCHAR)AutoEdgeEn,		MENU_SKIP,	(PCHAR)OnOffItem,		1,		GetArrayMax(OnOffItem),		(PCHAR)EffectMenuTitle	},
#else
	{(PCHAR)EdgeEnhance,	MENU_VALUE,	NULL,					10,		20,							(PCHAR)EffectMenuTitle	},
	{(PCHAR)AutoSatEn,		MENU_ITEM,	(PCHAR)AutoAdjustItem,	2,		GetArrayMax(AutoAdjustItem),(PCHAR)EffectMenuTitle	},	
	{(PCHAR)AutoEdgeEn,		MENU_ITEM,	(PCHAR)OnOffItem,		1,		GetArrayMax(OnOffItem),		(PCHAR)EffectMenuTitle	},		
#endif
//	{(PCHAR)AutoBrightEn,	MENU_ITEM,	(PCHAR)AutoAdjustItem,	2,		GetArrayMax(AutoAdjustItem),(PCHAR)EffectMenuTitle	},			
	{(PCHAR)DefaultMenu,	MENU_FUNC,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),		(PCHAR)EffectMenuTitle	},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),		(PCHAR)EffectMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 7-2. Effect - GMCorrect Option
//--------------------------------------------------------------------------------------------------------------------------

static ROMDATA char GMCMenuTitle[][MenuTitleCol]	= {
	"GAMMA CORRECTION OPTION   ", "                          "
};

// GMCorrect mode setup menu
static ROMDATA char GM_FRONT[][MenuNameCol]			= {{"FRONT GM EN   "}};
static ROMDATA char GM_REAR[][MenuNameCol]			= {{"REAR GM EN    "}};
static ROMDATA char GM_8TAP_INV[][MenuNameCol]		= {{"REAR GM INV   "}};
static ROMDATA char GM_Bayer_LUT_EXTAND[][MenuNameCol]	= {{"BAY LUT EXTEND"}};
static ROMDATA char GM_RGB_LUT_EXTAND[][MenuNameCol]	= {{"RGB LUT EXTEND"}};
static ROMDATA char GM_YC_LUT_EXTAND[][MenuNameCol]	= {{"Y LUT EXTEND  "}};
static ROMDATA char GM_BYLUT_Xresolution[][MenuNameCol]	= {{"BYLUT XSAMPLE "}};
static ROMDATA char GM_RGBLUT_Xresolution[][MenuNameCol]	= {{"RGBLUT XSAMPLE"}};
static ROMDATA char GM_RGBLUT_Thermo[][MenuNameCol]	= {{"RGBLUT THERMO "}};

static ROMDATA char GM_F_Item[][MenuItemCol] = {					// MDIN-i51X (i510, i540, i550)
	"DEFAULT   ", "OFF-BYPASS", "0.9       ", "0.8       ", 
	"0.7       ",  "0.6       ", "0.55      ", "0.5       ", "0.45      ", "0.4       ", "CUSTOM    "
};

static ROMDATA char GM_R_Item[][MenuItemCol] = {					// MDIN-i51X (i510, i540, i550)
	"DEFAULT   ", "OFF-BYPASS", "0.9       ", "0.8       ", 
	"0.7       ",  "0.6       ", "0.55      ", "0.5       ", "0.45      ", "0.4       " 
};

static ROMDATA char GM_INV_Item[][MenuItemCol] = {				// MDIN-i51X (i510, i540, i550)
	"DEFAULT   ", "OFF       ", "0.9       ", "0.8       ", 
	"0.7       ",  "0.6       ", "0.55      ", "0.5       ", "0.45      ", "0.4       ", "CUSTOM    "
};

static ROMDATA char GM_LUT_EXTAND_Item[][MenuItemCol] = {			// MDIN-i51X (i510, i540, i550)
	"UNIFORM   ", "EXTEND-F2C",//	"EXTEND-C2F" 
};

static ROMDATA char GM_LUT_Xresolution_Item[][MenuItemCol] = {	// MDIN-i51X (i510, i540, i550)
	"256x256LUT", "128x256LUT", "64x256LUT ", "32x256LUT "
};

static ROMDATA char GM_RGBLUT_Thermo_Item[][MenuItemCol] = {	// MDIN-i51X (i510, i540, i550)
	"NORMAL    ",	"THERMO    ", "IR TEST   "
};

#if __TONE_RGB_BOOT_THERMO_ENABLE__
#define RGBLUT_Thermo	1
#else
#define RGBLUT_Thermo	0
#endif


// GM menu info
static ROMDATA OSD_MENU_INFO GMCMenuInfo[] = {
	// menu,				attb,		Item,					init,	max,						title
	{(PCHAR)GM_FRONT,		MENU_ITEM,		(PCHAR)GM_F_Item,		0,	GetArrayMax(GM_F_Item),		(PCHAR)GMCMenuTitle	},
	
	#if defined(__RGBGAMMA_USE_ADJUST__)	
	{(PCHAR)GM_REAR,		MENU_SKIP,		(PCHAR)GM_R_Item,		0,	GetArrayMax(GM_R_Item),		(PCHAR)GMCMenuTitle	},	
	#else
	{(PCHAR)GM_REAR,		MENU_ITEM,		(PCHAR)GM_R_Item,		0,	GetArrayMax(GM_R_Item),		(PCHAR)GMCMenuTitle	},	
	#endif	
	
	{(PCHAR)GM_8TAP_INV,	MENU_ITEM,		(PCHAR)GM_INV_Item,	0,		GetArrayMax(GM_INV_Item),	(PCHAR)GMCMenuTitle	},		
	
	{(PCHAR)GM_Bayer_LUT_EXTAND,	MENU_TEXT,	(PCHAR)GM_LUT_EXTAND_Item,	0,	GetArrayMax(GM_LUT_EXTAND_Item),	(PCHAR)GMCMenuTitle	},	
	
	#if defined(__RGBGAMMA_USE_ADJUST__)
	{(PCHAR)GM_RGB_LUT_EXTAND,	MENU_TEXT,		(PCHAR)GM_LUT_EXTAND_Item,	0,	GetArrayMax(GM_LUT_EXTAND_Item),	(PCHAR)GMCMenuTitle	},		
	#else
	{(PCHAR)GM_RGB_LUT_EXTAND,	MENU_ITEM,		(PCHAR)GM_LUT_EXTAND_Item,	0,	GetArrayMax(GM_LUT_EXTAND_Item),	(PCHAR)GMCMenuTitle	},	
	#endif
	
	{(PCHAR)GM_YC_LUT_EXTAND,	MENU_ITEM,		(PCHAR)GM_LUT_EXTAND_Item,	0,	GetArrayMax(GM_LUT_EXTAND_Item),	(PCHAR)GMCMenuTitle	},		
	{(PCHAR)GM_BYLUT_Xresolution,	MENU_ITEM,	(PCHAR)GM_LUT_Xresolution_Item,	2,	GetArrayMax(GM_LUT_Xresolution_Item),	(PCHAR)GMCMenuTitle	},		
	{(PCHAR)GM_RGBLUT_Xresolution,	MENU_ITEM,	(PCHAR)GM_LUT_Xresolution_Item,	2,	GetArrayMax(GM_LUT_Xresolution_Item),	(PCHAR)GMCMenuTitle	},	
	{(PCHAR)GM_RGBLUT_Thermo,		MENU_ITEM,	(PCHAR)GM_RGBLUT_Thermo_Item,	RGBLUT_Thermo,	GetArrayMax(GM_RGBLUT_Thermo_Item),	(PCHAR)GMCMenuTitle	},		
	{(PCHAR)RetMenu,	MENU_EXIT,	(PCHAR)BlankItem,	0,		GetArrayMax(BlankItem),	(PCHAR)GMCMenuTitle	},	
};


//--------------------------------------------------------------------------------------------------------------------------
// 8-1.Special
//--------------------------------------------------------------------------------------------------------------------------

// special menu title
static ROMDATA char SpecialMenuTitle[][MenuTitleCol]	= {
	"SPECIAL                   ", "                          "
};

// special menu
static ROMDATA char Dzoom[][MenuNameCol]			= {{"DZOOM         "}};
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)
static ROMDATA char DefogDwdr[][MenuNameCol]		= {{"DEFOG/DWDR    "}};
#else
static ROMDATA char Defog[][MenuNameCol]		    = {{"DEFOG         "}};
static ROMDATA char Dwdr[][MenuNameCol]		    = {{"DWDR          "}};
#endif
static ROMDATA char MDetect[][MenuNameCol]			= {{"MOTION DETECT "}};
static ROMDATA char DISMenu[][MenuNameCol]			= {{"DIS           "}};
static ROMDATA char BLight[][MenuNameCol]			= {{"BACKLIGHT     "}};
static ROMDATA char Binning[][MenuNameCol]			= {{"BINNING       "}};
static ROMDATA char FLKDet[][MenuNameCol]			= {{"FLICKER DETECT"}};
static ROMDATA char DPixel[][MenuNameCol]			= {{"DEFECT PIXEL  "}};
static ROMDATA char DispFunc[][MenuNameCol]		= {{"DISP. FUNCTION"}};
static ROMDATA char WDRMenu[][MenuNameCol]			= {{"WDR           "}};
static ROMDATA char BlankMenu[][MenuNameCol]		= {{"              "}};

// special menu item
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)
static ROMDATA char DefogDwdrItem[][MenuItemCol]		= {
#if defined(__USE_MDIN_i500__)				// MDIN-i500
	"OFF       ",	"AUTO      ",	"DEFOG ON  "
#else										// MDIN-i51X (i510, i540, i550)
	"OFF       ",	"DEFOG AUTO",	"DWDR AUTO>",	"FULL AUTO>", "DEFOG ON >", "DWDR ON  >"
#endif
};
#else
static ROMDATA char DefogItem[][MenuItemCol]		= {
#if defined(__USE_MDIN_i500__)				// MDIN-i500
	"OFF       ",	"AUTO      ",	"DEFOG ON  "
#else										// MDIN-i51X (i510, i540, i550)
#if defined(__USE_4K60_INOUT__)
	"OFF       ",	"ON       >"//,	"AUTO     >"	// not supported 'auto'
#else
	"OFF       ",	"ON       >",	"AUTO     >"
#endif
#endif
};

static ROMDATA char DWDRItem[][MenuItemCol]		= {
#if defined(__USE_4K60_INOUT__)
	"OFF       ",	"ON       >"//,	"AUTO     >"	// not supported 'auto'
#else
	"OFF       ",	"ON       >",	"AUTO     >"
#endif
};
#endif

static ROMDATA char BLightItem[][MenuItemCol]		= {
	"OFF       ",	"BLC      >",	"HLM      >"
};

static ROMDATA char BinningItem[][MenuItemCol] = {
	"OFF       ",	"ON        "	//, "MODE 2    "
};

static ROMDATA char DPixelItem[][MenuItemCol] = {
	"OFF      >",	"ON       >"	, "STATIC   >"
//#if defined(__USE_MDIN_i5XX_REV__)	// i5x0
//	,"ON(FIXED)>"
//#endif
};


// special menu info
static ROMDATA OSD_MENU_INFO SpecialMenuInfo[] = {
	// menu,			attb,		Item,				init,	max,						title
#if defined(__MDIN_i550_USB_V100__)
	{(PCHAR)Dzoom,		MENU_SKIP,	(PCHAR)OnOffSub,	0,		GetArrayMax(OnOffSub),		(PCHAR)SpecialMenuTitle	},
#else
	{(PCHAR)Dzoom,		MENU_SUB,	(PCHAR)OnOffSub,	0,		GetArrayMax(OnOffSub),		(PCHAR)SpecialMenuTitle	},
#endif
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)	
	{(PCHAR)DefogDwdr,	MENU_SUB,	(PCHAR)DefogDwdrItem,	0,	GetArrayMax(DefogDwdrItem),	(PCHAR)SpecialMenuTitle	},
	{(PCHAR)BlankMenu,	MENU_SKIP,	(PCHAR)BlankItem,	0,		GetArrayMax(BlankItem),		(PCHAR)SpecialMenuTitle	},	// temp menu
#else
	{(PCHAR)Defog,	    MENU_SUB,	(PCHAR)DefogItem,	0,	GetArrayMax(DefogItem),	(PCHAR)SpecialMenuTitle	},
	{(PCHAR)Dwdr,	    MENU_SUB,	(PCHAR)DWDRItem,	0,	GetArrayMax(DWDRItem),	(PCHAR)SpecialMenuTitle	},
#endif		
#if defined(__USE_MDIN_i500__)			// MDIN-i500
	{(PCHAR)MDetect,	MENU_SKIP,	(PCHAR)OnOffSub,	0,		GetArrayMax(OnOffSub),		(PCHAR)SpecialMenuTitle	},
#elif defined(__MDIN_i550_USB_V100__)
	{(PCHAR)MDetect,	MENU_SKIP,	(PCHAR)OnOffSub,	0,		GetArrayMax(OnOffSub),		(PCHAR)SpecialMenuTitle	},
#else									// MDIN-i51X (i510, i540, i550)
	{(PCHAR)MDetect,	MENU_SUB,	(PCHAR)OnOffSub,	0,		GetArrayMax(OnOffSub),		(PCHAR)SpecialMenuTitle	},
#endif
#if (defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)) && !defined(__USE_MDIN_i5XX_REV__) 	// i510, i540, i550 org. chip
	{(PCHAR)DISMenu,	MENU_SKIP,	(PCHAR)OnOffItem,	0,		GetArrayMax(OnOffItem),		(PCHAR)SpecialMenuTitle	},		
	{(PCHAR)BLight,		MENU_SKIP,	(PCHAR)BLightItem,	0,		GetArrayMax(BLightItem),	(PCHAR)SpecialMenuTitle	},
#elif defined(__USE_AFZOOM_LENS__) && (defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX334__) || defined(__USE_CMOS_IMX253__))		//both 4k AF and DIS bandwith limit
	{(PCHAR)DISMenu,	MENU_SKIP,	(PCHAR)OnOffItem,	0,		GetArrayMax(OnOffItem),		(PCHAR)SpecialMenuTitle	},		
	{(PCHAR)BLight,		MENU_SUB,	(PCHAR)BLightItem,	0,		GetArrayMax(BLightItem),	(PCHAR)SpecialMenuTitle	},
#elif defined(__USE_4K60_INOUT__)
	{(PCHAR)DISMenu,	MENU_SKIP,	(PCHAR)OnOffItem,	0,		GetArrayMax(OnOffItem),		(PCHAR)SpecialMenuTitle	},
	{(PCHAR)BLight,		MENU_SKIP,	(PCHAR)BLightItem,	0,		GetArrayMax(BLightItem),	(PCHAR)SpecialMenuTitle	},
#elif defined(__MDIN_i550_USB_V100__)
	{(PCHAR)DISMenu,	MENU_SKIP,	(PCHAR)OnOffItem,	0,		GetArrayMax(OnOffItem),		(PCHAR)SpecialMenuTitle	},
	{(PCHAR)BLight,		MENU_SKIP,	(PCHAR)BLightItem,	0,		GetArrayMax(BLightItem),	(PCHAR)SpecialMenuTitle	},
#else									// MDIN-i500, i51X revision chip
	{(PCHAR)DISMenu,	MENU_ITEM,	(PCHAR)OnOffItem,	0,		GetArrayMax(OnOffItem),		(PCHAR)SpecialMenuTitle	},
	{(PCHAR)BLight,		MENU_SUB,	(PCHAR)BLightItem,	0,		GetArrayMax(BLightItem),	(PCHAR)SpecialMenuTitle	},
#endif
	{(PCHAR)Binning,	MENU_ITEM,	(PCHAR)BinningItem,	0,		GetArrayMax(BinningItem),	(PCHAR)SpecialMenuTitle},
#if defined(__USE_4K60_MD2__) || defined(__USE_NO__CMOS__) || defined(__USE_EXT2IPP__)
	{(PCHAR)DPixel,		MENU_SKIP,	(PCHAR)DPixelItem,	1,		GetArrayMax(DPixelItem),	(PCHAR)SpecialMenuTitle	},	
#else
	{(PCHAR)DPixel,		MENU_SUB,	(PCHAR)DPixelItem,	1,		GetArrayMax(DPixelItem),	(PCHAR)SpecialMenuTitle	},
#endif
	{(PCHAR)DispFunc,	MENU_SUB,	(PCHAR)SubItem,		0,		GetArrayMax(SubItem),		(PCHAR)SpecialMenuTitle	},
#if defined(__USE_WDR_PROCESS__) && !defined(__MDIN_i550_USB_V100__)
	{(PCHAR)WDRMenu,	MENU_ITEM,	(PCHAR)OnOffItem,	0,		GetArrayMax(OnOffItem),		(PCHAR)SpecialMenuTitle	},
#else
	{(PCHAR)WDRMenu,	MENU_SKIP,	(PCHAR)OnOffItem,	0,		GetArrayMax(OnOffItem),		(PCHAR)SpecialMenuTitle	},
#endif
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	{(PCHAR)FLKDet,		MENU_ITEM,	(PCHAR)OnOffItem,	0,		GetArrayMax(OnOffItem),		(PCHAR)SpecialMenuTitle},
#else																						// i500
	{(PCHAR)FLKDet,		MENU_SKIP,	(PCHAR)OnOffItem,	0,		GetArrayMax(OnOffItem),		(PCHAR)SpecialMenuTitle},
#endif
	{(PCHAR)RetMenu,	MENU_EXIT,	(PCHAR)BlankItem,	0,		GetArrayMax(BlankItem),		(PCHAR)SpecialMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 8-2-1'.Special	- DEFOG & DWDR_PARAMETER
//--------------------------------------------------------------------------------------------------------------------------

// defog/dwdr menu title
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)
static ROMDATA char DefogDwdrMenuTitle[][MenuTitleCol]	= {
	"DEFOG / D-WDR             ", "                          "
};
// defog/dwdr menu
static ROMDATA char DefogDwdrSens[][MenuNameCol]		= {{"STRENGTH      "}};
static ROMDATA char DefogDwdrThld[][MenuNameCol]		= {{"THRESHOLD     "}};
static ROMDATA char DefogDwdrSat[][MenuNameCol]		= {{"SATURATION    "}};
static ROMDATA char DefogDwdrLocal[][MenuNameCol]		= {{"LOCAL RATIO   "}};
static ROMDATA char DefogDwdrSatSync[][MenuNameCol]	= {{"SAT. SYNC     "}};
#else

// defog menu title
static ROMDATA char DefogMenuTitle[][MenuTitleCol]	= {
	
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	"DEFOG AUTO                ", "                          "
#else       						// MDIN-i51X (i510, i540, i550)
	"DEFOG SETUP               ", "                          "
#endif
};

// defog menu
static ROMDATA char DefogSens[][MenuNameCol]			= {{"STRENGTH      "}};
static ROMDATA char DefogThld[][MenuNameCol]			= {{"THRESHOLD     "}};
static ROMDATA char DefogAutoLevel[][MenuNameCol]		= {{"AUTO LEVEL    "}};
static ROMDATA char DWDRAutoLevel[][MenuNameCol]		= {{"AUTO LEVEL    "}};
#endif

#if defined(__USE_MDIN_i500__)		// MDIN-i500
static ROMDATA char DefogBright[][MenuNameCol]			= {{"DEFOG BRIGHT  "}};
static ROMDATA char DefogSensItem[][MenuItemCol]	= {
	"1         ",	"2         ", 	"3         ",	"4         ", "5         "
};
#endif

static ROMDATA char DefogDwdrALItem[][MenuItemCol]	= {
	"HIGH      ","MID       ","LOW       "
};

#if defined(__USE_DEFOG_DWDR_ONE_MENU__)
static ROMDATA char DefogDwdrSSItem[][MenuItemCol]	= {
	"OFF       ",	"ON        "
};
// defog menu info
static ROMDATA OSD_MENU_INFO DefogDwdrMenuInfo[] = {
	// menu,				attb,		Item,					init,		max,							title
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	{(PCHAR)DefogDwdrSens,	MENU_ITEM,	(PCHAR)DefogDwdrSensItem,	3,		GetArrayMax(DefogDwdrSensItem),	(PCHAR)DefogDwdrMenuTitle	},
	{(PCHAR)DefogBright,	MENU_VALUE,	NULL,						0,		20,								(PCHAR)DefogDwdrMenuTitle	},
	{(PCHAR)DefogDwdrSat,	MENU_SKIP,	NULL,						0,		16,								(PCHAR)DefogDwdrMenuTitle	},
	{(PCHAR)DefogDwdrLocal,	MENU_SKIP,	NULL,						8,		16,								(PCHAR)DefogDwdrMenuTitle	},
	{(PCHAR)DefogDwdrSatSync,	MENU_SKIP,	(PCHAR)DefogDwdrSSItem,	0,		GetArrayMax(DefogDwdrSSItem),	(PCHAR)DefogDwdrMenuTitle	},
#else								// MDIN-i51X (i510, i540, i550)
	{(PCHAR)DefogDwdrSens,	MENU_VALUE,	NULL,						0,		16,								(PCHAR)DefogDwdrMenuTitle	},
	{(PCHAR)DefogDwdrThld,	MENU_VALUE,	NULL,						0,		3,								(PCHAR)DefogDwdrMenuTitle	},
	{(PCHAR)DefogDwdrSat,	MENU_VALUE,	NULL,						0,		16,								(PCHAR)DefogDwdrMenuTitle	},
	{(PCHAR)DefogDwdrLocal,	MENU_VALUE,	NULL,						8,		16,								(PCHAR)DefogDwdrMenuTitle	},
	{(PCHAR)DefogDwdrSatSync,	MENU_ITEM,	(PCHAR)DefogDwdrSSItem,	0,		GetArrayMax(DefogDwdrSSItem),	(PCHAR)DefogDwdrMenuTitle	},
#endif
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,			0,		GetArrayMax(BlankItem),			(PCHAR)DefogDwdrMenuTitle	},
};
#else

// defog menu info
static ROMDATA OSD_MENU_INFO DefogMenuInfo[] = {
	// menu,				attb,		Item,					init,	max,						title
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	{(PCHAR)DefogSens,		MENU_ITEM,	(PCHAR)DefogSensItem,	3,		GetArrayMax(DefogSensItem),	(PCHAR)DefogMenuTitle	},
	{(PCHAR)DefogBright,	MENU_VALUE,	NULL,					0,		20,							(PCHAR)DefogMenuTitle	},
#else								// MDIN-i51X (i510, i540, i550)

	{(PCHAR)DefogSens,		MENU_VALUE,	NULL,					0,		16,							(PCHAR)DefogMenuTitle	},
	{(PCHAR)DefogThld,		MENU_VALUE,	NULL,					0,		3,							(PCHAR)DefogMenuTitle	},
	{(PCHAR)DefogAutoLevel,	MENU_ITEM,	(PCHAR)DefogDwdrALItem,		2,		GetArrayMax(DefogDwdrALItem),(PCHAR)DefogMenuTitle	},
#endif
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),		(PCHAR)DefogMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 8-3-1.Special	- D-WDR
//--------------------------------------------------------------------------------------------------------------------------
// d-wdr menu title
static ROMDATA char DWDRMenuTitle[][MenuTitleCol]	= {
	"DWDR SETUP                ", "                          "
};

// d-wdr menu
static ROMDATA char DWDRSens[][MenuNameCol]			= {{"STRENGTH      "}};
static ROMDATA char DWDRSat[][MenuNameCol]				= {{"SATURATION    "}};
static ROMDATA char DWDRLocal[][MenuNameCol]			= {{"LOCAL RATIO   "}};
static ROMDATA char DWDRSatSync[][MenuNameCol]			= {{"SAT. SYNC     "}};

// d-wdr menu item
static ROMDATA char DWDRSSItem[][MenuItemCol]	= {
	"OFF       ",	"ON        "
};

// d-wdr menu info
static ROMDATA OSD_MENU_INFO DWDRMenuInfo[] = {
	// menu,				attb,		Item,					init,	max,					title
	{(PCHAR)DWDRSens,		MENU_VALUE,	NULL,					0,		16,						(PCHAR)DWDRMenuTitle},
	{(PCHAR)DWDRSat,		MENU_VALUE,	NULL,					0,		16,						(PCHAR)DWDRMenuTitle},
	{(PCHAR)DWDRLocal,		MENU_VALUE,	NULL,					8,		16,						(PCHAR)DWDRMenuTitle},
	{(PCHAR)DWDRSatSync,	MENU_ITEM,	(PCHAR)DWDRSSItem,	    0,		GetArrayMax(DWDRSSItem),(PCHAR)DWDRMenuTitle},
	{(PCHAR)DWDRAutoLevel,	MENU_ITEM,	(PCHAR)DefogDwdrALItem,		2,		GetArrayMax(DefogDwdrALItem),(PCHAR)DWDRMenuTitle},	
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),	(PCHAR)DWDRMenuTitle},
};

#endif
//--------------------------------------------------------------------------------------------------------------------------
// 8-6-1.Special	- Backlight(BLC)
//--------------------------------------------------------------------------------------------------------------------------

// backlight(blc) menu title
static ROMDATA char BLCMenuTitle[][MenuTitleCol]	= {
	"BACKLIGHT - BLC           ",
	"                          "
};

// backlight(blc) menu
static ROMDATA char BLCMode[][MenuNameCol]				= {{"BLC           "}};
static ROMDATA char BLCDisplay[][MenuNameCol]			= {{"DISPLAY       "}};

// backlight(blc) menu item
static ROMDATA char BLCModeItem[][MenuItemCol]	= {
//	"OFF       ",	"LOW       ",	"MIDDLE    ",	"HIGH      "
	"OFF       ",	"ON        "
};

// backlight(blc) menu info
static ROMDATA OSD_MENU_INFO BLCMenuInfo[] = {
	// menu,				attb,		Item,					init,	max,						title
	{(PCHAR)BLCMode,		MENU_ITEM,	(PCHAR)BLCModeItem,		1,		GetArrayMax(BLCModeItem),	(PCHAR)BLCMenuTitle	},
	{(PCHAR)SizePosition,	MENU_SUB,	(PCHAR)SubItem,			0,		GetArrayMax(SubItem),		(PCHAR)BLCMenuTitle	},
	{(PCHAR)BLCDisplay,		MENU_ITEM,	(PCHAR)OnOffItem,		1,		GetArrayMax(OnOffItem),		(PCHAR)BLCMenuTitle	},
	{(PCHAR)DefaultMenu,	MENU_FUNC,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),		(PCHAR)BLCMenuTitle	},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),		(PCHAR)BLCMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 8-6-1.Special	- Backlight(HSBLC)
//--------------------------------------------------------------------------------------------------------------------------

// backlight(hsblc) menu title
static ROMDATA char HSBLCMenuTitle[][MenuTitleCol]	= {
	"BACKLIGHT - HLM           ", "                          "
};

// backlight(hsblc) menu
static ROMDATA char BlackMask[][MenuNameCol]			= {{"BLACK MASK    "}};

// backlight(hsblc) menu item
static ROMDATA char HSBLCAreaItem[][MenuItemCol]	= {
	"AREA 1    "	//"AREA 2    ",	"AREA 3    ",	"AREA 4    "
};

// backlight(hsblc) menu info
static ROMDATA OSD_MENU_INFO HSBLCMenuInfo[] = {
	// menu,				attb,		Item,					init,	max,						title
	{(PCHAR)AreaMenu,		MENU_SKIP,	(PCHAR)HSBLCAreaItem,	0,		GetArrayMax(HSBLCAreaItem),	(PCHAR)HSBLCMenuTitle	},
	{(PCHAR)AreaDisplay,	MENU_ITEM,	(PCHAR)OnOffItem,		1,		GetArrayMax(OnOffItem),		(PCHAR)HSBLCMenuTitle	},
	{(PCHAR)SizePosition,	MENU_SUB,	(PCHAR)SubItem,			0,		GetArrayMax(SubItem),		(PCHAR)HSBLCMenuTitle	},
	{(PCHAR)LevelMenu,		MENU_VALUE,	NULL,					3,		20,							(PCHAR)HSBLCMenuTitle	},
	{(PCHAR)BlackMask,		MENU_ITEM,	(PCHAR)OnOffItem,		1,		GetArrayMax(OnOffItem),		(PCHAR)HSBLCMenuTitle	},
	{(PCHAR)DefaultMenu,	MENU_FUNC,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),		(PCHAR)HSBLCMenuTitle	},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),		(PCHAR)HSBLCMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 8-4-1.Special	- Motion Detect
//--------------------------------------------------------------------------------------------------------------------------

// md menu title
static ROMDATA char MDMenuTitle[][MenuTitleCol]	= {
	"MOTION DETECT             ",
	"                          "
};

// md menu
static ROMDATA char MDAreaEnable[][MenuNameCol]			= {{" AREA ENABLE  "}};
static ROMDATA char MDAreaDisplay[][MenuNameCol]		= {{" AREA DISPLAY "}};
static ROMDATA char MDSizePosition[][MenuNameCol]		= {{" SIZE/POSITION"}};
static ROMDATA char Sensitivity[][MenuNameCol]			= {{"SENSITIVITY   "}};
static ROMDATA char MotionView[][MenuNameCol]			= {{"MOTION VIEW   "}};

// md menu item
static ROMDATA char MDAreaItem[][MenuItemCol]	= {
	"AREA 1    ",	"AREA 2    ",	"AREA 3    ",	"AREA 4    "
};

// md menu info
static ROMDATA OSD_MENU_INFO MDMenuInfo[] = {
	// menu,				attb,		Item,					init,	max,					title
	{(PCHAR)AreaMenu,		MENU_ITEM,	(PCHAR)MDAreaItem,		0,		GetArrayMax(MDAreaItem),(PCHAR)MDMenuTitle	},
	{(PCHAR)MDAreaEnable,	MENU_ITEM,	(PCHAR)OnOffItem,		1,		GetArrayMax(OnOffItem),	(PCHAR)MDMenuTitle	},
	{(PCHAR)MDAreaDisplay,	MENU_ITEM,	(PCHAR)OnOffItem,		1,		GetArrayMax(OnOffItem),	(PCHAR)MDMenuTitle	},
	{(PCHAR)MDSizePosition,	MENU_SUB,	(PCHAR)SubItem,			0,		GetArrayMax(SubItem),	(PCHAR)MDMenuTitle	},
	{(PCHAR)Sensitivity,	MENU_VALUE,	NULL,					20,		40,						(PCHAR)MDMenuTitle	},
	{(PCHAR)MotionView,		MENU_ITEM,	(PCHAR)OnOffItem,		1,		GetArrayMax(OnOffItem),	(PCHAR)MDMenuTitle	},
	{(PCHAR)DefaultMenu,	MENU_FUNC,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),	(PCHAR)MDMenuTitle	},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),	(PCHAR)MDMenuTitle	},
};


//--------------------------------------------------------------------------------------------------------------------------
// 8-1-1.Special	- D-ZOOM
//--------------------------------------------------------------------------------------------------------------------------

// dzoom menu title
static ROMDATA char DZMenuTitle[][MenuTitleCol]	= {
	"D-ZOOM                    ", "                          "
};

// dzoom menu
static ROMDATA char DZControl[][MenuNameCol]		= {{"ZOOM CONTROL  "}};
static ROMDATA char DZPIP[][MenuNameCol]			= {{"PIP           "}};
static ROMDATA char DZPIPSize[][MenuNameCol]		= {{"PIP SIZE      "}};
static ROMDATA char DZPIPPosi[][MenuNameCol]		= {{"PIP POSITION  "}};
static ROMDATA char DZSave[][MenuNameCol]			= {{"DZOOM SAVE    "}};

// dzoom menu item
static ROMDATA char PIPSizeItem[][MenuItemCol]	= {	
	"1/4       ",	"1/9       ",	"1/16      ",	"1/25      "
};

// dzoom menu info
static ROMDATA OSD_MENU_INFO DZMenuInfo[] = {
	// menu,				attb,		Item,					init,	max,					title
	{(PCHAR)DZControl,		MENU_SUB,	(PCHAR)SubItem,			0,		GetArrayMax(SubItem),	(PCHAR)DZMenuTitle	},
	{(PCHAR)DZPIP,			MENU_ITEM,	(PCHAR)OnOffItem,		1,		GetArrayMax(OnOffItem),	(PCHAR)DZMenuTitle	},
	{(PCHAR)DZPIPSize,		MENU_ITEM,	(PCHAR)PIPSizeItem,		1,		GetArrayMax(PIPSizeItem),	(PCHAR)DZMenuTitle	},
	{(PCHAR)DZPIPPosi,		MENU_SUB,	(PCHAR)SubItem,			0,		GetArrayMax(SubItem),	(PCHAR)DZMenuTitle	},
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	{(PCHAR)DZSave,			MENU_SKIP,	(PCHAR)WaitOkItem,		0,		GetArrayMax(WaitOkItem),(PCHAR)DZMenuTitle	},
#else
	{(PCHAR)DZSave,			MENU_FUNC,	(PCHAR)WaitOkItem,		0,		GetArrayMax(WaitOkItem),(PCHAR)DZMenuTitle	},
#endif
	{(PCHAR)DefaultMenu,	MENU_FUNC,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),	(PCHAR)DZMenuTitle	},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),	(PCHAR)DZMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 8-8-1.Special - Defect Pixel
//--------------------------------------------------------------------------------------------------------------------------

// dpc menu title
static ROMDATA char DPCMenuTitle[][MenuTitleCol]	= {
	"DEFECT PIXEL              ", "                          "
};

// dpc menu
static ROMDATA char DPCProcess[][MenuNameCol]		= {{"STATIC PROCESS"}};
static ROMDATA char DPDShut[][MenuNameCol]		= {{"DETECT SHUT LV"}};
static ROMDATA char SDPCUpdate[][MenuNameCol]		= {{"SDPC UPDATE  "}};
static ROMDATA char SDPCClear[][MenuNameCol]		= {{"SDPC CLEAR   "}};
static ROMDATA char DDPCAuto[][MenuNameCol]		= {{"DDPC AUTO PARA"}};
static ROMDATA char DDPCTHHD1[][MenuNameCol]		= {{"DDPC THHD1    "}};
static ROMDATA char DDPCTHHD2[][MenuNameCol]		= {{"DDPC THHD2    "}};
static ROMDATA char DDPCSelMode[][MenuNameCol]	= {{"DDPC SEL MODE "}};
static ROMDATA char DPCDebugDisp[][MenuNameCol]	= {{"DPC DEBUG DISP"}};

static ROMDATA char DPDShutlvItem[][MenuItemCol]	= {
	"DEFAULT   ",	"FASTER    ",	"FASTEST   "
}; 

static ROMDATA char DDPCAutoPARAMItem[][MenuItemCol]	= {
	"PWR-0     ",	"PWR-1     ",	"PWR-2     ",	"PWR-3    ",	"PWR-4    ",	"PWR-5    ",	"PWR-6    ",	"PWR-7    "//,	"FIXED    "
};

static ROMDATA char DPCDebugDispItem[][MenuItemCol]	= {
	"OFF       ",	"DDPC PWR  ",	"SDPC      "
}; 

// dpc menu info
static ROMDATA OSD_MENU_INFO DPCMenuInfo[] = {
	// menu,				attb,		Item,				init,	max,						title
#if defined(__USE_4K60_INOUT__) || defined(__PANORAMA_SENSOR_SETUP__) || defined(__USE_EXT2IPP__) || defined(__USE_NO__CMOS__)
	{(PCHAR)DPCProcess,		MENU_SKIP,	(PCHAR)WaitOkItem,	0,		GetArrayMax(WaitOkItem),	(PCHAR)DPCMenuTitle	},	
#else
	{(PCHAR)DPCProcess,		MENU_FUNC,	(PCHAR)WaitOkItem,	0,		GetArrayMax(WaitOkItem),	(PCHAR)DPCMenuTitle	},
#endif
	{(PCHAR)DPDShut,		MENU_ITEM,	(PCHAR)DPDShutlvItem,	0,	GetArrayMax(DPDShutlvItem),	(PCHAR)DPCMenuTitle	},	
	{(PCHAR)SDPCUpdate,		MENU_FUNC,	(PCHAR)WaitOkItem,	0,		GetArrayMax(WaitOkItem),	(PCHAR)DPCMenuTitle	},
	{(PCHAR)SDPCClear,		MENU_FUNC,	(PCHAR)WaitOkItem,	0,		GetArrayMax(WaitOkItem),	(PCHAR)DPCMenuTitle	},	
	{(PCHAR)DDPCAuto,		MENU_ITEM,	(PCHAR)DDPCAutoPARAMItem,	0,		GetArrayMax(DDPCAutoPARAMItem),		(PCHAR)DPCMenuTitle	},
	{(PCHAR)DDPCTHHD1,		MENU_VALUE,	 		NULL,		100,				240,				(PCHAR)DPCMenuTitle	},
	{(PCHAR)DDPCTHHD2,		MENU_VALUE,	 		NULL,		170,				240,				(PCHAR)DPCMenuTitle	},
	{(PCHAR)DDPCSelMode,		MENU_VALUE,	 		NULL,		0,				3,				(PCHAR)DPCMenuTitle	},	
	{(PCHAR)DPCDebugDisp,		MENU_ITEM,	(PCHAR)DPCDebugDispItem,	0,		GetArrayMax(DPCDebugDispItem),		(PCHAR)DPCMenuTitle	},	
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,	0,		GetArrayMax(BlankItem),		(PCHAR)DPCMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 8-9-1.Special - Display Function
//--------------------------------------------------------------------------------------------------------------------------

// display function menu title
static ROMDATA char DispFuncMenuTitle[][MenuTitleCol]	= {
	"DISPLAY FUNCTION          ", "                          "
};

// display function menu
static ROMDATA char Freeze[][MenuNameCol]			= {{"FREEZE        "}};
static ROMDATA char Mirror[][MenuNameCol]			= {{"MIRROR        "}};
static ROMDATA char Rotation[][MenuNameCol]		= {{"ROTATION      "}};
static ROMDATA char ImgEffect[][MenuNameCol]		= {{"IMAGE EFFECT  "}};

static ROMDATA char PicStyle[][MenuNameCol]		= {{"PICTURE STYLE "}};
static ROMDATA char PicStyleCon[][MenuNameCol]	= {{"PIC CONTRAST  "}};
static ROMDATA char PicStyleSat[][MenuNameCol]	= {{"PIC SATURATION"}};
static ROMDATA char PicStyleEE[][MenuNameCol]	= {{"PIC EDGE      "}};

// display function menu item
static ROMDATA char MirrorItem[][MenuItemCol]	= {
	"OFF       ",	"MIRROR    ",	"V-FLIP    ",	"BOTH(H/V) "
};

static ROMDATA char ImgEffectItem[][MenuItemCol]	= {
	"OFF       ",	"NEGATIVE  ",	"GREY MODE ",
	"REDDISH-1 ",	"REDDISH-2 ",	"REDDISH-3 ",	"REDDISH-4 ",
	"BLUISH-1  ",	"BLUISH-2  ",	"BLUISH-3  ",	"BLUISH-4  ",
	"GREENISH-1",	"GREENISH-2",	"GREENISH-3",	"GREENISH-4",
};

static ROMDATA char RotationItem[][MenuItemCol]	= {
	"OFF       ",	"ROT_L FULL",	"ROT_L 1:1 ",
	"ROT_R FULL",	"ROT_R 1:1 ",
};

static ROMDATA char PSItem[][MenuItemCol] = {
	"OFF       ", "STANDARD >", "VIVID    >", "CLEAR    >", "VIVID CLR>", "NEUTRAL  >",
};

// display function menu info
static ROMDATA OSD_MENU_INFO DispFuncMenuInfo[] = {
	// menu,			attb,		Item,					init,	max,						title
	{(PCHAR)Freeze,		MENU_ITEM,	(PCHAR)OnOffItem,		0,		GetArrayMax(OnOffItem),		(PCHAR)DispFuncMenuTitle	},
	{(PCHAR)Mirror,		MENU_ITEM,	(PCHAR)MirrorItem,		0,		GetArrayMax(MirrorItem),	(PCHAR)DispFuncMenuTitle	},
#if defined(__MDIN_i550_USB_V100__) || defined(__MDIN_i540_4K60_V10__)
	{(PCHAR)ImgEffect,	MENU_SKIP,	(PCHAR)ImgEffectItem,	0,		GetArrayMax(ImgEffectItem),	(PCHAR)DispFuncMenuTitle	},
#else
	{(PCHAR)ImgEffect,	MENU_ITEM,	(PCHAR)ImgEffectItem,	0,		GetArrayMax(ImgEffectItem),	(PCHAR)DispFuncMenuTitle	},
#endif
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	{(PCHAR)Rotation,	MENU_SKIP,	(PCHAR)RotationItem,	0,		GetArrayMax(RotationItem),	(PCHAR)DispFuncMenuTitle	},
	{(PCHAR)PicStyle,	MENU_SKIP,	(PCHAR)PSItem,		    0,		GetArrayMax(PSItem),		(PCHAR)DispFuncMenuTitle	},	
#else								// MDIN-i51X (i510, i540, i550)
#if defined(__USE_4K60_INOUT__) || (defined(__USE_CMOS_IMX334__) && !defined(__USE_ISP_SCLK_OUT__)) || defined(__MDIN_i550_USB_V100__)
	{(PCHAR)Rotation,	MENU_SKIP,	(PCHAR)RotationItem,	0,		GetArrayMax(RotationItem),	(PCHAR)DispFuncMenuTitle	},
#else
	{(PCHAR)Rotation,	MENU_ITEM,	(PCHAR)RotationItem,	0,		GetArrayMax(RotationItem),	(PCHAR)DispFuncMenuTitle	},
#endif
	{(PCHAR)PicStyle,	MENU_ITEM,	(PCHAR)PSItem,		    1,		GetArrayMax(PSItem),		(PCHAR)DispFuncMenuTitle	},	
	{(PCHAR)PicStyleCon,MENU_VALUE,	 NULL,	                3,		6,	                        (PCHAR)DispFuncMenuTitle	},
	{(PCHAR)PicStyleSat,MENU_VALUE,	 NULL,	                3,		6,	                        (PCHAR)DispFuncMenuTitle	},
	{(PCHAR)PicStyleEE, MENU_VALUE,	 NULL,	                3,		6,	                        (PCHAR)DispFuncMenuTitle	},	
#endif
	{(PCHAR)RetMenu,	MENU_EXIT,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),		(PCHAR)DispFuncMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------




//--------------------------------------------------------------------------------------------------------------------------
// 9-1.Setup
//--------------------------------------------------------------------------------------------------------------------------

// setup menu title
static ROMDATA char SetupMenuTitle[][MenuTitleCol]	= {
	"SETUP                     ", "                          "
};

// setup menu
static ROMDATA char CamTitle[][MenuNameCol]		= {{"CAM TITLE     "}};
static ROMDATA char NtscPal[][MenuNameCol]			= {{"NTSC/PAL      "}};
static ROMDATA char RS485Menu[][MenuNameCol]		= {{"RS485         "}};
static ROMDATA char LangMenu[][MenuNameCol]		= {{"LANGUAGE      "}};
static ROMDATA char MenuColor[][MenuNameCol]		= {{"MENU COLOR    "}};
static ROMDATA char FactoryRst[][MenuNameCol]		= {{"FACTORY RESET "}};
static ROMDATA char SysInfo[][MenuNameCol]			= {{"SYSTEM INFO   "}};
static ROMDATA char MenuPosi[][MenuNameCol]		= {{"MENU POSITION "}};
static ROMDATA char OutFrmat[][MenuNameCol]		= {{"OUTPUT FORMAT "}};
static ROMDATA char LEDPWMLow[][MenuNameCol]		= {{"LED PWM(LOW)  "}};
static ROMDATA char LEDPWMHigh[][MenuNameCol]		= {{"LED PWM(HIGH) "}};

// setup menu item
static ROMDATA char NtscPalItem[][MenuItemCol]	= {
	"NTSC      ",	"PAL       "
};

static ROMDATA char LangItem[][MenuItemCol]	= {
	"ENG       ",	"CHN       ",	"JPN       ",	"KOR       "
};

static ROMDATA char MenuPosiItem[][MenuItemCol] = {
	"DEFAULT   ",	"TOP-LEFT  ",	"TOP-RIGHT ",	"BOT-RIGHT ",	"BOT-LEFT  "
};

static ROMDATA char OutFrmtItem[][MenuItemCol] = {
#if defined(__USE_59Hz_OUTPUT__)
	"720p60   >", "720p59   >", "720p50   >",
	"720p30   >", "720p25   >", "720p24   >", 			// not work in gv7600(SEP422)
	"1080i60  >", "1080i59  >", "1080i50  >",
	"1080p60  >", "1080p59  >", "1080p50  >",
	"1080p30  >", "1080p29  >", "1080p25  >", "1080p24  >",
#else
	"720p60   >", "720p50   >",
	"720p30   >", "720p25   >", "720p24   >", 			// not work in gv7600(SEP422)
	"1080i60  >", "1080i50  >",
	"1080p60  >", "1080p50  >",
	"1080p30  >", "1080p25  >", "1080p24  >",
#endif

#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	"4K3840H30>",
#if defined(__USE_59Hz_OUTPUT__)
	"4K3840H29>",
#endif
	"4K3840H25>", "4K3840H24>", 
	"4K3840H15>", "4K3840H12>",
	"4K4096H30>",
#if defined(__USE_59Hz_OUTPUT__)
	"4K4096H29>",
#endif
	"4K4096H25>", "4K4096H24>",
	"4M1440p30>", "4M1440p25>",
#if defined(__USE_4K60_INOUT__)
	"4K3840H60>",
#if defined(__USE_59Hz_OUTPUT__)
	"4K3840H59>",
#endif
	"4K3840H50>",
	"4K4096H60>",
#if defined(__USE_59Hz_OUTPUT__)
	"4K4096H59>",
#endif
	"4K4096H50>",
#endif
#endif


#if defined(__USE_SENS_240Hz_MENU__)
	"720P240  >", "720P200  >",
#else
	"720P120  >", "720P100  >",
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	"1080P120 >", "1080P100 >",
#endif
#endif

#if 0//1 //for test vga format
	"640x480p6>", "640x480p7>", "800x600p6>",
	"800x600p7>", "1024x7~p6>", "1024x7~p7>",
	"1280x960p>",
	"1024p60  >", "1024p75  >",
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	"1024p100 >",
#endif
	"1360x768 >",
	"1366x768 >", "1400x1050>", "1600x1200>",
	"1440x9~p6>", "1440x9~p7>", "1680x1050>",
	"1680x105R>", "1920x108R>", "1920x120R>",
#endif
};

// setup menu info
static ROMDATA OSD_MENU_INFO SetupMenuInfo[] = {
	// menu,			attb,		Item,					init,		max,						title
	{(PCHAR)CamTitle,	MENU_SUB,	(PCHAR)OnOffSub,		0,			GetArrayMax(OnOffSub),		(PCHAR)SetupMenuTitle	},
#if defined(__MDIN_i550_USB_V100__)
	{(PCHAR)NtscPal,	MENU_SKIP,	(PCHAR)NtscPalItem,		0,			GetArrayMax(NtscPalItem),	(PCHAR)SetupMenuTitle	},
#else
	{(PCHAR)NtscPal,	MENU_ITEM,	(PCHAR)NtscPalItem,		0,			GetArrayMax(NtscPalItem),	(PCHAR)SetupMenuTitle	},
#endif
	{(PCHAR)RS485Menu,	MENU_SUB,	(PCHAR)SubItem,			0,			GetArrayMax(SubItem),		(PCHAR)SetupMenuTitle	},
	{(PCHAR)LangMenu,	(MENU_ITEM|MENU_SKIP),	(PCHAR)LangItem,	0,	GetArrayMax(LangItem),		(PCHAR)SetupMenuTitle	},
	{(PCHAR)MenuColor,	MENU_SUB,	(PCHAR)SubItem,			0,			GetArrayMax(SubItem),		(PCHAR)SetupMenuTitle	},
	{(PCHAR)FactoryRst,	MENU_SUB,	(PCHAR)SubItem,			0,			GetArrayMax(SubItem),		(PCHAR)SetupMenuTitle	},
	{(PCHAR)SysInfo,	MENU_SUB,	(PCHAR)SubItem,			0,			GetArrayMax(SubItem),		(PCHAR)SetupMenuTitle	},
	{(PCHAR)MenuPosi,	MENU_ITEM,	(PCHAR)MenuPosiItem,	0,			GetArrayMax(MenuPosiItem),	(PCHAR)SetupMenuTitle	},
	{(PCHAR)OutFrmat,	MENU_SUB,	(PCHAR)OutFrmtItem,		0,			GetArrayMax(OutFrmtItem),	(PCHAR)SetupMenuTitle	},
#if defined(__USE_IR_LED_PWM__)
	{(PCHAR)LEDPWMLow,	(MENU_VALUE|MENU_SKIP),	NULL,		5,			33,							(PCHAR)SetupMenuTitle	},
	{(PCHAR)LEDPWMHigh,	(MENU_VALUE|MENU_SKIP),	NULL,		3,			33,							(PCHAR)SetupMenuTitle	},
#endif
	{(PCHAR)RetMenu,	MENU_EXIT,	(PCHAR)BlankItem,		0,			GetArrayMax(BlankItem),		(PCHAR)SetupMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 9-1-1.Setup - CAM Title
//--------------------------------------------------------------------------------------------------------------------------

// cam title menu title
static ROMDATA char CamTitleMenuTitle[][MenuTitleCol]	= {
	"CAM TITLE                 ", "                          "
};

// cam title menu
static ROMDATA char TitleMenu[][MenuNameCol]		= {{"TITLE         "}};
static ROMDATA char TextColor[][MenuNameCol]		= {{"TEXT COLOR    "}};

// cam title menu item
static char CamTitleText[][MenuItemCol]	= {
	{"          "},
};

static BYTE OSDTextDigit = 0;       
  
// cam title menu info
static ROMDATA OSD_MENU_INFO CamTitleMenuInfo[] = {
	// menu,			attb,		Item,					init,		max,						title
	{(PCHAR)TitleMenu,	MENU_CTITLE,(PCHAR)CamTitleText,	0,			GetArrayMax(CamTitleText),	(PCHAR)CamTitleMenuTitle	},
	{(PCHAR)TextColor,	MENU_ITEM,	(PCHAR)ColorItem,		OSD_WHITE,	GetArrayMax(ColorItem),		(PCHAR)CamTitleMenuTitle	},
	{(PCHAR)RetMenu,	MENU_EXIT,	(PCHAR)BlankItem,		0,			GetArrayMax(BlankItem),		(PCHAR)CamTitleMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 9-2-1.Setup - RS485
//--------------------------------------------------------------------------------------------------------------------------

// rs485 menu title
static ROMDATA char RS485MenuTitle[][MenuTitleCol]	= {
	"RS485                     ", "                          "
};

// rs485 menu
static ROMDATA char CamID[][MenuNameCol]			= {{"CAM ID        "}};
static ROMDATA char IDDisplay[][MenuNameCol]		= {{"ID DISPLAY    "}};
static ROMDATA char BaudRate[][MenuNameCol]		= {{"BAUD RATE     "}};

// rs485 menu item
static ROMDATA char BaudRateItem[][MenuItemCol]	= {
	"2400      ",	"4800      ",	"9600      ",	"19200     ",  "38400     "
};

// rs485 menu info
static ROMDATA OSD_MENU_INFO RS485MenuInfo[] = {
	// menu,			attb,		Item,					init,	max,						title
	{(PCHAR)CamID,		MENU_VALUE,	NULL,					1,		255,						(PCHAR)RS485MenuTitle	},
	{(PCHAR)IDDisplay,	MENU_ITEM,	(PCHAR)OnOffItem,		0,		GetArrayMax(OnOffItem),		(PCHAR)RS485MenuTitle	},
	{(PCHAR)BaudRate,	MENU_ITEM,	(PCHAR)BaudRateItem,	2,		GetArrayMax(BaudRateItem),	(PCHAR)RS485MenuTitle	},
	{(PCHAR)RetMenu,	MENU_EXIT,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),		(PCHAR)RS485MenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 9-5-1.Setup - Menu Color
//--------------------------------------------------------------------------------------------------------------------------

// osd color menu title
static ROMDATA char MenuColorMenuTitle[][MenuTitleCol]	= {
	"MENU COLOR                ", "                          "
};

// osd color menu
static ROMDATA char MenuTextColor[][MenuNameCol]		= {{"TEXT COLOR    "}};
static ROMDATA char MenuTextColorH[][MenuNameCol]		= {{"TEXT(H) COLOR "}};
static ROMDATA char MenuBGColor[][MenuNameCol]			= {{"B/G COLOR     "}};
static ROMDATA char MenuBGColorH[][MenuNameCol]		= {{"B/G(H) COLOR  "}};

// osd color menu info
static ROMDATA OSD_MENU_INFO MenuColorMenuInfo[] = {
	// menu,				attb,		Item,				init,			max,					title
	{(PCHAR)MenuTextColor,	MENU_ITEM,	(PCHAR)ColorItem,	OSD_WHITE,		GetArrayMax(ColorItem),	(PCHAR)MenuColorMenuTitle	},
	{(PCHAR)MenuTextColorH,	MENU_ITEM,	(PCHAR)ColorItem,	OSD_YELLOW,		GetArrayMax(ColorItem),	(PCHAR)MenuColorMenuTitle	},
	{(PCHAR)MenuBGColor,	MENU_ITEM,	(PCHAR)ColorItem,	OSD_NOCOLOR,	GetArrayMax(ColorItem),	(PCHAR)MenuColorMenuTitle	},
	{(PCHAR)MenuBGColorH,	MENU_ITEM,	(PCHAR)ColorItem,	OSD_BLACK_OP,	GetArrayMax(ColorItem),	(PCHAR)MenuColorMenuTitle	},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,	0,				GetArrayMax(BlankItem),	(PCHAR)MenuColorMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 9-6-1.Setup - Factory Reset
//--------------------------------------------------------------------------------------------------------------------------

// factory reset menu title
static ROMDATA char FResetMenuTitle[][MenuTitleCol]	= {
	"FACTORY RESET ?           ", "                          "
};

// factory reset menu
static ROMDATA char FResetNo[][MenuNameCol]		= {{" NO          >"}};
static ROMDATA char FResetYes[][MenuNameCol]		= {{" YES         >"}};

// factory reset menu info
static ROMDATA OSD_MENU_INFO FResetMenuInfo[] = {
	// menu,			attb,		Item,				init,	max,					title
	{(PCHAR)FResetNo,	MENU_EXIT,	(PCHAR)BlankItem,	0,		GetArrayMax(BlankItem),	(PCHAR)FResetMenuTitle	},
	{(PCHAR)FResetYes,	MENU_FUNC,	(PCHAR)BlankItem,	0,		GetArrayMax(BlankItem),	(PCHAR)FResetMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 9-7-1.Setup - SYSTEM INFORMATION
//--------------------------------------------------------------------------------------------------------------------------

// sysinfo menu title
static ROMDATA char SysInfoMenuTitle[][MenuTitleCol]	= {
	"SYSTEM INFORMATION        ", "                          "
};

// sysinfo menu
static ROMDATA char HWVer[][MenuNameCol]			= {{"H/W Ver       "}};
static ROMDATA char SWVer[][MenuNameCol]			= {{"S/W Ver       "}};
static ROMDATA char SWDate[][MenuNameCol]			= {{"S/W Date      "}};
static ROMDATA char SensorNo[][MenuNameCol]		= {{"SENSOR        "}};
static ROMDATA char LensType[][MenuNameCol]		= {{"LENS TYPE     "}};
static ROMDATA char LensMaker[][MenuNameCol]		= {{"LENS MAKER    "}};

// sysinfo menu item
static ROMDATA char HWVerItem[][MenuItemCol]	= {
	"FPGA      ",  "i500 EDK  ",  "i500 REF  ",
	"i510 EDK10",  "i510 REF10",  "i540 EDK10",  "i540 REF10",
	"i550 EDK10",  "i550 REF10",
	"i510 + 290",
	"i510 EDK11",  "i510 REF13",
	"i540 EDK11",  "i540 REF12",
	"i550 EDK11",  "i550 REF12",
	"i540 4K60 ",  "i550 USB  ",
};

static ROMDATA char SensorNoItem[][MenuItemCol]	= {
	"IMX136    ",  "MN34220   ",  "IMX236    ",  "IMX124    ",  "IMX222    ",
	"IMX185    ",  "IMX238    ",  "IMX174    ",  "AR0331    ",  "MT9P031   ",
	"MT9M131   ",  "MN34041   ",  "IMX076    ",  "PO3100K   ",  "PS1210K   ",
	"IMX290    ",  "IMX322    ",  "IMX178    ",  "IMX226    ",  "EXT2      ",
	"EXT1      ",  "OV2710    ",  "IMX274    ",  "IMX265    ",  "MN34420   ",
	"IMX326    ",  "IMX253    ",  "PS1400K   ",  "AR0134    ",  "EXT3      ",
	"IMX323    ",  "IMX252    ",  "IMX385    ",  "IMX327    ",  "IMX273    ",
	"IMX334    ", "          ",  "          ",
};

static ROMDATA char LensTypeItem[][MenuItemCol]	= {
	// zoom lens
	"DAIWON (Z)",  "YOPTIC (Z)",  "TAMRON X30",  "CANON (Z) ",  "SUNNY (Z) ",
	"ZMAX X03  ",  "UNION X10 ",  "ZMAX X10  ",  "UNION X12 ",  "          ",
	"          ",  "          ",  "          ",  "          ",  "          ",
	"          ",
	// manual lens
	"RV4515    ",  "RV0850    ",  "KAWA12120 ",  "RV1530    ",  "8X8FM1214C",
	"TAMRON 5M ",  "TAMRON    ",  "TAMRON PWM",  "SUNEX115  ",  "RV3620    ",
	"RV3220    ",  "BW3M30B400",  "BW4K40 100",  "BW3ML43B 1",  "RV10FM1614",
	"WW 4KUHD  ",  "WW 8mmIR  ",  "          ",  "          ",  "          ",
};



// sysinfo menu info
static ROMDATA OSD_MENU_INFO SysInfoMenuInfo[] = {
	// menu,			attb,		Item,					init,	max,						title
	{(PCHAR)HWVer,		MENU_TEXT,	(PCHAR)HWVerItem,		0,		GetArrayMax(HWVerItem),		(PCHAR)SysInfoMenuTitle	},
	{(PCHAR)SWVer,		MENU_TEXT,	(PCHAR)FirmwareVer,		0,		GetArrayMax(FirmwareVer),	(PCHAR)SysInfoMenuTitle	},
	{(PCHAR)SWDate,		MENU_TEXT,	(PCHAR)FirmwareDate,	0,		GetArrayMax(FirmwareDate),	(PCHAR)SysInfoMenuTitle	},
	{(PCHAR)SensorNo,	MENU_TEXT,	(PCHAR)SensorNoItem,	0,		GetArrayMax(SensorNoItem),	(PCHAR)SysInfoMenuTitle	},
	{(PCHAR)LensType,	MENU_TEXT,	(PCHAR)LensTypeItem,	0,		GetArrayMax(LensTypeItem),	(PCHAR)SysInfoMenuTitle	},
	{(PCHAR)RetMenu,	MENU_EXIT,	(PCHAR)BlankItem,		0,		GetArrayMax(BlankItem),		(PCHAR)SysInfoMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 9-9-1.Setup - Output Format
//--------------------------------------------------------------------------------------------------------------------------

// output format menu title
static ROMDATA char OutFrmtMenuTitle[][MenuTitleCol]	= {
	"Are You Sure ?            ", "                          "
};

// output format menu
static ROMDATA char OutFrmtNo[][MenuNameCol]		= {{" NO          >"}};
static ROMDATA char OutFrmtYes[][MenuNameCol]		= {{" YES         >"}};

// output format menu info
static ROMDATA OSD_MENU_INFO OutFrmtMenuInfo[] = {
	// menu,				attb,		Item,				init,	max,					title
	{(PCHAR)OutFrmtNo,	MENU_EXIT,	(PCHAR)BlankItem,	0,		GetArrayMax(BlankItem),	(PCHAR)OutFrmtMenuTitle	},
	{(PCHAR)OutFrmtYes,	MENU_FUNC,	(PCHAR)BlankItem,	0,		GetArrayMax(BlankItem),	(PCHAR)OutFrmtMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 10-1.Engr. Menu
//--------------------------------------------------------------------------------------------------------------------------

// engineer menu title
static ROMDATA char EngrMenuTitle[][MenuTitleCol]	= {
	"ENGINEER MENU             ", "                          "
};

// engineer menu
static ROMDATA char InSelect[][MenuNameCol]		= {{"INPUT SELECT  "}};
static ROMDATA char InFrmat[][MenuNameCol]			= {{"INPUT FORMAT  "}};
static ROMDATA char CCorrect[][MenuNameCol]		= {{"C-CORRECTION  "}};
static ROMDATA char TPInput[][MenuNameCol]			= {{"TP-INPUT      "}};
static ROMDATA char TPOutput[][MenuNameCol]		= {{"TP-OUTPUT     "}};
static ROMDATA char TPSensor[][MenuNameCol]		= {{"TP-SENSOR     "}};
static ROMDATA char BYRBypass[][MenuNameCol]		= {{"BAYER BYPASS  "}};
static ROMDATA char SensorFPS[][MenuNameCol]		= {{"SENSOR FPS    "}};
static ROMDATA char MiscTuneMenu[][MenuNameCol]	= {{"MISC TUNING   "}};
static ROMDATA char WDRTuneMenu[][MenuNameCol]		= {{"WDR TUNING    "}};
static ROMDATA char DNRTuneMenu[][MenuNameCol]		= {{"DNR TUNING    "}};

// engineer menu item
static ROMDATA char InSelectItem[][MenuItemCol] = {
	"SENSOR    ",	"HDMI      ",	"SEN / HDMI", "HDMI / SEN",// "SEN + HDMI"
};

static ROMDATA char InFrmtItem[][MenuItemCol] = {
#if defined(__USE_59Hz_OUTPUT__)
	"720x480i60", "720x576i50", "960x480i60", "960x576i50",
	"720x480p60", "720x576p50", "960x480p60", "960x576p50",
	"720p60    ", "720p59    ", "720p50    ",
	"720p30    ", "720p25    ", "720p24    ",
	"1080i60   ", "1080i59   ", "1080i50   ", 
	"1080p60   ", "1080p59   ", "1080p50   ",
	"1080p30   ", "1080p29   ", "1080p25   ", "1080p24   ",
#else
	"720x480i60", "720x576i50", "960x480i60", "960x576i50",
	"720x480p60", "720x576p50", "960x480p60", "960x576p50",
	"720p60    ", "720p50    ",
	"720p30    ", "720p25    ", "720p24    ",
	"1080i60   ", "1080i50   ", 
	"1080p60   ", "1080p50   ",
	"1080p30   ", "1080p25   ", "1080p24   ",	
#endif

#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
	"4K3840H30>",
#if defined(__USE_59Hz_OUTPUT__)
	"4K3840H29>",
#endif
	"4K3840H25>", "4K3840H24>",
	"4K3840H15>", "4K3840H12>",
	"4K4096H30>",
#if defined(__USE_59Hz_OUTPUT__)
	"4K4096H29>",
#endif
	"4K4096H25>", "4K4096H24>",
	"4M1440p30>", "4M1440p25>",
#if defined(__USE_4K60_INOUT__)
	"4K3840H60>",
#if defined(__USE_59Hz_OUTPUT__)
	"4K3840H59>",
#endif
	"4K3840H50>",
	"4K4096H60>",
#if defined(__USE_59Hz_OUTPUT__)
	"4K4096H59>",
#endif
	"4K4096H50>",
#endif
#endif
};

static ROMDATA char BYRBypassItem[][MenuItemCol] = {
	"OFF       ",	"F BYPASS  ", "R BYPASS  ", "F&R BYPASS"
};

static ROMDATA char SensorFpsItem[][MenuItemCol]	= {
#if	defined(__USE_SENS_240Hz_MENU__)
	"25Hz      ",	"30Hz      ",	"50Hz      ",	"60Hz      ",	"100Hz     ",	"120Hz     ",	"200Hz     ",	"240Hz     "
#elif   defined(__USE_SENS_120Hz_MENU__)
	"25Hz      ",	"30Hz      ",	"50Hz      ",	"60Hz      ",	"100Hz     ",	"120Hz     "
#elif   defined(__USE_SENS_60Hz_MENU__)
	"25Hz      ",	"30Hz      ",	"50Hz      ",	"60Hz      "
#elif   defined(__USE_SENS_50Hz_MENU__)
	"25Hz      ",	"30Hz      ",	"50Hz      "
#elif defined(__USE_SENS_10Hz_MENU__)
	"10Hz      ",	"12Hz      "
#else
	"25Hz      ",	"30Hz      "
#endif
};

static ROMDATA char WDRItem[][MenuItemCol]	= {
	"OFF       ",
#if (!defined(__USE_WDR_EXT_MENU__))
	"ON       >"
#else
  #if WDR_SENS_DOL_3FRM
	"LINE-3   >",
  #endif
  #if WDR_SENS_DOL_2FRM
	"LINE-2   >",
  #endif
  #if WDR_SENS_BUILT_IN
	"TONEMAP  >",
  #endif
  #if WDR_SENS_MULT_EXP
	"FRAME    >",
  #endif
  #if WDR_SENS_BYPAS_IN
	"DWDR     >",
  #endif
#endif
};

#if defined(__USE_25Hz_SENSOR__)
#if defined(__USE_SENS_240Hz_MENU__)
#define 	SenFpsInit	6
#elif defined(__USE_SENS_120Hz_MENU__)
#define 	SenFpsInit	4
#elif defined(__USE_SENS_60Hz_MENU__)
#define 	SenFpsInit	2
#elif defined(__USE_SENS_50Hz_MENU__)
#define 	SenFpsInit	0
#else
#define 	SenFpsInit	0
#endif
#else
#if defined(__USE_SENS_240Hz_MENU__)
#define 	SenFpsInit	7
#elif defined(__USE_SENS_120Hz_MENU__)
#define 	SenFpsInit	5
#elif defined(__USE_SENS_60Hz_MENU__)
#define 	SenFpsInit	3
#elif defined(__USE_SENS_50Hz_MENU__)
#define 	SenFpsInit	1
#else
#define 	SenFpsInit	1
#endif
#endif

#if defined(__USE_MDIN_i500__) || defined(__USE_MDIN_i550__) 		// MDIN-i500, i550
#if defined(__USE_CMOS_AUX_SYNC_OUT__) && (defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX326__))
#define		InSelAttb		MENU_SKIP
#else
#define		InSelAttb		MENU_ITEM
#endif
#else																// MDIN-i510, i540
#define		InSelAttb		MENU_SKIP
#endif

#if defined(__USE_MDIN_i550__)		// i550
#if defined(__USE_59Hz_OUTPUT__)
#define		InFrmtInit		24		// 4K_3840H30
#else
#define		InFrmtInit		20		// 4K_3840H30
#endif
#else								// i500
#if defined(__USE_59Hz_OUTPUT__)
#define		InFrmtInit		17		// 1080p60
#else
#define		InFrmtInit		15		// 1080p60
#endif
#endif

// engineer menu info
static ROMDATA OSD_MENU_INFO EngrMenuInfo[] = {
	// menu,				attb,		Item,					init,		max,						title
	{(PCHAR)InSelect,		InSelAttb,	(PCHAR)InSelectItem,	0,			GetArrayMax(InSelectItem),	(PCHAR)EngrMenuTitle	},
	{(PCHAR)InFrmat,		MENU_ITEM,	(PCHAR)InFrmtItem,		InFrmtInit,	GetArrayMax(InFrmtItem),	(PCHAR)EngrMenuTitle	},
	{(PCHAR)CCorrect,		MENU_SUB,	(PCHAR)SubItem,			0,			GetArrayMax(SubItem),		(PCHAR)EngrMenuTitle	},
	{(PCHAR)TPInput,		MENU_VALUE,	NULL,					0,			14,	(PCHAR)EngrMenuTitle							},
	{(PCHAR)TPOutput,		MENU_VALUE,	NULL,					0,			13,	(PCHAR)EngrMenuTitle							},
	{(PCHAR)TPSensor,		MENU_SUB,	(PCHAR)SubItem,			0,			GetArrayMax(SubItem),		(PCHAR)EngrMenuTitle	},
	{(PCHAR)BYRBypass,		MENU_ITEM,	(PCHAR)BYRBypassItem,	0,			GetArrayMax(BYRBypassItem),	(PCHAR)EngrMenuTitle	},
#if defined(__USE_NO__CMOS__) || defined(__USE_EXT2IPP__)
	{(PCHAR)SensorFPS,		MENU_SKIP,	(PCHAR)SensorFpsItem,	SenFpsInit,	GetArrayMax(SensorFpsItem),	(PCHAR)EngrMenuTitle	},
#else
	{(PCHAR)SensorFPS,		MENU_ITEM,	(PCHAR)SensorFpsItem,	SenFpsInit,	GetArrayMax(SensorFpsItem),	(PCHAR)EngrMenuTitle	},
#endif
	{(PCHAR)MiscTuneMenu,	MENU_SUB,	(PCHAR)SubItem,			0,			GetArrayMax(SubItem),		(PCHAR)EngrMenuTitle	},
#if defined(__USE_WDR_PROCESS__)
	{(PCHAR)WDRTuneMenu,	MENU_SUB,	(PCHAR)WDRItem,			0,			GetArrayMax(WDRItem),		(PCHAR)EngrMenuTitle	},
#else
	{(PCHAR)WDRTuneMenu,	MENU_SKIP,	(PCHAR)WDRItem,			0,			GetArrayMax(WDRItem),		(PCHAR)EngrMenuTitle	},
#endif
	{(PCHAR)DNRTuneMenu,	MENU_SUB,	(PCHAR)SubItem,			0,			GetArrayMax(SubItem),		(PCHAR)EngrMenuTitle	},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,		0,			GetArrayMax(BlankItem),		(PCHAR)EngrMenuTitle	},
};
//--------------------------------------------------------------------------------------------------------------------------
//  10-3-1. Color Correction Tuning
//--------------------------------------------------------------------------------------------------------------------------
// Color Correction menu title
static ROMDATA char ColorCorrMenuTitle[][MenuTitleCol]	= {
	"COLOR CORRECTION TUNING   ",
	"                          "
};

static ROMDATA char ColorCorrRed[][MenuNameCol]	= {{"CC RED LINE   "}};
static ROMDATA char ColorCorrGreen[][MenuNameCol]	= {{"CC GREEN LINE "}};
static ROMDATA char ColorCorrBlue[][MenuNameCol]	= {{"CC BLUE LINE  "}};
static ROMDATA char CACAREASEL[][MenuNameCol]		= {{"CAC AREA CTRL "}};
#if defined(CAC_ctrl_theta)
static ROMDATA char CACTHETA[][MenuNameCol]	= {{"CAC THETA     "}};
#else
static ROMDATA char CACCOS[][MenuNameCol]	= {{"CAC COSINE    "}};
static ROMDATA char CACSIN[][MenuNameCol]	= {{"CAC SINE      "}};
#endif
static ROMDATA char CACSAT[][MenuNameCol]	= {{"CAC SATURATION"}};
static ROMDATA char CACSAVE[][MenuNameCol]	= {{"CAC SAVE PROM "}};

#if defined(__USE_IMX265_MONO_SENSOR__)
#define Def_ColorCorr	4
#else
#define Def_ColorCorr	7
#endif

static ROMDATA char CACItem[][MenuItemCol] = {
	"MAGENTA   ", "RED       ", "YELLOW    ", "GREEN     ", "CYAN      ", "BLUE      ",
};

// Color Correction menu info
static ROMDATA OSD_MENU_INFO ColorCorrMenuInfo[] = {
	// menu,						attb,		Item,				init,				max,						title
	{(PCHAR)ColorCorrRed,	MENU_VALUE,	NULL,			Def_ColorCorr,		10,					(PCHAR)ColorCorrMenuTitle	},
	{(PCHAR)ColorCorrGreen,	MENU_VALUE,	NULL,			Def_ColorCorr,		10,					(PCHAR)ColorCorrMenuTitle	},	
	{(PCHAR)ColorCorrBlue,	MENU_VALUE,	NULL,			Def_ColorCorr,		10,					(PCHAR)ColorCorrMenuTitle	},
	{(PCHAR)CACAREASEL,		MENU_ITEM,	(PCHAR)CACItem,		0,		GetArrayMax(CACItem),		(PCHAR)ColorCorrMenuTitle	},	
#if defined(CAC_ctrl_theta)
	{(PCHAR)CACTHETA,		MENU_VALUE,	 NULL,				90,				180,					(PCHAR)ColorCorrMenuTitle	},
#else
	{(PCHAR)CACCOS,			MENU_VALUE,	 NULL,				32,				64,					(PCHAR)ColorCorrMenuTitle	},
	{(PCHAR)CACSIN,			MENU_VALUE,	 NULL,				32,				64,					(PCHAR)ColorCorrMenuTitle	},
#endif
	{(PCHAR)CACSAT,			MENU_VALUE,	 NULL,				32,				64,					(PCHAR)ColorCorrMenuTitle	},
	{(PCHAR)CACSAVE,		MENU_FUNC,	(PCHAR)WaitOkItem,	0,		GetArrayMax(WaitOkItem),	(PCHAR)ColorCorrMenuTitle	},	
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,	0,		GetArrayMax(BlankItem),		(PCHAR)ColorCorrMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 10-7-1. Sensor TP Select
//--------------------------------------------------------------------------------------------------------------------------

// sensor tp menu title
static ROMDATA char TPMenuTitle[][MenuTitleCol]	= {
	"TEST PATTERN - BAYERDATA  ", "                          "
};

// sensor tp menu
static ROMDATA char TPSelect[][MenuNameCol]		= {{"TP SELECT     "}};
static ROMDATA char TPOnOff[][MenuNameCol]			= {{"TP OFF/ON     "}};
static ROMDATA char TPSizePosi[][MenuNameCol]		= {{"TP SIZE/POS   "}};

#if defined(__USE_MDIN_i500__)		// MDIN-i500
#define	TPSensorMAX	39
#else								// MDIN-i51X (i510, i540, i550)
#define	TPSensorMAX	50
#endif

// sensor tp menu info
static ROMDATA OSD_MENU_INFO TPMenuInfo[] = {
	// menu,				attb,		Item,				init,	max,					title
	{(PCHAR)TPSelect,		MENU_VALUE,	NULL,				0,		TPSensorMAX,			(PCHAR)TPMenuTitle	},
	{(PCHAR)TPOnOff,		MENU_ITEM,	(PCHAR)OnOffItem,	0,		GetArrayMax(OnOffItem),	(PCHAR)TPMenuTitle	},
	{(PCHAR)TPSizePosi,		MENU_SUB,	(PCHAR)SubItem,		0,		GetArrayMax(SubItem),	(PCHAR)TPMenuTitle	},
	{(PCHAR)DefaultMenu,	MENU_FUNC,	(PCHAR)BlankItem,	0,		GetArrayMax(BlankItem),	(PCHAR)TPMenuTitle	},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,	0,		GetArrayMax(BlankItem),	(PCHAR)TPMenuTitle	},
};


//--------------------------------------------------------------------------------------------------------------------------
//  10-9-1. Misc Tuning
//--------------------------------------------------------------------------------------------------------------------------

// misc(tuning) menu title
static ROMDATA char MiscTuneMenuTitle[][MenuTitleCol]	= {
	"MISC TUNING               ",
	"                          "
};

// misc(tuning) menu
static ROMDATA char YCConversion[][MenuNameCol]	= {{"YC CONVERSION "}};
static ROMDATA char OUTCSCDefine[][MenuNameCol]	= {{"OUTCSC CONTROL"}};
static ROMDATA char Ditheren[][MenuNameCol]		= {{"OUTPUT DITHER "}};
static ROMDATA char USERPLLTune[][MenuNameCol]	= {{"USER PLL TEST "}};
static ROMDATA char IRRemoteEn[][MenuNameCol]		= {{"IR REMOTE     "}};
static ROMDATA char HDMITxMode[][MenuNameCol]		= {{"HDMI TX MODE  "}};
static ROMDATA char V4K30P60PMode[][MenuNameCol]	= {{"4K 30/60P MODE"}};
static ROMDATA char PanCtrlMode[][MenuNameCol]	= {{"PAN CTRL MODE "}};
static ROMDATA char PanCtrlParam[][MenuNameCol]	= {{"PAN CTRL VALUE"}};
static ROMDATA char ChgTestMode[][MenuNameCol]	= {{"CHG TEST MODE "}};

// misc(tuning) menu item
static ROMDATA char YCConverItem[][MenuItemCol] = {
//	"HD0-255   ",  "HD16-235  ",  "HD16-255  ",	"HD0-235   "	
	"BT709 FULL",  "BT709 STD ",  "BT.2020   ",  	"ADOBE RGB "	
};

static ROMDATA char OUTCSCDefineItem[][MenuItemCol] = {
#if defined(__USE_4K60_INOUT__)
	"STD  YCRGB", "FULL YCRGB"
#else
	"FULLBYPASS", "STD BYPASS", "SONY RANGE"
#endif
};

static ROMDATA char USERPLLTuneItem[][MenuItemCol] = {
	"15fr(144M)",  "17fr(166M)",  "20fr(193M)",	"22fr(211M)",
	"25fr(241M)",  "30fr(288M)"
};

static ROMDATA char HDMITxModeItem[][MenuItemCol] = {
	"444(TV)709",	"444(TV)601",	"422(M400) ",	
};

static ROMDATA char PanCtrlModeItem[][MenuItemCol] = {
	"Select..->",	"WBRGainSS1",  "WBBGainSS1",  "WBRGainSS3",
	"WBBGainSS3",
};

#if	defined(__MDIN_i5XX_FPGA__)			// FPGA board
#define 	YCConvInit	1
#define 	OUTCSCDef	1
#elif defined(__MDIN_i540_4K60_V10__)
#define 	YCConvInit	1
#define 	OUTCSCDef	0
#else
#define 	YCConvInit	0
#define 	OUTCSCDef	0
#endif

// misc(tuning) menu info
static ROMDATA OSD_MENU_INFO MiscTuneMenuInfo[] = {
	// menu,				attb,		Item,					init,		max,						title
	{(PCHAR)YCConversion,	MENU_ITEM,	(PCHAR)YCConverItem,	YCConvInit,	GetArrayMax(YCConverItem),	(PCHAR)MiscTuneMenuTitle	},
	{(PCHAR)OUTCSCDefine,	MENU_ITEM,	(PCHAR)OUTCSCDefineItem,	OUTCSCDef,			GetArrayMax(OUTCSCDefineItem),	(PCHAR)MiscTuneMenuTitle	},
	{(PCHAR)Ditheren,		MENU_ITEM,	(PCHAR)OnOffItem,		ON,			GetArrayMax(OnOffItem),		(PCHAR)MiscTuneMenuTitle	},
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	{(PCHAR)USERPLLTune,	MENU_SKIP,	(PCHAR)USERPLLTuneItem,	0,			GetArrayMax(USERPLLTuneItem),	(PCHAR)MiscTuneMenuTitle	},
	{(PCHAR)IRRemoteEn,		MENU_SKIP,	(PCHAR)OnOffItem,		ON,			GetArrayMax(OnOffItem),		(PCHAR)MiscTuneMenuTitle	},
	{(PCHAR)HDMITxMode,		MENU_SKIP,	(PCHAR)HDMITxModeItem,	0,			GetArrayMax(HDMITxModeItem),	(PCHAR)MiscTuneMenuTitle	},
	{(PCHAR)PanCtrlMode,	MENU_SKIP,	(PCHAR)PanCtrlModeItem,	0,			GetArrayMax(PanCtrlModeItem),	(PCHAR)MiscTuneMenuTitle	},
	{(PCHAR)PanCtrlParam,	MENU_SKIP,	NULL,					125,		250,							(PCHAR)MiscTuneMenuTitle	},
#else								// MDIN-i51X (i510, i540, i550)
#if (defined(__USE_ISP_SCLK_OUT__) && !defined(__USE_4K60_INOUT__) && !defined(__MDIN_i550_USB_V100__))
#if defined(__USE_IMX226_4K15P_MODE__)
	{(PCHAR)USERPLLTune,	MENU_ITEM,	(PCHAR)USERPLLTuneItem,	0,			GetArrayMax(USERPLLTuneItem),	(PCHAR)MiscTuneMenuTitle	},
#else
	{(PCHAR)USERPLLTune,	MENU_ITEM,	(PCHAR)USERPLLTuneItem,	5,			GetArrayMax(USERPLLTuneItem),	(PCHAR)MiscTuneMenuTitle	},
#endif
#else
	{(PCHAR)USERPLLTune,	MENU_SKIP,	(PCHAR)USERPLLTuneItem,	5,			GetArrayMax(USERPLLTuneItem),	(PCHAR)MiscTuneMenuTitle	},
#endif
#if defined(__USE_IRKEY_SWITCH__)
	{(PCHAR)IRRemoteEn,		MENU_ITEM,	(PCHAR)OnOffItem,		OFF,		GetArrayMax(OnOffItem),		(PCHAR)MiscTuneMenuTitle	},
#else
	{(PCHAR)IRRemoteEn,		MENU_ITEM,	(PCHAR)OnOffItem,		ON,			GetArrayMax(OnOffItem),		(PCHAR)MiscTuneMenuTitle	},
#endif
	{(PCHAR)HDMITxMode,		MENU_ITEM,	(PCHAR)HDMITxModeItem,	0,			GetArrayMax(HDMITxModeItem),	(PCHAR)MiscTuneMenuTitle	},
#if defined(__USE_4K60_INOUT__)
	{(PCHAR)V4K30P60PMode,	MENU_ITEM,	(PCHAR)OnOffItem,		0,			GetArrayMax(OnOffItem),		(PCHAR)MiscTuneMenuTitle	},
#elif defined(__PANORAMA_SETUP__)
	{(PCHAR)PanCtrlMode,	MENU_ITEM,	(PCHAR)PanCtrlModeItem,	0,			GetArrayMax(PanCtrlModeItem),	(PCHAR)MiscTuneMenuTitle	},
	{(PCHAR)PanCtrlParam,	MENU_VALUE,	NULL,					125,		250,							(PCHAR)MiscTuneMenuTitle	},
#else
	{(PCHAR)ChgTestMode,	MENU_FUNC,	(PCHAR)TestOkItem,		0,			GetArrayMax(TestOkItem),		(PCHAR)MiscTuneMenuTitle	},
#endif
#endif
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,		0,			GetArrayMax(BlankItem),		(PCHAR)MiscTuneMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
//  10-11. WDR (common define/string)
//--------------------------------------------------------------------------------------------------------------------------
static ROMDATA char WDRFramView[][MenuNameCol] = {{"FRAME VIEW SEL"}};
static ROMDATA char WDRStrength[][MenuNameCol] = {{"WDR STRENGTH  "}};
#if WDR_TM_SIZE_CTRL
static ROMDATA char WDRContrast[][MenuNameCol] = {{"LOCAL CONTRAST"}};
#else
static ROMDATA char WDRGhostRem[][MenuNameCol] = {{"DEBLUR&DEGHOST"}};
#endif
static ROMDATA char WDRAutoExpo[][MenuNameCol] = {{"WDR AE MODE   "}};
static ROMDATA char WDRAETarget[][MenuNameCol] = {{"AE BRIGHT     "}};
static ROMDATA char WDRSensGain[][MenuNameCol] = {{"AGC           "}};
static ROMDATA char WDRLensIris[][MenuNameCol] = {{"IRIS          "}};
static ROMDATA char WDRSensShut[][MenuNameCol] = {{"SHUTTER       "}};
static ROMDATA char WDRExpRatio[][MenuNameCol] = {{"EXPOSURE RATIO"}};

// "AUTO EXPOSURE "
static ROMDATA char xWDRSetWDRAutoItem[][MenuItemCol]	= {
	"MANUAL    ", "AUTO      "
};


//--------------------------------------------------------------------------------------------------------------------------
//  10-11-a. L3WDR - Line by Line (3-Frame)
//--------------------------------------------------------------------------------------------------------------------------

// wdr(line mode 3frame) menu title
static ROMDATA char L3WDRSetMenuTitle[][MenuTitleCol]	= {
	" WDR (Line by Line 3Frame)", "                          "
};

// wdr(line mode 3frame) menu item
// "FRAME VIEW SEL"
static ROMDATA char L3WDRSetWDRViewItem[][MenuItemCol]	= {
	"WDR ON    ", "LONG      ", "SHORT     ", "SHORTER   "
};

// "SHUTTER RATIO"
static ROMDATA char L3WDRSetExpRtoItem[][MenuItemCol]	= {
	"1:6:36    ", "1:8:64    "
};

// wdr(line mode 3frame) menu info
static ROMDATA OSD_MENU_INFO L3WDRSetMenuInfo[] = {
	// menu,				attb,		Item,						init,					max,								title
	{(PCHAR)WDRFramView,	MENU_ITEM,	(PCHAR)L3WDRSetWDRViewItem,	MISP_WDR_FVIEW_WDR,		GetArrayMax(L3WDRSetWDRViewItem),	(PCHAR)L3WDRSetMenuTitle	},
#if WDR_USE_MANU_TM
	{(PCHAR)WDRStrength,	MENU_VALUE,	NULL,						12,						29,									(PCHAR)L3WDRSetMenuTitle	},
#else
	{(PCHAR)WDRStrength,	MENU_VALUE,	NULL,						3,						8,									(PCHAR)L3WDRSetMenuTitle	},
#endif
#if WDR_TM_SIZE_CTRL
	{(PCHAR)WDRContrast,	MENU_VALUE,	NULL,						2,						5,									(PCHAR)L3WDRSetMenuTitle	},
#else
	{(PCHAR)WDRGhostRem,	MENU_VALUE,	NULL,						7,						14,									(PCHAR)L3WDRSetMenuTitle	},
#endif
	{(PCHAR)WDRAutoExpo,	MENU_ITEM,	(PCHAR)xWDRSetWDRAutoItem,	MISP_WDR_AE_AUTO,		GetArrayMax(xWDRSetWDRAutoItem),	(PCHAR)L3WDRSetMenuTitle	},
	{(PCHAR)WDRAETarget,	MENU_VALUE,	NULL,						10,						19,									(PCHAR)L3WDRSetMenuTitle	},
	{(PCHAR)WDRSensGain,	MENU_VALUE,	NULL,						0,						19,									(PCHAR)L3WDRSetMenuTitle	},
	{(PCHAR)WDRLensIris,	MENU_VALUE,	NULL,						19,						19,									(PCHAR)L3WDRSetMenuTitle	},
	{(PCHAR)WDRSensShut,	MENU_VALUE,	NULL,						19,						19,									(PCHAR)L3WDRSetMenuTitle	},
#if L3WDR_HOLD_RATIO
	{(PCHAR)WDRExpRatio,	MENU_SKIP,	(PCHAR)L3WDRSetExpRtoItem,	1,						GetArrayMax(L3WDRSetExpRtoItem),	(PCHAR)L3WDRSetMenuTitle	},
#else
	{(PCHAR)WDRExpRatio,	MENU_ITEM,	(PCHAR)L3WDRSetExpRtoItem,	1,						GetArrayMax(L3WDRSetExpRtoItem),	(PCHAR)L3WDRSetMenuTitle	},
#endif
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,			0,						GetArrayMax(BlankItem),				(PCHAR)L3WDRSetMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
//  10-11-b. L2WDR - Line by Line (2-Frame)
//--------------------------------------------------------------------------------------------------------------------------

// wdr(line mode 4frame) menu title
static ROMDATA char L2WDRSetMenuTitle[][MenuTitleCol]	= {
	" WDR (Line by Line 2Frame)", "                          "
};

// wdr(line mode 4frame) menu item
// "FRAME VIEW SEL"
static ROMDATA char L2WDRSetWDRViewItem[][MenuItemCol]	= {
	"WDR ON    ", "LONG      ", "SHORT     "
};

// "SHUTTER RATIO"
static ROMDATA char L2WDRSetExpRtoItem[][MenuItemCol]	= {
	"1:16      ", "1:32      "
};

// wdr(line mode 2frame) menu info
static ROMDATA OSD_MENU_INFO L2WDRSetMenuInfo[] = {
	// menu,				attb,		Item,						init,					max,								title
	{(PCHAR)WDRFramView,	MENU_ITEM,	(PCHAR)L2WDRSetWDRViewItem,	MISP_WDR_FVIEW_WDR,		GetArrayMax(L2WDRSetWDRViewItem),	(PCHAR)L2WDRSetMenuTitle	},
#if WDR_USE_MANU_TM
	{(PCHAR)WDRStrength,	MENU_VALUE, NULL,						12,						29,									(PCHAR)L2WDRSetMenuTitle	},
#else
	{(PCHAR)WDRStrength,	MENU_VALUE, NULL,						3,						8,									(PCHAR)L2WDRSetMenuTitle	},
#endif
#if WDR_TM_SIZE_CTRL
	{(PCHAR)WDRContrast,	MENU_VALUE,	NULL,						2,						5,									(PCHAR)L2WDRSetMenuTitle	},
#else
	{(PCHAR)WDRGhostRem,	MENU_SKIP,	NULL,						0,						14,									(PCHAR)L2WDRSetMenuTitle	},
#endif
	{(PCHAR)WDRAutoExpo,	MENU_ITEM,	(PCHAR)xWDRSetWDRAutoItem,	MISP_WDR_AE_AUTO,		GetArrayMax(xWDRSetWDRAutoItem),	(PCHAR)L2WDRSetMenuTitle	},
	{(PCHAR)WDRAETarget,	MENU_VALUE,	NULL,						10,						19,									(PCHAR)L2WDRSetMenuTitle	},
	{(PCHAR)WDRSensGain,	MENU_VALUE,	NULL,						0,						19,									(PCHAR)L2WDRSetMenuTitle	},
	{(PCHAR)WDRLensIris,	MENU_VALUE, NULL,						19,						19,									(PCHAR)L2WDRSetMenuTitle	},
	{(PCHAR)WDRSensShut,	MENU_VALUE, NULL,						19,						19,									(PCHAR)L2WDRSetMenuTitle	},
#if L2WDR_HOLD_RATIO
	{(PCHAR)WDRExpRatio,	MENU_SKIP,	(PCHAR)L2WDRSetExpRtoItem,	1,						GetArrayMax(L2WDRSetExpRtoItem),	(PCHAR)L2WDRSetMenuTitle	},
#else
	{(PCHAR)WDRExpRatio,	MENU_ITEM,	(PCHAR)L2WDRSetExpRtoItem,	1,						GetArrayMax(L2WDRSetExpRtoItem),	(PCHAR)L2WDRSetMenuTitle	},
#endif
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,			0,						GetArrayMax(BlankItem),				(PCHAR)L2WDRSetMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
//  10-11-c. TMWDR - Tone map only
//--------------------------------------------------------------------------------------------------------------------------

// wdr(tone map) menu title
static ROMDATA char TMWDRSetMenuTitle[][MenuTitleCol]	= {
	" WDR (Tone-map only)      ", "                          "
};

// wdr(tone map) menu item
// "FRAME VIEW SEL"
static ROMDATA char TMWDRSetWDRViewItem[][MenuItemCol]	= {
	"WDR ON    ", "DISP[11:0]", "DISP[13:2]", "DISP[15:4]"
};

// wdr(tone map) menu info
static ROMDATA OSD_MENU_INFO TMWDRSetMenuInfo[] = {
	// menu,				attb,		Item,						init,					max,								title
	{(PCHAR)WDRFramView,	MENU_ITEM,	(PCHAR)TMWDRSetWDRViewItem,	MISP_WDR_FVIEW_WDR,		GetArrayMax(TMWDRSetWDRViewItem),	(PCHAR)TMWDRSetMenuTitle	},
#if WDR_USE_MANU_TM
	{(PCHAR)WDRStrength,	MENU_VALUE, NULL,						12,						29,									(PCHAR)TMWDRSetMenuTitle	},
#else
	{(PCHAR)WDRStrength,	MENU_VALUE, NULL,						3,						8,									(PCHAR)TMWDRSetMenuTitle	},
#endif
#if WDR_TM_SIZE_CTRL
	{(PCHAR)WDRContrast,	MENU_VALUE,	NULL,						3,						5,									(PCHAR)TMWDRSetMenuTitle	},
#endif
	{(PCHAR)WDRAutoExpo,	MENU_ITEM,	(PCHAR)xWDRSetWDRAutoItem,	MISP_WDR_AE_AUTO,		GetArrayMax(xWDRSetWDRAutoItem),	(PCHAR)TMWDRSetMenuTitle	},
	{(PCHAR)WDRAETarget,	MENU_VALUE,	NULL,						10,						19,									(PCHAR)TMWDRSetMenuTitle	},
	{(PCHAR)WDRSensGain,	MENU_VALUE,	NULL,						0,						19,									(PCHAR)TMWDRSetMenuTitle	},
	{(PCHAR)WDRLensIris,	MENU_VALUE,	NULL,						19,						19,									(PCHAR)TMWDRSetMenuTitle	},
	{(PCHAR)WDRSensShut,	MENU_VALUE,	NULL,						19,						19,									(PCHAR)TMWDRSetMenuTitle	},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,			0,						GetArrayMax(BlankItem),				(PCHAR)TMWDRSetMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
//  10-11-d. FRWDR - Frame by Frame
//--------------------------------------------------------------------------------------------------------------------------

// wdr(frame mode) menu title
static ROMDATA char FRWDRSetMenuTitle[][MenuTitleCol]	= {
	" WDR (Frame by Frame Mode)",
	"                          "
};

// wdr(frame mode) menu item
// "FRAME VIEW SEL"
static ROMDATA char FRWDRSetWDRViewItem[][MenuItemCol]	= {
	"WDR ON    ", "LONG      ", "SHORT     "
};

// "SHUTTER RATIO"
static ROMDATA char FRWDRSetExpRtoItem[][MenuItemCol]	= {
	"1:16      ", "1:32      "
};

// wdr(frame mode) menu info
static ROMDATA OSD_MENU_INFO FRWDRSetMenuInfo[] = {
	// menu,				attb,		Item,						init,					max,								title
	{(PCHAR)WDRFramView,	MENU_ITEM,	(PCHAR)FRWDRSetWDRViewItem,	MISP_WDR_FVIEW_WDR,		GetArrayMax(FRWDRSetWDRViewItem),	(PCHAR)FRWDRSetMenuTitle	},
#if WDR_USE_MANU_TM
	{(PCHAR)WDRStrength,	MENU_VALUE, NULL,						12,						29,									(PCHAR)FRWDRSetMenuTitle	},
#else
	{(PCHAR)WDRStrength,	MENU_VALUE, NULL,						3,						8,									(PCHAR)FRWDRSetMenuTitle	},
#endif
#if WDR_TM_SIZE_CTRL
	{(PCHAR)WDRContrast,	MENU_VALUE,	NULL,						2,						5,									(PCHAR)FRWDRSetMenuTitle	},
#else
	{(PCHAR)WDRGhostRem,	MENU_SKIP,	NULL,						0,						14,									(PCHAR)FRWDRSetMenuTitle	},
#endif
	{(PCHAR)WDRAutoExpo,	MENU_ITEM,	(PCHAR)xWDRSetWDRAutoItem,	MISP_WDR_AE_AUTO,		GetArrayMax(xWDRSetWDRAutoItem),	(PCHAR)FRWDRSetMenuTitle	},
	{(PCHAR)WDRAETarget,	MENU_VALUE,	NULL,						10,						19,									(PCHAR)FRWDRSetMenuTitle	},
	{(PCHAR)WDRSensGain,	MENU_VALUE,	NULL,						0,						19,									(PCHAR)FRWDRSetMenuTitle	},
	{(PCHAR)WDRLensIris,	MENU_VALUE,	NULL,						19,						19,									(PCHAR)FRWDRSetMenuTitle	},
	{(PCHAR)WDRSensShut,	MENU_VALUE,	NULL,						19,						19,									(PCHAR)FRWDRSetMenuTitle	},
#if defined(__USE_MDIN_i500__)  // MDIN-i500 -- default 1:16
	{(PCHAR)WDRExpRatio,	MENU_ITEM,	(PCHAR)FRWDRSetExpRtoItem,	0,						GetArrayMax(FRWDRSetExpRtoItem),	(PCHAR)FRWDRSetMenuTitle 	},
#else 							// MDIN-i51X (i510, i540, i550)
	{(PCHAR)WDRExpRatio,	MENU_ITEM,	(PCHAR)FRWDRSetExpRtoItem,	1,						GetArrayMax(FRWDRSetExpRtoItem),	(PCHAR)FRWDRSetMenuTitle	},
#endif
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,			0,						GetArrayMax(BlankItem),				(PCHAR)FRWDRSetMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
//  10-11-e. DGWDR - Single Frame Digital WDR
//--------------------------------------------------------------------------------------------------------------------------

// wdr(single mode) menu title
static ROMDATA char DGWDRSetMenuTitle[][MenuTitleCol]	= {
	" WDR (Single Frame Mode)  ",
	"                          "
};

// wdr(single mode) menu info
static ROMDATA OSD_MENU_INFO DGWDRSetMenuInfo[] = {
	// menu,				attb,		Item,						init,					max
#if WDR_USE_MANU_TM
	{(PCHAR)WDRStrength,	MENU_VALUE, NULL,						12,						29,						(PCHAR)DGWDRSetMenuTitle	},
#else
	{(PCHAR)WDRStrength,	MENU_VALUE, NULL,						3,						8,						(PCHAR)DGWDRSetMenuTitle	},
#endif
#if WDR_TM_SIZE_CTRL
	{(PCHAR)WDRContrast,	MENU_VALUE,	NULL,						3,						5,						(PCHAR)DGWDRSetMenuTitle	},
#endif
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,			0,						GetArrayMax(BlankItem),	(PCHAR)DGWDRSetMenuTitle	},
};


//--------------------------------------------------------------------------------------------------------------------------
//  10-12-1. DNR Tuning
//--------------------------------------------------------------------------------------------------------------------------

// dnr(tuning) menu title
static ROMDATA char DNRTuneMenuTitle[][MenuTitleCol]	= {
	" DNR TUNING               ",
	"                          "
};

// dnr(tuning) menu
static ROMDATA char DNRDemoMode[][MenuNameCol]			= {{"DEMO_MODE     "}};
static ROMDATA char DNRLowLightLev[][MenuNameCol]		= {{"NR STATUS     "}};
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
static ROMDATA char DNRM2DMode[][MenuNameCol]			= {{"NR Middle2D   "}};
static ROMDATA char DNRL2DMode[][MenuNameCol]			= {{"NR Last2D     "}};
#endif

// dnr(tuning) menu item
static ROMDATA char DNRDemoModeItem[][MenuItemCol]	= {
	"OFF       ",	"LEFT ON   ",	"RIGHT ON  "
};

static ROMDATA char DNRLowModeItem[][MenuItemCol]	= {
	"NORMAL    ",	"LOW LIGHT "
};

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
static ROMDATA char DNRM2DModeItem[][MenuItemCol]	= {
	"OFF       ",	"LOW       ",	"MID       ",	"HIGH      "
};

static ROMDATA char DNRL2DModeItem[][MenuItemCol]	= {
	"OFF       ",	"LOW       ",	"MID       ",	"HIGH      "
};
#endif

// dnr(tuning) menu info
static ROMDATA OSD_MENU_INFO DNRTuneMenuInfo[] = {
	// menu,				attb,		Item,					init,				max,							title
	{(PCHAR)DNRDemoMode,	MENU_ITEM,	(PCHAR)DNRDemoModeItem,	0,					GetArrayMax(DNRDemoModeItem),	(PCHAR)DNRTuneMenuTitle	},
	{(PCHAR)DNRLowLightLev,	MENU_FUNC,	(PCHAR)DNRLowModeItem,	0,					GetArrayMax(DNRLowModeItem),	(PCHAR)DNRTuneMenuTitle	},
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	{(PCHAR)DNRM2DMode,		MENU_ITEM,	(PCHAR)DNRM2DModeItem,	1,					GetArrayMax(DNRM2DModeItem),	(PCHAR)DNRTuneMenuTitle	},
	{(PCHAR)DNRL2DMode,		MENU_ITEM,	(PCHAR)DNRL2DModeItem,	0,					GetArrayMax(DNRL2DModeItem),	(PCHAR)DNRTuneMenuTitle	},
#endif
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,		0,					GetArrayMax(BlankItem),			(PCHAR)DNRTuneMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 8-6-2-1. BLC Size/Position Menu
//--------------------------------------------------------------------------------------------------------------------------

// blc size position menu info
static ROMDATA OSD_MENU_INFO BLCSizeMenuInfo[] = {
	// menu,				attb,		Item,				init(x),	max(x)
#if defined(__USE_MDIN_i500__)						// MDIN-i500
	{(PCHAR)PositionMenu,	MENU_VALUE,	NULL,				15,			48						},
	{(PCHAR)SizeMenu,		MENU_VALUE,	NULL,				32,			48						},
#else 												// MDIN-i51X (i510, i540, i550)
	{(PCHAR)PositionMenu,	MENU_VALUE,	NULL,				15,			96						},
	{(PCHAR)SizeMenu,		MENU_VALUE,	NULL,				32,			96						},
#endif
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,	0,			GetArrayMax(BlankItem)	},
};

// blc size position (y) 
static ROMDATA BYTE BLCSizeYInit[] 	= {11, 22};		// position(Y), size(Y)
#if defined(__USE_MDIN_i500__)							// MDIN-i500
static ROMDATA BYTE BLCSizeYMax[] 		= {33, 33};		// position(Y), size(Y)
#else 													// MDIN-i51X (i510, i540, i550)
static ROMDATA BYTE BLCSizeYMax[] 		= {67, 67};		// position(Y), size(Y)
#endif


//--------------------------------------------------------------------------------------------------------------------------
// 8-6-3-1. HSBLC Size/Position Menu
//--------------------------------------------------------------------------------------------------------------------------

// hsblc size position menu info
static ROMDATA OSD_MENU_INFO HSBLCSizeMenuInfo[] = {
	// menu,				attb,		Item,				init(x),	max(x)
#if defined(__USE_MDIN_i500__)						// MDIN-i500
	{(PCHAR)PositionMenu,	MENU_VALUE,	NULL,				15,			48						},
	{(PCHAR)SizeMenu,		MENU_VALUE,	NULL,				32,			48						},
#else 												// MDIN-i51X (i510, i540, i550)
	{(PCHAR)PositionMenu,	MENU_VALUE,	NULL,				15,			96						},
	{(PCHAR)SizeMenu,		MENU_VALUE,	NULL,				32,			96						},
#endif
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,	0,			GetArrayMax(BlankItem)	},
};

// hsblc size position (y) 
static ROMDATA BYTE HSBLCSizeYInit[] 	= {11, 22};		// position(Y), size(Y)
#if defined(__USE_MDIN_i500__)							// MDIN-i500
static ROMDATA BYTE HSBLCSizeYMax[] 	= {33, 33};		// position(Y), size(Y)
#else 													// MDIN-i51X (i510, i540, i550)
static ROMDATA BYTE HSBLCSizeYMax[] 	= {67, 67};		// position(Y), size(Y)
#endif

//--------------------------------------------------------------------------------------------------------------------------
// 8-4-4-1. Motion Detect Size/Position Menu
//--------------------------------------------------------------------------------------------------------------------------

// md size position menu info
static ROMDATA OSD_MENU_INFO MDSizeMenuInfo[] = {
	// menu,				attb,		Item,				init(x),	max(x)
	{(PCHAR)PositionMenu,	MENU_VALUE,	NULL,				0,			255						},
	{(PCHAR)SizeMenu,		MENU_VALUE,	NULL,				0,			255						},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,	0,			GetArrayMax(BlankItem)	},
};

// md size position (y) 
static ROMDATA BYTE MDSizeYInit[] 	= {0, 0};		// position(Y), size(Y)
static ROMDATA BYTE MDSizeYMax[] 	= {134, 135};		// position(Y), size(Y)


//--------------------------------------------------------------------------------------------------------------------------
// 8-1-1-1. D-ZOOM Ratio/Position Menu
//--------------------------------------------------------------------------------------------------------------------------

// dzoom ratio menu
static ROMDATA char DZRatMenu[][MenuNameCol]		= {"RATIO         "};

// dzoom ratio menu info
static ROMDATA OSD_MENU_INFO DZCtrlMenuInfo[] = {
	// menu,				attb,		Item,				init(x),	max(x)
	{(PCHAR)DZRatMenu,		MENU_VALUE,	NULL,				0,			50										},
	{(PCHAR)PositionMenu,	MENU_VALUE,	NULL,				120,		240										},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,	0,			GetArrayMax(BlankItem)	},
};

// dzoom ratio & position (y) 
static ROMDATA BYTE DZCtrlYInit[] 	= {0, 120};			// ratio(Y), position(Y)
static ROMDATA BYTE DZCtrlYMax[] 	= {50, 240};		// ratio(Y), Position(Y), step:(Full - zoom)/120


//--------------------------------------------------------------------------------------------------------------------------
// 8-1-4-1. PIP Position Menu
//--------------------------------------------------------------------------------------------------------------------------

// pip size position menu info
static ROMDATA OSD_MENU_INFO PIPPosiMenuInfo[] = {
	// menu,				attb,		Item,				init(x),	max(x)
	{(PCHAR)PositionMenu,	MENU_VALUE,	NULL,				10,			10						},
//	{(PCHAR)SizeMenu,		MENU_VALUE,	NULL,				0,			60						},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,	0,			GetArrayMax(BlankItem)	},
};

// pip size position (y) 
static ROMDATA BYTE PIPPosiYInit[]	= {10};				// position(Y)
static ROMDATA BYTE PIPPosiYMax[] 	= {10};				// position(Y)


//--------------------------------------------------------------------------------------------------------------------------
// 10-7-3-1. Sensor TP Size/Position Menu
//--------------------------------------------------------------------------------------------------------------------------

// sensor tp size position menu info
static ROMDATA OSD_MENU_INFO TPSizeMenuInfo[] = {
	// menu,				attb,		Item,				init(x),	max(x)
	{(PCHAR)PositionMenu,	MENU_VALUE,	NULL,				0,			60						},
	{(PCHAR)SizeMenu,		MENU_VALUE,	NULL,				60,			60						},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,	0,			GetArrayMax(BlankItem)	},
};

// sensor tp size position (y) 
static ROMDATA BYTE TPSizeYInit[] = {0, 34};				// position(Y), size(Y)
static ROMDATA BYTE TPSizeYMax[] 	= {34, 34};				// position(Y), size(Y)


//--------------------------------------------------------------------------------------------------------------------------
// 12-1.Debug Menu
//--------------------------------------------------------------------------------------------------------------------------

// debug menu title
static ROMDATA char DebugMenuTitle[][MenuTitleCol]	= {
	"DEBUG MENU                ", "                          "
};

// debug menu
static ROMDATA char DebugMode[][MenuNameCol]		= {{"DEBUG MODE    "}};
static ROMDATA char AgingTest[][MenuNameCol]		= {{"AGING TEST    "}};
static ROMDATA char VectorScope[][MenuNameCol]		= {{"VECTOR SCOPE  "}};
static ROMDATA char FactorySetup[][MenuNameCol]		= {{"FACTORY SETUP "}};

// debug menu item
static ROMDATA char DebugModeItem[][MenuItemCol] = {
	"OFF       ",	"AE        ",	"AF        ",	"AWB       ",
	"AE CON    ",	"AF  CON   ",	"AWB CON   "
};

// debug menu info
static ROMDATA OSD_MENU_INFO DebugMenuInfo[] = {
	// menu,			attb,			Item,					init,		max,						title
	{(PCHAR)DebugMode,	MENU_ITEM,		(PCHAR)DebugModeItem,	0,			GetArrayMax(DebugModeItem),	(PCHAR)DebugMenuTitle	},
	{(PCHAR)AgingTest,	MENU_ITEM,		(PCHAR)OnOffItem,		0,			GetArrayMax(OnOffItem),		(PCHAR)DebugMenuTitle	},
	{(PCHAR)VectorScope,MENU_SUB,		(PCHAR)OnOffSub,		0,			GetArrayMax(OnOffSub),		(PCHAR)DebugMenuTitle	},
	{(PCHAR)FactorySetup,MENU_SUB,		(PCHAR)SubItem,			0,			GetArrayMax(SubItem),		(PCHAR)DebugMenuTitle	},
	{(PCHAR)CCorrect,	MENU_SUB,		(PCHAR)SubItem,			0,			GetArrayMax(SubItem),		(PCHAR)DebugMenuTitle	},	
	{(PCHAR)RetMenu,	MENU_FUNC,		(PCHAR)BlankItem,		0,			GetArrayMax(BlankItem),		(PCHAR)DebugMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 12-3-1.vector scope
//--------------------------------------------------------------------------------------------------------------------------

// vector scope menu title
static ROMDATA char VSMenuTitle[][MenuTitleCol]	= {
	"VECTOR SCOPE              ", "                          "
};

// vector scope menu
static ROMDATA char VSArea[][MenuNameCol]		= {{"SCAN AREA     "}};
static ROMDATA char VSPosiX[][MenuNameCol]		= {{" POSITION-X   "}};
static ROMDATA char VSPosiY[][MenuNameCol]		= {{" POSITION-Y   "}};
static ROMDATA char VSSizeX[][MenuNameCol]		= {{" SIZE-X       "}};
static ROMDATA char VSSizeY[][MenuNameCol]		= {{" SIZE-Y       "}};
static ROMDATA char VSDraw[][MenuNameCol]		= {{"DRAW SCOPE    "}};

// vector scope menu item
static ROMDATA char VSAreaItem[][MenuItemCol] = {
	"FULL      ",	"USER AREA "
};

// vector scope menu info
static ROMDATA OSD_MENU_INFO VSMenuInfo[] = {
	// menu,			attb,			Item,					init,		max,						title
	{(PCHAR)VSArea,		MENU_ITEM,		(PCHAR)VSAreaItem,		0,			GetArrayMax(VSAreaItem),	(PCHAR)VSMenuTitle	},
	{(PCHAR)VSPosiX,	MENU_VALUE,		NULL,					0,			128,						(PCHAR)VSMenuTitle	},
	{(PCHAR)VSPosiY,	MENU_VALUE,		NULL,					0,			68,							(PCHAR)VSMenuTitle	},
	{(PCHAR)VSSizeX,	MENU_VALUE,		NULL,					0,			128,						(PCHAR)VSMenuTitle	},
	{(PCHAR)VSSizeY,	MENU_VALUE,		NULL,					0,			68,							(PCHAR)VSMenuTitle	},
	{(PCHAR)VSDraw,		MENU_FUNC,		(PCHAR)WaitOkItem,		0,			GetArrayMax(WaitOkItem),	(PCHAR)VSMenuTitle	},
	{(PCHAR)RetMenu,	MENU_EXIT,		(PCHAR)BlankItem,		0,			GetArrayMax(BlankItem),		(PCHAR)VSMenuTitle	},
};


//--------------------------------------------------------------------------------------------------------------------------
// 12-4-1.factory setup
//--------------------------------------------------------------------------------------------------------------------------

// factory setup menu title
static ROMDATA char FSMenuTitle[][MenuTitleCol]	= {
	"FACTORY SETUP             ", "                          "
};

// factory setup menu
static ROMDATA char PwmIrisTune[][MenuNameCol]	= {{"PWM IRIS TUNE "}};
static ROMDATA char PwmIrisSpeed[][MenuNameCol]	= {{"PWM IRIS SPEED"}};

// factory setup menu item

// factory setup menu info
static ROMDATA OSD_MENU_INFO FSMenuInfo[] = {
	// menu,				attb,		Item,					init,		max,						title
	{(PCHAR)PwmIrisTune,	MENU_FUNC,	(PCHAR)WaitOkItem,		0,			GetArrayMax(WaitOkItem),	(PCHAR)FSMenuTitle	},
	{(PCHAR)PwmIrisSpeed,	MENU_VALUE,	NULL,					3,			5,							(PCHAR)FSMenuTitle	},
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,		0,			GetArrayMax(BlankItem),		(PCHAR)FSMenuTitle	},
};

//--------------------------------------------------------------------------------------------------------------------------
// 12-5-1.Color Correction Fine tune setup
//--------------------------------------------------------------------------------------------------------------------------
// Title : share ColorCorrMenuTitle
static ROMDATA char CCFtuneMenuTitle[][MenuTitleCol]	= {
	"COLOR CORRECTION TUNING   ",
	"                          "
};

static ROMDATA char CCFTypeSel[][MenuNameCol]		= {{"CCF TYPE SEL  "}};
static ROMDATA char CCFGain[][MenuNameCol]	= {{"CCF GAIN      "}};
static ROMDATA char CCFBalance[][MenuNameCol]	= {{"CCF BALANCE   "}};
static ROMDATA char CCFTypeApply[][MenuNameCol]		= {{"CCF T.APPLIED "}};
static ROMDATA char CCFUartPrint[][MenuNameCol]		= {{"CCF PRINT     "}};

static ROMDATA char CCFTypeSeltem[][MenuItemCol] = {
	"R LINE Ty1", "R LINE Ty2", "G LINE Ty1", "B LINE Ty1", "B LINE Ty2"
};
static ROMDATA char CCFTypeApplyItem[][MenuItemCol] = {
	"R1G1B1    ", "R1G1B2    ", "R2G1B1    ", "R2G1B2    "
};


// Color Correction menu info
static ROMDATA OSD_MENU_INFO CCFtuneMenuInfo[] = {
	// menu,						attb,		Item,				init,				max,						title
	{(PCHAR)CCFTypeSel,		MENU_ITEM,	(PCHAR)CCFTypeSeltem,		0,		GetArrayMax(CCFTypeSeltem),		(PCHAR)CCFtuneMenuTitle	},	
	{(PCHAR)CCFGain,			MENU_VALUE,	 NULL,				0,				100,					(PCHAR)CCFtuneMenuTitle	},
	{(PCHAR)CCFBalance,		MENU_VALUE,	 NULL,				50,				100,					(PCHAR)CCFtuneMenuTitle	},	
	{(PCHAR)CCFTypeApply,	MENU_ITEM,	(PCHAR)CCFTypeApplyItem,		0,		GetArrayMax(CCFTypeApplyItem),		(PCHAR)CCFtuneMenuTitle	},		
	{(PCHAR)CCFUartPrint,	MENU_FUNC,	(PCHAR)WaitOkItem,	0,		GetArrayMax(WaitOkItem),	(PCHAR)CCFtuneMenuTitle	},	
	{(PCHAR)RetMenu,		MENU_EXIT,	(PCHAR)BlankItem,	0,		GetArrayMax(BlankItem),		(PCHAR)CCFtuneMenuTitle	},
};
#endif	/* __OSDMENU_H__ */
