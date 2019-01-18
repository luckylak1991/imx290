// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_AR0134__) || defined( __USE_CMOS_AR0135__)
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
#if defined(__USE_CMOS_AR0134_TTL__)
static void AR0134_Parallel_720p(void);
#else
static void AR0134_LVDS_720p(void);
#endif
// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void AR0134_Initialize(void)
{
// ------------	Sensor Output Mode Selection	------------//
#if defined (__USE_CMOS_AR0134_TTL__)
	AR0134_Parallel_720p();		//TTL_nomal
#else
	AR0134_LVDS_720p();		//__USE_CMOS_AR0134_LVDS_S2__
#endif	//defined (__USE_CMOS_AR0331_TTL__)


}

#if defined(__USE_CMOS_AR0134_TTL__)
static void AR0134_Parallel_720p(void)
{
	UARTprintf("AR0134&&AR0135_TTL_Initialize\n\r");

	Delay_mSec(200);	

//	[PLL_settings]
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3028, 0x0010);	//ROW_SPEED = 16
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x302A, 0x0008);	//VT_PIX_CLK_DIV = 8
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x302C, 0x0001);	//VT_SYS_CLK_DIV = 1
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x302E, 0x0002);	//PRE_PLL_CLK_DIV = 2
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3030, 0x002C);	//PLL_MULTIPLIER = 44
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3032, 0x0000);	//DIGITAL_BINNING = 0
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x30B0, 0x0080);	//DIGITAL_TEST = 128

//	[Timing_settings]

	CMOS_WordWrite(I2C_AR0134_ADDR, 0x301A, 0x00D8);	//RESET_REGISTER = 216
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x301A, 0x10DC);	//RESET_REGISTER = 4316
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3002, 0x007C);	//Y_ADDR_START = 124
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3004, 0x0002);	//X_ADDR_START = 2
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3006, 0x0351);	//Y_ADDR_END = 849
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3008, 0x0508);	//X_ADDR_END = 1288
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x300A, 0x02ED);	//FRAME_LENGTH_LINES = 747+2
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x300C, 0x0672);	//LINE_LENGTH_PCK = 1650

	
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3012, 0x01C2);	//COARSE_INTEGRATION_TIME = 450
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3014, 0x0009);	//FINE_INTEGRATION_TIME = 900
	
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x30A6, 0x0001);	//Y_ODD_INC = 1
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x308C, 0x007C);	//Y_ADDR_START_CB = 119
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x308A, 0x0002);	//X_ADDR_START_CB = 2
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3090, 0x0351);	//Y_ADDR_END_CB = 849
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x308E, 0x0508);	//X_ADDR_END_CB = 1288	
	
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x30AA, 0x02ED);	//FRAME_LENGTH_LINES_CB = 747+2

	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3016, 0x01C2);	//COARSE_INTEGRATION_TIME_CB = 450
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3018, 0x0009);	//FINE_INTEGRATION_TIME_CB = 9
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x30A8, 0x0001);	//Y_ODD_INC_CB = 1

#if defined(__USE_AR0134_VFLIP__)	
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3040, 0x8000);	//READ_MODE = 0. invert
#elif defined(__USE_AR0134_HVFLIP__)
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3040, 0xC000);	//READ_MODE = 0. invert
#else
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3040, 0x0000);	//READ_MODE = 0
#endif

	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3044, 0x1404|0x0200);	//DARK_CONTROL [12]=1, enable SHOW_DARK_COLS for ob area read. 

	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3064, 0x1982);	//EMBEDDED_DATA_CTRL = 6530
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x31C6, 0x8008);	//HISPI_CONTROL_STATUS = 32776	


