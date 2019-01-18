// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_IMX385__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
#if defined(__USE_IMX385_HCG_MODE__)
#define HCG 1  //FDG_SEL (0:LCG mode, 1:HCG mode)
#else
#define HCG 0  //FDG_SEL (0:LCG mode, 1:HCG mode)
#endif

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static IMX385_Fps_t imx385_fps=IMX385_FPS_30Hz;

#if defined(__USE_SENSOR_BAYER_10BIT__)
static BOOL imx385_normal_bit_mode=0; // 10b per pixel
#else
static BOOL imx385_normal_bit_mode=1; // 12b per pixel
#endif

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
static void IMX385_ChangeFps(IMX385_Fps_t fps);
static void IMX385_SetAdBits(BYTE bit12);

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void IMX385_ChangeFps_Normal(BYTE nID) {
	switch(nID){
		case  0 : 	
		case  1 :	IMX385_ChangeFps(IMX385_FPS_30Hz);	break;
		case  2 :
		case  3 :	IMX385_ChangeFps(IMX385_FPS_60Hz);	break;
		case  4 : 	
		case  5 :	IMX385_ChangeFps(IMX385_FPS_120Hz);	break;
		default : break;
	}
}


//-----------------------------------------------------------------------

void IMX385_Initialize(void)
{	

//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
#if defined(__USE_IMX385_720P30_MODE__)
	ROMDATA char RES[] = {"720pHD"} ;
	ROMDATA char FPS[] = {"@30"} ;
#endif

#if defined(__USE_IMX385_720P60_MODE__)
	ROMDATA char RES[] = {"720pHD"} ;
	ROMDATA char FPS[] = {"@60"} ;
#endif

#if defined(__USE_IMX385_1080P30_MODE__)	
	ROMDATA char RES[] = {"1080pFHD"} ;
	ROMDATA char FPS[] = {"@30"} ;
#endif	
	
#if defined(__USE_IMX385_1080P60_MODE__)	
	ROMDATA char RES[] = {"1080pFHD"} ;
	ROMDATA char FPS[] = {"@60"} ;
#endif		
	
#if defined(__USE_IMX385_1080P120_MODE__)	
	ROMDATA char RES[] = {"1080pFHD"} ;
	ROMDATA char FPS[] = {"@120"} ;
#endif	
//-----------------------------------------	
#if defined(__USE_CMOS_IMX385_LVDS_S2__)		
	ROMDATA char CH[]  = {"2ch lvds"} ;
#endif	

#if defined(__USE_CMOS_IMX385_LVDS_S4__)		
	ROMDATA char CH[]  = {"4ch lvds"} ;
#endif
	
#if defined(__USE_CMOS_IMX385_LVDS_S8__)		
	ROMDATA char CH[]  = {"8ch lvds"} ;
#endif	

#if defined(__USE_CMOS_IMX385_CSI2_L2__)		
	ROMDATA char CH[]  = {"2ch mipi"} ;
#endif	

#if defined(__USE_CMOS_IMX385_CSI2_L4__)		
	ROMDATA char CH[]  = {"4ch mipi"} ;
#endif
//-----------------------------------------	
	UARTprintf("\n\rIMX385_Initialize(%s%s %s mode)\n\r",RES,FPS,CH);
//-----------------------------------------	
//-----------------------------------------	
//-----------------------------------------	

//////////////////		Chip ID = 02h			//////////////////////	
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0201, 0x00);		// Register hold[0]	0: Invalid 1: Valid
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0202, 0x01);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0203, 0x00);		// Software Reset

#if defined(__USE_IMX385_720P30_MODE__)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0207, 0x40|0x00);	// [0] Vertical (V) direction readout inversion control,  [1] Horiznetal direction readout inversion control [7:4]  4: WINDOW CROP mode
#else
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0207, 0x00|0x00);	// [0] Vertical (V) direction readout inversion control,  [1] Horiznetal direction readout inversion control [7:4]  0 OR 1: 1080p mode
#endif

	//set frame rate with h/v total size
