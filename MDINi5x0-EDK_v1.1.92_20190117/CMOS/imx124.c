// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

#if	defined(__USE_CMOS_IMX124__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
typedef	struct {
	WORD vmax;
	WORD hmax;
	BYTE drsel;  // 0:120hz, 1:60hz, 2:30hz
	BYTE adbit;  // fixed to 2 : 12bit (AD conversion bits)
	BYTE odbit;  // fixed to 1 : 12bit (Number of output bit)
	BYTE oport;  // 2:4ch, 3:8ch
	WORD hs_1;   // be aware!! imx123 only (imx124 fixed to 42h)
	WORD hs_2;   // be aware!! imx123 only (imx124 fixed to 3Eh)
}	FPS_REG, *PFPS_REG;

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
#if defined (__USE_IMX124_OSC27Mhz__)
	static FPS_REG fps_do2={1600*1,  750/2,  0, 2, 1, 3, 0x89, 0x80};  //QXGA, 8ch, 120fps
	static FPS_REG fps_do3={1600*1,  750/2,  0, 2, 1, 3, 0x89, 0x80};  //QXGA, 8ch,  60fps
	#if 0  //test 30->15Hz
	static FPS_REG fps_htm={1600*2,  750*2,  2, 2, 1, 2, 0x4D, 0x44};  //QXGA, 4ch,  15fps
	#else
	static FPS_REG fps_htm={1600*2,  750*1,  1, 2, 1, 2, 0x4D, 0x44};  //QXGA, 4ch,  30fps
	#endif

 #if defined(__USE_CMOS_IMX123_LVDS_S8__)
  #if   defined(__USE_IMX124_1536P60_MODE__)
	static FPS_REG fps_nor={1600*1,  750*1,  1, 2, 1, 3, 0x4D, 0x44};
  #elif defined(__USE_IMX124_1536P30_MODE__)
	static FPS_REG fps_nor={1600*1,  750*2,  1, 2, 1, 3, 0x89, 0x80};
  #elif defined(__USE_IMX124_1080P60_MODE__)
	static FPS_REG fps_nor={1125*1, 1100*1,  1, 2, 1, 3, 0x4D, 0x44};
  #elif defined(__USE_IMX124_1080P30_MODE__)
	static FPS_REG fps_nor={1125*1, 1100*2,  2, 2, 1, 3, 0x89, 0x80};
  #endif
 #else
  #if   defined(__USE_IMX124_1536P60_MODE__)
	static FPS_REG fps_nor={1600*1,  750*1,  1, 2, 1, 2, 0x4D, 0x44};
  #elif defined(__USE_IMX124_1536P30_MODE__)
	static FPS_REG fps_nor={1600*1,  750*2,  2, 2, 1, 2, 0x4D, 0x44};
  #elif defined(__USE_IMX124_1080P60_MODE__)
	static FPS_REG fps_nor={1125*1, 1100*1,  1, 2, 1, 2, 0x4D, 0x44};
  #elif defined(__USE_IMX124_1080P30_MODE__)
	static FPS_REG fps_nor={1125*1, 1100*2,  2, 2, 1, 2, 0x4D, 0x44};
  #endif
 #endif
  
  #if   defined(__USE_IMX124_1536P60_MODE__)||\
        defined(__USE_IMX124_1536P30_MODE__)
	static FPS_REG fps_wdr={1600*1,  750*1,  1, 2, 1, 2, 0x4D, 0x44};
  #elif defined(__USE_IMX124_1080P60_MODE__)||\
        defined(__USE_IMX124_1080P30_MODE__)
	static FPS_REG fps_wdr={1125*1, 1100*1,  1, 2, 1, 2, 0x4D, 0x44};
  #endif

