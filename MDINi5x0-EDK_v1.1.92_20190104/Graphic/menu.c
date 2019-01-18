//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	MENU.C
// Description 		:
// Ref. Docment		: MDIN380 DDK v2.52
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<stdarg.h>
#include	"osd.h"
#include	"video.h"


#if defined(__USE_CHN_FONT__)
#include	"osdmenu_eng_chn.h"
#else
#include	"osdmenu.h"
#endif

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
#define		INC			1
#define		DEC			0

#if CPU_ALIGN_BIG_ENDIAN == 1
#define		pID(x)		pID[((x)? 1:0)]
#else
#define		pID(x)		pID[((x)? 0:1)]
#endif

#define 	GetWeight(x)		((4-(x))*4)					// convert level(1~4) to weight(12,8,4,0)
#define 	MenuTreeMask(x)		(0x000F << GetWeight(x))

// define osd menu tree
// Level 1
#define		MAIN_TREE		0xF000

// Level 2
#define		FOCUS_TREE		0x1F00
#define		EXPOSURE_TREE	0x2F00
#define 	WB_TREE			0x3F00
#define		DYNT_TREE		0x4F00
#define		DNR_TREE		0x5F00
#define		PM_TREE			0x6F00
#define		EFFECT_TREE		0x7F00
#define		SPECIAL_TREE	0x8F00
#define		SETUP_TREE		0x9F00
#define		ENGR_TREE		0xAF00
#define		DEBUG_TREE		0xCF00

// Level 3
#define		CONFER_TREE		0x17F0
#define		GMC_TREE		0x72F0
#define		DZ_TREE			0x81F0
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)
 #define		DEFOGDWDR_TREE	0x82F0
#else
 #define		DEFOG_TREE		0x82F0
 #define		DWDR_TREE		0x83F0
#endif
#define		MD_TREE			0x84F0
#define		BLC_TREE		0x86F0
#define		HSBLC_TREE		0x86F0
#define		DPIXEL_TREE		0x88F0
#define		DISPFUNC_TREE	0x89F0

#define		CAMTITLE_TREE	0x91F0
#define		RS485_TREE		0x93F0
#define		MENUCOLOR_TREE	0x95F0
#define		FRESET_TREE		0x96F0
#define 	SYSINFO_TREE	0x97F0
#define 	OUTFRMT_TREE	0x99F0
#define		COLOR_CORR_TREE	0xA3F0
#define		TP_TREE			0xA6F0
#define		MISC_TUNE_TREE	0xA9F0
#define		WDR_TUNE_TREE	0xAAF0
#define		DNR_TUNE_TREE	0xABF0
#define		VS_TREE			0xC3F0
#define		FS_TREE			0xC4F0
#define		CCF_TREE		0xC5F0

// Level 4
#define		DZRATIO_TREE	0x811F
#define		PIPPOSI_TREE	0x814F
#define		MDSIZE_TREE		0x844F
#define		BLCSIZE_TREE	0x862F
#define		HSBLCSIZE_TREE	0x863F
#define		TPSIZE_TREE		0xA63F

#define 	TEXT_CURSOR		'_'

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

static WORD OSDMenuID, OSDCombID, OSDpMenuID=0;

BYTE fMenuUpdate=OFF, fMessageOSD=OFF, fMenuWaitFunc=OFF, fMenuDrawAll=OFF;
BYTE OSDAuxHideSprite=0;


typedef	struct
{
	BYTE Main[GetArrayMax(MainMenuInfo)];				// main
	BYTE Focus[GetArrayMax(FocusMenuInfo)];				// focus
	BYTE Exposure[GetArrayMax(ExpMenuInfo)];			// exposure
	BYTE WB[GetArrayMax(WBMenuInfo)];					// white balance
	BYTE DYNT[GetArrayMax(DYNTMenuInfo)];				// day&night
	BYTE DNR[GetArrayMax(DNRMenuInfo)];					// dnr
	BYTE L3WDRSet[GetArrayMax(L3WDRSetMenuInfo)];		// wdr (Line by Line - 3Frame)
	BYTE L2WDRSet[GetArrayMax(L2WDRSetMenuInfo)];		// wdr (Line by Line - 2Frame)
	BYTE TMWDRSet[GetArrayMax(TMWDRSetMenuInfo)];		// wdr (Tone map only)
	BYTE FRWDRSet[GetArrayMax(FRWDRSetMenuInfo)];		// wdr (Frame by Frame)
	BYTE DGWDRSet[GetArrayMax(DGWDRSetMenuInfo)];		// wdr (DWDR)
	BYTE Special[GetArrayMax(SpecialMenuInfo)];			// special
	BYTE GMC[GetArrayMax(GMCMenuInfo)];					// gmc	
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)
	BYTE DefogDwdr[GetArrayMax(DefogDwdrMenuInfo)];		// defog & dwdr
#else
	BYTE Defog[GetArrayMax(DefogMenuInfo)];		// defog & dwdr
	BYTE DWDR[GetArrayMax(DWDRMenuInfo)];		// defog & dwdr		
#endif		
	BYTE BLC[GetArrayMax(BLCMenuInfo)];					// blc
	BYTE BLCSizPos[GetArrayMax(BLCSizeMenuInfo)];		// blc size/position(x)
	BYTE BLCSizPosY[GetArrayMax(BLCSizeYInit)];			// blc size/position(y)
	BYTE HSBLC[GetArrayMax(HSBLCMenuInfo)];				// hsblc
	BYTE HSBLCSizPos[GetArrayMax(HSBLCSizeMenuInfo)];	// hsblc size/position(x)
	BYTE HSBLCSizPosY[GetArrayMax(HSBLCSizeYInit)];		// hsblc size/position(y)
	BYTE MD[GetArrayMax(MDMenuInfo)];					// md
	BYTE MDSizPos[GetArrayMax(MDSizeMenuInfo)];			// md size/position(x)
	BYTE MDSizPosY[GetArrayMax(MDSizeYInit)];			// md size/position(y)
	BYTE PM[GetArrayMax(PMMenuInfo)];					// pm
	BYTE DZoom[GetArrayMax(DZMenuInfo)];				// dzoom
	BYTE Effect[GetArrayMax(EffectMenuInfo)];			// adjust
	BYTE DPC[GetArrayMax(DPCMenuInfo)];					// defect pixel
	BYTE Setup[GetArrayMax(SetupMenuInfo)];				// setup
	BYTE CamTitle[GetArrayMax(CamTitleMenuInfo)];		// cam title
	BYTE RS485[GetArrayMax(RS485MenuInfo)];				// rs485
	BYTE MenuColor[GetArrayMax(MenuColorMenuInfo)];		// menu color
	BYTE Engr[GetArrayMax(EngrMenuInfo)];				// engineer menu
	BYTE ColorCorr[GetArrayMax(ColorCorrMenuInfo)];		// ColorCorr menu
	BYTE TP[GetArrayMax(TPMenuInfo)];					// sensor tp
	BYTE TPSizPos[GetArrayMax(TPSizeMenuInfo)];			// sensor tp size/position(x)
	BYTE TPSizPosY[GetArrayMax(TPSizeYInit)];			// sensor tp size/position(y)
	BYTE DZCtrl[GetArrayMax(DZCtrlMenuInfo)];			// dzoom ratio/position(x)
	BYTE DZCtrlY[GetArrayMax(DZCtrlYInit)];				// dzoom ratio/position(y)
	BYTE PIPPos[GetArrayMax(PIPPosiMenuInfo)];			// pip position(x)
	BYTE PIPPosY[GetArrayMax(PIPPosiYInit)];			// pip position(y)
	BYTE DispFunc[GetArrayMax(DispFuncMenuInfo)];		// display function
	BYTE FReset[GetArrayMax(FResetMenuInfo)];			// factory reset
	BYTE SysInfo[GetArrayMax(SysInfoMenuInfo)];			// system info.
	BYTE OutFrmt[GetArrayMax(OutFrmtMenuInfo)];			// output format
	BYTE DNRTune[GetArrayMax(DNRTuneMenuInfo)];			// dnr(tuning)
	BYTE Confer[GetArrayMax(ConferMenuInfo)];			// af conference menu
	BYTE MiscTune[GetArrayMax(MiscTuneMenuInfo)];		// misc(tuning)
	BYTE Debug[GetArrayMax(DebugMenuInfo)];				// debug menu
	BYTE VS[GetArrayMax(VSMenuInfo)];					// vector scope menu
	BYTE FS[GetArrayMax(FSMenuInfo)];					// factory setup menu
	BYTE CCF[GetArrayMax(CCFtuneMenuInfo)];	// Color Correction Fine tune menu
//	BYTE PicStyleSTD[GetArrayMax(DispFuncMenuInfo)];	// PictureStyle STD
//	BYTE PicStyleVID[GetArrayMax(DispFuncMenuInfo)];	// PictureStyle VIVID
//	BYTE PicStyleCLR[GetArrayMax(DispFuncMenuInfo)];	// PictureStyle CLEAR
//	BYTE PicStyleVCLR[GetArrayMax(DispFuncMenuInfo)];	// PictureStyle VIVID CLEAR
//	BYTE PicStyleNEU[GetArrayMax(DispFuncMenuInfo)];	// PictureStyle NEUTRAL
//	BYTE Compress[GetArrayMax(CompressMenuInfo)];		// compress

} stPACKED OSD_MENU_DATA, *POSD_MENU_DATA;

OSD_MENU_INFO	stMenuInfo;
OSD_MENU_DATA	stMenuData;

static char CamTitleBAK[MenuItemCol]	= "          ";


// message osd(chinese) -- max 20 characters
ROMDATA WORD msg_chn_please_wait[] = {256,421,299,300,46,46,256,33,256,'\0'};
ROMDATA WORD msg_chn_turn_off_wdr_compr[] = {256,421,284,292,279,293,256,33,256,'\0'};
ROMDATA WORD msg_chn_turn_off_cascade[] = {256,421,284,67,65,83,67,65,68,69,256,33,256,'\0'};
ROMDATA WORD msg_chn_turn_off_dzoom[] = {256,421,284,270,271,345,280,256,33,256,'\0'};
ROMDATA WORD msg_chn_turn_off_dis[] = {256,421,284,270,271,346,347,256,33,256,'\0'};
ROMDATA WORD msg_chn_turn_off_binning[] = {256,421,284,334,443,310,311,256,33,256,'\0'};
ROMDATA WORD msg_chn_turn_off_rotation[] = {256,421,284,82,79,84,65,84,73,79,78,256,33,256,'\0'};
ROMDATA WORD msg_chn_set_sensor_input[] = {256,421,374,375,497,498,493,494,256,33,256,'\0'};
ROMDATA WORD msg_chn_bootloader_fail[] = {32,66,79,79,84,76,79,65,68,69,82,32,70,65,73,76,32,33,32,'\0'};
ROMDATA WORD msg_chn_fw_download[] = {32,70,47,87,32,68,79,87,78,76,79,65,68,32,33,32,'\0'};
ROMDATA WORD msg_chn_factory_reset[] = {' ','F','a','c','t','o','r','y',' ','R','e','s','e','t',' ','!','\0'};
ROMDATA WORD msg_chn_not_available[] = {' ','N','o','t',' ','A','v','a','i','l','a','b','l','e',' ','!','\0'};

// title osd(chinese) -- max 10 characters
ROMDATA WORD title_chn_dzoom_on[] = {256,270,271,345,280,45,285,256,0,0};
ROMDATA WORD title_chn_dzoom_off[] = {256,270,271,345,280,45,284,256,0,0};


// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
extern WORD cntMessageOSD;
extern BOOL fCropMode ;
// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
static void OSD_MenuClear(void)
{
#if OSD_MAIN_EN
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX0], OFF);	// disappear sprite
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX1], OFF);	// disappear sprite
//	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX2], OFF);	// disappear sprite
#endif
	
#if	OSD_AUX_EN
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX8], OFF);	// disappear sprite
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX9], OFF);	// disappear sprite
#endif
	
	OSDpMenuID=0;
}

static BYTE OSD_GetMenuLevel(WORD mID)
{
	BYTE i, level=0;

	for(i=0;i<4;i++)	if((mID << 4*i) & 0xffff)	level++;
	
	return level;
}

