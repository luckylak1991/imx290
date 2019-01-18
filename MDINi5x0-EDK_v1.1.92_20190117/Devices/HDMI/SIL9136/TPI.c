/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

// Standard C Library
#include <stdio.h>
#include "common.h"
#include "sil9136.h"

// Needs to be rearranged and minimized
#include "defs.h"
#include "TypeDefs.h"
#include "Constants.h"
//#include "gpio.h"
#include "externals.h"
#include "Macros.h"
#include "TPI_Regs.h"
#include "TPI_Access.h"
#include "TPI.h"
//#include "util.h"
#include "AV_Config.h"
//#include "i2c_master_sw.h"
//#include "si_apiConfigure.h"



#if defined(__USE_SIL9136_TX__)


// Serial Communication Buffer assignments and macros
#define CommandLength				g_CommData[Sil9136ChipSel][1]
#define Arg(a)                      g_CommData[Sil9136ChipSel][(2+a)]
#define IS_CEC 0

// May be empty depending on project definitions
//#include "EDID.h"
//#include "HDCP.h"

#ifdef F_9136
//#include "ehdmi.h"
#endif

extern void InitSamabeth(void); //%%%%%%%%%%%%%%%%

#if (IS_CEC == 1)
extern bool SI_CecInit (void);
extern byte SI_CecHandler (byte, bool);
#endif

static void TxHW_Reset (void);
static bool StartTPI (void);

//static void WakeUpFromD3 (void);
static void InitializeStateVariables (void);

static void HotPlugService (void);

static bool EnableInterrupts (byte);
static bool DisableInterrupts (byte);

static bool SetPreliminaryInfoFrames (void);

static void OnHdmiCableConnected (void);
static void OnHdmiCableDisconnected (void);

static void OnDownstreamRxPoweredDown (void);
static void OnDownstreamRxPoweredUp (void);

static void TxPowerStateD0 (void);
#ifdef F_9136
static void TxPowerStateD3 (void);
#endif

#ifdef RCP_ENABLE
extern void CpCbusInitialize(void);
extern void CpCbusHandler(void);
#endif


#define T_EN_TPI       	10
#define T_HPD_DELAY    	10

static byte txPowerState[SIL9136_CHIP_NUM]={0,};
static bool tmdsPoweredUp[SIL9136_CHIP_NUM]={0,};
static bool hdmiCableConnected[SIL9136_CHIP_NUM]={0,};
static bool dsRxPoweredUp[SIL9136_CHIP_NUM]={0,};

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      : TPI_Init()
//
// PURPOSE       : TPI initialization: HW Reset, Interrupt enable.
//
// INPUT PARAMS  : None
//
// OUTPUT PARAMS : void
//
// GLOBALS USED  : None
//
// RETURNS      :   TRUE
//
//////////////////////////////////////////////////////////////////////////////
bool TPI_Init(void)
{
	int i = 0;
	TPI_TRACE_PRINT((">>TPI_Init()\n"));

	TPI_TRACE_PRINT(("TPI Firmware Version "));
	for (i=0; i < (int)sizeof(TPI_FW_VERSION); i++)
	{
		TPI_TRACE_PRINT(("%c", TPI_FW_VERSION[i]));
	}
	TPI_TRACE_PRINT(("\n"));

	InitializeStateVariables();

	TxHW_Reset();									// Toggle TX reset pin

	if (StartTPI())									// Enable HW TPI mode, check device ID
	{
#ifdef DEV_SUPPORT_HDCP
		g_HDCP_Override[Sil9136ChipSel] = FALSE;
		HDCP_Init();
#endif

#ifdef DEV_SUPPORT_EHDMI
		EHDMI_Init();
#endif


#if (IS_CEC == 1)
	    SI_CecInit();
#endif

#ifdef RCP_ENABLE
		TPI_DEBUG_PRINT (("\ncalling CpCbusInitialize\n\n"));
		CpCbusInitialize();
#endif
		
		// Interrupt Operation 'INT signal'
		ReadModifyWriteIndexedRegister(INDEXED_PAGE_0, 0x7B, 0x04, 0x00);	// [2]INT_output, 0:push-pull, 1:open-drain(default)
		//ReadModifyWriteIndexedRegister(INDEXED_PAGE_0, 0x7B, 0x02, 0x02);	// [1]INT_polarity, 0:active-high, 1:active-low(default)
		
		return TRUE;
	}

	return FALSE;
}


