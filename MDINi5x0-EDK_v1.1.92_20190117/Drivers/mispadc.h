//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MISPADC.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISPADC_H__
#define		__MISPADC_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
#define	__USE_SMIC_ADC__  //[SMIC. S55NLLAD2D] + [MIT Controller]
#else 									// MDIN-i500
//#define	(__EMPTY__)     //[Fujitsu. AM4ASAE] + [ADC Controller]
#endif


typedef	struct {
	BYTE init_ok;   // to notify configure init
	BYTE adc_off;   // 1bit : turn off controller (use ADC IP)
	BYTE pd_wait;   // 5bit : wait clock after NCE 1~31 (reg_val+1)
	BYTE soc_len;   // 5bit : SOC width 1~31 (reg_val+1)
	WORD clk_div;   //10bit : clock divider 1/1 ~ 1/1023
	BYTE mode_en;   // 2bit : Mode - disable("0x") / 1-time("10") / N-time mode("11")
	BYTE chn_sel;   // 3bit : ADC Channel Selection
	BYTE pw_down;   // 1bit : power-down control
	BYTE dummy_1;   // dummy to align 32bit pointer
} ADC_CTRL_PARAM, *PADC_CTRL_PARAM;

typedef	enum {  //ADC processing mode
	ADC_MODE_OFF=0,    //ADC off
	ADC_MODE_1TIME=2,  //One-time
	ADC_MODE_nTIME=3   //#N-time
} ADC_MODE_t;

typedef	enum {  //Select Controller
	ADC_IP_ADC=0, //ADC controller
	ADC_IP_MIT    //MIT controller
} ADC_IP_t;

typedef	enum {  //ADC source
	ADC_SRC_VIN0=0,  // adc_vin0 -- day&night
	ADC_SRC_VIN1,    // adc_vin1 -- key
	ADC_SRC_VIN2,    // adc_vin2
	ADC_SRC_VIN3,    // adc_vin3
	ADC_SRC_NUM
} ADC_SRC_t;

//MIT ADC controller
#define MADC_BASE_ADDR	0x80012000  //USER APB00
#define R_MADCCTRL	((volatile unsigned int*  )(MADC_BASE_ADDR+0x0))
#define R_MADCDATA	((volatile unsigned short*)(MADC_BASE_ADDR+0x4))  // [15] data ready,  [12:10] channel,  [9:0] data

// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void Init_ADC(void);
void ISPM ADC_CtrlConvRun(void);
void ISPM ADC_CtrlConvGet(void);
WORD ISPM GetADC_ConvData(ADC_SRC_t inp);
void ADC_ProcessHandler(void);

#if defined(__USE_SMIC_ADC__)
void SetAdc_SelectIp(ADC_IP_t ip);
void SetAdc_PowerWait(BYTE nID);
void SetAdc_SocLength(BYTE nID);
void SetAdc_ClockDiv(WORD nID);
void SetAdc_AdcMode(ADC_MODE_t Mode);
BYTE GetAdc_AdcMode(void);
void SetAdc_Channel(BYTE nID);
BYTE GetAdc_Channel(void);
void SetAdc_PowerCtrl(BYTE En);
#endif

#endif	/* __MISPADC_H__ */

