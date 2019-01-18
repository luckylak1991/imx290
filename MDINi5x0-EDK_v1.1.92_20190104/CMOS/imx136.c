// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_IMXx36__)
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

void IMX136_Initialize(void)
{	
#if defined(__USE_CMOS_IMX136__) 	
	UARTprintf("IMX136_Initialize\n\r");
#endif
#if defined(__USE_CMOS_IMX236__) 	
	UARTprintf("IMX236_Initialize\n\r");
#endif	
#if defined(__USE_CMOS_IMXx36_720pReadOut__)
	UARTprintf("IMXx36_720pReadOutMode\n\r");
#endif
//////////////////		Chip ID = 02h			//////////////////////	
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY
//	CMOS_ByteRead(SPI_IMX136_ADDR, 0x0200);
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0202, 0x01);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop

	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0205, 0x01);		//12bit // ADBIT [0] - 0: 10bit, 1: 12bit, STD12EN [4] - (when ADBIT = 0) 0: No bit shift, 1: bit shift leftward (AD 12 bit normalization)
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0205, 0x00);		//10bit // ADBIT [0] - 0: 10bit, 1: 12bit, STD12EN [4] - (when ADBIT = 0) 0: No bit shift, 1: bit shift leftward (AD 12 bit normalization)

	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0206, 0x00);		//All-pix scan // MODE [5:0] - 00h-All-pix scan mode,  22h: 2¡¿2 binning,  03h: Vertical 1/2 subsampling,  33h: Horizontal/Vertical 1/2 subsampling

#if defined(__USE_IMXx36_FLIP__)
	#if defined(__USE_CMOS_IMXx36_720pReadOut__)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0207, 0x23);		//1080p // VREVERSE [0], HREVERSE [1] - 0: normal, 1: inverted,  WINMODE [7:4] - 0: All pixel(WUXGA),  1: 1080p,  2: 720p,  4: window crop
	#else
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0207, 0x13);		//1080p // VREVERSE [0], HREVERSE [1] - 0: normal, 1: inverted,  WINMODE [7:4] - 0: All pixel(WUXGA),  1: 1080p,  2: 720p,  4: window crop
	#endif
#else	
	#if defined(__USE_CMOS_IMXx36_720pReadOut__)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0207, 0x20);
	#else	
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0207, 0x10);
	#endif	
#endif

	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x020A, 0xF0);		// BLKLEVEL[7:0] - Black level (LSB)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x020B, 0x00);		// BLKLEVEL[0] - Black level (MSB)

	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0214, 0x3C);		// GAIN [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0215, 0x00);		// GAIN [0] (MSB)

	// 1920x1080 30fps - VMAX: 465h, HMAX: 898h
	IMX136_ChangeFps_Normal();  //30fps

	// Shutter
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0220, 0x00);		// SHS1 [7:0]	(LSB)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0221, 0x00);		// SHS1 [7:0]
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0222, 0x00);		// SHS1 [0]	(MSB)

	// Crop mode register
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0238, 0x3C);		// WINPV [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0239, 0x00);		// WINPV [3:0] (MSB)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x023A, 0x50);		// WINWV [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x023B, 0x04);		// WINWV [3:0] (MSB)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x023C, 0x00);		// WINPH [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x023D, 0x00);		// WINPH [2:0] (MSB)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x023E, 0x9C);		// WINWH [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x023F, 0x07);		// WINWH [2:0] (MSB)

//-------------------------------------------------------------------------------------
	// ODBIT [0] - output data bit, 0: 10bit,  1: 12bit   
	// OPORTSEL [7:4] - 0h: CMOS SDR,  6h: Parallel LVDS DDR, Dh: Serial LVDS 2ch,  Eh: Serial LVDS 4ch

