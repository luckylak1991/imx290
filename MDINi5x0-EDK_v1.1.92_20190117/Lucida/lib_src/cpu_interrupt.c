/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/

#include <sys/syscall.h>
/*
#define	SYS_exit	1
#define	SYS_fork	2

#define	SYS_read	3
#define	SYS_write	4
#define	SYS_open	5
#define	SYS_close	6
#define	SYS_wait4	7
#define	SYS_creat	8
#define	SYS_link	9
#define	SYS_unlink	10
#define	SYS_execv	11
#define	SYS_chdir	12
#define	SYS_mknod	14
#define	SYS_chmod	15
#define	SYS_chown	16
#define	SYS_lseek	19
#define	SYS_getpid	20
#define SYS_isatty      21
#define SYS_fstat       22
#define SYS_time 	23


#define SYS_ARG		24
#define	SYS_stat	38


#define	SYS_pipe	42
#define	SYS_execve	59
#define   SYS_times       43
#define SYS_gettimeofday 116

*/
#include "macroimg.h"
	
typedef enum 
{
	EXCEPTION_NMI=0,
	EXCEPTION_DOUBLE_FAULT,
	EXCEPTION_BUS_ERROR,
	EXCEPTION_CP0,
	EXCEPTION_UNDEFINED,
	EXCEPTION_UNIMPLEMTED,
	EXCEPTION_NOTUSED_ISR,
	EXCEPTION_MAX,	
}EXCEPTION_TYPE;

static char* error_to_msg[EXCEPTION_MAX]={"NMI","Double Fault","Bus Error","Coprocessor 0","Undefined","unimplemented instruction","Not registered Interrupt"};


/*!
 * 
 * \param[in] addr
 * 
 */
static void DumpAllReg(U32* addr)
{

	char* sprname[8]={"CR0","CR1","ML","MH","ER","LR","PC","SR"};
	int i;
	debugstring("\r\n ======== DUMP ALL REGISTER ========\r\n");

	for (i=0;i<16;)
	{
		debugprintf("R%d = 0x%08x \t",i,addr[i]);
		i++;
		if (i % 4 == 0)
		{
			debugstring("\r\n");
		}
	}
	for (;i<24;)
	{
		debugprintf("%s = 0x%08x \t",sprname[i-16],addr[i]);
		i++;
		if (i % 4 == 0)
		{
			debugstring("\r\n");
		}
	}
	debugstring(" ===================================\r\n");
}

void unrecoverable_error(EXCEPTION_TYPE type,U32* registers)
{
	while(1)
	{
		if(type<EXCEPTION_MAX)
		{
			debugstring("======== unrecoverable error ==========\r\n");
			debugstring(error_to_msg[type]);
			debugstring("\r\n");
			DumpAllReg(registers);
			debugstring("=======================================\r\n");
		}
		if(type == EXCEPTION_CP0)
		{
			int register cp0status;
			asm("push %r0");
			asm("mvfc 0,%r15"); // coprocessor 15
			asm("sync");
			asm("lea (%%r0,0), %0" : "=r"(cp0status) :);
			asm("pop %r0");

			cp0status = (cp0status>>2) & 0xf;
			switch (cp0status)
			{
			case 0:
				debugstring("Access violation exception(Instruction)\r\n");
				break;
			case 1:
				debugstring("TLB miss exception(Instruction)\r\n");
				break;
			case 2:
				debugstring("Privilege violation exception\r\n");
				break;
			case 3:
				debugstring("Address alignment error exception(Data)\r\n");
				break;
			case 4:
				debugstring("Access violation exception(Data)\r\n");
				break;
			case 5:
				debugstring("TLB miss exception(Data)\r\n");
				break;
			case 6:
				debugstring("Write to Read-Only region(Data)\r\n");
				break;
			case 7:
				debugstring("Address alignment error exceptioin(Instruction)\r\n");
				break;
			}
		}
		else if(type == EXCEPTION_NOTUSED_ISR)
		{
			debugprintf(" unregistered ISR : regst(0):0x%x\r\n system halted\r\n",*R_INTPEND);
			debugprintf(" unregistered ISR : regst(1):0x%x\r\n system halted\r\n",*R_INTPEND);
		}
		
		while(1);
		
		int i=0;
		for(i=0;i<0xffff;i++)
		{
			asm("nop");
			asm("nop");
			asm("nop");
		}
	}
}

