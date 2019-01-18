/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#ifndef _DEFS_H_
#define _DEFS_H_

void TXHAL_InitMicroGpios (void);
void TXHAL_InitPreReset (void);
void TXHAL_InitPostReset (void);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// SiI9136
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define F_9136

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Project Definitions
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// Timers - Target system uses these timers
#define HALTIMER
#define ELAPSED_TIMER               0xFF
#define ELAPSED_TIMER1              0xFE

typedef enum TimerId
{
    TIMER_0 = 0,		// DO NOT USE - reserved for TimerWait()
    TIMER_POLLING,		// Reserved for main polling loop
    TIMER_2,			// Available
    TIMER_3,			// Available
    TIMER_SERIAL,
    TIMER_LINK_CHECK,
    TIMER_LINK_RESPONSE,
    TIMER_COUNT			// MUST BE LAST!!!!
} timerId_t;

#define T_MONITORING_PERIOD		10
#define R_INIT_PERIOD			10
#define R_MONITORING_PERIOD		600

#define TX_HW_RESET_PERIOD      200
#define RX_HW_RESET_PERIOD		600

#define INT_CONTROL 0x00 // Interrupt pin is push-pull and active high (this is normally 0x06)
#define SiI_DEVICE_ID           0xB4
#define SiI_DEVICE_STRING       "SiI 9136\n"

#define SCK_SAMPLE_EDGE			0x80

#define F_9022A_9334            // for now other TPI new features...
//#define F_9334            	// for Evita only features, such as CDC
#define CLOCK_EDGE_FALLING
//#define RX_ONBOARD		// Evita 300 DOES NOT HAVE A PEBBLES ATTACHED!
//#define RX_SAMABETH_ONBOARD	// Samabeth Receiver - For Evita 300 only
//#define DEEP_COLOR
#define I2S_AUDIO

//#define IS_SIMCONFIG		// This must be defined if any of the next 4 are defined
//#define IS_EVENT                // define to enable Events support
//#define IS_POWERUP_SIM          // define to enable Powerup simulation
//#define IS_VMODE_SIM            // define to enable Video Mode simulation
//#define IS_EDID_SIM             // define to enable EDID simulation


//#define DEV_SUPPORT_EDID
//#define DEV_SUPPORT_HDCP
//#define DEV_SUPPORT_CEC_FEATURE_ABORT
//#define DEV_SUPPORT_CEC_CONFIG_CPI_0
//#define DEV_SUPPORT_EHDMI

#define DEV_INDEXED_READ
#define DEV_INDEXED_WRITE
#define DEV_INDEXED_RMW

#define HDCP_DONT_CLEAR_BSTATUS



/*\
| | TPI API Version
\*/

#define TPI_PRODUCT_CODE		1

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Debug Definitions
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define DISABLE 0x00
#define ENABLE  0xFF

// Compile debug prints inline or not
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD2__)	// 4k60 slave only
#define CONF__TPI_DEBUG_PRINT   (DISABLE)
#define CONF__TPI_TRACE_PRINT	(DISABLE)
#else
#define CONF__TPI_DEBUG_PRINT   (ENABLE)
#define CONF__TPI_TRACE_PRINT	(ENABLE)
#endif
#define CONF__TPI_EDID_PRINT    (ENABLE)
#define CONF__TPI_CDC_PRINT		(ENABLE)
#define CONF__TPI_TIME_PRINT	(ENABLE)

/*\
| | Trace Print Macro
| |
| | Note: TPI_DEBUG_PRINT Requires double parenthesis
| | Example:  TPI_DEBUG_PRINT(("hello, world!\n"));
\*/

#if (CONF__TPI_TRACE_PRINT == ENABLE)
    #define TPI_TRACE_PRINT(x)      printf("[SIL%d] ", Sil9136ChipSel); printf x
#else
    #define TPI_TRACE_PRINT(x)
#endif

/*\
| | Debug Print Macro
| |
| | Note: TPI_DEBUG_PRINT Requires double parenthesis
| | Example:  TPI_DEBUG_PRINT(("hello, world!\n"));
\*/

#if (CONF__TPI_DEBUG_PRINT == ENABLE)
    #define TPI_DEBUG_PRINT(x)      printf("[SIL%d] ", Sil9136ChipSel); printf x
#else
    #define TPI_DEBUG_PRINT(x)
#endif

/*\
| | EDID Print Macro
| |
| | Note: To enable EDID description printing, both CONF__TPI_EDID_PRINT and CONF__TPI_DEBUG_PRINT must be enabled
| |
| | Note: TPI_EDID_PRINT Requires double parenthesis
| | Example:  TPI_EDID_PRINT(("hello, world!\n"));
\*/

#if (CONF__TPI_EDID_PRINT == ENABLE)
    #define TPI_EDID_PRINT(x)       TPI_DEBUG_PRINT(x)
#else
    #define TPI_EDID_PRINT(x)
#endif


/*\
| | CDC Print Macro
| |
| | Note: To enable CDC description printing, both CONF__TPI_CDC_PRINT and CONF__TPI_DEBUG_PRINT must be enabled
| |
| | Note: TPI_CDC_PRINT Requires double parenthesis
| | Example:  TPI_CDC_PRINT(("hello, world!\n"));
\*/

#if (CONF__TPI_CDC_PRINT == ENABLE)
    #define TPI_CDC_PRINT(x)       TPI_DEBUG_PRINT(x)
#else
    #define TPI_CDC_PRINT(x)
#endif


/*\
| | Timer Print Macro
| |
| | Note: To enable Timer description printing, both CONF__TPI_TIME_PRINT and CONF__TPI_DEBUG_PRINT must be enabled
| |
| | Note: TPI_CDC_PRINT Requires double parenthesis
| | Example:  TPI_CDC_PRINT(("hello, world!\n"));
\*/

#if (CONF__TPI_TIME_PRINT == ENABLE)
    #define TPI_TIME_PRINT(x)       TPI_DEBUG_PRINT(x)
#else
    #define TPI_TIME_PRINT(x)
#endif



#endif // _DEFS_H_

