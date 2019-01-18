// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_IMX274__)
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
//-----------------------------------------------------------------------

void IMX274_Initialize(void)
{	
	UARTprintf("\n\r IMX274_Initialize\n\r");
#if defined(__USE_IMX274_1080P120_MODE__)
	UARTprintf("\n\r [mode3(4) 2x2 binning mode 12bit 1920x1080(1932x1102) lvds 6ch mode 10bit, INCK 74.25Mhz] \n\r");
#elif defined(__USE_IMX274_1080P60_MODE__)
	UARTprintf("\n\r [mode2 2x2 binning mode 12bit 1920x1080(1932x1102) lvds 4ch mode, INCK72Mhz] \n\r");
#elif defined(__USE_IMX274_2160P60_MODE1__)
	UARTprintf("\n\r [All pixel scan mode 10bit 3840x2160p60 lvds 10ch mode, INCK72Mhz] \n\r");
#else
	UARTprintf("\n\r [All pixel scan mode 12bit 3840x2160 lvds 6ch mode, INCK74.25Mhz] \n\r");
#endif		

//-------------------------------------------------------------------------------------
//	Analog stablization period(108page step 1, 2)
//-------------------------------------------------------------------------------------
	//step1(V1 Period)
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0000, 0x1A);		//stanby = 0, stbblogic=1, stbmipi = 1, stbdb=1
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x003E, 0x00);		//SYSMODE = 0
	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0120, 0x80);		//PLRD1 register(see datasheet 44p)
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0121, 0x00);		//PLRD1 register(see datasheet 44p)	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0122, 0x03);		//PLRD2 register(see datasheet 44p)
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0129, 0x68);		//PLRD3 register(see datasheet 44p)	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x012A, 0x03);		//PLRD4 register(see datasheet 44p)	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x012D, 0x02);		//PLRD5 register(see datasheet 44p)	

	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x010B, 0x00);		//Stbpl_ad register, Stbpl_IF register(PLL standby control, Interface and ADconvert)

	//Initialize Communication
	//First Se tStanby=1
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0000, 0x1B);		//stanby = 1, stbblogic=1, stbmipi = 1, stbdb=1		
	//set all register of PSMOVEN, PSLVDS1~4, PLSTMG (datasheet page 47)
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x00EE, 0x01);		// PSMOVEN(default : 0x00)

#if defined(__USE_IMX274_1080P120_MODE__)
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x032C, 0x98);		// PSLVDS1 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x032D, 0x08);		// PSLVDS1 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x034A, 0x98);		// PSLVDS2 set XVS period[XHS] - 32h,  1080p120fps = 2250(with user clock out 74.25Mhz)
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x034B, 0x08);		// PSLVDS2 set XVS period[XHS] - 32h,  2250-0x32h=2200(0x898)
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B6, 0x98);		// PSLVDS3 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B7, 0x08);		// PSLVDS3 set XVS period[XHS] - 32h		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B8, 0x98);		// PSLVDS4 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B9, 0x08);		// PSLVDS4 set XVS period[XHS] - 32h		
/*	 
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x032C, 0x98);		// PSLVDS1 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x032D, 0x08);		// PSLVDS1 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x034A, 0x98);		// PSLVDS2 set XVS period[XHS] - 32h,
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x034B, 0x08);		// PSLVDS2 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B6, 0x98);		// PSLVDS3 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B7, 0x08);		// PSLVDS3 set XVS period[XHS] - 32h		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B8, 0x98);		// PSLVDS4 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B9, 0x08);		// PSLVDS4 set XVS period[XHS] - 32h	
*/
#elif defined(__USE_IMX274_1080P60_MODE__)
#if 1
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x032C, 0x78);		// PSLVDS1 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x032D, 0x08);		// PSLVDS1 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x034A, 0x78);		// PSLVDS2 set XVS period[XHS] - 32h, 1080p60fps = 2200, 1080p50fps = 2640  (see page 83p)	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x034B, 0x08);		// PSLVDS2 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B6, 0x78);		// PSLVDS3 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B7, 0x08);		// PSLVDS3 set XVS period[XHS] - 32h		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B8, 0x78);		// PSLVDS4 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B9, 0x08);		// PSLVDS4 set XVS period[XHS] - 32h	
#else
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x032C, 0xE6);		// PSLVDS1 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x032D, 0x08);		// PSLVDS1 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x034A, 0xE6);		// PSLVDS2 set XVS period[XHS] - 32h, 1080p60fps = 2310, 1080p50fps = 2500  (see page 83p)	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x034B, 0x08);		// PSLVDS2 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B6, 0xE6);		// PSLVDS3 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B7, 0x08);		// PSLVDS3 set XVS period[XHS] - 32h		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B8, 0xE6);		// PSLVDS4 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B9, 0x08);		// PSLVDS4 set XVS period[XHS] - 32h	
#endif
	 //Mode 3 264-32h = 	
	 /*
	 CMOS_ByteWrite(SPI_IMX274_ADDR, 0x032C, 0xB1);		// PSLVDS1 set XVS period[XHS] - 32h	
	 CMOS_ByteWrite(SPI_IMX274_ADDR, 0x032D, 0x08);		// PSLVDS1 set XVS period[XHS] - 32h	
	 CMOS_ByteWrite(SPI_IMX274_ADDR, 0x034A, 0xB1);		// PSLVDS2 set XVS period[XHS] - 32h, UHD30fps = 4550, UHD25fps = 5000  (see page 83p)	
	 CMOS_ByteWrite(SPI_IMX274_ADDR, 0x034B, 0x08);		// PSLVDS2 set XVS period[XHS] - 32h	
	 CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B6, 0xB1);		// PSLVDS3 set XVS period[XHS] - 32h	
	 CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B7, 0x08);		// PSLVDS3 set XVS period[XHS] - 32h		
	 CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B8, 0xB1);		// PSLVDS4 set XVS period[XHS] - 32h	
	 CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B9, 0x08);		// PSLVDS4 set XVS period[XHS] - 32h			
	 */	
