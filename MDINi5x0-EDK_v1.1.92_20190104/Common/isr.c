//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	ISR.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
VBYTE count_16ms, fSysIRQMsgODMVD, fSysStateSTABLE, fSysIRQCntODMVD, fIRQGAfinish=0, fVdPulse=0;
WORD  cntMessageOSD=0, cntDZTickDly=0;
VLONG g_tcount = 0;
#if	defined(__MDIN_i510_REF__)				// i510 REF board
BOOL  fIOExpIRQ = FALSE;
#endif
#if	(defined(__USE_CMOS_IMX265__) && !defined(__USE_IMX265_SLAVE_MODE__))
VBYTE reset_mscnt_trig=0;
VBYTE max_mscnt_trig=0;
#endif



BOOL  fTxIRQ = FALSE;

//int inti=0;
// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
extern BOOL DIS_OSD_en;
extern BYTE fMessageOSD, fMenuWaitFunc;
#if defined(__USE_IR_LED_PWM__)
extern BYTE pwm5_high, pwm5_low;
#endif
// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void ISPM EIRQ1_ISR()		//ISR - EIRQ1(IR_IN)
{
#if !defined(__MDIN_i540_4K60__)
	if((*R_EIRQMOD & (F_EIRQMOD_EIRQ1ST)) == 0)		IrReceivePulseDecode();
#endif
	*R_PENDCLR = (INTNUM_EIRQ1+0x20);	//clear int pending reg.
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM EIRQ2_ISR(void)		//ISR - EIQR2(IRQ_N)
{
	WORD	irq_id;
	
	if((*R_EIRQMOD & (F_EIRQMOD_EIRQ2ST)) == 0)		{
		
		irq_id = GetHOST_IRQID();
		
		if(irq_id & MISP_IRQ_VDPULSE)	{   // output vdpulse_host signal (DIS vector write)
			DIS_OSD_en = 1;
			DIS_VectorWrite();
			
			fVdPulse = 1;					// set vd-pulse flag
		}

		if(irq_id & MISP_IRQ_VDPULSE)			fAECtrlODM_XFER = TRUE;	// AE update (output vdpulse_host)
		if(irq_id & MISP_IRQ_ISP_VACT_OUT_3A)	ODMVsyncPulseDecode();
#if defined(__USE_CMOS_IMX226__)
		if(irq_id & MISP_IRQ_ISP_VACT_OUT_3A)	AE_UpdateDGainVBlank();
		if(irq_id & MISP_IRQ_ISP_VACT_OUT_3A)	AE_UpdateGainVBlank();
		if(irq_id & MISP_IRQ_ISP_VACT_OUT_3A)	AE_UpdateSensUpVBlank();
#endif
		if(irq_id & MISP_IRQ_DIS)				DIS_VsyncPulse();		// DIS run interrupt signal
		if(irq_id & MISP_IRQ_GA_GAC_FINISH)		fIRQGAfinish=1;			// check gac finish irq

	#if defined(__USE_FD_PROCESS__)
		if(irq_id & MISP_IRQ_WDR_INTERRUPT)     fFDCtrlProcCtrl|=FD_ITER_START;  //vact rise to start FD
		if(irq_id & MISP_IRQ_ISP_VACT_OUT_3A)   fFDCtrlProcCtrl|=FD_ITER_SEIZE;  //vact fall to seize FD
	#endif
	
#if defined(__USE_4K60_INOUT__)
		if(irq_id & MISP_IRQ_VDPULSE)			SetAPP_Set4K30P60P_Tick();	// for 4k30p60p demo mode
#endif
	}
	
	*R_PENDCLR = (INTNUM_EIRQ2+0x20);	//clear int pending reg with int vector No.
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM GPIO0_ISR()		// ISR - GPIO-A
{
#if defined(__USE_IRKEY_SWITCH__) //|| defined(__USE_KEY_TACK_SW__)
	if(*R_P0EDS & (1<<3))					//P0.3 -- SWITCH
	{
		IrkeyEnableToggle();
		*R_P0EDS = (1<<3);
	}
#endif
	
#if defined(__MDIN_i540_4K60__)
	if(*R_P0EDS & (1<<0))					//P0.0 -- IRR_IN
	{
		IrReceivePulseDecode();
		*R_P0EDS = (1<<0);
	}
#endif
	
	if(*R_P0EDS & (1<<2))					//P0.2 -- TX_INT
	{
		fTxIRQ = TRUE;
		*R_P0EDS = (1<<2);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM GPIO2_ISR()		// ISR - GPIO-C
{
#if	defined(__MDIN_i510_REF__)				// i510 REF board
	if(*R_P2EDS & (1<<2))					//P2.2 -- INT_IO_EXP
	{
		fIOExpIRQ = TRUE;
		*R_P2EDS = (1<<2);
	}
/*
#elif defined(__MDIN_i550_REF__) &&  defined(__USE_KEY_TACK_SW__)
	if(*R_P2EDS & (1<<1))					//P2.2 -- INT_IO_EXP
	{
		inti++;
//		UARTprintf("%d \r\n",inti);
		if(inti%2==1){		//for the key swich input 
//			DE_485(HIGH);
//			Delay_uSec(500);	
//			DE_485(LOW);
			
			SetGSTriggerPushPulse();			

				
		}
		
		*R_P2EDS = (1<<1);
	}
*/
#endif
}

#if defined(__USE_IR_LED_PWM__)
//--------------------------------------------------------------------------------------------------------------------------
void ISPM GPIO3_ISR()		// ISR - GPIO-D(internal gpio for cpu)
{
	if(*R_P3EDS & (1<<0))					//P3.0 -- isp_vact_out_3a(ODM Vsync)
	{
		if( (*R_P3ILEV & (1<<0)) == 1 )	{	//rising edge
			*R_TM5CTRL |= F_TMCTRL_TMEN_EN;		// timer5 enable
		}
		*R_P3EDS = (1<<0);
	}
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM MISC_Trigger(void)
{
	VISCA_VSyncTrigger();				// visca v.sync trigger
	PELCO_VSyncTrigger();				// pelco v.sync trigger
	VideoTurnOnCounter();				// v.out display turn on
	ResetWDTcounter();					// reset wdt counter every 1ms
	
#if defined(__USE_IT6802_RX__)
	SetIT680x_Tick();					// excute hdmi-rx main loop
#endif
	
#if defined(__USE_LT8619_RX__)
	SetLT8619C_Tick();					// excute hdmi-rx main loop
#endif
	
#if defined(__USE_IT66121_TX__)
	SetIT66121_Tick();					// excute hdmi-tx main loop
#endif

#if defined(__USE_IT6615_TX__)
	SetITE6615_Tick();					// excute hdmi-tx main loop
#endif
	
#if defined(__USE_SIL9136_TX__)
	SIL9136_SetPollingStart();			// excute hdmi-tx polling loop
#endif
	
#if defined(__USE_IRKEY_SWITCH__) 
	IrSwitchIrqCounter();				// set ir on/off switch irq enable delay
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void ISPM TimeService_16ms(void)	
{
	count_16ms++;//	if (count_16ms==1)	fAECtrlODM_XFER = TRUE;		// AE update
	
#if (defined(__USE_CMOS_IMX265__) && !defined(__USE_IMX265_SLAVE_MODE__))
	if(max_mscnt_trig>reset_mscnt_trig) reset_mscnt_trig++;
	SetGSTriggerOffHandler();
#endif
		
	switch ((count_16ms%16)) {
#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__))
#if !defined(__USE_4K60_MD2__)
		case  1:	LENS_CtrlSPILineBUSY();	break;
		case  2:	LENS_CtrlSPILineFREE();	break;
#endif
#endif
		case  3:	SerialTimeOutCounter();
//#if defined(__USE_USB_VOUT__)
//			silkyUSBTimeOutCounter();
//#endif		
					IrCodeTimeOutCounter(); break;

		case  5:	SerialCommandParse();	
//#if defined(__USE_USB_VOUT__)
//			saveSilkyUSBCommand();
//#endif			
					break;
			
		case  7:	IrCodeCommandParse();	break;		// parse ircode command
#if defined(__USE_CPU_SPI_SLAVE__)
		case  8:	SPIsAPP_SetRawUpdateFlag();	break;	// enable spi-slave raw data update flag
#endif
#if !defined(__USE_BUTTONKEY_DISABLE__)
		case  9:	ButtonCommandParse();	break;		// parse button command
#endif
		case 11:	MessageCtrlClassify();	break;		// parse control message
		case 13:	MISC_Trigger();			break;
	}

	//request ADC : twice every 16ms(DYNT_ProcessHandler, ButtonCommandParse)
	if(!(count_16ms%(16/ADC_SRC_NUM)))  ADC_CtrlConvRun();
}

LONG get_ms(void)
{
	return g_tcount;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM Timer2_Handler(void)		// Timer0 - internal timer
{
	TimeService_16ms();
	g_tcount++;
	
	if(fMessageOSD | fMenuWaitFunc)		cntMessageOSD++;
	
	if(fVdPulse)	if(cntDZTickDly++ >= 3)		{fVdPulse=0;	cntDZTickDly=0;	SetDZOOM_Tick();}	// tick : vdpulse + 3ms
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM Timer4_Handler(void)		// Timer4 - delay timer
{
	if (usDelay) usDelay--;
	if (msDelay) msDelay--;
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM UART0_ISR(void)		// Serial0 - UART0
{
	U32 iir;	BYTE ch;
	U32 uart_base = (U32) R_UART_BASE(0);		//UART0 base addr
	
	iir = * ( volatile U32 * ) ( uart_base + UART_IIR );
	iir &= 0xf;
	switch ( iir )
	{
		case 2:		SerialTxHandler();	break;	//Tx
		case 0xc:
		case 4:		uart_getch(0, &ch);
#if defined(__USE_UART0_VISCA__)
					if(fSysStateSTABLE)		VISCA_RxHandler(ch);
#endif
					if(fSysStateSTABLE)		SerialRxHandler(ch);
				break;
		case 6:		ch = *(PBYTE)(uart_base+UART_LSR);	break;	// overrun/parity/frame error(clear when read LSR)
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM UART1_ISR(void)		// Serial1 - UART1
{
	U32 iir;	BYTE ch;
	U32 uart_base = (U32) R_UART_BASE(1);		//UART1 base addr
	
	iir = * ( volatile U32 * ) ( uart_base + UART_IIR );
	iir &= 0xf;
	switch ( iir )
	{
//		case 2:		ViscaTxHandler();	break;	//Tx
		case 0xc:
		case 4:		
			
			uart_getch(1, &ch);
#if defined(__USE_USB_VOUT__)
			//silkyUSBRxHandler(ch);
			//break;
#endif		
			
#if !defined(__USE_UART0_VISCA__)
					if(fSysStateSTABLE)		VISCA_RxHandler(ch);
#endif
					if(fSysStateSTABLE)		PELCO_RxHandler(ch);
				break;
		case 6:		ch = *(PBYTE)(uart_base+UART_LSR);	break;	// overrun/parity/frame error(clear when read LSR)
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM UART2_ISR(void)
{
	U32 iir;	BYTE ch;
	U32 uart_base = (U32) R_UART2_BASE;		//UART2 base addr

	iir = * ( volatile U32 * ) ( uart_base + UART_IIR );
	iir &= 0xf;
	switch ( iir )
	{
//		case 2:		SerialTxHandler();	break;	//Tx
		case 0xc:
		case 4:	if( *(volatile U32*)(uart_base + UART_LSR) & F_ULS_DRDY )		{
						ch = *(volatile U8*)(uart_base + UART_RBR);
//						SerialRxHandler(ch);
					}
					break;	//Rx
		case 6:		ch = *(volatile U8*)(uart_base+UART_LSR);	break;	// overrun/parity/frame error(clear when read LSR)
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM ADC_ISR(void)		// ADC
{
	ADC_CtrlConvGet();
}

#if defined(__USE_IR_LED_PWM__)
//--------------------------------------------------------------------------------------------------------------------------
void ISPM PWM5_ISR(void)		// timer5 pwm out
{
	*R_TM5CTRL &= (~F_TMCTRL_TMEN_EN);		// pwm out diable.
	*R_TM5CNT = (pwm5_low+pwm5_high)  * (get_apb_clock()/8/1000);					// pwm-total(low + high) period
	*R_TM5DUT = pwm5_low  * (get_apb_clock()/8/1000);								// pwm-low period
}
#endif

//-------------------------------------------------------------------------------------
void ISPM SPI1_ISR(void)
{
#if defined(__USE_CPU_SPI_SLAVE__)
	if((*R_SPI0STAT(SPI_CH1) & SPISTAT_SPIF) && (*R_SPI0STAT(SPI_CH1) & SPISTAT_STXE))	{	// if SPI Finish & Tx FIFO empty..
		SPIs_RxHandler();
	}
#endif
}

//-------------------------------------------------------------------------------------
BOOL ISPM IsODMVsyncBlank(void)
{
	if(fSysIRQMsgODMVD == ON)		return TRUE;
	if(count_16ms == 0)				return TRUE;
	
	return FALSE;
}


/*  FILE_END_HERE */
