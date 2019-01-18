//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2015  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	VISCA_DRV.C
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
VISCA_CTRL_PARAM	stVISCACTRL;
VISCA_INFO_PARAM	stVISCAINFO;

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static volatile BOOL fVISCA_CmdParsed;
volatile BOOL fVISCA_CmdReceive, fSerialCmdReceive;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
static void VISCA_ReplyMsg(BYTE msg, BYTE sn);

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

void VISCA_Initialize(void)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[24];
#endif

	memset(&stVISCACTRL, 0, sizeof(VISCA_CTRL_PARAM));
	memset(&stVISCAINFO, 0, sizeof(VISCA_INFO_PARAM));
	
	stVISCACTRL.cam_addr = 0x01;					// visca camera address(1~7)
	
	// lens control system
	stVISCAINFO.lens.ctrl1 = VISCA_AFSENS_NORMAL;
	stVISCAINFO.lens.ctrl1 |= (VISCAAPP_GetDZoomMode()?  VISCA_DZOOM_ON:  VISCA_DZOOM_OFF);
	stVISCAINFO.lens.ctrl1 |= (VISCAAPP_GetDZoomCSMode()?  VISCA_DZOOM_COMBINE_OFF:  VISCA_DZOOM_COMBINE_ON);
	stVISCAINFO.lens.ctrl1 |= (VISCAAPP_GetFocusMode()?  VISCA_FOCUS_MODE_AUTO:  VISCA_FOCUS_MODE_MANUAL);
	stVISCAINFO.lens.ctrl1 |= (VISCAAPP_GetAFMode() << 3);								// 0:norm, 1:inter, 2:zoom-trig
	
	stVISCAINFO.lens.ctrl2 = VISCA_LOWCONTRAST_DET_OFF | VISCA_CAMMEM_RECALL_STOP | VISCA_FOCUS_CMD_STOP | \
							VISCA_ZOOM_CMD_STOP;
	
	// camera control system
	stVISCAINFO.cam.rgain = VISCAAPP_GetWB_ManGainR();
	stVISCAINFO.cam.bgain = VISCAAPP_GetWB_ManGainB();
	stVISCAINFO.cam.wb_mode = VISCAAPP_GetWB_Mode();
	stVISCAINFO.cam.aperture_gain = VISCAAPP_GetAperture();			// 0~f
	stVISCAINFO.cam.exposure_mode = VISCAAPP_GetExposure_Mode();
	stVISCAINFO.cam.ctrl = VISCA_HR_OFF | VISCA_WD_OFF | VISCA_SPOTAE_OFF | VISCA_EXPCOMP_OFF;// | VISCA_SLOWSHUTTER_OFF;
	stVISCAINFO.cam.ctrl |= GetBLC_OnOff()?  VISCA_BACKLIGHT_ON:  VISCA_BACKLIGHT_OFF;
	stVISCAINFO.cam.ctrl |= VISCAAPP_GetAutoSlowShutterOnOff()?  VISCA_SLOWSHUTTER_ON:  VISCA_SLOWSHUTTER_OFF;
	stVISCAINFO.cam.shutter_posi = VISCAAPP_GetShutterPosi();			// 0(1/1) ~ 0x15(1/10000), 0x05(1/30)
	stVISCAINFO.cam.iris_posi = VISCAAPP_GetIrisPosi();	// 0(close) ~ 0x11(F1.6)
	stVISCAINFO.cam.gain_posi = VISCAAPP_GetAGCGain();	// 1(0 step) ~ 0x0F(28 step)
	stVISCAINFO.cam.bright_posi = 0x11;				// 0(close + 0 step) ~ 0x11(F1.6 + 0 step) ~ 0x1F(F1.6 + 28 step)
	stVISCAINFO.cam.expcomp_posi = 0x07;			// 0(-10.5dB) ~ 7(0dB) ~ 0x0E(+10.5dB)
	
	// other
	stVISCAINFO.other.ctrl1 = VISCA_AUTOICR_ALARM_OFF | VISCA_POWER_ON;
	stVISCAINFO.other.ctrl1 |= ((VISCAAPP_GetAutoICRMode()==0x02)?  VISCA_AUTOICR_ON:  VISCA_AUTOICR_OFF);
	
	stVISCAINFO.other.ctrl2 = VISCA_STABILIZER_OFF | VISCA_STABILIZER_HOLD_OFF | VISCA_FREEZE_OFF | VISCA_LRREVERSE_OFF;
	stVISCAINFO.other.ctrl2 |= (VISCAAPP_GetICRMode()==0x02)?  VISCA_ICR_ON:  VISCA_ICR_OFF;
	
	stVISCAINFO.other.ctrl3 = VISCA_PM_OFF | VISCA_MUTE_OFF | VISCA_TILEDISP_OFF | VISCA_DISP_OFF;
	stVISCAINFO.other.pic_effect_mode = VISCA_PICEFFECT_OFF;
	stVISCAINFO.other.cam_id = 0x0000;				// 0 ~ 0xFFFF
	stVISCAINFO.other.ctrl4 = VISCA_MEM_NOTPROVIDED | VISCA_ICR_NOTPROVIDED | \
								VISCA_STABILIZER_NOTPROVIDED | VISCA_SYSTEM_60_30HZ;
	
	// enlargement function-1
	stVISCAINFO.efunc1.dzoom_posi = 0x00;			// sep:0(x1) ~ 0xEB(x12), comb:0x4000(x1) ~ 0x7AC0(x12)
	stVISCAINFO.efunc1.spotae_posi = 0x88;			// X/Y:0~0x0F
	stVISCAINFO.efunc1.ctrl1 = VISCA_FLIP_OFF;
	stVISCAINFO.efunc1.ctrl1 |= (GetMD_OnOff())?  VISCA_MD_ON:  VISCA_MD_OFF;
	stVISCAINFO.efunc1.color_gain = 0x04;			// 0(60%) ~ 4(100%) ~ 0x0E(200%)
	stVISCAINFO.efunc1.ctrl2 = VISCA_ADVPM_NOTPROVIDED | VISCA_FLIP_PROVIDED;
	stVISCAINFO.efunc1.ae_response = 0x01;			// 1(1sec) ~ 0x30(600sec)
	stVISCAINFO.efunc1.gamma = 0x00;				// 0 ~ 4
	stVISCAINFO.efunc1.ctrl3 = VISCA_HIGHSENS_OFF;
	stVISCAINFO.efunc1.nr_level = 0x02;				// 0 ~ 3
	stVISCAINFO.efunc1.chroma_suppress = 0;//0x02;		// 0 ~ 3
	
	// enlargement function-2
	stVISCAINFO.efunc2.wd_mode = VISCAAPP_GetWDROnOff()?  VISCA_WDMODE_ON:  VISCA_WDMODE_OFF;
	stVISCAINFO.efunc2.wd_bright_lev = 0x03;		// 0(dark) ~ 6(bright)
	stVISCAINFO.efunc2.wd_bright_comp_sel = 0x02;	// 0(v.dark), 1(dark), 2(standard), 3(bright)
	stVISCAINFO.efunc2.wd_comp_lev = 0x01;			// 0(low), 1(mid), 2(high)
	stVISCAINFO.efunc2.defog_mode = VISCAAPP_GetDefogMode();	// 0(off), 1(on/auto)
	stVISCAINFO.efunc2.defog_lev = VISCAAPP_GetDefogLevel();	// 0~5(low), 6~10(mid), 11~16(high)
	
	// enlargement function-3
	stVISCAINFO.efunc3.color_hue = 0x07;			// 0(-14) ~ 7(0) ~ 0x0E(+14)
	
	// pm
	stVISCAINFO.pm.mask_disp = VISCA_PM_MASK_OFF;
	
	// register
	stVISCAINFO.reg.out_en = VISCA_REG_OUTEN_BOTH_EN;
	
	// extended
	stVISCAINFO.enlarge.expcomp_posi = 0x80;		// 0(-10.5dB) ~ 0x80(0dB) ~ 0xFF(+10.5dB)
	stVISCAINFO.enlarge.aperture_gain = 0x80;		// 0 ~ 0xFF
	stVISCAINFO.enlarge.color_gain = 0x80;			// 0(0%) ~ 0x80(100%) ~ 0xFF(200%)
	stVISCAINFO.enlarge.color_hue = 0x80;			// 0(-14) ~ 0x80(0) ~ 0xFF(+14)
	
	// user define
	stVISCAINFO.user.memory = 0x00;					// 0 ~ 0x0F(memory number called last)
	stVISCAINFO.user.ctrl = VISCA_IRCORRECTION_OFF;
	stVISCAINFO.user.lens_temp = 0x14;				// 0xFB(-5) ~ 0(0) ~ 0x3C(60)
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	EEPROM_MultiRead(EEPROM_VISCA_MEMSAVE, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE) 	stVISCAINFO.user.memsave[0] = MAKEWORD(buff[1], buff[2]);
	if(buff[3] == EEPROM_SAVE_DONE) 	stVISCAINFO.user.memsave[1] = MAKEWORD(buff[4], buff[5]);
	if(buff[6] == EEPROM_SAVE_DONE) 	stVISCAINFO.user.memsave[2] = MAKEWORD(buff[7], buff[8]);
	if(buff[9] == EEPROM_SAVE_DONE) 	stVISCAINFO.user.memsave[3] = MAKEWORD(buff[10], buff[11]);
	if(buff[12] == EEPROM_SAVE_DONE) 	stVISCAINFO.user.memsave[4] = MAKEWORD(buff[13], buff[14]);
	if(buff[15] == EEPROM_SAVE_DONE) 	stVISCAINFO.user.memsave[5] = MAKEWORD(buff[16], buff[17]);
	if(buff[18] == EEPROM_SAVE_DONE) 	stVISCAINFO.user.memsave[6] = MAKEWORD(buff[19], buff[20]);
	if(buff[21] == EEPROM_SAVE_DONE) 	stVISCAINFO.user.memsave[7] = MAKEWORD(buff[22], buff[23]);
