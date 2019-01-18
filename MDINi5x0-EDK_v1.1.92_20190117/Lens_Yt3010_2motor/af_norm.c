// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

typedef	struct
{
	LONG	nBUFF[5];		// buffer of norm-FILT data
	SHORT	nDIFF[4];		// difference of norm-FILT data
	CHAR	nSIGN[4];		// diff-sign of norm-FILT data

	SHORT	nFOKS[5];		// buffer of focus position
	SHORT	nMOVE;			// distance of focus position

}	AFN_BUFF_PARAM, *PAFN_BUFF_PARAM;

typedef	struct
{
	LONG	nMIN;			// reference min-data of min-rate in norm
	LONG	nMAX;			// reference max-data of max-rate in norm
	LONG	nINC;			// calcurate min-data of min-rate in norm
	LONG	nDEC;			// calcurate max-data of max-rate in norm

	LONG	zMIN;			// reference min-data of min-zero in norm
	LONG	zMAX;			// reference max-data of max-zero in norm
	LONG	zINC;			// calcurate min-data of min-zero in norm
	LONG	zDEC;			// calcurate max-data of max-zero in norm

	SHORT	n_UP;			// threshold min-data of min-rate in norm
	SHORT	n_DN;			// threshold max-data of max-rate in norm
	SHORT	z_UP;			// threshold min-data of min-zero in norm
	SHORT	z_DN;			// threshold max-data of max-zero in norm

	LONG	nCHK;			// current status in norm

}	AFN_RATE_PARAM, *PAFN_RATE_PARAM;

typedef	struct
{
	LONG	nHIGH;			// peak-filt-val of HPFn in norm
	SHORT	nPEAK;			// peak-position of HPFn in norm
	SHORT	nDUMY;			// dummy

}	AFN_PEAK_PARAM, *PAFN_PEAK_PARAM;

typedef	struct
{
	LONG	YSUM;			// original YSUM data
	LONG	YPRV;			// previous YSUM data
	LONG	YREF;			// original YREF data
	LONG	GAIN;			// original GAIN data
	LONG	YCNT;			// original YCNT data
	LONG	FLTb;			// compensation threshold FILT data

	BYTE	nCHK;			// conditional min check
	BYTE	nRST;			// conditional rst check
	BYTE	nERR;			// conditional err check
	BYTE	nCNT;			// conditional cnt check

}	AFN_NORM_PARAM, *PAFN_NORM_PARAM;

typedef	struct
{
	LONG	FLTa;			// original FILT-A data
	LONG	FLTb;			// original FILT-B data
	LONG	FLTo;			// original FILTA+FILTB
	LONG	FLTn;			// normalized FILT data

}	AFN_DATA_PARAM, *PAFN_DATA_PARAM;

typedef	struct
{
	LONG	FLTn;			// normalized FILT data
	LONG	FLTd;			// current difference FILT data
	LONG	FLTx;			// previous difference FILT data
	LONG	FLTr;			// reference FILT data for rate

	LONG	nBUFF[5];		// buffer of norm-FILT data
	LONG	nDIFF[4];		// difference of norm-FILT data
	CHAR	nSIGN[4];		// diff-sign of norm-FILT data
	SHORT	nFOKS[5];		// buffer of focus position
	SHORT	nMOVE;			// distance of focus position

	LONG	nMIN;			// reference min-data of min-rate in norm
	LONG	nMAX;			// reference max-data of max-rate in norm
	LONG	nINC;			// calcurate min-data of min-rate in norm
	LONG	nDEC;			// calcurate max-data of max-rate in norm

	LONG	zMIN;			// reference min-data of min-zero in norm
	LONG	zMAX;			// reference max-data of max-zero in norm
	LONG	zINC;			// calcurate min-data of min-zero in norm
	LONG	zDEC;			// calcurate max-data of max-zero in norm

	SHORT	n_UP;			// threshold min-data of min-rate in norm
	SHORT	n_DN;			// threshold max-data of max-rate in norm
	SHORT	z_UP;			// threshold min-data of min-zero in norm
	SHORT	z_DN;			// threshold max-data of max-zero in norm

	LONG	nCHK;			// current status in norm

	LONG	nHIGH;			// peak-filt-val of HPFn in norm
	LONG	xHIGH;			// peak-filt-val of HPFx in norm

	SHORT	nPEAK;			// peak-position of HPFn in norm
	SHORT	xPEAK;			// peak-position of HPFx in norm

}	AFN_FILT_PARAM, *PAFN_FILT_PARAM;

typedef	struct
{
	BYTE	xMIN;			// adjust min-speed count
	BYTE	xSTD;			// adjust std-speed count
	BYTE	xMID;			// adjust mid-speed count
	BYTE	xMAX;			// adjust max-speed count
	BYTE	xFWD;			// adjust fwd-speed count
	BYTE	xBWD;			// adjust bwd-speed count
	BYTE	xTOP;			// adjust top-speed count
	BYTE	xDMY;			// dummy

}	AFN_STEP_COUNT, *PAFN_STEP_COUNT;

typedef	struct
{
	BYTE	nMIN;			// minimum-speed step
	BYTE	nSTD;			// standard-speed step
	BYTE	nMID;			// middle-speed step
	BYTE	nMAX;			// maximum-speed step

	BYTE	nFWD;			// largest-speed step
	BYTE	nBWD;			// reverse-speed step
	BYTE	nTOP;			// search-speed step

	BYTE	rCNT;			// ref count for speed-up
	BYTE	nCNT;			// cur count for speed-up

	BYTE	nSPD;			// current speed status
	BYTE	dMIN;			// dec-min speed flag
	BYTE	nCHK;			// max-speed flag

	AFN_STEP_COUNT	stDIR_I;
	AFN_STEP_COUNT	stDEC_I;
	AFN_STEP_COUNT	stDEC_D;

}	AFN_STEP_PARAM, *PAFN_STEP_PARAM;

typedef	struct
{
	BYTE	nSEL;			// AF control filter select
	BYTE	nDIR;			// AF control focus drive direction
	BYTE	eCNT;			// AF motor drive endpoint counter
	BYTE	xCNT;			// AF control buffer counter
	BYTE	vCNT;			// AF control shake counter

	BYTE	cur;			// AF control current progress step
	BYTE	prv;			// AF control previous progress step
	BYTE	nxt;			// AF control next progress step

	WORD	step;			// AF control motor drive step
	SHORT	goal;			// AF control motor drive target
	SHORT	peak;			// AF control motor peak position
	SHORT	fVAL;			// AF control focus position data
	SHORT	fMAX;			// AF control focus drive max range
	SHORT	fMIN;			// AF control focus drive min range
	SHORT	fMID;			// AF control focus middle value
	SHORT	fGAP;			// AF control focus range (FAR-NEAR)
	SHORT	fMOV;			// AF control focus moving diatance
	SHORT	zVAL;			// AF control zVAL position data
	SHORT	ZOOM;			// AF control zoom position data
	WORD	time;			// AF control execution time
	WORD	nDMY;			// dummy

	AFN_NORM_PARAM	stNORM_y;	// data normalize Y-info
	AFN_DATA_PARAM	stDATA_r;	// data normalization RPF
	AFN_DATA_PARAM	stDATA_l;	// data normalization LPF
	AFN_DATA_PARAM	stDATA_v;	// data normalization VPF
	AFN_DATA_PARAM	stDATA_b;	// data normalization BPF
	AFN_DATA_PARAM	stDATA_h;	// data normalization HPF
	AFN_FILT_PARAM	stOUTn_b;	// filter compesation BPF
	AFN_FILT_PARAM	stOUTn_h;	// filter compesation HPF
	AFN_STEP_PARAM	stSTEP_m;	// driving focus step

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
	SHORT	n_UP;			// threshold min-data of min-rate
	SHORT	n_DN;			// threshold max-data of max-rate
	SHORT	z_UP;			// threshold min-data of min-zero
	SHORT	z_DN;			// threshold max-data of max-zero

}	AFN_TUNE_RATIO, *PAFN_TUNE_RATIO;

typedef	struct
{
	BYTE	r_MIN;			// ref-count of MIN-SPD up/dn
	BYTE	r_STD;			// ref-count of STD-SPD up/dn
	BYTE	r_MID;			// ref-count of MID-SPD up/dn
	BYTE	r_MAX;			// ref-count of MAX-SPD up/dn

}	AFN_TUNE_COUNT, *PAFN_TUNE_COUNT;

typedef	struct
{
	LONG	n_GAIN;			// normalized reference GAIN
	LONG	n_YSUM;			// normalized reference YSUM
	LONG	n_FLTr;			// normalized reference FLTr

	LONG	c_FLTb;			// conditional reference BPFn
	LONG	c_FLTh;			// conditional reference HPFn
	LONG	c_GMIN;			// conditional reference GMIN
	LONG	c_GMAX;			// conditional reference GMAX

	SHORT	h_ZOOM;			// hysteresis step of ZOOM
	SHORT	h_FOKS;			// hysteresis step of FOKS

	AFN_TUNE_RATIO	stPRV_b;		// shake-prv rate of BPF
	AFN_TUNE_RATIO	stPRV_h;		// shake-prv rate of HPF

	AFN_TUNE_RATIO	stDIR_b;		// direction rate of BPF
	AFN_TUNE_RATIO	stDIR_h;		// direction rate of HPF

	AFN_TUNE_RATIO	stDEC_b;		// climb-dec rate of BPF
	AFN_TUNE_RATIO	stDEC_h;		// climb-dec rate of HPF

	AFN_TUNE_RATIO	stTOP_b;		// fine-peak rate of BPF
	AFN_TUNE_RATIO	stTOP_h;		// fine-peak rate of HPF

}	AFN_TUNE_PARAM, *PAFN_TUNE_PARAM;

#define		MIN3(a,b,c)				(MIN(MIN(a,b),c))
#define		MAX3(a,b,c)				(MAX(MAX(a,b),c))

