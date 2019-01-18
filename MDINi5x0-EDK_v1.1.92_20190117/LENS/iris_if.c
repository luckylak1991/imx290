//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2005  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	IRIS_IF.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

#if defined(__USE_AN41919__)
//--------------------------------------------------------------------------------------------------------------------------
static void AN41919_Initialize(void)
{
	IRIS_VD(HIGH);	IRIS_VD(LOW);
	IRIS_WordWrite(SPI_AN41919_ADDR, 0x000, 0x02c0);	// Luma Target
	IRIS_WordWrite(SPI_AN41919_ADDR, 0x001, 0x6000);	// PWM Enable

#if defined(__USE_CMOS_IMXx36__)
	IRIS_WordWrite(SPI_AN41919_ADDR, 0x006, 0x006d);	// Offset DAC
#else
	IRIS_WordWrite(SPI_AN41919_ADDR, 0x006, 0x0060);	// Offset DAC
#endif

	IRIS_WordWrite(SPI_AN41919_ADDR, 0x007, 0x0407);	// Analog Adder
	IRIS_WordWrite(SPI_AN41919_ADDR, 0x008, 0x9900);	// Analog Gain
	IRIS_WordWrite(SPI_AN41919_ADDR, 0x009, 0x0401);	// IRIS Enable
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE AN41919_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	IRIS_CS(HIGH);
	err = SPI_Write(SPI_AN41919_ADDR, rAddr, pBuff, bytes);
	IRIS_CS(LOW);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE AN41919_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	WORD i; BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	for (i=0; i<bytes/2; i++) {	// not consecutive read
		IRIS_CS(HIGH);
		err |= SPI_Read(SPI_AN41919_ADDR, rAddr+i, pBuff+2*i, 2);
		IRIS_CS(LOW);
	}

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE IRIS_MultiWrite(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return AN41919_SPIWrite(rAddr, pBuff, bytes);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE IRIS_WordWrite(BYTE sAddr, WORD rAddr, WORD wData)
{
	return IRIS_MultiWrite(sAddr, rAddr, (PBYTE)&wData, 2);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE IRIS_MultiRead(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return AN41919_SPIRead(rAddr, pBuff, bytes);
}

//--------------------------------------------------------------------------------------------------------------------------
WORD IRIS_WordRead(BYTE sAddr, WORD rAddr)
{
	WORD rData = 0;

	IRIS_MultiRead(sAddr, rAddr, (PBYTE)&rData, 2);
	return rData;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE IRIS_WordField(BYTE sAddr, WORD rAddr, WORD bPos, WORD bCnt, WORD bData)
{
	WORD temp;
	temp = IRIS_WordRead(sAddr, rAddr);
	bCnt = ~(0xffff<<bCnt);
	temp &= ~(bCnt<<bPos);
	temp |= ((bData&bCnt)<<bPos);
	return IRIS_WordWrite(sAddr, rAddr, temp);
}
#endif

#if defined(__USE_AN41908__)
//--------------------------------------------------------------------------------------------------------------------------
static void AN41908_Initialize(void)
{
	IRIS_VD(HIGH);	IRIS_VD(LOW);

#if defined(__USE_DCIRIS_HALL__)
#if defined(__USE_OLPF_SUNNY__)		//use sunny-zoom lens
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x001, 0x2000);	 // DGAIN[15:9] = 0x8
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x004, 0x877E);	 // HALL_OFFSET_DAC[15:08], HALL_BIAS_OFFSET[07:00]
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x005, 0x0500);	 // HALL_GAIN_DAC[11:08] PID_INV[5]
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x00E, 0x1B00);	 // AVE_SPEED[D12:D8] = 14 for 60Hz, 27 for 30Hz

#elif defined(__USE_OLPF_CANON__)		//use canon-zoom lens
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x001, 0x2000);	 // DGAIN[15:9] = 0x8
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x004, 0x832E);	 // HALL_OFFSET_DAC[15:08], HALL_BIAS_OFFSET[07:00]
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x005, 0x0520);	 // HALL_GAIN_DAC[11:08] PID_INV[5]
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x00E, 0x0E00);	 // AVE_SPEED[D12:D8] = 14 for 60Hz, 27 for 30Hz

#elif defined(__USE_X30X_TAMRON__)		//use tamron-zoom lens
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x001, 0x2000);	 // DGAIN[15:9] = 0x8
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x004, 0x942D);	 // HALL_OFFSET_DAC[15:08], HALL_BIAS_OFFSET[07:00]
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x005, 0x0500);	 // HALL_GAIN_DAC[11:08]
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x00E, 0x0E00);	 // AVE_SPEED[D12:D8] = 14 for 60Hz, 27 for 30Hz

#elif defined(__USE_X10X_UNION__)		//use union-zoom lens
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x001, 0x7E00);	 // 0C00:DGAIN[15:9] = 0x6 --> 7E00:63
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x004, 0x8042);	 // HALL_OFFSET_DAC[15:08], HALL_BIAS_OFFSET[07:00]
	#if defined(__USE_MDIN_i5XX_REV__)
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x005, 0x0200);	 // HALL_GAIN_DAC[11:08]
	#else
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x005, 0x0300);	 // HALL_GAIN_DAC[11:08]
	#endif
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x00E, 0x0E00);	 // AVE_SPEED[D12:D8] = 14 for 60Hz, 27 for 30Hz

#elif defined(__USE_X10X_ZMAX__)		//use zmax-zoom lens
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x001, 0x7E00);	 // 0C00:DGAIN[15:9] = 0x6 --> 7E00:63
	//IRIS_WordWrite(SPI_AN41908_ADDR, 0x004, 0x9F57);	 // HALL_OFFSET_DAC[15:08], HALL_BIAS_OFFSET[07:00]
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x004, 0x8942);	 // HALL_OFFSET_DAC[15:08], HALL_BIAS_OFFSET[07:00]
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x005, 0x0300);	 // HALL_GAIN_DAC[11:08]
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x00E, 0x0E00);	 // AVE_SPEED[D12:D8] = 14 for 60Hz, 27 for 30Hz

