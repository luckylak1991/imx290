// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_IMX185__)
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


//-----------------------------------------------------------------------

void IMX185_Initialize(void)
{	
	UARTprintf("\n\rIMX185_Initialize(1080pFHD 4ch lvds mode)\n\r");


//////////////////		Chip ID = 02h			//////////////////////	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0201, 0x00);		// Register hold[0]	0: Invalid 1: Valid
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0202, 0x01);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0203, 0x00);		// Software Reset
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0204, 0x10);		// Fixed (Default Set:0x10) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0205, 0x00|0x01);	// [0] ADBIT [4]STD12EN, Valid when 10bit mode
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0206, 0x00);		// Drive mode [5:0] 00 is all pixel scan mode
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0207, 0x00|0x10);	// [0] Vertical (V) direction readout inversion control,  [1] Horiznetal direction readout inversion control [7:4]  0: WXGA, 1:1080p mode
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0208, 0x10);		// Fixed (Default Set:0x10) 	

	//set frame rate with h/v total size
	IMX185_ChangeFps_Normal();

	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x020A, 0xF0);		// BLKLEVEL[7:0] - Black level (LSB)
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x020B, 0x00);		// BLKLEVEL[0] - Black level (MSB)	

//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x020C, 0x00);             // Fixed (Default Set:0x00) 
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x020D, 0x00);             // Fixed (Default Set:0x00) 	
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x020E, 0x01);             // Fixed (Default Set:0x01) 	
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x020F, 0x01);             // Fixed (Default Set:0x01) 
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0210, 0x39);             // Fixed (Default Set:0x39) 
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0211, 0x00);             // Fixed (Default Set:0x00) 
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0212, 0x50);             // Fixed (Default Set:0x50) 
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0213, 0x00);             // Fixed (Default Set:0x00) 

	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0214, 0x00);		// Gain[7:0]
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0215, 0x00);             // Fixed (Default Set:0x00) 
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0216, 0x08);             // Fixed (Default Set:0x08) 
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0217, 0x00);             // Fixed (Default Set:0x00) 

	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x021D, 0x08);             // Set (Default Set:0xFF) 
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x021E, 0x02);             // Set (Default Set:0x01) 	
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x021F, 0x00);             // Fixed (Default Set:0x00) 
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0220, 0x00);		// SHS1(LSB)
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0221, 0x00);           // SHS1 
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0222, 0x00);           // SHS1(MSB 1bit) 	
	//223~235 do not rewrite
	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0236, 0x10);           // WINWV_OB[4:0]
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0237, 0x00);             // Fixed (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0238, 0x00);           // WINPB(LSB)
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0239, 0x00);           // WINPB(MSB)
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x023A, 0x00);           // WINWV(LSB)
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x023B, 0x00);           // WINWV(MSB)	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x023C, 0x00);           // WINPH(LSB)
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x023D, 0x00);           // WINPH(MSB)	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x023E, 0x00);           // WINWH(LSB)
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x023F, 0x00);           // WINWH(MSB)	
	
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0240, 0x01);           // Fixed (Default Set:0x01) 	
	//241~43 readout only
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0244, 0x01|0xE0);  // Odbit and OPORTSEL
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0245, 0x01);           // Fixed (Default Set:0x01) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0246, 0x00);           // XVSLANG
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0247, 0x08);           // XHSLANG
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0248, 0x33);           // Set (Default Set:0x13) 
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0249, 0x00);           // XHS&XVS out sel
	//24A~5B do not write

	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x025C, 0x20);           // INCLKSEL1
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x025D, 0x00);           // INCLKSEL2
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x025E, 0x18);           // INCLKSEL3
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x025F, 0x00);           // INCLKSEL4
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0260, 0x00);          // Fixed (Default Set:0x00) 
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0261, 0x01);           // Fixed (Default Set:0x01) 
//	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0262, 0x08);           // Fixed (Default Set:0x08) 
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0263, 0x74);           //INCLKSEL5	//see datasheet 111page

