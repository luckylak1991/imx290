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
#include "typedefs.h"
//#include "gpio.h"
#include "TPI_REGS.h"
#include "Constants.h"
#include "Externals.h"
#include "Macros.h"
#include "TPI_Access.h"
#include "TPI.h"
//#include "Util.h"
#include "AV_Config.h"
//#include "i2c_master_sw.h"
#include "VideoModeTable.h"
//#include "SerialPort.h"

//#include "edid.h"
//#include "HDCP.h"



#if defined(__USE_SIL9136_TX__)

/*****************************************/
// directives from SerialPort.h
#define VIDEO_SETUP_CMD_LEN     	(9)
#define VIDEO_SYNC_SETUP_CMD_LEN	(20)

#define AUDIO_SETUP_CMD_LEN 		(7)
#define I2S_MAPPING_CMD_LEN     	(4)
#define I2S_INPUT_CFG_CMD_LEN   	(1)
#define I2S_STREAM_HDR_CMD_LEN  	(5)

#define AVI_INFOFRAME_CMD_LEN		(14)
#define AUDIO_INFOFRAME_CMD_LEN		(15)

#define EHDMI_MODE_CMD_LEN			(1)
#define HDCP_OVERRIDE_MODE_CMD_LEN	(1)

#define PRODUCT_CODE_CMD_LEN		(1)
#define TPI_FW_VERSION_CMD_LEN		((byte)sizeof(TPI_FW_VERSION))
#define CTRL_CMD_VERSION_CMD_LEN	(1)
/*****************************************/

// Serial Communication Buffer assignments and macros
#define Command						g_CommData[Sil9136ChipSel][0]
#define CommandLength				g_CommData[Sil9136ChipSel][1] 
#define Arg(a)                      g_CommData[Sil9136ChipSel][(2+a)]

// VSIF Constants
//===============
#define VSIF_TYPE  						0x81
#define VSIF_VERSION					0x01
#define VSIF_LEN						0x07

#ifdef DEV_SUPPORT_EDID
void Set_VSIF(void);
#endif

#if defined(__MDIN_i550_USB_V100__)
#define DEV_EMBEDDED		// for aux sync embedded output
#endif

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  InitVideo()
//
// PURPOSE       :  Set the 9022/4 to the video mode determined by GetVideoMode()
//
// INPUT PARAMS  :  Index of video mode to set; Flag that distinguishes between
//                  calling this function after power up and after input
//                  resolution change
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  VModesTable, VideoCommandImage
//
// RETURNS       :  TRUE
//
//////////////////////////////////////////////////////////////////////////////

