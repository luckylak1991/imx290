/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.
 Title  :   Timer Control
******************************************************************************/

#pragma once

//======================================================================================================================
// UART Channel Receiver Buffer Register (UxRB)

#define R_U0RB		((volatile unsigned int*)0x80010800)

// [ 7: 0] RO When DLAB 0
#define F_URB(x)				( x<< 0)
//======================================================================================================================

//======================================================================================================================
// UART Channel Transmitter Holding Register (UxTH)

#define R_U0TH		((volatile unsigned int*)0x80010800)

// [ 7: 0] WO When DLAB 0
#define F_UTH(x)				( x<< 0)
//======================================================================================================================

//======================================================================================================================
// UART Channel Interrupt Enable Register (UxIE)

#define R_U0IE		((volatile unsigned int*)0x80010804)

// [ 2] RW When DLAB 0 RLSIEN : Receiver Line Status Interrupt Enable Bit
// 0 : Disable
// 1 : Enable
#define F_UIE_RLSIEN_DIS		( 0<< 2)	// Disable
#define F_UIE_RLSIEN_EN			( 1<< 2)	// Enable
#define F_UIE_RLSIEN(x)			( x<< 2)	// x = 0, 1

// [ 1] RW When DLAB 0 THEIEN : Transmitter Holding Empty Interrupt Enable Bit
// 0 : Disable
// 1 : Enable
#define F_UIE_THEIEN_DIS		( 0<< 1)	// Disable
#define F_UIE_THEIEN_EN			( 1<< 1)	// Enable
#define F_UIE_THEIEN(x)			( x<< 1)	// x = 0, 1

// [ 0] RW When DLAB 0 RDAIEN : Received Data Available Interrupt Enable Bit
// 0 : Disable
// 1 : Enable
#define F_UIE_RDAIEN_DIS		( 0<< 0)	// Disable
#define F_UIE_RDAIEN_EN			( 1<< 0)	// Enable
#define F_UIE_RDAIEN(x)			( x<< 0)	// x = 0, 1
//======================================================================================================================

//======================================================================================================================
// UART Channel Interrupt Identification Register (UxII)

#define R_U0II		((volatile unsigned int*)0x80010808)

// [ 7: 6] RO When DLAB 0 FIFOST : FIFOs Enabled Status Bit
#define F_UII_FIFOST_NFIFO		( 0<< 6)	// Not In FIFO Mode
#define F_UII_FIFOST_FIFO		( 3<< 6)	// FIFO Mode

// [ 3: 0] RO When DLAB 0 INTID : UART Interrupt ID
#define F_UII_INTID				(15<< 0)	// UART Interrupt ID
#define F_UII_INTID_NONE		( 1<< 0)	// None
#define F_UII_INTID_RLS			( 6<< 0)	// Receiver Line Status
#define F_UII_INTID_RDA			( 4<< 0)	// Received Data Available
#define F_UII_INTID_CTI			(12<< 0)	// Character Timeout Indication
#define F_UII_INTID_THR			( 2<< 0)	// Transmitter Holding Register Empty
//======================================================================================================================

//======================================================================================================================
// UART Channel FIFO Control Register (UxFC)

#define R_U0FC		((volatile unsigned int*)0x80010808)

// [ 7: 6] WO When DLAB 0/RO When DLAB 1 RFTL : Receiver FIFO Trigger Level
#define F_UFC_RFTL_1			( 0<< 6)	// 1 Byte
#define F_UFC_RFTL_4			( 1<< 6)	// 4 Byte
#define F_UFC_RFTL_8			( 2<< 6)	// 8 Byte
#define F_UFC_RFTL_14			( 3<< 6)	// 14 Byte
#define F_UFC_RFTL(x)			( x<< 6)	// x = 0, 1, 2, 3

// [ 2] WO When DLAB 0/RO When DLAB 1 XFR : XMIT FIFO Reset not in Shift Register
#define F_UFC_XFR				( 1<< 2)	// Reset

// [ 1] WO When DLAB 0/RO When DLAB 1 RFR : RCVR FIFO Reset not in Shift Register
#define F_UFC_RFR				( 1<< 1)	// Reset

// [ 0] WO When DLAB 0/RO When DLAB 1 FIFOEN : FIFO Enable Bit
// 0 : 16450 UART Mode
// 1 : Enable FIFO
#define F_UFC_FIFOEN_DIS		( 0<< 0)	// 16450 UART Mode
#define F_UFC_FIFOEN_EN			( 1<< 0)	// Enable FIFO
#define F_UFC_FIFOEN(x)			( x<< 0)	// x = 0, 1
//======================================================================================================================

//======================================================================================================================
// UART Channel Line Control Register (UxLC)

#define R_U0LC		((volatile unsigned int*)0x8001080C)

