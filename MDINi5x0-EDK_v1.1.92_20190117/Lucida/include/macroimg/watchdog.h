/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author	:	Software Team.
 Title	:	Watchdog Timer Control
******************************************************************************/

#pragma once

//======================================================================================================================
// Watchdog Timer Control Register (WDTCTRL)

#define R_WDTCTRL	((volatile unsigned int*)0x80010000)

// [ 4] RO Clear WDTST : Watchdog Timer Status Bit
// WDTMOD 0 : (0 : No watchdog reset 1 : Watchdog reset)
// WDTMOD 1 : (0 : No watchdog interrupt 1 : Watchdog interrupt)
#define F_WDTCTRL_WDTST			( 1<< 4)

// [ 1] R/W WDTMOD : Watchdog Timer Mode Select Bit
#define F_WDTCTRL_WDTMOD_RST	( 0<< 1)		// Reset Mode
#define F_WDTCTRL_WDTMOD_INT	( 1<< 1)		// Interrupt Mode
#define F_WDTCTRL_WDTMOD(x)		( x<< 1)		// x = 0, 1

// [ 0] R/W WDTEN : Watchdog Timer Enable Bit
#define F_WDTCTRL_WDTEN_DIS		( 0<< 0)		// Disable
#define F_WDTCTRL_WDTEN_EN		( 1<< 0)		// Enable
#define F_WDTCTRL_WDTEN(x)		( x<< 0)		// x = 0, 1
//======================================================================================================================

//======================================================================================================================
// Watchdog Timer Counter Value Register (WDTCNT)

#define R_WDTCNT  ((volatile unsigned int*)0x80010004)

// [31: 0] R/W Watchdog Timer Counter 32bit Value
#define F_WDTCNT(x)				( x<< 0)		// x = 0 ~ 0xFFFF_FFFF
//======================================================================================================================

