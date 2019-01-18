// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<math.h>
#include	<string.h>
#include	"video.h"
#include	"menu.h"

#define __MISP100_COLOR_DBG__ 0 // debug only

#define	OSD_COLOR	MAKEBYTE(OSD_WHITE, OSD_NOCOLOR)

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

typedef	struct
{
	SHORT	sX;	
	SHORT	sY;	
	SHORT	eX;	
	SHORT	eY;	

}	COLOR_AREA_PARAM, *PCOLOR_AREA_PARAM;

typedef	struct
{
	BYTE	ctrl;			

	SHORT	offX;			
	SHORT	offY;			

	WORD	nPXL;			
	WORD	nODR;			
	LONG	raw[2][2];
	WORD	buffWord[64];
	WORD	buffWord_odd[64];

	COLOR_AREA_PARAM	stAREA;	
	COLOR_AREA_PARAM	stWIND;	

}	COLOR_VIEW_PARAM, *PCOLOR_VIEW_PARAM;

#define		WBVIEW_TRIGGER_ODDRAW		0x04

#define		ROUND(a)	(LONG)(((a)>0)? ((a)+0.5) : ((a)-0.5))

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
#if defined(__USE_GET_AWB_RGB_INFO_SIZE_4__) && (defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__)|| defined(__USE_CMOS_IMX334__))
#define colorviewWinSizeH 64
#define colorviewWinSizeV 30
#define numSearchArea 15
#else
#define colorviewWinSizeH 64
#define colorviewWinSizeV 64
#define numSearchArea 13
#endif

#if !defined(__USE_GET_RGB_MODE__)
static BYTE colorSpotCnt=0;
#endif

static COLOR_VIEW_PARAM stCOLORVIEW;

#if defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__)
#define captureWinFactor 8
#define hColorW 3840/captureWinFactor
#define hColorH 2160/captureWinFactor
#else
#define captureWinFactor 1
#define hColorW 1920/captureWinFactor
#define hColorH 1080/captureWinFactor
#endif

