//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2016  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	PELCO_DRV.C
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
PELCO_CTRL_PARAM	stPELCOCTRL;
PELCO_INFO_PARAM	stPELCOINFO;

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static volatile BOOL fPELCO_CmdParsed;
volatile BOOL fPELCO_CmdReceive;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

void PELCO_Initialize(void)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[6];
#endif
	
	memset(&stPELCOCTRL, 0, sizeof(PELCO_CTRL_PARAM));
	memset(&stPELCOINFO, 0, sizeof(PELCO_INFO_PARAM));
	
	stPELCOCTRL.cam_addr = 0x01;					// pelco camera address(1~255)
	stPELCOCTRL.id_disp = OFF; 						// pelco id display
	stPELCOCTRL.baud = BPS_9600; 					// pelco uart baud rate
	stPELCOCTRL.uart_no = 1; 						// pelco uart port number(0:uart_0, 1:uart_1, 2:uart_2)
	stPELCOCTRL.ctrl = PELCO_IF_RS485; 				// pelco use rs485 i/f(it need direction control)
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	EEPROM_MultiRead(EEPROM_RS485_ID, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0]==EEPROM_SAVE_DONE)		stPELCOCTRL.cam_addr = buff[1];
	if(buff[2]==EEPROM_SAVE_DONE)		stPELCOCTRL.id_disp = buff[3];
	if(buff[4]==EEPROM_SAVE_DONE)		stPELCOCTRL.baud = (BAUD_t)buff[5];
#endif
	
	PELCO_SetCamID(stPELCOCTRL.cam_addr);
	PELCO_SetIDDisp(stPELCOCTRL.id_disp);
#if !defined(__USE_USB_VOUT__)
	PELCO_SetBaudRate(stPELCOCTRL.baud);
