//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2016  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  SPICOMM.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__SPICOMM_H__
#define		__SPICOMM_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------


// -----------
// spi slave
// -----------

#define		SPIS_CMD_BUF_MAX			512

typedef struct
{
	LONG	ctrl;				// frame buffer capture control
	
	WORD	lx;					// frame buffer capture start-x
	WORD	ly;					// frame buffer capture start-y
	WORD	w;					// frame buffer capture width
	WORD	h;					// frame buffer capture height
	WORD	rd_x;				// frame buffer capture x index counter
	WORD	rd_y;				// frame buffer capture y index counter
	WORD	cnt_col;			// frame buffer column counter per line
	DWORD 	cnt_tx;				// frame buffer capture tx packet counter(dword)
	DWORD	cnt_rd;				// frame buffer capture ddr read counter(dword)
	WORD	cnt_push;			// frame buffer capture raw data buffer push counter
	WORD	cnt_pop;			// frame buffer capture raw data buffer pop counter
	DWORD	txsize;				// frame buffer capture tx packet size(dword)
	DWORD	e_addr_y;			// frame buffer capture ddr address(y)
	DWORD	e_addr_c;			// frame buffer capture ddr address(c)
	DWORD	e_addr_rd;			// frame buffer capture ddr address(y)
	BYTE	rd_buff[8];			// frame buffer capture read data buffer
	BYTE	raw_buff[16];		// frame buffer capture raw data 16byte buffer
	BYTE	tx_buff[4];			// frame buffer capture tx data buffer

}	SPIs_FBCAP_PARAM, *PSPIs_FBCAP_PARAM;


#define		SPIS_FBCAP_CTRL_RUN				0x00000001		// spi-slave frame buffer capture start flag
#define		SPIS_FBCAP_CTRL_STOP			0x00000000		// spi-slave frame buffer capture stop flag


typedef struct
{
	LONG	ctrl;				// spi-slave tx control
	
	WORD	cnt_tx;				// spi-slave tx packet counter
	WORD	txsize;				// spi-slave tx packet size
	DWORD	buff_3a[50];		// spi-slave tx 3a packet buffer
	DWORD	buff_nr[50];		// spi-slave tx nr packet buffer
	WORD	size_3a;			// spi-slave tx 3a packet size
	WORD	size_nr;			// spi-slave tx nr packet size

}	SPIs_TXRAW_PARAM, *PSPIs_TXRAW_PARAM;

#define		SPIS_TXRAW_CTRL_UPDATE			0x00000001		// spi-slave set raw data update flag


typedef struct
{
	DWORD	sys_info;			// spi-slave master's system info.
	BOOL	dynt_status;		// spi-slave master's day&night status. ( 0:day, 1:night )
	BYTE	pm_rect_no;			// spi-slave master's pm rect number ( 0 ~ 7 )
	BYTE	pm_poly_no;			// spi-slave master's pm poly number ( 0 ~ 7 )
	BYTE	pm_poly_edge;		// spi-slave master's pm edge select ( 0 ~ 3 )
	BYTE	md_area_no;			// spi-slave master's md area number ( 0 ~ 3 )
	BYTE	camtitle_txt[10];	// spi-slave master's cam title text
	BYTE	camtitle_color;		// spi-slave master's cam title color
	WORD	temp_h16b;			// spi-slave 16bit temp buffer
	BYTE	cac_area;			// spi-slave master's cac area ( magenta, red, yellow, green, cyan, blue )
	BYTE	dummy;

}	SPIs_MASTER_INFO, *PSPIs_MASTER_INFO;


typedef	struct
{
	LONG	info;
	
	WORD	reg_addr;			// spi-slave register r/w address, [15:12]region(0:host, 2:local, 6:sdram), [11:0]addr
	WORD	reg_val;			// spi-slave register r/w value
	DWORD	sdr_addr;			// spi-slave sdram r/w address
	
	SPIs_FBCAP_PARAM	stFBCAP;
	SPIs_TXRAW_PARAM	stTXRAW;
	SPIs_MASTER_INFO	stMAINFO;

}	SPIs_APP_PARAM, *PSPIs_APP_PARAM;


typedef	struct
{
	LONG	ctrl;				// spi-slave control flag
	
	DWORD	RxBuffer;			// spi-slave rx data buffer
	DWORD	TxBuffer;			// spi-slave tx data buffer
	DWORD	CmdBuffer[SPIS_CMD_BUF_MAX];		// spi-slave rx command buffer
	WORD	CmdPushCnt;			// spi-slave rx command push counter
	WORD	CmdPopCnt;			// spi-slave rx command pop counter

}	SPIs_CTRL_PARAM, *PSPIs_CTRL_PARAM;