#if defined(__USE_CMOS_AR0134__INTERNAL_AE)
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3100,  0x0013);	//Internal Auto-AE enable
#endif

	//CMOS_WordWrite(I2C_AR0134_ADDR, 0x3056, 32);	//Green#1 gain
	//CMOS_WordWrite(I2C_AR0134_ADDR, 0x3058, 32);	//Blue gain
	//CMOS_WordWrite(I2C_AR0134_ADDR, 0x305A, 32);	//Red gain
	//CMOS_WordWrite(I2C_AR0134_ADDR, 0x305C, 32);	//Green#2 gain
	//CMOS_WordWrite(I2C_AR0134_ADDR, 0x305E, 32);	//Global gain change all gain...

	//CMOS_WordWrite(I2C_AR0134_ADDR, 0x3108, 16);	//AE_MIN_EV_STEP_REG
	

}
#else

static void AR0134_LVDS_720p(void)
{
	UARTprintf("AR0134&&AR0135_LVDS_2CH_Initialize\n\r");

	Delay_mSec(200);	

//	[PLL_settings]
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3028, 0x0010);	//ROW_SPEED = 16
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x302A, 0x0006);	//VT_PIX_CLK_DIV = 6
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x302C, 0x0001);	//VT_SYS_CLK_DIV = 1
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x302E, 0x0002);	//PRE_PLL_CLK_DIV = 2
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3030, 0x0021);	//PLL_MULTIPLIER = 33
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3032, 0x0000);	//DIGITAL_BINNING = 0
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x30B0, 0x0080);	//DIGITAL_TEST = 128

//	[Timing_settings]

	CMOS_WordWrite(I2C_AR0134_ADDR, 0x301A, 0x00D8);	//RESET_REGISTER = 216
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x301A, 0x005C);	//RESET_REGISTER = 92
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3002, 0x007C);	//Y_ADDR_START = 124
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3004, 0x0002);	//X_ADDR_START = 2
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3006, 0x0351);	//Y_ADDR_END = 849
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3008, 0x0508);	//X_ADDR_END = 1288
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x300A, 0x02ED);	//FRAME_LENGTH_LINES = 747+2
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x300C, 0x0672);	//LINE_LENGTH_PCK = 1650

	
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3012, 0x01C2);	//COARSE_INTEGRATION_TIME = 450
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3014, 0x0009);	//FINE_INTEGRATION_TIME = 9
	
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x30A6, 0x0001);	//Y_ODD_INC = 1
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x308C, 0x007C);	//Y_ADDR_START_CB = 119
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x308A, 0x0002);	//X_ADDR_START_CB = 2
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3090, 0x0351);	//Y_ADDR_END_CB = 849
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x308E, 0x0508);	//X_ADDR_END_CB = 1288	
	
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x30AA, 0x02ED);	//FRAME_LENGTH_LINES_CB = 747+2

	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3016, 0x01C2);	//COARSE_INTEGRATION_TIME_CB = 450
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3018, 0x0009);	//FINE_INTEGRATION_TIME_CB = 9
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x30A8, 0x0001);	//Y_ODD_INC_CB = 1

#if defined(__USE_AR0134_VFLIP__)	
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3040, 0x8000);	//READ_MODE = 0. invert
#elif defined(__USE_AR0134_HVFLIP__)
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3040, 0xC000);	//READ_MODE = 0. invert
#else
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3040, 0x0000);	//READ_MODE = 0
#endif

	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3044, 0x1404|0x0200);	//DARK_CONTROL [12]=1, enable SHOW_DARK_COLS for ob area read. 

	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3064, 0x1982);	//EMBEDDED_DATA_CTRL = 6530
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x31C6, 0x8000);	//HISPI_CONTROL_STATUS = 32776	


#if defined(__USE_CMOS_AR0134__INTERNAL_AE)
	CMOS_WordWrite(I2C_AR0134_ADDR, 0x3100,  0x0013);	//Internal Auto-AE enable
#endif


}
#endif

#endif	/* __USE_CMOS_AR0134__ */ /* __USE_CMOS_AR0135__*/

/*  FILE_END_HERE */

