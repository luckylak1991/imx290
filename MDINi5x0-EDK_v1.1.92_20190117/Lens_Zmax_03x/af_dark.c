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

}	AFD_BUFF_PARAM, *PAFD_BUFF_PARAM;

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

}	AFD_RATE_PARAM, *PAFD_RATE_PARAM;

typedef	struct
{
	LONG	nHIGH;			// peak-filt-val of HPFn in norm
	SHORT	nPEAK;			// peak-position of HPFn in norm
	SHORT	nDUMY;			// dummy

}	AFD_PEAK_PARAM, *PAFD_PEAK_PARAM;

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

}	AFD_NORM_PARAM, *PAFD_NORM_PARAM;

typedef	struct
{
	LONG	FLTa;			// original FILT-A data
	LONG	FLTb;			// original FILT-B data
	LONG	FLTo;			// original FILTA+FILTB
	LONG	FLTn;			// normalized FILT data

}	AFD_DATA_PARAM, *PAFD_DATA_PARAM;

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

}	AFD_FILT_PARAM, *PAFD_FILT_PARAM;

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

}	AFD_STEP_COUNT, *PAFD_STEP_COUNT;

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

	AFD_STEP_COUNT	stDIR_I;
	AFD_STEP_COUNT	stDEC_I;
	AFD_STEP_COUNT	stDEC_D;

}	AFD_STEP_PARAM, *PAFD_STEP_PARAM;

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
	SHORT	KNEE;			// AF control knee position data
	WORD	time;			// AF control execution time

	BYTE	DARK;			//
	BYTE	DUMY;			// dummy

	AFD_NORM_PARAM	stNORM_y;	// data normalize Y-info
	AFD_DATA_PARAM	stDATA_r;	// data normalization RPF
	AFD_DATA_PARAM	stDATA_l;	// data normalization LPF
	AFD_DATA_PARAM	stDATA_v;	// data normalization VPF
	AFD_DATA_PARAM	stDATA_b;	// data normalization BPF
	AFD_DATA_PARAM	stDATA_h;	// data normalization HPF
	AFD_FILT_PARAM	stOUTn_b;	// filter compesation BPF
	AFD_FILT_PARAM	stOUTn_h;	// filter compesation HPF
	AFD_STEP_PARAM	stSTEP_m;	// driving focus step

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
	SHORT	n_UP;			// threshold min-data of min-rate
	SHORT	n_DN;			// threshold max-data of max-rate
	SHORT	z_UP;			// threshold min-data of min-zero
	SHORT	z_DN;			// threshold max-data of max-zero

}	AFD_TUNE_RATIO, *PAFD_TUNE_RATIO;

typedef	struct
{
	BYTE	r_MIN;			// ref-count of MIN-SPD up/dn
	BYTE	r_STD;			// ref-count of STD-SPD up/dn
	BYTE	r_MID;			// ref-count of MID-SPD up/dn
	BYTE	r_MAX;			// ref-count of MAX-SPD up/dn

}	AFD_TUNE_COUNT, *PAFD_TUNE_COUNT;

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

	AFD_TUNE_RATIO	stPRV_b;		// shake-prv rate of BPF
	AFD_TUNE_RATIO	stPRV_h;		// shake-prv rate of HPF

	AFD_TUNE_RATIO	stDIR_b;		// direction rate of BPF
	AFD_TUNE_RATIO	stDIR_h;		// direction rate of HPF

	AFD_TUNE_RATIO	stDEC_b;		// climb-dec rate of BPF
	AFD_TUNE_RATIO	stDEC_h;		// climb-dec rate of HPF

	AFD_TUNE_RATIO	stTOP_b;		// fine-peak rate of BPF
	AFD_TUNE_RATIO	stTOP_h;		// fine-peak rate of HPF

}	AFD_TUNE_PARAM, *PAFD_TUNE_PARAM;

#define		MIN3(a,b,c)				(MIN(MIN(a,b),c))
#define		MAX3(a,b,c)				(MAX(MAX(a,b),c))

#define		MIN4(a,b,c,d)			(MIN(MIN(a,b),MIN(c,d)))
#define		MAX4(a,b,c,d)			(MAX(MAX(a,b),MAX(c,d)))

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static AFD_PROC_PARAM stAFDARK;
static AFD_TUNE_PARAM stAFDCTL = {
//	n_GAIN	n_YSUM	n_FLTr	c_FLTb	c_FLTh	c_GMIN	c_GMAX
	0,		0,		0,		500,		2048,	32767,	65535,

//	h_ZOOM	h_FOKS
	0,		0,

//   n_UP    n_DN    z_UP    z_DN	for shake-prv
	{  50,    -30,     50,     -5},	// BPF
	{  50,    -30,     50,     -5},	// HPF

//   n_UP    n_DN    z_UP    z_DN	for direction
	{  50,    -50,     10,    -10},	// BPF
	{  50,    -50,     10,    -10},	// HPF

//   n_UP    n_DN    z_UP    z_DN	for climb-dec
	{  50,    -50,     10,    -10},	// BPF
	{  50,    -50,     10,    -10},	// HPF

//   n_UP    n_DN    z_UP    z_DN	for find-peak
	{  20,    -10,      1,     -1},	// BPF
	{  20,    -10,      1,     -1}	// HPF
};

static LONG xVAL;
static BYTE eID, xID = 0;	// for debug print

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
extern VBYTE count_16ms;

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
	stAFDARK.stNORM_y.GAIN = (pCTRL->stFILT.GAIN)>>0; // AGC gain
	stAFDARK.stNORM_y.YREF = (AECTRL_GetBrightness()); //AE ref

	// get conditional FLTb data
	nGAIN = ((FLOAT)AECTRL_GetGAINMin())/stAFDARK.stNORM_y.GAIN;

	if (stAFDARK.stNORM_y.GAIN>stAFDCTL.c_GMAX)
		 nYSUM = ((FLOAT)stAFDARK.stNORM_y.YSUM)/stAFDARK.stNORM_y.YREF;
	else nYSUM = ((FLOAT)stAFDARK.stNORM_y.YREF)/stAFDARK.stNORM_y.YSUM;

	stAFDARK.stNORM_y.FLTb = (LONG)(nGAIN*nYSUM*stAFDARK.stDATA_b.FLTo+0.5);

	// get normalized FILT data
	if (stAFDCTL.n_GAIN==0) stAFDCTL.n_GAIN = stAFDARK.stNORM_y.GAIN;
	nGAIN = ((FLOAT)stAFDCTL.n_GAIN)/stAFDARK.stNORM_y.GAIN;

	if (stAFDCTL.n_YSUM==0) stAFDCTL.n_YSUM = stAFDARK.stNORM_y.YSUM;
	nYSUM = ((FLOAT)stAFDCTL.n_YSUM)/stAFDARK.stNORM_y.YSUM;

	stAFDARK.stDATA_b.FLTo += stAFDARK.stDATA_b.FLTb+stAFDARK.stDATA_v.FLTb;
	stAFDARK.stDATA_h.FLTo += stAFDARK.stDATA_h.FLTb+stAFDARK.stDATA_v.FLTb;

	stAFDARK.stDATA_r.FLTn = (LONG)(nGAIN*nYSUM*stAFDARK.stDATA_r.FLTo+0.5);
	stAFDARK.stDATA_b.FLTn = (LONG)(nGAIN*nYSUM*stAFDARK.stDATA_b.FLTo+0.5);
	stAFDARK.stDATA_h.FLTn = (LONG)(nGAIN*nYSUM*stAFDARK.stDATA_h.FLTo+0.5);
