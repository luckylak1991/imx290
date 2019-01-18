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

}	AFS_BUFF_PARAM, *PAFS_BUFF_PARAM;

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

}	AFS_RATE_PARAM, *PAFS_RATE_PARAM;

typedef	struct
{
	LONG	nHIGH;
	SHORT	nPEAK;
	SHORT	nDUMY;

}	AFS_PEAK_PARAM, *PAFS_PEAK_PARAM;

typedef	struct
{
	LONG	YSUM;
	LONG	YPRV;
	LONG	YREF;
	LONG	GAIN;
	LONG	YCNT;
	LONG	FLTd;

	BYTE	nCHK;
	BYTE	nRST;
	BYTE	nERR;
	BYTE	nCNT;

}	AFS_NORM_PARAM, *PAFS_NORM_PARAM;

typedef	struct
{
	LONG	FLTa;
	LONG	FLTb;
	LONG	FLTo;
	LONG	FLTn;

}	AFS_DATA_PARAM, *PAFS_DATA_PARAM;

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

}	AFS_FILT_PARAM, *PAFS_FILT_PARAM;

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

}	AFS_STEP_COUNT, *PAFS_STEP_COUNT;

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

	AFS_STEP_COUNT	stDIR_I;
	AFS_STEP_COUNT	stDEC_I;
	AFS_STEP_COUNT	stDEC_D;

}	AFS_STEP_PARAM, *PAFS_STEP_PARAM;

typedef	struct
{
	BYTE	nSEL;
	BYTE	nDIR;
	BYTE	eCNT;
	BYTE	xCNT;
	BYTE	vCNT;

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
	SHORT	fABV;
	SHORT	fBLO;
	SHORT	fGAP;
	SHORT	fMOV;
	SHORT	zVAL;
	SHORT	ZOOM;
	SHORT	KNEE;
	WORD	time;
	BYTE	nMOD;
	CHAR	nCHK;

	AFS_NORM_PARAM	stNORM_y;
	AFS_DATA_PARAM	stDATA_r;
	AFS_DATA_PARAM	stDATA_l;
	AFS_DATA_PARAM	stDATA_v;
	AFS_DATA_PARAM	stDATA_b;
	AFS_DATA_PARAM	stDATA_h;
	AFS_FILT_PARAM	stOUTn_b;
	AFS_FILT_PARAM	stOUTn_h;
	AFS_STEP_PARAM	stSTEP_m;

}	AFS_PROC_PARAM, *PAFS_PROC_PARAM;

typedef enum {
	PRE_FILTER_SEL = 0x10, DRV_FILTER_SEL, INV_FILTER_SEL, END_FILTER_SEL,
	PRE_SEARCH_MOV = 0x20, DRV_SEARCH_MOV, INV_SEARCH_MOV, END_SEARCH_MOV,
	PRE_SEARCH_FIT = 0x30, DRV_SEARCH_FIT, INV_SEARCH_FIT, END_SEARCH_FIT,
	PRE_SEARCH_FAR = 0x40, DRV_SEARCH_FAR, INV_SEARCH_FAR, END_SEARCH_FAR,
	PRE_SEARCH_NAR = 0x50, DRV_SEARCH_NAR, INV_SEARCH_NAR, END_SEARCH_NAR,
	PRE_SEARCH_DIR = 0x60, DRV_SEARCH_DIR, INV_SEARCH_DIR, END_SEARCH_DIR,
	PRE_CLIMB_TOP1 = 0x70, DRV_CLIMB_TOP1, INV_CLIMB_TOP1, END_CLIMB_TOP1,
	PRE_CLIMB_TOP2 = 0x80, DRV_CLIMB_TOP2, INV_CLIMB_TOP2, END_CLIMB_TOP2,
	PRE_CLIMB_PEAK = 0x90, DRV_CLIMB_PEAK, INV_CLIMB_PEAK, END_CLIMB_PEAK,
	PRE_FORCE_PEAK = 0xA0, DRV_FORCE_PEAK, INV_FORCE_PEAK, END_FORCE_PEAK,
	END_CLEAR_PROC = 0x00, END_CLEAR_AUTO, END_CLEAR_WAIT

} 	AFS_PROCESS_STEP;

typedef enum {
	USE_HPF = 0, USE_LPF, USE_BPF

} 	AFS_PROCESS_FILT;

typedef enum {
	GOOD_DEC = 0x000008, G_DEC_OK = 0x000004, G_INC_OK = 0x000002, GOOD_INC = 0x000001,
	ZERO_DEC = 0x000080, ZERO_INC = 0x000010,
	RATE_DEC = 0x000800, R_DEC_OK = 0x000400, R_INC_OK = 0x000200, RATE_INC = 0x000100,										 
	HIGH_DEC = 0x008000, HIGH_INC = 0x001000,
	GAIN_MAX = 0x040000, GAIN_MID = 0x020000, GAIN_FLT = 0x010000,
	GOOD_END = 0x800000

} 	AFS_STATUS_RATE;

typedef	struct
{
	SHORT	n_UP;
	SHORT	n_DN;
	SHORT	z_UP;
	SHORT	z_DN;

}	AFS_TUNE_RATIO, *PAFS_TUNE_RATIO;

typedef	struct
{
	BYTE	r_MIN;
	BYTE	r_STD;
	BYTE	r_MID;
	BYTE	r_MAX;

}	AFS_TUNE_COUNT, *PAFS_TUNE_COUNT;

typedef	struct
{
	LONG	n_GAIN;	
	LONG	n_YSUM;	
	LONG	n_FLTr;	

	LONG	c_FLTr;	
	LONG	c_FLTx;	
	LONG	c_GMIN;	
	LONG	c_GMAX;	

	SHORT	r_LOCI;	
	SHORT	r_DUMY;	

	SHORT	h_ZOOM;	
	SHORT	h_FOKS;	

	AFS_TUNE_RATIO	stPRV_b;
	AFS_TUNE_RATIO	stPRV_h;

	AFS_TUNE_RATIO	stDIR_b;
	AFS_TUNE_RATIO	stDIR_h;

	AFS_TUNE_RATIO	stDEC_b;
	AFS_TUNE_RATIO	stDEC_h;

	AFS_TUNE_RATIO	stTOP_b;
	AFS_TUNE_RATIO	stTOP_h;

}	AFS_TUNE_PARAM, *PAFS_TUNE_PARAM;

#define		MIN3(a,b,c)				(MIN(MIN(a,b),c))
#define		MAX3(a,b,c)				(MAX(MAX(a,b),c))

