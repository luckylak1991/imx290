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

AE_CTRL_PARAM stAECTRL;
AE_XFER_PARAM stAEXFER;
WB_CTRL_PARAM stWBCTRL;

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
AF_CTRL_PARAM stAFCTRL;
#endif

#if defined(__USE_CMOS_IMX334__)	
static BOOL PassInitODMVD = OFF;
#endif

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
extern VBYTE count_16ms;
extern BYTE AutoSat_Level;
extern BOOL AutoCon_OnOff;
extern BOOL AutoEdge_OnOff;
extern BYTE AutoBright_Level;

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
void CreateVideoInstance(void)
{
	SYSINFO_initialize();
#if defined(__MDIN_i5XX_FPGA__)		// FPGA board(when use same spi bus between sensor & iris ic)
	#ifndef __USE_CMOS_EXT2__
	#ifndef __USE_CMOS_EXT1__	
	CMOS_Initialize();			// for FPGA board boot problem..
	#endif
	#endif
#endif

#if (defined(__USE_DCIRIS_HALL__) || defined(__USE_DCIRIS_DAMP__)) || defined(__USE_PWMIRIS_ONLY__) || defined(__USE_P_IRIS_ONLY__)
#if !defined(__USE_CPU_SPI_SLAVE__)
	IRISDRV_Initialize();
#else
	Delay_mSec(2);
#endif
#endif
	
#if defined(__USE_LATTICE_LCMX03LF__)
#if !defined(__USE_4K60_MD2__)		// skip 4k60 mdin-2
	LCMX03LF_Init();
#endif
#endif
	
	Delay_mSec(10);

#if defined(__USE_LATTICE_LIAMD6K__)	
	Delay_mSec(2000); // Wait for Crosslink configuration done : Minimun waiting time to avoid i2c error(min 1.3sec)
	LIAMD6K_Init();
#endif		
	
	MISP_Initialize();	
	
//	set parameter for AE control
	AEXFER_SetInstancePARAM(&stAEXFER);
	AECTRL_SetInstancePARAM(&stAECTRL);

//	set parameter for AE control
	WBCTRL_SetInstancePARAM(&stWBCTRL);

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
//	set parameter for AF control
	#if !defined(__USE_4K60_MD2__) && !defined(__USE_MISP_AF_MD2__)
	AFCTRL_SetInstancePARAM(&stAFCTRL);
	#endif
#endif


#if defined(__SENSOR_VSYNC_BOOT__) 
	WAIT_ODMVsyncPulse();
#else
	*R_TM2CTRL |= F_TMCTRL_TMEN_EN;
#endif


#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
//	set parameter & position for LENS
	#if !defined(__USE_4K60_MD2__) && !defined(__USE_MISP_AF_MD2__)
	LENS_SetInstancePARAM();
	#endif
#endif
	
#if !defined(__USE_4K60_MD2__)
	SetDYNT_ICR(OFF);		// icr off(day)
#endif

#if defined(__SENSOR_VSYNC_BOOT__) 
	WAIT_ODMVsyncPulse();
#endif

#if defined(__SENSOR_VSYNC_BOOT__) 
	WAIT_ODMVsyncPulse();
#endif
	
	
	MISC_Initialize();
	


#if defined(__USE_EXT_PLL_IC__)		// for external pll ic
#if !defined(__USE_4K60_MD2__)		// skip 4k60 mdin-2
	EXTPLL_Init();
#endif
#endif

#if defined(__USE_PS8409_RETIMER__)
#if !defined(__USE_4K60_MD2__)		// skip 4k60 mdin-2
	RETIMER_Init();
#endif
#endif

#if	defined (__USE_TVI_TX__)
	TVI_RESET(HIGH);		// release reset
	Delay_uSec(20000);		// delay 20ms
	Init_TVITx_RegSet();	// TP2912 (TVI TX)
#endif

#if defined (__USE_IT66121_TX__)
	IT66121_Init();			// IT66121 (HDMI TX)
#endif

#if defined (__USE_IT6615_TX__)
	ITE6615_Init();			// ITE6615 (HDMI TX)
#endif

#if defined (__USE_LT8618EX_TX__)
	#if defined (__ECOM1_UNION_12X__)
		SIL1136_Init();
	#else 
		LT8618EX_Init();		// LT8618EX (HDMI TX)
	#endif
#endif

#if defined (__USE_LT8618SX_TX__)
	LT8618SX_Init();		// LT8618SX (HDMI TX)
#endif

#if defined (__USE_SIL9136_TX__)
	HDTX_RESET(HIGH);		// release reset
	Delay_uSec(20000);		// delay 20ms
	SIL9136_Init();			// Sil9136 (HDMI TX)
#endif

#if defined(__USE_IT6802_RX__)
	it6802HPDCtrl(0,0);	// HDMI port , set HPD = 0
	Delay_mSec(100);	// delay 100ms
	IT6802_fsm_init();
	it6802HPDCtrl(0,1);	// HDMI port , set HPD = 1
#endif

#if defined(__USE_LT8619_RX__)
	LT8619C_Init();			// LT8619C (HDMI RX)
#endif

#if defined(__USE_EN332T__)
	EN332x_Initialize();
#endif

#if defined(__USE_IMX290_1080P120_MODE__)
	//initial frame rate set fps=60
//	DEMO_SetSensorFps(3);
//	SetMenuStatus(SENSOR_FPS_MENU, 3);
#endif
	
	fSysStateSTABLE = TRUE;		// steady-state mode	
}

