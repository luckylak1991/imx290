// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_IMX326__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
//#define IMX326_INCK_36Mhz
//#define IMX326_INCK_24Mhz

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

void IMX326_Initialize(void)
{	
	UARTprintf("\n\r IMX326_Initialize\n\r");
#if defined(__USE_IMX326_2592x1944_MODE2__)
	UARTprintf("\n\r [4:3 crop mode 12bit 2592x1944 lvds 6ch mode2, osc72Mhz] \n\r");
#elif defined(__USE_IMX326_3072x1728_MODE4__)
	UARTprintf("\n\r [16:9 crop mode 12bit 3072x1728 lvds 6ch mode4, osc72Mhz] \n\r");
#elif defined(__USE_IMX326_2160x2160_MODE6__)
	UARTprintf("\n\r [1:1 crop mode 12bit 2160x2160 lvds 6ch mode6, osc72Mhz] \n\r");
#else
	UARTprintf("\n\r [All pixel scan mode 12bit 3072x2160 lvds 6ch mode0, osc72Mhz] \n\r");
#endif		

//-------------------------------------------------------------------------------------
//	Analog stablization period(108page step 1, 2)
//-------------------------------------------------------------------------------------
	//step1(V1 Period)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0000, 0x1A);		//stanby = 0, stbblogic=1, stbmipi = 1, stbdb=1
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x003E, 0x00);		//SYSMODE = 0
#if defined(IMX326_INCK_36Mhz)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0120, 0x80);		//PLRD1 register(see datasheet 44p)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0121, 0x00);		//PLRD1 register(see datasheet 44p)	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0122, 0x02);		//PLRD2 register(see datasheet 44p)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0129, 0x68);		//PLRD3 register(see datasheet 44p)	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x012A, 0x02);		//PLRD4 register(see datasheet 44p)	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x012D, 0x02);		//PLRD5 register(see datasheet 44p)	
#elif defined(IMX326_INCK_24Mhz)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0120, 0xC0);		//PLRD1 register(see datasheet 44p)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0121, 0x00);		//PLRD1 register(see datasheet 44p)	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0122, 0x02);		//PLRD2 register(see datasheet 44p)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0129, 0x9C);		//PLRD3 register(see datasheet 44p)	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x012A, 0x02);		//PLRD4 register(see datasheet 44p)	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x012D, 0x02);		//PLRD5 register(see datasheet 44p)		
#else
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0120, 0x80);		//PLRD1 register(see datasheet 44p)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0121, 0x00);		//PLRD1 register(see datasheet 44p)	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0122, 0x03);		//PLRD2 register(see datasheet 44p)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0129, 0x68);		//PLRD3 register(see datasheet 44p)	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x012A, 0x03);		//PLRD4 register(see datasheet 44p)	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x012D, 0x02);		//PLRD5 register(see datasheet 44p)	
#endif

	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x010B, 0x00);		//Stbpl_ad register, Stbpl_IF register(PLL standby control, Interface and ADconvert)

	//Initialize Communication
	//First Se tStanby=1
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0000, 0x1B);		//stanby = 1, stbblogic=1, stbmipi = 1, stbdb=1		
	//set all register of PSMOVEN, PSLVDS1~4, PLSTMG (datasheet page 53)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00EE, 0x01);		// PSMOVEN(default : 0x00)

	//UHD29.97fps = 4550, UHD25fps = 5000  (see page 76,77p)
	//(16:9 mode 0) 12bit 3072x2160 30fps = 4800  			
	//(4:3 mode 2) 12bit 2592x1944
	//(16:9 mode 4) 12bit 3072x1728 30fps = 4800  			
	//(1:1 mode 6) 12bit 2160x2160

	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x032C, 0xA0);		// PSLVDS1 set XVS period[XHS] - 32h
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x032D, 0x12);		// PSLVDS1 set XVS period[XHS] - 32h
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x034A, 0xA0);		// PSLVDS2 set XVS period[XHS] - 32h
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x034B, 0x12);		// PSLVDS2 set XVS period[XHS] - 32h
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x05B6, 0xA0);		// PSLVDS3 set XVS period[XHS] - 32h
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x05B7, 0x12);		// PSLVDS3 set XVS period[XHS] - 32h	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x05B8, 0xA0);		// PSLVDS4 set XVS period[XHS] - 32h
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x05B9, 0x12);		// PSLVDS4 set XVS period[XHS] - 32h			

	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x004C, 0x00);		//PLSTMG01
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x004D, 0x03);		//PLSTMG02	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x031C, 0x1A);		//PLSTMG03	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x031D, 0x00);		//PLSTMG03
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0502, 0x02);		//PLSTMG04
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0529, 0x0E);		//PLSTMG05		
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x052A, 0x0E);		//PLSTMG06
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x052B, 0x0E);		//PLSTMG07
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0538, 0x0E);		//PLSTMG08
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0539, 0x0E);		//PLSTMG09		
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0553, 0x00);		//PLSTMG10

	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x057D, 0x05);		//PLSTMG11
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x057F, 0x05);		//PLSTMG12	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0581, 0x04);		//PLSTMG13	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0583, 0x76);		//PLSTMG14
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0587, 0x01);		//PLSTMG15
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x05BB, 0x0E);		//PLSTMG16		
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x05BC, 0x0E);		//PLSTMG17
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x05BD, 0x0E);		//PLSTMG18
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x05BE, 0x0E);		//PLSTMG19
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x05BF, 0x0E);		//PLSTMG20

	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x066E, 0x00);		//PLSTMG21	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x066F, 0x00);		//PLSTMG22	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0670, 0x00);		//PLSTMG23	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0671, 0x00);		//PLSTMG24		

	//input  xvs and xhs at this time, isp to sensor(slave sync)
	UARTprintf("\n\r IMX326 Minimum XHS = 493(UHD), 260(FHD) \n\r");

	//step2
	Delay_mSec(20);	//delays 10ms or more(1st analog stabilization period)

	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00E6, 0x01);		//PLL_CKEN = 1
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00E8, 0x01);		//PACKEN = 1

	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0000, 0x18);		//stanby = 0, stbblogic=0, stbmipi = 1, stbdb=1	

