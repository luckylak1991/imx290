//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2005  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	MISP100.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
typedef	enum {			
	MISP_ODM_BAYER_PATTERN_0 = 0,
	MISP_ODM_BAYER_PATTERN_1,
	MISP_ODM_BAYER_PATTERN_3,
	MISP_ODM_BAYER_PATTERN_2,		 
}	MISP_ODM_BAYER_PATTERN_t;

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
MISP_VIDEO_WINDOW	stAE_WIN; // orginal AE window
MISP_VIDEO_WINDOW	stAWB_WIN; // orginal AWB window
MISP_VIDEO_WINDOW	stAF_WIN; // orginal AF window

	// af var filter
#define __AF_FILTER_COMMAND_ADDR__	0x06DB
#define __AF_FILTER_DATA_ADDR__		0x06DC
PWORD af_var_filter;
ROMDATA WORD af_var_filter_table[25] = 
				{	0x0000,
					0xffff,	0xfffe,	0xfefd,	0xfdfc,	0xfcfb,	0xfafa,	0xf9f9,	0xf8f8,
					0xf7f7,	0xf7f6,	0xf6f7,	0xf7f7,	0xf8f8,	0xf9fa,	0xfbfd,	0xfe00,
					0x0103,	0x0406,	0x0809,	0x0b0c,	0x0e0f,	0x1011,	0x1212,	0x1313
				};

float odmRatioW= 1.0f;
float odmRatioH = 1.0f;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void IRISPWM_SetCtrlInit(void)
{
#if defined(__USE_MDIN_i500__)
	WORD hsync, hmin;
#endif

#if defined(__SENSOR_VSYNC_BOOT__)
	UARTprintf("***** [Sensor VSYNC-CHECK] Start \r\n");
	WAIT_ODMVsyncPulse();
	WAIT_ODMVsyncPulse();
	UARTprintf("***** [Sensor VSYNC-CHECK] OK \r\n");	
#else
	UARTprintf("***** [Sensor VSYNC-CHECK] Bypass vsync check \r\n");	
#endif

#if defined(__USE_MDIN_i500__)
	MISP_RegRead(MISP_LOCAL_ID, 0x64F, &hsync);
	hmin = (WORD)hsync*1/100/8;	// 1% of Hsync PWM
	MISP_RegWrite(MISP_LOCAL_ID, 0x67E, hmin);

	UARTprintf("***** [HSYNC-CHECK] SIZE = %04d *****\r\n", hsync);
#else
	MISP_RegWrite(MISP_LOCAL_ID, 0x67E, 0);
	MISP_RegWrite(MISP_LOCAL_ID, 0x67F, 0); // no use
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetODM_Bayer_data_order(BYTE val)
{
	switch (val)
	{
		case MISP_SS_BAYER_PATTERN_0 : MISP_RegField(MISP_LOCAL_ID, 0x600, 0, 2, MISP_ODM_BAYER_PATTERN_0) ; break ;
		case MISP_SS_BAYER_PATTERN_1 : MISP_RegField(MISP_LOCAL_ID, 0x600, 0, 2, MISP_ODM_BAYER_PATTERN_1) ; break ;
		case MISP_SS_BAYER_PATTERN_2 : MISP_RegField(MISP_LOCAL_ID, 0x600, 0, 2, MISP_ODM_BAYER_PATTERN_2) ; break ;
		case MISP_SS_BAYER_PATTERN_3 : MISP_RegField(MISP_LOCAL_ID, 0x600, 0, 2, MISP_ODM_BAYER_PATTERN_3) ; break ;
		default : MISP_RegField(MISP_LOCAL_ID, 0x600, 0, 2, MISP_ODM_BAYER_PATTERN_1) ; break ;
	}	
}

//--------------------------------------------------------------------------------------------------------------------------
void Init_ODM(void) {
	WORD tmpWHV;
	float tmpFH, tmpFV;
	WORD tmpWH, tmpWV;
	WORD aWH, aWV;
	WORD ratioWH, ratioWV;

 // set CONTROL - mainly bayer order
#if defined(__USE_CMOS_AR0331__)		// use aptina cmos
	#if defined (__USE_AR0331_NORMAL_3MegaMODE__)	
	#if !defined (__USE_CMOS_AR0331_TTL__)
	MISP_RegWrite(MISP_LOCAL_ID, 0x600, 0xf078);	// set CONTROL
	#endif
	#endif
#elif defined(__USE_CMOS_MT9P031__)		// use aptina cmos
	#if defined(__USE_CMOS_MT9P031_5M_PIXEL__)	// use 9p031 5M cmos mode
	MISP_RegWrite(MISP_LOCAL_ID, 0x600, 0xf07a);	// set CONTROL
	#endif
#elif defined(__USE_CMOS_IMX076__)	
	MISP_RegWrite(MISP_LOCAL_ID, 0x600, 0xf078);	// set CONTROL

#else

	MISP_RegWrite(MISP_LOCAL_ID, 0x600, 0xf079);	// set CONTROL
					// (15) 1 : 	odm_raw_out_en
					// (14) 1 : 	AE on /off
					// (13) 1 : 	AWB on /off
					// (12) 1 : 	AF on /off

					// (11) 0 :
					// (10) 0 : 	odm_ae_hor_cut_en
					// (09) 0 : 	odm_in_fifo_en
					// (08) 0 : 	odm_af_conv_en

					// (07) 0 : 	odm_ae_clip_en
					// (06) 1 : 	odm_rb_comp_en
					// (05) 1 : 	odm_dc_gain_en
					// (04) 1 : 	odm_wb_gain_en

					// (3:2) 2 : 	odm_wbctl_mode
					// (1:0) 1 : 	odm_data_order

#endif



	MISP_RegWrite(MISP_LOCAL_ID, 0x601, 0x0100);	// set DC-GAIN
	MISP_RegWrite(MISP_LOCAL_ID, 0x602, 0xf251);	// set RB-COMP
	MISP_RegWrite(MISP_LOCAL_ID, 0x603, 0xfffd);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x604, 0x0100);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x605, 0x0000);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x606, 0x01b9);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x607, 0x00e5);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x608, 0x0100);	// set WB-GAIN
	MISP_RegWrite(MISP_LOCAL_ID, 0x609, 0x0100);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x60A, 0x0100);	//

