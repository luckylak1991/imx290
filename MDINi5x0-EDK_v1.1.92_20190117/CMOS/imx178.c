// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_IMX178__)
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
void IMX178_ChangeFps_Normal(void) {
	WORD hmax, vmax;
#if defined(__USE_CMOS_IMX178_LVDS_S4__)			
	#if defined(__USE_IMX178_1080P60_MODE__)
		//hmax=0x02EE;  //750
		#if defined(__MDIN_i500_REF__)
		hmax=0x0300;  //768
		#else
		hmax=0x02FF;  //767
		#endif
		vmax=0x0672;  //1650
	#elif defined(__USE_IMX178_1728P30_MODE__)
	//	hmax=0x04E0;  // original = 0x04E2 (1250)
	//	[2500]*4/3 = 3333.33, 198MHz/(3333.33x1980) = 30.000003fps (It is not 30.0fps, and this value causes H-line skips at intervals of thousands of frames )
	//	[2502]*4/3 = 3336, 198MHz/(3336x(VMAX=7BB)) = 29.991...fps
		hmax=0x09C6;  //2502  original = 0x9C4 //2500
		vmax=0x07BB;  //1980
	#elif defined(__USE_IMX178_5M29_MODE__)
		hmax=0x03E9;  // 1001	(htotal = 2669)
		vmax=0x09AB;  // 2475
	#else
		hmax=0x0898;  //2200
		vmax=0x0465;  //1125	
	#endif
#else	//use LVDS_S8
	#if defined(__USE_IMX178_1080P60_MODE__)
		hmax=0x02EE;  //1100
		vmax=0x0672;  //1650
	#elif defined(__USE_IMX178_1728P30_MODE__)
		hmax=0x09C6;	// set to 15fps
		vmax=0x07BB;
	#elif defined(__USE_IMX178_6M29_MODE__)
		hmax=0x0258;  // 600
		vmax=0x0BBB;  // 3003
	#elif defined(__USE_IMX178_5M29_MODE__)
		hmax=0x0208;  // 520
		vmax=0x0D89;  // 3465
	#else
		hmax=0x0898;  //2200
		vmax=0x0465;  //1125	
	#endif
#endif
 
	//pal system
#if defined(__VTOTAL_CTRL_PAL__)
	if(GetSS_SensorVoutPAL())  vmax*=1.2f;
#else
	if(GetSS_SensorVoutPAL())  hmax*=1.2f;
#endif

#if defined(__USE_MDIN_i500__)  //[MDIN-i500] htotal has to be divided into four
	//hmax=(hmax==0x05D8)?0x05DC:hmax;  //[(hmax*1.5)*1.2]
#endif

	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x022F, LOBYTE(hmax));	// HMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0230, HIBYTE(hmax));	// HMAX [7:0] (MSB)
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x022C, LOBYTE(vmax));	// VMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x022D, HIBYTE(vmax));	// VMAX [7:0] (MSB)
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x022E, 0x00);			// VMAX [0]   (MSB)

	UARTprintf("\n\rHMAX / VMAX = %d/%d\n\r", hmax, vmax);

#if defined(__USE_CMOS_IMX178_LVDS_S4__)				
	#if defined(__USE_IMX178_1080P60_MODE__) || defined(__USE_IMX178_6M29_MODE__) || defined(__USE_IMX178_5M29_MODE__)
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0301, 0x30|0x00);		// FREQ[1:0]	Set to datarate.		
	#elif defined(__USE_IMX178_1728P30_MODE__)
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0301, 0x30|0x01);		// FREQ[1:0]	Set to datarate.
	#else	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0301, 0x30|0x01);		// FREQ[1:0]	Set to datarate.		
	#endif
#else
	#if defined(__USE_IMX178_1080P60_MODE__) || defined(__USE_IMX178_6M29_MODE__) || defined(__USE_IMX178_5M29_MODE__)
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0301, 0x30|0x00);		// FREQ[1:0]	Set to datarate.		
	#elif defined(__USE_IMX178_1728P30_MODE__)
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0301, 0x30|0x02);	// set to 15fps
	#else	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0301, 0x30|0x01);		// FREQ[1:0]	Set to datarate.		
	#endif

#endif


}
//-----------------------------------------------------------------------

