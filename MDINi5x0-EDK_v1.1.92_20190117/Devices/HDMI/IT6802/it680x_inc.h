///*****************************************
//  Copyright (C) 2009-2016
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <it680x.h>
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2016/05/03
//   @fileversion: ITE_MHLRX_SAMPLE_V1.24
//******************************************/

#include	"common.h"


#if defined(__USE_IT6802_RX__)


#ifndef _IT680X_INC_H_
#define _IT680X_INC_H_


#ifndef _IT680x_
#define _IT680x_
#endif


//#ifdef _MCU_
// #if 1
	// #include "IT680x_global.h"
	// #include "Mhlrx.h"
	// #include "Mhlrx_reg.h"
	// #include "IT680x_typedef.h"
	// //    #include "Utility.h"
// #else
    // #define _CODE const
    // #include "common.h"
    // #include "i2c.h"
    // #include "int.h"
    // #include "digrxs.h"
    // #include "rta_tsk.h"
    // #include "rta_sem.h"
    // #include "rta_pub.h"
    // #include "ddp442x_rtos_include.h"
    // #include "sysmon.h"
    // #include "mailbox.h"
    // #include "tmr.h"
    // #include "dbmessage.h"
    // #include "gpiofunction.h"
    // #include "datapath.h"
    // #include "src_3d.h"
    // #include "detect3D.h"
    // #include "defines.h"
    // #include "digcontrol.h"
// #endif
#include "it680x_config.h"
#include "it680x_typedef.h"
#include "it680x_global.h"
#include "it680x_cec.h"
#include "Mhlrx.h"
#include "Mhlrx_reg.h"


//FIX_ID_016 xxxxx Support Dual Pixel Mode for IT6803 Only
#if defined(_IT6803_)
#pragma message ("defined ENABLE_IT6803")
#endif
//FIX_ID_016 xxxxx

/*****************************************************************************/
/* Local Defines    **********************************************************/
/*****************************************************************************/
#define HDMIRX_MAX_KSV 8

//#define DISABLE_HDMI_CSC
#define Enable_Vendor_Specific_packet
//#define EN_DUAL_PIXEL_MODE	//2013-0520


//FIX_ID_003 xxxxx	//Add IT6802 Video Output Configure setting
// 0 eRGB444_SDR=0,
// 1	eYUV444_SDR,
// 2	eRGB444_DDR,
// 3	eYUV444_DDR,
// 4	eYUV422_Emb_Sync_SDR,
// 5	eYUV422_Emb_Sync_DDR,
// 6	eYUV422_Sep_Sync_SDR,
// 7	eYUV422_Sep_Sync_DDR,
// 8	eCCIR656_Emb_Sync_SDR,
// 9	eCCIR656_Emb_Sync_DDR,
// 10 eCCIR656_Sep_Sync_SDR,
// 11 eCCIR656_Sep_Sync_DDR,
// 12 eRGB444_Half_Bus,
// 13 eYUV444_Half_Bus,
// 14 eBTA1004_SDR,
// 15 eBTA1004_DDR
//06-27 disable --> #define HDMIRX_OUTPUT_VID_MODE (F_MODE_EN_UDFILT | F_MODE_RGB444)
#define HDMIRX_OUTPUT_VID_MODE eRGB444_SDR
//FIX_ID_003 xxxxx

#define MS_TimeOut(x) (x+1)

//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
#define VSATE_CONFIRM_SCDT_COUNT	MS_TimeOut(100)
//FIX_ID_033 xxxxx

#ifdef _FIX_ID_028_
//xxxxx 2014-0417
//FIX_ID_028 xxxxx //For Debug Audio error with S2
#define AUDIO_READY_TIMEOUT			MS_TimeOut(0)	// change 100ms to 0 for speed up audio on
//FIX_ID_028 xxxxx
//xxxxx 2014-0417
#else
//FIX_ID_023 xxxxx		//Fixed for Audio Channel Status Error with invalid HDMI source
#define AUDIO_READY_TIMEOUT			MS_TimeOut(200)
//FIX_ID_023 xxxxx
#endif
#define AUDIO_MONITOR_TIMEOUT              MS_TimeOut(150)

