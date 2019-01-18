// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

typedef	struct
{
	LONG	RPFa;
	LONG	LPFa;
	LONG	VPFa;
	LONG	BPFa;
	LONG	HPFa;
	LONG	CNTa;

	LONG	RPFb;
	LONG	LPFb;
	LONG	VPFb;
	LONG	BPFb;
	LONG	HPFb;
	LONG	CNTb;

	LONG	FLToR;
	LONG	FLToB;
	LONG	FLToH;

	LONG	GAIN;
	LONG	YSUM;

}	AF_EXTRACT_FILT, *PAF_EXTRACT_FILT;

typedef	struct
{
	BYTE	cur;
	BYTE	prv;

	WORD	step;
	WORD	goal;
	WORD	fVal;
	WORD	fMax;
	WORD	fMin;
	WORD	zVal;
	WORD	time;

}	AF_EXTRACT_DATA, *PAF_EXTRACT_DATA;

typedef enum {
	PRE_fFIND_DATA = 0x10, DRV_fFIND_DATA, END_fFIND_DATA,
	PRE_fFIND_NEAR = 0x20, DRV_fFIND_NEAR, END_fFIND_NEAR, END_fCLIP_WAIT,
	PRE_fFIND_NEXT = 0x30, DRV_fFIND_NEXT, END_fFIND_NEXT,
	PRE_fEXIT_DATA = 0x40, DRV_fEXIT_DATA, END_fEXIT_DATA,
	ALL_fSTOP_DATA = 0x00

} 	AF_EXTRACT_STEP;

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static AF_EXTRACT_FILT stAFFILT;
static AF_EXTRACT_DATA stAFDATA;

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
static void AfDataExtractODMdata(PAF_CTRL_PARAM pCTRL)
{
	stAFFILT.RPFa = (pCTRL->stFILT.RPFa)>>0;
	stAFFILT.LPFa = (pCTRL->stFILT.LPFa)>>0;
	stAFFILT.VPFa = (pCTRL->stFILT.VPFa)>>0;
	stAFFILT.BPFa = (pCTRL->stFILT.BPFa)>>0;

	stAFFILT.RPFb = (pCTRL->stFILT.RPFb)<<2;
	stAFFILT.LPFb = (pCTRL->stFILT.LPFb)<<2;
	stAFFILT.VPFb = (pCTRL->stFILT.VPFb)<<2;
	stAFFILT.BPFb = (pCTRL->stFILT.BPFb)<<2;

	stAFFILT.HPFa = (pCTRL->stFILT.HPFa)>>0;
	stAFFILT.HPFb = (pCTRL->stFILT.HPFb)>>2;

	stAFFILT.CNTa = (pCTRL->stFILT.CNTa)>>0;
	stAFFILT.CNTb = (pCTRL->stFILT.CNTb)>>0;

	stAFFILT.FLToR = stAFFILT.RPFa;
	stAFFILT.FLToB = stAFFILT.BPFa+stAFFILT.VPFa;
	stAFFILT.FLToH = stAFFILT.HPFa+stAFFILT.VPFa;

	stAFFILT.YSUM = (pCTRL->stFILT.YSUM)>>0;
	stAFFILT.GAIN = (pCTRL->stFILT.GAIN)>>0; // AGC gain
}

