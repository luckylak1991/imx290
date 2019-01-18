/************************************************************
*	ProjectName:	   LT8522EX
*	FileName:	       lt8522ex.c
*	BuildData:	     2014-03-27
*	Version£º        V1.00
* Company:	       Lontium
************************************************************/
#include "video.h"

#if defined(__USE_LT8618EX_TX__)

// #define 	Check_Ycbcr444 		0x04
// #define 	Check_RGB888    	0x44

// #define 	Out_Ycbcr444 			0x50
// #define 	Out_RGB888    			0x10

#define 	Out_Ycbcr422 			0x01
#define 	Out_Ycbcr444 			0x02
#define 	Out_RGB444  			0x00

// Input port color space
#define 	Color_BYPASS   				0x00 //CSC bpass
#define 	Color_BYPASSUS  			0x10 //CSC bpass with 422->444 upscale
#define 	Color_RGB444 					0x01 //RGB444 to YCbCr444 color space conversion 

#define 	Color_Ycbcr444BT601 		0x02 //YCbCr444 to RGB444 color space conversion BT601
#define 	Color_Ycbcr444BT601RS	0x06 //YCbCr444 to RGB444 color space conversion BT601 range scale
#define 	Color_Ycbcr444BT709 		0x0A //YCbCr444 to RGB444 color space conversion BT709
#define 	Color_Ycbcr444BT709RS	0x0E //YCbCr444 to RGB444 color space conversion BT709 range scale

#define 	Color_Ycbcr422BT601		0x12 //YCbCr422 to RGB444 color space conversion BT601
#define 	Color_Ycbcr422BT601RS	0x16 //YCbCr422 to RGB444 color space conversion BT601 range scale
#define 	Color_Ycbcr422BT709		0x1A //YCbCr422 to RGB444 color space conversion BT709
#define 	Color_Ycbcr422BT709RS	0x1E //YCbCr422 to RGB444 color space conversion BT709 range scale

