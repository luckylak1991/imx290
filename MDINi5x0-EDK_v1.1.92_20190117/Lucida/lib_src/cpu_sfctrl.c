#include "macroimg.h"

#define SFMBASE_C(a)  *(volatile unsigned char  *) (0x30000000 + a)
#define SFMBASE_S(a)  *(volatile unsigned short *) (0x30000000 + a)
#define SFMBASE_L(a)  *(volatile unsigned long  *) (0x30000000 + a)

#define START_ADDR  0x01000000 
#define TEST_SIZE   256 

//void read_id(void) __attribute__((section(".data")));
void read_id(void)
{
	debugstring("SPI Flash ID Read \r\n");
	
    *R_SF1MOD = 0x0;
    *R_SF1DAT_C = 0x9f;
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);
	
    *R_SF1MOD = 0x100;
    *R_SF1MOD = 0x0;
    *R_SF1DAT_C = 0x4B;

	// for 4
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);

	// for 8
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);

    *R_SF1MOD = 0x100;
}

void power_down(void) {
	debugstring("Power down \r\n");
	*R_SF1MOD = 0x0;
	*R_SF1DAT_C = 0xB9;
	*R_SF1MOD = 0x100;
}

void release_pwdn(void) {
	debugstring("Release Power down \r\n");
	*R_SF1MOD = 0x0;
	*R_SF1DAT_C = 0xAB;
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);	// Dummy
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);	// Dummy
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);	// Dummy
	debugprintf("R_SF1DAT_C = 0x%x \r\n", *R_SF1DAT_C);	// Device ID
	*R_SF1MOD = 0x100;
}

void winbond_quad_set(void) {
	debugstring("Write Enable \r\n");
    *R_SF1CMD = 0x06;
	debugprintf("R_SF1STS = 0x%x \r\n", *R_SF1STS);
	debugprintf("R_SF1STS2 = 0x%x \r\n", *R_SF1STS2);
    *R_SF1STS2 = 0x200;
    *R_SF1MOD  = 0x102;
}

void sfsts_test(void) {
	debugstring("Write Enable \r\n");
    *R_SF1CMD = 0x06;
	debugprintf("R_SF1STS = 0x%x \r\n", *R_SF1STS);

	debugstring("Write Status \r\n");
    *R_SF1STS = 0x00;
}

//void serial_mem_test(void) __attribute__((section(".data")));
void serial_mem_test(void) {
    unsigned int n;

	debugstring("\r\n\r\nSerial Flash Test  \r\n");

	debugprintf(" R_SF1MOD = 0x%x \r\n", *R_SF1MOD);	// 0x8102

	debugstring(" Sector Erase \r\n");
    *R_SF1SEA = 0x1f000;

	debugstring(" Block Erase \r\n");
    *R_SF1BEA = 0x20000;

	debugstring(" Flash Write \r\n");
	
    SFMBASE_L(0x1f000) = 0x13579ACE;
	debugprintf(" Flash Read = 0x%x \r\n", SFMBASE_L(0x1f000));

    SFMBASE_S(0x1f004) = 0x159C;
	debugprintf(" Flash Read = 0x%x \r\n", SFMBASE_S(0x1f004));

    SFMBASE_C(0x1f006) = 0x3A;
	debugprintf(" Flash Read = 0x%x \r\n", SFMBASE_C(0x1f006));

    SFMBASE_C(0x1f007) = 0x7E;
	debugprintf(" Flash Read = 0x%x \r\n", SFMBASE_C(0x1f007));

	
	debugstring("Sector Erase \r\n");
    *R_SF1SEA = START_ADDR;

	debugstring("Block Erase \r\n");
    *R_SF1BEA = START_ADDR + 0X20000;

	debugstring("Flash Write \r\n");
    for (n=START_ADDR; n<(START_ADDR + TEST_SIZE); n=n+4)
		SFMBASE_L(0x20000+n) = 0x00010001 * n;

	debugstring("Flash Compare \r\n");
    for (n=START_ADDR; n<(START_ADDR + TEST_SIZE); n=n+4) {
		if (SFMBASE_L(0x20000+n) != 0x00010001 * n) {
			debugprintf(" Error, Flash Write = 0x%x \r\n", 0x00010001*n);
			debugprintf(" Error, Flash Read = 0x%x \r\n", SFMBASE_L(0x20000+n));
		}
    }

	debugprintf(" Flash Read = 0x%x \r\n", SFMBASE_L(0x1f000));
	debugprintf(" Flash Read = 0x%x \r\n", SFMBASE_S(0x1f004));
	debugprintf(" Flash Read = 0x%x \r\n", SFMBASE_C(0x1f006));
	debugprintf(" Flash Read = 0x%x \r\n", SFMBASE_C(0x1f007));
	
	debugstring("Flash Memory Test End \r\n");

    //R_SF1PEM = 1;
}

