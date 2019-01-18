//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2005  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	INIT.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include 	"video.h"
#include 	"gpiomux.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
extern WORD tick, lead, repeat, data_0, data_1;
extern WORD tol_lead, tol_repeat, tol_data_0, tol_data_1;
// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Global functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
static void SetPortDirection(void)
{
#if	defined(__MDIN_i500_EDK__) || defined(__MDIN_i500_REF__)			// EDK & REF board
	//gpio pin mux setting
#if defined(__P_IRIS_TEST__)
	*R_PAF0_PAF1 = F_PAF1_6_SCL1 | F_PAF1_7_SDA1;						//I2C1
#else
 	*R_PAF0_PAF1 = F_PAF0_0_PWM5 | F_PAF0_7_PWM3A | \
				F_PAF1_6_SCL1 | F_PAF1_7_SDA1;							//PWM5, IRIS_PWM_OUT, I2C1
#endif
	
#if defined(__USE_CPU_SPI_SLAVE__)
	*R_PAF2 = F_PAF2_0_EIRQ1 | F_PAF2_6_UART_TX1 | F_PAF2_7_UART_RX1 | \
			F_PAF2_2_SPI_SSX1 | F_PAF2_3_SPI_SCK1 | F_PAF2_4_SPI_MOSI1 | F_PAF2_5_SPI_MISO1;	//EIRQ1, UART1, SPI1
#else
	*R_PAF2 = F_PAF2_0_EIRQ1 | F_PAF2_6_UART_TX1 | F_PAF2_7_UART_RX1 | \
			F_PAF2_3_SPI_SCK1 | F_PAF2_4_SPI_MOSI1 | F_PAF2_5_SPI_MISO1;	//EIRQ1, UART1, SPI1
#endif
	
	//gpio pin direction
	*R_PODIR(0) = 0xF7;	//Port.A : Out(P0.0~2, P0.4~7)					-- ISP_VACT(ext), 485_DE, ISP_MD_OUT(ext), ISP_IRQ_N(ext), MCU_CS, SENSOR_RESETN, IRIS_PWM_OUT
	*R_PODIR(1) = 0xCF;	//Port.B : Out(P1.0~3, P1.6~7)             		-- DAY_CTRL, NIGHT_CTRL, LENS_VD, IRIS_VD, MISP_SCL, MISP_SDA
	*R_PODIR(2) = 0xFE;	//Port.C : Out(P2.1~7) 							-- ZF_CS, IRIS_CS, PERI_SCK/MOSI/MISO, 485_TX/RX
	*R_PODIR(3) = 0x00;	//Port.D : Out() 								-- 
	
	*R_PIDIR(0) = 0x08;	//Port.A : in(P0.3)								-- SWITCH
	*R_PIDIR(1) = 0x30;	//Port.B : in(P1.4~5)							-- FOCUS_RESET, ZOOM_RESET
	*R_PIDIR(2) = 0x01;	//Port.C : in(P2.0)								-- IRR_IN
	*R_PIDIR(3) = 0xFF;	//Port.D : in(P3.0 ~ P3.7)						-- vact(3a), hact(3a), md_flag_out, vact(yc2md), irq_n, dis_interrupt, vdpulse_host, hact_pwm(3a)
	
	SENSOR_RSTN(LOW);	SDI_RATE(HIGH);
	LENS_CS(LOW);	LENS_VD(LOW);	IRIS_CS(LOW);	IRIS_VD(LOW);		// LENS & IRIS
	OLPF_DY(HIGH);	OLPF_NT(HIGH);					// set OLPF
	DE_485(LOW); 									// set RS485 direction to rx(default)
#endif	//__MDIN_i500_EDK__, __MDIN_i500_REF__


#if	defined(__MDIN_i510_EDK__)											// i510 EDK(socket) board
	//gpio pin mux setting
	*R_GP_A_USE_VID_IO = F_GP_A_GPIO;
	*R_GP_B_C_USE_AUD_IO = F_GP_B_GPIO | F_GP_C_GPIO;
	
	*R_PAF0_PAF1 = F_PAF1_1_PWM3A | F_PAF1_6_SCL1 | F_PAF1_7_SDA1;	// IRIS_PWM_OUT, I2C1	// for 4byte
#if defined(__USE_CPU_SPI_SLAVE__)
	*R_PAF2 = F_PAF2_0_EIRQ1 | F_PAF2_6_UART_TX1 | F_PAF2_7_UART_RX1 | \
			F_PAF2_2_SPI_SSX1 | F_PAF2_3_SPI_SCK1 | F_PAF2_4_SPI_MOSI1 | F_PAF2_5_SPI_MISO1;	// EIRQ1, UART1, SPI1
#else
	*R_PAF2 = F_PAF2_0_EIRQ1 | F_PAF2_6_UART_TX1 | F_PAF2_7_UART_RX1 | \
			F_PAF2_3_SPI_SCK1 | F_PAF2_4_SPI_MOSI1 | F_PAF2_5_SPI_MISO1;	// EIRQ1, UART1, SPI1
#endif
	
	//gpio pin direction
	*R_PODIR(0) = 0xFF;	//Port.A : Out(-)								-- n.c
	*R_PODIR(1) = 0xCF;	//Port.B : Out(P1.0~3, P1.6~7)             		-- [0]SENSOR_RESETN, [1]IRIS_PWM_OUT,  [2]LENS_VD, [3]IRIS_VD, [6]MISP_SCL, [7]MISP_SDA
	*R_PODIR(2) = 0xFE;	//Port.C : Out(P2.1~7) 							-- [1]ZF_CS, [2]IRIS_CS, [3]PERI_SCK, [4]MOSI, [5]MISO, [6]485_TX, [7]485_RX
	*R_PODIR(3) = 0x00;	//Port.D : Out(-) 								-- 
	
	*R_PIDIR(0) = 0x00;	//Port.A : in(-)								-- 
	*R_PIDIR(1) = 0x30;	//Port.B : in(P1.4~5)							-- [4]FOCUS_RESET, [5]ZOOM_RESET
	*R_PIDIR(2) = 0x01;	//Port.C : in(P2.0)								-- [0]IRR_IN
	*R_PIDIR(3) = 0xFF;	//Port.D : in(P3.0 ~ P3.7)						-- vact(3a), hact(3a), md_flag_out, vact(yc2md), irq_n, dis_interrupt, vdpulse_host, hact_pwm(3a)
	
	SENSOR_RSTN(LOW);	SDI_RATE(HIGH);
	OLPF_DY(HIGH);	OLPF_NT(HIGH);					// set OLPF
	DE_485(LOW); 									// set RS485 direction to rx(default)
#endif	//__MDIN_i510_EDK__


#if	defined(__MDIN_i510_REF__)											// i510 REF board
	//gpio pin mux setting
	*R_GP_A_USE_VID_IO = F_GP_A_GPIO;
	*R_GP_B_C_USE_AUD_IO = F_GP_B_GPIO | F_GP_C_GPIO;
	
	*R_PAF0_PAF1 = F_PAF1_1_PWM3A | F_PAF1_6_SCL1 | F_PAF1_7_SDA1;		// IRIS_PWM_OUT, I2C1
#if defined(__USE_CPU_SPI_SLAVE__)
	*R_PAF2 = F_PAF2_0_EIRQ1 | F_PAF2_6_UART_TX1 | F_PAF2_7_UART_RX1 | \
			F_PAF2_2_SPI_SSX1 | F_PAF2_3_SPI_SCK1 | F_PAF2_4_SPI_MOSI1 | F_PAF2_5_SPI_MISO1;	//EIRQ1, UART1, SPI1
#else
	*R_PAF2 = F_PAF2_0_EIRQ1 | F_PAF2_6_UART_TX1 | F_PAF2_7_UART_RX1 | \
			F_PAF2_3_SPI_SCK1 | F_PAF2_4_SPI_MOSI1 | F_PAF2_5_SPI_MISO1;	//EIRQ1, UART1, SPI1
#endif
	
	//gpio open drain mode
	*R_P0ODM = 0x00;	//Port A : Open Drain(-)						--
	*R_P1ODM = 0x00;	//Port B : Open Drain(-)						--
	*R_P2ODM = 0x00;	//Port C : Open Drain(-)						--
	*R_P3ODM = 0x00;	//Port D : Open Drain(-)						--

	//gpio pin direction
	*R_PODIR(0) = 0xFF;	//Port.A : Out(-)								-- n.c
	*R_PODIR(1) = 0xCF;	//Port.B : Out(P1.0~3, P1.6~7)             		-- [0]SENSOR_RESETN, [1]IRIS_PWM_OUT,  [2]LENS_VD, [3]IRIS_VD, [6]MISP_SCL, [7]MISP_SDA
	*R_PODIR(2) = 0xFA;	//Port.C : Out(P2.1, P2.3~7) 					-- [1]ZF_CS, [3]PERI_SCK, [4]MOSI, [5]MISO, [6]485_TX, [7]RX
	*R_PODIR(3) = 0x00;	//Port.D : Out(-) 								-- 
	
	*R_PIDIR(0) = 0x00;	//Port.A : in(-)								-- 
	*R_PIDIR(1) = 0x30;	//Port.B : in(-)								-- [4]FOCUS_RESET, [5]ZOOM_RESET
	*R_PIDIR(2) = 0x05;	//Port.C : in(P2.0, P2.2)						-- [0]IRR_IN, [2]INT_IO_EXP
	*R_PIDIR(3) = 0xFF;	//Port.D : in(P3.0 ~ P3.7)						-- vact(3a), hact(3a), md_flag_out, vact(yc2md), irq_n, dis_interrupt, vdpulse_host, hact_pwm(3a)

	SENSOR_RSTN(LOW);
	LENS_CS(LOW);	LENS_VD(LOW);	IRIS_CS(LOW);	IRIS_VD(LOW);		// LENS & IRIS
#endif	//__MDIN_i510_REF__


#if	defined(__MDIN_i510_IMX290__)											// i510 + IMX290 board
	//gpio pin mux setting
	*R_GP_A_USE_VID_IO = F_GP_A_GPIO;
	*R_GP_B_C_USE_AUD_IO = F_GP_B_GPIO | F_GP_C_GPIO;
	
	*R_PAF0_PAF1 = F_PAF1_1_PWM3A;						// IRIS_PWM_OUT	// for 4byte
#if defined(__USE_CPU_SPI_SLAVE__)
	*R_PAF2 = F_PAF2_0_EIRQ1 | F_PAF2_6_UART_TX1 | F_PAF2_7_UART_RX1 | \
			F_PAF2_2_SPI_SSX1 | F_PAF2_3_SPI_SCK1 | F_PAF2_4_SPI_MOSI1 | F_PAF2_5_SPI_MISO1;	//EIRQ1, UART1, SPI1
#else
	*R_PAF2 = F_PAF2_0_EIRQ1 | F_PAF2_6_UART_TX1 | F_PAF2_7_UART_RX1 | \
			F_PAF2_3_SPI_SCK1 | F_PAF2_4_SPI_MOSI1 | F_PAF2_5_SPI_MISO1;	//EIRQ1, UART1, SPI1
#endif
	
	//gpio pin direction
	*R_PODIR(0) = 0xFF;	//Port.A : Out(-)								-- n.c
	*R_PODIR(1) = 0xFF;	//Port.B : Out(P1.0~7)             				-- [0]SENSOR_RESETN, [1]IRIS_PWM_OUT,  [2]SDI_DIV_1001, [3]DE_485, [4]DAY_CTRL, [5]NIGHT_CTRL, [6]SDI_DET_TRS, [7]SDI_20B_10B
	*R_PODIR(2) = 0xFE;	//Port.C : Out(P2.1~7) 							-- [1]SDI_RATE, [2]SDI_TX_CS, [3]PERI_SCK, [4]MOSI, [5]MISO, [6]485_TX, [7]RX
	*R_PODIR(3) = 0x00;	//Port.D : Out(-) 								-- 
	
	*R_PIDIR(0) = 0x00;	//Port.A : in(-)								-- 
	*R_PIDIR(1) = 0x00;	//Port.B : in(-)								-- 
	*R_PIDIR(2) = 0x01;	//Port.C : in(P2.0)								-- [0]IRR_IN
	*R_PIDIR(3) = 0xFF;	//Port.D : in(P3.0 ~ P3.7)						-- vact(3a), hact(3a), md_flag_out, vact(yc2md), irq_n, dis_interrupt, vdpulse_host, hact_pwm(3a)

	SENSOR_RSTN(LOW);	
	//SDI_RATE(LOW);	// 1.5G
	SDI_RATE(HIGH);	// 3G
	SDI_DIV_1001(LOW);	SDI_DET_TRS(HIGH);	SDI_20B_10B(HIGH);			// 59.94(x), vout_sync_emb, vout_20b 	20161011
	OLPF_DY(HIGH);	OLPF_NT(HIGH);					// set OLPF
	DE_485(LOW); 									// set RS485 direction to rx(default)
#endif	//__MDIN_i510_IMX290__


#if	defined(__MDIN_i540_EDK__) || defined(__MDIN_i540_REF__)			// i540 EDK & REF board
	//gpio pin mux setting
	*R_GP_A_USE_VID_IO = F_GP_A_GPIO;
	*R_GP_B_C_USE_AUD_IO = F_GP_B_GPIO | F_GP_C_GPIO;
	
#if defined(__USE_IR_LED_PWM__)
	*R_PAF0_PAF1 = F_PAF0_0_PWM5 | F_PAF1_1_PWM3A | F_PAF1_6_SCL1 | F_PAF1_7_SDA1;	//PWM5, IRIS_PWM_OUT, I2C1
#else
	*R_PAF0_PAF1 = F_PAF1_1_PWM3A | F_PAF1_6_SCL1 | F_PAF1_7_SDA1;	//IRIS_PWM_OUT, I2C1
#endif
	
#if defined(__USE_CPU_SPI_SLAVE__)
	*R_PAF2 = F_PAF2_0_EIRQ1 | F_PAF2_6_UART_TX1 | F_PAF2_7_UART_RX1 | \
			F_PAF2_2_SPI_SSX1 | F_PAF2_3_SPI_SCK1 | F_PAF2_4_SPI_MOSI1 | F_PAF2_5_SPI_MISO1;	//EIRQ1, UART1, SPI1
#else
	*R_PAF2 = F_PAF2_0_EIRQ1 | F_PAF2_6_UART_TX1 | F_PAF2_7_UART_RX1 | \
			F_PAF2_3_SPI_SCK1 | F_PAF2_4_SPI_MOSI1 | F_PAF2_5_SPI_MISO1;	//EIRQ1, UART1, SPI1
#endif
	
	//gpio open drain mode
	*R_P0ODM = 0x00;	//Port A : Open Drain(-)						--
	*R_P1ODM = 0x00;	//Port B : Open Drain(-)						--
	*R_P2ODM = 0x00;	//Port C : Open Drain(-)						--
	*R_P3ODM = 0x00;	//Port D : Open Drain(-)						--

	//gpio pin direction
	*R_PODIR(0) = 0xF3;	//Port.A : Out(P0.0~1, P0.4~7)					-- [0]PWM5, [1]485_DE, [4]SDI_RATE_SEL, [5]HDTX_RESET, [6]DAY_CTRL, [7]NIGHT_CTRL
	*R_PODIR(1) = 0xCF;	//Port.B : Out(P1.0~3, P1.6~7)             		-- [0]SENSOR_RESETN, [1]IRIS_PWM_OUT,  [2]LENS_VD, [3]IRIS_VD, [6]MISP_SCL, [7]MISP_SDA
	*R_PODIR(2) = 0xFE;	//Port.C : Out(P2.1~7) 							-- [1]ZF_CS, [2]IRIS_CS, [3]PERI_SCK, [4]MOSI, [5]MISO, [6]485_TX, [7]485_RX
	*R_PODIR(3) = 0x00;	//Port.D : Out(-) 								-- 
	
	*R_PIDIR(0) = 0x0C;	//Port.A : in(P0.2~3)							-- [2]HDMI_TX_INT, [3]SWITCH
	*R_PIDIR(1) = 0x30;	//Port.B : in(P1.4~5)							-- [4]FOCUS_RESET, [5]ZOOM_RESET
	*R_PIDIR(2) = 0x01;	//Port.C : in(P2.0)								-- [0]IRR_IN
	*R_PIDIR(3) = 0xFF;	//Port.D : in(P3.0 ~ P3.7)						-- vact(3a), hact(3a), md_flag_out, vact(yc2md), irq_n, dis_interrupt, vdpulse_host, hact_pwm(3a)
	
	SENSOR_RSTN(LOW);	SDI_RATE(HIGH);
	LENS_CS(LOW);	LENS_VD(LOW);	IRIS_CS(LOW);	IRIS_VD(LOW);		// LENS & IRIS
	OLPF_DY(HIGH);	OLPF_NT(HIGH);					// set OLPF
	DE_485(LOW); 									// set RS485 direction to rx(default)
#endif	//__MDIN_i540_EDK__, __MDIN_i540_REF__


#if	defined(__MDIN_i540_4K60__)
	//gpio pin mux setting
	*R_GP_A_USE_VID_IO = F_GP_A_GPIO;
	*R_GP_B_C_USE_AUD_IO = F_GP_B_GPIO | F_GP_C_GPIO;
	
	*R_PAF0_PAF1 = F_PAF1_1_PWM3A | F_PAF1_6_SCL1 | F_PAF1_7_SDA1;	//IRIS_PWM_OUT, I2C1
	
#if defined(__USE_CPU_SPI_SLAVE__)
	*R_PAF2 = F_PAF2_0_EIRQ1 | F_PAF2_6_UART_TX1 | F_PAF2_7_UART_RX1 | \
			F_PAF2_2_SPI_SSX1 | F_PAF2_3_SPI_SCK1 | F_PAF2_4_SPI_MOSI1 | F_PAF2_5_SPI_MISO1;	//EIRQ1, UART1, SPI1
#else
	*R_PAF2 = F_PAF2_0_EIRQ1 | F_PAF2_6_UART_TX1 | F_PAF2_7_UART_RX1 | \
			F_PAF2_3_SPI_SCK1 | F_PAF2_4_SPI_MOSI1 | F_PAF2_5_SPI_MISO1;	//EIRQ1, UART1, SPI1
#endif
	
	//gpio pin direction
	*R_PODIR(0) = 0xFA;	//Port.A : Out(P0.1, P0.3~7)					-- [1]485_DE, [3]TX_RESETN, [4]NC, [5]41908_SPI_EN, [6]DAY_CTRL, [7]NIGHT_CTRL
	*R_PODIR(1) = 0xCF;	//Port.B : Out(P1.0~3, P1.6~7)             		-- [0]SENSOR_RESETN, [1]IRIS_PWM_OUT,  [2]LENS_VD, [3]IRIS_VD, [6]MISP_SCL, [7]MISP_SDA
	*R_PODIR(2) = 0xFE;	//Port.C : Out(P2.1~7) 							-- [1]ZF_CS, [2]PERI_CS, [3]PERI_SCK, [4]MOSI, [5]MISO, [6]485_TX, [7]485_RX
	*R_PODIR(3) = 0x00;	//Port.D : Out(-) 								-- 
	
	*R_PIDIR(0) = 0x05;	//Port.A : in(P0.0, P0.2)						-- [0]IRR_IN, [2]HDMI_TX_INT
	*R_PIDIR(1) = 0x30;	//Port.B : in(P1.4~5)							-- [4]FOCUS_RESET, [5]ZOOM_RESET
	*R_PIDIR(2) = 0x01;	//Port.C : in(P2.0)								-- [0]FPGA_INT
	*R_PIDIR(3) = 0xFF;	//Port.D : in(P3.0 ~ P3.7)						-- vact(3a), hact(3a), md_flag_out, vact(yc2md), irq_n, dis_interrupt, vdpulse_host, hact_pwm(3a)
	
	SENSOR_RSTN(LOW);	SDI_RATE(HIGH);
	LENS_CS(LOW);	LENS_VD(LOW);	IRIS_CS(LOW);	IRIS_VD(LOW);		// LENS & IRIS
	OLPF_DY(HIGH);	OLPF_NT(HIGH);					// set OLPF
	DE_485(LOW); 									// set RS485 direction to rx(default)
	ZF_MISO_EN(LOW);
	MISP_CS_S2(HIGH);
#endif


#if	defined(__MDIN_i550_EDK__) || defined(__MDIN_i550_REF__)			// i550 EDK & REF board
	//gpio pin mux setting
	*R_GP_A_USE_VID_IO = F_GP_A_GPIO;
	*R_GP_B_C_USE_AUD_IO = F_GP_B_GPIO | F_GP_C_GPIO;
	
#if defined(__USE_IR_LED_PWM__)
	*R_PAF0_PAF1 = F_PAF0_0_PWM5 | F_PAF1_1_PWM3A | F_PAF1_6_SCL1 | F_PAF1_7_SDA1;	//PWM5, IRIS_PWM_OUT, I2C1
#else
	*R_PAF0_PAF1 = F_PAF1_1_PWM3A | F_PAF1_6_SCL1 | F_PAF1_7_SDA1;	//PWM5, IRIS_PWM_OUT, I2C1
#endif
	
#if defined(__USE_CPU_SPI_SLAVE__)
	*R_PAF2 = F_PAF2_0_EIRQ1 | F_PAF2_6_UART_TX1 | F_PAF2_7_UART_RX1 | \
			F_PAF2_2_SPI_SSX1 | F_PAF2_3_SPI_SCK1 | F_PAF2_4_SPI_MOSI1 | F_PAF2_5_SPI_MISO1;	//EIRQ1, UART1, SPI1
#else
	*R_PAF2 = F_PAF2_0_EIRQ1 | F_PAF2_6_UART_TX1 | F_PAF2_7_UART_RX1 | \
			F_PAF2_3_SPI_SCK1 | F_PAF2_4_SPI_MOSI1 | F_PAF2_5_SPI_MISO1;	//EIRQ1, UART1, SPI1
#endif
	
	//gpio open drain mode
	*R_P0ODM = 0x00;	//Port A : Open Drain(-)						--
	*R_P1ODM = 0x00;	//Port B : Open Drain(-)						--
	*R_P2ODM = 0x00;	//Port C : Open Drain(-)						--
	*R_P3ODM = 0x00;	//Port D : Open Drain(-)						--

	//gpio pin direction
	*R_PODIR(0) = 0xF3;	//Port.A : Out(P0.0~1, P0.4~7)					-- [0]PWM5, [1]485_DE, [4]SDI_RATE_SEL, [5]MCU_CS, [6]DAY_CTRL, [7]NIGHT_CTRL
	*R_PODIR(1) = 0xCF;	//Port.B : Out(P1.0~3, P1.6~7)             		-- [0]SENSOR_RESETN, [1]IRIS_PWM_OUT,  [2]LENS_VD, [3]IRIS_VD, [6]MISP_SCL, [7]MISP_SDA
	*R_PODIR(2) = 0xFE;	//Port.C : Out(P2.1~7) 							-- [1]ZF_CS, [2]IRIS_CS, [3]PERI_SCK, [4]MOSI, [5]MISO, [6]485_TX, [7]485_RX
	*R_PODIR(3) = 0x00;	//Port.D : Out(-) 								-- 
	
	*R_PIDIR(0) = 0x0C;	//Port.A : in(P0.3)								-- [2]HDMI_TX_INT, [3]SWITCH
	*R_PIDIR(1) = 0x30;	//Port.B : in(P1.4~5)							-- [4]FOCUS_RESET, [5]ZOOM_RESET
	*R_PIDIR(2) = 0x01;	//Port.C : in(P2.0)								-- [0]IRR_IN
	*R_PIDIR(3) = 0xFF;	//Port.D : in(P3.0 ~ P3.7)						-- vact(3a), hact(3a), md_flag_out, vact(yc2md), irq_n, dis_interrupt, vdpulse_host, hact_pwm(3a)
	
	SENSOR_RSTN(LOW);	SDI_RATE(HIGH);
	LENS_CS(LOW);	LENS_VD(LOW);	IRIS_CS(LOW);	IRIS_VD(LOW);		// LENS & IRIS
	OLPF_DY(HIGH);	OLPF_NT(HIGH);					// set OLPF
	DE_485(LOW); 									// set RS485 direction to rx(default)
#endif	//__MDIN_i550_EDK__, __MDIN_i550_REF__


#if defined(__MDIN_i5XX_FPGA__)											// FPGA board
	//gpio pin mux setting
	*R_GP_A_USE_VID_IO = F_GP_A_GPIO;
	*R_GP_B_C_USE_AUD_IO = F_GP_B_GPIO | F_GP_C_GPIO;
//	*R_GP_B_C_USE_AUD_IO = F_B0_AUD_IN | F_B1_AUD_IN | F_B2_AUD_IN | F_B3_AUD_IN | F_B4_AUD_IN | F_B5_GPIO | F_B6_GPIO | F_B7_GPIO;
//	*R_GP_B_C_USE_AUD_IO |= F_C0_GPIO | F_C1_AUD_OUT | F_C2_AUD_OUT | F_C3_AUD_OUT | F_C4_AUD_OUT | F_C5_AUD_OUT | F_C6_AUD_OUT | F_C7_GPIO;
	
	*R_PAF0_PAF1 = 0;													//
	*R_PAF2 = F_PAF2_0_EIRQ1 | F_PAF2_6_UART_TX1 | F_PAF2_7_UART_RX1;	//EIRQ1, UART1
	
	//gpio pin direction
	*R_PODIR(0) = 0xF4;	//Port.A : Out(P0.2, P0.4, P0.5, P0.6, P0.7)	-- MICOM_RST_OUT_N(CHIP_EN), DY_CTRL, NT_CTRL, MD_OUT, FPGA_CDS_IN(LENS_CS 로 사용)
	*R_PODIR(1) = 0x20;	//Port.B : Out(P1.5)                    		-- IRIS_VD
	*R_PODIR(2) = 0x3C;	//Port.C : Out(P2.2, P2.3, P2.4, P2.5) 			-- IRIS_CS, CMOS_CS, 485_DE, LENS_VD
	*R_PODIR(3) = 0x00;	//Port.D : Out(P3.7) 							-- isp_vact_wdr_sl_flag_in
	
	*R_PIDIR(0) = 0x0B;	//Port.A : in(P0.0, P0.1, P0.3)					-- IRIS_TYPE, PD2(LENS_FRST), FPGA_EXT_IN
	*R_PIDIR(1) = 0xDF;	//Port.B : in(P1.0~P1.4, P1.6, P1.7)			-- MENU_xx, SCL1, SDA1
	*R_PIDIR(2) = 0xC3;	//Port.C : in(P2.0, P2.1, P2.6, P2.7)			-- IR_IN, N.C(LENS_ZRST), UART1_TX, UART1_RX
	*R_PIDIR(3) = 0xFF;	//Port.D : in(P3.0 ~ P3.6)						-- vact(3a), hact(3a), md_flag, vact(yc2md), v(yc2nr), iris_pwm, v(odm2wdr)
	
	LENS_CS(LOW);	LENS_VD(LOW);	IRIS_CS(LOW);	IRIS_VD(LOW);		// LENS & IRIS
	OLPF_DY(HIGH);	OLPF_NT(HIGH);					// set OLPF
	DE_485(LOW); 									// set RS485 direction to rx(default)
	CMOS_CS(HIGH);									// set SPI-CS
#endif	// __MDIN_i5XX_FPGA__
	

#if defined(__ECOM1_UNION_12X__)
	OSC_Select(LOW);	//tommy HIGH:26.987Mhz, LOW:27MH
#endif
	
#if	defined (__USE_TVI_TX__)
	TVI_RESET(LOW);
#endif

#if	defined (__USE_SIL9136_TX__)
	HDTX_RESET(LOW);
#endif
}