#else
	//xvs set same : both mode0 and mode1
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x032C, 0xA6);		// PSLVDS1 set XVS period[XHS] - 32h
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x032D, 0x11);		// PSLVDS1 set XVS period[XHS] - 32h
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x034A, 0xA6);		// PSLVDS2 set XVS period[XHS] - 32h, UHD30fps = 4550, UHD25fps = 5000  (see page 83p)
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x034B, 0x11);		// PSLVDS2 set XVS period[XHS] - 32h
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B6, 0xA6);		// PSLVDS3 set XVS period[XHS] - 32h
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B7, 0x11);		// PSLVDS3 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B8, 0xA6);		// PSLVDS4 set XVS period[XHS] - 32h
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05B9, 0x11);		// PSLVDS4 set XVS period[XHS] - 32h			
#endif
	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x004C, 0x00);		//PLSTMG01	//always set to 0x00	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x004D, 0x03);		//PLSTMG02	//always set to 0x03	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x031C, 0x1A);		//PLSTMG03	//always set to 0x001A	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x031D, 0x00);		//PLSTMG03	//always set to 0x001A	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0502, 0x02);		//PLSTMG04	//always set to 0x02	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0529, 0x0E);		//PLSTMG05	//always set to 0x0E			
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x052A, 0x0E);		//PLSTMG06	//always set to 0x0E	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x052B, 0x0E);		//PLSTMG07	//always set to 0x0E	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0538, 0x0E);		//PLSTMG08	//always set to 0x0E	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0539, 0x0E);		//PLSTMG09	//always set to 0x0E		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0553, 0x00);		//PLSTMG10	//always set to 0x00	

	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x057D, 0x05);		//PLSTMG11	//always set to 0x05
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x057F, 0x05);		//PLSTMG12	//always set to 0x05
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0581, 0x04);		//PLSTMG13	//always set to 0x04
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0583, 0x76);		//PLSTMG14	//always set to 0x76
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0587, 0x01);		//PLSTMG15	//always set to 0x01	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05BB, 0x0E);		//PLSTMG16	//always set to 0x0E	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05BC, 0x0E);		//PLSTMG17	//always set to 0x0E	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05BD, 0x0E);		//PLSTMG18	//always set to 0x0E	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05BE, 0x0E);		//PLSTMG19	//always set to 0x0E	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05BF, 0x0E);		//PLSTMG20	//always set to 0x0E	

	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066E, 0x00);		//PLSTMG21	//always set to 0x00	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066F, 0x00);		//PLSTMG22	//always set to 0x00		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0670, 0x00);		//PLSTMG23	//always set to 0x00		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0671, 0x00);		//PLSTMG24	//always set to 0x00			

	//input  xvs and xhs at this time, isp to sensor(slave sync)
	UARTprintf("\n\r IMX274 XHS = 264(UHD 60fps) 528(UHD 30fps), 364(FHD 60fps), 260(FHD 30fps) \n\r");

	//step2
	Delay_mSec(20);	//delays 10ms or more(1st analog stabilization period)

	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x00E6, 0x01);		//PLL_CKEN = 1
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x00E8, 0x01);		//PACKEN = 1

	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0000, 0x18);		//stanby = 0, stbblogic=0, stbmipi = 1, stbdb=1	

//-------------------------------------------------------------------------------------
//	stanby cancel sequence(108page step 3) (V2 Period)
//-------------------------------------------------------------------------------------
	Delay_mSec(10);	//delays 7ms or more(2nd analog stabilization period)	

	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0001, 0x10);		//CLPSQRST=1, 0x0001[4] = 1 clamp circuit reset
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0009, 0x01);		//DCKRST=1

	//Set read out drive mode register(page 53~54), 
