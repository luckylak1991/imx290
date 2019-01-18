// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_IMX238__)
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
void IMX238_ChangeFps_Normal(void) {
	WORD hmax, vmax;
	//60FPS lvds 4ch, see datasheet 49page
	hmax=0x8ca;		//2250
	vmax=0x044c;  	//1100
 	//30FPS lvds 4ch
// 	hmax=0x1194;	//4500
//	vmax=0x044c;  	//1100
	
	//pal system
#if defined(__VTOTAL_CTRL_PAL__)
	if(GetSS_SensorVoutPAL())  vmax*=1.2f;
#else
	if(GetSS_SensorVoutPAL())  hmax=(hmax==0x05D8)?0x05DC:hmax;  //[(hmax*1.5)*1.2] has to be divided into four
	if(GetSS_SensorVoutPAL())  hmax*=1.2f;
#endif

	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x021B, LOBYTE(hmax));	// HMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x021C, HIBYTE(hmax));	// HMAX [7:0] (MSB)
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0218, LOBYTE(vmax));	// VMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0219, HIBYTE(vmax));	// VMAX [7:0] (MSB)



	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0221, 0x00);		// FREQ[1:0]	Set to datarate.		4ch lvds 60fps 12bit

}


//-----------------------------------------------------------------------

void IMX238_Initialize(void)
{	
	UARTprintf("\n\rIMX238_Initialize\n\r");


//////////////////		Chip ID = 02h			//////////////////////	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0201, 0x00);		// Register hold[0]	0: Invalid 1: Valid
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0202, 0x01);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0203, 0x00);		// SW_RESET [0] - 0: operating, Reset
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0204, 0x10);             // Fixed (Default Set:0x10) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0205, 0x01);             // Set (Default Set:0x00) 		
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0206, 0x00);             // Fixed (Default Set:0x0) 		

