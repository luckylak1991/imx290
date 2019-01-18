// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if defined(__USE_DCIRIS_DAMP__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

typedef	struct
{
	LONG	Ysum;				// current AE integral summation value
	LONG	Yprv;				// previous AE integral summation value

	SHORT	gain;				// IRIS current gain
	SHORT	min;				// IRIS minimum value
	SHORT	max;				// IRIS maximum value

	BYTE	proc;				// AE IRIS control process counter
	BYTE	frmID;				// AE IRIS control wait-VD counter
	BYTE	cntID;				// AE IRIS control average counter

	BYTE	cntH2;				// AE IRIS control over-H2 counter
	BYTE	cntH1;				// AE IRIS control over-H1 counter
	BYTE	cntH0;				// AE IRIS control over-H0 counter
	BYTE	cntL2;				// AE IRIS control over-L2 counter
	BYTE	cntL1;				// AE IRIS control over-L1 counter
	BYTE	cntL0;				// AE IRIS control over-L0 counter
	BYTE	cntST;				// AE IRIS control stable counter

}	AE_IRIS_PARAM, *PAE_IRIS_PARAM;

#define		AEIRIS_STABLE_PROC		0x00		// case of stable
#define		AEIRIS_OVERH0_PROC		0x10		// case of over-high0
#define		AEIRIS_OVERH1_PROC		0x11		// case of over-high1
#define		AEIRIS_OVERH2_PROC		0x12		// case of over-high2
#define		AEIRIS_OVERL0_PROC		0x20		// case of over-low0
#define		AEIRIS_OVERL1_PROC		0x21		// case of over-low1
#define		AEIRIS_OVERL2_PROC		0x22		// case of over-low2

#define		AEIRIS_DARKH0_PROC		0x13		// case of dark-high0
#define		AEIRIS_DARKL0_PROC		0x23		// case of dark-low0

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static AE_IRIS_PARAM stIRIS;

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
static void DCLENS_IRISCtrlDefineProc(PAE_CTRL_PARAM pCTRL)
{
	// define PROC by YSUM
		stIRIS.proc = AEIRIS_STABLE_PROC;

	if (stIRIS.Ysum>(pCTRL->Yref+(pCTRL->band*1+700*0)))	// normal-range
		stIRIS.proc = AEIRIS_OVERH0_PROC;
	if (stIRIS.Ysum<(pCTRL->Yref-(pCTRL->band*0+700*0)))
		stIRIS.proc = AEIRIS_OVERL0_PROC;

	if (stIRIS.Ysum>(pCTRL->Yref+(pCTRL->band*1+700*1)))	// 1st over-range
		stIRIS.proc = AEIRIS_OVERH1_PROC;
	if (stIRIS.Ysum<(pCTRL->Yref-(pCTRL->band*0+700*1)))
		stIRIS.proc = AEIRIS_OVERL1_PROC;

	// define H0/L2 by CNT1
	if (stIRIS.proc==AEIRIS_OVERH1_PROC&&stIRIS.cntH1>=15)	// check H1(1000ms)
		stIRIS.proc = AEIRIS_OVERH0_PROC;
	if (stIRIS.proc==AEIRIS_OVERL1_PROC&&stIRIS.cntL1>=20)	// check L1(1000ms)
		stIRIS.proc = AEIRIS_OVERL2_PROC;

	// define H2/L2 by GAIN & CNT2
	if (stIRIS.Ysum>pCTRL->Yref && stIRIS.gain==stIRIS.min) stIRIS.cntH2 = 15;
	if (stIRIS.Ysum>pCTRL->Yref && stIRIS.cntL2)  stIRIS.cntH2 = stIRIS.cntL2;

	if (stIRIS.cntH2)							 stIRIS.proc = AEIRIS_OVERH2_PROC;
	if (stIRIS.Ysum<pCTRL->Yref && stIRIS.cntL2) stIRIS.proc = AEIRIS_OVERL2_PROC;
		
	// clear CNT
	if (stIRIS.proc!=AEIRIS_OVERH0_PROC) stIRIS.cntH0 = 0;
	if (stIRIS.proc!=AEIRIS_OVERL0_PROC) stIRIS.cntL0 = 0;
	if (stIRIS.proc!=AEIRIS_OVERH0_PROC  &&
		stIRIS.proc!=AEIRIS_OVERH1_PROC) stIRIS.cntH1 = 0;
	if (stIRIS.proc!=AEIRIS_OVERL1_PROC) stIRIS.cntL1 = 0;
	if (stIRIS.proc!=AEIRIS_OVERL2_PROC) stIRIS.cntL2 = 0;
	if (stIRIS.proc!=AEIRIS_STABLE_PROC) stIRIS.cntST = 0;

	// update PRV
	if (stIRIS.proc!=AEIRIS_OVERL1_PROC) stIRIS.Yprv = stIRIS.Ysum;
}