#define SCDT_OFF_TIMEOUT              		MS_TimeOut(20)		//100 x MS_LOOP = 5000 ms = 5 sec
#define ECC_TIMEOUT              				MS_TimeOut(20)
#define DESKEW_TIMEOUT            			MS_TimeOut(20)



// Debug Mode
#define EnCBusDbgMode  	FALSE
#define MSCCBusDbgCtrl 	TRUE
#define  DDCCBusDbgCtrl 	FALSE
#define  RCLKFreqSel 	1	//; //0: RING/2; 1: RING/4; 2: RING/8; 3: RING/16
#define GenPktRecType	0x81
#define PPHDCPOpt	TRUE	//2013-0509 MHL 1080p packet pixel mode HDCP

#ifndef IT6811B0
#define PPHDCPOpt2	TRUE	//2013-0509 MHL 1080p packet pixel mode HDCP
#else
#define PPHDCPOpt2	FALSE 	//only for it6811b0
#endif

//FIX_ID_021 xxxxx		//To use CP_100ms for CBus_100ms and CEC_100m
//FIX_ID_004 xxxxx //Add 100ms calibration for Cbus
//#ifdef _SelectExtCrystalForCbus_
#define T10usSrcSel   TRUE	//FALSE: 100ms calibration , TRUR: 27MHz Crystal(only IT6802)
//#else
//#define T10usSrcSel   FALSE	 //FALSE: 100ms calibration , TRUR: 27MHz Crystal(only IT6802)
//#endif
//FIX_ID_004 xxxxx
//FIX_ID_021 xxxxx

#define EnMSCBurstWr	TRUE
#define MSCBurstWrID	TRUE   // TRUE: from MHL5E/MHL5F
#define MSCBurstWrOpt	FALSE  // TRUE: Not write Adopter ID unsigned char o ScratchPad
#define EnPktFIFOBurst	TRUE
// DDC Option
#define EnDDCSendAbort	TRUE  // Send ABORT after segment write with EOF
//CBUS Capability
// FIX_ID_046 MHL version changed from 2.0 -> 2.2
#define MHLVersion	0x22
#define PLIM	1
#define POW	1
#define DEV_TYPE_SINK	1 //06-26
#define DEV_TYPE	1
#define ADOPTER_ID_H	0x02
#define ADOPTER_ID_L	0x45
#define DEVICE_ID_H		0x68
#define DEVICE_ID_L		0x02
#define AckHigh	0xB
#define AckLow	1
// CBUS INput Option
#define EnCBusDeGlitch	TRUE
//---------------------//
//----- WatchDog -----//
//--------------------//
#define DeltaNum 	1
#define RegBurstWrTOSel	2 // 2	//0: 320ms, 1: 340ms, 2: 360ms (ATC)
#define Reg100msTOAdj	2 // 2	//00: 100ms, 01: 99ms, 10: 101ms (ATC)
#define EnMSCHwRty	FALSE
#define EnHWPathEn	FALSE

// FIX_ID_050
// #ifndef _SUPPORT_UCP_
// FIX_ID_057
#ifdef _SUPPORT_UCP_
#define MSCRxUCP2Nack	TRUE
#else
#define MSCRxUCP2Nack	FALSE
#endif
//~FIX_ID_050

/////////////////////////////////////////
//Cbus command fire wait time
//Maxmun time for determin CBUS fail
//	CBUSWAITTIME(ms) x CBUSWAITNUM
/////////////////////////////////////////
//FIX_ID_024 xxxxx	//Fixed for RCP compliance issue
#define CBUSWAITTIME    1
#define CBUSWAITNUM     100
//FIX_ID_024	xxxxx



#define  HDCPIntKey   FALSE   //TRUE: Internal HDCP Key, FALSE: SIPROM

