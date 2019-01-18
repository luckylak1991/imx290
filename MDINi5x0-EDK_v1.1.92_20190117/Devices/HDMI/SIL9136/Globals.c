/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

//#include "MCU_Regs.h"
#include "common.h"
#include "sil9136.h"

#include "defs.h"
#include "TypeDefs.h"
#include "Constants.h"
//#include "si_apiConfigure.h"



#if defined(__USE_SIL9136_TX__)


VideoModeDescription_t VideoModeDescription[SIL9136_CHIP_NUM];		// Video Mode description block
int VModeTblIndex[SIL9136_CHIP_NUM];					// Index to Video Mode Table row for our current mode

// Patches
//========
byte EmbeddedSynPATCH[SIL9136_CHIP_NUM];


//UART
//====
byte TXBusy[SIL9136_CHIP_NUM];

byte IDX_InChar[SIL9136_CHIP_NUM];
byte NumOfArgs[SIL9136_CHIP_NUM];
byte MessageType[SIL9136_CHIP_NUM];
bool F_SBUF_DataReady[SIL9136_CHIP_NUM];
bool F_CollectingData[SIL9136_CHIP_NUM];

bool USRX_OutputChange[SIL9136_CHIP_NUM];


// Simulation Configuration
bool g_EDID_Simulation[SIL9136_CHIP_NUM];
bool g_Video_Mode_Simulation[SIL9136_CHIP_NUM];

// Checksums
byte g_audio_Checksum[SIL9136_CHIP_NUM];	// Audio checksum

// Serial Communication Buffer
byte g_CommData[SIL9136_CHIP_NUM][GLOBAL_BYTE_BUF_BLOCK_SIZE];

// HDCP Override
bool g_HDCP_Override[SIL9136_CHIP_NUM];

#endif	//__USE_SIL9136_TX__