//--------------------------------------------------------------------------------------------------------------------------
static void DCLENS_IRISCtrlOverH2Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stIRIS.proc!=AEIRIS_OVERH2_PROC) return;

	stIRIS.frmID = 1;	if (stIRIS.cntH2) stIRIS.cntH2--;

	if (stIRIS.cntH2)	stIRIS.gain += pCTRL->mark/15;
	else				stIRIS.gain  = pCTRL->mark;

	stIRIS.gain = MIN(MAX(stIRIS.gain,stIRIS.min),pCTRL->mark);

#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01) return;
	UARTprintf("[IRIS-OVERH2] gain:%03d Ysum:%05d Yprv:%05d cntH2:%02d\r\n",
				stIRIS.gain, stIRIS.Ysum, stIRIS.Yprv, stIRIS.cntH2);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void DCLENS_IRISCtrlOverH1Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stIRIS.proc!=AEIRIS_OVERH1_PROC) return;

	stIRIS.frmID = 2;			 stIRIS.cntH1++;

	stIRIS.gain = MIN(MAX(pCTRL->mark,stIRIS.min),stIRIS.max);

#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01) return;
	UARTprintf("[IRIS-OVERH1] gain:%03d Ysum:%05d Yprv:%05d cntH1:%02d\r\n",
				stIRIS.gain, stIRIS.Ysum, stIRIS.Yprv, stIRIS.cntH1);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void DCLENS_IRISCtrlOverH0Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stIRIS.proc!=AEIRIS_OVERH0_PROC) return;

	stIRIS.frmID = 1;			 stIRIS.cntH0++;
	if (stIRIS.cntH0<15) return; stIRIS.cntH0 = 0;	// 500ms

	stIRIS.gain = MAX(MIN(stIRIS.gain+1,stIRIS.max),pCTRL->mark);

#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01) return;
	UARTprintf("[IRIS-OVERH0] gain:%03d Ysum:%05d Yprv:%05d cntH0:%02d\r\n",
				stIRIS.gain, stIRIS.Ysum, stIRIS.Yprv, stIRIS.cntH0);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void DCLENS_IRISCtrlOverL2Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stIRIS.proc!=AEIRIS_OVERL2_PROC) return;

	stIRIS.frmID = 1;	if (stIRIS.cntL2<15) stIRIS.cntL2++;

	if (stIRIS.cntL2)	stIRIS.gain -= pCTRL->mark/15;
	else				stIRIS.gain  = stIRIS.min;

	stIRIS.gain = MIN(MAX(stIRIS.gain,stIRIS.min),pCTRL->mark);

#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01) return;
	UARTprintf("[IRIS-OVERL2] gain:%03d Ysum:%05d Yprv:%05d cntL2:%02d\r\n",
				stIRIS.gain, stIRIS.Ysum, stIRIS.Yprv, stIRIS.cntL2);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void DCLENS_IRISCtrlOverL1Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stIRIS.proc!=AEIRIS_OVERL1_PROC) return;

	stIRIS.frmID = 1;			 stIRIS.cntL1++;
//	if (stIRIS.cntL1%3) return; //stIRIS.cntL1 = 0;	// 500ms

	stIRIS.gain = MIN(MAX(stIRIS.gain-2,stIRIS.min),pCTRL->mark);
//	stIRIS.gain = MIN(MAX(pCTRL->mark,stIRIS.min),stIRIS.max);

	if ((stIRIS.Ysum-stIRIS.Yprv)>pCTRL->band) {
		stIRIS.cntL1 = 0;	stIRIS.Yprv = stIRIS.Ysum;
	}

