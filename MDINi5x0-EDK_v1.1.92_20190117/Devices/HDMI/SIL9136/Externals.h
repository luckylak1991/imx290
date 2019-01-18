/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

#if defined(__USE_SIL9136_TX__)

extern VideoModeDescription_t VideoModeDescription[SIL9136_CHIP_NUM];
extern int VModeTblIndex[SIL9136_CHIP_NUM];

// Patches
//========
extern byte EmbeddedSynPATCH[SIL9136_CHIP_NUM];

//UART
//====
extern byte TXBusy[SIL9136_CHIP_NUM];

extern byte IDX_InChar[SIL9136_CHIP_NUM];
extern byte NumOfArgs[SIL9136_CHIP_NUM];
extern byte MessageType[SIL9136_CHIP_NUM];
extern bool F_SBUF_DataReady[SIL9136_CHIP_NUM];
extern bool F_CollectingData[SIL9136_CHIP_NUM];

extern bool USRX_OutputChange[SIL9136_CHIP_NUM];

// Simulation Configuration
extern bool g_EDID_Simulation[SIL9136_CHIP_NUM];
extern bool g_Video_Mode_Simulation[SIL9136_CHIP_NUM];

extern byte g_audio_Checksum[SIL9136_CHIP_NUM];	// Audio checksum

// Communication
extern byte g_CommData[SIL9136_CHIP_NUM][GLOBAL_BYTE_BUF_BLOCK_SIZE];
     
// HDCP Override
extern bool g_HDCP_Override[SIL9136_CHIP_NUM];

#endif //#if defined(__USE_SIL9136_TX__)
