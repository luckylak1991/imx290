//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	MISPADC.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"misp100.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
#define ADC_FLAG_OFF  0x00
#define ADC_FLAG_RUN  0x01
#define ADC_FLAG_GET  0x02

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static ADC_CTRL_PARAM stADCCTRL={0,};
static VBYTE fSysReqMsgADC=ADC_FLAG_OFF;
static WORD adc_vin[ADC_SRC_NUM]={0, 0, 0, 0};

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
#define F_MADC_ADC_OFF(a)  ((a)<<26)  // 1bit : turn off controller (use ADC IP)
#define F_MADC_PD_WAIT(a)  ((a)<<21)  // 5bit : wait clock after NCE 1~31 (reg_val+1)
#define F_MADC_SOC_LEN(a)  ((a)<<16)  // 5bit : SOC width 1~31 (reg_val+1)
#define F_MADC_CLK_DIV(a)  ((a)<< 6)  //10bit : clock divider 1/1 ~ 1/1023
#define F_MADC_MODE_EN(a)  ((a)<< 4)  // 2bit : Mode - disable("0x") / 1-time("10") / N-time mode("11")
#define F_MADC_CHN_SEL(a)  ((a)<< 1)  // 3bit : ADC Channel Selection
#define F_MADC_PW_DOWN(a)  ((a)<< 0)  // 1bit : power-down control
	
// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------
static void stAdc_AdjustCtrl(PADC_CTRL_PARAM pCTRL) {
	*R_MADCCTRL = (
		F_MADC_ADC_OFF(pCTRL->adc_off)|   // 1bit : turn off controller (use ADC IP)
		F_MADC_PD_WAIT(pCTRL->pd_wait)|   // 5bit : wait clock after NCE 1~31 (reg_val+1)
		F_MADC_SOC_LEN(pCTRL->soc_len)|   // 5bit : SOC width 1~31 (reg_val+1)
		F_MADC_CLK_DIV(pCTRL->clk_div)|   //10bit : clock divider 1/1 ~ 1/1023
		F_MADC_MODE_EN(pCTRL->mode_en)|   // 2bit : Mode - disable("0x") / 1-time("10") / N-time mode("11")
		F_MADC_CHN_SEL(pCTRL->chn_sel)|   // 3bit : ADC Channel Selection
		F_MADC_PW_DOWN(pCTRL->pw_down));  // 1bit : power-down control
}

// ----------------------------------------------------------------------
static void Init_ADC_PARAM(PADC_CTRL_PARAM pCTRL) {
	pCTRL->init_ok=1;   // init done
#if defined(__USE_SMIC_ADC__)
	pCTRL->adc_off=0;   // use MIT ADC
#else
	pCTRL->adc_off=1;   // use ADC ADC
#endif
	pCTRL->pd_wait=0;   // wait after NCE : 1clk
	pCTRL->soc_len=0;   // SOC length : 1clk
	pCTRL->clk_div=22;  // ADC clock : 49.5M/22 (recommemd 2.2M)
	pCTRL->mode_en=0;   // turn off
#if defined(__USE_SMIC_ADC__)
	pCTRL->chn_sel=2;   // channel - 0:Day&Night, 2:KeyButton
#else
	pCTRL->chn_sel=5;   // channel - 0:Day&Night, 5:KeyButton
#endif
	pCTRL->pw_down=0;   // hold NCE as '0'
}

// ----------------------------------------------------------------------
static void Init_ADC_REGISTER(PADC_CTRL_PARAM pCTRL) {
	stAdc_AdjustCtrl(pCTRL);
}

// ----------------------------------------------------------------------
void Init_ADC(void) {
	//init MIT controller (default:off)
	memset(&stADCCTRL, 0, sizeof(ADC_CTRL_PARAM));
	Init_ADC_PARAM(&stADCCTRL);
	Init_ADC_REGISTER(&stADCCTRL);

	//init ADC controller (default:off)
	*R_ADCCON = F_ADC_OFF;
	*R_ADCSAM = F_ADCCLK_SEL_128 | F_STC_15W;  // adc clk : pclk/64(100Khz~20Mhz), STCHP : 15clk width, EOC:11clk(105Khz*11=104us)
	*R_ADCCH  = F_ADCCH0;   // adc channel 0
}


//--------------------------------------------------------------------------------------------------------------------------
// issue ADC request
void ISPM ADC_CtrlConvRun(void) {
	fSysReqMsgADC|=ADC_FLAG_RUN;
}


//--------------------------------------------------------------------------------------------------------------------------
// fetch ADC result
void ISPM ADC_CtrlConvGet(void) {
	fSysReqMsgADC|=ADC_FLAG_GET;
}

//--------------------------------------------------------------------------------------------------------------------------
// return ADC result
WORD ISPM GetADC_ConvData(ADC_SRC_t inp) {
	return adc_vin[inp];
}


// ----------------------------------------------------------------------
// request 1-time mode ADC (toggle channel 0, 5)
static void stADC_RequestConv(void) {
#if defined(__USE_SMIC_ADC__)
	if(GetAdc_Channel()==6)  SetAdc_Channel(0);
	else                     SetAdc_Channel(GetAdc_Channel()+2);	// 0->2->4->6->0,,,
	SetAdc_AdcMode(ADC_MODE_OFF);
	SetAdc_AdcMode(ADC_MODE_1TIME);
#else
	if(*R_ADCCH==F_ADCCH6)  *R_ADCCH=F_ADCCH0;
	else                    *R_ADCCH *= F_ADCCH2;
	*R_ADCCON = F_ADC_STC|F_ADC_ON;  // 1-time mode
#endif
}


