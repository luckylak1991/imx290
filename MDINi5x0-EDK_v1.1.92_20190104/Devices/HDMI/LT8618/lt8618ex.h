/************************************************************
*	ProjectName:	   LT86104
*	FileName:	       lt86104.h
*	BuildData:	     2013-01-03
*	Version£º        V1.3.2
*	Company:	       Lontium
************************************************************/

#if defined(__USE_LT8618EX_TX__)

#ifndef		__LT8618EX_H__
#define	__LT8618EX_H__

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#define	LT8618EX_ADR              0x76  // Tx general perpose addr
#define	LT8618EX_ADR_last       0x7e // Tx audio & info frame addr

#define	LT8618EX_INIT_CSC		1	// [0 : Color_Ycbcr422BT601] [1: Color_Ycbcr422BT709]

#define		MDIN_AVI_INFO_CTRL
#if defined(MDIN_AVI_INFO_CTRL)
typedef	struct
{
	BYTE VIC;
	BYTE Y;
	WORD dummy;

}	AVI_Infoframe_t;
#endif

typedef enum {
	LT8618_COLOR_RGB=0, LT8618_COLOR_YCbCr422, LT8618_COLOR_YCbCr444
} 	LT8618_COLOR_MODE;

typedef enum {
	LT8618_SYNC_EMBD=0, LT8618_SYNC_SEPA
} 	LT8618_SYNC_MODE;

typedef enum {
	LT8618_SIGN_DVI=0, LT8618_SIGN_HDMI
} 	LT8618_SIGN_MODE;

typedef enum {
	LT8618_VID_720P60=0,
	LT8618_VID_720P59,
	LT8618_VID_720P50,
	LT8618_VID_720P30,
	LT8618_VID_720P25,
	LT8618_VID_720P24,
	
	LT8618_VID_1080I60,
	LT8618_VID_1080I59,
	LT8618_VID_1080I50,
	
	LT8618_VID_1080P60,
	LT8618_VID_1080P59,
	LT8618_VID_1080P50,
	LT8618_VID_1080P30,
	LT8618_VID_1080P29,
	LT8618_VID_1080P25,
	LT8618_VID_1080P24,
	
	LT8618_VID_720P120,
	LT8618_VID_720P100,
	
	LT8618_VID_3840x2160P30,
	LT8618_VID_3840x2160P29,
	LT8618_VID_3840x2160P25,
	LT8618_VID_3840x2160P24,
	LT8618_VID_3840x2160P15,
	LT8618_VID_3840x2160P12,
	
	LT8618_VID_4096x2160P30,
	LT8618_VID_4096x2160P29,
	LT8618_VID_4096x2160P25,
	LT8618_VID_4096x2160P24,
	
//	LT8618_VID_2160P60
	
} 	LT8618_VID_FRMT;

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------
void LT8618EX_Init(void);

void LT8618EX_SetCSC(BYTE mode);
BYTE LT8618EX_GetCSC(void);
BYTE LT8618EX_OutputVideoFormat(LT8618_VID_FRMT format);
void LT8618EX_Rev_Out_Mode(LT8618_SYNC_MODE sync, LT8618_SIGN_MODE sign);
void LT8618EX_SetFrmt(void);

void LT8618EX_SetAviInfo(BYTE VIC, BYTE Y); // Writing infoframe only

//----------------------------------------//
void LT8618EX_RegWrite(BYTE sAddr, BYTE rAddr, BYTE data);
BYTE LT8618EX_RegRead(BYTE sAddr, BYTE rAddr);

BYTE LT8618EX_MultiWrite(BYTE sAddr, BYTE rAddr, PBYTE pBuff, WORD bytes);
BYTE LT8618EX_MultiRead(BYTE sAddr, BYTE rAddr, PBYTE pBuff, WORD bytes);
//----------------------------------------//

#endif /* __LT8618EX_H__ */
#endif // #if defined(__USE_LT8618EX_TX__)
