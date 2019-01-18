//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	SERIAL.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<stdio.h>
#include	<string.h>
#include	<stdarg.h>
#include	"video.h"

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
#define		XRC_COMM_BGN			0xA0
#define		XRC_COMM_ADJ			0xAC
#define		XRC_COMM_END			0xAF

#define		RSC_COMM_BGN			0xB0
#define		RSC_COMM_END			0xBF

#define		uCOM_IRAM_ID			0x00

#define		TXIRQ_Enable()			uart_tx_int_enable(0, TRUE);
#define		TXIRQ_Disable()			uart_tx_int_enable(0, FALSE);
#define		TXBUF(x)				{uart_putch(0, x); TXIRQ_Enable();}

#define		XRC_BUFF_MAX			128

#if defined(__USE_USB_VOUT__)
#define	MU3_COMD_BGN					0xbe
#define	MU3_COMD_END					0xef

#define MU3_COMD_MSG					0xde
#define MU3_COMD_COMD					0xad

#define MU3_COMD_RES_SET				0xa0
#define MU3_COMD_RES_GET				0xa1
#define MU3_COMD_PU_SET					0xb0
#define MU3_COMD_PU_GET					0xb1
#define MU3_COMD_FM_SET					0xb2
#define MU3_COMD_FM_GET					0xb3

#define	MU3_COMD_PU_BRIGHTNESS			0x00
#define	MU3_COMD_PU_CONTRAST			0x01
#define	MU3_COMD_PU_HUE					0x02
#define	MU3_COMD_PU_SATURATION			0x03
#define	MU3_COMD_PU_SHARPNESS			0x04

#define MU3_COMD_FM_FW_VERSION			0x00
#define MU3_COMD_FM_RESET				0x01
#define MU3_COMD_FM_PREPARE_DOWNLOAD	0x02

#endif


// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static volatile BYTE SerialTxCount, SerialTxData[6], SerialTxLimit = 6;
static volatile BYTE SerialRxCount, SerialRxData[6], SerialRxLimit = 6;
static volatile BYTE SerialAckCode, SerialTimeOut;

static volatile BOOL fSerialCmdParsed, fSerialRxCompleted, fSerialTxCompleted;
static volatile BOOL fSerialXRCParsed, fSerialCommXRC, fSerialModeXRC, fSerialPreXRC;

static volatile BYTE UARTCmd[6], UARTexe[6], XRCMainCmd, XRCSize;
static volatile WORD XRCAddr;

static volatile BYTE XRCDebugIRAM[XRC_BUFF_MAX];


#if defined(__USE_USB_VOUT__)
volatile BYTE mu3USBRxCount=0, mu3USBRxData[16], mu3USBRxLimit = 16;
volatile BOOL mu3USBCommandSet, mu3USBRxCompleted;
volatile BYTE mu3USBCommand[16], mu3USBTimeOut;
volatile BYTE mu3USBTxData[8];
#endif

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
extern void PrepareFWUpdate(void);
// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM SerialCommandResponse(void)
{
	fSerialTxCompleted = 0;	TXBUF(SerialTxData[0]);
}

