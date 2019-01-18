/************************************************************
*	ProjectName:	 Sil1136
*	FileName:	     Sil1136.c
*	BuildData:	     2016-06-27
*	Version：        V1.00
* 	Company:	     Silicon Image
* 	Made:	     	 Tommy Cho
************************************************************/
#include "video.h"


BYTE	I2CADR_S;

//--------------------------------------------------------------------------------------------------------------------------
BYTE Sil1136_MultiWrite(BYTE sAddr, BYTE rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Write(TWI_CH0, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes); 	// addr(8bit), data(8bit)
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE SIL1136_MultiRead(BYTE sAddr, BYTE rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Read(TWI_CH0, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes); 	// addr(8bit), data(8bit)
}

//--------------------------------------------------------------------------------------------------------------------------
void SIL1136_RegWrite(BYTE sAddr, BYTE rAddr, BYTE data)
{
	Sil1136_MultiWrite(sAddr, rAddr, (PBYTE)&data, 1);
	//Delay_mSec(1);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE Sil1136_RegRead(BYTE sAddr, BYTE rAddr)
{
	BYTE data;
	SIL1136_MultiRead(sAddr, rAddr, (PBYTE)&data, 1);
	return data;
}

void SIL1136_Init(void)
{
	//First TEST 1080p60 with Audio
	I2CADR_S = 0x72;	//0x72

	UARTprintf("***** SIL1136 Init Start !!!\n\r");
	
	Delay_mSec(1000);

	
		
//RGB_2_RGB (1080p @ 8bpp)

	SIL1136_RegWrite(I2CADR_S, 0xC7,0x00);	//Enable HW TPI
	SIL1136_RegWrite(I2CADR_S, 0x2A,0x00);	//HDCP Off
	SIL1136_RegWrite(I2CADR_S, 0x1A,0x11);	//TMDS off, HDMI out
	SIL1136_RegWrite(I2CADR_S, 0x1E,0x00);	// D0 Full operation
	SIL1136_RegWrite(I2CADR_S, 0x00,0x00);
	SIL1136_RegWrite(I2CADR_S, 0x01,0x00);
	SIL1136_RegWrite(I2CADR_S, 0x02,0x00);
	SIL1136_RegWrite(I2CADR_S, 0x03,0x00);
	SIL1136_RegWrite(I2CADR_S, 0x04,0x00);
	SIL1136_RegWrite(I2CADR_S, 0x05,0x00);
	SIL1136_RegWrite(I2CADR_S, 0x06,0x00);
	SIL1136_RegWrite(I2CADR_S, 0x07,0x00);
	SIL1136_RegWrite(I2CADR_S, 0x08,0x60);	//TMDS clk * 1, full wide	
	//SIL1136_RegWrite(I2CADR_S, 0x09,0x82);	//YCbCr 10bit	tommy		{0x72} [0x9] = 0x0  //RGB 8bpp
	//SIL1136_RegWrite(I2CADR_S, 0x0A,0x82);	//YCbCr 10bit	tommy		{0x72} [0xA] = 0x0  //RGB 8bpp
	SIL1136_RegWrite(I2CADR_S, 0x09,0x00);	//YCbCr 10bit	tommy		{0x72} [0x9] = 0x0  //RGB 8bpp
	SIL1136_RegWrite(I2CADR_S, 0x0A,0x00);	//YCbCr 10bit	tommy		{0x72} [0xA] = 0x0  //RGB 8bpp
	SIL1136_RegWrite(I2CADR_S, 0x40,0x04);	//DC Enable
	SIL1136_RegWrite(I2CADR_S, 0x0B,0x00);	// YC normal input
//	SIL1136_RegWrite(I2CADR_S, 0x1A,0x07);	//TMDS on, HDMI out

	SIL1136_RegWrite(I2CADR_S, 0x0C,0xF0);	// AVI Checksum (0x100 - ((0x82+0x02+0x0D) + sum of (0x0D~0x19)))
	SIL1136_RegWrite(I2CADR_S, 0x0D,0x20);	// Byte 1 - YCbCr422
	SIL1136_RegWrite(I2CADR_S, 0x0E,0x00);    // Byte 2 - ITU601, 16:9 Aspect
	SIL1136_RegWrite(I2CADR_S, 0x0F,0x00);
	SIL1136_RegWrite(I2CADR_S, 0x10,0x5F);	  // Byte 4 - VIC Code: 16 // 95
	SIL1136_RegWrite(I2CADR_S, 0x11,0x00);	  // Byte 5 - No pixel replication
	SIL1136_RegWrite(I2CADR_S, 0x12,0x00);
	SIL1136_RegWrite(I2CADR_S, 0x13,0x00);
	SIL1136_RegWrite(I2CADR_S, 0x14,0x00);
	SIL1136_RegWrite(I2CADR_S, 0x15,0x00);
	SIL1136_RegWrite(I2CADR_S, 0x16,0x00);
	SIL1136_RegWrite(I2CADR_S, 0x17,0x00);
	SIL1136_RegWrite(I2CADR_S, 0x18,0x00);
	SIL1136_RegWrite(I2CADR_S, 0x19,0x00);
	

// I2S setting - PCM
	SIL1136_RegWrite(I2CADR_S, 0x26,0x90);	//I2S, PCM
	SIL1136_RegWrite(I2CADR_S, 0x20,0x90);	//SCK Rising, MCLK/256
//		SIL1136_RegWrite(I2CADR_S, 0x20,0xB0);	//SCK Rising, MCLK/512 => 48
	//SIL1136_RegWrite(I2CADR_S, 0x1F,0x88);	//SD Select Enable, SD0, Auto Down-sample to basic
	SIL1136_RegWrite(I2CADR_S, 0x1F,0x80);	//SD Select Enable, SD0, Auto Down-sample to basic
	SIL1136_RegWrite(I2CADR_S, 0x27,0xD8);	//24bit, 48Khz, High Bit-Rate Audio Enable
		//SIL1136_RegWrite(I2CADR_S, 0x27,0x58);	//24bit, 48Khz, High Bit-Rate Audio Enable
	//	SIL1136_RegWrite(I2CADR_S, 0x27,0x5C);	//16bit, 48Khz, High Bit-Rate Audio Enable
	SIL1136_RegWrite(I2CADR_S, 0x21,0x00);	//I2S Channel Status Byte 0
	SIL1136_RegWrite(I2CADR_S, 0x22,0x00);	//I2S Channel Status Byte 1, Category Code
	SIL1136_RegWrite(I2CADR_S, 0x23,0x00);	//I2S Channel Status Byte 2, Source, Channel Number
	SIL1136_RegWrite(I2CADR_S, 0x24,0x02);	//I2S Channel Status Byte 3 48Khz
	SIL1136_RegWrite(I2CADR_S, 0x25,0x0B);	//I2S Channel Status Byte 4, Original f, sample length
	//	SIL1136_RegWrite(I2CADR_S, 0x25,0x02);	//I2S Channel Status Byte 4, Original f, sample length
	//SIL1136_RegWrite(I2CADR_S, 0x26,0x80);	//Audio Interface I2S PCM 설정 ?(레퍼런스는 Refer to stream Header 로 되어 있음.)
		SIL1136_RegWrite(I2CADR_S, 0x26,0x81);	//Audio Interface I2S PCM 설정 ?(레퍼런스는 Refer to stream Header 로 되어 있음.)


// Audio InfoFrame 
	SIL1136_RegWrite(I2CADR_S, 0xBF,0xC2);	//Enable, Repeat, Info Frame to load

	SIL1136_RegWrite(I2CADR_S, 0xC0,0x84);	//Type:InfoFrame, CEA-861-E spec 은 4 ? 여기에?? Aux data?
	SIL1136_RegWrite(I2CADR_S, 0xC1,0x01);	
	SIL1136_RegWrite(I2CADR_S, 0xC2,0x0A);	
	SIL1136_RegWrite(I2CADR_S, 0xC3,0x61);	//Checksum
	SIL1136_RegWrite(I2CADR_S, 0xC4,0x01);	
	SIL1136_RegWrite(I2CADR_S, 0xC5,0x0F);	
	SIL1136_RegWrite(I2CADR_S, 0xC6,0x00);	
	SIL1136_RegWrite(I2CADR_S, 0xC7,0x00);	
	SIL1136_RegWrite(I2CADR_S, 0xC8,0x00);	
	SIL1136_RegWrite(I2CADR_S, 0xC9,0x00);	
	SIL1136_RegWrite(I2CADR_S, 0xCA,0x00);	
	SIL1136_RegWrite(I2CADR_S, 0xCB,0x00);	
	SIL1136_RegWrite(I2CADR_S, 0xCC,0x00);	
	SIL1136_RegWrite(I2CADR_S, 0xCD,0x00);	
	
	//SIL1136_RegWrite(I2CADR_S, 0x60,0x80);	
	//차이가 없어~ SIL1136_RegWrite(I2CADR_S, 0x61,0x00);			//tommy Input VSYNC & HSYNC Polarity Active Low

	SIL1136_RegWrite(I2CADR_S, 0x1A,0x07);	//TMDS on, HDMI out

	UARTprintf("***** SIL1136 Init END !!!\n\r");
}

