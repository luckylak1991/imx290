//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  VIDEO.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__VIDEO_H__
#define		__VIDEO_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__COMMON_H__
#include	"common.h"
#endif
#ifndef		__CMOS_IF_H__
#include	"cmos_if.h"
#endif
#ifndef		__MISP_AE_H__
#include	"ae.h"
#endif
#ifndef		__MISP_AWB_H__
#include	"awb.h"
#endif
#ifndef		__MISC_H__
#include	"misc.h"
#endif
#ifndef		__TP2912_H__
#include	"tp2912.h"
#endif
#ifndef		__IT66121_H__
#include	"it66121.h"
#endif
#ifndef		__IT680X_H__
#include	"it680x.h"
#endif
#ifndef		__ITE6615_GOLBAL_H__
#include	"ITE6615_Global.h"
#endif
#ifndef		__LT8618EX_H__
#include	"lt8618ex.h"
#endif
#ifndef		__LT8618SX_H__
#include	"lt8618sx.h"
#endif
#ifndef		__LT8619C_H__
#include	"lt8619c.h"
#endif
#ifndef		__SIL9136_H__
#include	"sil9136.h"
#endif
#ifndef		__PROTOCOL_H__
#include	"protocol.h"
#endif
#ifndef		__CDCE913_H__
#include	"cdce913.h"
#endif
#ifndef		__ENXDEV_H__
#include	"enx_dev.h"
#endif
#ifndef		__LCMX03LF_H__
#include	"lcmx03lf.h"
#endif
#ifndef		__PS8409_H__
#include	"ps8409.h"
#endif
#ifndef		__LIAMD6K_H__
#include	"liamd6k.h"
#endif

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
#ifndef		__MISP_AF_H__
#include	"af.h"
#endif
#ifndef		__LENS_IF_H__
#include	"lens_if.h"
#endif
#endif

#if (defined(__USE_AN41919__) || defined(__USE_AN41908__))
#ifndef		__IRIS_IF_H__
#include	"iris_if.h"
#endif
#endif

#ifndef		__APPLICATION_INIT_H__
#include	"app_initial.h"
#endif

#if defined(__M33_V101__)
//#define MIMI_AF_DEBUG
//#define MIMI_AE_DEBUG
#else
//#define MIMI_AF_DEBUG
//#define MIMI_AE_DEBUG
#endif

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
typedef void (*DEMO_func)(WORD);

typedef	enum {
	CIS_15FPS = 0,		// CMOS : 30Fps
	CIS_30FPS,		// CMOS : 30Fps
	CIS_36FPS,			// CMOS : 36Fps
	CIS_48FPS,			// CMOS : 48Fps
	CIS_60FPS,			// CMOS : 60Fps
}	CIS_FPS_t;

typedef	enum {
	LowLight_Off = 0,		// Auto Control Off
	LowLight_Low,			// Auto Control Low
	LowLight_Mid,			// Auto Control Mid
	LowLight_High,			// Auto Control High
}	MISP_LowLight_Level_t;

typedef	enum {
	Trig_Stby_Off = 0,		// Normal shutter mode
	Trig_Stby_Wait,			// Sensor Wating trigger pulse
	Trig_Stby_Running		// Sensor Running
}	MISP_TriggerStandbymode_t;

typedef	struct
{
	BYTE 	shut[6];
	BYTE	iris[6];
	BYTE 	agc[6];
	BYTE 	agcmax[6];
	BYTE 	sensup[6];
	BYTE 	bright[6];
	BYTE	mode; 		// ae control mode(auto, shut fix,, )
	BYTE	irisspeed;
	BYTE	lensmode;
	BYTE	flkmode;
	
}	AE_MENU_PARAM, *PAE_MENU_PARAM;


#define		GET_FPS		(GetSS_SensorVoutFps())
	
// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------
extern AE_CTRL_PARAM stAECTRL;
extern AE_XFER_PARAM stAEXFER;
extern WB_CTRL_PARAM stWBCTRL;
extern AE_MENU_PARAM stAEMENU;

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
extern AF_CTRL_PARAM stAFCTRL;
#endif


// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

//ae_pwm.c
void AECTRL_IRISPWM_SetFullOpen(void);
void AECTRL_IRISPWM_SetFullClose(void);

void IRISPWM_SetCtrlInit(void);

