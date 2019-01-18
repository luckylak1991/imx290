// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_AR0331__)
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
static void AR0331_Parallel_Linear_Nomal_1536p20(void);

static void AR0331_LVDS_Linear_1080p30(void);
static void AR0331_LVDS_Linear_1080p60(void);
static void AR0331_LVDS_HDR_1080p30(void);
static void AR0331_LVDS_COMP_1080p30(void);
static void AR0331_LVDS_Linear_1536p30(void);
// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void AR0331_Initialize(void)
{
	UARTprintf("AR0331_Initialize\n\r");

// ------------	Sensor Output Mode Selection	------------//
#if defined (__USE_CMOS_AR0331_TTL__)
#if defined (__USE_AR0331_NORMAL_MODE__)
	AR0331_Parallel_Linear_Nomal_1080p30();		//TTL_nomal
#elif defined (__USE_AR0331_NORMAL_3MegaMODE__)	
	AR0331_Parallel_Linear_Nomal_1536p20();
#elif defined (__USE_AR0331_NORMAL_ALTM_MODE__)
	AR0331_Parallel_Linear_ALTM_1080p30();
#elif defined (__USE_AR0331_HDR_ALTM_MODE__)
	AR0331_Parallel_HDR_ALTM_1080p30();		//TTL HDR
#endif

#else // LVDS mode

#if defined (__USE_AR0331_NORMAL_MODE__)
	#if defined (__USE_CMOS_AR0331_LVDS_S4_60P__)
	AR0331_LVDS_Linear_1080p60();		//LVDS nomal - not tested
	#else	
	AR0331_LVDS_Linear_1080p30();		//LVDS nomal - not tested
	#endif
#elif defined (__USE_AR0331_NORMAL_3MegaMODE__)	
	AR0331_LVDS_Linear_1536p30();
#elif defined (__USE_AR0331_HDR_ALTM_MODE__)
 	AR0331_LVDS_HDR_1080p30();		//LVDS HDR - not tested
#endif
 
#endif	//defined (__USE_CMOS_AR0331_TTL__)

	//-----------------------select only one initial function-------------//
#if 1
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3012, 0x0117);	// COARSE_INTEGRATION_TIME  : for demo(we will be dimiss)
#else
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3012, 1054);	// COARSE_INTEGRATION_TIME  : for demo(we will be dimiss)
#endif
	
}


void AR0331_WDR_Initialize(void)
{
	UARTprintf("AR0331_WDR_Initialize\n\r");

// ------------	Sensor Output Mode Selection	------------//
#if defined (__USE_CMOS_AR0331_TTL__)
#if defined (__USE_AR0331_NORMAL_MODE__)
	AR0331_Parallel_HDR_COMP_1080p30();		//TTL
#endif

#else // LVDS mode

#if defined (__USE_AR0331_NORMAL_MODE__)
	AR0331_LVDS_COMP_1080p30();		//LVDS HDR - not tested
#endif
 
#endif	//defined (__USE_CMOS_AR0331_TTL__)

}


void AR0331_Parallel_Linear_Nomal_1080p30(void)
{
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10D8);	// reset (stream stop)
	
	Delay_mSec(200);	

	AR0331_Linear_Sequencer_Load();

//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10D8);	// reset (stream stop)
	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30B0, 0x0000);	// DIGITAL_TEST
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30BA, 0x06EC);	// DIGITAL_CTRL
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AC, 0x0C0C);	// DATA_FORMAT_BITS

	PLL_settings_LVDS(OFF);									// Set Parallel

//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x00E4);	// Y_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x00DC);	// Y_ADDR_START
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x0042);	// X_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x003A);	// X_ADDR_START
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x0523);	// Y_ADDR_END
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x052B);	// Y_ADDR_END
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07C9);	// X_ADDR_END
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07D1);	// X_ADDR_END
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07E5);	// X_ADDR_END	

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300A, 0x0465);	// FRAME_LENGTH_LINES
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300C, 0x044C);	// LINE_LENGTH_PCK
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3012, 0x0416);	// COARSE_INTEGRATION_TIME
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A2, 0x0001);	// X_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A6, 0x0001);	// Y_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3040, 0x0000);	// READ_MODE
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AE, 0x0301);	// SERIAL_FORMAT

	HDR_Mode(0);										// Linear Mode

	AR0331_Analog_Settings();								// Set Analog Parameter

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3ED2, 0x9F46);	// DAC_LD_6_7
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10DC);	// reset (stream start)	

}

static void AR0331_Parallel_Linear_Nomal_1536p20(void)
{
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10D8);	// reset (stream stop)
	
	Delay_mSec(200);	

	AR0331_Linear_Sequencer_Load();

//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10D8);	// reset (stream stop)
	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30B0, 0x0000);	// DIGITAL_TEST
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30BA, 0x06EC);	// DIGITAL_CTRL
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AC, 0x0C0C);	// DATA_FORMAT_BITS

	PLL_settings_LVDS(OFF);									// Set Parallel