#define		SPIS_TX_MODE_MASK				0x0000000F		// spi-slave tx mode mask
#define		SPIS_TX_MODE_3A_DATA			0x00000001		// spi-slave tx 3a raw data to spi-master
#define		SPIS_TX_MODE_FB_DATA			0x00000002		// spi-slave frame buffer capture data to spi-master
#define		SPIS_TX_MODE_NR_DATA			0x00000003		// spi-slave tx nr raw data to spi-master
#define		SPIS_TRIGGER					0x80000000		// spi-slave process handler tirgger

#define		SPIS_ACK_CMD_BUF_FULL			0x40000000		// spi-slave return dummy data
#define		SPIS_ACK_CMD_BUF_FREE			0x40100000		// spi-slave return dummy data




// -----------
// spi master
// -----------

#define		SPIM_TDLY					4
#define		SPIM_MISP_CID_S1			1
#define		SPIM_MISP_CID_S2			2
#define		SPIM_MISP_CID_S3			3

typedef	struct
{
	DWORD	ysum[5];		// ae, y-sum
	DWORD	iris;
	DWORD	gain;
#if defined(__USE_IRDATA_SLAVE2MASER__)
	DWORD	irdata;
#endif

	LONG	RPFa;			// af, RPF data of window A
	LONG	LPFa;			// af, LPF data of window A
	LONG	VPFa;			// af, VPF data of window A
	LONG	BPFa;			// af, BPF data of window A
	LONG	HPFa;			// af, HPF data of window A
	LONG	CNTa;			// af, CNT data of window A

	LONG	RPFb;			// af, RPF data of window B
	LONG	LPFb;			// af, LPF data of window B
	LONG	VPFb;			// af, VPF data of window B
	LONG	BPFb;			// af, BPF data of window B
	LONG	HPFb;			// af, HPF data of window B
	LONG	CNTb;			// af, CNT data of window B

/*
	DWORD	yint_cnt;		// awb, y-integral count
	DWORD	yint_sum;		// awb, y-integral sum
	WORD	max[10];		// awb, 0:rr, 1:rg, 2:rb, 3:gr, 4:gg, 5:gb, 6:br, 7:bg, 8:bb, 9:dummy
	WORD	prm[8];			// awb, 0-1:R-prm, 2-3:G-prm, 4-5:B-prm, 6:prm cnt, 7:dummy
	WORD	gry[8];			// awb, 0-1:R-gry, 2-3:G-gry, 4-5:B-gry, 6:gry cnt, 7:dummy
	WORD	ctl[8];			// awb, 0-1:R-ctl, 2-3:G-ctl, 4-5:B-ctl, 6:ctl cnt, 7:dummy

	DWORD	gainR;			// awb, wb gain-r
	DWORD	gainB;			// awb, wb gain-b
*/
}	SPIm_RX3A_PARAM, *PSPIm_RX3A_PARAM;

typedef	struct
{
	WORD	stdev_int_br;
	WORD	stdev_frac_br;
	WORD	stdev_int_da;
	WORD	stdev_frac_da;
	WORD	stdev_int;
	WORD	stdev_frac;
	
}	SPIm_RXNR_PARAM, *PSPIm_RXNR_PARAM;

typedef	struct
{
	LONG	AEGainGain;
	LONG	AEGainMax;
	LONG	AEGainMin;
	LONG	AEGainPMax;
	LONG	AEYSum;
	WORD	AEBright;
	WORD	AEDGain;
	SHORT	AEShutGain;
	SHORT	AEIRISGain;
	SHORT	WBgainR;
	SHORT	WBgainB;
	
	WORD	nr_fr_misc;				// [12]agc_max, [9:8]under_exp, [7:1]edge_level, [0]lowlight_mode
	WORD	nr_fr_stdev;
	WORD	nr_fr_stdev_br;
	WORD	nr_fr_stdev_da;
	WORD	nr_up_cnt;
	
	WORD	ll_sharpness;
	WORD	ll_saturation;
	WORD	csp_ls_x;
	WORD	cac_g_gain;
	WORD	cac_m_gain;
	
	WORD	ddpc_reg[5];			// [0]0x403, [1]0x404, [2]0x405, [3]0x406, [4]0x40c
	WORD	dummy;

}	SPIm_TX_PARAM, *PSPIm_TX_PARAM;

typedef	struct
{
	LONG				ctrl;		// spi-master control flag
	
	SPIm_RX3A_PARAM		stRX3A;		// spi-master rx 3a raw data buffer
	SPIm_RXNR_PARAM		stRXNR;		// spi-master rx nr raw data buffer
	SPIm_TX_PARAM		stTX;

}	SPIm_APP_PARAM, *PSPIm_APP_PARAM;

#define		SPIM_RX_CNT_MASK				0x000000FF		// spi-slave rx enable counter



// spi master command set
// inquiry
#define SPIM_INQ_3A_RAWDATA					0x09070000
#define SPIM_INQ_NR_RAWDATA					0x09080000
#define SPIM_INQ_REG_READ					0x09A00000
#define SPIM_INQ_FBCAP_READY				0x09A30000


