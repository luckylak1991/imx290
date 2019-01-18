// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_IMX252__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static IMX252_Fps_t imx252_fps=IMX252_FPS_30Hz;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------
static void IMX252_ChangeFps(IMX252_Fps_t fps);
// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//-----------------------------------------------------------------------
void IMX252_ChangeFps_Normal(BYTE nID) {
	switch(nID){
		case  0 : 	
		case  1 :		IMX252_ChangeFps(IMX252_FPS_30Hz);	break;
		case  2 :
		case  3 :		IMX252_ChangeFps(IMX252_FPS_60Hz);	break;
		default : break;
	}

}
//-----------------------------------------------------------------------
#if defined(__USE_IMX252_SLAVE_MODE__)
static void IMX252_Slave_Initialize(void)
{
	UARTprintf("\n\r IMX252 Slave Initialize(MIT)\n\r");

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY	

#if defined(__USE_IMX252_LVDS_S8__)
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0205, 0x10);		// STBLVDS[7:4] - 0: 16ch  1: 8ch  2: 4ch
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x021C, 0x30);		// STBLVDS[7:4] - 1: 16ch  3: 8ch  9: 4ch
#else
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0205, 0x20);		// STBLVDS[7:4] - 0: 16ch  1: 8ch  2: 4ch
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x021C, 0x30);		// STBLVDS[7:4] - 9: 16ch  1: 8ch  3: 4ch		(see image data output format)
#endif
	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x020A, 0x01);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop		

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x020C, 0x01);		// ADBIT [1:0] - 0: 10bit, 1: 12bit  2:8bit
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0216, 0x01);		// ODBIT [1:0] - 0: 10bit, 1: 12bit  2:8bit	

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x020B, 0x00);		// TRIGEN [0] - 0: normal mode, 1: Trigger mode		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0226, 0x00);		// TOUT1SEL[1:0]  TOUT2SEL[3:2]  = 0 pulse low fixed
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0229, 0x00);		// TOUT1SEL[2:0]  TOUT2SEL[6:4]  = 2 pulse1,2 output	

#if defined(__USE_TRIGGER_MODE__)
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x020B, 0x01);		// TRIGEN [0] - 0: normal mode, 1: Trigger mode	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0226, 0x07);		// TOUT1SEL[1:0]  TOUT2SEL[3:2]  = 3 pulse output
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0229, 0x07);		// TOUT1SEL[2:0] = 1 TOUT2SEL[6:4]  = 2 pulse1,2 output	

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x026d, 0x00);		// PULSE1_EN_NOR[0] PULSE1_EN_TRIG[1] 0:disable, 1: en  PULSE1_POL[2] 0 = highactive, 1:lowactive	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0270, 0x00);		// PULSE1_UP	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0271, 0x00);		// PULSE1_UP
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0272, 0x00);		// PULSE1_UP
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0274, 0x00);		// PULSE1_DN
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0275, 0x00);		// PULSE1_DN
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0276, 0x00);		// PULSE1_DN	

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0279, 0x00);		// PULSE2_EN_NOR[0] PULSE2_EN_TRIG[1] 0:disable, 1: en  PULSE2_POL[2] 0 = highactive, 1:lowactive	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x027C, 0x00);		// PULSE2_UP	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x027D, 0x00);		// PULSE2_UP
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x027E, 0x00);		// PULSE2_UP
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0280, 0x00);		// PULSE2_DN
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0281, 0x00);		// PULSE2_DN
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0282, 0x00);		// PULSE2_DN		

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x02AA, 0x01);		// VINT_EN[0], 0: Vinterupt disable, 1:enable, 	Setting interupt mode in trigger mode
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x02AE, 0x00);		// LOWLAGTRG[0], 0: sequential trigger mode, 1:fast-trigger mode	Selection of trigter mode	
	
