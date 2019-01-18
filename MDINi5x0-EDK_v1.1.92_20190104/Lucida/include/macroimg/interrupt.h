/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author	:	Software Team.
 Title	:	Interrupt Control
******************************************************************************/

#pragma once

//======================================================================================================================
// Interrupt Pending Clear Register (PENDCLR)

#define R_PENDCLR	((volatile unsigned int*)0xFFFF0000)

// [ 7: 0] WO Interrupt Pending Register Clear Value
#define F_PENDCLR(x)			(x+0x20)	// Ex. F_PENDCLR(INTNUM_EIRQ0)
//======================================================================================================================

//======================================================================================================================
// External Interrupt Mode and External PIN Level Register (EIRQMOD)

#define R_EIRQMOD	((volatile unsigned int*)0xFFFF0004)

// [ 3] RO EIRQ0ST : EIRQ0 PIN Level
#define F_EIRQMOD_EIRQ0ST		( 1<< 3)	// 1 : High Level 0 : Low Level

// [ 2: 0] RW EIRQMOD : EIRQ0 Active Status
#define F_EIRQMOD_EIRQ0LLV			( 0<< 0)	// Low Level
#define F_EIRQMOD_EIRQ0HLV			( 1<< 0)	// High Level
#define F_EIRQMOD_EIRQ0FED			( 2<< 0)	// Falling Edge
#define F_EIRQMOD_EIRQ0RED			( 3<< 0)	// Rising Edge
#define F_EIRQMOD_EIRQ0AED			( 4<< 0)	// Any Edge

// [ 7] RO EIRQ0ST : EIRQ1 PIN Level
#define F_EIRQMOD_EIRQ1ST		( 1<< 7)	// 1 : High Level 0 : Low Level

// [ 6: 4] RW EIRQMOD : EIRQ1 Active Status
#define F_EIRQMOD_EIRQ1LLV			( 0<< 4)	// Low Level
#define F_EIRQMOD_EIRQ1HLV			( 1<< 4)	// High Level
#define F_EIRQMOD_EIRQ1FED			( 2<< 4)	// Falling Edge
#define F_EIRQMOD_EIRQ1RED			( 3<< 4)	// Rising Edge
#define F_EIRQMOD_EIRQ1AED			( 4<< 4)	// Any Edge

// [ 11] RO EIRQ0ST : EIRQ2 PIN Level
#define F_EIRQMOD_EIRQ2ST		( 1<< 11)	// 1 : High Level 0 : Low Level

// [ 10: 8] RW EIRQMOD : EIRQ2 Active Status
#define F_EIRQMOD_EIRQ2LLV			( 0<< 8)	// Low Level
#define F_EIRQMOD_EIRQ2HLV			( 1<< 8)	// High Level
#define F_EIRQMOD_EIRQ2FED			( 2<< 8)	// Falling Edge
#define F_EIRQMOD_EIRQ2RED			( 3<< 8)	// Rising Edge
#define F_EIRQMOD_EIRQ2AED			( 4<< 8)	// Any Edge
//======================================================================================================================

//======================================================================================================================
// Internal Interrupt Mode Register (IINTMOD)

#define R_IINTMOD	((volatile unsigned int*)0xFFFF0008)

// [15: 1] RW Vector No. x Interrupt Mode Bit
#define F_IINTMOD_HLV(x)		( 0<< x)	// High Level Mode Ex. F_IINTMOD(INTNUM_TIMER0)
#define F_IINTMOD_RED(x)		( 1<< x)	// Rising Edge Mode Ex. F_IINTMOD(INTNUM_TIMER0)
//======================================================================================================================

//======================================================================================================================
// Interrupt Pending Register (INTPEND)

#define R_INTPEND	((volatile unsigned int*)0xFFFF000C)

// [15: 0] RO Vector No. x Interrupt Pending Bit
#define F_INTPEND(x)			( 1<< x)	// Ex. F_INTPEND(INTNUM_EIRQ0)
//======================================================================================================================

//======================================================================================================================
// Interrupt Enable Register (INTEN)

#define R_INTEN		((volatile unsigned int*)0xFFFF0010)

// [15: 0] RW Vector No. x Interrupt Enable Bit
#define F_INTEN_DIS(x)			( 0<< x)	// Interrupt Disable Ex. F_INTEN(INTNUM_EIRQ0)
#define F_INTEN_EN(x)			( 1<< x)	// Interrupt Enable Ex. F_INTEN(INTNUM_EIRQ0)
//======================================================================================================================