// command
#define SPIM_CMD_REG_WRITE					0x01000000
#define SPIM_CMD_REG_ADDR					0x01010000
#define SPIM_CMD_IR_KEY						0x01020000
#define SPIM_CMD_FBCAP_START				0x01030001
#define SPIM_CMD_FBCAP_STOP					0x01030000
#define SPIM_CMD_FBCAP_X					0x01040000
#define SPIM_CMD_FBCAP_Y 					0x01050000
#define SPIM_CMD_FBCAP_W 					0x01060000
#define SPIM_CMD_FBCAP_H 					0x01070000

// command - panorame board
#if defined(__PANORAMA_SETUP__)  // panorama SPI slave command
#define SPIM_CMD_PANO_DISP					0x01100000
#define SPIM_CMD_PANO_SAVE					0x01110000
#define SPIM_CMD_PANO_DEL 					0x01120000
#define SPIM_CMD_PANO_BLND 					0x01130000
#define SPIM_CMD_PANO_BLVL 					0x01140000
#define SPIM_CMD_PANO_SELSS 				0x01150000
#define SPIM_CMD_PANO_SSADJ 				0x01160000
#define SPIM_CMD_PANO_CNTRL 				0x01170000

#define SPIM_CMD_PANO_SS0	 				0x01180000
#define SPIM_CMD_PANO_SS1	 				0x01180001
#define SPIM_CMD_PANO_SS2	 				0x01180002
#define SPIM_CMD_PANO_SS3	 				0x01180003
#define SPIM_CMD_PANO_BLEND	 				0x01180004

#define SPIM_CMD_PANO_CRP_LX_U	 			0x01190000
#define SPIM_CMD_PANO_CRP_LX_D	 			0x01190001
#define SPIM_CMD_PANO_CRP_RX_U	 			0x01190002
#define SPIM_CMD_PANO_CRP_RX_D	 			0x01190003
#define SPIM_CMD_PANO_CRP_TY_U	 			0x01190004
#define SPIM_CMD_PANO_CRP_TY_D	 			0x01190005
#define SPIM_CMD_PANO_CRP_BY_U	 			0x01190006
#define SPIM_CMD_PANO_CRP_BY_D	 			0x01190007
#elif defined(__CASCADE_CAM_SETUP__)
#define SPIM_CMD_CASCADE_DISP				0x01100000
#define SPIM_CMD_CASCADE_SAVE				0x01110000
#define SPIM_CMD_CASCADE_DEL 				0x01120000
#define SPIM_CMD_CASCADE_BLND 				0x01130000
#define SPIM_CMD_CASCADE_BLVL 				0x01140000
#define SPIM_CMD_CASCADE_SELSS 				0x01150000
#define SPIM_CMD_CASCADE_SSADJ 				0x01160000
#define SPIM_CMD_CASCADE_CNTRL 				0x01170000

#define SPIM_CMD_CASCADE_SS0	 			0x01180000
#define SPIM_CMD_CASCADE_SS1	 			0x01180001
#define SPIM_CMD_CASCADE_SS2	 			0x01180002
#define SPIM_CMD_CASCADE_SS3	 			0x01180003
#define SPIM_CMD_CASCADE_BLEND	 			0x01180004

#define SPIM_CMD_CASCADE_CRP_LX_U	 		0x01190000
#define SPIM_CMD_CASCADE_CRP_LX_D	 		0x01190001
#define SPIM_CMD_CASCADE_CRP_RX_U	 		0x01190002
#define SPIM_CMD_CASCADE_CRP_RX_D	 		0x01190003
#define SPIM_CMD_CASCADE_CRP_TY_U	 		0x01190004
#define SPIM_CMD_CASCADE_CRP_TY_D	 		0x01190005
#define SPIM_CMD_CASCADE_CRP_BY_U	 		0x01190006
#define SPIM_CMD_CASCADE_CRP_BY_D	 		0x01190007

#endif // #if defined(__PANORAMA_SETUP__)

// command - 3a
#define SPIM_CMD_WBGAINR					0x01200000
#define SPIM_CMD_WBGAINB					0x01210000
#define SPIM_CMD_AEDGAIN					0x01220000
#define SPIM_CMD_AEGAINGAINH				0x01230000
#define SPIM_CMD_AEGAINGAINL				0x01240000
#define SPIM_CMD_AESHUTGAIN					0x01250000
#define SPIM_CMD_AEIRISGAIN					0x01260000
#define SPIM_CMD_AEGAINMAXH					0x01270000
#define SPIM_CMD_AEGAINMAXL					0x01280000
#define SPIM_CMD_AEGAINMINH					0x01290000
#define SPIM_CMD_AEGAINMINL					0x012A0000
#define SPIM_CMD_AEGAINPMAXH				0x012B0000
#define SPIM_CMD_AEGAINPMAXL				0x012C0000
#define SPIM_CMD_AEBRIGHT					0x012D0000
#define SPIM_CMD_AEYSUMH					0x012E0000
#define SPIM_CMD_AEYSUML					0x012F0000

