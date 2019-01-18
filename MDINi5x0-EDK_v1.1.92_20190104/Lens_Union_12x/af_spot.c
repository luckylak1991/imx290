// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if defined(__USE_AFZOOM_LENS__)
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

}	AFS_BUFF_PARAM, *PAFS_BUFF_PARAM;

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

}	AFS_RATE_PARAM, *PAFS_RATE_PARAM;

typedef	struct
{
	LONG	nHIGH;			// peak-filt-val of HPFn in norm
	SHORT	nPEAK;			// peak-position of HPFn in norm
	SHORT	nDUMY;			// dummy

}	AFS_PEAK_PARAM, *PAFS_PEAK_PARAM;

typedef	struct
{
	LONG	YSUM;			// original YSUM data
	LONG	YPRV;			// previous YSUM data
	LONG	YREF;			// original YREF data
	LONG	GAIN;			// original GAIN data
	LONG	YCNT;			// original YCNT data
	LONG	FLTd;			// threshold FILT data

	BYTE	nCHK;			// conditional min check
	BYTE	nRST;			// conditional rst check
	BYTE	nERR;			// conditional err check
	BYTE	nCNT;			// conditional cnt check

}	AFS_NORM_PARAM, *PAFS_NORM_PARAM;

typedef	struct
{
	LONG	FLTa;			// original FILT-A data
	LONG	FLTb;			// original FILT-B data
	LONG	FLTo;			// original FILTA+FILTB
	LONG	FLTn;			// normalized FILT data

}	AFS_DATA_PARAM, *PAFS_DATA_PARAM;

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

}	AFS_FILT_PARAM, *PAFS_FILT_PARAM;

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

}	AFS_STEP_COUNT, *PAFS_STEP_COUNT;

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

	AFS_STEP_COUNT	stDIR_I;
	AFS_STEP_COUNT	stDEC_I;
	AFS_STEP_COUNT	stDEC_D;

}	AFS_STEP_PARAM, *PAFS_STEP_PARAM;

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
	SHORT	fABV;			// AF control focus above max value
	SHORT	fBLO;			// AF control focus below min value
	SHORT	fGAP;			// AF control focus range (FAR-NEAR)
	SHORT	fMOV;			// AF control focus moving diatance
	SHORT	zVAL;			// AF control zVAL position data
	SHORT	ZOOM;			// AF control zoom position data
	SHORT	KNEE;			// AF control knee position data
	WORD	time;			// AF control execution time
	BYTE	nMOD;			// dummy
	CHAR	nCHK;			// dummy

	AFS_NORM_PARAM	stNORM_y;	// data normalize Y-info
	AFS_DATA_PARAM	stDATA_r;	// data normalization RPF
	AFS_DATA_PARAM	stDATA_l;	// data normalization LPF
	AFS_DATA_PARAM	stDATA_v;	// data normalization VPF
	AFS_DATA_PARAM	stDATA_b;	// data normalization BPF
	AFS_DATA_PARAM	stDATA_h;	// data normalization HPF
	AFS_FILT_PARAM	stOUTn_b;	// filter compesation BPF
	AFS_FILT_PARAM	stOUTn_h;	// filter compesation HPF
	AFS_STEP_PARAM	stSTEP_m;	// driving focus step

}	AFS_PROC_PARAM, *PAFS_PROC_PARAM;

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
	PRE_SEARCH_FIT = 0xc0, DRV_SEARCH_FIT, INV_SEARCH_FIT, END_SEARCH_FIT,
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
	SHORT	n_UP;			// threshold min-data of min-rate
	SHORT	n_DN;			// threshold max-data of max-rate
	SHORT	z_UP;			// threshold min-data of min-zero
	SHORT	z_DN;			// threshold max-data of max-zero

}	AFS_TUNE_RATIO, *PAFS_TUNE_RATIO;

typedef	struct
{
	BYTE	r_MIN;			// ref-count of MIN-SPD up/dn
	BYTE	r_STD;			// ref-count of STD-SPD up/dn
	BYTE	r_MID;			// ref-count of MID-SPD up/dn
	BYTE	r_MAX;			// ref-count of MAX-SPD up/dn

}	AFS_TUNE_COUNT, *PAFS_TUNE_COUNT;

typedef	struct
{
	LONG	n_GAIN;			// normalized reference GAIN
	LONG	n_YSUM;			// normalized reference YSUM
	//LONG	n_FLTr;			// normalized reference FLTr

	LONG	c_FLTr;			// conditional reference FLTr
	//LONG	c_FLTx;			// conditional reference FLTx
	//LONG	c_GMIN;			// conditional reference GMIN
	//LONG	c_GMAX;			// conditional reference GMAX

	SHORT	r_LOCI;			// conditional reference LOCI
	SHORT	r_DUMY;			// dummy

	//SHORT	h_ZOOM;			// hysteresis step of ZOOM
	SHORT	h_FOKS;			// hysteresis step of FOKS

	AFS_TUNE_RATIO	stPRV_b;		// shake-prv rate of BPF
	AFS_TUNE_RATIO	stPRV_h;		// shake-prv rate of HPF

	AFS_TUNE_RATIO	stDIR_b;		// direction rate of BPF
	AFS_TUNE_RATIO	stDIR_h;		// direction rate of HPF

	AFS_TUNE_RATIO	stDEC_b;		// climb-dec rate of BPF
	AFS_TUNE_RATIO	stDEC_h;		// climb-dec rate of HPF

	AFS_TUNE_RATIO	stTOP_b;		// fine-peak rate of BPF
	AFS_TUNE_RATIO	stTOP_h;		// fine-peak rate of HPF

}	AFS_TUNE_PARAM, *PAFS_TUNE_PARAM;

#define		MIN3(a,b,c)				(MIN(MIN(a,b),c))
#define		MAX3(a,b,c)				(MAX(MAX(a,b),c))