void IMX178_Initialize(void)
{	
	//sensor support
	//3072x2048 : only support LVDS parallel mode.	see datasheet 44,47page
	//3072x1728 : window cropiing 16:9 	see datasheet 65page - 5.3Mega(16:9)
	//1920x1080 : hd 1080p 			see datasheet 68page

	BYTE IMX178_WINMODE;

	#if defined(__USE_CMOS_IMX178_LVDS_S4__)			
	UARTprintf("\n\rIMX178_Initialize LVDS_s4 \n\r");
	//support lvds s4 12bit mode(datasheet 44page)
	//1.     12bit Window cropping mode 30fps   
	//1-1.  12bit 1080p 30fps make MDIN-i500 ISP(Image Crop) at Window cropping mode 30fps mode
	//2.     12bit 1080p 60fps, 
	#else
	UARTprintf("\n\rIMX178_Initialize LVDS_s8 \n\r");
	//1.     12bit Window cropping mode 60fps 30fps   
	//1-1.  12bit 1080p 30fps make MDIN-i500 ISP(Image Crop) at Window cropping mode 30fps mode
	//2.     12bit 1080p 60fps, 
	#endif

	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0200, 0x01|0x02|0x04);		// STANDBY[0], STBEXPL[0], STBLOGIC [0] ON
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0203, 0x10|0x40);			// [6] STBDCK2, DCK2 standby control
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0205, 0x00);				// Register hold[0]	0: Invalid 1: Valid
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0208, 0x01);				// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
	
	#if defined(__USE_CMOS_IMX178_LVDS_S4__)			
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0204, 0x03);		// LVDS standby control  0h : 10 ch 1h : 8 ch  3h : 4ch
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0259, 0x01|0x30);	// [6:4] Output channel selection	0: 10ch 1h: 8 ch 3h: 4 ch  [1:0] Number of output bit setting	0: 10 bit 1: 12 bit		
	#else
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0204, 0x01);		// LVDS standby control 0h : 10 ch 1h : 8 ch  3h : 4ch
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0259, 0x01|0x10);	// [6:4] Output channel selection	0: 10ch 1h: 8 ch 3h: 4 ch  [1:0] Number of output bit setting	0: 10 bit 1: 12 bit		
	#endif

	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0209, 0x00);		// Software reset	0d: Operating	1d: Rese
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x020D, 0x05);		// AD conversion bits setting	0: 10 bit 1: 12 bit 2: 14bit

	#if defined(__USE_IMX178_1080P60_MODE__) || defined(__USE_IMX178_1080P30_MODE__)
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x020E, 0x01);		// 00h: All-pixel scan mode	01h: HD 1080p	23h: 2 ¡¿ 2 binning	24h: HD 720p 2 ¡¿ 2 binning	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0266, 0x03);		// VNDMY [7:0]	VSetting of the period from reflection	register updated timing to read start timing	VDesignated in XHS units counted from updated timing	(1d to 255d)	0d: Setting prohibited		
	IMX178_WINMODE=0;									// 0h: All pixel (6 M, HD 1080, HD 720)
	#elif defined(__USE_IMX178_1728P30_MODE__)
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x020E, 0x00);		// 00h: All-pixel scan mode	01h: HD 1080p	23h: 2 ¡¿ 2 binning	24h: HD 720p 2 ¡¿ 2 binning	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0266, 0x06);		// VNDMY [7:0]	VSetting of the period from reflection	register updated timing to read start timing	VDesignated in XHS units counted from updated timing	(1d to 255d)	0d: Setting prohibited	
	IMX178_WINMODE=0x30;								// 3h: Window cropping (16:9)
	#elif defined(__USE_IMX178_5M29_MODE__)
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x020E, 0x00);		// 00h: All-pixel scan mode	01h: HD 1080p	23h: 2 ¡¿ 2 binning	24h: HD 720p 2 ¡¿ 2 binning	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0266, 0x06);		// VNDMY [7:0]	VSetting of the period from reflection	register updated timing to read start timing	VDesignated in XHS units counted from updated timing	(1d to 255d)	0d: Setting prohibited	
	IMX178_WINMODE=0x10;
	#else	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x020E, 0x00);		// 00h: All-pixel scan mode	01h: HD 1080p	23h: 2 ¡¿ 2 binning	24h: HD 720p 2 ¡¿ 2 binning	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0266, 0x06);		// VNDMY [7:0]	VSetting of the period from reflection	register updated timing to read start timing	VDesignated in XHS units counted from updated timing	(1d to 255d)	0d: Setting prohibited	
	IMX178_WINMODE=0x30;								// 3h: Window cropping (16:9)
	#endif

	#if defined(__USE_IMX178_FLIP__)		
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x020F, IMX178_WINMODE|0x03);		// h,v reverse
	#else
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x020F, IMX178_WINMODE|0x00);		// h,v reverse
	#endif
	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0210, 0x00);		// TCYCLE [1:0]
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0215, 0x3C);		// BLKLEVEL[7:0] - Black level (LSB)
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0216, 0x00);		// BLKLEVEL[0] - Black level (MSB)	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x021B, 0x00);		// Light performance mode setting   00(normal) or 1E(low light)
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x021F, 0x00);		//High light Performance mode: 0.0 dB to 48.0 dB, Low light Performance mode: 3.0 dB to 51.0 dB step 0.1db
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0220, 0x00);		//
	
	//set frame rate with h/v total size
	IMX178_ChangeFps_Normal();

	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0234, 0x00);		// SHS [7:0]	(SHS [11:0])
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0235, 0x00);		// SHS [11:8]	(SHS [11:0])

	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0241, 0x00);		// SVS [7:0]	(SVS [9:0]) Specifies the integration shutdown vertical period.Integration time = Setting value + 1 frame
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0242, 0x00);		// SVS [9:8]
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0243, 0x00);		// SPL1 [7:0]	(SVS [9:0]) Designates the number of sweep frames.
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0244, 0x00);		// SPL1 [9:8]

	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x025B, 0x00);		//XVSLNG [1:0]
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x025C, 0x00);		//XHSLNG [1:0]

	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x025E, 0x00);		//XVSOUTSEL [3:2], XHSOUTSEL [1:0]
	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x039C, 0x00);             // WINPH [7:0]
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x039D, 0x00);             // WINPH [10:8]
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x039E, 0x9C);             // WINWH [7:0]
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x039F, 0x07);             // WINWH [11:8]
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x03A0, 0x00);             // VWINPOS [7:0]
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x03A1, 0x00);             // VWINPOS [11:8]
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x03A2, 0x00);             // VWIDCUT [7:0]
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x03A3, 0x00);             // VWIDCUT [12:8]
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x03A4, 0x00);             // WINENH[0]	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x03A5, 0x00);             // VWINSEL [0]

