// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_IMX253__)
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
static DWORD vmax;
void IMX253_ChangeFps_Normal(void) {
	WORD hmax;

#if defined(__USE_IMX253_1080P60_MODE__) 
	hmax=0x044C;	//1100
	vmax=0x0465;  	//1125
#elif defined (__USE_IMX253_1080P30_MODE__)	
	hmax=0x0898;	//2200
	vmax=0x0465;  	//1125
#else
//	#if defined(__USE_SENSOR_BAYER_10BIT__)
//	hmax=0x555;		//clock count
//	vmax=0xBF6;  				//line count
//	#else
	hmax=0x44C;				//clock count
	vmax=0x8CA;  				//line count
//	hmax=0x411;						//clock count : imx267
//	vmax=0x8A2;  					//line count : imx267
#endif

	//pal system
#if defined(__VTOTAL_CTRL_PAL__)
	if(GetSS_SensorVoutPAL())  vmax*=1.2f;
#else
	if(GetSS_SensorVoutPAL())  hmax=(hmax==0x05D8)?0x05DC:hmax;  //[(hmax*1.5)*1.2] has to be divided into four
	if(GetSS_SensorVoutPAL())  hmax*=1.2f;
#endif

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0214, LOBYTE(hmax));	// HMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0215, HIBYTE(hmax));	// HMAX [7:0] (MSB)
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0210, LOBYTE(LOWORD(vmax)));	// VMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0211, HIBYTE(LOWORD(vmax)));	// VMAX [15:8] (MSB)
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0212, LOBYTE(HIWORD(vmax)));	// VMAX [19:16] (MSB)


	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0221, 0x00);		// FREQ[1:0]	Set to datarate.		4ch lvds 60fps 12bit

}

DWORD CMOS_GetVmax(void)
{
	return vmax;
}

void IMX253_Initialize(void)
{	
	UARTprintf("\n\r IMX253 Master Initialize\n\r");			//XMASTER PIN GND Low
	//UARTprintf("\n\r IMX253 Slave Mode Initialize\n\r"); 		//XMASTER PIN OVdd High
	
	UARTprintf("\n\r [All pixel scan mode 12bit 4112x3018 lvds 8ch mode, osc37.125Mhz] \n\r");

	//reg init setting
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x020A, 0x01);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop		

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0205, 0x10);		// STBLVDS [7:4] 1:8ch, 2:4ch 0:16ch(IMX253, 255)

#if defined(__USE_SENSOR_BAYER_10BIT__)
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x020C, 0x00);		// ADBIT [1:0] - 0: 10bit, 1: 12bit 2:8bit	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0216, 0x00);		// ODBIT [1:0] - 0: 10bit, 1: 12bit 2:8bit	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x021B, 0x00);		// FREQ [1:0] 0: normal, 1: 1/2 clock
#else
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x020C, 0x01);		// ADBIT [1:0] - 0: 10bit, 1: 12bit 2:8bit	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0216, 0x01);		// ODBIT [1:0] - 0: 10bit, 1: 12bit 2:8bit	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x021B, 0x00);		// FREQ [1:0] 0: normal, 1: 1/2 clock
#endif
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x021C, 0x10);		// OPORTSEL [7:4] 1:8ch, 3:4ch 9:16ch

#if defined(__USE_IMX253_FLIP__)
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x020E, 0x03);		// [0] Vertical (V) [1] Horizontal direction readout inversion control,  
#else
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x020E, 0x00);		// [0] Vertical (V) [1] Horizonta direction readout inversion control, 
#endif	
	
#if defined(__USE_TRIGGER_MODE__)						// Trig mode run only slave mode
	UARTprintf("\n\r IMX253 GS TRIG mode Enable\n\r");

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x020B, 0x01);		// TRIGEN [0] - 0: normal mode, 1: Trigger mode	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0226, 0x07);		// TOUT1SEL[1:0]  TOUT2SEL[3:2]  = 3 pulse output
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0229, 0x07);		// TOUT1SEL[2:0] = 1 TOUT2SEL[6:4]  = 2 pulse1,2 output	

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x026d, 0x00);		// PULSE1_EN_NOR[0] PULSE1_EN_TRIG[1] 0:disable, 1: en  PULSE1_POL[2] 0 = highactive, 1:lowactive	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0270, 0x00);		// PULSE1_UP[7:0] 	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0271, 0x00);		// PULSE1_UP[15:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0272, 0x00);		// PULSE1_UP[19:16]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0274, 0x00);		// PULSE1_DN[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0275, 0x00);		// PULSE1_DN[15:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0276, 0x00);		// PULSE1_DN[19:16]	

