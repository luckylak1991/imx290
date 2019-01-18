// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
#if defined(__USE_IMX290_HCG_MODE__)
#define HCG 1  //FDG_SEL (0:LCG mode, 1:HCG mode)
#else
#define HCG 0  //FDG_SEL (0:LCG mode, 1:HCG mode)
#endif

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
#if defined(__USE_IMX290_1080P120_MODE__) || defined(__USE_IMX290_720P120_MODE__) || defined(__USE_IMX290_720P120_ISPCROP__)
static IMX290_Fps_t imx290_fps=IMX290_FPS_120Hz;
#elif defined(__USE_IMX290_1080P60_MODE__) || defined(__USE_IMX290_720P60_MODE__)
static IMX290_Fps_t imx290_fps=IMX290_FPS_60Hz;
#else
static IMX290_Fps_t imx290_fps=IMX290_FPS_30Hz;
#endif



#if defined(__USE_SENSOR_BAYER_10BIT__)
static BOOL imx290_normal_bit_mode=0; // 10b per pixel
#else
static BOOL imx290_normal_bit_mode=1; // 12b per pixel
#endif

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
static void IMX290_ChangeFps(IMX290_Fps_t fps);
static void IMX290_SetAdBits(BYTE bit12);

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void IMX290_ChangeFps_Normal(BYTE nID) {
	switch(nID){
		case  0 : 	
		case  1 :	IMX290_ChangeFps(IMX290_FPS_30Hz);	break;
		case  2 :
		case  3 :	IMX290_ChangeFps(IMX290_FPS_60Hz);	break;
		case  4 : 	
		case  5 : IMX290_ChangeFps(IMX290_FPS_120Hz);	break;
		default : break;
	}
}


//-----------------------------------------------------------------------

void IMX290_Initialize(void)
{	

//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
#if defined(__USE_IMX290_720P30_MODE__)
	ROMDATA char RES[] = {"720pHD"} ;
	ROMDATA char FPS[] = {"@30"} ;
#endif

#if defined(__USE_IMX290_720P60_MODE__)
	ROMDATA char RES[] = {"720pHD"} ;
	ROMDATA char FPS[] = {"@60"} ;
#endif

#if defined(__USE_IMX290_720P120_MODE__)
	ROMDATA char RES[] = {"720pHD"} ;
	ROMDATA char FPS[] = {"@120"} ;
#endif

#if defined(__USE_IMX290_720P120_ISPCROP__)	
	ROMDATA char RES[] = {"1080CROP"} ;
	ROMDATA char FPS[] = {"@120"} ;
#endif	

#if defined(__USE_IMX290_1080P30_MODE__)	
	ROMDATA char RES[] = {"1080pFHD"} ;
	ROMDATA char FPS[] = {"@30"} ;
#endif	
	
#if defined(__USE_IMX290_1080P60_MODE__)	
	ROMDATA char RES[] = {"1080pFHD"} ;
	ROMDATA char FPS[] = {"@60"} ;
#endif		
	
#if defined(__USE_IMX290_1080P120_MODE__)	
	ROMDATA char RES[] = {"1080pFHD"} ;
	ROMDATA char FPS[] = {"@120"} ;
#endif	
//-----------------------------------------	
#if defined(__USE_CMOS_IMX290_LVDS_S2__)		
	ROMDATA char CH[]  = {"2ch lvds"} ;
#endif	

#if defined(__USE_CMOS_IMX290_LVDS_S4__)		
	ROMDATA char CH[]  = {"4ch lvds"} ;
#endif
	
#if defined(__USE_CMOS_IMX290_LVDS_S8__)		
	ROMDATA char CH[]  = {"8ch lvds"} ;
#endif	
//-----------------------------------------	
#if defined(__USE_CMOS_IMX327__)
	UARTprintf("\n\rIMX327_Initialize(%s%s %s mode)\n\r",RES,FPS,CH);
#else
	UARTprintf("\n\rIMX290_Initialize(%s%s %s mode)\n\r",RES,FPS,CH);
#endif
//-----------------------------------------	
//-----------------------------------------	
//-----------------------------------------	

//////////////////		Chip ID = 02h			//////////////////////	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0201, 0x00);		// Register hold[0]	0: Invalid 1: Valid
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0202, 0x01);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0203, 0x00);		// Software Reset
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0204, 0x10);		// Fixed (Default Set:0x10) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0205, 0x00|0x01);	// [0] ADBIT [4]STD12EN, Valid when 10bit mode
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0206, 0x00);		// Fixed (Default Set:0x00) 	
#if defined(__USE_IMX290_720P30_MODE__) || defined(__USE_IMX290_720P60_MODE__) || defined(__USE_IMX290_720P120_MODE__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0207, 0x10|0x00);	// [0] Vertical (V) direction readout inversion control,  [1] Horiznetal direction readout inversion control [7:4]  1: 720p mode
#else
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0207, 0x00|0x00);	// [0] Vertical (V) direction readout inversion control,  [1] Horiznetal direction readout inversion control [7:4]  0: 1080p mode
#endif
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0208, 0xA0);		// Fixed (Default Set:0xA0) 	

	//set frame rate with h/v total size
	if(imx290_fps==IMX290_FPS_30Hz)	IMX290_ChangeFps_Normal(1);
	else if(imx290_fps==IMX290_FPS_60Hz)	IMX290_ChangeFps_Normal(3);	
	else if(imx290_fps==IMX290_FPS_120Hz)	IMX290_ChangeFps_Normal(5);	
	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x020A, 0xF0);		// BLKLEVEL[7:0] - Black level (LSB)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x020B, 0x00);		// BLKLEVEL[0] - Black level (MSB)	