#else

	static FPS_REG fps_do2={1650*1,  750/2,  0, 2, 1, 3, 0x89, 0x80};  //QXGA, 8ch, 120fps
	static FPS_REG fps_do3={1650*1,  750/2,  0, 2, 1, 3, 0x89, 0x80};  //QXGA, 8ch,  60fps
	static FPS_REG fps_htm={1650*2,  750*1,  0, 2, 1, 3, 0x89, 0x80};  //QXGA, 8ch,  30fps
	
 #if defined(__USE_CMOS_IMX123_LVDS_S8__)
  #if   defined(__USE_IMX124_1536P60_MODE__)
	static FPS_REG fps_nor={1650*1,   750*1,  0, 2, 1, 3, 0x89, 0x80};
  #elif defined(__USE_IMX124_1536P30_MODE__)
	static FPS_REG fps_nor={1650*1,   750*2,  1, 2, 1, 3, 0x89, 0x80};
  #elif defined(__USE_IMX124_1080P60_MODE__)
	static FPS_REG fps_nor={1125*1,   550*4,  1, 2, 1, 3, 0x4D, 0x44};
  #elif defined(__USE_IMX124_1080P30_MODE__)
	static FPS_REG fps_nor={1125*1, 1100*4,  2, 2, 1, 3, 0x89, 0x80};
  #endif
 #else
  #if   defined(__USE_IMX124_1536P60_MODE__)
	static FPS_REG fps_nor={1650*1,   750*1,  0, 2, 1, 3, 0x89, 0x80};	//not support
  #elif defined(__USE_IMX124_1536P30_MODE__)
	static FPS_REG fps_nor={1650*1,   750*2,  1, 2, 1, 2, 0x89, 0x80};
  #elif defined(__USE_IMX124_1080P60_MODE__)
	static FPS_REG fps_nor={1125*1,   550*4,  1, 2, 1, 2, 0x4D, 0x44};
  #elif defined(__USE_IMX124_1080P30_MODE__)
	static FPS_REG fps_nor={1125*1, 1100*2,  2, 2, 1, 2, 0x4D, 0x44};
  #endif
#endif

  //copy upper(osc27Mhz) case then recheck it
  #if   defined(__USE_IMX124_1536P60_MODE__)||\
        defined(__USE_IMX124_1536P30_MODE__)
	static FPS_REG fps_wdr={1600*1,  750*1,  1, 2, 1, 2, 0x4D, 0x44};
  #elif defined(__USE_IMX124_1080P60_MODE__)||\
        defined(__USE_IMX124_1080P30_MODE__)
	static FPS_REG fps_wdr={1125*1, 1100*1,  1, 2, 1, 2, 0x4D, 0x44};
  #endif

#endif
static PFPS_REG pfps_set;

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
static void IMX124_SetMasterSpan(IMX124_Mode_t Mode) {  //set HMAX, VMAX
	//get configure
	switch(Mode) {
		case IMX124_WDR:   pfps_set=&fps_wdr;  break;
		case IMX124_HTM:   pfps_set=&fps_htm;  break;
		case IMX124_DOL2:  pfps_set=&fps_do2;  break;
		case IMX124_DOL3:  pfps_set=&fps_do3;  break;
		default:           pfps_set=&fps_nor;  break;
	}

	/////////////////////
	// set HMAX, VMAX
	/////////////////////
	WORD hmax=pfps_set->hmax;
	WORD vmax=pfps_set->vmax;

	//pal system
#if defined(__VTOTAL_CTRL_PAL__)
	if(GetSS_SensorVoutPAL())  vmax*=1.2f;
#else
	if(GetSS_SensorVoutPAL())  hmax*=1.2f;
#endif

#if defined(__USE_MDIN_i500__)  //[MDIN-i500] htotal has to be divided into four
  #if defined(__USE_IMX124_1536P60_MODE__)
	hmax=(hmax== 750)?( 750-2):hmax;  //htotal=hmax*2*1.5 (2250->2244)
  #elif defined(__USE_IMX124_1536P30_MODE__)
	hmax=(hmax==1500)?(1500-4):hmax;  //htotal=hmax*1*1.5 (2250->2244)
  #endif
#endif

	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x021B, LOBYTE(hmax));	// HMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x021C, HIBYTE(hmax));	// HMAX [7:0] (MSB)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0218, LOBYTE(vmax));	// VMAX [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0219, HIBYTE(vmax));	// VMAX [7:0] (MSB)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x021A, 0x00);			// VMAX [0]   (MSB)

	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0303, LOBYTE(vmax-1)); // Set to VMAX(640h) -1
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0304, HIBYTE(vmax-1)); // Set to VMAX -1
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0305, 0x00);           // Set to VMAX -1

	/////////////////////
	// set DRSEL, ADBIT, ODBIT, OPORT, HS_1, HS_2
	/////////////////////
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0209, (pfps_set->drsel));		// DRSEL [1:0] - 1: 60fps,  2: 30 fps INCLK=27MHz(OSCCLK)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0205, (pfps_set->adbit));		// Fixed	, imx123 ADBIT
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0244, (pfps_set->oport<<4)|(1<<2)|(pfps_set->odbit));
#if defined(__USE_CMOS_IMX123__)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0330, (pfps_set->hs_1));		// Set (Default Set:0x89)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x03EB, (pfps_set->hs_2));		// Set (Default Set:0x80)
#else
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0330, 0x42);		// Set to 42h
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x03EB, 0x3E);		// Set to 3Eh
#endif

	return;
}


