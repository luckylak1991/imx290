/******************************************************************************
  * @project: LT8619C
  * @file: lt8619.h
  * @author: Yin Chao
  * @company: LONTIUM COPYRIGHT and CONFIDENTIAL
  * @date: 2016.11.10
******************************************************************************/


#if defined(__USE_LT8619_RX__)


#ifndef		__LT8619C_H__
#define		__LT8619C_H__

#define LT8619C_ADR 0x64

//====================================================================================//

typedef enum
{
    OUTPUT_RGB888,
    OUTPUT_RGB666,
    OUTPUT_RGB565,
    OUTPUT_YCBCR444,
    OUTPUT_YCBCR422_16BIT,
    OUTPUT_YCBCR422_20BIT,
    OUTPUT_YCBCR422_24BIT,
    OUTPUT_BT656_8BIT,
    OUTPUT_BT656_10BIT,
    OUTPUT_BT656_12BIT,
    OUTPUT_BT1120_8BIT,
    OUTPUT_BT1120_10BIT,
    OUTPUT_BT1120_12BIT,
    OUTPUT_BT1120_16BIT,
    OUTPUT_BT1120_20BIT,
    OUTPUT_BT1120_24BIT,
    OUTPUT_LVDS_1_PORT,
    OUTPUT_LVDS_2_PORT
}
_LT8619C_OUTPUTMODE;

#define LT8619C_OUTPUTMODE  OUTPUT_RGB888

//====================================================================================//

typedef enum
{
    COLOR_RGB = 0x00,
    COLOR_YCBCR444 = 0x40,
    COLOR_YCBCR422 = 0x20,
	COLOR_NULL = 0xFF
}
_INPUT_COLORSPACE;

#define LT8619C_OUTPUTCOLOR  COLOR_RGB

//====================================================================================//

typedef enum
{
/*-----------------------------------------------------------------------------------------------------------
If add other video resolution, please modify array Input_Resolution_Parameter[] in file panel.c accordingly.
------------------------------------------------------------------------------------------------------------*/
    INDEX_640x480_60P = 0,
    INDEX_656x496_60P,
    INDEX_720x480_60P,
    INDEX_720x480_50P,
    INDEX_720x576_50P,
    INDEX_1280x720_60P,
    INDEX_1280x720_50P,
    INDEX_1920x1080_60P,
    INDEX_1920x1080_30P,
    INDEX_1920x1080_50P,
    INDEX_1920x1080_25P,
    INDEX_1920x1080_24P,
    INDEX_800x600_60P,
    INDEX_848x480_60P,
    INDEX_1024x768_60P,
    INDEX_1280x768_RB_60P,
    INDEX_1280x768_60P,
    INDEX_1280x800_RB_60P,
    INDEX_1280x800_60P,
    INDEX_1280x960_60P,
    INDEX_1280x1024_60P,
    INDEX_1360x768_60P,
    INDEX_1366x768_RB_60P,
    INDEX_1366x768_60P,
    INDEX_1440x900_RB_60P,
    INDEX_1440x900_60P,
    INDEX_1600x900_RB_60P,
    INDEX_1600x1200_60P,
    INDEX_1920x1200_RB_60P,
    INDEX_1920x1200_60P,
    INDEX_3840x2160_24P,
    INDEX_3840x2160_25P,
    INDEX_3840x2160_30P,
    INDEX_4096x2160_24P,

    INDEX_NO_SIGNAL,
    INDEX_UNSUPPORTED_FORMAT

}Video_FormatIndex;

//====================================================================================//

typedef struct {
	LONG	ctrl;
	
    BOOL flag_RXClkStable;
    BOOL flag_RXClkDetected;
    BOOL flag_RXPLLLocked;
    BOOL Flag_HsyncStable;
	BOOL flag_GetInputInfo;
    BOOL input_hdmimode;
    BYTE   input_vic;
    BYTE   input_colorspace;
    BYTE   input_colordepth;
    BYTE   input_colorimetry;
    BYTE   input_QuantRange;
    BYTE   input_videoindex;
    DWORD  ClkFreqValCurrent;
    DWORD  ClkFreqValPrevious;

    
}_LT8619C_RXStatus;

// control parameter
#define		LT8619CCTRL_TRIGGER			0x40000000		// lt8619c main loop excute trigger

//====================================================================================//

void LT8619C_Init(void);
void LT8619C_MainLoop(void);
BYTE LT8619C_MultiWrite(BYTE rAddr, PBYTE pBuff, WORD bytes);
BYTE LT8619C_MultiRead(BYTE rAddr, PBYTE pBuff, WORD bytes);
void ISPM SetLT8619C_Tick(void);
void ExtInput_Monitor(void);

int LT8619C_GetVideoInfo(WORD *H_active, WORD *V_active, WORD *F_rate);
BYTE LT8619C_Getvideoindex(void);
#endif	// __LT8619C_H__


#endif	// __USE_LT8619_RX__

