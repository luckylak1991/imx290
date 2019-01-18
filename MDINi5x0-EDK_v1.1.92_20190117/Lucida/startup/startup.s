## -----------------------------------------
## -----------------------------------------
    .section .text
    .global _Reset_Start
	.type _Reset_Start, @function
_Reset_Start:

################################################################################
#	Serial Flash Memory 0 CFG : Winbond/EON Flash 
################################################################################

# Flash Mode				# flash read mode (single read mode)
    ldi    0x80000000,  %R6
    ldi    0x00008100,	%R7
    st    %R7, (%R6,0)

# Flash Command1			# flash write enable command(06h)
    ldi   0x80000010, %R6
    ldi   0x00000006, %R7
    st    %R7, (%R6,0)

# Flash Commnad2			# flash status register-1 (block protect bits enable)
    ldi   0x80000014, %R6
    ldi   0x0000001C, %R7
    st    %R7, (%R6,0)

# Flash Command3			# flash write enable command(06h)
    ldi   0x80000010, %R6
    ldi   0x00000006, %R7
    st    %R7, (%R6,0)

# Flash Commnad4			# flash status register-1&2 (quad enable,  block protect bits enable)
    ldi   0x8000002C, %R6
    ldi   0x0000021C, %R7
    st    %R7, (%R6,0)

# Flash Command5			# flash write disable command(04h)
    ldi   0x80000010, %R6
    ldi   0x00000004, %R7
    st    %R7, (%R6,0)

# Flash Mode : Winbond 4byte address mode
    ldi    0x80000000,  %R6
    ldi    0x00008182,	%R7			# flash write disable in flash controller
    st    %R7, (%R6,0)

# Feedback Clock Delay Value 
    ldi	   0x80000028, %R6
    ldi	   0x0000000f,	%R7
    st	  %R7, (%R6,0)

# Flash Baudrate 
    ldi    0x80000004,  %R6
    ldi    0x00000000,	%R7
    st    %R7, (%R6,0)

# Flash CS High Time  
    ldi    0x80000008,  %R6
    ldi    0x0000000a,	%R7
    st    %R7, (%R6,0)

################################################################################
#   SPM Global register setting
################################################################################
	ldi 0x00000700,	%R0
	mvtc 0, %R3
	nop
	ldi 0x00421421, %R0 #iSPM:16KB x 4bank, dSPM:16KB x 4bank
	mvtc 0, %R4
	nop
	
################################################################################
#	ISPM CFG : Bank0~Bank3
################################################################################
# ISPM address register setting: 0x1000_0000 ~ 0x1000_FFFF (64KB)

# Bank0 ################	
# Bank0 Start Address :	
	ldi 0x00000702, %R0
	mvtc 0, %R3
	nop
	ldi 0x10000000, %R0
	mvtc 0, %R4
	nop

# Bank0 End Address :	
	ldi 0x00000703, %R0
	mvtc 0, %R3
	nop
	ldi 0x10003FFF, %R0
	mvtc 0, %R4
	nop

# Bank0 Enable
	ldi 0x00000701, %R0
	mvtc 0, %R3
	nop
	ldi 0x00000111, %R0
	mvtc 0, %R4
	nop

# Bank1 ################	
# Bank1 Start Address :	
	ldi 0x00000712, %R0
	mvtc 0, %R3
	nop
	ldi 0x10004000, %R0
	mvtc 0, %R4
	nop

# Bank1 End Address :	
	ldi 0x00000713, %R0
	mvtc 0, %R3
	nop
	ldi 0x10007FFF, %R0
	mvtc 0, %R4
	nop

# Bank1 Enable
	ldi 0x00000711, %R0
	mvtc 0, %R3
	nop
	ldi 0x00000111, %R0
	mvtc 0, %R4
	nop
	
# Bank2 ################	
# Bank2 Start Address :	
	ldi 0x00000722, %R0
	mvtc 0, %R3
	nop
	ldi 0x10008000, %R0
	mvtc 0, %R4
	nop

