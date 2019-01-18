// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"
#include	"menu.h"

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

#define		AFCTRL_PINTMODE_MASK			0x0000000F
#define		AFCTRL_PINTDONE_MODE		0x00000000
#define		AFCTRL_PINTMAXF_MODE			0x00000001
#define		AFCTRL_PINTTELE_MODE			0x00000002
#define		AFCTRL_PINTWIDE_MODE			0x00000003

static LONG	pintMode = AFCTRL_PINTDONE_MODE;

typedef	struct
{
	LONG	YSUM;			// AE integration YSUM data
	LONG	YREF;			// AX normalized ref. data
	LONG	FLTo;			// AX integration HPF data
	LONG	FLTn;			// AX normalized HPF data
	LONG	FLTx;			// AX maximum norm-HPF data

	BYTE	fDIR;			// AX control focus drive direction
	BYTE	fCUR;			// AX control focus current process
	BYTE	fPRV;			// AX control focus previous process
	BYTE	fCHK;			// dummy

	BYTE	zDIR;			// AX control zoom drive direction
	BYTE	zCUR;			// AX control zoom current process
	BYTE	zPRV;			// AX control zoom previous process
	BYTE	zCHK;			// dummy

	SHORT	xCNT;			// AX control motor drive step
	SHORT	xGAP;			// AX control motor drive range
	SHORT	fTGT;			// AX control focus drive target
	SHORT	zTGT;			// AX control zoom drive target

	SHORT	goal;			// AX control motor drive target
	SHORT	peak;			// AX control motor peak position
	SHORT	mMAX;			// AX control motor mechanic max
	SHORT	mMIN;			// AX control motor mechanic min
	SHORT	fVAL;			// AX control focus position data
	SHORT	fTOP;			// AX control focus previous peak
	SHORT	fMAX;			// AX control focus drive max range
	SHORT	fMIN;			// AX control focus drive min range
	SHORT	zVAL;			// AX control zoom position data
	SHORT	zMAX;			// AX control zoom drive max range
	SHORT	zMIN;			// AX control zoom drive min range
	WORD	time;			// AX control execution time

}	AX_PINT_PARAM, *PAX_PINT_PARAM;

typedef enum {
	PRE_FILTER_SEL = 0x10, DRV_FILTER_SEL, INV_FILTER_SEL, END_FILTER_SEL,
	PRE_FINAL_EXIT = 0x60, DRV_FINAL_EXIT, INV_FINAL_EXIT, END_FINAL_EXIT,
	PRE_SWING_MOVE = 0x70, DRV_SWING_MOVE, INV_SWING_MOVE, END_SWING_MOVE,
	PRE_SWING_SCAN = 0x80, DRV_SWING_SCAN, INV_SWING_SCAN, END_SWING_SCAN,
	END_CLEAR_PROC = 0x00

}	AX_PROCESS_STEP;

#define		OSD_COLOR	MAKEBYTE(OSD_WHITE, OSD_NOCOLOR)

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
VBYTE fIsrDRVLensSTOP = FALSE;

static WORD zGoal, fGoal;
static BYTE fSysDRVModePINT = 0;

static AX_PINT_PARAM stAXPINT;

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
static void PINT_DisplayCLEAR(void)
{
	BYTE i;
	for (i=0; i<19; i++) OSD_StrDispAttb(i,0,ON, OSD_COLOR, "%36s", "");
}

