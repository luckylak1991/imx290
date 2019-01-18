//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2016  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	SPIS_APP.C
// Description 		: spi-slave application
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"
#include	"osd.h"

#if defined(__USE_CPU_SPI_SLAVE__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//-------------------------------------------------------------------------------------
#if defined(__USE_IRDATA_SLAVE2MASER__)
extern WORD gIrComCommand;
extern BOOL gIRComCommand_on;
#endif

void SPIsAPP_Update3aRawData(void)			// 80us -> 30us(ispm) -> 20us(bus ispm), tx 37words : 4.7ms
{
	PDWORD pBuff = &stSPIsAPP.stTXRAW.buff_3a[0];
	WORD rVal[40], i=0;
	
	if((stSPIsAPP.stTXRAW.ctrl & SPIS_TXRAW_CTRL_UPDATE) == 0)		return;	// check update flag(set every 'v.sync+8ms')

	if(stSPIsCTRL.ctrl & SPIS_TX_MODE_3A_DATA)		return;	// skip update tx buffer while sending 3a raw data
	if(stSPIsCTRL.ctrl & SPIS_TX_MODE_NR_DATA)		return;	// skip update tx buffer while sending nr raw data

	// ae //7
	MISP_MultiRead(MISP_LOCAL_ID, 0x690, (PBYTE)rVal, 20);		// 690~69Fh, 5 dword(20 byte)
	pBuff[i++] = MAKEDWORD(rVal[0]&0x00ff, rVal[1])>>2;			// ysum win-0
	pBuff[i++] = MAKEDWORD(rVal[2]&0x00ff, rVal[3])>>2;			// ysum win-1
	pBuff[i++] = MAKEDWORD(rVal[4]&0x00ff, rVal[5])>>2;			// ysum win-2
	pBuff[i++] = MAKEDWORD(rVal[6]&0x00ff, rVal[7])>>2;			// ysum win-3
	pBuff[i++] = MAKEDWORD(rVal[8]&0x00ff, rVal[9])>>2;			// ysum win-4
	pBuff[i++] = stAEXFER.iris;									// ae IRIS
	pBuff[i++] = AECTRL_GetGAINGain();							// ae GAIN

#if defined(__USE_IRDATA_SLAVE2MASER__)
	if(gIRComCommand_on) {
		pBuff[i++] = gIrComCommand;							// IR data
		gIRComCommand_on = FALSE;
	 	UARTprintf("IR code : %d\r\n",gIrComCommand);
	} else
		pBuff[i++] = 0xff;
#endif

#if defined(__USE_AF_SLAVE__)
	// af //12
	MISP_MultiRead(MISP_LOCAL_ID, 0x6e8, (PBYTE)rVal, 48);		// 6E8~6FFh, 12 dword(48 byte)
	pBuff[i++] = MAKEDWORD(rVal[8], rVal[9]);					// RPFa
	pBuff[i++] = MAKEDWORD(rVal[10], rVal[11]);					// LPFa
	pBuff[i++] = MAKEDWORD(rVal[12], rVal[13]);					// VPFa
	pBuff[i++] = MAKEDWORD(rVal[2], rVal[3]);					// BPFa
	pBuff[i++] = MAKEDWORD(rVal[14], rVal[15]);					// HPFa
	pBuff[i++] = MAKEDWORD(rVal[0]&0x00ff, rVal[1]);			// CNTa
	pBuff[i++] = MAKEDWORD(rVal[16], rVal[17]);					// RPFb
	pBuff[i++] = MAKEDWORD(rVal[18], rVal[19]);					// LPFb
	pBuff[i++] = MAKEDWORD(rVal[20], rVal[21]);					// VPFb
	pBuff[i++] = MAKEDWORD(rVal[6], rVal[7]);					// BPFb
	pBuff[i++] = MAKEDWORD(rVal[22], rVal[23]);					// HPFb
	pBuff[i++] = MAKEDWORD(rVal[4]&0x00ff, rVal[5]);			// CNTb
#endif
	
/*
	// awb //21
	MISP_MultiRead(MISP_LOCAL_ID, 0x650, (PBYTE)rVal, 78);		// 650~676h, 19 dword(78 byte)
	pBuff[i++] = MAKEDWORD(0x0000, rVal[0]>>4);					// y-integral count
	pBuff[i++] = MAKEDWORD(rVal[0]&0x000f, rVal[1]);			// [31:0]y-integral sum
	pBuff[i++] = MAKEDWORD(rVal[3]&0x03ff, rVal[2]&0x03ff);		// [31:16]rg-max, [15:0]rr-max
	pBuff[i++] = MAKEDWORD(rVal[5]&0x03ff, rVal[4]&0x03ff);		// [31:16]gr-max, [15:0]rb-max
	pBuff[i++] = MAKEDWORD(rVal[7]&0x03ff, rVal[6]&0x03ff);		// [31:16]gb-max, [15:0]gg-max
	pBuff[i++] = MAKEDWORD(rVal[9]&0x03ff, rVal[8]&0x03ff);		// [31:16]bg-max, [15:0]br-max
	pBuff[i++] = MAKEDWORD(0x0000, rVal[10]&0x03ff);			// bb-max
	pBuff[i++] = MAKEDWORD(rVal[11]&0x00ff, rVal[12]);			// r-prm
	pBuff[i++] = MAKEDWORD(rVal[13]&0x00ff, rVal[14]);			// g-prm
	pBuff[i++] = MAKEDWORD(rVal[15]&0x00ff, rVal[16]);			// b-prm
	pBuff[i++] = MAKEDWORD(0x0000, rVal[17]&0x0fff);			// prm counter
	pBuff[i++] = MAKEDWORD(rVal[18]&0x00ff, rVal[19]);			// r-gry
	pBuff[i++] = MAKEDWORD(rVal[20]&0x00ff, rVal[21]);			// g-gry
	pBuff[i++] = MAKEDWORD(rVal[22]&0x00ff, rVal[23]);			// b-gry
	pBuff[i++] = MAKEDWORD(0x0000, rVal[24]&0x0fff);			// gry counter
	pBuff[i++] = MAKEDWORD(rVal[32]&0x00ff, rVal[33]);			// r-ctl
	pBuff[i++] = MAKEDWORD(rVal[34]&0x00ff, rVal[35]);			// g-ctl
	pBuff[i++] = MAKEDWORD(rVal[36]&0x00ff, rVal[37]);			// b-ctl
	pBuff[i++] = MAKEDWORD(0x0000, rVal[38]);					// ctl counter

	MISP_MultiRead(MISP_LOCAL_ID, 0x608, (PBYTE)rVal, 6);		// 608~60Ah, 2 dword(6 byte)
	pBuff[i++] = MAKEDWORD(0x0000, rVal[0]);					// wb gain-r(proc_gainr)
	pBuff[i++] = MAKEDWORD(0x0000, rVal[2]);					// wb gain-b(proc_gainb)
*/
}

