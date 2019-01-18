// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_IMX273__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
static DWORD vmax;
// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//-------------------------------------------------------------------------------------

//----------------------------------------------------------------------
//lvds 4ch  720p


//lvds 4ch 1456x1080p


//lvds 8ch (12bit freq = 1)


//lvds 8ch (10bit freq =0) 226.5frame/s



//----------------------------------------------------------------------

static void stIMX273_SetFps(IMX273_Fps_t fps) {
	WORD hmax;

	//vmax = 750d setting must be sensor slave mode. and vmax=750, real image output v size = 720(708 + 12OBvalue)

	#if defined(__USE_IMX273_ALLSCAN_MODE__)
		switch(fps) {
			case IMX273_FPS_30Hz:
				hmax=1650;		vmax=1500;	
				IMX273_SetAdBits(1);			
				break;
			case IMX273_FPS_60Hz:
			default:
				hmax=825;		vmax=1500;	
				IMX273_SetAdBits(1);
				break;
		}
	#elif defined(__USE_IMX273_ROI_MODE__)
		switch(fps) {
			case IMX273_FPS_30Hz:
				hmax=3000;		vmax=825;	
				IMX273_SetAdBits(1);			
				break;
			case IMX273_FPS_120Hz:
				hmax=750;		vmax=825;					
				IMX273_SetAdBits(1);			
				break;
			case IMX273_FPS_240Hz:				
				hmax=375;		vmax=825;					
				IMX273_SetAdBits(1);			
				break;				
			case IMX273_FPS_60Hz:
			default:
				hmax=1500;		vmax=825;		
				IMX273_SetAdBits(1);
				break;
		}
	#else
		switch(fps) {
			case IMX273_FPS_30Hz:
				hmax=2250;		vmax=1100;	
				IMX273_SetAdBits(1);			
				break;
			case IMX273_FPS_60Hz:
			default:
				hmax=1125;		vmax=1100;		
				IMX273_SetAdBits(1);
				break;
		}

	#endif

	//pal system
	#if defined(__VTOTAL_CTRL_PAL__)
			if(GetSS_SensorVoutPAL())  vmax*=1.2f;
	#else
			if(GetSS_SensorVoutPAL())  hmax*=1.2f;
	#endif		
	
	UARTprintf("vmax:%d / hmax:%d\r\n",vmax,hmax);

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0214, LOBYTE(hmax));	// HMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0215, HIBYTE(hmax));	// HMAX [7:0] (MSB)
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0210, LOBYTE(LOWORD(vmax)));	// VMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0211, HIBYTE(LOWORD(vmax)));	// VMAX [15:8] (MSB)
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0212, LOBYTE(HIWORD(vmax)));	// VMAX [19:16] (MSB)


	return;	
}

void IMX273_SetAdBits(BYTE bit12) {
	if(bit12) {  //ADBIT : 12bit
		CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020C, 0x01);	// ADBIT [1:0] - 0: 10bit, 1: 12bit 2:8bit	
		CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0216, 0x01);	// ODBIT [1:0] - 0: 10bit, 1: 12bit 2:8bit	
		CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0454, 0xF0);	// BLKLEVEL[7:0]
		CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0455, 0x00);	// BLKLEVEL[11:8]
	}
	else {  //ADBIT : 10bit
		CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020C, 0x00);	// ADBIT [1:0] - 0: 10bit, 1: 12bit 2:8bit	
		CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0216, 0x00);	// ODBIT [1:0] - 0: 10bit, 1: 12bit 2:8bit	
		CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0454, 0x3C);	// BLKLEVEL[7:0]
		CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0455, 0x00);	// BLKLEVEL[11:8]
	}
}

//set IMX273 Frame WDR mode
void IMX273_WDR_Mode(IMX273_Mode_t Mode)  {
	//standby
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020A, 0x01);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop		
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY	

	//set mode
	switch(Mode) {
		case IMX273_WDR:
			CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0221, 0x01);  // WDSEL[1:0]=1
			CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0412, 0x00);	// GAINDLY(normal mode)	08 : gain reflect at the frame, 09 gain reflect at the next frame
			break;
		default:
			CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0221, 0x00);  // WDSEL[1:0]=0
			CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0412, 0x08);	// GAINDLY(normal mode)	08 : gain reflect at the frame, 09 gain reflect at the next frame
			break;
	}

	//change fps