//----------------------------------------------------------------------------------
static void OSD_GetMenuItemInfo(POSD_MENU_INFO pINFO, WORD mID)
{
	PBYTE pID = (PBYTE)&mID;		
	BYTE nID = HI4BIT(pID(0))-1;	BYTE sID = LO4BIT(pID(0))-1;	// Level 1,2 ID
	BYTE tID = HI4BIT(pID(1))-1;	BYTE zID = LO4BIT(pID(1))-1;	// Level 3,4 ID
	BYTE level = 0, idx=0;

	memset(pINFO, 0, sizeof(OSD_MENU_INFO));						// clear menu info. 

	level = OSD_GetMenuLevel(mID);
	
	switch(level)	{												// get id by level
		case 1:	idx=nID;	break;		case 2:	idx=sID;	break;
		case 3:	idx=tID;	break;		case 4:	idx=zID;	break;
	}

	switch(mID | MenuTreeMask(level)) {

// level-1
		case MAIN_TREE:	memcpy(pINFO, &MainMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.Main[idx];
					pINFO->MenuRow = GetArrayMax(MainMenuInfo);
					
					switch(idx) {  //if wdr controls exposure, skip 'exposure' menu
						case 1:  pINFO->Attb |= (GetWDR_AEProcMode()? MENU_SKIP: 0);	break;	// exposure(skip:wdr control)
					}
					break;

// level-2
		case FOCUS_TREE:	memcpy(pINFO, &FocusMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.Focus[idx];
					pINFO->MenuRow = GetArrayMax(FocusMenuInfo);
					
					switch(idx) {	// if 'focus mode' != interval, skip 'af interval' menu
#if !defined(__USE_2MOTOR_LENS__)
						case 5:	pINFO->Attb |= ((stMenuData.Focus[0]!=1)? MENU_SKIP: 0);	break;	// af interval(skip:not interval mode)
#endif
					}
					break;
		case EXPOSURE_TREE:	memcpy(pINFO, &ExpMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.Exposure[idx];
					pINFO->MenuRow = GetArrayMax(ExpMenuInfo);

					// shutter text (ntsc/pal)
#if defined(__USE_CHN_FONT__)
					if(idx==1)  pINFO->pItem = GetSS_SensorVoutPAL()?(PWORD)ExpShutItemPal:(PWORD)ExpShutItemNtsc;
#else
					if(idx==1)  pINFO->pItem = GetSS_SensorVoutPAL()?(PCHAR)ExpShutItemPal:(PCHAR)ExpShutItemNtsc;
#endif

					switch(idx) {	// if 'ae mode == xxx', skip 'shut, iris, agc' menu
#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
						case 1:	pINFO->Attb |= ((stMenuData.Exposure[0]==0)? MENU_SKIP: 0);	// shutter(skip:auto mode)
								pINFO->Attb |= ((stMenuData.Exposure[0]==2)? MENU_SKIP: 0);	// shutter(skip:iris fix mode)
								pINFO->Attb |= ((stMenuData.Exposure[0]==3)? MENU_SKIP: 0);	// shutter(skip:agc fix mode)
								//pINFO->Attb |= ((stMenuData.Exposure[0]==4)? MENU_SKIP: 0);	// shutter(skip:bright mode)
								break;
						case 2:	pINFO->Attb |= ((stMenuData.Exposure[0]==0)? MENU_SKIP: 0);	// iris(skip:auto mode)
								pINFO->Attb |= ((stMenuData.Exposure[0]==1)? MENU_SKIP: 0);	// iris(skip:shut fix mode)
								break;
						case 3:	pINFO->Attb |= ((stMenuData.Exposure[0]==0)? MENU_SKIP: 0);	// agc(skip:auto mode)
								pINFO->Attb |= ((stMenuData.Exposure[0]==1)? MENU_SKIP: 0);	// agc(skip:shut fix mode)
								pINFO->Attb |= ((stMenuData.Exposure[0]==2)? MENU_SKIP: 0);	// agc(skip:iris fix mode)
								break;
						case 4:	pINFO->Attb |= ((stMenuData.Exposure[0]==4)? MENU_SKIP: 0);	// agc max(skip:manual mode)
									pINFO->Attb |= ((stMenuData.Exposure[3]!=0)? MENU_SKIP: 0);	// agc max(skip:agc manual mode)
								break;
						case 5:	//pINFO->Attb |= ((stMenuData.Exposure[0]==1)? MENU_SKIP: 0);	// sensup(skip:shut fix mode)
								pINFO->Attb |= ((stMenuData.Exposure[0]==3)? MENU_SKIP: 0);	// sensup(skip:agc fix mode)
								break;
						case 6:	pINFO->Attb |= ((stMenuData.Exposure[0]==4)? MENU_SKIP: 0);	// brightness(skip:manual mode)
								//pINFO->Attb |= ((stMenuData.Exposure[4]==0)? MENU_SKIP: 0);	// brightness(skip:agc-max off)
								break;
#else
						case 1:	pINFO->Attb |= ((stMenuData.Exposure[0]==0)? MENU_SKIP: 0);	// shutter(skip:auto mode)
								pINFO->Attb |= ((stMenuData.Exposure[0]==2)? MENU_SKIP: 0);	// shutter(skip:agc fix mode)
								//pINFO->Attb |= ((stMenuData.Exposure[0]==3)? MENU_SKIP: 0);	// shutter(skip:bright mode)
								break;
						case 2:	pINFO->Attb |= ((stMenuData.Exposure[0]==0)? MENU_SKIP: 0);	// iris(skip:auto mode)
								pINFO->Attb |= ((stMenuData.Exposure[0]==1)? MENU_SKIP: 0);	// iris(skip:shut fix mode)
								break;
						case 3:	pINFO->Attb |= ((stMenuData.Exposure[0]==0)? MENU_SKIP: 0);	// agc(skip:auto mode)
								pINFO->Attb |= ((stMenuData.Exposure[0]==1)? MENU_SKIP: 0);	// agc(skip:shut fix mode)
								break;
						case 4:	pINFO->Attb |= ((stMenuData.Exposure[0]==3)? MENU_SKIP: 0);	// agc max(skip:manual mode)
									pINFO->Attb |= ((stMenuData.Exposure[3]!=0)? MENU_SKIP: 0);	// agc max(skip:agc manual mode)
								break;
						case 5:	//pINFO->Attb |= ((stMenuData.Exposure[0]==1)? MENU_SKIP: 0);	// sensup(skip:shut fix mode)
								pINFO->Attb |= ((stMenuData.Exposure[0]==2)? MENU_SKIP: 0);	// sensup(skip:agc fix mode)
								break;
						case 6:	pINFO->Attb |= ((stMenuData.Exposure[0]==3)? MENU_SKIP: 0);	// brightness(skip:manual mode)
								//pINFO->Attb |= ((stMenuData.Exposure[4]==0)? MENU_SKIP: 0);	// brightness(skip:agc-max off)
								break;
#endif
					}
					break;
		case WB_TREE:	memcpy(pINFO, &WBMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.WB[idx];
					pINFO->MenuRow = GetArrayMax(WBMenuInfo);
					
					switch(idx) {	// if 'wb mode' == xxx, skip 'push, red, blue' menu
						case 1:	pINFO->Attb |= ((stMenuData.WB[0]!=1)? MENU_SKIP: 0);	break;	// push(skip:not push mode)
						case 2:	case 3:
								pINFO->Attb |= ((stMenuData.WB[0]!=4)? MENU_SKIP: 0);	break;	// blue & red(skip:not manual mode)
						case 4:	pINFO->Attb |= (((stMenuData.WB[0]==1) || (stMenuData.WB[0]==4))? MENU_SKIP: 0);	break;	// speed(skip:manual, one push mode)
						case 5:	case 6:
								pINFO->Attb |= ((stMenuData.WB[0]==4)? MENU_SKIP: 0);	break;	// offset-b/r(skip:manual mode)
					}
					break;
		case DYNT_TREE:	memcpy(pINFO, &DYNTMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.DYNT[idx];
					pINFO->MenuRow = GetArrayMax(DYNTMenuInfo);
					
					switch(idx) {	// if 'daynight mode' == xxx, skip 'delay, level, color, burst' menu
						case 1:	pINFO->Attb |= ((stMenuData.DYNT[0]!=0)? MENU_SKIP: 0);	break;	// delay(skip:not auto mode)
						case 2:	pINFO->Attb |= ((stMenuData.DYNT[0]!=0)? MENU_SKIP: 0);	break;	// d->n agc level(skip:not auto mode)
						case 3:	pINFO->Attb |= ((stMenuData.DYNT[0]!=0)? MENU_SKIP: 0);	break;	// n->d agc level(skip:not auto mode)
						case 4:	pINFO->Attb |= ((stMenuData.DYNT[0]==1)? MENU_SKIP: 0);	break;	// night color(skip:color mode)
						case 5:	pINFO->Attb |= ((stMenuData.DYNT[0]==1)? MENU_SKIP: 0);			// color burst(skip:color mode)
								pINFO->Attb |= ((stMenuData.DYNT[4]==1)? MENU_SKIP: 0);	break;	// color burst(skip:night color enabled)
					}
					break;
		case DNR_TREE: memcpy(pINFO, &DNRMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.DNR[idx];
					pINFO->MenuRow = GetArrayMax(DNRMenuInfo);
					
					switch(idx) {	// if 'dnr mode' != 3D, skip 'level' menu
						case 1:	pINFO->Attb |= ((stMenuData.DNR[0]<1)? MENU_SKIP: 0);	break;	// level(skip:NR off)
						case 2:	pINFO->Attb |= ((stMenuData.DNR[0]<2)? MENU_SKIP: 0);	break;	// level(skip:not 3d mode)
					}
					break;
		case PM_TREE:	memcpy(pINFO, &PMMenuInfo[idx], sizeof(OSD_MENU_INFO));
						pINFO->cID = stMenuData.PM[idx];
						pINFO->MenuRow = GetArrayMax(PMMenuInfo);
						
					switch(idx) {	// if 'pm type' == square, skip 'polygon edge select' menu
						case 5:	pINFO->Attb |= ((stMenuData.PM[1]==0)? MENU_SKIP: 0);	break;	// polygon edge select(skip:square type)
						case 8:	case 9:
								pINFO->Attb |= ((stMenuData.PM[1]!=0)? MENU_SKIP: 0);	break;	// square size(skip:polygon type)
					}
						
					switch(idx) {	// if 'pm mode' == OFF, skip all menu
						case 1:	case 2:	case 3:	case 4: case 5:
						case 6:	case 7:	case 8:	case 9: case 10:
							pINFO->Attb |= ((stMenuData.PM[0]==OFF)? MENU_SKIP: 0);	break;	// all(skip:off mode)
					}
					break;
		case EFFECT_TREE:	memcpy(pINFO, &EffectMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.Effect[idx];
					pINFO->MenuRow = GetArrayMax(EffectMenuInfo);
					switch(idx) {
#if defined(__USE_MDIN_i500__)
						case 1:  pINFO->Attb |= ((stMenuData.Special[9])? MENU_SKIP: 0);  break;  // gamma(skip:wdr)
#else
	#if defined(__RGBGAMMA_USE_CONTRAST__)
						case 2:	pINFO->Attb |= ((stMenuData.DispFunc[4]!=MDIN_PS_OFF)? MENU_SKIP: 0); break; // If picture style is applied
	#endif
						case 3:	pINFO->Attb |= ((stMenuData.DispFunc[4]!=MDIN_PS_OFF)? MENU_SKIP: 0); break; // If picture style is applied
						case 4:	pINFO->Attb |= ((stMenuData.DispFunc[4]!=MDIN_PS_OFF)? MENU_SKIP: 0); break; // If picture style is applied
						case 5:	pINFO->Attb |= ((stMenuData.DispFunc[4]!=MDIN_PS_OFF)? MENU_SKIP: 0); break; // If picture style is applied
						case 6:	pINFO->Attb |= ((stMenuData.DispFunc[4]!=MDIN_PS_OFF)? MENU_SKIP: 0); break; // If picture style is applied
#endif
						case 7:  pINFO->Attb |= ((stMenuData.Special[9])? MENU_SKIP: 0);  break;  // auto.saturate(skip:wdr)
						case 8:  pINFO->Attb |= ((stMenuData.Special[9])? MENU_SKIP: 0);  break;  // auto.edge(skip:wdr)
						case 9:  pINFO->Attb |= ((stMenuData.Special[9])? MENU_SKIP: 0);  break;  // auto.bright(skip:wdr)
					}
					break;
		case SPECIAL_TREE:	memcpy(pINFO, &SpecialMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.Special[idx];
					pINFO->MenuRow = GetArrayMax(SpecialMenuInfo);
					switch(idx) {
						case 0:	pINFO->Attb |= ((stMenuData.Special[4])? MENU_SKIP: 0);  // dzoom(skip:dis 'on')
									pINFO->Attb |= ((stMenuData.DispFunc[3])? MENU_SKIP: 0);  // dzoom(skip:rotation 'on')
#if USE_IPC_COMPRES
								pINFO->Attb |= ((stMenuData.Special[9])? MENU_SKIP: 0);  // dzoom(skip:wdr compress 'on')
#endif
								break;	
						case 1:	pINFO->Attb |= ((stMenuData.Special[5])? MENU_SKIP: 0);	break;  // defog/dwdr(skip:blc/hlm 'on')
						case 4:	pINFO->Attb |= ((stMenuData.Special[0])? MENU_SKIP: 0);  // dis(skip:dzoom 'on')
									pINFO->Attb |= ((stMenuData.DispFunc[1])? MENU_SKIP: 0);  // dis(skip:mirror 'on')
									pINFO->Attb |= ((stMenuData.DispFunc[3])? MENU_SKIP: 0);  // dis(skip:rotation 'on')
//#if USE_IPC_COMPRES
								pINFO->Attb |= ((stMenuData.Special[9])? MENU_SKIP: 0);  // dis(skip:wdr compress 'on')
//#endif
								break;	
						case 5:	pINFO->Attb |= ((stMenuData.Special[1])? MENU_SKIP: 0);	break;  // blc/hlm(skip:defog 'on/auto')
						case 6:	pINFO->Attb |= ((stMenuData.Special[9])? MENU_SKIP: 0);  // binning(skip:wdr 'on')
								break;
						case 9:	pINFO->Attb |= ((stMenuData.Engr[0]!=0)? MENU_SKIP: 0);  // wdr(skip:no sensor input)
#if USE_IPC_COMPRES
								pINFO->Attb |= ((stMenuData.Special[0])? MENU_SKIP: 0);  // wdr(skip:dzoom 'on')
#endif								
								pINFO->Attb |= ((stMenuData.Special[4])? MENU_SKIP: 0);  // wdr(skip:dis 'on')
								pINFO->Attb |= ((stMenuData.Special[6])? MENU_SKIP: 0);  // wdr(skip:binning 'on')
								pINFO->Attb |= ((stMenuData.DispFunc[3])?MENU_SKIP: 0);  // wdr(skip:rotation 'on')
								break;
					}
					break;

		case SETUP_TREE:	memcpy(pINFO, &SetupMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.Setup[idx];
					pINFO->MenuRow = GetArrayMax(SetupMenuInfo);	break;
		case ENGR_TREE:		memcpy(pINFO, &EngrMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.Engr[idx];
					pINFO->MenuRow = GetArrayMax(EngrMenuInfo);
					switch(idx) {
						case 0:	pINFO->Attb |= ((stMenuData.Engr[9]!=0)? MENU_SKIP: 0);  // in.select(skip:wdr)
								break;
						case 1:	pINFO->Attb |= ((stMenuData.Engr[0]==0)? MENU_SKIP: 0);  // in.format(skip:sensor input)
								pINFO->Attb |= ((stMenuData.Engr[0]==2)? MENU_SKIP: 0);  // in.format(skip:sensor pip mode)
								break;
						case 7:	pINFO->Attb |= ((stMenuData.DispFunc[3])? MENU_SKIP: 0);  // sensor fps(skip:rotation 'on')
								break;
						case 9:	pINFO->Attb |= ((stMenuData.Engr[0]!=0)? MENU_SKIP: 0);  // wdr(skip:no sensor input)
#if USE_IPC_COMPRES
								pINFO->Attb |= ((stMenuData.Special[0])? MENU_SKIP: 0);  // wdr(skip:dzoom 'on')
#endif
								pINFO->Attb |= ((stMenuData.Special[4])? MENU_SKIP: 0);  // wdr(skip:dis 'on')
								pINFO->Attb |= ((stMenuData.Special[6])? MENU_SKIP: 0);  // wdr(skip:binning 'on')
								pINFO->Attb |= ((stMenuData.DispFunc[3])?MENU_SKIP: 0);  // wdr(skip:rotation 'on')
								break;
					}
					break;
		case DEBUG_TREE:	memcpy(pINFO, &DebugMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.Debug[idx];
					pINFO->MenuRow = GetArrayMax(DebugMenuInfo);	break;

// level-3
		case CONFER_TREE:	memcpy(pINFO, &ConferMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.Confer[idx];
					pINFO->MenuRow = GetArrayMax(ConferMenuInfo);
					switch(idx) {
						case 1:	pINFO->Attb |= ((stMenuData.Confer[0]==0)? MENU_SKIP: 0);	break;	// win-width(skip:conference mode off)
						case 2:	pINFO->Attb |= ((stMenuData.Confer[0]==0)? MENU_SKIP: 0);	break;	// win-height(skip:conference mode off)
						case 3:	pINFO->Attb |= ((stMenuData.Confer[0]==0)? MENU_SKIP: 0);	break;	// win-xpos(skip:conference mode off)
						case 4:	pINFO->Attb |= ((stMenuData.Confer[0]==0)? MENU_SKIP: 0);	break;	// win-ypos(skip:conference mode off)
					}
					break;
		case GMC_TREE:	memcpy(pINFO, &GMCMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.GMC[idx];
					pINFO->MenuRow = GetArrayMax(GMCMenuInfo);
					switch(idx) {
						case 0: pINFO->Attb |= ((stMenuData.Engr[9]!=0)? MENU_SKIP: 0);	break;  // front gamma(skip:wdr)
					}
					break;
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)			
		case DEFOGDWDR_TREE: memcpy(pINFO, &DefogDwdrMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.DefogDwdr[idx];
					pINFO->MenuRow = GetArrayMax(DefogDwdrMenuInfo);
					switch(idx) {	
						case 0:	pINFO->Attb |= ((stMenuData.Special[1]==0)? MENU_SKIP: 0);	// strength(skip: 'off')
								pINFO->Attb |= ((stMenuData.Special[1]==1)? MENU_SKIP: 0);	// strength(skip: 'auto')
								break;
						case 1:	pINFO->Attb |= ((stMenuData.Special[1]==0)? MENU_SKIP: 0);	// threshold(skip: 'off')
								pINFO->Attb |= ((stMenuData.Special[1]==1)? MENU_SKIP: 0);	// threshold(skip: 'auto')
								pINFO->Attb |= ((stMenuData.Special[1]==3)? MENU_SKIP: 0);	// threshold(skip: 'dwdr on')
								break;
						case 2:	pINFO->Attb |= ((stMenuData.Special[1]==0)? MENU_SKIP: 0);	// saturation(skip: 'off')
								pINFO->Attb |= ((stMenuData.Special[1]==1)? MENU_SKIP: 0);	// saturation(skip: 'auto')
								pINFO->Attb |= ((stMenuData.Special[1]==2)? MENU_SKIP: 0);	// saturation(skip: 'defog on')
								break;
						case 3:	pINFO->Attb |= ((stMenuData.Special[1]==0)? MENU_SKIP: 0);	// local ratio(skip: 'off')
								pINFO->Attb |= ((stMenuData.Special[1]==1)? MENU_SKIP: 0);	// local ratio(skip: 'auto')
								pINFO->Attb |= ((stMenuData.Special[1]==2)? MENU_SKIP: 0);	// local ratio(skip: 'defog on')
								break;
						case 4:	pINFO->Attb |= ((stMenuData.Special[1]==0)? MENU_SKIP: 0);	// sat. sync(skip: 'off')
								pINFO->Attb |= ((stMenuData.Special[1]==2)? MENU_SKIP: 0);	// sat. sync(skip: 'defog on')
								pINFO->Attb |= ((stMenuData.Special[1]==3)? MENU_SKIP: 0);	// sat. sync(skip: 'dwdr on')
								break;
					}
					break;
#else
		case DEFOG_TREE: memcpy(pINFO, &DefogMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.Defog[idx];
					pINFO->MenuRow = GetArrayMax(DefogMenuInfo);

					switch(idx) {	
						case 0:	pINFO->Attb |= ((stMenuData.Special[1]==2)? MENU_SKIP: 0);	break;
						case 1:	pINFO->Attb |= ((stMenuData.Special[1]==1)? MENU_SKIP: 0);	break;
						case 2:	pINFO->Attb |= ((stMenuData.Special[1]==1)? MENU_SKIP: 0);	break;
					}
					break;
		case DWDR_TREE: memcpy(pINFO, &DWDRMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.DWDR[idx];
					pINFO->MenuRow = GetArrayMax(DWDRMenuInfo);

					switch(idx) {	
						case 0:	pINFO->Attb |= ((stMenuData.Special[2]==2)? MENU_SKIP: 0);	break;
						case 1:	pINFO->Attb |= ((stMenuData.Special[2]==2)? MENU_SKIP: 0);	break;
						case 2:	pINFO->Attb |= ((stMenuData.Special[2]==2)? MENU_SKIP: 0);	break;
						case 3:	pINFO->Attb |= ((stMenuData.Special[2]==1)? MENU_SKIP: 0);	break;
						case 4:	pINFO->Attb |= ((stMenuData.Special[2]==1)? MENU_SKIP: 0);	break;
					}
					
					break;
#endif				
		case HSBLC_TREE:
				if (stMenuData.Special[sID] == 1)	{						// blc
					memcpy(pINFO, &BLCMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.BLC[idx];
					pINFO->MenuRow = GetArrayMax(BLCMenuInfo);		break;
				}
				else	{													// hsblc
					memcpy(pINFO, &HSBLCMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.HSBLC[idx];
					pINFO->MenuRow = GetArrayMax(HSBLCMenuInfo);	break;
				}
		case MD_TREE:	memcpy(pINFO, &MDMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.MD[idx];
					pINFO->MenuRow = GetArrayMax(MDMenuInfo);
					switch(idx) {
						case 2:	pINFO->Attb |= ((stMenuData.MD[1]==OFF)? MENU_SKIP: 0);	break;	// md area display on/off(skip:area disable)
						case 3:	pINFO->Attb |= ((stMenuData.MD[1]==OFF)? MENU_SKIP: 0);	break;	// md area size/position (skip:area disable)
					}
					break;
		case DZ_TREE:	memcpy(pINFO, &DZMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.DZoom[idx];
					pINFO->MenuRow = GetArrayMax(DZMenuInfo);
#if defined(__USE_CMOS_AUX_SYNC_OUT__) && (defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__))	// skip pip process in 4k sensor
					switch(idx) {
						case 1:	pINFO->Attb |= MENU_SKIP;	break;	// pip on/off(skip:4k sensor)
						case 2:	pINFO->Attb |= MENU_SKIP;	break;	// pip size(skip:4k sensor)
						case 3:	pINFO->Attb |= MENU_SKIP;	break;	// pip position(skip:4k sensor)
					}
#endif
					break;
		case DPIXEL_TREE:	memcpy(pINFO, &DPCMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.DPC[idx];
					pINFO->MenuRow = GetArrayMax(DPCMenuInfo);		break;
		case DISPFUNC_TREE:	memcpy(pINFO, &DispFuncMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.DispFunc[idx];
					pINFO->MenuRow = GetArrayMax(DispFuncMenuInfo);	
					switch(idx) {
						case 1:	pINFO->Attb |= ((stMenuData.Special[4])? MENU_SKIP: 0);  // mirror(skip:dis 'on')
									break;
						case 3:	pINFO->Attb |= ((stMenuData.Special[0])? MENU_SKIP: 0);  // rotation(skip:dzoom 'on')
								pINFO->Attb |= ((stMenuData.Special[3])? MENU_SKIP: 0);  // rotation(skip:md 'on')
								pINFO->Attb |= ((stMenuData.Special[4])? MENU_SKIP: 0);  // rotation(skip:dis 'on')
								pINFO->Attb |= ((stMenuData.Special[9])? MENU_SKIP: 0);  // rotation(skip:wdr 'on')
								pINFO->Attb |= ((stMenuData.Engr[7]>1)? MENU_SKIP: 0);  // rotation(skip:sensor fps '50/60hz')
								break;
						// Picture style			
#if defined(__USE_MDIN_i510__)||defined(__USE_MDIN_i540__)||defined(__USE_MDIN_i550__)
						case 5:	pINFO->Attb |= ((stMenuData.DispFunc[4]==MDIN_PS_OFF)? MENU_SKIP: 0);  // picture contrast(picture style off)
								break;	
						case 6:	pINFO->Attb |= ((stMenuData.DispFunc[4]==MDIN_PS_OFF)? MENU_SKIP: 0);  // picture saturation(picture style off)
								break;
						case 7:	pINFO->Attb |= ((stMenuData.DispFunc[4]==MDIN_PS_OFF)? MENU_SKIP: 0);  // picture edge(picture style off)
								break;								
#endif
					}
					break;
		case CAMTITLE_TREE:	memcpy(pINFO, &CamTitleMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.CamTitle[idx];
					pINFO->MenuRow = GetArrayMax(CamTitleMenuInfo);	break;
		case RS485_TREE:	memcpy(pINFO, &RS485MenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.RS485[idx];
					pINFO->MenuRow = GetArrayMax(RS485MenuInfo);	break;
		case MENUCOLOR_TREE:memcpy(pINFO, &MenuColorMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.MenuColor[idx];
					pINFO->MenuRow = GetArrayMax(MenuColorMenuInfo);break;
		case FRESET_TREE:	memcpy(pINFO, &FResetMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.FReset[idx];
					pINFO->MenuRow = GetArrayMax(FResetMenuInfo);	break;
		case SYSINFO_TREE:	memcpy(pINFO, &SysInfoMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.SysInfo[idx];
					pINFO->MenuRow = GetArrayMax(SysInfoMenuInfo);	break;
		case OUTFRMT_TREE:	memcpy(pINFO, &OutFrmtMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.OutFrmt[idx];
					pINFO->MenuRow = GetArrayMax(OutFrmtMenuInfo);	break;
		case COLOR_CORR_TREE: memcpy(pINFO, &ColorCorrMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.ColorCorr[idx];
					pINFO->MenuRow = GetArrayMax(ColorCorrMenuInfo);		break;			
		case TP_TREE:		memcpy(pINFO, &TPMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.TP[idx];
					pINFO->MenuRow = GetArrayMax(TPMenuInfo);		break;
		case MISC_TUNE_TREE:	memcpy(pINFO, &MiscTuneMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.MiscTune[idx];
					pINFO->MenuRow = GetArrayMax(MiscTuneMenuInfo);
					switch(idx) {
#if !defined(__USE_IRKEY_SWITCH__)
						case 4:  pINFO->Attb |= MENU_SKIP;  break;  // ir remote on/off(skip:ir key switch option not defined..)
#endif
					}
					break;
		case WDR_TUNE_TREE:
				switch(GetWDR_ProcesMode()) {
					case MISP_WDR_PROC_LINE3:
						memcpy(pINFO, &L3WDRSetMenuInfo[idx], sizeof(OSD_MENU_INFO));
						pINFO->cID = stMenuData.L3WDRSet[idx];
						pINFO->MenuRow = GetArrayMax(L3WDRSetMenuInfo);
						switch(idx) {  //set attribute -> stMenuData.L3WDRSet[3] : auto expsoure (0:on, 1:off)
							case 4:  pINFO->Attb |= ((stMenuData.L3WDRSet[3]==MISP_WDR_AE_MANU)? MENU_SKIP: 0);  break;  //brightness
							case 5:  pINFO->Attb |= ((stMenuData.L3WDRSet[3]==MISP_WDR_AE_AUTO)? MENU_SKIP: 0);  break;  //gain
#if WDR_IRIS_CTRL_EN
							case 6:  pINFO->Attb |= ((stMenuData.L3WDRSet[3]==MISP_WDR_AE_AUTO)? MENU_SKIP: 0);  break;  //iris
#else
							case 6:  pINFO->Attb |= MENU_SKIP;                                                   break;  //iris
#endif
							case 7:  pINFO->Attb |= ((stMenuData.L3WDRSet[3]==MISP_WDR_AE_AUTO)? MENU_SKIP: 0);  break;  //shutter
						}
						break;
					case MISP_WDR_PROC_LINE2:
						memcpy(pINFO, &L2WDRSetMenuInfo[idx], sizeof(OSD_MENU_INFO));
						pINFO->cID = stMenuData.L2WDRSet[idx];
						pINFO->MenuRow = GetArrayMax(L2WDRSetMenuInfo);
						switch(idx) {  //set attribute -> stMenuData.L2WDRSet[3] : auto expsoure (0:on, 1:off)
							case 4:  pINFO->Attb |= ((stMenuData.L2WDRSet[3]==MISP_WDR_AE_MANU)? MENU_SKIP: 0);  break;  //brightness
							case 5:  pINFO->Attb |= ((stMenuData.L2WDRSet[3]==MISP_WDR_AE_AUTO)? MENU_SKIP: 0);  break;  //gain
#if WDR_IRIS_CTRL_EN
							case 6:  pINFO->Attb |= ((stMenuData.L2WDRSet[3]==MISP_WDR_AE_AUTO)? MENU_SKIP: 0);  break;  //iris
#else
							case 6:  pINFO->Attb |= MENU_SKIP;                                                   break;  //iris
#endif
							case 7:  pINFO->Attb |= ((stMenuData.L2WDRSet[3]==MISP_WDR_AE_AUTO)? MENU_SKIP: 0);  break;  //shutter
						}
						break;
					case MISP_WDR_PROC_BUILT:
						memcpy(pINFO, &TMWDRSetMenuInfo[idx], sizeof(OSD_MENU_INFO));
						pINFO->cID = stMenuData.TMWDRSet[idx];
						pINFO->MenuRow = GetArrayMax(TMWDRSetMenuInfo);
						switch(idx) {  //set attribute -> stMenuData.TMWDRSet[3] : auto expsoure (0:on, 1:off)
							case 4:  pINFO->Attb |= ((stMenuData.TMWDRSet[3]==MISP_WDR_AE_MANU)? MENU_SKIP: 0);  break;  //brightness
#if HTM_FIXED_GAIN
							case 5:  pINFO->Attb |= MENU_SKIP;                                                   break;  //gain
#else
							case 5:  pINFO->Attb |= ((stMenuData.TMWDRSet[3]==MISP_WDR_AE_AUTO)? MENU_SKIP: 0);  break;  //gain
#endif
#if WDR_IRIS_CTRL_EN
							case 6:  pINFO->Attb |= ((stMenuData.TMWDRSet[3]==MISP_WDR_AE_AUTO)? MENU_SKIP: 0);  break;  //iris
#else
							case 6:  pINFO->Attb |= MENU_SKIP;                                                   break;  //iris
#endif
							case 7:  pINFO->Attb |= ((stMenuData.TMWDRSet[3]==MISP_WDR_AE_AUTO)? MENU_SKIP: 0);  break;  //shutter
						}
						break;
					case MISP_WDR_PROC_FRAME:
						memcpy(pINFO, &FRWDRSetMenuInfo[idx], sizeof(OSD_MENU_INFO));
						pINFO->cID = stMenuData.FRWDRSet[idx];
						pINFO->MenuRow = GetArrayMax(FRWDRSetMenuInfo);
						switch(idx) {  //set attribute -> stMenuData.FRWDRSet[3] : auto expsoure (0:on, 1:off)
							case 4:  pINFO->Attb |= ((stMenuData.FRWDRSet[3]==MISP_WDR_AE_MANU)? MENU_SKIP: 0);  break;  //brightness
							case 5:  pINFO->Attb |= ((stMenuData.FRWDRSet[3]==MISP_WDR_AE_AUTO)? MENU_SKIP: 0);  break;  //gain
#if WDR_IRIS_CTRL_EN
							case 6:  pINFO->Attb |= ((stMenuData.FRWDRSet[3]==MISP_WDR_AE_AUTO)? MENU_SKIP: 0);  break;  //iris
#else
							case 6:  pINFO->Attb |= MENU_SKIP;                                                   break;  //iris
#endif
							case 7:  pINFO->Attb |= ((stMenuData.FRWDRSet[3]==MISP_WDR_AE_AUTO)? MENU_SKIP: 0);  break;  //shutter
						}
						break;
					case MISP_WDR_PROC_SINGL:
						memcpy(pINFO, &DGWDRSetMenuInfo[idx], sizeof(OSD_MENU_INFO));
						pINFO->cID = stMenuData.DGWDRSet[idx];
						pINFO->MenuRow = GetArrayMax(DGWDRSetMenuInfo);
						break;
					default:
						break;
				}
				break;  //WDR_TUNE_TREE
		case DNR_TUNE_TREE:	memcpy(pINFO, &DNRTuneMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.DNRTune[idx];
					pINFO->MenuRow = GetArrayMax(DNRTuneMenuInfo);	break;
		case VS_TREE:	memcpy(pINFO, &VSMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.VS[idx];
					pINFO->MenuRow = GetArrayMax(VSMenuInfo);
					switch(idx) {
						case 1:  pINFO->Attb |= ((stMenuData.VS[0]==0)?  MENU_SKIP:  0);  break;  // position-x(skip:scan 'full')
						case 2:  pINFO->Attb |= ((stMenuData.VS[0]==0)?  MENU_SKIP:  0);  break;  // position-y(skip:scan 'full')
						case 3:  pINFO->Attb |= ((stMenuData.VS[0]==0)?  MENU_SKIP:  0);  break;  // size-x(skip:scan 'full')
						case 4:  pINFO->Attb |= ((stMenuData.VS[0]==0)?  MENU_SKIP:  0);  break;  // size-y(skip:scan 'full')
					}
					break;
		case FS_TREE:	memcpy(pINFO, &FSMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.FS[idx];
					pINFO->MenuRow = GetArrayMax(FSMenuInfo);
					break;

		case CCF_TREE:	memcpy(pINFO, &CCFtuneMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.CCF[idx];
					pINFO->MenuRow = GetArrayMax(CCFtuneMenuInfo);
					break;					

// level-4
		case BLCSIZE_TREE:	memcpy(pINFO, &BLCSizeMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.BLCSizPos[idx];			pINFO->MenuRow = GetArrayMax(BLCSizeMenuInfo);
					pINFO->pos_y = stMenuData.BLCSizPosY[idx%2];	pINFO->pos_y_max = BLCSizeYMax[idx%2];	
					break;
		case HSBLCSIZE_TREE:memcpy(pINFO, &HSBLCSizeMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.HSBLCSizPos[idx];		pINFO->MenuRow = GetArrayMax(HSBLCSizeMenuInfo);
					pINFO->pos_y = stMenuData.HSBLCSizPosY[idx%2];	pINFO->pos_y_max = HSBLCSizeYMax[idx%2];
					break;
		case MDSIZE_TREE:	memcpy(pINFO, &MDSizeMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.MDSizPos[idx];			pINFO->MenuRow = GetArrayMax(MDSizeMenuInfo);
					pINFO->pos_y = stMenuData.MDSizPosY[idx%2];		pINFO->pos_y_max = MDSizeYMax[idx%2];
					break;
		case DZRATIO_TREE:	memcpy(pINFO, &DZCtrlMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.DZCtrl[idx];			pINFO->MenuRow = sizeof(stMenuData.DZCtrl);
					pINFO->pos_y = stMenuData.DZCtrlY[idx%2];		pINFO->pos_y_max = DZCtrlYMax[idx%2];
					break;
		case PIPPOSI_TREE:	memcpy(pINFO, &PIPPosiMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.PIPPos[idx];			pINFO->MenuRow = GetArrayMax(PIPPosiMenuInfo);
					pINFO->pos_y = stMenuData.PIPPosY[idx%2];		pINFO->pos_y_max = PIPPosiYMax[idx%2];	
					break;
		case TPSIZE_TREE:	memcpy(pINFO, &TPSizeMenuInfo[idx], sizeof(OSD_MENU_INFO));
					pINFO->cID = stMenuData.TPSizPos[idx];			pINFO->MenuRow = GetArrayMax(TPSizeMenuInfo);
					pINFO->pos_y = stMenuData.TPSizPosY[idx%2];		pINFO->pos_y_max = TPSizeYMax[idx%2];	
					break;
		// case PICSTYLE_TREE:	
					// memcpy(pINFO, &PicStyleMenuInfo[idx], sizeof(OSD_MENU_INFO));								
					// switch(GetApp_ssPictureStyle()) {
						// case MDIN_PS_STANDARD: pINFO->cID = stMenuData.PicStyleSTD[idx]; break;								
						// case MDIN_PS_VIVID:	pINFO->cID = stMenuData.PicStyleVID[idx]; break;
						// case MDIN_PS_CLEAR:	pINFO->cID = stMenuData.PicStyleCLR[idx]; break;
						// case MDIN_PS_VIVID_CLEAR: pINFO->cID = stMenuData.PicStyleVCLR[idx]; break;
						// case MDIN_PS_NEUTRAL: pINFO->cID = stMenuData.PicStyleNEU[idx];	break;						
						// default: break;
					// }
					// pINFO->MenuRow = GetArrayMax(PicStyleMenuInfo);	
					// break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void OSD_MenuSelect(BYTE n, BYTE mask, PBYTE pID, BOOL dir)	//OSDMenuID +/-
{
	BYTE msb = HI4BIT(*pID), lsb = LO4BIT(*pID);
	BYTE val = (mask==0x0f)? lsb : msb;

	if (dir) {val += (val==n)? -(n-1):1;}
	else	 {val -= (val==1)? -(n-1):1;}

	if (mask==0x0f)	*pID = MAKEBYTE(msb,val);
	else			*pID = MAKEBYTE(val,lsb);
}

//--------------------------------------------------------------------------------------------------------------------------
static void OSD_CombSelect(WORD n, PWORD pID, BOOL dir)
{
	if (dir) {*pID += (*pID==n-1)? -(n-1):1;}
	else	 {*pID -= (*pID == 0)? -(n-1):1;}
}

//--------------------------------------------------------------------------------------------------------------------------
static void OSD_ProgSelect(WORD n, PWORD pID, BOOL dir)
{
	if (dir) {*pID += (*pID==n)? 0:1;}
	else	 {*pID -= (*pID==0)? 0:1;}
}

//--------------------------------------------------------------------------------------------------------------------------
static void OSD_ProgSelect2(BYTE n, PBYTE pID, BOOL dir)
{
	if (dir) {*pID += (*pID==n)? 0:1;}
	else	 {*pID -= (*pID==0)? 0:1;}
}

//-------------------------------------------------------------------------
#if defined(__USE_CHN_FONT__)
static BYTE OSD_GetCombString(PWORD pSTR, PWORD pMenu, BYTE cID, BYTE Col)
#else
static BYTE OSD_GetCombString(PBYTE pSTR, PCHAR pMenu, BYTE cID, BYTE Col)
#endif
{
	BYTE j, k=0;
		
	for(j=0; j<MenuMargin; j++)		pSTR[k++] = ' ';					//Menu margin(blank)
	for(j=0; j<Col; j++)			pSTR[k++] = pMenu[cID*Col+j];	//Menu Right Char
	for(j=0; j<MenuMargin; j++)		pSTR[k++] = ' ';					//Menu margin(blank)

	return k;
}

//-------------------------------------------------------------------------
#if defined(__USE_CHN_FONT__)
static BYTE OSD_GetProgString(PWORD pSTR, BYTE cID)
#else
static BYTE OSD_GetProgString(PBYTE pSTR, BYTE cID)
#endif
{
	BYTE j, k=0, Col=0;
	WORD den = 1000;

	for(j=0; j<MenuMargin; j++)		pSTR[k++] = ' ';				//Menu margin(blank)
	while (den/10) {if (cID/den) break; den /= 10;}
	while (den) {pSTR[k++] = '0'+cID/den; cID %= den; den /= 10; Col++;}
	for(j=0; j<(4-Col); j++)		pSTR[k++] = ' ';

	return k;
}

//--------------------------------------------------------------------------	 
#if defined(__USE_CHN_FONT__)
static BYTE OSD_GetProgBar(PWORD pSTR, BYTE value, BYTE max)
#else
static BYTE OSD_GetProgBar(PBYTE pSTR, BYTE value, BYTE max)
#endif
{
	BYTE q=0, r=0;
	BYTE i,k=0;

	q = (value * 100 / max) / 20;
	r = (value * 100 / max) % 20;

	pSTR[k++] = 30;	// -|

	if (q>=5)
	{
		q=5;
		for (i=0; i<q; i++)			pSTR[k++] = 29;
	}
	else
	{
		for (i=0; i<q; i++)			pSTR[k++] = 29;
		pSTR[k++] = (19 + (r/2));					//0~1(0), 2~3(1),,18~19(9)
		for (i=0; i<(4-q); i++)		pSTR[k++] = 19;
	}

	pSTR[k++] = 31;	// |-

	return k;
}

//--------------------------------------------------------------------------------------------------------------------------
#if defined(__USE_CHN_FONT__)
static BYTE OSD_GetMenuString(PWORD pSTR, BYTE index)
#else
static BYTE OSD_GetMenuString(PBYTE pSTR, BYTE index)
#endif
{
 	BYTE i=0, j=0, k=0, level;
	WORD mID;

	level = OSD_GetMenuLevel(OSDMenuID);

	if(index < MenuTitleRow)	mID = OSDMenuID;							// use menu id in title
	else	{
		mID = OSDMenuID & ~(MenuTreeMask(level));	    					// erase cursor id
		mID |= (index-1) << GetWeight(level);				  				// insert index id
	}

	OSD_GetMenuItemInfo(&stMenuInfo, mID);

	if (index >= stMenuInfo.MenuRow + MenuTitleRow)	{
		for(i=0;i<MenuTitleCol+MenuMargin*2;i++)	pSTR[k++] = ' ';					// blank
		return 0;
	}

	if (index < MenuTitleRow)	{														// title
		if(OSD_GetMenuLang()==1) 	stMenuInfo.pTitle += MenuTitleCol*2; 				// if chinese font..
		
		k += MenuMargin;
		for(j=0; j<MenuTitleCol; j++)	pSTR[k++] = stMenuInfo.pTitle[index*MenuTitleCol+j];	// title
		k += MenuMargin;
		stMenuInfo.Attb = MENU_NULL;
	}
	else {
		if(OSD_GetMenuLang()==1) 	stMenuInfo.pMenu += MenuNameCol; 					// if chinese font..
		
		k += MenuMargin;
		for(j=0; j<MenuNameCol; j++)	pSTR[k++] = stMenuInfo.pMenu[j];			// menu
		k += MenuMargin;
		
		if (stMenuInfo.Attb & MENU_SKIP)	{											// item skip
#if defined(__USE_CHN_FONT__)
			if(OSD_GetMenuLang()==1) 	k += OSD_GetCombString(&pSTR[k], (PWORD)BlankItem, 1, MenuItemCol);	// if chinese font..
			else 						k += OSD_GetCombString(&pSTR[k], (PWORD)BlankItem, 0, MenuItemCol);
#else
			k += OSD_GetCombString(&pSTR[k], (PCHAR)BlankItem, 0, MenuItemCol);
#endif
		}
		else {
			if (stMenuInfo.Attb & MENU_VALUE)	{
				k += OSD_GetProgString(&pSTR[k], stMenuInfo.cID);							// value
				k += OSD_GetProgBar(&pSTR[k], stMenuInfo.cID, stMenuInfo.Max);				// prog. bar
			}
			else	{
				if(OSD_GetMenuLang()==1) 	stMenuInfo.pItem += (MenuItemCol * stMenuInfo.Max); 	// if chinese font..
				k += OSD_GetCombString(&pSTR[k], stMenuInfo.pItem, stMenuInfo.cID, MenuItemCol);	// item
			}
		}
	}

	return k;
}

//---------------------------------------------------------------------------------------
#if defined(__USE_CHN_FONT__)
static void OSD_DrawFont(WORD y, WORD x, PWORD pBuff, BYTE len, BYTE attb, SPRITE_INDEX_t sprite)
#else
static void OSD_DrawFont(WORD y, WORD x, PBYTE pBuff, BYTE len, BYTE attb, SPRITE_INDEX_t sprite)
#endif
{
	BYTE  i, str_bgn=0;
	BYTE  prev_font=0, cur_font=0;
	BYTE  bSTR[100];//bSTR[len];
	
	for(i=0;i<len;i++)	{
		bSTR[i] = pBuff[i] % 256;
		cur_font = pBuff[i] / 256; 						// 0:eng, 1:chn-1, 2:chn-2,,
		
		if(prev_font != cur_font)	{ 						// check string type(eng? chn?)
			OSD_SetFontLang(prev_font);						// set font map address(0:eng, 1:chn,,)
			OSD_SetFontGAC(sprite);							// set sprite(main osd)
#if	OSD_AUX_EN
			if(sprite < SPRITE_INDEX2) 	OSD_SetFontGAC((SPRITE_INDEX_t)(SPRITE_INDEX8+sprite));	// set sprite(aux osd)
			if(sprite == SPRITE_INDEX4)	OSD_SetFontGAC((SPRITE_INDEX_t)(SPRITE_INDEXA));		// set sprite(aux cam id osd)
			if(sprite == SPRITE_INDEX5)	OSD_SetFontGAC((SPRITE_INDEX_t)(SPRITE_INDEXB));		// set sprite(aux message osd)
#endif
			
			MISPGAC_SetDrawXYMode(&stFONT, y, x + str_bgn, &bSTR[str_bgn], i-str_bgn, attb);	// draw prev. string
			str_bgn = i;		prev_font = cur_font;
		}
	}
	
	OSD_SetFontLang(prev_font);								// set font map address(0:eng, 1:chn,,)
	OSD_SetFontGAC(sprite);									// set sprite(main osd)
#if	OSD_AUX_EN
	if(sprite < SPRITE_INDEX2) 	OSD_SetFontGAC((SPRITE_INDEX_t)(SPRITE_INDEX8+sprite));	// set sprite(aux osd)
	if(sprite == SPRITE_INDEX4)	OSD_SetFontGAC((SPRITE_INDEX_t)(SPRITE_INDEXA));		// set sprite(aux cam id osd)
	if(sprite == SPRITE_INDEX5)	OSD_SetFontGAC((SPRITE_INDEX_t)(SPRITE_INDEXB));		// set sprite(aux message osd)
#endif

	MISPGAC_SetDrawXYMode(&stFONT, y, x + str_bgn, &bSTR[str_bgn], i-str_bgn, attb);	// draw remained string
//	for(i=0;i<len;i++)	if(bSTR[i] < 0x80)	UARTprintf("%c", bSTR[i]);		UARTprintf("\r\n");
}

//---------------------------------------------------------------------------------------
static void OSD_SetMenuRepeatAuto(void)
{
#if OSD_REPEAT_EN
	BYTE h_rpt=OFF, v_rpt=OFF;
	WORD Hact=GetOUT_MainSizeHA(), Vact=GetOUT_MainSizeVA();
	WORD MenuW=OSD_FONT_W*28, MenuH=OSD_FONT_H*14;
	
	if(MenuW *2 + 100 <= Hact)		h_rpt = 1;	// repeat(2x)
	//if(MenuW *4 + 100 <= Hact)		h_rpt = 2;	// repeat(4x)
	
	if(MenuH *2 + 100 <= Vact)		v_rpt = 1;	// repeat(2x)
	//if(MenuH *4 + 100 <= Vact)		v_rpt = 2;	// repeat(4x)
	
	MISPOSD_EnableLayerRepeat(&stLayer[LAYER_INDEX0], h_rpt, v_rpt);
#if !defined(__USE_OSD_TITLE__)
	MISPOSD_EnableLayerRepeat(&stLayer[LAYER_INDEX1], h_rpt, v_rpt);
#endif

#if OSD_AUX_EN
	if((GetDZOOM_OnOff()) || GetOSDMenuID()==DZMODE_MENU)		return;		// skip aux repeat when dzoom on
	
	h_rpt = OFF;				v_rpt = OFF;
	Hact=GetOUT_AuxSizeHA();	Vact=GetOUT_AuxSizeVA();
	
	if(MenuW *2 + 100 <= Hact)		h_rpt = 1;	// repeat(2x)
	//if(MenuW *4 + 100 <= Hact)		h_rpt = 2;	// repeat(4x)
	
	if(MenuH *2 + 100 <= Vact)		v_rpt = 1;	// repeat(2x)
	//if(MenuH *4 + 100 <= Vact)		v_rpt = 2;	// repeat(4x)
	
	MISPOSD_EnableLayerRepeat(&stLayer[LAYER_INDEX2], h_rpt, v_rpt);
#endif

#endif
}

//---------------------------------------------------------------------------------------
void OSD_InfoDisplay(BYTE mode, PBYTE pSTR1, PBYTE pSTR2)
{
	BYTE i, attb = MAKEBYTE(OSD_WHITE_OP, OSD_BLACK_OP);
	
#if defined(__USE_CHN_FONT__)
	WORD pSTR[80];
#else
	BYTE pSTR[80];
#endif
	
	if (mode==0)	{ MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX2], OFF);		return; }		
	
	OSD_SetMenuRepeatAuto();
	
	OSD_SetFontGAC(SPRITE_INDEX2);									// set sprite(main osd)

	memset(pSTR, 0, sizeof(pSTR));								// clear buffer
	OSD_DrawFont(0, 0, pSTR, 40*2, 0xff, SPRITE_INDEX2);		// clear osd
	
	// text-1
	for(i=0; i<10; i++)		pSTR[i] = pSTR1[i];
	OSD_DrawFont(0, 0, pSTR, 10, attb, SPRITE_INDEX2);			// write osd
	
	// text-2
	if(mode==2)	{
		for(i=0;i<10;i++)	pSTR[i] = pSTR2[i];
#if OSD_REPEAT_EN
		if(GetOUT_MainSizeHA() >= 3840)	{							// for 4k v.out
			OSD_DrawFont(0, (40-10)*2, pSTR, 10, attb, SPRITE_INDEX2);	// write osd
		}
		else	{
			OSD_DrawFont(0, (40-10)*1, pSTR, 10, attb, SPRITE_INDEX2);	// write osd
		}
#else
		OSD_DrawFont(0, (40-10)*2, pSTR, 10, attb, SPRITE_INDEX2);	// write osd
#endif
	}
	
#if OSD_MAIN_EN
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX2], ON);
#endif
}

//---------------------------------------------------------------------------------------
void OSD_IrkeyTitleDisplay(BOOL OnOff, PBYTE pSTR1)
{
	OSD_InfoDisplay(OnOff, pSTR1, NULL);
}

//---------------------------------------------------------------------------------------
void OSD_DrawOutlineQuad(BOOL OnOff, WORD w, WORD h)
{
	BYTE	offy=0, offx=0;
	MISP_COLORRGB color = RGB(110,41,240);	// blue
	
	if(OnOff)	{
#if defined(__USE_MDIN_i500__)
		switch(h)	{
			case  1536: 	offy=28; 	offx=2; 	break; 	// 1536p out
			case  1080: 	offy=28; 	offx=2; 	break; 	// 1080p out
			case  720: 	offy=14; 	offx=2; 	break; 	// 720p out
			default: 		offy=28; 	offx=2; 	return;	//break;
		}
#endif
		
		MISPOSD_SetSBoxArea(&stSBOX[SBOX_INDEX0],   0,   0+offy,   w/2,  h/2+offy); 	// left_top
		MISPOSD_SetSBoxArea(&stSBOX[SBOX_INDEX1], w/2,   0+offy, w-offx,  h/2+offy); 	// right_top
		MISPOSD_SetSBoxArea(&stSBOX[SBOX_INDEX2],   0, h/2+offy,   w/2,  h+offy); 		// left_bottom
		MISPOSD_SetSBoxArea(&stSBOX[SBOX_INDEX3], w/2, h/2+offy, w-offx,  h+offy); 	// right_bottom
		MISPOSD_SetSBoxBorderColor(&stSBOX[SBOX_INDEX0], color);	MISPOSD_SetSBoxBorderColor(&stSBOX[SBOX_INDEX1], color);
		MISPOSD_SetSBoxBorderColor(&stSBOX[SBOX_INDEX2], color);	MISPOSD_SetSBoxBorderColor(&stSBOX[SBOX_INDEX3], color);
	}
	
	MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX0], OnOff);	MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX1], OnOff);
	MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX2], OnOff);	MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX3], OnOff);
}

//---------------------------------------------------------------------------------------
void OSD_DrawOutlineDouble(BOOL OnOff, WORD w, WORD h)
{
	BYTE	offy=0, offx=0;
	MISP_COLORRGB color = RGB(128,318,128);	// gray	//RGB(110,41,240);	// blue
	
	if(OnOff)	{
#if defined(__USE_MDIN_i500__)
		switch(h)	{
			case  1536: 	offy=28; 	offx=2; 	break; 	// 1536p out
			case  1080: 	offy=28; 	offx=2; 	break; 	// 1080p out
			case  720: 	offy=14; 	offx=2; 	break; 	// 720p out
			default: 		offy=28; 	offx=2; 	return;	//break;
		}
#endif
		
		MISPOSD_SetSBoxArea(&stSBOX[SBOX_INDEX0],   0,   0+offy,   w/2,  h+offy); 	// left
		MISPOSD_SetSBoxArea(&stSBOX[SBOX_INDEX1], w/2,   0+offy, w-offx,  h+offy); 	// right
		MISPOSD_SetSBoxBorderColor(&stSBOX[SBOX_INDEX0], color);
		MISPOSD_SetSBoxBorderColor(&stSBOX[SBOX_INDEX1], color);
		MISPOSD_SetSBoxBorderThickH(&stSBOX[SBOX_INDEX0], 0);
		MISPOSD_SetSBoxBorderThickV(&stSBOX[SBOX_INDEX0], 0);
		MISPOSD_SetSBoxBorderThickH(&stSBOX[SBOX_INDEX1], 0);
		MISPOSD_SetSBoxBorderThickV(&stSBOX[SBOX_INDEX1], 0);
	}
	else	{
		MISPOSD_SetSBoxBorderThickH(&stSBOX[SBOX_INDEX0], 1);
		MISPOSD_SetSBoxBorderThickV(&stSBOX[SBOX_INDEX0], 1);
		MISPOSD_SetSBoxBorderThickH(&stSBOX[SBOX_INDEX1], 1);
		MISPOSD_SetSBoxBorderThickV(&stSBOX[SBOX_INDEX1], 1);
	}
	
	MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX0], OnOff);
	MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX1], OnOff);
}

//---------------------------------------------------------------------------------------
void OSD_CHNoDisplay(BOOL OnOff, WORD w, WORD h)
{
	BYTE	CH1[]="CH1", CH2[]="CH2", CH3[]="CH3", CH4[]="CH4";
	BYTE	i, attb=MAKEBYTE(OSD_GRAY, OSD_BLACK_OP);
	
#if defined(__USE_CHN_FONT__)
	WORD pSTR[40];
#else
	BYTE pSTR[40];
#endif
	
	if(h != 1080)				return; 	// support only 1080p out
	
	if(OnOff)	{
		OSD_SetMenuRepeatAuto();
		
		//OSD_SetFontMAP();
		memset(pSTR, 0, sizeof(pSTR));				OSD_DrawFont(0,  0, pSTR, 40, 0xff, SPRITE_INDEX2);
		for(i=0;i<3;i++)	pSTR[i] = CH1[i];		OSD_DrawFont(0,  0, pSTR,  3, attb, SPRITE_INDEX2);
		for(i=0;i<3;i++)	pSTR[i] = CH2[i];		OSD_DrawFont(0, 30, pSTR,  3, attb, SPRITE_INDEX2);
		MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX2], ON);
		
		memset(pSTR, 0, sizeof(pSTR));				OSD_DrawFont(0,  0, pSTR, 40, 0xff, SPRITE_INDEX3);
		for(i=0;i<3;i++)	pSTR[i] = CH3[i];		OSD_DrawFont(0,  0, pSTR,  3, attb, SPRITE_INDEX3);
		for(i=0;i<3;i++)	pSTR[i] = CH4[i];		OSD_DrawFont(0, 30, pSTR,  3, attb, SPRITE_INDEX3);
		MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX3], ON);
	}
	else	{
		MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX2], OFF);
		MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX3], OFF);
	}
}

//---------------------------------------------------------------------------------------
void OSD_MessageBox(BOOL OnOff, PBYTE pSTR1)
{
	BYTE i, cnt, attb=MAKEBYTE(OSD_GRAY, OSD_GRAY);
	BYTE h_rpt, v_rpt;
	WORD osd_w, osd_h, out_w, out_h;
#if defined(__USE_CHN_FONT__)
	WORD pSTR[20];
#else
	BYTE pSTR[20];
#endif
	
	if (OnOff==OFF)	{
#if OSD_MAIN_EN
		MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX5], OFF);
#endif
#if OSD_AUX_EN
		MISPOSD_EnableSprite(&stOSD[SPRITE_INDEXB], OFF);
		if(OSDAuxHideSprite >= SPRITE_INDEX8)	{
			MISPOSD_EnableSprite(&stOSD[OSDAuxHideSprite], ON);			OSDAuxHideSprite = 0;
		}
#endif
		return;
	}		
	
	OSD_SetMenuRepeatAuto();
	
	memset(pSTR, 0, sizeof(pSTR));							// clear buffer
	OSD_DrawFont(0, 0, pSTR, 20, 0xff, SPRITE_INDEX5);		// clear osd
	
	if(OSD_GetMenuLang()==1)	{
		for(cnt=0;cnt<20;cnt++)		if(((PWORD)pSTR1)[cnt]==0)	break;	// count text
		for(i=0;i<cnt*2;i++)		((PBYTE)pSTR)[i] = pSTR1[i];
	}
	else	{
		for(cnt=0;cnt<20;cnt++)		if(pSTR1[cnt]==0)	break;	// count text
		for(i=0;i<cnt;i++)			pSTR[i] = pSTR1[i];
	}
	
	OSD_DrawFont(0, 0, pSTR, cnt, attb, SPRITE_INDEX5);	// write osd
	
	// adjust message osd position
	MISPOSD_GetLayerRepeat(&stLayer[LAYER_INDEX0], &h_rpt, &v_rpt);	// 0:off, 1:2x, 2:4x
	osd_w = stOSD[SPRITE_INDEX5].stBMP.w << h_rpt;
	osd_h = stOSD[SPRITE_INDEX5].stBMP.h << v_rpt;
	out_w = GetOUT_MainSizeHA();
	out_h = GetOUT_MainSizeVA();
	stOSD[SPRITE_INDEX5].x = (out_w - osd_w) / 2;
	stOSD[SPRITE_INDEX5].y = (out_h - osd_h) / 2;
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX5], SPRITE_INDEX5);
	
	// enable message osd
#if OSD_MAIN_EN
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX5], ON);
#endif

#if OSD_AUX_EN
	// disable aux menu osd
	if(stOSD[SPRITE_INDEX8].ctrl & SPRITE_DISPLAY_ON)	{
		MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX8], OFF);		OSDAuxHideSprite = SPRITE_INDEX8;
	}
	if(stOSD[SPRITE_INDEX9].ctrl & SPRITE_DISPLAY_ON)	{
		MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX9], OFF);		OSDAuxHideSprite = SPRITE_INDEX9;
	}
	
	// adjust message osd position
	MISPOSD_GetLayerRepeat(&stLayer[LAYER_INDEX2], &h_rpt, &v_rpt);	// 0:off, 1:2x, 2:4x
	osd_w = stOSD[SPRITE_INDEXB].stBMP.w << h_rpt;
	osd_h = stOSD[SPRITE_INDEXB].stBMP.h << v_rpt;
	out_w = GetOUT_AuxSizeHA();
	out_h = GetOUT_AuxSizeVA();
	stOSD[SPRITE_INDEXB].x = (out_w - osd_w) / 2;
	stOSD[SPRITE_INDEXB].y = (out_h - osd_h) / 2;
	stOSD[SPRITE_INDEXB].x /= ((GetOUT_AuxMode()==MISP_OUT_MUX656_8) || (GetOUT_AuxMode()==MISP_OUT_MUX656_10))?  2:  1;	// aux sprite position(mux656 : *2)
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEXB], SPRITE_INDEXB);

	// enable message osd
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEXB], ON);
#endif

	fMessageOSD=1;		cntMessageOSD=0;
}

//---------------------------------------------------------------------------------------
void OSD_CamTitleDisplay(BOOL OnOff)
{
	BYTE i, attb;
	
#if defined(__USE_CHN_FONT__)
	WORD pSTR[10];
#else
	BYTE pSTR[10];
#endif
	
#if OSD_REPEAT_EN
	WORD title_w = stOSD[SPRITE_INDEX4].stBMP.w *2;
#else
	WORD title_w = stOSD[SPRITE_INDEX4].stBMP.w;
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)OnOff;
	EEPROM_MultiWrite(EEPROM_SETUP_CTITLE, (PBYTE)&buff[0], sizeof(buff));
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	if(OnOff)	{
		for(i=0;i<10;i++)	SPImAPP_SetSlaveCamTitleTxt(i, CamTitleText[0][i]);	// send text
		SPImAPP_SetSlaveCamTitleColor(stMenuData.CamTitle[1]);
		SPImAPP_SetSlaveCamTitleDisplay(ON);
	}
	else	SPImAPP_SetSlaveCamTitleDisplay(OFF);
	
	return;
#elif defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD2__)	// 4k60 slave only
	SPIsAPP_GetMasterCamTitleTxt(&CamTitleText[0][0]);
	SPIsAPP_GetMasterCamTitleColor(&stMenuData.CamTitle[1]);
#endif
	
	if (OnOff==OFF)	{
#if OSD_MAIN_EN
		MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX4], OFF);
#endif
#if OSD_AUX_EN
		MISPOSD_EnableSprite(&stOSD[SPRITE_INDEXA], OFF);
#endif
		return;
	}		
	
	OSD_SetMenuRepeatAuto();
	
	// set osd color(text & backgroud)
	attb = ((stMenuData.CamTitle[1] << 4) | OSD_BLACK_OP);
	
	// convert osd text to font index
	memset(pSTR, 0, sizeof(pSTR));
	for(i=0;i<10;i++) 	if(CamTitleText[0][i] != TEXT_CURSOR) 	pSTR[i] = CamTitleText[0][i];
	
	// write text
	OSD_DrawFont(0, 0, pSTR, 10, attb, SPRITE_INDEX4);
	
	// adjust osd position
	stOSD[SPRITE_INDEX4].x = (GetOUT_MainSizeHA() - title_w) - 40;
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEX4], SPRITE_INDEX4);
	
	// osd display enable
