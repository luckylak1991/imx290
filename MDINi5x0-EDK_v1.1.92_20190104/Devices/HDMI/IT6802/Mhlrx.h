///*****************************************
//  Copyright (C) 2009-2016
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <Mhlrx.h>
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2016/05/03
//   @fileversion: ITE_MHLRX_SAMPLE_V1.24
//******************************************/


#if defined(__USE_IT6802_RX__)


#ifndef _MHLRX_H_
#define _MHLRX_H_

//#include "IT680x_config.h"
//#include "IT680x_debug.h"
//#include "IT680x_typedef.h"
//#include "Mcu.h"
//#include "IO.h"
//#include "Utility.h"
//#include "stdio.h"

#if 1


//FIX_ID_036	xxxxx //Enable MHL Function for IT68XX
#define _ENABLE_IT68XX_MHL_FUNCTION_

#ifdef _ENABLE_IT68XX_MHL_FUNCTION_
#define _SUPPORT_RCP_				TRUE		// for support it6811 and it6802 MHL CBUS
#define _SUPPORT_RAP_				TRUE		// for support it6811 and it6802 MHL CBUS
//#define _SUPPORT_UCP_				FALSE		// for support it6811 and it6802 MHL CBUS
#endif
//FIX_ID_036	xxxxx



#define _SUPPORT_HDCP_				FALSE
#define _SUPPORT_EDID_RAM_		TRUE

//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
#define _SUPPORT_AUTO_EQ_                        TRUE
#define _SUPPORT_EQ_ADJUST_                     TRUE
//FIX_ID_001 xxxxx


//FIX_ID_021 xxxxx		//To use CP_100ms for CBus_100ms and CEC_100m
//FIX_ID_004 xxxxx 		//Add calibration ring osc for Cbus
#define _SelectExtCrystalForCbus_	TRUE
//FIX_ID_004 xxxxx
//FIX_ID_021 xxxxx

//FIX_ID_023 xxxxx		//Fixed for Audio Channel Status Error with invalid HDMI source
//#define EnableCalFs
//FIX_ID_023 xxxxx

//FIX_ID_028 xxxxx //For Debug Audio error with S2
//#define _FIX_ID_028_
//FIX_ID_028 xxxxx
//FIX_ID_026 xxxxx 		//Support RB SWAP for TTL output
//#define _SUPPORT_RBSWAP_
//FIX_ID_026 xxxxx

#define FIX_ID_013
#define FIX_ID_013_

/*****************************************************************************/
/* Type defs enums  **********************************************************/
/*****************************************************************************/
//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
//xxxxx 2014-0421 modify 50 to 100
#define MS_LOOP                 100
//xxxxx 2014-0421
//FIX_ID_033 xxxxx

enum
{
  MHD_RAP_CMD_POLL         	= 0x00,
  MHD_RAP_CMD_CHG_ACTIVE_PWR  = 0x10,
  MHD_RAP_CMD_CHG_QUIET       = 0x11,
  MHD_RAP_CMD_END             = 0x12
};

//RAPK sub commands
enum
{
    MHD_MSC_MSG_RAP_NO_ERROR        		= 0x00,     // RAP No Error
    MHD_MSC_MSG_RAP_UNRECOGNIZED_ACT_CODE  	= 0x01,
    MHD_MSC_MSG_RAP_UNSUPPORTED_ACT_CODE  	= 0x02,
    MHD_MSC_MSG_RAP_RESPONDER_BUSY   		= 0x03
};

enum
{
    RCP_SELECT          = 0x00,
    RCP_UP              = 0x01,
    RCP_DOWN            = 0x02,
    RCP_LEFT            = 0x03,
    RCP_RIGHT           = 0x04,
    RCP_RIGHT_UP        = 0x05,
    RCP_RIGHT_DOWN      = 0x06,
    RCP_LEFT_UP         = 0x07,
    RCP_LEFT_DOWN       = 0x08,
    RCP_ROOT_MENU       = 0x09,
    RCP_SETUP_MENU      = 0x0A,
    RCP_CONTENTS_MENU   = 0x0B,
    RCP_FAVORITE_MENU   = 0x0C,
    RCP_EXIT            = 0x0D,