#if defined(__USE_CMOS_IMX327__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0211, 0x0A);             // Fixed (Default Set:0x00) 
#else
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x020C, 0x00);             // Fixed (Default Set:0x00)
	
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x020D, 0x00);             // Fixed (Default Set:0x00) 	
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x020E, 0x01);             // Fixed (Default Set:0x01) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x020F, 0x00);             // Set (Default Set:0x01) 
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0210, 0x21);             // set (Default Set:0x01) 
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0211, 0x00);             // Fixed (Default Set:0x00) 

	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0212, 0x64);             // Fixed (Default Set:0xF0) 
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0213, 0x00);             // Fixed (Default Set:0x00) 

	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0214, 0x00);		// Gain[7:0]	- 0.3db step
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0215, 0x00);             // Fixed (Default Set:0x00) 
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0216, 0x09);             // Fixed (Default Set:0x08) 
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0217, 0x00);             // Fixed (Default Set:0x00) 
#endif

//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x021B, 0x00);	// Fixed (Default Set:0x00) 

//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x021E, 0xB2);             // Set (Default Set:0xB2) 
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x021F, 0x01);             // Set (Default Set:0x01) 	

	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0220, 0x04);		// SHS1(LSB)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0221, 0x00);       // SHS1 
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0222, 0x00);       // SHS1(MSB 4bit)
	//223~239 do not rewrite

	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x023A, 0x0C);          // WINWV_OB[3:0]
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x023B, 0x00);             // Fixed (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x023C, 0x00);           // WINPB(LSB)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x023D, 0x00);           // WINPB(MSB)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x023E, 0x49);           // WINWV(LSB)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x023F, 0x04);           // WINWV(MSB)	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0240, 0x00);           // WINPH(LSB)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0241, 0x00);           // WINPH(MSB)	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0242, 0x9C);           // WINWH(LSB)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0243, 0x07);           // WINWH(MSB)	
	//244~245 do not rewrite	

#if defined(__USE_IMX290_1080P120_MODE__) || defined(__USE_IMX290_720P120_ISPCROP__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x00|0xF0);  // Odbit and OPORTSEL		LVDS 8ch only at 120hz fhd mode
#elif defined(__USE_IMX290_720P120_MODE__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x00|0xE0);  // Odbit and OPORTSEL		LVDS 4ch 120hz hd mode
#else
#if defined(__USE_CMOS_IMX290_LVDS_S2__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x01|0xD0);  // Odbit and OPORTSEL		LVDS 2ch
#elif defined(__USE_CMOS_IMX290_LVDS_S4__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x01|0xE0);  // Odbit and OPORTSEL		LVDS 4ch
#elif defined(__USE_CMOS_IMX290_LVDS_S8__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x01|0xF0);  // Odbit and OPORTSEL		LVDS 8ch
#endif
#endif
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0247, 0x01);           // Fixed (Default Set:0x01) 	

	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0248, 0x00);           // XVSLANG
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0249, 0x08);           // XHSLANG
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x024A, 0x00);           // Fixed (Default Set:0x00) 	
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x024B, 0x00);           // Fixed (Default Set:0x00) 	
	//24C~5B do not write
	//INCK