//	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0279, 0x00|0x80);		// PULSE2_EN_NOR[0] PULSE2_EN_TRIG[1] 0:disable, 1: en  PULSE2_POL[2] 0 = highactive, 1:lowactive	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0279, 0x00|0x08);		// PULSE2_EN_NOR[0] PULSE2_EN_TRIG[1] 0:disable, 1: en  PULSE2_POL[2] 0 = highactive, 1:lowactive	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x027C, 0x00);		// PULSE2_UP[7:0] 	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x027D, 0x00);		// PULSE2_UP[15:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x027E, 0x00);		// PULSE2_UP[19:16]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0280, 0x00);		// PULSE2_DN[7:0] 
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0281, 0x00);		// PULSE2_DN[15:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0282, 0x00);		// PULSE2_DN[19:16]		

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x02AA, 0x01);		// VINT_EN[0], 0: Vinterupt disable, 1:enable, 	Setting interupt mode in trigger mode
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x02AE, 0x00);		// LOWLAGTRG[0], 0: sequential trigger mode, 1:fast-trigger mode	Selection of trigter mode	
#else
	UARTprintf("\n\r IMX253 GS Normal mode\n\r");

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x020B, 0x00);		// TRIGEN [0] - 0: normal mode, 1: Trigger mode		
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0226, 0x00);		// TOUT1SEL[1:0]  TOUT2SEL[3:2]  = 0 pulse low fixed
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0229, 0x00);		// TOUT1SEL[2:0]  TOUT2SEL[6:4]  = 2 pulse1,2 output	

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x026d, 0x00);		// PULSE1_EN_NOR[0] PULSE1_EN_TRIG[1] 0:disable, 1: en  PULSE1_POL[2] 0 = highactive, 1:lowactive	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0270, 0x00);		// PULSE1_UP[7:0]	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0271, 0x00);		// PULSE1_UP[15:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0272, 0x00);		// PULSE1_UP[19:16]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0274, 0x00);		// PULSE1_DN[7:0] 
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0275, 0x00);		// PULSE1_DN[15:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0276, 0x00);		// PULSE1_DN[19:16]	

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0279, 0x00|0x80);		// PULSE2_EN_NOR[0] PULSE2_EN_TRIG[1] 0:disable, 1: en  PULSE2_POL[2] 0 = highactive, 1:lowactive	
//	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0279, 0x00|0x08);		// PULSE2_EN_NOR[0] PULSE2_EN_TRIG[1] 0:disable, 1: en  PULSE2_POL[2] 0 = highactive, 1:lowactive
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x027C, 0x00);		// PULSE2_UP[7:0] 	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x027D, 0x00);		// PULSE2_UP[15:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x027E, 0x00);		// PULSE2_UP[19:16]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0280, 0x00);		// PULSE2_DN[7:0] 
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0281, 0x00);		// PULSE2_DN[15:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0282, 0x00);		// PULSE2_DN[19:16]		

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x02AA, 0x01);		// VINT_EN[0], 0: Vinterupt disable, 1:enable, 	Setting interupt mode in trigger mode
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x02AE, 0x00);		// LOWLAGTRG[0], 0: sequential trigger mode, 1:fast-trigger mode	Selection of trigter mode		
#endif


	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0236, 0xC0|0x00);	// [7:6] fixed=1, SyncSel[5:4](0:Normal Operation, 3: hi-z)
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x020D, 0x00|0x00);	// WINMODE[3:0] && [4]HMODE  both   = 0 all pixel, 1 is 1/2 sub sampling mode(3Mega mode)
	
	//INCKSEL
#if defined(__USE_OSC_74_25MHZ__)
	//74.25MHz
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0289, 0x10);		// INCKSEL0
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x028A, 0x00);		// INCKSEL1
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x028B, 0x10);		// INCKSEL2
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x028C, 0x00);		// INCKSEL3
#else
	//37.125MHz
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0289, 0x10);		// INCKSEL0
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x028A, 0x02);		// INCKSEL1
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x028B, 0x10);		// INCKSEL2
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x028C, 0x02);		// INCKSEL3
#endif
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x028D, 0xAA);		// SHS[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x028E, 0x00);		// SHS[15:8]	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x028F, 0x00);		// SHS[19:16]		

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0404, 0x00);		// GAIN[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0405, 0x00);		// GAIN[8]	0.1 db stap, 0~12 (refer gaindly 0x412h)
//	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0412, 0x09);		// GAINDLY	08 : gain reflect at the frame, 09 gain reflect at the next frame. 09h : not run. 
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0412, 0x08);		// GAINDLY	08 : gain reflect at the frame, 09 gain reflect at the next frame
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0454, 0xF0);		// BLKLEVEL[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0455, 0x00);		// BLKLEVEL[11:8]
	