#if 1
	switch(Mode) {
		case IMX273_WDR:  //60->30Hz
			stIMX273_SetFps(IMX273_FPS_60Hz);
			break;
		default:  //30Hz
			stIMX273_SetFps(IMX273_FPS_30Hz);
			break;
	}
#endif

	//release operation
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0: Operation,  1: STANDBY	
	Delay_mSec(10);                             	    // Wait for internal regulator stabilization
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020A, 0x00);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop		
	return;
}

void IMX273_ChangeFps_Normal(BYTE nID) {
	switch(nID){
		case  0 : 	
		case  1 :	stIMX273_SetFps(IMX273_FPS_30Hz);	break;
		case  2 :
		case  3 :	stIMX273_SetFps(IMX273_FPS_60Hz);	break;
		case  4 : 	
		case  5 : stIMX273_SetFps(IMX273_FPS_120Hz);	break;
		case  6 : 
		case  7 : stIMX273_SetFps(IMX273_FPS_240Hz);	break;		
		default : break;
	}
}

DWORD CMOS_GetVmax(void)
{
#if defined(__USE_IMX273_SLAVE_MODE__)
	return GetSS_SensorSlave_Vsize();
#else
	return vmax;
#endif
}


void IMX273_Initialize(void)
{	
#if defined(__USE_IMX273_SLAVE_MODE__)
	UARTprintf("\n\r IMX273 Slave Mode Initialize\n\r"); 		//XMASTER PIN OVdd High
#else
	UARTprintf("\n\r IMX273 Master Initialize\n\r");			//XMASTER PIN GND Low
#endif
	UARTprintf("\n\r [All pixel scan mode 12bit 1440x1080 lvds 8ch mode, osc37.125Mhz] \n\r");

	//reg init setting
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020A, 0x01);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop		

#if defined(__USE_TRIGGER_MODE__) && defined(__USE_IMX273_ALLSCAN_MODE__)
	IMX273_ChangeTriggerMode(3);		//3   //fast trigger mode only support all scan mode
#elif defined(__USE_TRIGGER_MODE__) && !defined(__USE_IMX273_ALLSCAN_MODE__)
	IMX273_ChangeTriggerMode(1);		//3   //sequencial trigger mode
#else
	IMX273_ChangeTriggerMode(0);
#endif


#if defined(__USE_CMOS_IMX273_LVDS_S8__)
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0205, 0x00|0x01);	// STBLVDS [7:4] 2:4ch, 3:2ch 0:8ch(IMX273), [0] = fixed 1
#elif defined(__USE_CMOS_IMX273_LVDS_S2__)
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0205, 0x30|0x01);	// STBLVDS [7:4] 2:4ch, 3:2ch 0:8ch(IMX273), [0] = fixed 1
#else
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0205, 0x20|0x01);	// STBLVDS [7:4] 2:4ch, 3:2ch 0:8ch(IMX273), [0] = fixed 1
#endif

#if defined(__USE_SENSOR_BAYER_10BIT__)
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020C, 0x00);		// ADBIT [1:0] - 0: 10bit, 1: 12bit 2:8bit	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0216, 0x00);		// ODBIT [1:0] - 0: 10bit, 1: 12bit 2:8bit	

#else
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020C, 0x01);		// ADBIT [1:0] - 0: 10bit, 1: 12bit 2:8bit	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0216, 0x01);		// ODBIT [1:0] - 0: 10bit, 1: 12bit 2:8bit	
#endif

	//for 240hz mode freq must set to 0
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x021B, 0x00);		// FREQ [1:0] 0: normal, 1: 1/2 clock. lvds data rate half.

#if defined(__USE_CMOS_IMX273_LVDS_S8__)
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x021C, 0x10);		// OPORTSEL [7:4] 1:8ch, 3:4ch 9:16ch
#elif defined(__USE_CMOS_IMX273_LVDS_S2__)
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x021C, 0x40);		// OPORTSEL [7:4] 1:8ch, 3:4ch 4:2ch 9:16ch
#else
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x021C, 0x30);		// OPORTSEL [7:4] 1:8ch, 3:4ch 9:16ch
#endif