//======================================================================================================================
// Interrupt Mask Status Register (INTMASK)

#define R_INTMASK	((volatile unsigned int*)0xFFFF0014)

// [15: 0] RO Vector No. x Interrupt mask Status Bit
#define F_INTMASK(x)			( 1<< x)	// F_INTMASK(INTNUM_EIRQ0)
//======================================================================================================================

//======================================================================================================================
// Interrupt Mask Set Register (MASKSET)

#define R_MASKSET	((volatile unsigned int*)0xFFFF0014)

// [15: 0] WO Vector No. x Interrupt Request Set Bit
// 0 : No Effect
// 1 : Pending Interrupt is allowed to become active (Interrupts sent to CPU)
#define F_MASKSET(x)			( 1<< x)	// Ex. F_MASKSET(INTNUM_EIRQ0)
//======================================================================================================================

//======================================================================================================================
// Interrupt Mask Clear Register (MASKCLR)

#define R_MASKCLR	((volatile unsigned int*)0xFFFF0018)

// [15: 0] WO Vector No. x Interrupt Request Clear Bit
// 0 : No Effect
// 1 : Pending Interrupt is masked from becoming active (Interrupts not sent to CPU)
#define F_MASKCLR(x)			( 1<< x)	// Ex. F_MASKCLR(INTNUM_EIRQ0)
//======================================================================================================================

//======================================================================================================================
// Interrupt Vector and Priority

typedef enum 
{
	INTNUM_EIRQ0	= 0	,	// External Interrupt 0
	INTNUM_TIMER0		,	// Timer Ch0 Interrupt
	INTNUM_UINT0		,	// User Interrupt 0
	INTNUM_UINT1		,	// User Interrupt 1

	INTNUM_EIRQ1	= 4	,	// External Interrupt 1
	INTNUM_TIMER1		,	// Timer Ch1 Interrupt
	INTNUM_UINT2		,	// User Interrupt 2
	INTNUM_UINT3		,	// User Interrupt 3

	INTNUM_EIRQ2	= 8	,	// External Interrupt 2
	INTNUM_SPI0			,	// SPI Ch0 Interrupt
	INTNUM_SPI1			,	// SPI Ch1 Interrupt
	INTNUM_UART0		,	// UART Ch0 Interrupt

	INTNUM_EIRQ3	= 12,	// External Interrupt 3
	INTNUM_TIMER2		,	// Timer Ch2 Interrupt
	INTNUM_GPIOA		,	// GPIO A Group Interrupt
	INTNUM_GPIOB		,	// GPIO B Group Interrupt
	
	INTNUM_EIRQ4	= 16,	// External Interrupt 4
	INTNUM_TIMER3		,	// Timer Ch3 Interrupt
	INTNUM_GPIOC		,	// GPIO C Group Interrupt
	INTNUM_GPIOD		,	// GPIO D Group Interrupt
	
	INTNUM_EIRQ5	= 20,	// External Interrupt 5
	INTNUM_TIMER4		,	// Timer Ch4 Interrupt
	INTNUM_TIMER5		,	// Timer Ch5 Interrupt
	INTNUM_WDT			,	// Watchdog Timer Interrupt

	INTNUM_EIRQ6	= 24,	// External Interrupt 5
	INTNUM_TWI0			,	// TWI Ch0 Interrupt
	INTNUM_TWI1			,	// TWI Ch1 Interrupt
	INTNUM_TWI2			,	// TWI Ch1 Interrupt
		
	INTNUM_EIRQ7	= 28,	// External Interrupt 5
	INTNUM_ADC			,	// DMA Interrupt
	INTNUM_UART1		,	// UART Ch1 Interrupt
	INTNUM_CAP			,	// Capture Interrupt
	
	INTNUM_MAX
}INTERRUPT_TYPE;
//======================================================================================================================

#define CRITICAL_ENTER()	asm("set 8");asm("sync") // back up interrupt status
#define CRITICAL_EXIT()		asm("clr 8");asm("sync") // restor interrupt status
#define INTERRUPT_ENABLE	asm("set 13");
#define INTERRUPT_DISABLE	asm("clr 13");
#define NMI_ENABLE			asm("set 14");
#define NMI_DISABLE			asm("clr 14");
#define SETVECTORED			asm("set 12");

void init_interrupt();
BOOL set_interrupt(INTERRUPT_TYPE intnum, void (*fp)());
void* get_interrupt(INTERRUPT_TYPE intnum);
void enable_interrupt(INTERRUPT_TYPE num, BOOL b);