//--------------------------------------------------------------------------------------------------
static void ISPM ACTVD_ResetCounter(void)	
{
	if (fSysStateSTABLE==FALSE) return;

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
//	LENS_CtrlSPILineBUSY();
#endif

	count_16ms = 0;
	
	*R_TP2CTRL = F_TPCTRL_CNTCLR_SET;
	*R_TP2CTRL = F_TPCTRL_CNTCLR_CLR;
	
	*R_TM2CNT = (GetSS_SensorVoutPAL()? 1231: 1026) * (get_apb_clock()/8/1000) / 1000;	// 2M_30/60
	
#if (defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__) || defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX334__)) && !defined(__USE_MDIN_i5XX_REV__)	// i5xx org chip
	*R_TM2CNT = 1563 * (get_apb_clock()/8/1000) / 1000;	// 4k_19.6875fr(50.793ms/32.5 = 1.5628ms)
#endif
	
#if  (defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)) && (defined(__USE_IMX290_720P120_MODE__) || defined(__USE_IMX290_720P120_ISPCROP__) || defined(__USE_IMX290_1080P120_MODE__) || defined(__USE_IMX273_720P120_MODE__))
	*R_TM2CNT = (GetSS_SensorVoutPAL()? 1231/2: 1026/2) * (get_apb_clock()/8/1000) / 1000;	// 2M_100/120		
#elif defined(__USE_IMX273_720P240_MODE__) 
	*R_TM2CNT = (GetSS_SensorVoutPAL()? 1231/4: 1026/4) * (get_apb_clock()/8/1000) / 1000;	// 2M_100/120		
#endif
	
	*R_TM2CTRL |= F_TMCTRL_TMEN_EN;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM ODMVsyncPulseDecode(void)
{
	fSysIRQMsgODMVD = ON;	//IRIS_VD(HIGH);
	fSysIRQCntODMVD = fSysIRQCntODMVD?0:1;
	ACTVD_ResetCounter();	//IRIS_VD(LOW);
}

//--------------------------------------------------------------------------------------------------------------------------
void WAIT_ODMVsyncPulse(void)
{
	#if defined(__USE_CMOS_IMX334__)		//IMX334 slave mode : code for power on boot with LATTICE_LIAMD6K fpga chipset
											//IMX334 master mode : This is the code required to successfully complete a power-on boot with the e-con board connected.
		fSysIRQMsgODMVD = OFF;
		while (fSysIRQMsgODMVD==OFF){		
			if(PassInitODMVD==OFF) {
				MISP_RegField(MISP_LOCAL_ID, 0x58F, 8, 1, 0);
				MISP_RegField(MISP_LOCAL_ID, 0x58F, 8, 1, 1);
			}
		}

		PassInitODMVD=ON;
		
	#else									//normal case
		fSysIRQMsgODMVD = OFF;
		while (fSysIRQMsgODMVD==OFF);	// wait VD
	#endif
}

//--------------------------------------------------------------------------------------------------------------------------
#if defined(__USE_GET_RGB_MODE__)
static WORD cntWB = 0;
#endif
void ODMAutoCtrlHandler(void)
{
	WORD pBuf;
	WORD flagAE, flagWB, flagAF;

	MISP_RegRead(MISP_LOCAL_ID, 0x600, &pBuf);

	flagAE = (pBuf & 0x4000);
	flagWB = (pBuf & 0x2000);
	flagAF = (pBuf & 0x1000);

#if !defined(__USE_4K60_MD2__)
	if(flagAE) AE_UpdateHandler(&stAEXFER);	// AE move
#endif


#if defined(__USE_AF_SLAVE__)
	SPIsAPP_Update3aRawData();
#if defined(__USE_4K60_MD2__)
	SPIsAPP_UpdateNRRawData();
#endif
	SPIsAPP_ClearRawUpdateFlag();
#endif

#if defined(__USE_AF_SLAVE__) || defined(__USE_4K60_MD1__) // just for testing
	//LONG slaveFLTa[6],slaveFLTb[6];

	//SPImAPP_GetSlaveAFfiltA((PBYTE)&slaveFLTa[0]); //24
	//SPImAPP_GetSlaveAFfiltB((PBYTE)&slaveFLTb[0]); //24
	//UARTprintf("slaveFLTa:%d %d %d %d %d %d\r\n",slaveFLTa[0],slaveFLTa[1],slaveFLTa[2],slaveFLTa[3],slaveFLTa[4],slaveFLTa[5]);
#endif

#if !defined(__USE_AF_SLAVE__) && !(defined(__USE_EXT_AF_MD1__) || defined(__USE_MISP_AF_MD2__))
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	if(flagAF) DZ_UpdateHandler(&stAFCTRL);
#endif
#endif


	if (fSysIRQMsgODMVD==OFF) return;		// check odm v.sync flag
	fSysIRQMsgODMVD = OFF;

#if defined(__USE_4K60_MD1__) 
		Check_FPGA_OB();		//imx226 stable booting..
#endif

#if defined(__USE_WDR_PROCESS__)
	//If current WDR mode is frame-by-frame mode,
	//do rest of process for the long(short) phase only.
	if(!GetWDR_ODMValidFrame()) {
		WDR_ProcessHandler();
		return;
	}
#endif

#if defined(__SENSOR_ERR_CHECK__)
	AE_YsumHandler(&stAECTRL);
	if(flagAE) AE_ProcessHandler(&stAECTRL);
	if(flagWB) WB_ProcessHandler(&stWBCTRL);
//	sensor_check_print();
	sensor_check_print2();
	return;
#endif

#if defined(__USE_SENS_PANNING_MODE__)
	if(SS_image_panning_update()) return;
#endif

//	DIS_BayerPosVect_Gen(GetDIS_bayer_vect());		// for sensor test pattern
	if(StaticDPC_Process()) SDPC_Return();
	if(StaticDPD_Busy()) return;		//when static dpd read process other block do not need run(include 3a..). this is only run static dpd process

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	TONE_ProcessHandler(); // TONE_ProcessHandler moved into ODMAutoCtrlHandler 2017.06.01
#endif			
		
#if (defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)) || defined(__USE_EXT_AF_MD1__)
	SPImAPP_RxSlaveRawData();	// receive slave raw data
#endif
	
#if !defined(__USE_4K60_MD2__)
	AE_YsumHandler(&stAECTRL);
#endif
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))&&!defined(__USE_AF_SLAVE__)&&!defined(__USE_MISP_AF_MD2__)	// 3a master mode
    #if defined(__USE_2MOTOR_LENS__)
	if(flagAF) focusPickSearch(); // mimitest
    #endif

	if(flagAF) AF_ProcessHandler(&stAFCTRL); // AF process
	//if(flagAF) AF_DisplayHandler(&stAFCTRL); // AF window display
  #if !defined(__USE_2MOTOR_LENS__)
	if(flagAF) AX_ProcessHandler(&stAFCTRL); // pint adjust process
  #endif
	if(flagAF) AA_ProcessHandler(&stAFCTRL); // aging
	if(flagAF) AF_ExtractHandler(&stAFCTRL); // extrack af data process