#define  VCLK_INV	0
#define  VCLK_DLY	0
#define  EnMultiSeg     TRUE
#define  EnIntEDID      TRUE

//Discovery
#define  CBUSFloatAdj	FALSE
#define EQFAILCNT 2



//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
#define EQRETRYFAILCNT 1	// for EQ interrupt
#define RCLKVALUE 12			// for show TMDS and Pixel Clk
#define TMDSCLKVALUE 160	// for TMDS > 160 then set RS to 00, otherwise set to 3F

#define TMDSCLKVALUE_1080P 160	// for TMDS > 160 then set RS to 00, otherwise set to 3F
#define TMDSCLKVALUE_480P 35
#define TMDSCLKVALUE_MHL_ER1 90
#define JUDGE_ER1_VALUE 90

//FIX_ID_001 xxxxx


//FIX_ID_021 xxxxx		//To use CP_100ms for CBus_100ms and CEC_100m
//FIX_ID_004 xxxxx 		//Add 100ms calibration for Cbus
//#ifndef _SelectExtCrystalForCbus_
#define _RCLK_FREQ_20M  FALSE
//#endif
//FIX_ID_004 xxxxx
//FIX_ID_021 xxxxx


//FIX_ID_037 xxxxx //Allion MHL compliance issue !!!
//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
//FIX_ID_005 xxxx	//Wait for video on then read MHL device capability
#define MAX_CBUS_WAITNO 		(300/MS_LOOP)		// 250ms
#define MAX_PATHEN_WAITNO 	(700/MS_LOOP)		// 700ms
#define MAX_BUSY_WAITNO 		(2500/MS_LOOP)		// 150ms
#define MAX_DISCOVERY_WAITNO 	(100/MS_LOOP)		// 100ms
//FIX_ID_005 xxxx
//FIX_ID_033 xxxxx
//FIX_ID_037 xxxxx

//FIX_ID_014 xxxx
#define MAX_TMDS_WAITNO 		(350/MS_LOOP)		// 400ms
#define MAX_HDCP_WAITNO 		(100/MS_LOOP)		// 150ms
//FIX_ID_014 xxxx
//FIX_ID_018	xxxxx	//modify 1K pull-down to 1.033K ohm HDMI Reg1C0[3:2]=2
#define RENEW_WAKEUP		(12000/MS_LOOP)
#define IGNORE_WAKEUP		(1000/MS_LOOP)
#define TOGGLE_WAKEUP		(4000/MS_LOOP)
#define CDSENSE_WAKEUP		(500/MS_LOOP)
//FIX_ID_018	xxxxx

//ITE_150302  Chroma A222908 issue
#define CBUS_CLK_MODE_TIMEOUT (3500/MS_LOOP)
//

#define DEFAULT_EQVALUE 0x1F

//FIX_ID_052 DCLK DELAY DIFF FOR CSC ON AND OFF
#define DCLK_DELAY_CSC_ON  0x02
#define DCLK_DELAY_CSC_OFF 0x00
//~FIX_ID_052


//////////////////////////////////////////////////////////////////////////////////////////////
// function
//////////////////////////////////////////////////////////////////////////////////////////////
unsigned char hdmirxrd( unsigned char RegAddr);
unsigned char hdmirxwr( unsigned char RegAddr,unsigned char DataIn);
unsigned char  hdmirxset( unsigned char  offset, unsigned char  mask, unsigned char  ucdata );
void hdmirxbwr( unsigned char offset, unsigned char byteno, unsigned char *rddata );

//FIX_ID_036	xxxxx //Enable MHL Function for IT68XX
#ifdef _ENABLE_IT68XX_MHL_FUNCTION_
unsigned char mhlrxrd( unsigned char offset );
unsigned char mhlrxwr( unsigned char offset, unsigned char ucdata );
unsigned char mhlrxset( unsigned char offset, unsigned char mask, unsigned char ucdata );
void mhlrxbrd( unsigned char offset, unsigned char byteno, unsigned char *rddata );
void mhlrxbwr( unsigned char offset, unsigned char byteno, unsigned char *rddata );
void mhlrx_write_init(struct IT6802_REG_INI _CODE *tdata);
#endif
//FIX_ID_036	xxxxx


