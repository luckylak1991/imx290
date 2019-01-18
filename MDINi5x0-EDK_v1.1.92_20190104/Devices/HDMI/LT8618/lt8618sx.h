/******************************************************************************
  * @project: LT8618SX
  * @file: lt8618sx.c
  * @author: xhguo
  * @company: LONTIUM COPYRIGHT and CONFIDENTIAL
  * @date: 2017.07.25
******************************************************************************/
#if defined(__USE_LT8618SX_TX__)

#ifndef		__LT8618SX_H__
#define		__LT8618SX_H__

//#define   LT8618SX_ADR            0x76
#define   LT8618SX_ADR            0x76//0x72
//#define   LT8618SX_ADR_last       0x7e


typedef struct
{
u16 hfp;
u16 hs;
u16 hbp;
u16 hact;
u16 htotal;
u16 vfp;
u16 vs;
u16 vbp;
u16 vact;
u16 vtotal;
}video_timing;



typedef enum
{
 video_640x480_60Hz_vic1,      //vic 1
 video_720x480_60Hz_vic3,      //vic 2
 video_1280x720_60Hz_vic4,    //vic 3
 video_1920x1080_60Hz_vic16,    //vic 4
 
 video_1920x1080i_60Hz_169=5,  //vic 5
 video_720x480i_60Hz_43=6,     //vic 6
 video_720x480i_60Hz_169=7,    //vic 7
 video_720x240P_60Hz_43=8,     //vic 8
 video_720x240P_60Hz_169=9    //vic 9
//video_720x480_60Hz_43,       //vic 2
}VideoFormat;

//video_forma=video_640x480_60Hz_vic1;
//#define Video_Format video_1920x1080_60Hz_43;


typedef enum
{
 I2S_2CH,
 I2S_8CH,
 SPDIF
}
_Audio_Input_Mode;
#define     Audio_Input_Mode   I2S_2CH //I2S_2CH


typedef enum
{
 LT8618SX_Input_RGB888,//
 LT8618SX_Input_RGB565,//no
 LT8618SX_Input_YCbCr444,//yes
 LT8618SX_Input_YCbCr422_16BIT,//yes
 LT8618SX_Input_YCbCr422_20BIT,//no
 LT8618SX_Input_YCbCr422_24BIT,//no
 LT8618SX_Input_BT1120_16BIT,//ok
 LT8618SX_Input_BT1120_20BIT,//no
 LT8618SX_Input_BT1120_24BIT,//no
 LT8618SX_Input_BT656_8BIT,//OK
 LT8618SX_Input_BT656_10BIT,//no use
 LT8618SX_Input_BT656_12BIT//no use
}_LT8618SX_Input_Mode;

#define LT8618SX_Input_Mode   LT8618SX_Input_BT1120_16BIT//LT8618SX_Input_RGB888//LT8618SX_Input_BT656_8BIT


typedef enum
{
    LT8618SX_Output_RGB888,
	LT8618SX_Output_YCbCr444,
	LT8618SX_Output_YCbCr422_16BIT,
	LT8618SX_Output_YCbCr422_20BIT,
	LT8618SX_Output_YCbCr422_24BIT
}
_LT8618SX_Output_Mode;

#define LT8618SX_Output_Mode  LT8618SX_Output_RGB888


//extern @near u8 Sink_EDID[256];
void LT8618SX_Init(void);
void LT8618SX_Chip_ID(void);
void LT8618SX_Video_Check(void);
bool LT8618SX_HPD_Status(void);
bool LT8918SX_Read_EDID(void);
void LT8618SX_SetCSC(WORD mode);
BYTE LT8618SX_GetCSC(void);
void video_check();
BYTE LT8618SX_MultiWrite(BYTE sAddr, BYTE rAddr, PBYTE pBuff, WORD bytes);
BYTE LT8618SX_MultiRead(BYTE sAddr, BYTE rAddr, PBYTE pBuff, WORD bytes);
#endif	//__LT8618SX_H__

#endif	//__USE_LT8618SX_TX__
