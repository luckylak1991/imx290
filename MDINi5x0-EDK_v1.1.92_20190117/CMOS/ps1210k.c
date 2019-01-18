// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_PS1210K__)
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

void PS1210K_Initialize(void)
{	

		//start up
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x03,0x00);	//page a		
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x29,0x98);	//output Hi-z release

		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x0C,0x00);	//windowx1_h (00)			
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x0D,0x0D);	//windowx1_l (01)	
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x0E,0x00);	//windowx1_h (00)			
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x0F,0x10);	//windowx1_l (02)	

		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x10,0x07);	//windowx1_h (00)			
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x11,0x97);	//windowx1_l (0D)	
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x12,0x04);	//windowx1_h (00)			
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x13,0x58);	//windowx1_l (0D)	
		
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x14,0x00);	//windowx1_h (00)			
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x15,0x1A);	//windowx1_l (0D)	
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x16,0x04);	//windowx1_h (04)			
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x17,0x5F);	//windowx1_l (53)	

		//30fps setting
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x25,0x00);	//CLK DIV1
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x26,0xC3);	//CLK DIV1		
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x41,0x0B);	//# pll_m_cnt (21)		
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x42,0x04);	//# pll_r_cnt (04)

		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x40,0x10);	//# pll_m_cnt (21)	
		Delay_mSec(50);			
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x40,0x00);	//# pll_m_cnt (21)	

		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x05,0x03);	//# mirror/flip	
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x33,0x01);	//# pixelbias	
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x34,0x02);	//# compbias


		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x03,0x01);	//page b	
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0xC0,0x04);	//# inttime_h  
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0xC1,0x4F);	//# inttime_m  		
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0xC2,0x00);	//# inttime_l  
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0xC3,0x00);	//# globalgain 
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0xC4,0x40);	//# digitalgain
		CMOS_ByteWrite(I2C_PS1210K_ADDR,0x26,0x03);	//# blacksun_th_h


}

#endif	
/*  FILE_END_HERE */
