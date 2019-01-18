/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

byte ReadByteTPI(byte);
void WriteByteTPI(byte, byte);

byte ReadByteCBUS (byte Offset);
void WriteByteCBUS (byte Offset, byte Data);
void ReadModifyWriteCBUS (byte Offset, byte Mask, byte Value);

void ReadSetWriteTPI (byte, byte);
void ReadClearWriteTPI (byte, byte);
void ReadModifyWriteTPI (byte Offset, byte Mask, byte Value);

void ReadBlockTPI (byte, word, byte *);
void WriteBlockTPI (byte, word, byte *);

bool GetDDC_Access(byte* SysCtrlRegVal);
bool ReleaseDDC(byte SysCtrlRegVal);

#ifdef READKSV
void ReadBlockHDCP(byte, word, byte *);
#endif

byte ReadIndexedRegister(byte, byte);
void WriteIndexedRegister(byte, byte, byte);
void ReadModifyWriteIndexedRegister(byte, byte, byte, byte);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// Indexed Register Offsets, Constants
//====================================
#define INDEXED_PAGE_0			0x01
#define INDEXED_PAGE_1			0x02
#define INDEXED_PAGE_2			0x03

#define DEVICE_ID_LOW_BYTE      0x02
#define DEVICE_ID_HI_BYTE       0x03
#define AUDIO_INPUT_LENGTH		0x24

#define TMDS_CONT_REG           0x82

#define SW_RESET                0x05
#define POWER_DOWN              0x6F


#define DIV_BY_2                0x00
#define MULT_BY_1               0x01
#define MULT_BY_2               0x02
#define MULT_BY_4               0x03

#ifdef DEV_INDEXED_PLL
bool SetPLL(byte);
#endif

// Prototype Declarations
//=======================

bool SetInputWordLength(byte);

bool SetChannelLayout(byte);