#if defined(__USE_IMX273_FLIP__)
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020E, 0x03);		// [0] Vertical (V) [1] Horizontal direction readout inversion control,  
#else
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020E, 0x00);		// [0] Vertical (V) [1] Horizonta direction readout inversion control, 
#endif	
	



#if defined(__USE_IMX273_480P60_MODE__) || defined(__USE_IMX273_480P30_MODE__)
	#if defined(__USE_IMX273_ROI_MODE__)
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020D, 0x00|0x00);	// WINMODE[3:0] && [4]HMODE  both   = 0 all pixel, 1 is 1/2 sub sampling mode(3Mega mode)
	#else
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020D, 0x10|0x01);	// WINMODE[3:0] && [4]HMODE  both   = 0 all pixel, 1 is 1/2 sub sampling mode(3Mega mode)
	#endif
#else
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020D, 0x00|0x00);	// WINMODE[3:0] && [4]HMODE  both   = 0 all pixel, 1 is 1/2 sub sampling mode(3Mega mode)
#endif
	
	//INCKSEL
#if defined(__USE_OSC_74_25MHZ__)
	//74.25MHz
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0289, 0x80);		// INCKSEL0
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x028A, 0x0F);		// INCKSEL1
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x028B, 0x80);		// INCKSEL2
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x028C, 0x0C);		// INCKSEL3
#else
	//37.125MHz
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0289, 0x80);		// INCKSEL0
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x028A, 0x0B);		// INCKSEL1
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x028B, 0x80);		// INCKSEL2
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x028C, 0x08);		// INCKSEL3
#endif
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x028D, 0x9A);		// SHS[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x028E, 0x00);		// SHS[15:8]	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x028F, 0x00);		// SHS[19:16]		

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0404, 0x00);		// GAIN[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0405, 0x00);		// GAIN[8]	0.1 db stap, 0~12 (refer gaindly 0x412h)
//	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0412, 0x09);		// GAINDLY	08 : gain reflect at the frame, 09 gain reflect at the next frame. 09h : not run. 
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0412, 0x08);		// GAINDLY	08 : gain reflect at the frame, 09 gain reflect at the next frame

#if defined(__USE_SENSOR_BAYER_10BIT__)
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0454, 0x3C);		// BLKLEVEL[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0455, 0x00);		// BLKLEVEL[11:8]
#else
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0454, 0xF0);		// BLKLEVEL[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0455, 0x00);		// BLKLEVEL[11:8]
#endif

//	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0474, 0xB0);		// VOPB_VBLK_HWIDTH[7:0]
//	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0475, 0x50);		// VOPB_VBLK_HWIDTH[12:8]
//	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0476, 0x00);		// FINFO width setting[7:0]
//	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0477, 0x05);		// FINFO width setting[12:8]

#if defined(__USE_IMX273_ROI_MODE__)	
	IMX273_SetROIMode();
#endif

	IMX273_SetHiddenRegister();
#if defined(__USE_IMX273_720P240_MODE__)
	stIMX273_SetFps(IMX273_FPS_240Hz);
#elif defined(__USE_IMX273_720P120_MODE__)
	stIMX273_SetFps(IMX273_FPS_120Hz);
#elif defined(__USE_IMX273_720P30_MODE__)
	stIMX273_SetFps(IMX273_FPS_30Hz);
#else
	stIMX273_SetFps(IMX273_FPS_60Hz);
#endif

	//standby cancel
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0: Operation,  1: STANDBY

	//delay more 1ms
	Delay_mSec(5);	

	#if !defined(__USE_IMX273_SLAVE_MODE__)					// Trig mode run only slave mode
	//XMASTER = 0. after 8 frame, image stable
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020A, 0x00);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop	
	#endif

	Delay_mSec(5);	
	
//	IMX273_SensorInformation();
}