#endif
	
	SetMenuStatus(RS485_ID_MENU, stPELCOCTRL.cam_addr);
	SetMenuStatus(RS485_DISP_MENU, stPELCOCTRL.id_disp);
	SetMenuStatus(RS485_BAUD_MENU, stPELCOCTRL.baud);
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_TxResponse(PBYTE pMsg, BYTE bytes)
{
	BYTE i;
	
	if(stPELCOCTRL.ctrl & PELCO_IF_RS485)		DE_485(HIGH); 						// 485 Tx enable(0:rx, 1:tx)
	
	for(i=0;i<bytes;i++)						uart_putch(stPELCOCTRL.uart_no, pMsg[i]); // Tx response
	
	if(stPELCOCTRL.ctrl & PELCO_IF_RS485)		DE_485(LOW); 						// 485 Rx enable(0:rx, 1:tx)
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE PELCO_CalcChksum(PBYTE pMsg, BYTE bytes)
{
	BYTE i, sum=0;
	
	if(pMsg[0] == PELCO_D_SYNC) 	{for(i=1;i<(bytes-1);i++)		sum += pMsg[i];} 	// pelco-d
	else 							{for(i=0;i<(bytes-1);i++)		sum ^= pMsg[i];} 	// pelco-p
	
	return 	sum;
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_GeneralResponse(void)
{
	PBYTE pMsg = stPELCOCTRL.TxPacket;

	memset(&pMsg[0], 0, PELCO_TX_PACKET_MAX);
	
	pMsg[0] = stPELCOCTRL.CmdBuffer[0]; 		// sync,start code
	pMsg[1] = stPELCOCTRL.cam_addr; 			// address
	pMsg[2] = PELCOAPP_GetAlarm();
	pMsg[3] = PELCO_CalcChksum(&pMsg[0], 4);
	
	PELCO_TxResponse(&pMsg[0], 4);
}
/*
//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_QueryResponse(void)
{
	PBYTE pMsg = stPELCOCTRL.TxPacket;

	memset(&pMsg[0], 0, PELCO_TX_PACKET_MAX);
	
	pMsg[0] = stPELCOCTRL.CmdBuffer[0]; 		// sync,start code
	pMsg[1] = stPELCOCTRL.cam_addr; 			// address
	pMsg[2] = 'D'; 			pMsg[3] = 'D'; 		// reply example for part number query
	pMsg[4] = '5'; 			pMsg[5] = '3';
	pMsg[6] = 'C'; 			pMsg[7] = 'B';
	pMsg[8] = 'W';
	pMsg[9] = 0x00; 		pMsg[10] = 0x00;
	pMsg[11] = 0x00; 		pMsg[12] = 0x00;
	pMsg[13] = 0x00; 		pMsg[14] = 0x00;
	pMsg[15] = 0x00; 		pMsg[16] = 0x00;
	pMsg[17] = PELCO_CalcChksum(&pMsg[0], 18);
	
	PELCO_TxResponse(&pMsg[0], 18);
}
*/
//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExtendedResponse(BYTE resp1, BYTE resp2, BYTE data1, BYTE data2)
{
	PBYTE pMsg = stPELCOCTRL.TxPacket;

	memset(&pMsg[0], 0, PELCO_TX_PACKET_MAX);
	
	pMsg[0] = stPELCOCTRL.CmdBuffer[0]; 		// sync,start code
	pMsg[1] = stPELCOCTRL.cam_addr; 			// address
	pMsg[2] = resp1;
	pMsg[3] = resp2; 							// usually a secondary op-code
	pMsg[4] = data1;
	pMsg[5] = data2;
	pMsg[6] = PELCO_CalcChksum(&pMsg[0], 7); 	// chksum
	
	PELCO_TxResponse(&pMsg[0], 7);
}
/*
//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeSetPreset(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeClearPreset(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}
*/
//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeGotoPreset(PBYTE pBuf)
{
	switch(pBuf[5])	{
		case  0x21:	PELCOAPP_SetFlipOnOff();	break;
		case  0x5F:	PELCOAPP_EnterMenuMode();	break;
		// add function here
	}
	
	PELCO_GeneralResponse();
}
/*
//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeSetAux(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeClearAux(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeRemoteReset(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeSetZoneStart(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeSetZoneEnd(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeWriteCharToScreen(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeClearScreen(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeAlarmAck(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeZoneScanOn(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeZoneScanOff(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeSetPatternStart(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeSetPatternStop(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeRunPattern(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}
*/
//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeSetZoomSpeed(PBYTE pBuf)
{
	PELCOAPP_ZoomSpeed(pBuf[5]); 						// 0:slow, 1:low, 2:high, 3:highest
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeSetFocusSpeed(PBYTE pBuf)
{
	PELCOAPP_FocusSpeed(pBuf[5]); 						// 0:slow, 1:low, 2:high, 3:highest
	PELCO_GeneralResponse();
}
/*
//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeResetCamDefault(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}
*/
//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeAutoFocusOnOff(PBYTE pBuf)
{
	PELCOAPP_AutoFocusOnOff(pBuf[5]); 					// 0:auto, 1:manual
	PELCO_GeneralResponse();
}
/*
//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeAutoIrisOnOff(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeAGCOnOff(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeBLCOnOff(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeAWBOnOff(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeEnDevPhaseDelayMode(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeSetShutSpeed(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeAdjLineLockPhaseDelay(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeAdjWhiteBalanceRB(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeAdjWhiteBalanceMG(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeAdjGain(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeAdjAutoIrisLevel(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeAdjAutoIrisPeakValue(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeQuery(PBYTE pBuf)
{
	// add function here
	PELCO_QueryResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeSetZeroPosition(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}
*/
//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeSetPanPosition(PBYTE pBuf)
{
	PELCOAPP_SetPanPosition(MAKEWORD(pBuf[4], pBuf[5])); 	// 0 ~ 35999(degree * 100)
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeSetTiltPosition(PBYTE pBuf)
{
	PELCOAPP_SetTiltPosition(MAKEWORD(pBuf[4], pBuf[5])); 	// 0 ~ 35999(degree * 100)
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeSetZoomPosition(PBYTE pBuf)
{
	PELCOAPP_SetZoomPosition(MAKEWORD(pBuf[4], pBuf[5])); 	// 0 ~ 65535(posi / limit * 65535)
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeQueryPanPosition(PBYTE pBuf)
{
	WORD pan_posi = PELCOAPP_GetPanPosition();
	
	PELCO_ExtendedResponse(0x00, 0x59, HIBYTE(pan_posi), LOBYTE(pan_posi));
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeQueryTiltPosition(PBYTE pBuf)
{
	WORD tilt_posi = PELCOAPP_GetTiltPosition();
	
	PELCO_ExtendedResponse(0x00, 0x5B, HIBYTE(tilt_posi), LOBYTE(tilt_posi));
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeQueryZoomPosition(PBYTE pBuf)
{
	WORD zoom_posi = PELCOAPP_GetZoomPosition();
	
	PELCO_ExtendedResponse(0x00, 0x5D, HIBYTE(zoom_posi), LOBYTE(zoom_posi));
}
/*
//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeSetMagnification(PBYTE pBuf)
{
	// add function here
	PELCO_GeneralResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeQueryMagnification(PBYTE pBuf)
{
	WORD mag_val = PELCOAPP_GetMagVal();
	
	PELCO_ExtendedResponse(0x00, 0x63, HIBYTE(mag_val), LOBYTE(mag_val));
}
*/
//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeExtendedCmd(PBYTE pBuf)
{
	switch(pBuf[3])	{
		// case 0x03:		PELCO_ExeSetPreset(pBuf); 					break;
		// case 0x05:		PELCO_ExeClearPreset(pBuf); 				break;
		case 0x07:		PELCO_ExeGotoPreset(pBuf); 					break;
		// case 0x09:		PELCO_ExeSetAux(pBuf); 						break;
		// case 0x0B:		PELCO_ExeClearAux(pBuf); 					break;
		// case 0x0F:		PELCO_ExeRemoteReset(pBuf); 				break;
		// case 0x11:		PELCO_ExeSetZoneStart(pBuf); 				break;
		// case 0x13:		PELCO_ExeSetZoneEnd(pBuf); 					break;
		// case 0x15:		PELCO_ExeWriteCharToScreen(pBuf); 			break;
		// case 0x17:		PELCO_ExeClearScreen(pBuf); 				break;
		// case 0x19:		PELCO_ExeAlarmAck(pBuf); 					break;
		// case 0x1B:		PELCO_ExeZoneScanOn(pBuf); 					break;
		// case 0x1D:		PELCO_ExeZoneScanOff(pBuf); 				break;
		// case 0x1F:		PELCO_ExeSetPatternStart(pBuf); 			break;
		// case 0x21:		PELCO_ExeSetPatternStop(pBuf); 				break;
		// case 0x23:		PELCO_ExeRunPattern(pBuf); 					break;
		case 0x25:		PELCO_ExeSetZoomSpeed(pBuf); 				break;
		case 0x27:		PELCO_ExeSetFocusSpeed(pBuf); 				break;
		// case 0x29:		PELCO_ExeResetCamDefault(pBuf); 			break;
		case 0x2B:		PELCO_ExeAutoFocusOnOff(pBuf); 				break;
		// case 0x2D:		PELCO_ExeAutoIrisOnOff(pBuf); 				break;
		// case 0x2F:		PELCO_ExeAGCOnOff(pBuf); 					break;
		// case 0x31:		PELCO_ExeBLCOnOff(pBuf); 					break;
		// case 0x33:		PELCO_ExeAWBOnOff(pBuf); 					break;
		// case 0x35:		PELCO_ExeEnDevPhaseDelayMode(pBuf); 		break;
		// case 0x37:		PELCO_ExeSetShutSpeed(pBuf); 				break;
		// case 0x39:		PELCO_ExeAdjLineLockPhaseDelay(pBuf); 		break;
		// case 0x3B:		PELCO_ExeAdjWhiteBalanceRB(pBuf); 			break;
		// case 0x3D:		PELCO_ExeAdjWhiteBalanceMG(pBuf); 			break;
		// case 0x3F:		PELCO_ExeAdjGain(pBuf); 					break;
		// case 0x41:		PELCO_ExeAdjAutoIrisLevel(pBuf); 			break;
		// case 0x43:		PELCO_ExeAdjAutoIrisPeakValue(pBuf); 		break;
		// case 0x45:		PELCO_ExeQuery(pBuf); 						break;
		// case 0x49:		PELCO_ExeSetZeroPosition(pBuf); 			break;
		case 0x4B:		PELCO_ExeSetPanPosition(pBuf); 				break;
		case 0x4D:		PELCO_ExeSetTiltPosition(pBuf); 			break;
		case 0x4F:		PELCO_ExeSetZoomPosition(pBuf); 			break;
		case 0x51:		PELCO_ExeQueryPanPosition(pBuf); 			break;
		case 0x53:		PELCO_ExeQueryTiltPosition(pBuf); 			break;
		case 0x55:		PELCO_ExeQueryZoomPosition(pBuf); 			break;
		// case 0x5F:		PELCO_ExeSetMagnification(pBuf); 			break;
		// case 0x61:		PELCO_ExeQueryMagnification(pBuf); 			break;
	}
	
	// if(pBuf[0] == PELCO_D_SYNC) 	UARTprintf("D_ext[%02x, %02x, %02x, %02x, %02x, %02x, %02x] \r\n", pBuf[0], pBuf[1], pBuf[2], pBuf[3], pBuf[4], pBuf[5], pBuf[6]);
	// else	UARTprintf("P_ext[%02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x] \r\n", pBuf[0], pBuf[1], pBuf[2], pBuf[3], pBuf[4], pBuf[5], pBuf[6], pBuf[7]);
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ConvStdCmdPtoD(PBYTE pBuf) 		// pelco-p std -->  pelco-d std command
{
	BYTE data1, data2;
	
	if(pBuf[0] != PELCO_P_STX) 		return; 	// check pelco-p stx
	
	data1 = pBuf[2];	data2 = pBuf[3];
	pBuf[2] = 0x00;		pBuf[3] = 0x00; 			// clear cmd buffer
	
	if(data1&0x40) 		pBuf[2] |= 0x80; 			// [6]cam on --> [7]sens
	if(data1&0x20) 		pBuf[2] |= 0x10; 			// [5]autoscan on --> [4]auto/man scan
	if(data1&0x10) 		pBuf[2] |= 0x08; 			// [4]cam on/off --> [3]cam on/off
	if(data1&0x08) 		pBuf[2] |= 0x04; 			// [3]iris close --> [2]iris close
	if(data1&0x04) 		pBuf[2] |= 0x02; 			// [2]iris open --> [1]iris open
	if(data1&0x02) 		pBuf[2] |= 0x01; 			// [1]focus near --> [0]focus near
	if(data1&0x01) 		pBuf[3] |= 0x80; 			// [0]focus far --> [7]focus far
	if(data2&0x7E) 		pBuf[3] |= (data2&0x7E); 	// [6:1]ptz --> [6:1]ptz
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeAutoManScan(PBYTE pBuf)
{
	if((pBuf[2] & 0x10) == 0) 					return; 	// cmd1[4] : auto/manual scan config
	
	switch(pBuf[2] & 0x80)	{ 								// cmd1[7] : sense
		case 0x00:		PELCOAPP_AutoScan(OFF);	break;
		case 0x80:		PELCOAPP_AutoScan(ON);	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeCamOnOff(PBYTE pBuf)
{
	if((pBuf[2] & 0x08) == 0) 					return; 	// cmd1[3] : camera on/off config
	
	switch(pBuf[2] & 0x80)	{ 								// cmd1[7] : sense
		case 0x00:		PELCOAPP_CamOnOff(OFF);	break;
		case 0x80:		PELCOAPP_CamOnOff(ON);	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeIris(PBYTE pBuf)
{
	switch(pBuf[2] & 0x06)	{ 								// cmd1[1] : iris open,  cmd1[2] : iris close
		case 0x00:		PELCOAPP_IrisStop();	break;
		case 0x02:		PELCOAPP_IrisOpen();	break;
		case 0x04:		PELCOAPP_IrisClose();	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeFocus(PBYTE pBuf)
{
	switch((pBuf[2] & 0x01) | (pBuf[3] & 0x80))	{ 		// cmd1[0] : focus near,  cmd2[7] : focus far
		case 0x00:		PELCOAPP_FocusStop();	break;
		case 0x01:		PELCOAPP_FocusNear();	break;
		case 0x80:		PELCOAPP_FocusFar();	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExePan(PBYTE pBuf)
{
	switch(pBuf[3] & 0x06)	{ 									// cmd2[1] : pan right,  cmd2[2] : pan left
		case 0x00:		PELCOAPP_PanStop();			break;
		case 0x02:		PELCOAPP_PanRight(pBuf[4]);	break;
		case 0x04:		PELCOAPP_PanLeft(pBuf[4]);	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeTilt(PBYTE pBuf)
{
	switch(pBuf[3] & 0x18)	{ 									// cmd2[3] : tilt up,  cmd2[4] : tilt down
		case 0x00:		PELCOAPP_TiltStop();		break;
		case 0x08:		PELCOAPP_TiltUp(pBuf[5]);	break;
		case 0x10:		PELCOAPP_TiltDown(pBuf[5]);	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeZoom(PBYTE pBuf)
{
	switch(pBuf[3] & 0x60)	{ 									// cmd2[5] : zoom tele,  cmd2[6] : zoom wide
		case 0x00:		PELCOAPP_ZoomStop();	break;
		case 0x20:		PELCOAPP_ZoomTele();	break;
		case 0x40:		PELCOAPP_ZoomWide();	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_ExeStandardCmd(PBYTE pBuf)
{
	PELCO_ConvStdCmdPtoD(pBuf); 					// convert pelco-p std -> pelco-d std
	
	PELCO_ExeAutoManScan(pBuf);
	PELCO_ExeCamOnOff(pBuf);
	
	PELCO_ExeIris(pBuf);
	PELCO_ExeFocus(pBuf);
	
	PELCO_ExePan(pBuf);
	PELCO_ExeTilt(pBuf);
	PELCO_ExeZoom(pBuf);

	// if(pBuf[0] == PELCO_D_SYNC)	UARTprintf("D_std[%02x, %02x, %02x, %02x, %02x, %02x, %02x] \r\n", pBuf[0], pBuf[1], pBuf[2], pBuf[3], pBuf[4], pBuf[5], pBuf[6]);
	// else 	UARTprintf("P_std[%02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x] \r\n", pBuf[0], pBuf[1], pBuf[2], pBuf[3], pBuf[4], pBuf[5], pBuf[6], pBuf[7]);
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_CmdExcute(PPELCO_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl & PELCO_CMD_PARSED) == 0)			return;	// check cmd received ?
	pCTRL->ctrl &= (~PELCO_CMD_PARSED);
	
	switch(pCTRL->CmdBuffer[3]&0x01)	{ 							// check standard/extended command
		case 0x00:		PELCO_ExeStandardCmd(&pCTRL->CmdBuffer[0]);	break;
		case 0x01:		PELCO_ExeExtendedCmd(&pCTRL->CmdBuffer[0]);	break;
	}
	
#if defined(__USE_EN332T__)
	EN332x_ExeUccRx(&pCTRL->CmdBuffer[0]);								// for ucc rx
#endif

	memset(&pCTRL->CmdBuffer[0], 0, PELCO_RX_PACKET_MAX);			// clear cmd buffer
}

//--------------------------------------------------------------------------------------------------------------------------
static void PELCO_CmdParse(PPELCO_CTRL_PARAM pCTRL)
{
	if(DIFF(pCTRL->RxPacketPushCnt, pCTRL->RxPacketPopCnt) ==0)		return;	// received new rx packet ?
	
	memcpy(&pCTRL->CmdBuffer[0], &pCTRL->RxPacket[pCTRL->RxPacketPopCnt][0], PELCO_RX_PACKET_MAX); 		// copy packet to cmd buffer
	pCTRL->ctrl |= PELCO_CMD_PARSED;
	
	if(pCTRL->RxPacketPopCnt < 19)		pCTRL->RxPacketPopCnt++;				// 0~19
	else								pCTRL->RxPacketPopCnt=0;				// 19 -> 0
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCO_ProcessHandler(void)
{
	PELCO_CmdParse(&stPELCOCTRL);				// parse command
	PELCO_CmdExcute(&stPELCOCTRL);				// execute command

	if((stPELCOCTRL.ctrl & PELCO_VSYNC_TRIGGER)==0)			return;
	stPELCOCTRL.ctrl &= (~PELCO_VSYNC_TRIGGER);
	
//	PELCO_DisplayProcess(&stPELCOINFO);			// data screen display
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM PELCO_VSyncTrigger(void)
{
	stPELCOCTRL.ctrl |= PELCO_VSYNC_TRIGGER;	// for pelco osd
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM PELCO_PreemptMsgHooking(void)
{
	if (!IsOtherMsgPreemptive(MSG_PELCOM)) return FALSE;

	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM IsPELCO_MsgHooking(void)
{
	if (PELCO_PreemptMsgHooking()) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM PELCO_MessageClassify(void)
{
	if (fPELCO_CmdParsed==FALSE) return;

	fPELCO_CmdParsed = FALSE;
	if (IsPELCO_MsgHooking()==TRUE) return;

	SetMsgPELCO_CtrlMode();
	SetMsgPELCO_NewCmd(stPELCOCTRL.RepeatCmd);
//	PELCO_CommandResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL ISPM IsPELCO_CommandZOOM(void)
{
	switch (stPELCOCTRL.RepeatCmd) {
		case 0x1000:	case 0x2000:	// ZOOM Standard
		case 0x5000:					// ZOOM direct
		case 0x3000:	case 0x4000:	// FOKS Standard
						return TRUE;
		default:		return FALSE;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCO_MessageExecution(void)
{
	PELCOAPP_ExeRepeatCmd(stPELCOCTRL.RepeatCmd);
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCO_SetRepeatCmd(WORD cmd)
{
	stPELCOCTRL.RepeatCmd = cmd;
	fPELCO_CmdParsed = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL PELCO_GetActStatus(void)
{
	return		(stPELCOCTRL.ctrl & PELCO_IF_ACTIVATED)?  ON:  OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCO_SetCamID(BYTE nID)
{
	stPELCOCTRL.cam_addr = nID;
	
	PELCO_SetIDDisp(stPELCOCTRL.id_disp);
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCO_SetIDDisp(BOOL OnOff)
{
	char str[10];
	
	memset(&str[0], 0x20, sizeof(str));
	
	str[0] = ' ';
	str[1] = 'I';
	str[2] = 'D';
	str[3] = ':';
	str[4] = (stPELCOCTRL.cam_addr / 100 % 10) + '0';
	str[5] = (stPELCOCTRL.cam_addr / 10 % 10) + '0';
	str[6] = (stPELCOCTRL.cam_addr % 10) + '0';
	str[9] = 0x00;
	
	OSD_IrkeyTitleDisplay(OnOff, &str[0]);
	stPELCOCTRL.id_disp = OnOff;
}

//--------------------------------------------------------------------------------------------------------------------------
void PELCO_SetBaudRate(BYTE nID)
{
	int baud;

	switch(nID)	{
		case  0:  baud = 2400;	break;
		case  1:  baud = 4800;	break;
		case  2:  baud = 9600;	break;
		case  3:  baud = 19200;	break;
		case  4:  baud = 38400;	break;
		default:  baud = 9600;	break;
	}
	
	SetUARTBaudRate(stPELCOCTRL.uart_no, baud);
	stPELCOCTRL.baud = (BAUD_t)nID;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM PELCO_RxHandler(BYTE ch)
{
	PBYTE  pBuf = (PBYTE)stPELCOCTRL.RxBuffer;
	
	// check other protocol communication
	if (fSerialCmdReceive || fVISCA_CmdReceive)	return;
	
	// check start code
	if ((stPELCOCTRL.RxCnt==0) && (ch!=PELCO_D_SYNC && ch!=PELCO_P_STX)) 	return;
	else		fPELCO_CmdReceive = ON;

	pBuf[stPELCOCTRL.RxCnt] = ch;						stPELCOCTRL.RxCnt++;
	
	// check packet length
	if(pBuf[0]==PELCO_D_SYNC && stPELCOCTRL.RxCnt < PELCO_D_PACKET_MAX) 	return;
	if(pBuf[0]==PELCO_P_STX && stPELCOCTRL.RxCnt < PELCO_P_PACKET_MAX) 	return;
	
	// move rx buffer --> packet buffer
	if(pBuf[1]==stPELCOCTRL.cam_addr) 	{ 												// check 'cam addr'
		if( (pBuf[0]==PELCO_D_SYNC && pBuf[6]==PELCO_D_CHKSUM(pBuf)) || \
			(pBuf[0]==PELCO_P_STX && pBuf[7]==PELCO_P_CHKSUM(pBuf)) ) 	{ 				// verify start&chksum code
			
			memcpy(stPELCOCTRL.RxPacket[stPELCOCTRL.RxPacketPushCnt], pBuf, PELCO_RX_PACKET_MAX);	// move rx buffer to packet buffer
			
			if(DIFF(stPELCOCTRL.RxPacketPushCnt+1, stPELCOCTRL.RxPacketPopCnt))	{			// buffer not full ?
				if(stPELCOCTRL.RxPacketPushCnt < 19)		stPELCOCTRL.RxPacketPushCnt++;	// push counter : 0~19 -> 0~19,,
				else	{
					if(stPELCOCTRL.RxPacketPopCnt != 0)		stPELCOCTRL.RxPacketPushCnt=0;	// buffer full ? stop push counter.
				}
			}
		}
	}
	
	fPELCO_CmdReceive = OFF;
	memset(pBuf, 0, stPELCOCTRL.RxCnt);					stPELCOCTRL.RxCnt=0;			// clear rx buffer & counter
}


/*  FILE_END_HERE */