//-------------------------------------------------------------------------------------
//	stanby cancel sequence(108page step 3) (V2 Period)
//-------------------------------------------------------------------------------------
	Delay_mSec(10);	//delays 7ms or more(2nd analog stabilization period)	

	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0001, 0x10);		//CLPSQRST=1, 0x0001[4] = 1 clamp circuit reset
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0009, 0x01);		//DCKRST=1

	//Set read out drive mode register(page 53~54), 
	//-------------------READOUT MODE0------------------------------------------//
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0003, 0x22);		// [3:0] = STBLVDS,	[7:4] LANESEL
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0004, 0x00);		// MDSEL1
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0005, 0x07);		// MDSEL2
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0006, 0x00);		// MDSEL3
	#if defined(__USE_IMX326_2592x1944_MODE2__) || defined(__USE_IMX326_3072x1728_MODE4__)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0007, 0xA2);		// MDSEL4
	#else
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0007, 0x02);		// MDSEL4
	#endif
	
	//see frame rate adjustment(SVR increase then frame rate reduced)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x000E, 0x00);		// SVR[7:0](Default : 07h) - Integration Start[7:0]		0~ FFFF see note 2         
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x000F, 0x00);		// SVR[7:0](Default : 07h) - Integration Start[15:8]		0~ FFFF see note 2. 	
	