/*static void WakeUpFromD3 (void)
{

	TPI_DEBUG_PRINT(("Waking up...\n"));

	InitializeStateVariables ();
	TXHAL_InitPostReset();
	//WriteInitialRegisterValues();
#if (IS_CEC == 1)
    SI_CecInit();
#endif
}*/


static void InitializeStateVariables (void)
{
	txPowerState[Sil9136ChipSel] = TX_POWER_STATE_D0;				// Chip powers up in D2 mode, but init to D0 for testing prupose.
	tmdsPoweredUp[Sil9136ChipSel] = FALSE;
	hdmiCableConnected[Sil9136ChipSel] = FALSE;
	dsRxPoweredUp[Sil9136ChipSel] = FALSE;

	VideoModeDescription[Sil9136ChipSel].VIC = stInfoframe.VIC/*97*/;
	VideoModeDescription[Sil9136ChipSel].AspectRatio = 2/*R_16*/;
	VideoModeDescription[Sil9136ChipSel].ColorSpace = stInfoframe.Y/*VMD_COLOR_SPACE_RGB*/;
	VideoModeDescription[Sil9136ChipSel].ColorDepth = VMD_COLOR_DEPTH_10BIT;
	VideoModeDescription[Sil9136ChipSel].HDCPAuthenticated = VMD_HDCP_NOT_AUTHENTICATED;
	VideoModeDescription[Sil9136ChipSel].HDMIVideoFormat = VMD_HDMIFORMAT_CEA_VIC;
	VideoModeDescription[Sil9136ChipSel].ThreeDStructure = SIDE_BY_SIDE_HALF;  // ?

#ifdef DEV_SUPPORT_EDID
	edidDataValid = FALSE;							// Move this into EDID_Init();
#endif
}

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   HotPlugService()
//
// PURPOSE      :   Implement Hot Plug Service Loop activities
//
// INPUT PARAMS :   None
//
// OUTPUT PARAMS:   void
//
// GLOBALS USED :   LinkProtectionLevel
//
// RETURNS      :   An error code that indicates success or cause of failure
//
//////////////////////////////////////////////////////////////////////////////

void HotPlugService (void)
{
	TPI_TRACE_PRINT((">>HotPlugService()\n"));

	DisableInterrupts(0xFF);

#ifndef IS_VMODE_SIM
  #if defined(MDIN_AVI_INFO_CTRL)
	//VideoModeDescription[Sil9136ChipSel].VIC = stInfoframe.VIC;
	SIL9136_ConvVICtoHDVFrmt(stInfoframe.VIC);
  #else
	VideoModeDescription[Sil9136ChipSel].VIC = EDID_Data.VideoDescriptor[0];					// use 1st mode supported by sink
  #endif
	// workaround for Bug#18128
	if (VideoModeDescription[Sil9136ChipSel].ColorDepth == VMD_COLOR_DEPTH_8BIT)
	{
		// Yes it is, so force 16bpps first!
		VideoModeDescription[Sil9136ChipSel].ColorDepth = VMD_COLOR_DEPTH_16BIT;
		InitVideo(X1);
		// Now put it back to 8bit and go do the expected InitVideo() call
		VideoModeDescription[Sil9136ChipSel].ColorDepth = VMD_COLOR_DEPTH_8BIT;
	}
	// end workaround
#endif // IS_VMODE_SIM


	InitVideo(X1);						// Set PLL Multiplier to x1 upon power up

#ifndef POWER_STATE_D0_AFTER_TMDS_ENABLE
	TxPowerStateD0();
#endif

	if (IsHDMI_Sink())											// Set InfoFrames only if HDMI output mode
	{
		SetPreliminaryInfoFrames();
		SetBasicAudio();										// set audio interface to basic audio (an external command is needed to set to any other mode
	}
	else
	{
		SetAudioMute(AUDIO_MUTE_MUTED);
	}

#ifdef DEV_SUPPORT_EHDMI
//	EHDMI_ARC_Common_Enable();
	EHDMI_ARC_Common_With_HEC_Enable();
#endif

#ifdef DEV_SUPPORT_HDCP
	if ((HDCP_TxSupports == TRUE) && (VideoModeDescription[Sil9136ChipSel].HDCPAuthenticated == VMD_HDCP_AUTHENTICATED))
	{
		if (HDCP_AksvValid == TRUE)
		{
			// AV MUTE
			TPI_DEBUG_PRINT (("TMDS -> Enabled (Video Muted)\n"));
			ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, LINK_INTEGRITY_MODE_MASK | TMDS_OUTPUT_CONTROL_MASK
					| AV_MUTE_MASK, LINK_INTEGRITY_DYNAMIC | TMDS_OUTPUT_CONTROL_ACTIVE | AV_MUTE_MUTED);
			tmdsPoweredUp[Sil9136ChipSel] = TRUE;

			EnableInterrupts(HOT_PLUG_EVENT | RX_SENSE_EVENT | AUDIO_ERROR_EVENT | SECURITY_CHANGE_EVENT | V_READY_EVENT | HDCP_CHANGE_EVENT);
		}		

	}
	else