// command - misc
#define SPIM_CMD_LLSHARPNESS				0x01300000
#define SPIM_CMD_LLSATURATION				0x01310000
#define SPIM_CMD_CSPLS						0x01320000
#define SPIM_CMD_CACGGAIN					0x01330000
#define SPIM_CMD_CACMGAIN					0x01340000

#define SPIM_CMD_DYNTCTRL					0x01400000

#define SPIM_CMD_NRMODE						0x01500000
#define SPIM_CMD_NRINTENSITY				0x01510000
#define SPIM_CMD_NRAPERTURE					0x01520000
#define SPIM_CMD_NRZOOMMODE					0x01530000
#define SPIM_CMD_NRMISC						0x01540000
#define SPIM_CMD_NRSTDEV					0x01550000
#define SPIM_CMD_NRSTDEVBR					0x01560000
#define SPIM_CMD_NRSTDEVDA					0x01570000
#define SPIM_CMD_NRUPCNT					0x01580000

#define	SPIM_CMD_PMCTRL						0x01600000
#define	SPIM_CMD_PMRECTSIZEW				0x01610000
#define	SPIM_CMD_PMRECTSIZEH				0x01620000
#define	SPIM_CMD_PMRECTPOSIX				0x01630000
#define	SPIM_CMD_PMRECTPOSIY				0x01640000
#define	SPIM_CMD_PMPOLYPOSIX				0x01650000
#define	SPIM_CMD_PMPOLYPOSIY				0x01660000

#define	SPIM_CMD_GMCTRL						0x01700000
#define	SPIM_CMD_EFFECTCTRL					0x01710000

#define	SPIM_CMD_DZOOMCTRL					0x01800000
#define	SPIM_CMD_DEFOGCTRL					0x01810000
#define	SPIM_CMD_DWDRCTRL					0x01820000

#define	SPIM_CMD_MDCTRL						0x01830000
#define	SPIM_CMD_MDPOSI						0x01840000
#define	SPIM_CMD_MDSIZE						0x01850000

#define	SPIM_CMD_DIS						0x01860000

#define	SPIM_CMD_BACKLIGHTCTRL				0x01870000
#define	SPIM_CMD_BLCPOSI					0x01880000
#define	SPIM_CMD_BLCSIZE					0x01890000
#define	SPIM_CMD_HLCPOSI					0x018A0000
#define	SPIM_CMD_HLCSIZE					0x018B0000

#define	SPIM_CMD_BINNING					0x018C0000
#define	SPIM_CMD_DISPFUNCCTRL				0x018D0000
#define	SPIM_CMD_WDR						0x018E0000
#define	SPIM_CMD_FLICKER					0x018F0000

#define	SPIM_CMD_CAMTITLECTRL				0x01900000
#define	SPIM_CMD_NTSCPAL					0x01910000
#define	SPIM_CMD_RS485CTRL					0x01920000
#define	SPIM_CMD_LANG						0x01930000
#define	SPIM_CMD_FACTORYRESET				0x01950000
#define	SPIM_CMD_OUTPUTFORMAT				0x01980000

#define	SPIM_CMD_ENGRCTRL					0x01A00000
#define	SPIM_CMD_CCCTRL						0x01A10000
#define	SPIM_CMD_SSTPPOSI					0x01A20000
#define	SPIM_CMD_SSTPSIZE					0x01A30000
#define	SPIM_CMD_MISCTUNECTRL				0x01A80000
#define	SPIM_CMD_WDRTUNECTRL				0x01A90000
#define	SPIM_CMD_DNRTUNECTRL				0x01AA0000

#define	SPIM_CMD_IREDGEONOFF				0x01F00000

#define SPIM_CMD_SDPCCTRL					0x01F10000



// ----------------------------------------------------------------------
// Exported Variables
// ----------------------------------------------------------------------
extern SPIs_CTRL_PARAM	stSPIsCTRL;
extern SPIs_APP_PARAM	stSPIsAPP;
extern SPIm_APP_PARAM	stSPImAPP;



// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// spis_drv.c
void SPIs_Initialize(void);
void SPIs_ProcessHandler(void);
void ISPM SPIs_RxHandler(void);


// spim_drv.c
void SPIm_Initialize(void);
MISP_ERROR_t SPIm_Command(BYTE cID, DWORD cmd);
void SPIm_Inquiry(BYTE cID, DWORD inq, PBYTE pBuff);
void SPIm_Inq3aRawData(BYTE cID, PBYTE pBuff, PBYTE pSize);
void SPIm_InqNRRawData(BYTE cID, PBYTE pBuff, PBYTE pSize);


