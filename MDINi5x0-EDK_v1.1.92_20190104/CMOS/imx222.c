// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_IMXx22__)  || defined (__USE_CMOS_IMX323__)
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

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void IMX222_ChangeFps_Normal(void) {
	WORD hmax, vmax;
 	//30FPS TTL(See datasheet 50page, 25Fps see 51page)
 #if defined(__USE_IMXx22_720P30_MODE__)
 	hmax=0x0672;	//1100
	vmax=0x02EE;  	//1125
 #elif defined(__USE_IMXx22_1080P15_MODE__)
  	hmax=0x0898;	//2200 for FHD15fps
	vmax=0x0465;  	//1125  	
 #else
 	hmax=0x044C;	//1100
	vmax=0x0465;  	//1125
#endif	
	//pal system
#if defined(__VTOTAL_CTRL_PAL__)
	if(GetSS_SensorVoutPAL())  vmax*=1.2f;
#else
	if(GetSS_SensorVoutPAL())  hmax*=1.2f;
#endif

	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0203, LOBYTE(hmax));	// HMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0204, HIBYTE(hmax));	// HMAX [13:8] (MSB)
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0205, LOBYTE(vmax));	// VMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0206, HIBYTE(vmax));	// VMAX [15:0] (MSB)


}


//-----------------------------------------------------------------------

void IMX222_Initialize(void)
{	
#if defined(__USE_CMOS_IMX323__)
#if defined(__USE_IMXx22_720P30_MODE__)
	UARTprintf("\n\rIMX323_TTL_720p30 12bit mode Initialize\n\r");
#else
	UARTprintf("\n\rIMX323_TTL_1080p30 Initialize\n\r");
#endif
#else
#if defined(__USE_IMXx22_720P30_MODE__)
	UARTprintf("\n\rIMXx22_TTL_720p30 12bit mode Initialize\n\r");
#else
	UARTprintf("\n\rIMXx22_TTL_1080p30 Initialize\n\r");
#endif
#endif

//////////////////		Chip ID = 02h			//////////////////////	
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x022C, 0x01);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop	

	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0201, 0x00);		// VREVERSE 0: Normal	1: Inverted
#if defined(__USE_IMXx22_720P30_MODE__)
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0202, 0x01);		// MODE [3:0] 1h:HD720 p, F:1080p
#else
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0202, 0x0F);		// MODE [3:0] 1h:HD720 p, F:1080p
#endif
//	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0207, 0x00);             // Fixed (Default Set:0x10) 	
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0208, 0x00);             // SHS1 [7:0]	
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0209, 0x00);             // SHS1 [15:8] 		
//	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x020A, 0x00);             // Fixed (Default Set:0x0) 		
//	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x020B, 0x00);             // Fixed (Default Set:0x0) 		
//	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x020C, 0x00);             // Fixed (Default Set:0x0) 		
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x020D, 0x00);             // SPL [7:0]	
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x020E, 0x00);             // SPL [9:8] 	
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x020F, 0x00);             // SVS[7:0]	
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0210, 0x00);             // SVS [9:8] 	
#if defined(__USE_IMXx22_720P30_MODE__)	|| defined(__USE_IMXx22_1080P15_MODE__)
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0211, 0x01);             // FRSEL [2:0] OPORTSEL [4:3]	
#else
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0211, 0x00);             // FRSEL [2:0] OPORTSEL [4:3]
#endif
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0212, 0x80|0x02);	// SSBRK [0] ADRES AD gradation setting	0: 10 bits, 1: 12 bits	
//	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0213, 0x40);             // Fixed (Default Set:0x40) 	
#if defined(__USE_IMXx22_720P30_MODE__)
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0214, 0x40);             // WINPH [7:0]
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0215, 0x01);             // WINPH [11:8]		
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0216, 0xF0);             // WINPV [7:0]
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0217, 0x00);             // WINPV [11:8]	
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0218, 0x40);             // WINWH [7:0]
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0219, 0x05);             // WINWH [11:8]	
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x021A, 0xE9);             // WINWV [7:0]
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x021B, 0x02);             // WINWV [10:8]		
#else
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0214, 0x00);             // WINPH [7:0]
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0215, 0x00);             // WINPH [11:8]		
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0216, 0x3C);             // WINPV [7:0]
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0217, 0x00);             // WINPV [11:8]		
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0218, 0xC0);             // WINWH [7:0]
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0219, 0x07);             // WINWH [11:8]	
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x021A, 0x51);             // WINWV [7:0]
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x021B, 0x04);             // WINWV [10:8]		
#endif

	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x021E, 0x00);             //GAIN[7:0]	
