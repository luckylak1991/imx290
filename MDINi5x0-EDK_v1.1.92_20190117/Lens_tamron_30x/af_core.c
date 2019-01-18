// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if defined(__USE_AFZOOM_LENS__) && defined(__USE_X30X_TAMRON__)
#define NumOfBUFF 5
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

typedef	struct
{
	// previous Auto Focus value
	LONG	xFLTr;
	LONG	xFLTh;
	LONG	xFLTb;
	LONG	xYSUM;
	LONG	xGAIN;

	// new
	LONG	nFLTr;
	LONG	nFLTh;
	LONG	nFLTb;
	LONG	nYSUM;
	LONG	nGAIN;

	//old
	LONG	oFLTr;
	LONG	oFLTh;
	LONG	oFLTb;
	LONG	oYSUM;
	LONG	oGAIN;

	LONG	RATEh;
	LONG	RATEb;

	SHORT	CNTh;
	SHORT	CNTb;

	SHORT	ZOOM;
	SHORT	KNEE;

	LONG	rBUFF[NumOfBUFF+1];
	LONG	hBUFF[NumOfBUFF+1];
	LONG	bBUFF[NumOfBUFF+1];
	LONG	yBUFF[NumOfBUFF+1];
	CHAR	nIDX;
	BYTE	PROC;

	BYTE	nFRM;
	BYTE	nEXE;

	LONG	rYSUM;
	LONG	yRATE;
	LONG	yDIFF;
	SHORT	CNTy;

	BYTE	nRST;
	BYTE	nCNT;

	LONG	Branch_DARK_LL; // 65536 //0x10000
	LONG	Branch_DARK_ML; // 16384 //0x4000
	LONG	Branch_DARK_HL; // 8192 //ox2000
	LONG	Branch_SPOT_XX_Cnt; // 7
}	AF_AUTO_PARAM, *PAF_AUTO_PARAM;

typedef	struct
{
	SHORT	rVALy;
	SHORT	rGAPyL;
	SHORT	rGAPyH;
	SHORT	rLOWy;
	SHORT	rCNTy;
	SHORT	rDIFFyL; // AFCORE_CtrlTriggerByYSUM TH
	SHORT	rDIFFyM; // AFCORE_CtrlTriggerByYSUM TH
	SHORT	rDIFFyH; // AFCORE_CtrlTriggerByYSUM TH

	SHORT	rFLTh;
	SHORT	rFLTb;
	SHORT	rCNTh;
	SHORT	rCNTb;

	LONG	rSPOT;

	LONG	rMUL1L;
	LONG	rMUL1H;
	LONG	rMUL2L;
	LONG	rMUL2H;
	LONG	rMUL2LUMA;
	LONG	rMUL2DATA;
	LONG	rMUL2CORE;

}	AF_TUNE_PARAM, *PAF_TUNE_PARAM;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
typedef	struct
{
	WORD	HI[23];
	WORD	LO[23];
}	AF_HBPF_PARAM, *PAF_HBPF_PARAM;
#else 												// MDIN-i51X (i510, i540, i550)
typedef	struct
{
	WORD	HI[38];
	WORD	LO[38];
}	AF_HBPF_PARAM, *PAF_HBPF_PARAM;
#endif

typedef enum {
	OLD_BUFF = 0, NEW_BUFF

} 	AF_PROCESS_BUFF;

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static AF_AUTO_PARAM stAFAUTO = {0,};
#if 0 // mimitest
static AF_TUNE_PARAM stAFTUNE = {
	//rVALy	rGAPyL	rGAPyH	rLOWy	rCNTy	rDIFFyL	rDIFFyM	rDIFFyH
	  200,	500,		1000,	4000,	15,		5,		10,		20,
	//rFLTh	rFLTb	rCNTh	rCNTb
	  100,	100,		15,		15,
	//rSPOT	rMUL1L	rMUL1H	rMUL2L	rMUL2H
	 4500,	500,		1000,	1000,	2000,
	//rMUL2LUMA	rMUL2DATA	rMUL2CORE
	1000,		1000,		1000
};
#else
static AF_TUNE_PARAM stAFTUNE = {
	//rVALy	rGAPyL	rGAPyH	rLOWy	rCNTy	rDIFFyL	rDIFFyM	rDIFFyH
	  200,	500,		1000,	4000,	15,		5,		10,		15,
	//rFLTh	rFLTb	rCNTh	rCNTb
	  100,	200,		15,		15,
	//rSPOT	rMUL1L	rMUL1H	rMUL2L	rMUL2H
	 4500,	500,		1000,	1000,	2000,
	//rMUL2LUMA	rMUL2DATA	rMUL2CORE
	1000,		1000,		1000
};
#endif

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
static ROMDATA AF_HBPF_PARAM stAFHBPF = {
	{
		0x0102, 0x02ff, 0xff02, 0x02fe, 0xfd00, 0x00fb, 0xfbff, 0x00fa,
		0xfc03, 0x02fd, 0x0009, 0x06ff, 0x060e, 0x07ff, 0x070e, 0x00f7,
		0x0406, 0xefe9, 0xfdf9, 0xd6d9, 0x00ed, 0xa9ce, 0x7de9
	},

	{
		0xfefe, 0xfefe, 0xfefd, 0xfcfa, 0xfafb, 0xfdfe, 0xfdfa, 0xf7f8,
		0xfbfe, 0xfffb, 0xf7f5, 0xf8fd, 0x00fe, 0xf7f2, 0xf3fa, 0x0203,
		0xfbef, 0xebf3, 0x030d, 0x05ee, 0xd9dc, 0x0444, 0x8098
	}
};
#else 												// MDIN-i51X (i510, i540, i550)
static ROMDATA AF_HBPF_PARAM stAFHBPF = {
	{
		0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
		0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0x0102,
		0x02ff, 0xff02, 0x02fe, 0xfd00, 0x00fb, 0xfbff, 0x00fa, 0xfc03,
		0x02fd, 0x0009, 0x06ff, 0x060e, 0x07ff, 0x070e, 0x00f7, 0x0406,
		0xefe9, 0xfdf9, 0xd6d9, 0x00ed, 0xa9ce, 0x7de9
	},

	{
		0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
		0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0xfefe,
		0xfefe, 0xfefd, 0xfcfa, 0xfafb, 0xfdfe, 0xfdfa, 0xf7f8, 0xfbfe,
		0xfffb, 0xf7f5, 0xf8fd, 0x00fe, 0xf7f2, 0xf3fa, 0x0203, 0xfbef,
		0xebf3, 0x030d, 0x05ee, 0xd9dc, 0x0444, 0x8098
	}
};
#endif

