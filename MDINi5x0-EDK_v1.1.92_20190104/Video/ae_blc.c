// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

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
extern FLOAT aeWinRatio;
void AECTRL_BLCModeHandler(PAE_CTRL_PARAM pCTRL)
{
	DWORD mode = (pCTRL->ctrl&AECTRL_BLC_MASK);

	if ((pCTRL->ctrl&AECTRL_TRIGGER_ISP)==0) return;
	pCTRL->ctrl &= (~AECTRL_TRIGGER_ISP);

	if (mode==AECTRL_BLC_MODE_OFF) {							// set FIX window
		MISP_RegWrite(MISP_LOCAL_ID, 0x680, MAKEWORD(pCTRL->stFIX.sX, pCTRL->stFIX.sY));
		MISP_RegWrite(MISP_LOCAL_ID, 0x681, MAKEWORD(pCTRL->stFIX.nW, pCTRL->stFIX.nH));

		MISP_RegWrite(MISP_LOCAL_ID, 0x682, pCTRL->stFIX.W0);	// set FIX weight
		MISP_RegWrite(MISP_LOCAL_ID, 0x683, MAKEWORD(pCTRL->stFIX.W1, pCTRL->stFIX.W4));
		MISP_RegWrite(MISP_LOCAL_ID, 0x684, MAKEWORD(pCTRL->stFIX.W2, pCTRL->stFIX.W3));
	}
	else if (mode==AECTRL_BLC_MODE_BLC) {						// set BLC window
		MISP_RegWrite(MISP_LOCAL_ID, 0x680, MAKEWORD(pCTRL->stBLC.sX, pCTRL->stBLC.sY));
		MISP_RegWrite(MISP_LOCAL_ID, 0x681, MAKEWORD(pCTRL->stBLC.nW, pCTRL->stBLC.nH));
		//UARTprintf("BLC window:%d/%d/%d/%d - ",pCTRL->stBLC.sX,pCTRL->stBLC.sY,pCTRL->stBLC.nW,pCTRL->stBLC.nH);

		MISP_RegWrite(MISP_LOCAL_ID, 0x682, pCTRL->stBLC.W0);	// set BLC weight
		MISP_RegWrite(MISP_LOCAL_ID, 0x683, MAKEWORD(pCTRL->stBLC.W1, pCTRL->stBLC.W4));
		MISP_RegWrite(MISP_LOCAL_ID, 0x684, MAKEWORD(pCTRL->stBLC.W2, pCTRL->stBLC.W3));
		//UARTprintf("BLC weight:%d/%d/%d/%d/%d\r\n",pCTRL->stBLC.W0,pCTRL->stBLC.W1,pCTRL->stBLC.W2,pCTRL->stBLC.W3,pCTRL->stBLC.W4);
	}
	else if (mode==AECTRL_BLC_MODE_SPOT) {						// set SPOT window
		MISP_RegWrite(MISP_LOCAL_ID, 0x680, MAKEWORD(pCTRL->stSPOT.sX, pCTRL->stSPOT.sY));
		MISP_RegWrite(MISP_LOCAL_ID, 0x681, MAKEWORD(pCTRL->stSPOT.nW, pCTRL->stSPOT.nH));

		MISP_RegWrite(MISP_LOCAL_ID, 0x682, pCTRL->stSPOT.W0);	// set SPOT weight
		MISP_RegWrite(MISP_LOCAL_ID, 0x683, MAKEWORD(pCTRL->stSPOT.W1, pCTRL->stSPOT.W4));
		MISP_RegWrite(MISP_LOCAL_ID, 0x684, MAKEWORD(pCTRL->stSPOT.W2, pCTRL->stSPOT.W3));
	}
	else {														// set HLC window
		MISP_RegWrite(MISP_LOCAL_ID, 0x680, MAKEWORD(pCTRL->stHLC.sX, pCTRL->stHLC.sY));
		MISP_RegWrite(MISP_LOCAL_ID, 0x681, MAKEWORD(pCTRL->stHLC.nW, pCTRL->stHLC.nH));

		MISP_RegWrite(MISP_LOCAL_ID, 0x682, pCTRL->stHLC.W0);	// set HLC weight
		MISP_RegWrite(MISP_LOCAL_ID, 0x683, MAKEWORD(pCTRL->stHLC.W1, pCTRL->stHLC.W4));
		MISP_RegWrite(MISP_LOCAL_ID, 0x684, MAKEWORD(pCTRL->stHLC.W2, pCTRL->stHLC.W3));
	}

	MISP_RegWrite(MISP_LOCAL_ID, 0x687, ((pCTRL->stHLC.max)&0xff)<<8);	// set HLC clip-factor //lsb
	MISP_RegWrite(MISP_LOCAL_ID, 0x686, (pCTRL->stHLC.max)>>8);	// set HLC clip-factor // msb
	MISP_RegWrite(MISP_LOCAL_ID, 0x685, MAKEWORD(pCTRL->stHLC.fix, pCTRL->stHLC.val));

	// set HLC-clipping mode ON/OFF
	MISP_RegField(MISP_LOCAL_ID, 0x600, 7, 1, (mode==AECTRL_BLC_MODE_HLC)? ON : OFF);

	aeWinRatio = AECTRL_CalAERatio(mode,pCTRL);
	//UARTprintf("%2.3f\r\n",aeWinRatio);
}

/*  FILE_END _HERE */
