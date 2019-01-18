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
	BYTE	cur;	
	BYTE	prev;	
	BYTE	cnt;	
	BYTE	wait;	

	SHORT	min;	
	SHORT	max;	
	SHORT	ival;	
	SHORT	rcnt;	

	LONG	sum;
	LONG	ref;

}	AE_VIEW_PARAM, *PAE_VIEW_PARAM;

typedef enum {
	ALL_STOP_GRAPH = 0, SET_GAIN_VALUE, SET_WAIT_VALUE,
	GET_YSUM_VALUE, SET_YSUM_PRINT

} 	AE_VIEWGRP_PROC;

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static AE_VIEW_PARAM stAEIRIS;
static AE_VIEW_PARAM stAEGAIN;
static AE_VIEW_PARAM stAESHUT;

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
#if defined(__USE_DCIRIS_HALL__) ||defined(__USE_DCIRIS_DAMP__	)||defined(__USE_PWMIRIS_ONLY__) || defined(__USE_P_IRIS_ONLY__)
static SHORT IRIS_REF;
#endif

static void AEIRIS_SetGainValue(PAE_CTRL_PARAM pCTRL)
{
#if defined(__USE_DCIRIS_HALL__) ||defined(__USE_DCIRIS_DAMP__	) || defined(__USE_P_IRIS_ONLY__)
	if (stAEIRIS.cur!=SET_GAIN_VALUE) return;

	AECTRL_SetBrightness(pCTRL, stAEIRIS.ival);
	DCLENS_SetIRISGain(DCLENS_GetIRISGain()-8);
	stAEIRIS.cnt = 0;	stAEIRIS.cur = SET_WAIT_VALUE;
#elif defined(__USE_PWMIRIS_ONLY__)
	if (stAEIRIS.cur!=SET_GAIN_VALUE) return;

	stAEIRIS.ref = pCTRL->Yref;
	MISP_RegWrite(MISP_LOCAL_ID, 0x67F, stAEIRIS.ival);
	stAEIRIS.cnt = 0;	stAEIRIS.cur = GET_YSUM_VALUE;
#else
	return;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AEIRIS_GetOverState(PAE_CTRL_PARAM pCTRL)
{
#if defined(__USE_DCIRIS_HALL__) ||defined(__USE_DCIRIS_DAMP__) || defined(__USE_P_IRIS_ONLY__)
	if (stAEIRIS.cur!=SET_WAIT_VALUE) return;

	if (stAEIRIS.cnt<stAEIRIS.wait) stAEIRIS.cnt++;
	if (stAEIRIS.cnt<stAEIRIS.wait) return;

	stAEIRIS.cnt = 0;	stAEIRIS.cur = GET_YSUM_VALUE;
#else
	return;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AEIRIS_GetProcState(PAE_CTRL_PARAM pCTRL)
{
#if defined(__USE_DCIRIS_HALL__) ||defined(__USE_DCIRIS_DAMP__	)||defined(__USE_PWMIRIS_ONLY__) || defined(__USE_P_IRIS_ONLY__)
	if (stAEIRIS.cur!=GET_YSUM_VALUE) return;

	if(stAEIRIS.cnt == 0)
		stAEIRIS.sum = Get_AE_Ysum();
	else
		stAEIRIS.sum += Get_AE_Ysum();
	//UARTprintf("					ival:%d/sum:%d/ysum:%d\r\n",stAEIRIS.ival,stAEIRIS.sum,Get_AE_Ysum());
	if (stAEIRIS.cnt<stAEIRIS.wait) stAEIRIS.cnt++;
	if (stAEIRIS.cnt<stAEIRIS.wait) return;

	stAEIRIS.sum /= stAEIRIS.wait;

	UARTprintf("rcnt[%4d]-			ival:%6d/sum:%6d/ref:%6d\r\n",stAEIRIS.rcnt,stAEIRIS.ival,stAEIRIS.sum, stAEIRIS.ref);

	stAEIRIS.cnt = 0;
	stAEIRIS.wait = 16;

	if(stAEIRIS.sum <= stAEIRIS.ref) {
		if(stAEIRIS.rcnt ==0) {
			UARTprintf("Calib. of PWM IRIS Error!!!!!!!\r\n" );
			UARTprintf("	Change initial value of (stAESHUT.ival)\r\n" );
			stAESHUT.cur = ALL_STOP_GRAPH;
		}
		IRIS_REF =  stAEIRIS.ival -1;
		stAEIRIS.cur = SET_YSUM_PRINT;
	} else {
		stAEIRIS.cur = SET_GAIN_VALUE;
	}
	stAEIRIS.ival++;
	stAEIRIS.rcnt++;
#else
	return;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AEIRIS_SetYsumPrint(PAE_CTRL_PARAM pCTRL)
{
	if (stAEIRIS.cur!=SET_YSUM_PRINT) return;

#if defined(__USE_DCIRIS_HALL__) ||defined(__USE_DCIRIS_DAMP__	) || defined(__USE_P_IRIS_ONLY__)
	UARTprintf("%03d %06d %04d\r\n", stAEIRIS.ival, pCTRL->Ysum, DCLENS_GetIRISGain());
	stAEIRIS.sum = 0;	stAEIRIS.ival--;
	if (stAEIRIS.ival<stAEIRIS.min)	stAEIRIS.cur = ALL_STOP_GRAPH;
	else							stAEIRIS.cur = SET_GAIN_VALUE;

	if (stAEIRIS.cur!=ALL_STOP_GRAPH) return;
#elif defined(__USE_PWMIRIS_ONLY__)
	stAEIRIS.cur = ALL_STOP_GRAPH;
	UARTprintf("\r\nIRIS Ref value : %4d\r\n",IRIS_REF);

	UARTprintf("\r\nEnd Calib. of PWM IRIS...\r\n" );
	stAEIRIS.cur = ALL_STOP_GRAPH;

#endif

	AECTRL_SetBrightness(pCTRL, 30);
	SetMsgCommandLongExec(OFF);
}

//--------------------------------------------------------------------------------------------------------------------------
void AE_DBGIrisHandler(PAE_CTRL_PARAM pCTRL)
{
	if (stAEIRIS.cur==ALL_STOP_GRAPH) return;

	AEIRIS_SetGainValue(pCTRL);
	AEIRIS_GetOverState(pCTRL);
	AEIRIS_GetProcState(pCTRL);
	AEIRIS_SetYsumPrint(pCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
void SetAEDBGIrisTrigger(void)
{
	SetMsgCommandLongExec(ON);

#if defined(__USE_DCIRIS_HALL__) ||defined(__USE_DCIRIS_DAMP__	) || defined(__USE_P_IRIS_ONLY__)
	stAEIRIS.min  = 0;	stAEIRIS.max = 128;
	stAEIRIS.wait = 5;	stAEIRIS.ival = 128;
#elif defined(__USE_PWMIRIS_ONLY__)
	UARTprintf("\r\nStart Calib. of PWM IRIS...\r\n" );
	stAEIRIS.min  = 0;	stAEIRIS.max = 4095;
	stAEIRIS.wait = 16;
	stAEIRIS.ival = 1000;
	stAEIRIS.rcnt = 0;
#endif
	stAEIRIS.cur = SET_GAIN_VALUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AEGAIN_SetGainValue(PAE_CTRL_PARAM pCTRL)
{
	if (stAEGAIN.cur!=SET_GAIN_VALUE) return;

	CMOS_SetGain((WORD)stAEGAIN.ival);
	stAEGAIN.cnt = 0;	stAEGAIN.cur = SET_WAIT_VALUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AEGAIN_SetWaitValue(PAE_CTRL_PARAM pCTRL)
{
	if (stAEGAIN.cur!=SET_WAIT_VALUE) return;

	if (stAEGAIN.cnt<stAEGAIN.wait) stAEGAIN.cnt++;
	if (stAEGAIN.cnt<stAEGAIN.wait) return;

	stAEGAIN.cnt = 0;	stAEGAIN.cur = GET_YSUM_VALUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AEGAIN_GetYsumValue(PAE_CTRL_PARAM pCTRL)
{
	if (stAEGAIN.cur!=GET_YSUM_VALUE) return;

	if (stAEGAIN.cnt<stAEGAIN.wait) stAEGAIN.cnt++;

	stAEGAIN.sum += pCTRL->Ysum;
	if (stAEGAIN.cnt<stAEGAIN.wait) return;

	stAEGAIN.cnt = 0;	stAEGAIN.cur = SET_YSUM_PRINT;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AEGAIN_SetYsumPrint(PAE_CTRL_PARAM pCTRL)
{
	if (stAEGAIN.cur!=SET_YSUM_PRINT) return;

	UARTprintf("%03d %06d\r\n", stAEGAIN.ival, stAEGAIN.sum/stAEGAIN.wait);

	stAEGAIN.sum = 0;	stAEGAIN.ival++;

	if (stAEGAIN.ival>stAEGAIN.max)	stAEGAIN.cur = ALL_STOP_GRAPH;
	else							stAEGAIN.cur = SET_GAIN_VALUE;

	if (stAEGAIN.cur!=ALL_STOP_GRAPH) return;

	CMOS_SetGain((WORD)stAEGAIN.min);
	SetMsgCommandLongExec(OFF);
}

//--------------------------------------------------------------------------------------------------------------------------
void AE_DBGGainHandler(PAE_CTRL_PARAM pCTRL)
{
	if (stAEGAIN.cur==ALL_STOP_GRAPH) return;

	AEGAIN_SetGainValue(pCTRL);
	AEGAIN_SetWaitValue(pCTRL);
	AEGAIN_GetYsumValue(pCTRL);
	AEGAIN_SetYsumPrint(pCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
void SetAEDBGGainTrigger(void)
{
	SetMsgCommandLongExec(ON);

	stAEGAIN.min = 0;	stAEGAIN.max = 320;
	stAEGAIN.ival = 0;	stAEGAIN.wait = 5;

	stAEGAIN.cur = SET_GAIN_VALUE;

	UARTprintf("idx   AE  \r\n");
//              000 000000
}

//--------------------------------------------------------------------------------------------------------------------------
static void AESHUT_SetGainValue(PAE_CTRL_PARAM pCTRL)
{
	if (stAESHUT.cur!=SET_GAIN_VALUE) return;

	CMOS_SetShutter((WORD)stAESHUT.ival);
	stAESHUT.cnt = 0;	stAESHUT.cur = SET_WAIT_VALUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AESHUT_SetWaitValue(PAE_CTRL_PARAM pCTRL)
{
	if (stAESHUT.cur!=SET_WAIT_VALUE) return;

	if (stAESHUT.cnt<stAESHUT.wait) stAESHUT.cnt++;
	if (stAESHUT.cnt<stAESHUT.wait) return;

	stAESHUT.cnt = 0;	stAESHUT.cur = GET_YSUM_VALUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AESHUT_GetYsumValue(PAE_CTRL_PARAM pCTRL)
{
	if (stAESHUT.cur!=GET_YSUM_VALUE) return;

	if (stAESHUT.cnt<stAESHUT.wait) stAESHUT.cnt++;

	stAESHUT.sum += pCTRL->Ysum;
	if (stAESHUT.cnt<stAESHUT.wait) return;

	stAESHUT.cnt = 0;	stAESHUT.cur = SET_YSUM_PRINT;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AESHUT_SetYsumPrint(PAE_CTRL_PARAM pCTRL)
{
	if (stAESHUT.cur!=SET_YSUM_PRINT) return;

	UARTprintf("%04d %06d\r\n", stAESHUT.ival, stAESHUT.sum/stAESHUT.wait);

	stAESHUT.sum = 0;	stAESHUT.ival++;

	if (stAESHUT.ival>stAESHUT.max)	stAESHUT.cur = ALL_STOP_GRAPH;
	else							stAESHUT.cur = SET_GAIN_VALUE;

	if (stAESHUT.cur!=ALL_STOP_GRAPH) return;

	CMOS_SetShutter((WORD)stAESHUT.min);
	SetMsgCommandLongExec(OFF);
}

//--------------------------------------------------------------------------------------------------------------------------
void AE_DBGShutHandler(PAE_CTRL_PARAM pCTRL)
{
	if (stAESHUT.cur==ALL_STOP_GRAPH) return;

	AESHUT_GetYsumValue(pCTRL);
	AESHUT_SetWaitValue(pCTRL);
	AESHUT_SetGainValue(pCTRL);
	AESHUT_SetYsumPrint(pCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
void SetAEDBGShutTrigger(void)
{
	SetMsgCommandLongExec(ON);

	stAESHUT.min = 0;	stAESHUT.max = 1120;
	stAESHUT.ival = 0;	stAESHUT.wait = 2;

	stAESHUT.cur = SET_GAIN_VALUE;

	UARTprintf(" id    AE  \r\n");
//              0000 000000
}

void SetAEDBGCleanAll(void)
{
	stAEIRIS.cur=ALL_STOP_GRAPH;
	stAESHUT.cur=ALL_STOP_GRAPH;
	stAEGAIN.cur=ALL_STOP_GRAPH;
}

/*  FILE_END _HERE */