// video.c
void CreateVideoInstance(void);
void ISPM ODMVsyncPulseDecode(void);
void WAIT_ODMVsyncPulse(void);
void ODMAutoCtrlHandler(void);
void VIDEO_RxHandler(void);
void VIDEO_TxHandler(void);
void VIDEO_ChkTunePINT(BYTE y);
void VIDEO_ChkTuneCOMP(BYTE y);
void VIDEO_ChkTunePOLY(BYTE y);
SHORT Get_YUV_CNT(void);
SHORT Get_YUV_Y(void);
SHORT Get_YUV_U(void);
SHORT Get_YUV_V(void);
BOOL Get_RGB_RatioDiff(LONG margin);
BOOL Get_YUV_Ratio(BOOL mode, LONG margin);

// app_3a.c
void GetAEDataState(void);
void GetAFDataState(void);
void SetAECtrlOnOffToggle(void);
void SetAECtrlOnOff(BYTE onoff);

void SetAE_BLCOnOff(void);
void SetAE_AGCOnOff(void);
void SetAE_SHTOnOff(void);
void setSpotAEOnOff(BOOL onoff);
void setSportAEPostion(BYTE x, BYTE y);

void SetAFModeOnOff(void);
void SetFOKSDrvLIMIT(void);
void SetZOOMDrvSPEED(void);
void SetWIDEDrvLIMIT(SHORT val);
void SetTELEDrvLIMIT(SHORT val);
void SetAFIntvalTIME(WORD val);

void SetAFModeOnOffMenu(WORD nID);
void SetWIDEDrvLIMITMenu(SHORT val);
void SetTELEDrvLIMITMenu(SHORT val);
void SetZOOMDrvSPEEDMenu(WORD nID);
void SetFOKSDrvLIMITMenu(WORD nID);
void SetAFIntvalTIMEMenu(WORD val);
WORD GetAFModeOnOffMenu(void);
WORD GetWIDEDrvLIMITMenu(void);
WORD GetTELEDrvLIMITMenu(void);
WORD GetZOOMDrvSPEEDMenu(void);
WORD GetFOKSDrvLIMITMenu(void);
WORD GetAFIntvalTIMEMenu(void);

void SetWBCtrlOnOff(void);
void SetWBModeOnOff(void);
void SetWBModeSetMenu(WORD nID);
WORD GetWBModeSetMenu(void);
WORD GetWBManualBGainMenu(void);
WORD GetWBManualRGainMenu(void);
WORD GetWBSpeedMenu(void);
WORD GetWBAutoBGainMenu(void);
WORD GetWBAutoRGainMenu(void);

LONG GetAFOPDWIND(BYTE sel);


// WB get rgb, yuv
SHORT Get_RGB_CNT(void);
SHORT Get_RGB_R(void);
SHORT Get_RGB_G(void);
SHORT Get_RGB_B(void);
SHORT Get_RGB_Y(void);
SHORT Get_RGB_U(void);
SHORT Get_RGB_V(void);

WORD Get_WB_U(WORD idx);
WORD Get_WB_V(WORD idx);

// jwlee 20130125
void SetToneMappingOnoff(BOOL OnOff);

void SetMainSource(BOOL src);
void SetPIPSource(BOOL src);

#if defined(__SENSOR_ERR_CHECK__)
void sensor_check_print(void);
void sensor_check_print2(void);
#endif

void sensor_check_osd(void);

void LowLight_AutoGamma(BOOL OnOff);
void LowLight_AutoSat(BYTE nID);
void LowLight_AutoCon(BOOL OnOff);
void LowLight_AutoEdge(BOOL OnOff);
void LowLight_AutoBrightness(BYTE nID);
void LowLight_AutoDyanmicDPC(void);
void LowLight_CSP(void);

void CAC_prevent_escape(void);

void AUTODefogCCtrl(void);
void SDPC_Return(void);		//control ae and menu control. because dpc code move driver level.

//-------------------------------------------
/*//do not delete this comment
void DEMO_SET_ADCCON(WORD nID);
void DEMO_SET_ADCSAM(WORD nID);
void DEMO_SET_ADC_Channel(WORD nID);
BOOL DEMO_ADC_EOC_Check(void);
WORD DEMO_ADC_Data_Read(BOOL EOC);
*/