#else
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x026d, 0x00);		// PULSE1_EN_NOR[0] PULSE1_EN_TRIG[1] 0:disable, 1: en  PULSE1_POL[2] 0 = highactive, 1:lowactive	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0270, 0x00);		// PULSE1_UP	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0271, 0x00);		// PULSE1_UP
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0272, 0x00);		// PULSE1_UP
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0274, 0x00);		// PULSE1_DN
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0275, 0x00);		// PULSE1_DN
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0276, 0x00);		// PULSE1_DN	
	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0279, 0x00|0x08);		// PULSE2_EN_NOR[0] PULSE2_EN_TRIG[1] 0:disable, 1: en  PULSE2_POL[2] 0 = highactive, 1:lowactive	[3] fixed 1
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x027C, 0x00);		// PULSE2_UP	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x027D, 0x00);		// PULSE2_UP
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x027E, 0x00);		// PULSE2_UP
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0280, 0x00);		// PULSE2_DN
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0281, 0x00);		// PULSE2_DN
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0282, 0x00);		// PULSE2_DN	

	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x02AA, 0x01);		// VINT_EN[0], 0: Vinterupt disable, 1:enable, 	Setting interupt mode in trigger mode
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x02AE, 0x00);		// LOWLAGTRG[0], 0: sequential trigger mode, 1:fast-trigger mode	Selection of trigter mode		
#endif


#if defined(__USE_IMX252_FLIP__)
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x020E, 0x03);		// [1:0] Vertical (V) direction readout inversion control,  
#else
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x020E, 0x00);		// [1:0] Vertical (V) direction readout inversion control, 
#endif		

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0236, 0xC0|0x00);	// [7:6] fixed=1, SyncSel[5:4](0:Normal Operation, 3: hi-z)

	//INCKSEL
