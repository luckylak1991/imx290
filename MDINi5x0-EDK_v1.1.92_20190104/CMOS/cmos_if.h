//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  CMOS_IF.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__CMOS_IF_H__
#define		__CMOS_IF_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__MISPTYPE_H__
#include	"misptype.h"
#endif
#ifndef		__MT9M131_H__
#include	"mt9m131.h"
#endif
#ifndef		__MN34041_H__
#include	"mn34041.h"
#endif
#ifndef		__MN34220_H__
#include	"mn34220.h"
#endif
#ifndef		__MN34420_H__
#include	"mn34420.h"
#endif
#ifndef		__IMX076_H__
#include	"imx076.h"
#endif
#ifndef		__IMX124_H__
#include	"imx124.h"
#endif
#ifndef		__IMX136_H__
#include	"imx136.h"
#endif
#ifndef		__IMX174_H__
#include	"imx174.h"
#endif
#ifndef		__IMX178_H__
#include	"imx178.h"
#endif
#ifndef		__IMX185_H__
#include	"imx185.h"
#endif
#ifndef		__IMX222_H__
#include	"imx222.h"
#endif
#ifndef		__IMX226_H__
#include	"imx226.h"
#endif
#ifndef		__IMX238_H__
#include	"imx238.h"
#endif
#ifndef		__IMX253_H__
#include	"imx253.h"
#endif
#ifndef		__IMX273_H__
#include	"imx273.h"
#endif
#ifndef		__IMX265_H__
#include	"imx252.h"
#endif
#ifndef		__IMX265_H__
#include	"imx265.h"
#endif
#ifndef		__IMX274_H__
#include	"imx274.h"
#endif
#ifndef		__IMX326_H__
#include	"imx326.h"
#endif
#ifndef		__IMX290_H__
#include	"imx290.h"
#endif
#ifndef		__IMX385_H__
#include	"imx385.h"
#endif
#ifndef		__IMX334_H__
#include	"imx334.h"
#endif
#ifndef		__AR0331_H__
#include	"ar0331.h"
#endif
#ifndef		__AR0134_H__
#include	"ar0134.h"
#endif
#ifndef		__PO3100K_H__
#include	"po3100k.h"
#endif
#ifndef		__PS1210K_H__
#include	"ps1210k.h"
#endif
#ifndef		__PS1400K_H__
#include	"ps1400k.h"
#endif
//#ifndef		__CMOSEXT2_H__
//#include	"CMOSEXT2.h"
//#endif
#ifndef		__OV2710_H__
#include	"ov2710.h"
#endif
#ifndef		__MT9P031_H__
#include	"mt9p031.h"
#endif
//#ifndef		__EXT3_H__
//#include	"ext3.h"
//#endif
// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
#define		I2C_MT9M131_ADDR		0x90
//-------------------------------------------------
#define		I2C_MT9P031_ADDR		0xBA
//-------------------------------------------------
#define		SPI_MN34041_ADDR		0x92
//-------------------------------------------------
#define		SPI_MN34220_ADDR		0x98
//-------------------------------------------------
#define		SPI_MN34420_ADDR		0x9A
//-------------------------------------------------
#define		SPI_IMX124_ADDR			0x2A		//sony spi raddr word style(in i500) = (chip id)&(byte addr). read addr is 0x2B(xrc)
//-------------------------------------------------
#define		SPI_IMX174_ADDR			0x2C			//sony spi raddr word style(in i500) = (chip id)&(byte addr). read addr is 0x2D(xrc)
//-------------------------------------------------
#define		SPI_IMX178_ADDR			0x24			//sony spi raddr word style(in i500) = (chip id)&(byte addr). read addr is 0x25(xrc), FPGA b/d only
//-------------------------------------------------
#define		SPI_IMX185_ADDR			0x2E			//sony spi raddr word style(in i500) = (chip id)&(byte addr). read addr is 0x2f(xrc)
//-------------------------------------------------
#define		SPI_IMX226_ADDR			0x3C			//sony spi raddr word style(in i500) = (chip id)&(byte addr). read addr is 0x2D(xrc)
//-------------------------------------------------
#define		SPI_IMX274_ADDR			0x18			//sony spi raddr word style(in i500) = (chip id)&(byte addr). read addr is 0x2D(xrc)
//-------------------------------------------------
#define		SPI_IMX326_ADDR			0x18			//sony spi raddr word style(in i500) = (chip id)&(byte addr). read addr is 0x2D(xrc)
//-------------------------------------------------
#define		SPI_IMX222_ADDR			0x22
//-------------------------------------------------
#define		SPI_IMX238_ADDR			0x26	
//-------------------------------------------------
#define		SPI_IMX290_ADDR			0x28			//sony spi raddr word style(in i500) = (chip id)&(byte addr). read addr is 0x29(xrc)
//-------------------------------------------------
#define		SPI_IMX265_ADDR			0x32		
//-------------------------------------------------
#define		SPI_IMX252_ADDR			0x38	
//-------------------------------------------------
#define		SPI_IMX253_ADDR			0x36		
//-------------------------------------------------
#define		SPI_IMX273_ADDR			0x40
//-------------------------------------------------
#define		SPI_IMX385_ADDR			0x3E			
//-------------------------------------------------
#define		SPI_IMX136_ADDR			0x3A		//sony spi raddr word style(in i500) = (chip id)&(byte addr). read addr is 0x3B(xrc)
//-------------------------------------------------
#define		I2C_SONY_CMOS_ADDR		0x34
//-------------------------------------------------
#define		I2C_AR0134_ADDR			0x20		//real 0x20, 21
#define		I2C_AR0331_ADDR			0x30
//-------------------------------------------------
#define		I2C_PO3100K_ADDR		0x64
//#define		I2C_PO3100K_ADDR		0x60
//#define		I2C_PO3100K_ADDR		0x62
//#define		I2C_PO3100K_ADDR		0x66
//-------------------------------------------------
#define		I2C_PS1210K_ADDR		0xEE
#define		I2C_PS1400K_ADDR		0x6C			
//-------------------------------------------------
#define		I2C_EXT2_ADDR		0x6C
#define		I2C_EXT1_ADDR		0x6C
//-------------------------------------------------
#define		I2C_OV2710_ADDR			0x6C		
//-------------------------------------------------
#define		SPI_EXT3_ADDR			0x70	
//-------------------------------------------------
#define		SPI_IMX076_ADDR_1		0x02
#define		SPI_IMX076_ADDR_2		0x03
//-------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void CMOS_Initialize(void);
BYTE CMOS_SetGain(WORD val);
void CMOS_SetShutter(WORD val);
void CMOS_SHS2(WORD val);
void CMOS_SHS3(WORD val);
void CMOS_SHS4(WORD val);	
WORD CMOS_GetGain(void);
void CMOS_SetSensup(LONG val);

BYTE CMOS_MultiWrite(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);
BYTE CMOS_ByteWrite(BYTE sAddr, WORD rAddr, BYTE wData);
BYTE CMOS_WordWrite(BYTE sAddr, WORD rAddr, WORD wData);
BYTE CMOS_DWordWrite(BYTE sAddr, WORD rAddr, DWORD wData);
BYTE CMOS_MultiRead(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);
BYTE CMOS_ByteRead(BYTE sAddr, WORD rAddr);
WORD CMOS_WordRead(BYTE sAddr, WORD rAddr);
DWORD CMOS_DWordRead(BYTE sAddr, WORD rAddr);
BYTE CMOS_ByteField(BYTE sAddr, WORD rAddr, BYTE bPos, BYTE bCnt, BYTE bData);
BYTE CMOS_WordField(BYTE sAddr, WORD rAddr, WORD bPos, WORD bCnt, WORD bData);

void SetCMOSGainMax(SHORT dB, SHORT num);
void SetCMOSGainOpt_num(SHORT val);
SHORT GetCMOSGainMax_dB(void);
SHORT GetCMOSGainMax_num(void);
SHORT GetCMOSGainOpt_num(void);

BYTE ISPM IMX226_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes);


#endif	/* __CMOS_IF_H__ */