// spis_app.c
void SPIsAPP_Update3aRawData(void);
void SPIsAPP_UpdateNRRawData(void);
void ISPM SPIsAPP_SetRawUpdateFlag(void);
void SPIsAPP_ClearRawUpdateFlag(void);
void SPIsAPP_CmdRegAddr(WORD addr);
void SPIsAPP_CmdRegWrite(WORD val);
void SPIsAPP_CmdIRKey(WORD val);
void SPIsAPP_CmdFBcapCtrl(WORD val);
void SPIsAPP_CmdFBcapX(WORD val);
void SPIsAPP_CmdFBcapY(WORD val);
void SPIsAPP_CmdFBcapW(WORD val);
void SPIsAPP_CmdFBcapH(WORD val);

#if defined(__PANORAMA_SETUP__) // panorama SPI slave fuctions

void SPIsAPP_CmdPanoDisp(WORD val);
void SPIsAPP_CmdPanoSave(WORD val);
void SPIsAPP_CmdPanoDel(WORD val);
void SPIsAPP_CmdPanoBlend(WORD val);
void SPIsAPP_CmdPanoBLvL(WORD val);

void SPIsAPP_CmdPanoSelSS(WORD val);
void SPIsAPP_CmdPanoSSAdj(WORD val);
void SPIsAPP_CmdPanoCntrl(WORD val);

void SPIsAPP_CmdPanoSelSS_MDINAPP(WORD val);
void SPIsAPP_CmdPanoControl_MDINAPP(WORD val);

#endif //  #if defined(__PANORAMA_SETUP__)

void SPIsAPP_CmdWBGainR(WORD val);
void SPIsAPP_CmdWBGainB(WORD val);
void SPIsAPP_CmdAEDGain(WORD val);
void SPIsAPP_CmdAEGainGainH(WORD val);
void SPIsAPP_CmdAEGainGainL(WORD val);
void SPIsAPP_CmdAEShutGain(WORD val);
void SPIsAPP_CmdAEIrisGain(WORD val);
void SPIsAPP_CmdAEGainMaxH(WORD val);
void SPIsAPP_CmdAEGainMaxL(WORD val);
void SPIsAPP_CmdAEGainMinH(WORD val);
void SPIsAPP_CmdAEGainMinL(WORD val);
void SPIsAPP_CmdAEGainPMaxH(WORD val);
void SPIsAPP_CmdAEGainPMaxL(WORD val);
void SPIsAPP_CmdAEBright(WORD val);
void SPIsAPP_CmdAEYsumH(WORD val);
void SPIsAPP_CmdAEYsumL(WORD val);

void SPIsAPP_CmdLLSharpness(WORD val);
void SPIsAPP_CmdLLSaturation(WORD val);
void SPIsAPP_CmdCSPLS(WORD val);
void SPIsAPP_CmdCACGgain(WORD val);
void SPIsAPP_CmdCACMgain(WORD val);

void SPIsAPP_CmdDYNTCtrl(BYTE cmd, BYTE val);

void SPIsAPP_CmdNRMode(WORD val);
void SPIsAPP_CmdNRIntensity(WORD val);
void SPIsAPP_CmdNRAperture(WORD val);
void SPIsAPP_CmdNRZoomMode(WORD val);
void SPIsAPP_CmdNRMisc(WORD val);
void SPIsAPP_CmdNRStdev(WORD val);
void SPIsAPP_CmdNRStdevBr(WORD val);
void SPIsAPP_CmdNRStdevDa(WORD val);
void SPIsAPP_CmdNRUpCnt(WORD val);

void SPIsAPP_CmdPMCtrl(BYTE cmd, BYTE val);
void SPIsAPP_CmdPMRectSizeW(WORD w);
void SPIsAPP_CmdPMRectSizeH(WORD h);
void SPIsAPP_CmdPMRectPosiX(WORD x);
void SPIsAPP_CmdPMRectPosiY(WORD y);
void SPIsAPP_CmdPMPolyPosiX(WORD y);
void SPIsAPP_CmdPMPolyPosiY(WORD y);

void SPIsAPP_CmdGammaCtrl(BYTE cmd, BYTE val);
void SPIsAPP_CmdEffectCtrl(BYTE cmd, BYTE val);

void SPIsAPP_CmdDZOOMCtrl(BYTE cmd, BYTE val);
void SPIsAPP_CmdDefogCtrl(BYTE cmd, BYTE val);
void SPIsAPP_CmdDWDRCtrl(BYTE cmd, BYTE val);

void SPIsAPP_CmdMDCtrl(BYTE cmd, BYTE val);
void SPIsAPP_CmdMDPosi(WORD val);
void SPIsAPP_CmdMDSize(WORD val);

void SPIsAPP_CmdBacklightCtrl(BYTE cmd, BYTE val);
void SPIsAPP_CmdBLCPosi(BYTE x, BYTE y);
void SPIsAPP_CmdBLCSize(BYTE w, BYTE h);
void SPIsAPP_CmdHLCPosi(BYTE x, BYTE y);
void SPIsAPP_CmdHLCSize(BYTE w, BYTE h);