#if defined(__USE_CMOS_IMXx36__)||defined(__USE_CMOS_IMX174__)||\
	defined(__USE_CMOS_IMX249__) || defined(__USE_CMOS_IMX178__) ||\
	defined(__USE_CMOS_IMX185__) || defined(__USE_CMOS_IMX238__) ||\
	defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMXx22__) ||\
	defined(__USE_CMOS_IMX124__) || defined(__USE_CMOS_IMX226__) ||\
	defined (__USE_CMOS_IMX323__)|| defined (__USE_CMOS_IMX274__)||\
	defined (__USE_CMOS_EXT2__)	|| defined (__USE_CMOS_EXT1__)||\
	defined (__USE_CMOS_IMX265__) || defined (__USE_CMOS_IMX326__)||\
	defined (__USE_CMOS_IMX252__) || defined (__USE_CMOS_IMX253__)||\
	defined (__USE_CMOS_IMX385__) || defined (__USE_CMOS_IMX327__)||\
	defined (__USE_CMOS_IMX273__)
	#if defined (__USE_MDIN_i500__) 					// MDIN-i500
		MISP_RegWrite(MISP_LOCAL_ID, 0x60B, 0xF00F);	//MSB12bit is datacut
	#else 												// MDIN-i51X (i510, i540, i550)
		#if defined (__USE_CMOS_EXT2__)	|| defined (__USE_CMOS_EXT1__)
		MISP_RegWrite(MISP_LOCAL_ID, 0x60B, 0xA00F);	//MSB12bit is datacut
		#elif defined (__USE_CMOS_EXT3__)		
		MISP_RegWrite(MISP_LOCAL_ID, 0x60B, 0xA00F);	//MSB12bit is datacut		
		#else
		MISP_RegWrite(MISP_LOCAL_ID, 0x60B, 0xF0EF);	//MSB12bit is datacut
		#endif
	#endif
