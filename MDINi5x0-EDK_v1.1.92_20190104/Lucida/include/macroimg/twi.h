/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author	:	Software Team.
 Title	:	Sync. TWI Control
******************************************************************************/

#pragma once

#define TWI_CH0	0
#define TWI_CH1	1
#define TWI_CH2	2

#define TWI_CH	TWI_CH0
//#define TWI_CH	TWI_CH1
//#define TWI_CH	TWI_CH2

/******************************************************************************
	INTERNAL TIME OUT
******************************************************************************/
/*
	TWI Function에서 Status를 기다리는 최대 시간을 설정한다.
*/
#define TWI_RESP_TIME_OUT_COUNT (7200*100)	// About 100ms @ AHB 101MHz //bh 2015.09.23

//======================================================================================================================
// TWI Control Register (TWICTRL)

#define R_TWICTRL(N)	((volatile unsigned int*)(0x80011000 + (N * 0x400)))
#define R_TWI0CTRL	((volatile unsigned int*)0x80011000)
#define R_TWI1CTRL	((volatile unsigned int*)0x80011400)
#define R_TWI2CTRL	((volatile unsigned int*)0x80011800)

// [ 7] R/W TWIEN : TWI Controller Enable
#define F_TWICTRL_TWIEN_DIS		( 0<< 7)		// Disable
#define F_TWICTRL_TWIEN_EN		( 1<< 7)		// Enable
#define F_TWICTRL_TWIEN(x)		( x<< 7)		// x = 1, 0

// [ 5] R/W TWIMOD : Master/Slave Mode Select
#define F_TWICTRL_TWIMOD_SLAVE	( 0<< 5)		// Slave, Stop Condition
#define F_TWICTRL_TWIMOD_MASTER	( 1<< 5)		// Master, Start Condition
#define F_TWICTRL_TWIMOD(x)		( x<< 5)		// x = 1, 0

// [ 4] R/W TWITR : Transmit/Receive Mode Select
#define F_TWICTRL_TWITR_RX		( 0<< 4)		// Master Receive
#define F_TWICTRL_TWITR_TX		( 1<< 4)		// Master Transmit
#define F_TWICTRL_TWITR(x)		( x<< 4)		// x = 1, 0

// [ 3] R/W TWIAK : Transmit Acknowlede Enable
#define F_TWICTRL_TWIAK_ACK		( 0<< 3)		// Master Acknowledge
#define F_TWICTRL_TWIAK_NACK	( 1<< 3)		// Master No Acknowledge
#define F_TWICTRL_TWIAK(x)		( x<< 3)		// x = 1, 0

// [ 2] R/W REPST : Repeated Start
#define F_TWICTRL_REPST			( 1<< 2)		// Repeated START Contidion

// [ 1] R/W TCIE : Transfer Complete Interrupt Enable Bit
#define F_TWICTRL_TCIE_DIS		( 0<< 1)		// Disable
#define F_TWICTRL_TCIE_EN		( 1<< 1)		// Enable
#define F_TWICTRL_TCIE(x)		( x<< 1)		// x = 1, 0

// [ 0] R/W LSTIE : Lost Arbitration Interrupt Enable Bit
#define F_TWICTRL_LSTIE_DIS		( 0<< 0)		// Disable
#define F_TWICTRL_LSTIE_EN		( 1<< 0)		// Enable
#define F_TWICTRL_LSTIE(x)		( x<< 0)		// x = 1, 0
//======================================================================================================================

//======================================================================================================================
// TWI Status Register (TWISTAT)

#define R_TWISTAT(N)	((volatile unsigned int*)(0x80011004 + (N * 0x400)))
#define R_TWI0STAT	((volatile unsigned int*)0x80011004)
#define R_TWI1STAT	((volatile unsigned int*)0x80011404)
#define R_TWI2STAT	((volatile unsigned int*)0x80011804)

// [ 9] R/W TXEMPTY : TX Buffer Empty
#define F_TWISTAT_TXEMPTY_FULL	( 0<< 9)		// Full
#define F_TWISTAT_TXEMPTY_EMPTY	( 1<< 9)		// Empty
#define F_TWISTAT_TXEMPTY(x)	( x<< 9)		// x = 1, 0

// [ 8] R/W RXFULL : RX Buffer Full
#define F_TWISTAT_RXFULL_EMPTY	( 0<< 8)		// Empty
#define F_TWISTAT_RXFULL_FULL	( 1<< 8)		// Full
#define F_TWISTAT_RXFULL(x)		( x<< 8)		// x = 1, 0

// [ 7] R/W TWIDT : Data Transferring Bit
#define F_TWISTAT_TWIDT_DOING	( 0<< 7)		// Transferring
#define F_TWISTAT_TWIDT_DONE	( 1<< 7)		// Transfer Done, Clear
#define F_TWISTAT_TWIDT(x)		( x<< 7)		// x = 1, 0