    //0x0E - 0x1F are reserved

    RCP_NUM_0           = 0x20,
    RCP_NUM_1           = 0x21,
    RCP_NUM_2           = 0x22,
    RCP_NUM_3           = 0x23,
    RCP_NUM_4           = 0x24,
    RCP_NUM_5           = 0x25,
    RCP_NUM_6           = 0x26,
    RCP_NUM_7           = 0x27,
    RCP_NUM_8           = 0x28,
    RCP_NUM_9           = 0x29,

    RCP_DOT             = 0x2A,
    RCP_ENTER           = 0x2B,
    RCP_CLEAR           = 0x2C,

    //0x2D - 0x2F are reserved

    RCP_CH_UP           = 0x30,
    RCP_CH_DOWN         = 0x31,
    RCP_PRE_CH          = 0x32,
    RCP_SOUND_SELECT    = 0x33,
    RCP_INPUT_SELECT    = 0x34,
    RCP_SHOW_INFO       = 0x35,
    RCP_HELP            = 0x36,
    RCP_PAGE_UP         = 0x37,
    RCP_PAGE_DOWN       = 0x38,

    //0x39 - 0x40 are reserved

    RCP_VOL_UP	        = 0x41,
    RCP_VOL_DOWN        = 0x42,
    RCP_MUTE            = 0x43,
    RCP_PLAY            = 0x44,
    RCP_STOP            = 0x45,
    RCP_PAUSE           = 0x46,
    RCP_RECORD          = 0x47,
    RCP_REWIND          = 0x48,
    RCP_FAST_FWD        = 0x49,
    RCP_EJECT           = 0x4A,
    RCP_FWD             = 0x4B,
    RCP_BKWD            = 0x4C,

    //0x4D - 0x4F are reserved

    RCP_ANGLE            = 0x50,
    RCP_SUBPICTURE       = 0x51,

    //0x52 - 0x5F are reserved

    RCP_PLAY_FUNC       = 0x60,
    RCP_PAUSE_PLAY_FUNC = 0x61,
    RCP_RECORD_FUNC     = 0x62,
    RCP_PAUSE_REC_FUNC  = 0x63,
    RCP_STOP_FUNC       = 0x64,
    RCP_MUTE_FUNC       = 0x65,
    RCP_UN_MUTE_FUNC    = 0x66,
    RCP_TUNE_FUNC       = 0x67,
    RCP_MEDIA_FUNC      = 0x68,

    //0x69 - 0x70 are reserved

    RCP_F1              = 0x71,
    RCP_F2              = 0x72,
    RCP_F3              = 0x73,
    RCP_F4              = 0x74,
    RCP_F5              = 0x75,

    //0x76 - 0x7D are reserved

    RCP_VS              = 0x7E,
    RCP_RSVD            = 0x7F

};

//FIX_ID_003 xxxxx	//Add IT6802 Video Output Configure setting
typedef enum _Video_Output_Configure
{
    eRGB444_SDR=0,
    eYUV444_SDR,
    eRGB444_DDR,
    eYUV444_DDR,
    eYUV422_Emb_Sync_SDR,
    eYUV422_Emb_Sync_DDR,
    eYUV422_Sep_Sync_SDR,
    eYUV422_Sep_Sync_DDR,
    eCCIR656_Emb_Sync_SDR,
    eCCIR656_Emb_Sync_DDR,
    eCCIR656_Sep_Sync_SDR,
    eCCIR656_Sep_Sync_DDR,
    eRGB444_Half_Bus,
    eYUV444_Half_Bus,
    eBTA1004_SDR,
    eBTA1004_DDR,
    eVOMreserve
}Video_Output_Configure;

