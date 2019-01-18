//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	IR.C
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

#define		IRCODE_CUSTOM		0xfe01  //NEC
//#define		IRCODE_CUSTOM		0xef10  //FINE-HD

#define		IRCODE_POWER				0x55aa
#define		IRCODE_STDBY				0x59a6

#define		IRCODE_ONOFF				0x50AF
#define		IRCODE_CH					0x51AE
#define		IRCODE_SWAP					0x52AD
#define		IRCODE_POS					0x53AC

#define		IRCODE_NUM_1				0x02fd
#define		IRCODE_NUM_2				0x03fc
#define		IRCODE_NUM_3				0x04fb
#define		IRCODE_NUM_4				0x05fa
#define		IRCODE_NUM_5				0x06f9
#define		IRCODE_NUM_6				0x07f8
#define		IRCODE_NUM_7				0x08f7
#define		IRCODE_NUM_8				0x09f6
#define		IRCODE_NUM_9				0x0af5
#define		IRCODE_NUM_0				0x0bf4
#define		IRCODE_CC					0x45ba
#define		IRCODE_TTX					0x42bd

#define		IRCODE_VOL_UP				0x5fa0
#define		IRCODE_VOL_DN				0x5ba4
#define		IRCODE_RADIO				0x54ab
#define		IRCODE_MUTE					0x0df2
#define		IRCODE_ADDEL				0x0ff0
#define		IRCODE_FAV_UP				0x57a8
#define		IRCODE_FAV_DN				0x47b8

#define		IRCODE_ASPECT				0x18e7
#define		IRCODE_RECALL				0x0ef1
#define		IRCODE_LIST					0x01fe
#define		IRCODE_AUDIO				0x46b9

#define		IRCODE_DIR_UP				0x11ee
#define		IRCODE_DIR_DN				0x15ea
#define		IRCODE_DIR_LT				0x12ed
#define		IRCODE_DIR_RT				0x14eb
#define		IRCODE_ENTER				0x13ec

#define		IRCODE_MENU					0x16e9
#define		IRCODE_EXIT					0x17e8
#define		IRCODE_INFO					0x1de2

#define		IRCODE_PVR_REW				0x4cb3 
#define		IRCODE_PVR_PLAY				0x48b7 
#define		IRCODE_PVR_FF				0x4db2 
#define		IRCODE_PVR_START			0x4eb1 
#define		IRCODE_PVR_PAUSE			0x49b6 
#define		IRCODE_PVR_END				0x4fb0 
#define		IRCODE_PVR_STOP				0x4ab5 
#define		IRCODE_PVR_TSHIFT			0x58a7 
#define		IRCODE_PVR_REC				0x4bb4 

#define		IRCODE_STATUS				0x44bb
#define		IRCODE_FREEZE				0x1ae5
#define		IRCODE_PVR_MARK				0x5aa5 
#define		IRCODE_PVR_PLIST			0x5ca3 
#define		IRCODE_VOUT_L				0x1ee1
#define		IRCODE_VOUT_R				0x41be
#define		IRCODE_FORMAT				0x1be4
#define		IRCODE_PVR_TREC				0x56a9 

//RHD-2000 remocon
#define		IRCODE_RED					0x50af 
#define		IRCODE_GREEN				0x51ae 
#define		IRCODE_YELLOW				0x52ad 
#define		IRCODE_BLUE					0x53ac 
#define 	IRCODE_STAR					0x0cf3
#define 	IRCODE_SHARP				0x1ce3
#define 	IRCODE_SUB					0x10ef
#define 	IRCODE_SLEEP				0x19e6
#define 	IRCODE_POWER_2000			0x00ff
#define 	IRCODE_VOL_UP_2000			0x20df
#define 	IRCODE_VOL_DN_2000			0x21de
#define 	IRCODE_FAV_UP_2000			0x22dd
#define 	IRCODE_FAV_DN_2000			0x23dc
#define 	IRCODE_AUDIO_2000			0x1fe0

//SSW capture (no remote con)
#define		IRCODE_SSW_ON_OFF			0xe01f
#define		IRCODE_SSW_STOP				0xe11e

#define		IRCODE_WB_POLY_POS			0xe21d


//Panorama Control (no remote con)
#define		__MDIN_APP_PANO_
#if defined(__MDIN_APP_PANO_)		

	#define		IRCODE_PANO_CRP_LX_U		0xa05f
	#define		IRCODE_PANO_CRP_LX_D		0xa15e
	#define		IRCODE_PANO_CRP_RX_U		0xa25d
	#define		IRCODE_PANO_CRP_RX_D		0xa35c
	#define		IRCODE_PANO_CRP_TY_U		0xa45b
	#define		IRCODE_PANO_CRP_TY_D		0xa55a
	#define		IRCODE_PANO_CRP_BY_U		0xa659
	#define		IRCODE_PANO_CRP_BY_D		0xa758

	#define		IRCODE_PANO_SS0				0xa857
	#define		IRCODE_PANO_SS1				0xa956
	#define		IRCODE_PANO_SS2				0xaa55
	#define		IRCODE_PANO_SS3				0xab54
	#define		IRCODE_PANO_BLEND			0xac53

	#define		IRCODE_PANO_EEPROM_SAVE		0xad52
	#define		IRCODE_PANO_EEPROM_DEL		0xae51

	#define		IRCODE_PANO_DISP_MODE		0xb04f
	#define		IRCODE_PANO_BLEND_AREA		0xb14e
	#define		IRCODE_PANO_BLEND_LEVEL		0xb24d
#endif


// new sticker version remocon
#define		IRCODE_AEONOFF				IRCODE_POWER
#define		IRCODE_FOCUS				IRCODE_RADIO
#define		IRCODE_NR					IRCODE_NUM_1
#define		IRCODE_EDGE					IRCODE_NUM_2
#define		IRCODE_AF_MODE				IRCODE_NUM_3
#define		IRCODE_NR_DEMO				IRCODE_NUM_4
#define		IRCODE_DEFOG				IRCODE_NUM_5
#define		IRCODE_DIS					IRCODE_NUM_6
#define		IRCODE_WDR					IRCODE_NUM_7
#define		IRCODE_DWDR					IRCODE_NUM_8
#define		IRCODE_AE_BRIGHT			IRCODE_NUM_9
#define		IRCODE_AE_MODE				IRCODE_NUM_0
#define		IRCODE_DIGZOOM				IRCODE_STAR
#define		IRCODE_AE_DARK				IRCODE_SHARP
#define		IRCODE_DZOOM_IN				IRCODE_VOL_UP
#define		IRCODE_DZOOM_OUT			IRCODE_VOL_DN
#define		IRCODE_FOCUS_NEAR			IRCODE_MUTE
#define		IRCODE_FOCUS_FAR			IRCODE_ADDEL
#define		IRCODE_ZOOM_TELE			IRCODE_FAV_UP
#define		IRCODE_ZOOM_WIDE			IRCODE_FAV_DN
#define		IRCODE_AF_SETUP				IRCODE_STATUS
#define		IRCODE_AF_START				IRCODE_CC
#define		IRCODE_AF_END				IRCODE_TTX
#define	 	IRCODE_AWB_SETUP			IRCODE_SUB
#define		IRCODE_AWB_START			IRCODE_RECALL
#define		IRCODE_AWB_MODE				IRCODE_AUDIO_2000
#define		IRCODE_FREEZEMIRR			IRCODE_FREEZE
#define		IRCODE_PMMD					IRCODE_ASPECT
#define		IRCODE_VID_RES				IRCODE_FORMAT
#define	 	IRCODE_3A_WIN				IRCODE_SLEEP
#define		IRCODE_FACTORY_L			IRCODE_VOUT_L
#define		IRCODE_FACTORY_R			IRCODE_VOUT_R
#define		IRCODE_PIPONOFF				IRCODE_RED
#define		IRCODE_PIPSEL				IRCODE_GREEN
#define		IRCODE_PIPSIZE				IRCODE_YELLOW
#define		IRCODE_PIPPOS				IRCODE_BLUE


// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static volatile CHAR IrPulseCount, IrNothingCount;
static volatile WORD IrComCommand, IrCustomCode, IrDataCode;
static volatile BOOL fIrCodeCmdParsed, fIrCodeCompleted, fIrContinueCheck;
static volatile BOOL fIrCodeUARTprintf;

static volatile WORD DBG_IrCount, DBG_IrPulse[128];

static WORD Defog_DEMOview=0;
static BOOL DemoPIPCtrlOnOff=OFF;
static WORD PipPosPreSet=0;
static BOOL WB_HOLD=OFF;
//static BOOL tTestFlag = FALSE;
static BOOL tNewFlag = FALSE;

#if defined ( __USE_CMOS_MT9P031__ )
static BOOL Turn5Mega=OFF;
static BOOL Turn2MegaPan=OFF;
#endif

#define AF_NEW_FILE

#if defined(__USE_PWMIRIS_ONLY__)
static int keyRL=0,keyUD=0;
#endif

/*
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
static SHORT z_WID = 0, zGoalW[5] = {   0,  200,  400,  600,  800};
static SHORT z_TID = 0, zGoalT[5] = {2195, 2315, 2435, 2555, 2675};
#endif
*/

static BYTE CascadeMode=0;
BOOL fDGen = FALSE;
BOOL f_dGain = FALSE;
BOOL fSqr = FALSE;
BYTE fAECtl = 0;
LONG fGainSet = 0;

static BOOL DemoStatusOSD=0;
//static BOOL DemoLensType=0, ICR_OnOff=0;
BYTE DemoDWDROnOff=0, DemoPmMdOnOff=0, DemoFreezeOnOff=0, DemoSysInfo=0, DemoFreezeMirrOnOff=0;
//static BOOL DemoBWExtOnOff=0;

static BOOL SSWCaptureOnOff=OFF;
static BOOL SSWStopOnOff=OFF;

#if 0
static WORD IRIS_VAL=0;
#endif
static BYTE FResetCnt=0;
static BYTE VoutFormatCnt=0;
static BYTE DebugMenuCnt=0;

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
//static BYTE ChromaSup_lev=0;
//static BOOL Demosaic_mode=0;
#endif

#if defined(__USE_CMOS_OV2710__)    // mdin-i51x
static BOOL OV2710_INNER_AE=OFF;
#endif

WORD tick, lead, repeat, data_0, data_1;
WORD tol_lead, tol_repeat, tol_data_0, tol_data_1;

#if defined(__USE_IRKEY_SWITCH__)
BYTE IR_KEY_ENABLE=0;
BYTE IR_KEY_IRQ_CNT=0;
#else
BYTE IR_KEY_ENABLE=1;
BYTE IR_KEY_IRQ_CNT=0;
#endif

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
extern BOOL MainSrc, PIPSrc;
extern BOOL AutoConSat_OnOff;
extern AE_CTRL_PARAM stAECTRL;

#if defined(__SENSOR_ERR_CHECK__)
extern BOOL sensor_err_en;
extern WORD pre_sensor_herr, pre_sensor_verr;
#endif

#if defined ( __USE_CMOS_MT9P031__ )
extern WORD AptinaPosH;
extern WORD AptinaPosV;
#endif

#if defined(__USE_IRDATA_SLAVE2MASER__) && defined(__USE_CPU_SPI_SLAVE__)
WORD	gIrComCommand = 0xFF; 
BOOL	gIRComCommand_on = FALSE;
#endif

extern BYTE GS_Sensor_Status;
// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------
static void IrCode_DirUControl(void);		 //by hungry 2013.08.02	gcc bug fix
static void IrCode_DirDControl(void);
static void IrCode_DirRControl(void);
static void IrCode_DirLControl(void);
#if defined (__USE_CMOS_MT9P031__)
static void IrCode2MegaMoveDirU(void);
static void IrCode2MegaMoveDirD(void);
static void IrCode2MegaMoveDirL(void);
static void IrCode2MegaMoveDirR(void);
#endif

