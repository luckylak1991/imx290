/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/


#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_


#ifdef TRUE	
#undef TRUE 
#endif
#define TRUE 1

#ifdef FALSE 
#undef FALSE
#endif
#define FALSE 0

#ifdef true
#undef true 
#endif
#define true 1

#ifdef false 
#undef false
#endif
#define false 0

#if defined(BOOL)
#else 
typedef int BOOL;
#endif
#if defined(bool)
#else
#ifndef __GNUG__ 
typedef int bool;
#endif//__GNUG___
#endif

typedef signed char		S8;
typedef signed short	S16;
typedef signed int		S32;

typedef signed char		s8;
typedef signed short	s16;
typedef signed int		s32;

typedef unsigned char	U8;
typedef unsigned short	U16;
typedef unsigned int	U32;
typedef unsigned long long	U64;

typedef volatile unsigned char	vU8;
typedef volatile unsigned short	vU16;
typedef volatile unsigned int	vU32;
typedef volatile unsigned long long	vU64;

typedef unsigned char	u8;
typedef unsigned short	u16;
typedef unsigned int	u32;
typedef unsigned long long	u64;

typedef unsigned char	__u8;
typedef unsigned short	__u16;
typedef unsigned int	__u32;
typedef unsigned long long	__u64;


typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned long	ulong;

typedef struct _tagRect
{
	int x;
	int y;
	int w;
	int h;
}RECT;


#define ALIGN4  __attribute__((__aligned__(4)))


#endif

