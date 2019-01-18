// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_IMX076__)
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

void IMX076_Initialize(void)
{	
	UARTprintf("IMX076_Initialize\n\r");
	
//----------------- initial sequence -----------------------
/*	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x00, 0x31);		// STANDBY[0] - 1: STANDBY / TESTEN[5:4] 3: register write enb
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x01);		// FRSEL[2:0] - 0: 60fps, 1: 30fps 
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x02, 0x00);		// MODE [3:0] - 0: All pixel, 1: 720p, 2: Window cropping, 3: 2x2 mode ....
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x01, 0x00);		// VREVERSE[0] - 0: Normal, 1: Inverted
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x12, 0x82);		// ADRES[1] - 0: 10bit, 1: 12bit
	// LVDS DDR 30fps mode
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x19);		// OPORTSEL[4:3] - 0: CMOS SDR, 1: CMOS DDR, 2: LVDS 1ch, 3: LVDS 2ch	
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x18);		// LVDS DDR 60fps mode
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x01);			// CMOS SDR mode
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x07, 0x00);		// SVS
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x08, 0x00);		// SVS[0] - MSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x0D, 0x00);		// SPL
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x0E, 0x00);		// SPL[0] - MSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x09, 0x20);		// SHS1[7:0] - Shutter (LSB)
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x0A, 0x02);		// SHS1[7:0] - Shutter (MSB)
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x1E, 0x00);		// GAIN [7:0]
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x20, 0xF0);		// BLKLEVEL
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x21, 0x00);		// BLKLEVEL[0] - MSB , XHSLNG [5:4]
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x22, 0x40);		// XVSLNG [2:0]
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x05, 0x42);		// VMAX[7:0] - LSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x06, 0x04);		// VMAX[7:0] - MSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x03, 0x72);		// HMAX[7:0] - LSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x04, 0x06);		// HMAX[5:0] - MSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x2C, 0x00);		// XMSTA[0]
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x00, 0x30);		// STANDBY[0] - 0 : Normal operation
*/
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x00, 0x31);		// STANDBY[0] - 1: STANDBY / TESTEN[5:4] 3: register write enb
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x01, 0x00);		// VREVERSE[0] - 0: Normal, 1: Inverted
// jwlee test 20130218 .. test 2x2
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x02, 0x00);		// MODE [3:0] - 0: All pixel, 1: 720p, 2: Window cropping, 3: 2x2 mode ....
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x03, 0xe8);		// HMAX[7:0] - LSB
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x04, 0x0c);		// HMAX[5:0] - MSB
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x05, 0x28);		// VMAX[7:0] - LSB
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x06, 0x02);		// VMAX[7:0] - MSB

#if defined(__USE_CMOS_IMX076_LVDS__)		// use sony cmos

#if defined(__USE_CMOS_IMX076_ALLPIXEL__)		// use sony cmos
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x02, 0x00);		// MODE [3:0] - 0: All pixel, 1: 720p, 2: Window cropping, 3: 2x2 mode ....

	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x03, 0x72);		// HMAX[7:0] - LSB  30Hz
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x04, 0x06);		// HMAX[5:0] - MSB  30Hz
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x03, 0x39);		// HMAX[7:0] - LSB  60Hz
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x04, 0x03);		// HMAX[5:0] - MSB  60H

	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x05, 0x42);		// VMAX[7:0] - LSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x06, 0x04);		// VMAX[7:0] - MSB	

//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x11);		// LVDS 1ch mode (60fps) // TTL  OPORTSEL[4:3] - 2,FRSEL[2:0] - 0 org .. TTL
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x10);		// LVDS 1ch mode (60fps) // TTL  OPORTSEL[4:3] - 2,FRSEL[2:0] - 0 org .. ok LVDS 1ch clock 320MHz
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x19);		// LVDS 1ch mode (60fps) // TTL  OPORTSEL[4:3] - 2,FRSEL[2:0] - 0 org .. ok LVDS 2ch clock 160MHz
	
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x01);		// CMOS SDR mode (30fps 10fps) // TTL  OPORTSEL[4:3] - 0,FRSEL[2:0] - 1  54MHz  30Hz
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x00);		// CMOS SDR mode (60fps 30fps) // TTL  OPORTSEL[4:3] - 0,FRSEL[2:0] - 0  54MHz  60Hz
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x12, 0x82);		// ADRES[1] - 0: 10bit, 1: 12bit  30Hz
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x12, 0x80);		// ADRES[1] - 0: 10bit, 1: 12bit 60Hz
#endif
#if defined(__USE_CMOS_IMX076_2X2PIXEL__)		// use sony cmos
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x02, 0x03);		// MODE [3:0] - 0: All pixel, 1: 720p, 2: Window cropping, 3: 2x2 mode ....
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x03, 0xE8);		// HMAX[7:0] - LSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x04, 0x0C);		// HMAX[5:0] - MSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x05, 0x28);		// VMAX[7:0] - LSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x06, 0x02);		// VMAX[7:0] - MSB#endif
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x13);		// LVDS 1ch mode (60fps) // TTL  OPORTSEL[4:3] - 2,FRSEL[2:0] - 3 org
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x12, 0x80);		// ADRES[1] - 0: 10bit, 1: 12bit 2x2 10bit