ROMDATA BYTE Video_mode720P60[16]={0x72,0x06,0x6E,0x28,0x00,0xDC,0xEE,0x20,0x05,0x05,0x00,0x05,0x14,0xD0,0x72,0x12};
ROMDATA BYTE Video_mode720P59[16]={0x72,0x06,0x6E,0x28,0x00,0xDC,0xEE,0x20,0x05,0x05,0x00,0x05,0x14,0xD0,0x72,0x12};
ROMDATA BYTE Video_mode720P50[16]={0xBC,0x07,0xB8,0x28,0x01,0xDC,0xEE,0x20,0x05,0x05,0x00,0x05,0x14,0xD0,0xBC,0x22};
ROMDATA BYTE Video_mode720P30[16]={0xE4,0x0C,0xE0,0x28,0x06,0xDC,0xEE,0x20,0x05,0x05,0x00,0x05,0x14,0xD0,0xE4,0x72};
ROMDATA BYTE Video_mode720P25[16]={0x78,0x0F,0x74,0x28,0x09,0xDC,0xEE,0x20,0x05,0x05,0x00,0x05,0x14,0xD0,0x78,0xA2};
ROMDATA BYTE Video_mode720P24[16]={0xE4,0x0C,0xE0,0x28,0x06,0xDC,0xEE,0x20,0x05,0x05,0x00,0x05,0x14,0xD0,0xE4,0x72};
//ROMDATA BYTE Video_mode1080i60[16]={0x98,0x08,0x58,0x2C,0x00,0x94,0x33,0x20,0x02,0x05,0x80,0x07,0x1E,0x1C,0x18,0x12};
ROMDATA BYTE Video_mode1080i60[16]={0x98,0x08,0x58,0x2C,0x00,0x94,0x33,0x20,0x03,0x05,0x80,0x07,0x1E,0x1C,0x18,0x12};
//ROMDATA BYTE Video_mode1080i59[16]={0x98,0x08,0x58,0x2C,0x00,0x94,0x33,0x20,0x02,0x05,0x80,0x07,0x1E,0x1C,0x18,0x12};
ROMDATA BYTE Video_mode1080i59[16]={0x98,0x08,0x58,0x2C,0x00,0x94,0x33,0x20,0x03,0x05,0x80,0x07,0x1E,0x1C,0x18,0x12};
//ROMDATA BYTE Video_mode1080i50[16]={0x50,0x0A,0x10,0x2C,0x02,0x94,0x33,0x20,0x02,0x05,0x80,0x07,0x1E,0x1C,0xD0,0x22};
ROMDATA BYTE Video_mode1080i50[16]={0x50,0x0A,0x10,0x2C,0x02,0x94,0x33,0x20,0x03,0x05,0x80,0x07,0x1E,0x1C,0xD0,0x22};
ROMDATA BYTE Video_mode1080P60[16]={0x98,0x08,0x58,0x2C,0x00,0x94,0x65,0x40,0x04,0x05,0x80,0x07,0x24,0x38,0x18,0x14};
ROMDATA BYTE Video_mode1080P59[16]={0x98,0x08,0x58,0x2C,0x00,0x94,0x65,0x40,0x04,0x05,0x80,0x07,0x24,0x38,0x18,0x14};
ROMDATA BYTE Video_mode1080P50[16]={0x50,0x0A,0x10,0x2C,0x02,0x94,0x65,0x40,0x04,0x05,0x80,0x07,0x24,0x38,0xD0,0x24};
ROMDATA BYTE Video_mode1080P30[16]={0x98,0x08,0x58,0x2C,0x00,0x94,0x65,0x40,0x04,0x05,0x80,0x07,0x24,0x38,0x18,0x14};
ROMDATA BYTE Video_mode1080P29[16]={0x98,0x08,0x58,0x2C,0x00,0x94,0x65,0x40,0x04,0x05,0x80,0x07,0x24,0x38,0x18,0x14};
ROMDATA BYTE Video_mode1080P25[16]={0x50,0x0A,0x10,0x2C,0x02,0x94,0x65,0x40,0x04,0x05,0x80,0x07,0x24,0x38,0xD0,0x24};
ROMDATA BYTE Video_mode1080P24[16]={0xBE,0x0A,0x7E,0x2C,0x02,0x94,0x65,0x40,0x04,0x05,0x80,0x07,0x24,0x38,0x3E,0x34};
ROMDATA BYTE Video_mode720P120[16]={0x72,0x06,0x6E,0x28,0x00,0xDC,0xEE,0x20,0x05,0x05,0x00,0x05,0x14,0xD0,0x72,0x12};
ROMDATA BYTE Video_mode720P100[16]={0xBC,0x07,0xB8,0x28,0x01,0xDC,0xEE,0x20,0x05,0x05,0x00,0x05,0x14,0xD0,0xBC,0x22};
ROMDATA BYTE Video_mode3840x2160p30[16]={0x30,0x11,0xB0,0x58,0x00,0x28,0xCA,0x81,0x08,0x0A,0x00,0x0F,0x48,0x70,0x30,0x28};
ROMDATA BYTE Video_mode3840x2160p29[16]={0x30,0x11,0xB0,0x58,0x00,0x28,0xCA,0x81,0x08,0x0A,0x00,0x0F,0x48,0x70,0x30,0x28};
ROMDATA BYTE Video_mode3840x2160p25[16]={0xA0,0x14,0x20,0x58,0x04,0x28,0xCA,0x81,0x08,0x0A,0x00,0x0F,0x48,0x70,0xA0,0x58};
ROMDATA BYTE Video_mode3840x2160p24[16]={0x7C,0x15,0xFC,0x58,0x04,0x28,0xCA,0x81,0x08,0x0A,0x00,0x0F,0x48,0x70,0x7C,0x68};
ROMDATA BYTE Video_mode4096x2160p30[16]={0x30,0x11,0x58,0x58,0x00,0x80,0xCA,0x80,0x08,0x0A,0x00,0x10,0x48,0x70,0x30,0x18};
ROMDATA BYTE Video_mode4096x2160p29[16]={0x30,0x11,0x58,0x58,0x00,0x80,0xCA,0x80,0x08,0x0A,0x00,0x10,0x48,0x70,0x30,0x18};
ROMDATA BYTE Video_mode4096x2160p25[16]={0xA0,0x14,0xC8,0x58,0x03,0x80,0xCA,0x80,0x08,0x0A,0x00,0x10,0x48,0x70,0xA0,0x48};
ROMDATA BYTE Video_mode4096x2160p24[16]={0x7C,0x15,0xFC,0x58,0x03,0x28,0xCA,0x81,0x08,0x0A,0x00,0x10,0x48,0x70,0x7C,0x58};
	
#define LT8618EX_BT709 0
#define LT8618EX_BT601 1
	
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------

