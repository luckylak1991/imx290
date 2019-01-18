/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.
 Titile	: UART Control
******************************************************************************/

#include "macroimg.h"
#include "board.h"
#define UART_MAX 2

#if defined(__USE_USB_VOUT__)
#define USB_UART_USES_RX_RING_BUFFER
#endif

#ifdef CONFIG_UART_RX_INTERRUPT
//#warning "UART RX interrupt mode Build"
	#undef USB_UART_USES_RX_RING_BUFFER
#endif
#ifdef CONFIG_UART_TX_INTERRUPT
//#warning "UART TX interrupt mode Build"
#endif

#define RTL_SIM 0
//#define RTL_SIM 1
#if RTL_SIM
#define VUART   *(volatile unsigned char *)0x80010800
#endif


typedef struct _UART_RING_BUF {

//#ifdef CONFIG_UART_RX_INTERRUPT
#if defined(CONFIG_UART_RX_INTERRUPT)||defined(USB_UART_USES_RX_RING_BUFFER)	
	U8    *RingBufRxInPtr;  // Pointer to where next character will be inserted
	U8    *RingBufRxOutPtr; // Pointer from where next character will be extracted
	U8    RingBufRx[UART_BUF_SIZE]; // Ring buffer character storage (Rx)
	U16   RingBufRxCtr;     // Number of characters in the Rx ring buffer
#endif

#ifdef CONFIG_UART_TX_INTERRUPT
	U8    *RingBufTxInPtr;    // Pointer to where next character will be inserted
	U8    *RingBufTxOutPtr; // Pointer from where next character will be extracted
	U8    RingBufTx[UART_BUF_SIZE];  // Ring buffer character storage (Tx)
	U16   RingBufTxCtr;     // Number of characters in the Tx ring buffer
#endif
		
} UART_RING_BUF;

#ifdef USB_UART_USES_RX_RING_BUFFER
static UART_RING_BUF ubuf[1];
#else
static UART_RING_BUF ubuf[UART_MAX];
#endif

#ifdef USB_UART_USES_RX_RING_BUFFER
static void InitRingBuf(int ch)
{
	if(ch != 1)
		return;
	
	UART_RING_BUF* pRing = &ubuf[0];
		
	pRing->RingBufRxCtr    = 0;
	pRing->RingBufRxInPtr  = &pRing->RingBufRx[0];
	pRing->RingBufRxOutPtr = &pRing->RingBufRx[0];
}
#else
static void InitRingBuf(int ch)
{
#if defined(CONFIG_UART_RX_INTERRUPT)||defined(CONFIG_UART_TX_INTERRUPT)	// bh 2015.07.29
	UART_RING_BUF* pRing = &ubuf[ch];
#endif
		
#ifdef CONFIG_UART_RX_INTERRUPT
	pRing->RingBufRxCtr    = 0;
	pRing->RingBufRxInPtr  = &pRing->RingBufRx[0];
	pRing->RingBufRxOutPtr = &pRing->RingBufRx[0];
#endif
#ifdef CONFIG_UART_TX_INTERRUPT
	pRing->RingBufTxCtr    = 0;
	pRing->RingBufTxInPtr  = &pRing->RingBufTx[0];
	pRing->RingBufTxOutPtr = &pRing->RingBufTx[0];
#endif	
}
#endif // #ifdef USB_UART_USES_RX_RING_BUFFER

#ifdef USB_UART_USES_RX_RING_BUFFER
static inline UART_RING_BUF* getringbuf(int ch)
{
	if(ch != 1)
		return NULL;
	return &ubuf[0];
}
#else
static inline UART_RING_BUF* getringbuf(int ch)
{
	return &ubuf[ch];
}
#endif  //#ifdef USB_UART_USES_RX_RING_BUFFER


void uart_rx_int_enable(int ch,BOOL b)
{
	U32	uart_base = (U32)R_UART_BASE(ch);
	if(b)
		*(U32*)(uart_base+UART_IER) |= F_UIE_RDAIEN_EN;
	else
		*(U32*)(uart_base+UART_IER) &= ~F_UIE_RDAIEN_EN;
}

void uart_tx_int_enable(int ch,BOOL b)
{
	U32	uart_base = (U32)R_UART_BASE(ch);
	if(b)
		*(U32*)(uart_base+UART_IER) |= F_UIE_THEIEN_EN;
	else
		*(U32*)(uart_base+UART_IER) &= ~F_UIE_THEIEN_EN;
}