#if defined(__USE_IMX385_1080P120_MODE__)
	IMX385_ChangeFps_Normal(5);
#elif defined(__USE_IMX385_1080P60_MODE__) || defined(__USE_IMX385_720P60_MODE__)
	IMX385_ChangeFps_Normal(3);
#else
	IMX385_ChangeFps_Normal(1);
#endif

	// CMOS_ByteWrite(SPI_IMX385_ADDR, 0x020A, 0xF0);		// BLKLEVEL[7:0] - Black level (LSB)
	// CMOS_ByteWrite(SPI_IMX385_ADDR, 0x020B, 0x00);		// BLKLEVEL[0] - Black level (MSB)	

	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0212, 0x2C);             // Fixed (Default Set:2C) 
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0213, 0x01);             // Fixed (Default Set:01) 

	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0214, 0x00);		// Gain[7:0]	- 0.3db step
//	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0215, 0x00);             // Fixed (Default Set:0x00) and gain msb[1:0]
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0216, 0x09);             // Gain Reflect timing(09h : nextframe)

	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0220, 0x04);		// SHS1(LSB)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0221, 0x00);       // SHS1 
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0222, 0x00);       // SHS1(MSB 4bit)

	//223~235 do not rewrite

	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0236, 0x10);          // WINWV_OB[3:0]
	
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0238, 0x00);           // WINPV(LSB)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0239, 0x00);           // WINPV(MSB)
	// CMOS_ByteWrite(SPI_IMX385_ADDR, 0x023A, 0xD1);           // WINWV(LSB)
	// CMOS_ByteWrite(SPI_IMX385_ADDR, 0x023B, 0x03);           // WINWV(MSB)	
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x023A, 0x49);           // WINWV(LSB)
	// CMOS_ByteWrite(SPI_IMX385_ADDR, 0x023A, 0x48);           // WINWV(LSB)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x023B, 0x04);           // WINWV(MSB)		
	
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x023C, 0x00);           // WINPH(LSB)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x023D, 0x00);           // WINPH(MSB)	
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x023E, 0x1C);           // WINWH(LSB)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x023F, 0x05);           // WINWH(MSB)	

	//240~243 do not rewrite	
	
// #if defined(__USE_IMX385_CSI2_MODE__)
	// CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x01|0x00);  // Odbit and OPORTSEL		CSI-2
// #else	//sensor lvds mode
	// #if defined(__USE_IMX385_1080P120_MODE__)
		// CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x00|0xF0);  // Odbit and OPORTSEL		LVDS 8ch only at 120hz normal mode
	// #else
		// #if defined(__USE_CMOS_IMX385_LVDS_S2__)
		// CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x01|0xD0);  // Odbit and OPORTSEL		LVDS 4ch
		// #elif defined(__USE_CMOS_IMX385_LVDS_S4__)
		// CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x01|0xE0);  // Odbit and OPORTSEL		LVDS 2ch
		// #elif defined(__USE_CMOS_IMX385_LVDS_S8__)
		// CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x01|0xF0);  // Odbit and OPORTSEL		LVDS 8ch
		// #endif
	// #endif
// #endif

	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0246, 0x00);           		// [7:4] XVSLANG
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0247, 0x00|0x08);         // [7:4] XHSLANG [3] = Fixed value='1'
	
	//24A~53 do not write
