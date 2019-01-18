//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2005  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	LENS_IF.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"


#if defined(__USE_AN41908__)||defined(__USE_GPIO_MTC_AF__)||defined(__USE_GPIO_MTC_AE__)

#if defined(__USE_GPIO_MTC_AE__)
//--------------------------------------------------------------------------------------------------
static void ISPM GpioMoterCtrlIRIS(SHORT step)
{

	switch(step) { // IRIS Driving
		case 0: GPIO_MEC_AP(HIGH); GPIO_MEC_AM(LOW); GPIO_MEC_BP(LOW); GPIO_MEC_BM(LOW); break;
		case 1: GPIO_MEC_AP(HIGH); GPIO_MEC_AM(LOW); GPIO_MEC_BP(LOW); GPIO_MEC_BM(HIGH); break;
		case 2: GPIO_MEC_AP(LOW); GPIO_MEC_AM(LOW); GPIO_MEC_BP(LOW); GPIO_MEC_BM(HIGH); break;
		case 3: GPIO_MEC_AP(LOW); GPIO_MEC_AM(HIGH); GPIO_MEC_BP(LOW); GPIO_MEC_BM(HIGH); break;
		case 4: GPIO_MEC_AP(LOW); GPIO_MEC_AM(HIGH); GPIO_MEC_BP(LOW); GPIO_MEC_BM(LOW); break;
		case 5: GPIO_MEC_AP(LOW); GPIO_MEC_AM(HIGH); GPIO_MEC_BP(HIGH); GPIO_MEC_BM(LOW); break;
		case 6: GPIO_MEC_AP(LOW); GPIO_MEC_AM(LOW); GPIO_MEC_BP(HIGH); GPIO_MEC_BM(LOW); break;
		case 7: GPIO_MEC_AP(HIGH); GPIO_MEC_AM(LOW); GPIO_MEC_BP(HIGH); GPIO_MEC_BM(LOW); break;
	}
}
#endif

//--------------------------------------------------------------------------------------------------
static void ISPM LENS_XferDriveIRIS(void)
{
#if defined(__USE_GPIO_MTC_AE__)
	if(stAEXFER.irisCur > stAEXFER.iris)		stAEXFER.irisCur--;
	else if(stAEXFER.irisCur < stAEXFER.iris)	stAEXFER.irisCur++;

	GpioMoterCtrlIRIS(stAEXFER.irisCur&0x7);
#elif defined(__USE_DCIRIS_HALL__)
	LENS_WordWrite(SPI_AN41908_ADDR, 0x00, (WORD)stAEXFER.iris);
#endif
}


//--------------------------------------------------------------------------------------------------
void ISPM LENS_CtrlSPILineBUSY(void)
{
//	OLPF_CtrlTimeCounter();	LENS_CtrlTimeCounter();
	LENS_CtrlTimeCounter();
	if (fIsrDRVLensSTOP || fSysStateSTABLE==FALSE) return;
	fSysSPILineBUSY = ON;
}

//--------------------------------------------------------------------------------------------------
void ISPM LENS_CtrlSPILineFREE(void)
{
	if (fSysSPILineBUSY) LENS_XferDriveMOTOR();

#if defined(__DRIVE_VD_SHORT__) || defined(__USE_P_IRIS_ONLY__)
	if (fSysSPILineBUSY) LENS_XferDriveIRIS();
#endif

	fSysSPILineBUSY = OFF;
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
static BYTE ISPM AN41908_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	LENS_CS(HIGH);
	err = SPI_Write(SPI_AN41908_ADDR, rAddr, pBuff, bytes);
	LENS_CS(LOW);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE AN41908_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	WORD i;	BYTE err = SPI_OK;

	for (i=0; i<bytes/2; i++) {		// not consecutive read
		LENS_CS(HIGH);
		err |= SPI_Read(SPI_AN41908_ADDR, rAddr+i, pBuff+2*i, 2);
		LENS_CS(LOW);
	}

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ISPM LENS_MultiWrite(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return AN41908_SPIWrite(rAddr, pBuff, bytes);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ISPM LENS_WordWrite(BYTE sAddr, WORD rAddr, WORD wData)
{
	return LENS_MultiWrite(sAddr, rAddr, (PBYTE)&wData, 2);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE LENS_MultiRead(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return AN41908_SPIRead(rAddr, pBuff, bytes);
}

//--------------------------------------------------------------------------------------------------------------------------
WORD LENS_WordRead(BYTE sAddr, WORD rAddr)
{
	WORD rData = 0;

	LENS_MultiRead(sAddr, rAddr, (PBYTE)&rData, 2);
	return rData;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE LENS_WordField(BYTE sAddr, WORD rAddr, WORD bPos, WORD bCnt, WORD bData)
{
	WORD temp;
	temp = LENS_WordRead(sAddr, rAddr);
	bCnt = ~(0xffff<<bCnt);
	temp &= ~(bCnt<<bPos);
	temp |= ((bData&bCnt)<<bPos);
	return LENS_WordWrite(sAddr, rAddr, temp);
}

/*  FILE_END_HERE */