#if OSD_MAIN_EN
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX4], ON);
#endif
	
#if OSD_AUX_EN
	// adjust osd position
	stOSD[SPRITE_INDEXA].x = (GetOUT_AuxSizeHA() - title_w) - 40;
	stOSD[SPRITE_INDEXA].x /= ((GetOUT_AuxMode()==MISP_OUT_MUX656_8) || (GetOUT_AuxMode()==MISP_OUT_MUX656_10))?  2:  1;	// aux sprite position(mux656 : *2)
	stOSD[SPRITE_INDEXA].x -= ((GetOUT_AuxMode()==MISP_OUT_MUX656_8) || (GetOUT_AuxMode()==MISP_OUT_MUX656_10))?  40:  0;
	MISPOSD_SetSpriteConfig(&stOSD[SPRITE_INDEXA], SPRITE_INDEXA);
	
	// osd display enable
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEXA], ON);
#endif
}

//---------------------------------------------------------------------------------------
BYTE OSD_GetCamTitleDisplay(void)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
	
	EEPROM_MultiRead(EEPROM_SETUP_CTITLE, (PBYTE)&buff[0], sizeof(buff));
	
	if((buff[0]==EEPROM_SAVE_DONE) && (buff[1]))	return 1;
	else											return 0;
#endif
#else
	if(stOSD[SPRITE_INDEX4].ctrl & SPRITE_DISPLAY_ON)		return  1;
	else 													return  0;
