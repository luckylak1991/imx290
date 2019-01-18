//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2016  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	SPIS_DRV.C
// Description 		: spi-slave driver
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"

#if defined(__USE_CPU_SPI_SLAVE__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
SPIs_CTRL_PARAM	stSPIsCTRL;
SPIs_APP_PARAM	stSPIsAPP;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void SPIs_Initialize(void)
{
	memset(&stSPIsCTRL, 0, sizeof(SPIs_CTRL_PARAM));
	memset(&stSPIsAPP, 0, sizeof(SPIs_APP_PARAM));

	stSPIsAPP.stTXRAW.size_3a = 7;		// ae ysum data(5 dwords), iris(1), agc gain(1)
#if defined(__USE_IRDATA_SLAVE2MASER__)
	stSPIsAPP.stTXRAW.size_3a += 1;		// IR data
#endif

#if defined(__USE_AF_SLAVE__)
	stSPIsAPP.stTXRAW.size_3a += 12;	// af filtered and counter data : 12 dwords
#endif
	//stSPIsAPP.stTXRAW.size_3a += 21;	// awb poly data(19 dwords) + gain(2)
#if defined(__USE_4K60_MD2__)
	stSPIsAPP.stTXRAW.size_nr = 3;		// nr (3 dwords)
#endif
}