#endif
	
	
	VISCA_ReplyMsg(VISCA_MSG_NET_CHANGE, 0);
	VISCAAPP_DisplayOnOff(ON);						// clear data screen display
	VISCAAPP_DisplayOnOff(OFF);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ReplyMsg(BYTE msg, BYTE sn)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	BYTE txaddr = 0x80 | (stVISCACTRL.cam_addr<<4);					// cam address + 8
	BYTE i;

	switch(msg)	{
		case VISCA_MSG_ACK:				pMsg[0]=txaddr;	pMsg[1]=(0x40|sn);	pMsg[2]=0xff;	break;	// ack(socket1/2)
		case VISCA_MSG_CMD_COMPLETE:	pMsg[0]=txaddr;	pMsg[1]=(0x50|sn);	pMsg[2]=0xff;	break;	// cmd complete
		case VISCA_MSG_INQ_COMPLETE:	pMsg[0]=txaddr;	pMsg[1]=0x50;						break;	// inq complete
		case VISCA_MSG_ERR_SYNTAX:		pMsg[0]=txaddr;	pMsg[1]=0x60;		pMsg[2]=0x02;	pMsg[3]=0xff;	break;	// syntax error
		case VISCA_MSG_ERR_CMDBUFFULL:	pMsg[0]=txaddr;	pMsg[1]=0x60;		pMsg[2]=0x03;	pMsg[3]=0xff;	break;	// cmd buffer full
		case VISCA_MSG_ERR_CMDCANCEL:	pMsg[0]=txaddr;	pMsg[1]=(0x60|sn);	pMsg[2]=0x04;	pMsg[3]=0xff;	break;	// cmd canceled
		case VISCA_MSG_ERR_NOSOCKET:	pMsg[0]=txaddr;	pMsg[1]=(0x60|sn);	pMsg[2]=0x05;	pMsg[3]=0xff;	break;	// no socket
		case VISCA_MSG_ERR_CMDNOTEXEC:	pMsg[0]=txaddr;	pMsg[1]=(0x60|sn);	pMsg[2]=0x41;	pMsg[3]=0xff;	break;	// cmd not executable
		case VISCA_MSG_NET_CHANGE:		pMsg[0]=txaddr;	pMsg[1]=0x38;		pMsg[2]=0xff;	break;
		case VISCA_MSG_AUTO_REPLY:		pMsg[0]=txaddr;	pMsg[1]=0x07;						break;	// auto reply
		case VISCA_MSG_USER_DEFINE:		break;
	}
	
	for(i=0;i<16;i++)	{
		uart_putch(VISCA_PORT_NO, pMsg[i]);		if(pMsg[i] == 0xff)		break;
	}
	fVISCA_CmdReceive = OFF;	
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ACK(void)
{
	VISCA_ReplyMsg(VISCA_MSG_ACK, stVISCACTRL.sn);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_COMP(void)
{
	VISCA_ReplyMsg(VISCA_MSG_CMD_COMPLETE, stVISCACTRL.sn);
}
/*
//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_nEXE(void)
{
	VISCA_ReplyMsg(VISCA_MSG_ERR_CMDNOTEXEC, stVISCACTRL.sn);
}
*/
//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_SynERR(void)
{
	VISCA_ReplyMsg(VISCA_MSG_ERR_SYNTAX, 0);
}

// Inquiry func.
//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_AddressSet(PBYTE pBuf)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	BYTE  rxaddr = pBuf[2] & 0x0F;
	
//	if(rxaddr != stVISCACTRL.cam_addr)			return;	// check cam address
	stVISCACTRL.cam_addr = rxaddr;
	
	pMsg[0] = 0x88;							// always broadcasted
	pMsg[1] = 0x30;							// address set
	pMsg[2] = rxaddr+1;						// camera address + 1
	pMsg[3] = 0xff;							// terminator

	VISCA_ReplyMsg(VISCA_MSG_USER_DEFINE, 0);	
	
	stVISCACTRL.ctrl |= VISCA_IF_ACTIVATED;	// visca i/f activated by external equipment
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_CMDIFClear(PBYTE pBuf)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	BYTE  rxaddr = pBuf[0] & 0x0F;
	
	memset((PBYTE)stVISCACTRL.CmdBuffer[0], 0, 16*2);							// clear buffer(1,2)
	stVISCACTRL.ctrl &= ~(VISCA_CMD_S1_PARSED | VISCA_CMD_S2_PARSED);	// clear flag
	
	if(rxaddr == stVISCACTRL.cam_addr)	{
		pMsg[0] = 0x80 | (rxaddr<<4);		// camera address + 8
		pMsg[1] = 0x50;						// complete reply
		pMsg[2] = 0xff;						// terminator
	}
	else	{
		pMsg[0] = 0x88;						// broadcast
		pMsg[1] = 0x01;						// same command
		pMsg[2] = 0x00;						// same command
		pMsg[3] = 0x01;						// same command
		pMsg[4] = 0xff;						// terminator
	}

	VISCA_ReplyMsg(VISCA_MSG_USER_DEFINE, 0);	
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqBlockLens(void)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	PVISCA_LENS_PARAM	pINFO = &stVISCAINFO.lens;
	WORD focus_posi, zoom_posi, nearlimit;
	
	focus_posi = VISCAAPP_GetFocusPosi();
	zoom_posi = VISCAAPP_GetZoomPosi();
	nearlimit = VISCAAPP_GetFocusNearLimit();
	
	pINFO->ctrl1 &= ~(VISCA_DZOOM_ON | VISCA_DZOOM_COMBINE_OFF | VISCA_FOCUS_MODE_AUTO | VISCA_AFMODE_MASK);	// clear flag
	if(VISCAAPP_GetDZoomMode())		pINFO->ctrl1 |= VISCA_DZOOM_ON;							// dzoom on
	if(VISCAAPP_GetDZoomCSMode())	pINFO->ctrl1 |= VISCA_DZOOM_COMBINE_OFF;				// separate mode
	if(VISCAAPP_GetFocusMode())		pINFO->ctrl1 |= VISCA_FOCUS_MODE_AUTO;					// focus(auto)
	stVISCAINFO.lens.ctrl1 |= (VISCAAPP_GetAFMode() << 3);									// 0:norm, 1:inter, 2:zoom-trig
	
	memset(pMsg, 0, 16);
	pMsg[2] = HI4BIT(HIBYTE(zoom_posi));				pMsg[3] = LO4BIT(HIBYTE(zoom_posi));
	pMsg[4] = HI4BIT(LOBYTE(zoom_posi));				pMsg[5] = LO4BIT(LOBYTE(zoom_posi));
	pMsg[6] = HI4BIT(HIBYTE(nearlimit));				pMsg[7] = LO4BIT(HIBYTE(nearlimit));
	pMsg[8] = HI4BIT(HIBYTE(focus_posi));				pMsg[9] = LO4BIT(HIBYTE(focus_posi));
	pMsg[10] = HI4BIT(LOBYTE(focus_posi));				pMsg[11] = LO4BIT(LOBYTE(focus_posi));
	pMsg[12] = 0x00;									pMsg[13] = pINFO->ctrl1;
	pMsg[14] = pINFO->ctrl2;							pMsg[15] = 0xff;
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqBlockCamera(void)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	PVISCA_CAM_PARAM	pINFO = &stVISCAINFO.cam;
	
	pINFO->iris_posi = VISCAAPP_GetIrisPosi();
	pINFO->gain_posi = VISCAAPP_GetAGCGain();
	pINFO->shutter_posi = VISCAAPP_GetShutterPosi();
	pINFO->exposure_mode = VISCAAPP_GetExposure_Mode();
	pINFO->wb_mode = VISCAAPP_GetWB_Mode();
	pINFO->rgain = VISCAAPP_GetWB_ManGainR();
	pINFO->bgain = VISCAAPP_GetWB_ManGainB();
	pINFO->aperture_gain = VISCAAPP_GetAperture();
	
	pINFO->ctrl &= ~(VISCA_BACKLIGHT_ON);
	pINFO->ctrl |= GetBLC_OnOff()?  VISCA_BACKLIGHT_ON:  VISCA_BACKLIGHT_OFF;
	
	pINFO->ctrl &= ~(VISCA_SLOWSHUTTER_ON);
	pINFO->ctrl |= VISCAAPP_GetAutoSlowShutterOnOff()?  VISCA_SLOWSHUTTER_ON:  VISCA_SLOWSHUTTER_OFF;
	
	memset(pMsg, 0, 16);
	pMsg[2] = HI4BIT(pINFO->rgain);					pMsg[3] = LO4BIT(pINFO->rgain);
	pMsg[4] = HI4BIT(pINFO->bgain);					pMsg[5] = LO4BIT(pINFO->bgain);
	pMsg[6] = LO4BIT(pINFO->wb_mode);				pMsg[7] = LO4BIT(pINFO->aperture_gain);
	pMsg[8] = (pINFO->exposure_mode & 0x1f);		pMsg[9] = pINFO->ctrl;
	pMsg[10] = (pINFO->shutter_posi & 0x1f);		pMsg[11] = (pINFO->iris_posi & 0x1f);
	pMsg[12] = LO4BIT(pINFO->gain_posi);			pMsg[13] = (pINFO->bright_posi & 0x1f);
	pMsg[14] = LO4BIT(pINFO->expcomp_posi);			pMsg[15] = 0xff;
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqBlockOther(void)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	PVISCA_OTHER_PARAM	pINFO = &stVISCAINFO.other;
	
	pINFO->ctrl1 &= ~(VISCA_AUTOICR_ON);
	pINFO->ctrl1 |= ((VISCAAPP_GetAutoICRMode()==0x02)?  VISCA_AUTOICR_ON:  VISCA_AUTOICR_OFF);
	
	pINFO->ctrl2 &= ~(VISCA_ICR_ON);
	pINFO->ctrl2 |= (VISCAAPP_GetICRMode()==0x02)?  VISCA_ICR_ON:  VISCA_ICR_OFF;
	
	memset(pMsg, 0, 16);
	pMsg[2] = pINFO->ctrl1;							pMsg[3] = pINFO->ctrl2;
	pMsg[4] = pINFO->ctrl3;							pMsg[5] = LO4BIT(pINFO->pic_effect_mode);
	pMsg[6] = 0x00;									pMsg[7] = 0x00;
	pMsg[8] = HI4BIT(HIBYTE(pINFO->cam_id));		pMsg[9] = LO4BIT(HIBYTE(pINFO->cam_id));
	pMsg[10] = HI4BIT(LOBYTE(pINFO->cam_id));		pMsg[11] = LO4BIT(LOBYTE(pINFO->cam_id));
	pMsg[12] = pINFO->ctrl4;						pMsg[13] = 0x00;
	pMsg[14] = 0x00;								pMsg[15] = 0xff;
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqBlockExFuncOne(void)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	PVISCA_EFUNC1_PARAM	pINFO = &stVISCAINFO.efunc1;
	BYTE gain_limit, af_act_time, af_inter_time;
	
	stVISCAINFO.efunc1.dzoom_posi = VISCAAPP_GetDZoomPosi();
	af_act_time = VISCAAPP_GetAFActiveTime();
	af_inter_time = VISCAAPP_GetAFIntervalTime();
	gain_limit = VISCAAPP_GetGainLimit();
	pINFO->ctrl1 &= (~VISCA_MD_ON);
	pINFO->ctrl1 |= GetMD_OnOff()?  VISCA_MD_ON:  VISCA_MD_OFF;
	
	memset(pMsg, 0, 16);
	pMsg[2] = HI4BIT(pINFO->dzoom_posi);			pMsg[3] = LO4BIT(pINFO->dzoom_posi);
	pMsg[4] = HI4BIT(af_act_time);					pMsg[5] = LO4BIT(af_act_time);
	pMsg[6] = HI4BIT(af_inter_time);				pMsg[7] = LO4BIT(af_inter_time);
	pMsg[8] = HI4BIT(pINFO->spotae_posi);			pMsg[9] = LO4BIT(pINFO->spotae_posi);
	pMsg[10] = pINFO->ctrl1;						pMsg[11] = (pINFO->color_gain<<3) | pINFO->ctrl2;
	pMsg[12] = pINFO->ae_response & 0x1f;			pMsg[13] = (pINFO->gamma<<4) | (pINFO->ctrl3 | pINFO->nr_level);
	pMsg[14] = MAKEBYTE(pINFO->chroma_suppress, gain_limit);
	pMsg[15] = 0xff;
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqBlockExFuncTwo(void)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	PVISCA_EFUNC2_PARAM	pINFO = &stVISCAINFO.efunc2;
	
	pINFO->defog_mode = VISCAAPP_GetDefogMode();
	pINFO->defog_lev = VISCAAPP_GetDefogLevel();
	pINFO->wd_mode = VISCAAPP_GetWDROnOff()?  VISCA_WDMODE_ON:  VISCA_WDMODE_OFF;
	
	memset(pMsg, 0, 16);
	pMsg[2] = pINFO->wd_mode & 0x03;				pMsg[3] = 0x00;
	pMsg[4] = pINFO->wd_bright_lev & 0x07;			pMsg[5] = pINFO->wd_bright_comp_sel & 0x03;
	pMsg[6] = pINFO->wd_comp_lev & 0x03;			pMsg[7] = (pINFO->defog_mode)?  1:  0;
	pMsg[8] = pINFO->defog_lev & 0x03;				pMsg[9] = 0x00;
	pMsg[10] = 0x00;								pMsg[11] = 0x00;
	pMsg[12] = 0x00;								pMsg[13] = 0x00;
	pMsg[14] = 0x00;								pMsg[15] = 0xff;
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqBlockExFuncThree(void)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	PVISCA_EFUNC3_PARAM	pINFO = &stVISCAINFO.efunc3;
	
	memset(pMsg, 0, 16);
	pMsg[2] = LO4BIT(pINFO->color_hue);				pMsg[3] = 0x00;
	pMsg[4] = 0x00;									pMsg[5] = 0x00;
	pMsg[6] = 0x00;									pMsg[7] = 0x00;
	pMsg[8] = 0x00;									pMsg[9] = 0x00;
	pMsg[10] = 0x00;								pMsg[11] = 0x00;
	pMsg[12] = 0x00;								pMsg[13] = 0x00;
	pMsg[14] = 0x00;								pMsg[15] = 0xff;
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqBlockCommand(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case 0x00:	VISCA_InqBlockLens();			break;
		case 0x01:	VISCA_InqBlockCamera();			break;
		case 0x02:	VISCA_InqBlockOther();			break;
		case 0x03:	VISCA_InqBlockExFuncOne();		break;
		case 0x04:	VISCA_InqBlockExFuncTwo();		break;
		case 0x05:	VISCA_InqBlockExFuncThree();	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqReplyByte(BYTE status)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	
	pMsg[2] = status;		pMsg[3] = 0xff;
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqReplyWord(BYTE val1, BYTE val2)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	
	pMsg[2] = HI4BIT(val1);					pMsg[3] = LO4BIT(val1);
	pMsg[4] = HI4BIT(val2);					pMsg[5] = LO4BIT(val2);
	pMsg[6] = 0xff;
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamPower(void)
{
	VISCA_InqReplyByte((stVISCAINFO.other.ctrl1 & VISCA_POWER_ON)?  0x02:  0x03);	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamZoomPosi(void)
{
	WORD zoom_posi = VISCAAPP_GetZoomPosi();
	
	VISCA_InqReplyWord(HIBYTE(zoom_posi), LOBYTE(zoom_posi));	// zoom position
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamDZoomMode(void)
{
	if(VISCAAPP_GetDZoomMode())		stVISCAINFO.lens.ctrl1 |= VISCA_DZOOM_ON;		// dzoom on
	else							stVISCAINFO.lens.ctrl1 &= (~VISCA_DZOOM_ON);	// dzoom off
	
	VISCA_InqReplyByte((stVISCAINFO.lens.ctrl1 & VISCA_DZOOM_ON)?  0x02:  0x03);	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamDZoomCSMode(void)
{
	if(VISCAAPP_GetDZoomCSMode())	stVISCAINFO.lens.ctrl1 |= VISCA_DZOOM_COMBINE_OFF;		// separate mode
	else							stVISCAINFO.lens.ctrl1 &= (~VISCA_DZOOM_COMBINE_OFF);	// combine mode
	
	VISCA_InqReplyByte((stVISCAINFO.lens.ctrl1 & VISCA_DZOOM_COMBINE_OFF)?  0x01:  0x00);	// 0:comb, 1:sep
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamDZoomPosi(void)
{
	stVISCAINFO.efunc1.dzoom_posi = VISCAAPP_GetDZoomPosi();
	VISCA_InqReplyWord(0x00, stVISCAINFO.efunc1.dzoom_posi);			// dzoom position
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamFocusMode(void)
{
	if(VISCAAPP_GetFocusMode())		stVISCAINFO.lens.ctrl1 |= VISCA_FOCUS_MODE_AUTO;	// focus(auto, interval, zoom trig)
	else							stVISCAINFO.lens.ctrl1 &= (~VISCA_FOCUS_MODE_AUTO);	// focus(manual, one push)
	
	VISCA_InqReplyByte((stVISCAINFO.lens.ctrl1 & VISCA_FOCUS_MODE_AUTO)?  0x02:  0x03);	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamFocusPosi(void)
{
	WORD focus_posi = VISCAAPP_GetFocusPosi();
	
	VISCA_InqReplyWord(HIBYTE(focus_posi), LOBYTE(focus_posi));	// focus position
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamFocusNearLimit(void)
{
	WORD nearlimit = VISCAAPP_GetFocusNearLimit();
	
	VISCA_InqReplyWord(HIBYTE(nearlimit), LOBYTE(nearlimit));		// focus near limit
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamAFSensitivity(void)
{
	VISCA_SynERR();			// inq not implemented
//	VISCA_InqReplyByte((stVISCAINFO.lens.ctrl1 & VISCA_AFSENS_NORMAL)?  0x02:  0x03);	// 2:normal, 3:low
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamAFMode(void)
{
	stVISCAINFO.lens.ctrl1 |= (VISCAAPP_GetAFMode() << 3);
	VISCA_InqReplyByte((stVISCAINFO.lens.ctrl1 & VISCA_AFMODE_MASK) >> 3);	// 0:norm, 1:inter, 2:zoom-trig
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamAFTimeSetting(void)
{
	VISCA_InqReplyWord(VISCAAPP_GetAFActiveTime(), VISCAAPP_GetAFIntervalTime());
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamIRCorrection(void)
{
	VISCA_SynERR();			// inq not implemented
//	VISCA_InqReplyByte((stVISCAINFO.user.ctrl & VISCA_IRCORRECTION_ON)?  0x01:  0x00);	// 0:off, 1:IR light
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamWBMode(void)
{
	stVISCAINFO.cam.wb_mode = VISCAAPP_GetWB_Mode();
	VISCA_InqReplyByte(stVISCAINFO.cam.wb_mode);	// 0:auto, 1:in, 2:out, 3:onepush, 4:atw, 5:manual, 6:outauto, 7~9:sodium-xx
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamWBGainR(void)
{
	stVISCAINFO.cam.rgain = VISCAAPP_GetWB_ManGainR();
	VISCA_InqReplyWord(0x00, stVISCAINFO.cam.rgain);					// R Gain
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamWBGainB(void)
{
	stVISCAINFO.cam.bgain = VISCAAPP_GetWB_ManGainB();
	VISCA_InqReplyWord(0x00, stVISCAINFO.cam.bgain);					// B Gain
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamAEMode(void)
{
	stVISCAINFO.cam.exposure_mode = VISCAAPP_GetExposure_Mode();
	VISCA_InqReplyByte(stVISCAINFO.cam.exposure_mode);	// 0:auto, 3:man, A:shut, B:iris, D:bright
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamAutoSlowShutter(void)
{
	VISCA_InqReplyByte(VISCAAPP_GetAutoSlowShutterOnOff()?  0x02:  0x03);	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamShutterPosi(void)
{
	stVISCAINFO.cam.shutter_posi = VISCAAPP_GetShutterPosi();
	VISCA_InqReplyWord(0x00, stVISCAINFO.cam.shutter_posi);			// shutter position
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamIrisPosi(void)
{
	stVISCAINFO.cam.iris_posi = VISCAAPP_GetIrisPosi();
	VISCA_InqReplyWord(0x00, stVISCAINFO.cam.iris_posi);				// iris position
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamGainPosi(void)
{
	stVISCAINFO.cam.gain_posi = VISCAAPP_GetAGCGain();
	VISCA_InqReplyWord(0x00, stVISCAINFO.cam.gain_posi);				// gain position
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamGainLimit(void)
{
	VISCA_InqReplyByte(VISCAAPP_GetGainLimit());				// gain limit
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamBrightPosi(void)
{
	VISCA_InqReplyWord(0x00, stVISCAINFO.cam.bright_posi);				// bright position
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamExpCompMode(void)
{
	VISCA_InqReplyByte((stVISCAINFO.cam.ctrl & VISCA_EXPCOMP_ON)?  0x02:  0x03);	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamExpCompPosi(void)
{
	VISCA_InqReplyWord(0x00, stVISCAINFO.cam.expcomp_posi);			// expcomp position
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamBackLightMode(void)
{
	VISCA_InqReplyByte(GetBLC_OnOff()?  0x02:  0x03);				// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamSpotAEMode(void)
{
	VISCA_InqReplyByte((stVISCAINFO.cam.ctrl & VISCA_SPOTAE_ON)?  0x02:  0x03);	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamSpotAEPosi(void)
{
	VISCA_InqReplyWord(HI4BIT(stVISCAINFO.efunc1.spotae_posi), 			// X position
						LO4BIT(stVISCAINFO.efunc1.spotae_posi));		// Y position
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamWDMode(void)
{
	stVISCAINFO.efunc2.wd_mode = VISCAAPP_GetWDROnOff()?  VISCA_WDMODE_ON:  VISCA_WDMODE_OFF;
	VISCA_InqReplyByte((stVISCAINFO.efunc2.wd_mode & VISCA_WDMODE_VE_ON)?  0x06: \
						((stVISCAINFO.efunc2.wd_mode & VISCA_WDMODE_ON)?  0x02:  0x03));	// 2:on, 3:off, 6:ve on
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamWDParameter(void)
{
	VISCA_SynERR();			// inq not implemented
/*
	PBYTE pMsg = stVISCACTRL.TxPacket;
	PVISCA_EFUNC2_PARAM pINFO = &stVISCAINFO.efunc2;
	
	pMsg[2] = 0x00;									pMsg[3] = LO4BIT(pINFO->wd_bright_lev);
	pMsg[4] = LO4BIT(pINFO->wd_bright_comp_sel);	pMsg[5] = LO4BIT(pINFO->wd_comp_lev);
	pMsg[6] = 0x00;									pMsg[7] = 0x00;						// always 0
	pMsg[8] = 0x00;									pMsg[9] = 0x00;						// blank
	pMsg[10] = 0xff;
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
*/
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamAEResponse(void)
{
	VISCA_InqReplyByte(stVISCAINFO.efunc1.ae_response);	// pp:0x01 ~ 0x30
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamDefog(void)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	
	stVISCAINFO.efunc2.defog_mode = VISCAAPP_GetDefogMode();
	stVISCAINFO.efunc2.defog_lev = VISCAAPP_GetDefogLevel();
	
	pMsg[2] = (stVISCAINFO.efunc2.defog_mode)?  0x02:  0x03;
	pMsg[3] = stVISCAINFO.efunc2.defog_lev;
	pMsg[4] = 0xff;
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamAperture(void)
{
	stVISCAINFO.cam.aperture_gain = VISCAAPP_GetAperture();
	VISCA_InqReplyWord(0x00, stVISCAINFO.cam.aperture_gain);			// aperture position
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamHRMode(void)
{
	VISCA_SynERR();			// inq not implemented
//	VISCA_InqReplyByte((stVISCAINFO.cam.ctrl & VISCA_HR_ON)?  0x02:  0x03);	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamNRMode(void)
{
	VISCA_InqReplyByte(stVISCAINFO.efunc1.nr_level);	// p:0~5
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamGamma(void)
{
	VISCA_SynERR();			// inq not implemented
//	VISCA_InqReplyByte(stVISCAINFO.efunc1.gamma);	// p:0~4
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamHighSens(void)
{
	VISCA_SynERR();			// inq not implemented
//	VISCA_InqReplyByte((stVISCAINFO.efunc1.ctrl3 & VISCA_HIGHSENS_ON)?  0x02:  0x03);	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamLRRerverseMode(void)
{
	VISCA_InqReplyByte(VISCAAPP_GetLRReverseMode());	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamFreezeMode(void)
{
	VISCA_InqReplyByte((stVISCAINFO.other.ctrl2 & VISCA_FREEZE_ON)?  0x02:  0x03);	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamPicEffectMode(void)
{
	VISCA_InqReplyByte(stVISCAINFO.other.pic_effect_mode);	// 0:off, 2:neg, 4:b&w
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamPicFlipMode(void)
{
	VISCA_InqReplyByte(VISCAAPP_GetPictureFlipMode());	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamICRMode(void)
{
	VISCA_InqReplyByte(VISCAAPP_GetICRMode());	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamAutoICRMode(void)
{
	VISCA_InqReplyByte(VISCAAPP_GetAutoICRMode());	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamAutoICRThres(void)
{
	VISCA_InqReplyWord(0x00, VISCAAPP_GetAutoICROnOffThres());			// auto icr threshold(icr on -> off threshold level)
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamAutoICRDelay(void)
{
	VISCA_InqReplyWord(0x00, VISCAAPP_GetAutoICRDelay());			// auto icr time delay(icr on <-> off)
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamAutoICRAlarm(void)
{
	VISCA_InqReplyByte((stVISCAINFO.other.ctrl1 & VISCA_AUTOICR_ALARM_ON)?  0x02:  0x03);	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamMemory(void)
{
	VISCA_SynERR();			// inq not implemented
//	VISCA_InqReplyByte(stVISCAINFO.user.memory);		// memory number recalled last(0 ~ 15?)
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamMemSave(PBYTE pBuf)
{
	BYTE  addr = pBuf[4];
	WORD  data = stVISCAINFO.user.memsave[MIN(addr, 7)];
	
	VISCA_InqReplyWord(HIBYTE(data), LOBYTE(data));	// addr(0~7), data(0 ~ 0xFFFF)
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamDisplayMode(void)
{
	VISCA_InqReplyByte((stVISCAINFO.other.ctrl3 & VISCA_DISP_ON)?  0x02:  0x03);	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamStabilizerMode(void)
{
	VISCA_InqReplyByte((stVISCAINFO.other.ctrl2 & VISCA_STABILIZER_HOLD_ON)?  0x00: \
						((stVISCAINFO.other.ctrl2 & VISCA_STABILIZER_ON)?  0x02:  0x03));	// 0:hold, 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamMuteMode(void)
{
	VISCA_InqReplyByte((stVISCAINFO.other.ctrl3 & VISCA_MUTE_ON)?  0x02:  0x03);	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamPMMaskDisplay(void)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	PVISCA_PM_PARAM pINFO = &stVISCAINFO.pm;
	
	pMsg[2] = HIBYTE(HIWORD(pINFO->mask_disp));		pMsg[3] = LOBYTE(HIWORD(pINFO->mask_disp));
	pMsg[4] = HIBYTE(LOWORD(pINFO->mask_disp));		pMsg[5] = LOBYTE(LOWORD(pINFO->mask_disp));
	pMsg[6] = 0xff;
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamPMPanTilt(void)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	PVISCA_PM_PARAM pINFO = &stVISCAINFO.pm;
	
	pMsg[2] = LO4BIT(HIBYTE(pINFO->pan));			pMsg[3] = HI4BIT(LOBYTE(pINFO->pan));
	pMsg[4] = LO4BIT(LOBYTE(pINFO->pan));			pMsg[5] = LO4BIT(HIBYTE(pINFO->tilt));
	pMsg[6] = HI4BIT(LOBYTE(pINFO->tilt));			pMsg[7] = LO4BIT(LOBYTE(pINFO->tilt));
	pMsg[8] = 0xff;
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamPMPTZ(PBYTE pBuf)
{
	BYTE  no = pBuf[4];						// mask no : 0 ~ 23
	PBYTE pMsg = stVISCACTRL.TxPacket;
	PVISCA_PM_PARAM pINFO = &stVISCAINFO.pm;
	
	pMsg[2] = LO4BIT(HIBYTE(pINFO->maskpan[no]));	pMsg[3] = HI4BIT(LOBYTE(pINFO->maskpan[no]));
	pMsg[4] = LO4BIT(LOBYTE(pINFO->maskpan[no]));	pMsg[5] = LO4BIT(HIBYTE(pINFO->masktilt[no]));
	pMsg[6] = HI4BIT(LOBYTE(pINFO->masktilt[no]));	pMsg[7] = LO4BIT(LOBYTE(pINFO->masktilt[no]));
	pMsg[8] = HI4BIT(HIBYTE(pINFO->maskzoom[no]));	pMsg[9] = LO4BIT(HIBYTE(pINFO->maskzoom[no]));
	pMsg[10] = HI4BIT(LOBYTE(pINFO->maskzoom[no]));	pMsg[11] = LO4BIT(LOBYTE(pINFO->maskzoom[no]));
	pMsg[12] = 0xff;
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamPMMonitor(void)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	PVISCA_PM_PARAM pINFO = &stVISCAINFO.pm;
	
	pMsg[2] = HIBYTE(HIWORD(pINFO->mask_disp));		pMsg[3] = LOBYTE(HIWORD(pINFO->mask_disp));
	pMsg[4] = HIBYTE(LOWORD(pINFO->mask_disp));		pMsg[5] = LOBYTE(LOWORD(pINFO->mask_disp));
	pMsg[6] = 0xff;
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamID(void)
{
	VISCA_InqReplyWord(HIBYTE(stVISCAINFO.other.cam_id), LOBYTE(stVISCAINFO.other.cam_id));
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamVersion(void)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	WORD fw_ver = __FW_VERSION__;//VISCAAPP_GetFWVer();
	
	pMsg[2] = HIBYTE(VISCA_VENDER_ID);				pMsg[3] = LOBYTE(VISCA_VENDER_ID);		// vender id
	pMsg[4] = HIBYTE(VISCA_MODEL_ID);				pMsg[5] = LOBYTE(VISCA_MODEL_ID);		// model id
	pMsg[6] = HIBYTE(fw_ver);						pMsg[7] = LOBYTE(fw_ver);				// rom version
	pMsg[8] = VISCA_MAX_SOCKET;						pMsg[9] = 0xff;							// max socket
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamMDMode(void)
{
	stVISCAINFO.efunc1.ctrl1 &= (~VISCA_MD_ON);
	stVISCAINFO.efunc1.ctrl1 |= GetMD_OnOff()?  VISCA_MD_ON:  VISCA_MD_OFF;
	VISCA_InqReplyByte((stVISCAINFO.efunc1.ctrl1 & VISCA_MD_ON)?  0x02:  0x03);	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamMDFunction(void)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	PVISCA_MD_PARAM pINFO = &stVISCAINFO.md;
	
	pINFO->det_frame = GetMD_AreaDisp(0)*1 + GetMD_AreaDisp(1)*2 + GetMD_AreaDisp(2)*4 + GetMD_AreaDisp(3)*8;
	pINFO->disp_mode = (pINFO->det_frame)?  0x01:  0x00;
	pINFO->disp_mode |= GetMD_MotionView()?  0x02:  0x00;
	pINFO->thres = GetMD_Threshold()*6;						// 0~40 -> 0~ff
	
	pMsg[2] = pINFO->disp_mode;						pMsg[3] = pINFO->det_frame;
	pMsg[4] = HI4BIT(pINFO->thres);					pMsg[5] = LO4BIT(pINFO->thres);
	pMsg[6] = HI4BIT(pINFO->inter_time);			pMsg[7] = LO4BIT(pINFO->inter_time);
	pMsg[8] = 0xff;
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamMDWindow(PBYTE pBuf)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	PVISCA_MD_PARAM pINFO = &stVISCAINFO.md;
	BYTE no = pBuf[4];
	
	VISCAAPP_GetMDWindow(no, &pINFO->lx[no], &pINFO->ly[no], &pINFO->rx[no], &pINFO->ry[no]);
	
	pMsg[2] = pINFO->lx[no];						pMsg[3] = pINFO->ly[no];
	pMsg[4] = pINFO->rx[no];						pMsg[5] = pINFO->ry[no];
	pMsg[6] = 0xff;
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamZPosRepMode(void)
{
	VISCA_InqReplyByte((stVISCAINFO.user.ctrl & VISCA_ZOOMPOS_REPLY_ON)?  0x02:  0x03);	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamZPosRepInterTime(void)
{
	VISCA_InqReplyWord(0x00, stVISCAINFO.user.zposrep_inter_time);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamFPosRepMode(void)
{
	VISCA_InqReplyByte((stVISCAINFO.user.ctrl & VISCA_FOCUSPOS_REPLY_ON)?  0x02:  0x03);	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamFPosRepInterTime(void)
{
	VISCA_InqReplyWord(0x00, stVISCAINFO.user.fposrep_inter_time);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamRegister(PBYTE pBuf)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	PVISCA_REG_PARAM pINFO = &stVISCAINFO.reg;
	BYTE addr = pBuf[4], data;
	BYTE baud_rate, mon_mode;
	
	baud_rate = VISCAAPP_GetRegViscaBaudrate();	//0(9600) ~ 3(115200)
	mon_mode = VISCAAPP_GetVOUT_Frmt();
	
	switch(addr)	{
		case  0x00:		data = baud_rate;				break;
		case  0x72:		data = mon_mode;				break;
		case  0x73:		data = pINFO->out_en;			break;
		case  0x74:		data = pINFO->lvds_mode;		break;
		case  0x50:		data = pINFO->zoom_widelimit;	break;
		case  0x51:		data = pINFO->zoom_telelimit;	break;
		case  0x52:		data = pINFO->dzoom_max;		break;
		case  0x53:		data = pINFO->stablezoom;		break;
		case  0x54:		data = pINFO->focus_trace;		break;
		case  0x55:		data = pINFO->focus_offset;		break;
		case  0x5F:		data = pINFO->ex_mode;			break;
		default:		data = 0x00;	VISCA_SynERR();	break;	// inq not implemented
	}
	
	pMsg[2] = HI4BIT(data);							pMsg[3] = LO4BIT(data);
	pMsg[4] = 0xff;
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamColorEnhanceLvl(void)
{
	VISCA_SynERR();			// inq not implemented
/*
	PBYTE pMsg = stVISCACTRL.TxPacket;
	PVISCA_COLENH_PARAM pINFO = &stVISCAINFO.color_enhance;
	
	pMsg[2] = pINFO->thres;							pMsg[3] = 0x00;
	pMsg[4] = pINFO->y_high;						pMsg[5] = pINFO->cr_high;
	pMsg[6] = pINFO->cb_high;						pMsg[7] = pINFO->y_low;
	pMsg[8] = pINFO->cr_low;						pMsg[9] = pINFO->cb_low;
	pMsg[10] = 0xff;
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
*/
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamColorEnhanceMode(void)
{
	VISCA_SynERR();			// inq not implemented
//	VISCA_InqReplyByte((stVISCAINFO.color_enhance.onoff & ON)?  0x02:  0x03);	// 2:on, 3:off
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamChromaSuppress(void)
{
	VISCA_SynERR();			// inq not implemented
//	VISCA_InqReplyByte(stVISCAINFO.efunc1.chroma_suppress);
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamColorGain(void)
{
	VISCA_InqReplyWord(0x00, LO4BIT(stVISCAINFO.efunc1.color_gain));	// 0(60%) ~ 0x0E(200%)
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamColorHue(void)
{
	VISCA_InqReplyWord(0x00, LO4BIT(stVISCAINFO.efunc3.color_hue));		// 0(-14) ~ 0x0E(+14)
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamLensTemp(void)
{
	VISCA_SynERR();			// inq not implemented
//	VISCA_InqReplyWord(0x00, stVISCAINFO.user.lens_temp);				// 0xFB(-5) ~ 0(0) ~ 0x3C(60)
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamExExpCompPosi(void)
{
	VISCA_SynERR();			// inq not implemented
//	VISCA_InqReplyWord(0x00, stVISCAINFO.enlarge.expcomp_posi);			// 0(-10.5dB) ~ E(+10.5dB)  ->  00(-128) ~ FF(+127)
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamExAperture(void)
{
	stVISCAINFO.enlarge.aperture_gain = VISCAAPP_GetExAperture();
	VISCA_InqReplyWord(0x00, stVISCAINFO.enlarge.aperture_gain);		// 0( ) ~ F( )  ->  00(-128) ~ FF(+127)
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamExColorGain(void)
{
	VISCA_InqReplyWord(0x00, stVISCAINFO.enlarge.color_gain);			// 0(60%) ~ E(200%)  ->  00(0%) ~ FF(200%)
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamExColorHue(void)
{
	VISCA_InqReplyWord(0x00, stVISCAINFO.enlarge.color_hue);			// 0(-14) ~ E(+14)  ->  00(-14) ~ FF(+14)
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamExAutoICRThres(PBYTE pBuf)
{
	if(pBuf[5] == 0)	VISCA_InqReplyWord(0x00, VISCAAPP_GetExAutoICROnOffThres());	// on(night)->off(day) threshold level, 0 ~ 0xff
	else				VISCA_InqReplyWord(0x00, VISCAAPP_GetExAutoICROffOnThres());	// off(day)->on(night) threshold level, 0 ~ 0x1C
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamExCtrl(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case 0x4E:	VISCA_InqCamExExpCompPosi();		break;
		case 0x42:	VISCA_InqCamExAperture();			break;
		case 0x49:	VISCA_InqCamExColorGain();			break;
		case 0x4F:	VISCA_InqCamExColorHue();			break;
		case 0x21:	VISCA_InqCamExAutoICRThres(pBuf);	break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCamHLC(void)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	BYTE hlc_level, hlc_mask_level;
	
	hlc_level = VISCAAPP_GetHLCLevel();
	hlc_mask_level = VISCAAPP_GetHLCMaskLevel();
	
	pMsg[2] = LO4BIT(hlc_level);					pMsg[3] = LO4BIT(hlc_mask_level);
	pMsg[4] = 0xff;
	
	VISCA_ReplyMsg(VISCA_MSG_INQ_COMPLETE, 0);
}


// 4th byte
//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_CMDInterface(PBYTE pBuf)
{
	switch(pBuf[3])	{
		case 0x01:	VISCA_CMDIFClear(pBuf);		break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_CMDGeneral(PBYTE pBuf)
{
	if((stVISCACTRL.ctrl & VISCA_CMD_S1_PARSED) && (stVISCACTRL.ctrl & VISCA_CMD_S2_PARSED))	{
		stVISCACTRL.ctrl |= VISCA_CMD_NOT_PARSED;		return;
//		VISCA_ReplyMsg(VISCA_MSG_ERR_CMDBUFFULL, 0);	return;				// cmd buffer full !!!
	}
	
	if(stVISCACTRL.ctrl & VISCA_CMD_S1_PARSED)	{				// cmd socket-1 used ??
		memcpy(stVISCACTRL.CmdBuffer[1], pBuf, 16);		stVISCACTRL.ctrl |= VISCA_CMD_S2_PARSED;	// store cmd socket-2
		stVISCACTRL.ctrl &= (~VISCA_CMD_S2_HIGHPRIORITY);		// set socket-1 high priority
//		VISCA_ReplyMsg(VISCA_MSG_ACK, 2);						// ack for cmd socket-2
	}
	else {														// store cmd socket-1 & set socket-2 high priority
		memcpy(stVISCACTRL.CmdBuffer[0], pBuf, 16);		stVISCACTRL.ctrl |= VISCA_CMD_S1_PARSED;	// store cmd socket-1
		stVISCACTRL.ctrl |= VISCA_CMD_S2_HIGHPRIORITY;			// set socket-2 high priority
//		VISCA_ReplyMsg(VISCA_MSG_ACK, 1);						// ack for cmd socket-1
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqInterface(PBYTE pBuf)
{
	switch(pBuf[3])	{
		case 0x02:	VISCA_InqCamVersion();			break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCameraOne(PBYTE pBuf)
{
	switch(pBuf[3])	{
		case 0x00:	VISCA_InqCamPower();			break;
		case 0x47:	VISCA_InqCamZoomPosi();			break;
		case 0x06:	VISCA_InqCamDZoomMode();		break;
		case 0x36:	VISCA_InqCamDZoomCSMode();		break;
		case 0x46:	VISCA_InqCamDZoomPosi();		break;
		case 0x38:	VISCA_InqCamFocusMode();		break;
		case 0x48:	VISCA_InqCamFocusPosi();		break;
		case 0x28:	VISCA_InqCamFocusNearLimit();	break;
		case 0x58:	VISCA_InqCamAFSensitivity();	break;
		case 0x57:	VISCA_InqCamAFMode();			break;
		case 0x27:	VISCA_InqCamAFTimeSetting();	break;
		case 0x11:	VISCA_InqCamIRCorrection();		break;
		case 0x35:	VISCA_InqCamWBMode();			break;
		case 0x43:	VISCA_InqCamWBGainR();			break;
		case 0x44:	VISCA_InqCamWBGainB();			break;
		case 0x39:	VISCA_InqCamAEMode();			break;
		case 0x5A:	VISCA_InqCamAutoSlowShutter();	break;
		case 0x4A:	VISCA_InqCamShutterPosi();		break;
		case 0x4B:	VISCA_InqCamIrisPosi();			break;
		case 0x4C:	VISCA_InqCamGainPosi();			break;
		case 0x2C:	VISCA_InqCamGainLimit();		break;
		case 0x4D:	VISCA_InqCamBrightPosi();		break;
		case 0x3E:	VISCA_InqCamExpCompMode();		break;
		case 0x4E:	VISCA_InqCamExpCompPosi();		break;
		case 0x33:	VISCA_InqCamBackLightMode();	break;
		case 0x59:	VISCA_InqCamSpotAEMode();		break;
		case 0x29:	VISCA_InqCamSpotAEPosi();		break;
		case 0x3D:	VISCA_InqCamWDMode();			break;
		case 0x2D:	VISCA_InqCamWDParameter();		break;
		case 0x5D:	VISCA_InqCamAEResponse();		break;
		case 0x37:	VISCA_InqCamDefog();			break;
		case 0x42:	VISCA_InqCamAperture();			break;
		case 0x52:	VISCA_InqCamHRMode();			break;
		case 0x53:	VISCA_InqCamNRMode();			break;
		case 0x5B:	VISCA_InqCamGamma();			break;
		case 0x5E:	VISCA_InqCamHighSens();			break;
		case 0x61:	VISCA_InqCamLRRerverseMode();	break;
		case 0x62:	VISCA_InqCamFreezeMode();		break;
		case 0x63:	VISCA_InqCamPicEffectMode();	break;
		case 0x66:	VISCA_InqCamPicFlipMode();		break;
		case 0x01:	VISCA_InqCamICRMode();			break;
		case 0x51:	VISCA_InqCamAutoICRMode();		break;
		case 0x21:	VISCA_InqCamAutoICRThres();		break;
		case 0x31:	VISCA_InqCamAutoICRAlarm();		break;
		case 0x3F:	VISCA_InqCamMemory();			break;
		case 0x23:	VISCA_InqCamMemSave(pBuf);		break;
		case 0x15:	VISCA_InqCamDisplayMode();		break;
		case 0x34:	VISCA_InqCamStabilizerMode();	break;
		case 0x75:	VISCA_InqCamMuteMode();			break;
		case 0x77:	VISCA_InqCamPMMaskDisplay();	break;
		case 0x79:	VISCA_InqCamPMPanTilt();		break;
		case 0x7B:	VISCA_InqCamPMPTZ(pBuf);		break;
		case 0x6F:	VISCA_InqCamPMMonitor();		break;
		case 0x22:	VISCA_InqCamID();				break;
		case 0x1B:	VISCA_InqCamMDMode();			break;
		case 0x1C:	VISCA_InqCamMDFunction();		break;
		case 0x1D:	VISCA_InqCamMDWindow(pBuf);		break;
		case 0x69:	VISCA_InqCamZPosRepMode();		break;
		case 0x6A:	VISCA_InqCamZPosRepInterTime();	break;
		case 0x16:	VISCA_InqCamFPosRepMode();		break;
		case 0x1A:	VISCA_InqCamFPosRepInterTime();	break;
		case 0x24:	VISCA_InqCamRegister(pBuf);		break;
		case 0x20:	VISCA_InqCamColorEnhanceLvl();	break;
		case 0x50:	VISCA_InqCamColorEnhanceMode();	break;
		case 0x5F:	VISCA_InqCamChromaSuppress();	break;
		case 0x49:	VISCA_InqCamColorGain();		break;
		case 0x4F:	VISCA_InqCamColorHue();			break;
		case 0x68:	VISCA_InqCamLensTemp();			break;
		case 0x1F:	VISCA_InqCamExCtrl(pBuf);		break;
		case 0x14:	VISCA_InqCamHLC();				break;
		default:	VISCA_SynERR();					break;		// inq not implemented
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqPanTilt(PBYTE pBuf)
{
	switch(pBuf[3])	{
		case 0x06:	VISCA_InqCamDisplayMode();		break;
		default:	VISCA_SynERR();					break;		// inq not implemented
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCameraTwo(PBYTE pBuf)
{
	VISCA_SynERR();					// inq not implemented
//	UARTprintf("Cam two inquiry not defined ... \r\n");
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqBlock(PBYTE pBuf)
{
	switch(pBuf[3])	{
		case 0x7E:	VISCA_InqBlockCommand(pBuf);	break;
		default:	VISCA_SynERR();					break;	// inq not implemented
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_InqCameraMIT(PBYTE pBuf)			// inquiry for MIT(not compatible with sony cam)
{
	switch(pBuf[3])	{
		case 0x01:	VISCA_InqCamAutoICRDelay();		break;
		default:	VISCA_SynERR();					break;	// inq not implemented
	}
}

// 3rd byte(category code)
//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_PacketCommand(PBYTE pBuf)
{
	switch(pBuf[2])	{
		case 0x00:		VISCA_CMDInterface(pBuf);	break;
		case 0x04:
		case 0x06:
		case 0x07:		VISCA_CMDGeneral(pBuf);		break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_PacketInquiry(PBYTE pBuf)
{
	if((stVISCACTRL.ctrl & VISCA_CMD_S1_PARSED) || (stVISCACTRL.ctrl & VISCA_CMD_S2_PARSED))	{	// cmd buffer not blank ?
		stVISCACTRL.ctrl |= VISCA_INQ_NOT_EXCUTED;		return;
	}
	
	switch(pBuf[2])	{
		case 0x00:		VISCA_InqInterface(pBuf);	break;
		case 0x04:		VISCA_InqCameraOne(pBuf);	break;
		case 0x06:		VISCA_InqPanTilt(pBuf);		break;
		case 0x07:		VISCA_InqCameraTwo(pBuf);	break;
		case 0x7E:		VISCA_InqBlock(pBuf);		break;
		case 0xA1:		VISCA_InqCameraMIT(pBuf);	break;		// inquiry for MIT(not compatible with sony cam)
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_CMDCancel(PBYTE pBuf)
{
	BYTE sn = pBuf[1]&0x0F;			// target socket no.(1,2)
	
	if(stVISCACTRL.sn == sn)	{VISCA_ReplyMsg(VISCA_MSG_ERR_NOSOCKET, sn);	return;}	// 'target cmd socket' is in use by main handler
	
	switch(sn)	{
		case 1:	if(stVISCACTRL.ctrl&VISCA_CMD_S1_PARSED)	{
					memset((PBYTE)stVISCACTRL.CmdBuffer[0],  0,  16);								// clear cmd buffer
					VISCA_ReplyMsg(VISCA_MSG_ERR_CMDCANCEL, sn);
				}
				else VISCA_ReplyMsg(VISCA_MSG_ERR_NOSOCKET, sn);					// no cmd to cancel
				break;
		
		case 2:	if(stVISCACTRL.ctrl&VISCA_CMD_S2_PARSED)	{
					memset((PBYTE)stVISCACTRL.CmdBuffer[1],  0,  16);								// clear cmd buffer
					VISCA_ReplyMsg(VISCA_MSG_ERR_CMDCANCEL, sn);
				}
				else VISCA_ReplyMsg(VISCA_MSG_ERR_NOSOCKET, sn);					// no cmd to cancel
				break;
	}
}


// 1st(cam addr) & 2nd byte(command/inquiry)
//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_PacketDecoder(PBYTE pBuf)
{
	BYTE rxaddr = pBuf[0] & 0x0F;										// 1st byte(cam address)

	if((rxaddr != stVISCACTRL.cam_addr) && (rxaddr != 8))		return;	// cam address check
	
	switch(pBuf[1])	{												// 2nd byte(command/inquiry)
		case 0x01:		VISCA_PacketCommand(pBuf);		break;
		case 0x09:		VISCA_PacketInquiry(pBuf);		break;
		case 0x30:		VISCA_AddressSet(pBuf);			break;
		case 0x21:
		case 0x22:		VISCA_CMDCancel(pBuf);			break;
	}
}


//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamPower(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_SynERR();	VISCAAPP_PowerOnOff(ON);					break;	// cmd not implemented
		case  0x03:		VISCA_ACK();	VISCAAPP_PowerOnOff(OFF);	VISCA_COMP();	break;
		case  0x0C:		case  0x0D:
		case  0x13:		case  0x04:
		case  0x20:		VISCA_ACK();	VISCAAPP_MaintenanceMode(pBuf[4]);	VISCA_COMP();	break;
		default:		VISCA_SynERR();														break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamZoom(PBYTE pBuf)
{
	if		((pBuf[4]&0xff)==0x00)	{VISCA_ACK();	VISCAAPP_ZoomStop();	VISCA_COMP();}
	else if ((pBuf[4]&0xff)==0x02)	{VISCA_ACK();	VISCAAPP_ZoomTeleStd();	VISCA_COMP();}
	else if ((pBuf[4]&0xff)==0x03)	{VISCA_ACK();	VISCAAPP_ZoomWideStd();	VISCA_COMP();}

	else if ((pBuf[4]&0xf0)==0x20)	{VISCA_ACK();	VISCAAPP_ZoomTeleVar(pBuf[4]&0x0f);	VISCA_COMP();}
	else if ((pBuf[4]&0xf0)==0x30)	{VISCA_ACK();	VISCAAPP_ZoomWideVar(pBuf[4]&0x0f);	VISCA_COMP();}
	else							{VISCA_SynERR();	return;}

	if (HI4BIT(pBuf[4])==0)
		 stVISCACTRL.RepeatCmd = MAKEWORD(pBuf[3], pBuf[4]&0xff);
	else stVISCACTRL.RepeatCmd = MAKEWORD(pBuf[3], pBuf[4]&0xf0);

	fVISCA_CmdParsed = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamDZoom(PBYTE pBuf)
{
	if		((pBuf[4]&0xff)==0x00)	{VISCA_ACK();	VISCAAPP_DZoomStop();		VISCA_COMP();}
	else if ((pBuf[4]&0xff)==0x02)	{VISCA_ACK();	VISCAAPP_DZoomOnOff(ON);	VISCA_COMP();}
	else if ((pBuf[4]&0xff)==0x03)	{VISCA_ACK();	VISCAAPP_DZoomOnOff(OFF);	VISCA_COMP();}
	else if ((pBuf[4]&0xff)==0x10)	{VISCA_ACK();	VISCAAPP_DZoomSwitch();		VISCA_COMP();}

	else if ((pBuf[4]&0xf0)==0x20)	{VISCA_ACK();	VISCAAPP_DZoomTeleVar(pBuf[4]&0x0f);	VISCA_COMP();}
	else if ((pBuf[4]&0xf0)==0x30)	{VISCA_ACK();	VISCAAPP_DZoomWideVar(pBuf[4]&0x0f);	VISCA_COMP();}
	else							{VISCA_SynERR();	return;}

	if (HI4BIT(pBuf[4])==0)
		 stVISCACTRL.RepeatCmd = MAKEWORD(pBuf[3], pBuf[4]&0xff);
	else stVISCACTRL.RepeatCmd = MAKEWORD(pBuf[3], pBuf[4]&0xf0);

	fVISCA_CmdParsed = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamDZoomCombine(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_DZoomCombine(ON);	VISCA_COMP();	break;
		case  0x01:		VISCA_ACK();	VISCAAPP_DZoomCombine(OFF);	VISCA_COMP();	break;
		default:		VISCA_SynERR();												break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamDZoomDirect(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)		{VISCA_ACK();	VISCAAPP_DZoomDirect(MAKEBYTE(pBuf[6], pBuf[7]));	VISCA_COMP();}
	else					VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamFocus(PBYTE pBuf)
{
	if		((pBuf[4]&0xff)==0x00)	{VISCA_ACK();	VISCAAPP_FocusStop();		VISCA_COMP();}
	else if ((pBuf[4]&0xff)==0x02)	{VISCA_ACK();	VISCAAPP_FocusFarStd();		VISCA_COMP();}
	else if ((pBuf[4]&0xff)==0x03)	{VISCA_ACK();	VISCAAPP_FocusNearStd();	VISCA_COMP();}

	else if ((pBuf[4]&0xf0)==0x20)	{VISCA_ACK();	VISCAAPP_FocusFarVar(pBuf[4]&0x0f);		VISCA_COMP();}
	else if ((pBuf[4]&0xf0)==0x30)	{VISCA_ACK();	VISCAAPP_FocusNearVar(pBuf[4]&0x0f);	VISCA_COMP();}
	else							{VISCA_SynERR();	return;}

	if (HI4BIT(pBuf[4])==0)
		 stVISCACTRL.RepeatCmd = MAKEWORD(pBuf[3], pBuf[4]&0xff);
	else stVISCACTRL.RepeatCmd = MAKEWORD(pBuf[3], pBuf[4]&0xf0);

	fVISCA_CmdParsed = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamFocusDirect(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)		{
		VISCA_ACK();
		VISCAAPP_FocusDirect(MAKEWORD(MAKEBYTE(pBuf[4], pBuf[5]), MAKEBYTE(pBuf[6], pBuf[7])));
		VISCA_COMP();
	}
	else	{VISCA_SynERR();	return;}
	
	stVISCACTRL.RepeatCmd = MAKEWORD(pBuf[3], 0x0D);
	fVISCA_CmdParsed = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamFocusAutoOnOff(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_ACK();	VISCAAPP_FocusAutoOnOff(ON);	VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_FocusAutoOnOff(OFF);	VISCA_COMP();	break;
		case  0x10:		VISCA_ACK();	VISCAAPP_FocusAutoManual();		VISCA_COMP();	break;
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamFocusOnepush(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x01:		VISCA_ACK();	VISCAAPP_FocusOnepushTrig();	VISCA_COMP();	break;
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamFocusNearLimit(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)		{
		VISCA_ACK();
		VISCAAPP_FocusNearLimit(MAKEWORD(MAKEBYTE(pBuf[4], pBuf[5]), MAKEBYTE(pBuf[6], pBuf[7])));
		VISCA_COMP();
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamAFSensitivity(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_SynERR();	VISCAAPP_AFSensitivity(HIGH);	break;	// cmd not implemented
		case  0x03:		VISCA_SynERR();	VISCAAPP_AFSensitivity(LOW);	break;	// cmd not implemented
		default:		VISCA_SynERR();									break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamAFMode(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_AFMode(0);				VISCA_COMP();	break;
		case  0x01:		VISCA_ACK();	VISCAAPP_AFMode(1);				VISCA_COMP();	break;
		case  0x02:		VISCA_ACK();	VISCAAPP_AFMode(2);				VISCA_COMP();	break;
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamAFModeInterTime(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)		{
		VISCA_ACK();
		VISCAAPP_AFIntervalTime(MAKEBYTE(pBuf[4], pBuf[5]), MAKEBYTE(pBuf[6], pBuf[7]));
		VISCA_COMP();
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamIRCorrection(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_SynERR();	VISCAAPP_IRCorrectionStandard();	break;	// cmd not implemented
		case  0x01:		VISCA_SynERR();	VISCAAPP_IRCorrectionIRLight();		break;	// cmd not implemented
		default:		VISCA_SynERR();										break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamZoomFocusDirect(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)		{								// zoom direct
		VISCA_ACK();
		VISCAAPP_ZoomDirect(MAKEWORD(MAKEBYTE(pBuf[4], pBuf[5]), MAKEBYTE(pBuf[6], pBuf[7])));
		VISCA_COMP();
		stVISCACTRL.RepeatCmd = MAKEWORD(pBuf[3], 0x0D);
	}
	else if(pBuf[12] == 0xff)	{												// zoom & focus direct
		VISCA_ACK();
		VISCAAPP_ZoomFocusDirect(MAKEWORD(MAKEBYTE(pBuf[4], pBuf[5]), MAKEBYTE(pBuf[6], pBuf[7])),
								MAKEWORD(MAKEBYTE(pBuf[8], pBuf[9]), MAKEBYTE(pBuf[10], pBuf[11])));
		VISCA_COMP();
		stVISCACTRL.RepeatCmd = MAKEWORD(pBuf[3], 0x0F);
	}
	else	{VISCA_SynERR();	return;}
	
	fVISCA_CmdParsed = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamInitialize(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x01:		VISCA_SynERR();	VISCAAPP_LensInit();						break;	// cmd not implemented
		case  0x03:		VISCA_SynERR();	VISCAAPP_CameraInit();						break;	// cmd not implemented
		default:		VISCA_SynERR();												break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamWB(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_WBMode(0);	VISCA_COMP();			break;
		case  0x01:		VISCA_ACK();	VISCAAPP_WBMode(1);	VISCA_COMP();			break;
		case  0x02:		VISCA_ACK();	VISCAAPP_WBMode(2);	VISCA_COMP();			break;
		case  0x03:		VISCA_ACK();	VISCAAPP_WBMode(3);	VISCA_COMP();			break;
		case  0x04:		VISCA_ACK();	VISCAAPP_WBMode(4);	VISCA_COMP();			break;
		case  0x05:		VISCA_ACK();	VISCAAPP_WBMode(5);	VISCA_COMP();			break;
		case  0x06:		VISCA_SynERR();												break;	// cmd not implemented
		case  0x07:		VISCA_SynERR();												break;	// cmd not implemented
		case  0x08:		VISCA_SynERR();												break;	// cmd not implemented
		case  0x09:		VISCA_SynERR();												break;	// cmd not implemented
		default:		VISCA_SynERR();												break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamWBOnePush(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x05:		VISCA_ACK();	VISCAAPP_WBOnepushTrig();	VISCA_COMP();	break;
		default:		VISCA_SynERR();												break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamWBRGain(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_WBRGainReset();	VISCA_COMP();	break;
		case  0x02:		VISCA_ACK();	VISCAAPP_WBRGainUp();		VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_WBRGainDown();		VISCA_COMP();	break;
		default:		VISCA_SynERR();												break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamWBRGainDirect(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)		{VISCA_ACK();	VISCAAPP_WBRGainDirect(MAKEBYTE(pBuf[6], pBuf[7]));	VISCA_COMP();}
	else					VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamWBBGain(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_WBBGainReset();	VISCA_COMP();	break;
		case  0x02:		VISCA_ACK();	VISCAAPP_WBBGainUp();		VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_WBBGainDown();		VISCA_COMP();	break;
		default:		VISCA_SynERR();												break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamWBBGainDirect(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)		{VISCA_ACK();	VISCAAPP_WBBGainDirect(MAKEBYTE(pBuf[6], pBuf[7]));	VISCA_COMP();}
	else					VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamAE(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_AEMode(0x00);	VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_AEMode(0x03);	VISCA_COMP();	break;
		case  0x0A:		VISCA_ACK();	VISCAAPP_AEMode(0x0A);	VISCA_COMP();	break;
		case  0x0B:		VISCA_ACK();	VISCAAPP_AEMode(0x0B);	VISCA_COMP();	break;
		case  0x0D:		VISCA_ACK();	VISCAAPP_AEMode(0x0D);	VISCA_COMP();	break;
		default:		VISCA_SynERR();											break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamAutoSlowShutter(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x11:		VISCA_ACK();	VISCAAPP_AutoSlowShutterSetLimit(1);	VISCA_COMP();		break; // 1/30(1/25)
		case  0x12:		VISCA_ACK();	VISCAAPP_AutoSlowShutterSetLimit(2);	VISCA_COMP();		break; // 1/15(1/12)
		case  0x13:		VISCA_ACK();	VISCAAPP_AutoSlowShutterSetLimit(3);	VISCA_COMP();		break; // 1/8(1/6)
		case  0x14:		VISCA_ACK();	VISCAAPP_AutoSlowShutterSetLimit(4);	VISCA_COMP();		break; // 1/4(1/3)
		case  0x15:		VISCA_ACK();	VISCAAPP_AutoSlowShutterSetLimit(5);	VISCA_COMP();		break; // 1/2(1/2)
		case  0x16:		VISCA_ACK();	VISCAAPP_AutoSlowShutterSetLimit(6);	VISCA_COMP();		break; // 1/1(1/1)
		case  0x02:		VISCA_ACK();	VISCAAPP_AutoSlowShutterOnOff(ON);	VISCA_COMP();		break;
		case  0x03:		VISCA_ACK();	VISCAAPP_AutoSlowShutterOnOff(OFF);	VISCA_COMP();		break;
		default:		VISCA_SynERR();												break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamShutter(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_ShutterReset();	VISCA_COMP();	break;
		case  0x02:		VISCA_ACK();	VISCAAPP_ShutterUp();		VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_ShutterDown();		VISCA_COMP();	break;
		default:		VISCA_SynERR();												break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamShutterDirect(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)		{VISCA_ACK();	VISCAAPP_ShutterDirect(MAKEBYTE(pBuf[6], pBuf[7]));	VISCA_COMP();}
	else					VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamIris(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_IrisReset();	VISCA_COMP();		break;
		case  0x02:		VISCA_ACK();	VISCAAPP_IrisUp();		VISCA_COMP();		break;
		case  0x03:		VISCA_ACK();	VISCAAPP_IrisDown();	VISCA_COMP();		break;
		default:		VISCA_SynERR();												break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamIrisDirect(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)		{VISCA_ACK();	VISCAAPP_IrisDirect(MAKEBYTE(pBuf[6], pBuf[7]));	VISCA_COMP();}
	else					VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamGain(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_GainReset();	VISCA_COMP();		break;
		case  0x02:		VISCA_ACK();	VISCAAPP_GainUp();		VISCA_COMP();		break;
		case  0x03:		VISCA_ACK();	VISCAAPP_GainDown();	VISCA_COMP();		break;
		default:		VISCA_SynERR();												break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamGainDirect(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)		{VISCA_ACK();	VISCAAPP_GainDirect(MAKEBYTE(pBuf[6], pBuf[7]));	VISCA_COMP();}
	else					VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamGainLimit(PBYTE pBuf)
{
	if(pBuf[5] == 0xff)		{VISCA_ACK();	VISCAAPP_GainLimit(pBuf[4]&0x0f);	VISCA_COMP();}
	else					VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamBright(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_BrightReset();		VISCA_COMP();	break;
		case  0x02:		VISCA_ACK();	VISCAAPP_BrightUp();		VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_BrightDown();		VISCA_COMP();	break;
		default:		VISCA_SynERR();												break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamBrightDirect(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)		{VISCA_ACK();	VISCAAPP_BrightDirect(MAKEBYTE(pBuf[6], pBuf[7]));	VISCA_COMP();}
	else					VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamExpComp(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_ACK();	VISCAAPP_ExpCompOnOff(ON);		VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_ExpCompOnOff(OFF);		VISCA_COMP();	break;
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamExpCompAmountSet(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_ExpCompReset();	VISCA_COMP();		break;
		case  0x02:		VISCA_ACK();	VISCAAPP_ExpCompUp();		VISCA_COMP();		break;
		case  0x03:		VISCA_ACK();	VISCAAPP_ExpCompDown();		VISCA_COMP();		break;
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamExpCompDirect(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)		{VISCA_ACK();	VISCAAPP_ExpCompDirect(MAKEBYTE(pBuf[6], pBuf[7]));	VISCA_COMP();}
	else					VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamBacklight(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_ACK();	VISCAAPP_BacklightOnOff(ON);	VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_BacklightOnOff(OFF);	VISCA_COMP();	break;
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamSpotAE(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_ACK();	VISCAAPP_SpotAEOnOff(ON);		VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_SpotAEOnOff(OFF);		VISCA_COMP();	break;
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamSpotAEPosition(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)		{
		VISCA_ACK();
		VISCAAPP_SpotAEPosition(MAKEBYTE(pBuf[4], pBuf[5]), MAKEBYTE(pBuf[6], pBuf[7]));
		VISCA_COMP();
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamAEResponse(PBYTE pBuf)
{
	if(pBuf[5] == 0xff)		{VISCA_ACK();	VISCAAPP_AEResponse(pBuf[4]);	VISCA_COMP();}
	else					VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamWD(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_ACK();	VISCAAPP_WDOnOff(ON);	VISCAAPP_WDVEOnOff(OFF);	VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_WDOnOff(OFF);	VISCAAPP_WDVEOnOff(OFF);	VISCA_COMP();	break;
		case  0x06:		VISCA_SynERR();	VISCAAPP_WDVEOnOff(ON);												break;	// cmd not implemented
		default:		VISCA_SynERR();																		break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamWDSetParam(PBYTE pBuf)
{
	if(pBuf[12] == 0xff)		{VISCA_SynERR();	VISCAAPP_WDSetParameter(pBuf[5], pBuf[6], pBuf[7]);	}	// cmd not implemented
	else						VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamDefog(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_ACK();	VISCAAPP_Defog(MAKEBYTE(ON, pBuf[5]));	VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_Defog(OFF);					VISCA_COMP();	break;
		default:		VISCA_SynERR();															break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamAperture(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_ApertureReset();	VISCA_COMP();		break;
		case  0x02:		VISCA_ACK();	VISCAAPP_ApertureUp();		VISCA_COMP();		break;
		case  0x03:		VISCA_ACK();	VISCAAPP_ApertureDown();	VISCA_COMP();		break;
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamApertureDirect(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)		{VISCA_ACK();	VISCAAPP_ApertureDirect(MAKEBYTE(pBuf[6], pBuf[7]));	VISCA_COMP();}
	else					VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamHR(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_SynERR();	VISCAAPP_HROnOff(ON);				break;
		case  0x03:		VISCA_SynERR();	VISCAAPP_HROnOff(OFF);				break;
		default:		VISCA_SynERR();										break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamNR(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_NRSet(OFF);	VISCA_COMP();	break;
		case  0x01:		VISCA_ACK();	VISCAAPP_NRSet(1);		VISCA_COMP();	break;
		case  0x02:		VISCA_ACK();	VISCAAPP_NRSet(2);		VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_NRSet(3);		VISCA_COMP();	break;
		case  0x04:		VISCA_SynERR();											break;	// cmd not implemented
		case  0x05:		VISCA_SynERR();											break;	// cmd not implemented
		default:		VISCA_SynERR();											break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamGamma(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_SynERR();	VISCAAPP_GammaStandard();				break;	// cmd not implemented
		case  0x01:		VISCA_SynERR();	VISCAAPP_GammaStraight();				break;	// cmd not implemented
		default:		VISCA_SynERR();											break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamHighSensitivity(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_SynERR();	VISCAAPP_HighSensitivityOnOff(ON);		break;
		case  0x03:		VISCA_SynERR();	VISCAAPP_HighSensitivityOnOff(OFF);		break;
		default:		VISCA_SynERR();											break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamLRReverse(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_ACK();	VISCAAPP_LRReverseOnOff(ON);	VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_LRReverseOnOff(OFF);	VISCA_COMP();	break;
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamFreeze(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_ACK();	VISCAAPP_FreezeOnOff(ON);		VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_FreezeOnOff(OFF);		VISCA_COMP();	break;
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamPictureEffect(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_PictureEffectOff();	VISCA_COMP();	break;
		case  0x02:		VISCA_ACK();	VISCAAPP_PictureEffectNeg();	VISCA_COMP();	break;
		case  0x04:		VISCA_ACK();	VISCAAPP_PictureEffectBW();		VISCA_COMP();	break;
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamPictureFlip(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_ACK();	VISCAAPP_PictureFlipOnOff(ON);	VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_PictureFlipOnOff(OFF);	VISCA_COMP();	break;
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamICR(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_ACK();	VISCAAPP_ICROnOff(ON);		VISCA_COMP();		break;
		case  0x03:		VISCA_ACK();	VISCAAPP_ICROnOff(OFF);		VISCA_COMP();		break;
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamAutoICR(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_ACK();	VISCAAPP_AutoICROnOff(ON);	VISCA_COMP();		break;
		case  0x03:		VISCA_ACK();	VISCAAPP_AutoICROnOff(OFF);	VISCA_COMP();		break;
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamAutoICRThres(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)		{VISCA_ACK();	VISCAAPP_AutoICROnOffThres(MAKEBYTE(pBuf[6], pBuf[7]));	VISCA_COMP();}
	else					VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamAutoICRAlarmReply(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_ACK();	VISCAAPP_ICRAlarmReplyOnOff(ON);	VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_ICRAlarmReplyOnOff(OFF);	VISCA_COMP();	break;
		default:		VISCA_SynERR();														break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamStabilizer(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_SynERR();	VISCAAPP_StabilizerHold();						break;	// cmd not implemented
		case  0x02:		VISCA_ACK();	VISCAAPP_StabilizerOnOff(ON);	VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_StabilizerOnOff(OFF);	VISCA_COMP();	break;
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamMemory(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_MemoryReset(pBuf[5]&0x0f);		VISCA_COMP();	break;
		case  0x01:		VISCA_ACK();	VISCAAPP_MemorySet(pBuf[5]&0x0f);		VISCA_COMP();	break;
		case  0x02:		VISCA_ACK();	VISCAAPP_MemoryRecall(pBuf[5]&0x0f);	VISCA_COMP();	break;
		default:		VISCA_SynERR();															break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamCustom(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_SynERR();	VISCAAPP_CustomReset();							break;	// cmd not implemented
		case  0x01:		VISCA_SynERR();	VISCAAPP_CustomSet();							break;	// cmd not implemented
		case  0x02:		VISCA_SynERR();	VISCAAPP_CustomRecall();						break;	// cmd not implemented
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamMemoryCustom(PBYTE pBuf)
{
	switch(pBuf[5]&0xf0)	{
		case 0x00:		VISCA_ExeCamMemory(pBuf);										break;
		case 0x70:		VISCA_ExeCamCustom(pBuf);										break;
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamMemSave(PBYTE pBuf)
{
	if(pBuf[9] == 0xff)		{
		VISCA_ACK();
		VISCAAPP_MemSave(pBuf[4]&0x0f, MAKEWORD(MAKEBYTE(pBuf[5], pBuf[6]), MAKEBYTE(pBuf[7], pBuf[8])));
		VISCA_COMP();
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamDisplay(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_ACK();	VISCAAPP_DisplayOnOff(ON);		VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_DisplayOnOff(OFF);		VISCA_COMP();	break;
		case  0x10:		VISCA_ACK();	VISCAAPP_DisplayToggle();		VISCA_COMP();	break;
		default:		VISCA_SynERR();													break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamMultiLineTitle(PBYTE pBuf)
{
	switch(pBuf[4]&0xf0)	{
		case  0x10:		VISCA_ACK();	VISCAAPP_MultiLineTitleSet1(pBuf[4]&0x0f, pBuf[6], pBuf[7], pBuf[8]);	VISCA_COMP();	break;
		case  0x20:		VISCA_ACK();	VISCAAPP_MultiLineTitleSet2(pBuf[4]&0x0f, &pBuf[5]);					VISCA_COMP();	break;
		case  0x30:		VISCA_ACK();	VISCAAPP_MultiLineTitleSet3(pBuf[4]&0x0f, &pBuf[5]);					VISCA_COMP();	break;
		default:		VISCA_SynERR();																							break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamMultiLineTitleCtrl(PBYTE pBuf)
{
	switch(pBuf[4]&0xf0)	{
		case  0x10:		VISCA_ACK();	VISCAAPP_MultiLineTitleClear(pBuf[4]&0x0f);	VISCA_COMP();	break;
		case  0x20:		VISCA_ACK();	VISCAAPP_MultiLineTitleOn(pBuf[4]&0x0f);	VISCA_COMP();	break;
		case  0x30:		VISCA_ACK();	VISCAAPP_MultiLineTitleOff(pBuf[4]&0x0f);	VISCA_COMP();	break;
		default:		VISCA_SynERR();																break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamMute(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_ACK();	VISCAAPP_MuteOnOff(ON);		VISCA_COMP();			break;
		case  0x03:		VISCA_ACK();	VISCAAPP_MuteOnOff(OFF);	VISCA_COMP();			break;
		case  0x10:		VISCA_ACK();	VISCAAPP_MuteToggle();		VISCA_COMP();			break;
		default:		VISCA_SynERR();														break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamPMSetMask(PBYTE pBuf)
{
	if(pBuf[10] == 0xff)	{
		VISCA_ACK();
		VISCAAPP_PMSetMask(pBuf[4], pBuf[5], MAKEBYTE(pBuf[6], pBuf[7]), MAKEBYTE(pBuf[8], pBuf[9]));
		VISCA_COMP();
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamPMDisplay(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)	{
		VISCA_ACK();
		VISCAAPP_PMDisplay(MAKEDWORD(MAKEWORD(pBuf[4], pBuf[5]), MAKEWORD(pBuf[6], pBuf[7])));
		VISCA_COMP();
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamPMSetMaskColor(PBYTE pBuf)
{
	if(pBuf[10] == 0xff)	{
		VISCA_ACK();
		VISCAAPP_PMSetMaskColor(MAKEDWORD(MAKEWORD(pBuf[4], pBuf[5]), MAKEWORD(pBuf[6], pBuf[7])), 
									pBuf[8], pBuf[9]);
		VISCA_COMP();
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamPMSetPanTiltAngle(PBYTE pBuf)
{
	if(pBuf[10] == 0xff)	{
		VISCA_ACK();
		VISCAAPP_PMSetPanTiltAngle(MAKEWORD(MAKEBYTE(0, pBuf[4]), MAKEBYTE(pBuf[5], pBuf[6])), 
									MAKEWORD(MAKEBYTE(0, pBuf[7]), MAKEBYTE(pBuf[8], pBuf[9])));
		VISCA_COMP();
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamPMSetPTZMask(PBYTE pBuf)
{
	if(pBuf[15] == 0xff)	{
		VISCA_ACK();
		VISCAAPP_PMSetPTZMask(pBuf[4], MAKEWORD(MAKEBYTE(0, pBuf[5]), MAKEBYTE(pBuf[6], pBuf[7])), 
									MAKEWORD(MAKEBYTE(0, pBuf[8]), MAKEBYTE(pBuf[9], pBuf[10])), 
									MAKEWORD(MAKEBYTE(pBuf[11], pBuf[12]), MAKEBYTE(pBuf[13], pBuf[14])));
		VISCA_COMP();
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamPMNonInterlockMask(PBYTE pBuf)
{
	if(pBuf[13] == 0xff)	{
		VISCA_ACK();
		VISCAAPP_PMNonInterlockMask(pBuf[4], MAKEBYTE(pBuf[5], pBuf[6]), MAKEBYTE(pBuf[7], pBuf[8]), 
											MAKEBYTE(pBuf[9], pBuf[10]), MAKEBYTE(pBuf[11], pBuf[12]));
		VISCA_COMP();
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamPMCenterLine(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x04:		VISCA_SynERR();	VISCAAPP_PMCenterLineOnOff(ON);				break;	// cmd not implemented
		case  0x03:		VISCA_SynERR();	VISCAAPP_PMCenterLineOnOff(OFF);			break;	// cmd not implemented
		default:		VISCA_SynERR();												break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamIDWrite(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)	{
		VISCA_ACK();
		VISCAAPP_CamIDWrite(MAKEWORD(MAKEBYTE(pBuf[4], pBuf[5]), MAKEBYTE(pBuf[6], pBuf[7])));
		VISCA_COMP();
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamMD(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_ACK();	VISCAAPP_MDOnOff(ON);	VISCA_COMP();		break;
		case  0x03:		VISCA_ACK();	VISCAAPP_MDOnOff(OFF);	VISCA_COMP();		break;
		default:		VISCA_SynERR();												break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamMDFunctionSet(PBYTE pBuf)
{
	if(pBuf[10] == 0xff)	{
		VISCA_ACK();
		VISCAAPP_MDFunctionSet(pBuf[4], pBuf[5], MAKEBYTE(pBuf[6], pBuf[7]), MAKEBYTE(pBuf[8], pBuf[9]));
		VISCA_COMP();
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamMDWindowSet(PBYTE pBuf)
{
	if(pBuf[9] == 0xff)	{
		VISCA_ACK();
		VISCAAPP_MDWindowSet(pBuf[4], pBuf[5], pBuf[6], pBuf[7], pBuf[8]);
		VISCA_COMP();
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamContZoomPosReply(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_SynERR();	VISCAAPP_ContZoomPosReplyOnOff(ON);			break;	// cmd not implemented
		case  0x03:		VISCA_SynERR();	VISCAAPP_ContZoomPosReplyOnOff(OFF);		break;	// cmd not implemented
		default:		VISCA_SynERR();												break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamZoomPosReplyInterTime(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)	{
		VISCA_SynERR();
		VISCAAPP_ZoomPosReplyInterTimeSet(MAKEBYTE(pBuf[6], pBuf[7]));	// cmd not implemented
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamContFocusPosReply(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_SynERR();	VISCAAPP_ContZoomPosReplyOnOff(ON);			break;	// cmd not implemented
		case  0x03:		VISCA_SynERR();	VISCAAPP_ContZoomPosReplyOnOff(OFF);		break;	// cmd not implemented
		default:		VISCA_SynERR();												break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamFocusPosReplyInterTime(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)	{
		VISCA_SynERR();
		VISCAAPP_ZoomPosReplyInterTimeSet(MAKEBYTE(pBuf[6], pBuf[7]));	// cmd not implemented
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamRegisterValue(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_RegViscaBaudrate(MAKEBYTE(pBuf[5], pBuf[6]));	VISCA_COMP();	break;
		case  0x72:		VISCA_ACK();	VISCAAPP_RegMonitoringMode(MAKEBYTE(pBuf[5], pBuf[6]));	VISCA_COMP();	break;
		case  0x73:		VISCA_SynERR();	VISCAAPP_RegOutputEnable(MAKEBYTE(pBuf[5], pBuf[6]));		break;	// cmd not implemented
		case  0x74:		VISCA_SynERR();	VISCAAPP_RegLVDSMode(MAKEBYTE(pBuf[5], pBuf[6]));			break;	// cmd not implemented
		case  0x50:		VISCA_SynERR();	VISCAAPP_RegZoomWideLimit(MAKEBYTE(pBuf[5], pBuf[6]));		break;	// cmd not implemented
		case  0x51:		VISCA_SynERR();	VISCAAPP_RegZoomTeleLimit(MAKEBYTE(pBuf[5], pBuf[6]));		break;	// cmd not implemented
		case  0x52:		VISCA_SynERR();	VISCAAPP_RegDZoomMax(MAKEBYTE(pBuf[5], pBuf[6]));			break;	// cmd not implemented
		case  0x53:		VISCA_SynERR();	VISCAAPP_RegStableZoom(MAKEBYTE(pBuf[5], pBuf[6]));			break;	// cmd not implemented
		case  0x54:		VISCA_SynERR();	VISCAAPP_RegFocusTraceZD(MAKEBYTE(pBuf[5], pBuf[6]));		break;	// cmd not implemented
		case  0x55:		VISCA_SynERR();	VISCAAPP_RegFocusOffsetDC(MAKEBYTE(pBuf[5], pBuf[6]));		break;	// cmd not implemented
		case  0x5F:		VISCA_SynERR();	VISCAAPP_RegEnlargementMode(MAKEBYTE(pBuf[5], pBuf[6]));	break;	// cmd not implemented
		default:		VISCA_SynERR();	VISCAAPP_RegValue(pBuf[4], MAKEBYTE(pBuf[5], pBuf[6]));		break;	// cmd not implemented
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamColorEnhance(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x02:		VISCA_SynERR();	VISCAAPP_ColorEnhanceOnOff(ON);						break;	// cmd not implemented
		case  0x03:		VISCA_SynERR();	VISCAAPP_ColorEnhanceOnOff(OFF);					break;	// cmd not implemented
		default:		VISCA_SynERR();														break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamColorEnhanceParam(PBYTE pBuf)
{
	if(pBuf[12] == 0xff)	{
		VISCA_SynERR();
		VISCAAPP_ColorEnhanceParam(pBuf[4], pBuf[6], pBuf[7], pBuf[8], pBuf[9], pBuf[10], pBuf[11]);	// cmd not implemented
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamChromaSuppress(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case  0x00:		VISCA_SynERR();	VISCAAPP_ChromaSuppress(OFF);	break;	// cmd not implemented
		case  0x01:		VISCA_SynERR();	VISCAAPP_ChromaSuppress(1);		break;	// cmd not implemented
		case  0x02:		VISCA_SynERR();	VISCAAPP_ChromaSuppress(2);		break;	// cmd not implemented
		case  0x03:		VISCA_SynERR();	VISCAAPP_ChromaSuppress(3);		break;	// cmd not implemented
		default:		VISCA_SynERR();									break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamColorGain(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)		{VISCA_ACK();	VISCAAPP_ColorGain(pBuf[7]&0x0f);	VISCA_COMP();}
	else					VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamColorHue(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)		{VISCA_ACK();	VISCAAPP_ColorHue(pBuf[7]&0x0f);	VISCA_COMP();}
	else					VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamGammaOffset(PBYTE pBuf)
{
	if(pBuf[10] == 0xff)	{VISCA_SynERR();	VISCAAPP_GammaOffset(pBuf[7], MAKEBYTE(pBuf[8], pBuf[9]));}	// cmd not implemented
	else					VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamExExpComp(PBYTE pBuf)
{
	switch(pBuf[5])	{
		case  0x00:		VISCA_SynERR();	VISCAAPP_ExExpCompReset();					break;	// cmd not implemented
		case  0x02:		VISCA_SynERR();	VISCAAPP_ExExpCompUp(pBuf[6]);				break;	// cmd not implemented
		case  0x03:		VISCA_SynERR();	VISCAAPP_ExExpCompDown(pBuf[6]);			break;	// cmd not implemented
		default:		VISCA_SynERR();												break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamExExpCompDirect(PBYTE pBuf)
{
	if(pBuf[9] == 0xff)		{VISCA_SynERR();	VISCAAPP_ExExpCompDirect(MAKEBYTE(pBuf[7], pBuf[8]));}	// cmd not implemented
	else					VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamExAperture(PBYTE pBuf)
{
	switch(pBuf[5])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_ExApertureReset();			VISCA_COMP();	break;
		case  0x02:		VISCA_ACK();	VISCAAPP_ExApertureUp(pBuf[6]);		VISCA_COMP();	break;
		case  0x03:		VISCA_ACK();	VISCAAPP_ExApertureDown(pBuf[6]);	VISCA_COMP();	break;
		default:		VISCA_SynERR();														break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamExApertureDirect(PBYTE pBuf)
{
	if(pBuf[9] == 0xff)		{VISCA_ACK();	VISCAAPP_ExApertureDirect(MAKEBYTE(pBuf[7], pBuf[8]));	VISCA_COMP();}
	else					VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamExAutoICRThres(PBYTE pBuf)
{
	switch(pBuf[5])	{
		case  0x00:		VISCA_ACK();	VISCAAPP_ExAutoICROnOffThres(MAKEBYTE(pBuf[7], pBuf[8]));	VISCA_COMP();	break;
		case  0x01:		VISCA_ACK();	VISCAAPP_ExAutoICROffOnThres(MAKEBYTE(pBuf[7], pBuf[8]));	VISCA_COMP();	break;
		default:		VISCA_SynERR();																				break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamExColorGain(PBYTE pBuf)
{
	if(pBuf[9] == 0xff)		{
		switch(pBuf[6])	{
			case  0x00:		VISCA_ACK();	VISCAAPP_ExColorGain(0, MAKEBYTE(pBuf[7], pBuf[8]));	VISCA_COMP();	break;
			case  0x01:		VISCA_SynERR();	break;	// 1.Magenta, cmd not implemented
			case  0x02:		VISCA_SynERR();	break;	// 2.Red, cmd not implemented
			case  0x03:		VISCA_SynERR();	break;	// 3.Yellow, cmd not implemented
			case  0x04:		VISCA_SynERR();	break;	// 4.Green, cmd not implemented
			case  0x05:		VISCA_SynERR();	break;	// 5.Cyan, cmd not implemented
			case  0x06:		VISCA_SynERR();	break;	// 6.Blue, cmd not implemented
			default:		VISCA_SynERR();	break;
		}
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamExColorHue(PBYTE pBuf)
{
	if(pBuf[9] == 0xff)	{
		switch(pBuf[6])	{
			case  0x00:		VISCA_ACK();	VISCAAPP_ExColorHue(0, MAKEBYTE(pBuf[7], pBuf[8]));	VISCA_COMP();	break;
			case  0x01:		VISCA_SynERR();	break;	// 1.Magenta, cmd not implemented
			case  0x02:		VISCA_SynERR();	break;	// 2.Red, cmd not implemented
			case  0x03:		VISCA_SynERR();	break;	// 3.Yellow, cmd not implemented
			case  0x04:		VISCA_SynERR();	break;	// 4.Green, cmd not implemented
			case  0x05:		VISCA_SynERR();	break;	// 5.Cyan, cmd not implemented
			case  0x06:		VISCA_SynERR();	break;	// 6.Blue, cmd not implemented
			default:		VISCA_SynERR();	break;
		}
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamExCtrl(PBYTE pBuf)
{
	switch(pBuf[4])	{
		case 0x0E:	VISCA_ExeCamExExpComp(pBuf);				break;
		case 0x4E:	VISCA_ExeCamExExpCompDirect(pBuf);			break;
		case 0x02:	VISCA_ExeCamExAperture(pBuf);				break;
		case 0x42:	VISCA_ExeCamExApertureDirect(pBuf);			break;
		case 0x21:	VISCA_ExeCamExAutoICRThres(pBuf);			break;
		case 0x49:	VISCA_ExeCamExColorGain(pBuf);				break;
		case 0x4F:	VISCA_ExeCamExColorHue(pBuf);				break;
		default:	VISCA_SynERR();								break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeHLC(PBYTE pBuf)
{
	if(pBuf[6] == 0xff)	{
		VISCA_ACK();
		VISCAAPP_HLCLevel(pBuf[4]);
		VISCAAPP_HLCMaskLevel(pBuf[5]);
		VISCA_COMP();
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_CtrlWBRAW(PBYTE pBuf)
{
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
	stAFCTRL.ctrl &= (~AFCTRL_AFAUTO_MASK);
	stAFCTRL.ctrl |= ( AFCTRL_MANUAL_MODE);		// manual
#endif

	if (pBuf[4]==0x21)	SetWBRAWOnOffPOLY();
	if (pBuf[4]==0x22)	SetWBRAWOnOffCOMP();
	if (pBuf[4]==0x23)	SetWBRAWOnOffTIME();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamCalibAF(PBYTE pBuf)
{
	switch(pBuf[4])	{
#if defined(__USE_AFZOOM_LENS__)
		case  0x00:	VISCA_ACK();	SetDrvOnOffPINT();		VISCA_COMP();	break;
		case  0x01:	VISCA_ACK();	AX_EnableProcess(ON);	VISCA_COMP();	break;
//		case  0x02:	VISCA_ACK();	SetDirDrivePINT(zWIDE);	VISCA_COMP();	break;
//		case  0x03:	VISCA_ACK();	SetDirDrivePINT(zTELE);	VISCA_COMP();	break;
#endif

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
		case  0x08:	VISCA_ACK();	SetAgeOnOffLENS();		VISCA_COMP();	break;
		case  0x10:	VISCA_ACK();	AA_ZOOMHandler(zTELE);	VISCA_COMP();	break;
		case  0x11:	VISCA_ACK();	AA_ZOOMHandler(zWIDE);	VISCA_COMP();	break;
		case  0x12:	VISCA_ACK();	AA_FOKSHandler(fFAR);	VISCA_COMP();	break;
		case  0x13:	VISCA_ACK();	AA_FOKSHandler(fNEAR);	VISCA_COMP();	break;
		case  0x14:	VISCA_ACK();	AA_IRISHandler(ON);		VISCA_COMP();	break;
		case  0x15:	VISCA_ACK();	AA_IRISHandler(OFF);	VISCA_COMP();	break;
		case  0x16:	VISCA_ACK();	AA_OLPFHandler(ON);		VISCA_COMP();	break;
		case  0x17:	VISCA_ACK();	AA_OLPFHandler(OFF);	VISCA_COMP();	break;
#endif
		case  0x20:	VISCA_ACK();	SetWBRAWViewTrigger(0); VISCA_COMP();	break;
		case  0x21:	VISCA_ACK();	VISCA_CtrlWBRAW(pBuf);	VISCA_COMP();	break;
		case  0x22:	VISCA_ACK();	VISCA_CtrlWBRAW(pBuf);	VISCA_COMP();	break;
		case  0x23:	VISCA_ACK();	VISCA_CtrlWBRAW(pBuf);	VISCA_COMP();	break;
//		case  0x24:	VISCA_ACK();	SetWBRAWViewShiftL();	VISCA_COMP();	break;
//		case  0x25:	VISCA_ACK();	SetWBRAWViewShiftR());	VISCA_COMP();	break;
//		case  0x26:	VISCA_ACK();	SetWBRAWViewShiftU());	VISCA_COMP();	break;
//		case  0x27:	VISCA_ACK();	SetWBRAWViewShiftD());	VISCA_COMP();	break;

		default:	VISCA_ACK();	stVISCACTRL.RepeatCmd = MAKEWORD(pBuf[3],pBuf[4]);
					VISCA_COMP();	fVISCA_CmdParsed = TRUE;				break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExePanTiltDrive(PBYTE pBuf)
{
	if(pBuf[8] != 0xff)			return;
	
	switch(MAKEBYTE(pBuf[6], pBuf[7]))	{
		case 0x31:	VISCA_SynERR();	VISCAAPP_PanTiltDriveUp(pBuf[4], pBuf[5]);			break;	// cmd not implemented
		case 0x32:	VISCA_SynERR();	VISCAAPP_PanTiltDriveDown(pBuf[4], pBuf[5]);		break;	// cmd not implemented
		case 0x13:	VISCA_SynERR();	VISCAAPP_PanTiltDriveLeft(pBuf[4], pBuf[5]);		break;	// cmd not implemented
		case 0x23:	VISCA_SynERR();	VISCAAPP_PanTiltDriveRight(pBuf[4], pBuf[5]);		break;	// cmd not implemented
		case 0x11:	VISCA_SynERR();	VISCAAPP_PanTiltDriveUpLeft(pBuf[4], pBuf[5]);		break;	// cmd not implemented
		case 0x21:	VISCA_SynERR();	VISCAAPP_PanTiltDriveUpRight(pBuf[4], pBuf[5]);		break;	// cmd not implemented
		case 0x12:	VISCA_SynERR();	VISCAAPP_PanTiltDriveDownLeft(pBuf[4], pBuf[5]);	break;	// cmd not implemented
		case 0x22:	VISCA_SynERR();	VISCAAPP_PanTiltDriveDownRight(pBuf[4], pBuf[5]);	break;	// cmd not implemented
		case 0x33:	VISCA_SynERR();	VISCAAPP_PanTiltDriveStop(pBuf[4], pBuf[5]);		break;	// cmd not implemented
		default:	VISCA_SynERR();														break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExePanTiltABSPosi(PBYTE pBuf)
{
	if(pBuf[14] != 0xff)	return;
	
	VISCA_SynERR();													// cmd not implemented
	
	VISCAAPP_PanTiltDriveABSPosi(pBuf[4], pBuf[5], \
								MAKEWORD(MAKEBYTE(pBuf[6], pBuf[7]), MAKEBYTE(pBuf[8], pBuf[9])), \
								MAKEWORD(MAKEBYTE(pBuf[10], pBuf[11]), MAKEBYTE(pBuf[12], pBuf[13])));
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExePanTiltRelPosi(PBYTE pBuf)
{
	if(pBuf[14] != 0xff)	return;
	
	VISCA_SynERR();													// cmd not implemented
	
	VISCAAPP_PanTiltDriveRelPosi(pBuf[4], pBuf[5], \
								MAKEWORD(MAKEBYTE(pBuf[6], pBuf[7]), MAKEBYTE(pBuf[8], pBuf[9])), \
								MAKEWORD(MAKEBYTE(pBuf[10], pBuf[11]), MAKEBYTE(pBuf[12], pBuf[13])));
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExePanTiltLimit(PBYTE pBuf)
{
	WORD pan_limit, tilt_limit;
	
	if(pBuf[14] != 0xff)	return;
	
	pan_limit = MAKEWORD(MAKEBYTE(pBuf[6], pBuf[7]), MAKEBYTE(pBuf[8], pBuf[9]));
	tilt_limit = MAKEWORD(MAKEBYTE(pBuf[10], pBuf[11]), MAKEBYTE(pBuf[12], pBuf[13]));
	
	switch(MAKEBYTE(pBuf[4], pBuf[5]))	{
		case 0x00:	VISCA_SynERR();	VISCAAPP_PanTiltLimitSetDownLeft(pan_limit, tilt_limit);	break;	// cmd not implemented
		case 0x01:	VISCA_SynERR();	VISCAAPP_PanTiltLimitSetUpRight(pan_limit, tilt_limit);		break;	// cmd not implemented
		case 0x10:	VISCA_SynERR();	VISCAAPP_PanTiltLimitClrDownLeft(pan_limit, tilt_limit);	break;	// cmd not implemented
		case 0x11:	VISCA_SynERR();	VISCAAPP_PanTiltLimitSetUpRight(pan_limit, tilt_limit);		break;	// cmd not implemented
		default:	VISCA_SynERR();																break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCamAutoICRDelay(PBYTE pBuf)
{
	if(pBuf[8] == 0xff)	{
		VISCA_ACK();
		VISCAAPP_AutoICRDelay(MAKEBYTE(pBuf[6], pBuf[7]));
		VISCA_COMP();
	}
	else	VISCA_SynERR();
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCameraOne(PBYTE pBuf)
{
	switch(pBuf[3])	{
		case 0x00:	VISCA_ExeCamPower(pBuf);					break;
		
		case 0x07:	VISCA_ExeCamZoom(pBuf);						break;
		
		case 0x06:	VISCA_ExeCamDZoom(pBuf);					break;
		case 0x36:	VISCA_ExeCamDZoomCombine(pBuf);				break;
		case 0x46:	VISCA_ExeCamDZoomDirect(pBuf);				break;
		
		case 0x08:	VISCA_ExeCamFocus(pBuf);					break;
		case 0x18:	VISCA_ExeCamFocusOnepush(pBuf);				break;
		case 0x28:	VISCA_ExeCamFocusNearLimit(pBuf);			break;
		case 0x38:	VISCA_ExeCamFocusAutoOnOff(pBuf);			break;
		case 0x48:	VISCA_ExeCamFocusDirect(pBuf);				break;
		
		case 0x58:	VISCA_ExeCamAFSensitivity(pBuf);			break;
		case 0x57:	VISCA_ExeCamAFMode(pBuf);					break;
		case 0x27:	VISCA_ExeCamAFModeInterTime(pBuf);			break;
		
		case 0x11:	VISCA_ExeCamIRCorrection(pBuf);				break;
		case 0x47:	VISCA_ExeCamZoomFocusDirect(pBuf);			break;
		case 0x19:	VISCA_ExeCamInitialize(pBuf);				break;
		
		case 0x35:	VISCA_ExeCamWB(pBuf);						break;
		case 0x10:	VISCA_ExeCamWBOnePush(pBuf);				break;
		case 0x03:	VISCA_ExeCamWBRGain(pBuf);					break;
		case 0x43:	VISCA_ExeCamWBRGainDirect(pBuf);			break;
		case 0x04:	VISCA_ExeCamWBBGain(pBuf);					break;
		case 0x44:	VISCA_ExeCamWBBGainDirect(pBuf);			break;
		
		case 0x39:	VISCA_ExeCamAE(pBuf);						break;
		
		case 0x5A:	VISCA_ExeCamAutoSlowShutter(pBuf);			break;
		
		case 0x0A:	VISCA_ExeCamShutter(pBuf);					break;
		case 0x4A:	VISCA_ExeCamShutterDirect(pBuf);			break;
		
		case 0x0B:	VISCA_ExeCamIris(pBuf);						break;
		case 0x4B:	VISCA_ExeCamIrisDirect(pBuf);				break;
		
		case 0x0C:	VISCA_ExeCamGain(pBuf);						break;
		case 0x4C:	VISCA_ExeCamGainDirect(pBuf);				break;
		case 0x2C:	VISCA_ExeCamGainLimit(pBuf);				break;
		
		case 0x0D:	VISCA_ExeCamBright(pBuf);					break;
		case 0x4D:	VISCA_ExeCamBrightDirect(pBuf);				break;
		
		case 0x3E:	VISCA_ExeCamExpComp(pBuf);					break;
		case 0x0E:	VISCA_ExeCamExpCompAmountSet(pBuf);			break;
		case 0x4E:	VISCA_ExeCamExpCompDirect(pBuf);			break;
		
		case 0x33:	VISCA_ExeCamBacklight(pBuf);				break;
		
		case 0x59:	VISCA_ExeCamSpotAE(pBuf);					break;
		case 0x29:	VISCA_ExeCamSpotAEPosition(pBuf);			break;
		
		case 0x5D:	VISCA_ExeCamAEResponse(pBuf);				break;
		
		case 0x3D:	VISCA_ExeCamWD(pBuf);						break;
		case 0x2D:	VISCA_ExeCamWDSetParam(pBuf);				break;
		
		case 0x37:	VISCA_ExeCamDefog(pBuf);					break;
		
		case 0x02:	VISCA_ExeCamAperture(pBuf);					break;
		case 0x42:	VISCA_ExeCamApertureDirect(pBuf);			break;
		
		case 0x52:	VISCA_ExeCamHR(pBuf);						break;
		
		case 0x53:	VISCA_ExeCamNR(pBuf);						break;
		case 0x5B:	VISCA_ExeCamGamma(pBuf);					break;
		
		case 0x5E:	VISCA_ExeCamHighSensitivity(pBuf);			break;
		case 0x61:	VISCA_ExeCamLRReverse(pBuf);				break;
		case 0x62:	VISCA_ExeCamFreeze(pBuf);					break;
		case 0x63:	VISCA_ExeCamPictureEffect(pBuf);			break;
		case 0x66:	VISCA_ExeCamPictureFlip(pBuf);				break;
		
		case 0x01:	VISCA_ExeCamICR(pBuf);						break;
		case 0x51:	VISCA_ExeCamAutoICR(pBuf);					break;
		case 0x21:	VISCA_ExeCamAutoICRThres(pBuf);				break;
		case 0x31:	VISCA_ExeCamAutoICRAlarmReply(pBuf);		break;
		
		case 0x34:	VISCA_ExeCamStabilizer(pBuf);				break;
		case 0x3F:	VISCA_ExeCamMemoryCustom(pBuf);				break;
		case 0x23:	VISCA_ExeCamMemSave(pBuf);					break;
		case 0x15:	VISCA_ExeCamDisplay(pBuf);					break;
		case 0x73:	VISCA_ExeCamMultiLineTitle(pBuf);			break;
		case 0x74:	VISCA_ExeCamMultiLineTitleCtrl(pBuf);		break;
		case 0x75:	VISCA_ExeCamMute(pBuf);						break;
		
		case 0x76:	VISCA_ExeCamPMSetMask(pBuf);				break;
		case 0x77:	VISCA_ExeCamPMDisplay(pBuf);				break;
		case 0x78:	VISCA_ExeCamPMSetMaskColor(pBuf);			break;
		case 0x79:	VISCA_ExeCamPMSetPanTiltAngle(pBuf);		break;
		case 0x7B:	VISCA_ExeCamPMSetPTZMask(pBuf);				break;
		case 0x6F:	VISCA_ExeCamPMNonInterlockMask(pBuf);		break;
		case 0x7C:	VISCA_ExeCamPMCenterLine(pBuf);				break;
		
		case 0x22:	VISCA_ExeCamIDWrite(pBuf);					break;
		
		case 0x1B:	VISCA_ExeCamMD(pBuf);						break;
		case 0x1C:	VISCA_ExeCamMDFunctionSet(pBuf);			break;
		case 0x1D:	VISCA_ExeCamMDWindowSet(pBuf);				break;
		
		case 0x69:	VISCA_ExeCamContZoomPosReply(pBuf);			break;
		case 0x6A:	VISCA_ExeCamZoomPosReplyInterTime(pBuf);	break;
		case 0x16:	VISCA_ExeCamContFocusPosReply(pBuf);		break;
		case 0x1A:	VISCA_ExeCamFocusPosReplyInterTime(pBuf);	break;
		
		case 0x24:	VISCA_ExeCamRegisterValue(pBuf);			break;
		
		case 0x50:	VISCA_ExeCamColorEnhance(pBuf);				break;
		case 0x20:	VISCA_ExeCamColorEnhanceParam(pBuf);		break;
		case 0x5F:	VISCA_ExeCamChromaSuppress(pBuf);			break;
		case 0x49:	VISCA_ExeCamColorGain(pBuf);				break;
		case 0x4F:	VISCA_ExeCamColorHue(pBuf);					break;
		case 0x1E:	VISCA_ExeCamGammaOffset(pBuf);				break;
		
		case 0x1F:	VISCA_ExeCamExCtrl(pBuf);					break;
		case 0x14:	VISCA_ExeHLC(pBuf);							break;
		case 0xCF:	VISCA_ExeCamCalibAF(pBuf);					break;
		default:	VISCA_SynERR();								break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExePanTilt(PBYTE pBuf)
{
	switch(pBuf[3])	{
		case 0x01:	VISCA_ExePanTiltDrive(pBuf);				break;
		case 0x02:	VISCA_ExePanTiltABSPosi(pBuf);				break;
		case 0x03:	VISCA_ExePanTiltRelPosi(pBuf);				break;
		case 0x04:	VISCA_SynERR();	VISCAAPP_PanTiltDriveHome();	break;
		case 0x05:	VISCA_SynERR();	VISCAAPP_PanTiltDriveReset();	break;
		case 0x06:	VISCA_ExeCamDisplay(pBuf);					break;
		case 0x07:	VISCA_ExePanTiltLimit(pBuf);				break;
		default:	VISCA_SynERR();								break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCameraTwo(PBYTE pBuf)
{
	VISCA_SynERR();													// cmd not implemented
//	UARTprintf("Cam two command not defined ... \r\n");
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCameraMIT(PBYTE pBuf) 	// command for MIT(not compatible with sony cam)
{
	switch(pBuf[3])	{
		case 0x01:	VISCA_ExeCamAutoICRDelay(pBuf);				break;
		default:	VISCA_SynERR();								break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ExeCmd(PBYTE pBuf, BYTE sn)
{
	stVISCACTRL.sn = sn;
	
	switch(pBuf[2])	{											// 3rd byte(category code)
		case 0x04:		VISCA_ExeCameraOne(pBuf);	break;
		case 0x06:		VISCA_ExePanTilt(pBuf);		break;
		case 0x07:		VISCA_ExeCameraTwo(pBuf);	break;
		case 0xA1:		VISCA_ExeCameraMIT(pBuf);	break;			// command for MIT(not compatible with sony cam)
	}
	
//	VISCA_ReplyMsg(VISCA_MSG_CMD_COMPLETE, sn);
	
	memset(pBuf, 0, 16);	stVISCACTRL.sn = 0;				// clear cmd buffer & socket no.
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_CmdProcess(PVISCA_CTRL_PARAM	pCTRL)
{
	if ((pCTRL->ctrl & VISCA_CMD_RX_MASK) == 0)			return;	// check cmd received ?
	
	switch(pCTRL->ctrl & VISCA_CMD_RX_MASK)	{
		case  VISCA_CMD_S1_PARSED:		VISCA_ExeCmd(pCTRL->CmdBuffer[0], 1);	pCTRL->ctrl &= ~VISCA_CMD_S1_PARSED;	break;
		case  VISCA_CMD_S2_PARSED:		VISCA_ExeCmd(pCTRL->CmdBuffer[1], 2);	pCTRL->ctrl &= ~VISCA_CMD_S2_PARSED;	break;
		case  VISCA_CMD_S1S2_PARSED:
				if(pCTRL->ctrl & VISCA_CMD_S2_HIGHPRIORITY)	{
					VISCA_ExeCmd(pCTRL->CmdBuffer[1], 2);	pCTRL->ctrl &= ~VISCA_CMD_S2_PARSED;
				}
				else	{
					VISCA_ExeCmd(pCTRL->CmdBuffer[0], 1);	pCTRL->ctrl &= ~VISCA_CMD_S1_PARSED;
				}
				break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_PacketProcess(PVISCA_CTRL_PARAM pCTRL)
{
	if(DIFF(pCTRL->RxPacketPushCnt, pCTRL->RxPacketPopCnt) ==0)		return;	// received new rx packet ?
	
	VISCA_PacketDecoder(pCTRL->RxPacket[pCTRL->RxPacketPopCnt]);
	
	if(pCTRL->ctrl & (VISCA_CMD_NOT_PARSED | VISCA_INQ_NOT_EXCUTED))	{		// cmd not parsed or inq not excuted ?
		pCTRL->ctrl &= ~(VISCA_CMD_NOT_PARSED | VISCA_INQ_NOT_EXCUTED);
		return;
	}
	
	if(pCTRL->RxPacketPopCnt < 19)		pCTRL->RxPacketPopCnt++;				// 0~19
	else								pCTRL->RxPacketPopCnt=0;				// 19 -> 0
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ReplyAutoICRAlarm(BYTE status)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	
	if(status)		{							// 0:no change, 2:off->on(night), 3:on->off(day)
		pMsg[2] = 0x04;							pMsg[3] = 0x31;
		pMsg[4] = status;						pMsg[5] = 0xff;
		VISCA_ReplyMsg(VISCA_MSG_AUTO_REPLY, 0);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ReplyMDAlarm(BYTE det_frame_no)
{
	PBYTE pMsg = stVISCACTRL.TxPacket;
	
	if(HI4BIT(det_frame_no))		{			// 0:no detection, 0x8?:detection(0x80~3:frame0~3)
		pMsg[2] = 0x04;							pMsg[3] = 0x1B;
		pMsg[4] = LO4BIT(det_frame_no);			pMsg[5] = 0xff;
		VISCA_ReplyMsg(VISCA_MSG_AUTO_REPLY, 0);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_ReplyProcess(PVISCA_INFO_PARAM pINFO)
{
	if(pINFO->other.ctrl1 & VISCA_AUTOICR_ALARM_ON)		VISCA_ReplyAutoICRAlarm(VISCAAPP_GetAutoICRAlarm());
	if(pINFO->efunc1.ctrl1 & VISCA_MD_ON)				VISCA_ReplyMDAlarm(VISCAAPP_GetMDAlarm());

}

//--------------------------------------------------------------------------------------------------------------------------
static void VISCA_DisplayProcess(PVISCA_INFO_PARAM pINFO)
{
	if((pINFO->other.ctrl3 & VISCA_DISP_ON) || (pINFO->other.ctrl3 & VISCA_TILEDISP_ON))	{
		if(pINFO->user.dscr_inter_cnt == 0)	{
			if(pINFO->other.ctrl3 & VISCA_TILEDISP_ON)		VISCAAPP_MultiLineTitleProcess();
			else if(pINFO->other.ctrl3 & VISCA_DISP_ON)		VISCAAPP_DisplayProcess();
		}
		if(pINFO->user.dscr_inter_cnt++ >= 30)		pINFO->user.dscr_inter_cnt=0;	// check every 500ms
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCA_ProcessHandler(void)
{
	VISCA_PacketProcess(&stVISCACTRL);			// packet decoding, reply for inquiry
	VISCA_CmdProcess(&stVISCACTRL);				// run command
	
	if((stVISCACTRL.ctrl & VISCA_VSYNC_TRIGGER)==0)			return;
	stVISCACTRL.ctrl &= (~VISCA_VSYNC_TRIGGER);
	
	VISCA_ReplyProcess(&stVISCAINFO);			// auto alarm reply
	VISCA_DisplayProcess(&stVISCAINFO);			// data screen display
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM VISCA_VSyncTrigger(void)
{
	stVISCACTRL.ctrl |= VISCA_VSYNC_TRIGGER;	// for visca reply & display
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM VISCA_PreemptMsgHooking(void)
{
	if (!IsOtherMsgPreemptive(MSG_VISCOM)) return FALSE;

//	VISCA_AckCode &= 0xe0;
//	VISCA_AckCode |= (1<<GetPreemptiveMessage(MSG_VISCOM));
//	VISCA_CommandResponse();
	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL ISPM IsVISCA_MsgHooking(void)
{
	if (VISCA_PreemptMsgHooking()) return TRUE;
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM VISCA_MessageClassify(void)
{
	if (fVISCA_CmdParsed==FALSE) return;

	fVISCA_CmdParsed = FALSE;
	if (IsVISCA_MsgHooking()==TRUE) return;

	SetMsgVISCA_CtrlMode();
	SetMsgVISCA_NewCmd(stVISCACTRL.RepeatCmd);
//	VISCA_CommandResponse();
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL ISPM IsVISCA_CommandZOOM(void)
{
	switch (stVISCACTRL.RepeatCmd) {
		case 0x0802:	case 0x0803:	// FOKS Standard
		case 0x0820:	case 0x0830:	// FOKS variable
		case 0x480D:					// FOKS direct

		case 0x0702:	case 0x0703:	// ZOOM Standard
		case 0x0720:	case 0x0730:	// ZOOM variable
		case 0x470D:	case 0x470F:	// ZOOM direct
						return TRUE;
		default:		return FALSE;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCA_MessageExecution(void)
{
	VISCAAPP_ExeRepeatCmd(stVISCACTRL.RepeatCmd);
}

//--------------------------------------------------------------------------------------------------------------------------
void VISCA_SetRepeatCmd(WORD cmd)
{
	stVISCACTRL.RepeatCmd = cmd;
	fVISCA_CmdParsed = TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL VISCA_GetActStatus(void)
{
	return		(stVISCACTRL.ctrl & VISCA_IF_ACTIVATED)?  ON:  OFF;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM VISCA_RxHandler(BYTE ch)
{
	PBYTE  pBuf = (PBYTE)stVISCACTRL.RxBuffer;
	BYTE bStartIdx=0;
	
	// check other protocol communication
	if (fSerialCmdReceive || fPELCO_CmdReceive)	return;
	
	// check start code
	if (fVISCA_CmdReceive==0 && stVISCACTRL.RxCnt==0 && (ch&0xf0)!=VISCA_RX_BGN) return;
	else		fVISCA_CmdReceive = ON;

	pBuf[stVISCACTRL.RxCnt] = ch;						stVISCACTRL.RxCnt++;
	
	// check packet length
	if((pBuf[stVISCACTRL.RxCnt-1] != VISCA_RX_END) && (stVISCACTRL.RxCnt < VISCA_PACKET_MAX))	return;
	
#if defined(__IDEO_ZOOM_V101__)				//_IDEO : 20150807, find STX Check!
	while(bStartIdx < stVISCACTRL.RxCnt)	{
		if ((pBuf[bStartIdx]&VISCA_RX_BGN) == 0x80)	break;
		else											bStartIdx++;
	}
#endif
	
	// move rx buffer --> packet buffer
	if ((pBuf[bStartIdx]&VISCA_RX_BGN)&&(pBuf[stVISCACTRL.RxCnt-1]==VISCA_RX_END)) {	// verify start&end code
		
		memcpy(stVISCACTRL.RxPacket[stVISCACTRL.RxPacketPushCnt], pBuf+bStartIdx, VISCA_PACKET_MAX-bStartIdx);	// move rx buffer to packet buffer
		
		if(DIFF(stVISCACTRL.RxPacketPushCnt+1, stVISCACTRL.RxPacketPopCnt))	{			// buffer not full ?
			if(stVISCACTRL.RxPacketPushCnt < 19)		stVISCACTRL.RxPacketPushCnt++;	// push counter : 0~19 -> 0~19,,
			else	{
				if(stVISCACTRL.RxPacketPopCnt != 0)		stVISCACTRL.RxPacketPushCnt=0;	// buffer full ? stop push counter.
			}
		}
	}
	
	memset(pBuf, 0, stVISCACTRL.RxCnt);					stVISCACTRL.RxCnt=0;			// clear rx buffer & counter
}


/*  FILE_END_HERE */
