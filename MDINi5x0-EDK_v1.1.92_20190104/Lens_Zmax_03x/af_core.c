// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
#define NumOfBUFF 5

typedef	struct
{
	LONG	xFLTr;			// reference FLTr for auto-trig
	LONG	xFLTh;			// reference FLTh for auto-trig
	LONG	xFLTb;			// reference FLTb for auto-trig
	LONG	xYSUM;			// reference YSUM for auto-trig
	LONG	xGAIN;			// reference GAIN for auto-trig

	LONG	nFLTr;			// new-value FLTr for auto-trig
	LONG	nFLTh;			// new-value FLTh for auto-trig
	LONG	nFLTb;			// new-value FLTb for auto-trig
	LONG	nYSUM;			// new-value YSUM for auto-trig
	LONG	nGAIN;			// new-value GAIN for auto-trig

	LONG	oFLTr;			// old-value FLTr for auto-trig
	LONG	oFLTh;			// old-value FLTh for auto-trig
	LONG	oFLTb;			// old-value FLTb for auto-trig
	LONG	oYSUM;			// old-value YSUM for auto-trig
	LONG	oGAIN;			// old-value GAIN for auto-trig

	LONG	RATEh;			// calcurate FLTh for auto-trig
	LONG	RATEb;			// calcurate FLTb for auto-trig

	SHORT	CNTh;			// cur-count FLTh for auto-trig
	SHORT	CNTb;			// cur-count FLTb for auto-trig

	SHORT	ZOOM;			// zoom position data for auto
	SHORT	KNEE;			// knee position data for auto

	LONG	rBUFF[NumOfBUFF+1];
	LONG	hBUFF[NumOfBUFF+1];
	LONG	bBUFF[NumOfBUFF+1];
	LONG	yBUFF[NumOfBUFF+1];
	CHAR	nIDX;
	BYTE	PROC;

	BYTE	nFRM;
	BYTE	nEXE;

	LONG	rYSUM;			// reference YSUM for luma-trig
	LONG	yRATE;			// calcurate LUMA for luma-trig
	LONG	yDIFF;			// calcurate DIFF for luma-trig
	SHORT	CNTy;			// cur-count YSUM for luma-trig

	BYTE	nRST;			// reset status for luma-trig
	BYTE	nCNT;			// wait counter for luma-trig

}	AF_AUTO_PARAM, *PAF_AUTO_PARAM;

