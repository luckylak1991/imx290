// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_IMX334__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
#if defined(__USE_IMX334_HCG_MODE__)
#define HCG 1  //FDG_SEL (0:LCG mode, 1:HCG mode)
#else
#define HCG 0  //FDG_SEL (0:LCG mode, 1:HCG mode)
#endif

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static IMX334_Fps_t imx334_fps=IMX334_FPS_30Hz;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
static void IMX334_ChangeFps(IMX334_Fps_t fps);
static void IMX334_SetAdBits(BYTE bit12);

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void IMX334_ChangeFps_Normal(BYTE nID) {
	switch(nID){
		case  0 : 	
		case  1 :	IMX334_ChangeFps(IMX334_FPS_30Hz);	break;
		case  2 :
		case  3 :	IMX334_ChangeFps(IMX334_FPS_60Hz);	break;
		case  4 : 	
		case  5 : 
		default : break;
	}
}


//-----------------------------------------------------------------------

void IMX334_Initialize(void)
{	

//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
#if defined(__USE_IMX334_2160P30_MODE__)	
	ROMDATA char RES[] = {"2160pFHD"} ;
	ROMDATA char FPS[] = {"@30"} ;
#endif	
	
#if defined(__USE_IMX334_2160P60_MODE__)	
	ROMDATA char RES[] = {"2160pFHD"} ;
	ROMDATA char FPS[] = {"@60"} ;
#endif		
	
//-----------------------------------------	
#if defined(__USE_CMOS_IMX334_CSI2_L4__)		
	ROMDATA char CH[]  = {"4ch mipi"} ;
#endif	

#if defined(__USE_CMOS_IMX334_CSI2_L8__)		
	ROMDATA char CH[]  = {"8ch mipi"} ;
#endif
//-----------------------------------------	
	UARTprintf("\n\rIMX334_Initialize(%s%s %s mode)\n\r",RES,FPS,CH);
//-----------------------------------------	
//-----------------------------------------	
//-----------------------------------------	

//////////////////		Chip ID = 02h			//////////////////////	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3000,0x01);		// Standby mode = 1
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3002,0x01);		// XMASTER = 1

#if defined(__USE_IMX334_SLAVE_MODE__)
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3010,0x07);		// EN2ND[0], ACKEN2ND[1]
//	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3010,0x05);		// EN2ND[0], ACKEN2ND[1]
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x31D9,0x01);		// EXTMODE[0] 
#endif		

#if 0	//74.25Mhz INCK
	//Data rate : 1782Mhz/lane(for 4lane 60hz)
	/*
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x300C,0xB6);		// BCWAIT_TIME
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x300D,0x7F);		// CPWAIT_TIME
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x314C,0xC0);		// INCKSEL1[7:0]	see page 75
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x314D,0x00);		// INCKSEL1[8]		see page 75
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x315A,0x03|0x00);	// INCKSEL2[1:0], 	PLL_IF_GC[3:2] ,1782&1188Mhz is 0, 891Mhz 1
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3168,0x68);		// INCKSEL3[7:0]
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x316A,0x03|0x7C);	// INCKSEL4[1:0], 	0x7C is fixed 7c + 2 = 7e
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x319E,0x00);		// SYS_MODE 0=1728Mhz, 1=1188Mhz(expect datasheet miss typing), 3:891Mhz	
	*/
	//Data rate : 1188Mhz/lane (for 50hz or 25hz or binning 8lane 120hz)
	/*
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x300C,0xB6);		// BCWAIT_TIME
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x300D,0x7F);		// CPWAIT_TIME
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x314C,0x80);		// INCKSEL1[7:0]	see page 75
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x314D,0x00);		// INCKSEL1[8]		see page 75
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x315A,0x03|0x00);	// INCKSEL2[1:0], 	PLL_IF_GC[3:2] ,1782&1188Mhz is 0, 891Mhz 1
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3168,0x68);		// INCKSEL3[7:0]
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x316A,0x03|0x7C);	// INCKSEL4[1:0], 	0x7C is fixed 7c + 2 = 7e
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x319E,0x01);		// SYS_MODE 0=1728Mhz, 1=1188Mhz(expect datasheet miss typing), 3:891Mhz	
	*/
	//Data rate : 891Mhz/lane	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x300C,0xB6);		// BCWAIT_TIME
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x300D,0x7F);		// CPWAIT_TIME
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x314C,0xC0);		// INCKSEL1[7:0]	see page 75
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x314D,0x00);		// INCKSEL1[8]		see page 75
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x315A,0x03|0x01);	// INCKSEL2[1:0], 	PLL_IF_GC[3:2] ,1782&1188Mhz is 0, 891Mhz 1
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3168,0x68);		// INCKSEL3[7:0]
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x316A,0x03|0x7C);	// INCKSEL4[1:0], 	0x7C is fixed 7c + 3 = 7f	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x319E,0x02);		// SYS_MODE 0=1728Mhz, 1=1188Mhz(expect datasheet miss typing), 3:891Mhz	
	