void WB_AWCProcHandler(void);
void DEMO_SetContrast(WORD nID);
void DEMO_SetBrightness(WORD nID);
void DEMO_SetAECTRL_Brightness(WORD nID);
void DEMO_SetSaturation(WORD nID);
void DEMO_SetHue(WORD nID);
void DEMO_SetSharpness(WORD nID);
void DEMO_SetAutoConOnOff(BOOL OnOff);
void DEMO_SetAutoSaturation(BYTE nID);
void DEMO_SetAutoEdgeOnOff(BOOL OnOff);
void DEMO_SetAutoBright(BYTE nID);
void DEMO_SetZoomMode(WORD nID);
void DEMO_SetZoomRatio(WORD nID);
void DEMO_SetZoomPosition(WORD nID);
void DEMO_SaveZoomRatioPosi(void);
void IrCodeZoomMoveDirL(void);
void IrCodeZoomMoveDirR(void);
void IrCodeZoomMoveDirU(void);
void IrCodeZoomMoveDirD(void);
void DEMO_SetPIPOnOff(WORD nID);
void DEMO_SetPIPMode(WORD nID);
void DEMO_SetDemoPIPMode(WORD nID);
void DEMO_SetPIPSize(WORD nID);
void DEMO_SetPIPPosition(WORD nID);
void DEMO_SetWBAutoBlue(WORD nID);
void DEMO_SetWBAutoRed(WORD nID);
void DEMO_SetWBManBlue(WORD nID);
void DEMO_SetWBManRed(WORD nID);
void DEMO_SetWBSpeed(WORD nID);
void DEMO_SetGammaValue(WORD nID);
BYTE DEMO_GetContrast(void);
BYTE DEMO_GetBrightness(void);
BYTE DEMO_GetSaturation(void);
BYTE DEMO_GetHue(void);
BYTE DEMO_GetSharpness(void);
BOOL DEMO_GetAutoConOnOff(void);
BYTE DEMO_GetAutoSatLevel(void);
BOOL DEMO_GetAutoEdgeOnOff(void);
BYTE DEMO_GetAutoBright(void);

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
void DEMO_SetFrontGammaEn(BYTE nID);
void DEMO_SetRearGammaEn(BYTE nID);
void DEMO_SetGamma8tap(WORD nID);
void DEMO_SetBAYER_LUT_Extend(BYTE nID);
void DEMO_SetRGB_LUT_Extend(BYTE nID);
void DEMO_SetYC_LUT_Extend(BYTE nID);

void DEMO_SetBYLUT_Xresolution(BYTE nID);
void DEMO_SetRGBLUT_Xresolution(BYTE nID);
void DEMO_SetRGBLUT_ThermoEN(BYTE nID);
 
#endif

void DEMO_SetLiveDPCMode(WORD nID);
void DEMO_SetStatDPCMode(WORD nID);
void DEMO_SetAutoDynamicDPCEn(BOOL OnOff);
void DEMO_StatDPCRun(WORD nID);
void DEMO_SetDWDRMode(WORD nID);
void DEMO_SetSensorHDMI_PIPSEL(WORD nID);
void DEMO_SetInputFormat(WORD nID);	
void DEMO_SetOutputFormat(WORD nID);
WORD DEMO_GetOutputFormat(void);
void Setchange5MegaMode(void);
void Setchange2MegaMode(void); 
void IrCodePanningMoveDirL(void);
void IrCodePanningMoveDirR(void);
void IrCodePanningMoveDirU(void); 		
void IrCodePanningMoveDirD(void);		
void IrCodeCropMoveDirL(void);
void IrCodeCropMoveDirR(void);
void IrCodeCropMoveDirU(void);
void IrCodeCropMoveDirD(void);
void DEMO_SetNRInputMode(void);
void DEMO_SetFreezeMode(WORD nID);
void DEMO_SetMirrorMode(WORD nID);
BYTE DEMO_GetMirrorMode(void);
void DEMO_SetExpAGC(WORD nID);
void DEMO_SetFactoryReset(void);
void DEMO_SetExpSensUp(WORD nID, int mode); // mode 0 : normal , 1 : for AE only
WORD DEMO_GetExpSensUp(void);
void DEMO_SetImageEffect(WORD nID);
BYTE DEMO_GetImageEffect(void);
void DEMO_SetFlkMode(WORD nID);
void DEMO_SetEffectDefault(void);
void DEMO_SetHSBLCOnOff(WORD nID);
void DEMO_SetHSBLCArea(WORD nID);
void DEMO_SetHSBLCAreaDisp(WORD nID);
void DEMO_SetHSBLCPosition(WORD nID);
void DEMO_SetHSBLCSize(WORD nID);
void DEMO_SetHSBLCLevel(WORD nID);
void DEMO_SetHSBLCBM(WORD nID);
void DEMO_SetHSBLCDefault(BOOL OnOff);
void DEMO_SetBLCOnOff(WORD nID);
void DEMO_SetBLCMode(WORD nID);
void DEMO_SetBLCPosition(WORD nID);
void DEMO_SetBLCSize(WORD nID);
void DEMO_SetBLCDefault(BOOL OnOff);
void DEMO_SetFixAEDefault(BOOL OnOff);
void DEMO_SetBLC_DISPMode(WORD nID);
void DEMO_SetMDetMode(WORD nID);
void DEMO_SetMDetArea(WORD nID);
void DEMO_SetMDAreaOnOff(WORD nID);
void DEMO_SetMDetAreaDisp(WORD nID);
void DEMO_SetMDetPosition(WORD nID);
void DEMO_SetMDetSize(WORD nID);
void DEMO_SetMDetSens(WORD nID);
void DEMO_SetMDetMView(WORD nID);
void DEMO_SetMDetDefault(void);
void DEMO_Set485CamID(WORD nID);
void DEMO_Set485IDDisp(WORD nID);
void DEMO_Set485Baudrate(WORD nID);
void DEMO_SetNTPal(WORD nID);
WORD DEMO_GetNTPal(void);
void DEMO_SetLang(WORD nID);
void DEMO_SetMenuPosition(WORD nID);
WORD DEMO_GetMenuPosition(void);
void DEMO_SetSensorFps(BYTE nID);
BYTE DEMO_GetSensorFps(void);
void DEMO_SetSensorFpsToggle(void);
void DEMO_LoadNR3DLowMode(WORD nID);