typedef enum _Video_OutputDataTrigger_Mode
{
    eSDR=0,
    eHalfPCLKDDR,
    eHalfBusDDR,
    eSDR_BTA1004,
    eDDR_BTA1004
}Video_DataTrigger_Mode;

typedef enum _Video_OutputSync_Mode
{
    eSepSync=0,
    eEmbSync,
    eCCIR656SepSync,
    eCCIR656EmbSync
}Video_OutputSync_Mode;

//FIX_ID_003 xxxxx

typedef enum _Video_State_Type {
    VSTATE_Off=0,
    VSTATE_TerminationOff,
    VSTATE_TerminationOn,
    VSTATE_5VOff,
    VSTATE_SyncWait,
    VSTATE_SWReset,
    VSTATE_SyncChecking,
    VSTATE_HDCPSet,
    VSTATE_HDCP_Reset,
    VSTATE_ModeDetecting,
    VSTATE_VideoOn,
    VSTATE_ColorDetectReset,
    VSTATE_HDMI_OFF,
    VSTATE_Unknown,
    VSTATE_Reserved
} Video_State_Type;


typedef enum _Audio_State_Type {
    ASTATE_AudioOff = 0,
    ASTATE_RequestAudio ,
    ASTATE_ResetAudio,
    ASTATE_WaitForReady,
    ASTATE_AudioOn ,
    ASTATE_Unknown,
    ASTATE_Reserved
} Audio_State_Type;


typedef enum _RxHDCP_State_Type {
    RxHDCP_PwrOff=0,
    RxHDCP_ModeCheck,
    RxHDCP_Receiver,
    RxHDCP_Repeater,
    RxHDCP_SetKSVFifoList,
    RxHDCP_GenVR,
    RxHDCP_WriteVR,
    RxHDCP_Auth_WaitRi,
    RxHDCP_Authenticated,
    RxHDCP_Reserved
} RxHDCP_State_Type;

typedef enum  {
    RCP_Received = 0,
    RCP_Error,
    RCP_ACK,
    RCP_Transfer,
    RCP_Empty,
    RCP_Unknown
}RCPState_Type;

typedef enum  {
    RCP_Result_OK = 0,
    RCP_Result_FAIL,
    RCP_Result_ABORT,
    RCP_Result_Transfer,
    RCP_Result_Finish,
    RCP_Result_Unknown
}RCPResult_Type;




#define F_MODE_RGB24  0
#define F_MODE_RGB444  0
#define F_MODE_YUV422 1
#define F_MODE_YUV444 2
//FIX_ID_044  add YCbCr420 supporting
#define F_MODE_YUV420 3 // CEA-861F defined YUV420 3
//~FIX_ID_044
#define F_MODE_CLRMOD_MASK 3
#define F_MODE_ITU709  (1<<4)
#define F_MODE_ITU601  0
#define F_MODE_0_255   0
#define F_MODE_16_235  (1<<5)
#define F_MODE_EN_UDFILT (1<<6)
#define F_MODE_EN_DITHER (1<<7)

#define RCVABORT        2
#define RCVNACK         3
#define ARBLOSE         4
#define FWTXFAIL        5
#define FWRXPKT         6
#define FAIL			-1
#define ABORT           -2

#define HIGH			1
#define LOW				0

#define CD8BIT			4
#define CD10BIT	 		5
#define CD12BIT			6
#define CD16BIT			7

#define OUT8B           0
#define OUT10B          1
#define OUT12B          2

#define RGB444			0
#define YCbCr422		1
#define YCbCr444		2

#define NORM            0
#define FAST            1
#define SLOW            2

#define AUD32K			0x3
#define AUD48K			0x2
#define AUD96K			0xA
#define AUD192K			0xE
#define AUD44K			0x0
#define AUD88K			0x8
#define AUD176K			0xC

