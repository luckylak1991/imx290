// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_IMX226__)
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
#if defined(__USE_CMOS_IMX172__)
static void IMX172_DriveMode(void);
#else
static void IMX226_DriveMode(void);
#endif
// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
// imx226(4k cmos sensor) 
// SPI Chip ID = 81 fix
// Chip ID (8bit) + Upper Start Addr(8bit) + Lower Start Addr(8bit) + Nbyte Data
// Imx226.c, We write start Addr, Lower Addr 
// We support Lvds 8ch 12bit mode i51x(mode 2) 
// Other Sony Sensor 
// Chip ID(8bit, this is page register. 02h, 03h, 04h ....) + Start Addr + Nbyter Data. 
//--------------------------------------------------------------------------------------------------------------------------

void IMX226_Initialize(void)
{	
	//////////////////		SPI Chip ID = 81 fix		//////////////////////	
	Delay_mSec(60);	
	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0000, 0x03|0x04);	// STANDBY[0] - 0: Operation,  1: STANDBY, STBLOGIC[1] - 0: Operation,  1: STANDBY(Valid Only Standby[0]='0'), [7:2] default = 1h
	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0001, 0x00);		// DCKRST[0]	0->1 Fixes LVDS clock output phase  (auto 1->0 return)  CLPSQRST[4] after reset 1->0 return auto by datasheet) 	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0002, 0x00);		// SSBRK[0] When changed from 0h to 1h interupt enable  (break mode)

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0008, 0x00);		// Set Default(Default : 00h)

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x000B, 0x08);		// SHR[7:0](Default : 07h) - Integration Start[7:0]		8~ (SVR+1) * (number of XHS pulses per frame - 4)   = SHS
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x000C, 0x00);		// SHR[7:0](Default : 07h) - Integration Start[15:8]			
	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x000D, 0x00);		// SVR[7:0](Default : 07h) - Integration Start[7:0]		0~ FFFF see note 2          min 1                              => Sensup?
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x000E, 0x00);		// SVR[7:0](Default : 07h) - Integration Start[15:8]		0~ FFFF see note 2		                                     => Sensup?
	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x000F, 0x00);		// SPL[7:0](Default : 07h) - 0~FFFFh can be set	=> Sensup?											
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0010, 0x00);		// SPL[7:0](Default : 07h) - 					=> Sensup?

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0011, 0x00);		// DGain[1:0] 0 : 0db, 1 : 6db 2: 12db 3:18db(Set 0~3)
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0012, 0x00);		// FReq[0] 0 : INCKx4 DDR output, 1 : INCK x 2 ddr output


	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0045, 0x32);		// BLKLEVEL (Default : 32h) When 12bit read out mode x4 value 1=4 (50d x 4 = 200d)

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0009, 0x00);		// PGC(Default : 00h) - Analog Gain[7:0]			-- 0 to 1957d (-20log[(2048-pGC[10:0])/2048])
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x000A, 0x00);		// PGC[2:0](Default : 00h) - Analog Gain[10:8]	

#if defined(__USE_IMX226_2160P60_MODE4__) || defined(__USE_IMX226_1080P60_MODE__)
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0352, 0x1E);		// APGC01[7:0] Analog Gain Setting 01			-- dependent PGC(see datasheet 37p)
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0353, 0x00);		// APGC01[8] Analog Gain Setting 01

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0356, 0x1D);		// APGC02[7:0] Analog Gain Setting 02                   -- dependent PGC(see datasheet 37p)
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0357, 0x00);		// APGC02[8] Analog Gain Setting 02
#else
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0352, 0x3C);		// APGC01[7:0] Analog Gain Setting 01			-- dependent PG(mode 0, 2 and 3 is 3Ch fixed)
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0353, 0x00);		// APGC01[8] Analog Gain Setting 01

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0356, 0x3B);		// APGC02[7:0] Analog Gain Setting 02                   -- dependent PGC(mode 0, 2 and 3 is 3B fixed)
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0357, 0x00);		// APGC02[8] Analog Gain Setting 02
#endif
//-------------------------------Sensor Read Out mode Setting. See Data Sheet 40p--------------------------------------
#if defined(__USE_CMOS_IMX226_LVDS_S4__) || defined(__USE_IMX226_1080P60_MODE__)
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0003, 0x33);		// STBLVDS[3:0] 0: 10ch 1:8ch 3: 4ch     CHSEL[6:4]  0: 10ch 1:8ch 3: 4ch  Number of LVDS output channels selection 
#elif defined(__USE_IMX226_4K3KP30_MODE0__)
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0003, 0x11);
#elif defined(__USE_IMX226_2160P60_MODE4__)
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0003, 0x00);
#else
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0003, 0x11);		// STBLVDS[3:0] 0: 10ch 1:8ch 3: 4ch     CHSEL[6:4]  0: 10ch 1:8ch 3: 4ch  Number of LVDS output channels selection 
#endif