#define		MIN4(a,b,c,d)			(MIN(MIN(a,b),MIN(c,d)))
#define		MAX4(a,b,c,d)			(MAX(MAX(a,b),MAX(c,d)))

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static AFN_PROC_PARAM stAFNORM;
static AFN_TUNE_PARAM stAFNCTL = {
//	n_GAIN  n_YSUM  n_FLTr  c_FLTb  c_FLTh  c_GMIN  c_GMAX
	     0,      0,      0,    500,   2048,   8191,  65535,

//	h_ZOOM  h_FOKS
	     0,      0,

//   n_UP    n_DN    z_UP    z_DN	for shake-prv //stPRV_b
	{  40,    -20,     30,     -5},	// BPF
	{  40,    -20,     30,     -5},	// HPF

//   n_UP    n_DN    z_UP    z_DN	for direction //stDIR_b
	{  49,    -49,      1,     -5},	// BPF
	{  49,    -49,      1,     -5},	// HPF

//   n_UP    n_DN    z_UP    z_DN	for climb-dec //stDEC_b
	{  51,    -51,      1,     -2},	// BPF
	{  51,    -51,      1,     -5},	// HPF

//   n_UP    n_DN    z_UP    z_DN	for find-peak //stTOP_b
	{  20,    -20,      1,     -1},	// BPF
	{  20,     -5,      1,     -1}	// HPF
};

