// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_IMX174__) || defined(__USE_CMOS_IMX249__)
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
void stIMX174_SetFps(IMX174_Fps_t fps) {
	WORD vmax=0x0465;
	WORD hmax=0x0898;
	switch(fps) {
		case IMX174_FPS_30Hz:
		#if defined(__USE_CMOS_IMX249__)
//			hmax=0x1130;  //2200
			hmax=0x0898;  //2200
		#else
			CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0221, 0x01);		// FREQ[1:0]	Set to datarate.  4ch lvds 30fps 12bit
			hmax=0x0898;  //2200
		#endif
			break;
		case IMX174_FPS_60Hz:
		default:
		#if defined(__USE_CMOS_IMX249__)
			hmax=0x0898;  //2200
		#else	
			CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0221, 0x00);		// FREQ[1:0]	Set to datarate.  4ch lvds 60fps 12bit
			hmax=0x044C;  //1100
		#endif
			break;
	}

	//pal system
#if defined(__VTOTAL_CTRL_PAL__)
	if(GetSS_SensorVoutPAL())  vmax*=1.2f;
#else
	if(GetSS_SensorVoutPAL())  hmax*=1.2f;
#endif

	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x021A, LOBYTE(hmax));	// HMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x021B, HIBYTE(hmax));	// HMAX [7:0] (MSB)
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0217, LOBYTE(vmax));	// VMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0218, HIBYTE(vmax));	// VMAX [7:0] (MSB)
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0219, 0x00);			// VMAX [0]   (MSB)
}


//-----------------------------------------------------------------------

void IMX174_Initialize(void)
{	
#if defined(__USE_CMOS_IMX249__)
	UARTprintf("\n\rIMX249_Initialize(1080pFHD mode)\n\r");
#else
	UARTprintf("\n\rIMX174_Initialize(1080pFHD mode)\n\r");
#endif

	// refer to Data_Sheet_E14511C52
//////////////////		Chip ID = 02h			//////////////////////	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY
	//201~204 fixed 00h
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0205, 0x01);		// STBLVDS [3:0] LVDS channels that not using be standby	0h: 8 ch active	1h: 4 ch active	2h: 2 ch active Others: Setting prohibited
#if defined(__USE_CMOS_IMX249__)
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0205, 0x20);		// STBLVDS [3:0] LVDS channels that not using be standby	2h: 2 ch support only at imx249
#else
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0205, 0x10);		// STBLVDS [3:0] LVDS channels that not using be standby	0h: 8 ch active	1h: 4 ch active	2h: 2 ch active Others: Setting prohibited
#endif

	//205~20B fixed 00h
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x020C, 0x00);		// Register hold[0]	0: Invalid 1: Valid
	//20D~211 fixed 00h	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0212, 0x01);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
#if defined(__USE_TRIGGER_MODE__) &! defined(__USE_MDIN_i500__)		//mdin i500 dosn't support trigger shutter control.
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0213, 0x01);		// Global shutter mode setting	0: Normal mode 1: Trigger mode
#else
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0213, 0x00);		// Global shutter mode setting	0: Normal mode 1: Trigger mode
#endif
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0214, 0x01);		// AD conversion bits setting	0: 10 bit 1: 12 bit
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0215, 0x04);		// Drive mode setting	0h: WUXGA 2h: UXGA 4h: 1080p-Full HD	Others: Setting prohibited
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0216, 0x00);		// [1:0] Vertical (V) direction readout inversion control,  [3:2] Horiznetal direction readout inversion control

	//set frame rate with h/v total size
#if defined(__USE_IMX174_1080P60_MODE__)
	stIMX174_SetFps(IMX174_FPS_60Hz);
#else
	stIMX174_SetFps(IMX174_FPS_30Hz);
#endif

#if defined(__USE_CMOS_IMX249__)
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x021C, 0x41);		// [6:4] Output channel selection	fixed 2 ch & 12 bit
#else
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x021C, 0x01|0x30);	// [6:4] Output channel selection	1h: 8 ch 3h: 4 ch 4h: 2 ch  [1:0] Number of output bit setting	0: 10 bit 1: 12 bit	
#endif

	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x021d, 0x00);             // Fixed (Default Set:0x01) 
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x021e, 0x02);             // Fixed (Default Set:0x02) 
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x021f, 0x01);             // CKSEL[0] The value is set according to drive mode.	When WUXGA, UXGA, ROI: 0	When 1080p-Full HD: 1
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0220, 0x01);             // Fixed (Default Set:0x01) 

	//222~227 do not rewrite
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0228, 0x30);             // Fixed (Default Set:0x01) 	
	//229~22D do not rewrite