void IMX273_SetROIMode(void)
{
#if defined(__USE_IMX273_ROI_MODE__)
	#if defined(__USE_IMX273_480P60_MODE__) || defined(__USE_IMX273_480P30_MODE__)
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0500, 0x03);		// FID0_ROIH1ON[0], FID0_ROIV1ON[1], FID0_ROIH2ON[2], FID0_ROIV2ON[3], FID0_ROIH3ON[4], FID0_ROIV3ON[5], FID0_ROIH4ON[6], FID0_ROIV4ON[7]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0501, 0x00);		// FID0_ROIH5ON[0], FID0_ROIV5ON[1], FID0_ROIH6ON[2], FID0_ROIV6ON[3], FID0_ROIH7ON[4], FID0_ROIV7ON[5], FID0_ROIH8ON[6], FID0_ROIV8ON[7]

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0510, 0x58);		// FID0_ROIPH1[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0511, 0x01);		// FID0_ROIPH1[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0512, 0x80);		// FID0_ROIPV1[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0513, 0x00);		// FID0_ROIPV1[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0514, 0x00);		// FID0_ROIWH1[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0515, 0x03);		// FID0_ROIWH1[12:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0516, 0x00);		// FID0_ROIWV1[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0517, 0x02);		// FID0_ROIWV1[11:8]
	#else
//	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0500, 0x03);
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0500, 0x02);		// FID0_ROIH1ON[0], FID0_ROIV1ON[1], FID0_ROIH2ON[2], FID0_ROIV2ON[3], FID0_ROIH3ON[4], FID0_ROIV3ON[5], FID0_ROIH4ON[6], FID0_ROIV4ON[7]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0501, 0x00);		// FID0_ROIH5ON[0], FID0_ROIV5ON[1], FID0_ROIH6ON[2], FID0_ROIV6ON[3], FID0_ROIH7ON[4], FID0_ROIV7ON[5], FID0_ROIH8ON[6], FID0_ROIV8ON[7]
	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0510, 0x00);		// FID0_ROIPH1[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0511, 0x00);		// FID0_ROIPH1[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0512, 0x80);		// FID0_ROIPV1[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0513, 0x00);		// FID0_ROIPV1[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0514, 0x78);		// FID0_ROIWH1[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0515, 0x05);		// FID0_ROIWH1[12:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0516, 0xD0);		// FID0_ROIWV1[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0517, 0x03);		// FID0_ROIWV1[11:8]
	#endif
#else
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0510, 0x00);		// FID0_ROIPH1[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0511, 0x00);		// FID0_ROIPH1[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0512, 0x00);		// FID0_ROIPV1[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0513, 0x00);		// FID0_ROIPV1[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0514, 0x00);		// FID0_ROIWH1[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0515, 0x00);		// FID0_ROIWH1[12:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0516, 0x00);		// FID0_ROIWV1[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0517, 0x00);		// FID0_ROIWV1[11:8]
#endif

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0518, 0x00);		// FID0_ROIPH2[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0519, 0x00);		// FID0_ROIPH2[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x051A, 0x00);		// FID0_ROIPV2[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x051B, 0x00);		// FID0_ROIPV2[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x051C, 0x00);		// FID0_ROIWH2[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x051D, 0x00);		// FID0_ROIWH2[12:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x051E, 0x00);		// FID0_ROIWV2[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x051F, 0x00);		// FID0_ROIWV2[11:8]

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0520, 0x00);		// FID0_ROIPH3[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0521, 0x00);		// FID0_ROIPH3[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0522, 0x00);		// FID0_ROIPV3[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0523, 0x00);		// FID0_ROIPV3[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0524, 0x00);		// FID0_ROIWH3[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0525, 0x00);		// FID0_ROIWH3[12:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0526, 0x00);		// FID0_ROIWV3[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0527, 0x00);		// FID0_ROIWV3[11:8]

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0528, 0x00);		// FID0_ROIPH4[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0529, 0x00);		// FID0_ROIPH4[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x052A, 0x00);		// FID0_ROIPV4[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x052B, 0x00);		// FID0_ROIPV4[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x052C, 0x00);		// FID0_ROIWH4[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x052D, 0x00);		// FID0_ROIWH4[12:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x052E, 0x00);		// FID0_ROIWV4[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x052F, 0x00);		// FID0_ROIWV4[11:8]
	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0530, 0x00);		// FID0_ROIPH5[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0531, 0x00);		// FID0_ROIPH5[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0532, 0x00);		// FID0_ROIPV5[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0533, 0x00);		// FID0_ROIPV5[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0534, 0x00);		// FID0_ROIWH5[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0535, 0x00);		// FID0_ROIWH5[12:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0536, 0x00);		// FID0_ROIWV5[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0537, 0x00);		// FID0_ROIWV5[11:8]

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0538, 0x00);		// FID0_ROIPH6[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0539, 0x00);		// FID0_ROIPH6[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x053A, 0x00);		// FID0_ROIPV6[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x053B, 0x00);		// FID0_ROIPV6[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x053C, 0x00);		// FID0_ROIWH6[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x053D, 0x00);		// FID0_ROIWH6[12:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x053E, 0x00);		// FID0_ROIWV6[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x053F, 0x00);		// FID0_ROIWV6[11:8]

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0530, 0x00);		// FID0_ROIPH7[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0531, 0x00);		// FID0_ROIPH7[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0532, 0x00);		// FID0_ROIPV7[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0533, 0x00);		// FID0_ROIPV7[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0534, 0x00);		// FID0_ROIWH7[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0535, 0x00);		// FID0_ROIWH7[12:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0536, 0x00);		// FID0_ROIWV7[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0537, 0x00);		// FID0_ROIWV7[11:8]

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0538, 0x00);		// FID0_ROIPH8[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0539, 0x00);		// FID0_ROIPH8[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x053A, 0x00);		// FID0_ROIPV8[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x053B, 0x00);		// FID0_ROIPV8[11:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x053C, 0x00);		// FID0_ROIWH8[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x053D, 0x00);		// FID0_ROIWH8[12:8]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x053E, 0x00);		// FID0_ROIWV8[7:0]
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x053F, 0x00);		// FID0_ROIWV8[11:8]	
		

}

