/*
 * EDID Table
 */


#if defined(__USE_LT8619_RX__)


 const unsigned char EDID_TABLE_NAME[256] = {
/* Base EDID */
// (00H)      Header
	0x00,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0x00,
// (08H-09H)  ID Manufacturer Name, 3-character code
	word2bytes(swap16(mfgname2id(MFG_CODE1,MFG_CODE2,MFG_CODE3))),
// (0AH-0BH)  Product ID Code, 2-byte code
	word2bytes(PROD_CODE),
// (0CH-0FH)  Serial Number, 32 bits, little endian, 0 if not used
	dword2bytes(SERIAL),
// (10H)      Week of Manufacture (1-54), 0 if not used
	WEEK,
// (11H)      Year of Manufacture (1990-2245), if week=255, it is the model year instead
	YEAR-1990,
// (12H)      EDID version 1.3
	0x01,  0x03,

//           Basic Display Parameters
//(14H)      VIDEO INPUT DEFINITION: Digital Signal
	1<<7|	//[7] digital input if 1
	0<<4|	//[6:4] color bit depth = undefined
	0<<0,	//[3:0] digital video interface = undefined
//(15H)      Maximum Horizontal Image Size in cm
//(16H)      Maximum Vertical Image Size in cm
	0x00, 0x00,	//undefined
//(17H)      Display Gamma, (gamma*100)-100
	(GAMMA)*100-100,
//(18H)      DPMS and Supported Feature(s):
//			 Display Type = RGB Color, Preferred Timing Mode
	SUPPORT_STANDBY<<7|	//[7] standby mode supported if 1
	SUPPORT_SUSPEND<<6|	//[6] suspend mode supported if 1
	SUPPORT_ACTIVEOFF<<5|	//[5] active-off supported if 1
	SUPPORT_YCBCR422<<4|	//[4] YCbCr 4:2:2 supported if 1
	SUPPORT_YCBCR444<<3|	//[3] YCbCr 4:4:4 supported if 1
	0<<2|	//[2] color space = sRGB standard if 1
	1<<1|	//[1] preferred timing mode specified in descriptor block 1 if 1
	0<<0,	//[0] continuous frequency if 1, multi-mode if 0

//(19H-22H)  CHROMA INFO: 0-0.999 (1023/1024)
	cc_lsbs4(cc2bits10(CC_RED_X),cc2bits10(CC_RED_Y),cc2bits10(CC_GREEN_X),cc2bits10(CC_GREEN_Y)),
	cc_lsbs4(cc2bits10(CC_BLUE_X),cc2bits10(CC_BLUE_Y),cc2bits10(CC_WHITE_X),cc2bits10(CC_WHITE_Y)),
	(cc2bits10(CC_RED_X)>>2)&0xFF, (cc2bits10(CC_RED_Y)>>2)&0xFF,
	(cc2bits10(CC_GREEN_X)>>2)&0xFF, (cc2bits10(CC_GREEN_Y)>>2)&0xFF,
	(cc2bits10(CC_BLUE_X)>>2)&0xFF, (cc2bits10(CC_BLUE_Y)>>2)&0xFF,
	(cc2bits10(CC_WHITE_X)>>2)&0xFF, (cc2bits10(CC_WHITE_Y)>>2)&0xFF,

//(23H)      ESTABLISHED TIMING I:
	((SUPPORT_720x400P70)<<7) |
	((SUPPORT_720x400P88)<<6) |
	((SUPPORT_640x480P60)<<5) |
	((SUPPORT_640x480P67)<<4) |
	((SUPPORT_640x480P72)<<3) |
	((SUPPORT_640x480P75)<<2) |
	((SUPPORT_800x600P56)<<1) |
	((SUPPORT_800x600P60)<<0),
//(24H)      ESTABLISHED TIMING II:
	((SUPPORT_800x600P72)<<7) |
	((SUPPORT_800x600P75)<<6) |
	((SUPPORT_832x624P75)<<5) |
	((SUPPORT_1024x768I87)<<4) |
	((SUPPORT_1024x768P60)<<3) |
	((SUPPORT_1024x768P72)<<2) |
	((SUPPORT_1024x768P75)<<1) |
	((SUPPORT_1280x1024P75)<<0),
//(25H)      Manufacturer's Reserved Timing:
	((SUPPORT_1152x870P75)<<7),

//(26H-35H)  Standard Timing Identification:
//  1st byte = (Xresolution/8)-31
//  2nd byte = (aspect_ratio<<6) + refresh_rate-60
//  (aspect_ratio: 00=16:10; 01=4:3; 10=5:4; 11=16:9)
//  Not Used = 0x01, 0x01
	0x01, 0x01,  0x01, 0x01,  0x01, 0x01,  0x01, 0x01,
	0x01, 0x01,  0x01, 0x01,  0x01, 0x01,  0x01, 0x01,

//(36H-47H) Descriptor Block 1: Preferred Timing Mode
	EDID_PRERRED_TIMING,
//(48H-59H) Descriptor Block 2: Detailed Timing or Display Descriptor
#ifdef EDID_2ND_DETAILED_TIMING
	EDID_2ND_DETAILED_TIMING,
#else //Serial Number Descriptor
	/* (+00H) Header */
	0x00, 0x00,	0x00, 0xFF, 0x00,
	/* (+05H~0x11H) ASCII string + optional end marker(0x0A) & padded spaces(0x20) */
	SERIAL_NUMBER,
#endif
//(5AH-6BH) Descriptor Block 3: Detailed Timing or Display Descriptor
#ifdef EDID_3RD_DETAILED_TIMING
	EDID_3RD_DETAILED_TIMING,
#else //Display Range Limits Descriptor
	/* (+00H) Header */
	0x00, 0x00,	0x00, 0xFD, 0x00,
	/* (+05H) Vertical field rate (1-255 Hz) */
	VFREQ_MIN, VFREQ_MAX,
	/* (+07H) Horizontal line rate (1-255 kHz) */
	HFREQ_MIN, HFREQ_MAX,
	/* (+09H) Maximum pixel clock rate, rounded up to 10 MHz multiple (10-2550 MHz) */
	(CLOCK_MAX/10),
	/* (+0AH) No extended timing information type */
	0x00,
	/* (+0BH~11H) End marker + padded spaces */
	0x0A, 0x20,  0x20,  0x20,  0x20,  0x20,  0x20,
#endif

//(6CH-7DH) Descriptor Block 4: Detailed Timing or Display Descriptor
#ifdef EDID_4TH_DETAILED_TIMING
	EDID_4TH_DETAILED_TIMING,
#else //Display Product Name Descriptor
	/* (+00H) Header */
	0x00, 0x00,	0x00, 0xFC, 0x00,
	/* (+05H~0x11H) ASCII string + optional end marker(0x0A) & padded spaces(0x20) */
	PRODUCT_NAME,
#endif

//(7EH) Number Of Extension Blocks
	0x01,	//CEA861 EDID extension
//(7FH) Check Sum
	0x00,	/* 1-byte sum of (0x00-0x7F) must be 0 */


/* CEA861 EDID extension */
//(80H) Tag Code
	0x02,	//CEA 861 EXTENSION Block Tag Code
//(81H) Revision Number
	0x03,	//CEA 861 EXTENSION Block Version 3
//(82H) Byte offset of DTD
	CEA_DTD_OFFSET,
//(83H) 
	0<<7|	//[7] underscan supported if 1
	1<<6|	//[6] basic audio supported if 1
	SUPPORT_YCBCR444<<5|	//[5] YCbCr 4:4:4 supported if 1
	SUPPORT_YCBCR422<<4|	//[4] YCbCr 4:2:2 supported if 1
	CEA_NUM_DTDS,	//[3:0] total number of native DTDs
//(84H) CEA Data Block Collection
#if CEA_NUM_SVDS>0
 //Video Data Block
	CEA_TAG_VIDEO<<5|	//[7:5] Video Data Block Tag Code
	CEA_NUM_SVDS,	//[4:0] total number of video data bytes
	//CEA Short Video Descriptors
	CEA_SVD_LIST,
#endif
#if CEA_NUM_SADS>0
 //Audio Data Block
	CEA_TAG_AUDIO<<5|	//[7:5] Audio Data Block Tag Code
	(CEA_NUM_SADS*3),	//[4:0] total number of audio data bytes
	//CEA Short Audio Descriptors
	CEA_AUD_LPCM<<3 |	//[6:3] Audio format code
	(CEA_MAX_NUM_LPCM_AC-1),	//[2:0] (Max Number of channels - 1)
	CEA_LPCM_ASF_LIST,	//audio sampling frequency
	CEA_LPCM_BIT_LIST,	//LPCM bit precision
#endif
#if CEA_VSDB_LEN>0
 //Vendor-Specific Data Block
	/* (+00H) Header */
	CEA_TAG_VSDB<<5|	//[7:5] Vendor-Specific Data Block Tag Code
	CEA_VSDB_LEN,	//[4:0] total number of VSDB bytes
	//HDMI-LLC VSDB
	/* (+01H~03H) */
	0x03, 0x0C, 0x00,	//IEEE Registration Identifier
	/* (+04H~05H) */
	(SRC_PHY_ADDR>>16)&0xFF, SRC_PHY_ADDR&0xFF,	//Source Physical Address (refer to HDMI spec 8.7.2)
	/* (+06H) */
	0<<7 |	//[7]Supports_AI
	SUPPORT_DC_48B<<6|SUPPORT_DC_36B<<5|SUPPORT_DC_30B<<4 |
	SUPPORT_DC_Y444<<3 |
	0<<0,	//[0]DVI_Dual
	/* (+07H) */
	(MAX_TMDS_CLOCK/5),	//Max_TMDS_Clock (MHz)
#endif
//(nnH-nnH) Detailed Timing Descriptors
	CEA_DTD_LIST,
//zero paddings

//(FFH) Check Sum
	0x00	/* 1-byte sum of (0x00-0x7F) must be 0 */
};


#endif	// __USE_LT8619_RX__