//	aptina given
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x0006);	// Y_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x0006);	// X_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x0609);	// Y_ADDR_END
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x0805);	// X_ADDR_END	

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300A, 0x0625);	// FRAME_LENGTH_LINES
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300C, 0x049C);	// LINE_LENGTH_PCK
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3012, 0x046C);	// COARSE_INTEGRATION_TIME
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A2, 0x0001);	// X_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30AE, 0x0001);	// X_ODD_INC_CB
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A6, 0x0001);	// Y_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A8, 0x0001);	// Y_ODD_INC_CB
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3040, 0x0000);	// READ_MODE
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AE, 0x0301);	// SERIAL_FORMAT

	HDR_Mode(0);										// Linear Mode

	AR0331_Analog_Settings();								// Set Analog Parameter

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3ED2, 0x9F46);	// DAC_LD_6_7
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10DC);	// reset (stream start)	

}

void AR0331_Parallel_Linear_ALTM_1080p30(void)
{
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10D8);	// reset (stream stop)
	
	Delay_mSec(200);	

//	AR0331_Linear_Sequencer_Load();
	AR0331_HDR_Sequencer_Load();

//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10D8);	// reset (stream stop)
	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30B0, 0x0000);	// DIGITAL_TEST
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30BA, 0x06EC);	// DIGITAL_CTRL
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AC, 0x0C0C);	// DATA_FORMAT_BITS

	PLL_settings_LVDS(OFF);									// Set Parallel

//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x00E4);	// Y_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x00DC);	// Y_ADDR_START
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x0042);	// X_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x003A);	// X_ADDR_START
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x0523);	// Y_ADDR_END
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x052B);	// Y_ADDR_END
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07C9);	// X_ADDR_END
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07D1);	// X_ADDR_END
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07E5);	// X_ADDR_END	

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300A, 0x0465);	// FRAME_LENGTH_LINES
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300C, 0x044C);	// LINE_LENGTH_PCK
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3012, 0x0416);	// COARSE_INTEGRATION_TIME
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A2, 0x0001);	// X_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A6, 0x0001);	// Y_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3040, 0x0000);	// READ_MODE
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AE, 0x0301);	// SERIAL_FORMAT

	HDR_Mode(1);										// Linear Mode

	AR0331_Analog_Settings();								// Set Analog Parameter

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3ED2, 0x9F46);	// DAC_LD_6_7
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10DC);	// reset (stream start)	

	Delay_mSec(33);
	
}



void AR0331_Parallel_HDR_COMP_1080p30(void)
{
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x0001);	// reset (s/w reset)
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10D8);	// reset (stream stop)
	Delay_mSec(200);	

	AR0331_Linear_Sequencer_Load();
//	AR0331_HDR_Sequencer_Load();

//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10D8);	// reset (stream stop)
	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30B0, 0x0000);	// DIGITAL_TEST
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30BA, 0x07EC);	// DIGITAL_CTRL
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AC, 0x100C);	// DATA_FORMAT_BITS

	PLL_settings_LVDS(OFF);									// Set Parallel

//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x00E4);	// Y_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x00DC);	// Y_ADDR_START
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x0042);	// X_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x003A);	// X_ADDR_START
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x0523);	// Y_ADDR_END
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x052B);	// Y_ADDR_END
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07C9);	// X_ADDR_END
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07D1);	// X_ADDR_END
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07E5);	// X_ADDR_END	

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300A, 0x0465);	// FRAME_LENGTH_LINES
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300C, 0x044C);	// LINE_LENGTH_PCK
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3012, 0x03F4);	// COARSE_INTEGRATION_TIME
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A2, 0x0001);	// X_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A6, 0x0001);	// Y_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3040, 0x0000);	// READ_MODE

	HDR_Mode(2);										// HDR Mode

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AE, 0x0301);	// SERIAL_FORMAT

	AR0331_Analog_Settings();								// Set Analog Parameter

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3ED2, 0xBF46);	// DAC_LD_6_7
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10DC);	// reset (stream start)	

	Delay_mSec(33);

}


void AR0331_Parallel_HDR_ALTM_1080p30(void)
{
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10D8);	// reset (stream stop)
	
	Delay_mSec(200);	

	AR0331_HDR_Sequencer_Load();

//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10D8);	// reset (stream stop)
	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30B0, 0x0000);	// DIGITAL_TEST
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30BA, 0x07EC);	// DIGITAL_CTRL
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AC, 0x100C);	// DATA_FORMAT_BITS

	PLL_settings_LVDS(OFF);									// Set Parallel

//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x00E4);	// Y_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x00DC);	// Y_ADDR_START
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x0042);	// X_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x003A);	// X_ADDR_START
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x0523);	// Y_ADDR_END
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x052B);	// Y_ADDR_END
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07C9);	// X_ADDR_END
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07D1);	// X_ADDR_END
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07E5);	// X_ADDR_END	

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300A, 0x0465);	// FRAME_LENGTH_LINES
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300C, 0x044C);	// LINE_LENGTH_PCK
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3012, 0x03F4);	// COARSE_INTEGRATION_TIME
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A2, 0x0001);	// X_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A6, 0x0001);	// Y_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3040, 0x0000);	// READ_MODE
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AE, 0x0301);	// SERIAL_FORMAT

	HDR_Mode(3);										// HDR Mode


	AR0331_Analog_Settings();								// Set Analog Parameter

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3ED2, 0xBF46);	// DAC_LD_6_7
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10DC);	// reset (stream start)

	Delay_mSec(33);

}
//----------------------------------------------------------------------------

