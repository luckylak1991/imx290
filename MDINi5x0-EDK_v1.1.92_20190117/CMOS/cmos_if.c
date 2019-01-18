//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2005  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	CMOS_IF.C
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
typedef	struct
{
	SHORT	gainMax_dB;
	SHORT	gainMax_num;
	SHORT	gainOpt_num;
}	CMOS_PARAM, *pCMOS_PARAM;

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static volatile CMOS_PARAM stCMOS;

static WORD MT9M131_Page = 0xff;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
void CMOS_Initialize(void)
{
#if	defined(__USE_CMOS_MT9M131__)
	MT9M131_Initialize();
#elif defined(__USE_CMOS_MN34041__)
	MN34041_Initialize();
#elif defined(__USE_CMOS_MN34220__)
	MN34220_Initialize();
#elif defined(__USE_CMOS_MN34420__)
	MN34420_Initialize();
#elif defined(__USE_CMOS_IMX076__)
	IMX076_Initialize();
#elif defined(__USE_CMOS_IMX124__)
	IMX124_Initialize();
#elif defined(__USE_CMOS_IMX252__)
	IMX252_Initialize();
#elif defined(__USE_CMOS_IMX265__)
	IMX265_Initialize();
#elif defined(__USE_CMOS_IMX174__)  || defined(__USE_CMOS_IMX249__)
	IMX174_Initialize();
#elif defined(__USE_CMOS_IMX185__)
	IMX185_Initialize();
#elif defined(__USE_CMOS_IMXx36__)
	IMX136_Initialize();
#elif defined(__USE_CMOS_IMXx22__) || defined(__USE_CMOS_IMX323__)
	IMX222_Initialize();
#elif defined(__USE_CMOS_IMX238__)
	IMX238_Initialize();
#elif defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__)
	IMX290_Initialize();
#elif defined(__USE_CMOS_IMX385__)
	IMX385_Initialize();
#elif defined(__USE_CMOS_IMX334__)
	IMX334_Initialize();
#elif defined(__USE_CMOS_AR0331__)
	AR0331_Initialize();
#elif defined(__USE_CMOS_AR0134__) || defined(__USE_CMOS_AR0135__) 
	AR0134_Initialize();
#elif defined(__USE_CMOS_PO3100K__)
	PO3100K_Initialize();
#elif defined(__USE_CMOS_PS1210K__)
	PS1210K_Initialize();
#elif defined(__USE_CMOS_PS1400K__)
	PS1400K_Initialize();
#elif defined(__USE_CMOS_EXT3__)
	EXT3_Initialize();
#elif defined(__USE_CMOS_EXT2__)
	EXT2_Initialize();
#elif defined(__USE_CMOS_EXT1__)
	EXT1_Initialize();
#elif defined(__USE_CMOS_OV2710__)
	OV2710_Initialize();
	OV2710_Read();
#elif defined(__USE_CMOS_MT9P031__)
	MT9P031_Initialize();
#elif defined(__USE_CMOS_IMX178__)
	IMX178_Initialize();
#elif defined(__USE_CMOS_IMX226__)
	IMX226_Initialize();
#elif defined(__USE_CMOS_IMX253__)
	IMX253_Initialize();
#elif defined(__USE_CMOS_IMX273__)
	IMX273_Initialize();
#elif defined(__USE_CMOS_IMX274__)
	IMX274_Initialize();
#elif defined(__USE_CMOS_IMX326__)
	IMX326_Initialize();
#endif
	
	
	SetSYSINFO_SensorInitDone();
}