void SPIsAPP_CmdBinning(WORD nID);
void SPIsAPP_CmdDispFuncCtrl(BYTE cmd, BYTE val);

void SPIsAPP_CmdCamTitleCtrl(BYTE cmd, BYTE val);
void SPIsAPP_CmdNTPAL(WORD nID);
void SPIsAPP_CmdFactoryReset(WORD val);
void SPIsAPP_CmdOutputFormat(WORD val);

void SPIsAPP_CmdEngrCtrl(BYTE cmd, BYTE val);
void SPIsAPP_CmdCCCtrl(BYTE cmd, BYTE val);
void SPIsAPP_CmdSSTPPosi(BYTE x, BYTE y);
void SPIsAPP_CmdSSTPSize(BYTE w, BYTE h);
void SPIsAPP_CmdMISCTUNECtrl(BYTE cmd, BYTE val);
void SPIsAPP_Cmd4K30P60POSD(WORD OnOff);

void SPIsAPP_CmdIREdgeOnOff(WORD nID);

void SPIsAPP_CmdSDPCCtrl(BYTE cmd, BYTE val);


WORD ISPM SPIsAPP_InqRegRead(WORD addr);
WORD ISPM SPIsAPP_InqUSR0(WORD val);
WORD ISPM SPIsAPP_InqUSR1(WORD val);
WORD ISPM SPIsAPP_InqUSR2(WORD val);
WORD ISPM SPIsAPP_Inq3aTxSize(void);
WORD ISPM SPIsAPP_InqNRTxSize(void);
void ISPM SPIsAPP_Inq3aRaw(PDWORD pBuff);
void ISPM SPIsAPP_InqNRRaw(PDWORD pBuff);
WORD ISPM SPIsAPP_InqFBcapStatus(WORD val);
void ISPM SPIsAPP_InqFBcapRaw(PDWORD pBuff);

BOOL SPIsAPP_GetMasterDYNTStatus(void);
void SPIsAPP_GetMasterCamTitleTxt(PBYTE pBuff);
void SPIsAPP_GetMasterCamTitleColor(PBYTE pBuff);




// spim_app.c
//MISP_ERROR_t SPImAPP_CmdRegMultiWrite(BYTE cID, BYTE region, DWORD addr, PBYTE pBuff, DWORD bytes);
MISP_ERROR_t SPImAPP_CmdRegWrite(BYTE cID, BYTE region, DWORD addr, WORD wData);
MISP_ERROR_t SPImAPP_CmdStd(BYTE cID, DWORD cmd, WORD option);
MISP_ERROR_t SPImAPP_InqRegMultiRead(BYTE cID, BYTE region, DWORD addr, PBYTE pBuff, DWORD bytes);
MISP_ERROR_t SPImAPP_InqRegRead(BYTE cID, BYTE region, DWORD addr, PWORD rData);
MISP_ERROR_t SPImAPP_InqStd(BYTE cID, DWORD inq, WORD option, PDWORD rData);
void SPImAPP_RxSlaveRawData(void);
void SPImAPP_GetSlaveAEysum(PBYTE pBuff);
void SPImAPP_GetSlaveIRdata(PBYTE pBuff);
DWORD SPImAPP_GetSlaveWBycnt(void);
DWORD SPImAPP_GetSlaveWBysum(void);
void SPImAPP_GetSlaveWBmax(PBYTE pBuff);
void SPImAPP_GetSlaveWBprm(PBYTE pBuff);
void SPImAPP_GetSlaveWBgry(PBYTE pBuff);
void SPImAPP_GetSlaveWBctl(PBYTE pBuff);
SHORT SPImAPP_GetSlaveWBgainR(void);
SHORT SPImAPP_GetSlaveWBgainB(void);
void SPImAPP_GetSlaveAFfiltA(PBYTE pBuff);
void SPImAPP_GetSlaveAFfiltB(PBYTE pBuff);
void SPImAPP_GetSlaveNRStdev(PWORD pstdev_int, PWORD pstdev_frac);
void SPImAPP_GetSlaveNRStdevBr(PWORD pstdev_int_br, PWORD pstdev_frac_br);
void SPImAPP_GetSlaveNRStdevDa(PWORD pstdev_int_da, PWORD pstdev_frac_da);

void SPImAPP_SetSlaveAEDGain(WORD gain);
void SPImAPP_SetSlaveAEGainGain(LONG gain);
void SPImAPP_SetSlaveAEGainMax(LONG val);
void SPImAPP_SetSlaveAEGainMin(LONG val);
void SPImAPP_SetSlaveAEGainPMax(LONG val);
void SPImAPP_SetSlaveAEYSum(LONG val);
void SPImAPP_SetSlaveAEBright(WORD val);
void SPImAPP_SetSlaveWBgain(WORD gainR, WORD gainB);