//--------------------------------------------------------------------------------------------------------------------------
static void IMX124_SetInckConfig(IMX124_Mode_t Mode) {  //set INCK
#if defined(__USE_CMOS_IMX123__)
  #if defined (__USE_IMX124_OSC27Mhz__)
    #if defined(__USE_IMX124_RES_QXGA__)
	BYTE inck_123[6]={0xB1, 0x00, 0x2C, 0x09, 0x60, 0x18};  //QXGA (123 Normal)
	BYTE inck_dol[6]={0xB1, 0x00, 0x2C, 0x09, 0x60, 0x18};  //QXGA (123 DOL)
    #else  //FHD
	BYTE inck_123[6]={0xA1, 0x00, 0x2C, 0x09, 0x60, 0x16};  //FHD (123 Normal)
	BYTE inck_dol[6]={0xB1, 0x00, 0x2C, 0x09, 0x60, 0x18};  //FHD (123 DOL)
    #endif

  #else  //OSC 37.125Mhz
    #if defined(__USE_IMX124_RES_QXGA__)
	BYTE inck_123[6]={0xA1, 0x00, 0x20, 0x09, 0x61, 0x20};  //QXGA (123 Normal)
	BYTE inck_dol[6]={0xB1, 0x00, 0x20, 0x09, 0x61, 0x18};  //QXGA (123 DOL)
    #else  //FHD
	BYTE inck_123[6]={0x81, 0x00, 0x20, 0x09, 0x61, 0x18};  //FHD (123 Normal)
	BYTE inck_dol[6]={0x81, 0x00, 0x20, 0x09, 0x61, 0x18};  //FHD (123 DOL)
    #endif
  #endif

	//assign register set
	PBYTE inck_reg=((Mode==IMX124_DOL2)||(Mode==IMX124_DOL3))?inck_dol:inck_123;

#else  //imx124
  #if defined (__USE_IMX124_OSC27Mhz__)
    #if defined(__USE_IMX124_RES_QXGA__)
	BYTE inck_124[6]={0xB1, 0x00, 0x2C, 0x09, 0x60, 0x18};  //QXGA
    #else  //FHD
	BYTE inck_124[6]={0xA1, 0x00, 0x2C, 0x09, 0x60, 0x16};  //FHD
    #endif

  #else  //OSC 37.125Mhz
    #if defined(__USE_IMX124_RES_QXGA__)
	BYTE inck_124[6]={0xA1, 0x00, 0x20, 0x09, 0x61, 0x20};  //QXGA
    #else  //FHD
	BYTE inck_124[6]={0x81, 0x00, 0x20, 0x09, 0x61, 0x18};  //FHD
    #endif
  #endif

	//assign register set
	PBYTE inck_reg=inck_124;
#endif

	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0261, inck_reg[0]);	// INCKSEL1 
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0262, inck_reg[1]);	// INCKSEL2
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x036C, inck_reg[2]);	// INCKSEL3
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x036D, inck_reg[3]);	// INCKSEL4
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0370, inck_reg[4]);	// INCKSEL5
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0371, inck_reg[5]);	// INCKSEL6
}