//--------------------------------------------------------------------------------------------------------------------------
static DWORD CMOS_CnvGain(WORD val)
{
#if	defined(__USE_CMOS_MT9M131__)
	if (val<128)		return MAKEDWORD(0, MAKEWORD(0, 0x00|(val-64*0)));
	else if (val<192)	return MAKEDWORD(0, MAKEWORD(0, 0x80|(val-64*1)));
	else if (val<256)	return MAKEDWORD(0, MAKEWORD(1, 0x80|(val-64*2)));
	else				return MAKEDWORD(0, MAKEWORD(1, 0xff));
#elif defined(__USE_CMOS_MN34041__)
	if (val<64)		return MAKEDWORD(MAKEWORD(0x00, 0x80), 0x080|(val-64*0));
	else if (val<128)	return MAKEDWORD(MAKEWORD(0x80, 0x80), 0x080|(val-64*1));
	else if (val<192)	return MAKEDWORD(MAKEWORD(0xc0, 0x80), 0x080|(val-64*2));
	else if (val<320)	return MAKEDWORD(MAKEWORD(0xc0, 0xc0), 0x080|(val-64*3));
	else				return MAKEDWORD(MAKEWORD(0xc0, 0xc0), 0x100);
#elif defined(__USE_CMOS_MN34220__)
	if (val<321)		return MAKEDWORD(0, 0x0100+ val);			//use analog gain only
	else if(val<641)	return MAKEDWORD(val-321+0x100, 0x0240);			//use analog gain + digital gain
	else				return MAKEDWORD(0x0240, 0x0240);
#elif defined(__USE_CMOS_MN34420__)
	if (val<321)		return MAKEDWORD(0, 0x0120+ val);			//use analog gain only
	else if(val<641)	return MAKEDWORD(val-321+0x120, 0x0260);			//use analog gain + digital gain
	else				return MAKEDWORD(0x0260, 0x0260);
#elif defined(__USE_CMOS_AR0331__)
	if (val<6)		return MAKEDWORD(MAKEWORD(0, 6), 0x0080+ val);
	else	 if(val<12)	return MAKEDWORD(MAKEWORD(0, 7), 0x0080+ val-6);
	else if(val<17)	return MAKEDWORD(MAKEWORD(0, 8), 0x0080+ val-12);
	else	 if(val<23)	return MAKEDWORD(MAKEWORD(0, 9), 0x0080+ val-17);
	else if(val<30)	return MAKEDWORD(MAKEWORD(0, 10), 0x0080+ val-23);
	else	 if(val<37)	return MAKEDWORD(MAKEWORD(0, 11), 0x0080+ val-30);
	else if(val<45)	return MAKEDWORD(MAKEWORD(0, 12), 0x0080+ val-37);
	else	 if(val<52)	return MAKEDWORD(MAKEWORD(0, 13), 0x0080+ val-45);
	else if(val<60)	return MAKEDWORD(MAKEWORD(0, 14), 0x0080+ val-52);
	else if(val<69)	return MAKEDWORD(MAKEWORD(0, 15), 0x0080+ val-60);
	else if(val<78)	return MAKEDWORD(MAKEWORD(0, 16), 0x0080+ val-69);	
	else if(val<87)	return MAKEDWORD(MAKEWORD(0, 18), 0x0080+ val-78);
	else if(val<98)	return MAKEDWORD(MAKEWORD(0, 20), 0x0080+ val-87);
	else if(val<109)	return MAKEDWORD(MAKEWORD(0, 22), 0x0080+ val-98);
	else if(val<121)	return MAKEDWORD(MAKEWORD(0, 24), 0x0080+ val-109);
	else if(val<134)	return MAKEDWORD(MAKEWORD(0, 26), 0x0080+ val-121);
	else if(val<149)	return MAKEDWORD(MAKEWORD(0, 28), 0x0080+ val-134);
	else if(val<165)	return MAKEDWORD(MAKEWORD(0, 30), 0x0080+ val-149);
	else if(val<183)	return MAKEDWORD(MAKEWORD(0, 32), 0x0080+ val-165);
	else if(val<205)	return MAKEDWORD(MAKEWORD(0, 36), 0x0080+ val-183);
	else if(val<231)	return MAKEDWORD(MAKEWORD(0, 40), 0x0080+ val-205);
	else	 if(val<263)	return MAKEDWORD(MAKEWORD(0, 44), 0x0080+ val-231);	
	else 			return MAKEDWORD(MAKEWORD(0, 48), 0x0080+ val-263);
#elif defined(__USE_CMOS_MT9P031__)
//	9p031 datasheet ver
//	if (val<25)		return MAKEDWORD(0, 8+val);
//	else	 if(val<41)	return MAKEDWORD(0, 0x0040+17+val-25);
//	else if(val<161)	return MAKEDWORD(0, MAKEWORD(1+val-41,0x0060));

//	9p031 datasheet ver - init bug correct(do not use global gain 8)
	if (val<24)		return MAKEDWORD(0, 9+val); 
	else	 if(val<40)	return MAKEDWORD(0, 0x0040+17+val-24);
	else if(val<160)	return MAKEDWORD(0, MAKEWORD(1+val-40,0x0060));
#elif defined(__USE_CMOS_IMX124__)		// use sony cmos
	return val;	
#elif defined(__USE_CMOS_IMXx36__)		// use sony cmos
	return val;
#else
	return val;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE CMOS_SetGain(WORD val)
{
	DWORD wVal;
	wVal = CMOS_CnvGain(val);

#if	defined(__USE_CMOS_MT9M131__)
	return CMOS_WordWrite(I2C_MT9M131_ADDR, 0x02f, LOWORD(wVal));

#elif defined(__USE_CMOS_MN34041__)
	CMOS_WordWrite(SPI_MN34041_ADDR, 0x020, HIWORD(wVal));
	CMOS_WordWrite(SPI_MN34041_ADDR, 0x021, LOWORD(wVal));
	return 0;
#elif defined(__USE_CMOS_MN34220__)
	CMOS_WordWrite(SPI_MN34220_ADDR, 0x0204, MAKEWORD(LOBYTE(LOWORD(wVal)),HIBYTE(LOWORD(wVal))));
//	CMOS_WordWrite(SPI_MN34220_ADDR, 0x3108, MAKEWORD(LOBYTE(HIWORD(wVal)),HIBYTE(HIWORD(wVal))));
	return 0;	
#elif defined(__USE_CMOS_IMX076__)		// use sony cmos, MAX = 0x8C 
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x00, 0x30);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x1E, wVal);
	return 0;
#elif defined(__USE_CMOS_IMX124__)		// use sony cmos MAX=0x10E
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0200, 0x00);
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0215, HIBYTE(wVal));	// MSB 1bit
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0214, LOBYTE(wVal));	// LSB 8bit
	return 0;
#elif defined(__USE_CMOS_IMX174__)  || defined(__USE_CMOS_IMX249__)		// use sony cmos MAX=0x10E
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0405, HIBYTE(wVal));	// MSB 1bit
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0404, LOBYTE(wVal));	// LSB 8bit
	return 0;
