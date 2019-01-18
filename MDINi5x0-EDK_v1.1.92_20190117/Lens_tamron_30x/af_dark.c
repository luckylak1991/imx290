// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if defined(__USE_AFZOOM_LENS__) && defined(__USE_X30X_TAMRON__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

typedef	struct
{
	LONG	nFltBUFF[5];
	SHORT	nDIFF[4];
	CHAR	nSIGN[4];

	SHORT	nFOKS[5];
	SHORT	nMOVE;

}	AFD_BUFF_PARAM, *PAFD_BUFF_PARAM;

typedef	struct
{
	LONG	nMIN;
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

	LONG	nCHK;

}	AFD_RATE_PARAM, *PAFD_RATE_PARAM;

typedef	struct
{
	LONG	nHIGH;
	SHORT	nPEAK;
	SHORT	nDUMY;

}	AFD_PEAK_PARAM, *PAFD_PEAK_PARAM;

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

}	AFD_NORM_PARAM, *PAFD_NORM_PARAM;

typedef	struct
{
	LONG	FLTa;
	LONG	FLTb;
	LONG	FLTo;
	LONG	FLTn;

}	AFD_DATA_PARAM, *PAFD_DATA_PARAM;

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

	LONG	nMIN;
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

	LONG	nCHK;

	LONG	nHIGH;
	LONG	xHIGH;

	SHORT	nPEAK;
	SHORT	xPEAK;

}	AFD_FILT_PARAM, *PAFD_FILT_PARAM;

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

}	AFD_STEP_COUNT, *PAFD_STEP_COUNT;

typedef	struct
{
	BYTE	sMIN;
	BYTE	sSTD;
	BYTE	sMID;
	BYTE	sMAX;

	BYTE	sFWD;
	BYTE	sBWD;
	BYTE	sTOP;

	BYTE	sCNT;

	BYTE	sSPD;
	BYTE	sCHK;

	AFD_STEP_COUNT	stDIR_I;
	AFD_STEP_COUNT	stDEC_I;
	AFD_STEP_COUNT	stDEC_D;

}	AFD_STEP_PARAM, *PAFD_STEP_PARAM;

typedef	struct
{
	BYTE	nSEL;
	BYTE	nDIR;
	BYTE	eCNT;
	BYTE	xCNT; // inner counter
	BYTE	vCNT; // main counter

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

	BYTE	DARK;
	BYTE	DUMY;

	AFD_NORM_PARAM	stNORM_y;
	AFD_DATA_PARAM	stDATA_r;
	AFD_DATA_PARAM	stDATA_l;
	AFD_DATA_PARAM	stDATA_v;
	AFD_DATA_PARAM	stDATA_b;
	AFD_DATA_PARAM	stDATA_h;
	AFD_FILT_PARAM	stOUTn_b;
	AFD_FILT_PARAM	stOUTn_h;
	AFD_STEP_PARAM	stSTEP_m;

}	AFD_PROC_PARAM, *PAFD_PROC_PARAM;

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

} 	AFD_PROCESS_STEP;

typedef enum {
	USE_HPF = 0, USE_LPF, USE_BPF

} 	AFD_PROCESS_FILT;

typedef enum {
	GOOD_DEC = 0x000008, G_DEC_OK = 0x000004, G_INC_OK = 0x000002, GOOD_INC = 0x000001,
	ZERO_DEC = 0x000080, Z_DEC_OK = 0x000040, Z_INC_OK = 0x000020, ZERO_INC = 0x000010,
	RATE_DEC = 0x000800, R_DEC_OK = 0x000400, R_INC_OK = 0x000200, RATE_INC = 0x000100,										 
	HIGH_DEC = 0x008000, HIGH_INC = 0x001000,
	GAIN_MAX = 0x040000, GAIN_MID = 0x020000, GAIN_FLT = 0x010000,
	GOOD_END = 0x800000

} 	AFD_STATUS_RATE;

typedef	struct
{
	SHORT	n_UP;
	SHORT	n_DN;
	SHORT	z_UP;
	SHORT	z_DN;

}	AFD_TUNE_RATIO, *PAFD_TUNE_RATIO;

typedef	struct
{
	BYTE	r_MIN;
	BYTE	r_STD;
	BYTE	r_MID;
	BYTE	r_MAX;

}	AFD_TUNE_COUNT, *PAFD_TUNE_COUNT;

typedef	struct
{
	LONG	n_GAIN;
	LONG	n_YSUM;
	LONG	n_FLTr;

	LONG	c_FLTb;
	LONG	c_FLTh;
	LONG	c_GMIN;
	LONG	c_GMAX;

	SHORT	h_ZOOM;
	SHORT	h_FOKS;

	AFD_TUNE_RATIO	stPRV_b;
	AFD_TUNE_RATIO	stPRV_h;

	AFD_TUNE_RATIO	stDIR_b;
	AFD_TUNE_RATIO	stDIR_h;

	AFD_TUNE_RATIO	stDEC_b;
	AFD_TUNE_RATIO	stDEC_h;

	AFD_TUNE_RATIO	stTOP_b;
	AFD_TUNE_RATIO	stTOP_h;

}	AFD_TUNE_PARAM, *PAFD_TUNE_PARAM;

#define		MIN3(a,b,c)				(MIN(MIN(a,b),c))
#define		MAX3(a,b,c)				(MAX(MAX(a,b),c))

#define		MIN4(a,b,c,d)			(MIN(MIN(a,b),MIN(c,d)))
#define		MAX4(a,b,c,d)			(MAX(MAX(a,b),MAX(c,d)))

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static AFD_PROC_PARAM stAFDARK;
static AFD_TUNE_PARAM stAFDTUNE = {
	//n_GAIN	n_YSUM	n_FLTr	c_FLTb	c_FLTh	c_GMIN	c_GMAX
	0,			0,		0,		500,		2048,	32767,	65535,
	//h_ZOOM	h_FOKS;
	0,			0,
	{  50,    -50,      1,     -1}, // stPRV_b : n_UP n_DN z_UP z_DN;
	{  50,    -50,      1,     -1}, // stDIR_b
	{  50,    -50,     10,    -10}, // stPRV_h
	{  50,    -50,     10,    -10}, // stDIR_h
	{  50,    -50,     10,    -10}, // stDEC_b
	{  50,    -50,     10,    -10}, // stDEC_h
	{  20,    -10,      1,     -1}, // stTOP_b
	{  20,    -10,      1,     -1} // stTOP_h
};

static LONG xVAL;

