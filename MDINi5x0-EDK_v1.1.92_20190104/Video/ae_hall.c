// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if defined(__USE_DCIRIS_HALL__) || defined(__USE_P_IRIS_ONLY__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

typedef	struct
{
	LONG	Ysum;				// current AE integral summation value
	LONG	Yprv;				// previous AE integral summation value

	SHORT	gain;				// HALL current gain
	SHORT	igain;				// HALL current calculated gain
	SHORT	min;					// HALL minimum value
	SHORT	max;				// HALL maximum value
	SHORT	omin;				// HALL optimal minimum
	SHORT	omax;				// HALL optimal maximum
	SHORT	pmin;				// HALL pysical minimum
	SHORT	pmax;				// HALL pysical maximum

	BYTE	proc;				// AE HALL control process counter
	BYTE	frmID;				// AE HALL control wait-VD counter
	BYTE	cntID;				// AE HALL control average counter
}	AE_HALL_PARAM, *PAE_HALL_PARAM;

#define		AEHALL_STABLE_PROC		0x00		// case of stable
#define		AEHALL_OVERH0_PROC		0x10		// case of over-high0
#define		AEHALL_OVERH1_PROC		0x11		// case of over-high1
#define		AEHALL_OVERL0_PROC		0x20		// case of over-low0
#define		AEHALL_OVERL1_PROC		0x21		// case of over-low1
#define		AEHALL_OVER_CALC			0x24		// case of over-low3

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static AE_HALL_PARAM stHALL;

static BYTE aeMID = 0;
static BYTE aeXID = 0;

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

static BOOL fForceIRISCtrl = FALSE;
WORD resultGain, currentGain;

#define ISTEP_1H			4
#define ISTEP_1L			4

#define NGAIN_SCALE_L	12	// large : slow AE  small : fast AE
#define NGAIN_SCALE_H	12

