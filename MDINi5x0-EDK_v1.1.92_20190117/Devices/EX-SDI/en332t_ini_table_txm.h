// ==================================================================================
// Master mode
// VD(16bit) to HD-SDI(1.485Gbps)
// ==================================================================================
#define TXM_HDSDI_Size	31
const UINT TXM_HDSDI[TXM_HDSDI_Size][2]  =
{
	//Address	, Data
	{ 0x000000c5, 0x00000000 },	// RPCK_A Disable
	{ 0x000000c8, 0x40004000 },	// VD_A Disable
	{ 0x000000ca, 0x00000000 },	// EXD_A, EXM_A Disable

	{ 0x00000031, 0x0000f705 },	// TSDI_RATE select
	{ 0x00000041, 0x00003800 },	// In-buffer out data select
	{ 0x0000004a, 0x00800050 },	// Internal data MUX selection
	{ 0x00000065, 0x00400080 }, // BT_DI_POL[17] = 0 : sampled falling edge
//	{ 0x00000065, 0x00420080 }, // BT_DI_POL[17] = 1 : sampled rising edge
	{ 0x00000066, 0x0000811e }, // VDI_REON
	{ 0x00000094, 0x000010b0 },	// OBUF Rate select
	{ 0x000000c2, 0x00000000 },	// TXFCK clock select
	{ 0x000000c3, 0x0000000a },	// TXREFCK clock select
	{ 0x000000c5, 0x80000040 },	// Internal clock selection
	{ 0x000000c8, 0x40044004 },	// PCK_A, Internal clock selection
	{ 0x000000ca, 0x00000000 },	// EXSDI Encoder clock select
	{ 0x000000d1, 0x00004800 },	// VDI_DI_SEL[11] = 1 : VD_A(Y), VD_B(C)
//	{ 0x000000d1, 0x00004000 },	// VDI_DI_SEL[11] = 0 : VD_A(C), VD_B(Y)
	{ 0x000000d2, 0x00003004 },	// Internal data MUX selection
	{ 0x000000d3, 0x00000400 },	// EX_ENC Data input select
	{ 0x000000d9, 0x00000003 },	// EXENC Codec mode
	                                                                         
	{ 0x00000112, 0x00004c00 },	// EXSDI Version
	{ 0x0000012a, 0x8607080b },	// EX-SDI Voffset
	{ 0x0000012b, 0x0d07080a },	// EX-SDI Voffset
	                                                                         
	{ 0x000000c5, 0x800000c0 },	// RPCK_A enable
	{ 0x000000ca, 0x00000000 },	// EXD_A, EXM_A Disable
	                                                                         
	{ 0x000000c6, 0x00000000 },	// Encoder Disable
	{ 0x000000c6, 0x00000005 },	// Encoder clock select
	{ 0x000000c6, 0x00000015 },	// Encoder Enable
	                                                                         
	{ 0x00000033, 0x0002191f },	// TSDI_SR_EN
	{ 0x00000091, 0x00024801 },	// TSDI_DO_OFF
	{ 0x00000098, 0x00004008 },	// ENC_ANC_ON[24] off <-- Ancillary data off to avoid Illegal Code error
                                                                            
//	{ 0x000000c8, 0xc0044004 }, // PCK A enable
//	{ 0x000000de, 0x00017fff }	// PCK A Direction
	{ 0x000000c8, 0x4004c004 }, // PCK B enable
//	{ 0x000000de, 0x0001dfff }	// PCK B Direction
	{ 0x000000de, 0x0000dff9 }	// PCK B Direction(ucc enable)

};

