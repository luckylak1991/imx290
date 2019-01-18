#ifndef _SHA_1_H_
#define _SHA_1_H_

#ifdef _MCU_8051_
    #include "Mcu.h"
#endif

#include	"common.h"

#if defined(__USE_IT6615_TX__)

#include <string.h>
#include "iTE6615_debug.h"
#include "iTE6615_Config.h"
#include "iTE6615_Typedef.h"
#if IT6615_Debug_message
    #include <stdio.h>
#endif


#ifndef HDCP_DEBUG_PRINTF
    #define HDCP_DEBUG_PRINTF(x)
#endif //HDCP_DEBUG_PRINTF

#ifndef HDCP_DEBUG_PRINTF1
    #define HDCP_DEBUG_PRINTF1(x)
#endif //HDCP_DEBUG_PRINTF1

#ifndef HDCP_DEBUG_PRINTF2
    #define HDCP_DEBUG_PRINTF2(x)
#endif //HDCP_DEBUG_PRINTF2


#ifndef DISABLE_HDCP
void SHA_Simple(void *p,u32 len,u8 *output);
void SHATransform(u32 * h);
#endif

#endif // _SHA_1_H_

#endif		//__USE_IT6615_TX__
