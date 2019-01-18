//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  gpiomux.h
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__CPUIOMUX_H__
#define		__CPUIOMUX_H__


//======================================================================================================================
// PLL Control Register (CPUPLLCTRL, CPUHCLKSEL)

#define R_CPU_PLL_CTRL		((volatile unsigned int*)(0x72000000 + (0x03D*4)))
#define R_CPU_HCLK_SEL		((volatile unsigned int*)(0x72000000 + (0x040*4)))
#define R_PRE_DIV_MCLK		((volatile unsigned int*)(0x72000000 + (0x032*4)))
#define R_POST_DIV_MCLK		((volatile unsigned int*)(0x72000000 + (0x034*4)))
#define R_PLL_DIS			((volatile unsigned int*)(0x72000000 + (0x020*4)))
#define R_DDR_DRV_ST_SEL	((volatile unsigned int*)(0x72000000 + (0x022*4)))
#define R_HARD_RESET		((volatile unsigned int*)(0x72000000 + (0x072*4)))

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
#define R_ISP_RESET_BY_WDT	((volatile unsigned int*)(0x72000000 + (0x071*4)))
#endif

#define F_CPUHCLKSEL_XTAL1		(0 << 0)
#define	F_CPUHCLKSEL_HOSTCLK	(1 << 0)
#define	F_CPUHCLKSEL_INIT		(2 << 0)
#define	F_CPUHCLKSEL_XTAL2		(4 << 0)
#define	F_CPUHCLKSEL_MCLK		(6 << 0)

#define F_CPUHCLK_DIV1			(1 << 8)
#define F_CPUHCLK_DIV2			(2 << 8)
#define F_CPUHCLK_DIV4			(4 << 8)
#define F_CPUHCLK_DIV8			(8 << 8)

#define	F_CPUPCLK_DLY0			(0 << 5)
#define	F_CPUPCLK_DLY1			(1 << 5)
#define	F_CPUPCLK_DLY2			(2 << 5)
#define	F_CPUPCLK_DLY3			(3 << 5)

#define F_CPUPCLK_INV			(1 << 7)

#define F_PLL_DIS_MCLK_EN		(0 << 1)

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
#define F_SW_RESET_EN			(1 << 0)
#define F_HW_RESET_EN			(1 << 1)
#endif

//======================================================================================================================
// Port Alternate Function Register 0 (cpu_gpio_peri_sel0 )

#define R_PAF0_PAF1 ((volatile unsigned int*)(0x72000000 + (0x08B*4)))
//#define R_PAF1    ((volatile unsigned int*)(0x72000000 + (0x08B*4)))

#define F_PAF0_0_GP0			(0 << 0)
#define F_PAF0_0_PWM5			(1 << 0)
#define F_PAF0_1_GP1			(0 << 1)
#define F_PAF0_1_PWM4			(1 << 1)

#if defined(__USE_MDIN_i500__)		// MDIN-i500
#define F_PAF0_2_GP2			(0 << 2)
#define F_PAF0_2_CAP3			(1 << 2)
#define F_PAF0_3_GP3			(0 << 3)
#define F_PAF0_3_CAP2			(1 << 3)
#define F_PAF0_4_GP4			(0 << 4)
#define F_PAF0_4_CAP1			(1 << 4)
#define F_PAF0_5_GP5			(0 << 5)
#define F_PAF0_5_CAP0			(1 << 5)
#define F_PAF0_6_GP6			(0 << 6)
//#define F_PAF0_6_SENRSTN		(0 << 6)
#define F_PAF0_7_GP7			(0 << 7)
#define F_PAF0_7_PWM3A			(1 << 7)

#define F_PAF1_0_GP0			(0 << 8)
#define F_PAF1_0_CAP5			(1 << 8)
#define F_PAF1_1_GP1			(0 << 9)
#define F_PAF1_1_CAP4			(1 << 9)
#define F_PAF1_2_GP2			(0 << 10)
#define F_PAF1_2_PWM3			(1 << 10)
#define F_PAF1_3_GP3			(0 << 11)
#define F_PAF1_3_PWM2			(1 << 11)
#endif	//__USE_MDIN_i500__


#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
#define F_PAF0_2_GP2			(0 << 2)
#define F_PAF0_2_PWM3			(1 << 2)
#define F_PAF0_3_GP3			(0 << 3)
#define F_PAF0_3_PWM2			(1 << 3)
#define F_PAF0_4_GP4			(0 << 4)
#define F_PAF0_4_CAP3			(1 << 4)
#define F_PAF0_5_GP5			(0 << 5)
#define F_PAF0_5_CAP2			(1 << 5)
#define F_PAF0_6_GP6			(0 << 6)
#define F_PAF0_6_CAP1			(1 << 6)
#define F_PAF0_7_GP7			(0 << 7)
#define F_PAF0_7_CAP0			(1 << 7)

#define F_PAF1_0_GP0			(0 << 8)
//#define F_PAF1_0_SENRSTN		(0 << 8)
#define F_PAF1_1_GP1			(0 << 9)
#define F_PAF1_1_PWM3A			(1 << 9)
#define F_PAF1_2_GP2			(0 << 10)
#define F_PAF1_2_UART_TX2		(1 << 10)
#define F_PAF1_3_GP3			(0 << 11)
#define F_PAF1_3_UART_RX2		(1 << 11)
#endif	// MDIN-i51X (i510, i540, i550)