#endif

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	//MISP_RegWrite(MISP_LOCAL_ID, 0x60C, 0x0070);	// set SNS_3a_gain on
	WBCTRL_SetAWBCtrlPath(0,0,1,1,1);
#else 												// MDIN-i51X (i510, i540, i550)
	//MISP_RegWrite(MISP_LOCAL_ID, 0x60C, 0x7CF0);	// set SNS_3a_gain on
	WBCTRL_SetAWBCtrlPath(0,1,1,1,1);
#endif
// set ODM-WIN
#if 0
	WORD odmHsize, odmVsize;
	MISP_RegRead(MISP_LOCAL_ID, 0x64D, &odmHsize);
	MISP_RegRead(MISP_LOCAL_ID, 0x64E, &odmVsize);
#else
	WORD odmHsize = GetSS_image_active_size_h();
	WORD odmVsize = GetSS_image_active_size_v();
#endif

	MISP_RegWrite(MISP_LOCAL_ID, 0x60D, 0x0014);	//odm_enwin_bgn_x(12:0) (20)
	MISP_RegWrite(MISP_LOCAL_ID, 0x60E, 0x000C);	//odm_enwin_bgn_y(11:0) (12)

	if((odmHsize > 1920) || (odmVsize > 1080)) {
		tmpFH = 20.0f * ((float)odmHsize - 1920.0f) / (4096.0f - 1920.0f)  + ODM_WND_DEF_MUL_W;
		tmpFV = 16.0f * ((float)odmVsize - 1080.0f) / (3000.0f - 1920.0f)  + ODM_WND_DEF_MUL_H;
	} else  if(odmHsize >=900) {
		tmpFH = ODM_WND_DEF_MUL_W - 20.0f * (1920.0f - (float)odmHsize) / (1920.0f - 900.0f);
		tmpFV = ODM_WND_DEF_MUL_H - 16.0f * (1080.0f - (float)odmVsize) / (1080.0f - 504.0f);
	} else {
		tmpFH = 20.0f;
		tmpFV = 16.0f;
	}
	ratioWH = (WORD) tmpFH;	ratioWH += (ratioWH%2);
	ratioWV = (WORD) tmpFV;	ratioWV += (ratioWV%2);
	tmpWHV = (ratioWH<<8) + ratioWV;

	UARTprintf("Input Resolution %d x %d\r\n",odmHsize,odmVsize);
	UARTprintf("Input Resolution boundary %d x %d\r\n",GetSS_image_boundary_size_h(), GetSS_image_boundary_size_v());
	UARTprintf("ODM Window Ratio(step)(h:%d/v:%d)\r\n", ratioWH, ratioWV);
#if	defined(__USE_CMOS_AR0134__) || defined( __USE_CMOS_AR0135__)		//720p	
	MISP_RegWrite(MISP_LOCAL_ID, 0x60F, 0x1E18);		// hard coding //720p
#else
	MISP_RegWrite(MISP_LOCAL_ID, 0x60F, tmpWHV);	//odm_enwin_num_w(13:8) odm_enwin_num_h(5:0) // ratio(step)
#endif

	MISP_RegWrite(MISP_LOCAL_ID, 0x610, 0x0000);	// set ODM-WB

	aWH = odmHsize /ratioWH - 1;
	if(aWH > 63) aWH = 63;
	aWV = odmVsize /ratioWV - 1;
	if(aWV > 63) aWV = 63;
	stAWB_WIN.x = 0;
	stAWB_WIN.y = 0;
	stAWB_WIN.w = aWH;
	stAWB_WIN.h = aWV;

#if	defined(__USE_CMOS_AR0134__) || defined( __USE_CMOS_AR0135__)		//720p
	MISP_RegWrite(MISP_LOCAL_ID, 0x611, 0x291D);		// hard coding //720p
	MISP_RegWrite(MISP_LOCAL_ID, 0x612, 180);	// set WBC-N // 90% of (pxlW * pxlH)