#endif
}

//---------------------------------------------------------------------------------------
void OSD_StrDispAttb(BYTE y, BYTE x, BOOL OnOff, BYTE color, const char* format, ...)
{
	BYTE i, cnt, hmax=(36-x), vmax=19;	// 36 x 19
	CHAR pSTR[100];//pSTR[hmax];
	va_list arg_ptr;
	//int arg_cnt;
	
#if defined(__USE_CHN_FONT__)
	WORD pDST[36];//pDST[hmax];
#else
	BYTE pDST[36];//pDST[hmax];
#endif
	
	memset(pSTR, 0, sizeof(pSTR));									// clear buffer
	memset(pDST, 0, sizeof(pDST));									// clear buffer
	
	va_start(arg_ptr, format);
	//arg_cnt = vsprintf(pSTR, format, arg_ptr);
	vsprintf(pSTR, format, arg_ptr);
//	UARTprintf("[%d]%s\r\n", arg_cnt, pSTR);
	va_end(arg_ptr);
	
	for(cnt=0;cnt<hmax;cnt++)		if(pSTR[cnt]==0)	break;		// count text
	
	if(OnOff)	{													// on(write text), off(clear text)
		for(i=0;i<cnt;i++)			pDST[i] = pSTR[i];
	}
	
	if(y < vmax)	{OSD_DrawFont(y, x, pDST, MIN(hmax,cnt), color, SPRITE_INDEX6);}	// write osd
}

//------------------------------------------------------------------------------------
void OSD_StrDispOnOff(BOOL OnOff)
{
	OSD_SetMenuRepeatAuto();
	
#if OSD_MAIN_EN
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX6], OnOff);
#endif
}

//------------------------------------------------------------------------------------
#if defined(__USE_CHN_FONT__)
static void OSD_ConvDZoomRatio(PWORD pSTR)
#else
static void OSD_ConvDZoomRatio(PBYTE pSTR)
#endif
{
	WORD ratio = ConvDZOOM_MenuRatio(GetMenuStatus(DZRATIO_MENU));
	
	if(ratio >= 1000)	{	// over x10
		pSTR[0] = 'x';
		pSTR[1] = '0' + (ratio / 1000);
		pSTR[2] = '0' + ((ratio % 1000) / 100);
		pSTR[3] = '.';
		pSTR[4] = '0' + ((ratio % 100) / 10);
	}
	else	{				// x1.00 ~ x9.99
		pSTR[0] = 'x';
		pSTR[1] = '0' + (ratio / 100);
		pSTR[2] = '.';
		pSTR[3] = '0' + ((ratio % 100) / 10);
		pSTR[4] = '0' + ((ratio % 100) % 10);
	}
}

//------------------------------------------------------------------------------------
static void OSD_MenuSubDisplay(void)		//size/position..
{
	PBYTE pID = (PBYTE)&OSDMenuID;
	BYTE zID = LO4BIT(pID(1))-1;		
	BYTE i, attb;
	BYTE Menu_Color, Menu_H_Color;
	
#if defined(__USE_CHN_FONT__)
	WORD pSTR[36];
#else
	BYTE pSTR[36];
#endif
	
	Menu_Color = ((stMenuData.MenuColor[0] << 4) | stMenuData.MenuColor[2]);	// Text/Bg color
	Menu_H_Color = ((stMenuData.MenuColor[1] << 4) | stMenuData.MenuColor[3]);	// Text/Bg color(Highlight)

	for (i=0; i<3; i++) {
		attb = (i==zID)? Menu_H_Color : Menu_Color;

		memset(pSTR, 0, sizeof(pSTR));					// clear buffer

		OSD_GetMenuString(&pSTR[0], i+MenuTitleRow);	// Left/Right Menu string, sub menu(title x)
		if(i==zID)	pSTR[0] = 0x0a;						// '*'

		OSD_DrawFont(0, i*12, pSTR, 12, attb, SPRITE_INDEX1);	// Row 1
	}
	
	memset(pSTR, 0, sizeof(pSTR));						// clear buffer

	if ((OSDMenuID | 0x0f) == DZRATIO_TREE)	{			// for dzoom ratio
		OSD_ConvDZoomRatio(pSTR);
		OSD_DrawFont(1, 0, pSTR, 36, Menu_Color, SPRITE_INDEX1);	// Row 2
	}
	else	OSD_DrawFont(1, 0, pSTR, 36, 0xFF, SPRITE_INDEX1);	// Row 2(blank)
}

