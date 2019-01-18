/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

typedef struct
{
    byte Mode_C1;
    byte Mode_C2;
    byte SubMode;
} ModeIdType;

typedef struct
{
    word Pixels;
    word Lines;
} PxlLnTotalType;

typedef struct
{
    word H;
    word V;
} HVPositionType;

typedef struct
{
    word H;
    word V;
} HVResolutionType;

typedef struct
{
    byte           RefrTypeVHPol;
    word           VFreq;
    PxlLnTotalType Total;
} TagType;

typedef struct
{
    byte IntAdjMode;
    word HLength;
    byte VLength;
    word Top;
    word Dly;
    word HBit2HSync;
    byte VBit2VSync;
    word Field2Offset;
}  _656Type;

typedef struct
{
    byte VactSpace1;
    byte VactSpace2;
    byte Vblank1;
    byte Vblank2;
    byte Vblank3;
} Vspace_Vblank;

typedef struct
{
    ModeIdType       ModeId;
    word             PixClk;
    TagType          Tag;
    HVPositionType   Pos;
    HVResolutionType Res;
    byte             AspectRatio;
    _656Type         _656;
    byte             PixRep;
	Vspace_Vblank    VsVb;
    byte             _3D_Struct;
} VModeInfoType;

#define NSM                     0   // No Sub-Mode

#define	DEFAULT_VIDEO_MODE		0	// 640  x 480p @ 60 VGA

#define ProgrVNegHNeg           0x00
#define ProgrVNegHPos           0x01
#define ProgrVPosHNeg           0x02
#define ProgrVPosHPos           0x03

#define InterlaceVNegHNeg       0x04
#define InterlaceVPosHNeg       0x05
#define InterlaceVNgeHPos       0x06
#define InterlaceVPosHPos       0x07

#define CEA_VIC_BASE             0
#define HDMI_VIC_BASE           48
#define VIC_3D_BASE             54
#define PC_BASE                 68

// Aspect ratio
//=============
#define R_4                      0   // 4:3
#define R_4or16                  1   // 4:3 or 16:9
#define R_16                     2   // 16:9

extern const VModeInfoType VModesTable[];
extern const byte AspectRatioTable[];

//byte ConvertVIC_To_VM_Index(byte, byte);
int ConvertVIC_To_VM_Index(void);