#endif
#if defined(__USE_CMOS_IMX076_HOR_1_2__)		// use sony cmos
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x02, 0x05);		// MODE [3:0] - 0: All pixel, 1: 720p, 2: Window cropping, 3: 2x2 mode ....
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x03, 0x72);		// HMAX[7:0] - LSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x04, 0x06);		// HMAX[5:0] - MSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x05, 0x42);		// VMAX[7:0] - LSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x06, 0x04);		// VMAX[7:0] - MSB#endif
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x11);		// LVDS 1ch mode (30fps) // TTL  OPORTSEL[4:3] - 2,FRSEL[2:0] - 1 org
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x12, 0x82);		// ADRES[1] - 0: 10bit, 1: 12bit

#endif
#if defined(__USE_CMOS_IMX076_VER_1_2__)		// use sony cmos
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x02, 0x04);		// MODE [3:0] - 0: All pixel, 1: 720p, 2: Window cropping, 3: 2x2 mode ....
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x03, 0x72);		// HMAX[7:0] - LSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x04, 0x06);		// HMAX[5:0] - MSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x05, 0x28);		// VMAX[7:0] - LSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x06, 0x02);		// VMAX[7:0] - MSB#endif
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x11);		// LVDS 1ch mode (30fps) // TTL  OPORTSEL[4:3] - 2,FRSEL[2:0] - 1 org
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x12, 0x82);		// ADRES[1] - 0: 10bit, 1: 12bit

#endif
#endif // __USE_CMOS_IMX076_LVDS__


#if defined(__USE_CMOS_IMX076_TTL__)		// use sony cmos
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x02, 0x00);		// MODE [3:0] - 0: All pixel, 1: 720p, 2: Window cropping, 3: 2x2 mode ....

	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x03, 0x72);		// HMAX[7:0] - LSB  30Hz
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x04, 0x06);		// HMAX[5:0] - MSB  30Hz
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x03, 0x39);		// HMAX[7:0] - LSB  60Hz
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x04, 0x03);		// HMAX[5:0] - MSB  60H

	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x05, 0x42);		// VMAX[7:0] - LSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x06, 0x04);		// VMAX[7:0] - MSB

	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x01);		// CMOS SDR mode (30fps 10fps) // TTL  OPORTSEL[4:3] - 0,FRSEL[2:0] - 1  54MHz  30Hz
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x00);		// CMOS SDR mode (60fps 30fps) // TTL  OPORTSEL[4:3] - 0,FRSEL[2:0] - 0  54MHz  60Hz
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x12, 0x82);		// ADRES[1] - 0: 10bit, 1: 12bit  30Hz
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x12, 0x80);		// ADRES[1] - 0: 10bit, 1: 12bit 60Hz
#endif


	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x07, 0x00);		// SVS
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x08, 0x00);		// SVS[0] - MSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x09, 0x20);		// SHS1[7:0] - Shutter (LSB)
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x0A, 0x02);		// SHS1[7:0] - Shutter (MSB)
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x0B, 0x00);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x0C, 0x00);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x0D, 0x00);		// SPL
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x0E, 0x00);		// SPL[0] - MSB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x0F, 0x00);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x10, 0x00);
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x01);		// FRSEL[2:0] - 0: 60fps, 1: 30fps	
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x19);		// LVDS DDR 30fps mode // OPORTSEL[4:3] - 0: CMOS SDR, 1: CMOS DDR, 2: LVDS 1ch, 3: LVDS 2ch	
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x18);		// LVDS DDR 60fps mode
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x11, 0x12);		// LVDS 1ch mode (30fps) // TTL  OPORTSEL[4:3] - 2,FRSEL[2:0] - 2  2x2



	
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x13, 0x40);

// jwlee test 20130208
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x14, 0x00); // WINPH L Hori Start Address //100 0x64 // 0
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x15, 0x00); // WINPH H Hori Start Address
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x16, 0x00); // WINPV L Ver Start Address //170 0xAA // 0
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x17, 0x00); // WINPV H Ver Start Address
// 	// 1280x720  0x500 0x2D0  inclk 37.125MHz // 1368x1049 0x558 0x419 (1650x1090)
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x18, 0x58); // WINWH L Hor No. of effective pixel // 1368 0x558
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x19, 0x05); // WINWH H Hor No. of effective pixel // 
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x1A, 0x19); // WINWV L Ver No. of effective pixel // 1049 0x419
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x1B, 0x04); // WINWV H Ver No. of effective pixel // 

//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x14, 0x00); // WINPH L Hori Start Address
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x15, 0x00); // WINPH H Hori Start Address
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x16, 0x00); // WINPV L Ver Start Address
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x17, 0x00); // WINPV H Ver Start Address
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x18, 0x58); // WINWH L Hor No. of effective pixel
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x19, 0x05); // WINWH H Hor No. of effective pixel
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x1A, 0x19); // WINWV L Ver No. of effective pixel
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x1B, 0x04); // WINWV H Ver No. of effective pixel

	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x1C, 0x50);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x1D, 0x00);	

