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
	LONG	prev;	
	LONG	goal;	
	LONG	step;	
	LONG	diff;	

	LONG	min;	
	LONG	max;
	LONG	pmax;

	BYTE		proc;	
	CHAR	frmID;	
	CHAR	cntID;	

	BYTE		nIDX;	
	LONG	nBUFF[30];

}	AE_GAIN_PARAM, *PAE_GAIN_PARAM;

#define		AEGAIN_STABLE_PROC		0x00
#define		AEGAIN_OVERH0_PROC		0x10
#define		AEGAIN_OVERH1_PROC		0x11
#define		AEGAIN_OVERH2_PROC		0x12
#define		AEGAIN_OVERL0_PROC		0x20
#define		AEGAIN_OVERL1_PROC		0x21
#define		AEGAIN_OVERL2_PROC		0x22

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static volatile AE_GAIN_PARAM stGAIN;

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
BOOL gStopAgcRun = FALSE;
static DWORD fineOLD = 256;

//--------------------------------------------------------------------------------------------------------------------------
static LONG AECTRL_GAINCtrlYSUM(PAE_CTRL_PARAM pCTRL)
{
	LONG i, Ysum = 0;

	if (stGAIN.proc!=AEGAIN_STABLE_PROC || stGAIN.nIDX==0) return pCTRL->Ysum;

	stGAIN.nBUFF[stGAIN.nIDX-1] = pCTRL->Ysum;
	if (stGAIN.nIDX<30) return pCTRL->Ysum;

	for (i=0; i<stGAIN.nIDX; i++) Ysum += stGAIN.nBUFF[i];
	for (i=1; i<stGAIN.nIDX; i++) stGAIN.nBUFF[i-1] = stGAIN.nBUFF[i];

	//UARTprintf("						nIDX:%05d Y:%05d\r\n", stGAIN.nIDX, stGAIN.Ysum);
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01) return (Ysum/stGAIN.nIDX);;
	//UARTprintf("[AECTRL_GAINCtrlYSUM] :nIDX:%05d Y:%05d\r\n", stGAIN.nIDX, stGAIN.Ysum);
#endif
	return (Ysum/stGAIN.nIDX);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_GAINCtrlDefineProc(PAE_CTRL_PARAM pCTRL)
{
	stGAIN.proc = AEGAIN_STABLE_PROC;

	if (stGAIN.Ysum>(pCTRL->Yref+(pCTRL->band*9))) // 700*4
		 stGAIN.proc = AEGAIN_OVERH2_PROC;
	else if (stGAIN.Ysum<(pCTRL->Yref-(pCTRL->band*9))) // 700*4
		 stGAIN.proc = AEGAIN_OVERL2_PROC;
	else if (stGAIN.Ysum>(pCTRL->Yref+(pCTRL->band*6))) // 700*2
		 stGAIN.proc = AEGAIN_OVERH1_PROC;
	else if (stGAIN.Ysum<(pCTRL->Yref-(pCTRL->band*6))) // 700*2
		 stGAIN.proc = AEGAIN_OVERL1_PROC;
	else if (stGAIN.Ysum>(pCTRL->Yref+(pCTRL->band/2*3))) // 700*1
		 stGAIN.proc = AEGAIN_OVERH0_PROC;
	else if (stGAIN.Ysum<(pCTRL->Yref-(pCTRL->band/2*3))) // 700*1
		 stGAIN.proc = AEGAIN_OVERL0_PROC;

	stGAIN.Ymin = ((1920*1080)/(256*20))+(pCTRL->band*9); // 405 + 2800
	if (stGAIN.proc>=AEGAIN_OVERL0_PROC && pCTRL->Yref<stGAIN.Ymin)
		 stGAIN.proc = AEGAIN_OVERL2_PROC;
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01) return;
	//UARTprintf("DIFF:%4d ", stGAIN.Ysum - pCTRL->Yref);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_GAINCtrlOverH2Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stGAIN.proc!=AEGAIN_OVERH2_PROC) return;

	stGAIN.frmID = 3;	stGAIN.nIDX = 0;

	stGAIN.goal = (LONG)((FLOAT)pCTRL->Yref/stGAIN.Ysum*stGAIN.gain);
	//UARTprintf("H2 - Yref:%d / Ysum:%d / goal : %d\r\n",pCTRL->Yref,pCTRL->Ysum,stGAIN.goal); 
	stGAIN.goal = MIN(MAX(stGAIN.goal, stGAIN.min/2), stGAIN.max);
	stGAIN.step = MAX(DIFF(stGAIN.gain,stGAIN.goal)/pCTRL->stepDiv, 1);
	stGAIN.gain = MAX(stGAIN.gain-stGAIN.step,stGAIN.min);