#if defined(__USE_IMX226_1080P60_MODE__)
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0004, 0x89);		// MDSEL1(Default : 2Ah)	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0005, 0x4D);		// MDSEL2(Default : 1Ch)
#elif defined(__USE_IMX226_4K3KP30_MODE0__)
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0004, 0x00);		// MDSEL1(Default : 2Ah)	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0005, 0x07);		// MDSEL2(Default : 1Ch)
#elif defined(__USE_IMX226_2160P60_MODE4__)
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0004, 0x80);		// MDSEL1(Default : 2Ah)	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0005, 0x41);		// MDSEL2(Default : 1Ch)
#else
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0004, 0x80);		// MDSEL1(Default : 2Ah)	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0005, 0x47);		// MDSEL2(Default : 1Ch)
#endif
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0006, 0x00);		// MDSEL3(Default : 2Ah)	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0007, 0x00);		// MDSEL4(Default : 1Ch)

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x001A, 0x00);		// MDVREV[0] 0 : normal  1:Vertical Filp	

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0026, 0x74);		// MDSEL13[0] (Default : 74h) Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0027, 0x74);		// MDSEL14[0] (Default : 74h) Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0028, 0x74);		// MDSEL15[0] (Default : 74h) Sensor Read Out mode dependent	

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x007E, 0x20);		// MDSEL5[7:0]  (Default : 00h)   Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x007F, 0x01);		// MDSEL5[15:8] (Default : 00h)   Sensor Read Out mode dependent

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0080, 0x00);           // MDPLS01 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0081, 0x00);           // MDPLS02 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0082, 0x00);           // MDPLS03 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0083, 0x00);           // MDPLS04 (Default Set:0x00) 	 Sensor Read Out mode dependent	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0084, 0x00);           // MDPLS05 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0085, 0x00);           // MDPLS06 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0086, 0x00);           // MDPLS07 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0087, 0x00);           // MDPLS08 (Default Set:0x00) 	 Sensor Read Out mode dependent	

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0095, 0x00);           // MDPLS09 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0096, 0x00);           // MDPLS10 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0097, 0x00);           // MDPLS11 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0098, 0x00);           // MDPLS12 (Default Set:0x00) 	 Sensor Read Out mode dependent	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0099, 0x00);           // MDPLS13 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x009A, 0x00);           // MDPLS14 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x009B, 0x00);           // MDPLS15 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x009C, 0x00);           // MDPLS16 (Default Set:0x00) 	 Sensor Read Out mode dependent

#if defined(__USE_IMX226_1080P60_MODE__)
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00B6, 0x67);		// MDSEL6[7:0]  (Default : 00h)   Sensor Read Out mode dependent
#else
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00B6, 0x47);		// MDSEL6[7:0]  (Default : 00h)   Sensor Read Out mode dependent
#endif
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00B7, 0x00);		// MDSEL7[7:0] (Default : 00h)   Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00B8, 0x00);		// MDSEL8[7:0]  (Default : 00h)   Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00B9, 0x00);		// MDSEL9[7:0] (Default : 00h)   Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00BA, 0x00);		// MDSEL10[7:0] (Default : 00h)   Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00BB, 0x00);		// MDSEL11[7:0] (Default : 00h)   Sensor Read Out mode dependent	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00BC, 0x00);           // MDPLS17 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00BD, 0x00);           // MDPLS18 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00BE, 0x00);           // MDPLS19 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00BF, 0x00);           // MDPLS20 (Default Set:0x00) 	 Sensor Read Out mode dependent	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00C0, 0x00);           // MDPLS21 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00C1, 0x00);           // MDPLS22 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00C2, 0x00);           // MDPLS23 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00C3, 0x00);           // MDPLS24 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00C4, 0x00);           // MDPLS25 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00C5, 0x00);           // MDPLS26 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00C6, 0x00);           // MDPLS27 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00C7, 0x00);           // MDPLS28 (Default Set:0x00) 	 Sensor Read Out mode dependent	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00C8, 0x00);           // MDPLS29 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00C9, 0x00);           // MDPLS30 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00CA, 0x00);           // MDPLS31 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00CB, 0x00);           // MDPLS32 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00CC, 0x00);           // MDPLS33 (Default Set:0x00) 	 Sensor Read Out mode dependent
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x00CE, 0x0E);		// MDSEL12[7:0] (Default : 00h)   Sensor Read Out mode dependent	


