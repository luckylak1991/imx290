/// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_MT9P031__)
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
static void MT9P031_CMOS_Reset(void);
static void MT9P031_96Mhz_DevwareBayerOffset(void);
static void MT9P031_CMOS_1080p30(BOOL OnOff);
static void MT9P031_CMOS_5Mega_p15(BOOL OnOff);
static void MT9P031_CMOS_5Mega_p15_actarea(BOOL OnOff);
// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------

void MT9P031_Initialize(void)
{	

#if defined(__USE_CMOS_MT9P031_1080p__)		// use 9p031 full hd 30fps mode
	MT9P031_CMOS_1080p30(ON);
	MT9P031_CMOS_5Mega_p15(OFF);
#endif

#if defined(__USE_CMOS_MT9P031_5M_PIXEL__)	// use 9p031 5M cmos mode
	MT9P031_CMOS_1080p30(OFF);
	MT9P031_CMOS_5Mega_p15(OFF);		 //Full Scan mode h : 2751, For test mode
	MT9P031_CMOS_5Mega_p15_actarea(ON);	 //5M active Scan mode h: 2592, Generally use this
#endif

//   Check I2C debug code
//	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x09, 0x284);	//RESET_REG(check wave form datasheet)

}

static void MT9P031_CMOS_Reset(void)
{
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x0D, 1);	//RESET_REG 
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x0D, 0);	//RESET_REG 	
	Delay_mSec(100);

}

static void MT9P031_96Mhz_DevwareBayerOffset(void)
{
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x05, 0x0600);	// HORZ_BLANK_REG (HB)=1536
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x10, 0x0051);	// PLL_CONTROL; POWER UP PLL
	
//-------------------------------------------------------------------------------
	//aptina default
//	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x11, 0x1801);	// PLL_CONFIG_1: m=24, n=1
//	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x12, 0x0002);	// PLL_CONFIG_2: p1=2, p2=0

	//V_Sync : 29.98Hz, H_Sync : 34.11Khz, Pixel_CLK : 96Mhz
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x11, 0x3903);	// PLL_CONFIG_1: m=57, n=3   
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x12, 0x0003);	// PLL_CONFIG_2: p1=3, p2=0
	Delay_mSec(1);			// Wait 1ms for VCO to lock
//-------------------------------------------------------------------------------

	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x10, 0x0053);	// PLL_CONTROL; USE PLL
	Delay_mSec(200);		
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x05, 0x01C2);	//(1) HORZ_BLANK_REG

	//aptina default	
//	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x07, 0x1F8E);	//Enable Parallel Fifo Data

	//V_Sync : 29.98Hz, H_Sync : 34.11Khz, Pixel_CLK : 96Mhz
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x07, 0x1E06);	//Enable Parallel Fifo Data
}

//1080P tunning for MISP
static void MT9P031_CMOS_1080p30(BOOL OnOff)
{
	if(OnOff==OFF) return;
	
	MT9P031_CMOS_Reset();
	MT9P031_96Mhz_DevwareBayerOffset();
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x01, 0x01ae);	// ROW WINDOW START(430=1af)
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x02, 0x014e); // COL WINDOW START
	
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x03, 0x04B0);	// ROW_WINDOW_SIZE_REG(1200)
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x04, 0x0900);	// COL_WINDOW_SIZE_REG(2200)	
	
//	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x03, 0x0439);	// ROW_WINDOW_SIZE_REG(1079,0x437)
//	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x04, 0x077F);	// COL_WINDOW_SIZE_REG(1919 + 2=77f + 2)
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x05, 0x0030);	// HORZ BLANK 0
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x06, 0x0010);	// VERT BLANK 0
	
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x08, 0x0000);	// SHUTTER_WIDTH_HI
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x09, 0x01AC);	//  SHUTTER_WIDTH_LOW (INTEG_TIME_REG) always bigger then 1
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x0c, 0x0000);	// SHUTTER_DELAY_REG

	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x20, 0x0840);	//Read Mode 2. (sensor's black level(courase) on, see OB area row)	
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x22, 0x0000);	// ROW_MODE(row bin/row skip)
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x23, 0x0000);	// COL_MODE(col bin/col skip)
	
//	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x06, 0x0037);	// VERT_BLANK_REG has been adjusted to give precisely 30fps
//	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x05, 0x0000);	// HORZ_BLANK_REG

}