#if defined(__USE_CMOS_IMXx36_TTL__)		// use sony cmos
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0244, 0x01);		// 12bit, Parallel CMOS output
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0244, 0x00);		// 10bit, Parallel CMOS output
#endif
#if defined(__USE_CMOS_IMXx36_LVDS_P__)		// use sony cmos
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0244, 0x61);		// 12bit, LVDS Parallel output
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0244, 0x60);		// 10bit, LVDS Parallel output
#endif
#if defined(__USE_CMOS_IMXx36_LVDS_S2__)		// use sony cmos
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0244, 0xD1);		// 12bit, LVDS Serial LVDS 2ch output
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0244, 0xD0);		// 10bit, LVDS Serial LVDS 2ch output
#endif
#if defined(__USE_CMOS_IMXx36_LVDS_S4__)		// use sony cmos
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0244, 0xE1);		// 12bit, LVDS Serial LVDS 4ch output
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0244, 0xE0);		// 10bit, LVDS Serial LVDS 4ch output	
#endif
//-------------------------------------------------------------------------------------

	//CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0246, 0x00);		// XVSLNG [5:4]
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0246, 0x30);		// XVSLNG [5:4] XVS pulse width setting 0: 1H, 1: 2H, 2: 4H, 3: 8H

/*
#if defined(__USE_CMOS_IMX136_TTL__) || defined(__USE_CMOS_IMX136_LVDS_P__)		// use sony cmos
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0247, 0x08);		// XHSLNG [5:4]
#endif	
#if defined(__USE_CMOS_IMX136_LVDS_S2__) || defined(__USE_CMOS_IMX136_LVDS_S4__)		// use sony cmos
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0247, 0x08);		// XHSLNG [5:4]
#endif
*/

	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0247, 0x08);		// XHSLNG [5:4]
	
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0249, 0x0A);		// XVSOUTSEL[1:0] - 0: Set XVS High,  2:  VSYNC out,    XHSOUTSEL[3:2] - 0: Set XHS High,  2:  HSYNC out

	// 0x4A ~ 0x53 : DO NOT WRITE!!
	// 0x55 ~ 0x5A : DO NOT WRITE!!

	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0254, 0x63);             // Fixed

	// INCK setting register
#if defined(__USE_CMOS_IMXx36_TTL__) || defined(__USE_CMOS_IMXx36_LVDS_P__)		// use sony cmos
	// When [37.125Mhz input, 1080p mode, Parallel CMOS/LVDS]		--(Refer datasheet!!!)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x025B, 0x01);		// INCKSEL0 
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x025C, 0x20);		// INCKSEL1
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x025D, 0x06);		// INCKSEL2
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x025E, 0x30);		// INCKSEL3
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x025F, 0x04);		// INCKSEL4
#endif

#if defined(__USE_CMOS_IMXx36_LVDS_S2__) || defined(__USE_CMOS_IMXx36_LVDS_S4__)		// use sony cmos
	// When [37.125Mhz input, 1080p mode, Serial LVDS 2/4ch]	--(Refer datasheet!!!)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x025B, 0x00);		// INCKSEL0 
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x025C, 0x30);		// INCKSEL1
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x025D, 0x04);		// INCKSEL2
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x025E, 0x30);		// INCKSEL3
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x025F, 0x04);		// INCKSEL4
#endif

	// When [74.25Mhz input, 1080p mode, Serial LVDS 2/4ch]	--(Refer datasheet!!!)
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x025B, 0x00);		// INCKSEL0 
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x025C, 0x30);		// INCKSEL1
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x025D, 0x08);		// INCKSEL2
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x025E, 0x30);		// INCKSEL3
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x025F, 0x08);		// INCKSEL4
	
	// 0x60 ~ 0xFE : DO NOT WRITE!!




//////////////////		Chip ID = 03h			//////////////////////
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x030F, 0x0E);		// fixed
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0316, 0x02);		// fixed

	// 0xC5 ~ 0xD4 : Never Communicate!!!