//-------------------------------------------------------------------------------------
void SPIsAPP_UpdateNRRawData(void)
{
	PBYTE pBuff = (PBYTE)&stSPIsAPP.stTXRAW.buff_nr[0];
	WORD rval[6];
	
	if((stSPIsAPP.stTXRAW.ctrl & SPIS_TXRAW_CTRL_UPDATE) == 0)		return;	// check update flag(set every 'v.sync+8ms')
	
	if(stSPIsCTRL.ctrl & SPIS_TX_MODE_3A_DATA)		return;	// skip update tx buffer while sending 3a raw data
	if(stSPIsCTRL.ctrl & SPIS_TX_MODE_NR_DATA)		return;	// skip update tx buffer while sending nr raw data

	GetNR_StDEV_br(&rval[0], &rval[1], 0);			// [0]stdev_int_br, [1]stdev_frac_br
	GetNR_StDEV_da(&rval[2], &rval[3], 0);			// [2]stdev_int_da, [3]stdev_frac_da
	GetNR_StDEV(&rval[4], &rval[5] ,0);				// [4]stdev_int, [5]stdev_frac
	memcpy(pBuff, (PBYTE)&rval[0], sizeof(rval));
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SPIsAPP_SetRawUpdateFlag(void)
{
	stSPIsAPP.stTXRAW.ctrl |= SPIS_TXRAW_CTRL_UPDATE;		// set raw data update flag
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_ClearRawUpdateFlag(void)
{
	stSPIsAPP.stTXRAW.ctrl &= (~SPIS_TXRAW_CTRL_UPDATE);	// clear raw data update flag
}



// command
//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdRegAddr(WORD addr)
{
	stSPIsAPP.reg_addr = addr;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdRegWrite(WORD val)
{
	switch(HI4BIT(HIBYTE(stSPIsAPP.reg_addr))|0xC0)	{
		case  MISP_HOST_ID:	MISP_RegWrite(MISP_HOST_ID, (stSPIsAPP.reg_addr&0xFFF), val);	break;
		case  MISP_LOCAL_ID:	MISP_RegWrite(MISP_LOCAL_ID, (stSPIsAPP.reg_addr&0xFFF), val);	break;
		//case  MISP_SDRAM_ID:	MISP_RegWrite(MISP_SDRAM_ID, (stSPIsAPP.reg_addr&0xFFF), val);	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdIRKey(WORD val)
{
	switch(val)		{
		case  0x16e9:		MenuMessageControl();				break;	// menu
		case  0x11ee:		DirUMessageControl();				break;	// up
		case  0x15ea:		DirDMessageControl();				break;	// down
		case  0x12ed:		DirLMessageControl();				break;	// left
		case  0x14eb:		DirRMessageControl();				break;	// right
		case  0x13ec:		EnterMessageControl();				break;	// enter
#if defined(__PANORAMA_SETUP__)
		case  0x06f9:		IrCode_PanoCtrlMain();				break;	// '5'
		case  0x07f8:		IrCode_PanoCtrlAux();				break;	// '6'
		case  0x09f6:		SetApp_Pano_Control(1);;			break;	// '8'
		case  0x0af5:		SetApp_Pano_Control(0);				break;	// '9'
		case  0x0bf4:		SetApp_Pano_WriteEEPROM();			break;	// '0'
		case  0x1ce3:		SetApp_Pano_ResetEEPROM();			break;	// '#'
#endif
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdFBcapCtrl(WORD ctrl)
{
	PSPIs_FBCAP_PARAM pCAP = &stSPIsAPP.stFBCAP;
	WORD rVal=0;
	
	if(ctrl == 1)	{		// start capture
		pCAP->cnt_tx = 0;
		pCAP->cnt_rd = 0;
		pCAP->cnt_push = 0;
		pCAP->cnt_pop = 0;
		pCAP->txsize = pCAP->w * pCAP->h *2;	// Y + C
		pCAP->rd_x = pCAP->lx;
		pCAP->rd_y = pCAP->ly;
		
		MISP_RegRead(MISP_LOCAL_ID, 0x1C4, &rVal);		// map1 col count per line
		pCAP->cnt_col = (rVal&0x3FF) * 8;
		MISP_RegRead(MISP_LOCAL_ID, 0x1B2, &rVal);		// map1 start row address
		pCAP->e_addr_y = (DWORD)rVal << 12;
		MISP_RegRead(MISP_LOCAL_ID, 0x1B8, &rVal);
		pCAP->e_addr_c = (DWORD)rVal << 12;
		
		MISP_RegField(MISP_LOCAL_ID, 0x040, 0, 2, 3); // [0]main_wr_stop, [1]main_freeze
		
		pCAP->ctrl = SPIS_FBCAP_CTRL_RUN;
	}
	else	{		// stop capture
		MISP_RegField(MISP_LOCAL_ID, 0x040, 0, 2, 0); // [0]main_wr_stop, [1]main_freeze
		
		pCAP->ctrl = SPIS_FBCAP_CTRL_STOP;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdFBcapX(WORD val)
{
	stSPIsAPP.stFBCAP.lx = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdFBcapY(WORD val)
{
	stSPIsAPP.stFBCAP.ly = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdFBcapW(WORD val)
{
	stSPIsAPP.stFBCAP.w = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdFBcapH(WORD val)
{
	stSPIsAPP.stFBCAP.h = val;
}

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
#if defined(__PANORAMA_SETUP__)
// panorama SPI slave functions
void SPIsAPP_CmdPanoDisp(WORD val)
{
	// blank func.
	BYTE buff[2] ;
	buff[0] = HIBYTE(LOWORD(val));		buff[1] = LOBYTE(LOWORD(val));
	
	SetApp_Pano_DisplayMode(buff[0],buff[1]) ;
}
//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdPanoSave(WORD val)
{
	SetApp_Pano_WriteEEPROM() ;
}
//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdPanoDel(WORD val)
{
	SetApp_Pano_ResetEEPROM() ;
}
//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdPanoBlend(WORD val)
{
	SetApp_Pano_BlendRightArea(val) ;
}
//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdPanoBLvL(WORD val)
{
	SetApp_Pano_BlendLevel(val);
}
//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdPanoSelSS(WORD val)
{
	IrCode_PanoCtrlSelectSensor() ;
}
//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdPanoSSAdj(WORD val)
{
	IrCode_PanoCtrlSelectAdjust() ;
}
//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdPanoCntrl(WORD val)
{
	SetApp_Pano_Control(val) ;
}

// MDINAPP
//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdPanoSelSS_MDINAPP(WORD val)
{
	IrCode_PanoCtrlSelectSensor_MDINAPP(val) ;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdPanoControl_MDINAPP(WORD val)
{
	IrCode_Pano_Control_MDINAPP(val) ;
}

#endif // #if defined(__PANORAMA_SETUP__)

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdWBGainR(WORD val)
{
	LONG mode = stWBCTRL.ctrl&WBCTRL_MODE_MASK;
	
	stWBCTRL.stMANUAL.R  = val;

	stWBCTRL.ctrl &= (~WBCTRL_MODE_MASK);
	stWBCTRL.ctrl |= (mode|WBCTRL_TRIGGER_ISP);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdWBGainB(WORD val)
{
	LONG mode = stWBCTRL.ctrl&WBCTRL_MODE_MASK;
	
	stWBCTRL.stMANUAL.B  = val;

	stWBCTRL.ctrl &= (~WBCTRL_MODE_MASK);
	stWBCTRL.ctrl |= (mode|WBCTRL_TRIGGER_ISP);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdAEDGain(WORD val)
{
	stAEXFER.fine = val;								// for 'Get_AE_DGain'
#if defined(__USE_CMOS_IMX226__)
	stAEXFER.fUpdateDGainVBlank = 1;
#else
	MISP_RegWrite(MISP_LOCAL_ID, 0x601, LOWORD(val));	// digital gain
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdAEGainGainH(WORD val)
{
	stSPIsAPP.stMAINFO.temp_h16b = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdAEGainGainL(WORD val)
{
	AECTRL_SetGAINGain(MAKEDWORD(stSPIsAPP.stMAINFO.temp_h16b, val));
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdAEShutGain(WORD val)
{
	AECTRL_SetSHUTGain((SHORT)val);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdAEIrisGain(WORD val)
{
#if defined(__USE_DCIRIS_HALL__) || defined(__USE_DCIRIS_DAMP__) || defined(__USE_P_IRIS_ONLY__)
	DCLENS_SetIRISGain((SHORT)val);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdAEGainMaxH(WORD val)
{
	stSPIsAPP.stMAINFO.temp_h16b = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdAEGainMaxL(WORD val)
{
	AECTRL_SetGAINMax(MAKEDWORD(stSPIsAPP.stMAINFO.temp_h16b, val));
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdAEGainMinH(WORD val)
{
	stSPIsAPP.stMAINFO.temp_h16b = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdAEGainMinL(WORD val)
{
	AECTRL_SetGAINMin(MAKEDWORD(stSPIsAPP.stMAINFO.temp_h16b, val));
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdAEGainPMaxH(WORD val)
{
	stSPIsAPP.stMAINFO.temp_h16b = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdAEGainPMaxL(WORD val)
{
	AECTRL_SetGAINPMax(MAKEDWORD(stSPIsAPP.stMAINFO.temp_h16b, val));
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdAEBright(WORD val)
{
	AECTRL_SetBrightness(&stAECTRL, val);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdAEYsumH(WORD val)
{
	stSPIsAPP.stMAINFO.temp_h16b = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdAEYsumL(WORD val)
{
	stAECTRL.Ysum = MAKEDWORD(stSPIsAPP.stMAINFO.temp_h16b, val);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdLLSharpness(WORD val)
{
	//DEMO_SetSharpness(val);
	SetIN_MainHPFLevel(val);
	SetIN_MainHPFLDslope(val);
	SetFC_MainVPFLevel(val);
	SetFC_MainVPFLDslope(val);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdLLSaturation(WORD val)
{
	SetOUT_MainSaturation(val);
	SetOUT_AuxSaturation(val);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdCSPLS(WORD val)
{
	SetCSP_LS_P0(HIBYTE(val));	// [15:8] x0
	SetCSP_LS_P1(LOBYTE(val));	// [7:0] x1
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdCACGgain(WORD val)
{
	Set_CAC_Green_gain(val);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdCACMgain(WORD val)
{
	Set_CAC_Magenta_gain(val);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdDYNTCtrl(BYTE cmd, BYTE val)
{
	switch(cmd)	{
		case 0x01:	stSPIsAPP.stMAINFO.dynt_status = (val)?  1:  0;		break;	// 0(day), 1(night)
		case 0x02:	SetDYNT_NTColor(val);								break;
		case 0x03:	SetDYNT_CVBSBurst(val);								break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdNRMode(WORD val)
{
	SetNR_Mode((MISP_NR_MODE_t)val);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdNRIntensity(WORD val)
{
	SetNR_intensity((MISP_NR3D_MODE_t)val);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdNRAperture(WORD val)
{
	SetNR_Aperture(val);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdNRZoomMode(WORD val)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	SetNR_NRZoomingMode();
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdNRMisc(WORD val)	// [12]agc_max, [9:8]under_exp, [7:1]edge_level, [0]lowlight_mode
{
	BYTE agc_max ;
	BYTE under_exp ;
	BYTE edge_level ;
	BYTE lowlight_mode ;
	
	agc_max = HI4BIT(HIBYTE(val));
	under_exp = LO4BIT(HIBYTE(val));
	edge_level = LOBYTE(val)>>1;
	lowlight_mode = val&1;
	
	SetNRS_FNMisc(agc_max,under_exp,edge_level,lowlight_mode) ;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdNRStdev(WORD val)
{
	BYTE stdev_int ;
	BYTE stdev_frac ;	
	
	stdev_int = HIBYTE(val);
	stdev_frac = LOBYTE(val);
	
	SetNRS_FNStdev(stdev_int,stdev_frac);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdNRStdevBr(WORD val)
{
	BYTE stdev_int_br ;
	BYTE stdev_frac_br ;		
	
	stdev_int_br = HIBYTE(val);
	stdev_frac_br = LOBYTE(val);
	
	SetNRS_FNStdevBr(stdev_int_br,stdev_frac_br) ;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdNRStdevDa(WORD val)
{
	BYTE stdev_int_da ;
	BYTE stdev_frac_da ;		
	
	stdev_int_da = HIBYTE(val);
	stdev_frac_da = LOBYTE(val);
	
	SetNRS_FNStdevDa(stdev_int_da,stdev_frac_da) ;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdNRUpCnt(WORD val)
{
	SetNRS_FNUpdateCounter((BYTE)val) ;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdPMCtrl(BYTE cmd, BYTE val)
{
	BYTE no = HI4BIT(val);
	
	switch(cmd)	{
		case 0x01:	SetPM_OnOff((val)?  ON:  OFF);						break;
		case 0x02:	SetPM_RectOnOff(no, (LO4BIT(val))?  ON:  OFF);		break;
		case 0x03:	SetPM_FactoryDefaultMenu();							break;
		case 0x04:	stSPIsAPP.stMAINFO.pm_rect_no = val;				break;
		case 0x05:	SetPM_Type(val);									break;
		case 0x06:	SetPM_RectColor(no, (LO4BIT(val)));					break;
		case 0x11:	SetPM_PolyOnOff(no, (LO4BIT(val))?  ON:  OFF);		break;
		case 0x12:	SetPM_PolyColor(no, (LO4BIT(val)));					break;
		case 0x13:	stSPIsAPP.stMAINFO.pm_poly_no = no;
					stSPIsAPP.stMAINFO.pm_poly_edge = LO4BIT(val);		break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdPMRectSizeW(WORD w)
{
	stSPIsAPP.stMAINFO.temp_h16b = w;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdPMRectSizeH(WORD h)
{
	WORD w = stSPIsAPP.stMAINFO.temp_h16b;
	
	SetPM_RectSizeDirect(stSPIsAPP.stMAINFO.pm_rect_no, w, h);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdPMRectPosiX(WORD x)
{
	stSPIsAPP.stMAINFO.temp_h16b = x;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdPMRectPosiY(WORD y)
{
	WORD x = stSPIsAPP.stMAINFO.temp_h16b;
	
	SetPM_RectPosiDirect(stSPIsAPP.stMAINFO.pm_rect_no, x, y);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdPMPolyPosiX(WORD x)
{
	stSPIsAPP.stMAINFO.temp_h16b = x;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdPMPolyPosiY(WORD y)
{
	WORD x = stSPIsAPP.stMAINFO.temp_h16b;
	
	SetPM_PolyPosi(stSPIsAPP.stMAINFO.pm_poly_no, stSPIsAPP.stMAINFO.pm_poly_edge, x, y);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdGammaCtrl(BYTE cmd, BYTE val)
{
	switch(cmd)	{
		case 0x01:	DEMO_SetGammaValue(val);							break;
		case 0x02:	DEMO_SetFrontGammaEn(val);							break;
		case 0x03:	DEMO_SetRearGammaEn(val);							break;
		case 0x04:	DEMO_SetGamma8tap(val);								break;
		case 0x05:	DEMO_SetBAYER_LUT_Extend(val);						break;
		case 0x06:	DEMO_SetRGB_LUT_Extend(val);						break;
		case 0x07:	DEMO_SetYC_LUT_Extend(val);							break;
		case 0x08:	DEMO_SetBYLUT_Xresolution(val);						break;
		case 0x09:	DEMO_SetRGBLUT_Xresolution(val);					break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdEffectCtrl(BYTE cmd, BYTE val)
{
	switch(cmd)	{
		case 0x01:	DEMO_SetContrast(val);								break;
		case 0x02:	DEMO_SetBrightness(val);							break;
		case 0x03:	DEMO_SetSaturation(val);							break;
		case 0x04:	DEMO_SetHue(val);									break;
		case 0x05:	DEMO_SetSharpness(val);								break;
		case 0x06:	DEMO_SetAutoSaturation(val);						break;
		case 0x07:	DEMO_SetAutoEdgeOnOff(val);							break;
		case 0x08:	SetMenuDefaultItem(EFFECTDEFAULT_MENU);
					DEMO_SetEffectDefault();							break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdDZOOMCtrl(BYTE cmd, BYTE val)
{
	switch(cmd)	{
		case 0x01:	SetMenuStatus(PIPPOSI_MENU, 0);	// fix x position
					DEMO_SetPIPPosition(0);
					DEMO_SetZoomMode(val);								break;
		case 0x02:	DEMO_SetZoomRatio(val);								break;
		case 0x03:	SetMenuStatus(PIPPOSI_MENU, 0);	// fix x position
					DEMO_SetPIPPosition(0);
					DEMO_SetPIPMode(val);								break;
		case 0x04:	DEMO_SetPIPSize(val);								break;
		case 0x05:	SetMenuPosiY(PIPPOSI_MENU, val);
					SetMenuStatus(PIPPOSI_MENU, 0);	// fix x position
					DEMO_SetPIPPosition(0);								break;
		case 0x06:	SetMenuDefaultItem(DZDEFAULT_MENU);
					SetDZOOM_FactoryDefaultMenu();
					SetMenuStatus(PIPPOSI_MENU, 0);
					DEMO_SetPIPPosition(0);								break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdDefogCtrl(BYTE cmd, BYTE val)
{
	switch(cmd)	{
		case 0x01:	SetBY_DefogMode(val);								break;
		case 0x02:	SetBY_DefogStrength(val);							break;
		case 0x03:	SetBY_DefogThreshold(val);							break;
		case 0x04:	SetBY_DefogAutoLevel(val);							break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdDWDRCtrl(BYTE cmd, BYTE val)
{
	switch(cmd)	{
		case 0x01:	SetBY_DWDR_mode(val);								break;
		case 0x02:	SetBY_DWDRStrength(val);							break;
		case 0x03:	SetBY_DWDRSaturation(val);							break;
		case 0x04:	SetBY_DWDRLocalRatio(val);							break;
		case 0x05:	SetBY_DWDRSatSync(val);								break;
		case 0x06:	SetBY_DWDRAutoLevel(val);							break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdMDCtrl(BYTE cmd, BYTE val)
{
	BYTE no = HI4BIT(val);
	
	switch(cmd)	{
		case 0x01:	SetMD_OnOff((val)?  ON:  OFF);						break;
		case 0x02:	SetMD_AreaSelMenu(val);								break;
		case 0x03:	SetMD_AreaEn(no, (LO4BIT(val))?  ON:  OFF);			break;
		case 0x04:	SetMD_AreaDisp(no, (LO4BIT(val))?  ON:  OFF);		break;
		case 0x05:	stSPIsAPP.stMAINFO.md_area_no = val;				break;
		case 0x06:	SetMD_Threshold(val);								break;
		case 0x07:	SetMD_MotionView(val);								break;
		case 0x08:	SetMenuDefaultItem(MDDEFAULT_MENU);
					SetMD_FactoryDefaultMenu();							break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdMDPosi(WORD val)
{
	stSPIsAPP.stMAINFO.temp_h16b = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdMDSize(WORD val)
{
	BYTE no = stSPIsAPP.stMAINFO.md_area_no;
	BYTE lx = HIBYTE(stSPIsAPP.stMAINFO.temp_h16b);
	BYTE ly = LOBYTE(stSPIsAPP.stMAINFO.temp_h16b);
	BYTE w = HIBYTE(val);
	BYTE h = LOBYTE(val);
	
	SetMD_AreaSizePosi(no, w, h, lx, ly);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdBacklightCtrl(BYTE cmd, BYTE val)
{
	switch(cmd)	{
		case 0x01:	DEMO_SetBackLightMode(val);							break;
		case 0x02:	SetBLC_OnOff(val);									break;
		case 0x03:	SetBLC_WinOnOff(val);								break;
		case 0x04:	SetMenuDefaultItem(BLCDEFAULT_MENU);
					SetBLC_FactoryDefaultMenu(ON);						break;
		case 0x05:	SetHLC_WinOnOff(val);								break;
		case 0x06:	SetHLC_MaskLevel(val);								break;
		case 0x07:	SetMenuDefaultItem(val);
					SetHLC_FactoryDefaultMenu(ON);						break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdBLCPosi(BYTE x, BYTE y)
{
	SetBLC_Position(x, y);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdBLCSize(BYTE w, BYTE h)
{
	SetBLC_Size(w, h);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdHLCPosi(BYTE x, BYTE y)
{
	SetHLC_Position(x, y);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdHLCSize(BYTE w, BYTE h)
{
	SetHLC_Size(w, h);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdBinning(WORD nID)
{
	DEMO_BINNING_MODE(nID);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdDispFuncCtrl(BYTE cmd, BYTE val)
{
	switch(cmd)	{
		case 0x01:	DEMO_SetFreezeMode(val);							break;
		case 0x02:	DEMO_SetMirrorMode(val);							break;
		case 0x03:	DEMO_SetImageEffect(val);							break;
		case 0x04:	Demo_SetRotation(val);								break;
		case 0x05:	SetApp_ssPictureStyle(val);							break;
		case 0x06:	SetApp_ssPictureContrast(val);						break;
		case 0x07:	SetApp_ssPictureSaturation(val);					break;
		case 0x08:	SetApp_ssPictureEE(val);							break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdCamTitleCtrl(BYTE cmd, BYTE val)
{
	switch(HI4BIT(cmd))	{
		case 0x01:	stSPIsAPP.stMAINFO.camtitle_txt[LO4BIT(cmd)] = val;	return;	break;
	}
	
	switch(cmd)	{
		case 0x01:	OSD_CamTitleDisplay(val);							break;
		case 0x02:	stSPIsAPP.stMAINFO.camtitle_color = val;
					SaveCamTitleColor(val);
					OSD_CamTitleDisplay(ON);							break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdNTPAL(WORD nID)
{
	DEMO_SetNTPal(nID);
}






//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdFactoryReset(WORD val)
{
	DEMO_SetFactoryReset();
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdOutputFormat(WORD val)
{
#if defined(__USE_4K60_INOUT__)
	SetAPP_Set4K60OutFrmt(val);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdEngrCtrl(BYTE cmd, BYTE val)
{
	switch(cmd)	{
		case 0x01:	DEMO_SetInputPattern(val);							break;
		case 0x02:	SetOUT_MainTestPattern(val);
					SetOUT_AuxTestPattern(val);							break;
		case 0x03:	DEMO_SetTPCtrl(val);								break;
		case 0x04:	DEMO_SetTPOnOffCtrl(val);							break;
		case 0x05:	SetMenuDefaultItem(TPDEFAULT_MENU);
					DEMO_SetTPDefault();								break;
		case 0x06:	DEMO_SetB_BYPASSCtrl(val);							break;
		case 0x07:	DEMO_SetSensorFps(val);								break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdCCCtrl(BYTE cmd, BYTE val)
{
	BYTE area = stSPIsAPP.stMAINFO.cac_area;
	
	switch(cmd)	{
		case 0x01:	SetDM_ColorCCtrl_Red(val);							break;
		case 0x02:	SetDM_ColorCCtrl_Green(val);						break;
		case 0x03:	SetDM_ColorCCtrl_Blue(val);							break;
		case 0x04:	stSPIsAPP.stMAINFO.cac_area = val;					break;
		case 0x05:	SetApp_CACTheta(val, area);							break;
		case 0x06:	SetApp_CACSaturation(val, area);					break;
		case 0x07:	SetApp_CACCos(val, area);							break;
		case 0x08:	SetApp_CACSin(val, area);							break;
		case 0x09:	SetApp_CAC_WriteEEPROM();							break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdSSTPPosi(BYTE x, BYTE y)
{
	SetMenuStatus(TP_POSI_MENU, x);
	SetMenuPosiY(TP_POSI_MENU, y);
	DEMO_SetTPPosition(0);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdSSTPSize(BYTE w, BYTE h)
{
	SetMenuStatus(TP_SIZE_MENU, w);
	SetMenuPosiY(TP_SIZE_MENU, h);
	DEMO_SetTPSize(0);
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdMISCTUNECtrl(BYTE cmd, BYTE val)
{
	switch(cmd)	{
		case 0x01:	DEMO_SetYCConv(val);								break;
		case 0x02:	DEMO_SetOUTCSCDefine(val);							break;
		case 0x03:	SetOUT_MainDithering(val);							break;
		case 0x06:	DEMO_SetHdmiTxMode(val);							break;
		case 0x07:	SPIsAPP_Cmd4K30P60POSD(val);						break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_Cmd4K30P60POSD(WORD OnOff)
{
#if defined(__USE_4K60_INOUT__)
	SetAPP_Set4K30P60P_Mode((OnOff)?  ON:  OFF);
	
	if(OnOff==OFF)		SetOUT_MainFreezeEN(OFF);
#endif
	
#if defined(__USE_OSD_TITLE__)
	OSD_SetTitleBMP((OnOff)?  ON:  OFF,  0x02);		// 2 : 4k30p, 5 : 4k60p
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdSDPCCtrl(BYTE cmd, BYTE val)
{
	switch(cmd)	{
		case 0x01:	DEMO_StatDPCRun(val);								break;
		case 0x02:	SetSDPC_MemWrite_Order();							break;
		case 0x03:	SetSDPC_MemRead_Order();							break;
		case 0x04:	SetSDPC_MemReset_Order();							break;
		case 0x05:	SetSDPC_MemReload_Order();							break;
		case 0x06:	SetSDPC_FlashPrint_Order();							break;
		case 0x07:	SetSDPC_FlashUpdate_Order();						break;
		case 0x08:	SetSDPC_FlashWrite_Order();							break;
		case 0x09:	SetSDPC_FlashClear_Order();							break;
		case 0x0A:	SetSDPC_FlashRead_Order();							break;
		case 0x0B:	SetSDPC_FlashTempRead_Order();						break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_CmdIREdgeOnOff(WORD nID)
{
	SetIN_MainHoriPeaking((nID)?  ON:  OFF);
	SetFC_MainVertPeaking((nID)?  ON:  OFF);
}



//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------

// inquiry
//-------------------------------------------------------------------------------------
WORD ISPM SPIsAPP_InqRegRead(WORD addr)
{
	WORD rVal=0;
	
	switch(HI4BIT(HIBYTE(addr))|0xC0)	{
		case  MISP_HOST_ID:	MISP_RegRead(MISP_HOST_ID, (addr&0xFFF), &rVal);		break;
		case  MISP_LOCAL_ID:	MISP_RegRead(MISP_LOCAL_ID, (addr&0xFFF), &rVal);	break;
		//case  MISP_SDRAM_ID:	MISP_RegRead(MISP_SDRAM_ID, (addr&0xFFF), &rVal);	break;
	}
	
	return rVal;
}

//--------------------------------------------------------------------------------------------------------------------------
WORD ISPM SPIsAPP_Inq3aTxSize(void)
{
	stSPIsAPP.stTXRAW.cnt_tx = 0;							// clear tx counter
	stSPIsAPP.stTXRAW.txsize = stSPIsAPP.stTXRAW.size_3a;	// set tx size
	return stSPIsAPP.stTXRAW.txsize;
}

//--------------------------------------------------------------------------------------------------------------------------
WORD ISPM SPIsAPP_InqNRTxSize(void)
{
	stSPIsAPP.stTXRAW.cnt_tx = 0;							// clear tx counter
	stSPIsAPP.stTXRAW.txsize = stSPIsAPP.stTXRAW.size_nr;	// set tx size
	return stSPIsAPP.stTXRAW.txsize;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SPIsAPP_Inq3aRaw(PDWORD pBuff)
{
	PSPIs_TXRAW_PARAM pTXRAW = &stSPIsAPP.stTXRAW;
	
	if(pTXRAW->cnt_tx >= pTXRAW->txsize)	{					// tx all done ?
		stSPIsCTRL.ctrl &= (~SPIS_TX_MODE_MASK);		return;	// clear tx mode & counter
	}
	
	// move raw data to tx buffer
	*pBuff = pTXRAW->buff_3a[pTXRAW->cnt_tx++];
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SPIsAPP_InqNRRaw(PDWORD pBuff)
{
	PSPIs_TXRAW_PARAM pTXRAW = &stSPIsAPP.stTXRAW;
	
	if(pTXRAW->cnt_tx >= pTXRAW->txsize)	{					// tx all done ?
		stSPIsCTRL.ctrl &= (~SPIS_TX_MODE_MASK);		return;	// clear tx mode & counter
	}
	
	// move raw data to tx buffer
	*pBuff = pTXRAW->buff_nr[pTXRAW->cnt_tx++];
}

//--------------------------------------------------------------------------------------------------------------------------
WORD ISPM SPIsAPP_InqFBcapStatus(WORD val)
{
	PSPIs_FBCAP_PARAM pCAP = &stSPIsAPP.stFBCAP;
	
	if(pCAP->ctrl & SPIS_FBCAP_CTRL_RUN)	{
		stSPIsCTRL.ctrl &= SPIS_TX_MODE_MASK;	stSPIsCTRL.ctrl |= SPIS_TX_MODE_FB_DATA;	// set tx flag
	}
	
	return (pCAP->ctrl & SPIS_FBCAP_CTRL_RUN)?  1:  0;	// ack code(0:stop, 1:run)
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM SPIsAPP_SetFBcapDDRaddr(PSPIs_FBCAP_PARAM pCAP)
{
	if(pCAP->rd_x == pCAP->lx)	{	// every new line
		if(pCAP->cnt_rd < pCAP->txsize/2)	{
			pCAP->e_addr_rd = pCAP->e_addr_y;	// y
			pCAP->e_addr_rd += pCAP->ly * pCAP->cnt_col + pCAP->lx;					// x,y start offset
			pCAP->e_addr_rd += (pCAP->rd_y - pCAP->ly) * pCAP->cnt_col;				// calc y offset
		}
		else	{
			pCAP->e_addr_rd = pCAP->e_addr_c;	// c
			pCAP->e_addr_rd += pCAP->ly * pCAP->cnt_col + pCAP->lx;					// x,y start offset
			pCAP->e_addr_rd += (pCAP->rd_y - pCAP->h - pCAP->ly) * pCAP->cnt_col;	// calc y offset
		}
		
		pCAP->e_addr_rd -= (pCAP->lx % 8);		// read 8bytes from prev. -- new line x(not 8x)
	}
	else	pCAP->e_addr_rd += 8;
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM SPIsAPP_SetFBcapDDRread(PSPIs_FBCAP_PARAM pCAP)
{
	BYTE bytes, idx, i;
	
	if((pCAP->rd_x % 8 == 0))	{
		if(pCAP->rd_x + 8 < pCAP->lx + pCAP->w)	{			// normal
			bytes = 8;							idx = 0;
			pCAP->rd_x += 8;
		}
		else if(pCAP->rd_x + 8 == pCAP->lx + pCAP->w)	{	// last bytes(8x)
			bytes = 8;							idx = 0;
			pCAP->rd_x = pCAP->lx;	pCAP->rd_y++;
		}
		else	{											// last bytes(not 8x)
			bytes = (pCAP->lx + pCAP->w) % 8;	idx = 8 - bytes;
			pCAP->rd_x = pCAP->lx;	pCAP->rd_y++;
		}
	}														// new line x(not 8x)
	else	{
		bytes = 8 - (pCAP->rd_x % 8);			idx = 0;				
		pCAP->rd_x += 8 - (pCAP->rd_x % 8);
	}
	
	// [0] [1] [2] [3]    [4] [5] [6] [7] ,, -- read buff
	// P7  P6  P5  P4     P3  P2  P1  P0     P15  P14  P13  P12 ,,
	MISP_MultiRead(MISP_SDRAM_ID, pCAP->e_addr_rd, &pCAP->rd_buff[0], 8);	// ddr read
	for(i=0;i<bytes;i++)	{
		pCAP->raw_buff[pCAP->cnt_push] = pCAP->rd_buff[bytes-1-i+idx];		// move(read buffer -> raw buffer, P0,P1,P2,,,)
		
		if(pCAP->cnt_push >= 15)	pCAP->cnt_push=0;		else	pCAP->cnt_push++;
	}
	
	pCAP->cnt_rd += bytes;
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM SPIsAPP_SetFBcapTxBuff(PSPIs_FBCAP_PARAM pCAP)
{
	BYTE i;
	
	for(i=0;i<4;i++)	{
		pCAP->tx_buff[i] = pCAP->raw_buff[pCAP->cnt_pop];	// move(raw buffer -> tx buffer)
		
		if(pCAP->cnt_pop >= 15)		pCAP->cnt_pop=0;		else	pCAP->cnt_pop++;
	}
	
	pCAP->cnt_tx += 4;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SPIsAPP_InqFBcapRaw(PDWORD pBuff)
{
	PSPIs_FBCAP_PARAM pCAP = &stSPIsAPP.stFBCAP;
	
	if(pCAP->cnt_tx >= pCAP->txsize)	{						// tx all done ??
		stSPIsCTRL.ctrl &= (~SPIS_TX_MODE_FB_DATA);		return;	// clear tx mode
	}
	
	// read frame buffer
	if(pCAP->cnt_rd < (pCAP->cnt_tx + 4))	{
		SPIsAPP_SetFBcapDDRaddr(pCAP);
		SPIsAPP_SetFBcapDDRread(pCAP);
		
		if(pCAP->cnt_rd < (pCAP->cnt_tx + 4))	{
			SPIsAPP_SetFBcapDDRaddr(pCAP);
			SPIsAPP_SetFBcapDDRread(pCAP);
		}
	}
	
	// move raw data to tx buffer
	SPIsAPP_SetFBcapTxBuff(pCAP);
	memcpy(pBuff, &pCAP->tx_buff[0], 4);
}


// get/set func.
//--------------------------------------------------------------------------------------------------------------------------
BOOL SPIsAPP_GetMasterDYNTStatus(void)
{
	return	stSPIsAPP.stMAINFO.dynt_status;
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_GetMasterCamTitleTxt(PBYTE pBuff)
{
	memcpy(pBuff, &stSPIsAPP.stMAINFO.camtitle_txt[0], sizeof(stSPIsAPP.stMAINFO.camtitle_txt));
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIsAPP_GetMasterCamTitleColor(PBYTE pBuff)
{
	memcpy(pBuff, &stSPIsAPP.stMAINFO.camtitle_color, sizeof(stSPIsAPP.stMAINFO.camtitle_color));
}



#endif	//__USE_CPU_SPI_SLAVE__


/*  FILE_END_HERE */