#elif defined(__USE_CMOS_IMXx36__)		// use sony cmos 0x1A4
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x00);
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0215, HIBYTE(wVal));	// MSB 1bit
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0214, LOBYTE(wVal));	// LSB 8bit
	return 0;
#elif defined(__USE_CMOS_AR0331__)		// use sony cmos 0x1A4

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3060, HIWORD(wVal) );	//Analog_gain([5,4] couarse gain, [3:0] fine gain, start 0x06 -> end 0x30(42step) or 0x3F(57step)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3056, LOWORD(wVal) );	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3058, LOWORD(wVal) );	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x305a, LOWORD(wVal) );	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x305c, LOWORD(wVal) );	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x305E, LOWORD(wVal) );	//global_gain(128~2047) 0~1919step
	
	return 0;
#elif defined(__USE_CMOS_MT9P031__)
	// global gain(0x035) is 16bit word 
	//[14:8] digital gain [6] Analog Multiplier, [5:0] Analog Gain 
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x035, LOWORD(wVal));	
	return 0;
#elif defined (__USE_CMOS_PO3100K__)
	CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03, 0x01);					//page b
	CMOS_ByteWrite(I2C_PO3100K_ADDR,0xC3, LOBYTE(MIN(wVal,95)));	//gloval gain
	return 0;
#else
	return 0;
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void CMOS_SetShutter(WORD val)
{

#if defined(__USE_CMOS_MN34041__)
	CMOS_WordWrite(SPI_MN34041_ADDR, 0x00a1, val);		//Set CIS Shutter step.
	#if __MISP100_DBGPRT__ == 1
//		UARTprintf("DEMO_SetExpShutter : addr(0x00a1), data(%x) \n", val);
	#endif
#elif defined(__USE_CMOS_IMX076__)
		CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x00, 0x30);
	 	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x09, LOBYTE(val));
		CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x0A, HIBYTE(val));
	#if __MISP100_DBGPRT__ == 1
//		UARTprintf("DEMO_SetExpShutter : addr(0x0A, 0x09), data(%x) \n", val);
	#endif		
#elif defined (__USE_CMOS_IMX124__)	//when frame mode wdr, this is determine "short shutter 1
		CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0200, 0x00);
		CMOS_ByteWrite(SPI_IMX124_ADDR, 0x021E, LOBYTE(val));		// SHS1 [7:0]	(LSB)	// 2 to (Number of lines per frame - 2)
		CMOS_ByteWrite(SPI_IMX124_ADDR, 0x021F, HIBYTE(val));		// SHS1 [7:0]
		CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0220, HIWORD(val));		// SHS1 [0]	(MSB)
	#if __MISP100_DBGPRT__ == 1
		UARTprintf("DEMO_SetExpShutter : addr(0x1E, 0x1F), data(%x) \r\n", val);	
	#endif
#elif defined (__USE_CMOS_IMXx36__)	//when frame mode wdr, this is determine "short shutter 1
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x00);
	 	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0220, LOBYTE(val));
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0221, HIBYTE(val));
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0222, HIWORD(val));
	#if __MISP100_DBGPRT__ == 1
//		UARTprintf("DEMO_SetExpShutter : addr(0x20, 0x21), data(%x) \n", val);
	#endif		
#elif defined (__USE_CMOS_IMX174__)	|| defined(__USE_CMOS_IMX249__)
		CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0200, 0x00);
		CMOS_ByteWrite(SPI_IMX174_ADDR, 0x029A, LOBYTE(val));
		CMOS_ByteWrite(SPI_IMX174_ADDR, 0x029B, HIBYTE(val));
#elif defined (__USE_CMOS_AR0331__)		
//need more code to limit HDR mode(limit 0x460)
		if(val>0x465) val=0x465;	
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3012, 0x465-val);	
#elif defined(__USE_CMOS_MT9P031__)
//min 1, max 1942, Shutter Width Lower 0x09 register
	if(val>1941) val=1941;

	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x09, 1942-val);
#elif defined (__USE_CMOS_MN34220__)

	if(val<1123) val=1123-val;		
	else val =0;
	
	CMOS_WordWrite(SPI_MN34220_ADDR, 0x202, MAKEWORD(LOBYTE(val), HIBYTE(val)));
#elif defined (__USE_CMOS_MN34420__)

	if(val<1123) val=1123-val;		
	else val =0;
	
//	CMOS_WordWrite(SPI_MN34420_ADDR, 0x202, MAKEWORD(LOBYTE(val), HIBYTE(val)));
#elif defined (__USE_CMOS_PO3100K__)

	if(val>740) val=740;		//736 is near max(max is 740)
	
	CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x01);	//page b
	CMOS_ByteWrite(I2C_PO3100K_ADDR,0xC0,HIBYTE(740-val));	//Integration time (line) H
	CMOS_ByteWrite(I2C_PO3100K_ADDR,0xC1,LOBYTE(740-val));	//Integration time (line) L
//	CMOS_ByteWrite(I2C_PO3100K_ADDR,0xC2,0x00);	//Integration time (column)
		

#endif
}

