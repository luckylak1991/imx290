// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<stdio.h>
#include	"video.h"

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#define	MSG_FLAGS()		(((BYTE)fMsgExecutionFlag<<7)|((BYTE)fMsgExecutionBusy<<6)|\
						 ((BYTE)fMsgExecutionOnce<<4)|((BYTE)fMsgExecCompleted<<3)|\
						 ((BYTE)fMsgExecTargetKey<<2)|((BYTE)fMsgDriveMotorKey<<1)|\
						 ((BYTE)fMsgExecRepeatKey<<0))

#define	CLEAR_FLAGS()	fMsgExecutionFlag = fMsgExecutionBusy = fMsgExecutionOnce =\
						fMsgExecCompleted = fMsgExecRepeatKey = fMsgExecGuardZOOM = 0

#define	MSG_MODES()		(((BYTE)fMsgUSB_CtrlMode<<5)|((BYTE)fMsgPELCO_CtrlMode<<4)|\
						 ((BYTE)fMsgVISCA_CtrlMode<<3)|((BYTE)fMsgSerialCtrlMode<<2)|\
						 ((BYTE)fMsgIrCodeCtrlMode<<1)|((BYTE)fMsgButtonCtrlMode<<0))

#define	CLEAR_MODES()	fMsgPELCO_CtrlMode = fMsgUSB_CtrlMode =\
						fMsgVISCA_CtrlMode = fMsgSerialCtrlMode =\
						fMsgIrCodeCtrlMode = fMsgButtonCtrlMode = 0

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static volatile BOOL fMsgExecutionFlag, fMsgExecutionBusy, fMsgExecutionOnce, fMsgExecCompleted;
static volatile BOOL fMsgExecTargetKey, fMsgZoomTargetKey, fMsgFoksTargetKey;
static volatile BOOL fMsgDriveMotorKey, fMsgExecRepeatKey, fMsgExecGuardZOOM;
static volatile BOOL fMsgExecTriggerAF; // AF Trigger Flag
static volatile BOOL fMsgVISCA_CtrlMode, fMsgSerialCtrlMode, fMsgPELCO_CtrlMode, fMsgUSB_CtrlMode;
static volatile BOOL fMsgIrCodeCtrlMode, fMsgButtonCtrlMode;

static volatile BYTE ButtonNewCmd, ButtonOldCmd;
static volatile WORD IrCodeNewCmd, IrCodeOldCmd;
static volatile WORD SerialNewCmd, SerialOldCmd;
static volatile WORD VISCA_NewCmd, VISCA_OldCmd;
static volatile WORD PELCO_NewCmd, PELCO_OldCmd;
static volatile WORD USB_NewCmd, USB_OldCmd;