#define I2S				0
#define SPDIF			1

#define MHLInt00B       0x20
#define DCAP_CHG        0x01
#define DSCR_CHG        0x02
#define REQ_WRT         0x04
#define GRT_WRT         0x08

#define MHLInt01B       0x21
#define EDID_CHG        0x01

#define MHLSts00B       0x30
#define DCAP_RDY        0x01

#define MHLSts01B       0x31
#define NORM_MODE       0x03
#define PACK_MODE       0x02
#define PATH_EN         0x08
#define MUTED           0x10

#define MSG_MSGE	0x02
#define MSG_RCP		0x10
#define MSG_RCPK	0x11
#define MSG_RCPE	0x12
#define MSG_RAP		0x20
#define MSG_RAPK	0x21
#define MSG_UCP		0x30
#define MSG_UCPK	0x31
#define MSG_UCPE	0x32


#define MHL  1
#define HDMI 0

#define BUS10B 1
#define BUS20B 0


#define MAXRCPINDEX 5
// FIX_ID_043
#define MAX_TOGGLE_FAIL_COUNT 16
//~FIX_ID_043

#define SUPPORT_INPUTRGB
#define SUPPORT_INPUTYUV444
#define SUPPORT_INPUTYUV422

#if defined(SUPPORT_INPUTYUV444)|| defined(SUPPORT_INPUTYUV422)
#define SUPPORT_INPUTYUV
#endif


#define SUPPORT_OUTPUTYUV
#define SUPPORT_OUTPUTYUV444
#define SUPPORT_OUTPUTYUV422
//FIX_ID_044  add YCbCr420 supporting
#define SUPPORT_OUTPUTYUV420
//~FIX_ID_044

#if (defined(SUPPORT_OUTPUTYUV444))||(defined(SUPPORT_OUTPUTYUV422))
#define SUPPORT_OUTPUTYUV
#endif

#define F_PORT_SEL_0      0
#define F_PORT_SEL_1      1

#endif


#if 1
/*****************************************************************************/
/* Type defs struct **********************************************************/
/*****************************************************************************/
struct IT6802_REG_INI
{
    unsigned char ucAddr;
    unsigned char andmask;
    unsigned char ucValue;
};


struct IT6802_VIDEO_CONFIGURE_REG
{
    unsigned char ucReg51;
    unsigned char ucReg65;
};

typedef struct _3D_SourceConfiguration
{
    unsigned char          Format;              /**< Type of 3D source format expected or found.                                                        */
    unsigned char    LR_Reference;        /**< Source of the 3D L/R reference.                                                                    */
    unsigned char FrameDominance;      /**< Left or Right Eye is first in L/R image pair.                                                      */
    unsigned char     LR_Encoding;         /**< Type of 3D L/R encoding expected or detected.                                                      */
    unsigned char    TB_Reference;        /**< Top/Bottom reference for vertically sub-sampled sources.                                           */
    unsigned char    OE_Reference;        /**< Odd/Even reference for horizontally sub-sampled sources.                                           */
    unsigned char NumActiveBlankLines;                 /**< Number of lines separating vertically packed L/R data to be removed (cropped)
                                                  *  before being displayed. Does not include any embedded encoding.                                    */
    unsigned char NumberOfEncodedLines;                /**< Number of encoded lines in one L/R eye frame of the display data
                                                  *  to be blanked out with "Blanking Color". (assumed same number in second eye frame)                 */
    unsigned int  LeftEncodedLineLocation;             /**< Active line number of 1st encoded line in one Left eye frame of the display data (-1=unknown).     */
    unsigned int  RightEncodedLineLocation;            /**< Active line number of 1st encoded line in one Right eye frame of the display data (-1=unknown).
                                                  *  If format is Horizontally Packed, set RightEncodedLineLocation=LeftEncodedLineLocation             */
    unsigned char BlankingColor;                 /**< Color to use when blanking (or masking off) any embedded L/R encoding.                             */
} SRC_3D_SOURCE_CONFIG;