void Demo_DIS_OnOff(WORD nID);
void Demo_DIS_InitOverScan(BOOL OnOff);

void DEMO_HTMOnOFF(WORD nID);
void  DEMO_HTMCurveOption(WORD nID);

void SaveDefault_AE(void);

//test func
void DEMO_SetTPCtrl(WORD nID);
void	DEMO_SetTPOnOffCtrl(BOOL OnOFF);
void DEMO_SetTPDefault(void);		
void DEMO_SetTPSize(WORD nID);
void DEMO_SetTPPosition(WORD nID);
void DEMO_SetAWBHold(BOOL holdf);
//void DEMO_SDI_OUT_TEST(void);

void DEMO_SetBackLightMode(WORD nID);
BYTE DEMO_GetBackLightMode(void);
void DEMO_BINNING_MODE(WORD nID);
void DEMO_FLKDET_MODE(WORD nID);
BYTE Get_FLKDet_Mode(void);
void DEMO_SetB_BYPASSCtrl(WORD nID);
void DEMO_SetYCConv(WORD nID);
void DEMO_SetOUTCSCDefine(WORD nID);

void DEMO_SetMISPSoftReset(void);		// SoftRST util

void DEMO_SetInputPattern(WORD nID);

void DEMO_SetBNRCtrl(WORD nID);
void DEMO_SetCompressCtrl(WORD nID)	;
void DEMO_SetCompressDefault(WORD nID);
void DEMO_SetCompressMode(WORD nID)	;
void IrCodeDWindowMoveDirL(void);
void IrCodeDWindowMoveDirR(void);
void DEMO_SetCascade(BYTE mode)	;

// exposure
void SetAEInitGainCtrlMode(LONG mode);
void SetAEGainCtrlMode(WORD nID);
void SetAEShutSpeed(WORD nID);
void SetAEIrisGain(WORD nID);
void SetAEAGCGain(WORD nID)	;
void SetAEBrightLvl(WORD val);
void SetAEIrisSpeed(WORD val);
void SetAELensMode(WORD val);
void SetAEFlkMode(WORD val);
BYTE GetAEGainCtrlMode(void);
BYTE GetAEShutSpeed(void);
BYTE GetAEIrisGain(void);
BYTE GetAEAGCGain(void);
BYTE GetAEBrightLvl(void);
BYTE GetAEIrisSpeed(void);
BYTE GetAELensMode(void);
BYTE GetAEFlkMode(void);
void SetAECtrlMode(WORD nID);
void SetGSTriggerStandby(WORD nID);
void SetGSTriggerFrameCounter(void);
void SetGSTriggerPushPulse(void);
void SetGSTriggerOffHandler(void);

