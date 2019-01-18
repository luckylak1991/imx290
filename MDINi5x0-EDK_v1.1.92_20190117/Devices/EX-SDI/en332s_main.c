// ==================================================================================
//	File Name			: 
//	Description			: EN334S register initailize
//	Date				: 2016.10.13
//	Created by			:
//	Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
//	Revision History	: 
// ==================================================================================
//#include "../cpu/cpu_main.h"
//#include "enx_dev.h"
#include  "video.h"


#if defined(__USE_EN332T__)

#include "en332s_ini_table.h"
#include "en332t_ini_table_tx.h"
#include "en332t_ini_table_txo.h"
#include "en332t_ini_table_txm.h"
#include "en332t_ini_table_tpo.h"

// ==================================================================================
// VCLK is internal processing clock
// VCLK is set to  EN332T clock when EN332T master mode

// VCLK_OUT_I is bridge clock between VCLK & VCLK_OUT
#define _MIDN_VCLK_OUT_I_INV_   0	// [ 0:normal ] [ 1:invert vclk_out_i ] 
//#define _MIDN_VCLK_OUT_I_INV_   1	// [ 0:normal ] [ 1:invert vclk_out_i ] 

#define _MIDN_VCLK_OUT_I_DLY_  0 	// [ 0~3 : normal~most delayed ]
// #define _MIDN_VCLK_OUT_I_DLY_  1 	// [ 0~3 : normal~most delayed ]
// #define _MIDN_VCLK_OUT_I_DLY_  2 	// [ 0~3 : normal~most delayed ]
// #define _MIDN_VCLK_OUT_I_DLY_  3 	// [ 0~3 : normal~most delayed ]
 
// VCLK_OUT is actual output clock of MDIN
#define _MIDN_VCLK_OUT_INV_      0	// [ 0:normal ] [ 1:invert vclk_out ] 
// #define _MIDN_VCLK_OUT_INV_      1	// [ 0:normal ] [ 1:invert vclk_out ] 

#define _MIDN_VCLK_OUT_DLY_     0 	// [ 0~3 : normal~most delayed ]
// #define _MIDN_VCLK_OUT_DLY_     1 	// [ 0~3 : normal~most delayed ]
// #define _MIDN_VCLK_OUT_DLY_     2 	// [ 0~3 : normal~most delayed ]
// #define _MIDN_VCLK_OUT_DLY_     3 	// [ 0~3 : normal~most delayed ]

// ==================================================================================

BYTE en332t_mode, en332t_frmt;

// ==================================================================================
// Initial Device
// ==================================================================================
void EN332S_Init(WORD DevAddr)
{
	BYTE IniCnt;
	for(IniCnt = 0; IniCnt < INI_Size ; IniCnt ++)
		ENX_I2Cw(DevAddr,EN332S_INI[IniCnt][0],EN332S_INI[IniCnt][1]);
}

void EN332S_Init_Ucc(WORD DevAddr)
{
	BYTE IniCnt;
	for(IniCnt = 0; IniCnt < UCC_INI_Size ; IniCnt ++) {
		ENX_I2Cw(DevAddr,UCC_INI[IniCnt][0],UCC_INI[IniCnt][1]);
	}
}

