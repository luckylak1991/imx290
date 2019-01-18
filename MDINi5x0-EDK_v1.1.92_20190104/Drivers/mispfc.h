//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MISPFC.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISPFC_H__
#define		__MISPFC_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

typedef	enum {
	MISP_MFCFILT_HY = 0,		// horizontal interpolation filter for Y
	MISP_MFCFILT_HC,			// horizontal interpolation filter for C
	MISP_MFCFILT_VY,			// vertical interpolation filter for Y
	MISP_MFCFILT_VC,			// vertical interpolation filter for C
	MISP_MFCFILT_VP				// vertical peaking filter
}	MISP_MFC_FILTER_t;

typedef	enum {
	MISP_MFCCOEF_SPLN_1_00 = 0,  // mfc itp coefficient (Spline, B=0.00, C=1.00, 4-tap, 32-phase)
	MISP_MFCCOEF_SPLN_0_75,      // mfc itp coefficient (Spline, B=0.00, C=0.75, 4-tap, 32-phase)
	MISP_MFCCOEF_SPLN_0_50,      // mfc itp coefficient (Spline, B=0.00, C=0.50, 4-tap, 32-phase)
	MISP_MFCCOEF_BI_LINEAR,      // mfc itp coefficient (Bi-linear (2-tap), 32-phase)
	MISP_MFCCOEF_MOST_BLUR,      // mfc itp coefficient (Spline, B=1.00, C=0.00, 4-tap, 32-phase)
	MISP_MFCCOEF_MORE_BLUR,      // mfc itp coefficient (Spline, B=0.50, C=0.00, 4-tap, 32-phase)
	MISP_MFCCOEF_LANC_0_50,      // mfc itp coefficient (Lanczos, S=0.50, 4-tap, 32-phase)
	MISP_MFCCOEF_VPF_BPF50_8,      // mfc itp coefficient (BPF for vertical peaking filter coefficients fshift 0.5)
	MISP_MFCCOEF_VPF_BPF40_8,      // mfc itp coefficient (BPF for vertical peaking filter coefficients fshift 0.4)
	MISP_MFCCOEF_VPF_BPF_6,      // mfc itp coefficient (BPF for vertical peaking filter coefficients)
}	MISP_MFC_COEF_t;

typedef	struct
{
	WORD	w;			// Horizontal size of video area
	WORD	h;			// Vertical size of video area
	WORD	x;			// Horizontal position of the top-left corner of video area
	WORD	y;			// Vertical position of the top-left corner of video area

}	MISP_VIDEO_WINDOW, *PMISP_VIDEO_WINDOW;

typedef 	struct
{
	BYTE				mirror;		// mirrorring (0:off, 1:hori, 2:vert, 3:h+v)
	BYTE				seamless;	// seamless control (load_size_en)
	WORD				overscan_h;	// horizontal overscan value
	WORD				overscan_v;	// vertical overscan value
	BYTE				aspect;		// aspect ratio(0:full, 1:4:3,,,)
	BYTE				dummy1;		// dummy to align 32bit pointer

	MISP_VIDEO_WINDOW	stSRC;		// source window
	MISP_VIDEO_WINDOW	stDST;		// destination window

}	MISP_MFCSCALE_INFO, *PMISP_MFCSCALE_INFO;

#if defined(__USE_MDIN_i500__)		// MDIN-i500
typedef	struct
{
	WORD	lower[64];		// coefficients of lower 32-phase of mfc filter
	WORD	upper[64];		// coefficients of upper 32-phase of mfc filter

}	MISP_MFCFILT_COEF, *PMISP_MFCFILT_COEF;
#else								// MDIN-i51X (i510, i540, i550)
typedef	WORD	MISP_MFCFILT_COEF, *PMISP_MFCFILT_COEF;
#endif

#if defined(__USE_MDIN_i500__)		// MDIN-i500
typedef	struct
{
	WORD	hy[5];			// coefficients of aux H-filter for Y
	WORD	hc[4];			// coefficients of aux H-filter for C
	WORD	vy[3];			// coefficients of aux V-filter for Y
	WORD	vc[2];			// coefficients of aux V-filter for C
	
}	stPACKED MISP_AUXFILT_COEF, *PMISP_AUXFILT_COEF;
#else 								// MDIN-i51X (i510, i540, i550)
typedef	struct
{
	WORD	hy[8];			// coefficients of aux H-filter for Y	// i51x : 15-tap
	WORD	hc[4];			// coefficients of aux H-filter for C	// i51x : 7-tap
	WORD	vy[8];			// coefficients of aux V-filter for Y	// i51x : 15/7/5-tap
	WORD	vc[3];			// coefficients of aux V-filter for C	// i51x : 5/none/3-tap
	
}	stPACKED MISP_AUXFILT_COEF, *PMISP_AUXFILT_COEF;
#endif

