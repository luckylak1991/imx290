//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name			:  MISPTYPE.H
// Description			:  This file contains typedefine for the driver files	
// Ref. Docment			: 
// Revision History 	:

#ifndef		__MISPTYPE_H__	
#define		__MISPTYPE_H__

#ifndef		_LUCIDA_H_
#include	"macroimg.h"
#endif

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

typedef void				VOID, *PVOID;		// void
typedef signed char			CHAR, *PCHAR;		// signed 8bit
typedef short				SHORT, *PSHORT;		// signed 16bit
typedef long				LONG, *PLONG;		// signed 32bit

typedef unsigned char		BYTE, *PBYTE;		// unsigned 8bit
typedef unsigned short		WORD, *PWORD;		// unsigned 16bit
typedef unsigned long		DWORD, *PDWORD;		// unsigned 32bit
typedef	float				FLOAT, *PFLOAT;		// floating number

typedef volatile void		VVOID, *VPVOID;		// volatile void
typedef volatile char		VCHAR, *VPCHAR;		// volatile signed 8bit
typedef volatile short		VSHORT, *VPSHORT;	// volatile signed 16bit
typedef volatile long		VLONG, *VPLONG;		// volatile signed 32bit

typedef volatile BYTE		VBYTE, *VPBYTE;		// volatile unsigned 8bit
typedef volatile WORD		VWORD, *VPWORD;		// volatile unsigned 16bit
typedef volatile DWORD		VDWORD, *VPDWORD;	// volatile unsigned 32bit

typedef DWORD				MISP_COLORRGB, *PMISP_COLORRGB;	// RGB color
typedef DWORD				MISP_COLORYBR, *PMISP_COLORYBR;	// YCbCr color

#define	BOOL				BYTE				// boolean flag
#define	VBOOL				VBYTE				// volatile boolean flag
#define	ROMDATA				const				// program(read-only) memory

#define	TRUE				1
#define	FALSE				0

#define	ON					1
#define	OFF					0

#define	HIGH				1
#define	LOW					0

#define	POSI				1
#define	NEGA				0

#define	NACK				1
#define	ACK					0

#define DMA_RD				1
#define DMA_WR				0

#ifndef NULL
#define	NULL				0
#endif

#define		DIFF(a,b)			((a)>(b)? (a-b):(b-a))
#define		MBIT(a,b)			(((a)&(b))? ON : OFF)
#define		RBIT(a,b)			(((a)&(b))? OFF : ON)

#define		ABS(a)				((a)>0? (a):-(a))
#define		MIN(a,b)			((a)>(b)? (b):(a))
#define		MAX(a,b)			((a)>(b)? (a):(b))

#define		HI4BIT(a)			((BYTE)((a)>>4))
#define		LO4BIT(a)			((BYTE)((a)&0x0f))

#define		HIBYTE(a)			((BYTE)((a)>>8))
#define		LOBYTE(a)			((BYTE)((a)&0xff))

#define		HIWORD(a)			((WORD)((a)>>16))
#define		LOWORD(a)			((WORD)((a)&0xffff))

#define		CLIP12(a)			((WORD)((((a)>2047)? 2047:((a)<-2048)? -2048:(a))))
#define		CLIP09(a)			((WORD)((((a)>255)? 255:((a)<-256)? -256:(a))))

#define		MAKEBYTE(a,b)		((BYTE)(((BYTE)(a)<<4)|(b)))
#define		MAKEWORD(a,b)		((WORD)(((WORD)(a)<<8)|(b)))
#define		MAKEDWORD(a,b)		((DWORD)(((DWORD)(a)<<16)|(b)))

#define		BYTESWAP(a)			(MAKEWORD(LOBYTE(a), HIBYTE(a)))

#define		ADDR2ROW(a,b)		((WORD)((((a)/(b))*2)|(((a)%(b))/((b)/2))))
#define		ADDR2COL(a,b)		((WORD)((a)%((b)/2)))
#define		ROW2ADDR(a,b)		((DWORD)((a)/2*(b))+(((a)&1)*((b)/2)))

#define		ROUNDUP(a,b)		((WORD)(((a)/(b))+(((a)%(b))? 1:0)))
#define		ROUNDDN(a,b)		((WORD)(((a)/(b))))


#define 	CPU_ALIGN_BIG_ENDIAN	0	// "1" for big-endian, "0" for little-endian

#define 	I2C_MISP100_ADDR	0xDE	// MISP100 I2C slave address
#define 	I2C_FPGA100_ADDR	0xD2	// FPGA100 I2C slave address

//#define		SYSTEM_USE_OSC36M			// use 36Mhz OSC

#define		MISP_USE_BUS_HIF			// MISP100 setting with AHB

#define		MISP_TUNE_RGB_GAIN_OFFSET	0	// "1" for tune gain/offset, "0" for not tune

#define 	__MISP100_DBGPRT__		0	// "1" for debug print, "0" for disable debug