#if defined(__USE_IMX290_720P30_MODE__) || defined(__USE_IMX290_720P60_MODE__) || defined(__USE_IMX290_720P120_MODE__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x025C, 0x20);           // INCLKSEL1
#else
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x025C, 0x18);           // INCLKSEL1
#endif
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x025D, 0x00);           // INCLKSEL2
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x025E, 0x20);           // INCLKSEL3
#if defined(__USE_IMX290_720P30_MODE__) || defined(__USE_IMX290_720P60_MODE__) || defined(__USE_IMX290_720P120_MODE__)	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x025F, 0x01);           // INCLKSEL4
#else
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x025F, 0x10);           // INCLKSEL4
#endif

#if defined(__USE_CMOS_IMX327__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x029E, 0x4A);           // Fixed (Default Set:0x5A) 		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x029F, 0x4A);           // Fixed (Default Set:0x5A) 
#else
	//260~6F do not write	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0270, 0x02);           // Fixed (Default Set:0x01) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0271, 0x11);           // Fixed (Default Set:0x00) 		
	//272~9A do not write		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x029B, 0x10);           // Fixed (Default Set:0x00) 	
	//29C~A1 do not write		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x029C, 0x22);           // Fixed (Default Set:0x32)  //OB Clamp threshold
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x02A2, 0x02);           // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x02A6, 0x20);           // Fixed (Default Set:0x10) 		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x02A8, 0x20);           // Fixed (Default Set:0x10) 
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x02AA, 0x20);           // Fixed (Default Set:0x10) 
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x02AC, 0x20);           // Fixed (Default Set:0x10) 
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x02B0, 0x43);           // Fixed (Default Set:0x41) 
#endif

	//300~18 do not write	

#if defined(__USE_CMOS_IMX327__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0328, 0x04);           // Fixed (Default Set:0x05) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0329, 0x00);           // ADBIT1[7:0] 12bit 0
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x033B, 0x41);           // Fixed (Default Set:0x51) 		
#else
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0319, 0x9E);           // Fixed (Default Set:0x92) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x031C, 0x1E);           // Fixed (Default Set:0x12) 	
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x031D, 0x00);           // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x031E, 0x08);           // Fixed (Default Set:0x05) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0328, 0x05);           // Fixed (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0329, 0x00);           // ADBIT1[7:0] 12bit 0
		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x033D, 0x83);           // Fixed (Default Set:0x80) 		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0350, 0x03);           // Fixed (Default Set:0x02) 	
#endif

	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x037C, 0x00);           // ADBIT2[7:0](Default Set:0x17) 		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x037E, 0x00);           // Set 00h
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x03EC, 0x0E);           // ADBIT3[7:0]
	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x035E, 0x1A);             // INCLKSEL5
	//35f~63 do not write		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0364, 0x1A);             // INCLKSEL6	
	//365~ff do not write	

#if	defined(__USE_CMOS_IMX290__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x04B8, 0x50);           // Fixed (Default Set:0x92) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x04B9, 0x10);           // Fixed (Default Set:0x12) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x04BA, 0x00);           // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x04BB, 0x04);           // Fixed (Default Set:0x05) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x04C8, 0x50);           // Fixed (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x04C9, 0x10);           // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x04CA, 0x00);           // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x04CB, 0x04);           // Fixed (Default Set:0x17) 	

	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x052C, 0xD3);           // Fixed (Default Set:0x92) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x052D, 0x10);           // Fixed (Default Set:0x12) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x052E, 0x0D);           // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0558, 0x06);           // Fixed (Default Set:0x05) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0559, 0xE1);           // Fixed (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x055A, 0x11);           // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0560, 0x1E);           // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0561, 0x61);           // Fixed (Default Set:0x17) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0562, 0x10);           // Fixed (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x05B0, 0x50);           // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x05B2, 0x1A);           // Fixed (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x05B3, 0x04);           // Fixed (Default Set:0x17) 	
#endif	

	//06xx~ is mipi(sub-lvds mode register)
#if defined (__USE_DOL_2F_WDR_BOOT__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0618, 0xB2);
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0619, 0x08);
#elif defined (__USE_DOL_3F_WDR_BOOT__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0618, 0x17);
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0619, 0x0D);
#endif