// test 
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x1E, 0x00);		// GAIN [7:0] 0x00  0dB
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x1E, 0x0A);		// GAIN [7:0] 0x0A 3dB
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x1E, 0x14);		// GAIN [7:0] 0x14 6dB
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x1E, 0x28);		// GAIN [7:0] 0x28 12dB
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x1E, 0x50);		// GAIN [7:0]  0x50 24dB

	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x1F, 0x31);	

// jwlee test 20130218
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x20, 0x00);		// BLKLEVEL
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x20, 0xF0);		// BLKLEVEL
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x20, 0x3C);		// BLKLEVEL // default

// jwlee test 20130208
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x21, 0x20);		// BLKLEVEL[0] - MSB , XHSLNG [5:4]
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x22, 0x42);		// XVSLNG [2:0]	
// jwlee test 20130218
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x21, 0x30);		// BLKLEVEL[0] - MSB , XHSLNG [5:4]

// jwlee 20130228
#if defined(__USE_CMOS_IMX076_LVDS__)	// use sony lvds
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x22, 0x43);		// XVSLNG [2:0]	
#endif
#if defined(__USE_CMOS_IMX076_TTL__)		// use sony cmos
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x22, 0x43);		// XVSLNG [2:0]	
#endif


//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x21, 0x00);		// BLKLEVEL[0] - MSB , XHSLNG [5:4]
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x22, 0x40);		// XVSLNG [2:0]	


//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x00, 0x30);		// STANDBY[0] - 0 : Normal operation
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x23, 0x08);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x24, 0x30);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x25, 0x00);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x26, 0x80);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x27, 0x20);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x28, 0x34);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x29, 0x63);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x2A, 0x00);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x2B, 0x00);	
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x2C, 0x00);		// XMSTA[0]

// jwlee test 20130218
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x2D, 0x42);  //DCKDLY 180' CMOS only
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x2D, 0x40);

	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x2E, 0x00);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x2F, 0x02);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x30, 0x30);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x31, 0x20);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x32, 0x00);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x33, 0x14);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x34, 0x20);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x35, 0x60);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x36, 0x00);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x37, 0x23);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x38, 0x01);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x39, 0x00);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x3A, 0xA8);

// jwlee 20130219
#if defined(__USE_CMOS_IMX076_LVDS__)		// use sony cmos
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x3B, 0xD0); // sync code 1 LVDS // SOD, EOD, SOL, EOL, SOF, EOF
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x3B, 0x90); // sync code 2 LVDS // SOL, EOL, SOF, EOF
#endif
#if defined(__USE_CMOS_IMX076_TTL__)		// use sony cmos
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x3B, 0xE0); // parallel
#endif

	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x3C, 0x06);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x40, 0x42);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x75, 0x07);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x76, 0x80);
// jwlee 20130215 test pattern test
// test pattern enable
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x65, 0x81);
//	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x66, 0x02); // pattern Gradation Pattern 1
	
// test pattern disable
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x65, 0x00);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x66, 0x00); // pattern 

	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x07, 0xFA);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x0A, 0x30);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x0B, 0x38);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x12, 0x10);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x16, 0x33);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x17, 0x4D);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x28, 0x05);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x73, 0x50);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x7D, 0x8A);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x7E, 0x60);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x85, 0x70);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x86, 0x42);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x87, 0x16);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x88, 0x00);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x89, 0x61);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x8A, 0x0B);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x8B, 0x29);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x8C, 0x74);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x8D, 0x81);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x8E, 0x10);
	CMOS_ByteWrite(SPI_IMX076_ADDR_2, 0x8F, 0xBA);
	CMOS_ByteWrite(SPI_IMX076_ADDR_1, 0x00, 0x30);		// STANDBY[0] - 0 : Normal operation
}

void IMX076_reg_read(void)
{
	UARTprintf("IMX076_reg_read\n\r");
	
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x00);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x01);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x02);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x03);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x04);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x05);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x06);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x07);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x08);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x09);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x0A);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x0B);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x0C);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x0D);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x0E);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x0F);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x10);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x11);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x12);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x13);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x14);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x15);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x16);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x17);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x18);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x19);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x1A);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x1B);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x1C);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x1D);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x1E);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x1F);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x20);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x21);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x22);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x23);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x24);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x25);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x26);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x27);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x28);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x29);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x2A);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x2B);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x2C);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x2D);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x2E);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x2F);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x30);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x31);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x32);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x33);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x34);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x35);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x36);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x37);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x38);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x39);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x3A);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x3B);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x3C);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x40);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x75);
	CMOS_ByteRead(SPI_IMX076_ADDR_1, 0x76);

	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x07);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x0A);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x0B);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x12);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x16);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x17);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x28);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x73);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x7D);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x7E);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x85);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x86);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x87);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x88);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x89);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x8A);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x8B);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x8C);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x8D);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x8E);
	CMOS_ByteRead(SPI_IMX076_ADDR_2, 0x8F);
}
#endif	/* __USE_CMOS_IMX076__ */

/*  FILE_END_HERE */