// ==================================================================================
// Master mode
// VD(16bit) to EX-SDI 1.0(270Mbps)
// ==================================================================================
#define TXM_EX270_HD_Size	38
const UINT TXM_EX270_HD[TXM_EX270_HD_Size][2] =
{
	{ 0x000000c5, 0x00000000 },	// RPCK_A Disable
	{ 0x000000c8, 0x40004000 },	// VD_A Disable
	{ 0x000000ca, 0x00000000 },	// EXD_A, EXM_A Disable

	{ 0x00000031, 0x0000f705 },	// TSDI_RATE select
	{ 0x00000041, 0x00003800 },	// In-buffer out data select
	{ 0x0000004a, 0x00800050 },	// Internal data MUX selection
	{ 0x00000065, 0x00400080 }, // BT_DI_POL[17] = 0 : sampled falling edge
//	{ 0x00000065, 0x00420080 }, // BT_DI_POL[17] = 1 : sampled rising edge
	{ 0x00000066, 0x0000811e },	// VDI_REON
	{ 0x00000094, 0x000011b0 },	// OBUF Rate select
	{ 0x000000c2, 0x00000044 },	// TXFCK clock select
	{ 0x000000c3, 0x0000000a },	// TXREFCK clock select
	{ 0x000000c5, 0x80000040 },	// Internal clock selection
	{ 0x000000c8, 0x40044004 },	// PCK_A, Internal clock selection
	{ 0x000000ca, 0x03500039 },	// EXSDI Encoder clock select
	{ 0x000000d1, 0x00004800 },	// VDI_DI_SEL[11] = 1 : VD_A(Y), VD_B(C)
//	{ 0x000000d1, 0x00004000 },	// VDI_DI_SEL[11] = 0 : VD_A(C), VD_B(Y)
	{ 0x000000d2, 0x00003005 },	// Internal data MUX selection
	{ 0x000000d3, 0x00000400 },	// EX_ENC Data input select
	{ 0x000000d9, 0x00000001 },	// EXENC Codec mode

	{ 0x00000112, 0x10004c00 },	// EXSDI Version
	{ 0x00000113, 0x02018100 },
	{ 0x00000114, 0x02000080 },
	{ 0x00000119, 0x00000000 },
	{ 0x00000125, 0x01ae0190 },
	{ 0x00000126, 0x8831f201 },
	{ 0x00000127, 0x81a60d80 },
	{ 0x00000128, 0x00000000 },
	{ 0x0000011d, 0x00000000 },	// EX Split off
	{ 0x0000012a, 0x8607080b },	// EX-SDI Voffset
	{ 0x0000012b, 0x0d07080a },	// EX-SDI Voffset

	{ 0x000000c5, 0x800000c0 },	// RPCK_A enable
	{ 0x000000ca, 0x13500139 },	// EXD_A, EXM_A enable

	{ 0x000000c6, 0x00000000 },	// Encoder Disable
	{ 0x000000c6, 0x0000000f },	// Encoder clock select
	{ 0x000000c6, 0x0000001f },	// Encoder Enable 

	{ 0x00000033, 0x00021904 },	// TSDI_SR_EN 
	{ 0x00000091, 0x00024809 },	// TSDI_DO_OFF

//	{ 0x000000c8, 0xc0044004 }, // PCK A enable
//	{ 0x000000de, 0x00017fff }	// PCK A Direction
	{ 0x000000c8, 0x4004c004 }, // PCK B enable
//	{ 0x000000de, 0x0001dfff }	// PCK B Direction
	{ 0x000000de, 0x0000dff9 }	// PCK B Direction(ucc enable)

};

