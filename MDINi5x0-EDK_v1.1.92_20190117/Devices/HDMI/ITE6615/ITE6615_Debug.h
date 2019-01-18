#include <stdio.h>
#include <stdarg.h>

#if defined(__USE_IT6615_TX__)

#ifndef _IT6615_DEBUG_H_
#define _IT6615_DEBUG_H_


#ifndef IT6615_Debug_message
#define IT6615_Debug_message 0
#endif

#if IT6615_Debug_message
    #define HDMITX_INT_PRINTF(x)  printf x
    #define HDMITX_AUDIO_PRINTF(x)  printf x //kuro
    #define HDMITX_DEBUG_PRINTF(x) printf x //kuro
    #define HDMITX_SCDC_PRINTF(x) printf x //kuro
    #define CEC_DEBUG_PRINTF(x) //printf x
    #define HDCP_DEBUG_PRINTF(x) printf x
    #define REG_PRINTF(x) //printf x
    #define PRINTF_EDID(x) //printf x
    #define PRINTF_BUFFER(x) //printf x
    #define HWRD_MSG(x)

    //#define HDMITX_INT_PRINTF(x)  //printf x
    //#define HDMITX_AUDIO_PRINTF(x)  //printf x //kuro
    //#define HDMITX_DEBUG_PRINTF(x) //printf x //kuro
    //#define HDMITX_SCDC_PRINTF(x) //printf x //kuro
    //#define CEC_DEBUG_PRINTF(x) //printf x
    //#define HDCP_DEBUG_PRINTF(x) //printf x
    //#define REG_PRINTF(x) printf x
    //#define PRINTF_EDID(x) //printf x
    //#define PRINTF_BUFFER(x) //printf x
#else
    #define HDMITX_INT_PRINTF(x)
    #define HDMITX_AUDIO_PRINTF(x)
    #define HDMITX_DEBUG_PRINTF(x)
    #define HDMITX_SCDC_PRINTF(x)
    #define CEC_DEBUG_PRINTF(x)
    #define HDCP_DEBUG_PRINTF(x)
    #define REG_PRINTF(x)
    #define PRINTF_EDID(x)
    #define PRINTF_BUFFER(x)
    #define HWRD_MSG(x)
#endif




#endif /*_IT6615_DEBUG_H_*/


#endif		//__USE_IT6615_TX__