#pragma interrupt_handler
static void NOTUSEDISR(void)
{
	U32* registers = __builtin_frame_address(0);
	unrecoverable_error(EXCEPTION_NOTUSED_ISR,registers);
}


#pragma interrupt_handler
static void nmi_autovector(void)
{
	U32* registers = __builtin_frame_address(0);
	unrecoverable_error(EXCEPTION_NMI,registers);
}

#pragma interrupt_handler
static void double_fault_exception(void)
{
	U32* registers = __builtin_frame_address(0);
	unrecoverable_error(EXCEPTION_DOUBLE_FAULT,registers);
}
#pragma interrupt_handler
static void bus_error_exception(void)
{
	U32* registers = __builtin_frame_address(0);
	unrecoverable_error(EXCEPTION_DOUBLE_FAULT,registers);
}

#pragma interrupt_handler
static void coprocessor0_exception(void)
{

	U32* registers = __builtin_frame_address(0);
	unrecoverable_error(EXCEPTION_CP0,registers);
}

#pragma interrupt_handler
static void undefined_exception(void)
{
	U32* registers = __builtin_frame_address(0);
	unrecoverable_error(EXCEPTION_UNDEFINED,registers);
}

#pragma interrupt_handler
static void unimplemented_exception(void)
{
	U32* registers = __builtin_frame_address(0);
	unrecoverable_error(EXCEPTION_UNIMPLEMTED,registers);
}


/**
 * 사용자가 system call에 해당 하는 함수룰 호출 했을 경우 실행된다.
 * 시스템 콜을 호출 하는 함수는 write, read 와 같은 low-level 함수를 호출 했을 경우인데
 * 예를 들어서 fopen, fwrite, fread, printf 와 같이 OS 의존적인 함수를 사용자가 호출 했을 경우이다.
 * printf 만 구현 되어 있어서 사용자가 사용 할 수 있다.
 */
U32 systemcallback(U32 func,U32 p1,U32 p2,U32 p3)
{
	//	int i;
	U32 ret=0; // 0 is OK

	switch (func)
	{
	case SYS_write:
		if (p1 == 1 || p1 == 2)//stdout, stderr.. p2 is string, p3 is length
		{
			char str[1024] ;
			strncpy(str,(char*)p2,p3);
			str[p3]=0;
			debugstring(str);
			ret = p3;

		}
		else
		{
			debugstring("SYS_write function\r\n");
			debugprintf("not support parameter : 0x%x\r\n",p1);
		}
		break;

	default :
		debugprintf("not supported systemcall parameter : func=0x%x, p1=0x%x, p2=0x%x, p3=0x%x,\r\n",func,p1,p2,p3);
		break;
	}
	return ret;
}
/* 1st arg : %r8
* 2st arg : %r9
* 3st arg : %sp + 0xc
* 4st arg : %sp + 0x10
All regsiter 16+6(cr0,cr1,ml,mh,%er,%lr) + 2(SR,PC)=24
*/

#pragma interrupt
static void SystemCall()
{

	U32 ret;
	volatile U32 *registers;
	int func,p1,p2,p3;
	registers = __builtin_frame_address(0);

	//	_vgDumpAllReg(registers);
	func = registers[8];
	p1 = registers[9];
	p2 = registers[24+3];
	p3 = registers[24+4];

	ret = systemcallback(func,p1,p2,p3);

	registers[8] = ret;

}
static void unregister_isr()
{
	U32* registers = __builtin_frame_address(0);
	unrecoverable_error(EXCEPTION_NOTUSED_ISR,registers);
}

static void (*UserVector_table[32])(void) =
{
	unregister_isr,
	unregister_isr,
	unregister_isr,
	unregister_isr,
	
	unregister_isr,
	unregister_isr,
	unregister_isr,
	unregister_isr,
	
	unregister_isr,
	unregister_isr,
	unregister_isr,
	unregister_isr,
	
	unregister_isr,
	unregister_isr,
	unregister_isr,
	unregister_isr,

	unregister_isr,
	unregister_isr,
	unregister_isr,
	unregister_isr,
	
	unregister_isr,
	unregister_isr,
	unregister_isr,
	unregister_isr,
	
	unregister_isr,
	unregister_isr,
	unregister_isr,
	unregister_isr,
	
	unregister_isr,
	unregister_isr,
	unregister_isr,
	unregister_isr
};
/*
PRE_DEF_ISR_HW do not supports nested interrupt.
*/