//	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x021F, 0x31);             // Fixed (Default Set:0x31) 		
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0220, 0xF0);		// BLKLEVEL[7:0] - Black level (LSB)
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0221, 0x00);		// BLKLEVEL[8] - Black level (MSB)		
#if defined(__USE_IMXx22_720P30_MODE__)	
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0222, 0xC0|0x00);             // XVSLNG [2:0]	if 720p mode use, then 0xC0|0x00
#else
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0222, 0x40|0x00);             // XVSLNG [2:0]	if 720p mode use, then 0xC0|0x00
#endif
	//223~226 do not rewrite		
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0227, 0x20);             // Set (Default Set:0x21) 
	//228~22B do not rewrite			
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x022D, 0x40|0x00);    // BITSEL[3], DCKDLY DCK phase delay For SDR output ... 0: 0¡Æ, 1: 180¡Æ
	//22E~23A do not rewrite
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x023B, 0xE1);             // SYNCCODE [7:0] 0xE1 is CMOS

#if defined(__USE_CMOS_IMX323__)
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x023F, 0x0A);             // Set (Default Set:0x00) 
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x024F, 0x07);             // Set (Default Set:0x07) 
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0254, 0x00);             // Set (Default Set:0x00) 
#endif

	//23C~279 do not rewrite
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x027A, 0x00);             // Setting registers for 10 bit.
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x027B, 0x00);             // Setting registers for 10 bit.
	//27C~297 do not rewrite
#if defined(__USE_IMXx22_720P30_MODE__)		
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0298, 0x26);             // 10B1080 P [11:0] Setting registers for 10 bit.
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0299, 0x02);             // 10B1080 P [11:0] Setting registers for 10 bit.
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x029A, 0x4C);             // 12B1080 P [11:0] Setting registers for 10 bit.
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x029B, 0x04);             // 12B1080 P [11:0] Setting registers for 10 bit.
	//29C~2CD do not rewrite		
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x02CE, 0x40);             // PRES[6:0] Setting registers for 10 bit.
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x02CF, 0x81);             // DRES Setting registers for 10 bit.
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x02D0, 0x01);             // DRES Setting registers for 10 bit.		
#else
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0298, 0x26);             // 10B1080 P [11:0] Setting registers for 10 bit.
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0299, 0x02);             // 10B1080 P [11:0] Setting registers for 10 bit.
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x029A, 0x26);             // 12B1080 P [11:0] Setting registers for 10 bit.
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x029B, 0x02);             // 12B1080 P [11:0] Setting registers for 10 bit.
	//29C~2CD do not rewrite	
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x02CE, 0x16);             // PRES[6:0] Setting registers for 10 bit.
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x02CF, 0x82);             // DRES Setting registers for 10 bit.
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x02D0, 0x00);             // DRES Setting registers for 10 bit.			
#endif
		
	//set frame rate with h/v total size
	IMX222_ChangeFps_Normal();
			
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0317, 0x0D);		 // Set (Default Set:0x4D) 	


//-------------------------------------------------------------------------------------
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0 : Normal operation
	Delay_mSec(100);										// Wait for internal regulator stabilization
	CMOS_ByteWrite(SPI_IMX222_ADDR, 0x022C, 0x00);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
	Delay_mSec(100);										// 


}



#endif	/* __USE_CMOS_IMXx22__ and  || __USE_CMOS_IMX323__*/

/*  FILE_END_HERE */