// ==================================================================================
// Master mode
// VD(16bit) to EX-SDI 2.0(135Mbps)
// ==================================================================================
#define TXM_EX135_HD_Size	38
const UINT TXM_EX135_HD[TXM_EX135_HD_Size][2] =
{
	{ 0x000000c5, 0x00000000 },	// RPCK_A Disable
	{ 0x000000c8, 0x40004000 },	// VD_A Disable
	{ 0x000000ca, 0x00000000 },	// EXD_A, EXM_A Disable
	                          
	{ 0x00000031, 0x0000f707 },	// TSDI_RATE select
	{ 0x00000041, 0x00003800 },	// In-buffer out data select
	{ 0x0000004a, 0x00800050 },	// Internal data MUX selection
	{ 0x00000065, 0x00400080 }, // BT_DI_POL[17] = 0 : sampled falling edge
//	{ 0x00000065, 0x00420080 }, // BT_DI_POL[17] = 1 : sampled rising edge
	{ 0x00000066, 0x0000811e }, // VDI_REON
	{ 0x00000094, 0x00001388 },	// OBUF Rate select
	{ 0x000000c2, 0x00000054 },	// TXFCK clock select
	{ 0x000000c3, 0x0000000a },	// TXREFCK clock select
	{ 0x000000c5, 0x80000040 },	// Internal clock selection
	{ 0x000000c8, 0x40044004 },	// PCK_A, Internal clock selection
	{ 0x000000ca, 0x03500039 },	// EXSDI Encoder clock select
	{ 0x000000d1, 0x00004800 },	// VDI_DI_SEL[11] = 1 : VD_A(Y), VD_B(C)
//	{ 0x000000d1, 0x00004000 },	// VDI_DI_SEL[11] = 0 : VD_A(C), VD_B(Y)
	{ 0x000000d2, 0x00003005 },	// Internal data MUX selection
	{ 0x000000d3, 0x00000400 },	// EX_ENC Data input select
	{ 0x000000d9, 0x00000001 },	// EXENC Codec mode
	                          
	{ 0x00000112, 0x20804c00 },	// EXSDI Version
	{ 0x00000113, 0x60018800 },
	{ 0x00000114, 0x02000080 },
	{ 0x00000119, 0x00000001 },
	{ 0x00000125, 0x00c800b4 },
	{ 0x00000126, 0xe019f201 },
	{ 0x00000127, 0x81a61d80 },
	{ 0x00000128, 0x00000000 },
	{ 0x0000011d, 0x00000000 },	// EX Split off
	{ 0x0000012a, 0x8607080b },	// EX-SDI Voffset
	{ 0x0000012b, 0x0d07080a },	// EX-SDI Voffset
	                          
	{ 0x000000c5, 0x800000c0 },	// RPCK_A enable
	{ 0x000000ca, 0x13500139 },	// EXD_A, EXM_A enable
	                          
	{ 0x000000c6, 0x00000000 },	// Encoder Disable
	{ 0x000000c6, 0x0000000f },	// Encoder clock select
	{ 0x000000c6, 0x0000001f },	// Encoder Enable 
	                          
	{ 0x00000033, 0x00021902 },	// TSDI_SR_EN 
	{ 0x00000091, 0x00024809 },	// TSDI_DO_OFF
	                        
//	{ 0x000000c8, 0xc0044004 },	// PCK A enable
//	{ 0x000000de, 0x00017fff }	// PCK A Direction
	{ 0x000000c8, 0x4004c004 },	// PCK B enable
//	{ 0x000000de, 0x0001dfff }	// PCK B Direction
	{ 0x000000de, 0x0000dff9 }	// PCK B Direction(ucc enable)
	
};