typedef struct _de3dframe
{
    unsigned char VIC;
    unsigned char HB0;
    unsigned char HB1;
    unsigned char HB2;
    unsigned char PB0;
    unsigned char PB1;
    unsigned char PB2;
    unsigned char PB3;
    unsigned char PB4;
    unsigned char PB5;
    unsigned char PB6;
    unsigned char PB7;
} DE3DFRAME;

typedef struct _set_de3d_frame
{
    unsigned char Vic;
    unsigned int V_total;        // Vtotal -1
    unsigned int V_act_start;    // VTotal -1 + Vactive_start -1
    unsigned int V_act_end;      // Vactive end -1
    unsigned int V_sync_end;     // LSB(Vtotal -1 + sync With)
    unsigned int V_2D_active_total;     // V_2D_active_total

} SET_DE3D_FRAME;

typedef enum
{
    VSYNC_SEPARATED_HALF,       /**< VSync separated (field sequential) format.                         */
    VSYNC_SEPARATED_FULL,       /**< VSync separated (frame sequential progressive) format.             */
    VERT_PACKED_HALF,           /**< Over Under (vertically packed) half resolution format.             */
    VERT_PACKED_FULL,           /**< Over Under (vertically packed) full resolution format.             */
    HORIZ_PACKED_HALF,          /**< Side by Side (horizontally packed) half resolution format.         */
    HORIZ_PACKED_FULL,          /**< Side by Side (horizontally packed) full resolution format.         */
    UNDEFINED_FORMAT            /**< Undefined format.                                                  */
} SRC_3D_FORMAT;

typedef enum _pixel_mode
{
    SINGLE_PIXEL,
    DUAL_PIXEL  ,
    MODE_UNKNOWN
}PIXEL_MODE;


//FIX_ID_010 xxxxx 	//Add JudgeBestEQ to avoid wrong EQ setting
#define MaxEQIndex 3
//FIX_ID_010 xxxxx

//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
#ifdef _SUPPORT_EQ_ADJUST_
struct it6802_eq_data
{
unsigned char ucEQState;
unsigned char ucAuthR0;								//20130327 for R0 fail issue
unsigned char ucECCvalue;								//20130328 for acc ecc error
unsigned char ucECCfailCount;							//20130328 for acc ecc error
unsigned char ucPkt_Err;
unsigned char ucPortID;
unsigned char f_manualEQadjust;
//FIX_ID_010 xxxxx 	//Add JudgeBestEQ to avoid wrong EQ setting
unsigned char ErrorCount[MaxEQIndex];
//FIX_ID_010 xxxxx
};
#endif
//FIX_ID_001 xxxxx



//FIX_ID_005 xxxxx	//Add Cbus Event Handler
    #define B_MSC_Waiting 		0x10
    #define B_DevCapChange 		0x08
    #define B_3DSupporpt 		0x04 // bit2 B_3DSupporpt
    #define B_ReadDevCap 		0x02 // bit1 B_ReadDevCap
    #define B_DiscoveryDone 		0x01 // bit0 B_DiscoveryDone
//FIX_ID_005 xxxxx

//FIX_ID_014 xxxxx
    #define B_PORT1_TimingChgEvent	0x40
    #define B_PORT1_TMDSEvent	0x20
    #define B_PORT1_Waiting		0x10
    #define B_PORT0_TimingChgEvent	0x04
    #define B_PORT0_TMDSEvent	0x02
    #define B_PORT0_Waiting		0x01
//FIX_ID_014 xxxxx

//FIX_ID_013	xxxxx	//For MSC 3D request issue
#define MSC_3D_VIC		(0x0010)
#define MSC_3D_DTD		(0x0011)