void CMOS_SHS2(WORD val)		//2F WDR Long shutter set, 4F WDR S2 shutter case - sensor direct case
{
#if defined(__USE_CMOS_IMX076__)
		CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x00, 0x30);
	 	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x0B, LOBYTE(val));
		CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x0C, HIBYTE(val));
	#if __MISP100_DBGPRT__ == 1
//		UARTprintf("DEMO_SetExpShutter : FPS(%d), addr(0x0A, 0x09), data(%x) \n", CIS_FPS, val);
	#endif		
#elif defined (__USE_CMOS_IMXx36__)	//when frame mode wdr, this is determine "short shutter 1
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x00);
	 	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0223, LOBYTE(val));
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0224, HIBYTE(val));
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0225, HIWORD(val));
	#if __MISP100_DBGPRT__ == 1
//		UARTprintf("DEMO_SetExpShutter : FPS(%d), addr(0x20, 0x21), data(%x) \n", CIS_FPS, val);
	#endif				
#endif


}

void CMOS_SHS3(WORD val)		//4F WDR S1 shutter case - sensor direct case
{
#if defined (__USE_CMOS_IMXx36__)		//when frame mode wdr, this is determine "short shutter 1
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x00);
	 	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0226, LOBYTE(val));
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0227, HIBYTE(val));
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0228, HIWORD(val));
	#if __MISP100_DBGPRT__ == 1
//		UARTprintf("DEMO_SetExpShutter : FPS(%d), addr(0x20, 0x21), data(%x) \n", CIS_FPS, val);
	#endif				
#endif

}

void CMOS_SHS4(WORD val)		//4F WDR Long shutter set - sensor direct case
{
#if defined (__USE_CMOS_IMXx36__)		//when frame mode wdr, this is determine "short shutter 1
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x00);
	 	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0229, LOBYTE(val));
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x022A, HIBYTE(val));
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x022B, HIWORD(val));
	#if __MISP100_DBGPRT__ == 1
//		UARTprintf("DEMO_SetExpShutter : FPS(%d), addr(0x20, 0x21), data(%x) \n", CIS_FPS, val);
	#endif				
#endif

}

//--------------------------------------------------------------------------------------------------------------------------
WORD CMOS_GetGain(void)
{
#if	defined(__USE_CMOS_MT9M131__)
	return CMOS_WordRead(I2C_MT9M131_ADDR, 0x02f);

#elif defined(__USE_CMOS_MN34041__)
	return 0;

#elif defined (__USE_CMOS_IMX076__) || defined (__USE_CMOS_IMXx36__)
	return 0;

#else
	return 0;
#endif

}

void CMOS_SetSensup(LONG val)
{
#if	defined(__USE_CMOS_MT9M131__)
	//return CMOS_WordRead(I2C_MT9M131_ADDR, 0x02f);
	CMOS_WordRead(I2C_MT9M131_ADDR, 0x02f);
#elif defined(__USE_CMOS_MN34041__)
	CMOS_WordWrite(SPI_MN34041_ADDR, 0x00a5, val);

#elif defined (__USE_CMOS_MN34220__)
	CMOS_WordWrite(SPI_MN34220_ADDR, 0x3122, val);

#elif defined (__USE_CMOS_MN34420__)
	CMOS_WordWrite(SPI_MN34420_ADDR, 0x3122, val);
	
#elif defined (__USE_CMOS_IMXx36__)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0218, LOBYTE(val));
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0219, HIBYTE(val));	
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x021A, 0);
#elif defined (__USE_CMOS_IMX124__)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0218, LOBYTE(val));
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0219, HIBYTE(val));	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x021A, 0);
#elif defined (__USE_CMOS_IMX185__)
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0218, LOBYTE(val));
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0219, HIBYTE(val));	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x021A, 0);	
#elif defined (__USE_CMOS_IMXx22__) || defined (__USE_CMOS_IMX323__)
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0205, LOBYTE(val));
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0206, HIBYTE(val));	
#elif defined (__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0218, LOBYTE(val));
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0219, HIBYTE(val));	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x021A, HIWORD(val));
#elif defined(__USE_CMOS_IMX385__)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0218, LOBYTE(val));
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0219, HIBYTE(val));	
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x021A, HIWORD(val));
#elif defined (__USE_CMOS_IMX226__)
	MISP_RegWrite(MISP_LOCAL_ID,0x56B,val);
	//UARTprintf("imx226-sensup:%d\r\n",val);
#elif defined (__USE_CMOS_IMX265__)
    #if defined(__USE_IMX265_SLAVE_MODE__) //defined(__USE_TRIGGER_MODE__)
	MISP_RegWrite(MISP_LOCAL_ID,0x56B,val);
    #else
	CMOS_ByteWrite(SPI_IMX265_ADDR, 0x0210, LOBYTE(val));
	CMOS_ByteWrite(SPI_IMX265_ADDR, 0x0211, HIBYTE(val));
	CMOS_ByteWrite(SPI_IMX265_ADDR, 0x0212, HIWORD(val));
    #endif
	//UARTprintf("imx265-sensup:%d\r\n",val);
#elif defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX273__)
	MISP_RegWrite(MISP_LOCAL_ID,0x56B,val);