// [ 6] R/W TWIAS : Addressed as Slave Bit
#define F_TWISTAT_TWIAS_NMATCH	( 0<< 6)		// Not Matched
#define F_TWISTAT_TWIAS_MATCH	( 1<< 6)		// Matched
#define F_TWISTAT_TWIAS(x)		( x<< 6)		// x = 1, 0

// [ 5] R/W TWIBUSY : Bus Busy Bit
#define F_TWISTAT_TWIBUSY_IDLE	( 0<< 5)		// Bus Idle
#define F_TWISTAT_TWIBUSY_BUSY	( 1<< 5)		// Bus Busy
#define F_TWISTAT_TWIBUSY(x)	( x<< 5)		// x = 1, 0

// [ 4] R/W TWILOST : Lost Arbitration Bit
#define F_TWISTAT_TWILOST_NOCCR	( 0<< 4)		// Not Occured
#define F_TWISTAT_TWILOST_OCCR	( 1<< 4)		// Occured
#define F_TWISTAT_TWILOST(x)	( x<< 4)		// x = 1, 0

// [ 3] R/W TWISRW : Slave Read/Write Bit
#define F_TWISTAT_TWISRW_RX		( 0<< 3)		// Slave Rx Mode
#define F_TWISTAT_TWISRW_TX		( 1<< 3)		// Slave Tx Mode
#define F_TWISTAT_TWISRW(x)		( x<< 3)		// x = 1, 0

// [ 1] R/W RSF : Repeated Start Flag
#define F_TWISTAT_RSF_NOCCR		( 0<< 1)		// Not Occured or Stop Condition Occured
#define F_TWISTAT_RSF_OCCR		( 1<< 1)		// Occured
#define F_TWISTAT_RSF(x)		( x<< 1)		// x = 1, 0

// [ 0] R/W TWIRXAK : Received Acknowledge Bit
#define F_TWISTAT_TWIRXAK_ACK	( 0<< 0)		// Received Acknowledge
#define F_TWISTAT_TWIRXAK_NACK	( 1<< 0)		// Received No Acknowledge
#define F_TWISTAT_TWIRXAK(x)	( x<< 0)		// x = 1, 0
//======================================================================================================================

//======================================================================================================================
// TWI Address Register (TWIADR)

#define R_TWIADR(N)	((volatile unsigned int*)(0x80011008 + (N * 0x400)))
#define R_TWI0ADR	((volatile unsigned int*)0x80011008)
#define R_TWI1ADR	((volatile unsigned int*)0x80011408)
#define R_TWI2ADR	((volatile unsigned int*)0x80011808)

// [7 : 1] RW 7-Bit Slave Address
#define F_TWIADR(x)				( x<< 1)
//======================================================================================================================

//======================================================================================================================
// TWI Data Register (TWIDAT)

#define R_TWIDAT(N)	((volatile unsigned int*)(0x8001100C + (N * 0x400)))
#define R_TWI0DAT	((volatile unsigned int*)0x8001100C)
#define R_TWI1DAT	((volatile unsigned int*)0x8001140C)
#define R_TWI2DAT	((volatile unsigned int*)0x8001180C)

// [7 : 0] RW TWI Data
#define F_TWIDAT(x)				( x<< 0)
//======================================================================================================================

//======================================================================================================================
// TWI Baud Rate 0 Register (TWIBR0)

#define R_TWIBR0(N)	((volatile unsigned int*)(0x80011010 + (N * 0x400)))
#define R_TWI0BR0	((volatile unsigned int*)0x80011010)
#define R_TWI1BR0	((volatile unsigned int*)0x80011410)
#define R_TWI2BR0	((volatile unsigned int*)0x80011810)

// [7 : 0] RW Baudrate 0 Value TWIBR0>=3
#define F_TWIBR0(x)				( x<< 0)
//======================================================================================================================

//======================================================================================================================
// TWI Baud Rate 1 Register (TWIBR1)

#define R_TWIBR1(N)	((volatile unsigned int*)(0x80011014 + (N * 0x400)))
#define R_TWI0BR1	((volatile unsigned int*)0x80011014)
#define R_TWI1BR1	((volatile unsigned int*)0x80011414)
#define R_TWI2BR1	((volatile unsigned int*)0x80011814)

// [7 : 0] RW Baudrate 1 Value TWIBR0>=0
#define F_TWIBR1(x)				( x<< 0)
//======================================================================================================================

#define F_TWI_RECEIVE         0x21
#define F_TWI_TRANSMIT        0x22

int twi_set_freq (U8 ch, U32 freq);
BOOL twi_trans_complete(U8 ch, U8 devaddr);
BOOL twi_wait_busy(U8 ch, U8 devaddr);
void twi_start (U8 ch, U8 devaddr, U8 *waddr, U32 len, U32 flags);
int twi_write(U8 ch, U8 devaddr, U8 *waddr, U32 len, U8 *buffer, U32 length);
int twi_write2(U8 ch, U8 devaddr, U8 waddr, U8 *buffer, U32 length);
int twi_read(U8 ch, U8 devaddr, U8 *waddr, U32 len, U8 *buffer, U32 length);

