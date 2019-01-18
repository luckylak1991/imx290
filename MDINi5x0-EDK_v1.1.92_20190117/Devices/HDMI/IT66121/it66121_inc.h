///*****************************************
//  Copyright (C) 2009-2017
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <hdmitx.h>
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2017/12/20
//   @fileversion: ITE_HDMITX_SAMPLE_3.25
//******************************************/

#include "common.h"


#if defined(__USE_IT66121_TX__)


#ifndef _IT66121_INC_H_
#define _IT66121_INC_H_

//#ifdef _MCU_8051_
//    #include "mcu.h"
//    #include "io.h"
//    #include "utility.h"
//
//#else // not MCU
//    #include <windows.h>
//    #include <winioctl.h>
//    #include <stdio.h>
//    #include <stdlib.h>
//    #include <string.h>
//    #include <stdarg.h>
//    #include "usbi2c.h"
//    #include "io.h"
//#endif // MCU
#include "it66121_debug.h"
#include "it66121_config.h"
#include "it66121_typedef.h"
#include "it66121.h"
#include "it66121_cec.h"
#include "it66121_drv.h"
#include "it66121_hdcp.h"
#include "it66121_input.h"
#include "it66121_sys.h"
#include "sha1.h"


#define HDMITX_MAX_DEV_COUNT 1


#define		HDMITX_ReadI2C_Byte(rAddr)				I2C_ByteRead(TWI_CH0, IT66121_ADR, rAddr)
#define		HDMITX_WriteI2C_Byte(rAddr, wData)		I2C_ByteWrite(TWI_CH0, IT66121_ADR, rAddr, wData)
#define		HDMITX_ReadI2C_ByteN(rAddr, pData, N)	I2C_Read(TWI_CH0, IT66121_ADR, rAddr, I2C_ADDR_8BIT, (PBYTE)pData, I2C_DATA_8BIT, N)
#define		HDMITX_SetI2C_Byte(rAddr, M, R)			I2C_ByteWrite(TWI_CH0, IT66121_ADR, rAddr, (I2C_ByteRead(TWI_CH0, IT66121_ADR, rAddr)&~(M)) | ((R)&(M)))
#define		CEC_ReadI2C_Byte(rAddr)					I2C_ByteRead(TWI_CH0, IT66121_CECADR, rAddr)
#define		CEC_WriteI2C_Byte(rAddr, wData)			I2C_ByteWrite(TWI_CH0, IT66121_CECADR, rAddr, wData)
#define		CEC_SetI2C_Byte(rAddr, M, R)			I2C_ByteWrite(TWI_CH0, IT66121_CECADR, rAddr, (I2C_ByteRead(TWI_CH0, IT66121_CECADR, rAddr)&~(M)) | ((R)&(M)))


///////////////////////////////////////////////////////////////////////
// Output Mode Type
///////////////////////////////////////////////////////////////////////

#define RES_ASPEC_4x3 0
#define RES_ASPEC_16x9 1
#define F_MODE_REPT_NO 0
#define F_MODE_REPT_TWICE 1
#define F_MODE_REPT_QUATRO 3
#define F_MODE_CSC_ITU601 0
#define F_MODE_CSC_ITU709 1


#define TIMER_LOOP_LEN 10
#define MS(x) (((x)+(TIMER_LOOP_LEN-1))/TIMER_LOOP_LEN); // for timer loop

// #define SUPPORT_AUDI_AudSWL 16 // Jeilin case.
#define SUPPORT_AUDI_AudSWL 24 // Jeilin case.

#if(SUPPORT_AUDI_AudSWL==16)
    #define CHTSTS_SWCODE 0x02
#elif(SUPPORT_AUDI_AudSWL==18)
    #define CHTSTS_SWCODE 0x04
#elif(SUPPORT_AUDI_AudSWL==20)
    #define CHTSTS_SWCODE 0x03
#else
    #define CHTSTS_SWCODE 0x0B
#endif

#endif // _IT66121_INC_H_


#endif	// __USE_IT66121_TX__