#if defined(__USE_4K60_MD1__)
	FLOAT fineFLOAT;
	DWORD fineDWORD = AECTRL_CnvGAINtoFINE((DWORD)stGAIN.gain);

	if(fineDWORD > fineOLD)
		fineFLOAT = (FLOAT)fineDWORD / fineOLD;
	else
		fineFLOAT = (FLOAT)fineOLD / fineDWORD;

	if(fineFLOAT > 1.03)
		stGAIN.gain = MIN(stGAIN.gain+2*stGAIN.step/3,0x40000);
#endif

	if (stGAIN.gain<stGAIN.min) pCTRL->more = 1;

	//UARTprintf("H2 : goal(%5d) step(%5d) gain(%5d) : %5d\r\n", stGAIN.goal, stGAIN.step, stGAIN.gain, AECTRL_CnvGAINtoCMOS(stGAIN.gain));
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01) return;
	UARTprintf("[GAIN-OVERH2] GAIN:%5d STEP:%05d ", stGAIN.gain, stGAIN.step);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_GAINCtrlOverH1Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stGAIN.proc!=AEGAIN_OVERH1_PROC) return;

	stGAIN.frmID = 3;	stGAIN.nIDX = 0;

	stGAIN.goal = (LONG)((FLOAT)pCTRL->Yref/stGAIN.Ysum*stGAIN.gain);
	stGAIN.goal = MIN(MAX(stGAIN.goal, stGAIN.min/2), stGAIN.max);
	stGAIN.step = MAX(DIFF(stGAIN.gain,stGAIN.goal)/pCTRL->stepDiv, 1);
	stGAIN.gain = MAX(stGAIN.gain-stGAIN.step,stGAIN.min);

	//UARTprintf("H1 : goal(%5d) step(%5d) gain(%5d) : %5d\r\n", stGAIN.goal, stGAIN.step, stGAIN.gain, AECTRL_CnvGAINtoCMOS(stGAIN.gain));
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01) return;
	UARTprintf("[GAIN-OVERH1] GAIN:%5d STEP:%05d ", stGAIN.gain, stGAIN.step);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_GAINCtrlOverH0Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stGAIN.proc!=AEGAIN_OVERH0_PROC) return;

	stGAIN.frmID = 3;	stGAIN.nIDX = 0;

	stGAIN.goal = (LONG)((FLOAT)pCTRL->Yref/stGAIN.Ysum*stGAIN.gain);
	stGAIN.goal = MIN(MAX(stGAIN.goal, stGAIN.min/2), stGAIN.max);
	stGAIN.step = MAX(DIFF(stGAIN.gain,stGAIN.goal)/pCTRL->stepDiv, 1);
	stGAIN.gain = MAX(stGAIN.gain-stGAIN.step,stGAIN.min);

	//UARTprintf("H0 : goal(%5d) step(%5d) gain(%5d) : %5d\r\n", stGAIN.goal, stGAIN.step, stGAIN.gain, AECTRL_CnvGAINtoCMOS(stGAIN.gain));
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01) return;
	UARTprintf("[GAIN-OVERH0] GAIN:%5d STEP:%05d ", stGAIN.gain, stGAIN.step);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_GAINCtrlOverL2Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stGAIN.proc!=AEGAIN_OVERL2_PROC) return;

	stGAIN.frmID = 3;	stGAIN.nIDX = 0;

	if(gStopAgcRun) return;

	stGAIN.goal = (LONG)((FLOAT)pCTRL->Yref/stGAIN.Ysum*stGAIN.gain);
	//stGAIN.goal = (LONG)((FLOAT)stGAIN.Ysum/pCTRL->Yref*stGAIN.gain);
	//UARTprintf("L2 - Yref:%d / Ysum:%d / goal : %d\r\n",pCTRL->Yref,pCTRL->Ysum,stGAIN.goal); 
	stGAIN.goal = MIN(MAX(stGAIN.goal, stGAIN.min), stGAIN.gain*2);
	stGAIN.step = MAX(DIFF(stGAIN.gain,stGAIN.goal)/pCTRL->stepDiv, 1);
	stGAIN.gain = MIN(stGAIN.gain+stGAIN.step,stGAIN.max);