//--------------------------------------------------------------------------------------------------------------------------
static void OSD_MenuMainDisplay(void)
{
	BYTE i, j, attb;
	BYTE nID, level;
	BYTE ColMax = (MenuTitleCol + MenuMargin*2);
	BYTE Menu_Color, Menu_H_Color, row_st, row_end;
	
#if defined(__USE_CHN_FONT__)
	WORD pSTR[28];
#else
	BYTE pSTR[28];
#endif
	
	level = OSD_GetMenuLevel(OSDMenuID);
	nID = ((OSDMenuID >> GetWeight(level)) & 0x0f) +MenuTitleRow -1; 	// cursor row

//	UARTprintf("prev.mID[%04x] -> curr.mID[%04x], lev[%d] \r\n", 
//					OSDpMenuID, OSDMenuID, OSD_GetMenuLevel(OSDMenuID) );

	Menu_Color = ((stMenuData.MenuColor[0] << 4) | stMenuData.MenuColor[2]);	// Text/Bg color
	Menu_H_Color = ((stMenuData.MenuColor[1] << 4) | stMenuData.MenuColor[3]);	// Text/Bg color(Highlight)
	
	if(((OSDpMenuID >> GetWeight(level)) & 0x0f) == 0)		fMenuDrawAll = ON; 	// enter sub. menu : prevID(lev) == 0
	if(((OSDpMenuID >> GetWeight(level+1)) & 0x0f) != 0)	fMenuDrawAll = ON; 	// exit sub. menu : prevID(lev+1) != 0
	
	if(fMenuDrawAll == ON) 	{ 						// menu draw all line(0~13)
		row_st = 0;		row_end = MenuTotalRow;
		fMenuDrawAll = OFF;
	}
	else	{ 											// menu draw 2 line(prev-cursor line & current-cursor line)
		row_st = ((OSDpMenuID >> GetWeight(level)) & 0x0f) +MenuTitleRow -1; 	// prev-cursor line no
		row_end = MAX(row_st, nID) +1; 										// nID : current-cursor line no.
		row_st = MIN(row_st, nID);
	}
	OSDpMenuID = OSDMenuID;
	
	for (i=row_st; i<row_end; i++) {					// 1st:0~3, 2nd:4~7, 3rd:8~11
		attb = (i==nID)? Menu_H_Color : Menu_Color;	// font/board color set, defMENU_COLOR=0x71

		memset(pSTR, 0, sizeof(pSTR));				// clear buffer

		j = 0;
		j += OSD_GetMenuString(&pSTR[0], i);		// Left/Right Menu string
			
		if(i==nID)	pSTR[0] = 0x2a;					// '*'
		if(stMenuInfo.Attb & (MENU_SKIP | MENU_TEXT))		attb = (OSD_GRAY<<4)| (Menu_Color&0x0f);
		
		if(j > 0)	OSD_DrawFont(i, 0, pSTR, j, attb, SPRITE_INDEX0);				//Left/Right Menu
		else		OSD_DrawFont(i, 0, pSTR, ColMax, 0xFF, SPRITE_INDEX0);		//menu blank line(clear)
		
//		for(j=0;j<28;j++)	if(pSTR[j] < 0x80)	UARTprintf("%c", pSTR[j]);		UARTprintf("\r\n");
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void OSD_MenuDisplay(void)
{
	PBYTE pID = (PBYTE)&OSDMenuID;

	if (!LO4BIT(pID(1)))	OSD_MenuMainDisplay();
	else					OSD_MenuSubDisplay();
	
#if OSD_MAIN_EN
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX0], (pID(1)&0x0f)? OFF : ON);	//Lv1~3 : ON, Lv4 : OFF
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX1], (pID(1)&0x0f)? ON : OFF);	//Lv4 : ON 
#endif
	
	OSD_SetMenuRepeatAuto();
	
#if	OSD_AUX_EN
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX8], (pID(1)&0x0f)? OFF : ON);	//Lv1~3 : ON, Lv4 : OFF
	MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX9], (pID(1)&0x0f)? ON : OFF);	//Lv4 : ON 
#endif

#if __MISP100_DBGPRT__ == 1
	UARTprintf("mID[%04x], lev[%d] \n",	OSDMenuID, OSD_GetMenuLevel(OSDMenuID) );
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void MenuDisplayHandler(void)
{
	if ((fMessageOSD | fMenuWaitFunc) && (cntMessageOSD >= 2000))	{			// wait 2 sec
		if(fMessageOSD)		OSD_MessageBox(OFF, NULL);
		if(fMenuWaitFunc)	fMenuUpdate=ON;
		
		fMessageOSD=OFF;	fMenuWaitFunc=OFF;	cntMessageOSD=0;
	}
	
	if (fMenuUpdate==OFF) return;
	else	   fMenuUpdate = OFF;

	if (OSDMenuID==0)	OSD_MenuClear();		// menu clear
	else				OSD_MenuDisplay();
}

//--------------------------------------------------------------------------------------------------
void SetOSDMenuID(WORD nID)
{
	OSDMenuID = nID;
	fMenuUpdate = ON;
}

//--------------------------------------------------------------------------------------------------
WORD GetOSDMenuID(void)
{
	return OSDMenuID;
}

//--------------------------------------------------------------------------------------------------
void SetOSDCombID(WORD nID)
{
	OSDCombID = nID;
}

//--------------------------------------------------------------------------------------------------
WORD GetOSDCombID(void)	
{
	return OSDCombID;
}

//--------------------------------------------------------------------------------------------------
WORD GetMenuPosiY(WORD mID)
{
	OSD_GetMenuItemInfo(&stMenuInfo, mID);				//Get Menu item info. from cur. menu.
	
	return stMenuInfo.pos_y;
}

//--------------------------------------------------------------------------------------------------
WORD GetMenuStatus(WORD mID)
{
	OSD_GetMenuItemInfo(&stMenuInfo, mID);				//Get Menu item info. from cur. menu.
	
	return stMenuInfo.cID;
}

//--------------------------------------------------------------------------------------------------
WORD GetMenuItemMax(WORD mID)
{
	OSD_GetMenuItemInfo(&stMenuInfo, mID);				//Get Menu item info. from cur. menu.
	
	return stMenuInfo.Max;
}

//--------------------------------------------------------------------------------------------------
void SetMenuStatus(WORD mID, BYTE val)
{
	BYTE	level, nID, sID, tID, zID, idx=0;

	nID = HI4BIT(HIBYTE(mID)) -1;		sID = LO4BIT(HIBYTE(mID)) -1;
	tID = HI4BIT(LOBYTE(mID)) -1;		zID = LO4BIT(LOBYTE(mID)) -1;
	
	level = OSD_GetMenuLevel(mID);
	switch(level)	{
		case 1:	idx=nID;	break;		case 2:	idx=sID;	break;
		case 3:	idx=tID;	break;		case 4:	idx=zID;	break;
	}

	switch(mID | MenuTreeMask(level)) {
// level-1
		case MAIN_TREE: 		stMenuData.Main[idx] = val; 		break;		// main
		
// level-2
		case FOCUS_TREE: 		stMenuData.Focus[idx] = val; 		break;		// focus
		case EXPOSURE_TREE: 	stMenuData.Exposure[idx] = val; 	break;		// exposure
		case WB_TREE:			stMenuData.WB[idx] = val;			break; 	// wb
		case DYNT_TREE: 		stMenuData.DYNT[idx] = val; 		break;		// day & night
		case DNR_TREE: 			stMenuData.DNR[idx] = val; 			break;		// dnr
		case PM_TREE: 			stMenuData.PM[idx] = val; 			break;		// pm
		case EFFECT_TREE: 		stMenuData.Effect[idx] = val; 		break;		// effect
		case SPECIAL_TREE: 		stMenuData.Special[idx] = val; 		break;		// special
		case SETUP_TREE: 		stMenuData.Setup[idx] = val; 		break;		// setup
		case ENGR_TREE: 		stMenuData.Engr[idx] = val; 		break;		// engineer menu
		case DEBUG_TREE: 		stMenuData.Debug[idx] = val; 		break;		// debug menu
	
// level-3
		case CONFER_TREE :		stMenuData.Confer[idx] = val; 		break;		// af conference menu
		case GMC_TREE:		stMenuData.GMC[idx] = val; 		break;		// defog
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)	
		case DEFOGDWDR_TREE :	stMenuData.DefogDwdr[idx] = val; 	break;		// defog & dwdr
#else
		case DEFOG_TREE :		stMenuData.Defog[idx] = val; 		break;		// defog
		case DWDR_TREE :		stMenuData.DWDR[idx] = val; 		break;		// dwdr
#endif			
			
//		case BLC_TREE:													// blc/hsblc
		case HSBLC_TREE: 	if (stMenuData.Special[sID] == 1)	stMenuData.BLC[idx] = val;
					 		else								stMenuData.HSBLC[idx] = val;	break;
		case MD_TREE: 			stMenuData.MD[idx] = val; 			break;		// md
		case DZ_TREE: 			stMenuData.DZoom[idx] = val; 		break;		// dzoom
		case DPIXEL_TREE: 		stMenuData.DPC[idx] = val; 			break;		// dpc
		case DISPFUNC_TREE: 	stMenuData.DispFunc[idx] = val; 	break;		// disp func
		case CAMTITLE_TREE: 	stMenuData.CamTitle[idx] = val; 	break;		// cam title
		case RS485_TREE: 		stMenuData.RS485[idx] = val; 		break;		// rs485
		case MENUCOLOR_TREE: 	stMenuData.MenuColor[idx] = val; 	break;		// menu color
		case FRESET_TREE: 		stMenuData.FReset[idx] = val; 		break;		// factory reset
		case SYSINFO_TREE: 		stMenuData.SysInfo[idx] = val; 		break;		// system info
		case COLOR_CORR_TREE: 	stMenuData.ColorCorr[idx] = val; 	break;		// ColorCorrection
		case TP_TREE:			stMenuData.TP[idx]=val;				break;		// sensor tp
		case MISC_TUNE_TREE: 	stMenuData.MiscTune[idx] = val; 	break;		// misc(tuning)
		case WDR_TUNE_TREE:
			switch(GetWDR_ProcesMode()) {
				case MISP_WDR_PROC_LINE3:  stMenuData.L3WDRSet[idx]=val;  break;  // wdr (Line by Line - 3Frame)
				case MISP_WDR_PROC_LINE2:  stMenuData.L2WDRSet[idx]=val;  break;  // wdr (Line by Line - 2Frame)
				case MISP_WDR_PROC_BUILT:  stMenuData.TMWDRSet[idx]=val;  break;  // wdr (Tone map only)
				case MISP_WDR_PROC_FRAME:  stMenuData.FRWDRSet[idx]=val;  break;  // wdr (Frame by Frame)
				case MISP_WDR_PROC_SINGL:  stMenuData.DGWDRSet[idx]=val;  break;  // wdr (DWDR)
				default:                                                  break;  // wdr (off)
			}
			break;
		case DNR_TUNE_TREE: 	stMenuData.DNRTune[idx] = val; 		break;		// dnr(tuning)
		case VS_TREE: 			stMenuData.VS[idx] = val; 			break;		// vector scope
		case FS_TREE: 			stMenuData.FS[idx] = val; 			break;		// factory setup
		case CCF_TREE: 			stMenuData.CCF[idx] = val; 			break;		// factory setup
		
// level-4
		case BLCSIZE_TREE: 		stMenuData.BLCSizPos[idx] = val; 	break;		// blc size/position
		case HSBLCSIZE_TREE: 	stMenuData.HSBLCSizPos[idx] = val; 	break;		// hsblc size/position
		case MDSIZE_TREE: 		stMenuData.MDSizPos[idx] = val; 	break;		// md size/position
		case DZRATIO_TREE: 		stMenuData.DZCtrl[idx] = val; 		break;		// dz ratio & position
		case PIPPOSI_TREE: 		stMenuData.PIPPos[idx] = val; 		break;		// pip position
		case TPSIZE_TREE: 		stMenuData.TPSizPos[idx] = val; 	break;		// sensor tp size/position
		// case PICSTYLE_TREE: 	
			// switch(GetApp_ssPictureStyle()) {
				// case MDIN_PS_STANDARD: stMenuData.PicStyleSTD[idx]=val; break;								
				// case MDIN_PS_VIVID:	stMenuData.PicStyleVID[idx]=val; break;
				// case MDIN_PS_CLEAR:	stMenuData.PicStyleCLR[idx]=val; break;
				// case MDIN_PS_VIVID_CLEAR: stMenuData.PicStyleVCLR[idx]=val; break;
				// case MDIN_PS_NEUTRAL: stMenuData.PicStyleNEU[idx]=val; break;						
				// default: break;
			// }
			// break;		// picture style
	}
}

//--------------------------------------------------------------------------------------------------
void SetMenuPosiY(WORD mID, BYTE val)
{
	BYTE	level, zID;

	zID = LO4BIT(LOBYTE(mID)) -1;
	level = OSD_GetMenuLevel(mID);

	switch(mID | MenuTreeMask(level)) {
		case BLCSIZE_TREE: 		stMenuData.BLCSizPosY[zID%2] = val; 	break;	//0:position, 1:size
		case HSBLCSIZE_TREE: 	stMenuData.HSBLCSizPosY[zID%2] = val;	break;	//0:position, 1:size
		case MDSIZE_TREE: 		stMenuData.MDSizPosY[zID%2] = val; 		break;	//0:pisition, 1:size
		case DZRATIO_TREE: 		stMenuData.DZCtrlY[zID%2] = val; 		break;	//0:position
		case PIPPOSI_TREE: 		stMenuData.PIPPosY[0] = val; 			break;	//0:position
		case TPSIZE_TREE: 		stMenuData.TPSizPosY[zID%2] = val; 		break;	//0:position, 1:size		
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SetMenuDefaultItem(WORD mID)
{
	PBYTE  pID = (PBYTE)&mID;		BYTE  level, sID = LO4BIT(pID(0))-1;
	WORD  i, xID;

	OSD_GetMenuItemInfo(&stMenuInfo, mID);
	level = OSD_GetMenuLevel(mID);

	for (i=0;i<stMenuInfo.MenuRow;i++) {
		xID = mID & ~(MenuTreeMask(level));								// erase x digit
		xID |= (i+1) << GetWeight(level);								// insert xID
		
		switch(mID | MenuTreeMask(level))	{
			case  DZ_TREE :			SetMenuStatus(xID, DZMenuInfo[i].Init);	break;
			case  EFFECT_TREE : 	SetMenuStatus(xID, EffectMenuInfo[i].Init);	break;
//			case  BLC_TREE :
			case  HSBLC_TREE :	if (stMenuData.Special[sID] == 1)	{			// blc
									SetMenuStatus(xID, BLCMenuInfo[i].Init);	}
								else											// hsblc
									SetMenuStatus(xID, HSBLCMenuInfo[i].Init);
								break;
			case  MD_TREE : 		SetMenuStatus(xID, MDMenuInfo[i].Init);		break;
			case  PM_TREE : 		SetMenuStatus(xID, PMMenuInfo[i].Init);		break;
			case  TP_TREE : 		SetMenuStatus(xID, TPMenuInfo[i].Init);		break;			
		}
	}

	switch(mID)	{
		case  DZDEFAULT_MENU :	
				for(i=0;i<sizeof(stMenuData.DZCtrl);i++)	stMenuData.DZCtrl[i] = DZCtrlMenuInfo[i].Init;
				for(i=0;i<sizeof(stMenuData.PIPPos);i++)	stMenuData.PIPPos[i] = PIPPosiMenuInfo[i].Init;
				memcpy(stMenuData.DZCtrlY,	DZCtrlYInit,	sizeof(stMenuData.DZCtrlY));
				memcpy(stMenuData.PIPPosY,	PIPPosiYInit,	sizeof(stMenuData.PIPPosY));			
				break;
		case  MDDEFAULT_MENU :
				for(i=0;i<sizeof(stMenuData.MDSizPos);i++)	stMenuData.MDSizPos[i] = MDSizeMenuInfo[i].Init;
				memcpy(stMenuData.MDSizPosY,	MDSizeYInit,	sizeof(stMenuData.MDSizPosY));	
				break;
		case  TPDEFAULT_MENU : 
				for(i=0;i<sizeof(stMenuData.TPSizPos);i++)	stMenuData.TPSizPos[i] = TPSizeMenuInfo[i].Init;
				memcpy(stMenuData.TPSizPosY,	TPSizeYInit,	sizeof(stMenuData.TPSizPosY));	
				break;
	}
}


// main menu
//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuFocusInit(void)
{
#if !defined(__USE_2MOTOR_LENS__)
	
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[12];
	
	EEPROM_MultiRead(EEPROM_FOCUS_MENU, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE) 	SetAFModeOnOffMenu(MIN(buff[1], FocusMenuInfo[0].Max-1));
	if(buff[2] == EEPROM_SAVE_DONE) 	SetWIDEDrvLIMITMenu(MIN(buff[3], FocusMenuInfo[1].Max)); 	// max(16)
	if(buff[4] == EEPROM_SAVE_DONE) 	SetTELEDrvLIMITMenu(MIN(buff[5], FocusMenuInfo[2].Max)); 	// max(16)
	if(buff[6] == EEPROM_SAVE_DONE) 	SetZOOMDrvSPEEDMenu(MIN(buff[7], FocusMenuInfo[3].Max-1));
	if(buff[8] == EEPROM_SAVE_DONE) 	SetFOKSDrvLIMITMenu(MIN(buff[9], FocusMenuInfo[4].Max-1));
	if(buff[10] == EEPROM_SAVE_DONE) 	SetAFIntvalTIMEMenu(MIN(buff[11], FocusMenuInfo[5].Max)); 	// max(255)
#endif
	
	// update menu item value with api
	stMenuData.Focus[0] = GetAFModeOnOffMenu();
	stMenuData.Focus[1] = GetWIDEDrvLIMITMenu();
	stMenuData.Focus[2] = GetTELEDrvLIMITMenu();
	stMenuData.Focus[3] = GetZOOMDrvSPEEDMenu();
	stMenuData.Focus[4] = GetFOKSDrvLIMITMenu();
	stMenuData.Focus[5] = GetAFIntvalTIMEMenu();
	
#else	// __USE_2MOTOR_LENS__
	
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[6];
	
	EEPROM_MultiRead(EEPROM_TWOMOTOR_MENU, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE) 	SetTwoMotorAfMode(MIN(buff[1], FocusMenuInfo[0].Max-1));
	if(buff[2] == EEPROM_SAVE_DONE) 	SetTwoMotorScanMode(MIN(buff[3], FocusMenuInfo[1].Max-1));
	if(buff[4] == EEPROM_SAVE_DONE) 	SetTwoMotorSyncTDN(MIN(buff[5], FocusMenuInfo[3].Max-1));
#endif
	
	// update menu item value with api
	stMenuData.Focus[0] = (buff[0]==EEPROM_SAVE_DONE)?  buff[1]:  0;//GetTwoMotorAfMode();
	stMenuData.Focus[1] = (buff[2]==EEPROM_SAVE_DONE)?  buff[3]:  0;//GetTwoMotorScanMode();
	stMenuData.Focus[3] = (buff[4]==EEPROM_SAVE_DONE)?  buff[5]:  0;//GetTwoMotorSyncTDN();
	
#endif	// __USE_2MOTOR_LENS__
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuExposureInit(void)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[44], i;
#endif
	
	// set ae menu parameter with ae api
	SetAEModeDefault();
	
	// load eeprom & update 'ae menu parameter'(not api)
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	EEPROM_MultiRead(EEPROM_EXPOSURE_MENU, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0]==EEPROM_SAVE_DONE) 		stAEMENU.mode = MIN(buff[1], ExpMenuInfo[0].Max-1);
	if(buff[2]==EEPROM_SAVE_DONE) 		for(i=0;i<5;i++) 	stAEMENU.shut[i] = MIN(buff[3+i], ExpMenuInfo[1].Max-1);
	if(buff[8]==EEPROM_SAVE_DONE) 		for(i=0;i<5;i++) 	stAEMENU.iris[i] = MIN(buff[9+i], ExpMenuInfo[2].Max-1);
	if(buff[14]==EEPROM_SAVE_DONE) 		for(i=0;i<5;i++) 	stAEMENU.agc[i] = MIN(buff[15+i], ExpMenuInfo[3].Max-1);
	if(buff[20]==EEPROM_SAVE_DONE) 		for(i=0;i<5;i++) 	stAEMENU.agcmax[i] = MIN(buff[21+i], ExpMenuInfo[4].Max-1);
	if(buff[26]==EEPROM_SAVE_DONE) 		for(i=0;i<5;i++) 	stAEMENU.sensup[i] = MIN(buff[27+i], ExpMenuInfo[5].Max-1);
	if(buff[32]==EEPROM_SAVE_DONE) 		for(i=0;i<5;i++) 	stAEMENU.bright[i] = MIN(buff[33+i], ExpMenuInfo[6].Max); 	// max(20)
	if(buff[42]==EEPROM_SAVE_DONE) 		SetAEFlkMode(MIN(buff[43], ExpMenuInfo[7].Max-1));
#if defined(__USE_PWMIRIS_ONLY__)
	if(buff[40]==EEPROM_SAVE_DONE) 		SetAELensMode(MIN(buff[41], ExpMenuInfo[8].Max-1));
#endif
#endif
	
	// update menu item value with api
	stMenuData.Exposure[0] = GetAECtrlMode();
	stMenuData.Exposure[1] = GetAEShutSpeed();
	stMenuData.Exposure[2] = GetAEIrisGain();
	stMenuData.Exposure[3] = GetAEAGCGain();
	stMenuData.Exposure[4] = GetAEGainCtrlMode();
	stMenuData.Exposure[5] = OFF; 						// sens-up menu(default:'off')
	stMenuData.Exposure[6] = GetAEBrightLvl();
	stMenuData.Exposure[7] = GetAEFlkMode();
#if defined(__USE_PWMIRIS_ONLY__)
	stMenuData.Exposure[8] = GetAELensMode();
#endif
	
	// compare ae menu parameter & menu item value, update menu item value & api with ae menu parameter
#if defined(__USE_EXT2IPP__)
	stMenuData.Exposure[0] = 3;		//set manual mode
	stAEMENU.mode = 3;
	SetAECtrlMode(stAEMENU.mode);	
#else	//Default : normal case
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	SetAECtrlMode(stAEMENU.mode);
#endif
#endif


}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuWBInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[12];
	
	EEPROM_MultiRead(EEPROM_WB_MENU, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE) 	SetWBModeSetMenu(MIN(buff[1], WBMenuInfo[0].Max-1));
	if(buff[2] == EEPROM_SAVE_DONE) 	DEMO_SetWBManBlue(MIN(buff[3], WBMenuInfo[2].Max)); 	// max(100)
	if(buff[4] == EEPROM_SAVE_DONE) 	DEMO_SetWBManRed(MIN(buff[5], WBMenuInfo[3].Max)); 		// max(100)
	if(buff[6] == EEPROM_SAVE_DONE) 	DEMO_SetWBSpeed(MIN(buff[7], WBMenuInfo[4].Max)); 	// max(7)
	if(buff[8] == EEPROM_SAVE_DONE) 	DEMO_SetWBAutoBlue(MIN(buff[9], WBMenuInfo[5].Max)); 	// max(100)
	if(buff[10] == EEPROM_SAVE_DONE) 	DEMO_SetWBAutoRed(MIN(buff[11], WBMenuInfo[6].Max)); 		// max(100)
#endif
	
	// update menu item value with api
	stMenuData.WB[0] = GetWBModeSetMenu();
	stMenuData.WB[2] = GetWBManualBGainMenu();
	stMenuData.WB[3] = GetWBManualRGainMenu();
	stMenuData.WB[4] = GetWBSpeedMenu();
	stMenuData.WB[5] = GetWBAutoBGainMenu();
	stMenuData.WB[6] = GetWBAutoRGainMenu();

#if defined(__USE_EXT2IPP__)
	stMenuData.WB[0] = 4;
	SetWBModeSetMenu(WBCTRL_MANUAL_MODE) ;
	WBCTRL_SetAWBCtrlPath(0,0,0,0,0);
#endif
 }

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuDYNTInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[12];
	
	EEPROM_MultiRead(EEPROM_DYNT_MENU, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE) 	SetDYNT_Mode(MIN(buff[1], DYNTMenuInfo[0].Max-1));
	if(buff[2] == EEPROM_SAVE_DONE) 	SetDYNT_AutoDly(MIN(buff[3], DYNTMenuInfo[1].Max)); 	// max(60)
	if(buff[4] == EEPROM_SAVE_DONE) 	SetDYNT_AGCD2N(MIN(buff[5], DYNTMenuInfo[2].Max)); 		// max(255)
	if(buff[6] == EEPROM_SAVE_DONE) 	SetDYNT_AGCN2D(MIN(buff[7], DYNTMenuInfo[3].Max)); 		// max(254)
	if(buff[8] == EEPROM_SAVE_DONE) 	SetDYNT_NTColor(MIN(buff[9], DYNTMenuInfo[4].Max-1));
	if(buff[10] == EEPROM_SAVE_DONE) 	SetDYNT_CVBSBurst(MIN(buff[11], DYNTMenuInfo[5].Max-1));
