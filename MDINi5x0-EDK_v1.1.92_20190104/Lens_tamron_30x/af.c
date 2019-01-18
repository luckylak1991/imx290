// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if defined(__USE_AFZOOM_LENS__)
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

	stAFCTRL.ctrl = AFCTRL_AFAUTO_MODE;
//	stAFCTRL.ctrl = AFCTRL_MANUAL_MODE;

	LENS_SetFOKSDrvLIMIT(&stAFCTRL, AFFOKS_LIMIT_0p3M);	// set 0.3m
	LENS_SetFOKSDrvSPEED(&stAFCTRL, AFFOKS_SPEED_VAR0);	// set slow

	LENS_SetWIDEDrvLIMIT(&stAFCTRL, 0);					// set wide
	LENS_SetTELEDrvLIMIT(&stAFCTRL, 0);					// set tele
	LENS_SetZOOMDrvSPEED(&stAFCTRL, AFZOOM_SPEED_VAR4);	// set fast
	LENS_SetAFIntvalTIME(&stAFCTRL, 5);		// set intval-time 5sec
	LENS_SetAFActiveTIME(&stAFCTRL, 5);		// set active-time 5sec

	LENS_SetDIGIDrvOnOff(&stAFCTRL, OFF);
	LENS_SetDIGIMixOnOff(&stAFCTRL, OFF);
	LENS_SetDIGIDrvLIMIT(&stAFCTRL, DZOOMCTRL_RATIO_MAX);

	AF_SetAUTOInfo(&stAFCTRL, 0);	// initialize auto-info 
}

FLOAT AFCTRL_CalAFRatio(void)
{
	return 1.0f;
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