SYS_STATUS EDID_RAM_Write(unsigned char offset,unsigned char byteno,unsigned char *p_data );
unsigned char EDID_RAM_Read(unsigned char offset);
/* IT680x Configuration and Initialization ***********************************/
struct it6802_dev_data* get_it6802_dev_data(void);
void hdimrx_write_init(struct IT6802_REG_INI _CODE *tdata);
//FIX_ID_036	xxxxx //Enable MHL Function for IT68XX
//static void mhlrx_write_init(struct IT6802_REG_INI _CODE *tdata);
//FIX_ID_036	xxxxx

//FIX_ID_003 xxxxx	//Add IT6802 Video Output Configure setting
void IT6802_VideoOutputConfigure_Init(struct it6802_dev_data *it6802,Video_Output_Configure eVidOutConfig);
//FIX_ID_003 xxxxx

void hdmirx_Var_init(struct it6802_dev_data *it6802);
void IT6802_Rst( struct it6802_dev_data *it6802 );
//FIX_ID_002 xxxxx 	Check IT6802 chip version Identify for TogglePolarity and Port 1 Deskew
void IT6802_Identify_Chip(void);
//FIX_ID_002 xxxxx
//void IT6802_fsm_init(void);
//FIX_ID_016 xxxxx Support Dual Pixel Mode for IT6803 Only
#if defined(_IT6803_)
void IT6803OutputPixelModeSet(unsigned char bSignalPixelMode);
void IT6803JudgeOutputMode(void);
#endif
//FIX_ID_016 xxxxx

/* HDMI RX functions   *********************************************************/
void chgbank(int bank);
unsigned char CheckSCDT(struct it6802_dev_data *it6802);
void WaitingForSCDT(struct it6802_dev_data *it6802);
unsigned char CLKCheck(unsigned char ucPortSel);

//FIX_ID_009 xxxxx	//verify interrupt event with reg51[0] select port
unsigned char  IT6802_IsSelectedPort(unsigned char ucPortSel);
//FIX_ID_009 xxxxx

//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
#ifdef _SUPPORT_EQ_ADJUST_
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void HDMIStartEQDetect(struct it6802_eq_data *ucEQPort);
void HDMISetEQValue(struct it6802_eq_data *ucEQPort,unsigned char ucIndex);
void HDMISwitchEQstate(struct it6802_eq_data *ucEQPort,unsigned char state);
void HDMICheckSCDTon(struct it6802_eq_data *ucEQPort);
void HDMIPollingErrorCount(struct it6802_eq_data *ucEQPort);
void HDMIJudgeECCvalue(struct it6802_eq_data *ucEQPort);
void HDMIAdjustEQ(struct it6802_eq_data *ucEQPort);
//FIX_ID_010 xxxxx 	//Add JudgeBestEQ to avoid wrong EQ setting
void JudgeBestEQ(struct it6802_eq_data *ucEQPort);
void StoreEccCount(struct it6802_eq_data *ucEQPort);
//FIX_ID_010 xxxxx
void IT6802VideoCountClr(void);
//-------------------------------------------------------------------------------------------------------
#endif

