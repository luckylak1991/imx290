//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2016  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	SPIM_APP.C
// Description 		: spi-master application
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"

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

//---------------------------------------------------------------------------------
MISP_ERROR_t SPImAPP_CmdRegMultiWrite(BYTE cID, BYTE region, DWORD addr, PBYTE pBuff, DWORD bytes)
{
	DWORD cmd_addr=0, i;
	
	switch(region&0xfe)	{
		case MISP_HOST_ID:		cmd_addr = SPIM_CMD_REG_ADDR + 0x0000 + addr; break;
		case MISP_LOCAL_ID:	cmd_addr = SPIM_CMD_REG_ADDR + 0x2000 + addr; break;
		//case MISP_SDRAM_ID:	cmd_addr = SPIM_CMD_REG_ADDR + 0x6000 + addr; break;
		default:				return MISP_SPI_ERROR;
	}
	
	for (i=0; i<bytes/2; i++) {
		if(SPIm_Command(cID, cmd_addr+i))		return MISP_SPI_ERROR;
		if(SPIm_Command(cID, SPIM_CMD_REG_WRITE + ((PWORD)pBuff)[i]))	return MISP_SPI_ERROR;
	}
	
	return MISP_NO_ERROR;
}

//---------------------------------------------------------------------------------
MISP_ERROR_t SPImAPP_CmdRegWrite(BYTE cID, BYTE region, DWORD addr, WORD wData)
{
	if(SPImAPP_CmdRegMultiWrite(cID, region, addr, (PBYTE)&wData, 2))		return MISP_SPI_ERROR;
	return MISP_NO_ERROR;
}

//---------------------------------------------------------------------------------
MISP_ERROR_t SPImAPP_CmdStd(BYTE cID, DWORD cmd, WORD option)
{
	if(SPIm_Command(cID, cmd + option))		return MISP_SPI_ERROR;
	return MISP_NO_ERROR;
}

//---------------------------------------------------------------------------------
MISP_ERROR_t SPImAPP_InqRegMultiRead(BYTE cID, BYTE region, DWORD addr, PBYTE pBuff, DWORD bytes)
{
	BYTE reply[4];
	DWORD inq_addr=0, i;
	
	switch(region&0xfe)	{
		case MISP_HOST_ID:		inq_addr = SPIM_INQ_REG_READ + 0x0000 + addr; break;
		case MISP_LOCAL_ID:	inq_addr = SPIM_INQ_REG_READ + 0x2000 + addr; break;
		//case MISP_SDRAM_ID:	inq_addr = SPIM_INQ_REG_READ + 0x6000 + addr; break;
		default:				return MISP_SPI_ERROR;
	}
	
	for (i=0; i<bytes/2; i++) {
		SPIm_Inquiry(cID, inq_addr+i, &reply[0]);
		
		if(reply[3]==0x50)		((PWORD)pBuff)[i]  = MAKEWORD(reply[1], reply[0]);	// check reply ack code
		else					return MISP_SPI_ERROR;
	}
	
	return MISP_NO_ERROR;
}

//---------------------------------------------------------------------------------
MISP_ERROR_t SPImAPP_InqRegRead(BYTE cID, BYTE region, DWORD addr, PWORD rData)
{
	if(SPImAPP_InqRegMultiRead(cID, region, addr, (PBYTE)rData, 2))		return MISP_SPI_ERROR;
	return MISP_NO_ERROR;
}

//---------------------------------------------------------------------------------
MISP_ERROR_t SPImAPP_InqStd(BYTE cID, DWORD inq, WORD option, PDWORD rData)
{
	SPIm_Inquiry(cID, inq + option, (PBYTE)rData);
	
	if(HIBYTE(HIWORD(*rData)) == 0x50)		return MISP_NO_ERROR;		// check reply ack code.
	else 									return MISP_SPI_ERROR;
}

