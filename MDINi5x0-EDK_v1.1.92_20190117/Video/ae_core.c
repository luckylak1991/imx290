// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"
#include	"osd.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static const SHORT AECTRL_DCIRIS_REF[]		= {
	959, 954, 949, 943, 938, 933, 928, 923, 918, 912,	//  0 ~  9
	907, 902, 897, 891, 886, 881, 876, 871, 866, 860,	// 10 ~ 19
	855, 850, 845, 840, 834, 829, 824, 819, 814, 808,	// 20 ~ 29
	803, 798, 793, 788, 782, 777, 772, 767, 762, 757,	// 30 ~ 39
	751, 743, 735, 727, 718, 710, 702, 694, 685, 677,	// 40 ~ 49
	669, 661, 652, 644, 636, 627, 619, 611, 602, 594,	// 50 ~ 59
	586, 578, 569, 561, 553, 545, 537, 528, 520, 511,	// 60 ~ 69
	503, 495, 487, 479, 470, 462, 454, 446, 438, 429,	// 70 ~ 79
	421, 410, 400, 390, 379, 369, 359, 349, 338, 328,	// 80 ~ 89
	317, 307, 297, 287, 276, 266, 256, 245, 235, 225,	// 90 ~ 99
	214													// 100
};

static LONG AE_Yref;
static WORD DEBUG_ODM;
VBYTE fAECtrlODM_XFER = FALSE;

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

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

AE_SENS_PARAM stSENS;

//--------------------------------------------------------------------------------------------------------------------------
static void AECTRL_SensCtrlDefineProc(PAE_CTRL_PARAM pCTRL)
{
	LONG tmpl;

	stSENS.frmID=1;

	if(pCTRL->Ydiff >= 0) {
		if(pCTRL->YDIFF < (pCTRL->band+700*2)) {
			tmpl = 0;
		} else {
			tmpl = (LONG)((FLOAT)pCTRL->Yref/ pCTRL->Ysum*stSENS.val) - stSENS.val;
			//tmpl = tmpl * pCTRL->level / 30;
			tmpl = MAX(MIN(tmpl/4, -1), (stSENS.min - stSENS.max)/8); // max = -1
			//UARTprintf("tmpl:%d/min:%d/max:%d\r\n",tmpl,stSENS.min,stSENS.max);
		}
	} else {
		if((pCTRL->YDIFF) < (pCTRL->band+700*1)) {
			tmpl = 0;
		} else {
			tmpl = (LONG)((FLOAT)pCTRL->Yref/ pCTRL->Ysum*stSENS.val) - stSENS.val;
			//tmpl = tmpl * 30 / pCTRL->level;
			tmpl = MAX(MIN(tmpl/8, (stSENS.max - stSENS.min)/16), 1); // min = 1
			stSENS.val = stSENS.val + tmpl;
			//UARTprintf("tmpl:%d/min:%d/max:%d\r\n",tmpl,stSENS.min,stSENS.max);
		}
	}
	stSENS.val = stSENS.val + tmpl;
	stSENS.val = MAX(stSENS.min,MIN(stSENS.max,stSENS.val));
	//UARTprintf("stSENS.val:%d\r\n",stSENS.val);

#if defined(__USE_CMOS_IMX226__)
	AE_UpdateSensUpVal(1, stSENS.val);
#else
	AE_UpdateSensUpVal(0, stSENS.val);
#endif
}

#if defined(__USE_CMOS_IMX226__)
//--------------------------------------------------------------------------------------------------------------------------
void ISPM AE_UpdateSensUpVBlank(void)
{
	if(stSENS.fUpdateSensUpVBlank)
		CMOS_SetSensup(stSENS.val);

	stSENS.fUpdateSensUpVBlank = 0;
}
#endif

