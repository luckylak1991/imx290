// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_OV2710__)
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

void OV2710_Initialize(void)
{	
	UARTprintf("OV2710_Initialize dvp 1920x1080 ref code 1 at google web\n\r");
   
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3103,0x93);  //--clock
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3008,0x82);  //--nomal work mode
	
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3017,0x7f);  //-- output
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3018,0xfc);  //--output
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3706,0x61);  //
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3712,0x0c);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3630,0x6d);
	
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3801,0xb4);  //--HREF Horizontal start point
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3621,0x04);  //array control
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3604,0x60);//
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3603,0xa7);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3631,0x26);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3600,0x04);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3620,0x37);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3623,0x00);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3702,0x9e);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3703,0x5c);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3704,0x40);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x370d,0x0f);//vitical bining
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3713,0x9f);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3714,0x4c);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3710,0x9e);
	
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3801,0xc4);//h start point
	
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3605,0x05);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3606,0x3f);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x302d,0x90);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x370b,0x40);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3716,0x31);
	
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3707,0x52);
	
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x380d,0x74);//2420

	//start, size reg
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3809, 0x86);	
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x380B, 0x3E);	
	
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x5181,0x20);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x518f,0x00);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x4301,0xff);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x4303,0x00);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3a00,0x78);
	
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x300f,0x88);
	
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3011,0x28);
	
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3a1a,0x06);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3a18,0x00);
	//CMOS_ByteWrite(I2C_OV2710_ADDR,0x3a19,0x7a);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3a19,0xe0);
	//CMOS_ByteWrite(I2C_OV2710_ADDR,0x3a13,0x54);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3a13,0x50);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x382e,0x0f);
	//CMOS_ByteWrite(I2C_OV2710_ADDR,0x3818,0xe0);//add last for flip
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x381a,0x1a);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x401d,0x02);

	CMOS_ByteWrite(I2C_OV2710_ADDR,0x5688,0x03);
	
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x5684,0x07);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x5685,0xa0);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x5686,0x14);//CMOS_ByteWrite(I2C_OV2710_ADDR,0x5686,0x04);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x5687,0x43);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3a0f,0xf0);//CMOS_ByteWrite(I2C_OV2710_ADDR,0x3a0f,0x40);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3a10,0x38);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3a1b,0xf8);//CMOS_ByteWrite(I2C_OV2710_ADDR,0x3a1b,0x48);

	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3a1e,0x30);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3a11,0x90);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3a1f,0x10);

	//CMOS_ByteWrite(I2C_OV2710_ADDR,0x3010,0x20);//0x10,15fps;0x00,30fps
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3010,0x00);
	
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x4708, 0x03);	//DVP Control2   [1] is vsync invert, [0] is default 1   


	
	/*close ae */
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3503,0x17); // org 0x07 -> 0x17 // jwlee 20160525 // debug hor gain problem.
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3501,0x2e);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3502,0x00);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x350b,0x00);

	/* close awb */	
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3406,0x01);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3400,0x04);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3401,0x00);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3402,0x04);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3403,0x00);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3404,0x04);
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3405,0x00);
	
	/* modified blc */
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x4000,0x05);

	/*drive capacity*/
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x302c,0x00);

	/* close lenc */
	CMOS_ByteWrite(I2C_OV2710_ADDR,0x5000,0xdf);

	CMOS_ByteWrite(I2C_OV2710_ADDR,0x3008,0x02);//nomal work mode

	// expose max value(VTS)
//	CMOS_ByteWrite(I2C_OV2710_ADDR,0x350c, 0x4);	// msb(9:8)
//	CMOS_ByteWrite(I2C_OV2710_ADDR,0x350d, 0x38);	// lsb(7:0)

}


