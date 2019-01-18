//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2015  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	MISC.C
// Description 		:	Privacy, Day&Night, D.Zoom
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"
#include	"osd.h"		//test!!!

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
DYNT_CTRL_PARAM		stDYNTCTRL;
DZOOM_CTRL_PARAM	stDZOOMCTRL;
BLC_CTRL_PARAM		stBLCCTRL;
HLC_CTRL_PARAM		stHLCCTRL;
VS_CTRL_PARAM		stVSCTRL;

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
#if defined(__USE_FHDCROP_MODE__)	
extern BYTE move_in_LR , move_in_UD ;
extern volatile BOOL fCropMode ;
#endif
// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//------------------------------------------------------------------------------------------
static void Init_DYNT_PARAM(PDYNT_CTRL_PARAM pCTRL)
{
	pCTRL->ctrl = DYNTCTRL_MODE_AUTO;
	
#if	defined(__MDIN_i5XX_FPGA__) || defined(__PANORAMA_SETUP__)			// fpga & panorama board only
	pCTRL->ctrl = DYNTCTRL_MODE_DAY;
#endif
	
	pCTRL->dw_time = 5;														// auto delay time(sec)
	pCTRL->dw_cnt = pCTRL->dw_time * GET_FPS;								// auto delay counter
	pCTRL->agc_d2n = 165;			pCTRL->agc_n2d = 65;				// agc level init
}

//--------------------------------------------------------------------------------------------------
static void Init_DZOOM_PARAM(PDZOOM_CTRL_PARAM pCTRL)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[14];
#endif
	
	pCTRL->DZratio = 0;														// dzoom ratio(0:x1 ~ 55:x12_1080p) -- inc/dec by direct posi command(visca,etc)
	pCTRL->DZtarget = 0;													// dzoom target ratio(0:x1 ~ 55:x12_1080p) -- inc/dec by menu
	pCTRL->DZoffset = 1;													// dzoom ratio offset(0:x1 ~ 55:x12_1080p) -- inc/dec by tele/wide command(visca,etc)
	pCTRL->DZposix = 120;													// dzoom position-x(0~240)
	pCTRL->DZposiy = 120;													// dzoom position-y(0~240)
	pCTRL->PIPonoff = ON; 													// pip display on/off
#if defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__)|| defined(__USE_CMOS_IMX334__)
	pCTRL->PIPsize = 2;														// pip size(0:1/4, 1:1/9, 2:1/16, 3:1/25)
#else
	pCTRL->PIPsize = 1;														// pip size(0:1/4, 1:1/9, 2:1/16, 3:1/25)
#endif
	pCTRL->PIPposix = 10; 													// pip position-x(0~10)
	pCTRL->PIPposiy = 10;													// pip position-y(0~10)
	pCTRL->DZwin.w = MIN(GetFC_MainFCsrcSizeH(), GetFC_MainFCdstSizeH());	// dzoom window width(unit:pixel)
	pCTRL->DZwin.h = GetFC_MainFCsrcSizeV();								// dzoom window height(unit:pixel)
	pCTRL->DZwin.x = 0;														// dzoom window position-x(unit:pixel)
	pCTRL->DZwin.y = 0;														// dzoom window position-y(unit:pixel)
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	EEPROM_MultiRead(EEPROM_DZOOM_MENU, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE) 	pCTRL->DZratio = MIN(buff[1], DZOOMCTRL_RATIO_MAX);
	if(buff[2] == EEPROM_SAVE_DONE) 	{pCTRL->DZposix = MIN(buff[3], 240);	pCTRL->DZposiy = MIN(buff[4], 240);}
	if(buff[6] == EEPROM_SAVE_DONE) 	pCTRL->PIPonoff = MIN(buff[7], 1);
	if(buff[8] == EEPROM_SAVE_DONE) 	pCTRL->PIPsize = MIN(buff[9], 3);
	if(buff[10] == EEPROM_SAVE_DONE)	{pCTRL->PIPposix = MIN(buff[11], 10);	pCTRL->PIPposiy = MIN(buff[12], 10);}
#endif

}

//--------------------------------------------------------------------------------------------------
static void Init_BLC_PARAM(PBLC_CTRL_PARAM pCTRL)
{
	WORD temp, odm_blk_w, odm_blk_h, odm_area_bgn_x, odm_area_bgn_y;
	
	pCTRL->x[0] = stAECTRL.stBLC.sX;		pCTRL->y[0] = stAECTRL.stBLC.sY;	// set size/position
	pCTRL->w[0] = stAECTRL.stBLC.nW;		pCTRL->h[0] = stAECTRL.stBLC.nH;
	pCTRL->x[1] = stAECTRL.stBLC.sX;		pCTRL->y[1] = stAECTRL.stBLC.sY;	// set default value
	pCTRL->w[1] = stAECTRL.stBLC.nW;		pCTRL->h[1] = stAECTRL.stBLC.nH;
	
	// read odm block size(w/h), odm window start position(x/y)
	MISP_RegRead(MISP_LOCAL_ID, 0x60D, &odm_area_bgn_x);
	MISP_RegRead(MISP_LOCAL_ID, 0x60E, &odm_area_bgn_y);
	MISP_RegRead(MISP_LOCAL_ID, 0x60F, &temp);
	odm_blk_w = HIBYTE(temp);				odm_blk_h = LOBYTE(temp);
	
	// calc blc window size limit with odm block size
	pCTRL->w_max = (GetIN_MainSrcSizeH() - odm_area_bgn_x) / odm_blk_w;
	pCTRL->h_max = (GetIN_MainSrcSizeV() - odm_area_bgn_y) / odm_blk_h;
	
	pCTRL->ctrl = BLCCTRL_WIND_ON;							// window display on
}

//--------------------------------------------------------------------------------------------------
static void Init_HLC_PARAM(PHLC_CTRL_PARAM pCTRL)
{
	WORD temp, odm_blk_w, odm_blk_h, odm_area_bgn_x, odm_area_bgn_y;
	
	pCTRL->x[0] = stAECTRL.stHLC.sX;		pCTRL->y[0] = stAECTRL.stHLC.sY;
	pCTRL->w[0] = stAECTRL.stHLC.nW;		pCTRL->h[0] = stAECTRL.stHLC.nH;	// set size/position
	pCTRL->x[1] = stAECTRL.stHLC.sX;		pCTRL->y[1] = stAECTRL.stHLC.sY;
	pCTRL->w[1] = stAECTRL.stHLC.nW;		pCTRL->h[1] = stAECTRL.stHLC.nH;	// set default value
	
	// read odm block size(w/h), odm window start position(x/y)
	MISP_RegRead(MISP_LOCAL_ID, 0x60D, &odm_area_bgn_x);
	MISP_RegRead(MISP_LOCAL_ID, 0x60E, &odm_area_bgn_y);
	MISP_RegRead(MISP_LOCAL_ID, 0x60F, &temp);
	odm_blk_w = HIBYTE(temp);				odm_blk_h = LOBYTE(temp);
	
	// calc hlc window size limit with odm block size
	pCTRL->w_max = (GetIN_MainSrcSizeH() - odm_area_bgn_x) / odm_blk_w;
	pCTRL->h_max = (GetIN_MainSrcSizeV() - odm_area_bgn_y) / odm_blk_h;
	
	
	pCTRL->mask_level[0] = 3;				pCTRL->mask_level[1] = 3;			// mask level(0~20)
	pCTRL->ctrl = (HLCCTRL_WIND_ON | HLCCTRL_MASK_ON);							// window/mask display on
}

//--------------------------------------------------------------------------------------------------
static void Init_VS_PARAM(PVS_CTRL_PARAM pCTRL)
{
	pCTRL->ctrl = VSCTRL_MODE_OFF | VSCTRL_SCAN_FULL;
	pCTRL->x = 0;
	pCTRL->y = 0;
	pCTRL->w = MIN(GetFC_MainFCsrcSizeH(), GetFC_MainFCdstSizeH());
	pCTRL->h = GetFC_MainFCsrcSizeV();
	pCTRL->w_max = MIN(GetFC_MainFCsrcSizeH(), GetFC_MainFCdstSizeH());
	pCTRL->h_max = GetFC_MainFCsrcSizeV();
}


//--------------------------------------------------------------------------------------------------
static void DYNT_SetICRDrvStopCount(WORD cnt)
{
	stDYNTCTRL.wOLPFCnt = cnt;				// icr driver stop counter
}

//--------------------------------------------------------------------------------------------------
static void DYNT_ICRDrvStopCounter(void)
{
	if(stDYNTCTRL.wOLPFCnt >= GET_FPS/10)	return;	// check 100ms
	
	stDYNTCTRL.wOLPFCnt++;
	
#if defined(__USE_MANU_WW_8mmIR__) || defined(__USE_MANU_WW_4KUHD__)
	if(stDYNTCTRL.wOLPFCnt == GET_FPS/10)	{OLPF_DY(LOW);	OLPF_NT(LOW);}		// icr driver stop
#else
	if(stDYNTCTRL.wOLPFCnt == GET_FPS/10)	{OLPF_DY(HIGH);	OLPF_NT(HIGH);}		// icr driver stop
#endif
}

//--------------------------------------------------------------------------------------------------
static BYTE DYNT_GetAGCNight(void)
{
	LONG  agc_gain, agc_max;//, iris_gain, shut_gain;
	
	if(GetIN_MainPath() != MISP_MAINPATH_SENSOR)	return 0;		// external main input(always day !)
	if(GetMenuStatus(EXP_MODE_MENU) != 0)			return 0;		// exposure not auto mode(always day !)
	if(GetWDR_ProcesMode())							return 0;		// wdr mode(controls saturation itself !)
	
	agc_gain = AECTRL_GetGAINGain();
	agc_max = AECTRL_GetGAINPMax();
	//UARTprintf("gain:%d/max:%d\r\n",agc_gain,agc_max);

	if(agc_gain < DYNTCTRL_AGC_OFFSET)		agc_gain = 0;
	else									agc_gain = (agc_gain - DYNTCTRL_AGC_OFFSET) / (agc_max / 256);	// gain -> 0~255
	
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	if(stDYNTCTRL.ctrl & DYNTCTRL_IS_NIGHT)		agc_gain = (agc_gain * 167)/100;		// gain * 1.67(agc decreased 40% when IR filter off)
#endif
	
#if defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__) || defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX334__)
//	return 0;	// always day -- imx274 color noise is low in lowlight mode
#endif

#if defined(__USE_GET_AWB_RGB_INFO__)
	//if((Get_AE_Ydiff()) > 5000) return 1; // night ! mimitest
#endif

	if(agc_gain >=  stDYNTCTRL.agc_d2n)			return 1;	// night !
	else if(agc_gain <= stDYNTCTRL.agc_n2d)		return 0;	// day !
	else											return 2;	// trans period !
}

