// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_MT9M131__)
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
void MT9M131_Initialize(void)
{
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x00d, 0x0001);	// reset
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x00d, 0x0000);

	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x13a, 0x0600);	// color pipe A

	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x007, 0x00be);	// sensor core A
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x008, 0x0011);
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x021, 0x040c);

	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x29c, 0xdf20);	// camera control B

	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x19b, 0x0600);	// color pipe B

	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x022, 0x0329);	// sensor core B
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x005, 0x0184);
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x006, 0x002a);

	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x237, 0x0080);	// camera control C

//	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x106, 0x748e);	// color pipe C
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x106, 0x348c);	// color pipe C
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x1b3, 0x0002);

	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x001, 0x000c);	// sensor core C
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x002, 0x001e);
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x003, 0x0400);
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x004, 0x0500);

	// set resolution to SXGA
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x00d, 0x8008);	// reset

	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x1af, 0x0403);	// reducer zoom
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x19f, 0x0000);	// reducer horizontal pan
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x1a0, 0x0500);	// reducer horizontal zoom
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x1a1, 0x0500);	// reducer horizontal size
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x1a2, 0x0000);	// reducer vertical pan
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x1a3, 0x0400);	// reducer vertical zoom
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x1a4, 0x0400);	// reducer vertical size

	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x237, 0x0080);	// 
	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x2c8, 0x9f0b);	// 

	CMOS_WordWrite(I2C_MT9M131_ADDR, 0x00d, 0x0008);
}

#endif	/* __USE_CMOS_MT9M131__ */

/*  FILE_END_HERE */