//--------------------------------------------------------------------------------------------------------------------------
void Call_IrkeyDispOff(void)
{
	OSD_IrkeyTitleDisplay(OFF, NULL);
}
//--------------------------------------------------------------------------------------------------------------------------
void IrCodeDebugHandler(void)
{
	if (fIrCodeUARTprintf==FALSE) return;
	
	fIrCodeUARTprintf = FALSE;
	UARTprintf("IR code = 0x%04X\r\n", IrComCommand);
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM IrReceivePulseDecode(void)
{
	WORD count = *R_TM3CNT;
	
	*R_TP3CTRL = F_TPCTRL_CNTCLR_SET;					// clear Timer pre-scale reg.	//for lucida
	*R_TP3CTRL = F_TPCTRL_CNTCLR_CLR;
	*R_TM3CTRL |= F_TMCTRL_TMEN_EN;
	
//	if (DBG_IrCount<128) DBG_IrPulse[DBG_IrCount++] = count;

	if (IrPulseCount==0) {
		if (count>=(lead-tol_lead)&&count<=(lead+tol_lead));				// lead pulse
		else if (fIrContinueCheck==FALSE)
				IrPulseCount = (CHAR)-1;
		else if (count>=(repeat-tol_repeat)&&count<=(repeat+tol_repeat))	// repeat
				fIrCodeCompleted = 1;			// resolve code
		else	IrPulseCount = (CHAR)-1;
	}
	else if (count>=(data_0-tol_data_0)&&count<=(data_0+tol_data_0)) {		// "0" data
		if (IrPulseCount<=16) IrCustomCode<<=1;
//		else if (IrCustomCode==0xf30c) IrDataCode<<=1;	// for MyHD
		else if (IrCustomCode==IRCODE_CUSTOM) IrDataCode>>=1;	// for NEC
		else	IrPulseCount = (CHAR)-1;
	}
	else if (count>=(data_1-tol_data_1)&&count<=(data_1+tol_data_1)) {		// "1" data
		if (IrPulseCount<=16) IrCustomCode<<=1;
//		else if (IrCustomCode==0xf30c) IrDataCode<<=1;	// for MyHD
		else if (IrCustomCode==IRCODE_CUSTOM) IrDataCode>>=1;	// for NEC
		else	IrPulseCount = (CHAR)-1;

		if (IrPulseCount<=16) 		   {IrCustomCode &= 0xffff;	IrCustomCode++;}	// for 4byte
//		else if (IrCustomCode==0xf30c) IrDataCode++;	// for MyHD
		else						{IrDataCode &= 0xffff;	IrDataCode|=0x8000;}	// for NEC	// for 4byte
	}
	else	IrPulseCount = (CHAR)-1;

	if (IrPulseCount==32) fIrCodeCompleted = 1;	// resolve code

	if (IrPulseCount<32) IrPulseCount++;
	else				 IrPulseCount=0;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM IrCodeCommandParse(void)
{
	if (!fIrCodeCompleted) return;
	fIrCodeCompleted = FALSE;	IrNothingCount = 10;	// Time-out is 140ms

	fIrContinueCheck = TRUE;	fIrCodeCmdParsed = TRUE;
	IrComCommand = IrDataCode;	fIrCodeUARTprintf = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM IrCodeTimeOutCounter(void)
{
	if (IrNothingCount) IrNothingCount--;
	if (IrNothingCount) return;

	SetMsgIrCodeNewCmd(0);
	fIrContinueCheck = FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM IrCodePreemptMsgHooking(void)
{
	return (IsOtherMsgPreemptive(MSG_IRCODE))? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM IsIrCodeMsgHooking(void)
{
	if (IrCodePreemptMsgHooking()) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM IrCodeMessageClassify(void)
{
	if (fIrCodeCmdParsed==FALSE) return;
	fIrCodeCmdParsed = FALSE;

	if (IsIrCodeMsgHooking()) return;

	SetMsgIrCodeCtrlMode();
	SetMsgIrCodeNewCmd(IrComCommand);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeWBCtrlOnOff(void)
{
	WB_HOLD^=ON;
	
	if(WB_HOLD)	OSD_IrkeyTitleDisplay(ON, "WB_STOP   ");		 
	else		OSD_IrkeyTitleDisplay(OFF, "WB_RUN    ");			

	SetWBCtrlOnOff();
	
}

//--------------------------------------------------------------------------------------------------------------------------
#if  defined(__USE_PWMIRIS_ONLY__)
extern BOOL initIRIS;
#endif
static void IrCodeAECtrlOnOff(void)
{
#if  defined(__USE_PWMIRIS_ONLY__)
	if(initIRIS == TRUE)	initIRIS = FALSE;
	else	initIRIS = TRUE;
	return;
#endif

	LONG mode = stAECTRL.ctrl&AECTRL_PROCESS_OFF;

	if (mode==AECTRL_PROCESS_OFF)	OSD_IrkeyTitleDisplay(OFF, "AE_RUN    ");
	else								OSD_IrkeyTitleDisplay(ON, "AE_HOLD   ");

	SetAECtrlOnOffToggle();

}

void UARTgets(PBYTE rStr, BYTE len)
{
	BYTE buf[10], rbuff[100];
	BYTE i,rx_cnt=0, loop=1;
	PBYTE ptr = rbuff;
	
	while(loop)
	{
		rx_cnt=uart_getdata(1, buf, 9);
		
		if(rx_cnt != 0)
		{
			for(i=0;i<rx_cnt;i++)
			{
				*ptr = buf[i];	ptr++;
				if(buf[i] == 0x0d || buf[i] == 0x0a || buf[i] == 0x1b)	loop=0;
			}
		}
	}
	
	for(i=0;i<len;i++)	rStr[i] = rbuff[i];
	rStr[i] = 0;
//	UARTprintf("rStr[%d] : ", len);
//	for(i=0;i<len;i++)	UARTprintf("%x ", rStr[i]);
//	UARTprintf("\r\n");
}

/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeCSCCoefSRC(void)
{
	SetCSCCoefSRC();
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeCSCCoefOUT(void)
{
	SetCSCCoefOUT();
}
*/

//--------------------------------------------------------------------------------------------------------------------------
#if defined(__USE_AFZOOM_LENS__)
static int AF_Range_Val[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static int AF_Cur = 0;
static int first_AF_RV = 0;

static void IrCodeExit(void)
{
	// BYTE retM;

	if(MenuMessageExit()) return; // exit MENU

	int i;

	#if defined(AF_NEW_FILE)
	AF_SetTrMode(0);
	//SetDrvOffPINT();
	AFNORM_ChangeAFCtlVal(2,0,0);
	#endif
	first_AF_RV = 0;
	for(i=0;i<sizeof(AF_Range_Val);i++) AF_Range_Val[i]=0;
	
	ODM_SetDEBUGMode(0); // off debug mode
	OSD_IrkeyTitleDisplay(OFF, "          "); // clear Key osd

	WORD buf;
	MISP_RegRead(MISP_LOCAL_ID, 0x752, &buf);
	if(buf != 0)
		MISP_RegWrite(MISP_LOCAL_ID, 0x752, 0); // clear 3A windows display
	else
		MISP_RegWrite(MISP_LOCAL_ID, 0x752, 0x1f); // set all 3A windows display

	//SetAEDBGCleanAll();
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
static WORD pwmCnt = 0;
 void PWM_Test()
{ //IRCODE_AE_MODE
#if defined(__USE_PWMIRIS_ONLY__)
#else
	WORD minVal;

	pwmCnt ++;
	if(pwmCnt > 19) pwmCnt = 0;

	minVal = pwmCnt*(GetSS_image_active_size_h()/20);
	MISP_RegWrite(MISP_LOCAL_ID, 0x67F, 1);
	MISP_RegWrite(MISP_LOCAL_ID, 0x67E,minVal);
	UARTprintf("pwm test...minVal:%d\r\n",minVal);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void AF_Range_Test(BYTE mod)
{
#if defined(__USE_PWMIRIS_ONLY__)
	switch (mod)
	{
		case 0 : if(keyRL < 9) keyRL++; break; // R
		case 1 : if(keyRL > -9) keyRL--; break;  // L
		case 2 : if(keyUD < 400) keyUD++; break;  // U
		case 3 : if(keyUD > -400) keyUD--; break;  // D
	}

	PWM_Test();
#elif defined(__USE_AFZOOM_LENS__)
	char afCh[11];
	int i;

	if(mod==0) {
		if(AF_Cur<(sizeof(AF_Range_Val)-1)) AF_Cur++;
		else return;
	} else if(mod == 1) {
		if(AF_Cur>0) AF_Cur--;
		else return;
	}	

	if(mod==2) {
		if(AF_Range_Val[AF_Cur]<9) AF_Range_Val[AF_Cur]++;
		else return;
	} else if(mod == 3) {
		if(AF_Range_Val[AF_Cur]>-9) AF_Range_Val[AF_Cur]--;
		else return;
	}	
	sprintf(afCh,"%1d%1d%2d%2d%2d%2d",AF_Cur/4,AF_Cur%4,AF_Range_Val[(AF_Cur/4)*4+0],AF_Range_Val[(AF_Cur/4)*4+1],AF_Range_Val[(AF_Cur/4)*4+2],AF_Range_Val[(AF_Cur/4)*4+3]);
	OSD_IrkeyTitleDisplay(ON,afCh);
	UARTprintf("AF_Range_Val:");
	for(i=0;i<sizeof(AF_Range_Val);i++) {
		if(i%4 == 0) UARTprintf("/");
		UARTprintf("%d ",AF_Range_Val[i]);
	}
	UARTprintf("\r\n\r\n");

	#if defined(AF_NEW_FILE)
	AFNORM_ChangeAFCtlVal(first_AF_RV,AF_Cur,AF_Range_Val[AF_Cur]);
	#endif
	first_AF_RV = 1;
#endif
}
/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeICROnOff(void)
{
	if(ICR_OnOff)		{ICR_OnOff=OFF;	SetDYNT_ICR(OFF);}	// icr off(day)
	else				{ICR_OnOff=ON;	SetDYNT_ICR(ON);}	// icr on(night)
}
*/
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeAFProcessON(void)
{
	char posC[11];
	//if ((stAFCTRL.ctrl&AFCTRL_AFAUTO_MASK)!=AFCTRL_MANUAL_MODE) return;
	AF_EnableProcess(ON);
	sprintf(posC, "FOKS:%5d", LENS_GetRelPositionFOKS());
	OSD_IrkeyTitleDisplay(ON, posC);

}

/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeASProcessON(void)
{
	AFSPOT_EnableProcess(ON);
}
*/

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeFOKSDrvDirF(void)
{
	char posC[11];

	SetDirDriveFOKS(fFAR);
	sprintf(posC, "FOKS:%5d", LENS_GetRelPositionFOKS());
	OSD_IrkeyTitleDisplay(ON, posC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeFOKSDrvDirN(void)
{
	char posC[11];

	SetDirDriveFOKS(fNEAR);
	sprintf(posC, "FOKS:%5d", LENS_GetRelPositionFOKS());
	OSD_IrkeyTitleDisplay(ON, posC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeZOOMDrvDirT(void)
{
	SetDirDriveZOOM(zTELE);
	SetNR_NRZoomingMode() ;
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeZOOMDrvDirW(void)
{
	SetDirDriveZOOM(zWIDE);
	SetNR_NRZoomingMode() ;
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeFOKSCoarseF(void)
{
	char posC[11];

//	TargetDriveFOKS(0x1000);
//	TargetDriveLENS(0x7ac0, 0x4098);
	CoarseDriveFOKS(1,fFAR); //0:coarse 1:fine
	sprintf(posC, "FOKS:%5d", LENS_GetRelPositionFOKS());
	OSD_IrkeyTitleDisplay(ON, posC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeFOKSCoarseN(void)
{
	char posC[11];

//	TargetDriveFOKS(0xf000);
//	TargetDriveLENS(0x0000, 0x71a4);
	CoarseDriveFOKS(1,fNEAR); //0:coarse 1:fine
	sprintf(posC, "FOKS:%5d", LENS_GetRelPositionFOKS());
	OSD_IrkeyTitleDisplay(ON, posC);
}
/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeZOOMTargetW(void)
{
	z_WID++; if (z_WID==5) z_WID = 0;

	SetWIDEDrvLIMIT(zGoalW[z_WID]);
	UARTprintf("ID:%d, ZOOM:%05d\r\n", z_WID, zGoalW[z_WID]);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeZOOMTargetT(void)
{
	z_TID++; if (z_TID==5) z_TID = 0;

	SetTELEDrvLIMIT(zGoalT[z_TID]);
	UARTprintf("ID:%d, ZOOM:%05d\r\n", z_TID, zGoalT[z_TID]);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeFOKSCompleF(void)
{
//	TargetDriveFOKS(30000);
	CompleDriveFOKS(fFAR);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeFOKSCompleN(void)
{
//	TargetDriveFOKS(18500);
	CompleDriveFOKS(fNEAR);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeZOOMTargetT(void)
{
//	TargetDriveZOOM(30000);
	CoarseDriveZOOM(zTELE);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeZOOMTargetW(void)
{
//	TargetDriveZOOM(16500);
	CoarseDriveZOOM(zWIDE);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeZOOMCompleT(void)
{
//	TargetDriveZOOM(30000);
	CompleDriveZOOM(zTELE);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeZOOMCompleW(void)
{
//	TargetDriveZOOM(16500);
	CompleDriveZOOM(zWIDE);
}
*/

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeGetValAFODM(void)
{
	SetAFExtractTrigger();	// display AF-data
}

#endif

#if defined(__USE_AFZOOM_LENS__)
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeGetValAFMOV(void)
{
//	SetAgeOnOffLENS();
//	SetAFPanTiltTrigger();	// display PT-move

	IrCodeExit();
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeAFPINTOnOff(void)
{
	SetDrvOnOffPINT();
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeAXProcessON(void)
{
	AX_EnableProcess(ON);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodePINTTargetT(void)
{
	SetDirDrivePINT(zTELE);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodePINTTargetW(void)
{
	SetDirDrivePINT(zWIDE);
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeGetIrisGRAPH(void)
{
#if defined(__USE_DCIRIS_HALL__)||defined(__USE_DCIRIS_DAMP__	)||defined(__USE_PWMIRIS_ONLY__) || defined(__USE_P_IRIS_ONLY__)
	SetAEDBGIrisTrigger();	// display IRIS-locus
#else
	OSD_MessageBox(ON, " Not available! ");
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeWBCOMPOnOff(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	stAFCTRL.ctrl &= (~AFCTRL_AFAUTO_MASK);
	stAFCTRL.ctrl |= ( AFCTRL_MANUAL_MODE);		// manual
#endif

	SetWBRAWOnOffCOMP();
//	SetWBRAWOnOffTIME(0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeWBPOLYOnOff(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	stAFCTRL.ctrl &= (~AFCTRL_AFAUTO_MASK);
	stAFCTRL.ctrl |= ( AFCTRL_MANUAL_MODE);		// manual
#endif

	SetWBRAWOnOffPOLY();
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeGetCurSTATE(void)
{
	switch(ODM_GetDEBUGMode()) {
		case 0 : ODM_SetDEBUGMode(8); break;
		case 8 : ODM_SetDEBUGMode(7); break;
		case 7 : ODM_SetDEBUGMode(2); break;
		case 2 : ODM_SetDEBUGMode(6); break;
		case 6 : ODM_SetDEBUGMode(5); break;
		case 5 : ODM_SetDEBUGMode(4); break;
		case 4 : ODM_SetDEBUGMode(3); break;
		case 3 : ODM_SetDEBUGMode(1); break;
		case 1 : ODM_SetDEBUGMode(0); break;
		default : ODM_SetDEBUGMode(0); break;
	}

	if (ODM_GetDEBUGMode()==0) { UARTprintf("[STATUS display]\r\n"); OSD_IrkeyTitleDisplay(OFF, "Status    ");}
	else if (ODM_GetDEBUGMode()==1) { UARTprintf("[ODM-AE display]\r\n"); OSD_IrkeyTitleDisplay(ON, "AE Debug  ");}
	else if (ODM_GetDEBUGMode()==2) { UARTprintf("[ODM-WB display]\r\n"); OSD_IrkeyTitleDisplay(ON, "WB Debug  ");}
	else if (ODM_GetDEBUGMode()==3) { UARTprintf("[ODM-AF display]\r\n"); OSD_IrkeyTitleDisplay(ON, "AF Debug  ");}
	else if (ODM_GetDEBUGMode()==4) { UARTprintf("[ODM-AF Trigger]\r\n"); OSD_IrkeyTitleDisplay(ON, "AF Trigger");}
	else if (ODM_GetDEBUGMode()==5) { UARTprintf("[ODM-AF Data]\r\n"); OSD_IrkeyTitleDisplay(ON, "AF Data   ");}
	else if (ODM_GetDEBUGMode()==6) { UARTprintf("[ODM-WB other]\r\n"); OSD_IrkeyTitleDisplay(ON, "WB other  ");}
	else if (ODM_GetDEBUGMode()==7) { UARTprintf("[MIMI Test]\r\n"); OSD_IrkeyTitleDisplay(ON, "mimi test ");}
	else if (ODM_GetDEBUGMode()==8) { UARTprintf("[AF Cursor]\r\n"); OSD_IrkeyTitleDisplay(ON, "AF Cursor ");}

	if (ODM_GetDEBUGMode()!=0) return;

	GetAEDataState();		// display AE-data
	GetAFDataState();		// display AF-data
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeGetViewBAYER(void)
{
	SetWBRAWViewTrigger(0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeGetGainGRAPH(void)
{
	SetAEDBGGainTrigger();	// display GAIN-locus
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeGetShutGRAPH(void)
{
	SetAEDBGShutTrigger();	// display SHUT-locus
}
/*
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
//--------------------------------------------------------------------------------------------------------------------------	
static void IrCodeAuxGammaOnOff(void)
{
	WORD	OnOff = GetTONE_AYCGAMMA_enable();
	
	if ( OnOff == OFF )	SetTONE_AYCGAMMA_enable(ON) ;
	else				SetTONE_AYCGAMMA_enable(OFF);
}
#endif
*/
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeNR2D3DCtrlOnOff(void)
{
	WORD	mode = GetNR_Mode();
	
	switch(mode)	{
		case MISP_NR_OFF	:
		case MISP_NR_2D		:
		case MISP_NR_3D		:	mode = MISP_NR_2D3D;	OSD_IrkeyTitleDisplay(OFF, "NR_ON     ");	break;
		case MISP_NR_2D3D	:	mode = MISP_NR_OFF;		OSD_IrkeyTitleDisplay(ON, "NR_OFF    ");	break;
	}
	
	SetNR_Mode((MISP_NR_MODE_t)mode);
	SetMenuStatus(DNR_MODE_MENU, mode);
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_NR3DDemomode(void)
{
	WORD	mode;
	
#if defined(__USE_4K60_INOUT__)
	OSD_MessageBox(ON, " Not available! ");
	return;
#endif
	
	if(GetNR_Mode() == MISP_NR_OFF)		return;
	
	mode = GetNR_DemoMode();    mode++;
	
	if(mode > MISP_NRDEMO_RIGHT_ON)		mode=MISP_NRDEMO_OFF;
	
	switch(mode) {
		case MISP_NRDEMO_OFF:		OSD_InfoDisplay(OFF, "NR  ON    ", "NR  ON    "); break;
		case MISP_NRDEMO_LEFT_ON: 	OSD_InfoDisplay(  2, "NR  ON    ", "NR  OFF   "); break;
		case MISP_NRDEMO_RIGHT_ON: 	OSD_InfoDisplay(  2, "NR  OFF   ", "NR  ON    "); break;
	}
	
	SetNR_DemoMode(mode);
	SetMenuStatus(DNRTUNE_DEMO_MENU, mode);
	OSD_DrawOutlineDouble((mode)?  ON:  OFF, GetOUT_MainSizeHA(), GetOUT_MainSizeVA());
}
/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_NRDebugDisplay(void)
{
	WORD	mode;
	// WORD	stdev_int , stdev_frac ;
	// char	write_data_buff[10];
	
	if(GetNR_Mode() == MISP_NR_OFF)		return;
	
	mode = GetNR_DemoMode();   	
	// GetNR_StDEV(&stdev_int , &stdev_frac ,1 ) ; // Stored Stdev 
		
	if (  mode < MISP_NRDEMO_RIGHT_ON )  mode = MISP_NRDEMO_IDM_L ;
	else  mode++ ;	
	
	if(mode > MISP_NRDEMO_SRM)		mode=MISP_NRDEMO_OFF;
	
	switch(mode) {
		case MISP_NRDEMO_OFF : OSD_IrkeyTitleDisplay(OFF, "NR Debug off"); break;
		case MISP_NRDEMO_IDM_L : OSD_IrkeyTitleDisplay(ON, "IDM_L     "); break;
		case MISP_NRDEMO_IDM_H : OSD_IrkeyTitleDisplay(ON, "IDM_H     "); break;
		case MISP_NRDEMO_PDM : OSD_IrkeyTitleDisplay(ON, "PDM       "); break;
		case MISP_NRDEMO_SRM : OSD_IrkeyTitleDisplay(ON, "SRM       "); break;
		// case MISP_NRDEMO_IDM_L : sprintf(write_data_buff, "IDM_L:%d.%d ", stdev_int, stdev_frac); break;
		// case MISP_NRDEMO_IDM_H : sprintf(write_data_buff, "IDM_H:%d.%d ", stdev_int, stdev_frac); break;
		// case MISP_NRDEMO_PDM_U : sprintf(write_data_buff, "PDM_U:%d.%d ", stdev_int, stdev_frac); break;
		// case MISP_NRDEMO_PDM : sprintf(write_data_buff, "PDM :%d.%d  ", stdev_int, stdev_frac); break;
		// case MISP_NRDEMO_SRM : sprintf(write_data_buff, "SRM :%d.%d  ", stdev_int, stdev_frac); break;		
	}
	
	// if ( mode == MISP_NRDEMO_OFF ) OSD_IrkeyTitleDisplay(OFF, "NR Debug off");
	// else OSD_IrkeyTitleDisplay(ON, write_data_buff) ;

	SetNR_DemoMode((MISP_NRDEMO_MODE_t)mode);   
}
*/

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeDefogOnOff(void)
{
	Defog_DEMOview = GetMenuStatus(DEFOG_MENU);
	
#if defined(__USE_4K60_INOUT__)
	if(Defog_DEMOview<1)		Defog_DEMOview++;
	else					Defog_DEMOview=0;
#else
	if(Defog_DEMOview<2)		Defog_DEMOview++;
	else					Defog_DEMOview=0;
#endif
	
#if defined(__USE_DEFOG_DWDR_ONE_MENU__)
	SetBY_DefogDWDRMode(Defog_DEMOview);
#else
	SetBY_DefogMode(Defog_DEMOview);
#endif

	switch(Defog_DEMOview)	{
		case 0:	OSD_IrkeyTitleDisplay(OFF, "DEFOG OFF ");	break;
		case 1:	OSD_IrkeyTitleDisplay(ON,  "DEFOG On  ");	break;
		case 2:	OSD_IrkeyTitleDisplay(ON,  "DEFOG Auto");	break;
	}	

	SetMenuStatus(DEFOG_MENU, Defog_DEMOview);	
	SetMenuRedraw();	

}

/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeDefogDemomode(void)
{
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	WORD	mode;
	
	if(GetBY_DefogMode() == MISP_DEFOG_OFF)		return;
	
	mode = GetBY_DefogDispSel();    mode++;
	
	if(mode > MISP_SCREEN_R_ON)		mode=MISP_SCREEN_OFF;
	
	switch(mode) {
		case MISP_SCREEN_OFF:	OSD_InfoDisplay(OFF, "DEFOG  ON ", "DEFOG  ON "); break;
		case MISP_SCREEN_L_ON: 	OSD_InfoDisplay(  2, "DEFOG  ON ", "DEFOG  OFF"); break;
		case MISP_SCREEN_R_ON: 	OSD_InfoDisplay(  2, "DEFOG  OFF", "DEFOG  ON "); break;
	}
	
	SetBY_DefogDispSel(mode);
	SetMenuStatus(DEFOG_DEMOSCR_MENU, mode);
	OSD_DrawOutlineDouble((mode)?  ON:  OFF, GetOUT_MainSizeHA(), GetOUT_MainSizeVA());
#else 												// MDIN-i51X (i510, i540, i550)
	OSD_MessageBox(ON, " Not supported ...  ");
#endif
}
*/
//--------------------------------------------------------------------------------------------------------------------------
static void pip_mode_display(void)
{
	if(MainSrc==1)	{
		if(PIPSrc==1)		OSD_IrkeyTitleDisplay(ON, "HDMI/HDMI ");
		else				OSD_IrkeyTitleDisplay(ON, "HDMI/CMOS ");
	}
	else	{
		if(PIPSrc==1)		OSD_IrkeyTitleDisplay(ON, "CMOS/HDMI ");
		else				OSD_IrkeyTitleDisplay(ON, "CMOS/CMOS ");
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodePIPCtrlOnOff(void)
{
	WORD	PIPMode = GetMenuStatus(PIPMODE_MENU);			// 0:off, 1:on
	WORD	PIPSize = GetMenuStatus(PIPSIZE_MENU);			// 0:1/4, 1:1/9, 2:1/16, 3:1/25
	
#if defined(__USE_CMOS_AUX_SYNC_OUT__)
	if(OSD_GetMenuLang()==0)	{OSD_MessageBox(ON, " Not Available ! ");			return;}
	else						{OSD_MessageBox(ON, (PBYTE)msg_chn_not_available);	return;}
#endif
	
	if(GetSYSINFO_BoardType()==MISP_SYSINFO_I550_USB_V10)	{OSD_MessageBox(ON, " Not Available ! ");	return;}
	
	if(OSD_GetMenuLang()==0)	{	// eng
		if(GetDIS_OnOff())	{OSD_MessageBox(ON, " Turn off DIS ! ");		return;}
		if(CascadeMode)		{OSD_MessageBox(ON, " Turn off Cascade ! ");	return;}
	}
	else	{						// chn
		if(GetDIS_OnOff())	{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_dis);		return;}
		if(CascadeMode)		{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_cascade);	return;}
	}
	
	if (PIPMode)	PIPMode = OFF;
	else			PIPMode = ON;

	SetMenuStatus(PIPMODE_MENU, PIPMode);
	SetPIPSource(PIPSrc);	 DEMO_SetPIPOnOff(PIPMode); DEMO_SetPIPSize(PIPSize); 	

	if(PIPMode == OFF)		OSD_IrkeyTitleDisplay(OFF, "PIP:OFF   ");
	else					pip_mode_display();
	
#if __MISP100_DBGPRT__ == 1
	UARTprintf("IrCodePIPCtrlOnOff : %s, PIP Size(%d) \r\n", (PIPMode > 0)? "ON": "OFF", PIPSize);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodePIPSize(void)
{
	WORD	PIPMode = GetMenuStatus(PIPMODE_MENU);			// 0:off, 1:on
	WORD	PIPSize = GetMenuStatus(PIPSIZE_MENU);			// 0:1/4, 1:1/9, 2:1/16, 3:1/25
	
#if defined(__USE_CMOS_AUX_SYNC_OUT__)
	if(OSD_GetMenuLang()==0)	{OSD_MessageBox(ON, " Not Available ! ");			return;}
	else						{OSD_MessageBox(ON, (PBYTE)msg_chn_not_available);	return;}
#endif
	
	if(GetSYSINFO_BoardType()==MISP_SYSINFO_I550_USB_V10)	{OSD_MessageBox(ON, " Not Available ! ");	return;}
	
	if(PIPMode==0) 							return;

#if defined(__USE_CMOS_IMX226__)||defined(__USE_CMOS_IMX274__)||defined(__USE_CMOS_IMX326__) // 4K sensors restrict PiP size 
	if (PIPSize >= 3)	PIPSize = 2;	// pip off, size default=2(1/16)
#else
	if (PIPSize >= 3)	PIPSize = 0;	// pip off, size default=0(1/4)
#endif
	else				PIPSize++;						// inc pip size

	SetMenuStatus(PIPSIZE_MENU, PIPSize);
	DEMO_SetPIPSize(PIPSize); 	

#if __MISP100_DBGPRT__ == 1
	UARTprintf("IrCodePIPSize : %d \r\n", PIPSize);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodePipPos(void)
{
	WORD	PIPMode = GetMenuStatus(PIPMODE_MENU);			// 0:off, 1:on
	WORD	PIPSize = GetMenuStatus(PIPSIZE_MENU);			// 0:1/4, 1:1/9, 2:1/16, 3:1/25

#if defined(__USE_4K60_INOUT__)
	OSD_MessageBox(ON, " Not Available ! ");
	return;
#endif
	
#if defined(__USE_CMOS_AUX_SYNC_OUT__)
	if(OSD_GetMenuLang()==0)	{OSD_MessageBox(ON, " Not Available ! ");			return;}
	else						{OSD_MessageBox(ON, (PBYTE)msg_chn_not_available);	return;}
#endif
	
	if(GetSYSINFO_BoardType()==MISP_SYSINFO_I550_USB_V10)	{OSD_MessageBox(ON, " Not Available ! ");	return;}
	
	if((DemoPIPCtrlOnOff==ON)||(PIPMode==0)) return;
	if(PIPSize > 3)							return;		// skip : 1:1, 4ch
	
	if(PipPosPreSet<3)	PipPosPreSet++;
	else PipPosPreSet=0;

	switch(PipPosPreSet)	{
		case 0: SetMenuStatus(PIPPOSI_MENU, 10);	SetMenuPosiY(PIPPOSI_MENU, 10);	break;
		case 1: SetMenuStatus(PIPPOSI_MENU, 0);	SetMenuPosiY(PIPPOSI_MENU, 10);	break;
		case 2: SetMenuStatus(PIPPOSI_MENU, 0);	SetMenuPosiY(PIPPOSI_MENU, 0);	break;
		case 3: SetMenuStatus(PIPPOSI_MENU, 10);	SetMenuPosiY(PIPPOSI_MENU, 0);	break;
		default: SetMenuStatus(PIPPOSI_MENU, 10);	SetMenuPosiY(PIPPOSI_MENU, 10);	break;
	}

	DEMO_SetPIPPosition(0);
}
/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodePIPSWAP(void)
{
	WORD ZoomMode = GetMenuStatus(DZMODE_MENU);			// 0:off, 1:on
	
	if(OSD_GetMenuLang()==0)	{	// eng
		if(ZoomMode)				{OSD_MessageBox(ON, " Turn off DZOOM ! ");		return;}
	}
	else	{						// chn
		if(ZoomMode)				{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_dzoom);	return;}
	}
	
	MainSrc = (MainSrc==1)? 0: 1;						// main input swap(sensor <-> hdmi)
	
	SetMainSource(MainSrc);		SetPIPSource(PIPSrc);
	pip_mode_display();
}
*/
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodePIPCHSEL(void)
{
	WORD PIPMode = GetMenuStatus(PIPMODE_MENU);			// 0:off, 1:on
	WORD ZoomMode = GetMenuStatus(DZMODE_MENU);			// 0:off, 1:on
	
#if defined(__USE_4K60_INOUT__)
	OSD_MessageBox(ON, " Not Available ! ");
	return;
#endif
	
#if defined(__USE_CMOS_AUX_SYNC_OUT__) || defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__)
	if(OSD_GetMenuLang()==0)	{OSD_MessageBox(ON, " Not Available ! ");			return;}
	else						{OSD_MessageBox(ON, (PBYTE)msg_chn_not_available);	return;}
#endif
	
	if(GetSYSINFO_BoardType()==MISP_SYSINFO_I550_USB_V10)	{OSD_MessageBox(ON, " Not Available ! ");	return;}
	
	if(PIPMode != ON)			return;
	
	if(OSD_GetMenuLang()==0)	{	// eng
		if(ZoomMode)				{OSD_MessageBox(ON, " Turn off DZOOM ! ");		return;}
	}
	else	{						// chn
		if(ZoomMode)				{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_dzoom);	return;}
	}
	
	PIPSrc = (PIPSrc==1)? 0: 1;
	SetPIPSource(PIPSrc);
	pip_mode_display();
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeDZoomCtrlOnOff(void)
{
	WORD	ZoomMode = GetMenuStatus(DZMODE_MENU);						// 0:off, 1:on
	
	if(OSD_GetMenuLang()==0)	{	// eng
#if defined(__USE_MDIN_i500__)
		if(GetWDR_ProcesMode())		{OSD_MessageBox(ON, " Turn off WDR/Compr.");	return;}
#endif
		if(GetDIS_OnOff())			{OSD_MessageBox(ON, " Turn off DIS ! ");		return;}
		if(CascadeMode)				{OSD_MessageBox(ON, " Turn off Cascade ! ");	return;}
		
		if (ZoomMode)	{ ZoomMode = OFF;	fZOOMMove = FALSE; OSD_IrkeyTitleDisplay(OFF, "DZOOM OFF ");	}	// zoom on, move enable
		else			{ ZoomMode = ON;	fZOOMMove = TRUE; OSD_IrkeyTitleDisplay(ON, "DZOOM ON  ");	}	// zoom on, move enable
	}
	else	{						// chn
#if defined(__USE_MDIN_i500__)
		if(GetWDR_ProcesMode())		{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_wdr_compr);	return;}
#endif
		if(GetDIS_OnOff())			{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_dis);		return;}
		if(CascadeMode)				{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_cascade);	return;}
		
		if (ZoomMode)	{ ZoomMode = OFF;	fZOOMMove = FALSE; OSD_IrkeyTitleDisplay(OFF, (PBYTE)title_chn_dzoom_off);	}	// zoom on, move enable
		else			{ ZoomMode = ON;	fZOOMMove = TRUE; OSD_IrkeyTitleDisplay(ON, (PBYTE)title_chn_dzoom_on);	}	// zoom on, move enable
	}
	
	if(GetSYSINFO_BoardType()==MISP_SYSINFO_I550_USB_V10)	{OSD_MessageBox(ON, " Not Available ! ");	return;}

	SetMenuStatus(DZMODE_MENU, ZoomMode);
	DEMO_SetZoomMode(ZoomMode);

#if __MISP100_DBGPRT__ == 1	
	UARTprintf("IrCodeDZoomCtrlOnOff : %s, Ratio(%d) \r\n", (ZoomMode > 0)? "ON": "OFF", ZoomRatio);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeDZoomIN(void)
{
	WORD	ZoomMode = GetMenuStatus(DZMODE_MENU);						// 0:off, 1:on
	WORD	PIPOnOff = GetMenuStatus(PIPMODE_MENU);
	WORD	ZoomRatio = GetDZOOM_Ratio();
	
	if(GetWDR_ComprState())		return;
	if(GetOSDMenuID())			return;		// skip dzoom in/out ir key on menu
	
	if(GetSYSINFO_BoardType()==MISP_SYSINFO_I550_USB_V10)	{OSD_MessageBox(ON, " Not Available ! ");	return;}

	if (ZoomMode)	{
//		if (ZoomRatio >= 50)	{ ZoomRatio=50;						}	// zoom in max
		if (ZoomRatio >= DZOOMCTRL_RATIO_MAX)        { ZoomRatio=DZOOMCTRL_RATIO_MAX;    }
		else					{ ZoomRatio++; 	fZOOMMove = TRUE; 	}	// inc zoomratio, move enable
		
#if defined(__USE_4K60_INOUT__)
		DEMO_SetZoomRatio(ZoomRatio);
		SetMenuStatus(DZRATIO_MENU, ZoomRatio);
		PIPOnOff=PIPOnOff;
#else
		SetDZOOM_TargetRatio(ZoomRatio);								// dzoom ratio control
		SetMenuStatus(DZRATIO_MENU, ZoomRatio);
		DEMO_SetPIPOnOff(PIPOnOff);										// pip box osd move
#endif
	} else {
#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
		//if((stAECTRL.ctrl & AECTRL_PROCESS_OFF) == AECTRL_PROCESS_OFF)
		//	DCLENS_ForceIRISUpDown(0); // for IRIS manual control
#endif
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeDZoomOUT(void)
{
	WORD	ZoomMode = GetMenuStatus(DZMODE_MENU);						// 0:off, 1:on
	WORD	PIPOnOff = GetMenuStatus(PIPMODE_MENU);
	WORD	ZoomRatio = GetDZOOM_Ratio();

	if(GetWDR_ComprState())		return;
	if(GetOSDMenuID())			return;		// skip dzoom in/out ir key on menu
	
	if(GetSYSINFO_BoardType()==MISP_SYSINFO_I550_USB_V10)	{OSD_MessageBox(ON, " Not Available ! ");	return;}
	
	if (ZoomMode)	{
		if (ZoomRatio == 0)	{ ZoomRatio=0;	fZOOMMove = FALSE; }	// zoom out min
		else				{ ZoomRatio--; 	fZOOMMove = TRUE; }		// dec zoomratio, move enable
		
#if defined(__USE_4K60_INOUT__)
		DEMO_SetZoomRatio(ZoomRatio);
		SetMenuStatus(DZRATIO_MENU, ZoomRatio);
		PIPOnOff=PIPOnOff;
#else
		SetDZOOM_TargetRatio(ZoomRatio);								// dzoom ratio control
		SetMenuStatus(DZRATIO_MENU, ZoomRatio);
		DEMO_SetPIPOnOff(PIPOnOff);									// pip box osd move
#endif
	} else {
#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
		//if((stAECTRL.ctrl & AECTRL_PROCESS_OFF) == AECTRL_PROCESS_OFF)
		//	DCLENS_ForceIRISUpDown(1); // for IRIS manual control
#endif
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeDISOnOff(void)
{
#if defined(__USE_4K60_INOUT__)
	OSD_MessageBox(ON, " Not available! ");
	return;
#elif defined(__USE_AFZOOM_LENS__) && (defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX334__) || defined(__USE_CMOS_IMX253__))		
	OSD_MessageBox(ON, " Not available! ");
	return;
#endif
	
	if(GetSYSINFO_BoardType()==MISP_SYSINFO_I550_USB_V10)	{OSD_MessageBox(ON, " Not Available ! ");	return;}
	
	if(OSD_GetMenuLang()==0)	{		// eng
		if(GetWDR_ProcesMode())			{OSD_MessageBox(ON, " Turn off WDR/Compr.");	return;}
		if(CascadeMode)					{OSD_MessageBox(ON, " Turn off Cascade ! ");	return;}
		if(GetDZOOM_OnOff())			{OSD_MessageBox(ON, " Turn off DZOOM ! ");		return;}
	}
	else	{							// chn
		if(GetWDR_ProcesMode())			{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_wdr_compr);	return;}
		if(CascadeMode)					{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_cascade);	return;}
		if(GetDZOOM_OnOff())			{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_dzoom);		return;}
	}
	
	if (GetDIS_OnOff())		{Demo_DIS_OnOff(OFF);	SetMenuStatus(DIS_MENU, OFF);}
	else					{Demo_DIS_OnOff(ON);	SetMenuStatus(DIS_MENU, ON);}
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_WdrOnOff(BYTE nID)
{
	if(GetSYSINFO_BoardType()==MISP_SYSINFO_I550_USB_V10)	{OSD_MessageBox(ON, " Not Available ! ");	return;}

#if defined(__USE_WDR_PROCESS__)
//#if USE_IPC_COMPRES
	if(OSD_GetMenuLang()==0)	{		// eng
		if(GetDIS_OnOff())				{OSD_MessageBox(ON, " Turn off DIS ! ");      return;}
		if(CascadeMode)					{OSD_MessageBox(ON, " Turn off Cascade ! ");  return;}
		if(GetDZOOM_OnOff())			{OSD_MessageBox(ON, " Turn off DZOOM ! ");    return;}
	}
	else	{							// chn
		if(GetDIS_OnOff())				{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_dis); return;}
		if(CascadeMode)					{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_cascade); return;}
		if(GetDZOOM_OnOff())			{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_dzoom);   return;}
	}
//#endif
	if(OSD_GetMenuLang()==0)	{		// eng
		if(GetSS_binning_enable())					{OSD_MessageBox(ON, " Turn off Binning ! ");  return;}
		if(GetIN_MainPath()!=MISP_MAINPATH_SENSOR)	{OSD_MessageBox(ON, " Set Sensor Input ! ");  return;}
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		if(GetROT_RotationEnable())					{OSD_MessageBox(ON, " Turn off Rotation ! "); return;}
#endif
	}
	else	{							// chn
		if(GetSS_binning_enable())					{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_binning);  return;}
		if(GetIN_MainPath()!=MISP_MAINPATH_SENSOR)	{OSD_MessageBox(ON, (PBYTE)msg_chn_set_sensor_input);  return;}
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		if(GetROT_RotationEnable())					{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_rotation); return;}
#endif
	}

	//configure
	if(GetMenuStatus(WDR_TUNE_MENU)==nID) {  //ON->OFF
		DEMO_SetWDR_MODE(OFF);
	}
	else {  //OFF->ON
		DEMO_SetWDR_MODE(nID);
	}

	//display
	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_DISBL:  OSD_IrkeyTitleDisplay(OFF, "WDR OFF   ");  break;
#if defined(__USE_WDR_EXT_MENU__)
		case MISP_WDR_PROC_FRAME:  OSD_IrkeyTitleDisplay(ON,  "WDR(FRAME)");  break;
		case MISP_WDR_PROC_BUILT:  OSD_IrkeyTitleDisplay(ON,  "WDR(TONE) ");  break;
		case MISP_WDR_PROC_SINGL:  OSD_IrkeyTitleDisplay(ON,  "WDR(DWDR) ");  break;
		case MISP_WDR_PROC_LINE2:  OSD_IrkeyTitleDisplay(ON,  "WDR(2LINE)");  break;
		case MISP_WDR_PROC_LINE3:  OSD_IrkeyTitleDisplay(ON,  "WDR(3LINE)");  break;
#endif
		default:                   OSD_IrkeyTitleDisplay(ON,  "WDR ON    ");  break;
	}

	return;
#else	//__USE_WDR_PROCESS__
	OSD_MessageBox(ON, " Not available! ");
#endif	//__USE_WDR_PROCESS__
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_WdrDemoMode(void)
{
#if defined(__USE_WDR_PROCESS__)
	BYTE STR_NULL[]="          ";
	BYTE STR_WDR2[]="WDR-2FRAME";
	BYTE STR_WDR3[]="WDR-3FRAME";
	BYTE STR_LONG[]="LDR-LONG  ";
	BYTE STR_SH_1[]="LDR-SHORT ";
	BYTE STR_SH_2[]="LDR-SHORTr";

	static BYTE demo=0;
	BYTE mode = 0;  //0:normal, 1:left only, 2:right only
	BYTE fsel;  //0:long, 1:sh_1, 2:sh_2
	PBYTE pStr_l;
	PBYTE pStr_r;
	switch(GetWDR_ProcesMode()) {
		case MISP_WDR_PROC_FRAME:
		case MISP_WDR_PROC_LINE2:
			switch(demo) {  //2->1 merge
				case 0:   demo=1;  mode=1;  fsel=0;  pStr_l=STR_WDR2;  pStr_r=STR_LONG;  break;
				case 1:   demo=2;  mode=1;  fsel=2;  pStr_l=STR_WDR2;  pStr_r=STR_SH_1;  break;
				case 2:   demo=3;  mode=2;  fsel=0;  pStr_l=STR_LONG;  pStr_r=STR_WDR2;  break;
				case 3:   demo=4;  mode=2;  fsel=2;  pStr_l=STR_SH_1;  pStr_r=STR_WDR2;  break;
				default:  demo=0;  mode=0;  fsel=0;  pStr_l=STR_NULL;  pStr_r=STR_NULL;  break;
			}
			break;
		case MISP_WDR_PROC_LINE3:
			switch(demo) {  //3->1 merge
				case 0:   demo=1;  mode=1;  fsel=0;  pStr_l=STR_WDR3;  pStr_r=STR_LONG;  break;
				case 1:   demo=2;  mode=1;  fsel=1;  pStr_l=STR_WDR3;  pStr_r=STR_SH_1;  break;
				case 2:   demo=3;  mode=1;  fsel=2;  pStr_l=STR_WDR3;  pStr_r=STR_SH_2;  break;
				case 3:   demo=4;  mode=2;  fsel=0;  pStr_l=STR_LONG;  pStr_r=STR_WDR3;  break;
				case 4:   demo=5;  mode=2;  fsel=1;  pStr_l=STR_SH_1;  pStr_r=STR_WDR3;  break;
				case 5:   demo=6;  mode=2;  fsel=2;  pStr_l=STR_SH_2;  pStr_r=STR_WDR3;  break;
				default:  demo=0;  mode=0;  fsel=0;  pStr_l=STR_NULL;  pStr_r=STR_NULL;  break;
			}
			break;
		default:
			demo=0;  mode=0;  fsel=0;  pStr_l=STR_NULL;  pStr_r=STR_NULL;
			break;
	}

	//set demo mode
	SetWDR_DemoMode(mode, fsel);

	//draw OSD
	if(mode) {
		OSD_InfoDisplay(  2, pStr_l, pStr_r);
		stSBOX[SBOX_INDEX0].ctrl|= SBOX_PLANE_ON;
	}
	else {
		OSD_InfoDisplay(OFF, pStr_l, pStr_r);
		stSBOX[SBOX_INDEX0].ctrl&=~SBOX_PLANE_ON;
	}
	stSBOX[SBOX_INDEX0].stRECT.lx=1920/2-2;  stSBOX[SBOX_INDEX0].stRECT.ly=0;
	stSBOX[SBOX_INDEX0].stRECT.rx=1920/2+1;  stSBOX[SBOX_INDEX0].stRECT.ry=1080;
	MISPOSD_SetSBoxConfig(&stSBOX[SBOX_INDEX0], (SBOX_INDEX_t)SBOX_INDEX0);
#else	//__USE_WDR_PROCESS__
	OSD_MessageBox(ON, " Not available! ");
#endif	//__USE_WDR_PROCESS__
}


//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_WdrTestFunc(BYTE nID)
{
#if defined(__USE_WDR_PROCESS__)
	PBYTE pStr=SetWDR_TestFunc(nID);
	if(!GetWDR_ProcesMode()) return;  //WDR OFF

	if(*pStr)  OSD_IrkeyTitleDisplay(ON,  pStr);
	else       OSD_IrkeyTitleDisplay(OFF, pStr);
#else
	OSD_MessageBox(ON, " Not available! ");
#endif
}

/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeCascadeOnOff(void)
{
	WORD PIPMode;
	
	if(OSD_GetMenuLang()==0)	{		// eng
		if(GetDIS_OnOff())			{OSD_MessageBox(ON, " Turn off DIS ! ");	return;}
		if(GetDZOOM_OnOff())		{OSD_MessageBox(ON, " Turn off DZOOM ! ");    return;}
		if(GetWDR_ComprState())		{OSD_MessageBox(ON, " Turn off WDR/Compr.");	return;}
	}
	else	{
		if(GetDIS_OnOff())			{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_dis);	return;}
		if(GetDZOOM_OnOff())		{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_dzoom);    return;}
		if(GetWDR_ComprState())		{OSD_MessageBox(ON, (PBYTE)msg_chn_turn_off_wdr_compr);	return;}
	}
	
	if(CascadeMode >= 3)		{CascadeMode=OFF;	PIPMode=OFF;	PIPSrc=0;}
	else 						{CascadeMode++;		PIPMode=ON;		PIPSrc=1;}
	
	if(CascadeMode == 1)		CascadeMode++;	// skip Main+Aux 1:1 mode
	
	SetPIPSource(PIPSrc); 		// set pip source to external input(0:sensor, 1:ext.in, default:sensor)
	DEMO_SetCascade(CascadeMode);
}
*/

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeVOUTToggle()	
{
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550	
	WORD current_mode ;
//	WORD PIPMode = GetMenuStatus(PIPMODE_MENU);			// 0:off, 1:on
//	WORD PIPSize = GetMenuStatus(PIPSIZE_MENU);			// 0:1/4, 1:1/9, 2:1/16, 3:1/25	
	
	if(GetDIS_OnOff())	{OSD_MessageBox(ON, " Turn off DIS ! ");		return;}
	
	current_mode = GetOUT_MainFrmt();
	
	if ( current_mode == MISP_VIDOUT_3840x2160p30 )
	{
#if defined(__USE_FHDCROP_MODE__)	// only for 4K input
		Demo_Set_OutFHD_Crop() ;
#else
		Demo_Set_OutFHD_Scale() ;
#endif
	}
	else
	{
		Demo_Set_OutUHD() ;
	}

//	DEMO_SetPIPOnOff(PIPMode); DEMO_SetPIPSize(PIPSize);	
//	SetPIPSource(PIPSrc);
#endif
}
	
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeVOUTFormat(BOOL OnOff)
{
	BYTE frmt = GetMenuStatus(OUTFRMT_MENU) + MISP_VIDOUT_1280x720p60;

	if(OnOff)		VoutFormatCnt++;
	else			VoutFormatCnt=0;
	
	if(VoutFormatCnt >= 2)	{		// press remote key '2 times'
		VoutFormatCnt=0;
		
		switch(frmt)	{			// i500&510 : 1080p60/50 <-> 1080p30/25,  i540&550 : 1080p60/50 <-> 4K30/25
			case  MISP_VIDOUT_1920x1080p60:	frmt=MISP_VIDOUT_1920x1080p50;	break;
#if defined(__USE_MDIN_i500__) || defined(__USE_MDIN_i510__) 	// i500, i510
			case  MISP_VIDOUT_1920x1080p50:	frmt=MISP_VIDOUT_1920x1080p30;	break;
			case  MISP_VIDOUT_1920x1080p30:	frmt=MISP_VIDOUT_1920x1080p25;	break;
			case  MISP_VIDOUT_1920x1080p25:	frmt=MISP_VIDOUT_1920x1080p60;	break;
#endif
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
#if defined(__USE_4K60_INOUT__)
			case  MISP_VIDOUT_1920x1080p50:	frmt=MISP_VIDOUT_1920x2160p60;	break;
			case  MISP_VIDOUT_1920x2160p60:	frmt=MISP_VIDOUT_1920x2160p50;	break;
			case  MISP_VIDOUT_1920x2160p50:	frmt=MISP_VIDOUT_1920x1080p60;	break;
#else
			case  MISP_VIDOUT_1920x1080p50:	frmt=MISP_VIDOUT_3840x2160p30;	break;
			case  MISP_VIDOUT_3840x2160p30:	frmt=MISP_VIDOUT_3840x2160p25;	break;
			case  MISP_VIDOUT_3840x2160p25:	frmt=MISP_VIDOUT_1920x1080p60;	break;
#endif
#endif
			default:							frmt=MISP_VIDOUT_1920x1080p60;	break;
		}
		
#if defined(__MDIN_i550_USB_V100__)
		if(frmt==MISP_VIDOUT_3840x2160p25)		frmt=MISP_VIDOUT_1920x1080p60;	// HFP : 25p(1056), 24p(1276), sil9136 limit(1~1023)
#endif
		
		SetMenuStatus(OUTFRMT_MENU, frmt-MISP_VIDOUT_1280x720p60);
		DEMO_SetOutputFormat(frmt);
		
		switch(frmt)	{
			case  MISP_VIDOUT_1920x1080p30:	OSD_IrkeyTitleDisplay(ON,  "1080p 30  ");	break;
			case  MISP_VIDOUT_1920x1080p25:	OSD_IrkeyTitleDisplay(ON,  "1080p 25  ");	break;
			case  MISP_VIDOUT_1920x1080p60:	OSD_IrkeyTitleDisplay(ON,  "1080p 60  ");	break;
			case  MISP_VIDOUT_1920x1080p50:	OSD_IrkeyTitleDisplay(ON,  "1080p 50  ");	break;
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
			case  MISP_VIDOUT_3840x2160p30:	OSD_IrkeyTitleDisplay(ON,  "2160p 30  ");	break;
			case  MISP_VIDOUT_3840x2160p25:	OSD_IrkeyTitleDisplay(ON,  "2160p 25  ");	break;
#endif
		}
		
		UARTprintf("video out format changed !!\r\n");
	}
}

/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeBWExtOnOff(void)
{
	if(DemoBWExtOnOff) 	{DemoBWExtOnOff = 0;	SetOUT_BWExtension(OFF);	OSD_IrkeyTitleDisplay(ON, "BWExt. OFF");}
	else 				{DemoBWExtOnOff = 1;	SetOUT_BWExtension(ON);	OSD_IrkeyTitleDisplay(ON, "BWExt.  ON");}
}
*/
//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_AEBRIGHT(void)
{
	LONG AE_MODE=Get_AE_Mode();
	WORD Bright_Level=GetMenuStatus(EXP_BRIGHT_MENU);
	
	//load ae mode. if manu mode then return
	if(AE_MODE==AECTRL_ALL_MANUAL){
		OSD_MessageBox(ON, " NOW MANUAL AE  ");	
		return;
	}

	//Get ae bright menu value. if lower than maximum, then ae bright +1
	if(Bright_Level<20){
		SetAEBrightLvl(Bright_Level+1);
		SetMenuStatus(EXP_BRIGHT_MENU,Bright_Level+1);
	}
	else{
		//OSD_MessageBox(ON, " AEBRIGHT MAX   ");	
	}
	
	//show osd short list
	switch(GetMenuStatus(EXP_BRIGHT_MENU))	{
		case 0:	OSD_MessageBox(ON, " AEBRIGHT : 0   ");		break;
		case 1:	OSD_MessageBox(ON, " AEBRIGHT : 1   ");		break;
		case 2:	OSD_MessageBox(ON, " AEBRIGHT : 2   ");		break;
		case 3:	OSD_MessageBox(ON, " AEBRIGHT : 3   ");		break;
		case 4:	OSD_MessageBox(ON, " AEBRIGHT : 4   ");		break;
		case 5:	OSD_MessageBox(ON, " AEBRIGHT : 5   ");		break;		
		case 6:	OSD_MessageBox(ON, " AEBRIGHT : 6   ");		break;
		case 7:	OSD_MessageBox(ON, " AEBRIGHT : 7   ");		break;
		case 8:	OSD_MessageBox(ON, " AEBRIGHT : 8   ");		break;
		case 9:	OSD_MessageBox(ON, " AEBRIGHT : 9   ");		break;
		case 10:	OSD_MessageBox(ON, " AEBRIGHT : 10  ");		break;
		case 11:	OSD_MessageBox(ON, " AEBRIGHT : 11  ");		break;	
		case 12:	OSD_MessageBox(ON, " AEBRIGHT : 12  ");		break;
		case 13:	OSD_MessageBox(ON, " AEBRIGHT : 13  ");		break;
		case 14:	OSD_MessageBox(ON, " AEBRIGHT : 14  ");		break;
		case 15:	OSD_MessageBox(ON, " AEBRIGHT : 15  ");		break;
		case 16:	OSD_MessageBox(ON, " AEBRIGHT : 16  ");		break;
		case 17:	OSD_MessageBox(ON, " AEBRIGHT : 17  ");		break;		
		case 18:	OSD_MessageBox(ON, " AEBRIGHT : 18  ");		break;
		case 19:	OSD_MessageBox(ON, " AEBRIGHT : 19  ");		break;
		case 20:	OSD_MessageBox(ON, " AEBRIGHT : 20  ");		break;	
	}	

}
//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_AEDARK(void)
{
	LONG AE_MODE=Get_AE_Mode();
	WORD Bright_Level=GetMenuStatus(EXP_BRIGHT_MENU);
	
	//load ae mode. if manu mode then return
	if(AE_MODE==AECTRL_ALL_MANUAL){
		OSD_MessageBox(ON, " NOW MANUAL AE  ");	
		return;
	}

	//Get ae bright menu value. if lower than maximum, then ae bright +1
	if(Bright_Level>0){
		SetAEBrightLvl(Bright_Level-1);
		SetMenuStatus(EXP_BRIGHT_MENU,Bright_Level-1);
	}
	else{
		//OSD_MessageBox(ON, " AEBRIGHT MIN   ");	
	}

	//show osd short list
	switch(GetMenuStatus(EXP_BRIGHT_MENU))	{
		case 0:	OSD_MessageBox(ON, " AEBRIGHT : 0   ");		break;
		case 1:	OSD_MessageBox(ON, " AEBRIGHT : 1   ");		break;
		case 2:	OSD_MessageBox(ON, " AEBRIGHT : 2   ");		break;
		case 3:	OSD_MessageBox(ON, " AEBRIGHT : 3   ");		break;
		case 4:	OSD_MessageBox(ON, " AEBRIGHT : 4   ");		break;
		case 5:	OSD_MessageBox(ON, " AEBRIGHT : 5   ");		break;		
		case 6:	OSD_MessageBox(ON, " AEBRIGHT : 6   ");		break;
		case 7:	OSD_MessageBox(ON, " AEBRIGHT : 7   ");		break;
		case 8:	OSD_MessageBox(ON, " AEBRIGHT : 8   ");		break;
		case 9:	OSD_MessageBox(ON, " AEBRIGHT : 9   ");		break;
		case 10:	OSD_MessageBox(ON, " AEBRIGHT : 10  ");		break;
		case 11:	OSD_MessageBox(ON, " AEBRIGHT : 11  ");		break;	
		case 12:	OSD_MessageBox(ON, " AEBRIGHT : 12  ");		break;
		case 13:	OSD_MessageBox(ON, " AEBRIGHT : 13  ");		break;
		case 14:	OSD_MessageBox(ON, " AEBRIGHT : 14  ");		break;
		case 15:	OSD_MessageBox(ON, " AEBRIGHT : 15  ");		break;
		case 16:	OSD_MessageBox(ON, " AEBRIGHT : 16  ");		break;
		case 17:	OSD_MessageBox(ON, " AEBRIGHT : 17  ");		break;		
		case 18:	OSD_MessageBox(ON, " AEBRIGHT : 18  ");		break;
		case 19:	OSD_MessageBox(ON, " AEBRIGHT : 19  ");		break;
		case 20:	OSD_MessageBox(ON, " AEBRIGHT : 20  ");		break;	
	}	

}
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeDWDROnOff(void)
{
	DemoDWDROnOff = GetMenuStatus(DWDR_MENU);
	
#if defined(__USE_4K60_INOUT__)
	if(DemoDWDROnOff<1)		DemoDWDROnOff++;
	else					DemoDWDROnOff=0;
#else
	if(DemoDWDROnOff<2)		DemoDWDROnOff++;
	else					DemoDWDROnOff=0;
#endif
	
	DEMO_SetDWDRMode(DemoDWDROnOff);
	
	switch(DemoDWDROnOff)	{
		case 0:	OSD_IrkeyTitleDisplay(OFF, "DWDR. OFF ");	break;
		case 1:	OSD_IrkeyTitleDisplay(ON,  "DWDR. On  ");	break;
		case 2:	OSD_IrkeyTitleDisplay(ON,  "DWDR. Auto");	break;
	}

	SetMenuStatus(DWDR_MENU, DemoDWDROnOff);	
	SetMenuRedraw();		
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeFreezeMirrOnOff(void)
{
	BYTE freeze=0, mirror=0;
	
#if defined(__USE_4K60_INOUT__)
	if(DemoFreezeMirrOnOff < 1)			DemoFreezeMirrOnOff++;
	else if(DemoFreezeMirrOnOff == 1)	DemoFreezeMirrOnOff=3;
	else								DemoFreezeMirrOnOff=0;
#else
	if(DemoFreezeMirrOnOff < 4)		DemoFreezeMirrOnOff++;
	else							DemoFreezeMirrOnOff=0;
#endif
	
	switch(DemoFreezeMirrOnOff)	{
		case 0:	freeze=0;	mirror=0;		break;
		case 1:	freeze=1;	mirror=0;		break;
		case 2:	freeze=0;	mirror=1;		break;
		case 3:	freeze=0;	mirror=2;		break;
		case 4:	freeze=0;	mirror=3;		break;
	}
	
	DEMO_SetFreezeMode(freeze);	SetMenuStatus(DISP_FREEZE_MENU, freeze);	// freeze(off, on)
	DEMO_SetMirrorMode(mirror);	SetMenuStatus(DISP_MIRROR_MENU, mirror);	// mirror(off, h, v, h+v)
	
	switch(DemoFreezeMirrOnOff)	{
		case 0:	OSD_IrkeyTitleDisplay(OFF, "FREEZE:OFF");	break;
		case 1:	OSD_IrkeyTitleDisplay(ON,  "FREEZE: ON");	break;
		case 2:	OSD_IrkeyTitleDisplay(ON,  "MIRROR: H ");	break;
		case 3:	OSD_IrkeyTitleDisplay(ON,  "MIRROR: V ");	break;
		case 4:	OSD_IrkeyTitleDisplay(ON,  "MIRROR:H+V");	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodePmMdOnOff(void)
{
	BYTE pm=0, md=0;
	
	if(GetSYSINFO_BoardType()==MISP_SYSINFO_I550_USB_V10)	{OSD_MessageBox(ON, " Not Available ! ");	return;}
	
	if(DemoPmMdOnOff < 2)		DemoPmMdOnOff++;
	else						DemoPmMdOnOff=0;
	
	switch(DemoPmMdOnOff)	{
		case 0:	pm=0;	md=0;		break;
		case 1:	pm=1;	md=0;		break;
		case 2:	pm=0;	md=1;		break;
	}
	
	SetPM_OnOffMenu(pm);	SetMenuStatus(PM_MODE_MENU, pm);	// pm(off, on)
	SetMD_OnOffMenu(md);	SetMenuStatus(MD_MENU, md);			// md(off, on)
	
	if(md==1 || pm==1)	{	// disable overlap function(pm, md, hlc, blc)
		SetBLC_OnOffMenu(OFF);	SetHLC_OnOffMenu(OFF);	SetMenuStatus(BLIGHT_MENU, OFF);
	}
	
	switch(DemoPmMdOnOff)	{
		case 0:	OSD_IrkeyTitleDisplay(OFF, "PM : OFF  ");	break;
		case 1:	OSD_IrkeyTitleDisplay(ON,  "PM : ON   ");	break;
		case 2:	OSD_IrkeyTitleDisplay(ON,  "MD : ON   ");	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeSysInfo(void)
{
	if(DemoSysInfo)		{DemoSysInfo=OFF;	SetOSDMenuID(0x0000);}
	else				{DemoSysInfo=ON;	SetOSDMenuID(SYS_INFO_MENU);}
}

//--------------------------------------------------------------------------------------------------------------------------
#if defined(__USE_FD_PROCESS__)
static void IrCodeFdProcess(void)
{
	if(GetFD_ProcesMode()) { OSD_IrkeyTitleDisplay(OFF, "FD OFF    "); SetFD_ProcesMode(OFF); }  //ON->OFF
	else                   { OSD_IrkeyTitleDisplay(ON,  "FD ON     "); SetFD_ProcesMode(ON);  }  //OFF->ON
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeStatusDisplay(void)
{
	DemoStatusOSD = (~DemoStatusOSD);
	
	if(DemoStatusOSD)	DEMO_StatusDisplay(ON);
	else				DEMO_StatusDisplay(OFF);
}

BOOL getStatusOSDOnOff(void)
{
	return DemoStatusOSD;
}
/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeSetLensType(void)
{
	DemoLensType = (~DemoLensType);
	
	if(DemoLensType==0)		{OSD_IrkeyTitleDisplay(ON, "LENS:MAN  ");	SaveAELensType(0);}
	else					{OSD_IrkeyTitleDisplay(ON, "LENS:DC   ");	SaveAELensType(1);}
}
*/
/*
static void IrCode5MegaMode(void)
{	

#if defined ( __USE_CMOS_MT9P031__ )
	Turn5Mega^=ON;
	if(Turn5Mega){
		if (Turn2MegaPan) {
			OSD_IrkeyTitleDisplay(OFF, "Panning ON");
			Turn2MegaPan = OFF ;
		}
		Setchange5MegaMode();	
	}
	else{
		Setchange2MegaMode();
	}
#else
		OSD_IrkeyTitleDisplay(ON, "USE_5MCMOS");
#endif
} 
*/
/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCode2MegaPan(void)
{	  
#if defined ( __USE_CMOS_MT9P031__ )
	if ( Turn5Mega == OFF )
		Turn2MegaPan^=ON;
	
	if (Turn2MegaPan) {
		OSD_IrkeyTitleDisplay(ON, "Panning ON");
	}
	else {
		OSD_IrkeyTitleDisplay(OFF, "Panning ON");
	}
#else
		OSD_IrkeyTitleDisplay(ON, "USE_5MCMOS");
#endif
}
*/

//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_EdgeEnhanceOnOff(void)
{
	// BYTE mode = GetMenuStatus(EDGE_MENU);
	
	// if(GetMenuStatus(AUTO_EDGE_MENU)==ON)			{OSD_MessageBox(ON, " Turn off AUTO EDGE!");	return;}
	
	// if(mode>0)		{mode=0;	OSD_IrkeyTitleDisplay(ON, " EDGE OFF ");}
	// else			{mode=10;	OSD_IrkeyTitleDisplay(OFF, " EDGE ON  ");}
	
	// SetMenuStatus(EDGE_MENU, mode);
	// DEMO_SetSharpness(mode);

	BOOL OnOff ;
	
	OnOff = GetIN_MainHoriPeaking();
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveIREdgeOnOff((OnOff)?  OFF:  ON);
#endif
	
	if ( OnOff == ON ) 
	{
		SetIN_MainHoriPeaking(OFF) ;
		SetFC_MainVertPeaking(OFF) ;
		OSD_IrkeyTitleDisplay(ON, " EDGE OFF ");
	}
	else
	{
		SetIN_MainHoriPeaking(ON) ;
		SetFC_MainVertPeaking(ON) ;
		OSD_IrkeyTitleDisplay(OFF, " EDGE ON  ");
	}	
}

/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_NRYcMODE(void)
{
	DEMO_SetNRInputMode();
}
*/
/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_NRWBPreset(void)
{
	AutoConSat_OnOff^=ON;
	
	if(AutoConSat_OnOff)		OSD_IrkeyTitleDisplay(ON, "SAT-AUTO  ");
	else					{	OSD_IrkeyTitleDisplay(OFF, "SAT-MAN   ");
		DEMO_SetContrast(GetMenuStatus(CONTRAST_MENU));
		DEMO_SetSaturation(GetMenuStatus(SATURATION_MENU));
	}
}
*/
/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeSetVCLKOutDly(void)
{
	BYTE delay = GetCLK_VCLKOutDly();
	
	if(delay<3)		delay++;
	else			delay=0;
	
	SetCLK_VCLKOutDly(delay);
	
#if __MISP100_DBGPRT__ == 1
	UARTprintf("IrCodeSetVCLKOutDly : %d \r\n", delay);
#endif
}
*/
/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeDISBayerVect(void)
{
	WORD vect=GetDIS_bayer_vect();
	if(vect<7) SetDIS_bayer_vect(vect+1);
	else       SetDIS_bayer_vect(0);
}
*/
/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeDISBayerVect_Step(void)
{
	if(DIS_Vect_num<3) DIS_Vect_num++;
	else DIS_Vect_num=0;

	DIS_BayerVect_Step(DIS_Vect_num);

}
*/
/*
//--------------------------------------------------------------------------------------------------------------------------
static void DIS_ThresDown(void)
{
	if (DIS_thres>0)
		DIS_thres = DIS_thres - 5;
	
}
*/
//--------------------------------------------------------------------------------------------------------------------------
/*static void DIS_ThresUp(void)
{
	BYTE thres=GetDIS_thres();
	if (thres<40)
		SetDIS_thres(thres+5);
}*/

/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeDISTB(void)
{
	if(DIS_TB_num==0) 	DIS_TB_num=1;
	
	if(DIS_TB_num==1)	{DISTestVect_Gen(1); DIS_TB_num=2;}
	else if(DIS_TB_num==5) {DISTestVect_Gen(5); DIS_TB_num=2;}
	else {DISTestVect_Gen(DIS_TB_num); DIS_TB_num++;}
	
}
*/
	
/*
//--------------------------------------------------------------------------------------------------------------------------
// added by hulee on 20131011
static void IrCode_CVBS_mode(void)
{
	if(CVBSOutSel) 	{CVBSOutSel = 0;	SetOUT_CVBSNTSC960H();	OSD_IrkeyTitleDisplay(ON, "CVBS_960H ");}
	else 			{CVBSOutSel = 1;	SetOUT_CVBSNTSC720H();	OSD_IrkeyTitleDisplay(ON, "CVBS_720H ");}
}
*/
	
// added by kw5210 on 20151016
/*
static void IrCode_Demosaic_mode(void)
{
	if(Demosaic_mode) 	{Demosaic_mode = 0;	Init_DM_REGISTER_DAY();		OSD_IrkeyTitleDisplay(ON, "DEMO_DAY  ");}
	else 				{Demosaic_mode = 1;	Init_DM_REGISTER_NIGHT();	OSD_IrkeyTitleDisplay(ON, "DEMO_NIGHT");}
}
*/

/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_ChromaSuppress(void)
{
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
//	if(ChromaSup_lev < 3)		ChromaSup_lev++;		else	ChromaSup_lev=0;
	if(ChromaSup_lev < 1)		ChromaSup_lev++;		else	ChromaSup_lev=0;
	

//	Demo_SetChromaSuppress(ChromaSup_lev);
	Demo_SetChromaSuppress(0);
	
	Demo_SetDemosaicChromaSuppress(ChromaSup_lev);
		
//	switch(ChromaSup_lev)	{
//		case  0:	OSD_IrkeyTitleDisplay(OFF, "C-SUP: OFF");	break;
//		case  1:	OSD_IrkeyTitleDisplay(ON , "C-SUP: LOW");	break;
//		case  2:	OSD_IrkeyTitleDisplay(ON , "C-SUP: MID");	break;
//		case  3:	OSD_IrkeyTitleDisplay(ON , "C-SUP:HIGH");	break;
//		default:	OSD_IrkeyTitleDisplay(OFF, "C-SUP: OFF");	break;
//	}
		
	switch(ChromaSup_lev)	{
			case  0:	OSD_IrkeyTitleDisplay(OFF, "C-SUP: OFF");	break;
			case  1:	OSD_IrkeyTitleDisplay(ON , "C-SUP:  ON");	break;
			default:	OSD_IrkeyTitleDisplay(OFF, "C-SUP: OFF");	break;
	}
	
#endif
}
*/
/*
//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeSetAudioBypass(BYTE Channel, BOOL OnOff)
{
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550

	OSD_IrkeyTitleDisplay(OnOff, "AUD_BYPASS");


	MISP_RegField(MISP_HOST_ID, 0x03E, 15, 1, OnOff);  	//audio_bypass  <= slv_regs(16#03E#)(15);  -- ¡®1¡¯ bypass  ¡®0¡¯ normal

	//comment exsist mispmemorymap.c function SetDDRMAP_SetAudioDelay 

	MISP_RegField(MISP_HOST_ID, 0x08F, 0, 8, 16+1);  	//audio_bypass  <= slv_regs(16#03E#)(15);  -- ¡®1¡¯ bypass  ¡®0¡¯ normal
	MISP_RegField(MISP_HOST_ID, 0x08F, 8, 8, 32+2);  	//audio_bypass  <= slv_regs(16#03E#)(15);  -- ¡®1¡¯ bypass  ¡®0¡¯ normal	
#endif
}
*/


//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_SSWCaptureOnOff(void)
{
	if(SSWCaptureOnOff) 	{
		SSWCaptureOnOff = OFF;	
		DEMO_SetSensorWrite(OFF);	
		SetNR_Mode(MISP_NR_2D3D);
		SetFC_StopMainFC(OFF);
		OSD_IrkeyTitleDisplay(OFF, "SSW-CAP ON");
	}	// SSW Capture off
	else 					{
		SSWCaptureOnOff = ON;
		SetFC_StopMainFC(ON) ;
		DEMO_SetSensorWrite(ON);	
		SetNR_Mode(MISP_NR_OFF) ;
		OSD_IrkeyTitleDisplay(ON, "SSW-CAP ON");
	}	// SSW Capture on
	
	UARTprintf("IrCode_SSWCaptureOnOff [%s] \r\n", (SSWCaptureOnOff)? "ON": "OFF");
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_SSWStopOnOff(void)
{
	if(SSWStopOnOff) 	{SSWStopOnOff = OFF;	DEMO_SetSensorWstop(OFF);	OSD_IrkeyTitleDisplay(OFF, "SSW-STP ON");}	// SSW Stop off
	else 				{SSWStopOnOff = ON;		DEMO_SetSensorWstop(ON);	OSD_IrkeyTitleDisplay(ON, "SSW-STP ON");}	// SSW Stop on
	
	UARTprintf("IrCode_SSWStopOnOff [%s] \r\n", (SSWStopOnOff)? "ON": "OFF");
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_WBPolyRead(void)
{
	SHORT rgb_cnt, r, g, b, y, u, v;

	rgb_cnt = Get_RGB_CNT();
	r = Get_RGB_R();
	g = Get_RGB_G();
	b = Get_RGB_B();
	y = Get_RGB_Y();
	u = Get_RGB_U();
	v = Get_RGB_V();
	
	UARTprintf("%d - %d,%d,%d / %d,%d,%d\n", rgb_cnt, r, g, b, y, u, v);
}



#if defined(__SENSOR_ERR_CHECK__)
//--------------------------------------------------------------------------------------------------------------------------
static void Sensor_data_checksumStart(void)
{
		MISP_RegWrite(MISP_LOCAL_ID, 0x57A,0x4000);  		//Song's start

		MISP_RegField(MISP_LOCAL_ID, 0x567, 15, 1, 1);  		// clear on
		MISP_RegField(MISP_LOCAL_ID, 0x567, 15, 1, 0);  		// clear off
		Delay_mSec(100);
	//	MISP_RegWrite(MISP_LOCAL_ID, 0x566, 2016);
	//	MISP_RegWrite(MISP_LOCAL_ID, 0x567, 1108);

		sensor_err_en=ON;
		pre_sensor_herr=0, pre_sensor_verr=0;	
		UARTprintf("\r\n [data_checksumStart] \r\n");
}
#endif	// __SENSOR_ERR_CHECK__

//--------------------------------------------------------------------------------------------------------------------------
/* do not delet this comment. because adc test complated code
static void IrCodeADCTestOneTime(void)
{
	BOOL EOC;
	
	DEMO_SET_ADCSAM(8);
	DEMO_SET_ADCCON(1);
	DEMO_SET_ADC_Channel(1);
	
	DEMO_ADC_Data_Read(EOC);
	
}

static void IrCodeADCTestBurst(void)
{
	BOOL EOC;
	
	DEMO_SET_ADCSAM(8);
	DEMO_SET_ADCCON(2);
	DEMO_SET_ADC_Channel(1);
	
	while(1){
		EOC = DEMO_ADC_EOC_Check();
		DEMO_ADC_Data_Read(EOC);	
	}

}

static void IrCodeADCSAM(void)
{
	if(ADCSAM==8) ADCSAM=0;
	else ADCSAM++;

	DEMO_SET_ADCSAM(ADCSAM);
}


*/

//--------------------------------------------------------------------------------------------------------------------------
static void IrCodeFactoryReset(BOOL OnOff)
{
	if(OnOff)		FResetCnt++;
	else			FResetCnt=0;
	
	if(FResetCnt >= 3)	{			// press remote key '3 times'
		FResetCnt=0;
		
		if(OSD_GetMenuLang()==0)	OSD_MessageBox(ON, " Factory Reset !");
		else						OSD_MessageBox(ON, (PBYTE)msg_chn_factory_reset);
		
		UARTprintf("Factory Reset !! \r\n");
		DEMO_SetFactoryReset();
	}
}


BOOL fTestEn = FALSE;
BOOL fNewEn = TRUE;

extern AE_CTRL_PARAM stAECTRL;

void AF_New_Enable(BOOL onoff)
{
	if(onoff == TRUE) {
		fNewEn = TRUE;
	} else {
		fNewEn = FALSE;
	}
}

void mimi_test_func1(void)
{
#if 1
	if(tNewFlag == TRUE) {
		tNewFlag = FALSE;
	} else {
		tNewFlag = TRUE;
	}
	AF_New_Enable(tNewFlag);
#else
	#ifdef __P_IRIS_TEST__
		AE_P_IRIS_Close();
	#endif
#endif
}

#if 0
void OV2710_test()
{
#if defined(__USE_CMOS_OV2710__)    // mdin-i51x
	SetAECtrlOnOff (OFF);

	if(OV2710_INNER_AE==OFF){
		OSD_IrkeyTitleDisplay(ON, "SENS-IN AE");
		CMOS_ByteWrite(I2C_OV2710_ADDR,0x3503,0x00);
		OV2710_INNER_AE=ON;	
	}
	else{
		OSD_IrkeyTitleDisplay(OFF, "SENS-AEOFF");
		CMOS_ByteWrite(I2C_OV2710_ADDR,0x3503,0x07);
		OV2710_INNER_AE=OFF;			
	}
#endif

/*	BYTE Buff;

	CMOS_MultiRead(I2C_OV2710_ADDR, 0x503D, &Buff, 1);
	if(fSqr)	Buff |= 0x80;
	else		Buff &= 0x7F;
	UARTprintf("OV2710(0x503D = %x\n\r", Buff);
	CMOS_MultiWrite(I2C_OV2710_ADDR, 0x503D, &Buff, 1);

	CMOS_MultiRead(I2C_OV2710_ADDR, 0x503D, &Buff, 1);
	UARTprintf("OV2710(0x503D = %x\n\r", Buff);

	if(fSqr) fSqr = FALSE;
	else		fSqr = TRUE;
*/
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
BOOL dGainFlag = FALSE;
int	dGainVal = 0;
int	digitalGainVal = 0;
int	dGainInt = 256;
int	dGainCnt = 0;
int	afDebugMode = 0;
int	afDispCnt = 0;
void ext_test()
{
	CMOS_WordWrite(I2C_EXT1_ADDR, 0x000F, (WORD)dGainCnt);
	dGainCnt = dGainCnt + 100;
	if (dGainCnt > 1104) {
		CMOS_WordWrite(I2C_EXT1_ADDR, 0x000E, dGainCnt);
	}else {
		CMOS_WordWrite(I2C_EXT1_ADDR, 0x000E, 0);
	}
	UARTprintf("shutter : %d\r\n",  (dGainCnt));
}

#if defined(__USE_AFZOOM_LENS__)
 void AF_TestMode()
{
	#if defined(AF_NEW_FILE)
	if(AF_GetTrMode() < 3)	AF_SetTrMode(AF_GetTrMode()+1);
	else					AF_SetTrMode(0);
	switch(AF_GetTrMode()) {
		case 1 : UARTprintf("AF Force Normal Mode...\r\n"); OSD_IrkeyTitleDisplay(ON, "AF Normal "); break;
		case 2 : UARTprintf("AF Force Dark Mode...\r\n"); OSD_IrkeyTitleDisplay(ON, "AF Dark   "); break;
		case 3 : UARTprintf("AF Force Spot Mode...\r\n"); OSD_IrkeyTitleDisplay(ON, "AF Spot   "); break;
		default : UARTprintf("AF Auto Mode...\r\n"); OSD_IrkeyTitleDisplay(OFF, "AF Auto   "); break;
	}
	#endif
}
#endif

void histogram_test() // histogram
{
	WORD tBuf[2];
	WORD cntA;
	WORD af_max;
#if 0
	int x;
	DWORD histO[64];

	Histogram_Read(0, 64, histO);

	for(x=0;x<64;x++)
		UARTprintf("%3d ", histO[x]);
	UARTprintf("\r\n");
#endif

	MISP_RegRead(MISP_LOCAL_ID, 0x6A5, &af_max);
	UARTprintf("af max : %d\r\n", af_max);
	MISP_MultiRead(MISP_LOCAL_ID, 0x6e8, (PBYTE)tBuf, 4);
	cntA = MAKEDWORD(tBuf[0], tBuf[1]);
	UARTprintf("cntA : %d\r\n", cntA);

}

void af_win_display(void)
{
//#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	WORD wBuf[4];
	WORD odmSH, odmSV;
	WORD stepH, stepV;

#if defined(__USE_4K60_INOUT__)
	return;
#endif
	
	MISP_RegRead(MISP_LOCAL_ID, 0x60D, &wBuf[0]);
	MISP_RegRead(MISP_LOCAL_ID, 0x60E, &wBuf[1]);
	odmSH = wBuf[0]; odmSV = wBuf[1];
	UARTprintf("\r\nODM Window start(h:%d / v:%d)...\r\n", wBuf[0], wBuf[1]);
	MISP_RegRead(MISP_LOCAL_ID, 0x60F, &wBuf[0]);
	stepH = (wBuf[0]>>8); stepV = (wBuf[0]&0xFF);
	UARTprintf("ODM Window step(h:%d / v:%d)...\r\n", (wBuf[0]>>8), (wBuf[0]&0xFF));

	switch(afDispCnt) {
		case 0 : afDispCnt = 1;
			MISP_RegRead(MISP_LOCAL_ID, 0x680, &wBuf[0]);
			MISP_RegRead(MISP_LOCAL_ID, 0x681, &wBuf[1]);
			UARTprintf("AE Window start(h:%d / v:%d)...\r\n", (wBuf[0]>>8), (wBuf[0]&0xFF));
			UARTprintf("AE Window size(h:%d / v:%d)...\r\n", (wBuf[1]>>8), (wBuf[1]&0xFF));
			UARTprintf("AE Window real size(w:%d / h:%d)...\r\n", (wBuf[1]>>8)*stepH, (wBuf[1]&0xFF)*stepV);
			OSD_IrkeyTitleDisplay(ON, "AE Window ");
		break;
		case 1 : afDispCnt = 2;
			MISP_RegRead(MISP_LOCAL_ID, 0x610, &wBuf[0]);
			MISP_RegRead(MISP_LOCAL_ID, 0x611, &wBuf[1]);
			UARTprintf("AWB Window start(h:%d / v:%d)...\r\n", (wBuf[0]>>8), (wBuf[0]&0xFF));
			UARTprintf("AWB Window size(h:%d / v:%d)...\r\n", (wBuf[1]>>8), (wBuf[1]&0xFF));
			UARTprintf("AWB Window real size(w:%d / h:%d)...\r\n", (wBuf[1]>>8)*stepH, (wBuf[1]&0xFF)*stepV);
			OSD_IrkeyTitleDisplay(ON, "AWB Window");
		break;
		case 2 : afDispCnt = 4;
			UARTprintf("AWB RAW Window...\r\n");
			OSD_IrkeyTitleDisplay(ON, "RAW Window");
		break;
		case 4 : afDispCnt = 8;
			MISP_RegRead(MISP_LOCAL_ID, 0x6A0, &wBuf[0]);
			MISP_RegRead(MISP_LOCAL_ID, 0x6A1, &wBuf[1]);
			UARTprintf("AF Window0 start(h:%d / v:%d)...\r\n", (wBuf[0]>>8), (wBuf[0]&0xff));
			UARTprintf("AF Window0 size(h:%d / v:%d)...\r\n", (wBuf[1]>>8), (wBuf[1]&0xff));
			UARTprintf("AF Window real size(w:%d / h:%d)...\r\n", (wBuf[1]>>8)*stepH, (wBuf[1]&0xff)*stepV);
			OSD_IrkeyTitleDisplay(ON, "AF Window0");
 		break;

		case 8 : afDispCnt = 16;
 			MISP_RegRead(MISP_LOCAL_ID, 0x6A2, &wBuf[0]);
			MISP_RegRead(MISP_LOCAL_ID, 0x6A3, &wBuf[1]);
			UARTprintf("AF Window1 start(h:%d / v:%d)...\r\n", (wBuf[0]>>8), (wBuf[0]&0xff));
			UARTprintf("AF Window1 size(h:%d / v:%d)...\r\n", (wBuf[1]>>8), (wBuf[1]&0xff));
			UARTprintf("AF Window real size(w:%d / h:%d)...\r\n", (wBuf[1]>>8)*stepH, (wBuf[1]&0xff)*stepV);
			OSD_IrkeyTitleDisplay(ON, "AF Window1");
		break;

		default : afDispCnt = 0;
			UARTprintf("Window Off\r\n");
			OSD_IrkeyTitleDisplay(OFF, "          ");
		break;
	}

	MISP_RegWrite(MISP_LOCAL_ID, 0x752, afDispCnt);
//#endif
}

BOOL AE_GAIN_TEST = 0;
BOOL AE_DGAIN_TEST = 0;
static DWORD cGainBak=0;
extern int AE_GaindB;
void AE_get_cmos_gain()
{
#if 1
#if 1
	DWORD gainT;

	for(gainT=256;gainT<0x20000;gainT++) {
		if(AECTRL_CnvGAINtoCMOS(gainT) != cGainBak)
		//UARTprintf("gain : %d / %d\r\n",gainT,AECTRL_CnvGAINtoCMOS(gainT));
		cGainBak = AECTRL_CnvGAINtoCMOS(gainT);
	}
#else
	char gainC[11];

	//GetNR_StDEV(&StDev_int, &StDev_frac, 0);
	//StDev = (WORD)((FLOAT)StDev_int*10.0f+(FLOAT)StDev_frac*1.25f);

	stAEXFER.gain = 10*digitalGainVal/3;

	sprintf(gainC, "%5d[dB]", digitalGainVal);
	OSD_IrkeyTitleDisplay(ON, gainC);
	
	digitalGainVal++;
	if(digitalGainVal == 72) digitalGainVal = 0;

	AE_GAIN_TEST = 1;
#endif
#else
	DWORD cgain;

	cgain = AECTRL_CnvGAINtoCMOS(0x20000);
	UARTprintf("0x20000 / cmos gain : %d\r\n", cgain);

	cgain = AECTRL_CnvGAINtoCMOS(0x20000/2);
	UARTprintf("0x10000 / cmos gain : %d\r\n", cgain);

	cgain = AECTRL_CnvGAINtoCMOS(0x20000/4);
	UARTprintf("0x8000 / cmos gain : %d\r\n", cgain);

	cgain = AECTRL_CnvGAINtoCMOS(0x20000/8);
	UARTprintf("0x4000 / cmos gain : %d\r\n", cgain);
#endif
}

void AE_digital_gain_test(void)
{
	LONG mode=AECTRL_GAINCTL_MAX;
#if 1
	if(digitalGainVal < 3) {
		digitalGainVal ++;
	} else {
		digitalGainVal = 0;
	}

	if (digitalGainVal==0)		mode = AECTRL_GAINCTL_MAX;
	else if (digitalGainVal==1)		mode = AECTRL_GAINCTL_DMIN;
	else	 if (digitalGainVal==2)		mode = AECTRL_GAINCTL_DMID;
	else	 if (digitalGainVal==3)		mode = AECTRL_GAINCTL_DMAX;
	else mode = AECTRL_GAINCTL_AUTO;

	stAECTRL.ctrl &= (~AECTRL_GAIN_MASK);
	stAECTRL.ctrl |= (mode);

	switch(digitalGainVal) {
		case 0 : AECTRL_SetGAINMax(0x20000+0x20000*0/8); break;
		case 1 : AECTRL_SetGAINMax(0x20000+0x20000*2/8); break; // 3X
		case 2 : AECTRL_SetGAINMax(0x20000+0x20000*5/8); break; // 6X
		case 3 : AECTRL_SetGAINMax(0x20000+0x20000*8/8); break; // 9X
	}

	//UARTprintf("dGain mode : X%d\r\n", digitalGainVal*3);
#if 0
	dGainFlag = TRUE;
	dGainCnt = (dGainCnt+1)%2;

	if(dGainCnt == 0) {
		//dGainInt = (dGainInt+100)%256;
		dGainInt += 10;
		dGainVal = dGainInt;

		CMOS_WordWrite(I2C_EXT1_ADDR, 0x0023,	5);
		CMOS_WordWrite(I2C_EXT1_ADDR, 0x0024,	5);
	} else {
		dGainVal = 256;

		CMOS_WordWrite(I2C_EXT1_ADDR, 0x0023,	6);
		CMOS_WordWrite(I2C_EXT1_ADDR, 0x0024,	6);
	}

	MISP_RegWrite(MISP_LOCAL_ID, 0x601, LOWORD(dGainVal));
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_AEDGAIN, dGainVal);
#endif
	
	UARTprintf("dGain = %d\r\n", dGainVal);
#endif
#else
    #if 1
	#if 1
		//if(f_dGain) f_dGain = FALSE;
		//else		f_dGain = TRUE;

		if(fSqr) fSqr = FALSE;
		else		fSqr = TRUE;

		AECTRL_SetBrightness(&stAECTRL, stAECTRL.level);


		#define AE_SQR_FLAG0	0x0400
		#define AE_SQR_FLAG1	0x8000
		WORD pBuf;
		MISP_RegRead(MISP_LOCAL_ID, 0x600, &pBuf);

		if(fSqr) pBuf = pBuf | AE_SQR_FLAG0;
		else pBuf = pBuf &(~AE_SQR_FLAG0);

		MISP_RegWrite(MISP_LOCAL_ID, 0x600, pBuf);

		UARTprintf("%x\r\n", pBuf);

		MISP_RegRead(MISP_LOCAL_ID, 0x60C, &pBuf);

		if(fSqr) pBuf = pBuf | AE_SQR_FLAG1;
		else pBuf = pBuf &(~AE_SQR_FLAG1);

		MISP_RegWrite(MISP_LOCAL_ID, 0x60C, pBuf);

		UARTprintf("%x\r\n", pBuf);
		
		#if 0
			//if(fDGen) fDGen = FALSE;
			//else		fDGen = TRUE;
			if(f_dGain) f_dGain = FALSE;
			else		f_dGain = TRUE;
		#endif
		#if 0
			if(tTestFlag == TRUE) {
				tTestFlag = FALSE;
			} else {
				tTestFlag = TRUE;
			}
		#endif
	#else
		if(tTestFlag == FALSE) {
			setAEWindowK(TRUE);
			tTestFlag = TRUE;
		} else {
			tTestFlag = FALSE;
			setAEWindowK(FALSE);
		}
	#endif
    #else
	//AECTRL_setSlowAE(2);
	//IrCodeAFFilterSelect();
	#ifdef __P_IRIS_TEST__
		AE_P_IRIS_Open();
	#else
		if(tTestFlag == FALSE) {
			setSpotAEOnOff(TRUE);
			tTestFlag = TRUE;
			setSportAEPostion(8, 8);
		} else {
			setSpotAEOnOff(FALSE);
			tTestFlag = FALSE;
		}
	#endif
    #endif
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetIRCodeNewCmdAF(void)
{
	SetMsgIrCodeNewCmd(IRCODE_FOCUS);
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL ISPM IsIrCodeCommandZOOM(void)
{
	switch (IrComCommand) {
		case IRCODE_FOCUS_FAR:	case IRCODE_PVR_MARK:
		case IRCODE_FOCUS_NEAR:	case IRCODE_PVR_PLIST:		// FOKS-Key

		case IRCODE_ZOOM_TELE:	case IRCODE_FAV_UP_2000:
		case IRCODE_ZOOM_WIDE:	case IRCODE_FAV_DN_2000:	// ZOOM-Key
					return TRUE;
		default:	return FALSE;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_DirUControl(void)
{
#if defined (__USE_CMOS_MT9P031__)
	if (fZOOMMove && (GetOSDMenuID()==0))	{IrCodeZoomMoveDirU(); SetDZOOM_RepeatKeyEnable();}
	else if (Turn2MegaPan) {IrCode2MegaMoveDirU(); SetDZOOM_RepeatKeyEnable(); }
	else if (fMainCrop)	IrCodeCropMoveDirU();
	else if (GetWBRAWOnOffMODE()) SetWBRAWViewShiftU();
	else					DirUMessageControl();  
#else
	if (fZOOMMove && (GetOSDMenuID()==0))	{ IrCodeZoomMoveDirU(); SetDZOOM_RepeatKeyEnable(); }
	else if (fMainCrop)	{IrCodeCropMoveDirU();SetDZOOM_RepeatKeyEnable();}
#if defined(__USE_SENS_PANNING_MODE__)
	else if (fSensorPanningMove && (GetOSDMenuID()==0)) {IrCodePanningMoveDirU();SetDZOOM_RepeatKeyEnable();}
#endif
	else if (GetWBRAWOnOffMODE()) SetWBRAWViewShiftU();
	else					DirUMessageControl();  
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_DirDControl(void)
{
#if defined (__USE_CMOS_MT9P031__)
	if		(fZOOMMove && (GetOSDMenuID()==0)) {IrCodeZoomMoveDirD(); SetDZOOM_RepeatKeyEnable();}
	else if (Turn2MegaPan){IrCode2MegaMoveDirD();SetDZOOM_RepeatKeyEnable();}
	else if (fMainCrop)	IrCodeCropMoveDirD();
	else if (GetWBRAWOnOffMODE()) SetWBRAWViewShiftD();
	else					DirDMessageControl();
#else
	if		(fZOOMMove && (GetOSDMenuID()==0)) { IrCodeZoomMoveDirD();SetDZOOM_RepeatKeyEnable();}
	else if (fMainCrop)	{ IrCodeCropMoveDirD();SetDZOOM_RepeatKeyEnable();}
#if defined(__USE_SENS_PANNING_MODE__)	
	else if (fSensorPanningMove && (GetOSDMenuID()==0)) { IrCodePanningMoveDirD();SetDZOOM_RepeatKeyEnable();}
#endif	
	else if (GetWBRAWOnOffMODE()) SetWBRAWViewShiftD();
	else					DirDMessageControl();
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_DirLControl(void)
{
#if defined (__USE_CMOS_MT9P031__)
	if		(fZOOMMove && (GetOSDMenuID()==0))	{IrCodeZoomMoveDirL(); SetDZOOM_RepeatKeyEnable(); }
	else if (Turn2MegaPan) {IrCode2MegaMoveDirL(); SetDZOOM_RepeatKeyEnable();}
	else if (fMainCrop)	IrCodeCropMoveDirL();
	else if (GetWBRAWOnOffMODE()) SetWBRAWViewShiftL();
	else					DirLMessageControl();
#else
	if		(fZOOMMove && (GetOSDMenuID()==0))	{IrCodeZoomMoveDirL(); SetDZOOM_RepeatKeyEnable(); }
	else if (fMainCrop)	{IrCodeCropMoveDirL();SetDZOOM_RepeatKeyEnable();}
	else if (fDWindowMove)	{IrCodeDWindowMoveDirL();	SetDZOOM_RepeatKeyEnable();}
#if defined(__USE_SENS_PANNING_MODE__)	
	else if (fSensorPanningMove && (GetOSDMenuID()==0)) {IrCodePanningMoveDirL();SetDZOOM_RepeatKeyEnable();}
#endif
	else if (GetWBRAWOnOffMODE()) SetWBRAWViewShiftL();
	else					DirLMessageControl();
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_DirRControl(void)
{
#if defined (__USE_CMOS_MT9P031__)
	if		(fZOOMMove && (GetOSDMenuID()==0))	{IrCodeZoomMoveDirR(); SetDZOOM_RepeatKeyEnable();}
	else if (Turn2MegaPan) {IrCode2MegaMoveDirR(); SetDZOOM_RepeatKeyEnable();}
	else if (fMainCrop)	IrCodeCropMoveDirR();
	else if (GetWBRAWOnOffMODE()) SetWBRAWViewShiftR();
	else					DirRMessageControl();
#else
	if		(fZOOMMove && (GetOSDMenuID()==0))	{ IrCodeZoomMoveDirR(); SetDZOOM_RepeatKeyEnable(); }
	else if (fMainCrop)	{IrCodeCropMoveDirR();SetDZOOM_RepeatKeyEnable();}
	else if (fDWindowMove)	{IrCodeDWindowMoveDirR();	SetDZOOM_RepeatKeyEnable();}
#if defined(__USE_SENS_PANNING_MODE__)	
	else if (fSensorPanningMove && (GetOSDMenuID()==0)) {IrCodePanningMoveDirR();SetDZOOM_RepeatKeyEnable();}
#endif	
	else if (GetWBRAWOnOffMODE()) SetWBRAWViewShiftR();
	else					DirRMessageControl();
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void IrCode_ExitMenu(BOOL OnOff)
{
	if(OnOff)	MenuMessageExit();
	
	// check secret debug menu ir key
	if(OnOff)		DebugMenuCnt++;
	else			DebugMenuCnt=0;
	
	if(DebugMenuCnt >= 5)	{			// when press remote key '5 times'
		DebugMenuCnt=0;
		SetOSDMenuID(DEBUGMODE_MENU);	// debug menu 'on'
	}
}

//--------------------------------------------------------------------------------------------------------------------------
#if defined ( __USE_CMOS_MT9P031__ )
static void IrCode2MegaMoveDirU(void)
{
	 AptinaPosV = AptinaPosV-10 ;
	 CMOS_WordWrite(I2C_MT9P031_ADDR, 0x01, AptinaPosV);
}

static void IrCode2MegaMoveDirD(void)
{
	 AptinaPosV = AptinaPosV+10 ;	
	 CMOS_WordWrite(I2C_MT9P031_ADDR, 0x01, AptinaPosV);
}

static void IrCode2MegaMoveDirL(void)
{
	
	AptinaPosH = AptinaPosH-10 ;
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x02, AptinaPosH);
}

static void IrCode2MegaMoveDirR(void)
{
	 AptinaPosH = AptinaPosH+10 ;
	 CMOS_WordWrite(I2C_MT9P031_ADDR, 0x02, AptinaPosH);
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
void IrkeyEnableOnOff(BOOL OnOff)
{
	IR_KEY_ENABLE = (OnOff)?  1:  0;
}

#if defined(__USE_IRKEY_SWITCH__) //|| defined(__USE_KEY_TACK_SW__)
//--------------------------------------------------------------------------------------------------------------------------
void ISPM IrkeyEnableToggle(void)
{
#if defined(__USE_IRKEY_SWITCH__)
	if(IR_KEY_ENABLE)		IR_KEY_ENABLE = 0;
	else 					IR_KEY_ENABLE = 1;

	enable_interrupt(INTNUM_GPIOA,FALSE);	// disable 'GPIO-0 IRQ'
	IR_KEY_IRQ_CNT = 1;						// start 'GPIO-0 IRQ' enable counter	
#endif	

#if defined(__USE_KEY_TACK_SW__)
	//DE_485(HIGH);
	//Delay_uSec(500);	
	//DE_485(LOW);
#endif		
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM IrSwitchIrqCounter(void)
{
	if(IR_KEY_IRQ_CNT==0)		return;
	
	if(IR_KEY_IRQ_CNT++ > 120)	{				// wait delay 2000 ms
		*R_P0EDS = (1<<3);						// clear irq que
		
		enable_interrupt(INTNUM_GPIOA,TRUE);	// enable 'GPIO-0 IRQ'
		IR_KEY_IRQ_CNT = 0;						// stop 'GPIO-0 IRQ' enable counter
	}
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
void IrCodeSetIrComCommand(WORD code)
{
	IrComCommand = code;
	IrCodeMessageExecution();
}

//--------------------------------------------------------------------------------------------------------------------------
void IRCodeExeDemo(WORD val)	// for promotion ir key(menu & arrow key + some function key only)
{
	switch(val)	{				// for promotion ir key(menu & arrow key + some function key only)
		case IRCODE_MENU:			MenuMessageControl(); 		break; 
		case IRCODE_EXIT:			MenuMessageExit(); 			break;
		case IRCODE_DIR_RT:			IrCode_DirRControl();		break;
		case IRCODE_DIR_LT:			IrCode_DirLControl();		break;
		case IRCODE_DIR_UP:			IrCode_DirUControl();		break;
		case IRCODE_DIR_DN:			IrCode_DirDControl();		break;
		case IRCODE_ENTER:			EnterMessageControl(); 		break;
 		case IRCODE_VOL_UP_2000:	case IRCODE_VOL_UP:		IrCodeDZoomIN();	SetDZOOM_RepeatKeyEnable();	break;
		case IRCODE_VOL_DN_2000:	case IRCODE_VOL_DN:		IrCodeDZoomOUT();	SetDZOOM_RepeatKeyEnable();	break;
		case IRCODE_INFO:		IrCodeStatusDisplay(); break;
		case IRCODE_STATUS:		IrCodePIPCtrlOnOff(); break;
		case IRCODE_CC:			IrCodePIPCHSEL(); break;
		case IRCODE_TTX :		IrCodeVOUTToggle(); break;

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
		case IRCODE_RADIO:		break;		
		case IRCODE_MUTE:		//LENS_FocusMove(0,0);			break;
								IrCodeFOKSDrvDirN(); if (GetDIS_OnOff()) IrCodeDISOnOff(); break;
		case IRCODE_ADDEL:		//LENS_FocusMove(0,1);			break;
								IrCodeFOKSDrvDirF(); if (GetDIS_OnOff()) IrCodeDISOnOff(); break;
		case IRCODE_FAV_UP_2000:
		case IRCODE_FAV_UP:		//LENS_FocusMove(1,0);			break;
								IrCodeZOOMDrvDirT(); if (GetDIS_OnOff()) IrCodeDISOnOff(); break;
		case IRCODE_FAV_DN_2000:
		case IRCODE_FAV_DN:		//LENS_FocusMove(1,1);			break;
								IrCodeZOOMDrvDirW(); if (GetDIS_OnOff()) IrCodeDISOnOff(); break;
#endif

#if defined(__PANORAMA_SETUP__)		

#if __PANORAMA_5x0__
	#if defined(__MDIN_APP_PANO_)
		case IRCODE_PANO_CRP_LX_U:	IrCode_Pano_Control_MDINAPP(0);	break;
		case IRCODE_PANO_CRP_LX_D:	IrCode_Pano_Control_MDINAPP(1);	break;
		case IRCODE_PANO_CRP_RX_U:	IrCode_Pano_Control_MDINAPP(2);	break;
		case IRCODE_PANO_CRP_RX_D:	IrCode_Pano_Control_MDINAPP(3);	break;
		case IRCODE_PANO_CRP_TY_U:	IrCode_Pano_Control_MDINAPP(4);	break;
		case IRCODE_PANO_CRP_TY_D:	IrCode_Pano_Control_MDINAPP(5);	break;
		case IRCODE_PANO_CRP_BY_U:	IrCode_Pano_Control_MDINAPP(6);	break;
		case IRCODE_PANO_CRP_BY_D:	IrCode_Pano_Control_MDINAPP(7);	break;

		case IRCODE_PANO_EEPROM_SAVE:	SetApp_Pano_WriteEEPROM();		break;
		case IRCODE_PANO_EEPROM_DEL:	SetApp_Pano_ResetEEPROM();		break;

	#endif
#else
		case IRCODE_NUM_0:		SetApp_Pano_WriteEEPROM();			break;
		case IRCODE_NUM_5:		IrCode_PanoCtrlSelectSensor();			break;
		case IRCODE_NUM_6:		IrCode_PanoCtrlSelectAdjust();			break;
		case IRCODE_NUM_8:		SetApp_Pano_Control(1);	SetMsgCommandRepeatKey(MSG_LEVEL); break;
		case IRCODE_NUM_9:		SetApp_Pano_Control(0);	SetMsgCommandRepeatKey(MSG_LEVEL); break;
		case IRCODE_SHARP:		SetApp_Pano_ResetEEPROM();		break;

#if defined(__MDIN_APP_PANO_)
		//case IRCODE_PANO_SS_SEL:		IrCode_PanoCtrlSelectSensor();			break;
		//case IRCODE_PANO_ADJ_SEL:		IrCode_PanoCtrlSelectAdjust();			break;
		//case IRCODE_PANO_INC:			SetApp_Pano_Control(1);	SetMsgCommandRepeatKey(MSG_LEVEL); break;
		//case IRCODE_PANO_DEC:			SetApp_Pano_Control(0);	SetMsgCommandRepeatKey(MSG_LEVEL); break;

		case IRCODE_PANO_CRP_LX_U:	IrCode_Pano_Control_MDINAPP(0);	break;
		case IRCODE_PANO_CRP_LX_D:	IrCode_Pano_Control_MDINAPP(1);	break;
		case IRCODE_PANO_CRP_RX_U:	IrCode_Pano_Control_MDINAPP(2);	break;
		case IRCODE_PANO_CRP_RX_D:	IrCode_Pano_Control_MDINAPP(3);	break;
		case IRCODE_PANO_CRP_TY_U:	IrCode_Pano_Control_MDINAPP(4);	break;
		case IRCODE_PANO_CRP_TY_D:	IrCode_Pano_Control_MDINAPP(5);	break;
		case IRCODE_PANO_CRP_BY_U:	IrCode_Pano_Control_MDINAPP(6);	break;
		case IRCODE_PANO_CRP_BY_D:	IrCode_Pano_Control_MDINAPP(7);	break;

		case IRCODE_PANO_SS0:	IrCode_PanoCtrlSelectSensor_MDINAPP(0);	break;
		case IRCODE_PANO_SS1:	IrCode_PanoCtrlSelectSensor_MDINAPP(1);	break;
		case IRCODE_PANO_SS2:	IrCode_PanoCtrlSelectSensor_MDINAPP(2);	break;
		case IRCODE_PANO_SS3:	IrCode_PanoCtrlSelectSensor_MDINAPP(3);	break;
		case IRCODE_PANO_BLEND:	IrCode_PanoCtrlSelectSensor_MDINAPP(4);	break;

		case IRCODE_PANO_EEPROM_SAVE:	SetApp_Pano_WriteEEPROM();		break;
		case IRCODE_PANO_EEPROM_DEL:	SetApp_Pano_ResetEEPROM();		break;

		case IRCODE_PANO_DISP_MODE:		IrCode_Pano_Control_MDINAPP(8);	break;
		case IRCODE_PANO_BLEND_AREA:	IrCode_Pano_Control_MDINAPP(9);	break;
		case IRCODE_PANO_BLEND_LEVEL:	IrCode_Pano_Control_MDINAPP(10);break;
#endif

#endif
		
#endif
		
		default:				SetMsgCommandComplete(ON); 		break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void IrCodeMessageExecution(void)
{
#if defined(__USE_IRKEY_DEMO__)
	if(IR_KEY_ENABLE == 0)		{IRCodeExeDemo(IrComCommand);	return;}
#else
	if (IR_KEY_ENABLE == 0)	 return;			// check ir enable button
#endif


#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	if (GetAgeOnOffLENS()==TRUE) SetMsgCommandComplete(ON);
	if (GetAgeOnOffLENS()==TRUE) return;
#endif

	if (GetWBRAWOnOffTIME()==TRUE) SetMsgCommandComplete(ON);
	if (GetWBRAWOnOffTIME()==TRUE) return;

#if defined(__USE_IRDATA_SLAVE2MASER__) && defined(__USE_CPU_SPI_SLAVE__)
	gIrComCommand = IrComCommand;
	gIRComCommand_on = TRUE;
#endif

	switch (IrComCommand) {
		case IRCODE_MENU:			MenuMessageControl(); 		break; 
		case IRCODE_DIR_RT:			IrCode_DirRControl();		break;
		case IRCODE_DIR_LT:			IrCode_DirLControl();		break;
		case IRCODE_DIR_UP:			IrCode_DirUControl();		break;
		case IRCODE_DIR_DN:			IrCode_DirDControl();		break;
		case IRCODE_ENTER:			EnterMessageControl(); 		break;
		case IRCODE_EXIT:			IrCode_ExitMenu(ON); 		break;
		
#if defined(__PANORAMA_SETUP__)		
		
#if __PANORAMA_5x0__
	#if defined(__MDIN_APP_PANO_)
		case IRCODE_PANO_CRP_LX_U:	IrCode_Pano_Control_MDINAPP(0);	break;
		case IRCODE_PANO_CRP_LX_D:	IrCode_Pano_Control_MDINAPP(1);	break;
		case IRCODE_PANO_CRP_RX_U:	IrCode_Pano_Control_MDINAPP(2);	break;
		case IRCODE_PANO_CRP_RX_D:	IrCode_Pano_Control_MDINAPP(3);	break;
		case IRCODE_PANO_CRP_TY_U:	IrCode_Pano_Control_MDINAPP(4);	break;
		case IRCODE_PANO_CRP_TY_D:	IrCode_Pano_Control_MDINAPP(5);	break;
		case IRCODE_PANO_CRP_BY_U:	IrCode_Pano_Control_MDINAPP(6);	break;
		case IRCODE_PANO_CRP_BY_D:	IrCode_Pano_Control_MDINAPP(7);	break;

		case IRCODE_PANO_EEPROM_SAVE:	SetApp_Pano_WriteEEPROM();		break;
		case IRCODE_PANO_EEPROM_DEL:	SetApp_Pano_ResetEEPROM();		break;

	#endif
#else
		case IRCODE_AE_MODE:		SetApp_Pano_WriteEEPROM();		break;
		case IRCODE_DEFOG:			IrCode_PanoCtrlSelectSensor();	break;
		case IRCODE_DIS:			IrCode_PanoCtrlSelectAdjust();	break;
		case IRCODE_DWDR:			SetApp_Pano_Control(1);	SetMsgCommandRepeatKey(MSG_LEVEL); break;
		case IRCODE_AE_BRIGHT:		SetApp_Pano_Control(0);	SetMsgCommandRepeatKey(MSG_LEVEL); break;
		case IRCODE_AE_DARK:		SetApp_Pano_ResetEEPROM();	break;

#if defined(__MDIN_APP_PANO_)
		//case IRCODE_PANO_SS_SEL:		IrCode_PanoCtrlSelectSensor();			break;
		//case IRCODE_PANO_ADJ_SEL:		IrCode_PanoCtrlSelectAdjust();			break;
		//case IRCODE_PANO_INC:			SetApp_Pano_Control(1);	SetMsgCommandRepeatKey(MSG_LEVEL); break;
		//case IRCODE_PANO_DEC:			SetApp_Pano_Control(0);	SetMsgCommandRepeatKey(MSG_LEVEL); break;
		
		case IRCODE_PANO_CRP_LX_U:	IrCode_Pano_Control_MDINAPP(0);	break;
		case IRCODE_PANO_CRP_LX_D:	IrCode_Pano_Control_MDINAPP(1);	break;
		case IRCODE_PANO_CRP_RX_U:	IrCode_Pano_Control_MDINAPP(2);	break;
		case IRCODE_PANO_CRP_RX_D:	IrCode_Pano_Control_MDINAPP(3);	break;
		case IRCODE_PANO_CRP_TY_U:	IrCode_Pano_Control_MDINAPP(4);	break;
		case IRCODE_PANO_CRP_TY_D:	IrCode_Pano_Control_MDINAPP(5);	break;
		case IRCODE_PANO_CRP_BY_U:	IrCode_Pano_Control_MDINAPP(6);	break;
		case IRCODE_PANO_CRP_BY_D:	IrCode_Pano_Control_MDINAPP(7);	break;

		case IRCODE_PANO_SS0:	IrCode_PanoCtrlSelectSensor_MDINAPP(0);	break;
		case IRCODE_PANO_SS1:	IrCode_PanoCtrlSelectSensor_MDINAPP(1);	break;
		case IRCODE_PANO_SS2:	IrCode_PanoCtrlSelectSensor_MDINAPP(2);	break;
		case IRCODE_PANO_SS3:	IrCode_PanoCtrlSelectSensor_MDINAPP(3);	break;
		case IRCODE_PANO_BLEND:	IrCode_PanoCtrlSelectSensor_MDINAPP(4);	break;

		case IRCODE_PANO_EEPROM_SAVE:	SetApp_Pano_WriteEEPROM();		break;
		case IRCODE_PANO_EEPROM_DEL:	SetApp_Pano_ResetEEPROM();		break;

		case IRCODE_PANO_DISP_MODE:		IrCode_Pano_Control_MDINAPP(8);	break;
		case IRCODE_PANO_BLEND_AREA:	IrCode_Pano_Control_MDINAPP(9);	break;
		case IRCODE_PANO_BLEND_LEVEL:	IrCode_Pano_Control_MDINAPP(10);break;
#endif

#endif

#else
		case IRCODE_AE_MODE:		PWM_Test();
								//IrCodeGetCurSTATE();
															break;
		case IRCODE_DEFOG:			IrCodeDefogOnOff();			break;
		case IRCODE_DIS:			IrCodeDISOnOff();			break;	
		case IRCODE_DWDR:			IrCodeDWDROnOff();			break;
		case IRCODE_AE_BRIGHT:		IrCode_AEBRIGHT();		break;
		case IRCODE_AE_DARK:		IrCode_AEDARK(); 			break;
#endif
		
		case IRCODE_WDR:			IrCode_WdrOnOff(1);			break;  //WDR on/off (Line/Built-in mode)
		
		case IRCODE_DIGZOOM:		IrCodeDZoomCtrlOnOff();		break;
 		case IRCODE_VOL_UP_2000:
		case IRCODE_DZOOM_IN:		IrCodeDZoomIN();	SetDZOOM_RepeatKeyEnable();	break;
		case IRCODE_VOL_DN_2000:
		case IRCODE_DZOOM_OUT:		IrCodeDZoomOUT();	SetDZOOM_RepeatKeyEnable();	break;

		case IRCODE_INFO:
			#if defined(__USE_AFZOOM_LENS__)
									if(GetDrvOnOffPINT()) {
										IrCodeAFPINTOnOff();
									} else {
										IrCodeSysInfo();
									}
 			#else
									IrCodeSysInfo();
			#endif
																break;

		case IRCODE_LIST:			IrCodeStatusDisplay();		break;
		case IRCODE_VID_RES:		IrCodeVOUTFormat(ON);		break;
		case IRCODE_FACTORY_L:
		case IRCODE_FACTORY_R:		IrCodeFactoryReset(ON);		break;
		case IRCODE_FREEZEMIRR:		IrCodeFreezeMirrOnOff();	break;
		case IRCODE_PMMD:			IrCodePmMdOnOff();			break;
		
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
		case IRCODE_FOCUS_NEAR:		//LENS_FocusMove(0,0);			break;
									IrCodeFOKSDrvDirN(); 		break;
		case IRCODE_FOCUS_FAR:		//LENS_FocusMove(0,1);			break;
									IrCodeFOKSDrvDirF(); 		break;
		case IRCODE_FAV_UP_2000:
		case IRCODE_ZOOM_TELE:		//LENS_FocusMove(1,0);			break;
									IrCodeZOOMDrvDirT(); 	 if (GetDIS_OnOff()) IrCodeDISOnOff(); 	break;
		case IRCODE_FAV_DN_2000:
		case IRCODE_ZOOM_WIDE:		//LENS_FocusMove(1,1);			break;
									IrCodeZOOMDrvDirW(); 	 if (GetDIS_OnOff()) IrCodeDISOnOff(); 	break;
#endif
		
		case IRCODE_PIPONOFF:		IrCodePIPCtrlOnOff();		break; 
		case IRCODE_PIPSEL:			IrCodePIPCHSEL();			break;	//PIP CH SEL
		case IRCODE_PIPSIZE:		IrCodePIPSize();			break;	//PIP SWAP
		case IRCODE_PIPPOS:			IrCodePipPos();				break;	//PIP position
	
		case IRCODE_POWER_2000:
		case IRCODE_AEONOFF:		IrCodeAECtrlOnOff();		break;
		case IRCODE_STDBY:			IrCodeWBCtrlOnOff();		break;
		case IRCODE_NR:				IrCodeNR2D3DCtrlOnOff(); 	break;
		case IRCODE_EDGE:			IrCode_EdgeEnhanceOnOff(); 	break;
		case IRCODE_NR_DEMO:		IrCode_NR3DDemomode(); 		break;
		case IRCODE_AWB_SETUP:		IrCodeWBPOLYOnOff(); 		break;
		case IRCODE_AWB_MODE:		IrCodeWBCOMPOnOff();		break;
		case IRCODE_AWB_START:		IrCodeGetViewBAYER(); 		break;
		
		case IRCODE_SSW_ON_OFF:		IrCode_SSWCaptureOnOff();		break;
		case IRCODE_SSW_STOP:		IrCode_SSWStopOnOff();			break;
		
		case IRCODE_WB_POLY_POS:	IrCode_WBPolyRead();	break;
		// for AF
#if defined(__USE_AFZOOM_LENS__)
		case IRCODE_AF_SETUP:		if(GetDrvOnOffPINT())
										IrCodeAXProcessON();
									else
										IrCodeAFPINTOnOff();
									break;
		case IRCODE_AF_START:		if(GetDrvOnOffPINT())
										IrCodePINTTargetW();
									else
										IrCodeGetValAFODM();
															break;
		case IRCODE_AF_END:		if(GetDrvOnOffPINT())
										IrCodePINTTargetT();
								else
								#if 1
									//sense up test
									//DEMO_SetExpSensUp(1,2);
									// AE Digital Gain test
									if(AE_DGAIN_TEST == 0)
										AE_DGAIN_TEST = 1;
									else
										AE_DGAIN_TEST = 0;
								#else
									#if 0
										// test IRIS value
										UARTprintf("Ysum:%d\r\n",Get_AE_Ysum());
										IRIS_VAL+=10;
										if(IRIS_VAL>1023) IRIS_VAL = 0;
										//IRIS_WordWrite(SPI_AN41908_ADDR, 0x00, IRIS_VAL+10);
										UARTprintf("\r\nIRIS_VAL:%d / ",IRIS_VAL);

										if(IRIS_VAL <100)
											DCLENS_SetIRISGain(100);
										else
											DCLENS_SetIRISGain(800);
									#else
										// IRIS open/close/auto
										#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
													fAECtl = (fAECtl + 1) %3;
													DCLENS_ForceIRISCtl(fAECtl);
										#endif
										#if 0
													for(fGainSet = 0x20000;fGainSet >=256;fGainSet= fGainSet-(0x20000/1024)) {
														AECTRL_SetGAINGain(fGainSet);
														UARTprintf("fGainSet : 0x%x / ", fGainSet);
													}
										#endif
									#endif
								#endif
															break;
		//case IRCODE_PVR_STOP:		IrCodeAFPINTOnOff();	break;

		//case IRCODE_FREEZE:			SetFOKSDrvLIMIT();		break;

		case IRCODE_PVR_MARK:		IrCodeFOKSCoarseN();	break;
		case IRCODE_PVR_PLIST:		IrCodeFOKSCoarseF();	break;
		case IRCODE_FOCUS:			IrCodeAFProcessON();	break;
		//case IRCODE_EXIT:			SetZOOMDrvSPEED();		break;
		//case IRCODE_EXIT:			IrCodeGetValAFMOV();	break;
		//case IRCODE_AWB_SETUP:	IrCodeGetValAFMOV();	break;
#else
		case IRCODE_AF_END:		// IRIS open/close/auto
								#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
									fAECtl = (fAECtl + 1) %3;
									DCLENS_ForceIRISCtl(fAECtl);
								#endif
															break;
#endif
		case IRCODE_3A_WIN :
									//2710_test();
									//DEMO_SetFactoryReset();
									//ext_test();
									//histogram_test();
									af_win_display();
									//AE_get_cmos_gain();
									//AFCTRL_GetZoomRatio();
 															break;
		case IRCODE_AF_MODE:
#if defined(__USE_AFZOOM_LENS__)
									AF_TestMode();
									//af_win_display();
									//extern const LONG AECTRL_F_VALUE[8];
									//UARTprintf("size of AECTRL_F_VALUE : %d/%d\r\n", sizeof(AECTRL_F_VALUE), sizeof(AECTRL_F_VALUE[0]));
#elif defined(__USE_PWMIRIS_ONLY__)
									//PWM_Test();
									IrCodeGetIrisGRAPH();
#endif
															break;
		
		//case IRCODE_AWB_MODE:		IrCodeGetIrisGRAPH(); break;
		//case IRCODE_AWB_MODE:		IrCodeGetShutGRAPH(); break;
		//case IRCODE_AWB_MODE:		IrCodeGetGainGRAPH(); break;

		// SSW capture
		// case IRCODE_AUDIO_2000:	IrCode_SSWCaptureOnOff();		break;
		// case IRCODE_SUB:			IrCode_SSWStopOnOff();			break;
		// case IRCODE_SHARP:		IrCode_ChromaSuppress();						break;
		// case IRCODE_PVR_FF:		Sensor_data_checksumStart();	break;
		
		// wdr test key
		// case IRCODE_DWDR:			IrCode_WdrDemoMode();                          break;  //WDR MGR demo mode
		// case IRCODE_PVR_REW:		IrCode_WdrTestFunc(MISP_WDR_TEST_HOLD_SS_WR);  break;  //WDR hold sensor write
		// case IRCODE_PVR_PLAY:		IrCode_WdrTestFunc(MISP_WDR_TEST_DEBUG_MODE);  break;  //WDR debug level
		// case IRCODE_PVR_FF:	    	IrCode_WdrTestFunc(MISP_WDR_TEST_FORCE_TM);    break;  //WDR bypass TM
		
#if !(defined(__USE_AFZOOM_LENS__)&&!defined(__USE_2MOTOR_LENS__))
		// case IRCODE_FOCUS_NEAR: 	IrCode_WdrOnOff(2);                            break;  //WDR on/off (2nd WDR)
		// case IRCODE_FOCUS_FAR:		IrCode_WdrOnOff(3);                            break;  //WDR on/off (3rd WDR)
		// case IRCODE_AF_SETUP:		IrCode_ToggleSensorFps();                      break;  //toggles sensor frame rate
		// case IRCODE_AF_START:		IrCode_WdrTestFunc(MISP_WDR_TEST_DISP_FLAG);   break;  //WDR display flag
		// case IRCODE_AF_END:			IrCode_WdrTestFunc(MISP_WDR_TEST_CSP_EN);      break;  //WDR toggle CPS
		// case IRCODE_PVR_STOP:		IrCode_WdrTestFunc(MISP_WDR_TEST_DISP_FRAME);  break;  //WDR MGR use frame
#endif
#if !defined(__PANORAMA_SETUP__)
		//case IRCODE_NUM_5:	    IrCode_WdrTestFunc(MISP_WDR_TEST_TM_LGAIN);    break;  //WDR TM Local gain
		//case IRCODE_NUM_6:		IrCode_WdrTestFunc(MISP_WDR_TEST_TM_OFFSET);   break;  //WDR TM offset control
		//case IRCODE_NUM_8:		IrCode_WdrTestFunc(MISP_WDR_TEST_HOLD_AE);     break;  //WDR hold AE proc.
		//case IRCODE_NUM_9:		IrCode_WdrTestFunc(MISP_WDR_TEST_HOLD_TM);     break;  //WDR hold TM proc.
#endif

		default:				SetMsgCommandComplete(ON); 		break;
	}
	
	
	if((IrComCommand != IRCODE_FACTORY_L) && (IrComCommand != IRCODE_FACTORY_R))		IrCodeFactoryReset(OFF);
	if(IrComCommand != IRCODE_VID_RES)		IrCodeVOUTFormat(OFF);
	if(IrComCommand != IRCODE_EXIT)			IrCode_ExitMenu(OFF);
}


/*  FILE_END_HERE */
