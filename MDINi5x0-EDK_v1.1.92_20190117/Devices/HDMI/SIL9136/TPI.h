/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

bool TPI_Init (void);			// Document purpose, usage
void TPI_Poll (void);			// Document purpose, usage, rename

void EnableTMDS (void);			// Document purpose, usage
void DisableTMDS (void);		// Document purpose, usage

void RestartHDCP (void);		// Document purpose, usage

void SetAudioMute (byte audioMute);

#define CONTROL_COMMAND_VERSION (5)

// TPI Firmware Version
//=====================
static const char TPI_FW_VERSION[] = {CONTROL_COMMAND_VERSION + '0', '.', '5', '2'};
