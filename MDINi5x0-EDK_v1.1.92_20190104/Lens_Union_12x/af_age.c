// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"
#include	"menu.h"

#if defined(__USE_AFZOOM_LENS__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

#define 	__MISP100_AA_DBG__			1

typedef	struct
{
	LONG	xCNT;
	LONG	nCNT;
	LONG	YMAX;
	LONG	YMIN;

	SHORT	fMAX;
	SHORT	fMIN;
	SHORT	fCUR;
	SHORT	fPIN;
	LONG	fSUM;
	SHORT	fBUF[10];

	SHORT	zMAX;
	SHORT	zMIN;
	SHORT	zCUR;
	SHORT	zPIN;
	LONG	zSUM;
	SHORT	zBUF[10];

	BYTE	proc;
	BYTE	prev;
	SHORT	wait;

}	AA_LENS_PARAM, *PAA_LENS_PARAM;

typedef enum {
	DRV_MIN_PROC = 0x10, DRV_MAX_PROC = 0x20,
	DRV_PIN_PROC = 0x30, DRV_END_PROC = 0x40,
	DRV_ORG_PROC = 0x50, DRV_END_EXIT = 0x00,

	DRV_ERR_LENS = 0xe0, DRV_ERR_IRIS = 0xe1,
	DRV_ERR_YSUM = 0xe2

}	AA_PROCESS_STEP;

#define		WAIT_TIME		GET_FPS/5
#define		OSD_COLOR		MAKEBYTE(OSD_WHITE, OSD_NOCOLOR)


// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

static AA_LENS_PARAM stAALENS;
static BYTE fSysAGEModeLENS = OFF, OnOff = OFF;

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
static void AALENS_DisplayCLEAR(void)
{
	BYTE i;
	for (i=0; i<19; i++) OSD_StrDispAttb(i,0,ON, OSD_COLOR, "%36s", "");
}