static void AR0331_LVDS_Linear_1536p30(void)
{
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x0058);	// reset (stream stop)
	
	Delay_mSec(200);	

	AR0331_Linear_Sequencer_Load();	
	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30B0, 0x0000);	// DIGITAL_TEST
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30BA, 0x06EC);	// DIGITAL_CTRL
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AC, 0x0C0C);	// DATA_FORMAT_BITS

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C6, 0x8402);	// HISPI_CONTROL_STATUS (default : 0x8000)
#if defined(__USE_CMOS_AR0331_LVDS_S4__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0x8000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0x9000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xA000);	// hispi_timing
// 	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xB000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xC000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xD000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xE000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xF000);	// hispi_timing
#endif	
#if defined(__USE_CMOS_AR0331_LVDS_S2__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0x8000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0x9000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xA000);	// hispi_timing
// 	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xB000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xC000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xD000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xE000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xF000);	// hispi_timing
#endif

#if defined(__USE_CMOS_AR0331_LVDS_S4__)
	PLL_settings_LVDS(1);									// Set PLL to LVDS 4ch mode
#elif defined(__USE_CMOS_AR0331_LVDS_S2__)
	PLL_settings_LVDS(2);									// Set PLL to LVDS 2ch mode
#endif

#if defined(__USE_CMOS_AR0331_LVDS_S4__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AE, 0x0304);	// SERIAL_FORMAT
#elif defined(__USE_CMOS_AR0331_LVDS_S2__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AE, 0x0302);	// SERIAL_FORMAT
#endif
	
//	aptina given
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x0004);	// Y_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x0006);	// X_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x0609);	// Y_ADDR_END  2053+6
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x0809);	// X_ADDR_END
	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300A, 1578);	// FRAME_LENGTH_LINES(fps:23hz)
#if defined(__USE_CMOS_AR0331_LVDS_S4__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300C, 2064);	// LINE_LENGTH_PCK
#else
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300C, 1032);	// LINE_LENGTH_PCK
#endif	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3012, 0x0624);	// COARSE_INTEGRATION_TIME
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A2, 0x0001);	// X_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A6, 0x0001);	// Y_ODD_INC

	HDR_Mode(0);										// HDR Mode

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3082, 0x000D);	//OPERATION_MODE_CTRL = 13
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3084, 0x000D);	//OPERATION_MODE_CTRL_CB = 13

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x308C, 0x0004);	// Y_ADDR_START_CB = 4
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x308A, 0x0006);	// X_ADDR_START_CB = 6
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3090, 0x0609);	// Y_ADDR_END_CB = 1539+6
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x308E, 0x0805);	// X_ADDR_END_CB = 2053
	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30AA, 0x0625);	// FRAME_LENGTH_LINES_CB = 1573
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x303E, 0x0625);	// LINE_LENGTH_PCK_CB = 1573
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3016, 0x0624);	// COARSE_INTEGRATION_TIME_CB = 1572
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30AE, 0x0001);	// X_ODD_INC_CB
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A8, 0x0001);	// Y_ODD_INC_CB

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x306E, 0x9010);	// Data Path Select

	AR0331_Analog_Settings();		

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3ED2, 0x9F46);	// DAC_LD_6_7

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3040, 0x0000);	// READ_MODE
	//CMOS_WordWrite(I2C_AR0331_ADDR, 0x3070, 0x0002);	// test_pattern_mode (0: off, 1: solid color, 2: color bar, 3: gray color bar)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x318E, 0x0000);	// HDR_MC_CTRL3 = 0
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x005C);	// reset (stream start)	

}

//----------------------------------------------------------------------------

static void AR0331_LVDS_Linear_1080p30(void)
{
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x0058);	// reset (stream stop)
	
	Delay_mSec(200);	

	AR0331_Linear_Sequencer_Load();
	
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x0058);	// reset (stream stop)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30B0, 0x0000);	// DIGITAL_TEST
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30BA, 0x06EC);	// DIGITAL_CTRL
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AC, 0x0C0C);	// DATA_FORMAT_BITS

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C6, 0x8402);	// HISPI_CONTROL_STATUS (default : 0x8000)
#if defined(__USE_CMOS_AR0331_LVDS_S4__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0x8000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0x9000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xA000);	// hispi_timing
// 	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xB000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xC000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xD000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xE000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xF000);	// hispi_timing
#endif	
#if defined(__USE_CMOS_AR0331_LVDS_S2__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0x8000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0x9000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xA000);	// hispi_timing
// 	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xB000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xC000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xD000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xE000);	// hispi_timing
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0xF000);	// hispi_timing
#endif

#if defined(__USE_CMOS_AR0331_LVDS_S4__)
	PLL_settings_LVDS(1);									// Set PLL to LVDS 4ch mode
#elif defined(__USE_CMOS_AR0331_LVDS_S2__)
	PLL_settings_LVDS(2);									// Set PLL to LVDS 2ch mode