//--------------------------------------------------------------------------------------------------------------------------
static void IMX124_SetWdrConfig(IMX124_Mode_t Mode) {  //set WDR
	//about DOL
	switch(Mode) {
		case IMX124_DOL2:
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02CB, 0x03|0x20);	// XVSMSKCNT_INT[6:5]  0: Normal, 1: DOL 2 frame, 3: DOL 3 frame
			break;
		case IMX124_DOL3:
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02CB, 0x03|0x60);	// XVSMSKCNT_INT[6:5]  0: Normal, 1: DOL 2 frame, 3: DOL 3 frame
			break;
		default:  //Normal
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02CB, 0x03|0x00);	// XVSMSKCNT_INT[6:5]  0: Normal, 1: DOL 2 frame, 2: DOL 3 frame
			break;
	}

	//about HTM
	switch(Mode) {
		case IMX124_HTM:
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x020F, 0x05);  // [  2] WDC_CMPEN ... 1 : Compressed enable 
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0210, 0x00);  // [7:0]           ... Set to "00h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0212, 0x2D);  // [7:0]           ... Set to "2Dh" (HTM)
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0213, 0x00);  // [7:0]           ... Set to "00h" (HTM)

#if defined(__USE_IMX124_HTM_OLD_COMP__)
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0265, 0x80);  // [7:0]           ... Set to "00h"
#else
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0265, 0x00);  // [7:0]           ... Set to "00h"
#endif
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0284, 0x1A);  // [6:0] VRSET     ... Midpoint potential setting, Set to "1Ah"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0286, 0x10);  // [7:0]           ... Set to "10h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02CF, 0xF1);  // [7:0]           ... Set to "F1h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02D0, 0x28);  // [7:0]           ... Set to "28h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02D2, 0x93);  // [7:0]           ... Set to "93h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02D3, 0x01);  // [7:0]           ... Set to "01h"

			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0355, 0x01);  // [7:0]           ... Set to "01h"

			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0491, 0x03);  // [7:0]           ... Set to "03h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0492, 0xED);  // [7:0]           ... Set to "EDh"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0493, 0x30);  // [7:0]           ... Set to "30h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0494, 0x10);  // [7:0]           ... Set to "10h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x04A0, 0xED);  // [7:0]           ... Set to "EDh"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x04A1, 0x30);  // [7:0]           ... Set to "30h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x04A2, 0x10);  // [7:0]           ... Set to "10h"
			break;
		default:
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x020F, 0x01);  // [  2] WDC_CMPEN ... 1 : Compressed enable 
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0210, 0x01);  // [7:0]           ... Set to "00h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0212, 0x0E);  // [7:0]           ... Set to "0Eh" (Normal)
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0213, 0x01);  // [7:0]           ... Set to "01h" (Normal)

			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0265, 0x80);  // [7:0]           ... Set to "00h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0284, 0x00);  // [6:0] VRSET     ... Midpoint potential setting, Set to "1Ah"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0286, 0x01);  // [7:0]           ... Set to "10h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02CF, 0xD1);  // [7:0]           ... Set to "F1h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02D0, 0x1B);  // [7:0]           ... Set to "28h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02D2, 0x5F);  // [7:0]           ... Set to "93h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02D3, 0x00);  // [7:0]           ... Set to "01h"

			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0355, 0x00);  // [7:0]           ... Set to "01h"

			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0491, 0x00);  // [7:0]           ... Set to "03h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0492, 0x15);  // [7:0]           ... Set to "EDh"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0493, 0xB1);  // [7:0]           ... Set to "30h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0494, 0x12);  // [7:0]           ... Set to "10h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x04A0, 0x15);  // [7:0]           ... Set to "EDh"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x04A1, 0xB1);  // [7:0]           ... Set to "30h"
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x04A2, 0x12);  // [7:0]           ... Set to "10h"
			break;
	}

	return;
}

//--------------------------------------------------------------------------------------------------------------------------
static void IMX124_SetProcMode(IMX124_Mode_t Mode) {
	IMX124_SetMasterSpan(Mode);  //set about HMAX, VMAX
	IMX124_SetInckConfig(Mode);  //set about INCK
	IMX124_SetWdrConfig(Mode);   //set about WDR (mode, compress)
}


// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void IMX124_ChangeFps_Normal(void) {
	IMX124_SetProcMode(IMX124_NORMAL);
}

//-----------------------------------------------------------------------
void IMX124_DCKRST_Sequence(void)
{
//DCKRST reset sequence
	const BYTE omode=1;  //fixed to 1h:LVDS
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0244, (1<<6)|(pfps_set->oport<<4)|(omode<<2)|(pfps_set->odbit));  // DCKRST = 1
	Delay_mSec(20);
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0244, (0<<6)|(pfps_set->oport<<4)|(omode<<2)|(pfps_set->odbit));  // DCKRST = 0
}

//-----------------------------------------------------------------------
void IMX124_Initialize(void)
{	

	#if defined(__USE_CMOS_IMX123__)
	UARTprintf("\r\nIMX123_Initialize\r\n");
	#else
	UARTprintf("\r\nIMX124_Initialize\r\n");
	#endif

//////////////////		Chip ID = 02h			//////////////////////	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0201, 0x00);		// RegHold
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0202, 0x01);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop

#if defined(__USE_CMOS_IMX123__)
	#if defined(__USE_IMX124_FLIP__)
		#if defined(__USE_IMX124_1080P30_MODE__) || defined(__USE_IMX124_1080P60_MODE__)
		CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0207, 0x00|0x13);	//WINMODE [7:4] - 0: All pixel(WUXGA),  1: 1080p,  2: 720p,  4: window crop    ||  VREVERSE [0], HREVERSE [1] - 0: normal, 1: inverted, 
		#else
		CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0207, 0x00|0x03);	//WINMODE [7:4]  use 0(QUXGA)
		#endif
	#else
		#if defined(__USE_IMX124_1080P30_MODE__) || defined(__USE_IMX124_1080P60_MODE__)
		CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0207, 0x00|0x10);	//WINMODE [7:4] - 0: All pixel(WUXGA),  1: 1080p,  2: 720p,  4: window crop    ||  VREVERSE [0], HREVERSE [1] - 0: normal, 1: inverted, 
		#else
		CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0207, 0x00|0x00);	//WINMODE [7:4]  use 0(QUXGA)
		#endif
	#endif
#else
	#if defined(__USE_IMX124_FLIP__)
		#if defined(__USE_IMX124_1080P30_MODE__) || defined(__USE_IMX124_1080P60_MODE__)
		CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0207, 0x08|0x13);	//WINMODE [7:4] - 0: All pixel(WUXGA),  1: 1080p,  2: 720p,  4: window crop    ||  VREVERSE [0], HREVERSE [1] - 0: normal, 1: inverted, 
		#else
		CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0207, 0x08|0x03);	//WINMODE [7:4]  use 0(QUXGA)
		#endif
	#else
		#if defined(__USE_IMX124_1080P30_MODE__) || defined(__USE_IMX124_1080P60_MODE__)
		CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0207, 0x08|0x10);	//WINMODE [7:4] - 0: All pixel(WUXGA),  1: 1080p,  2: 720p,  4: window crop    ||  VREVERSE [0], HREVERSE [1] - 0: normal, 1: inverted, 
		#else
		CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0207, 0x08|0x00);	//WINMODE [7:4]  use 0(QUXGA)
		#endif
	#endif
#endif

	//set frame rate, inck, wdr configure
	IMX124_SetProcMode(IMX124_NORMAL);

	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x020A, 0xF0);	// BLKLEVEL[7:0] - Black level (LSB)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x020B, 0x00);	// BLKLEVEL[0] - Black level (MSB)

	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0214, 0x00);	// GAIN [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0215, 0x00);	// GAIN [0] (MSB)

	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x021E, 0x99);  // SHS1
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x021F, 0x01);
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0220, 0x00);


	// Crop mode register
//	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0238, 0x00);		// WINPV [7:0] (LSB)
//	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0239, 0x00);		// WINPV [3:0] (MSB)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x023A, 0x10);	// WINWV [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x023B, 0x06);		// WINWV [3:0] (MSB)
//	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x023C, 0x00);		// WINPH [7:0] (LSB)
//	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x023D, 0x00);	// WINPH [2:0] (MSB)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x023E, 0x9C);	// WINWH [7:0] (LSB)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x023F, 0x00);	// WINWH [2:0] (MSB)