#ifdef _SUPPORT_AUTO_EQ_
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DisableOverWriteRS (unsigned char ucPortSel);
void AmpValidCheck (unsigned char ucPortSel);
void TogglePolarity (unsigned char ucPortSel);
void TMDSCheck(unsigned char ucPortSel);
void OverWriteAmpValue2EQ (unsigned char ucPortSel);
//-------------------------------------------------------------------------------------------------------
#endif
//FIX_ID_001 xxxxx
unsigned char CheckAVMute(void);
//FIX_ID_037 xxxxx //Allion MHL compliance issue !!!
unsigned char CheckPlg5VPwr(unsigned char ucPortSel);
//FIX_ID_037 xxxxx
unsigned char IsVideoOn(void);
unsigned char IsHDMIMode(void);
void GetAVIInfoFrame(struct it6802_dev_data *it6802);
void SetVideoInputFormatWithInfoFrame(struct it6802_dev_data *it6802);
void SetColorimetryByInfoFrame(struct it6802_dev_data *it6802);
//static void SetCSCBYPASS(struct it6802_dev_data *it6802);
void SetColorSpaceConvert(struct it6802_dev_data *it6802);
void SetNewInfoVideoOutput(struct it6802_dev_data *it6802);
void SetVideoInputFormatWithoutInfoFrame(struct it6802_dev_data *it6802,unsigned char bInMode);
void SetColorimetryByMode(struct it6802_dev_data *it6802);
void SetDVIVideoOutput(struct it6802_dev_data *it6802);

//FIX_ID_003 xxxxx	//Add IT6802 Video Output Configure setting
void IT6802_VideoOutputModeSet(struct it6802_dev_data *it6802);
//FIX_ID_003 xxxxx

void IT6802VideoOutputConfigure(struct it6802_dev_data *it6802);
void SetVideoOutputColorFormat(struct it6802_dev_data *it6802);
//void it6802PortSelect(unsigned char ucPortSel);
void IT6802_HalfPCLKFinetune(struct it6802_dev_data *it6802); //ITE_150409
void IT680x_StandbyMode(unsigned char ucstate);//ITE_151021

void hdmirx_ECCTimingOut(unsigned char ucport);

/* HDMI Audio function    *********************************************************/
void aud_fiforst( void );
void IT6802AudioOutputEnable(unsigned char bEnable);
void hdmirx_ResetAudio(void);
void hdmirx_SetHWMuteClrMode(void);
void hdmirx_SetHWMuteClr(void);
void hdmirx_ClearHWMuteClr(void);
void getHDMIRXInputAudio(AUDIO_CAPS *pAudioCaps);
void IT6802SwitchAudioState(struct it6802_dev_data *it6802,Audio_State_Type state);
#ifdef _FIX_ID_028_
//FIX_ID_028 xxxxx //For Debug Audio error with S2
// remove --> static void IT6802AudioHandler(struct it6802_dev_data *it6802);
//FIX_ID_028 xxxxx
#else
void IT6802AudioHandler(struct it6802_dev_data *it6802);
#endif

#ifdef EnableCalFs
//FIX_ID_023 xxxxx		//Fixed for Audio Channel Status Error with invalid HDMI source
void AudioFsCal(void);
unsigned int PCLKGet(void);
void TMDSGet(void);
void DumpNCTSReg(void);
//FIX_ID_023 xxxxx
#endif

/* HDMI Video function    *********************************************************/
void IT6802_AFE_Rst( void );

//FIX_ID_037 xxxxx //Allion MHL compliance issue !!!
//xxxxx 2014-0529 //Content On/Off
void IT6802_HDCP_ContentOff(unsigned char ucPort , unsigned char bOff);
void IT6802_RAPContentOff(unsigned char bOff);
//xxxxx 2014-0529
//FIX_ID_037 xxxxx

void IT6802_SetVideoMute(struct it6802_dev_data *it6802,unsigned char bMute);
//static void IT6802VideoOutputCDSet(void);
void IT6802VideoOutputEnable(unsigned char bEnableOutput);
void IT6802VideoCountClr(void);
void IT6802SwitchVideoState(struct it6802_dev_data *it6802,Video_State_Type  eNewVState);
void IT6802VideoHandler(struct it6802_dev_data *it6802);
void IT680x_StandbyMode(unsigned char ucstate);//ITE_150408