// ----------------------------------------------------------------------
// fetch ADC result
static void stADC_FetchResult(void) {
#if defined(__USE_SMIC_ADC__)
	WORD adc_read = *R_MADCDATA;
	WORD chan =(adc_read>>10) & 0x0007;  //channel 0~7
	WORD data =(adc_read>> 0) & 0x03FF;  //data 10bit
	switch(chan)	{
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)		// i540, i550
		case 0:  adc_vin[ADC_SRC_VIN0]=data;  break;  // adc_vin0 -- day&night
		case 2:  adc_vin[ADC_SRC_VIN1]=data;  break;  // adc_vin1 -- key
		case 4:  adc_vin[ADC_SRC_VIN2]=data;  break;  // adc_vin2
		case 6:  adc_vin[ADC_SRC_VIN3]=data;  break;  // adc_vin3
#else																// i510
		case 2:  adc_vin[ADC_SRC_VIN1]=data;  break;  // adc_vin1 -- key
		case 6:  adc_vin[ADC_SRC_VIN0]=data;  break;  // adc_vin0 -- day&night
#endif
		default:                             break;
	}
#else
	WORD data = *R_ADCDATA & 0x3FF;
	switch(*R_ADCCH & 0x1FFFF)	{
		case F_ADCCH0:  adc_vin[ADC_SRC_VIN0]=data;  break;  // adc_vin0 -- day&night
		case F_ADCCH2:  adc_vin[ADC_SRC_VIN1]=data;  break;  // adc_vin1 -- key
		case F_ADCCH4:  adc_vin[ADC_SRC_VIN2]=data;  break;  // adc_vin2
		case F_ADCCH6:  adc_vin[ADC_SRC_VIN3]=data;  break;  // adc_vin3
		default:                                    break;
	}
#endif

	//clear mode register for 1-time mode processing
#if defined(__USE_SMIC_ADC__)
	if(GetAdc_AdcMode()==ADC_MODE_1TIME)
		SetAdc_AdcMode(ADC_MODE_OFF);
#else
//	if(!(*R_ADCCON&F_ADC_NSTC)) 		// disabled to reduce adc power-on noise
//		*R_ADCCON=F_ADC_OFF;
#endif
}


// ----------------------------------------------------------------------
void ADC_ProcessHandler(void) {
	if(stADCCTRL.init_ok==0)  return;  //not initialized

	//request 1-time mode ADC (toggle channel 0, 5)
	if(fSysReqMsgADC&ADC_FLAG_RUN) {
		fSysReqMsgADC&=~ADC_FLAG_RUN;
		stADC_RequestConv();
	}

	//fetch data converted
	if(fSysReqMsgADC&ADC_FLAG_GET) {
		fSysReqMsgADC&=~ADC_FLAG_GET;
		stADC_FetchResult();
	}
}


#if defined(__USE_SMIC_ADC__)
// ----------------------------------------------------------------------
// turn off controller (use ADC IP)
void SetAdc_SelectIp(ADC_IP_t ip) {
	stADCCTRL.adc_off=(BYTE)ip;
	stAdc_AdjustCtrl(&stADCCTRL);
}

// ----------------------------------------------------------------------
// wait clock after NCE 1~31 (reg_val+1)
void SetAdc_PowerWait(BYTE nID) {
	stADCCTRL.pd_wait=nID;
	stAdc_AdjustCtrl(&stADCCTRL);
}

// ----------------------------------------------------------------------
// SOC width 1~31 (reg_val+1)
void SetAdc_SocLength(BYTE nID) {
	stADCCTRL.soc_len=nID;
	stAdc_AdjustCtrl(&stADCCTRL);
}

// ----------------------------------------------------------------------
// clock divider 1/1 ~ 1/1023
void SetAdc_ClockDiv(WORD nID) {
	stADCCTRL.clk_div=nID;
	stAdc_AdjustCtrl(&stADCCTRL);
}

// ----------------------------------------------------------------------
// Mode - disable("0x") / 1-time("10") / N-time mode("11")
void SetAdc_AdcMode(ADC_MODE_t Mode) {
	stADCCTRL.mode_en=(BYTE)Mode;
	stAdc_AdjustCtrl(&stADCCTRL);
}

BYTE GetAdc_AdcMode(void) {
	return stADCCTRL.mode_en;
}

// ----------------------------------------------------------------------
// ADC Channel Selection 0~7
void SetAdc_Channel(BYTE nID) {
	stADCCTRL.chn_sel=nID;
	stAdc_AdjustCtrl(&stADCCTRL);
}

BYTE GetAdc_Channel(void) {
	return stADCCTRL.chn_sel;
}

// ----------------------------------------------------------------------
// power-down control
void SetAdc_PowerCtrl(BYTE En) {
	stADCCTRL.pw_down=En;
	stAdc_AdjustCtrl(&stADCCTRL);
}

#endif //#if defined(__USE_SMIC_ADC__)


/*  FILE_END_HERE */