#endif
	
	// update menu item value with api
	stMenuData.DYNT[0] = GetDYNT_Mode();
	stMenuData.DYNT[1] = GetDYNT_AutoDly();
	stMenuData.DYNT[2] = GetDYNT_AGCD2N();
	stMenuData.DYNT[3] = GetDYNT_AGCN2D();
	stMenuData.DYNT[4] = GetDYNT_NTColor();
	stMenuData.DYNT[5] = GetDYNT_CVBSBurst();
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuDNRInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[6];
	
	EEPROM_MultiRead(EEPROM_DNR_MENU, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE) 	SetNR_Mode((MISP_NR_MODE_t)MIN(buff[1], (DNRMenuInfo[0].Max-1)));
	if(buff[2] == EEPROM_SAVE_DONE) 	SetNR_3Dintensity((MISP_NR3D_MODE_t)MIN(buff[3], (DNRMenuInfo[1].Max-1)));
	if(buff[4] == EEPROM_SAVE_DONE) 	SetNR_Aperture(MIN(buff[5], (DNRMenuInfo[2].Max-1)));
#endif
	
	// update menu item value with api
	stMenuData.DNR[0] = GetNR_Mode();
	//stMenuData.DNR[1] = GetNR_3Dintensity();
	stMenuData.DNR[1] = GetNR_intensity();
	stMenuData.DNR[2] = GetNR_Aperture();
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuPMInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)

	BYTE buff[32], i;
	
	EEPROM_MultiRead(EEPROM_PM_MENU, (PBYTE)&buff[0], 20); 						// pm normal
	
	if(buff[0] == EEPROM_SAVE_DONE) 	SetPM_OnOff(MIN(buff[1], PMMenuInfo[0].Max-1));
	if(buff[2] == EEPROM_SAVE_DONE) 	SetPM_Type(MIN(buff[3], PMMenuInfo[1].Max-1));
	if(buff[4] == EEPROM_SAVE_DONE)		for(i=0;i<8;i++) SetPM_RectOnOff(i, (buff[5]>>i &1)?  ON:  OFF);
	if(buff[6] == EEPROM_SAVE_DONE)		for(i=0;i<8;i++) SetPM_PolyOnOff(i, (buff[7]>>i &1)?  ON:  OFF);
	if(buff[8] == EEPROM_SAVE_DONE) 	for(i=0;i<8;i++) SetPM_RectColor(i, (i%2)?  HI4BIT(buff[9+i/2]):  LO4BIT(buff[9+i/2]));
	if(buff[14] == EEPROM_SAVE_DONE) 	for(i=0;i<8;i++) SetPM_PolyColor(i, (i%2)?  HI4BIT(buff[15+i/2]):  LO4BIT(buff[15+i/2]));
	
	EEPROM_MultiRead(EEPROM_PM_RECTSIZE, (PBYTE)&buff[0], 32); 				// rect size
	for(i=0;i<8;i++)	{
		if(buff[4*i] == EEPROM_SAVE_DONE) 		SetPM_RectSizeDirect(i, buff[4*i+1] *32,  buff[4*i+2] *32);
	}
	
	EEPROM_MultiRead(EEPROM_PM_RECTPOSI, (PBYTE)&buff[0], 32); 				// rect posi
	for(i=0;i<8;i++)	{
		if(buff[4*i] == EEPROM_SAVE_DONE) 		SetPM_RectPosiDirect(i, buff[4*i+1] *32,  buff[4*i+2] *32);
	}
	
	for(i=0;i<8;i++)	{
		EEPROM_MultiRead(EEPROM_PM_POLYPOSI + (24*i), (PBYTE)&buff[0], 24); 	// poly posi
		if(buff[0] == EEPROM_SAVE_DONE) 	SetPM_PolyPosi(i, 0, MAKEWORD(buff[3],buff[2]), MAKEWORD(buff[5],buff[4])); 	// top-left
		if(buff[6] == EEPROM_SAVE_DONE) 	SetPM_PolyPosi(i, 1, MAKEWORD(buff[9],buff[8]), MAKEWORD(buff[11],buff[10])); 	// top-right
		if(buff[12] == EEPROM_SAVE_DONE) 	SetPM_PolyPosi(i, 2, MAKEWORD(buff[15],buff[14]), MAKEWORD(buff[17],buff[16])); 	// bot-left
		if(buff[18] == EEPROM_SAVE_DONE) 	SetPM_PolyPosi(i, 3, MAKEWORD(buff[21],buff[20]), MAKEWORD(buff[23],buff[22])); // bot-right
	}
#endif
	
	// update menu item value with api
	stMenuData.PM[0] = GetPM_OnOff();
	stMenuData.PM[1] = GetPM_Type();
	stMenuData.PM[2] = 0; 								// mask no. menu(default '0:mask-1')
	stMenuData.PM[3] = GetPM_MaskOnOffMenu();
	stMenuData.PM[4] = GetPM_ColorMenu();
	stMenuData.PM[5] = 0; 								// poly edge sel menu(default '0:L-Top')
	stMenuData.PM[6] = GetPM_PosiXMenu();
	stMenuData.PM[7] = GetPM_PosiYMenu();
	stMenuData.PM[8] = GetPM_WidthMenu();
	stMenuData.PM[9] = GetPM_HeightMenu();
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuEffectInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[20];
	
	EEPROM_MultiRead(EEPROM_EFFECT_MENU, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE) 	SetBY_Lens_corr_en(MIN(buff[1], EffectMenuInfo[0].Max-1));
	if(buff[2] == EEPROM_SAVE_DONE) 	DEMO_SetGammaValue(MIN(buff[3], EffectMenuInfo[1].Max-1));
	if(buff[4] == EEPROM_SAVE_DONE) 	DEMO_SetContrast(MIN(buff[5], EffectMenuInfo[2].Max)); 		// max(20)
	if(buff[6] == EEPROM_SAVE_DONE) 	DEMO_SetBrightness(MIN(buff[7], EffectMenuInfo[3].Max)); 	// max(20)
	if(buff[8] == EEPROM_SAVE_DONE) 	DEMO_SetSaturation(MIN(buff[9], EffectMenuInfo[4].Max)); 	// max(20)
	if(buff[10] == EEPROM_SAVE_DONE) 	DEMO_SetHue(MIN(buff[11], EffectMenuInfo[5].Max)); 			// max(20)
	if(buff[12] == EEPROM_SAVE_DONE) 	DEMO_SetSharpness(MIN(buff[13], EffectMenuInfo[6].Max)); 	// max(20)
#endif
	
	// update menu item value with api
	stMenuData.Effect[0] = GetBY_Lens_corr_en();
	stMenuData.Effect[1] = GetBY_GammaValue();
	stMenuData.Effect[2] = DEMO_GetContrast();
	stMenuData.Effect[3] = DEMO_GetBrightness();
	stMenuData.Effect[4] = DEMO_GetSaturation();
	stMenuData.Effect[5] = DEMO_GetHue();
	stMenuData.Effect[6] = DEMO_GetSharpness();
	
	
	// load eeprom & update menu item for auto function.
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	if(buff[14] == EEPROM_SAVE_DONE) 	DEMO_SetAutoSaturation(MIN(buff[15], EffectMenuInfo[7].Max-1));
	if(buff[16] == EEPROM_SAVE_DONE) 	DEMO_SetAutoEdgeOnOff(MIN(buff[17], EffectMenuInfo[8].Max-1));
//	if(buff[18] == EEPROM_SAVE_DONE) 	DEMO_SetAutoBright(MIN(buff[19], EffectMenuInfo[9].Max-1));
#endif
	
	stMenuData.Effect[7] = DEMO_GetAutoSatLevel();
	stMenuData.Effect[8] = DEMO_GetAutoEdgeOnOff();
//	stMenuData.Effect[9] = DEMO_GetAutoBright();
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuSpecialInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[22];
	
	EEPROM_MultiRead(EEPROM_SPECIAL_MENU, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE) 	DEMO_SetZoomMode(MIN(buff[1], SpecialMenuInfo[0].Max-1));
#if 1	
	if(buff[4] == EEPROM_SAVE_DONE) 	SetBY_DefogMode(MIN(buff[5], SpecialMenuInfo[1].Max-1));
#else	
	if(buff[2] == EEPROM_SAVE_DONE) 	SetBY_DefogDWDRMode(MIN(buff[3], SpecialMenuInfo[1].Max-1));
#endif	
	
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
# if 1	
	if(buff[6] == EEPROM_SAVE_DONE) 	DEMO_SetDWDRMode(MIN(buff[7], SpecialMenuInfo[2].Max-1));
#endif	
    if(buff[8] == EEPROM_SAVE_DONE) 	SetMD_OnOffMenu(MIN(buff[9], SpecialMenuInfo[3].Max-1));
	if(buff[20] == EEPROM_SAVE_DONE)	DEMO_FLKDET_MODE(MIN(buff[21], SpecialMenuInfo[10].Max-1));
#endif
	
	if(buff[10] == EEPROM_SAVE_DONE)		Demo_DIS_OnOff(MIN(buff[11], SpecialMenuInfo[4].Max-1));
	if(buff[12] == EEPROM_SAVE_DONE) 	DEMO_SetBackLightMode(MIN(buff[13], SpecialMenuInfo[5].Max-1));
	if(buff[14] == EEPROM_SAVE_DONE) 	DEMO_BINNING_MODE(MIN(buff[15], SpecialMenuInfo[6].Max-1));

#if defined(__USE_EXT2IPP__)
										DEMO_SetLiveDPCMode(0);
#else
	if(buff[16] == EEPROM_SAVE_DONE) 	DEMO_SetLiveDPCMode(MIN(buff[17], SpecialMenuInfo[7].Max-1));
	else								DEMO_SetLiveDPCMode(MIN(1, SpecialMenuInfo[7].Max-1));
#endif

	if(buff[18] == EEPROM_SAVE_DONE)	DEMO_SetWDR_MODE(MIN(buff[19], SpecialMenuInfo[9].Max-1));
#endif
	
	// update menu item value with api
	stMenuData.Special[0] = GetDZOOM_OnOff();
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)
	stMenuData.Special[1] = GetBY_DefogDWDRMode();
#else	
	stMenuData.Special[1] = GetBY_DefogMode();
	stMenuData.Special[2] = GetBY_dwdr_mode();
#endif	
	stMenuData.Special[3] = GetMD_OnOff();
	stMenuData.Special[4] = GetDIS_OnOff();
	stMenuData.Special[5] = DEMO_GetBackLightMode();
	stMenuData.Special[6] = GetSS_binning_enable();
	stMenuData.Special[7] = GetSDPC_OnOff();
	stMenuData.Special[9] = GetWDR_ProcesMode()?  ON:  OFF;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stMenuData.Special[10] = Get_FLKDet_Mode();
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuSetupInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[10];
	
	EEPROM_MultiRead(EEPROM_SETUP_MENU, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE) 	OSD_CamTitleDisplay(MIN(buff[1], SetupMenuInfo[0].Max-1));
	if(buff[2] == EEPROM_SAVE_DONE) 	DEMO_SetNTPal(MIN(buff[3], SetupMenuInfo[1].Max-1));
	if(buff[4] == EEPROM_SAVE_DONE) 	DEMO_SetLang(MIN(buff[5], SetupMenuInfo[3].Max-1));
	if(buff[6] == EEPROM_SAVE_DONE) 	DEMO_SetMenuPosition(MIN(buff[7], SetupMenuInfo[7].Max-1));
	if(buff[8] == EEPROM_SAVE_DONE) 	DEMO_SetOutputFormat(buff[9]);
#endif
	
	// update menu item value with api
	stMenuData.Setup[0] = OSD_GetCamTitleDisplay();
	stMenuData.Setup[1] = DEMO_GetNTPal();
	//stMenuData.Setup[3] = ();	// set in 'DEMO_SetLang'
	stMenuData.Setup[7] = DEMO_GetMenuPosition();
	stMenuData.Setup[8] = DEMO_GetOutputFormat();
	
	DEMO_SetMenuPosition(stMenuData.Setup[7]);
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuEngrInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
	
	EEPROM_MultiRead(EEPROM_SS_FPS, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE) 	DEMO_SetSensorFps(MIN(buff[1], EngrMenuInfo[7].Max-1));	// sensor frame rate
#endif
	
	// update menu item value with api
	stMenuData.Engr[7] = DEMO_GetSensorFps();
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuDNRTuneInit(void)
{
	WORD DNR_strategy ;
	DNR_strategy = GetNR_Strategy() ;
	
	stMenuData.DNRTune[2] = (DNR_strategy&0x0003) ;
	stMenuData.DNRTune[3] = (DNR_strategy&0x0000) ;
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuMISCTuneInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
	
	EEPROM_MultiRead(EEPROM_HDMITX_MODE, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE) 	DEMO_SetHdmiTxMode(MIN(buff[1], MiscTuneMenuInfo[5].Max-1));	// hdmi tx mode
#endif
	
	// update menu item value with api
	stMenuData.MiscTune[5] = DEMO_GetHdmiTxMode();
}

// sub menu
//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuConferInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[10];
	
	EEPROM_MultiRead(EEPROM_CONFER_MENU, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE) 	SetAFConferenceMode(MIN(buff[1], ConferMenuInfo[0].Max-1));
	if(buff[2] == EEPROM_SAVE_DONE) 	SetAFConferenceWinWidth(MIN(buff[3], ConferMenuInfo[1].Max)); 	// max(32)
	if(buff[4] == EEPROM_SAVE_DONE) 	SetAFConferenceWinHeight(MIN(buff[5], ConferMenuInfo[2].Max)); 	// max(32)
	if(buff[6] == EEPROM_SAVE_DONE) 	SetAFConferenceWinXPos(MIN(buff[7], ConferMenuInfo[3].Max)); 	// max(64)
	if(buff[8] == EEPROM_SAVE_DONE) 	SetAFConferenceWinYPos(MIN(buff[9], ConferMenuInfo[4].Max)); 	// max(64)
#endif
	
	// update menu item value with api
	stMenuData.Confer[0] = GetAFConferenceMode();
	stMenuData.Confer[1] = GetAFConferenceWinWidth();
	stMenuData.Confer[2] = GetAFConferenceWinHeight();
	stMenuData.Confer[3] = GetAFConferenceWinXPos();
	stMenuData.Confer[4] = GetAFConferenceWinYPos();
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuDZoomInit(void)
{
	WORD x, y;
	
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
//	BYTE buff[5];
	
//	EEPROM_MultiRead(EEPROM_DZOOM_MENU, (PBYTE)&buff[0], sizeof(buff));
	
	// dz & pip data in eeprom loaded in misc init func, but reload dz ratio & position for lens+dz mix
#endif
	
	// update menu item value with api
	stMenuData.DZCtrl[0] = GetDZOOM_Ratio();
	
	GetDZOOM_Position(&x, &y);
	stMenuData.DZCtrl[1] = (BYTE)x;
	stMenuData.DZCtrlY[1] = (BYTE)y;
	
	stMenuData.DZoom[1] = GetPIP_DispOnOff();
	stMenuData.DZoom[2] = GetPIP_Size();
	
	GetPIP_Position(&x, &y);
	stMenuData.PIPPos[0] = (BYTE)x;
	stMenuData.PIPPosY[0] = (BYTE)y;
}

//--------------------------------------------------------------------------------------------------------------------------
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)

static void SetMenuDefogDwdrInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[10];
	
	EEPROM_MultiRead(EEPROM_DEFOGDWDR_MENU, (PBYTE)&buff[0], sizeof(buff));
	
#if defined(__USE_MDIN_i500__)						// MDIN-i500
	if(buff[0] == EEPROM_SAVE_DONE) 	SetBY_DefogStrength(MIN(buff[1], DefogDwdrMenuInfo[0].Max-1));
	if(buff[2] == EEPROM_SAVE_DONE) 	SetBY_DefogBright(MIN(buff[3], DefogDwdrMenuInfo[1].Max)); 	// max(20)
