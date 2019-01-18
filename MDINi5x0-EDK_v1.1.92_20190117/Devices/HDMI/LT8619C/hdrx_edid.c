
#include "common.h"


#if defined(__USE_LT8619_RX__)


#include "hdrx_edid_table.h"

/*
 * User parameters to define EDID table
 */
#define MFG_CODE1	'M'	//manufacturer's 3-character code
#define MFG_CODE2	'I'
#define MFG_CODE3	'T'
#define PROD_CODE	0x0001	//2-byte product code
#define SERIAL	0x00000000	//32-bit serial number
#define YEAR	2016	//year 1990~2245
#define WEEK	12	//week 1~54

#define GAMMA	2.2	//1.00~3.54

//DPMS and supported features
#define SUPPORT_STANDBY	0
#define SUPPORT_SUSPEND	0
#define SUPPORT_ACTIVEOFF	0
#define SUPPORT_YCBCR444	1
#define SUPPORT_YCBCR422	1

//Chromaticity coordinates
#define CC_RED_X	0.640
#define CC_RED_Y	0.330
#define CC_GREEN_X	0.300
#define CC_GREEN_Y	0.600
#define CC_BLUE_X	0.150
#define CC_BLUE_Y	0.060
#define CC_WHITE_X	0.313
#define CC_WHITE_Y	0.329

//Supported timings
#define SUPPORT_720x400P70	0
#define SUPPORT_720x400P88	0
#define SUPPORT_640x480P60	0
#define SUPPORT_640x480P67	0
#define SUPPORT_640x480P72	0
#define SUPPORT_640x480P75	0
#define SUPPORT_800x600P56	0
#define SUPPORT_800x600P60	0
#define SUPPORT_800x600P72	0
#define SUPPORT_800x600P75	0
#define SUPPORT_832x624P75	0
#define SUPPORT_1024x768I87	0
#define SUPPORT_1024x768P60	0
#define SUPPORT_1024x768P72	0
#define SUPPORT_1024x768P75	0
#define SUPPORT_1280x1024P75	0
#define SUPPORT_1152x870P75	0

//Preferred Timing Descriptor (required)
#define EDID_PRERRED_TIMING	DTD_1920X1080P60
//Detailed Timing Descriptors (optional)
#define EDID_2ND_DETAILED_TIMING	DTD_1920X1080P50
//#define EDID_3RD_DETAILED_TIMING	DTD_1280X720P60
//#define EDID_4TH_DETAILED_TIMING	DTD_1280X720P50

//Serial Number Descriptor
// will be included when EDID_2ND_DETAILED_TIMING is not defined.
// ASCII string + optional end marker(0x0A)&padded spaces(0x20) = 13 characters
#define SERIAL_NUMBER	'A','0','1','2','3','4','5','6',0x0A,0x20,0x20,0x20,0x20

//Display Range Limits Descriptor
// will be included when EDID_3RD_DETAILED_TIMING is not defined.
#define VFREQ_MIN	48	//(1-255 Hz)
#define VFREQ_MAX	62
#define HFREQ_MIN	14	//(1-255 kHz)
#define HFREQ_MAX	70
#define CLOCK_MAX	300	//(10-2550 MHz)

//Display Product Name Descriptor
// will be included when EDID_4TH_DETAILED_TIMING is not defined.
// ASCII string + optional end marker(0x0A)&padded spaces(0x20) = 13 characters
#define PRODUCT_NAME	'M','D','I','N','-','4','0','0',0x0A,0x20,0x20,0x20,0x20


/*
 * User parameters to define CEA861 EDID extension
 */
//Supported CEA-861 SVD(Short Video Descriptor) list
// NVF(Native Video Format) flag can be used only for VIC code 1~64
#define CEA_SVD_LIST \
	NVF | CEA_VID_1920X1080P60,	\
	CEA_VID_1920X1080P50,	\
	CEA_VID_1280X720P60,	\
	CEA_VID_1280X720P50,	\
	CEA_VID_3840X2160P25,	\
	CEA_VID_3840X2160P30
#define CEA_NUM_SVDS	6	//number of SVDs in CEA_SVD_LIST

//Parameters for CEA-861 LPCM SAD(Short Audio Descriptor)
//Maximum number of LPCM audio channels
#define CEA_MAX_NUM_LPCM_AC	2	//2~8
//Supported CEA-861 LPCM Audio Sampling Frequency list
#define CEA_LPCM_ASF_LIST \
	CEA_ASF_192KHZ | \
	CEA_ASF_176KHZ | \
	CEA_ASF_96KHZ | \
	CEA_ASF_88KHZ | \
	CEA_ASF_48KHZ | \
	CEA_ASF_44KHZ | \
	CEA_ASF_32KHZ
//Supported CEA-861 LPCM Audio Bit Precision list
#define CEA_LPCM_BIT_LIST \
	CEA_LPCM_24B | \
	CEA_LPCM_20B | \
	CEA_LPCM_16B	//LPCM bit precision
#define CEA_NUM_SADS	1	//number of SVDs (LPCM only)

//Parameters for CEA-861 VSDB(Vendor-Specific Data Block)
#define SRC_PHY_ADDR	0x1000	//Source Physical Address (0xABCD=A.B.C.D)
#define SUPPORT_DC_30B	1	//Set if Sink supports 30 bits/pixel (10 bits/color)
#define SUPPORT_DC_36B	0	//Set if Sink supports 36 bits/pixel (12 bits/color)
#define SUPPORT_DC_48B	0	//Set if Sink supports 48 bits/pixel (16 bits/color)
#define SUPPORT_DC_Y444	1	//Set if Sink supports YCBCR 4:4:4 in Deep Color modes
#define MAX_TMDS_CLOCK	300	//in 5MHz step
#define CEA_VSDB_LEN	7	//VSDB length (fixed to 7)

//Detailed Timing Descriptors in CEA861 extension
#define CEA_DTD_LIST \
	DTD_3840X2160P30, \
	DTD_3840X2160P25, \
	DTD_3840X2160P24
#define CEA_NUM_DTDS	3	//number of DTDs in CEA_DTD_LIST

#define CEA_DTD_OFFSET	(4 + 1+CEA_NUM_SVDS + 1+CEA_NUM_SADS*3 + 1+CEA_VSDB_LEN)
#define CEA_EXT_LEN	(CEA_DTD_OFFSET + CEA_NUM_DTDS*3)


//include table constant definition
#define EDID_TABLE_NAME	RX1_EDID_Table
#include "hdrx_edid_table.c"


#endif	// __USE_LT8619_RX__