// ==================================================================================
// Master mode
// VD(16bit) to EX-SDI 4M(270Mbps)
// ==================================================================================
#define TXM_EX270_4M_Size	38
const UINT TXM_EX270_4M[TXM_EX270_4M_Size][2] =
{
	{ 0x000000c5, 0x00000000 },	// RPCK_A Disable
	{ 0x000000c8, 0x40004000 },	// VD_A Disable
	{ 0x000000ca, 0x00000000 },	// EXD_A, EXM_A Disable
	                          
	{ 0x00000031, 0x0000F705 },	// TSDI_RATE select
	{ 0x00000041, 0x00003800 },	// In-buffer out data select
	{ 0x0000004a, 0x00800050 },	// Internal data MUX selection
	{ 0x00000065, 0x00400080 }, // BT_DI_POL[17] = 0 : sampled falling edge
//	{ 0x00000065, 0x00420080 }, // BT_DI_POL[17] = 1 : sampled rising edge
	{ 0x00000066, 0x0000811E }, // VDI_REON
	{ 0x00000094, 0x00001188 },	// OBUF Rate select
	{ 0x000000c2, 0x00000043 },	// TXFCK clock select
	{ 0x000000c3, 0x0000000A },	// TXREFCK clock select
	{ 0x000000c5, 0x90000030 },	// Internal clock selection
	{ 0x000000c8, 0x40034003 },	// PCK_A, Internal clock selection
	{ 0x000000ca, 0x02500029 },	// EXSDI Encoder clock select
	{ 0x000000d1, 0x00004800 },	// VDI_DI_SEL[11] = 1 : VD_A(Y), VD_B(C)
//	{ 0x000000d1, 0x00004000 },	// VDI_DI_SEL[11] = 0 : VD_A(C), VD_B(Y)
	{ 0x000000d2, 0x00003005 },	// Internal data MUX selection
	{ 0x000000d3, 0x00000400 },	// EX_ENC Data input select
	{ 0x000000d9, 0x00000001 },	// EXENC Codec mode
	                          
	{ 0x00000112, 0x20804C00 },	// EXSDI Version
	{ 0x00000113, 0x60018800 },
	{ 0x00000114, 0x02000080 },
	{ 0x00000119, 0x00000001 },
	{ 0x00000125, 0x00C800B4 },
	{ 0x00000126, 0xC019F201 },
	{ 0x00000127, 0x81A61FC0 },
	{ 0x00000128, 0xC1000000 },
	{ 0x0000011d, 0x00000000 },	// EX Split off
	{ 0x0000012a, 0x8607080B },	// EX-SDI Voffset
	{ 0x0000012b, 0x0D07080A },	// EX-SDI Voffset
	                          
	{ 0x000000c5, 0x900000B0 },	// RPCK_A enable
	{ 0x000000ca, 0x12500129 },	// EXD_A, EXM_A enable
	                          
	{ 0x000000c6, 0x00000000 },	// Encoder Disable
	{ 0x000000c6, 0x0000000F },	// Encoder clock select
	{ 0x000000c6, 0x0000001F },	// Encoder Enable 
	                          
	{ 0x00000033, 0x00021904 },	// TSDI_SR_EN 
	{ 0x00000091, 0x00024809 },	// TSDI_DO_OFF
	                        
//	{ 0x000000c8, 0xc0044004 }, // PCK A enable
//	{ 0x000000de, 0x00017fff }	// PCK A Direction
	{ 0x000000c8, 0x4003C003 }, // PCK B enable
//	{ 0x000000de, 0x0001dfff }	// PCK B Direction
	{ 0x000000de, 0x0000dff9 }	// PCK B Direction(ucc enable)

};

