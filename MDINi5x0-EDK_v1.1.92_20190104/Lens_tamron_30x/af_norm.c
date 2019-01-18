// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if defined(__USE_AFZOOM_LENS__) && defined(__USE_X30X_TAMRON__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

#define num_xCNT	3

typedef	struct
{
	LONG	nFltBUFF[5];
	SHORT	nDIFF[4];
	CHAR	nSIGN[4];

	SHORT	nFOKS[5];
	SHORT	nMOVE;	

}	AFN_BUFF_PARAM, *PAFN_BUFF_PARAM;

typedef	struct
{
	//same from here #1
	LONG	nMIN; // filter min
	LONG	nMAX;
	LONG	nINC;
	LONG	nDEC;

	LONG	zMIN;
	LONG	zMAX;
	LONG	zINC;
	LONG	zDEC;

	SHORT	n_UP;
	SHORT	n_DN;
	SHORT	z_UP;
	SHORT	z_DN;

	LONG	nCHKf;
	//same end
}	AFN_RATE_PARAM, *PAFN_RATE_PARAM;

typedef	struct
{
	LONG	nHIGH;
	SHORT	nPEAK;
	SHORT	nDUMY;

}	AFN_PEAK_PARAM, *PAFN_PEAK_PARAM;

typedef	struct
{
	LONG	YSUM;
	LONG	YPRV;
	LONG	YREF;
	LONG	GAIN;
	LONG	YCNT;
	LONG	FLTb;

	BYTE	nCHK;
	BYTE	nRST;
	BYTE	nERR;
	BYTE	nCNT;

	LONG	nGain_Lm; //1023
	LONG	nGain_Mm; //8191
	LONG	nGain_Hm; //16383
	LONG	nGain_L; //1024
	LONG	nGain_M; //8192
	LONG	nGain_H; //16384
}	AFN_NORM_PARAM, *PAFN_NORM_PARAM;

typedef	struct
{
	LONG	FLTa;
	LONG	FLTb;
	LONG	FLTo;
	LONG	FLTn;

}	AFN_DATA_PARAM, *PAFN_DATA_PARAM;

typedef	struct
{
	LONG	FLTn;
	LONG	FLTd;
	LONG	FLTx;
	LONG	FLTr;

	LONG	nFltBUFF[5];
	LONG	nDIFF[4];
	CHAR	nSIGN[4];
	SHORT	nFOKS[5];
	SHORT	nMOVE;
	// same from here #1
	LONG	nMIN; // filter min
	LONG	nMAX;
	LONG	nINC;
	LONG	nDEC;

	LONG	zMIN;
	LONG	zMAX;
	LONG	zINC;
	LONG	zDEC;

	SHORT	n_UP;
	SHORT	n_DN;
	SHORT	z_UP;
	SHORT	z_DN;

	LONG	nCHKf;
	// same end

	LONG	nHIGH;
	LONG	xHIGH;

	SHORT	nPEAK;
	SHORT	xPEAK;

}	AFN_FILT_PARAM, *PAFN_FILT_PARAM;

typedef	struct
{
	BYTE	xMIN;
	BYTE	xSTD;
	BYTE	xMID;
	BYTE	xMAX;
	BYTE	xFWD;
	BYTE	xBWD;
	BYTE	xTOP;
	BYTE	xDMY;

}	AFN_STEP_COUNT, *PAFN_STEP_COUNT;

typedef	struct
{
	BYTE	sMIN; // step min
	BYTE	sSTD;
	BYTE	sMID;
	BYTE	sMAX;

	BYTE	sFWD;
	BYTE	sBWD;
	BYTE	sTOP;

	BYTE	sCNT;

	BYTE	sSPD;
	BYTE	sCHK;

	AFN_STEP_COUNT	stDIR_I;
	AFN_STEP_COUNT	stDEC_I;
	AFN_STEP_COUNT	stDEC_D;

}	AFN_STEP_PARAM, *PAFN_STEP_PARAM;

typedef	struct
{
	BYTE	nSEL;
	BYTE	nDIR;
	BYTE	eCNT; // end counter
	BYTE	xCNT; // buffer counter
	BYTE	vCNT; // hpf and change speed

	BYTE	cur;
	BYTE	prv;
	BYTE	nxt;

	WORD	step;
	SHORT	goal;
	SHORT	peak;
	SHORT	fVAL;
	SHORT	fMAX;
	SHORT	fMIN;
	SHORT	fMID;
	SHORT	fGAP;
	SHORT	fMOV;
	SHORT	zVAL;
	SHORT	ZOOM;
	SHORT	KNEE;
	WORD	time;
	WORD	nDMY;

	AFN_NORM_PARAM	stNORM_y;
	AFN_DATA_PARAM	stDATA_r;
	AFN_DATA_PARAM	stDATA_l;
	AFN_DATA_PARAM	stDATA_v;
	AFN_DATA_PARAM	stDATA_b;
	AFN_DATA_PARAM	stDATA_h;
	AFN_FILT_PARAM	stOUTn_b;
	AFN_FILT_PARAM	stOUTn_h;
	AFN_STEP_PARAM	stSTEP_m;

}	AFN_PROC_PARAM, *PAFN_PROC_PARAM;

typedef enum {
	PRE_FILTER_SEL = 0x10, DRV_FILTER_SEL, INV_FILTER_SEL, END_FILTER_SEL,
	PRE_SEARCH_MOV = 0x20, DRV_SEARCH_MOV, INV_SEARCH_MOV, END_SEARCH_MOV,
	PRE_SEARCH_FAR = 0x30, DRV_SEARCH_FAR, INV_SEARCH_FAR, END_SEARCH_FAR,
	PRE_SEARCH_NAR = 0x40, DRV_SEARCH_NAR, INV_SEARCH_NAR, END_SEARCH_NAR,
	PRE_SEARCH_DIR = 0x50, DRV_SEARCH_DIR, INV_SEARCH_DIR, END_SEARCH_DIR,
	PRE_CLIMB_TOP1 = 0x60, DRV_CLIMB_TOP1, INV_CLIMB_TOP1, END_CLIMB_TOP1,
	PRE_CLIMB_TOP2 = 0x70, DRV_CLIMB_TOP2, INV_CLIMB_TOP2, END_CLIMB_TOP2,
	PRE_CLIMB_PEAK = 0x80, DRV_CLIMB_PEAK, INV_CLIMB_PEAK, END_CLIMB_PEAK,
	PRE_FORCE_PEAK = 0x90, DRV_FORCE_PEAK, INV_FORCE_PEAK, END_FORCE_PEAK,
	PRE_SWING_MOVE = 0xA0, DRV_SWING_MOVE, INV_SWING_MOVE, END_SWING_MOVE,
	PRE_SWING_SCAN = 0xB0, DRV_SWING_SCAN, INV_SWING_SCAN, END_SWING_SCAN,
	END_CLEAR_PROC = 0x00, END_CLEAR_AUTO, END_CLEAR_WAIT

} 	AFN_PROCESS_STEP;

typedef enum {
	USE_HPF = 0, USE_LPF, USE_BPF

} 	AFN_PROCESS_FILT;

typedef enum {
	GOOD_DEC = 0x000008, G_DEC_OK = 0x000004, G_INC_OK = 0x000002, GOOD_INC = 0x000001,
	ZERO_DEC = 0x000080, ZERO_INC = 0x000010,
	RATE_DEC = 0x000800, R_DEC_OK = 0x000400, R_INC_OK = 0x000200, RATE_INC = 0x000100,										 
	HIGH_DEC = 0x008000, HIGH_INC = 0x001000,
	GAIN_MAX = 0x040000, GAIN_MID = 0x020000, GAIN_FLT = 0x010000,
	GOOD_END = 0x800000

} 	AFN_STATUS_RATE;

typedef	struct
{
	SHORT	n_UP;
	SHORT	n_DN;
	SHORT	z_UP;
	SHORT	z_DN;

}	AFN_TUNE_RATIO, *PAFN_TUNE_RATIO;

typedef	struct
{
	BYTE	r_MIN;
	BYTE	r_STD;
	BYTE	r_MID;
	BYTE	r_MAX;

}	AFN_TUNE_COUNT, *PAFN_TUNE_COUNT;

typedef	struct
{
	LONG	n_GAIN;	
	LONG	n_YSUM;	
	LONG	n_FLTr;	

	LONG	c_FLTb;	
	LONG	c_FLTh;	
	LONG	c_GMAX;	

	SHORT	h_ZOOM;	
	SHORT	h_FOKS;	

	AFN_TUNE_RATIO	stPRV_b;
	AFN_TUNE_RATIO	stPRV_h;

	AFN_TUNE_RATIO	stDIR_b;
	AFN_TUNE_RATIO	stDIR_h;

	AFN_TUNE_RATIO	stDEC_b;
	AFN_TUNE_RATIO	stDEC_h;

	AFN_TUNE_RATIO	stTOP_b;
	AFN_TUNE_RATIO	stTOP_h;

}	AFN_TUNE_PARAM, *PAFN_TUNE_PARAM;

#define		MIN3(a,b,c)				(MIN(MIN(a,b),c))
#define		MAX3(a,b,c)				(MAX(MAX(a,b),c))