bool InitVideo(byte TclkSel)
{
	int	ModeTblIndex;
#ifdef DEEP_COLOR
	byte temp;
#endif
	byte B_Data[8];

#ifdef DEV_EMBEDDED
	byte EMB_Status;
#endif
#ifdef USE_DE_GENERATOR
	byte DE_Status;
#endif

#ifndef DEV_INDEXED_PLL
	byte Pattern;
#endif
	TPI_TRACE_PRINT((">>InitVideo() - TclkSel:%d\n", TclkSel));

	TPI_TRACE_PRINT(("InitVideo: V:%d", (int) VideoModeDescription[Sil9136ChipSel].VIC));
	TPI_TRACE_PRINT((" A:%x", (int) VideoModeDescription[Sil9136ChipSel].AspectRatio));
	TPI_TRACE_PRINT((" CS:%x", (int) VideoModeDescription[Sil9136ChipSel].ColorSpace));
	TPI_TRACE_PRINT((" CD:%x", (int) VideoModeDescription[Sil9136ChipSel].ColorDepth));
	TPI_TRACE_PRINT((" HA:%d", (int) VideoModeDescription[Sil9136ChipSel].HDCPAuthenticated));
	TPI_TRACE_PRINT((" HF:%d", (int) VideoModeDescription[Sil9136ChipSel].HDMIVideoFormat));
	TPI_TRACE_PRINT((" 3D:%d\n", (int) VideoModeDescription[Sil9136ChipSel].ThreeDStructure));

	ModeTblIndex = (byte)ConvertVIC_To_VM_Index();

#ifdef DEV_INDEXED_PLL
	SetPLL(TclkSel);
#else
	Pattern = (TclkSel << 6) & TWO_MSBITS;								// Use TPI 0x08[7:6] for 9022A/24A video clock multiplier
	ReadSetWriteTPI(TPI_PIX_REPETITION, Pattern);
#endif

	// Take values from VModesTable[]:

	B_Data[0] = VModesTable[ModeTblIndex].PixClk & 0x00FF;			// write Pixel clock to TPI registers 0x00, 0x01
	B_Data[1] = (VModesTable[ModeTblIndex].PixClk >> 8) & 0xFF;

	B_Data[2] = VModesTable[ModeTblIndex].Tag.VFreq & 0x00FF;			// write Vertical Frequency to TPI registers 0x02, 0x03
	B_Data[3] = (VModesTable[ModeTblIndex].Tag.VFreq >> 8) & 0xFF;

	B_Data[4] = VModesTable[ModeTblIndex].Tag.Total.Pixels & 0x00FF;	// write total number of pixels to TPI registers 0x04, 0x05
	B_Data[5] = (VModesTable[ModeTblIndex].Tag.Total.Pixels >> 8) & 0xFF;

	B_Data[6] = VModesTable[ModeTblIndex].Tag.Total.Lines & 0x00FF;	// write total number of lines to TPI registers 0x06, 0x07
	B_Data[7] = (VModesTable[ModeTblIndex].Tag.Total.Lines >> 8) & 0xFF;

	WriteBlockTPI(TPI_PIX_CLK_LSB, 8, B_Data);					// Write TPI Mode data.

	B_Data[0] = (VModesTable[ModeTblIndex].PixRep) & LOW_BYTE;		// Set pixel replication field of 0x08
	B_Data[0] |= BIT_BUS_24;									// Set 24 bit bus

#ifndef DEV_INDEXED_PLL
	B_Data[0] |= (TclkSel << 6) & TWO_MSBITS;
#endif
#ifdef CLOCK_EDGE_FALLING
	B_Data[0] &= ~BIT_EDGE_RISE;								// Set to falling edge
#endif
#ifdef CLOCK_EDGE_RISING
	B_Data[0] |= BIT_EDGE_RISE;									// Set to rising edge
#endif
	WriteByteTPI(TPI_PIX_REPETITION, B_Data[0]);				// 0x08

#ifdef DEV_EMBEDDED
	EMB_Status = SetEmbeddedSync();
	EnableEmbeddedSync();
#endif

#ifdef USE_DE_GENERATOR
	ReadClearWriteTPI(TPI_SYNC_GEN_CTRL, MSBIT);				// set 0x60[7] = 0 for External Sync
	DE_Status = SetDE();									// Call SetDE() with Video Mode as a parameter
	B_Data[0] = (((BITS_IN_YCBCR422 | BITS_IN_AUTO_RANGE) & ~BIT_EN_DITHER_10_8) & ~BIT_EXTENDED_MODE);	// 0x09
#else
#if defined(__MDIN_i540_4K60__)
//	B_Data[0] = (((BITS_IN_RGB | BITS_IN_FULL_RANGE) & ~BIT_EN_DITHER_10_8) & ~BIT_EXTENDED_MODE);		// 0x09 -- slave(green noise), master(blinking) in dark image(or red pattern)
	B_Data[0] = (((BITS_IN_RGB | BITS_IN_LTD_RANGE) & ~BIT_EN_DITHER_10_8) & ~BIT_EXTENDED_MODE);		// 0x09
#else
	B_Data[0] = (((BITS_IN_YCBCR422 | BITS_IN_LTD_RANGE) & ~BIT_EN_DITHER_10_8) & ~BIT_EXTENDED_MODE);		// 0x09
#endif
#endif

#ifdef DEEP_COLOR
	switch (VideoModeDescription[Sil9136ChipSel].ColorDepth)
	{
		case 0:  temp = 0x00; ReadModifyWriteTPI(TPI_DEEP_COLOR_GCP, BIT_2, 0x00); break;
		case 1:  temp = 0x80; ReadModifyWriteTPI(TPI_DEEP_COLOR_GCP, BIT_2, BIT_2); break;
		case 2:  temp = 0xC0; ReadModifyWriteTPI(TPI_DEEP_COLOR_GCP, BIT_2, BIT_2); break;
		case 3:  temp = 0x40; ReadModifyWriteTPI(TPI_DEEP_COLOR_GCP, BIT_2, BIT_2); break;
		default: temp = 0x00; ReadModifyWriteTPI(TPI_DEEP_COLOR_GCP, BIT_2, 0x00); break;
	}
	B_Data[0] = ((B_Data[0] & 0x3F) | temp);
#endif

	if(IsOUT_MainSyncEmb())		B_Data[0] = ((B_Data[0] & 0xFC) | 0x02);
#ifdef DEV_EMBEDDED
	B_Data[0] = ((B_Data[0] & 0xFC) | 0x02);
#endif

#if defined(__MDIN_i540_4K60__)
	B_Data[1] = (BITS_OUT_RGB | BITS_OUT_LTD_RANGE) & ~BIT_BT_709;	// video range compression 'off(full)', 'on(ltd)'
#else
	B_Data[1] = (BITS_OUT_RGB | BITS_OUT_FULL_RANGE) | BIT_BT_709;	// video range compression 'off(full)', 'on(ltd)'
#endif
	
#ifdef DEEP_COLOR
	B_Data[1] = ((B_Data[1] & 0x3F) | temp);
#endif

#ifdef DEV_SUPPORT_EDID
	if (!IsHDMI_Sink()) {
		B_Data[1] = ((B_Data[1] & 0xFC) | 0x03);
		}
	else {
		// Set YCbCr color space depending on EDID
		if (EDID_Data.YCbCr_4_4_4) {
			B_Data[1] = ((B_Data[1] & 0xFC) | 0x01);
			}
		else {
			if (EDID_Data.YCbCr_4_2_2) {
				B_Data[1] = ((B_Data[1] & 0xFC) | 0x02);
				}
			}
		}
#endif

	SetFormat(B_Data);
	SIL9136_SetCSC(Sil9136CSC);
	
	ReadClearWriteTPI(TPI_SYNC_GEN_CTRL, BIT_2);	// Number HSync pulses from VSync active edge to Video Data Period should be 20 (VS_TO_VIDEO)

#ifdef SOURCE_TERMINATION_ON
	{
		byte regData;
		regData = ReadIndexedRegister(INDEXED_PAGE_1, TMDS_CONT_REG);
		regData = (regData & 0x3F) | 0x25;
		WriteIndexedRegister(INDEXED_PAGE_1, TMDS_CONT_REG, regData);
	}
#endif

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   SetFormat(byte * Data)
//
// PURPOSE      :   
//
// INPUT PARAMS :   
//
// OUTPUT PARAMS:   
//
// GLOBALS USED :   
//
// RETURNS      :   
//
//////////////////////////////////////////////////////////////////////////////
void SetFormat(byte *Data)
{
	if (!IsHDMI_Sink())
	{
		ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, OUTPUT_MODE_MASK, OUTPUT_MODE_HDMI); // Set HDMI mode to allow color space conversion
	}

	WriteBlockTPI(TPI_INPUT_FORMAT_REG, 2, Data);   // Program TPI AVI Input and Output Format
	WriteByteTPI(TPI_END_RIGHT_BAR_MSB, 0x00);	    // Set last byte of TPI AVI InfoFrame for TPI AVI I/O Format to take effect

	if (!IsHDMI_Sink()) 
	{
		ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, OUTPUT_MODE_MASK, OUTPUT_MODE_DVI);
	}

#ifdef DEV_EMBEDDED
	EnableEmbeddedSync();							// Last byte of TPI AVI InfoFrame resets Embedded Sync Extraction