#if defined(__USE_TRIGGER_MODE__)		
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x022E, 0x00|0x00);	// XVSOUTSEL[1:0]	XHSOUTSEL[1:0], 2:Master, 0:Slave mode //slave mode setting
#else
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x022E, 0x02|0x08);	// XVSOUTSEL[1:0]	XHSOUTSEL[1:0], 2:Master, 0:Slave mode //master mode setting
#endif
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x022F, 0x00|0x00);	// TOUT1SEL[1:0]	TOUT2SEL[1:0], 3:Purse output , 0:Low fixed
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0230, 0x00);             // Fixed (Default Set:0x00) 
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0231, 0x00);             // Fixed (Default Set:0x00) 
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0232, 0x00|0x00);	// TRIG_TOUT1_SEL[2:0]	TRIG_TOUT2_SEL[6:4], 2:Purse output , 0:Low fixed
	//233~275 do not rewrite
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0276, 0x00);		// [0]:PULSE1_EN_NOR [0], [1]:PULSE1_EN_TRIG [0], [2]:PULSE1_POL [0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0277, 0x00);		// PULSE1_UP[7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0278, 0x00);		// PULSE1_UP[15:8]
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0279, 0x00);             // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x027A, 0x00);		// PULSE1_DN[7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x027B, 0x00);		// PULSE1_DN[15:8]
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x027C, 0x00);             // Fixed (Default Set:0x00) 	
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x027D, 0x00);             // Fixed (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x027E, 0x00);		// [0]:PULSE2_EN_NOR [0], [1]:PULSE2_EN_TRIG [0], [2]:PULSE2_POL [0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x027F, 0x00);		// PULSE2_UP[7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0280, 0x00);		// PULSE2_UP[15:8]
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0281, 0x00);             // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0282, 0x00);		// PULSE2_DN[7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0283, 0x00);		// PULSE2_DN[15:8]
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0284, 0x00);             // Fixed (Default Set:0x00) 		
	//285~291 do not rewrite	
	
	// INCK setting register		(Refer datasheet!!!)
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0292, 0x18);		// INCKSEL0
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0293, 0x00);		// INCKSEL1
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0294, 0x20);		// INCKSEL2
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0295, 0x00);		// INCKSEL3
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0296, 0x00);             // Fixed (Default Set:0x00) 		
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0297, 0x00);             // Fixed (Default Set:0x00) 		
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0298, 0x00);             // Fixed (Default Set:0x00) 		
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0299, 0x00);             // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x029A, 0x0A);		// SHS [7:0]	(SHS [11:0])
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x029B, 0x00);		// SHS [11:8]	(SHS [11:0])

#if defined(__USE_CMOS_IMX249__)	
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x029C, 0x00);             // Fixed (Default Set:0x00) 		
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x029D, 0x6E);             // Fixed (Default Set:0x6E) 		
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x029E, 0x02);             // Fixed (Default Set:0x02) 		
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x029F, 0x00);             // Fixed (Default Set:0x00) 		
#else
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x029C, 0x00);             // Fixed (Default Set:0x00) 		
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x029D, 0x00);             // Fixed (Default Set:0x00) 		
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x029E, 0x00);             // Fixed (Default Set:0x00) 		
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x029F, 0x00);             // Fixed (Default Set:0x00) 	
#endif

	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02A0, 0x64);		// The value is set according to drive mode.	When WUXGA, UXGA, ROI: A4h		When 1080p-Full HD: 64h
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02A0, 0xA4);		// The value is set according to drive mode.	When WUXGA, UXGA, ROI: A4h		When 1080p-Full HD: 64h
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02A1, 0x02);             // Fixed (Default Set:0x02) 		
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02A2, 0x01);             // Fixed (Default Set:0x01) 		
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02A3, 0x00);             // Fixed (Default Set:0x00) 		
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02A4, 0x00);             // Fixed (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02A5, 0x04);           // GSDLY [7:0]		The value is set according to drive mode.	When WUXGA, UXGA, ROI: 08h		When 1080p-Full HD: 04h
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02A5, 0x08);           // GSDLY [7:0]		The value is set according to drive mode.	When WUXGA, UXGA, ROI: 08h	
	//2A6~2BB do not rewrite		
