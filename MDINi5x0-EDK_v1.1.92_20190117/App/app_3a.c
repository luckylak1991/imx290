// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	<string.h>
#include	"video.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
#define 	__MISP100_ODMDBG__		1

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
void SetAECtrlOnOffToggle(void)
{
	LONG mode = stAECTRL.ctrl&AECTRL_PROCESS_OFF;

	if (mode==AECTRL_PROCESS_OFF)	mode = AECTRL_PROCESS_RUN;
	else							mode = AECTRL_PROCESS_OFF;

	stAECTRL.ctrl &= (~AECTRL_PROCESS_OFF);
	stAECTRL.ctrl |= (mode);

#if __MISP100_ODMDBG__ == 1
	if (mode==AECTRL_PROCESS_OFF)	UARTprintf("[AE] OFF!!!\r\n");
	else							UARTprintf("[AE] RUN!!!\r\n");
#endif
}

void SetAECtrlOnOff(BYTE onoff)
{
	LONG mode;

	if (onoff==ON)	mode = AECTRL_PROCESS_RUN;
	else				mode = AECTRL_PROCESS_OFF;

	stAECTRL.ctrl &= (~AECTRL_PROCESS_OFF);
	stAECTRL.ctrl |= (mode);

}

//--------------------------------------------------------------------------------------------------------------------------
void SetAE_BLCOnOff(void)
{
	LONG mode = stAECTRL.ctrl&AECTRL_BLC_MASK;

	if		(mode==AECTRL_BLC_MODE_OFF)	mode = AECTRL_BLC_MODE_BLC;
	else if (mode==AECTRL_BLC_MODE_BLC)	mode = AECTRL_BLC_MODE_HLC;
	else								mode = AECTRL_BLC_MODE_OFF;

	stAECTRL.ctrl &= (~AECTRL_BLC_MASK);
	stAECTRL.ctrl |= (mode|AECTRL_TRIGGER_ISP);

#if __MISP100_ODMDBG__ == 1
	if		(mode==AECTRL_BLC_MODE_OFF)	UARTprintf("[BLC] OFF!!!\r\n");
	else if (mode==AECTRL_BLC_MODE_BLC) UARTprintf("[BLC] RUN!!!\r\n");
	else								UARTprintf("[HLC] RUN!!!\r\n");
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void setSpotAEOnOff(BOOL onoff)
{
	if(onoff == TRUE) {
		stAECTRL.ctrl &= (~AECTRL_BLC_MASK);
		stAECTRL.ctrl |= (AECTRL_BLC_MODE_SPOT|AECTRL_TRIGGER_ISP);
	//	UARTprintf("AECTRL_BLC_MODE_SPOT ON!!!!!\r\n");


		//MISP_MultiRead(MISP_LOCAL_ID, 0x680, (PBYTE)&rAE_B, 4); // read AE wnd
		//MISP_MultiRead(MISP_LOCAL_ID, 0x680, rAE_B, 4); // read AE wnd
		//UARTprintf("r : rAE_B : %x %x\r\n", rAE_B[0], rAE_B[1]);
		//UARTprintf("r : rAE_B : %x %x %d %d\r\n", rAE_B[0], rAE_B[1], rAE_B[2], rAE_B[3]);
	} else {
		stAECTRL.ctrl &= (~AECTRL_BLC_MASK);
		stAECTRL.ctrl |= (AECTRL_BLC_MODE_OFF|AECTRL_TRIGGER_ISP);
		//UARTprintf("OFF\r\n");

		//UARTprintf("r : rAE_B : %x %x\r\n", rAE_B[0], rAE_B[1]);
		//UARTprintf("r : rAE_B : %x %x %d %d\r\n", rAE_B[0], rAE_B[1], rAE_B[2], rAE_B[3]);
		//MISP_MultiWrite(MISP_LOCAL_ID, 0x680, (PBYTE)&rAE_B, 4);
		//MISP_MultiWrite(MISP_LOCAL_ID, 0x680, rAE_B, 4);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void setSportAEPostion(BYTE x, BYTE y)
{
	WORD xInt, yInt;
	WORD rODM[3]; //, wVal[4];
	WORD xs, ys, xw, yh;

	DWORD mode = (stAECTRL.ctrl&AECTRL_BLC_MASK);

	if(mode == AECTRL_BLC_MODE_SPOT)
		stAECTRL.ctrl |= AECTRL_TRIGGER_ISP;

	MISP_MultiRead(MISP_LOCAL_ID, 0x60d, (PBYTE)&rODM, 6); // read ODM wnd

	//UARTprintf("ft : rODM : %d %d %d\r\n", rODM[0], rODM[1], rODM[2]);

	xInt = (GetOUT_MainSizeHA() / 16);
	yInt = (GetOUT_MainSizeVA() / 16);

	xw = 4*xInt - 1;
	if(x < 2) {
		xs = 0;
	} else {
		xs = ((WORD)x-2) * xInt;
	}

	yh = 5*xInt - 1;
	if(y < 2) {
		ys = 0;
	} else {
		ys = ((WORD)y-2) * yInt;
	}

	stAECTRL.stSPOT.sX = (BYTE)(xs / HIBYTE(rODM[2])); // x start
	stAECTRL.stSPOT.sY = (BYTE)(ys / LOBYTE(rODM[2])); // y start
	stAECTRL.stSPOT.nW = (BYTE)(xw / HIBYTE(rODM[2])); // x width
	stAECTRL.stSPOT.nH = (BYTE)(yh / LOBYTE(rODM[2])); // y hight
	//UARTprintf("ft : spot pos : %d %d %d %d\r\n", stAECTRL.stSPOT.sX, stAECTRL.stSPOT.sY, stAECTRL.stSPOT.nW, stAECTRL.stSPOT.nH);
}
//--------------------------------------------------------------------------------------------------------------------------
void SetAE_AGCOnOff(void)
{
/*
	LONG gain, mode = stAECTRL.ctrl&AECTRL_AGCX_MASK;

	switch (mode) {
		case AECTRL_GAINCTL_AUTO:	gain =    384; mode = AECTRL_GAINCTL_X1p5; break;
		case AECTRL_GAINCTL_X1p5:	gain =    512; mode = AECTRL_GAINCTL_X002; break;
		case AECTRL_GAINCTL_X002:	gain =    768; mode = AECTRL_GAINCTL_X003; break;
		case AECTRL_GAINCTL_X003:	gain =   1024; mode = AECTRL_GAINCTL_X004; break;
		case AECTRL_GAINCTL_X004:	gain =   1536; mode = AECTRL_GAINCTL_X006; break;
		case AECTRL_GAINCTL_X006:	gain =   2048; mode = AECTRL_GAINCTL_X008; break;
		case AECTRL_GAINCTL_X008:	gain =   3072; mode = AECTRL_GAINCTL_X012; break;
		case AECTRL_GAINCTL_X012:	gain =   4096; mode = AECTRL_GAINCTL_X016; break;
		case AECTRL_GAINCTL_X016:	gain =   6144; mode = AECTRL_GAINCTL_X024; break;
		case AECTRL_GAINCTL_X024:	gain =   8192; mode = AECTRL_GAINCTL_X032; break;
		case AECTRL_GAINCTL_X032:	gain =  12288; mode = AECTRL_GAINCTL_X048; break;
		case AECTRL_GAINCTL_X048:	gain =  16384; mode = AECTRL_GAINCTL_X064; break;
		case AECTRL_GAINCTL_X064:	gain =  24576; mode = AECTRL_GAINCTL_X096; break;
		case AECTRL_GAINCTL_X096:	gain =  32768; mode = AECTRL_GAINCTL_X128; break;
		case AECTRL_GAINCTL_X128:	gain =  49152; mode = AECTRL_GAINCTL_X192; break;
		case AECTRL_GAINCTL_X192:	gain =  65536; mode = AECTRL_GAINCTL_X256; break;
		case AECTRL_GAINCTL_X256:	gain =  98304; mode = AECTRL_GAINCTL_X384; break;
		case AECTRL_GAINCTL_X384:	gain = 131072; mode = AECTRL_GAINCTL_X512; break;
		case AECTRL_GAINCTL_X512:	gain = 196608; mode = AECTRL_GAINCTL_X768; break;
		case AECTRL_GAINCTL_X768:	gain = 262144; mode = AECTRL_GAINCTL_XA24; break;
		default:					gain =    256; mode = AECTRL_GAINCTL_AUTO; break;
	}

	if (mode==AECTRL_GAINCTL_AUTO)
		 AECTRL_SetGAINCtrl(&stAECTRL, gain, AECTRL_GAINCTL_AUTO);
	else AECTRL_SetGAINCtrl(&stAECTRL, gain, AECTRL_GAINCTL_MANU);

	stAECTRL.ctrl &= (~AECTRL_AGCX_MASK);
	stAECTRL.ctrl |= (mode);

	if		(mode==AECTRL_GAINCTL_AUTO)	UARTprintf("[GAIN] AUTO!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X1p5)	UARTprintf("[GAIN] X1.5!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X002)	UARTprintf("[GAIN] X002!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X003)	UARTprintf("[GAIN] X003!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X004)	UARTprintf("[GAIN] X004!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X006)	UARTprintf("[GAIN] X006!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X008)	UARTprintf("[GAIN] X008!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X012)	UARTprintf("[GAIN] X012!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X016)	UARTprintf("[GAIN] X016!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X024)	UARTprintf("[GAIN] X024!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X032)	UARTprintf("[GAIN] X032!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X048)	UARTprintf("[GAIN] X048!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X064)	UARTprintf("[GAIN] X064!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X096)	UARTprintf("[GAIN] X096!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X128)	UARTprintf("[GAIN] X128!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X192)	UARTprintf("[GAIN] X192!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X256)	UARTprintf("[GAIN] X256!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X384)	UARTprintf("[GAIN] X384!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X512)	UARTprintf("[GAIN] X512!!!\r\n");
	else if (mode==AECTRL_GAINCTL_X768)	UARTprintf("[GAIN] X768!!!\r\n");
	else								UARTprintf("[GAIN] X1024!!!\r\n");
*/
/*
	LONG min, max, mode = stAECTRL.ctrl&AECTRL_GAIN_MASK;

	if		(mode==AECTRL_GAINCTL_OFF)	mode = AECTRL_GAINCTL_MIN;
	else if (mode==AECTRL_GAINCTL_MIN)	mode = AECTRL_GAINCTL_MID;
	else if (mode==AECTRL_GAINCTL_MID)	mode = AECTRL_GAINCTL_MAX;
	else								mode = AECTRL_GAINCTL_OFF;

	stAECTRL.ctrl &= (~AECTRL_GAIN_MASK);
	stAECTRL.ctrl |= (mode);

	switch (mode) {
		case AECTRL_GAINCTL_OFF: min = 0x00100; max = 0x00100; break;
		case AECTRL_GAINCTL_MIN: min = 0x00100; max = 0x01fff; break;
		case AECTRL_GAINCTL_MID: min = 0x00100; max = 0x04fff; break;
		default:				 min = 0x00100; max = 0x10000; break;
	}

	AECTRL_SetGAINRange(min, max);	// set GAIN range

	if		(mode==AECTRL_GAINCTL_OFF)	UARTprintf("[GAIN] OFF!!!\r\n");
	else if (mode==AECTRL_GAINCTL_MIN)	UARTprintf("[GAIN] MIN!!!\r\n");
	else if (mode==AECTRL_GAINCTL_MID)	UARTprintf("[GAIN] MID!!!\r\n");
	else								UARTprintf("[GAIN] MAX!!!\r\n");
*/
}

//--------------------------------------------------------------------------------------------------------------------------
void SetAE_SHTOnOff(void)
{
	LONG shut =0 , mode = stAECTRL.ctrl&AECTRL_SHUT_MASK;

/*
#if defined(__USE_CMOS_MN34041__)	
	switch (mode) {
		case AECTRL_SHUTCTL_AUTO:	shut =  449; mode = AECTRL_SHUTCTL_1_50;	break;
		case AECTRL_SHUTCTL_1_50:	shut =  561; mode = AECTRL_SHUTCTL_1_60;	break;
		case AECTRL_SHUTCTL_1_60:	shut =  786; mode = AECTRL_SHUTCTL_1_100;	break;
		case AECTRL_SHUTCTL_1_100:	shut =  842; mode = AECTRL_SHUTCTL_1_120;	break;
		case AECTRL_SHUTCTL_1_120:	shut =  988; mode = AECTRL_SHUTCTL_1_250;	break;
		case AECTRL_SHUTCTL_1_250:	shut = 1055; mode = AECTRL_SHUTCTL_1_500;	break;
		case AECTRL_SHUTCTL_1_500:	shut = 1089; mode = AECTRL_SHUTCTL_1_1000;	break;
		case AECTRL_SHUTCTL_1_1000:	shut = 1106; mode = AECTRL_SHUTCTL_1_2000;	break;
		case AECTRL_SHUTCTL_1_2000:	shut = 1116; mode = AECTRL_SHUTCTL_1_5000;	break;
		case AECTRL_SHUTCTL_1_5000:	shut = 1119; mode = AECTRL_SHUTCTL_1_10000; break;
		default:					shut =    0; mode = AECTRL_SHUTCTL_AUTO;	break;
	}
#endif

#if defined(__USE_CMOS_MN34220__) || defined(__USE_CMOS_MN34420__)
	switch (mode) {
		case AECTRL_SHUTCTL_AUTO:	shut =  449; mode = AECTRL_SHUTCTL_1_50;	break;
		case AECTRL_SHUTCTL_1_50:	shut =  562; mode = AECTRL_SHUTCTL_1_60;	break;
		case AECTRL_SHUTCTL_1_60:	shut =  786; mode = AECTRL_SHUTCTL_1_100;	break;
		case AECTRL_SHUTCTL_1_100:	shut =  842; mode = AECTRL_SHUTCTL_1_120;	break;
		case AECTRL_SHUTCTL_1_120:	shut =  988; mode = AECTRL_SHUTCTL_1_250;	break;
		case AECTRL_SHUTCTL_1_250:	shut = 1056; mode = AECTRL_SHUTCTL_1_500;	break;
		case AECTRL_SHUTCTL_1_500:	shut = 1089; mode = AECTRL_SHUTCTL_1_1000;	break;
		case AECTRL_SHUTCTL_1_1000:	shut = 1106; mode = AECTRL_SHUTCTL_1_2000;	break;
		case AECTRL_SHUTCTL_1_2000:	shut = 1116; mode = AECTRL_SHUTCTL_1_5000;	break;
		case AECTRL_SHUTCTL_1_5000:	shut = 1120; mode = AECTRL_SHUTCTL_1_10000; break;
		default:					shut =    0; mode = AECTRL_SHUTCTL_AUTO;	break;
	}
#endif

#if defined(__USE_CMOS_IMX124__)	
	#if defined(__USE_IMX124_1536P30_MODE__)	//write exact value future
	switch (mode) {
		case AECTRL_SHUTCTL_AUTO:	shut =  640; mode = AECTRL_SHUTCTL_1_50;	break;
		case AECTRL_SHUTCTL_1_50:	shut =  800; mode = AECTRL_SHUTCTL_1_60;	break;
		case AECTRL_SHUTCTL_1_60:	shut = 1120; mode = AECTRL_SHUTCTL_1_100;	break;
		case AECTRL_SHUTCTL_1_100:	shut = 1200; mode = AECTRL_SHUTCTL_1_120;	break;
		case AECTRL_SHUTCTL_1_120:	shut = 1408; mode = AECTRL_SHUTCTL_1_250;	break;
		case AECTRL_SHUTCTL_1_250:	shut = 1504; mode = AECTRL_SHUTCTL_1_500;	break;
		case AECTRL_SHUTCTL_1_500:	shut = 1552; mode = AECTRL_SHUTCTL_1_1000;	break;
		case AECTRL_SHUTCTL_1_1000:	shut = 1576; mode = AECTRL_SHUTCTL_1_2000;	break;
		case AECTRL_SHUTCTL_1_2000:	shut = 1590; mode = AECTRL_SHUTCTL_1_5000;	break;
		case AECTRL_SHUTCTL_1_5000:	shut = 1595; mode = AECTRL_SHUTCTL_1_10000; break;
		default:					shut =    2; mode = AECTRL_SHUTCTL_AUTO;	break;
	}
	#endif

	#if defined(__USE_IMX124_1080P30_MODE__)	//write exact value future
	switch (mode) {
		case AECTRL_SHUTCTL_AUTO:	shut =  450; mode = AECTRL_SHUTCTL_1_50;	break;
		case AECTRL_SHUTCTL_1_50:	shut =  562; mode = AECTRL_SHUTCTL_1_60;	break;
		case AECTRL_SHUTCTL_1_60:	shut =  787; mode = AECTRL_SHUTCTL_1_100;	break;
		case AECTRL_SHUTCTL_1_100:	shut =  844; mode = AECTRL_SHUTCTL_1_120;	break;
		case AECTRL_SHUTCTL_1_120:	shut =  990; mode = AECTRL_SHUTCTL_1_250;	break;
		case AECTRL_SHUTCTL_1_250:	shut = 1057; mode = AECTRL_SHUTCTL_1_500;	break;
		case AECTRL_SHUTCTL_1_500:	shut = 1091; mode = AECTRL_SHUTCTL_1_1000;	break;
		case AECTRL_SHUTCTL_1_1000:	shut = 1108; mode = AECTRL_SHUTCTL_1_2000;	break;
		case AECTRL_SHUTCTL_1_2000:	shut = 1118; mode = AECTRL_SHUTCTL_1_5000;	break;
		case AECTRL_SHUTCTL_1_5000:	shut = 1121; mode = AECTRL_SHUTCTL_1_10000; break;
		default:					shut =    2; mode = AECTRL_SHUTCTL_AUTO;	break;
	}
	#endif
#endif

#if defined(__USE_CMOS_IMXx36__)	
	switch (mode) {
		case AECTRL_SHUTCTL_AUTO:	shut =  450; mode = AECTRL_SHUTCTL_1_50;	break;
		case AECTRL_SHUTCTL_1_50:	shut =  562; mode = AECTRL_SHUTCTL_1_60;	break;
		case AECTRL_SHUTCTL_1_60:	shut =  787; mode = AECTRL_SHUTCTL_1_100;	break;
		case AECTRL_SHUTCTL_1_100:	shut =  844; mode = AECTRL_SHUTCTL_1_120;	break;
		case AECTRL_SHUTCTL_1_120:	shut =  990; mode = AECTRL_SHUTCTL_1_250;	break;
		case AECTRL_SHUTCTL_1_250:	shut = 1057; mode = AECTRL_SHUTCTL_1_500;	break;
		case AECTRL_SHUTCTL_1_500:	shut = 1091; mode = AECTRL_SHUTCTL_1_1000;	break;
		case AECTRL_SHUTCTL_1_1000:	shut = 1108; mode = AECTRL_SHUTCTL_1_2000;	break;
		case AECTRL_SHUTCTL_1_2000:	shut = 1118; mode = AECTRL_SHUTCTL_1_5000;	break;
		case AECTRL_SHUTCTL_1_5000:	shut = 1121; mode = AECTRL_SHUTCTL_1_10000; break;
		default:					shut =    0; mode = AECTRL_SHUTCTL_AUTO;	break;
	}
#endif

*/

	if (mode==AECTRL_SHUTCTL_AUTO)
		 AECTRL_SetSHUTCtrl(&stAECTRL, (SHORT)shut, AECTRL_SHUTCTL_AUTO);
	else AECTRL_SetSHUTCtrl(&stAECTRL, (SHORT)shut, AECTRL_SHUTCTL_MANU);

	stAECTRL.ctrl &= (~AECTRL_SHUT_MASK);
	stAECTRL.ctrl |= (mode);

#if __MISP100_ODMDBG__ == 1
	if		(mode==AECTRL_SHUTCTL_AUTO)		UARTprintf("[SHUT] AUTO!!!\r\n");
	else if (mode==AECTRL_SHUTCTL_1_25)		UARTprintf("[SHUT] 1/25s!!!\r\n");	
	else if (mode==AECTRL_SHUTCTL_1_30)		UARTprintf("[SHUT] 1/30s!!!\r\n");		
	else if (mode==AECTRL_SHUTCTL_1_50)		UARTprintf("[SHUT] 1/50s!!!\r\n");
	else if (mode==AECTRL_SHUTCTL_1_60)		UARTprintf("[SHUT] 1/60s!!!\r\n");
	else if (mode==AECTRL_SHUTCTL_1_100)		UARTprintf("[SHUT] 1/100s!!!\r\n");
	else if (mode==AECTRL_SHUTCTL_1_120)		UARTprintf("[SHUT] 1/120s!!!\r\n");
	else if (mode==AECTRL_SHUTCTL_1_150)		UARTprintf("[SHUT] 1/150s!!!\r\n");
	else if (mode==AECTRL_SHUTCTL_1_180)		UARTprintf("[SHUT] 1/180s!!!\r\n");
	else if (mode==AECTRL_SHUTCTL_1_200)		UARTprintf("[SHUT] 1/200s!!!\r\n");
	else if (mode==AECTRL_SHUTCTL_1_240)		UARTprintf("[SHUT] 1/240s!!!\r\n");	
	else if (mode==AECTRL_SHUTCTL_1_250)		UARTprintf("[SHUT] 1/250s!!!\r\n");	
	else if (mode==AECTRL_SHUTCTL_1_300)		UARTprintf("[SHUT] 1/300s!!!\r\n");		
	else if (mode==AECTRL_SHUTCTL_1_360)		UARTprintf("[SHUT] 1/360s!!!\r\n");
	else if (mode==AECTRL_SHUTCTL_1_480)		UARTprintf("[SHUT] 1/480s!!!\r\n");	
	else if (mode==AECTRL_SHUTCTL_1_500)		UARTprintf("[SHUT] 1/500s!!!\r\n");
	else if (mode==AECTRL_SHUTCTL_1_600)		UARTprintf("[SHUT] 1/600s!!!\r\n");	
	else if (mode==AECTRL_SHUTCTL_1_1000)	UARTprintf("[SHUT] 1/1000s!!!\r\n");
	else if (mode==AECTRL_SHUTCTL_1_2000)	UARTprintf("[SHUT] 1/2000s!!!\r\n");
	else if (mode==AECTRL_SHUTCTL_1_5000)	UARTprintf("[SHUT] 1/5000s!!!\r\n");
	else										UARTprintf("[SHUT] 1/10000s!!!\r\n");
#endif

}

//--------------------------------------------------------------------------------------------------------------------------
void SetWBCtrlOnOff(void)
{
	LONG mode = stWBCTRL.ctrl&WBCTRL_MODE_MASK;
	if (mode!=WBCTRL_WBHOLD_MODE) return;

	stWBCTRL.ctrl &= (~WBCTRL_PROCESS_OFF);		// run AUTO
#if __MISP100_ODMDBG__ == 1
	UARTprintf("[WB-HOLD] RUN!!!\r\n");
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetWBModeOnOff(void)
{
	LONG mode = stWBCTRL.ctrl&WBCTRL_MODE_MASK;

	if		(mode==WBCTRL_WBAUTO_MODE) mode = WBCTRL_WBHOLD_MODE;
	else if (mode==WBCTRL_WBHOLD_MODE) mode = WBCTRL_INDOOR_MODE;
	else if (mode==WBCTRL_INDOOR_MODE) mode = WBCTRL_OTDOOR_MODE;
	else if (mode==WBCTRL_OTDOOR_MODE) mode = WBCTRL_PRESET_MODE;
	else if (mode==WBCTRL_PRESET_MODE) mode = WBCTRL_MANUAL_MODE;
	else							   mode = WBCTRL_WBAUTO_MODE;

	stWBCTRL.ctrl &= (~WBCTRL_MODE_MASK);
	stWBCTRL.ctrl |= (mode|WBCTRL_TRIGGER_ISP);

#if __MISP100_ODMDBG__ == 1
	if (mode==WBCTRL_WBAUTO_MODE)	UARTprintf("[WB] AUTO!!!\r\n");
	if (mode==WBCTRL_WBHOLD_MODE)	UARTprintf("[WB] HOLD!!!\r\n");
	if (mode==WBCTRL_INDOOR_MODE)	UARTprintf("[WB] INDOOR!!!\r\n");
	if (mode==WBCTRL_OTDOOR_MODE)	UARTprintf("[WB] OUTDOOR!!!\r\n");
	if (mode==WBCTRL_PRESET_MODE)	UARTprintf("[WB] PRESET!!!\r\n");
	if (mode==WBCTRL_MANUAL_MODE)	UARTprintf("[WB] MANUAL!!!\r\n");
#endif
}

#if defined(__USE_AFZOOM_LENS__)
//--------------------------------------------------------------------------------------------------------------------------
void SetFOKSDrvLIMIT(void)
{
	LONG mode = (LONG)stAFCTRL.near;

#if defined(__USE_X30X_TAMRON__)
	if		(mode==AFFOKS_LIMIT_0p1M) mode = AFFOKS_LIMIT_0p3M;
	else if (mode==AFFOKS_LIMIT_0p3M) mode = AFFOKS_LIMIT_1p0M;
	else if (mode==AFFOKS_LIMIT_1p0M) mode = AFFOKS_LIMIT_1p5M;
	else if (mode==AFFOKS_LIMIT_1p5M) mode = AFFOKS_LIMIT_2p0M;
	else if (mode==AFFOKS_LIMIT_2p0M) mode = AFFOKS_LIMIT_3p0M;
	else if (mode==AFFOKS_LIMIT_3p0M) mode = AFFOKS_LIMIT_6p0M;
	else if (mode==AFFOKS_LIMIT_6p0M) mode = AFFOKS_LIMIT_10M;
	else if (mode==AFFOKS_LIMIT_10M)  mode = AFFOKS_LIMIT_INF;
	else							  mode = AFFOKS_LIMIT_0p1M;

	LENS_SetFOKSDrvLIMIT(&stAFCTRL, (SHORT)mode);

  #if __MISP100_ODMDBG__ == 1
	if (mode==AFFOKS_LIMIT_0p1M)	UARTprintf("[DISTANCE] 0.1m!!!\r\n");
	if (mode==AFFOKS_LIMIT_0p3M)	UARTprintf("[DISTANCE] 0.3m!!!\r\n");
	if (mode==AFFOKS_LIMIT_1p0M)	UARTprintf("[DISTANCE] 1.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_1p5M)	UARTprintf("[DISTANCE] 1.5m!!!\r\n");
	if (mode==AFFOKS_LIMIT_2p0M)	UARTprintf("[DISTANCE] 2.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_3p0M)	UARTprintf("[DISTANCE] 3.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_6p0M)	UARTprintf("[DISTANCE] 6.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_10M)		UARTprintf("[DISTANCE] 10m!!!\r\n");
	if (mode==AFFOKS_LIMIT_INF)		UARTprintf("[DISTANCE] INF!!!\r\n");
  #endif

#elif defined(__USE_X10X_UNION__)
	if		(mode==AFFOKS_LIMIT_0p1M) mode = AFFOKS_LIMIT_0p3M;
	else if (mode==AFFOKS_LIMIT_0p3M) mode = AFFOKS_LIMIT_1p0M;
	else if (mode==AFFOKS_LIMIT_1p0M) mode = AFFOKS_LIMIT_1p5M;
	else if (mode==AFFOKS_LIMIT_1p5M) mode = AFFOKS_LIMIT_2p0M;
	else if (mode==AFFOKS_LIMIT_2p0M) mode = AFFOKS_LIMIT_3p0M;
	else if (mode==AFFOKS_LIMIT_3p0M) mode = AFFOKS_LIMIT_6p0M;
	else if (mode==AFFOKS_LIMIT_6p0M) mode = AFFOKS_LIMIT_10M;
	else if (mode==AFFOKS_LIMIT_10M)  mode = AFFOKS_LIMIT_INF;
	else							  mode = AFFOKS_LIMIT_0p1M;

	LENS_SetFOKSDrvLIMIT(&stAFCTRL, (SHORT)mode);

  #if __MISP100_ODMDBG__ == 1
	if (mode==AFFOKS_LIMIT_0p1M)	UARTprintf("[DISTANCE] 0.1m!!!\r\n");
	if (mode==AFFOKS_LIMIT_0p3M)	UARTprintf("[DISTANCE] 0.3m!!!\r\n");
	if (mode==AFFOKS_LIMIT_1p0M)	UARTprintf("[DISTANCE] 1.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_1p5M)	UARTprintf("[DISTANCE] 1.5m!!!\r\n");
	if (mode==AFFOKS_LIMIT_2p0M)	UARTprintf("[DISTANCE] 2.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_3p0M)	UARTprintf("[DISTANCE] 3.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_6p0M)	UARTprintf("[DISTANCE] 6.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_10M)		UARTprintf("[DISTANCE] 10m!!!\r\n");
	if (mode==AFFOKS_LIMIT_INF)		UARTprintf("[DISTANCE] INF!!!\r\n");
  #endif

#elif defined(__USE_X10X_ZMAX__) 
	if		(mode==AFFOKS_LIMIT_0p1M) mode = AFFOKS_LIMIT_0p5M;
	else if (mode==AFFOKS_LIMIT_0p5M) mode = AFFOKS_LIMIT_1p0M;
	else if (mode==AFFOKS_LIMIT_1p0M) mode = AFFOKS_LIMIT_1p5M;
	else if (mode==AFFOKS_LIMIT_1p5M) mode = AFFOKS_LIMIT_2p0M;
	else if (mode==AFFOKS_LIMIT_2p0M) mode = AFFOKS_LIMIT_3p0M;
	else if (mode==AFFOKS_LIMIT_3p0M) mode = AFFOKS_LIMIT_5p0M;
	else if (mode==AFFOKS_LIMIT_5p0M) mode = AFFOKS_LIMIT_10M;
	else if (mode==AFFOKS_LIMIT_10M)  mode = AFFOKS_LIMIT_INF;
	else							  mode = AFFOKS_LIMIT_0p1M;

	LENS_SetFOKSDrvLIMIT(&stAFCTRL, (SHORT)mode);

  #if __MISP100_ODMDBG__ == 1
	if (mode==AFFOKS_LIMIT_0p1M)	UARTprintf("[DISTANCE] 0.1m!!!\r\n");
	if (mode==AFFOKS_LIMIT_0p5M)	UARTprintf("[DISTANCE] 0.5m!!!\r\n");
	if (mode==AFFOKS_LIMIT_1p0M)	UARTprintf("[DISTANCE] 1.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_1p5M)	UARTprintf("[DISTANCE] 1.5m!!!\r\n");
	if (mode==AFFOKS_LIMIT_2p0M)	UARTprintf("[DISTANCE] 2.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_3p0M)	UARTprintf("[DISTANCE] 3.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_5p0M)	UARTprintf("[DISTANCE] 5.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_10M)		UARTprintf("[DISTANCE] 10m!!!\r\n");
	if (mode==AFFOKS_LIMIT_INF)		UARTprintf("[DISTANCE] INF!!!\r\n");
  #endif

#elif defined(__USE_X12X_UNION__)
	if		(mode==AFFOKS_LIMIT_0p1M) mode = AFFOKS_LIMIT_0p5M;
	else if (mode==AFFOKS_LIMIT_0p5M) mode = AFFOKS_LIMIT_1p0M;
	else if (mode==AFFOKS_LIMIT_1p0M) mode = AFFOKS_LIMIT_1p5M;
	else if (mode==AFFOKS_LIMIT_1p5M) mode = AFFOKS_LIMIT_2p0M;
	else if (mode==AFFOKS_LIMIT_2p0M) mode = AFFOKS_LIMIT_3p0M;
	else if (mode==AFFOKS_LIMIT_3p0M) mode = AFFOKS_LIMIT_5p0M;
	else if (mode==AFFOKS_LIMIT_5p0M) mode = AFFOKS_LIMIT_10M;
	else if (mode==AFFOKS_LIMIT_10M)  mode = AFFOKS_LIMIT_INF;
	else							  mode = AFFOKS_LIMIT_0p1M;

	LENS_SetFOKSDrvLIMIT(&stAFCTRL, (SHORT)mode);

  #if __MISP100_ODMDBG__ == 1
	if (mode==AFFOKS_LIMIT_0p1M)	UARTprintf("[DISTANCE] 0.1m!!!\r\n");
	if (mode==AFFOKS_LIMIT_0p5M)	UARTprintf("[DISTANCE] 0.5m!!!\r\n");
	if (mode==AFFOKS_LIMIT_1p0M)	UARTprintf("[DISTANCE] 1.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_1p5M)	UARTprintf("[DISTANCE] 1.5m!!!\r\n");
	if (mode==AFFOKS_LIMIT_2p0M)	UARTprintf("[DISTANCE] 2.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_3p0M)	UARTprintf("[DISTANCE] 3.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_5p0M)	UARTprintf("[DISTANCE] 5.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_10M)		UARTprintf("[DISTANCE] 10m!!!\r\n");
	if (mode==AFFOKS_LIMIT_INF)		UARTprintf("[DISTANCE] INF!!!\r\n");
  #endif

#elif defined(__USE_X03X_ZMAX__)
	if		(mode==AFFOKS_LIMIT_0p5M) mode = AFFOKS_LIMIT_1p0M;
	else if (mode==AFFOKS_LIMIT_1p0M) mode = AFFOKS_LIMIT_1p5M;
	else if (mode==AFFOKS_LIMIT_1p5M) mode = AFFOKS_LIMIT_2p0M;
	else if (mode==AFFOKS_LIMIT_2p0M) mode = AFFOKS_LIMIT_2p5M;
	else if (mode==AFFOKS_LIMIT_2p5M) mode = AFFOKS_LIMIT_3p0M;
	else if (mode==AFFOKS_LIMIT_3p0M) mode = AFFOKS_LIMIT_5p0M;
	else if (mode==AFFOKS_LIMIT_5p0M) mode = AFFOKS_LIMIT_10M;
	else if (mode==AFFOKS_LIMIT_10M)  mode = AFFOKS_LIMIT_INF;
	else							  mode = AFFOKS_LIMIT_0p5M;

	LENS_SetFOKSDrvLIMIT(&stAFCTRL, (SHORT)mode);

  #if __MISP100_ODMDBG__ == 1
	if (mode==AFFOKS_LIMIT_0p5M)	UARTprintf("[DISTANCE] 0.5m!!!\r\n");
	if (mode==AFFOKS_LIMIT_1p0M)	UARTprintf("[DISTANCE] 1.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_1p5M)	UARTprintf("[DISTANCE] 1.5m!!!\r\n");
	if (mode==AFFOKS_LIMIT_2p0M)	UARTprintf("[DISTANCE] 2.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_2p5M)	UARTprintf("[DISTANCE] 2.5m!!!\r\n");
	if (mode==AFFOKS_LIMIT_3p0M)	UARTprintf("[DISTANCE] 3.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_5p0M)	UARTprintf("[DISTANCE] 5.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_10M)		UARTprintf("[DISTANCE] 10m!!!\r\n");
	if (mode==AFFOKS_LIMIT_INF)		UARTprintf("[DISTANCE] INF!!!\r\n");
  #endif

#elif defined(__USE_YT3010_2MOTOR__)
	if		(mode==AFFOKS_LIMIT_0p5M) mode = AFFOKS_LIMIT_1p0M;
	else if (mode==AFFOKS_LIMIT_1p0M) mode = AFFOKS_LIMIT_1p5M;
	else if (mode==AFFOKS_LIMIT_1p5M) mode = AFFOKS_LIMIT_2p0M;
	else if (mode==AFFOKS_LIMIT_2p0M) mode = AFFOKS_LIMIT_2p5M;
	else if (mode==AFFOKS_LIMIT_2p5M) mode = AFFOKS_LIMIT_3p0M;
	else if (mode==AFFOKS_LIMIT_3p0M) mode = AFFOKS_LIMIT_5p0M;
	else if (mode==AFFOKS_LIMIT_5p0M) mode = AFFOKS_LIMIT_10M;
	else if (mode==AFFOKS_LIMIT_10M)  mode = AFFOKS_LIMIT_INF;
	else							  mode = AFFOKS_LIMIT_0p5M;

	LENS_SetFOKSDrvLIMIT(&stAFCTRL, (SHORT)mode);

  #if __MISP100_ODMDBG__ == 1
	if (mode==AFFOKS_LIMIT_0p5M)	UARTprintf("[DISTANCE] 0.5m!!!\r\n");
	if (mode==AFFOKS_LIMIT_1p0M)	UARTprintf("[DISTANCE] 1.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_1p5M)	UARTprintf("[DISTANCE] 1.5m!!!\r\n");
	if (mode==AFFOKS_LIMIT_2p0M)	UARTprintf("[DISTANCE] 2.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_2p5M)	UARTprintf("[DISTANCE] 2.5m!!!\r\n");
	if (mode==AFFOKS_LIMIT_3p0M)	UARTprintf("[DISTANCE] 3.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_5p0M)	UARTprintf("[DISTANCE] 5.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_10M)		UARTprintf("[DISTANCE] 10m!!!\r\n");
	if (mode==AFFOKS_LIMIT_INF)		UARTprintf("[DISTANCE] INF!!!\r\n");
  #endif

#else
	if		(mode==AFFOKS_LIMIT_0p5M) mode = AFFOKS_LIMIT_1p0M;
	else if (mode==AFFOKS_LIMIT_1p0M) mode = AFFOKS_LIMIT_1p5M;
	else if (mode==AFFOKS_LIMIT_1p5M) mode = AFFOKS_LIMIT_2p0M;
	else if (mode==AFFOKS_LIMIT_2p0M) mode = AFFOKS_LIMIT_2p5M;
	else if (mode==AFFOKS_LIMIT_2p5M) mode = AFFOKS_LIMIT_3p0M;
	else if (mode==AFFOKS_LIMIT_3p0M) mode = AFFOKS_LIMIT_5p0M;
	else if (mode==AFFOKS_LIMIT_5p0M) mode = AFFOKS_LIMIT_10M;
	else if (mode==AFFOKS_LIMIT_10M)  mode = AFFOKS_LIMIT_INF;
	else							  mode = AFFOKS_LIMIT_0p5M;

	LENS_SetFOKSDrvLIMIT(&stAFCTRL, (SHORT)mode);
  #if __MISP100_ODMDBG__ == 1
	if (mode==AFFOKS_LIMIT_0p5M)	UARTprintf("[DISTANCE] 0.5m!!!\r\n");
	if (mode==AFFOKS_LIMIT_1p0M)	UARTprintf("[DISTANCE] 1.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_1p5M)	UARTprintf("[DISTANCE] 1.5m!!!\r\n");
	if (mode==AFFOKS_LIMIT_2p0M)	UARTprintf("[DISTANCE] 2.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_2p5M)	UARTprintf("[DISTANCE] 2.5m!!!\r\n");
	if (mode==AFFOKS_LIMIT_3p0M)	UARTprintf("[DISTANCE] 3.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_5p0M)	UARTprintf("[DISTANCE] 5.0m!!!\r\n");
	if (mode==AFFOKS_LIMIT_10M)		UARTprintf("[DISTANCE] 10m!!!\r\n");
	if (mode==AFFOKS_LIMIT_INF)		UARTprintf("[DISTANCE] INF!!!\r\n");
  #endif
#endif

}

//--------------------------------------------------------------------------------------------------------------------------
void SetZOOMDrvSPEED(void)
{
	LONG mode = stAFCTRL.ctrl&AFZOOM_SPEED_MASK;

	if		(mode==AFZOOM_SPEED_VAR0) mode = AFZOOM_SPEED_VAR1;
	else if (mode==AFZOOM_SPEED_VAR1) mode = AFZOOM_SPEED_VAR2;
	else if (mode==AFZOOM_SPEED_VAR2) mode = AFZOOM_SPEED_VAR3;
	else if (mode==AFZOOM_SPEED_VAR3) mode = AFZOOM_SPEED_VAR4;
	else if (mode==AFZOOM_SPEED_VAR4) mode = AFZOOM_SPEED_VAR5;
	else if (mode==AFZOOM_SPEED_VAR5) mode = AFZOOM_SPEED_VAR6;
	else if (mode==AFZOOM_SPEED_VAR6) mode = AFZOOM_SPEED_VAR7;
	else							  mode = AFZOOM_SPEED_VAR0;

	LENS_SetZOOMDrvSPEED(&stAFCTRL, mode);

#if __MISP100_ODMDBG__ == 1
	if (mode==AFZOOM_SPEED_VAR0)	UARTprintf("[SPEED] VAR0!!!\r\n");
	if (mode==AFZOOM_SPEED_VAR1)	UARTprintf("[SPEED] VAR1!!!\r\n");
	if (mode==AFZOOM_SPEED_VAR2)	UARTprintf("[SPEED] VAR2!!!\r\n");
	if (mode==AFZOOM_SPEED_VAR3)	UARTprintf("[SPEED] VAR3!!!\r\n");
	if (mode==AFZOOM_SPEED_VAR4)	UARTprintf("[SPEED] VAR4!!!\r\n");
	if (mode==AFZOOM_SPEED_VAR5)	UARTprintf("[SPEED] VAR5!!!\r\n");
	if (mode==AFZOOM_SPEED_VAR6)	UARTprintf("[SPEED] VAR6!!!\r\n");
	if (mode==AFZOOM_SPEED_VAR7)	UARTprintf("[SPEED] VAR7!!!\r\n");
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetWIDEDrvLIMIT(SHORT val)
{
	// where "val" is from 0 to 255 in tamron zoom lens
	LENS_SetWIDEDrvLIMIT(&stAFCTRL, val);
}

//--------------------------------------------------------------------------------------------------------------------------
void SetTELEDrvLIMIT(SHORT val)
{
	// where "val" is from 0 to 255 in tamron zoom lens
	LENS_SetTELEDrvLIMIT(&stAFCTRL, val);
}

//--------------------------------------------------------------------------------------------------------------------------
void SetAFIntvalTIME(WORD val)
{
	LENS_SetAFIntvalTIME(&stAFCTRL, val);
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
void GetAEDataState(void)
{
#if __MISP100_ODMDBG__ == 1
#if defined(__USE_DCIRIS_HALL__) ||defined(__USE_DCIRIS_DAMP__	) || defined(__USE_P_IRIS_ONLY__)
	UARTprintf("[AE] sumY = %d, IRIS = %04X, GAIN = %04X, SHUT = %04X\r\n",
		Get_AE_Ysum(), DCLENS_GetIRISGain(), AECTRL_GetGAINGain(), AECTRL_GetSHUTGain());
#else
	UARTprintf("[AE] sumY = %d, GAIN = %04X, SHUT = %04X\r\n",
		Get_AE_Ysum(), AECTRL_GetGAINGain(), AECTRL_GetSHUTGain());
#endif
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void GetAFDataState(void)
{
#if __MISP100_ODMDBG__ == 1
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
//	UARTprintf("[AF] zREL = %d, fREL = %d\r\n", LENS_GetRelPositionZOOM(), LENS_GetRelPositionFOKS());
	UARTprintf("[AF] zCUR = %d[%04X]", LENS_GetCurPositionZOOM(), (WORD)LENS_GetVISCAPositionZOOM());
	UARTprintf(" fCUR = %d[%04X]\r\n", LENS_GetCurPositionFOKS(), (WORD)LENS_GetVISCAPositionFOKS());
#endif
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void SetAEValUpCtrl(void)
{
	if ((stAECTRL.ctrl&AECTRL_PROCESS_OFF)==AECTRL_PROCESS_OFF) return;

	SetMsgCommandRepeatKey(MSG_LEVEL);	// set repeater MsgFlag
	stAECTRL.level = MIN(stAECTRL.level+1,100);
	AECTRL_SetBrightness(&stAECTRL, stAECTRL.level);
	//UARTprintf("[AE] level = %d\r\n", stAECTRL.level);
}

//--------------------------------------------------------------------------------------------------------------------------
void SetAEValDnCtrl(void)
{
	if ((stAECTRL.ctrl&AECTRL_PROCESS_OFF)==AECTRL_PROCESS_OFF) return;

	SetMsgCommandRepeatKey(MSG_LEVEL);	// set repeater MsgFlag
	stAECTRL.level = MAX(stAECTRL.level-1,0);
	AECTRL_SetBrightness(&stAECTRL, stAECTRL.level);
	//UARTprintf("[AE] level = %d\r\n", stAECTRL.level);
}


LONG GetAFOPDWIND(BYTE sel)
{
	WORD rVal[16];
	LONG res[13];

	if(sel > 12) sel = 0;

	MISP_MultiRead(MISP_LOCAL_ID, 0x6f0, (PBYTE)rVal, 32);
	res[ 0] = (LONG)(MAKEDWORD(rVal[ 0], rVal[ 1])>>0); //RPFa data of window A
	res[ 1] = (LONG)(MAKEDWORD(rVal[ 2], rVal[ 3])>>0); //LPFa data of window A
	res[ 2] = (LONG)(MAKEDWORD(rVal[ 4], rVal[ 5])>>0); //VPFa data of window A
	res[ 3] = (LONG)(MAKEDWORD(rVal[ 6], rVal[ 7])>>0); //HPFa data of window A

	res[ 4] = (LONG)(MAKEDWORD(rVal[ 8], rVal[ 9])>>0); //RPFb data of window B
	res[ 5] = (LONG)(MAKEDWORD(rVal[10], rVal[11])>>0); //LPFb data of window B
	res[ 6] = (LONG)(MAKEDWORD(rVal[12], rVal[13])>>0); //VPFb data of window B
	res[ 7] = (LONG)(MAKEDWORD(rVal[14], rVal[15])>>0); //HPFb data of window B

	MISP_MultiRead(MISP_LOCAL_ID, 0x6E8, (PBYTE)rVal, 16);
	res[ 8] = (LONG)(MAKEDWORD(rVal[0], rVal[1])>>0); //CNTa data of window A
	res[ 9] = (LONG)(MAKEDWORD(rVal[2], rVal[3])>>0); //BPFa data of window A
	res[10] = (LONG)(MAKEDWORD(rVal[4], rVal[5])>>0); //CNTb data of window B
	res[11] = (LONG)(MAKEDWORD(rVal[6], rVal[7])>>0); //BPFb  data of window B

	res[12] = Get_AE_Ysum();

	//UARTprintf("a:%d %d %d %d %d / b:%d %d %d %d %d /cnt:%d %d / YSUM:%d\r\n",res[0],res[1],res[2],res[3],res[4],res[5],res[6],res[7],res[8],res[9],res[10],res[11],res[12]);

	return res[sel];

}


/*  FILE_END_HERE */
