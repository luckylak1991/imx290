// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"
#include	"osd.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
extern void uart_cmd_proc(void);

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
static void SystemMainCtrlLoop(void)
{
	while (TRUE) {
		ODMAutoCtrlHandler();
		HostCLK_ProcessHandler() ;
		SS_ProcessHandler();		// Sensor top
		DM_ProcessHandler();		// Demosaic process handler
		IN_ProcessHandler();		// Video In process handler
		OUT_ProcessHandler();		// Video out process handler
		FC_ProcessHandler();		// Video FC process handler
		DDRMAP_ProcessHandler();		
		DISCommunication();			// DIS
		DIS_OSD_Display();
		DZOOM_ProcessHandler();
		ADC_ProcessHandler();
		CSP_ProcessHandler();
		
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		ROT_ProcessHandler() ;		
		// TONE_ProcessHandler(); // TONE_ProcessHandler moved into ODMAutoCtrlHandler 2017.06.01
#endif

#if defined(__USE_FD_PROCESS__)
		FD_ProcessHandler();		// FD process handler
#endif
		
		VIDEO_RxHandler();
		VIDEO_TxHandler();
		
#if defined(__USE_CPU_SPI_SLAVE__)
		SPIs_ProcessHandler();
#endif
		
		VISCA_ProcessHandler();
		PELCO_ProcessHandler();
		
		XRCDebugMsgHandler();
#if defined(__USE_USB_VOUT__)
		mu3USBCommandHandler();
#endif
		MessageCtrlHandler();
		MenuDisplayHandler();
		MenuItemExeHandler();
		
//		IrCodeDebugHandler();
//		ButtonDebugHandler();
//		uart_cmd_proc();

#ifndef  __USE_IMX226_1080P60_MODE__
		StableClkConvSens();		//imx226 stable booting..
#endif

		SetAPP_SyncLock_Handler();

#if defined(__USE_4K60_INOUT__)
		SetAPP_Set4K30P60P_Handler();
#endif
	}
}

//--------------------------------------------------------------------------------------------------
int main(void)
{
	while (TRUE) {
		uComOnChipInitial();
		Delay_mSec(500);
		
		CreateVideoInstance();
		CreateOSDInstance();
		
#if defined(__USE_MDIN_i510__)||defined(__USE_MDIN_i540__)||defined(__USE_MDIN_i550__)
		APPICATION_initialize();
#endif
		
		VISCA_Initialize();
		PELCO_Initialize();

#if defined(__USE_AFZOOM_LENS__)
		#if !defined(__USE_4K60_MD2__)
		VIDEO_ChkTunePINT(0);
		#endif
#endif
//		VIDEO_ChkTuneCOMP(1);
//		VIDEO_ChkTunePOLY(2);

		SystemMainCtrlLoop();	// system ctrl main loop
	}
}

/*  FILE_END_HERE */