#else	//37.125Mhz INCK
	/*
	//Data rate : 1782Mhz/lane(for 4lane 60hz)
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x300C,0x5B);		// BCWAIT_TIME
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x300D,0x40);		// CPWAIT_TIME
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x314C,0xC0);		// INCKSEL1[7:0]	see page 75
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x314D,0x00);		// INCKSEL1[8]		see page 75
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x315A,0x02|0x00);	// INCKSEL2[1:0], 	PLL_IF_GC[3:2] ,1782&1188Mhz is 0, 891Mhz 1
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3168,0x68);		// INCKSEL3[7:0]
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x316A,0x02|0x7C);	// INCKSEL4[1:0], 	0x7C is fixed 7c + 2 = 7e
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x319E,0x00);		// SYS_MODE 0=1728Mhz, 1=1188Mhz(expect datasheet miss typing), 3:891Mhz	
	*/
	/*
	//Data rate : 1188Mhz/lane (for 50hz or 25hz or binning 8lane 120hz)	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x300C,0x5B);		// BCWAIT_TIME
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x300D,0x40);		// CPWAIT_TIME
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x314C,0x80);		// INCKSEL1[7:0]	see page 75
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x314D,0x00);		// INCKSEL1[8]		see page 75
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x315A,0x02|0x00);	// INCKSEL2[1:0], 	PLL_IF_GC[3:2] ,1782&1188Mhz is 0, 891Mhz 1
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3168,0x68);		// INCKSEL3[7:0]
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x316A,0x02|0x7C);	// INCKSEL4[1:0], 	0x7C is fixed 7c + 2 = 7e
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x319E,0x01);		// SYS_MODE 0=1728Mhz, 1=1188Mhz(expect datasheet miss typing), 3:891Mhz	
	*/
	//Data rate : 891Mhz/lane	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x300C,0x5B);		// BCWAIT_TIME
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x300D,0x40);		// CPWAIT_TIME
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x314C,0xC0);		// INCKSEL1[7:0]	see page 75
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x314D,0x00);		// INCKSEL1[8]		see page 75
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x315A,0x02|(0x01<<2));	// INCKSEL2[1:0], 	PLL_IF_GC[3:2] ,1782&1188Mhz is 0, 891Mhz 1
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3168,0x68);		// INCKSEL3[7:0]
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x316A,0x02|0x7C);	// INCKSEL4[1:0], 	0x7C is fixed 7c + 2 = 7e	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x319E,0x02);		// SYS_MODE 0=1728Mhz, 1=1188Mhz(expect datasheet miss typing), 3:891Mhz	

	/*
	//power on sensor initial default
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x300C,0xB6);		// BCWAIT_TIME
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x300D,0x7F);		// CPWAIT_TIME	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x314C,0x80);		// INCKSEL1[7:0]	see page 75
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x314D,0x00);		// INCKSEL1[8]		see page 75
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x315A,0x03|0x00);	// INCKSEL2[1:0], 	PLL_IF_GC[3:2] ,1782&1188Mhz is 0, 891Mhz 1
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3168,0x68);			// INCKSEL3[7:0]
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x316A,0x03|0x7C);	// INCKSEL4[1:0], 	0x7C is fixed	
	*/