typedef	struct
{
	BYTE				FltSel_HY;		// selection of HY coef table (horizontal Y)
	BYTE				FltSel_HC;		// selection of HC coef table (horizontal C)
	BYTE				FltSel_VY;		// selection of VY coef table (vertical Y)
	BYTE				FltSel_VC;		// selection of VC coef table (vertical C)
	BYTE				FltSel_VP;		// selection of VP coef table (vertical peaking)
	BYTE				dummy0;			// dummy to align 32bit pointer
	WORD				dummy1;			// dummy to align 32bit pointer

	BYTE				VPFsat_value;	// VPF satuaration value
	BYTE				VPFcor_value;	// VPF coring value
	BOOL				VPFsat_en;		// VPF saturation enable
	BOOL				VPFcor_en;		// VPF corring enable
	BYTE				VPFlevel;		// output gain of vpf, 0~64(100%)
	BOOL				v_peak_en;		// enable vertical peaking filter
	WORD				dummy2;			// dummy to align 32bit pointer

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	BOOL				VPFinverse;		// subtract VPF result from VY when VPF is low-pass
	BOOL				VPFcor_half_en;
	BOOL				level_depend_en;
	BYTE				level_depend_th;
	WORD				level_depend_slope;
	BYTE				level_depend_offset;
	BYTE				dummy3;			// dummy to align 32bit pointer
#endif

	BOOL				h_peak_en;		// enable horizontal peaking filter	
	WORD				dummy4;			// dummy to align 32bit pointer
}	MISP_MFCFILT_INFO, *PMISP_MFCFILT_INFO;

typedef	struct
{
	// PMISP_AUXFILT_COEF	pCoef;			// coefficient of aux(HY, HC, VY, VC)
	BYTE				Coef;			// coefficient of aux(HY, HC, VY, VC)	// pointer link to table ID
	BOOL				hflt_y;			// Enable control of 9-tap horizontal filter for luminance
	BOOL				hflt_c;			// Enable control of 7-tap horizontal filter for chrominance
	BOOL				vflt_y;			// Enable control of 5-tap vertical FIR filter for luminance
	BOOL				vflt_c;			// Enable control of 3-tap vertical FIR filter for chrominance
}	MISP_AUXFILT_INFO, *PMISP_AUXFILT_INFO;


typedef struct {
	LONG	ctrl;					// input control flag

	MISP_MFCSCALE_INFO	stMFC_m;	// scaler info of main path, but only use in API function
	MISP_MFCSCALE_INFO	stMFC_x;	// scaler info of aux path, but only use in API function
	MISP_MFCFILT_INFO	stFLT_m;	// control info of mfc filter
	MISP_AUXFILT_INFO	stFLT_x;	// control info of aux filter

}	FC_CTRL_PARAM, *PFC_CTRL_PARAM;

// control parameter
#define		FCCTRL_TRIGGER_ISP			0x40000000		// ISP update
#define		FCCTRL_UPDATE_MAIN			0x00000001		// fc main update
#define		FCCTRL_UPDATE_AUX			0x00000002		// fc aux update
#define		FCCTRL_UPDATE_MAIN_FLT		0x00000004		// fc main update
#define		FCCTRL_UPDATE_AUX_FLT		0x00000008		// fc aux update
#define		FCCTRL_UPDATE_MAIN_COEF_HY	0x00000010		// fc mfc filter coefficient update (horizontal Y)
#define		FCCTRL_UPDATE_MAIN_COEF_HC	0x00000020		// fc mfc filter coefficient update (horizontal C)
#define		FCCTRL_UPDATE_MAIN_COEF_VY	0x00000040		// fc mfc filter coefficient update (vertical Y)
#define		FCCTRL_UPDATE_MAIN_COEF_VC	0x00000080		// fc mfc filter coefficient update (vertical C)
#define		FCCTRL_UPDATE_MAIN_COEF_VP	0x00000100		// fc mfc filter coefficient update (vertical peaking)
#define		FCCTRL_UPDATE_AUX_COEF		0x00000200		// fc aux filter coefficient update
#define		FCCTRL_UPDATE_MAIN_VPF			0x00000200		// fc VPF level slope  etc. adjust (vertical peaking)

