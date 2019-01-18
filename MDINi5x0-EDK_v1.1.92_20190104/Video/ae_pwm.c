// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if  defined(__USE_PWMIRIS_ONLY__)
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
WORD getIRISPosition(void)
{
	return GetADC_ConvData(ADC_SRC_VIN3);
}
		
//--------------------------------------------------------------------------------------------------------------------------
extern BOOL gStopAgcRun;

#define IMIN 0
#define IMAX 4095
#define IFRMMAX 5 //bigger than 1
#define IFRMTH (IFRMMAX+1)/2
#define IFRMDIFTH IFRMMAX-2
#define FRM_DIF_YSUM_TH 20

static WORD offsetIRISfrmRate = 30;

#if (defined(__USE_MANU_WW_4KUHD__) || defined(__USE_MANU_WW_8mmIR__))
#define OFFSET_IRIS_INIT		950		// 30hz / when diff = 0 with no AGC then..... use (printed value - 15)
static WORD irisPF = 100;
#else
#define OFFSET_IRIS_INIT		500		// NEW H/W  // 30hz /  when diff = 0 with no AGC then..... use (printed value - 15)
//#define OFFSET_IRIS_INIT	650		// OLD H/W  // 30hz /  when diff = 0 with no AGC then..... use (printed value - 15)
static WORD irisPF = 50;
#endif

WORD offsetIRISInit = (OFFSET_IRIS_INIT-100);
WORD offsetIRIS = OFFSET_IRIS_INIT;


static WORD defaultOffsetIRIS = 0;
static SHORT integral = 0;
static LONG bakFrmYsum= 0;
static WORD frmCnt = 0;
static SHORT frmDIFFCnt = 0;
static SHORT frmIDCnt = 0;

static LONG bakFrmGain = 0;
static LONG bakFrmPWM = 0;

BOOL initIRIS = TRUE;
#if defined(__USE_PWM_ADC__)
BOOL startInitADC = FALSE;
BYTE adcIRIS = 210;
#endif
static WORD initIRISCntStart = 0;
static WORD initIRISCntEn = 0;

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_IRIS_Init(PAE_CTRL_PARAM pCTRL)
{
	LONG error,PWM,ERROR;
	char diffs[50];

	// for initialise IRIS
	if(initIRIS == FALSE) {
		// frame counter
		if(frmCnt >= IFRMMAX*2) {
			frmCnt = 0;
		} else {
			frmCnt ++;
		}

		if(pCTRL->Ydiff > 0) {
			if(pCTRL->Ydiff > pCTRL->Yref) {
				error = pCTRL->Yref;
				ERROR = pCTRL->Yref;
			} else {
				error = pCTRL->Ydiff;
				ERROR = pCTRL->YDIFF;
			}
		} else {
			error = pCTRL->Ydiff;
			ERROR = pCTRL->YDIFF;
		}

		PWM = error/irisPF + offsetIRISInit;

		if(frmCnt == 0) {
			if(offsetIRISInit > PWM) {
				if(error < 0) {
					if(ERROR > 8000)
						offsetIRISInit -= 12;
					else if(ERROR > 4000)
						offsetIRISInit -= 8;
					else if(ERROR > 2000)
						offsetIRISInit -= 4;
					else
						offsetIRISInit -= 2;
				}
			}else if(offsetIRISInit < PWM) {
				if(error > 0) {
					if(ERROR > 8000)
						offsetIRISInit += 12;
					else if(ERROR > 4000)
						offsetIRISInit += 8;
					else if(ERROR > 2000)
						offsetIRISInit += 4;
					else
						offsetIRISInit += 2;
				}
			}
		}

		initIRISCntStart++;

		sprintf(diffs,"%5d:%4d:%4d", (int)pCTRL->Ydiff,(int)offsetIRISInit,(int)PWM);
		UARTprintf("count: %5d / error : %6d / offsetIRISInit : %4d / PWM : %5d\r\n",initIRISCntStart,error,offsetIRISInit,PWM);

		if(initIRISCntStart == 1) {
	 		AECTRL_SetAE_GainFixMode();
			AECTRL_SetGAINGain(0x10000);
		} else if(initIRISCntStart>= 5) {
			MISP_RegWrite(MISP_LOCAL_ID, 0x67E, PWM);
			if(pCTRL->YDIFF < 500) {
				initIRISCntEn++;
				if(initIRISCntEn > 5) {
					AECTRL_SetAE_AutoMode();
					initIRIS = TRUE;
					initIRISCntEn = 0;
					defaultOffsetIRIS = 0;
					initIRISCntStart = 0;
					offsetIRIS = offsetIRISInit; // mimitest
					offsetIRISInit = (OFFSET_IRIS_INIT-100);

				#if defined(__USE_PWM_ADC__)
					startInitADC = TRUE;
					AECTRL_IRISPWM_SetFullOpen();
				#endif

					sprintf(diffs,"IRIS init done:%d",offsetIRIS);
					UARTprintf("\r\n -------IRIS init done :%d ... \r\n",offsetIRIS);
					#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
					BYTE buff[2];
					buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)(offsetIRIS - (OFFSET_IRIS_INIT-100));
					EEPROM_MultiWrite(EEPROM_EXPOSURE_IRIS_OFFSET, (PBYTE)&buff[0], sizeof(buff));
					#endif
				}
			} else {
				if(initIRISCntStart > 30*20) {
					AECTRL_SetAE_AutoMode();
					initIRIS = TRUE;
					initIRISCntStart = 0;
					offsetIRISInit = (OFFSET_IRIS_INIT-100);
				}
				initIRISCntEn = 0;
			}
		}
		OSD_StrDispOnOff(OFF);
		OSD_MessageBox(ON, diffs);
	}