#endif
	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3199,0x00);		// HADD[4],VADD[5] [5]&[4]='1' then 2x2 binning, 0 : all pixel scan

	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3018,0x00);		// WINMODE[3:0] 0: all pixel scan, 1:2x2 binning, 4:windows crop mode
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x302C,0x30);		// HTRIMMING START[7:0], window crop mode only
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x302D,0x00);		// HTRIMMING START[11:8], window crop mode only	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x302E,0x18);		// HNUM[7:0], window crop mode only
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x302F,0x0F);		// HNUM[11:8],	 window crop mode only	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3074,0xB0);		// AREA3_ST_ADR_1[7:0]  window crop mode only	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3075,0x00);		// AREA3_ST_ADR_1[12:8]  window crop mode only		
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3076,0x84);		// AREA3_WIDTH_1[7:0]  window crop mode only	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3077,0x08);		// AREA3_WIDTH_1[12:8]  window crop mode only	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x308E,0xB0+1);		// AREA3_ST_ADR_2[7:0]  window crop mode only	set : AREA3_ST_ADR_1 + 1
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x308F,0x00);		// AREA3_ST_ADR_2[12:8]  window crop mode only	set : AREA3_ST_ADR_1 + 1	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3090,0x84);		// AREA3_WIDTH_2[7:0]  window crop mode only	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3091,0x08);		// AREA3_WIDTH_2[12:8]  window crop mode only		
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x30C6,0x00);		// BLACK OFSET ADR[7:0]  window crop mode only	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x30C7,0x00);		// BLACK OFSET ADR[12:8]  window crop mode only		
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x30CE,0x00);		// UNARD_LINE_MAX[7:0]  window crop mode only	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x30CF,0x00);		// UNARD_LINE_MAX[12:8]  window crop mode only		
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x30D8,0x8F);		// UNARD_ED_ADR[7:0]  window crop mode only	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x30D9,0x11);		// UNARD_ED_ADR[12:8]  window crop mode only		
		
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x304C,0x14);		// OPBSIZE[5:0],	Vertical direction OB setting

	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x304E,0x00);		// H-Reverse
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x304F,0x00);		// V-Reverse : see 51page..
	
	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3058,0x05);		// SHR-0[7:0]
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3059,0x00);		// SHR-0[15:8]
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x305A,0x00);		// SHR-0[15:8]

	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x30E8,0x00);		// GAIN[7:0]  0.3db / step
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x30E9,0x00);		// GAIN[10:8]  0.3db / step

	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x31A0,0x0A|0x20);	// OOO_OUTSEL, [5] FIXED=1
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x31D4,0x00);		// XVSLNG
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x31D5,0x00);		// XHSLNG
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x31DD,0x03);		// VALID EXPAND	3: all pixel scan mode, 4 : binning mode

	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3300,0x00);		// TCYCLE[1:0]		0: all pixel scan mode, 1 : binning mode
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3302,0x32);		// BLKLEVEL[7:0]	10bit mode : 1digit/1h, 12bit mode : 4digit/1h
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3303,0x00);		// BLKLEVEL[9:8]	

	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3308,0x84);		// YOUTSIZE[7:0]	all pixel scan mode 884h / binning : 442h
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3309,0x08);		// YOUTSIZE[12:8]		

#if defined(__USE_CMOS_IMX334_CSI2_L8__)
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A01,0x07);		// LANEMODE[2:0]		7: 8lane

#else
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A01,0x03);		// LANEMODE[2:0]		3: 4lane