//--------------------------------------------------------------------------------------------------------------------------
static void AXPINT_GetAXOPDWIND(PAF_CTRL_PARAM pCTRL)
{
	FLOAT nYSUM;

	stAXPINT.FLTo = pCTRL->stFILT.HPFa+pCTRL->stFILT.HPFb*4;
	stAXPINT.YSUM = pCTRL->stFILT.YSUM;

	if (stAXPINT.YREF==0) stAXPINT.YREF = stAXPINT.YSUM;
	nYSUM = ((FLOAT)stAXPINT.YREF)/stAXPINT.YSUM;

	stAXPINT.FLTn = (LONG)(nYSUM*stAXPINT.FLTo+0.5);

	stAXPINT.fDIR = LENS_GetDirectionFOKS();
	stAXPINT.fVAL = LENS_GetCurPositionFOKS();
	stAXPINT.fTGT = LENS_GetPINTDrvGOAL(fPINT);

	stAXPINT.zDIR = LENS_GetDirectionZOOM();
	stAXPINT.zVAL = LENS_GetCurPositionZOOM();
	stAXPINT.zTGT = LENS_GetPINTDrvGOAL(zPINT);
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFPINT_IsDriveEnd(void)
{
	if (stAXPINT.fVAL<=stAXPINT.fMIN) return TRUE;
	if (stAXPINT.fVAL>=stAXPINT.fMAX) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFPINT_SetChangeDIR(void)
{
	stAXPINT.fDIR = (stAXPINT.fDIR==fNEAR)? fFAR : fNEAR;
	LENS_SetDirectionFOKS(stAXPINT.fDIR);
	return stAXPINT.fDIR;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFPINT_SetDriveSTEP(BYTE nID)
{
	stAXPINT.goal = LENS_GetCurPositionFOKS();

	if (stAXPINT.fDIR==fFAR)	stAXPINT.goal += stAXPINT.xCNT;
	else						stAXPINT.goal -= stAXPINT.xCNT;

	stAXPINT.goal = MIN(stAXPINT.goal, stAXPINT.fMAX);	// check range
	stAXPINT.goal = MAX(stAXPINT.goal, stAXPINT.fMIN);

	LENS_SetDriveFOKS(stAXPINT.goal);	// start motor drive
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFPINT_SetDrivePEAK(WORD val)
{
	stAXPINT.goal = val;

	stAXPINT.goal = MIN(stAXPINT.goal, stAXPINT.fMAX);	// check range
	stAXPINT.goal = MAX(stAXPINT.goal, stAXPINT.fMIN);

	LENS_SetDriveFOKS(stAXPINT.goal);	// start motor drive
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFPINT_GetJudgeINFO(void)
{
	if (stAXPINT.FLTn<stAXPINT.FLTx) return;

	stAXPINT.FLTx = stAXPINT.FLTn;
	stAXPINT.peak = stAXPINT.fVAL;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFPINT_SetDrvDirBWD(BYTE nID)
{
	AFPINT_SetChangeDIR();	stAXPINT.fCUR = INV_SWING_SCAN;

	stAXPINT.peak = MIN(stAXPINT.peak, stAXPINT.fMAX);	// check range
	stAXPINT.peak = MAX(stAXPINT.peak, stAXPINT.fMIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFPINT_SetDrvDirFWD(BYTE nID)
{
	BOOL end = AFPINT_IsDriveEnd();

	if (end==TRUE)	AFPINT_SetDrvDirBWD(nID);
	else			AFPINT_SetDriveSTEP(nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFPINT_PreFilterSEL(void)
{
	if (stAXPINT.fCUR!=PRE_FILTER_SEL) return;

	stAXPINT.FLTx = 0;	stAXPINT.YREF = 0;	

	stAXPINT.peak = LENS_GetCurPositionFOKS();
	stAXPINT.mMIN = LENS_GetMinPositionFOKS();
	stAXPINT.mMAX = LENS_GetMaxPositionFOKS();

	stAXPINT.xCNT = LENS_GetPINTDrvSTEP(fPINT);
	stAXPINT.xGAP = LENS_GetPINTDrvRANGE(fPINT);

	stAXPINT.fMIN = MAX(stAXPINT.fVAL-stAXPINT.xGAP,stAXPINT.mMIN);
	stAXPINT.fMAX = MIN(stAXPINT.fVAL+stAXPINT.xGAP,stAXPINT.mMAX);

	stAXPINT.fCUR = END_FILTER_SEL;
	LENS_SetDirectionFOKS(fNEAR);	// fix NEAR direction
	LENS_EnableMotorFOKS(ON);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFPINT_EndFilterSEL(void)
{
	if (stAXPINT.fCUR!=END_FILTER_SEL) return;
	stAXPINT.fCUR = PRE_SWING_MOVE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFPINT_PreSwingMOVE(void)
{
	if (stAXPINT.fCUR!=PRE_SWING_MOVE) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	LENS_SetDriveFOKS(stAXPINT.fMIN);

	if (AFPINT_IsDriveEnd()==FALSE) return;
	stAXPINT.fCUR = END_SWING_MOVE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFPINT_EndSwingMOVE(void)
{
	if (stAXPINT.fCUR!=END_SWING_MOVE) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFPINT_SetChangeDIR();
	stAXPINT.fCUR = PRE_SWING_SCAN;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFPINT_PreSwingSCAN(void)
{
	if (stAXPINT.fCUR!=PRE_SWING_SCAN) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
	
	AFPINT_GetJudgeINFO();

	stAXPINT.fCUR = DRV_SWING_SCAN;
	AFPINT_SetDriveSTEP(DRV_SWING_SCAN);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFPINT_DrvSwingSCAN(void)
{
	if (stAXPINT.fCUR!=DRV_SWING_SCAN) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFPINT_GetJudgeINFO();
	AFPINT_SetDrvDirFWD(DRV_SWING_SCAN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFPINT_EndSwingSCAN(void)
{
	if (stAXPINT.fCUR!=END_SWING_SCAN) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAXPINT.fCUR = END_FINAL_EXIT;
	AFPINT_SetDrivePEAK(stAXPINT.peak);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFPINT_EndClimbPEAK(void)
{
	if (stAXPINT.fCUR!=END_FINAL_EXIT) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAXPINT.fCUR = END_CLEAR_PROC;	// stop progress
	LENS_EnableMotorFOKS(OFF);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFPINT_EndDriveWAIT(void)
{
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	switch (stAXPINT.fCUR) {
		case INV_SWING_MOVE: stAXPINT.fCUR = END_SWING_MOVE; break;
		case INV_SWING_SCAN: stAXPINT.fCUR = END_SWING_SCAN; break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFPINT_ProcessHandler(void)
{
	if (stAXPINT.fCUR==0) return;
	stAXPINT.fPRV = stAXPINT.fCUR;	// for debug print

	AFPINT_PreFilterSEL();		// select filter
	AFPINT_PreSwingMOVE();		// moving bgn-scan
	AFPINT_PreSwingSCAN();		// search all-scan

	AFPINT_DrvSwingSCAN();		// drive all-scan

	AFPINT_EndFilterSEL();		// arrange filter
	AFPINT_EndSwingMOVE();		// arrange bgn-scan
	AFPINT_EndSwingSCAN();		// arrange all-scan

	AFPINT_EndClimbPEAK();		// drive peak-point
	AFPINT_EndDriveWAIT();		// wait drive-stop
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AZPINT_IsDriveEnd(void)
{
	if (stAXPINT.zVAL<=stAXPINT.zMIN) return TRUE;
	if (stAXPINT.zVAL>=stAXPINT.zMAX) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AZPINT_SetChangeDIR(void)
{
	stAXPINT.zDIR = (stAXPINT.zDIR==zWIDE)? zTELE : zWIDE;
	LENS_SetDirectionZOOM(stAXPINT.zDIR);
	return stAXPINT.zDIR;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AZPINT_SetDriveSTEP(BYTE nID)
{
	stAXPINT.goal = LENS_GetCurPositionZOOM();

	if (stAXPINT.zDIR==zTELE)	stAXPINT.goal += stAXPINT.xCNT;
	else						stAXPINT.goal -= stAXPINT.xCNT;

	stAXPINT.goal = MIN(stAXPINT.goal, stAXPINT.zMAX);	// check range
	stAXPINT.goal = MAX(stAXPINT.goal, stAXPINT.zMIN);

	LENS_SetDriveZOOM(stAXPINT.goal);	// start motor drive
}

//--------------------------------------------------------------------------------------------------------------------------
static void AZPINT_SetDrivePEAK(WORD val)
{
	stAXPINT.goal = val;

	stAXPINT.goal = MIN(stAXPINT.goal, stAXPINT.zMAX);	// check range
	stAXPINT.goal = MAX(stAXPINT.goal, stAXPINT.zMIN);

	LENS_SetDriveZOOM(stAXPINT.goal);	// start motor drive
}

//--------------------------------------------------------------------------------------------------------------------------
static void AZPINT_GetJudgeINFO(void)
{
	if (stAXPINT.FLTn<stAXPINT.FLTx) return;

	stAXPINT.FLTx = stAXPINT.FLTn;
	stAXPINT.peak = stAXPINT.zVAL;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AZPINT_SetDrvDirBWD(BYTE nID)
{
	AZPINT_SetChangeDIR();	stAXPINT.zCUR = INV_SWING_SCAN;

	stAXPINT.peak = MIN(stAXPINT.peak, stAXPINT.zMAX);	// check range
	stAXPINT.peak = MAX(stAXPINT.peak, stAXPINT.zMIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AZPINT_SetDrvDirFWD(BYTE nID)
{
	BOOL end = AZPINT_IsDriveEnd();

	if (end==TRUE)	AZPINT_SetDrvDirBWD(nID);
	else			AZPINT_SetDriveSTEP(nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AZPINT_PreFilterSEL(void)
{
	if (stAXPINT.zCUR!=PRE_FILTER_SEL) return;

	stAXPINT.FLTx = 0;	stAXPINT.YREF = 0;	
	
	stAXPINT.peak = LENS_GetCurPositionZOOM();
	stAXPINT.mMIN = LENS_GetMinPositionZOOM();
	stAXPINT.mMAX = LENS_GetMaxPositionZOOM();

	stAXPINT.xCNT = LENS_GetPINTDrvSTEP(zPINT);
	stAXPINT.xGAP = LENS_GetPINTDrvRANGE(zPINT);

	stAXPINT.zMIN = MAX(stAXPINT.zVAL-stAXPINT.xGAP,stAXPINT.mMIN);
	stAXPINT.zMAX = MIN(stAXPINT.zVAL+stAXPINT.xGAP,stAXPINT.mMAX);

	stAXPINT.zCUR = END_FILTER_SEL;
	LENS_SetDirectionZOOM(zWIDE);	// fix WIDE direction
	LENS_EnableMotorZOOM(ON);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AZPINT_EndFilterSEL(void)
{
	if (stAXPINT.zCUR!=END_FILTER_SEL) return;
	stAXPINT.zCUR = PRE_SWING_MOVE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AZPINT_PreSwingMOVE(void)
{
	if (stAXPINT.zCUR!=PRE_SWING_MOVE) return;
	if (LENS_IsDriveCompleteZOOM()==FALSE) return;

	LENS_SetDriveZOOM(stAXPINT.zMIN);

	if (AZPINT_IsDriveEnd()==FALSE) return;
	stAXPINT.zCUR = END_SWING_MOVE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AZPINT_EndSwingMOVE(void)
{
	if (stAXPINT.zCUR!=END_SWING_MOVE) return;
	if (LENS_IsDriveCompleteZOOM()==FALSE) return;

	AZPINT_SetChangeDIR();
	stAXPINT.zCUR = PRE_SWING_SCAN;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AZPINT_PreSwingSCAN(void)
{
	if (stAXPINT.zCUR!=PRE_SWING_SCAN) return;
	if (LENS_IsDriveCompleteZOOM()==FALSE) return;
	
	AZPINT_GetJudgeINFO();

	stAXPINT.zCUR = DRV_SWING_SCAN;
	AZPINT_SetDriveSTEP(DRV_SWING_SCAN);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AZPINT_DrvSwingSCAN(void)
{
	if (stAXPINT.zCUR!=DRV_SWING_SCAN) return;
	if (LENS_IsDriveCompleteZOOM()==FALSE) return;

	AZPINT_GetJudgeINFO();
	AZPINT_SetDrvDirFWD(DRV_SWING_SCAN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AZPINT_EndSwingSCAN(void)
{
	if (stAXPINT.zCUR!=END_SWING_SCAN) return;
	if (LENS_IsDriveCompleteZOOM()==FALSE) return;

	stAXPINT.zCUR = END_FINAL_EXIT;
	AZPINT_SetDrivePEAK(stAXPINT.peak);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AZPINT_EndClimbPEAK(void)
{
	if (stAXPINT.zCUR!=END_FINAL_EXIT) return;
	if (LENS_IsDriveCompleteZOOM()==FALSE) return;

	stAXPINT.zCUR = END_CLEAR_PROC;	// stop progress
	LENS_EnableMotorZOOM(OFF);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AZPINT_EndDriveWAIT(void)
{
	if (LENS_IsDriveCompleteZOOM()==FALSE) return;

	switch (stAXPINT.zCUR) {
		case INV_SWING_MOVE: stAXPINT.zCUR = END_SWING_MOVE; break;
		case INV_SWING_SCAN: stAXPINT.zCUR = END_SWING_SCAN; break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AZPINT_ProcessHandler(void)
{
	if (stAXPINT.zCUR==0) return;
	stAXPINT.zPRV = stAXPINT.zCUR;	// for debug print

	AZPINT_PreFilterSEL();		// select filter
	AZPINT_PreSwingMOVE();		// moving bgn-scan
	AZPINT_PreSwingSCAN();		// search all-scan

	AZPINT_DrvSwingSCAN();		// drive all-scan

	AZPINT_EndFilterSEL();		// arrange filter
	AZPINT_EndSwingMOVE();		// arrange bgn-scan
	AZPINT_EndSwingSCAN();		// arrange all-scan

	AZPINT_EndClimbPEAK();		// drive peak-point
	AZPINT_EndDriveWAIT();		// wait drive-stop
}

//--------------------------------------------------------------------------------------------------------------------------
static void AXPINT_DBGPrint(void)
{
/*
	if (stAXPINT.time==0) {
		UARTprintf("\r\nAX PROCESS TRACE...\r\n");
		UARTprintf(" No   FLTo     FLTn     YSUM     FLTx   xPeak  fVAL F PR|CU  zVAL Z PR|CU\r\n");
//				    000 00000000 00000000 00000000 00000000 00000 00000 0 00000 00000 0 00000
	}
	else {
		UARTprintf("%03d ",		stAXPINT.time);
		UARTprintf("%08d ",		stAXPINT.FLTo);
		UARTprintf("%08d ",		stAXPINT.FLTn);
		UARTprintf("%08d ",		stAXPINT.YSUM);
		UARTprintf("%08d ",		stAXPINT.FLTx);
		UARTprintf("%05d ",		stAXPINT.peak);
		UARTprintf("%05d ",		stAXPINT.fVAL);
		UARTprintf("%01d ",		stAXPINT.fDIR);
		UARTprintf("%02X|%02X ",stAXPINT.fPRV,stAXPINT.fCUR);
		UARTprintf("%05d ",		stAXPINT.zVAL);
		UARTprintf("%01d ",		stAXPINT.zDIR);
		UARTprintf("%02X|%02X ",stAXPINT.zPRV,stAXPINT.zCUR);
		UARTprintf("\r\n");
	}
	stAXPINT.time++;

	if (stAXPINT.fCHK && stAXPINT.fCUR==END_CLEAR_PROC) {
		OSD_StrDispAttb(1,0,ON, OSD_COLOR, " PRV-VALUE: %05d               ", stAXPINT.fTOP);
		OSD_StrDispAttb(2,0,ON, OSD_COLOR, " CUR-VALUE: %05d               ", stAXPINT.fVAL);

		UARTprintf("[PREVIOUSLY CALIBRATED VALUE] ==============> %05d\r\n\r\n", stAXPINT.fTOP);
//				    000 00000000 00000000 00000000 00000000 00000 00000
		stAXPINT.fTOP = stAXPINT.fVAL;
	}
*/
	if (!stAXPINT.fCHK && stAXPINT.zCUR==END_CLEAR_PROC) {
		UARTprintf("\r\n[PRV VALUE]  %05d", stAXPINT.fTOP);
		UARTprintf("\r\n[CUR VALUE]  %05d", stAXPINT.fVAL);

		//OSD_StrDispAttb(1,0,ON, OSD_COLOR, " PRV-VALUE: %05d               ", stAXPINT.fTOP);
		//OSD_StrDispAttb(2,0,ON, OSD_COLOR, " CUR-VALUE: %05d               ", stAXPINT.fVAL);
		OSD_StrDispAttb(3,0,ON, OSD_COLOR, ">>> START(next)                 ");
	}

	if (stAXPINT.fCHK && stAXPINT.fCUR==END_CLEAR_PROC) {
		UARTprintf("\r\n[PRV VALUE]  %05d", stAXPINT.fTOP);
		UARTprintf("\r\n[CUR VALUE]  %05d", stAXPINT.fVAL);

		if (ABS(stAXPINT.fVAL-stAXPINT.fTOP)>2) {
			UARTprintf(" ==> Please Re-Try!\r\n");
			if(pintMode == AFCTRL_PINTMAXF_MODE)
				OSD_StrDispAttb(3,0,ON, OSD_COLOR, ">>> SETUP(next) | INFO(exit)    ");
			else if(pintMode == AFCTRL_PINTTELE_MODE)
				OSD_StrDispAttb(3,0,ON, OSD_COLOR, ">>> START(next) | SETUP(retry)  ");
			else
				OSD_StrDispAttb(3,0,ON, OSD_COLOR, ">>> INFO(save) | END(retry)     ");
		} else {
			UARTprintf(" ==> OK!! Good Job!\r\n");
			fSysDRVModePINT++;
			if(pintMode == AFCTRL_PINTMAXF_MODE)
				OSD_StrDispAttb(3,0,ON, OSD_COLOR, ">>> END(next)                   ");
			else if(pintMode == AFCTRL_PINTTELE_MODE)
				OSD_StrDispAttb(3,0,ON, OSD_COLOR, ">>> START(next)                 ");
			else
				OSD_StrDispAttb(3,0,ON, OSD_COLOR, ">>> INFO(save)                  ");
		}

		if(stAXPINT.fTOP != 0)
			OSD_StrDispAttb(1,0,ON, OSD_COLOR, " PRV-VALUE: %05d               ", stAXPINT.fTOP);
		OSD_StrDispAttb(2,0,ON, OSD_COLOR, " CUR-VALUE: %05d               ", stAXPINT.fVAL);
		stAXPINT.fTOP = stAXPINT.fVAL;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void AX_ProcessHandler(PAF_CTRL_PARAM pCTRL)
{
	if (stAXPINT.fCUR==0 && stAXPINT.zCUR==0) return;

	AXPINT_GetAXOPDWIND(pCTRL);		// get AXOPD data
	AFPINT_ProcessHandler();
	AZPINT_ProcessHandler();
	AXPINT_DBGPrint();
}

//--------------------------------------------------------------------------------------------------------------------------
void AX_EnableProcess(BOOL OnOff)
{
	if (fSysDRVModePINT==0) return;
	if (stAXPINT.fCUR || stAXPINT.zCUR) return;

	LENS_EnableTrackZOOM(OFF);	// disable zoom tracking

	if (stAXPINT.fCHK)	stAXPINT.fCUR = PRE_FILTER_SEL;
	else				stAXPINT.zCUR = PRE_FILTER_SEL;

	if (stAXPINT.fCHK)	UARTprintf("\r\n[PINT] Drvie MAX-Position FOKS!!!\r\n");
	else				UARTprintf("\r\n[PINT] Drive MAX-Position ZOOM!!!\r\n");

	if (stAXPINT.fCHK) OSD_StrDispAttb(0,0,ON, OSD_COLOR, "PINT PROGRESS-FOKS              ");
	else OSD_StrDispAttb(0,0,ON, OSD_COLOR, "PINT PROGRESS-ZOOM              ");

	stAXPINT.time = 0;	// for debug print

	OSD_StrDispAttb(3,0,ON, OSD_COLOR, "                                ");

}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AX_IsProcessExecuting(void)
{
	return (stAXPINT.fCUR || stAXPINT.zCUR)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void PINT_PreProcControl(BOOL dir)
{
	SetMsgCommandRepeatKey(0);		// set repeater MsgFlag
	SetMsgCommandDriveLENS(1);		// set drvMotor-goal flag
	LENS_EnableMotorZOOM(ON);		// enable zoom motor channel
	LENS_EnableMotorFOKS(ON);		// enable foks motor channel

	LENS_SetDirectionZOOM(dir);	LENS_SetDirectionFOKS(dir);

	zGoal = LENS_GetCurPositionZOOM()+((dir==zTELE)? stAXPINT.zTGT : -stAXPINT.zTGT);
	fGoal = LENS_GetCurPositionFOKS()+((dir==zTELE)? stAXPINT.fTGT : -stAXPINT.fTGT); //mimitest


	if (dir==zTELE)	UARTprintf("\r\n[PINT] Drive TELE-Position RUN!!!\r\n");
	else			UARTprintf("\r\n[PINT] Drive WIDE-Position RUN!!!\r\n");

	if (dir==zTELE)	OSD_StrDispAttb(0,0,ON, OSD_COLOR, "PINT PROGRESS-TELE              ");
	else				OSD_StrDispAttb(0,0,ON, OSD_COLOR, "PINT PROGRESS-WIDE              ");

	OSD_StrDispAttb(1,0,ON, OSD_COLOR, "                                ");
	OSD_StrDispAttb(2,0,ON, OSD_COLOR, "                                ");
	OSD_StrDispAttb(3,0,ON, OSD_COLOR, ">>> SETUP(next) | INFO(exit)    ");
}

//--------------------------------------------------------------------------------------------------------------------------
static void PINT_ProcessControl(BOOL dir)
{
	LENS_EnableTrackZOOM(OFF);		// disable zoom tracking
	LENS_SetDriveZOOM(zGoal);		// start motor drive
	LENS_SetDriveFOKS(fGoal);
	SetMsgCommandComplete(ON);
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDirDrivePINT(BOOL dir)
{
	if (fSysDRVModePINT==0) return;

	if(dir ==zTELE) pintMode = AFCTRL_PINTTELE_MODE;
	else pintMode = AFCTRL_PINTWIDE_MODE;

	stAXPINT.fCHK = (dir==zTELE)? OFF : ON;	// AF or AZ

	if (IsMsgPreExecCommand())	PINT_PreProcControl(dir);
	else						PINT_ProcessControl(dir);
}

//--------------------------------------------------------------------------------------------------------------------------
static void PINT_ProcessBGN(void)
{
	UARTprintf("\r\n[PINT] Find MECHA-Position RUN!!!\r\n");

	PINT_DisplayCLEAR();
	OSD_StrDispAttb(0,0,ON, OSD_COLOR, "PINT PROGRESS-MECH              "); // 19 char
	OSD_StrDispOnOff(ON);

	memset(&stAXPINT, 0, sizeof(AX_PINT_PARAM));

	fIsrDRVLensSTOP = ON;	WAIT_ODMVsyncPulse();

	LENS_ResetPosition();	WAIT_ODMVsyncPulse();
	LENS_LimitPosition();	WAIT_ODMVsyncPulse();
	LENS_CurvePosition();	WAIT_ODMVsyncPulse();

	fIsrDRVLensSTOP = OFF;	WAIT_ODMVsyncPulse();
	stAXPINT.fCHK = ON;	LENS_SetMechanicMODE(ON);

	UARTprintf("[PINT] Drive START-Position END!!!\r\n");
	OSD_StrDispAttb(3,0,ON, OSD_COLOR, ">>> SETUP(next) | INFO(exit)    ");
}

//--------------------------------------------------------------------------------------------------------------------------
static void PINT_ProcessEND(void)
{
	if(fSysDRVModePINT > 0) {
		LENS_SetMechanicMODE(OFF);
		if(fSysDRVModePINT > 1) {
			LENS_SetOpticalPINT();
			//OSD_StrDispAttb(0,0,ON, OSD_COLOR, "PINT PROGRESS-DONE              ");
			UARTprintf("PINT PROGRESS-DONE\r\n");
		}
	}
	SetDrvOffPINT();
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDrvOnOffPINT(void)
{
	if (GetAgeOnOffLENS()) return;
	else {
		if (fSysDRVModePINT==0) {
			pintMode = AFCTRL_PINTMAXF_MODE;
			PINT_ProcessBGN();
			fSysDRVModePINT = 1;
		} else {
			pintMode = AFCTRL_PINTDONE_MODE;
			PINT_ProcessEND();
		}
	}
}

void SetDrvOffPINT(void)
{
	fSysDRVModePINT = 0;
	pintMode = AFCTRL_PINTDONE_MODE;
	OSD_StrDispAttb(0,0,ON, OSD_COLOR, "                                ");
	OSD_StrDispAttb(1,0,ON, OSD_COLOR, "                                ");
	OSD_StrDispAttb(2,0,ON, OSD_COLOR, "                                ");
	OSD_StrDispAttb(3,0,ON, OSD_COLOR, "                                ");
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL GetDrvOnOffPINT(void)
{
	return (fSysDRVModePINT>0)? ON : OFF;
}
#endif

/*  FILE_END_HERE */