#else
	UARTprintf("AWB Window SIZE : (h:%d/v:%d)\r\n", aWH, aWV);
	UARTprintf("AWB Window real SIZE : (h:%d/v:%d)\r\n", aWH*ratioWH, aWV*ratioWV);
	MISP_RegWrite(MISP_LOCAL_ID, 0x611, ((aWH<<8)+aWV));

	UARTprintf("WBC-N : %d\r\n", (ratioWH*ratioWV)/3);
	MISP_RegWrite(MISP_LOCAL_ID, 0x612, (ratioWH*ratioWV)/3);	// set WBC-N // 90% of (pxlW * pxlH)
	//MISP_RegWrite(MISP_LOCAL_ID, 0x612, 0x0140);	// set WBC-N // 90% of (pxlW * pxlH)
#endif
	MISP_RegWrite(MISP_LOCAL_ID, 0x613, 0xc018);	// set pxl-Y
	MISP_RegWrite(MISP_LOCAL_ID, 0x614, 0xc018);	// set prm-Y
	MISP_RegWrite(MISP_LOCAL_ID, 0x615, 0xc018);	// set ctl-Y
	MISP_RegWrite(MISP_LOCAL_ID, 0x616, 0x0000);	// set yel-M
	MISP_RegWrite(MISP_LOCAL_ID, 0x617, 0xffff);	// set cyn-M
	MISP_RegWrite(MISP_LOCAL_ID, 0x618, 0x0000);	// set yel-W
	MISP_RegWrite(MISP_LOCAL_ID, 0x619, 0xffff);	// set cyn-W
	MISP_RegWrite(MISP_LOCAL_ID, 0x61A, 0x0018);	// set PRM-R
	MISP_RegWrite(MISP_LOCAL_ID, 0x61B, 0x0018);	// set PRM-G
	MISP_RegWrite(MISP_LOCAL_ID, 0x61C, 0x0018);	// set PRM-B
	MISP_RegWrite(MISP_LOCAL_ID, 0x61D, 0xffff);	// set offset

	MISP_RegWrite(MISP_LOCAL_ID, 0x620, 0x0080);	// set polygon
	MISP_RegWrite(MISP_LOCAL_ID, 0x621, 0xa180);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x622, 0x008b);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x623, 0x7a4d);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x624, 0x00a9);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x625, 0x642d);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x626, 0x00db);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x627, 0x45f9);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x628, 0x0121);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x629, 0x206b);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x62A, 0x0179);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x62B, 0x075b);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x62C, 0x71e1);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x62D, 0xce53);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x62E, 0x7211);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x62F, 0x7bab);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x630, 0x7e00);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x631, 0xc92e);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x632, 0x7e4f);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x633, 0xde99);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x634, 0x7ea2);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x635, 0xf100);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x636, 0x7f00);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x637, 0xff12);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x638, 0x0f37);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x639, 0x026e);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x63A, 0x0f6b);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x63B, 0x02df);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x63C, 0x0f84);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x63D, 0x00c0);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x63E, 0x7e00);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x63F, 0xb50e);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x640, 0x7e4f);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x641, 0xcce8);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x642, 0x7ea2);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x643, 0xe1bf);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x644, 0x7f00);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x645, 0xf258);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x646, 0x7f37);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x647, 0xf6fc);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x648, 0x7f6b);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x649, 0xf874);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x64A, 0x7f84);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x64B, 0xf6c0);	//

	tmpFH = (float)odmHsize/1920.0f;
	tmpFV = (float)odmVsize/1080.0f;
	odmRatioW = 40.0f * tmpFH / (float)ratioWH;
	odmRatioH = 32.0f * tmpFV / (float)ratioWV;
	UARTprintf("odmRatio(w:%f/h:%f)\r\n", odmRatioW, odmRatioH);
	tmpWH = (WORD)(13.0f*odmRatioW);
	if(tmpWH > 31) tmpWH = 31;
	stAE_WIN.x = tmpWH;
	tmpWV = (WORD)(9.0f*odmRatioH);
	if(tmpWV > 31) tmpWV = 31;
	stAE_WIN.y = tmpWV;
	tmpWHV = (tmpWH<<8) + tmpWV;
	MISP_RegWrite(MISP_LOCAL_ID, 0x680, tmpWHV);	// set ODM-AE

	tmpWH = (WORD)(AE_WND_DEF_SIZE_W*odmRatioW);
	if(tmpWH > 63) tmpWH = 63;
	stAE_WIN.w = tmpWH;
	tmpWV = (WORD)(AE_WND_DEF_SIZE_H*odmRatioH);
	if(tmpWV > 63) tmpWV = 63;
	stAE_WIN.h = tmpWV;
	tmpWH = (tmpWH<<8) + tmpWV;
	MISP_RegWrite(MISP_LOCAL_ID, 0x681, tmpWH);	//

	MISP_RegWrite(MISP_LOCAL_ID, 0x688, 0x0404);	// set AE-HLC
	MISP_RegWrite(MISP_LOCAL_ID, 0x689, 0x110b);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x68A, 0x1804);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x68B, 0x110b);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x68C, 0x0412);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x68D, 0x110b);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x68E, 0x1812);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x68F, 0x110b);	//

	// set ODM-AF