//	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0474, 0x00);		// VOPB_VBLK_HWIDTH[7:0]
//	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0475, 0x10);		// VOPB_VBLK_HWIDTH[12:8]
//	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0476, 0x00);		// FINFO width setting[7:0]
//	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0477, 0x08);		// FINFO width setting[12:8]
	
//	IMX253_SetROIMode();

	IMX253_SetHiddenRegister();

	IMX253_ChangeFps_Normal();

	//standby cancel
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0: Operation,  1: STANDBY

	//delay more 1ms
	Delay_mSec(5);	

	#if !(defined(__USE_TRIGGER_MODE__) ||defined(__USE_IMX253_SLAVE_MODE__) )					// Trig mode run only slave mode
	//XMASTER = 0. after 8 frame, image stable
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x020A, 0x00);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop	
	#endif

	IMX253_SensorInformation();
}

void IMX253_SetROIMode(void)
{

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0500, 0x00);		// FID0_ROIH1ON[0], FID0_ROIV1ON[1], FID0_ROIH2ON[2], FID0_ROIV2ON[3], FID0_ROIH3ON[4], FID0_ROIV3ON[5], FID0_ROIH4ON[6], FID0_ROIV4ON[7]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0501, 0x00);		// FID0_ROIH5ON[0], FID0_ROIV5ON[1], FID0_ROIH6ON[2], FID0_ROIV6ON[3], FID0_ROIH7ON[4], FID0_ROIV7ON[5], FID0_ROIH8ON[6], FID0_ROIV8ON[7]

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0510, 0x00);		// FID0_ROIPH1[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0511, 0x00);		// FID0_ROIPH1[11:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0512, 0x00);		// FID0_ROIPV1[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0513, 0x00);		// FID0_ROIPV1[11:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0514, 0x00);		// FID0_ROIWH1[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0515, 0x00);		// FID0_ROIWH1[12:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0516, 0x00);		// FID0_ROIWV1[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0517, 0x00);		// FID0_ROIWV1[11:8]

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0518, 0x00);		// FID0_ROIPH2[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0519, 0x00);		// FID0_ROIPH2[11:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x051A, 0x00);		// FID0_ROIPV2[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x051B, 0x00);		// FID0_ROIPV2[11:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x051C, 0x00);		// FID0_ROIWH2[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x051D, 0x00);		// FID0_ROIWH2[12:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x051E, 0x00);		// FID0_ROIWV2[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x051F, 0x00);		// FID0_ROIWV2[11:8]

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0520, 0x00);		// FID0_ROIPH3[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0521, 0x00);		// FID0_ROIPH3[11:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0522, 0x00);		// FID0_ROIPV3[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0523, 0x00);		// FID0_ROIPV3[11:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0524, 0x00);		// FID0_ROIWH3[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0525, 0x00);		// FID0_ROIWH3[12:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0526, 0x00);		// FID0_ROIWV3[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0527, 0x00);		// FID0_ROIWV3[11:8]

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0528, 0x00);		// FID0_ROIPH4[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0529, 0x00);		// FID0_ROIPH4[11:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x052A, 0x00);		// FID0_ROIPV4[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x052B, 0x00);		// FID0_ROIPV4[11:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x052C, 0x00);		// FID0_ROIWH4[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x052D, 0x00);		// FID0_ROIWH4[12:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x052E, 0x00);		// FID0_ROIWV4[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x052F, 0x00);		// FID0_ROIWV4[11:8]
	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0530, 0x00);		// FID0_ROIPH5[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0531, 0x00);		// FID0_ROIPH5[11:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0532, 0x00);		// FID0_ROIPV5[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0533, 0x00);		// FID0_ROIPV5[11:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0534, 0x00);		// FID0_ROIWH5[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0535, 0x00);		// FID0_ROIWH5[12:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0536, 0x00);		// FID0_ROIWV5[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0537, 0x00);		// FID0_ROIWV5[11:8]

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0538, 0x00);		// FID0_ROIPH6[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0539, 0x00);		// FID0_ROIPH6[11:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x053A, 0x00);		// FID0_ROIPV6[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x053B, 0x00);		// FID0_ROIPV6[11:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x053C, 0x00);		// FID0_ROIWH6[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x053D, 0x00);		// FID0_ROIWH6[12:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x053E, 0x00);		// FID0_ROIWV6[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x053F, 0x00);		// FID0_ROIWV6[11:8]

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0530, 0x00);		// FID0_ROIPH7[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0531, 0x00);		// FID0_ROIPH7[11:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0532, 0x00);		// FID0_ROIPV7[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0533, 0x00);		// FID0_ROIPV7[11:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0534, 0x00);		// FID0_ROIWH7[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0535, 0x00);		// FID0_ROIWH7[12:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0536, 0x00);		// FID0_ROIWV7[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0537, 0x00);		// FID0_ROIWV7[11:8]

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0538, 0x00);		// FID0_ROIPH8[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0539, 0x00);		// FID0_ROIPH8[11:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x053A, 0x00);		// FID0_ROIPV8[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x053B, 0x00);		// FID0_ROIPV8[11:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x053C, 0x00);		// FID0_ROIWH8[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x053D, 0x00);		// FID0_ROIWH8[12:8]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x053E, 0x00);		// FID0_ROIWV8[7:0]
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x053F, 0x00);		// FID0_ROIWV8[11:8]	
		

}