#endif
	
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x00E4);	// Y_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x00DC);	// Y_ADDR_START
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x0042);	// X_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x003A);	// X_ADDR_START
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x0523);	// Y_ADDR_END
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x052B);	// Y_ADDR_END
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07C9);	// X_ADDR_END
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07D1);	// X_ADDR_END
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07E5);	// X_ADDR_END	

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300A, 0x0465);	// FRAME_LENGTH_LINES
#if defined(__USE_CMOS_AR0331_LVDS_S4__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300C, 0x0898);	// LINE_LENGTH_PCK
#else
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300C, 0x044C);	// LINE_LENGTH_PCK
#endif	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3012, 0x0464);	// COARSE_INTEGRATION_TIME
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A2, 0x0001);	// X_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A6, 0x0001);	// Y_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3040, 0x0000);	// READ_MODE
	
	HDR_Mode(0);										// HDR Mode

#if defined(__USE_CMOS_AR0331_LVDS_S4__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AE, 0x0304);	// SERIAL_FORMAT
#elif defined(__USE_CMOS_AR0331_LVDS_S2__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AE, 0x0302);	// SERIAL_FORMAT
#endif
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x306E, 0x9010);	// Data Path Select

	AR0331_Analog_Settings();		

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3ED2, 0x9F46);	// DAC_LD_6_7

	//CMOS_WordWrite(I2C_AR0331_ADDR, 0x3070, 0x0002);	// test_pattern_mode (0: off, 1: solid color, 2: color bar, 3: gray color bar)
	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x005C);	// reset (stream start)	

}

static void AR0331_LVDS_Linear_1080p60(void)
{
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x0058);	// reset (stream stop)
	
	Delay_mSec(200);	

	AR0331_Linear_Sequencer_Load();
	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x0058);	// reset (stream stop)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30B0, 0x0000);	// DIGITAL_TEST
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30BA, 0x06EC);	// DIGITAL_CTRL
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AC, 0x0C0C);	// DATA_FORMAT_BITS

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C6, 0x8402);	// HISPI_CONTROL_STATUS (default : 0x8000)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0x8000);	// hispi_timing
	PLL_settings_LVDS(ON);									// Set LVDS
	
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x00E4);	// Y_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x00DC);	// Y_ADDR_START
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x0042);	// X_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x003A);	// X_ADDR_START
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x0523);	// Y_ADDR_END
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x052B);	// Y_ADDR_END
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07C9);	// X_ADDR_END
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07D1);	// X_ADDR_END
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07E5);	// X_ADDR_END	

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300A, 0x0465);	// FRAME_LENGTH_LINES
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300C, 0x044C);	// LINE_LENGTH_PCK
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3012, 0x0438);	// COARSE_INTEGRATION_TIME
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A2, 0x0001);	// X_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A6, 0x0001);	// Y_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3040, 0x0000);	// READ_MODE
	
	HDR_Mode(0);										// HDR Mode
	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AE, 0x0304);	// SERIAL_FORMAT
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x306E, 0x9010);	// Data Path Select

	AR0331_Analog_Settings();		

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3ED2, 0x9F46);	// DAC_LD_6_7

	//CMOS_WordWrite(I2C_AR0331_ADDR, 0x3070, 0x0002);	// test_pattern_mode (0: off, 1: solid color, 2: color bar, 3: gray color bar)
	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x005C);	// reset (stream start)	

}


static void AR0331_LVDS_COMP_1080p30(void)
{
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x0058);	// reset (stream stop)
		
	Delay_mSec(200);	

	AR0331_HDR_Sequencer_Load();
	
	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30B0, 0x0000);	// DIGITAL_TEST
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30BA, 0x07EC);	// DIGITAL_CTRL
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AC, 0x100C);	// DATA_FORMAT_BITS

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C6, 0x8402);	// HISPI_CONTROL_STATUS (default : 0x8000)
#if defined(__USE_CMOS_AR0331_LVDS_S4__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0x8000);	// hispi_timing
#endif	
#if defined(__USE_CMOS_AR0331_LVDS_S2__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0x8000);	// hispi_timing
#endif
	
#if defined(__USE_CMOS_AR0331_LVDS_S4__)
	PLL_settings_LVDS(1);									// Set PLL to LVDS 4ch mode
#elif defined(__USE_CMOS_AR0331_LVDS_S2__)
	PLL_settings_LVDS(2);									// Set PLL to LVDS 2ch mode
#endif
	
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x00E4);	// Y_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x00DC);	// Y_ADDR_START
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x0042);	// X_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x003A);	// X_ADDR_START
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x0523);	// Y_ADDR_END
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x052B);	// Y_ADDR_END
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07C9);	// X_ADDR_END
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07D1);	// X_ADDR_END
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07E5);	// X_ADDR_END	

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300A, 0x08A1);	// FRAME_LENGTH_LINES
#if defined(__USE_CMOS_AR0331_LVDS_S4__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300C, 0x08C0);	// LINE_LENGTH_PCK
#else
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300C, 0x0460);	// LINE_LENGTH_PCK
#endif		
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300C, 0x0460);	// LINE_LENGTH_PCK
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3012, 0x0467);	// COARSE_INTEGRATION_TIME
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A2, 0x0001);	// X_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A6, 0x0001);	// Y_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3040, 0x0000);	// READ_MODE
	
	HDR_Mode(2);										// HDR Mode
	