#if defined(__USE_IMX274_1080P120_MODE__)
	//-------------------READOUT MODE3------------------------------------------//
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0003, 0x22);		// [3:0] = STBLVDS,	[7:4] LANESEL	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0004, 0x02);		// MDSEL1	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0005, 0x21);		// MDSEL2
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0006, 0x00);		// MDSEL3	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0007, 0x11);		// MDSEL4		
#elif defined(__USE_IMX274_1080P60_MODE__)
	//-------------------READOUT MODE2------------------------------------------//
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0003, 0x33);		// [3:0] = STBLVDS,	[7:4] LANESEL	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0004, 0x02);		// MDSEL1	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0005, 0x27);		// MDSEL2	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0006, 0x00);		// MDSEL3	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0007, 0x11);		// MDSEL4		
	//-------------------READOUT MODE3------------------------------------------//
	/*	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0003, 0x22);		// [3:0] = STBLVDS,	[7:4] LANESEL	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0004, 0x02);		// MDSEL1	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0005, 0x21);		// MDSEL2//이거 하면 영상 안나옴.	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0006, 0x00);		// MDSEL3	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0007, 0x11);		// MDSEL4		
	*/
#elif defined(__USE_IMX274_2160P60_MODE1__)
	//-------------------READOUT MODE1------------------------------------------//
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0003, 0x00);		// [3:0] = STBLVDS,	[7:4] LANESEL
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0004, 0x00);		// MDSEL1
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0005, 0x01);		// MDSEL2
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0006, 0x00);		// MDSEL3
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0007, 0x02);		// MDSEL4
#else
	//-------------------READOUT MODE0------------------------------------------//
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0003, 0x22);		// [3:0] = STBLVDS,	[7:4] LANESEL
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0004, 0x00);		// MDSEL1
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0005, 0x07);		// MDSEL2
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0006, 0x00);		// MDSEL3
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0007, 0x02);		// MDSEL4
#endif

	//see frame rate adjustment(SVR increase then frame rate reduced)
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x000E, 0x00);		// SVR[7:0](Default : 07h) - Integration Start[7:0]		0~ FFFF see note 2         
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x000F, 0x00);		// SVR[7:0](Default : 07h) - Integration Start[15:8]		0~ FFFF see note 2. 	
	
#if defined(__USE_IMX274_FLIP__)
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x001A, 0x01);		// vertical invert
#else
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x001A, 0x00);		// no invert
#endif

#if defined(__USE_IMX274_1080P120_MODE__)
	//mode 3
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x006B, 0x05);		// MDPLS17	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x00E2, 0x02);		// VCUTMODE //	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0342, 0x0A);		// MDPLS01	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0343, 0x00);		// MDPLS01			
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0344, 0x1A);		// MDPLS02	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0345, 0x00);		// MDPLS02		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x03A6, 0x01);		// MDPLS16	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0528, 0x0E);		// MDPLS03		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0554, 0x00);		// MDPLS04	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0555, 0x01);		// MDPLS05		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0556, 0x01);		// MDPLS06	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0557, 0x01);		// MDPLS07	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0558, 0x01);		// MDPLS08	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0559, 0x00);		// MDPLS09	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x055A, 0x00);		// MDPLS10	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05BA, 0x0E);		// MDPLS11	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066A, 0x1B);		// MDPLS12	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066B, 0x1A);		// MDPLS13	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066C, 0x19);		// MDPLS14	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066D, 0x17);		// MDPLS15	
#elif defined(__USE_IMX274_1080P60_MODE__)
	//mode 2			page 53	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x006B, 0x07);		// MDPLS17	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x00E2, 0x02);		// VCUTMODE 
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0342, 0xFF);		// MDPLS01	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0343, 0x01);		// MDPLS01			
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0344, 0xFF);		// MDPLS02	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0345, 0x01);		// MDPLS02		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x03A6, 0x01);		// MDPLS16	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0528, 0x0F);		// MDPLS03		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0554, 0x00);		// MDPLS04	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0555, 0x00);		// MDPLS05		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0556, 0x00);		// MDPLS06	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0557, 0x00);		// MDPLS07	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0558, 0x00);		// MDPLS08	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0559, 0x1F);		// MDPLS09	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x055A, 0x1F);		// MDPLS10	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05BA, 0x0F);		// MDPLS11	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066A, 0x00);		// MDPLS12	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066B, 0x00);		// MDPLS13	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066C, 0x00);		// MDPLS14	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066D, 0x00);		// MDPLS15		
	
	//mode 3
	/*	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x006B, 0x05);		// MDPLS17	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x00E2, 0x02);		// VCUTMODE //	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0342, 0x0A);		// MDPLS01	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0343, 0x00);		// MDPLS01			
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0344, 0x1A);		// MDPLS02	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0345, 0x00);		// MDPLS02		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x03A6, 0x01);		// MDPLS16	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0528, 0x0E);		// MDPLS03		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0554, 0x00);		// MDPLS04	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0555, 0x01);		// MDPLS05		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0556, 0x01);		// MDPLS06	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0557, 0x01);		// MDPLS07	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0558, 0x01);		// MDPLS08	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0559, 0x00);		// MDPLS09	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x055A, 0x00);		// MDPLS10	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05BA, 0x0E);		// MDPLS11	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066A, 0x1B);		// MDPLS12	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066B, 0x1A);		// MDPLS13	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066C, 0x19);		// MDPLS14	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066D, 0x17);		// MDPLS15	
	*/