#if defined (__USE_SENSOR_BAYER_10BIT__)
	IMX290_SetAdBits(0);
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0205, 0x00);    // [0] ADBIT = 0
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0329, 0x1D);    // [7:0] ADBIT1
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x037C, 0x12);    // [7:0] ADBIT2
//	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x03EC, 0x37);    // [7:0] ADBIT3

#if defined(__USE_IMX290_1080P120_MODE__) || defined(__USE_IMX290_720P120_ISPCROP__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x00|0xF0);  // Odbit and OPORTSEL		LVDS 8ch only at 120hz normal mode
#elif defined(__USE_IMX290_720P120_MODE__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x00|0xE0);  // Odbit and OPORTSEL		LVDS 4ch 120hz hd mode	
#else
#if defined(__USE_CMOS_IMX290_LVDS_S2__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x00|0xD0);  // Odbit and OPORTSEL		LVDS 4ch
#elif defined(__USE_CMOS_IMX290_LVDS_S4__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x00|0xE0);  // Odbit and OPORTSEL		LVDS 2ch
#elif defined(__USE_CMOS_IMX290_LVDS_S8__)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x00|0xF0);  // Odbit and OPORTSEL		LVDS 8ch
#endif
#endif

	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x020A, 0x3C);		// BLKLEVEL[7:0] - Black level (LSB)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x020B, 0x00);		// BLKLEVEL[0] - Black level (MSB)	
#endif

//-------------------------------------------------------------------------------------
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0 : Normal operation
	Delay_mSec(50);										// Wait for internal regulator stabilization
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0202, 0x00);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
														// Wait for image stabilization > 8 frames(=33.33ms * 8)

}


//set IMX290 Frame WDR mode
void IMX290_WDR_Mode(IMX290_Mode_t Mode)
{
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x01);	// STANDBY[0] - 0: Operation,  1: STANDBY
	
	//set mode
	switch(Mode) {
		case IMX290_NORMAL:
#if	defined(__USE_IMX290_1080P120_MODE__) || defined(__USE_IMX290_720P120_MODE__) || defined(__USE_IMX290_720P120_ISPCROP__)
			IMX290_ChangeFps(IMX290_FPS_120Hz);
#elif	defined(__USE_IMX290_1080P60_MODE__) || defined(__USE_IMX290_720P60_MODE__)
			IMX290_ChangeFps(IMX290_FPS_60Hz);
#else
			IMX290_ChangeFps(IMX290_FPS_30Hz);
#endif
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x020C, 0x00);  // WDSEL[5:4]=0,  WDMODE[0]=0
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0245, 0x01);  // HINFOEN[2]=0,  DOLSYDINFOEN[1]=0,  DOLSCDEN[0]=1
			break;
		case IMX290_WDR:
#if	defined(__MDIN_i5XX_FPGA__)
			IMX290_ChangeFps(IMX290_FPS_30Hz);
#else
			IMX290_ChangeFps(IMX290_FPS_60Hz);
#endif
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x020C, 0x10);  // WDSEL[5:4]=1,  WDMODE[0]=0
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0245, 0x01);  // HINFOEN[2]=0,  DOLSYDINFOEN[1]=0,  DOLSCDEN[0]=1
			break;
		case IMX290_DOL2:
#if	defined(__MDIN_i5XX_FPGA__)
			IMX290_ChangeFps(IMX290_FPS_30Hz_DOL2);
#else
			IMX290_ChangeFps(IMX290_FPS_60Hz_DOL2);
#endif
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x020C, 0x11);  // WDSEL[5:4]=1,  WDMODE[0]=1
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0245, 0x03);  // HINFOEN[2]=0,  DOLSYDINFOEN[1]=1,  DOLSCDEN[0]=1
			break;
		case IMX290_DOL3:
#if	defined(__MDIN_i5XX_FPGA__)
			IMX290_ChangeFps(IMX290_FPS_60Hz_DOL3);
#else
			IMX290_ChangeFps(IMX290_FPS_120Hz_DOL3);
#endif
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x020C, 0x21);  // WDSEL[5:4]=2,  WDMODE[0]=1
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0245, 0x03);  // HINFOEN[2]=0,  DOLSYDINFOEN[1]=1,  DOLSCDEN[0]=1
			break;
		default:
#if	defined(__USE_IMX290_1080P120_MODE__) || defined(__USE_IMX290_720P120_MODE__) || defined(__USE_IMX290_720P120_ISPCROP__)
			IMX290_ChangeFps(IMX290_FPS_120Hz);