#if defined(__USE_IMX385_CSI2_MODE__)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0254, 0x66|0x00);		// [7:1] FIXED [0] SCDEN = '1' when lvds mode, '0' CSI mode
#else
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0254, 0x66|0x01);		// [7:1] FIXED [0] SCDEN = '1' when lvds mode, '0' CSI mode
#endif

	//INCK 37.125
	// CMOS_ByteWrite(SPI_IMX385_ADDR, 0x025C, 0x18);           // INCLKSEL1 		SEE DATASHEET 87PAGE	<Setup in IMX385_SetAdBits()>
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x025D, 0x00);           // INCLKSEL2
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x025E, 0x20);           // INCLKSEL3
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x025F, 0x00);           // INCLKSEL4

	//300~30A do not write	
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x030B, 0x07);           // Fixed (Default Set:0x04) 	
	//30C~30F do not write		
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0310, 0x12);           // Fixed (Default Set:0x0E) 	
	//311~3EC do not write		
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x03ED, 0x38);           // Fixed (Default Set:0x0E) 	

	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0538, 0xD4);           // Fixed (Default Set:0xA0) 	
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0539, 0x40);           // Fixed (Default Set:0xC0) 	
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x053A, 0x10);           // Fixed (Default Set:0x12) 		
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x053B, 0x00);           // Fixed (Default Set:0x01) 	
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x053C, 0xD4);           // Fixed (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x053D, 0x40);           // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x053E, 0x10);           // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x053F, 0x00);           // Fixed (Default Set:0x01) 	

 #if defined(__USE_IMX385_CSI2_MODE__)					//datasheet page 57
	#if defined(__USE_CMOS_IMX385_CSI2_L2__)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0546, 0x01);		//PHYSICAL_LANE_NUM[1:0]	1: 2lane, 3:4lane
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x057F, 0x01);		//CSI_LANE_MODE[1:0]	1: 2lane, 3:4lane
	#else // #if defined(__USE_CMOS_IMX385_CSI2_L4__)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0546, 0x03);		//PHYSICAL_LANE_NUM[1:0]	1: 2lane, 3:4lane
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x057F, 0x03);		//CSI_LANE_MODE[1:0]	1: 2lane, 3:4lane
	#endif
	
	// CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0553, 0x02);		//OB_SIZE_V[5:0] OPB data line number settiong
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0553, 0x0E);		//OB_SIZE_V[5:0] OPB data line number settiong
	// CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0557, 0x49);		//PIC_SIZE_V[7:0]
	// CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0558, 0x04);		//PIC_SIZE_V[12:8]
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x056C, 0x1F);		//TCLKPRE(Global timing)

	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0580, 0x20);		//INCK_FREQ1[7:0]
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0581, 0x25);		//INCK_FREQ1[15:8]	

	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x058D, 0xB4);		//INCK_FREQ2[7:0]
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x058E, 0x01);		//INCK_FREQ2[10:8]		
#endif

	//06xx~ is mipi(sub-lvds mode register)
#if defined (__USE_DOL_2F_WDR_BOOT__)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0618, 0xB2);
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0619, 0x08);
#elif defined (__USE_DOL_3F_WDR_BOOT__)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0618, 0x17);
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0619, 0x0D);
#endif

#if defined (__USE_SENSOR_BAYER_10BIT__)
	IMX385_SetAdBits(0);

#if defined(__USE_IMX385_1080P120_MODE__)

CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x00|0xF0);  // Odbit and OPORTSEL		LVDS 8ch only at 120hz normal mode
	
#else

#if defined(__USE_CMOS_IMX385_LVDS_S2__)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x00|0xD0);  // Odbit and OPORTSEL		LVDS 4ch
#elif defined(__USE_CMOS_IMX385_LVDS_S4__)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x00|0xE0);  // Odbit and OPORTSEL		LVDS 2ch
#elif defined(__USE_CMOS_IMX385_LVDS_S8__)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x00|0xF0);  // Odbit and OPORTSEL		LVDS 8ch
#endif // #if defined(__USE_CMOS_IMX385_LVDS_S2__)

#endif // #if defined(__USE_IMX385_1080P120_MODE__)

#else
	IMX385_SetAdBits(1);

#endif // #if defined (__USE_SENSOR_BAYER_10BIT__)

//-------------------------------------------------------------------------------------
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0 : Normal operation
	Delay_mSec(50);										// Wait for internal regulator stabilization
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0202, 0x00);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
														// Wait for image stabilization > 8 frames(=33.33ms * 8)

}