typedef enum _PARSE3D_STA{
    PARSE3D_START,
    PARSE3D_LEN,
    PARSE3D_STRUCT_H,
    PARSE3D_STRUCT_L,
    PARSE3D_MASK_H,
    PARSE3D_MASK_L,
    PARSE3D_VIC,
    PARSE3D_DONE
}PARSE3D_STA;

typedef enum _MHL3D_STATE{
    MHL3D_REQ_START,
    MHL3D_REQ_WRT,
    MHL3D_GNT_WRT,
    MHL3D_WRT_BURST,
    MHL3D_REQ_DONE
}MHL3D_STATE;

struct PARSE3D_STR
{
    unsigned char	uc3DEdidStart;
    unsigned char	uc3DBlock;
    unsigned char	uc3DInfor[32];
    unsigned char	ucVicStart;
    unsigned char	ucVicCnt;
    unsigned char	uc3DTempCnt;
    unsigned char	ucDtdCnt;
    unsigned char	bVSDBspport3D;
};
//FIX_ID_013	xxxxx


struct AVI_info
{
    unsigned char ColorMode;
    unsigned char Colorimetry;
    unsigned char ExtendedColorimetry;
    unsigned char RGBQuantizationRange;
    unsigned char YCCQuantizationRange;
    unsigned char VIC;
    //unsigned char PixelRepetition;
};


struct it6802_dev_data
{
    Video_State_Type m_VState;
    Audio_State_Type m_AState;
    RxHDCP_State_Type m_RxHDCPState;
    AUDIO_CAPS m_RxAudioCaps;
    unsigned short m_SWResetTimeOut;
    unsigned short m_VideoCountingTimer;
    unsigned short m_AudioCountingTimer;
    unsigned char m_ucCurrentHDMIPort;
    unsigned char m_bOutputVideoMode;
    unsigned char m_bInputVideoMode;

//FIX_ID_039 xxxxx fix image flick when enable RGB limited / Full range convert
#ifdef _AVOID_REDUNDANCE_CSC_
    unsigned char m_Backup_OutputVideoMode;
    unsigned char m_Backup_InputVideoMode;
#endif
//FIX_ID_039 xxxxx

    unsigned char m_ucSCDTOffCount;
    unsigned char m_ucEccCount_P0;
    unsigned char m_ucEccCount_P1;
    unsigned char m_ucDeskew_P0;
    unsigned char m_ucDeskew_P1;

    SRC_3D_SOURCE_CONFIG de3dframe_config;
    DE3DFRAME s_Current3DFr;

    unsigned char oldVIC;
    unsigned char newVIC;
    unsigned char  f_de3dframe_hdmi;

//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
    #ifdef _SUPPORT_EQ_ADJUST_
    struct it6802_eq_data EQPort[2];
    // FIX_ID_043
    unsigned char ToggleCount[2];
    //~FIX_ID_043
    #endif
//FIX_ID_001 xxxxx

    //FIX_ID_003 xxxxx	//Add IT6802 Video Output Configure setting
    Video_Output_Configure m_VidOutConfigMode;
    Video_DataTrigger_Mode m_VidOutDataTrgger;
    Video_OutputSync_Mode m_VidOutSyncMode;
    //FIX_ID_003 xxxxx
//FIX_ID_005 xxxxx	//Add Cbus Event Handler
    unsigned char CBusIntEvent;
    unsigned char CBusSeqNo;
    unsigned char CBusWaitNo;
//FIX_ID_005 xxxxx

    unsigned char CbusDcapReadNo;//20150608_FIX_CHROMA

//FIX_ID_014 xxxxx	//Add Cbus Event Handler
    unsigned char HDMIIntEvent;
    unsigned char HDMIWaitNo[2];
//FIX_ID_014 xxxxx

//FIX_ID_021 xxxxx		//To use CP_100ms for CBus_100ms and CEC_100m
// #ifndef _SelectExtCrystalForCbus_
// 	unsigned long RCLK;
// 	unsigned long PCLK;
//
// 	int  t10usint;
// 	int  t10usflt;
// #endif
//FIX_ID_021 xxxxx