#if defined(__USE_IMX252_1080P60_MODE__) || defined(__USE_IMX252_1080P30_MODE__)	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x020D, 0x00|0x0C);	// WINMODE[3:0] = 0 all pixel, = C is Full HD mode
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0219, 0x01);		// CKSEL[0]

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0289, 0x18);		// INCKSEL0
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028A, 0x00);		// INCKSEL1
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028B, 0x10);		// INCKSEL2
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028C, 0x02);		// INCKSEL3

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x029E, 0x06);		// GTWAIT
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x02A0, 0x02);		// GSDLY	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x02AF, 0x0A);		// drive mode, full hd - A, all pixel - E
#else
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x020D, 0x00|0x00);	// WINMODE[3:0] = 0 all pixel, = C is Full HD mode
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0219, 0x00);		// CKSEL[0]
	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0289, 0x10);		// INCKSEL0
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028A, 0x02);		// INCKSEL1
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028B, 0x10);		// INCKSEL2
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028C, 0x02);		// INCKSEL3

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x029E, 0x08);		// GTWAIT	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x02A0, 0x04);		// GSDLY		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x02AF, 0x0E);		// drive mode, full hd - A, all pixel - E
#endif	


	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028D, 0x0A);		// SHS[7:0]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028E, 0x00);		// SHS[15:8]	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028F, 0x00);		// SHS[3:0]	

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0404, 0x00);		// GAIN[7:0]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0405, 0x00);		// GAIN[8]	0.1 db stap, 0~12 (refer gaindly 0x412h)
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0412, 0x09);		// GAINDLY	08 : gain reflect at the frame, 09 gain reflect at the next frame
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0454, 0xF0);		// BLKLEVEL[7:0]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0455, 0x00);		// BLKLEVEL[11:8]
	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0500, 0x00);		// FID0_ROIH1ON[0], FID0_ROIV1ON[1]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0510, 0x00);		// FID0_ROIPH1[7:0]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0511, 0x00);		// FID0_ROIPH1[11:8]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0512, 0x00);		// FID0_ROIPV1[7:0]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0513, 0x00);		// FID0_ROIPV1[11:8]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0514, 0x00);		// FID0_ROIWH1[7:0]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0515, 0x00);		// FID0_ROIWH1[12:8]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0516, 0x00);		// FID0_ROIWV1[7:0]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0517, 0x00);		// FID0_ROIWV1[11:8]


	// ChipID = 02
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0201, 0xd0);				// XMSTA
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0202, 0xaa);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0218, 0x01);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0223, 0x00);				//

	// ChipID = 03
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0368, 0xd8);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0369, 0xa0);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x037d, 0xa1);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0380, 0x62);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0390, 0x9b);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0391, 0xa0);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x03a4, 0x3f);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x03a5, 0xb1);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x03e2, 0x00);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x03ea, 0x00);				// 
	// ChipID = 04
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0426, 0x03);				// 
	// ChipID = 07
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x07aa, 0xb3);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x07ac, 0x68);
	// ChipID = 09
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x091c, 0xb4);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x091d, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x091e, 0xde);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x091f, 0x00);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0928, 0xb4);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0929, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x092a, 0xde);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x092b, 0x00);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x093a, 0x36);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0946, 0x36);
	
	// ChipID = 0A
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0ae0, 0xeb);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0ae1, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0ae2, 0x0d);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0ae3, 0x01);
	// ChipID = 0B
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0bc4, 0xeb);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0bc5, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0bc6, 0x0c);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0bc7, 0x01);
	// ChipID = 0F
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f02, 0x6e);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f04, 0xe3);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f05, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f0c, 0x73);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f0e, 0x6e);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f10, 0xe8);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f11, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f12, 0xe3);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f13, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f14, 0x6b);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f16, 0x1c);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f18, 0x1c);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f1a, 0x6b);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f1c, 0x6e);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f1e, 0x9a);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f20, 0x12);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f22, 0x3e);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f28, 0xb4);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f29, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f2a, 0x66);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f34, 0x69);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f36, 0x17);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f38, 0x6a);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f3a, 0x18);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f3e, 0xff);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f3f, 0x0f);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f46, 0xff);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f47, 0x0f);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f4e, 0x4c);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f50, 0x50);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f54, 0x73);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f56, 0x6e);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f58, 0xe8);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f59, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f5a, 0xcf);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f5b, 0x00);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f5e, 0x64);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f66, 0x61);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f6e, 0x0d);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f70, 0xff);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f71, 0x0f);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f72, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f73, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f74, 0x11);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f76, 0x6a);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f78, 0x7f);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f7a, 0xb3);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f7c, 0x29);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f7e, 0x64);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f80, 0xb1);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f82, 0xb3);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f84, 0x62);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f86, 0x64);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f88, 0xb1);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f8a, 0xb3);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f8c, 0x62);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f8e, 0x64);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f90, 0x6d);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f92, 0x65);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f94, 0x65);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f96, 0x6d);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f98, 0x20);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f9a, 0x28);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f9c, 0x81);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f9e, 0x89);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f9f, 0x01);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fa0, 0x66);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fa2, 0x7b);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fa4, 0x21);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fa6, 0x27);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fa8, 0x8b);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fa9, 0x01);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0faa, 0x95);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fab, 0x01);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fac, 0x12);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fae, 0x1c);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fb0, 0x98);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fb1, 0x01);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fb2, 0xa0);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fb3, 0x01);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fb4, 0x13);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fb6, 0x1d);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fb8, 0x99);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fb9, 0x01);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fba, 0xa1);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fbb, 0x01);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fbc, 0x14);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fbe, 0x1e);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fc0, 0x9a);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fc1, 0x01);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fc2, 0xa2);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fc3, 0x01);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fc4, 0x64);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fc6, 0x6e);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fc8, 0x17);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fca, 0x26);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fcc, 0x9d);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fcd, 0x01);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fce, 0xac);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fcf, 0x01);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fd0, 0x65);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fd2, 0x6f);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fd4, 0x18);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fd6, 0x27);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fd8, 0x9e);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fd9, 0x01);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fda, 0xad);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fdb, 0x01);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fdc, 0x66);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fde, 0x70);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fe0, 0x19);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fe2, 0x28);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fe4, 0x9f);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fe5, 0x01);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fe6, 0xae);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fe7, 0x01);	
	// ChipID = 10
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1004, 0x9d);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1006, 0xb0);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1007, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1008, 0x6b);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x100a, 0x7e);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1024, 0xe3);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1025, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1026, 0x9a);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1027, 0x01);
	// ChipID = 11
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1120, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1121, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1122, 0xff);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1123, 0x3f);
	// ChipID = 12
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1203, 0x55);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1205, 0xff);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x120b, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x120c, 0x54);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x120d, 0xb8);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x120e, 0x48);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x120f, 0xa2);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1212, 0x53);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1213, 0x0a);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1214, 0x0c);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1215, 0x0a);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x122a, 0x7f);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x122c, 0x29);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1230, 0x73);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1232, 0x8d);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1233, 0x01);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1249, 0x02);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1256, 0x18);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x128c, 0x9a);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x128e, 0xaa);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1290, 0x3e);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1292, 0x5f);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1294, 0x0a);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1296, 0x0a);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1298, 0x7f);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x129a, 0xb3);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x129c, 0x29);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x129e, 0x64);