#if defined(__USE_AF_FAR_LIMIT__)
	WORD bWH, bWV;
	bWH=aWH; bWV=aWV;
	aWH=aWH/3; aWV=aWV/3;
	bWH=bWH-aWH; bWV=bWV-aWV;
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x6A0, (((bWH/2)<<8)+(bWV/2)));	//odm_afw_start_a = x(13:8) y(5:0)   //refer 60d~60f  //xstart = 20+40x1 = 60  ystart = 0x32 = 0
	MISP_RegWrite(MISP_LOCAL_ID, 0x6A1, ((aWH<<8)+aWV));	//odm_afw_width_a = width(13:8) height(5:0) (45,33)  //width = 45x40 = 1800 33x32 = 1056   ====> (60,0) ~ (1860,1056)
	MISP_RegWrite(MISP_LOCAL_ID, 0x6A2, (((aWH/4+bWH/2)<<8)+(aWV/4+bWV/2)));	//odm_afw_start_b = (12,8) // xstart = 12x40 = 480 ystart = 8x32 = 256
	MISP_RegWrite(MISP_LOCAL_ID, 0x6A3, (((aWH/2)<<8)+aWV/2));	//odm_afw_width_b (22,16) // width = 22x40 = 880 height = 16x32 = 512  ====> (480,256) ~ (1360,768)
#else
	WORD bWH, bWV;
	bWH=aWH; bWV=aWV;
	aWH=aWH*2/3; aWV=aWV*2/3;
	bWH=bWH-aWH; bWV=bWV-aWV;

	UARTprintf("res: %5d x %5d \r\n",odmHsize,odmVsize);

	MISP_RegWrite(MISP_LOCAL_ID, 0x6A0, (((bWH/2)<<8)+(bWV/2)));	//odm_afw_start_a = x(13:8) y(5:0)   //refer 60d~60f  //xstart = 20+40x1 = 60  ystart = 0x32 = 0
	MISP_RegWrite(MISP_LOCAL_ID, 0x6A1, ((aWH<<8)+aWV));	//odm_afw_width_a = width(13:8) height(5:0) (45,33)  //width = 45x40 = 1800 33x32 = 1056   ====> (60,0) ~ (1860,1056)
	MISP_RegWrite(MISP_LOCAL_ID, 0x6A2, (((aWH/4+bWH/2)<<8)+(aWV/4+bWV/2)));	//odm_afw_start_b = (12,8) // xstart = 12x40 = 480 ystart = 8x32 = 256
	MISP_RegWrite(MISP_LOCAL_ID, 0x6A3, (((aWH/2)<<8)+aWV/2));	//odm_afw_width_b (22,16) // width = 22x40 = 880 height = 16x32 = 512  ====> (480,256) ~ (1360,768)