#if defined (__USE_IMX178_OSC27Mhz__)
	// inck 27MHz settigs
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x030C, 0x00);             // DCK2EN [0]	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05BE, 0x21);		 // Set (Default Set:0x0C) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05BF, 0x21);		 // Set (Default Set:0x10) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05C0, 0x2C);		 // Set (Default Set:0x10) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05C1, 0x2C);		 // Set (Default Set:0x10) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05C2, 0x21);		 // Set (Default Set:0x0C) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05C3, 0x2C);		 // Set (Default Set:0x10) 		
//	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05C4, 0x2C);		 // When 594/297MHz, set 0x2C
#if defined (__USE_IMX178_6M29_MODE__) || (defined (__USE_IMX178_5M29_MODE__) && defined(__USE_CMOS_IMX178_LVDS_S8__))
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05C4, 0x20);		 // When 432/216MHz, set 0x20
#else
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05C4, 0x2c);		 // When 594/297MHz, set 0x2C
#endif
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05C5, 0x00);		 // Set (Default Set:0x00) 	
#else
	//inck(37.125Mhz) set. this setting must be done during standby mode
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x030C, 0x00);             // DCK2EN [0]	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05BE, 0x18);		 // Set (Default Set:0x0C) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05BF, 0x18);		 // Set (Default Set:0x10) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05C0, 0x20);		 // Set (Default Set:0x10) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05C1, 0x20);		 // Set (Default Set:0x10) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05C2, 0x18);		 // Set (Default Set:0x0C) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05C3, 0x20);		 // Set (Default Set:0x10) 		
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05C4, 0x20);		 // Set (Default Set:0x10) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05C5, 0x00);		 // Set (Default Set:0x00)
#endif

	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x031C, 0x34);             // Set (Default Set:0x48) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x031D, 0x28);             // Set (Default Set:0x28) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x031E, 0xAB);             // Set (Default Set:0xAB) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x031F, 0x00);             // Set (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0320, 0x95);             // Set (Default Set:0xA8) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0321, 0x00);             // Set (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0322, 0xB4);             // Set (Default Set:0xB3) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0323, 0x00);             // Set (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0324, 0x8C);             // Set (Default Set:0x8B) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0325, 0x02);             // Set (Default Set:0x02) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x032D, 0x03);             // Set (Default Set:0x03) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x032E, 0x0C);             // Set (Default Set:0x16) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x032F, 0x28);             // Set (Default Set:0x17) 		
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0331, 0x2D);             // Set (Default Set:0x36) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0332, 0x00);             // Set (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0333, 0xB4);             // Set (Default Set:0x77) 		
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0334, 0x00);             // Set (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0337, 0x50);             // Set (Default Set:0x50) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0338, 0x08);             // Set (Default Set:0x08) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0339, 0x00);             // Set (Default Set:0x15) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x033A, 0x07);             // Set (Default Set:0x12) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x033D, 0x05);             // Set (Default Set:0x05) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0340, 0x06);             // Set (Default Set:0x06) 	

	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0420, 0x8B);             // Set (Default Set:0x8B) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0421, 0x00);             // Set (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0422, 0x74);             // Set (Default Set:0x74) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0423, 0x00);             // Set (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0426, 0xC2);             // Set (Default Set:0xC2) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0427, 0x00);             // Set (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04A9, 0x1B);             // Set (Default Set:0x1B) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04AA, 0x00);             // Set (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04B3, 0x0E);             // Set (Default Set:0x0E) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04B4, 0x00);             // Set (Default Set:0x00) 	

	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05D6, 0x16);             // Set (Default Set:0x18) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05D7, 0x15);             // Set (Default Set:0x18) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05D8, 0x14);             // Set (Default Set:0x16) 		
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05D9, 0x10);             // Set (Default Set:0x10) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05DA, 0x08);             // Set (Default Set:0x08) 	
	
	//set register
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0211, 0x00);		 // Set (Default Set:0x10) 		
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x021B, 0x00);		 // Set (Default Set:0x0C) 
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0237, 0x08);		 // Set (Default Set:0x07) 
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0238, 0x00);		 // Set (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0239, 0x00);		 // Set (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x02AD, 0x49);		 // Set (Default Set:0x05) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x02AF, 0x54);		 // Set (Default Set:0x14) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x02B0, 0x33);		 // Set (Default Set:0x23) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x02B3, 0x00);		 // Set (Default Set:0x0A) 
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x02C4, 0x30);		 // Set (Default Set:0x10) 	
	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0303, 0x03);		 // Set (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0304, 0x08);		 // Set (Default Set:0x1C) 
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0307, 0x10);		 // Set (Default Set:0x11) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x030F, 0x01);		 // Set (Default Set:0x02) 
	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04E5, 0x00);		 // Set (Default Set:0x0E) 		
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04E6, 0x00);		 // Set (Default Set:0x1F) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04E7, 0x1F);		 // Set (Default Set:0x1F) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04E8, 0x00);		 // Set (Default Set:0x1F) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04E9, 0x00);		 // Set (Default Set:0x1F) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04EA, 0x00);		 // Set (Default Set:0x1E) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04EB, 0x00);		 // Set (Default Set:0x1D) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04EC, 0x00);		 // Set (Default Set:0x1A) 		
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04EE, 0x00);		 // Set (Default Set:0x1E) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04F2, 0x02);		 // Set (Default Set:0x03) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04F4, 0x00);		 // Set (Default Set:0x04) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04F5, 0x00);		 // Set (Default Set:0x06) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04F6, 0x00);		 // Set (Default Set:0x04) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04F7, 0x00);		 // Set (Default Set:0x06) 		
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04F8, 0x00);		 // Set (Default Set:0x06) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x04FC, 0x02);		 // Set (Default Set:0x07) 	

	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0510, 0x11);		 // Set (Default Set:0x12) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0538, 0x81);		 // Set (Default Set:0x41) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x053D, 0x00);		 // Set (Default Set:0x01) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0562, 0x00);		 // Set (Default Set:0x01) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x056B, 0x02);		 // Set (Default Set:0x01) 		
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x056E, 0x11);		 // Set (Default Set:0x21) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05B4, 0xFE);		 // Set (Default Set:0x96) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05B5, 0x06);		 // Set (Default Set:0x06) 	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x05B9, 0x00);		 // Set (Default Set:0x01) 		
//-------------------------------------------------------------------------------------
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0200, 0x00|0x02|0x04);		// STANDBY OFF(1)
	Delay_mSec(5);												// Wait for internal regulator stabilization	
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0200, 0x00|0x00|0x00);		// STANDBY OFF(2)
	Delay_mSec(20);										// Wait for internal regulator stabilization
	CMOS_ByteWrite(SPI_IMX178_ADDR, 0x0208, 0x00);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop


}



#endif	/* __USE_CMOS_IMX178__ */

/*  FILE_END_HERE */
