// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<math.h>
#include	<string.h>
#include	"video.h"
#include	"menu.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

typedef	struct
{
	SHORT	sX;	
	SHORT	sY;	
	SHORT	eX;	
	SHORT	eY;	

}	WB_AREA_PARAM, *PWB_AREA_PARAM;

typedef	struct
{
	LONG	R;
	LONG	Gr;
	LONG	B;
	LONG	Gb;

	FLOAT	U;
	FLOAT	V;

}	WB_AREA_PIXEL, *PWB_AREA_PIXEL;

typedef	struct
{
	LONG	R[2];
	LONG	G[2];
	LONG	B[2];
	LONG	W[2];

	FLOAT	U;
	FLOAT	V;

}	WB_DATA_WHITE, *PWB_DATA_WHITE;

typedef	struct
{
	LONG	tACT;
	LONG	nCNT;

}	WB_TIME_PARAM, *PWB_TIME_PARAM;

typedef	struct
{
	BYTE	ctrl;			
	BYTE	nID;			
	SHORT	step;			

	SHORT	offX;			
	SHORT	offY;			

	WORD	nPXL;			
	WORD	nODR;			
	LONG	raw[2][2];
#if defined(__USE_MDIN_i500__) 			// MDIN-i500
	BYTE	buffByte[128];
#else 									// MDIN-i51X (i510, i540, i550)
	WORD	buffWord[64];
	WORD	buffWord_odd[64];
#endif
	WORD	nLINE[44];

	WB_AREA_PARAM	stAREA;	
	WB_AREA_PARAM	stWIND;	
	WB_TIME_PARAM	stTIME;	
	WB_COMP_PARAM	stCOMP;	
	WB_AREA_PIXEL	stPXL[9];
	WB_DATA_WHITE	stDATA;

}	WB_VIEW_PARAM, *PWB_VIEW_PARAM;

typedef	struct
{
	CHAR	U[8];
	CHAR	D[8];
	CHAR	P[2];
	CHAR	L[2];

}	WB_TUNE_PARAM, *PWB_TUNE_PARAM;

#define		WBVIEW_PROCESS_RUN			0x80
#define		WBVIEW_POLYGON_RUN			0x40
#define		WBVIEW_DISPLAY_RUN			0x20
#define		WBVIEW_TIMECNT_RUN			0x10
#define		WBVIEW_TRIGGER_ISP			0x08
#define		WBVIEW_TRIGGER_ODDRAW		0x04

#define		OSD_COLOR		MAKEBYTE(OSD_WHITE, OSD_NOCOLOR)
#define		YEL_COLOR		MAKEBYTE(OSD_YELLOW, OSD_NOCOLOR)
#define		GRY_COLOR		MAKEBYTE(OSD_GRAY, OSD_NOCOLOR)
#define		ROUND(a)	(LONG)(((a)>0)? ((a)+0.5) : ((a)-0.5))

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static WB_VIEW_PARAM stWBVIEW;

#if defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__) || defined(__USE_CMOS_IMX385__)
static WB_AREA_PARAM stSPOT = { 928,  524,  992,  588};
#elif defined(__USE_CMOS_IMX226__)
static WB_AREA_PARAM stSPOT = {1888, 1048, 1952, 1112};
#else
static WB_AREA_PARAM stSPOT = { 928,  524,  992,  588};
//static WB_AREA_PARAM stSPOT = {1888, 1048, 1952, 1112};
#endif

static WB_TUNE_PARAM stWBTUNE[3]	=	{
	{
		{(CHAR) 13,(CHAR) 15,(CHAR) 17,(CHAR) 18,(CHAR) 20,(CHAR) 22,(CHAR) 23,(CHAR) 23},
		{(CHAR)-13,(CHAR)-13,(CHAR)-13,(CHAR)-13,(CHAR)-13,(CHAR)-13,(CHAR)-13,(CHAR)-13},
		{(CHAR)  1,(CHAR)  8 															},
		{(CHAR)  1,(CHAR)  7															}
 	},
	{										
		{(CHAR)  6,(CHAR)  6,(CHAR)  6,(CHAR)  6,(CHAR)  6,(CHAR)  6,(CHAR)  6,(CHAR)  6},
		{(CHAR) -6,(CHAR) -6,(CHAR) -6,(CHAR) -6,(CHAR) -6,(CHAR) -6,(CHAR) -6,(CHAR) -6},
		{(CHAR)  1,(CHAR)  8															},
		{(CHAR)  1,(CHAR)  7															}
 	},
	{										
		{(CHAR)  6,(CHAR)  6,(CHAR)  6,(CHAR)  6,(CHAR)  6,(CHAR)  6,(CHAR)  6,(CHAR)  6},
		{(CHAR) -6,(CHAR) -6,(CHAR) -6,(CHAR) -6,(CHAR) -6,(CHAR) -6,(CHAR) -6,(CHAR) -6},
		{(CHAR)  5,(CHAR)  7															},
		{(CHAR)  5,(CHAR)  6															}
 	}
};

static ROMDATA CHAR pSTR[][5]		=	{
	"LBA23", "LBA16", "LBA12", "LBA08", "LBA04", "5100K", "LBB04", "LBB10",
	"2300K", "2800K", "3200K", "3600K", "4100K", "5100K", "6200K", "9999K",
	"DONE!"
};

