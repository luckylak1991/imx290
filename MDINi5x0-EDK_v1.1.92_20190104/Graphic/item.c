//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	ITEM.C
// Description 		:
// Ref. Docment		: MDIN380 DDK v2.52
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"osd.h"
#include	"video.h"

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static WORD OSDItemID;
static BOOL fItemExecute;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
extern BYTE fMenuWaitFunc;
// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void SetOSDItemID(WORD nID)
{
	OSDItemID = nID;
	fItemExecute = ON;
	SetOSDCombID(nID);	// for VOUT key
}

//Lv4
//--------------------------------------------------------------------------------------
static void ItemExeBLCSizPos(BYTE zID)
{
	switch (zID)	{
		case  1:	SetBLC_PosiMenu(OSDItemID);		break;
		case  2:	SetBLC_SizeMenu(OSDItemID);			break;
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeHSBLCSizPos(BYTE zID)
{
	switch (zID)	{
		case  1:	SetHLC_PosiMenu(OSDItemID);	break;
		case  2:	SetHLC_SizeMenu(OSDItemID); 		break;
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeMDetSizPos(BYTE zID)
{
	switch (zID)	{
		case  1:	SetMD_PosiMenu(OSDItemID);	break;	//Position
		case  2:	SetMD_SizeMenu(OSDItemID);		break;	//Size
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeZoomPosition(BYTE zID)
{
	switch (zID)	{
		case  1:	DEMO_SetZoomRatio(OSDItemID);		break;	//Size
		case  2:	DEMO_SetZoomPosition(OSDItemID);	break;	//position
	}
}

//--------------------------------------------------------------------------------------
static void ItemExePIPPosition(BYTE zID)
{
	DEMO_SetPIPPosition(OSDItemID);		//value(0~??)
}
//--------------------------------------------------------------------------------------
static void ItemExeTPSizPos(BYTE zID)
{
	switch (zID)	{
		case  1:	DEMO_SetTPPosition(OSDItemID);	break;	//Position	
		case  2:	DEMO_SetTPSize(OSDItemID);		break;	//Size 	
	}
}

//Lv3
//--------------------------------------------------------------------------------------
static void ItemExeBLC(BYTE tID)
{
	WORD mID = GetOSDMenuID();
	BYTE zID = LO4BIT(LOBYTE(mID));	//Level 3,4 ID

	switch (tID)	{
		case  1: SetBLC_OnOff(OSDItemID);	break;
		case  2: ItemExeBLCSizPos(zID);			break;
		case  3: SetBLC_WinOnOff(OSDItemID);	break;
		case  4: SetMenuDefaultItem(mID);					// Menu default setting
				 SetBLC_FactoryDefaultMenu(ON);		 SetMenuRedraw();	
				 break;				
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeHSBLC(BYTE tID)
{
	WORD mID = GetOSDMenuID();
	BYTE zID = LO4BIT(LOBYTE(mID));	//Level 3,4 ID

	switch (tID)	{
		case  1: break;	//Area(Area1,2,3,4)
		case  2: SetHLC_WinOnOff(OSDItemID);	break;	//Area display(Off, On)
		case  3: ItemExeHSBLCSizPos(zID);			break;	//Size/Position
		case  4: SetHLC_MaskLevel(OSDItemID);	break;	//Level(0~20)
		case  5: SetHLC_MaskOnOff(OSDItemID);		break;	//Black Mask(Off, On)
		case  6: SetMenuDefaultItem(mID);					// Menu default setting
				 SetHLC_FactoryDefaultMenu(ON);	SetMenuRedraw();	
				 break;				
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeBLight(BYTE tID)
{
	switch (tID)	{
		case  0:  DEMO_SetBackLightMode(OSDItemID);	SetMenuRedraw();	break;
		default:	
				switch(GetMenuStatus(BLIGHT_MENU))	{
					case  1:  ItemExeBLC(tID);		break;
					case  2:  ItemExeHSBLC(tID);	break;
				}
				break;
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeMDet(BYTE tID)
{
	WORD mID = GetOSDMenuID();
	BYTE zID = LO4BIT(LOBYTE(mID));	//Level 3,4 ID

	if(GetMenuStatus(MD_MENU)!=OFF)	{			//blocking overlap block -md,pm,backlight
		SetBLC_OnOffMenu(OFF); 				SetHLC_OnOffMenu(OFF); 			SetPM_OnOffMenu(OFF);
		SetMenuStatus(BLIGHT_MENU, OFF);	SetMenuStatus(PM_MODE_MENU, OFF);
	}

	switch (tID)	{
		case  0: SetMD_OnOffMenu(OSDItemID); break;	// Motion Detect Mode(Off, On>)
		case  1: SetMD_AreaSelMenu(OSDItemID);		SetMenuRedraw();	break;	// Area(Area1,2,3,4)
		case  2: SetMD_AreaOnOffMenu(OSDItemID);	SetMenuRedraw();	break;	// 
		case  3: SetMD_AreaDispMenu(OSDItemID);	break;	// Area display(Off, On)
		case  4: ItemExeMDetSizPos(zID);			break;	// Size/Position
		case  5: SetMD_Threshold(OSDItemID);		break;	// Sensitivity(0~60)
		case  6: SetMD_MotionView(OSDItemID);		break;	// Motion View(Off, On)
		case  7: SetMenuDefaultItem(mID);					// Menu default setting
				 SetMD_FactoryDefaultMenu();		SetMenuRedraw();
				 break;
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeGMCorrect(BYTE tID)
{
	switch(tID){
		case  0: DEMO_SetGammaValue(OSDItemID);		break;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		case  1: DEMO_SetFrontGammaEn(OSDItemID);		break;	
		case  2: DEMO_SetRearGammaEn(OSDItemID);		break;		
		case  3: DEMO_SetGamma8tap(OSDItemID); 		break;
		case  4: DEMO_SetBAYER_LUT_Extend(OSDItemID);		break;
		case  5: DEMO_SetRGB_LUT_Extend(OSDItemID);			break;		
		case  6: DEMO_SetYC_LUT_Extend(OSDItemID);			break;		
		case  7: DEMO_SetBYLUT_Xresolution(OSDItemID);		break;
		case  8: DEMO_SetRGBLUT_Xresolution(OSDItemID);		break;		
		case  9: DEMO_SetRGBLUT_ThermoEN(OSDItemID);		break;				
#endif
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeDZoom(BYTE tID)
{
	WORD mID = GetOSDMenuID();
	BYTE zID = LO4BIT(LOBYTE(mID));	//Level 3,4 ID

	switch (tID)	{
		case  0: DEMO_SetZoomMode(OSDItemID);	SetMenuRedraw();		break;	// E-Zoom Mode(Off, On>)
		case  1: ItemExeZoomPosition(zID);			break;	// Zoom Position(U/D/L/R)
		case  2: DEMO_SetPIPMode(OSDItemID);		break;	// PIP Mode(Off, On)
		case  3: DEMO_SetPIPSize(OSDItemID);	SetMenuRedraw();		break;	// PIP Size
		case  4: ItemExePIPPosition(zID);			break;	// PIP Position(U/D/L/R)
		case  5: DEMO_SaveZoomRatioPosi();	SetMenuStatus(DZSAVE_MENU, 0); 	fMenuWaitFunc=ON;	break;
		case  6: SetMenuDefaultItem(mID);					// Menu default setting
					SetDZOOM_FactoryDefaultMenu();
					SetMenuRedraw();
				 break;	
	}
}

//--------------------------------------------------------------------------------------
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)
static void ItemExeDefogDwdr(BYTE tID)
{
	switch (tID)	{
		case  0: SetBY_DefogDWDRMode(OSDItemID);	SetMenuRedraw(); break;
		case  1: SetBY_DefogStrength(OSDItemID);	SetMenuRedraw(); break;	// Sensitivity(Low, MID, High)
#if defined(__USE_MDIN_i500__)						// MDIN-i500
		case  2: SetBY_DefogBright(OSDItemID);		break;	// Bright(0-20, center 10)
#else												// MDIN-i51X (i510, i540, i550)
		case  2: SetBY_DefogThreshold(OSDItemID);		break;	// Bright(0-20, center 10)
		case  3: SetBY_DWDRSaturation(OSDItemID);		SetMenuRedraw(); break;	// Bright(0-20, center 10)
		case  4: SetBY_DWDRLocalRatio(OSDItemID);		SetMenuRedraw(); break;
		case  5: SetBY_DWDRSatSync(OSDItemID);			break;
#endif
	}
}

#else
static void ItemExeDefog(BYTE tID)
{
	switch (tID)	{
		case  0: SetBY_DefogMode(OSDItemID);		break;
		case  1: SetBY_DefogStrength(OSDItemID);		SetMenuRedraw(); break;	// Sensitivity(Low, MID, High)
#if defined(__USE_MDIN_i500__)						// MDIN-i500
		case  2: SetBY_DefogBright(OSDItemID);		break;	// Bright(0-20, center 10)
#else												// MDIN-i51X (i510, i540, i550)
		case  2: SetBY_DefogThreshold(OSDItemID);		break;	// Bright(0-20, center 10)
		case  3: SetBY_DefogAutoLevel(OSDItemID);		break;	// Bright(0-20, center 10)
#endif
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeDWDR(BYTE tID)
{
	switch (tID)	{
		case  0: DEMO_SetDWDRMode(OSDItemID);		break;
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		case  1: SetBY_DWDRStrength(OSDItemID);		SetMenuRedraw(); break;	// Sensitivity(Low, MID, High)
		case  2: SetBY_DWDRSaturation(OSDItemID);		SetMenuRedraw(); break;	// Bright(0-20, center 10)
		case  3: SetBY_DWDRLocalRatio(OSDItemID);		SetMenuRedraw(); break;
		case  4: SetBY_DWDRSatSync(OSDItemID);			break;
		case  5: SetBY_DWDRAutoLevel(OSDItemID);		break;
#endif
	}
}


#endif

//--------------------------------------------------------------------------------------
static void ItemExeDefPixel(BYTE tID)
{
	switch (tID)	{
		case  0:	DEMO_SetLiveDPCMode(OSDItemID);	break;	// Live DPC Mode(Auto, off, on)
		case  1:	DEMO_StatDPCRun(OSDItemID);	break;		// static process go
		case  2:	SetSDPD_Shutter_level(OSDItemID);	break;		// dpd thhd set		
		case  3:	//SetSDPC_FlashTempRead_Order();
				SetSDPC_FlashUpdate_Order();	
				SetSDPC_MemWrite_Order();
				SetMenuStatus(SDPC_WRITE_MENU, 0); 	fMenuWaitFunc=ON;	
				break;	// flash write
		case  4:	OSD_MessageBox(ON, "SDPC RESULT CLEAR");
				SetSDPC_FlashClear_Order();	
				SetSDPC_MemReset_Order();	
				SetMenuStatus(SDPC_CLEAR_MENU, 0); fMenuWaitFunc=ON;	
				break;	// flash write	
		case  5:	 
			SetMenuStatus(DDPC_THHD1_MENU,GetDDPC_Threshold1(OSDItemID));		
			SetMenuStatus(DDPC_THHD2_MENU,GetDDPC_Threshold2(OSDItemID));			
			SetMenuStatus(DDPC_SELMODE_MENU,GetDDPC_SelMode(OSDItemID));						
			SetMenuRedraw();	
			break;
		case  6:	SetDDPC_Threshold1(GetMenuStatus(DDPC_AUTO_MENU),OSDItemID);	
				if((GetMenuStatus(DPC_MENU)==1)||(GetMenuStatus(DPC_MENU)==3))	DEMO_SetAutoDynamicDPCEn(ON);
			break;
		case  7:	SetDDPC_Threshold2(GetMenuStatus(DDPC_AUTO_MENU),OSDItemID);	
				if((GetMenuStatus(DPC_MENU)==1)||(GetMenuStatus(DPC_MENU)==3))	DEMO_SetAutoDynamicDPCEn(ON);
			break;
		case  8: SetDDPC_SelMode(GetMenuStatus(DDPC_AUTO_MENU),OSDItemID);	
				if((GetMenuStatus(DPC_MENU)==1)||(GetMenuStatus(DPC_MENU)==3))	DEMO_SetAutoDynamicDPCEn(ON);
			break;
		case  9: 	SetDPC_DebugDisp(OSDItemID);	
		
				if((GetMenuStatus(DPC_MENU)==1)||(GetMenuStatus(DPC_MENU)==3))	{
					if(GetDPC_DebugDisp()!=1)	OSD_IrkeyTitleDisplay(OFF, "          ");	
				}
				else		OSD_IrkeyTitleDisplay(OFF, "          ");	
			break;
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeDispFunc(BYTE tID)
{
	switch (tID)	{
		case  1: DEMO_SetFreezeMode(OSDItemID);		break;	// Freeze(Off, On)
		case  2: DEMO_SetMirrorMode(OSDItemID);		break;	// Mirror(Off, Mirror, V-flip, both)
		case  3: DEMO_SetImageEffect(OSDItemID);	break;	// Image Effect(Off, Neg, Red, Blue, Green)
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		case  4: Demo_SetRotation(OSDItemID);	SetMenuRedraw();	break;	// Rotation(Off, rotate_left, rotate_right)
		// Picture style
		case  5: 
			SetApp_ssPictureStyle(OSDItemID); 
			SetMenuStatus(PICSTYLE_CON_MENU,GetApp_ssPictureContrast());	
			SetMenuStatus(PICSTYLE_SAT_MENU,GetApp_ssPictureSaturation());	
			SetMenuStatus(PICSTYLE_EDE_MENU,GetApp_ssPictureEE());	
			SetMenuRedraw();	break;	// Picture style
		case  6: SetApp_ssPictureContrast(OSDItemID); break;	// Picture style : contrast
		case  7: SetApp_ssPictureSaturation(OSDItemID);	break;	// Picture style : saturation
		case  8: SetApp_ssPictureEE(OSDItemID);	break;	// Picture style : edge enhance
#endif
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeCamTitle(BYTE tID)
{
	switch (tID)	{
		case  0: OSD_CamTitleDisplay(OSDItemID);	break;	// Cam Title(off, on)
		case  1: OSD_CamTitleDisplay(ON);	break;			// Text input.
		case  2: SaveCamTitleColor(OSDItemID);	OSD_CamTitleDisplay(ON);	break;			// Text color
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeRS485(BYTE tID)
{
	switch (tID)	{
		case  1: DEMO_Set485CamID(OSDItemID);	 break;		// Cam ID(0~255)
		case  2: DEMO_Set485IDDisp(OSDItemID);	 break;		// Cam ID display
		case  3: DEMO_Set485Baudrate(OSDItemID); break;		// Cam ID display
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeMenuColor(BYTE tID)
{
	switch (tID)	{
		case  1:  SaveMenuColor(1, OSDItemID); 	SetMenuRedraw(); 	break;
		case  2:  SaveMenuColor(2, OSDItemID); 	SetMenuRedraw(); 	break;
		case  3:  SaveMenuColor(3, OSDItemID); 	SetMenuRedraw(); 	break;
		case  4:  SaveMenuColor(4, OSDItemID); 	SetMenuRedraw(); 	break;
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeFactoryReset(BYTE tID)
{
	switch (tID)	{
		case  2: 	OSD_WaitMessageBox(ON);
					DEMO_SetFactoryReset();
					OSD_WaitMessageBox(OFF);
					break;			// Factory Reset
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeOutFormat(BYTE tID)
{
	switch (tID)	{
		case  2: 	DEMO_SetOutputFormat(GetMenuStatus(OUTFRMT_MENU) + MISP_VIDOUT_1280x720p60);	// nID -> frmt
					SetOSDMenuID(OUTFRMT_MENU);
					SetMenuRedraw();	break;
	}
}

//--------------------------------------------------------------------------------------
static void ItemExe_CCCtrl(BYTE tID)
{
	switch (tID) {
		case  1: SetDM_ColorCCtrl_Red(OSDItemID);		break;
		case  2: SetDM_ColorCCtrl_Green(OSDItemID);		break;	
		case  3: SetDM_ColorCCtrl_Blue(OSDItemID);		break;	
#if defined(CAC_ctrl_theta)
		case  4: 
			SetMenuStatus(CAC_Theta_MENU,GetApp_CACTheta(GetMenuStatus(CAC_AreaSel_MENU)));		
			SetMenuStatus(CAC_Saturation_MENU,GetApp_CACSaturation(GetMenuStatus(CAC_AreaSel_MENU)));					
			SetMenuRedraw();	
			break;	// Picture style		
		case  5: SetApp_CACTheta(OSDItemID, GetMenuStatus(CAC_AreaSel_MENU));		break;
		case  6: SetApp_CACSaturation(OSDItemID, GetMenuStatus(CAC_AreaSel_MENU));		break;		
		case  7: SetApp_CAC_WriteEEPROM();		SetMenuStatus(CAC_EEPROM_MENU, 0); fMenuWaitFunc=ON;	break;					
#else
		case  4: 
			SetMenuStatus(CAC_Cos_MENU,GetApp_CACCos(GetMenuStatus(CAC_AreaSel_MENU)));		
			SetMenuStatus(CAC_Sin_MENU,GetApp_CACSin(GetMenuStatus(CAC_AreaSel_MENU)));		
			SetMenuStatus(CAC_Saturation_MENU,GetApp_CACSaturation(GetMenuStatus(CAC_AreaSel_MENU)));					
			SetMenuRedraw();	
			break;	// Picture style		
		case  5: SetApp_CACCos(OSDItemID, GetMenuStatus(CAC_AreaSel_MENU));		break;
		case  6: SetApp_CACSin(OSDItemID, GetMenuStatus(CAC_AreaSel_MENU));		break;	
		case  7: SetApp_CACSaturation(OSDItemID, GetMenuStatus(CAC_AreaSel_MENU));		break;	
		case  8: SetApp_CAC_WriteEEPROM();		SetMenuStatus(CAC_EEPROM_MENU, 0); fMenuWaitFunc=ON;	break;			
#endif

		default : break;
	}

}
//--------------------------------------------------------------------------------------

static void ItemExe_TPCtrl(BYTE tID)
{

	WORD mID = GetOSDMenuID();
	BYTE zID = LO4BIT(LOBYTE(mID));	//Level 3,4 ID
	
	switch (tID) {
		case  1: DEMO_SetTPCtrl(OSDItemID);	 break;
		case  2: DEMO_SetTPOnOffCtrl(OSDItemID);	break;
		case  3: ItemExeTPSizPos(zID);	break;
		case  4: SetMenuDefaultItem(mID);					// Menu default setting
				DEMO_SetTPDefault();		SetMenuRedraw();
				break;
	}
	 
}

//--------------------------------------------------------------------------------------
static void ItemExeMiscTune(BYTE tID)
{
	switch (tID) {
		case  1: DEMO_SetYCConv(OSDItemID); 				break; 	// GBGR(Off,On)
		case  2: DEMO_SetOUTCSCDefine(OSDItemID); 				break; 	// GBGR(Off,On)
		case  3: SetOUT_MainDithering(OSDItemID); 			break; 	// video output dither
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		case  4: DEMO_SetLVDSPLL(OSDItemID); 				break; 	// lvds pll -- for test
		case  5: IrkeyEnableOnOff((OSDItemID)?  ON:  OFF); break; 	// ir key enable on/off -- for test
		case  6: DEMO_SetHdmiTxMode(OSDItemID);		break;	// 0:select.., 1:422->422(for M400_quad), 2:422->444(709), 3:444(601)
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
		case  7: SetAPP_Set4K30P60P_Mode(OSDItemID);		break;
#elif defined(__PANORAMA_SETUP__)
		case  7: SetAPP_Pano_Menu_Ctrl(OSDItemID);						// panorama menu control mode
				SetMenuStatus(MISCTUNE_PANPARAM_MENU, GetAPP_Pano_Menu_Param());		SetMenuRedraw();
				break;
		case  8: SetAPP_Pano_Menu_Param(OSDItemID);	break;			// panorama menu control parameter
#else
		case  7: ChangeTestMode(OSDItemID);			break;
#endif
#endif
	}
}

//----------------------------------------------------------------------------------------
static void ItemExeWDRTune(BYTE tID)
{
	if(tID<DEMO_GetWDR_FNUM()) {
		PBYTE func_tbl=DEMO_GetWDR_FTBL();
		BYTE fID=func_tbl[tID];
		DEMO_SetWDR_FUNC[fID](OSDItemID);
		if(tID==0)	SetMenuRedraw();					// menu redraw : wdr on/off
		if((tID==3) || (tID==4))	SetMenuRedraw();	// menu redraw : wdr ae, tm(3), line/frame(4)
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeDNRTune(BYTE tID)
{
	switch (tID) {
		case  1: SetNR_DemoMode((MISP_NRDEMO_MODE_t)OSDItemID);
					OSD_DrawOutlineDouble((OSDItemID)?  ON:  OFF, GetOUT_MainSizeHA(), GetOUT_MainSizeVA());
				break;		// Demo mode select
		case  2: DEMO_LoadNR3DLowMode(OSDItemID);	SetMenuRedraw();	break;	// check low light mode
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		case  3: SetNR_MittlerfilterLevel(OSDItemID) ;	break;		
		case  4: SetNR_LetzteresfilterLevel(OSDItemID) ;	break;		
#endif
	}
}

#if !defined(__USE_2MOTOR_LENS__)
//--------------------------------------------------------------------------------------
static void ItemExeConference(BYTE tID)
{
	switch (tID)	{
		case  1:	SetAFConferenceMode(OSDItemID); 	SetMenuRedraw();	break;		// on/off
		case  2:	SetAFConferenceWinWidth(OSDItemID);						break;		// win width
		case  3:	SetAFConferenceWinHeight(OSDItemID);					break;		// win height
		case  4:	SetAFConferenceWinXPos(OSDItemID);						break;		// win x-position
		case  5:	SetAFConferenceWinYPos(OSDItemID);						break;		// win y-position
	}
}
#endif

//--------------------------------------------------------------------------------------
static void ItemExeVectorScope(BYTE tID)
{
	switch (tID) {
		case  0: SetVS_OnOffMenu(OSDItemID);						break;
		case  1: SetVS_ScanAreaMenu(OSDItemID);	SetMenuRedraw();	break;
		case  2: SetVS_PosiXMenu(OSDItemID);	SetMenuRedraw(); 	break;
		case  3: SetVS_PosiYMenu(OSDItemID);	SetMenuRedraw(); 	break;
		case  4: SetVS_SizeXMenu(OSDItemID);	SetMenuRedraw(); 	break;
		case  5: SetVS_SizeYMenu(OSDItemID);	SetMenuRedraw(); 	break;
		case  6: SetVS_DrawMenu();	SetMenuStatus(VSDRAW_MENU, 0);	fMenuWaitFunc=ON;	break;
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeFactorySetup(BYTE tID)
{
	switch (tID) {
		case  1: SetPwmIrisTune();				break;	// pwm iris offset
		case  2: SetAEIrisSpeed(OSDItemID); 	break;	// pwm iris speed
	}
}

static void ItemExeCCFSetup(BYTE tID)
{
	switch (tID) {
		case  1: 	SetMenuStatus(CCF_Gain_MENU,GetCCFGain(GetMenuStatus(CCF_LineSel_MENU)));		
					SetMenuStatus(CCF_Balance_MENU,GetCCFBalance(GetMenuStatus(CCF_LineSel_MENU)));					
					SetMenuRedraw();				
					break;	
		case  2: SetCCFGain(OSDItemID, GetMenuStatus(CCF_LineSel_MENU));		break;
		case  3: SetCCFBalance(OSDItemID, GetMenuStatus(CCF_LineSel_MENU));	break;
		case  4: SetCCFType(OSDItemID);		break;
		case  5: CCFResultPrint();		break;
		
	}
}



/*
//--------------------------------------------------------------------------------------
static void ItemExeCompress(BYTE tID)
{
	switch (tID)	{
		case  1: DEMO_SetBNRCtrl(OSDItemID);			break;		// BNR ctrl
		case  2: DEMO_SetCompressCtrl(OSDItemID);		break;		// Compress ctrl
		case  3: DEMO_SetCompressMode(OSDItemID);		break;		// Compress mode
		case  4: DEMO_SetCompressDefault(OSDItemID);	break;		// Compress default
	}
}
*/
/*
//--------------------------------------------------------------------------------------
static void ItemExeAR0331ModeSel(BYTE tID)	
{
#if defined(__USE_CMOS_AR0331__)	// use Aptina HDR cmos	

	WORD	ShutterReturn = GetMenuStatus(SHUTTER_MENU);

	switch (tID) {
		case  0: AR0331_Parallel_Linear_Nomal_1080p30();	
				DEMO_HTMOnOFF(OFF);	SetMenuStatus(HTMOnOff_MENU, 0);		DEMO_SetExpShutter(ShutterReturn);	SetMenuStatus(SHUTTER_MENU,ShutterReturn);		break;
		case  1: AR0331_Parallel_HDR_ALTM_1080p30();		
				DEMO_HTMOnOFF(OFF);		SetMenuStatus(HTMOnOff_MENU, 0);		break;
		case  2: AR0331_Parallel_HDR_COMP_1080p30();		
				DEMO_HTMOnOFF(ON);		SetMenuStatus(HTMOnOff_MENU, 1);		DEMO_SetExpShutter(ShutterReturn);	SetMenuStatus(SHUTTER_MENU,ShutterReturn);	break;
	}

//	switch (tID) {
//		case  0: AR0331_Parallel_Linear_Nomal_1080p30(ON);	DEMO_HTMOnOFF(OFF);		SetMenuStatus(HTMOnOff_MENU, 0);		break;
//		case  1: AR0331_Parallel_Linear_ALTM_1080p30(ON);	DEMO_HTMOnOFF(OFF);		SetMenuStatus(HTMOnOff_MENU, 0);		break;
//		case  2: AR0331_Parallel_HDR_COMP_1080p30(ON);		DEMO_HTMOnOFF(ON);		SetMenuStatus(HTMOnOff_MENU, 1);		break;
//		case  3: AR0331_Parallel_HDR_ALTM_1080p30(ON);		DEMO_HTMOnOFF(OFF);		SetMenuStatus(HTMOnOff_MENU, 0);		break;	
//	}

	SetMenuRedraw();
	
#endif
}

static void ItemExeIMX136_BulitinWDRModeSel(BYTE tID)
{
#if defined(__USE_CMOS_IMXx36__)	// use SONY 136 HDR cmos	

	WORD	ShutterReturn = GetMenuStatus(SHUTTER_MENU);

	switch (tID) {
		case  0: IMX136_Nomal_Mode_return();
				DEMO_HTMOnOFF(OFF);	SetMenuStatus(HTMOnOff_MENU, 0);		DEMO_SetExpShutter(ShutterReturn);	SetMenuStatus(SHUTTER_MENU,ShutterReturn);		break;
		case  1: IMX136_Builtin_WDRmode_comp();	
				DEMO_HTMOnOFF(ON);		SetMenuStatus(HTMOnOff_MENU, 1);		break;
		case  2: 	IMX136_Builtin_WDRmode();		
				DEMO_HTMOnOFF(OFF);		SetMenuStatus(HTMOnOff_MENU, 0);		break;
	}



	SetMenuRedraw();
	
#endif
}
*/

//Lv2
//--------------------------------------------------------------------------------------
static void ItemExeFocus(BYTE sID)								// 1.focus
{
#if !defined(__USE_2MOTOR_LENS__)
	WORD mID = GetOSDMenuID();
	BYTE tID = HI4BIT(LOBYTE(mID));
#endif
	
	switch (sID)	{
#if !defined(__USE_2MOTOR_LENS__)
		case  1: SetAFModeOnOffMenu(OSDItemID);	SetMenuRedraw();		break;
		case  2: SetWIDEDrvLIMITMenu(OSDItemID);	break;
		case  3: SetTELEDrvLIMITMenu(OSDItemID);	break;		
		case  4: SetZOOMDrvSPEEDMenu(OSDItemID);	break;
		case  5: SetFOKSDrvLIMITMenu(OSDItemID);	break;
		case  6: SetAFIntvalTIMEMenu(OSDItemID);	break;
		case  7: ItemExeConference(tID);		break;
#else
		case  1: SetTwoMotorAfMode(OSDItemID);		break;
		case  2: SetTwoMotorScanMode(OSDItemID);	break;
		case  3: SetTwoMotorOnePush();
				SetMenuStatus(FCS_ONEPUSH_MENU, 0); fMenuWaitFunc=ON;	break;
		case  4: SetTwoMotorSyncTDN(OSDItemID);		break;
		case  5: SetTwoMotorInitial();
				SetMenuStatus(FCS_INIT_MENU, 0); fMenuWaitFunc=ON;	break;
#endif
	}
}

//----------------------------------------------------------------------------------------
static void ItemExeExposure(BYTE tID)							// 2.Exposure
{
	switch (tID) {
		case  1: SetAECtrlMode(OSDItemID);	SetMenuRedraw();	break;	// ae mode
		case  2: SetAEShutSpeed(OSDItemID);	SetMenuRedraw();	break;	// shutter(exposure)
		case  3: SetAEIrisGain(OSDItemID);					break;	// iris(exposure)
		case  4: SetAEAGCGain(OSDItemID);	SetMenuRedraw();	break;	// agc(exposure)
		case  5: SetAEGainCtrlMode(OSDItemID);				break;	// agc max(exposure)
		case  6: DEMO_SetExpSensUp(OSDItemID, 0);				break;	// sensup
		case  7: SetAEBrightLvl(OSDItemID); 				break;	// brightness(exposure)
		case  8: SetAEFlkMode(OSDItemID); 					break;	// flk mode
#if defined(__USE_PWMIRIS_ONLY__)
		case  9: SetAELensMode(OSDItemID); 					break;	// lens mode
#if (defined(__USE_CMOS_IMX265__) && !defined(__USE_IMX265_SLAVE_MODE__))
		case 10: SetGSTriggerStandby	(OSDItemID); 	break;
#endif
#else
#if (defined(__USE_CMOS_IMX265__) && !defined(__USE_IMX265_SLAVE_MODE__))
		case 9:	SetGSTriggerStandby	(OSDItemID); 	break;
#endif
#endif

	}
}

//----------------------------------------------------------------------------------------
static void ItemExeWB(BYTE sID)								// 3.White Balance
{
	switch (sID) {
		case  1: SetWBModeSetMenu(OSDItemID); 	SetMenuRedraw();	break;			// wb mode(atw, onepush, indoor, outdoor, preset, manual)
		case  2: SetWBCtrlOnOff();	SetMenuStatus(WB_PUSH_MENU, 0); fMenuWaitFunc=ON;	break;	// wb onepush set
		case  3: DEMO_SetWBManBlue(OSDItemID);	break;
		case  4: DEMO_SetWBManRed(OSDItemID);	break;
		case  5: DEMO_SetWBSpeed(OSDItemID);	break;
		case  6: DEMO_SetWBAutoBlue(OSDItemID);	break;
		case  7: DEMO_SetWBAutoRed(OSDItemID);	break;
	}
}

//----------------------------------------------------------------------------------------
static void ItemExeDYNT(BYTE sID)								// 4.Day & Night
{
	switch (sID) {
		case  1: SetDYNT_Mode(OSDItemID);	SetMenuRedraw();			break;		// daynight mode
		case  2: 
				if(OSDItemID < 1)	{									// min : 1
					OSDItemID=1;	SetMenuStatus(DANT_DELAY_MENU, OSDItemID);	SetMenuRedraw();
				}
				SetDYNT_AutoDly(OSDItemID);				break;		// daynight delay
		case  3: if(OSDItemID < 1)	{									// min : 1
					OSDItemID=1;	SetMenuStatus(DANT_AGCD2N_MENU, OSDItemID);	SetMenuRedraw();
				}
				SetDYNT_AGCD2N(OSDItemID);	SetMenuRedraw();	break;		// daynight day -> night agc level
		case  4: SetDYNT_AGCN2D(OSDItemID);	SetMenuRedraw();	break;		// daynight night -> day agc level
		case  5: SetDYNT_NTColor(OSDItemID);	SetMenuRedraw();	break;		// daynight night color mode
		case  6: SetDYNT_CVBSBurst(OSDItemID);				break;		// daynight color burst
//		case  7: SetDYNT_SaveEEPROMInfo();					break;		// daynight info save to eeprom
	}
}

//----------------------------------------------------------------------------------------
static void ItemExeDNR(BYTE sID)								// 5.Noise Reduction
{
	switch (sID) {
		case  1: SetNR_Mode((MISP_NR_MODE_t)OSDItemID);	 SetMenuStatus(DNR_LEVEL_MENU,GetNR_intensity());  SetMenuRedraw();	break;		// dnr mode
		// case  2: SetNR_3Dintensity((MISP_NR3D_MODE_t)OSDItemID);		break;		// dnr level
		case  2: SetNR_intensity((MISP_NR3D_MODE_t)OSDItemID);		break;		// dnr level
		case  3: SetNR_Aperture(OSDItemID);		break;		// dnr aperture
	}
}

//--------------------------------------------------------------------------------------
static void ItemExePMask(BYTE sID)								// 6.Privacy Mask
{
	WORD mID = GetOSDMenuID();
	WORD PMType = GetMenuStatus(PM_TYPE_MENU);

	if(GetMenuStatus(PM_MODE_MENU)==ON)	{			//blocking overlap block -md,pm,backlight
		SetBLC_OnOffMenu(OFF); 			SetHLC_OnOffMenu(OFF); 		SetMD_OnOffMenu(OFF);
		SetMenuStatus(BLIGHT_MENU,0); 	SetMenuStatus(MD_MENU,0);
	}

	switch (sID)	{
		case  1: SetPM_OnOffMenu(OSDItemID);	SetMenuRedraw();	break;		// pm mode
		case  2: SetPM_TypeMenu(OSDItemID);	SetMenuRedraw();	break;		// pm type(square, polygon)
		case  3: SetPM_MaskNoMenu(OSDItemID);	SetMenuRedraw();	break;	// Area(Area1,2,3,4)
		case  4: SetPM_MaskOnOffMenu(OSDItemID);				break;		// Area display(Off, On)
		case  5: SetPM_ColorMenu(OSDItemID);					break;		// Color(16 color)
		case  6: if(PMType==1)	SetPM_PolyEdgeSelMenu(OSDItemID);	SetMenuRedraw();	break;	// polygon edge select
		case  7: if(PMType==1)	SetPM_PolyXposiMenu(OSDItemID);			// polygon position(x)
				  else			SetPM_RectPosiXMenu(OSDItemID); 		// polygon position(x)
				  SetMenuRedraw();
				break;
		case  8: if(PMType==1)	SetPM_PolyYposiMenu(OSDItemID);			// polygon position(y)
				  else			SetPM_RectPosiYMenu(OSDItemID);	// square position(y)
				  SetMenuRedraw();
				break;
		case  9: if(OSDItemID < 1)	{
					OSDItemID=1;	SetMenuStatus(PM_SQRW_MENU, OSDItemID);	SetMenuRedraw();	
				  }
					SetPM_RectSizeWMenu(OSDItemID);							// square size(x)
					SetMenuRedraw();
					break;
		case 10: if(OSDItemID < 1)	{
					OSDItemID=1;	SetMenuStatus(PM_SQRH_MENU, OSDItemID);	SetMenuRedraw();	
				  }
					SetPM_RectSizeHMenu(OSDItemID);							// square size(x)
					SetMenuRedraw();
					break;
		case 11: SetMenuDefaultItem(mID);
					OSD_WaitMessageBox(ON);
					SetPM_FactoryDefaultMenu();
					OSD_WaitMessageBox(OFF);
					SetOSDMenuID(PM_RETURN_MENU);	SetMenuRedraw();	break;
	}
}

//----------------------------------------------------------------------------------------
static void ItemExeEffect(BYTE sID)						// 7.Effect
{
	WORD mID = GetOSDMenuID();
	BYTE tID = HI4BIT(LOBYTE(mID));
	
	switch (sID) {		 
		case  1: SetBY_Lens_corr_en(OSDItemID);	break;		// LSC(Off, On)
		case  2: ItemExeGMCorrect(tID);		break;
		case  3: DEMO_SetContrast(OSDItemID); 	break;		// Contrast(0~20)
		case  4: DEMO_SetBrightness(OSDItemID); break;		// Brightness(0~20)
		case  5: DEMO_SetSaturation(OSDItemID); break;		// Saturation(0~20)
		case  6: DEMO_SetHue(OSDItemID); 		break;		// Hue(0~20)
		case  7: DEMO_SetSharpness(OSDItemID);	break;		// Sharpness(0~20)
		case  8: DEMO_SetAutoSaturation(OSDItemID);	break;		// Menu default setting		
		case  9: DEMO_SetAutoEdgeOnOff(OSDItemID);	break;		// Menu default setting	
//		case  10: DEMO_SetAutoBright(OSDItemID);	break;		// Menu default setting			
		case  10: SetMenuDefaultItem(mID);					// Menu default setting
				 DEMO_SetEffectDefault();	SetMenuRedraw();				
				 break;	
	}
}

//----------------------------------------------------------------------------------------
static void ItemExeSpecial(BYTE sID)						// 8.Special
{
	WORD mID = GetOSDMenuID();
	BYTE tID = HI4BIT(LOBYTE(mID));	//BYTE zID = LO4BIT(LOBYTE(mID));	//Level 3,4 ID

	switch (sID) {
		case  1: ItemExeDZoom(tID);			break;		// D-Zoom(Off, On>)
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)		
		case  2: ItemExeDefogDwdr(tID);			break;		// Defog/Dwdr(>)
		case  3: break;										// blank
#else
		case  2: ItemExeDefog(tID);			SetMenuRedraw();	break;		// Defog(Off, Auto>, ON)
		case  3: ItemExeDWDR(tID);				break;		// D-WDR(Off, AUTO, ON = force)
		
#endif		
		case  4: ItemExeMDet(tID);				break;		// Motion Detect(Off, On>)
		case  5: Demo_DIS_OnOff(OSDItemID);	SetMenuRedraw();		break;		// dis(off, on)
		case  6: ItemExeBLight(tID);			break;		// Back Light mode(Off, BLC, HSBLC)
		case  7: DEMO_BINNING_MODE(OSDItemID);	SetMenuRedraw();	break;		// Binning Mode	
		case  8: ItemExeDefPixel(tID);			break;		// Defect Pixel(>)
		case  9: ItemExeDispFunc(tID);			break;		// Disp. Func(Freeze, Mirror, Img.effect)
		case 10: ItemExeWDRTune(tID);			break;		// wdr(off/on)
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		case 11: DEMO_FLKDET_MODE(OSDItemID);	SetMenuRedraw();	break;	// FLK Detect Mode	
#endif
	}
}

//----------------------------------------------------------------------------------------
static void ItemExeSetup(BYTE sID)							// 9.Setup
{
	WORD mID = GetOSDMenuID();
	BYTE tID = HI4BIT(LOBYTE(mID));

	switch (sID) {
		case  1: ItemExeCamTitle(tID);			break;		// Cam Title(Off, On>)
		case  2: DEMO_SetNTPal(OSDItemID);		break;		// NTSC/PAL(NTSC, PAL)
		case  3: ItemExeRS485(tID);				break;		// RS485(>)		
		case  4: DEMO_SetLang(OSDItemID);	SetMenuRedraw();		break;		// Language(ENG, CHN, JPN, KOR)
		case  5: ItemExeMenuColor(tID);		break;		// Menu Color
		case  6: ItemExeFactoryReset(tID);		break;		// Factory Reset
		case  8: DEMO_SetMenuPosition(OSDItemID);	break;
		case  9: ItemExeOutFormat(tID);	break;	// video output resolution
#if defined(__USE_IR_LED_PWM__)
		case 10: DEMO_SetLEDPWMLowWidth(OSDItemID);		break;	// led pwm low pulse width
		case 11: DEMO_SetLEDPWMHighWidth(OSDItemID);	break;	// led pwm high pulse width
#endif
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeEngr(BYTE sID)									// 10.Engineer Menu
{

	WORD mID = GetOSDMenuID();
	BYTE tID = HI4BIT(LOBYTE(mID));

	switch (sID)	{
		case  1: DEMO_SetSensorHDMI_PIPSEL(OSDItemID);	SetMenuRedraw();	break;
		case  2: DEMO_SetInputFormat(OSDItemID);		break;
		case  3: ItemExe_CCCtrl(tID);						break;		// CSC(Off, On)
		case  4: DEMO_SetInputPattern(OSDItemID);		break;
		case  5: SetOUT_MainTestPattern(OSDItemID);
				SetOUT_AuxTestPattern(OSDItemID);		break;
		case  6: ItemExe_TPCtrl(tID);					break;		// TP Sub Menu(>)
		case  7: DEMO_SetB_BYPASSCtrl(OSDItemID);		break;		// Menu Position(center, top-left/right, bot-right/left)
		case  8: DEMO_SetSensorFps(OSDItemID);			break;		// Sensor FPS
		case  9: ItemExeMiscTune(tID);					break;		// misc tuning
		case 10: ItemExeWDRTune(tID);					break;		// wdr tuning
		case 11: ItemExeDNRTune(tID);					break;		// dnr tuning
	}
}

//--------------------------------------------------------------------------------------
static void ItemExeDebug(BYTE sID)									// 12.Debug Menu
{
	WORD mID = GetOSDMenuID();
	BYTE tID = HI4BIT(LOBYTE(mID));
	
	switch (sID)	{
		case  1: DEMO_SetDebugMode(OSDItemID);		break;
		case  2: DEMO_SetAgingTest(OSDItemID);		break;
		case  3: ItemExeVectorScope(tID);				break;
		case  4: ItemExeFactorySetup(tID);			break;
		case  5: ItemExeCCFSetup(tID);				
				SetMenuStatus(CCF_Print_MENU, 0); 	fMenuWaitFunc=ON;		break;
		case  6: SetOSDMenuID(0x0000);				break;	// exit debug osd
	}
}


//Lv 1
//----------------------------------------------------------------------
static void ItemExecute(WORD mID)		//Main menu.
{
	BYTE nID = HI4BIT(HIBYTE(mID));	BYTE sID = LO4BIT(HIBYTE(mID));	//Level 1,2 ID

	switch (nID)	{
		case  1:	ItemExeFocus(sID);		break;		// focus
		case  2:	ItemExeExposure(sID);	break;		// exposure
		case  3:	ItemExeWB(sID);			break;		// white balance
		case  4:	ItemExeDYNT(sID); 		break;		// day & night
		case  5:	ItemExeDNR(sID);		break;		// dnr
		case  6:	ItemExePMask(sID);		break;		// privacy mask
		case  7:	ItemExeEffect(sID);		break;		// effect
		case  8:	ItemExeSpecial(sID);	break;		// special
		case  9:	ItemExeSetup(sID);		break;		// setup
		case 10:	ItemExeEngr(sID);		break;		// engineer menu
		case 12:	ItemExeDebug(sID);		break;		// debug menu
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void MenuItemExeHandler(void)
{
	WORD mID = GetOSDMenuID();

	if (fItemExecute==OFF) return;
	else	   fItemExecute = OFF;

//	UARTprintf("mID = %04x\n", mID);
	ItemExecute(mID);
}

/*  FILE_END_HERE */