//----------------------------------------------------------------------
#if defined(__USE_IMX252_1080P60_MODE__)
	IMX252_ChangeFps_Normal(3);
#elif defined(__USE_IMX252_1080P30_MODE__)	
	IMX252_ChangeFps_Normal(1);
#else
	IMX252_ChangeFps_Normal(1);
#endif

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0: Operation,  1: STANDBY

	//delay more 1ms
	Delay_mSec(5);	
	
}
#else
static void IMX252_Master_Initialize(void)
{
	UARTprintf("\n\r IMX252 Master Initialize\n\r");

	//reg init setting
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY	

#if defined(__USE_IMX252_LVDS_S8__)
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0205, 0x10);		// STBLVDS[7:4] - 0: 16ch  1: 8ch  2: 4ch
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x021C, 0x30);		// STBLVDS[7:4] - 1: 16ch  3: 8ch  9: 4ch
#else
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0205, 0x20);		// STBLVDS[7:4] - 0: 16ch  1: 8ch  2: 4ch
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x021C, 0x30);		// STBLVDS[7:4] - 9: 16ch  1: 8ch  3: 4ch		(see image data output format)
#endif
	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x020A, 0x01);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop		

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x020B, 0x00);		// TRIGEN [0] - 0: normal mode, 1: Trigger mode		

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x020C, 0x01);		// ADBIT [1:0] - 0: 10bit, 1: 12bit  2:8bit
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0216, 0x01);		// ODBIT [1:0] - 0: 10bit, 1: 12bit  2:8bit	

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x021B, 0x00);		// FREQ [1:0] - 0: normal, 1: data rate 1/2
	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0226, 0x00);		// TOUT1SEL[1:0]  TOUT2SEL[3:2]  = 0 pulse low fixed
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0229, 0x00);		// TOUT1SEL[2:0]  TOUT2SEL[6:4]  = 2 pulse1,2 output	

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x026d, 0x00);		// PULSE1_EN_NOR[0] PULSE1_EN_TRIG[1] 0:disable, 1: en  PULSE1_POL[2] 0 = highactive, 1:lowactive	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0270, 0x00);		// PULSE1_UP	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0271, 0x00);		// PULSE1_UP
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0272, 0x00);		// PULSE1_UP
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0274, 0x00);		// PULSE1_DN
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0275, 0x00);		// PULSE1_DN
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0276, 0x00);		// PULSE1_DN	
	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0279, 0x00|0x08);		// PULSE2_EN_NOR[0] PULSE2_EN_TRIG[1] 0:disable, 1: en  PULSE2_POL[2] 0 = highactive, 1:lowactive	[3] fixed 1
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x027C, 0x00);		// PULSE2_UP	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x027D, 0x00);		// PULSE2_UP
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x027E, 0x00);		// PULSE2_UP
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0280, 0x62);		// PULSE2_DN
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0281, 0x00);		// PULSE2_DN
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0282, 0x00);		// PULSE2_DN		 

#if defined(__USE_IMX252_FLIP__)
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x020E, 0x03);		// [1:0] Vertical (V) direction readout inversion control,  
#else
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x020E, 0x00);		// [1:0] Vertical (V) direction readout inversion control, 
#endif	

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0236, 0xC0|0x00);	// [7:6] fixed=1, SyncSel[5:4](0:Normal Operation, 3: hi-z)

	//INCKSEL