//--------------------------------------------------------------------------------------------------------------------------
static void	uRAM_MultiWrite(BYTE nID, WORD sAddr, PBYTE pBuf, WORD bytes)
{
	//memcpy((PBYTE)(0x20000000+sAddr), pBuf, bytes);
	switch(sAddr)	{
		case  0:	IrCodeSetIrComCommand(MAKEWORD(pBuf[1],pBuf[0]));	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void	WriteSerialModeXRC(void)
{
	XRCSize = MIN(XRCSize, XRC_BUFF_MAX);
	
	switch (XRCMainCmd&0xfe) {
		case uCOM_IRAM_ID:
			uRAM_MultiWrite(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize); break;
		case MISP_HOST_ID: case MISP_LOCAL_ID: case MISP_FPGA_ID: case MISP_SDRAM_ID: case MISP_SDRAM_TEST_ID:
			MISP_MultiWrite(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize); break;
		case I2C_AT24C02_ADDR:
			EEPROM_MultiWrite(XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);			break;
#if defined(__USE_IT66121_TX__)
		case IT66121_ADR:
			IT66121_MultiWrite(XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_IT6615_TX__)
		case IT6615_ADR:
			IT6615_MultiWrite(XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_IT6802_RX__)
		case IT6802A0_ADR:	case IT6802B0_ADR:
		case IT6802MHL_ADR:	case IT6802EDID_ADR:	//case IT6802CEC_ADR:
			IT6802_MultiWrite(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_LT8618EX_TX__)
		case LT8618EX_ADR:	case LT8618EX_ADR_last:
			LT8618EX_MultiWrite(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_LT8618SX_TX__)
		case LT8618SX_ADR:
			LT8618SX_MultiWrite(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_LT8619_RX__)
		case LT8619C_ADR:
			LT8619C_MultiWrite(XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_TVI_TX__)
		case TP2912_ADR:
			TVI_MultiWrite(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_AN41908__)||defined(__USE_AN41919__)
		case SPI_AN41908_ADDR:
			if((XRCAddr == 0x00)) {
		#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
				DCLENS_ForceSetIRISGain2((SHORT)(XRCDebugIRAM[1]<<8)+(SHORT)(XRCDebugIRAM[0]));
				//UARTprintf("gain : %d\n", (XRCDebugIRAM[1]<<8)+(XRCDebugIRAM[0]));
		#endif
			} else {
				if((XRCAddr == 0x24) || (XRCAddr == 0x29)) LENS_VD(HIGH);
				IRIS_VD(HIGH);	IRIS_VD(LOW);
				LENS_MultiWrite(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);
				if((XRCAddr == 0x24) || (XRCAddr == 0x29)) LENS_VD(LOW);
				break;
			}
		case SPI_AN41919_ADDR:
			IRIS_MultiWrite(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize); break;
#endif
#if defined(__USE_EN332T__)
		case ENX_DEV0_ADDR:	case ENX_DEV1_ADDR:
			ENX_MultiWrite(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_LATTICE_LCMX03LF__)
		case I2C_LCMX03L_ADDR:
			LCMX03L_MultiWrite(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_LATTICE_LIAMD6K__)
		case I2C_LIAMD6K_ADDR:
			LIAMD6K_MultiWrite(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif			
#if defined(__USE_PS8409_RETIMER__)
		case I2C_PS8409_ADDR:
			PS8409_MultiWrite(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
		
//		default:  I2C_Write(TWI_CH0, XRCMainCmd+0x00, XRCAddr, 2, (PBYTE)XRCDebugIRAM, XRCSize); break;
		default:  CMOS_MultiWrite(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize); break;
	}

	SerialTxData[0] = XRC_COMM_BGN;	SerialTxData[1] = XRC_COMM_ADJ;
	SerialTxData[2] = 0xC0;			SerialTxData[3] = XRC_COMM_ADJ;
	SerialTxData[4] = XRC_COMM_ADJ;	SerialTxData[5] = XRC_COMM_END;

	fSerialCommXRC = FALSE;		SerialRxLimit = 6;
	SerialCommandResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
static void	uRAM_MultiRead(BYTE nID, WORD sAddr, PBYTE pBuf, WORD bytes)
{
	WORD UV;

	//memcpy(pBuf, (PBYTE)(0x20000000+sAddr), bytes);

	// White Balance Polygon Read
	if ((sAddr > 0)&&(sAddr < 9))		UV = Get_WB_U(sAddr - 1);
	else if((sAddr > 8)&&(sAddr < 17))	UV = Get_WB_V(sAddr - 9);

	// 4K@60Hz Master / Slave Identifier
	else if(sAddr == 64)
#if defined(__USE_4K60_MD1__)	// Master
		UV = 0x0001;
#elif defined(__USE_4K60_MD2__)	// Slave
		UV = 0x0002;
#else							// Unknown
		UV = 0x0000;
#endif

	else	return;

	pBuf[0] = LOBYTE(UV);	pBuf[1] = HIBYTE(UV);

}

//--------------------------------------------------------------------------------------------------------------------------
static void	ReadSerialModeXRC(void)
{
	XRCSize = MIN(XRCSize, XRC_BUFF_MAX);
	
	SerialTxLimit = XRCSize;

	switch (XRCMainCmd&0xfe) {
		case uCOM_IRAM_ID:
			uRAM_MultiRead(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize); break;
		case MISP_HOST_ID: case MISP_LOCAL_ID: case MISP_FPGA_ID: case MISP_SDRAM_ID: case MISP_SDRAM_TEST_ID:
			MISP_MultiRead(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize); break;
		case I2C_AT24C02_ADDR:
			EEPROM_MultiRead(XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);				break;
#if defined(__USE_IT66121_TX__)
		case IT66121_ADR:
			IT66121_MultiRead(XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_IT6615_TX__)
		case IT6615_ADR:
			IT6615_MultiRead(XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_IT6802_RX__)
		case IT6802A0_ADR:	case IT6802B0_ADR:
		case IT6802MHL_ADR:	case IT6802EDID_ADR:	//case IT6802CEC_ADR:
			IT6802_MultiRead(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_LT8618EX_TX__)
		case LT8618EX_ADR:	case LT8618EX_ADR_last:
			LT8618EX_MultiRead(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_LT8618SX_TX__)
		case LT8618SX_ADR:
			LT8618SX_MultiRead(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_LT8619_RX__)
		case LT8619C_ADR:
			LT8619C_MultiRead(XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_TVI_TX__)
		case TP2912_ADR:
			TVI_MultiRead(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_AN41908__)||defined(__USE_AN41919__)
		case SPI_AN41908_ADDR:
			LENS_MultiRead(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize); break;
		case SPI_AN41919_ADDR:
			IRIS_MultiRead(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize); break;
#endif
#if defined(__USE_EN332T__)
		case ENX_DEV0_ADDR:	case ENX_DEV1_ADDR:
			ENX_MultiRead(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_LATTICE_LCMX03LF__)
		case I2C_LCMX03L_ADDR:
			LCMX03L_MultiRead(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
#if defined(__USE_LATTICE_LIAMD6K__)
		case I2C_LIAMD6K_ADDR:
			LIAMD6K_MultiRead(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif			
#if defined(__USE_PS8409_RETIMER__)
		case I2C_PS8409_ADDR:
			PS8409_MultiRead(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize);	break;
#endif
		
		//default:  I2C_Read(TWI_CH0, XRCMainCmd+0x00, XRCAddr, 2, (PBYTE)XRCDebugIRAM, XRCSize); break;
		default:  CMOS_MultiRead(XRCMainCmd+0x00, XRCAddr, (PBYTE)XRCDebugIRAM, XRCSize); break;
	}

	fSerialCommXRC = TRUE;	fSerialTxCompleted = 0;
	TXBUF(XRCDebugIRAM[0]);	while (!fSerialTxCompleted);
	fSerialCommXRC = FALSE;	SerialTxLimit = 6;
}

//--------------------------------------------------------------------------------------------------------------------------
static void SerialXRCPreClassify(PBYTE pBuf)
{
	fSerialPreXRC = FALSE;	SerialTxData[2] = 0xC0;
	SerialCommandResponse();

	XRCAddr = MAKEWORD(pBuf[3], pBuf[4]);
	XRCSize = pBuf[2]; while (!fSerialTxCompleted);	// wait Tx completed

	fSerialCommXRC = TRUE;
	if (XRCMainCmd&1) ReadSerialModeXRC();
	else			SerialRxLimit = pBuf[2];
}

//--------------------------------------------------------------------------------------------------------------------------
static void SerialXRCCmdClassify(PBYTE pBuf)
{
	// case of register read/write pre
	if ((pBuf[1]==0x5A)&&(pBuf[2]==0xA5)) {
		fSerialPreXRC = TRUE;
		SerialTxData[2] = 0xC0;		XRCMainCmd = pBuf[3];
	}
	// case of adjust mode enter
	else if (((pBuf[1]|pBuf[3])==0x55)&&((pBuf[2]|pBuf[4])==0xAD)) {
		fSerialModeXRC = TRUE;
		SerialTxData[2] = 0xC0;		SerialAckCode |= 0x40;
	}
	// case of adjust mode exit
	else if (((pBuf[1]|pBuf[3])==0x59)&&((pBuf[2]|pBuf[4])==0xAE)) {
		fSerialModeXRC = FALSE;
		SerialTxData[2] = 0x80;		SerialAckCode &= 0xBF;
	}
	// case of loop execution On/Off
//	else if ((pBuf[1]==0xB0)&&((pBuf[2]|pBuf[4])==0x00)) {
//		SerialTxData[2] = 0xC0;
//		if (pBuf[3]==0x05)	fSerialLoopStop = TRUE;
//		else				fSerialLoopStop = FALSE;
//	}
	else	SerialTxData[2] = SerialAckCode&0x3F;

	SerialCommandResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
void XRCDebugMsgHandler(void)
{
	if (fSerialXRCParsed==FALSE) return;
	else	   fSerialXRCParsed = FALSE;

	if (fSerialCommXRC)	WriteSerialModeXRC();									//xrc write call

	else if (UARTCmd[0]==XRC_COMM_BGN&&UARTCmd[5]==XRC_COMM_END) {
		if (fSerialPreXRC)	SerialXRCPreClassify((PBYTE)UARTCmd);				//xrc read call
		else				SerialXRCCmdClassify((PBYTE)UARTCmd);
	}
	else {	// case of wrong command
		SerialTxData[2] = 0x40;	SerialCommandResponse();
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM SerialXRCCmdParse(PBYTE pBuf)
{
	if (fSerialModeXRC||(pBuf[0]==XRC_COMM_BGN&&pBuf[5]==XRC_COMM_END)) {
		fSerialXRCParsed = TRUE;	// XRC-debug mode
		memcpy((PBYTE)UARTCmd, pBuf, 6);
	}
	else {
		SerialTxData[2] = SerialAckCode&0x7f;
		SerialCommandResponse();
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM SerialSETCmdParse(PBYTE pBuf)	
{
	fSerialCmdParsed = TRUE; memcpy((PBYTE)UARTCmd, pBuf, 6);
	SerialTxData[2] = GetPreemptiveMessage(MSG_SERIAL)|0x80;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SerialCommandParse(void)
{
	PBYTE pSrc = (PBYTE)SerialRxData;

	if (fSerialRxCompleted) {
		fSerialRxCompleted = FALSE;
		memcpy((PBYTE)SerialTxData, pSrc, 6);

		if (pSrc[0]==RSC_COMM_BGN&&pSrc[5]==RSC_COMM_END)
			 SerialSETCmdParse(pSrc);
		else SerialXRCCmdParse(pSrc);

		memset(pSrc, 0, 6); // clear data
	}
}


//--------------------------------------------------------------------------------------------------------------------------
void ISPM SerialTimeOutCounter(void)
{
	if (SerialTimeOut) SerialTimeOut--;
	if (SerialTimeOut) return;

	SerialRxCount = 0;
//	fSerialCmdReceive = OFF;
//	fSerialTxCompleted = ON;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SerialRxHandler(BYTE ch)
{
	if (fVISCA_CmdReceive || fPELCO_CmdReceive)	return;

	if (fSerialCmdReceive==0 && SerialRxCount==0 && ch!=XRC_COMM_BGN) return;
	else		fSerialCmdReceive = ON;
/*
	if (fSerialCommXRC)	RXBUF(XRCDebugIRAM[SerialRxCount]);
	else				RXBUF(SerialRxData[SerialRxCount]);
*/
	if (fSerialCommXRC)	XRCDebugIRAM[SerialRxCount] = ch;
	else				SerialRxData[SerialRxCount] = ch;

	if (++SerialRxCount>=SerialRxLimit) {
		SerialRxCount = 0;	fSerialRxCompleted = TRUE;
	}
	SerialTimeOut = 10;		// Time-out is 100ms
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SerialTxHandler(void)
{
	if (++SerialTxCount>=SerialTxLimit) {
		SerialTxCount = 0;	fSerialTxCompleted = TRUE;
		TXIRQ_Disable();	//fSerialCmdReceive = OFF;
	}
	else if (fSerialCommXRC)
		 {TXBUF(XRCDebugIRAM[SerialTxCount]);}
	else {TXBUF(SerialTxData[SerialTxCount]);}

	if (fSerialTxCompleted && fSerialModeXRC==FALSE)
		fSerialCmdReceive = OFF;

	SerialTimeOut = 10;		// Time-out is 100ms
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM SerialPreemptMsgHooking(void)
{
	if (!IsOtherMsgPreemptive(MSG_SERIAL)) return FALSE;

	SerialAckCode &= 0xe0;
	SerialAckCode |= (1<<GetPreemptiveMessage(MSG_SERIAL));
	SerialCommandResponse();
	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM IsSerialMsgHooking(void)
{
	if (SerialPreemptMsgHooking()) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM SetSerialEXECMsg(PBYTE pCmd)
{
	memcpy((PBYTE)UARTexe, pCmd, 6);
	SetMsgSerialNewCmd(MAKEWORD(pCmd[1], pCmd[3]));
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SerialMessageClassify(void)
{
	if (fSerialCmdParsed==FALSE) return;

	fSerialCmdParsed = FALSE;
	if (IsSerialMsgHooking()==TRUE) return;

	SetMsgSerialCtrlMode();	SetSerialEXECMsg((PBYTE)UARTCmd);

	SerialCommandResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
void SerialMessageExecution(void)
{
}

//--------------------------------------------------------------------------------------------------------------------------
void UARTprintf(const char* format, ...)
{
	char pSTR[256];	int i, arg_cnt;	va_list arg_ptr;
	
	if (fVISCA_CmdReceive || fSerialCmdReceive || fPELCO_CmdReceive) return;
	
#if defined(__USE_UART0_VISCA__) && (__DEBUGPRT_UART1__==0) || \
	!defined(__USE_UART0_VISCA__) && (__DEBUGPRT_UART1__==1) 	// uart-0/1 : printf & visca using same uart port
	if(VISCA_GetActStatus()==ON)				return;		// skip printf when visca i/f activated..
#endif

	va_start(arg_ptr, format);

	arg_cnt = vsprintf(pSTR, format, arg_ptr);

	for (i=0; i<arg_cnt; i++) {
		if (fVISCA_CmdReceive || fSerialCmdReceive || fPELCO_CmdReceive) break;
#if defined(__USE_4K60_INOUT__)
		if(SetAPP_Get4KUartStop()==TRUE)		{SetAPP_Set4KUartBackup(pSTR[i]);	continue;}
#endif
		uart_putch(__DEBUGPRT_UART1__, pSTR[i]);
	}

	va_end(arg_ptr);
}

//--------------------------------------------------------------------------------------------------------------------------
void tm_printf(const char* format, ...)
{
	char pSTR[256];	int i, arg_cnt;	va_list arg_ptr;
	
	if (fVISCA_CmdReceive || fSerialCmdReceive || fPELCO_CmdReceive) return;
	
#if defined(__USE_UART0_VISCA__) && (__DEBUGPRT_UART1__==0) || \
	!defined(__USE_UART0_VISCA__) && (__DEBUGPRT_UART1__==1) 	// uart-0/1 : printf & visca using same uart port
	if(VISCA_GetActStatus()==ON)				return;		// skip printf when visca i/f activated..
#endif
	
	LONG t = get_ms();
	
	int sec = t/1000;
	
	arg_cnt = sprintf(pSTR,"%d.%d: ", sec, (int)(t-sec*1000));
	

	for (i=0; i<arg_cnt; i++) {
		if (fVISCA_CmdReceive || fSerialCmdReceive || fPELCO_CmdReceive) break;
		uart_putch(__DEBUGPRT_UART1__, pSTR[i]);
	}

	va_start(arg_ptr, format);

	arg_cnt = vsprintf(pSTR, format, arg_ptr);

	for (i=0; i<arg_cnt; i++) {
		if (fVISCA_CmdReceive || fSerialCmdReceive || fPELCO_CmdReceive) break;
		uart_putch(__DEBUGPRT_UART1__, pSTR[i]);
	}

	va_end(arg_ptr);
}

#if defined(__USE_USB_VOUT__)

#if 0
void ISPM mu3USBTimeOutCounter(void)
{
	if (mu3USBTimeOut) mu3USBTimeOut--;
	if (mu3USBTimeOut) return;

	mu3USBRxCount = 0;
}

void ISPM mu3USBRxHandler(BYTE ch)
{

	mu3USBRxData[mu3USBRxCount] = ch;

	if (++mu3USBRxCount>=mu3USBRxLimit) 
	{
		mu3USBRxCount = 0;
		mu3USBRxCompleted = TRUE;
	}
	mu3USBTimeOut = 10;
}

void ISPM saveSilkyUSBCommand(void)
{
	PBYTE pSrc = (PBYTE)mu3USBRxData;

	// save command bytes to mu3Command buffer
	if (mu3USBRxCompleted) {
		mu3USBRxCompleted = FALSE;
		mu3USBCommandSet = TRUE;
		memcpy((PBYTE)mu3USBCommand, pSrc, mu3USBRxLimit);		
		memset(pSrc, 0, mu3USBRxLimit); // clear data
	}
}
#endif

void mu3USBCommandHandler(void)
{
	char ch;
	char printmsg[13];
	char *pSrc = (PCHAR)&mu3USBRxData[2];	
	
	//if(uart_getch(1, &ch))
	while(uart_getch(1, &ch))
	{
		//UARTprintf("%02x", ch);
		if((BYTE)ch == MU3_COMD_BGN)
		{
			//UARTprintf("&&");
			mu3USBRxCount = 0;
		}

		mu3USBRxData[mu3USBRxCount] = ch;

		if (++mu3USBRxCount>=mu3USBRxLimit) 
		{
			mu3USBRxCount = 0;
			mu3USBRxCompleted = TRUE;
			//UARTprintf("**");
			break;
		}		
	}
	
	if (mu3USBRxCompleted&&(mu3USBRxData[0]==MU3_COMD_BGN&&mu3USBRxData[mu3USBRxLimit-1]==MU3_COMD_END)) {

		//UARTprintf("sil0:(%x)", mu3USBCommand[1]);		
		switch (mu3USBRxData[1]){
			
			case MU3_COMD_MSG: // debug print
				memset(printmsg, 0, 13); // clear data
				memcpy((PBYTE)printmsg, pSrc, 12);		
				UARTprintf("%s", printmsg);
				break;
			case MU3_COMD_COMD:
				//UARTprintf("sil: %x, %x, %d\r\n", mu3USBRxData[2],mu3USBRxData[3],mu3USBRxData[4]);
				
				if(mu3USBRxData[2]==MU3_COMD_PU_SET){
					
					switch (mu3USBRxData[3])
					{											
						case MU3_COMD_PU_BRIGHTNESS:
							DEMO_SetBrightness(mu3USBRxData[4]);
							//UARTprintf("set brightness:%d\n", mu3USBRxData[4]);
							break;
						case MU3_COMD_PU_CONTRAST:
							DEMO_SetContrast(mu3USBRxData[4]);
							//UARTprintf("set contrast:%d\n", mu3USBRxData[4]);
							break;
						case MU3_COMD_PU_HUE:
							DEMO_SetHue(mu3USBRxData[4]);
							//UARTprintf("set hue:%d\n", mu3USBRxData[4]);
							break;
						case MU3_COMD_PU_SATURATION:
							DEMO_SetSaturation(mu3USBRxData[4]);
							//UARTprintf("set saturation:%d\n", mu3USBRxData[4]);
							break;
						case MU3_COMD_PU_SHARPNESS:
							DEMO_SetSharpness(mu3USBRxData[4]);
							//UARTprintf("set sharpness:%d\n", mu3USBRxData[4]);
							break;
							
						default:
							break;
					}					
				}
				else if(mu3USBRxData[2]==MU3_COMD_PU_GET)
				{
					memset(mu3USBTxData, 0, 8);
					mu3USBTxData[0] = MU3_COMD_BGN;
					mu3USBTxData[1] = MU3_COMD_COMD;
					mu3USBTxData[2] = MU3_COMD_PU_GET;
					mu3USBTxData[7] = MU3_COMD_END;			
										
					switch (mu3USBRxData[3])
					{	
						case MU3_COMD_PU_BRIGHTNESS:
							mu3USBTxData[3] = MU3_COMD_PU_BRIGHTNESS;
							mu3USBTxData[4] = DEMO_GetBrightness();
							uart_putdata(1, mu3USBTxData, 8);
							break;
						case MU3_COMD_PU_CONTRAST:
							mu3USBTxData[3] = MU3_COMD_PU_CONTRAST;
							mu3USBTxData[4] = DEMO_GetContrast();
							uart_putdata(1, mu3USBTxData, 8);
							break;
						case MU3_COMD_PU_HUE:
							mu3USBTxData[3] = MU3_COMD_PU_HUE;
							mu3USBTxData[4] = DEMO_GetHue();
							uart_putdata(1, mu3USBTxData, 8);
							break;
						case MU3_COMD_PU_SATURATION:
							mu3USBTxData[3] = MU3_COMD_PU_SATURATION;
							mu3USBTxData[4] = DEMO_GetSaturation();
							uart_putdata(1, mu3USBTxData, 8);
							break;
						case MU3_COMD_PU_SHARPNESS:
							mu3USBTxData[3] = MU3_COMD_PU_SHARPNESS;
							mu3USBTxData[4] = DEMO_GetSharpness();
							uart_putdata(1, mu3USBTxData, 8);
							break;
						default:
							break;
					}										
				}
				else if(mu3USBRxData[2]==MU3_COMD_FM_GET)
				{
					memset(mu3USBTxData, 0, 8);
					mu3USBTxData[0] = MU3_COMD_BGN;
					mu3USBTxData[1] = MU3_COMD_COMD;
					mu3USBTxData[2] = MU3_COMD_FM_GET;
					mu3USBTxData[7] = MU3_COMD_END;			
										
					switch (mu3USBRxData[3])
					{	
						case MU3_COMD_FM_FW_VERSION:
							mu3USBTxData[3] = MU3_COMD_FM_FW_VERSION;
							mu3USBTxData[4] = __FW_VERSION__;
							uart_putdata(1, mu3USBTxData, 8);
							break;
						default:
							break;
					}										
				}
				else if(mu3USBRxData[2]==MU3_COMD_FM_SET)
				{
					memset(mu3USBTxData, 0, 8);
					mu3USBTxData[0] = MU3_COMD_BGN;
					mu3USBTxData[1] = MU3_COMD_COMD;
					mu3USBTxData[2] = MU3_COMD_FM_SET;
					mu3USBTxData[7] = MU3_COMD_END;			
										
					switch (mu3USBRxData[3])
					{	
						case MU3_COMD_FM_RESET:
							break;
						case MU3_COMD_FM_PREPARE_DOWNLOAD:
							tm_printf("prepare_download\n");
							PrepareFWUpdate();
							break;							
							
						default:
							break;
					}										
				}
				else if(mu3USBRxData[2]==MU3_COMD_RES_SET){
					
					switch(mu3USBRxData[3]){
						case 1:
							
							switch(mu3USBRxData[4]){
								case 60:
									SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p60);
									break;
								case 50:
									SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p50);
									break;
								case 30:
									SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p30);
									break;
								case 24:
									SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p24);
									break;
								case 20:
									SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p20);
									break;
								case 15:
									SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p15);
									break;
								case 10:
									SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p10);
									break;
								case 7:
									SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p7d5);
									break;
								case 5:
									SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p05);
									break;								
								default:
									SetOUT_MainFrmt(MISP_VIDOUT_1920x1080p30);
									break;								
							}							
							break;
						case 2:
							switch(mu3USBRxData[4]){
								case 60:
									SetOUT_MainFrmt(MISP_VIDOUT_1280x720p60);
									break;
								case 50:
									SetOUT_MainFrmt(MISP_VIDOUT_1280x720p50);
									break;
								case 30:
									SetOUT_MainFrmt(MISP_VIDOUT_1280x720p30);
									break;
								case 24:
									SetOUT_MainFrmt(MISP_VIDOUT_1280x720p24);
									break;
								case 20:
									SetOUT_MainFrmt(MISP_VIDOUT_1280x720p20);
									break;
								case 15:
									SetOUT_MainFrmt(MISP_VIDOUT_1280x720p15);
									break;
								case 10:
									SetOUT_MainFrmt(MISP_VIDOUT_1280x720p10);
									break;
								case 7:
									SetOUT_MainFrmt(MISP_VIDOUT_1280x720p7d5);
									break;
								case 5:
									SetOUT_MainFrmt(MISP_VIDOUT_1280x720p05);
									break;								
								default:
									SetOUT_MainFrmt(MISP_VIDOUT_1280x720p30);
									break;								
							}							
							break;
						case 3:
							switch(mu3USBRxData[4]){
								case 60:
									SetOUT_MainFrmt(MISP_VIDOUT_720x480p60);
									break;
								case 50:
									SetOUT_MainFrmt(MISP_VIDOUT_720x480p50);
									break;
								case 30:
									SetOUT_MainFrmt(MISP_VIDOUT_720x480p30);
									break;
								case 24:
									SetOUT_MainFrmt(MISP_VIDOUT_720x480p24);
									break;
								default:
									SetOUT_MainFrmt(MISP_VIDOUT_720x480p60);
									break;								
							}							
							break;
						case 4:
							switch(mu3USBRxData[4]){
								case 15:
									SetOUT_MainFrmt(MISP_VIDOUT_3840x2160p15);
									break;
								case 10:
									SetOUT_MainFrmt(MISP_VIDOUT_3840x2160p10);
									break;
								case 7:
									SetOUT_MainFrmt(MISP_VIDOUT_3840x2160p7d5);
									break;
								case 5:
									SetOUT_MainFrmt(MISP_VIDOUT_3840x2160p05);
									break;
								default:
									SetOUT_MainFrmt(MISP_VIDOUT_3840x2160p10);
									break;								
							}							
							break;
					}
					SetFC_MainFCdstSizeH(GetOUT_MainSizeHA());
					SetFC_MainFCdstSizeV(GetOUT_MainSizeVA());
					
				}
				break;
			default:
				break;
		}
		
		mu3USBRxCompleted = FALSE;
		mu3USBRxCount = 0;
	}	
}

#endif

//--------------------------------------------------------------------------------------------------------------------------
void SetUARTBaudRate(int ch, int baud)
{
	INTERRUPT_TYPE INTNUM = (ch)? INTNUM_UART1 : INTNUM_UART0;

//	UARTprintf("\r\nNow, UART-(%d) BaudRate is %dbps!\r\n", ch, baud);
	
	uart_rx_int_enable(ch, FALSE);
	enable_interrupt(INTNUM, FALSE);

	uart_rx_flush(ch);
	uart_tx_flush(ch);
	uart_config(ch, baud, DATABITS_8, STOPBITS_1, UART_PARNONE);

	*R_IINTMOD  &= ~(1<<INTNUM);	//for the first empty interrupt	//for lucida
	set_interrupt (INTNUM, (ch)? UART1_ISR : UART0_ISR);

	enable_interrupt(INTNUM, TRUE);
	uart_rx_int_enable(ch, TRUE);
	
	set_debug_channel(__DEBUGPRT_UART1__);
}

//--------------------------------------------------------------------------------------------------------------------------
int GetUARTBaudRate(int ch)
{
	U32 divisor, udl_l, udl_h;
	U32 uart_base = (U32)R_UART_BASE(ch);
	
	*(volatile U32*)(uart_base + UART_LCR) |= F_ULC_DLAB_EN;		//divisor latch access enable, fifo control read mode.
	udl_l = *(volatile U32*)(uart_base + UART_DIV_LO) & 0xff;
	udl_h = *(volatile U32*)(uart_base + UART_DIV_HI) & 0xff;
	*(volatile U32*)(uart_base + UART_LCR) &= (~F_ULC_DLAB_EN);	//divisor latch access disable, fifo control write mode.
	
	divisor = MAKEWORD(udl_h, udl_l);
	
	return  	get_apb_clock() / (16 * divisor);					// calc baudrate with udl value
}

/*  FILE_END_HERE */