#define		MIN4(a,b,c,d)			(MIN(MIN(a,b),MIN(c,d)))
#define		MAX4(a,b,c,d)			(MAX(MAX(a,b),MAX(c,d)))

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static AFN_PROC_PARAM stAFNORM;
static AFN_TUNE_PARAM stAFNTUNE = {
	//n_GAIN	n_YSUM	n_FLTr	c_FLTb	c_FLTh	c_GMAX	
	     0,		0,		0,		500,		2048,	65535,
	//h_ZOOM	h_FOKS	
	     0,      0,
	{  50,    -50,      1,     -1}, // stPRV_b 	-- n_UP; n_DN, z_UP, z_DN
	{  50,    -50,      1,     -1}, // stPRV_h
	{  50,    -50,      1,     -5}, // stDIR_b
	{  50,    -50,      1,     -5}, // stDIR_h
	{  50,    -50,      1,     -5}, // stDEC_b
	{  50,    -50,      1,     -5}, // stDEC_h
	{  20,    -20,      1,     -1}, // stTOP_b
	{  20,     -5,      1,     -1} // stTOP_h
};

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
static void AFNORM_GetAFOPDWIND(PAF_CTRL_PARAM pCTRL)
{
	FLOAT nGAIN, nYSUM, nHPFr;

	stAFNTUNE.c_GMAX = AECTRL_GetGAINMax()/2 - 1; // mimitest

	stAFNORM.stDATA_r.FLTa = (pCTRL->stFILT.RPFa)>>0; //hpfa // window a is bigger than window b
	stAFNORM.stDATA_v.FLTa = (pCTRL->stFILT.VPFa)>>0; // vpfa
	stAFNORM.stDATA_b.FLTa = (pCTRL->stFILT.BPFa)>>0; //vara 
	stAFNORM.stDATA_r.FLTb = (pCTRL->stFILT.RPFb)<<2; // window a is bigger than window b
	stAFNORM.stDATA_v.FLTb = (pCTRL->stFILT.VPFb)<<2;
	stAFNORM.stDATA_b.FLTb = (pCTRL->stFILT.BPFb)<<2;

	stAFNORM.stNORM_y.YCNT = (pCTRL->stFILT.CNTa)>>0;
	stAFNORM.stDATA_h.FLTa = (pCTRL->stFILT.HPFa)>>0; //bpfa
	stAFNORM.stDATA_h.FLTb = (pCTRL->stFILT.HPFb)<<2;

	stAFNORM.stDATA_r.FLTo = stAFNORM.stDATA_r.FLTa; //hpf
	stAFNORM.stDATA_b.FLTo = stAFNORM.stDATA_b.FLTa+stAFNORM.stDATA_v.FLTa; // var + vpf
	stAFNORM.stDATA_h.FLTo = stAFNORM.stDATA_h.FLTa+stAFNORM.stDATA_v.FLTa; // bpf + vpf

	stAFNORM.stNORM_y.YPRV = (stAFNORM.stNORM_y.YSUM); // prev
	stAFNORM.stNORM_y.YSUM = (pCTRL->stFILT.YSUM)>>0;
	stAFNORM.stNORM_y.GAIN = (pCTRL->stFILT.GAIN)>>0;
	stAFNORM.stNORM_y.YREF = (AECTRL_GetBrightness());

	stAFNORM.stNORM_y.nGain_Lm = AECTRL_GetGAINMax()/64-1; //1024*2-1
	stAFNORM.stNORM_y.nGain_Mm =AECTRL_GetGAINMax()/8-1; //8192*2-1
	stAFNORM.stNORM_y.nGain_Hm = AECTRL_GetGAINMax()/4-1; //16384*2-1

	stAFNORM.stNORM_y.nGain_L = AECTRL_GetGAINMax()/64; //1024*2
	stAFNORM.stNORM_y.nGain_M =AECTRL_GetGAINMax()/8; //8192*2
	stAFNORM.stNORM_y.nGain_H = AECTRL_GetGAINMax()/4; //16384*2

	nGAIN = ((FLOAT)AECTRL_GetGAINMin())/stAFNORM.stNORM_y.GAIN; // 256 / gain(256~)  = smaller than 1

	if (stAFNORM.stNORM_y.GAIN>stAFNTUNE.c_GMAX) // 65535
		 nYSUM = ((FLOAT)stAFNORM.stNORM_y.YSUM)/stAFNORM.stNORM_y.YREF; // almost  = 1
	else nYSUM = ((FLOAT)stAFNORM.stNORM_y.YREF)/stAFNORM.stNORM_y.YSUM; // under expose condition  = over 1

	stAFNORM.stNORM_y.FLTb = (LONG)(nGAIN*nYSUM*stAFNORM.stDATA_b.FLTo+0.5);

	if (stAFNTUNE.n_GAIN==0) stAFNTUNE.n_GAIN = stAFNORM.stNORM_y.GAIN;
	nGAIN = ((FLOAT)stAFNTUNE.n_GAIN)/stAFNORM.stNORM_y.GAIN; // n_GAIN = 0  when AFNORM_PreFilterSEL() -- for same waiting during processing(normalize)

	if (stAFNTUNE.n_YSUM==0) stAFNTUNE.n_YSUM = stAFNORM.stNORM_y.YSUM;
	nYSUM = ((FLOAT)stAFNTUNE.n_YSUM)/stAFNORM.stNORM_y.YSUM; // n_YSUM = 0 when AFNORM_PreFilterSEL()
	stAFNORM.stDATA_h.FLTo += stAFNORM.stDATA_h.FLTb+stAFNORM.stDATA_v.FLTb;  // bpfa + vpfa + bpfb + vpfb

	stAFNORM.stDATA_r.FLTn = (LONG)(nGAIN*nYSUM*stAFNORM.stDATA_r.FLTo+0.5);
	stAFNORM.stDATA_b.FLTn = (LONG)(nGAIN*nYSUM*stAFNORM.stDATA_b.FLTo+0.5);
	stAFNORM.stDATA_h.FLTn = (LONG)(nGAIN*nYSUM*stAFNORM.stDATA_h.FLTo+0.5);

	if (stAFNORM.stNORM_y.GAIN>stAFNTUNE.c_GMAX && stAFNORM.stNORM_y.FLTb<stAFNTUNE.c_FLTb) // gain > 65535 && FLTb > 500
		 stAFNORM.stNORM_y.nCHK = 1;
	else stAFNORM.stNORM_y.nCHK = 0;

	if (stAFNORM.stNORM_y.nCHK && stAFNTUNE.n_FLTr==0) stAFNTUNE.n_FLTr = stAFNORM.stDATA_r.FLTn; // n_FLTr = 0 when AFNORM_PreFilterSEL()

	nHPFr = (stAFNORM.stNORM_y.nCHK)? ((FLOAT)stAFNTUNE.n_FLTr)/stAFNORM.stDATA_r.FLTn : 1.0f;

	stAFNORM.stOUTn_b.FLTn = (LONG)(nHPFr*stAFNORM.stDATA_b.FLTn+0.5);
	stAFNORM.stOUTn_h.FLTn = (LONG)(nHPFr*stAFNORM.stDATA_h.FLTn+0.5);

	stAFNORM.nDIR = LENS_GetDirectionFOKS();
	stAFNORM.fVAL = LENS_GetCurPositionFOKS();
	stAFNORM.zVAL = LENS_GetCurPositionZOOM();
	stAFNORM.ZOOM = LENS_GetRelPositionZOOM();
	stAFNORM.KNEE = LENS_GetKneePositionZOOM();
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFNORM_IsDriveEnd(void)
{
	if (stAFNORM.nDIR==fNEAR && stAFNORM.fVAL<=stAFNORM.fMIN) return TRUE;
	if (stAFNORM.nDIR== fFAR && stAFNORM.fVAL>=stAFNORM.fMAX) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFNORM_SetChangeDIR(void)
{
	stAFNORM.nDIR = (stAFNORM.nDIR==fNEAR)? fFAR : fNEAR;
	LENS_SetDirectionFOKS(stAFNORM.nDIR);
	return stAFNORM.nDIR;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_GetDriveSTEP(BYTE nID)
{
	WORD gap = (WORD)ABS(stAFNORM.peak-stAFNORM.fVAL);
	WORD quo = (WORD)ABS(stAFNORM.peak-stAFNORM.fVAL)/stAFNORM.step;
	WORD rem = (WORD)ABS(stAFNORM.peak-stAFNORM.fVAL)%stAFNORM.step;
	WORD max = (WORD)stAFNORM.stSTEP_m.sBWD;
	WORD min = (WORD)stAFNORM.stSTEP_m.sMIN;

	switch (nID) {
		case DRV_STEP_MIN:	stAFNORM.step = (WORD)stAFNORM.stSTEP_m.sMIN; break;
		case DRV_STEP_STD:	stAFNORM.step = (WORD)stAFNORM.stSTEP_m.sSTD; break;
		case DRV_STEP_MID:	stAFNORM.step = (WORD)stAFNORM.stSTEP_m.sMID; break;
		case DRV_STEP_MAX:	stAFNORM.step = (WORD)stAFNORM.stSTEP_m.sMAX; break;

		case DRV_STEP_FWD:	stAFNORM.step = (WORD)stAFNORM.stSTEP_m.sFWD; break;
		case DRV_STEP_BWD:	stAFNORM.step = (WORD)stAFNORM.stSTEP_m.sBWD; break;
		case DRV_STEP_TOP:	stAFNORM.step = (WORD)stAFNORM.stSTEP_m.sTOP; break;

		case DRV_STEP_DIV:	stAFNORM.step = MIN(MAX((gap+3)/4,min), max); break;
		case DRV_STEP_REM:	stAFNORM.step = (quo)? stAFNORM.step : rem; break;
	}

	if ((nID&0xf0)==DRV_STEP_MIN) stAFNORM.stSTEP_m.sSPD = nID;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_GetDriveRATE(BYTE nID)
{
	PBYTE pTUNEb = NULL, pTUNEh = NULL;

	switch (nID) {
		case DRV_SEARCH_FAR: case PRE_SEARCH_FAR:
		case DRV_SEARCH_NAR: case PRE_SEARCH_NAR:
			pTUNEb = (PBYTE)&stAFNTUNE.stPRV_b;	pTUNEh = (PBYTE)&stAFNTUNE.stPRV_h;	break; // 50 -50 1 -1

		case DRV_SEARCH_DIR: case PRE_SEARCH_DIR:
			pTUNEb = (PBYTE)&stAFNTUNE.stDIR_b;	pTUNEh = (PBYTE)&stAFNTUNE.stDIR_h;	break; // 50 -50 1 -5

		case DRV_CLIMB_TOP2: case PRE_CLIMB_TOP2:
			pTUNEb = (PBYTE)&stAFNTUNE.stDEC_b;	pTUNEh = (PBYTE)&stAFNTUNE.stDEC_h;	break; // 50 -50 1 -5

		case DRV_CLIMB_PEAK: case PRE_CLIMB_PEAK:
			pTUNEb = (PBYTE)&stAFNTUNE.stTOP_b;	pTUNEh = (PBYTE)&stAFNTUNE.stTOP_h;	break; // 20 -20 1 -1 / 20 -5 1 -1
	}

	memcpy((PBYTE)&stAFNORM.stOUTn_b.n_UP, (PBYTE)pTUNEb, sizeof(AFN_TUNE_RATIO));
	memcpy((PBYTE)&stAFNORM.stOUTn_h.n_UP, (PBYTE)pTUNEh, sizeof(AFN_TUNE_RATIO));
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_SetDriveSTEP(BYTE nID)
{
	stAFNORM.goal = stAFNORM.fVAL;

	if (stAFNORM.nDIR==fFAR) stAFNORM.goal += stAFNORM.step;
	else					 stAFNORM.goal -= stAFNORM.step;

	stAFNORM.goal = MIN(stAFNORM.goal, stAFNORM.fMAX);
	stAFNORM.goal = MAX(stAFNORM.goal, stAFNORM.fMIN);

	LENS_SetDriveFOKS(stAFNORM.goal);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_SetDrivePEAK(void)
{
	stAFNORM.goal = stAFNORM.peak;

	stAFNORM.goal = MIN(stAFNORM.goal, stAFNORM.fMAX);
	stAFNORM.goal = MAX(stAFNORM.goal, stAFNORM.fMIN);

	LENS_SetDriveFOKS(stAFNORM.goal);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_ClrDrvCountSPD(PAFN_STEP_COUNT pCNT)
{
	SHORT sFWD = LENS_GetAFDriveSTEP(DRV_STEP_FWD);
	LONG  GAIN = stAFNORM.stNORM_y.GAIN, KNEE = MIN(GAIN,stAFNORM.stNORM_y.nGain_M);

	if (GAIN>stAFNORM.stNORM_y.nGain_Lm) sFWD -= (SHORT)(6.0f*(FLOAT)KNEE/stAFNORM.stNORM_y.nGain_L+0.5);
	if (GAIN>stAFNORM.stNORM_y.nGain_Mm) sFWD -= (SHORT)(6.0f*(FLOAT)GAIN/stAFNORM.stNORM_y.nGain_H+0.5);
	stAFNORM.stSTEP_m.sFWD = MAX(sFWD,stAFNORM.stSTEP_m.sSTD*4);

	switch (stAFNORM.stSTEP_m.sSPD) {
		case DRV_STEP_MIN:	pCNT->xSTD = pCNT->xMID = pCNT->xMAX = pCNT->xFWD = 0; break;
		case DRV_STEP_STD:	pCNT->xMIN = pCNT->xMID = pCNT->xMAX = pCNT->xFWD = 0; break;
		case DRV_STEP_MID:	pCNT->xMIN = pCNT->xSTD = pCNT->xMAX = pCNT->xFWD = 0; break;
		case DRV_STEP_MAX:	pCNT->xMIN = pCNT->xSTD = pCNT->xMID = pCNT->xFWD = 0; break;
		case DRV_STEP_FWD:	pCNT->xMIN = pCNT->xSTD = pCNT->xMID = pCNT->xMAX = 0; break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DirDrvIncSPD_BI(BYTE nID)
{
	PAFN_STEP_COUNT pCNT = &stAFNORM.stSTEP_m.stDIR_I;

	AFNORM_ClrDrvCountSPD(pCNT);

	switch (stAFNORM.stSTEP_m.sSPD) {
		case DRV_STEP_MIN:		AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;

		case DRV_STEP_STD:		if (pCNT->xSTD<6) pCNT->xSTD++;
			if (pCNT->xSTD==6)	AFNORM_GetDriveSTEP(DRV_STEP_MID);
			else				AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;

		case DRV_STEP_MID:		if (pCNT->xMID<3) pCNT->xMID++;
			if (pCNT->xMID==3)	AFNORM_GetDriveSTEP(DRV_STEP_MAX);
			else				AFNORM_GetDriveSTEP(DRV_STEP_MID);	break;

		case DRV_STEP_MAX:		if (pCNT->xMAX<2) pCNT->xMAX++;
			if (pCNT->xMAX==2)	AFNORM_GetDriveSTEP(DRV_STEP_FWD);
			else				AFNORM_GetDriveSTEP(DRV_STEP_MAX);	break;

		case DRV_STEP_FWD:		AFNORM_GetDriveSTEP(DRV_STEP_FWD);	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DecDrvIncSPD_GX(BYTE nID)
{
	PAFN_STEP_COUNT pCNT = &stAFNORM.stSTEP_m.stDEC_I;

	AFNORM_ClrDrvCountSPD(pCNT);
	memset(&stAFNORM.stSTEP_m.stDEC_D, 0, sizeof(AFN_STEP_COUNT));

	if (stAFNORM.stSTEP_m.sSPD==DRV_STEP_MAX) stAFNORM.stSTEP_m.sCHK = 1;
	if (stAFNORM.stSTEP_m.sSPD==DRV_STEP_FWD) stAFNORM.stSTEP_m.sCHK = 1;

	if (stAFNORM.stSTEP_m.sCHK) {
		switch (stAFNORM.stSTEP_m.sSPD) {
			case DRV_STEP_MIN:		AFNORM_GetDriveSTEP(DRV_STEP_MIN);	break;

			case DRV_STEP_STD:		AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		AFNORM_GetDriveSTEP(DRV_STEP_MID);	break;

			case DRV_STEP_MAX:		if (pCNT->xMAX<4) pCNT->xMAX++;
				if (pCNT->xMAX==4)	AFNORM_GetDriveSTEP(DRV_STEP_MID);
				else				AFNORM_GetDriveSTEP(DRV_STEP_MAX);	break;

			case DRV_STEP_FWD:		AFNORM_GetDriveSTEP(DRV_STEP_MAX);	break;
		}
	}
	else {
		switch (stAFNORM.stSTEP_m.sSPD) {
			case DRV_STEP_MIN:		AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_STD:		if (pCNT->xSTD<4) pCNT->xSTD++;
				if (pCNT->xSTD==4)	AFNORM_GetDriveSTEP(DRV_STEP_MID);
				else				AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		AFNORM_GetDriveSTEP(DRV_STEP_MID);	break;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DecDrvIncSPD_BI(BYTE nID)
{
	PAFN_STEP_COUNT pCNT = &stAFNORM.stSTEP_m.stDEC_I;

	AFNORM_ClrDrvCountSPD(pCNT);
	memset(&stAFNORM.stSTEP_m.stDEC_D, 0, sizeof(AFN_STEP_COUNT));

	switch (stAFNORM.stSTEP_m.sSPD) {
		case DRV_STEP_MIN:		if (pCNT->xMIN<4) pCNT->xMIN++;
			if (pCNT->xMIN==4)	AFNORM_GetDriveSTEP(DRV_STEP_STD);
			else				AFNORM_GetDriveSTEP(DRV_STEP_MIN);	break;

		case DRV_STEP_STD:		if (pCNT->xSTD<3) pCNT->xSTD++;
			if (pCNT->xSTD==3)	AFNORM_GetDriveSTEP(DRV_STEP_MID);
			else				AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;

		case DRV_STEP_MID:		if (pCNT->xMID<2) pCNT->xMID++;
			if (pCNT->xMID==2)	AFNORM_GetDriveSTEP(DRV_STEP_MAX);
			else				AFNORM_GetDriveSTEP(DRV_STEP_MID);	break;

		case DRV_STEP_MAX:		AFNORM_GetDriveSTEP(DRV_STEP_FWD);	break;

		case DRV_STEP_FWD:		AFNORM_GetDriveSTEP(DRV_STEP_FWD);	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DecDrvDecSPD_GD(BYTE nID)
{
	PAFN_STEP_COUNT pCNT = &stAFNORM.stSTEP_m.stDEC_D;

	AFNORM_ClrDrvCountSPD(pCNT);
	memset(&stAFNORM.stSTEP_m.stDEC_I, 0, sizeof(AFN_STEP_COUNT));

	if (stAFNORM.stSTEP_m.sSPD==DRV_STEP_MAX) stAFNORM.stSTEP_m.sCHK = 1;
	if (stAFNORM.stSTEP_m.sSPD==DRV_STEP_FWD) stAFNORM.stSTEP_m.sCHK = 1;

	if (stAFNORM.stSTEP_m.sCHK) {
		switch (stAFNORM.stSTEP_m.sSPD) {
			case DRV_STEP_MIN:		AFNORM_GetDriveSTEP(DRV_STEP_MIN);	break;

			case DRV_STEP_STD:		if (pCNT->xSTD<4) pCNT->xSTD++;
				if (pCNT->xSTD==4)	AFNORM_GetDriveSTEP(DRV_STEP_MIN);
				else				AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MAX:		AFNORM_GetDriveSTEP(DRV_STEP_MID);	break;

			case DRV_STEP_FWD:		AFNORM_GetDriveSTEP(DRV_STEP_MAX);	break;
		}
	}
	else {
		switch (stAFNORM.stSTEP_m.sSPD) {
			case DRV_STEP_MIN:		AFNORM_GetDriveSTEP(DRV_STEP_MIN);	break;

			case DRV_STEP_STD:		if (pCNT->xSTD<4) pCNT->xSTD++;
				if (pCNT->xSTD==4)	AFNORM_GetDriveSTEP(DRV_STEP_MIN);
				else				AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		if (pCNT->xMID<2) pCNT->xMID++;
				if (pCNT->xMID==2)	AFNORM_GetDriveSTEP(DRV_STEP_STD);
				else				AFNORM_GetDriveSTEP(DRV_STEP_MID);	break;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DecDrvStdSPD_XX(BYTE nID)
{
	memset(&stAFNORM.stSTEP_m.stDEC_D, 0, sizeof(AFN_STEP_COUNT));
	memset(&stAFNORM.stSTEP_m.stDEC_I, 0, sizeof(AFN_STEP_COUNT));

	switch (stAFNORM.stSTEP_m.sSPD) {
		case DRV_STEP_MIN:		AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_SetDrvCurSPD_XX(BYTE nID)
{
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_ClrJudgeRATE(void)
{
	// clear MIN-MAX info
	stAFNORM.stOUTn_h.nMIN = stAFNORM.stOUTn_h.FLTn;
	stAFNORM.stOUTn_h.nMAX = stAFNORM.stOUTn_h.FLTn;
	stAFNORM.stOUTn_h.zMIN = stAFNORM.stOUTn_h.FLTn;
	stAFNORM.stOUTn_h.zMAX = stAFNORM.stOUTn_h.FLTn;

	stAFNORM.stOUTn_b.nMIN = stAFNORM.stOUTn_b.FLTn;
	stAFNORM.stOUTn_b.nMAX = stAFNORM.stOUTn_b.FLTn;
	stAFNORM.stOUTn_b.zMIN = stAFNORM.stOUTn_b.FLTn;
	stAFNORM.stOUTn_b.zMAX = stAFNORM.stOUTn_b.FLTn;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_ClrJudgePEAK(void)
{
	stAFNORM.stOUTn_h.nHIGH = stAFNORM.stOUTn_h.FLTn;
	stAFNORM.stOUTn_b.nHIGH = stAFNORM.stOUTn_b.FLTn;

	stAFNORM.stOUTn_h.nPEAK = stAFNORM.fVAL;
	stAFNORM.stOUTn_h.xPEAK = stAFNORM.fVAL;
	stAFNORM.stOUTn_b.nPEAK = stAFNORM.fVAL;
	stAFNORM.stOUTn_b.xPEAK = stAFNORM.fVAL;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_ClrJudgeINFO(void)
{
	stAFNORM.stOUTn_h.nCHKf = 0;
	stAFNORM.stOUTn_b.nCHKf = 0;

	stAFNORM.stSTEP_m.sCNT = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_ClrJudgeBUFF(void)
{
	memset(stAFNORM.stOUTn_h.nFltBUFF, 0, sizeof(AFN_BUFF_PARAM));
	memset(stAFNORM.stOUTn_b.nFltBUFF, 0, sizeof(AFN_BUFF_PARAM));

	stAFNORM.xCNT = 0;	stAFNORM.vCNT &= 0x80;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_MovShiftBUFF(BYTE nID)
{
	BYTE i;

	for (i=1; i<nID; i++) {
		stAFNORM.stOUTn_h.nFltBUFF[i-1] = stAFNORM.stOUTn_h.nFltBUFF[i];
		stAFNORM.stOUTn_b.nFltBUFF[i-1] = stAFNORM.stOUTn_b.nFltBUFF[i];

		stAFNORM.stOUTn_h.nFOKS[i-1] = stAFNORM.stOUTn_h.nFOKS[i];
		stAFNORM.stOUTn_b.nFOKS[i-1] = stAFNORM.stOUTn_b.nFOKS[i];
	}
}
//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_GetPrvJudgeBUFF(PAFN_FILT_PARAM pFILT, BYTE nID) // nID = xCNT
{
	FLOAT rate, gap; BYTE i; PLONG pBUFF = (PLONG)pFILT->nFltBUFF;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFNORM.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	pBUFF[nID] = pFILT->FLTn; pFILT->nFOKS[nID] = stAFNORM.fVAL;
	if (nID<(num_xCNT-1)) return;

	for (i=0; i<nID; i++) {
		pFILT->nDIFF[i] = (LONG)(pBUFF[i+1]-pBUFF[i]);

		if	(pFILT->nDIFF[i]>0)	pFILT->nSIGN[i] =  1;
		else if (pFILT->nDIFF[i]<0)	pFILT->nSIGN[i] = -1;
		else						pFILT->nSIGN[i] =  0;
	}

	pFILT->nMOVE = MAX3(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2]) // max value of 3 nFOKS
				 - MIN3(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2]); // min value of 3 nFOKS

	stAFNORM.fMOV = pFILT->nMOVE/nID; // check nID mimitest

	if (pFILT->nSIGN[0]==pFILT->nSIGN[1]) {
		gap = (pFILT->nMOVE)? (15.0f*nID)/pFILT->nMOVE : 1.0f; // nMOVE = 15*3(nID)/nMOVE
	}
	else {
		gap = (1.0f); pFILT->nMIN = pFILT->nMAX = pFILT->FLTn;
	}

	rate = (pFILT->nMIN)? ((FLOAT)pFILT->FLTn/pFILT->nMIN-1)*1000 : 0;
	pFILT->nINC = (LONG)((rate*gap<0)? 0 : rate*gap+0.5);

	rate = (pFILT->nMAX)? ((FLOAT)pFILT->FLTn/pFILT->nMAX-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate*gap>0)? 0 : rate*gap-0.5);

	rate = (pFILT->nMIN)? ((FLOAT)pFILT->FLTn/pFILT->nMIN-1)*500 : 0;
	pFILT->zINC = (LONG)((rate*gap<0)? 0 : rate*gap+0.5);

	rate = (pFILT->nMAX)? ((FLOAT)pFILT->FLTn/pFILT->nMAX-1)*500 : 0;
	pFILT->zDEC = (LONG)((rate*gap>0)? 0 : rate*gap-0.5);

	if (pFILT->nINC>=(LONG)pFILT->n_UP)	pFILT->nCHKf |= ( GOOD_INC|G_INC_OK);
	else								pFILT->nCHKf &= (~GOOD_INC);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN)	pFILT->nCHKf |= ( GOOD_DEC);
	else								pFILT->nCHKf &= (~GOOD_DEC);

	if (pFILT->zINC>=(LONG)pFILT->z_UP)	pFILT->nCHKf |= ( ZERO_INC);
	else								pFILT->nCHKf &= (~ZERO_INC);

	if (pFILT->zDEC<=(LONG)pFILT->z_DN)	pFILT->nCHKf |= ( ZERO_DEC);
	else								pFILT->nCHKf &= (~ZERO_DEC);
	pFILT->FLTx = pFILT->FLTd = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_GetDirJudgeBUFF(PAFN_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap;	PLONG pBUFF = (PLONG)pFILT->nFltBUFF;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFNORM.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	pBUFF[nID] = pFILT->FLTn; pFILT->nFOKS[nID] = stAFNORM.fVAL;
	if (nID<num_xCNT) return;

	pFILT->nMOVE = MAX4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3])
				 - MIN4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3]);

	stAFNORM.fMOV = pFILT->nMOVE/nID;

	gap = MAX((15.0f*nID)/pFILT->nMOVE, 1.0f);

	pFILT->zMIN = (pBUFF[0]+pBUFF[1]+pBUFF[2]+pBUFF[3]+2)/4;
	pFILT->zMAX = (pBUFF[0]+pBUFF[1]+pBUFF[2]+pBUFF[3]+2)/4;
	pFILT->FLTr = (pFILT->FLTn);

	rate = (pFILT->nMIN)? ((FLOAT)pFILT->FLTn/pFILT->nMIN-1)*500 : 0;
	pFILT->nINC = (LONG)((rate<0)? 0 : rate+0.5);

	rate = (pFILT->nMAX)? ((FLOAT)pFILT->FLTn/pFILT->nMAX-1)*500 : 0;
	pFILT->nDEC = (LONG)((rate>0)? 0 : rate-0.5);

	rate = (pFILT->zMIN)? ((FLOAT)pFILT->FLTr/pFILT->zMIN-1)*1000 : 0;
	pFILT->zINC = (LONG)((rate*gap<0)? 0 : rate*gap+0.5);

	rate = (pFILT->zMAX)? ((FLOAT)pFILT->FLTr/pFILT->zMAX-1)*1000 : 0;
	pFILT->zDEC = (LONG)((rate*gap>0)? 0 : rate*gap-0.5);

	if (pFILT->nINC>=(LONG)pFILT->n_UP || pFILT->zINC>=(LONG)pFILT->n_UP)
			pFILT->nCHKf |= ( GOOD_INC);
	else	pFILT->nCHKf &= (~GOOD_INC);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN || pFILT->zDEC<=(LONG)pFILT->n_DN)
			pFILT->nCHKf |= ( GOOD_DEC);
	else	pFILT->nCHKf &= (~GOOD_DEC);

	pFILT->FLTx = pFILT->FLTd = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_InvDirJudgeBUFF(PAFN_FILT_PARAM pFILT, BYTE nID)
{
	PLONG pBUFF = (PLONG)pFILT->nFltBUFF;
	FLOAT rate, gap = 15.0f/stAFNORM.step;

	rate = (pBUFF[2])? ((FLOAT)pFILT->FLTn/pBUFF[2]-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate*gap>0)? 0 : rate*gap-0.5);

	if (pFILT->nDEC<=-1)	pFILT->nCHKf |= ( ZERO_DEC);
	else					pFILT->nCHKf &= (~ZERO_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_GetDecJudgeBUFF(PAFN_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap;	PLONG pBUFF = (PLONG)pFILT->nFltBUFF;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFNORM.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	pBUFF[nID] = pFILT->FLTn; pFILT->nFOKS[nID] = stAFNORM.fVAL;
	if (nID<num_xCNT) return;

	pFILT->nMOVE = MAX4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3])
				 - MIN4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3]);

	stAFNORM.fMOV = pFILT->nMOVE/nID;

	gap = (15.0f*nID)/pFILT->nMOVE;

	pFILT->zMAX = MAX4(pBUFF[0],pBUFF[1],pBUFF[2],pBUFF[3]);
	pFILT->zMIN = (pBUFF[0]+pBUFF[1]+pBUFF[2]+pBUFF[3]+2)/4;

	pFILT->FLTr = (pFILT->FLTn);
	pFILT->FLTx = MAX(pFILT->FLTd-MAX((LONG)(5.0f/gap+0.5),5),5);

	rate = (pFILT->zMIN)? ((FLOAT)pFILT->FLTr/pFILT->zMIN-1)*1000 : 0;
	pFILT->FLTd = (LONG)((rate*gap<0)? 0 : rate*gap+0.5);

	rate = (pFILT->zMIN)? ((FLOAT)pFILT->FLTr/pFILT->zMIN-1)*1000 : 0;
	pFILT->zINC = (LONG)((rate*gap<0)? 0 : rate*gap+0.5);

	rate = (pFILT->zMAX)? ((FLOAT)pFILT->FLTn/pFILT->zMAX-1)*1000 : 0;
	pFILT->zDEC = (LONG)((rate*gap>0)? 0 : rate*gap-0.5);

	rate = (pFILT->nMIN)? ((FLOAT)pFILT->FLTn/pFILT->nMIN-1)*500 : 0;
	pFILT->nINC = (LONG)((rate<0)? 0 : rate+0.5);

	rate = (pFILT->nMAX)? ((FLOAT)pFILT->FLTn/pFILT->nMAX-1)*500 : 0;
	pFILT->nDEC = (LONG)((rate>0)? 0 : rate-0.5);

	if (pFILT->zINC>=(LONG)pFILT->n_UP)	pFILT->nCHKf |= ( GOOD_INC|G_INC_OK);
	else								pFILT->nCHKf &= (~GOOD_INC);

	if (pFILT->zDEC<=(LONG)pFILT->n_DN)	pFILT->nCHKf |= ( GOOD_DEC|G_DEC_OK);
	else								pFILT->nCHKf &= (~GOOD_DEC);

	if (pFILT->zINC>=(LONG)pFILT->z_UP)	pFILT->nCHKf |= ( ZERO_INC);
	else								pFILT->nCHKf &= (~ZERO_INC);

	if (pFILT->zDEC<=(LONG)pFILT->z_DN)	pFILT->nCHKf |= ( ZERO_DEC);
	else								pFILT->nCHKf &= (~ZERO_DEC);

	if (pFILT->nINC>=(LONG)pFILT->n_UP)	pFILT->nCHKf |= ( HIGH_INC);
	else								pFILT->nCHKf &= (~HIGH_INC);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN)	pFILT->nCHKf |= ( HIGH_DEC);
	else								pFILT->nCHKf &= (~HIGH_DEC);

	if ((pFILT->nCHKf&HIGH_INC)==0) pFILT->nCHKf &= (~(GOOD_INC));
	if ((pFILT->nCHKf&G_INC_OK)==0) return;

	if (pFILT->FLTx>=(LONG)pFILT->n_UP)	pFILT->nCHKf |= ( GOOD_INC);

	if (pFILT->FLTd<=(LONG)pFILT->FLTx)	pFILT->nCHKf |= ( RATE_DEC|R_DEC_OK);
	else								pFILT->nCHKf &= (~RATE_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_InvDecJudgeBUFF(PAFN_FILT_PARAM pFILT, BYTE nID)
{
	PLONG pBUFF = (PLONG)pFILT->nFltBUFF;
	FLOAT rate, gap = 15.0f/stAFNORM.step;

	rate = (pBUFF[2])? ((FLOAT)pFILT->FLTn/pBUFF[2]-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate*gap>0)? rate*gap : rate*gap-0.5);

	if (pFILT->nDEC<=20)	pFILT->nCHKf |= ( ZERO_DEC);
	else					pFILT->nCHKf &= (~ZERO_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_GetTopJudgeBUFF(PAFN_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFNORM.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	rate = (pFILT->nMIN)? ((FLOAT)pFILT->FLTn/pFILT->nMIN-1)*1000 : 0;
	pFILT->nINC = (LONG)((rate<0)? 0 : rate+0.5);

	rate = (pFILT->nMAX)? ((FLOAT)pFILT->FLTn/pFILT->nMAX-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate>0)? 0 : rate-0.5);

	if (pFILT->nINC>=(LONG)pFILT->n_UP)	pFILT->nCHKf |= ( GOOD_INC);
	else								pFILT->nCHKf &= (~GOOD_INC);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN)	pFILT->nCHKf |= ( GOOD_DEC);
	else								pFILT->nCHKf &= (~GOOD_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_InvTopJudgeBUFF(PAFN_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate;

	rate = (pFILT->nMAX)? ((FLOAT)pFILT->FLTn/pFILT->nMAX-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate>0)? rate+0.5 : rate-0.5);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN)	pFILT->nCHKf |= ( ZERO_DEC);
	else								pFILT->nCHKf &= (~ZERO_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_InvEndJudgeBUFF(PAFN_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate = ((FLOAT)pFILT->xHIGH/pFILT->FLTn-1);

	rate *= ((nID==USE_HPF)? 500 : 1000);
	pFILT->nDEC = (LONG)((rate<0)? rate-0.5 : rate+0.5);

	if (ABS(pFILT->nDEC)<100)	pFILT->nCHKf |= ( GOOD_END);
	else						pFILT->nCHKf &= (~GOOD_END);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_GetJudge_FAR(void)
{
	PAFN_FILT_PARAM pFILTx = &stAFNORM.stOUTn_b;
	PAFN_FILT_PARAM pFILTh = &stAFNORM.stOUTn_h;
	PAFN_FILT_PARAM pFILTb = &stAFNORM.stOUTn_b;

	if (stAFNORM.vCNT&0x80) pFILTx = &stAFNORM.stOUTn_h;

	AFNORM_GetDriveRATE(DRV_SEARCH_FAR); // 50 -50 1 -1

	AFNORM_GetPrvJudgeBUFF(&stAFNORM.stOUTn_h, stAFNORM.xCNT);
	AFNORM_GetPrvJudgeBUFF(&stAFNORM.stOUTn_b, stAFNORM.xCNT);

	if (AFNORM_IsDriveEnd()==FALSE) {
		stAFNORM.xCNT++;	if (stAFNORM.xCNT<num_xCNT) return;
		stAFNORM.vCNT++;
	}

	if (AFNORM_IsDriveEnd()==TRUE) {
		stAFNORM.eCNT++;			AFNORM_ClrJudgeRATE();
		stAFNORM.cur = INV_SEARCH_FAR;	stAFNORM.nxt = PRE_SEARCH_DIR;
	}
	else if (pFILTx->nCHKf&GOOD_DEC && (pFILTb->nCHKf&ZERO_INC)==0) {
		stAFNORM.cur = INV_SEARCH_FAR;	stAFNORM.nxt = END_SEARCH_FAR;
	}
	else if (pFILTx->nCHKf&GOOD_INC) {
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;
	}
	else if ((stAFNORM.vCNT&0xf)==5) {
		stAFNORM.cur = INV_SEARCH_FAR;	stAFNORM.nxt = PRE_SEARCH_DIR;
	}
	else {
		stAFNORM.xCNT--;			AFNORM_MovShiftBUFF(num_xCNT);
		stAFNORM.cur = DRV_SEARCH_FAR;	stAFNORM.nxt = DRV_SEARCH_FAR;
	}

	if ((stAFNORM.vCNT&0xf)==5 && pFILTb->nCHKf&GOOD_DEC) {
		stAFNORM.cur = INV_SEARCH_FAR;	stAFNORM.nxt = END_SEARCH_FAR;
	}
	if ((stAFNORM.vCNT&0xf)==5 && pFILTb->nCHKf&GOOD_INC) {
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;
	}

	if (pFILTh->nCHKf&GOOD_DEC) stAFNORM.nSEL = USE_HPF;
	if (pFILTh->nCHKf&GOOD_INC) stAFNORM.nSEL = USE_HPF;

	if (stAFNORM.cur==DRV_CLIMB_TOP2) AFNORM_ClrJudgePEAK();
	if (stAFNORM.cur==INV_SEARCH_FAR) AFNORM_ClrJudgeBUFF();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_GetJudge_NAR(void)
{
	PAFN_FILT_PARAM pFILTx = &stAFNORM.stOUTn_b;
	PAFN_FILT_PARAM pFILTh = &stAFNORM.stOUTn_h;
	PAFN_FILT_PARAM pFILTb = &stAFNORM.stOUTn_b;

	if (stAFNORM.vCNT&0x80) pFILTx = &stAFNORM.stOUTn_h;

	AFNORM_GetDriveRATE(DRV_SEARCH_NAR); // 50 -50 1 -1

	AFNORM_GetPrvJudgeBUFF(&stAFNORM.stOUTn_h, stAFNORM.xCNT);
	AFNORM_GetPrvJudgeBUFF(&stAFNORM.stOUTn_b, stAFNORM.xCNT);

	if (AFNORM_IsDriveEnd()==FALSE) {
		stAFNORM.xCNT++;	if (stAFNORM.xCNT<num_xCNT) return;
		stAFNORM.vCNT++;
	}

	if (AFNORM_IsDriveEnd()==TRUE) {
				stAFNORM.eCNT++;			AFNORM_ClrJudgeRATE();
		stAFNORM.cur = INV_SEARCH_NAR;	stAFNORM.nxt = PRE_SEARCH_DIR;
	}
	else if (pFILTx->nCHKf&GOOD_DEC && (pFILTb->nCHKf&ZERO_INC)==0) {
		stAFNORM.cur = INV_SEARCH_NAR;	stAFNORM.nxt = DRV_FORCE_PEAK;
	}
	else if (pFILTx->nCHKf&ZERO_INC || pFILTb->nCHKf&ZERO_INC) {
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;
	}
	else if ((stAFNORM.vCNT&0xf)==5) {
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;
	}
	else {		stAFNORM.xCNT--;			AFNORM_MovShiftBUFF(num_xCNT);
		stAFNORM.cur = DRV_SEARCH_NAR;	stAFNORM.nxt = DRV_SEARCH_NAR;
	}

	if (pFILTh->nCHKf&GOOD_DEC) stAFNORM.nSEL = USE_HPF;
	if (pFILTh->nCHKf&GOOD_INC) stAFNORM.nSEL = USE_HPF;

	if (stAFNORM.cur==INV_SEARCH_NAR) AFNORM_ClrJudgeBUFF();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_GetJudge_DIR(void)
{
	PAFN_FILT_PARAM pFILTh = &stAFNORM.stOUTn_h;
	PAFN_FILT_PARAM pFILTb = &stAFNORM.stOUTn_b;

	AFNORM_GetDriveRATE(DRV_SEARCH_DIR);

	AFNORM_GetDirJudgeBUFF(&stAFNORM.stOUTn_h, stAFNORM.xCNT);
	AFNORM_GetDirJudgeBUFF(&stAFNORM.stOUTn_b, stAFNORM.xCNT);

	if (AFNORM_IsDriveEnd()==FALSE) {
		stAFNORM.xCNT++; if (stAFNORM.xCNT<(num_xCNT+1)) return;
	}

	if (stAFNORM.xCNT) stAFNORM.xCNT--;	AFNORM_MovShiftBUFF(num_xCNT+1);

	if (AFNORM_IsDriveEnd()==TRUE) {
				stAFNORM.eCNT++;			AFNORM_ClrJudgeRATE();
		stAFNORM.cur = INV_SEARCH_DIR;	stAFNORM.nxt = DRV_SEARCH_DIR;
	}
	else if (pFILTb->nCHKf&GOOD_DEC) {
			AFNORM_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFNORM.cur = INV_SEARCH_DIR;	stAFNORM.nxt = PRE_CLIMB_TOP2;
	}
	else if (pFILTb->nCHKf&GOOD_INC) {
			AFNORM_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;
	}
	else if (pFILTh->nCHKf&GOOD_DEC) {
			AFNORM_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFNORM.cur = INV_SEARCH_DIR;	stAFNORM.nxt = PRE_CLIMB_TOP2;
	}
	else if (pFILTh->nCHKf&GOOD_INC) {
			AFNORM_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;
	}
	else {
			AFNORM_DirDrvIncSPD_BI(DRV_SEARCH_DIR);
		stAFNORM.cur = DRV_SEARCH_DIR;	stAFNORM.nxt = DRV_SEARCH_DIR;
	}

	if (pFILTh->nCHKf&GOOD_DEC) stAFNORM.nSEL = USE_HPF;
	if (pFILTh->nCHKf&GOOD_INC) stAFNORM.nSEL = USE_HPF;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_InvJudge_DIR(void)
{
	PAFN_FILT_PARAM pFILTx = &stAFNORM.stOUTn_b;
	PAFN_FILT_PARAM pFILTb = &stAFNORM.stOUTn_b;

	if (stAFNORM.nSEL==USE_HPF) pFILTx = &stAFNORM.stOUTn_h;

	if (stAFNORM.nxt!=PRE_CLIMB_TOP2) AFNORM_ClrJudgeBUFF();
	if (stAFNORM.nxt!=PRE_CLIMB_TOP2) return;

	AFNORM_GetDriveRATE(DRV_SEARCH_DIR);

	AFNORM_InvDirJudgeBUFF(&stAFNORM.stOUTn_h, stAFNORM.xCNT);
	AFNORM_InvDirJudgeBUFF(&stAFNORM.stOUTn_b, stAFNORM.xCNT);

	if ((pFILTb->nCHKf&ZERO_DEC)==0 || (pFILTx->nCHKf&ZERO_DEC)==0) {
		stAFNORM.nxt = DRV_SEARCH_DIR;	AFNORM_SetChangeDIR();
		pFILTb->nMAX = (pFILTb->nMAX+pFILTb->FLTn)/2;
		pFILTx->nMAX = (pFILTx->nMAX+pFILTx->FLTn)/2;
	}

	if (stAFNORM.nxt==DRV_SEARCH_DIR) AFNORM_ClrJudgePEAK();
	if (stAFNORM.nxt==PRE_CLIMB_TOP2) AFNORM_ClrJudgePEAK();
	if (stAFNORM.nxt==PRE_CLIMB_TOP2) AFNORM_ClrJudgeBUFF();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_GetJudge_DEC(void)
{
	PAFN_FILT_PARAM pFILTx = &stAFNORM.stOUTn_b;
	PAFN_FILT_PARAM pFILTh = &stAFNORM.stOUTn_h;
	PAFN_FILT_PARAM pFILTb = &stAFNORM.stOUTn_b;

	if (stAFNORM.nSEL==USE_HPF) pFILTx = &stAFNORM.stOUTn_h;

	AFNORM_GetDriveRATE(DRV_CLIMB_TOP2);

	AFNORM_GetDecJudgeBUFF(&stAFNORM.stOUTn_h, stAFNORM.xCNT);
	AFNORM_GetDecJudgeBUFF(&stAFNORM.stOUTn_b, stAFNORM.xCNT);

	if (stAFNORM.ZOOM<=stAFNORM.KNEE && (pFILTh->nCHKf&HIGH_INC)==0)
		pFILTb->nCHKf &= (~(GOOD_INC|G_INC_OK));

	if (AFNORM_IsDriveEnd()==FALSE) {
		stAFNORM.xCNT++; if (stAFNORM.xCNT<(num_xCNT+1)) return;
	}

	if (stAFNORM.xCNT) stAFNORM.xCNT--;	AFNORM_MovShiftBUFF(num_xCNT+1);

	if (AFNORM_IsDriveEnd()==TRUE) {
				stAFNORM.eCNT++;			AFNORM_ClrJudgeRATE();
		stAFNORM.cur = INV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;
	}
	else if (pFILTh->nCHKf&GOOD_DEC && pFILTb->nCHKf&GOOD_DEC) {
		stAFNORM.cur = INV_CLIMB_TOP2;	stAFNORM.nxt = PRE_CLIMB_PEAK;
	}
	else if (pFILTb->nCHKf&GOOD_INC && pFILTb->nCHKf&RATE_DEC) {
			AFNORM_DecDrvDecSPD_GD(DRV_CLIMB_TOP2);
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;
	}
	else if (pFILTb->nCHKf&GOOD_INC) {
			AFNORM_DecDrvIncSPD_GX(DRV_CLIMB_TOP2);
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;
	}
	else if (pFILTh->nCHKf&GOOD_DEC || pFILTh->nCHKf&HIGH_DEC) {
			AFNORM_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;
	}
	else {
			AFNORM_DecDrvIncSPD_BI(DRV_CLIMB_TOP2);
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;
	}

	if (pFILTh->nCHKf&GOOD_INC || pFILTh->nCHKf&HIGH_INC) stAFNORM.nSEL = USE_HPF;
	if (pFILTh->nCHKf&GOOD_DEC || pFILTh->nCHKf&HIGH_DEC) stAFNORM.nSEL = USE_HPF;

	if (pFILTb->nCHKf&R_DEC_OK && pFILTx->nCHKf&ZERO_DEC) {
		stAFNORM.cur = INV_CLIMB_TOP2;	stAFNORM.nxt = PRE_CLIMB_PEAK;
	}

	if (pFILTb->nCHKf&HIGH_DEC && pFILTx->nCHKf&ZERO_DEC) {
		stAFNORM.cur = INV_CLIMB_TOP2;	stAFNORM.nxt = PRE_CLIMB_PEAK;
	}

	if (pFILTh->nCHKf&G_INC_OK && pFILTh->nCHKf&HIGH_DEC) {
		stAFNORM.cur = INV_CLIMB_TOP2;	stAFNORM.nxt = PRE_CLIMB_PEAK;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_InvJudge_DEC(void)
{
	PAFN_FILT_PARAM pFILTx = &stAFNORM.stOUTn_b;

	if (stAFNORM.nxt!=PRE_CLIMB_PEAK) AFNORM_ClrJudgeBUFF();
	if (stAFNORM.nxt!=PRE_CLIMB_PEAK) return;

	AFNORM_GetDriveRATE(DRV_CLIMB_TOP2);

	AFNORM_InvDecJudgeBUFF(&stAFNORM.stOUTn_h, stAFNORM.xCNT);
	AFNORM_InvDecJudgeBUFF(&stAFNORM.stOUTn_b, stAFNORM.xCNT);

	if ((pFILTx->nCHKf&ZERO_DEC)==0) {
		stAFNORM.nxt = DRV_CLIMB_TOP2;	AFNORM_SetChangeDIR();
	}

	AFNORM_DecDrvStdSPD_XX(DRV_CLIMB_TOP2);
	if (stAFNORM.nxt==DRV_CLIMB_TOP2) return;

	AFNORM_GetDriveSTEP(DRV_STEP_DIV);
	AFNORM_ClrJudgeBUFF();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_GetJudge_TOP(void)
{
	PAFN_FILT_PARAM pFILTx = &stAFNORM.stOUTn_b;

	if (stAFNORM.nSEL==USE_HPF) pFILTx = &stAFNORM.stOUTn_h;

	AFNORM_GetDriveRATE(DRV_CLIMB_PEAK);

	AFNORM_GetTopJudgeBUFF(&stAFNORM.stOUTn_h, stAFNORM.xCNT);
	AFNORM_GetTopJudgeBUFF(&stAFNORM.stOUTn_b, stAFNORM.xCNT);

	if (AFNORM_IsDriveEnd()==TRUE) {
		stAFNORM.cur = INV_CLIMB_PEAK;	stAFNORM.nxt = DRV_FORCE_PEAK;
	}
	else if (pFILTx->nCHKf&GOOD_DEC) {
		stAFNORM.cur = INV_CLIMB_PEAK;	stAFNORM.nxt = DRV_FORCE_PEAK;
	}
	else {
		stAFNORM.cur = DRV_CLIMB_PEAK;	stAFNORM.nxt = DRV_CLIMB_PEAK;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_InvJudge_TOP(void)
{
	PAFN_FILT_PARAM pFILTx = &stAFNORM.stOUTn_b;

	if (stAFNORM.nSEL==USE_HPF) pFILTx = &stAFNORM.stOUTn_h;

	if (AFNORM_IsDriveEnd()==TRUE) return;

	AFNORM_GetDriveRATE(DRV_CLIMB_PEAK);

	AFNORM_InvTopJudgeBUFF(&stAFNORM.stOUTn_h, stAFNORM.xCNT);
	AFNORM_InvTopJudgeBUFF(&stAFNORM.stOUTn_b, stAFNORM.xCNT);

	if (pFILTx->nCHKf&ZERO_DEC) return;

	stAFNORM.nxt = DRV_CLIMB_PEAK;
	AFNORM_SetChangeDIR();
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFNORM_InvJudge_END(void)
{
	PAFN_FILT_PARAM pFILTb = &stAFNORM.stOUTn_b;
	PAFN_FILT_PARAM pFILTh = &stAFNORM.stOUTn_h;

	if (stAFNORM.eCNT==2) return FALSE;
	AFNORM_GetDriveRATE(DRV_CLIMB_PEAK);

	AFNORM_InvEndJudgeBUFF(&stAFNORM.stOUTn_h, USE_HPF);
	AFNORM_InvEndJudgeBUFF(&stAFNORM.stOUTn_b, USE_BPF);
	if ((pFILTb->nCHKf&GOOD_END)==0) return TRUE;
	if ((pFILTh->nCHKf&GOOD_END)==0) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_SetInitPARAM(void)
{
	stAFNORM.peak = stAFNORM.fVAL;

	stAFNORM.fMID = LENS_GetMIDPositionLOCI();
	stAFNORM.fGAP = LENS_GetGAPPositionLOCI();

	stAFNORM.fMIN = LENS_GetMINPositionLOCI(stAFNORM.zVAL);
	stAFNORM.fMAX = LENS_GetMAXPositionLOCI(stAFNORM.zVAL);
	stAFNORM.nDIR = LENS_GetDirectionFOKS();
	LENS_SetDirectionFOKS(stAFNORM.nDIR);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_PreFilterSEL(void)
{
	if (stAFNORM.cur!=PRE_FILTER_SEL) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFNORM_SetInitPARAM();	LENS_EnableMotorFOKS(ON);
	stAFNORM.eCNT = 0; stAFNORM.cur = PRE_SEARCH_MOV;
	stAFNORM.vCNT = 0; stAFNORM.nxt = PRE_SEARCH_MOV;

	memset(&stAFNORM.stSTEP_m, 0, sizeof(AFN_STEP_PARAM));
	stAFNORM.stSTEP_m.sMIN = LENS_GetAFDriveSTEP(DRV_STEP_MIN);
	stAFNORM.stSTEP_m.sSTD = LENS_GetAFDriveSTEP(DRV_STEP_STD);
	stAFNORM.stSTEP_m.sMID = LENS_GetAFDriveSTEP(DRV_STEP_MID);
	stAFNORM.stSTEP_m.sMAX = LENS_GetAFDriveSTEP(DRV_STEP_MAX);
	stAFNORM.stSTEP_m.sFWD = LENS_GetAFDriveSTEP(DRV_STEP_FWD);
	stAFNORM.stSTEP_m.sBWD = LENS_GetAFDriveSTEP(DRV_STEP_BWD);
	stAFNORM.stSTEP_m.sTOP = LENS_GetAFDriveSTEP(DRV_STEP_TOP);

	if (stAFNORM.nSEL==USE_HPF) stAFNORM.vCNT |= 0x80;
	stAFNORM.nSEL = USE_BPF;	AFNORM_ClrJudgeBUFF(); // buffer clear

	memset(&stAFNORM.stOUTn_h.nMIN, 0, sizeof(AFN_RATE_PARAM));
	memset(&stAFNORM.stOUTn_b.nMIN, 0, sizeof(AFN_RATE_PARAM));

	stAFNORM.stNORM_y.nRST = 0;
	stAFNORM.stNORM_y.nERR = 0;
	stAFNORM.stNORM_y.nCNT = 0;

	stAFNTUNE.n_GAIN = 0;
	stAFNTUNE.n_YSUM = 0;
	stAFNTUNE.n_FLTr = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
//static void AFNORM_EndFilterSEL(void)
//{
//	if (stAFNORM.cur!=END_FILTER_SEL) return;
//	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
//
//	stAFNORM.cur = PRE_SEARCH_MOV;
//	stAFNORM.nxt = PRE_SEARCH_MOV;
//}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_PreSearchMOV(void)
{
	if (stAFNORM.cur!=PRE_SEARCH_MOV) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFNORM.stOUTn_b.xHIGH = stAFNORM.stOUTn_b.FLTn; // backup
	stAFNORM.stOUTn_h.xHIGH = stAFNORM.stOUTn_h.FLTn;

	stAFNORM.cur = DRV_SEARCH_MOV;
	//stAFNTUNE.n_GAIN = 0;
	//stAFNTUNE.n_YSUM = 0;
	//stAFNTUNE.n_FLTr = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DrvSearchMOV(void)
{
	if (stAFNORM.cur!=DRV_SEARCH_MOV) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFNORM.cur = PRE_SEARCH_FAR;
	stAFNORM.nxt = PRE_SEARCH_FAR;

	AFNORM_ClrJudgeRATE(); // nMIN, nMAX, zMIN & zMAX
	AFNORM_ClrJudgePEAK(); // nHIGH, nPEAK & xPEAK
	AFNORM_ClrJudgeINFO(); // nCHKf & nCNT

	AFNORM_GetJudge_FAR();

	AFNORM_GetDriveSTEP(DRV_STEP_MIN);
	AFNORM_SetDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_PreSearchFAR(void)
{
	if (stAFNORM.cur!=PRE_SEARCH_FAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFNORM.cur = DRV_SEARCH_FAR;
	stAFNORM.nxt = DRV_SEARCH_FAR;

	AFNORM_GetJudge_FAR();

	AFNORM_SetDriveSTEP(DRV_STEP_MIN); // goal
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DrvSearchFAR(void)
{
	if (stAFNORM.cur!=DRV_SEARCH_FAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFNORM_GetJudge_FAR();

	if (stAFNORM.cur==INV_SEARCH_FAR) return;
	AFNORM_SetDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndSearchFAR(void)
{
	if (stAFNORM.cur!=END_SEARCH_FAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFNORM.nSEL==USE_HPF)
		 stAFNORM.peak = stAFNORM.stOUTn_h.xPEAK;
	else stAFNORM.peak = stAFNORM.stOUTn_b.xPEAK;

	stAFNORM.cur = stAFNORM.nxt;
	stAFNORM.nxt = PRE_SEARCH_NAR;

	if (stAFNORM.cur==DRV_FORCE_PEAK) return;
	if (stAFNORM.fVAL==stAFNORM.peak) return;

	AFNORM_GetDriveSTEP(DRV_STEP_BWD);
	AFNORM_GetDriveSTEP(DRV_STEP_REM);
	AFNORM_SetDriveSTEP(DRV_STEP_REM);
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFNORM_MovSearchNAR(void)
{
	if (stAFNORM.cur!=PRE_SEARCH_NAR) return FALSE;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return FALSE;

	if (stAFNORM.fVAL==stAFNORM.peak) return TRUE;

	if (stAFNORM.nDIR== fFAR && stAFNORM.fVAL<stAFNORM.peak)
		AFNORM_GetDriveSTEP(DRV_STEP_REM);

	if (stAFNORM.nDIR==fNEAR && stAFNORM.fVAL>stAFNORM.peak)
		AFNORM_GetDriveSTEP(DRV_STEP_REM);

	AFNORM_SetDriveSTEP(DRV_STEP_REM);
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_PreSearchNAR(void)
{
	if (stAFNORM.cur!=PRE_SEARCH_NAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (AFNORM_MovSearchNAR()==FALSE) return;

	stAFNORM.cur = DRV_SEARCH_NAR;
	stAFNORM.nxt = DRV_SEARCH_NAR;

	AFNORM_ClrJudgeRATE();
	AFNORM_ClrJudgePEAK();
	AFNORM_ClrJudgeINFO();

	AFNORM_GetJudge_NAR();

	AFNORM_GetDriveSTEP(DRV_STEP_MIN);
	AFNORM_SetDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DrvSearchNAR(void)
{
	if (stAFNORM.cur!=DRV_SEARCH_NAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFNORM_GetJudge_NAR();

	if (stAFNORM.cur==INV_SEARCH_NAR) return;
	AFNORM_SetDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndSearchNAR(void)
{
	if (stAFNORM.cur!=END_SEARCH_NAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFNORM.nSEL==USE_HPF)
		 stAFNORM.peak = stAFNORM.stOUTn_h.xPEAK;
	else stAFNORM.peak = stAFNORM.stOUTn_b.xPEAK;

	stAFNORM.cur = stAFNORM.nxt;

	if (stAFNORM.cur==DRV_FORCE_PEAK) return;
	AFNORM_SetDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_PreSearchDIR(void)
{
	if (stAFNORM.cur!=PRE_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFNORM.cur = DRV_SEARCH_DIR;
	stAFNORM.nxt = DRV_SEARCH_DIR;

	AFNORM_GetJudge_DIR();

	AFNORM_GetDriveSTEP(DRV_STEP_STD);
	AFNORM_SetDriveSTEP(DRV_STEP_STD);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DrvSearchDIR(void)
{
	if (stAFNORM.cur!=DRV_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFNORM_GetJudge_DIR();

	if (stAFNORM.cur==INV_SEARCH_DIR) return;
	AFNORM_SetDriveSTEP(DRV_STEP_STD);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndSearchDIR(void)
{
	if (stAFNORM.cur!=END_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFNORM.nSEL==USE_HPF)
		 stAFNORM.peak = stAFNORM.stOUTn_h.nPEAK;
	else stAFNORM.peak = stAFNORM.stOUTn_b.nPEAK;

	AFNORM_InvJudge_DIR();

	if (stAFNORM.eCNT==2)
		 stAFNORM.cur = DRV_FORCE_PEAK;
	else stAFNORM.cur = stAFNORM.nxt;

	if (stAFNORM.cur==DRV_FORCE_PEAK) return;
	AFNORM_SetDriveSTEP(DRV_STEP_PRV);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_PreClimbTOP2(void)
{
	if (stAFNORM.cur!=PRE_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFNORM.cur = DRV_CLIMB_TOP2;
	stAFNORM.nxt = DRV_CLIMB_TOP2;

	AFNORM_ClrJudgeRATE();
	AFNORM_ClrJudgeINFO();
	AFNORM_GetJudge_DEC();

	AFNORM_SetDriveSTEP(DRV_STEP_PRV);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DrvClimbTOP2(void)
{
	if (stAFNORM.cur!=DRV_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFNORM_GetJudge_DEC();

	if (stAFNORM.cur==INV_CLIMB_TOP2) return;
	AFNORM_SetDriveSTEP(DRV_STEP_PRV);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndClimbTOP2(void)
{
	if (stAFNORM.cur!=END_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFNORM.nSEL==USE_HPF)
		 stAFNORM.peak = stAFNORM.stOUTn_h.nPEAK;
	else stAFNORM.peak = stAFNORM.stOUTn_b.nPEAK;

	AFNORM_InvJudge_DEC();

	if (stAFNORM.eCNT==2)
		 stAFNORM.cur = DRV_FORCE_PEAK;
	else stAFNORM.cur = stAFNORM.nxt;

	if (stAFNORM.cur==DRV_FORCE_PEAK) return;
	if (stAFNORM.cur==PRE_CLIMB_PEAK &&
		stAFNORM.fVAL==stAFNORM.peak) return;

	AFNORM_GetDriveSTEP(DRV_STEP_PRV);
	AFNORM_SetDriveSTEP(DRV_STEP_PRV);
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFNORM_MovClimbPEAK(void)
{
	if (stAFNORM.cur!=PRE_CLIMB_PEAK) return FALSE;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return FALSE;

	if (stAFNORM.fVAL==stAFNORM.peak) return TRUE;

	if (stAFNORM.nDIR== fFAR && stAFNORM.fVAL>stAFNORM.peak) {
		stAFNORM.stNORM_y.nERR = TRUE;	return TRUE;
	}
	if (stAFNORM.nDIR==fNEAR && stAFNORM.fVAL<stAFNORM.peak) {
		stAFNORM.stNORM_y.nERR = TRUE;	return TRUE;
	}

	if (stAFNORM.nDIR== fFAR && stAFNORM.fVAL<stAFNORM.peak)
		AFNORM_GetDriveSTEP(DRV_STEP_REM);

	if (stAFNORM.nDIR==fNEAR && stAFNORM.fVAL>stAFNORM.peak)
		AFNORM_GetDriveSTEP(DRV_STEP_REM);

	AFNORM_SetDriveSTEP(DRV_STEP_REM);
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_PreClimbPEAK(void)
{
	if (stAFNORM.cur!=PRE_CLIMB_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (AFNORM_MovClimbPEAK()==FALSE) return;

	stAFNORM.cur = DRV_CLIMB_PEAK;
	stAFNORM.nxt = DRV_CLIMB_PEAK;

	AFNORM_ClrJudgeRATE();
	AFNORM_ClrJudgePEAK();
	AFNORM_ClrJudgeINFO();
	AFNORM_GetJudge_TOP();

	if (stAFNORM.stNORM_y.nERR) return;

	AFNORM_GetDriveSTEP(DRV_STEP_TOP);
	AFNORM_SetDriveSTEP(DRV_STEP_TOP);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DrvClimbPEAK(void)
{
	if (stAFNORM.cur!=DRV_CLIMB_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFNORM_GetJudge_TOP();

	if (stAFNORM.cur==INV_CLIMB_PEAK) return;
	AFNORM_SetDriveSTEP(DRV_STEP_TOP);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndClimbPEAK(void)
{
	if (stAFNORM.cur!=END_CLIMB_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFNORM.nSEL==USE_HPF)
		 stAFNORM.peak = stAFNORM.stOUTn_h.nPEAK;
	else stAFNORM.peak = stAFNORM.stOUTn_b.nPEAK;

	if (stAFNORM.nDIR==fFAR)
		 stAFNORM.peak += 2*stAFNORM.stSTEP_m.sTOP;
	else stAFNORM.peak -= 0*stAFNORM.stSTEP_m.sTOP;

	AFNORM_InvJudge_TOP();

	stAFNORM.cur = stAFNORM.nxt;

	if (stAFNORM.cur==DRV_FORCE_PEAK) return;
	AFNORM_SetDriveSTEP(DRV_STEP_TOP);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DrvForcePEAK(void)
{
	if (stAFNORM.cur!=DRV_FORCE_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFNORM.nDIR==fFAR) stAFNORM.peak += stAFNTUNE.h_FOKS;
	else					 stAFNORM.peak -= stAFNTUNE.h_FOKS;

	stAFNORM.cur = END_FORCE_PEAK;
	AFNORM_SetDrivePEAK();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndForcePEAK(void)
{
	if (stAFNORM.cur!=END_FORCE_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFNORM.cur = END_CLEAR_WAIT;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndDriveWAIT(void)
{
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	switch (stAFNORM.cur) {
		case INV_FILTER_SEL: stAFNORM.cur = PRE_SEARCH_MOV; break;
		case INV_SEARCH_FAR: stAFNORM.cur = END_SEARCH_FAR; break;
		case INV_SEARCH_NAR: stAFNORM.cur = END_SEARCH_NAR; break;
		case INV_SEARCH_DIR: stAFNORM.cur = END_SEARCH_DIR; break;
		case INV_CLIMB_TOP1: stAFNORM.cur = END_CLIMB_TOP1; break;
		case INV_CLIMB_TOP2: stAFNORM.cur = END_CLIMB_TOP2; break;
		case INV_CLIMB_PEAK: stAFNORM.cur = END_CLIMB_PEAK; break;
		case INV_SWING_SCAN: stAFNORM.cur = END_SWING_SCAN; break;
		default:			 return;
	}
	AFNORM_SetChangeDIR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndClearLUMA(PAF_CTRL_PARAM pCTRL)
{
	WORD nID = MAKEWORD(stAFNORM.stNORM_y.nERR,stAFNORM.fMOV);

	if (stAFNORM.cur<=END_SEARCH_MOV) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFNORM.stNORM_y.nRST |= AF_GetLUMAInfo(pCTRL,nID); // check wrong LUMA change
	stAFNORM.stNORM_y.nERR  = FALSE;

	if (stAFNORM.stNORM_y.nRST) stAFNORM.cur = END_FORCE_PEAK;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndClearAUTO(PAF_CTRL_PARAM pCTRL)
{
	if (stAFNORM.cur!=END_CLEAR_AUTO) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFNORM.cur = END_CLEAR_PROC;

	if (stAFNORM.stNORM_y.nRST==TRUE) return;
	AF_SetAUTOInfo(pCTRL, stAFNORM.stNORM_y.nCHK); // set AF auto information end of AUTO AF

	if (stAFNORM.ZOOM<=stAFNORM.KNEE) return;
	LENS_SetTrackingINFO(stAFNORM.peak); // calculate fRatio
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndClearWAIT(PAF_CTRL_PARAM pCTRL)
{
	BOOL nID;

	if (stAFNORM.cur!=END_CLEAR_WAIT) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	nID = (AFNORM_InvJudge_END())? TRUE : FALSE;
	stAFNORM.stNORM_y.nRST |= AF_GetDATAInfo(pCTRL,nID);

	stAFNORM.cur = END_CLEAR_AUTO;
	LENS_EnableMotorFOKS(OFF);
}

//--------------------------------------------------------------------------------------------------------------------------
void AFNORM_ProcessHandler(PAF_CTRL_PARAM pCTRL)
{
	if (stAFNORM.cur==0) return;
	stAFNORM.prv = stAFNORM.cur;

	AFNORM_GetAFOPDWIND(pCTRL);
	AFNORM_EndClearLUMA(pCTRL);
	AFNORM_EndClearAUTO(pCTRL);
	AFNORM_EndClearWAIT(pCTRL);

	AFNORM_PreFilterSEL();		
	AFNORM_PreSearchMOV();		
	AFNORM_PreSearchFAR();		
	AFNORM_PreSearchNAR();		
	AFNORM_PreSearchDIR();		
	AFNORM_PreClimbTOP2();		
	AFNORM_PreClimbPEAK();		

	AFNORM_DrvSearchMOV();		
	AFNORM_DrvSearchFAR();		
	AFNORM_DrvSearchNAR();		
	AFNORM_DrvSearchDIR();		
	AFNORM_DrvClimbTOP2();		
	AFNORM_DrvClimbPEAK();		
	AFNORM_DrvForcePEAK();		

	//AFNORM_EndFilterSEL();		
	AFNORM_EndSearchFAR();		
	AFNORM_EndSearchNAR();		
	AFNORM_EndSearchDIR();		
	AFNORM_EndClimbTOP2();		
	AFNORM_EndClimbPEAK();		

	AFNORM_EndForcePEAK();		
	AFNORM_EndDriveWAIT();		
}

//--------------------------------------------------------------------------------------------------------------------------
void AFNORM_EnableProcess(BOOL OnOff)
{
	if (OnOff)	stAFNORM.cur = PRE_FILTER_SEL;
	else		stAFNORM.cur = END_CLEAR_PROC;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AFNORM_IsProcessExecuting(void)
{
	return (stAFNORM.cur==0)? FALSE : TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static SHORT UPDNb[32];
void AFNORM_ChangeAFCtlVal(int init,int cur,int val)
{
	if(init==0) {
		UPDNb[ 0]=stAFNTUNE.stPRV_b.n_UP;
		UPDNb[ 1]=stAFNTUNE.stPRV_b.n_DN;
		UPDNb[ 2]=stAFNTUNE.stPRV_b.z_UP;
		UPDNb[ 3]=stAFNTUNE.stPRV_b.z_DN;
		UPDNb[ 4]=stAFNTUNE.stPRV_h.n_UP;
		UPDNb[ 5]=stAFNTUNE.stPRV_h.n_DN;
		UPDNb[ 6]=stAFNTUNE.stPRV_h.z_UP;
		UPDNb[ 7]=stAFNTUNE.stPRV_h.z_DN;
		UPDNb[ 8]=stAFNTUNE.stDIR_b.n_UP;
		UPDNb[ 9]=stAFNTUNE.stDIR_b.n_DN;
		UPDNb[10]=stAFNTUNE.stDIR_b.z_UP;
		UPDNb[11]=stAFNTUNE.stDIR_b.z_DN;
		UPDNb[12]=stAFNTUNE.stDIR_h.n_UP;
		UPDNb[13]=stAFNTUNE.stDIR_h.n_DN;
		UPDNb[14]=stAFNTUNE.stDIR_h.z_UP;
		UPDNb[15]=stAFNTUNE.stDIR_h.z_DN;
		UPDNb[16]=stAFNTUNE.stDEC_b.n_UP;
		UPDNb[17]=stAFNTUNE.stDEC_b.n_DN;
		UPDNb[18]=stAFNTUNE.stDEC_b.z_UP;
		UPDNb[19]=stAFNTUNE.stDEC_b.z_DN;
		UPDNb[20]=stAFNTUNE.stDEC_h.n_UP;
		UPDNb[21]=stAFNTUNE.stDEC_h.n_DN;
		UPDNb[22]=stAFNTUNE.stDEC_h.z_UP;
		UPDNb[23]=stAFNTUNE.stDEC_h.z_DN;
		UPDNb[24]=stAFNTUNE.stTOP_b.n_UP;
		UPDNb[25]=stAFNTUNE.stTOP_b.n_DN;
		UPDNb[26]=stAFNTUNE.stTOP_b.z_UP;
		UPDNb[27]=stAFNTUNE.stTOP_b.z_DN;
		UPDNb[28]=stAFNTUNE.stTOP_h.n_UP;
		UPDNb[29]=stAFNTUNE.stTOP_h.n_DN;
		UPDNb[30]=stAFNTUNE.stTOP_h.z_UP;
		UPDNb[31]=stAFNTUNE.stTOP_h.z_DN;
	}else if(init==2) {
		stAFNTUNE.stPRV_b.n_UP=UPDNb[ 0];
		stAFNTUNE.stPRV_b.n_DN=UPDNb[ 1];
		stAFNTUNE.stPRV_b.z_UP=UPDNb[ 2];
		stAFNTUNE.stPRV_b.z_DN=UPDNb[ 3];
		stAFNTUNE.stPRV_h.n_UP=UPDNb[ 4];
		stAFNTUNE.stPRV_h.n_DN=UPDNb[ 5];
		stAFNTUNE.stPRV_h.z_UP=UPDNb[ 6];
		stAFNTUNE.stPRV_h.z_DN=UPDNb[ 7];
		stAFNTUNE.stDIR_b.n_UP=UPDNb[ 8];
		stAFNTUNE.stDIR_b.n_DN=UPDNb[ 9];
		stAFNTUNE.stDIR_b.z_UP=UPDNb[10];
		stAFNTUNE.stDIR_b.z_DN=UPDNb[11];
		stAFNTUNE.stDIR_h.n_UP=UPDNb[12];
		stAFNTUNE.stDIR_h.n_DN=UPDNb[13];
		stAFNTUNE.stDIR_h.z_UP=UPDNb[14];
		stAFNTUNE.stDIR_h.z_DN=UPDNb[15];
		stAFNTUNE.stDEC_b.n_UP=UPDNb[16];
		stAFNTUNE.stDEC_b.n_DN=UPDNb[17];
		stAFNTUNE.stDEC_b.z_UP=UPDNb[18];
		stAFNTUNE.stDEC_b.z_DN=UPDNb[19];
		stAFNTUNE.stDEC_h.n_UP=UPDNb[20];
		stAFNTUNE.stDEC_h.n_DN=UPDNb[21];
		stAFNTUNE.stDEC_h.z_UP=UPDNb[22];
		stAFNTUNE.stDEC_h.z_DN=UPDNb[23];
		stAFNTUNE.stTOP_b.n_UP=UPDNb[24];
		stAFNTUNE.stTOP_b.n_DN=UPDNb[25];
		stAFNTUNE.stTOP_b.z_UP=UPDNb[26];
		stAFNTUNE.stTOP_b.z_DN=UPDNb[27];
		stAFNTUNE.stTOP_h.n_UP=UPDNb[28];
		stAFNTUNE.stTOP_h.n_DN=UPDNb[29];
		stAFNTUNE.stTOP_h.z_UP=UPDNb[30];
		stAFNTUNE.stTOP_h.z_DN=UPDNb[31];
	}

	switch(cur) {
		case  0 : 		stAFNTUNE.stPRV_b.n_UP=UPDNb[ 0]+5*val; break;
		case  1 : 		stAFNTUNE.stPRV_b.n_DN=UPDNb[ 1]+5*val; break;
		case  2 : 		stAFNTUNE.stPRV_b.z_UP=UPDNb[ 2]+2*val; break;
		case  3 : 		stAFNTUNE.stPRV_b.z_DN=UPDNb[ 3]+2*val; break;
		case  4 : 		stAFNTUNE.stPRV_h.n_UP=UPDNb[ 4]+5*val; break;
		case  5 : 		stAFNTUNE.stPRV_h.n_DN=UPDNb[ 5]+5*val; break;
		case  6 : 		stAFNTUNE.stPRV_h.z_UP=UPDNb[ 6]+2*val; break;
		case  7 : 		stAFNTUNE.stPRV_h.z_DN=UPDNb[ 7]+2*val; break;
		case  8 : 		stAFNTUNE.stDIR_b.n_UP=UPDNb[ 8]+5*val; break;
		case  9 : 		stAFNTUNE.stDIR_b.n_DN=UPDNb[ 9]+5*val; break;
		case 10 : 		stAFNTUNE.stDIR_b.z_UP=UPDNb[10]+2*val; break;
		case 11 : 		stAFNTUNE.stDIR_b.z_DN=UPDNb[11]+2*val; break;
		case 12 : 		stAFNTUNE.stDIR_h.n_UP=UPDNb[12]+5*val; break;
		case 13 : 		stAFNTUNE.stDIR_h.n_DN=UPDNb[13]+5*val; break;
		case 14 : 		stAFNTUNE.stDIR_h.z_UP=UPDNb[14]+2*val; break;
		case 15 : 		stAFNTUNE.stDIR_h.z_DN=UPDNb[15]+2*val; break;
		case 16 : 		stAFNTUNE.stDEC_b.n_UP=UPDNb[16]+5*val; break;
		case 17 : 		stAFNTUNE.stDEC_b.n_DN=UPDNb[17]+5*val; break;
		case 18 : 		stAFNTUNE.stDEC_b.z_UP=UPDNb[18]+2*val; break;
		case 19 : 		stAFNTUNE.stDEC_b.z_DN=UPDNb[19]+2*val; break;
		case 20 : 		stAFNTUNE.stDEC_h.n_UP=UPDNb[20]+5*val; break;
		case 21 : 		stAFNTUNE.stDEC_h.n_DN=UPDNb[21]+5*val; break;
		case 22 : 		stAFNTUNE.stDEC_h.z_UP=UPDNb[22]+2*val; break;
		case 23 : 		stAFNTUNE.stDEC_h.z_DN=UPDNb[23]+2*val; break;
		case 24 : 		stAFNTUNE.stTOP_b.n_UP=UPDNb[24]+5*val; break;
		case 25 : 		stAFNTUNE.stTOP_b.n_DN=UPDNb[25]+5*val; break;
		case 26 : 		stAFNTUNE.stTOP_b.z_UP=UPDNb[26]+2*val; break;
		case 27 : 		stAFNTUNE.stTOP_b.z_DN=UPDNb[27]+2*val; break;
		case 28 : 		stAFNTUNE.stTOP_h.n_UP=UPDNb[28]+5*val; break;
		case 29 : 		stAFNTUNE.stTOP_h.n_DN=UPDNb[29]+5*val; break;
		case 30 : 		stAFNTUNE.stTOP_h.z_UP=UPDNb[30]+2*val; break;
		case 31 : 		stAFNTUNE.stTOP_h.z_DN=UPDNb[31]+2*val; break;
	}

	UARTprintf("stAFNTUNE.stPRV_b : %3d %3d %3d %3d\r\n",stAFNTUNE.stPRV_b.n_UP,stAFNTUNE.stPRV_b.n_DN,stAFNTUNE.stPRV_b.z_UP,stAFNTUNE.stPRV_b.z_DN);
	UARTprintf("stAFNTUNE.stPRV_h : %3d %3d %3d %3d\r\n",stAFNTUNE.stPRV_h.n_UP,stAFNTUNE.stPRV_h.n_DN,stAFNTUNE.stPRV_h.z_UP,stAFNTUNE.stPRV_h.z_DN);
	UARTprintf("stAFNTUNE.stDIR_b : %3d %3d %3d %3d\r\n",stAFNTUNE.stDIR_b.n_UP,stAFNTUNE.stDIR_b.n_DN,stAFNTUNE.stDIR_b.z_UP,stAFNTUNE.stDIR_b.z_DN);
	UARTprintf("stAFNTUNE.stDIR_h : %3d %3d %3d %3d\r\n",stAFNTUNE.stDIR_h.n_UP,stAFNTUNE.stDIR_h.n_DN,stAFNTUNE.stDIR_h.z_UP,stAFNTUNE.stDIR_h.z_DN);
	UARTprintf("stAFNTUNE.stDEC_b : %3d %3d %3d %3d\r\n",stAFNTUNE.stDEC_b.n_UP,stAFNTUNE.stDEC_b.n_DN,stAFNTUNE.stDEC_b.z_UP,stAFNTUNE.stDEC_b.z_DN);
	UARTprintf("stAFNTUNE.stDEC_h : %3d %3d %3d %3d\r\n",stAFNTUNE.stDEC_h.n_UP,stAFNTUNE.stDEC_h.n_DN,stAFNTUNE.stDEC_h.z_UP,stAFNTUNE.stDEC_h.z_DN);
	UARTprintf("stAFNTUNE.stTOP_b : %3d %3d %3d %3d\r\n",stAFNTUNE.stTOP_b.n_UP,stAFNTUNE.stTOP_b.n_DN,stAFNTUNE.stTOP_b.z_UP,stAFNTUNE.stTOP_b.z_DN);
	UARTprintf("stAFNTUNE.stTOP_h : %3d %3d %3d %3d\r\n",stAFNTUNE.stTOP_h.n_UP,stAFNTUNE.stTOP_h.n_DN,stAFNTUNE.stTOP_h.z_UP,stAFNTUNE.stTOP_h.z_DN);
	UARTprintf("\r\n");
}

#endif	/* defined(__USE_AFZOOM_LENS__) && defined(__USE_X30X_TAMRON__) */

/*  FILE_END_HERE */