#if defined(__USE_IMX252_1080P60_MODE__) || defined(__USE_IMX252_1080P30_MODE__)	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x020D, 0x00|0x0C);	// WINMODE[3:0] = 0 all pixel, = C is Full HD mode
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0219, 0x01);		// CKSEL[0]

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0289, 0x18);		// INCKSEL0
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028A, 0x00);		// INCKSEL1
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028B, 0x10);		// INCKSEL2
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028C, 0x02);		// INCKSEL3

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x029E, 0x06);		// GTWAIT
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x02A0, 0x02);		// GSDLY	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x02AF, 0x0A);		// drive mode, full hd - A, all pixel - E
#else
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x020D, 0x00|0x00);	// WINMODE[3:0] = 0 all pixel, = C is Full HD mode
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0219, 0x00);		// CKSEL[0]
	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0289, 0x10);		// INCKSEL0
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028A, 0x02);		// INCKSEL1
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028B, 0x10);		// INCKSEL2
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028C, 0x02);		// INCKSEL3

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x029E, 0x08);		// GTWAIT	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x02A0, 0x04);		// GSDLY		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x02AF, 0x0E);		// drive mode, full hd - A, all pixel - E
#endif

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028D, 0x0A);		// SHS[7:0]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028E, 0x00);		// SHS[15:8]	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x028F, 0x00);		// SHS[3:0]	

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x02AA, 0x01);		// VINT_EN[0], 0: Vinterupt disable, 1:enable, 	Setting interupt mode in trigger mode
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x02AE, 0x00);		// LOWLAGTRG[0], 0: sequential trigger mode, 1:fast-trigger mode	Selection of trigter mode	


	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0404, 0x00);		// GAIN[7:0]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0405, 0x00);		// GAIN[8]	0.1 db stap, 0~12 (refer gaindly 0x412h)
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0412, 0x09);		// GAINDLY	08 : gain reflect at the frame, 09 gain reflect at the next frame
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0454, 0xF0);		// BLKLEVEL[7:0]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0455, 0x00);		// BLKLEVEL[11:8]
	
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0500, 0x00);		// FID0_ROIH1ON[0], FID0_ROIV1ON[1]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0510, 0x00);		// FID0_ROIPH1[7:0]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0511, 0x00);		// FID0_ROIPH1[11:8]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0512, 0x00);		// FID0_ROIPV1[7:0]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0513, 0x00);		// FID0_ROIPV1[11:8]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0514, 0x00);		// FID0_ROIWH1[7:0]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0515, 0x00);		// FID0_ROIWH1[12:8]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0516, 0x00);		// FID0_ROIWV1[7:0]
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0517, 0x00);		// FID0_ROIWV1[11:8]

#if defined(__USE_IMX252_1080P60_MODE__)
	IMX252_ChangeFps_Normal(3);
#elif defined(__USE_IMX252_1080P30_MODE__)	
	IMX252_ChangeFps_Normal(1);
#else
	IMX252_ChangeFps_Normal(1);
#endif
	//hidden register(JE15x06 2.0.xlsx)