//--------------------------------------------------------------------------------------------------
static BOOL DYNT_GetAutoModeNight(PWB_CTRL_PARAM pWBCTRL)
{
	BYTE agc_status = DYNT_GetAGCNight();							// 0:day, 1:night
	BYTE agc_status_old = (stDYNTCTRL.ctrl & DYNTCTRL_IS_NIGHT)?  1:  0;	// 0:day, 1:night

	if(agc_status == 2) {
		return agc_status_old;				// agc status == mid range ?
	}

  #if defined(__USE_GET_AWB_RGB_INFO__)
	if(agc_status != agc_status_old)	{
		if(agc_status_old == 1) {
			// option add...if bright then thres. 
			if(pWBCTRL->stWBCOLOR.cntColor == 0) {
				if(stDYNTCTRL.dw_cnt == 0) {
					if(AECTRL_GetGAINRatio() == 0) {
						if(Get_YUV_Ratio(TRUE, 50)) { // IR ramp on status
							UARTprintf("IR lamp on...\r\n");
							stDYNTCTRL.dw_cnt = stDYNTCTRL.dw_time;
							return 1;
						} else {
							UARTprintf("IR lamp off...\r\n");
							stDYNTCTRL.dw_cnt = stDYNTCTRL.dw_time * GET_FPS;
							return 0;
						}
					}
				} else {
					stDYNTCTRL.dw_cnt--;
				}
			}
		} else {
			if(stDYNTCTRL.dw_cnt == 0) {
				stDYNTCTRL.dw_cnt = stDYNTCTRL.dw_time * GET_FPS;
				return agc_status;
			}else {
				stDYNTCTRL.dw_cnt--;
			}
		}
		//UARTprintf("pWBCTRL->stWBCOLOR.cntColor:%3d / agc_status_old:%1d / agc_status:%1d / stDYNTCTRL.dw_cnt:%5d\r\n",pWBCTRL->stWBCOLOR.cntColor,agc_status_old,agc_status,stDYNTCTRL.dw_cnt);
	} else {
		if(agc_status_old == 1)
			stDYNTCTRL.dw_cnt = stDYNTCTRL.dw_time;
		else
			stDYNTCTRL.dw_cnt = stDYNTCTRL.dw_time * GET_FPS;
	}
	return 	agc_status_old;

#else

	if(agc_status != agc_status_old)	{									// agc status changed ???
		if(stDYNTCTRL.dw_cnt == 0) {
			stDYNTCTRL.dw_cnt = stDYNTCTRL.dw_time * GET_FPS;				// set user defined dwell time.
			return agc_status;					// return 'agc status' when counter == 0
		}else {
			stDYNTCTRL.dw_cnt--;				// decrease dwell counter
		}
	} else {
		stDYNTCTRL.dw_cnt = stDYNTCTRL.dw_time * GET_FPS;				// set user defined dwell time.
	}

	return 	agc_status_old;
#endif
}

//--------------------------------------------------------------------------------------------------
static BOOL DYNT_GetExtModeNight(void)
{
	return 	(GetADC_ConvData(ADC_SRC_VIN0) < 600)?  1:  0;		// 0:day, 1:night(under 2V ?)
}

//--------------------------------------------------------------------------------------------------------------------------
static void DZOOM_SetRatio(WORD val)		//0:off, 1:x1.1,,,
{
	WORD	Posi_x, Posi_y, step_x, step_y, step_w, step_h, old_w, old_h, src_w, src_h;
	WORD	cut_h, cut_v, src_ratio;
	PMISP_VIDEO_WINDOW	pDZWIN = &stDZOOMCTRL.DZwin;
	PMISP_OUTVIDEO_SYNC	pOUTSYNC = GetOUT_MainSync();
	
	src_w = MIN(GetFC_MainFCsrcSizeH(), GetFC_MainFCdstSizeH());		// v.in > out, use v.out hsize(ffc on..)
	src_h = GetFC_MainFCsrcSizeV();

	// calc cut size
	src_ratio = GetFC_MainFCsrcSizeH()*10 / GetFC_MainFCsrcSizeV();
	if(src_ratio > 15)	{cut_h = src_w*32 / 1920;	cut_v = src_h*18 / 1080;}	// 16:9
	else				{cut_h = src_w*32 / 2048;	cut_v = src_h*24 / 1536;}	// 4:3
	
	cut_h += (cut_h%2);							cut_v += (cut_v%2);

	// calc size
	old_w = pDZWIN->w; 								old_h = pDZWIN->h;
	pDZWIN->w = src_w - (val * cut_h);				pDZWIN->h = src_h - (val * cut_v);
	
	if(old_w == pDZWIN->w)		return;	//zoom in/out max !!!
	
	// calc position step
	step_w = DIFF(old_w, pDZWIN->w)/2;				step_h = DIFF(old_h, pDZWIN->h)/2;
	step_x = (src_w - pDZWIN->w)*100/240;			step_y = (src_h - pDZWIN->h)*100/240;
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	step_w *= 2;					// for fix right end position
#elif defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD2__)
	step_w = 0;						// for fix left start position
#endif
	
	// calc position
	if(old_w > pDZWIN->w)	{	//zoom in
		pDZWIN->x += step_w;						pDZWIN->y += step_h;
	}
	else	{						//zoom out
		pDZWIN->x = (pDZWIN->x < step_w)?  0:  (pDZWIN->x - step_w);
		pDZWIN->y = (pDZWIN->y < step_h)?  0:  (pDZWIN->y - step_h);
	}
	
	//adj position(check h/v position limit)
	Posi_x = GetMenuStatus(DZPOSI_MENU),			Posi_y = GetMenuPosiY(DZPOSI_MENU);
	if((pDZWIN->x + pDZWIN->w) > src_w)			pDZWIN->x = src_w - pDZWIN->w; 	// h-max ?
	if((pDZWIN->y + pDZWIN->h) > src_h)			pDZWIN->y = src_h - pDZWIN->h; 	// v-max ?
	
	//save position data to menu.
	Posi_x = (pDZWIN->x * 100) / step_x;			Posi_y = (pDZWIN->y * 100) / step_y;
	SetMenuStatus(DZPOSI_MENU, MIN(Posi_x, 240));	SetMenuPosiY(DZPOSI_MENU, MIN(Posi_y, 240));
	
	//set fc reg.
	SetFC_MainFCsrcOverscanH(src_w - pDZWIN->w); 	SetFC_MainFCsrcOverscanV(src_h - pDZWIN->h);
#if defined(__USE_FHDCROP_MODE__)	 // Crop Dzoom mode	
	if ( fCropMode ) {SetFC_MainFCsrcPosiX(pDZWIN->x+move_in_LR*64);				SetFC_MainFCsrcPosiY(pDZWIN->y+move_in_UD*36);}
	else {SetFC_MainFCsrcPosiX(pDZWIN->x);				SetFC_MainFCsrcPosiY(pDZWIN->y);}	
#else // Noemal operation
	SetFC_MainFCsrcPosiX(pDZWIN->x);				SetFC_MainFCsrcPosiY(pDZWIN->y);
#endif	
	// overrun check
	if(val > 0)		MISP_RegWrite(MISP_LOCAL_ID, 0x09E, (pOUTSYNC->totVS - pDZWIN->y) | (1<<15));	// main overrun en & set offset
	else			MISP_RegWrite(MISP_LOCAL_ID, 0x09E, 0x0000);	//overrun check disable
	
	if(val) MISP_RegField(MISP_LOCAL_ID, 0x480, 11, 1, 0);		//bayer fczip on
	else MISP_RegField(MISP_LOCAL_ID, 0x480, 11, 1, 1);			//if 1x fc zip off

#if __MISP100_DBGPRT__ == 1
	UARTprintf("[DZ:%2d], pos(%d,%d), x.y(%d.%d), wh(%d x %d)->(%d x %d), cut(%d,%d)\r\n", 
		val, Posi_x, Posi_y, pDZWIN->x,pDZWIN->y, old_w, old_h, pDZWIN->w, pDZWIN->h, cut_h, cut_v);
#endif
}

//--------------------------------------------------------------------------------------------------
static void DZOOM_SetPosition(WORD Posi_x, WORD Posi_y)
{
	WORD	Posi_x_step, Posi_y_step, src_w, src_h;
	PMISP_VIDEO_WINDOW pDZWIN = &stDZOOMCTRL.DZwin;
	PMISP_OUTVIDEO_SYNC	pOUTSYNC = GetOUT_MainSync();
	
	src_w = MIN(GetFC_MainFCsrcSizeH(), GetFC_MainFCdstSizeH());		// v.in > out, use v.out hsize(ffc on..)
	src_h = GetFC_MainFCsrcSizeV();

	Posi_x_step = (src_w - pDZWIN->w)*100/240;		Posi_y_step = (src_h - pDZWIN->h)*100/240;
	pDZWIN->x = Posi_x_step * Posi_x/100;			pDZWIN->y = Posi_y_step * Posi_y/100;
	SetMenuStatus(DZPOSI_MENU, MIN(Posi_x, 240));	SetMenuPosiY(DZPOSI_MENU, MIN(Posi_y, 240));

#if defined(__USE_FHDCROP_MODE__)	 // Crop Dzoom mode
	if ( fCropMode ) {SetFC_MainFCsrcPosiX((pDZWIN->x&0xFFFE)+move_in_LR*64);				SetFC_MainFCsrcPosiY((pDZWIN->y&0xFFFE)+move_in_UD*36);}
	else {SetFC_MainFCsrcPosiX(pDZWIN->x&0xFFFE);			SetFC_MainFCsrcPosiY(pDZWIN->y&0xFFFE);}		
#else // Noemal operation	
	SetFC_MainFCsrcPosiX(pDZWIN->x&0xFFFE);			SetFC_MainFCsrcPosiY(pDZWIN->y&0xFFFE);
#endif
	
	// overrun check
	MISP_RegWrite(MISP_LOCAL_ID, 0x09E, (pOUTSYNC->totVS - pDZWIN->y) | (1<<15));	// main overrun en & set offset

#if __MISP100_DBGPRT__ == 1
	UARTprintf("DEMO_SetZoomPosition : posi(%d,%d) -> Position:%d/%d(0x%x/0x%x) \n", 
					Posi_x, Posi_y, pDZWIN->x, pDZWIN->y, pDZWIN->x, pDZWIN->y);
#endif
}