//set IMX385 Frame WDR mode
void IMX385_WDR_Mode(IMX385_Mode_t Mode)
{
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x01);	// STANDBY[0] - 0: Operation,  1: STANDBY
	
	//set mode
	switch(Mode) {
		case IMX385_NORMAL:
			IMX385_ChangeFps(IMX385_FPS_30Hz);
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0207, 0x00|0x00);	// [0] Vertical (V) direction readout inversion control,  [1] Horiznetal direction readout inversion control [7:4]  0: 1080p mode			
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x020C, 0x00);  // WDSEL[5:4]=0,  WDMODE[0]=0
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0243, 0x01);  // HINFOEN[2]=0,  DOLSYDINFOEN[1]=0,  DOLSCDEN[0]=1
			break;
		case IMX385_WDR:
#if	defined(__MDIN_i5XX_FPGA__)
			IMX385_ChangeFps(IMX385_FPS_30Hz);
#else
			IMX385_ChangeFps(IMX385_FPS_60Hz);
#endif
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0207, 0x00|0x00);	// [0] Vertical (V) direction readout inversion control,  [1] Horiznetal direction readout inversion control [7:4]  0: 1080p mode	
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x020C, 0x10);  // WDSEL[5:4]=1,  WDMODE[0]=0
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0243, 0x01);  // HINFOEN[2]=0,  DOLSYDINFOEN[1]=0,  DOLSCDEN[0]=1
			break;
		case IMX385_DOL2:
#if	defined(__MDIN_i5XX_FPGA__)
			IMX385_ChangeFps(IMX385_FPS_30Hz_DOL2);
#else
			IMX385_ChangeFps(IMX385_FPS_60Hz_DOL2);
#endif
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0207, 0x01|0x00);	// [0] Vertical (V) direction readout inversion control,  [1] Horiznetal direction readout inversion control [7:4]  0: 1080p mode	
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x020C, 0x11);  // WDSEL[5:4]=1,  WDMODE[0]=1
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0243, 0x03);  // HINFOEN[2]=0,  DOLSYDINFOEN[1]=1,  DOLSCDEN[0]=1
			break;
		case IMX385_DOL3:
#if	defined(__MDIN_i5XX_FPGA__)
			IMX385_ChangeFps(IMX385_FPS_60Hz_DOL3);
#else
			IMX385_ChangeFps(IMX385_FPS_120Hz_DOL3);
#endif
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0207, 0x00|0x00);	// [0] Vertical (V) direction readout inversion control,  [1] Horiznetal direction readout inversion control [7:4]  0: 1080p mode	
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x020C, 0x21);  // WDSEL[5:4]=2,  WDMODE[0]=1
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0243, 0x03);  // HINFOEN[2]=0,  DOLSYDINFOEN[1]=1,  DOLSCDEN[0]=1
			break;
		default:
			IMX385_ChangeFps(IMX385_FPS_30Hz);
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x020C, 0x00);  // WDSEL[5:4]=0	,  WDMODE[0]=0
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0243, 0x01);  // HINFOEN[2]=0,  DOLSYDINFOEN[1]=0,  DOLSCDEN[0]=1
			break;
	}
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x00);	// STANDBY[0] - 0: Operation,  1: STANDBY

	return;
}

static void IMX385_SetAdBits(BYTE bit12) {
	if(bit12) {  //ADBIT : 12bit
		CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0205, 0x01);    // [0] ADBIT
		CMOS_ByteWrite(SPI_IMX385_ADDR, 0x020A, 0xF0);    // BLKLEVEL[7:0] - Black level (LSB)
		CMOS_ByteWrite(SPI_IMX385_ADDR, 0x020B, 0x00);    // BLKLEVEL[0] - Black level (MSB)	

		CMOS_ByteWrite(SPI_IMX385_ADDR, 0x025C, 0x18);           // INCLKSEL1 	SEE DATASHEET 87PAGE			

		#if defined(__USE_IMX385_CSI2_MODE__)
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x057D, 0x0C);		//CSI_DT_FMT[7:0] < RAW10 : 0x00 > < RAW12 :0x0C>
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x057E, 0x0C);		//CSI_DT_FMT[15:8] < RAW10 : 0x00 > < RAW12 :0x0C>	
		#endif		
		
	}
	else {  //ADBIT : 10bit
		CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0205, 0x00);    // [0] ADBIT
		CMOS_ByteWrite(SPI_IMX385_ADDR, 0x020A, 0x3C);    // BLKLEVEL[7:0] - Black level (LSB)
		CMOS_ByteWrite(SPI_IMX385_ADDR, 0x020B, 0x00);    // BLKLEVEL[0] - Black level (MSB)	

		CMOS_ByteWrite(SPI_IMX385_ADDR, 0x025C, 0x28);           // INCLKSEL1 	SEE DATASHEET 87PAGE	

		#if defined(__USE_IMX385_CSI2_MODE__)
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x057D, 0x0A);		//CSI_DT_FMT[7:0]
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x057E, 0x0A);		//CSI_DT_FMT[15:8]			
		#endif				
	}
}