void IMX273_SetHiddenRegister(void)
{
#if 1
//	imx273 hidden option
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x02AF, 0x0D);		// register_xlsx		

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x03C8, 0xF3);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x03D0, 0xF4);		// register_xlsx			

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x041A, 0x00);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0426, 0x02);		// register_xlsx		

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0718, 0x78);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0719, 0x0C);		// register_xlsx		

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0958, 0xA3);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0959, 0x00);		// register_xlsx			
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x095A, 0x85);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x095B, 0x00);		// register_xlsx	

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0A32, 0xF5);		// register_xlsx	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0A33, 0x00);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0AA2, 0xF6);		// register_xlsx	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0AA3, 0x00);		// register_xlsx		

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0C00, 0x80);		// register_xlsx	

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0F48, 0xA3);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0F49, 0x00);		// register_xlsx			
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0F4A, 0x85);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0F4B, 0x00);		// register_xlsx			

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x1214, 0x1C);		// register_xlsx	

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x12C1, 0xF6);		// register_xlsx	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x12C7, 0x0F);		// register_xlsx	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x12C8, 0x00);		// register_xlsx	
	
#else
//	imx267 hidden option
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0201, 0xD0);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0202, 0xAA);		// register_xlsx			
//	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020C, 0x01);		// ADBIT	
//	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0210, 0xA2);		// VMAX			
//	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0211, 0x08);		// VMAX		
//	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0216, 0x01);		// ODBIT			
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0218, 0x01);		// register_xlsx		
//	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0279, 0x80);		// update above	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0290, 0x0A);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0294, 0x0A);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0298, 0x0A);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x029E, 0x08);		// register_xlsx		
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x02A0, 0x06);		// register_xlsx	

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0365, 0x40);		// register_xlsx, same 273
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0366, 0x00);		// register_xlsx, same 273

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0426, 0x03);		// register_xlsx, same 273
	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0718, 0x78);		// register_xlsx, same 273			
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0719, 0x0C);		// register_xlsx, same 273			

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x1202, 0x20);		// register_xlsx, same 273
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x1203, 0x55);		// register_xlsx	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x1217, 0x03);		// register_xlsx, same 273	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x121E, 0x03);		// register_xlsx, same 273	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x123D, 0x24);		// register_xlsx, same 273	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x1240, 0x09);		// register_xlsx, same 273	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x1241, 0x6A);		// register_xlsx, same 273	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x124A, 0xC0);		// register_xlsx, same 273	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x1256, 0x18);		// register_xlsx
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x1294, 0x06);		// register_xlsx, same 273				
	
#endif	
}

