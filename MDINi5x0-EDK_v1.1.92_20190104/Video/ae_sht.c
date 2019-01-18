// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

typedef	struct
{
	LONG	Ysum;	
	LONG	Ymin;	

	LONG	gain;	
	LONG	goal;	
	LONG	step;	

	LONG	min;	
	LONG	max;
	LONG	omin;

	BYTE	proc;	
	BYTE	roll;	
	CHAR	frmID;
	CHAR	cntID;

	LONG	nBUFF[8];

	LONG	divRatio;

}	AE_SHUT_PARAM, *PAE_SHUT_PARAM;

#define		AESHUT_STABLE_PROC		0x00	
#define		AESHUT_OVERH0_PROC		0x10	
#define		AESHUT_OVERH1_PROC		0x11	
#define		AESHUT_OVERH2_PROC		0x12	
#define		AESHUT_OVERL0_PROC		0x20	
#define		AESHUT_OVERL1_PROC		0x21	
#define		AESHUT_OVERL2_PROC		0x22	

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static volatile AE_SHUT_PARAM stSHUT;

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
static void AECTRL_SHUTCtrlDefineProc(PAE_CTRL_PARAM pCTRL)
{
	stSHUT.proc = AESHUT_STABLE_PROC;

	if (stSHUT.Ysum>(pCTRL->Yref+(pCTRL->band*2))) // 1+ 700*1
		 stSHUT.proc = AESHUT_OVERH0_PROC;
	if (stSHUT.Ysum<(pCTRL->Yref-(pCTRL->band*2))) //0+ 700*1
		 stSHUT.proc = AESHUT_OVERL0_PROC;

	if (stSHUT.Ysum>(pCTRL->Yref+(pCTRL->band*4))) // 700*1
		 stSHUT.proc = AESHUT_OVERH1_PROC;
	if (stSHUT.Ysum<(pCTRL->Yref-(pCTRL->band*4))) // 700*1
		 stSHUT.proc = AESHUT_OVERL1_PROC;

	if (stSHUT.Ysum>(pCTRL->Yref+(pCTRL->band*8))) // 700*1
		 stSHUT.proc = AESHUT_OVERH2_PROC;
	if (stSHUT.Ysum<(pCTRL->Yref-(pCTRL->band*8))) // 700*1
		 stSHUT.proc = AESHUT_OVERL2_PROC;

	// for prevent rolling when very-fast speed 
	if (stSHUT.roll) stSHUT.proc = AESHUT_STABLE_PROC;

	stSHUT.Ymin = ((1920*1080)/(256*20))+(pCTRL->band*8);
	if (stSHUT.proc>=AESHUT_OVERL0_PROC && pCTRL->Yref<stSHUT.Ymin) {
		 stSHUT.proc = AESHUT_OVERL2_PROC;
		//UARTprintf("2\n"); 
	}

	FLOAT ratio;
	ratio = ((FLOAT)stSHUT.gain/(FLOAT)stSHUT.max)*64; // 0 ~ 64;
	stSHUT.divRatio = 8 - MIN((BYTE)ratio, 7); // 1~8
	//UARTprintf("---------------------------------------------------------------gain : %d / ratio : %f / stSHUT.divRatio : %d\r\n", stSHUT.gain, ratio, stSHUT.divRatio);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_SHUTCtrlOverH2Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stSHUT.proc!=AESHUT_OVERH2_PROC) return;

	stSHUT.frmID = 3;

	stSHUT.goal = (LONG)((FLOAT)pCTRL->Yref/stSHUT.Ysum*stSHUT.gain);
	stSHUT.goal = MIN(MAX(stSHUT.goal, stSHUT.omin), stSHUT.max);
	stSHUT.step = MAX(DIFF(stSHUT.gain,stSHUT.goal)/2, 1);

	//UARTprintf("GOAL:%5d ", stSHUT.goal);
	//UARTprintf("STEP:%5d ", stSHUT.step);
	stSHUT.gain -= DIFF(stSHUT.gain,stSHUT.goal)/(4*stSHUT.divRatio);
	//UARTprintf("GAIN:%5d ", stSHUT.gain);
	stSHUT.gain  = MAX(stSHUT.gain, stSHUT.goal);
	//UARTprintf("GAIN:%5d ", stSHUT.gain);
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01 || pCTRL==NULL) return;
	UARTprintf("[SHUT-OVERH2] GAIN:%5d GOAL:%5d STEP:%05d\r\n", stSHUT.gain, stSHUT.goal, stSHUT.step);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_SHUTCtrlOverH1Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stSHUT.proc!=AESHUT_OVERH1_PROC) return;

	stSHUT.frmID = 3;

	stSHUT.goal = (LONG)((FLOAT)pCTRL->Yref/stSHUT.Ysum*stSHUT.gain);
	stSHUT.goal = MIN(MAX(stSHUT.goal, stSHUT.omin), stSHUT.max);
	stSHUT.step = MIN(MAX(DIFF(stSHUT.gain,stSHUT.goal)/4,1),2);
	//UARTprintf("\r\nGOAL:%5d ", stSHUT.goal);
	//UARTprintf("STEP:%5d ", stSHUT.step);

	stSHUT.gain -= (LONG)(stSHUT.step*((FLOAT)stSHUT.gain/stSHUT.min)/(2*stSHUT.divRatio));
	//UARTprintf("GAIN:%5d ", stSHUT.gain);
	stSHUT.gain  = MAX(stSHUT.gain, stSHUT.goal);
	//UARTprintf("GAIN:%5d ", stSHUT.gain);
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01 || pCTRL==NULL) return;
	UARTprintf("[SHUT-OVERH1] GAIN:%5d GOAL:%5d STEP:%05d\r\n", stSHUT.gain, stSHUT.goal, stSHUT.step);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_SHUTCtrlOverH0Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stSHUT.proc!=AESHUT_OVERH0_PROC) return;

	stSHUT.frmID = 3;

	stSHUT.goal = (LONG)((FLOAT)pCTRL->Yref/stSHUT.Ysum*stSHUT.gain);
	stSHUT.goal = MIN(MAX(stSHUT.goal, stSHUT.omin), stSHUT.max);
	stSHUT.step = MIN(MAX(DIFF(stSHUT.gain,stSHUT.goal)/8,1),1);
	//UARTprintf("GOAL:%5d ", stSHUT.goal);
	//UARTprintf("STEP:%5d ", stSHUT.step);

	stSHUT.gain -= (LONG)(stSHUT.step*((FLOAT)stSHUT.gain/stSHUT.min)/(2*stSHUT.divRatio));
	//UARTprintf("GAIN:%5d ", stSHUT.gain);
	stSHUT.gain  = MAX(stSHUT.gain, stSHUT.goal);
	//UARTprintf("GAIN:%5d ", stSHUT.gain);
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01 || pCTRL==NULL) return;
	UARTprintf("[SHUT-OVERH0] GAIN:%5d GOAL:%5d STEP:%05d\r\n", stSHUT.gain, stSHUT.goal, stSHUT.step);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_SHUTCtrlOverL2Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stSHUT.proc!=AESHUT_OVERL2_PROC) return;

	stSHUT.frmID = 3;

	stSHUT.goal = (LONG)((FLOAT)pCTRL->Yref/stSHUT.Ysum*stSHUT.gain);
	stSHUT.goal = MIN(MAX(stSHUT.goal, stSHUT.omin), stSHUT.max*2);
	stSHUT.step = MAX(DIFF(stSHUT.gain,stSHUT.goal)/2, 1);
	//UARTprintf("GOAL:%5d ", stSHUT.goal);
	//UARTprintf("STEP:%5d ", stSHUT.step);

	stSHUT.gain += DIFF(stSHUT.gain,stSHUT.goal)/(4*stSHUT.divRatio);
	//UARTprintf("GAIN:%5d ", stSHUT.gain);
	stSHUT.gain  = MIN(stSHUT.gain, stSHUT.goal);
	//UARTprintf("GAIN:%5d ", stSHUT.gain);
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01 || pCTRL==NULL) return;
	UARTprintf("[SHUT-OVERL2] GAIN:%5d GOAL:%5d STEP:%05d\r\n", stSHUT.gain, stSHUT.goal, stSHUT.step);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_SHUTCtrlOverL1Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stSHUT.proc!=AESHUT_OVERL1_PROC) return;

	stSHUT.frmID = 3;

	stSHUT.goal = (LONG)((FLOAT)pCTRL->Yref/stSHUT.Ysum*stSHUT.gain);
	stSHUT.goal = MIN(MAX(stSHUT.goal, stSHUT.omin), stSHUT.max*2);
	stSHUT.step = MIN(MAX(DIFF(stSHUT.gain,stSHUT.goal)/4,1),2);
	//UARTprintf("\r\nGOAL:%5d ", stSHUT.goal);
	//UARTprintf("STEP:%5d ", stSHUT.step);

	stSHUT.gain += (LONG)(stSHUT.step*((FLOAT)stSHUT.gain/stSHUT.min)/(2*stSHUT.divRatio));
	//UARTprintf("GAIN:%5d ", stSHUT.gain);
	stSHUT.gain  = MIN(stSHUT.gain, stSHUT.goal);
	//UARTprintf("GAIN:%5d ", stSHUT.gain);
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01 || pCTRL==NULL) return;
	UARTprintf("[SHUT-OVERL1] GAIN:%5d GOAL:%5d STEP:%05d\r\n", stSHUT.gain, stSHUT.goal, stSHUT.step);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_SHUTCtrlOverL0Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stSHUT.proc!=AESHUT_OVERL0_PROC) return;

	stSHUT.frmID = 3;

	stSHUT.goal = (LONG)((FLOAT)pCTRL->Yref/stSHUT.Ysum*stSHUT.gain);
	stSHUT.goal = MIN(MAX(stSHUT.goal, stSHUT.omin), stSHUT.max*2);
	stSHUT.step = MIN(MAX(DIFF(stSHUT.gain,stSHUT.goal)/8,1),1);
	//UARTprintf("GOAL:%5d ", stSHUT.goal);
	//UARTprintf("STEP:%5d ", stSHUT.step);

	stSHUT.gain += (LONG)(stSHUT.step*((FLOAT)stSHUT.gain/stSHUT.min)/(2*stSHUT.divRatio));
	//UARTprintf("GAIN:%5d ", stSHUT.gain);
	stSHUT.gain  = MIN(stSHUT.gain, stSHUT.goal);
	//UARTprintf("GAIN:%5d ", stSHUT.gain);
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01 || pCTRL==NULL) return;
	UARTprintf("[SHUT-OVERL0] GAIN:%5d GOAL:%5d STEP:%05d\r\n", stSHUT.gain, stSHUT.goal, stSHUT.step);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_SHUTCtrlStableMode(PAE_CTRL_PARAM pCTRL)
{
	if (stSHUT.proc!=AESHUT_STABLE_PROC) return;

	stSHUT.frmID = 3;

	stSHUT.goal = (LONG)((FLOAT)pCTRL->Yref/stSHUT.Ysum*stSHUT.gain);
	stSHUT.goal = MIN(MAX(stSHUT.goal, stSHUT.omin), stSHUT.max*2);
	stSHUT.step = MAX(DIFF(stSHUT.gain,stSHUT.goal)/16, 0);
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01 || pCTRL==NULL) return;
	UARTprintf(".\r\n");
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_SHUTCtrlUpdateMode(PAE_CTRL_PARAM pCTRL)
{
	//if (stSHUT.proc==AESHUT_STABLE_PROC) return;
	stSHUT.gain = MIN(MAX(stSHUT.gain, stSHUT.omin), stSHUT.max);
	//UARTprintf("gain:%d/min:%d/max/%d\r\n",stSHUT.gain,stSHUT.omin,stSHUT.max);

	stAEXFER.shut = AECTRL_CnvSHUTtoCMOS(stSHUT.gain);
	//stSHUT.gain = AECTRL_CnvCMOStoSHUT(stAEXFER.shut);
	//UARTprintf("[SHUT-UPDATE] stAEXFER.shut : %d\r\n", stAEXFER.shut);
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01 || pCTRL==NULL) return;
	UARTprintf("stSHUT.gain : %d[%d ~ %d] / ", stSHUT.gain, stSHUT.omin, stSHUT.max);
	UARTprintf("[SHUT-UPDATE] stAEXFER.shut : %d\r\n", stAEXFER.shut);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SHUTHandler(PAE_CTRL_PARAM pCTRL)
{
	//UARTprintf("shut min max : %d %d  AECTRL_GetSHUTMin : %d AECTRL_GetSHUTOmax : %d\r\n", stSHUT.omin, stSHUT.max, AECTRL_GetSHUTMin(), AECTRL_GetSHUTOmax());
	//UARTprintf("AECTRL_SHUTHandler 0\r\n");
	if ((pCTRL->ctrl&AECTRL_SHUTCTL_MANU)!=AECTRL_SHUTCTL_AUTO) return;
	//UARTprintf("AECTRL_SHUTHandler 1\r\n");
	if ((pCTRL->ctrl&AECTRL_PROC_MASK)!=AECTRL_PROC_SHUT) return;
	//UARTprintf("AECTRL_SHUTHandler 2\r\n");

	if (stSHUT.cntID<MAX(stSHUT.frmID,3)) stSHUT.cntID++;
	if (stSHUT.cntID<MAX(stSHUT.frmID,3)) return;

	stSHUT.cntID = 2; stSHUT.Ysum = pCTRL->Ysum;

	AECTRL_SHUTCtrlDefineProc(pCTRL);
	AECTRL_SHUTCtrlOverH2Mode(pCTRL);
	AECTRL_SHUTCtrlOverH1Mode(pCTRL);
	AECTRL_SHUTCtrlOverH0Mode(pCTRL);
	AECTRL_SHUTCtrlOverL0Mode(pCTRL);
	AECTRL_SHUTCtrlOverL1Mode(pCTRL);
	AECTRL_SHUTCtrlOverL2Mode(pCTRL);
	AECTRL_SHUTCtrlStableMode(pCTRL);
	AECTRL_SHUTCtrlUpdateMode(pCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_ClearModeSHUT(void)
{
	stSHUT.cntID = 2;	stSHUT.frmID = 3;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AECTRL_IsSHUTCtrlMode(PAE_CTRL_PARAM pCTRL)
{
	if (pCTRL->ctrl&AECTRL_SHUTCTL_MANU) return FALSE;
#if 0
	return (stSHUT.gain<stSHUT.max)? TRUE : FALSE;
#else
	if((stSHUT.gain>stSHUT.omin) && (stSHUT.gain<stSHUT.max))	return TRUE;
	else														return FALSE;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AECTRL_IsSHUTCtrlGmin(void)
{
#if 0
	WORD wTmp;
	LONG LTmp;

	wTmp = (WORD)(stSHUT.max - stSHUT.omin);
	wTmp = wTmp/10 + (WORD)stSHUT.omin;
	LTmp = (LONG) wTmp;
	return (stSHUT.gain<LTmp)? TRUE : FALSE;
#else

	//UARTprintf("shut gain:%d/min:%d / shut gain:%d(%d:%d)\r\n",stSHUT.gain,stSHUT.omin, AECTRL_GetSHUTGain(),AECTRL_GetSHUTOmax(),AECTRL_GetSHUTMin());
	return (AECTRL_GetSHUTGain()>=(AECTRL_GetSHUTOmax()-1))? TRUE : FALSE;
	//return (stSHUT.gain<=stSHUT.omin)? TRUE : FALSE;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AECTRL_IsSHUTCtrlGmax(void)
{
#if 0
	WORD wTmp;
	LONG LTmp;

	wTmp = (WORD)(stSHUT.max - stSHUT.omin);
	//UARTprintf("max(%d) min(%d) gain(%d)\r\n", stSHUT.max, stSHUT.omin, stSHUT.gain);
	wTmp = wTmp*9/10 + (WORD)stSHUT.omin;
	LTmp = (LONG) wTmp;
	return (stSHUT.gain>LTmp)? TRUE : FALSE;
#else
	//UARTprintf("shut gain:%d/max:%d\r\n",stSHUT.gain,stSHUT.max);
	return (AECTRL_GetSHUTGain()<=(AECTRL_GetSHUTMin()+1))? TRUE : FALSE;
	//return (stSHUT.gain>=stSHUT.max)? TRUE : FALSE;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetSHUTRange(SHORT min, SHORT max)
{
	stSHUT.max = AECTRL_CnvCMOStoSHUT((LONG)min);
	stSHUT.min = AECTRL_CnvCMOStoSHUT((LONG)max);
	stSHUT.omin = AECTRL_CnvCMOStoSHUT((LONG)max);
	//UARTprintf("shut : (%d~%d), (%d~%d)\r\n", min, max, stSHUT.min, stSHUT.max);
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetSHUTMax(SHORT max)
{
	stSHUT.min = AECTRL_CnvCMOStoSHUT((LONG)max);
}

//--------------------------------------------------------------------------------------------------------------------------

void AECTRL_SetSHUTDefaultMax(void)
{
	stSHUT.omin = AECTRL_CnvCMOStoSHUT((LONG)stSHUT.max);
}

void AECTRL_SetSHUTOmax(SHORT omin)
{
	stSHUT.omin = AECTRL_CnvCMOStoSHUT((LONG)omin);
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetSHUTGain(SHORT gain)
{
	stSHUT.gain = AECTRL_CnvCMOStoSHUT((DWORD)gain);
	//UARTprintf("gain:%d / stSHUT.gain : %d / max:%d(%d) / min:%d(%d)\r\n", gain, stSHUT.gain,AECTRL_CnvCMOStoSHUT(AECTRL_GetSHUTMax()),AECTRL_GetSHUTMax(), AECTRL_CnvCMOStoSHUT(AECTRL_GetSHUTMin()),AECTRL_GetSHUTMin());
	AECTRL_SHUTCtrlUpdateMode(NULL);
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT AECTRL_GetSHUTGain(void)
{
	return (SHORT)AECTRL_CnvSHUTtoCMOS(stSHUT.gain);
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT AECTRL_GetSHUTMax(void) // 1123
{
	return (SHORT)AECTRL_CnvSHUTtoCMOS(stSHUT.min);
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT AECTRL_GetSHUTOmax(void)
{
	return (SHORT)AECTRL_CnvSHUTtoCMOS(stSHUT.omin);
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT AECTRL_GetSHUTMin(void) // 1
{
	return (SHORT)AECTRL_CnvSHUTtoCMOS(stSHUT.max);
}

/*  FILE_END _HERE */