//----------------------------------Drive Pulse Timing Setting(Data sheet 39p)----------------------------------------
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x004D, 0x00|0x10);	// PLSTMG01[5:4] Set 1h (Default : 00h) Drive pulse timing setting 01	

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0054, 0x04);		// PLSTMG02[7:0] Set 4h (Default : 05h) Drive pulse timing setting 02

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0057, 0x24);		// PLSTMG03[7:0] Set 24h (Default : 25h) Drive pulse timing setting 03
//--------------------------------------------------------------------------------------------------------------
#if defined(__USE_CMOS_IMX172__)
	IMX172_DriveMode();
#else
	IMX226_DriveMode();
#endif
									
//-------------------------------------------------------------------------------------

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0000, 0x02|0x04);	// STANDBY[0] - 0: Operation,  1: STANDBY, STBLOGIC[1] - 0: Operation,  1: STANDBY(Valid Only Standby[0]='0'), [7:2] default = 1h
	Delay_mSec(2);		
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0000, 0x00|0x04);	// STANDBY[0] - 0: Operation,  1: STANDBY, STBLOGIC[1] - 0: Operation,  1: STANDBY(Valid Only Standby[0]='0'), [7:2] default = 1h
	Delay_mSec(20);		
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0001, 0x11);		// DCKRST[0]	0->1 Fixes LVDS clock output phase  (auto 1->0 return)  CLPSQRST[4] after reset 1->0 return auto by datasheet) 	

}