#endif
	{
		TPI_DEBUG_PRINT (("TMDS -> Enabled\n"));
		ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, LINK_INTEGRITY_MODE_MASK | TMDS_OUTPUT_CONTROL_MASK | AV_MUTE_MASK, LINK_INTEGRITY_DYNAMIC | TMDS_OUTPUT_CONTROL_ACTIVE | AV_MUTE_NORMAL);
		EnableInterrupts(HOT_PLUG_EVENT | RX_SENSE_EVENT | AUDIO_ERROR_EVENT | V_READY_EVENT);
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD2__)
		//SIL9136_SetPollingStop();
#endif
	}

#ifdef POWER_STATE_D0_AFTER_TMDS_ENABLE
	TxPowerStateD0();
#endif

	//ReadModifyWriteTPI(TPI_END_RIGHT_BAR_MSB, 0x00, 0x00);
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      : HW_Reset()
//
// PURPOSE       : Send a
//
// INPUT PARAMS  : None
//
// OUTPUT PARAMS : void
//
// GLOBALS USED  : None
//
// RETURNS       : Void
//
//////////////////////////////////////////////////////////////////////////////

static void TxHW_Reset(void)
{
	I2C_BusID bus_id=(Sil9136ChipSel==0)?SIL9136TX1_BUSID:SIL9136TX2_BUSID;

	// Does this need to be done for every chip? Should it be moved into TXHAL_InitPostReset() for each applicable device?
	I2C_ByteWrite(bus_id, TPI_BASE_ADDR, 0x7C, 0x14);					// HW debounce to 64ms (0x14)
#ifdef F_9136
	I2C_ByteWrite(bus_id, TPI_BASE_ADDR, 0x82, 0xA5);
#endif
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      : StartTPI()
//
// PURPOSE       : Start HW TPI mode by writing 0x00 to TPI address 0xC7. This
//                 will take the Tx out of power down mode.
//
// INPUT PARAMS  : None
//
// OUTPUT PARAMS : void
//
// GLOBALS USED  : None
//
// RETURNS       : TRUE if HW TPI started successfully. FALSE if failed to.
//
//////////////////////////////////////////////////////////////////////////////

static bool StartTPI(void)
{
	byte devID = 0x00;
        word wID = 0x0000;

        TPI_TRACE_PRINT((">>StartTPI()\n"));

    WriteByteTPI(TPI_ENABLE, 0x00);            // Write "0" to 72:C7 to start HW TPI mode
        DelayMS(100);

        devID = ReadIndexedRegister(INDEXED_PAGE_0, 0x03);
        wID = devID;
        wID <<= 8;
        devID = ReadIndexedRegister(INDEXED_PAGE_0, 0x02);
        wID |= devID;

    devID = ReadByteTPI(TPI_DEVICE_ID);

        TPI_TRACE_PRINT(("devID : 0x%04X\n", (int) wID));

        if (devID == SiI_DEVICE_ID) {
        //TPI_TRACE_PRINT((SiI_DEVICE_STRING));
        return TRUE;
        }

    TPI_TRACE_PRINT(("Unsupported TX\n"));
    return FALSE;
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  EnableInterrupts()
//
// PURPOSE       :  Enable the interrupts specified in the input parameter
//
// INPUT PARAMS  :  A bit pattern with "1" for each interrupt that needs to be
//                  set in the Interrupt Enable Register (TPI offset 0x3C)
//
// OUTPUT PARAMS :  void
//
// GLOBALS USED  :  None
//
// RETURNS       :  TRUE
//
//////////////////////////////////////////////////////////////////////////////
bool EnableInterrupts(byte Interrupt_Pattern)
{
    TPI_TRACE_PRINT((">>EnableInterrupts()\n"));
    ReadSetWriteTPI(TPI_INTERRUPT_ENABLE_REG, Interrupt_Pattern);

    return TRUE;
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  DisableInterrupts()
//
// PURPOSE       :  Enable the interrupts specified in the input parameter
//
// INPUT PARAMS  :  A bit pattern with "1" for each interrupt that needs to be
//                  cleared in the Interrupt Enable Register (TPI offset 0x3C)
//
// OUTPUT PARAMS :  void
//
// GLOBALS USED  :  None
//
// RETURNS       :  TRUE
//
//////////////////////////////////////////////////////////////////////////////
static bool DisableInterrupts(byte Interrupt_Pattern)
{
        TPI_TRACE_PRINT((">>DisableInterrupts()\n"));
    ReadClearWriteTPI(TPI_INTERRUPT_ENABLE_REG, Interrupt_Pattern);

    return TRUE;
}


#if defined HAS_CTRL_BUS
static bool CBUS_Discovery (void)
{
    byte i;

    for (i = 0; i < 20; i++)
    {
        WriteByteTPI (TPI_DEVICE_POWER_STATE_CTRL_REG, CTRL_PIN_DRIVEN_TX_BRIDGE | TX_POWER_STATE_D0);                  // Start CBUS self-discovery
        DelayMS (T_CBUSDISCOVERY_DELAY);

        if (ReadByteCBUS(0x0A) & 0x01)
        {
            TPI_DEBUG_PRINT (("CBUS discovered in %d attempt(s).\n", (int)(i + 1)));
             return TRUE;
        }

        WriteByteTPI (TPI_DEVICE_POWER_STATE_CTRL_REG, CTRL_PIN_TRISTATE | TX_POWER_STATE_D0);
        DelayMS (T_CBUSDISCOVERY_DELAY);
    }

    TPI_DEBUG_PRINT (("CBUS downstream device not detected.\n0xC8:0x0A = %02X\n", (int)ReadByteCBUS(0x0A)));
    return FALSE;
}
#endif


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      : SetPreliminaryInfoFrames()
//
// PURPOSE       : Set InfoFrames for default (initial) setup only
//
// INPUT PARAMS  : VIC, output mode,
//
// OUTPUT PARAMS : void
//
// GLOBALS USED  : None
//
// RETURNS       : TRUE
//
//////////////////////////////////////////////////////////////////////////////
static bool SetPreliminaryInfoFrames()
{
    byte i;
    //API_Cmd Command;        // to allow using function SetAVI_InfoFrames()

        TPI_TRACE_PRINT((">>SetPreliminaryInfoFrames()\n"));

    for (i = 0; i < MAX_COMMAND_ARGUMENTS; i++)
        Arg(i) = 0;

        CommandLength = 0;      // fixes SetAVI_InfoFrames() from going into an infinite loop

    Arg(0) = VideoModeDescription[Sil9136ChipSel].VIC;

#ifdef DEV_SUPPORT_EDID
        if (EDID_Data.YCbCr_4_4_4)
        {
                Arg(3) = 0x01;
        }
        else
        {
                if (EDID_Data.YCbCr_4_2_2)
                {
                        Arg(3) = 0x02;
                }
        }
#else
        Arg(3) = 0x00;
#endif

    SetAVI_InfoFrames();
    return TRUE;
}


static void TxPowerStateD0 (void)
{

	ReadModifyWriteTPI(TPI_DEVICE_POWER_STATE_CTRL_REG, TX_POWER_STATE_MASK, TX_POWER_STATE_D0);
	TPI_DEBUG_PRINT(("TX Power State D0\n"));
	txPowerState[Sil9136ChipSel] = TX_POWER_STATE_D0;
}


#ifdef F_9136
static void TxPowerStateD3 (void)
{
		ReadModifyWriteTPI(TPI_DEEP_COLOR_GCP, BIT_0, ENABLE);
		DelayMS(10);
		ReadModifyWriteTPI(TPI_DEEP_COLOR_GCP, BIT_0, DISABLE);

	ReadModifyWriteTPI(TPI_DEVICE_POWER_STATE_CTRL_REG, TX_POWER_STATE_MASK, TX_POWER_STATE_D3);

	TPI_DEBUG_PRINT(("TX Power State D3\n"));
	txPowerState[Sil9136ChipSel] = TX_POWER_STATE_D3;
}
#endif

void EnableTMDS (void)
{

    TPI_DEBUG_PRINT(("TMDS -> Enabled\n"));
    ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, TMDS_OUTPUT_CONTROL_MASK, TMDS_OUTPUT_CONTROL_ACTIVE);
    tmdsPoweredUp[Sil9136ChipSel] = TRUE;
	TPI_DEBUG_PRINT(("tmdsPoweredUp:%d\n", tmdsPoweredUp[Sil9136ChipSel]));
}


void DisableTMDS (void)
{

    TPI_DEBUG_PRINT(("TMDS -> Disabled\n"));

    // AV MUTE
    ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, TMDS_OUTPUT_CONTROL_MASK |
		    	AV_MUTE_MASK, TMDS_OUTPUT_CONTROL_POWER_DOWN | AV_MUTE_MUTED);
    tmdsPoweredUp[Sil9136ChipSel] = FALSE;
	TPI_DEBUG_PRINT(("tmdsPoweredUp:%d\n", tmdsPoweredUp[Sil9136ChipSel]));
}


#ifdef DEV_SUPPORT_HDCP
void RestartHDCP (void)
{
	TPI_DEBUG_PRINT (("HDCP -> Restart\n"));

	DisableTMDS();
	HDCP_Off();
	EnableTMDS();
}
#endif

void SetAudioMute (byte audioMute)
{
	ReadModifyWriteTPI(TPI_AUDIO_INTERFACE_REG, AUDIO_MUTE_MASK, audioMute);
}

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   TPI_Poll ()
//
// PURPOSE      :   Poll Interrupt Status register for new interrupts
//
// INPUT PARAMS :   None
//
// OUTPUT PARAMS:   None
//
// GLOBALS USED :   LinkProtectionLevel
//
// RETURNS      :   None
//
//////////////////////////////////////////////////////////////////////////////

void TPI_Poll (void)
{

	byte InterruptStatusImage;

	if (txPowerState[Sil9136ChipSel] == TX_POWER_STATE_D0)
	{
		InterruptStatusImage = ReadByteTPI(TPI_INTERRUPT_STATUS_REG);

		if (InterruptStatusImage & HOT_PLUG_EVENT)
		{

			TPI_DEBUG_PRINT (("HPD  -> "));

			ReadSetWriteTPI(TPI_INTERRUPT_ENABLE_REG, HOT_PLUG_EVENT);  // Enable HPD interrupt bit

			// Repeat this loop while cable is bouncing:
			do
			{
				WriteByteTPI(TPI_INTERRUPT_STATUS_REG, HOT_PLUG_EVENT);
				DelayMS(T_HPD_DELAY); // Delay for metastability protection and to help filter out connection bouncing
				InterruptStatusImage = ReadByteTPI(TPI_INTERRUPT_STATUS_REG);    // Read Interrupt status register
			} while (InterruptStatusImage & HOT_PLUG_EVENT);              // loop as long as HP interrupts recur

			if (((InterruptStatusImage & HOT_PLUG_STATE) >> 2) != hdmiCableConnected[Sil9136ChipSel])
			{

				if (hdmiCableConnected[Sil9136ChipSel] == TRUE)
				{
					OnHdmiCableDisconnected();
				}
				else
				{
					OnHdmiCableConnected();
					ReadModifyWriteIndexedRegister(INDEXED_PAGE_0, 0x0A, 0x08, 0x08);
				}

				if (hdmiCableConnected[Sil9136ChipSel] == FALSE)
				{
					return;
				}
			}
		}

#ifdef HAS_CTRL_BUS
		if (InterruptStatusImage == NON_MASKABLE_INT)
		{
			// Check if NMI has occurred
			TPI_DEBUG_PRINT (("TP -> NMI Detected\n"));
			TPI_Init();												// Reset and re-initialize
			HotPlugService();
		}
#endif

		// Check rx power
		if (((InterruptStatusImage & RX_SENSE_STATE) >> 3) != dsRxPoweredUp[Sil9136ChipSel])
		{

			if (hdmiCableConnected[Sil9136ChipSel] == TRUE)
			{
				if (dsRxPoweredUp[Sil9136ChipSel] == TRUE)
				{
					OnDownstreamRxPoweredDown();
				}
				else
				{
					OnDownstreamRxPoweredUp();
				}
			}

			ClearInterrupt(RX_SENSE_EVENT);
		}

		// Check if Audio Error event has occurred:
		if (InterruptStatusImage & AUDIO_ERROR_EVENT)
		{
			//TPI_DEBUG_PRINT (("TP -> Audio Error Event\n"));
			//  The hardware handles the event without need for host intervention (PR, p. 31)
			ClearInterrupt(AUDIO_ERROR_EVENT);
		}

#ifdef DEV_SUPPORT_HDCP
		if ((hdmiCableConnected[Sil9136ChipSel] == TRUE)
				&& (dsRxPoweredUp[Sil9136ChipSel] == TRUE)
				&& (VideoModeDescription[Sil9136ChipSel].HDCPAuthenticated == VMD_HDCP_AUTHENTICATED))
		{
			HDCP_CheckStatus(InterruptStatusImage);
		}
#endif

#if (IS_CEC == 1)
		SI_CecHandler(0 , 0);
#endif

	}

#ifdef F_9136
else if (txPowerState[Sil9136ChipSel] == TX_POWER_STATE_D3)
	{
		byte PinTxInt=1;  // jins
		if (PinTxInt == 0)
		{
			TPI_Init();
		}
	}
#endif
}

static void OnHdmiCableConnected(void)
{
	TPI_DEBUG_PRINT (("HDMI Connected\n"));

	// No need to call TPI_Init here unless TX has been powered down on cable removal.
	//TPI_Init();

	hdmiCableConnected[Sil9136ChipSel] = TRUE;

#ifdef HDCP_DONT_CLEAR_BSTATUS
#else
	if ((HDCP_TxSupports == TRUE) && (HDCP_AksvValid == TRUE) && (VideoModeDescription[Sil9136ChipSel].HDCPAuthenticated == VMD_HDCP_AUTHENTICATED)) {
		WriteIndexedRegister(INDEXED_PAGE_0, 0xCE, 0x00); // Clear BStatus
		WriteIndexedRegister(INDEXED_PAGE_0, 0xCF, 0x00);
	}
#endif

#ifdef DEV_SUPPORT_EDID
	DoEdidRead();
#endif

#ifdef READKSV
	ReadModifyWriteTPI(0xBB, 0x08, 0x08);
#endif

	if (IsHDMI_Sink())              // select output mode (HDMI/DVI) according to sink capabilty
	{
		TPI_DEBUG_PRINT (("HDMI Sink Detected\n"));
		ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, OUTPUT_MODE_MASK, OUTPUT_MODE_HDMI);
	}
	else
	{
		TPI_DEBUG_PRINT (("DVI Sink Detected\n"));
		ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, OUTPUT_MODE_MASK, OUTPUT_MODE_DVI);
	}
}


