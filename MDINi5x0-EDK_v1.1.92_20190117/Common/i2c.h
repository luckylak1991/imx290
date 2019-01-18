//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  I2C.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__UCOMI2C_H__
#define		__UCOMI2C_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__MISPTYPE_H__
#include	"misptype.h"
#endif

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
#define		I2C_OK					0
#define		I2C_NOT_FREE			1
#define		I2C_HOST_NACK			2
#define		I2C_TIME_OUT			3

#define		I2C_AT24C02_ADDR		0xA0
#define		I2C_TCA6408_ADDR		0x40
#define		I2C_CDCE913_ADDR		0xCA
#define		I2C_LCMX03L_ADDR		0x82
#define		I2C_LIAMD6K_ADDR		0x86
#define		I2C_PS8409_ADDR			0x16

#define		I2C_ADDR_8BIT			0
#define		I2C_ADDR_16BIT			1
#define		I2C_DATA_8BIT			0
#define		I2C_DATA_16BIT			1

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
BYTE I2C_Write(BYTE ch, BYTE sAddr, WORD rAddr, BOOL addr_16bit, PBYTE pBuff, BOOL data_16bit, WORD bytes);
BYTE I2C_Read(BYTE ch, BYTE sAddr, WORD rAddr, BOOL addr_16bit, PBYTE pBuff, BOOL data_16bit, WORD bytes);
void I2C_ByteWrite(BYTE ch, BYTE sAddr, BYTE rAddr, BYTE wData);
BYTE I2C_ByteRead(BYTE ch, BYTE sAddr, BYTE rAddr);
BYTE I2CGPIO_Write(BYTE sAddr, WORD rAddr, BOOL addr_16bit, PBYTE pBuff, BOOL data_16bit, WORD bytes);
BYTE I2CGPIO_Read(BYTE sAddr, WORD rAddr, BOOL addr_16bit, PBYTE pBuff, BOOL data_16bit, WORD bytes);

WORD EEPROM_RegRead(WORD rAddr);
void EEPROM_RegWrite(WORD rAddr, WORD wData);
void EEPROM_MultiWrite(WORD rAddr, PBYTE pBuff, WORD bytes);
void EEPROM_MultiRead(WORD rAddr, PBYTE pBuff, WORD bytes);
void IOExp_Write(BYTE nID, WORD rAddr, BYTE wData);
BYTE IOExp_Read(BYTE nID, WORD rAddr);
void IOExp_Field(BYTE nID, WORD rAddr, BYTE bPos, BYTE bCnt, BYTE bData);
BYTE CDCE913_RegRead(WORD rAddr);
void CDCE913_RegWrite(WORD rAddr, BYTE wData);
void CDCE913_RegField(WORD rAddr, BYTE bPos, BYTE bCnt, BYTE bData);

BYTE ENX_I2Cw(WORD sAddr, WORD rAddr, DWORD RegData);
BYTE ENX_I2Cr(WORD sAddr, WORD rAddr, PDWORD pBuff);
void ENX_MSK_I2Cw(WORD sAddr, PDWORD pTable, BYTE size);
void ENX_TBL_I2Cw(WORD sAddr, PDWORD pTable, BYTE size);
BYTE ENX_MultiRead(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);
BYTE ENX_MultiWrite(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);

BYTE TVI_MultiRead(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);
BYTE TVI_MultiWrite(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);

BYTE LCMX03L_RegRead(WORD rAddr);
void LCMX03L_RegWrite(WORD rAddr, BYTE wData);
void LCMX03L_RegField(WORD rAddr, BYTE bPos, BYTE bCnt, BYTE bData);
BYTE LCMX03L_MultiRead(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);
BYTE LCMX03L_MultiWrite(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);

BYTE LIAMD6K_RegRead(WORD rAddr);
void LIAMD6K_RegWrite(WORD rAddr, BYTE wData);
void LIAMD6K_RegField(WORD rAddr, BYTE bPos, BYTE bCnt, BYTE bData);
BYTE LIAMD6K_MultiRead(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);
BYTE LIAMD6K_MultiWrite(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);

BYTE PS8409_RegRead(WORD rAddr);
void PS8409_RegWrite(WORD rAddr, BYTE wData);
void PS8409_RegField(WORD rAddr, BYTE bPos, BYTE bCnt, BYTE bData);
BYTE PS8409_MultiRead(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);
BYTE PS8409_MultiWrite(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes);


#endif	/* __UCOMI2C_H__ */