#elif defined (__USE_CMOS_IMX253__)
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0210, LOBYTE(val));
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0211, HIBYTE(val));	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0212, HIWORD(val));	
#endif

}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE PO3100K_I2CWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Write(TWI_CH0, I2C_PO3100K_ADDR, LOBYTE(rAddr), I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes); 	// addr(8bit), data(8bit)
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE PO3100K_I2CRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Read(TWI_CH0, I2C_PO3100K_ADDR, LOBYTE(rAddr), I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes); 	// addr(8bit), data(8bit)
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE PS1210K_I2CWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Write(TWI_CH0, I2C_PS1210K_ADDR, LOBYTE(rAddr), I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes); 	// addr(8bit), data(8bit)
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE PS1210K_I2CRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Read(TWI_CH0, I2C_PS1210K_ADDR, LOBYTE(rAddr), I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes); 	// addr(8bit), data(8bit)
}
#if defined(__USE_CMOS_PS1400K__)
//--------------------------------------------------------------------------------------------------------------------------
static BYTE PS1400K_I2CWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
#if defined(__USE_PS1400K_I2C_PORT0__)
	return I2C_Write(TWI_CH0, I2C_PS1400K_ADDR, rAddr, I2C_ADDR_16BIT, pBuff, I2C_DATA_16BIT, bytes); 	// addr(16bit), data(16bit)
#else
	return I2C_Write(TWI_CH1, I2C_PS1400K_ADDR, rAddr, I2C_ADDR_16BIT, pBuff, I2C_DATA_16BIT, bytes); 	// addr(16bit), data(16bit)
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE PS1400K_I2CRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
#if defined(__USE_PS1400K_I2C_PORT0__)
	return I2C_Read(TWI_CH0, I2C_PS1400K_ADDR, rAddr, I2C_ADDR_16BIT, pBuff, I2C_DATA_16BIT, bytes); 	// addr(16bit), data(16bit)
#else
	return I2C_Read(TWI_CH1, I2C_PS1400K_ADDR, rAddr, I2C_ADDR_16BIT, pBuff, I2C_DATA_16BIT, bytes); 	// addr(16bit), data(16bit)
#endif
}
#endif
#if defined(__USE_CMOS_EXT2__)	||	defined(__USE_CMOS_EXT1__)	
//--------------------------------------------------------------------------------------------------------------------------
static BYTE EXT2_I2CWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Write(TWI_CH0, I2C_EXT2_ADDR, rAddr, I2C_ADDR_16BIT, pBuff, I2C_DATA_16BIT, bytes); 	// addr(16bit), data(16bit)
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE EXT2_I2CRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Read(TWI_CH0, I2C_EXT2_ADDR, rAddr, I2C_ADDR_16BIT, pBuff, I2C_DATA_16BIT, bytes); 	// addr(16bit), data(16bit)
}
#endif
#if defined(__USE_CMOS_OV2710__)
//--------------------------------------------------------------------------------------------------------------------------
static BYTE OV2710_I2CWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Write(TWI_CH0, I2C_OV2710_ADDR, rAddr, I2C_ADDR_16BIT, pBuff, I2C_DATA_8BIT, bytes); 	// addr(16bit), data(8bit)
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE OV2710_I2CRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Read(TWI_CH0, I2C_OV2710_ADDR, rAddr, I2C_ADDR_16BIT, pBuff, I2C_DATA_8BIT, bytes); 	// addr(16bit), data(8bit)
}
//--------------------------------------------------------------------------------------------------------------------------
#endif
static BYTE MT9M131_SetPage(WORD page)
{
	if (page==MT9M131_Page) 	return I2C_OK;
	else 						MT9M131_Page = page;
	
	return I2C_Write(TWI_CH0, I2C_MT9M131_ADDR, 0xf0, I2C_ADDR_8BIT, (PBYTE)&page, I2C_DATA_16BIT, 2);	// write page
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE MT9M131_I2CWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = I2C_OK;

	err = MT9M131_SetPage(HIBYTE(rAddr));	// write page
	if (err) return err;

	err = I2C_Write(TWI_CH0, I2C_MT9M131_ADDR, LOBYTE(rAddr), I2C_ADDR_8BIT, pBuff, I2C_DATA_16BIT, bytes); 	// addr(8bit), data(16bit)
	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE MT9M131_I2CRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = I2C_OK;

	err = MT9M131_SetPage(HIBYTE(rAddr));	// write page
	if (err) return err;

	err = I2C_Read(TWI_CH0, I2C_MT9M131_ADDR, LOBYTE(rAddr), I2C_ADDR_8BIT, pBuff, I2C_DATA_16BIT, bytes); 	// addr(8bit), data(16bit)
	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE MT9P031_I2CWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Write(TWI_CH0, I2C_MT9P031_ADDR, LOBYTE(rAddr), I2C_ADDR_8BIT, pBuff, I2C_DATA_16BIT, bytes); 	// addr(8bit), data(16bit)
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE MT9P031_I2CRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Read(TWI_CH0, I2C_MT9P031_ADDR, LOBYTE(rAddr), I2C_ADDR_8BIT, pBuff, I2C_DATA_16BIT, bytes); 	// addr(8bit), data(16bit)
}
//--------------------------------------------------------------------------------------------------------------------------
static BYTE AR0134_I2CWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Write(TWI_CH0, I2C_AR0134_ADDR, rAddr, I2C_ADDR_16BIT, pBuff, I2C_DATA_16BIT, bytes); 	// addr(16bit), data(16bit)
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE AR0134_I2CRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Read(TWI_CH0, I2C_AR0134_ADDR, rAddr, I2C_ADDR_16BIT, pBuff, I2C_DATA_16BIT, bytes); 	// addr(16bit), data(16bit)
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE AR0331_I2CWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Write(TWI_CH0, I2C_AR0331_ADDR, rAddr, I2C_ADDR_16BIT, pBuff, I2C_DATA_16BIT, bytes); 	// addr(16bit), data(16bit)
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE AR0331_I2CRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Read(TWI_CH0, I2C_AR0331_ADDR, rAddr, I2C_ADDR_16BIT, pBuff, I2C_DATA_16BIT, bytes); 	// addr(16bit), data(16bit)
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE MN34041_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Write(SPI_MN34041_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);
	
	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE MN34041_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	WORD i; BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	for (i=0; i<bytes/2; i++) {	// not consecutive read
		CMOS_CS(LOW);
		err = SPI_Read(SPI_MN34041_ADDR, rAddr+i, pBuff+2*i, 2);
		CMOS_CS(HIGH);
	}

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE MN34220_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_MN34220_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE MN34220_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;
//	WORD i; 

	while (fSysSPILineBUSY);	// check SPI-Line status