//////////////////		Chip ID = 04h			//////////////////////	
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0436, 0x71);
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0439, 0xF1);
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0441, 0xF2);
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0442, 0x21);
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0443, 0x21);
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0448, 0xF2);
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0449, 0x21);
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x044A, 0x21);
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0452, 0x01);
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0454, 0xB1);
	
	//set registers about Built-in WDR
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0256, 0xc9);		//WDC_CCMP	
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0257, 0x64);		//WDC_ACMP															

	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0284, 0x00);		//VRSET		
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0 : Normal operation

	Delay_mSec(100);

//	CMOS_ByteRead(SPI_IMX136_ADDR, 0x0200);
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0202, 0x00);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop

}

#if 0
void IMX136_Builtin_WDRmode(void)			//bulit in wdr 16bit output
{
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x020C, 0x02);	// WDM MODE[1:0]
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x020F, 0x01);	//'00000101' compressed out enable and [0] is fixed '1'
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0210, 0x00);		
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0212, 0x2d);		
	//shs1,2 set
//	CMOS_SetShutter(5);
//	CMOS_SHS2(1055);

//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0244, 0x02);	//'12bit mode is [1:0] = '01', 	'16bit mode is	[1:0] = '10'     
														//[7:4] = 0 CMOS SDR	 6h is LVDS DDR, Dh LVDS 2ch DDR  Eh LVDS 4ch DDR output
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0256, 0xc9);	//WDC_CCMP	
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0257, 0x64);	//WDC_ACMP															
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0265, 0x00);		
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0284, 0x10);	//VRSET		
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0286, 0x10);				
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x02CF, 0xE1);	
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x02D0, 0x30);	
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x02D2, 0xC4);	
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x02D3, 0x01);	
}
#endif

#if 0
void IMX136_Builtin_WDRmode_comp(void)	//bulit in wdr 12bit compressed output
{
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x020C, 0x02);	// WDM MODE[1:0]
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x020F, 0x05);	//'00000101' compressed out enable and [0] is fixed '1'
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0210, 0x00);		
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0212, 0x2d);		
	//shs1,2 set
	CMOS_SetShutter(5);
	CMOS_SHS2(1055);

//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0244, 0xD1);	//'12bit mode is [1:0] = '01', 	'16bit mode is	[1:0] = '10'     if 16bit mode use, check this line
														//[7:4] = 0 CMOS SDR	 6h is LVDS DDR, Dh LVDS 2ch DDR  Eh LVDS 4ch DDR output
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0256, 0xc9);	//WDC_CCMP	
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0257, 0x64);	//WDC_ACMP															
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0265, 0x00);		
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0284, 0x10);	//VRSET		
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0286, 0x10);				
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x02CF, 0xE1);	
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x02D0, 0x30);	
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x02D2, 0xC4);	
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x02D3, 0x01);	
	

}
#endif

#if 0
void IMX136_Nomal_Mode_return(void)
{
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x01);	// STANDBY[0] - 0: Operation,  1: STANDBY
//	Delay_mSec(100);
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0202, 0x01);	// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop

	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x020C, 0x00);	// WDM MODE[1:0] (normal mode)

	//revert fps
#if defined (__USE_IMXx36_60P_MODE__)  //return to 60fps
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0209, 0x00);		// FRSEL [1:0] - 0: 60fps,  1: 30 fps
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x021B, 0x4C);		// HMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x021C, 0x04);		// HMAX [7:0] (MSB)
#endif

	//reset compress
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0205, 0x00);	// ADBIT(0:10bit, 1:12bit)

	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x020F, 0x01);	//'00000101' compressed out enable and [0] is fixed '1'
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0210, 0x01);		
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0212, 0x00);		

//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0202, 0x00);	// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
//	Delay_mSec(100);
//	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x00);	// STANDBY[0] - 0 : Normal operation
}
#endif