//-------------------------------------------------------------------------------------
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0246, 0x00);		// XVSLNG [5:4] XVS pulse width setting 0: 1H, 1: 2H, 2: 4H, 3: 8H
//	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0246, 0x30);		// XVSLNG [5:4] XVS pulse width setting 0: 1H, 1: 2H, 2: 4H, 3: 8H
	
//	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0247, 0x08);		// XHSLNG [5:4] 0: Min. to 3: Max, [3] = fix 1
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0247, 0x00);		// XHSLNG [5:4] 0: Min. to 3: Max, [3] = fix 1

	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0249, 0x00);		// XVSOUTSEL[1:0] - 0: Set XVS High,  2:  VSYNC out,    XHSOUTSEL[3:2] - 0: Set XHS High,  2:  HSYNC out


//-------------------------------------------------------------------------------------
	//set registers about DOL WDR
#if defined(__USE_CMOS_IMX123__)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02C9, 0x03);		// [6]DOLHBFIXEN=0, [3:0]HBLK1[11:8]	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0254, 0x61|0x0C);	// DOLSCDEN[2]=1, DOLSYDINFOEN[3]=1
#endif

//-------------------------------------------------------------------------------------
	//set registers about Built-in WDR
#if defined(__USE_CMOS_IMX123__)
  #if defined(__USE_IMX123_HTM_X32__)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0256, 0x09);  // [4:0] WDC_CCMP1 ... The first point changing slope of compressed output. 
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0257, 0x0D);  // [4:0] WDC_CCMP2 ... The second point changing slope of compressed output. 
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0258, 0x75);  // [7:4] WDC_ACMP2  [3:0] WDC_ACMP1 .. Gain setting of the first/second region of compressed output.
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02CC, 0x31);  // [7:0]           ... Exposure ratio 16-times:21h, 32-times:31h
  #else
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0256, 0x09);  // [4:0] WDC_CCMP1 ... The first point changing slope of compressed output. 
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0257, 0x0C);  // [4:0] WDC_CCMP2 ... The second point changing slope of compressed output. 
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0258, 0x64);  // [7:4] WDC_ACMP2  [3:0] WDC_ACMP1 .. Gain setting of the first/second region of compressed output.
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02CC, 0x21);  // [7:0]           ... Exposure ratio 16-times:21h, 32-times:31h
  #endif
#endif

//---------------------------FIXE, SET~~-------------------------------------
	/*
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0204, 0x10);		// Fixed	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0205, 0x02);		// Fixed	, imx123 ADBIT
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0206, 0x00);		// Fixed	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0208, 0x01);		// Fixed	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x020c, 0x00);		// Fixed	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x020D, 0x00);		// Fixed	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x020e, 0x01);		// Fixed	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x020F, 0x01);		// Fixed	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0210, 0x01);		// Fixed	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0211, 0x00);		// Fixed	
	*/

	/*
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0212, 0x0e);		// Set : (Default Set: 0xF0)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0213, 0x01);		// Set : (Default: 0x00)
	*/

	/*	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0216, 0x09);		// Fixed	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0217, 0x00);		// Fixed		
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x021d, 0x00);		// Fixed	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0240, 0x01);		// Fixed	
	*/	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0241, 0x00);		// Fixed	datasheet err
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0242, 0x01);		// Fixed	datasheet err
	/*		
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0243, 0x00);		// Fixed	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0245, 0x00);		// Fixed	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0248, 0x2d);		// Fixed	
	*/	
	#if defined(__USE_CMOS_IMX123__)
	//Do not rewrite	
	#else
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x025A, 0x01);		// Set(Default Set:0x04)	-  about vsync
	#endif
	//24A~259, 25B~260	Do not rewrite
	//263~29C	Do not rewrite
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x029D, 0x82);		// Set(Default Set:0x40)	--> fliker sony datasheet setting v0.7
//	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x029E, 0x71);		// Fixed	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x029F, 0x01);		// Set(Default Set:0x41)	

	//2A0~2B2	Do not rewrite	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02B3, 0x91);		// Set (Default Set:0x91)	