void IMX253_SetHiddenRegister(void)
{
#if 1
//	imx253 hidden option
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0365, 0x40);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0366, 0x00);		// register_xlsx			

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0426, 0x03);		// register_xlsx		

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0718, 0x78);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0719, 0x0C);		// register_xlsx		

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0F70, 0x1E);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0F71, 0x00);		// register_xlsx			
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0F72, 0x67);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0F73, 0x01);		// register_xlsx			
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0F74, 0x1E);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0F75, 0x00);		// register_xlsx			
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0F76, 0x67);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0F77, 0x01);		// register_xlsx			

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x1202, 0x20);		// register_xlsx
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x1217, 0x03);		// register_xlsx	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x121E, 0x03);		// register_xlsx	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x123D, 0x24);		// register_xlsx	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x1240, 0x09);		// register_xlsx	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x1241, 0x6A);		// register_xlsx	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x124A, 0xC0);		// register_xlsx	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x1294, 0x06);		// register_xlsx			
#else
//	imx267 hidden option
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0201, 0xD0);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0202, 0xAA);		// register_xlsx			
//	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x020C, 0x01);		// ADBIT	
//	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0210, 0xA2);		// VMAX			
//	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0211, 0x08);		// VMAX		
//	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0216, 0x01);		// ODBIT			
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0218, 0x01);		// register_xlsx		
//	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0279, 0x80);		// update above	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0290, 0x0A);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0294, 0x0A);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0298, 0x0A);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x029E, 0x08);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x02A0, 0x06);		// register_xlsx	

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0365, 0x40);		// register_xlsx, same 253
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0366, 0x00);		// register_xlsx, same 253

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0426, 0x03);		// register_xlsx, same 253
	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0718, 0x78);		// register_xlsx, same 253			
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x0719, 0x0C);		// register_xlsx, same 253			

	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x1202, 0x20);		// register_xlsx, same 253
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x1203, 0x55);		// register_xlsx	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x1217, 0x03);		// register_xlsx, same 253	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x121E, 0x03);		// register_xlsx, same 253	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x123D, 0x24);		// register_xlsx, same 253	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x1240, 0x09);		// register_xlsx, same 253	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x1241, 0x6A);		// register_xlsx, same 253	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x124A, 0xC0);		// register_xlsx, same 253	
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x1256, 0x18);		// register_xlsx
	CMOS_ByteWrite(SPI_IMX253_ADDR, 0x1294, 0x06);		// register_xlsx, same 253				
	
#endif	
}

BOOL IMX253_SensorInformation(void)
{
	BOOL	GraySensor;
	BYTE	temp1, temp2;
	BYTE	sensor_id;

	temp1=CMOS_ByteRead(SPI_IMX253_ADDR, 0x348);
	temp2=CMOS_ByteRead(SPI_IMX253_ADDR, 0x349);

	sensor_id=(temp2&0x7F)*4 + (temp1&0xC0)/64;
	
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

#endif	/* __USE_CMOS_IMX253__ */

/*  FILE_END_HERE */