#endif
}

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   SetEmbeddedSync()
//
// PURPOSE      :   Set the 9022/4 registers to extract embedded sync.
//
// INPUT PARAMS :   Index of video mode to set
//
// OUTPUT PARAMS:   None
//
// GLOBALS USED :   VModesTable[]
//
// RETURNS      :   TRUE
//
//////////////////////////////////////////////////////////////////////////////
bool SetEmbeddedSync()
{
    int	ModeTblIndex;
    word H_Bit_2_H_Sync;
    word Field2Offset;
    word H_SyncWidth;

    byte V_Bit_2_V_Sync;
    byte V_SyncWidth;
    byte B_Data[8];

    TPI_TRACE_PRINT((">>SetEmbeddedSync()\n"));

	ReadModifyWriteIndexedRegister(INDEXED_PAGE_0, 0x0A, 0x01, 0x01);

	ReadClearWriteTPI(TPI_SYNC_GEN_CTRL, MSBIT);	 // set 0x60[7] = 0 for DE mode
//	WriteByteTPI(0x63, 0x30);
	ReadModifyWriteTPI(TPI_DE_CTRL, BITS_5_4, 0x00);	// sync polarity 0x63[4]HS, [5]VS, 0:positive, 1:negative
    ReadSetWriteTPI(TPI_SYNC_GEN_CTRL, MSBIT);       // set 0x60[7] = 1 for Embedded Sync

    ModeTblIndex = ConvertVIC_To_VM_Index(); 

    H_Bit_2_H_Sync = VModesTable[ModeTblIndex]._656.HBit2HSync;
    Field2Offset = VModesTable[ModeTblIndex]._656.Field2Offset;
    H_SyncWidth = VModesTable[ModeTblIndex]._656.HLength;
    V_Bit_2_V_Sync = VModesTable[ModeTblIndex]._656.VBit2VSync;
    V_SyncWidth = VModesTable[ModeTblIndex]._656.VLength;

    B_Data[0] = H_Bit_2_H_Sync & LOW_BYTE;                  // Setup HBIT_TO_HSYNC 8 LSBits (0x62)

    B_Data[1] = (H_Bit_2_H_Sync >> 8) & TWO_LSBITS;         // HBIT_TO_HSYNC 2 MSBits
    //B_Data[1] |= BIT_EN_SYNC_EXTRACT;                     // and Enable Embedded Sync to 0x63

    EmbeddedSynPATCH[Sil9136ChipSel] = B_Data[1];

    B_Data[2] = Field2Offset & LOW_BYTE;                    // 8 LSBits of "Field2 Offset" to 0x64
    B_Data[3] = (Field2Offset >> 8) & LOW_NIBBLE;           // 2 MSBits of "Field2 Offset" to 0x65

    B_Data[4] = H_SyncWidth & LOW_BYTE;
    B_Data[5] = (H_SyncWidth >> 8) & TWO_LSBITS;                    // HWIDTH to 0x66, 0x67
    B_Data[6] = V_Bit_2_V_Sync;                                     // VBIT_TO_VSYNC to 0x68
    B_Data[7] = V_SyncWidth;                                        // VWIDTH to 0x69

    Delay_mSec(500);                                        // wait for isp vout stable(i->p)
	WriteBlockTPI(TPI_HBIT_TO_HSYNC_7_0, 8, &B_Data[0]);
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   EnableEmbeddedSync
//
// PURPOSE      :
//
// INPUT PARAMS :
//
// OUTPUT PARAMS:
//
// GLOBALS USED :
//
// RETURNS      :
//
//////////////////////////////////////////////////////////////////////////////
void EnableEmbeddedSync()
{
    TPI_TRACE_PRINT((">>EnableEmbeddedSync()\n"));

	ReadClearWriteTPI(TPI_SYNC_GEN_CTRL, MSBIT);	 // set 0x60[7] = 0 for DE mode
//	WriteByteTPI(0x63, 0x30);
	ReadModifyWriteTPI(TPI_DE_CTRL, BITS_5_4, 0x00);	// sync polarity 0x63[4]HS, [5]VS, 0:positive, 1:negative
    ReadSetWriteTPI(TPI_SYNC_GEN_CTRL, MSBIT);       // set 0x60[7] = 1 for Embedded Sync
	ReadSetWriteTPI(TPI_DE_CTRL, BIT_6);
}

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   SetDE()
//
// PURPOSE      :   Set the 9022/4 internal DE generator parameters
//
// INPUT PARAMS : 
//
// OUTPUT PARAMS:   None
//
// GLOBALS USED :   None
//
// RETURNS      :   DE_SET_OK
//
// NOTE         :   0x60[7] must be set to "0" for the follwing settings to
//                  take effect
//
//////////////////////////////////////////////////////////////////////////////
byte SetDE()
{
    byte RegValue;
	int	ModeTblIndex;

    word H_StartPos;
    word V_StartPos;
	word Htotal;
	word Vtotal;
    word H_Res;
    word V_Res;

    byte Polarity;
    byte B_Data[12];

    TPI_TRACE_PRINT((">>SetDE()\n"));

    ModeTblIndex = ConvertVIC_To_VM_Index();

    if (VModesTable[ModeTblIndex]._3D_Struct != NO_3D_SUPPORT)
    {
        TPI_TRACE_PRINT((">>SetDE() not allowed with 3D video format\n"));
        return DE_CANNOT_BE_SET_WITH_3D_MODE;
    }

    // Make sure that External Sync method is set before enableing the DE Generator:
    RegValue = ReadByteTPI(TPI_SYNC_GEN_CTRL);

    if (RegValue & BIT_7)
    {
        return DE_CANNOT_BE_SET_WITH_EMBEDDED_SYNC;
    }

    H_StartPos = VModesTable[ModeTblIndex].Pos.H;
    V_StartPos = VModesTable[ModeTblIndex].Pos.V;

   	Htotal = VModesTable[ModeTblIndex].Tag.Total.Pixels;
	Vtotal = VModesTable[ModeTblIndex].Tag.Total.Lines;

    Polarity = (~VModesTable[ModeTblIndex].Tag.RefrTypeVHPol) & TWO_LSBITS;

    H_Res = VModesTable[ModeTblIndex].Res.H;

        if ((VModesTable[ModeTblIndex].Tag.RefrTypeVHPol & 0x04))
        {
        	V_Res = (VModesTable[ModeTblIndex].Res.V) >> 1;
        }
        else
        {
            V_Res = (VModesTable[ModeTblIndex].Res.V);
        }

    B_Data[0] = H_StartPos & LOW_BYTE;              // 8 LSB of DE DLY in 0x62

    B_Data[1] = (H_StartPos >> 8) & TWO_LSBITS;     // 2 MSBits of DE DLY to 0x63
    B_Data[1] |= (Polarity << 4);                   // V and H polarity
    B_Data[1] |= BIT_EN_DE_GEN;                     // enable DE generator

    B_Data[2] = V_StartPos & SEVEN_LSBITS;      // DE_TOP in 0x64
    B_Data[3] = 0x00;                           // 0x65 is reserved
    B_Data[4] = H_Res & LOW_BYTE;               // 8 LSBits of DE_CNT in 0x66
    B_Data[5] = (H_Res >> 8) & LOW_NIBBLE;      // 4 MSBits of DE_CNT in 0x67
    B_Data[6] = V_Res & LOW_BYTE;               // 8 LSBits of DE_LIN in 0x68
    B_Data[7] = (V_Res >> 8) & THREE_LSBITS;    // 3 MSBits of DE_LIN in 0x69
	B_Data[8] = Htotal & LOW_BYTE;				// 8 LSBits of H_RES in 0x6A
	B_Data[9] =	(Htotal >> 8) & LOW_NIBBLE;		// 4 MSBITS of H_RES in 0x6B
	B_Data[10] = Vtotal & LOW_BYTE;				// 8 LSBits of V_RES in 0x6C
	B_Data[11] = (Vtotal >> 8) & BITS_2_1_0;	// 3 MSBITS of V_RES in 0x6D

    WriteBlockTPI(TPI_DE_DLY, 12, &B_Data[0]);

    return DE_SET_OK;                               // Write completed successfully
}



//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      : SetBasicAudio()
//
// PURPOSE       : Set the 9022/4 audio interface to basic audio.
//
// INPUT PARAMS  : None
//
// OUTPUT PARAMS : None
//
// GLOBALS USED  : None
//
// RETURNS       : void.
//
//////////////////////////////////////////////////////////////////////////////
void SetBasicAudio(void)
{

        TPI_TRACE_PRINT((">>SetBasicAudio()\n"));

#ifdef I2S_AUDIO
    WriteByteTPI(TPI_AUDIO_INTERFACE_REG,  AUD_IF_I2S);                             // 0x26
    WriteByteTPI(TPI_AUDIO_HANDLING, 0x08 | AUD_DO_NOT_CHECK);          // 0x25
#else
    WriteByteTPI(TPI_AUDIO_INTERFACE_REG, AUD_IF_SPDIF);                    // 0x26 = 0x40
    WriteByteTPI(TPI_AUDIO_HANDLING, AUD_PASS_BASIC);                   // 0x25 = 0x00
#endif

#ifndef F_9022A_9334
            SetChannelLayout(TWO_CHANNELS);             // Always 2 channesl in S/PDIF
#else
            ReadClearWriteTPI(TPI_AUDIO_INTERFACE_REG, BIT_5); // Use TPI 0x26[5] for 9022A/24A and 9334 channel layout
#endif

#ifdef I2S_AUDIO
        // I2S - Map channels - replace with call to API MAPI2S
        WriteByteTPI(TPI_I2S_EN, 0x80); // 0x1F
//        WriteByteTPI(TPI_I2S_EN, 0x91);
//        WriteByteTPI(TPI_I2S_EN, 0xA2);
//        WriteByteTPI(TPI_I2S_EN, 0xB3);

        // I2S - Stream Header Settings - replace with call to API SetI2S_StreamHeader
        WriteByteTPI(TPI_I2S_CHST_0, 0x00); // 0x21
        WriteByteTPI(TPI_I2S_CHST_1, 0x00);
        WriteByteTPI(TPI_I2S_CHST_2, 0x00);
        WriteByteTPI(TPI_I2S_CHST_3, 0x02);
        WriteByteTPI(TPI_I2S_CHST_4, 0x02);

        // I2S - Input Configuration - replace with call to API ConfigI2SInput
        WriteByteTPI(TPI_I2S_IN_CFG, (0x10 | SCK_SAMPLE_EDGE)); //0x20
#endif

    WriteByteTPI(TPI_AUDIO_SAMPLE_CTRL, TWO_CHANNELS);  // 0x27 = 0x01
    SetAudioInfoFrames(TWO_CHANNELS, 0x00, 0x00, 0x00, 0x00);
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  ChangeVideoMode()
//
// PURPOSE       :  Changes the 9022/4 video resolution following a command from
//                  System Control
//
// INPUT PARAMS  :  API_Cmd type structure with command parameters, sent from
//                  the system controller
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  RxCommand
//
// RETURNS       :  Success message if video resolution changed successfully.
//                  Error Code if resolution change failed
//
// NOTE         :   Function calls InitVideo() with the 2nd parameter set to
//                  MODE_CHANGE (==0). That will initialized only the basic
//                  parameters (Pix. Clk; VFreq; H. # of pixels;...). Other
//                  parmeters will not be set by InitVideo(), and will be set
//                  by this function, based on the values passed in the
//                  API_Cmd typed parameter
//
//////////////////////////////////////////////////////////////////////////////
byte ChangeVideoMode(void)   
{

  	TPI_TRACE_PRINT((">>converts to tabel indexChangeVideoMode()\n"));

#ifdef DEV_SUPPORT_HDCP
    HDCP_Off();
#endif

    DisableTMDS();                  // turn off TMDS output
    DelayMS(T_RES_CHANGE_DELAY);    // allow control InfoFrames to pass through to the sink device.

	if (VideoModeDescription[Sil9136ChipSel].ColorDepth == VMD_COLOR_DEPTH_8BIT)
	{
		// Yes it is, so force 16bpps first!
		VideoModeDescription[Sil9136ChipSel].ColorDepth = VMD_COLOR_DEPTH_16BIT;
        InitVideo(((Arg(1) >> 6) & TWO_LSBITS));
		// Now put it back to 6bit and go do the expected InitVideo() call
		VideoModeDescription[Sil9136ChipSel].ColorDepth = VMD_COLOR_DEPTH_8BIT;
	}
		
    InitVideo(((Arg(1) >> 6) & TWO_LSBITS));  // Will set values based on VModesTable[Arg[0])

#ifdef F_9136                                        // Deep color for 9334 only
    if (((Arg(2) & BITS_7_6) >> 6) == DC_48)         // 16 bit Deep Color. Forcer 0x08[5] to 0 for half pixel mode
        Arg(1) &= ~BIT_5;
#endif


#ifndef F_9136
  	Arg(2) &= ~BITS_7_6;                            // No Deep Color in 9022A/24A, 9022/4
#endif

    WriteByteTPI(TPI_INPUT_FORMAT_REG, Arg(2));     // Write input formats to register 0x09

#ifdef F_9136  
	if (((Arg(2) & BITS_7_6) >> 6) >= DC_48)         // 16 bit Deep Color. Forcer 0x08[5] to 0 for half pixel mode
        ReadModifyWriteTPI(TPI_DEEP_COLOR_GCP, BIT_2, BIT_2); 
	else
		ReadModifyWriteTPI(TPI_DEEP_COLOR_GCP, BIT_2, 0x00); 
#endif


#ifdef DEV_SUPPORT_EDID
 	if (IsHDMI_Sink())                                                  // sink suppurts HDMI
    {
    	ReadSetWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, OUTPT_MODE_HDMI);  // Set 0x1A[0] to HDMI
    }
	else
	{
		ReadSetWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, OUTPT_MODE_DVI);   // Set 0x1A[0] to DVI
    }
#endif

	WriteByteTPI(TPI_PIX_REPETITION, Arg(1));      // 0x08

    WriteByteTPI(TPI_OUTPUT_FORMAT_REG, Arg(3));   // Write output color depth to registers 0x0A

#ifdef DEV_SUPPORT_EDID
	if (IsHDMI_Sink())
    {
		SetAVI_InfoFrames();                       // InfoFrames - only if output mode is HDMI

#ifdef F_9136
		// Setup the Vendor Specific Info Frame
		Set_VSIF();
#endif
	}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// PATCH!!!PATCH!!!PATCH!!!PATCH!!!PATCH!!!PATCH!!!PATCH!!!PATCH!!!PATCH!!!PATCH!!!PATCH!!!PATCH!!!
//

// THIS PATCH IS NEEDED BECAUSE SETTING UP AVI InfoFrames CLEARS 0x63 and 0x60[5]

 	WriteByteTPI(TPI_SYNC_GEN_CTRL, Arg(4));   // Set 0x60 according to Command.Arg[4]
                                               // THIS PATCH IS NEEDED BECAUSE SETTING UP AVI InfoFrames CLEARS 0x63 and 0x60[5]

    if(Arg(4) & MSBIT)                         // THIS PATCH IS NEEDED BECAUSE SETTING UP AVI InfoFrames CLEARS 0x63
	{
    	WriteByteTPI(TPI_DE_CTRL, EmbeddedSynPATCH[Sil9136ChipSel]); // (set in function SetEmbeddedSync())TO 0.
	}
//
// PATCH!!!PATCH!!!PATCH!!!PATCH!!!PATCH!!!PATCH!!!PATCH!!!PATCH!!!PATCH!!!PATCH!!!PATCH!!!PATCH!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#endif

 	// Command.Arg[7][3:2] -> 0x0E[7:6] Colorimetry
    ReadModifyWriteTPI(TPI_AVI_BYTE_2, BITS_7_6, Arg(7) << 4);

    // Command.Arg[7][6:4] -> 0x0F[6:4] Extended Colorimetry
    if ((Arg(7) & BITS_3_2) == SET_EX_COLORIMETRY)
    {
		ReadModifyWriteTPI(TPI_AVI_BYTE_3, BITS_6_5_4, Arg(7));
    }

    //==========================================================

#ifdef F_9022A_9334
 	// YC Input Mode Select - Command.Arg[6] - offset 0x0B
    WriteByteTPI(TPI_YC_Input_Mode, Arg(6));
#endif
		
	WriteByteTPI(TPI_SYNC_GEN_CTRL, Arg(4));

    EnableTMDS();

    return VIDEO_MODE_SET_OK;
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  SetAudioMode()
//
// PURPOSE       :  Changes the 9022/4 audio mode as defined by a command from
//                  the System Controller
//
// INPUT PARAMS  :  API_Cmd type structure with command parameters
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  None
//
// RETURNS       :  Success message if audio mode set successfully. Error
//                  Code if failed
//
//////////////////////////////////////////////////////////////////////////////
byte SetAudioMode (void)  
{
	byte  AudioMode;
    TPI_TRACE_PRINT((">>SetAudioMode()\n"));		   

	SetAudioMute(AUDIO_MUTE_MUTED);

	WriteByteTPI(TPI_AUDIO_INTERFACE_REG, Arg(0));

    if ((Arg(0) & TWO_MSBITS) == AUD_IF_SPDIF)                      // S/PDIF audio interface
    {
        ReadModifyWriteTPI(TPI_AUDIO_HANDLING, BITS_1_0, Arg(3));   // 0x25[1:0] - Audio Handling (S/PDIF only)

        ReadModifyWriteTPI(TPI_AUDIO_SAMPLE_CTRL, BITS_7_6 | BITS_5_4_3, Arg(1)); // Sample Frequency 0x27[5:3]; Sample Size 0x27[7:6]

#ifndef F_9022A_9334
		SetChannelLayout(TWO_CHANNELS);                                    // Always 2 channels in S/PDIF
#else
  		ReadClearWriteTPI(TPI_AUDIO_INTERFACE_REG, BIT_5);                 // Use TPI 0x26[5] for 9022A/24A and 9334 channel layout
#endif
   	}
 	else                                                        // I2S or DSD Audio interface
  	{
		WriteByteTPI(TPI_AUDIO_SAMPLE_CTRL, Arg(1));   // 0x27 - HBR; Sample Frequency; Sample Size

#ifndef F_9022A_9334
     	SetChannelLayout(Arg(0) & BIT_5);
#else

		AudioMode = Arg(0) & TWO_MSBITS; 
        if ((AudioMode == AUD_IF_DSD) || (AudioMode == AUD_IF_I2S))
		{
			// Set user's channel layout selection to TPI 0x26[5]
			ReadModifyWriteTPI(TPI_AUDIO_INTERFACE_REG, BIT_5, Arg(0)); 
		}

#endif

#ifndef F_9022A_9334                                           // 9022/24 only.
  		SetInputWordLength(Arg(4) & LOW_NIBBLE);       
#endif
  	}

	SetAudioMute(AUDIO_MUTE_NORMAL);

	//channel count, coding type, sample size, sample freq, and spk config.	
	SetAudioInfoFrames(Arg(5) & THREE_LSBITS, Arg(0) & FOUR_LSBITS, Arg(1) & BITS_7_6, Arg(1) & BITS_5_4_3, Arg(2)); 	

	ReadModifyWriteTPI(TPI_AUDIO_INTERFACE_REG, BIT_4, Arg(0));		// Set audio mute/unmute bit.

#ifdef DEV_SUPPORT_HDCP
    HDCP_Off();
#endif

	DisableTMDS();                  
	EnableTMDS();

	return AUDIO_MODE_SET_OK;    
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  MapI2S()
//
// PURPOSE       :  Changes the 9022/4 I2S channel mapping as defined by a
//                  command sent from the System Controller
//
// INPUT PARAMS  :  API_Cmd type structure with command parameters
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  None
//
// RETURNS       :  Success message if channel mapping successful. Error
//                  Code if failed
//
//////////////////////////////////////////////////////////////////////////////
byte MapI2S(void)  
{
    byte B_Data;
	int i;
	// byte temp;

        TPI_TRACE_PRINT((">>MapI2S()\n"));

    B_Data = ReadByteTPI(TPI_AUDIO_INTERFACE_REG);

    if ((B_Data & TWO_MSBITS) != AUD_IF_I2S)    // 0x26 not set to I2S interface
    {
        return I2S_NOT_SET;
    }

//      WriteByteTPI(TPI_I2S_EN, Command.Arg[0]);

//      TPI_TRACE_PRINT(("I2sMapping:\n"));

		for (i = 0; i < I2S_MAPPING_CMD_LEN; i++)
        {
			WriteByteTPI(TPI_I2S_EN, Arg(i));

//			temp = ReadByteTPI(TPI_I2S_EN);
//         	TPI_TRACE_PRINT(("    FIFO#%d = 0x%2X\n", i, (int)temp));

			if ((Arg(i+1) & BITS_1_0) == 0)
				return 0;
        }
    return I2S_MAPPING_SUCCESSFUL;
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  ConfigI2SInput()
//
// PURPOSE       :  Sets the 9022/4 I2S channel bit direction, justification
//                  and polarity as defined by acommand sent from the System
//                  Controller
//
// INPUT PARAMS  :  API_Cmd type structure with command parameters
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  None
//
// RETURNS       :  Success message if video I2S channels configuredsuccessfully.
//                  Error Code if setting failed
//
//////////////////////////////////////////////////////////////////////////////
byte ConfigI2SInput(void)  
{
    byte B_Data;

        TPI_TRACE_PRINT((">>ConfigI2SInput()\n"));

    B_Data = ReadByteTPI(TPI_AUDIO_INTERFACE_REG);

    if ((B_Data & TWO_MSBITS) != AUD_IF_I2S)    // 0x26 not set to I2S interface
    {
        return I2S_NOT_SET;
    }

    WriteByteTPI(TPI_I2S_IN_CFG, Arg(0));

    return I2S_INPUT_CONFIG_SUCCESSFUL;
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  SetI2S_StreamHeader()
//
// PURPOSE       :  Sets the 9022/4 I2S Channel Status bytes, as defined by
//                  a command sent from the System Controller
//
// INPUT PARAMS  :  API_Cmd type structure with command parameters
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  None
//
// RETURNS       :  Success message if stream header set successfully. Error
//                  Code if failed
//
//////////////////////////////////////////////////////////////////////////////
byte SetI2S_StreamHeader(void) 
{
    byte B_Data;
    int i;

        TPI_TRACE_PRINT((">>SetI2S_StreamHeader()\n"));

    B_Data = ReadByteTPI(TPI_AUDIO_INTERFACE_REG);

    if ((B_Data & TWO_MSBITS) != AUD_IF_I2S)    // 0x26 not set to I2S interface
    {
        return I2S_NOT_SET;
    }

    for (i = 0; i < CommandLength; i++)
    {
        WriteByteTPI(TPI_I2S_CHST_0 + i, Arg(i));
    }

    return I2S_HEADER_SET_SUCCESSFUL;
}


#ifdef F_9022A_9334
//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   SetGBD_InfoFrame(()
//
// PURPOSE      :   Sets and sends the the 9022A/4A GBD InfoFrames according
//                  to data sent from the System Controller
//
// INPUT PARAMS :   None
//
// OUTPUT PARAMS:   None
//
// GLOBALS USED :   None
//
// RETURNS      :   Success message if GBD packet set successfully. Error
//                  Code if failed
//
// NOTE         :   Currently this function is a place holder. It always
//                  returns a Success message
//
//////////////////////////////////////////////////////////////////////////////
byte SetGBD_InfoFrame(void)
{
        byte CheckSum;

        TPI_TRACE_PRINT((">>SetGBD_InfoFrame()\n"));

        // Set MPEG InfoFrame Header to GBD InfoFrame Header values:
        WriteByteTPI(MISC_INFO_FRAMES_CTRL, MPEG_INFOFRAME_CODE);                       // 0xBF = Use MPEG      InfoFrame for GBD - 0x03
        WriteByteTPI(MISC_INFO_FRAMES_TYPE, TYPE_GBD_INFOFRAME);                        // 0xC0 = 0x0A
        WriteByteTPI(MISC_INFO_FRAMES_VER, NEXT_FIELD | GBD_PROFILE | AFFECTED_GAMUT_SEQ_NUM);   // 0x0C1 = 0x81
        WriteByteTPI(MISC_INFO_FRAMES_LEN, ONLY_PACKET | CURRENT_GAMUT_SEQ_NUM);                                // 0x0C2 = 0x31

        CheckSum = TYPE_GBD_INFOFRAME +
                           NEXT_FIELD +
                           GBD_PROFILE +
                           AFFECTED_GAMUT_SEQ_NUM +
                           ONLY_PACKET +
                           CURRENT_GAMUT_SEQ_NUM;

        CheckSum = 0x100 - CheckSum;

        WriteByteTPI(MISC_INFO_FRAMES_CTRL, EN_AND_RPT_MPEG);  // Enable and Repeat MPEG InfoFrames
        WriteByteTPI(MISC_INFO_FRAMES_CHKSUM, CheckSum);                        // 0X00 - Send header only

        return GBD_SET_SUCCESSFULLY;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  SetAVI_InfoFrames()
//
// PURPOSE       :  Load AVI InfoFrame data into registers and send to sink
//
// INPUT PARAMS  :  An API_Cmd parameter that holds the data to be sent
//                  in the InfoFrames
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  None
//
// RETURNS       :  TRUE
//
// Note:          : Infoframe contents are from spec CEA-861-D
//
//////////////////////////////////////////////////////////////////////////////
#if defined(MDIN_AVI_INFO_CTRL)
bool SetAVI_InfoFrames(void)  
{
    byte B_Data[SIZE_AVI_INFOFRAME];
    byte i;

    TPI_TRACE_PRINT((">>SetAVI_InfoFrames() - VIC:%d, Y:%d\n", stInfoframe.VIC, stInfoframe.Y));

    for (i = 0; i < SIZE_AVI_INFOFRAME; i++)
        B_Data[i] = 0;

	B_Data[1] = stInfoframe.Y<<5;
	B_Data[2] = 0;
	B_Data[3] = 0;
	B_Data[4] = stInfoframe.VIC;
	B_Data[5] = 0;

	// Calculate AVI InfoFrame ChecKsum
    B_Data[0] = 0x82 + 0x02 +0x0D;
    for (i = 1; i < SIZE_AVI_INFOFRAME; i++)
        B_Data[0] += B_Data[i];
    B_Data[0] = 0x100 - B_Data[0];

	// Write the Inforframe data to the TPI Infoframe registers
    WriteBlockTPI(TPI_AVI_BYTE_0, SIZE_AVI_INFOFRAME, B_Data);
	WriteByteTPI(TPI_END_RIGHT_BAR_MSB, 0x00);	    // Set last byte of TPI AVI InfoFrame for TPI AVI I/O Format to take effect

	if(IsOUT_MainSyncEmb())			{SetEmbeddedSync();		EnableEmbeddedSync();}
#ifdef DEV_EMBEDDED
    SetEmbeddedSync();
    EnableEmbeddedSync();
#endif

	// touch output to refresh sink
    ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, TMDS_OUTPUT_CONTROL_MASK, TMDS_OUTPUT_CONTROL_POWER_DOWN);
    ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, TMDS_OUTPUT_CONTROL_MASK, TMDS_OUTPUT_CONTROL_ACTIVE);
    return TRUE;
}

#else  // defined(MDIN_AVI_INFO_CTRL)
bool SetAVI_InfoFrames(void)  
{
    byte B_Data[SIZE_AVI_INFOFRAME];
    byte i;
    byte TmpVal;

    TPI_TRACE_PRINT((">>SetAVI_InfoFrames()\n"));

    for (i = 0; i < SIZE_AVI_INFOFRAME; i++)
        B_Data[i] = 0;

    if (CommandLength > VIDEO_SETUP_CMD_LEN)    // Command length > 9. AVI InfoFrame is set by the host
    {
        for (i = 1; i < CommandLength - VIDEO_SETUP_CMD_LEN; i++)
            B_Data[i] = Arg(VIDEO_SETUP_CMD_LEN + i - 1);
    }
    else                                                // Command length == 7. AVI InfoFrame is set by the FW
    {
        if ((Arg(3) & TWO_LSBITS) == 1)         // AVI InfoFrame DByte1
            TmpVal = 2;
        else if ((Arg(3) & TWO_LSBITS) == 2)
            TmpVal = 1;
        else
            TmpVal = 0;

            B_Data[1] = (TmpVal << 5) & BITS_OUT_FORMAT;                    // AVI Byte1: Y1Y0 (output format)

            if (((Arg(6) >> 2) & TWO_LSBITS) == 3)                  // Extended colorimetry - xvYCC
            {
                B_Data[2] = 0xC0;                                           // Extended colorimetry info (B_Data[3] valid (CEA-861D, Table 11)

                if (((Arg(6) >> 4) & THREE_LSBITS) == 0)            // xvYCC601
                    B_Data[3] &= ~BITS_6_5_4;

                else if (((Arg(6) >> 4) & THREE_LSBITS) == 1)       // xvYCC709
                    B_Data[3] = (B_Data[3] & ~BITS_6_5_4) | BIT_4;
            }

            else if (((Arg(6) >> 2) & TWO_LSBITS) == 2)             // BT.709
                B_Data[2] = 0x80;                                           // AVI Byte2: C1C0

            else if (((Arg(6) >> 2) & TWO_LSBITS) == 1)             // BT.601
                B_Data[2] = 0x40;                                           // AVI Byte2: C1C0

			else															// Carries no data
			{																// AVI Byte2: C1C0
				B_Data[2] &= ~BITS_7_6;										// colorimetry = 0
				B_Data[3] &= ~BITS_6_5_4;									// Extended colorimetry = 0
			}

            VModeTblIndex[Sil9136ChipSel] = ConvertVIC_To_VM_Index();

			B_Data[4] = VideoModeDescription[Sil9136ChipSel].VIC;

			//  Set the Aspect Ration info into the Infoframe Byte 2
			B_Data[2] |= _4_To_3;                           // AVI Byte4: VIC

            B_Data[2] |= SAME_AS_AR;                        // AVI Byte2: R3..R1 - Set to "Same as Picture Aspect Ratio"
            B_Data[5] = VModesTable[VModeTblIndex[Sil9136ChipSel]].PixRep;      // AVI Byte5: Pixel Replication - PR3..PR0
    }

	// Calculate AVI InfoFrame ChecKsum
    B_Data[0] = 0x82 + 0x02 +0x0D;
    for (i = 1; i < SIZE_AVI_INFOFRAME; i++)
    {
        B_Data[0] += B_Data[i];
    }
    B_Data[0] = 0x100 - B_Data[0];

	// Write the Inforframe data to the TPI Infoframe registers
    WriteBlockTPI(TPI_AVI_BYTE_0, SIZE_AVI_INFOFRAME, B_Data);

#ifdef DEV_EMBEDDED
    EnableEmbeddedSync();
#endif

    return TRUE;
}
#endif


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  SetAudioInfoFrames()
//
// PURPOSE       :  Load Audio InfoFrame data into registers and send to sink
//
// INPUT PARAMS  :  (1) Channel count (2) speaker configuration per CEA-861D
//                  Tables 19, 20 (3) Coding type: 0x09 for DSD Audio. 0 (refer
//                                      to stream header) for all the rest (4) Sample Frequency. Non
//                                      zero for HBR only (5) Audio Sample Length. Non zero for HBR
//                                      only.
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  None
//
// RETURNS       :  TRUE
//
//////////////////////////////////////////////////////////////////////////////
bool SetAudioInfoFrames(byte ChannelCount, byte CodingType, byte SS, byte Fs, byte SpeakerConfig)
{
    byte B_Data[SIZE_AUDIO_INFOFRAME];  // 14
    byte i;

    TPI_TRACE_PRINT((">>SetAudioInfoFrames()\n"));

    for (i = 0; i < SIZE_AUDIO_INFOFRAME +1; i++)
        B_Data[i] = 0;

    B_Data[0] = EN_AUDIO_INFOFRAMES;        // 0xC2
    B_Data[1] = TYPE_AUDIO_INFOFRAMES;      // 0x84
    B_Data[2] = AUDIO_INFOFRAMES_VERSION;   // 0x01
    B_Data[3] = AUDIO_INFOFRAMES_LENGTH;    // 0x0A

    B_Data[5] = ChannelCount;               // 0 for "Refer to Stream Header" or for 2 Channels. 0x07 for 8 Channels
	B_Data[5] |= (CodingType << 4);                 // 0xC7[7:4] == 0b1001 for DSD Audio
    B_Data[4] = 0x84 + 0x01 + 0x0A;         // Calculate checksum

//    B_Data[6] = (Fs << 2) | SS;
	B_Data[6] = (Fs >> 1) | (SS >> 6);

	//write Fs to 0x27[5:3] and SS to 0x27[7:6] to update the IForm with the current value.
//	ReadModifyWriteTPI(TPI_AUDIO_SAMPLE_CTRL, BITS_7_6 | BITS_5_4_3, (B_Data[6] & BITS_1_0) << 6 | (B_Data[6] & 0x1C) << 1);

	B_Data[8] = SpeakerConfig;

    for (i = 5; i < SIZE_AUDIO_INFOFRAME; i++)
        B_Data[4] += B_Data[i];

    B_Data[4] = 0x100 - B_Data[4];
	g_audio_Checksum[Sil9136ChipSel] = B_Data[4];	// Audio checksum for global use

    WriteBlockTPI(TPI_AUDIO_BYTE_0, SIZE_AUDIO_INFOFRAME, B_Data);
	#ifdef DEV_EMBEDDED
    EnableEmbeddedSync();
	#endif
    return TRUE;
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  Set_VSIF()
//
// PURPOSE       :  Construct Vendor Specific InfoFrame for3D support. use
//                                      MPEG InfoFrame
//
// INPUT PARAMS  :
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  VideoModeDescription
//
// RETURNS       :  void
//
//////////////////////////////////////////////////////////////////////////////
#ifdef DEV_SUPPORT_EDID
void Set_VSIF(void)
{
    byte i;
    byte Data[16];
	   
	// Header Bytes
    Data[0] = VSIF_TYPE;                // HB0 Packet Type 0x81
    Data[1] = VSIF_VERSION;             // HB1 Version = 0x01
    Data[2] = VSIF_LEN;                 // HB2 Length = 7

	// Packet Bytes
    Data[3] = VSIF_TYPE+                // PB0 partial checksum
              VSIF_VERSION+
              VSIF_LEN;

	// PB1 - PB3 contain the 24bit IEEE Registration Identifier
    Data[4] = 0x03;                     // HDMI Signature LS Byte
    Data[5] = 0x0C;                     // HDMI Signature middle byte
    Data[6] = 0x00;                     // HDMI Signature MS Byte

	// PB4 - HDMI_Video_Format into bits 7:5
    Data[7] = VideoModeDescription[Sil9136ChipSel].HDMIVideoFormat << 5;

	// PB5 - Depending on the video format, this byte will contain either the HDMI_VIC
	// code in buts 7:0, OR the 3D_Structure in bits 7:4.
	switch(VideoModeDescription[Sil9136ChipSel].HDMIVideoFormat)
	{
		case VMD_HDMIFORMAT_HDMI_VIC:
			// This is a 2x4K mode, set the HDMI_VIC in buts 7:0.  Values
			// are from HDMI 1.4 Spec, 8.2.3.1 (Table 8-13).
			Data[8] = VideoModeDescription[Sil9136ChipSel].VIC;
			Data[9] = 0;
			break;

		case VMD_HDMIFORMAT_3D:
			// This is a 3D mode, set the 3D_Structure in buts 7:4
			// Bits 3:0 are reseved so set to 0.  Values are from HDMI 1.4
			// Spec, Appendix H (Table H-2).
			Data[8] = VideoModeDescription[Sil9136ChipSel].ThreeDStructure << 4;
			// Add the Extended data field when the 3D format is Side-by-Side(Half).
			// See Spec Table H-3 for details.
			Data[9] = 0;	// %%%%% TODO  See Bug#19539
			break;

		case VMD_HDMIFORMAT_CEA_VIC:
		default:
			Data[8] = 0;
			Data[9] = 0;
			break;
	}

	// Complete the checksum with PB1 through PB7
    for (i = 4; i < 10; i++)
	{
        Data[3] += Data[i];
	}
    Data[3] %= 0x100;
    Data[3] = 100 - Data[3];            // Final checksum

    WriteByteTPI(MISC_INFO_FRAMES_CTRL, EN_AND_RPT_MPEG);         // Enable and Repeat MPEG InfoFrames
    WriteBlockTPI(MISC_INFO_FRAMES_TYPE, 10, Data);               // Write VSIF to MPEG registers and start transmission
}
#endif

#endif	//__USE_SIL9136_TX__
