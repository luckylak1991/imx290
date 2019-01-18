/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#include "macroimg.h"

/*
	defined at linker-script.
*/
extern unsigned int _rom_data_start[];
extern unsigned int _ram_data_start[];
extern unsigned int _ram_data_end[];

extern unsigned int _rom_ispm_start[];
extern unsigned int _ram_ispm_start[];
extern unsigned int _ram_ispm_end[];

extern unsigned int _rom_dspm_start[];
extern unsigned int _ram_dspm_start[];
extern unsigned int _ram_dspm_end[];

extern unsigned int _bss_start[];
extern unsigned int _bss_end[];
/*
void data_section_copy();
asm(".section .text");
asm("    .global _data_section_copy");
asm("	.type _data_section_copy, @function");
asm("_data_section_copy:");
asm("	push %r0-%r2");
asm("	ldi	__rom_data_start,%r0");
asm(" 	ldi	__ram_data_start,%r1");
asm(" 	ldi	__ram_data_end,%r2");
asm("_1:	cmp  %r2, %r1");
asm("   jnc  __data_copy_end");
asm("   ld    (%r0, 0), %r3");
asm("   st    %r3, (%r1, 0)");
asm("	addq 4, %r0");
asm("   addq 4, %r1");
asm("   jmp  _1");
asm("__data_copy_end:");
asm("	pop %r0-%r2");
asm("jplr");
asm(".size _data_section_copy, .-_data_section_copy");

void bss_init();
asm(".section .text");
asm("    .global _bss_init");
asm("	.type _bss_init, @function");
asm("_bss_init:");
asm("	push %r0-%r2");
asm("	ldi	__bss_start,%r0");
asm(" 	ldi	__bss_end,%r1");
asm(" 	ldi	0,%r2");
asm("_2:	cmp  %r1, %r0");
asm("   jnc  __bss_init_end");
asm("   st    %r2, (%r0, 0)");
asm("	addq 4, %r0");
asm("   jmp  _2");
asm("__bss_init_end:");
asm("	pop %r0-%r2");
asm("jplr");
asm(".size _bss_init, .-_bss_init");
*/
	

/*!
 * \brief
 * Write brief comment for crt0main here.
 *  
 * Data, ISPM, DSPM copy
 * BSS initialize
 * 
 */
 
//void crt0main() __attribute__((weak)) __attribute__((section(".ispm")));

void crt0main()
{
	register unsigned int* src;
	register unsigned int* dest;
	register unsigned int* destend;
	register int len;
	
	/* data copy */
	src = _rom_data_start;
	dest = _ram_data_start;
	destend = _ram_data_end;
	len = (unsigned int)destend - (unsigned int)dest;
	memcpy((void*)dest,(void*)src,(int)len);

	/* ISPM copy */
	src = _rom_ispm_start;
	dest = _ram_ispm_start;
	destend = _ram_ispm_end;
	len = (unsigned int)destend - (unsigned int)dest;
	if(len>0)
		memcpy(dest,src,len);
	

	/* BSS init
	*/
	dest = _bss_start;
	destend = _bss_end;
	len = (unsigned int)destend - (unsigned int)dest;
	if(len>0)
		memset(dest,0,len);

	
	//for C++
	/*
	typedef void (*pfunc) ();
	extern pfunc __ctors[];
    extern pfunc __ctors_end[];
    pfunc *p;
	for (p = __ctors_end; p > __ctors; )
		(*--p) ();
	*/
}