static void IMX385_ChangeFps(IMX385_Fps_t fps) 
{
	WORD hmax, vmax;
#if defined(__USE_IMX385_720P30_MODE__) || defined(__USE_IMX385_720P60_MODE__)
	const WORD hbase=3300;
	const WORD vbase= 750;
#else
	const WORD hbase=2200;
	const WORD vbase=1125;
#endif
	imx385_fps=fps;
	switch(fps) {
		case IMX385_FPS_30Hz:
			hmax=hbase*2;
			vmax=vbase*1;
			
			//common setting
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0209, HCG<<4|0x02);  // FRSEL[1:0]	Set to datarate.		4ch lvds 30fps 12bit
			IMX385_SetAdBits(1);  //ADBIT:12bit
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0554, 0x01);   
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0557, 0x49);    // [7:0] PIC_SIZE_V
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0558, 0x04);    // [3:0] PIC_SIZE_V				
			
			#if defined(__USE_IMX385_LVDS_MODE__)
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x01|0xE0);    // ODBIT and OPORTSEL LVDS 4ch

			#elif defined(__USE_IMX385_CSI2_MODE__)
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x01|0x00);    // ODBIT and OPORTSEL CSI-2
				#if defined(__USE_CMOS_IMX385_CSI2_L2__)
				//datasheet page 57, 12bit 2lane 1080p25,30 & 4lane 1080p50,60 data rate 445.5Mbps/Lane
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0544, 0x00|0x00);	//REPETITION[5:4]
				
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x056B, 0x3F);		//THSEXIT[7:0]
				
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0582, 0x67);		//TCLKPOST[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0583, 0x1F);		//THSPREPARE[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0584, 0x3F);		//THSZERO[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0585, 0x27);		//THSRAIL[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0586, 0x1F);		//TCLKRAIL[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0587, 0x17);		//TCLKPREPARE[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0588, 0x77);		//TCLKZERO[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0589, 0x27);		//TLPX[7:0]					
				#else // #if defined(__USE_CMOS_IMX385_CSI2_L4__)
				//datasheet page 57, 12bit 4lane 12bit 25,30, data rate 222.75Mbps/Lane
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0544, 0x10|0x00);	//REPETITION[5:4]
				
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x056B, 0x2F);		//THSEXIT[7:0]
				
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0582, 0x5F);		//TCLKPOST[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0583, 0x17);		//THSPREPARE[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0584, 0x2F);		//THSZERO[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0585, 0x17);		//THSRAIL[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0586, 0x17);		//TCLKRAIL[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0587, 0x0F);		//TCLKPREPARE[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0588, 0x4F);		//TCLKZERO[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0589, 0x27);		//TLPX[7:0]	
				#endif
			#endif			
			break;
		case IMX385_FPS_60Hz:
			hmax=hbase*1;
			vmax=vbase*1;
						
			// CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x01|0xE0);    // ODBIT and OPORTSEL		LVDS 4ch
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0209, HCG<<4|0x01);  // FRSEL[1:0]	Set to datarate.		4ch lvds 60fps 12bit
			IMX385_SetAdBits(1);  //ADBIT:12bit
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0554, 0x01);    // NULL0_SIZE_V
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0557, 0x49);    // [7:0] PIC_SIZE_V
			//CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0557, 0x48);    // [7:0] PIC_SIZE_V
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0558, 0x04);    // [3:0] PIC_SIZE_V		

			#if defined(__USE_IMX385_LVDS_MODE__)
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x01|0xE0);    // ODBIT and OPORTSEL LVDS 4ch

			#elif defined(__USE_IMX385_CSI2_MODE__)
				//datasheet page 57, 12bit 4lane 1080p50,60 data rate 445.5Mbps/Lane < 2lane cannot do 12bit 4lane 1080p50,60 >
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x01|0x00);    // ODBIT and OPORTSEL LVDS 4ch
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0544, 0x00|0x00);	//REPETITION[5:4]
				
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x056B, 0x3F);		//THSEXIT[7:0]
				
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0582, 0x67);		//TCLKPOST[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0583, 0x1F);		//THSPREPARE[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0584, 0x3F);		//THSZERO[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0585, 0x27);		//THSRAIL[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0586, 0x1F);		//TCLKRAIL[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0587, 0x17);		//TCLKPREPARE[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0588, 0x77);		//TCLKZERO[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0589, 0x27);		//TLPX[7:0]			
			#endif			
			break;
		case IMX385_FPS_120Hz:
			hmax=hbase/2;
			vmax=vbase*1;
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x00|0xF0);    // ODBIT and OPORTSEL		LVDS 8ch
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0209, HCG<<4|0x00);  // FRSEL[1:0]	Set to datarate.		4ch lvds 60fps 12bit
			IMX385_SetAdBits(0);  //ADBIT:10bit
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0554, 0x01);    // NULL0_SIZE_V
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0557, 0x49);    // [7:0] PIC_SIZE_V
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0558, 0x04);    // [3:0] PIC_SIZE_V			

			#if defined(__USE_IMX385_CSI2_MODE__)
				//datasheet page 57, 10bit 4lane 1080p100,120 data rate 445.5
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0544, 0x00|0x00);	//REPETITION[5:4]
				
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x056B, 0x3F);		//THSEXIT[7:0]
				
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0582, 0x67);		//TCLKPOST[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0583, 0x1F);		//THSPREPARE[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0584, 0x3F);		//THSZERO[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0585, 0x27);		//THSRAIL[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0586, 0x1F);		//TCLKRAIL[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0587, 0x17);		//TCLKPREPARE[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0588, 0x77);		//TCLKZERO[7:0]	
				CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0589, 0x27);		//TLPX[7:0]			
			#endif			
			break;		
		case IMX385_FPS_30Hz_DOL2:
			hmax=hbase*2;
			vmax=vbase*2;
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x01|0xE0);    // ODBIT and OPORTSEL		LVDS 4ch
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0209, HCG<<4|0x02);  // FRSEL[1:0]	Set to datarate.		4ch lvds 60fps 12bit
			IMX385_SetAdBits(1);  //ADBIT:12bit
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0554, 0x00);    // [0] NULL0_SIZE_V		
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0557, 0xB2);    // [7:0] PIC_SIZE_V
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0558, 0x08);    // [3:0] PIC_SIZE_V					
			break;
		case IMX385_FPS_30Hz_DOL3:
			hmax=hbase*2;
			vmax=vbase*4;
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x01|0xE0);    // ODBIT and OPORTSEL		LVDS 4ch
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0209, HCG<<4|0x02);  // FRSEL[1:0]	Set to datarate.		4ch lvds 60fps 12bit
			IMX385_SetAdBits(1);  //ADBIT:12bit
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0554, 0x00);    // [0] NULL0_SIZE_V		
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0557, 0x17);    // [7:0] PIC_SIZE_V
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0558, 0x0D);    // [3:0] PIC_SIZE_V				
			break;
		case IMX385_FPS_60Hz_DOL2:
			hmax=hbase*1;
			vmax=vbase*2;
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x01|0xE0);    // ODBIT and OPORTSEL		LVDS 4ch
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0209, HCG<<4|0x01);  // FRSEL[1:0]	Set to datarate.		4ch lvds 60fps 12bit
			IMX385_SetAdBits(1);  //ADBIT:12bit
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0554, 0x00);    // [0] NULL0_SIZE_V						
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0557, 0xB2);    // [7:0] PIC_SIZE_V
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0558, 0x08);    // [3:0] PIC_SIZE_V		
			break;
		case IMX385_FPS_60Hz_DOL3:
			hmax=hbase*1;
			vmax=vbase*4;
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x01|0xE0);    // ODBIT and OPORTSEL		LVDS 4ch
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0209, HCG<<4|0x01);  // FRSEL[1:0]	Set to datarate.		4ch lvds 60fps 12bit
			IMX385_SetAdBits(1);  //ADBIT:12bit
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0554, 0x00);    // [0] NULL0_SIZE_V	
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0557, 0x17);    // [7:0] PIC_SIZE_V
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0558, 0x0D);    // [3:0] PIC_SIZE_V					
			break;
		case IMX385_FPS_120Hz_DOL2:
			hmax=hbase/2;
			vmax=vbase*2;
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x00|0xF0);    // ODBIT and OPORTSEL		LVDS 8ch
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0209, HCG<<4|0x00);  // FRSEL[1:0]	Set to datarate.		8ch lvds 120fps 10bit
			IMX385_SetAdBits(0);  //ADBIT:10bit
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0554, 0x00);    // [0] NULL0_SIZE_V						
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0557, 0xB2);    // [7:0] PIC_SIZE_V
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0558, 0x08);    // [3:0] PIC_SIZE_V		
			break;
		case IMX385_FPS_120Hz_DOL3:
			hmax=hbase/2;
			vmax=vbase*4;
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x00|0xF0);    // ODBIT and OPORTSEL		LVDS 8ch
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0209, HCG<<4|0x00);  // FRSEL[1:0]	Set to datarate.		8ch lvds 120fps 10bit
			IMX385_SetAdBits(0);  //ADBIT:10bit
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0554, 0x00);    // [0] NULL0_SIZE_V	
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0557, 0x17);    // [7:0] PIC_SIZE_V
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0558, 0x0D);    // [3:0] PIC_SIZE_V					
			break;
		default:
			hmax=hbase*2;
			vmax=vbase*1;
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0244, 0x01|0xE0);    // ODBIT and OPORTSEL		LVDS 4ch
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0209, HCG<<4|0x02);  // FRSEL[1:0]	Set to datarate.		4ch lvds 30fps 12bit
			IMX385_SetAdBits(1);  //ADBIT:12bit
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0554, 0x01);    // NULL0_SIZE_V
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0557, 0x49);    // [7:0] PIC_SIZE_V
			CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0558, 0x04);    // [3:0] PIC_SIZE_V					
			break;
	}

	//pal system
#if defined(__VTOTAL_CTRL_PAL__)
	if(GetSS_SensorVoutPAL())  vmax*=1.2f;
#else
	if(GetSS_SensorVoutPAL())  hmax*=1.2f;
#endif

	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x021B, LOBYTE(hmax));	// HMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x021C, HIBYTE(hmax));	// HMAX [7:0] (MSB)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0218, LOBYTE(vmax));	// VMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0219, HIBYTE(vmax));	// VMAX [7:0] (MSB)
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x021A, 0x00);			// VMAX [0]   (MSB)
	
}


// temp code to test fps
IMX385_Fps_t IMX385_GetFrameRate(void) {
	return imx385_fps;
}

void IMX385_SetFrameRate(IMX385_Fps_t fps) {
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0200, 0x01);	// STANDBY[0] - 0: Operation,  1: STANDBY
	IMX385_ChangeFps(fps);
	CMOS_ByteWrite(SPI_IMX385_ADDR, 0x0200, 0x00);	// STANDBY[0] - 0: Operation,  1: STANDBY
	return;
}


#endif	/* __USE_CMOS_IMX385__ */

/*  FILE_END_HERE */