// ==================================================================================
// Master mode
// VD(16bit) to EX-SDI 4K(1.485Gbps)
// ==================================================================================
#define TXM_EX1G5_4K_Size	52
const UINT TXM_EX1G5_4K[TXM_EX1G5_4K_Size][2] =
{
	{ 0x000000c5, 0x00000000 },	// RPCK_A Disable
	{ 0x000000c8, 0x40004000 },	// VD_A Disable
	{ 0x000000ca, 0x00000000 },	// EXD_A, EXM_A Disable
	{ 0x000000cb, 0x00000000 },	// EXD_B, EXM_B Disable       //************* only 4K
	                           
	{ 0x00000031, 0x0000f705 },	// TSDI_RATE select
	{ 0x00000041, 0x00003800 },	// In-buffer out data select
	{ 0x0000004a, 0x00800050 },	// Internal data MUX selection
	{ 0x00000065, 0x00400000 }, // BT_DI_POL[17] = 0 : sampled falling edge
//	{ 0x00000065, 0x00420000 }, // BT_DI_POL[17] = 1 : sampled rising edge
	{ 0x00000066, 0x0000811e }, // VDI_REON
	{ 0x00000094, 0x000010b0 },	// OBUF Rate select
	{ 0x000000c2, 0x00000000 },	// TXFCK clock select
	{ 0x000000c3, 0x0000000a },	// TXREFCK clock select
	{ 0x000000c5, 0xa0000020 },	// Internal clock selection
	{ 0x000000c8, 0x40024002 },	// PCK_A, Internal clock selection
	{ 0x000000ca, 0x0a500029 },	// EXSDI Encoder clock select
	{ 0x000000cb, 0x0025002d },	// EXSDI Encoder clock select //************* only 4K
	{ 0x000000d1, 0x00004800 },	// VDI_DI_SEL[11] = 1 : VD_A(Y), VD_B(C)
//	{ 0x000000d1, 0x00004000 },	// VDI_DI_SEL[11] = 0 : VD_A(C), VD_B(Y)
	{ 0x000000d2, 0x00003005 },	// Internal data MUX selection
	{ 0x000000d3, 0x00000400 },	// EX_ENC Data input select
	{ 0x000000d9, 0x00000000 },	// EXENC Codec mode
                             
	{ 0x000000e4, 0x01043060 },	// Test Pattern Generation ON
	                         
	{ 0x000000e1, 0x0000112f },	// Horizontal total
	{ 0x000000e2, 0x08c90f00 },	// Vertical total, Horizontal active
	{ 0x000000e3, 0x0870005a },	// Vertical active, Vertical start point
	                           
	{ 0x00000112, 0x20024c00 },	// EXSDI Version
	{ 0x00000113, 0x02018080 },	// EXSDI Setting for 4K EX-SDI
	{ 0x00000114, 0x82000100 },	// EXSDI Setting for 4K EX-SDI
	{ 0x00000119, 0x00000000 },	// EXSDI Setting for 4K EX-SDI
	{ 0x00000125, 0x00d000c0 },	// EXSDI Setting for 4K EX-SDI          
	{ 0x00000126, 0xf215f201 },	// EXSDI Setting for 4K EX-SDI          
	{ 0x00000127, 0x81a62580 },	// EXSDI Setting for 4K EX-SDI          
	{ 0x00000128, 0x00000000 },	// EXSDI Setting for 4K EX-SDI     
	{ 0x0000011d, 0x90002980 },	// EX Split on				  //************* only 4K
	{ 0x00000131, 0x40004000 },	// Split ID					  //************* only 4K
	{ 0x00000132, 0x20024c00 },	// EXSDI Version			  //************* only 4K
	{ 0x00000133, 0x02018080 },	// EXSDI Setting for 4K EX-SDI
	{ 0x00000134, 0x82000100 },	// EXSDI Setting for 4K EX-SDI
	{ 0x00000139, 0x00000000 },	// EXSDI Setting for 4K EX-SDI
	{ 0x00000145, 0x00d000c0 },	// EXSDI Setting for 4K EX-SDI          
	{ 0x00000146, 0xf215f201 },	// EXSDI Setting for 4K EX-SDI          
	{ 0x00000147, 0x81a62580 },	// EXSDI Setting for 4K EX-SDI          
	{ 0x00000148, 0x00000000 },	// EXSDI Setting for 4K EX-SDI
	                           
	{ 0x000000c5, 0xa00000a0 },	// RPCK_A enable
	{ 0x000000ca, 0x1a500129 },	// EXD_A, EXM_A enable
	{ 0x000000cb, 0x0125012d },	// EXD_B, EXM_B Disable       //************* only 4K
	                           
	{ 0x000000c6, 0x00000000 },	// Encoder Disable
	{ 0x000000c6, 0x0000000f },	// Encoder clock select
	{ 0x000000c6, 0x0000001f },	// Encoder Enable 
	                           
	{ 0x00000033, 0x0002191f },	// TSDI_SR_EN 
	{ 0x00000091, 0x00024809 },	// TSDI_DO_OFF
	                         
//	{ 0x000000c8, 0x80020002 }, // PCK A enable
//	{ 0x000000de, 0x00017fff }	// PCK A Direction
	{ 0x000000c8, 0x40028002 }, // PCK B enable
//	{ 0x000000de, 0x0001dfff }	// PCK B Direction
	{ 0x000000de, 0x0000dff9 }	// PCK B Direction(ucc enable)

};