//	UARTprintf("IMX252 HIDDEN REGWRITE\n\r");

	// ChipID = 02
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0201, 0xd0);				// XMSTA
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0202, 0xaa);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0218, 0x01);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0223, 0x00);				//

	// ChipID = 03
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0368, 0xd8);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0369, 0xa0);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x037d, 0xa1);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0380, 0x62);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0390, 0x9b);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0391, 0xa0);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x03a4, 0x3f);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x03a5, 0xb1);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x03e2, 0x00);				// 
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x03ea, 0x00);				// 
	// ChipID = 04
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0426, 0x03);				// 
	// ChipID = 07
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x07aa, 0xb3);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x07ac, 0x68);
	// ChipID = 09
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x091c, 0xb4);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x091d, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x091e, 0xde);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x091f, 0x00);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0928, 0xb4);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0929, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x092a, 0xde);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x092b, 0x00);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x093a, 0x36);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0946, 0x36);
	
	// ChipID = 0A
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0ae0, 0xeb);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0ae1, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0ae2, 0x0d);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0ae3, 0x01);
	// ChipID = 0B
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0bc4, 0xeb);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0bc5, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0bc6, 0x0c);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0bc7, 0x01);
	// ChipID = 0F
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f02, 0x6e);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f04, 0xe3);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f05, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f0c, 0x73);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f0e, 0x6e);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f10, 0xe8);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f11, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f12, 0xe3);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f13, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f14, 0x6b);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f16, 0x1c);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f18, 0x1c);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f1a, 0x6b);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f1c, 0x6e);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f1e, 0x9a);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f20, 0x12);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f22, 0x3e);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f28, 0xb4);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f29, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f2a, 0x66);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f34, 0x69);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f36, 0x17);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f38, 0x6a);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f3a, 0x18);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f3e, 0xff);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f3f, 0x0f);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f46, 0xff);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f47, 0x0f);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f4e, 0x4c);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f50, 0x50);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f54, 0x73);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f56, 0x6e);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f58, 0xe8);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f59, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f5a, 0xcf);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f5b, 0x00);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f5e, 0x64);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f66, 0x61);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f6e, 0x0d);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f70, 0xff);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f71, 0x0f);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f72, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f73, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f74, 0x11);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f76, 0x6a);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f78, 0x7f);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f7a, 0xb3);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f7c, 0x29);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f7e, 0x64);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f80, 0xb1);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f82, 0xb3);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f84, 0x62);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f86, 0x64);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f88, 0xb1);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f8a, 0xb3);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f8c, 0x62);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f8e, 0x64);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f90, 0x6d);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f92, 0x65);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f94, 0x65);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f96, 0x6d);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f98, 0x20);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f9a, 0x28);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f9c, 0x81);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f9e, 0x89);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0f9f, 0x01);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fa0, 0x66);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fa2, 0x7b);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fa4, 0x21);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fa6, 0x27);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fa8, 0x8b);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fa9, 0x01);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0faa, 0x95);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fab, 0x01);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fac, 0x12);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fae, 0x1c);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fb0, 0x98);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fb1, 0x01);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fb2, 0xa0);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fb3, 0x01);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fb4, 0x13);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fb6, 0x1d);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fb8, 0x99);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fb9, 0x01);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fba, 0xa1);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fbb, 0x01);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fbc, 0x14);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fbe, 0x1e);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fc0, 0x9a);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fc1, 0x01);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fc2, 0xa2);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fc3, 0x01);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fc4, 0x64);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fc6, 0x6e);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fc8, 0x17);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fca, 0x26);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fcc, 0x9d);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fcd, 0x01);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fce, 0xac);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fcf, 0x01);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fd0, 0x65);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fd2, 0x6f);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fd4, 0x18);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fd6, 0x27);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fd8, 0x9e);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fd9, 0x01);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fda, 0xad);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fdb, 0x01);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fdc, 0x66);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fde, 0x70);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fe0, 0x19);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fe2, 0x28);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fe4, 0x9f);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fe5, 0x01);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fe6, 0xae);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0fe7, 0x01);	
	// ChipID = 10
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1004, 0x9d);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1006, 0xb0);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1007, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1008, 0x6b);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x100a, 0x7e);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1024, 0xe3);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1025, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1026, 0x9a);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1027, 0x01);
	// ChipID = 11
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1120, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1121, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1122, 0xff);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1123, 0x3f);
	// ChipID = 12
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1203, 0x55);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1205, 0xff);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x120b, 0x00);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x120c, 0x54);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x120d, 0xb8);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x120e, 0x48);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x120f, 0xa2);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1212, 0x53);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1213, 0x0a);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1214, 0x0c);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1215, 0x0a);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x122a, 0x7f);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x122c, 0x29);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1230, 0x73);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1232, 0x8d);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1233, 0x01);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1249, 0x02);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1256, 0x18);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x128c, 0x9a);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x128e, 0xaa);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1290, 0x3e);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1292, 0x5f);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1294, 0x0a);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1296, 0x0a);
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x1298, 0x7f);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x129a, 0xb3);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x129c, 0x29);		
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x129e, 0x64);

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0: Operation,  1: STANDBY

	//delay more 1ms
	Delay_mSec(5);	

	//XMASTER = 0. after 8 frame, image stable
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x020A, 0x00);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop		
}
#endif
//-----------------------------------------------------------------------
static void IMX252_ChangeFps(IMX252_Fps_t fps) 
{
	WORD hmax, vmax;

	imx252_fps=fps;
	#if defined (__USE_IMX252_1080P30_MODE__) || defined (__USE_IMX252_1080P60_MODE__) 	
		switch(fps) {
			case IMX252_FPS_30Hz:
				hmax=0x0898;	//2200
				vmax=0x0465;  	//1125
			break;
			case IMX252_FPS_60Hz:
				hmax=0x044C;	//1100
				vmax=0x0465;  	//1125
			break;				
			default:
				hmax=0x0898;	//2200
				vmax=0x0465;  	//1125
			break;
	}
	#else
	//3M 1536p sensor max : 55.6 frame... 
			hmax=0x34e;		
			vmax=0x628;  
	#endif

	//pal system
#if defined(__VTOTAL_CTRL_PAL__)
	if(GetSS_SensorVoutPAL())  vmax*=1.2f;
#else
//	if(GetSS_SensorVoutPAL())  hmax=(hmax==0x05D8)?0x05DC:hmax;  //[(hmax*1.5)*1.2] has to be divided into four
	if(GetSS_SensorVoutPAL())  hmax*=1.2f;
#endif



//	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY
//	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0221, 0x00);		// FREQ[1:0]	Set to datarate.		4ch lvds 60fps 12bit

	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0214, LOBYTE(hmax));	// HMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0215, HIBYTE(hmax));	// HMAX [7:0] (MSB)
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0210, LOBYTE(vmax));	// VMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0211, HIBYTE(vmax));	// VMAX [15:8] (MSB)
	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0212, 0);				// VMAX [19:16](MSB)	
	