//	if(flagAF) AF_PanTiltHandler(&stAFCTRL);
//	if(flagAF) AF_ConvertHandler(&stAFCTRL);
#endif

	BY_ProcessHandler();
	AUTODefogCCtrl();
	
#if !defined(__USE_4K60_MD2__)
	if(flagAE) AE_ProcessHandler(&stAECTRL);
#endif

#if defined(__USE_GET_RGB_MODE__) && defined(__USE_GET_AWB_RGB_INFO__)
	cntWB++;
	cntWB = (cntWB%10);
	if(cntWB == 0) {
		AWB_ModeChange(FALSE);
		if(flagWB) WB_ProcessHandler(&stWBCTRL);
	} else if(cntWB == 1) {
		AWB_ModeChange(TRUE);
		if(flagWB) WB_ProcessHandler(&stWBCTRL);
	} else if(cntWB == 2) {
		Color_ProcessHandler(&stWBCTRL);
	} else {
		if(flagWB) WB_ProcessHandler(&stWBCTRL);
	}
#else
	if(flagWB) WB_ProcessHandler(&stWBCTRL);
#endif

#if defined(__USE_WDR_PROCESS__)
	WDR_ProcessHandler();
#endif

	// Sensor Deserializer Error Correction (test code)
//	SSCTRL_DesErrCorrection();