// [ 7] RW DLAB : Divisor Latch Access Bit
#define F_ULC_DLAB_DIS			( 0<< 7)	// Disable
#define F_ULC_DLAB_EN			( 1<< 7)	// Enable
#define F_ULC_DLAB(x)			( x<< 7)	// x = 0, 1

// [ 6] RW SB : Set Break
#define F_ULC_SB_DIS			( 0<< 6)	// Disable
#define F_ULC_SB_EN				( 1<< 6)	// Enable
#define F_ULC_SB(x)				( x<< 6)	// x = 0, 1

// [ 5] RW SP : Stick Parity
// 0 : Disables Stick Parity
// 1 : Parity Bit 0 When PEN, EPS, SP 1
//      Parity Bit 1 When PEN, SP 1 EPS 0
#define F_ULC_SP_DIS			( 0<< 5)	// Disable
#define F_ULC_SP_EN				( 1<< 5)	// Enable
#define F_ULC_SP(x)				( x<< 5)	// x = 0, 1

// [ 4] RW EPS : Even Parity Select
// 0 : Select Odd Parity
// 1 : Select Even Parity
#define F_ULC_EPS_ODD			( 0<< 4)	// Odd
#define F_ULC_EPS_EVEN			( 1<< 4)	// Even
#define F_ULC_EPS(x)			( x<< 4)	// x = 0, 1

// [ 3] RW PEN : Parity Enable Bit
// 0 : Disable Parity
// 1 : Enable Parity
#define F_ULC_PEN_DIS			( 0<< 3)	// Disable
#define F_ULC_PEN_EN			( 1<< 3)	// Enable
#define F_ULC_PEN(x)			( x<< 3)	// x = 0, 1

// [ 2] RW STB : Number of Stop Bit
// 0 : 1 Stop Bit
// 1 : 2 Stop Bit(1.5 Stop Bit When WLS 5 Bits)
#define F_ULC_STB_1				( 0<< 2)	// 1 Stop
#define F_ULC_STB_2				( 1<< 2)	// 2 Stop
#define F_ULC_STB(x)			( x<< 2)	// x = 0, 1

// [ 1: 0] RW WLS : Word Length Select
#define F_ULC_WLS_5				( 0<< 0)	// 5 Bits/Character
#define F_ULC_WLS_6				( 1<< 0)	// 6 Bits/Character
#define F_ULC_WLS_7				( 2<< 0)	// 7 Bits/Character
#define F_ULC_WLS_8				( 3<< 0)	// 8 Bits/Character
//======================================================================================================================

//======================================================================================================================
// UART Channel Line Status Register (UxLS)

#define R_U0LS		((volatile unsigned int*)0x80010814)

// [ 7] RO EIRF : Error in RCVR FIRO
// 0 : 16450 UART Mode
// 1 : When One of OE, PE, FE, BI Set
#define F_ULS_EIRF				( 1<< 7)

// [ 6] RO TEMP : Transmitter Empty
// 0 : THR or TSR is not Empty
// 1 : Both THR(Transmitter FIFO) and TSR is Empty
#define F_ULS_TEMP				( 1<< 6)

// [ 5] RO THRE : Transmitter Holding Register Empty
// 0 : THR is not Empty
// 1 : THR(Transmitter FIFO) is Empty
#define F_ULS_THRE				( 1<< 5)

// [ 4] RO BINT : Break Interrupt
// 1 : Received Data is '0' while Full-word transmitting, Cleared when CPU Read LSR.
#define F_ULS_BINT				( 1<< 4)

// [ 3] RO FERR : Framing Error
// 1 : Received Data have Invalid Stop Bit, Cleared when CPU Read LSR.
#define F_ULS_FERR				( 1<< 3)

// [ 2] RO PERR : Parity Error
// 1 : The Parity Bit of Received Data is different with UxLC's PEN. Cleared when CPU Read LSR.
#define F_ULS_PERR				( 1<< 2)

// [ 1] RO OERR : Overrun Error
// 1 : UxRB(Receiver Shift Register) is overwritten. Cleared when CPU Read LSR.
#define F_ULS_OERR				( 1<< 1)

// [ 0] RO DRDY : Data Ready
// 1 : UxRB(FIFO) is written. Cleared when CPU Read LSR.
#define F_ULS_DRDY				( 1<< 0)
//======================================================================================================================

//======================================================================================================================
// Uart Channel Divisor Latch LSB Register (UxDLL)

#define R_U0DLL		((volatile unsigned int*)0x80010800)

// [ 7: 0] RW When DLAB 1 Divisor latch Least Significant Byte
#define F_UDLL(x)				( x<< 0)
//======================================================================================================================

//======================================================================================================================
// Uart Channel Divisor Latch MSB Register (UxDLM)