#elif	defined(__USE_IMX290_1080P60_MODE__)
			IMX290_ChangeFps(IMX290_FPS_60Hz);
#else
			IMX290_ChangeFps(IMX290_FPS_30Hz);
#endif
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x020C, 0x00);  // WDSEL[5:4]=0	,  WDMODE[0]=0
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0245, 0x01);  // HINFOEN[2]=0,  DOLSYDINFOEN[1]=0,  DOLSCDEN[0]=1
			break;
	}
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x00);	// STANDBY[0] - 0: Operation,  1: STANDBY

	return;
}

static void IMX290_SetAdBits(BYTE bit12) {
	if(bit12) {  //ADBIT : 12bit
		CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0205, 0x01);    // [0] ADBIT
		CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0329, 0x00);    // [7:0] ADBIT1
		CMOS_ByteWrite(SPI_IMX290_ADDR, 0x037C, 0x00);    // [7:0] ADBIT2
		CMOS_ByteWrite(SPI_IMX290_ADDR, 0x03EC, 0x0E);    // [7:0] ADBIT3
		CMOS_ByteWrite(SPI_IMX290_ADDR, 0x020A, 0xF0);    // BLKLEVEL[7:0] - Black level (LSB)
		CMOS_ByteWrite(SPI_IMX290_ADDR, 0x020B, 0x00);    // BLKLEVEL[0] - Black level (MSB)	
	}
	else {  //ADBIT : 10bit
		CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0205, 0x00);    // [0] ADBIT
		CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0329, 0x1D);    // [7:0] ADBIT1
		CMOS_ByteWrite(SPI_IMX290_ADDR, 0x037C, 0x12);    // [7:0] ADBIT2
		CMOS_ByteWrite(SPI_IMX290_ADDR, 0x03EC, 0x37);    // [7:0] ADBIT3
		CMOS_ByteWrite(SPI_IMX290_ADDR, 0x020A, 0x3C);    // BLKLEVEL[7:0] - Black level (LSB)
		CMOS_ByteWrite(SPI_IMX290_ADDR, 0x020B, 0x00);    // BLKLEVEL[0] - Black level (MSB)	
	}
}


static void IMX290_ChangeFps(IMX290_Fps_t fps) 
{
	WORD hmax, vmax;
#if defined(__USE_IMX290_720P30_MODE__) || defined(__USE_IMX290_720P60_MODE__) || defined(__USE_IMX290_720P120_MODE__)
	WORD hbase=3300;
	WORD vbase= 750;
#else
	WORD hbase=2200;
	WORD vbase=1125;
#endif
	imx290_fps=fps;
	switch(fps) {
		case IMX290_FPS_30Hz:
			hmax=hbase*2;
			vmax=vbase*1;
#if defined(__USE_CMOS_IMX290_LVDS_S8__)
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x01|0xF0);    // ODBIT and OPORTSEL		LVDS 8ch			
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0209, HCG<<4|0x01);  // FRSEL[1:0]	Set to datarate.		8ch lvds 30fps 12bit
#else
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x01|0xE0);    // ODBIT and OPORTSEL		LVDS 4ch
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0209, HCG<<4|0x02);  // FRSEL[1:0]	Set to datarate.		4ch lvds 30fps 12bit
#endif
			IMX290_SetAdBits(1);  //ADBIT:12bit
			break;
		case IMX290_FPS_60Hz:
			hmax=hbase*1;
			vmax=vbase*1;
#if defined(__USE_CMOS_IMX290_LVDS_S8__)
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x01|0xF0);    // ODBIT and OPORTSEL		LVDS 8ch			
#else
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x01|0xE0);    // ODBIT and OPORTSEL		LVDS 4ch
#endif
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0209, HCG<<4|0x01);  // FRSEL[1:0]	Set to datarate.		4ch lvds 60fps 12bit
			IMX290_SetAdBits(1);  //ADBIT:12bit
			break;
		case IMX290_FPS_120Hz:
			hmax=hbase/2;
			vmax=vbase*1;
#if defined(__USE_CMOS_IMX290_LVDS_S8__)			
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x00|0xF0);    // ODBIT and OPORTSEL		LVDS 8ch
#else
	#if defined(__USE_IMX290_720P120_MODE__)
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x00|0xE0);  // Odbit and OPORTSEL		LVDS 4ch 120hz hd mode
	#else
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x01|0xE0);    // ODBIT and OPORTSEL		LVDS 4ch
	#endif
