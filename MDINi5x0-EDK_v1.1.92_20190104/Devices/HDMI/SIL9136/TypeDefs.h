/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

// typedef unsigned char		bool;
 typedef unsigned char		byte;
 typedef unsigned short		word;
 typedef unsigned long		dword;


typedef enum
{
	SS_FATAL_ERROR,
	SS_RX_NOT_READY,
	SS_RX_NEW_VIDEO_MODE,
	SS_TX_READY
} SYSTEM_STATE;

//------------------------------------------------------------------------------
// Video Mode Description
// This is the common API that passes Video Mode information to the firmware,
// whether the source is the AMF libray or Simon.
//------------------------------------------------------------------------------
typedef struct
{
    unsigned char  VIC;				// VIC OR the HDMI_VIC
    unsigned char  AspectRatio;			// 4x3 or 16x9
    unsigned char  ColorSpace;
    unsigned char  ColorDepth;
    unsigned char  HDCPAuthenticated;
    unsigned char  HDMIVideoFormat;		// VIC, HDMI_VIC or 3D
    unsigned char  ThreeDStructure;		// Only used when 3D
} VideoModeDescription_t;