// test settings - 20150902
#if defined EN774_HIDDEN_REGISTER_EN					// report from hitron
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0284, 0x0F);             // for test
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0286, 0x10);             // for test
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x02CF, 0xE1);             // for test
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x02D0, 0x29);             // for test
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x02D2, 0x9B);             // for test
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x02D3, 0x01);             // for test
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0326, 0xDF);             // for test
#endif

	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x031D, 0x0A);             // Set (Default Set:0x08) 
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0323, 0x0F);             // Set (Default Set:0x07) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0347, 0x87);             // Set (Default Set:0xB4) 
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x03E1, 0x9E);             // Set (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x03E2, 0x01);            // Set (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x03E5, 0x05);             // Set (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x03E6, 0x05);             // Set (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x03E7, 0x3A);             // Set (Default Set:0x3D) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x03E8, 0x3A);             // Set (Default Set:0x3D) 		
	//2C5~D4 do not write	

	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0403, 0xC8);             // Set (Default Set:0xCD) 
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0407, 0x54);             // Set (Default Set:0x4B) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0413, 0x16);             // Set (Default Set:0x1B) 
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0415, 0xF6);             // Set (Default Set:0xED) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x041A, 0x14);             // Set (Default Set:0x19) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x041B, 0x51);             // Set (Default Set:0xA1) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0429, 0xE7);             // Set (Default Set:0xEC) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x042A, 0xF0);             // Set (Default Set:0x40) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x042B, 0x10);             // Set (Default Set:0x11) 			
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0431, 0xE7);             // Set (Default Set:0xEC) 
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0432, 0xF0);             // Set (Default Set:0x40) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0433, 0x10);             // Set (Default Set:0x11) 
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x043C, 0xE8);             // Set (Default Set:0xED) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x043D, 0x70);		  // Set (Default Set:0xC0) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0443, 0x08);             // Set (Default Set:0x0D) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0444, 0xE1);             // Set (Default Set:0x31) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0445, 0x10);             // Set (Default Set:0x11) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0447, 0xE7);             // Set (Default Set:0xEC) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0448, 0x60);             // Set (Default Set:0xD0) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0449, 0x1E);             // Set (Default Set:0x1D) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x044B, 0x00);             // Set (Default Set:0x05) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x044C, 0x41);             // Set (Default Set:0x91) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0450, 0x30);             // Set (Default Set:0xA0) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0451, 0x0A);             // Set (Default Set:0x08) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0452, 0xFF);             // Set (Default Set:0xD4) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0453, 0xFF);             // Set (Default Set:0x20) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0454, 0xFF);             // Set (Default Set:0x1D) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0455, 0x02);             // Set (Default Set:0x03) 
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0457, 0xF0);             // Set (Default Set:0x60) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x045A, 0xA6);             // Set (Default Set:0xA9) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x045D, 0x14);             // Set (Default Set:0xF3) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x045E, 0x51);             // Set (Default Set:0xF0) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0460, 0x00);             // Set (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0461, 0x61);             // Set (Default Set:0x5E) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0466, 0x30);             // Set (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0467, 0x05);             // Set (Default Set:0x05) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0475, 0xE7);             // Set (Default Set:0xEC) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0481, 0xEA);             // Set (Default Set:0xEF) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0482, 0x70);             // Set (Default Set:0xC0) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0485, 0xFF);             // Set (Default Set:0xF6) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x048A, 0xF0);             // Set (Default Set:0x60) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x048D, 0xB6);             // Set (Default Set:0xBB) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x048E, 0x40);             // Set (Default Set:0x90) 	

	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0490, 0x42);             // Set (Default Set:0x39) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0491, 0x51);            // Set (Default Set:0xC1) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0492, 0x1E);             // Set (Default Set:0x1D) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0494, 0xC4);             // Set (Default Set:0xC9) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0495, 0x20);             // Set (Default Set:0x70) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0497, 0x50);             // Set (Default Set:0x47) 			
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0498, 0x31);             // Set (Default Set:0xA1) 
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0499, 0x1F);             // Set (Default Set:0x1E) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x049B, 0xC0);             // Set (Default Set:0xC5) 
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x049C, 0x60);             // Set (Default Set:0xB0) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x049E, 0x4C);		  // Set (Default Set:0x43) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x049F, 0x71);             // Set (Default Set:0xE1) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04A0, 0x1F);             // Set (Default Set:0x1E) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04A2, 0xB6);             // Set (Default Set:0xBB) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04A3, 0xC0);             // Set (Default Set:0x10) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04A4, 0x0B);             // Set (Default Set:0x0C) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04A9, 0x24);             // Set (Default Set:0x29) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04AA, 0x41);             // Set (Default Set:0x91) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04B0, 0x25);             // Set (Default Set:0x2A) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04B1, 0x51);             // Set (Default Set:0xA1) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04B7, 0x1C);             // Set (Default Set:0x21) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04B8, 0xC1);             // Set (Default Set:0x11) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04B9, 0x12);             // Set (Default Set:0x13) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04BE, 0x1D);             // Set (Default Set:0x22) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04BF, 0xD1);             // Set (Default Set:0x21) 
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04C0, 0x12);             // Set (Default Set:0x13) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04C2, 0xA8);             // Set (Default Set:0xAD) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04C3, 0xC0);             // Set (Default Set:0x10) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04C4, 0x0A);             // Set (Default Set:0x0B) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04C5, 0x1E);             // Set (Default Set:0x23) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04C6, 0x21);             // Set (Default Set:0x71) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04C9, 0xB0);             // Set (Default Set:0xB5) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04CA, 0x40);             // Set (Default Set:0x90) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04CC, 0x26);             // Set (Default Set:0x2B) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04CD, 0xA1);             // Set (Default Set:0xF1) 	

	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04D0, 0xB6);             // Set (Default Set:0xBB) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04D1, 0xC0);             // Set (Default Set:0x10) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04D2, 0x0B);             // Set (Default Set:0x0C) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04D4, 0xE2);             // Set (Default Set:0xE7) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04D5, 0x40);             // Set (Default Set:0x90) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04D8, 0x4E);             // Set (Default Set:0x45) 		
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04D9, 0xA1);             // Set (Default Set:0x11) 	
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x04EC, 0xF0);             // Set (Default Set:0x60) 		

	//05xx~ is mipi(sub-lvds mode register)