void uart_status_int_enable(int ch,BOOL b)
{
	U32	uart_base = (U32)R_UART_BASE(ch);
	if(b)
		*(U32*)(uart_base+UART_IER) |= F_UIE_RLSIEN_EN;
	else
		*(U32*)(uart_base+UART_IER) &= ~F_UIE_RLSIEN_EN;
}


static void getuartfifo(U32 channel, UART_RING_BUF* pbuf)
{
	U32 uart_base = (U32)R_UART_BASE(channel);
	U32 iir;
//#if defined(CONFIG_UART_RX_INTERRUPT)||defined(CONFIG_UART_TX_INTERRUPT)	// bh 2015.07.29
#if defined(CONFIG_UART_RX_INTERRUPT)||defined(CONFIG_UART_TX_INTERRUPT)||defined(USB_UART_USES_RX_RING_BUFFER)	
	char ch;
#endif
	
	iir = *(volatile U32*)(uart_base + UART_IIR);
	iir &= 0xf;
	
	switch(iir)
	{
#ifdef CONFIG_UART_TX_INTERRUPT
	case 2:
		if(pbuf->RingBufTxCtr > 0)
		{
			int i;
			int txmax = pbuf->RingBufTxCtr>UART_FIFO_DEPTH?UART_FIFO_DEPTH:pbuf->RingBufTxCtr;
			// int txmax = pbuf->RingBufTxCtr;
			for(i=0;i<txmax;i++)
			{
				*(volatile U8*)(uart_base+UART_THR) = *pbuf->RingBufTxOutPtr;
				pbuf->RingBufTxOutPtr++;
				if(pbuf->RingBufTxOutPtr == &pbuf->RingBufTx[UART_BUF_SIZE])
				{
					pbuf->RingBufTxOutPtr = &pbuf->RingBufTx[0];
				}
			}
			pbuf->RingBufTxCtr -= i;
		}
		if(pbuf->RingBufTxCtr == 0)
			uart_tx_int_enable(channel,FALSE);
		break;
#endif
//#ifdef CONFIG_UART_RX_INTERRUPT
#if defined(CONFIG_UART_RX_INTERRUPT)||defined(USB_UART_USES_RX_RING_BUFFER)	
	case 6:
		debugstring("UART Line Status Error(Overrun,Frame,Parity)\r\n");
	case 4:
	case 0xc:
		while((*(volatile U32*)(uart_base + UART_LSR)) & F_ULS_DRDY)//data ready
		{
			ch = *(volatile U8*)(uart_base+UART_RBR);
			if (pbuf->RingBufRxCtr < UART_BUF_SIZE) 
			{ 
				pbuf->RingBufRxCtr++; 
				*pbuf->RingBufRxInPtr=ch;
				pbuf->RingBufRxInPtr++;
				/* Wrap OUT pointer     */
				if (pbuf->RingBufRxInPtr == &pbuf->RingBufRx[UART_BUF_SIZE]) {
					pbuf->RingBufRxInPtr = &pbuf->RingBufRx[0];
				}
			}
		}
		break;
#endif
	}
}

void Uart0ISR()
{
	getuartfifo(0,&ubuf[0]);
}

#ifdef USB_UART_USES_RX_RING_BUFFER
void Uart1ISR()
{
	getuartfifo(1,&ubuf[0]);
}
#else
void Uart1ISR()
{
	getuartfifo(1,&ubuf[1]);
}
#endif