    //AVI_info m_avi;
    unsigned char ColorMode;
    unsigned char Colorimetry;
    unsigned char ExtendedColorimetry;
    unsigned char RGBQuantizationRange;
    unsigned char YCCQuantizationRange;
    unsigned char VIC;

//FIX_ID_034 xxxxx //Add MHL HPD Control by it6802HPDCtrl( )
    unsigned char m_DiscoveryDone;
//FIX_ID_034 xxxxx

//FIX_ID_037 xxxxx //Allion MHL compliance issue !!!
    //xxxxx 2014-0529 //Manual Content On/Off
    unsigned char m_RAP_ContentOff;
    unsigned char m_HDCP_ContentOff;
    //xxxxx
//FIX_ID_037 xxxxx

    PIXEL_MODE pixelMode;	//Output TTL Pixel mode
    unsigned char GCP_CD;	//Output Color Depth
    unsigned char  DE3DFormat_HDMIFlag:1;
    unsigned char  FramePacking_Flag:1;
    unsigned char  TopAndBottom_Flag:1;
    unsigned char  SideBySide_Flag:1;

    #ifdef _IT6607_GeNPacket_Usage_
    BYTE m_PollingPacket;
    BYTE m_PacketState;
    BYTE m_ACPState;
    BYTE m_GeneralRecPackType;
    BYTE m_GamutPacketRequest:1;
    #endif

//#if(_SUPPORT_HDCP_)
//    //HDCP
//    unsigned char HDCPEnable;
//    HDCPSts_Type Hdcp_state;
//    unsigned int HDCPFireCnt;
//#endif
    // 2015/07/01 added by jjtseng
    // for solving the new AVI interrupt issued by Audio Force MUTE.
    unsigned char prevAVIDB[2] ;

    //CBUS MSC
    unsigned char Mhl_devcap[16];
    unsigned char txmsgdata[2];
    unsigned char rxmsgdata[2];
    unsigned char txscrpad[16];
    unsigned char rxscrpad[16];
    unsigned char RCPTxArray[MAXRCPINDEX];
    unsigned char	RCPhead;
    unsigned char	RCPtail;
    RCPState_Type RCPState;
    RCPResult_Type RCPResult;
//FIX_ID_015	xxxxx peer device no response
    unsigned char RCPCheckResponse;
//FIX_ID_015	xxxxx

//FIX_ID_024	xxxxx Fixed for RCP compliance issue
    unsigned char m_bRCPTimeOut:1;
    unsigned char m_bRCPError:1;
//FIX_ID_024	xxxxx
    unsigned char m_bRxAVmute:1;
    unsigned char m_bVideoOnCountFlag:1;
    unsigned char m_MuteAutoOff:1;
    unsigned char m_bUpHDMIMode:1;
    unsigned char m_bUpHDCPMode:1;
    unsigned char m_NewAVIInfoFrameF:1;
    unsigned char m_NewAUDInfoFrameF:1;
    unsigned char m_HDCPRepeater:1;
    unsigned char m_MuteByPKG:1;
    // pIT680XCallbackList_t	pCallbackFunctionsList;
    unsigned char m_bHDCPrepeater:1;

	//ITE_150302  Chroma A222908 issue
	unsigned char cbus_wait_clk_mode;
	unsigned char cbus_clockmode_timeout;
	//

#ifdef _SUPPORT_HDMI_REPEATER_
    unsigned char m_sinkHPD ;
#endif // _SUPPORT_HDMI_REPEATER_

};

#endif


/*****************************************************************************/
/* Error codes ***************************************************************/
/*****************************************************************************/
/* Error code specifying an I2C driver error */
#define IT680x_I2C_DRIVER_ERROR                     -4
#define IT680x_I2C_DRIVER_TERMINAL_ERROR            -5