//-------------------------------------------------------------------------------------
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0 : Normal operation
	Delay_mSec(50);										// Wait for internal regulator stabilization

	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0202, 0x00);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
														// Wait for image stabilization > 8 frames(=33.33ms * 8)

}


//-------------------------------------------------------------------------------------
//set IMX185 FPS
static void stIMX185_SetFps(IMX185_Fps_t fps) {
	WORD hmax;
	WORD vmax=0x0465;  //1125
	if(fps==IMX185_FPS_60Hz) {
		hmax=0x044C;  //1100
		CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0209, 0x01);		// FRSEL[1:0]	Set to datarate.		4ch lvds 60fps 12bit
	}
	else {
		hmax=0x0898;  //2200
		CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0209, 0x02);		// FRSEL[1:0]	Set to datarate.		4ch lvds 30fps 12bit
	}

	//pal system
#if defined(__VTOTAL_CTRL_PAL__)
	if(GetSS_SensorVoutPAL())  vmax*=1.2f;
#else
	if(GetSS_SensorVoutPAL())  hmax*=1.2f;
#endif

	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x021B, LOBYTE(hmax));	// HMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x021C, HIBYTE(hmax));	// HMAX [7:0] (MSB)
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0218, LOBYTE(vmax));	// VMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0219, HIBYTE(vmax));	// VMAX [7:0] (MSB)
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x021A, 0x00);			// VMAX [0]   (MSB)
}


//-------------------------------------------------------------------------------------
//set IMX185 Frame WDR mode
void IMX185_WDR_Mode(IMX185_Mode_t Mode)  {
	//standby
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0212, 0x01);  // XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0200, 0x01);  // STANDBY[0] - 0: Operation,  1: STANDBY

	//set mode
	switch(Mode) {
		case IMX185_WDR:
			CMOS_ByteWrite(SPI_IMX185_ADDR, 0x020C, 0x10);  // WDSEL[5:4]=1
			break;
		default:
			CMOS_ByteWrite(SPI_IMX185_ADDR, 0x020C, 0x00);  // WDSEL[5:4]=0
			break;
	}

	//change fps
	switch(Mode) {
		case IMX185_WDR:  //60->30Hz
#if	defined(__MDIN_i5XX_FPGA__)
			stIMX185_SetFps(IMX185_FPS_30Hz);
#else
			stIMX185_SetFps(IMX185_FPS_60Hz);
#endif
			break;
		default:  //30Hz
#if defined(__USE_IMX185_1080P60_MODE__)
			stIMX185_SetFps(IMX185_FPS_60Hz);
#else
			stIMX185_SetFps(IMX185_FPS_30Hz);
#endif
			break;
	}

	//release operation
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0200, 0x00);  // STANDBY[0] - 0: Operation,  1: STANDBY
	Delay_mSec(10);                                 // Wait for internal regulator stabilization
	CMOS_ByteWrite(SPI_IMX185_ADDR, 0x0212, 0x00);  // XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
	return;
}

//--------------------------------------------------------------------------------------------------------------------------
//set IMX185 FPS
void IMX185_ChangeFps_Normal(void) {
#if defined(__USE_IMX185_1080P60_MODE__)
	stIMX185_SetFps(IMX185_FPS_60Hz);
#else
	stIMX185_SetFps(IMX185_FPS_30Hz);
#endif
}



#endif	/* __USE_CMOS_IMX185__ */

/*  FILE_END_HERE */