#if defined(__USE_CMOS_AR0331_LVDS_S4__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AE, 0x0304);	// SERIAL_FORMAT
#elif defined(__USE_CMOS_AR0331_LVDS_S2__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AE, 0x0302);	// SERIAL_FORMAT
#endif
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x306E, 0x9010);	// Data Path Select

	AR0331_Analog_Settings();		

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3ED2, 0x9546);	// DAC_LD_6_7
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x005C);	// reset (stream start)
}

static void AR0331_LVDS_HDR_1080p30(void)
{
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10D8);	// reset (stream stop)
	
	Delay_mSec(200);	

	AR0331_HDR_Sequencer_Load();
	
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x0058);	// reset (stream stop)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30B0, 0x0000);	// DIGITAL_TEST
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30BA, 0x07EC);	// DIGITAL_CTRL
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AC, 0x100C);	// DATA_FORMAT_BITS

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C6, 0x8402);	// HISPI_CONTROL_STATUS (default : 0x8000)
#if defined(__USE_CMOS_AR0331_LVDS_S4__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0x8000);	// hispi_timing
#endif	
#if defined(__USE_CMOS_AR0331_LVDS_S2__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31C0, 0x8000);	// hispi_timing
#endif
	
#if defined(__USE_CMOS_AR0331_LVDS_S4__)
	PLL_settings_LVDS(1);									// Set PLL to LVDS 4ch mode
#elif defined(__USE_CMOS_AR0331_LVDS_S2__)
	PLL_settings_LVDS(2);									// Set PLL to LVDS 2ch mode
#endif
	
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x00E4);	// Y_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3002, 0x00DC);	// Y_ADDR_START
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x0042);	// X_ADDR_START
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3004, 0x003A);	// X_ADDR_START
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x0523);	// Y_ADDR_END
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3006, 0x052B);	// Y_ADDR_END
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07C9);	// X_ADDR_END
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07D1);	// X_ADDR_END
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3008, 0x07E5);	// X_ADDR_END

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300A, 0x08A1);	// FRAME_LENGTH_LINES
#if defined(__USE_CMOS_AR0331_LVDS_S4__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300C, 0x08C0);	// LINE_LENGTH_PCK
#else
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300C, 0x0460);	// LINE_LENGTH_PCK
#endif		
//	CMOS_WordWrite(I2C_AR0331_ADDR, 0x300C, 0x0460);	// LINE_LENGTH_PCK
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3012, 0x0467);	// COARSE_INTEGRATION_TIME
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A2, 0x0001);	// X_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30A6, 0x0001);	// Y_ODD_INC
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3040, 0x0000);	// READ_MODE
	
	HDR_Mode(3);										// HDR Mode
	
#if defined(__USE_CMOS_AR0331_LVDS_S4__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AE, 0x0304);	// SERIAL_FORMAT
#elif defined(__USE_CMOS_AR0331_LVDS_S2__)
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AE, 0x0302);	// SERIAL_FORMAT
#endif
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x306E, 0x9010);	// Data Path Select

	AR0331_Analog_Settings();		

	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3ED2, 0x9546);	// DAC_LD_6_7
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x005C);	// reset (stream start)
}


void PLL_settings_LVDS(BOOL mode)
{	
	if(mode == 1)
	{		
		/*	PLL_settings - LVDS 4-Lane Mode	*/
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x302A, 0x0006);	// VT_PIX_CLK_DIV
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x302C, 0x0001);	// VT_SYS_CLK_DIV
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x302E, 0x0004);	// PRE_PLL_CLK_DIV
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3030, 0x0042);	// PLL_MULTIPLIER
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3036, 0x000C);	// OP_PIX_CLK_DIV
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3038, 0x0001);	// OP_SYS_CLK_DIV
	}
	else if(mode == 2)
	{		
		/*	PLL_settings - LVDS 2-Lane Mode	*/
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x302A, 0x0006);	// VT_PIX_CLK_DIV
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x302C, 0x0002);	// VT_SYS_CLK_DIV
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x302E, 0x0002);	// PRE_PLL_CLK_DIV
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3030, 0x0021);	// PLL_MULTIPLIER
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3036, 0x000C);	// OP_PIX_CLK_DIV
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3038, 0x0001);	// OP_SYS_CLK_DIV
	}
	else	
	{
		/*	PLL_settings - Parallel	*/
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x302A, 0x0008);	// VT_PIX_CLK_DIV
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x302C, 0x0001);	// VT_SYS_CLK_DIV
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x302E, 0x0002);	// PRE_PLL_CLK_DIV
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3030, 0x002C);	// PLL_MULTIPLIER
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3036, 0x000C);	// OP_PIX_CLK_DIV
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3038, 0x0001);	// OP_SYS_CLK_DIV
	}
	
}