/* Error code specifying RTA errors */
#define IT680x_RTA_SEM_IN_USE                       -6
#define IT680x_RTA_ERROR                            -7

/* Error code specifying other errors */
#define IT680x_DATA_INVALID                         -8





//void read_mscmsg( void );
//void parse_devcap( BYTE *devcap );
//BYTE read_devcap_fwrpdmsg( void );
//BYTE read_devcap_fwreqmsg( void );
//void IT6802_fsm_init(void);
//void IT6802_fsm(void);
//void IT6802VideoOutputConfigure(void);
//void SetVideoOutputFormat(void);
//void it6802PortSelect(unsigned char ucPortSel);
//void RCPKeyPush(unsigned char ucKey);
//



/*****************************************************************************/
/* Public function prototypes ************************************************/
/*****************************************************************************/
#if 0
void    IT680x_CfgIT680x(I2CPORT i2c_port, I2CINIT *i2c_cfg);
void    IT680x_SwitchPort(IT6802_CURRENT_PORT port);
void    IT680x_InitIT680x(void);
void    IT680x_GetVersion(void);
void    IT680x_PollIT680x(void);
void    IT680x_EnableVideoOutput(PIXEL_MODE mode);

int08   HDMI_SetHDMIPower(POWER_STATE pwr);
int08   MHL_SetMHLPower(POWER_STATE pwr);

void    MHL_GetPixelMode(PIXEL_MODE *mode);
void    HDMI_GetColorType(uint08 *type);
void    MHL_GetColorType(uint08 *type);
//BOOL    MHL_Is_LINK_TrainDone(void);

BOOL    MHL_IsInputInterlace(void);
BOOL    MHL_IsMHLVideoON(void);
BOOL    HDMI_IsInputHDMIMode(void);
BOOL    HDMI_IsHDMIVideoON(void);
BOOL    HDMI_IsHDMI5V(void);
BOOL    HDMI_IsSCDT(void);
void    IT6802_SetVideoMute(BOOL bMute);
BOOL    oemIT6802PollEnable_Get(void);
void    oemIT6802PollEnable_Set(BOOL val);
BOOL    HDMI_IsRGBLimitedRange(BOOL isHighDefinitionVideo);
void    HDMI_GetIT680x_3DStatus(uint08 *ReceiveData);
void    HDMI_GetIT680x_InputTiming(uint16 *ReceiveData);
BOOL    HDMI_GetThreeDMode(eThreeDMode type);
void    HDMI_OutputCtrl(BOOL bEnableOutput);

#endif



/* IT680x IO Functions   ***********************************************************/
/* IT680x Configuration and Initialization ***********************************/
void IT6802_fsm_init(void);
/* HDMI RX functions   *********************************************************/
void it6802PortSelect(unsigned char ucPortSel);
void it6802HPDCtrl(unsigned char ucport,unsigned char ucEnable);
/* HDMI Audio function    *********************************************************/
/* HDMI Video function    *********************************************************/
/* HDMI Interrupt function    *********************************************************/



/* MHLRX functions    *********************************************************/
/* MHL interrupt    *******************************************************/
/* MHL 3D functions    *******************************************************/
/* EDID RAM  functions    *******************************************************/
/* RCP functions    *******************************************************/
#ifdef _SUPPORT_RCP_
void RCPKeyPush(unsigned char ucKey);
#endif
/* Driver State Machine Process **********************************************/
void IT6802_fsm(void);

void IT6802ChangeTTLVideoOutputMode(void);
void IT6802MSCCmdTest(unsigned char ucCmd);
void  Dump_IT680xReg(void);
void debugEQ(unsigned char ucCmd);
void debugxxx(void);

void show_vid_info( void );
void get_vid_info( void );


#ifdef SUPPORT_UART_CMD
extern unsigned char m_UartCmd;
#endif

#endif


#endif	// __USE_IT6802_RX__