//--------------------------------------------------------------------------------------------------------------------------
static WORD SetAFDriveTarget(BOOL dir)
{
	WORD fVal = LENS_GetCurPositionFOKS();

	if (dir==fNEAR)	stAFDATA.goal = stAFDATA.fMin;
	else			stAFDATA.goal = fVal+stAFDATA.step;
	
	stAFDATA.goal = MIN(stAFDATA.goal, stAFDATA.fMax);
	stAFDATA.goal = MAX(stAFDATA.goal, stAFDATA.fMin);

	LENS_SetDriveFOKS(stAFDATA.goal);
	return stAFDATA.goal;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL IsAFDriveEndPoint(void)
{
	stAFDATA.fVal = LENS_GetCurPositionFOKS();
	if (stAFDATA.fVal>=stAFDATA.fMax) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static WORD GetAFDriveStep(void)
{
	//return LENS_GetAFDriveSTEP(DRV_STEP_STD);
	return LENS_GetAFDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetAfMotorParamInitial(void)
{
	stAFDATA.zVal = LENS_GetCurPositionZOOM();
	stAFDATA.fVal = LENS_GetCurPositionFOKS();

	stAFDATA.fMin = LENS_GetMINPositionLOCI(stAFDATA.zVal);
	stAFDATA.fMax = LENS_GetMAXPositionLOCI(stAFDATA.zVal);

	LENS_SetDirectionFOKS(fNEAR);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AfDataExtractPrepare(void)
{
	if (stAFDATA.cur!=PRE_fFIND_DATA) return;

	SetAfMotorParamInitial();
	LENS_EnableMotorFOKS(ON);
	stAFDATA.cur = END_fFIND_DATA;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AfDataExtractComplete(void)
{
	if (stAFDATA.cur!=END_fFIND_DATA) return;

	stAFDATA.cur = PRE_fFIND_NEAR;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AfDataNEAREndPrepare(void)
{
	if (stAFDATA.cur!=PRE_fFIND_NEAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
	
	stAFDATA.cur = DRV_fFIND_NEAR;
	SetAFDriveTarget(fNEAR);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AfDataNEAREndDriving(void)
{
	if (stAFDATA.cur!=DRV_fFIND_NEAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	LENS_SetDrvTimeCount(0);
	stAFDATA.cur = END_fFIND_NEAR;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AfDataNEAREndComplete(void)
{
	if (stAFDATA.cur!=END_fFIND_NEAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
	if (LENS_GetDrvTimeCount()<2*GET_FPS) return;

	LENS_SetDrvTimeCount(0);
	LENS_SetDirectionFOKS(fFAR);
	stAFDATA.cur = END_fCLIP_WAIT;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AfDataNEAREndClipWait(void)
{
	if (stAFDATA.cur!=END_fCLIP_WAIT) return;
	if (LENS_GetDrvTimeCount()<2*GET_FPS) return;

	stAFDATA.time = 0;
	stAFDATA.cur = PRE_fFIND_NEXT;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AfDataFARNextPrepare(void)
{
	if (stAFDATA.cur!=PRE_fFIND_NEXT) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
	
	stAFDATA.step = GetAFDriveStep();

	if (IsAFDriveEndPoint()) stAFDATA.cur = END_fEXIT_DATA;
	else					 stAFDATA.cur = DRV_fFIND_NEXT;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AfDataFARNextDriving(void)
{
	if (stAFDATA.cur!=DRV_fFIND_NEXT) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	SetAFDriveTarget(fFAR);
	stAFDATA.cur = PRE_fFIND_NEXT;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AfDataFARNextComplete(void)
{
	if (stAFDATA.cur!=END_fFIND_NEXT) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFDATA.cur = PRE_fEXIT_DATA;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AfExitExtractPrepare(void)
{
	if (stAFDATA.cur!=PRE_fEXIT_DATA) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AfExitExtractComplete(void)
{
	if (stAFDATA.cur!=END_fEXIT_DATA) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	LENS_EnableMotorFOKS(OFF);
	SetMsgCommandLongExec(OFF);
	stAFDATA.cur = ALL_fSTOP_DATA;
}

//--------------------------------------------------------------------------------------------------------------------------
static void DBG_PrintCurveAF(void)
{
	if (stAFDATA.cur<PRE_fFIND_NEXT) return;

	if (stAFDATA.time==0) {
//		UARTprintf("\nAF PEAK CURVE...\r\n");
		UARTprintf(" No   RPFa     LPFa     VPFa     BPFa     HPFa    CNTa  ");
//				    000 00000000 00000000 00000000 00000000 00000000 000000 
		UARTprintf("  RPFb     LPFb     VPFb     BPFb     HPFb    CNTb     FLToR     FLToB     FLToH  ");
//				    00000000 00000000 00000000 00000000 00000000 000000 
		UARTprintf(" YSUM   GAIN  focus  zoom\r\n");
//					000000 000000 00000 00000
	}

	UARTprintf("%03d ", stAFDATA.time);
	UARTprintf("%08d ", stAFFILT.RPFa);
	UARTprintf("%08d ", stAFFILT.LPFa);
	UARTprintf("%08d ", stAFFILT.VPFa);
	UARTprintf("%08d ", stAFFILT.BPFa);
	UARTprintf("%08d ", stAFFILT.HPFa);
	UARTprintf("%06d ", stAFFILT.CNTa);

	UARTprintf("%08d ", stAFFILT.RPFb);
	UARTprintf("%08d ", stAFFILT.LPFb);
	UARTprintf("%08d ", stAFFILT.VPFb);
	UARTprintf("%08d ", stAFFILT.BPFb);
	UARTprintf("%08d ", stAFFILT.HPFb);
	UARTprintf("%06d ", stAFFILT.CNTb);

	UARTprintf("%08d ", stAFFILT.FLToR);
	UARTprintf("%08d ", stAFFILT.FLToB);
	UARTprintf("%08d ", stAFFILT.FLToH);

	UARTprintf("%06d ", stAFFILT.YSUM);
	UARTprintf("%06d ", stAFFILT.GAIN);
	UARTprintf("%05d ", LENS_GetCurPositionFOKS());
	UARTprintf("%05d ", LENS_GetCurPositionZOOM());
	UARTprintf("\r\n");

	stAFDATA.time++;
}

//--------------------------------------------------------------------------------------------------------------------------
void AF_ExtractHandler(PAF_CTRL_PARAM pCTRL)
{
	if (stAFDATA.cur==0) return;
	stAFDATA.prv = stAFDATA.cur;

	AfDataExtractODMdata(pCTRL);

	AfDataExtractPrepare();
	AfDataNEAREndPrepare();
	AfDataFARNextPrepare();
	AfExitExtractPrepare();

	AfDataNEAREndDriving();
	AfDataFARNextDriving();

	AfDataExtractComplete();
	AfDataNEAREndComplete();
	AfDataFARNextComplete();
	AfExitExtractComplete();

	AfDataNEAREndClipWait();
	DBG_PrintCurveAF();
}

//--------------------------------------------------------------------------------------------------------------------------
void SetAFExtractTrigger(void)
{
	SetMsgCommandLongExec(ON);
	SetMsgCommandDriveFOKS(0);
	stAFDATA.cur = PRE_fFIND_DATA;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AF_IsExtractExecuting(void)
{
	return (stAFDATA.cur==0)? FALSE : TRUE;
}
#endif

/*  FILE_END_HERE */