#if defined(__USE_CMOS_IMX249__)	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02A9, 0x30);		// LVDS output timing 8ch:0Ch  4ch:18h  2ch:30h
#else
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02A9, 0x18);		// LVDS output timing 8ch:0Ch  4ch:18h  2ch:30h
#endif

	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02BC, 0x10);           // 10bit: Fixed to 30h / 12bit: Fixed to 10h 
	//2BD do not rewrite			
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02BE, 0x45);             // Fixed (Default Set:0x3E) 		
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02BF, 0x20);             // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02C0, 0x02);             // Fixed (Default Set:0x00) 		
	//2C1 do not rewrite	
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02C2, 0x0A);             // Fixed (Default Set:0x20) 		
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02C2, 0xA0);             // Fixed (Default Set:0x20) 		
	//2C3~D6 do not rewrite	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02C6, 0x03);             // 10bit: Fixed to 01h / 12bit: Fixed to 03h  		
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02D2, 0x0F);             // 10bit: Fixed to 05h / 12bit: Fixed to 0Fh 		
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x02D7, 0x00);             // Fixed (Default Set:0x01) 	
	//2D8~FF do not rewrite	

#if defined(__USE_CMOS_IMX174__)	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0300, 0x00);             // ROIH1ON [0], ROIV1ON [0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0301, 0x00);             // ROIPH1 [7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0302, 0x00);             // ROIPH1 [10:8]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0303, 0x00);             // ROIPV1 [7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0304, 0x00);             // ROIPV1 [10:8]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0305, 0x00);             // ROIWH1 [7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0306, 0x00);             // ROIWH1 [10:8]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0307, 0x00);             // ROIWV1 [7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0308, 0x00);             // ROIWV1 [10:8]
	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0309, 0x00);             // ROIH2ON [0], ROIV2ON [0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x030A, 0x00);             // ROIPH2 [7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x030B, 0x00);             // ROIPH2 [10:8]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x030C, 0x00);             // ROIPV2 [7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x030D, 0x00);             // ROIPV2 [10:8]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x030E, 0x00);             // ROIWH2 [7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x030F, 0x00);             // ROIWH2 [10:8]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0310, 0x00);             // ROIWV2 [7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0311, 0x00);             // ROIWV2 [10:8]
	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0312, 0x00);             // ROIH3ON [0], ROIV3ON [0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0313, 0x00);             // ROIPH3 [7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0314, 0x00);             // ROIPH3 [10:8]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0315, 0x00);             // ROIPV3 [7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0316, 0x00);             // ROIPV3 [10:8]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0317, 0x00);             // ROIWH3 [7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0318, 0x00);             // ROIWH3 [10:8]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0319, 0x00);             // ROIWV3 [7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x031A, 0x00);             // ROIWV3 [10:8]

	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x031B, 0x00);             // ROIH4ON [0], ROIV4ON [0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x031C, 0x00);             // ROIPH4 [7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x031D, 0x00);             // ROIPH4 [10:8]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x031E, 0x00);             // ROIPV4 [7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x031F, 0x00);             // ROIPV4 [10:8]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0320, 0x00);             // ROIWH4 [7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0321, 0x00);             // ROIWH4 [10:8]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0322, 0x00);             // ROIWV4 [7:0]
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0323, 0x00);             // ROIWV4 [10:8]
	//324~FF do not rewrite	
#endif

//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0400, 0x01);		// Fixed (Default Set:0x01) 		
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0401, 0x00);             // Fixed (Default Set:0x00) 		
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0402, 0xF0);             // Fixed (Default Set:0xF0) 		
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0403, 0x00);             // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0404, 0x00);		// GAIN [7:0] (LSB)	max 480d
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0405, 0x00);		// GAIN [0] (MSB)
	//406~11 do not rewrite	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0412, 0x20);		 // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0413, 0x20);		 // Fixed (Default Set:0x00) 			
	//414~19 do not rewrite	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x041A, 0x08);		 //10bit: Fixed to 0Fh / 12bit: Fixed to 08h *
	//41B~57h do not rewrite	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0458, 0xF0);		// BLKLEVEL[7:0] - Black level (LSB)
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0459, 0x00);		// BLKLEVEL[0] - Black level (MSB)	
	//45A~FF do not rewrite

	//500~66 do not rewrite		
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0567, 0x04);		// Fixed (Default Set:0x34) 	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0568, 0x11);		// Fixed (Default Set:0x46) 
	//569~6B do not rewrite	
#if defined(__USE_CMOS_IMX249__)		
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x056A, 0x11);		// low power mode is 0x21(fixed), normal mode is 0x11(fixed)
#endif
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x056C, 0x05);		// Fixed (Default Set:0x00) 	
	//56D~72 do not rewrite	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0573, 0x0C);		// Fixed (Default Set:0x2C) 	
	//574 do not rewrite	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0575, 0x0F);		// 10bit: Fixed to 0Bh / 12bit: Fixed to 0Fh *
	//576~8E do not rewrite	

