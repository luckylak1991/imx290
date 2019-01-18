// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

typedef	struct
{
	WORD	yyy[2];	
	WORD	max[9];	
	WORD	prm[7];	
	WORD	gry[7];	
	WORD	ctl[7];	

}	WB_OPD_INFO, *PWB_OPD_INFO;

typedef	struct
{
	FLOAT	fY;

	LONG	Y;
	WORD	W;

	WORD	w_CNT;
	WORD	n_CNT;
	WORD	n_DMY;

	LONG	y_BUF[8];
	WORD	w_BUF[8];

}	WB_YYY_INFO, *PWB_YYY_INFO;

typedef	struct
{
	FLOAT	fR;
	FLOAT	fG;
	FLOAT	fB;
	FLOAT	fS;

	FLOAT	Y;
	FLOAT	U;
	FLOAT	V;

	LONG	R;
	LONG	G;
	LONG	B;
	LONG	W;

	LONG	w_CNT;
	WORD	n_CNT;
	WORD	n_DMY;

	LONG	r_BUF[8];
	LONG	g_BUF[8];
	LONG	b_BUF[8];
	LONG	w_BUF[8];

}	WB_CTL_INFO, *PWB_CTL_INFO;

typedef	struct
{
	CHAR	proc;
	CHAR	fCNT;
	CHAR	w_ID;
	CHAR	size;

	LONG	R;
	LONG	G;
	LONG	B;
	LONG	W;

	SHORT	gainR;
	SHORT	goalR;
	SHORT	stepR;
	SHORT	gainB;
	SHORT	goalB;
	SHORT	stepB;

	LONG	wdrR;
	LONG	wdrB;
	LONG	offR;
	LONG	offB;	

	WORD	wBLK;
	WORD	wCNT;

	WB_OPD_INFO 	stOPD;
	WB_YYY_INFO		stYYY;
	WB_CTL_INFO		stGRY;
	WB_CTL_INFO		stCTL;

}	WB_PROC_PARAM, *PWB_PROC_PARAM;

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static WB_PROC_PARAM stWBPROC;

