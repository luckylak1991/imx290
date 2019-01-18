/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author	:	Software Team.
 Title	:	Timer Control
******************************************************************************/

#pragma once

//======================================================================================================================
// Timer Pre-scale Control Register (TPxCTRL)

#define R_TP0CTRL	((volatile unsigned int*)0x80010400)
#define R_TP1CTRL	((volatile unsigned int*)0x80010420)
#define R_TP2CTRL	((volatile unsigned int*)0x80010440)
#define R_TP3CTRL	((volatile unsigned int*)0x80010460)
#define R_TP4CTRL	((volatile unsigned int*)0x80010480)
#define R_TP5CTRL	((volatile unsigned int*)0x800104A0)
#define R_TPCTRL(x)	((volatile unsigned int*)(0x80010400+(x*0x20)))		// x = 0 ~ 5

// [ 1] RW CNTCLR : Pre-scale Counter and Timer Counter Reset
// When this bit is '1' the Timer Pre-scale and Counter will by reset
#define F_TPCTRL_CNTCLR_CLR		( 0<< 1)	// Reset Clear
#define F_TPCTRL_CNTCLR_SET		( 1<< 1)	// Reset Set
#define F_TPCTRL_CNTCLR(x)		( x<< 1)	// x = 0, 1

// [ 0] RW CLKSEL : Pre-scale Clock Selection
// 0 : System Clock
// 1 : CAPx
#define F_TPCTRL_CLKSEL_SYSCLK	( 0<< 0)	// System Clock
#define F_TPCTRL_CLKSEL_CAP		( 1<< 0)	// CAPx
#define F_TPCTRL_CLKSEL(x)		( x<< 0)	// x = 0, 1
//======================================================================================================================

//======================================================================================================================
// Timer Control Register (TMxCTRL)

#define R_TM0CTRL	((volatile unsigned int*)0x80010404)
#define R_TM1CTRL	((volatile unsigned int*)0x80010424)
#define R_TM2CTRL	((volatile unsigned int*)0x80010444)
#define R_TM3CTRL	((volatile unsigned int*)0x80010464)
#define R_TM4CTRL	((volatile unsigned int*)0x80010484)
#define R_TM5CTRL	((volatile unsigned int*)0x800104A4)
#define R_TMCTRL(x)	((volatile unsigned int*)(0x80010404+(x*0x20)))		// x = 0 ~ 5

// [15:14] RW TMOD : Timer/Counter Mode
#define F_TMCTRL_TMOD_TM		( 0<<14)	// Timer
#define F_TMCTRL_TMOD_PWM		( 1<<14)	// PWM
#define F_TMCTRL_TMOD_CAP		( 2<<14)	// Capture

// [13] RW OCEN : Output Compare Mode Enable Bit for channel 2 and channel 3
// 0 : Disable
// 1 : Enable
#define F_TMCTRL_OCEN_DIS		( 0<<13)	// Disable
#define F_TMCTRL_OCEN_EN		( 1<<13)	// Enable
#define F_TMCTRL_OCEN(x)		( x<<13)	// x = 0, 1

// [11] RW OVST : Capture Overflow Status bit
// Cleared Read
#define F_TMCTRL_OVST			( 1<<11)	// Overflow Status

// [10: 8] RW CAPMOD : Capture Mode Selection
#define F_TMCTRL_CAPMOD_LHP		( 0<< 8)	// Low/High Pulse Capture Mode
#define F_TMCTRL_CAPMOD_LP		( 2<< 8)	// Low Pulse Capture Mode
#define F_TMCTRL_CAPMOD_HP		( 3<< 8)	// High Pulse Capture Mode
#define F_TMCTRL_CAPMOD_FFED	( 4<< 8)	// Falling to Falling Period Capture Mode
#define F_TMCTRL_CAPMOD_RRED	( 6<< 8)	// Rising to Rising Period Capture Mode

// [ 6] RW PWMO : PWM Output One Period Generation
// 0 : Disable
// 1 : Enable
#define F_TMCTRL_PWMO_DIS		( 0<< 6)	// Disable
#define F_TMCTRL_PWMO_EN		( 1<< 6)	// Enable
#define F_TMCTRL_PWMO(x)		( x<< 6)	// x = 0, 1

// [ 5] RW PWML : PWM Output Start Level
// 0 : Start Level is Low
// 1 : Start Level is High
#define F_TMCTRL_PWML_LLV		( 0<< 5)	// Low Level
#define F_TMCTRL_PWML_HLV		( 1<< 5)	// High Level
#define F_TMCTRL_PWML(x)		( x<< 5)	// x = 0, 1