#if defined(__USE_CMOS_IMX249__)	
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0586, 0xFE);		// low power mode is 0x68, normal is 0xFE
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0587, 0x14);		// low power mode is 0x10, normal is 0x14
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x05A8, 0x33);		// low power mode is 0x31, normal is 0x33
#endif
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x058F, 0x7C);		// Fixed (Default Set:0x00) 	

	//590~FF do not rewrite	

#if defined(__USE_CMOS_IMX249__)	
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x062A, 0xFF);		// low power mode is 0x90, normal is 0xFF
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x062B, 0xFF);		// low power mode is 0x51, normal is 0xFF
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x062C, 0xFF);		// low power mode is 0xC9, normal is 0xFF
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x064C, 0xF0);		// low power mode is 0xA0, normal is 0xF0
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0652, 0xB0);		// low power mode is 0x90, normal is 0xB0
//	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0655, 0xD0);		// low power mode is 0xB0, normal is 0xD0
#endif
	//600~FF do not rewrite

	//700~B6 do not rewrite
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x07B7, 0x04);		// Fixed (Default Set:0x00) 	
	//7B8~7C4 do not rewrite	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x07C5, 0x85);		// Fixed (Default Set:0x80) 	
	//7C6~7D4 do not rewrite	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x07D5, 0x5A);		// Fixed (Default Set:0x0A)
	//7D6~7FF do not rewrite		
	
	//800~24 do not rewrite
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0825, 0x10);		// Fixed (Default Set:0x80) 	
	//826~2A do not rewrite	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x082B, 0xE0);		// Fixed (Default Set:0x50) 	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x082C, 0x0A);		// Fixed (Default Set:0x0B) 		
	//82D~2F do not rewrite	
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0830, 0xAF);		// Fixed (Default Set:0xB6)
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0831, 0x10);		// Fixed (Default Set:0x80)	
	//832~FF do not rewrite		

	//900~FF do not rewrite
	
//-------------------------------------------------------------------------------------
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0 : Normal operation
	Delay_mSec(10);										// Wait for internal regulator stabilization

	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0212, 0x00);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
	Delay_mSec(300);									// Wait for image stabilization > 8 frames(=33.33ms * 8)

}

//set IMX174 Frame WDR mode
void IMX174_WDR_Mode(IMX174_Mode_t Mode)
{
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0212, 0x01);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY

	//set mode
	switch(Mode) {
		case IMX174_WDR:
			CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0226, 0x01);	// WDSEL[0] (0:Normal mode, 1:WDR mode)
			break;
		default:
			CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0226, 0x00);	// WDSEL[0] (0:Normal mode, 1:WDR mode)
			break;
	}

	//change fps
#if	!defined(__MDIN_i5XX_FPGA__)||defined(__USE_DDR_MCLK162__)
	const BOOL valid_60fps=TRUE;
#else
	const BOOL valid_60fps=FALSE;
#endif
#if defined(__USE_IMX174_1080P60_MODE__)
	switch(Mode) {
		case IMX174_WDR:  //60->30Hz
			if(valid_60fps)  stIMX174_SetFps(IMX174_FPS_60Hz);
			else             stIMX174_SetFps(IMX174_FPS_30Hz);
			break;
		default:  //60Hz
			if(valid_60fps)  stIMX174_SetFps(IMX174_FPS_60Hz);
			else             stIMX174_SetFps(IMX174_FPS_30Hz);
			break;
	}
#else
	switch(Mode) {
		case IMX174_WDR:  //60->30Hz
			if(valid_60fps)  stIMX174_SetFps(IMX174_FPS_60Hz);
			else             stIMX174_SetFps(IMX174_FPS_30Hz);
			break;
		default:
			if(valid_60fps)  stIMX174_SetFps(IMX174_FPS_30Hz);
			else             stIMX174_SetFps(IMX174_FPS_30Hz);
			break;
	}
#endif

	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0: Operation,  1: STANDBY
	Delay_mSec(10);										// Wait for internal regulator stabilization
	CMOS_ByteWrite(SPI_IMX174_ADDR, 0x0212, 0x00);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
	//Delay_mSec(300);									// Wait for image stabilization > 8 frames(=33.33ms * 8)

	return;
}

//set FPS regarding NTSC/PAL system
void IMX174_ChangeFps_Normal(BYTE nID) {
	switch(nID){
		case  0 : 	
		case  1 :		stIMX174_SetFps(IMX174_FPS_30Hz);	break;
		case  2 :
		case  3 :		stIMX174_SetFps(IMX174_FPS_60Hz);	break;
		default : break;
	}
}

#endif	/* __USE_CMOS_IMX174__ */

/*  FILE_END_HERE */
