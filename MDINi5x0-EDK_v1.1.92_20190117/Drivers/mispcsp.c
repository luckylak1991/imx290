// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#include	"misp100.h"

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static CSP_CTRL_PARAM stCSP;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
// Device drive functions
static void Init_CSP_PARAM( PCSP_CTRL_PARAM pCTRL ) ;
static void Init_CSP_REGISTER( PCSP_CTRL_PARAM pCTRL ) ;

static void CSPCTRL_LSHandler( PCSP_CTRL_PARAM pCTRL ) ;
static void CSPCTRL_LDHandler( PCSP_CTRL_PARAM pCTRL ) ;
static void CSPCTRL_VDHandler( PCSP_CTRL_PARAM pCTRL ) ;

static WORD CSPCTRL_SLOPE( BYTE x0, BYTE x1, WORD y0, WORD y1 ) ;

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
static WORD CSPCTRL_SLOPE( BYTE x0, BYTE x1, WORD y0, WORD y1 )
{
	WORD slope ; // max 8191
	WORD wy0, wy1 ;
	WORD numerator, denominator, value ;

	wy0 = y0<<6 ;
	wy1 = y1<<6 ;
	
	if ( x1 > x0 )
	{
		if ( wy1 > wy0 )
		{
			numerator = wy1 - wy0 ; // y is increasing
			slope = 0 ;				
		}
		else
		{
			numerator = wy0 - wy1 ; // y is decreasing	
			slope = 8192 ;			
		}		
		denominator = x1 - x0 ;
		value = numerator/denominator ;
		
		// UARTprintf("[value:%d][numerator:%d][denominator:%d]\r\n",value,numerator,denominator);
		
		if ( slope == 0 )
		{
			if ( value > 4095 ) slope = 4095 ;
			else slope = value ;
		}
		else
		{
			if ( value > 4096 ) slope = 4096 ;
			else slope = slope - value ;			
		}
	}
	else
	{
		slope = 0 ; // divide by 0		
	}
	
	// UARTprintf("[slope:%d]\r\n",slope) ;
	
	return slope ;
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
static void Init_CSP_PARAM( PCSP_CTRL_PARAM pCTRL )
{
	SetCSP_LS_EN(1) ;
	SetCSP_LS_P0(0) ; // x0 value on the curve(0~255)
	SetCSP_LS_P1(4) ; // x1 value on the curve(0~255)
	
	SetCSP_LD_EN(0) ;
	SetCSP_LD_P0(32,0) ;
	SetCSP_LD_P1(48,256) ;
	SetCSP_LD_P2(208,256) ;
	SetCSP_LD_P3(224,0) ;
	
	SetCSP_VD_EN(0) ;
	SetCSP_VD_P0(32,256) ;
	SetCSP_VD_P1(64,240) ;
	SetCSP_VD_P2(128,176) ;
	SetCSP_VD_P3(198,48) ;
}
// ----------------------------------------------------------------------
static void Init_CSP_REGISTER( PCSP_CTRL_PARAM pCTRL )
{
	CSPCTRL_LSHandler(pCTRL);
	CSPCTRL_LDHandler(&stCSP);	
	CSPCTRL_VDHandler(&stCSP);	
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
static void CSPCTRL_LSHandler( PCSP_CTRL_PARAM pCTRL ) 
{

	if ((pCTRL->ctrl&CSPCTRL_LS_ENABLE)!=0) MISP_RegField(MISP_LOCAL_ID, 0x7D3, 2, 1, (pCTRL->stCSPLS.en)) ;
		
	if ((pCTRL->ctrl&CSPCTRL_LS_UPDATE)==0) return;
	pCTRL->ctrl &= (~CSPCTRL_LS_UPDATE);		
	
	pCTRL->stCSPLS.s0 = CSPCTRL_SLOPE(pCTRL->stCSPLS.x0,pCTRL->stCSPLS.x1,0,256) ; // LS y0 = 0 , y1 = 256 as constant
	
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x7DD, (pCTRL->stCSPLS.x1<<8)|pCTRL->stCSPLS.x0) ;
	MISP_RegWrite(MISP_LOCAL_ID, 0x7DE, pCTRL->stCSPLS.s0) ;	
}
// ----------------------------------------------------------------------
static void CSPCTRL_LDHandler( PCSP_CTRL_PARAM pCTRL ) 
{
	if ((pCTRL->ctrl&CSPCTRL_LD_ENABLE)!=0) MISP_RegField(MISP_LOCAL_ID, 0x7D3, 1, 1, (pCTRL->stCSPLD.en)) ;
	
	if ((pCTRL->ctrl&CSPCTRL_LD_UPDATE)==0) return;
	pCTRL->ctrl &= (~CSPCTRL_LD_UPDATE);		
	
	pCTRL->stCSPLD.s0 = CSPCTRL_SLOPE(pCTRL->stCSPLD.x0,pCTRL->stCSPLD.x1,pCTRL->stCSPLD.y0,pCTRL->stCSPLD.y1) ;
	pCTRL->stCSPLD.s1 = CSPCTRL_SLOPE(pCTRL->stCSPLD.x1,pCTRL->stCSPLD.x2,pCTRL->stCSPLD.y1,pCTRL->stCSPLD.y2) ;
	pCTRL->stCSPLD.s2 = CSPCTRL_SLOPE(pCTRL->stCSPLD.x2,pCTRL->stCSPLD.x3,pCTRL->stCSPLD.y2,pCTRL->stCSPLD.y3) ;
		
	MISP_RegWrite(MISP_LOCAL_ID, 0x7D4, (pCTRL->stCSPLD.x1<<8)|pCTRL->stCSPLD.x0) ;
	MISP_RegWrite(MISP_LOCAL_ID, 0x7D5, (pCTRL->stCSPLD.x3<<8)|pCTRL->stCSPLD.x2) ;
	MISP_RegField(MISP_LOCAL_ID, 0x7D6, 0, 9, pCTRL->stCSPLD.y0) ;	
	MISP_RegField(MISP_LOCAL_ID, 0x7D7, 0, 9, pCTRL->stCSPLD.y1) ;	
	MISP_RegField(MISP_LOCAL_ID, 0x7D8, 0, 9, pCTRL->stCSPLD.y2) ;	
	MISP_RegField(MISP_LOCAL_ID, 0x7D9, 0, 9, pCTRL->stCSPLD.y3) ;	
	MISP_RegField(MISP_LOCAL_ID, 0x7DA, 0, 13, pCTRL->stCSPLD.s0) ;	
	MISP_RegField(MISP_LOCAL_ID, 0x7DB, 0, 13, pCTRL->stCSPLD.s1) ;	
	MISP_RegField(MISP_LOCAL_ID, 0x7DC, 0, 13, pCTRL->stCSPLD.s2) ;	
}
// ----------------------------------------------------------------------
static void CSPCTRL_VDHandler( PCSP_CTRL_PARAM pCTRL ) 
{
	if ((pCTRL->ctrl&CSPCTRL_VD_ENABLE)!=0)	MISP_RegField(MISP_LOCAL_ID, 0x7D3, 3, 1, (pCTRL->stCSPVD.en)) ;
	
	if ((pCTRL->ctrl&CSPCTRL_VD_UPDATE)==0) return;
	pCTRL->ctrl &= (~CSPCTRL_VD_UPDATE);
	
	pCTRL->stCSPVD.s0 = CSPCTRL_SLOPE(pCTRL->stCSPVD.x0,pCTRL->stCSPVD.x1,pCTRL->stCSPVD.y0,pCTRL->stCSPVD.y1) ;
	pCTRL->stCSPVD.s1 = CSPCTRL_SLOPE(pCTRL->stCSPVD.x1,pCTRL->stCSPVD.x2,pCTRL->stCSPVD.y1,pCTRL->stCSPVD.y2) ;
	pCTRL->stCSPVD.s2 = CSPCTRL_SLOPE(pCTRL->stCSPVD.x2,pCTRL->stCSPVD.x3,pCTRL->stCSPVD.y2,pCTRL->stCSPVD.y3) ;
		
	MISP_RegWrite(MISP_LOCAL_ID, 0x7DF, (pCTRL->stCSPVD.x1<<8)|pCTRL->stCSPVD.x0) ;
	MISP_RegWrite(MISP_LOCAL_ID, 0x7E0, (pCTRL->stCSPVD.x3<<8)|pCTRL->stCSPVD.x2) ;
	MISP_RegField(MISP_LOCAL_ID, 0x7E1, 0, 9, pCTRL->stCSPVD.y0) ;	
	MISP_RegField(MISP_LOCAL_ID, 0x7E2, 0, 9, pCTRL->stCSPVD.y1) ;	
	MISP_RegField(MISP_LOCAL_ID, 0x7E3, 0, 9, pCTRL->stCSPVD.y2) ;	
	MISP_RegField(MISP_LOCAL_ID, 0x7E4, 0, 9, pCTRL->stCSPVD.y3) ;	
	MISP_RegField(MISP_LOCAL_ID, 0x7E5, 0, 13, pCTRL->stCSPVD.s0) ;	
	MISP_RegField(MISP_LOCAL_ID, 0x7E6, 0, 13, pCTRL->stCSPVD.s1) ;	
	MISP_RegField(MISP_LOCAL_ID, 0x7E7, 0, 13, pCTRL->stCSPVD.s2) ;		
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void Init_CSP(void) {

	memset(&stCSP, 0, sizeof(CSP_CTRL_PARAM));
	
	Init_CSP_PARAM(&stCSP);
	Init_CSP_REGISTER(&stCSP);
	
	stCSP.ctrl = 0 ;
}
// ----------------------------------------------------------------------
void CSP_ProcessHandler( void )
{
	if ((stCSP.ctrl&CSPCTRL_TRIGGER_ISP)==0) return;
	stCSP.ctrl &= (~CSPCTRL_TRIGGER_ISP);	
	
	CSPCTRL_LSHandler(&stCSP);	
	CSPCTRL_LDHandler(&stCSP);	
	CSPCTRL_VDHandler(&stCSP);	
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// API Get / Set functions
void SetCSP_LS_EN(BYTE en)
{
	stCSP.stCSPLS.en = en ;
	stCSP.ctrl |= (CSPCTRL_LS_ENABLE|CSPCTRL_TRIGGER_ISP);
}
// ----------------------------------------------------------------------
void SetCSP_LS_P0(BYTE x0)
{
	stCSP.stCSPLS.x0 = x0 ;
	
	stCSP.ctrl |= (CSPCTRL_LS_UPDATE|CSPCTRL_TRIGGER_ISP);		
}
// ----------------------------------------------------------------------
void SetCSP_LS_P1(BYTE x1)
{
	stCSP.stCSPLS.x1 = x1 ;
	
	stCSP.ctrl |= (CSPCTRL_LS_UPDATE|CSPCTRL_TRIGGER_ISP);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
void SetCSP_LD_EN(BYTE en)
{
	stCSP.stCSPLD.en = en ;
	stCSP.ctrl |= (CSPCTRL_LD_ENABLE|CSPCTRL_TRIGGER_ISP);
}
// ----------------------------------------------------------------------
void SetCSP_LD_P0(BYTE x0, WORD y0)
{
	stCSP.stCSPLD.x0 = x0 ;
	stCSP.stCSPLD.y0 = y0 ;
	
	stCSP.ctrl |= (CSPCTRL_LD_UPDATE|CSPCTRL_TRIGGER_ISP);		
}
// ----------------------------------------------------------------------
void SetCSP_LD_P1(BYTE x1, WORD y1)
{
	stCSP.stCSPLD.x1 = x1 ;
	stCSP.stCSPLD.y1 = y1 ;
	
	stCSP.ctrl |= (CSPCTRL_LD_UPDATE|CSPCTRL_TRIGGER_ISP);
}
// ----------------------------------------------------------------------
void SetCSP_LD_P2(BYTE x2, WORD y2)
{
	stCSP.stCSPLD.x2 = x2 ;
	stCSP.stCSPLD.y2 = y2 ;
	
	stCSP.ctrl |= (CSPCTRL_LD_UPDATE|CSPCTRL_TRIGGER_ISP);		
}
// ----------------------------------------------------------------------
void SetCSP_LD_P3(BYTE x3, WORD y3)
{
	stCSP.stCSPLD.x3 = x3 ;
	stCSP.stCSPLD.y3 = y3 ;
	
	stCSP.ctrl |= (CSPCTRL_LD_UPDATE|CSPCTRL_TRIGGER_ISP);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
void SetCSP_VD_EN(BYTE en)
{
	stCSP.stCSPVD.en = en ;
	stCSP.ctrl |= (CSPCTRL_VD_ENABLE|CSPCTRL_TRIGGER_ISP);
}
// ----------------------------------------------------------------------
void SetCSP_VD_P0(BYTE x0, WORD y0)
{
	stCSP.stCSPVD.x0 = x0 ;
	stCSP.stCSPVD.y0 = y0 ;
	
	stCSP.ctrl |= (CSPCTRL_VD_UPDATE|CSPCTRL_TRIGGER_ISP);		
}
// ----------------------------------------------------------------------
void SetCSP_VD_P1(BYTE x1, WORD y1)
{
	stCSP.stCSPVD.x1 = x1 ;
	stCSP.stCSPVD.y1 = y1 ;
	
	stCSP.ctrl |= (CSPCTRL_VD_UPDATE|CSPCTRL_TRIGGER_ISP);
}
// ----------------------------------------------------------------------
void SetCSP_VD_P2(BYTE x2, WORD y2)
{
	stCSP.stCSPVD.x2 = x2 ;
	stCSP.stCSPVD.y2 = y2 ;
	
	stCSP.ctrl |= (CSPCTRL_VD_UPDATE|CSPCTRL_TRIGGER_ISP);		
}
// ----------------------------------------------------------------------
void SetCSP_VD_P3(BYTE x3, WORD y3)
{
	stCSP.stCSPVD.x3 = x3 ;
	stCSP.stCSPVD.y3 = y3 ;
	
	stCSP.ctrl |= (CSPCTRL_VD_UPDATE|CSPCTRL_TRIGGER_ISP);
}
//-----------------------------------------------------------------------

#endif // MDIN-i51X (i510, i540, i550)