static ROMDATA SHORT AWB_Speed[][4]		= {
//	 div, cnt, max, size
	{ 32,  49,   4,   8},		// 0
	{ 24,  33,   8,   7},		// 1
	{ 16,  17,   8,   6},		// 2
	{ 16,   9,  12,   5},		// 3
	{ 16,   5,  12,   4},		// 4
	{ 12,   3,  16,   3},		// 5
	{  8,   3,  16,   2},		// 6
	{  4,   1,  24,   2}		// 7
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
static void WBCTRL_GetOPDInfo(PWB_CTRL_PARAM pCTRL, PWB_PROC_PARAM pINFO)
{
	MISP_MultiRead(MISP_LOCAL_ID, 0x650, (PBYTE)pINFO->stOPD.yyy, 50);
	MISP_MultiRead(MISP_LOCAL_ID, 0x670, (PBYTE)pINFO->stOPD.ctl, 14);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_GetYYYInfo(PWB_CTRL_PARAM pCTRL, PWB_PROC_PARAM pINFO)
{
	WORD i, *rVal = (PWORD)stWBPROC.stOPD.yyy;
	PWB_YYY_INFO pYYY = (PWB_YYY_INFO)&pINFO->stYYY;

	pYYY->w_CNT = pYYY->W = (rVal[0]&0xfff0)>>4;
	if (pYYY->W<=2*pINFO->wCNT) {pYYY->n_CNT = 0; return;}

	for (i=1; i<stWBPROC.size; i++) {
		pYYY->y_BUF[i-1] = pYYY->y_BUF[i]; pYYY->w_BUF[i-1] = pYYY->w_BUF[i];
	}

	pYYY->y_BUF[stWBPROC.size-1] = MAKEDWORD((rVal[0]&0x0f), rVal[1]);
	pYYY->w_BUF[stWBPROC.size-1] = (rVal[0]&0xfff0)>>4;

	if (pYYY->n_CNT<stWBPROC.size) pYYY->n_CNT++;	
	if (pYYY->n_CNT<stWBPROC.size) return;	

	pYYY->Y = pYYY->W = 0;		// clear

	for (i=0; i<stWBPROC.size; i++) {
		pYYY->Y += pYYY->y_BUF[i];	pYYY->W += pYYY->w_BUF[i];
	}

	pYYY->fY = (FLOAT)pYYY->Y/pYYY->W;
	MISP_RegField(MISP_LOCAL_ID, 0x614, 0, 8, (WORD)pYYY->fY);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_GetGRYInfo(PWB_CTRL_PARAM pCTRL, PWB_PROC_PARAM pINFO)
{
	WORD i, *rVal = (PWORD)stWBPROC.stOPD.gry;
	PWB_YYY_INFO pYYY = (PWB_YYY_INFO)&pINFO->stYYY;
	PWB_CTL_INFO pGRY = (PWB_CTL_INFO)&pINFO->stGRY;

	pGRY->w_CNT = pGRY->W = rVal[6]&0x0fff;
	if (pYYY->w_CNT<=2*pINFO->wCNT) {pGRY->n_CNT = 0; return;}
	if (pGRY->W<=1*pINFO->wCNT) {pGRY->n_CNT = 0; return;}

	for (i=1; i<stWBPROC.size; i++) {
		pGRY->r_BUF[i-1] = pGRY->r_BUF[i];	pGRY->g_BUF[i-1] = pGRY->g_BUF[i];
		pGRY->b_BUF[i-1] = pGRY->b_BUF[i];	pGRY->w_BUF[i-1] = pGRY->w_BUF[i];
	}

	pGRY->r_BUF[stWBPROC.size-1] = MAKEDWORD((rVal[0]&0xff), rVal[1]);
	pGRY->g_BUF[stWBPROC.size-1] = MAKEDWORD((rVal[2]&0xff), rVal[3]);
	pGRY->b_BUF[stWBPROC.size-1] = MAKEDWORD((rVal[4]&0xff), rVal[5]);
	pGRY->w_BUF[stWBPROC.size-1] = MAKEDWORD((rVal[6]&0x00), rVal[6]);

	if (pGRY->n_CNT<stWBPROC.size) pGRY->n_CNT++;	
	if (pGRY->n_CNT<stWBPROC.size) return;	

	pGRY->R = pGRY->G = pGRY->B = pGRY->W = 0;	// clear

	for (i=0; i<stWBPROC.size; i++) {
		pGRY->R += pGRY->r_BUF[i]; pGRY->G += pGRY->g_BUF[i];
		pGRY->B += pGRY->b_BUF[i]; pGRY->W += pGRY->w_BUF[i];
	}

	pGRY->fR = (FLOAT)pGRY->R/pGRY->W;
	pGRY->fG = (FLOAT)pGRY->G/pGRY->W;
	pGRY->fB = (FLOAT)pGRY->B/pGRY->W;

	pGRY->R  = (LONG)((FLOAT)pGRY->R/stWBPROC.size+0.5);
	pGRY->G  = (LONG)((FLOAT)pGRY->G/stWBPROC.size+0.5);
	pGRY->B  = (LONG)((FLOAT)pGRY->B/stWBPROC.size+0.5);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_GetCTLInfo(PWB_CTRL_PARAM pCTRL, PWB_PROC_PARAM pINFO)
{
	WORD i, *rVal = (PWORD)stWBPROC.stOPD.ctl;
	PWB_CTL_INFO pGRY = (PWB_CTL_INFO)&pINFO->stGRY;
	PWB_CTL_INFO pCTL = (PWB_CTL_INFO)&pINFO->stCTL;

	pCTL->w_CNT = pCTL->W = rVal[6]&0x0fff;
	if (pGRY->w_CNT<=1*pINFO->wCNT) {pCTL->n_CNT = 0; return;}
	if (pCTL->W<=2*pINFO->wCNT) {pCTL->n_CNT = 0; return;}

	for (i=1; i<stWBPROC.size; i++) {
		pCTL->r_BUF[i-1] = pCTL->r_BUF[i];	pCTL->g_BUF[i-1] = pCTL->g_BUF[i];
		pCTL->b_BUF[i-1] = pCTL->b_BUF[i];	pCTL->w_BUF[i-1] = pCTL->w_BUF[i];
	}

	pCTL->r_BUF[stWBPROC.size-1] = MAKEDWORD((rVal[0]&0xff), rVal[1]);
	pCTL->g_BUF[stWBPROC.size-1] = MAKEDWORD((rVal[2]&0xff), rVal[3]);
	pCTL->b_BUF[stWBPROC.size-1] = MAKEDWORD((rVal[4]&0xff), rVal[5]);
	pCTL->w_BUF[stWBPROC.size-1] = MAKEDWORD((rVal[6]&0x00), rVal[6]);

	if (pCTL->n_CNT<stWBPROC.size) pCTL->n_CNT++;	
	if (pCTL->n_CNT<stWBPROC.size) return;	

	pCTL->R = pCTL->G = pCTL->B = pCTL->W = 0;	// clear

	for (i=0; i<stWBPROC.size; i++) {
		pCTL->R += pCTL->r_BUF[i]; pCTL->G += pCTL->g_BUF[i];
		pCTL->B += pCTL->b_BUF[i]; pCTL->W += pCTL->w_BUF[i];
	}

	pCTL->fR = (FLOAT)pCTL->R/pCTL->W;
	pCTL->fG = (FLOAT)pCTL->G/pCTL->W;
	pCTL->fB = (FLOAT)pCTL->B/pCTL->W;

	pCTL->R  = (LONG)((FLOAT)pCTL->R/stWBPROC.size+0.5);
	pCTL->G  = (LONG)((FLOAT)pCTL->G/stWBPROC.size+0.5);
	pCTL->B  = (LONG)((FLOAT)pCTL->B/stWBPROC.size+0.5);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_SetTGTInfo(PWB_CTRL_PARAM pCTRL, PWB_PROC_PARAM pINFO)
{
	PWB_CTL_INFO pGRY = (PWB_CTL_INFO)&pINFO->stGRY;
	PWB_CTL_INFO pCTL = (PWB_CTL_INFO)&pINFO->stCTL;
	FLOAT X = (pGRY->W)? (FLOAT)pCTL->W/pGRY->W : 0;

	if (pGRY->n_CNT<pINFO->size || pCTL->n_CNT<pINFO->size) return;

	pINFO->R = pGRY->R*X+pCTL->R; pINFO->G = pGRY->G*X+pCTL->G;
	pINFO->B = pGRY->B*X+pCTL->B; pINFO->W = pGRY->W*1+pCTL->W;

	if (pGRY->W<(1*pINFO->wCNT*pINFO->size)) return;
	if (pCTL->W<(2*pINFO->wCNT*pINFO->size)) return;

	pINFO->goalR = MIN(MAX(pINFO->G*(256+2*pCTRL->offR)/pINFO->R, 0x001), 0x7ff);
	pINFO->goalB = MIN(MAX(pINFO->G*(256+2*pCTRL->offB)/pINFO->B, 0x001), 0x7ff);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_SetCTLInfo(PWB_CTRL_PARAM pCTRL, PWB_PROC_PARAM pINFO)
{
	PWB_CTL_INFO pGRY = (PWB_CTL_INFO)&pINFO->stGRY;
	PWB_CTL_INFO pCTL = (PWB_CTL_INFO)&pINFO->stCTL;

	FLOAT stepR, diffR = DIFF(pINFO->goalR, pINFO->gainR);
	FLOAT stepB, diffB = DIFF(pINFO->goalB, pINFO->gainB);

	SHORT nID = ((pCTRL->ctrl&WBCTRL_SPEED_MASK)>>8)&0x0f;

	if (pGRY->W<(1*pINFO->wCNT*pINFO->size)) return;
	if (pCTL->W<(2*pINFO->wCNT*pINFO->size)) return;

	if ((pCTRL->ctrl&WBCTRL_MODE_MASK)==WBCTRL_WBHOLD_MODE) nID = 7;

	stepR = MAX(diffR/8,1.0f);	stepB = MAX(diffB/8,1.0f);

	if (diffR!=0 && diffR>diffB) {
		stepR = MIN(stepR,(FLOAT)AWB_Speed[nID][2]);
		stepB = MIN(stepB,(FLOAT)AWB_Speed[nID][2]*diffB/diffR);
	}

	if (diffB!=0 && diffR<diffB) {
		stepB = MIN(stepB,(FLOAT)AWB_Speed[nID][2]);
		stepR = MIN(stepR,(FLOAT)AWB_Speed[nID][2]*diffR/diffB);
	}

	pINFO->stepR = (diffR)? MAX((SHORT)stepR,1.0f) : 0;
	pINFO->stepB = (diffB)? MAX((SHORT)stepB,1.0f) : 0;
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_RESETHandler(PWB_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&WBCTRL_PROCESS_RST)==0) return;

	memset(&stWBPROC, 0, sizeof(WB_PROC_PARAM));
	pCTRL->ctrl &= (~WBCTRL_PROCESS_RST);
	pCTRL->ctrl |= ( WBCTRL_TRIGGER_ISP);

	WB_SetAWBDrvSPEED(pCTRL, pCTRL->ctrl&WBCTRL_SPEED_MASK);

	MISP_RegRead(MISP_LOCAL_ID, 0x608, (PWORD)&stWBPROC.gainR);
	MISP_RegRead(MISP_LOCAL_ID, 0x60A, (PWORD)&stWBPROC.gainB);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_FRAMEHandler(PWB_CTRL_PARAM pCTRL)
{
	if (stWBPROC.fCNT==16) stWBPROC.fCNT = 0;

	if ((pCTRL->ctrl&WBCTRL_WDR_MASK)==WBCTRL_WDR_FRAME) {
		if (stWBPROC.fCNT%2) stWBPROC.proc = stWBPROC.fCNT/2;
		else				 stWBPROC.proc = (CHAR)-1;
	}
	else					 stWBPROC.proc = stWBPROC.fCNT/1;

	if (stWBPROC.fCNT<16) stWBPROC.fCNT++;
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_ADAPTHandler(PWB_CTRL_PARAM pCTRL)
{
	if (stWBPROC.proc==(CHAR)-1) return;
	if (stWBPROC.proc%2) return;

	WBCTRL_GetOPDInfo(pCTRL, &stWBPROC);
	WBCTRL_GetYYYInfo(pCTRL, &stWBPROC);
	WBCTRL_GetGRYInfo(pCTRL, &stWBPROC);
	WBCTRL_GetCTLInfo(pCTRL, &stWBPROC);

	WBCTRL_SetTGTInfo(pCTRL, &stWBPROC);
	WBCTRL_SetCTLInfo(pCTRL, &stWBPROC);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_SetCOREGain(PWB_CTRL_PARAM pCTRL)
{
	WBCTRL_GetCOMPGain(pCTRL, 1);

	stWBPROC.wdrR = (LONG)pCTRL->stWBCOMP.sR*stWBPROC.gainR;
	stWBPROC.wdrB = (LONG)pCTRL->stWBCOMP.sB*stWBPROC.gainB;
	MISP_RegWrite(MISP_LOCAL_ID, 0x5AC, HIWORD(stWBPROC.wdrR));
	MISP_RegWrite(MISP_LOCAL_ID, 0x5AD, LOWORD(stWBPROC.wdrR));
	MISP_RegWrite(MISP_LOCAL_ID, 0x5B2, HIWORD(stWBPROC.wdrB));
	MISP_RegWrite(MISP_LOCAL_ID, 0x5B3, LOWORD(stWBPROC.wdrB));

	stWBPROC.offR  = (LONG)pCTRL->stWBCOMP.jR*stWBPROC.gainR;
	stWBPROC.offB  = (LONG)pCTRL->stWBCOMP.jB*stWBPROC.gainB;
	stWBPROC.offR += (stWBPROC.offR<0)? 65536*4-1 : 65536*4;
	stWBPROC.offB += (stWBPROC.offB<0)? 65536*4-1 : 65536*4;
	stWBPROC.offR  = (0x200000+(stWBPROC.offR>>19))&0x1fffff;
	stWBPROC.offB  = (0x200000+(stWBPROC.offB>>19))&0x1fffff;
	MISP_RegWrite(MISP_LOCAL_ID, 0x5F7, HIWORD(stWBPROC.offR));
	MISP_RegWrite(MISP_LOCAL_ID, 0x5F8, LOWORD(stWBPROC.offR));
	MISP_RegWrite(MISP_LOCAL_ID, 0x5F9, HIWORD(stWBPROC.offB));
	MISP_RegWrite(MISP_LOCAL_ID, 0x5FA, LOWORD(stWBPROC.offB));
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_SetCOMPGain(PWB_CTRL_PARAM pCTRL)
{
	WORD slop;		WBCTRL_GetCOMPGain(pCTRL, 0);

	switch (pCTRL->ctrl&WBCTRL_WDR_MASK) {
		case WBCTRL_WDR_2LINE:
		case WBCTRL_WDR_3LINE:
		case WBCTRL_WDR_4LINE: WBCTRL_GetCOMPGain(pCTRL, 1); break;	
		case WBCTRL_WDR_BUILT: WBCTRL_GetCOMPGain(pCTRL, 2); break;	
	}

	slop = ((HIWORD(pCTRL->stWBCOMP.jR)&0xf)<<12)|(pCTRL->stWBCOMP.sR&0x7ff);
	MISP_RegWrite(MISP_LOCAL_ID, 0x602, slop);
	MISP_RegWrite(MISP_LOCAL_ID, 0x603, LOWORD(pCTRL->stWBCOMP.jR));

	slop = ((HIWORD(pCTRL->stWBCOMP.jB)&0xf)<<12)|(pCTRL->stWBCOMP.sB&0x7ff);
	MISP_RegWrite(MISP_LOCAL_ID, 0x606, slop);
	MISP_RegWrite(MISP_LOCAL_ID, 0x607, LOWORD(pCTRL->stWBCOMP.jB));
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_SetPOLYInfo(PWB_CTRL_PARAM pCTRL)
{
	switch (pCTRL->ctrl&WBCTRL_MODE_MASK) {
		case WBCTRL_OTDOOR_MODE: WBCTRL_GetPOLYInfo(pCTRL, 1); break;
		case WBCTRL_INDOOR_MODE: WBCTRL_GetPOLYInfo(pCTRL, 2); break;
		default:				 WBCTRL_GetPOLYInfo(pCTRL, 0); break;
	}
	MISP_MultiWrite(MISP_LOCAL_ID, 0x620, (PBYTE)&pCTRL->stWBPOLY, 88);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_SetAUTOGain(PWB_CTRL_PARAM pCTRL) // auto gain update
{
	SHORT diffR = DIFF(stWBPROC.goalR, stWBPROC.gainR);
	SHORT diffB = DIFF(stWBPROC.goalB, stWBPROC.gainB);
	SHORT signR = (stWBPROC.goalR<stWBPROC.gainR)? -1 : 1;
	SHORT signB = (stWBPROC.goalB<stWBPROC.gainB)? -1 : 1;

	if (stWBPROC.W<16) return;

	stWBPROC.gainR += MIN(diffR, stWBPROC.stepR)*signR;
	stWBPROC.gainB += MIN(diffB, stWBPROC.stepB)*signB;

	stWBPROC.gainR = MIN(MAX(stWBPROC.gainR, 0x001), 0x7ff);
	stWBPROC.gainB = MIN(MAX(stWBPROC.gainB, 0x001), 0x7ff);

	MISP_RegWrite(MISP_LOCAL_ID, 0x608, (WORD)stWBPROC.gainR);
	MISP_RegWrite(MISP_LOCAL_ID, 0x60A, (WORD)stWBPROC.gainB);

#if (defined (__PANORAMA_SETUP__) && defined(__PANORAMA_02__)) || \
	(defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__))	// 4k60 master only
	SPImAPP_SetSlaveWBgain((WORD)stWBPROC.gainR, (WORD)stWBPROC.gainB);
#endif
	
	WBCTRL_SetCOREGain(pCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL WBCTRL_AUTOProcess(PWB_CTRL_PARAM pCTRL)
{
	if (pCTRL->ctrl&WBCTRL_PROCESS_OFF) return FALSE;

//	if(AECTRL_GetGAINRatio()) return FALSE;

	WBCTRL_SetAUTOGain(pCTRL);

	if (stWBPROC.gainR!=stWBPROC.goalR) return FALSE;
	if (stWBPROC.gainB!=stWBPROC.goalB) return FALSE;
	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_WBAUTOHandler(PWB_CTRL_PARAM pCTRL)
{
	if (pCTRL->ctrl&WBCTRL_TRIGGER_ISP) {
		pCTRL->ctrl &= (~WBCTRL_TRIGGER_ISP);
		pCTRL->ctrl &= (~WBCTRL_PROCESS_OFF);

		MISP_RegRead(MISP_LOCAL_ID, 0x608, (PWORD)&stWBPROC.gainR);
		MISP_RegRead(MISP_LOCAL_ID, 0x60A, (PWORD)&stWBPROC.gainB);
		MISP_RegRead(MISP_LOCAL_ID, 0x611, (PWORD)&stWBPROC.wBLK);

		stWBPROC.wCNT = HIBYTE(stWBPROC.wBLK)*LOBYTE(stWBPROC.wBLK);
		stWBPROC.wCNT = stWBPROC.wCNT/50;	// 2%

		WBCTRL_SetCOMPGain(pCTRL);
		WBCTRL_SetPOLYInfo(pCTRL);
	}
	else	WBCTRL_AUTOProcess(pCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_WBHOLDHandler(PWB_CTRL_PARAM pCTRL)
{
	if (pCTRL->ctrl&WBCTRL_TRIGGER_ISP) {	
		pCTRL->ctrl &= (~WBCTRL_TRIGGER_ISP);
		pCTRL->ctrl |= ( WBCTRL_PROCESS_OFF);

		stWBPROC.gainR = pCTRL->stWBHOLD.R;	
		stWBPROC.gainB = pCTRL->stWBHOLD.B;
		MISP_RegWrite(MISP_LOCAL_ID, 0x608, pCTRL->stWBHOLD.R);
		MISP_RegWrite(MISP_LOCAL_ID, 0x60A, pCTRL->stWBHOLD.B);

		WBCTRL_SetCOREGain(pCTRL);	
		WBCTRL_SetCOMPGain(pCTRL);	
		WBCTRL_SetPOLYInfo(pCTRL);	
		
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveWBgain((WORD)stWBPROC.gainR, (WORD)stWBPROC.gainB);
#endif
	}

	if (WBCTRL_AUTOProcess(pCTRL)==TRUE) {	
		pCTRL->stWBHOLD.R = stWBPROC.gainR;
		pCTRL->stWBHOLD.B = stWBPROC.gainB;
		pCTRL->ctrl |= ( WBCTRL_PROCESS_OFF);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_INDOORHandler(PWB_CTRL_PARAM pCTRL)
{
	if (pCTRL->ctrl&WBCTRL_TRIGGER_ISP) {
		pCTRL->ctrl &= (~WBCTRL_TRIGGER_ISP);
		pCTRL->ctrl &= (~WBCTRL_PROCESS_OFF);

		stWBPROC.gainR = pCTRL->stINDOOR.R;	
		stWBPROC.gainB = pCTRL->stINDOOR.B;
		MISP_RegWrite(MISP_LOCAL_ID, 0x608, pCTRL->stINDOOR.R);
		MISP_RegWrite(MISP_LOCAL_ID, 0x60A, pCTRL->stINDOOR.B);

		WBCTRL_SetCOREGain(pCTRL);
		WBCTRL_SetCOMPGain(pCTRL);
		WBCTRL_SetPOLYInfo(pCTRL);
		
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveWBgain((WORD)stWBPROC.gainR, (WORD)stWBPROC.gainB);
#endif
	}
	else	WBCTRL_AUTOProcess(pCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_OTDOORHandler(PWB_CTRL_PARAM pCTRL)
{
	if (pCTRL->ctrl&WBCTRL_TRIGGER_ISP) {
		pCTRL->ctrl &= (~WBCTRL_TRIGGER_ISP);
		pCTRL->ctrl &= (~WBCTRL_PROCESS_OFF);

		stWBPROC.gainR = pCTRL->stOTDOOR.R;	
		stWBPROC.gainB = pCTRL->stOTDOOR.B;
		MISP_RegWrite(MISP_LOCAL_ID, 0x608, pCTRL->stOTDOOR.R);
		MISP_RegWrite(MISP_LOCAL_ID, 0x60A, pCTRL->stOTDOOR.B);

		WBCTRL_SetCOREGain(pCTRL);
		WBCTRL_SetCOMPGain(pCTRL);
		WBCTRL_SetPOLYInfo(pCTRL);
		
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveWBgain((WORD)stWBPROC.gainR, (WORD)stWBPROC.gainB);
#endif
	}
	else	WBCTRL_AUTOProcess(pCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_PRESETHandler(PWB_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&WBCTRL_TRIGGER_ISP)==0) return;
	pCTRL->ctrl &= (~WBCTRL_TRIGGER_ISP);
	pCTRL->ctrl |= ( WBCTRL_PROCESS_OFF);

	stWBPROC.gainR = pCTRL->stPRESET.R;	
	stWBPROC.gainB = pCTRL->stPRESET.B;
	MISP_RegWrite(MISP_LOCAL_ID, 0x608, pCTRL->stPRESET.R);
	MISP_RegWrite(MISP_LOCAL_ID, 0x60A, pCTRL->stPRESET.B);

	WBCTRL_SetCOREGain(pCTRL);
	WBCTRL_SetCOMPGain(pCTRL);
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveWBgain((WORD)stWBPROC.gainR, (WORD)stWBPROC.gainB);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBCTRL_MANUALHandler(PWB_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&WBCTRL_TRIGGER_ISP)==0) return;
	pCTRL->ctrl &= (~WBCTRL_TRIGGER_ISP);
	pCTRL->ctrl |= ( WBCTRL_PROCESS_OFF);

	stWBPROC.gainR = pCTRL->stMANUAL.R;
	stWBPROC.gainB = pCTRL->stMANUAL.B;
	MISP_RegWrite(MISP_LOCAL_ID, 0x608, pCTRL->stMANUAL.R);
	MISP_RegWrite(MISP_LOCAL_ID, 0x60A, pCTRL->stMANUAL.B);

	WBCTRL_SetCOREGain(pCTRL);	
	WBCTRL_SetCOMPGain(pCTRL);	
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveWBgain((WORD)stWBPROC.gainR, (WORD)stWBPROC.gainB);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void WB_ProcessHandler(PWB_CTRL_PARAM pCTRL)
{
	WBCTRL_RESETHandler(pCTRL);	
	WBCTRL_FRAMEHandler(pCTRL);	
	WBCTRL_ADAPTHandler(pCTRL);	

	switch (pCTRL->ctrl&WBCTRL_MODE_MASK) {
		case WBCTRL_WBAUTO_MODE: WBCTRL_WBAUTOHandler(pCTRL); break;
		case WBCTRL_WBHOLD_MODE: WBCTRL_WBHOLDHandler(pCTRL); break;
		case WBCTRL_INDOOR_MODE: WBCTRL_INDOORHandler(pCTRL); break;
		case WBCTRL_OTDOOR_MODE: WBCTRL_OTDOORHandler(pCTRL); break;
		case WBCTRL_PRESET_MODE: WBCTRL_PRESETHandler(pCTRL); break;
		case WBCTRL_MANUAL_MODE: WBCTRL_MANUALHandler(pCTRL); break;
		default:				 WBCTRL_WBAUTOHandler(pCTRL); break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void WB_DisplayHandler(PWB_CTRL_PARAM pCTRL)
{
	WORD rODM[3], rVal[2], wVal[4];

	MISP_RegRead(MISP_LOCAL_ID, 0x752, &rODM[0]);
	if((rODM[0]&0x1f) == 0) return;

	MISP_MultiRead(MISP_LOCAL_ID, 0x60D, (PBYTE)&rODM, 6);
	MISP_MultiRead(MISP_LOCAL_ID, 0x610, (PBYTE)&rVal, 4);

	wVal[0] = rODM[0]+HIBYTE(rVal[0])*HIBYTE(rODM[2]);
	wVal[1] = rODM[1]+LOBYTE(rVal[0])*LOBYTE(rODM[2]);
	wVal[2] = wVal[0]+HIBYTE(rVal[1])*HIBYTE(rODM[2]);
	wVal[3] = wVal[1]+LOBYTE(rVal[1])*LOBYTE(rODM[2]);

	MISP_MultiWrite(MISP_LOCAL_ID, 0x757, (PBYTE)wVal, 8);
}

//--------------------------------------------------------------------------------------------------------------------------
void WB_SetAWBDrvSPEED(PWB_CTRL_PARAM pCTRL, LONG val)
{
	stWBPROC.size = (BYTE)AWB_Speed[(val>>8)&0x0f][3];
	pCTRL->ctrl &= (~WBCTRL_SPEED_MASK);
	pCTRL->ctrl |= val;
}

//--------------------------------------------------------------------------------------------------------------------------
void WB_SetAWBDrvSHIFT(PWB_CTRL_PARAM pCTRL, SHORT offR, SHORT offB)
{
	pCTRL->offR = offR;		pCTRL->offB = offB;
}

//--------------------------------------------------------------------------------------------------------------------------
void WB_GetAWBSumVALUE(PLONG R, PLONG G, PLONG B, PLONG W)
{
	*R = stWBPROC.R; *G = stWBPROC.G; *B = stWBPROC.B; *W = stWBPROC.W;
}

//--------------------------------------------------------------------------------------------------------------------------
void WB_GetAWBGryVALUE(PLONG R, PLONG G, PLONG B, PLONG W)
{
	*R = stWBPROC.stGRY.R; *G = stWBPROC.stGRY.G;
	*B = stWBPROC.stGRY.B; *W = stWBPROC.stGRY.W;
}

//--------------------------------------------------------------------------------------------------------------------------
void WB_GetAWBCtlVALUE(PLONG R, PLONG G, PLONG B, PLONG W)
{
	*R = stWBPROC.stCTL.R; *G = stWBPROC.stCTL.G;
	*B = stWBPROC.stCTL.B; *W = stWBPROC.stCTL.W;
}

//--------------------------------------------------------------------------------------------------------------------------
void WBCTRL_SetAWBCtrlPath(BOOL ishtm, BOOL RBslope, BOOL RBscomp, BOOL DCgain, BOOL WBgain )
{
	BYTE write_buf ;
	
	write_buf = (RBslope<<7)|(RBscomp<<6)|(DCgain<<5)|(WBgain<<4) ;
	
	if ( ishtm == 1 ) write_buf = write_buf>>4 ;
	
	MISP_RegField(MISP_LOCAL_ID, 0x60C, 0, 8, write_buf) ;	
	
}

/*  FILE_END _HERE */
