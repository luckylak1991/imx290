//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2016  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  PELCO.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__PELCO_H__
#define		__PELCO_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#define 	PELCO_D_PACKET_MAX				7
#define 	PELCO_P_PACKET_MAX				8
#define 	PELCO_RX_PACKET_MAX				8
#define 	PELCO_TX_PACKET_MAX				18

#define		PELCO_D_SYNC					0xFF
#define 	PELCO_P_STX 					0xA0
#define 	PELCO_P_ETX 					0xAF
#define		PELCO_D_CHKSUM(pBuf)			((pBuf[1]+pBuf[2]+pBuf[3]+pBuf[4]+pBuf[5])&0xFF)	// modulo256 sum of byte2~6
#define		PELCO_P_CHKSUM(pBuf)			(pBuf[0]^pBuf[1]^pBuf[2]^pBuf[3]^pBuf[4]^pBuf[5]^pBuf[6])	// xor sum of byte1~7


typedef	enum {
	BPS_2400 = 0,
	BPS_4800,
	BPS_9600,
	BPS_19200,
	BPS_38400,
}	BAUD_t;


typedef	struct
{
	LONG	info;
	
	BYTE	power; 								// 0:off, 1:on
	BYTE	scan; 								// 0:manual, 1:auto
	BYTE	iris; 								// 0:stop, 1:open, 2:close
	BYTE	foks; 								// 0:stop, 1:near, 2:far
	BYTE	pan; 								// 0:stop, 1:right, 2:left
	BYTE	tilt; 								// 0:stop, 1:up, 2:down
	BYTE	zoom; 								// 0:stop, 1:tele, 2:wide
	BYTE	alarm; 								// [7]none, [6:0]alarm7~1
	WORD	z_posi; 							// zoom direct position
	WORD	dummy;

}	PELCO_INFO_PARAM, *PPELCO_INFO_PARAM;


typedef	struct
{
	LONG	ctrl;								// pelco control flag
	
	BYTE 	RxBuffer[PELCO_RX_PACKET_MAX];		// pelco rx data buffer
	BYTE	RxPacket[20][PELCO_RX_PACKET_MAX];	// pelco rx packet buffer
	BYTE 	TxPacket[PELCO_TX_PACKET_MAX];		// pelco tx packet buffer
	BYTE 	CmdBuffer[PELCO_RX_PACKET_MAX];		// pelco command buffer(0/1 : socket 1/2)
	WORD	RepeatCmd; 							// pelco command buffer for repeat command(zoom/focus,,)
	BYTE 	RxCnt;								// pelco rx packet byte counter
	BYTE 	TxCnt;								// pelco tx packet byte counter
	BYTE	RxPacketPushCnt;					// pelco rx packet buffer push counter
	BYTE	RxPacketPopCnt;						// pelco rx packet buffer pop counter
	BYTE	cam_addr;							// pelco camera address, pelco-d:0x01(addr1), pelco-p:0x00~0x1F
	BYTE	uart_no; 							// pelco uart port number(0:uart_0, 1:uart_1, 2:uart_2)
	BAUD_t	baud;								// pelco uart baud rate(0:2400, 1:4800, 2:9600, 3:19200, 4:38400)
	BYTE	id_disp;							// pelco id display
	WORD	dummy;

}	PELCO_CTRL_PARAM, *PPELCO_CTRL_PARAM;


// ctrl param
#define 	PELCO_CMD_PARSED					0x00000001		// pelco command parsed(socket 1)
#define		PELCO_IF_RS485						0x00000010		// pelco use rs485 i/f(it need direction control)
#define		PELCO_IF_ACTIVATED					0x00000100		// pelco i/f activated by external equipment
#define		PELCO_TRIGGER						0x80000000		// pelco process handler tirgger
#define		PELCO_VSYNC_TRIGGER					0x40000000		// pelco process handler v-sync tirgger(set every v-sync)


// info param
#define		PELCO_FLIP_ON 						0x00000001 		// pelco flip mode enable
#define		PELCO_MENU_ON						0x00000002 		// pelco menu mode enable


// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------
extern PELCO_INFO_PARAM	stPELCOINFO;
extern PELCO_CTRL_PARAM	stPELCOCTRL;
extern volatile BOOL fPELCO_CmdReceive;

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// pelco_drv.c
void PELCO_Initialize(void);
void PELCO_ProcessHandler(void);
void PELCO_MessageExecution(void);
void ISPM PELCO_RxHandler(BYTE ch);
void ISPM PELCO_MessageClassify(void);
void ISPM PELCO_VSyncTrigger(void);
BOOL ISPM IsPELCO_CommandZOOM(void);
void PELCO_SetRepeatCmd(WORD cmd);
BOOL PELCO_GetActStatus(void);
void PELCO_SetCamID(BYTE nID);
void PELCO_SetIDDisp(BOOL OnOff);
void PELCO_SetBaudRate(BYTE nID);

// pelco_app.c
void PELCOAPP_AutoScan(BOOL OnOff);
void PELCOAPP_CamOnOff(BOOL OnOff);
void PELCOAPP_IrisStop(void);
void PELCOAPP_IrisClose(void);
void PELCOAPP_IrisOpen(void);
void PELCOAPP_FocusStop(void);
void PELCOAPP_FocusNear(void);
void PELCOAPP_FocusFar(void);
void PELCOAPP_PanStop(void);
void PELCOAPP_PanLeft(BYTE speed);
void PELCOAPP_PanRight(BYTE speed);
void PELCOAPP_TiltStop(void);
void PELCOAPP_TiltDown(BYTE speed);
void PELCOAPP_TiltUp(BYTE speed);
void PELCOAPP_ZoomStop(void);
void PELCOAPP_ZoomWide(void);
void PELCOAPP_ZoomTele(void);
void PELCOAPP_ZoomSpeed(BYTE val);
void PELCOAPP_FocusSpeed(BYTE val);
void PELCOAPP_AutoFocusOnOff(BYTE val);
void PELCOAPP_SetPanPosition(WORD val);
void PELCOAPP_SetTiltPosition(WORD val);
void PELCOAPP_SetZoomPosition(WORD val);
void PELCOAPP_SetFlipOnOff(void);
void PELCOAPP_EnterMenuMode(void);

BYTE PELCOAPP_GetAlarm(void);
WORD PELCOAPP_GetPanPosition(void);
WORD PELCOAPP_GetTiltPosition(void);
WORD PELCOAPP_GetZoomPosition(void);
WORD PELCOAPP_GetMagVal(void);

void PELCOAPP_ExeRepeatCmd(WORD cmd);


#endif	/* __PELCO_H__ */