void OV2710_Read(void)
{
/*
		BYTE temp, temp2;

		//start, size
		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3800);
		temp2=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3801);
		UARTprintf("[OV2710] 0x3800,1 reg HS[11:0] : %d \r\n", temp*256+temp2);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3802);
		temp2=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3803);		
		UARTprintf("[OV2710] 0x3802,3 reg VS[11:0] : %d \r\n",  temp*256+temp2);			

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3804);
		temp2=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3805);		
		UARTprintf("[OV2710] 0x3804,5 reg HW[11:0] : %d \r\n", temp*256+temp2);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3806);
		temp2=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3807);		
		UARTprintf("[OV2710] 0x3806,7 reg VH[11:0] : %d \r\n", temp*256+temp2);

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x4740);
		UARTprintf("[OV2710] 0x4740 reg pola : %x \r\n", temp);			

		//vsync
		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x301d);
		if(temp!=0x00)	UARTprintf("[OV2710] 0x3103 reg PAD output sel 01 : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3012);
		UARTprintf("[OV2710] 0x3012 reg err PLL : %x \r\n", temp);		
		
		
		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3103);
		if(temp!=0x03)	UARTprintf("[OV2710] 0x3103reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3008);
		if(temp!=0x82)	UARTprintf("[OV2710] 0x3008 reg err RST : %x \r\n", temp);	
		
		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3017);
		if(temp!=0x7f)	UARTprintf("[OV2710] 0x3017 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3018);
		if(temp!=0xfc)	UARTprintf("[OV2710] 0x3018 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3706);
		if(temp!=0x61)	UARTprintf("[OV2710] 0x3706 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3712);
		if(temp!=0x0c)	UARTprintf("[OV2710] 0x3712 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3630);
		if(temp!=0x6d)	UARTprintf("[OV2710] 0x3630 reg err : %x \r\n", temp);	
			
		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3621);
		if(temp!=0x04)	UARTprintf("[OV2710] 0x3621 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3604);
		if(temp!=0x60)	UARTprintf("[OV2710] 0x3604 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3603);
		if(temp!=0xa7)	UARTprintf("[OV2710] 0x3603 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3631);
		if(temp!=0x26)	UARTprintf("[OV2710] 0x3603 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3600);
		if(temp!=0x04)	UARTprintf("[OV2710] 0x3600 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3620);
		if(temp!=0x37)	UARTprintf("[OV2710] 0x3620 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3623);
		if(temp!=0x00)	UARTprintf("[OV2710] 0x3623 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3702);
		if(temp!=0x9e)	UARTprintf("[OV2710] 0x3702 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3703);
		if(temp!=0x74)	UARTprintf("[OV2710] 0x3703 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3704);
		if(temp!=0x10)	UARTprintf("[OV2710] 0x3704 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x370d);
		if(temp!=0x0f)	UARTprintf("[OV2710] 0x370d reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3713);
		if(temp!=0x8b)	UARTprintf("[OV2710] 0x3713 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3714);
		if(temp!=0x74)	UARTprintf("[OV2710] 0x3714 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3710);
		if(temp!=0x9e)	UARTprintf("[OV2710] 0x3710 reg err : %x \r\n", temp);	
	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3605);
		if(temp!=0x05)	UARTprintf("[OV2710] 0x3605 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3606);
		if(temp!=0x12)	UARTprintf("[OV2710] 0x3606 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x370b);
		if(temp!=0x40)	UARTprintf("[OV2710] 0x370b reg err : %x \r\n", temp);		

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x380d);
		if(temp!=0x74)	UARTprintf("[OV2710] 0x380d reg err : %x \r\n", temp);		

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x5181);
		if(temp!=0x20)	UARTprintf("[OV2710] 0x5181 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x518f);
		if(temp!=0x00)	UARTprintf("[OV2710] 0x5181 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x4301);
		if(temp!=0xff)	UARTprintf("[OV2710] 0x4301 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x4303);
		if(temp!=0x00)	UARTprintf("[OV2710] 0x4303 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3a00);
		if(temp!=0x78)	UARTprintf("[OV2710] 0x3a00 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x300f);
		if(temp!=0x88)	UARTprintf("[OV2710] 0x300f reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3010);
		if(temp!=0x00)	UARTprintf("[OV2710] 0x3010 reg err PLL : %x \r\n", temp);	
		
		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3011);
		if(temp!=0x28)	UARTprintf("[OV2710] 0x3011 reg err PLL : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3012);
		if(temp!=0x28)	UARTprintf("[OV2710] 0x3012 reg err PLL : %x \r\n", temp);			

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3a18);
		if(temp!=0x00)	UARTprintf("[OV2710] 0x3a18 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3a19);
		if(temp!=0x7a)	UARTprintf("[OV2710] 0x3a19 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x3a13);
		if(temp!=0x54)	UARTprintf("[OV2710] 0x3a13 reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x382e);
		if(temp!=0x0f)	UARTprintf("[OV2710] 0x382e reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x381a);
		if(temp!=0x1a)	UARTprintf("[OV2710] 0x381a reg err : %x \r\n", temp);	

		temp=CMOS_ByteRead(I2C_OV2710_ADDR, 0x5688);
		if(temp!=0x03)	UARTprintf("[OV2710] 0x5688 reg err : %x \r\n", temp);	
*/

}
#endif	



/*  FILE_END_HERE */