// ==================================================================================
// Register initail table
// ==================================================================================
void EN332STask(WORD DevAddr, BYTE mode, BYTE frmt)
{
	switch(mode)	{
		case 0x00:		// Slave mode : BT1120 to TxPHY
					if(frmt==0x2)		ENX_TBL_I2Cw(DevAddr,(UINT*)TX_HDSDI,TX_HDSDI_Size);
					else if(frmt==0x3)	ENX_TBL_I2Cw(DevAddr,(UINT*)TX_EX270_HD,TX_EX270_HD_Size);
					else if(frmt==0x4)	ENX_TBL_I2Cw(DevAddr,(UINT*)TX_EX135_HD,TX_EX135_HD_Size);
					else if(frmt==0x5)	ENX_TBL_I2Cw(DevAddr,(UINT*)TX_EX1G5_4K,TX_EX1G5_4K_Size);
					else if(frmt==0x6)	ENX_TBL_I2Cw(DevAddr,(UINT*)TX_EX270_4M,TX_EX270_4M_Size);
					else if(frmt==0x7)	ENX_TBL_I2Cw(DevAddr,(UINT*)TX_3GSDI,TX_3GSDI_Size);
				break;
		case 0x01:		// Master mode : EN332T clock out and data receive
					if(frmt==0x2)		ENX_TBL_I2Cw(DevAddr,(UINT*)TXM_HDSDI,TXM_HDSDI_Size);
					else if(frmt==0x3)	ENX_TBL_I2Cw(DevAddr,(UINT*)TXM_EX270_HD,TXM_EX270_HD_Size);
					else if(frmt==0x4)	ENX_TBL_I2Cw(DevAddr,(UINT*)TXM_EX135_HD,TXM_EX135_HD_Size);
					else if(frmt==0x5)	ENX_TBL_I2Cw(DevAddr,(UINT*)TXM_EX1G5_4K,TXM_EX1G5_4K_Size);
					else if(frmt==0x6)	ENX_TBL_I2Cw(DevAddr,(UINT*)TXM_EX270_4M,TXM_EX270_4M_Size);
					else if(frmt==0x7)	ENX_TBL_I2Cw(DevAddr,(UINT*)TXM_3GSDI,TXM_3GSDI_Size);
				break;
		case 0x02:		// Common mode : XI(27Mhz) share
					if(frmt==0x2)		ENX_TBL_I2Cw(DevAddr,(UINT*)TXO_HDSDI,TXO_HDSDI_Size);
					else if(frmt==0x3)	ENX_TBL_I2Cw(DevAddr,(UINT*)TXO_EX270_HD,TXO_EX270_HD_Size);
					else if(frmt==0x4)	ENX_TBL_I2Cw(DevAddr,(UINT*)TXO_EX135_HD,TXO_EX135_HD_Size);
					else if(frmt==0x5)	ENX_TBL_I2Cw(DevAddr,(UINT*)TXO_EX1G5_4K,TXO_EX1G5_4K_Size);
					else if(frmt==0x6)	ENX_TBL_I2Cw(DevAddr,(UINT*)TXO_EX270_4M,TXO_EX270_4M_Size);
					else if(frmt==0x7)	ENX_TBL_I2Cw(DevAddr,(UINT*)TXO_3GSDI,TXO_3GSDI_Size);
				break;
		case 0x03:		// Self generation test pattern
					if(frmt==0x9)		ENX_TBL_I2Cw(DevAddr,(UINT*)TP_HDSDI,TP_HDSDI_Size);
					else if(frmt==0x7)	ENX_TBL_I2Cw(DevAddr,(UINT*)TP_EX270_HD,TP_EX270_HD_Size);
					else if(frmt==0x8)	ENX_TBL_I2Cw(DevAddr,(UINT*)TP_EX135_HD,TP_EX135_HD_Size);
					else if(frmt==0xa)	ENX_TBL_I2Cw(DevAddr,(UINT*)TP_EX1G5_TDM_A,TP_EX1G5_TDM_A_Size);
					else if(frmt==0xc)	ENX_TBL_I2Cw(DevAddr,(UINT*)TP_EX1G5_4K,TP_EX1G5_4K_Size);
					else if(frmt==0xb)	ENX_TBL_I2Cw(DevAddr,(UINT*)TP_EX270_4M,TP_EX270_4M_Size);
					else if(frmt==0xd)	ENX_TBL_I2Cw(DevAddr,(UINT*)TP_3GSDI,TP_3GSDI_Size);
				break;
	}
}

// ==================================================================================
// Set Tx mode
// ==================================================================================
void EN332x_Initialize(void)
{
	// set tx mode
	//en332t_mode = 0x00;		// slave mode
	//en332t_mode = 0x01;		// master mode
	en332t_mode = 0x02;		// common mode
	//en332t_mode = 0x03;		// self generation tp
	
	EN332S_Init(ENX_DEV0_ADDR);
	EN332S_Init_Ucc(ENX_DEV0_ADDR);
	SetEN332T_Frmt();
	
	// set isp clk & out mode
	if(en332t_mode == 0x01)	{			// master mode only
		SetCLK_SrcVpllHostCLK();		// Master mode only : set isp vpll source to 'host_clk_in'
		SetCLK_SetBypassVideoPLL();		// Master mode only : bypass isp v.pll for 'host_clk_in'
		
		SetCLK_VCLKOutiInv(_MIDN_VCLK_OUT_I_INV_);		// Master mode only : set 'vclk_out_i_inverse'
		SetCLK_VCLKOutiDly(_MIDN_VCLK_OUT_I_DLY_);		// Master mode only : set 'vclk_out_i_delay'
		
		SetCLK_VCLKOutInv(_MIDN_VCLK_OUT_INV_);			// Master mode only : set 'vclk_out_inverse'
		SetCLK_VCLKOutDly(_MIDN_VCLK_OUT_DLY_);		// Master mode only : set 'vclk_out_delay'
	}
}


