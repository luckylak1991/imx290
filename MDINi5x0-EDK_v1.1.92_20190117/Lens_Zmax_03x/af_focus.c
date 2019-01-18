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
static WORD  drvFOKS, fGoal;

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
static void FOKS_PreProcControl(BOOL dir)
{
	WORD zVal = (WORD)LENS_GetCurPositionZOOM();

	SetMsgCommandRepeatKey(0);		// set repeater MsgFlag
	SetMsgCommandDriveFOKS(0);		// set drvMotor-dir flag
	LENS_EnableMotorFOKS(ON);		// enable focus motor channel
	LENS_SetDirectionFOKS(dir);		// set drive direction

	if (dir==fFAR)	fGoal = LENS_GetMAXPositionLOCI(zVal);
	else			fGoal = LENS_GetMINPositionLOCI(zVal);
}

//--------------------------------------------------------------------------------------------------------------------------
static void FOKS_ProcessControl(BOOL dir)
{
	WORD fVal, zVal = (WORD)LENS_GetCurPositionZOOM();

	if (dir==fFAR)	fVal = LENS_GetMAXPositionLOCI(zVal);
	else			fVal = LENS_GetMINPositionLOCI(zVal);

	LENS_SetDriveFOKS(fVal);	// start motor drive
	LENS_EnableSpeedFOKS(ON);	// enable focus speed

	if (fGoal!=(WORD)fVal) SetMsgCommandOnceExec(OFF);

	if (dir== fFAR && fVal==LENS_GetCurPositionFOKS()) {
		LENS_ForceBreakFOKS(ON);	SetMsgCommandComplete(ON);
	}
	if (dir==fNEAR && fVal==LENS_GetCurPositionFOKS()) {
		LENS_ForceBreakFOKS(ON);	SetMsgCommandComplete(ON);
	}

	if (LENS_GetCurPositionFOKS()==fVal) return;
	UARTprintf("[MF] zCUR = %d, fCUR = %d\r\n", zVal, LENS_GetCurPositionFOKS());
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDirDriveFOKS(BOOL dir)
{
	if (GetDrvOnOffPINT()==ON || GetAgeOnOffLENS()==ON) return;
#if defined(AF_NO_MAN_FOCUS_IN_AUTO)
	if ((stAFCTRL.ctrl&AFCTRL_AFAUTO_MASK)!=AFCTRL_MANUAL_MODE) return;
#endif

	if (IsMsgPreExecCommand())	FOKS_PreProcControl(dir);
	else						FOKS_ProcessControl(dir);
}

//--------------------------------------------------------------------------------------------------------------------------
static void FOKS_PreStepControl(BOOL CoarseFine, BOOL dir)
{
	WORD fVal = LENS_GetCurPositionFOKS();
	WORD zVal = LENS_GetCurPositionZOOM();

	SetMsgCommandRepeatKey(0);		// set repeater MsgFlag
	SetMsgCommandDriveFOKS(1);		// set drvMotor-goal flag
	LENS_EnableMotorFOKS(ON);		// enable focus motor channel
	LENS_SetDirectionFOKS(dir);		// set drive direction

	if(CoarseFine == 1) {
		drvFOKS = (dir==fFAR)? fVal+1 : fVal-1;
		//UARTprintf("drvFOKS:%d/max:%d/min:%d\r\n",drvFOKS,LENS_GetMAXPositionLOCI(zVal),LENS_GetMINPositionLOCI(zVal));
	} else {
		if (dir==fFAR)	drvFOKS = LENS_GetMAXPositionLOCI(zVal);
		else			drvFOKS = LENS_GetMINPositionLOCI(zVal);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void FOKS_MsgStepControl(BOOL dir)
{
//	drvFOKS = MIN(drvFOKS, LENS_GetMaxPositionFOKS());	// check range
//	drvFOKS = MAX(drvFOKS, LENS_GetMinPositionFOKS());

	LENS_SetDriveFOKS(drvFOKS);		// start motor drive
}

//--------------------------------------------------------------------------------------------------------------------------
void CoarseDriveFOKS(BOOL CoarseFine, BOOL dir)
{
	if (GetDrvOnOffPINT()==ON || GetAgeOnOffLENS()==ON) return;
	if ((stAFCTRL.ctrl&AFCTRL_AFAUTO_MASK)!=AFCTRL_MANUAL_MODE) return;

	if (IsMsgPreExecCommand())	FOKS_PreStepControl(CoarseFine, dir);
	else						FOKS_MsgStepControl(dir);
}

//--------------------------------------------------------------------------------------------------------------------------
static void FOKS_PreCmplControl(BOOL dir)
{
	SetMsgCommandRepeatKey(0);		// set repeater MsgFlag
	SetMsgCommandDriveFOKS(1);		// set drvMotor-goal flag
	LENS_EnableMotorFOKS(ON);		// enable focus motor channel
	LENS_SetDirectionFOKS(dir);		// set drive direction

	drvFOKS = LENS_GetCurPositionFOKS();
	drvFOKS += (dir==fNEAR)? -5 : 5;
}

//--------------------------------------------------------------------------------------------------------------------------
static void FOKS_MsgCmplControl(BOOL dir)
{
//	drvFOKS = MIN(drvFOKS, LENS_GetMaxPositionFOKS());	// check range
//	drvFOKS = MAX(drvFOKS, LENS_GetMinPositionFOKS());

	LENS_SetDriveFOKS(drvFOKS);		// start motor drive
}

//--------------------------------------------------------------------------------------------------------------------------
void CompleDriveFOKS(BOOL dir)
{
	if (IsMsgPreExecCommand())	FOKS_PreCmplControl(dir);
	else						FOKS_MsgCmplControl(dir);
}

//--------------------------------------------------------------------------------------------------------------------------
static void FOKS_PreGoalControl(WORD val)
{
	WORD fVal = (WORD)LENS_GetCurPositionFOKS();
	WORD zVal = (WORD)LENS_GetCurPositionZOOM();

	SetMsgCommandRepeatKey(0);		// set repeater MsgFlag
	SetMsgCommandDriveFOKS(1);		// set drvMotor-goal flag
	LENS_EnableMotorFOKS(ON);		// enable focus motor channel

	val = MIN(val, LENS_GetMAXPositionLOCI(zVal));	// check range
	val = MAX(val, LENS_GetMINPositionLOCI(zVal));

	if (fVal>val) LENS_SetDirectionFOKS(fNEAR);
	if (fVal<val) LENS_SetDirectionFOKS( fFAR);

	fGoal = val;
}

//--------------------------------------------------------------------------------------------------------------------------
static void FOKS_MsgGoalControl(WORD val)
{
	WORD zVal = (WORD)LENS_GetCurPositionZOOM();

	val = MIN(val, LENS_GetMAXPositionLOCI(zVal));	// check range
	val = MAX(val, LENS_GetMINPositionLOCI(zVal));

	LENS_SetDriveFOKS(val);			// start motor drive

	if (val!=(WORD)LENS_GetCurPositionFOKS()) return;
	SetMsgCommandComplete(ON);
}

//--------------------------------------------------------------------------------------------------------------------------
void TargetDriveFOKS(WORD val)
{
	SHORT fCnv = LENS_CnvVISCAPositionFOKS((SHORT)val);

	if (GetDrvOnOffPINT()==ON || GetAgeOnOffLENS()==ON) return;
	if ((stAFCTRL.ctrl&AFCTRL_AFAUTO_MASK)!=AFCTRL_MANUAL_MODE) return;

	if (fGoal!=(WORD)fCnv) SetMsgCommandOnceExec(OFF);

	if (IsMsgPreExecCommand())	FOKS_PreGoalControl(fCnv);
	else						FOKS_MsgGoalControl(fCnv);
}
#endif

/*  FILE_END_HERE */