//--------------------------------------------------------------------------------------------------------------------------
SHORT DCLENS_IRISCtrlGainConv()
{
#if defined(__USE_X30X_TAMRON__)||defined(__USE_X33X_CHINA__)||defined(__USE_X10X_UNION__)||defined(__USE_OLPF_SUNNY__) || defined(__USE_X10X_ZMAX__)
	return (stHALL.pmax - stHALL.gain + stHALL.pmin);
#endif
#if defined(__USE_OLPF_CANON__) || defined(__USE_X03X_ZMAX__) || defined(__USE_X12X_UNION__)
	return stHALL.gain;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT DCLENS_IRISCtrlGainConvVal(SHORT val)
{
#if defined(__USE_X30X_TAMRON__)||defined(__USE_X33X_CHINA__)||defined(__USE_X10X_UNION__)||defined(__USE_OLPF_SUNNY__) || defined(__USE_X10X_ZMAX__)
	return (IRIS_PMAX - val);
#endif
#if defined(__USE_OLPF_CANON__) || defined(__USE_X03X_ZMAX__) || defined(__USE_X12X_UNION__)
	return val;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
// Bright(Yref ~ ) --> IRIS close --> gain increase
// Dark(0 ~ Yref) --> IRIS open --> gain decrease
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
static SHORT ngainBak = 0;
static void DCLENS_IRISCtrlDefineProc(PAE_CTRL_PARAM pCTRL)
{
	int fReStart = 0;
	SHORT tmpl;


	if(pCTRL->Ydiff > (pCTRL->band+700*1)) {
		stHALL.frmID = 1;

		tmpl = (SHORT)((FLOAT)pCTRL->Yref/stHALL.Ysum*stHALL.gain) - stHALL.gain;	aeMID = 0x01;
		tmpl = MAX(MIN(tmpl/NGAIN_SCALE_L, -1), (stHALL.omin - stHALL.omax)/4); // max = -1
		stHALL.igain = stHALL.gain + tmpl;
		stHALL.igain = MIN(MAX(stHALL.igain, stHALL.omin), stHALL.omax);
		tmpl = stHALL.igain - stHALL.gain;
	} else if(pCTRL->Ydiff < -(pCTRL->band+700*1)) {
		stHALL.frmID = 1;

		tmpl = (SHORT)((FLOAT)pCTRL->Yref/stHALL.Ysum*stHALL.gain) - stHALL.gain;	aeMID = 0x11;
		tmpl = MAX(MIN(tmpl/NGAIN_SCALE_H, (stHALL.omax - stHALL.omin)/4), 1); // min = 1
		stHALL.igain = stHALL.gain + tmpl;
		stHALL.igain = MIN(MAX(stHALL.igain, stHALL.omin), stHALL.omax);
		tmpl = stHALL.igain - stHALL.gain;
	} else {
		tmpl = 0;
	}

#if defined(__USE_X12X_UNION__)
	resultGain = (WORD) DCLENS_IRISCtrlGainConv(); // current gain
#elif defined(__USE_DCIRIS_HALL__)
	resultGain = IRIS_WordRead(SPI_AN41908_ADDR, 0x0C); // IRIS result gain
#endif
	currentGain = (WORD) DCLENS_IRISCtrlGainConv(); // current gain

	if(((tmpl > 0) && (ngainBak > 0)) ||((tmpl < 0) && (ngainBak < 0)))	fReStart = 0; // if direction is different... update always
	else if(tmpl !=0)											fReStart = 1;
	else														fReStart = 0;
#if defined(__USE_X03X_ZMAX__)
	if((DIFF(resultGain,currentGain) <=5) || (fReStart == 1))
		stHALL.gain = stHALL.igain;
#elif defined(__USE_DCIRIS_HALL__)
	if((DIFF(resultGain,currentGain) <=1) || (fReStart == 1))
		stHALL.gain = stHALL.igain;
#else
	stHALL.gain = stHALL.igain;
#endif
	ngainBak = tmpl;

#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01 || pCTRL==NULL) return;
	UARTprintf("%d %d %d %d\r\n", stHALL.omin, stHALL.omax, stHALL.min, stHALL.max);
	UARTprintf("[AE] IRIS / Ydiff : %d stHALL.gain : %d stHALL.igain : %d\r\n", pCTRL->Ydiff, stHALL.gain, stHALL.igain);
	UARTprintf("[AE] xID : %2x  mID : %2x\r\n", aeXID, aeMID);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void DCLENS_IRISCtrlUpdateMode(PAE_CTRL_PARAM pCTRL)
{
	if(fForceIRISCtrl == FALSE)
		stAEXFER.iris = (DWORD) DCLENS_IRISCtrlGainConv();

#if defined(__USE_X12X_UNION__)
		AECTRL_PIRIS_SetPosition((SHORT)stAEXFER.iris);
#endif
	//UARTprintf("stAEXFER.iris : %d\r\n",stAEXFER.iris);
}

//--------------------------------------------------------------------------------------------------------------------------
static void DCLENS_IRISHandler(PAE_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&AECTRL_PROC_MASK)!=AECTRL_PROC_IRIS) return;
	//if (stHALL.cntID<MAX(stHALL.frmID,1)) stHALL.cntID++;
	//if (stHALL.cntID<MAX(stHALL.frmID,1)) return;
	stHALL.cntID = 0;

	DCLENS_IRISCtrlDefineProc(pCTRL);	// define process
	DCLENS_IRISCtrlUpdateMode(pCTRL);	// update mode
}

//--------------------------------------------------------------------------------------------------------------------------
extern BOOL AutoFLKremove;
static void DCLENS_ModeHandler(PAE_CTRL_PARAM pCTRL)
{
	LONG mode = stWBCTRL.ctrl&WBCTRL_MODE_MASK;
	//LONG longTmp;

	pCTRL->ctrl &= (~AECTRL_PROC_MASK);

	pCTRL->ctrl &= (~AECTRL_GAINCTL_MANU); // gain auto
	pCTRL->ctrl &= (~AECTRL_SHUTCTL_MANU); // shut auto

	if((pCTRL->ctrl & AECTRL_PRIORITY_MASK) == AECTRL_PRIORITY_AUTO) {
		if (mode==WBCTRL_INDOOR_MODE) { // InDoor mode
			//
			// if gain is inc (means dark) 
			//
			if(AECTRL_GetGAINGain() > AECTRL_GetGAINPMax()/32) { //8192
				DCLENS_SetIRISOptiMax(stHALL.max);	aeXID = 0x00;
			} else if(AECTRL_GetGAINGain() < AECTRL_GetGAINPMax()/512) { // 512
				DCLENS_SetIRISOptiMax(IRIS_OMAX);		aeXID = 0x01;
			}

			if (stHALL.Ysum > (pCTRL->Yref+pCTRL->band*1)) {
				if(AECTRL_IsSensUpMin() == FALSE) {
					pCTRL->ctrl |= AECTRL_PROC_SENS;	aeMID = 0x00;
				}else if (AECTRL_IsGAINCtrlMin()!=TRUE) {
					pCTRL->ctrl |= AECTRL_PROC_GAIN;	aeMID = 0x01;
				} else if (AECTRL_IsSHUTCtrlGmin() != TRUE){
					pCTRL->ctrl |= AECTRL_PROC_SHUT;	aeMID = 0x03;
				} else {
					pCTRL->ctrl |= AECTRL_PROC_IRIS;	aeMID = 0x02;
				}
			} else if (stHALL.Ysum < (pCTRL->Yref-pCTRL->band*1)){
				if (DCLENS_IsIRISCtrlOptiMax()!=TRUE) {
					pCTRL->ctrl |= AECTRL_PROC_IRIS;	aeMID = 0x05;
				} else if(AECTRL_IsSHUTCtrlGmax() != TRUE) {
					pCTRL->ctrl |= AECTRL_PROC_SHUT;	aeMID = 0x04;
				} else if(AECTRL_IsGAINCtrlMax()!=TRUE) {
					pCTRL->ctrl |= AECTRL_PROC_GAIN;	aeMID = 0x06;
				} else {
					if(AECTRL_IsSensUpMax() == FALSE) {
						pCTRL->ctrl |= AECTRL_PROC_SENS;	aeMID = 0x07;
					}
				}
			}
		}else { // OutDoor mode // auto default
			//control large F value (bright)
			if((AECTRL_IsGAINCtrlMin() == TRUE) && (AECTRL_GetSHUTGain() > 1000)) {
				DCLENS_SetIRISOptiMin(stHALL.min);	aeXID = 0x10;
				//UARTprintf("min...%d\r\n",AECTRL_GetSHUTGain());
 			} else if(AECTRL_GetGAINGain() > AECTRL_GetGAINMin()) {
				DCLENS_SetIRISOptiMin(IRIS_OMIN);	aeXID = 0x11;
 				if(stHALL.gain < IRIS_OMIN) stHALL.gain = IRIS_OMIN;
				//UARTprintf("omin...\r\n");
			}

#if defined(__USE_CMOS_IMX290__)  || defined(__USE_CMOS_IMX327__)
			//control smal F value (dark)
			if(AECTRL_GetGAINGain() > AECTRL_GetGAINPMax()/32) { // 0x2000(8192)
				DCLENS_SetIRISOptiMax(stHALL.max);	aeXID = 0x12;
			} else if(AECTRL_GetGAINGain() < AECTRL_GetGAINPMax()/256) { //0x400(1024)
				DCLENS_SetIRISOptiMax(IRIS_OMAX);	aeXID = 0x13;
				if(stHALL.gain > IRIS_OMAX) stHALL.gain = IRIS_OMAX;
			}
#elif defined(__USE_CMOS_IMX385__)
			//control smal F value (dark)
			if(AECTRL_GetGAINGain() > AECTRL_GetGAINPMax()/32) { // 0x2000(8192)
				DCLENS_SetIRISOptiMax(stHALL.max);	aeXID = 0x12;
			} else if(AECTRL_GetGAINGain() < AECTRL_GetGAINPMax()/256) { //0x400(1024)
				DCLENS_SetIRISOptiMax(IRIS_OMAX);	aeXID = 0x13;
				if(stHALL.gain > IRIS_OMAX) stHALL.gain = IRIS_OMAX;
			}
#elif defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__)
			//control smal F value (dark)
			if(AECTRL_GetGAINGain() > 7*AECTRL_GetGAINPMax()/16) {
				DCLENS_SetIRISOptiMax(stHALL.max);	aeXID = 0x12;
			} else if(AECTRL_GetGAINGain() < AECTRL_GetGAINPMax()/4) {
				DCLENS_SetIRISOptiMax(IRIS_OMAX);	aeXID = 0x13;
				if(stHALL.gain > IRIS_OMAX) stHALL.gain = IRIS_OMAX;
			}
#else
			//control smal F value (dark)
			if(AECTRL_GetGAINGain() > AECTRL_GetGAINPMax()/4) {
				DCLENS_SetIRISOptiMax(stHALL.max);	aeXID = 0x12;
			} else if(AECTRL_GetGAINGain() < AECTRL_GetGAINPMax()/8) {
				DCLENS_SetIRISOptiMax(IRIS_OMAX);	aeXID = 0x13;
				if(stHALL.gain > IRIS_OMAX) stHALL.gain = IRIS_OMAX;
			}
#endif
			if (stHALL.Ysum > (pCTRL->Yref+pCTRL->band*1)) {
				if(AutoFLKremove==FALSE) {
					if(AECTRL_IsSensUpMin() == FALSE) {
						pCTRL->ctrl |= AECTRL_PROC_SENS;	aeMID = 0x10;
					} else if (AECTRL_IsGAINCtrlMin()!=TRUE) {
						pCTRL->ctrl |= AECTRL_PROC_GAIN;	aeMID = 0x11;
					} else if(AECTRL_IsSHUTCtrlGmin() != TRUE) {
						pCTRL->ctrl |= AECTRL_PROC_SHUT;	aeMID = 0x12;
					} else {
						pCTRL->ctrl |= AECTRL_PROC_IRIS;	aeMID = 0x13;
					}
				} else {
					if(AECTRL_IsSensUpMin() == FALSE) {
						pCTRL->ctrl |= AECTRL_PROC_SENS;	aeMID = 0x10;
					} else if (AECTRL_IsGAINCtrlMin()!=TRUE) {
						pCTRL->ctrl |= AECTRL_PROC_GAIN;	aeMID = 0x11;
					} else if (DCLENS_IsIRISCtrlOptiMin()!=TRUE){
						pCTRL->ctrl |= AECTRL_PROC_IRIS;	aeMID = 0x13;
					} else {
						pCTRL->ctrl |= AECTRL_PROC_SHUT;	aeMID = 0x12;
					}
				}
			} else if (stHALL.Ysum < (pCTRL->Yref-pCTRL->band*1)){
				if(AutoFLKremove==FALSE) {
					if (DCLENS_IsIRISCtrlOptiMax()!=TRUE) {
						pCTRL->ctrl |= AECTRL_PROC_IRIS;	aeMID = 0x14;
					} else if(AECTRL_IsSHUTCtrlGmax() != TRUE) {
						pCTRL->ctrl |= AECTRL_PROC_SHUT;	aeMID = 0x15;
					} else if(AECTRL_IsGAINCtrlMax()!=TRUE){
						pCTRL->ctrl |= AECTRL_PROC_GAIN;	aeMID = 0x16;
					} else {
						if(AECTRL_IsSensUpMax() == FALSE) {
							pCTRL->ctrl |= AECTRL_PROC_SENS;	aeMID = 0x17;
						}
					}
				} else {
					if(AECTRL_IsSHUTCtrlGmax() != TRUE) {
						pCTRL->ctrl |= AECTRL_PROC_SHUT;	aeMID = 0x15;
					} else if (DCLENS_IsIRISCtrlOptiMax()!=TRUE) {
						pCTRL->ctrl |= AECTRL_PROC_IRIS;	aeMID = 0x14;
					} else if(AECTRL_IsGAINCtrlMax()!=TRUE){
						pCTRL->ctrl |= AECTRL_PROC_GAIN;	aeMID = 0x16;
					} else {
						if(AECTRL_IsSensUpMax() == FALSE) {
							pCTRL->ctrl |= AECTRL_PROC_SENS;	aeMID = 0x17;
						}
					}
				}
			}
		}
	} else if((pCTRL->ctrl & AECTRL_PRIORITY_MASK) == AECTRL_PRIORITY_SHUTTER) {

		// set Shutter here

		pCTRL->ctrl &= (~AECTRL_GAINCTL_MANU); // gain auto
		pCTRL->ctrl |= AECTRL_SHUTCTL_MANU; // shut manual

		DCLENS_SetIRISOptiMin(stHALL.min);

		if(AECTRL_GetGAINGain() > AECTRL_GetGAINPMax()/32) {
			DCLENS_SetIRISOptiMax(stHALL.max);	aeXID = 0x20;
		} else if(AECTRL_GetGAINGain() < AECTRL_GetGAINPMax()/256) {
			DCLENS_SetIRISOptiMax(IRIS_OMAX);	aeXID = 0x21;
		}

		if (stHALL.Ysum > (pCTRL->Yref+pCTRL->band*1)) {
			if (AECTRL_IsGAINCtrlMin()!=TRUE) {
				pCTRL->ctrl |= AECTRL_PROC_GAIN;	aeMID = 0x20;
			} else {
				pCTRL->ctrl |= AECTRL_PROC_IRIS;	aeMID = 0x21;
			}
		} else if (stHALL.Ysum < (pCTRL->Yref-pCTRL->band*1)){
			if (DCLENS_IsIRISCtrlOptiMax()!=TRUE) {
				pCTRL->ctrl |= AECTRL_PROC_IRIS;	aeMID = 0x22;
			} else {
				pCTRL->ctrl |= AECTRL_PROC_GAIN;	aeMID = 0x23;
			}
		}
	} else if((pCTRL->ctrl & AECTRL_PRIORITY_MASK) == AECTRL_PRIORITY_APERTURE) {

		// set Apeture here

		pCTRL->ctrl &= (~AECTRL_GAINCTL_MANU); // gain auto
		pCTRL->ctrl &= (~AECTRL_SHUTCTL_MANU); // shut auto

		if (stHALL.Ysum > (pCTRL->Yref+pCTRL->band*1)) {
			if(AECTRL_IsSensUpMin() == FALSE) {
				pCTRL->ctrl |= AECTRL_PROC_SENS;	aeMID = 0x30;
 			} else if (AECTRL_IsGAINCtrlMin()!=TRUE) {
 				pCTRL->ctrl |= AECTRL_PROC_GAIN;	aeMID = 0x31;
			} else {
 				pCTRL->ctrl |= AECTRL_PROC_SHUT;	aeMID = 0x32;
			}
		} else if(stHALL.Ysum < (pCTRL->Yref-pCTRL->band*1)) {
			if (AECTRL_IsSHUTCtrlGmax()!=TRUE) {
 				pCTRL->ctrl |= AECTRL_PROC_SHUT;	aeMID = 0x33;
			} else if(AECTRL_IsGAINCtrlMax()!=TRUE) {
 				pCTRL->ctrl |= AECTRL_PROC_GAIN;	aeMID = 0x34;
			} else {
				if(AECTRL_IsSensUpMax() == FALSE) {
 					pCTRL->ctrl |= AECTRL_PROC_SENS;	aeMID = 0x35;
				}
			}
		}
	} else { // AECTRL_PRIORITY_GAIN
		pCTRL->ctrl |= AECTRL_GAINCTL_MANU; // gain manual
		pCTRL->ctrl &= (~AECTRL_SHUTCTL_MANU); // shut auto

		DCLENS_SetIRISOptiMin(stHALL.min);

		if (stHALL.Ysum > (pCTRL->Yref+pCTRL->band*1)) {
			if(AutoFLKremove==FALSE) {
				if(AECTRL_IsSensUpMin() == FALSE) {
					pCTRL->ctrl |= AECTRL_PROC_SENS;	aeMID = 0x40;
				} else if(AECTRL_IsSHUTCtrlGmin() != TRUE) {
					pCTRL->ctrl |= AECTRL_PROC_SHUT;	aeMID = 0x41;
				} else {
					pCTRL->ctrl |= AECTRL_PROC_IRIS;	aeMID = 0x42;
				}
			} else {
				if(AECTRL_IsSensUpMin() == FALSE) {
					pCTRL->ctrl |= AECTRL_PROC_SENS;	aeMID = 0x43;
				} else if(DCLENS_IsIRISCtrlOptiMin() != TRUE) {
					pCTRL->ctrl |= AECTRL_PROC_IRIS;	aeMID = 0x44;
				} else if(AECTRL_IsSHUTCtrlGmin() != TRUE) {
					pCTRL->ctrl |= AECTRL_PROC_SHUT;	aeMID = 0x45;
				}
			}
		} else if (stHALL.Ysum < (pCTRL->Yref-pCTRL->band*1)) {
			if(AutoFLKremove==FALSE) {
				if(DCLENS_IsIRISCtrlOptiMax() != TRUE) {
					pCTRL->ctrl |= AECTRL_PROC_IRIS;	aeMID = 0x46;
				} else if(AECTRL_IsSHUTCtrlGmax()!=TRUE) {
					pCTRL->ctrl |= AECTRL_PROC_SHUT;	aeMID = 0x47;
				} else {
					if(AECTRL_IsSensUpMax() == FALSE) {
						pCTRL->ctrl |= AECTRL_PROC_SENS;	aeMID = 0x48;
					}
				}
			} else {
				if(AECTRL_IsSHUTCtrlGmax()!=TRUE) {
					pCTRL->ctrl |= AECTRL_PROC_SHUT;	aeMID = 0x49;
				} else if(DCLENS_IsIRISCtrlOptiMax() != TRUE) {
					pCTRL->ctrl |= AECTRL_PROC_IRIS;	aeMID = 0x4a;
				} else {
					if(AECTRL_IsSensUpMax() == FALSE) {
						pCTRL->ctrl |= AECTRL_PROC_SENS;	aeMID = 0x4b;
					}
				}
			}
		}
	}

#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01 || pCTRL==NULL) return;
	UARTprintf("[AE] stHALL.omax : %d omin : %d gain : %d  / gaingain : %d[%d~%d] \r\n", stHALL.omax, stHALL.omin, stHALL.gain, AECTRL_GetGAINGain(), AECTRL_GetGAINMin(), AECTRL_GetGAINPMax());
	UARTprintf("[AE] xID : %2x  mID : %2x\r\n", aeXID, aeMID);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void DCLENS_IRISinGAIN(PAE_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&AECTRL_PROC_MASK)!=AECTRL_PROC_GAIN) return;

	DCLENS_ClearModeIRIS();
}