void ISPM AE_UpdateSensUpVal(BOOL mode,LONG val) //mode 1: update by vblink  , 0:update directly
{
	if(mode) {
		stSENS.val = val;
		stSENS.fUpdateSensUpVBlank = 1;
	} else {
		CMOS_SetSensup(val);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SensHandler(PAE_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&AECTRL_PROC_MASK)!=AECTRL_PROC_SENS) return;
	if (stSENS.cntID<MAX(stSENS.frmID,1)) stSENS.cntID++;
	if (stSENS.cntID<MAX(stSENS.frmID,1)) return;
	stSENS.cntID = 0;

	AECTRL_SensCtrlDefineProc(pCTRL);	// define process
}

//--------------------------------------------------------------------------------------------------------------------------
WORD AECTRL_GetSensUpMax(void)
{
	return((WORD)stSENS.max);
}

//--------------------------------------------------------------------------------------------------------------------------
WORD AECTRL_GetSensUpMin(void)
{
	return((WORD)stSENS.min);
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetSensUpMax(WORD val)
{
	stSENS.max = (int) val;
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetSensUpMin(WORD val)
{
	stSENS.min = (int) val;
}

//--------------------------------------------------------------------------------------------------------------------------
WORD AECTRL_GetSensUpVal(void)
{
	//UARTprintf("stSENS.val:%d\r\n",stSENS.val);
	return(stSENS.val);
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AECTRL_IsSensUpMin(void)
{
	if(stSENS.val == stSENS.min) return TRUE;
	else return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AECTRL_IsSensUpMax(void)
{
	if(stSENS.val == stSENS.max) return TRUE;
	else return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetSensUpVal(WORD val)
{
	stSENS.val = val;
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SensUpInit(LONG min)
{
	stSENS.min = min;
	stSENS.val = min;
	stSENS.frmID = 0;
	stSENS.cntID = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
extern FLOAT aeWinRatio;
#if defined(__USE_GET_AWB_RGB_INFO__)
static BYTE centerAEratio = 0;
#endif
static void AECTRL_GetAEOPDWIND(PAE_CTRL_PARAM pCTRL)
{
	WORD rVal[10];	LONG WND[5];
#if defined(__USE_GET_AWB_RGB_INFO__)
	LONG centerSum;	LONG boundSum;
#endif
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	LONG slaveSum;
	LONG WNDs[5];
#endif

	Histogram_Read();

	MISP_MultiRead(MISP_LOCAL_ID, 0x690, (PBYTE)rVal, 20);
	WND[0] = MAKEDWORD((rVal[0]&0xff), rVal[1])>>2;	// AEWIND0
	WND[1] = MAKEDWORD((rVal[2]&0xff), rVal[3])>>2;	// AEWIND1
	WND[2] = MAKEDWORD((rVal[4]&0xff), rVal[5])>>2;	// AEWIND2
	WND[3] = MAKEDWORD((rVal[6]&0xff), rVal[7])>>2;	// AEWIND3
	WND[4] = MAKEDWORD((rVal[8]&0xff), rVal[9])>>2;	// AEWIND4
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_GetSlaveAEysum((PBYTE)&WNDs[0]);
	slaveSum = (LONG)((FLOAT)(WNDs[0]+WNDs[1]+WNDs[2]+WNDs[3]+WNDs[4])*aeWinRatio)/10.0f;
#endif
	
#if defined(__USE_GET_AWB_RGB_INFO__)
    #if 1 // valid when night mode
	if(GetDYNT_Status()) { // night mode
			if(centerAEratio < 64)
				centerAEratio++;
	} else {
		if(centerAEratio > 0)
			centerAEratio--;
	}
    #else // valid when nigh mode & IR on
	if(GetDYNT_Status()) { // night mode
		if(Get_YUV_Ratio(FALSE, 50)) {
			if(centerAEratio < 64)
				centerAEratio++;
		} else {
			if(centerAEratio > 0)
				centerAEratio--;
		}
	} else {
		if(centerAEratio > 0)
			centerAEratio--;
	}
    #endif
	//UARTprintf("GetDYNT_Status:%d / centerAEratio:%d\r\n",GetDYNT_Status(),centerAEratio);
	centerSum = WND[0]*centerAEratio/64;
	boundSum = (WND[0]+WND[1]+WND[2]+WND[3]+WND[4])*(64-centerAEratio)/(64*5);
	pCTRL->Ysum = (LONG)((FLOAT)(centerSum+boundSum)*aeWinRatio);
#else
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	pCTRL->Ysum = (LONG)((FLOAT)(WND[0]+WND[1]+WND[2]+WND[3]+WND[4])*aeWinRatio)/10.0f;
	//UARTprintf("Ysum master : %d\r\n", pCTRL->Ysum);
	pCTRL->Ysum += slaveSum;
#else
	pCTRL->Ysum = (LONG)((FLOAT)(WND[0]+WND[1]+WND[2]+WND[3]+WND[4])*aeWinRatio)/5.0f;
#endif
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveAEYSum(pCTRL->Ysum);
#endif

	pCTRL->Ydiff = pCTRL->Ysum - pCTRL->Yref;

	pCTRL->YDIFF = DIFF(pCTRL->Ysum, pCTRL->Yref);

#if defined(__USE_AF_DATA_OUT__)
	static LONG maxAFdata = 0;
	if(pCTRL->testmode == 1) {
		LONG AFdata = GetAFOPDWIND(0);

		if(AFdata > maxAFdata) maxAFdata = AFdata;
		UARTprintf("AFdata:%5d / %5d\r\n",AFdata,maxAFdata);

		OSD_StrDispOnOff(OFF);
		char diffs[30];
		sprintf(diffs,"%5d:%5d:%4d:%4d", (int)pCTRL->Ydiff,(int)AFdata,(int)AECTRL_GetSHUTGain(),(int)AECTRL_GetGAINGain());
		OSD_MessageBox(ON, diffs);
	} else {
		maxAFdata = 0;
	}
#else
	if(pCTRL->testmode == 1) {
		OSD_StrDispOnOff(OFF);
		char diffs[30];
		extern WORD offsetIRIS;
	#if defined(__USE_PWMIRIS_ONLY__)
		sprintf(diffs,"%5d:%4d:%4d:%4d", (int)pCTRL->Ydiff,(int)offsetIRIS,(int)AECTRL_GetSHUTGain(),(int)AECTRL_GetGAINGain());
	#else
		sprintf(diffs,"%5d:%4d:%4d", (int)pCTRL->Ydiff,(int)AECTRL_GetSHUTGain(),(int)AECTRL_GetGAINGain());
	#endif
		OSD_MessageBox(ON, diffs);
	}
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void MANUAL_ModeHandler(PAE_CTRL_PARAM pCTRL)
{
	WORD xID;

	AECTRL_IRISPWM_SetFullOpen();
	
	pCTRL->ctrl &= (~AECTRL_PROC_MASK);

	if((pCTRL->ctrl & AECTRL_PRIORITY_MASK) == AECTRL_PRIORITY_AUTO) {
		pCTRL->ctrl &= (~AECTRL_SHUTCTL_MANU); // shut auto
		pCTRL->ctrl &= (~AECTRL_GAINCTL_MANU); // gain auto

		if (AECTRL_IsSensUpMin() == FALSE) {
			pCTRL->ctrl |= AECTRL_PROC_SENS; xID = 0;
		} else if (AECTRL_IsGAINCtrlMode(pCTRL)==TRUE) { // (stGAIN.gain>stGAIN.min)
			if(AECTRL_IsGAINCtrlMax() == TRUE) {
				if (pCTRL->Ysum>pCTRL->Yref) {
					pCTRL->ctrl |= AECTRL_PROC_GAIN; xID = 1;// no operation
				} else {
					if (pCTRL->Ysum<(pCTRL->Yref-pCTRL->band*1)) {
						if(AECTRL_IsSensUpMax() == FALSE) {
							pCTRL->ctrl |= AECTRL_PROC_SENS;  xID = 2;
						}
					} else {
						pCTRL->ctrl |= AECTRL_PROC_GAIN; xID = 3;// no operation
					}
				}
			} else {
				pCTRL->ctrl |= AECTRL_PROC_GAIN; xID = 4;
			}
		} else if (AECTRL_IsSHUTCtrlMode(pCTRL)==TRUE) { // (stSHUT.gain<stSHUT.max)
			pCTRL->ctrl |= AECTRL_PROC_SHUT; xID = 5;
		} else if (pCTRL->Ysum>(pCTRL->Yref+pCTRL->band*1)) {
			pCTRL->ctrl |= AECTRL_PROC_SHUT; xID = 6;
		} else if (pCTRL->Ysum<(pCTRL->Yref-pCTRL->band*1)) {
			if(AECTRL_IsGAINCtrlMax() == TRUE) {
				if(AECTRL_IsSensUpMax() == FALSE) {
					pCTRL->ctrl |= AECTRL_PROC_SENS; xID = 7;
				}
			} else {
				pCTRL->ctrl |= AECTRL_PROC_GAIN; xID = 8;
			}
		} else {
				pCTRL->ctrl |= AECTRL_PROC_GAIN; xID = 9;
		}
	} else if((pCTRL->ctrl & AECTRL_PRIORITY_MASK) == AECTRL_PRIORITY_SHUTTER) {
		pCTRL->ctrl |= AECTRL_SHUTCTL_MANU; // shut manual
		pCTRL->ctrl &= (~AECTRL_GAINCTL_MANU); // gain auto
		pCTRL->ctrl |= AECTRL_PROC_GAIN;
	} else if((pCTRL->ctrl & AECTRL_PRIORITY_MASK) == AECTRL_PRIORITY_GAIN) {
		pCTRL->ctrl &= (~AECTRL_SHUTCTL_MANU); // shut auto
		pCTRL->ctrl |= AECTRL_GAINCTL_MANU; // gain manual

		if (AECTRL_IsSensUpMin() == FALSE) {
			pCTRL->ctrl |= AECTRL_PROC_SENS;
		} else if (AECTRL_IsSHUTCtrlMode(pCTRL)==TRUE) { // (stSHUT.gain<stSHUT.max)
			pCTRL->ctrl |= AECTRL_PROC_SHUT;
		} else {
				pCTRL->ctrl |= AECTRL_PROC_SHUT;
		}
	}

	//UARTprintf("%d:%5d\r\n",xID,pCTRL->Ysum-pCTRL->Yref);
}

//--------------------------------------------------------------------------------------------------------------------------
static void MANUAL_GAINinSHUT(PAE_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&AECTRL_PROC_MASK)!=AECTRL_PROC_SHUT) return;
	if ((pCTRL->ctrl&AECTRL_GAINCTL_MANU)==AECTRL_GAINCTL_MANU) return;

	AECTRL_SetGAINGain(AECTRL_GetGAINMin());
	//UARTprintf("MANUAL_GAINinSHUT : gain : %d", AECTRL_GetGAINMin());
	AECTRL_ClearModeGAIN();
}

//--------------------------------------------------------------------------------------------------------------------------
static void MANUAL_SHUTinGAIN(PAE_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&AECTRL_PROC_MASK)!=AECTRL_PROC_GAIN) return;
	if ((pCTRL->ctrl&AECTRL_SHUTCTL_MANU)==AECTRL_SHUTCTL_MANU) return;

	AECTRL_SetSHUTGain(AECTRL_GetSHUTMax());
	AECTRL_ClearModeSHUT();

	if (pCTRL->more==0) return;		// more shut control

#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()==1) UARTprintf("[MORE]");
#endif

	pCTRL->ctrl &= (~AECTRL_PROC_MASK);
	pCTRL->ctrl |= ( AECTRL_PROC_SHUT);
	pCTRL->more = 0; AECTRL_SHUTHandler(pCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
static void MANUAL_AECtrlProcess(PAE_CTRL_PARAM pCTRL)
{
	if (pCTRL->ctrl&AECTRL_PROCESS_OFF) return;

	MANUAL_ModeHandler(pCTRL);
	AECTRL_SensHandler(pCTRL);
	AECTRL_SHUTHandler(pCTRL);
	AECTRL_GAINHandler(pCTRL);

	if((pCTRL->ctrl & AECTRL_PRIORITY_MASK) != AECTRL_PRIORITY_AUTO) {
		MANUAL_GAINinSHUT(pCTRL);
		MANUAL_SHUTinGAIN(pCTRL);
	}
}

#ifdef __P_IRIS_TEST__
void AE_P_IRIS_Open()
{
	UARTprintf("P_IRIS Test...open...\r\n");
	P_IRIS_C0(HIGH);
	P_IRIS_C1(LOW);
	P_IRIS_C2(LOW);
	P_IRIS_C3(LOW);
	Delay_uSec(500);

	P_IRIS_C0(HIGH);
	P_IRIS_C1(HIGH);
	P_IRIS_C2(LOW);
	P_IRIS_C3(LOW);
	Delay_uSec(500);

	P_IRIS_C0(LOW);
	P_IRIS_C1(HIGH);
	P_IRIS_C2(LOW);
	P_IRIS_C3(LOW);
	Delay_uSec(500);

	P_IRIS_C0(LOW);
	P_IRIS_C1(HIGH);
	P_IRIS_C2(LOW);
	P_IRIS_C3(HIGH);
	Delay_uSec(500);

	P_IRIS_C0(LOW);
	P_IRIS_C1(LOW);
	P_IRIS_C2(LOW);
	P_IRIS_C3(HIGH);
	Delay_uSec(500);

	P_IRIS_C0(LOW);
	P_IRIS_C1(LOW);
	P_IRIS_C2(HIGH);
	P_IRIS_C3(HIGH);
	Delay_uSec(500);

	P_IRIS_C0(LOW);
	P_IRIS_C1(LOW);
	P_IRIS_C2(HIGH);
	P_IRIS_C3(LOW);
	Delay_uSec(500);

	P_IRIS_C0(HIGH);
	P_IRIS_C1(LOW);
	P_IRIS_C2(HIGH);
	P_IRIS_C3(LOW);
	Delay_uSec(500);


}

void AE_P_IRIS_Close()
{
	P_IRIS_C0(HIGH);
	P_IRIS_C1(LOW);
	P_IRIS_C2(HIGH);
	P_IRIS_C3(LOW);
	Delay_uSec(500);

	P_IRIS_C0(LOW);
	P_IRIS_C1(LOW);
	P_IRIS_C2(HIGH);
	P_IRIS_C3(LOW);
	Delay_uSec(500);

	P_IRIS_C0(LOW);
	P_IRIS_C1(LOW);
	P_IRIS_C2(HIGH);
	P_IRIS_C3(HIGH);
	Delay_uSec(500);

	P_IRIS_C0(LOW);
	P_IRIS_C1(LOW);
	P_IRIS_C2(LOW);
	P_IRIS_C3(HIGH);
	Delay_uSec(500);

	P_IRIS_C0(LOW);
	P_IRIS_C1(HIGH);
	P_IRIS_C2(LOW);
	P_IRIS_C3(HIGH);
	Delay_uSec(500);

	P_IRIS_C0(LOW);
	P_IRIS_C1(HIGH);
	P_IRIS_C2(LOW);
	P_IRIS_C3(LOW);
	Delay_uSec(500);

	P_IRIS_C0(HIGH);
	P_IRIS_C1(HIGH);
	P_IRIS_C2(LOW);
	P_IRIS_C3(LOW);
	Delay_uSec(500);

	P_IRIS_C0(HIGH);
	P_IRIS_C1(LOW);
	P_IRIS_C2(LOW);
	P_IRIS_C3(LOW);
	Delay_uSec(500);
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
static int dGain_Fcnt = 0;
static int dGainInt = 256;
static int dGainVal = 0;
static int dGainYsum0 = 0;
static int dGainYsum1 = 0;
extern BOOL dGainFlag;
static int aGainInt = 0;
static int aGainVal = 0;

int dGainInterval[13];

static void AECTRL_DGainHandler(PAE_CTRL_PARAM pCTRL)
{
	int i;

	if(dGainFlag == FALSE) return;

	if(dGain_Fcnt > 10) {
		dGain_Fcnt = 0;
	} else {
		dGain_Fcnt++;
	}

	if(dGain_Fcnt == 3)
		dGainYsum0 = pCTRL->Ysum;
	else if(dGain_Fcnt == 6)
		dGainYsum1 = pCTRL->Ysum;

	if(aGainInt == 13) {
		aGainInt++;
		UARTprintf("Setup end~\r\n");
		for(i = 0;i < 13;i++)
			UARTprintf("%2d : %3d\r\n", i, dGainInterval[i]);
		return;
	} else if(aGainInt > 13) return;

	if(dGain_Fcnt == 1) { // again = cmosgain + 1 / dgain = 256
		dGainVal = 256;
		aGainVal = aGainInt + 1;
		UARTprintf("%02d:", aGainInt);
	} else if(dGain_Fcnt == 4) { // again = cmosgain / dgain += 10
		aGainVal = aGainInt;
		dGainInt += 10;
		if(dGainInt > 1024) dGainInt = 256;
		dGainVal = dGainInt;
		UARTprintf("%03d/", dGainVal);
		//UARTprintf("1 : aGainVal : %d / dGainInt : %d\r\n", aGainVal, dGainInt);
	} else if(dGain_Fcnt == 7) {
		if(dGainYsum0 <= dGainYsum1) {
			UARTprintf("\r\naGainVal : %2d / dGainInt : %d\r\n", aGainVal, dGainInt-256-10);
			dGainInterval[aGainInt] = dGainInt-256-10;
			aGainInt ++;
			dGainInt = 256;
		}
	}

	CMOS_WordWrite(I2C_EXT1_ADDR, 0x0023,	aGainVal);
	CMOS_WordWrite(I2C_EXT1_ADDR, 0x0024,	aGainVal);
	MISP_RegWrite(MISP_LOCAL_ID, 0x601, LOWORD(dGainVal));
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_CmdStd(SPIM_MISP_CID_S2, SPIM_CMD_AEDGAIN, dGainVal);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void AE_YsumHandler(PAE_CTRL_PARAM pCTRL)
{
	AECTRL_GetAEOPDWIND(pCTRL);		// get AE window data
}

//--------------------------------------------------------------------------------------------------------------------------
void AE_ProcessHandler(PAE_CTRL_PARAM pCTRL)
{
	AECTRL_BLCModeHandler(pCTRL);	// BLC mode handler

	AECTRL_DGainHandler(pCTRL);	// BLC mode handler

	if ((pCTRL->ctrl&AECTRL_LENS_MASK)==AECTRL_DCIRIS_LENS) {
#if defined(__USE_DCIRIS_HALL__) || defined(__USE_DCIRIS_DAMP__) || defined(__USE_PWMIRIS_ONLY__) || defined(__USE_P_IRIS_ONLY__)
		DCLENS_AECtrlProcess(pCTRL); // auto iris lens
#endif
	} else if ((pCTRL->ctrl&AECTRL_LENS_MASK)==AECTRL_MANUAL_LENS) {
		MANUAL_AECtrlProcess(pCTRL); // manual lens
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void AE_DisplayHandler(PAE_CTRL_PARAM pCTRL)
{
	WORD rODM[3], rVal[2], wVal[4];

	MISP_RegRead(MISP_LOCAL_ID, 0x752, &rODM[0]); //odm ae window display enable
	if((rODM[0]&0x1f) == 0) return;

	MISP_MultiRead(MISP_LOCAL_ID, 0x60d, (PBYTE)&rODM, 6); // odm_enwin_bgn_x~
	MISP_MultiRead(MISP_LOCAL_ID, 0x680, (PBYTE)&rVal, 4); // odm_ae_win_bgn_x ~

	wVal[0] = rODM[0]+HIBYTE(rVal[0])*HIBYTE(rODM[2]);	// sX of WNDA
	wVal[1] = rODM[1]+LOBYTE(rVal[0])*LOBYTE(rODM[2]);	// sY of WNDA
	wVal[2] = wVal[0]+HIBYTE(rVal[1])*HIBYTE(rODM[2]);	// eX of WNDA
	wVal[3] = wVal[1]+LOBYTE(rVal[1])*LOBYTE(rODM[2]);	// eY of WNDA

	MISP_MultiWrite(MISP_LOCAL_ID, 0x753, (PBYTE)wVal, 8); //odm window #1 (0x752 is visiable enable)
	MISP_RegField(MISP_LOCAL_ID, 0x752, 0, 1, pCTRL->blcwinden);	// odm window display enable
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetBrightness(PAE_CTRL_PARAM pCTRL, SHORT val)
{
	pCTRL->level = MIN(MAX(val,0),100); // 0 ~ 100

	pCTRL->Yref = AECTRL_GetYREFVal(pCTRL->level);
	//UARTprintf("level : %d / Yref : %d\r\n", pCTRL->level, pCTRL->Yref);
	pCTRL->gain = AECTRL_DCIRIS_REF[pCTRL->level];
	//UARTprintf("pCTRL->gain :%d\r\n", pCTRL->gain);
	AE_Yref = (LONG)(pCTRL->Yref+(pCTRL->band/2));
	//UARTprintf("AE_Yref : %d\r\n", AE_Yref);
#if defined(__USE_CMOS_AR0134__INTERNAL_AE)
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3102,  pCTRL->Yref);	//set AE reference
#endif

#if defined(__USE_DCIRIS_HALL__) ||defined(__USE_DCIRIS_DAMP__	) || defined(__USE_P_IRIS_ONLY__)
	if ((pCTRL->ctrl&AECTRL_LENS_MASK)==AECTRL_MANUAL_LENS)
		 pCTRL->mark = DCLENS_GetIRISMin();
	else pCTRL->mark = pCTRL->gain+pCTRL->diff;
 	DCLENS_ClearModeIRIS();
#elif defined(__USE_PWMIRIS_ONLY__)
	DCLENS_IRISAutoMode(0,pCTRL);
#endif
//	if (AECTRL_IsGAINCtrlMode(pCTRL)==FALSE)
//		DCLENS_SetIRISGain(pCTRL->mark);
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveAEBright(val);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
LONG AECTRL_GetBrightness(void)
{
	return (LONG)AE_Yref;
}

//--------------------------------------------------------------------------------------------------------------------------
// fmode = 0(normal), 1(btw min and max)
//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetGAINCtrl(PAE_CTRL_PARAM pCTRL, LONG val, LONG ctrl, BOOL fmode)
{
	pCTRL->ctrl &= (~AECTRL_GAINCTL_MANU); // change to Manual AE mode
	pCTRL->ctrl |= ctrl; // set AE mode

	if(fmode == 0)
		AECTRL_SetGAINGain(val); // btw (min, max)
	else
		AECTRL_SetGAINGainDirect(val); //just set
	//UARTprintf("AECTRL_SetGAINCtrl : gain : %d", AECTRL_GetGAINMin());
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_SetSHUTCtrl(PAE_CTRL_PARAM pCTRL, SHORT val, LONG ctrl)
{
	pCTRL->ctrl &= (~AECTRL_SHUTCTL_MANU); // change to Manual AE mode
	pCTRL->ctrl |= ctrl; // set AE mode

	AECTRL_SetSHUTGain(val);
}

//--------------------------------------------------------------------------------------------------------------------------
void ODM_SetDEBUGMode(BYTE mode)
{
	DEBUG_ODM = (WORD)mode;
	UARTprintf("--------------- DEBUG_ODM : %d\r\n",DEBUG_ODM);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ODM_GetDEBUGMode(void)
{
	return (LOBYTE(DEBUG_ODM));
}

/*  FILE_END _HERE */