void IMX273_ChangeTriggerMode(WORD nID) 
{
	if(nID==3){			//fast trigger mode
	//support only all scanmode	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020B, 0x01);		// TRIGEN [0] - 0: normal mode, 1: Trigger mode	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0226, 0x0F);		// TOUT1SEL[1:0]  TOUT2SEL[3:2]  = 3 pulse output
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0229, 0x21);		// TOUT1SEL[2:0] = 1 TOUT2SEL[6:4]  = 2 pulse1,2 output	

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x026d, 0x00);		// PULSE1_EN_NOR[0] PULSE1_EN_TRIG[1] 0:disable, 1: en  PULSE1_POL[2] 0 = highactive, 1:lowactive	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0270, 0x00);		// PULSE1_UP	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0271, 0x00);		// PULSE1_UP
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0272, 0x00);		// PULSE1_UP
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0274, 0x00);		// PULSE1_DN
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0275, 0x00);		// PULSE1_DN
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0276, 0x00);		// PULSE1_DN	

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0279, 0x00|0x08);		// PULSE2_EN_NOR[0] PULSE2_EN_TRIG[1] 0:disable, 1: en  PULSE2_POL[2] 0 = highactive, 1:lowactive	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x027C, 0x02);		// PULSE2_UP	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x027D, 0x00);		// PULSE2_UP
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x027E, 0x00);		// PULSE2_UP
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0280, 0x08);		// PULSE2_DN
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0281, 0x4E);		// PULSE2_DN
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0282, 0x00);		// PULSE2_DN		

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x02AA, 0x01);		// VINT_EN[0], 0: Vinterupt disable, 1:enable, 	Setting interupt mode in trigger mode
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x02AE, 0x01);		// LOWLAGTRG[0], 0: sequential trigger mode, 1:fast-trigger mode	Selection of trigter mode	

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0236, 0xF0|0x00);	// [7:6] fixed=1, SyncSel[5:4](0:Normal Operation, 3: hi-z)	

	}
	else if(nID==2){		//sequential trigger mode and VIENT off
	//support all scanmode, ROI mode
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020B, 0x01);		// TRIGEN [0] - 0: normal mode, 1: Trigger mode	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0226, 0x0F);		// TOUT1SEL[1:0]  TOUT2SEL[3:2]  = 3 pulse output
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0229, 0x21);		// TOUT1SEL[2:0] = 1 TOUT2SEL[6:4]  = 2 pulse1,2 output	

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x026d, 0x03);		// PULSE1_EN_NOR[0] PULSE1_EN_TRIG[1] 0:disable, 1: en  PULSE1_POL[2] 0 = highactive, 1:lowactive	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0270, 0x00);		// PULSE1_UP	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0271, 0x00);		// PULSE1_UP
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0272, 0x00);		// PULSE1_UP
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0274, 0x00);		// PULSE1_DN
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0275, 0x00);		// PULSE1_DN
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0276, 0x00);		// PULSE1_DN	

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0279, 0x03|0x08);		// PULSE2_EN_NOR[0] PULSE2_EN_TRIG[1] 0:disable, 1: en  PULSE2_POL[2] 0 = highactive, 1:lowactive	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x027C, 0x02);		// PULSE2_UP	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x027D, 0x00);		// PULSE2_UP
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x027E, 0x00);		// PULSE2_UP
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0280, 0x08);		// PULSE2_DN
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0281, 0x4E);		// PULSE2_DN
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0282, 0x00);		// PULSE2_DN		

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x02AA, 0x00);		// VINT_EN[0], 0: Vinterupt disable, 1:enable, 	Setting interupt mode in trigger mode
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x02AE, 0x00);		// LOWLAGTRG[0], 0: sequential trigger mode, 1:fast-trigger mode	Selection of trigter mode		

