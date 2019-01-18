///*****************************************
//  Copyright (C) 2009-2017
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iTE6805_Global.h>
//   @author Kuro.Chung@ite.com.tw
//   @date   2017/02/24
//   @fileversion: iTE6805_MCUSRC_0.63
//******************************************/


#if defined(__USE_IT6802_RX__)


#ifndef		__ITE680x_GOLBAL_H__
#define		__ITE680x_GOLBAL_H__

//#include "common.h"
//#define	_CODE

// #define 		Enable_IT6802_CEC		//for ARC and HEAC Function

#ifdef Enable_IT6802_CEC
//	#include "it680x_cec.h"

	#define DEBUG_IT6802_CEC
	#define IT6802CECGPIOid		0		// for identife IT6802_CEC use Pin 1.0 & 1.1
#endif

/****************************************************************************/

/****************************************************************************/
#define	IT6802_MAX			1//2

#define IT6802A0_ADDR0		0x94	//Hardware Fixed I2C address of IT6802 HDMI
#define IT6802B0_ADDR0		0x90	//Hardware Fixed I2C address of IT6802 HDMI
#define MHL_ADDR0			0xE0	//Software programmable I2C address of IT6802 MHL
#define EDID_ADDR0			0xA8	//Software programmable I2C address of IT6802 EDID RAM
#define CEC_ADDR0			0xC8	//Software programmable I2C address of IT6802 CEC

#define IT6802A0_ADDR1		0x96	//Hardware Fixed I2C address of IT6802 HDMI
#define IT6802B0_ADDR1		0x92	//Hardware Fixed I2C address of IT6802 HDMI
#define MHL_ADDR1			0xE2	//Software programmable I2C address of IT6802 MHL
#define EDID_ADDR1			0xAA	//Software programmable I2C address of IT6802 EDID RAM
#define CEC_ADDR1			0xCA	//Software programmable I2C address of IT6802 CEC

#define IT6802A0_ADDR(ID)	((ID)==0? IT6802A0_ADDR0: IT6802A0_ADDR1)
#define IT6802B0_ADDR(ID)	((ID)==0? IT6802B0_ADDR0: IT6802B0_ADDR1)
#define MHL_ADDR(ID)			((ID)==0? MHL_ADDR0: MHL_ADDR1)
#define EDID_ADDR(ID)		((ID)==0? EDID_ADDR0: EDID_ADDR1)
#define CEC_ADDR(ID)			((ID)==0? CEC_ADDR0: CEC_ADDR1)


#define DP_ADDR				0x90
#define ADC_ADDR			0x90

#define HDMI_DEV				0
#define DP_DEV				0

#define RXDEV				0
#define MAXRXDEV			1

/****************************************************************************/

/****************************************************************************/
#define HDMI_DEV			0
#define SWADR			0x96
#define EDID_HEAD		0xA0


#ifdef _HPDMOS_
	#define HPDON		0
	#define HPDOFF		1
#else
	#define HPDON		1
	#define HPDOFF		0
#endif

/****************************************************************************/
//#define	ITE6805_BUSID		I2C_1
//#define	IT6802_RX1_ADDR		0x90	//Hardware Fixed I2C address of IT6802 HDMI
//#define	IT6802_RX2_ADDR		0x92	//Hardware Fixed I2C address of IT6802 HDMI

//I2C wrapper macros
#define i2c_read_byte(sAddr, rAddr, bytes, pBuff, dev)		I2C_Read(TWI_CH1, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes)
#define i2c_write_byte(sAddr, rAddr, bytes, pBuff, dev)		I2C_Write(TWI_CH1, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes)
/****************************************************************************/
#define	_HDMIRX_DBGPRT_ 	0
#if _HDMIRX_DBGPRT_ == 1
	#include <stdio.h>
	#define MHLRX_DEBUG_PRINTF(x)		printf  x
	#define EQ_DEBUG_PRINTF(x)			printf  x
	#define EQ_PORT0_PRINTF(x)			printf  x
	#define EQ_PORT1_PRINTF(x)			printf  x
	#define VIDEOTIMNG_DEBUG_PRINTF(x)	// printf  x
	#define IT6802_DEBUG_INT_PRINTF(x)	printf  x
	#define IT6802_DEBUG_INT_PRINTF1(x)	//printf  x
	#define IT6802_DEBUG_INT_PRINTF2(x)	//printf  x
	#define IT6802_DEBUG_INT_PRINTF3(x)	printf  x
	#define IT6802_MHL_DEBUG_PRINTF(x)	//printf x
	#define MHL_MSC_DEBUG_PRINTF(x)		//printf x
	#define HDMIRX_VIDEO_PRINTF(x)		printf  x
	#define HDMIRX_AUDIO_PRINTF(x)		//printf  x
	#define HDMIRX_DEBUG_PRINT(x)		//printf x
	#define CEC_DEBUG_PRINTF(x)			//printf x
	#define EDID_DEBUG_PRINTF(x)			//printf  x
	#define IT680X_DEBUG_PRINTF(x)		// printf  x
	#define VSDB_DEBUG_PRINTF(x)			//printf  x
	#define RCP_DEBUG_PRINTF(x)			printf  x
	#define MHL3D_DEBUG_PRINTF(x)		//printf  x
	#define CBUS_CAL_PRINTF(x)			printf  x
	#define MHL_INT_PRINTF(x)				printf  x
	#define RXHDCP_DEBUG_PRINT(x)		printf x

	#define HDCP_DEBUG_PRINTF(x)			//printf x
	#define HDCP_DEBUG_PRINTF1(x)		//printf x
	#define HDCP_DEBUG_PRINTF2(x)		//printf x
#else
	#define MHLRX_DEBUG_PRINTF(x)
	#define EQ_DEBUG_PRINTF(x)
	#define EQ_PORT0_PRINTF(x)
	#define EQ_PORT1_PRINTF(x)
	#define VIDEOTIMNG_DEBUG_PRINTF(x)
	#define IT6802_DEBUG_INT_PRINTF(x)
	#define IT6802_DEBUG_INT_PRINTF1(x)
	#define IT6802_DEBUG_INT_PRINTF2(x)
	#define IT6802_DEBUG_INT_PRINTF3(x)
	#define IT6802_MHL_DEBUG_PRINTF(x)
	#define MHL_MSC_DEBUG_PRINTF(x)
	#define HDMIRX_VIDEO_PRINTF(x)
	#define HDMIRX_AUDIO_PRINTF(x)
	#define HDMIRX_DEBUG_PRINT(x)
	#define CEC_DEBUG_PRINTF(x)
	#define EDID_DEBUG_PRINTF(x)
	#define IT680X_DEBUG_PRINTF(x)
	#define VSDB_DEBUG_PRINTF(x)
	#define RCP_DEBUG_PRINTF(x)
	#define MHL3D_DEBUG_PRINTF(x)
	#define CBUS_CAL_PRINTF(x)
	#define MHL_INT_PRINTF(x)
	#define RXHDCP_DEBUG_PRINT(x)
#endif

#define delay1ms(x) Delay_mSec(x)

#endif  /*__ITE680x_GOLBAL_H__*/ 


#endif	// __USE_IT6802_RX__