#endif
	stAF_WIN.x = 0;
	stAF_WIN.y = 0;
	stAF_WIN.w = aWH;
	stAF_WIN.h = aWV;

	WORD wBuf[4];

	MISP_RegRead(MISP_LOCAL_ID, 0x6A0, &wBuf[0]);
	MISP_RegRead(MISP_LOCAL_ID, 0x6A1, &wBuf[1]);
	UARTprintf("AF Window0 start(h:%d / v:%d)...\r\n", (wBuf[0]>>8), (wBuf[0]&0xff));
	UARTprintf("AF Window0 size(h:%d / v:%d)...\r\n", (wBuf[1]>>8), (wBuf[1]&0xff));
	UARTprintf("AF Window real size(w:%d / h:%d)...\r\n", (wBuf[1]>>8)*ratioWH, (wBuf[1]&0xff)*ratioWV);

	MISP_RegRead(MISP_LOCAL_ID, 0x6A2, &wBuf[0]);
	MISP_RegRead(MISP_LOCAL_ID, 0x6A3, &wBuf[1]);
	UARTprintf("AF Window1 start(h:%d / v:%d)...\r\n", (wBuf[0]>>8), (wBuf[0]&0xff));
	UARTprintf("AF Window1 size(h:%d / v:%d)...\r\n", (wBuf[1]>>8), (wBuf[1]&0xff));
	UARTprintf("AF Window real size(w:%d / h:%d)...\r\n", (wBuf[1]>>8)*ratioWH, (wBuf[1]&0xff)*ratioWV);

	MISP_RegWrite(MISP_LOCAL_ID, 0x6A4, 0x0000);	//null
	MISP_RegWrite(MISP_LOCAL_ID, 0x6A5, 0x00f0);	//odm_af_luma_max // if over then 80-data is zero
	MISP_RegWrite(MISP_LOCAL_ID, 0x6A6, 0x0400);	//null
	MISP_RegWrite(MISP_LOCAL_ID, 0x6A7, 0x0a14);	//null
	MISP_RegWrite(MISP_LOCAL_ID, 0x6A8, 0x4444);	//odm_af_weight_a
	MISP_RegWrite(MISP_LOCAL_ID, 0x6A9, 0x4444);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6AA, 0x4444);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6AB, 0x4444);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6AC, 0x4444);	//odm_af_weight_b
	MISP_RegWrite(MISP_LOCAL_ID, 0x6AD, 0x4444);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6AE, 0x4444);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6AF, 0x4444);	//

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B0, 0x0102);	// set AF-FLT    
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B1, 0x02ff);	// 0.30 ~ 2.10
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B2, 0xff02);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B3, 0x02fe);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B4, 0xfd00);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B5, 0x00fb);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B6, 0xfbff);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B7, 0x00fa);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B8, 0xfc03);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B9, 0x02fd);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6BA, 0x0009);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6BB, 0x06ff);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6BC, 0x060e);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6BD, 0x07ff);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6BE, 0x070e);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6BF, 0x00f7);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C0, 0x0406);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C1, 0xefe9);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C2, 0xfdf9);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C3, 0xd6d9);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C4, 0x00ed);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C5, 0xa9ce);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C6, 0x7de9);	//
#else 												// MDIN-i51X (i510, i540, i550)
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B0, 0x0000);	// set AF-FLT    
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B1, 0x0000);	// 0.30 ~ 2.10
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B2, 0x0000);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B3, 0x0000);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B4, 0x0000);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B5, 0x0000);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B6, 0x0000);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B7, 0x0000);	///
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B8, 0x0000);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6B9, 0x0000);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6BA, 0x0000);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6BB, 0x0000);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6BC, 0x0000);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6BD, 0x0000);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6BE, 0x0000);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6BF, 0x0102);	///
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C0, 0x02ff);	// 
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C1, 0xff02);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C2, 0x02fe);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C3, 0xfd00);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C4, 0x00fb);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C5, 0xfbff);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C6, 0x00fa);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C7, 0xfc03);	///
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C8, 0x02fd);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C9, 0x0009);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6CA, 0x06ff);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6CB, 0x060e);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6CC, 0x07ff);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6CD, 0x070e);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6CE, 0x00f7);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6CF, 0x0406);	///
	MISP_RegWrite(MISP_LOCAL_ID, 0x6D0, 0xefe9);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6D1, 0xfdf9);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6D2, 0xd6d9);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6D3, 0x00ed);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6D4, 0xa9ce);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6D5, 0x7de9);	//
