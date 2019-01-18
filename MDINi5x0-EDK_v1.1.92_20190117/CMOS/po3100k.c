// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_PO3100K__)
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

void PO3100K_Initialize(void)
{	
		Delay_mSec(20);
//	UARTprintf("PO3100K_Initialize\n\r");
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x00);	//page a
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x2D,0x01);	//hidden
//		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x28,0x00);	// pclk, data drivability [0Step]				
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x28,0xA0);	// pclk, data drivability [2Step up]
//		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x29,0x98);	//pad_control3 -orginal
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x29,0x9C);	//pad_control3 -change for lsb out enable(sensor md disable)

		Delay_mSec(20);
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x00);	//page a
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x04,0x02);	//chip_mode
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x05,0x03);	//mirror
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x41,0x02);	//pll_ms
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x42,0x0B);	//pll_ns
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x40,0x3C); 	//pll_control
		Delay_mSec(30);				
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x05,0x00);	//mirror
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x40,0x2C); 	//pll_control
		
		Delay_mSec(20);
//		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x06,0x06);	//framewidth_h	//org
//		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x07,0x71);	//framewidth_l	//org
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x06,0x06);	//framewidth_h		//org-2 for 2fwdr
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x07,0x6F);	//framewidth_l		//org-2 for 2fwdr
		
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x08,0x02);	//fd_fheight_a_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x09,0xED);	//fd_fheight_a_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x0A,0x02);	//fd_fheight_b_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x0B,0xED);	//fd_fheight_b_l
	
		#if 1		//use 1304x730 out
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x00);	//page a		
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x0C,0x00);	//windowx1_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x0D,0x01);	//windowx1_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x0E,0x00);	//windowy1_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x0F,0x01);	//windowy1_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x10,0x05);	//windowx2_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x11,0x18);	//windowx2_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x12,0x02);	//windowy2_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x13,0xDC);	//windowy2_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x14,0x00);	//vsyncstartrow_f0_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x15,0x0E);	//vsyncstartrow_f0_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x16,0x02);	//vsyncstoprow_f0_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x17,0xE8);	//vsyncstoprow_f0_l				
		#else		//use 1280x720 exactily out
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x00);	//page a				
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x0C,0x00);	//windowx1_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x0D,0x05);	//windowx1_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x0E,0x00);	//windowy1_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x0F,0x05);	//windowy1_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x10,0x05);	//windowx2_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x11,0x04);	//windowx2_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x12,0x02);	//windowy2_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x13,0xD4);	//windowy2_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x14,0x00);	//vsyncstartrow_f0_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x15,0x17);	//vsyncstartrow_f0_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x16,0x02);	//vsyncstoprow_f0_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x17,0xE8);	//vsyncstoprow_f0_l		
		#endif		

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x00);	//page a
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x33,0x02);	//hidden
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x34,0x01);	//hidden
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x36,0x80);	//hidden

		//set po3100k pll(frame rate)
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x00);	//page a

		#if	defined(__USE_PO3100K_30to1080p30__)			// sensor 720p30fps, out 1080p30fps
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x25,0x08);	//clk1 div 0x00 60fps, 0x80 30fps
		#else											// sensor 720p60fps
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x25,0x00);	//clk1 div 0x00 60fps, 0x08 30fps		
		#endif
		Delay_mSec(20);

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x26,0x41);	//hidden	
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x38,0x58);	//hidden

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x01);	//page b
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x1E,0x0E);	//hidden
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x26,0x03);	//hidden

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x01);	//page b
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xB1,0x30);	//hidden

		#if 1	//use 1304x730 out
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x01);	//page b
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x33,0x02);	//tg vsync start
//		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x33,0x08);	//tg vsync start
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x35,0xEC);	//tg vsync stop		
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x36,0x01);	//TG vsync start h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x37,0x0A);	//TG vsync start l    
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x38,0x06);	//TG vsync stop h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x39,0x2A);	//TG vsync stop l
		#endif		
		Delay_mSec(20);

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x06,0x98);	//auto control3

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x01);	//page b
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xA4,0x88);	//hidden
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xA5,0x88);	//hidden
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xA6,0x88);	//hidden
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xA7,0x00);	//hidden
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xA8,0x00);	//hidden
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xA9,0x08);	//hidden

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x06,0xB8);	//autocontrol 3

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x75,0x28);	//AWB gain clamping fitting control - awb_rgain_min1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x76,0x28);	//AWB gain clamping fitting control - awb_rgain_min2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x77,0x78);	//AWB gain clamping fitting control - awb_rgain_max1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x78,0x78);	//AWB gain clamping fitting control - awb_rgain_max2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x79,0x48);	//AWB gain clamping fitting control - awb_bgain_min1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x7A,0x48);	//AWB gain clamping fitting control - awb_bgain_min2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x7B,0xB8);	//AWB gain clamping fitting control - awb_bgain_max1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x7C,0xB8);	//AWB gain clamping fitting control - awb_bgain_max2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x7D,0x01);	//awb_cmp_th1_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x7E,0x00);	//awb_cmp_th1_m
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x7F,0x02);	//awb_cmp_th2_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x80,0x07);	//awb_cmp_th2_m

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x73,0x08);	//awb_lock
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x74,0x04);	//awb_speed

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x51,0x10);	//awb_size
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x52,0xE0);	//awb_maxc
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x53,0x02);	//awb_minc
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x54,0x02);	//awb_redp
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x55,0x40);	//awb_gth
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x56,0xC0);	//awb_nth
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x57,0x04);	//awb_ath
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x58,0x6E);	//awb_ired
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x59,0x45);	//awb_iblu

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5A,0x23);	//awb_rmin1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5B,0x4B);	//awb_rmax1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5C,0x64);	//awb_bmin1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5D,0xAA);	//awb_bmax1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5E,0x23);	//awb_dbup1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5F,0x28);	//awb_dblw1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x60,0x4B);	//awb_rmin2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x61,0x73);	//awb_rmax2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x62,0x3C);	//awb_bmin2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x63,0x87);	//awb_bmax2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x64,0x2D);	//awb_dbup2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x65,0x2D);	//awb_dblw2

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x6E,0x3A);	//ratio_axis_a
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x6F,0x50);	//ratio_axis_b
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x70,0x60);	//ratio_axis_c

		//CS matrix / lens gain fitting reference
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x03);	//page d
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x16,0x3A);	//axis_a
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x17,0x50);	//axis_b
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x18,0x60);	//axis_c

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x05,0x64);	//auto_control_2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3B,0xA0);	//Min / max ytarget control reference : max_yt1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3C,0x70);	//Min / max ytarget control reference : max_yt2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3D,0x70);	//Min / max ytarget control reference : min_yt1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3E,0x50);	//Min / max ytarget control reference : min_yt2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3F,0x24);	//Min / max ytarget control reference : gg_ref0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x40,0x4B);	//Min / max ytarget control reference : gg_ref1

		//set AE reference
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x12,0x02);	//expfrmh_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x13,0xE8);	//expfrmh_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x14,0x02);	//midfrmheight_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x15,0xE8);	//midfrmheight_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x16,0x02);	//maxfrmheight_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x17,0xE8);	//maxfrmheight_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x1A,0x0C);	//minexp_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x1B,0x00);	//midexp_t
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x1C,0x5D);	//midexp_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x1D,0x00);	//midexp_m
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x1E,0x00);	//maxexp_t
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x1F,0x5D);	//maxexp_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x20,0x00);	//maxexp_m

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x48,0x08);	//ae_up_speed
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x49,0x08);	//ae_down_speed
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x4A,0x08);	//ae_lock

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x2C,0xBB);	//Saturation decision TH : ae_ysat

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x41,0x04);	//Ytarget control reference : sratio_ref0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x42,0x08);	//Ytarget control reference : sratio_ref1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x43,0x10);	//Ytarget control reference : sratio_ref2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x44,0x20);	//Ytarget control reference : sratio_ref3
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x2E,0x04);	//sratio_weight 

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x00);	//page a
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x4F,0x08);	//flicker_control1

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x00);	//page a
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x59,0x00);	//Flicker period for A state : fd_period_a_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5A,0xBA);	//Flicker period for A state : fd_period_a_m
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5B,0x00);	//Flicker period for A state : fd_period_a_l

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x02);	//page c
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x04,0x31);	//isp_func_0(po3100k isp func on/off)
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x05,0x12);	//isp_func_1(po3100k isp func on/off)
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x06,0x00);	//isp_func_2(po3100k isp func on/off)
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x09,0x00);	//isp_func_5(po3100k isp func on/off)
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x0A,0x11);	//isp_func_6(po3100k isp func on/off)
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x0B,0x80);	//isp_func_7(po3100k isp func on/off)
		
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x33,0x37);	//Color correction matrix value : ccr_m11
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x34,0x90);	//Color correction matrix value : ccr_m12
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x35,0x87);	//Color correction matrix value : ccr_m13
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x36,0x8E);	//Color correction matrix value : ccr_m21
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x37,0x3B);	//Color correction matrix value : ccr_m22
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x38,0x8C);	//Color correction matrix value : ccr_m23
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x39,0x82);	//Color correction matrix value : ccr_m31
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3A,0x98);	//Color correction matrix value : ccr_m32
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3B,0x3A);	//Color correction matrix value : ccr_m32

		//Dark bilateral filter fitting control
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x03);	//page d
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x0C,0x25);	//dark_blf0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x0D,0x88);	//dark_blf1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x0E,0x00);	//dark_blf2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x0F,0x25);	//dark_blf

		//Y gamma1 coefficient
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x02);	//page C
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3D,0x00);	//ygm1_y0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3E,0x0F);	//ygm1_y1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3F,0x26);	//ygm1_y2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x40,0x37);	//ygm1_y3
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x41,0x43);	//ygm1_y4
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x42,0x54);	//ygm1_y5
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x43,0x62);	//ygm1_y6
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x44,0x77);	//ygm1_y7
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x45,0x88);	//ygm1_y8
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x46,0xA4);	//ygm1_y9
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0xBB);	//ygm1_y10
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x48,0xCF);	//ygm1_y11
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x49,0xE0);	//ygm1_y12
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x4A,0xF1);	//ygm1_y13
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x4B,0xFF);	//ygm1_y14

		//RGB gamma1 coefficient
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x02);	//page C
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5B,0x00);	//cgm1_y0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5C,0x05);	//cgm1_y1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5D,0x14);	//cgm1_y2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5E,0x25);	//cgm1_y3
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5F,0x34);	//cgm1_y4
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x60,0x4B);	//cgm1_y5
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x61,0x5B);	//cgm1_y6
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x62,0x73);	//cgm1_y7
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x63,0x86);	//cgm1_y8
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x64,0xA3);	//cgm1_y9
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x65,0xBA);	//cgm1_y10
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x66,0xCE);	//cgm1_y11
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x67,0xE0);	//cgm1_y12
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x68,0xF0);	//cgm1_y13
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x69,0xFF);	//cgm1_y14

		//RGB gamma2 coefficient
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x02);	//page C
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x6A,0x00);	//cgm2_y0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x6B,0x0B);	//cgm2_y1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x6C,0x13);	//cgm2_y2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x6D,0x1A);	//cgm2_y3
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x6E,0x20);	//cgm2_y4
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x6F,0x2B);	//cgm2_y5
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x70,0x36);	//cgm2_y6
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x71,0x49);	//cgm2_y7
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x72,0x5A);	//cgm2_y8
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x73,0x7B);	//cgm2_y9
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x74,0x98);	//cgm2_y10
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x75,0xB4);	//cgm2_y11
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x76,0xCE);	//cgm2_y12
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x77,0xE7);	//cgm2_y13
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x78,0xFF);	//cgm2_y14

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x02);	//Page c
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x8D,0x30);	//Y weight 

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x02);	//page c
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x2F,0x20);	//edge_gain
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x30,0x40);	//Positive max edge clamp gain
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x31,0x40);	//Negative max edge clamp gain

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x09,0x00);	//hidden

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x03);	//page d
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x26,0x00);	//hidden
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x27,0x10);	//hidden
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x28,0x40);	//hidden

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x03);	//page d
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x77,0x00);	//de-color dark filter : dark_dc0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x78,0x08);	//de-color dark filter : dark_dc1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x79,0x10);	//de-color dark filter : dark_dc2

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x03);	//page d
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xA1,0x20);	//Dark edge BLF fitting control : e_dark_blf0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xA2,0x12);	//Dark edge BLF fitting control : e_dark_blf1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xA3,0x04);	//Dark edge BLF fitting control : e_dark_blf2

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x02);	//page c
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x95,0x00);	//Dark Y brightness fitting control : ybrightness_ref0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x96,0x00);	//Dark Y brightness fitting control : ybrightness_ref1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x97,0x10);	//Dark Y brightness fitting control : ybrightness_ref2

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x05,0x64);	//auto controll 2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3B,0x90);	//Min / max ytarget control reference : max_yt1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3C,0x78);	//Min / max ytarget control reference : max_yt2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3D,0x70);	//Min / max ytarget control reference : min_yt1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3E,0x78);	//Min / max ytarget control reference : min_yt2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3F,0x24);	//Min / max ytarget control reference : gg_ref0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x40,0x4B);	//Min / max ytarget control reference : gg_ref1

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x02);	//page c
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3D,0x00);	//Y gamma1 coefficient : ygm1_y0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3E,0x27);	//Y gamma1 coefficient : ygm1_y1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x3F,0x36);	//Y gamma1 coefficient : ygm1_y2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x40,0x40);	//Y gamma1 coefficient : ygm1_y3
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x41,0x49);	//Y gamma1 coefficient : ygm1_y4
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x42,0x58);	//Y gamma1 coefficient : ygm1_y5
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x43,0x64);	//Y gamma1 coefficient : ygm1_y6
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x44,0x78);	//Y gamma1 coefficient : ygm1_y7
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x45,0x89);	//Y gamma1 coefficient : ygm1_y8
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x46,0xA4);	//Y gamma1 coefficient : ygm1_y9
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0xBB);	//Y gamma1 coefficient : ygm1_y10
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x48,0xCF);	//Y gamma1 coefficient : ygm1_y11
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x49,0xE0);	//Y gamma1 coefficient : ygm1_y12
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x4A,0xF1);	//Y gamma1 coefficient : ygm1_y13
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x4B,0xFF);	//Y gamma1 coefficient : ygm1_y14

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x02);	//page c
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5B,0x00);	//RGB gamma1 coefficient : cgm1_y0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5C,0x0B);	//RGB gamma1 coefficient : cgm1_y1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5D,0x13);	//RGB gamma1 coefficient : cgm1_y2
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5E,0x1A);	//RGB gamma1 coefficient : cgm1_y3
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x5F,0x20);	//RGB gamma1 coefficient : cgm1_y4
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x60,0x2B);	//RGB gamma1 coefficient : cgm1_y5
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x61,0x36);	//RGB gamma1 coefficient : cgm1_y6
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x62,0x49);	//RGB gamma1 coefficient : cgm1_y7
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x63,0x5A);	//RGB gamma1 coefficient : cgm1_y8
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x64,0x7B);	//RGB gamma1 coefficient : cgm1_y9
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x65,0x98);	//RGB gamma1 coefficient : cgm1_y10
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x66,0xB4);	//RGB gamma1 coefficient : cgm1_y11
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x67,0xCE);	//RGB gamma1 coefficient : cgm1_y12
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x68,0xE7);	//RGB gamma1 coefficient : cgm1_y13
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x69,0xFF);	//RGB gamma1 coefficient : cgm1_y14

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x03);	//page d
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x77,0x00);	//de-color dark filter : dark_dc0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x78,0x00);	//de-color dark filter : dark_dc1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x79,0x00);	//de-color dark filter : dark_dc2

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x03);	//page d
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xA1,0x20);	//Dark edge BLF fitting control : e_dark_blf0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xA2,0x20);	//Dark edge BLF fitting control : e_dark_blf1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xA3,0x20);	//Dark edge BLF fitting control : e_dark_blf2
		//AE reference
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x12,0x02);	//expfrmh_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x13,0xE8);	//expfrmh_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x14,0x02);	//midfrmheight_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x15,0xE8);	//midfrmheight_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x16,0x02);	//maxfrmheight_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x17,0xE8);	//maxfrmheight_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x1B,0x00);	//midexp_t
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x1C,0x74);	//midexp_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x1D,0x40);	//midexp_m
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x1E,0x00);	//maxexp_t
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x1F,0x74);	//maxexp_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x20,0x40);	//maxexp_m

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x02);	//page c
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x2F,0x14);	//edge_gain
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x30,0x40);	//Positive max edge clamp gain
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x31,0x40);	//Negative max edge clamp gain

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x03);	//page d
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x2E,0x7F);	//Dark bilateral filter fitting control : dark_blf0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x2F,0x7F);	//Dark bilateral filter fitting control : dark_blf1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x30,0x7F);	//Dark bilateral filter fitting control : dark_blf2

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x03);	//page d
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x32,0x00);	//Dark noise floor fitting control : dark_nf0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x33,0x00);	//Dark noise floor fitting control : dark_nf1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x34,0x00);	//Dark noise floor fitting control : dark_nf2

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x03);	//page d
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xA1,0x30);	//Dark edge BLF fitting control : e_dark_blf0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xA2,0x7F);	//Dark edge BLF fitting control : e_dark_blf1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xA3,0x7F);	//Dark edge BLF fitting control : e_dark_blf2

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x02);	//page c
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x95,0x00);	//Dark Y brightness fitting control : ybrightness_ref0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x96,0x00);	//Dark Y brightness fitting control : ybrightness_ref1
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x97,0x00);	//Dark Y brightness fitting control : ybrightness_ref2

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x03);	//page d
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x26,0x00);	//hidden
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x27,0x10);	//hidden
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x28,0x20);	//hidden

		//set AE reference
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x12,0x02);	//expfrmh_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x13,0xE8);	//expfrmh_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x14,0x02);	//midfrmheight_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x15,0xE8);	//midfrmheight_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x16,0x02);	//maxfrmheight_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x17,0xE8);	//maxfrmheight_l
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x1B,0x00);	//midexp_t
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x1C,0xBA);	//midexp_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x1D,0x00);	//midexp_m
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x1E,0x00);	//maxexp_t
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x1F,0xBA);	//maxexp_h
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x20,0x00);	//maxexp_m

		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x02);	//page c
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x29,0x41);   //format
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x9A,0x81);	//sync controll 0
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x9C,0x00);	//hidden, add for lsb out and sensor md disable
		
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x04);	//page e
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x04,0x9C);	//auto controll 1. use po3100k internal ae
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x47,0x4A);	//auto controll 1. use po3100k internal ae		0x80 to 0x4A
//		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x04,0x9F);	//auto controll 1 use misp ae
//		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xA1,0x10);	//ref_gain4 hidden register, pixel recommend
		

		//set integration time
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0x03,0x01);	//page b
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xC0,0x02);	//
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xC1,0xE8);	//
		CMOS_ByteWrite(I2C_PO3100K_ADDR,0xC2,0x00);	//

}

#endif	
/*  FILE_END_HERE */