void SPImAPP_SetSlaveLLSharpness(WORD val);
void SPImAPP_SetSlaveLLSaturation(WORD val);
void SPImAPP_SetSlaveCSPLS(BYTE x0, BYTE x1);
void SPImAPP_SetSlaveCACGgain(WORD val);
void SPImAPP_SetSlaveCACMgain(WORD val);

void SPImAPP_SetSlaveDYNTStatus(BOOL status);
void SPImAPP_SetSlaveDYNTNightColor(BOOL onoff);
void SPImAPP_SetSlaveDYNTColorBurst(BOOL onoff);

void SPImAPP_SetSlaveNRMode(MISP_NR_MODE_t val);
void SPImAPP_SetSlaveNRIntensity(MISP_NR3D_MODE_t val);
void SPImAPP_SetSlaveNRAperture(WORD val);
void SPImAPP_SetSlaveNRZoomMode(void);
void SPImAPP_SetSlaveNRMISC(BYTE agc_max, BYTE under_exp, BYTE edge_level, BYTE lowlight_mode);
void SPImAPP_SetSlaveNRStdev(WORD stdev_int, WORD stdev_frac);
void SPImAPP_SetSlaveNRStdevBr(WORD stdev_int_br, WORD stdev_frac_br);
void SPImAPP_SetSlaveNRStdevDa(WORD stdev_int_da, WORD stdev_frac_da);
void SPImAPP_SetSlaveNRUpCnt(BYTE up_cnt);

void SPImAPP_SetSlavePMOnOff(BOOL onoff);
void SPImAPP_SetSlavePMRectOnOff(BYTE no, BOOL onoff);
void SPImAPP_SetSlavePMRectSize(BYTE no, WORD w, WORD h);
void SPImAPP_SetSlavePMRectPosi(BYTE no, WORD x, WORD y);
void SPImAPP_SetSlavePMDefault(void);
void SPImAPP_SetSlavePMType(BYTE type);
void SPImAPP_SetSlavePMRectColor(BYTE no, BYTE color);
void SPImAPP_SetSlavePMPolyOnOff(BYTE no, BYTE onoff);
void SPImAPP_SetSlavePMPolyColor(BYTE no, BYTE color);
void SPImAPP_SetSlavePMPolyPosi(BYTE no, BYTE edge, WORD x, WORD y);

void SPImAPP_SetSlaveGammaOnOff(WORD nID);
void SPImAPP_SetSlaveGammaFrontEn(WORD nID);
void SPImAPP_SetSlaveGammaRearEn(WORD nID);
void SPImAPP_SetSlaveGamma8tap(WORD nID);
void SPImAPP_SetSlaveGammaBYLUTExt(WORD nID);
void SPImAPP_SetSlaveGammaRGBLUTExt(WORD nID);
void SPImAPP_SetSlaveGammaYCLUTExt(WORD nID);
void SPImAPP_SetSlaveGammaBYLUTXresol(WORD nID);
void SPImAPP_SetSlaveGammaRGBLUTXresol(WORD nID);

void SPImAPP_SetSlaveEffectContrast(WORD nID);
void SPImAPP_SetSlaveEffectBrightness(WORD nID);
void SPImAPP_SetSlaveEffectSaturation(WORD nID);
void SPImAPP_SetSlaveEffectHue(WORD nID);
void SPImAPP_SetSlaveEffectEdgeEnhance(WORD nID);
void SPImAPP_SetSlaveEffectAutoSaturate(WORD nID);
void SPImAPP_SetSlaveEffectAutoEdge(WORD nID);
void SPImAPP_SetSlaveEffectDefault(WORD nID);

void SPImAPP_SetSlaveDZOOMMode(WORD nID);
void SPImAPP_SetSlaveDZOOMRatio(WORD nID);
void SPImAPP_SetSlavePIPMode(WORD nID);
void SPImAPP_SetSlavePIPSize(WORD nID);
void SPImAPP_SetSlavePIPPosi(WORD nID);
void SPImAPP_SetSlaveDZOOMDefault(void);

void SPImAPP_SetSlaveDefogMode(WORD nID);
void SPImAPP_SetSlaveDefogStrength(WORD nID);
void SPImAPP_SetSlaveDefogThreshold(WORD nID);
void SPImAPP_SetSlaveDefogAutoLevel(WORD nID);
void SPImAPP_SetSlaveDWDRMode(WORD nID);
void SPImAPP_SetSlaveDWDRStrength(WORD nID);
void SPImAPP_SetSlaveDWDRSaturation(WORD nID);
void SPImAPP_SetSlaveDWDRLocalRatio(WORD nID);
void SPImAPP_SetSlaveDWDRSatSync(WORD nID);
void SPImAPP_SetSlaveDWDRAutoLevel(WORD nID);