static void sf_block_protect_enable(void)	//bh 2017.12.21
{
	U32 sts1, sts2, mode;
	
	sts1 = *R_SF0STS & 0xff;
	sts2 = *R_SF0STS2 & 0xff;
	mode = *R_SF0MOD;
	
	*R_SF0MOD = mode & (~0x03);					// quad read disable(0:single, 1:dual, 2:quad)
	
	// status register-1
	*R_SF0CMD = 0x06;							// status reg. write enable
	*R_SF0STS = sts1 | 0x1c;					// block protect bits(bp2, bp1, bp0) enable
	
	// status register-1&2
	*R_SF0CMD = 0x06;							// status reg. write enable
	*R_SF0STS2 = (sts2<<8) | (sts1 | 0x1c);		// block protect bits(bp2, bp1, bp0) enable
	
	*R_SF0CMD = 0x04;							// status reg. write disable
	*R_SF0MOD = mode | 0x02;					// quad read enable(0:single, 1:dual, 2:quad)
}

static void sf_block_protect_disable(void)	//bh 2017.12.21
{
	U32 sts1, sts2, mode;
	
	sts1 = *R_SF0STS & 0xff;
	sts2 = *R_SF0STS2 & 0xff;
	mode = *R_SF0MOD;
	
	*R_SF0MOD = mode & (~0x03);					// quad read disable(0:single, 1:dual, 2:quad)
	
	// status register-1
	*R_SF0CMD = 0x06;							// status reg. write enable
	*R_SF0STS = sts1 & (~0x1c);					// block protect bits(bp2, bp1, bp0) clear
	
	// status register-1&2
	*R_SF0CMD = 0x06;							// status reg. write enable
	*R_SF0STS2 = (sts2<<8) | (sts1 & (~0x1c));	// block protect bits(bp2, bp1, bp0) clear
	
	*R_SF0CMD = 0x04;							// status reg. write disable
	*R_SF0MOD = mode | 0x02;					// quad read enable(0:single, 1:dual, 2:quad)
}

void sf_erase_sector(int sector, int len) //bh 2017.12.21
{
	int i, flash_sector_size=4*1024;
	U32 mode, addr;
	
	CacheDisable();
	sf_block_protect_disable();
	mode = *R_SFMOD(0);
	*R_SFMOD(0) &= ~(1<<7);	//write enable
	addr = sector*flash_sector_size;
	for(i=0; i<len; i++)
	{
		*R_SFSEA(0)=addr;
		addr += flash_sector_size;
	}
	*R_SFMOD(0) = mode;
	sf_block_protect_enable();
	CacheEnable();
}

void sf_write(U32 addr, U8* buf, int len) //bh 2017.12.21
{
	U32 mode;
	
	CacheDisable();
	sf_block_protect_disable();
	mode = *R_SFMOD(0);
	*R_SFMOD(0) &= ~(1<<7);	//write enable
	memcpy((void*)addr, (void*)buf, len);
	*R_SFMOD(0) = mode;
	sf_block_protect_enable();
	CacheEnable();
}

void sf_read(U32 addr, U8* buf, int len)  //bh 2014.11.20
{
	U32 mode;
	
//	CacheDisable();
	mode = *R_SFMOD(0);
	memcpy((void*)buf, (void*)addr, len);
//	CacheEnable();
//	debugprintf("flash mode = %x\r\n", mode);
}