//--------------------------------------------------------------------------------------------------------------------------
static LONG AALENS_GetYSUM(void)
{
	return Get_AE_Ysum();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AALENS_MoveIRIS(BOOL OnOff)
{
	if (OnOff==ON)	stAEXFER.iris = 0x000;
	else			stAEXFER.iris = 0x3ff;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AALENS_MoveIRCUT(BOOL iris, BOOL mode)
{
	if (iris==ON)	stAEXFER.iris = 0x000;
	else			stAEXFER.iris = 0x3ff;

	if (stAALENS.xCNT%10 && mode==0) return;

	OnOff = (mode)? OFF : (OnOff^ON);
	// OLPF_SetDrvTimeCount(0);

	// if (OnOff==ON)	{OLPF_DY(HIGH); OLPF_NT(LOW); }
	// else			{OLPF_DY(LOW);  OLPF_NT(HIGH);}
	SetDYNT_ICR(OnOff);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AALENS_MoveCntHandler(void)
{
	if (LO4BIT(stAALENS.proc)==1) return;
	if (stAALENS.prev==DRV_END_EXIT) return;

	if (LENS_IsDriveCompleteZOOM()==FALSE) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAALENS.xCNT<stAALENS.nCNT) return;
	stAALENS.proc = DRV_PIN_PROC;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE AALENS_GetStatus(LONG nID)
{
	BYTE i, x = (BYTE)((stAALENS.xCNT-1)%10);
	nID = (stAALENS.xCNT<10)? stAALENS.xCNT : 10;
	stAALENS.zBUF[x] = stAALENS.zCUR; for (i=0;i<10;i++) stAALENS.zSUM += stAALENS.zBUF[i];
	stAALENS.fBUF[x] = stAALENS.fCUR; for (i=0;i<10;i++) stAALENS.fSUM += stAALENS.fBUF[i];

	stAALENS.zCUR = (SHORT)(((FLOAT)stAALENS.zSUM)/nID+0.5);	stAALENS.zSUM = 0;
	stAALENS.fCUR = (SHORT)(((FLOAT)stAALENS.fSUM)/nID+0.5);	stAALENS.fSUM = 0;

	if (DIFF(stAALENS.zCUR,14000)>2 || DIFF(stAALENS.fCUR,24000)>4) {
		UARTprintf("\r\n[AGING] Z:%05d F:%05d IRIS:%06d %06d CNT:%d ==> LENS-RST: FAIL!!!\r\n",
					stAALENS.zCUR,stAALENS.fCUR,stAALENS.YMAX,stAALENS.YMIN,stAALENS.xCNT);

		OSD_StrDispAttb(0,0,ON, OSD_COLOR, "AGING LENS-RST:FAIL");
		stAALENS.prev = DRV_END_PROC;
	}

	if (stAALENS.prev!=DRV_END_PROC && (stAALENS.xCNT%100)==0) {
		UARTprintf("[AGING] Z:%05d F:%05d IRIS:%06d %04d CNT:%06d ==> SECTION: OK!!!\r\n",
					stAALENS.zCUR,stAALENS.fCUR,stAALENS.YMAX,stAALENS.YMIN,stAALENS.xCNT);
	}

	return stAALENS.prev;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AALENS_WaitOrgHandler(void)
{
	UARTprintf("30 ");

	if (stAALENS.wait==WAIT_TIME-1) stAALENS.YMAX = AALENS_GetYSUM();
	if (stAALENS.wait==WAIT_TIME-2) AALENS_MoveIRIS(OFF);

	if (stAALENS.wait) stAALENS.wait--;
	return (stAALENS.wait)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AALENS_MoveOrgHandler(void)
{
	if ((stAALENS.proc&0xf0)!=DRV_ORG_PROC) return;
	if (LENS_IsDriveCompleteZOOM()==FALSE) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
	if (AALENS_WaitOrgHandler()==TRUE) return;

	OSD_StrDispAttb(0,0,ON, OSD_COLOR, "AGING PROGRESS-ZERO");

	fIsrDRVLensSTOP = ON;	WAIT_ODMVsyncPulse();
	LENS_ZeroPosition();	WAIT_ODMVsyncPulse();
	fIsrDRVLensSTOP = OFF;	WAIT_ODMVsyncPulse();

	stAALENS.zCUR = LENS_GetCurPositionZOOM();
	stAALENS.fCUR = LENS_GetCurPositionFOKS();

	stAALENS.zCUR = stAALENS.zMIN+(stAALENS.zCUR-stAALENS.zMIN)/4;
	stAALENS.xCNT++;

	stAALENS.prev = AALENS_GetStatus(stAALENS.xCNT);
	AALENS_MoveIRCUT(0,0);	stAALENS.wait = WAIT_TIME;

	OSD_StrDispAttb(1,0,ON, OSD_COLOR, " EXEC CNT: %06d", stAALENS.xCNT);
	OSD_StrDispAttb(2,0,ON, OSD_COLOR, " ZOOM VAL: %06d", stAALENS.zCUR);
	OSD_StrDispAttb(3,0,ON, OSD_COLOR, " FOKS VAL: %06d", stAALENS.fCUR);
	OSD_StrDispAttb(4,0,ON, OSD_COLOR, " IRIS MAX: %06d", stAALENS.YMAX);
	OSD_StrDispAttb(5,0,ON, OSD_COLOR, " IRIS MIN: %06d", stAALENS.YMIN);

	LENS_SetCurPositionZOOM(14000);	LENS_SetCurPositionFOKS(24000);

	if (stAALENS.prev==DRV_END_PROC) stAALENS.proc = DRV_END_PROC;
	else							 stAALENS.proc = DRV_MAX_PROC;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AALENS_WaitMinHandler(void)
{
	if (stAALENS.wait==WAIT_TIME-1) stAALENS.YMIN = AALENS_GetYSUM();
	if (stAALENS.wait==WAIT_TIME-2) AALENS_MoveIRIS(ON);

	if (stAALENS.wait) stAALENS.wait--;
	return (stAALENS.wait)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AALENS_MoveMinHandler(void)
{
	if ((stAALENS.proc&0xf0)!=DRV_MIN_PROC) return;
	if (LENS_IsDriveCompleteZOOM()==FALSE) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
	if (AALENS_WaitMinHandler()==TRUE) return;

	stAALENS.zCUR = LENS_GetCurPositionZOOM();
	stAALENS.fCUR = LENS_GetCurPositionFOKS();

	if		(LO4BIT(stAALENS.proc)==0) {	stAALENS.proc |= 0x01;
		LENS_SetDirectionZOOM(zWIDE);		LENS_SetDirectionFOKS(fNEAR);
		UARTprintf("21 ");
	}

	else if (LO4BIT(stAALENS.proc)==1) {	stAALENS.proc &= 0xf0;
		LENS_SetDriveZOOM(stAALENS.zMIN);	LENS_SetDriveFOKS(stAALENS.fMIN);
		stAALENS.prev = DRV_MIN_PROC;		AALENS_MoveIRIS(ON);
		UARTprintf("22 ");
	}

	if (stAALENS.proc==DRV_MIN_PROC)		stAALENS.proc = DRV_ORG_PROC;

	if (stAALENS.proc!=DRV_ORG_PROC)		return;
	else									stAALENS.wait = WAIT_TIME;
	OSD_StrDispAttb(0,0,ON, OSD_COLOR, "AGING PROGRESS-WIDE");
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AALENS_WaitMaxHandler(void)
{
	if (stAALENS.wait) stAALENS.wait--;
	return (stAALENS.wait)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AALENS_MoveMaxHandler(void)
{
	if ((stAALENS.proc&0xf0)!=DRV_MAX_PROC) return;
	if (LENS_IsDriveCompleteZOOM()==FALSE) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
	if (AALENS_WaitMaxHandler()==TRUE) return;

	stAALENS.zCUR = LENS_GetCurPositionZOOM();
	stAALENS.fCUR = LENS_GetCurPositionFOKS();

	if		(LO4BIT(stAALENS.proc)==0) {	stAALENS.proc |= 0x01;
		LENS_SetDirectionZOOM(zTELE);		LENS_SetDirectionFOKS( fFAR);
		UARTprintf("11 ");
	}

	else if (LO4BIT(stAALENS.proc)==1) {	stAALENS.proc &= 0xf0;
		LENS_SetDriveZOOM(stAALENS.zMAX);	LENS_SetDriveFOKS(stAALENS.fMAX);
		stAALENS.prev = DRV_MAX_PROC;		AALENS_MoveIRIS(OFF);
		UARTprintf("12 ");
	}

	if (stAALENS.proc==DRV_MAX_PROC)		stAALENS.proc = DRV_MIN_PROC;

	if (stAALENS.proc!=DRV_MIN_PROC)		return;
	else									stAALENS.wait = WAIT_TIME;
	OSD_StrDispAttb(0,0,ON, OSD_COLOR, "AGING PROGRESS-TELE");

}

//--------------------------------------------------------------------------------------------------------------------------
static void AALENS_MovePinHandler(void)
{
	if ((stAALENS.proc&0xf0)!=DRV_PIN_PROC) return;
	if (LENS_IsDriveCompleteZOOM()==FALSE) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAALENS.zCUR = LENS_GetCurPositionZOOM();
	stAALENS.fCUR = LENS_GetCurPositionFOKS();

	if		(LO4BIT(stAALENS.proc)==0) {	stAALENS.proc |= 0x01;
		if (stAALENS.zCUR<stAALENS.zPIN)	LENS_SetDirectionZOOM(zTELE);
		else								LENS_SetDirectionZOOM(zWIDE);
		if (stAALENS.fCUR<stAALENS.fPIN)	LENS_SetDirectionFOKS( fFAR);
		else								LENS_SetDirectionFOKS(fNEAR);
	}

	else if (LO4BIT(stAALENS.proc)==1) {	stAALENS.proc &= 0xf0;
		LENS_SetDriveZOOM(stAALENS.zPIN);	LENS_SetDriveFOKS(stAALENS.fPIN);
		stAALENS.prev = DRV_END_EXIT;		AALENS_MoveIRCUT(1,1);
	}

	if (stAALENS.proc==DRV_PIN_PROC)		stAALENS.proc = DRV_END_PROC;

	if (stAALENS.proc!=DRV_END_PROC)		return;
	OSD_StrDispAttb(0,0,ON, OSD_COLOR, "AGING PROGRESS-PINT");

#if __MISP100_AA_DBG__ == 1
	UARTprintf("[AGING] DRV-PIN CNT:%06d\r\n",stAALENS.xCNT);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AALENS_MoveEndHandler(void)
{
	if ((stAALENS.proc&0xf0)!=DRV_END_PROC) return;
	if (LENS_IsDriveCompleteZOOM()==FALSE) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAALENS.zCUR = LENS_GetCurPositionZOOM();
	stAALENS.fCUR = LENS_GetCurPositionFOKS();

	stAECTRL.ctrl &= (~AECTRL_PROCESS_OFF);
	fSysAGEModeLENS = OFF;	stAALENS.proc = DRV_END_EXIT;

	LENS_EnableMotorZOOM(OFF);	LENS_EnableMotorFOKS(OFF);
	LENS_SetMechanicMODE(OFF);

	if (stAALENS.prev!=DRV_END_PROC)
		 OSD_StrDispAttb(0,0,ON, OSD_COLOR, "AGING COMPLETION!!!");

#if __MISP100_AA_DBG__ == 1
	UARTprintf("\r\n[AGING] Lens Aging EXIT!!!\r\n");
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void AA_ProcessHandler(PAF_CTRL_PARAM pCTRL)
{
	if (stAALENS.proc==DRV_END_EXIT) return;

	AALENS_MoveCntHandler();
	AALENS_MoveOrgHandler();
	AALENS_MoveMinHandler();
	AALENS_MoveMaxHandler();
	AALENS_MovePinHandler();
	AALENS_MoveEndHandler();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AGING_ProcessBGN(void)
{
	stAECTRL.ctrl |= ( AECTRL_PROCESS_OFF);
	memset(&stAALENS, 0, sizeof(AA_LENS_PARAM));

	LENS_EnableMotorZOOM(ON);	LENS_EnableMotorFOKS(ON);
	LENS_SetMechanicMODE(ON);	LENS_EnableTrackZOOM(OFF);

	stAALENS.zMIN = LENS_GetMinPositionZOOM();
	stAALENS.zMAX = LENS_GetMaxPositionZOOM();
	stAALENS.zPIN = LENS_GetPintPositionZOOM();

	stAALENS.fMIN = LENS_GetMinPositionFOKS();
	stAALENS.fMAX = LENS_GetMaxPositionFOKS();
	stAALENS.fPIN = LENS_GetPintPositionFOKS();

	stAALENS.nCNT = 500000;		stAALENS.proc = DRV_MAX_PROC;
	AALENS_DisplayCLEAR();
	OSD_StrDispAttb(0,0,ON, OSD_COLOR, "AGING PROGRESS-ZERO");
	OSD_StrDispAttb(1,0,ON, OSD_COLOR, " EXEC CNT: %06d", stAALENS.xCNT);
	OSD_StrDispAttb(2,0,ON, OSD_COLOR, " ZOOM VAL: %06d", stAALENS.xCNT);
	OSD_StrDispAttb(3,0,ON, OSD_COLOR, " FOKS VAL: %06d", stAALENS.xCNT);
	OSD_StrDispAttb(4,0,ON, OSD_COLOR, " IRIS MAX: %06d", stAALENS.xCNT);
	OSD_StrDispAttb(5,0,ON, OSD_COLOR, " IRIS MIN: %06d", stAALENS.xCNT);
	OSD_StrDispOnOff(ON);	WAIT_ODMVsyncPulse();

#if __MISP100_AA_DBG__ == 1
	UARTprintf("\r\n[AGING] Lens Aging RUN!!!\r\n");
#endif

	fIsrDRVLensSTOP = ON;	WAIT_ODMVsyncPulse();
	LENS_ZeroPosition();	WAIT_ODMVsyncPulse();
	fIsrDRVLensSTOP = OFF;	WAIT_ODMVsyncPulse();

	LENS_SetCurPositionZOOM(14000);
	LENS_SetCurPositionFOKS(24000);
	AALENS_MoveIRCUT(0,1);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AGING_ProcessEND(void)
{
	stAALENS.nCNT = stAALENS.xCNT;

#if __MISP100_AA_DBG__ == 1
	UARTprintf("\r\n[AGING] Lens Aging STOP!!!\r\n");
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetAgeOnOffLENS(void)
{
	if (GetDrvOnOffPINT()) return;
	else	fSysAGEModeLENS ^= ON;

	if (fSysAGEModeLENS==ON) AGING_ProcessBGN();
	else					 AGING_ProcessEND();
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL GetAgeOnOffLENS(void)
{
	return (stAALENS.proc==0)? FALSE : TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
void AA_ZOOMHandler(BOOL dir)
{
	if (LENS_IsDriveCompleteZOOM()==FALSE) return;

	fSysAGEModeLENS = ON;
	stAECTRL.ctrl |= ( AECTRL_PROCESS_OFF);		// AE OFF

	LENS_SetMechanicMODE(ON);	LENS_EnableTrackZOOM(OFF);
	LENS_EnableMotorZOOM(ON);	WAIT_ODMVsyncPulse();

	stAALENS.zCUR = LENS_GetCurPositionZOOM();
	stAALENS.zMAX = LENS_GetMaxPositionZOOM();
	stAALENS.zMIN = LENS_GetMinPositionZOOM();

	LENS_SetDirectionZOOM(dir);	WAIT_ODMVsyncPulse();

	if (dir==zTELE)	LENS_SetDriveZOOM(stAALENS.zMAX);
	else			LENS_SetDriveZOOM(stAALENS.zMIN);

	while (LENS_IsDriveCompleteZOOM()==FALSE);
	WAIT_ODMVsyncPulse();WAIT_ODMVsyncPulse();
}

//--------------------------------------------------------------------------------------------------------------------------
void AA_FOKSHandler(BOOL dir)
{
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	fSysAGEModeLENS = ON;
	stAECTRL.ctrl |= ( AECTRL_PROCESS_OFF);		// AE OFF

	LENS_SetMechanicMODE(ON);	LENS_EnableTrackZOOM(OFF);
	LENS_EnableMotorFOKS(ON);	WAIT_ODMVsyncPulse();

	stAALENS.fCUR = LENS_GetCurPositionFOKS();
	stAALENS.fMAX = LENS_GetMaxPositionFOKS();
	stAALENS.fMIN = LENS_GetMinPositionFOKS();

	LENS_SetDirectionFOKS(dir);	WAIT_ODMVsyncPulse();

	if (dir==fFAR)	LENS_SetDriveFOKS(stAALENS.fMAX);
	else			LENS_SetDriveFOKS(stAALENS.fMIN);

	while (LENS_IsDriveCompleteFOKS()==FALSE);
	WAIT_ODMVsyncPulse();WAIT_ODMVsyncPulse();
}

//--------------------------------------------------------------------------------------------------------------------------
void AA_IRISHandler(BOOL OnOff)
{
	if (OnOff==ON)	stAEXFER.iris = 0x000;	// open
	else			stAEXFER.iris = 0x3ff;	// close
}

//--------------------------------------------------------------------------------------------------------------------------
void AA_OLPFHandler(BOOL OnOff)
{
	// OLPF_SetDrvTimeCount(0);	// OLPF-cnt clear

	// if (OnOff==ON)	{OLPF_DY(LOW);  OLPF_NT(HIGH);}
	// else			{OLPF_DY(HIGH); OLPF_NT(LOW); }
	SetDYNT_ICR(OnOff);
}
#endif	/* defined(__USE_AFZOOM_LENS__) */

/*  FILE_END_HERE */