static CHAR  fSysRAWModeTIME = OFF, fSysRAWModeSAVE = OFF, str1[] = "     ";
static BYTE  fSysRAWModeCOMP = OFF, fSysRAWModePOLY = OFF, str2[] = "     ";

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
static void WBRAW_DisplayPOLY(BYTE nIDX)
{
	WORD i, U, V, sR, sB;	DWORD R, Gr, B, Gb;

	if (fSysRAWModePOLY==OFF && nIDX!=4) return;

	else if (nIDX==0) {
		memcpy((PBYTE)str1, pSTR[stWBVIEW.nID+0], sizeof(str1)-1);
		memcpy((PBYTE)str2, pSTR[stWBVIEW.nID+8], sizeof(str2)-1);
#if __MISP100_AWB_DBG__ == 1
		UARTprintf("\r\n[POLY] Find UV for Polygon MODE!!!");
		UARTprintf("\r\n[POLY] Please Install FILT <%s[%s]>\r\n", str1,str2);
#endif
		OSD_StrDispAttb(0,0,ON, OSD_COLOR, " UV for POLYGON-MODE       ");
		OSD_StrDispAttb(1,0,ON, YEL_COLOR, " PREPARE FILT: <%s[%s]>", str1,str2);
		for (i=0; i<19; i++) OSD_StrDispAttb(i+2,0,ON, OSD_COLOR, "                                ");
		OSD_StrDispOnOff(ON);
	}

	else if (nIDX==1) {
#if __MISP100_AWB_DBG__ == 1
		UARTprintf("\r\n[POLY] Find UV for Polygon RUN!!!\r\n");
#endif
		OSD_StrDispAttb(0,0,ON, OSD_COLOR, " UV for POLYGON-RUN        ");
		for (i=0; i<20; i++) OSD_StrDispAttb(i+1,0,ON, OSD_COLOR, "                                ");
	}

	else if (nIDX==2) {
		memcpy((PBYTE)str1, pSTR[stWBVIEW.nID+0], sizeof(str1)-1);
		memcpy((PBYTE)str2, pSTR[stWBVIEW.nID+8], sizeof(str2)-1);
#if __MISP100_AWB_DBG__ == 1
		UARTprintf("\r\n[POLY] Find UV for <%s[%s]> END!!! ", str1,str2);
#endif
		OSD_StrDispAttb(0,0,ON, OSD_COLOR, " GET UV for <%s[%s]> ", str1,str2);

		memcpy((PBYTE)str1, pSTR[stWBVIEW.nID+1], sizeof(str1)-1);
		memcpy((PBYTE)str2, pSTR[stWBVIEW.nID+9], sizeof(str2)-1);
#if __MISP100_AWB_DBG__ == 1
		UARTprintf("=> NEXT <%s[%s]>\r\n", str1,str2);
#endif
		OSD_StrDispAttb(1,0,ON, YEL_COLOR, " NXT UV for <%s[%s]> ", str1,str2);
	}

	else if (nIDX==3) {
#if __MISP100_AWB_DBG__ == 1
		UARTprintf("[POLY] Find UV for polygon END!!!\r\n");
#endif
		OSD_StrDispAttb( 0,0,ON, OSD_COLOR, " UV for POLYGON-END        ");
		OSD_StrDispAttb( 1,0,ON, OSD_COLOR, "--------------------       ");
		OSD_StrDispAttb(10,0,ON, GRY_COLOR, "(  R  ) ( G_r ) (  B  ) ( G_b )");

		for (i=0; i<8; i++) {
			memcpy((PBYTE)str2, (PBYTE)pSTR[i+8], sizeof(str2)-1);
			U = (WORD)ROUND(stWBVIEW.stPXL[i].U*256);
			V = (WORD)ROUND(stWBVIEW.stPXL[i].V*256);
			R = (DWORD)(stWBVIEW.stPXL[i].R); Gr = (DWORD)(stWBVIEW.stPXL[i].Gr);
			B = (DWORD)(stWBVIEW.stPXL[i].B); Gb = (DWORD)(stWBVIEW.stPXL[i].Gb);
#if __MISP100_AWB_DBG__ == 1
			UARTprintf("%04X %04X %07d %07d %07d %07d\r\n", U,V,R,Gr,B,Gb);
#endif
			OSD_StrDispAttb(i+ 2,0,ON, OSD_COLOR, "%s> U:%04X V:%04X", str2,U,V);
			OSD_StrDispAttb(i+11,0,ON, OSD_COLOR, "%07d %07d %07d %07d ", R,Gr,B,Gb);

			stWBCTRL.stWBCOLOR.U[i] = U;	// for MDINAPP
			stWBCTRL.stWBCOLOR.V[i] = V;	// for MDINAPP
		}

		sR = stWBVIEW.stCOMP.sR;	sB = stWBVIEW.stCOMP.sB;
#if __MISP100_AWB_DBG__ == 1
		UARTprintf("(sR, sB) = (%03d, %03d)\r\n", sR,sB);
#endif
		OSD_StrDispAttb(7,21,ON, YEL_COLOR, " (%03d %03d) ", sR,sB);
	}

	else if (nIDX==4) {
#if __MISP100_AWB_DBG__ == 1
		UARTprintf("\r\n[POLY] Find UV for Polygon DONE!!!");
#endif
		OSD_StrDispAttb(0,0,ON, OSD_COLOR, " UV for POLYGON-DONE       ");
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBRAW_DisplayCOMP(BYTE nIDX)
{
	WORD i, U, V, sR, sB;	DWORD R, G, B, W, Gr, Gb;

	if (fSysRAWModeCOMP==OFF && nIDX!=4) return;

	else if (nIDX==0) {
#if __MISP100_AWB_DBG__ == 1
		UARTprintf("\r\n[PreWB] Find R/B Correction MODE!!!\r\n");
#endif
		OSD_StrDispAttb(0,0,ON, OSD_COLOR, " <R/B CORRECTION-MODE>");
		for (i=0; i<11; i++) OSD_StrDispAttb(i+1,0,ON, OSD_COLOR, "                                ");
		OSD_StrDispOnOff(ON);
	}

	else if (nIDX==1) {
#if __MISP100_AWB_DBG__ == 1
		UARTprintf("\r\n[PreWB] Find R/B Correction RUN!!!\r\n");
#endif
		OSD_StrDispAttb(0,0,ON, OSD_COLOR, " <R/B CORRECTION-RUN> ");
		for (i=0; i<11; i++) OSD_StrDispAttb(i+1,0,ON, OSD_COLOR, "                                ");
	}

	else if (nIDX==2) {
		R = (DWORD)(stWBVIEW.stPXL[8].R); Gr = (DWORD)(stWBVIEW.stPXL[8].Gr);
		B = (DWORD)(stWBVIEW.stPXL[8].B); Gb = (DWORD)(stWBVIEW.stPXL[8].Gb);
		U = (WORD)ROUND(stWBVIEW.stPXL[8].U*256); sR = (WORD)stWBVIEW.stCOMP.sR;
		V = (WORD)ROUND(stWBVIEW.stPXL[8].V*256); sB = (WORD)stWBVIEW.stCOMP.sB;

#if __MISP100_AWB_DBG__ == 1
		UARTprintf("\r\n[PreWB] Find R/B Correction END!!!\r\n");
		UARTprintf("=> sR %04d sB %04d\r\n", sR,sB);
		UARTprintf("%04X %04X %07d %07d %07d %07d\r\n", U,V,R,Gr,B,Gb);
#endif
		OSD_StrDispAttb(0,0,ON, OSD_COLOR, " <R/B CORRECTION-END> ");
		OSD_StrDispAttb(1,0,ON, OSD_COLOR, " GET> sR:%04d sB:%04d ", sR,sB);
		OSD_StrDispAttb(2,0,ON, OSD_COLOR, " GET> nU:%04X nV:%04X ",  U, V);
		OSD_StrDispAttb(3,0,ON, OSD_COLOR, "-------------------------------");
		OSD_StrDispAttb(4,0,ON, OSD_COLOR, "(  R  ) ( G_r ) (  B  ) ( G_b )");
		OSD_StrDispAttb(5,0,ON, OSD_COLOR, "%07d %07d %07d %07d ", R,Gr,B,Gb);
	}

	else if (nIDX==3) {
		R = (DWORD)(stWBVIEW.stDATA.R[0]); G = (DWORD)(stWBVIEW.stDATA.G[0]);
		B = (DWORD)(stWBVIEW.stDATA.B[0]); W = (DWORD)(stWBVIEW.stDATA.W[0]);
		U = (WORD)ROUND(stWBVIEW.stDATA.U*256);
		V = (WORD)ROUND(stWBVIEW.stDATA.V*256);

#if __MISP100_AWB_DBG__ == 1
		UARTprintf("\r\n[PreWB] Find R/B White-Data END!!!\r\n");
		UARTprintf("%04X %04X %07d %07d %07d %07d\r\n", U,V,R,G,B,W);
#endif
		OSD_StrDispAttb( 7,0,ON, OSD_COLOR, " GET> wU:%04X wV:%04X ",  U, V);
		OSD_StrDispAttb( 8,0,ON, OSD_COLOR, "-------------------------------");
		OSD_StrDispAttb( 9,0,ON, OSD_COLOR, "(  R  ) (  G  ) (  B  ) (  W  )");
		OSD_StrDispAttb(10,0,ON, OSD_COLOR, "%07d %07d %07d %07d ", R,G,B,W);

		R = (DWORD)(stWBVIEW.stDATA.R[1]); G = (DWORD)(stWBVIEW.stDATA.G[1]);
		B = (DWORD)(stWBVIEW.stDATA.B[1]); W = (DWORD)(stWBVIEW.stDATA.W[1]);

#if __MISP100_AWB_DBG__ == 1
		UARTprintf("[CTL-VAL] %07d %07d %07d %07d\r\n", R,G,B,W);
#endif
		OSD_StrDispAttb(11,0,ON, OSD_COLOR, "%07d %07d %07d %07d ", R,G,B,W);
	}

	else if (nIDX==4) {
#if __MISP100_AWB_DBG__ == 1
		UARTprintf("\r\n[PreWB] Find R/B Correction DONE!!!\r\n");
#endif
		OSD_StrDispAttb(0,0,ON, OSD_COLOR, " <R/B CORRECTION-DONE>");
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBRAW_DisplayTIME(BYTE nIDX)
{
	WORD i, U, V, sR, sB;	DWORD R, G, B, W, Gr, Gb;

	if (fSysRAWModeTIME==OFF && nIDX!=4) return;

	else if (nIDX==0) {
#if __MISP100_AWB_DBG__ == 1
		UARTprintf("\r\n[TIME] Find R/B Time-Point MODE!!!\r\n");
#endif
		OSD_StrDispAttb(0,0,ON, OSD_COLOR, " <R/B TIME-POINT-MODE>");
		for (i=0; i<11; i++) OSD_StrDispAttb(i+1,0,ON, OSD_COLOR, "                                ");
		OSD_StrDispOnOff(ON);
	}

	else if (nIDX==1) {
#if __MISP100_AWB_DBG__ == 1
		UARTprintf("\r\n[TIME] Find R/B Time-Point RUN!!!\r\n");
#endif
		OSD_StrDispAttb(0,0,ON, OSD_COLOR, " <R/B TIME-POINT-RUN> ");
	}

	else if (nIDX==2) {
		R = (DWORD)(stWBVIEW.stPXL[8].R); Gr = (DWORD)(stWBVIEW.stPXL[8].Gr);
		B = (DWORD)(stWBVIEW.stPXL[8].B); Gb = (DWORD)(stWBVIEW.stPXL[8].Gb);
		U = (WORD)ROUND(stWBVIEW.stPXL[8].U*256); sR = (WORD)stWBVIEW.stCOMP.sR;
		V = (WORD)ROUND(stWBVIEW.stPXL[8].V*256); sB = (WORD)stWBVIEW.stCOMP.sB;

#if __MISP100_AWB_DBG__ == 1
		UARTprintf("\r\n[TIME] Find R/B Time-Point END!!!\r\n");
		UARTprintf("=> sR %04d sB %04d\r\n", sR,sB);
		UARTprintf("%04X %04X %07d %07d %07d %07d\r\n", U,V,R,Gr,B,Gb);
#endif
		OSD_StrDispAttb(0,0,ON, OSD_COLOR, " <R/B TIME-POINT-END> ");
		OSD_StrDispAttb(1,0,ON, OSD_COLOR, " GET> sR:%04d sB:%04d ", sR,sB);
		OSD_StrDispAttb(2,0,ON, OSD_COLOR, " GET> nU:%04X nV:%04X ",  U, V);
		OSD_StrDispAttb(3,0,ON, OSD_COLOR, "-------------------------------");
		OSD_StrDispAttb(4,0,ON, OSD_COLOR, "(  R  ) ( G_r ) (  B  ) ( G_b )");
		OSD_StrDispAttb(5,0,ON, OSD_COLOR, "%07d %07d %07d %07d ", R,Gr,B,Gb);
	}

	else if (nIDX==3) {
		R = (DWORD)(stWBVIEW.stDATA.R[0]); G = (DWORD)(stWBVIEW.stDATA.G[0]);
		B = (DWORD)(stWBVIEW.stDATA.B[0]); W = (DWORD)(stWBVIEW.stDATA.W[0]);
		U = (WORD)ROUND(stWBVIEW.stDATA.U*256);
		V = (WORD)ROUND(stWBVIEW.stDATA.V*256);

#if __MISP100_AWB_DBG__ == 1
		UARTprintf("\r\n[TIME] Find R/B White-Data END!!!\r\n");
		UARTprintf("%04X %04X %07d %07d %07d %07d\r\n", U,V,R,G,B,W);
#endif
		OSD_StrDispAttb( 7,0,ON, OSD_COLOR, " GET> wU:%04X wV:%04X ",  U, V);
		OSD_StrDispAttb( 8,0,ON, OSD_COLOR, "-------------------------------");
		OSD_StrDispAttb( 9,0,ON, OSD_COLOR, "(  R  ) (  G  ) (  B  ) (  W  )");
		OSD_StrDispAttb(10,0,ON, OSD_COLOR, "%07d %07d %07d %07d ", R,G,B,W);

		R = (DWORD)(stWBVIEW.stDATA.R[1]); G = (DWORD)(stWBVIEW.stDATA.G[1]);
		B = (DWORD)(stWBVIEW.stDATA.B[1]); W = (DWORD)(stWBVIEW.stDATA.W[1]);

#if __MISP100_AWB_DBG__ == 1
		UARTprintf("[CTL-VAL] %07d %07d %07d %07d\r\n", R,G,B,W);
#endif
		OSD_StrDispAttb(11,0,ON, OSD_COLOR, "%07d %07d %07d %07d ", R,G,B,W);
	}

	else if (nIDX==4) {
#if __MISP100_AWB_DBG__ == 1
		UARTprintf("\r\n[TIME] Find R/B Time-Point DONE!!!\r\n");
#endif
		OSD_StrDispAttb(0,0,ON, OSD_COLOR, " <R/B TIME-POINT-DONE>");
	}
}

//--------------------------------------------------------------------------------------------------------------------------
#if defined(__USE_MDIN_i500__) 				// MDIN-i500
static void WBRAW_GetBayerSUMByte(BYTE nID, PBYTE pBuff, WORD bytes)
{
	WORD i;	PLONG pSRC = stWBVIEW.raw[stWBVIEW.nPXL%2];

	if (stWBVIEW.nPXL==0) memset((PBYTE)stWBVIEW.raw, 0, 16);
	for (i=0; i<(bytes/2); i++) pSRC[i%2] += ((PWORD)pBuff)[i];
	stWBVIEW.nPXL++;
}
#else 										// MDIN-i51X (i510, i540, i550)
static void WBRAW_GetBayerSUMWord(BYTE nID, PWORD pBuff, WORD words, BOOL mode)
{
	WORD i;	PLONG pSRC = stWBVIEW.raw[stWBVIEW.nPXL%2];

#if defined(__USE_MDIN_i5XX_REV__)
	if ((stWBVIEW.nPXL==2) || (stWBVIEW.nPXL==3)) pSRC[mode] = 0; //mimitest

	for (i=0; i<words; i++) pSRC[mode] += pBuff[i];

	//UARTprintf("pSRC[%d] = %d\r\n",mode,pSRC[mode]);
	//UARTprintf("nPXL=%d / mode:%d =  %d\r\n",stWBVIEW.nPXL,mode, (2*stWBVIEW.nPXL%2)+mode);
	//UARTprintf("stWBVIEW.raw=%d %d %d %d\r\n",stWBVIEW.raw[0][0],stWBVIEW.raw[0][1],stWBVIEW.raw[1][0],stWBVIEW.raw[1][1]);
	//UARTprintf("\r\n");
	
#else
	if (stWBVIEW.nPXL==0) memset((PBYTE)stWBVIEW.raw, 0, 16); //sSRC , raw = sum of pBuff
	for (i=0; i<words; i++) pSRC[i%2] += pBuff[i];
#endif

	stWBVIEW.nPXL++;
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
static void WBRAW_GetSlopeRB(LONG R, LONG Gr, LONG B, LONG Gb)
{
	WORD sR, sG, sB; FLOAT fR, fG, fB, fY;

	if (fSysRAWModeCOMP==OFF && fSysRAWModeTIME==OFF) return;

	MISP_RegRead(MISP_LOCAL_ID, 0x602, &sR);	sR &= 0xfff;
	MISP_RegRead(MISP_LOCAL_ID, 0x604, &sG);	sG &= 0xfff;
	MISP_RegRead(MISP_LOCAL_ID, 0x606, &sB);	sB &= 0xfff;

	stWBVIEW.stPXL[8].R = R; stWBVIEW.stPXL[8].Gr = Gr;
	stWBVIEW.stPXL[8].B = B; stWBVIEW.stPXL[8].Gb = Gb;

	fR = ((FLOAT)R/(stWBVIEW.nPXL*16*1))*((FLOAT)sR/256.0f);
	fB = ((FLOAT)B/(stWBVIEW.nPXL*16*1))*((FLOAT)sB/256.0f);
	fG = ((FLOAT)Gr+Gb)/(stWBVIEW.nPXL*16*2)*((FLOAT)sG/256.0f);

	fY = (FLOAT)(128.0f)/ ( 0.299f*fR+0.587f*fG+0.114f*fB);
	stWBVIEW.stPXL[8].U = (-0.299f*fR-0.587f*fG+0.886f*fB)*fY;
	stWBVIEW.stPXL[8].V = ( 0.701f*fR-0.587f*fG-0.114f*fB)*fY;

	stWBVIEW.stCOMP.sR = (SHORT)((FLOAT)Gr*256.0f/R+0.5);
	stWBVIEW.stCOMP.sB = (SHORT)((FLOAT)Gb*256.0f/B+0.5);

	WBRAW_DisplayCOMP(2);	WBRAW_DisplayTIME(2);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBRAW_GetWhiteRB(LONG R, LONG G, LONG B, LONG W)
{
	FLOAT fR, fG, fB, fY;

	if (fSysRAWModeCOMP==OFF && fSysRAWModeTIME==OFF) return;

	WB_GetAWBGryVALUE(&R, &G, &B, &W);

	stWBVIEW.stDATA.R[0] = R; stWBVIEW.stDATA.G[0] = G;
	stWBVIEW.stDATA.B[0] = B; stWBVIEW.stDATA.W[0] = W;

	fR = (FLOAT)R/W; fB = (FLOAT)B/W; fG = (FLOAT)G/W;

	fY = (FLOAT)(128.0f)/(0.299f*fR+0.587f*fG+0.114f*fB);
	stWBVIEW.stDATA.U = (-0.299f*fR-0.587f*fG+0.886f*fB)*fY;
	stWBVIEW.stDATA.V = ( 0.701f*fR-0.587f*fG-0.114f*fB)*fY;

	WB_GetAWBCtlVALUE(&R, &G, &B, &W);

	stWBVIEW.stDATA.R[1] = R; stWBVIEW.stDATA.G[1] = G;
	stWBVIEW.stDATA.B[1] = B; stWBVIEW.stDATA.W[1] = W;

	WBRAW_DisplayCOMP(3);	WBRAW_DisplayTIME(3);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBRAW_GetAxisYUV(void)
{
	BYTE i; FLOAT R, G, B, Y; SHORT sR, sB;

	sR = (SHORT)((FLOAT)stWBVIEW.stPXL[5].Gr*256.0f/stWBVIEW.stPXL[5].R+0.5);
	sB = (SHORT)((FLOAT)stWBVIEW.stPXL[5].Gb*256.0f/stWBVIEW.stPXL[5].B+0.5);

	for (i=0; i<8; i++) {
		R = ((FLOAT)stWBVIEW.stPXL[i].R/(stWBVIEW.nPXL*16*1))*((FLOAT)sR/256.0f);
		B = ((FLOAT)stWBVIEW.stPXL[i].B/(stWBVIEW.nPXL*16*1))*((FLOAT)sB/256.0f);
		G = ((FLOAT)stWBVIEW.stPXL[i].Gr+stWBVIEW.stPXL[i].Gb)/(stWBVIEW.nPXL*16*2);

		Y = (FLOAT)(128.0f) / ( 0.299f*R+0.587f*G+0.114f*B);
		stWBVIEW.stPXL[i].U = (-0.299f*R-0.587f*G+0.886f*B)*Y;
		stWBVIEW.stPXL[i].V = ( 0.701f*R-0.587f*G-0.114f*B)*Y;
	}

	stWBVIEW.stCOMP.sR = sR;	stWBVIEW.stCOMP.sB = sB;
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBRAW_GetPolygon(LONG R, LONG Gr, LONG B, LONG Gb)
{
	if (fSysRAWModePOLY==OFF) return;

	stWBVIEW.stPXL[stWBVIEW.nID].R = R; stWBVIEW.stPXL[stWBVIEW.nID].Gr = Gr;
	stWBVIEW.stPXL[stWBVIEW.nID].B = B; stWBVIEW.stPXL[stWBVIEW.nID].Gb = Gb;

	WBRAW_GetAxisYUV();	WBRAW_DisplayPOLY(2);

	stWBVIEW.nID++;	if (stWBVIEW.nID<8) return;

	stWBVIEW.nID = 0;	fSysRAWModeSAVE = ON;	WBRAW_DisplayPOLY(3);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBRAW_GetBayerPXL(void)
{
	LONG R, Gr, B, Gb;

	MISP_RegRead(MISP_LOCAL_ID, 0x600, &stWBVIEW.nODR);

	switch (stWBVIEW.nODR&0x03) {
		case  0: R = stWBVIEW.raw[1][1]; Gr = stWBVIEW.raw[1][0];
				 B = stWBVIEW.raw[0][0]; Gb = stWBVIEW.raw[0][1]; break;

		case  1: R = stWBVIEW.raw[1][0]; Gr = stWBVIEW.raw[1][1];
				 B = stWBVIEW.raw[0][1]; Gb = stWBVIEW.raw[0][0]; break;

		case  2: R = stWBVIEW.raw[0][0]; Gr = stWBVIEW.raw[0][1];
				 B = stWBVIEW.raw[1][1]; Gb = stWBVIEW.raw[1][0]; break;

		default: R = stWBVIEW.raw[0][1]; Gr = stWBVIEW.raw[0][0];
				 B = stWBVIEW.raw[1][0]; Gb = stWBVIEW.raw[1][1]; break;
	}
	//UARTprintf("R:%d / Gr:%d / B:%d / Gb:%d\r\n", R, Gr, B, Gb);
	WBRAW_GetSlopeRB(R, Gr, B, Gb);		WBRAW_GetPolygon(R, Gr, B, Gb);
	WBRAW_GetWhiteRB(R, Gr, B, Gb);
}

//--------------------------------------------------------------------------------------------------------------------------
void WD_ProcessHandler(PWB_CTRL_PARAM pCTRL)
{
	WORD cntW, endX, endY = stWBVIEW.stWIND.eY+0;

	if ((stWBVIEW.ctrl&WBVIEW_PROCESS_RUN)==0) return;

	if (stWBVIEW.ctrl&WBVIEW_TRIGGER_ISP) {
#if defined(__USE_MDIN_i500__) 					// MDIN-i500
		MISP_MultiRead(MISP_LOCAL_ID, 0x700, stWBVIEW.buffByte, 128); // odm_raw_data // different 500 and 510
		WBRAW_GetBayerSUMByte(MISP_LOCAL_ID, stWBVIEW.buffByte, 128);
#else 											// MDIN-i51X (i510, i540, i550)
		int i;

#if defined(__USE_MDIN_i5XX_REV__)
		if((stWBVIEW.ctrl&WBVIEW_TRIGGER_ODDRAW)==0) {
			MISP_RegWrite(MISP_LOCAL_ID, 0x6E3, 1); // indirect method -- read enable
			//UARTprintf("e:");
			for(i = 0;i < 32;i++) {
				MISP_RegRead(MISP_LOCAL_ID, 0x6E4, &stWBVIEW.buffWord[i]);
				//UARTprintf("%d ",stWBVIEW.buffWord[i]);
			}
			//UARTprintf("\r\n");
			MISP_RegWrite(MISP_LOCAL_ID, 0x6E3, 0); // indirect method -- read disable
			WBRAW_GetBayerSUMWord(MISP_LOCAL_ID, stWBVIEW.buffWord, 32, 0); // 64->32
		} else {
			MISP_RegWrite(MISP_LOCAL_ID, 0x6E3, 1); // indirect method -- read enable
			//UARTprintf("o:");
			for(i = 0;i < 32;i++) {
				MISP_RegRead(MISP_LOCAL_ID, 0x6E4, &stWBVIEW.buffWord_odd[i]);
				//UARTprintf("%d ",stWBVIEW.buffWord_odd[i]);
			}
			//UARTprintf("\r\n");
			MISP_RegWrite(MISP_LOCAL_ID, 0x6E3, 0); // indirect method -- read disable
			WBRAW_GetBayerSUMWord(MISP_LOCAL_ID, stWBVIEW.buffWord_odd, 32, 1); // 64->32
		}
#else
		MISP_RegWrite(MISP_LOCAL_ID, 0x6E3, 1); // indirect method -- read enable
		for(i = 0;i < 64;i++) {
			MISP_RegRead(MISP_LOCAL_ID, 0x6E4, &stWBVIEW.buffWord[i]);
			//UARTprintf("%d ",stWBVIEW.buffWord[i]);
		}
		//UARTprintf("\r\n");
		MISP_RegWrite(MISP_LOCAL_ID, 0x6E3, 0); // indirect method -- read disable
		WBRAW_GetBayerSUMWord(MISP_LOCAL_ID, stWBVIEW.buffWord, 64, 0);
#endif

#endif
	}
	else stWBVIEW.ctrl |= WBVIEW_TRIGGER_ISP;

	if (stWBVIEW.stAREA.sY==endY) stWBVIEW.ctrl &= (~WBVIEW_PROCESS_RUN);
	if (stWBVIEW.stAREA.sY==endY) SetMsgCommandLongExec(OFF);

#if defined(__USE_MDIN_i5XX_REV__)
	if((stWBVIEW.ctrl&WBVIEW_TRIGGER_ODDRAW)==0) {
		if (stWBVIEW.stAREA.sY==endY)	SetWBRAWViewTrigger(1);
	} else {
		if (stWBVIEW.stAREA.sY==endY) WBRAW_GetBayerPXL();
	}
#else
	if (stWBVIEW.stAREA.sY==endY) WBRAW_GetBayerPXL();
#endif

	if ((stWBVIEW.ctrl&WBVIEW_TRIGGER_ISP)==0) return;
	if ((stWBVIEW.ctrl&WBVIEW_PROCESS_RUN)==0) return;

	endX = stWBVIEW.stAREA.eX-stWBVIEW.stWIND.sX;
	cntW = stWBVIEW.stWIND.eX-stWBVIEW.stWIND.sX;

	stWBVIEW.stAREA.sX = stWBVIEW.stWIND.sX+(endX%cntW);
	stWBVIEW.stAREA.sY = stWBVIEW.stAREA.sY+(endX/cntW);
	stWBVIEW.stAREA.eX = stWBVIEW.stAREA.sX+(64);
	stWBVIEW.stAREA.eY = stWBVIEW.stAREA.sY+( 1);
	//UARTprintf("1:%d/%d/%d/%d\r\n",stWBVIEW.stAREA.sX,stWBVIEW.stAREA.sY,stWBVIEW.stAREA.eX,stWBVIEW.stAREA.eY);
	MISP_MultiWrite(MISP_LOCAL_ID, 0x66C, (PBYTE)&stWBVIEW.stAREA, 8); // dm_raw_area_sx
}

//--------------------------------------------------------------------------------------------------------------------------
void WD_TriggerHandler(PWB_CTRL_PARAM pCTRL)
{
	PWB_TIME_PARAM pTIME = &stWBVIEW.stTIME;

	if ((stWBVIEW.ctrl&WBVIEW_TIMECNT_RUN)==0) return;

//	pTIME->tACT++;	if (pTIME->tACT<GET_FPS*60*3) return;	// 3min
	pTIME->tACT++;	if (pTIME->tACT<GET_FPS* 1*5) return;	// 5sec

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
//	EEPROM_RegWrite(EEPROM_TIME_UV+0x000+pTIME->nCNT*2, (WORD)ROUND(stWBVIEW.stPXL[8].U*256));
//	EEPROM_RegWrite(EEPROM_TIME_UV+0x400+pTIME->nCNT*2, (WORD)ROUND(stWBVIEW.stPXL[8].V*256));
#endif

	pTIME->tACT = 0; if (pTIME->nCNT<511) pTIME->nCNT++;

	SetWBRAWViewTrigger(0);
}

//--------------------------------------------------------------------------------------------------------------------------
void SetWBRAWViewTrigger(BOOL mode)
{
	if ((stWBVIEW.ctrl&WBVIEW_DISPLAY_RUN)==0) return;

	WBRAW_DisplayCOMP(1);	WBRAW_DisplayTIME(1);	WBRAW_DisplayPOLY(1);

	MISP_MultiRead(MISP_LOCAL_ID, 0x75B, (PBYTE)&stWBVIEW.stWIND, 8); // odm window
	memcpy((PBYTE)&stSPOT, &stWBVIEW.stWIND, sizeof(WB_AREA_PARAM));

	MISP_MultiRead(MISP_LOCAL_ID, 0x64D, (PBYTE)&stWBVIEW.offX, 4); //odm_insize_h odm_insize_v
	stWBVIEW.offX = (stWBVIEW.offX-WBVIEW_HACTIVE_LEN)/4*2;
	stWBVIEW.offY = (stWBVIEW.offY-WBVIEW_VACTIVE_LEN)/4*2;

	stWBVIEW.stWIND.sX += (stWBVIEW.offX>0)? stWBVIEW.offX : 0;
	stWBVIEW.stWIND.eX += (stWBVIEW.offX>0)? stWBVIEW.offX : 0;
	stWBVIEW.stWIND.sY += (stWBVIEW.offY>0)? stWBVIEW.offY : 0;
	stWBVIEW.stWIND.eY += (stWBVIEW.offY>0)? stWBVIEW.offY : 0;

	SetMsgCommandLongExec(ON);	stWBVIEW.nPXL = 0;

#if defined(__USE_MDIN_i5XX_REV__)
	if(mode)
		stWBVIEW.ctrl |= WBVIEW_TRIGGER_ODDRAW;
	else
		stWBVIEW.ctrl &= (~WBVIEW_TRIGGER_ODDRAW);
#else
	stWBVIEW.ctrl &= (~WBVIEW_TRIGGER_ODDRAW);
#endif
	stWBVIEW.ctrl &= (~WBVIEW_TRIGGER_ISP);
	stWBVIEW.ctrl |= ( WBVIEW_PROCESS_RUN);

	if (fSysRAWModeTIME) stWBVIEW.ctrl |= ( WBVIEW_TIMECNT_RUN);

	if((stWBVIEW.ctrl&WBVIEW_TRIGGER_ODDRAW)==0) {
		stWBVIEW.stAREA.sX = stWBVIEW.stWIND.sX- 0;
		stWBVIEW.stAREA.eX = stWBVIEW.stAREA.sX+64;
		stWBVIEW.stAREA.sY = stWBVIEW.stWIND.sY- 0;
		stWBVIEW.stAREA.eY = stWBVIEW.stAREA.sY+ 1;
		//UARTprintf("2e:%d/%d/%d/%d\r\n",stWBVIEW.stAREA.sX,stWBVIEW.stAREA.sY,stWBVIEW.stAREA.eX,stWBVIEW.stAREA.eY);
		MISP_MultiWrite(MISP_LOCAL_ID, 0x66C, (PBYTE)&stWBVIEW.stAREA, 8); // odm_raw_area_sx
	} else {
		stWBVIEW.stAREA.sX = stWBVIEW.stWIND.sX- 0+1;
		stWBVIEW.stAREA.eX = stWBVIEW.stAREA.sX+64;
		stWBVIEW.stAREA.sY = stWBVIEW.stWIND.sY- 0;
		stWBVIEW.stAREA.eY = stWBVIEW.stAREA.sY+ 1;
		//UARTprintf("2o:%d/%d/%d/%d\r\n",stWBVIEW.stAREA.sX,stWBVIEW.stAREA.sY,stWBVIEW.stAREA.eX,stWBVIEW.stAREA.eY);
		MISP_MultiWrite(MISP_LOCAL_ID, 0x66C, (PBYTE)&stWBVIEW.stAREA, 8); // odm_raw_area_sx
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBRAW_DisplayCLEAR(void)
{
	BYTE i;
	for (i=0; i<19; i++) OSD_StrDispAttb(i,0,ON, OSD_COLOR, "%36s", "");
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBRAW_DisplayVIEW(void)
{
	memcpy(&stWBVIEW.stWIND, &stSPOT, sizeof(WB_AREA_PARAM));

	if ((stWBVIEW.ctrl&WBVIEW_DISPLAY_RUN)==0)
		 MISP_RegField(MISP_LOCAL_ID, 0x752, 2, 1, 0);
	else MISP_RegField(MISP_LOCAL_ID, 0x752, 2, 1, 1);

	if ((stWBVIEW.ctrl&WBVIEW_DISPLAY_RUN)==0) return;
	MISP_MultiWrite(MISP_LOCAL_ID, 0x75B, (PBYTE)&stWBVIEW.stWIND, 8);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBRAW_SetLinePOLY(PWORD pBuff, FLOAT slop, FLOAT jeol)
{
	WORD   j = (WORD)(ROUND((jeol>=0)? jeol*256 : 524288+jeol*256)/65536);
	pBuff[1] = (WORD)(ROUND((jeol>=0)? jeol*256 : 524288+jeol*256)%65536);
	pBuff[0] = (WORD)(ROUND((slop>=0)? slop*256 : 4096+slop*256))|(j<<12);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBRAW_CutLinePOLY(BYTE nID)
{
	CHAR i;
	CHAR ulP = stWBTUNE[nID].P[0], lrP = stWBTUNE[nID].P[1];
	CHAR ulL = stWBTUNE[nID].L[0], lrL = stWBTUNE[nID].L[1];

	for (i=0; i<16/2; i++) {
		if (i<ulP-1) stWBVIEW.nLINE[ 0+i*2+0] = stWBVIEW.nLINE[ 0+(ulP-1)*2+0];
		if (i<ulP-1) stWBVIEW.nLINE[ 0+i*2+1] = stWBVIEW.nLINE[ 0+(ulP-1)*2+1];
		if (i>lrP-1) stWBVIEW.nLINE[ 0+i*2+0] = stWBVIEW.nLINE[ 0+(lrP-1)*2+0];
		if (i>lrP-1) stWBVIEW.nLINE[ 0+i*2+1] = stWBVIEW.nLINE[ 0+(lrP-1)*2+1];
	}

	for (i=0; i<14/2; i++) {
		if (i<ulL-1) stWBVIEW.nLINE[16+i*2+0] = stWBVIEW.nLINE[16+(ulL-1)*2+0];
		if (i<ulL-1) stWBVIEW.nLINE[16+i*2+1] = stWBVIEW.nLINE[16+(ulL-1)*2+1];
		if (i>lrL-1) stWBVIEW.nLINE[16+i*2+0] = stWBVIEW.nLINE[16+(lrL-1)*2+0];
		if (i>lrL-1) stWBVIEW.nLINE[16+i*2+1] = stWBVIEW.nLINE[16+(lrL-1)*2+1];

		if (i<ulL-1) stWBVIEW.nLINE[30+i*2+0] = stWBVIEW.nLINE[30+(ulL-1)*2+0];
		if (i<ulL-1) stWBVIEW.nLINE[30+i*2+1] = stWBVIEW.nLINE[30+(ulL-1)*2+1];
		if (i>lrL-1) stWBVIEW.nLINE[30+i*2+0] = stWBVIEW.nLINE[30+(lrL-1)*2+0];
		if (i>lrL-1) stWBVIEW.nLINE[30+i*2+1] = stWBVIEW.nLINE[30+(lrL-1)*2+1];
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBRAW_GetLinePOLY(BYTE nID)
{
	PWB_AREA_PIXEL pPXL = (PWB_AREA_PIXEL)stWBVIEW.stPXL;
	PWB_TUNE_PARAM pSPC = (PWB_TUNE_PARAM)&stWBTUNE[nID];
	FLOAT sO[8], jO[8], hU[8], hV[8], lU[8], lV[8];
	FLOAT sH, jH, sL, jL;	CHAR i;

	for (i=0; i<8; i++) {
		CHAR l = (i==5)? i-1 : i; CHAR h = (i==5)? i+1 : 5;
//		CHAR l = (((i-1)<0)? 0 : i-1), h = (((i+1)>7)? 7 : i+1);
		sO[i] = (pPXL[l].U-pPXL[h].U)/(pPXL[h].V-pPXL[l].V);
		jO[i] = (pPXL[i].V-pPXL[i].U*sO[i]);				

		WBRAW_SetLinePOLY(&stWBVIEW.nLINE[0]+i*2, sO[i], jO[i]);

		hU[i] = pPXL[i].U+(FLOAT)pSPC->U[i]*1.00f/sqrt(1+sO[i]*sO[i]);
		hV[i] = pPXL[i].V+(FLOAT)pSPC->U[i]*sO[i]/sqrt(1+sO[i]*sO[i]);

		lU[i] = pPXL[i].U+(FLOAT)pSPC->D[i]*1.00f/sqrt(1+sO[i]*sO[i]);
		lV[i] = pPXL[i].V+(FLOAT)pSPC->D[i]*sO[i]/sqrt(1+sO[i]*sO[i]);
	}

	for (i=0; i<7; i++) {
		sH = ((FLOAT)hV[i+1]-hV[i])/(hU[i+1]-hU[i]);
		jH = ((FLOAT)hV[i]-hU[i]*sH);				

		sL = ((FLOAT)lV[i+1]-lV[i])/(lU[i+1]-lU[i]);
		jL = ((FLOAT)lV[i]-lU[i]*sL);				

		WBRAW_SetLinePOLY(&stWBVIEW.nLINE[16]+i*2, sH, jH);
		WBRAW_SetLinePOLY(&stWBVIEW.nLINE[30]+i*2, sL, jL);
	}

	WBRAW_CutLinePOLY(nID);		// cut left/right boundary
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBRAW_WriteToCOMP(void)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	EEPROM_RegWrite(EEPROM_COMP_ID,  EEPROM_COMP_DONE);
	EEPROM_MultiWrite(EEPROM_COMP_NDR, (PBYTE)&stWBVIEW.stCOMP, sizeof(WB_COMP_PARAM));
	EEPROM_MultiWrite(EEPROM_COMP_WDR, (PBYTE)&stWBVIEW.stCOMP, sizeof(WB_COMP_PARAM));
	EEPROM_MultiWrite(EEPROM_COMP_HTM, (PBYTE)&stWBVIEW.stCOMP, sizeof(WB_COMP_PARAM));
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBRAW_WriteToPOLY(void)
{
	WORD i;

	if (fSysRAWModeSAVE==OFF)  return;
	else		fSysRAWModeSAVE = OFF;

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	EEPROM_RegWrite(EEPROM_POLY_ID,  EEPROM_POLY_DONE);
#endif

	WBRAW_GetLinePOLY(0);
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	EEPROM_MultiWrite(EEPROM_POLY_ATW, (PBYTE)&stWBVIEW.nLINE, sizeof(WB_POLY_PARAM));
#endif

	WBRAW_GetLinePOLY(1);
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	EEPROM_MultiWrite(EEPROM_POLY_OUT, (PBYTE)&stWBVIEW.nLINE, sizeof(WB_POLY_PARAM));
#endif

	WBRAW_GetLinePOLY(2);
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	EEPROM_MultiWrite(EEPROM_POLY_IN,  (PBYTE)&stWBVIEW.nLINE, sizeof(WB_POLY_PARAM));
#endif

	for (i=0; i<8; i++) {
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
		EEPROM_RegWrite(EEPROM_POLY_UV+i*2+ 0,  (WORD)ROUND(stWBVIEW.stPXL[i].U*256));
		EEPROM_RegWrite(EEPROM_POLY_UV+i*2+16,  (WORD)ROUND(stWBVIEW.stPXL[i].V*256));

		EEPROM_RegWrite(EEPROM_POLY_RAW+i*4+ 0, (WORD)HIWORD(stWBVIEW.stPXL[i].R));
		EEPROM_RegWrite(EEPROM_POLY_RAW+i*4+ 2, (WORD)LOWORD(stWBVIEW.stPXL[i].R));
		EEPROM_RegWrite(EEPROM_POLY_RAW+i*4+32, (WORD)HIWORD(stWBVIEW.stPXL[i].Gr));
		EEPROM_RegWrite(EEPROM_POLY_RAW+i*4+34, (WORD)LOWORD(stWBVIEW.stPXL[i].Gr));
		EEPROM_RegWrite(EEPROM_POLY_RAW+i*4+64, (WORD)HIWORD(stWBVIEW.stPXL[i].B));
		EEPROM_RegWrite(EEPROM_POLY_RAW+i*4+66, (WORD)LOWORD(stWBVIEW.stPXL[i].B));
		EEPROM_RegWrite(EEPROM_POLY_RAW+i*4+96, (WORD)HIWORD(stWBVIEW.stPXL[i].Gb));
		EEPROM_RegWrite(EEPROM_POLY_RAW+i*4+98, (WORD)LOWORD(stWBVIEW.stPXL[i].Gb));
#endif
	}

#if __MISP100_AWB_DBG__ == 1
	UARTprintf("[POLY] The Calibration data writes to EEPROM!!!\r\n");
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBRAW_ProcessBGN(BOOL nID)
{
	memset((PBYTE)&stWBVIEW, 0, sizeof(WB_VIEW_PARAM));

	if (fSysRAWModeCOMP || fSysRAWModeTIME || fSysRAWModePOLY)
		 stWBVIEW.ctrl |= ( WBVIEW_DISPLAY_RUN);
	else stWBVIEW.ctrl &= (~WBVIEW_DISPLAY_RUN);

	WBRAW_DisplayCLEAR();	WBRAW_DisplayVIEW();
	WBRAW_DisplayCOMP(0);	WBRAW_DisplayTIME(0);
	WBRAW_DisplayPOLY(0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void WBRAW_ProcessEND(BOOL nID)
{
	/*WORD i;

	if (nID==0) {
		for (i=0; i<8; i++) {
			stWBCTRL.stWBCOLOR.U[i] = (WORD)ROUND(stWBVIEW.stPXL[i].U*256);
			stWBCTRL.stWBCOLOR.V[i] = (WORD)ROUND(stWBVIEW.stPXL[i].V*256);
		}
	}*/

	if (nID==0) WBRAW_WriteToPOLY();
	if (nID==0) WBRAW_WriteToCOMP();

	stWBVIEW.ctrl &= (~WBVIEW_TIMECNT_RUN);
	MISP_RegField(MISP_LOCAL_ID, 0x752, 2, 1, 0);

	if (nID==0)	WBRAW_DisplayPOLY(4);
	if (nID==1)	WBRAW_DisplayCOMP(4);
	if (nID==2)	WBRAW_DisplayTIME(4);

#if __MISP100_AWB_DBG__ == 1
	if (nID==0)	UARTprintf("\r\n[POLY] Exit UV for Polygon process!!!\r\n");
	if (nID==1)	UARTprintf("\r\n[PreWB] Exit R/B Correction process!!!\r\n");
	if (nID==2)	UARTprintf("\r\n[TIME] Exit R/B Time-Point process!!!\r\n");
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetWBRAWOnOffPOLY(void)
{
	if (fSysRAWModeCOMP || fSysRAWModeTIME) return;
	else					 fSysRAWModePOLY ^= ON;

	if (fSysRAWModePOLY==ON) WBRAW_ProcessBGN(0);
	else					 WBRAW_ProcessEND(0);
}

//--------------------------------------------------------------------------------------------------------------------------
void SetWBRAWOnOffCOMP(void)
{
	if (fSysRAWModePOLY || fSysRAWModeTIME) return;
	else					 fSysRAWModeCOMP ^= ON;

	if (fSysRAWModeCOMP==ON) WBRAW_ProcessBGN(1);
	else					 WBRAW_ProcessEND(1);
}

//--------------------------------------------------------------------------------------------------------------------------
void SetWBRAWOnOffTIME(void)
{
	if (fSysRAWModeCOMP || fSysRAWModePOLY) return;
	else					 fSysRAWModeTIME ^= ON;
//	fSysRAWModeTIME = ON;	fSysRAWModeCOMP = ON;

	if (fSysRAWModeTIME==ON) WBRAW_ProcessBGN(2);
	else					 WBRAW_ProcessEND(2);
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL GetWBRAWOnOffMODE(void)
{
	return (fSysRAWModeCOMP || fSysRAWModeTIME || fSysRAWModePOLY)? ON : OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL GetWBRAWOnOffTIME(void)
{
	return (stWBVIEW.ctrl&WBVIEW_TIMECNT_RUN)? ON : OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetWBRAWViewShiftL(void)
{
	if ((stWBVIEW.ctrl&WBVIEW_DISPLAY_RUN)==0) return;

	SetMsgCommandRepeatKey(MSG_SHIFT);
	stWBVIEW.step = (IsMsgPreExecCommand())? 2 : 8;

	stSPOT.sX = MAX(stSPOT.sX-stWBVIEW.step, 0);
	stSPOT.eX = MAX(stSPOT.eX-stWBVIEW.step, stSPOT.sX+64);

	WBRAW_DisplayVIEW();
}

//--------------------------------------------------------------------------------------------------------------------------
void SetWBRAWViewShiftR(void)
{
	if ((stWBVIEW.ctrl&WBVIEW_DISPLAY_RUN)==0) return;

	SetMsgCommandRepeatKey(MSG_SHIFT);
	stWBVIEW.step = (IsMsgPreExecCommand())? 2 : 8;

	stSPOT.eX = MIN(stSPOT.eX+stWBVIEW.step, WBVIEW_HACTIVE_LEN);
	stSPOT.sX = MIN(stSPOT.sX+stWBVIEW.step, stSPOT.eX-64);

	WBRAW_DisplayVIEW();
}

//--------------------------------------------------------------------------------------------------------------------------
void SetWBRAWViewShiftU(void)
{
	if ((stWBVIEW.ctrl&WBVIEW_DISPLAY_RUN)==0) return;

	SetMsgCommandRepeatKey(MSG_SHIFT);
	stWBVIEW.step = (IsMsgPreExecCommand())? 2 : 8;

	stSPOT.sY = MAX(stSPOT.sY-stWBVIEW.step, 0);
	stSPOT.eY = MAX(stSPOT.eY-stWBVIEW.step, stSPOT.sY+64);

	WBRAW_DisplayVIEW();
}

//--------------------------------------------------------------------------------------------------------------------------
void SetWBRAWViewShiftD(void)
{
	if ((stWBVIEW.ctrl&WBVIEW_DISPLAY_RUN)==0) return;

	SetMsgCommandRepeatKey(MSG_SHIFT);
	stWBVIEW.step = (IsMsgPreExecCommand())? 2 : 8;

	stSPOT.eY = MIN(stSPOT.eY+stWBVIEW.step, WBVIEW_VACTIVE_LEN);
	stSPOT.sY = MIN(stSPOT.sY+stWBVIEW.step, stSPOT.eY-64);

	WBRAW_DisplayVIEW();
}

/*  FILE_END _HERE */