#define PRE_DEF_ISR_SW(A)	void predef_isr##A(void) __attribute__((weak)) ;\
	asm(".section .ispm,\"ax\",@progbits");\
	asm(".align 1");\
	asm(".type _predef_isr"#A", @function");\
	asm("_predef_isr"#A":");\
	asm("push	%cr0,%cr1,%ml,%mh,%er,%lr");\
	asm("push	%r7");\
	asm("push	%r8,%r9");\
	asm("ldi	_UserVector_table +(4 * "#A") ,%r8");\
	asm("ld		(%r8,0),%r9");\
	asm("jalr	%r9");\
	asm("ldi	0x20 + "#A",%r8");\
	asm("ldi	0xFFFF0000,%r9");\
	asm("st		%r8,(%r9,0)");\
	asm("pop	%r8,%r9");\
	asm("pop	%r7");\
	asm("pop	%cr0,%cr1,%ml,%mh,%er,%lr,%pc,%sr");\
	asm(".size _predef_isr"#A", .-_predef_isr"#A);

PRE_DEF_ISR_SW(0)
PRE_DEF_ISR_SW(1)
PRE_DEF_ISR_SW(2)
PRE_DEF_ISR_SW(3)
PRE_DEF_ISR_SW(4)
PRE_DEF_ISR_SW(5)
PRE_DEF_ISR_SW(6)
PRE_DEF_ISR_SW(7)
PRE_DEF_ISR_SW(8)
PRE_DEF_ISR_SW(9)
PRE_DEF_ISR_SW(10)
PRE_DEF_ISR_SW(11)
PRE_DEF_ISR_SW(12)
PRE_DEF_ISR_SW(13)
PRE_DEF_ISR_SW(14)
PRE_DEF_ISR_SW(15)
PRE_DEF_ISR_SW(16)
PRE_DEF_ISR_SW(17)
PRE_DEF_ISR_SW(18)
PRE_DEF_ISR_SW(19)
PRE_DEF_ISR_SW(20)
PRE_DEF_ISR_SW(21)
PRE_DEF_ISR_SW(22)
PRE_DEF_ISR_SW(23)
PRE_DEF_ISR_SW(24)
PRE_DEF_ISR_SW(25)
PRE_DEF_ISR_SW(26)
PRE_DEF_ISR_SW(27)
PRE_DEF_ISR_SW(28)
PRE_DEF_ISR_SW(29)
PRE_DEF_ISR_SW(30)
PRE_DEF_ISR_SW(31)

extern void dma_isr();//dma.c
extern void Reset_Start();
typedef void (*fp)(void) ;

/*
	for FreeRTOS 
*/
void  __attribute__((weak)) swi1isr();
#pragma interrupt
void  swi1isr()
{
	debugprintf("SWI 1 ISR\r\n");
	asm("halt 1");
}

fp vector_table[] __attribute__((section (".vects")))=
{
	Reset_Start      	        , /* V00 : Reset Vector          */
	nmi_autovector              , /* V01 : NMI Vector            */
	NOTUSEDISR        			, /* V02 : Interrupt Auto Vector */
	double_fault_exception      , /* V03 : Double fault Vector   */
	bus_error_exception         , /* V04 : Bus Error Exception   */
	NOTUSEDISR                  , /* V05 : Reserved              */
	NOTUSEDISR                  , /* V06: Reserved               */
	NOTUSEDISR                  , /* V07 : Reserved              */
	coprocessor0_exception      , /* V08 : Coprocessor0 Exception*/
	NOTUSEDISR      			, /* V09 : Coprocessor1 Exception*/
	NOTUSEDISR     				, /* V0A : Coprocessor2 Exception*/
	NOTUSEDISR      			, /* V0B : Coprocessor3 Exception*/
	NOTUSEDISR            		, /* V0C : OSI reset vector      */
	NOTUSEDISR         			, /* V0D : OSI break exception   */
	undefined_exception         , /* V0E : Reserved              */
	unimplemented_exception     , /* V0F : Reserved              */
	NOTUSEDISR          		, /* V10 : SWI 00 Vector         */
	swi1isr          			, /* V11 : SWI 01 Vector         */
	NOTUSEDISR          		, /* V12 : SWI 02 Vector         */
	NOTUSEDISR          		, /* V13 : SWI 03 Vector         */
	NOTUSEDISR          		, /* V14 : SWI 04 Vector         */
	NOTUSEDISR          		, /* V15 : SWI 05 Vector         */
	SystemCall          		, /* V16 : SWI 06 Vector         */
	NOTUSEDISR          		, /* V17 : SWI 07 Vector         */
	NOTUSEDISR         			, /* V18 : SWI 08 Vector         */
	NOTUSEDISR         			, /* V19 : SWI 09 Vector         */
	NOTUSEDISR          		, /* V1A : SWI 0A Vector         */
	NOTUSEDISR          		, /* V1B : SWI 0B Vector         */
	NOTUSEDISR          		, /* V1C : SWI 0C Vector         */
	NOTUSEDISR          		, /* V1D : SWI 0D Vector         */
	NOTUSEDISR          		, /* V1E : SWI 0E Vector         */
	NOTUSEDISR         			, /* V1F : SWI 0F Vector         */
	predef_isr0          		, /* V20 : User Vector 0         */
	predef_isr1          		, /* V21 : User Vector 1         */
	predef_isr2          		, /* V22 : User Vector 2         */
	predef_isr3          		, /* V23 : User Vector 3         */
	predef_isr4          		, /* V24 : User Vector 4         */
	predef_isr5          		, /* V25 : User Vector 5         */
	predef_isr6          		, /* V26 : User Vector 6         */
	predef_isr7         		, /* V27 : User Vector 7         */
	predef_isr8          		, /* V28 : User Vector 8         */
	predef_isr9          		, /* V29 : User Vector 9         */
	predef_isr10          		, /* V2A : User Vector 10        */
	predef_isr11          		, /* V2B : User Vector 11        */
	predef_isr12          		, /* V2C : User Vector 12        */
	predef_isr13         		, /* V2D : User Vector 13        */
	predef_isr14          		, /* V2E : User Vector 14        */
	predef_isr15         		, /* V2F : User Vector 15        */
	predef_isr16        		, /* V30 : User Vector 16        */
	predef_isr17        		, /* V31 : User Vector 17        */
	predef_isr18        		, /* V32 : User Vector 18        */
	predef_isr19        		, /* V33 : User Vector 19        */
	predef_isr20        		, /* V34 : User Vector 20        */
	predef_isr21        		, /* V35 : User Vector 21        */
	predef_isr22        		, /* V36 : User Vector 22        */
	predef_isr23        		, /* V37 : User Vector 23        */
	predef_isr24        		, /* V38 : User Vector 24        */
	predef_isr25        		, /* V39 : User Vector 25        */
	predef_isr26        		, /* V3A : User Vector 26        */
	predef_isr27        		, /* V3B : User Vector 27        */
	predef_isr28        		, /* V3C : User Vector 28        */
	predef_isr29        		, /* V3D : User Vector 29        */
	predef_isr30        		, /* V3E : User Vector 30        */
	predef_isr31        		, /* V3F : User Vector 31        */
};

void init_interrupt()
{
	SETVECTORED;
	INTERRUPT_ENABLE;

	*R_INTEN = 0; //all disable
	*R_MASKCLR = 0xffffffff;// all interrupt disable

	//copy vector table to DSPM
	memcpy((void*)0x20000000,vector_table,sizeof(vector_table));
	
	//set vector base register
	asm("push %r0");
	asm("ldi	0x20000000,%r0");
//	asm("ldi	_vector_table, %r0");	//program load address
	asm("mvtc	0,%r12");
	asm("sync");
	asm("pop %r0");	
	
	
	
}

BOOL set_interrupt(INTERRUPT_TYPE intnum, void (*fp)())
{
	if (intnum >= INTNUM_MAX)
		return FALSE;

	UserVector_table[intnum]=fp;
	return TRUE;

}

void* get_interrupt(INTERRUPT_TYPE intnum)
{
	return UserVector_table[intnum];
}

void enable_interrupt(INTERRUPT_TYPE num, BOOL b)
{
	CRITICAL_ENTER();
	if (!b) //disable
	{
		*R_INTEN &= ~F_INTEN_EN(num);
		*R_MASKCLR = F_MASKSET(num);
	}
	else
	{
		*R_INTEN |= F_INTEN_EN(num);
		*R_MASKSET = F_MASKSET(num);
	}
	CRITICAL_EXIT();
}