static volatile BYTE rptMsgSpeed, rptMsgCount, drvEndCount;

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
static void ISPM MsgDZ1XCtrlSpeed(void)	
{
	rptMsgCount++;
	switch (rptMsgCount) {		
//		case (28):	rptMsgCount = 23;		
		case (25):	rptMsgCount = 23;		//bh 2013.12.17 for dzomm
		case (23):
//		case (18):
//		case (12):			                           // 1st execution speed
//		case (  6):			
		fMsgExecutionFlag = TRUE; break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM MsgDZ2XCtrlSpeed(void)	
{
	rptMsgCount++;
	switch (rptMsgCount) {		
		case (20):	rptMsgCount = 19;
		case (19):
		fMsgExecutionFlag = TRUE; break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM MsgLevelCtrlSpeed(void)
{
	rptMsgCount++;
	switch (rptMsgCount) {
		case (210):	rptMsgCount = 204;		// 3rd execution speed
		case (204):
		case (192):
		case (180):
		case (168):
		case (156):
		case (144):
		case (132):
		case (120):
		case (108):
		case ( 96):							// 2nd execution speed
		case ( 84):
		case ( 42):							// 1st execution speed
		fMsgExecutionFlag = TRUE; break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM MsgDirectionSpeed(void)
{
	rptMsgCount++;
	switch (rptMsgCount) {
		case (28):	rptMsgCount = 26;		// 2nd execution speed
		case (26):
		case ( 1):							// 1st execution speed
		fMsgExecutionFlag = TRUE; break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM MessageExeSpeedAdjust(void)	
{
	switch (rptMsgSpeed) {
		case MSG_LEVEL:	MsgLevelCtrlSpeed(); break;
		case MSG_SHIFT:	MsgDirectionSpeed(); break;
		case MSG_DZ1X: MsgDZ1XCtrlSpeed(); break;
		case MSG_DZ2X: MsgDZ2XCtrlSpeed(); break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM IsKeyStatePushed(void)
{
	BOOL status = TRUE;

	if (fMsgButtonCtrlMode&&ButtonOldCmd!=ButtonNewCmd) {
		status = FALSE;	ButtonOldCmd = ButtonNewCmd;
	}
	if (fMsgIrCodeCtrlMode&&IrCodeOldCmd!=IrCodeNewCmd) {
		status = FALSE;	IrCodeOldCmd = IrCodeNewCmd;
	}
	
	if (status==FALSE) fMsgExecCompleted = TRUE;
	return status;
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM MessageDrvCompletion(void)	
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	if (!fMsgZoomTargetKey) LENS_ForceBreakZOOM(ON);	// force complete
	if (!fMsgFoksTargetKey) LENS_ForceBreakFOKS(ON);	// force complete

	if (AF_IsProcessExecuting()==TRUE) return;
	if (LENS_IsDriveCompleteZOOM()==FALSE) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
	if (++drvEndCount<2) return;  drvEndCount = 0;		// wait 1VD

	fMsgDriveMotorKey = FALSE;	fMsgExecTargetKey = FALSE;
	fMsgZoomTargetKey = FALSE;	fMsgFoksTargetKey = FALSE;
	LENS_EnableMotorZOOM(OFF);	LENS_EnableMotorFOKS(OFF);	// disable motor
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM MessageExeCompletion(void)
{
	rptMsgCount = 0;
	CLEAR_FLAGS();
	CLEAR_MODES();
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM IsMsgCommandOccur(void)
{
	return (MSG_MODES()&0x3F)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM IsMsgExecutionBusy(void)
{
	return (fMsgExecutionBusy)? TRUE : FALSE;
}

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM IsAFExecBreakByZOOM(BYTE msg)
{
	if (AF_IsProcessExecuting()==FALSE) return FALSE;
	if (IsMsgCommandGuardZOOM()==TRUE)  return FALSE;

	switch (msg) {
//		case MSG_BUTTON: if (IsButtonCommandZOOM()) break;
//						 return FALSE;
		case MSG_IRCODE: if (IsIrCodeCommandZOOM()) break;
						 return FALSE;
		case MSG_VISCOM: if (IsVISCA_CommandZOOM()) break;
						 return FALSE;
		case MSG_PELCOM: if (IsPELCO_CommandZOOM()) break;
						 return FALSE;
		case MSG_USBCOM: if (IsUSB_CommandZOOM()) break;
						 return FALSE;
	}	
	AF_EnableProcess(OFF);	return TRUE;
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM IsMsgAFProcessBusy(void)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))

	switch (MSG_MODES()&0x3F) {
//		case (1<<MSG_BUTTON):	return IsAFExecBreakByZOOM(MSG_BUTTON);
		case (1<<MSG_IRCODE):	return IsAFExecBreakByZOOM(MSG_IRCODE);
//		case (1<<MSG_SERIAL):	return IsAFExecBreakByZOOM(MSG_SERIAL);
		case (1<<MSG_VISCOM):	return IsAFExecBreakByZOOM(MSG_VISCOM);
		case (1<<MSG_PELCOM):	return IsAFExecBreakByZOOM(MSG_PELCOM);
		case (1<<MSG_USBCOM):	return IsAFExecBreakByZOOM(MSG_USBCOM);
	}
#endif

	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM MessageCtrlClassify(void)	
{
	ButtonMessageClassify();
	IrCodeMessageClassify();
	SerialMessageClassify();
	VISCA_MessageClassify();
	PELCO_MessageClassify();
	USB_MessageClassify();

	if (IsMsgCommandOccur()==FALSE) return;
	if (IsMsgExecutionBusy()==TRUE) return;
	if (IsMsgAFProcessBusy()==TRUE) return;

	if		(fMsgExecCompleted) {
		if (fMsgDriveMotorKey)	MessageDrvCompletion();
		else					MessageExeCompletion();
	}
	else if (fMsgDriveMotorKey)	fMsgExecutionFlag = TRUE;
	else if (fMsgExecRepeatKey) {
		if (IsKeyStatePushed()) MessageExeSpeedAdjust();
	}
	else fMsgExecutionFlag = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
void MessageCtrlHandler(void)
{
	if (fMsgExecutionFlag==FALSE) return;
	fMsgExecutionFlag = FALSE;

	if		(fMsgButtonCtrlMode) {		// case of key-button message
		if (!fMsgExecutionOnce) {		ButtonOldCmd = ButtonNewCmd;
			ButtonMessageExecution();	fMsgExecutionOnce = TRUE;
		}
		else if (ButtonOldCmd!=ButtonNewCmd) {
			fMsgExecCompleted = TRUE;	ButtonOldCmd = ButtonNewCmd;
		}
		else if (fMsgExecRepeatKey)		ButtonMessageExecution();
	}

	if		(fMsgIrCodeCtrlMode) {		// case of remote-control message
		if (!fMsgExecutionOnce) {		IrCodeOldCmd = IrCodeNewCmd;
			IrCodeMessageExecution();	fMsgExecutionOnce = TRUE;	
		}
		else if (IrCodeOldCmd!=IrCodeNewCmd) {
			fMsgExecCompleted = TRUE;	IrCodeOldCmd = IrCodeNewCmd;
		}
		else if (fMsgExecRepeatKey)		IrCodeMessageExecution();
	}
	else if (fMsgSerialCtrlMode) {		// case of serial-control message
		if (!fMsgExecutionOnce) {		SerialOldCmd = SerialNewCmd;
			SerialMessageExecution();	fMsgExecutionOnce = TRUE;
		}
		else if (SerialOldCmd!=SerialNewCmd) {
			fMsgExecCompleted = TRUE;	SerialOldCmd = SerialNewCmd;
		}
		else if (fMsgExecRepeatKey)		SerialMessageExecution();
		else							fMsgExecCompleted = TRUE;
	}
	else if (fMsgVISCA_CtrlMode) {		// case of viscom-control message
		if (!fMsgExecutionOnce) {		VISCA_OldCmd = VISCA_NewCmd;
			VISCA_MessageExecution();	fMsgExecutionOnce = TRUE;
		}
		else if (VISCA_OldCmd!=VISCA_NewCmd) {
			fMsgExecCompleted = TRUE;	VISCA_OldCmd = VISCA_NewCmd;
		}
		else if (fMsgExecRepeatKey)		VISCA_MessageExecution();
		else							fMsgExecCompleted = TRUE;
	}
	else if (fMsgPELCO_CtrlMode) {		// case of pelcom-control message
		if (!fMsgExecutionOnce) {		PELCO_OldCmd = PELCO_NewCmd;
			PELCO_MessageExecution();	fMsgExecutionOnce = TRUE;
		}
		else if (PELCO_OldCmd!=PELCO_NewCmd) {
			fMsgExecCompleted = TRUE;	PELCO_OldCmd = PELCO_NewCmd;
		}
		else if (fMsgExecRepeatKey)		PELCO_MessageExecution();
		else							fMsgExecCompleted = TRUE;
	}
	else if (fMsgUSB_CtrlMode) {		// case of usb-control message
		if (!fMsgExecutionOnce) {		USB_OldCmd = USB_NewCmd;
			USB_MessageExecution();		fMsgExecutionOnce = TRUE;
		}
		else if (USB_OldCmd!=USB_NewCmd) {
			fMsgExecCompleted = TRUE;	USB_OldCmd = USB_NewCmd;
		}
		else if (fMsgExecRepeatKey)		USB_MessageExecution();
		else							fMsgExecCompleted = TRUE;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SetMsgButtonCtrlMode(void)
{
	fMsgButtonCtrlMode = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SetMsgButtonNewCmd(BYTE val)
{
	ButtonNewCmd = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SetMsgIrCodeCtrlMode(void)
{
	fMsgIrCodeCtrlMode = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SetMsgIrCodeNewCmd(WORD val)
{
	IrCodeNewCmd = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SetMsgSerialCtrlMode(void)	
{
	fMsgSerialCtrlMode = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SetMsgSerialNewCmd(WORD val)	
{
	SerialNewCmd = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SetMsgVISCA_CtrlMode(void)	
{
	fMsgVISCA_CtrlMode = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SetMsgVISCA_NewCmd(WORD val)	
{
	// zoom standard/variable/direct
	if (val==0x0702 || val==0x0703 || val==0x0720 || val==0x0730 || val==0x470D)
		val = 0x470D;

	// foks standard/variable/direct
	if (val==0x0802 || val==0x0803 || val==0x0820 || val==0x0830 || val==0x480D)
		val = 0x480D;

	VISCA_NewCmd = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SetMsgPELCO_CtrlMode(void)	
{
	fMsgPELCO_CtrlMode = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SetMsgPELCO_NewCmd(WORD val)	
{
	if (val==0x1000 || val==0x2000 || val==0x5000) 		val = 0x1000; 	// zoom standard/direct
	if (val==0x3000 || val==0x4000)						val = 0x3000; 	// foks standard
	
	PELCO_NewCmd = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SetMsgUSB_CtrlMode(void)
{
	fMsgUSB_CtrlMode = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SetMsgUSB_NewCmd(WORD val)
{
	if (val==0x1000 || val==0x2000 || val==0x5000) 		val = 0x1000; 	// zoom standard/direct
	if (val==0x3000 || val==0x4000 || val==0x6000)		val = 0x3000; 	// foks standard/direct
	
	USB_NewCmd = val;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL ISPM IsOtherMsgPreemptive(BYTE msg)
{
	msg = (~(1<<msg))&0x3F;
	return (MSG_MODES()&msg)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL IsMsgCommand_IR_KEY(void)
{
	if (fMsgIrCodeCtrlMode==TRUE) return TRUE;
	if (fMsgButtonCtrlMode==TRUE) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL IsMsgCommand_UART(void)
{
	if (fMsgVISCA_CtrlMode==TRUE) return TRUE;
	if (fMsgPELCO_CtrlMode==TRUE) return TRUE;
	if (fMsgUSB_CtrlMode==TRUE) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL IsMsgPreExecCommand(void)
{
	return (fMsgExecutionOnce==FALSE)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetMsgCommandComplete(BOOL OnOff)
{
	fMsgExecCompleted = (OnOff==ON)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SetMsgCommandLongExec(BOOL OnOff)
{
	fMsgExecutionBusy = (OnOff==ON)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetMsgCommandOnceExec(BOOL OnOff)
{
	fMsgExecutionOnce = (OnOff==ON)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetMsgCommandRepeatKey(BYTE val)
{
	rptMsgSpeed = val;
	fMsgExecRepeatKey = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetMsgCommandRepeatKeyOff(BYTE val)
{
	rptMsgSpeed = val;
	fMsgExecRepeatKey = FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SetMsgCommandDriveLENS(BOOL OnOff)
{
	fMsgExecCompleted = FALSE;
	fMsgDriveMotorKey = TRUE;

	fMsgZoomTargetKey = OnOff;
	fMsgFoksTargetKey = OnOff;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SetMsgCommandDriveZOOM(BOOL OnOff)
{
	fMsgExecCompleted = FALSE;
	fMsgDriveMotorKey = TRUE;
	fMsgZoomTargetKey = OnOff;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM SetMsgCommandDriveFOKS(BOOL OnOff)
{
	fMsgExecCompleted = FALSE;
	fMsgDriveMotorKey = TRUE;
	fMsgFoksTargetKey = OnOff;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetMsgCommandTriggerAF(BOOL OnOff) // set AF Trigger flag
{
	fMsgExecTriggerAF = (OnOff==ON)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL IsMsgCommandTriggerAF(void) // check AF Trigger
{
	return (fMsgExecTriggerAF)? TRUE : FALSE;
}

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
//--------------------------------------------------------------------------------------------------------------------------
void SetMsgCommandGuardZOOM(BOOL OnOff)
{
	fMsgExecGuardZOOM = (OnOff==ON)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL IsMsgCommandGuardZOOM(void)
{
	return (fMsgExecGuardZOOM)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL IsMsgGuardZOOMComplete(void)	
{
	if (IsMsgCommandGuardZOOM()==FALSE) return FALSE;
	if (LENS_IsDriveCompleteZOOM()==FALSE) return FALSE;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return FALSE;
	if (++drvEndCount<2) return FALSE;  drvEndCount = 0;	// wait 2VD

	fMsgDriveMotorKey = FALSE;	fMsgExecTargetKey = FALSE;
	fMsgZoomTargetKey = FALSE;	fMsgFoksTargetKey = FALSE;
	LENS_EnableMotorZOOM(OFF);	LENS_EnableMotorFOKS(OFF);	// disable motor
	return TRUE;
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
BOOL IsMsgExecutionComplete(void) // check current Message
{
	WORD msg = MAKEWORD(MSG_FLAGS(), MSG_MODES());
	return (msg==0)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ISPM GetPreemptiveMessage(BYTE msg)
{
	msg = (~(1<<msg))&0x3F;
	return MSG_MODES()&msg;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetMsgClrFlags(BOOL OnOff)
{
	if(OnOff)		CLEAR_FLAGS();
}


/*  FILE_END_HERE */