#define F_PAF1_4_GP4			(0 << 12)
#define F_PAF1_4_PWM1			(1 << 12)
#define F_PAF1_5_GP5			(0 << 13)
#define F_PAF1_5_PWM0			(1 << 13)
#define F_PAF1_6_GP6			(0 << 14)
#define F_PAF1_6_SCL1			(1 << 14)
#define F_PAF1_7_GP7			(0 << 15)
#define F_PAF1_7_SDA1			(1 << 15)


//======================================================================================================================
//======================================================================================================================
// Port Alternate Function Register 1 (cpu_gpio_peri_sel1)

#define R_PAF2    ((volatile unsigned int*)(0x72000000 + (0x08C*4)))

#define F_PAF2_0_GP0			(0 << 0)
#define F_PAF2_0_EIRQ1			(1 << 0)
#define F_PAF2_1_GP1			(0 << 1)
#define F_PAF2_1_EIRQ0			(1 << 1)
#define F_PAF2_2_GP2			(0 << 2)
#define F_PAF2_2_SPI_SSX1		(1 << 2)
#define F_PAF2_3_GP3			(0 << 3)
#define F_PAF2_3_SPI_SCK1		(1 << 3)
#define F_PAF2_4_GP4			(0 << 4)
#define F_PAF2_4_SPI_MOSI1		(1 << 4)
#define F_PAF2_5_GP5			(0 << 5)
#define F_PAF2_5_SPI_MISO1		(1 << 5)
#define F_PAF2_6_GP6			(0 << 6)
#define F_PAF2_6_UART_TX1		(1 << 6)
#define F_PAF2_7_GP7			(0 << 7)
#define F_PAF2_7_UART_RX1		(1 << 7)


#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
//======================================================================================================================
// Port Alternate Function Register(Video In) --  (cpu_gpio_a_vid_in_en)
#define R_GP_A_USE_VID_IO    ((volatile unsigned int*)(0x72000000 + (0x08D*4)))

#define F_GP_A0_GPIO			(0 << 0)
#define F_GP_A0_VID_IN			(1 << 0)
#define F_GP_A1_GPIO			(0 << 1)
#define F_GP_A1_VID_IN			(1 << 1)
#define F_GP_A2_GPIO			(0 << 2)
#define F_GP_A2_VID_IN			(1 << 2)
#define F_GP_A3_GPIO			(0 << 3)
#define F_GP_A3_VID_IN			(1 << 3)
#define F_GP_A4_GPIO			(0 << 4)
#define F_GP_A4_VID_IN			(1 << 4)
#define F_GP_A5_GPIO			(0 << 5)
#define F_GP_A5_VID_IN			(1 << 5)
#define F_GP_A6_GPIO			(0 << 6)
#define F_GP_A6_VID_IN			(1 << 6)
#define F_GP_A7_GPIO			(0 << 7)
#define F_GP_A7_VID_IN			(1 << 7)
#define F_GP_A_GPIO				(0x00 << 0)


// Port Alternate Function Register(Audio In/Out) --  (cpu_gpio_b_aud_in_en, cpu_gpio_c_aud_out_en )
#define R_GP_B_C_USE_AUD_IO    ((volatile unsigned int*)(0x72000000 + (0x08F*4)))

#define F_B0_GPIO				(0 << 0)
#define F_B0_AUD_IN				(1 << 0)
#define F_B1_GPIO				(0 << 1)
#define F_B1_AUD_IN				(1 << 1)
#define F_B2_GPIO				(0 << 2)
#define F_B2_AUD_IN				(1 << 2)
#define F_B3_GPIO				(0 << 3)
#define F_B3_AUD_IN				(1 << 3)
#define F_B4_GPIO				(0 << 4)
#define F_B4_AUD_IN				(1 << 4)
#define F_B5_GPIO				(0 << 5)
#define F_B5_AUD_IN				(1 << 5)
#define F_B6_GPIO				(0 << 6)
#define F_B6_AUD_IN				(1 << 6)
#define F_B7_GPIO				(0 << 7)
#define F_B7_AUD_IN				(1 << 7)
#define F_GP_B_GPIO				(0x00 << 0)

#define F_C0_GPIO				(0 << 8)
#define F_C0_AUD_OUT			(1 << 8)
#define F_C1_GPIO				(0 << 9)
#define F_C1_AUD_OUT			(1 << 9)
#define F_C2_GPIO				(0 << 10)
#define F_C2_AUD_OUT			(1 << 10)
#define F_C3_GPIO				(0 << 11)
#define F_C3_AUD_OUT			(1 << 11)
#define F_C4_GPIO				(0 << 12)
#define F_C4_AUD_OUT			(1 << 12)
#define F_C5_GPIO				(0 << 13)
#define F_C5_AUD_OUT			(1 << 13)
#define F_C6_GPIO				(0 << 14)
#define F_C6_AUD_OUT			(1 << 14)
#define F_C7_GPIO				(0 << 15)
#define F_C7_AUD_OUT			(1 << 15)
#define F_GP_C_GPIO				(0x00 << 8)

#endif


#endif	/* __CPUIOMUX_H__ */