#else 												// MDIN-i51X (i510, i540, i550)
	if(buff[0] == EEPROM_SAVE_DONE) 	SetBY_DefogStrength(MIN(buff[1], DefogDwdrMenuInfo[0].Max)); 	// max(16)
	if(buff[2] == EEPROM_SAVE_DONE) 	SetBY_DefogThreshold(MIN(buff[3], DefogDwdrMenuInfo[1].Max)); 	// max(3)
	if(buff[4] == EEPROM_SAVE_DONE) 	SetBY_DWDRSaturation(MIN(buff[5], DefogDwdrMenuInfo[2].Max)); 	// max(16)
	if(buff[6] == EEPROM_SAVE_DONE) 	SetBY_DWDRLocalRatio(MIN(buff[7], DefogDwdrMenuInfo[3].Max)); 	// max(16)
	if(buff[8] == EEPROM_SAVE_DONE) 	SetBY_DWDRSatSync(MIN(buff[9], DefogDwdrMenuInfo[4].Max)); 	// max(1)
#endif
#endif
	
	// update menu item value with api
	stMenuData.DefogDwdr[0] = GetBY_DefogStrength();
#if defined(__USE_MDIN_i500__)						// MDIN-i500
	stMenuData.DefogDwdr[1] = GetBY_DefogBright();
#else 												// MDIN-i51X (i510, i540, i550)
	stMenuData.DefogDwdr[1] = GetBY_DefogThreshold();
	stMenuData.DefogDwdr[2] = GetBY_DWDRSaturation();
	stMenuData.DefogDwdr[3] = GetBY_DWDRLocalRatio();
	stMenuData.DefogDwdr[4] = GetBY_DWDRSatSync();
#endif
}


#else


static void SetMenuDefogInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[4];
	
	EEPROM_MultiRead(EEPROM_DEFOG_MENU, (PBYTE)&buff[0], sizeof(buff));
	
#if defined(__USE_MDIN_i500__)						// MDIN-i500
	if(buff[0] == EEPROM_SAVE_DONE) 	SetBY_DefogStrength(MIN(buff[1], DefogMenuInfo[0].Max-1));
	if(buff[2] == EEPROM_SAVE_DONE) 	SetBY_DefogBright(MIN(buff[3], DefogMenuInfo[1].Max)); 	// max(20)
#else 												// MDIN-i51X (i510, i540, i550)
	if(buff[0] == EEPROM_SAVE_DONE) 	SetBY_DefogStrength(MIN(buff[1], DefogMenuInfo[0].Max)); 	// max(16)
	if(buff[2] == EEPROM_SAVE_DONE) 	SetBY_DefogThreshold(MIN(buff[3], DefogMenuInfo[1].Max)); 	// max(3)
#endif
#endif
	
	// update menu item value with api
	stMenuData.Defog[0] = GetBY_DefogStrength();
#if defined(__USE_MDIN_i500__)						// MDIN-i500
	stMenuData.Defog[1] = GetBY_DefogBright();
#else 												// MDIN-i51X (i510, i540, i550)
	stMenuData.Defog[1] = GetBY_DefogThreshold();
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuDWDRInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[12];
	
	EEPROM_MultiRead(EEPROM_DWDR_MENU, (PBYTE)&buff[0], sizeof(buff));
	
#if defined(__USE_MDIN_i500__)						// MDIN-i500
#else 												// MDIN-i51X (i510, i540, i550)
	if(buff[0] == EEPROM_SAVE_DONE) 	SetBY_DWDRStrength(MIN(buff[1], DWDRMenuInfo[0].Max));
	if(buff[2] == EEPROM_SAVE_DONE) 	SetBY_DWDRSaturation(MIN(buff[3], DWDRMenuInfo[1].Max));
	if(buff[4] == EEPROM_SAVE_DONE) 	SetBY_DWDRLocalRatio(MIN(buff[5], DWDRMenuInfo[2].Max));
	if(buff[6] == EEPROM_SAVE_DONE) 	SetBY_DWDRSatSync(MIN(buff[7], DWDRMenuInfo[3].Max));
	if(buff[8] == EEPROM_SAVE_DONE) 	SetBY_DWDRAutoLevel(MIN(buff[9], DWDRMenuInfo[4].Max));	
	
	if(buff[10] == EEPROM_SAVE_DONE) 	SetBY_DefogAutoLevel(MIN(buff[11], DefogMenuInfo[2].Max)); 	// max(1)		
#endif
	
#endif
	
#if defined(__USE_MDIN_i500__)						// MDIN-i500
#else 												// MDIN-i51X (i510, i540, i550)
	// update menu item value with api
	stMenuData.DWDR[0] = GetBY_DWDRStrength();
	stMenuData.DWDR[1] = GetBY_DWDRSaturation();
	stMenuData.DWDR[2] = GetBY_DWDRLocalRatio();
	stMenuData.DWDR[3] = GetBY_DWDRSatSync();
	stMenuData.DWDR[4] = GetBY_DWDRAutoLevel();
	
	stMenuData.Defog[2] = GetBY_DefogAutoLevel();	
	
#endif
}

#endif

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuMDInit(void)
{
#if defined(__USE_MDIN_i510__)||defined(__USE_MDIN_i540__)||defined(__USE_MDIN_i550__)
	
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[24], i;
	
	EEPROM_MultiRead(EEPROM_MD_MENU, (PBYTE)&buff[0], 8); 		// md normal
	
	if(buff[0] == EEPROM_SAVE_DONE)		for(i=0;i<4;i++) SetMD_AreaEn(i, (buff[1]>>i &1)?  ON:  OFF);
	if(buff[2] == EEPROM_SAVE_DONE)		for(i=0;i<4;i++) SetMD_AreaDisp(i, (buff[3]>>i &1)?  ON:  OFF);
	if(buff[4] == EEPROM_SAVE_DONE)		SetMD_Threshold(buff[5]);
	if(buff[6] == EEPROM_SAVE_DONE)		SetMD_MotionView((buff[7])?  ON:  OFF);
	
	EEPROM_MultiRead(EEPROM_MD_SIZEPOSI, (PBYTE)&buff[0], 24);	// md area size/position
	for(i=0;i<24;i+=6)	{
		if(buff[i] == EEPROM_SAVE_DONE) SetMD_AreaSizePosi((i/6), buff[i+1], buff[i+2], buff[i+3], buff[i+4]);
	}
#endif
	
	// update menu item value with api
	stMenuData.MD[0] = 0;					// area no. menu(default '0:area-1')
	stMenuData.MD[1] = GetMD_AreaEn(0);		// area-1 enable
	stMenuData.MD[2] = GetMD_AreaDisp(0);	// area-1 display
	stMenuData.MD[4] = GetMD_Threshold();
	stMenuData.MD[5] = GetMD_MotionView();
	
	stMenuData.MDSizPos[0] = GetMD_AreaPosiX(0);
	stMenuData.MDSizPos[1] = GetMD_AreaWidth(0);
	stMenuData.MDSizPosY[0] = GetMD_AreaPosiY(0);
	stMenuData.MDSizPosY[1] = GetMD_AreaHeight(0);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuBLCInit(void)
{
	WORD x, y;
	
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[14];//, mode;
	
//	mode = DEMO_GetBackLightMode();
	
	// blc
	EEPROM_MultiRead(EEPROM_BLC_MENU, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE) 	SetBLC_OnOff(MIN(buff[1], BLCMenuInfo[0].Max-1));
	if(buff[2] == EEPROM_SAVE_DONE) 	SetBLC_WinOnOff(MIN(buff[3], BLCMenuInfo[2].Max-1));
	
	if(buff[4] == EEPROM_SAVE_DONE) 	SetBLC_Position(MIN(buff[5], BLCSizeMenuInfo[0].Max), MIN(buff[6], BLCSizeYMax[0])); 	// max(48,33), i51x(96,67)
	if(buff[8] == EEPROM_SAVE_DONE) 	SetBLC_Size(MIN(buff[9], BLCSizeMenuInfo[1].Max), MIN(buff[10], BLCSizeYMax[1])); 	// max(48,33), i51x(96,67)
	
	// hlc
	EEPROM_MultiRead(EEPROM_HLC_MENU, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE) 	SetHLC_WinOnOff(MIN(buff[1], HSBLCMenuInfo[1].Max-1));
	if(buff[2] == EEPROM_SAVE_DONE) 	SetHLC_MaskLevel(MIN(buff[3], HSBLCMenuInfo[3].Max)); 	// max(20)
	if(buff[4] == EEPROM_SAVE_DONE) 	SetHLC_MaskOnOff(MIN(buff[5], HSBLCMenuInfo[4].Max-1));
	
	if(buff[6] == EEPROM_SAVE_DONE) 	SetHLC_Position(MIN(buff[7], HSBLCSizeMenuInfo[0].Max), MIN(buff[8], HSBLCSizeYMax[0])); 	// max(48,33), i51x(96,67)
	if(buff[10] == EEPROM_SAVE_DONE) 	SetHLC_Size(MIN(buff[11], HSBLCSizeMenuInfo[1].Max), MIN(buff[12], HSBLCSizeYMax[1])); 	// max(48,33), i51x(96,67)
	
//	DEMO_SetBackLightMode(mode);
#endif
	
	// update menu item value with api
	// blc
	stMenuData.BLC[0] = GetBLC_OnOff();
	stMenuData.BLC[2] = GetBLC_WinOnOff();
	
	GetBLC_Position(&x, &y);
	stMenuData.BLCSizPos[0] = (BYTE)x;
	stMenuData.BLCSizPosY[0] = (BYTE)y;
	
	GetBLC_Size(&x, &y);
	stMenuData.BLCSizPos[1] = (BYTE)x;
	stMenuData.BLCSizPosY[1] = (BYTE)y;
	
	// hlc
	stMenuData.HSBLC[1] = GetHLC_WinOnOff();
	stMenuData.HSBLC[3] = GetHLC_MaskLevel();
	stMenuData.HSBLC[4] = GetHLC_MaskOnOff();
	
	GetHLC_Position(&x, &y);
	stMenuData.HSBLCSizPos[0] = (BYTE)x;
	stMenuData.HSBLCSizPosY[0] = (BYTE)y;
	
	GetHLC_Size(&x, &y);
	stMenuData.HSBLCSizPos[1] = (BYTE)x;
	stMenuData.HSBLCSizPosY[1] = (BYTE)y;
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuDPCInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[4];
	
	EEPROM_MultiRead(EEPROM_DPC_MENU, (PBYTE)&buff[0], sizeof(buff));
	
//	if(buff[0] == EEPROM_SAVE_DONE) 	SetSDPD_AGC_Level(MIN(buff[1], DPCMenuInfo[1].Max-1));
//	if(buff[2] == EEPROM_SAVE_DONE) 	DEMO_SetAutoDynamicDPCEn(MIN(buff[3], DPCMenuInfo[4].Max-1));
#endif
	// update menu item value with api
	stMenuData.DPC[5] = GetDDPC_Threshold1(0);
	stMenuData.DPC[6] = GetDDPC_Threshold2(0);
	stMenuData.DPC[7] = GetDDPC_SelMode(0);	

}
//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuCCInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE i=0;
	SetApp_CAC_ReadEEPROM();

	for(i=0;i<6;i++){
		#if defined(CAC_ctrl_theta)
		SetApp_CACTheta(GetApp_CACTheta(i), i);
		SetApp_CACSaturation(GetApp_CACSaturation(i), i);		
		#else		
		SetApp_CACCos(GetApp_CACCos(i), i);
		SetApp_CACSin(SetApp_CACSin(i), i);
		SetApp_CACSaturation(GetApp_CACSaturation(i), i);				
		#endif
	}
#endif
	
	// update menu item value with api
#if defined(CAC_ctrl_theta)	
	stMenuData.ColorCorr[4] = GetApp_CACTheta(0);
	stMenuData.ColorCorr[5] = GetApp_CACSaturation(0);
#else
	stMenuData.ColorCorr[4] = GetApp_CACCos(0);
	stMenuData.ColorCorr[5] = GetApp_CACSin(0);
	stMenuData.ColorCorr[6] = GetApp_CACSaturation(0);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuDispFuncInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[6];
	
	EEPROM_MultiRead(EEPROM_DISP_MENU, (PBYTE)&buff[0], sizeof(buff));
	
//	if(buff[0] == EEPROM_SAVE_DONE) 	DEMO_SetFreezeMode(MIN(buff[1], DispFuncMenuInfo[0].Max-1));	// freeze not saved..
	if(buff[2] == EEPROM_SAVE_DONE) 	DEMO_SetMirrorMode(MIN(buff[3], DispFuncMenuInfo[1].Max-1));
	if(buff[4] == EEPROM_SAVE_DONE) 	DEMO_SetImageEffect(MIN(buff[5], DispFuncMenuInfo[2].Max-1));
#endif
	
	// update menu item value with api
//	stMenuData.DispFunc[0] = DEMO_GetFreezeMode();	// freeze not saved..
	stMenuData.DispFunc[1] = DEMO_GetMirrorMode();
	stMenuData.DispFunc[2] = DEMO_GetImageEffect();
	
#if defined(__USE_MDIN_i510__)||defined(__USE_MDIN_i540__)||defined(__USE_MDIN_i550__)
	stMenuData.DispFunc[4] = GetApp_ssPictureStyle();
	if (GetApp_ssPictureStyle() != MDIN_PS_OFF)
	{
		stMenuData.DispFunc[5] = GetApp_ssPictureContrast();
		stMenuData.DispFunc[6] = GetApp_ssPictureSaturation();
		stMenuData.DispFunc[7] = GetApp_ssPictureEE();
	}
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuCamtitleInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[12], i;
	
	EEPROM_MultiRead(EEPROM_CTITLE_MENU, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE)		for(i=0;i<10;i++) 		CamTitleText[0][i] = buff[i+1];
#if defined(__USE_CHN_FONT__)
	if(buff[0] == EEPROM_SAVE_DONE)		for(i=0;i<10;i++) 		CamTitleText[1][i] = buff[i+1];
#endif
	
	EEPROM_MultiRead(EEPROM_CTITLE_COLOR, (PBYTE)&buff[0], 2);
	
	if(buff[0] == EEPROM_SAVE_DONE) 	stMenuData.CamTitle[1] = MIN(buff[1], CamTitleMenuInfo[1].Max-1);
	
	if(OSD_GetCamTitleDisplay()) 		OSD_CamTitleDisplay(ON);
#endif
	
	// update menu item value with api
//	stMenuData.CamTitle[0] = ();
//	stMenuData.CamTitle[1] = ();
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuMColorInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[8];
	
	EEPROM_MultiRead(EEPROM_MCOLOR_MENU, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE) 	stMenuData.MenuColor[0] = MIN(buff[1], MenuColorMenuInfo[0].Max-1);
	if(buff[2] == EEPROM_SAVE_DONE) 	stMenuData.MenuColor[1] = MIN(buff[3], MenuColorMenuInfo[1].Max-1);
	if(buff[4] == EEPROM_SAVE_DONE) 	stMenuData.MenuColor[2] = MIN(buff[5], MenuColorMenuInfo[2].Max-1);
	if(buff[6] == EEPROM_SAVE_DONE) 	stMenuData.MenuColor[3] = MIN(buff[7], MenuColorMenuInfo[3].Max-1);
#endif
	
	// update menu item value with api
//	stMenuData.MenuColor[0] = ();
//	stMenuData.MenuColor[1] = ();
//	stMenuData.MenuColor[2] = ();
//	stMenuData.MenuColor[3] = ();
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuSysInfoInit(void)
{
	stMenuData.SysInfo[0] = LOBYTE(LOWORD(GetSYSINFO_BoardType())); 		// system info. h/w version
	stMenuData.SysInfo[3] = HIBYTE(LOWORD(GetSYSINFO_SensorNo())); 			// system info. sensor type
	stMenuData.SysInfo[4] = HIBYTE(HIWORD(GetSYSINFO_LensType())); 			// system info. lens type
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetMenuFactorySetupInit(void)
{
	// load eeprom & update api value
#if defined (__USE_EEPROM__)
	BYTE buff[6];
	
	EEPROM_MultiRead(EEPROM_FACTORY_SETUP, (PBYTE)&buff[0], sizeof(buff));
	
	//if(buff[0] == EEPROM_SAVE_DONE)		DCLENS_IRIS_SetOffset(buff[1]);
	//if(buff[2] == EEPROM_SAVE_DONE)		DCLENS_IRIS_SetADC(buff[3]);
	if(buff[4] == EEPROM_SAVE_DONE) 	SetAEIrisSpeed(MIN(buff[5], FSMenuInfo[1].Max)); 	// max(5)
#endif
	
	// update menu item value with api
	stMenuData.FS[1] = GetAEIrisSpeed();
	stMenuData.CCF[1] = GetCCFGain(Red_Line_Type1);
	stMenuData.CCF[2] = GetCCFBalance(Red_Line_Type1);
	stMenuData.CCF[3] = GetCCFType();
}

//--------------------------------------------------------------------------------------------------------------------------
void SetMenuInitValue(void)
{
	// main menu
	SetMenuFocusInit();			// set focus menu init value from api & eeprom.
	SetMenuExposureInit();		// exposure
	SetMenuWBInit(); 			// white balance
	SetMenuDYNTInit();			// day & night
	SetMenuDNRInit();			// dnr
	SetMenuPMInit();			// privacy mask
	SetMenuEffectInit();		// effect
	SetMenuSpecialInit();		// special
	SetMenuSetupInit();			// setup
	SetMenuEngrInit();			// engineer
	
	// sub menu
	SetMenuConferInit(); 		// focus - conference
	SetMenuDZoomInit(); 		// special - dzoom
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)
	SetMenuDefogDwdrInit();		// special - defog/dwdr
#else	
	SetMenuDefogInit();		// special - defog/dwdr
	SetMenuDWDRInit();		// special - defog/dwdr		
#endif		
	SetMenuMDInit();			// special - motion detect
	SetMenuBLCInit(); 			// special - backlight
	SetMenuDPCInit(); 			// special - dpc	
	SetMenuCCInit();			// eng - CC&CAC
	SetMenuDispFuncInit(); 		// special - display
	SetMenuCamtitleInit(); 		// setup - camtitle
	SetMenuMColorInit(); 		// setup - menu color
	SetMenuSysInfoInit(); 		// setup - system info

	SetMenuDNRTuneInit() ;		// Engr - DNR tune
	SetMenuMISCTuneInit();		// Engr - MISC tune

	SetMenuFactorySetupInit();	// Debug - Factory Setup
}
	
//--------------------------------------------------------------------------------------------------------------------------
void SetMenuFactoryDefault(void)
{
	WORD i;
	
	memset((PBYTE)&stMenuData, 0, sizeof(stMenuData));					// menu data clear

	// move init value in osdmenu.h to menu data structure.
	for(i=0;i<sizeof(stMenuData.Main);i++)			stMenuData.Main[i] = MainMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.Focus);i++)		stMenuData.Focus[i] = FocusMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.Exposure);i++)		stMenuData.Exposure[i] = ExpMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.WB);i++)			stMenuData.WB[i] = WBMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.DYNT);i++)			stMenuData.DYNT[i] = DYNTMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.DNR);i++)			stMenuData.DNR[i] = DNRMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.PM);i++)			stMenuData.PM[i] = PMMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.L3WDRSet);i++)		stMenuData.L3WDRSet[i] = L3WDRSetMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.L2WDRSet);i++)		stMenuData.L2WDRSet[i] = L2WDRSetMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.TMWDRSet);i++)		stMenuData.TMWDRSet[i] = TMWDRSetMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.FRWDRSet);i++)		stMenuData.FRWDRSet[i] = FRWDRSetMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.DGWDRSet);i++)		stMenuData.DGWDRSet[i] = DGWDRSetMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.DZCtrl);i++)		stMenuData.DZCtrl[i] = DZCtrlMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.Special);i++)		stMenuData.Special[i] = SpecialMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.Confer);i++)		stMenuData.Confer[i] = ConferMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.GMC);i++)			stMenuData.GMC[i] = GMCMenuInfo[i].Init;	
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)
	for(i=0;i<sizeof(stMenuData.DefogDwdr);i++)		stMenuData.DefogDwdr[i] = DefogDwdrMenuInfo[i].Init;
#else	
	for(i=0;i<sizeof(stMenuData.Defog);i++)		stMenuData.Defog[i] = DefogMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.DWDR);i++)			stMenuData.DWDR[i] = DWDRMenuInfo[i].Init;	
