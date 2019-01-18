//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2018  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  IT680X.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:


#if defined(__USE_IT6802_RX__)


#ifndef		__IT680X_H__
#define		__IT680X_H__

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

// define for xrc
#define IT6802A0_ADR		0x94	//Hardware Fixed I2C address of IT6802 HDMI				// it680x_global.h
#define IT6802B0_ADR		0x90	//Hardware Fixed I2C address of IT6802 HDMI				// it680x_global.h
#define IT6802MHL_ADR		0xE0	//Software programmable I2C address of IT6802 MHL		// it680x_global.h
#define IT6802EDID_ADR		0xA8	//Software programmable I2C address of IT6802 EDID RAM	// it680x_global.h
//#define IT6802CEC_ADR		0xC8	//Software programmable I2C address of IT6802 CEC		// it680x_global.h

#define IT6802_MultiRead(sAddr, rAddr, pBuff, bytes)	I2C_Read(TWI_CH1, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes)
#define IT6802_MultiWrite(sAddr, rAddr, pBuff, bytes)	I2C_Write(TWI_CH1, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes)

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
extern void it6802HPDCtrl(unsigned char ucport,unsigned char ucEnable);	// Mhlrx.h
extern void IT6802_fsm_init(void);										// Mhlrx.h
extern void IT6802_fsm(void);											// Mhlrx.h

void ISPM SetIT680x_Tick(void);
void IT6802_InputMonitor(void);


#endif	/* __IT680X_H__ */


#endif	// __USE_IT6802_RX__