typedef	struct
{
	SHORT	rVALy;			// ref-value LUMA for luma-trig
	SHORT	rGAPy;			// gap-value LUMA for luma-trig
	SHORT	rLOWy;			// low-value LUMA for luma-trig
	SHORT	rCNTy;			// ref-count YSUM for luma-trig

	SHORT	rFLTh;			// ref-value FLTh for auto-trig
	SHORT	rFLTb;			// ref-value FLTb for auto-trig
	SHORT	rCNTh;			// ref-count FLTh for auto-trig
	SHORT	rCNTb;			// ref-count FLTb for auto-trig

	SHORT	rFLTh_NORMMUL;		// change ratio multi factor of xFLTh/nFLTh in Norm
	SHORT	rFLTb_NORMMUL;		// change ratio multi factor of xFLTb/nFLTb in Norm
	SHORT	rYSUM_LUMAMUL;		// change ratio multi factor of rYSUM/nYSUM in Luma
	SHORT	rFLTh_DATAMUL;		// change ratio multi factor of oFLTh/nFLTh in Data
	SHORT	rFLTb_DATAMUL;		// change ratio multi factor of oFLTh/nFLTh in Data
	SHORT	rFLTh_AUTOMUL;		// change ratio multi factor of oFLTh/nFLTh in Auto
	SHORT	rFLTb_AUTOMUL;		// change ratio multi factor of oFLTh/nFLTh in Auto
	SHORT	rSPOTCnt;			// ref-count nSPT for SPOT operation
	LONG	rSPOTGain;			// ref-count CNTA for spot-trig

	DWORD	rDARKGainH;
	DWORD	rDARKGainM;
	DWORD	rDARKGainL;
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

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
typedef	struct
{
	WORD	HI[25];
	WORD	LO[25];

}	AF_LBPF_PARAM, *PAF_LBPF_PARAM;
#else
typedef	struct
{
	WORD	HI[38];
	WORD	LO[38];

}	AF_LBPF_PARAM, *PAF_LBPF_PARAM;
#endif

typedef enum {
	OLD_BUFF = 0, NEW_BUFF

} 	AF_PROCESS_BUFF;

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static AF_AUTO_PARAM stAFAUTO = {0,};
static AF_TUNE_PARAM stAFTUNE = {
//	VALy   GAPy   LOWy   CNTy	of ref-luma for auto-trigger
	200,   500,  4000,    15,

//	FLTh   FLTb   CNTh   CNTb		of ref-filt for auto-trigger
	100,   100,    15,    15,

//	rFLTh_NORMMUL	rFLTb_NORMMUL
	1000,			1000,
//	rYSUM_LUMAMUL
	1000,
//	rFLTh_DATAMUL	rFLTb_DATAMUL
	1000,			1000,

//	rFLTh_AUTOMUL	rFLTb_AUTOMUL
	500,				1000,

//	rSPOTCnt			rSPOTGain					of ref-filt for spot-trigger
	3,				4500,

//	rDARKGainH		rDARKGainM		rDARKGainL
#if defined(__USE_CMOS_IMX290__)
	16384,			32768,			65536,
#elif defined(__USE_CMOS_IMX185__)
	32768,			65536,			65536,
#elif defined(__USE_CMOS_IMX226__)
	65536,			65536,			65536,
#else
	0x18000,			0x20000,			0x30000,
#endif
};

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
static ROMDATA AF_HBPF_PARAM stAFHBPF = {
	{	// HI 0.10 ~ 2.00
		0x00ff, 0x0104, 0x03ff, 0xff02, 0x03ff, 0xfc00, 0x02fd, 0xfafd,
		0x01fc, 0xf7fb, 0x00fb, 0xf4f9, 0x00f9, 0xf1f8, 0x00f8, 0xedf6,
		0x02f7, 0xe8f5, 0x07f6, 0xdef4, 0x15f5, 0xbef3, 0x97f5
	},
/*
	{	// LO 0.08 ~ 0.80
		0x0202, 0x0201, 0xfffe, 0xfcfb, 0xfbfc, 0xfdff, 0x0000, 0xfefc,
		0xf9f7, 0xf6f7, 0xfafd, 0x0000, 0xfef9, 0xf5f1, 0xf0f2, 0xf8fe,
		0x0304, 0x00f8, 0xede4, 0xe1e6, 0xf60e, 0x2a44, 0x575e
	}
*/
	{	// LO 0.06 ~ 1.24
		0xfefe, 0xfefe, 0xfefd, 0xfcfa, 0xfafb, 0xfdfe, 0xfdfa, 0xf7f8,
		0xfbfe, 0xfffb, 0xf7f5, 0xf8fd, 0x00fe, 0xf7f2, 0xf3fa, 0x0203,
		0xfbef, 0xebf3, 0x030d, 0x05ee, 0xd9dc, 0x0444, 0x8098
	}
};
#else 												// MDIN-i51X (i510, i540, i550)
static ROMDATA AF_HBPF_PARAM stAFHBPF = {
	{	// HI 0.10 ~ 2.00
		0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
		0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
		0x00ff, 0x0104, 0x03ff, 0xff02, 0x03ff, 0xfc00, 0x02fd, 0xfafd,
		0x01fc, 0xf7fb, 0x00fb, 0xf4f9, 0x00f9, 0xf1f8, 0x00f8, 0xedf6,
		0x02f7, 0xe8f5, 0x07f6, 0xdef4, 0x15f5, 0xbef3, 0x97f5
	},
/*
	{	// LO 0.08 ~ 0.80
		0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
		0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
		0x0202, 0x0201, 0xfffe, 0xfcfb, 0xfbfc, 0xfdff, 0x0000, 0xfefc,
		0xf9f7, 0xf6f7, 0xfafd, 0x0000, 0xfef9, 0xf5f1, 0xf0f2, 0xf8fe,
		0x0304, 0x00f8, 0xede4, 0xe1e6, 0xf60e, 0x2a44, 0x575e
	}
*/
	{	// LO 0.06 ~ 1.24
		0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
		0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
		0xfefe, 0xfefe, 0xfefd, 0xfcfa, 0xfafb, 0xfdfe, 0xfdfa, 0xf7f8,
		0xfbfe, 0xfffb, 0xf7f5, 0xf8fd, 0x00fe, 0xf7f2, 0xf3fa, 0x0203,
		0xfbef, 0xebf3, 0x030d, 0x05ee, 0xd9dc, 0x0444, 0x8098
	}
};
#endif

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
static ROMDATA AF_LBPF_PARAM stAFLBPF = {
	{	// HI 0.05 ~ 0.20
		0x0000, 0xffff, 0xfffe, 0xfefd, 0xfdfc, 0xfcfb, 0xfafa, 0xf9f9, 0xf8f8,
		0xf7f7, 0xf7f6, 0xf6f7, 0xf7f7, 0xf8f8, 0xf9fa, 0xfbfd, 0xfe00, 0x0103,
		0x0406, 0x0809, 0x0b0c, 0x0e0f, 0x1011, 0x1212, 0x1313
	},
/*
	{	// LO 0.05 ~ 0.16
		0xfcfe, 0xfefd, 0xfdfc, 0xfcfc, 0xfbfb, 0xfafa, 0xf9f9, 0xf9f8, 0xf8f8,
		0xf8f8, 0xf8f8, 0xf9f9, 0xfafa, 0xfbfc, 0xfdfe, 0xff00, 0x0102, 0x0304,
		0x0607, 0x0809, 0x0a0b, 0x0c0d, 0x0e0e, 0x0f0f, 0x0f0f
	}
*/
	{	// LO 0.05 ~ 0.20
		0x0000, 0xffff, 0xfffe, 0xfefd, 0xfdfc, 0xfcfb, 0xfafa, 0xf9f9, 0xf8f8,
		0xf7f7, 0xf7f6, 0xf6f7, 0xf7f7, 0xf8f8, 0xf9fa, 0xfbfd, 0xfe00, 0x0103,
		0x0406, 0x0809, 0x0b0c, 0x0e0f, 0x1011, 0x1212, 0x1313
	}
};
#else 												// MDIN-i51X (i510, i540, i550)
static ROMDATA AF_LBPF_PARAM stAFLBPF = {
	{	// HI 0.05 ~ 0.20
		0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
		0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 
		0x0000, 0xffff, 0xfffe, 0xfefd, 0xfdfc, 0xfcfb, 0xfafa, 0xf9f9, 0xf8f8,
		0xf7f7, 0xf7f6, 0xf6f7, 0xf7f7, 0xf8f8, 0xf9fa, 0xfbfd, 0xfe00, 0x0103,
		0x0406, 0x0809, 0x0b0c, 0x0e0f, 0x1011, 0x1212, 0x1313
	},
/*
	{	// LO 0.05 ~ 0.16
		0xfcfe, 0xfefd, 0xfdfc, 0xfcfc, 0xfbfb, 0xfafa, 0xf9f9, 0xf9f8, 0xf8f8,
		0xf8f8, 0xf8f8, 0xf9f9, 0xfafa, 0xfbfc, 0xfdfe, 0xff00, 0x0102, 0x0304,
		0x0607, 0x0809, 0x0a0b, 0x0c0d, 0x0e0e, 0x0f0f, 0x0f0f
	}
*/
	{	// LO 0.05 ~ 0.20
		0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
		0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 
		0x0000, 0xffff, 0xfffe, 0xfefd, 0xfdfc, 0xfcfb, 0xfafa, 0xf9f9, 0xf8f8,
		0xf7f7, 0xf7f6, 0xf6f7, 0xf7f7, 0xf8f8, 0xf9fa, 0xfbfd, 0xfe00, 0x0103,
		0x0406, 0x0809, 0x0b0c, 0x0e0f, 0x1011, 0x1212, 0x1313
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
extern FLOAT afWinRatio;
extern FLOAT aeWinRatio;
 static void AFCORE_GetAFOPDWIND(PAF_CTRL_PARAM pCTRL)
{
#if defined(__USE_EXT_AF_MD1__)
	LONG WNDs[5];

	SPImAPP_GetSlaveAFfiltA(&pCTRL->stFILT.RPFa);
	pCTRL->stFILT.RPFa *= afWinRatio;
	pCTRL->stFILT.LPFa *= afWinRatio;
	pCTRL->stFILT.VPFa *= afWinRatio;
	pCTRL->stFILT.BPFa *= afWinRatio;
	pCTRL->stFILT.HPFa *= afWinRatio;
	pCTRL->stFILT.CNTa *= afWinRatio;

	SPImAPP_GetSlaveAFfiltB(&pCTRL->stFILT.RPFb);
	pCTRL->stFILT.RPFb *= afWinRatio;
	pCTRL->stFILT.LPFb *= afWinRatio;
	pCTRL->stFILT.VPFb *= afWinRatio;
	pCTRL->stFILT.BPFb *= afWinRatio;
	pCTRL->stFILT.HPFb *= afWinRatio;
	pCTRL->stFILT.CNTb *= afWinRatio;

	pCTRL->stFILT.YPRV = (pCTRL->stFILT.YSUM);
	SPImAPP_GetSlaveAEysum((PBYTE)&WNDs[0]);
	pCTRL->stFILT.YSUM = (LONG)((FLOAT)(WNDs[0]+WNDs[1]+WNDs[2]+WNDs[3]+WNDs[4])*aeWinRatio)/5.0f;
	//UARTprintf("slave ysum : %d\r\n",pCTRL->stFILT.YSUM);

	SPImAPP_GetSlaveAEgain((PBYTE)&WNDs[0]);
	pCTRL->stFILT.GAIN = WNDs[0];

#elif defined(__USE_4K60_MD1__)
	SPImAPP_GetSlaveAFfiltA((PBYTE)&pCTRL->stFILT.RPFa);
	SPImAPP_GetSlaveAFfiltB((PBYTE)&pCTRL->stFILT.RPFb);
	pCTRL->stFILT.RPFa /= 2;
	pCTRL->stFILT.LPFa /= 2;
	pCTRL->stFILT.VPFa /= 2;
	pCTRL->stFILT.BPFa /= 2;
	pCTRL->stFILT.HPFa /= 2;
	pCTRL->stFILT.CNTa /= 2;

	pCTRL->stFILT.RPFb /= 2;
	pCTRL->stFILT.LPFb /= 2;
	pCTRL->stFILT.VPFb /= 2;
	pCTRL->stFILT.BPFb /= 2;
	pCTRL->stFILT.HPFb /= 2;
	pCTRL->stFILT.CNTb /= 2;

	WORD rVal[16];

	MISP_MultiRead(MISP_LOCAL_ID, 0x6f0, (PBYTE)rVal, 32);
	pCTRL->stFILT.RPFa += (LONG)(((FLOAT)(MAKEDWORD(rVal[ 0], rVal[ 1])>>0)) * afWinRatio)/2;
	pCTRL->stFILT.LPFa += (LONG)(((FLOAT)(MAKEDWORD(rVal[ 2], rVal[ 3])>>0)) * afWinRatio)/2;
	pCTRL->stFILT.VPFa += (LONG)(((FLOAT)(MAKEDWORD(rVal[ 4], rVal[ 5])>>0)) * afWinRatio)/2;
	pCTRL->stFILT.HPFa += (LONG)(((FLOAT)(MAKEDWORD(rVal[ 6], rVal[ 7])>>0)) * afWinRatio)/2;
	pCTRL->stFILT.RPFb += (LONG)(((FLOAT)(MAKEDWORD(rVal[ 8], rVal[ 9])>>0)) * afWinRatio)/2;
	pCTRL->stFILT.LPFb += (LONG)(((FLOAT)(MAKEDWORD(rVal[10], rVal[11])>>0)) * afWinRatio)/2;
	pCTRL->stFILT.VPFb += (LONG)(((FLOAT)(MAKEDWORD(rVal[12], rVal[13])>>0)) * afWinRatio)/2;
	pCTRL->stFILT.HPFb += (LONG)(((FLOAT)(MAKEDWORD(rVal[14], rVal[15])>>0)) * afWinRatio)/2;

	MISP_MultiRead(MISP_LOCAL_ID, 0x6E8, (PBYTE)rVal, 16);
	pCTRL->stFILT.CNTa += (LONG)(((FLOAT)(MAKEDWORD(rVal[0], rVal[1])>>0)) * afWinRatio)/2;
	pCTRL->stFILT.BPFa += (LONG)(((FLOAT)(MAKEDWORD(rVal[2], rVal[3])>>0)) * afWinRatio)/2;
	pCTRL->stFILT.CNTb += (LONG)(((FLOAT)(MAKEDWORD(rVal[4], rVal[5])>>0)) * afWinRatio)/2;
	pCTRL->stFILT.BPFb += (LONG)(((FLOAT)(MAKEDWORD(rVal[6], rVal[7])>>0)) * afWinRatio)/2;

	pCTRL->stFILT.YPRV = (pCTRL->stFILT.YSUM);
	pCTRL->stFILT.YSUM = Get_AE_Ysum();
	//RTprintf("cal Ysum : %d / %s\r\n", pCTRL->stFILT.YSUM, __FUNCTION__);
	pCTRL->stFILT.GAIN = (AECTRL_GetGAINGain());

#else
	WORD rVal[16];

	MISP_MultiRead(MISP_LOCAL_ID, 0x6f0, (PBYTE)rVal, 32);
	pCTRL->stFILT.RPFa = (LONG)(((FLOAT)(MAKEDWORD(rVal[ 0], rVal[ 1])>>0)) * afWinRatio);
	pCTRL->stFILT.LPFa = (LONG)(((FLOAT)(MAKEDWORD(rVal[ 2], rVal[ 3])>>0)) * afWinRatio);
	pCTRL->stFILT.VPFa = (LONG)(((FLOAT)(MAKEDWORD(rVal[ 4], rVal[ 5])>>0)) * afWinRatio);
	pCTRL->stFILT.HPFa = (LONG)(((FLOAT)(MAKEDWORD(rVal[ 6], rVal[ 7])>>0)) * afWinRatio);
	pCTRL->stFILT.RPFb = (LONG)(((FLOAT)(MAKEDWORD(rVal[ 8], rVal[ 9])>>0)) * afWinRatio);
	pCTRL->stFILT.LPFb = (LONG)(((FLOAT)(MAKEDWORD(rVal[10], rVal[11])>>0)) * afWinRatio);
	pCTRL->stFILT.VPFb = (LONG)(((FLOAT)(MAKEDWORD(rVal[12], rVal[13])>>0)) * afWinRatio);
	pCTRL->stFILT.HPFb = (LONG)(((FLOAT)(MAKEDWORD(rVal[14], rVal[15])>>0)) * afWinRatio);

	MISP_MultiRead(MISP_LOCAL_ID, 0x6E8, (PBYTE)rVal, 16);
	pCTRL->stFILT.CNTa = (LONG)(((FLOAT)(MAKEDWORD(rVal[0], rVal[1])>>0)) * afWinRatio); // 0x6a5 (af_luma_max)
	pCTRL->stFILT.BPFa = (LONG)(((FLOAT)(MAKEDWORD(rVal[2], rVal[3])>>0)) * afWinRatio);
	pCTRL->stFILT.CNTb = (LONG)(((FLOAT)(MAKEDWORD(rVal[4], rVal[5])>>0)) * afWinRatio);
	pCTRL->stFILT.BPFb = (LONG)(((FLOAT)(MAKEDWORD(rVal[6], rVal[7])>>0)) * afWinRatio);

	pCTRL->stFILT.YPRV = (pCTRL->stFILT.YSUM);
	pCTRL->stFILT.YSUM = Get_AE_Ysum();
	//UARTprintf("cal Ysum : %d / %s\r\n", pCTRL->stFILT.YSUM, __FUNCTION__);
	pCTRL->stFILT.GAIN = (AECTRL_GetGAINGain());
#endif

	//Histogram_Read(void);

	if (pCTRL->stFILT.CNTa<stAFTUNE.rSPOTGain) {
		pCTRL->nSPT = 0;
	} else {
		if(pCTRL->nSPT < stAFTUNE.rSPOTCnt)
			pCTRL->nSPT += 1;
	}
	//UARTprintf("pCTRL->nSPT : %d\r\n", pCTRL->nSPT);

	stAFAUTO.ZOOM = LENS_GetRelPositionZOOM();
	stAFAUTO.KNEE = LENS_GetKneePositionZOOM();
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerByZOOM(PAF_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_MANUAL_MODE)
		SetMsgCommandTriggerAF(OFF);

	if ((pCTRL->ctrl&AFCTRL_AFMODE_MASK)==AFCTRL_INTVAL_MODE)
		SetMsgCommandTriggerAF(OFF);

	if (IsMsgCommandTriggerAF()==FALSE) return;

	if (IsMsgExecutionComplete()) {
		AF_EnableProcess(ON);	SetMsgCommandTriggerAF(OFF);
	}

	if (IsMsgGuardZOOMComplete()) {
		AF_EnableProcess(ON);	SetMsgCommandTriggerAF(OFF);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static LONG AFCORE_CtrlMeanBUFF(PLONG pBUFF, BOOL nID)
{
	BYTE i;		LONG sum = 0;

	for (i=0+nID; i<NumOfBUFF+nID; i++) sum += pBUFF[i];
	//return MAX((LONG)((FLOAT)sum/(FLOAT)NumOfBUFF+0.5),100);
	return MAX((LONG)((FLOAT)sum/(FLOAT)NumOfBUFF+0.5),30);
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


	if (stAFAUTO.nIDX<NumOfBUFF) stAFAUTO.nIDX++;
	if (stAFAUTO.nIDX<NumOfBUFF) return;

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
#if __MISP100_AF_DBG__ == 1
	if ((ODM_GetDEBUGMode()==0x04)) {
		UARTprintf("stAFAUTO.xFLTb : %d / stAFAUTO.nFLTb : %d\r\n", stAFAUTO.xFLTb, stAFAUTO.nFLTb);
	}
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerByLUMA(PAF_CTRL_PARAM pCTRL) //check YSUM ratio -- If ysum ratio is too big... reset the counter ( AE Change or block)
{
	FLOAT rate;	SHORT rVALy, rGAPy;

	if ((pCTRL->ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_MANUAL_MODE) return;
	if ((pCTRL->ctrl&AFCTRL_AFMODE_MASK)!=AFCTRL_AFAUTO_MODE) return;
	if (LENS_IsDriveEnableZOOM() || LENS_IsDriveEnableFOKS()) return;
	if (stAFAUTO.nIDX<NumOfBUFF) return;

	if (stAFAUTO.nGAIN==AECTRL_GetGAINMax())
			stAFAUTO.rYSUM  = (LONG)stAFAUTO.oYSUM;
	else	stAFAUTO.rYSUM  = AECTRL_GetBrightness();

	if (stAFAUTO.nYSUM<stAFTUNE.rLOWy)
			stAFAUTO.rYSUM += 2*(LONG)stAFTUNE.rVALy;

	rate = ((FLOAT)stAFAUTO.rYSUM/stAFAUTO.nYSUM-1)*stAFTUNE.rYSUM_LUMAMUL;
	stAFAUTO.yRATE = ABS((LONG)((rate<0)? rate-0.5 : rate+0.5));

	rVALy = stAFTUNE.rVALy;	rGAPy = stAFTUNE.rGAPy;

	if (stAFAUTO.yRATE>=(LONG)rVALy) stAFAUTO.CNTh = 0;
	if (stAFAUTO.yRATE>=(LONG)rVALy) stAFAUTO.CNTb = 0;

	stAFAUTO.yDIFF = ABS(stAFAUTO.oYSUM-stAFAUTO.nYSUM);
	if (stAFAUTO.yDIFF>=(LONG)rGAPy) stAFAUTO.CNTh = 0;
	if (stAFAUTO.yDIFF>=(LONG)rGAPy) stAFAUTO.CNTb = 0;
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerByDATA(PAF_CTRL_PARAM pCTRL)
{
	FLOAT rate;	SHORT rFLTh, rFLTb;

	if ((pCTRL->ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_MANUAL_MODE) return;
	if ((pCTRL->ctrl&AFCTRL_AFMODE_MASK)!=AFCTRL_AFAUTO_MODE) return;
	if (LENS_IsDriveEnableZOOM() || LENS_IsDriveEnableFOKS()) return;
	if (stAFAUTO.nIDX<NumOfBUFF) return;

	rFLTh = (LONG)((FLOAT)pCTRL->stFILT.GAIN/(FLOAT)(AECTRL_GetGAINMax()/8)+20.5); //16384
	rFLTb = (LONG)((FLOAT)pCTRL->stFILT.GAIN/(FLOAT)(AECTRL_GetGAINMax()/8)+20.5);

	// check local-value rate
	rate = ((FLOAT)stAFAUTO.oFLTh/stAFAUTO.nFLTh-1)*stAFTUNE.rFLTh_DATAMUL; //1000
	if (ABS(stAFAUTO.oFLTh-stAFAUTO.nFLTh)<10) rate = 0.0f;
	stAFAUTO.RATEh = ABS((LONG)((rate<0)? rate-0.5 : rate+0.5));

	rate = ((FLOAT)stAFAUTO.oFLTb/stAFAUTO.nFLTb-1)*stAFTUNE.rFLTb_DATAMUL; //1000
	if (ABS(stAFAUTO.oFLTb-stAFAUTO.nFLTb)<10) rate = 0.0f;
	stAFAUTO.RATEb = ABS((LONG)((rate<0)? rate-0.5 : rate+0.5));

	if (stAFAUTO.RATEh<(LONG)rFLTh) {stAFAUTO.CNTh += 1;}
	else		{stAFAUTO.CNTb  = 0; stAFAUTO.CNTh  = 0;}

	if (stAFAUTO.RATEb<(LONG)rFLTb) {stAFAUTO.CNTb += 1;}
	else		{stAFAUTO.CNTb  = 0; stAFAUTO.CNTh  = 0;}

	stAFAUTO.CNTh = MIN(stAFAUTO.CNTh,stAFTUNE.rCNTh);
	stAFAUTO.CNTb = MIN(stAFAUTO.CNTb,stAFTUNE.rCNTb);

#if __MISP100_AF_DBG__ == 1
	if ((stAFAUTO.RATEh>=rFLTh||stAFAUTO.RATEb>=rFLTb)&&((ODM_GetDEBUGMode()==0x04))) {
		UARTprintf("[AUTO-CLEAR by DATA] nH:%06d rH:%06d", stAFAUTO.RATEh, rFLTh);
		UARTprintf(" nB:%06d rB:%06d\r\n", stAFAUTO.RATEb, rFLTb);
	}
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerByNORM(PAF_CTRL_PARAM pCTRL)
{
	FLOAT nYSUM, nHPFr, rate;	SHORT rFLTh, rFLTb;

	if ((pCTRL->ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_MANUAL_MODE) return;
	if ((pCTRL->ctrl&AFCTRL_AFMODE_MASK)!=AFCTRL_AFAUTO_MODE) return;
	if (LENS_IsDriveEnableZOOM() || LENS_IsDriveEnableFOKS()) return;
	if (stAFAUTO.nIDX<NumOfBUFF) return;

	nYSUM = ((FLOAT)stAFAUTO.xYSUM)/stAFAUTO.nYSUM;
	stAFAUTO.nFLTr = (LONG)(nYSUM*stAFAUTO.nFLTr+0.5);
	stAFAUTO.nFLTh = (LONG)(nYSUM*stAFAUTO.nFLTh+0.5);
	stAFAUTO.nFLTb = (LONG)(nYSUM*stAFAUTO.nFLTb+0.5);

	nHPFr = ((FLOAT)stAFAUTO.xFLTr)/stAFAUTO.nFLTr;
	if (stAFAUTO.xFLTr==0) nHPFr = (1.0f);
	stAFAUTO.nFLTh = (LONG)(nHPFr*stAFAUTO.nFLTh+0.5);
	stAFAUTO.nFLTb = (LONG)(nHPFr*stAFAUTO.nFLTb+0.5);

	rate = ((FLOAT)(AECTRL_GetGAINMax()/8)/(FLOAT)pCTRL->stFILT.GAIN)*stAFTUNE.rFLTh+0.5; //16384
	rFLTh = MIN(MAX((SHORT)rate,stAFTUNE.rFLTh/2),stAFTUNE.rFLTh);

	rate = ((FLOAT)(AECTRL_GetGAINMax()/8)/(FLOAT)pCTRL->stFILT.GAIN)*stAFTUNE.rFLTb+0.5;
	rFLTb = MIN(MAX((SHORT)rate,stAFTUNE.rFLTb/2),stAFTUNE.rFLTb);

	rate = ((FLOAT)stAFAUTO.xFLTh/stAFAUTO.nFLTh-1)*stAFTUNE.rFLTh_NORMMUL;
	stAFAUTO.RATEh = ABS((LONG)((rate<0)? rate-0.5 : rate+0.5));

	rate = ((FLOAT)stAFAUTO.xFLTb/stAFAUTO.nFLTb-1)*stAFTUNE.rFLTb_NORMMUL; //1000
	stAFAUTO.RATEb = ABS((LONG)((rate<0)? rate-0.5 : rate+0.5));

	if (stAFAUTO.RATEh<(LONG)rFLTh) {stAFAUTO.CNTh  = 0;}
	if (stAFAUTO.RATEb<(LONG)rFLTb) {stAFAUTO.CNTb  = 0;}
#if __MISP100_AF_DBG__ == 1
	if ((stAFAUTO.RATEh>=rFLTh||stAFAUTO.RATEb>=rFLTb)&&((ODM_GetDEBUGMode()==0x04))) {
		UARTprintf("stAFAUTO.xFLTb : %d / stAFAUTO.nFLTb : %d\r\n", stAFAUTO.xFLTb, stAFAUTO.nFLTb);
		UARTprintf("[AFCORE_CtrlTriggerByNORM] nH:%06d rH:%06d", stAFAUTO.RATEh, rFLTh);
		UARTprintf(" nB:%06d rB:%06d\r\n", stAFAUTO.RATEb, rFLTb);
	}
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerByAUTO(PAF_CTRL_PARAM pCTRL)
{
	FLOAT rate, nYSUM, nGAIN;	SHORT rFLTh, rFLTb;

	if ((pCTRL->ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_MANUAL_MODE) return;
	if ((pCTRL->ctrl&AFCTRL_AFMODE_MASK)!=AFCTRL_AFAUTO_MODE) return;
	if (LENS_IsDriveEnableZOOM() || LENS_IsDriveEnableFOKS()) return;
	if (stAFAUTO.nIDX<NumOfBUFF) return;

	if (stAFAUTO.CNTh<stAFTUNE.rCNTh && stAFAUTO.CNTb<stAFTUNE.rCNTb) return;

	rate = ((FLOAT)(AECTRL_GetGAINMax()/8)/(FLOAT)pCTRL->stFILT.GAIN)*stAFTUNE.rFLTh+0.5; //16384
	rFLTh = MIN(MAX((SHORT)rate,stAFTUNE.rFLTh/1),stAFTUNE.rFLTh);

	rate = ((FLOAT)(AECTRL_GetGAINMax()/8)/(FLOAT)pCTRL->stFILT.GAIN)*stAFTUNE.rFLTb+0.5;
	rFLTb = MIN(MAX((SHORT)rate,stAFTUNE.rFLTb/1),stAFTUNE.rFLTb);

	stAFAUTO.CNTh = 0;	stAFAUTO.CNTb = 0;

	nYSUM = ((FLOAT)stAFAUTO.xYSUM)/pCTRL->stFILT.YSUM;
	nGAIN = ((FLOAT)stAFAUTO.xGAIN)/pCTRL->stFILT.GAIN;

	stAFAUTO.nFLTh = (LONG)(nYSUM*nGAIN*stAFAUTO.nFLTh+0.5);
	stAFAUTO.nFLTb = (LONG)(nYSUM*nGAIN*stAFAUTO.nFLTb+0.5);

	rate = ((FLOAT)stAFAUTO.xFLTh/stAFAUTO.nFLTh-1)*stAFTUNE.rFLTh_AUTOMUL;
	stAFAUTO.RATEh = ABS((LONG)((rate<0)? rate-0.5 : rate+0.5));

	rate = ((FLOAT)stAFAUTO.xFLTb/stAFAUTO.nFLTb-1)*stAFTUNE.rFLTb_AUTOMUL;
	stAFAUTO.RATEb = ABS((LONG)((rate<0)? rate-0.5 : rate+0.5));

	if (stAFAUTO.RATEh<(LONG)rFLTh && stAFAUTO.RATEb<(LONG)rFLTb) return;

#if __MISP100_AF_DBG__ == 1
	if ((ODM_GetDEBUGMode()==0x04)) {
		UARTprintf("stAFAUTO.xFLTb : %d / stAFAUTO.nFLTb : %d\r\n", stAFAUTO.xFLTb, stAFAUTO.nFLTb);
		UARTprintf("\r\n[AF-TRIGGER by FILT] nH:%05d rH:%05d", stAFAUTO.RATEh, rFLTh);
		UARTprintf(" nB:%05d rB:%05d\r\n", stAFAUTO.RATEb, rFLTb);
	}
#endif

	AF_EnableProcess(ON);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerByYSUM(PAF_CTRL_PARAM pCTRL)
{
	LONG nGAIN;	SHORT rGAPy = 5;

	if ((pCTRL->ctrl&AFCTRL_AFAUTO_MASK)==AFCTRL_MANUAL_MODE) return;
	if ((pCTRL->ctrl&AFCTRL_AFMODE_MASK)!=AFCTRL_AFAUTO_MODE) return;
	if (LENS_IsDriveEnableZOOM() || LENS_IsDriveEnableFOKS()) return;
	if (stAFAUTO.nIDX<NumOfBUFF) return;

	if (stAFAUTO.nGAIN==AECTRL_GetGAINMin()) stAFAUTO.CNTy = 0;
	if (stAFAUTO.nGAIN==AECTRL_GetGAINMax()) rGAPy = 20;

	stAFAUTO.yDIFF = ABS(stAFAUTO.oYSUM-stAFAUTO.nYSUM);
	if (stAFAUTO.yDIFF>=(LONG)rGAPy) stAFAUTO.CNTy  = 0;
	else							 stAFAUTO.CNTy += 1;

	if (stAFAUTO.CNTy<stAFTUNE.rCNTy)  return;

	nGAIN = AECTRL_GetGAINMax();	stAFAUTO.CNTy = 0;
	if (stAFAUTO.nGAIN==nGAIN)	rGAPy = stAFTUNE.rGAPy*1;
	else						rGAPy = stAFTUNE.rGAPy*2;

	stAFAUTO.yDIFF = ABS(stAFAUTO.xYSUM-stAFAUTO.nYSUM);
	if (stAFAUTO.yDIFF<(LONG)rGAPy) return;

#if __MISP100_AF_DBG__ == 1
	if ((ODM_GetDEBUGMode()==0x04)) {
		UARTprintf("\r\n[AF-TRIGGER by YSUM] nY:%05d xY:%05d dY:%05d nG:%05d\r\n",
			stAFAUTO.nYSUM, stAFAUTO.xYSUM, stAFAUTO.yDIFF, stAFAUTO.nGAIN);
	}
#endif

	AF_EnableProcess(ON);
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_CtrlTriggerByPROC(PAF_CTRL_PARAM pCTRL)
{
	if (stAFAUTO.nRST==0 || AF_IsProcessExecuting()) return;

	stAFAUTO.nRST = 0;
	AF_EnableProcess(ON);
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
	if (pCTRL->nSPT<stAFTUNE.rSPOTCnt) return;

	AFNORM_EnableProcess(OFF);
	AFDARK_EnableProcess(OFF);
	AFSPOT_EnableProcess(ON);
	//UARTprintf("SPOT TRIG on...\r\n");
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_TriggerHandler(PAF_CTRL_PARAM pCTRL)
{
	if (LENS_IsDriveExecuting() || GetDrvOnOffPINT() || GetAgeOnOffLENS()) return;

	AFCORE_CtrlTriggerByZOOM(pCTRL);	// AF trigger by ZOOM
	AFCORE_CtrlTriggerByBUFF(pCTRL);	// AF trigger by BUFF
	AFCORE_CtrlTriggerByLUMA(pCTRL);	// AF trigger by LUMA
	AFCORE_CtrlTriggerByDATA(pCTRL);	// AF trigger by DATA
	AFCORE_CtrlTriggerByNORM(pCTRL);	// AF trigger by NORM
	AFCORE_CtrlTriggerByAUTO(pCTRL);	// AF trigger by AUTO
	AFCORE_CtrlTriggerByYSUM(pCTRL);	// AF trigger by YSUM
	AFCORE_CtrlTriggerByPROC(pCTRL);	// AF trigger by PROC
	AFCORE_CtrlTriggerByTIME(pCTRL);	// AF trigger by TIME
	AFCORE_CtrlTriggerBySTOP(pCTRL);	// AF trigger by STOP
	AFCORE_CtrlTriggerBySPOT(pCTRL);	// AF trigger by SPOT
}

//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_MtrMoveHandler(PAF_CTRL_PARAM pCTRL)
{
	if (LENS_IsDriveExecuting()==FALSE) return;

	LENS_DrvHandlerFOKS(pCTRL);		// drive FOKS-range
	LENS_DrvHandlerZOOM(pCTRL);		// drive ZOOM-range
}

BYTE AF_TR_MODE = 0;
//--------------------------------------------------------------------------------------------------------------------------
static void AFCORE_BranchHandler(PAF_CTRL_PARAM pCTRL)
{
	FLOAT rate;	LONG yRATE;	PBYTE pCOEF;

	if (stAFAUTO.PROC==OFF) return;
	LENS_EnableMotorFOKS(ON);

	rate = ((FLOAT)pCTRL->stFILT.YSUM/AECTRL_GetBrightness()-1)*1000;
	yRATE = ABS((LONG)((rate*2<0)? rate*2-0.5 : rate*2+0.5));

	if (stAFAUTO.nCNT<30)	stAFAUTO.nCNT++;
	if (stAFAUTO.nCNT<30 && yRATE>stAFTUNE.rVALy) return;

	stAFAUTO.PROC = stAFAUTO.nCNT = 0;	pCTRL->xMOD = pCTRL->nMOD;

	// get norm-light define MODE
	if		(pCTRL->stFILT.GAIN>stAFTUNE.rDARKGainL)	pCTRL->nMOD  = ( DARK_LL); //65536
	else if	(pCTRL->stFILT.GAIN>stAFTUNE.rDARKGainM)	pCTRL->nMOD  = ( DARK_ML); //32768
	else if (pCTRL->stFILT.GAIN> stAFTUNE.rDARKGainH)	pCTRL->nMOD  = ( DARK_HL); //16384 for 290
	else								pCTRL->nMOD  = ( NORM_XX);

//	if (pCTRL->stFILT.CNTa<stAFTUNE.rSPOTGain)	// spot-light mode
	if (pCTRL->nSPT<stAFTUNE.rSPOTCnt) {
		pCTRL->nMOD &= (~SPOT_XX);
	} else {
		pCTRL->nMOD |= ( SPOT_XX);
	}

	if(AF_TR_MODE == 1) {
		pCTRL->nMOD  = ( NORM_XX);
		pCTRL->nSPT = 0;
	} else if(AF_TR_MODE == 2) {
		pCTRL->nMOD  = ( DARK_LL);
		pCTRL->nSPT = 0;
	} else if(AF_TR_MODE == 3) {
		pCTRL->nMOD = ( SPOT_XX);
		pCTRL->nSPT = stAFTUNE.rSPOTCnt;
	}

	if (pCTRL->nMOD&SPOT_XX)			AFSPOT_EnableProcess(ON);
	else if (pCTRL->nMOD==NORM_XX)	AFNORM_EnableProcess(ON);
	else								AFDARK_EnableProcess(ON);

	pCTRL->nINT = 0;	pCTRL->nACT = 0;	// clear time-counter

	if (LO4BIT(pCTRL->nMOD)==LO4BIT(pCTRL->xMOD))	return;

	// set AF-FILT(HPF)
	if		(LO4BIT(pCTRL->nMOD)==NORM_XX)	pCOEF = (PBYTE)stAFHBPF.HI;
//	else if (LO4BIT(pCTRL->nMOD)==DARK_HL)	pCOEF = (PBYTE)stAFHBPF.HI;
	else									pCOEF = (PBYTE)stAFHBPF.LO;

	MISP_MultiWrite(MISP_LOCAL_ID, 0x6B0, pCOEF, sizeof(stAFHBPF.HI));

	// set AF-FILT(LPF)
	if		(LO4BIT(pCTRL->nMOD)==NORM_XX)	pCOEF = (PBYTE)stAFLBPF.HI;
//	else if (LO4BIT(pCTRL->nMOD)==DARK_HL)	pCOEF = (PBYTE)stAFLBPF.HI;
	else									pCOEF = (PBYTE)stAFLBPF.LO;

	MISP_MultiWrite(MISP_LOCAL_ID, 0x6C7, pCOEF, sizeof(stAFHBPF.HI));
#if __MISP100_AF_DBG__ == 1
	if ((ODM_GetDEBUGMode()==0x04) || (ODM_GetDEBUGMode()==0x05)) {
		if (pCTRL->nMOD&SPOT_XX)			UARTprintf("AF SPOT...\r\n");
		else if (pCTRL->nMOD==NORM_XX)	UARTprintf("AF NORM...\r\n");
		else								UARTprintf("AF DARK...\r\n");
	}
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFCORE_RESETHandler(PAF_CTRL_PARAM pCTRL)
{
	fSysStateCHANGE = (pCTRL->ctrl&AFCTRL_RST_FRAME)? TRUE : FALSE;

	if ((pCTRL->ctrl&AFCTRL_RST_FRAME)==0) return FALSE;

#if __MISP100_AF_DBG__ == 1
	if ((ODM_GetDEBUGMode()==0x04)) UARTprintf("[AF-RESET by WDR]\r\n");
#endif

	stAFAUTO.nFRM = 0;	stAFAUTO.nEXE = 0;
	return (stAFAUTO.nEXE)? FALSE : TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL AFCORE_FRAMEHandler(PAF_CTRL_PARAM pCTRL)
{
	if (pCTRL->ctrl&AFCTRL_RST_FRAME) return TRUE;

	if (pCTRL->ctrl&AFCTRL_WDR_FRAME) {
		if (stAFAUTO.nFRM%2) stAFAUTO.nEXE = 1;
		else				 stAFAUTO.nEXE = 0;
	}
	else					 stAFAUTO.nEXE = 1;

	stAFAUTO.nFRM++;

	return (stAFAUTO.nEXE)? FALSE : TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
void AF_ProcessHandler(PAF_CTRL_PARAM pCTRL)
{
	if (AFCORE_RESETHandler(pCTRL)) return;	// inactive when WDR mode change
	if (AFCORE_FRAMEHandler(pCTRL)) return;	// inactive when WDR short frame

	AFCORE_GetAFOPDWIND(pCTRL);		// get AFOPD data

	AFCORE_TriggerHandler(pCTRL);	// AF trigger
	AFCORE_MtrMoveHandler(pCTRL);	// AF move range

	AFNORM_ProcessHandler(pCTRL);
	AFDARK_ProcessHandler(pCTRL);
	AFSPOT_ProcessHandler(pCTRL);

	AFCORE_BranchHandler(pCTRL);	// AF branch
}

//--------------------------------------------------------------------------------------------------------------------------
void AF_DisplayHandler(PAF_CTRL_PARAM pCTRL)
{
	WORD rODM[3], rVal[4], wVal[8];

	MISP_RegRead(MISP_LOCAL_ID, 0x752, &rODM[0]); //odm af window display enable
	if((rODM[0]&0x1f) == 0) return;

	MISP_MultiRead(MISP_LOCAL_ID, 0x60D, (PBYTE)&rODM, 6);
	MISP_MultiRead(MISP_LOCAL_ID, 0x6A0, (PBYTE)&rVal, 4);

	wVal[0] = rODM[0]+HIBYTE(rVal[0])*HIBYTE(rODM[2]);	// sX of WNDA
	wVal[1] = rODM[1]+LOBYTE(rVal[0])*LOBYTE(rODM[2]);	// sY of WNDA
	wVal[2] = wVal[0]+HIBYTE(rVal[1])*HIBYTE(rODM[2]);	// eX of WNDA
	wVal[3] = wVal[1]+LOBYTE(rVal[1])*LOBYTE(rODM[2]);	// eY of WNDA
	MISP_MultiWrite(MISP_LOCAL_ID, 0x75F, (PBYTE)wVal, 8); // odm window #4

	MISP_MultiRead(MISP_LOCAL_ID, 0x6A2, (PBYTE)&rVal, 4);
	wVal[0] = rODM[0]+HIBYTE(rVal[0])*HIBYTE(rODM[2]);	// sX of WNDB
	wVal[1] = rODM[1]+LOBYTE(rVal[0])*LOBYTE(rODM[2]);	// sY of WNDB
	wVal[2] = wVal[0]+HIBYTE(rVal[1])*HIBYTE(rODM[2]);	// eX of WNDB
	wVal[3] = wVal[1]+LOBYTE(rVal[1])*LOBYTE(rODM[2]);	// eY of WNDB

	MISP_MultiWrite(MISP_LOCAL_ID, 0x763, (PBYTE)wVal, 8); // odm window #5
}

//--------------------------------------------------------------------------------------------------------------------------
void AF_EnableProcess(BOOL OnOff)
{
	stAFAUTO.nCNT = 0;	stAFAUTO.nIDX = 0;
	stAFAUTO.CNTh = 0;	stAFAUTO.CNTb = 0;
	stAFAUTO.CNTy = 0;

	//UARTprintf("AF Active : %d\r\n",OnOff);
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
void AF_SetFILTCoef(void)
{
	MISP_MultiWrite(MISP_LOCAL_ID, 0x6B0, (PBYTE)stAFHBPF.HI, sizeof(stAFHBPF.HI));
	MISP_MultiWrite(MISP_LOCAL_ID, 0x6C7, (PBYTE)stAFLBPF.HI, sizeof(stAFLBPF.HI));
}

//--------------------------------------------------------------------------------------------------------------------------
void AF_SetAUTOInfo(PAF_CTRL_PARAM pCTRL, BOOL nCHK)
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
	PAF_FILT_PARAM pFILT = &pCTRL->stFILT;
	LONG diff = (LONG)(pFILT->YSUM-pFILT->YPRV);
	LONG yGAP = (LONG)(stAFTUNE.rGAPy*2);

	if (HIBYTE(nID)==TRUE)	stAFAUTO.nRST = TRUE;

	if (diff<=-yGAP && pFILT->YPRV<AECTRL_GetBrightness())
		stAFAUTO.nRST = TRUE;

	if (diff>= yGAP && pFILT->YPRV>AECTRL_GetBrightness())
		stAFAUTO.nRST = TRUE;

#if __MISP100_AF_DBG__ == 1
	if ((ODM_GetDEBUGMode()==0x04))
		UARTprintf("[AUTO-RESET by LUMA] diff:%06d yGAP:%06d\r\n", diff, yGAP);
#endif

	return (stAFAUTO.nRST)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL AF_GetDATAInfo(PAF_CTRL_PARAM pCTRL, BOOL nID)
{
	if (nID==TRUE)	stAFAUTO.nRST = TRUE;
	return (stAFAUTO.nRST)? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
LONG AF_GetFILTVal(PAF_CTRL_PARAM pCTRL, BYTE nID)
{
	if		(nID==0)	return pCTRL->stFILT.RPFa;
	else if (nID==1)	return pCTRL->stFILT.VPFa;
	else if (nID==2)	return pCTRL->stFILT.BPFa;
	else				return pCTRL->stFILT.HPFa;
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

#if __MISP100_AF_CNV__ == 1
//--------------------------------------------------------------------------------------------------------------------------
void AF_ConvertHandler(PAF_CTRL_PARAM pCTRL)
{
	WORD i, *pLUT = (PWORD)AF_ConvLUT;

	if ((AF_CtrlLUT&1)==0) return;

	if ((AF_CtrlLUT&2)==0) {	// write data
		UARTprintf("\r\n[LUT-WRITE]");

		MISP_RegWrite(MISP_LOCAL_ID, 0x6E0, 0x0000);	//
		MISP_RegWrite(MISP_LOCAL_ID, 0x6E0, 0x0001);	//

		for (i=0; i<128; i++) {
			MISP_RegWrite(MISP_LOCAL_ID, 0x6E1, pLUT[i]);	//
			if ((i%8)==0) UARTprintf("\r\n");
			UARTprintf("0x%04X ", pLUT[i]);
		}

		UARTprintf("\r\n");
	}
	else {
		UARTprintf("\r\n[LUT-READ]");

		MISP_RegWrite(MISP_LOCAL_ID, 0x6E0, 0x0002);	//
		MISP_RegWrite(MISP_LOCAL_ID, 0x6E0, 0x0003);	//

		for (i=0; i<128; i++) {
			MISP_RegRead(MISP_LOCAL_ID, 0x6E2, &pLUT[i]);	//
			if ((i%8)==0) UARTprintf("\r\n");
			UARTprintf("0x%04X ", pLUT[i]);
		}
		UARTprintf("\r\n");
	}

	AF_CtrlLUT = 0;
}
#endif	/* __MISP100_AF_CNV__ == 1 */
#endif

/*  FILE_END_HERE */