void HDR_Mode(WORD nID)
{

	if(nID == 3)
	{
		/*	HDR Mode	*/
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3082, 0x0008);	// operation_mode_ctrl (HDR mode)	
		Delay_mSec(60);			
	
		Motion_2D_Compensation(ON);							// 2D Motion Compensation ON

		//altm enable
		ALTM_Mode(ON);
		ADACD_Mode(ON);										// ADACD Enable
		Companding_Mode(OFF);
		
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x31E0, 0x0200);	// PIX_DEF_ID
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3060, 0x0006);	// ANALOG_GAIN
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x318A, 0x0E10);	// HDR_MC_CTRL1

		Embedded_Data_n_Stats_Mode(ON);

		
	}
	else if(nID == 2)
	{
		/*	HDR Companding Mode		*/
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3082, 0x0008);	// operation_mode_ctrl (HDR mode)	
		
		Motion_2D_Compensation(ON);
		ALTM_Mode(OFF);
		ADACD_Mode(ON);
		Companding_Mode(ON);

		CMOS_WordWrite(I2C_AR0331_ADDR, 0x31E0, 0x0200);	// PIX_DEF_ID
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3060, 0x0006);	// ANALOG_GAIN
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x318A, 0x0E10);	// HDR_MC_CTRL1		

		Embedded_Data_n_Stats_Mode(ON);
	}	
	else if(nID == 1)
	{
		/*	Linear ALTM  Mode		*/
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3082, 0x0009);	// operation_mode_ctrl (linear mode)
		
		Motion_2D_Compensation(OFF);
		ALTM_Mode(ON);
		ADACD_Mode(ON);
		Companding_Mode(OFF);

		CMOS_WordWrite(I2C_AR0331_ADDR, 0x31E0, 0x0200);	// PIX_DEF_ID
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3060, 0x0006);	// ANALOG_GAIN

		Embedded_Data_n_Stats_Mode(ON);
	}	
	else
	{
		/*	Linear Nomal Mode		*/
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3082, 0x0009);	// operation_mode_ctrl (linear mode)
		
		Motion_2D_Compensation(OFF);
		ALTM_Mode(OFF);
		ADACD_Mode(OFF);
		Companding_Mode(OFF);

		CMOS_WordWrite(I2C_AR0331_ADDR, 0x31E0, 0x0200);	// PIX_DEF_ID
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3060, 0x0006);	// ANALOG_GAIN

		Embedded_Data_n_Stats_Mode(ON);
	}
	
}


void Motion_2D_Compensation (BOOL mode)
{
	if(mode == ON)
	{
		// [2D motion compensation ON]
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x318C, 0xC001);	// HDR_MC_CTRL2
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3198, 0x061E);	// HDR_MC_CTRL8
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x30FE, 0x0000);	// NOISE_PEDESTAL
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x320A, 0x0000);	// ADACD_PEDESTAL
	}
	else
	{
		// [2D motion compensation OFF]
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x318C, 0x0000);	// HDR_MC_CTRL2
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3190, 0x0000);	// HDR_MC_CTRL4
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x30FE, 0x0080);	// NOISE_PEDESTAL
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x320A, 0x0080);	// ADACD_PEDESTAL
	}
}


void ALTM_Mode (BOOL mode)
{
	if(mode == ON)
	{
		// [ALTM Enabled]
//		CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10D8);	// bit[2] = 0 : stream stop
		
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x2410, 0x0010);	// ALTM_POWER_GAIN
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x2412, 0x0010);	// ALTM_POWER_GAIN
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x2442, 0x0080);	// ALTM_CONTROL_KEY_K0
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x2444, 0x0000);	// ALTM_CONTROL_KEY_K01_LO
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x2446, 0x0004);	// ALTM_CONTROL_KEY_K01_HI
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x2440, 0x0002);	// ALTM_CONTROL_DAMPER
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x2450, 0x0000);	// ALTM_OUT_PEDESTAL
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x2438, 0x0010);	// ALTM_CONTROL_MIN_FACTOR
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x243A, 0x0020);	// ALTM_CONTROL_MAX_FACTOR
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x243C, 0x0000);	// ALTM_CONTROL_DARK_FLOOR
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x243E, 0x0200);	// ALTM_CONTROL_BRIGHT_FLOOR
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x31D0, 0x0000);	// COMPANDING
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x301E, 0x0000);	// DATA_PEDESTAL
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x2400, 0x0002);	// ALTM_CONTROL
		
//		CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10DC);	// bit[2] = 1 : stream start
		
	}
	else
	{
		// [ALTM Bypassed]
//		CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10D8);	// bit[2] = 0 : stream stop
		
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x2400, 0x0003);	// ALTM_CONTROL
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x2450, 0x0000);	// ALTM_OUT_PEDESTAL
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x301E, 0x00A8);	// DATA_PEDESTAL

//		CMOS_WordWrite(I2C_AR0331_ADDR, 0x301A, 0x10DC);	// bit[2] = 1 : stream start
	}
}


void ADACD_Mode (BOOL mode)
{
	if(mode == ON)
	{
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3202, 0x00CF);	
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3206, 0x0A06);	
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3208, 0x1A12);	
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3200, 0x0002);	// ADACD_CONTROL
	}
	else
	{
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3200, 0x0000);	// ADACD_CONTROL
	}
}


void Companding_Mode (BOOL mode)
{
	if(mode == ON)
	{
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AC, 0x100C);	// DATA_FORMAT_BITS
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x31D0, 0x0001);	// COMPANDING
	}
	else
	{
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x31AC, 0x0C0C);	// DATA_FORMAT_BITS	
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x31D0, 0x0000);	// COMPANDING
	}
}


void Embedded_Data_n_Stats_Mode (BOOL mode)
{
	if(mode == ON)
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3064, 0x1982);	// SMIA_TEST, EMBEDDED_STATS_EN, EMBEDDED_DATA
	else
		CMOS_WordWrite(I2C_AR0331_ADDR, 0x3064, 0x1802);	// SMIA_TEST, EMBEDDED_STATS_EN, EMBEDDED_DATA
}