static void MT9P031_CMOS_5Mega_p15_actarea(BOOL OnOff)
{
	//this is sensor send after courase blc -> v_ob area + active image
	
	if(OnOff==OFF) return;

	MT9P031_CMOS_Reset();
	
	//PLL SETTING, V_SYNC : 15Hz, H_SYNC : 29.38Khz, Pixel Clock 
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x10, 0x0051);	// PLL_CONTROL; POWER UP PLL
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x11, 0x2402);	// PLL_CONFIG_1: m=36, n=2
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x12, 0x0002);	// PLL_CONFIG_2: p1=2, p2=0
	Delay_mSec(1);			// Wait 1ms for VCO to lock
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x10, 0x0053);	// PLL_CONTROL; USE PLL
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x07, 0x1F0E);	//Enable Parallel Fifo Data
	Delay_mSec(200);	

	//Timing Setting
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x2b, 0x0008);//(3) GREEN1_GAIN_REG = 8
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x2c, 0x0008);//(3) BLUE_GAIN_REG = 8(ref 0x18)
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x2d, 0x0008);//(3) RED_GAIN_REG = 8(ref 0x10)
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x2e, 0x0008);//(3) GREEN2_GAIN_REG = 8
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x01, 0x0036);//Row Start = 54
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x02, 0x0011);//Column Start = 17
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x03, 0x0797);//Row Size = 1943
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x04, 0x0A20);//Column Size = 2592
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x05, 0x021D);//Horz. Blank = 289
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x06, 0x0010);//Vert. Blank = 9
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x1e, 0x0006);//Read Mode 1 = 6
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x20, 0x0840);	//Read Mode 2. (sensor course black level on, see V OB area)
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x22, 0x0000);//Row Mode = 0
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x23, 0x0000);	//Column Mode = 0
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x08, 0x0000);	//Shut. Wid. Upper = 0

	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x4B, 0x00A8);	//out offset register = 0x100  (case sensor auto blc off)
	
	//shutter init
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x09, 0x07A0); //Integration Time = 1146
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x0c, 0x0000); // Shutter Delay = 0
	
}

static void MT9P031_CMOS_5Mega_p15(BOOL OnOff)
{

	if(OnOff==OFF) return;

	MT9P031_CMOS_Reset();
	
	//PLL SETTING, V_SYNC : 15Hz, H_SYNC : 29.38Khz, Pixel Clock 
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x10, 0x0051);	// PLL_CONTROL; POWER UP PLL
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x11, 0x2702);	// PLL_CONFIG_1: m=36, n=2
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x12, 0x0002);	// PLL_CONFIG_2: p1=2, p2=0
	Delay_mSec(1);			// Wait 1ms for VCO to lock
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x10, 0x0053);	// PLL_CONTROL; USE PLL
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x07, 0x1F0E);	//Enable Parallel Fifo Data
	Delay_mSec(200);	


	//Timing Setting (Full Scan Mode. Sensor's caurse blc off)
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x2b, 0x0008);//(3) GREEN1_GAIN_REG = 8
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x2c, 0x0008);//(3) BLUE_GAIN_REG = 8(ref 0x18)
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x2d, 0x0008);//(3) RED_GAIN_REG = 8(ref 0x10)
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x2e, 0x0008);//(3) GREEN2_GAIN_REG = 8
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x01, 0x0000);//Row Start = 54
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x02, 0x0000);//Column Start = 16
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x03, 0x07D3);//Row Size = 2003
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x04, 0x0ABF);//Column Size = 2751
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x05, 0x021D);//Horz. Blank = 289
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x06, 0x0010);//Vert. Blank = 9
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x1e, 0x0006);//Read Mode 1 = 6
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x20, 0x0000);	//Read Mode 2. (sensor black level off, see OB area)
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x22, 0x0000);//Row Mode = 0
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x23, 0x0000);	//Column Mode = 0
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x08, 0x0000);	//Shut. Wid. Upper = 0
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x49, 0x0100);	//out offset register = 0x100  (case sensor auto blc off)	
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x4B, 0x0000);	//out offset register = 0x100  (case sensor auto blc off)
	
	//shutter init
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x09, 0x07A0); //Integration Time = 1146
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x0c, 0x0000); // Shutter Delay = 0
/*	
	//Full Resolution Optimization - 15FPS @ 96MHz
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x70, 0x0079);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x71, 0x7800);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x72, 0x7800);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x73, 0x0300);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x74, 0x0300); // hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x75, 0x3c00);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x76, 0x4e3d);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x77, 0x4e3d);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x78, 0x774f);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x79, 0x7900);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x7a, 0x7900);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x7b, 0x7800);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x7c, 0x7800);// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x7e, 0x7800);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x7f, 0x7800);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x06, 0x0000);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x29, 0x0481);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x3e, 0x0087);// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x3f, 0x0007);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x41, 0x0003);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x48, 0x0018);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x5f, 0x1c16);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x57, 0x0007);	// hidden
	CMOS_WordWrite(I2C_MT9P031_ADDR, 0x2a, 0xff74);	// hidden	
	*/
}

#endif	




/*  FILE_END_HERE */
