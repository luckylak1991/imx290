OUTPUT_FORMAT("elf32-ae32000-little", "elf32-ae32000-big",
			"elf32-ae32000-little")
OUTPUT_ARCH(ae32000)
ENTRY(_Reset_Start)
/*GROUP(-lc -lgcc -lm )*/
/*SEARCH_DIR(.);*/

MEMORY
{
	rom (rx)	: ORIGIN = 0x00000000,	LENGTH = 16M   /* jtag */
/*	rom (rx)	: ORIGIN = 0x00005000,	LENGTH = 16M 	/* boot loader */ /*Serial load*/

/*	ispm(x)		: ORIGIN = 0x10000000,	LENGTH = 16K	/* ispm 16kb */
/*	ispm(x)		: ORIGIN = 0x10000000,	LENGTH = 32K	/* ispm 32kb */
	ispm(x)		: ORIGIN = 0x10000000,	LENGTH = 64K	/* ispm 64kb */

/*	dspm(!rx)	: ORIGIN = 0x20000200,	LENGTH = (16K-512)	/* dspm 16kb */
/*	dspm(!rx)	: ORIGIN = 0x20000200,	LENGTH = (32K-512)	/* dspm 32kb */
	dspm(!rx)	: ORIGIN = 0x20000200,	LENGTH = (48K-512)	/* dspm 48kb */
/*	dspm(!rx)	: ORIGIN = 0x20000200,	LENGTH = (64K-512)	/* dspm 64kb */
}

SECTIONS
{
  .text : {
	*(.vects)
	*(.version)
	*(.text)
	. = ALIGN(4);
	*(.stub)
	*(.rodata)
	*(.rodata.str1.*)
	*(.rodata1)
	___ctors = . ;
	*(.ctors)
	___ctors_end = . ;
	___dtors = . ;
	*(.dtors)
	___dtors_end = . ;
	*(.gnu.linkonce.*) 
	*(.gcc_except_table) 
  } > rom
  _etext = . ;
  PROVIDE (etext = .);
    
  .ispm : AT ( (LOADADDR (.text) + SIZEOF(.text) + 3) & ~ (4 - 1)) {
    . = ALIGN(4);
    __ram_ispm_start = . ;
    *(.ispm)
    . = ALIGN(4);
    __ram_ispm_end = .;
  } > ispm
  __rom_ispm_start  =  LOADADDR (.ispm);

  .data : AT ( (LOADADDR (.ispm) + SIZEOF(.ispm) + 3) & ~ (4 - 1)) {
    . = ALIGN(4);
    __ram_data_start = . ;
	*(.dspm)
	. = ALIGN(4);
    *(.data)
    *(.sdata)
    /*CONSTRUCTORS*/
    . = ALIGN(4);
    __ram_data_end = .;
  } > dspm
  __rom_data_start  =  LOADADDR (.data);

  .bss ALIGN(0x4) : AT ((LOADADDR (.data) + SIZEOF(.data) + 3) & ~ (4 - 1) ) {
	  . = ALIGN(4);
    __bss_start = .;
    *(.dynbss)
    *(.bss)
    *(COMMON)
    . = ALIGN(4);
    __bss_end = .;
  } > dspm

  _end = . ;
  PROVIDE (end = .);

/*  ._stack 0x20003FFC :	/* dspm 16kb */
/*  ._stack 0x20007FFC :	/* dspm 32kb */
  ._stack 0x2000BFFC :	/* dspm 48kb */
/*  ._stack 0x2000FFFC :	/* dspm 64kb */
  {
    _stack = .;
    *(._stack)
  } 
}
PROVIDE(___data_size = SIZEOF(.data));
PROVIDE(___bss_size = SIZEOF(.bss));
