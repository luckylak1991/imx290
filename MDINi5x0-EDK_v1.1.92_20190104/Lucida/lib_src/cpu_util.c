#include "typedef.h"
#include "macroimg/util.h"
asm("\
.section .text \n\
	.global _get_cur_sp \n\
	.type _get_cur_sp, @function \n\
	\n\
_get_cur_sp: \n\
mov %sp, %r8 \n\
	jplr \n\
	.size _get_cur_sp, .-_get_cur_sp \n\
	\n\
	.section .text \n\
	.global _get_cur_heapend \n\
	.type _get_cur_heapend, @function \n\
_get_cur_heapend: \n\
push %lr	\n\
	ldi 0,%r8	\n\
	jal __sbrk	\n\
	pop %pc	\n\
	.size _get_cur_heapend, .-_get_cur_heapend ");

BOOL heap_stack_collision()
{
	if(get_cur_sp()<get_cur_heapend())
		return TRUE;
	return FALSE;
}
