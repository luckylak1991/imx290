// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static WORD  drvZOOM, zGoal, fGoal;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

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
void ZOOM_PreProcControl(BOOL dir)
{
	SetMsgCommandRepeatKey(0);		// set repeater MsgFlag
	SetMsgCommandDriveZOOM(0);		// set drvMotor-dir flag
	SetMsgCommandTriggerAF(ON);		// set trigger-AF flag
	LENS_EnableMotorZOOM(ON);		// enable zoom motor channel
	if ((stAFCTRL.ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_AFAUTO_MODE) {
		LENS_EnableMotorFOKS(ON);		// enable foks motor channel
		LENS_SetTrackingZOOM(dir);		// set zoom tracking
	}
	LENS_SetDirectionZOOM(dir);		// set drive direction

	if (dir==zTELE) zGoal = LENS_GetMaxPositionZOOM();
	else			zGoal = LENS_GetMinPositionZOOM();
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ZoomCrossF = 0;
static BOOL ZoomCrossFbak = 0;
static BOOL zoomFirstF = 1;
extern BOOL fDZ_UpdateHandler;
extern BOOL startFocusPickSearch;
void ZOOM_ProcessControl(BOOL dir)
{
	WORD zVal;

	if (dir==zTELE)	zVal = LENS_GetMaxPositionZOOM();
	else			zVal = LENS_GetMinPositionZOOM();

	if ((stAFCTRL.ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_AFAUTO_MODE)	LENS_EnableTrackZOOM(ON);		// enable zoom tracking
	else		LENS_EnableTrackZOOM(OFF);		// disable zoom tracking
	LENS_SetDriveZOOM(zVal);		// start motor drive
	LENS_EnableSpeedZOOM(ON);		// enable zoom speed

	if (zGoal!=(WORD)zVal) SetMsgCommandOnceExec(OFF);

	if (dir==zTELE && zVal==LENS_GetCurPositionZOOM()) {
		LENS_ForceBreakZOOM(ON);	SetMsgCommandGuardZOOM(ON);
		if (IsMsgCommand_UART())	SetMsgCommandComplete(ON);
	}
	if (dir==zWIDE && zVal==LENS_GetCurPositionZOOM()) {
		LENS_ForceBreakZOOM(ON);	SetMsgCommandGuardZOOM(ON);
		if (IsMsgCommand_UART())	SetMsgCommandComplete(ON);
	}

	//if((LENS_GetCurPositionDZOOM() == 0) && (LENS_GetRelPositionZOOM() <= diffMinMaxZoom))
	if(LENS_GetCurPositionZOOM() <= limitDigitalZoom)	ZoomCrossF = 1;
	else												ZoomCrossF = 0;

	if(zoomFirstF == 0) ZoomCrossFbak = ZoomCrossF;
	zoomFirstF = 1;
	
	if(ZoomCrossFbak != ZoomCrossF) {
		//UARTprintf("Digital <---> Analog Zoom\r\n");
		//LENS_ForceBreakZOOM(ON);
		//SetMsgCommandRepeatKeyOff(0);
	}
	ZoomCrossFbak = ZoomCrossF;

	fDZ_UpdateHandler = 1;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDirDriveZOOM(BOOL dir)
{
	if (GetAgeOnOffLENS()==ON) return;
	if (IsMsgCommandGuardZOOM()==TRUE) return;

	if ((stAFCTRL.ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_AFAUTO_MODE) {
		if(stAFCTRL.autoFcnt >= 60) return;
		startFocusPickSearch = TRUE;
	} else {
		startFocusPickSearch = FALSE;
	}

	if (IsMsgPreExecCommand())	ZOOM_PreProcControl(dir);
	else						ZOOM_ProcessControl(dir);
}

//--------------------------------------------------------------------------------------------------------------------------
static void ZOOM_PreStepControl(BOOL dir)
{
	SetMsgCommandRepeatKey(0);		// set repeater MsgFlag
	SetMsgCommandDriveZOOM(1);		// set drvMotor-goal flag
	LENS_EnableMotorZOOM(ON);		// enable zoom motor channel
	LENS_SetDirectionZOOM(dir);		// set drive direction

	drvZOOM = LENS_GetCurPositionZOOM();
	drvZOOM += (dir==zWIDE)? -20 : 20;
}

//--------------------------------------------------------------------------------------------------------------------------
static void ZOOM_MsgStepControl(BOOL dir)
{
//	drvZOOM = MIN(drvZOOM, LENS_GetMaxPositionZOOM());	// check range
//	drvZOOM = MAX(drvZOOM, LENS_GetMinPositionZOOM());

	LENS_EnableTrackZOOM(OFF);		// disable zoom tracking
	LENS_SetDriveZOOM(drvZOOM);		// start motor drive
}

//--------------------------------------------------------------------------------------------------------------------------
void CoarseDriveZOOM(BOOL dir)
{
	if (GetAgeOnOffLENS()==ON) return;

	if (IsMsgPreExecCommand())	ZOOM_PreStepControl(dir);
	else						ZOOM_MsgStepControl(dir);
}

//--------------------------------------------------------------------------------------------------------------------------
static void ZOOM_PreCmplControl(BOOL dir)
{
	SetMsgCommandRepeatKey(0);		// set repeater MsgFlag
	SetMsgCommandDriveZOOM(1);		// set drvMotor-goal flag
	LENS_EnableMotorZOOM(ON);		// enable focus motor channel
	LENS_SetDirectionZOOM(dir);		// set drive direction

	drvZOOM = LENS_GetCurPositionZOOM();
	drvZOOM += (dir==zWIDE)? -5 : 5;
}

//--------------------------------------------------------------------------------------------------------------------------
static void ZOOM_MsgCmplControl(BOOL dir)
{
//	drvZOOM = MIN(drvZOOM, LENS_GetMaxPositionZOOM());	// check range
//	drvZOOM = MAX(drvZOOM, LENS_GetMinPositionZOOM());

	LENS_EnableTrackZOOM(OFF);		// disable zoom tracking
	LENS_SetDriveZOOM(drvZOOM);		// start motor drive
}

//--------------------------------------------------------------------------------------------------------------------------
void CompleDriveZOOM(BOOL dir)
{
	if (IsMsgPreExecCommand())	ZOOM_PreCmplControl(dir);
	else						ZOOM_MsgCmplControl(dir);
}

//--------------------------------------------------------------------------------------------------------------------------
static void ZOOM_PreGoalControl(WORD val)
{
	WORD zVal = (WORD)LENS_GetCurPositionZOOM();

	SetMsgCommandRepeatKey(0);		// set repeater MsgFlag
	SetMsgCommandDriveZOOM(1);		// set drvMotor-goal flag
	SetMsgCommandTriggerAF(ON);		// set trigger-AF flag
	LENS_EnableMotorZOOM(ON);		// enable zoom motor channel
	LENS_EnableMotorFOKS(ON);		// enable foks motor channel

	val = MIN(val, LENS_GetMaxPositionZOOM());	// check range
	val = MAX(val, LENS_GetMinPositionZOOM());

	if (zVal>val) LENS_SetDirectionZOOM(zWIDE);
	if (zVal<val) LENS_SetDirectionZOOM(zTELE);

	zGoal = val;
}

//--------------------------------------------------------------------------------------------------------------------------
static void ZOOM_MsgGoalControl(WORD val)
{
	val = MIN(val, LENS_GetMaxPositionZOOM());	// check range
	val = MAX(val, LENS_GetMinPositionZOOM());

//	LENS_EnableTrackZOOM(OFF);		// disable zoom tracking
	if ((stAFCTRL.ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_AFAUTO_MODE)
		LENS_EnableTrackZOOM(ON);		// enable zoom tracking
	else
		LENS_EnableTrackZOOM(OFF);		// disable zoom tracking
	LENS_SetDriveZOOM(val);			// start motor drive

	if (val!=(WORD)LENS_GetCurPositionZOOM()) return;
	SetMsgCommandComplete(ON);
}

//--------------------------------------------------------------------------------------------------------------------------
void TargetDriveZOOM(WORD val)
{
	SHORT zCnv = LENS_CnvVISCAPositionZOOM((SHORT)val);

	if (GetAgeOnOffLENS()==ON) return;
	if (zGoal!=(WORD)zCnv) SetMsgCommandOnceExec(OFF);

	if (IsMsgPreExecCommand())	ZOOM_PreGoalControl(zCnv);
	else						ZOOM_MsgGoalControl(zCnv);
}

//--------------------------------------------------------------------------------------------------------------------------
static void LENS_PreGoalControl(WORD zVal, WORD fVal)
{
	WORD zCur = (WORD)LENS_GetCurPositionZOOM();
	WORD fCur = (WORD)LENS_GetCurPositionFOKS();

	SetMsgCommandRepeatKey(0);		// set repeater MsgFlag
	SetMsgCommandDriveLENS(1);		// set drvMotor-goal flag
	SetMsgCommandTriggerAF(ON);		// set trigger-AF flag
	LENS_EnableMotorZOOM(ON);		// enable zoom motor channel
	LENS_EnableMotorFOKS(ON);		// enable foks motor channel

	zVal = MIN(zVal, LENS_GetMaxPositionZOOM());	// check range
	zVal = MAX(zVal, LENS_GetMinPositionZOOM());

	if (zCur>zVal) LENS_SetDirectionZOOM(zWIDE);
	if (zCur<zVal) LENS_SetDirectionZOOM(zTELE);

	fVal = MIN(fVal,LENS_GetMAXPositionLOCI(zVal));	// check range
	fVal = MAX(fVal,LENS_GetMINPositionLOCI(zVal));

	if (fCur>fVal) LENS_SetDirectionFOKS(fNEAR);
	if (fCur<fVal) LENS_SetDirectionFOKS( fFAR);

	zGoal = zVal;	fGoal = fVal;
}

//--------------------------------------------------------------------------------------------------------------------------
static void LENS_MsgGoalControl(WORD zVal, WORD fVal)
{
	zVal = MIN(zVal, LENS_GetMaxPositionZOOM());	// check range
	zVal = MAX(zVal, LENS_GetMinPositionZOOM());

	fVal = MIN(fVal,LENS_GetMAXPositionLOCI(zVal)); // check range
	fVal = MAX(fVal,LENS_GetMINPositionLOCI(zVal));

	LENS_EnableTrackZOOM(OFF);		// disable zoom tracking
//	LENS_EnableTrackZOOM(ON);		// enable zoom tracking
	LENS_SetDriveZOOM(zVal);		// start motor drive
	LENS_SetDriveFOKS(fVal);		// start motor drive

	SetMsgCommandComplete(ON);
}

//--------------------------------------------------------------------------------------------------------------------------
void TargetDriveLENS(WORD zVal, WORD fVal)
{
	SHORT zCnv = LENS_CnvVISCAPositionZOOM((SHORT)zVal);
	SHORT fCnv = LENS_CnvVISCAPositionLOCI((SHORT)zVal,(SHORT)fVal);

	if (GetAgeOnOffLENS()==ON) return;

	if (IsMsgPreExecCommand())	LENS_PreGoalControl(zCnv, fCnv);
	else						LENS_MsgGoalControl(zCnv, fCnv);
}
#endif

/*  FILE_END_HERE */