static void stIMX136_SetFps(IMX136_Fps_t fps) {
#if defined(__USE_CMOS_IMXx36_720pReadOut__)
	WORD vmax=0x02ee;
	WORD hmax=0x0672;

	switch(fps) {
		case IMX136_FPS_30Hz:
			CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0209, 0x02);		// FRSEL [1:0] - 0:120fps 1: 60fps,  2: 30 fps
			hmax=0x0CE4;  //3300
			break;
		case IMX136_FPS_60Hz:
		default:
			CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0209, 0x01);		// FRSEL [1:0] - 0:120fps 1: 60fps,  2: 30 fps
			hmax=0x0672;  //1650
			break;
	}

#else
	WORD vmax=0x0465;
	WORD hmax=0x0898;
	switch(fps) {
		case IMX136_FPS_15Hz:
			CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0209, 0x02);		// FRSEL [1:0] - 0: 60fps,  1: 30 fps
			hmax=0x1130;  //4400
			break;
		case IMX136_FPS_30Hz:
			CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0209, 0x01);		// FRSEL [1:0] - 0: 60fps,  1: 30 fps
			hmax=0x0898;  //2200
			break;
		case IMX136_FPS_60Hz:
		default:
			CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0209, 0x00);		// FRSEL [1:0] - 0: 60fps,  1: 30 fps
			hmax=0x044C;  //1100
			break;
	}
#endif

#if defined(__VTOTAL_CTRL_PAL__)
	if(GetSS_SensorVoutPAL())  vmax*=1.2f;
#else
	if(GetSS_SensorVoutPAL())  hmax*=1.2f;
#endif
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0218, LOBYTE(vmax));	// VMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0219, HIBYTE(vmax));	// VMAX [7:0] 
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x021A, 0x00);			// VMAX[0] (MSB)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x021B, LOBYTE(hmax));	// HMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x021C, HIBYTE(hmax));	// HMAX [7:0] (MSB)

	return;	
}

void IMX136_WDR_Mode(IMX136_Mode_t Mode)
{
	static IMX136_Mode_t pre_mode=IMX136_NORMAL;

	BOOL pass_stdby=FALSE;
	if(	((pre_mode!=IMX136_HTM)&&(Mode==IMX136_HTM))||
		((pre_mode==IMX136_HTM)&&(Mode!=IMX136_HTM)) )
		pass_stdby=TRUE;  //go through standby mode (Built-in WDR)
	if(	((pre_mode!=IMX136_WDR)&&(Mode==IMX136_WDR))||
		((pre_mode==IMX136_WDR)&&(Mode!=IMX136_WDR)) )
		pass_stdby=TRUE;  //go through standby mode (60Hz<->30Hz)
	pre_mode=Mode;

	if(pass_stdby)
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x01);	// STANDBY[0] - 0: Operation,  1: STANDBY

	//set mode
	switch(Mode) {
		case IMX136_NORMAL:
			CMOS_ByteWrite(SPI_IMX136_ADDR, 0x020C, 0x00);	// WDM MODE[1:0] (normal mode)
			break;
		case IMX136_WDR:
			CMOS_ByteWrite(SPI_IMX136_ADDR, 0x020C, 0x10);	// WDMODE[1:0] (0x10 : 2 frame sequential), (0x30 : 4 frame)
			break;
		case IMX136_HTM:
		default:
			CMOS_ByteWrite(SPI_IMX136_ADDR, 0x020C, 0x02);	// WDMODE[1:0] (built-in mode)
			break;
	}

	//change fps
	BOOL base_60fps=FALSE;
	if( (GetSS_SensorVoutFps()==50)||(GetSS_SensorVoutFps()==60) )
		base_60fps=TRUE;

#if	(!defined(__MDIN_i5XX_FPGA__)||defined(__USE_DDR_MCLK198__)||defined(__USE_DDR_MCLK180__))&&\
  	(defined(__USE_CMOS_IMXx36_LVDS_S4__))  //only 6ch mode supports 60Hz
	const BOOL valid_60wdr=TRUE;