#if defined(__USE_PWM_ADC__)
	if(startInitADC == TRUE) {
		initIRISCntStart++;
		if(initIRISCntStart > irisPF) {
			adcIRIS = getIRISPosition();
			initIRISCntStart = 0;
			startInitADC = FALSE;

			sprintf(diffs,"initADC:%d",adcIRIS);
			UARTprintf("\r\n -------init ADC :%d ... \r\n",adcIRIS);
			#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
			BYTE buff[2];
			buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)(adcIRIS-5);
			EEPROM_MultiWrite(EEPROM_EXPOSURE_IRIS_ADC, (PBYTE)&buff[0], sizeof(buff));
			#endif
			OSD_StrDispOnOff(OFF);
		}
	}
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_IRISAutoMode(SHORT mode,PAE_CTRL_PARAM pCTRL) // 0: auto depends on gain, 1:auto
{
	LONG error,PWM;

#if defined(__USE_PWM_ADC__)
	if((initIRIS==FALSE) || (startInitADC == TRUE)) {
#else
	if(initIRIS==FALSE) {
#endif
		DCLENS_IRIS_Init(pCTRL);
		return;
	}

	if(GetAELensMode() == 2) {
		AECTRL_IRISPWM_SetFullOpen();
		gStopAgcRun = FALSE;
		return;
	}

	// frame counter
	if(frmCnt >= IFRMMAX) {
		frmCnt = 0;
	} else {
		frmCnt ++;
	}

	//UARTprintf("%5d - %5d\r\n",defaultOffsetIRIS,offsetIRIS);
	if(defaultOffsetIRIS == 0) {
		defaultOffsetIRIS = offsetIRIS;
	} else {
		offsetIRIS = MAX(defaultOffsetIRIS-32,MIN(defaultOffsetIRIS+64, offsetIRIS));
	}

	// calculate PWM IRIS offset by frame rate
	WORD target = GetSS_SensorVoutFps();
	if(target != offsetIRISfrmRate) {
		offsetIRIS = offsetIRIS*offsetIRISfrmRate/target;
		offsetIRISfrmRate = target;
	}

	// check AE mode
	WORD blcdiv;
	BOOL isGainCtlMod;
	BOOL isShutCtlMod;

	if ((pCTRL->ctrl&AECTRL_BLC_MASK) == AECTRL_BLC_MODE_BLC) blcdiv = 2;
	else blcdiv = 1;

	if((pCTRL->ctrl & AECTRL_PRIORITY_MASK) == AECTRL_PRIORITY_GAIN) isGainCtlMod = TRUE;
	else isGainCtlMod = FALSE;

	if((pCTRL->ctrl & AECTRL_PRIORITY_MASK) == AECTRL_PRIORITY_SHUTTER) isShutCtlMod = TRUE;
	else isShutCtlMod = FALSE;

	if(pCTRL->Ydiff > 0) {
		if(pCTRL->Ydiff > 1*(pCTRL->Yref)/blcdiv) {
			error = 1*pCTRL->Yref/blcdiv;
		} else {
			//error = (pCTRL->Ydiff*pCTRL->Ydiff) / (1*pCTRL->Yref/blcdiv);
			error = pCTRL->Ydiff/blcdiv;
		}
	} else {
		//if(pCTRL->Ydiff > 1*(pCTRL->Yref)/(blcdiv*2)) {
		//	error = 2*pCTRL->Yref/blcdiv;
		//} else {
			error = 1*pCTRL->Ydiff/blcdiv;
		//}
	}

	//UARTprintf("error#1:%5d /",error);

	#define IRISCLOSESPD (-32)

	if(isGainCtlMod == FALSE) {
		if((AECTRL_IsGAINCtrlMin() == FALSE) || ((GetAELensMode() == 1) && (AECTRL_GetSHUTGain() < (AECTRL_GetSHUTOmax()-2)) && (isShutCtlMod == FALSE))) {
		#if 1
			if(AECTRL_GetGAINGain() > 512*256) { // 0x20000
				if(error > (IRISCLOSESPD*16*irisPF))
					error = IRISCLOSESPD*16*irisPF;
			} else if(AECTRL_GetGAINGain() > 512*32) {
				if(error > (IRISCLOSESPD*12*irisPF))
					error = IRISCLOSESPD*12*irisPF;
			} else if(AECTRL_GetGAINGain() > 512*16) {
				if(error > (IRISCLOSESPD*10*irisPF))
					error = IRISCLOSESPD*10*irisPF;
			} else if(AECTRL_GetGAINGain() > 512*8) {
				if(error > (IRISCLOSESPD*8*irisPF))
					error = IRISCLOSESPD*8*irisPF;
			} else if(AECTRL_GetGAINGain() > 512*4) {
				if(error > (IRISCLOSESPD*6*irisPF))
					error = IRISCLOSESPD*6*irisPF;
			} else if(AECTRL_GetGAINGain() > 512*2) {
				if(error > (IRISCLOSESPD*4*irisPF))
					error = IRISCLOSESPD*4*irisPF;
			} else if(AECTRL_GetGAINGain() > 512*1) {
				if(error > (IRISCLOSESPD*2*irisPF))
					error = IRISCLOSESPD*2*irisPF;
			} else {
				if(error > (IRISCLOSESPD*irisPF))
					error = IRISCLOSESPD*irisPF;
			}
		#else
			if(error > (-32*irisPF))
					error = -32*irisPF;
		#endif
		}
	} else {
		if(AECTRL_GetGAINGain() < 0x20000)		error = error/2;
		else if(AECTRL_GetGAINGain() < 0x28000)	error = error/4;
		else if(AECTRL_GetGAINGain() < 0x30000)	error = error/5;
		else if(AECTRL_GetGAINGain() < 0x38000)	error = error/6;
		else if(AECTRL_GetGAINGain() < 0x3c000)	error = error/7;
		else		error = error/8;
	}

	PWM = error/(irisPF*blcdiv) + offsetIRIS;

	if(PWM > IMAX)		PWM = IMAX;
	else if(PWM < IMIN)	PWM = IMIN;

	// check whethere frame avg is constant
	LONG frmDIFFPWM = DIFF(PWM, bakFrmPWM);
	LONG frmDIFFYsum = DIFF(pCTRL->Ysum, bakFrmYsum);
	LONG frmDiffYsum = pCTRL->Ysum - bakFrmYsum;
	if(frmDIFFYsum < FRM_DIF_YSUM_TH) {
		if(pCTRL->YDIFF > pCTRL->band*3) {
			frmDIFFCnt++;
		}
	} else {
		frmDIFFCnt = 0;
	}

	// check whether increase(decrease) image
	if(frmDIFFYsum > 3) {
		if(bakFrmYsum > pCTRL->Ysum)	frmIDCnt--;
		else								frmIDCnt++;
	}

	if((AECTRL_IsGAINCtrlMax() == FALSE)&&(AECTRL_IsSensUpMin() == TRUE)&&
		(//((isGainCtlMod == TRUE) && (((offsetIRIS < PWM)&&(error > 0)) || ((offsetIRIS > PWM)&&(error < 0))) && ((GetAELensMode() == 0) || ((GetAELensMode() == 1) && (AECTRL_GetSHUTGain() > (AECTRL_GetSHUTOmax()-2)))))||
		((isGainCtlMod == FALSE) && (AECTRL_IsGAINCtrlMin() == TRUE) && ((GetAELensMode() == 0) || ((AECTRL_GetSHUTGain() > (AECTRL_GetSHUTOmax()-2)) || (isShutCtlMod == TRUE)))))) {
		if((gStopAgcRun == TRUE)&&(frmCnt == 0)&&(ABS(frmIDCnt) <= IFRMTH)&&(frmDIFFCnt > IFRMDIFTH)&&(frmDIFFPWM == 0)) {
			if((pCTRL->YDIFF > pCTRL->band*3)&&(pCTRL->YDIFF < pCTRL->band*16)) {
				if(offsetIRIS > PWM) {
					if(AECTRL_GetGAINMax() > 256) {
						if(offsetIRIS > 0) {
							offsetIRIS--;
						}
					}
				} else if (offsetIRIS < PWM) {
					if(offsetIRIS < IMAX/2) {
						offsetIRIS++;
					}
				}
			}
		}
	}

	if(frmCnt == 0) {
		frmDIFFCnt = 0;
		frmIDCnt = 0;
	}

#if defined(__USE_PWM_ADC__)
	if(getIRISPosition() < adcIRIS)
		gStopAgcRun = TRUE;
	else
		gStopAgcRun = FALSE;
	
#else
	LONG i;
	if((pCTRL->Ydiff > pCTRL->band*1) || (pCTRL->Ydiff < -pCTRL->band*3))		i = integral + error/(3*irisPF*blcdiv);
	else							i = integral;

	//UARTprintf("(%5d - %5d) / ",error/(irisPF*blcdiv), i);

	if(i > IMAX)		integral = IMAX;
	else if(i < IMIN)	integral = IMIN;
	else				integral = i;

	if((isGainCtlMod == TRUE) || (integral > IMIN)) {
		gStopAgcRun = TRUE;
	} else {
		gStopAgcRun = FALSE;
	}
#endif

	//if(frmCnt == 0)
	//	UARTprintf("(%1d / Ydiff:%6d(%6d)(%6d) / offsetIRIS:%4d(%4d) / integral:%4d / AGC gain:%4d\r\n",(int)gStopAgcRun,(int)pCTRL->Ydiff,(int)error,(int)frmDiffYsum,(int)offsetIRIS,(int)PWM,(int)integral,(int)AECTRL_GetGAINGain());

	//UARTprintf("%1d---error:%5d(%5d)(%5d) / offsetIRIS:%4d(%4d) / integral:%4d / AGC gain:%4d\r\n",gStopAgcRun,(int)pCTRL->Ydiff,(int)error,(int)frmDiffYsum,(int)offsetIRIS,(int)PWM,(int)integral,(int)AECTRL_GetGAINGain());
	//UARTprintf("%1d---error:%5d(%5d)(%5d) / offsetIRIS:%4d / PWM:%4d / integral:%4d / AGC gain:%4d / band:%4d\r\n",gStopAgcRun,(int)pCTRL->Ydiff,(int)error,(int)frmDiffYsum,(int)offsetIRIS,(int)PWM,(int)integral,(int)AECTRL_GetGAINGain(),pCTRL->band);

	//UARTprintf("AGC stop:%d - ",gStopAgcRun);
	//UARTprintf("frmCnt:%2d / frmIDCnt:%2d / band:%3d / AECTRL_GetSHUTOmax:%5d / AECTRL_GetSHUTGain:%5d / ",frmCnt,frmIDCnt,pCTRL->band,AECTRL_GetSHUTOmax(),AECTRL_GetSHUTGain());
	//UARTprintf("pCTRL->Ydiff:%5d / frmDIFFYsum:%5d / p:%5d / integral:%6d / offsetIRIS:%5d / PWM:%5d --- gain:%5x(%5d)\r\n",pCTRL->Ydiff,frmDIFFYsum,p/irisPF,integral,offsetIRIS,PWM,AECTRL_GetGAINGain(),(AECTRL_GetGAINGain() - bakFrmGain));

	//OSD_StrDispOnOff(OFF);
	//char diffs[50];
	//sprintf(diffs,"%5d:%4d:%4d:%4d", (int)pCTRL->Ydiff,(int)offsetIRIS,(int)PWM,(int)AECTRL_GetGAINGain());
	//OSD_MessageBox(ON, diffs);

	//UARTprintf("ADC : %d(%d)\r\n",	getIRISPosition(),adcIRIS);

	bakFrmGain = AECTRL_GetGAINGain();
	bakFrmYsum = pCTRL->Ysum;
	bakFrmPWM = PWM;

	MISP_RegWrite(MISP_LOCAL_ID, 0x67E, PWM);
	//MISP_RegWrite(MISP_LOCAL_ID, 0x67F, 0);
}


//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_IRIS_SetOffset(WORD offset)
{
	offsetIRIS = offset + (OFFSET_IRIS_INIT-100);
}

//--------------------------------------------------------------------------------------------------------------------------
WORD DCLENS_IRIS_GetOffset(void)
{
	return offsetIRIS;
}

#if defined(__USE_PWM_ADC__)
//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_IRIS_SetADC(WORD val)
{
	adcIRIS = val;
}

//--------------------------------------------------------------------------------------------------------------------------
WORD DCLENS_IRIS_GetADC(void)
{
	return adcIRIS;
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_IRIS_Tune(void) // start IRIS tune
{
	initIRIS = FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_LensMode(BYTE val)
{
	if(val == 0) {
		AECTRL_SetSHUTDefaultMax();
		AECTRL_SetSHUTGain(0);
	} else if(val == 1) {
		AECTRL_SetSHUTOmax(2064); // 1/500 sec
	} else {
		AECTRL_SetSHUTDefaultMax();
		AECTRL_SetSHUTGain(0);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void DCLENS_IrisSpeed(BYTE val)
{
	switch(val) {
		case 0 : irisPF = 150; break;
		case 1 : irisPF = 130; break;
		case 2 : irisPF = 100; break;
		case 3 : irisPF = 80; break;
		case 4 : irisPF = 50; break;
		default : irisPF = 30; break;
	}
}

BYTE DCLENS_getIrisSpeed()
{
	return irisPF;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE aeXID,aeMID=0;
static void DCLENS_ModeHandler(PAE_CTRL_PARAM pCTRL)
{
	pCTRL->ctrl &= (~AECTRL_PROC_MASK);

	pCTRL->ctrl &= (~AECTRL_GAINCTL_MANU); // gain auto
	pCTRL->ctrl &= (~AECTRL_SHUTCTL_MANU); // shut auto

	if((pCTRL->ctrl & AECTRL_PRIORITY_MASK) == AECTRL_PRIORITY_AUTO) {
		DCLENS_IRISAutoMode(0,pCTRL);
		if (stHALL.Ysum > (pCTRL->Yref+pCTRL->band*1)) {
			if(AECTRL_GetSensUpVal() > AECTRL_GetSensUpMin()) {
				pCTRL->ctrl |= AECTRL_PROC_SENS; aeXID = 0x00;
			} else if (AECTRL_IsGAINCtrlMin()!=TRUE) {
				pCTRL->ctrl |= AECTRL_PROC_GAIN; aeXID = 0x01;
			} else if ((AECTRL_IsSHUTCtrlGmin() != TRUE) && (GetAELensMode() != 0)){
				pCTRL->ctrl |= AECTRL_PROC_SHUT; aeXID = 0x02;
			}
		} else if (stHALL.Ysum < (pCTRL->Yref-pCTRL->band*1)){
			if((AECTRL_IsSHUTCtrlGmax() != TRUE) && (GetAELensMode() != 0)) {
				pCTRL->ctrl |= AECTRL_PROC_SHUT; aeXID = 0x10;
			} else if(AECTRL_IsGAINCtrlMax()!=TRUE){
				pCTRL->ctrl |= AECTRL_PROC_GAIN; aeXID = 0x11;
			} else {
				if(AECTRL_GetSensUpMax() != AECTRL_GetSensUpVal()) {
					pCTRL->ctrl |= AECTRL_PROC_SENS; aeXID = 0x12;
				}
			}
		}
	} else if((pCTRL->ctrl & AECTRL_PRIORITY_MASK) == AECTRL_PRIORITY_SHUTTER) {
		AECTRL_SetSHUTDefaultMax();
		DCLENS_IRISAutoMode(0,pCTRL);
		pCTRL->ctrl &= (~AECTRL_GAINCTL_MANU); // gain auto
		pCTRL->ctrl |= AECTRL_SHUTCTL_MANU; // shut manual

		if (stHALL.Ysum > (pCTRL->Yref+pCTRL->band*1)) {
			pCTRL->ctrl |= AECTRL_PROC_GAIN;
		} else if (stHALL.Ysum < (pCTRL->Yref-pCTRL->band*1)) {
			pCTRL->ctrl |= AECTRL_PROC_GAIN;
		}
	} else { // AECTRL_PRIORITY_GAIN
		DCLENS_IRISAutoMode(0,pCTRL);
		pCTRL->ctrl |= AECTRL_GAINCTL_MANU; // gain manual
		pCTRL->ctrl &= (~AECTRL_SHUTCTL_MANU); // shut auto

		if (stHALL.Ysum > (pCTRL->Yref+pCTRL->band*1)) {
			if(AECTRL_GetSensUpVal() > AECTRL_GetSensUpMin()) {
				pCTRL->ctrl |= AECTRL_PROC_SENS;
			} else if((AECTRL_IsSHUTCtrlGmin() != TRUE) && (GetAELensMode() != 0)) {
				pCTRL->ctrl |= AECTRL_PROC_SHUT;
			}
		} else if (stHALL.Ysum < (pCTRL->Yref-pCTRL->band*1)){
			if((AECTRL_IsSHUTCtrlGmax()!=TRUE) && (GetAELensMode() != 0)) {
				pCTRL->ctrl |= AECTRL_PROC_SHUT;
			} else {
				if(AECTRL_GetSensUpMax() != AECTRL_GetSensUpVal()) {
					pCTRL->ctrl |= AECTRL_PROC_SENS;
				}
			}
		}
	}

#if __MISP100_AE_DBG__ == 1
	if (ODM_GetDEBUGMode()!=0x01 || pCTRL==NULL) return;
	UARTprintf("[AE] stHALL.omax : %d omin : %d gain : %d  / gaingain : %d[%d~%d] \r\n", stHALL.omax, stHALL.omin, stHALL.gain, AECTRL_GetGAINGain(), AECTRL_GetGAINMin(), AECTRL_GetGAINMax());
	UARTprintf("[AE] xID : %2x  mID : %2x\r\n", aeXID, aeMID);
#endif
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
#endif

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_IRISPWM_SetFullOpen(void)
{
	//WORD hsync, hmin;

	MISP_RegWrite(MISP_LOCAL_ID, 0x67E, 0); //min. register PWM
	MISP_RegWrite(MISP_LOCAL_ID, 0x67F, 0); //mul. register PWM
}

//--------------------------------------------------------------------------------------------------------------------------
void AECTRL_IRISPWM_SetFullClose(void)
{
	MISP_RegWrite(MISP_LOCAL_ID, 0x67E, 4095); //min. register PWM
	//MISP_RegWrite(MISP_LOCAL_ID, 0x67F, 4095); //mul. register
}

/*  FILE_END _HERE */