#endif	
	for(i=0;i<sizeof(stMenuData.BLC);i++)			stMenuData.BLC[i] = BLCMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.BLCSizPos);i++)	stMenuData.BLCSizPos[i] = BLCSizeMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.HSBLC);i++)		stMenuData.HSBLC[i] = HSBLCMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.HSBLCSizPos);i++)	stMenuData.HSBLCSizPos[i] = HSBLCSizeMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.MD);i++)			stMenuData.MD[i] = MDMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.MDSizPos);i++)		stMenuData.MDSizPos[i] = MDSizeMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.DZoom);i++)		stMenuData.DZoom[i] = DZMenuInfo[i].Init; //
	for(i=0;i<sizeof(stMenuData.Effect);i++)		stMenuData.Effect[i] = EffectMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.DPC);i++)			stMenuData.DPC[i] = DPCMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.Setup);i++)		stMenuData.Setup[i] = SetupMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.CamTitle);i++)		stMenuData.CamTitle[i] = CamTitleMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.RS485);i++)		stMenuData.RS485[i] = RS485MenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.MenuColor);i++)	stMenuData.MenuColor[i] = MenuColorMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.FReset);i++)		stMenuData.FReset[i] = FResetMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.SysInfo);i++)		stMenuData.SysInfo[i] = SysInfoMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.DispFunc);i++)		stMenuData.DispFunc[i] = DispFuncMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.Engr);i++)			stMenuData.Engr[i] = EngrMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.ColorCorr);i++)		stMenuData.ColorCorr[i] = ColorCorrMenuInfo[i].Init;	
	for(i=0;i<sizeof(stMenuData.TP);i++)			stMenuData.TP[i] = TPMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.TPSizPos);i++)		stMenuData.TPSizPos[i] = TPSizeMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.PIPPos);i++)		stMenuData.PIPPos[i] = PIPPosiMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.DNRTune);i++)		stMenuData.DNRTune[i] = DNRTuneMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.MiscTune);i++)		stMenuData.MiscTune[i] = MiscTuneMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.Debug);i++)			stMenuData.Debug[i] = DebugMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.VS);i++)			stMenuData.VS[i] = VSMenuInfo[i].Init;
	for(i=0;i<sizeof(stMenuData.FS);i++)			stMenuData.FS[i] = FSMenuInfo[i].Init;
	
	for(i=0;i<sizeof(stMenuData.MDSizPosY);i++)	stMenuData.MDSizPosY[i] = MDSizeYInit[i];
	for(i=0;i<sizeof(stMenuData.BLCSizPosY);i++)	stMenuData.BLCSizPosY[i] = BLCSizeYInit[i];
	for(i=0;i<sizeof(stMenuData.HSBLCSizPosY);i++)	stMenuData.HSBLCSizPosY[i] = HSBLCSizeYInit[i];
	for(i=0;i<sizeof(stMenuData.DZCtrlY);i++)		stMenuData.DZCtrlY[i] = DZCtrlYInit[i];
	for(i=0;i<sizeof(stMenuData.TPSizPosY);i++)	stMenuData.TPSizPosY[i] = TPSizeYInit[i];
	for(i=0;i<sizeof(stMenuData.PIPPosY);i++)		stMenuData.PIPPosY[i] = PIPPosiYInit[i];
//	for(i=0;i<sizeof(stMenuData.Compress);i++)		stMenuData.Compress[i] = CompressMenuInfo[i].Init;

	OSDMenuID = OSDCombID = 0;

#if __MISP100_DBGPRT__ == 1
	UARTprintf("Set Menu status to factory setting ! \n");
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void EraseE2promMenuStatus(void)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	WORD i;
	WORD buff=0xffff;
	
	for(i=0;i<256;i+=2) 	EEPROM_RegWrite(EEPROM_MAIN_MENU+i, buff);		// erase main menu data in eeprom(0x500 ~ 0x5ff, 256byte)
	for(i=0;i<256;i+=2) 	EEPROM_RegWrite(EEPROM_SUB_MENU+i, buff); 		// erase sub menu data in eeprom(0x600 ~ 0x6ff, 256byte)
	for(i=0;i<256;i+=2) 	EEPROM_RegWrite(EEPROM_PM_SIZEPOSI+i, buff); 	// erase pm size/posi param in eeprom(0x700 ~ 0x7ff, 256byte)
	for(i=0;i<256;i+=2) 	EEPROM_RegWrite(EEPROM_DDPC_PARAM+i, buff); 	// erase menu misc data in eeprom(0x800 ~ 0x8ff, 256byte)
	for(i=0;i<256;i+=2) 	EEPROM_RegWrite(EEPROM_USER_AREA+i, buff);		// erase user app parameter(0x1000 ~ 0x10ff, 256byte)
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
#if defined(__USE_CHN_FONT__)
BYTE FindText(PWORD Target, char Text, BYTE Size, BYTE cID)
#else
BYTE FindText(PCHAR Target, char Text, BYTE Size, BYTE cID)
#endif
{
	BYTE i;

	for (i=0; i<Size; i++)	{	
		if(Target[cID*Size +i] == Text)	return 1;
	}

	return 0;		//found(1), no found(0)
}

//--------------------------------------------------------------------------------------------------------------------------
static void OSD_TextSelect(BOOL dir)
{
	BYTE dig = OSDTextDigit;
	BYTE text = CamTitleText[0][dig];
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)		// save text to eeprom
	BYTE buff[12], i;
#endif
	
	if (dir)	{
		switch(text)	{	// ! ~ /, 0 ~ 9, ' ', A ~ Z, a ~ z
			case TEXT_CURSOR:		text = 'A';		break;	//cursor -> 'A'
			case '9':				text = 'A';		break;	//'9' -> 'A'
			case 'Z':				text = 'a';		break;	//'Z' -> 'a' 
			case 'z':				text = '!';		break;	//'z' -> '!'	
			default :				text += 1;		break;
		}
	}
	else	{
		switch(text)	{
			case TEXT_CURSOR:		text = '9';		break;	//cursor -> '9'
			case 'A':				text = '9';		break;	//'A' -> '9'
			case 'a':				text = 'Z';		break;	//'a' -> 'Z'	
			case '!':				text = 'z';		break;	//'!' -> 'z'
			default :				text -= 1;		break;
		}
	}

	CamTitleText[0][dig] = text;
#if defined(__USE_CHN_FONT__)
	CamTitleText[1][dig] = text;
#endif

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)		// save text to eeprom
	buff[0] = EEPROM_SAVE_DONE;
	for(i=0;i<10;i++)				buff[1+i] = CamTitleText[0][i];
	EEPROM_MultiWrite(EEPROM_CTITLE_TEXT, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void OSD_TextDigSelect(void)
{
	if(CamTitleText[0][OSDTextDigit] == TEXT_CURSOR)	{
		CamTitleText[0][OSDTextDigit] = CamTitleBAK[OSDTextDigit];	// restore text of cursor digit
#if defined(__USE_CHN_FONT__)
		CamTitleText[1][OSDTextDigit] = CamTitleBAK[OSDTextDigit];
#endif
	}
	
	if (OSDTextDigit < 9)	OSDTextDigit ++;
	else					OSDTextDigit = 0;
	
	if(CamTitleText[0][OSDTextDigit] != TEXT_CURSOR)	{
		CamTitleBAK[OSDTextDigit] = CamTitleText[0][OSDTextDigit];			// backup text of cursor digit
		CamTitleText[0][OSDTextDigit] = TEXT_CURSOR;						// change text to cursor
#if defined(__USE_CHN_FONT__)
		CamTitleText[1][OSDTextDigit] = TEXT_CURSOR;						// change text to cursor
#endif
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void MenuMessageControl()
{
	PBYTE pID = (PBYTE)&OSDMenuID;
	WORD	ZoomOnOff = GetMenuStatus(DZMODE_MENU);

//	if (((OSDMenuID & 0x0f) == BLCSIZE_TREE) || ((OSDMenuID & 0x0f) == HSBLCSIZE_TREE)) 
//		DEMO_SetBLCAreaDisplay(OFF);

	if (pID(0)==0) SetOSDMenuID(OSDMenuID|0x1000);	//Lv0 -> Lv1
	else if (pID(1)&0x0f) SetOSDMenuID(OSDMenuID&0xfff0);	//Lv4 -> Lv3
	else if (pID(1)&0xf0) SetOSDMenuID(OSDMenuID&0xff00);	//Lv3 -> Lv2
	else if (pID(0)&0x0f) SetOSDMenuID(OSDMenuID&0xf000);	//Lv2 -> Lv1
	else				  SetOSDMenuID(OSDMenuID&0x0000);	//Lv1 -> Lv0
	
	if(OSDMenuID == 0xC000)		SetOSDMenuID(0x0000);	// exit debug menu

	if (pID(0))			{ fZOOMMove = FALSE;	fMainCrop = FALSE; }	// disable zoom move.
	else if(ZoomOnOff)	{ fZOOMMove = TRUE;	}							// enable zoom move.

#if defined(__USE_FHDCROP_MODE__)
	else if(fCropMode){ fZOOMMove = FALSE;	fMainCrop = TRUE ; } // if crop mode and menu closed and dzoom is off
	else { fZOOMMove = FALSE;	fMainCrop = FALSE ;}
#endif

}

//--------------------------------------------------------------------------------------------------------------------------
BYTE MenuMessageExit()
{
	PBYTE pID = (PBYTE)&OSDMenuID;

	if (pID(0)==0) {
		return 0;
	} else {
		SetOSDMenuID(OSDMenuID&0x0000);
		return 1;
	}
}

static void DirMsgPosControl(BOOL dir);

//--------------------------------------------------------------------------------------------------------------------------
static void DirMainControl(BOOL dir)
{
	PBYTE pID = (PBYTE)&OSDMenuID;
	BYTE level = 0;
	WORD	CamTitleOnOff = GetMenuStatus(CAMTITLE_MENU);

	OSD_GetMenuItemInfo(&stMenuInfo, OSDMenuID);
	level = OSD_GetMenuLevel(OSDMenuID);

	if (stMenuInfo.Attb & MENU_CTITLE) 	if(CamTitleOnOff) OSD_CamTitleDisplay(ON);	
	if (level == 4)		DirMsgPosControl(dir);			 //skip u/d in Lv4

	do{
		switch(level) {				 //OSDMenuID +/-
			case 1:	OSD_MenuSelect(stMenuInfo.MenuRow, 0xf0, &pID(0), dir);	break;
			case 2:	OSD_MenuSelect(stMenuInfo.MenuRow, 0x0f, &pID(0), dir);	break;
			case 3:	OSD_MenuSelect(stMenuInfo.MenuRow, 0xf0, &pID(1), dir);	break;
//			case 4: OSD_MenuSelect(stMenuInfo.MenuRow, 0x0f, &pID(1), dir);	break; //skip u/d Lv4
		}
		OSD_GetMenuItemInfo(&stMenuInfo, OSDMenuID);

	} while(stMenuInfo.Attb & (MENU_SKIP | MENU_TEXT));	// skip menu(attb == skip or text)

	if (stMenuInfo.Attb & MENU_CTITLE)	{					// cam title menu(change text to cursor)
		switch(CamTitleText[0][OSDTextDigit])	{
			case TEXT_CURSOR:												break;	// cursor : no action
			case ' ':			CamTitleText[0][OSDTextDigit] = TEXT_CURSOR;
#if defined(__USE_CHN_FONT__)
								CamTitleText[1][OSDTextDigit] = TEXT_CURSOR;
#endif
																			break;	// blank : blank -> cursor
			default:			OSD_TextDigSelect();						break;	// text : digit++ & text -> cursor
		}
	}

	OSDCombID = stMenuInfo.cID;
}

//--------------------------------------------------------------------------------------------------------------------------
void DirUMessageControl(void)
{
	PBYTE pID = (PBYTE)&OSDMenuID;

	if (pID(0)==0)	return;
	else			DirMainControl(DEC);

 	SetOSDMenuID(OSDMenuID);
}

//--------------------------------------------------------------------------------------------------------------------------
void DirDMessageControl(void)
{
	PBYTE pID = (PBYTE)&OSDMenuID;

	if (pID(0)==0)	return;
	else			DirMainControl(INC);

 	SetOSDMenuID(OSDMenuID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void DirMsgCombControl(BOOL dir)		//OSDCombID +/-, Store to demoXXX[sID]
{
	OSD_CombSelect(stMenuInfo.Max, &stMenuInfo.cID, dir);		// cID +/-
	SetMenuStatus(OSDMenuID, stMenuInfo.cID);					// save demoXXX
}

//--------------------------------------------------------------------------------------------------------------------------
static void DirMsgProgControl(BOOL dir)	   //OSDCombID +/- , Store demoXXX[sID] call by DirL/R
{
	OSD_ProgSelect(stMenuInfo.Max, &stMenuInfo.cID, dir);		// cID +/-
	SetMenuStatus(OSDMenuID, stMenuInfo.cID);					// save demoXXX	
	
	if((OSDMenuID | 0x0f) == DZRATIO_TREE)		SetDZOOM_RepeatKeyEnable();	// repeat speed for dzoom
	else										SetMsgCommandRepeatKey(MSG_LEVEL);	// repeat speed for normal
}

//--------------------------------------------------------------------------------------------------------------------------
static void DirMsgPosControl(BOOL dir)
{
	OSD_ProgSelect2(stMenuInfo.pos_y_max, &stMenuInfo.pos_y, dir);	// position(y) +/-
	SetMenuPosiY(OSDMenuID, stMenuInfo.pos_y);						// save demoXXXSizPosY
	
	if((OSDMenuID | 0x0f) == DZRATIO_TREE)		SetDZOOM_RepeatKeyEnable();	// repeat speed for dzoom
	else										SetMsgCommandRepeatKey(MSG_LEVEL);	// repeat speed for normal

	SetOSDItemID(OSDCombID); 		//item func call
}

//--------------------------------------------------------------------------------------------------------------------------
void DirRMessageControl(void)			//OSDCombID + , Store to demoXXX[sID]
{										 
	PBYTE pID = (PBYTE)&OSDMenuID;

	if (pID(0)==0)		  return;		//no menu, return

	OSD_GetMenuItemInfo(&stMenuInfo, OSDMenuID);

	if (stMenuInfo.Attb & MENU_VALUE)	DirMsgProgControl(INC);	//OSDCombID -, Store to demoxx
	else if(stMenuInfo.Attb & MENU_CTITLE) OSD_TextSelect(INC);
	else								DirMsgCombControl(INC);	//OSDCombID -, Store to demoxx
	
	fMenuUpdate = ON;
	
	OSDCombID = stMenuInfo.cID;
	SetOSDItemID(OSDCombID);	 //set OSDItemID & run Item function !!! 
}

//--------------------------------------------------------------------------------------------------------------------------
void DirLMessageControl(void)				//OSDCombID - , Store to demoXXX[sID]
{											
	PBYTE pID = (PBYTE)&OSDMenuID;

	if (pID(0)==0)		  return;		//no menu, return

	OSD_GetMenuItemInfo(&stMenuInfo, OSDMenuID);

	if (stMenuInfo.Attb & MENU_VALUE)	DirMsgProgControl(DEC);	//OSDCombID -, Store to demoxx
	else if(stMenuInfo.Attb & MENU_CTITLE) OSD_TextSelect(DEC);
	else								DirMsgCombControl(DEC);	//OSDCombID -, Store to demoxx

	fMenuUpdate = ON;
	
	OSDCombID = stMenuInfo.cID;
	SetOSDItemID(OSDCombID);		   //set OSDItemID & run Item func.
}

//--------------------------------------------------------------------------------------------------------------------------
void EnterMessageControl(void)
{
	PBYTE pID = (PBYTE)&OSDMenuID;	BYTE level;
	WORD	ZoomOnOff = GetMenuStatus(DZMODE_MENU);

	OSD_GetMenuItemInfo(&stMenuInfo, OSDMenuID);
	level = OSD_GetMenuLevel(OSDMenuID);

	switch(stMenuInfo.Attb)	{
		case MENU_ITEM:	  return;
		case MENU_CTITLE:	  OSD_TextDigSelect();	break;			//Text digit shift	
		case MENU_FUNC:	  DirRMessageControl();	break;			//run Item func.
		case MENU_EXIT:	  SetOSDMenuID(OSDMenuID & (0xFFFF<<(5-level)*4));	break; //Lv dec.	
		case MENU_VALUE:									//Size/Position menu move.
						if (level == 4)
							OSD_MenuSelect(stMenuInfo.MenuRow, 0x0f, &pID(1), INC);		
						break;
		case MENU_SUB: 										//Menu level up
						if (FindText(stMenuInfo.pItem, '>', 10, stMenuInfo.cID) == 1) {
							if (pID(1)&0x0f) SetOSDMenuID(OSDMenuID);				//Lv4 -> Lv4
							else if (pID(1)&0xf0) SetOSDMenuID(OSDMenuID|0x0001);	//Lv3 -> Lv4
							else if (pID(0)&0x0f) SetOSDMenuID(OSDMenuID|0x0010);	//Lv2 -> Lv3
							else if (pID(0)&0xf0) SetOSDMenuID(OSDMenuID|0x0100);	//Lv1 -> Lv2
						}
						break;
	}

	SetOSDMenuID(OSDMenuID);

	OSD_GetMenuItemInfo(&stMenuInfo, OSDMenuID);
	OSDCombID = stMenuInfo.cID;

	if (stMenuInfo.Attb & (MENU_SKIP | MENU_TEXT))	DirDMessageControl();	//if menu_skip? move down.
	
	if (stMenuInfo.Attb & MENU_CTITLE)	{									// cam title menu(change text to cursor)
		switch(CamTitleText[0][OSDTextDigit])	{
			case TEXT_CURSOR:												break;	// cursor : no action
			case ' ':			CamTitleText[0][OSDTextDigit] = TEXT_CURSOR;
#if defined(__USE_CHN_FONT__)
								CamTitleText[1][OSDTextDigit] = TEXT_CURSOR;
#endif
																			break;	// blank : blank -> cursor
			default:			OSD_TextDigSelect();						break;	// text : digit++ & text -> cursor
		}
	}

	if (pID(0))			{ fZOOMMove = FALSE;	fMainCrop = FALSE; }	// disable zoom move.
	else if(ZoomOnOff)	{ fZOOMMove = TRUE;	}							// enable zoom move.
}

//--------------------------------------------------------------------------------------------------
void SetMenuRedraw(void)
{
//	OSD_SetFontMAP();  //repair crashed font map
	fMenuUpdate = ON;
	fMenuDrawAll = ON;
}

//--------------------------------------------------------------------------------------------------
static BYTE txt2byte(WORD txt)
{
	if(txt >= '0' && txt <= '9')		return (txt - '0' + 0x0);	// 0 ~ 9
	else if(txt >= 'A' && txt <= 'F')	return (txt - 'A' + 0xa);	// A ~ F
	else if(txt >= 'a' && txt <= 'f')	return (txt - 'a' + 0xa);	// a ~ f
	else								return 0;
}

//--------------------------------------------------------------------------------------------------
DWORD GetMenu_FirmwareVer(void)
{
	DWORD ver;
	
	ver = MAKEDWORD(MAKEWORD(MAKEBYTE(txt2byte(FirmwareVer[0][0]), txt2byte(FirmwareVer[0][2])), \
			MAKEBYTE(txt2byte(FirmwareVer[0][4]), txt2byte(FirmwareVer[0][5]))),	\
			MAKEWORD(MAKEBYTE(txt2byte(FirmwareVer[0][6]), txt2byte(FirmwareVer[0][7])), \
			MAKEBYTE(txt2byte(FirmwareVer[0][8]), txt2byte(FirmwareVer[0][9]))));

	return ver;
}

//----------------------------------------------------------------------------------
BYTE OSD_GetMenuLang(void) 	// 0:eng, 1:chn
{
	return  (stMenuData.Setup[3]==0)?  0:  1;
}

//--------------------------------------------------------------------------------------------------
void OSD_WaitMessageBox(BOOL OnOff)
{
	if(OSD_GetMenuLang()==0)	{
		if(OnOff)		OSD_MessageBox(ON, " Please wait.. ");
		else			OSD_MessageBox(OFF, NULL);
	}
	else	{
		if(OnOff)		OSD_MessageBox(ON, (PBYTE)msg_chn_please_wait);
		else			OSD_MessageBox(OFF, NULL);
	}
}

/*  FILE_END_HERE */