#elif defined(__USE_IMX274_2160P60_MODE1__)
	//mode 1
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x006B, 0x05);		// MDPLS17	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x00E2, 0x00);		// VCUTMODE 
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0342, 0x0A);		// MDPLS01	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0343, 0x00);		// MDPLS01			
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0344, 0x16);		// MDPLS02	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0345, 0x00);		// MDPLS02		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x03A6, 0x01);		// MDPLS16	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0528, 0x0E);		// MDPLS03		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0554, 0x1F);		// MDPLS04	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0555, 0x01);		// MDPLS05		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0556, 0x01);		// MDPLS06	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0557, 0x01);		// MDPLS07	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0558, 0x01);		// MDPLS08	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0559, 0x00);		// MDPLS09	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x055A, 0x00);		// MDPLS10	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05BA, 0x0E);		// MDPLS11	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066A, 0x1B);		// MDPLS12	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066B, 0x1A);		// MDPLS13	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066C, 0x19);		// MDPLS14	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066D, 0x17);		// MDPLS15		
#else
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x006B, 0x07);		// MDPLS17
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x00E2, 0x00);		// VCUTMODE

	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0342, 0xFF);		// MDPLS01
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0343, 0x01);		// MDPLS01		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0344, 0xFF);		// MDPLS02
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0345, 0x01);		// MDPLS02	

	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x03A6, 0x01);		// MDPLS16
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0528, 0x0F);		// MDPLS03	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0554, 0x00);		// MDPLS04
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0555, 0x00);		// MDPLS05	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0556, 0x00);		// MDPLS06
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0557, 0x00);		// MDPLS07
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0558, 0x00);		// MDPLS08
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0559, 0x1F);		// MDPLS09
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x055A, 0x1F);		// MDPLS10

	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x05BA, 0x0F);		// MDPLS11
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066A, 0x00);		// MDPLS12
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066B, 0x00);		// MDPLS13
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066C, 0x00);		// MDPLS14
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x066D, 0x00);		// MDPLS15
#endif

#if defined(__USE_TRIGGER_MODE__)
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0008, 0x01);		// SMD   0 : rolling shutter, 1 : trigger shutter
#else
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0008, 0x00);		// SMD   0 : rolling shutter, 1 : trigger shutter
#endif

	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0045, 0x32);		//BLKLevel


	Delay_mSec(40);	//input normal operation (V3 Period)

	//set shutter and gain register

	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x000A, 0x00);		// PGC(0~0x7A5, 27db)  -20log{(2048-PGC[10:0])/2048}
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x000B, 0x00);		// PGC(0~1957, 27db)	
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0012, 0x00);		// DGain[1:0] 0 : 0db, 1 : 6db 2: 12db 3:18db(Set 0~6)
	
	//SHR : 12~(SVR+1)*(4550-4) then 12~4546
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x000C, 0x0C);		// SHR[7:0](Default : 07h) - Integration Start[7:0]		12 ~ (SVR+1) * (number of XHS pulses per frame - 4)   = SHS
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x000D, 0x00);		// SHR[7:0](Default : 07h) - Integration Start[15:8]			
	
//	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x000F, 0x00);		// SPL[7:0](Default : 07h) - 0~FFFFh can be set	=> Sensup?											
//	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0010, 0x00);		// SPL[7:0](Default : 07h) - 					=> Sensup?

	// digital logic standby on/off for imx274 boot problem.. //bh 2017.03.20
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0000, 0x1A);		//stanby = 0, stbblogic=1, stbmipi = 1, stbdb=1		
	CMOS_ByteWrite(SPI_IMX274_ADDR, 0x0000, 0x18);		//stanby = 0, stbblogic=0, stbmipi = 1, stbdb=1		
}

DWORD CMOS_GetVmax(void)
{
	return GetSS_SensorSlave_Vsize();
}

#endif	/* __USE_CMOS_IMX274__ */

/*  FILE_END_HERE */