#elif defined(__USE_X12X_UNION__)		//use union-zoom lens
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x001, 0x7E00);	 // 0C00:DGAIN[15:9] = 0x6 --> 7E00:63
	//IRIS_WordWrite(SPI_AN41908_ADDR, 0x004, 0x9F57);	 // HALL_OFFSET_DAC[15:08], HALL_BIAS_OFFSET[07:00]
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x004, 0x8942);	 // HALL_OFFSET_DAC[15:08], HALL_BIAS_OFFSET[07:00]
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x005, 0x0300);	 // HALL_GAIN_DAC[11:08]
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x00E, 0x0E00);	 // AVE_SPEED[D12:D8] = 14 for 60Hz, 27 for 30Hz

#elif defined(__USE_X33X_CHINA__)		//use china-zoom lens
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x001, 0x2000);	 // DGAIN[15:9] = 0x8
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x004, 0x8769);	 // HALL_OFFSET_DAC[15:08], HALL_BIAS_OFFSET[07:00]
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x005, 0x0500);	 // HALL_GAIN_DAC[11:08]
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x00E, 0x0E00);	 // AVE_SPEED[D12:D8] = 14 for 60Hz, 27 for 30Hz

#elif defined(__USE_X03X_ZMAX__)
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x001, 0x7E00);	 // 0C00:DGAIN[15:9] = 0x6 --> 7E00:63
	//IRIS_WordWrite(SPI_AN41908_ADDR, 0x004, 0x9F57);	 // HALL_OFFSET_DAC[15:08], HALL_BIAS_OFFSET[07:00]
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x004, 0x7D46);	 // HALL_OFFSET_DAC[15:08], HALL_BIAS_OFFSET[07:00]
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x005, 0x0320);	 // HALL_GAIN_DAC[11:08]
	IRIS_WordWrite(SPI_AN41908_ADDR, 0x00E, 0x0E00);	 // AVE_SPEED[D12:D8] = 14 for 60Hz, 27 for 30Hz

#endif

	IRIS_WordWrite(SPI_AN41908_ADDR, 0x00b, 0x0480);	// PDWEN[10] = (IRIS Enable)
#else
	WORD mode = (0<10)|(0<<9)|(0<<8)|(1<<7);	// PDWEN[10] = (IRIS Disable)
	LENS_WordWrite(SPI_AN41908_ADDR, 0x0B, mode);
#endif
	UARTprintf("\r\nIRIS 41908 Initialize...\r\n");
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE AN41908_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	IRIS_CS(HIGH);
	err = SPI_Write(SPI_AN41908_ADDR, rAddr, pBuff, bytes);
	IRIS_CS(LOW);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE AN41908_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	WORD i; BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	for (i=0; i<bytes/2; i++) {	// not consecutive read
		IRIS_CS(HIGH);
		err |= SPI_Read(SPI_AN41908_ADDR, rAddr+i, pBuff+2*i, 2);
		IRIS_CS(LOW);
	}

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE IRIS_MultiWrite(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return AN41908_SPIWrite(rAddr, pBuff, bytes);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE IRIS_WordWrite(BYTE sAddr, WORD rAddr, WORD wData)
{
	return IRIS_MultiWrite(sAddr, rAddr, (PBYTE)&wData, 2);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE IRIS_MultiRead(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return AN41908_SPIRead(rAddr, pBuff, bytes);
}

//--------------------------------------------------------------------------------------------------------------------------
WORD IRIS_WordRead(BYTE sAddr, WORD rAddr)
{
	WORD rData = 0;

	IRIS_MultiRead(sAddr, rAddr, (PBYTE)&rData, 2);
	return rData;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE IRIS_WordField(BYTE sAddr, WORD rAddr, WORD bPos, WORD bCnt, WORD bData)
{
	WORD temp;
	temp = IRIS_WordRead(sAddr, rAddr);
	bCnt = ~(0xffff<<bCnt);
	temp &= ~(bCnt<<bPos);
	temp |= ((bData&bCnt)<<bPos);
	return IRIS_WordWrite(sAddr, rAddr, temp);
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
void IRISDRV_Initialize(void)
{
#if defined(__USE_PWMIRIS_ONLY__)
	WORD offset = EEPROM_RegRead(EEPROM_EXPOSURE_IRIS_OFFSET);
	if((offset&0xff) == EEPROM_SAVE_DONE) {
		offset = (offset >> 8);
		DCLENS_IRIS_SetOffset(offset);
		UARTprintf("\r\n IRIS Offset : %d\r\n", DCLENS_IRIS_GetOffset());
	} else {
		UARTprintf("\r\n IRIS Offset is not initialised...\r\n");
	}

#if defined(__USE_PWM_ADC__)
	WORD adcval = EEPROM_RegRead(EEPROM_EXPOSURE_IRIS_ADC);
	if((adcval&0xff) == EEPROM_SAVE_DONE) {
		adcval = (adcval >> 8);
		DCLENS_IRIS_SetADC(adcval);
		UARTprintf("\r\n IRIS ADC : %d\r\n", DCLENS_IRIS_GetADC());
	} else {
		UARTprintf("\r\n IRIS ADC is not initialised...\r\n");
	}

#endif

#endif

#if defined(__USE_AN41908__)
	AN41908_Initialize();
#endif

#if defined(__USE_AN41919__)
	AN41919_Initialize();
#endif
}

/*  FILE_END_HERE */