#if defined(__USE_IMX326_FLIP__)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x001A, 0x01);		// vertical invert
#else
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x001A, 0x00);		// no invert
#endif

	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0037, 0x01);		// HTRIMMING _EN

	#if defined(__USE_IMX326_2592x1944_MODE2__) 
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0038, 0x70);		// HTRIMMING _START 
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0039, 0x02);
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x003A, 0xA8);		// HTRIMMING _END 
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x003B, 0x0C);	
	#elif defined(__USE_IMX326_2160x2160_MODE6__)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0038, 0x48);		// HTRIMMING _START 
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0039, 0x03);
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x003A, 0xD0);		// HTRIMMING _END 
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x003B, 0x0B);		
	#else
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0038, 0x80);		// HTRIMMING _START 
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0039, 0x01);
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x003A, 0x98);		// HTRIMMING _END 
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x003B, 0x0D);		
	#endif


	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x006B, 0x07);		// MDPLS17


	#if defined(__USE_IMX326_2592x1944_MODE2__) 
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00DD, 0x01);		// VWIDCUTEN
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00DE, 0x6C);		// VWIDCUT
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00DF, 0x00);		// VWIDCUT
	#if defined(__USE_IMX326_FLIP__)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00E0, 0xCA);		// VWINPOS	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00E1, 0x0F);		// VWINPOS	
	#else
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00E0, 0x36);		// VWINPOS	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00E1, 0x00);		// VWINPOS	
	#endif
	#elif defined(__USE_IMX326_3072x1728_MODE4__)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00DD, 0x01);		// VWIDCUTEN
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00DE, 0xD8);		// VWIDCUT
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00DF, 0x00);		// VWIDCUT
	#if defined(__USE_IMX326_FLIP__)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00E0, 0x94);		// VWINPOS	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00E1, 0x0F);		// VWINPOS	
	#else
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00E0, 0x6C);		// VWINPOS	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00E1, 0x00);		// VWINPOS	
	#endif
	#else
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00DD, 0x00);		// VWIDCUTEN
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00DE, 0x00);		// VWIDCUT
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00DF, 0x00);		// VWIDCUT
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00E0, 0x00);		// VWINPOS	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00E1, 0x00);		// VWINPOS		
	#endif

	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x00E2, 0x00);		// VCUTMODE		

	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0342, 0xFF);		// MDPLS01
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0343, 0x01);		// MDPLS01		
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0344, 0xFF);		// MDPLS02
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0345, 0x01);		// MDPLS02	

	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x03A6, 0x01);		// MDPLS16
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0528, 0x0F);		// MDPLS03	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0554, 0x00);		// MDPLS04
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0555, 0x00);		// MDPLS05	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0556, 0x00);		// MDPLS06
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0557, 0x00);		// MDPLS07
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0558, 0x00);		// MDPLS08
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0559, 0x1F);		// MDPLS09
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x055A, 0x1F);		// MDPLS10

	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x05BA, 0x0F);		// MDPLS11
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x066A, 0x00);		// MDPLS12
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x066B, 0x00);		// MDPLS13
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x066C, 0x00);		// MDPLS14
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x066D, 0x00);		// MDPLS15

#if defined(__USE_TRIGGER_MODE__)
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0008, 0x01);		// SMD   0 : rolling shutter, 1 : trigger shutter
#else
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0008, 0x00);		// SMD   0 : rolling shutter, 1 : trigger shutter
#endif

	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0045, 0x32);		//BLKLevel


	Delay_mSec(40);	//input normal operation (V3 Period)

	//set shutter and gain register

	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x000A, 0x00);		// PGC(0~0x7A5, 27db)  -20log{(2048-PGC[10:0])/2048}
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x000B, 0x00);		// PGC(0~1957, 27db)	
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0012, 0x00);		// DGain[1:0] 0 : 0db, 1 : 6db 2: 12db 3:18db(Set 0~6)
	
	//SHR : 12~(SVR+1)*(4550-4) then 12~4546
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x000C, 0x0C);		// SHR[7:0](Default : 07h) - Integration Start[7:0]		12 ~ (SVR+1) * (number of XHS pulses per frame - 4)   = SHS
	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x000D, 0x00);		// SHR[7:0](Default : 07h) - Integration Start[15:8]			
	
//	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x000F, 0x00);		// SPL[7:0](Default : 07h) - 0~FFFFh can be set	=> Sensup?											
//	CMOS_ByteWrite(SPI_IMX326_ADDR, 0x0010, 0x00);		// SPL[7:0](Default : 07h) - 					=> Sensup?

	
}

#endif	/* __USE_CMOS_IMX326__ */

/*  FILE_END_HERE */