/* HDMI Interrupt function    *********************************************************/
void hdmirx_INT_5V_Pwr_Chg(struct it6802_dev_data *it6802,unsigned char ucport);
void hdmirx_INT_P0_ECC(struct it6802_dev_data *it6802);
void hdmirx_INT_P1_ECC(struct it6802_dev_data *it6802);
void hdmirx_INT_P0_Deskew(struct it6802_dev_data *it6802);
void hdmirx_INT_P1_Deskew(struct it6802_dev_data *it6802);
//FIX_ID_009 xxxxx	//verify interrupt event with reg51[0] select port
void hdmirx_INT_HDMIMode_Chg(struct it6802_dev_data *it6802,unsigned char ucport);
//FIX_ID_009 xxxxx
void hdmirx_INT_SCDT_Chg(struct it6802_dev_data *it6802);

//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
#ifdef _SUPPORT_AUTO_EQ_
void hdmirx_INT_EQ_FAIL(struct it6802_dev_data *it6802,unsigned char ucPortSel);
#endif
//FIX_ID_001 xxxxx


/* MHL HDCP functions    *********************************************************/
// disable -> static void hdcpsts( void );

/* MHLRX functions    *********************************************************/
//FIX_ID_036	xxxxx //Enable MHL Function for IT68XX
#ifdef _ENABLE_IT68XX_MHL_FUNCTION_

#ifdef _SUPPORT_RCP_
void parse_rcpkey( unsigned char rcpkey );
void mhl_parse_RCPkey(struct it6802_dev_data *it6802);
#endif

#ifdef _SUPPORT_RAP_
void mhl_parse_RAPkey(struct it6802_dev_data *it6802);
#endif

#ifdef _SUPPORT_RAP_
void ucp_report_event( unsigned char key);
void mhl_parse_UCPkey(struct it6802_dev_data *it6802);
#endif

void mhl_read_mscmsg( struct it6802_dev_data *it6802 );
int mscWait( void );
int mscFire( int offset, int wdata );
int cbus_send_mscmsg( struct it6802_dev_data *it6802 );

/*  MHL interrupt    *******************************************************/
void parse_devcap(unsigned char *devcap );
int read_devcap_hw( struct it6802_dev_data *it6802 );
int set_mhlint( unsigned char offset, unsigned char field );
int set_mhlsts( unsigned char offset, unsigned char status );
void write_burst(struct it6802_dev_data *it6802, int offset, int byteno );
void set_wrburst_data(struct it6802_dev_data *it6802, int offset, int byteno );

/* MHL 3D functions    *******************************************************/
//static void v3d_burst1st(struct it6802_dev_data *it6802);
//static void v3d_burst2nd(struct it6802_dev_data *it6802);
//static void v3d_burst3rd(struct it6802_dev_data *it6802);
//static void v3d_burst4th(struct it6802_dev_data *it6802);
//static void v3d_unsupport1st(struct it6802_dev_data *it6802);
//static void v3d_unsupport2nd(struct it6802_dev_data *it6802);

#ifdef FIX_ID_013_
//FIX_ID_013	xxxxx	//For MSC 3D request issue
#ifdef  _SUPPORT_EDID_RAM_
struct PARSE3D_STR* get_EDID_VSDB_3Ddata(void);
void EDID_ParseVSDB_3Dblock(struct PARSE3D_STR *pstParse3D);
#endif
void Msc_3DProcess(MHL3D_STATE *e3DReqState);
MHL3D_STATE MSC_3DInforSend(unsigned char b3dDtd);
unsigned char Msc_WriteBurstDataFill(unsigned char ucOffset, unsigned char ucByteNo, unsigned char *pucData);
//FIX_ID_013	xxxxx
#else
void v3d_unsupport1st(struct it6802_dev_data *it6802);
void v3d_unsupport2nd(struct it6802_dev_data *it6802);
#endif //FIX_ID_013



