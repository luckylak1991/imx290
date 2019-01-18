/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include <stdio.h>
#include "common.h"
#include "sil9136.h"

#include "defs.h"
#include "TypeDefs.h"
//#include "gpio.h"
#include "Constants.h"
#include "TPI_Regs.h"
#include "Externals.h"
#include "Macros.h"
//#include "i2c_master_sw.h"
#include "TPI_Access.h"
//#include "util.h"



#if defined(__USE_SIL9136_TX__)


#define T_DDC_ACCESS    50

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   ReadByteTPI ()
//
// PURPOSE      :   Read one byte from a given offset of the TPI interface.
//
// INPUT PARAMS :   Offset of TPI register to be read; A pointer to the variable
//                  where the data read will be stored
//
// OUTPUT PARAMS:   Data - Contains the value read from the register value
//                  specified as the first input parameter
//
// GLOBALS USED :   None
//
// RETURNS      :   TRUE
//
// NOTE         :   ReadByteTPI() is ported from the PC based FW to the uC
//                  version while retaining the same function interface. This
//                  will save the need to modify higher level I/O functions
//                  such as ReadSetWriteTPI(), ReadClearWriteTPI() etc.
//                  A dummy returned value (always TRUE) is provided for
//                  the same reason
//
//////////////////////////////////////////////////////////////////////////////
byte ReadByteTPI(byte RegOffset)
{
	I2C_BusID bus_id=(Sil9136ChipSel==0)?SIL9136TX1_BUSID:SIL9136TX2_BUSID;
	return I2C_ByteRead(bus_id, TX_SLAVE_ADDR, RegOffset);
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  WriteByteTPI ()
//
// PURPOSE       :  Write one byte to a given offset of the TPI interface.
//
// INPUT PARAMS  :  Offset of TPI register to write to; value to be written to
//                  that TPI retister
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  None
//
// RETURNS       :  VOID
//
//////////////////////////////////////////////////////////////////////////////
void WriteByteTPI(byte RegOffset, byte Data)
{
	I2C_BusID bus_id=(Sil9136ChipSel==0)?SIL9136TX1_BUSID:SIL9136TX2_BUSID;
	I2C_ByteWrite(bus_id, TPI_BASE_ADDR, RegOffset, Data);
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  ReadSetWriteTPI(byte Offset, byte Pattern)
//
// PURPOSE       :  Write "1" to all bits in TPI offset "Offset" that are set
//                  to "1" in "Pattern"; Leave all other bits in "Offset"
//                  unchanged.
//
// INPUT PARAMS  :  Offset  :   TPI register offset
//                  Pattern :   GPIO bits that need to be set
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  None
//
// RETURNS       :  TRUE
//
//////////////////////////////////////////////////////////////////////////////
void ReadSetWriteTPI(byte Offset, byte Pattern)
{
    byte Tmp;

    Tmp = ReadByteTPI(Offset);

    Tmp |= Pattern;
    WriteByteTPI(Offset, Tmp);
}

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  ReadClearWriteTPI(byte Offset, byte Pattern)
//
// PURPOSE       :  Write "0" to all bits in TPI offset "Offset" that are set
//                  to "1" in "Pattern"; Leave all other bits in "Offset"
//                  unchanged.
//
// INPUT PARAMS  :  Offset  :   TPI register offset
//                  Pattern :   "1" for each TPI register bit that needs to be
//                              cleared
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  None
//
// RETURNS       :  TRUE
//
//////////////////////////////////////////////////////////////////////////////
void ReadClearWriteTPI(byte Offset, byte Pattern)
{
    byte Tmp;

    Tmp = ReadByteTPI(Offset);

    Tmp &= ~Pattern;
    WriteByteTPI(Offset, Tmp);
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  ReadModifyWriteTPI(byte Offset, byte Mask, byte Value)
//
// PURPOSE       :  Write "Value" to all bits in TPI offset "Offset" that are set
//                  to "1" in "Mask"; Leave all other bits in "Offset"
//                  unchanged.
//
// INPUT PARAMS  :  Offset  :   TPI register offset
//                  Mask    :   "1" for each TPI register bit that needs to be
//                              modified
//					Value   :   The desired value for the register bits in their
//								proper positions
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  None
//
// RETURNS       :  void
//
//////////////////////////////////////////////////////////////////////////////
void ReadModifyWriteTPI(byte Offset, byte Mask, byte Value)
{
    byte Tmp;

    Tmp = ReadByteTPI(Offset);
    Tmp &= ~Mask;
	Tmp |= (Value & Mask);
    WriteByteTPI(Offset, Tmp);
}


////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION         :   ReadBlockTPI ()
//
// PURPOSE          :   Read NBytes from offset Addr of the TPI slave address
//                      into a byte Buffer pointed to by Data
//
// INPUT PARAMETERS :   TPI register offset, number of bytes to read and a
//                      pointer to the data buffer where the data read will be
//                      saved
//
// OUTPUT PARAMETERS:   pData - pointer to the buffer that will store the TPI
//                      block to be read
//
// RETURNED VALUE   :   VOID
//
// GLOBALS USED     :   None
//
////////////////////////////////////////////////////////////////////////////////
void ReadBlockTPI(byte TPI_Offset, word NBytes, byte * pData)
{
	I2C_BusID bus_id=(Sil9136ChipSel==0)?SIL9136TX1_BUSID:SIL9136TX2_BUSID;
	I2C_Read(bus_id, TPI_BASE_ADDR, TPI_Offset, I2C_ADDR_8BIT, pData, I2C_DATA_8BIT, NBytes);
}


////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION         :   WriteBlockTPI ()
//
// PURPOSE          :   Write NBytes from a byte Buffer pointed to by Data to
//                      the TPI I2C slave starting at offset Addr
//
// INPUT PARAMETERS :   TPI register offset to start writing at, number of bytes
//                      to write and a pointer to the data buffer that contains
//                      the data to write
//
// OUTPUT PARAMETERS:   None.
//
// RETURNED VALUES  :   void
//
// GLOBALS USED     :   None
//
////////////////////////////////////////////////////////////////////////////////
void WriteBlockTPI(byte TPI_Offset, word NBytes, byte * pData)
{
	I2C_BusID bus_id=(Sil9136ChipSel==0)?SIL9136TX1_BUSID:SIL9136TX2_BUSID;
	I2C_Write(bus_id, TPI_BASE_ADDR, TPI_Offset, I2C_ADDR_8BIT, pData, I2C_DATA_8BIT, NBytes);
}


byte ReadByteCBUS(byte Offset)
{
	I2C_BusID bus_id=(Sil9136ChipSel==0)?SIL9136TX1_BUSID:SIL9136TX2_BUSID;
	return I2C_ByteRead(bus_id, CBUS_SLAVE_ADDR, Offset);
}

void WriteByteCBUS(byte Offset, byte Data)
{
	I2C_BusID bus_id=(Sil9136ChipSel==0)?SIL9136TX1_BUSID:SIL9136TX2_BUSID;
	I2C_ByteWrite(bus_id, CBUS_SLAVE_ADDR, Offset, Data);
}

void ReadModifyWriteCBUS(byte Offset, byte Mask, byte Value) {
    byte Tmp;

    Tmp = ReadByteCBUS(Offset);
    Tmp &= ~Mask;
	Tmp |= (Value & Mask);
    WriteByteCBUS(Offset, Tmp);
	}

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  GetDDC_Access(void)
//
// PURPOSE       :  Request access to DDC bus from the receiver
//
// INPUT PARAMS  :  None
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  None
//
// RETURNS       :  TRUE if bus obtained successfully. FALSE if failed.
//
//////////////////////////////////////////////////////////////////////////////

bool GetDDC_Access (byte* SysCtrlRegVal)
{
	byte sysCtrl;
	byte DDCReqTimeout = T_DDC_ACCESS;
	byte TPI_ControlImage;

	TPI_TRACE_PRINT((">>GetDDC_Access()\n"));

	sysCtrl = ReadByteTPI (TPI_SYSTEM_CONTROL_DATA_REG);			// Read and store original value. Will be passed into ReleaseDDC()
	*SysCtrlRegVal = sysCtrl;

	sysCtrl |= DDC_BUS_REQUEST_REQUESTED;
	WriteByteTPI (TPI_SYSTEM_CONTROL_DATA_REG, sysCtrl);

	while (DDCReqTimeout--)											// Loop till 0x1A[1] reads "1"
	{
		TPI_ControlImage = ReadByteTPI(TPI_SYSTEM_CONTROL_DATA_REG);

		if (TPI_ControlImage & DDC_BUS_GRANT_MASK)					// When 0x1A[1] reads "1"
		{
			sysCtrl |= DDC_BUS_GRANT_GRANTED;
			WriteByteTPI(TPI_SYSTEM_CONTROL_DATA_REG, sysCtrl);		// lock host DDC bus access (0x1A[2:1] = 11)
			return TRUE;
		}
		WriteByteTPI(TPI_SYSTEM_CONTROL_DATA_REG, sysCtrl);			// 0x1A[2] = "1" - Requst the DDC bus
		DelayMS(200);
	}

	WriteByteTPI(TPI_SYSTEM_CONTROL_DATA_REG, sysCtrl);				// Failure... restore original value.
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  ReleaseDDC(void)
//
// PURPOSE       :  Release DDC bus
//
// INPUT PARAMS  :  None
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  None
//
// RETURNS       :  TRUE if bus released successfully. FALSE if failed.
//
//////////////////////////////////////////////////////////////////////////////

bool ReleaseDDC(byte SysCtrlRegVal)
{
	byte DDCReqTimeout = T_DDC_ACCESS;
	byte TPI_ControlImage;

	TPI_TRACE_PRINT((">>ReleaseDDC()\n"));

	SysCtrlRegVal &= ~BITS_2_1;					// Just to be sure bits [2:1] are 0 before it is written

	while (DDCReqTimeout--)						// Loop till 0x1A[1] reads "0"
	{
		// Cannot use ReadClearWriteTPI() here. A read of TPI_SYSTEM_CONTROL is invalid while DDC is granted.
		// Doing so will return 0xFF, and cause an invalid value to be written back.
		//ReadClearWriteTPI(TPI_SYSTEM_CONTROL,BITS_2_1); // 0x1A[2:1] = "0" - release the DDC bus

		WriteByteTPI(TPI_SYSTEM_CONTROL_DATA_REG, SysCtrlRegVal);
		TPI_ControlImage = ReadByteTPI(TPI_SYSTEM_CONTROL_DATA_REG);

		if (!(TPI_ControlImage & BITS_2_1))		// When 0x1A[2:1] read "0"
			return TRUE;
	}

	return FALSE;								// Failed to release DDC bus control
}


////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION         :   ReadBlockHDCP ()
//
// PURPOSE          :   Read NBytes from offset Addr of the HDCP slave address
//                      into a byte Buffer pointed to by Data
//
// INPUT PARAMETERS :   HDCP port offset, number of bytes to read and a
//                      pointer to the data buffer where the data read will be
//                      saved
//
// OUTPUT PARAMETERS:   Data, a buffer that contains the block read from the CPI
//
// RETURNED VALUE   :   void
//
// GLOBALS USED     :   None
//
////////////////////////////////////////////////////////////////////////////////
#ifdef READKSV
void ReadBlockHDCP(byte TPI_Offset, word NBytes, byte * pData)
{
	TPI_TRACE_PRINT((">>ReadBlockHDCP()\n"));
    I2C_ReadBlock(HDCP_SLAVE_ADDR, TPI_Offset, pData, NBytes);
}
#endif


#define TPI_INTERNAL_PAGE_REG	0xBC
#define TPI_INDEXED_OFFSET_REG	0xBD
#define TPI_INDEXED_VALUE_REG	0xBE


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   ReadIndexedRegister()
//
// PURPOSE      :   Read an indexed register value
//
//                  Write:
//                      1. 0xBC => Internal page num
//                      2. 0xBD => Indexed register offset
//
//                  Read:
//                      3. 0xBE => Returns the indexed register value
//
// INPUT PARAMS :   Internal page number, indexed register offset, pointer to
//                  buffer to store read value
//
// OUTPUT PARAMS:   Buffer that stores the read value
//
// GLOBALS USED :   None
//
// RETURNS      :   TRUE 
//
//////////////////////////////////////////////////////////////////////////////
#ifdef DEV_INDEXED_READ
byte ReadIndexedRegister(byte PageNum, byte RegOffset) {
    WriteByteTPI(TPI_INTERNAL_PAGE_REG, PageNum);		// Internal page
    WriteByteTPI(TPI_INDEXED_OFFSET_REG, RegOffset);	// Indexed register
    return ReadByteTPI(TPI_INDEXED_VALUE_REG); 		// Return read value
}
#endif


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   WriteIndexedRegister()
//
// PURPOSE      :   Write a value to an indexed register
//
//                  Write:
//                      1. 0xBC => Internal page num
//                      2. 0xBD => Indexed register offset
//                      3. 0xBE => Set the indexed register value
//
// INPUT PARAMS :   Internal page number, indexed register offset, value
//                  to write to indexed register
//
// OUTPUT PARAMS:   None
//
// GLOBALS USED :   None
//
// RETURNS      :   TRUE 
//
//////////////////////////////////////////////////////////////////////////////
#ifdef DEV_INDEXED_WRITE
void WriteIndexedRegister(byte PageNum, byte RegOffset, byte RegValue) {
    WriteByteTPI(TPI_INTERNAL_PAGE_REG, PageNum);  // Internal page
    WriteByteTPI(TPI_INDEXED_OFFSET_REG, RegOffset);  // Indexed register
    WriteByteTPI(TPI_INDEXED_VALUE_REG, RegValue);    // Read value into buffer
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  ReadModifyWriteIndexedRegister(byte PageNum, byte Offset, byte Mask, byte Value)
//
// PURPOSE       :  Write "Value" to all bits in TPI offset "Offset" that are set
//                  to "1" in "Mask"; Leave all other bits in "Offset"
//                  unchanged.
//
// INPUT PARAMS  :  Offset  :   TPI register offset
//                  Mask    :   "1" for each TPI register bit that needs to be
//                              modified
//					Value   :   The desired value for the register bits in their
//								proper positions
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  None
//
// RETURNS       :  void
//
//////////////////////////////////////////////////////////////////////////////
#ifdef DEV_INDEXED_RMW
void ReadModifyWriteIndexedRegister(byte PageNum, byte RegOffset, byte Mask, byte Value)
{
    byte Tmp;

    WriteByteTPI(TPI_INTERNAL_PAGE_REG, PageNum);
    WriteByteTPI(TPI_INDEXED_OFFSET_REG, RegOffset);
    Tmp = ReadByteTPI(TPI_INDEXED_VALUE_REG);

    Tmp &= ~Mask;
	Tmp |= (Value & Mask);

    WriteByteTPI(TPI_INDEXED_VALUE_REG, Tmp);
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   SetPLL()
//
// PURPOSE      :   Set the 9024/9024/9222 chip PLL to multiply the input pixel
//                  clock by the value passed to this function as a parameter
//                  registers:
//
// INPUT PARAMS :   PLL Multiplication factor (represents 0.5, 1, 2 or 4)
//
// OUTPUT PARAMS:   None
//
// GLOBALS USED :   None
//
// RETURNS      :   TRUE 
//
//////////////////////////////////////////////////////////////////////////////
#ifdef DEV_INDEXED_PLL
bool SetPLL(byte TClkSel)
{
    byte RegValue;
    byte Pattern;

    // Set up register 0x82[6:5] (same for all input pixel frequencies for a specific multiplier value):

    ReadIndexedRegister(0x01, TMDS_CONT_REG, &RegValue);

    Pattern = TClkSel << 5;
    RegValue &= ~BITS_6_5;
    RegValue |= Pattern;

    WriteByteTPI(TPI_REGISTER_VALUE_REG, RegValue);

    return TRUE;
}
#endif


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   SetChannelLayout()
//
// PURPOSE      :   Set up the Channel layout field of internal register 0x2F
//                  (0x2F[1])
//
// INPUT PARAMS :   Number of audio channels: "0 for 2-Channels ."1" for 8.
//
// OUTPUT PARAMS:   None
//
// GLOBALS USED :   None
//
// RETURNS      :   TRUE 
//
//////////////////////////////////////////////////////////////////////////////
#ifdef SetChannelLayout
bool SetChannelLayout(byte Count)
{
    // Indexed register 0x7A:0x2F[1]:
    WriteByteTPI(TPI_INTERNAL_PAGE_REG, 0x02); // Internal page 2
    WriteByteTPI(TPI_REGISTER_OFFSET_REG, 0x2F);

    Count &= THREE_LSBITS;

    if (Count == TWO_CHANNEL_LAYOUT)
    {
        // Clear 0x2F:
        ReadClearWriteTPI(TPI_REGISTER_VALUE_REG, BIT_1);
    }

    else if (Count == EIGHT_CHANNEL_LAYOUT)
    {
        // Set 0x2F[0]:
        ReadSetWriteTPI(TPI_REGISTER_VALUE_REG, BIT_1);
    }

    return TRUE;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   SetInputWordLength()
//
// PURPOSE      :   Tell the Tx what the I2S input word is
//
// INPUT PARAMS :   Input word length code per SiI-PR-1032-0.02, table 20
//
// OUTPUT PARAMS:   None
//
// GLOBALS USED :   None
//
// RETURNS      :   TRUE
//
// NOTE         :   Used by 9022/4 only. 9022A/24A and 9334 set that value
//                  automatically when 0x25[5:3] is set
//
//////////////////////////////////////////////////////////////////////////////
#ifdef SETINPUTWORDLENGTH
bool SetInputWordLength(byte Length)
{
    byte RegValue;

    RegValue = ReadIndexedRegister(0x02, AUDIO_INPUT_LENGTH);

    RegValue &= ~LOW_NIBBLE;
    RegValue |= Length;

    WriteByteTPI(TPI_REGISTER_VALUE_REG, RegValue);

    return TRUE;
}
#endif

#endif	//__USE_SIL9136_TX__