void SetEN332T_Frmt(void)
{
	if(IsOUT_MainFrmt4K())			en332t_frmt = (en332t_mode==0x03)?  0x0c:  0x05;	// 4K ex-sdi
	else if(IsOUT_MainFrmt4M())		en332t_frmt = (en332t_mode==0x03)?  0x0b:  0x06;	// 4M ex-sdi
	else if(IsOUT_MainFrmt3G())		en332t_frmt = (en332t_mode==0x03)?  0x0d:  0x07;	// 3G sdi
	else							en332t_frmt = (en332t_mode==0x03)?  0x09:  0x02;	// HD sdi

	EN332STask(ENX_DEV0_ADDR, en332t_mode, en332t_frmt);
}


/*
// ==================================================================================
// Device mode check
// ==================================================================================
void Mode_chk(BYTE *ModeTrig)
{
	BYTE m0,m1,m2,m3;
	BYTE BufMode;
	BYTE TmpMode;
	
	*ModeTrig = 0;
	
	m0 = ~GpioGetPin(GPIO_SW1);	
	m1 = ~GpioGetPin(GPIO_SW2);	
	m2 = ~GpioGetPin(GPIO_SW3);	
	m3 = ~GpioGetPin(GPIO_SW4);	
	BufMode = ( ((m0&0x1)<<3) | ((m1&0x1)<<2) | ((m2&0x1)<<1) | (m3&0x1) );

	// Mode switch change detection
	if(FixMode!=BufMode){	
		TmpMode = BufMode;
		WaitUs(10000000);	WaitUs(10000000);	WaitUs(10000000);
		WaitUs(10000000);	WaitUs(10000000);	WaitUs(10000000);
		m0 = ~GpioGetPin(GPIO_SW1);	
		m1 = ~GpioGetPin(GPIO_SW2);	
		m2 = ~GpioGetPin(GPIO_SW3);
		m3 = ~GpioGetPin(GPIO_SW4);	
		BufMode = ( ((m0&0x1)<<3) | ((m1&0x1)<<2) | ((m2&0x1)<<1) | (m3&0x1) );
	
		if(TmpMode==BufMode){
			TxStrNoIRQ("Mode selection : ",BufMode,2);
			FixMode = BufMode;
			*ModeTrig = 1;
		}
	}
	
	if(FixMode){	// I2C control mode
		GpioFuncPin(GPIO_I2C_SCL);
		GpioFuncPin(GPIO_I2C_SDA);}
	else{			// I2C release mode
		while(1){
			GpioFuncPinOff(GPIO_I2C_SCL);
			GpioFuncPinOff(GPIO_I2C_SDA);
			GpioInDir(GPIO_I2C_SCL);
			GpioInDir(GPIO_I2C_SDA);
			
			m0 = ~GpioGetPin(GPIO_SW1);	
			m1 = ~GpioGetPin(GPIO_SW2);	
			m2 = ~GpioGetPin(GPIO_SW3);	
			m3 = ~GpioGetPin(GPIO_SW4);	
			BufMode = ( ((m0&0x1)<<3) | ((m1&0x1)<<2) | ((m2&0x1)<<1) | (m3&0x1) );

			if(FixMode!=BufMode){	
				TmpMode = BufMode;
				WaitUs(10000000);	WaitUs(10000000);	WaitUs(10000000);
				WaitUs(10000000);	WaitUs(10000000);	WaitUs(10000000);
				m0 = ~GpioGetPin(GPIO_SW1);	
				m1 = ~GpioGetPin(GPIO_SW2);	
				m2 = ~GpioGetPin(GPIO_SW3);	
				m3 = ~GpioGetPin(GPIO_SW4);	
				BufMode = ( ((m0&0x1)<<3) | ((m1&0x1)<<2) | ((m2&0x1)<<1) | (m3&0x1) );
	
				if(TmpMode==BufMode){
					TxStrNoIRQ("While Mode selection : ",BufMode,2);
					FixMode = BufMode;
					*ModeTrig = 1;
					GpioFuncPin(GPIO_I2C_SCL);
					GpioFuncPin(GPIO_I2C_SDA);
					break;
				}
			}
			WDT_CNT_RST= 1;
		}
	}
}
*/
		
#endif	//#if defined(__USE_EN332T__)