#if 1
//FIX_ID_021 xxxxx		//To use CP_100ms for CBus_100ms and CEC_100m
//FIX_ID_004 xxxxx 		//Add 100ms calibration for Cbus
//#ifndef _SelectExtCrystalForCbus_
//static unsigned long m_ROSCCLK;
// don't use floating calculation on firmware.
// static float m_ROSCCLK;
void Cal_oclk( void );
//#endif
//FIX_ID_004
#define CP_MEAN_VALUE		(48696)
#define CP_MAX_DEVIATION	(CP_MEAN_VALUE*15/100)
//#define CP_DEFAULT_VALUE	(CP_MEAN_VALUE*100)
unsigned char OSCvalueCompare(unsigned long *calibrationValue);
unsigned long CP_OCLK( void );
//FIX_ID_021 xxxxx
#endif



/* RCP functions    *******************************************************/
#ifdef _SUPPORT_RCP_
void RCPinitQ(struct it6802_dev_data *it6802);
int RCPKeyPop(struct it6802_dev_data *it6802);
void SwitchRCPResult(struct it6802_dev_data *it6802,RCPResult_Type RCPResult);
void SwitchRCPStatus(struct it6802_dev_data *it6802,RCPState_Type RCPState);
void RCPManager(struct it6802_dev_data *it6802);

//FIX_ID_005 xxxxx	//Add Cbus Event Handler
void IT6802CbusEventManager(struct it6802_dev_data *it6802);
//FIX_ID_005 xxxxx


//FIX_ID_014 xxxxx
void IT6802HDMIEventManager(struct it6802_dev_data *it6802);
//FIX_ID_014 xxxxx

//void RCPKeyPush(unsigned char ucKey);

//FIX_ID_018	xxxxx	//modify 1K pull-down to 1.033K ohm HDMI Reg1C0[3:2]=2
void IT6802_WakeupProcess(void);
//FIX_ID_018 xxxxx



//FIX_ID_024 xxxxx		//Fixed for RCP compliance issue
void WaitRCPresponse(struct it6802_dev_data *it6802);
//FIX_ID_024 xxxxx
#endif

/* Driver State Machine Process **********************************************/
void IT6802MHLInterruptHandler(struct it6802_dev_data *it6802);

#endif
//FIX_ID_036	xxxxx

/* EDID RAM  functions    *******************************************************/
#ifdef _SUPPORT_EDID_RAM_
//static unsigned char UpdateEDIDRAM(_CODE unsigned char *pEDID,unsigned char BlockNUM);
unsigned char UpdateEDIDRAM(unsigned char *pEDID,unsigned char BlockNUM);
void EnableEDIDupdate(void);
void DisableEDIDupdate(void);
//static void EDIDRAMInitial(_CODE unsigned char *pIT6802EDID);
void EDIDRAMInitial(unsigned char *pIT6802EDID);
//static unsigned char Find_Phyaddress_Location(_CODE unsigned char *pEDID,unsigned char Block_Number);
unsigned char Find_Phyaddress_Location(unsigned char *pEDID,unsigned char Block_Number);
void UpdateEDIDReg(unsigned char u8_VSDB_Addr, unsigned char CEC_AB, unsigned char CEC_CD, unsigned char Block1_CheckSum);
void PhyAdrSet(void);
#endif






/* Driver State Machine Process **********************************************/
//FIX_ID_036	xxxxx //Enable MHL Function for IT68XX
//static void IT6802MHLInterruptHandler(struct it6802_dev_data *it6802);
//FIX_ID_036	xxxxx
void IT6802HDMIInterruptHandler(struct it6802_dev_data *it6802);
//void IT6802_fsm(void);


#ifndef Enable_IR
void it6802AutoPortSelect(struct it6802_dev_data *it6802);
#endif
// disable -> static void it6802ShowErrorCount(void);
//void show_vid_info( void );
//void get_vid_info( void );

#ifdef Enable_Vendor_Specific_packet
void Dump3DReg(void);
unsigned char IT6802_DE3DFrame(unsigned char ena_de3d);
#endif

#ifdef 	_SUPPORT_HDMI_REPEATER_
	#include "it680x_repeater.h"
#endif


#endif // _IT680X_INC_H_


#endif	// __USE_IT6802_RX__