BYTE GetAECtrlMode(void);
void SetAEModeDefault(void);
void SetAEIrisBRGain(WORD nID);
BYTE LoadAELensType(void);
void SaveAELensType(BYTE type);
void SaveMenuColor(WORD id, WORD val);
void SaveCamTitleColor(BYTE val);
LONG Get_AE_Ysum(void);
LONG Get_AE_Yref(void);
LONG Get_AE_Ydiff(void);
LONG Get_AE_YDIFF(void);

DWORD Get_AE_Gain(void);
DWORD Get_AE_DGain(void);
SHORT Get_AE_BrightLevel(void);
LONG Get_AE_Mode(void);


void SetAFConferenceMode(WORD nID);
void SetAFConferenceWinWidth(WORD nID);
void SetAFConferenceWinHeight(WORD nID);
void SetAFConferenceWinXPos(WORD nID);
void SetAFConferenceWinYPos(WORD nID);
BYTE GetAFConferenceMode(void);
WORD GetAFConferenceWinWidth(void);
WORD GetAFConferenceWinHeight(void);
WORD GetAFConferenceWinXPos(void);
WORD GetAFConferenceWinYPos(void);

void DEMO_SetLEDPWMLowWidth(BYTE val);
void DEMO_SetLEDPWMHighWidth(BYTE val);
void DEMO_StatusDisplay(BOOL OnOff);
void DEMO_SetCropPosiCenter(BOOL OnOff);

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
void FLK_ProcessHandler(void);
void FLK_ProcessOnOff(BOOL OnOff);
void Demo_SetChromaSuppress(WORD nID);
void Demo_SetDemosaicChromaSuppress(WORD nID);		// demosaic chroma suppress
void Demo_SetRotation(WORD nID);
void DEMO_SetLVDSPLL(WORD nID);
void Demo_Set_OutFHD_Crop(void);
void Demo_Set_OutFHD_Scale(void);
void Demo_Set_OutUHD(void);
#endif
void DEMO_SetSensorWrite(BOOL OnOff);
void DEMO_SetSensorWstop(BOOL OnOff);
void DEMO_fbcap_full(void);
void DEMO_fbcap_area(WORD lx, WORD ly, WORD w, WORD h);
void DEMO_byrcap_area(WORD lx, WORD ly, WORD w, WORD h, PBYTE pR, PBYTE pG, PBYTE pB);
void DEMO_SetCVBSAspectCrop(void);
void DEMO_SetCVBSAspectFull(void);
void DEMO_GetSyncInfo(void);
void DEMO_GetAuxSyncInfo(void);
void DEMO_SetDebugMode(WORD val);
void DEMO_SetAgingTest(WORD val);
void IOExp_ProcessHandler(void);
void DEMO_SetHdmiTxMode(WORD val);
BYTE DEMO_GetHdmiTxMode(void);
void DEMO_SetOSD_Title(BOOL OnOff);
void SetTwoMotorAfMode(WORD nID);
void SetTwoMotorScanMode(WORD nID);
void SetTwoMotorOnePush(void);
void SetTwoMotorSyncTDN(WORD nID);
void SetTwoMotorInitial(void);
BYTE GetTwoMotorAfMode(void);
BYTE GetTwoMotorScanMode(void);
BYTE GetTwoMotorSyncTDN(void);
void SetPwmIrisTune(void);
void ChangeTestMode(WORD nID);


// wdr
extern const DEMO_func DEMO_SetWDR_FUNC[];
PBYTE DEMO_GetWDR_FTBL(void);
BYTE DEMO_GetWDR_FNUM(void);
void DEMO_SetWDR_MODE(WORD nID);
void DEMO_SetWDR_VIEW(WORD nID);	
void DEMO_SetWDR_STRN(WORD nID);
void DEMO_SetWDR_CONT(WORD nID);
void DEMO_SetWDR_AUTO(WORD nID);
void DEMO_SetWDR_GOAL(WORD nID);
void DEMO_SetWDR_GAIN(WORD nID);
void DEMO_SetWDR_IRIS(WORD nID);
void DEMO_SetWDR_SHUT(WORD nID);
void DEMO_SetWDR_SRTO(WORD nID);
void DEMO_SetWDR_MDGR(WORD nID);


#endif	/* __VIDEO_H__ */