// [ 3: 1] RW PFSEL : Pre-scale Factor Selection
#define F_TMCTRL_PFSEL_2		( 0<< 1)	// 1/2
#define F_TMCTRL_PFSEL_8		( 1<< 1)	// 1/8
#define F_TMCTRL_PFSEL_32		( 2<< 1)	// 1/32
#define F_TMCTRL_PFSEL_128		( 3<< 1)	// 1/128
#define F_TMCTRL_PFSEL_512		( 4<< 1)	// 1/512
#define F_TMCTRL_PFSEL_2048		( 5<< 1)	// 1/2048
#define F_TMCTRL_PFSEL_8192		( 6<< 1)	// 1/8192
#define F_TMCTRL_PFSEL_32768	( 7<< 1)	// 1/32768
#define F_TMCTRL_PFSEL(x)		( x<< 1)	// x = 0, 1 ~ 7

// [ 0] RW TMEN : Timer/Counter or PWM Enable
// 0 : Disable
// 1 : Enable
#define F_TMCTRL_TMEN_DIS		( 0<< 0)	// Disable
#define F_TMCTRL_TMEN_EN		( 1<< 0)	// Enable
#define F_TMCTRL_TMEN(x)		( x<< 0)	// x = 0, 1
//======================================================================================================================

//======================================================================================================================
// Timer Counter Registers (TMxCNT)

#define R_TM0CNT	((volatile unsigned int*)0x80010408)
#define R_TM1CNT	((volatile unsigned int*)0x80010428)
#define R_TM2CNT	((volatile unsigned int*)0x80010448)
#define R_TM3CNT	((volatile unsigned int*)0x80010468)
#define R_TM4CNT	((volatile unsigned int*)0x80010488)
#define R_TM5CNT	((volatile unsigned int*)0x800104A8)
#define R_TMCNT(x)	((volatile unsigned int*)(0x80010408+(x*0x20)))		// x = 0 ~ 5

// [15: 0]	if Timer Mode then WRITE : Timer Counter Value, READ : Current Up-counter Value
//			if PWM Mode then RW : PWM Period Value
#define F_TMCNT(x)				(x << 0)
//======================================================================================================================

//======================================================================================================================
// Capture Counter / PWM Duty / Output Compare A Register (TMxDUT)

#define R_TM0DUT	((volatile unsigned int*)0x8001040C)
#define R_TM1DUT	((volatile unsigned int*)0x8001042C)
#define R_TM2DUT	((volatile unsigned int*)0x8001044C)
#define R_TM3DUT	((volatile unsigned int*)0x8001046C)
#define R_TM4DUT	((volatile unsigned int*)0x8001048C)
#define R_TM5DUT	((volatile unsigned int*)0x800104AC)
#define R_TMDUT(x)	((volatile unsigned int*)(0x8001040C+(x*0x20)))		// x = 0 ~ 5

// [15: 0]	if Capture Mode then READ : Result value of counting at the sampling period
//			if PWM Mode then RW : PWM Duty Value
//			if Output Compare Mode then RW : Output Compare A Value(channel 2 and 3)
#define F_TMDUT(x)				(x << 0)
//======================================================================================================================

//======================================================================================================================
// PWM Pulse Count / Output Compare B Register (TMxPUL)

#define R_TM0PUL	((volatile unsigned int*)0x80010410)
#define R_TM1PUL	((volatile unsigned int*)0x80010430)
#define R_TM2PUL	((volatile unsigned int*)0x80010450)
#define R_TM3PUL	((volatile unsigned int*)0x80010470)
#define R_TM4PUL	((volatile unsigned int*)0x80010490)
#define R_TM5PUL	((volatile unsigned int*)0x800104B0)
#define R_TMPUL(x)	((volatile unsigned int*)(0x80010410+(x*0x20)))		// x = 0 ~ 5

// [15: 0]	if PWM Mode then RW : PWM Pulse Number Value
//			if Output Compare Mode then RW : Output Compare B Value(channel 2 and 3)
#define F_TMPUL(x)				(x << 0)
//======================================================================================================================


#define TIMER_BASE_ADDR	0x80010400

// #define TMRST_ADDR(ch)		(TIMER_BASE_ADDR+(0x1000*ch))
#define TMRST_ADDR(ch)		(TIMER_BASE_ADDR + (ch*0x20))

// #define TMCTRL_ADDR(ch) 	(TIMER_BASE_ADDR+(0x1000*ch)+4)
#define TMCTRL_ADDR(ch) 	(TIMER_BASE_ADDR + (ch*0x20) + 4)

// #define TMCNT_ADDR(ch)		(TIMER_BASE_ADDR+(0x1000*ch)+8)
#define TMCNT_ADDR(ch)		(TIMER_BASE_ADDR + (ch*0x20) + 8)

//#define MAX_TIMER_CHANNEL 5
#define MAX_TIMER_CHANNEL 4
#define MAX_TIMER_COUNT (0xFFFF)

BOOL set_timer_ms(U32 nCh, U32 ms);
BOOL stop_timer(U32 nCh);
BOOL delayms(U32 ms); //return false, if ms is too big, check the timer.c
void ISPM delayus(U32 us);