#if defined (__USE_MDIN_i500__)
//--------------------------------------------------------------------------------------------------
static WORD GetSBOXoffy(BYTE frmt)
{
	switch(frmt)	{
		case MISP_VIDOUT_1280x720p60:
#if defined(__USE_59Hz_OUTPUT__)
		case MISP_VIDOUT_1280x720p59:
#endif
		case MISP_VIDOUT_1280x720p50:									return 12;	break;
		
		case MISP_VIDOUT_1920x1080i60:
#if defined(__USE_59Hz_OUTPUT__)
		case MISP_VIDOUT_1920x1080i59:
#endif
		case MISP_VIDOUT_1920x1080i50:									return 16;	break;
		
		case MISP_VIDOUT_1920x1080p60:
#if defined(__USE_59Hz_OUTPUT__)
		case MISP_VIDOUT_1920x1080p59:
#endif
		case MISP_VIDOUT_1920x1080p50:	case MISP_VIDOUT_1920x1080p30:
#if defined(__USE_59Hz_OUTPUT__)
		case MISP_VIDOUT_1920x1080p29:
#endif
		case MISP_VIDOUT_1920x1080p25:
		case MISP_VIDOUT_1920x1080p24:
		default:														return 28;	break;
	}
}
#endif	//__USE_MDIN_i500__

//--------------------------------------------------------------------------------------------------
static void PIP_SetCtrl(WORD val)		//0:off, 1:1/4, 2:1/9, 3:1/16, 4:1/25
{
	PMISP_VIDEO_WINDOW pDZWIN = &stDZOOMCTRL.DZwin,  pPIPWIN = &stDZOOMCTRL.PIPwin;
	WORD  posi_x = stDZOOMCTRL.PIPposix, posi_y = stDZOOMCTRL.PIPposiy;
	
	MISP_VIDEO_WINDOW	pip, dzwin;
	WORD  lx, ly, rx, ry, main_io_h_ratio, main_io_v_ratio, sbox_offy, sbox_offx, dzratio_h, dzratio_v;
	BYTE  frmt, mode, aux_set_en=0;
	
	if(val == OFF)	{	// pip off
		MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX0], OFF);
#if defined(__USE_4K60_INOUT__)
		MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX1], OFF);
		MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX2], OFF);
#endif
		SetFC_AuxFCdstPosiX(0);					SetFC_AuxFCdstPosiY(0);	// reset aux win position for dis
		SetOUT_AuxPipEN(OFF);					SetOUT_CVBSOutEN();	return;
	}
	
	if(GetOUT_AuxPipEN() != ON)					SetOUT_AuxPipEN(ON);			// pip on
	
	// calc pip size,position
	pip.w = GetOUT_MainSizeHA() / (val+1);		pip.h = GetOUT_MainSizeVA() / (val+1);	// main/2, /3, /4,,
	pip.x = (GetOUT_MainSizeHA() - pip.w) * posi_x/10;
	pip.y = (GetOUT_MainSizeVA() - pip.h) * posi_y/10;
	pip.x &= 0xFFFE;							pip.y &= 0xFFFE;
	
	// set aux frmt, mode
	mode = GetOUT_MainMode();					frmt = GetOUT_MainFrmt();
	if(mode != GetOUT_AuxMode())				{SetOUT_AuxMode(mode);	aux_set_en=1;}
	if(frmt != GetOUT_AuxFrmt())				{SetOUT_AuxFrmt(frmt);	aux_set_en=1;}
	
	// check pip info.(old vs new)
	if(pPIPWIN->w != pip.w)	aux_set_en=1;		if(pPIPWIN->h != pip.h)	aux_set_en=1;
	if(pPIPWIN->x != pip.x)	aux_set_en=1;		if(pPIPWIN->y != pip.y)	aux_set_en=1;
	
	// set aux sync & fc reg.
	if(aux_set_en)	{
		memcpy(pPIPWIN, &pip, sizeof(MISP_VIDEO_WINDOW));		// store pip info.
		SetOUT_AuxDispOnOffAuto(8);  //SetOUT_AuxDispEN(ON);
		SetFC_AuxFCdstSizeH(pip.w);				SetFC_AuxFCdstSizeV(pip.h);
		SetFC_AuxFCdstPosiX(pip.x);				SetFC_AuxFCdstPosiY(pip.y);
	}
	
	// calc dzoom window size/position in pip
	main_io_h_ratio = GetOUT_MainSizeHA() * 1000 / MIN(GetFC_MainFCsrcSizeH(), GetFC_MainFCdstSizeH());
	main_io_v_ratio = GetOUT_MainSizeVA() * 1000 / GetFC_MainFCsrcSizeV();
	dzratio_h = pDZWIN->w * 1000 / MIN(GetFC_MainFCsrcSizeH(), GetFC_MainFCdstSizeH());
	dzratio_v = pDZWIN->h * 1000 / GetFC_MainFCsrcSizeV();
	
//	dzwin.w = pDZWIN->w * main_io_h_ratio / (val+1) / 1000;	// src -> dst -> pip
//	dzwin.h = pDZWIN->h * main_io_v_ratio / (val+1) / 1000;
	dzwin.w = pip.w * dzratio_h / 1000;
	dzwin.h = pip.h * dzratio_v / 1000;
	dzwin.x = pDZWIN->x * main_io_h_ratio / (val+1) / 1000;
	dzwin.y = pDZWIN->y * main_io_v_ratio / (val+1) / 1000;
	
	if(!GetOUT_MainScanTypeProg())				{dzwin.h /=2;	dzwin.y /=2;}	//for interlace out
	
	// calc box osd position
#if defined (__USE_MDIN_i500__) 				// MDIN-i500
	sbox_offx = 3;								sbox_offy = GetSBOXoffy(frmt);
#else											// MDIN-i51X (i510, i540, i550)
	sbox_offx = 0;								sbox_offy = 0;
#endif
	lx = dzwin.x + pip.x;						lx -= (lx)?  sbox_offx:  0;

	if(!GetOUT_MainScanTypeProg())				{ly = dzwin.y + pip.y/2;	ly += sbox_offy;}
	else										{ly = dzwin.y + pip.y;		ly += sbox_offy;}

#if defined (__USE_MDIN_i500__)					// MDIN-i500
	rx = lx + dzwin.w;							ry = ly + dzwin.h;
#else											// MDIN-i51X (i510, i540, i550)
	rx = lx + dzwin.w - 1;						ry = ly + dzwin.h - 1;
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	rx = GetOUT_MainSizeHA()-1;
#endif

	MISPOSD_SetSBoxArea(&stSBOX[SBOX_INDEX0], lx, ly, rx, ry);
	MISPOSD_SetSBoxBorderColor(&stSBOX[SBOX_INDEX0], RGB(110,41,240)); 			//Blue
	
	if(GetPIP_BoxOSDOnOff())	MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX0], ON);			//sbox osd en/disable.
	
#if defined(__USE_4K60_INOUT__)
	if(GetPIP_BoxOSDOnOff())	{
		MISPOSD_SetSBoxArea(&stSBOX[SBOX_INDEX0], lx, ly, rx, ly+1);	// top
#if defined(__USE_4K60_MD1__)
		MISPOSD_SetSBoxArea(&stSBOX[SBOX_INDEX1], lx, ly, lx+1, ry);	// left
#else
		MISPOSD_SetSBoxArea(&stSBOX[SBOX_INDEX1], rx-1, ly, rx, ry);	// right
#endif
		MISPOSD_SetSBoxArea(&stSBOX[SBOX_INDEX2], lx, ry-1, rx, ry);	// bottom
		
		MISPOSD_SetSBoxBorderColor(&stSBOX[SBOX_INDEX1], RGB(110,41,240));	// blue
		MISPOSD_SetSBoxBorderColor(&stSBOX[SBOX_INDEX2], RGB(110,41,240));	// blue
		
		MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX1], ON);
		MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX2], ON);
	}
#endif

#if __MISP100_DBGPRT__ == 1
	UARTprintf("[PIP] RUN(1/%d)!!!, PIP(%d*%d, %d.%d), BOX(%d,%d - %d,%d)\r\n", 
					(val+1) * (val+1), pip.w, pip.h, pip.x, pip.y, lx, ly, rx, ry);
	UARTprintf("dzwin(%d*%d, %d.%d), io(h:%d, v:%d), dz(h:%d, v:%d) \r\n", 
				dzwin.w, dzwin.h, dzwin.x, dzwin.y, main_io_h_ratio, main_io_v_ratio, dzratio_h, dzratio_v);
#endif
}


//--------------------------------------------------------------------------------------------------
static void BLC_Process(PBLC_CTRL_PARAM pCTRL)
{
	if(pCTRL->ctrl & BLCCTRL_MODE_ON)	{
		stAECTRL.ctrl &= ~(AECTRL_BLC_MASK | AECTRL_WND_MASK);
		stAECTRL.ctrl |= (AECTRL_BLC_MODE_BLC | AECTRL_BLC_WIND_BLC | AECTRL_TRIGGER_ISP);	// ae blc mode on
		
		AECTRL_MoveBLCWIND(&stAECTRL, pCTRL->x[0], pCTRL->y[0], pCTRL->w[0], pCTRL->h[0]);	// set ae blc win param
		AECTRL_ViewBLCWIND((pCTRL->ctrl & BLCCTRL_WIND_ON)?  ON:  OFF);						// window on/off
	}
	else	{
		if(GetHLC_OnOff()==OFF)	{
			stAECTRL.ctrl &= ~(AECTRL_BLC_MASK | AECTRL_WND_MASK);
			stAECTRL.ctrl |= (AECTRL_BLC_MODE_OFF | AECTRL_BLC_WIND_OFF | AECTRL_TRIGGER_ISP);	// ae blc mode off
			
			AECTRL_ViewBLCWIND(OFF);															// window off
		}
	}
	BLC_HLC_Display();
}