//	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02B4, 0x00);		// Fixed	
//	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02B5, 0x60);		// Fixed	
//	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02B6, 0x96);		// Fixed	
//	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02B7, 0x10);		// Fixed	
	#if defined(__USE_CMOS_IMX123__)
	//Do not rewrite	
	#else
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x02B8, 0x04);		// Set  (Default Set:0x0F)		
	#endif
	//2B9~2FF	Do not rewrite
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0306, 0x07);		// Set (Default Set:0x06)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0307, 0x00);		// Set (Default Set:0x18)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0308, 0x00);		// Set (Default Set:0x18)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0309, 0x00);		// Set (Default Set:0x16)	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x030A, 0x00);		// Set (Default Set:0x10)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x030B, 0x00);		// Set (Default Set:0x08)
	//30C~30F	Do not rewrite
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0310, 0xF2);		// Set (Default Set:0x00)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0311, 0x03);		// Set (Default Set:0x04)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0312, 0xEB);		// Set (Default Set:0x2C)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0313, 0x07);		// Set (Default Set:0x05)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0314, 0xED);		// Set (Default Set:0x00)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0315, 0x07);		// Set (Default Set:0x06)	 
	//316~325	Do not rewrite	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0326, 0x91);		// Set (Default Set:0x11) 
	//327~329	Do not rewrite		
	

	//331~332	Do not rewrite			
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0333, 0x12);		// Set (Default Set:0x11)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0334, 0x10);		// Set (Default Set:0x1F)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0335, 0x12);		// Set (Default Set:0x11)
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0336, 0x10);		// Set (Default Set:0x1F) 	
	//337~339	Do not rewrite			
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x033A, 0x0C);		// Set (Default Set:0x0C) 		
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x033B, 0x0C);		// Set (Default Set:0x0C) 		
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x033C, 0x0C);		// Set (Default Set:0x0C) 		
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x033D, 0x0C);		// Set (Default Set:0x0C) 	
	//33E~33F	Do not rewrite			
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0340, 0x00);		// Set (Default Set:0x02) 			
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0341, 0x00);		// Set (Default Set:0x04) 			
//	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0342, 0x00);		// Fixed		
//	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0343, 0x00);		// Fixed		
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0344, 0x1E);		// Set (Default Set:0x00) 		
	//345~348	Do not rewrite			
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0349, 0x55);		// Set (Default Set:0x00) 	
//	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x034A, 0x1C);		// Fixed	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x034B, 0x99);		// Set (Default Set:0x99) 	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x034C, 0x99);		// Set (Default Set:0x99) 	
	//34D~353	Do not rewrite	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0354, 0xE7);		// Set (Default Set:0xF1) 
	//355~359	Do not rewrite		
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x035A, 0x04);		// Set (Default Set:0x05) 

	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0379, 0x94);		// Set (Default Set:0xFF) 	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x037A, 0x06);		// set (Default Set:0x07)	-  about vsync: 0x06 => 77 line out(Error)
	//37B-3EA	Do not rewrite			
	//3EB SETTING IMX123_HS_SETTING
	//3EC-3F5	Do not rewrite		
	#if defined(__USE_CMOS_IMX123__)
	//Do not rewrite	
	#else
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x03F6, 0x59);		// Set (Default Set:0x58) 	
	#endif
	//3F7-3FF	Do not rewrite		