//Compile time assertion
//; useful to check the range of constant at compile time
//; error message will be "size of array CASSERT_failed_msg_NN is negative" if expr is FALSE
#define CASSERT(expr, msg) _impl_CASSERT_LINE(expr,__LINE__,msg)
#define _impl_CASSERT_PASTE(a,b) a##b
#define _impl_CASSERT_LINE(expr, line, msg) \
    typedef char _impl_CASSERT_PASTE(CASSERT_failed_##msg##_,line)[(expr)?1:-1]


typedef	enum {
	MISP_NO_ERROR = 0,		// no error
	MISP_I2C_ERROR,			// i2c error (i2c_busy, i2c_no_ack)
	MISP_TIMEOUT_ERROR,		// timeout error
	MISP_INVALID_FORMAT,	// invalid src/out video format
	MISP_INVALID_PARAM,		// invalid parameters
	MISP_INVALID_DEV_ID,	// invalid device-ID

	MISP_DDC_ACK_ERROR,		// ddc error (ddc_busy, ddc_no_ack)
	MISP_DDC_CRC_ERROR,		// crc error (edid_crc)
	MISP_SHA_CMP_ERROR,		// sha error (compare vi)
	MISP_SPI_ERROR			// spi error

}	MISP_ERROR_t;

typedef	enum {
	MISP_HOST_ID	= 0xc0,		// host block ID
	MISP_LOCAL_ID	= 0xc2,		// local block ID
	MISP_HDMI_ID	= 0xc4,		// hdmi block ID
	MISP_SDRAM_ID	= 0xc6,		// sdram block ID
	MISP_FPGA_ID	= 0xc8,		// fpga block ID
	MISP_SDRAM_TEST_ID	= 0xca	// sdram block ID (within row region) added on 08May2012

}	MISP_BLOCK_ID_t;

#ifdef		SYSTEM_USE_ARM_LINUX
#define		stPACKED			__attribute__((packed))
#else
#define		stPACKED
#endif

#if 0
#define	MISP_RegWrite(id,addr,data)					MISPBUS_RegWrite(id,addr,data)
#define	MISP_RegRead(id,addr,pBuf)					MISPBUS_RegRead(id,addr,pBuf)
#define	MISP_RegField(id,addr,pos,cnt,data)			MISPBUS_RegField(id,addr,pos,cnt,data)
#else

//Using direct memory access via macro ==> fast running & small code size
#define		MISP_RegWrite(id,addr,data)	\
	do { switch ((id)&0xfe) {	\
		case MISP_HOST_ID:	*((volatile WORD*)(MISP_HOST_BASE_ADDR+(addr)*4)) = (data); break;	\
		case MISP_LOCAL_ID:	*((volatile WORD*)(MISP_LOCAL_BASE_ADDR+(addr)*4)) = (data); break;	\
		case MISP_SDRAM_ID:	*((volatile BYTE*)(MISP_SDRAM_BASE_ADDR+(addr))) = (BYTE)(data); break;	\
	} } while(0)

#define		MISP_RegRead(id,addr,pBuf)	\
	do { switch ((id)&0xfe) {	\
		case MISP_HOST_ID:	*(WORD*)(pBuf) = *((volatile WORD*)(MISP_HOST_BASE_ADDR+(addr)*4)); break;	\
		case MISP_LOCAL_ID:	*(WORD*)(pBuf) = *((volatile WORD*)(MISP_LOCAL_BASE_ADDR+(addr)*4)); break;	\
		case MISP_SDRAM_ID:	*(BYTE*)(pBuf) = *((volatile BYTE*)(MISP_SDRAM_BASE_ADDR+(addr))); break;	\
	} } while(0)

#define		MISP_RegField(id,addr,pos,cnt,data)	\
	do { \
		/* EISC compiler ignores assertion expression if pos or cnt is not constant */	\
		CASSERT((pos)>=0 && (pos)<16 && (cnt)>0 && (cnt)<=16 && ((pos)+(cnt))<=16, MISP_RegField);	\
		WORD m = (~((~0)<<(cnt)))<<(pos);	\
		WORD r; \
		MISP_RegRead(id, addr, &r);	\
		r = (r&~m) | (((data)<<(pos))&m);	\
		MISP_RegWrite(id, addr, r);	\
	} while(0)
#endif

//Using function call for burst access
#define		MISP_MultiWrite(id,addr,pBuf,bytes)			MISPBUS_Write(id,addr,pBuf,bytes)
#define		MISP_MultiRead(id,addr,pBuf,bytes)			MISPBUS_Read(id,addr,pBuf,bytes)

//Macro to check an error on MISP access function calls
// ==> cmd should be a function call with a return value!!!
//#define		MISP_CHK_ERR(cmd)	if(cmd) return MISP_I2C_ERROR
// ==> The following macro should be used when cmd returns no value!!!
#define		MISP_CHK_ERR(cmd)	cmd

#define		MISP_GetSizeOfBank()						2048


#endif	/* __MISPTYPE_H__ */