#if defined(__USE_IMX238_FLIP__)
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0207, 0x03);		// [1:0] Vertical (V) direction readout inversion control,  [3:2] Horiznetal direction readout inversion control
#else
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0207, 0x00);		// [1:0] Vertical (V) direction readout inversion control,  [3:2] Horiznetal direction readout inversion control
#endif
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0208, 0x10);             // Fixed (Default Set:0x10) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0209, 0x01);             // FRSEL [1:0] Set 60fps, if use 2 set 30fps. 
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x020A, 0x3C);		// BLKLEVEL[7:0] - Black level (LSB)
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x020B, 0x00);		// BLKLEVEL[0] - Black level (MSB)		
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x020C, 0x00);             // Fixed (Default Set:0x0) 		
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x020E, 0x20);             // Fixed (Default Set:0x20) 		
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x020D, 0x01);             // Fixed (Default Set:0x01) 		
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x020F, 0x01);             // Fixed (Default Set:0x01) 		
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0210, 0x39);             // Fixed (Default Set:0x39) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0211, 0x14);             // LP_MODE [7:0] 00h: High light performance mode	14h: Low light performance mode (recommend)
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0212, 0x50);             // Fixed (Default Set:0x50) 		
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0213, 0x00);             // Fixed (Default Set:0x00) 			
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0214, 0x00);		// GAIN [7:0] (LSB)	max 480d
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0215, 0x00);             // Fixed (Default Set:0x50) 		
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0216, 0x08);             // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0217, 0x01);             // Set (Default Set:0x01) 		
	
	//set frame rate with h/v total size
	IMX238_ChangeFps_Normal();
	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x021D, 0xFF);             // Set (Default Set:0x14) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x021E, 0x01);             // Set (Default Set:0x02) 		
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x021F, 0x00);             // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0220, 0x00);             // SHS1 [7:0]	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0221, 0x00);             // SHS1 [15:8] 		
	//222~235 do not rewrite
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0236, 0x14);             // WINWV_OB[4:0]
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0237, 0x00);             // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0238, 0x00);             // WINPV [7:0]
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0239, 0x00);             // WINPV [10:8]	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x023A, 0x19);             // WINWV [7:0]
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x023B, 0x04);             // WINWV [10:8]	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x023C, 0x00);             // WINPH [7:0]
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x023D, 0x00);             // WINPH [10:8]	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x023E, 0x1C);             // WINWH [7:0]
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x023F, 0x05);             // WINWH [10:8]	
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0240, 0x00);             // Fixed (Default Set:0x00) 	
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0241, 0x00);             // Fixed (Default Set:0x00) 	
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0242, 0x00);             // Fixed (Default Set:0x00) 	
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0243, 0x00);             // Fixed (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0244, 0xE0|0x01);             // OPORTSEL[3:0] Dh: Serial low-voltage LVDS 2 ch DDR output	Eh: Serial low-voltage LVDS 4 ch DDR output
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0245, 0x01);             // Fixed (Default Set:0x01) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0246, 0x00);             // XVSLNG [1:0]
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0247, 0x00|0x08);             // XHSLNG [1:0]
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0248, 0x00);             // Fixed (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0249, 0x00);             // XVSOUTSEL[1:0], XHSOUTSEL[1:0]
	//24A~253 do not rewrite
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0254, 0x63);             // Set (Default Set:0x61) 	
	//255~25A do not rewrite	

	// INCK setting register		(Refer datasheet!!!)
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x025B, 0x00);		// INCKSEL1 [0] = INCK setting 1	0: 37.125 MHz or 74.25 MHz	1: 27 MHz or 54 MHz
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x025C, 0x20);		// Fixed (Default Set:0x20) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x025D, 0x00);		// INCKSEL2	INCK setting 2	0: Input 27 MHz or 37.125 MHz	1: Input 54 MHz or 74.25 MHz
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x025E, 0x2C);		// Fixed (Default Set:0x20) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x025F, 0x00);		// INCKSEL3	0: 27 [MHz]	1: 54 [MHz]	When INCK frequency is 37.125 MHz or 74.25 MHz, this	register is invalid.
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0260, 0x00);             // Fixed (Default Set:0x00) 	
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0261, 0x21);             // Fixed (Default Set:0x21) 	
//	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0262, 0x08);             // Fixed (Default Set:0x08) 		
	//263~2BE do not rewrite		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x02BF, 0x1F);             // Set (Default Set:0x10) 	
	//2C0~2FF do not rewrite			

	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0312, 0x00);             // Set (Default Set:0x02) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x031D, 0x07);             // Set (Default Set:0x08) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0323, 0x07);             // Set (Default Set:0x0F) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0326, 0xDF);             // Set (Default Set:0x5F) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0347, 0x87);              // Set (Default Set:0xB4) 	
	//3C5~D4 do not rewrite	


	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0403, 0xCD);		 // Fixed (Default Set:0xB4) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0407, 0x4B);		 // Fixed (Default Set:0x32) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0409, 0xE9);		 // Fixed (Default Set:0xEC) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0413, 0x1B);		 // Fixed (Default Set:0xE5) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0415, 0xED);		 // Fixed (Default Set:0xFF) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0416, 0x01);		 // Fixed (Default Set:0x0F) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0418, 0x09);		 // Fixed (Default Set:0x0F) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x041A, 0x19);		 // Fixed (Default Set:0xE3) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x041B, 0xA1);		 // Fixed (Default Set:0x41) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x041C, 0x11);		 // Fixed (Default Set:0x1E) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0427, 0x00);		 // Fixed (Default Set:0x40) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0428, 0x05);		 // Fixed (Default Set:0x03) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0429, 0xEC);		 // Fixed (Default Set:0xD3) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x042A, 0x40);		 // Fixed (Default Set:0xB0) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x042B, 0x11);		 // Fixed (Default Set:0x0F) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x042D, 0x22);		 // Fixed (Default Set:0x06) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x042E, 0x00);		 // Fixed (Default Set:0x40) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x042F, 0x05);		 // Fixed (Default Set:0x03) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0431, 0xEC);		 // Fixed (Default Set:0xD3) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0432, 0x40);		 // Fixed (Default Set:0xB0) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0433, 0x11);		 // Fixed (Default Set:0x0F) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0435, 0x23);		 // Fixed (Default Set:0x07) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0436, 0xB0);		 // Fixed (Default Set:0xF0) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0437, 0x04);		 // Fixed (Default Set:0x02) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0439, 0x24);		 // Fixed (Default Set:0x08) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x043A, 0x30);		 // Fixed (Default Set:0x70) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x043B, 0x04);		 // Fixed (Default Set:0x02) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x043C, 0xED);		 // Fixed (Default Set:0xD4) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x043D, 0xC0);		 // Fixed (Default Set:0x30) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x043E, 0x10);		 // Fixed (Default Set:0x0F) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0440, 0x44);		 // Fixed (Default Set:0x28) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0441, 0xA0);		 // Fixed (Default Set:0xE0) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0442, 0x04);		 // Fixed (Default Set:0x02) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0443, 0x0D);		 // Fixed (Default Set:0xF4) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0444, 0x31);		 // Fixed (Default Set:0xA0) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0445, 0x11);		 // Fixed (Default Set:0x0F) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0447, 0xEC);		 // Fixed (Default Set:0xD3) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0448, 0xD0);		 // Fixed (Default Set:0x40) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0449, 0x1D);		 // Fixed (Default Set:0x1C) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0452, 0xFF);		 // Fixed (Default Set:0xD4) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0453, 0xFF);		 // Fixed (Default Set:0x20) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0454, 0xFF);		 // Fixed (Default Set:0x1D) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0455, 0x02);		 // Fixed (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0456, 0x54);		 // Fixed (Default Set:0x35) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0457, 0x60);		 // Fixed (Default Set:0xD0) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0458, 0x1F);		 // Fixed (Default Set:0x1D) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x045A, 0xA9);		 // Fixed (Default Set:0x90) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x045B, 0x50);		 // Fixed (Default Set:0xC0) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x045C, 0x0A);		 // Fixed (Default Set:0x08) 			

	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x045D, 0x25);		 // Fixed (Default Set:0x0C) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x045E, 0x11);		 // Fixed (Default Set:0x81) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x045F, 0x12);		 // Fixed (Default Set:0x10) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0461, 0x9B);		 // Fixed (Default Set:0x6E) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0466, 0xD0);		 // Fixed (Default Set:0xC0) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0467, 0x08);		 // Fixed (Default Set:0x07) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x046A, 0x20);		 // Fixed (Default Set:0x30) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x046B, 0x0A);		 // Fixed (Default Set:0x08) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x046E, 0x20);		 // Fixed (Default Set:0x30) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x046F, 0x0A);		 // Fixed (Default Set:0x08) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0472, 0x20);		 // Fixed (Default Set:0x30) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0473, 0x0A);		 // Fixed (Default Set:0x08) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0475, 0xEC);		 // Fixed (Default Set:0xD3) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x047D, 0xA5);		 // Fixed (Default Set:0x8C) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x047E, 0x20);		 // Fixed (Default Set:0x90) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x047F, 0x0A);		 // Fixed (Default Set:0x08) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0481, 0xEF);		 // Fixed (Default Set:0xD6) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0482, 0xC0);		 // Fixed (Default Set:0x30) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0483, 0x0E);		 // Fixed (Default Set:0x0D) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0485, 0xF6);		 // Fixed (Default Set:0xDD) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x048A, 0x60);		 // Fixed (Default Set:0xD0) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x048B, 0x1F);		 // Fixed (Default Set:0x1D) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x048D, 0xBB);		 // Fixed (Default Set:0xA2) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x048E, 0x90);		 // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x048F, 0x0D);		 // Fixed (Default Set:0x0C) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0490, 0x39);		 // Fixed (Default Set:0x20) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0491, 0xC1);		 // Fixed (Default Set:0x31) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0492, 0x1D);		 // Fixed (Default Set:0x1C) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0494, 0xC9);		 // Fixed (Default Set:0xB0) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0495, 0x70);		 // Fixed (Default Set:0xE0) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0496, 0x0E);		 // Fixed (Default Set:0x0C) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0497, 0x47);		 // Fixed (Default Set:0x2E) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0498, 0xA1);		 // Fixed (Default Set:0x11) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0499, 0x1E);		 // Fixed (Default Set:0x1D) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x049B, 0xC5);		 // Fixed (Default Set:0xAC) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x049C, 0xB0);		 // Fixed (Default Set:0x20) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x049D, 0x0E);		 // Fixed (Default Set:0x0D) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x049E, 0x43);		 // Fixed (Default Set:0x2A) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x049F, 0xE1);		 // Fixed (Default Set:0x51) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04A0, 0x1E);		 // Fixed (Default Set:0x1D) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04A2, 0xBB);		 // Fixed (Default Set:0x9C) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04A3, 0x10);		 // Fixed (Default Set:0x20) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04A4, 0x0C);		 // Fixed (Default Set:0x0A) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04A6, 0xB3);		 // Fixed (Default Set:0x94) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04A7, 0x30);		 // Fixed (Default Set:0x40) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04A8, 0x0A);		 // Fixed (Default Set:0x08) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04A9, 0x29);		 // Fixed (Default Set:0x10) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04AA, 0x91);		 // Fixed (Default Set:0x01) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04AB, 0x11);		 // Fixed (Default Set:0x10) 		
	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04AD, 0xB4);		 // Fixed (Default Set:0x95) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04AE, 0x40);		 // Fixed (Default Set:0x50) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04AF, 0x0A);		 // Fixed (Default Set:0x08) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04B0, 0x2A);		 // Fixed (Default Set:0x11) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04B1, 0xA1);		 // Fixed (Default Set:0x11) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04B2, 0x11);		 // Fixed (Default Set:0x10) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04B4, 0xAB);		 // Fixed (Default Set:0x8C) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04B5, 0xB0);		 // Fixed (Default Set:0xC0) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04B6, 0x0B);		 // Fixed (Default Set:0x09) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04B7, 0x21);		 // Fixed (Default Set:0x08) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04B8, 0x11);		 // Fixed (Default Set:0x81) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04B9, 0x13);		 // Fixed (Default Set:0x11) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04BB, 0xAC);		 // Fixed (Default Set:0x8D) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04BC, 0xC0);		 // Fixed (Default Set:0xD0) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04BD, 0x0B);		 // Fixed (Default Set:0x09) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04BE, 0x22);		 // Fixed (Default Set:0x09) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04BF, 0x21);		 // Fixed (Default Set:0x91) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04C0, 0x13);		 // Fixed (Default Set:0x11) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04C2, 0xAD);		 // Fixed (Default Set:0x8E) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04C3, 0x10);		 // Fixed (Default Set:0x20) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04C4, 0x0B);		 // Fixed (Default Set:0x09) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04C5, 0x23);		 // Fixed (Default Set:0x0A) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04C6, 0x71);		 // Fixed (Default Set:0xE1) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04C7, 0x12);		 // Fixed (Default Set:0x10) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04C9, 0xB5);		 // Fixed (Default Set:0x96) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04CA, 0x90);		 // Fixed (Default Set:0xA0) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04CB, 0x0B);		 // Fixed (Default Set:0x09) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04CC, 0x2B);		 // Fixed (Default Set:0x12) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04CD, 0xF1);		 // Fixed (Default Set:0x61) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04CE, 0x12);		 // Fixed (Default Set:0x11) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04D0, 0xBB);		 // Fixed (Default Set:0x9C) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04D1, 0x10);		 // Fixed (Default Set:0x20) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04D2, 0x0C);		 // Fixed (Default Set:0x0A) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04D4, 0xE7);		 // Fixed (Default Set:0xCE) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04D5, 0x90);		 // Fixed (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04D6, 0x0E);		 // Fixed (Default Set:0x0D) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04D8, 0x45);		 // Fixed (Default Set:0x2C) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04D9, 0x11);		 // Fixed (Default Set:0x81) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04DA, 0x1F);		 // Fixed (Default Set:0x1D) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04EB, 0xA4);		 // Fixed (Default Set:0x84) 	
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04EC, 0x60);		 // Fixed (Default Set:0xD0) 		
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x04ED, 0x1F);		 // Fixed (Default Set:0x1D) 	


//-------------------------------------------------------------------------------------
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0 : Normal operation
	Delay_mSec(30);										// Wait for internal regulator stabilization
	CMOS_ByteWrite(SPI_IMX238_ADDR, 0x0202, 0x00);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
	Delay_mSec(30);										// 


}



#endif	/* __USE_CMOS_IMX238__ */

/*  FILE_END_HERE */