static BYTE eID, xID = 0;	// for debug print

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
extern VBYTE count_16ms;

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

	stAFNORM.stDATA_r.FLTa = (pCTRL->stFILT.RPFa)>>0;
	stAFNORM.stDATA_v.FLTa = (pCTRL->stFILT.VPFa)>>0;
	stAFNORM.stDATA_b.FLTa = (pCTRL->stFILT.BPFa)>>0;
	stAFNORM.stDATA_r.FLTb = (pCTRL->stFILT.RPFb)<<2;
	stAFNORM.stDATA_v.FLTb = (pCTRL->stFILT.VPFb)<<2;
	stAFNORM.stDATA_b.FLTb = (pCTRL->stFILT.BPFb)<<2;

	stAFNORM.stNORM_y.YCNT = (pCTRL->stFILT.CNTa)>>0;
	stAFNORM.stDATA_h.FLTa = (pCTRL->stFILT.HPFa)>>0;
	stAFNORM.stDATA_h.FLTb = (pCTRL->stFILT.HPFb)<<2;

	stAFNORM.stDATA_r.FLTo = stAFNORM.stDATA_r.FLTa;
	stAFNORM.stDATA_b.FLTo = stAFNORM.stDATA_b.FLTa+stAFNORM.stDATA_v.FLTa;
	stAFNORM.stDATA_h.FLTo = stAFNORM.stDATA_h.FLTa+stAFNORM.stDATA_v.FLTa;

	stAFNORM.stNORM_y.YPRV = (stAFNORM.stNORM_y.YSUM);
	stAFNORM.stNORM_y.YSUM = (pCTRL->stFILT.YSUM)>>0;
	stAFNORM.stNORM_y.GAIN = (pCTRL->stFILT.GAIN)>>0;
	stAFNORM.stNORM_y.YREF = (AECTRL_GetBrightness());

	// get normalized FILT data
	if (stAFNCTL.n_GAIN==0) stAFNCTL.n_GAIN = stAFNORM.stNORM_y.GAIN;
		nGAIN = ((FLOAT)stAFNCTL.n_GAIN)/stAFNORM.stNORM_y.GAIN;

	if (stAFNCTL.n_YSUM==0) stAFNCTL.n_YSUM = stAFNORM.stNORM_y.YSUM;
		nYSUM = ((FLOAT)stAFNCTL.n_YSUM)/stAFNORM.stNORM_y.YSUM;

	stAFNORM.stDATA_h.FLTo += stAFNORM.stDATA_h.FLTb+stAFNORM.stDATA_v.FLTb;

	stAFNORM.stDATA_r.FLTn = (LONG)(nGAIN*nYSUM*stAFNORM.stDATA_r.FLTo+0.5);
	stAFNORM.stDATA_b.FLTn = (LONG)(nGAIN*nYSUM*stAFNORM.stDATA_b.FLTo+0.5);
	stAFNORM.stDATA_h.FLTn = (LONG)(nGAIN*nYSUM*stAFNORM.stDATA_h.FLTo+0.5);

	// get compensation FILT data
	nHPFr = (stAFNORM.stNORM_y.nCHK)? ((FLOAT)stAFNCTL.n_FLTr)/stAFNORM.stDATA_r.FLTn : 1.0f;

	stAFNORM.stOUTn_b.FLTn = (LONG)(nHPFr*stAFNORM.stDATA_b.FLTn+0.5);
	stAFNORM.stOUTn_h.FLTn = (LONG)(nHPFr*stAFNORM.stDATA_h.FLTn+0.5);

	// get current position
	stAFNORM.nDIR = LENS_GetDirectionFOKS();
	stAFNORM.fVAL = LENS_GetCurPositionFOKS();
	stAFNORM.zVAL = LENS_GetCurPositionZOOM();
	stAFNORM.ZOOM = LENS_GetRelPositionZOOM();
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFNORM_IsDriveEnd(void)
{
	if (stAFNORM.nDIR==fNEAR && stAFNORM.fVAL<=stAFNORM.fMIN) {
		//UARTprintf("fNEAR end... stAFNORM.fVAL : %d / stAFNORM.fMIN : %d\r\n", stAFNORM.fVAL, stAFNORM.fMIN);
		return TRUE;
	}
	if (stAFNORM.nDIR== fFAR && stAFNORM.fVAL>=stAFNORM.fMAX) {
		//UARTprintf("fFAR end... stAFNORM.fVAL : %d / stAFNORM.fMAX : %d\r\n", stAFNORM.fVAL, stAFNORM.fMAX);
		return TRUE;
	}
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
	WORD max = (WORD)stAFNORM.stSTEP_m.nBWD;
	WORD min = (WORD)stAFNORM.stSTEP_m.nMIN;

	switch (nID) {
		case DRV_STEP_MIN:	stAFNORM.step = (WORD)stAFNORM.stSTEP_m.nMIN; break;
		case DRV_STEP_STD:	stAFNORM.step = (WORD)stAFNORM.stSTEP_m.nSTD; break;
		case DRV_STEP_MID:	stAFNORM.step = (WORD)stAFNORM.stSTEP_m.nMID; break;
		case DRV_STEP_MAX:	stAFNORM.step = (WORD)stAFNORM.stSTEP_m.nMAX; break;

		case DRV_STEP_FWD:	stAFNORM.step = (WORD)stAFNORM.stSTEP_m.nFWD; break;
		case DRV_STEP_BWD:	stAFNORM.step = (WORD)stAFNORM.stSTEP_m.nBWD; break;
		case DRV_STEP_TOP:	stAFNORM.step = (WORD)stAFNORM.stSTEP_m.nTOP; break;

		case DRV_STEP_DIV:	stAFNORM.step = MIN(MAX((gap+3)/4,min), max); break;
		case DRV_STEP_REM:	stAFNORM.step = (quo)? stAFNORM.step : rem; break;
	}

	if ((nID&0xf0)==DRV_STEP_MIN) stAFNORM.stSTEP_m.nSPD = nID;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_GetDriveRATE(BYTE nID)
{
	PBYTE pTUNEb = NULL, pTUNEh = NULL;

	switch (nID) {
		case DRV_SEARCH_FAR: case PRE_SEARCH_FAR:
		case DRV_SEARCH_NAR: case PRE_SEARCH_NAR:
			pTUNEb = (PBYTE)&stAFNCTL.stPRV_b;	pTUNEh = (PBYTE)&stAFNCTL.stPRV_h;	break;

		case DRV_SEARCH_DIR: case PRE_SEARCH_DIR:
			pTUNEb = (PBYTE)&stAFNCTL.stDIR_b;	pTUNEh = (PBYTE)&stAFNCTL.stDIR_h;	break;

		case DRV_CLIMB_TOP2: case PRE_CLIMB_TOP2:
			pTUNEb = (PBYTE)&stAFNCTL.stDEC_b;	pTUNEh = (PBYTE)&stAFNCTL.stDEC_h;	break;

		case DRV_CLIMB_PEAK: case PRE_CLIMB_PEAK:
			pTUNEb = (PBYTE)&stAFNCTL.stTOP_b;	pTUNEh = (PBYTE)&stAFNCTL.stTOP_h;	break;
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

	stAFNORM.goal = MIN(stAFNORM.goal, stAFNORM.fMAX);	// check range
	stAFNORM.goal = MAX(stAFNORM.goal, stAFNORM.fMIN);

	LENS_SetDriveFOKS(stAFNORM.goal);	// start motor drive
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_SetDrivePEAK(void)
{
	stAFNORM.goal = stAFNORM.peak;

	stAFNORM.goal = MIN(stAFNORM.goal, stAFNORM.fMAX);	// check range
	stAFNORM.goal = MAX(stAFNORM.goal, stAFNORM.fMIN);

	LENS_SetDriveFOKS(stAFNORM.goal);	// start motor drive
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_ClrDrvCountSPD(PAFN_STEP_COUNT pCNT)
{
	SHORT nFWD = LENS_GetAFDriveSTEP(DRV_STEP_FWD);
	LONG  GAIN = stAFNORM.stNORM_y.GAIN, KNEE = MIN(GAIN,8192);

	nFWD -= (SHORT)(6.0f*(FLOAT)GAIN/16384+0.5);
	stAFNORM.stSTEP_m.nFWD = MAX(nFWD,stAFNORM.stSTEP_m.nSTD*4);

	switch (stAFNORM.stSTEP_m.nSPD) {
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

	AFNORM_ClrDrvCountSPD(pCNT);	// clear counter

	switch (stAFNORM.stSTEP_m.nSPD) {
		case DRV_STEP_MIN:		AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;

		case DRV_STEP_STD:		if (pCNT->xSTD<6) pCNT->xSTD++;
			if (pCNT->xSTD==6)	AFNORM_GetDriveSTEP(DRV_STEP_MID);
			else				AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;

		case DRV_STEP_MID:		if (pCNT->xMID<3) pCNT->xMID++;
			if (pCNT->xMID==3)	AFNORM_GetDriveSTEP(DRV_STEP_MAX);
			else				AFNORM_GetDriveSTEP(DRV_STEP_MID);	break;

		case DRV_STEP_MAX:		AFNORM_GetDriveSTEP(DRV_STEP_MAX);	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DecDrvIncSPD_GX(BYTE nID)
{
	PAFN_STEP_COUNT pCNT = &stAFNORM.stSTEP_m.stDEC_I;

	AFNORM_ClrDrvCountSPD(pCNT);	// clear counter
	memset(&stAFNORM.stSTEP_m.stDEC_D, 0, sizeof(AFN_STEP_COUNT));

	if (stAFNORM.stSTEP_m.nSPD==DRV_STEP_MAX) stAFNORM.stSTEP_m.nCHK = 1;

	if (stAFNORM.stSTEP_m.nCHK) {
		switch (stAFNORM.stSTEP_m.nSPD) {
			case DRV_STEP_MIN:		if (pCNT->xMIN<6) pCNT->xMIN++;
				if (pCNT->xMIN==6)	AFNORM_GetDriveSTEP(DRV_STEP_STD);
				else				AFNORM_GetDriveSTEP(DRV_STEP_MIN);	break;

			case DRV_STEP_STD:		if (pCNT->xSTD<4) pCNT->xSTD++;
				if (pCNT->xSTD==4)	AFNORM_GetDriveSTEP(DRV_STEP_MID);
				else				AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		AFNORM_GetDriveSTEP(DRV_STEP_MID);	break;

			case DRV_STEP_MAX:		AFNORM_GetDriveSTEP(DRV_STEP_MID);	break;
		}
	} else {
		switch (stAFNORM.stSTEP_m.nSPD) {
			case DRV_STEP_MIN:		AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_STD:		if (pCNT->xSTD<4) pCNT->xSTD++;
				if (pCNT->xSTD==4)	AFNORM_GetDriveSTEP(DRV_STEP_MID);
				else				AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		AFNORM_GetDriveSTEP(DRV_STEP_MAX);	break;

			case DRV_STEP_MAX:		AFNORM_GetDriveSTEP(DRV_STEP_MAX);	break;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DecDrvIncSPD_BI(BYTE nID)
{
	PAFN_STEP_COUNT pCNT = &stAFNORM.stSTEP_m.stDEC_I;

	AFNORM_ClrDrvCountSPD(pCNT);	// clear counter
	memset(&stAFNORM.stSTEP_m.stDEC_D, 0, sizeof(AFN_STEP_COUNT));

	switch (stAFNORM.stSTEP_m.nSPD) {
		case DRV_STEP_MIN:		if (pCNT->xMIN<4) pCNT->xMIN++;
			if (pCNT->xMIN==4)	AFNORM_GetDriveSTEP(DRV_STEP_STD);
			else				AFNORM_GetDriveSTEP(DRV_STEP_MIN);	break;

		case DRV_STEP_STD:		if (pCNT->xSTD<3) pCNT->xSTD++;
			if (pCNT->xSTD==3)	AFNORM_GetDriveSTEP(DRV_STEP_MID);
			else				AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;

		case DRV_STEP_MID:		if (pCNT->xMID<2) pCNT->xMID++;
			if (pCNT->xMID==2)	AFNORM_GetDriveSTEP(DRV_STEP_MAX);
			else				AFNORM_GetDriveSTEP(DRV_STEP_MID);	break;

		case DRV_STEP_MAX:		AFNORM_GetDriveSTEP(DRV_STEP_MAX);	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DecDrvDecSPD_GD(BYTE nID)
{
	PAFN_STEP_COUNT pCNT = &stAFNORM.stSTEP_m.stDEC_D;

	AFNORM_ClrDrvCountSPD(pCNT);	// clear counter
	memset(&stAFNORM.stSTEP_m.stDEC_I, 0, sizeof(AFN_STEP_COUNT));

	if (stAFNORM.stSTEP_m.nSPD==DRV_STEP_MAX) stAFNORM.stSTEP_m.nCHK = 1;
	//UARTprintf("stAFNORM.stSTEP_m.nCHK : %d / nSPD : 0x%0x\r\n", stAFNORM.stSTEP_m.nCHK, stAFNORM.stSTEP_m.nSPD);

	if (stAFNORM.stSTEP_m.nCHK) {
		switch (stAFNORM.stSTEP_m.nSPD) {
			case DRV_STEP_MIN:		AFNORM_GetDriveSTEP(DRV_STEP_MIN);	break;

			case DRV_STEP_STD:		if (pCNT->xSTD<2) pCNT->xSTD++;
				if (pCNT->xSTD==2)	AFNORM_GetDriveSTEP(DRV_STEP_MIN);
				else				AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MAX:		AFNORM_GetDriveSTEP(DRV_STEP_STD);	break;
		}
	} else {
		switch (stAFNORM.stSTEP_m.nSPD) {
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

	switch (stAFNORM.stSTEP_m.nSPD) {
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
	// clear PEAK-filt info
	stAFNORM.stOUTn_h.nHIGH = stAFNORM.stOUTn_h.FLTn;
	stAFNORM.stOUTn_b.nHIGH = stAFNORM.stOUTn_b.FLTn;

	// clear PEAK-foks info
	stAFNORM.stOUTn_h.nPEAK = stAFNORM.fVAL;
	stAFNORM.stOUTn_h.xPEAK = stAFNORM.fVAL;
	stAFNORM.stOUTn_b.nPEAK = stAFNORM.fVAL;
	stAFNORM.stOUTn_b.xPEAK = stAFNORM.fVAL;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_ClrJudgeINFO(void)
{
	// clear RATE-flag info
	stAFNORM.stOUTn_h.nCHK = 0;
	stAFNORM.stOUTn_b.nCHK = 0;

	stAFNORM.stSTEP_m.nCNT = 0;		// clear speed count
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_ClrJudgeBUFF(void)
{
	memset(stAFNORM.stOUTn_h.nBUFF, 0, sizeof(AFN_BUFF_PARAM));
	memset(stAFNORM.stOUTn_b.nBUFF, 0, sizeof(AFN_BUFF_PARAM));

	stAFNORM.xCNT = 0;	stAFNORM.vCNT &= 0xc0;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_MovShiftBUFF(BYTE nID)
{
	BYTE i;

	for (i=1; i<nID; i++) {
		stAFNORM.stOUTn_h.nBUFF[i-1] = stAFNORM.stOUTn_h.nBUFF[i];
		stAFNORM.stOUTn_b.nBUFF[i-1] = stAFNORM.stOUTn_b.nBUFF[i];

		stAFNORM.stOUTn_h.nFOKS[i-1] = stAFNORM.stOUTn_h.nFOKS[i];
		stAFNORM.stOUTn_b.nFOKS[i-1] = stAFNORM.stOUTn_b.nFOKS[i];
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_GetPrvJudgeBUFF(PAFN_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap; BYTE i; PLONG pBUFF = (PLONG)pFILT->nBUFF;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFNORM.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	pBUFF[nID] = pFILT->FLTn; pFILT->nFOKS[nID] = stAFNORM.fVAL;
	if (nID<2) return;

	for (i=0; i<nID; i++) {
		pFILT->nDIFF[i] = (LONG)(pBUFF[i+1]-pBUFF[i]);

		if		(pFILT->nDIFF[i]>0)	pFILT->nSIGN[i] =  1;
		else if (pFILT->nDIFF[i]<0)	pFILT->nSIGN[i] = -1;
		else						pFILT->nSIGN[i] =  0;
	}

	pFILT->nMOVE = MAX3(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2])
				 - MIN3(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2]);

	stAFNORM.fMOV = pFILT->nMOVE/nID;	// average driving distance

	if (pFILT->nSIGN[0]==pFILT->nSIGN[1]) {
		gap = (pFILT->nMOVE)? (13.0f*nID)/pFILT->nMOVE : 1.0f;
	} else {
		gap = (1.0f); pFILT->nMIN = pFILT->nMAX = pFILT->FLTn;
	}

	if (ABS(pFILT->nDIFF[0])<30 || ABS(pFILT->nDIFF[1])<30) { // mimitest
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

	//UARTprintf(" pFILT->FLTn:%d/pFILT->nMIN:%d/pFILT->nMAX:%d/pFILT->FLTr:%d/pFILT->zMIN:%d/pFILT->zMAX:%d\r\n", pFILT->FLTn,pFILT->nMIN,pFILT->nMAX,pFILT->FLTr,pFILT->zMIN,pFILT->zMAX);
	//UARTprintf("\r\nnINC : %d & n_UP : %d /", pFILT->nINC, pFILT->n_UP);
	//UARTprintf("nDEC : %d & n_DN : %d /", pFILT->nDEC, pFILT->n_DN);
	//UARTprintf("zINC : %d & z_UP : %d /", pFILT->zINC, pFILT->z_UP);
	//UARTprintf("zDEC : %d & z_DN : %d \r\n", pFILT->zDEC, pFILT->z_DN);
	
	if (pFILT->nINC>=(LONG)pFILT->n_UP)	pFILT->nCHK |= ( GOOD_INC|G_INC_OK); // 50 /50
	else								pFILT->nCHK &= (~GOOD_INC);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN)	pFILT->nCHK |= ( GOOD_DEC); // -30 / -30
	else								pFILT->nCHK &= (~GOOD_DEC);

	if (pFILT->zINC>=(LONG)pFILT->z_UP)	pFILT->nCHK |= ( ZERO_INC); // 50 /50
	else								pFILT->nCHK &= (~ZERO_INC);

	if (pFILT->zDEC<=(LONG)pFILT->z_DN)	pFILT->nCHK |= ( ZERO_DEC); // -5 /-5
	else								pFILT->nCHK &= (~ZERO_DEC);

	pFILT->FLTx = pFILT->FLTd = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_GetDirJudgeBUFF(PAFN_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap;	PLONG pBUFF = (PLONG)pFILT->nBUFF;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFNORM.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	pBUFF[nID] = pFILT->FLTn; pFILT->nFOKS[nID] = stAFNORM.fVAL;
	if (nID<3) return;

	pFILT->nMOVE = MAX4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3])
				 - MIN4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3]);

	stAFNORM.fMOV = pFILT->nMOVE/nID;	// average driving distance

	gap = MAX((13.0f*nID)/pFILT->nMOVE, 1.0f);

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
static void AFNORM_InvDirJudgeBUFF(PAFN_FILT_PARAM pFILT, BYTE nID)
{
	PLONG pBUFF = (PLONG)pFILT->nBUFF;
	FLOAT rate, gap = 13.0f/stAFNORM.step;

	rate = (pBUFF[2])? ((FLOAT)pFILT->FLTn/pBUFF[2]-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate*gap>0)? 0 : rate*gap-0.5);

	if (pFILT->nDEC<=-1)	pFILT->nCHK |= ( ZERO_DEC);
	else					pFILT->nCHK &= (~ZERO_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_GetDecJudgeBUFF(PAFN_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap;	PLONG pBUFF = (PLONG)pFILT->nBUFF;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFNORM.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	pBUFF[nID] = pFILT->FLTn; pFILT->nFOKS[nID] = stAFNORM.fVAL;
	if (nID<3) return;

	pFILT->nMOVE = MAX4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3])
				 - MIN4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3]);

	stAFNORM.fMOV = pFILT->nMOVE/nID;	// average driving distance

	gap = (13.0f*nID)/pFILT->nMOVE;

	pFILT->zMAX = MAX4(pBUFF[0],pBUFF[1],pBUFF[2],pBUFF[3]);
	pFILT->zMIN = (pBUFF[0]+pBUFF[1]+pBUFF[2]+pBUFF[3]+2)/4;

	pFILT->FLTr = (pFILT->FLTn);
//	pFILT->FLTx = MAX(pFILT->FLTd-MAX((LONG)(5.0f/gap+0.5),5),5);
	pFILT->FLTx = pFILT->FLTd-(LONG)(pFILT->FLTd*0.02f);

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

	if (pFILT->nINC>=(LONG)pFILT->n_UP)	pFILT->nCHK |= ( HIGH_INC|G_INC_OK);
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
static void AFNORM_InvDecJudgeBUFF(PAFN_FILT_PARAM pFILT, BYTE nID)
{
	PLONG pBUFF = (PLONG)pFILT->nBUFF;
	FLOAT rate, gap = 13.0f/stAFNORM.step;

	rate = (pBUFF[2])? ((FLOAT)pFILT->FLTn/pBUFF[2]-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate*gap>0)? rate*gap : rate*gap-0.5);

	if (pFILT->nDEC<=20)	pFILT->nCHK |= ( ZERO_DEC);
	else					pFILT->nCHK &= (~ZERO_DEC);
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

	if (pFILT->nINC>=(LONG)pFILT->n_UP)	pFILT->nCHK |= ( GOOD_INC);
	else								pFILT->nCHK &= (~GOOD_INC);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN)	pFILT->nCHK |= ( GOOD_DEC);
	else								pFILT->nCHK &= (~GOOD_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_InvTopJudgeBUFF(PAFN_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate;

	rate = (pFILT->nMAX)? ((FLOAT)pFILT->FLTn/pFILT->nMAX-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate>0)? rate+0.5 : rate-0.5);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN)	pFILT->nCHK |= ( ZERO_DEC);
	else								pFILT->nCHK &= (~ZERO_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_InvEndJudgeBUFF(PAFN_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate = ((FLOAT)pFILT->xHIGH/pFILT->FLTn-1);

	rate *= ((nID==USE_HPF)? 500 : 1000);
	pFILT->nDEC = (LONG)((rate<0)? rate-0.5 : rate+0.5);

	if (ABS(pFILT->nDEC)<100)	pFILT->nCHK |= ( GOOD_END);
	else						pFILT->nCHK &= (~GOOD_END);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_GetJudge_FAR(void)
{
	PAFN_FILT_PARAM pFILTx = &stAFNORM.stOUTn_b;
	PAFN_FILT_PARAM pFILTh = &stAFNORM.stOUTn_h;
	PAFN_FILT_PARAM pFILTb = &stAFNORM.stOUTn_b;

	if (stAFNORM.vCNT&0x80) pFILTx = &stAFNORM.stOUTn_h;

	AFNORM_GetDriveRATE(DRV_SEARCH_FAR);	// get judge RATE

	AFNORM_GetPrvJudgeBUFF(&stAFNORM.stOUTn_h, stAFNORM.xCNT);
	AFNORM_GetPrvJudgeBUFF(&stAFNORM.stOUTn_b, stAFNORM.xCNT);

	if (AFNORM_IsDriveEnd()==FALSE) {
		stAFNORM.xCNT++;	if (stAFNORM.xCNT<3) return;
		stAFNORM.vCNT++;
	}

	if (AFNORM_IsDriveEnd()==TRUE) {
		stAFNORM.eCNT++;				AFNORM_ClrJudgeRATE();
		stAFNORM.cur = INV_SEARCH_FAR;	stAFNORM.nxt = PRE_SEARCH_DIR;	xID = 0xa1;
		//UARTprintf("0xa1\r\n");
	} else if (pFILTx->nCHK&GOOD_DEC && (pFILTb->nCHK&ZERO_INC)==0) {
		stAFNORM.cur = INV_SEARCH_FAR;	stAFNORM.nxt = END_SEARCH_FAR;	xID = 0xa2;
	} else if (pFILTx->nCHK&GOOD_INC) {
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;	xID = 0xa3;
	} else if ((stAFNORM.vCNT&15)==5) {
		stAFNORM.cur = INV_SEARCH_FAR;	stAFNORM.nxt = PRE_SEARCH_DIR;	xID = 0xa4;
		//UARTprintf("0xa6\r\n");
	} else {		stAFNORM.xCNT--;			AFNORM_MovShiftBUFF(3);
		stAFNORM.cur = DRV_SEARCH_FAR;	stAFNORM.nxt = DRV_SEARCH_FAR;	xID = 0xa5;
	}

	if ((stAFNORM.vCNT&15)==5 && pFILTb->nCHK&GOOD_DEC) {
		stAFNORM.cur = INV_SEARCH_FAR;	stAFNORM.nxt = END_SEARCH_FAR;	xID = 0xa6;
	}
	if ((stAFNORM.vCNT&15)==5 && pFILTb->nCHK&GOOD_INC) {
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;	xID = 0xa7;
	}

	if (pFILTb->nCHK&ZERO_DEC) stAFNORM.vCNT |= 0x40;
	if (pFILTh->nCHK&GOOD_DEC) stAFNORM.nSEL = USE_HPF;
	if (pFILTh->nCHK&GOOD_INC) stAFNORM.nSEL = USE_HPF;

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

	AFNORM_GetDriveRATE(DRV_SEARCH_NAR);	// get judge RATE //stAFNORM.stOUTn_b.n_UP stAFNORM.stOUTn_h.n_UP

	AFNORM_GetPrvJudgeBUFF(&stAFNORM.stOUTn_h, stAFNORM.xCNT);
	AFNORM_GetPrvJudgeBUFF(&stAFNORM.stOUTn_b, stAFNORM.xCNT);

	if (AFNORM_IsDriveEnd()==FALSE) {
		stAFNORM.xCNT++;	if (stAFNORM.xCNT<3) return;
		stAFNORM.vCNT++;
	}

	if (AFNORM_IsDriveEnd()==TRUE) {
				stAFNORM.eCNT++;			AFNORM_ClrJudgeRATE();
		stAFNORM.cur = INV_SEARCH_NAR;	stAFNORM.nxt = PRE_SEARCH_DIR;	xID = 0xb1;
	} else if (pFILTx->nCHK&GOOD_DEC && (pFILTb->nCHK&ZERO_INC)==0) {
		stAFNORM.cur = INV_SEARCH_NAR;	stAFNORM.nxt = DRV_FORCE_PEAK;	xID = 0xb2;
	} else if (pFILTx->nCHK&ZERO_INC || pFILTb->nCHK&ZERO_INC) {
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;	xID = 0xb3;
	} else if ((stAFNORM.vCNT&15)==5) {
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;	xID = 0xb4;
	} else {		stAFNORM.xCNT--;			AFNORM_MovShiftBUFF(3);
		stAFNORM.cur = DRV_SEARCH_NAR;	stAFNORM.nxt = DRV_SEARCH_NAR;	xID = 0xb5;
	}

	if (pFILTb->nCHK&ZERO_DEC) stAFNORM.vCNT |= 0x40;
	if (pFILTh->nCHK&GOOD_DEC) stAFNORM.nSEL = USE_HPF;
	if (pFILTh->nCHK&GOOD_INC) stAFNORM.nSEL = USE_HPF;

	if (stAFNORM.nxt==PRE_SEARCH_DIR) AFNORM_ClrJudgeBUFF();

	if (stAFNORM.nxt==DRV_FORCE_PEAK && (stAFNORM.vCNT&0x40)==0) {
		stAFNORM.cur = INV_SEARCH_NAR;	stAFNORM.nxt = DRV_CLIMB_TOP2;	xID = 0xb6;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_GetJudge_DIR(void)
{
	PAFN_FILT_PARAM pFILTh = &stAFNORM.stOUTn_h;
	PAFN_FILT_PARAM pFILTb = &stAFNORM.stOUTn_b;

	AFNORM_GetDriveRATE(DRV_SEARCH_DIR);	// get judge RATE

	AFNORM_GetDirJudgeBUFF(&stAFNORM.stOUTn_h, stAFNORM.xCNT);
	AFNORM_GetDirJudgeBUFF(&stAFNORM.stOUTn_b, stAFNORM.xCNT);

	if (AFNORM_IsDriveEnd()==FALSE) {
		stAFNORM.xCNT++; if (stAFNORM.xCNT<4) return;
	}

	if (stAFNORM.xCNT) stAFNORM.xCNT--;	AFNORM_MovShiftBUFF(4);

	if (AFNORM_IsDriveEnd()==TRUE) {
				stAFNORM.eCNT++;			AFNORM_ClrJudgeRATE();
		stAFNORM.cur = INV_SEARCH_DIR;	stAFNORM.nxt = DRV_SEARCH_DIR;	xID = 0xc1;
	} else if (pFILTb->nCHK&GOOD_DEC) {
			AFNORM_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFNORM.cur = INV_SEARCH_DIR;	stAFNORM.nxt = PRE_CLIMB_TOP2;	xID = 0xc2;
	} else if (pFILTb->nCHK&GOOD_INC) {
			AFNORM_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;	xID = 0xc3;
	} else if (pFILTh->nCHK&GOOD_DEC) {
			AFNORM_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFNORM.cur = INV_SEARCH_DIR;	stAFNORM.nxt = PRE_CLIMB_TOP2;	xID = 0xc4;
	} else if (pFILTh->nCHK&GOOD_INC) {
			AFNORM_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;	xID = 0xc5;
	} else {
			AFNORM_DirDrvIncSPD_BI(DRV_SEARCH_DIR);
		stAFNORM.cur = DRV_SEARCH_DIR;	stAFNORM.nxt = DRV_SEARCH_DIR;	xID = 0xc6;
	}

	if (pFILTh->nCHK&GOOD_DEC) stAFNORM.nSEL = USE_HPF;
	if (pFILTh->nCHK&GOOD_INC) stAFNORM.nSEL = USE_HPF;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_InvJudge_DIR(void)
{
	PAFN_FILT_PARAM pFILTx = &stAFNORM.stOUTn_b;
	PAFN_FILT_PARAM pFILTb = &stAFNORM.stOUTn_b;

	if (stAFNORM.nSEL==USE_HPF) pFILTx = &stAFNORM.stOUTn_h;

	if (stAFNORM.nxt!=PRE_CLIMB_TOP2) AFNORM_ClrJudgeBUFF();
	if (stAFNORM.nxt!=PRE_CLIMB_TOP2) return;

	AFNORM_GetDriveRATE(DRV_SEARCH_DIR);	// get judge RATE

	AFNORM_InvDirJudgeBUFF(&stAFNORM.stOUTn_h, stAFNORM.xCNT);
	AFNORM_InvDirJudgeBUFF(&stAFNORM.stOUTn_b, stAFNORM.xCNT);

	if ((pFILTb->nCHK&ZERO_DEC)==0 || (pFILTx->nCHK&ZERO_DEC)==0) {
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

	AFNORM_GetDriveRATE(DRV_CLIMB_TOP2);	// get judge RATE

	AFNORM_GetDecJudgeBUFF(&stAFNORM.stOUTn_h, stAFNORM.xCNT);
	AFNORM_GetDecJudgeBUFF(&stAFNORM.stOUTn_b, stAFNORM.xCNT);

	if (AFNORM_IsDriveEnd()==FALSE) {
		stAFNORM.xCNT++; if (stAFNORM.xCNT<4) return;
	}

	if (stAFNORM.xCNT) stAFNORM.xCNT--;	AFNORM_MovShiftBUFF(4);

	if (AFNORM_IsDriveEnd()==TRUE) {
				stAFNORM.eCNT++;			AFNORM_ClrJudgeRATE();
		stAFNORM.cur = INV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;	xID = 0xd1;
	} else if (pFILTh->nCHK&GOOD_DEC && pFILTb->nCHK&GOOD_DEC) {
		stAFNORM.cur = INV_CLIMB_TOP2;	stAFNORM.nxt = PRE_CLIMB_PEAK;	xID = 0xd2;
	} else if (pFILTb->nCHK&HIGH_INC && pFILTb->nCHK&RATE_DEC) {
			AFNORM_DecDrvDecSPD_GD(DRV_CLIMB_TOP2);
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;	xID = 0xd3;
	} else if (pFILTb->nCHK&HIGH_INC) {
			AFNORM_DecDrvIncSPD_GX(DRV_CLIMB_TOP2);
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;	xID = 0xd4;
	}
/*	else if (pFILTh->nCHK&GOOD_INC && pFILTb->nCHK&RATE_DEC) {
			AFNORM_DecDrvDecSPD_GD(DRV_CLIMB_TOP2);
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;	xID = 0xd3;
	}
	else if (pFILTb->nCHK&GOOD_INC || pFILTx->nCHK&GOOD_INC) {
			AFNORM_DecDrvIncSPD_GX(DRV_CLIMB_TOP2);
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;	xID = 0xd4;
	}
*/	else if (pFILTh->nCHK&GOOD_DEC || pFILTh->nCHK&HIGH_DEC) {
			AFNORM_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;	xID = 0xd5;
	} else {
			AFNORM_DecDrvIncSPD_BI(DRV_CLIMB_TOP2);
		stAFNORM.cur = DRV_CLIMB_TOP2;	stAFNORM.nxt = DRV_CLIMB_TOP2;	xID = 0xd6;
	}

	if (pFILTh->nCHK&GOOD_INC || pFILTh->nCHK&HIGH_INC) stAFNORM.nSEL = USE_HPF;
	if (pFILTh->nCHK&GOOD_DEC || pFILTh->nCHK&HIGH_DEC) stAFNORM.nSEL = USE_HPF;

	if (pFILTb->nCHK&R_DEC_OK && pFILTx->nCHK&ZERO_DEC && pFILTb->nCHK&ZERO_DEC) {
		stAFNORM.cur = INV_CLIMB_TOP2;	stAFNORM.nxt = PRE_CLIMB_PEAK;	xID = 0xd7;
	}
/*
	if (pFILTh->nCHK&R_DEC_OK && pFILTh->nCHK&GOOD_DEC) {
		stAFNORM.cur = INV_CLIMB_TOP2;	stAFNORM.nxt = PRE_CLIMB_PEAK;	xID = 0xd8;
	}

	if (pFILTb->nCHK&HIGH_DEC && pFILTx->nCHK&ZERO_DEC) {
		stAFNORM.cur = INV_CLIMB_TOP2;	stAFNORM.nxt = PRE_CLIMB_PEAK;	xID = 0xd9;
	}
*/
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_InvJudge_DEC(void)
{
	PAFN_FILT_PARAM pFILTx = &stAFNORM.stOUTn_b;

	if (stAFNORM.nxt!=PRE_CLIMB_PEAK) AFNORM_ClrJudgeBUFF();
	if (stAFNORM.nxt!=PRE_CLIMB_PEAK) return;

	AFNORM_GetDriveRATE(DRV_CLIMB_TOP2);	// get judge RATE

	AFNORM_InvDecJudgeBUFF(&stAFNORM.stOUTn_h, stAFNORM.xCNT);
	AFNORM_InvDecJudgeBUFF(&stAFNORM.stOUTn_b, stAFNORM.xCNT);

	if ((pFILTx->nCHK&ZERO_DEC)==0) {
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

	AFNORM_GetDriveRATE(DRV_CLIMB_PEAK);	// get judge RATE

	AFNORM_GetTopJudgeBUFF(&stAFNORM.stOUTn_h, stAFNORM.xCNT);
	AFNORM_GetTopJudgeBUFF(&stAFNORM.stOUTn_b, stAFNORM.xCNT);

	if (AFNORM_IsDriveEnd()==TRUE) {
		stAFNORM.cur = INV_CLIMB_PEAK;	stAFNORM.nxt = DRV_FORCE_PEAK;	xID = 0xe1;
	} else if (pFILTx->nCHK&GOOD_DEC) {
		stAFNORM.cur = INV_CLIMB_PEAK;	stAFNORM.nxt = DRV_FORCE_PEAK;	xID = 0xe2;
	} else {
		stAFNORM.cur = DRV_CLIMB_PEAK;	stAFNORM.nxt = DRV_CLIMB_PEAK;	xID = 0xe3;
	}

	if (AFNORM_IsDriveEnd()==TRUE)	stAFNORM.eCNT = 2;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_InvJudge_TOP(void)
{
	PAFN_FILT_PARAM pFILTx = &stAFNORM.stOUTn_b;

	if (stAFNORM.nSEL==USE_HPF) pFILTx = &stAFNORM.stOUTn_h;

//	if (AFNORM_IsDriveEnd()==TRUE) return;
	if (stAFNORM.eCNT==2) return;
	AFNORM_GetDriveRATE(DRV_CLIMB_PEAK);	// get judge RATE

	AFNORM_InvTopJudgeBUFF(&stAFNORM.stOUTn_h, stAFNORM.xCNT);
	AFNORM_InvTopJudgeBUFF(&stAFNORM.stOUTn_b, stAFNORM.xCNT);

	if (pFILTx->nCHK&ZERO_DEC) return;

	stAFNORM.nxt = DRV_CLIMB_PEAK;
	AFNORM_SetChangeDIR();
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFNORM_InvJudge_END(void)
{
	PAFN_FILT_PARAM pFILTx = &stAFNORM.stOUTn_b;

	if (stAFNORM.nSEL==USE_HPF) pFILTx = &stAFNORM.stOUTn_h;

	if (stAFNORM.eCNT==2) return FALSE;
	AFNORM_GetDriveRATE(DRV_CLIMB_PEAK);	// get judge RATE

	AFNORM_InvEndJudgeBUFF(&stAFNORM.stOUTn_h, USE_HPF);
	AFNORM_InvEndJudgeBUFF(&stAFNORM.stOUTn_b, USE_BPF);

	return (pFILTx->nCHK&GOOD_END)? FALSE : TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_SetInitPARAM(void)
{
	stAFNORM.peak = stAFNORM.fVAL;

	stAFNORM.fMID = LENS_GetMIDPositionLOCI();
	stAFNORM.fGAP = LENS_GetGAPPositionLOCI();

#if defined(__USE_X03X_ZMAX__) // mimitest temp
	//stAFNORM.fMIN = LENS_GetMINPositionLOCI(stAFNORM.zVAL);
	stAFNORM.fMIN = LENS_GetMINPositionLOCI(stAFNORM.zVAL) - (stAFNORM.zVAL-13794)/2;
	stAFNORM.fMAX = LENS_GetMAXPositionLOCI(stAFNORM.zVAL) - (stAFNORM.zVAL-13794)/4;
#elif defined(__USE_2MOTOR_LENS__)
	stAFNORM.fMIN = LENS_GetMINPositionLOCI(stAFNORM.zVAL);
	stAFNORM.fMAX = LENS_GetMAXPositionLOCI(stAFNORM.zVAL);
#else
	stAFNORM.fMIN = LENS_GetMINPositionLOCI(stAFNORM.zVAL);
	stAFNORM.fMAX = LENS_GetMAXPositionLOCI(stAFNORM.zVAL);
#endif

	stAFNORM.nDIR = LENS_GetDirectionFOKS();
	LENS_SetDirectionFOKS(stAFNORM.nDIR);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_PreFilterSEL(void)
{
	if (stAFNORM.cur!=PRE_FILTER_SEL) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFNORM_SetInitPARAM();	LENS_EnableMotorFOKS(ON);
	stAFNORM.eCNT = 0; stAFNORM.cur = END_FILTER_SEL;
	stAFNORM.vCNT = 0; stAFNORM.nxt = END_FILTER_SEL;

	memset(&stAFNORM.stSTEP_m, 0, sizeof(AFN_STEP_PARAM));
	stAFNORM.stSTEP_m.nMIN = LENS_GetAFDriveSTEP(DRV_STEP_MIN);
	stAFNORM.stSTEP_m.nSTD = LENS_GetAFDriveSTEP(DRV_STEP_STD);
	stAFNORM.stSTEP_m.nMID = LENS_GetAFDriveSTEP(DRV_STEP_MID);
	stAFNORM.stSTEP_m.nMAX = LENS_GetAFDriveSTEP(DRV_STEP_MAX);
	stAFNORM.stSTEP_m.nFWD = LENS_GetAFDriveSTEP(DRV_STEP_FWD);
	stAFNORM.stSTEP_m.nBWD = LENS_GetAFDriveSTEP(DRV_STEP_BWD);
	stAFNORM.stSTEP_m.nTOP = LENS_GetAFDriveSTEP(DRV_STEP_TOP);

	if (stAFNORM.nSEL==USE_HPF) stAFNORM.vCNT |= 0x80;
	stAFNORM.nSEL = USE_BPF;	AFNORM_ClrJudgeBUFF();

	memset(&stAFNORM.stOUTn_h.nMIN, 0, sizeof(AFN_RATE_PARAM));
	memset(&stAFNORM.stOUTn_b.nMIN, 0, sizeof(AFN_RATE_PARAM));

	stAFNORM.stNORM_y.nRST = 0;		// clear reset LUMA-trig
	stAFNORM.stNORM_y.nERR = 0;		// clear error LUMA-trig
	stAFNORM.stNORM_y.nCNT = 0;		// clear count LUMA-trig

	stAFNCTL.n_GAIN = 0;	// clear normalized reference GAIN
	stAFNCTL.n_YSUM = 0;	// clear normalized reference YSUM
	stAFNCTL.n_FLTr = 0;	// clear normalized reference FLTr
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndFilterSEL(void)
{
	if (stAFNORM.cur!=END_FILTER_SEL) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFNORM.cur = PRE_SEARCH_MOV;
	stAFNORM.nxt = PRE_SEARCH_MOV;
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_PreSearchMOV(void)
{
	if (stAFNORM.cur!=PRE_SEARCH_MOV) return;

	stAFNORM.stOUTn_b.xHIGH = stAFNORM.stOUTn_b.FLTn;
	stAFNORM.stOUTn_h.xHIGH = stAFNORM.stOUTn_h.FLTn;

	stAFNORM.cur = DRV_SEARCH_MOV;
	stAFNCTL.n_GAIN = 0;	// clear normalized reference GAIN
	stAFNCTL.n_YSUM = 0;	// clear normalized reference YSUM
	stAFNCTL.n_FLTr = 0;	// clear normalized reference FLTr
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DrvSearchMOV(void)
{
	if (stAFNORM.cur!=DRV_SEARCH_MOV) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFNORM.cur = PRE_SEARCH_FAR;
	stAFNORM.nxt = PRE_SEARCH_FAR;

	AFNORM_ClrJudgeRATE();	// clear judge RATE
	AFNORM_ClrJudgePEAK();	// clear judge PEAK
	AFNORM_ClrJudgeINFO();	// clear judge INFO
	AFNORM_GetJudge_FAR();	// get judge INFO #1

	AFNORM_GetDriveSTEP(DRV_STEP_MIN);
	AFNORM_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_PreSearchFAR(void)
{
	if (stAFNORM.cur!=PRE_SEARCH_FAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFNORM.cur = DRV_SEARCH_FAR;
	stAFNORM.nxt = DRV_SEARCH_FAR;

	AFNORM_GetJudge_FAR();	// get judge INFO #2

	AFNORM_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DrvSearchFAR(void)
{
	if (stAFNORM.cur!=DRV_SEARCH_FAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFNORM_GetJudge_FAR();	// using HPFx & LPFx #3

	if (stAFNORM.cur==INV_SEARCH_FAR) return;
	AFNORM_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif
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
	AFNORM_SetDriveSTEP(DRV_STEP_REM);	// motor driving
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif
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

	AFNORM_SetDriveSTEP(DRV_STEP_REM);	// motor driving
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

	AFNORM_ClrJudgeRATE();	// clear judge RATE
	AFNORM_ClrJudgePEAK();	// clear judge PEAK
	AFNORM_ClrJudgeINFO();	// clear judge INFO
	AFNORM_GetJudge_NAR();	// get judge INFO #1

	AFNORM_GetDriveSTEP(DRV_STEP_MIN);
	AFNORM_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DrvSearchNAR(void)
{
	if (stAFNORM.cur!=DRV_SEARCH_NAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif

	AFNORM_GetJudge_NAR();	// using HPFx & LPFx #2

	if (stAFNORM.cur==INV_SEARCH_NAR) return;
	AFNORM_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndSearchNAR(void)
{
	if (stAFNORM.cur!=END_SEARCH_NAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif

	if (stAFNORM.nSEL==USE_HPF)
		 stAFNORM.peak = stAFNORM.stOUTn_h.xPEAK;
	else stAFNORM.peak = stAFNORM.stOUTn_b.xPEAK;

	stAFNORM.cur = stAFNORM.nxt;

	if (stAFNORM.cur==DRV_FORCE_PEAK) return;
	AFNORM_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_PreSearchDIR(void)
{
	if (stAFNORM.cur!=PRE_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFNORM.cur = DRV_SEARCH_DIR;
	stAFNORM.nxt = DRV_SEARCH_DIR;

	AFNORM_GetJudge_DIR();	// get judge INFO

	AFNORM_GetDriveSTEP(DRV_STEP_STD);
	AFNORM_SetDriveSTEP(DRV_STEP_STD);	// motor driving
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DrvSearchDIR(void)
{
	if (stAFNORM.cur!=DRV_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif

	AFNORM_GetJudge_DIR();	// using HPFx & LPFx

	if (stAFNORM.cur==INV_SEARCH_DIR) return;
	AFNORM_SetDriveSTEP(DRV_STEP_STD);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndSearchDIR(void)
{
	if (stAFNORM.cur!=END_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif

	if (stAFNORM.nSEL==USE_HPF)
		 stAFNORM.peak = stAFNORM.stOUTn_h.nPEAK;
	else stAFNORM.peak = stAFNORM.stOUTn_b.nPEAK;

	AFNORM_InvJudge_DIR();	// using HPFx & LPFx

	if (stAFNORM.eCNT==2)
		 stAFNORM.cur = DRV_FORCE_PEAK;
	else stAFNORM.cur = stAFNORM.nxt;

	if (stAFNORM.cur==DRV_FORCE_PEAK) return;
	AFNORM_SetDriveSTEP(DRV_STEP_PRV);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_PreClimbTOP2(void)
{
	if (stAFNORM.cur!=PRE_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFNORM.cur = DRV_CLIMB_TOP2;
	stAFNORM.nxt = DRV_CLIMB_TOP2;

	AFNORM_ClrJudgeRATE();	// clear judge RATE
	AFNORM_ClrJudgeINFO();	// clear judge INFO
	AFNORM_GetJudge_DEC();	// get judge INFO

	AFNORM_SetDriveSTEP(DRV_STEP_PRV);	// motor driving
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DrvClimbTOP2(void)
{
	if (stAFNORM.cur!=DRV_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif

	AFNORM_GetJudge_DEC();	// using HPFx & LPFx

	if (stAFNORM.cur==INV_CLIMB_TOP2) return;
	AFNORM_SetDriveSTEP(DRV_STEP_PRV);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndClimbTOP2(void)
{
	WORD step = stAFNORM.step/2;

	if (stAFNORM.cur!=END_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif

	if (stAFNORM.nSEL==USE_HPF)
		 stAFNORM.peak = stAFNORM.stOUTn_h.nPEAK;
	else stAFNORM.peak = stAFNORM.stOUTn_b.nPEAK;

	if (stAFNORM.fVAL==stAFNORM.peak) step = 0;

	if (stAFNORM.nDIR==fFAR) stAFNORM.peak -= step;
	else					 stAFNORM.peak += step;

	stAFNORM.peak = MAX(stAFNORM.peak,stAFNORM.fMIN);
	stAFNORM.peak = MIN(stAFNORM.peak,stAFNORM.fMAX);

	AFNORM_InvJudge_DEC();	// using HPFx & LPFx

	if (stAFNORM.eCNT==2)
		 stAFNORM.cur = DRV_FORCE_PEAK;
	else stAFNORM.cur = stAFNORM.nxt;

	if (stAFNORM.cur==DRV_FORCE_PEAK) return;
	if (stAFNORM.cur==PRE_CLIMB_PEAK && stAFNORM.fVAL==stAFNORM.peak) return;
	if (stAFNORM.cur==DRV_CLIMB_TOP2 && stAFNORM.fVAL==stAFNORM.peak) return;

	AFNORM_GetDriveSTEP(DRV_STEP_REM);
	AFNORM_SetDriveSTEP(DRV_STEP_REM);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFNORM_MovClimbPEAK(void)
{
	if (stAFNORM.cur!=PRE_CLIMB_PEAK) return FALSE;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return FALSE;
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif

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

	AFNORM_SetDriveSTEP(DRV_STEP_REM);	// motor driving
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_PreClimbPEAK(void)
{
	if (stAFNORM.cur!=PRE_CLIMB_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (AFNORM_MovClimbPEAK()==FALSE) return;
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif

	stAFNORM.cur = DRV_CLIMB_PEAK;
	stAFNORM.nxt = DRV_CLIMB_PEAK;

	AFNORM_ClrJudgeRATE();	// clear judge RATE
	AFNORM_ClrJudgePEAK();	// clear judge PEAK
	AFNORM_ClrJudgeINFO();	// clear judge INFO
	AFNORM_GetJudge_TOP();	// get judge INFO

	if (stAFNORM.stNORM_y.nERR) return;

	AFNORM_GetDriveSTEP(DRV_STEP_TOP);
	AFNORM_SetDriveSTEP(DRV_STEP_TOP);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DrvClimbPEAK(void)
{
	if (stAFNORM.cur!=DRV_CLIMB_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif

	AFNORM_GetJudge_TOP();	// using HPFx & LPFx

	if (stAFNORM.cur==INV_CLIMB_PEAK) return;
	AFNORM_SetDriveSTEP(DRV_STEP_TOP);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndClimbPEAK(void)
{
	if (stAFNORM.cur!=END_CLIMB_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif

	if (stAFNORM.nSEL==USE_HPF)
		 stAFNORM.peak = stAFNORM.stOUTn_h.nPEAK;
	else stAFNORM.peak = stAFNORM.stOUTn_b.nPEAK;

	if (stAFNORM.nDIR==fFAR)
		 stAFNORM.peak += 0*stAFNORM.stSTEP_m.nTOP;
	else stAFNORM.peak -= 0*stAFNORM.stSTEP_m.nTOP;

	AFNORM_InvJudge_TOP();	// using HPFx & LPFx

	stAFNORM.cur = stAFNORM.nxt;

	if (stAFNORM.cur==DRV_FORCE_PEAK) return;
	AFNORM_SetDriveSTEP(DRV_STEP_TOP);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DrvForcePEAK(void)
{
	if (stAFNORM.cur!=DRV_FORCE_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif

	if (stAFNORM.nDIR==fFAR) stAFNORM.peak += stAFNCTL.h_FOKS;
	else					 stAFNORM.peak -= stAFNCTL.h_FOKS;

	stAFNORM.cur = END_FORCE_PEAK;
	AFNORM_SetDrivePEAK();	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndForcePEAK(void)
{
	if (stAFNORM.cur!=END_FORCE_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif

	stAFNORM.cur = END_CLEAR_WAIT;	// wait progress
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndDriveWAIT(void)
{
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif

	switch (stAFNORM.cur) {
		case INV_FILTER_SEL: stAFNORM.cur = END_FILTER_SEL; break;
		case INV_SEARCH_FAR: stAFNORM.cur = END_SEARCH_FAR; break;
		case INV_SEARCH_NAR: stAFNORM.cur = END_SEARCH_NAR; break;
		case INV_SEARCH_DIR: stAFNORM.cur = END_SEARCH_DIR; break;
		case INV_CLIMB_TOP1: stAFNORM.cur = END_CLIMB_TOP1; break;
		case INV_CLIMB_TOP2: stAFNORM.cur = END_CLIMB_TOP2; break;
		case INV_CLIMB_PEAK: stAFNORM.cur = END_CLIMB_PEAK; break;
		case INV_SWING_SCAN: stAFNORM.cur = END_SWING_SCAN; break;
		default:			 return;
	}
	AFNORM_SetChangeDIR();	// change direction
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndClearLUMA(PAF_CTRL_PARAM pCTRL)
{
	WORD nID = MAKEWORD(stAFNORM.stNORM_y.nERR,stAFNORM.fMOV);

	if (stAFNORM.cur<=END_SEARCH_MOV) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFNORM.stNORM_y.nRST |= AF_GetLUMAInfo(pCTRL,nID);
	stAFNORM.stNORM_y.nERR  = FALSE;

	if (stAFNORM.stNORM_y.nRST) stAFNORM.cur = END_FORCE_PEAK;
#if __MISP100_AF_DBG__ == 1
//	if (ODM_GetDEBUGMode()==0x08)
//		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndClearAUTO(PAF_CTRL_PARAM pCTRL)
{
	if (stAFNORM.cur!=END_CLEAR_AUTO) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif

	stAFNORM.cur = END_CLEAR_PROC;	// stop progress

	if (stAFNORM.stNORM_y.nRST==TRUE) return;
	AF_SetAUTOInfo(pCTRL, stAFNORM.stNORM_y.nCHK);	// set AUTO

	LENS_SetTrackingINFO(stAFNORM.peak);	// update Track-info
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_EndClearWAIT(PAF_CTRL_PARAM pCTRL)
{
	BOOL nID;

	if (stAFNORM.cur!=END_CLEAR_WAIT) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	nID = (AFNORM_InvJudge_END())? TRUE : FALSE;
	stAFNORM.stNORM_y.nRST |= AF_GetDATAInfo(pCTRL,nID);

	stAFNORM.cur = END_CLEAR_AUTO;	// auto progress
	LENS_EnableMotorFOKS(OFF);
#if __MISP100_AF_DBG__ == 1
	if (ODM_GetDEBUGMode()==0x08)
		UARTprintf("%s ...\r\n", __FUNCTION__);
#endif
}

#if __MISP100_AF_DBG__ == 1
//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_HEADPrint(void)
{
	UARTprintf("\r\nAFNORM PROCESS TRACE...");
	UARTprintf("\r\n No   RPFo     BPFo     HPFo    YCNT   YSUM   DIFF  c  GAIN  v ");
//				    000 00000000 00000000 00000000 000000 000000 000000 0 000000 0 
	UARTprintf(" n_FLTr   n_FLTb    RPFn   ");
//				00000000 00000000 00000000 
	UARTprintf(" HPFx    HPFn   h_zINC h_nINC h_zDEC h_nDEC  hCHK  ");
//				000000 00000000 000000 000000 000000 000000 000000 
	UARTprintf(" BPFx    BPFn   b_zINC b_nINC b_zDEC b_nDEC  bCHK  ");
//				000000 00000000 000000 000000 000000 000000 000000 
	UARTprintf("fPeak  fVAL F D E STP SP PR|CU|NT  zoom n xV ID ");
//				00000 00000 0 0 0 000 00000000 00000 0 00 00

	UARTprintf("%02d %02d\r\n", eID, count_16ms);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_PROCPrint(void)
{
	UARTprintf("%03d ",		stAFNORM.time);
	UARTprintf("%08d ",		stAFNORM.stDATA_r.FLTo);
	UARTprintf("%08d ",		stAFNORM.stDATA_b.FLTo);
	UARTprintf("%08d ",		stAFNORM.stDATA_h.FLTo);
	UARTprintf("%06d ",		stAFNORM.stNORM_y.YCNT);
	UARTprintf("%06d ",		stAFNORM.stNORM_y.YSUM);
	UARTprintf("%06d ",		stAFNORM.stNORM_y.YSUM-stAFNORM.stNORM_y.YPRV);
	UARTprintf("%01X ",		stAFNORM.stNORM_y.nRST);
	UARTprintf("%06d ",		stAFNORM.stNORM_y.GAIN);
	UARTprintf("%01X ",		stAFNORM.stNORM_y.nCHK);

	UARTprintf("%08d ",		stAFNCTL.n_FLTr);
	UARTprintf("%08d ",		stAFNORM.stNORM_y.FLTb);
	UARTprintf("%08d ",		stAFNORM.stDATA_r.FLTn);

	UARTprintf("%06d ",		stAFNORM.stOUTn_h.FLTx);
	UARTprintf("%08d ",		stAFNORM.stOUTn_h.FLTn);
	UARTprintf("%06d ",		stAFNORM.stOUTn_h.zINC);
	UARTprintf("%06d ",		stAFNORM.stOUTn_h.nINC);
	UARTprintf("%06d ",		stAFNORM.stOUTn_h.zDEC);
	UARTprintf("%06d ",		stAFNORM.stOUTn_h.nDEC);
	UARTprintf("%06X ",		stAFNORM.stOUTn_h.nCHK);

	UARTprintf("%06d ",		stAFNORM.stOUTn_b.FLTx);
	UARTprintf("%08d ",		stAFNORM.stOUTn_b.FLTn);
	UARTprintf("%06d ",		stAFNORM.stOUTn_b.zINC);
	UARTprintf("%06d ",		stAFNORM.stOUTn_b.nINC);
	UARTprintf("%06d ",		stAFNORM.stOUTn_b.zDEC);
	UARTprintf("%06d ",		stAFNORM.stOUTn_b.nDEC);
	UARTprintf("%06X ",		stAFNORM.stOUTn_b.nCHK);

	UARTprintf("%05d ",		stAFNORM.peak);
	UARTprintf("%05d ",		stAFNORM.fVAL);
	UARTprintf("%01d ",		stAFNORM.nSEL);
	UARTprintf("%01d ",		stAFNORM.nDIR);
	UARTprintf("%01d ",		stAFNORM.eCNT);
	UARTprintf("%03d ",		stAFNORM.step);
	UARTprintf("%02x ",		stAFNORM.stSTEP_m.nSPD);
	UARTprintf("%02X|%02X|%02X ", stAFNORM.prv,stAFNORM.cur,stAFNORM.nxt);
	UARTprintf("%05d ",		stAFNORM.zVAL);
	UARTprintf("%01d ",		stAFNORM.stSTEP_m.nCHK);//stAFNORM.xCNT);
//	UARTprintf("%07d ",		stAFNORM.stOUTn_h.xHIGH);
//	UARTprintf("%07d ",		stAFNORM.stOUTn_b.xHIGH);
	UARTprintf("%02X ",		stAFNORM.vCNT);
	UARTprintf("%02X ",		xID);

	UARTprintf("%02d %02d\r\n", eID, count_16ms);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFNORM_DBGPrint(void)
{
	if (stAFNORM.time)	AFNORM_PROCPrint();
	else				AFNORM_HEADPrint();

	stAFNORM.time++;
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
void AFNORM_ProcessHandler(PAF_CTRL_PARAM pCTRL)
{
	if (stAFNORM.cur==0) return;
	stAFNORM.prv = stAFNORM.cur;	// for debug print

	AFNORM_GetAFOPDWIND(pCTRL);		// get AFOPD data
	AFNORM_EndClearLUMA(pCTRL);		// chk LUMA info
	AFNORM_EndClearAUTO(pCTRL);		// set AUTO info
	AFNORM_EndClearWAIT(pCTRL);		// set AUTO wait


	AFNORM_PreFilterSEL();		// select filter
	AFNORM_PreSearchMOV();		// search wait-luma
	AFNORM_PreSearchFAR();		// search shake-far
	AFNORM_PreSearchNAR();		// search shake-nar
	AFNORM_PreSearchDIR();		// search direction
	AFNORM_PreClimbTOP2();		// search climb-dec
	AFNORM_PreClimbPEAK();		// search fine-peak

	AFNORM_DrvSearchMOV();		// drive force-mov
	AFNORM_DrvSearchFAR();		// drive shake-far
	AFNORM_DrvSearchNAR();		// drive shake-nar
	AFNORM_DrvSearchDIR();		// drive direction
	AFNORM_DrvClimbTOP2();		// drive climb-dec
	AFNORM_DrvClimbPEAK();		// drive fine-peak
	AFNORM_DrvForcePEAK();		// drive force-top

	AFNORM_EndFilterSEL();		// arrange filter
	AFNORM_EndSearchFAR();		// arrange shake-far
	AFNORM_EndSearchNAR();		// arrange shake-nar
	AFNORM_EndSearchDIR();		// arrange direction
	AFNORM_EndClimbTOP2();		// arrange climb-dec
	AFNORM_EndClimbPEAK();		// arrange fine-peak

	AFNORM_EndForcePEAK();		// wait force-peak
	AFNORM_EndDriveWAIT();		// wait drive-stop
	eID = count_16ms;

#if __MISP100_AF_DBG__ == 1
	if ((ODM_GetDEBUGMode()!=0x03) && (ODM_GetDEBUGMode()!=0x05)) return;
	AFNORM_DBGPrint();
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void AFNORM_EnableProcess(BOOL OnOff)
{
	if (OnOff)	stAFNORM.cur = PRE_FILTER_SEL;
	else		stAFNORM.cur = END_CLEAR_PROC;

	stAFNORM.time = 0;	// for debug print
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
		UPDNb[ 0]=stAFNCTL.stPRV_b.n_UP;
		UPDNb[ 1]=stAFNCTL.stPRV_b.n_DN;
		UPDNb[ 2]=stAFNCTL.stPRV_b.z_UP;
		UPDNb[ 3]=stAFNCTL.stPRV_b.z_DN;
		UPDNb[ 4]=stAFNCTL.stPRV_h.n_UP;
		UPDNb[ 5]=stAFNCTL.stPRV_h.n_DN;
		UPDNb[ 6]=stAFNCTL.stPRV_h.z_UP;
		UPDNb[ 7]=stAFNCTL.stPRV_h.z_DN;
		UPDNb[ 8]=stAFNCTL.stDIR_b.n_UP;
		UPDNb[ 9]=stAFNCTL.stDIR_b.n_DN;
		UPDNb[10]=stAFNCTL.stDIR_b.z_UP;
		UPDNb[11]=stAFNCTL.stDIR_b.z_DN;
		UPDNb[12]=stAFNCTL.stDIR_h.n_UP;
		UPDNb[13]=stAFNCTL.stDIR_h.n_DN;
		UPDNb[14]=stAFNCTL.stDIR_h.z_UP;
		UPDNb[15]=stAFNCTL.stDIR_h.z_DN;
		UPDNb[16]=stAFNCTL.stDEC_b.n_UP;
		UPDNb[17]=stAFNCTL.stDEC_b.n_DN;
		UPDNb[18]=stAFNCTL.stDEC_b.z_UP;
		UPDNb[19]=stAFNCTL.stDEC_b.z_DN;
		UPDNb[20]=stAFNCTL.stDEC_h.n_UP;
		UPDNb[21]=stAFNCTL.stDEC_h.n_DN;
		UPDNb[22]=stAFNCTL.stDEC_h.z_UP;
		UPDNb[23]=stAFNCTL.stDEC_h.z_DN;
		UPDNb[24]=stAFNCTL.stTOP_b.n_UP;
		UPDNb[25]=stAFNCTL.stTOP_b.n_DN;
		UPDNb[26]=stAFNCTL.stTOP_b.z_UP;
		UPDNb[27]=stAFNCTL.stTOP_b.z_DN;
		UPDNb[28]=stAFNCTL.stTOP_h.n_UP;
		UPDNb[29]=stAFNCTL.stTOP_h.n_DN;
		UPDNb[30]=stAFNCTL.stTOP_h.z_UP;
		UPDNb[31]=stAFNCTL.stTOP_h.z_DN;
	}else if(init==2) {
		stAFNCTL.stPRV_b.n_UP=UPDNb[ 0];
		stAFNCTL.stPRV_b.n_DN=UPDNb[ 1];
		stAFNCTL.stPRV_b.z_UP=UPDNb[ 2];
		stAFNCTL.stPRV_b.z_DN=UPDNb[ 3];
		stAFNCTL.stPRV_h.n_UP=UPDNb[ 4];
		stAFNCTL.stPRV_h.n_DN=UPDNb[ 5];
		stAFNCTL.stPRV_h.z_UP=UPDNb[ 6];
		stAFNCTL.stPRV_h.z_DN=UPDNb[ 7];
		stAFNCTL.stDIR_b.n_UP=UPDNb[ 8];
		stAFNCTL.stDIR_b.n_DN=UPDNb[ 9];
		stAFNCTL.stDIR_b.z_UP=UPDNb[10];
		stAFNCTL.stDIR_b.z_DN=UPDNb[11];
		stAFNCTL.stDIR_h.n_UP=UPDNb[12];
		stAFNCTL.stDIR_h.n_DN=UPDNb[13];
		stAFNCTL.stDIR_h.z_UP=UPDNb[14];
		stAFNCTL.stDIR_h.z_DN=UPDNb[15];
		stAFNCTL.stDEC_b.n_UP=UPDNb[16];
		stAFNCTL.stDEC_b.n_DN=UPDNb[17];
		stAFNCTL.stDEC_b.z_UP=UPDNb[18];
		stAFNCTL.stDEC_b.z_DN=UPDNb[19];
		stAFNCTL.stDEC_h.n_UP=UPDNb[20];
		stAFNCTL.stDEC_h.n_DN=UPDNb[21];
		stAFNCTL.stDEC_h.z_UP=UPDNb[22];
		stAFNCTL.stDEC_h.z_DN=UPDNb[23];
		stAFNCTL.stTOP_b.n_UP=UPDNb[24];
		stAFNCTL.stTOP_b.n_DN=UPDNb[25];
		stAFNCTL.stTOP_b.z_UP=UPDNb[26];
		stAFNCTL.stTOP_b.z_DN=UPDNb[27];
		stAFNCTL.stTOP_h.n_UP=UPDNb[28];
		stAFNCTL.stTOP_h.n_DN=UPDNb[29];
		stAFNCTL.stTOP_h.z_UP=UPDNb[30];
		stAFNCTL.stTOP_h.z_DN=UPDNb[31];
	}

	switch(cur) {
		case  0 : 		stAFNCTL.stPRV_b.n_UP=UPDNb[ 0]+5*val; break;
		case  1 : 		stAFNCTL.stPRV_b.n_DN=UPDNb[ 1]+5*val; break;
		case  2 : 		stAFNCTL.stPRV_b.z_UP=UPDNb[ 2]+2*val; break;
		case  3 : 		stAFNCTL.stPRV_b.z_DN=UPDNb[ 3]+2*val; break;
		case  4 : 		stAFNCTL.stPRV_h.n_UP=UPDNb[ 4]+5*val; break;
		case  5 : 		stAFNCTL.stPRV_h.n_DN=UPDNb[ 5]+5*val; break;
		case  6 : 		stAFNCTL.stPRV_h.z_UP=UPDNb[ 6]+2*val; break;
		case  7 : 		stAFNCTL.stPRV_h.z_DN=UPDNb[ 7]+2*val; break;
		case  8 : 		stAFNCTL.stDIR_b.n_UP=UPDNb[ 8]+5*val; break;
		case  9 : 		stAFNCTL.stDIR_b.n_DN=UPDNb[ 9]+5*val; break;
		case 10 : 		stAFNCTL.stDIR_b.z_UP=UPDNb[10]+2*val; break;
		case 11 : 		stAFNCTL.stDIR_b.z_DN=UPDNb[11]+2*val; break;
		case 12 : 		stAFNCTL.stDIR_h.n_UP=UPDNb[12]+5*val; break;
		case 13 : 		stAFNCTL.stDIR_h.n_DN=UPDNb[13]+5*val; break;
		case 14 : 		stAFNCTL.stDIR_h.z_UP=UPDNb[14]+2*val; break;
		case 15 : 		stAFNCTL.stDIR_h.z_DN=UPDNb[15]+2*val; break;
		case 16 : 		stAFNCTL.stDEC_b.n_UP=UPDNb[16]+5*val; break;
		case 17 : 		stAFNCTL.stDEC_b.n_DN=UPDNb[17]+5*val; break;
		case 18 : 		stAFNCTL.stDEC_b.z_UP=UPDNb[18]+2*val; break;
		case 19 : 		stAFNCTL.stDEC_b.z_DN=UPDNb[19]+2*val; break;
		case 20 : 		stAFNCTL.stDEC_h.n_UP=UPDNb[20]+5*val; break;
		case 21 : 		stAFNCTL.stDEC_h.n_DN=UPDNb[21]+5*val; break;
		case 22 : 		stAFNCTL.stDEC_h.z_UP=UPDNb[22]+2*val; break;
		case 23 : 		stAFNCTL.stDEC_h.z_DN=UPDNb[23]+2*val; break;
		case 24 : 		stAFNCTL.stTOP_b.n_UP=UPDNb[24]+5*val; break;
		case 25 : 		stAFNCTL.stTOP_b.n_DN=UPDNb[25]+5*val; break;
		case 26 : 		stAFNCTL.stTOP_b.z_UP=UPDNb[26]+2*val; break;
		case 27 : 		stAFNCTL.stTOP_b.z_DN=UPDNb[27]+2*val; break;
		case 28 : 		stAFNCTL.stTOP_h.n_UP=UPDNb[28]+5*val; break;
		case 29 : 		stAFNCTL.stTOP_h.n_DN=UPDNb[29]+5*val; break;
		case 30 : 		stAFNCTL.stTOP_h.z_UP=UPDNb[30]+2*val; break;
		case 31 : 		stAFNCTL.stTOP_h.z_DN=UPDNb[31]+2*val; break;
	}

#if __MISP100_AF_DBG__ == 1
	UARTprintf("stAFNCTL.stPRV_b : %3d %3d %3d %3d\r\n",stAFNCTL.stPRV_b.n_UP,stAFNCTL.stPRV_b.n_DN,stAFNCTL.stPRV_b.z_UP,stAFNCTL.stPRV_b.z_DN);
	UARTprintf("stAFNCTL.stPRV_h : %3d %3d %3d %3d\r\n",stAFNCTL.stPRV_h.n_UP,stAFNCTL.stPRV_h.n_DN,stAFNCTL.stPRV_h.z_UP,stAFNCTL.stPRV_h.z_DN);
	UARTprintf("stAFNCTL.stDIR_b : %3d %3d %3d %3d\r\n",stAFNCTL.stDIR_b.n_UP,stAFNCTL.stDIR_b.n_DN,stAFNCTL.stDIR_b.z_UP,stAFNCTL.stDIR_b.z_DN);
	UARTprintf("stAFNCTL.stDIR_h : %3d %3d %3d %3d\r\n",stAFNCTL.stDIR_h.n_UP,stAFNCTL.stDIR_h.n_DN,stAFNCTL.stDIR_h.z_UP,stAFNCTL.stDIR_h.z_DN);
	UARTprintf("stAFNCTL.stDEC_b : %3d %3d %3d %3d\r\n",stAFNCTL.stDEC_b.n_UP,stAFNCTL.stDEC_b.n_DN,stAFNCTL.stDEC_b.z_UP,stAFNCTL.stDEC_b.z_DN);
	UARTprintf("stAFNCTL.stDEC_h : %3d %3d %3d %3d\r\n",stAFNCTL.stDEC_h.n_UP,stAFNCTL.stDEC_h.n_DN,stAFNCTL.stDEC_h.z_UP,stAFNCTL.stDEC_h.z_DN);
	UARTprintf("stAFNCTL.stTOP_b : %3d %3d %3d %3d\r\n",stAFNCTL.stTOP_b.n_UP,stAFNCTL.stTOP_b.n_DN,stAFNCTL.stTOP_b.z_UP,stAFNCTL.stTOP_b.z_DN);
	UARTprintf("stAFNCTL.stTOP_h : %3d %3d %3d %3d\r\n",stAFNCTL.stTOP_h.n_UP,stAFNCTL.stTOP_h.n_DN,stAFNCTL.stTOP_h.z_UP,stAFNCTL.stTOP_h.z_DN);
	UARTprintf("\r\n");
#endif
}

#endif

/*  FILE_END_HERE */