// command
//--------------------------------------------------------------------------------------------------------------------------
static void SPIs_ExeCmd(DWORD cmd)
{
	BYTE buff[4];
	
	buff[0] = HIBYTE(HIWORD(cmd));		buff[1] = LOBYTE(HIWORD(cmd));
	buff[2] = HIBYTE(LOWORD(cmd));		buff[3] = LOBYTE(LOWORD(cmd));
	
	switch(buff[1])	{
		case 0x00:	SPIsAPP_CmdRegWrite(MAKEWORD(buff[2], buff[3]));	break;
		case 0x01:	SPIsAPP_CmdRegAddr(MAKEWORD(buff[2], buff[3]));	break;
		case 0x02:	SPIsAPP_CmdIRKey(MAKEWORD(buff[2], buff[3]));	break;
		case 0x03:	SPIsAPP_CmdFBcapCtrl(MAKEWORD(buff[2], buff[3]));	break;
		case 0x04:	SPIsAPP_CmdFBcapX(MAKEWORD(buff[2], buff[3]));		break;
		case 0x05:	SPIsAPP_CmdFBcapY(MAKEWORD(buff[2], buff[3]));		break;
		case 0x06:	SPIsAPP_CmdFBcapW(MAKEWORD(buff[2], buff[3]));		break;
		case 0x07:	SPIsAPP_CmdFBcapH(MAKEWORD(buff[2], buff[3]));		break;
#if defined(__PANORAMA_SETUP__) // // panorama SPI slave command
		case 0x10:	SPIsAPP_CmdPanoDisp(MAKEWORD(buff[2], buff[3]));	break;
		case 0x11:	SPIsAPP_CmdPanoSave(MAKEWORD(buff[2], buff[3]));	break;
		case 0x12:	SPIsAPP_CmdPanoDel(MAKEWORD(buff[2], buff[3]));	break;
		case 0x13:	SPIsAPP_CmdPanoBlend(MAKEWORD(buff[2], buff[3]));	break;
		case 0x14:	SPIsAPP_CmdPanoBLvL(MAKEWORD(buff[2], buff[3]));	break;
		case 0x15:	SPIsAPP_CmdPanoSelSS(MAKEWORD(buff[2], buff[3]));	break;
		case 0x16:	SPIsAPP_CmdPanoSSAdj(MAKEWORD(buff[2], buff[3]));	break;
		case 0x17:	SPIsAPP_CmdPanoCntrl(MAKEWORD(buff[2], buff[3]));	break;
		
		case 0x18:	SPIsAPP_CmdPanoSelSS_MDINAPP(MAKEWORD(buff[2], buff[3]));	break;
		case 0x19:	SPIsAPP_CmdPanoControl_MDINAPP(MAKEWORD(buff[2], buff[3]));	break;
#endif
		
		case 0x20:	SPIsAPP_CmdWBGainR(MAKEWORD(buff[2], buff[3]));		break;
		case 0x21:	SPIsAPP_CmdWBGainB(MAKEWORD(buff[2], buff[3]));		break;
		case 0x22:	SPIsAPP_CmdAEDGain(MAKEWORD(buff[2], buff[3]));		break;
		case 0x23:	SPIsAPP_CmdAEGainGainH(MAKEWORD(buff[2], buff[3]));	break;
		case 0x24:	SPIsAPP_CmdAEGainGainL(MAKEWORD(buff[2], buff[3]));	break;
		case 0x25:	SPIsAPP_CmdAEShutGain(MAKEWORD(buff[2], buff[3]));	break;
		case 0x26:	SPIsAPP_CmdAEIrisGain(MAKEWORD(buff[2], buff[3]));	break;
		case 0x27:	SPIsAPP_CmdAEGainMaxH(MAKEWORD(buff[2], buff[3]));	break;
		case 0x28:	SPIsAPP_CmdAEGainMaxL(MAKEWORD(buff[2], buff[3]));	break;
		case 0x29:	SPIsAPP_CmdAEGainMinH(MAKEWORD(buff[2], buff[3]));	break;
		case 0x2A:	SPIsAPP_CmdAEGainMinL(MAKEWORD(buff[2], buff[3]));	break;
		case 0x2B:	SPIsAPP_CmdAEGainPMaxH(MAKEWORD(buff[2], buff[3]));	break;
		case 0x2C:	SPIsAPP_CmdAEGainPMaxL(MAKEWORD(buff[2], buff[3]));	break;
		case 0x2D:	SPIsAPP_CmdAEBright(MAKEWORD(buff[2], buff[3]));	break;
		case 0x2E:	SPIsAPP_CmdAEYsumH(MAKEWORD(buff[2], buff[3]));		break;
		case 0x2F:	SPIsAPP_CmdAEYsumL(MAKEWORD(buff[2], buff[3]));		break;
		
		case 0x30:	SPIsAPP_CmdLLSharpness(MAKEWORD(buff[2], buff[3]));	break;
		case 0x31:	SPIsAPP_CmdLLSaturation(MAKEWORD(buff[2], buff[3]));break;
		case 0x32:	SPIsAPP_CmdCSPLS(MAKEWORD(buff[2], buff[3]));		break;
		case 0x33:	SPIsAPP_CmdCACGgain(MAKEWORD(buff[2], buff[3]));	break;
		case 0x34:	SPIsAPP_CmdCACMgain(MAKEWORD(buff[2], buff[3]));	break;
		
		case 0x40:	SPIsAPP_CmdDYNTCtrl(buff[2], buff[3]);				break;
		
		case 0x50:	SPIsAPP_CmdNRMode(MAKEWORD(buff[2], buff[3]));		break;
		case 0x51:	SPIsAPP_CmdNRIntensity(MAKEWORD(buff[2], buff[3]));	break;
		case 0x52:	SPIsAPP_CmdNRAperture(MAKEWORD(buff[2], buff[3]));	break;
		case 0x53:	SPIsAPP_CmdNRZoomMode(MAKEWORD(buff[2], buff[3]));	break;
		case 0x54:	SPIsAPP_CmdNRMisc(MAKEWORD(buff[2], buff[3]));		break;
		case 0x55:	SPIsAPP_CmdNRStdev(MAKEWORD(buff[2], buff[3]));		break;
		case 0x56:	SPIsAPP_CmdNRStdevBr(MAKEWORD(buff[2], buff[3]));	break;
		case 0x57:	SPIsAPP_CmdNRStdevDa(MAKEWORD(buff[2], buff[3]));	break;
		case 0x58:	SPIsAPP_CmdNRUpCnt(MAKEWORD(buff[2], buff[3]));		break;
		
		case 0x60:	SPIsAPP_CmdPMCtrl(buff[2], buff[3]);				break;
		case 0x61:	SPIsAPP_CmdPMRectSizeW(MAKEWORD(buff[2], buff[3]));	break;
		case 0x62:	SPIsAPP_CmdPMRectSizeH(MAKEWORD(buff[2], buff[3]));	break;
		case 0x63:	SPIsAPP_CmdPMRectPosiX(MAKEWORD(buff[2], buff[3]));	break;
		case 0x64:	SPIsAPP_CmdPMRectPosiY(MAKEWORD(buff[2], buff[3]));	break;
		case 0x65:	SPIsAPP_CmdPMPolyPosiX(MAKEWORD(buff[2], buff[3]));	break;
		case 0x66:	SPIsAPP_CmdPMPolyPosiY(MAKEWORD(buff[2], buff[3]));	break;
		
		case 0x70:	SPIsAPP_CmdGammaCtrl(buff[2], buff[3]);				break;
		case 0x71:	SPIsAPP_CmdEffectCtrl(buff[2], buff[3]);			break;
		
		case 0x80:	SPIsAPP_CmdDZOOMCtrl(buff[2], buff[3]);				break;
		case 0x81:	SPIsAPP_CmdDefogCtrl(buff[2], buff[3]);				break;
		case 0x82:	SPIsAPP_CmdDWDRCtrl(buff[2], buff[3]);				break;
		
		case 0x83:	SPIsAPP_CmdMDCtrl(buff[2], buff[3]);				break;
		case 0x84:	SPIsAPP_CmdMDPosi(MAKEWORD(buff[2], buff[3]));		break;
		case 0x85:	SPIsAPP_CmdMDSize(MAKEWORD(buff[2], buff[3]));		break;
		
		case 0x87:	SPIsAPP_CmdBacklightCtrl(buff[2], buff[3]);			break;
		case 0x88:	SPIsAPP_CmdBLCPosi(buff[2], buff[3]);				break;
		case 0x89:	SPIsAPP_CmdBLCSize(buff[2], buff[3]);				break;
		case 0x8A:	SPIsAPP_CmdHLCPosi(buff[2], buff[3]);				break;
		case 0x8B:	SPIsAPP_CmdHLCSize(buff[2], buff[3]);				break;
		
		case 0x8C:	SPIsAPP_CmdBinning(MAKEWORD(buff[2], buff[3]));		break;
		case 0x8D:	SPIsAPP_CmdDispFuncCtrl(buff[2], buff[3]);			break;
		
		case 0x90:	SPIsAPP_CmdCamTitleCtrl(buff[2], buff[3]);			break;
		case 0x91:	SPIsAPP_CmdNTPAL(MAKEWORD(buff[2], buff[3]));		break;
		case 0x95:	SPIsAPP_CmdFactoryReset(MAKEWORD(buff[2], buff[3]));	break;
		case 0x98:	SPIsAPP_CmdOutputFormat(MAKEWORD(buff[2], buff[3]));	break;
		
		case 0xA0:	SPIsAPP_CmdEngrCtrl(buff[2], buff[3]);				break;
		case 0xA1:	SPIsAPP_CmdCCCtrl(buff[2], buff[3]);				break;
		case 0xA2:	SPIsAPP_CmdSSTPPosi(buff[2], buff[3]);				break;
		case 0xA3:	SPIsAPP_CmdSSTPSize(buff[2], buff[3]);				break;
		case 0xA8:	SPIsAPP_CmdMISCTUNECtrl(buff[2], buff[3]);			break;
		
		case 0xF0:	SPIsAPP_CmdIREdgeOnOff(MAKEWORD(buff[2], buff[3]));	break;
		case 0xF1:	SPIsAPP_CmdSDPCCtrl(buff[2], buff[3]);				break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void SPIs_CmdProcess(PSPIs_CTRL_PARAM pCTRL)
{
	if(DIFF(pCTRL->CmdPushCnt, pCTRL->CmdPopCnt) ==0)			return;	// received new command ?
	
	SPIs_ExeCmd(pCTRL->CmdBuffer[pCTRL->CmdPopCnt]);
	
	if((pCTRL->CmdPopCnt+1) < SPIS_CMD_BUF_MAX)		pCTRL->CmdPopCnt++;		// check cmd buffer max limit
	else											pCTRL->CmdPopCnt=0;
}



// Inquiry
//-------------------------------------------------------------------------------------
static void ISPM SPIs_InqCam3aRawData(PSPIs_CTRL_PARAM pCTRL)
{
	pCTRL->ctrl &= SPIS_TX_MODE_MASK;	pCTRL->ctrl |= SPIS_TX_MODE_3A_DATA;	// set tx flag
	pCTRL->TxBuffer = MAKEDWORD(MAKEWORD(0x50, 0x00), MAKEWORD(SPIsAPP_Inq3aTxSize(), 0x00));
}

//-------------------------------------------------------------------------------------
static void ISPM SPIs_InqRegValue(PSPIs_CTRL_PARAM pCTRL)
{
	pCTRL->TxBuffer = MAKEDWORD(MAKEWORD(0x50, 0x00), SPIsAPP_InqRegRead(LOWORD(pCTRL->RxBuffer)));
}

//-------------------------------------------------------------------------------------
static void ISPM SPIs_InqFBcapStatus(PSPIs_CTRL_PARAM pCTRL)
{
	pCTRL->TxBuffer = MAKEDWORD(MAKEWORD(0x50, 0x00), SPIsAPP_InqFBcapStatus(LOWORD(pCTRL->RxBuffer)));
}

//-------------------------------------------------------------------------------------
static void ISPM SPIs_InqCamNRRawData(PSPIs_CTRL_PARAM pCTRL)
{
	pCTRL->ctrl &= SPIS_TX_MODE_MASK;	pCTRL->ctrl |= SPIS_TX_MODE_NR_DATA;	// set tx flag
	pCTRL->TxBuffer = MAKEDWORD(MAKEWORD(0x50, 0x00), MAKEWORD(SPIsAPP_InqNRTxSize(), 0x00));
}

//-------------------------------------------------------------------------------------
static void ISPM SPIs_PacketInquiry(PSPIs_CTRL_PARAM pCTRL)
{
	switch(LOBYTE(HIWORD(pCTRL->RxBuffer)))	{		// 2nd byte
		case  0x07:	SPIs_InqCam3aRawData(pCTRL);		break;
		case  0x08:	SPIs_InqCamNRRawData(pCTRL);		break;
		case  0xA0:	SPIs_InqRegValue(pCTRL);			break;
		case  0xA3:	SPIs_InqFBcapStatus(pCTRL);			break;
	}
}

//-------------------------------------------------------------------------------------
static void ISPM SPIs_PacketCommand(PSPIs_CTRL_PARAM pCTRL)
{
	if(DIFF((pCTRL->CmdPushCnt+1), pCTRL->CmdPopCnt)) 	{				// buffer not full : push counter ++
		if((pCTRL->CmdPushCnt+1 >= SPIS_CMD_BUF_MAX) && (pCTRL->CmdPopCnt == 0))	{	// push(max -> 0), pop(0)
			stSPIsCTRL.TxBuffer = SPIS_ACK_CMD_BUF_FULL;
		}
		else	{
			pCTRL->CmdBuffer[pCTRL->CmdPushCnt++] = pCTRL->RxBuffer;				// move rx data to cmd buffer
			if(pCTRL->CmdPushCnt+1 > SPIS_CMD_BUF_MAX)		pCTRL->CmdPushCnt=0;	// push(max -> 0)
			
			stSPIsCTRL.TxBuffer = SPIS_ACK_CMD_BUF_FREE;
		}
	}
	else	{
		stSPIsCTRL.TxBuffer = SPIS_ACK_CMD_BUF_FULL;
	}
}

//-------------------------------------------------------------------------------------
static void ISPM SPIs_PacketDecoder(PSPIs_CTRL_PARAM pCTRL)
{
	switch(HIBYTE(HIWORD(pCTRL->RxBuffer)))	{		// 1st byte
		case  0x01:	SPIs_PacketCommand(pCTRL);				break;
		case  0x09:	SPIs_PacketInquiry(pCTRL);				break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void SPIs_ProcessHandler(void)
{
	SPIs_CmdProcess(&stSPIsCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SPIs_RxHandler(void)
{
	stSPIsCTRL.RxBuffer = *R_SPI0DATA(SPI_CH1);									// get rx data
	stSPIsCTRL.TxBuffer = SPI_ACK_DUMMY;											// set tx buffer to default dummy data
	
	if(stSPIsCTRL.ctrl & SPIS_TX_MODE_MASK)	{								// skip packet decoder when transfer raw data to master
		switch(stSPIsCTRL.ctrl & SPIS_TX_MODE_MASK)	{
			case SPIS_TX_MODE_3A_DATA:	SPIsAPP_Inq3aRaw(&stSPIsCTRL.TxBuffer);		break;
			case SPIS_TX_MODE_FB_DATA:	SPIsAPP_InqFBcapRaw(&stSPIsCTRL.TxBuffer);	break;
			case SPIS_TX_MODE_NR_DATA:	SPIsAPP_InqNRRaw(&stSPIsCTRL.TxBuffer);		break;
		}
	}
	else	SPIs_PacketDecoder(&stSPIsCTRL);										// packet decoder for cmd & inq
	
	*R_SPI0DATA(SPI_CH1) = stSPIsCTRL.TxBuffer;									// set tx data -- it will be transferred to spi-master on next rx irq
}

#endif	//__USE_CPU_SPI_SLAVE__


/*  FILE_END_HERE */