void AR0331_Analog_Settings(void)
{
	/*	[Analog Settings]	*/
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x30F4, 0x4000);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3ED4, 0x8F6C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3ED6, 0x6666);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3EDA, 0x8899);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3EE6, 0x00F0);
}


void AR0331_Linear_Sequencer_Load(void)
{
	/*	[AR0331 Linear sequencer load - 1.0]	*/
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3088, 0x8000);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4A03);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4316);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0443);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1645);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4045);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x6017);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5045);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x404B);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1244);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x6134);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4A31);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4342);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4560);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2714);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x3DFF);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x3DFF);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x3DEA);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2704);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x3D10);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2705);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x3D10);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2715);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x3527);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x053D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1045);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4027);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0427);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x143D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xFF3D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xFF3D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xEA62);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2728);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x3627);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x083D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x6444);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2C2C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2C2C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4B01);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x432D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4643);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1647);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x435F);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4F50);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2604);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2694);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2027);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xFC53);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0D5C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0D57);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5417);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0955);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5649);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5307);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5303);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4D28);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x6C4C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0928);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2C28);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x294E);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5C09);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4500);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4580);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x26B6);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x27F8);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1702);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x27FA);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5C0B);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1718);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x26B2);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5C03);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1744);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x27F2);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1702);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2809);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1710);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1628);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x084D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1A26);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x9316);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1627);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xFA45);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xA017);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0727);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xFB17);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2945);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x8017);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0827);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xFA17);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x285D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x170E);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2691);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5301);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1740);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5302);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1710);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2693);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2692);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x484D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4E28);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x094C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0B17);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5F27);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xF217);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1428);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0816);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4D1A);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1616);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x27FA);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2603);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5C01);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4540);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2798);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x172A);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4A0A);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4316);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0B43);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x279C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4560);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1707);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x279D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1725);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4540);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1708);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2798);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5D53);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0D26);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x455C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x014B);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1244);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5251);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1702);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x6018);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4A03);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4316);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0443);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1658);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4316);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5943);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x165A);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4316);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5B43);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4540);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x279C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4560);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1707);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x279D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1725);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4540);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1710);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2798);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1720);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x224B);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1244);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2C2C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2C2C);
}


void AR0331_HDR_Sequencer_Load(void)
{
	/*	[AR0331 HDR sequencer load - 3.0]	*/
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3088, 0x8000);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4540);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x6134);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4A31);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4342);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4560);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2714);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x3DFF);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x3DFF);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x3DEA);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2704);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x3D10);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2705);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x3D10);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2715);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x3527);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x053D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1045);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4027);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0427);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x143D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xFF3D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xFF3D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xEA62);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2728);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x3627);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x083D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x6444);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2C2C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2C2C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4B00);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x432D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x6343);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1664);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x435F);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4F50);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2604);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2694);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x27FC);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x530D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5C0D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5754);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1709);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5556);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4953);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0753);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x034D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x286C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4C09);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x282C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2828);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x261C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4E5C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0960);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4500);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4580);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x26BE);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x27F8);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1702);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x27FA);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5C0B);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1712);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x26BA);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5C03);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1713);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x27F2);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x171C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5F28);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0817);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0360);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x173C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x26B2);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1616);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5F4D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1926);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x9316);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1627);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xFA45);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xA017);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0527);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xFB17);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1F45);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x801F);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1705);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x27FA);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x171E);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5D17);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0C26);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x9248);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4D4E);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x269A);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2808);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4C0B);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x6017);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0327);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xF217);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2626);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x9216);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x165F);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4D19);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2693);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1616);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x27FA);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2643);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5C01);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4540);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2798);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1720);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4A65);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4316);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x6643);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5A43);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x165B);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4327);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x9C45);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x6017);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0627);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x9D17);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1C45);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4023);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1705);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2798);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5D26);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4417);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0E28);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0053);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x014B);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5251);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1244);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4B01);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x432D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4643);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1647);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x434F);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5026);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0426);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x8427);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xFC53);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0D5C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0D57);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5417);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0955);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5649);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5307);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5303);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4D28);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x6C4C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0928);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2C28);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2826);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0C4E);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5C09);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x6045);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0045);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x8026);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xAE27);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xF817);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0227);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xFA5C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0B17);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1226);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xAA5C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0317);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0B27);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xF217);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x265F);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2808);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1703);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x6017);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0326);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xA216);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x165F);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4D1A);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2683);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1616);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x27FA);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x45A0);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1705);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x27FB);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x171F);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4580);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2017);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0527);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xFA17);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1E5D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x170C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2682);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x484D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4E26);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x8A28);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x084C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0B60);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1707);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x27F2);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1738);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2682);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1616);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5F4D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1A26);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x8316);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1627);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0xFA26);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x435C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0145);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4027);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x9817);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1F4A);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1244);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0343);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1604);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4316);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5843);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1659);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4316);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x279C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4560);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x1705);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x279D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x171D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4540);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2217);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x0527);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x985D);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2645);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x170E);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2800);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5301);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4B52);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x5112);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x4460);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2C2C);
	CMOS_WordWrite(I2C_AR0331_ADDR, 0x3086, 0x2C2C);
}