#define		FCCTRL_UPDATE_ALL			0x000003FF		// fc all(main,aux) update

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void Init_FC(void);
void FC_ProcessHandler(void);

void SetFC_MainLoadSizeEN(BOOL nID);
void SetFC_MainFCsrcSizeH(WORD val);
void SetFC_MainFCsrcSizeV(WORD val);
void SetFC_MainFCsrcPosiX(WORD val);
void SetFC_MainFCsrcPosiY(WORD val);
void SetFC_MainFCsrcOverscanH(WORD val);
void SetFC_MainFCsrcOverscanV(WORD val);
void SetFC_MainFCdstSizeH(WORD val);
void SetFC_MainFCdstSizeV(WORD val);
WORD GetFC_MainFCdstSizeH(void);
WORD GetFC_MainFCdstSizeV(void);
void SetFC_MainFCdstPosiX(WORD val);
void SetFC_MainFCdstPosiY(WORD val);
void SetFC_MainVPFLevel(WORD val);
BYTE GetFC_MainVPFLevel(void);
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
void SetFC_MainVPFLDthresh(WORD val);
void SetFC_MainVPFLDslope(WORD val);
#endif

void SetFC_StopMainFC(BYTE nID);

void SetFC_AuxLoadSizeEN(BOOL nID);
void SetFC_AuxFCsrcSizeH(WORD val);
void SetFC_AuxFCsrcSizeV(WORD val);
void SetFC_AuxFCsrcPosiX(WORD val);
void SetFC_AuxFCsrcPosiY(WORD val);
void SetFC_AuxFCdstSizeH(WORD val);
void SetFC_AuxFCdstSizeV(WORD val);
void SetFC_AuxFCdstPosiX(WORD val);
void SetFC_AuxFCdstPosiY(WORD val);
void SetFC_StopAuxFC(WORD nID);
WORD GetFC_AuxFCdstSizeH(void);
WORD GetFC_AuxFCdstSizeV(void);

WORD GetFC_MainFCsrcPosiX(void);
WORD GetFC_MainFCsrcPosiY(void);
WORD GetFC_MainFCsrcSizeH(void);
WORD GetFC_MainFCsrcSizeV(void);
WORD GetFC_MainFCdstPosiX(void);
WORD GetFC_MainFCdstPosiY(void);

WORD GetFC_AuxFCsrcPosiX(void);
WORD GetFC_AuxFCsrcPosiY(void);
WORD GetFC_AuxFCsrcSizeH(void);
WORD GetFC_AuxFCsrcSizeV(void);
WORD GetFC_AuxFCdstPosiX(void);
WORD GetFC_AuxFCdstPosiY(void);

void SetFC_AuxFilterHY(BOOL OnOff);
void SetFC_AuxFilterHC(BOOL OnOff);
void SetFC_AuxFilterVY(BOOL OnOff);
void SetFC_AuxFilterVC(BOOL OnOff);
BOOL GetFC_AuxFilterHY(void);
BOOL GetFC_AuxFilterHC(void);
BOOL GetFC_AuxFilterVY(void);
BOOL GetFC_AuxFilterVC(void);

void SetFC_MainFilterCoefHY(BYTE val);
void SetFC_MainFilterCoefHC(BYTE val);
void SetFC_MainFilterCoefVY(BYTE val);
void SetFC_MainFilterCoefVC(BYTE val);
void SetFC_MainFilterCoefVP(BYTE val);
BYTE GetFC_MainFilterCoefHY(void);
BYTE GetFC_MainFilterCoefHC(void);
BYTE GetFC_MainFilterCoefVY(void);
BYTE GetFC_MainFilterCoefVC(void);
BYTE GetFC_MainFilterCoefVP(void);

void SetFC_AuxFilterCoef(BYTE val);

void SetFC_MainHoriPeaking(BOOL OnOff);
void SetFC_MainVertPeaking(BOOL OnOff);
BOOL GetFC_MainHoriPeaking(void);
BOOL GetFC_MainVertPeaking(void);

void SetFC_MainMirror(BYTE mode);
BYTE GetFC_MainMirror(void);
void SetFC_AuxMirror(BYTE mode);
BYTE GetFC_AuxMirror(void);
BOOL GetFC_MainFFC_OnOff(void);
void SetFC_AuxAspect(BYTE mode);

#endif	/* __MISPFC_H__ */