#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01) return;
	UARTprintf("[IRIS-OVERL1] gain:%03d Ysum:%05d Yprv:%05d cntL1:%02d\r\n",
				stIRIS.gain, stIRIS.Ysum, stIRIS.Yprv, stIRIS.cntL1);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void DCLENS_IRISCtrlOverL0Mode(PAE_CTRL_PARAM pCTRL)
{
	if (stIRIS.proc!=AEIRIS_OVERL0_PROC) return;

	stIRIS.frmID = 1;			 stIRIS.cntL0++;
	if (stIRIS.cntL0<15) return; stIRIS.cntL0 = 0;	// 500ms

	stIRIS.gain = MIN(MAX(stIRIS.gain-1,stIRIS.min),pCTRL->mark);

#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01) return;
	UARTprintf("[IRIS-OVERL0] gain:%03d Ysum:%05d Yprv:%05d cntL0:%02d\r\n",
				stIRIS.gain, stIRIS.Ysum, stIRIS.Yprv, stIRIS.cntL0);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void DCLENS_IRISCtrlStableMode(PAE_CTRL_PARAM pCTRL)
{
	if (stIRIS.proc!=AEIRIS_STABLE_PROC) return;

	stIRIS.frmID = 2;			 stIRIS.cntST++;
	if (stIRIS.cntST<30) return; stIRIS.cntST = 0;	// 2000ms

	stIRIS.gain = pCTRL->mark+(stIRIS.gain-pCTRL->mark)/2;
	stIRIS.gain = MIN(MAX(stIRIS.gain, pCTRL->gain-8), pCTRL->gain+8);

	pCTRL->diff = stIRIS.gain-pCTRL->gain;
	pCTRL->mark = pCTRL->gain+pCTRL->diff;

#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01) return;
	UARTprintf("[IRIS-STABLE] gain:%03d Ysum:%05d Yprv:%05d diff:%03d\r\n",
				stIRIS.gain, stIRIS.Ysum, stIRIS.Yprv, pCTRL->diff);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void DCLENS_IRISCtrlUpdateMode(PAE_CTRL_PARAM pCTRL)
{
	stIRIS.gain = MIN(MAX(stIRIS.gain, stIRIS.min), stIRIS.max);
	stAEXFER.iris = (DWORD)stIRIS.gain;
//	IRIS_WordWrite(SPI_AN41919_ADDR, 0x00, stIRIS.gain);
}

//--------------------------------------------------------------------------------------------------------------------------
static void DCLENS_IRISHandler(PAE_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&AECTRL_PROC_MASK)!=AECTRL_PROC_IRIS) return;

	if (stIRIS.cntID<MAX(stIRIS.frmID,1)) stIRIS.cntID++;
	if (stIRIS.cntID<MAX(stIRIS.frmID,1)) return;

	stIRIS.cntID = 0; stIRIS.Ysum = pCTRL->Ysum;

	DCLENS_IRISCtrlDefineProc(pCTRL);	// define process
	DCLENS_IRISCtrlOverH2Mode(pCTRL);	// full-open-dec
	DCLENS_IRISCtrlOverH1Mode(pCTRL);	// over-high1-dec
	DCLENS_IRISCtrlOverH0Mode(pCTRL);	// over-high0-dec
	DCLENS_IRISCtrlOverL0Mode(pCTRL);	// over-low0-inc
	DCLENS_IRISCtrlOverL1Mode(pCTRL);	// over-low1-inc
	DCLENS_IRISCtrlOverL2Mode(pCTRL);	// full-open-inc
	DCLENS_IRISCtrlStableMode(pCTRL);	// stable mode
	DCLENS_IRISCtrlUpdateMode(pCTRL);	// update mode
}

//--------------------------------------------------------------------------------------------------------------------------
static void DCLENS_ModeHandler(PAE_CTRL_PARAM pCTRL)
{
	pCTRL->ctrl &= (~AECTRL_PROC_MASK);

	if		(AECTRL_IsGAINCtrlMode(pCTRL)==TRUE)
		pCTRL->ctrl |= AECTRL_PROC_GAIN;

	else if (DCLENS_IsIRISCtrlMode()==TRUE)
		pCTRL->ctrl |= AECTRL_PROC_IRIS;

	else if (pCTRL->Ysum>(pCTRL->Yref+pCTRL->band*1))
		pCTRL->ctrl |= AECTRL_PROC_IRIS;

	else if (pCTRL->Ysum<(pCTRL->Yref-pCTRL->band*0))
		pCTRL->ctrl |= AECTRL_PROC_GAIN;
}

//--------------------------------------------------------------------------------------------------------------------------
static void DCLENS_IRISinGAIN(PAE_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&AECTRL_PROC_MASK)!=AECTRL_PROC_GAIN) return;

	DCLENS_ClearModeIRIS();
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_AECtrlProcess(PAE_CTRL_PARAM pCTRL)
{
	if (pCTRL->ctrl&AECTRL_PROCESS_OFF) return;

	DCLENS_ModeHandler(pCTRL);
	DCLENS_IRISHandler(pCTRL);
	AECTRL_GAINHandler(pCTRL);
	DCLENS_IRISinGAIN(pCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_ClearModeIRIS(void)
{
	stIRIS.cntID = 0;	stIRIS.frmID = 1;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL DCLENS_IsIRISCtrlMode(void)
{
	return (stIRIS.gain>stIRIS.min)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_SetIRISRange(SHORT min, SHORT max)
{
	stIRIS.min = min;	stIRIS.max = max;
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_ForceSetIRISGain(SHORT gain) // use after stop AE
{
	stIRIS.gain = gain;
	DCLENS_IRISCtrlUpdateMode(NULL);

	IRIS_WordWrite(SPI_AN41919_ADDR, 0x00, stAEXFER.iris);

	stIRIS.cntID = 0;	stIRIS.frmID = 1;
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_SetIRISGain(SHORT gain)
{
	if (stIRIS.proc==AEIRIS_OVERL2_PROC) return;
	if (stIRIS.proc==AEIRIS_OVERH2_PROC) return;

	stIRIS.gain = gain;
	DCLENS_IRISCtrlUpdateMode(NULL);

	stIRIS.cntID = 0;	stIRIS.frmID = 1;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT DCLENS_GetIRISGain(void)
{
	return stIRIS.gain;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT DCLENS_GetIRISMin(void)
{
	return stIRIS.min;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT DCLENS_GetIRISMax(void)
{
	return stIRIS.max;
}

//--------------------------------------------------------------------------------------------------------------------------
WORD DCLENS_GetIRISState(void)
{
	return (WORD)stIRIS.proc;
}

#endif	/* defined(__USE_DCIRIS_DAMP__) */

/*  FILE_END _HERE */