//--------------------------------------------------------------------------------------------------
static void HLCCTRL_SetMaskOnOff(BOOL OnOff)
{
	MISP_RegField(MISP_LOCAL_ID, 0x0750, 0, 1, (OnOff)?  ON:  OFF);		// hlc_wind1_mask_enb
}

//--------------------------------------------------------------------------------------------------
static void HLCCTRL_SetMaskLevel(BYTE level)
{
	MISP_RegField(MISP_LOCAL_ID, 0x0751, 0, 8, 235 - level*7);				// HLC_thres_H
}

//--------------------------------------------------------------------------------------------------
static void HLC_Process(PHLC_CTRL_PARAM pCTRL)
{
	if(pCTRL->ctrl & HLCCTRL_MODE_ON)	{
		stAECTRL.ctrl &= ~(AECTRL_BLC_MASK | AECTRL_WND_MASK);
		stAECTRL.ctrl |= (AECTRL_BLC_MODE_HLC | AECTRL_BLC_WIND_HLC | AECTRL_TRIGGER_ISP);	// ae hlc mode on
		
		AECTRL_MoveBLCWIND(&stAECTRL, pCTRL->x[0], pCTRL->y[0], pCTRL->w[0], pCTRL->h[0]);	// set ae hlc win param
		AECTRL_ViewBLCWIND((pCTRL->ctrl & HLCCTRL_WIND_ON)?  ON:  OFF);						// window on/off
		HLCCTRL_SetMaskOnOff((pCTRL->ctrl & HLCCTRL_MASK_ON)?  ON:  OFF);					// mask on/off
		HLCCTRL_SetMaskLevel(pCTRL->mask_level[0]);
	}
	else	{
		if(GetBLC_OnOff()==OFF)	{
			stAECTRL.ctrl &= ~(AECTRL_BLC_MASK | AECTRL_WND_MASK);
			stAECTRL.ctrl |= (AECTRL_BLC_MODE_OFF | AECTRL_BLC_WIND_OFF | AECTRL_TRIGGER_ISP);	// ae hlc mode off
			
			AECTRL_ViewBLCWIND(OFF);															// window off
		}
		HLCCTRL_SetMaskOnOff(OFF);															// mask off
	}
	BLC_HLC_Display();
}

//api
//----------------------------------------------------------------------------------------
void MISC_Initialize(void)
{
	memset(&stDYNTCTRL, 0, sizeof(DYNT_CTRL_PARAM));
	memset(&stDZOOMCTRL, 0, sizeof(DZOOM_CTRL_PARAM));
	memset(&stBLCCTRL, 0, sizeof(BLC_CTRL_PARAM));
	memset(&stHLCCTRL, 0, sizeof(HLC_CTRL_PARAM));
	memset(&stVSCTRL, 0, sizeof(VS_CTRL_PARAM));
	
	Init_DYNT_PARAM(&stDYNTCTRL);
	Init_DZOOM_PARAM(&stDZOOMCTRL);
	Init_BLC_PARAM(&stBLCCTRL);
	Init_HLC_PARAM(&stHLCCTRL);
	Init_VS_PARAM(&stVSCTRL);
}

//--------------------------------------------------------------------------------------------------
void DZOOM_ProcessHandler(void)
{
	PDZOOM_CTRL_PARAM	pCTRL=&stDZOOMCTRL;
	
	if((pCTRL->ctrl & DZOOMCTRL_TRIGGER)==0)					return;		// every vdpulse(+3ms) trigger
	pCTRL->ctrl &= (~DZOOMCTRL_TRIGGER);
	
	if(pCTRL->ctrl & DZOOMCTRL_PROC_MASK)	{									// dzoom process
		switch(pCTRL->ctrl & DZOOMCTRL_ZOOM_MASK)	{ 							// dzoom ratio
			case  DZOOMCTRL_TELE_IN:
					if(pCTRL->DZratio < DZOOMCTRL_RATIO_MAX)	{
						if(pCTRL->DZratio + pCTRL->DZoffset < DZOOMCTRL_RATIO_MAX)	pCTRL->DZratio += pCTRL->DZoffset;
						else														pCTRL->DZratio = DZOOMCTRL_RATIO_MAX;
						DZOOM_SetRatio(pCTRL->DZratio);
					}
					pCTRL->ctrl &= (~DZOOMCTRL_TELE_IN);						// clear process flag
					break;
			case  DZOOMCTRL_WIDE_OUT:
					if(pCTRL->DZratio > 0)	{
						if(pCTRL->DZratio > pCTRL->DZoffset)	pCTRL->DZratio -= pCTRL->DZoffset;
						else								pCTRL->DZratio = 0;
						DZOOM_SetRatio(pCTRL->DZratio);
					}
					pCTRL->ctrl &= (~DZOOMCTRL_WIDE_OUT);						// clear process flag
					break;
			case  DZOOMCTRL_DIRECTRATIO:
					DZOOM_SetRatio(pCTRL->DZratio);
					pCTRL->ctrl &= (~DZOOMCTRL_DIRECTRATIO);					// clear process flag
					break;
			case  DZOOMCTRL_TARGETRATIO:
					if(pCTRL->DZtarget > pCTRL->DZratio)	{					// dzoom tele_in
						if(pCTRL->DZtarget > pCTRL->DZratio + pCTRL->DZoffset)	pCTRL->DZratio += pCTRL->DZoffset;
						else													pCTRL->DZratio = pCTRL->DZtarget;
						DZOOM_SetRatio(pCTRL->DZratio);
					}
					else if(pCTRL->DZtarget < pCTRL->DZratio)	{				// dzoom wide_out
						if(pCTRL->DZtarget + pCTRL->DZoffset < pCTRL->DZratio)	pCTRL->DZratio -= pCTRL->DZoffset;
						else													pCTRL->DZratio = pCTRL->DZtarget;
						DZOOM_SetRatio(pCTRL->DZratio);
					}
					else	pCTRL->ctrl &= (~DZOOMCTRL_TARGETRATIO);			// clear process flag
					break;
		}
		
		if(pCTRL->ctrl & DZOOMCTRL_POSI) {										// dzoom position
			DZOOM_SetPosition(pCTRL->DZposix, pCTRL->DZposiy);
			pCTRL->ctrl &= (~DZOOMCTRL_POSI);					// clear process flag
		}
		
//		pCTRL->ctrl &= (~DZOOMCTRL_PROC_MASK); 									// clear process flag
#if !defined(__USE_4K60_MD2__)									// skip 4k60 mdin-2
		SetMD_OnOff(GetMD_OnOff());	// for md+dzoom
#endif
	}
	
	
#if defined(__USE_CMOS_AUX_SYNC_OUT__) && (defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__) || defined(__USE_CMOS_IMX334__)) || \
	defined(__MDIN_i550_USB_V100__)				// skip pip process in 4k sensor with i5xx org chip, i550_usb
#else
	if(pCTRL->ctrl & PIPCTRL_PROC_MASK)	{										// pip process
		switch(pCTRL->ctrl & PIPCTRL_PROC_MASK)	{
			case  PIPCTRL_PROC_EN: 	PIP_SetCtrl((pCTRL->PIPonoff)?  pCTRL->PIPsize+1:  OFF);	break;
			case  PIPCTRL_PROC_DIS: 	PIP_SetCtrl(OFF);	break;
		}
		
		pCTRL->ctrl &= (~PIPCTRL_PROC_MASK);
	}
#endif
}


//--------------------------------------------------------------------------------------------------
void DYNT_ProcessHandler(PWB_CTRL_PARAM pWBCTRL)												// 0:color, 1:b&w
{
	BOOL  IsNT=0, IsNT_old = (stDYNTCTRL.ctrl & DYNTCTRL_IS_NIGHT)?  1:  0;	// 0(day), 1(night)
	BOOL  BWen, BWen_old = (stDYNTCTRL.ctrl & DYNTCTRL_BW_EN)?  1:  0;		// 0(color), 1(b/w)
	BOOL  burst_en = (stDYNTCTRL.ctrl & DYNTCTRL_COLOR_BURST_EN)?  1:  0;	// 0(burst off), 1(burst on)
	BOOL  burst_en_old = GetOUT_CVBSBurst();

	switch(stDYNTCTRL.ctrl & DYNTCTRL_MODE_MASK)	{
		case DYNTCTRL_MODE_AUTO:	IsNT = DYNT_GetAutoModeNight(pWBCTRL)? 1: 0;	break;		// check agc level
		case DYNTCTRL_MODE_DAY:		IsNT = 0;								break;
		case DYNTCTRL_MODE_NIGHT:	IsNT = 1;								break;
		case DYNTCTRL_MODE_EXT:		IsNT = DYNT_GetExtModeNight()? 1: 0;	break;		// check ext. cds sensor
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD2__)							// 4k60 slave only
		case DYNTCTRL_MODE_SLAVE:	IsNT = SPIsAPP_GetMasterDYNTStatus();	break;		// 0(day), 1(night)
#endif
	}
	
	if(IsNT != IsNT_old)	{														// day <-> night
		if(IsNT) { // mimitest
			stDYNTCTRL.ctrl |= DYNTCTRL_IS_NIGHT;
			DEMO_SetAWBHold(TRUE);
		} else {
			stDYNTCTRL.ctrl &= (~DYNTCTRL_IS_NIGHT);
			DEMO_SetAWBHold(FALSE);
		}
		
		SetDYNT_ICR((IsNT)?  ON:  OFF);												// icr : night(on), day(off)
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
		SPImAPP_SetSlaveDYNTStatus(IsNT);
#endif
	}
	
	DYNT_ICRDrvStopCounter();														// icr driver stop counter : icr(on) -> 100ms -> icr(off)
	
	if((IsNT==1) && (stDYNTCTRL.ctrl & DYNTCTRL_NT_COLOR_EN))	BWen = 0;			// 0(color mode), 1(b/w mode)
	else														BWen = IsNT;
	
	if(BWen != BWen_old)	{														// color mode <-> b/w mode
		if(BWen)		stDYNTCTRL.ctrl |= DYNTCTRL_BW_EN;
		else			stDYNTCTRL.ctrl &= (~DYNTCTRL_BW_EN);
		
		SetIN_MainCSCgray((BWen)?  ON:  OFF);										// main input csc color en/dis
		// SetOUT_CVBSColor((BWen)?  OFF:  ON);										// cvbs out color en/dis
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
		SPImAPP_SetSlaveDYNTNightColor((BWen)?  OFF:  ON);
#endif
#if defined(__MDIN_i550_USB_V100__)
		SetIN_AuxCSCgray((BWen)?  ON:  OFF);										// aux input csc color en/dis
#endif
	}

	// if(BWen==0)		burst_en = 1;													// color mode -> burst always on
	burst_en = 1;													// color mode -> burst always on
	if(burst_en != burst_en_old)	{
		SetOUT_CVBSBurst(burst_en);						// cvbs burst en/dis
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
		SPImAPP_SetSlaveDYNTColorBurst(burst_en);
#endif
	}

}