#if !defined(__USE_4K60_MD2__)
	//if(flagAE) AE_DisplayHandler(&stAECTRL);
	if(flagAE) AE_DBGIrisHandler(&stAECTRL);
	if(flagAE) AE_DBGGainHandler(&stAECTRL);
	if(flagAE) AE_DBGShutHandler(&stAECTRL);
#endif

	//if(flagWB) WB_DisplayHandler(&stWBCTRL);
	if(flagWB) WD_ProcessHandler(&stWBCTRL);
	if(flagWB) WD_TriggerHandler(&stWBCTRL);

#if !defined(__USE_4K60_MD2__)
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	if(flagAE) AE_DigitalGainCtl(&stAECTRL);
#endif
	
#if !defined(__PANORAMA_SETUP__)	// panorama board S1,S2 agc level not correct...
//	LowLight_AutoGamma(OFF);
	// LowLight_AutoSat(AutoSat_Level); // for test
	LLApp_ProcessHandler();
	// LowLight_AutoCon(AutoCon_OnOff); // for test 
	// LowLight_AutoEdge(AutoEdge_OnOff);
	// LowLight_AutoBrightness(AutoBright_Level); // for test
	LowLight_AutoDyanmicDPC();

	CAC_prevent_escape();	
	
	// LowLight_CSP();
#endif
#endif

	NR_ProcessHandler();
	DYNT_ProcessHandler(&stWBCTRL);
	PM_ProcessHandler();
	
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	FLK_ProcessHandler();
#endif

#if !defined(__USE_GET_RGB_MODE__) && defined(__USE_GET_AWB_RGB_INFO__)
	Color_ProcessHandler(&stWBCTRL);
#endif

#if	defined(__MDIN_i510_REF__)				// i510 REF board
	IOExp_ProcessHandler();
#endif
	
}