#define		MIN4(a,b,c,d)			(MIN(MIN(a,b),MIN(c,d)))
#define		MAX4(a,b,c,d)			(MAX(MAX(a,b),MAX(c,d)))

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static AFS_PROC_PARAM stAFSPOT;
static AFS_TUNE_PARAM stAFSTUNE = {
	//n_GAIN	n_YSUM	n_FLTr	c_FLTr	c_FLTx	c_GMIN	c_GMAX
	     0,      0,      0,   4000,      0,   8191,  65535,
	//r_LOCI	r_DUMY	h_ZOOM	h_FOKS 
	     0,      0,      0,      0,
	{  50,    -50,      1,     -1}, // stPRV_b
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
static void AFSPOT_GetAFOPDWIND(PAF_CTRL_PARAM pCTRL)
{
	FLOAT nGAIN, nYSUM, nFLTr;

	stAFSPOT.stDATA_r.FLTa = (pCTRL->stFILT.RPFa)>>0;
	stAFSPOT.stDATA_v.FLTa = (pCTRL->stFILT.VPFa)>>0;
	stAFSPOT.stDATA_b.FLTa = (pCTRL->stFILT.BPFa)>>0;
	stAFSPOT.stDATA_r.FLTb = (pCTRL->stFILT.RPFb)<<2;
	stAFSPOT.stDATA_v.FLTb = (pCTRL->stFILT.VPFb)<<2;
	stAFSPOT.stDATA_b.FLTb = (pCTRL->stFILT.BPFb)<<2;

	stAFSPOT.stNORM_y.YCNT = (pCTRL->stFILT.CNTa)>>0;
	stAFSPOT.stDATA_h.FLTa = (pCTRL->stFILT.HPFa)>>0;
	stAFSPOT.stDATA_h.FLTb = (pCTRL->stFILT.HPFb)<<2;

	stAFSPOT.stDATA_r.FLTo = stAFSPOT.stDATA_r.FLTa;
	stAFSPOT.stDATA_b.FLTo = stAFSPOT.stDATA_h.FLTa+stAFSPOT.stDATA_v.FLTa;
	stAFSPOT.stDATA_h.FLTo = stAFSPOT.stDATA_h.FLTa+stAFSPOT.stDATA_v.FLTa;

	stAFSPOT.stNORM_y.YPRV = (stAFSPOT.stNORM_y.YSUM);
	stAFSPOT.stNORM_y.YSUM = (pCTRL->stFILT.YSUM)>>0;
	stAFSPOT.stNORM_y.GAIN = (pCTRL->stFILT.GAIN)>>0;
	stAFSPOT.stNORM_y.YREF = (AECTRL_GetBrightness());

	if (stAFSTUNE.n_GAIN==0) stAFSTUNE.n_GAIN = stAFSPOT.stNORM_y.GAIN;
		nGAIN = ((FLOAT)stAFSPOT.stNORM_y.GAIN)/stAFSTUNE.n_GAIN;

	if (stAFSTUNE.n_YSUM==0) stAFSTUNE.n_YSUM = stAFSPOT.stNORM_y.YSUM;
		nYSUM = ((FLOAT)stAFSTUNE.n_YSUM)/stAFSPOT.stNORM_y.YSUM;
		nYSUM = ((FLOAT)nGAIN*nYSUM*nYSUM);

	stAFSPOT.stDATA_b.FLTo += stAFSPOT.stDATA_h.FLTb+stAFSPOT.stDATA_v.FLTb;

	stAFSPOT.stDATA_b.FLTn = (LONG)(1.00f*stAFSPOT.stDATA_b.FLTo+0.5);
	stAFSPOT.stDATA_h.FLTn = (LONG)(nYSUM*stAFSPOT.stDATA_h.FLTo+0.5);

	stAFSPOT.stOUTn_b.FLTn = (LONG)(1.00f*stAFSPOT.stDATA_b.FLTn+0.5);
	stAFSPOT.stOUTn_h.FLTn = (LONG)(1.00f*stAFSPOT.stDATA_h.FLTn+0.5);

	nFLTr = (FLOAT)sqrt(stAFSPOT.stNORM_y.GAIN/256.0f);
	stAFSPOT.stNORM_y.FLTd = (LONG)(nFLTr*stAFSTUNE.c_FLTr+0.5);

	stAFSPOT.nDIR = LENS_GetDirectionFOKS();
	stAFSPOT.fVAL = LENS_GetCurPositionFOKS();
	stAFSPOT.zVAL = LENS_GetCurPositionZOOM();
	stAFSPOT.ZOOM = LENS_GetRelPositionZOOM();
	stAFSPOT.KNEE = LENS_GetKneePositionZOOM();
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFSPOT_IsDriveEnd(void)
{
	if (stAFSPOT.nDIR==fNEAR && stAFSPOT.fVAL<=stAFSPOT.fMIN) return TRUE;
	if (stAFSPOT.nDIR== fFAR && stAFSPOT.fVAL>=stAFSPOT.fMAX) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFSPOT_SetChangeDIR(void)
{
	stAFSPOT.nDIR = (stAFSPOT.nDIR==fNEAR)? fFAR : fNEAR;
	LENS_SetDirectionFOKS(stAFSPOT.nDIR);
	return stAFSPOT.nDIR;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetDriveSTEP(BYTE nID)
{
	WORD gap = (WORD)ABS(stAFSPOT.peak-stAFSPOT.fVAL);
	WORD quo = (WORD)ABS(stAFSPOT.peak-stAFSPOT.fVAL)/stAFSPOT.step;
	WORD rem = (WORD)ABS(stAFSPOT.peak-stAFSPOT.fVAL)%stAFSPOT.step;
	WORD max = (WORD)stAFSPOT.stSTEP_m.sBWD;
	WORD min = (WORD)stAFSPOT.stSTEP_m.sMIN;

	switch (nID) {
		case DRV_STEP_MIN:	stAFSPOT.step = (WORD)stAFSPOT.stSTEP_m.sMIN; break;
		case DRV_STEP_STD:	stAFSPOT.step = (WORD)stAFSPOT.stSTEP_m.sSTD; break;
		case DRV_STEP_MID:	stAFSPOT.step = (WORD)stAFSPOT.stSTEP_m.sMID; break;
		case DRV_STEP_MAX:	stAFSPOT.step = (WORD)stAFSPOT.stSTEP_m.sMAX; break;

		case DRV_STEP_FWD:	stAFSPOT.step = (WORD)stAFSPOT.stSTEP_m.sFWD; break;
		case DRV_STEP_BWD:	stAFSPOT.step = (WORD)stAFSPOT.stSTEP_m.sBWD; break;
		case DRV_STEP_TOP:	stAFSPOT.step = (WORD)stAFSPOT.stSTEP_m.sTOP; break;

		case DRV_STEP_DIV:	stAFSPOT.step = MIN(MAX((gap+3)/4,min), max); break;
		case DRV_STEP_REM:	stAFSPOT.step = (quo)? stAFSPOT.step : rem; break;
	}

	if ((nID&0xf0)==DRV_STEP_MIN) stAFSPOT.stSTEP_m.sSPD = nID;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetDriveRATE(BYTE nID)
{
	PBYTE pTUNEb = NULL, pTUNEh = NULL;

	switch (nID) {
		case DRV_SEARCH_FIT: case PRE_SEARCH_FIT:
		case DRV_SEARCH_FAR: case PRE_SEARCH_FAR:
		case DRV_SEARCH_NAR: case PRE_SEARCH_NAR:
			pTUNEb = (PBYTE)&stAFSTUNE.stPRV_b;	pTUNEh = (PBYTE)&stAFSTUNE.stPRV_h;	break;

		case DRV_SEARCH_DIR: case PRE_SEARCH_DIR:
			pTUNEb = (PBYTE)&stAFSTUNE.stDIR_b;	pTUNEh = (PBYTE)&stAFSTUNE.stDIR_h;	break;

		case DRV_CLIMB_TOP2: case PRE_CLIMB_TOP2:
			pTUNEb = (PBYTE)&stAFSTUNE.stDEC_b;	pTUNEh = (PBYTE)&stAFSTUNE.stDEC_h;	break;

		case DRV_CLIMB_PEAK: case PRE_CLIMB_PEAK:
			pTUNEb = (PBYTE)&stAFSTUNE.stTOP_b;	pTUNEh = (PBYTE)&stAFSTUNE.stTOP_h;	break;
	}

	memcpy((PBYTE)&stAFSPOT.stOUTn_b.n_UP, (PBYTE)pTUNEb, sizeof(AFS_TUNE_RATIO));
	memcpy((PBYTE)&stAFSPOT.stOUTn_h.n_UP, (PBYTE)pTUNEh, sizeof(AFS_TUNE_RATIO));
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_SetDriveSTEP(BYTE nID)
{
	stAFSPOT.goal = stAFSPOT.fVAL;

	if (stAFSPOT.nDIR==fFAR) stAFSPOT.goal += stAFSPOT.step;
	else					 stAFSPOT.goal -= stAFSPOT.step;

	stAFSPOT.goal = MIN(stAFSPOT.goal, stAFSPOT.fMAX);
	stAFSPOT.goal = MAX(stAFSPOT.goal, stAFSPOT.fMIN);

	LENS_SetDriveFOKS(stAFSPOT.goal);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_SetDrivePEAK(void)
{
	stAFSPOT.goal = stAFSPOT.peak;

	stAFSPOT.goal = MIN(stAFSPOT.goal, stAFSPOT.fMAX);
	stAFSPOT.goal = MAX(stAFSPOT.goal, stAFSPOT.fMIN);

	LENS_SetDriveFOKS(stAFSPOT.goal);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_ClrDrvCountSPD(PAFS_STEP_COUNT pCNT)
{
	SHORT sFWD = LENS_GetAFDriveSTEP(DRV_STEP_FWD);
	LONG  GAIN = stAFSPOT.stNORM_y.GAIN, KNEE = MIN(GAIN,8192);

	if (GAIN>1023) sFWD -= (SHORT)(6.0f*(FLOAT)KNEE/ 1024+0.5);
	if (GAIN>8191) sFWD -= (SHORT)(6.0f*(FLOAT)GAIN/16384+0.5);
	stAFSPOT.stSTEP_m.sFWD = MAX(sFWD,stAFSPOT.stSTEP_m.sSTD*3);

	switch (stAFSPOT.stSTEP_m.sSPD) {
		case DRV_STEP_MIN:	pCNT->xSTD = pCNT->xMID = pCNT->xMAX = pCNT->xFWD = 0; break;
		case DRV_STEP_STD:	pCNT->xMIN = pCNT->xMID = pCNT->xMAX = pCNT->xFWD = 0; break;
		case DRV_STEP_MID:	pCNT->xMIN = pCNT->xSTD = pCNT->xMAX = pCNT->xFWD = 0; break;
		case DRV_STEP_MAX:	pCNT->xMIN = pCNT->xSTD = pCNT->xMID = pCNT->xFWD = 0; break;
		case DRV_STEP_FWD:	pCNT->xMIN = pCNT->xSTD = pCNT->xMID = pCNT->xMAX = 0; break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DirDrvIncSPD_BI(BYTE nID)
{
	PAFS_STEP_COUNT pCNT = &stAFSPOT.stSTEP_m.stDIR_I;

	AFSPOT_ClrDrvCountSPD(pCNT);

	switch (stAFSPOT.stSTEP_m.sSPD) {
		case DRV_STEP_MIN:		AFSPOT_GetDriveSTEP(DRV_STEP_STD);	break;

		case DRV_STEP_STD:		if (pCNT->xSTD<6) pCNT->xSTD++;
			if (pCNT->xSTD==6)	AFSPOT_GetDriveSTEP(DRV_STEP_MID);
			else				AFSPOT_GetDriveSTEP(DRV_STEP_STD);	break;

		case DRV_STEP_MID:		if (pCNT->xMID<3) pCNT->xMID++;
			if (pCNT->xMID==3)	AFSPOT_GetDriveSTEP(DRV_STEP_MAX);
			else				AFSPOT_GetDriveSTEP(DRV_STEP_MID);	break;

		case DRV_STEP_MAX:		if (pCNT->xMAX<2) pCNT->xMAX++;
			if (pCNT->xMAX==2)	AFSPOT_GetDriveSTEP(DRV_STEP_FWD);
			else				AFSPOT_GetDriveSTEP(DRV_STEP_MAX);	break;

		case DRV_STEP_FWD:		AFSPOT_GetDriveSTEP(DRV_STEP_FWD);	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DecDrvIncSPD_GX(BYTE nID)
{
	PAFS_STEP_COUNT pCNT = &stAFSPOT.stSTEP_m.stDEC_I;

	AFSPOT_ClrDrvCountSPD(pCNT);
	memset(&stAFSPOT.stSTEP_m.stDEC_D, 0, sizeof(AFS_STEP_COUNT));

	if (stAFSPOT.stSTEP_m.sSPD==DRV_STEP_MAX) stAFSPOT.stSTEP_m.sCHK = 1;
	if (stAFSPOT.stSTEP_m.sSPD==DRV_STEP_FWD) stAFSPOT.stSTEP_m.sCHK = 1;

	if (stAFSPOT.stSTEP_m.sCHK) {
		switch (stAFSPOT.stSTEP_m.sSPD) {
			case DRV_STEP_MIN:		AFSPOT_GetDriveSTEP(DRV_STEP_MIN);	break;

			case DRV_STEP_STD:		AFSPOT_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		AFSPOT_GetDriveSTEP(DRV_STEP_MID);	break;

			case DRV_STEP_MAX:		if (pCNT->xMAX<4) pCNT->xMAX++;
				if (pCNT->xMAX==4)	AFSPOT_GetDriveSTEP(DRV_STEP_MID);
				else				AFSPOT_GetDriveSTEP(DRV_STEP_MAX);	break;

			case DRV_STEP_FWD:		AFSPOT_GetDriveSTEP(DRV_STEP_MAX);	break;
		}
	}
	else {
		switch (stAFSPOT.stSTEP_m.sSPD) {
			case DRV_STEP_MIN:		AFSPOT_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_STD:		if (pCNT->xSTD<4) pCNT->xSTD++;
				if (pCNT->xSTD==4)	AFSPOT_GetDriveSTEP(DRV_STEP_MID);
				else				AFSPOT_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		AFSPOT_GetDriveSTEP(DRV_STEP_MID);	break;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DecDrvIncSPD_BI(BYTE nID)
{
	PAFS_STEP_COUNT pCNT = &stAFSPOT.stSTEP_m.stDEC_I;

	AFSPOT_ClrDrvCountSPD(pCNT);
	memset(&stAFSPOT.stSTEP_m.stDEC_D, 0, sizeof(AFS_STEP_COUNT));

	switch (stAFSPOT.stSTEP_m.sSPD) {
		case DRV_STEP_MIN:		if (pCNT->xMIN<4) pCNT->xMIN++;
			if (pCNT->xMIN==4)	AFSPOT_GetDriveSTEP(DRV_STEP_STD);
			else				AFSPOT_GetDriveSTEP(DRV_STEP_MIN);	break;

		case DRV_STEP_STD:		if (pCNT->xSTD<3) pCNT->xSTD++;
			if (pCNT->xSTD==3)	AFSPOT_GetDriveSTEP(DRV_STEP_MID);
			else				AFSPOT_GetDriveSTEP(DRV_STEP_STD);	break;

		case DRV_STEP_MID:		if (pCNT->xMID<2) pCNT->xMID++;
			if (pCNT->xMID==2)	AFSPOT_GetDriveSTEP(DRV_STEP_MAX);
			else				AFSPOT_GetDriveSTEP(DRV_STEP_MID);	break;

		case DRV_STEP_MAX:		AFSPOT_GetDriveSTEP(DRV_STEP_FWD);	break;

		case DRV_STEP_FWD:		AFSPOT_GetDriveSTEP(DRV_STEP_FWD);	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DecDrvDecSPD_GD(BYTE nID)
{
	PAFS_STEP_COUNT pCNT = &stAFSPOT.stSTEP_m.stDEC_D;

	AFSPOT_ClrDrvCountSPD(pCNT);
	memset(&stAFSPOT.stSTEP_m.stDEC_I, 0, sizeof(AFS_STEP_COUNT));

	if (stAFSPOT.stSTEP_m.sSPD==DRV_STEP_MAX) stAFSPOT.stSTEP_m.sCHK = 1;
	if (stAFSPOT.stSTEP_m.sSPD==DRV_STEP_FWD) stAFSPOT.stSTEP_m.sCHK = 1;

	if (stAFSPOT.stSTEP_m.sCHK) {
		switch (stAFSPOT.stSTEP_m.sSPD) {
			case DRV_STEP_MIN:		AFSPOT_GetDriveSTEP(DRV_STEP_MIN);	break;

			case DRV_STEP_STD:		if (pCNT->xSTD<4) pCNT->xSTD++;
				if (pCNT->xSTD==4)	AFSPOT_GetDriveSTEP(DRV_STEP_MIN);
				else				AFSPOT_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		AFSPOT_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MAX:		AFSPOT_GetDriveSTEP(DRV_STEP_MID);	break;

			case DRV_STEP_FWD:		AFSPOT_GetDriveSTEP(DRV_STEP_MAX);	break;
		}
	}
	else {
		switch (stAFSPOT.stSTEP_m.sSPD) {
			case DRV_STEP_MIN:		AFSPOT_GetDriveSTEP(DRV_STEP_MIN);	break;

			case DRV_STEP_STD:		if (pCNT->xSTD<4) pCNT->xSTD++;
				if (pCNT->xSTD==4)	AFSPOT_GetDriveSTEP(DRV_STEP_MIN);
				else				AFSPOT_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		if (pCNT->xMID<2) pCNT->xMID++;
				if (pCNT->xMID==2)	AFSPOT_GetDriveSTEP(DRV_STEP_STD);
				else				AFSPOT_GetDriveSTEP(DRV_STEP_MID);	break;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DecDrvStdSPD_XX(BYTE nID)
{
	memset(&stAFSPOT.stSTEP_m.stDEC_D, 0, sizeof(AFS_STEP_COUNT));
	memset(&stAFSPOT.stSTEP_m.stDEC_I, 0, sizeof(AFS_STEP_COUNT));

	switch (stAFSPOT.stSTEP_m.sSPD) {
		case DRV_STEP_MIN:		AFSPOT_GetDriveSTEP(DRV_STEP_STD);	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_SetDrvCurSPD_XX(BYTE nID)
{
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_ClrJudgeRATE(void)
{
	stAFSPOT.stOUTn_h.nMIN = stAFSPOT.stOUTn_h.FLTn;
	stAFSPOT.stOUTn_h.nMAX = stAFSPOT.stOUTn_h.FLTn;
	stAFSPOT.stOUTn_h.zMIN = stAFSPOT.stOUTn_h.FLTn;
	stAFSPOT.stOUTn_h.zMAX = stAFSPOT.stOUTn_h.FLTn;

	stAFSPOT.stOUTn_b.nMIN = stAFSPOT.stOUTn_b.FLTn;
	stAFSPOT.stOUTn_b.nMAX = stAFSPOT.stOUTn_b.FLTn;
	stAFSPOT.stOUTn_b.zMIN = stAFSPOT.stOUTn_b.FLTn;
	stAFSPOT.stOUTn_b.zMAX = stAFSPOT.stOUTn_b.FLTn;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_ClrJudgePEAK(void)
{
	stAFSPOT.stOUTn_h.nHIGH = stAFSPOT.stOUTn_h.FLTn;
	stAFSPOT.stOUTn_b.nHIGH = stAFSPOT.stOUTn_b.FLTn;

	stAFSPOT.stOUTn_h.nPEAK = stAFSPOT.fVAL;
	stAFSPOT.stOUTn_h.xPEAK = stAFSPOT.fVAL;
	stAFSPOT.stOUTn_b.nPEAK = stAFSPOT.fVAL;
	stAFSPOT.stOUTn_b.xPEAK = stAFSPOT.fVAL;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_ClrJudgeINFO(void)
{
	stAFSPOT.stOUTn_h.nCHK = 0;
	stAFSPOT.stOUTn_b.nCHK = 0;

	stAFSPOT.stSTEP_m.sCNT = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_ClrJudgeBUFF(void)
{
	memset(stAFSPOT.stOUTn_h.nFltBUFF, 0, sizeof(AFS_BUFF_PARAM));
	memset(stAFSPOT.stOUTn_b.nFltBUFF, 0, sizeof(AFS_BUFF_PARAM));

	stAFSPOT.xCNT = 0;	stAFSPOT.vCNT &= 0x80;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_MovShiftBUFF(BYTE nID)
{
	BYTE i;

	for (i=1; i<nID; i++) {
		stAFSPOT.stOUTn_h.nFltBUFF[i-1] = stAFSPOT.stOUTn_h.nFltBUFF[i];
		stAFSPOT.stOUTn_b.nFltBUFF[i-1] = stAFSPOT.stOUTn_b.nFltBUFF[i];

		stAFSPOT.stOUTn_h.nFOKS[i-1] = stAFSPOT.stOUTn_h.nFOKS[i];
		stAFSPOT.stOUTn_b.nFOKS[i-1] = stAFSPOT.stOUTn_b.nFOKS[i];
	}
}
//--------------------------------------------------------------------------------------------------------------------------
static CHAR AFSPOT_GetFitJudgeSIGN(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap; SHORT i, move; PLONG pBUFF = (PLONG)pFILT->nFltBUFF;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFSPOT.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	pBUFF[nID] = pFILT->FLTn; pFILT->nFOKS[nID] = stAFSPOT.fVAL;
	if (nID<3) return 0;

	for (i=0; i<nID; i++) {
		move = ABS(pFILT->nFOKS[i+1]-pFILT->nFOKS[i]);
		rate = ((FLOAT)pBUFF[i+1]/pBUFF[i]-1)*1000;
		gap  = (move<15)? 15.0f/move : 1.0f;

		if (rate*gap<-10.0f)	pFILT->nSIGN[i] = -1;
		else					pFILT->nSIGN[i] =  1;
	}

	for (i=1; i<nID; i++) {
		if (pFILT->nSIGN[i-1]!=pFILT->nSIGN[i]) return 0;
	}

	return pFILT->nSIGN[0];
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetFitJudgeRATE(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap; LONG *pBUFF = (PLONG)pFILT->nFltBUFF;

	pFILT->nMOVE = MAX4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3])
				 - MIN4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3]);

	stAFSPOT.fMOV = pFILT->nMOVE/nID;

	gap = (pFILT->nMOVE<15*nID)? (15.0f*nID)/pFILT->nMOVE : 1.0f;

	if (stAFSPOT.nCHK==1)
		 rate = ((FLOAT)(pBUFF[1]+pBUFF[2]+pBUFF[3])/(pBUFF[0]+pBUFF[1]+pBUFF[2])-1);
	else rate = ((FLOAT)(pBUFF[0]+pBUFF[1]+pBUFF[2])/(pBUFF[1]+pBUFF[2]+pBUFF[3])-1);

	rate = (FLOAT)stAFSPOT.nCHK*(rate*1000);
	pFILT->nINC = (LONG)((rate*gap<0)? 0 : rate*gap+0.5);
	pFILT->nDEC = (LONG)((rate*gap>0)? 0 : rate*gap-0.5);

	if (pFILT->nINC>=(LONG)pFILT->n_UP)	pFILT->nCHK |= ( GOOD_INC);
	else								pFILT->nCHK &= (~GOOD_INC);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN)	pFILT->nCHK |= ( GOOD_DEC);
	else								pFILT->nCHK &= (~GOOD_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetFitJudgeDIFF(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap; LONG *pBUFF = (PLONG)pFILT->nFltBUFF;

	pFILT->nMOVE = MAX4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3])
				 - MIN4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3]);

	stAFSPOT.fMOV = pFILT->nMOVE/nID;

	gap = (pFILT->nMOVE<15*nID)? (15.0f*nID)/pFILT->nMOVE : 1.0f;

	rate = MAX4(pBUFF[0],pBUFF[1],pBUFF[2],pBUFF[3])-MIN4(pBUFF[0],pBUFF[1],pBUFF[2],pBUFF[3]);

	pFILT->nINC = (stAFSPOT.nCHK==0)? 0 : (LONG)((FLOAT)rate*gap+0.5);

	if (pFILT->nINC>=stAFSPOT.stNORM_y.FLTd) pFILT->nCHK |= ( GOOD_INC);
	else									 pFILT->nCHK &= (~GOOD_INC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetFitJudgeBUFF(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	CHAR hSIGN = AFSPOT_GetFitJudgeSIGN(&stAFSPOT.stOUTn_h, nID);
	CHAR bSIGN = AFSPOT_GetFitJudgeSIGN(&stAFSPOT.stOUTn_b, nID);

	if (nID<3) return;

	stAFSPOT.nCHK = (hSIGN==bSIGN)? hSIGN : 0;

	AFSPOT_GetFitJudgeRATE(&stAFSPOT.stOUTn_h, nID);
	AFSPOT_GetFitJudgeDIFF(&stAFSPOT.stOUTn_b, nID);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetPrvJudgeBUFF(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap; BYTE i; PLONG pBUFF = (PLONG)pFILT->nFltBUFF;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFSPOT.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	pBUFF[nID] = pFILT->FLTn; pFILT->nFOKS[nID] = stAFSPOT.fVAL;
	if (nID<2) return;

	for (i=0; i<nID; i++) {
		pFILT->nDIFF[i] = (LONG)(pBUFF[i+1]-pBUFF[i]);

		if		(pFILT->nDIFF[i]>0)	pFILT->nSIGN[i] =  1;
		else if (pFILT->nDIFF[i]<0)	pFILT->nSIGN[i] = -1;
		else						pFILT->nSIGN[i] =  0;
	}

	pFILT->nMOVE = MAX3(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2])
				 - MIN3(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2]);

	stAFSPOT.fMOV = pFILT->nMOVE/nID;

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

#if defined(TYPO_FIX)
	if (pFILT->zINC>=(LONG)pFILT->z_UP)	pFILT->nCHK |= ( ZERO_INC);
	else								pFILT->nCHK &= (~ZERO_INC);

	if (pFILT->zDEC<=(LONG)pFILT->z_DN)	pFILT->nCHK |= ( ZERO_DEC);
	else								pFILT->nCHK &= (~ZERO_DEC);
#else
	if (pFILT->zINC>=(LONG)pFILT->n_UP)	pFILT->nCHK |= ( ZERO_INC);
	else								pFILT->nCHK &= (~ZERO_INC);

	if (pFILT->zDEC<=(LONG)pFILT->n_DN)	pFILT->nCHK |= ( ZERO_DEC);
	else								pFILT->nCHK &= (~ZERO_DEC);
#endif
	pFILT->FLTx = pFILT->FLTd = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetDirJudgeBUFF(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap;	PLONG pBUFF = (PLONG)pFILT->nFltBUFF;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFSPOT.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	pBUFF[nID] = pFILT->FLTn; pFILT->nFOKS[nID] = stAFSPOT.fVAL;
	if (nID<3) return;

	pFILT->nMOVE = MAX4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3])
				 - MIN4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3]);

	stAFSPOT.fMOV = pFILT->nMOVE/nID;

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
			pFILT->nCHK |= ( GOOD_INC);
	else	pFILT->nCHK &= (~GOOD_INC);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN || pFILT->zDEC<=(LONG)pFILT->n_DN)
			pFILT->nCHK |= ( GOOD_DEC);
	else	pFILT->nCHK &= (~GOOD_DEC);

	pFILT->FLTx = pFILT->FLTd = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_InvDirJudgeBUFF(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	PLONG pBUFF = (PLONG)pFILT->nFltBUFF;
	FLOAT rate, gap = 15.0f/stAFSPOT.step;

	rate = (pBUFF[2])? ((FLOAT)pFILT->FLTn/pBUFF[2]-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate*gap>0)? 0 : rate*gap-0.5);

	if (pFILT->nDEC<=-1)	pFILT->nCHK |= ( ZERO_DEC);
	else					pFILT->nCHK &= (~ZERO_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetDecJudgeBUFF(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap;	PLONG pBUFF = (PLONG)pFILT->nFltBUFF;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFSPOT.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	pBUFF[nID] = pFILT->FLTn; pFILT->nFOKS[nID] = stAFSPOT.fVAL;
	if (nID<3) return;

	pFILT->nMOVE = MAX4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3])
				 - MIN4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3]);

	stAFSPOT.fMOV = pFILT->nMOVE/nID;

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

	if (pFILT->zINC>=(LONG)pFILT->n_UP)	pFILT->nCHK |= ( GOOD_INC|G_INC_OK);
	else								pFILT->nCHK &= (~GOOD_INC);

	if (pFILT->zDEC<=(LONG)pFILT->n_DN)	pFILT->nCHK |= ( GOOD_DEC|G_DEC_OK);
	else								pFILT->nCHK &= (~GOOD_DEC);

	if (pFILT->zINC>=(LONG)pFILT->z_UP)	pFILT->nCHK |= ( ZERO_INC);
	else								pFILT->nCHK &= (~ZERO_INC);

	if (pFILT->zDEC<=(LONG)pFILT->z_DN)	pFILT->nCHK |= ( ZERO_DEC);
	else								pFILT->nCHK &= (~ZERO_DEC);

	if (pFILT->nINC>=(LONG)pFILT->n_UP)	pFILT->nCHK |= ( HIGH_INC);
	else								pFILT->nCHK &= (~HIGH_INC);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN)	pFILT->nCHK |= ( HIGH_DEC);
	else								pFILT->nCHK &= (~HIGH_DEC);

	if ((pFILT->nCHK&HIGH_INC)==0) pFILT->nCHK &= (~(GOOD_INC));
	if ((pFILT->nCHK&G_INC_OK)==0) return;

	if (pFILT->FLTx>=(LONG)pFILT->n_UP)	pFILT->nCHK |= ( GOOD_INC);

	if (pFILT->FLTd<=(LONG)pFILT->FLTx)	pFILT->nCHK |= ( RATE_DEC|R_DEC_OK);
	else								pFILT->nCHK &= (~RATE_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_InvDecJudgeBUFF(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	PLONG pBUFF = (PLONG)pFILT->nFltBUFF;
	FLOAT rate, gap = 15.0f/stAFSPOT.step;

	rate = (pBUFF[2])? ((FLOAT)pFILT->FLTn/pBUFF[2]-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate*gap>0)? rate*gap : rate*gap-0.5);

	if (pFILT->nDEC<=20)	pFILT->nCHK |= ( ZERO_DEC);
	else					pFILT->nCHK &= (~ZERO_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetTopJudgeBUFF(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFSPOT.fVAL;
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
static void AFSPOT_InvTopJudgeBUFF(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate;

	rate = (pFILT->nMAX)? ((FLOAT)pFILT->FLTn/pFILT->nMAX-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate>0)? rate+0.5 : rate-0.5);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN)	pFILT->nCHK |= ( ZERO_DEC);
	else								pFILT->nCHK &= (~ZERO_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_InvEndJudgeBUFF(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate = ((FLOAT)pFILT->xHIGH/pFILT->FLTn-1);

	rate *= ((nID==USE_HPF)? 500 : 1000);
	pFILT->nDEC = (LONG)((rate<0)? rate-0.5 : rate+0.5);

	if (ABS(pFILT->nDEC)<100)	pFILT->nCHK |= ( GOOD_END);
	else						pFILT->nCHK &= (~GOOD_END);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetJudge_FIT(void)
{
	PAFS_FILT_PARAM pFILTh = &stAFSPOT.stOUTn_h;
	PAFS_FILT_PARAM pFILTb = &stAFSPOT.stOUTn_b;

	AFSPOT_GetDriveRATE(DRV_SEARCH_FIT);

	AFSPOT_GetFitJudgeBUFF(&stAFSPOT.stOUTn_h, stAFSPOT.xCNT);

	if (AFSPOT_IsDriveEnd()==TRUE)	stAFSPOT.xCNT  = 1;
	else {			if (stAFSPOT.xCNT != 0xff) stAFSPOT.xCNT += 1;
	}

	if (AFSPOT_IsDriveEnd()==FALSE && stAFSPOT.xCNT<4) return;

	if (stAFSPOT.xCNT) stAFSPOT.xCNT--;	AFSPOT_MovShiftBUFF(4);

	if (AFSPOT_IsDriveEnd()==TRUE) {
		if(stAFSPOT.eCNT != 0xff) stAFSPOT.eCNT++;			AFSPOT_ClrJudgeRATE();
		stAFSPOT.cur = INV_SEARCH_FIT;	stAFSPOT.nxt = DRV_SEARCH_FIT;
	}
	else if (pFILTh->nCHK&GOOD_DEC && pFILTb->nCHK&GOOD_INC) {
			AFSPOT_DecDrvDecSPD_GD(DRV_SEARCH_DIR);
		stAFSPOT.cur = INV_SEARCH_FIT;	stAFSPOT.nxt = PRE_SEARCH_DIR;
	}
	else if (pFILTh->nCHK&GOOD_INC && pFILTb->nCHK&GOOD_INC) {
			AFSPOT_DecDrvDecSPD_GD(DRV_SEARCH_DIR);
		stAFSPOT.cur = DRV_SEARCH_DIR;	stAFSPOT.nxt = DRV_SEARCH_DIR;
	}
	else {	AFSPOT_DirDrvIncSPD_BI(DRV_SEARCH_FIT);
		stAFSPOT.cur = DRV_SEARCH_FIT;	stAFSPOT.nxt = DRV_SEARCH_FIT;
	}

	if (stAFSPOT.nxt!=DRV_SEARCH_FIT) AFSPOT_ClrJudgeRATE();
	if (stAFSPOT.nxt!=DRV_SEARCH_FIT) AFSPOT_ClrJudgePEAK();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetJudge_FAR(void)
{
	PAFS_FILT_PARAM pFILTx = &stAFSPOT.stOUTn_h;

	AFSPOT_GetDriveRATE(DRV_SEARCH_FAR);

	AFSPOT_GetPrvJudgeBUFF(&stAFSPOT.stOUTn_h, stAFSPOT.xCNT);
	AFSPOT_GetPrvJudgeBUFF(&stAFSPOT.stOUTn_b, stAFSPOT.xCNT);

	if (AFSPOT_IsDriveEnd()==FALSE) {
		if (stAFSPOT.xCNT != 0xff) stAFSPOT.xCNT++;	if (stAFSPOT.xCNT<3) return;
		if ((stAFSPOT.vCNT&0xf) != 0xf) stAFSPOT.vCNT++;
	}

	if (AFSPOT_IsDriveEnd()==TRUE) {
		if(stAFSPOT.eCNT != 0xff) stAFSPOT.eCNT++;			AFSPOT_ClrJudgeRATE();
		stAFSPOT.cur = INV_SEARCH_FAR;	stAFSPOT.nxt = PRE_SEARCH_DIR;
	}
	else if (pFILTx->nCHK&GOOD_DEC) {
		stAFSPOT.cur = INV_SEARCH_FAR;	stAFSPOT.nxt = END_SEARCH_FAR;
	}
	else if (pFILTx->nCHK&GOOD_INC) {
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;
	}
	else if ((stAFSPOT.vCNT&0xf)==5) {
		stAFSPOT.cur = INV_SEARCH_FAR;	stAFSPOT.nxt = PRE_SEARCH_DIR;
	}
	else {	if (stAFSPOT.xCNT) stAFSPOT.xCNT--;			AFSPOT_MovShiftBUFF(3);
		stAFSPOT.cur = DRV_SEARCH_FAR;	stAFSPOT.nxt = DRV_SEARCH_FAR;
	}

	if (pFILTx->nCHK&GOOD_DEC) stAFSPOT.nSEL = USE_HPF;
	if (pFILTx->nCHK&GOOD_INC) stAFSPOT.nSEL = USE_HPF;

	if (stAFSPOT.cur==DRV_CLIMB_TOP2) AFSPOT_ClrJudgePEAK();
	if (stAFSPOT.cur==INV_SEARCH_FAR) AFSPOT_ClrJudgeBUFF();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetJudge_NAR(void)
{
	PAFS_FILT_PARAM pFILTx = &stAFSPOT.stOUTn_h;

	AFSPOT_GetDriveRATE(DRV_SEARCH_NAR);

	AFSPOT_GetPrvJudgeBUFF(&stAFSPOT.stOUTn_h, stAFSPOT.xCNT);
	AFSPOT_GetPrvJudgeBUFF(&stAFSPOT.stOUTn_b, stAFSPOT.xCNT);

	if (AFSPOT_IsDriveEnd()==FALSE) {
		if (stAFSPOT.xCNT != 0xff) stAFSPOT.xCNT++;	if (stAFSPOT.xCNT<3) return;
		if ((stAFSPOT.vCNT&0xf) != 0xf) stAFSPOT.vCNT++;
	}

	if (AFSPOT_IsDriveEnd()==TRUE) {
		if(stAFSPOT.eCNT != 0xff) stAFSPOT.eCNT++;			AFSPOT_ClrJudgeRATE();
		stAFSPOT.cur = INV_SEARCH_NAR;	stAFSPOT.nxt = PRE_SEARCH_DIR;
	}
	else if (pFILTx->nCHK&GOOD_DEC) {
		stAFSPOT.cur = INV_SEARCH_NAR;	stAFSPOT.nxt = DRV_FORCE_PEAK;
	}
	else if (pFILTx->nCHK&GOOD_INC) {
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;
	}
	else if ((stAFSPOT.vCNT&0xf)==5) {
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;
	}
	else {	if (stAFSPOT.xCNT) stAFSPOT.xCNT--;			AFSPOT_MovShiftBUFF(3);
		stAFSPOT.cur = DRV_SEARCH_NAR;	stAFSPOT.nxt = DRV_SEARCH_NAR;
	}

	if (stAFSPOT.cur==INV_SEARCH_NAR) AFSPOT_ClrJudgeBUFF();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetJudge_DIR(void)
{
	PAFS_FILT_PARAM pFILTh = &stAFSPOT.stOUTn_h;
	PAFS_FILT_PARAM pFILTb = &stAFSPOT.stOUTn_b;

	AFSPOT_GetDriveRATE(DRV_SEARCH_DIR);	// get judge RATE

	AFSPOT_GetDirJudgeBUFF(&stAFSPOT.stOUTn_h, stAFSPOT.xCNT);
	AFSPOT_GetDirJudgeBUFF(&stAFSPOT.stOUTn_b, stAFSPOT.xCNT);

	if (AFSPOT_IsDriveEnd()==FALSE) {
		if (stAFSPOT.xCNT != 0xff) stAFSPOT.xCNT++; if (stAFSPOT.xCNT<4) return;
	}

	if (stAFSPOT.xCNT) stAFSPOT.xCNT--;	AFSPOT_MovShiftBUFF(4);

	if (AFSPOT_IsDriveEnd()==TRUE) {
		if(stAFSPOT.eCNT != 0xff) stAFSPOT.eCNT++;			AFSPOT_ClrJudgeRATE();
		stAFSPOT.cur = INV_SEARCH_DIR;	stAFSPOT.nxt = DRV_SEARCH_DIR;
	}
	else if (pFILTb->nCHK&GOOD_DEC) {
			AFSPOT_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFSPOT.cur = INV_SEARCH_DIR;	stAFSPOT.nxt = PRE_CLIMB_TOP2;
	}
	else if (pFILTb->nCHK&GOOD_INC) {
			AFSPOT_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;
	}
	else if (pFILTh->nCHK&GOOD_DEC) {
			AFSPOT_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFSPOT.cur = INV_SEARCH_DIR;	stAFSPOT.nxt = PRE_CLIMB_TOP2;
	}
	else if (pFILTh->nCHK&GOOD_INC) {
			AFSPOT_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;
	}
	else {
			AFSPOT_DirDrvIncSPD_BI(DRV_SEARCH_DIR);
		stAFSPOT.cur = DRV_SEARCH_DIR;	stAFSPOT.nxt = DRV_SEARCH_DIR;
	}

	if (pFILTh->nCHK&GOOD_DEC) stAFSPOT.nSEL = USE_HPF;
	if (pFILTh->nCHK&GOOD_INC) stAFSPOT.nSEL = USE_HPF;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_InvJudge_DIR(void)
{
	PAFS_FILT_PARAM pFILTx = &stAFSPOT.stOUTn_b;
	PAFS_FILT_PARAM pFILTb = &stAFSPOT.stOUTn_b;

	if (stAFSPOT.nSEL==USE_HPF) pFILTx = &stAFSPOT.stOUTn_h;

	if (stAFSPOT.nxt!=PRE_CLIMB_TOP2) AFSPOT_ClrJudgeBUFF();
	if (stAFSPOT.nxt!=PRE_CLIMB_TOP2) return;

	AFSPOT_GetDriveRATE(DRV_SEARCH_DIR);

	AFSPOT_InvDirJudgeBUFF(&stAFSPOT.stOUTn_h, stAFSPOT.xCNT);
	AFSPOT_InvDirJudgeBUFF(&stAFSPOT.stOUTn_b, stAFSPOT.xCNT);

	if ((pFILTb->nCHK&ZERO_DEC)==0 || (pFILTx->nCHK&ZERO_DEC)==0) {
		stAFSPOT.nxt = DRV_SEARCH_DIR;	AFSPOT_SetChangeDIR();
		pFILTb->nMAX = (pFILTb->nMAX+pFILTb->FLTn)/2;
		pFILTx->nMAX = (pFILTx->nMAX+pFILTx->FLTn)/2;
	}

	if (stAFSPOT.nxt==DRV_SEARCH_DIR) AFSPOT_ClrJudgePEAK();
	if (stAFSPOT.nxt==PRE_CLIMB_TOP2) AFSPOT_ClrJudgePEAK();
	if (stAFSPOT.nxt==PRE_CLIMB_TOP2) AFSPOT_ClrJudgeBUFF();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetJudge_DEC(void)
{
	PAFS_FILT_PARAM pFILTx = &stAFSPOT.stOUTn_b;
	PAFS_FILT_PARAM pFILTh = &stAFSPOT.stOUTn_h;
	PAFS_FILT_PARAM pFILTb = &stAFSPOT.stOUTn_b;

	if (stAFSPOT.nSEL==USE_HPF) pFILTx = &stAFSPOT.stOUTn_h;

	AFSPOT_GetDriveRATE(DRV_CLIMB_TOP2);

	AFSPOT_GetDecJudgeBUFF(&stAFSPOT.stOUTn_h, stAFSPOT.xCNT);
	AFSPOT_GetDecJudgeBUFF(&stAFSPOT.stOUTn_b, stAFSPOT.xCNT);

	if (stAFSPOT.ZOOM<=stAFSPOT.KNEE && (pFILTh->nCHK&HIGH_INC)==0)
		pFILTb->nCHK &= (~(GOOD_INC|G_INC_OK));

	if (AFSPOT_IsDriveEnd()==FALSE) {
		if (stAFSPOT.xCNT != 0xff) stAFSPOT.xCNT++; if (stAFSPOT.xCNT<4) return;
	}

	if (stAFSPOT.xCNT) stAFSPOT.xCNT--;	AFSPOT_MovShiftBUFF(4);

	if (AFSPOT_IsDriveEnd()==TRUE) {
		if(stAFSPOT.eCNT != 0xff) stAFSPOT.eCNT++;			AFSPOT_ClrJudgeRATE();
		stAFSPOT.cur = INV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;
	}
	else if (pFILTh->nCHK&GOOD_DEC && pFILTb->nCHK&GOOD_DEC) {
		stAFSPOT.cur = INV_CLIMB_TOP2;	stAFSPOT.nxt = PRE_CLIMB_PEAK;
	}
	else if (pFILTb->nCHK&GOOD_INC && pFILTb->nCHK&RATE_DEC) {
			AFSPOT_DecDrvDecSPD_GD(DRV_CLIMB_TOP2);
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;
	}
	else if (pFILTb->nCHK&GOOD_INC) {
			AFSPOT_DecDrvIncSPD_GX(DRV_CLIMB_TOP2);
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;
	}
	else if (pFILTh->nCHK&GOOD_DEC || pFILTh->nCHK&HIGH_DEC) {
			AFSPOT_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;
	}
	else {
			AFSPOT_DecDrvIncSPD_BI(DRV_CLIMB_TOP2);
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;
	}

	if (pFILTh->nCHK&GOOD_INC || pFILTh->nCHK&HIGH_INC) stAFSPOT.nSEL = USE_HPF;
	if (pFILTh->nCHK&GOOD_DEC || pFILTh->nCHK&HIGH_DEC) stAFSPOT.nSEL = USE_HPF;

	if (pFILTb->nCHK&R_DEC_OK && pFILTx->nCHK&ZERO_DEC) {
		stAFSPOT.cur = INV_CLIMB_TOP2;	stAFSPOT.nxt = PRE_CLIMB_PEAK;
	}

	if (pFILTb->nCHK&HIGH_DEC && pFILTx->nCHK&ZERO_DEC) {
		stAFSPOT.cur = INV_CLIMB_TOP2;	stAFSPOT.nxt = PRE_CLIMB_PEAK;
	}

	if (pFILTh->nCHK&G_INC_OK && pFILTh->nCHK&HIGH_DEC) {
		stAFSPOT.cur = INV_CLIMB_TOP2;	stAFSPOT.nxt = PRE_CLIMB_PEAK;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_InvJudge_DEC(void)
{
	PAFS_FILT_PARAM pFILTx = &stAFSPOT.stOUTn_b;


	if (stAFSPOT.nxt!=PRE_CLIMB_PEAK) AFSPOT_ClrJudgeBUFF();
	if (stAFSPOT.nxt!=PRE_CLIMB_PEAK) return;

	AFSPOT_GetDriveRATE(DRV_CLIMB_TOP2);

	AFSPOT_InvDecJudgeBUFF(&stAFSPOT.stOUTn_h, stAFSPOT.xCNT);
	AFSPOT_InvDecJudgeBUFF(&stAFSPOT.stOUTn_b, stAFSPOT.xCNT);

	if ((pFILTx->nCHK&ZERO_DEC)==0) {
		stAFSPOT.nxt = DRV_CLIMB_TOP2;	AFSPOT_SetChangeDIR();
	}

	AFSPOT_DecDrvStdSPD_XX(DRV_CLIMB_TOP2);
	if (stAFSPOT.nxt==DRV_CLIMB_TOP2) return;

	AFSPOT_GetDriveSTEP(DRV_STEP_DIV);
	AFSPOT_ClrJudgeBUFF();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetJudge_TOP(void)
{
	PAFS_FILT_PARAM pFILTx = &stAFSPOT.stOUTn_b;

	if (stAFSPOT.nSEL==USE_HPF) pFILTx = &stAFSPOT.stOUTn_h;

	AFSPOT_GetDriveRATE(DRV_CLIMB_PEAK);

	AFSPOT_GetTopJudgeBUFF(&stAFSPOT.stOUTn_h, stAFSPOT.xCNT);
	AFSPOT_GetTopJudgeBUFF(&stAFSPOT.stOUTn_b, stAFSPOT.xCNT);

	if (AFSPOT_IsDriveEnd()==TRUE) {
		stAFSPOT.cur = INV_CLIMB_PEAK;	stAFSPOT.nxt = DRV_FORCE_PEAK;
	}
	else if (pFILTx->nCHK&GOOD_DEC) {
		stAFSPOT.cur = INV_CLIMB_PEAK;	stAFSPOT.nxt = DRV_FORCE_PEAK;
	}
	else {
		stAFSPOT.cur = DRV_CLIMB_PEAK;	stAFSPOT.nxt = DRV_CLIMB_PEAK;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_InvJudge_TOP(void)
{
	PAFS_FILT_PARAM pFILTx = &stAFSPOT.stOUTn_b;

	if (stAFSPOT.nSEL==USE_HPF) pFILTx = &stAFSPOT.stOUTn_h;

	if (AFSPOT_IsDriveEnd()==TRUE) return;

	AFSPOT_GetDriveRATE(DRV_CLIMB_PEAK);

	AFSPOT_InvTopJudgeBUFF(&stAFSPOT.stOUTn_h, stAFSPOT.xCNT);
	AFSPOT_InvTopJudgeBUFF(&stAFSPOT.stOUTn_b, stAFSPOT.xCNT);

	if (pFILTx->nCHK&ZERO_DEC) return;

	stAFSPOT.nxt = DRV_CLIMB_PEAK;
	AFSPOT_SetChangeDIR();
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFSPOT_InvJudge_END(void)
{
	PAFS_FILT_PARAM pFILTx = &stAFSPOT.stOUTn_b;

	if (stAFSPOT.nSEL==USE_HPF) pFILTx = &stAFSPOT.stOUTn_h;

	if (stAFSPOT.eCNT==2) return FALSE;
	AFSPOT_GetDriveRATE(DRV_CLIMB_PEAK);

	AFSPOT_InvEndJudgeBUFF(&stAFSPOT.stOUTn_h, USE_HPF);
	AFSPOT_InvEndJudgeBUFF(&stAFSPOT.stOUTn_b, USE_BPF);

	if ((pFILTx->nCHK&GOOD_END)!=0) return FALSE;
	else							return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_SetInitPARAM(void)
{
	stAFSPOT.peak = stAFSPOT.fVAL;

	stAFSPOT.fMID = LENS_GetMIDPositionLOCI();
	stAFSPOT.fGAP = LENS_GetGAPPositionLOCI();

	stAFSPOT.fMIN = LENS_GetMINPositionLOCI(stAFSPOT.zVAL);
	stAFSPOT.fMAX = LENS_GetMAXPositionLOCI(stAFSPOT.zVAL);

	stAFSPOT.fABV = LENS_GetABVPositionLOCI(stAFSTUNE.r_LOCI);
	stAFSPOT.fBLO = LENS_GetBLOPositionLOCI(stAFSTUNE.r_LOCI);

	stAFSPOT.nDIR = LENS_GetDirectionFOKS();
	LENS_SetDirectionFOKS(stAFSPOT.nDIR);

	if ((stAFSPOT.nMOD&SPOT_XX)==0) return;

	stAFSPOT.fMID = (stAFSPOT.fABV+stAFSPOT.fBLO)>>1;
	stAFSPOT.nDIR = (stAFSPOT.fVAL>stAFSPOT.fMID)? fNEAR : fFAR;
	LENS_SetDirectionFOKS(stAFSPOT.nDIR);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_PreFilterSEL(void)
{
	if (stAFSPOT.cur!=PRE_FILTER_SEL) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFSPOT_SetInitPARAM();	LENS_EnableMotorFOKS(ON);
	stAFSPOT.eCNT = 0; stAFSPOT.cur = PRE_SEARCH_MOV;
	stAFSPOT.vCNT = 0; stAFSPOT.nxt = PRE_SEARCH_MOV;

	memset(&stAFSPOT.stSTEP_m, 0, sizeof(AFS_STEP_PARAM));
	stAFSPOT.stSTEP_m.sMIN = LENS_GetAFDriveSTEP(DRV_STEP_MIN);
	stAFSPOT.stSTEP_m.sSTD = LENS_GetAFDriveSTEP(DRV_STEP_STD);
	stAFSPOT.stSTEP_m.sMID = LENS_GetAFDriveSTEP(DRV_STEP_MID);
	stAFSPOT.stSTEP_m.sMAX = LENS_GetAFDriveSTEP(DRV_STEP_MAX);
	stAFSPOT.stSTEP_m.sFWD = LENS_GetAFDriveSTEP(DRV_STEP_FWD);
	stAFSPOT.stSTEP_m.sBWD = LENS_GetAFDriveSTEP(DRV_STEP_BWD);
	stAFSPOT.stSTEP_m.sTOP = LENS_GetAFDriveSTEP(DRV_STEP_TOP);

	if (stAFSPOT.nSEL==USE_HPF) stAFSPOT.vCNT |= 0x80;
	stAFSPOT.nSEL = USE_HPF;	AFSPOT_ClrJudgeBUFF();

	memset(&stAFSPOT.stOUTn_h.nMIN, 0, sizeof(AFS_RATE_PARAM));
	memset(&stAFSPOT.stOUTn_b.nMIN, 0, sizeof(AFS_RATE_PARAM));

	stAFSPOT.stNORM_y.nRST = 0;
	stAFSPOT.stNORM_y.nERR = 0;
	stAFSPOT.stNORM_y.nCNT = 0;

	stAFSTUNE.n_GAIN = 0;
	stAFSTUNE.n_YSUM = 0;
	stAFSTUNE.n_FLTr = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
//static void AFSPOT_EndFilterSEL(void)
//{
//	if (stAFSPOT.cur!=END_FILTER_SEL) return;
//	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
//
//	stAFSPOT.cur = PRE_SEARCH_MOV;
//	stAFSPOT.nxt = PRE_SEARCH_MOV;
//}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_PreSearchMOV(void)
{
	if (stAFSPOT.cur!=PRE_SEARCH_MOV) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFSPOT.stOUTn_b.xHIGH = stAFSPOT.stOUTn_b.FLTn;
	stAFSPOT.stOUTn_h.xHIGH = stAFSPOT.stOUTn_h.FLTn;

	stAFSPOT.cur = DRV_SEARCH_MOV;
	//stAFSTUNE.n_GAIN = 0;
	//stAFSTUNE.n_YSUM = 0;
	//stAFSTUNE.n_FLTr = 0;

	stAFSPOT.nxt = PRE_SEARCH_FIT;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DrvSearchMOV(void)
{
	if (stAFSPOT.cur!=DRV_SEARCH_MOV) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFSPOT.cur = stAFSPOT.nxt;

	AFSPOT_ClrJudgeRATE();
	AFSPOT_ClrJudgePEAK();
	AFSPOT_ClrJudgeINFO();

	if (stAFSPOT.nxt==PRE_SEARCH_FAR)
		 AFSPOT_GetJudge_FAR();
	else AFSPOT_GetJudge_FIT();

	AFSPOT_GetDriveSTEP(DRV_STEP_MIN);
	AFSPOT_SetDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_PreSearchFIT(void)
{
	if (stAFSPOT.cur!=PRE_SEARCH_FIT) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFSPOT.cur = DRV_SEARCH_FIT;
	stAFSPOT.nxt = DRV_SEARCH_FIT;

	AFSPOT_GetJudge_FIT();

	AFSPOT_SetDriveSTEP(DRV_STEP_PRV);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DrvSearchFIT(void)
{
	if (stAFSPOT.cur!=DRV_SEARCH_FIT) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFSPOT_GetJudge_FIT();

	if (stAFSPOT.cur==INV_SEARCH_FIT) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_PRV);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_EndSearchFIT(void)
{
	if (stAFSPOT.cur!=END_SEARCH_FIT) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFSPOT.nSEL==USE_HPF)
		 stAFSPOT.peak = stAFSPOT.stOUTn_h.nPEAK;
	else stAFSPOT.peak = stAFSPOT.stOUTn_b.nPEAK;

	if (stAFSPOT.eCNT==2)
		 stAFSPOT.cur = DRV_FORCE_PEAK;
	else stAFSPOT.cur = stAFSPOT.nxt;

	if (stAFSPOT.cur==DRV_FORCE_PEAK) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_PRV);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_PreSearchFAR(void)
{
	if (stAFSPOT.cur!=PRE_SEARCH_FAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFSPOT.cur = DRV_SEARCH_FAR;
	stAFSPOT.nxt = DRV_SEARCH_FAR;

	AFSPOT_GetJudge_FAR();

	AFSPOT_SetDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DrvSearchFAR(void)
{
	if (stAFSPOT.cur!=DRV_SEARCH_FAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFSPOT_GetJudge_FAR();

	if (stAFSPOT.cur==INV_SEARCH_FAR) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_EndSearchFAR(void)
{
	if (stAFSPOT.cur!=END_SEARCH_FAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFSPOT.nSEL==USE_HPF)
		 stAFSPOT.peak = stAFSPOT.stOUTn_h.xPEAK;
	else stAFSPOT.peak = stAFSPOT.stOUTn_b.xPEAK;

	stAFSPOT.cur = stAFSPOT.nxt;
	stAFSPOT.nxt = PRE_SEARCH_NAR;

	if (stAFSPOT.cur==DRV_FORCE_PEAK) return;
	if (stAFSPOT.fVAL==stAFSPOT.peak) return;

	AFSPOT_GetDriveSTEP(DRV_STEP_BWD);
	AFSPOT_GetDriveSTEP(DRV_STEP_REM);
	AFSPOT_SetDriveSTEP(DRV_STEP_REM);
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFSPOT_MovSearchNAR(void)
{
	if (stAFSPOT.cur!=PRE_SEARCH_NAR) return FALSE;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return FALSE;

	if (stAFSPOT.fVAL==stAFSPOT.peak) return TRUE;

	if (stAFSPOT.nDIR== fFAR && stAFSPOT.fVAL<stAFSPOT.peak)
		AFSPOT_GetDriveSTEP(DRV_STEP_REM);

	if (stAFSPOT.nDIR==fNEAR && stAFSPOT.fVAL>stAFSPOT.peak)
		AFSPOT_GetDriveSTEP(DRV_STEP_REM);

	AFSPOT_SetDriveSTEP(DRV_STEP_REM);
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_PreSearchNAR(void)
{
	if (stAFSPOT.cur!=PRE_SEARCH_NAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (AFSPOT_MovSearchNAR()==FALSE) return;

	stAFSPOT.cur = DRV_SEARCH_NAR;
	stAFSPOT.nxt = DRV_SEARCH_NAR;

	AFSPOT_ClrJudgeRATE();
	AFSPOT_ClrJudgePEAK();
	AFSPOT_ClrJudgeINFO();
	AFSPOT_GetJudge_NAR();

	AFSPOT_GetDriveSTEP(DRV_STEP_MIN);
	AFSPOT_SetDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DrvSearchNAR(void)
{
	if (stAFSPOT.cur!=DRV_SEARCH_NAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFSPOT_GetJudge_NAR();

	if (stAFSPOT.cur==INV_SEARCH_NAR) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_EndSearchNAR(void)
{
	if (stAFSPOT.cur!=END_SEARCH_NAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFSPOT.nSEL==USE_HPF)
		 stAFSPOT.peak = stAFSPOT.stOUTn_h.xPEAK;
	else stAFSPOT.peak = stAFSPOT.stOUTn_b.xPEAK;

	stAFSPOT.cur = stAFSPOT.nxt;

	if (stAFSPOT.cur==DRV_FORCE_PEAK) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_MIN);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_PreSearchDIR(void)
{
	if (stAFSPOT.cur!=PRE_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFSPOT.cur = DRV_SEARCH_DIR;
	stAFSPOT.nxt = DRV_SEARCH_DIR;

	AFSPOT_GetJudge_DIR();

	AFSPOT_GetDriveSTEP(DRV_STEP_STD);
	AFSPOT_SetDriveSTEP(DRV_STEP_STD);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DrvSearchDIR(void)
{
	if (stAFSPOT.cur!=DRV_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFSPOT_GetJudge_DIR();

	if (stAFSPOT.cur==INV_SEARCH_DIR) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_STD);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_EndSearchDIR(void)
{
	if (stAFSPOT.cur!=END_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFSPOT.nSEL==USE_HPF)
		 stAFSPOT.peak = stAFSPOT.stOUTn_h.nPEAK;
	else stAFSPOT.peak = stAFSPOT.stOUTn_b.nPEAK;

	AFSPOT_InvJudge_DIR();

	if (stAFSPOT.eCNT==2)
		 stAFSPOT.cur = DRV_FORCE_PEAK;
	else stAFSPOT.cur = stAFSPOT.nxt;

	if (stAFSPOT.cur==DRV_FORCE_PEAK) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_PRV);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_PreClimbTOP2(void)
{
	if (stAFSPOT.cur!=PRE_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFSPOT.cur = DRV_CLIMB_TOP2;
	stAFSPOT.nxt = DRV_CLIMB_TOP2;

	AFSPOT_ClrJudgeRATE();
	AFSPOT_ClrJudgeINFO();
	AFSPOT_GetJudge_DEC();

	AFSPOT_SetDriveSTEP(DRV_STEP_PRV);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DrvClimbTOP2(void)
{
	if (stAFSPOT.cur!=DRV_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFSPOT_GetJudge_DEC();

	if (stAFSPOT.cur==INV_CLIMB_TOP2) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_PRV);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_EndClimbTOP2(void)
{
	if (stAFSPOT.cur!=END_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFSPOT.nSEL==USE_HPF)
		 stAFSPOT.peak = stAFSPOT.stOUTn_h.nPEAK;
	else stAFSPOT.peak = stAFSPOT.stOUTn_b.nPEAK;

	AFSPOT_InvJudge_DEC();

	if (stAFSPOT.eCNT==2)
		 stAFSPOT.cur = DRV_FORCE_PEAK;
	else stAFSPOT.cur = stAFSPOT.nxt;

	if (stAFSPOT.cur==DRV_FORCE_PEAK) return;
	if (stAFSPOT.cur==PRE_CLIMB_PEAK &&
		stAFSPOT.fVAL==stAFSPOT.peak) return;

	AFSPOT_GetDriveSTEP(DRV_STEP_PRV);
	AFSPOT_SetDriveSTEP(DRV_STEP_PRV);
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFSPOT_MovClimbPEAK(void)
{
	if (stAFSPOT.cur!=PRE_CLIMB_PEAK) return FALSE;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return FALSE;

	if (stAFSPOT.fVAL==stAFSPOT.peak) return TRUE;

	if (stAFSPOT.nDIR== fFAR && stAFSPOT.fVAL>stAFSPOT.peak) {
		stAFSPOT.stNORM_y.nERR = TRUE;	return TRUE;
	}
	if (stAFSPOT.nDIR==fNEAR && stAFSPOT.fVAL<stAFSPOT.peak) {
		stAFSPOT.stNORM_y.nERR = TRUE;	return TRUE;
	}

	if (stAFSPOT.nDIR== fFAR && stAFSPOT.fVAL<stAFSPOT.peak)
		AFSPOT_GetDriveSTEP(DRV_STEP_REM);

	if (stAFSPOT.nDIR==fNEAR && stAFSPOT.fVAL>stAFSPOT.peak)
		AFSPOT_GetDriveSTEP(DRV_STEP_REM);

	AFSPOT_SetDriveSTEP(DRV_STEP_REM);
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_PreClimbPEAK(void)
{
	if (stAFSPOT.cur!=PRE_CLIMB_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (AFSPOT_MovClimbPEAK()==FALSE) return;

	stAFSPOT.cur = DRV_CLIMB_PEAK;
	stAFSPOT.nxt = DRV_CLIMB_PEAK;

	AFSPOT_ClrJudgeRATE();
	AFSPOT_ClrJudgePEAK();
	AFSPOT_ClrJudgeINFO();
	AFSPOT_GetJudge_TOP();

	if (stAFSPOT.stNORM_y.nERR) return;

	AFSPOT_GetDriveSTEP(DRV_STEP_TOP);
	AFSPOT_SetDriveSTEP(DRV_STEP_TOP);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DrvClimbPEAK(void)
{
	if (stAFSPOT.cur!=DRV_CLIMB_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFSPOT_GetJudge_TOP();

	if (stAFSPOT.cur==INV_CLIMB_PEAK) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_TOP);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_EndClimbPEAK(void)
{
	if (stAFSPOT.cur!=END_CLIMB_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFSPOT.nSEL==USE_HPF)
		 stAFSPOT.peak = stAFSPOT.stOUTn_h.nPEAK;
	else stAFSPOT.peak = stAFSPOT.stOUTn_b.nPEAK;

	if (stAFSPOT.nDIR==fFAR)
		 stAFSPOT.peak += 2*stAFSPOT.stSTEP_m.sTOP;
	else stAFSPOT.peak -= 0*stAFSPOT.stSTEP_m.sTOP;

	AFSPOT_InvJudge_TOP();

	stAFSPOT.cur = stAFSPOT.nxt;

	if (stAFSPOT.cur==DRV_FORCE_PEAK) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_TOP);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DrvForcePEAK(void)
{
	if (stAFSPOT.cur!=DRV_FORCE_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFSPOT.nDIR==fFAR) stAFSPOT.peak += stAFSTUNE.h_FOKS;
	else					 stAFSPOT.peak -= stAFSTUNE.h_FOKS;

	stAFSPOT.cur = END_FORCE_PEAK;
	AFSPOT_SetDrivePEAK();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_EndForcePEAK(void)
{
	if (stAFSPOT.cur!=END_FORCE_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFSPOT.cur = END_CLEAR_WAIT;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_EndDriveWAIT(void)
{
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	switch (stAFSPOT.cur) {
		case INV_FILTER_SEL: stAFSPOT.cur = PRE_SEARCH_MOV; break;
		case INV_SEARCH_FIT: stAFSPOT.cur = END_SEARCH_FIT; break;
		case INV_SEARCH_FAR: stAFSPOT.cur = END_SEARCH_FAR; break;
		case INV_SEARCH_NAR: stAFSPOT.cur = END_SEARCH_NAR; break;
		case INV_SEARCH_DIR: stAFSPOT.cur = END_SEARCH_DIR; break;
		case INV_CLIMB_TOP1: stAFSPOT.cur = END_CLIMB_TOP1; break;
		case INV_CLIMB_TOP2: stAFSPOT.cur = END_CLIMB_TOP2; break;
		case INV_CLIMB_PEAK: stAFSPOT.cur = END_CLIMB_PEAK; break;
		default:			 return;
	}
	AFSPOT_SetChangeDIR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_EndClearLUMA(PAF_CTRL_PARAM pCTRL)
{
	WORD nID = MAKEWORD(stAFSPOT.stNORM_y.nERR,stAFSPOT.fMOV);

	if (stAFSPOT.cur<=END_SEARCH_MOV) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFSPOT.stNORM_y.nRST |= AF_GetLUMAInfo(pCTRL,nID);
	stAFSPOT.stNORM_y.nERR  = FALSE;

	if (stAFSPOT.stNORM_y.nRST) stAFSPOT.cur = END_FORCE_PEAK;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_EndClearAUTO(PAF_CTRL_PARAM pCTRL)
{
	if (stAFSPOT.cur!=END_CLEAR_AUTO) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFSPOT.cur = END_CLEAR_PROC;

	if (stAFSPOT.stNORM_y.nRST==TRUE) return;
	AF_SetAUTOInfo(pCTRL, stAFSPOT.stNORM_y.nCHK);

	if (stAFSPOT.ZOOM<=stAFSPOT.KNEE) return;
	LENS_SetTrackingINFO(stAFSPOT.peak);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_EndClearWAIT(PAF_CTRL_PARAM pCTRL)
{
	BOOL nID;

	if (stAFSPOT.cur!=END_CLEAR_WAIT) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	nID = (AFSPOT_InvJudge_END())? TRUE : FALSE;
	stAFSPOT.stNORM_y.nRST |= AF_GetDATAInfo(pCTRL,nID);

	stAFSPOT.cur = END_CLEAR_AUTO;
	LENS_EnableMotorFOKS(OFF);
}

//--------------------------------------------------------------------------------------------------------------------------
void AFSPOT_ProcessHandler(PAF_CTRL_PARAM pCTRL)
{
	if (stAFSPOT.cur==0) return;
	stAFSPOT.prv = stAFSPOT.cur;
	stAFSPOT.nMOD = pCTRL->nMOD;

	AFSPOT_GetAFOPDWIND(pCTRL);	
	AFSPOT_EndClearLUMA(pCTRL);	
	AFSPOT_EndClearAUTO(pCTRL);	
	AFSPOT_EndClearWAIT(pCTRL);	

	AFSPOT_PreFilterSEL();		
	AFSPOT_PreSearchMOV();		
	AFSPOT_PreSearchFIT();		
	AFSPOT_PreSearchFAR();		
	AFSPOT_PreSearchNAR();		
	AFSPOT_PreSearchDIR();		
	AFSPOT_PreClimbTOP2();		
	AFSPOT_PreClimbPEAK();		

	AFSPOT_DrvSearchMOV();		
	AFSPOT_DrvSearchFIT();		
	AFSPOT_DrvSearchFAR();		
	AFSPOT_DrvSearchNAR();		
	AFSPOT_DrvSearchDIR();		
	AFSPOT_DrvClimbTOP2();		
	AFSPOT_DrvClimbPEAK();		
	AFSPOT_DrvForcePEAK();		

	//AFSPOT_EndFilterSEL();		
	AFSPOT_EndSearchFIT();		
	AFSPOT_EndSearchFAR();		
	AFSPOT_EndSearchNAR();		
	AFSPOT_EndSearchDIR();		
	AFSPOT_EndClimbTOP2();		
	AFSPOT_EndClimbPEAK();		

	AFSPOT_EndForcePEAK();		
	AFSPOT_EndDriveWAIT();		
}

//--------------------------------------------------------------------------------------------------------------------------
void AFSPOT_EnableProcess(BOOL OnOff)
{
	if (OnOff)	stAFSPOT.cur = PRE_FILTER_SEL;
	else		stAFSPOT.cur = END_CLEAR_PROC;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AFSPOT_IsProcessExecuting(void)
{
	return (stAFSPOT.cur==0)? FALSE : TRUE;
}

#endif	/* defined(__USE_AFZOOM_LENS__) && defined(__USE_X30X_TAMRON__) */

/*  FILE_END_HERE */