//--------------------------------------------------------------------------------------------------------------------------
static void SetExtIrqFunction(void)
{
	// IRQ(1)
	*R_EIRQMOD = F_EIRQMOD_EIRQ1FED;		// EIRQ1(IR_IN)
	set_interrupt(INTNUM_EIRQ1,EIRQ1_ISR);
	enable_interrupt(INTNUM_EIRQ1,TRUE);	// Interrupt Mask Set Register - EIRQ1 Mask Set
	
	// IRQ(2) -- internal irq for cpu
	*R_EIRQMOD |= F_EIRQMOD_EIRQ2LLV;		// EIRQ2(IRQ_N)
	set_interrupt(INTNUM_EIRQ2,EIRQ2_ISR);
	enable_interrupt(INTNUM_EIRQ2,TRUE);	// Interrupt Mask Set Register - EIRQ2 Mask Set
	
	// GPIO-A IRQ
	*R_P0FED = (1<<2);						// P0.2(TX_INT)
	
#if defined(__USE_IRKEY_SWITCH__) || defined(__USE_KEY_TACK_SW__)
	*R_P0FED |= (1<<3);						// P0.3(SWITCH)
#endif
#if defined(__MDIN_i540_4K60__)
	*R_P0FED |= (1<<0);						// P0.0(IRR_IN)
#endif
	
	*R_P0RED = 0;							//
	set_interrupt(INTNUM_GPIOA,GPIO0_ISR);
	enable_interrupt(INTNUM_GPIOA,TRUE);
	
	// GPIO-C IRQ
#if defined(__MDIN_i510_REF__)
	*R_P2FED = (1<<2);						// P2.2(INT_IO_EXP)
	*R_P2RED = 0;							//
	set_interrupt(INTNUM_GPIOC,GPIO2_ISR);
	enable_interrupt(INTNUM_GPIOC,TRUE);
#endif
	
	// GPIO-D IRQ -- internal gpio for cpu
#if defined(__USE_IR_LED_PWM__)
	*R_P3FED = 0;							// clear
//	*R_P3FED = (1<<0);						// P3.0(isp_vact_out_3a) - falling edge
	*R_P3RED = (1<<0);						// P3.0(isp_vact_out_3a) - rising edge
	set_interrupt(INTNUM_GPIOD,GPIO3_ISR);
	enable_interrupt(INTNUM_GPIOD,TRUE);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetTimerFunction(void)
{
	//Timer 0 : pwm out(IRIS pwm out)
	//Timer 1 : pwm out(IRIS ref pwm)
	//Timer 2 : 16ms timer service(lucida)
	//Timer 3 : IR decoder
	//Timer 4 : delay(ms)
	//Timer 5 : pwm out(user adjustable)
	
	//for 16ms timer service. -- timer 2
	set_interrupt(INTNUM_TIMER2,Timer2_Handler);
	*R_TP2CTRL = F_TPCTRL_CNTCLR_SET;
	*R_TP2CTRL = F_TPCTRL_CNTCLR_CLR;
	
#ifndef	__USE_25Hz_SENSOR__
	*R_TM2CNT = 1026 * (get_apb_clock()/8/1000) / 1000;		// 1026us
#else
	*R_TM2CNT = 1231 * (get_apb_clock()/8/1000) / 1000;		// 1231us
#endif
	
	*R_TM2CTRL = (1<<7 | F_TMCTRL_PFSEL_8);
	enable_interrupt(INTNUM_TIMER2,TRUE);			//enable Timer2
	
	
	//for IR -- timer 3 ,  Tick/sec = APB clk / pre-scale = 50.625Mhz / 32,  0.632[us]
	*R_TP3CTRL = F_TPCTRL_CNTCLR_SET;			// clear pre-scale counter & timer counter
	*R_TP3CTRL = F_TPCTRL_CNTCLR_CLR;
	*R_TM3CTRL = (1<<7 | F_TMCTRL_PFSEL_32);	// set pre-scale factor(1/32)
	
	
	//for delay(ms) -- timer 4
	usValue = 10 * (get_apb_clock()/8/1000) / 1000;		// 10us
	msValue = 1  * (get_apb_clock()/8/1000);	      	// 1ms
	
	set_interrupt(INTNUM_TIMER4,Timer4_Handler);
	*R_TP4CTRL = F_TPCTRL_CNTCLR_SET;
	*R_TP4CTRL = F_TPCTRL_CNTCLR_CLR;
	
	*R_TM4CNT = msValue;							//1ms / xxxns
	*R_TM4CTRL = (1<<7 | F_TMCTRL_PFSEL_8);
	enable_interrupt(INTNUM_TIMER4,TRUE);			//enable Timer1
	
	
	//for pwm out. -- timer 5
#if defined(__USE_IR_LED_PWM__)
	set_interrupt(INTNUM_TIMER5,PWM5_ISR);
	*R_TP5CTRL = F_TPCTRL_CNTCLR_SET;
	*R_TP5CTRL = F_TPCTRL_CNTCLR_CLR;
	
	// timer5 enable -> delay(DUT) -> pulse(width:CNT-DUT) -> timer5 irq
	*R_TM5CNT = 8  * (get_apb_clock()/8/1000);					// 8ms : pwm-total(low + high) period
	*R_TM5DUT = 5  * (get_apb_clock()/8/1000);					// 5ms : pwm-low period
	*R_TM5PUL = 1;												// pulse count(1 pulse generation -> call timer5 irq)
	
	*R_TM5CTRL = (F_TMCTRL_TMOD_PWM | F_TMCTRL_PFSEL_8);
	enable_interrupt(INTNUM_TIMER5,TRUE);
#endif
	
	
	// calc ir init value for ir speed-up(when cpu cache off)
	tick = (get_apb_clock()/32/100);					// timer counter per 10ms
	lead = 13540 * tick / 10000;						// 13540us(13.54ms)
	repeat = 11260 * tick / 10000;						// 11260us(11.26ms)
	data_0 = 2260 * tick / 10000;						// 2260us(2.26ms)
	data_1 = 1140 * tick / 10000;						// 1140us(1.14ms)
	tol_lead = lead * 7/100;
	tol_repeat = repeat * 7/100;
	tol_data_0 = data_0 * 20/100;
	tol_data_1 = data_1 * 20/100;
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetUARTFunction(void)
{
	WORD hclk, pclk;
	BYTE ch;
	
#if defined(__USE_UART0_9600__)
	uart_config(0,  9600,DATABITS_8,STOPBITS_1,UART_PARNONE);
#else
	uart_config(0,115200,DATABITS_8,STOPBITS_1,UART_PARNONE);
#endif

#if defined(__USE_USB_VOUT__)	
	uart_config(1,  115200,DATABITS_8,STOPBITS_1,UART_PARNONE);
#else	
	uart_config(1,  9600,DATABITS_8,STOPBITS_1,UART_PARNONE);
#endif	
	
	//set UART0 interrupt.. for XRC
	*R_IINTMOD  &= ~(1<<INTNUM_UART0);	//for the first empty interrupt	//for lucida
	set_interrupt ( INTNUM_UART0, UART0_ISR );
	enable_interrupt ( INTNUM_UART0, TRUE );
	uart_rx_int_enable(0, TRUE);
	
#if !defined(__USE_USB_VOUT__)
	*R_IINTMOD  &= ~(1<<INTNUM_UART1);	//for the first empty interrupt	//for lucida
	set_interrupt ( INTNUM_UART1, UART1_ISR );
	enable_interrupt ( INTNUM_UART1, TRUE );
	uart_rx_int_enable(1, TRUE);
#endif
	
	set_debug_channel(__DEBUGPRT_UART1__);

	
	hclk = get_ahb_clock()/100000;	//hclk*10
	pclk = get_apb_clock()/100000;	//pclk*10
	
	debugstring("\r\n==========================================================\r\n");
	debugprintf("CPU \t: H/PCLK(%d.%d/%d.%dMhz), UART(%d) \r\n", 
					hclk/10, hclk%10, pclk/10, pclk%10, get_debug_channel());
	debugstring("----------------------------------------------------------\r\n");
	
	ch = *(VPBYTE)((U32)R_UART_BASE(0) + UART_RBR);		// clear pop-up garbage when uart rx open
	ch = *(VPBYTE)((U32)R_UART_BASE(1) + UART_RBR);		// clear pop-up garbage when uart rx open
	
	
	// uart-2
	uart2_config(115200, DATABITS_8, STOPBITS_1, UART_PARNONE);	// buffer(8byte)
	Delay_uSec(1000);	//min delay 30us
	
	*R_IINTMOD  &= ~(1<<INTNUM_UINT1);	//for the first empty interrupt	//for uart-2
	set_interrupt ( INTNUM_UINT1, UART2_ISR );
	enable_interrupt ( INTNUM_UINT1, TRUE );
	uart2_rx_int_enable(TRUE);
	uart2_status_int_enable(TRUE);
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetI2CFunction(void)
{
	DWORD	i2c_freq;
	
//	i2c_freq = 100000;			//100Khz
	i2c_freq = 400000;			//400Khz
	
#if defined(__MDIN_i5XX_FPGA__)				// FPGA board
	twi_set_freq (TWI_CH, i2c_freq);		//TWI_CH defined Lucida/include/macroimg/twi.h !!!
#else										// EDK & REF board
	twi_set_freq (TWI_CH0, i2c_freq);		//i2c0 -- for sensor
	twi_set_freq (TWI_CH1, i2c_freq);		//i2c1 -- for peri
#endif	// __MDIN_i5XX_FPGA__
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetSSIFunction(void)
{
#if defined(__MDIN_i5XX_FPGA__)					// FPGA board
	spi_master_init(SPI_CH);
	spi_set_freq(SPI_CH, SPI_MASTER, 4000000);	//freq = 4Mhz
#else											// EDK & REF board
	spi_master_init(SPI_CH0);					//spi0 -- for sensor
	spi_set_freq(SPI_CH0, SPI_MASTER, 8000000);	//freq = 8Mhz
	
#if defined(__USE_CPU_SPI_SLAVE__)
	// Slave Init								// spi1 -- for spi-slave i/f with external mcu
	*R_SPI0CON(SPI_CH1) = SPICON_CPOL | SPICON_CPHA | SPICON_EN | SPICON_SPISIZE32B;
	*R_SPI0INT(SPI_CH1) = SIMK_SPIFE;			// spi interrupt enable
	*R_SPI0DATA(SPI_CH1) = SPI_ACK_DUMMY;		// set tx dummy data for 1st rx irq
	
	set_interrupt(INTNUM_SPI1,SPI1_ISR);		// set spi slave irq
	enable_interrupt(INTNUM_SPI1,TRUE);
	
	SPIs_Initialize();
#else
	// Master init
	spi_master_init(SPI_CH1);					//spi1 -- for peri(iris/lens ctrl ic,,)
	spi_set_freq(SPI_CH1, SPI_MASTER, 4000000);	//freq = 4Mhz
	
	SPIm_Initialize();
#endif

#endif	// __MDIN_i5XX_FPGA__
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetPMUInit(void)
{
//1. change 'Static Library Files(libLucida_xxx.a)'
//2. modify 'cpu_hclk_sel & pre/post_div_mclk' in 'init.c(SetPMUInit)'
	WORD	i;
	BYTE temp[10];		//181226 mass

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
#if !defined(__USE_MDIN_i5XX_REV__)
	CacheDisable();				// cpu cache disable
#endif
#endif
	
#if	defined(__MDIN_i5XX_FPGA__)	// FPGA board
	*R_DDR_DRV_ST_SEL = 0x0002;	//for FPGA, 없으면 hclk 150khz 나옴
#endif
	
	*R_CPU_HCLK_SEL= F_CPUHCLKSEL_INIT;
	
#ifndef	SYSTEM_USE_OSC36M

#ifdef	__USE_DDR_MCLK162__
	*R_PRE_DIV_MCLK = 0x0002;	// P(2)	
	*R_POST_DIV_MCLK = 0x0018;	// M(24), S(0)
#endif	

#ifdef	__USE_DDR_MCLK121__
	*R_PRE_DIV_MCLK = 0x0002;	// P(2)	
	*R_POST_DIV_MCLK = 0x0012;	// M(18), S(0)
#endif	

#ifdef	__USE_DDR_MCLK108__
	*R_PRE_DIV_MCLK = 0x0002;	// P(2)	
	*R_POST_DIV_MCLK = 0x0010;	// M(16), S(0)
#endif

#ifdef	__USE_DDR_MCLK99__
	*R_PRE_DIV_MCLK = 0x0003;	// P(3)
	*R_POST_DIV_MCLK = 0x0016;	// M(22), S(0)
#endif

#ifdef	__USE_DDR_MCLK180__
	*R_PRE_DIV_MCLK = 0x0003;	// P(3)
	*R_POST_DIV_MCLK = 0x0028;	// M(40), S(0)
#endif

#ifdef	__USE_DDR_MCLK198__
	*R_PRE_DIV_MCLK = 0x0003;	// P(3)
	*R_POST_DIV_MCLK = 0x002C;	// M(44), S(0)
#endif
	
#ifdef	__USE_DDR_MCLK202__
	*R_PRE_DIV_MCLK = 0x0001;	// P(1)
	*R_POST_DIV_MCLK = 0x000F;	// M(15), S(0)
#endif	

#ifdef	__USE_DDR_MCLK324__		// cpu library(81M)
	*R_PRE_DIV_MCLK = 0x0001;	// S(2^0), P(1)
	*R_POST_DIV_MCLK = 0x0018;	// M(24) -- 324Mhz
#endif

#ifdef	__USE_DDR_MCLK339__		// cpu library(84.75M)
	*R_PRE_DIV_MCLK = 0x0009;	// S(2^0), P(9)
	*R_POST_DIV_MCLK = 0x00E2;	// M(226) -- 339Mhz
#endif
	
#ifdef	__USE_DDR_MCLK360__		// cpu library(90M)
	*R_PRE_DIV_MCLK = 0x0009;	// S(2^0), P(9)
	*R_POST_DIV_MCLK = 0x00F0;	// M(240) -- 360Mhz
#endif	
	
#ifdef	__USE_DDR_MCLK367__		// cpu library(91M875)
	*R_PRE_DIV_MCLK = 0x0009;	// S(2^0), P(9)
	*R_POST_DIV_MCLK = 0x00F5;	// M(245) -- 367.5Mhz
#endif

#ifdef	__USE_DDR_MCLK375__		// cpu library(93M75)
	*R_PRE_DIV_MCLK = 0x0009;	// S(2^0), P(9)
	*R_POST_DIV_MCLK = 0x00FA;	// M(250) -- 375Mhz
#endif

#ifdef	__USE_DDR_MCLK383__		// cpu library(95M625)
	*R_PRE_DIV_MCLK = 0x0013;	// S(2^0), P(3)
	*R_POST_DIV_MCLK = 0x00AA;	// M(85) -- 382.5Mhz
#endif	
	
#ifdef	__USE_DDR_MCLK396__		// cpu library(99M) 
	*R_PRE_DIV_MCLK = 0x0013;	// S(2^1), P(3)
	*R_POST_DIV_MCLK = 0x00B0;	// M(176) -- 396Mhz
#endif

#ifdef	__USE_DDR_MCLK405__		// cpu library(101.25M)
	*R_PRE_DIV_MCLK = 0x0013;	// S(2^1), P(3)
	*R_POST_DIV_MCLK = 0x00B4;	// M(180) -- 405Mhz
#endif	
	
	
	
#else	//SYSTEM_USE_OSC36M
#ifdef	__USE_DDR_MCLK108__
	*R_PRE_DIV_MCLK = 0x0004;	// P(4)
	*R_POST_DIV_MCLK = 0x0018;	// M(24), S(0)
#endif

#ifdef	__USE_DDR_MCLK99__
	*R_PRE_DIV_MCLK = 0x0004;	// P(4)
	*R_POST_DIV_MCLK = 0x0016;	// M(22), S(0)
#endif

#ifdef	__USE_DDR_MCLK198__
	*R_PRE_DIV_MCLK = 0x0004;	// P(4)
	*R_POST_DIV_MCLK = 0x002C;	// M(44), S(0)
#endif
	
#ifdef	__USE_DDR_MCLK202__
	*R_PRE_DIV_MCLK = 0x0004;	// P(4)
	*R_POST_DIV_MCLK = 0x002D;	// M(45), S(0)
#endif		
	
#ifdef	__USE_DDR_MCLK324__		// cpu library(81M)
	*R_PRE_DIV_MCLK = 0x0001;	// S(2^0), P(1)
	*R_POST_DIV_MCLK = 0x0012;	// M(18) -- 324Mhz
#endif

#ifdef	__USE_DDR_MCLK339__		// cpu library(84.75M)
	*R_PRE_DIV_MCLK = 0x0006;	// S(2^0), P(6)
	*R_POST_DIV_MCLK = 0x0071;	// M(113) -- 339Mhz
#endif
	
#ifdef	__USE_DDR_MCLK360__		// cpu library(90M)
	*R_PRE_DIV_MCLK = 0x0006;	// S(2^0), P(6)
	*R_POST_DIV_MCLK = 0x0078;	// M(120) -- 360Mhz
#endif	
	
#ifdef	__USE_DDR_MCLK375__		// cpu library(93M75)
	*R_PRE_DIV_MCLK = 0x0006;	// S(2^0), P(6)
	*R_POST_DIV_MCLK = 0x007D;	// M(125) -- 375Mhz
#endif

#ifdef	__USE_DDR_MCLK396__		// cpu library(99M) -- vertical noise
	*R_PRE_DIV_MCLK = 0x0001;	// S(2^0), P(1)
	*R_POST_DIV_MCLK = 0x0016;	// M(22) -- 396Mhz
#endif
	
#ifdef	__USE_DDR_MCLK405__		// cpu library(101.25M) -- vertical noise
	*R_PRE_DIV_MCLK = 0x0011;	// S(2^1), P(1)
	*R_POST_DIV_MCLK = 0x002D;	// M(45) -- 405Mhz
#endif	

	
#endif	//SYSTEM_USE_OSC36M
	
	*R_PLL_DIS = F_PLL_DIS_MCLK_EN;
//	for(i=0;i<100;i++);		//delay(140us, >20us)
	for(i=0;i<100;i++)    sf_read(0, &temp[0], sizeof(temp));		//181226 mass
	
#ifdef	__USE_CPU_CLK_XTAL__
	*R_CPU_HCLK_SEL= F_CPUHCLKSEL_XTAL1;
	//*R_CPU_HCLK_SEL= F_CPUHCLKSEL_HOSTCLK;
#else
#if defined(__MDIN_i5XX_FPGA__)			// FPGA board
  #if defined(__USE_DDR_MCLK162__)||\
	  defined(__USE_DDR_MCLK198__)||\
	  defined(__USE_DDR_MCLK180__)
	*R_CPU_HCLK_SEL |= F_CPUHCLK_DIV2;
  #else
	*R_CPU_HCLK_SEL |= F_CPUHCLK_DIV1;
  #endif
	*R_CPU_HCLK_SEL |= F_CPUHCLKSEL_MCLK;
#else									// EDK & REF board
  #if defined(__USE_DDR_MCLK324__)||\
	  defined(__USE_DDR_MCLK339__)||\
	  defined(__USE_DDR_MCLK360__)||\
	  defined(__USE_DDR_MCLK367__)||\
	  defined(__USE_DDR_MCLK375__)||\
	  defined(__USE_DDR_MCLK383__)||\
	  defined(__USE_DDR_MCLK396__)||\
	  defined(__USE_DDR_MCLK405__)
	*R_CPU_HCLK_SEL |= F_CPUHCLK_DIV4;
//	*R_CPU_HCLK_SEL= F_CPUHCLKSEL_MCLK | F_CPUHCLK_DIV4;		//old
  #else
	*R_CPU_HCLK_SEL |= F_CPUHCLK_DIV2;
  #endif
	*R_CPU_HCLK_SEL |= F_CPUHCLKSEL_MCLK;
#endif	// __MDIN_i5XX_FPGA__
#endif	// __USE_CPU_CLK_XTAL__
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetADCFunction(void)
{
	//register ADC interrupt
#if defined(__USE_SMIC_ADC__)
	*R_IINTMOD |= (1<<INTNUM_UINT0);  //rising edge
	set_interrupt(INTNUM_UINT0,ADC_ISR);
	enable_interrupt(INTNUM_UINT0,TRUE);
#else
	*R_IINTMOD |= (1<<INTNUM_ADC);  //rising edge
	set_interrupt(INTNUM_ADC,ADC_ISR);
	enable_interrupt(INTNUM_ADC,TRUE);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetISPHwReset(void)
{
	*R_HARD_RESET = 0x0001;		// isp h/w reset
	Delay_uSec(1000);			// delay 15ms(@27Mhz)
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetWDTFunction(void)
{
#if defined(__USE_CPU_WDT_RST__)
	if(*R_WDTCTRL & F_WDTCTRL_WDTST)	debugstring("\r\n WDT Reset Occured !!!!!!\r\n");
	
	*R_WDTCNT = (get_apb_clock()*60);							// set wdt counter for boot : 60sec
	*R_WDTCTRL = F_WDTCTRL_WDTMOD_RST | F_WDTCTRL_WDTEN_EN;		// wdt enable
	
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	*R_ISP_RESET_BY_WDT = (F_SW_RESET_EN | F_HW_RESET_EN);		// isp h/w & s/w reset by wdt enable
#endif
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void ISPM ResetWDTcounter(void)
{
	*R_WDTCNT = (get_apb_clock()*1);	// 1sec
}

//--------------------------------------------------------------------------------------------------------------------------
static void SetIOExpFunction(void)
{
#if	defined(__MDIN_i510_REF__)			// i510 REF board
	// port config(addr:0x03,  '0':out, '1':in)
	IOExp_Write(0, 0x03, 0x00);			// U4 : P0~7(out)
	IOExp_Write(1, 0x03, 0x0C);			// U6 : P0,1,4~7(out), P2,3(in)
	
	// port init
	SDI_RATE(HIGH);												// LOW(1.5G), HIGH(3G)
	SDI_DIV_1001(LOW);	SDI_DET_TRS(HIGH);	SDI_20B_10B(HIGH);	// 59.94(x), vout_sync_emb, vout_20b
	SDI_RSTN(LOW);	Delay_uSec(10000);	SDI_RSTN(HIGH);			// sdi-tx(gv7700) reset to setup '3G <-> 1.5G'
	
	OLPF_DY(HIGH);	OLPF_NT(HIGH);					// set OLPF
	DE_485(LOW); 									// set RS485 direction to rx(default)
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void uComOnChipInitial(void)
{
	INTERRUPT_DISABLE;		// interrupt disable
	SetISPHwReset();		// isp h/w reset
	SetPMUInit();			// set cpu hclk	
	SetPortDirection();		// set port direction
	SetExtIrqFunction();	// set external interrupt
	SetTimerFunction();		// set timer operation
	SetUARTFunction();		// set uart operation
	SetI2CFunction();		// set i2c operation
	SetSSIFunction();		// set ssi operation
	SetADCFunction();		// set adc operation
	SetWDTFunction();		// set watchdog timer
	SetIOExpFunction();		// set i/o expander
	INTERRUPT_ENABLE;		// interrupt enable
	
#if	!defined(__MDIN_i5XX_FPGA__)	// EDK & REF board
	SENSOR_RSTN(HIGH);
#endif

	SetSYSINFO_CpuInitDone();
}

/*  FILE_END_HERE */