/*
	// get conditional min-check nCHK
	if (stAFDARK.stNORM_y.GAIN>stAFDCTL.c_GMIN && stAFDARK.stNORM_y.FLTb<stAFDCTL.c_FLTb)
		 stAFDARK.stNORM_y.nCHK = 1;
	else stAFDARK.stNORM_y.nCHK = 0;
*/
	// get conditional dark-check DARK
	if (stAFDARK.stNORM_y.GAIN>stAFDCTL.c_GMIN)	stAFDARK.DARK = 1;
	else										stAFDARK.DARK = 0;

	// get normalized reference c_RPFn
	if (stAFDARK.stNORM_y.nCHK && stAFDCTL.n_FLTr==0) stAFDCTL.n_FLTr = stAFDARK.stDATA_r.FLTn;

	// get compensation FILT data
	nHPFr = (stAFDARK.stNORM_y.nCHK)? ((FLOAT)stAFDCTL.n_FLTr)/stAFDARK.stDATA_r.FLTn : 1.0f;

	stAFDARK.stOUTn_b.FLTn = (LONG)(nHPFr*stAFDARK.stDATA_b.FLTn+0.5);
	stAFDARK.stOUTn_h.FLTn = (LONG)(nHPFr*stAFDARK.stDATA_h.FLTn+0.5);

	// get current position
	stAFDARK.nDIR = LENS_GetDirectionFOKS();
	stAFDARK.fVAL = LENS_GetCurPositionFOKS();
	stAFDARK.zVAL = LENS_GetCurPositionZOOM();
	stAFDARK.ZOOM = LENS_GetRelPositionZOOM();
	stAFDARK.KNEE = LENS_GetKneePositionZOOM();

	xVAL = (pCTRL->nMOD==DARK_LL)? 1000 : 500;
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
	WORD max = (WORD)stAFDARK.stSTEP_m.nBWD;
	WORD min = (WORD)stAFDARK.stSTEP_m.nMIN;

	switch (nID) {
		case DRV_STEP_MIN:	stAFDARK.step = (WORD)stAFDARK.stSTEP_m.nMIN; break;
		case DRV_STEP_STD:	stAFDARK.step = (WORD)stAFDARK.stSTEP_m.nSTD; break;
		case DRV_STEP_MID:	stAFDARK.step = (WORD)stAFDARK.stSTEP_m.nMID; break;
		case DRV_STEP_MAX:	stAFDARK.step = (WORD)stAFDARK.stSTEP_m.nMAX; break;

		case DRV_STEP_FWD:	stAFDARK.step = (WORD)stAFDARK.stSTEP_m.nFWD; break;
		case DRV_STEP_BWD:	stAFDARK.step = (WORD)stAFDARK.stSTEP_m.nBWD; break;
		case DRV_STEP_TOP:	stAFDARK.step = (WORD)stAFDARK.stSTEP_m.nTOP; break;

		case DRV_STEP_DIV:	stAFDARK.step = MIN(MAX((gap+3)/4,min), max); break;
		case DRV_STEP_REM:	stAFDARK.step = (quo)? stAFDARK.step : rem; break;
	}

	if ((nID&0xf0)==DRV_STEP_MIN) stAFDARK.stSTEP_m.nSPD = nID;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_GetDriveRATE(BYTE nID)
{
	PBYTE pTUNEb = NULL, pTUNEh = NULL;

	switch (nID) {
		case DRV_SEARCH_FAR: case PRE_SEARCH_FAR:
		case DRV_SEARCH_NAR: case PRE_SEARCH_NAR:
			pTUNEb = (PBYTE)&stAFDCTL.stPRV_b;	pTUNEh = (PBYTE)&stAFDCTL.stPRV_h;	break;

		case DRV_SEARCH_DIR: case PRE_SEARCH_DIR:
			pTUNEb = (PBYTE)&stAFDCTL.stDIR_b;	pTUNEh = (PBYTE)&stAFDCTL.stDIR_h;	break;

		case DRV_CLIMB_TOP2: case PRE_CLIMB_TOP2:
			pTUNEb = (PBYTE)&stAFDCTL.stDEC_b;	pTUNEh = (PBYTE)&stAFDCTL.stDEC_h;	break;

		case DRV_CLIMB_PEAK: case PRE_CLIMB_PEAK:
			pTUNEb = (PBYTE)&stAFDCTL.stTOP_b;	pTUNEh = (PBYTE)&stAFDCTL.stTOP_h;	break;
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

	stAFDARK.goal = MIN(stAFDARK.goal, stAFDARK.fMAX);	// check range
	stAFDARK.goal = MAX(stAFDARK.goal, stAFDARK.fMIN);

	LENS_SetDriveFOKS(stAFDARK.goal);	// start motor drive
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_SetDrivePEAK(void)
{
	stAFDARK.goal = stAFDARK.peak;

	stAFDARK.goal = MIN(stAFDARK.goal, stAFDARK.fMAX);	// check range
	stAFDARK.goal = MAX(stAFDARK.goal, stAFDARK.fMIN);

	LENS_SetDriveFOKS(stAFDARK.goal);	// start motor drive
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_ClrDrvCountSPD(PAFD_STEP_COUNT pCNT)
{
	SHORT nFWD = LENS_GetAFDriveSTEP(DRV_STEP_FWD);
	LONG  GAIN = stAFDARK.stNORM_y.GAIN, KNEE = MIN(GAIN,8192);

	if (GAIN>1023) nFWD -= (SHORT)(6.0f*(FLOAT)KNEE/ 1024+0.5);
	if (GAIN>8191) nFWD -= (SHORT)(6.0f*(FLOAT)GAIN/16384+0.5);
	stAFDARK.stSTEP_m.nFWD = MAX(nFWD,stAFDARK.stSTEP_m.nSTD*3);

	switch (stAFDARK.stSTEP_m.nSPD) {
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

	AFDARK_ClrDrvCountSPD(pCNT);	// clear counter

	switch (stAFDARK.stSTEP_m.nSPD) {
		case DRV_STEP_MIN:		AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;

		case DRV_STEP_STD:		if (pCNT->xSTD<6) pCNT->xSTD++;
			if (pCNT->xSTD==6)	AFDARK_GetDriveSTEP(DRV_STEP_MID);
			else				AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;

		case DRV_STEP_MID:		if (pCNT->xMID<3) pCNT->xMID++;
			if (pCNT->xMID==3)	AFDARK_GetDriveSTEP(DRV_STEP_MAX);
			else				AFDARK_GetDriveSTEP(DRV_STEP_MID);	break;

		case DRV_STEP_MAX:		if (pCNT->xMAX<2) pCNT->xMAX++;
								AFDARK_GetDriveSTEP(DRV_STEP_MAX);	break;
	}
	if (pCNT->xMAX==1) AFDARK_ClrJudgePEAK();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DecDrvIncSPD_GX(BYTE nID)
{
	PAFD_STEP_COUNT pCNT = &stAFDARK.stSTEP_m.stDEC_I;

	AFDARK_ClrDrvCountSPD(pCNT);	// clear counter
	memset(&stAFDARK.stSTEP_m.stDEC_D, 0, sizeof(AFD_STEP_COUNT));

	if (stAFDARK.stSTEP_m.nSPD==DRV_STEP_MAX) stAFDARK.stSTEP_m.nCHK = 1;

	if (stAFDARK.stSTEP_m.nCHK) {
		switch (stAFDARK.stSTEP_m.nSPD) {
			case DRV_STEP_MIN:		if (pCNT->xMIN<6) pCNT->xMIN++;
				if (pCNT->xMIN==6)	AFDARK_GetDriveSTEP(DRV_STEP_STD);
				else				AFDARK_GetDriveSTEP(DRV_STEP_MIN);	break;

			case DRV_STEP_STD:		if (pCNT->xSTD<4) pCNT->xSTD++;
				if (pCNT->xSTD==4)	AFDARK_GetDriveSTEP(DRV_STEP_MID);
				else				AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		AFDARK_GetDriveSTEP(DRV_STEP_MID);	break;

			case DRV_STEP_MAX:		AFDARK_GetDriveSTEP(DRV_STEP_MID);	break;
		}
	}
	else {
		switch (stAFDARK.stSTEP_m.nSPD) {
			case DRV_STEP_MIN:		AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_STD:		if (pCNT->xSTD<4) pCNT->xSTD++;
				if (pCNT->xSTD==4)	AFDARK_GetDriveSTEP(DRV_STEP_MID);
				else				AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		AFDARK_GetDriveSTEP(DRV_STEP_MAX);	break;

			case DRV_STEP_MAX:		AFDARK_GetDriveSTEP(DRV_STEP_MAX);	break;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DecDrvIncSPD_BI(BYTE nID)
{
	PAFD_STEP_COUNT pCNT = &stAFDARK.stSTEP_m.stDEC_I;

	AFDARK_ClrDrvCountSPD(pCNT);	// clear counter
	memset(&stAFDARK.stSTEP_m.stDEC_D, 0, sizeof(AFD_STEP_COUNT));

	switch (stAFDARK.stSTEP_m.nSPD) {
		case DRV_STEP_MIN:		if (pCNT->xMIN<4) pCNT->xMIN++;
			if (pCNT->xMIN==4)	AFDARK_GetDriveSTEP(DRV_STEP_STD);
			else				AFDARK_GetDriveSTEP(DRV_STEP_MIN);	break;

		case DRV_STEP_STD:		if (pCNT->xSTD<3) pCNT->xSTD++;
			if (pCNT->xSTD==3)	AFDARK_GetDriveSTEP(DRV_STEP_MID);
			else				AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;

		case DRV_STEP_MID:		if (pCNT->xMID<2) pCNT->xMID++;
			if (pCNT->xMID==2)	AFDARK_GetDriveSTEP(DRV_STEP_MAX);
			else				AFDARK_GetDriveSTEP(DRV_STEP_MID);	break;

		case DRV_STEP_MAX:		AFDARK_GetDriveSTEP(DRV_STEP_MAX);	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DecDrvDecSPD_GD(BYTE nID)
{
	PAFD_STEP_COUNT pCNT = &stAFDARK.stSTEP_m.stDEC_D;

	AFDARK_ClrDrvCountSPD(pCNT);	// clear counter
	memset(&stAFDARK.stSTEP_m.stDEC_I, 0, sizeof(AFD_STEP_COUNT));

	if (stAFDARK.stSTEP_m.nSPD==DRV_STEP_MAX) stAFDARK.stSTEP_m.nCHK = 1;

	if (stAFDARK.stSTEP_m.nCHK) {
		switch (stAFDARK.stSTEP_m.nSPD) {
			case DRV_STEP_MIN:		AFDARK_GetDriveSTEP(DRV_STEP_MIN);	break;

			case DRV_STEP_STD:		if (pCNT->xSTD<2) pCNT->xSTD++;
				if (pCNT->xSTD==2)	AFDARK_GetDriveSTEP(DRV_STEP_MIN);
				else				AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MID:		AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;

			case DRV_STEP_MAX:		AFDARK_GetDriveSTEP(DRV_STEP_STD);	break;
		}
	}
	else {
		switch (stAFDARK.stSTEP_m.nSPD) {
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

	switch (stAFDARK.stSTEP_m.nSPD) {
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
	// clear RATE-flag info
	stAFDARK.stOUTn_h.nCHK = 0;
	stAFDARK.stOUTn_b.nCHK = 0;

	stAFDARK.stSTEP_m.nCNT = 0;		// clear speed count
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_ClrJudgeBUFF(void)
{
	memset(stAFDARK.stOUTn_h.nBUFF, 0, sizeof(AFD_BUFF_PARAM));
	memset(stAFDARK.stOUTn_b.nBUFF, 0, sizeof(AFD_BUFF_PARAM));

	stAFDARK.xCNT = 0;	stAFDARK.vCNT &= 0xc0;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_MovShiftBUFF(BYTE nID)
{
	BYTE i;

	for (i=1; i<nID; i++) {
		stAFDARK.stOUTn_h.nBUFF[i-1] = stAFDARK.stOUTn_h.nBUFF[i];
		stAFDARK.stOUTn_b.nBUFF[i-1] = stAFDARK.stOUTn_b.nBUFF[i];

		stAFDARK.stOUTn_h.nFOKS[i-1] = stAFDARK.stOUTn_h.nFOKS[i];
		stAFDARK.stOUTn_b.nFOKS[i-1] = stAFDARK.stOUTn_b.nFOKS[i];
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_GetPrvJudgeBUFF(PAFD_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap; BYTE i; PLONG pBUFF = (PLONG)pFILT->nBUFF;

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

	stAFDARK.fMOV = pFILT->nMOVE/nID;	// average driving distance

	if (pFILT->nSIGN[0]==pFILT->nSIGN[1]) {
		gap = (pFILT->nMOVE)? (13.0f*nID)/pFILT->nMOVE : 1.0f;
	}
	else {
		gap = (1.0f); pFILT->nMIN = pFILT->nMAX = pFILT->FLTn;
	}

	if (ABS(pFILT->nDIFF[0])<30 || ABS(pFILT->nDIFF[1])<30) {
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
	FLOAT rate, gap;	PLONG pBUFF = (PLONG)pFILT->nBUFF;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFDARK.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	pBUFF[nID] = pFILT->FLTn; pFILT->nFOKS[nID] = stAFDARK.fVAL;
	if (nID<3) return;

	pFILT->nMOVE = MAX4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3])
				 - MIN4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3]);

	stAFDARK.fMOV = pFILT->nMOVE/nID;	// average driving distance

	gap = MAX((13.0f*nID)/pFILT->nMOVE, 1.0f);

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
	PLONG pBUFF = (PLONG)pFILT->nBUFF;
	FLOAT rate, gap = 13.0f/stAFDARK.step;

	rate = (pBUFF[2])? ((FLOAT)pFILT->FLTn/pBUFF[2]-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate*gap>0)? 0 : rate*gap-0.5);

	if (pFILT->nDEC<=-1)	pFILT->nCHK |= ( ZERO_DEC);
	else					pFILT->nCHK &= (~ZERO_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_GetDecJudgeBUFF(PAFD_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap;	PLONG pBUFF = (PLONG)pFILT->nBUFF;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFDARK.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	pBUFF[nID] = pFILT->FLTn; pFILT->nFOKS[nID] = stAFDARK.fVAL;
	if (nID<3) return;

	pFILT->nMOVE = MAX4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3])
				 - MIN4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3]);

	stAFDARK.fMOV = pFILT->nMOVE/nID;	// average driving distance

	gap = (13.0f*nID)/pFILT->nMOVE;

	pFILT->zMAX = MAX4(pBUFF[0],pBUFF[1],pBUFF[2],pBUFF[3]);
	pFILT->zMIN = (pBUFF[0]+pBUFF[1]+pBUFF[2]+pBUFF[3]+2)/4;

	pFILT->FLTr = (pFILT->FLTn);
//	pFILT->FLTx = MAX(pFILT->FLTd-MAX((LONG)(5.0f/gap+0.5),5),50);
	pFILT->FLTx = pFILT->FLTd-(LONG)(pFILT->FLTd*0.02f);

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

	if (pFILT->nINC>=(LONG)pFILT->n_UP)	pFILT->nCHK |= ( HIGH_INC|G_INC_OK);
	else								pFILT->nCHK &= (~HIGH_INC);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN)	pFILT->nCHK |= ( HIGH_DEC);
	else								pFILT->nCHK &= (~HIGH_DEC);

	if ((pFILT->nCHK&HIGH_INC)==0)	pFILT->nCHK &= (~(GOOD_INC));
	else if (stAFDARK.DARK==1)	pFILT->nCHK |= ( (GOOD_INC|G_INC_OK));

	if ((pFILT->nCHK&G_INC_OK)==0)	return;

	if (pFILT->FLTd<=(LONG)pFILT->FLTx)	pFILT->nCHK |= ( RATE_DEC|R_DEC_OK);
	else								pFILT->nCHK &= (~RATE_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_InvDecJudgeBUFF(PAFD_FILT_PARAM pFILT, BYTE nID)
{
	PLONG pBUFF = (PLONG)pFILT->nBUFF;
	FLOAT rate, gap = 13.0f/stAFDARK.step;

	rate = (pBUFF[2])? ((FLOAT)pFILT->FLTn/pBUFF[2]-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate*gap>0)? rate*gap : rate*gap-0.5);

	if (pFILT->nDEC<=10)	pFILT->nCHK |= ( ZERO_DEC);
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

	AFDARK_GetDriveRATE(DRV_SEARCH_FAR);	// get judge RATE

	AFDARK_GetPrvJudgeBUFF(&stAFDARK.stOUTn_h, stAFDARK.xCNT);
	AFDARK_GetPrvJudgeBUFF(&stAFDARK.stOUTn_b, stAFDARK.xCNT);

	if (AFDARK_IsDriveEnd()==FALSE) {
		stAFDARK.xCNT++;	if (stAFDARK.xCNT<3) return;
		stAFDARK.vCNT++;
	}

	if (AFDARK_IsDriveEnd()==TRUE) {
				stAFDARK.eCNT++;			AFDARK_ClrJudgeRATE();
		stAFDARK.cur = INV_SEARCH_FAR;	stAFDARK.nxt = PRE_SEARCH_DIR;	xID = 0xa1;
	}
	else if (pFILTx->nCHK&GOOD_DEC && (pFILTb->nCHK&ZERO_INC)==0) {
		stAFDARK.cur = INV_SEARCH_FAR;	stAFDARK.nxt = END_SEARCH_FAR;	xID = 0xa3;
	}
	else if (pFILTx->nCHK&GOOD_INC) {
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;	xID = 0xa5;
	}
	else if ((stAFDARK.vCNT&15)==5) {
		stAFDARK.cur = INV_SEARCH_FAR;	stAFDARK.nxt = PRE_SEARCH_DIR;	xID = 0xa6;
	}
	else {		stAFDARK.xCNT--;			AFDARK_MovShiftBUFF(3);
		stAFDARK.cur = DRV_SEARCH_FAR;	stAFDARK.nxt = DRV_SEARCH_FAR;	xID = 0xa7;
	}

	if ((stAFDARK.vCNT&15)==5 && pFILTb->nCHK&GOOD_DEC) {
		stAFDARK.cur = INV_SEARCH_FAR;	stAFDARK.nxt = END_SEARCH_FAR;	xID = 0xa8;
	}
	if ((stAFDARK.vCNT&15)==5 && pFILTb->nCHK&GOOD_INC) {
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;	xID = 0xa9;
	}

	if (pFILTb->nCHK&ZERO_DEC) stAFDARK.vCNT |= 0x40;

	if (stAFDARK.cur==DRV_CLIMB_TOP2) AFDARK_ClrJudgePEAK();
	if (stAFDARK.cur==INV_SEARCH_FAR) AFDARK_ClrJudgeBUFF();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_GetJudge_NAR(void)
{
	PAFD_FILT_PARAM pFILTx = &stAFDARK.stOUTn_b;
	PAFD_FILT_PARAM pFILTb = &stAFDARK.stOUTn_b;

	if (stAFDARK.vCNT&0x80) pFILTx = &stAFDARK.stOUTn_h;

	AFDARK_GetDriveRATE(DRV_SEARCH_NAR);	// get judge RATE

	AFDARK_GetPrvJudgeBUFF(&stAFDARK.stOUTn_h, stAFDARK.xCNT);
	AFDARK_GetPrvJudgeBUFF(&stAFDARK.stOUTn_b, stAFDARK.xCNT);

	if (AFDARK_IsDriveEnd()==FALSE) {
		stAFDARK.xCNT++;	if (stAFDARK.xCNT<3) return;
		stAFDARK.vCNT++;
	}

	if (AFDARK_IsDriveEnd()==TRUE) {
				stAFDARK.eCNT++;			AFDARK_ClrJudgeRATE();
		stAFDARK.cur = INV_SEARCH_NAR;	stAFDARK.nxt = PRE_SEARCH_DIR;	xID = 0xb1;
	}
	else if (pFILTx->nCHK&GOOD_DEC && (pFILTb->nCHK&ZERO_INC)==0) {
		stAFDARK.cur = INV_SEARCH_NAR;	stAFDARK.nxt = DRV_FORCE_PEAK;	xID = 0xb2;
	}
	else if (pFILTx->nCHK&ZERO_INC || pFILTb->nCHK&ZERO_INC) {
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;	xID = 0xb3;
	}
	else if ((stAFDARK.vCNT&15)==5) {
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;	xID = 0xb4;
	}
	else {		stAFDARK.xCNT--;			AFDARK_MovShiftBUFF(3);
		stAFDARK.cur = DRV_SEARCH_NAR;	stAFDARK.nxt = DRV_SEARCH_NAR;	xID = 0xb5;
	}

	if (pFILTb->nCHK&ZERO_DEC) stAFDARK.vCNT |= 0x40;

	if (stAFDARK.cur==INV_SEARCH_NAR) AFDARK_ClrJudgeBUFF();

	if (stAFDARK.nxt==DRV_FORCE_PEAK && (stAFDARK.vCNT&0x40)==0) {
		stAFDARK.cur = INV_SEARCH_NAR;	stAFDARK.nxt = DRV_CLIMB_TOP2;	xID = 0xb6;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_GetJudge_DIR(void)
{
	PAFD_FILT_PARAM pFILTh = &stAFDARK.stOUTn_h;
	PAFD_FILT_PARAM pFILTb = &stAFDARK.stOUTn_b;

	AFDARK_GetDriveRATE(DRV_SEARCH_DIR);	// get judge RATE

	AFDARK_GetDirJudgeBUFF(&stAFDARK.stOUTn_h, stAFDARK.xCNT);
	AFDARK_GetDirJudgeBUFF(&stAFDARK.stOUTn_b, stAFDARK.xCNT);

	if (AFDARK_IsDriveEnd()==FALSE) {
		stAFDARK.xCNT++; if (stAFDARK.xCNT<4) return;
	}

	if (stAFDARK.xCNT) stAFDARK.xCNT--;	AFDARK_MovShiftBUFF(4);

	if (AFDARK_IsDriveEnd()==TRUE) {
				stAFDARK.eCNT++;			AFDARK_ClrJudgeRATE();
		stAFDARK.cur = INV_SEARCH_DIR;	stAFDARK.nxt = DRV_SEARCH_DIR;	xID = 0xc1;
	}
	else if (pFILTb->nCHK&GOOD_DEC) {
			AFDARK_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFDARK.cur = INV_SEARCH_DIR;	stAFDARK.nxt = PRE_CLIMB_TOP2;	xID = 0xc2;
	}
	else if (pFILTb->nCHK&GOOD_INC) {
			AFDARK_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;	xID = 0xc3;
	}
	else if (pFILTh->nCHK&GOOD_DEC) {
			AFDARK_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFDARK.cur = INV_SEARCH_DIR;	stAFDARK.nxt = PRE_CLIMB_TOP2;	xID = 0xc4;
	}
	else if (pFILTh->nCHK&GOOD_INC) {
			AFDARK_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;	xID = 0xc5;
	}
	else {
			AFDARK_DirDrvIncSPD_BI(DRV_SEARCH_DIR);
		stAFDARK.cur = DRV_SEARCH_DIR;	stAFDARK.nxt = DRV_SEARCH_DIR;	xID = 0xc6;
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

	AFDARK_GetDriveRATE(DRV_SEARCH_DIR);	// get judge RATE

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

	AFDARK_GetDriveRATE(DRV_CLIMB_TOP2);	// get judge RATE

	AFDARK_GetDecJudgeBUFF(&stAFDARK.stOUTn_h, stAFDARK.xCNT);
	AFDARK_GetDecJudgeBUFF(&stAFDARK.stOUTn_b, stAFDARK.xCNT);

	if (AFDARK_IsDriveEnd()==FALSE) {
		stAFDARK.xCNT++; if (stAFDARK.xCNT<4) return;
	}

	if (stAFDARK.xCNT) stAFDARK.xCNT--;	AFDARK_MovShiftBUFF(4);

	if (AFDARK_IsDriveEnd()==TRUE) {
				stAFDARK.eCNT++;			AFDARK_ClrJudgeRATE();
		stAFDARK.cur = INV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;	xID = 0xd1;
	}
	else if (pFILTh->nCHK&GOOD_DEC && pFILTb->nCHK&GOOD_DEC) {
		stAFDARK.cur = INV_CLIMB_TOP2;	stAFDARK.nxt = PRE_CLIMB_PEAK;	xID = 0xd2;
	}
	else if (pFILTb->nCHK&G_INC_OK && pFILTb->nCHK&RATE_DEC) {
			AFDARK_DecDrvDecSPD_GD(DRV_CLIMB_TOP2);
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;	xID = 0xd3;
	}
	else if (pFILTb->nCHK&GOOD_INC) {
			AFDARK_DecDrvIncSPD_GX(DRV_CLIMB_TOP2);
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;	xID = 0xd4;
	}
	else {
			AFDARK_DecDrvIncSPD_BI(DRV_CLIMB_TOP2);
		stAFDARK.cur = DRV_CLIMB_TOP2;	stAFDARK.nxt = DRV_CLIMB_TOP2;	xID = 0xd6;
	}

	if (pFILTh->nCHK&HIGH_INC && pFILTb->nCHK&HIGH_INC) stAFDARK.nSEL = USE_HPF;
	if (pFILTh->nCHK&HIGH_DEC && pFILTb->nCHK&HIGH_DEC) stAFDARK.nSEL = USE_HPF;

	if (pFILTb->nCHK&R_DEC_OK && pFILTb->nCHK&ZERO_DEC) {
		stAFDARK.cur = INV_CLIMB_TOP2;	stAFDARK.nxt = PRE_CLIMB_PEAK;	xID = 0xd7;
	}

	if (pFILTb->nCHK&HIGH_DEC) {
		stAFDARK.cur = INV_CLIMB_TOP2;	stAFDARK.nxt = PRE_CLIMB_PEAK;	xID = 0xd8;
	}
/*
	if (pFILTh->nCHK&Z_DEC_OK || pFILTh->nCHK&Z_INC_OK) stAFDARK.nSEL = USE_HPF;

	if (pFILTh->nCHK&G_INC_OK && pFILTh->nCHK&HIGH_DEC) {
		stAFDARK.cur = INV_CLIMB_TOP2;	stAFDARK.nxt = PRE_CLIMB_PEAK;	xID = 0xd9;
	}
*/
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_InvJudge_DEC(void)
{
	PAFD_FILT_PARAM pFILTx = &stAFDARK.stOUTn_b;

	if (stAFDARK.nxt!=PRE_CLIMB_PEAK) AFDARK_ClrJudgeBUFF();
	if (stAFDARK.nxt!=PRE_CLIMB_PEAK) return;

	AFDARK_GetDriveRATE(DRV_CLIMB_TOP2);	// get judge RATE

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

	AFDARK_GetDriveRATE(DRV_CLIMB_PEAK);	// get judge RATE

	AFDARK_GetTopJudgeBUFF(&stAFDARK.stOUTn_h, stAFDARK.xCNT);
	AFDARK_GetTopJudgeBUFF(&stAFDARK.stOUTn_b, stAFDARK.xCNT);

	if (AFDARK_IsDriveEnd()==TRUE) {
		stAFDARK.cur = INV_CLIMB_PEAK;	stAFDARK.nxt = DRV_FORCE_PEAK;	xID = 0xe1;
	}
	else if (pFILTx->nCHK&GOOD_DEC) {
		stAFDARK.cur = INV_CLIMB_PEAK;	stAFDARK.nxt = DRV_FORCE_PEAK;	xID = 0xe2;
	}
	else {
		stAFDARK.cur = DRV_CLIMB_PEAK;	stAFDARK.nxt = DRV_CLIMB_PEAK;	xID = 0xe3;
	}

	if (AFDARK_IsDriveEnd()==TRUE)	stAFDARK.eCNT = 2;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_InvJudge_TOP(void)
{
	PAFD_FILT_PARAM pFILTx = &stAFDARK.stOUTn_b;

	if (stAFDARK.nSEL==USE_HPF) pFILTx = &stAFDARK.stOUTn_h;

//	if (AFDARK_IsDriveEnd()==TRUE) return;
	if (stAFDARK.eCNT==2) return;
	AFDARK_GetDriveRATE(DRV_CLIMB_PEAK);	// get judge RATE

	AFDARK_InvTopJudgeBUFF(&stAFDARK.stOUTn_h, stAFDARK.xCNT);
	AFDARK_InvTopJudgeBUFF(&stAFDARK.stOUTn_b, stAFDARK.xCNT);

	if (pFILTx->nCHK&ZERO_DEC) return;

	stAFDARK.nxt = DRV_CLIMB_PEAK;
	AFDARK_SetChangeDIR();
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFDARK_InvJudge_END(void)
{
	PAFD_FILT_PARAM pFILTx = &stAFDARK.stOUTn_b;

	if (stAFDARK.nSEL==USE_HPF) pFILTx = &stAFDARK.stOUTn_h;

	if (stAFDARK.eCNT==2) return FALSE;
	AFDARK_GetDriveRATE(DRV_CLIMB_PEAK);	// get judge RATE

	AFDARK_InvEndJudgeBUFF(&stAFDARK.stOUTn_h, USE_HPF);
	AFDARK_InvEndJudgeBUFF(&stAFDARK.stOUTn_b, USE_BPF);

	return (pFILTx->nCHK&GOOD_END)? FALSE : TRUE;
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
	stAFDARK.eCNT = 0; stAFDARK.cur = END_FILTER_SEL;
	stAFDARK.vCNT = 0; stAFDARK.nxt = END_FILTER_SEL;

	memset(&stAFDARK.stSTEP_m, 0, sizeof(AFD_STEP_PARAM));
	stAFDARK.stSTEP_m.nMIN = LENS_GetAFDriveSTEP(DRV_STEP_MIN);
	stAFDARK.stSTEP_m.nSTD = LENS_GetAFDriveSTEP(DRV_STEP_STD);
	stAFDARK.stSTEP_m.nMID = LENS_GetAFDriveSTEP(DRV_STEP_MID);
	stAFDARK.stSTEP_m.nMAX = LENS_GetAFDriveSTEP(DRV_STEP_MAX);
	stAFDARK.stSTEP_m.nFWD = LENS_GetAFDriveSTEP(DRV_STEP_FWD);
	stAFDARK.stSTEP_m.nBWD = LENS_GetAFDriveSTEP(DRV_STEP_BWD);
	stAFDARK.stSTEP_m.nTOP = LENS_GetAFDriveSTEP(DRV_STEP_TOP);

	if (stAFDARK.nSEL==USE_HPF) stAFDARK.vCNT |= 0x80;
	stAFDARK.nSEL = USE_BPF;	AFDARK_ClrJudgeBUFF();

	memset(&stAFDARK.stOUTn_h.nMIN, 0, sizeof(AFD_RATE_PARAM));
	memset(&stAFDARK.stOUTn_b.nMIN, 0, sizeof(AFD_RATE_PARAM));

	stAFDARK.stNORM_y.nRST = 0;		// clear reset LUMA-trig
	stAFDARK.stNORM_y.nERR = 0;		// clear error LUMA-trig
	stAFDARK.stNORM_y.nCNT = 0;		// clear count LUMA-trig

	stAFDCTL.n_GAIN = 0;	// clear normalized reference GAIN
	stAFDCTL.n_YSUM = 0;	// clear normalized reference YSUM
	stAFDCTL.n_FLTr = 0;	// clear normalized reference FLTr
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_EndFilterSEL(void)
{
	if (stAFDARK.cur!=END_FILTER_SEL) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFDARK.cur = PRE_SEARCH_MOV;
	stAFDARK.nxt = PRE_SEARCH_MOV;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_PreSearchMOV(void)
{
	if (stAFDARK.cur!=PRE_SEARCH_MOV) return;

	stAFDARK.stOUTn_b.xHIGH = stAFDARK.stOUTn_b.FLTn;
	stAFDARK.stOUTn_h.xHIGH = stAFDARK.stOUTn_h.FLTn;

	stAFDARK.cur = DRV_SEARCH_MOV;
	stAFDCTL.n_GAIN = 0;	// clear normalized reference GAIN
	stAFDCTL.n_YSUM = 0;	// clear normalized reference YSUM
	stAFDCTL.n_FLTr = 0;	// clear normalized reference FLTr
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DrvSearchMOV(void)
{
	if (stAFDARK.cur!=DRV_SEARCH_MOV) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFDARK.cur = PRE_SEARCH_FAR;
	stAFDARK.nxt = PRE_SEARCH_FAR;

	AFDARK_ClrJudgeRATE();	// clear judge RATE
	AFDARK_ClrJudgePEAK();	// clear judge PEAK
	AFDARK_ClrJudgeINFO();	// clear judge INFO
	AFDARK_GetJudge_FAR();	// get judge INFO

	AFDARK_GetDriveSTEP(DRV_STEP_MIN);
	AFDARK_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_PreSearchFAR(void)
{
	if (stAFDARK.cur!=PRE_SEARCH_FAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFDARK.cur = DRV_SEARCH_FAR;
	stAFDARK.nxt = DRV_SEARCH_FAR;

	AFDARK_GetJudge_FAR();	// get judge INFO

	AFDARK_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DrvSearchFAR(void)
{
	if (stAFDARK.cur!=DRV_SEARCH_FAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFDARK_GetJudge_FAR();	// using HPFx & LPFx

	if (stAFDARK.cur==INV_SEARCH_FAR) return;
	AFDARK_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
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
	AFDARK_SetDriveSTEP(DRV_STEP_REM);	// motor driving
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

	AFDARK_SetDriveSTEP(DRV_STEP_REM);	// motor driving
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

	AFDARK_ClrJudgeRATE();	// clear judge RATE
	AFDARK_ClrJudgePEAK();	// clear judge PEAK
	AFDARK_ClrJudgeINFO();	// clear judge INFO
	AFDARK_GetJudge_NAR();	// get judge INFO

	AFDARK_GetDriveSTEP(DRV_STEP_MIN);
	AFDARK_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DrvSearchNAR(void)
{
	if (stAFDARK.cur!=DRV_SEARCH_NAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFDARK_GetJudge_NAR();	// using HPFx & LPFx

	if (stAFDARK.cur==INV_SEARCH_NAR) return;
	AFDARK_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
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
	AFDARK_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_PreSearchDIR(void)
{
	if (stAFDARK.cur!=PRE_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFDARK.cur = DRV_SEARCH_DIR;
	stAFDARK.nxt = DRV_SEARCH_DIR;

	AFDARK_GetJudge_DIR();	// get judge INFO

	AFDARK_GetDriveSTEP(DRV_STEP_STD);
	AFDARK_SetDriveSTEP(DRV_STEP_STD);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DrvSearchDIR(void)
{
	if (stAFDARK.cur!=DRV_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFDARK_GetJudge_DIR();	// using HPFx & LPFx

	if (stAFDARK.cur==INV_SEARCH_DIR) return;
	AFDARK_SetDriveSTEP(DRV_STEP_STD);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_EndSearchDIR(void)
{
	if (stAFDARK.cur!=END_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFDARK.nSEL==USE_HPF)
		 stAFDARK.peak = stAFDARK.stOUTn_h.nPEAK;
	else stAFDARK.peak = stAFDARK.stOUTn_b.nPEAK;

	AFDARK_InvJudge_DIR();	// using HPFx & LPFx

	if (stAFDARK.eCNT==2)
		 stAFDARK.cur = DRV_FORCE_PEAK;
	else stAFDARK.cur = stAFDARK.nxt;

	if (stAFDARK.cur==DRV_FORCE_PEAK) return;
	AFDARK_SetDriveSTEP(DRV_STEP_PRV);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_PreClimbTOP2(void)
{
	if (stAFDARK.cur!=PRE_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFDARK.cur = DRV_CLIMB_TOP2;
	stAFDARK.nxt = DRV_CLIMB_TOP2;

	AFDARK_ClrJudgeRATE();	// clear judge RATE
	AFDARK_ClrJudgeINFO();	// clear judge INFO
	AFDARK_GetJudge_DEC();	// get judge INFO

	AFDARK_SetDriveSTEP(DRV_STEP_PRV);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DrvClimbTOP2(void)
{
	if (stAFDARK.cur!=DRV_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFDARK_GetJudge_DEC();	// using HPFx & LPFx

	if (stAFDARK.cur==INV_CLIMB_TOP2) return;
	AFDARK_SetDriveSTEP(DRV_STEP_PRV);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_EndClimbTOP2(void)
{
	WORD step = stAFDARK.step/2;

	if (stAFDARK.cur!=END_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFDARK.nSEL==USE_HPF)
		 stAFDARK.peak = stAFDARK.stOUTn_h.nPEAK;
	else stAFDARK.peak = stAFDARK.stOUTn_b.nPEAK;

	if (stAFDARK.fVAL==stAFDARK.peak) step = 0;

	if (stAFDARK.nDIR==fFAR) stAFDARK.peak -= step;
	else					 stAFDARK.peak += step;

	stAFDARK.peak = MAX(stAFDARK.peak,stAFDARK.fMIN);
	stAFDARK.peak = MIN(stAFDARK.peak,stAFDARK.fMAX);

	AFDARK_InvJudge_DEC();	// using HPFx & LPFx

	if (stAFDARK.eCNT==2)
		 stAFDARK.cur = DRV_FORCE_PEAK;
	else stAFDARK.cur = stAFDARK.nxt;

	if (stAFDARK.cur==DRV_FORCE_PEAK) return;
	if (stAFDARK.cur==PRE_CLIMB_PEAK && stAFDARK.fVAL==stAFDARK.peak) return;
	if (stAFDARK.cur==DRV_CLIMB_TOP2 && stAFDARK.fVAL==stAFDARK.peak) return;

	AFDARK_GetDriveSTEP(DRV_STEP_REM);
	AFDARK_SetDriveSTEP(DRV_STEP_REM);	// motor driving
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

	AFDARK_SetDriveSTEP(DRV_STEP_REM);	// motor driving
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

	AFDARK_ClrJudgeRATE();	// clear judge RATE
	AFDARK_ClrJudgePEAK();	// clear judge PEAK
	AFDARK_ClrJudgeINFO();	// clear judge INFO
	AFDARK_GetJudge_TOP();	// get judge INFO

	if (stAFDARK.stNORM_y.nERR) return;

	AFDARK_GetDriveSTEP(DRV_STEP_TOP);
	AFDARK_SetDriveSTEP(DRV_STEP_TOP);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DrvClimbPEAK(void)
{
	if (stAFDARK.cur!=DRV_CLIMB_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFDARK_GetJudge_TOP();	// using HPFx & LPFx

	if (stAFDARK.cur==INV_CLIMB_PEAK) return;
	AFDARK_SetDriveSTEP(DRV_STEP_TOP);	// motor driving
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
		 stAFDARK.peak += 0*stAFDARK.stSTEP_m.nTOP;
	else stAFDARK.peak -= 0*stAFDARK.stSTEP_m.nTOP;

	AFDARK_InvJudge_TOP();	// using HPFx & LPFx

	stAFDARK.cur = stAFDARK.nxt;

	if (stAFDARK.cur==DRV_FORCE_PEAK) return;
	AFDARK_SetDriveSTEP(DRV_STEP_TOP);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DrvForcePEAK(void)
{
	if (stAFDARK.cur!=DRV_FORCE_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFDARK.nDIR==fFAR) stAFDARK.peak += stAFDCTL.h_FOKS;
	else					 stAFDARK.peak -= stAFDCTL.h_FOKS;

	stAFDARK.cur = END_FORCE_PEAK;
	AFDARK_SetDrivePEAK();	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_EndForcePEAK(void)
{
	if (stAFDARK.cur!=END_FORCE_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFDARK.cur = END_CLEAR_WAIT;	// wait progress
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_EndDriveWAIT(void)
{
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	switch (stAFDARK.cur) {
		case INV_FILTER_SEL: stAFDARK.cur = END_FILTER_SEL; break;
		case INV_SEARCH_FAR: stAFDARK.cur = END_SEARCH_FAR; break;
		case INV_SEARCH_NAR: stAFDARK.cur = END_SEARCH_NAR; break;
		case INV_SEARCH_DIR: stAFDARK.cur = END_SEARCH_DIR; break;
		case INV_CLIMB_TOP1: stAFDARK.cur = END_CLIMB_TOP1; break;
		case INV_CLIMB_TOP2: stAFDARK.cur = END_CLIMB_TOP2; break;
		case INV_CLIMB_PEAK: stAFDARK.cur = END_CLIMB_PEAK; break;
		case INV_SWING_SCAN: stAFDARK.cur = END_SWING_SCAN; break;
		default:			 return;
	}
	AFDARK_SetChangeDIR();	// change direction
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

	stAFDARK.cur = END_CLEAR_PROC;	// stop progress

	if (stAFDARK.stNORM_y.nRST==TRUE) return;
	AF_SetAUTOInfo(pCTRL, stAFDARK.stNORM_y.nCHK);	// set AUTO

	if (stAFDARK.ZOOM<=stAFDARK.KNEE) return;
	LENS_SetTrackingINFO(stAFDARK.peak);	// update Track-info
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_EndClearWAIT(PAF_CTRL_PARAM pCTRL)
{
	BOOL nID;

	if (stAFDARK.cur!=END_CLEAR_WAIT) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	nID = (AFDARK_InvJudge_END())? TRUE : FALSE;
	stAFDARK.stNORM_y.nRST |= AF_GetDATAInfo(pCTRL,nID);

	stAFDARK.cur = END_CLEAR_AUTO;	// auto progress
	LENS_EnableMotorFOKS(OFF);
}

#if __MISP100_AF_DBG__ == 1
//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_HEADPrint(void)
{
	UARTprintf("\r\nAFDARK PROCESS TRACE...");
	UARTprintf("\r\n No   RPFo     BPFo     HPFo    YCNT   YSUM   DIFF  c  GAIN  v ");
//				    000 00000000 00000000 00000000 000000 000000 000000 0 000000 0 
	UARTprintf(" n_FLTr   n_FLTb    RPFn   ");
//				00000000 00000000 00000000 

	UARTprintf(" HPFx   ");
//				000000
//	UARTprintf(" HPFx    HPFn   h_zINC h_nINC h_zDEC h_nDEC  hCHK  ");
//				000000 00000000 000000 000000 000000 000000 000000 

//	UARTprintf(" BPFx    BPFn   b_zINC b_nINC b_zDEC b_nDEC  bCHK  ");
//				00000000 000000 000000 000000 000000 000000 
	UARTprintf(" fPeak  fVAL F D E STP PR|CU|NT  zoom n xV ID ");
//				00000 00000 0 0 0 000 00000000 00000 0 00 00

	UARTprintf("%02d %02d\r\n", eID, count_16ms);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_PROCPrint(void)
{
	UARTprintf("%03d ",		stAFDARK.time);
	UARTprintf("%08d ",		stAFDARK.stDATA_r.FLTo);
	UARTprintf("%08d ",		stAFDARK.stDATA_b.FLTo);
	UARTprintf("%08d ",		stAFDARK.stDATA_h.FLTo);

	UARTprintf("%06d ",		stAFDARK.stNORM_y.YCNT);
	UARTprintf("%06d ",		stAFDARK.stNORM_y.YSUM);
	UARTprintf("%06d ",		stAFDARK.stNORM_y.YSUM-stAFDARK.stNORM_y.YPRV);
	UARTprintf("%01X ",		stAFDARK.stNORM_y.nRST);
	UARTprintf("%06d ",		stAFDARK.stNORM_y.GAIN);
	UARTprintf("%01X ",		stAFDARK.stNORM_y.nCHK);

	UARTprintf("%08d ",		stAFDCTL.n_FLTr);
	UARTprintf("%08d ",		stAFDARK.stNORM_y.FLTb);
	UARTprintf("%08d ",		stAFDARK.stDATA_r.FLTn);

//	UARTprintf("%06d ",		stAFDARK.stOUTn_h.FLTx);
	UARTprintf("%08d ",		stAFDARK.stOUTn_h.FLTn);
//	UARTprintf("%06d ",		stAFDARK.stOUTn_h.zINC);
//	UARTprintf("%06d ",		stAFDARK.stOUTn_h.nINC);
//	UARTprintf("%06d ",		stAFDARK.stOUTn_h.zDEC);
//	UARTprintf("%06d ",		stAFDARK.stOUTn_h.nDEC);
//	UARTprintf("%06X ",		stAFDARK.stOUTn_h.nCHK);

//	UARTprintf("%06d ",		stAFDARK.stOUTn_b.FLTx);
//	UARTprintf("%08d ",		stAFDARK.stOUTn_b.FLTn);
//	UARTprintf("%06d ",		stAFDARK.stOUTn_b.zINC);
//	UARTprintf("%06d ",		stAFDARK.stOUTn_b.nINC);
//	UARTprintf("%06d ",		stAFDARK.stOUTn_b.zDEC);
//	UARTprintf("%06d ",		stAFDARK.stOUTn_b.nDEC);
//	UARTprintf("%06X ",		stAFDARK.stOUTn_b.nCHK);

	UARTprintf("%05d ",		stAFDARK.peak);
	UARTprintf("%05d ",		stAFDARK.fVAL);
	UARTprintf("%01d ",		stAFDARK.nSEL);
	UARTprintf("%01d ",		stAFDARK.nDIR);
	UARTprintf("%01d ",		stAFDARK.eCNT);
	UARTprintf("%03d ",		stAFDARK.step);
	UARTprintf("%02X|%02X|%02X ", stAFDARK.prv,stAFDARK.cur,stAFDARK.nxt);
	UARTprintf("%05d ",		stAFDARK.zVAL);
	UARTprintf("%01d ",		stAFDARK.stSTEP_m.nCHK);//stAFDARK.xCNT);
//	UARTprintf("%07d ",		stAFDARK.stOUTn_h.xHIGH);
//	UARTprintf("%07d ",		stAFDARK.stOUTn_b.xHIGH);
	UARTprintf("%02X ",		stAFDARK.vCNT);
	UARTprintf("%02X ",		xID);

	UARTprintf("%02d %02d\r\n", eID, count_16ms);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFDARK_DBGPrint(void)
{
	if (stAFDARK.time)	AFDARK_PROCPrint();
	else				AFDARK_HEADPrint();

	stAFDARK.time++;
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
void AFDARK_ProcessHandler(PAF_CTRL_PARAM pCTRL)
{
	if (stAFDARK.cur==0) return;
	stAFDARK.prv = stAFDARK.cur;	// for debug print

	AFDARK_GetAFOPDWIND(pCTRL);		// get AFOPD data
	AFDARK_EndClearLUMA(pCTRL);		// chk LUMA info
	AFDARK_EndClearAUTO(pCTRL);		// set AUTO info
	AFDARK_EndClearWAIT(pCTRL);		// set AUTO wait

	AFDARK_PreFilterSEL();		// select filter
	AFDARK_PreSearchMOV();		// search wait-luma
	AFDARK_PreSearchFAR();		// search shake-far
	AFDARK_PreSearchNAR();		// search shake-nar
	AFDARK_PreSearchDIR();		// search direction
	AFDARK_PreClimbTOP2();		// search climb-dec
	AFDARK_PreClimbPEAK();		// search fine-peak

	AFDARK_DrvSearchMOV();		// drive force-mov
	AFDARK_DrvSearchFAR();		// drive shake-far
	AFDARK_DrvSearchNAR();		// drive shake-nar
	AFDARK_DrvSearchDIR();		// drive direction
	AFDARK_DrvClimbTOP2();		// drive climb-dec
	AFDARK_DrvClimbPEAK();		// drive fine-peak
	AFDARK_DrvForcePEAK();		// drive force-top

	AFDARK_EndFilterSEL();		// arrange filter
	AFDARK_EndSearchFAR();		// arrange shake-far
	AFDARK_EndSearchNAR();		// arrange shake-nar
	AFDARK_EndSearchDIR();		// arrange direction
	AFDARK_EndClimbTOP2();		// arrange climb-dec
	AFDARK_EndClimbPEAK();		// arrange fine-peak

	AFDARK_EndForcePEAK();		// wait force-peak
	AFDARK_EndDriveWAIT();		// wait drive-stop
	eID = count_16ms;

#if __MISP100_AF_DBG__ == 1
	if ((ODM_GetDEBUGMode()!=0x03) && (ODM_GetDEBUGMode()!=0x05)) return;
	AFDARK_DBGPrint();
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void AFDARK_EnableProcess(BOOL OnOff)
{
	if (OnOff)	stAFDARK.cur = PRE_FILTER_SEL;
	else		stAFDARK.cur = END_CLEAR_PROC;

	stAFDARK.time = 0;	// for debug print
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AFDARK_IsProcessExecuting(void)
{
	return (stAFDARK.cur==0)? FALSE : TRUE;
}

#endif

/*  FILE_END_HERE */