//	for (i=0; i<bytes/2; i++) {	// not consecutive read
		CMOS_CS(LOW);
		err = SPI_Read(SPI_MN34220_ADDR, rAddr, pBuff, 2);
//		err = SPI_Read(SPI_MN34220_ADDR, rAddr+i, pBuff+2*i, 2);
		CMOS_CS(HIGH);
//	}

	return err;
}
//--------------------------------------------------------------------------------------------------------------------------
static BYTE MN34420_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_MN34420_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE MN34420_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;
//	WORD i; 

	while (fSysSPILineBUSY);	// check SPI-Line status

//	for (i=0; i<bytes/2; i++) {	// not consecutive read
		CMOS_CS(LOW);
		err = SPI_Read(SPI_MN34420_ADDR, rAddr, pBuff, 2);
//		err = SPI_Read(SPI_MN34220_ADDR, rAddr+i, pBuff+2*i, 2);
		CMOS_CS(HIGH);
//	}

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX124_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_IMX124_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX124_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Read(SPI_IMX124_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX174_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_IMX174_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX174_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Read(SPI_IMX174_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX185_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_IMX185_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX185_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Read(SPI_IMX185_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX290_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_IMX290_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX290_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Read(SPI_IMX290_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}
//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX385_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_IMX385_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX385_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Read(SPI_IMX385_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX222_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_IMX222_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}
//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX252_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Read(SPI_IMX252_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX252_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_IMX252_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}
//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX265_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Read(SPI_IMX265_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX265_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_IMX265_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}
//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX253_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Read(SPI_IMX253_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX253_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_IMX253_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}
//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX273_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Read(SPI_IMX273_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX273_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_IMX273_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}
//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX222_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Read(SPI_IMX222_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}
//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX238_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_IMX238_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX238_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Read(SPI_IMX238_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}
//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX136_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_IMX136_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX136_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Read(SPI_IMX136_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE SONY_I2CWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Write(TWI_CH0, I2C_SONY_CMOS_ADDR, rAddr, I2C_ADDR_16BIT, pBuff, I2C_DATA_8BIT, bytes); 	// addr(8bit), data(8bit)
}
//--------------------------------------------------------------------------------------------------------------------------
static BYTE SONY_I2CRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Read(TWI_CH0, I2C_SONY_CMOS_ADDR, rAddr, I2C_ADDR_16BIT, pBuff, I2C_DATA_8BIT, bytes); 	// addr(8bit), data(8bit)
}
//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX076_136_SPIWrite(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Write(sAddr, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);
	
	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX076_136_SPIRead(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	WORD i; BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	for (i=0; i<bytes; i++) {	// not consecutive read
		CMOS_CS(LOW);
		err = SPI_Read(sAddr, rAddr+i, pBuff+2*i, 2);		
		CMOS_CS(HIGH);
	}
	
	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX178_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_IMX178_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX178_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Read(SPI_IMX178_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ISPM IMX226_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_IMX226_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX226_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Read(SPI_IMX226_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}
//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX274_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_IMX274_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE IMX274_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Read(SPI_IMX274_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}
//--------------------------------------------------------------------------------------------------------------------------
static BYTE EXT3_SPIWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status

	CMOS_CS(LOW);
	err = SPI_Write(SPI_EXT3_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE EXT3_SPIRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE err = SPI_OK;

	while (fSysSPILineBUSY);	// check SPI-Line status
	
	CMOS_CS(LOW);
	err = SPI_Read(SPI_EXT3_ADDR, rAddr, pBuff, bytes);
	CMOS_CS(HIGH);

	return err;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE CMOS_MultiWrite(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	switch (sAddr&0xfe) {
		case I2C_MT9M131_ADDR:	return MT9M131_I2CWrite(rAddr, pBuff, bytes);
		case I2C_AR0331_ADDR:	return AR0331_I2CWrite(rAddr, pBuff, bytes);
		case I2C_AR0134_ADDR:	return AR0134_I2CWrite(rAddr, pBuff, bytes);		
		case I2C_MT9P031_ADDR:	return  MT9P031_I2CWrite(rAddr, pBuff, bytes);				
		case SPI_MN34220_ADDR:	return MN34220_SPIWrite(rAddr, pBuff, bytes);
		case SPI_MN34420_ADDR:	return MN34420_SPIWrite(rAddr, pBuff, bytes);		
		case SPI_MN34041_ADDR:	return MN34041_SPIWrite(rAddr, pBuff, bytes);
		case SPI_IMX124_ADDR:		return IMX124_SPIWrite(rAddr, pBuff, bytes);	
		case SPI_IMX252_ADDR:		return IMX252_SPIWrite(rAddr, pBuff, bytes);			
		case SPI_IMX265_ADDR:		return IMX265_SPIWrite(rAddr, pBuff, bytes);			
		case SPI_IMX253_ADDR:		return IMX253_SPIWrite(rAddr, pBuff, bytes);	
		case SPI_IMX273_ADDR:		return IMX273_SPIWrite(rAddr, pBuff, bytes);			
		case SPI_IMX174_ADDR:		return IMX174_SPIWrite(rAddr, pBuff, bytes);			
		case SPI_IMX185_ADDR:		return IMX185_SPIWrite(rAddr, pBuff, bytes);	
		case SPI_IMX222_ADDR:		return IMX222_SPIWrite(rAddr, pBuff, bytes);				
		case SPI_IMX238_ADDR:		return IMX238_SPIWrite(rAddr, pBuff, bytes);				
		case SPI_IMX290_ADDR:		return IMX290_SPIWrite(rAddr, pBuff, bytes);				
		case SPI_IMX385_ADDR:		return IMX385_SPIWrite(rAddr, pBuff, bytes);		
		case SPI_EXT3_ADDR:		return EXT3_SPIWrite(rAddr, pBuff, bytes);	
		case SPI_IMX136_ADDR:		return IMX136_SPIWrite(rAddr, pBuff, bytes);			
		case I2C_SONY_CMOS_ADDR:	return SONY_I2CWrite(rAddr, pBuff, bytes);	
		case I2C_PO3100K_ADDR:	return PO3100K_I2CWrite(rAddr, pBuff, bytes);		
		case I2C_PS1210K_ADDR:	return PS1210K_I2CWrite(rAddr, pBuff, bytes);	
#if defined(__USE_CMOS_PS1400K__)		
		case I2C_PS1400K_ADDR:	return PS1400K_I2CWrite(rAddr, pBuff, bytes);			
#endif
#if defined(__USE_CMOS_EXT2__) || defined(__USE_CMOS_EXT1__)
		case I2C_EXT2_ADDR:	return EXT2_I2CWrite(rAddr, pBuff, bytes);	
#endif
#if defined(__USE_CMOS_OV2710__)
		case I2C_OV2710_ADDR:	return OV2710_I2CWrite(rAddr, pBuff, bytes);			
#endif
		case SPI_IMX178_ADDR:		return IMX178_SPIWrite(rAddr, pBuff, bytes);
		case SPI_IMX226_ADDR:		return IMX226_SPIWrite(rAddr, pBuff, bytes);		
		case SPI_IMX274_ADDR:		return IMX274_SPIWrite(rAddr, pBuff, bytes);			
		case SPI_IMX076_ADDR_1:	return IMX076_136_SPIWrite(sAddr, rAddr, pBuff, bytes);
		case SPI_IMX076_ADDR_2:	return IMX076_136_SPIWrite(sAddr, rAddr, pBuff, bytes);
	}
	return I2C_OK;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE CMOS_ByteWrite(BYTE sAddr, WORD rAddr, BYTE wData)
{
	CMOS_MultiWrite(sAddr, rAddr, (PBYTE)&wData, 1);
	return I2C_OK;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE CMOS_WordWrite(BYTE sAddr, WORD rAddr, WORD wData)
{
	CMOS_MultiWrite(sAddr, rAddr, (PBYTE)&wData, 2);
	return I2C_OK;
}

BYTE CMOS_DWordWrite(BYTE sAddr, WORD rAddr, DWORD wData)
{
	CMOS_MultiWrite(sAddr, rAddr, (PBYTE)&wData, 4);
	return I2C_OK;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE CMOS_MultiRead(BYTE sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	switch (sAddr&0xfe) {
		case I2C_MT9M131_ADDR:	return MT9M131_I2CRead(rAddr, pBuff, bytes);
		case I2C_AR0331_ADDR:	return AR0331_I2CRead(rAddr, pBuff, bytes);
		case I2C_AR0134_ADDR:	return AR0134_I2CRead(rAddr, pBuff, bytes);
		case I2C_MT9P031_ADDR:	return MT9P031_I2CRead(rAddr, pBuff, bytes);		
		case SPI_MN34220_ADDR:	return MN34220_SPIRead(rAddr, pBuff, bytes);
		case SPI_MN34420_ADDR:	return MN34420_SPIRead(rAddr, pBuff, bytes);		
		case SPI_MN34041_ADDR:	return MN34041_SPIRead(rAddr, pBuff, bytes);
		case SPI_IMX124_ADDR:		return IMX124_SPIRead(rAddr, pBuff, bytes);	
		case SPI_IMX252_ADDR:		return IMX252_SPIRead(rAddr, pBuff, bytes);				
		case SPI_IMX265_ADDR:		return IMX265_SPIRead(rAddr, pBuff, bytes);			
		case SPI_IMX253_ADDR:		return IMX253_SPIRead(rAddr, pBuff, bytes);		
		case SPI_IMX273_ADDR:		return IMX273_SPIRead(rAddr, pBuff, bytes);				
		case SPI_IMX174_ADDR:		return IMX174_SPIRead(rAddr, pBuff, bytes);			
		case SPI_IMX185_ADDR:		return IMX185_SPIRead(rAddr, pBuff, bytes);	
		case SPI_IMX222_ADDR:		return IMX222_SPIRead(rAddr, pBuff, bytes);			
		case SPI_IMX238_ADDR:		return IMX238_SPIRead(rAddr, pBuff, bytes);				
		case SPI_IMX290_ADDR:		return IMX290_SPIRead(rAddr, pBuff, bytes);				
		case SPI_IMX385_ADDR:		return IMX385_SPIRead(rAddr, pBuff, bytes);					
		case SPI_IMX136_ADDR:		return IMX136_SPIRead(rAddr, pBuff, bytes);	
		case I2C_SONY_CMOS_ADDR:	return SONY_I2CRead(rAddr, pBuff, bytes);			
		case I2C_PO3100K_ADDR:	return PO3100K_I2CRead(rAddr, pBuff, bytes);
		case I2C_PS1210K_ADDR:	return PS1210K_I2CRead(rAddr, pBuff, bytes);
		case SPI_EXT3_ADDR:	return EXT3_SPIRead(rAddr, pBuff, bytes);			
#if defined(__USE_CMOS_PS1400K__)		
		case I2C_PS1400K_ADDR:	return PS1400K_I2CRead(rAddr, pBuff, bytes);		
#endif
#if defined(__USE_CMOS_EXT2__)	||	defined(__USE_CMOS_EXT1__)		
		case I2C_EXT2_ADDR:	return EXT2_I2CRead(rAddr, pBuff, bytes);			
#endif
#if defined(__USE_CMOS_OV2710__)
		case I2C_OV2710_ADDR:	return OV2710_I2CRead(rAddr, pBuff, bytes);		
#endif
		case SPI_IMX178_ADDR:		return IMX178_SPIRead(rAddr, pBuff, bytes);				
		case SPI_IMX226_ADDR:		return IMX226_SPIRead(rAddr, pBuff, bytes);			
		case SPI_IMX274_ADDR:		return IMX274_SPIRead(rAddr, pBuff, bytes);				
		case SPI_IMX076_ADDR_1:	return IMX076_136_SPIRead(sAddr, rAddr, pBuff, bytes);
		case SPI_IMX076_ADDR_2:	return IMX076_136_SPIRead(sAddr, rAddr, pBuff, bytes);
	}
	return I2C_OK;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE CMOS_ByteRead(BYTE sAddr, WORD rAddr)
{
	BYTE rData = 0;
	
	CMOS_MultiRead(sAddr, rAddr, (PBYTE)&rData, 1);
	return rData;
}

//--------------------------------------------------------------------------------------------------------------------------
WORD CMOS_WordRead(BYTE sAddr, WORD rAddr)
{
	WORD rData = 0;
	
	CMOS_MultiRead(sAddr, rAddr, (PBYTE)&rData, 2);
	return rData;
}
//--------------------------------------------------------------------------------------------------------------------------
DWORD CMOS_DWordRead(BYTE sAddr, WORD rAddr)
{
	DWORD rData = 0;
	
	CMOS_MultiRead(sAddr, rAddr, (PBYTE)&rData, 4);
	return rData;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE CMOS_ByteField(BYTE sAddr, WORD rAddr, BYTE bPos, BYTE bCnt, BYTE bData)
{
	BYTE temp;
	temp = CMOS_ByteRead(sAddr, rAddr);
	bCnt = ~(0xff<<bCnt);
	temp &= ~(bCnt<<bPos);
	temp |= ((bData&bCnt)<<bPos);
	return CMOS_ByteWrite(sAddr, rAddr, temp);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE CMOS_WordField(BYTE sAddr, WORD rAddr, WORD bPos, WORD bCnt, WORD bData)
{
	WORD temp;
	temp = CMOS_WordRead(sAddr, rAddr);
	bCnt = ~(0xffff<<bCnt);
	temp &= ~(bCnt<<bPos);
	temp |= ((bData&bCnt)<<bPos);
	return CMOS_WordWrite(sAddr, rAddr, temp);
}

//--------------------------------------------------------------------------------------------------------------------------
void SetCMOSGainMax(short dB, short num) //CMOS gain max dB , dB = dB value, num = register value
{
	stCMOS.gainMax_dB = dB;
	stCMOS.gainMax_num = num;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetCMOSGainOpt_num(short val) // CMOS gain max value of using (register value)
{
	stCMOS.gainOpt_num = val;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT GetCMOSGainMax_dB(void) // get CMOS max gain dB
{
	return stCMOS.gainMax_dB;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT GetCMOSGainMax_num(void) // get CMOS max gain register value
{
	return stCMOS.gainMax_num;
}

//--------------------------------------------------------------------------------------------------------------------------
SHORT GetCMOSGainOpt_num(void) // get CMOS max using gain dB
{
	return stCMOS.gainOpt_num;
}

/*  FILE_END_HERE */