#endif

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C7, 0x0000);	// set AF-VAR
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C8, 0xffff);	// 0.05 ~ 0.20
	MISP_RegWrite(MISP_LOCAL_ID, 0x6C9, 0xfffe);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6CA, 0xfefd);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6CB, 0xfdfc);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6CC, 0xfcfb);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6CD, 0xfafa);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6CE, 0xf9f9);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6CF, 0xf8f8);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6D0, 0xf7f7);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6D1, 0xf7f6);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6D2, 0xf6f7);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6D3, 0xf7f7);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6D4, 0xf8f8);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6D5, 0xf9fa);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6D6, 0xfbfd);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6D7, 0xfe00);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6D8, 0x0103);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6D9, 0x0406);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6DA, 0x0809);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6DB, 0x0b0c);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6DC, 0x0e0f);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6DD, 0x1011);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6DE, 0x1212);	//
	MISP_RegWrite(MISP_LOCAL_ID, 0x6DF, 0x1313);	//
#else 												// MDIN-i51X (i510, i540, i550)
	af_var_filter = (PWORD)af_var_filter_table;

	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_COMMAND_ADDR__, 0x8000) ; // LUT write enable(15) + start addr(7:0)
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[0]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[1]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[2]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[3]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[4]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[5]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[6]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[7]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[8]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[9]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[10]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[11]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[12]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[13]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[14]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[15]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[16]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[17]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[18]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[19]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[20]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[21]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[22]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[23]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_DATA_ADDR__, af_var_filter[24]);
	MISP_RegWrite(MISP_LOCAL_ID, __AF_FILTER_COMMAND_ADDR__, 0x0000) ; // LUT read enable(15) + start addr(7:0)
#endif

#if defined(__ECOM1_UNION_12X__)
	AECTRL_PIRIS_Init();
#endif

	IRISPWM_SetCtrlInit();	// set IRIS PWM Init

	SetODM_Bayer_data_order (GetSS_bayer_data_order()) ;

#if defined(__USE_GET_AWB_RGB_INFO__)
#if !defined(__USE_GET_RGB_MODE__)
	ColorRAW_ProcessInit();
	SetColorRAWViewTrigger();
	
#if defined(__USE_AWB_RGB_WDR_MEM__)
	// set wdr write mode for capture bayer data
	SetDDRMAP_WDR(0, 0, MISP_DDRMAP_8b, MISP_DDRMAP_8b);
	SetSS_wdr_write_mode(MISP_WDR_MEM_WIRTE_HTM_MODE, MISP_DDRMAP_8b, 0x01);
#if defined(__USE_CMOS_IMX226__) || defined (__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX334__)
    #if defined(__USE_GET_AWB_RGB_INFO_SIZE_4__) && (defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX334__))
	SetSS_WDRSWimage_size(GetSS_image_boundary_size_h()/8, GetSS_image_boundary_size_v()/8, \
							odmHsize/4, odmVsize/64);	// shrink size for mem bandwidth
    #else
	SetSS_WDRSWimage_size(GetSS_image_boundary_size_h()/8, GetSS_image_boundary_size_v()/8, \
							odmHsize/8, odmVsize/8);	// shrink size for mem bandwidth
    #endif
#else
	SetSS_WDRSWimage_size(GetSS_image_boundary_size_h(), GetSS_image_boundary_size_v(), \
							odmHsize, odmVsize);
#endif
#endif
#endif

#endif
}

/*  FILE_END_HERE */