static BOOL fSysStateCHANGE = FALSE;

#if __MISP100_AF_CNV__ == 1
static BYTE AF_ConvLUT[] = {
	  0,  23,  32,  39,  45,  50,  55,  59,  63,  67,  71,  74,  77,  80,  83,  86,
	 89,  92,  94,  97,  99, 101, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122,
	124, 125, 127, 129, 131, 132, 134, 136, 137, 139, 140, 142, 143, 145, 146, 148,
	149, 150, 152, 153, 154, 156, 157, 158, 159, 161, 162, 163, 164, 165, 167, 168,
	169, 170, 171, 172, 173, 174, 175, 177, 178, 179, 180, 181, 182, 183, 184, 185,
	185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 194, 195, 196, 197, 198, 199,
	199, 200, 201, 202, 202, 203, 204, 205, 205, 206, 207, 208, 208, 209, 210, 210,
	211, 212, 212, 213, 214, 214, 215, 216, 216, 217, 218, 218, 219, 219, 220, 221,
	221, 222, 222, 223, 223, 224, 224, 225, 226, 226, 227, 227, 228, 228, 229, 229,
	230, 230, 231, 231, 231, 232, 232, 233, 233, 234, 234, 235, 235, 235, 236, 236,
	237, 237, 237, 238, 238, 239, 239, 239, 240, 240, 240, 241, 241, 241, 242, 242,
	242, 243, 243, 243, 244, 244, 244, 245, 245, 245, 245, 246, 246, 246, 247, 247,
	247, 247, 248, 248, 248, 248, 249, 249, 249, 249, 249, 250, 250, 250, 250, 250,
	251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253,
	253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

static WORD AF_CtrlLUT = 1;
#endif

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
extern int afDebugMode;
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
static void AFCORE_GetAFOPDWIND(PAF_CTRL_PARAM pCTRL)
{
	WORD rVal[16];	LONG WND[5];

	MISP_MultiRead(MISP_LOCAL_ID, 0x6f0, (PBYTE)rVal, 32);
	pCTRL->stFILT.RPFa = MAKEDWORD(rVal[ 0], rVal[ 1])>>0; //hpfa // window a is bigger than window b
	pCTRL->stFILT.LPFa = MAKEDWORD(rVal[ 2], rVal[ 3])>>0; //lpfa
	pCTRL->stFILT.VPFa = MAKEDWORD(rVal[ 4], rVal[ 5])>>0; //vpfa
	pCTRL->stFILT.HPFa = MAKEDWORD(rVal[ 6], rVal[ 7])>>0; //bpfa
	pCTRL->stFILT.RPFb = MAKEDWORD(rVal[ 8], rVal[ 9])>>0; //hpfb
	pCTRL->stFILT.LPFb = MAKEDWORD(rVal[10], rVal[11])>>0; //lpfb
	pCTRL->stFILT.VPFb = MAKEDWORD(rVal[12], rVal[13])>>0; //vpfb
	pCTRL->stFILT.HPFb = MAKEDWORD(rVal[14], rVal[15])>>0; //bpfb

	MISP_MultiRead(MISP_LOCAL_ID, 0x6e8, (PBYTE)rVal, 16);
	pCTRL->stFILT.CNTa = MAKEDWORD(rVal[0], rVal[1])>>0; //cnta
	pCTRL->stFILT.BPFa = MAKEDWORD(rVal[2], rVal[3])>>0; //vara
	pCTRL->stFILT.CNTb = MAKEDWORD(rVal[4], rVal[5])>>0; //cntb
	pCTRL->stFILT.BPFb = MAKEDWORD(rVal[6], rVal[7])>>0; //varb

	MISP_MultiRead(MISP_LOCAL_ID, 0x690, (PBYTE)rVal, 20);
	WND[0] = MAKEDWORD((rVal[0]&0xff), rVal[1])>>2; //ae sum 0
	WND[1] = MAKEDWORD((rVal[2]&0xff), rVal[3])>>2; //ae sum 1
	WND[2] = MAKEDWORD((rVal[4]&0xff), rVal[5])>>2; //ae sum 2
	WND[3] = MAKEDWORD((rVal[6]&0xff), rVal[7])>>2; //ae sum 3
	WND[4] = MAKEDWORD((rVal[8]&0xff), rVal[9])>>2; //ae sum 4

	pCTRL->stFILT.YPRV = (pCTRL->stFILT.YSUM);
	pCTRL->stFILT.YSUM = (WND[0]+WND[1]+WND[2]+WND[3]+WND[4])/5;
	pCTRL->stFILT.GAIN = (AECTRL_GetGAINGain());

	if (pCTRL->stFILT.CNTa<stAFTUNE.rSPOT) {
		if(pCTRL->nSPT  > 0)
			pCTRL->nSPT -= 1;
	} else {
		if(pCTRL->nSPT  < 90)
			pCTRL->nSPT += 1;
	}

	stAFAUTO.ZOOM = LENS_GetRelPositionZOOM();
	stAFAUTO.KNEE = LENS_GetKneePositionZOOM();

	stAFAUTO.Branch_DARK_LL = AECTRL_GetGAINMax() - 1; // mimitest
	stAFAUTO.Branch_DARK_ML = AECTRL_GetGAINMax()/4 - 1;
	stAFAUTO.Branch_DARK_HL = AECTRL_GetGAINMax()/8 - 1;

}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerByZOOM(PAF_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_MANUAL_MODE) // manual mode --> off
		SetMsgCommandTriggerAF(OFF); // fMsgExecTriggerAF( AF Trigger Flag)

	if ((pCTRL->ctrl&AFCTRL_AFMODE_MASK)==AFCTRL_INTVAL_MODE) // interval mode --> off
		SetMsgCommandTriggerAF(OFF);

	if (IsMsgCommandTriggerAF()==FALSE) return; // if not AF

	if (IsMsgExecutionComplete()) { // if all message done
		AF_EnableProcess(ON); // AF on
		SetMsgCommandTriggerAF(OFF); // clear  AF Trigger Flag
	}

	if (IsMsgGuardZOOMComplete()) {
		AF_EnableProcess(ON);
		SetMsgCommandTriggerAF(OFF);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static LONG AFCORE_CtrlMeanBUFF(PLONG pBUFF, BOOL nID)
{
	BYTE i;		LONG sum = 0;

	for (i=0+nID; i<NumOfBUFF+nID; i++) sum += pBUFF[i];
	return MAX((LONG)((FLOAT)sum/((FLOAT)NumOfBUFF)+0.5),100);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerByBUFF(PAF_CTRL_PARAM pCTRL)
{
	BYTE i;

	if ((pCTRL->ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_MANUAL_MODE) return;
	if ((pCTRL->ctrl&AFCTRL_AFMODE_MASK)!=AFCTRL_AFAUTO_MODE) return;
	if (LENS_IsDriveEnableZOOM() || LENS_IsDriveEnableFOKS()) return;

	if (stAFAUTO.xYSUM==0)	stAFAUTO.xYSUM = AECTRL_GetBrightness();
	if (stAFAUTO.xGAIN==0)	stAFAUTO.xGAIN = pCTRL->stFILT.GAIN;
	if (stAFAUTO.xFLTh==0)	stAFAUTO.xFLTh = 1;
	if (stAFAUTO.xFLTb==0)	stAFAUTO.xFLTb = 1;

	stAFAUTO.nGAIN = (LONG)(pCTRL->stFILT.GAIN);

	stAFAUTO.rBUFF[stAFAUTO.nIDX] = pCTRL->stFILT.RPFb; // buffering (0 ~ NumOfBUFF)
	stAFAUTO.hBUFF[stAFAUTO.nIDX] = pCTRL->stFILT.HPFb;
	stAFAUTO.bBUFF[stAFAUTO.nIDX] = pCTRL->stFILT.BPFb;
	stAFAUTO.yBUFF[stAFAUTO.nIDX] = pCTRL->stFILT.YSUM; // ySUM

	if (stAFAUTO.nIDX<NumOfBUFF) {
		stAFAUTO.nIDX++;
		return;
	}

	stAFAUTO.oFLTr = AFCORE_CtrlMeanBUFF(stAFAUTO.rBUFF,OLD_BUFF); //0 ~ NumOfBUFF-1 add
	stAFAUTO.oFLTh = AFCORE_CtrlMeanBUFF(stAFAUTO.hBUFF,OLD_BUFF);
	stAFAUTO.oFLTb = AFCORE_CtrlMeanBUFF(stAFAUTO.bBUFF,OLD_BUFF);
	stAFAUTO.oYSUM = AFCORE_CtrlMeanBUFF(stAFAUTO.yBUFF,OLD_BUFF);

	stAFAUTO.nFLTr = AFCORE_CtrlMeanBUFF(stAFAUTO.rBUFF,NEW_BUFF); // 1 ~ NumOfBUFF add
	stAFAUTO.nFLTh = AFCORE_CtrlMeanBUFF(stAFAUTO.hBUFF,NEW_BUFF);
	stAFAUTO.nFLTb = AFCORE_CtrlMeanBUFF(stAFAUTO.bBUFF,NEW_BUFF);
	stAFAUTO.nYSUM = AFCORE_CtrlMeanBUFF(stAFAUTO.yBUFF,NEW_BUFF);

	for (i=1; i<(NumOfBUFF+1); i++) {
		stAFAUTO.rBUFF[i-1] = stAFAUTO.rBUFF[i]; // buffering(5-->4-->3-->2-->1-->0) // 5 is new
		stAFAUTO.hBUFF[i-1] = stAFAUTO.hBUFF[i];
		stAFAUTO.bBUFF[i-1] = stAFAUTO.bBUFF[i];
		stAFAUTO.yBUFF[i-1] = stAFAUTO.yBUFF[i];
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerByLUMA(PAF_CTRL_PARAM pCTRL) //check YSUM ratio -- If ysum ratio is too big... reset the counter ( AE Change or block)
{
	FLOAT rate;	SHORT rVALy, rGAPy;

	if ((pCTRL->ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_MANUAL_MODE) return;
	if ((pCTRL->ctrl&AFCTRL_AFMODE_MASK)!=AFCTRL_AFAUTO_MODE) return;
	if (LENS_IsDriveEnableZOOM() || LENS_IsDriveEnableFOKS()) return;
	if (stAFAUTO.nIDX<NumOfBUFF) return;

	if (stAFAUTO.nGAIN==AECTRL_GetGAINMax()) // if gain is max
		stAFAUTO.rYSUM  = (LONG)stAFAUTO.oYSUM;
	else
		stAFAUTO.rYSUM  = AECTRL_GetBrightness(); // 16700

	if (stAFAUTO.nYSUM<stAFTUNE.rLOWy) // if nYSUM < 4000
		stAFAUTO.rYSUM += 2*(LONG)stAFTUNE.rVALy; // 200

	rVALy = stAFTUNE.rVALy; // 200
	rate = ((FLOAT)stAFAUTO.rYSUM/stAFAUTO.nYSUM-1)*stAFTUNE.rMUL2LUMA; //prev and next Luma ratio
	stAFAUTO.yRATE = ABS((LONG)((rate<0)? rate-0.5 : rate+0.5));
	if (stAFAUTO.yRATE>=(LONG)rVALy) {
		stAFAUTO.CNTh = 0;
		stAFAUTO.CNTb = 0;
	}

	rGAPy = stAFTUNE.rGAPyL; // 500
	stAFAUTO.yDIFF = ABS(stAFAUTO.oYSUM-stAFAUTO.nYSUM); // prev and next Luma difference
	if (stAFAUTO.yDIFF>=(LONG)rGAPy) {
		stAFAUTO.CNTh = 0;
		stAFAUTO.CNTb = 0;
	}
	if(afDebugMode == 1) UARTprintf("AFCORE_CtrlTriggerByLUMA : yRATE(%5d:%2d) / yDIFF(%5d:%2d) / stAFAUTO.CNTh=%5d / stAFAUTO.CNTb=%5d\r\n", stAFAUTO.yRATE, rVALy, stAFAUTO.yDIFF, rGAPy, stAFAUTO.CNTh, stAFAUTO.CNTb);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerByDATA(PAF_CTRL_PARAM pCTRL) // check FLT output ratio //too much FLT data change.. then reset cnt..
{
	FLOAT rate;	SHORT rFLTh, rFLTb;

	if ((pCTRL->ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_MANUAL_MODE) return;
	if ((pCTRL->ctrl&AFCTRL_AFMODE_MASK)!=AFCTRL_AFAUTO_MODE) return;
	if (LENS_IsDriveEnableZOOM() || LENS_IsDriveEnableFOKS()) return;
	if (stAFAUTO.nIDX<NumOfBUFF) return;

	rFLTh = (LONG)((FLOAT)pCTRL->stFILT.GAIN/AECTRL_GetGAINMax()+6.5); // agc gain
	rFLTb = (LONG)((FLOAT)pCTRL->stFILT.GAIN/AECTRL_GetGAINMax()+6.5);

	rate = ((FLOAT)stAFAUTO.oFLTh/stAFAUTO.nFLTh-1)*stAFTUNE.rMUL2DATA; // prev and next FLTh ratio
	if (ABS(stAFAUTO.oFLTh-stAFAUTO.nFLTh)<10) {
		rate = 0.0f;
	}
	stAFAUTO.RATEh = ABS((LONG)((rate<0)? rate-0.5 : rate+0.5));

	rate = ((FLOAT)stAFAUTO.oFLTb/stAFAUTO.nFLTb-1)*stAFTUNE.rMUL2DATA; // prev and next FLTb ratio
	if (ABS(stAFAUTO.oFLTb-stAFAUTO.nFLTb)<10) {
		rate = 0.0f;
	}
	stAFAUTO.RATEb = ABS((LONG)((rate<0)? rate-0.5 : rate+0.5));

	if (stAFAUTO.RATEh<(LONG)rFLTh) { // compare ratio of FLTh and GAIN
		stAFAUTO.CNTh += 1;
	} else {
		stAFAUTO.CNTh  = 0;
		stAFAUTO.CNTb  = 0;
	}

	if (stAFAUTO.RATEb<(LONG)rFLTb) {
		stAFAUTO.CNTb += 1;
	} else {
		stAFAUTO.CNTb  = 0;
		stAFAUTO.CNTh  = 0;
	}

	stAFAUTO.CNTh = MIN(stAFAUTO.CNTh,stAFTUNE.rCNTh); // 15
	stAFAUTO.CNTb = MIN(stAFAUTO.CNTb,stAFTUNE.rCNTb); // 15
	if(afDebugMode == 2) UARTprintf("AFCORE_CtrlTriggerByDATA : RATEh(%5d:%2d) / RATEb(%5d:%2d) / stAFAUTO.CNTh=%5d / stAFAUTO.CNTb=%5d\r\n", stAFAUTO.RATEh, rFLTh, stAFAUTO.RATEb, rFLTb, stAFAUTO.CNTh, stAFAUTO.CNTb);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerByNORM(PAF_CTRL_PARAM pCTRL)
{
	FLOAT nYSUM, nHPFr, rate;	SHORT rFLTh, rFLTb;

	if ((pCTRL->ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_MANUAL_MODE) return;
	if ((pCTRL->ctrl&AFCTRL_AFMODE_MASK)!=AFCTRL_AFAUTO_MODE) return;
	if (LENS_IsDriveEnableZOOM() || LENS_IsDriveEnableFOKS()) return;
	if (stAFAUTO.nIDX<NumOfBUFF) return;

	nYSUM = ((FLOAT)stAFAUTO.xYSUM)/stAFAUTO.nYSUM; // xYSUM = xRef, previous(origin) , nYSUM = new avg ySUM
	stAFAUTO.nFLTr = (LONG)(nYSUM*stAFAUTO.nFLTr+0.5); // normalization to xYSUM
	stAFAUTO.nFLTh = (LONG)(nYSUM*stAFAUTO.nFLTh+0.5);
	stAFAUTO.nFLTb = (LONG)(nYSUM*stAFAUTO.nFLTb+0.5);

	nHPFr = ((FLOAT)stAFAUTO.xFLTr)/stAFAUTO.nFLTr; // xFLTr = previous nFLTr = new ... normalization to xFLTr
	if (stAFAUTO.xFLTr==0) nHPFr = (1.0f);
	stAFAUTO.nFLTh = (LONG)(nHPFr*stAFAUTO.nFLTh+0.5); // HPF * FLTh
	stAFAUTO.nFLTb = (LONG)(nHPFr*stAFAUTO.nFLTb+0.5); // HPF * FLTb

	rate = (AECTRL_GetGAINMax()/(FLOAT)pCTRL->stFILT.GAIN)*stAFTUNE.rFLTh+0.5; // max / GAIN ... stAFTUNE.rFLTh = 100
	rFLTh = MIN(MAX((SHORT)rate,stAFTUNE.rFLTh/2),stAFTUNE.rFLTh); //50~100

	rate = (AECTRL_GetGAINMax()/(FLOAT)pCTRL->stFILT.GAIN)*stAFTUNE.rFLTb+0.5;
	rFLTb = MIN(MAX((SHORT)rate,stAFTUNE.rFLTb/2),stAFTUNE.rFLTb); // 100~200

	rate = ((FLOAT)stAFAUTO.xFLTh/stAFAUTO.nFLTh-1)*stAFTUNE.rMUL2L;
	stAFAUTO.RATEh = ABS((LONG)((rate<0)? rate-0.5 : rate+0.5));

	rate = ((FLOAT)stAFAUTO.xFLTb/stAFAUTO.nFLTb-1)*stAFTUNE.rMUL2H;
	stAFAUTO.RATEb = ABS((LONG)((rate<0)? rate-0.5 : rate+0.5));

	if (stAFAUTO.RATEh<(LONG)rFLTh) { // 50 ~ 100
		stAFAUTO.CNTh  = 0;
	}
	if (stAFAUTO.RATEb<(LONG)rFLTb) { // 100 ~ 200
		stAFAUTO.CNTb  = 0;
	}
	if(afDebugMode == 3) UARTprintf("AFCORE_CtrlTriggerByNORM : stAUTO.xYSUM(%5d) / stAUTO.nYSUM(%5d) / nYSUM(%f) / RATEh(%5d:%2d) / RATEb(%5d:%2d) / stAFAUTO.CNTh=%5d / stAFAUTO.CNTb=%5d\r\n", stAFAUTO.xYSUM, stAFAUTO.nYSUM, nYSUM, stAFAUTO.RATEh, rFLTh, stAFAUTO.RATEb, rFLTb, stAFAUTO.CNTh, stAFAUTO.CNTb);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerByAUTO(PAF_CTRL_PARAM pCTRL) //FLT
{
	FLOAT rate;	SHORT rFLTh, rFLTb;

	if ((pCTRL->ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_MANUAL_MODE) return;
	if ((pCTRL->ctrl&AFCTRL_AFMODE_MASK)!=AFCTRL_AFAUTO_MODE) return;
	if (LENS_IsDriveEnableZOOM() || LENS_IsDriveEnableFOKS()) return;
	if (stAFAUTO.nIDX<NumOfBUFF) return;
	if (stAFAUTO.CNTh<stAFTUNE.rCNTh && stAFAUTO.CNTb<stAFTUNE.rCNTb) return; // 15

	rate = (AECTRL_GetGAINMax()/(FLOAT)pCTRL->stFILT.GAIN)*stAFTUNE.rFLTh+0.5; // (max/gain)*100 .... gain change ratio
	rFLTh = MIN(MAX((SHORT)rate,stAFTUNE.rFLTh/2),stAFTUNE.rFLTh); // 50~100

	rate = (AECTRL_GetGAINMax()/(FLOAT)pCTRL->stFILT.GAIN)*stAFTUNE.rFLTb+0.5;
	rFLTb = MIN(MAX((SHORT)rate,stAFTUNE.rFLTb/2),stAFTUNE.rFLTb); // 100~200

	stAFAUTO.CNTh = 0;	stAFAUTO.CNTb = 0; // initialise

	rate = ((FLOAT)stAFAUTO.xFLTh/stAFAUTO.nFLTh-1)*stAFTUNE.rMUL1L; // filter output change ratio
	stAFAUTO.RATEh = ABS((LONG)((rate<0)? rate-0.5 : rate+0.5));

	rate = ((FLOAT)stAFAUTO.xFLTb/stAFAUTO.nFLTb-1)*stAFTUNE.rMUL1L;
	stAFAUTO.RATEb = ABS((LONG)((rate<0)? rate-0.5 : rate+0.5));
	if(afDebugMode == 4) UARTprintf("AFCORE_CtrlTriggerByAUTO : stAFAUTO.RATEh(%5d:%d) / stAFAUTO.RATEb(%5d:%d)\r\n", stAFAUTO.RATEh, rFLTh, stAFAUTO.RATEb, rFLTb);

	if (stAFAUTO.RATEh<(LONG)rFLTh && stAFAUTO.RATEb<(LONG)rFLTb) return; // 50~100, 100~200 .... if (filter output ratio) < (gain change ratio)
	if(afDebugMode != 8) AF_EnableProcess(ON);
	if((afDebugMode != 0) && (afDebugMode != 8)) UARTprintf("AFCORE_CtrlTriggerByAUTO : AF_EnableProcess(ON) / stAFAUTO.RATEh(%5d:%d) / stAFAUTO.RATEb(%5d:%d)\r\n", stAFAUTO.RATEh, rFLTh, stAFAUTO.RATEb, rFLTb);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerByYSUM(PAF_CTRL_PARAM pCTRL) //ySUM
{
	SHORT rDIFFy = 5;
	SHORT rGAPy;

	if ((pCTRL->ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_MANUAL_MODE) return;
	if ((pCTRL->ctrl&AFCTRL_AFMODE_MASK)!=AFCTRL_AFAUTO_MODE) return;
	if (LENS_IsDriveEnableZOOM() || LENS_IsDriveEnableFOKS()) return;
	if (stAFAUTO.nIDX<NumOfBUFF) return;

	if (stAFAUTO.nGAIN==AECTRL_GetGAINMin()) stAFAUTO.CNTy = 0; // stAFAUTO.nGAIN = stGAIN.gain
	if (stAFAUTO.nGAIN==AECTRL_GetGAINMax())	rDIFFy = stAFTUNE.rDIFFyH; // 15
	else										rDIFFy = stAFTUNE.rDIFFyL; // 5

	stAFAUTO.yDIFF = ABS(stAFAUTO.oYSUM-stAFAUTO.nYSUM);
	if (stAFAUTO.yDIFF>=(LONG)rDIFFy) stAFAUTO.CNTy  = 0; // 5 or 15
	else							 stAFAUTO.CNTy += 1;
	if(afDebugMode == 5) UARTprintf("AFCORE_CtrlTriggerByYSUM : stAFAUTO.CNTy(%5d:%d) / stAFAUTO.oYSUM(%5d) / stAFAUTO.nYSUM(%5d) / stAFAUTO.yDIFF(%5d:%d)\r\n", stAFAUTO.CNTy, stAFTUNE.rCNTy, stAFAUTO.oYSUM, stAFAUTO.nYSUM, stAFAUTO.yDIFF, rDIFFy);
	
	if (stAFAUTO.CNTy<stAFTUNE.rCNTy)  return; // 15

	stAFAUTO.CNTy = 0;
	if (stAFAUTO.nGAIN==AECTRL_GetGAINMax())	rGAPy = stAFTUNE.rGAPyL;
	else										rGAPy = stAFTUNE.rGAPyH;

	stAFAUTO.yDIFF = ABS(stAFAUTO.xYSUM-stAFAUTO.nYSUM);
	if(afDebugMode == 5) UARTprintf("AFCORE_CtrlTriggerByYSUM : stAFAUTO.nGAIN(%5d) / stAUTO.xYSUM(%5d) / stAUTO.nYSUM(%5d) / stAFAUTO.yDIFF(%5d:%d)\r\n", stAFAUTO.nGAIN, stAFAUTO.xYSUM, stAFAUTO.nYSUM, stAFAUTO.yDIFF, rGAPy);
	if (stAFAUTO.yDIFF<(LONG)rGAPy) return; // 500, 1000
	if(afDebugMode != 7) AF_EnableProcess(ON);
	if((afDebugMode != 0) && (afDebugMode != 7)) UARTprintf("AFCORE_CtrlTriggerByYSUM : AF_EnableProcess(ON) / stAFAUTO.yDIFF(%5d:%d)\r\n", stAFAUTO.yDIFF, rGAPy);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerByPROC(PAF_CTRL_PARAM pCTRL)
{
	if (stAFAUTO.nRST==0 || AF_IsProcessExecuting()) return;
	stAFAUTO.nRST = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerByTIME(PAF_CTRL_PARAM pCTRL)
{
	if (LENS_IsDriveEnableZOOM()) pCTRL->nINT = 0;

	if ((pCTRL->ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_MANUAL_MODE) return;
	if ((pCTRL->ctrl&AFCTRL_AFMODE_MASK)!=AFCTRL_INTVAL_MODE) return;
	if (AF_IsProcessExecuting()==TRUE) return;

	if (pCTRL->nINT<pCTRL->tINT*GET_FPS) pCTRL->nINT++;
	if (pCTRL->nINT<pCTRL->tINT*GET_FPS) return;

	pCTRL->nINT = 0;	AF_EnableProcess(ON);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerBySTOP(PAF_CTRL_PARAM pCTRL)
{
	if (LENS_IsDriveEnableZOOM()) pCTRL->nACT = 0;

	if ((pCTRL->ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_MANUAL_MODE) return;
	if ((pCTRL->ctrl&AFCTRL_AFMODE_MASK)==AFCTRL_AFAUTO_MODE) return;
	if (AF_IsProcessExecuting()==FALSE) return;

	if (pCTRL->nACT<pCTRL->tACT*GET_FPS) pCTRL->nACT++;
	if (pCTRL->nACT<pCTRL->tACT*GET_FPS) return;

	pCTRL->nACT = 0;	AF_EnableProcess(OFF);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerBySPOT(PAF_CTRL_PARAM pCTRL)
{
	if (AF_IsProcessExecuting()==FALSE) return;
	if (AFSPOT_IsProcessExecuting()==TRUE) return;
	if (pCTRL->nSPT<stAFAUTO.Branch_SPOT_XX_Cnt) return;

	//AFNORM_EnableProcess(OFF);
	//AFDARK_EnableProcess(OFF);
	//AFSPOT_EnableProcess(ON);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_TriggerHandler(PAF_CTRL_PARAM pCTRL)
{
	if (LENS_IsDriveExecuting() || GetDrvOnOffPINT() || GetAgeOnOffLENS()) return;

	AFCORE_CtrlTriggerByZOOM(pCTRL); // by zoom
	AFCORE_CtrlTriggerByBUFF(pCTRL); 
	AFCORE_CtrlTriggerByLUMA(pCTRL);
	AFCORE_CtrlTriggerByDATA(pCTRL);
	AFCORE_CtrlTriggerByNORM(pCTRL);
	AFCORE_CtrlTriggerByAUTO(pCTRL);
	AFCORE_CtrlTriggerByYSUM(pCTRL);
	AFCORE_CtrlTriggerByPROC(pCTRL);
	AFCORE_CtrlTriggerByTIME(pCTRL);
	AFCORE_CtrlTriggerBySTOP(pCTRL);
	AFCORE_CtrlTriggerBySPOT(pCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_MtrMoveHandler(PAF_CTRL_PARAM pCTRL)
{
	if (LENS_IsDriveExecuting()==FALSE) return;

	LENS_DrvHandlerFOKS(pCTRL);
	LENS_DrvHandlerZOOM(pCTRL);
}

BYTE AF_TR_MODE = 0;
//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_BranchHandler(PAF_CTRL_PARAM pCTRL)
{
	FLOAT rate;	LONG yRATE;	PBYTE pCOEF;

	if (stAFAUTO.PROC==OFF) return;
	LENS_EnableMotorFOKS(ON);

	rate = ((FLOAT)pCTRL->stFILT.YSUM/AECTRL_GetBrightness()-1)*stAFTUNE.rMUL2CORE;
	yRATE = ABS((LONG)((rate*2<0)? rate*2-0.5 : rate*2+0.5));

	if (stAFAUTO.nCNT<30)	stAFAUTO.nCNT++; // only working within 30cnt after AF enable
	if (stAFAUTO.nCNT<30 && yRATE>stAFTUNE.rVALy) return; // rVALy = 200 // large yRATE means (otw AE or too bright or too dark)

	stAFAUTO.PROC = stAFAUTO.nCNT = 0;	pCTRL->xMOD = pCTRL->nMOD; // xMOD is pre

	//af mode select
	if (pCTRL->stFILT.GAIN > stAFAUTO.Branch_DARK_LL)		pCTRL->nMOD  = ( DARK_LL);
	else if (pCTRL->stFILT.GAIN > stAFAUTO.Branch_DARK_ML)	pCTRL->nMOD  = ( DARK_ML);
	else if (pCTRL->stFILT.GAIN > stAFAUTO.Branch_DARK_HL)	pCTRL->nMOD  = ( DARK_HL);
	else													pCTRL->nMOD  = ( NORM_XX);

	//spot on/off
	//if (pCTRL->nSPT<stAFAUTO.Branch_SPOT_XX_Cnt)		pCTRL->nMOD &= (~SPOT_XX);
	//else												pCTRL->nMOD |= ( SPOT_XX);

	if (pCTRL->nMOD&SPOT_XX)		AFSPOT_EnableProcess(ON);
	else if (pCTRL->nMOD==NORM_XX)	AFNORM_EnableProcess(ON);
	else								AFDARK_EnableProcess(ON);

	pCTRL->nINT = 0;	pCTRL->nACT = 0;

	if (LO4BIT(pCTRL->nMOD)==LO4BIT(pCTRL->xMOD))	return; //if same .. return

	//filter select
	if		(LO4BIT(pCTRL->nMOD)==NORM_XX)	pCOEF = (PBYTE)stAFHBPF.HI;
	else if	(LO4BIT(pCTRL->nMOD)==DARK_HL)	pCOEF = (PBYTE)stAFHBPF.HI;
	else											pCOEF = (PBYTE)stAFHBPF.LO;

	MISP_MultiWrite(MISP_LOCAL_ID, 0x6b0, pCOEF, sizeof(stAFHBPF.HI)); // Filter BPF
}

//--------------------------------------------------------------------------------------------------------------------------
void AF_ProcessHandler(PAF_CTRL_PARAM pCTRL)
{
	AFCORE_GetAFOPDWIND(pCTRL); // get AF data

	AFCORE_TriggerHandler(pCTRL); // get AF Trigger 
	AFCORE_MtrMoveHandler(pCTRL);	// get AF move point

	AFNORM_ProcessHandler(pCTRL); // normal AF process
	AFDARK_ProcessHandler(pCTRL); // Dark AF process
	AFSPOT_ProcessHandler(pCTRL); // Spot AF process

	AFCORE_BranchHandler(pCTRL); // select AF process
}

//--------------------------------------------------------------------------------------------------------------------------
void AF_DisplayHandler(PAF_CTRL_PARAM pCTRL)
{
	WORD rODM[3], rVal[4], wVal[8];

	MISP_RegRead(MISP_LOCAL_ID, 0x752, &rODM[0]); //odm af window display enable
	if((rODM[0]&0x1f) == 0) return;

	MISP_MultiRead(MISP_LOCAL_ID, 0x60d, (PBYTE)&rODM, 6);
	MISP_MultiRead(MISP_LOCAL_ID, 0x6a0, (PBYTE)&rVal, 8);

	wVal[0] = rODM[0]+HIBYTE(rVal[0])*HIBYTE(rODM[2]);
	wVal[1] = rODM[1]+LOBYTE(rVal[0])*LOBYTE(rODM[2]);
	wVal[2] = wVal[0]+HIBYTE(rVal[1])*HIBYTE(rODM[2]);
	wVal[3] = wVal[1]+LOBYTE(rVal[1])*LOBYTE(rODM[2]);

	wVal[4] = rODM[0]+HIBYTE(rVal[2])*HIBYTE(rODM[2]);
	wVal[5] = rODM[1]+LOBYTE(rVal[2])*LOBYTE(rODM[2]);
	wVal[6] = wVal[4]+HIBYTE(rVal[3])*HIBYTE(rODM[2]);
	wVal[7] = wVal[5]+LOBYTE(rVal[3])*LOBYTE(rODM[2]);

	MISP_MultiWrite(MISP_LOCAL_ID, 0x75f, (PBYTE)wVal, 16);
}

//--------------------------------------------------------------------------------------------------------------------------
void AF_EnableProcess(BOOL OnOff)
{
	stAFAUTO.nCNT = 0;	stAFAUTO.nIDX = 0;
	stAFAUTO.CNTh = 0;	stAFAUTO.CNTb = 0;
	stAFAUTO.CNTy = 0;

	if (OnOff==ON)	stAFAUTO.PROC = ON;
	if (OnOff==ON)	return;

	AFNORM_EnableProcess(OFF);
	AFDARK_EnableProcess(OFF);
	AFSPOT_EnableProcess(OFF);
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AF_IsProcessExecuting(void)
{
	if (AFNORM_IsProcessExecuting()) return TRUE;
	if (AFDARK_IsProcessExecuting()) return TRUE;
	if (AFSPOT_IsProcessExecuting()) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
ISPM BOOL AF_IsProcessRESET(void)
{
	return (fSysStateCHANGE)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void AF_SetAUTOInfo(PAF_CTRL_PARAM pCTRL, BOOL nCHK) // set AF auto information end of AUTO AF
{
	stAFAUTO.xFLTr = (nCHK)? pCTRL->stFILT.RPFb : 0;
	stAFAUTO.xFLTh = pCTRL->stFILT.HPFb;
	stAFAUTO.xFLTb = pCTRL->stFILT.BPFb;
	stAFAUTO.xYSUM = pCTRL->stFILT.YSUM;
	stAFAUTO.xGAIN = pCTRL->stFILT.GAIN;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AF_GetLUMAInfo(PAF_CTRL_PARAM pCTRL, WORD nID)
{
	// init AF
	PAF_FILT_PARAM pFILT = &pCTRL->stFILT; // Get Filter value
	LONG diff = (LONG)(pFILT->YSUM-pFILT->YPRV); // YSUM diff
	LONG yGAP = (LONG)(stAFTUNE.rGAPyH); // 2 times GAP // 500*2

	if (HIBYTE(nID)==TRUE)	stAFAUTO.nRST = TRUE;

	if (diff<=-yGAP && pFILT->YPRV<AECTRL_GetBrightness()) // YPRV is smaller than REF &&  YSUM < YPRV - TH 
		stAFAUTO.nRST = TRUE;

	if (diff>= yGAP && pFILT->YPRV>AECTRL_GetBrightness()) // YPRV is larger then REF && YSUM > YPRV + TH
		stAFAUTO.nRST = TRUE;

	return (stAFAUTO.nRST)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AF_GetDATAInfo(PAF_CTRL_PARAM pCTRL, BOOL nID)
{
	if (nID==TRUE)	stAFAUTO.nRST = TRUE;
	return (stAFAUTO.nRST)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void AF_SetTrMode(BYTE val)
{
	AF_TR_MODE = val;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE AF_GetTrMode(void)
{
	return AF_TR_MODE;
}

//--------------------------------------------------------------------------------------------------------------------------
LONG AF_GetFILTVal(PAF_CTRL_PARAM pCTRL, BYTE nID)
{
	if		(nID==0)	return pCTRL->stFILT.RPFa;
	else if (nID==1)	return pCTRL->stFILT.VPFa;
	else if (nID==2)	return pCTRL->stFILT.BPFa;
	else				return pCTRL->stFILT.HPFa;
}

#if __MISP100_AF_CNV__ == 1
//--------------------------------------------------------------------------------------------------------------------------
void AF_ConvertHandler(PAF_CTRL_PARAM pCTRL)
{
	WORD i, *pLUT = (PWORD)AF_ConvLUT;

	if ((AF_CtrlLUT&1)==0) return;

	if ((AF_CtrlLUT&2)==0) {	// write data
		MISP_RegWrite(MISP_LOCAL_ID, 0x6e0, 0x0000);	//
		MISP_RegWrite(MISP_LOCAL_ID, 0x6e0, 0x0001);	//

		for (i=0; i<128; i++) {
			MISP_RegWrite(MISP_LOCAL_ID, 0x6e1, pLUT[i]);	//
		}
	}
	else {
		MISP_RegWrite(MISP_LOCAL_ID, 0x6e0, 0x0002);	//
		MISP_RegWrite(MISP_LOCAL_ID, 0x6e0, 0x0003);	//

		for (i=0; i<128; i++) {
			MISP_RegRead(MISP_LOCAL_ID, 0x6e2, &pLUT[i]);	//
		}
	}

	AF_CtrlLUT = 0;
}
#endif	/* __MISP100_AF_CNV__ == 1 */
#endif	/* defined(__USE_AFZOOM_LENS__) && defined(__USE_X30X_TAMRON__) */

/*  FILE_END_HERE */