void SPImAPP_SetSlaveMDOnOff(WORD nID);
void SPImAPP_SetSlaveMDAreaSel(WORD nID);
void SPImAPP_SetSlaveMDAreaOnOff(BYTE no, BYTE onoff);
void SPImAPP_SetSlaveMDAreaDisp(BYTE no, BYTE onoff);
void SPImAPP_SetSlaveMDSizePosi(BYTE no, BYTE w, BYTE h, BYTE lx, BYTE ly);
void SPImAPP_SetSlaveMDThreshold(WORD nID);
void SPImAPP_SetSlaveMDMotionView(WORD nID);
void SPImAPP_SetSlaveMDDefault(void);

void SPImAPP_SetSlaveBackLightMode(WORD nID);
void SPImAPP_SetSlaveBLCOnOff(WORD nID);
void SPImAPP_SetSlaveBLCDisp(WORD nID);
void SPImAPP_SetSlaveBLCDefault(WORD nID);
void SPImAPP_SetSlaveBLCPosi(BYTE x, BYTE y);
void SPImAPP_SetSlaveBLCSize(BYTE w, BYTE h);
void SPImAPP_SetSlaveHLCDispOnOff(WORD nID);
void SPImAPP_SetSlaveHLCLevel(WORD nID);
void SPImAPP_SetSlaveHLCBlackMask(WORD nID);
void SPImAPP_SetSlaveHLCDefault(void);
void SPImAPP_SetSlaveHLCPosi(BYTE x, BYTE y);
void SPImAPP_SetSlaveHLCSize(BYTE w, BYTE h);
void SPImAPP_SetSlaveBinning(WORD nID);
void SPImAPP_SetSlaveDDPCReg(PWORD pBuff);
void SPImAPP_SetSlaveSDPCRun(WORD nID);
void SPImAPP_SetSlaveSDPCMemWriteOrder(void);
void SPImAPP_SetSlaveSDPCMemReadOrder(void);
void SPImAPP_SetSlaveSDPCMemResetOrder(void);
void SPImAPP_SetSlaveSDPCMemReloadOrder(void);
void SPImAPP_SetSlaveSDPCFlashPrintOrder(void);
void SPImAPP_SetSlaveSDPCFlashUpdateOrder(void);
void SPImAPP_SetSlaveSDPCFlashWriteOrder(void);
void SPImAPP_SetSlaveSDPCFlashClearOrder(void);
void SPImAPP_SetSlaveSDPCFlashReadOrder(void);
void SPImAPP_SetSlaveSDPCFlashTempReadOrder(void);
void SPImAPP_SetSlaveFreezeMode(WORD nID);
void SPImAPP_SetSlaveMirrorMode(WORD nID);
void SPImAPP_SetSlaveImageEffect(WORD nID);
void SPImAPP_SetSlaveRotation(WORD nID);
void SPImAPP_SetSlavePicStyle(WORD nID);
void SPImAPP_SetSlavePicContrast(WORD nID);
void SPImAPP_SetSlavePicSaturation(WORD nID);
void SPImAPP_SetSlavePicEdgeEnhance(WORD nID);

void SPImAPP_SetSlaveCamTitleDisplay(WORD nID);
void SPImAPP_SetSlaveCamTitleTxt(BYTE idx, BYTE txt);
void SPImAPP_SetSlaveCamTitleColor(WORD nID);
void SPImAPP_SetSlaveNTPAL(WORD nID);

void SPImAPP_SetSlaveFactoryReset(void);
void SPImAPP_SetSlaveOutputFormat(WORD nID);

void SPImAPP_SetSlaveCCRed(WORD nID);
void SPImAPP_SetSlaveCCGreen(WORD nID);
void SPImAPP_SetSlaveCCBlue(WORD nID);
void SPImAPP_SetSlaveCACTheta(WORD level, WORD area);
void SPImAPP_SetSlaveCACSaturation(WORD level, WORD area);
void SPImAPP_SetSlaveCACCos(WORD level, WORD area);
void SPImAPP_SetSlaveCACSin(WORD level, WORD area);
void SPImAPP_SetSlaveCACWriteEEPROM(void);

void SPImAPP_SetSlaveInPattern(WORD nID);
void SPImAPP_SetSlaveOutPattern(WORD nID);
void SPImAPP_SetSlaveTPSel(WORD nID);
void SPImAPP_SetSlaveTPOnOff(WORD nID);
void SPImAPP_SetSlaveTPPosi(BYTE x, BYTE y);
void SPImAPP_SetSlaveTPSize(BYTE w, BYTE h);
void SPImAPP_SetSlaveTPDefault(void);
void SPImAPP_SetSlaveByrBypass(WORD nID);
void SPImAPP_SetSlaveSensorFps(WORD nID);
void SPImAPP_SetSlaveYCConv(WORD nID);
void SPImAPP_SetSlaveOutCSC(WORD nID);
void SPImAPP_SetSlaveOutDither(WORD nID);
void SPImAPP_SetSlaveHdmiTxMode(WORD nID);
void SPImAPP_SetSlave4K30P60POSD(WORD nID);
void SPImAPP_SetSlaveIREdgeOnOff(WORD nID);



#endif	/* __SPICOMM_H__ */