#if !defined(__USE_IMX273_SLAVE_MODE__)	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0236, 0xF0|0x00);	// [7:6] fixed=1, SyncSel[5:4](0:Normal Operation, 3: hi-z)	
#else
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0236, 0xE0|0x00);	// [7:6] fixed=1, SyncSel[5:4](0:Normal Operation, 3: hi-z)	
#endif	
	}
	else if(nID==1){		//sequential trigger mode VIENT on
	//all mode
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020B, 0x01);		// TRIGEN [0] - 0: normal mode, 1: Trigger mode	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0226, 0x0F);		// TOUT1SEL[1:0]  TOUT2SEL[3:2]  = 3 pulse output
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0229, 0x21);		// TOUT1SEL[2:0] = 1 TOUT2SEL[6:4]  = 2 pulse1,2 output	

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x026d, 0x03);		// PULSE1_EN_NOR[0] PULSE1_EN_TRIG[1] 0:disable, 1: en  PULSE1_POL[2] 0 = highactive, 1:lowactive	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0270, 0x00);		// PULSE1_UP	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0271, 0x00);		// PULSE1_UP
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0272, 0x00);		// PULSE1_UP
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0274, 0x00);		// PULSE1_DN
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0275, 0x00);		// PULSE1_DN
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0276, 0x00);		// PULSE1_DN	

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0279, 0x03|0x08);		// PULSE2_EN_NOR[0] PULSE2_EN_TRIG[1] 0:disable, 1: en  PULSE2_POL[2] 0 = highactive, 1:lowactive	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x027C, 0x02);		// PULSE2_UP	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x027D, 0x00);		// PULSE2_UP
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x027E, 0x00);		// PULSE2_UP
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0280, 0x08);		// PULSE2_DN
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0281, 0x4E);		// PULSE2_DN
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0282, 0x00);		// PULSE2_DN		

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x02AA, 0x01);		// VINT_EN[0], 0: Vinterupt disable, 1:enable, 	Setting interupt mode in trigger mode
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x02AE, 0x00);		// LOWLAGTRG[0], 0: sequential trigger mode, 1:fast-trigger mode	Selection of trigter mode	

#if !defined(__USE_IMX273_SLAVE_MODE__)	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0236, 0xF0|0x00);	// [7:6] fixed=1, SyncSel[5:4](0:Normal Operation, 3: hi-z)	
#else
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0236, 0xE0|0x00);	// [7:6] fixed=1, SyncSel[5:4](0:Normal Operation, 3: hi-z)	
#endif
	}
	else{		//normal mode(slave)
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x020B, 0x00);		// TRIGEN [0] - 0: normal mode, 1: Trigger mode		
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0226, 0x00);		// TOUT1SEL[1:0]  TOUT2SEL[3:2]  = 0 pulse low fixed
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0229, 0x00);		// TOUT1SEL[2:0]  TOUT2SEL[6:4]  = 2 pulse1,2 output	
	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x026d, 0x00);		// PULSE1_EN_NOR[0] PULSE1_EN_TRIG[1] 0:disable, 1: en  PULSE1_POL[2] 0 = highactive, 1:lowactive	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0270, 0x00);		// PULSE1_UP	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0271, 0x00);		// PULSE1_UP
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0272, 0x00);		// PULSE1_UP
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0274, 0x00);		// PULSE1_DN
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0275, 0x00);		// PULSE1_DN
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0276, 0x00);		// PULSE1_DN	
	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0279, 0x00|0x08);		// PULSE2_EN_NOR[0] PULSE2_EN_TRIG[1] 0:disable, 1: en  PULSE2_POL[2] 0 = highactive, 1:lowactive	[3] fixed 1
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x027C, 0x00);		// PULSE2_UP	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x027D, 0x00);		// PULSE2_UP
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x027E, 0x00);		// PULSE2_UP
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0280, 0x00);		// PULSE2_DN
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0281, 0x00);		// PULSE2_DN
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0282, 0x00);		// PULSE2_DN	

	
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x02AA, 0x01);		// VINT_EN[0], 0: Vinterupt disable, 1:enable, 	Setting interupt mode in trigger mode
	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x02AE, 0x00);		// LOWLAGTRG[0], 0: sequential trigger mode, 1:fast-trigger mode	Selection of trigter mode		

	CMOS_ByteWrite(SPI_IMX273_ADDR, 0x0236, 0xC0|0x00);	// [7:6] fixed=1, SyncSel[5:4](0:Normal Operation, 3: hi-z)		
	}
}

BOOL IMX273_SensorInformation(void)
{
	BOOL	GraySensor;
	BYTE	temp1, temp2;
	BYTE	sensor_id;

	temp1=CMOS_ByteRead(SPI_IMX273_ADDR, 0x348);
	temp2=CMOS_ByteRead(SPI_IMX273_ADDR, 0x349);

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

#endif	/* __USE_CMOS_IMX273__ */

/*  FILE_END_HERE */