// 3A master app
//---------------------------------------------------------------------------------
void SPImAPP_RxSlaveRawData(void)
{
	BYTE raw_data_size;
	
//	if((stSPImAPP.ctrl&0xff) < 60*5)			{stSPImAPP.ctrl++;	return;}		// wait 5 sec after system boot.

	SPIm_Inq3aRawData(SPIM_MISP_CID_S2, (PBYTE)&stSPImAPP.stRX3A, &raw_data_size);	// read slave 3a raw data
#if defined(__USE_IRDATA_SLAVE2MASER__)
	UARTprintf("irdata fr. slave : %d\r\n",stSPImAPP.stRX3A.irdata);

	//DWORD irdata;
	//SPImAPP_GetSlaveIRdata((PBYTE)&irdata);
	//if(irdata != 0xff) {
	//	UARTprintf("irdata : %d\r\n",irdata);
	//	IrCodeSetIrComCommand(irdata);
	//}
#endif

#if defined(__USE_4K60_MD1__)
 	SPIm_InqNRRawData(SPIM_MISP_CID_S2, (PBYTE)&stSPImAPP.stRXNR, &raw_data_size);	// read slave raw data
#endif
}

//---------------------------------------------------------------------------------
void SPImAPP_GetSlaveAEysum(PBYTE pBuff)
{
	memcpy(pBuff, &stSPImAPP.stRX3A.ysum[0], sizeof(stSPImAPP.stRX3A.ysum));
}

//---------------------------------------------------------------------------------
void SPImAPP_GetSlaveAEgain(PBYTE pBuff)
{
	memcpy(pBuff, &stSPImAPP.stRX3A.gain, sizeof(stSPImAPP.stRX3A.gain));
}

//---------------------------------------------------------------------------------
void SPImAPP_GetSlaveAEiris(PBYTE pBuff)
{
	memcpy(pBuff, &stSPImAPP.stRX3A.iris, sizeof(stSPImAPP.stRX3A.iris));
}

#if defined(__USE_IRDATA_SLAVE2MASER__)
//---------------------------------------------------------------------------------
void SPImAPP_GetSlaveIRdata(PBYTE pBuff)
{
	memcpy(pBuff, &stSPImAPP.stRX3A.irdata, sizeof(stSPImAPP.stRX3A.irdata));
}
#endif

//---------------------------------------------------------------------------------
void SPImAPP_GetSlaveAFfiltA(PBYTE pBuff)	// &stFILT.RPFa
{
	memcpy(pBuff, &stSPImAPP.stRX3A.RPFa, 24);	// RPFa, LPFa, VPFa, BPFa, HPFa, CNTa
}

//---------------------------------------------------------------------------------
void SPImAPP_GetSlaveAFfiltB(PBYTE pBuff)	// &stFILT.RPFb
{
	memcpy(pBuff, &stSPImAPP.stRX3A.RPFb, 24);	// RPFb, LPFb, VPFb, BPFb, HPFb, CNTb
}

//---------------------------------------------------------------------------------
void SPImAPP_GetSlaveNRStdev(PWORD pstdev_int, PWORD pstdev_frac)
{
	*pstdev_int = stSPImAPP.stRXNR.stdev_int;
	*pstdev_frac = stSPImAPP.stRXNR.stdev_frac;
}

//---------------------------------------------------------------------------------
void SPImAPP_GetSlaveNRStdevBr(PWORD pstdev_int_br, PWORD pstdev_frac_br)
{
	*pstdev_int_br = stSPImAPP.stRXNR.stdev_int_br;
	*pstdev_frac_br = stSPImAPP.stRXNR.stdev_frac_br;
}

//---------------------------------------------------------------------------------
void SPImAPP_GetSlaveNRStdevDa(PWORD pstdev_int_da, PWORD pstdev_frac_da)
{
	*pstdev_int_da = stSPImAPP.stRXNR.stdev_int_da;
	*pstdev_frac_da = stSPImAPP.stRXNR.stdev_frac_da;
}



//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveWBgain(WORD gainR, WORD gainB)
{
#if defined (__PANORAMA_SETUP__) && defined(__PANORAMA_02__)	// panorama master only
	if(stSPImAPP.stTX.WBgainR != gainR)	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S1, SPIM_CMD_WBGAINR, ConvApp_Pano_WB_GainR(SPIM_MISP_CID_S1, gainR));
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_WBGAINR, ConvApp_Pano_WB_GainR(SPIM_MISP_CID_S2, gainR));
	}
	if(stSPImAPP.stTX.WBgainB != gainB)	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S1, SPIM_CMD_WBGAINB, ConvApp_Pano_WB_GainB(SPIM_MISP_CID_S1, gainB));
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_WBGAINB, ConvApp_Pano_WB_GainB(SPIM_MISP_CID_S2, gainB));
	}
#endif

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	if(stSPImAPP.stTX.WBgainR != gainR)		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_WBGAINR, gainR);
	if(stSPImAPP.stTX.WBgainB != gainB)		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_WBGAINB, gainB);