void AR0331_reg_read(void)
{
	UARTprintf("AR0331_reg_read\n\r");
	CMOS_WordRead(I2C_AR0331_ADDR, 0x30B0);	// DIGITAL_TEST
	CMOS_WordRead(I2C_AR0331_ADDR, 0x30BA);	// DIGITAL_CTRL
	CMOS_WordRead(I2C_AR0331_ADDR, 0x31AC);	// DATA_FORMAT_BITS

	// PLL
	CMOS_WordRead(I2C_AR0331_ADDR, 0x302A);	// VT_PIX_CLK_DIV
	CMOS_WordRead(I2C_AR0331_ADDR, 0x302C);	// VT_SYS_CLK_DIV
	CMOS_WordRead(I2C_AR0331_ADDR, 0x302E);	// PRE_PLL_CLK_DIV
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3030);	// PLL_MULTIPLIER
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3036);	// OP_PIX_CLK_DIV
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3038);	// OP_SYS_CLK_DIV

	CMOS_WordRead(I2C_AR0331_ADDR, 0x3002);	// Y_ADDR_START
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3004);	// X_ADDR_START
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3006);	// Y_ADDR_END
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3008);	// X_ADDR_END
	CMOS_WordRead(I2C_AR0331_ADDR, 0x300A);	// FRAME_LENGTH_LINES
	CMOS_WordRead(I2C_AR0331_ADDR, 0x300C);	// LINE_LENGTH_PCK
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3012);	// COARSE_INTEGRATION_TIME
	CMOS_WordRead(I2C_AR0331_ADDR, 0x30A2);	// X_ODD_INC
	CMOS_WordRead(I2C_AR0331_ADDR, 0x30A6);	// Y_ODD_INC
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3040);	// READ_MODE
	CMOS_WordRead(I2C_AR0331_ADDR, 0x31AE);	// SERIAL_FORMAT
	// HDR mode
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3082);	// operation_mode_ctrl (HDR mode)
	// 2D motion compensation
	CMOS_WordRead(I2C_AR0331_ADDR, 0x318C);	// HDR_MC_CTRL2
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3190);	// HDR_MC_CTRL4
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3198);	// HDR_MC_CTRL8
	CMOS_WordRead(I2C_AR0331_ADDR, 0x30FE);	// NOISE_PEDESTAL
	CMOS_WordRead(I2C_AR0331_ADDR, 0x320A);	// ADACD_PEDESTAL
	CMOS_WordRead(I2C_AR0331_ADDR, 0x2410);	// ALTM_POWER_GAIN
	// ALTM 
	CMOS_WordRead(I2C_AR0331_ADDR, 0x2412);	// ALTM_POWER_GAIN
	CMOS_WordRead(I2C_AR0331_ADDR, 0x2442);	// ALTM_CONTROL_KEY_K0
	CMOS_WordRead(I2C_AR0331_ADDR, 0x2444);	// ALTM_CONTROL_KEY_K01_LO
	CMOS_WordRead(I2C_AR0331_ADDR, 0x2446);	// ALTM_CONTROL_KEY_K01_HI
	CMOS_WordRead(I2C_AR0331_ADDR, 0x2440);	// ALTM_CONTROL_DAMPER
	CMOS_WordRead(I2C_AR0331_ADDR, 0x2450);	// ALTM_OUT_PEDESTAL
	CMOS_WordRead(I2C_AR0331_ADDR, 0x2438);	// ALTM_CONTROL_MIN_FACTOR
	CMOS_WordRead(I2C_AR0331_ADDR, 0x243A);	// ALTM_CONTROL_MAX_FACTOR
	CMOS_WordRead(I2C_AR0331_ADDR, 0x243C);	// ALTM_CONTROL_DARK_FLOOR
	CMOS_WordRead(I2C_AR0331_ADDR, 0x243E);	// ALTM_CONTROL_BRIGHT_FLOOR
	CMOS_WordRead(I2C_AR0331_ADDR, 0x31D0);	// COMPANDING
	CMOS_WordRead(I2C_AR0331_ADDR, 0x301E);	// DATA_PEDESTAL
	CMOS_WordRead(I2C_AR0331_ADDR, 0x2400);	// ALTM_CONTROL
	// ADACD Mode
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3202);	
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3206);	
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3208);	
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3200);	// ADACD_CONTROL
	// HDR mode
	CMOS_WordRead(I2C_AR0331_ADDR, 0x31E0);	// PIX_DEF_ID
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3060);	// ANALOG_GAIN
	CMOS_WordRead(I2C_AR0331_ADDR, 0x318A);	// HDR_MC_CTRL1
	// Embedded_Data_n_Stats_Mode
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3064);	// SMIA_TEST, EMBEDDED_STATS_EN, EMBEDDED_DATA
	// Analog Settings
	CMOS_WordRead(I2C_AR0331_ADDR, 0x30F4);
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3ED4);
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3ED6);
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3EDA);
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3EE6);
	// ???
	CMOS_WordRead(I2C_AR0331_ADDR, 0x3ED2);	// DAC_LD_6_7
}


#endif	/* __USE_CMOS_AR0331__ */

/*  FILE_END_HERE */