#endif

	//global timing setting 12bit 4lane 30hz or 12bit 8ane 60hz (all pixel scan mode)
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A18,0x7F);		// TCLKPOST[7:0]		
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A19,0x00);		// TCLKPOST[15:0]		
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A1A,0x37);		// TCLKPREPARE[7:0]		
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A1B,0x00);		// TCLKPREPARE[15:0]	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A1C,0x37);		// TCLKTRAIL[7:0]		
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A1D,0x00);		// TCLKTRAIL[15:0]		
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A1E,0xF7);		// TCLKTZERO[7:0]		
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A1F,0x00);		// TCLKTZERO[15:0]		

	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A20,0x3F);		// THSPREPARE[7:0]		
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A21,0x00);		// THSPREPARE[15:0]	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A22,0x6F);		// THSTZERO[7:0]		
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A23,0x00);		// THSTZERO[15:0]		
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A24,0x3F);		// THSTRAIL[7:0]		
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A25,0x00);		// THSTRAIL[15:0]	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A26,0x5F);		// THSEXIT[7:0]		
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A27,0x00);		// THSEXIT[15:0]	
	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A28,0x2F);		// TLPX[7:0]	
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3A29,0x00);		// TLPX[15:0]	

#if defined(__USE_IMX334_2160P60_MODE__)	
	IMX334_ChangeFps_Normal(IMX334_FPS_60Hz);
#else
	IMX334_ChangeFps_Normal(IMX334_FPS_30Hz);
#endif


#if !defined(__USE_IMX334_SLAVE_MODE__)
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3002,0x00);		// XMASTER = 0, if master mode.
#endif

	Delay_mSec(50);										// Wait for internal regulator stabilization

	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3000,0x00);		// Standby mode = 0. running

}


static void IMX334_SetAdBits(BYTE bit12) {
	if(bit12) {  //ADBIT : 12bit
		CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3050,0x01);		// ADBIT[0] 1:12bit 0:10bit
		CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x319D,0x01);		// MDBIT[0] 1:12bit 0:10bit	
		CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x341C,0x47);		// ADBIT1[7:0] 47h:12bit 1FFh:10bit		
		CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x341D,0x00);		// ADBIT1[8]	
	}
	else {  //ADBIT : 10bit
		CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3050,0x00);		// ADBIT[0] 1:12bit 0:10bit
		CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x319D,0x00);		// MDBIT[0] 1:12bit 0:10bit	
		CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x341C,0xFF);		// ADBIT1[7:0] 47h:12bit 1FFh:10bit		
		CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x341D,0x01);		// ADBIT1[8]	
	}
}

static WORD vmax;

static void IMX334_ChangeFps(IMX334_Fps_t fps) 
{
	WORD hmax;

	const WORD hbase=550;
	const WORD vbase=2250;

	imx334_fps=fps;
	switch(fps) {
		case IMX334_FPS_30Hz:
			hmax=hbase*2;		//44ch=1100d=550x2
			vmax=vbase*1;

			IMX334_SetAdBits(1);  //ADBIT:12bit

			break;
		case IMX334_FPS_60Hz:
			hmax=hbase*1;
			vmax=vbase*1;

			IMX334_SetAdBits(1);  //ADBIT:12bit
	
			break;

		default:
			hmax=hbase*2;
			vmax=vbase*1;

			IMX334_SetAdBits(1);  //ADBIT:12bit
			
			break;
	}

	//pal system
#if defined(__VTOTAL_CTRL_PAL__)
	if(GetSS_SensorVoutPAL())  vmax*=1.2f;
#else
	if(GetSS_SensorVoutPAL())  hmax*=1.2f;
#endif

	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3034,LOBYTE(hmax));		// HMAX[7:0]
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3035,HIBYTE(hmax));		// HMAX[15:8]
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3030,LOBYTE(vmax));		// VMAX[7:0]
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3031,HIBYTE(vmax));		// VMAX[15:8]
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3032,0x00);				// VMAX[19:16]

	
	
}

DWORD CMOS_GetVmax(void)
{
	return vmax;
}

// temp code to test fps
IMX334_Fps_t IMX334_GetFrameRate(void) {
	return imx334_fps;
}

void IMX334_SetFrameRate(IMX334_Fps_t fps) {
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3000,0x01);		// Standby mode = 1
	IMX334_ChangeFps(fps);
	CMOS_ByteWrite(I2C_SONY_CMOS_ADDR,0x3000,0x00);		// Standby mode = 0
	return;
}


#endif	/* __USE_CMOS_IMX334__ */

/*  FILE_END_HERE */

