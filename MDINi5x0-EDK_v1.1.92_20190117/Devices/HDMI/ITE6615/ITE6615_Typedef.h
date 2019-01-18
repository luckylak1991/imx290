#if defined(__USE_IT6615_TX__)

#ifndef _ITE6615_TYPEDEF_H_
#define _ITE6615_TYPEDEF_H_

// WIN32 auto define by complier in PC
//#ifndef WIN32
//#define _MCU_8051_
//#endif


//////////////////////////////////////////////////
// MCU 8051data type
//////////////////////////////////////////////////


#define FALSE   0
#define TRUE    1


#ifndef NULL
    #define NULL ((void *) 0)
#endif


typedef char iTE6615_s8, *piTE6615_s8;
//typedef unsigned char u8, *pu8;
typedef unsigned char *pu8;
//typedef short s16, *ps16;
typedef short *ps16;
//typedef unsigned short u16, *pu16;
typedef unsigned short *pu16;

#ifdef _MCU_8051_

    typedef unsigned long u32, *pu32;
    typedef long s32, *ps32;

    typedef unsigned char u1;
    #define _CODE const
    #define _BIT       bit
    #define _BDATA     bdata
    #define _DATA      data
    #define _IDATA     idata
    #define _XDATA     xdata
    #define _FAR_       far
    #define _REENTRANT reentrant
#else

//    typedef unsigned int  u32, *pu32;
//    typedef int s32, *ps32;
    typedef unsigned int  *pu32;
    typedef int *ps32;

    // #ifndef bool
    // typedef unsigned char bool ;
    // #endif
    typedef bool u1;
    #define _CODE
    #define _BIT       BOOL
    #define _BDATA
    #define _DATA
    #define _IDATA
    #define _XDATA
    #define _FAR
    #define _REENTRANT
#endif




#endif // _ITE6615_TYPEDEF_H_

#endif		//__USE_IT6615_TX__