//--------------------------------------------------------------------------------------------------------------------------
void VIDEO_RxHandler(void)
{
#if defined(__USE_IT6802_RX__)
	IT6802_fsm();
	IT6802_InputMonitor();
#endif

#if defined(__USE_LT8619_RX__)
	LT8619C_MainLoop();
	ExtInput_Monitor();
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void VIDEO_TxHandler(void)
{
#if defined(__USE_IT66121_TX__)
	IT66121_ProcHanlder();
#endif

#if defined(__USE_IT6615_TX__)
	ITE6615ProcHanlder();
#endif
	
#if defined(__USE_SIL9136_TX__)
	SIL9136ProcHanlder();
#endif
	
#if defined(__USE_TVI_TX__)
	GetTVI_Interrupt();
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void VIDEO_ChkTunePINT(BYTE y)
{
#if defined (__USE_EEPROM__)
	if (EEPROM_RegRead(EEPROM_PINT_LENSNO)==EEPROM_PINT_DONE) return;

	OSD_StrDispAttb(y,0,ON, MAKEBYTE(OSD_WHITE, OSD_NOCOLOR), "NOT AF-CALIBRATION!");
	OSD_StrDispOnOff(ON);

	UARTprintf("\r\n********************************************\r\n");
	UARTprintf(    " You must do the AF-Calibration process !!! \r\n");
	UARTprintf(    "********************************************\r\n");
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void VIDEO_ChkTuneCOMP(BYTE y)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	if (EEPROM_RegRead(EEPROM_COMP_ID)==EEPROM_COMP_DONE) return;

	OSD_StrDispAttb(y,0,ON, MAKEBYTE(OSD_WHITE, OSD_NOCOLOR), "NOT WB-CALIBRATION!");
	OSD_StrDispOnOff(ON);

	UARTprintf("\r\n********************************************\r\n");
	UARTprintf(    " You must do the WB-Calibration process !!! \r\n");
	UARTprintf(    "********************************************\r\n");
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void VIDEO_ChkTunePOLY(BYTE y)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	if (EEPROM_RegRead(EEPROM_POLY_ID)==EEPROM_POLY_DONE) return;

	OSD_StrDispAttb(y,0,ON, MAKEBYTE(OSD_WHITE, OSD_NOCOLOR), "NOT DEFINE POLYGON!");
	OSD_StrDispOnOff(ON);

	UARTprintf("\r\n********************************************\r\n");
	UARTprintf(    " You must do the POLYGON-Define process !!! \r\n");
	UARTprintf(    "********************************************\r\n");
#endif
}

LONG Get_AE_Ysum(void) // Get AE Ysum
{
	return stAECTRL.Ysum;
}

LONG Get_AE_Ydiff(void) // Get AE Ydiff(signed)
{
	return stAECTRL.Ydiff;
}

LONG Get_AE_YDIFF(void) // Get AE YDIFF(abs)
{
	return stAECTRL.YDIFF;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT Get_AE_BrightLevel(void)
{
	return stAECTRL.level;
}

//--------------------------------------------------------------------------------------------------------------------------
LONG Get_AE_Yref(void) // Get AE Yref 
{
	return stAECTRL.Yref;
}

//--------------------------------------------------------------------------------------------------------------------------
DWORD Get_AE_DGain(void) // Get AE Digital Gain
{
	return stAEXFER.fine;
}

//--------------------------------------------------------------------------------------------------------------------------
DWORD Get_AE_Gain(void) // Get AE dB Gain
{
	return stAEXFER.gain;
}

//--------------------------------------------------------------------------------------------------------------------------
LONG Get_AE_Mode(void)
{
	return(stAECTRL.ctrl & AECTRL_PRIORITY_MASK);
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT Get_RGB_CNT(void)
{
	return stWBCTRL.stWBCOLOR.cntColor;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT Get_RGB_R(void)
{
	return stWBCTRL.stWBCOLOR.fAvgR;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT Get_RGB_G(void)
{
	return stWBCTRL.stWBCOLOR.fAvgG;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT Get_RGB_B(void)
{
	return stWBCTRL.stWBCOLOR.fAvgB;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT Get_RGB_Y(void)
{
	return stWBCTRL.stWBCOLOR.fAvgY;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT Get_RGB_U(void)
{
	return stWBCTRL.stWBCOLOR.fAvgU;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT Get_RGB_V(void)
{
	return stWBCTRL.stWBCOLOR.fAvgV;
}

//--------------------------------------------------------------------------------------------------------------------------
WORD Get_WB_U(WORD idx)
{
	return stWBCTRL.stWBCOLOR.U[idx];
}

//--------------------------------------------------------------------------------------------------------------------------
WORD Get_WB_V(WORD idx)
{
	return stWBCTRL.stWBCOLOR.V[idx];
}


//--------------------------------------------------------------------------------------------------------------------------
BOOL Get_RGB_RatioDiff(LONG margin) // 1: IR ramp on
{
	if((stWBCTRL.stWBCOLOR.fAvgR - stWBCTRL.stWBCOLOR.fAvgG) > margin)	return 1;
	else		return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL Get_YUV_Ratio(BOOL modef, LONG margin) // 1: IR ramp on
{
	FLOAT ratiof;
	LONG ratioi;

	if((stWBCTRL.stWBCOLOR.fAvgU <= 0) || (stWBCTRL.stWBCOLOR.fAvgV <=0)) return FALSE;

	if(stWBCTRL.stWBCOLOR.fAvgU >= stWBCTRL.stWBCOLOR.fAvgV)
		ratiof = (FLOAT)stWBCTRL.stWBCOLOR.fAvgV / (FLOAT)stWBCTRL.stWBCOLOR.fAvgU;
	else
		ratiof = (FLOAT)stWBCTRL.stWBCOLOR.fAvgU / (FLOAT)stWBCTRL.stWBCOLOR.fAvgV;
	ratioi = (LONG)(ratiof*256.0f);

	//if(modef == TRUE) UARTprintf("ratio : %d / %d\r\n",ratioi,DIFF(ratioi,256));

	if(DIFF(ratioi,256) < margin) return TRUE;
	else return FALSE;
}

/*  FILE_END_HERE */