BOOL uart_config(int ch,int baud,UART_DATABITS databits,UART_STOPBITS stopbit,UART_PARITY parity)
{
#if RTL_SIM
#else

	U32 apbclock;
	volatile U32 uart_base;
	
	if(ch >= UART_MAX)
		return FALSE;
	
	uart_base = (U32)R_UART_BASE(ch);
	
	InitRingBuf(ch);
	
	apbclock = get_apb_clock();
	U16 divisor = apbclock/(baud*16) ;
	
	if( (apbclock%(baud*16)) > ((baud*16)/2))
		divisor++;
	
	U32 lcr;//line control register
		
	lcr = ((stopbit - 1) << 2) + (databits - 5);
	
	switch (parity) 
	{
		case UART_PARODD:
			lcr |= F_ULC_PEN_EN;
			break;
		case UART_PAREVEN:
			lcr |= F_ULC_PEN_EN | F_ULC_EPS_EVEN;
			break;
		case UART_PARNONE:
		default:
			break;
	}
	
	*(volatile U32*)(uart_base + UART_FCR) = F_UFC_FIFOEN_EN | F_UFC_RFTL_1;
	lcr |= F_ULC_DLAB_EN;
	*(volatile U32*)(uart_base + UART_LCR) = lcr;
	*(volatile U32*)(uart_base + UART_DIV_LO) = divisor&0xff;
	*(volatile U32*)(uart_base + UART_DIV_HI) = (divisor>>8)&0xff;
	lcr &= ~F_ULC_DLAB_EN; //divisor latch access disable, fifo control write mode.
	*(volatile U32*)(uart_base + UART_LCR) = lcr;
	*(volatile U32*)(uart_base + UART_IER) = 0;//disable rx/tx interrupts
		
	 uart_rx_flush(ch);
	 uart_tx_flush(ch);
	 
#if defined(CONFIG_UART_RX_INTERRUPT)||defined(CONFIG_UART_TX_INTERRUPT)
	switch(ch)
	{
	case 0:
		set_interrupt(INTNUM_UART0, Uart0ISR);
		enable_interrupt(INTNUM_UART0, TRUE);
		break;
	case 1:
		set_interrupt(INTNUM_UART1,Uart1ISR);
		enable_interrupt(INTNUM_UART1,TRUE);
		break;
	}	
#endif
	
#ifdef CONFIG_UART_RX_INTERRUPT
	uart_rx_int_enable(ch,TRUE);
#endif
	
#ifdef 	 USB_UART_USES_RX_RING_BUFFER	
	*R_IINTMOD  &= ~(1<<INTNUM_UART1);	//for the first empty interrupt	//for lucida
	set_interrupt(INTNUM_UART1,Uart1ISR);
	enable_interrupt(INTNUM_UART1,TRUE);
	uart_rx_int_enable(ch,TRUE);
	 
#endif		

#endif
	return TRUE;
}

void uart_rx_flush(int ch)
{
	U32 fcr, uart_base = (U32)R_UART_BASE(ch);
	CRITICAL_ENTER();
//	*(volatile U32*)(uart_base + UART_FCR) |= F_UFC_RFR;  //bh 2015.09.17
	*(volatile U32*)(uart_base + UART_LCR) = *(volatile U32*)(uart_base + UART_LCR) | F_ULC_DLAB_EN;  //bh 2015.09.17
	fcr = *(volatile U32*)(uart_base + UART_FCR);
	*(volatile U32*)(uart_base + UART_LCR) = *(volatile U32*)(uart_base + UART_LCR) & (~F_ULC_DLAB_EN);
  *(volatile U32*)(uart_base + UART_FCR) = fcr | F_UFC_RFR;			// reset fifo
  *(volatile U32*)(uart_base + UART_FCR) = fcr;						// clear fifo reset bit
	CRITICAL_EXIT();
}
void uart_tx_flush(int ch)
{
	U32 fcr, uart_base = (U32)R_UART_BASE(ch);	//bh 2015.09.17
	CRITICAL_ENTER();
//	*(volatile U32*)(uart_base + UART_FCR) |= F_UFC_XFR;//tx fifo reset //bh 2015.09.17
  *(volatile U32*)(uart_base + UART_LCR) = *(volatile U32*)(uart_base + UART_LCR) | F_ULC_DLAB_EN;  //bh 2015.09.17
	fcr = *(volatile U32*)(uart_base + UART_FCR);
	*(volatile U32*)(uart_base + UART_LCR) = *(volatile U32*)(uart_base + UART_LCR) & (~F_ULC_DLAB_EN);
	*(volatile U32*)(uart_base + UART_FCR) = fcr | F_UFC_XFR;			// reset fifo
	*(volatile U32*)(uart_base + UART_FCR) = fcr;						// clear fifo reset bit
	CRITICAL_EXIT();
}


#ifdef USB_UART_USES_RX_RING_BUFFER

BOOL uart_getch(int n,char* ch)
{
	if(n==0)
	{
		U32 uart_base = (U32)R_UART_BASE(n);
	
		if(uart_base==0)
			return FALSE;
	
		U32 lsr = *(U32*)(uart_base + UART_LSR);
	
		if(lsr & F_ULS_DRDY)
		{
			*ch = *(u8*)(uart_base+UART_RBR);
			return TRUE;
		}
		else
			return FALSE;
		
	}else
	{
		UART_RING_BUF* pbuf = getringbuf(n);
	
		if(pbuf==0)
			return FALSE;
	
		CRITICAL_ENTER();
		if (pbuf->RingBufRxCtr > 0)
		{ 
			pbuf->RingBufRxCtr--; 				// No, decrement character count
			*ch = *pbuf->RingBufRxOutPtr++;		// Get character from buffer
		
			// Wrap OUT pointer
			if (pbuf->RingBufRxOutPtr == &pbuf->RingBufRx[UART_BUF_SIZE])
			{
				pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];
			}
			CRITICAL_EXIT();
			return TRUE;
		}
		else
		{
			CRITICAL_EXIT();
			return FALSE;
		}
	}
}