// ==================================================================================
// Master mode
// VD(16bit) to 3G-SDI(2.97Gbps)
// ==================================================================================
#define TXM_3GSDI_Size	30
const UINT TXM_3GSDI[TXM_3GSDI_Size][2]  =
{
	//Address	, Data
	{ 0x000000c5, 0x00000000 },	// RPCK_A Disable
	{ 0x000000c8, 0x40004000 },	// VD_A Disable
	{ 0x000000ca, 0x00000000 },	// EXD_A, EXM_A Disable

	{ 0x00000031, 0x0000f705 },	// TSDI_RATE select
	{ 0x00000041, 0x00003800 },	// In-buffer out data select
	{ 0x0000004a, 0x00800050 },	// Internal data MUX selection
	{ 0x00000065, 0x00400080 }, // BT_DI_POL[17] = 0 : sampled falling edge
//	{ 0x00000065, 0x00420080 }, // BT_DI_POL[17] = 1 : sampled rising edge
	{ 0x00000066, 0x0000811e }, // VDI_REON
	{ 0x00000094, 0x000012b0 },	// OBUF Rate select
	{ 0x000000c2, 0x00000000 },	// TXFCK clock select
	{ 0x000000c3, 0x0000000a },	// TXREFCK clock select
	{ 0x000000c5, 0x90000040 },	// Internal clock selection
	{ 0x000000c8, 0x40034003 },	// PCK_A, Internal clock selection
	{ 0x000000ca, 0x00000000 },	// EXSDI Encoder clock select
	{ 0x000000d1, 0x00004800 },	// VDI_DI_SEL[11] = 1 : VD_A(Y), VD_B(C)
//	{ 0x000000d1, 0x00004000 },	// VDI_DI_SEL[11] = 0 : VD_A(C), VD_B(Y)
	{ 0x000000d2, 0x00003004 },	// Internal data MUX selection
	{ 0x000000d3, 0x00000400 },	// EX_ENC Data input select
	{ 0x000000d9, 0x00000003 },	// EXENC Codec mode

	{ 0x00000112, 0x00004c00 },	// EXSDI Version
	{ 0x0000012a, 0x8607080b },	// EX-SDI Voffset
	{ 0x0000012b, 0x0d07080a },	// EX-SDI Voffset

	{ 0x000000c5, 0x900000c0 },	// RPCK_A enable
	{ 0x000000ca, 0x00000000 },	// EXD_A, EXM_A Disable

	{ 0x000000c6, 0x00000000 },	// Encoder Disable
	{ 0x000000c6, 0x00000004 },	// Encoder clock select
	{ 0x000000c6, 0x00000014 },	// Encoder Enable 

	{ 0x00000033, 0x0002191f },	// TSDI_SR_EN 
	{ 0x00000091, 0x00024801 },	// TSDI_DO_OFF

//	{ 0x000000c8, 0xc0034003 }, // PCK A enable
//	{ 0x000000de, 0x00017fff }	// PCK A Direction
	{ 0x000000c8, 0x4003c003 }, // PCK B enable
//	{ 0x000000de, 0x0001dfff }	// PCK B Direction
	{ 0x000000de, 0x0000dff9 }	// PCK B Direction(ucc enable)

};
