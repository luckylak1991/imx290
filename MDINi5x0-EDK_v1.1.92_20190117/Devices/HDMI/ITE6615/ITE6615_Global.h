#ifndef		__ITE6615_GOLBAL_H__
#define		__ITE6615_GOLBAL_H__

#include "common.h"

#if defined(__USE_IT6615_TX__)

#ifdef WIN32
#include <windows.h>
#include "..\\src\\USBI2C.h"
#endif

#include <string.h>    // for memcmp..

#include "ITE6615_Typedef.h"
#include "ITE6615_Config.h"
#include "ITE6615_Debug.h"

#include "ITE6615_DEV_DEFINE.h"

#if (_ENABLE_CEC_==TRUE)
    #include "ITE6615_CEC_DEFINE.h"
#endif

#include "ITE6615_DRV_TX.h"

#include "ITE6615_I2C_RDWR.h"

#include "ITE6615_EDID_PARSER.h"


#ifdef _MCU_8051_
// this file don't need add to PC Code
    #if _MCU_8051_EVB_
        #include "ITE6615_EVB_Debug.h"
    #endif
#include "ITE6615_IO.h"
#include "ITE6615_mcu.h"

#endif

//#include "iTE6615_Utility.h"
#include "ITE6615_SYS_FLOW.h"

#if (_ENABLE_CEC_==TRUE)
    #include "ITE6615_CEC_SYS.h"
#endif

//#include "mdindly.h"
#define delay1ms(x) Delay_mSec(x)
#define iTE6615_EVB_Ca100ms_RCLK(x) // used for _MCU_8051_EVB_

#endif  /*__ITE6615_GOLBAL_H__*/


#endif	//__USE_IT6615_TX__