#define xVAL_DARK_LL 1000
#define xVAL_DARK_N 500

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
static void AFDARK_ClrJudgePEAK(void);

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_GetAFOPDWIND(PAF_CTRL_PARAM pCTRL)
{
	FLOAT nGAIN, nYSUM, nHPFr;

	stAFDARK.stDATA_r.FLTa = (pCTRL->stFILT.RPFa)>>0;
	stAFDARK.stDATA_v.FLTa = (pCTRL->stFILT.VPFa)>>0;
	stAFDARK.stDATA_b.FLTa = (pCTRL->stFILT.BPFa)>>0;
	stAFDARK.stDATA_r.FLTb = (pCTRL->stFILT.RPFb)<<2;
	stAFDARK.stDATA_v.FLTb = (pCTRL->stFILT.VPFb)<<2;
	stAFDARK.stDATA_b.FLTb = (pCTRL->stFILT.BPFb)<<2;

	stAFDARK.stNORM_y.YCNT = (pCTRL->stFILT.CNTa)>>0;
	stAFDARK.stDATA_h.FLTa = (pCTRL->stFILT.HPFa)>>0;
	stAFDARK.stDATA_h.FLTb = (pCTRL->stFILT.HPFb)<<2;

	stAFDARK.stDATA_r.FLTo = stAFDARK.stDATA_r.FLTa;
	stAFDARK.stDATA_b.FLTo = stAFDARK.stDATA_b.FLTa+stAFDARK.stDATA_v.FLTa;
	stAFDARK.stDATA_h.FLTo = stAFDARK.stDATA_h.FLTa+stAFDARK.stDATA_v.FLTa;

	stAFDARK.stNORM_y.YPRV = (stAFDARK.stNORM_y.YSUM);
	stAFDARK.stNORM_y.YSUM = (pCTRL->stFILT.YSUM)>>0;
	stAFDARK.stNORM_y.GAIN = (pCTRL->stFILT.GAIN)>>0;
	stAFDARK.stNORM_y.YREF = (AECTRL_GetBrightness());

	nGAIN = ((FLOAT)AECTRL_GetGAINMin())/stAFDARK.stNORM_y.GAIN; // stGAIN.min / pCTRL->stFILT.GAIN = stGAIN.gain

	if (stAFDARK.stNORM_y.GAIN>stAFDTUNE.c_GMAX) // stAFDTUNE.c_GMAX = 65535
		 nYSUM = ((FLOAT)stAFDARK.stNORM_y.YSUM)/stAFDARK.stNORM_y.YREF;
	else nYSUM = ((FLOAT)stAFDARK.stNORM_y.YREF)/stAFDARK.stNORM_y.YSUM;

	stAFDARK.stNORM_y.FLTb = (LONG)(nGAIN*nYSUM*stAFDARK.stDATA_b.FLTo+0.5);

	if (stAFDTUNE.n_GAIN==0) stAFDTUNE.n_GAIN = stAFDARK.stNORM_y.GAIN;
		nGAIN = ((FLOAT)stAFDTUNE.n_GAIN)/stAFDARK.stNORM_y.GAIN;

	if (stAFDTUNE.n_YSUM==0) stAFDTUNE.n_YSUM = stAFDARK.stNORM_y.YSUM;
		nYSUM = ((FLOAT)stAFDTUNE.n_YSUM)/stAFDARK.stNORM_y.YSUM;
	stAFDARK.stDATA_b.FLTo += stAFDARK.stDATA_b.FLTb+stAFDARK.stDATA_v.FLTb;
	stAFDARK.stDATA_h.FLTo += stAFDARK.stDATA_h.FLTb+stAFDARK.stDATA_v.FLTb;

	stAFDARK.stDATA_r.FLTn = (LONG)(nGAIN*nYSUM*stAFDARK.stDATA_r.FLTo+0.5);
	stAFDARK.stDATA_b.FLTn = (LONG)(nGAIN*nYSUM*stAFDARK.stDATA_b.FLTo+0.5);
	stAFDARK.stDATA_h.FLTn = (LONG)(nGAIN*nYSUM*stAFDARK.stDATA_h.FLTo+0.5);

	if (stAFDARK.stNORM_y.GAIN>stAFDTUNE.c_GMIN && stAFDARK.stNORM_y.FLTb<stAFDTUNE.c_FLTb)
		 stAFDARK.stNORM_y.nCHK = 1;
	else stAFDARK.stNORM_y.nCHK = 0;

	if (stAFDARK.stNORM_y.GAIN>stAFDTUNE.c_GMIN)	stAFDARK.DARK = 1;
	else										stAFDARK.DARK = 0;

	if (stAFDARK.stNORM_y.nCHK && stAFDTUNE.n_FLTr==0) stAFDTUNE.n_FLTr = stAFDARK.stDATA_r.FLTn;

	nHPFr = (stAFDARK.stNORM_y.nCHK)? ((FLOAT)stAFDTUNE.n_FLTr)/stAFDARK.stDATA_r.FLTn : 1.0f;

	stAFDARK.stOUTn_b.FLTn = (LONG)(nHPFr*stAFDARK.stDATA_b.FLTn+0.5); // use Filter
	stAFDARK.stOUTn_h.FLTn = (LONG)(nHPFr*stAFDARK.stDATA_h.FLTn+0.5);

	stAFDARK.nDIR = LENS_GetDirectionFOKS();
	stAFDARK.fVAL = LENS_GetCurPositionFOKS();
	stAFDARK.zVAL = LENS_GetCurPositionZOOM();
	stAFDARK.ZOOM = LENS_GetRelPositionZOOM();
	stAFDARK.KNEE = LENS_GetKneePositionZOOM();

	xVAL = (pCTRL->nMOD==DARK_LL)? xVAL_DARK_LL : xVAL_DARK_N; // xVAL th.
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFDARK_IsDriveEnd(void)
{
	if (stAFDARK.nDIR==fNEAR && stAFDARK.fVAL<=stAFDARK.fMIN) return TRUE;
	if (stAFDARK.nDIR== fFAR && stAFDARK.fVAL>=stAFDARK.fMAX) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFDARK_SetChangeDIR(void)
{
	stAFDARK.nDIR = (stAFDARK.nDIR==fNEAR)? fFAR : fNEAR;
	LENS_SetDirectionFOKS(stAFDARK.nDIR);
	return stAFDARK.nDIR;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_GetDriveSTEP(BYTE nID)
{
	WORD gap = (WORD)ABS(stAFDARK.peak-stAFDARK.fVAL);
	WORD quo = (WORD)ABS(stAFDARK.peak-stAFDARK.fVAL)/stAFDARK.step;
	WORD rem = (WORD)ABS(stAFDARK.peak-stAFDARK.fVAL)%stAFDARK.step;
	WORD max = (WORD)stAFDARK.stSTEP_m.sBWD;
	WORD min = (WORD)stAFDARK.stSTEP_m.sMIN;

	switch (nID) {
		case DRV_STEP_MIN:	stAFDARK.step = (WORD)stAFDARK.stSTEP_m.sMIN; break;
		case DRV_STEP_STD:	stAFDARK.step = (WORD)stAFDARK.stSTEP_m.sSTD; break;
		case DRV_STEP_MID:	stAFDARK.step = (WORD)stAFDARK.stSTEP_m.sMID; break;
		case DRV_STEP_MAX:	stAFDARK.step = (WORD)stAFDARK.stSTEP_m.sMAX; break;

		case DRV_STEP_FWD:	stAFDARK.step = (WORD)stAFDARK.stSTEP_m.sFWD; break;
		case DRV_STEP_BWD:	stAFDARK.step = (WORD)stAFDARK.stSTEP_m.sBWD; break;
		case DRV_STEP_TOP:	stAFDARK.step = (WORD)stAFDARK.stSTEP_m.sTOP; break;

		case DRV_STEP_DIV:	stAFDARK.step = MIN(MAX((gap+3)/4,min), max); break;
		case DRV_STEP_REM:	stAFDARK.step = (quo)? stAFDARK.step : rem; break; //reminder
	}

	if ((nID&0xf0)==DRV_STEP_MIN) stAFDARK.stSTEP_m.sSPD = nID;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_GetDriveRATE(BYTE nID)
{
	PBYTE pTUNEb = NULL, pTUNEh = NULL;

	switch (nID) {
		case DRV_SEARCH_FAR: case PRE_SEARCH_FAR:
		case DRV_SEARCH_NAR: case PRE_SEARCH_NAR:
			pTUNEb = (PBYTE)&stAFDTUNE.stPRV_b;	pTUNEh = (PBYTE)&stAFDTUNE.stPRV_h;	break;

		case DRV_SEARCH_DIR: case PRE_SEARCH_DIR:
			pTUNEb = (PBYTE)&stAFDTUNE.stDIR_b;	pTUNEh = (PBYTE)&stAFDTUNE.stDIR_h;	break;

		case DRV_CLIMB_TOP2: case PRE_CLIMB_TOP2:
			pTUNEb = (PBYTE)&stAFDTUNE.stDEC_b;	pTUNEh = (PBYTE)&stAFDTUNE.stDEC_h;	break;

		case DRV_CLIMB_PEAK: case PRE_CLIMB_PEAK:
			pTUNEb = (PBYTE)&stAFDTUNE.stTOP_b;	pTUNEh = (PBYTE)&stAFDTUNE.stTOP_h;	break;
	}

	memcpy((PBYTE)&stAFDARK.stOUTn_b.n_UP, (PBYTE)pTUNEb, sizeof(AFD_TUNE_RATIO));
	memcpy((PBYTE)&stAFDARK.stOUTn_h.n_UP, (PBYTE)pTUNEh, sizeof(AFD_TUNE_RATIO));
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_SetDriveSTEP(BYTE nID)
{
	stAFDARK.goal = stAFDARK.fVAL;

	if (stAFDARK.nDIR==fFAR) stAFDARK.goal += stAFDARK.step;
	else					 stAFDARK.goal -= stAFDARK.step;

	stAFDARK.goal = MIN(stAFDARK.goal, stAFDARK.fMAX);
	stAFDARK.goal = MAX(stAFDARK.goal, stAFDARK.fMIN);

	LENS_SetDriveFOKS(stAFDARK.goal);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_SetDrivePEAK(void)
{
	stAFDARK.goal = stAFDARK.peak;

	stAFDARK.goal = MIN(stAFDARK.goal, stAFDARK.fMAX);
	stAFDARK.goal = MAX(stAFDARK.goal, stAFDARK.fMIN);

	LENS_SetDriveFOKS(stAFDARK.goal);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_ClrDrvCountSPD(PAFD_STEP_COUNT pCNT)
{
	SHORT sFWD = LENS_GetAFDriveSTEP(DRV_STEP_FWD); // get from lens zoom position
	LONG  GAIN = stAFDARK.stNORM_y.GAIN, KNEE = MIN(GAIN,8192);

	if (GAIN>1023) {
		sFWD -= (SHORT)(6.0f*(FLOAT)KNEE/ 1024+0.5); // set sFWD by GAIN
	}
	if (GAIN>8191) {
		sFWD -= (SHORT)(6.0f*(FLOAT)GAIN/16384+0.5); // set by GAIN
	}
	stAFDARK.stSTEP_m.sFWD = MAX(sFWD,stAFDARK.stSTEP_m.sSTD*3);

	// Clear others counter
	switch (stAFDARK.stSTEP_m.sSPD) {
		case DRV_STEP_MIN:	pCNT->xSTD = pCNT->xMID = pCNT->xMAX = pCNT->xFWD = 0; break;
		case DRV_STEP_STD:	pCNT->xMIN = pCNT->xMID = pCNT->xMAX = pCNT->xFWD = 0; break;
		case DRV_STEP_MID:	pCNT->xMIN = pCNT->xSTD = pCNT->xMAX = pCNT->xFWD = 0; break;
		case DRV_STEP_MAX:	pCNT->xMIN = pCNT->xSTD = pCNT->xMID = pCNT->xFWD = 0; break;
		case DRV_STEP_FWD:	pCNT->xMIN = pCNT->xSTD = pCNT->xMID = pCNT->xMAX = 0; break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DirDrvIncSPD_BI(BYTE nID)
{
	PAFD_STEP_COUNT pCNT = &stAFDARK.stSTEP_m.stDIR_I;

	AFDARK_ClrDrvCountSPD(pCNT);

	switch (stAFDARK.stSTEP_m.sSPD) {
		case DRV_STEP_MIN:		AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;

		case DRV_STEP_STD:		if (pCNT->xSTD<6) pCNT->xSTD++;
			if (pCNT->xSTD==6)	AFDARK_GetDriveSTEP(DRV_STEP_MID);
			else				AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;

		case DRV_STEP_MID:		if (pCNT->xMID<3) pCNT->xMID++;
			if (pCNT->xMID==3)	AFDARK_GetDriveSTEP(DRV_STEP_MAX);
			else				AFDARK_GetDriveSTEP(DRV_STEP_MID);	break;

		case DRV_STEP_MAX:		if (pCNT->xMAX<2) pCNT->xMAX++;
			if (pCNT->xMAX==2)	AFDARK_GetDriveSTEP(DRV_STEP_FWD);
			else				AFDARK_GetDriveSTEP(DRV_STEP_MAX);	break;

		case DRV_STEP_FWD:		if (pCNT->xFWD<2) pCNT->xFWD++;
								AFDARK_GetDriveSTEP(DRV_STEP_FWD);	break;
	}
	if (pCNT->xFWD==1) AFDARK_ClrJudgePEAK();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DecDrvIncSPD_GX(BYTE nID)
{
	PAFD_STEP_COUNT pCNT = &stAFDARK.stSTEP_m.stDEC_I;

	AFDARK_ClrDrvCountSPD(pCNT);	// clear counter
	memset(&stAFDARK.stSTEP_m.stDEC_D, 0, sizeof(AFD_STEP_COUNT));

	if (stAFDARK.stSTEP_m.sSPD==DRV_STEP_MAX) stAFDARK.stSTEP_m.sCHK = 1;
	if (stAFDARK.stSTEP_m.sSPD==DRV_STEP_FWD) stAFDARK.stSTEP_m.sCHK = 1;

	if (stAFDARK.stSTEP_m.sCHK) {
		switch (stAFDARK.stSTEP_m.sSPD) {
			case DRV_STEP_MIN:		AFDARK_GetDriveSTEP(DRV_STEP_MIN);	break;

			case DRV_STEP_STD:		AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		AFDARK_GetDriveSTEP(DRV_STEP_MID);	break;

			case DRV_STEP_MAX:		if (pCNT->xMAX<4) pCNT->xMAX++;
				if (pCNT->xMAX==4)	AFDARK_GetDriveSTEP(DRV_STEP_MID);
				else				AFDARK_GetDriveSTEP(DRV_STEP_MAX);	break;

			case DRV_STEP_FWD:		AFDARK_GetDriveSTEP(DRV_STEP_MAX);	break;
		}
	}
	else {
		switch (stAFDARK.stSTEP_m.sSPD) {
			case DRV_STEP_MIN:		AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_STD:		if (pCNT->xSTD<4) pCNT->xSTD++;
				if (pCNT->xSTD==4)	AFDARK_GetDriveSTEP(DRV_STEP_MID);
				else				AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		AFDARK_GetDriveSTEP(DRV_STEP_MID);	break;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DecDrvIncSPD_BI(BYTE nID)
{
	PAFD_STEP_COUNT pCNT = &stAFDARK.stSTEP_m.stDEC_I;

	AFDARK_ClrDrvCountSPD(pCNT);
	memset(&stAFDARK.stSTEP_m.stDEC_D, 0, sizeof(AFD_STEP_COUNT));

	switch (stAFDARK.stSTEP_m.sSPD) {
		case DRV_STEP_MIN:		if (pCNT->xMIN<4) pCNT->xMIN++;
			if (pCNT->xMIN==4)	AFDARK_GetDriveSTEP(DRV_STEP_STD);
			else				AFDARK_GetDriveSTEP(DRV_STEP_MIN);	break;

		case DRV_STEP_STD:		if (pCNT->xSTD<6) pCNT->xSTD++;
			if (pCNT->xSTD==6)	AFDARK_GetDriveSTEP(DRV_STEP_MID);
			else				AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;

		case DRV_STEP_MID:		if (pCNT->xMID<3) pCNT->xMID++;
			if (pCNT->xMID==3)	AFDARK_GetDriveSTEP(DRV_STEP_MAX);
			else				AFDARK_GetDriveSTEP(DRV_STEP_MID);	break;

		case DRV_STEP_MAX:		if (pCNT->xMAX<2) pCNT->xMAX++;
			if (pCNT->xMAX==2)	AFDARK_GetDriveSTEP(DRV_STEP_FWD);
			else				AFDARK_GetDriveSTEP(DRV_STEP_MAX);	break;

		case DRV_STEP_FWD:		AFDARK_GetDriveSTEP(DRV_STEP_FWD);	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DecDrvDecSPD_GD(BYTE nID)
{
	PAFD_STEP_COUNT pCNT = &stAFDARK.stSTEP_m.stDEC_D;

	AFDARK_ClrDrvCountSPD(pCNT);
	memset(&stAFDARK.stSTEP_m.stDEC_I, 0, sizeof(AFD_STEP_COUNT));

	if (stAFDARK.stSTEP_m.sSPD==DRV_STEP_MAX) stAFDARK.stSTEP_m.sCHK = 1;
	if (stAFDARK.stSTEP_m.sSPD==DRV_STEP_FWD) stAFDARK.stSTEP_m.sCHK = 1;

	if (stAFDARK.stSTEP_m.sCHK) {
		switch (stAFDARK.stSTEP_m.sSPD) {
			case DRV_STEP_MIN:		AFDARK_GetDriveSTEP(DRV_STEP_MIN);	break;

			case DRV_STEP_STD:		if (pCNT->xSTD<4) pCNT->xSTD++;
				if (pCNT->xSTD==4)	AFDARK_GetDriveSTEP(DRV_STEP_MIN);
				else				AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MAX:		AFDARK_GetDriveSTEP(DRV_STEP_MID);	break;

			case DRV_STEP_FWD:		AFDARK_GetDriveSTEP(DRV_STEP_MAX);	break;
		}
	}
	else {
		switch (stAFDARK.stSTEP_m.sSPD) {
			case DRV_STEP_MIN:		AFDARK_GetDriveSTEP(DRV_STEP_MIN);	break;

			case DRV_STEP_STD:		if (pCNT->xSTD<4) pCNT->xSTD++;
				if (pCNT->xSTD==4)	AFDARK_GetDriveSTEP(DRV_STEP_MIN);
				else				AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		if (pCNT->xMID<2) pCNT->xMID++;
				if (pCNT->xMID==2)	AFDARK_GetDriveSTEP(DRV_STEP_STD);
				else				AFDARK_GetDriveSTEP(DRV_STEP_MID);	break;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DecDrvStdSPD_XX(BYTE nID)
{
	memset(&stAFDARK.stSTEP_m.stDEC_D, 0, sizeof(AFD_STEP_COUNT));
	memset(&stAFDARK.stSTEP_m.stDEC_I, 0, sizeof(AFD_STEP_COUNT));

	switch (stAFDARK.stSTEP_m.sSPD) {
		case DRV_STEP_MIN:		AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_SetDrvCurSPD_XX(BYTE nID)
{
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_ClrJudgeRATE(void)
{
	// clear MIN-MAX info
	stAFDARK.stOUTn_h.nMIN = stAFDARK.stOUTn_h.FLTn;
	stAFDARK.stOUTn_h.nMAX = stAFDARK.stOUTn_h.FLTn;
	stAFDARK.stOUTn_h.zMIN = stAFDARK.stOUTn_h.FLTn;
	stAFDARK.stOUTn_h.zMAX = stAFDARK.stOUTn_h.FLTn;

	stAFDARK.stOUTn_b.nMIN = stAFDARK.stOUTn_b.FLTn;
	stAFDARK.stOUTn_b.nMAX = stAFDARK.stOUTn_b.FLTn;
	stAFDARK.stOUTn_b.zMIN = stAFDARK.stOUTn_b.FLTn;
	stAFDARK.stOUTn_b.zMAX = stAFDARK.stOUTn_b.FLTn;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_ClrJudgePEAK(void)
{
	// clear PEAK-filt info
	stAFDARK.stOUTn_h.nHIGH = stAFDARK.stOUTn_h.FLTn;
	stAFDARK.stOUTn_b.nHIGH = stAFDARK.stOUTn_b.FLTn;

	// clear PEAK-foks info
	stAFDARK.stOUTn_h.nPEAK = stAFDARK.fVAL;
	stAFDARK.stOUTn_h.xPEAK = stAFDARK.fVAL;
	stAFDARK.stOUTn_b.nPEAK = stAFDARK.fVAL;
	stAFDARK.stOUTn_b.xPEAK = stAFDARK.fVAL;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_ClrJudgeINFO(void)
{
	stAFDARK.stOUTn_h.nCHK = 0;
	stAFDARK.stOUTn_b.nCHK = 0;

//	stAFDARK.stSTEP_m.sCNT = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_ClrJudgeBUFF(void)
{
	memset(stAFDARK.stOUTn_h.nFltBUFF, 0, sizeof(AFD_BUFF_PARAM));
	memset(stAFDARK.stOUTn_b.nFltBUFF, 0, sizeof(AFD_BUFF_PARAM));

	stAFDARK.xCNT = 0;	stAFDARK.vCNT &= 0x80;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_MovShiftBUFF(BYTE nID)
{
	BYTE i;

	for (i=1; i<nID; i++) {
		stAFDARK.stOUTn_h.nFltBUFF[i-1] = stAFDARK.stOUTn_h.nFltBUFF[i];
		stAFDARK.stOUTn_b.nFltBUFF[i-1] = stAFDARK.stOUTn_b.nFltBUFF[i];

		stAFDARK.stOUTn_h.nFOKS[i-1] = stAFDARK.stOUTn_h.nFOKS[i];
		stAFDARK.stOUTn_b.nFOKS[i-1] = stAFDARK.stOUTn_b.nFOKS[i];
	}
}
//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_GetPrvJudgeBUFF(PAFD_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap; BYTE i; PLONG pBUFF = (PLONG)pFILT->nFltBUFF;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFDARK.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	pBUFF[nID] = pFILT->FLTn; pFILT->nFOKS[nID] = stAFDARK.fVAL;
	if (nID<2) return;

	for (i=0; i<nID; i++) {
		pFILT->nDIFF[i] = (LONG)(pBUFF[i+1]-pBUFF[i]);

		if		(pFILT->nDIFF[i]>0)	pFILT->nSIGN[i] =  1;
		else if (pFILT->nDIFF[i]<0)	pFILT->nSIGN[i] = -1;
		else						pFILT->nSIGN[i] =  0;
	}

	pFILT->nMOVE = MAX3(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2])
				 - MIN3(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2]);

	stAFDARK.fMOV = pFILT->nMOVE/nID;
	if (pFILT->nSIGN[0]==pFILT->nSIGN[1]) {
		gap = (pFILT->nMOVE)? (15.0f*nID)/pFILT->nMOVE : 1.0f;
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

	if (pFILT->nINC>=(LONG)pFILT->n_UP)	pFILT->nCHK |= ( GOOD_INC|G_INC_OK);
	else								pFILT->nCHK &= (~GOOD_INC);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN)	pFILT->nCHK |= ( GOOD_DEC);
	else								pFILT->nCHK &= (~GOOD_DEC);

	if (pFILT->zINC>=(LONG)pFILT->z_UP)	pFILT->nCHK |= ( ZERO_INC);
	else								pFILT->nCHK &= (~ZERO_INC);

	if (pFILT->zDEC<=(LONG)pFILT->z_DN)	pFILT->nCHK |= ( ZERO_DEC);
	else								pFILT->nCHK &= (~ZERO_DEC);

	pFILT->FLTx = pFILT->FLTd = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_GetDirJudgeBUFF(PAFD_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap;	PLONG pBUFF = (PLONG)pFILT->nFltBUFF;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFDARK.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	pBUFF[nID] = pFILT->FLTn; pFILT->nFOKS[nID] = stAFDARK.fVAL;
	if (nID<3) return;

	pFILT->nMOVE = MAX4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3])
				 - MIN4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3]);

	stAFDARK.fMOV = pFILT->nMOVE/nID;

	gap = MAX((15.0f*nID)/pFILT->nMOVE, 1.0f);

	pFILT->zMIN = (pBUFF[0]+pBUFF[1]+pBUFF[2]+pBUFF[3]+2)/4;
	pFILT->zMAX = (pBUFF[0]+pBUFF[1]+pBUFF[2]+pBUFF[3]+2)/4;
	pFILT->FLTr = (pFILT->FLTn);
	rate = (pFILT->nMIN)? ((FLOAT)pFILT->FLTn/pFILT->nMIN-1)*xVAL : 0;
	pFILT->nINC = (LONG)((rate<0)? 0 : rate+0.5);

	rate = (pFILT->nMAX)? ((FLOAT)pFILT->FLTn/pFILT->nMAX-1)*xVAL : 0;
	pFILT->nDEC = (LONG)((rate>0)? 0 : rate-0.5);

	rate = (pFILT->zMIN)? ((FLOAT)pFILT->FLTr/pFILT->zMIN-1)*1000 : 0;
	pFILT->zINC = (LONG)((rate*gap<0)? 0 : rate*gap+0.5);

	rate = (pFILT->zMAX)? ((FLOAT)pFILT->FLTr/pFILT->zMAX-1)*1000 : 0;
	pFILT->zDEC = (LONG)((rate*gap>0)? 0 : rate*gap-0.5);

	if (pFILT->nINC>=(LONG)pFILT->n_UP || pFILT->zINC>=(LONG)pFILT->n_UP)
			pFILT->nCHK |= ( GOOD_INC);
	else	pFILT->nCHK &= (~GOOD_INC);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN || pFILT->zDEC<=(LONG)pFILT->n_DN)
			pFILT->nCHK |= ( GOOD_DEC);
	else	pFILT->nCHK &= (~GOOD_DEC);

	if (pFILT->nINC>=(LONG)pFILT->z_UP || pFILT->zINC>=(LONG)pFILT->z_UP)
			pFILT->nCHK |= ( Z_INC_OK);

	if (pFILT->nDEC<=(LONG)pFILT->z_DN || pFILT->zDEC<=(LONG)pFILT->z_DN)
			pFILT->nCHK |= ( Z_DEC_OK);

	pFILT->FLTx = pFILT->FLTd = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_InvDirJudgeBUFF(PAFD_FILT_PARAM pFILT, BYTE nID)
{
	PLONG pBUFF = (PLONG)pFILT->nFltBUFF;
	FLOAT rate, gap = 15.0f/stAFDARK.step;

	rate = (pBUFF[2])? ((FLOAT)pFILT->FLTn/pBUFF[2]-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate*gap>0)? 0 : rate*gap-0.5);

	if (pFILT->nDEC<=-1)	pFILT->nCHK |= ( ZERO_DEC);
	else					pFILT->nCHK &= (~ZERO_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_GetDecJudgeBUFF(PAFD_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap;	PLONG pBUFF = (PLONG)pFILT->nFltBUFF;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFDARK.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	pBUFF[nID] = pFILT->FLTn; pFILT->nFOKS[nID] = stAFDARK.fVAL;
	if (nID<3) return;

	pFILT->nMOVE = MAX4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3])
				 - MIN4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3]);

	stAFDARK.fMOV = pFILT->nMOVE/nID;

	gap = (15.0f*nID)/pFILT->nMOVE;

	pFILT->zMAX = MAX4(pBUFF[0],pBUFF[1],pBUFF[2],pBUFF[3]);
	pFILT->zMIN = (pBUFF[0]+pBUFF[1]+pBUFF[2]+pBUFF[3]+2)/4;

	pFILT->FLTr = (pFILT->FLTn);
	pFILT->FLTx = MAX(pFILT->FLTd-MAX((LONG)(5.0f/gap+0.5),5),50);

	rate = (pFILT->zMIN)? ((FLOAT)pFILT->FLTr/pFILT->zMIN-1)*2000 : 0;
	pFILT->FLTd = (LONG)((rate*gap<0)? 0 : rate*gap+0.5);

	rate = (pFILT->zMIN)? ((FLOAT)pFILT->FLTr/pFILT->zMIN-1)*2000 : 0;
	pFILT->zINC = (LONG)((rate*gap<0)? 0 : rate*gap+0.5);

	rate = (pFILT->zMAX)? ((FLOAT)pFILT->FLTn/pFILT->zMAX-1)*1000 : 0;
	pFILT->zDEC = (LONG)((rate*gap>0)? 0 : rate*gap-0.5);

	rate = (pFILT->nMIN)? ((FLOAT)pFILT->FLTn/pFILT->nMIN-1)*xVAL : 0;
	pFILT->nINC = (LONG)((rate<0)? 0 : rate+0.5);

	rate = (pFILT->nMAX)? ((FLOAT)pFILT->FLTn/pFILT->nMAX-1)*xVAL : 0;
	pFILT->nDEC = (LONG)((rate>0)? 0 : rate-0.5);

	if (pFILT->zINC>=(LONG)pFILT->n_UP)	pFILT->nCHK |= ( GOOD_INC|G_INC_OK);
	else								pFILT->nCHK &= (~GOOD_INC);

	if (pFILT->zDEC<=(LONG)pFILT->n_DN)	pFILT->nCHK |= ( GOOD_DEC|G_DEC_OK);
	else								pFILT->nCHK &= (~GOOD_DEC);

	if (pFILT->zINC>=(LONG)pFILT->z_UP)	pFILT->nCHK |= ( ZERO_INC|Z_INC_OK);
	else								pFILT->nCHK &= (~ZERO_INC);

	if (pFILT->zDEC<=(LONG)pFILT->z_DN)	pFILT->nCHK |= ( ZERO_DEC|Z_DEC_OK);
	else								pFILT->nCHK &= (~ZERO_DEC);

	if (pFILT->nINC>=(LONG)pFILT->n_UP)	pFILT->nCHK |= ( HIGH_INC);
	else								pFILT->nCHK &= (~HIGH_INC);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN)	pFILT->nCHK |= ( HIGH_DEC);
	else								pFILT->nCHK &= (~HIGH_DEC);

	if ((pFILT->nCHK&HIGH_INC)==0)	pFILT->nCHK &= (~(GOOD_INC));
	else if (stAFDARK.DARK==TRUE)	pFILT->nCHK |= ( (GOOD_INC|G_INC_OK));

	if ((pFILT->nCHK&G_INC_OK)==0)	return;


	if (pFILT->FLTd<=(LONG)pFILT->FLTx)	pFILT->nCHK |= ( RATE_DEC|R_DEC_OK);
	else								pFILT->nCHK &= (~RATE_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_InvDecJudgeBUFF(PAFD_FILT_PARAM pFILT, BYTE nID)
{
	PLONG pBUFF = (PLONG)pFILT->nFltBUFF;
	FLOAT rate, gap = 15.0f/stAFDARK.step;

	rate = (pBUFF[2])? ((FLOAT)pFILT->FLTn/pBUFF[2]-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate*gap>0)? rate*gap : rate*gap-0.5);

	if (pFILT->nDEC<=-1)	pFILT->nCHK |= ( ZERO_DEC);
	else					pFILT->nCHK &= (~ZERO_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_GetTopJudgeBUFF(PAFD_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFDARK.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	rate = (pFILT->nMIN)? ((FLOAT)pFILT->FLTn/pFILT->nMIN-1)*1000 : 0;
	pFILT->nINC = (LONG)((rate<0)? 0 : rate+0.5);

	rate = (pFILT->nMAX)? ((FLOAT)pFILT->FLTn/pFILT->nMAX-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate>0)? 0 : rate-0.5);

	if (pFILT->nINC>=(LONG)pFILT->n_UP)	pFILT->nCHK |= ( GOOD_INC);
	else								pFILT->nCHK &= (~GOOD_INC);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN)	pFILT->nCHK |= ( GOOD_DEC);
	else								pFILT->nCHK &= (~GOOD_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_InvTopJudgeBUFF(PAFD_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate;

	rate = (pFILT->nMAX)? ((FLOAT)pFILT->FLTn/pFILT->nMAX-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate>0)? rate+0.5 : rate-0.5);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN)	pFILT->nCHK |= ( ZERO_DEC);
	else								pFILT->nCHK &= (~ZERO_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_InvEndJudgeBUFF(PAFD_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate = ((FLOAT)pFILT->xHIGH/pFILT->FLTn-1);

	rate *= ((nID==USE_HPF)? 500 : 1000);
	pFILT->nDEC = (LONG)((rate<0)? rate-0.5 : rate+0.5);

	if (ABS(pFILT->nDEC)<100)	pFILT->nCHK |= ( GOOD_END);
	else						pFILT->nCHK &= (~GOOD_END);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_GetJudge_FAR(void)
{
	PAFD_FILT_PARAM pFILTx = &stAFDARK.stOUTn_b;
	PAFD_FILT_PARAM pFILTb = &stAFDARK.stOUTn_b;

	if (stAFDARK.vCNT&0x80) pFILTx = &stAFDARK.stOUTn_h;

	AFDARK_GetDriveRATE(DRV_SEARCH_FAR);

	AFDARK_GetPrvJudgeBUFF(&stAFDARK.stOUTn_h, stAFDARK.xCNT);
	AFDARK_GetPrvJudgeBUFF(&stAFDARK.stOUTn_b, stAFDARK.xCNT);

	if (AFDARK_IsDriveEnd()==FALSE) {
		if(stAFDARK.xCNT != 0xff) stAFDARK.xCNT++;	if (stAFDARK.xCNT<3) return; // inner counter
		if((stAFDARK.vCNT&0xf) != 0xf) stAFDARK.vCNT++; // main counter
	}

	if (AFDARK_IsDriveEnd()==TRUE) {
		if(stAFDARK.eCNT != 0xff) stAFDARK.eCNT++;			AFDARK_ClrJudgeRATE();
		stAFDARK.cur = INV_SEARCH_FAR;	stAFDARK.nxt = PRE_SEARCH_DIR;
	}
	else if (pFILTx->nCHK&GOOD_DEC && (pFILTb->nCHK&ZERO_INC)==0) {
		stAFDARK.cur = INV_SEARCH_FAR;	stAFDARK.nxt = END_SEARCH_FAR;
	}
	else if (pFILTx->nCHK&GOOD_INC) {
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;
	}
	else if ((stAFDARK.vCNT&0xf)==5) {
		stAFDARK.cur = INV_SEARCH_FAR;	stAFDARK.nxt = PRE_SEARCH_DIR;
	}
	else {
		if(stAFDARK.xCNT) stAFDARK.xCNT--;			AFDARK_MovShiftBUFF(3);
		stAFDARK.cur = DRV_SEARCH_FAR;	stAFDARK.nxt = DRV_SEARCH_FAR;
	}

	if ((stAFDARK.vCNT&0xf)==5 && pFILTb->nCHK&GOOD_DEC) {
		stAFDARK.cur = INV_SEARCH_FAR;	stAFDARK.nxt = END_SEARCH_FAR;
	}
	if ((stAFDARK.vCNT&0xf)==5 && pFILTb->nCHK&GOOD_INC) {
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;
	}

	if (stAFDARK.cur==DRV_CLIMB_TOP2) AFDARK_ClrJudgePEAK();
	if (stAFDARK.cur==INV_SEARCH_FAR) AFDARK_ClrJudgeBUFF();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_GetJudge_NAR(void)
{
	PAFD_FILT_PARAM pFILTx = &stAFDARK.stOUTn_b;
	PAFD_FILT_PARAM pFILTb = &stAFDARK.stOUTn_b;

	if (stAFDARK.vCNT&0x80) pFILTx = &stAFDARK.stOUTn_h;

	AFDARK_GetDriveRATE(DRV_SEARCH_NAR);

	AFDARK_GetPrvJudgeBUFF(&stAFDARK.stOUTn_h, stAFDARK.xCNT);
	AFDARK_GetPrvJudgeBUFF(&stAFDARK.stOUTn_b, stAFDARK.xCNT);

	if (AFDARK_IsDriveEnd()==FALSE) {
		if(stAFDARK.xCNT != 0xff) stAFDARK.xCNT++;	if (stAFDARK.xCNT<3) return;
		if((stAFDARK.vCNT&0xf) != 0xf) stAFDARK.vCNT++;
	}

	if (AFDARK_IsDriveEnd()==TRUE) {
			if(stAFDARK.eCNT != 0xff) stAFDARK.eCNT++;			AFDARK_ClrJudgeRATE();
		stAFDARK.cur = INV_SEARCH_NAR;	stAFDARK.nxt = PRE_SEARCH_DIR;
	}
	else if (pFILTx->nCHK&GOOD_DEC && (pFILTb->nCHK&ZERO_INC)==0) {
		stAFDARK.cur = INV_SEARCH_NAR;	stAFDARK.nxt = DRV_FORCE_PEAK;
	}
	else if (pFILTx->nCHK&ZERO_INC || pFILTb->nCHK&ZERO_INC) {
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;
	}
	else if ((stAFDARK.vCNT&0xf)==5) {
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;
	}
	else {	if(stAFDARK.xCNT) stAFDARK.xCNT--;			AFDARK_MovShiftBUFF(3);
		stAFDARK.cur = DRV_SEARCH_NAR;	stAFDARK.nxt = DRV_SEARCH_NAR;
	}

	if (stAFDARK.cur==INV_SEARCH_NAR) AFDARK_ClrJudgeBUFF();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_GetJudge_DIR(void)
{
	PAFD_FILT_PARAM pFILTh = &stAFDARK.stOUTn_h;
	PAFD_FILT_PARAM pFILTb = &stAFDARK.stOUTn_b;

	AFDARK_GetDriveRATE(DRV_SEARCH_DIR);

	AFDARK_GetDirJudgeBUFF(&stAFDARK.stOUTn_h, stAFDARK.xCNT);
	AFDARK_GetDirJudgeBUFF(&stAFDARK.stOUTn_b, stAFDARK.xCNT);

	if (AFDARK_IsDriveEnd()==FALSE) {
		if(stAFDARK.xCNT != 0xff) stAFDARK.xCNT++; if (stAFDARK.xCNT<4) return;
	}

	if (stAFDARK.xCNT) stAFDARK.xCNT--;	AFDARK_MovShiftBUFF(4);

	if (AFDARK_IsDriveEnd()==TRUE) {
		if(stAFDARK.eCNT != 0xff) stAFDARK.eCNT++;			AFDARK_ClrJudgeRATE();
		stAFDARK.cur = INV_SEARCH_DIR;	stAFDARK.nxt = DRV_SEARCH_DIR;
	}
	else if (pFILTb->nCHK&GOOD_DEC) {
			AFDARK_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFDARK.cur = INV_SEARCH_DIR;	stAFDARK.nxt = PRE_CLIMB_TOP2;
	}
	else if (pFILTb->nCHK&GOOD_INC) {
			AFDARK_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;
	}
	else if (pFILTh->nCHK&GOOD_DEC) {
			AFDARK_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFDARK.cur = INV_SEARCH_DIR;	stAFDARK.nxt = PRE_CLIMB_TOP2;
	}
	else if (pFILTh->nCHK&GOOD_INC) {
			AFDARK_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;
	}
	else {
			AFDARK_DirDrvIncSPD_BI(DRV_SEARCH_DIR);
		stAFDARK.cur = DRV_SEARCH_DIR;	stAFDARK.nxt = DRV_SEARCH_DIR;
	}

	if (pFILTh->nCHK&GOOD_DEC && pFILTb->nCHK&GOOD_DEC) stAFDARK.nSEL = USE_HPF;
	if (pFILTh->nCHK&GOOD_INC && pFILTb->nCHK&GOOD_INC) stAFDARK.nSEL = USE_HPF;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_InvJudge_DIR(void)
{
	PAFD_FILT_PARAM pFILTx = &stAFDARK.stOUTn_b;
	PAFD_FILT_PARAM pFILTb = &stAFDARK.stOUTn_b;

	if (stAFDARK.nSEL==USE_HPF) pFILTx = &stAFDARK.stOUTn_h;

	if (stAFDARK.nxt!=PRE_CLIMB_TOP2) AFDARK_ClrJudgeBUFF();
	if (stAFDARK.nxt!=PRE_CLIMB_TOP2) return;

	AFDARK_GetDriveRATE(DRV_SEARCH_DIR);

	AFDARK_InvDirJudgeBUFF(&stAFDARK.stOUTn_h, stAFDARK.xCNT);
	AFDARK_InvDirJudgeBUFF(&stAFDARK.stOUTn_b, stAFDARK.xCNT);

	if ((pFILTb->nCHK&ZERO_DEC)==0 || (pFILTx->nCHK&ZERO_DEC)==0) {
		stAFDARK.nxt = DRV_SEARCH_DIR;	AFDARK_SetChangeDIR();
		pFILTb->nMAX = (pFILTb->nMAX+pFILTb->FLTn)/2;
		pFILTx->nMAX = (pFILTx->nMAX+pFILTx->FLTn)/2;
	}

	if (stAFDARK.nxt==PRE_CLIMB_TOP2) AFDARK_ClrJudgePEAK();
	if (stAFDARK.nxt==PRE_CLIMB_TOP2) AFDARK_ClrJudgeBUFF();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_GetJudge_DEC(void)
{
	PAFD_FILT_PARAM pFILTx = &stAFDARK.stOUTn_b;
	PAFD_FILT_PARAM pFILTh = &stAFDARK.stOUTn_h;
	PAFD_FILT_PARAM pFILTb = &stAFDARK.stOUTn_b;

	if (stAFDARK.nSEL==USE_HPF) pFILTx = &stAFDARK.stOUTn_h;

	AFDARK_GetDriveRATE(DRV_CLIMB_TOP2);

	AFDARK_GetDecJudgeBUFF(&stAFDARK.stOUTn_h, stAFDARK.xCNT);
	AFDARK_GetDecJudgeBUFF(&stAFDARK.stOUTn_b, stAFDARK.xCNT);
	if (AFDARK_IsDriveEnd()==FALSE) {
		if(stAFDARK.xCNT != 0xff) stAFDARK.xCNT++; if (stAFDARK.xCNT<4) return;
	}

	if (stAFDARK.xCNT) stAFDARK.xCNT--;	AFDARK_MovShiftBUFF(4);

	if (AFDARK_IsDriveEnd()==TRUE) {
				stAFDARK.eCNT++;			AFDARK_ClrJudgeRATE();
		stAFDARK.cur = INV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;
	}
	else if (pFILTh->nCHK&GOOD_DEC && pFILTb->nCHK&GOOD_DEC) {
		stAFDARK.cur = INV_CLIMB_TOP2;	stAFDARK.nxt = PRE_CLIMB_PEAK;
	}
	else if (pFILTb->nCHK&G_INC_OK && pFILTb->nCHK&RATE_DEC) {
			AFDARK_DecDrvDecSPD_GD(DRV_CLIMB_TOP2);
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;
	}
	else if (pFILTb->nCHK&GOOD_INC) {
			AFDARK_DecDrvIncSPD_GX(DRV_CLIMB_TOP2);
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;
	}
	else {
			AFDARK_DecDrvIncSPD_BI(DRV_CLIMB_TOP2);
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;
	}

	if (pFILTh->nCHK&HIGH_INC && pFILTb->nCHK&HIGH_INC) stAFDARK.nSEL = USE_HPF;
	if (pFILTh->nCHK&HIGH_DEC && pFILTb->nCHK&HIGH_DEC) stAFDARK.nSEL = USE_HPF;

	if (pFILTb->nCHK&RATE_DEC && pFILTb->nCHK&ZERO_DEC) {
		stAFDARK.cur = INV_CLIMB_TOP2;	stAFDARK.nxt = PRE_CLIMB_PEAK;
	}

	if (pFILTb->nCHK&HIGH_DEC) {
		stAFDARK.cur = INV_CLIMB_TOP2;	stAFDARK.nxt = PRE_CLIMB_PEAK;
	}

	if (pFILTh->nCHK&Z_DEC_OK || pFILTh->nCHK&Z_INC_OK) stAFDARK.nSEL = USE_HPF;


	if (pFILTh->nCHK&G_INC_OK && pFILTh->nCHK&HIGH_DEC) {
		stAFDARK.cur = INV_CLIMB_TOP2;	stAFDARK.nxt = PRE_CLIMB_PEAK;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_InvJudge_DEC(void)
{
	PAFD_FILT_PARAM pFILTx = &stAFDARK.stOUTn_b;


	if (stAFDARK.nxt!=PRE_CLIMB_PEAK) AFDARK_ClrJudgeBUFF();
	if (stAFDARK.nxt!=PRE_CLIMB_PEAK) return;

	AFDARK_GetDriveRATE(DRV_CLIMB_TOP2);

	AFDARK_InvDecJudgeBUFF(&stAFDARK.stOUTn_h, stAFDARK.xCNT);
	AFDARK_InvDecJudgeBUFF(&stAFDARK.stOUTn_b, stAFDARK.xCNT);

	if ((pFILTx->nCHK&ZERO_DEC)==0) {
		stAFDARK.nxt = DRV_CLIMB_TOP2;	AFDARK_SetChangeDIR();
	}

	AFDARK_DecDrvStdSPD_XX(DRV_CLIMB_TOP2);
	if (stAFDARK.nxt==DRV_CLIMB_TOP2) return;

	AFDARK_GetDriveSTEP(DRV_STEP_DIV);
	AFDARK_ClrJudgeBUFF();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_GetJudge_TOP(void)
{
	PAFD_FILT_PARAM pFILTx = &stAFDARK.stOUTn_b;

	if (stAFDARK.nSEL==USE_HPF) pFILTx = &stAFDARK.stOUTn_h;

	AFDARK_GetDriveRATE(DRV_CLIMB_PEAK);

	AFDARK_GetTopJudgeBUFF(&stAFDARK.stOUTn_h, stAFDARK.xCNT);
	AFDARK_GetTopJudgeBUFF(&stAFDARK.stOUTn_b, stAFDARK.xCNT);

	if (AFDARK_IsDriveEnd()==TRUE) {
		stAFDARK.cur = INV_CLIMB_PEAK;	stAFDARK.nxt = DRV_FORCE_PEAK;
	}
	else if (pFILTx->nCHK&GOOD_DEC) {
		stAFDARK.cur = INV_CLIMB_PEAK;	stAFDARK.nxt = DRV_FORCE_PEAK;
	}
	else {
		stAFDARK.cur = DRV_CLIMB_PEAK;	stAFDARK.nxt = DRV_CLIMB_PEAK;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_InvJudge_TOP(void)
{
	PAFD_FILT_PARAM pFILTx = &stAFDARK.stOUTn_b;

	if (stAFDARK.nSEL==USE_HPF) pFILTx = &stAFDARK.stOUTn_h;

	if (AFDARK_IsDriveEnd()==TRUE) return;

	AFDARK_GetDriveRATE(DRV_CLIMB_PEAK);

	AFDARK_InvTopJudgeBUFF(&stAFDARK.stOUTn_h, stAFDARK.xCNT);
	AFDARK_InvTopJudgeBUFF(&stAFDARK.stOUTn_b, stAFDARK.xCNT);

	if (pFILTx->nCHK&ZERO_DEC) return;

	stAFDARK.nxt = DRV_CLIMB_PEAK;
	AFDARK_SetChangeDIR();
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFDARK_InvJudge_END(void)
{
	PAFD_FILT_PARAM pFILTb = &stAFDARK.stOUTn_b;
	PAFD_FILT_PARAM pFILTh = &stAFDARK.stOUTn_h;

	if (stAFDARK.eCNT==2) return FALSE;
	AFDARK_GetDriveRATE(DRV_CLIMB_PEAK);

	AFDARK_InvEndJudgeBUFF(&stAFDARK.stOUTn_h, USE_HPF);
	AFDARK_InvEndJudgeBUFF(&stAFDARK.stOUTn_b, USE_BPF);
	if ((pFILTb->nCHK&GOOD_END)==0) return TRUE;
	if ((pFILTh->nCHK&GOOD_END)==0) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_SetInitPARAM(void)
{
	stAFDARK.peak = stAFDARK.fVAL;

	stAFDARK.fMID = LENS_GetMIDPositionLOCI();
	stAFDARK.fGAP = LENS_GetGAPPositionLOCI();

	stAFDARK.fMIN = LENS_GetMINPositionLOCI(stAFDARK.zVAL);
	stAFDARK.fMAX = LENS_GetMAXPositionLOCI(stAFDARK.zVAL);

	stAFDARK.nDIR = LENS_GetDirectionFOKS();
	LENS_SetDirectionFOKS(stAFDARK.nDIR);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_PreFilterSEL(void)
{
	if (stAFDARK.cur!=PRE_FILTER_SEL) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFDARK_SetInitPARAM();	LENS_EnableMotorFOKS(ON);
	stAFDARK.eCNT = 0; stAFDARK.cur = PRE_SEARCH_MOV;
	stAFDARK.vCNT = 0; stAFDARK.nxt = PRE_SEARCH_MOV;

	memset(&stAFDARK.stSTEP_m, 0, sizeof(AFD_STEP_PARAM));
	stAFDARK.stSTEP_m.sMIN = LENS_GetAFDriveSTEP(DRV_STEP_MIN);
	stAFDARK.stSTEP_m.sSTD = LENS_GetAFDriveSTEP(DRV_STEP_STD);
	stAFDARK.stSTEP_m.sMID = LENS_GetAFDriveSTEP(DRV_STEP_MID);
	stAFDARK.stSTEP_m.sMAX = LENS_GetAFDriveSTEP(DRV_STEP_MAX);
	stAFDARK.stSTEP_m.sFWD = LENS_GetAFDriveSTEP(DRV_STEP_FWD);
	stAFDARK.stSTEP_m.sBWD = LENS_GetAFDriveSTEP(DRV_STEP_BWD);
	stAFDARK.stSTEP_m.sTOP = LENS_GetAFDriveSTEP(DRV_STEP_TOP);

	if (stAFDARK.nSEL==USE_HPF) stAFDARK.vCNT |= 0x80;
	stAFDARK.nSEL = USE_BPF;	AFDARK_ClrJudgeBUFF();

	memset(&stAFDARK.stOUTn_h.nMIN, 0, sizeof(AFD_RATE_PARAM));
	memset(&stAFDARK.stOUTn_b.nMIN, 0, sizeof(AFD_RATE_PARAM));

	stAFDARK.stNORM_y.nRST = 0;
	stAFDARK.stNORM_y.nERR = 0;
	stAFDARK.stNORM_y.nCNT = 0;

	stAFDTUNE.n_GAIN = 0;
	stAFDTUNE.n_YSUM = 0;
	stAFDTUNE.n_FLTr = 0;

}

//--------------------------------------------------------------------------------------------------------------------------
//static void AFDARK_EndFilterSEL(void)
//{
//	if (stAFDARK.cur!=END_FILTER_SEL) return;
//	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
//
//	stAFDARK.cur = PRE_SEARCH_MOV;
//	stAFDARK.nxt = PRE_SEARCH_MOV;
//}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_PreSearchMOV(void)
{
	if (stAFDARK.cur!=PRE_SEARCH_MOV) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFDARK.stOUTn_b.xHIGH = stAFDARK.stOUTn_b.FLTn; // back up data
	stAFDARK.stOUTn_h.xHIGH = stAFDARK.stOUTn_h.FLTn;

	stAFDARK.cur = DRV_SEARCH_MOV;
	//stAFDTUNE.n_GAIN = 0;
	//stAFDTUNE.n_YSUM = 0;
	//stAFDTUNE.n_FLTr = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DrvSearchMOV(void)
{
	if (stAFDARK.cur!=DRV_SEARCH_MOV) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFDARK.cur = PRE_SEARCH_FAR;
	stAFDARK.nxt = PRE_SEARCH_FAR;

	AFDARK_ClrJudgeRATE(); // clear
	AFDARK_ClrJudgePEAK();
	AFDARK_ClrJudgeINFO();
	AFDARK_GetJudge_FAR();

	AFDARK_GetDriveSTEP(DRV_STEP_MIN); // min step
	AFDARK_SetDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_PreSearchFAR(void)
{
	if (stAFDARK.cur!=PRE_SEARCH_FAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFDARK.cur = DRV_SEARCH_FAR;
	stAFDARK.nxt = DRV_SEARCH_FAR;

	AFDARK_GetJudge_FAR();

	AFDARK_SetDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DrvSearchFAR(void)
{
	if (stAFDARK.cur!=DRV_SEARCH_FAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFDARK_GetJudge_FAR();

	if (stAFDARK.cur==INV_SEARCH_FAR) return;
	AFDARK_SetDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_EndSearchFAR(void)
{
	if (stAFDARK.cur!=END_SEARCH_FAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFDARK.nSEL==USE_HPF)
		 stAFDARK.peak = stAFDARK.stOUTn_h.xPEAK;
	else stAFDARK.peak = stAFDARK.stOUTn_b.xPEAK;

	stAFDARK.cur = stAFDARK.nxt;
	stAFDARK.nxt = PRE_SEARCH_NAR;

	if (stAFDARK.cur==DRV_FORCE_PEAK) return;
	if (stAFDARK.fVAL==stAFDARK.peak) return;

	AFDARK_GetDriveSTEP(DRV_STEP_BWD);
	AFDARK_GetDriveSTEP(DRV_STEP_REM);
	AFDARK_SetDriveSTEP(DRV_STEP_REM);
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFDARK_MovSearchNAR(void)
{
	if (stAFDARK.cur!=PRE_SEARCH_NAR) return FALSE;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return FALSE;

	if (stAFDARK.fVAL==stAFDARK.peak) return TRUE;

	if (stAFDARK.nDIR== fFAR && stAFDARK.fVAL<stAFDARK.peak)
		AFDARK_GetDriveSTEP(DRV_STEP_REM);

	if (stAFDARK.nDIR==fNEAR && stAFDARK.fVAL>stAFDARK.peak)
		AFDARK_GetDriveSTEP(DRV_STEP_REM);

	AFDARK_SetDriveSTEP(DRV_STEP_REM);
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_PreSearchNAR(void)
{
	if (stAFDARK.cur!=PRE_SEARCH_NAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (AFDARK_MovSearchNAR()==FALSE) return;

	stAFDARK.cur = DRV_SEARCH_NAR;
	stAFDARK.nxt = DRV_SEARCH_NAR;

	AFDARK_ClrJudgeRATE();
	AFDARK_ClrJudgePEAK();
	AFDARK_ClrJudgeINFO();
	AFDARK_GetJudge_NAR();

	AFDARK_GetDriveSTEP(DRV_STEP_MIN);
	AFDARK_SetDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DrvSearchNAR(void)
{
	if (stAFDARK.cur!=DRV_SEARCH_NAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFDARK_GetJudge_NAR();

	if (stAFDARK.cur==INV_SEARCH_NAR) return;
	AFDARK_SetDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_EndSearchNAR(void)
{
	if (stAFDARK.cur!=END_SEARCH_NAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFDARK.vCNT&0x80) stAFDARK.nSEL = USE_HPF;

	if (stAFDARK.nSEL==USE_HPF)
		 stAFDARK.peak = stAFDARK.stOUTn_h.xPEAK;
	else stAFDARK.peak = stAFDARK.stOUTn_b.xPEAK;

	stAFDARK.cur = stAFDARK.nxt;

	if (stAFDARK.cur==DRV_FORCE_PEAK) return;
	AFDARK_SetDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_PreSearchDIR(void)
{
	if (stAFDARK.cur!=PRE_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFDARK.cur = DRV_SEARCH_DIR;
	stAFDARK.nxt = DRV_SEARCH_DIR;

	AFDARK_GetJudge_DIR();

	AFDARK_GetDriveSTEP(DRV_STEP_STD);
	AFDARK_SetDriveSTEP(DRV_STEP_STD);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DrvSearchDIR(void)
{
	if (stAFDARK.cur!=DRV_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFDARK_GetJudge_DIR();

	if (stAFDARK.cur==INV_SEARCH_DIR) return;
	AFDARK_SetDriveSTEP(DRV_STEP_STD);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_EndSearchDIR(void)
{
	if (stAFDARK.cur!=END_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFDARK.nSEL==USE_HPF)
		 stAFDARK.peak = stAFDARK.stOUTn_h.nPEAK;
	else stAFDARK.peak = stAFDARK.stOUTn_b.nPEAK;

	AFDARK_InvJudge_DIR();

	if (stAFDARK.eCNT==2)
		 stAFDARK.cur = DRV_FORCE_PEAK;
	else stAFDARK.cur = stAFDARK.nxt;

	if (stAFDARK.cur==DRV_FORCE_PEAK) return;
	AFDARK_SetDriveSTEP(DRV_STEP_PRV);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_PreClimbTOP2(void)
{
	if (stAFDARK.cur!=PRE_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFDARK.cur = DRV_CLIMB_TOP2;
	stAFDARK.nxt = DRV_CLIMB_TOP2;

	AFDARK_ClrJudgeRATE();
	AFDARK_ClrJudgeINFO();
	AFDARK_GetJudge_DEC();

	AFDARK_SetDriveSTEP(DRV_STEP_PRV);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DrvClimbTOP2(void)
{
	if (stAFDARK.cur!=DRV_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFDARK_GetJudge_DEC();

	if (stAFDARK.cur==INV_CLIMB_TOP2) return;
	AFDARK_SetDriveSTEP(DRV_STEP_PRV);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_EndClimbTOP2(void)
{
	if (stAFDARK.cur!=END_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFDARK.nSEL==USE_HPF)
		 stAFDARK.peak = stAFDARK.stOUTn_h.nPEAK;
	else stAFDARK.peak = stAFDARK.stOUTn_b.nPEAK;

	AFDARK_InvJudge_DEC();

	if (stAFDARK.eCNT==2)
		 stAFDARK.cur = DRV_FORCE_PEAK;
	else stAFDARK.cur = stAFDARK.nxt;

	if (stAFDARK.cur==DRV_FORCE_PEAK) return;
	if (stAFDARK.cur==PRE_CLIMB_PEAK &&
		stAFDARK.fVAL==stAFDARK.peak) return;

	AFDARK_GetDriveSTEP(DRV_STEP_PRV);
	AFDARK_SetDriveSTEP(DRV_STEP_PRV);
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFDARK_MovClimbPEAK(void)
{
	if (stAFDARK.cur!=PRE_CLIMB_PEAK) return FALSE;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return FALSE;

	if (stAFDARK.fVAL==stAFDARK.peak) return TRUE;

	if (stAFDARK.nDIR== fFAR && stAFDARK.fVAL>stAFDARK.peak) {
		stAFDARK.stNORM_y.nERR = TRUE;	return TRUE;
	}
	if (stAFDARK.nDIR==fNEAR && stAFDARK.fVAL<stAFDARK.peak) {
		stAFDARK.stNORM_y.nERR = TRUE;	return TRUE;
	}

	if (stAFDARK.nDIR== fFAR && stAFDARK.fVAL<stAFDARK.peak)
		AFDARK_GetDriveSTEP(DRV_STEP_REM);

	if (stAFDARK.nDIR==fNEAR && stAFDARK.fVAL>stAFDARK.peak)
		AFDARK_GetDriveSTEP(DRV_STEP_REM);

	AFDARK_SetDriveSTEP(DRV_STEP_REM);
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_PreClimbPEAK(void)
{
	if (stAFDARK.cur!=PRE_CLIMB_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (AFDARK_MovClimbPEAK()==FALSE) return;

	stAFDARK.cur = DRV_CLIMB_PEAK;
	stAFDARK.nxt = DRV_CLIMB_PEAK;

	AFDARK_ClrJudgeRATE();
	AFDARK_ClrJudgePEAK();
	AFDARK_ClrJudgeINFO();
	AFDARK_GetJudge_TOP();

	if (stAFDARK.stNORM_y.nERR) return;

	AFDARK_GetDriveSTEP(DRV_STEP_TOP);
	AFDARK_SetDriveSTEP(DRV_STEP_TOP);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DrvClimbPEAK(void)
{
	if (stAFDARK.cur!=DRV_CLIMB_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFDARK_GetJudge_TOP();

	if (stAFDARK.cur==INV_CLIMB_PEAK) return;
	AFDARK_SetDriveSTEP(DRV_STEP_TOP);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_EndClimbPEAK(void)
{
	if (stAFDARK.cur!=END_CLIMB_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFDARK.nSEL==USE_HPF)
		 stAFDARK.peak = stAFDARK.stOUTn_h.nPEAK;
	else stAFDARK.peak = stAFDARK.stOUTn_b.nPEAK;

	if (stAFDARK.nDIR==fFAR)
		 stAFDARK.peak += 2*stAFDARK.stSTEP_m.sTOP;
	else stAFDARK.peak -= 0*stAFDARK.stSTEP_m.sTOP;

	AFDARK_InvJudge_TOP();

	stAFDARK.cur = stAFDARK.nxt;

	if (stAFDARK.cur==DRV_FORCE_PEAK) return;
	AFDARK_SetDriveSTEP(DRV_STEP_TOP);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DrvForcePEAK(void)
{
	if (stAFDARK.cur!=DRV_FORCE_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFDARK.nDIR==fFAR) stAFDARK.peak += stAFDTUNE.h_FOKS;
	else					 stAFDARK.peak -= stAFDTUNE.h_FOKS;

	stAFDARK.cur = END_FORCE_PEAK;
	AFDARK_SetDrivePEAK();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_EndForcePEAK(void)
{
	if (stAFDARK.cur!=END_FORCE_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFDARK.cur = END_CLEAR_WAIT;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_EndDriveWAIT(void)
{
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	switch (stAFDARK.cur) {
		case INV_FILTER_SEL: stAFDARK.cur = PRE_SEARCH_MOV; break;
		case INV_SEARCH_FAR: stAFDARK.cur = END_SEARCH_FAR; break;
		case INV_SEARCH_NAR: stAFDARK.cur = END_SEARCH_NAR; break;
		case INV_SEARCH_DIR: stAFDARK.cur = END_SEARCH_DIR; break;
		case INV_CLIMB_TOP1: stAFDARK.cur = END_CLIMB_TOP1; break;
		case INV_CLIMB_TOP2: stAFDARK.cur = END_CLIMB_TOP2; break;
		case INV_CLIMB_PEAK: stAFDARK.cur = END_CLIMB_PEAK; break;
		case INV_SWING_SCAN: stAFDARK.cur = END_SWING_SCAN; break;
		default:			 return;
	}
	AFDARK_SetChangeDIR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_EndClearLUMA(PAF_CTRL_PARAM pCTRL)
{
	WORD nID = MAKEWORD(stAFDARK.stNORM_y.nERR,stAFDARK.fMOV);

	if (stAFDARK.cur<=END_SEARCH_MOV) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFDARK.stNORM_y.nRST |= AF_GetLUMAInfo(pCTRL,nID);
	stAFDARK.stNORM_y.nERR  = FALSE;

	if (stAFDARK.stNORM_y.nRST) stAFDARK.cur = END_FORCE_PEAK;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_EndClearAUTO(PAF_CTRL_PARAM pCTRL)
{
	if (stAFDARK.cur!=END_CLEAR_AUTO) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFDARK.cur = END_CLEAR_PROC;

	if (stAFDARK.stNORM_y.nRST==TRUE) return;
	AF_SetAUTOInfo(pCTRL, stAFDARK.stNORM_y.nCHK); // set to stAFAUTO.*

	if (stAFDARK.ZOOM<=stAFDARK.KNEE) return;
	LENS_SetTrackingINFO(stAFDARK.peak); //?
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_EndClearWAIT(PAF_CTRL_PARAM pCTRL)
{
	BOOL nID;

	if (stAFDARK.cur!=END_CLEAR_WAIT) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	nID = (AFDARK_InvJudge_END())? TRUE : FALSE;
	stAFDARK.stNORM_y.nRST |= AF_GetDATAInfo(pCTRL,nID);

	stAFDARK.cur = END_CLEAR_AUTO;
	LENS_EnableMotorFOKS(OFF);
}

//--------------------------------------------------------------------------------------------------------------------------
void AFDARK_ProcessHandler(PAF_CTRL_PARAM pCTRL)
{
	if (stAFDARK.cur==0) return;
	stAFDARK.prv = stAFDARK.cur;

	AFDARK_GetAFOPDWIND(pCTRL);
	AFDARK_EndClearLUMA(pCTRL);
	AFDARK_EndClearAUTO(pCTRL);
	AFDARK_EndClearWAIT(pCTRL);


	AFDARK_PreFilterSEL();
	AFDARK_PreSearchMOV();
	AFDARK_PreSearchFAR();
	AFDARK_PreSearchNAR();
	AFDARK_PreSearchDIR();
	AFDARK_PreClimbTOP2();
	AFDARK_PreClimbPEAK();

	AFDARK_DrvSearchMOV();
	AFDARK_DrvSearchFAR();
	AFDARK_DrvSearchNAR();
	AFDARK_DrvSearchDIR();
	AFDARK_DrvClimbTOP2();
	AFDARK_DrvClimbPEAK();
	AFDARK_DrvForcePEAK();

	//AFDARK_EndFilterSEL();
	AFDARK_EndSearchFAR();
	AFDARK_EndSearchNAR();
	AFDARK_EndSearchDIR();
	AFDARK_EndClimbTOP2();
	AFDARK_EndClimbPEAK();

	AFDARK_EndForcePEAK();
	AFDARK_EndDriveWAIT();
}

//--------------------------------------------------------------------------------------------------------------------------
void AFDARK_EnableProcess(BOOL OnOff)
{
	if (OnOff)	stAFDARK.cur = PRE_FILTER_SEL;
	else		stAFDARK.cur = END_CLEAR_PROC;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AFDARK_IsProcessExecuting(void)
{
	return (stAFDARK.cur==0)? FALSE : TRUE;
}

#endif	/* defined(__USE_AFZOOM_LENS__) && defined(__USE_X30X_TAMRON__) */

/*  FILE_END_HERE */