# Bank2 End Address :	
	ldi 0x00000723, %R0
	mvtc 0, %R3
	nop
	ldi 0x1000BFFF, %R0
	mvtc 0, %R4
	nop

# Bank2 Enable
	ldi 0x00000721, %R0
	mvtc 0, %R3
	nop
	ldi 0x00000111, %R0
	mvtc 0, %R4
	nop
	
# Bank3 ################	
# Bank3 Start Address :	
	ldi 0x00000732, %R0
	mvtc 0, %R3
	nop
	ldi 0x1000C000, %R0
	mvtc 0, %R4
	nop

# Bank3 End Address :	
	ldi 0x00000733, %R0
	mvtc 0, %R3
	nop
	ldi 0x1000FFFF, %R0
	mvtc 0, %R4
	nop

# Bank3 Enable
	ldi 0x00000731, %R0
	mvtc 0, %R3
	nop
	ldi 0x00000111, %R0
	mvtc 0, %R4
	nop
	
################################################################################
#	DSPM CFG : Bank0~Bank3
################################################################################
# DSPM address register setting: 0x20000000 ~ 0x2000FFFF (64KB)
	
# Bank0 ################
# Bank0 Start Address :	
	ldi 0x00000705, %R0
	mvtc 0, %R3
	nop
	ldi 0x20000000, %R0
	mvtc 0, %R4
	nop

# Bank0 End Address	
	ldi 0x00000706, %R0
	mvtc 0, %R3
	nop
	ldi 0x20003FFF, %R0
	mvtc 0, %R4
	nop

# Bank0 Enable
	ldi 0x00000704, %R0
	mvtc 0, %R3
	nop
	ldi 0x00000111, %R0
	mvtc 0, %R4
	nop
	
# Bank1 ################
# Bank1 Start Address :	
	ldi 0x00000715, %R0
	mvtc 0, %R3
	nop
	ldi 0x20004000, %R0
	mvtc 0, %R4
	nop

# Bank1 End Address	
	ldi 0x00000716, %R0
	mvtc 0, %R3
	nop
	ldi 0x20007FFF, %R0
	mvtc 0, %R4
	nop

# Bank1 Enable
	ldi 0x00000714, %R0
	mvtc 0, %R3
	nop
	ldi 0x00000111, %R0
	mvtc 0, %R4
	nop

# Bank2 ################
# Bank2 Start Address :	
	ldi 0x00000725, %R0
	mvtc 0, %R3
	nop
	ldi 0x20008000, %R0
	mvtc 0, %R4
	nop

# Bank2 End Address	
	ldi 0x00000726, %R0
	mvtc 0, %R3
	nop
	ldi 0x2000BFFF, %R0
	mvtc 0, %R4
	nop

# Bank2 Enable
	ldi 0x00000724, %R0
	mvtc 0, %R3
	nop
	ldi 0x00000111, %R0
	mvtc 0, %R4
	nop

# Bank3 ################
# Bank3 Start Address :	
	ldi 0x00000735, %R0
	mvtc 0, %R3
	nop
	ldi 0x2000C000, %R0
	mvtc 0, %R4
	nop

# Bank3 End Address	
	ldi 0x00000736, %R0
	mvtc 0, %R3
	nop
	ldi 0x2000FFFF, %R0
	mvtc 0, %R4
	nop

# Bank3 Enable
	ldi 0x00000734, %R0
	mvtc 0, %R3
	nop
	ldi 0x00000111, %R0
	mvtc 0, %R4
	nop
	
################################################################################
#   Initialize stack pointer */    
################################################################################
	ldi _stack-8,%r8 /* stack pointer */
	mov %r8,%sp

################################################################################
#   Initialize Platform */    
################################################################################
	jal _CacheInit
	jal _crt0main
	jal _init_interrupt
    jal _main
	
	halt	0

4:  jmp     4b

.size _Reset_Start, .-_Reset_Start
.section ._stack
_stack:    .long     1
