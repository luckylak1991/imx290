//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  COMMON.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__COMMON_H__
#define		__COMMON_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__MISPTYPE_H__
#include	"misptype.h"
#endif
#ifndef		__BOARD_H__
#include	"board.h"
#endif
#ifndef		__UCOMI2C_H__
#include	"i2c.h"
#endif
#ifndef		__UCOMSPI_H__
#include	"spi.h"
#endif
#ifndef		__EEPROM_H__
#include	"eeprom.h"
#endif
#ifndef		__MISP100_H__
#include	"misp100.h"
#endif

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
// msg.c
typedef enum {
	MSG_BUTTON = 0, MSG_IRCODE, MSG_SERIAL, MSG_VISCOM, MSG_PELCOM, MSG_USBCOM
} 	MSG_CTRL_MODE;

typedef enum {
	MSG_LEVEL = 0, MSG_SHIFT, MSG_DZ1X, MSG_DZ2X
} 	MSG_EXE_SPEED;

#define	Delay_uSec(x)	delayus(x)

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------
extern VDWORD usValue, msValue;
extern VWORD  usDelay, msDelay;
extern VBYTE  fSysSPILineBUSY, fIsrDRVLensSTOP;
extern VBYTE  fSysIRQMsgODMVD, fSysStateSTABLE, fSysIRQCntODMVD;
extern volatile BOOL fZOOMMove, fDWindowMove;
extern volatile BOOL fMainCrop;
#if defined(__USE_SENS_PANNING_MODE__)
extern volatile BOOL fSensorPanningMove;
#endif
// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
// init.c
void uComOnChipInitial(void);
void ISPM ResetWDTcounter(void);

// delay.c
void Delay_mSec(WORD delay);

// led.c
void VOUTDisplayHandler(void);

// ir.c
void IrCodeDebugHandler(void);
void ISPM IrReceivePulseDecode(void);
void ClearIrCodeParsed(void);
void ISPM IrCodeCommandParse(void);
void ISPM IrCodeTimeOutCounter(void);
void ISPM IrCodeMessageClassify(void);
void IrCodeMessageExecution(void);
void SetIRCodeNewCmdAF(void);
BOOL ISPM IsIrCodeCommandZOOM(void);
void Call_IrkeyDispOff(void);
void IrkeyEnableOnOff(BOOL OnOff);
void ISPM IrkeyEnableToggle(void);
void ISPM IrSwitchIrqCounter(void);
void IrCodeSetIrComCommand(WORD code);

// key.c
void ButtonDebugHandler(void);
void ClearButtonParsed(void);
void ISPM ButtonCommandParse(void);
void ISPM ButtonMessageClassify(void);
void ButtonMessageExecution(void);

// serial.c
void XRCDebugMsgHandler(void);
void ISPM SerialRxHandler(BYTE ch);
void ISPM SerialTxHandler(void);
void ISPM SerialCommandParse(void);
void ISPM SerialTimeOutCounter(void);
void ISPM SerialMessageClassify(void);
void SerialMessageExecution(void);
void UARTprintf(const char* format, ...);
void SetUARTBaudRate(int ch, int baud);
int GetUARTBaudRate(int ch);
void tm_printf(const char* format, ...);
#if defined(__USE_USB_VOUT__)
void ISPM silkyUSBRxHandler(BYTE ch);
void ISPM saveSilkyUSBCommand(void);
void silkyUSBCommandHandler(void);
void ISPM silkyUSBTimeOutCounter(void);
#endif

// msg.c
void ISPM MessageCtrlClassify(void);
void MessageCtrlHandler(void);
void ISPM SetMsgButtonCtrlMode(void);
void ISPM SetMsgButtonNewCmd(BYTE val);
void ISPM SetMsgIrCodeCtrlMode(void);
void ISPM SetMsgIrCodeNewCmd(WORD val);
void ISPM SetMsgSerialCtrlMode(void);
void ISPM SetMsgSerialNewCmd(WORD val);
void ISPM SetMsgVISCA_CtrlMode(void);
void ISPM SetMsgVISCA_NewCmd(WORD val);
void ISPM SetMsgPELCO_CtrlMode(void);
void ISPM SetMsgPELCO_NewCmd(WORD val);
void ISPM SetMsgUSB_CtrlMode(void);
void ISPM SetMsgUSB_NewCmd(WORD val);
void RestoreMsgCtrlMode(void);
BOOL ISPM IsOtherMsgPreemptive(BYTE msg);
BOOL IsMsgCommand_IR_KEY(void);
BOOL IsMsgCommand_UART(void);
BOOL IsMsgPreExecCommand(void);
void SetMsgCommandComplete(BOOL OnOff);
void ISPM SetMsgCommandLongExec(BOOL OnOff);
void SetMsgCommandOnceExec(BOOL OnOff);
void SetMsgCommandRepeatKey(BYTE val);
void SetMsgCommandRepeatKeyOff(BYTE val);
void ISPM SetMsgCommandDriveLENS(BOOL OnOff);
void ISPM SetMsgCommandDriveZOOM(BOOL OnOff);
void ISPM SetMsgCommandDriveFOKS(BOOL OnOff);
void SetMsgCommandTriggerAF(BOOL OnOff);
BOOL IsMsgCommandTriggerAF(void);
void SetMsgCommandGuardZOOM(BOOL OnOff);
BOOL IsMsgCommandGuardZOOM(void);
BOOL IsMsgGuardZOOMComplete(void);
BOOL IsMsgExecutionComplete(void);
BYTE ISPM GetPreemptiveMessage(BYTE msg);
void SetMsgClrFlags(BOOL OnOff);


// isr.c
void ISPM GPIO0_ISR(void);
void ISPM GPIO2_ISR(void);
void ISPM GPIO3_ISR(void);
void ISPM EIRQ1_ISR(void);
void ISPM EIRQ2_ISR(void);
void ISPM Timer2_Handler(void);
void ISPM Timer4_Handler(void);
void ISPM PWM5_ISR(void);
void ISPM UART0_ISR(void);
void ISPM UART1_ISR(void);
void ISPM UART2_ISR(void);
void ISPM ADC_ISR(void);
void ISPM SPI1_ISR(void);
BOOL ISPM IsODMVsyncBlank(void);
LONG get_ms(void);

#endif	/* __COMMON_H__ */