#else
BOOL uart_getch(int n,char* ch)
{
#ifdef CONFIG_UART_RX_INTERRUPT
	UART_RING_BUF* pbuf = getringbuf(n);
	
	if(pbuf==0)
		return FALSE;
	
	CRITICAL_ENTER();
	if (pbuf->RingBufRxCtr > 0)
	{ 
		pbuf->RingBufRxCtr--; 				// No, decrement character count
		*ch = *pbuf->RingBufRxOutPtr++;		// Get character from buffer
		
		// Wrap OUT pointer
		if (pbuf->RingBufRxOutPtr == &pbuf->RingBufRx[UART_BUF_SIZE])
		{
			pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];
		}
		CRITICAL_EXIT();
		return TRUE;
	}
	else
	{
		CRITICAL_EXIT();
		return FALSE;
	}	

#else
	U32 uart_base = (U32)R_UART_BASE(n);
	
	if(uart_base==0)
		return FALSE;
	
	U32 lsr = *(U32*)(uart_base + UART_LSR);
	
	if(lsr & F_ULS_DRDY)
	{
		*ch = *(u8*)(uart_base+UART_RBR);
		return TRUE;
	}
	else
		return FALSE;
#endif
}
#endif //#ifdef USB_UART_USES_RX_RING_BUFFER



int uart_getdata(int n,U8* buf, int bufmax)
{
	int i;
#ifdef CONFIG_UART_RX_INTERRUPT
	UART_RING_BUF* pbuf = getringbuf(n);
	if(pbuf==0)
		return FALSE;
	
	CRITICAL_ENTER();
	for(i=0;i<bufmax;i++)
	{
		if (pbuf->RingBufRxCtr > 0)
		{
			pbuf->RingBufRxCtr--; 					// No, decrement character count
			buf[i] = *pbuf->RingBufRxOutPtr++; 		// Get character from buffer
			
			// Wrap OUT pointer
			if (pbuf->RingBufRxOutPtr == &pbuf->RingBufRx[UART_BUF_SIZE])
			{
				pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];
			}
		}
		else
		{
			break;
		}
	}
	CRITICAL_EXIT();

#else
	U32 uart_base = (U32)R_UART_BASE(n);
	
	if(uart_base==0)
		return FALSE;
	
	for(i=0;i<bufmax;i++)
	{
		U32 lsr = *(U32*)(uart_base + UART_LSR);
		if(lsr & F_ULS_DRDY)
		{
			buf[i] = *(u8*)(uart_base+UART_RBR);
		}
		else
			break;
	}
#endif
	
	return i;
}
//ISPM BOOL uart_putch(int n,char ch)
BOOL uart_putch(int n,char ch)
{	
	U32 uart_base = (U32)R_UART_BASE(n);
	
	if(uart_base==0)
		return FALSE;
	
#ifdef CONFIG_UART_TX_INTERRUPT
	UART_RING_BUF* pbuf = getringbuf(n);
	
	if(pbuf==0)
		return FALSE;
	
	CRITICAL_ENTER();
	if (pbuf->RingBufTxCtr < UART_BUF_SIZE) 
	{
		pbuf->RingBufTxCtr++;    		// No, increment character count
		*pbuf->RingBufTxInPtr = ch;		// Put character into buffer
		pbuf->RingBufTxInPtr++;
		if (pbuf->RingBufTxInPtr == &pbuf->RingBufTx[UART_BUF_SIZE]) 
		{
			pbuf->RingBufTxInPtr = &pbuf->RingBufTx[0];
		}
		if((*(U32*)(uart_base+UART_IER) & F_UIE_THEIEN_EN)==0)
		{
			uart_tx_int_enable(n,TRUE);
		}
		CRITICAL_EXIT();
		return TRUE;
	}
	else 
	{
		CRITICAL_EXIT();
		return FALSE;	
	}
#else
	
	#if RTL_SIM
        VUART = ch;
	#else
		//wait empty
		while(!((*(volatile U32*)(uart_base + UART_LSR))  & F_ULS_TEMP));
		uart_tx_flush(n);								// bh 2015.07.29 -- clear tx fifo, not clear shift reg.
		*(volatile u8*)(uart_base+UART_THR) = ch;
	#endif
	
	return TRUE;
#endif
}