#define R_U0DLM		((volatile unsigned int*)0x80010804)

// [ 7: 0] RW When DLAB 1 Divisor latch Most Significant Byte
#define F_UDLM(x)				( x<< 0)
//======================================================================================================================

//======================================================================================================================
// Uart IrDA Mode Register (UxIRM)

#define R_U0IRM		((volatile unsigned int*)0x80010888)

// [ 5] RW IrDA Rx Polarity Inversion
#define F_UIRM_RXPI				( 1<< 5)

// [ 4] RW IrDA Rx Decoding Enable
#define F_UIRM_RXDE				( 1<< 4)

// [ 1] RW IrDA Tx Polarity Inversion
#define F_UIRM_TXPI				( 1<< 1)

// [ 0] RW IrDA Tx Encoding Enable
#define F_UIRM_TXEE				( 1<< 0)
//======================================================================================================================

/***********************************************
* UART registers
***********************************************/
#define R_UART0_BASE ((volatile unsigned int*)0x80010800)
#define R_UART_BASE(N) ((volatile unsigned int*)(0x80010800+((N)*0x20)))
#define R_UART2_BASE 	((volatile unsigned int*)0x80012400)				// USER APB01 -- for UART2

#if 1
#define UART_RBR        0		// UART Channel Receiver Buffer Registers
#define UART_THR        0		//UART Channel Transimitter Holding Registers
#define UART_DIV_LO     0		//UART Channel Divisor Latch LSB Registers
#define UART_DIV_HI     (1*4) 	//UART Channel Divisor Latch MSB Registers
#define UART_IER        (1*4) 	//UART Channel Interrupt Enable Registers
#define UART_IIR        (2*4) 	//UART Channel Interrupt Identification Registers
#define UART_FCR        (2*4) 	//UART Channel FIFO Control Registers
#define UART_LCR        (3*4) 	//UART Channel Line Control Registers
#define UART_LSR        (5*4) 	//UART Channel Line Status Registers
#endif

typedef enum{
	DATABITS_5=5,
	DATABITS_6=6,
	DATABITS_7=7,
	DATABITS_8=8,
}UART_DATABITS;

typedef enum{
	STOPBITS_1=1,
	STOPBITS_2=2,
}UART_STOPBITS;

typedef enum{
	UART_PARNONE=0,
	UART_PARODD,
	UART_PAREVEN,
}UART_PARITY;

#define	UART_RX_USERBUF_EMPTY	2 /* Rx buffer is empty, no character available */
#define	UART_TX_USERBUF_FULL	3 /* Tx buffer is full, could not deposit char */
#define	UART_TX_USERBUF_EMPTY	4 /* If the Tx buffer is empty. */
#define	UART_RX_USERBUF_TIMEOUT	5 /* If a timeout occurred while waiting for a character*/
#define	UART_TX_USERBUF_TIMEOUT	6 /* If a timeout occurred while waiting to send a char.*/

#define UART_FIFO_DEPTH 16
#define UART_CHANNEL_MAX 8


BOOL uart_config(int ch,int baud,UART_DATABITS databits,UART_STOPBITS stopbit,UART_PARITY parity);
BOOL uart_getch(int n,char* ch);
int uart_getdata(int n,U8* buf, int bufmax);//return data byte length
BOOL uart_putch(int n,char ch);
BOOL uart_putdata(int n,U8* buf, int len);
BOOL uart_putstring(int n,U8* buf);

void uart_rx_flush(int ch);
void uart_tx_flush(int ch);

void set_debug_channel(int ch);
int get_debug_channel();
void debugprintf(const char * const format, ...);
void debugstring(const char* str);

void uart_rx_int_enable(int ch,BOOL b);	// bh 2015.07.29
void uart_tx_int_enable(int ch,BOOL b);	// bh 2015.07.29

BOOL uart2_config(int baud,UART_DATABITS databits,UART_STOPBITS stopbit,UART_PARITY parity);	//bh 2016.10.10
ISPM BOOL uart2_putch(char ch);			//bh 2016.10.10
ISPM BOOL uart2_getch(char* ch);		//bh 2016.10.10
void uart2_rx_int_enable(BOOL b);		//bh 2016.10.10
void uart2_tx_int_enable(BOOL b);		//bh 2016.10.10
void uart2_status_int_enable(BOOL b);	//bh 2016.10.10

#define PRINTLINE	debugprintf("%s , %d \r\n",__FILE__,__LINE__)
#define PRINTVAR(A)	debugprintf("%s , %d :%s=0x%x(%d)\r\n",__FILE__,__LINE__,#A,A,A)

//#define ASSERT(A)	if(!(A))debugprintf("ASSERT,%s,%d:%s \r\n",__FILE__,__LINE__,#A)
#define ASSERT(A)	
