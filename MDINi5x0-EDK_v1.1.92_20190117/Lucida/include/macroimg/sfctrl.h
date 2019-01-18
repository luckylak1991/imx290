#pragma once


#define R_SFMOD(N)	((volatile unsigned int*)(0x80000000 + (N * 0xC00)))
#define R_SF0MOD	((volatile unsigned int*)0x80000000)
#define R_SF1MOD	((volatile unsigned int*)0x80000C00)

#define R_SFBRT(N)	((volatile unsigned char*)(0x80000004 + (N * 0xC00)))
#define R_SF0BRT   	((volatile unsigned char*)0x80000004)
#define R_SF1BRT   	((volatile unsigned char*)0x80000C04)

#define R_SFCSH(N)	((volatile unsigned char*)(0x80000008 + (N * 0xC00)))
#define R_SF0CSH   	((volatile unsigned char*)0x80000008)
#define R_SF1CSH   	((volatile unsigned char*)0x80000C08)

#define R_SFPEM(N)	((volatile unsigned char*)(0x8000000C + (N * 0xC00)))
#define R_SF0PEM   	((volatile unsigned char*)0x8000000C)
#define R_SF1PEM   	((volatile unsigned char*)0x80000C0C)

#define R_SFCMD(N)	((volatile unsigned char*)(0x80000010 + (N * 0xC00)))
#define R_SF0CMD   	((volatile unsigned char*)0x80000010)
#define R_SF1CMD   	((volatile unsigned char*)0x80000C10)

#define R_SFSTS(N)	((volatile unsigned char*)(0x80000014 + (N * 0xC00)))
#define R_SF0STS   	((volatile unsigned char*)0x80000014)
#define R_SF1STS   	((volatile unsigned char*)0x80000C14)

#define R_SFSEA(N)	((volatile unsigned long*)(0x80000018 + (N * 0xC00)))
#define R_SF0SEA   	((volatile unsigned long*)0x80000018)
#define R_SF1SEA   	((volatile unsigned long*)0x80000C18)

#define R_SFBEA(N)	((volatile unsigned long*)(0x8000001C + (N * 0xC00)))
#define R_SF0BEA   	((volatile unsigned long*)0x8000001C)
#define R_SF1BEA   	((volatile unsigned long*)0x80000C1C)

#define R_SFDAT(N)		((volatile unsigned long*) (0x80000020 + (N * 0xC00)))
#define R_SFDAT_C(N)	((volatile unsigned char*) (0x80000020 + (N * 0xC00)))
#define R_SFDAT_S(N)	((volatile unsigned short*)(0x80000020 + (N * 0xC00)))
#define R_SFDAT_L(N)	((volatile unsigned long*) (0x80000020 + (N * 0xC00)))
#define R_SF0DAT 	((volatile unsigned long*)0x80000020)
#define R_SF0DAT_C 	((volatile unsigned char*)0x80000020)
#define R_SF0DAT_S 	((volatile unsigned short*)0x80000020)
#define R_SF0DAT_L 	((volatile unsigned long*)0x80000020)
#define R_SF1DAT 	((volatile unsigned long*)0x80000C20)
#define R_SF1DAT_C 	((volatile unsigned char*)0x80000C20)
#define R_SF1DAT_S 	((volatile unsigned short*)0x80000C20)
#define R_SF1DAT_L 	((volatile unsigned long*)0x80000C20)

#define R_SFPRD(N)	((volatile unsigned long*)(0x80000024 + (N * 0xC00)))
#define R_SF0PRD	((volatile unsigned long*)0x80000024)
#define R_SF1PRD	((volatile unsigned long*)0x80000C24)

#define R_SFCKDLY(N)	((volatile unsigned char*)(0x80000028 + (N * 0xC00)))
#define R_SF0CKDLY 	((volatile unsigned char*)0x80000028)
#define R_SF1CKDLY 	((volatile unsigned char*)0x80000C28)

#define R_SFSTS2(N)	((volatile unsigned short*)(0x8000002C + (N * 0xC00)))
#define R_SF0STS2  	((volatile unsigned short*)0x8000002C)
#define R_SF1STS2  	((volatile unsigned short*)0x80000C2C)


void read_id(void);
void power_down(void);
void release_pwdn(void);
void winbond_quad_set(void);
void sfsts_test(void);
void serial_mem_test(void);
void sf_erase_sector(int sector, int len);
void sf_write(U32 addr, U8* buf, int len);
void sf_read(U32 addr, U8* buf, int len);