#if defined(__USE_4K60_MD1__)
	FLOAT fineFLOAT;
	DWORD fineDWORD = AECTRL_CnvGAINtoFINE((DWORD)stGAIN.gain);

	if(fineDWORD > fineOLD)
		fineFLOAT = (FLOAT)fineDWORD / fineOLD;
	else
		fineFLOAT = (FLOAT)fineOLD / fineDWORD;

	if(fineFLOAT > 1.03)
		stGAIN.gain = MAX(stGAIN.gain-2*stGAIN.step/3,0x20000);
#endif

	//UARTprintf("L2 : goal(%5d) step(%5d) gain(%5d) : %5d\r\n", stGAIN.goal, stGAIN.step, stGAIN.gain, AECTRL_CnvGAINtoCMOS(stGAIN.gain));
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01) return;
	UARTprintf("[GAIN-OVERL2] GAIN:%5d STEP:%05d ", stGAIN.gain, stGAIN.step);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_GAINCtrlOverL1Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stGAIN.proc!=AEGAIN_OVERL1_PROC) return;

	stGAIN.frmID = 3;	stGAIN.nIDX = 0;

	if(gStopAgcRun) return;

	stGAIN.goal = (LONG)((FLOAT)pCTRL->Yref/stGAIN.Ysum*stGAIN.gain);
	//stGAIN.goal = (LONG)((FLOAT)stGAIN.Ysum/pCTRL->Yref*stGAIN.gain);
	stGAIN.goal = MIN(MAX(stGAIN.goal, stGAIN.min), stGAIN.gain*2);
	stGAIN.step = MAX(DIFF(stGAIN.gain,stGAIN.goal)/pCTRL->stepDiv, 1);
	stGAIN.gain = MIN(stGAIN.gain+stGAIN.step,stGAIN.max);

	//UARTprintf("L1 : goal(%5d) step(%5d) gain(%5d) : %5d\r\n", stGAIN.goal, stGAIN.step, stGAIN.gain, AECTRL_CnvGAINtoCMOS(stGAIN.gain));
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01) return;
	UARTprintf("[GAIN-OVERL1] GAIN:%5d STEP:%05d ", stGAIN.gain, stGAIN.step);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_GAINCtrlOverL0Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stGAIN.proc!=AEGAIN_OVERL0_PROC) return;

	stGAIN.frmID = 3;	stGAIN.nIDX = 0;

	if(gStopAgcRun) return;

	stGAIN.goal = (LONG)((FLOAT)pCTRL->Yref/stGAIN.Ysum*stGAIN.gain);
	//stGAIN.goal = (LONG)((FLOAT)stGAIN.Ysum/pCTRL->Yref*stGAIN.gain);
	stGAIN.goal = MIN(MAX(stGAIN.goal, stGAIN.min), stGAIN.gain*2);
	stGAIN.step = MAX(DIFF(stGAIN.gain,stGAIN.goal)/pCTRL->stepDiv, 1);
	stGAIN.gain = MIN(stGAIN.gain+stGAIN.step,stGAIN.max);

	//UARTprintf("L0 : goal(%5d) step(%5d) gain(%5d) : %5d\r\n", stGAIN.goal, stGAIN.step, stGAIN.gain, AECTRL_CnvGAINtoCMOS(stGAIN.gain));
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01) return;
	UARTprintf("[GAIN-OVERL0] GAIN:%5d STEP:%05d ", stGAIN.gain, stGAIN.step);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_GAINCtrlStableMode(PAE_CTRL_PARAM pCTRL)
{
	if (stGAIN.proc!=AEGAIN_STABLE_PROC) return;

	stGAIN.frmID = 3;	if (stGAIN.nIDX<30) stGAIN.nIDX++;

	stGAIN.goal = (LONG)((FLOAT)pCTRL->Yref/stGAIN.Ysum*stGAIN.gain);
	stGAIN.step = MAX(DIFF(stGAIN.goal,stGAIN.gain)/pCTRL->stepDiv, 0);

	//UARTprintf("ST : goal(%5d) step(%5d) gain(%5d) : %5d\r\n", stGAIN.goal, stGAIN.step, stGAIN.gain, AECTRL_CnvGAINtoCMOS(stGAIN.gain));
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01) return;
	UARTprintf("[GAIN-STABLE] GAIN:%5d STEP:%05d ", stGAIN.gain, stGAIN.step);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_GAINCtrlUpdateMode(PAE_CTRL_PARAM pCTRL)
{
	stGAIN.gain = MIN(MAX(stGAIN.gain, stGAIN.min), stGAIN.max);
	stAEXFER.gain = AECTRL_CnvGAINtoCMOS((DWORD)stGAIN.gain); // cmos gain update
	stAEXFER.fine = AECTRL_CnvGAINtoFINE((DWORD)stGAIN.gain); // digital gain update

	fineOLD = stAEXFER.fine;
	//UARTprintf("stAEXFER.gain : %5d / stAEXFER.fine : %5d\r\n", stAEXFER.gain, stAEXFER.fine);
#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01 || pCTRL==NULL) return;
	UARTprintf(" / [Update] stAEXFER.gain : %04d / stAEXFER.fine : %06d\r\n", stAEXFER.gain, stAEXFER.fine);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_GAINCtrlUpdateModeDirect(PAE_CTRL_PARAM pCTRL)
{
	stAEXFER.gain = AECTRL_CnvGAINtoCMOS((DWORD)stGAIN.gain); // cmos gain update
	stAEXFER.fine = AECTRL_CnvGAINtoFINE((DWORD)stGAIN.gain); // digital gain update
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_GAINHandler(PAE_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&AECTRL_GAINCTL_MANU)!=AECTRL_GAINCTL_AUTO) return;
	if ((pCTRL->ctrl&AECTRL_PROC_MASK)!=AECTRL_PROC_GAIN) return;

	if (stGAIN.cntID<MAX(stGAIN.frmID,3)) stGAIN.cntID++;
	if (stGAIN.cntID<MAX(stGAIN.frmID,3)) return;

	stGAIN.cntID = 2; stGAIN.Ysum = AECTRL_GAINCtrlYSUM(pCTRL);

	if(stGAIN.gain >= 0x20000) {
		pCTRL->stepDiv = 16*pCTRL->agcDiv;
	} else if(stGAIN.gain >= 0x1b000) {
		pCTRL->stepDiv = 4*pCTRL->agcDiv;
	} else if(stGAIN.gain >= 0x18000) {
		pCTRL->stepDiv = 2*pCTRL->agcDiv;
	} else if(stGAIN.gain >= 0x10000) {
		pCTRL->stepDiv = 1*pCTRL->agcDiv;
	} else {
		pCTRL->stepDiv = pCTRL->agcDiv/2;
	}

#if defined(__USE_PWMIRIS_ONLY__)
	if(GetAELensMode() !=2)
		pCTRL->stepDiv *= MAX(3,MIN(DCLENS_getIrisSpeed()/32,1));
#endif
	//UARTprintf("pCTRL->stepDiv:%d\r\n",pCTRL->stepDiv);

	AECTRL_GAINCtrlDefineProc(pCTRL);
	AECTRL_GAINCtrlOverH2Mode(pCTRL);
	AECTRL_GAINCtrlOverH1Mode(pCTRL);
	AECTRL_GAINCtrlOverH0Mode(pCTRL);
	AECTRL_GAINCtrlOverL0Mode(pCTRL);
	AECTRL_GAINCtrlOverL1Mode(pCTRL);
	AECTRL_GAINCtrlOverL2Mode(pCTRL);
	AECTRL_GAINCtrlStableMode(pCTRL);
	AECTRL_GAINCtrlUpdateMode(pCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_ClearModeGAIN(void)
{
	stGAIN.cntID = 2;	stGAIN.frmID = 3;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AECTRL_IsGAINCtrlMode(PAE_CTRL_PARAM pCTRL)
{
	if (pCTRL->ctrl&AECTRL_GAINCTL_MANU) return FALSE;
	return (stGAIN.gain>stGAIN.min)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AECTRL_IsGAINCtrlMax(void)
{
	//UARTprintf("stGAIN.gain:%d/stGAIN.max:%d\r\n",stGAIN.gain,stGAIN.max);
	return (stGAIN.gain>=stGAIN.max)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AECTRL_IsGAINCtrlHistMax(void)
{
	return (stGAIN.gain<(stGAIN.max/2))? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AECTRL_IsGAINCtrlMin(void)
{
	//(stGAIN.gain==stGAIN.min)? 	UARTprintf("TRUE/stGAIN.gain:%d/stGAIN.min:%d\r\n",stGAIN.gain,stGAIN.min) : UARTprintf("FALSE/stGAIN.gain:%d/stGAIN.min:%d\r\n",stGAIN.gain,stGAIN.min);
	return (stGAIN.gain==stGAIN.min)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetGAINRange(LONG min, LONG max)
{

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveAEGainMin(min);
	SPImAPP_SetSlaveAEGainMax(max);
#endif
//#else
	stGAIN.min = min;
	stGAIN.max = max;
//#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetGAINMax(LONG max)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveAEGainMax(max);
#endif
//#else
	stGAIN.max = max;
//#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetGAINMin(LONG min)
{
	stGAIN.min = min;
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetGAINPMax(LONG pmax)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveAEGainPMax(pmax);
#endif
//#else
	stGAIN.pmax = pmax;
//#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetGAINGain(LONG gain)
{
	stGAIN.gain = gain;
	AECTRL_GAINCtrlUpdateMode(NULL);
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetGAINGainDirect(LONG gain)
{
	stGAIN.gain = gain;
	AECTRL_GAINCtrlUpdateModeDirect(NULL);
}

//--------------------------------------------------------------------------------------------------------------------------
LONG AECTRL_GetGAINGain(void)
{
	return stGAIN.gain;
}

//--------------------------------------------------------------------------------------------------------------------------
LONG AECTRL_GetGAINAGain(void) // analog gain
{
	if(stGAIN.gain>AECTRL_GetGAINAMax()) return AECTRL_GetGAINAMax();
	else return stGAIN.gain;
}

//--------------------------------------------------------------------------------------------------------------------------
LONG AECTRL_GetGAINMin(void)
{
	return stGAIN.min;
}

//--------------------------------------------------------------------------------------------------------------------------
LONG AECTRL_GetGAINMax(void) // this value is changed by MENU
{
	return stGAIN.max;
}

//--------------------------------------------------------------------------------------------------------------------------
LONG AECTRL_GetGAINAMax(void) // analog gain max
{
	if(stGAIN.max > 0x20000) return 0x20000;
	else return stGAIN.max;
}

//--------------------------------------------------------------------------------------------------------------------------
LONG AECTRL_GetGAINPMax(void) // this value is fixed by CMOS
{
	return stGAIN.pmax;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE AECTRL_GetGAINCnt(void)
{
	return (BYTE)stGAIN.cntID;
}

//--------------------------------------------------------------------------------------------------------------------------
LONG AECTRL_GetGAINPrev(void)
{
	return MAX(stGAIN.prev,stGAIN.min);
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_GAINCheck(void)
{
	AECTRL_SetGAINGain(stGAIN.gain);
}

//--------------------------------------------------------------------------------------------------------------------------
LONG AECTRL_GetGainCTLMode(void)
{
	return stAECTRL.ctrl&AECTRL_GAIN_MASK ;
}

/*  FILE_END _HERE */