//get/set func.

//--------------------------------------------------------------------------------------------------
void SetDYNT_Mode(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	stDYNTCTRL.ctrl &= (~DYNTCTRL_MODE_MASK);					// clear flag
	
	switch(nID)	{
		case 0:		stDYNTCTRL.ctrl |= DYNTCTRL_MODE_AUTO;		break;
		case 1:		stDYNTCTRL.ctrl |= DYNTCTRL_MODE_DAY;		break;
		case 2:		stDYNTCTRL.ctrl |= DYNTCTRL_MODE_NIGHT;		break;
		case 3:		stDYNTCTRL.ctrl |= DYNTCTRL_MODE_EXT;		break;
		case 4:		stDYNTCTRL.ctrl |= DYNTCTRL_MODE_SLAVE;		break;
	}
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_DYNT_MODE, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
WORD GetDYNT_Mode(void)
{
	return		(stDYNTCTRL.ctrl & DYNTCTRL_MODE_MASK);
}

//--------------------------------------------------------------------------------------------------
BOOL GetDYNT_Status(void)	// 0:day, 1:night
{
	return		(stDYNTCTRL.ctrl & DYNTCTRL_IS_NIGHT)?  1:  0;
}

//--------------------------------------------------------------------------------------------------
void SetDYNT_AutoDly(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	stDYNTCTRL.dw_time = nID;					// 0~60 sec
	stDYNTCTRL.dw_cnt = nID * GET_FPS;			// 0~60 sec * (25hz or 30hz)
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_DYNT_DELAY, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
WORD GetDYNT_AGCD2N(void)
{
	return		stDYNTCTRL.agc_d2n;						// 1~255
}

//--------------------------------------------------------------------------------------------------
WORD GetDYNT_AGCN2D(void)
{
	return		stDYNTCTRL.agc_n2d;						// 0~254
}

//--------------------------------------------------------------------------------------------------
BYTE GetDYNT_AutoDly(void)
{
	return		stDYNTCTRL.dw_time;						// 0~60 sec
}

//--------------------------------------------------------------------------------------------------
void SetDYNT_AGCD2N(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	stDYNTCTRL.agc_d2n = nID;												// range : 1 ~ 255
	
	if(stDYNTCTRL.agc_d2n <= stDYNTCTRL.agc_n2d)	{						// d2n <= n2d ?  n2d = d2n-1
		SetDYNT_AGCN2D(nID-1);	SetMenuStatus(DANT_AGCN2D_MENU, nID-1);
	}
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_DYNT_D2N, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void SetDYNT_AGCN2D(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	stDYNTCTRL.agc_n2d = nID;											// range : 0 ~ 254
	
	if(stDYNTCTRL.agc_n2d >= stDYNTCTRL.agc_d2n)	{					// n2d >= d2n ?  d2n = n2d+1
		SetDYNT_AGCD2N(nID+1);	SetMenuStatus(DANT_AGCD2N_MENU, nID+1);
	}
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_DYNT_N2D, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void SetDYNT_NTColor(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	stDYNTCTRL.ctrl &= (~DYNTCTRL_NT_COLOR_EN);
	stDYNTCTRL.ctrl |= (nID)?  DYNTCTRL_NT_COLOR_EN:  DYNTCTRL_NT_COLOR_DIS;
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_DYNT_NTCOLOR, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void SetDYNT_CVBSBurst(WORD nID)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
	stDYNTCTRL.ctrl &= (~DYNTCTRL_COLOR_BURST_EN);
	stDYNTCTRL.ctrl |= (nID)?  DYNTCTRL_COLOR_BURST_EN:  DYNTCTRL_COLOR_BURST_DIS;
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)nID;
	EEPROM_MultiWrite(EEPROM_DYNT_CBURST, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
BOOL GetDYNT_NTColor(void)
{
	return		(stDYNTCTRL.ctrl & DYNTCTRL_NT_COLOR_EN)?  1:  0;
}

//--------------------------------------------------------------------------------------------------
BOOL GetDYNT_CVBSBurst(void)
{
	return		(stDYNTCTRL.ctrl & DYNTCTRL_COLOR_BURST_EN)?  1:  0;
}

//--------------------------------------------------------------------------------------------------
void SetDYNT_ICR(BOOL OnOff)
{
	DYNT_SetICRDrvStopCount(0);	// OLPF-cnt clear
	
	
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
#if defined(__USE_OLPF_YOPTIC__)
	if(OnOff)		{OLPF_DY(LOW);		OLPF_NT(HIGH);}		// night : ir-cut filter removal 'on'
	else			{OLPF_DY(HIGH);		OLPF_NT(LOW);}		// day : ir-cut filter removal 'off'
#elif defined(__USE_YT3010_2MOTOR__)
	if(OnOff)		{OLPF_DY(LOW);		OLPF_NT(HIGH);}		// night : ir-cut filter removal 'on'
	else			{OLPF_DY(HIGH);		OLPF_NT(LOW);}		// day : ir-cut filter removal 'off'
#else		// tamron,union,zmax,sunny,canon,,
	if(OnOff) 		{OLPF_DY(HIGH);		OLPF_NT(LOW);}		// night : ir-cut filter removal 'on'
	else			{OLPF_DY(LOW);		OLPF_NT(HIGH);}		// day : ir-cut filter removal 'off'
#endif

#else	
	
#if defined(__USE_CMOS_EXT1__)
	if(OnOff)		{OLPF_DY(LOW);		OLPF_NT(HIGH);}		// night : ir-cut filter removal 'on'
	else			{OLPF_DY(HIGH);		OLPF_NT(LOW);}		// day : ir-cut filter removal 'off'
#elif defined(__PANORAMA_SENSOR_SETUP__)
	if(OnOff)		{OLPF_DY(HIGH);		OLPF_NT(LOW);}		// night : ir-cut filter removal 'on'
	else			{OLPF_DY(LOW);		OLPF_NT(HIGH);}		// day : ir-cut filter removal 'off'
#elif defined(__USE_MANU_WW_8mmIR__) || defined(__USE_MANU_WW_4KUHD__)
	if(OnOff)		{OLPF_DY(LOW);		OLPF_NT(HIGH);}		// night : ir-cut filter removal 'on'
	else			{OLPF_DY(HIGH);		OLPF_NT(LOW);}		// day : ir-cut filter removal 'off'
#endif

#endif
	//if(OnOff)		UARTprintf("[DN] NIGHT!!!\r\n");
	//else			UARTprintf("[DN] DAY!!!\r\n");
}

//--------------------------------------------------------------------------------------------------
void ISPM SetDZOOM_Tick(void)
{
	stDZOOMCTRL.ctrl |= DZOOMCTRL_TRIGGER;
}

//--------------------------------------------------------------------------------------------------
void SetDZOOM_DriveTele(void)
{
	if(stDZOOMCTRL.ctrl & DZOOMCTRL_ZOOM_MASK)		return;	// prev. dzoom process not excuted..
	
	stDZOOMCTRL.ctrl |= DZOOMCTRL_TELE_IN;
	
	SetDZOOM_RepeatKeyEnable();
}

//--------------------------------------------------------------------------------------------------
void SetDZOOM_DriveWide(void)
{
	if(stDZOOMCTRL.ctrl & DZOOMCTRL_ZOOM_MASK)		return;	// prev. dzoom process not excuted..
	
	stDZOOMCTRL.ctrl |= DZOOMCTRL_WIDE_OUT;
	
	SetDZOOM_RepeatKeyEnable();
}

//--------------------------------------------------------------------------------------------------
void SetDZOOM_DriveTeleVar(BYTE speed)
{
	if(stDZOOMCTRL.ctrl & DZOOMCTRL_ZOOM_MASK)		return;	// prev. dzoom process not excuted..
	stDZOOMCTRL.ctrl |= DZOOMCTRL_TELE_IN;
//	stDZOOMCTRL.DZoffset = speed+1;								// 0:step(1) ~ 7:step(8)
	stDZOOMCTRL.DZoffset = (speed<=3)? 1: (speed-3);				// 0~3:step(1), 4:step(1) ~ 7:step(4)
	
	SetDZOOM_RepeatKeyEnable();
}

//--------------------------------------------------------------------------------------------------
void SetDZOOM_DriveWideVar(BYTE speed)
{
	if(stDZOOMCTRL.ctrl & DZOOMCTRL_ZOOM_MASK)		return;	// prev. dzoom process not excuted..
	stDZOOMCTRL.ctrl |= DZOOMCTRL_WIDE_OUT;
//	stDZOOMCTRL.DZoffset = speed+1;								// 0:step(1) ~ 7:step(8)
	stDZOOMCTRL.DZoffset = (speed<=3)? 1: (speed-3);				// 0~3:step(1), 4:step(1) ~ 7:step(4)
	
	SetDZOOM_RepeatKeyEnable();
}

//--------------------------------------------------------------------------------------------------
void SetDZOOM_DirectRatio(WORD val)
{
	if(stDZOOMCTRL.ctrl & DZOOMCTRL_ZOOM_MASK)		return;	// prev. dzoom process not excuted..
	stDZOOMCTRL.DZratio = MIN(DZOOMCTRL_RATIO_MAX, val);
	stDZOOMCTRL.ctrl &= (~DZOOMCTRL_ZOOM_MASK);
	stDZOOMCTRL.ctrl |= DZOOMCTRL_DIRECTRATIO;
}

//--------------------------------------------------------------------------------------------------
void SetDZOOM_TargetRatio(WORD val)
{
	stDZOOMCTRL.DZtarget = MIN(DZOOMCTRL_RATIO_MAX, val);
	stDZOOMCTRL.ctrl &= (~DZOOMCTRL_ZOOM_MASK);
	stDZOOMCTRL.ctrl |= DZOOMCTRL_TARGETRATIO;
}

//--------------------------------------------------------------------------------------------------
void SetDZOOM_RepeatKeyEnable(void)
{
#if !defined(__USE_MDIN_i5XX_REV__) && (defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__) || defined(__USE_CMOS_IMX334__))	// i5xx org chip
	SetMsgCommandRepeatKey(MSG_DZ2X);	// repeat 2/vs ( for 4K 20hz )
#else
	SetMsgCommandRepeatKey(MSG_DZ1X);	// repeat 1/vs ( normal )
#endif
}

//--------------------------------------------------------------------------------------------------
void SetDZOOM_Status(BOOL OnOff)
{
	if(OnOff)		stDZOOMCTRL.ctrl |= DZOOMCTRL_DZ_ON;		// dzoom status on flag
	else			stDZOOMCTRL.ctrl &= (~DZOOMCTRL_DZ_ON);		// dzoom status off flag
	
	SetFC_MainLoadSizeEN(OnOff);			// 'size_load_reg' enable when zoom on
}

//--------------------------------------------------------------------------------------------------
void SetDZOOM_Window(WORD wh, WORD wv)
{
	stDZOOMCTRL.DZwin.w = wh;
	stDZOOMCTRL.DZwin.h = wv;
	stDZOOMCTRL.DZwin.x = 0;
	stDZOOMCTRL.DZwin.y = 0;
}

//--------------------------------------------------------------------------------------------------
BOOL GetDZOOM_OnOff(void)
{
	if(stDZOOMCTRL.ctrl & DZOOMCTRL_DZ_ON)			return ON;
	else											return OFF;
}

//--------------------------------------------------------------------------------------------------
WORD GetDZOOM_Ratio(void)
{
	return		stDZOOMCTRL.DZratio;
}

//--------------------------------------------------------------------------------------------------
WORD ConvDZOOM_MenuRatio(WORD ratio)
{
	WORD src_ratio = GetIN_MainSrcSizeH()*10 / GetIN_MainSrcSizeV();
	WORD src_w = MIN(GetFC_MainFCsrcSizeH(), GetFC_MainFCdstSizeH());
	WORD cut_h;
	
	if(src_ratio > 15)		cut_h = src_w*32 / 1920;	// 16:9
	else					cut_h = src_w*32 / 2048;	// 4:3
	
	cut_h += (cut_h%2);
	
	return		(src_w * 100) / (src_w - (cut_h * ratio));	// 100:x1.00 ~ 1200:x12.0
}

//--------------------------------------------------------------------------------------------------
BOOL IsDZOOM_WideEND(void)
{
	return		(stDZOOMCTRL.DZratio <= stDZOOMCTRL.DZoffset)?  1:  0;
}

//--------------------------------------------------------------------------------------------------
BOOL GetPIP_BoxOSDOnOff(void)
{
	return		((stDZOOMCTRL.ctrl & PIPCTRL_BOXOSD_ON)?  ON:  OFF);
}

//--------------------------------------------------------------------------------------------------
void SetPIP_BoxOSDOnOff(BOOL OnOff)
{
	if(OnOff)		stDZOOMCTRL.ctrl |= PIPCTRL_BOXOSD_ON;		// on
	else			stDZOOMCTRL.ctrl &= (~PIPCTRL_BOXOSD_ON);	// off
}

//--------------------------------------------------------------------------------------------------
void SetPIP_Process(BOOL OnOff)
{
	stDZOOMCTRL.ctrl &= (~PIPCTRL_PROC_MASK);
	if(OnOff) 	stDZOOMCTRL.ctrl |= PIPCTRL_PROC_EN;
	else 		stDZOOMCTRL.ctrl |= PIPCTRL_PROC_DIS;
}

//--------------------------------------------------------------------------------------------------
void SetPIP_DispOnOff(BOOL OnOff)
{
	stDZOOMCTRL.PIPonoff = (OnOff)?  ON:  OFF;
	
	stDZOOMCTRL.ctrl &= (~PIPCTRL_PROC_MASK);
	stDZOOMCTRL.ctrl |= PIPCTRL_PROC_EN;
}

//--------------------------------------------------------------------------------------------------
BYTE GetPIP_DispOnOff(void)
{
	return  (stDZOOMCTRL.PIPonoff)?  ON:  OFF;
}

//--------------------------------------------------------------------------------------------------
void SetPIP_Size(WORD size)		// 0:1/4,,3:1/25
{
	stDZOOMCTRL.PIPsize = size;
	
	stDZOOMCTRL.ctrl &= (~PIPCTRL_PROC_MASK);
	stDZOOMCTRL.ctrl |= PIPCTRL_PROC_EN;
}

//--------------------------------------------------------------------------------------------------
WORD GetPIP_Size(void)		// 0:1/4,,3:1/25
{
	return stDZOOMCTRL.PIPsize ;
}

//--------------------------------------------------------------------------------------------------
void SetPIP_Position(WORD x, WORD y)
{
	stDZOOMCTRL.PIPposix = x;
	stDZOOMCTRL.PIPposiy = y;
	
	stDZOOMCTRL.ctrl &= (~PIPCTRL_PROC_MASK);
	stDZOOMCTRL.ctrl |= PIPCTRL_PROC_EN;
}

//--------------------------------------------------------------------------------------------------
void GetPIP_Position(PWORD px, PWORD py)
{
	*px = stDZOOMCTRL.PIPposix;
	*py = stDZOOMCTRL.PIPposiy;
}

//--------------------------------------------------------------------------------------------------
void SetDZOOM_Position(WORD x, WORD y)
{
//#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
//	BYTE buff[3];
//#endif
	
	stDZOOMCTRL.DZposix = x;
	stDZOOMCTRL.DZposiy = y;
	stDZOOMCTRL.ctrl |= DZOOMCTRL_POSI;
	
//#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
//	buff[0] = EEPROM_SAVE_DONE;
//	buff[1] = (BYTE)x; 					buff[2] = (BYTE)y;
//	EEPROM_MultiWrite(EEPROM_DZOOM_DZPOSI, (PBYTE)&buff[0], sizeof(buff));
//#endif
}

//--------------------------------------------------------------------------------------------------
void GetDZOOM_Position(PWORD px, PWORD py)
{
	*px = stDZOOMCTRL.DZposix;
	*py = stDZOOMCTRL.DZposiy;
}

//--------------------------------------------------------------------------------------------------
WORD GetDZOOM_WinPosiX(void)
{
	return stDZOOMCTRL.DZwin.x;
}

//--------------------------------------------------------------------------------------------------
WORD GetDZOOM_WinPosiY(void)
{
	return stDZOOMCTRL.DZwin.y;
}

//--------------------------------------------------------------------------------------------------
void SetDZOOM_FactoryDefaultMenu(void)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	WORD i;
	BYTE buff[14];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)
	SPImAPP_SetSlaveDZOOMDefault();
#endif
	
	DZOOM_SetRatio(0); 						// reset dzoom ratio
	
	// erase eeprom
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	for(i=0;i<sizeof(buff);i++)	buff[i] = 0xff;
	
	EEPROM_MultiWrite(EEPROM_SPECIAL_DZOOM, (PBYTE)&buff, 2); 				// erase special-dzoom menu (2byte)
	EEPROM_MultiWrite(EEPROM_DZOOM_MENU, (PBYTE)&buff, sizeof(buff)); 		// erase dzoom sub menu (14byte)
#endif
	
	// set dzoom parameter to factory default
	memset(&stDZOOMCTRL, 0, sizeof(DZOOM_CTRL_PARAM));
	Init_DZOOM_PARAM(&stDZOOMCTRL);
	
	// set pip window
	SetPIP_Process(ON);
	SetPIP_BoxOSDOnOff(ON);
	
	// set md window
	SetMD_OnOff(GetMD_OnOff());	// for md+dzoom
}

//--------------------------------------------------------------------------------------------------
void SetDZOOM_WriteEEPROM(void)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[5];
	
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)stDZOOMCTRL.DZratio;
	buff[2] = EEPROM_SAVE_DONE; 		buff[3] = (BYTE)stDZOOMCTRL.DZposix;
	buff[4] = (BYTE)stDZOOMCTRL.DZposiy;
	EEPROM_MultiWrite(EEPROM_DZOOM_DZRATIO, (PBYTE)&buff[0], sizeof(buff));
#endif
}

// blc api
//--------------------------------------------------------------------------------------------------
void SetBLC_Position(WORD x, WORD y)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[3];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveBLCPosi(x, y);
#endif
	
	stBLCCTRL.x[0] = x;					stBLCCTRL.y[0] = y;
	
	BLC_Process(&stBLCCTRL);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = (BYTE)x; 					buff[2] = (BYTE)y;
	EEPROM_MultiWrite(EEPROM_BLC_WINPOSI, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void SetBLC_Size(WORD width, WORD height)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[3];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveBLCSize(width, height);
#endif
	
	stBLCCTRL.w[0] = width;				stBLCCTRL.h[0] = height;
	
	BLC_Process(&stBLCCTRL);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = (BYTE)width; 				buff[2] = (BYTE)height;
	EEPROM_MultiWrite(EEPROM_BLC_WINSIZE, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void SetBLC_WinOnOff(WORD OnOff)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveBLCDisp(OnOff);
#endif
	
	stBLCCTRL.ctrl &= (~BLCCTRL_WIND_ON);
	stBLCCTRL.ctrl |= (OnOff)?  BLCCTRL_WIND_ON:  BLCCTRL_WIND_OFF;
	
	BLC_Process(&stBLCCTRL);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)OnOff;
	EEPROM_MultiWrite(EEPROM_BLC_WINDISP, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void SetBLC_OnOff(WORD OnOff)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveBLCOnOff(OnOff);
#endif
	
	stBLCCTRL.ctrl &= (~BLCCTRL_MODE_ON);
	stBLCCTRL.ctrl |= (OnOff)?  BLCCTRL_MODE_ON:  BLCCTRL_MODE_OFF;
	
	BLC_Process(&stBLCCTRL);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (OnOff)?  1:  0;
	EEPROM_MultiWrite(EEPROM_BLC_MODE, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void SetBLC_Default(void)
{
	SetBLC_Size(stBLCCTRL.w[1], stBLCCTRL.h[1]); 		// restore backup data
	SetBLC_Position(stBLCCTRL.x[1], stBLCCTRL.y[1]);
	
	SetBLC_WinOnOff(ON); 								// window display on
}

//--------------------------------------------------------------------------------------------------
void GetBLC_Position(PWORD px, PWORD py)
{
	*px = stBLCCTRL.x[0];					*py = stBLCCTRL.y[0];
}

//--------------------------------------------------------------------------------------------------
void GetBLC_Size(PWORD pwidth, PWORD pheight)
{
	*pwidth = stBLCCTRL.w[0];				*pheight = stBLCCTRL.h[0];
}

//--------------------------------------------------------------------------------------------------
void GetBLC_SizeLimit(PWORD pw_max, PWORD ph_max)
{
	*pw_max = stBLCCTRL.w_max;				*ph_max = stBLCCTRL.h_max;
}

//--------------------------------------------------------------------------------------------------
BOOL GetBLC_WinOnOff(void)
{
	return		(stBLCCTRL.ctrl & BLCCTRL_WIND_ON)?  ON:  OFF;
}

//--------------------------------------------------------------------------------------------------
BOOL GetBLC_OnOff(void)
{
	return		(stBLCCTRL.ctrl & BLCCTRL_MODE_ON)?  ON:  OFF;
}

// blc menu app
//--------------------------------------------------------------------------------------------
void SetBLC_OnOffMenu(WORD nID)
{
	WORD x, y, w, h;
	
	SetBLC_OnOff(nID?  ON:  OFF);
	
	if(nID)	{
		GetBLC_Position(&x, &y);				GetBLC_Size(&w, &h);
		SetMenuStatus(BLC_POSI_MENU, x);		SetMenuPosiY(BLC_POSI_MENU, y);
		SetMenuStatus(BLC_SIZE_MENU, w);		SetMenuPosiY(BLC_SIZE_MENU, h);
		SetMenuStatus(BLC_WIN_MENU, GetBLC_WinOnOff());
		SetMenuStatus(BLC_ONOFF_MENU, GetBLC_OnOff());
	}
}

//--------------------------------------------------------------------------------------------
void SetBLC_PosiMenu(WORD nID)
{
	WORD x = GetMenuStatus(BLC_POSI_MENU),	 y = GetMenuPosiY(BLC_POSI_MENU); 
	WORD w = GetMenuStatus(BLC_SIZE_MENU),	 h = GetMenuPosiY(BLC_SIZE_MENU); 
	WORD w_max, h_max;
	
	GetBLC_SizeLimit(&w_max, &h_max);
	
	// check position x/y limit
	if(x > 63)				{x = 63;			SetMenuStatus(BLC_POSI_MENU, x);}		// position_x register limit
	if(y > 63)				{y = 63;			SetMenuPosiY(BLC_POSI_MENU, y);}		// position_y register limit
	
	// check 'size + position' limit
	if(x + w > w_max)		{x = w_max - w;		SetMenuStatus(BLC_POSI_MENU, x);}
	if(y + h > h_max)		{y = h_max - h;		SetMenuPosiY(BLC_POSI_MENU, y);}
	
	SetBLC_Position(x, y);
}

//--------------------------------------------------------------------------------------------------
void SetBLC_SizeMenu(WORD nID)
{
	WORD x = GetMenuStatus(BLC_POSI_MENU),	 y = GetMenuPosiY(BLC_POSI_MENU); 
	WORD w = GetMenuStatus(BLC_SIZE_MENU),	 h = GetMenuPosiY(BLC_SIZE_MENU); 
	WORD w_max, h_max;
	
	GetBLC_SizeLimit(&w_max, &h_max);
	
	// check size limit
	if(w > 63)				{w = 63;			SetMenuStatus(BLC_SIZE_MENU, w);}		// size_w register limit
	if(h > 63)				{h = 63;			SetMenuPosiY(BLC_SIZE_MENU, h);}		// size_h register limit
	if(w <  1)				{w =  1;			SetMenuStatus(BLC_SIZE_MENU, w);}		// size_w min
	if(h <  1)				{h =  1;			SetMenuPosiY(BLC_SIZE_MENU, h);}		// size_h min
	
	// check 'size + position' limit
	if(x + w > w_max)		{w = w_max - x;		SetMenuStatus(BLC_SIZE_MENU, w);}
	if(y + h > h_max)		{h = h_max - y;		SetMenuPosiY(BLC_SIZE_MENU, h);}
	
	SetBLC_Size(w, h);
}

//------------------------------------------------------------------------------------------------
void SetBLC_FactoryDefaultMenu(BOOL OnOff)
{
	WORD x, y, w, h;
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveBLCDefault(OnOff);
#endif
	
	if(OnOff)	{
		SetBLC_Default();						SetBLC_OnOff(ON);
		
		GetBLC_Position(&x, &y);				GetBLC_Size(&w, &h);
		SetMenuStatus(BLC_POSI_MENU, x);		SetMenuPosiY(BLC_POSI_MENU, y);
		SetMenuStatus(BLC_SIZE_MENU, w);		SetMenuPosiY(BLC_SIZE_MENU, h);
		SetMenuStatus(BLC_WIN_MENU, GetBLC_WinOnOff());
		SetMenuStatus(BLC_ONOFF_MENU, GetBLC_OnOff());
	}
}

// hlc api
//--------------------------------------------------------------------------------------------------
void SetHLC_Position(WORD x, WORD y)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[3];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveHLCPosi(x, y);
#endif
	
	stHLCCTRL.x[0] = x;					stHLCCTRL.y[0] = y;
	
	HLC_Process(&stHLCCTRL);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = (BYTE)x; 					buff[2] = (BYTE)y;
	EEPROM_MultiWrite(EEPROM_HLC_WINPOSI, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void SetHLC_Size(WORD width, WORD height)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[3];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveHLCSize(width, height);
#endif
	
	stHLCCTRL.w[0] = width;				stHLCCTRL.h[0] = height;
	
	HLC_Process(&stHLCCTRL);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = (BYTE)width; 				buff[2] = (BYTE)height;
	EEPROM_MultiWrite(EEPROM_HLC_WINSIZE, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void SetHLC_WinOnOff(WORD OnOff)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveHLCDispOnOff(OnOff);
#endif
	
	stHLCCTRL.ctrl &= (~HLCCTRL_WIND_ON);
	stHLCCTRL.ctrl |= (OnOff)?  HLCCTRL_WIND_ON:  HLCCTRL_WIND_OFF;
	
	HLC_Process(&stHLCCTRL);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (OnOff)?  1:  0;
	EEPROM_MultiWrite(EEPROM_HLC_WINDISP, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void SetHLC_MaskOnOff(WORD OnOff)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveHLCBlackMask(OnOff);
#endif
	
	stHLCCTRL.ctrl &= (~HLCCTRL_MASK_ON);
	stHLCCTRL.ctrl |= (OnOff)?  HLCCTRL_MASK_ON:  HLCCTRL_MASK_OFF;
	
	HLC_Process(&stHLCCTRL);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)OnOff;
	EEPROM_MultiWrite(EEPROM_HLC_MASKDISP, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void SetHLC_MaskLevel(WORD level)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SetHLC_MaskLevel(level);
#endif
	
	stHLCCTRL.mask_level[0] = MIN(20, (BYTE)level); 			// 0 ~ 20(menu)
	
	HLC_Process(&stHLCCTRL);
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	buff[0] = EEPROM_SAVE_DONE; 		buff[1] = (BYTE)level;
	EEPROM_MultiWrite(EEPROM_HLC_MASKLEVEL, (PBYTE)&buff[0], sizeof(buff));
#endif
}

//--------------------------------------------------------------------------------------------------
void SetHLC_OnOff(WORD OnOff)
{
	stHLCCTRL.ctrl &= (~HLCCTRL_MODE_ON);
	stHLCCTRL.ctrl |= (OnOff)?  HLCCTRL_MODE_ON:  HLCCTRL_MODE_OFF;
	
	HLC_Process(&stHLCCTRL);
}

//--------------------------------------------------------------------------------------------------
void SetHLC_Default(void)
{
	SetHLC_Size(stHLCCTRL.w[1], stHLCCTRL.h[1]); 		// restore backup data
	SetHLC_Position(stHLCCTRL.x[1], stHLCCTRL.y[1]);
	SetHLC_MaskLevel(stHLCCTRL.mask_level[1]);
	
	SetHLC_WinOnOff(ON); 								// window display on
	SetHLC_MaskOnOff(ON); 								// mask display on
	
	HLC_Process(&stHLCCTRL);
}

//--------------------------------------------------------------------------------------------------
void GetHLC_Position(PWORD px, PWORD py)
{
	*px = stHLCCTRL.x[0];					*py = stHLCCTRL.y[0];
}

//--------------------------------------------------------------------------------------------------
void GetHLC_Size(PWORD pwidth, PWORD pheight)
{
	*pwidth = stHLCCTRL.w[0];				*pheight = stHLCCTRL.h[0];
}

//--------------------------------------------------------------------------------------------------
void GetHLC_SizeLimit(PWORD pw_max, PWORD ph_max)
{
	*pw_max = stHLCCTRL.w_max;				*ph_max = stHLCCTRL.h_max;
}

//--------------------------------------------------------------------------------------------------
BOOL GetHLC_WinOnOff(void)
{
	return		(stHLCCTRL.ctrl & HLCCTRL_WIND_ON)?  ON:  OFF;
}

//--------------------------------------------------------------------------------------------------
BYTE GetHLC_MaskLevel(void)
{
	return		stHLCCTRL.mask_level[0];
}

//--------------------------------------------------------------------------------------------------
BOOL GetHLC_MaskOnOff(void)
{
	return		(stHLCCTRL.ctrl & HLCCTRL_MASK_ON)?  ON:  OFF;
}

//--------------------------------------------------------------------------------------------------
BOOL GetHLC_OnOff(void)
{
	return		(stHLCCTRL.ctrl & HLCCTRL_MODE_ON)?  ON:  OFF;
}

// hlc menu app
//--------------------------------------------------------------------------------------------
void SetHLC_OnOffMenu(WORD nID)	
{
	WORD x, y, w, h;
	
	SetHLC_OnOff(nID?  ON:  OFF);
	
	if(nID)	{
		GetHLC_Position(&x, &y);				GetHLC_Size(&w, &h);
		SetMenuStatus(HSBLCPOSI_MENU, x);		SetMenuPosiY(HSBLCPOSI_MENU, y);
		SetMenuStatus(HSBLCSIZE_MENU, w);		SetMenuPosiY(HSBLCSIZE_MENU, h);
		SetMenuStatus(HSBLCDISP_MENU, GetHLC_WinOnOff());
		SetMenuStatus(HSBLCBMLEV_MENU, GetHLC_MaskLevel());
		SetMenuStatus(HSBLCBM_MENU, GetHLC_MaskOnOff());
	}
}

//--------------------------------------------------------------------------------------------
void SetHLC_PosiMenu(WORD nID)
{
	WORD x = GetMenuStatus(HSBLCPOSI_MENU),	 y = GetMenuPosiY(HSBLCPOSI_MENU); 
	WORD w = GetMenuStatus(HSBLCSIZE_MENU),	 h = GetMenuPosiY(HSBLCSIZE_MENU); 
	WORD w_max, h_max;
	
	GetHLC_SizeLimit(&w_max, &h_max);
	
	// check position x/y limit
	if(x > 63)				{x = 63;			SetMenuStatus(HSBLCPOSI_MENU, x);}		// position_x register limit
	if(y > 63)				{y = 63;			SetMenuPosiY(HSBLCPOSI_MENU, y);}		// position_y register limit
	
	// check 'size + position' limit
	if(x + w > w_max)		{x = w_max - w;		SetMenuStatus(HSBLCPOSI_MENU, x);}
	if(y + h > h_max)		{y = h_max - h;		SetMenuPosiY(HSBLCPOSI_MENU, y);}
	
	SetHLC_Position(x, y);
}

//--------------------------------------------------------------------------------------------
void SetHLC_SizeMenu(WORD nID)
{
	WORD x = GetMenuStatus(HSBLCPOSI_MENU),	 y = GetMenuPosiY(HSBLCPOSI_MENU); 
	WORD w = GetMenuStatus(HSBLCSIZE_MENU),	 h = GetMenuPosiY(HSBLCSIZE_MENU); 
	WORD w_max, h_max;
	
	GetHLC_SizeLimit(&w_max, &h_max);
	
	// check size limit
	if(w > 63)				{w = 63;			SetMenuStatus(HSBLCSIZE_MENU, w);}		// size_w register limit
	if(h > 63)				{h = 63;			SetMenuPosiY(HSBLCSIZE_MENU, h);}		// size_h register limit
	if(w <  1)				{w =  1;			SetMenuStatus(HSBLCSIZE_MENU, w);}		// size_w min
	if(h <  1)				{h =  1;			SetMenuPosiY(HSBLCSIZE_MENU, h);}		// size_h min
	
	// check 'size + position' limit
	if(x + w > w_max)		{w = w_max - x;		SetMenuStatus(HSBLCSIZE_MENU, w);}
	if(y + h > h_max)		{h = h_max - y;		SetMenuPosiY(HSBLCSIZE_MENU, h);}
	
	SetHLC_Size(w, h);
}

//--------------------------------------------------------------------------------------------
void SetHLC_FactoryDefaultMenu(BOOL OnOff)
{
	WORD x, y, w, h;
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)	// 4k60 master only
	SPImAPP_SetSlaveHLCDefault();
#endif
	
	SetHLC_OnOff(OnOff?  ON:  OFF);
	
	if(OnOff)	{
		SetHLC_Default();						SetHLC_OnOff(ON);
		
		GetHLC_Position(&x, &y);				GetHLC_Size(&w, &h);
		SetMenuStatus(HSBLCPOSI_MENU, x);		SetMenuPosiY(HSBLCPOSI_MENU, y);
		SetMenuStatus(HSBLCSIZE_MENU, w);		SetMenuPosiY(HSBLCSIZE_MENU, h);
		SetMenuStatus(HSBLCDISP_MENU, GetHLC_WinOnOff());
		SetMenuStatus(HSBLCBMLEV_MENU, GetHLC_MaskLevel());
		SetMenuStatus(HSBLCBM_MENU, GetHLC_MaskOnOff());
	}
}

//--------------------------------------------------------------------------------------------
static void VS_Process(PVS_CTRL_PARAM pCTRL)
{
	DWORD e_addr;
	WORD i, j, lx, ly, w, h;
	WORD col_count_per_line, row_count_per_frame, mem_w;
	BYTE pBuff[4096], Cb, Cr, bit_mode, dot=2;
	
	
	if((pCTRL->ctrl & VSCTRL_MODE_ON) != VSCTRL_MODE_ON)	{
		MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX7], OFF);
		MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX3], OFF);	
		return;
	}
	
	
	// draw area box
	if(pCTRL->ctrl & VSCTRL_SCAN_USER)	{
		lx = pCTRL->x;		ly = pCTRL->y;		w = pCTRL->w;		h = pCTRL->h;		// area scan
	}
	else	{
		lx = 0;				ly = 0;				w = pCTRL->w_max;	h = pCTRL->h_max;	// full scan
	}
	MISPOSD_SetSBoxArea(&stSBOX[SBOX_INDEX3], lx, ly, lx+w, ly+h);
	MISPOSD_EnableSBoxBorder(&stSBOX[SBOX_INDEX3], ON);	
	
	
	// draw vector scope
	if(pCTRL->ctrl & VSCTRL_DRAW_TRIG)	{
		pCTRL->ctrl &= (~VSCTRL_DRAW_TRIG);
		
		MISP_RegField(MISP_LOCAL_ID, 0x040, 0, 2, 3); 		// enable 'main_wr_stop'(0:disable, 1:enable)
		
		OSD_DrawVSboard();													// draw background
		MISPOSD_EnableSprite(&stOSD[SPRITE_INDEX7], ON);
		
		GetDDRMAP_IPC_MemCRPF(&bit_mode,&col_count_per_line,&row_count_per_frame);
		mem_w = (col_count_per_line&0x3FF) * 8;								// calc 1-line width in ddr mem
		
		for (i = 0; i <h; i++)
		{
			e_addr = ((DWORD)GetDDRMAP_IPC_C_start_row() << 12) + (i+ly)*mem_w + lx;
			MISP_MultiRead(MISP_SDRAM_ID, e_addr, &pBuff[0], w);			// read ddr mem.
			for (j = 0; j <w; j+=2)
			{
				Cb = 256 - pBuff[j];
				Cr = pBuff[j+1];
				e_addr = stOSD[SPRITE_INDEX7].stBMP.addr + Cb*256 + Cr;
				MISP_MultiWrite(MISP_SDRAM_ID, e_addr, &dot, 1);			// write osd sprite
			}
		}
		
		MISP_RegField(MISP_LOCAL_ID, 0x040, 0, 2, 0); 		// disable 'main_wr_stop'(0:disable, 1:enable)
	}
}

//--------------------------------------------------------------------------------------------
void SetVS_OnOffMenu(WORD OnOff)
{
	stVSCTRL.ctrl &= ~VSCTRL_MODE_ON;
	
	if(OnOff)	stVSCTRL.ctrl |= (VSCTRL_MODE_ON | VSCTRL_DRAW_TRIG);
	else		stVSCTRL.ctrl |= VSCTRL_MODE_OFF;
	
	VS_Process(&stVSCTRL);
}

//--------------------------------------------------------------------------------------------
void SetVS_ScanAreaMenu(WORD mode)
{
	stVSCTRL.ctrl &= ~VSCTRL_SCAN_USER;
	
	if(mode)	stVSCTRL.ctrl |= (VSCTRL_SCAN_USER | VSCTRL_DRAW_TRIG);
	else		stVSCTRL.ctrl |= (VSCTRL_SCAN_FULL | VSCTRL_DRAW_TRIG);
	
	SetMenuStatus(VSPOSIX_MENU, stVSCTRL.x/32);		SetMenuStatus(VSPOSIY_MENU, stVSCTRL.y/32);
	SetMenuStatus(VSSIZEX_MENU, stVSCTRL.w/32);		SetMenuStatus(VSSIZEY_MENU, stVSCTRL.h/32);
	
	VS_Process(&stVSCTRL);
}

//--------------------------------------------------------------------------------------------
void SetVS_DrawMenu(void)
{
	stVSCTRL.ctrl |= VSCTRL_DRAW_TRIG;
	
	VS_Process(&stVSCTRL);
}

//--------------------------------------------------------------------------------------------
void SetVS_PosiXMenu(WORD x)
{
	x *= 32;
	
	if(x + stVSCTRL.w > stVSCTRL.w_max)	{
		x = stVSCTRL.w_max - stVSCTRL.w;			SetMenuStatus(VSPOSIX_MENU, x/32);
	}
	
	stVSCTRL.x = x;
	VS_Process(&stVSCTRL);
}

//--------------------------------------------------------------------------------------------
void SetVS_PosiYMenu(WORD y)
{
	y *= 32;
	
	if(y + stVSCTRL.h > stVSCTRL.h_max)	{
		y = stVSCTRL.h_max - stVSCTRL.h;			SetMenuStatus(VSPOSIY_MENU, y/32);
	}
	
	stVSCTRL.y = y;
	VS_Process(&stVSCTRL);
}

//--------------------------------------------------------------------------------------------
void SetVS_SizeXMenu(WORD w)
{
	w *= 32;
	
	if(w + stVSCTRL.x > stVSCTRL.w_max)	{
		w = stVSCTRL.w_max - stVSCTRL.x;			SetMenuStatus(VSSIZEX_MENU, w/32);
	}
	
	stVSCTRL.w = w;
	VS_Process(&stVSCTRL);
}

//--------------------------------------------------------------------------------------------
void SetVS_SizeYMenu(WORD h)
{
	h *= 32;
	
	if(h + stVSCTRL.y > stVSCTRL.h_max)	{
		h = stVSCTRL.h_max - stVSCTRL.y;			SetMenuStatus(VSSIZEY_MENU, h/32);
	}
	
	stVSCTRL.h = h;
	VS_Process(&stVSCTRL);
}


/*  FILE_END_HERE */