#endif
	
	stSPImAPP.stTX.WBgainR = gainR;
	stSPImAPP.stTX.WBgainB = gainB;
	
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveAEDGain(WORD gain)
{
	if(stSPImAPP.stTX.AEDGain != gain)	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_AEDGAIN, gain);
		stSPImAPP.stTX.AEDGain = gain;
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveAEGainGain(LONG gain)
{
	if(stSPImAPP.stTX.AEGainGain != gain)	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_AEGAINGAINH, (gain>>16));
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_AEGAINGAINL, (gain&0xFFFF));
		stSPImAPP.stTX.AEGainGain = gain;
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveAEGainMax(LONG val)
{
	if(stSPImAPP.stTX.AEGainMax != val)	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_AEGAINMAXH, HIWORD(val));
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_AEGAINMAXL, LOWORD(val));
		stSPImAPP.stTX.AEGainMax = val;
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveAEGainMin(LONG val)
{
	if(stSPImAPP.stTX.AEGainMin != val)	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_AEGAINMINH, HIWORD(val));
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_AEGAINMINL, LOWORD(val));
		stSPImAPP.stTX.AEGainMin = val;
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveAEGainPMax(LONG val)
{
	if(stSPImAPP.stTX.AEGainPMax != val)	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_AEGAINPMAXH, HIWORD(val));
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_AEGAINPMAXL, LOWORD(val));
		stSPImAPP.stTX.AEGainPMax = val;
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveAEYSum(LONG val)
{
	if(stSPImAPP.stTX.AEYSum != val)	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_AEYSUMH, HIWORD(val));
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_AEYSUML, LOWORD(val));
		stSPImAPP.stTX.AEYSum = val;
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveAEBright(WORD val)
{
	if(stSPImAPP.stTX.AEBright != val)	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_AEBRIGHT, val);
		stSPImAPP.stTX.AEBright = val;
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveLLSharpness(WORD val)
{
	if(stSPImAPP.stTX.ll_sharpness != val)	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_LLSHARPNESS, val);
		stSPImAPP.stTX.ll_sharpness = val;
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveLLSaturation(WORD val)
{
	if(stSPImAPP.stTX.ll_saturation != val)	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_LLSATURATION, val);
		stSPImAPP.stTX.ll_saturation = val;
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveCSPLS(BYTE x0, BYTE x1)
{
	if(stSPImAPP.stTX.csp_ls_x != MAKEWORD(x0, x1))	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CSPLS, MAKEWORD(x0, x1));
		stSPImAPP.stTX.csp_ls_x = MAKEWORD(x0, x1);
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveCACGgain(WORD val)
{
	if(stSPImAPP.stTX.cac_g_gain != val)	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CACGGAIN, val);
		stSPImAPP.stTX.cac_g_gain = val;
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveCACMgain(WORD val)
{
	if(stSPImAPP.stTX.cac_m_gain != val)	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CACMGAIN, val);
		stSPImAPP.stTX.cac_m_gain = val;
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveDYNTStatus(BOOL status)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DYNTCTRL, MAKEWORD(0x01, status));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveDYNTNightColor(BOOL onoff)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DYNTCTRL, MAKEWORD(0x02, onoff));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveDYNTColorBurst(BOOL onoff)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DYNTCTRL, MAKEWORD(0x03, onoff));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveNRMode(MISP_NR_MODE_t val)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_NRMODE, (WORD)val);
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveNRIntensity(MISP_NR3D_MODE_t val)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_NRINTENSITY, (WORD)val);
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveNRAperture(WORD val)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_NRAPERTURE, val);
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveNRZoomMode(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_NRZOOMMODE, 0x0000);
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveNRMISC(BYTE agc_max, BYTE under_exp, BYTE edge_level, BYTE lowlight_mode)
{
	WORD nr_fr_misc = MAKEWORD(MAKEBYTE(agc_max, under_exp), ((edge_level<<1) | (lowlight_mode&1)));
	
	if(stSPImAPP.stTX.nr_fr_misc != nr_fr_misc)	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_NRMISC, nr_fr_misc);
		stSPImAPP.stTX.nr_fr_misc = nr_fr_misc;
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveNRStdev(WORD stdev_int, WORD stdev_frac)
{
	if(stSPImAPP.stTX.nr_fr_stdev != MAKEWORD(stdev_int, stdev_frac))	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_NRSTDEV, MAKEWORD(stdev_int, stdev_frac));
		stSPImAPP.stTX.nr_fr_stdev = MAKEWORD(stdev_int, stdev_frac);
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveNRStdevBr(WORD stdev_int_br, WORD stdev_frac_br)
{
	if(stSPImAPP.stTX.nr_fr_stdev_br != MAKEWORD(stdev_int_br, stdev_frac_br))	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_NRSTDEVBR, MAKEWORD(stdev_int_br, stdev_frac_br));
		stSPImAPP.stTX.nr_fr_stdev_br = MAKEWORD(stdev_int_br, stdev_frac_br);
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveNRStdevDa(WORD stdev_int_da, WORD stdev_frac_da)
{
	if(stSPImAPP.stTX.nr_fr_stdev_da != MAKEWORD(stdev_int_da, stdev_frac_da))	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_NRSTDEVDA, MAKEWORD(stdev_int_da, stdev_frac_da));
		stSPImAPP.stTX.nr_fr_stdev_da = MAKEWORD(stdev_int_da, stdev_frac_da);
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveNRUpCnt(BYTE up_cnt)
{
	if(stSPImAPP.stTX.nr_up_cnt != up_cnt)	{
		SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_NRUPCNT, up_cnt);
		stSPImAPP.stTX.nr_up_cnt = up_cnt;
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlavePMOnOff(BOOL onoff)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PMCTRL, MAKEWORD(0x01, onoff));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlavePMRectOnOff(BYTE no, BOOL onoff)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PMCTRL, MAKEWORD(0x02, MAKEBYTE(no, onoff)));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlavePMDefault(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PMCTRL, MAKEWORD(0x03, 0x00));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlavePMRectSize(BYTE no, WORD w, WORD h)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PMCTRL, MAKEWORD(0x04, no));
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PMRECTSIZEW, w);
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PMRECTSIZEH, h);
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlavePMRectPosi(BYTE no, WORD x, WORD y)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PMCTRL, MAKEWORD(0x04, no));
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PMRECTPOSIX, x);
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PMRECTPOSIY, y);
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlavePMType(BYTE type)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PMCTRL, MAKEWORD(0x05, type));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlavePMRectColor(BYTE no, BYTE color)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PMCTRL, MAKEWORD(0x06, MAKEBYTE(no, color)));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlavePMPolyOnOff(BYTE no, BYTE onoff)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PMCTRL, MAKEWORD(0x11, MAKEBYTE(no, onoff)));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlavePMPolyColor(BYTE no, BYTE color)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PMCTRL, MAKEWORD(0x12, MAKEBYTE(no, color)));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlavePMPolyPosi(BYTE no, BYTE edge, WORD x, WORD y)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PMCTRL, MAKEWORD(0x13, MAKEBYTE(no, edge)));
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PMPOLYPOSIX, x);
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_PMPOLYPOSIY, y);
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveGammaOnOff(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_GMCTRL, MAKEWORD(0x01, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveGammaFrontEn(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_GMCTRL, MAKEWORD(0x02, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveGammaRearEn(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_GMCTRL, MAKEWORD(0x03, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveGamma8tap(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_GMCTRL, MAKEWORD(0x04, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveGammaBYLUTExt(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_GMCTRL, MAKEWORD(0x05, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveGammaRGBLUTExt(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_GMCTRL, MAKEWORD(0x06, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveGammaYCLUTExt(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_GMCTRL, MAKEWORD(0x07, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveGammaBYLUTXresol(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_GMCTRL, MAKEWORD(0x08, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveGammaRGBLUTXresol(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_GMCTRL, MAKEWORD(0x09, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveEffectContrast(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_EFFECTCTRL, MAKEWORD(0x01, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveEffectBrightness(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_EFFECTCTRL, MAKEWORD(0x02, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveEffectSaturation(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_EFFECTCTRL, MAKEWORD(0x03, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveEffectHue(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_EFFECTCTRL, MAKEWORD(0x04, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveEffectEdgeEnhance(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_EFFECTCTRL, MAKEWORD(0x05, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveEffectAutoSaturate(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_EFFECTCTRL, MAKEWORD(0x06, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveEffectAutoEdge(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_EFFECTCTRL, MAKEWORD(0x07, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveEffectDefault(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_EFFECTCTRL, MAKEWORD(0x08, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveDZOOMMode(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DZOOMCTRL, MAKEWORD(0x01, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveDZOOMRatio(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DZOOMCTRL, MAKEWORD(0x02, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlavePIPMode(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DZOOMCTRL, MAKEWORD(0x03, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlavePIPSize(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DZOOMCTRL, MAKEWORD(0x04, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlavePIPPosi(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DZOOMCTRL, MAKEWORD(0x05, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveDZOOMDefault(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DZOOMCTRL, MAKEWORD(0x06, 0x00));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveDefogMode(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DEFOGCTRL, MAKEWORD(0x01, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveDefogStrength(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DEFOGCTRL, MAKEWORD(0x02, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveDefogThreshold(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DEFOGCTRL, MAKEWORD(0x03, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveDefogAutoLevel(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DEFOGCTRL, MAKEWORD(0x04, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveDWDRMode(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DWDRCTRL, MAKEWORD(0x01, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveDWDRStrength(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DWDRCTRL, MAKEWORD(0x02, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveDWDRSaturation(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DWDRCTRL, MAKEWORD(0x03, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveDWDRLocalRatio(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DWDRCTRL, MAKEWORD(0x04, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveDWDRSatSync(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DWDRCTRL, MAKEWORD(0x05, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveDWDRAutoLevel(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DWDRCTRL, MAKEWORD(0x06, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveMDOnOff(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_MDCTRL, MAKEWORD(0x01, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveMDAreaSel(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_MDCTRL, MAKEWORD(0x02, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveMDAreaOnOff(BYTE no, BYTE onoff)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_MDCTRL, MAKEWORD(0x03, MAKEBYTE(no, onoff)));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveMDAreaDisp(BYTE no, BYTE onoff)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_MDCTRL, MAKEWORD(0x04, MAKEBYTE(no, onoff)));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveMDSizePosi(BYTE no, BYTE w, BYTE h, BYTE lx, BYTE ly)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_MDCTRL, MAKEWORD(0x05, no));
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_MDPOSI, MAKEWORD(lx, ly));
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_MDSIZE, MAKEWORD(w, h));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveMDThreshold(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_MDCTRL, MAKEWORD(0x06, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveMDMotionView(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_MDCTRL, MAKEWORD(0x07, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveMDDefault(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_MDCTRL, MAKEWORD(0x08, 0x00));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveBackLightMode(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_BACKLIGHTCTRL, MAKEWORD(0x01, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveBLCOnOff(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_BACKLIGHTCTRL, MAKEWORD(0x02, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveBLCDisp(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_BACKLIGHTCTRL, MAKEWORD(0x03, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveBLCDefault(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_BACKLIGHTCTRL, MAKEWORD(0x04, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveBLCPosi(BYTE x, BYTE y)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_BLCPOSI, MAKEWORD(x, y));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveBLCSize(BYTE w, BYTE h)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_BLCSIZE, MAKEWORD(w, h));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveHLCDispOnOff(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_BACKLIGHTCTRL, MAKEWORD(0x05, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveHLCLevel(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_BACKLIGHTCTRL, MAKEWORD(0x06, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveHLCBlackMask(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_BACKLIGHTCTRL, MAKEWORD(0x07, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveHLCDefault(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_BACKLIGHTCTRL, 0x0000);
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveHLCPosi(BYTE x, BYTE y)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_HLCPOSI, MAKEWORD(x, y));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveHLCSize(BYTE w, BYTE h)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_HLCSIZE, MAKEWORD(w, h));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveBinning(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_BINNING, nID);
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveDDPCReg(PWORD pBuff)
{
	BYTE i;
	
	for(i=0;i<5;i++)	{
		if(stSPImAPP.stTX.ddpc_reg[i] != pBuff[i])	{
			SPImAPP_CmdRegWrite(SPIM_MISP_CID_S2, MISP_LOCAL_ID, 0x403+((i<4)?  i:  9), pBuff[i]);
			stSPImAPP.stTX.ddpc_reg[i] = pBuff[i];
		}
	}
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveSDPCRun(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_SDPCCTRL, MAKEWORD(0x01, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveSDPCMemWriteOrder(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_SDPCCTRL, MAKEWORD(0x02, 0x00));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveSDPCMemReadOrder(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_SDPCCTRL, MAKEWORD(0x03, 0x00));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveSDPCMemResetOrder(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_SDPCCTRL, MAKEWORD(0x04, 0x00));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveSDPCMemReloadOrder(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_SDPCCTRL, MAKEWORD(0x05, 0x00));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveSDPCFlashPrintOrder(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_SDPCCTRL, MAKEWORD(0x06, 0x00));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveSDPCFlashUpdateOrder(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_SDPCCTRL, MAKEWORD(0x07, 0x00));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveSDPCFlashWriteOrder(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_SDPCCTRL, MAKEWORD(0x08, 0x00));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveSDPCFlashClearOrder(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_SDPCCTRL, MAKEWORD(0x09, 0x00));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveSDPCFlashReadOrder(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_SDPCCTRL, MAKEWORD(0x0A, 0x00));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveSDPCFlashTempReadOrder(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_SDPCCTRL, MAKEWORD(0x0B, 0x00));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveFreezeMode(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DISPFUNCCTRL, MAKEWORD(0x01, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveMirrorMode(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DISPFUNCCTRL, MAKEWORD(0x02, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveImageEffect(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DISPFUNCCTRL, MAKEWORD(0x03, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveRotation(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DISPFUNCCTRL, MAKEWORD(0x04, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlavePicStyle(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DISPFUNCCTRL, MAKEWORD(0x05, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlavePicContrast(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DISPFUNCCTRL, MAKEWORD(0x06, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlavePicSaturation(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DISPFUNCCTRL, MAKEWORD(0x07, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlavePicEdgeEnhance(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_DISPFUNCCTRL, MAKEWORD(0x08, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveCamTitleDisplay(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CAMTITLECTRL, MAKEWORD(0x01, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveCamTitleTxt(BYTE idx, BYTE txt)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CAMTITLECTRL, MAKEWORD(0x10|idx, txt));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveCamTitleColor(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CAMTITLECTRL, MAKEWORD(0x02, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveNTPAL(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_NTSCPAL, nID);
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveFactoryReset(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_FACTORYRESET, 0x0000);
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveOutputFormat(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_OUTPUTFORMAT, nID);
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveCCRed(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CCCTRL, MAKEWORD(0x01, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveCCGreen(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CCCTRL, MAKEWORD(0x02, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveCCBlue(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CCCTRL, MAKEWORD(0x03, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveCACTheta(WORD level, WORD area)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CCCTRL, MAKEWORD(0x04, area));
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CCCTRL, MAKEWORD(0x05, level));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveCACSaturation(WORD level, WORD area)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CCCTRL, MAKEWORD(0x04, area));
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CCCTRL, MAKEWORD(0x06, level));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveCACCos(WORD level, WORD area)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CCCTRL, MAKEWORD(0x04, area));
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CCCTRL, MAKEWORD(0x07, level));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveCACSin(WORD level, WORD area)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CCCTRL, MAKEWORD(0x04, area));
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CCCTRL, MAKEWORD(0x08, level));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveCACWriteEEPROM(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_CCCTRL, MAKEWORD(0x09, 0x00));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveInPattern(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_ENGRCTRL, MAKEWORD(0x01, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveOutPattern(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_ENGRCTRL, MAKEWORD(0x02, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveTPSel(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_ENGRCTRL, MAKEWORD(0x03, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveTPOnOff(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_ENGRCTRL, MAKEWORD(0x04, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveTPPosi(BYTE x, BYTE y)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_SSTPPOSI, MAKEWORD(x, y));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveTPSize(BYTE w, BYTE h)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_SSTPSIZE, MAKEWORD(w, h));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveTPDefault(void)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_ENGRCTRL, MAKEWORD(0x05, 0x00));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveByrBypass(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_ENGRCTRL, MAKEWORD(0x06, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveSensorFps(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_ENGRCTRL, MAKEWORD(0x07, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveYCConv(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_MISCTUNECTRL, MAKEWORD(0x01, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveOutCSC(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_MISCTUNECTRL, MAKEWORD(0x02, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveOutDither(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_MISCTUNECTRL, MAKEWORD(0x03, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveHdmiTxMode(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_MISCTUNECTRL, MAKEWORD(0x06, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlave4K30P60POSD(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_MISCTUNECTRL, MAKEWORD(0x07, nID));
}

//---------------------------------------------------------------------------------
void SPImAPP_SetSlaveIREdgeOnOff(WORD nID)
{
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_IREDGEONOFF, nID);
}



/*  FILE_END_HERE */