#if defined(__USE_GET_AWB_RGB_INFO_SIZE_4__) && (defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__))
static COLOR_AREA_PARAM stCOLORSPOT00 = {64*0,		0,		64*0+colorviewWinSizeH,		0+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT01 = {64*1,		0,		64*1+colorviewWinSizeH,		0+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT02 = {64*2,		0,		64*2+colorviewWinSizeH,		0+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT03 = {64*3,		0,		64*3+colorviewWinSizeH,		0+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT04 = {64*4,		0,		64*4+colorviewWinSizeH,		0+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT05 = {64*5,		0,		64*5+colorviewWinSizeH,		0+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT06 = {64*6,		0,		64*6+colorviewWinSizeH,		0+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT07 = {64*7,		0,		64*7+colorviewWinSizeH,		0+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT08 = {64*8,		0,		64*8+colorviewWinSizeH,		0+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT09 = {64*9,		0,		64*9+colorviewWinSizeH,		0+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT10 = {64*10,		0,		64*10+colorviewWinSizeH,	0+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT11 = {64*11,		0,		64*11+colorviewWinSizeH,	0+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT12 = {64*12,		0,		64*12+colorviewWinSizeH,	0+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT13 = {64*13,		0,		64*13+colorviewWinSizeH,	0+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT14 = {64*14,		0,		64*14+colorviewWinSizeH,	0+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT15 = {64*14,		0,		64*14+colorviewWinSizeH,	0+colorviewWinSizeV};
#else
static COLOR_AREA_PARAM stCOLORSPOT00 = {hColorW/8*4,	hColorH/8*4,		hColorW/8*4+colorviewWinSizeH,	hColorH/8*4+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT01 = {hColorW/8*1,	hColorH/8*1,		hColorW/8*1+colorviewWinSizeH,	hColorH/8*1+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT02 = {hColorW/8*7,	hColorH/8*1,		hColorW/8*7+colorviewWinSizeH,	hColorH/8*1+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT03 = {hColorW/8*1,	hColorH/8*7,		hColorW/8*1+colorviewWinSizeH,	hColorH/8*7+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT04 = {hColorW/8*7,	hColorH/8*7,		hColorW/8*7+colorviewWinSizeH,	hColorH/8*7+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT05 = {hColorW/8*4,	hColorH/8*1,		hColorW/8*4+colorviewWinSizeH,	hColorH/8*1+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT06 = {hColorW/8*1,	hColorH/8*4,		hColorW/8*1+colorviewWinSizeH,	hColorH/8*4+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT07 = {hColorW/8*7,	hColorH/8*4,		hColorW/8*7+colorviewWinSizeH,	hColorH/8*4+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT08 = {hColorW/8*4,	hColorH/8*7,		hColorW/8*4+colorviewWinSizeH,	hColorH/8*7+colorviewWinSizeV};

static COLOR_AREA_PARAM stCOLORSPOT09 = {hColorW/16*5,	hColorH/16*5,		hColorW/16*5+colorviewWinSizeH,	hColorH/16*5+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT10 = {hColorW/16*11,	hColorH/16*5,		hColorW/16*11+colorviewWinSizeH,	hColorH/16*5+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT11 = {hColorW/16*5,	hColorH/16*11,	hColorW/16*5+colorviewWinSizeH,	hColorH/16*11+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT12 = {hColorW/16*11,	hColorH/16*11,	hColorW/16*11+colorviewWinSizeH,	hColorH/16*11+colorviewWinSizeV};

static COLOR_AREA_PARAM stCOLORSPOT13 = {hColorW/16*5,	hColorH/16*2,		hColorW/16*5+colorviewWinSizeH,	hColorH/16*2+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT14 = {hColorW/16*11,	hColorH/16*2,		hColorW/16*11+colorviewWinSizeH,	hColorH/16*2+colorviewWinSizeV};
static COLOR_AREA_PARAM stCOLORSPOT15 = {hColorW/16*2,	hColorH/16*5,		hColorW/16*2+colorviewWinSizeH,	hColorH/16*5+colorviewWinSizeV};
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
void AWB_ModeChange(BOOL mode) // mode 1 : normal  mode 0 : gray world for RGB calculation
{
	WORD tmpBuf;

	MISP_RegRead(MISP_LOCAL_ID, 0x600, &tmpBuf);

	if(mode) {
		tmpBuf &= 0xfff3;
		tmpBuf |= 0x8;
	} else {
		tmpBuf &= 0xfff3;
	}

	//UARTprintf("0x600 : %x\r\n",tmpBuf);

	MISP_RegWrite(MISP_LOCAL_ID, 0x600, tmpBuf);

#if 0//defined(__USE_GET_RGB_MODE__)
	WORD R,G,B;

	MISP_RegRead(MISP_LOCAL_ID, 0x671, &tmpBuf);
	stWBCTRL.stWBCOLOR.fAvgR = tmpBuf;
	MISP_RegRead(MISP_LOCAL_ID, 0x670, &tmpBuf);
	stWBCTRL.stWBCOLOR.fAvgR |= (tmpBuf<<15);

	MISP_RegRead(MISP_LOCAL_ID, 0x673, &tmpBuf);
	stWBCTRL.stWBCOLOR.fAvgG = tmpBuf;
	MISP_RegRead(MISP_LOCAL_ID, 0x672, &tmpBuf);
	stWBCTRL.stWBCOLOR.fAvgG |= (tmpBuf<<15);

	MISP_RegRead(MISP_LOCAL_ID, 0x675, &tmpBuf);
	stWBCTRL.stWBCOLOR.fAvgB = tmpBuf;
	MISP_RegRead(MISP_LOCAL_ID, 0x674, &tmpBuf);
	stWBCTRL.stWBCOLOR.fAvgB |= (tmpBuf<<15);

	UARTprintf("R:%d / G:%d / B:%d\r\n",stWBCTRL.stWBCOLOR.fAvgR,stWBCTRL.stWBCOLOR.fAvgG,stWBCTRL.stWBCOLOR.fAvgB);
#endif

}

//--------------------------------------------------------------------------------------------------------------------------
static void ColorRAW_GetBayerPXL(PWB_CTRL_PARAM pCTRL)
{
#if defined(__USE_GET_RGB_MODE__)
	WORD tmpBuf;

	MISP_RegRead(MISP_LOCAL_ID, 0x671, &tmpBuf);
	stWBCTRL.stWBCOLOR.fAvgR = tmpBuf;
	MISP_RegRead(MISP_LOCAL_ID, 0x670, &tmpBuf);
	stWBCTRL.stWBCOLOR.fAvgR |= (tmpBuf<<15);

	MISP_RegRead(MISP_LOCAL_ID, 0x673, &tmpBuf);
	stWBCTRL.stWBCOLOR.fAvgG = tmpBuf;
	MISP_RegRead(MISP_LOCAL_ID, 0x672, &tmpBuf);
	stWBCTRL.stWBCOLOR.fAvgG |= (tmpBuf<<15);

	MISP_RegRead(MISP_LOCAL_ID, 0x675, &tmpBuf);
	stWBCTRL.stWBCOLOR.fAvgB = tmpBuf;
	MISP_RegRead(MISP_LOCAL_ID, 0x674, &tmpBuf);
	stWBCTRL.stWBCOLOR.fAvgB |= (tmpBuf<<15);

	//UARTprintf("R:%d / G:%d / B:%d\r\n",stWBCTRL.stWBCOLOR.fAvgR,stWBCTRL.stWBCOLOR.fAvgG,stWBCTRL.stWBCOLOR.fAvgB);

	pCTRL->stWBCOLOR.fAvgR = pCTRL->stWBCOLOR.fAvgR*512/pCTRL->stWBCOMP.sR;
	pCTRL->stWBCOLOR.fAvgB = pCTRL->stWBCOLOR.fAvgB*512/pCTRL->stWBCOMP.sB;

	pCTRL->stWBCOLOR.fAvgY = (LONG)(0.299f*pCTRL->stWBCOLOR.fAvgR+0.587f*pCTRL->stWBCOLOR.fAvgG+0.114f*pCTRL->stWBCOLOR.fAvgB);
	pCTRL->stWBCOLOR.fAvgU = (LONG)(-0.299f*pCTRL->stWBCOLOR.fAvgR-0.587f*pCTRL->stWBCOLOR.fAvgG+0.886f*pCTRL->stWBCOLOR.fAvgB);
	pCTRL->stWBCOLOR.fAvgV = (LONG)(0.701f*pCTRL->stWBCOLOR.fAvgR-0.587f*pCTRL->stWBCOLOR.fAvgG-0.114f*pCTRL->stWBCOLOR.fAvgB);
	//UARTprintf("R:%4d / G:%4d / B:%4d : ", pCTRL->stWBCOLOR.fAvgR, pCTRL->stWBCOLOR.fAvgG, pCTRL->stWBCOLOR.fAvgB);
	//UARTprintf("Y:%4d / U:%4d / V:%4d\r\n", pCTRL->stWBCOLOR.fAvgY, pCTRL->stWBCOLOR.fAvgU, pCTRL->stWBCOLOR.fAvgV);
	//UARTprintf("\r\n");

	pCTRL->stWBCOLOR.cntColor = 0;

#else
	BYTE R,G,B;

	switch(colorSpotCnt) {
		case 0 :	DEMO_byrcap_area(stCOLORSPOT00.sX, stCOLORSPOT00.sY, colorviewWinSizeH, colorviewWinSizeV, &R,&G,&B); break;
		case 1 :	DEMO_byrcap_area(stCOLORSPOT01.sX, stCOLORSPOT01.sY, colorviewWinSizeH, colorviewWinSizeV, &R,&G,&B); break;
		case 2 :	DEMO_byrcap_area(stCOLORSPOT02.sX, stCOLORSPOT02.sY, colorviewWinSizeH, colorviewWinSizeV, &R,&G,&B); break;
		case 3 :	DEMO_byrcap_area(stCOLORSPOT03.sX, stCOLORSPOT03.sY, colorviewWinSizeH, colorviewWinSizeV, &R,&G,&B); break;
		case 4 :	DEMO_byrcap_area(stCOLORSPOT04.sX, stCOLORSPOT04.sY, colorviewWinSizeH, colorviewWinSizeV, &R,&G,&B); break;
		case 5 :	DEMO_byrcap_area(stCOLORSPOT05.sX, stCOLORSPOT05.sY, colorviewWinSizeH, colorviewWinSizeV, &R,&G,&B); break;
		case 6 :	DEMO_byrcap_area(stCOLORSPOT06.sX, stCOLORSPOT06.sY, colorviewWinSizeH, colorviewWinSizeV, &R,&G,&B); break;
		case 7 :	DEMO_byrcap_area(stCOLORSPOT07.sX, stCOLORSPOT07.sY, colorviewWinSizeH, colorviewWinSizeV, &R,&G,&B); break;
		case 8 :	DEMO_byrcap_area(stCOLORSPOT08.sX, stCOLORSPOT08.sY, colorviewWinSizeH, colorviewWinSizeV, &R,&G,&B); break;
		case 9 :	DEMO_byrcap_area(stCOLORSPOT09.sX, stCOLORSPOT09.sY, colorviewWinSizeH, colorviewWinSizeV, &R,&G,&B); break;
		case 10 :	DEMO_byrcap_area(stCOLORSPOT10.sX, stCOLORSPOT10.sY, colorviewWinSizeH, colorviewWinSizeV, &R,&G,&B); break;
		case 11 :	DEMO_byrcap_area(stCOLORSPOT11.sX, stCOLORSPOT11.sY, colorviewWinSizeH, colorviewWinSizeV, &R,&G,&B); break;
		case 12 :	DEMO_byrcap_area(stCOLORSPOT12.sX, stCOLORSPOT12.sY, colorviewWinSizeH, colorviewWinSizeV, &R,&G,&B); break;
		case 13 :	DEMO_byrcap_area(stCOLORSPOT13.sX, stCOLORSPOT13.sY, colorviewWinSizeH, colorviewWinSizeV, &R,&G,&B); break;
		case 14 :	DEMO_byrcap_area(stCOLORSPOT14.sX, stCOLORSPOT14.sY, colorviewWinSizeH, colorviewWinSizeV, &R,&G,&B); break;
		case 15 :	
		default :	DEMO_byrcap_area(stCOLORSPOT15.sX, stCOLORSPOT15.sY, colorviewWinSizeH, colorviewWinSizeV, &R,&G,&B); break;
	}
	//UARTprintf("R:%4d / G:%4d / B:%4d\r\n", R, G, B);

	if(colorSpotCnt == 0) {
		pCTRL->stWBCOLOR.fAvgR = (LONG)R; //stWBVIEW.stCOMP.sR
		pCTRL->stWBCOLOR.fAvgG = (LONG)G;
		pCTRL->stWBCOLOR.fAvgB = (LONG)B;
	} else {
		pCTRL->stWBCOLOR.fAvgR += (LONG)R; //stWBVIEW.stCOMP.sR
		pCTRL->stWBCOLOR.fAvgG += (LONG)G;
		pCTRL->stWBCOLOR.fAvgB += (LONG)B;
	}

	if(colorSpotCnt == (numSearchArea-1)) {
		//pCTRL->stWBCOLOR.fAvgR /= numSearchArea;
		//pCTRL->stWBCOLOR.fAvgG /= numSearchArea;
		//pCTRL->stWBCOLOR.fAvgB /= numSearchArea;

		//pCTRL->stWBCOLOR.fAvgR = pCTRL->stWBCOLOR.fAvgR*pCTRL->stWBCOMP.sR/512;
		//pCTRL->stWBCOLOR.fAvgB = pCTRL->stWBCOLOR.fAvgB*pCTRL->stWBCOMP.sB/512;
		//UARTprintf("AWB comp : rGain=%d bGain=%d\r\n",pCTRL->stWBCOMP.sR,pCTRL->stWBCOMP.sB);

		pCTRL->stWBCOLOR.fAvgR = pCTRL->stWBCOLOR.fAvgR*512/pCTRL->stWBCOMP.sR;
		pCTRL->stWBCOLOR.fAvgB = pCTRL->stWBCOLOR.fAvgB*512/pCTRL->stWBCOMP.sB;

		pCTRL->stWBCOLOR.fAvgY = (LONG)(0.299f*pCTRL->stWBCOLOR.fAvgR+0.587f*pCTRL->stWBCOLOR.fAvgG+0.114f*pCTRL->stWBCOLOR.fAvgB);
		pCTRL->stWBCOLOR.fAvgU = (LONG)(-0.299f*pCTRL->stWBCOLOR.fAvgR-0.587f*pCTRL->stWBCOLOR.fAvgG+0.886f*pCTRL->stWBCOLOR.fAvgB);
		pCTRL->stWBCOLOR.fAvgV = (LONG)(0.701f*pCTRL->stWBCOLOR.fAvgR-0.587f*pCTRL->stWBCOLOR.fAvgG-0.114f*pCTRL->stWBCOLOR.fAvgB);
		//UARTprintf("R:%4d / G:%4d / B:%4d : ", pCTRL->stWBCOLOR.fAvgR, pCTRL->stWBCOLOR.fAvgG, pCTRL->stWBCOLOR.fAvgB);
		//UARTprintf("Y:%4d / U:%4d / V:%4d\r\n", pCTRL->stWBCOLOR.fAvgY, pCTRL->stWBCOLOR.fAvgU, pCTRL->stWBCOLOR.fAvgV);
		//UARTprintf("\r\n");
	}

	colorSpotCnt = (colorSpotCnt+1)%numSearchArea;
//	pCTRL->stWBCOLOR.cntColor++;
	pCTRL->stWBCOLOR.cntColor = (DWORD)colorSpotCnt;

#endif

#if __MISP100_COLOR_DBG__ == 1
		int i;
		if(getStatusOSDOnOff() == OFF) {
			for(i=0;i<19;i++)
				OSD_StrDispAttb(i,0,ON, OSD_COLOR, "                                    ");
			//OSD_StrDispAttb(2,0,ON, OSD_COLOR, " R%04d G%04d B%04d", pCTRL->stWBCOLOR.fAvgR, pCTRL->stWBCOLOR.fAvgG, pCTRL->stWBCOLOR.fAvgB);
			OSD_StrDispAttb(2,0,ON, OSD_COLOR, "U%04d V%04d",pCTRL->stWBCOLOR.fAvgU, pCTRL->stWBCOLOR.fAvgV);
			OSD_StrDispOnOff(ON);
		}
#endif

}

//--------------------------------------------------------------------------------------------------------------------------
static void ColorRAW_DisplayVIEW(BYTE mode)
{
	if(GetWBRAWOnOffMODE()==ON) return;

	switch(mode) {
		case 0 :	memcpy(&stCOLORVIEW.stWIND, &stCOLORSPOT00, sizeof(COLOR_AREA_PARAM));break;
		case 1 :	memcpy(&stCOLORVIEW.stWIND, &stCOLORSPOT01, sizeof(COLOR_AREA_PARAM));break;
		case 2 :	memcpy(&stCOLORVIEW.stWIND, &stCOLORSPOT02, sizeof(COLOR_AREA_PARAM));break;
		case 3 :	memcpy(&stCOLORVIEW.stWIND, &stCOLORSPOT03, sizeof(COLOR_AREA_PARAM));break;
		case 4 :	memcpy(&stCOLORVIEW.stWIND, &stCOLORSPOT04, sizeof(COLOR_AREA_PARAM));break;
		case 5 :	memcpy(&stCOLORVIEW.stWIND, &stCOLORSPOT05, sizeof(COLOR_AREA_PARAM));break;
		case 6 :	memcpy(&stCOLORVIEW.stWIND, &stCOLORSPOT06, sizeof(COLOR_AREA_PARAM));break;
		case 7 :	memcpy(&stCOLORVIEW.stWIND, &stCOLORSPOT07, sizeof(COLOR_AREA_PARAM));break;
		case 8 :	memcpy(&stCOLORVIEW.stWIND, &stCOLORSPOT08, sizeof(COLOR_AREA_PARAM));break;
		case 9 :	memcpy(&stCOLORVIEW.stWIND, &stCOLORSPOT09, sizeof(COLOR_AREA_PARAM));break;
		case 10 :	memcpy(&stCOLORVIEW.stWIND, &stCOLORSPOT10, sizeof(COLOR_AREA_PARAM));break;
		case 11 :	memcpy(&stCOLORVIEW.stWIND, &stCOLORSPOT11, sizeof(COLOR_AREA_PARAM));break;
		case 12 :	memcpy(&stCOLORVIEW.stWIND, &stCOLORSPOT12, sizeof(COLOR_AREA_PARAM));break;
		case 13 :	memcpy(&stCOLORVIEW.stWIND, &stCOLORSPOT13, sizeof(COLOR_AREA_PARAM));break;
		case 14 :	memcpy(&stCOLORVIEW.stWIND, &stCOLORSPOT14, sizeof(COLOR_AREA_PARAM));break;
		default :	memcpy(&stCOLORVIEW.stWIND, &stCOLORSPOT14, sizeof(COLOR_AREA_PARAM));break;
	}
#if __MISP100_COLOR_DBG__ == 1
	MISP_RegField(MISP_LOCAL_ID, 0x752, 2, 1, 1);
#endif
	MISP_MultiWrite(MISP_LOCAL_ID, 0x75B, (PBYTE)&stCOLORVIEW.stWIND, 8);
}

//--------------------------------------------------------------------------------------------------------------------------
void Color_ProcessHandler(PWB_CTRL_PARAM pCTRL)
{
	if(GetWBRAWOnOffMODE()==ON) return;

	ColorRAW_GetBayerPXL(pCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
void SetColorRAWViewTrigger(void)
{
	if(GetWBRAWOnOffMODE()==ON) return;
#if !defined(__USE_GET_RGB_MODE__)
	ColorRAW_DisplayVIEW(colorSpotCnt);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void ColorRAW_ProcessInit(void)
{
	memset((PBYTE)&stCOLORVIEW, 0, sizeof(COLOR_VIEW_PARAM));

	ColorRAW_DisplayVIEW(4);
}

/*  FILE_END _HERE */