#else
	const BOOL valid_60wdr=FALSE;
#endif
#if defined(__USE_CMOS_IMXx36_LVDS_S4__)
	const BOOL valid_30htm=TRUE;
#else
	const BOOL valid_30htm=FALSE;
#endif

#if defined(__USE_CMOS_IMXx36_TTL__)
	switch(Mode) {
		case IMX136_NORMAL:  stIMX136_SetFps(IMX136_FPS_15Hz);  break;
		case IMX136_WDR:     stIMX136_SetFps(IMX136_FPS_15Hz);  break;  //n/a
		case IMX136_HTM:     stIMX136_SetFps(IMX136_FPS_15Hz);  break;
	}
#else
	switch(Mode) {
		case IMX136_NORMAL:  //normal
			if(base_60fps)   stIMX136_SetFps(IMX136_FPS_60Hz);
			else             stIMX136_SetFps(IMX136_FPS_30Hz);
			break;
		case IMX136_WDR:  //60->30Hz
			if(valid_60wdr)  stIMX136_SetFps(IMX136_FPS_60Hz);
			else             stIMX136_SetFps(IMX136_FPS_30Hz);
			break;
		case IMX136_HTM:  //30Hz
			if(valid_30htm)  stIMX136_SetFps(IMX136_FPS_30Hz);
			else             stIMX136_SetFps(IMX136_FPS_15Hz);
			break;
		default:
			break;
	}
#endif

	//set compress
	if(Mode==IMX136_HTM) {
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x020F, 0x05);	// WDC_CMPEN[2]
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0210, 0x00);	// 
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0212, 0x2D);	// 

		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0265, 0x00);
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0286, 0x10);
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x02CF, 0xE1);
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x02D0, 0x30);
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x02D2, 0xC4);
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x02D3, 0x01);
	}
	else {
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x020F, 0x01);	//'00000101' compressed out enable and [0] is fixed '1'
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0210, 0x01);		
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0212, 0x00);		

		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0265, 0x20);
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0286, 0x01);
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x02CF, 0xD1);
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x02D0, 0x1B);
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x02D2, 0x5F);
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x02D3, 0x00);
	}

	if(pass_stdby)
		CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x00);	// STANDBY[0] - 0: Operation,  1: STANDBY

	return;
}

void IMX136_ChangeFps_Normal(void) {
	BYTE base_60fps=FALSE;
	if( (GetSS_SensorVoutFps()==50)||(GetSS_SensorVoutFps()==60) )
		base_60fps=TRUE;

#if defined(__USE_CMOS_IMXx36_LVDS_S2__)
	stIMX136_SetFps(IMX136_FPS_30Hz);

#elif defined(__USE_CMOS_IMXx36_LVDS_S4__)
  #if defined (__USE_IMXx36_60P_MODE__)
	stIMX136_SetFps(IMX136_FPS_60Hz);
  #else
  	if(base_60fps)
	stIMX136_SetFps(IMX136_FPS_60Hz);
	else
	stIMX136_SetFps(IMX136_FPS_30Hz);
  #endif	

#elif defined(__USE_CMOS_IMXx36_TTL__)
	stIMX136_SetFps(IMX136_FPS_15Hz);

#elif defined(__USE_CMOS_IMXx36_LVDS_P__)
  #if defined (__USE_IMXx36_120P_MODE__)
	stIMX136_SetFps(IMX136_FPS_60Hz);
  #elif defined (__USE_IMXx36_60P_MODE__)
	stIMX136_SetFps(IMX136_FPS_30Hz);
  #elif defined (__USE_IMXx36_30P_MODE__)
	stIMX136_SetFps(IMX136_FPS_15Hz);
  #endif

#endif
}


#endif	/* __USE_CMOS_IMX136__ */

/*  FILE_END_HERE */
