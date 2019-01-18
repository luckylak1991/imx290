#pragma once

#define ADC_BASE_ADDR	0x80013000


#define R_ADCCON    ((volatile unsigned char*)(ADC_BASE_ADDR+0))
#define F_ADC_OFF   		(0 << 0)
#define F_ADC_ON   			(1 << 0)
#define F_ADC_STC   		(1 << 1)
#define F_ADC_NSTC   		(1 << 2)


#define R_ADCSAM    ((volatile unsigned char*)(ADC_BASE_ADDR+4))
#define F_ADCCLK_SEL_0		(0 << 0)
#define F_ADCCLK_SEL_4		(1 << 0)
#define F_ADCCLK_SEL_8		(2 << 0)
#define F_ADCCLK_SEL_16		(3 << 0)
#define F_ADCCLK_SEL_32		(4 << 0)
#define F_ADCCLK_SEL_64		(5 << 0)
#define F_ADCCLK_SEL_128	(6 << 0)
#define F_ADCCLK_SEL_256	(7 << 0)
#define F_ADCCLK_SEL_512	(8 << 0)
#define F_ADCCLK_SEL_X(x)	(x << 0)	// x = 0~8

#define F_STC_2W   			(2 << 4)
#define F_STC_3W   			(3 << 4)
#define F_STC_4W   			(4 << 4)
#define F_STC_5W   			(5 << 4)
#define F_STC_6W   			(6 << 4)
#define F_STC_7W   			(7 << 4)
#define F_STC_8W   			(8 << 4)
#define F_STC_9W   			(9 << 4)
#define F_STC_10W   		(10 << 4)
#define F_STC_11W   		(11 << 4)
#define F_STC_12W   		(12 << 4)
#define F_STC_13W   		(13 << 4)
#define F_STC_14W   		(14 << 4)
#define F_STC_15W   		(15 << 4)
#define F_STC_WSEL(x)		( x << 4)	// x = 2~15


#define R_ADCCH    ((volatile unsigned int*)(ADC_BASE_ADDR+0x8))
#define F_ADCCH0			(0x00001 << 0)
#define F_ADCCH1			(0x00002 << 0)
#define F_ADCCH2			(0x00004 << 0)
#define F_ADCCH3			(0x00008 << 0)
#define F_ADCCH4			(0x00010 << 0)
#define F_ADCCH5			(0x00020 << 0)
#define F_ADCCH6			(0x00040 << 0)
#define F_ADCCH7			(0x00080 << 0)
#define F_ADCCH8			(0x00100 << 0)
#define F_ADCCH9			(0x00200 << 0)
#define F_ADCCH10			(0x00400 << 0)
#define F_ADCCH11			(0x00800 << 0)
#define F_ADCCH12			(0x01000 << 0)
#define F_ADCCH13			(0x02000 << 0)
#define F_ADCCH14			(0x04000 << 0)
#define F_ADCCH15			(0x08000 << 0)
#define F_ADCCH16			(0x10000 << 0)
#define F_ADCCH_X(x)		(1 << x)	// x = 0~16


#define R_ADCDATA	((volatile unsigned short*)(ADC_BASE_ADDR+0xC))