//	CMOS_ByteWrite(SPI_IMX252_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0: Operation,  1: STANDBY	
	
}
//-----------------------------------------------------------------------
BOOL IMX252_SensorInformation(void)
{
	BOOL	GraySensor;
	BYTE	temp1, temp2;
	BYTE	sensor_id;

	temp1=CMOS_ByteRead(SPI_IMX252_ADDR, 0x348);
	temp2=CMOS_ByteRead(SPI_IMX252_ADDR, 0x349);

	sensor_id=(temp2&0x7F)*4 + (temp1&0xC0);
	
	if(temp2&0x80) {
		GraySensor=1;		//this is mono sensor
		UARTprintf("[IMX %d - Mono version] \n\r",sensor_id);	
	}
	else{
		GraySensor=0;
		UARTprintf("[IMX %d - Color version] \n\r",sensor_id);	
	}

	return GraySensor;
}
//-----------------------------------------------------------------------
void IMX252_Initialize(void)
{	
#if defined(__USE_IMX252_1536P_ALL_SCAN_MODE__)
	UARTprintf("\n\r[All pixel scan mode 12bit 2048x1536, osc37.125Mhz] \n\r");
#elif defined(__USE_IMX252_1080P60_MODE__)
	UARTprintf("\n\r[1080p60 mode 12bit lvds 4ch mode, osc37.125Mhz] \n\r");	
#else
	UARTprintf("\n\r[1080p30 mode 12bit lvds 4ch mode, osc37.125Mhz] \n\r");	
#endif

	
#if defined(__USE_IMX252_SLAVE_MODE__)
	IMX252_Slave_Initialize();		//use slave sync..
#else
	IMX252_Master_Initialize();
#endif

	IMX252_SensorInformation();

}

#endif	/* __USE_CMOS_IMX252__ */

/*  FILE_END_HERE */
