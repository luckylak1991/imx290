// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

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
FLOAT afWinRatio = 1.0f;
void AFCTRL_SetInstancePARAM(PAF_CTRL_PARAM pCTRL)
{
	memset((PBYTE)pCTRL, 0, sizeof(AF_CTRL_PARAM));


	//LENS_SetFOKSDrvSPEED(&stAFCTRL, AFFOKS_SPEED_VAR0);	// set slow
	LENS_SetFOKSDrvSPEED(&stAFCTRL, AFFOKS_SPEED_VAR4);	// set slow

	LENS_SetZOOMDrvSPEED(&stAFCTRL, AFZOOM_SPEED_VAR4);	// set fast
	LENS_SetAFIntvalTIME(&stAFCTRL, 5);		// set intval-time 5sec
	LENS_SetAFActiveTIME(&stAFCTRL, 5);		// set active-time 5sec

	LENS_SetDIGIDrvOnOff(&stAFCTRL, OFF);
	LENS_SetDIGIMixOnOff(&stAFCTRL, OFF);
	LENS_SetDIGIDrvLIMIT(&stAFCTRL, DZOOMCTRL_RATIO_MAX);

	AF_SetFILTCoef();				// initilaize filt-coef
	AF_SetAUTOInfo(&stAFCTRL, 0);	// initialize auto-info 

	afWinRatio = AFCTRL_CalAFRatio();
}

//--------------------------------------------------------------------------------------------------------------------------
FLOAT AFCTRL_CalAFRatio()
{
	WORD tmpBuf;
	WORD tmpW, tmpH, tmpNW, tmpNH;
	FLOAT retF;

	MISP_RegRead(MISP_LOCAL_ID, 0x60F, &tmpBuf);
	tmpW =(tmpBuf >> 8);
	tmpH =(tmpBuf&0xff);
	//UARTprintf("tmpW : %d / tmpH : %d\r\n",tmpW,tmpH);
	MISP_RegRead(MISP_LOCAL_ID, 0x6A1, &tmpBuf);
	tmpNW =(tmpBuf >> 8);
	tmpNH =(tmpBuf&0xff);
	//UARTprintf("tmpNW : %d / tmpNH : %d\r\n",tmpNW,tmpNH);
	//UARTprintf("cal Res : %d x %d\r\n", tmpW*tmpNW, tmpH*tmpNH);
	//UARTprintf("org Res : %d x %d\r\n", AF_WND_DEF_SIZE_W*ODM_WND_DEF_MUL_W, AF_WND_DEF_SIZE_H*ODM_WND_DEF_MUL_H);

	//UARTprintf("ref : %d / cal : %d\r\n",AF_WND_DEF_SIZE_W*ODM_WND_DEF_MUL_W*AF_WND_DEF_SIZE_H*ODM_WND_DEF_MUL_H,tmpW*tmpH*tmpNH*tmpNW);

	retF = (FLOAT)(AF_WND_DEF_SIZE_W*ODM_WND_DEF_MUL_W*AF_WND_DEF_SIZE_H*ODM_WND_DEF_MUL_H) / (FLOAT)(tmpW*tmpH*tmpNH*tmpNW);

	//UARTprintf("AF Window Ratio : %f\r\n", retF);

	return retF;
}



BYTE AFCTRL_GetZoomRatio(void) // for AWB
{
	SHORT curVal, refVal;

	curVal = LENS_GetRelPositionZOOM();
	refVal = LENS_GetRelMaxPositionZOOM();

	if (curVal < 1*refVal/8)		return 1;
	//else if (curVal < 2*refVal/8)	return 2;
	else if (curVal < 3*refVal/8)	return 2;
	//else if (curVal < 4*refVal/8)	return 4;
	else if (curVal < 5*refVal/8)	return 3;
	//else if (curVal < 6*refVal/8)	return 6;
	//else if (curVal < 7*refVal/8)	return 7;
	else 					return 4;
}

#endif

/*  FILE_END_HERE */