static BYTE slowAE = 1;
static BYTE minCnt = 0;
static WORD mYsum_b = 0;
static LONG minYsum = 0;
static WORD lYsum = 0;

static int slowTime[49] = {	1,    1,      2,    3,    4,   5,   6,    7,   8,   9,  10,
							14,   18,  22,  26,  30, 34,  38,  42, 46,  50,
							58,   66,  74,  82,  90, 98,106,114,122,130,
							146,162,178,194,210,226,242,258,274,290,
							322,354,380,418,450,482,515,546 }; // range 01 ~ 0x30(48)

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_AECtrlProcess(PAE_CTRL_PARAM pCTRL)
{
	WORD mYsum;
	SHORT diff;

	if (pCTRL->ctrl&AECTRL_PROCESS_OFF) return;

	if((slowAE > 48) || (slowAE == 0))
		slowAE = 1;

	if(slowAE == 1) {
		stHALL.Ysum = pCTRL->Ysum;
		minCnt = 0;
		minYsum = 0;
		mYsum_b = pCTRL->Ysum;
		lYsum = pCTRL->Ysum;
	} else {
		if(minCnt == 15) {
			minCnt = 0;
			minYsum += (LONG)pCTRL->Ysum;
			mYsum = (WORD)(minYsum / 16);
			diff = mYsum - mYsum_b;

			if(diff > 0) {
				lYsum = lYsum + MAX(diff/slowTime[slowAE], 1);
			} else if (diff < 0) {
				lYsum = lYsum + MIN(diff/slowTime[slowAE], -1);
			}

			mYsum_b = mYsum;
			minYsum = 0;
		} else {
			minCnt ++;
			minYsum += (LONG)pCTRL->Ysum;
			return;
		}

		stHALL.Ysum = lYsum;
	}

	DCLENS_ModeHandler(pCTRL);
	AECTRL_SensHandler(pCTRL);
	AECTRL_SHUTHandler(pCTRL);
	AECTRL_GAINHandler(pCTRL);
	DCLENS_IRISHandler(pCTRL);
	DCLENS_IRISinGAIN(pCTRL);
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_setSlowAE(BYTE val)
{
	slowAE = val;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE AECTRL_getSlowAE(void)
{
	return slowAE;
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_ClearModeIRIS(void)
{
	stHALL.cntID = 0;	stHALL.frmID = 1;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL DCLENS_IsIRISCtrlOptiMax(void)
{
	//UARTprintf("stHALL gain:%d/omax:%d\r\n",stHALL.gain,stHALL.omax);
	return (stHALL.gain >= (stHALL.omax - ISTEP_1L*2))? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL DCLENS_IsIRISCtrlOptiMin(void)
{
	return (stHALL.gain <= (stHALL.omin + ISTEP_1H*2))? TRUE : FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_SetIRISRange(SHORT min, SHORT max)
{
	stHALL.min = min;	stHALL.max = max;
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_SetIRISPhyRange(SHORT pmin, SHORT pmax)
{
	stHALL.pmin = pmin;	stHALL.pmax = pmax;
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_SetIRISOptiRange(SHORT omin, SHORT omax)
{
	stHALL.omin = omin;	stHALL.omax = omax;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT DCLENS_GetIRISOptiMax(void)
{
	return stHALL.omax;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT DCLENS_GetIRISOptiMin(void)
{
	return stHALL.omin;
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_SetIRISOptiMax(SHORT omax)
{
	stHALL.omax = omax;
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_SetIRISOptiMin(SHORT omin)
{
	stHALL.omin = omin;
}
//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_SetIRISGain(SHORT gain)
{
	stHALL.gain = gain;

	DCLENS_IRISCtrlUpdateMode(NULL);

	stHALL.cntID = 0;	stHALL.frmID = 1;
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_ForceSetIRISGain2(SHORT gain)
{
	stHALL.gain = gain;
	DCLENS_IRISCtrlUpdateMode(NULL);
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_ForceSetIRISGain(SHORT gain) // use after stop AE
{
	stHALL.gain = gain;
	DCLENS_IRISCtrlUpdateMode(NULL);

#if defined(__USE_X12X_UNION__)
	AECTRL_PIRIS_ForceSetPosition((SHORT)stAEXFER.iris);
#elif defined(__USE_DCIRIS_HALL__)
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x00, stAEXFER.iris);
#endif
	stHALL.cntID = 0;	stHALL.frmID = 1;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT DCLENS_GetIRISGain(void)
{
	return stHALL.gain;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT DCLENS_GetIRISMin(void)
{
	return stHALL.min;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT DCLENS_GetIRISMax(void)
{
	return stHALL.max;
}

//--------------------------------------------------------------------------------------------------------------------------
WORD DCLENS_GetIRISState(void)
{
	return (WORD)stHALL.proc;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE DCLENS_GetIRISValue(void)
{
	return (AECTRL_GetAE_IRISBRValue((LONG)stHALL.gain));
}

void DCLENS_IRISInit(void)
{
	DCLENS_SetIRISPhyRange(IRIS_PMIN, IRIS_PMAX);
	DCLENS_SetIRISOptiRange(IRIS_OMIN, IRIS_OMAX);
	stHALL.cntID = 0;	stHALL.frmID = 1;
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_ForceIRISCtl(BYTE val) // IRIS open/close
{
	if(val == 1) {
		fForceIRISCtrl = TRUE;
		stAEXFER.iris = DCLENS_IRISCtrlGainConvVal(0);
#if defined(__USE_X12X_UNION__)
		AECTRL_PIRIS_ForceSetPosition((SHORT)stAEXFER.iris);
#elif defined(__USE_DCIRIS_HALL__)
		IRIS_WordWrite(SPI_AN41908_ADDR, 0x00, stAEXFER.iris);
#endif
		UARTprintf("IRIS Close...\r\n");
	} else if(val == 2) {
		fForceIRISCtrl = TRUE;
		stAEXFER.iris = DCLENS_IRISCtrlGainConvVal(IRIS_PMAX);
#if defined(__USE_X12X_UNION__)
		AECTRL_PIRIS_ForceSetPosition((SHORT)stAEXFER.iris);
#elif defined(__USE_DCIRIS_HALL__)
		IRIS_WordWrite(SPI_AN41908_ADDR, 0x00, stAEXFER.iris);
#endif
		UARTprintf("IRIS Open...\r\n");
	} else {
		fForceIRISCtrl = FALSE;
		UARTprintf("IRIS Auto...\r\n");
	}
	
}

void DCLENS_ForceIRISUpDown(BOOL updn) // IRIS set value up/down
{
	if(updn == 0)		stHALL.gain +=5;
	else				stHALL.gain -=5;
	stHALL.gain = MIN(MAX(stHALL.gain,IRIS_PMIN),IRIS_PMAX);
	DCLENS_SetIRISGain(stHALL.gain);
	//UARTprintf("stHALL.gain : %d\r\n", stHALL.gain);
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_ForceIRISOpen(BOOL val) //TRUE = IRIS open ,  FALSE = Normal
{
	fForceIRISCtrl = TRUE;
	stAEXFER.iris = DCLENS_IRISCtrlGainConvVal(IRIS_PMAX);
#if defined(__USE_X12X_UNION__)
	AECTRL_PIRIS_ForceSetPosition((SHORT)stAEXFER.iris);
#elif defined(__USE_DCIRIS_HALL__)
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x00, stAEXFER.iris);
#endif
}

#endif

/*  FILE_END _HERE */