//////////////////		Chip ID = 04h			//////////////////////	

	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0401, 0x3C);		// Set (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0402, 0x01);		// Set (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0403, 0x0E);		// Set (Default Set:0x00) 		 
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0413, 0x05);		// Set (Default Set:0x00) 	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x041F, 0x05);		// Set (Default Set:0x00) 			
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x045F, 0x03);		// Set (Default Set:0x00) 		
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0469, 0x03);		// Set (Default Set:0x00) 			
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x04B6, 0x03);		// Set (Default Set:0x00) 			
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x04BA, 0x01);		// Set (Default Set:0x00) 			
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x04C4, 0x01);		// Set (Default Set:0x00) 				
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x04CB, 0x01);		// Set (Default Set:0x00) 			
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x04D9, 0x80);		// Set (Default Set:0x10) 			
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x04DC, 0xB0);		// Set (Default Set:0xA0) 			
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x04DD, 0x13);		// Set (Default Set:0x0B) 			
	
	
//////////////////		Chip ID = 05h			//////////////////////	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x052A, 0xFF);		// Set (Default Set:0x2B) 	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x052B, 0xFF);		// Set (Default Set:0xA2) 	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x052C, 0xFF);		// Set (Default Set:0x32) 		
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x052D, 0xFF);		// Set (Default Set:0x2B) 			
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x052E, 0xFF);		// Set (Default Set:0xA2) 		
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x052F, 0xFF);		// Set (Default Set:0x32) 		
	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0535, 0x50);		// Set (Default Set:0x5D) 	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0536, 0x80);		// Set (Default Set:0x50) 	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0537, 0x1B);		// Set (Default Set:0x10) 	
	
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x053C, 0x01);		// Set (Default Set:0x00) 			
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x053D, 0x03);		// Set (Default Set:0x04)

	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0 : Normal operation
	Delay_mSec(40);

	//DCKRST reset sequence
	IMX124_DCKRST_Sequence();

//	CMOS_ByteRead(SPI_IMX124_ADDR, 0x0200);
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0202, 0x00);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop


}


//-----------------------------------IMX124_WDR_Mode---------------------------------------
void IMX124_WDR_Mode(IMX124_Mode_t Mode) {
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0200, 0x01);		// STANDBY[0] - 0: Operation,  1: STANDBY
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0201, 0x00);		// RegHold
	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0202, 0x01);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop

	//set register
	switch(Mode) {
		case IMX124_NORMAL:
			UARTprintf("IMX124_WDR_Mode:IMX124_NORMAL\r\n");
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x020C, 0x00);  // WDMODE[2:0] - 0:Normal, 2:Built-in, 4:DOL     WDSEL[5:4] - 0:Normal, 1:2frame, 2:3frame
			IMX124_SetProcMode(Mode);
			break;
		case IMX124_WDR:
			UARTprintf("IMX124_WDR_Mode:IMX124_WDR\r\n");
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x020C, 0x10);  // WDMODE[2:0] - 0:Normal, 2:Built-in, 4:DOL     WDSEL[5:4] - 0:Normal, 1:2frame, 2:3frame
			IMX124_SetProcMode(Mode);
			break;
		case IMX124_HTM:
			UARTprintf("IMX124_WDR_Mode:IMX124_HTM\r\n");
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x020C, 0x02);  // WDMODE[2:0] - 0:Normal, 2:Built-in, 4:DOL     WDSEL[5:4] - 0:Normal, 1:2frame, 2:3frame
			IMX124_SetProcMode(Mode);
			break;
		case IMX124_DOL2:
			UARTprintf("IMX124_WDR_Mode:IMX124_DOL2\r\n");
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x020C, 0x14);  // WDMODE[2:0] - 0:Normal, 2:Built-in, 4:DOL     WDSEL[5:4] - 0:Normal, 1:2frame, 2:3frame
			IMX124_SetProcMode(Mode);
			break;
		case IMX124_DOL3:
			UARTprintf("IMX124_WDR_Mode:IMX124_DOL3\r\n");
			CMOS_ByteWrite(SPI_IMX124_ADDR, 0x020C, 0x24);  // WDMODE[2:0] - 0:Normal, 2:Built-in, 4:DOL     WDSEL[5:4] - 0:Normal, 1:2frame, 2:3frame
			IMX124_SetProcMode(Mode);
			break;
		default:
			break;
	}

	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0200, 0x00);		// STANDBY[0] - 0 : Normal operation
	Delay_mSec(40);

	//DCKRST reset sequence
	IMX124_DCKRST_Sequence();

	CMOS_ByteWrite(SPI_IMX124_ADDR, 0x0202, 0x00);		// XMSTA [0] - 0: Master mode operation start, 1: Master mode operation stop
}


#endif	/* __USE_CMOS_IMX124__ */

/*  FILE_END_HERE */