static void OnHdmiCableDisconnected(void)
{

	TPI_DEBUG_PRINT (("HDMI Disconnected\n"));

	hdmiCableConnected[Sil9136ChipSel] = FALSE;

#ifdef DEV_SUPPORT_EDID
	edidDataValid = FALSE;
#endif

	OnDownstreamRxPoweredDown();

#ifdef F_9136
	TxPowerStateD3();
#endif
}


static void OnDownstreamRxPoweredDown(void)
{

	TPI_DEBUG_PRINT (("DSRX -> Powered Down\n"));
	dsRxPoweredUp[Sil9136ChipSel] = FALSE;

#ifdef DEV_SUPPORT_HDCP
	if ((HDCP_TxSupports == TRUE)
					&& (HDCP_AksvValid == TRUE)
					&& (VideoModeDescription[Sil9136ChipSel].HDCPAuthenticated == VMD_HDCP_AUTHENTICATED))
	{
		HDCP_Off();
	}
#endif

	DisableTMDS();
}


static void OnDownstreamRxPoweredUp(void)
{
	TPI_DEBUG_PRINT (("DSRX -> Powered Up\n"));
	dsRxPoweredUp[Sil9136ChipSel] = TRUE;

	HotPlugService();

	USRX_OutputChange[Sil9136ChipSel] = FALSE;
}


#endif	//__USE_SIL9136_TX__