AVI_Infoframe_t stInfoframe ; // {VIC(Video ID) , Y( avi packet output colormode:(Y1:Y0)=00(RGB),01(YC422),10(YC444))}

BYTE	I2CADR;

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
BYTE LT8618EX_MultiWrite(BYTE sAddr, BYTE rAddr, PBYTE pBuff, WORD bytes)
{
#if defined(__USE_I2C_GPIO__)
	return I2CGPIO_Write(sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes);
#else
	return I2C_Write(TWI_CH0, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes); 	// addr(8bit), data(8bit)
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE LT8618EX_MultiRead(BYTE sAddr, BYTE rAddr, PBYTE pBuff, WORD bytes)
{
#if defined(__USE_I2C_GPIO__)
	return I2CGPIO_Read(sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes);//bh test
#else
	return I2C_Read(TWI_CH0, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes); 	// addr(8bit), data(8bit)
#endif
}
//--------------------------------------------------------------------------------------------------------------------------
void LT8618EX_RegWrite(BYTE sAddr, BYTE rAddr, BYTE data)
{
	LT8618EX_MultiWrite(sAddr, rAddr, (PBYTE)&data, 1);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE LT8618EX_RegRead(BYTE sAddr, BYTE rAddr)
{
	BYTE data;
	LT8618EX_MultiRead(sAddr, rAddr, (PBYTE)&data, 1);
	return data;
}
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
static void ChipInit(void)
{
	I2CADR = LT8618EX_ADR;
	// LT8618EX_RegWrite(LT8618EX_ADR, 0x02,0x06);
	// LT8618EX_RegWrite(LT8618EX_ADR, 0x03,0x00);
	// LT8618EX_RegWrite(LT8618EX_ADR, 0x04,0x72); // soft reset for module
	// LT8618EX_RegWrite(LT8618EX_ADR, 0x05,0x10); // soft reset for hdcp
	// LT8618EX_RegWrite(LT8618EX_ADR, 0x06,0xff); // power down receiver
	// LT8618EX_RegWrite(LT8618EX_ADR, 0x07,0x00);
	// LT8618EX_RegWrite(LT8618EX_ADR, 0x08,0xe1);
	// LT8618EX_RegWrite(LT8618EX_ADR, 0x09,0xff); // clock enabl
	
	////////////
	LT8618EX_RegWrite(LT8618EX_ADR,0x02,0x3f); 
	LT8618EX_RegWrite(LT8618EX_ADR,0x03,0x00);
	LT8618EX_RegWrite(LT8618EX_ADR,0x04,0x72); // soft reset for module
	LT8618EX_RegWrite(LT8618EX_ADR,0x05,0x10); // soft reset for hdcp
	LT8618EX_RegWrite(LT8618EX_ADR,0x06,0xff); // power down receiver
	LT8618EX_RegWrite(LT8618EX_ADR,0x07,0x00);
	LT8618EX_RegWrite(LT8618EX_ADR,0x09,0xff); // clock enable

	LT8618EX_RegWrite(LT8618EX_ADR,0x5f,0xa2);
	//LT8618EX_RegWrite(LT8618EX_ADR,0x60,0x98);
	//LT8618EX_RegWrite(LT8618EX_ADR,0x61,0x02);
	//LT8618EX_RegWrite(LT8618EX_ADR,0x2f,0x36);
	LT8618EX_RegWrite(LT8618EX_ADR,0x40,0x40);
	//LT8618EX_RegWrite(LT8618EX_ADR,0x06,0xfd);	
	
	LT8618EX_RegWrite(LT8618EX_ADR, 0x3e,0x00); //rg_txpll_rptck_sel[5:4] ,rg_txpll_pixck_sel[3:2],rg_txpll_prediv_sel[1:0]
}
//--------------------------------------------------------------------------------------------------------------------------
static void Hdcpmode(void)
{
	LT8618EX_RegWrite(LT8618EX_ADR, 0x02,0x86);
	LT8618EX_RegWrite(LT8618EX_ADR, 0x05,0xff);
	LT8618EX_RegWrite(LT8618EX_ADR, 0x08,0xff);
}
//--------------------------------------------------------------------------------------------------------------------------
static void Sourceselect(void)
{
	LT8618EX_RegWrite(LT8618EX_ADR, 0x2b,0x10);	// rgb input ch_sel[2:0] = b000 (R,G,B)
//	LT8618EX_RegWrite(LT8618EX_ADR, 0x2b,0x10|1); // rgb input ch_sel[2:0] = b001 (R,G,B)
//	LT8618EX_RegWrite(LT8618EX_ADR, 0x2b,0x10|2);	// rgb input ch_sel[2:0] = b001 (R,G,B)
//	LT8618EX_RegWrite(LT8618EX_ADR, 0x2b,0x10|3);	// rgb input ch_sel[2:0] = b001 (R,G,B)
//	LT8618EX_RegWrite(LT8618EX_ADR, 0x2b,0x10|4);	// rgb input ch_sel[2:0] = b001 (R,G,B)
//	LT8618EX_RegWrite(LT8618EX_ADR, 0x2b,0x10|5);	// rgb input ch_sel[2:0] = b001 (R,G,B)
}
//--------------------------------------------------------------------------------------------------------------------------
static void Swingset(void)
{
	LT8618EX_RegWrite(LT8618EX_ADR, 0x3B,0x78);	// [6:0] data driver swing control
	LT8618EX_RegWrite(LT8618EX_ADR, 0x3d,0x74);	// [6:0] clock driver swing control
}
//--------------------------------------------------------------------------------------------------------------------------
static void TxPllconfig(void)
{
	LT8618EX_RegWrite(LT8618EX_ADR, 0x3f,0x80);	// 1: TX PLL lock signal is invalid.
	LT8618EX_RegWrite(LT8618EX_ADR, 0x40,0x40);
//	LT8618EX_RegWrite(LT8618EX_ADR, 0x40,0x84);
}
//--------------------------------------------------------------------------------------------------------------------------
// static void Hdmimodepol(void)
// {
	// LT8618EX_RegWrite(LT8618EX_ADR, 0x78,0x8c);	// hdmi output [7]:1=hdmi mode, hs/vs polarity 
// }
//--------------------------------------------------------------------------------------------------------------------------
static void Audioconfig(void)
{
	// I2CADR = LT8618EX_ADR_last;
	LT8618EX_RegWrite(LT8618EX_ADR_last, 0x06,0x08);
	LT8618EX_RegWrite(LT8618EX_ADR_last, 0x07,0x10);
	// I2CADR = LT8618EX_ADR;
}
//--------------------------------------------------------------------------------------------------------------------------
static BYTE LT8618EX_Colorspace(BYTE color)
{
	BYTE output_Color;
	// BYTE is444_upscale;
	
	LT8618EX_RegWrite(LT8618EX_ADR, 0x8f,color);	
	
	// is444_upscale = ((color&0x10)>>4) ;	
	
	switch(color) // Output Color indicator for infoframe
	{
		case Color_BYPASSUS : 
		case Color_RGB444 : 
			output_Color = Out_Ycbcr444 ; 
			break ;
		case Color_BYPASS : 
		default : 
			output_Color = Out_RGB444 ;
			break ;
	}	
	return output_Color ;
}
//--------------------------------------------------------------------------------------------------------------------------
static void Systeminit(void)
{
	ChipInit();		// 1st : 0x02~09
	Sourceselect();	// 1st : 0x2b
	// Hdmimodepol();	// 1st : 0x78
	Swingset();		// 1st : 0x3b, 3d
	TxPllconfig();	// 1st : 0x3f, 40
	
	Audioconfig();	// 2nd : 0x06,07
	Hdcpmode();		// 1st : 0x02,05,08
}

//--------------------------------------------------------------------------------------------------------------------------
static void LT8618EX_CalcChecksum(void)
{
	WORD addr;
	BYTE rVal;
	BYTE checksum=0x00;

	BYTE infoFrame_type=0xff;
	BYTE infoFrame_version=0xff;
	BYTE infoFrame_length=0xff;

	for(addr=0x40; addr<=0x42; addr++) {
		// LT8618EX_MultiRead(addr, &rVal, 1);
		rVal = LT8618EX_RegRead(LT8618EX_ADR_last,addr);

		// if(addr==0x40) { infoFrame_type   =(rVal&0x7f); checksum-=rVal; }
		if(addr==0x40) { infoFrame_type     =(rVal&0xff); checksum-=rVal; } // 0x82
		if(addr==0x41) { infoFrame_version=(rVal&0xff); checksum-=rVal; } // 0x02
		if(addr==0x42) { infoFrame_length  =(rVal&0x1f); checksum-=rVal; } // 0x0d
	}

	if(!(infoFrame_type+infoFrame_version+infoFrame_length)) UARTprintf("++!! check infoFrame data!!++\r\n");

	// 0x43 is checksum
	for(addr=0x44; addr<(0x44+infoFrame_length); addr++) {
		if(addr>0x52) { UARTprintf("!! check infoFrame_length (%d)\n", infoFrame_length); break; }
		// LT8618EX_MultiRead(addr, &rVal, 1);
		rVal =  LT8618EX_RegRead(LT8618EX_ADR_last,addr);
		// UARTprintf("### 0x%03x : 0x%02x\r\n", addr, rVal);
		checksum-=rVal;
	}
#if 0
	UARTprintf("--> infoFrame_type:0x%02x\r\n", infoFrame_type);
	UARTprintf("--> infoFrame_version:0x%02x\r\n", infoFrame_version);
	UARTprintf("--> infoFrame_length:0x%02x\r\n", infoFrame_length);
	UARTprintf("--> checksum:0x%02x\r\n", checksum);
#endif

	// write checksum
	LT8618EX_RegWrite(LT8618EX_ADR_last,0x43, checksum); // PB0 : checksum
}
//--------------------------------------------------------------------------------------------------------------------------
static void AVI_Inforfram_Packetconfig(void)
{
	LT8618EX_RegWrite(LT8618EX_ADR_last,0x44, stInfoframe.Y<<5);  // // PB1 : avi packet output colormode [6:5](Y1:Y0)=00(RGB),01(YC422),10(YC444),11(Reserved)
	LT8618EX_RegWrite(LT8618EX_ADR_last,0x45, 0xa8);              // PB2 :  0x19->0xa8 (colorimetry:itu709, aspect ratio:16:9)
	LT8618EX_RegWrite(LT8618EX_ADR_last,0x46, 0x10);              // PB3 : 0x00->0x10 (xvYCC709)
	LT8618EX_RegWrite(LT8618EX_ADR_last,0x47, stInfoframe.VIC);   // PB4 : set VIC
	LT8618EX_RegWrite(LT8618EX_ADR_last,0x48, 0x10);              // PB5 :  0x00 (No Repetition)

	// calc. checksum
	LT8618EX_CalcChecksum();
}
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//

void LT8618EX_Init(void)
{
	BYTE CSC_result ;
//	BYTE VIC_result ;
	
//	RESET_Lt8618EX();	// reset GPIOD2 (low -> high)
//	Initstate();		// gpio setting for i2c
	Systeminit();		// init of LT8618EX

#if LT8618EX_INIT_CSC==0
	CSC_result = LT8618EX_Colorspace(Color_Ycbcr422BT601); // hdmi csc YCbCr422->RGB444 BT601
#else
	CSC_result = LT8618EX_Colorspace(Color_Ycbcr422BT709); // hdmi csc YCbCr422->RGB444 BT709
#endif
//	VIC_result = LT8618EX_OutputVideoFormat(LT8618_VID_1080P60); // Resolution 1080P60 

//	LT8618EX_SetAviInfo(VIC_result,CSC_result); // Making infoframe only.
	LT8618EX_SetFrmt();
	
	if(IsOUT_MainSyncEmb())	{	// vout sync embedded
		LT8618EX_Rev_Out_Mode(LT8618_SYNC_EMBD,LT8618_SIGN_HDMI); //config LT8618ex input sync mode
	}
	else	{
		// LT8618EX_Rev_Out_Mode(LT8618_SYNC_SEPA,LT8618_SIGN_DVI); //config LT8618ex input sync  mode
		LT8618EX_Rev_Out_Mode(LT8618_SYNC_SEPA,LT8618_SIGN_HDMI); //config LT8618ex input sync  mode
	}

	LT8618EX_RegWrite(LT8618EX_ADR, 0x3a,0x0f);	// 1st : control of bias, bandgap, drive current for clock
	LT8618EX_RegWrite(LT8618EX_ADR, 0x3c,0x64);	// 1st : control of bias, bandgap, drive current for data 
//	LT8618EX_RegWrite(LT8618EX_ADR, 0x2e,0x00);	// 1st : [1]=0 (normal mode), delay, input
//	LT8618EX_RegWrite(LT8618EX_ADR, 0x2e,0x04);	// 1st : [1]=0 (normal mode), delay, input
//	LT8618EX_RegWrite(LT8618EX_ADR, 0x2e,0x08);	// 1st : [1]=0 (normal mode), delay, input
	LT8618EX_RegWrite(LT8618EX_ADR, 0x2e,0x0c);	// 1st : [1]=0 (normal mode), delay, input (ori)
	LT8618EX_RegWrite(LT8618EX_ADR, 0x2f,0x16);	// 1st :  (ori)
//	LT8618EX_RegWrite(LT8618EX_ADR, 0x2f,0x36);	// 1st : [5]=1: video input clock(id_clk), 0: hdmi rx clock.
	// I2CADR = LT8618EX_ADR_last;
	// LT8618EX_RegWrite(LT8618EX_ADR_last, 0x43,0x36);	// 2nd : AVI infoframe (PB0) Checksum
	// LT8618EX_RegWrite(LT8618EX_ADR_last, 0x44,0x10);	// 2nd : AVI infoframe (PB1) [6:5](Y1:Y0)=00(RGB),01(YC422),10(YC444),11(Reserved)
	// LT8618EX_RegWrite(LT8618EX_ADR_last, 0x45,0x19);	// 2nd : AVI infoframe (PB2) colorimetry, aspect ratio(4:3)
	// LT8618EX_RegWrite(LT8618EX_ADR_last, 0x47,0x10);	// 2nd : AVI infoframe (PB4) VIC6~VIC0
	
}

//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//
void LT8618EX_SetCSC(BYTE mode)
{
	UARTprintf("HDMITxCSC : [%d] ",mode) ;

	if(mode==LT8618EX_BT601) {
		LT8618EX_SetAviInfo(stInfoframe.VIC,LT8618EX_Colorspace(Color_Ycbcr422BT601));
		UARTprintf("Ycbcr422->RGB444 BT601\r\n");
	} // CSC Ycbcr422->RGB444 BT601
	else if(mode==LT8618EX_BT709){
		LT8618EX_SetAviInfo(stInfoframe.VIC,LT8618EX_Colorspace(Color_Ycbcr422BT709));
		UARTprintf("Ycbcr422->RGB444 BT709\r\n");		
	} // CSC Ycbcr422->RGB444 BT709		
	else{		
		LT8618EX_SetAviInfo(stInfoframe.VIC,LT8618EX_Colorspace(Color_BYPASS));
		UARTprintf("M400 quad 422 bypass\r\n");
	}  // CSC bypass : Ycbcr422->Ycbcr422
}

//--------------------------------------------------------------------------------------------//
BYTE LT8618EX_GetCSC(void)
{
	switch(LT8618EX_RegRead(LT8618EX_ADR, 0x8f))	{
		case Color_Ycbcr422BT709:	return 0;	break;	// RGB444(bt709)
		case Color_Ycbcr422BT601:	return 1;	break;	// RGB444(bt601)
		case Color_BYPASS:			return 2;	break;	// YC422(for M400_quad)
	}
	
	return 0;
}

//--------------------------------------------------------------------------------------------//
void LT8618EX_SetAviInfo(BYTE VIC, BYTE Y)
{
	stInfoframe.VIC=VIC;
	stInfoframe.Y=Y;
	AVI_Inforfram_Packetconfig();

	// switch(VIC) {  //set default video input format,videoformat defined by main chipset
		// case  4:  LT8618EX_OutputVideoFormat(LT8618_VID_720P60);   break;
		// case 19:  LT8618EX_OutputVideoFormat(LT8618_VID_720P25);   break;  // 1280x720p50
		// case  5:  LT8618EX_OutputVideoFormat(LT8618_VID_1080I60);  break;
		// case 16:  LT8618EX_OutputVideoFormat(LT8618_VID_1080P60);  break;
		// case 95:  LT8618EX_OutputVideoFormat(LT8618_VID_2160P30);  break;
		// case 97:  LT8618EX_OutputVideoFormat(LT8618_VID_2160P60);  break;
		// default:  LT8618EX_OutputVideoFormat(LT8618_VID_1080P60);  break;
	// }
}
//--------------------------------------------------------------------------------------------------------------------------
BYTE LT8618EX_OutputVideoFormat(LT8618_VID_FRMT format)
{
	BYTE i, VIC;
	PBYTE pMode;
	
	switch(format) {
		case LT8618_VID_720P60:		pMode=(PBYTE)Video_mode720P60;		VIC =  4;	break;
		case LT8618_VID_720P59:		pMode=(PBYTE)Video_mode720P59;		VIC =  4;	break;
		case LT8618_VID_720P50:		pMode=(PBYTE)Video_mode720P50;		VIC = 19;	break;
		case LT8618_VID_720P30:		pMode=(PBYTE)Video_mode720P30;		VIC =  4;	break;
		case LT8618_VID_720P25:		pMode=(PBYTE)Video_mode720P25;		VIC = 19;	break;
		case LT8618_VID_720P24:		pMode=(PBYTE)Video_mode720P24;		VIC = 19;	break;
		case LT8618_VID_1080I60:	pMode=(PBYTE)Video_mode1080i60;		VIC =  5;	break;
		case LT8618_VID_1080I59:	pMode=(PBYTE)Video_mode1080i59;		VIC =  5;	break;
		case LT8618_VID_1080I50:	pMode=(PBYTE)Video_mode1080i50;		VIC = 20;	break;
		case LT8618_VID_1080P60:	pMode=(PBYTE)Video_mode1080P60;		VIC = 16;	break;
		case LT8618_VID_1080P59:	pMode=(PBYTE)Video_mode1080P59;		VIC = 16;	break;
		case LT8618_VID_1080P50:	pMode=(PBYTE)Video_mode1080P50;		VIC = 31;	break;
		case LT8618_VID_1080P30:	pMode=(PBYTE)Video_mode1080P30;		VIC = 34;	break;
		case LT8618_VID_1080P29:	pMode=(PBYTE)Video_mode1080P29;		VIC = 34;	break;
		case LT8618_VID_1080P25:	pMode=(PBYTE)Video_mode1080P25;		VIC = 33;	break;
		case LT8618_VID_1080P24:	pMode=(PBYTE)Video_mode1080P24;		VIC = 32;	break;
		case LT8618_VID_720P120:	pMode=(PBYTE)Video_mode720P120;		VIC = 47;	break;
		case LT8618_VID_720P100:	pMode=(PBYTE)Video_mode720P100;		VIC = 41;	break;
		
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)
		case LT8618_VID_3840x2160P30:	pMode=(PBYTE)Video_mode3840x2160p30;	VIC = 95;	break;
		case LT8618_VID_3840x2160P29:	pMode=(PBYTE)Video_mode3840x2160p29;	VIC = 95;	break;
		case LT8618_VID_3840x2160P25:	pMode=(PBYTE)Video_mode3840x2160p25;	VIC = 94;	break;
		case LT8618_VID_3840x2160P24:	pMode=(PBYTE)Video_mode3840x2160p24;	VIC = 93;	break;
		case LT8618_VID_4096x2160P30:	pMode=(PBYTE)Video_mode4096x2160p30;	VIC =100;	break;
		case LT8618_VID_4096x2160P29:	pMode=(PBYTE)Video_mode4096x2160p29;	VIC =100;	break;
		case LT8618_VID_4096x2160P25:	pMode=(PBYTE)Video_mode4096x2160p25;	VIC = 99;	break;
		case LT8618_VID_4096x2160P24:	pMode=(PBYTE)Video_mode4096x2160p24;	VIC = 98;	break;
#endif
		default:					pMode=(PBYTE)Video_mode1080P60;		VIC = 16;	break;
	}

	for(i=0;i<16;i++)	{
		LT8618EX_RegWrite(LT8618EX_ADR, 0x07C+i, (i==8)?  pMode[i]+1:  pMode[i]);	// VFP +1 (chip bug ??)
	}

	return VIC ;	
}
//--------------------------------------------------------------------------------------------------------------------------
void LT8618EX_Rev_Out_Mode(LT8618_SYNC_MODE sync, LT8618_SIGN_MODE sign)
{
	LT8618EX_RegWrite(LT8618EX_ADR,0x03e,0x00);
	if(sync==LT8618_SYNC_SEPA) { // jins, these routine seems that dependent about embedded/separated sync. not color space
		if(sign==LT8618_SIGN_HDMI)  { LT8618EX_RegWrite(LT8618EX_ADR,0x78,0x8c);  LT8618EX_RegWrite(LT8618EX_ADR,0xba,0x3c); }
		else                                        { LT8618EX_RegWrite(LT8618EX_ADR,0x78,0x0c);  LT8618EX_RegWrite(LT8618EX_ADR,0xba,0x34); }
		LT8618EX_RegWrite(LT8618EX_ADR,0x79,0x00);
	}
	else {
		if(sign==LT8618_SIGN_HDMI) { LT8618EX_RegWrite(LT8618EX_ADR,0x78,0x8f);  LT8618EX_RegWrite(LT8618EX_ADR,0xba,0x3c); }
		else                                       { LT8618EX_RegWrite(LT8618EX_ADR,0x78,0x0f);  LT8618EX_RegWrite(LT8618EX_ADR,0xba,0x34); }
		LT8618EX_RegWrite(LT8618EX_ADR,0x79,0x20);
	}
}

//--------------------------------------------------------------------------------------------//
void LT8618EX_SetFrmt(void)
{
	BYTE CSC_result, VIC_result, frmt;
	
	switch(GetOUT_MainFrmt())	{
		case MISP_VIDOUT_1280x720p60:	frmt = LT8618_VID_720P60;			break;
		case MISP_VIDOUT_1280x720p50:	frmt = LT8618_VID_720P50;			break;
		case MISP_VIDOUT_1280x720p30:	frmt = LT8618_VID_720P30;			break;
		case MISP_VIDOUT_1280x720p25:	frmt = LT8618_VID_720P25;			break;
		case MISP_VIDOUT_1280x720p24:	frmt = LT8618_VID_720P24;			break;
		case MISP_VIDOUT_1920x1080i60:	frmt = LT8618_VID_1080I60;			break;
		case MISP_VIDOUT_1920x1080i50:	frmt = LT8618_VID_1080I50;			break;
		case MISP_VIDOUT_1920x1080p60:	frmt = LT8618_VID_1080P60;			break;
		case MISP_VIDOUT_1920x1080p50:	frmt = LT8618_VID_1080P50;			break;
		case MISP_VIDOUT_1920x1080p30:	frmt = LT8618_VID_1080P30;			break;
		case MISP_VIDOUT_1920x1080p25:	frmt = LT8618_VID_1080P25;			break;
		case MISP_VIDOUT_1920x1080p24:	frmt = LT8618_VID_1080P24;			break;
		case MISP_VIDOUT_1280x720p120:	frmt = LT8618_VID_720P120;			break;
		case MISP_VIDOUT_1280x720p100:	frmt = LT8618_VID_720P100;			break;
		
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
		case MISP_VIDOUT_3840x2160p30:	frmt = LT8618_VID_3840x2160P30;		break;
		case MISP_VIDOUT_3840x2160p25:	frmt = LT8618_VID_3840x2160P25;		break;
		case MISP_VIDOUT_3840x2160p24:	frmt = LT8618_VID_3840x2160P24;		break;
		case MISP_VIDOUT_3840x2160p15:	frmt = LT8618_VID_3840x2160P15;		break;
		case MISP_VIDOUT_3840x2160p12:	frmt = LT8618_VID_3840x2160P12;		break;
		case MISP_VIDOUT_4096x2160p30:	frmt = LT8618_VID_4096x2160P30;		break;
		case MISP_VIDOUT_4096x2160p25:	frmt = LT8618_VID_4096x2160P25;		break;
		case MISP_VIDOUT_4096x2160p24:	frmt = LT8618_VID_4096x2160P24;		break;
#endif
		
#if defined(__USE_59Hz_OUTPUT__)
		case MISP_VIDOUT_1280x720p59:	frmt = LT8618_VID_720P59;			break;
		case MISP_VIDOUT_1920x1080i59:	frmt = LT8618_VID_1080I59;			break;
		case MISP_VIDOUT_1920x1080p59:	frmt = LT8618_VID_1080P59;			break;
		case MISP_VIDOUT_1920x1080p29:	frmt = LT8618_VID_1080P29;			break;
#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i540, i550
		case MISP_VIDOUT_3840x2160p29:	frmt = LT8618_VID_3840x2160P29;		break;
		case MISP_VIDOUT_4096x2160p29:	frmt = LT8618_VID_4096x2160P29;		break;
#endif
#endif
		default:						frmt = LT8618_VID_1080P60;			break;
	}
	
	CSC_result = LT8618EX_Colorspace(LT8618EX_RegRead(LT8618EX_ADR, 0x8f));
	VIC_result = LT8618EX_OutputVideoFormat(frmt);
	
	LT8618EX_SetAviInfo(VIC_result,CSC_result); // Making infoframe only.
}


#endif	//__USE_LT8618EX_TX__