BOOL uart_putdata(int n,U8* buf, int len)
{
	int i;
	for(i=0;i<len;i++)
		uart_putch(n,buf[i]);
	return TRUE;
}
BOOL uart_putstring(int n,U8* buf)
{
	while(*buf)
	{
		uart_putch(n,*buf);
		buf++;
	}
	return TRUE;
}

BOOL uart_putstring(int n,U8* buf);

static int debugch;//=DEBUG_UART_CH;
void set_debug_channel(int ch)
{
	if(ch > UART_MAX)
		return;
	debugch = ch;
}
int get_debug_channel()
{
	return debugch;
}

//ISPM void debugstring(const char* str)
void debugstring(const char* str)
{
	while(*str)
	{
#ifdef CONFIG_UART_TX_INTERRUPT
		while(1)
		{
			if(uart_putch(debugch,*str))
				break;
		}
#else
		uart_putch(debugch,*str);
#endif
		str++;
	}
}


/******************************************************************************
 uart-2 driver
******************************************************************************/

//--------------------------------------------------------------------------------------------------------------------------
static ISPM void uart2_rx_flush(void)
{
	U32 uart_base = (U32)R_UART2_BASE;	// UART2 base addr
	U32 fcr;
	
	CRITICAL_ENTER();
	*(volatile U32*)(uart_base + UART_LCR) = *(volatile U32*)(uart_base + UART_LCR) | F_ULC_DLAB_EN;
	fcr = *(volatile U32*)(uart_base + UART_FCR);
	*(volatile U32*)(uart_base + UART_LCR) = *(volatile U32*)(uart_base + UART_LCR) & (~F_ULC_DLAB_EN);
	*(volatile U32*)(uart_base + UART_FCR) = fcr | F_UFC_RFR;			// reset fifo
	*(volatile U32*)(uart_base + UART_FCR) = fcr;						// clear fifo reset bit
	CRITICAL_EXIT();
//	debugprintf("uart_mit_rx_flush, fcr(0x%x) \r\n", fcr);	//test
}

//--------------------------------------------------------------------------------------------------------------------------
static ISPM void uart2_tx_flush(void)
{
	U32 uart_base = (U32)R_UART2_BASE;	// UART2 base addr
	U32 fcr;
	
	CRITICAL_ENTER();
	*(volatile U32*)(uart_base + UART_LCR) = *(volatile U32*)(uart_base + UART_LCR) | F_ULC_DLAB_EN;
	fcr = *(volatile U32*)(uart_base + UART_FCR);
	*(volatile U32*)(uart_base + UART_LCR) = *(volatile U32*)(uart_base + UART_LCR) & (~F_ULC_DLAB_EN);
	*(volatile U32*)(uart_base + UART_FCR) = fcr | F_UFC_XFR;			// reset fifo
	*(volatile U32*)(uart_base + UART_FCR) = fcr;						// clear fifo reset bit
	CRITICAL_EXIT();
//	debugprintf("uart_mit_tx_flush, fcr(0x%x) \r\n", fcr);	//test
}

//-----------------------------------------------------------------------------------
void uart2_rx_int_enable(BOOL b)
{
	U32	uart_base = (U32)R_UART2_BASE;
	
	if(b)		*(volatile U32*)(uart_base+UART_IER) |= F_UIE_RDAIEN_EN;
	else		*(volatile U32*)(uart_base+UART_IER) &= ~F_UIE_RDAIEN_EN;
}

//-----------------------------------------------------------------------------------
void uart2_tx_int_enable(BOOL b)
{
	U32	uart_base = (U32)R_UART2_BASE;
	
	if(b)		*(volatile U32*)(uart_base+UART_IER) |= F_UIE_THEIEN_EN;
	else		*(volatile U32*)(uart_base+UART_IER) &= ~F_UIE_THEIEN_EN;
}