#if defined(__USE_CMOS_IMX172__)
static void IMX172_DriveMode(void)
{
	UARTprintf("\n\rIMX172_Initialize(UHD 12bit 8ch lvds [MODE2])\n\r");
	UARTprintf("\n\rIMX172_Stanby On\n\r");
	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0222, 0x31);		// PLSTMG11[7:0] Set 30h (Default : 2Ch)  Drive pulse timing setting 11
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0223, 0x01);		// PLSTMG11[8] Set 1h (Default : 01h) 	 Drive pulse timing setting 11

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0358, 0x01);		// PLSTMG00[7:0] Set 01h (Default : 00h)  Drive pulse timing setting 00

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0528, 0x0E);		// PLSTMG01[4:0] Set 0Eh (Default : 0Fh)  Drive pulse timing setting 01
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0529, 0x0E);		// PLSTMG13[4:0] Set 0Eh (Default : 0Fh)  Drive pulse timing setting 13

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x052A, 0x0E);		// PLSTMG02[7:0] Set 0Eh (Default : 0Ch)  Drive pulse timing setting 02
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x052B, 0x0E);		// PLSTMG14[7:0] Set 0Eh (Default : 0Ch)  Drive pulse timing setting 12

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0534, 0x10);		// PLSTMG15[7:0] Set 10h (Default : 00h)  Drive pulse timing setting 15
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x057E, 0x00);		// PLSTMG03[3:0] Set 00h (Default : 03h)  Drive pulse timing setting 03
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x057F, 0x10);		// PLSTMG04[7:0] Set 17h (Default : 10h)  Drive pulse timing setting 04
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0580, 0x0D);		// PLSTMG05[7:0] Set 0Dh (Default : 13h)  Drive pulse timing setting 05
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0581, 0x0D);		// PLSTMG06[7:0] Set 0Dh (Default : 10h)  Drive pulse timing setting 06
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0585, 0x00);		// PLSTMG07[7:0] Set 00h (Default : 4Bh)  Drive pulse timing setting 07
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0586, 0x07);		// PLSTMG07[2:0] Set 07h (Default : 07h)  Drive pulse timing setting 07

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0617, 0x10);		// PLSTMG12[4] Set 10h (Default : 00h)  Drive pulse timing setting 12
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x065C, 0x05);		// PLSTMG08[7:0] Set 07h (Default : 04h)  Drive pulse timing setting 08
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0700, 0x19);		// PLSTMG09[7:0] Set 01h (Default : 18h)  Drive pulse timing setting 09
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0701, 0x19);		// PLSTMG09[4:0] Set 04h (Default : 19h)  Drive pulse timing setting 09
}
#else
static void IMX226_DriveMode(void)
{
#if defined(__USE_IMX226_4K3KP30_MODE0__)
	UARTprintf("\n\rIMX226_Initialize(UHD 12bit 8ch lvds [MODE0])\n\r");
#elif defined(__USE_IMX226_2160P60_MODE4__)
	UARTprintf("\n\rIMX226_Initialize(UHD 12bit 10ch lvds [MODE4])\n\r");
#else
	UARTprintf("\n\rIMX226_Initialize(UHD 12bit 8ch lvds [MODE2])\n\r");
#endif
	UARTprintf("\n\rIMX226_Stanby On\n\r");
	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0210, 0x0A);		// PLSTMG04[7:0] Set 0Ah (Default : 00h)    Drive pulse timing setting 04
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0211, 0x00);		// PLSTMG04[8] Set 0h (Default : 00h)        Drive pulse timing setting 04
	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0212, 0x31);		// PLSTMG05[7:0] Set 31h (Default : FFh)   Drive pulse timing setting 05
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0213, 0x01);		// PLSTMG05[8] Set 1h (Default : 01h) 	 Drive pulse timing setting 05
	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x021C, 0x64);		// PLSTMG22[7:0] Set 64h (Default : 8Bh)   Drive pulse timing setting 22
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x021D, 0x00);		// PLSTMG22[8] Set 0h (Default : 00h) 	  Drive pulse timing setting 22
	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x021E, 0xEF);		// PLSTMG06[7:0] Set EFh (Default : EBh)  Drive pulse timing setting 06
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x021F, 0x00);		// PLSTMG06[8] Set 0h (Default : 01h) 	 Drive pulse timing setting 06

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0222, 0x30);		// PLSTMG23[7:0] Set 30h (Default : 2Ch)  Drive pulse timing setting 23
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0223, 0x01);		// PLSTMG23[8] Set 1h (Default : 01h) 	 Drive pulse timing setting 23

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0313, 0x10);		// PLSTMG07[7:0] Set 10h (Default : 00h)  Drive pulse timing setting 07

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0366, 0x01);		// PLSTMG08[0] Set 01h (Default : 00h)  Drive pulse timing setting 08

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0371, 0x01);		// PLSTMG09[1:0] Set 01h (Default : 02h)  Drive pulse timing setting 09

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0528, 0x0F);		// PLSTMG10[4:0] Set 01h (Default : 11h)  Drive pulse timing setting 10
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0529, 0x0F);		// PLSTMG11[4:0] Set 01h (Default : 11h)  Drive pulse timing setting 11

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x052C, 0x1F);		// PLSTMG12[7:0] Set 1Fh (Default : 00h)  Drive pulse timing setting 12

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x052D, 0x01);		// PLSTMG13[7:0] Set 01h (Default : 59h)  Drive pulse timing setting 13
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x052E, 0x04);		// PLSTMG13[7:0] Set 04h (Default : 02h)  Drive pulse timing setting 13	
	
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0534, 0x10);		// PLSTMG24[7:0] Set 10h (Default : 00h)  Drive pulse timing setting 24

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x057A, 0x0B);		// PLSTMG14[7:0] Set 0Bh (Default : 05h)  Drive pulse timing setting 14
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x057B, 0x00);		// PLSTMG14[0] Set 00h (Default : 00h)  Drive pulse timing setting 14	

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x057D, 0x0A);		// PLSTMG15[4:0] Set 0Ah (Default : 10h)  Drive pulse timing setting 15

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x057E, 0x00);		// PLSTMG16[3:0] Set 01h (Default : 03h)  Drive pulse timing setting 16

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0582, 0x07);		// PLSTMG17[2:0] Set 07h (Default : 01h)  Drive pulse timing setting 17

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0617, 0x10);		// PLSTMG18[4] Set 10h (Default : 00h)  Drive pulse timing setting 18

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0650, 0x00);		// PLSTMG19[7:0] Set 00h (Default : 01h)  Drive pulse timing setting 19

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x065C, 0x07);		// PLSTMG20[7:0] Set 07h (Default : 03h)  Drive pulse timing setting 20

	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0700, 0x19);		// PLSTMG21[7:0] Set 01h (Default : 18h)  Drive pulse timing setting 21
	CMOS_ByteWrite(SPI_IMX226_ADDR, 0x0701, 0x19);		// PLSTMG21[4:0] Set 04h (Default : 19h)  Drive pulse timing setting 21
}
#endif

#endif	/* __USE_CMOS_IMX226__ */

/*  FILE_END_HERE */