#endif
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0209, HCG<<4|0x00);  // FRSEL[1:0]	Set to datarate.		4ch lvds 60fps 12bit
			IMX290_SetAdBits(0);  //ADBIT:10bit
			break;		
		case IMX290_FPS_30Hz_DOL2:
			hmax=hbase*2;
			vmax=vbase*2;
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x01|0xE0);    // ODBIT and OPORTSEL		LVDS 4ch
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0209, HCG<<4|0x02);  // FRSEL[1:0]	Set to datarate.		4ch lvds 60fps 12bit
			IMX290_SetAdBits(1);  //ADBIT:12bit
			break;
		case IMX290_FPS_30Hz_DOL3:
			hmax=hbase*2;
			vmax=vbase*4;
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x01|0xE0);    // ODBIT and OPORTSEL		LVDS 4ch
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0209, HCG<<4|0x02);  // FRSEL[1:0]	Set to datarate.		4ch lvds 60fps 12bit
			IMX290_SetAdBits(1);  //ADBIT:12bit
			break;
		case IMX290_FPS_60Hz_DOL2:
			hmax=hbase*1;
			vmax=vbase*2;
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x01|0xE0);    // ODBIT and OPORTSEL		LVDS 4ch
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0209, HCG<<4|0x01);  // FRSEL[1:0]	Set to datarate.		4ch lvds 60fps 12bit
			IMX290_SetAdBits(1);  //ADBIT:12bit
			break;
		case IMX290_FPS_60Hz_DOL3:
			hmax=hbase*1;
			vmax=vbase*4;
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x01|0xE0);    // ODBIT and OPORTSEL		LVDS 4ch
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0209, HCG<<4|0x01);  // FRSEL[1:0]	Set to datarate.		4ch lvds 60fps 12bit
			IMX290_SetAdBits(1);  //ADBIT:12bit
			break;
		case IMX290_FPS_120Hz_DOL2:
			hmax=hbase/2;
			vmax=vbase*2;
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x00|0xF0);    // ODBIT and OPORTSEL		LVDS 8ch
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0209, HCG<<4|0x00);  // FRSEL[1:0]	Set to datarate.		8ch lvds 120fps 10bit
			IMX290_SetAdBits(0);  //ADBIT:10bit
			break;
		case IMX290_FPS_120Hz_DOL3:
			hmax=hbase/2;
			vmax=vbase*4;
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x00|0xF0);    // ODBIT and OPORTSEL		LVDS 8ch
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0209, HCG<<4|0x00);  // FRSEL[1:0]	Set to datarate.		8ch lvds 120fps 10bit
			IMX290_SetAdBits(0);  //ADBIT:10bit
			break;
		default:
			hmax=hbase*2;
			vmax=vbase*1;
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0246, 0x01|0xE0);    // ODBIT and OPORTSEL		LVDS 4ch
			CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0209, HCG<<4|0x02);  // FRSEL[1:0]	Set to datarate.		4ch lvds 30fps 12bit
			IMX290_SetAdBits(1);  //ADBIT:12bit
			break;
	}

	//pal system
#if defined(__VTOTAL_CTRL_PAL__)
	if(GetSS_SensorVoutPAL())  vmax*=1.2f;
#else
	if(GetSS_SensorVoutPAL())  hmax*=1.2f;
#endif

	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x021C, LOBYTE(hmax));	// HMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x021D, HIBYTE(hmax));	// HMAX [7:0] (MSB)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0218, LOBYTE(vmax));	// VMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x0219, HIBYTE(vmax));	// VMAX [7:0] (MSB)
	CMOS_ByteWrite(SPI_IMX290_ADDR, 0x021A, 0x00);			// VMAX [0]   (MSB)
	
}


// temp code to test fps
IMX290_Fps_t IMX290_GetFrameRate(void) {
	return imx290_fps;
}

void IMX290_SetFrameRate(IMX290_Fps_t fps) {
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x01);	// STANDBY[0] - 0: Operation,  1: STANDBY
	IMX290_ChangeFps(fps);
	CMOS_ByteWrite(SPI_IMX136_ADDR, 0x0200, 0x00);	// STANDBY[0] - 0: Operation,  1: STANDBY
	return;
}


#endif	/* __USE_CMOS_IMX290__ */

/*  FILE_END_HERE */
