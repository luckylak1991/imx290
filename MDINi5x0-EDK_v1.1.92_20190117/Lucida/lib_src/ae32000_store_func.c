//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2017  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	AE32000_STORE_FUNC.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"common.h"


#if defined(__USE_COMPILER_PATCH_VER__)

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

	//.file	"ae32000_store_func.c"
	asm(".section .ispm");


//# stb %r8,(%r9)
	asm(".align 1");
	asm(".global _ae32000_stb_r8_to_r9");
	asm(".type	_ae32000_stb_r8_to_r9, @function");
asm("_ae32000_stb_r8_to_r9:");
	asm("push %R0, %R1, %R2");
	asm("mov %r9,%r2	# dst_addr");
	asm("and 3,%r9	# offset");
	asm("mov %r8,%r0	# src_data");
	asm("cvb %r0");
	asm("ldi 255,%r1	# mask");
	asm("jmp .L8");
asm(".L10:");
	asm("asl 8, %r1	# mask <<= 8");
	asm("asl 8, %r0	# src_data <<= 8");
	asm("addq -1,%r9");
asm(".L8:");
	asm("cmpq 0,%r9");
	asm("jnz .L10");
	asm("mov %r2,%r8	# dst_addr");
	asm("and -4,%r8	# make it word-aligned");
	asm("ld (%r8),%r9	# read aligned word");
	asm("not %r1	# ~mask");
	asm("and %r1,%r9");
	asm("or %r0,%r9	# modifiy target word with src_data");
	asm("st %r9,(%r8)	# store modified word");
	asm("pop  %R0, %R1, %R2");
	asm("jplr");
	asm(".size	_ae32000_stb_r8_to_r9, .-_ae32000_stb_r8_to_r9");


//# sts %r8,(%r9)
	asm(".align 1");
	asm(".global _ae32000_sts_r8_to_r9");
	asm(".type	_ae32000_sts_r8_to_r9, @function");
asm("_ae32000_sts_r8_to_r9:");
	asm("push %R0, %R1");
	asm("mov  %r8,%r0");
	asm("cvs %r0");
	asm("mov %r9,%r8	# dst_addr");
	asm("and 2,%r8");
	asm("ldi -65536,%r1");
	asm("jz .L13");
	asm("ldi 65535,%r1");
	asm("asl 16, %r0	# src_data <<= 16");
asm(".L13:");
	asm("and -4,%r9	# make it word-aligned");
	asm("ld (%r9),%r8	# read aligned word");
	asm("and %r1,%r8");
	asm("or %r0,%r8	# modifiy target word with src_data");
	asm("st %r8,(%r9)	# store modified word");
	asm("pop  %R0, %R1");
	asm("jplr");
	asm(".size	_ae32000_sts_r8_to_r9, .-_ae32000_sts_r8_to_r9");


//# stb %r9,(%r8)
	asm(".align 1");
	asm(".global _ae32000_stb_r9_to_r8");
	asm(".type	_ae32000_stb_r9_to_r8, @function");
asm("_ae32000_stb_r9_to_r8:");
	asm("push %R0, %R1, %R2");
	asm("mov %r8,%r2	# dst_addr");
	asm("and 3,%r8	# offset");
	asm("mov %r9,%r0	# src_data");
	asm("cvb %r0");
	asm("ldi 255,%r1	# mask");
	asm("jmp .L8R");
asm(".L10R:");
	asm("asl 8, %r1	# mask <<= 8");
	asm("asl 8, %r0	# src_data <<= 8");
	asm("addq -1,%r8");
asm(".L8R:");
	asm("cmpq 0,%r8");
	asm("jnz .L10R");
	asm("mov %r2,%r9	# dst_addr");
	asm("and -4,%r9	# make it word-aligned");
	asm("ld (%r9),%r8	# read aligned word");
	asm("not %r1	# ~mask");
	asm("and %r1,%r8");
	asm("or %r0,%r8	# modifiy target word with src_data");
	asm("st %r8,(%r9)	# store modified word");
	asm("pop  %R0, %R1, %R2");
	asm("jplr");
	asm(".size	_ae32000_stb_r9_to_r8, .-_ae32000_stb_r9_to_r8");


//# sts %r9,(%r8)
	asm(".align 1");
	asm(".global _ae32000_sts_r9_to_r8");
	asm(".type	_ae32000_sts_r9_to_r8, @function");
asm("_ae32000_sts_r9_to_r8:");
	asm("push %R0, %R1");
	asm("mov  %r9,%r0");
	asm("cvs %r0");
	asm("mov %r8,%r9	# dst_addr");
	asm("and 2,%r9");
	asm("ldi -65536,%r1");
	asm("jz .L13R");
	asm("ldi 65535,%r1");
	asm("asl 16, %r0	# src_data <<= 16");
asm(".L13R:");
	asm("and -4,%r8	# make it word-aligned");
	asm("ld (%r8),%r9	# read aligned word");
	asm("and %r1,%r9");
	asm("or %r0,%r9	# modifiy target word with src_data");
	asm("st %r9,(%r8)	# store modified word");
	asm("pop  %R0, %R1");
	asm("jplr");
	asm(".size	_ae32000_sts_r9_to_r8, .-_ae32000_sts_r9_to_r8");


	//.ident	"GCC: (GNU) 3.4.5"

#endif	// defined(__USE_COMPILER_PATCH_VER__)


/*  FILE_END_HERE */