#define		MIN4(a,b,c,d)			(MIN(MIN(a,b),MIN(c,d)))
#define		MAX4(a,b,c,d)			(MAX(MAX(a,b),MAX(c,d)))

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static AFS_PROC_PARAM stAFSPOT;
static AFS_TUNE_PARAM stAFSCTL = {
//	n_GAIN	n_YSUM	c_FLTr	
	     0,		0,		2000,
//	n_GAIN	n_YSUM	n_FLTr	c_FLTr	c_FLTx	c_GMIN  c_GMAX
//	     0,		0,		0,		4000,	0,		8191,	65535,

//	r_LOCI  r_DUMY   h_FOKS
	     0,      0,           0,
//	r_LOCI  r_DUMY  h_ZOOM  h_FOKS
//	     0,      0,      0,      0,

//   n_UP    n_DN    z_UP    z_DN	for shake-prv  //stPRV_b
	{  30,    -30,      1,     -1},	// BPF
	{  30,    -30,      1,     -1},	// HPF

//   n_UP    n_DN    z_UP    z_DN	for direction //stDIR_b
	{  50,    -50,      1,     -5},	// BPF
	{  50,    -50,      1,     -5},	// HPF

//   n_UP    n_DN    z_UP    z_DN	for climb-dec //stDEC_b
	{  50,    -50,      1,     -5},	// BPF
	{  50,    -50,      1,     -5},	// HPF

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

	// get normalized FILT data
	if (stAFSCTL.n_GAIN==0) stAFSCTL.n_GAIN = stAFSPOT.stNORM_y.GAIN;
		nGAIN = ((FLOAT)stAFSPOT.stNORM_y.GAIN)/stAFSCTL.n_GAIN;

	if (stAFSCTL.n_YSUM==0) stAFSCTL.n_YSUM = stAFSPOT.stNORM_y.YSUM;
		nYSUM = ((FLOAT)stAFSCTL.n_YSUM)/stAFSPOT.stNORM_y.YSUM;
		nYSUM = ((FLOAT)nGAIN*nYSUM*nYSUM);

	stAFSPOT.stDATA_b.FLTo += stAFSPOT.stDATA_h.FLTb+stAFSPOT.stDATA_v.FLTb;

	stAFSPOT.stDATA_b.FLTn = (LONG)(1.00f*stAFSPOT.stDATA_b.FLTo+0.5);
	stAFSPOT.stDATA_h.FLTn = (LONG)(nYSUM*stAFSPOT.stDATA_h.FLTo+0.5);

	stAFSPOT.stOUTn_b.FLTn = (LONG)(1.00f*stAFSPOT.stDATA_b.FLTn+0.5);
	stAFSPOT.stOUTn_h.FLTn = (LONG)(1.00f*stAFSPOT.stDATA_h.FLTn+0.5);

	// get threshold FILT data
	nFLTr = (FLOAT)sqrt(stAFSPOT.stNORM_y.GAIN/256.0f);
	stAFSPOT.stNORM_y.FLTd = (LONG)(nFLTr*stAFSCTL.c_FLTr+0.5);
	//UARTprintf("stAFSPOT.stNORM_y.GAIN : %d / nFLTr : %f / stAFSCTL.c_FLTr : %d\r\n", stAFSPOT.stNORM_y.GAIN, nFLTr, stAFSCTL.c_FLTr);

	// get current position
	stAFSPOT.nDIR = LENS_GetDirectionFOKS();
	stAFSPOT.fVAL = LENS_GetCurPositionFOKS();
	stAFSPOT.zVAL = LENS_GetCurPositionZOOM();
	stAFSPOT.ZOOM = LENS_GetRelPositionZOOM();
	stAFSPOT.KNEE = LENS_GetKneePositionZOOM();
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFSPOT_IsDriveEnd(void)
{
	if (stAFSPOT.nDIR==fNEAR && stAFSPOT.fVAL<=stAFSPOT.fMIN) {
		//UARTprintf("fNEAR end... stAFSPOT.fVAL : %d / stAFSPOT.fMIN : %d\r\n", stAFSPOT.fVAL, stAFSPOT.fMIN);
		return TRUE;
	}
	if (stAFSPOT.nDIR== fFAR && stAFSPOT.fVAL>=stAFSPOT.fMAX) {
		//UARTprintf("fFAR end... stAFSPOT.fVAL : %d / stAFSPOT.fMAX : %d\r\n", stAFSPOT.fVAL, stAFSPOT.fMAX);
		return TRUE;
	}
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
	WORD max = (WORD)stAFSPOT.stSTEP_m.nBWD;
	WORD min = (WORD)stAFSPOT.stSTEP_m.nMIN;

	switch (nID) {
		case DRV_STEP_MIN:	stAFSPOT.step = (WORD)stAFSPOT.stSTEP_m.nMIN; break;
		case DRV_STEP_STD:	stAFSPOT.step = (WORD)stAFSPOT.stSTEP_m.nSTD; break;
		case DRV_STEP_MID:	stAFSPOT.step = (WORD)stAFSPOT.stSTEP_m.nMID; break;
		case DRV_STEP_MAX:	stAFSPOT.step = (WORD)stAFSPOT.stSTEP_m.nMAX; break;

		case DRV_STEP_FWD:	stAFSPOT.step = (WORD)stAFSPOT.stSTEP_m.nFWD; break;
		case DRV_STEP_BWD:	stAFSPOT.step = (WORD)stAFSPOT.stSTEP_m.nBWD; break;
		case DRV_STEP_TOP:	stAFSPOT.step = (WORD)stAFSPOT.stSTEP_m.nTOP; break;

		case DRV_STEP_DIV:	stAFSPOT.step = MIN(MAX((gap+3)/4,min), max); break;
		case DRV_STEP_REM:	stAFSPOT.step = (quo)? stAFSPOT.step : rem; break;
	}

	if ((nID&0xf0)==DRV_STEP_MIN) stAFSPOT.stSTEP_m.nSPD = nID;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetDriveRATE(BYTE nID)
{
	PBYTE pTUNEb = NULL, pTUNEh = NULL;

	switch (nID) {
		case DRV_SEARCH_FIT: case PRE_SEARCH_FIT:
		case DRV_SEARCH_FAR: case PRE_SEARCH_FAR:
		case DRV_SEARCH_NAR: case PRE_SEARCH_NAR:
			pTUNEb = (PBYTE)&stAFSCTL.stPRV_b;	pTUNEh = (PBYTE)&stAFSCTL.stPRV_h;	break;

		case DRV_SEARCH_DIR: case PRE_SEARCH_DIR:
			pTUNEb = (PBYTE)&stAFSCTL.stDIR_b;	pTUNEh = (PBYTE)&stAFSCTL.stDIR_h;	break;

		case DRV_CLIMB_TOP2: case PRE_CLIMB_TOP2:
			pTUNEb = (PBYTE)&stAFSCTL.stDEC_b;	pTUNEh = (PBYTE)&stAFSCTL.stDEC_h;	break;

		case DRV_CLIMB_PEAK: case PRE_CLIMB_PEAK:
			pTUNEb = (PBYTE)&stAFSCTL.stTOP_b;	pTUNEh = (PBYTE)&stAFSCTL.stTOP_h;	break;
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

	stAFSPOT.goal = MIN(stAFSPOT.goal, stAFSPOT.fMAX);	// check range
	stAFSPOT.goal = MAX(stAFSPOT.goal, stAFSPOT.fMIN);

	LENS_SetDriveFOKS(stAFSPOT.goal);	// start motor drive
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_SetDrivePEAK(void)
{
	stAFSPOT.goal = stAFSPOT.peak;

	stAFSPOT.goal = MIN(stAFSPOT.goal, stAFSPOT.fMAX);	// check range
	stAFSPOT.goal = MAX(stAFSPOT.goal, stAFSPOT.fMIN);

	LENS_SetDriveFOKS(stAFSPOT.goal);	// start motor drive
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_ClrDrvCountSPD(PAFS_STEP_COUNT pCNT)
{
	SHORT nFWD = LENS_GetAFDriveSTEP(DRV_STEP_FWD);
	LONG  GAIN = stAFSPOT.stNORM_y.GAIN, KNEE = MIN(GAIN,8192);

	if (GAIN>1023) nFWD -= (SHORT)(6.0f*(FLOAT)KNEE/ 1024+0.5);
	if (GAIN>8191) nFWD -= (SHORT)(6.0f*(FLOAT)GAIN/16384+0.5);
	stAFSPOT.stSTEP_m.nFWD = MAX(nFWD,stAFSPOT.stSTEP_m.nSTD*3);

	switch (stAFSPOT.stSTEP_m.nSPD) {
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

	AFSPOT_ClrDrvCountSPD(pCNT);	// clear counter

	switch (stAFSPOT.stSTEP_m.nSPD) {
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

	AFSPOT_ClrDrvCountSPD(pCNT);	// clear counter
	memset(&stAFSPOT.stSTEP_m.stDEC_D, 0, sizeof(AFS_STEP_COUNT));

	if (stAFSPOT.stSTEP_m.nSPD==DRV_STEP_MAX) stAFSPOT.stSTEP_m.nCHK = 1;
	if (stAFSPOT.stSTEP_m.nSPD==DRV_STEP_FWD) stAFSPOT.stSTEP_m.nCHK = 1;

	if (stAFSPOT.stSTEP_m.nCHK) {
		switch (stAFSPOT.stSTEP_m.nSPD) {
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
		switch (stAFSPOT.stSTEP_m.nSPD) {
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

	AFSPOT_ClrDrvCountSPD(pCNT);	// clear counter
	memset(&stAFSPOT.stSTEP_m.stDEC_D, 0, sizeof(AFS_STEP_COUNT));

	switch (stAFSPOT.stSTEP_m.nSPD) {
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

	AFSPOT_ClrDrvCountSPD(pCNT);	// clear counter
	memset(&stAFSPOT.stSTEP_m.stDEC_I, 0, sizeof(AFS_STEP_COUNT));

	if (stAFSPOT.stSTEP_m.nSPD==DRV_STEP_MAX) stAFSPOT.stSTEP_m.nCHK = 1;
	if (stAFSPOT.stSTEP_m.nSPD==DRV_STEP_FWD) stAFSPOT.stSTEP_m.nCHK = 1;

	if (stAFSPOT.stSTEP_m.nCHK) {
		switch (stAFSPOT.stSTEP_m.nSPD) {
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
		switch (stAFSPOT.stSTEP_m.nSPD) {
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

	switch (stAFSPOT.stSTEP_m.nSPD) {
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
	// clear MIN-MAX info
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
	// clear PEAK-filt info
	stAFSPOT.stOUTn_h.nHIGH = stAFSPOT.stOUTn_h.FLTn;
	stAFSPOT.stOUTn_b.nHIGH = stAFSPOT.stOUTn_b.FLTn;

	// clear PEAK-foks info
	stAFSPOT.stOUTn_h.nPEAK = stAFSPOT.fVAL;
	stAFSPOT.stOUTn_h.xPEAK = stAFSPOT.fVAL;
	stAFSPOT.stOUTn_b.nPEAK = stAFSPOT.fVAL;
	stAFSPOT.stOUTn_b.xPEAK = stAFSPOT.fVAL;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_ClrJudgeINFO(void)
{
	// clear RATE-flag info
	stAFSPOT.stOUTn_h.nCHK = 0;
	stAFSPOT.stOUTn_b.nCHK = 0;

	stAFSPOT.stSTEP_m.nCNT = 0;		// clear speed count
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_ClrJudgeBUFF(void)
{
	memset(stAFSPOT.stOUTn_h.nBUFF, 0, sizeof(AFS_BUFF_PARAM));
	memset(stAFSPOT.stOUTn_b.nBUFF, 0, sizeof(AFS_BUFF_PARAM));

	stAFSPOT.xCNT = 0;	stAFSPOT.vCNT &= 0x80;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_MovShiftBUFF(BYTE nID)
{
	BYTE i;

	for (i=1; i<nID; i++) {
		stAFSPOT.stOUTn_h.nBUFF[i-1] = stAFSPOT.stOUTn_h.nBUFF[i];
		stAFSPOT.stOUTn_b.nBUFF[i-1] = stAFSPOT.stOUTn_b.nBUFF[i];

		stAFSPOT.stOUTn_h.nFOKS[i-1] = stAFSPOT.stOUTn_h.nFOKS[i];
		stAFSPOT.stOUTn_b.nFOKS[i-1] = stAFSPOT.stOUTn_b.nFOKS[i];
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static CHAR AFSPOT_GetFitJudgeSIGN(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap; SHORT i, move; PLONG pBUFF = (PLONG)pFILT->nBUFF;

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
		gap  = (move<13)? 13.0f/move : 1.0f;

#if 0 // mimitest
		if (rate*gap<-10.0f)	pFILT->nSIGN[i] = -1;
		else				pFILT->nSIGN[i] =  1;
#else
		if (rate*gap<= -5.0f) {
			pFILT->nSIGN[i] = -1;
			//UARTprintf("Negative...(%fx%f = %f)\r\n", rate, gap, rate*gap);
		} else if (rate*gap> 5.0f) {
			pFILT->nSIGN[i] =  1;
			//UARTprintf("Positive...(%fx%f = %f)\r\n", rate, gap, rate*gap);
		} else {
			pFILT->nSIGN[i] =  0;
			//UARTprintf("Zero...(%fx%f = %f)\r\n", rate, gap, rate*gap);
		}
#endif
	}

	for (i=1; i<nID; i++) {
		if (pFILT->nSIGN[i-1]!=pFILT->nSIGN[i]) return 0;
	}

	return pFILT->nSIGN[0];
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetFitJudgeRATE(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap; LONG *pBUFF = (PLONG)pFILT->nBUFF;

	pFILT->nMOVE = MAX4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3])
				 - MIN4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3]);

	stAFSPOT.fMOV = pFILT->nMOVE/nID;	// average driving distance

	gap = (pFILT->nMOVE<13*nID)? (13.0f*nID)/pFILT->nMOVE : 1.0f;
	//UARTprintf("\r\nstAFSPOT.fMOV : %d pFILT->nMOVE : %d, nID : %d gap : %f / ", stAFSPOT.fMOV, pFILT->nMOVE, nID, gap);

	if (stAFSPOT.nCHK==1)
		 rate = ((FLOAT)(pBUFF[1]+pBUFF[2]+pBUFF[3])/(pBUFF[0]+pBUFF[1]+pBUFF[2])-1);
	else rate = ((FLOAT)(pBUFF[0]+pBUFF[1]+pBUFF[2])/(pBUFF[1]+pBUFF[2]+pBUFF[3])-1);

	rate = (FLOAT)stAFSPOT.nCHK*(rate*1000);
	//UARTprintf("rate : %f stAFSPOT.nCHK : %d / ", rate, stAFSPOT.nCHK);
	pFILT->nINC = (LONG)((rate*gap<0)? 0 : rate*gap+0.5);
	pFILT->nDEC = (LONG)((rate*gap>0)? 0 : rate*gap-0.5);
	//UARTprintf("pFILT->nINC : %d(%d) / pFILT->nDEC : %d(%d)\r\n", pFILT->nINC, pFILT->n_UP, pFILT->nDEC, pFILT->n_DN);

	if (pFILT->nINC>=(LONG)pFILT->n_UP)	pFILT->nCHK |= ( GOOD_INC);
	else								pFILT->nCHK &= (~GOOD_INC);

	if (pFILT->nDEC<=(LONG)pFILT->n_DN)	pFILT->nCHK |= ( GOOD_DEC);
	else								pFILT->nCHK &= (~GOOD_DEC);

	//if ((pFILT->nCHK & GOOD_INC) == 0)
	//	UARTprintf("1 : ~GOOD_INC\r\n");
	//else
	//	UARTprintf("1 : GOOD_INC\r\n");

	//if ((pFILT->nCHK & GOOD_DEC) == 0)
	//	UARTprintf("1 : ~GOOD_DEC\r\n");
	//else
	//	UARTprintf("1 : GOOD_DEC\r\n");

}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetFitJudgeDIFF(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap; LONG *pBUFF = (PLONG)pFILT->nBUFF;

	pFILT->nMOVE = MAX4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3])
				 - MIN4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3]);
	//UARTprintf("pFILT->nMOVE : %d / ", pFILT->nMOVE);

	stAFSPOT.fMOV = pFILT->nMOVE/nID;	// average driving distance
	//UARTprintf("stAFSPOT.fMOV : %d / ", stAFSPOT.fMOV);

	gap = (pFILT->nMOVE<13*nID)? (13.0f*nID)/pFILT->nMOVE : 1.0f;
	//UARTprintf("gap : %f  nID : %d / ", gap, nID);

	rate = MAX4(pBUFF[0],pBUFF[1],pBUFF[2],pBUFF[3])-MIN4(pBUFF[0],pBUFF[1],pBUFF[2],pBUFF[3]);
	//UARTprintf("rate : %f / ", rate);

	pFILT->nINC = (stAFSPOT.nCHK==0)? 0 : (LONG)((FLOAT)rate*gap+0.5);
	//UARTprintf("stAFSPOT.nCHK : %d / pFILT->nINC : %d(%d)\r\n", stAFSPOT.nCHK, pFILT->nINC, stAFSPOT.stNORM_y.FLTd);

	if (pFILT->nINC>=stAFSPOT.stNORM_y.FLTd) pFILT->nCHK |= ( GOOD_INC);
	else									 pFILT->nCHK &= (~GOOD_INC);

	//if ((pFILT->nCHK & GOOD_INC) == 0)
	//	UARTprintf("2 : ~GOOD_INC\r\n");
	//else
	//	UARTprintf("2 : GOOD_INC\r\n");
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetFitJudgeBUFF(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	CHAR hSIGN = AFSPOT_GetFitJudgeSIGN(&stAFSPOT.stOUTn_h, nID);
	CHAR bSIGN = AFSPOT_GetFitJudgeSIGN(&stAFSPOT.stOUTn_b, nID);

	if (nID<3) return;

#if 0 // mimitest
	stAFSPOT.nCHK = (hSIGN==bSIGN)? hSIGN : 0;
#else
	if (hSIGN == bSIGN)			stAFSPOT.nCHK = hSIGN;
	else if (hSIGN == 0)				stAFSPOT.nCHK = bSIGN;
	else if (bSIGN == 0)				stAFSPOT.nCHK = hSIGN;
	else							stAFSPOT.nCHK = 0;
	//UARTprintf("hSIGN : %d bSIGN : %d / stAFSPOT.nCHK : %d\r\n", hSIGN, bSIGN, stAFSPOT.nCHK);
#endif

	AFSPOT_GetFitJudgeRATE(&stAFSPOT.stOUTn_h, nID);	// get rate
	AFSPOT_GetFitJudgeDIFF(&stAFSPOT.stOUTn_b, nID);	// get diff
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetPrvJudgeBUFF(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap; BYTE i; PLONG pBUFF = (PLONG)pFILT->nBUFF;

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

	stAFSPOT.fMOV = pFILT->nMOVE/nID;	// average driving distance

	if (pFILT->nSIGN[0]==pFILT->nSIGN[1]) {
		gap = (pFILT->nMOVE)? (13.0f*nID)/pFILT->nMOVE : 1.0f;
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
static void AFSPOT_GetDirJudgeBUFF(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap;	PLONG pBUFF = (PLONG)pFILT->nBUFF;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFSPOT.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	pBUFF[nID] = pFILT->FLTn; pFILT->nFOKS[nID] = stAFSPOT.fVAL;
	if (nID<3) return;

	pFILT->nMOVE = MAX4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3])
				 - MIN4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3]);

	stAFSPOT.fMOV = pFILT->nMOVE/nID;	// average driving distance

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
static void AFSPOT_InvDirJudgeBUFF(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	PLONG pBUFF = (PLONG)pFILT->nBUFF;
	FLOAT rate, gap = 13.0f/stAFSPOT.step;

	rate = (pBUFF[2])? ((FLOAT)pFILT->FLTn/pBUFF[2]-1)*1000 : 0;
	pFILT->nDEC = (LONG)((rate*gap>0)? 0 : rate*gap-0.5);

	if (pFILT->nDEC<=-1)	pFILT->nCHK |= ( ZERO_DEC);
	else					pFILT->nCHK &= (~ZERO_DEC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetDecJudgeBUFF(PAFS_FILT_PARAM pFILT, BYTE nID)
{
	FLOAT rate, gap;	PLONG pBUFF = (PLONG)pFILT->nBUFF;

	if (pFILT->FLTn>=pFILT->nHIGH) pFILT->nPEAK = stAFSPOT.fVAL;
	pFILT->nHIGH = MAX(pFILT->nHIGH,pFILT->FLTn);
	pFILT->xHIGH = MAX(pFILT->xHIGH,pFILT->FLTn);

	pFILT->nMIN = MIN(pFILT->nMIN,pFILT->FLTn);
	pFILT->nMAX = MAX(pFILT->nMAX,pFILT->FLTn);

	pBUFF[nID] = pFILT->FLTn; pFILT->nFOKS[nID] = stAFSPOT.fVAL;
	if (nID<3) return;

	pFILT->nMOVE = MAX4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3])
				 - MIN4(pFILT->nFOKS[0],pFILT->nFOKS[1],pFILT->nFOKS[2],pFILT->nFOKS[3]);

	stAFSPOT.fMOV = pFILT->nMOVE/nID;	// average driving distance

	gap = (13.0f*nID)/pFILT->nMOVE;

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
	PLONG pBUFF = (PLONG)pFILT->nBUFF;
	FLOAT rate, gap = 13.0f/stAFSPOT.step;

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

	AFSPOT_GetDriveRATE(DRV_SEARCH_FIT);	// get judge RATE

	AFSPOT_GetFitJudgeBUFF(&stAFSPOT.stOUTn_h, stAFSPOT.xCNT);

	if (AFSPOT_IsDriveEnd()==TRUE)	stAFSPOT.xCNT  = 1;
	else							stAFSPOT.xCNT += 1;

	if (AFSPOT_IsDriveEnd()==FALSE && stAFSPOT.xCNT<4) return;

	if (stAFSPOT.xCNT) stAFSPOT.xCNT--;	AFSPOT_MovShiftBUFF(4);

	if (AFSPOT_IsDriveEnd()==TRUE) {
		stAFSPOT.eCNT++;			AFSPOT_ClrJudgeRATE();
		stAFSPOT.cur = INV_SEARCH_FIT;	stAFSPOT.nxt = DRV_SEARCH_FIT;	xID = 0xf1;
	} else if (pFILTh->nCHK&GOOD_DEC && pFILTb->nCHK&GOOD_INC) {
		AFSPOT_DecDrvDecSPD_GD(DRV_SEARCH_DIR);
		stAFSPOT.cur = INV_SEARCH_FIT;	stAFSPOT.nxt = PRE_SEARCH_DIR;	xID = 0xf2;
	} else if (pFILTh->nCHK&GOOD_INC && pFILTb->nCHK&GOOD_INC) {
		AFSPOT_DecDrvDecSPD_GD(DRV_SEARCH_DIR);
		stAFSPOT.cur = DRV_SEARCH_DIR;	stAFSPOT.nxt = DRV_SEARCH_DIR;	xID = 0xf3;
	} else {
		AFSPOT_DirDrvIncSPD_BI(DRV_SEARCH_FIT);
		stAFSPOT.cur = DRV_SEARCH_FIT;	stAFSPOT.nxt = DRV_SEARCH_FIT;	xID = 0xf4;
	}

	if (stAFSPOT.nxt!=DRV_SEARCH_FIT) AFSPOT_ClrJudgeRATE();
	if (stAFSPOT.nxt!=DRV_SEARCH_FIT) AFSPOT_ClrJudgePEAK();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_GetJudge_FAR(void)
{
	PAFS_FILT_PARAM pFILTx = &stAFSPOT.stOUTn_h;

	AFSPOT_GetDriveRATE(DRV_SEARCH_FAR);	// get judge RATE

	AFSPOT_GetPrvJudgeBUFF(&stAFSPOT.stOUTn_h, stAFSPOT.xCNT);
	AFSPOT_GetPrvJudgeBUFF(&stAFSPOT.stOUTn_b, stAFSPOT.xCNT);

	if (AFSPOT_IsDriveEnd()==FALSE) {
		stAFSPOT.xCNT++;	if (stAFSPOT.xCNT<3) return;
		stAFSPOT.vCNT++;
	}

	if (AFSPOT_IsDriveEnd()==TRUE) {
		stAFSPOT.eCNT++;			AFSPOT_ClrJudgeRATE();
		stAFSPOT.cur = INV_SEARCH_FAR;	stAFSPOT.nxt = PRE_SEARCH_DIR;	xID = 0xa1;
	} else if (pFILTx->nCHK&GOOD_DEC) {
		stAFSPOT.cur = INV_SEARCH_FAR;	stAFSPOT.nxt = END_SEARCH_FAR;	xID = 0xa2;
	} else if (pFILTx->nCHK&GOOD_INC) {
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;	xID = 0xa3;
	} else if ((stAFSPOT.vCNT&15)==5) {
		stAFSPOT.cur = INV_SEARCH_FAR;	stAFSPOT.nxt = PRE_SEARCH_DIR;	xID = 0xa4;
	} else {
		stAFSPOT.xCNT--;			AFSPOT_MovShiftBUFF(3);
		stAFSPOT.cur = DRV_SEARCH_FAR;	stAFSPOT.nxt = DRV_SEARCH_FAR;	xID = 0xa5;
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

	AFSPOT_GetDriveRATE(DRV_SEARCH_NAR);	// get judge RATE

	AFSPOT_GetPrvJudgeBUFF(&stAFSPOT.stOUTn_h, stAFSPOT.xCNT);
	AFSPOT_GetPrvJudgeBUFF(&stAFSPOT.stOUTn_b, stAFSPOT.xCNT);

	if (AFSPOT_IsDriveEnd()==FALSE) {
		stAFSPOT.xCNT++;	if (stAFSPOT.xCNT<3) return;
		stAFSPOT.vCNT++;
	}

	if (AFSPOT_IsDriveEnd()==TRUE) {
		stAFSPOT.eCNT++;			AFSPOT_ClrJudgeRATE();
		stAFSPOT.cur = INV_SEARCH_NAR;	stAFSPOT.nxt = PRE_SEARCH_DIR;	xID = 0xb1;
	} else if (pFILTx->nCHK&GOOD_DEC) {
		stAFSPOT.cur = INV_SEARCH_NAR;	stAFSPOT.nxt = DRV_FORCE_PEAK;	xID = 0xb2;
	} else if (pFILTx->nCHK&GOOD_INC) {
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;	xID = 0xb3;
	} else if ((stAFSPOT.vCNT&15)==5) {
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;	xID = 0xb4;
	} else {
		stAFSPOT.xCNT--;			AFSPOT_MovShiftBUFF(3);
		stAFSPOT.cur = DRV_SEARCH_NAR;	stAFSPOT.nxt = DRV_SEARCH_NAR;	xID = 0xb5;
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
		stAFSPOT.xCNT++; if (stAFSPOT.xCNT<4) return;
	}

	if (stAFSPOT.xCNT) stAFSPOT.xCNT--;	AFSPOT_MovShiftBUFF(4);

	if (AFSPOT_IsDriveEnd()==TRUE) {
		stAFSPOT.eCNT++;			AFSPOT_ClrJudgeRATE();
		stAFSPOT.cur = INV_SEARCH_DIR;	stAFSPOT.nxt = DRV_SEARCH_DIR;	xID = 0xca;
	} else if (pFILTb->nCHK&GOOD_DEC) {
		AFSPOT_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFSPOT.cur = INV_SEARCH_DIR;	stAFSPOT.nxt = PRE_CLIMB_TOP2;	xID = 0xcb;
	} else if (pFILTb->nCHK&GOOD_INC) {
		AFSPOT_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;	xID = 0xcc;
	} else if (pFILTh->nCHK&GOOD_DEC) {
		AFSPOT_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFSPOT.cur = INV_SEARCH_DIR;	stAFSPOT.nxt = PRE_CLIMB_TOP2;	xID = 0xcd;
	} else if (pFILTh->nCHK&GOOD_INC) {
		AFSPOT_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;	xID = 0xce;
	} else {
		AFSPOT_DirDrvIncSPD_BI(DRV_SEARCH_DIR);
		stAFSPOT.cur = DRV_SEARCH_DIR;	stAFSPOT.nxt = DRV_SEARCH_DIR;	xID = 0xcf;
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

	AFSPOT_GetDriveRATE(DRV_SEARCH_DIR);	// get judge RATE

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

	AFSPOT_GetDriveRATE(DRV_CLIMB_TOP2);	// get judge RATE

	AFSPOT_GetDecJudgeBUFF(&stAFSPOT.stOUTn_h, stAFSPOT.xCNT);
	AFSPOT_GetDecJudgeBUFF(&stAFSPOT.stOUTn_b, stAFSPOT.xCNT);

	if (stAFSPOT.ZOOM<=stAFSPOT.KNEE && (pFILTh->nCHK&HIGH_INC)==0)
		pFILTb->nCHK &= (~(GOOD_INC|G_INC_OK));

	if (AFSPOT_IsDriveEnd()==FALSE) {
		stAFSPOT.xCNT++; if (stAFSPOT.xCNT<4) return;
	}

	if (stAFSPOT.xCNT) stAFSPOT.xCNT--;	AFSPOT_MovShiftBUFF(4);

	if (AFSPOT_IsDriveEnd()==TRUE) {
		stAFSPOT.eCNT++;			AFSPOT_ClrJudgeRATE();
		stAFSPOT.cur = INV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;	xID = 0xd1;
	} else if (pFILTh->nCHK&GOOD_DEC && pFILTb->nCHK&GOOD_DEC) {
		stAFSPOT.cur = INV_CLIMB_TOP2;	stAFSPOT.nxt = PRE_CLIMB_PEAK;	xID = 0xd2;
	} else if (pFILTb->nCHK&GOOD_INC && pFILTb->nCHK&RATE_DEC) {
		AFSPOT_DecDrvDecSPD_GD(DRV_CLIMB_TOP2);
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;	xID = 0xd3;
	} else if (pFILTb->nCHK&GOOD_INC) {
		AFSPOT_DecDrvIncSPD_GX(DRV_CLIMB_TOP2);
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;	xID = 0xd4;
	} else if (pFILTh->nCHK&GOOD_DEC || pFILTh->nCHK&HIGH_DEC) {
		AFSPOT_SetDrvCurSPD_XX(DRV_CLIMB_TOP2);
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;	xID = 0xd5;
	} else {
		AFSPOT_DecDrvIncSPD_BI(DRV_CLIMB_TOP2);
		stAFSPOT.cur = DRV_CLIMB_TOP2;	stAFSPOT.nxt = DRV_CLIMB_TOP2;	xID = 0xd6;
	}

	if (pFILTh->nCHK&GOOD_INC || pFILTh->nCHK&HIGH_INC) stAFSPOT.nSEL = USE_HPF;
	if (pFILTh->nCHK&GOOD_DEC || pFILTh->nCHK&HIGH_DEC) stAFSPOT.nSEL = USE_HPF;

	if (pFILTb->nCHK&R_DEC_OK && pFILTx->nCHK&ZERO_DEC) {
		stAFSPOT.cur = INV_CLIMB_TOP2;	stAFSPOT.nxt = PRE_CLIMB_PEAK;	xID = 0xd7;
	}

	if (pFILTb->nCHK&HIGH_DEC && pFILTx->nCHK&ZERO_DEC) {
		stAFSPOT.cur = INV_CLIMB_TOP2;	stAFSPOT.nxt = PRE_CLIMB_PEAK;	xID = 0xd8;
	}

	if (pFILTh->nCHK&G_INC_OK && pFILTh->nCHK&HIGH_DEC) {
		stAFSPOT.cur = INV_CLIMB_TOP2;	stAFSPOT.nxt = PRE_CLIMB_PEAK;	xID = 0xd9;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_InvJudge_DEC(void)
{
	PAFS_FILT_PARAM pFILTx = &stAFSPOT.stOUTn_b;

	if (stAFSPOT.nxt!=PRE_CLIMB_PEAK) AFSPOT_ClrJudgeBUFF();
	if (stAFSPOT.nxt!=PRE_CLIMB_PEAK) return;

	AFSPOT_GetDriveRATE(DRV_CLIMB_TOP2);	// get judge RATE

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

	AFSPOT_GetDriveRATE(DRV_CLIMB_PEAK);	// get judge RATE

	AFSPOT_GetTopJudgeBUFF(&stAFSPOT.stOUTn_h, stAFSPOT.xCNT);
	AFSPOT_GetTopJudgeBUFF(&stAFSPOT.stOUTn_b, stAFSPOT.xCNT);

	if (AFSPOT_IsDriveEnd()==TRUE) {
		stAFSPOT.cur = INV_CLIMB_PEAK;	stAFSPOT.nxt = DRV_FORCE_PEAK;	xID = 0xe1;
	} else if (pFILTx->nCHK&GOOD_DEC) {
		stAFSPOT.cur = INV_CLIMB_PEAK;	stAFSPOT.nxt = DRV_FORCE_PEAK;	xID = 0xe2;
	} else {
		stAFSPOT.cur = DRV_CLIMB_PEAK;	stAFSPOT.nxt = DRV_CLIMB_PEAK;	xID = 0xe3;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_InvJudge_TOP(void)
{
	PAFS_FILT_PARAM pFILTx = &stAFSPOT.stOUTn_b;

	if (stAFSPOT.nSEL==USE_HPF) pFILTx = &stAFSPOT.stOUTn_h;

	if (AFSPOT_IsDriveEnd()==TRUE) return;

	AFSPOT_GetDriveRATE(DRV_CLIMB_PEAK);	// get judge RATE

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
	AFSPOT_GetDriveRATE(DRV_CLIMB_PEAK);	// get judge RATE

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

	stAFSPOT.fABV = LENS_GetABVPositionLOCI(stAFSCTL.r_LOCI);
	stAFSPOT.fBLO = LENS_GetBLOPositionLOCI(stAFSCTL.r_LOCI);

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
	stAFSPOT.eCNT = 0; stAFSPOT.cur = END_FILTER_SEL;
	stAFSPOT.vCNT = 0; stAFSPOT.nxt = END_FILTER_SEL;

	memset(&stAFSPOT.stSTEP_m, 0, sizeof(AFS_STEP_PARAM));
	stAFSPOT.stSTEP_m.nMIN = LENS_GetAFDriveSTEP(DRV_STEP_MIN);
	stAFSPOT.stSTEP_m.nSTD = LENS_GetAFDriveSTEP(DRV_STEP_STD);
	stAFSPOT.stSTEP_m.nMID = LENS_GetAFDriveSTEP(DRV_STEP_MID);
	stAFSPOT.stSTEP_m.nMAX = LENS_GetAFDriveSTEP(DRV_STEP_MAX);
	stAFSPOT.stSTEP_m.nFWD = LENS_GetAFDriveSTEP(DRV_STEP_FWD);
	stAFSPOT.stSTEP_m.nBWD = LENS_GetAFDriveSTEP(DRV_STEP_BWD);
	stAFSPOT.stSTEP_m.nTOP = LENS_GetAFDriveSTEP(DRV_STEP_TOP);

	if (stAFSPOT.nSEL==USE_HPF) stAFSPOT.vCNT |= 0x80;
	stAFSPOT.nSEL = USE_HPF;	AFSPOT_ClrJudgeBUFF();

	memset(&stAFSPOT.stOUTn_h.nMIN, 0, sizeof(AFS_RATE_PARAM));
	memset(&stAFSPOT.stOUTn_b.nMIN, 0, sizeof(AFS_RATE_PARAM));

	stAFSPOT.stNORM_y.nRST = 0;		// clear reset LUMA-trig
	stAFSPOT.stNORM_y.nERR = 0;		// clear error LUMA-trig
	stAFSPOT.stNORM_y.nCNT = 0;		// clear count LUMA-trig

	stAFSCTL.n_GAIN = 0;	// clear normalized reference GAIN
	stAFSCTL.n_YSUM = 0;	// clear normalized reference YSUM
	//stAFSCTL.n_FLTr = 0;	// clear normalized reference FLTr
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_EndFilterSEL(void)
{
	if (stAFSPOT.cur!=END_FILTER_SEL) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFSPOT.cur = PRE_SEARCH_MOV;
	stAFSPOT.nxt = PRE_SEARCH_MOV;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_PreSearchMOV(void)
{
	if (stAFSPOT.cur!=PRE_SEARCH_MOV) return;

	stAFSPOT.stOUTn_b.xHIGH = stAFSPOT.stOUTn_b.FLTn;
	stAFSPOT.stOUTn_h.xHIGH = stAFSPOT.stOUTn_h.FLTn;

	stAFSPOT.cur = DRV_SEARCH_MOV;
	stAFSCTL.n_GAIN = 0;	// clear normalized reference GAIN
	stAFSCTL.n_YSUM = 0;	// clear normalized reference YSUM
	//stAFSCTL.n_FLTr = 0;	// clear normalized reference FLTr

	stAFSPOT.nxt = PRE_SEARCH_FIT;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DrvSearchMOV(void)
{
	if (stAFSPOT.cur!=DRV_SEARCH_MOV) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFSPOT.cur = stAFSPOT.nxt;

	AFSPOT_ClrJudgeRATE();	// clear judge RATE
	AFSPOT_ClrJudgePEAK();	// clear judge PEAK
	AFSPOT_ClrJudgeINFO();	// clear judge INFO

	if (stAFSPOT.nxt==PRE_SEARCH_FAR)
		 AFSPOT_GetJudge_FAR();	// get judge INFO
	else AFSPOT_GetJudge_FIT();	// get judge INFO

	AFSPOT_GetDriveSTEP(DRV_STEP_MIN);
	AFSPOT_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_PreSearchFIT(void)
{
	if (stAFSPOT.cur!=PRE_SEARCH_FIT) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFSPOT.cur = DRV_SEARCH_FIT;
	stAFSPOT.nxt = DRV_SEARCH_FIT;

	AFSPOT_GetJudge_FIT();	// get judge INFO

	AFSPOT_SetDriveSTEP(DRV_STEP_PRV);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DrvSearchFIT(void)
{
	if (stAFSPOT.cur!=DRV_SEARCH_FIT) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFSPOT_GetJudge_FIT();	// using HPFx & LPFx

	if (stAFSPOT.cur==INV_SEARCH_FIT) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_PRV);	// motor driving
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
	AFSPOT_SetDriveSTEP(DRV_STEP_PRV);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_PreSearchFAR(void)
{
	if (stAFSPOT.cur!=PRE_SEARCH_FAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFSPOT.cur = DRV_SEARCH_FAR;
	stAFSPOT.nxt = DRV_SEARCH_FAR;

	AFSPOT_GetJudge_FAR();	// get judge INFO

	AFSPOT_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DrvSearchFAR(void)
{
	if (stAFSPOT.cur!=DRV_SEARCH_FAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFSPOT_GetJudge_FAR();	// using HPFx & LPFx

	if (stAFSPOT.cur==INV_SEARCH_FAR) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
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
	AFSPOT_SetDriveSTEP(DRV_STEP_REM);	// motor driving
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

	AFSPOT_SetDriveSTEP(DRV_STEP_REM);	// motor driving
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

	AFSPOT_ClrJudgeRATE();	// clear judge RATE
	AFSPOT_ClrJudgePEAK();	// clear judge PEAK
	AFSPOT_ClrJudgeINFO();	// clear judge INFO
	AFSPOT_GetJudge_NAR();	// get judge INFO

	AFSPOT_GetDriveSTEP(DRV_STEP_MIN);
	AFSPOT_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DrvSearchNAR(void)
{
	if (stAFSPOT.cur!=DRV_SEARCH_NAR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFSPOT_GetJudge_NAR();	// using HPFx & LPFx

	if (stAFSPOT.cur==INV_SEARCH_NAR) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
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
	AFSPOT_SetDriveSTEP(DRV_STEP_MIN);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_PreSearchDIR(void)
{
	if (stAFSPOT.cur!=PRE_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFSPOT.cur = DRV_SEARCH_DIR;
	stAFSPOT.nxt = DRV_SEARCH_DIR;

	AFSPOT_GetJudge_DIR();	// get judge INFO

	AFSPOT_GetDriveSTEP(DRV_STEP_STD);
	AFSPOT_SetDriveSTEP(DRV_STEP_STD);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DrvSearchDIR(void)
{
	if (stAFSPOT.cur!=DRV_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFSPOT_GetJudge_DIR();	// using HPFx & LPFx

	if (stAFSPOT.cur==INV_SEARCH_DIR) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_STD);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_EndSearchDIR(void)
{
	if (stAFSPOT.cur!=END_SEARCH_DIR) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFSPOT.nSEL==USE_HPF)
		 stAFSPOT.peak = stAFSPOT.stOUTn_h.nPEAK;
	else stAFSPOT.peak = stAFSPOT.stOUTn_b.nPEAK;

	AFSPOT_InvJudge_DIR();	// using HPFx & LPFx

	if (stAFSPOT.eCNT==2)
		 stAFSPOT.cur = DRV_FORCE_PEAK;
	else stAFSPOT.cur = stAFSPOT.nxt;

	if (stAFSPOT.cur==DRV_FORCE_PEAK) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_PRV);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_PreClimbTOP2(void)
{
	if (stAFSPOT.cur!=PRE_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFSPOT.cur = DRV_CLIMB_TOP2;
	stAFSPOT.nxt = DRV_CLIMB_TOP2;

	AFSPOT_ClrJudgeRATE();	// clear judge RATE
	AFSPOT_ClrJudgeINFO();	// clear judge INFO
	AFSPOT_GetJudge_DEC();	// get judge INFO

	AFSPOT_SetDriveSTEP(DRV_STEP_PRV);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DrvClimbTOP2(void)
{
	if (stAFSPOT.cur!=DRV_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFSPOT_GetJudge_DEC();	// using HPFx & LPFx

	if (stAFSPOT.cur==INV_CLIMB_TOP2) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_PRV);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_EndClimbTOP2(void)
{
	if (stAFSPOT.cur!=END_CLIMB_TOP2) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFSPOT.nSEL==USE_HPF)
		 stAFSPOT.peak = stAFSPOT.stOUTn_h.nPEAK;
	else stAFSPOT.peak = stAFSPOT.stOUTn_b.nPEAK;

	AFSPOT_InvJudge_DEC();	// using HPFx & LPFx

	if (stAFSPOT.eCNT==2)
		 stAFSPOT.cur = DRV_FORCE_PEAK;
	else stAFSPOT.cur = stAFSPOT.nxt;

	if (stAFSPOT.cur==DRV_FORCE_PEAK) return;
	if (stAFSPOT.cur==PRE_CLIMB_PEAK &&
		stAFSPOT.fVAL==stAFSPOT.peak) return;

	AFSPOT_GetDriveSTEP(DRV_STEP_PRV);
	AFSPOT_SetDriveSTEP(DRV_STEP_PRV);	// motor driving
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

	AFSPOT_SetDriveSTEP(DRV_STEP_REM);	// motor driving
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

	AFSPOT_ClrJudgeRATE();	// clear judge RATE
	AFSPOT_ClrJudgePEAK();	// clear judge PEAK
	AFSPOT_ClrJudgeINFO();	// clear judge INFO
	AFSPOT_GetJudge_TOP();	// get judge INFO

	if (stAFSPOT.stNORM_y.nERR) return;

	AFSPOT_GetDriveSTEP(DRV_STEP_TOP);
	AFSPOT_SetDriveSTEP(DRV_STEP_TOP);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DrvClimbPEAK(void)
{
	if (stAFSPOT.cur!=DRV_CLIMB_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	AFSPOT_GetJudge_TOP();	// using HPFx & LPFx

	if (stAFSPOT.cur==INV_CLIMB_PEAK) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_TOP);	// motor driving
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
		 stAFSPOT.peak += 2*stAFSPOT.stSTEP_m.nTOP;
	else stAFSPOT.peak -= 0*stAFSPOT.stSTEP_m.nTOP;

	AFSPOT_InvJudge_TOP();	// using HPFx & LPFx

	stAFSPOT.cur = stAFSPOT.nxt;

	if (stAFSPOT.cur==DRV_FORCE_PEAK) return;
	AFSPOT_SetDriveSTEP(DRV_STEP_TOP);	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DrvForcePEAK(void)
{
	if (stAFSPOT.cur!=DRV_FORCE_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	if (stAFSPOT.nDIR==fFAR) stAFSPOT.peak += stAFSCTL.h_FOKS;
	else					 stAFSPOT.peak -= stAFSCTL.h_FOKS;

	stAFSPOT.cur = END_FORCE_PEAK;
	AFSPOT_SetDrivePEAK();	// motor driving
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_EndForcePEAK(void)
{
	if (stAFSPOT.cur!=END_FORCE_PEAK) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	stAFSPOT.cur = END_CLEAR_WAIT;	// wait progress
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_EndDriveWAIT(void)
{
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	switch (stAFSPOT.cur) {
		case INV_FILTER_SEL: stAFSPOT.cur = END_FILTER_SEL; break;
		case INV_SEARCH_FIT: stAFSPOT.cur = END_SEARCH_FIT; break;
		case INV_SEARCH_FAR: stAFSPOT.cur = END_SEARCH_FAR; break;
		case INV_SEARCH_NAR: stAFSPOT.cur = END_SEARCH_NAR; break;
		case INV_SEARCH_DIR: stAFSPOT.cur = END_SEARCH_DIR; break;
		case INV_CLIMB_TOP1: stAFSPOT.cur = END_CLIMB_TOP1; break;
		case INV_CLIMB_TOP2: stAFSPOT.cur = END_CLIMB_TOP2; break;
		case INV_CLIMB_PEAK: stAFSPOT.cur = END_CLIMB_PEAK; break;
		default:			 return;
	}
	AFSPOT_SetChangeDIR();	// change direction
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

	stAFSPOT.cur = END_CLEAR_PROC;	// stop progress

	if (stAFSPOT.stNORM_y.nRST==TRUE) return;
	AF_SetAUTOInfo(pCTRL, stAFSPOT.stNORM_y.nCHK);	// set AUTO

	if (stAFSPOT.ZOOM<=stAFSPOT.KNEE) return;
	LENS_SetTrackingINFO(stAFSPOT.peak);	// update Track-info
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_EndClearWAIT(PAF_CTRL_PARAM pCTRL)
{
	BOOL nID;

	if (stAFSPOT.cur!=END_CLEAR_WAIT) return;
	if (LENS_IsDriveCompleteFOKS()==FALSE) return;

	nID = (AFSPOT_InvJudge_END())? TRUE : FALSE;
	stAFSPOT.stNORM_y.nRST |= AF_GetDATAInfo(pCTRL,nID);

	stAFSPOT.cur = END_CLEAR_AUTO;	// auto progress
	LENS_EnableMotorFOKS(OFF);
}

#if __MISP100_AF_DBG__ == 1
//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_HEADPrint(void)
{
	UARTprintf("\r\nAFSPOT PROCESS TRACE...");
	UARTprintf("\r\n No   RPFo     BPFo     HPFo    YCNT   YSUM   DIFF  c  GAIN  v ");
//				    000 00000000 00000000 00000000 000000 000000 000000 0 000000 0 
	UARTprintf(" s   FLTd   ");
//				00 00000000 
	UARTprintf(" HPFx    HPFn   h_zINC h_nINC h_zDEC h_nDEC  hCHK  ");
//				000000 00000000 000000 000000 000000 000000 000000 
	UARTprintf(" BPFx    BPFn   b_zINC b_nINC b_zDEC b_nDEC  bCHK  ");
//				000000 00000000 000000 000000 000000 000000 000000 
	UARTprintf("fPeak  fVAL F D E STP PR|CU|NT  zoom n xV ID ");
//				00000 00000 0 0 0 000 00000000 00000 0 00 00

	UARTprintf("%02d %02d\r\n", eID, count_16ms);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_PROCPrint(void)
{
	UARTprintf("%03d ",		stAFSPOT.time);
	UARTprintf("%08d ",		stAFSPOT.stDATA_r.FLTo);
	UARTprintf("%08d ",		stAFSPOT.stDATA_b.FLTo);
	UARTprintf("%08d ",		stAFSPOT.stDATA_h.FLTo);
	UARTprintf("%06d ",		stAFSPOT.stNORM_y.YCNT);
	UARTprintf("%06d ",		stAFSPOT.stNORM_y.YSUM);
	UARTprintf("%06d ",		stAFSPOT.stNORM_y.YSUM-stAFSPOT.stNORM_y.YPRV);
	UARTprintf("%01X ",		stAFSPOT.stNORM_y.nRST);
	UARTprintf("%06d ",		stAFSPOT.stNORM_y.GAIN);
	UARTprintf("%01X ",		stAFSPOT.stNORM_y.nCHK);

//	UARTprintf("%08d ",		stAFSCTL.n_FLTr);
	UARTprintf("%02d ",		stAFSPOT.nCHK);
	UARTprintf("%08d ",		stAFSPOT.stNORM_y.FLTd);
//	UARTprintf("%08d ",		stAFSPOT.stDATA_r.FLTn);

	UARTprintf("%06d ",		stAFSPOT.stOUTn_h.FLTx);
	UARTprintf("%08d ",		stAFSPOT.stOUTn_h.FLTn);
	UARTprintf("%06d ",		stAFSPOT.stOUTn_h.zINC);
	UARTprintf("%06d ",		stAFSPOT.stOUTn_h.nINC);
	UARTprintf("%06d ",		stAFSPOT.stOUTn_h.zDEC);
	UARTprintf("%06d ",		stAFSPOT.stOUTn_h.nDEC);
	UARTprintf("%06X ",		stAFSPOT.stOUTn_h.nCHK);

	UARTprintf("%06d ",		stAFSPOT.stOUTn_b.FLTx);
	UARTprintf("%08d ",		stAFSPOT.stOUTn_b.FLTn);
	UARTprintf("%06d ",		stAFSPOT.stOUTn_b.zINC);
	UARTprintf("%06d ",		stAFSPOT.stOUTn_b.nINC);
	UARTprintf("%06d ",		stAFSPOT.stOUTn_b.zDEC);
	UARTprintf("%06d ",		stAFSPOT.stOUTn_b.nDEC);
	UARTprintf("%06X ",		stAFSPOT.stOUTn_b.nCHK);

	UARTprintf("%05d ",		stAFSPOT.peak);
	UARTprintf("%05d ",		stAFSPOT.fVAL);
	UARTprintf("%01d ",		stAFSPOT.nSEL);
	UARTprintf("%01d ",		stAFSPOT.nDIR);
	UARTprintf("%01d ",		stAFSPOT.eCNT);
	UARTprintf("%03d ",		stAFSPOT.step);
	UARTprintf("%02X|%02X|%02X ", stAFSPOT.prv,stAFSPOT.cur,stAFSPOT.nxt);
	UARTprintf("%05d ",		stAFSPOT.zVAL);
	UARTprintf("%01d ",		stAFSPOT.xCNT);
//	UARTprintf("%07d ",		stAFSPOT.stOUTn_h.xHIGH);
//	UARTprintf("%07d ",		stAFSPOT.stOUTn_b.xHIGH);
	UARTprintf("%02X ",		stAFSPOT.vCNT);
	UARTprintf("%02X ",		xID);

	UARTprintf("%02d %02d\r\n", eID, count_16ms);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFSPOT_DBGPrint(void)
{
	if (stAFSPOT.time)	AFSPOT_PROCPrint();
	else				AFSPOT_HEADPrint();

	stAFSPOT.time++;
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
void AFSPOT_ProcessHandler(PAF_CTRL_PARAM pCTRL)
{
	if (stAFSPOT.cur==0) return;
	stAFSPOT.prv = stAFSPOT.cur;	// for debug print
	stAFSPOT.nMOD = pCTRL->nMOD;

	AFSPOT_GetAFOPDWIND(pCTRL);		// get AFOPD data
	AFSPOT_EndClearLUMA(pCTRL);		// chk LUMA info
	AFSPOT_EndClearAUTO(pCTRL);		// set AUTO info
	AFSPOT_EndClearWAIT(pCTRL);		// set AUTO wait


	AFSPOT_PreFilterSEL();		// select filter
	AFSPOT_PreSearchMOV();		// search wait-luma
	AFSPOT_PreSearchFIT();		// search force-fit
	AFSPOT_PreSearchFAR();		// search shake-far
	AFSPOT_PreSearchNAR();		// search shake-nar
	AFSPOT_PreSearchDIR();		// search direction
	AFSPOT_PreClimbTOP2();		// search climb-dec
	AFSPOT_PreClimbPEAK();		// search fine-peak

	AFSPOT_DrvSearchMOV();		// drive force-mov
	AFSPOT_DrvSearchFIT();		// drive force-fit
	AFSPOT_DrvSearchFAR();		// drive shake-far
	AFSPOT_DrvSearchNAR();		// drive shake-nar
	AFSPOT_DrvSearchDIR();		// drive direction
	AFSPOT_DrvClimbTOP2();		// drive climb-dec
	AFSPOT_DrvClimbPEAK();		// drive fine-peak
	AFSPOT_DrvForcePEAK();		// drive force-top

	AFSPOT_EndFilterSEL();		// arrange filter
	AFSPOT_EndSearchFIT();		// arrange force-fit
	AFSPOT_EndSearchFAR();		// arrange shake-far
	AFSPOT_EndSearchNAR();		// arrange shake-nar
	AFSPOT_EndSearchDIR();		// arrange direction
	AFSPOT_EndClimbTOP2();		// arrange climb-dec
	AFSPOT_EndClimbPEAK();		// arrange fine-peak

	AFSPOT_EndForcePEAK();		// wait force-peak
	AFSPOT_EndDriveWAIT();		// wait drive-stop
	eID = count_16ms;

#if __MISP100_AF_DBG__ == 1
	if ((ODM_GetDEBUGMode()!=0x03) && (ODM_GetDEBUGMode()!=0x05)) return;
	AFSPOT_DBGPrint();
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void AFSPOT_EnableProcess(BOOL OnOff)
{
	if (OnOff)	stAFSPOT.cur = PRE_FILTER_SEL;
	else		stAFSPOT.cur = END_CLEAR_PROC;

	stAFSPOT.time = 0;	// for debug print
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AFSPOT_IsProcessExecuting(void)
{
	return (stAFSPOT.cur==0)? FALSE : TRUE;
}

#endif	/* defined(__USE_AFZOOM_LENS__) */

/*  FILE_END_HERE */