//-----------------------------------------------------------------------------------
void uart2_status_int_enable(BOOL b)
{
	U32	uart_base = (U32)R_UART2_BASE;
	
	if(b)		*(volatile U32*)(uart_base+UART_IER) |= F_UIE_RLSIEN_EN;
	else		*(volatile U32*)(uart_base+UART_IER) &= ~F_UIE_RLSIEN_EN;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL uart2_config(int baud,UART_DATABITS databits,UART_STOPBITS stopbit,UART_PARITY parity)
{
	U32 uart_base = (U32)R_UART2_BASE;	// UART2 base addr
	U32 apbclock, lcr;//, lst, iir, fcr;
	U16 divisor;

	apbclock = get_apb_clock();
	divisor = apbclock/(baud*16) ;
	
	if( (apbclock%(baud*16)) > ((baud*16)/2))		divisor++;
	
	lcr = ((stopbit - 1) << 2) + (databits - 5);
	
	switch (parity) {
		case UART_PARODD:		lcr |= F_ULC_PEN_EN;					break;
		case UART_PAREVEN:		lcr |= F_ULC_PEN_EN | F_ULC_EPS_EVEN;	break;
		case UART_PARNONE:		
		default:														break;
	}
/*	
	switch(fifo_bytes)	{
		case  0:	*(volatile U32*)(uart_base + UART_FCR) = F_UFC_FIFOEN_DIS;	break;					// non-fifo mode
		case  1:	*(volatile U32*)(uart_base + UART_FCR) = F_UFC_FIFOEN_EN | F_UFC_RFTL_1;	break;	// 1byte fifo
		case  2:	*(volatile U32*)(uart_base + UART_FCR) = F_UFC_FIFOEN_EN | F_UFC_RFTL_4;	break;	// 4byte fifo
		case  3:	*(volatile U32*)(uart_base + UART_FCR) = F_UFC_FIFOEN_EN | F_UFC_RFTL_8;	break;	// 8byte fifo
		case  4:	*(volatile U32*)(uart_base + UART_FCR) = F_UFC_FIFOEN_EN | F_UFC_RFTL_14;	break;	// 14byte fifo
		default:	*(volatile U32*)(uart_base + UART_FCR) = F_UFC_FIFOEN_DIS;	break;					// non-fifo mode
	}
*/	
	*(volatile U32*)(uart_base + UART_FCR) = F_UFC_FIFOEN_EN | F_UFC_RFTL_14;	// 14byte fifo
	
	lcr |= F_ULC_DLAB_EN;
	*(volatile U32*)(uart_base + UART_LCR) = lcr;
	*(volatile U32*)(uart_base + UART_DIV_LO) = divisor&0xff;
	*(volatile U32*)(uart_base + UART_DIV_HI) = (divisor>>8)&0xff;
	lcr &= ~F_ULC_DLAB_EN; //divisor latch access disable, fifo control write mode.
	*(volatile U32*)(uart_base + UART_LCR) = lcr;
	*(volatile U32*)(uart_base + UART_IER) = 0;//disable rx/tx interrupts
	
	uart2_rx_flush();	// needed when fifo enable..
	uart2_tx_flush();	// needed when fifo enable..
	
/*	
	lsr = *(U32*)(uart_base + UART_LSR);	// why need ?????
	
	iir = *(volatile U32*)(uart_base + UART_IIR);						// read IIR(interrupt identification register)
	*(volatile U32*)(uart_base + UART_LCR) = *(volatile U32*)(uart_base + UART_LCR) | F_ULC_DLAB_EN;	// r&w
	fcr = *(volatile U32*)(uart_base + UART_FCR);
	*(volatile U32*)(uart_base + UART_LCR) = *(volatile U32*)(uart_base + UART_LCR) & (~F_ULC_DLAB_EN);	// r&w
*/	
	
	return  TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
ISPM BOOL uart2_putch(char ch)
{
	U32 uart_base = (U32)R_UART2_BASE;	// UART2 base addr
	
	while(!((*(volatile U32*)(uart_base + UART_LSR))  & F_ULS_TEMP));	//wait empty
	
	uart2_tx_flush();		// bh 2015.07.29 -- clear tx fifo, not clear shift reg.
	
	*(volatile u8*)(uart_base + UART_THR) =	ch;
	
	return  TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------
ISPM BOOL uart2_getch(char* ch)
{
	U32 uart_base = (U32)R_UART2_BASE;	// UART2 base addr
	U32 lsr;
	
	lsr = *(volatile U32*)(uart_base + UART_LSR);
	
	if(lsr & F_ULS_DRDY)		{ *ch = *(volatile u8*)(uart_base + UART_RBR);	return  TRUE;}
	else						{ return  FALSE; }
}

