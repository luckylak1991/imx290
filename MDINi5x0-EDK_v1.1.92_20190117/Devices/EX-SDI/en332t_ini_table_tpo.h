// ==================================================================================
// No input (EX-SDI 1.0 (270Mbps) test pattern out)
// ==================================================================================
#define TP_EX270_HD_Size	38
const UINT TP_EX270_HD[TP_EX270_HD_Size][2] =
{
	{ 0x000000c5, 0x00000000 },	// RPCK_A Disable
	{ 0x000000c8, 0x40004000 },	// VD_A Disable
	{ 0x000000ca, 0x00000000 },	// EXD_A, EXM_A Disable	                                      
	{ 0x00000031, 0x0000f705 },	// TSDI_RATE select
	{ 0x00000041, 0x00003800 },	// In-buffer out data select
	{ 0x0000004a, 0x00800050 },	// Internal data MUX selection
	{ 0x00000094, 0x000011b0 },	// OBUF Rate select
	{ 0x000000c2, 0x00000044 },	// TXFCK clock select
	{ 0x000000c3, 0x0000000a },	// TXREFCK clock select
	{ 0x000000c5, 0x00000040 },	// Internal clock selection
	{ 0x000000c8, 0x44434000 },	// PCK_A, Internal clock selection
	{ 0x000000ca, 0x03500039 },	// EXSDI Encoder clock select
	{ 0x000000d1, 0x00004800 },	// VDO_DI_A, MUX_DI_A Select
	{ 0x000000d2, 0x00003005 },	// Internal data MUX selection
	{ 0x000000d9, 0x00000001 },	// EXENC Codec mode	                                      
	{ 0x000000e1, 0x00000897 },	// Horizontal total
	{ 0x000000e2, 0x04640780 },	// Vertical total, Horizontal active
	{ 0x000000e3, 0x04380028 },	// Vertical active, Vertical start point
	{ 0x000000e4, 0x01043060 },	// Test Pattern Generation ON	                                      
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
	{ 0x000000c5, 0x000000c0 },	// RPCK_A enable
	{ 0x000000c8, 0xc4434000 }, // PCK_A enable
	{ 0x000000ca, 0x13500139 },	// EXD_A, EXM_A enable	                                      
	{ 0x000000c6, 0x00000000 },	// Encoder Disable
	{ 0x000000c6, 0x0000000f },	// Encoder clock select
	{ 0x000000c6, 0x0000001f },	// Encoder Enable 	                                      
	{ 0x00000033, 0x00021904 },	// TSDI_SR_EN 
	{ 0x00000091, 0x00024809 }	// TSDI_DO_OFF

};

// ==================================================================================
// No input (EX-SDI 2.0 (135Mbps) test pattern out)
// ==================================================================================
#define TP_EX135_HD_Size	38
const UINT TP_EX135_HD[TP_EX135_HD_Size][2] =
{
	{ 0x000000c5, 0x00000000 },	// RPCK_A Disable
	{ 0x000000c8, 0x40004000 },	// VD_A Disable
	{ 0x000000ca, 0x00000000 },	// EXD_A, EXM_A Disable
	                           	
	{ 0x00000031, 0x0000f707 },	// TSDI_RATE select
	{ 0x00000041, 0x00003800 },	// In-buffer out data select
	{ 0x0000004a, 0x00800050 },	// Internal data MUX selection
	{ 0x00000094, 0x00001388 },	// OBUF Rate select
	{ 0x000000c2, 0x00000054 },	// TXFCK clock select
	{ 0x000000c3, 0x0000000a },	// TXREFCK clock select
	{ 0x000000c5, 0x00000040 },	// Internal clock selection
	{ 0x000000c8, 0x44434000 },	// PCK_A, Internal clock selection
	{ 0x000000ca, 0x03500039 },	// EXSDI Encoder clock select
	{ 0x000000d1, 0x00004800 },	// VDO_DI_A, MUX_DI_A Select
	{ 0x000000d2, 0x00003005 },	// Internal data MUX selection
	{ 0x000000d9, 0x00000001 },	// EXENC Codec mode
	                           	
	{ 0x000000e1, 0x00000897 },	// Horizontal total
	{ 0x000000e2, 0x04640780 },	// Vertical total, Horizontal active
	{ 0x000000e3, 0x04380028 },	// Vertical active, Vertical start point
	{ 0x000000e4, 0x01043060 },	// Test Pattern Generation ON
	                           	
	{ 0x00000112, 0x20804c00 },	// EXSDI Version
	{ 0x00000113, 0x60018800 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000114, 0x02000080 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000119, 0x00000001 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000125, 0x00c800b4 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000126, 0xe019f201 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000127, 0x81a61d80 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000128, 0x00000000 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x0000011d, 0x00000000 },	// EX Split off
	{ 0x0000012a, 0x8607080b },	// EX-SDI Voffset
	{ 0x0000012b, 0x0d07080a },	// EX-SDI Voffset
	                           	
	{ 0x000000c5, 0x000000c0 },	// RPCK_A enable
	{ 0x000000c8, 0xc4434000 },	// PCK_A enable
	{ 0x000000ca, 0x13500139 },	// EXD_A, EXM_A enable
	                           	
	{ 0x000000c6, 0x00000000 },	// Encoder Disable
	{ 0x000000c6, 0x0000000f },	// Encoder clock select
	{ 0x000000c6, 0x0000001f },	// Encoder Enable 
	                           	
	{ 0x00000033, 0x00021902 },	// TSDI_SR_EN 
	{ 0x00000091, 0x00024809 }	// TSDI_DO_OFF

};

// ==================================================================================
// No input (HD-SDI(1.485Gbps) test pattern out)
// ==================================================================================
#define TP_HDSDI_Size	31
const UINT TP_HDSDI[TP_HDSDI_Size][2] =
{
	{ 0x000000c5, 0x00000000 },	// RPCK_A Disable
	{ 0x000000c8, 0x40004000 },	// VD_A Disable
	{ 0x000000ca, 0x00000000 },	// EXD_A, EXM_A Disable
	                           
	{ 0x00000031, 0x0000f705 },	// TSDI_RATE select
	{ 0x00000041, 0x00003800 },	// In-buffer out data select
	{ 0x0000004a, 0x00800050 },	// Internal data MUX selection
	{ 0x00000094, 0x000010b0 },	// OBUF Rate select
	{ 0x000000c2, 0x00000000 },	// TXFCK clock select
	{ 0x000000c3, 0x0000000a },	// TXREFCK clock select
	{ 0x000000c5, 0x00000040 },	// Internal clock selection
	{ 0x000000c8, 0x43434000 },	// PCK_A, Internal clock selection
	{ 0x000000ca, 0x00000000 },	// EXSDI Encoder clock select
	{ 0x000000d1, 0x00004800 },	// VDO_DI_A, MUX_DI_A Select
	{ 0x000000d2, 0x00003000 },	// Internal data MUX selection
	{ 0x000000d9, 0x00000003 },	// EXENC Codec mode
	                           
	{ 0x000000e1, 0x00000897 },	// Horizontal total
	{ 0x000000e2, 0x04640780 },	// Vertical total, Horizontal active
	{ 0x000000e3, 0x04380028 },	// Vertical active, Vertical start point
	{ 0x000000e4, 0x01043060 },	// Test Pattern Generation ON
                             
	{ 0x00000112, 0x00004c00 },	// EXSDI Version
	{ 0x0000012a, 0x8607080b },	// EX-SDI Voffset
	{ 0x0000012b, 0x0d07080a },	// EX-SDI Voffset
	                           
	{ 0x000000c5, 0x000000c0 },	// RPCK_A enable
	{ 0x000000c8, 0xc3434000 }, 	// PCK_A enable
	{ 0x000000ca, 0x00000000 },	// EXD_A, EXM_A Disable
	                           
	{ 0x000000c6, 0x00000000 },	// Encoder Disable
	{ 0x000000c6, 0x00000005 },	// Encoder clock select
	{ 0x000000c6, 0x00000015 },	// Encoder Enable 
	                           
	{ 0x00000033, 0x0002191f },	// TSDI_SR_EN 
	{ 0x00000091, 0x00024801 },	// TSDI_DO_OFF
	{ 0x00000098, 0x00004008 }	// ENC_ANC_ON[24] off <-- Ancillary data off to avoid Illegal Code error

};

// ==================================================================================
// No input (EX-SDI 4M (270Mbps) test pattern out)
// ==================================================================================
#define TP_EX270_4M_Size	38
const UINT TP_EX270_4M[TP_EX270_4M_Size][2] =
{
	{ 0x000000c5, 0x00000000 },	// RPCK_A Disable
	{ 0x000000c8, 0x40004000 },	// VD_A Disable
	{ 0x000000ca, 0x00000000 },	// EXD_A, EXM_A Disable
	                           
	{ 0x00000031, 0x0000f705 },	// TSDI_RATE select
	{ 0x00000041, 0x00003800 },	// In-buffer out data select
	{ 0x0000004a, 0x00800050 },	// Internal data MUX selection
	{ 0x00000094, 0x00001188 },	// OBUF Rate select
	{ 0x000000c2, 0x00000043 },	// TXFCK clock select
	{ 0x000000c3, 0x0000000a },	// TXREFCK clock select
	{ 0x000000c5, 0x00000030 },	// Internal clock selection
	{ 0x000000c8, 0x44434000 },	// PCK_A, Internal clock selection
	{ 0x000000ca, 0x02500029 },	// EXSDI Encoder clock select
	{ 0x000000d1, 0x00004800 },	// VDO_DI_A, MUX_DI_A Select
	{ 0x000000d2, 0x00003005 },	// Internal data MUX selection
	{ 0x000000d9, 0x00000001 },	// EXENC Codec mode
	                           
	{ 0x000000e1, 0x00000ce3 },	// Horizontal total
	{ 0x000000e2, 0x05db0a00 },	// Vertical total, Horizontal active
	{ 0x000000e3, 0x05a00030 },	// Vertical active, Vertical start point
	{ 0x000000e4, 0x01043060 },	// Test Pattern Generation ON
	                           
	{ 0x00000112, 0x20804c00 },	// EXSDI Version
	{ 0x00000113, 0x60018800 },	// EXSDI Setting for 3G EX-SDI
	{ 0x00000114, 0x02000080 },	// EXSDI Setting for 3G EX-SDI
	{ 0x00000119, 0x00000001 },	// EXSDI Setting for 3G EX-SDI
	{ 0x00000125, 0x00c800b4 },	// EXSDI Setting for 3G EX-SDI
	{ 0x00000126, 0xc019f201 },	// EXSDI Setting for 3G EX-SDI
	{ 0x00000127, 0x81a61fc0 },	// EXSDI Setting for 3G EX-SDI
	{ 0x00000128, 0xc1000000 },	// EXSDI Setting for 3G EX-SDI
	{ 0x0000011d, 0x00000000 },	// EX Split off
	{ 0x0000012a, 0x8607080b },	// EX-SDI Voffset
	{ 0x0000012b, 0x0d07080a },	// EX-SDI Voffset
	                           
	{ 0x000000c5, 0x000000b0 },	// RPCK_A enable
	{ 0x000000c8, 0xc4434000 }, // PCK_A enable
	{ 0x000000ca, 0x12500129 },	// EXD_A, EXM_A enable
	                           
	{ 0x000000c6, 0x00000000 },	// Encoder Disable
	{ 0x000000c6, 0x0000000f },	// Encoder clock select
	{ 0x000000c6, 0x0000001f },	// Encoder Enable 
	                           
	{ 0x00000033, 0x00021904 },	// TSDI_SR_EN 
	{ 0x00000091, 0x00024809 }	// TSDI_DO_OFF

};

// ==================================================================================
// No input (EX-SDI 4K (1.485Gbps) test pattern out)
// ==================================================================================
#define TP_EX1G5_4K_Size	48
const UINT TP_EX1G5_4K[TP_EX1G5_4K_Size][2] =
{
	{ 0x000000c5, 0x00000000 },	// RPCK_A Disable
	{ 0x000000c8, 0x40004000 },	// VD_A Disable
	{ 0x000000ca, 0x00000000 },	// EXD_A, EXM_A Disable
	{ 0x000000cb, 0x00000000 },	// EXD_B, EXM_B Disable       //************* only 4K
	                         
	{ 0x00000031, 0x0000f705 },	// TSDI_RATE select
	{ 0x00000041, 0x00003800 },	// In-buffer out data select
	{ 0x0000004a, 0x00800050 },	// Internal data MUX selection
	{ 0x00000094, 0x000010b0 },	// OBUF Rate select
	{ 0x000000c2, 0x00000000 },	// TXFCK clock select
	{ 0x000000c3, 0x0000000a },	// TXREFCK clock select
	{ 0x000000c8, 0x42224000 },	// PCK_A, Internal clock selection
	{ 0x000000c5, 0x00000020 },	// Internal clock selection
	{ 0x000000ca, 0x0a500029 },	// EXSDI Encoder clock select
	{ 0x000000cb, 0x0025002d },	// EXSDI Encoder clock select //************* only 4K
	{ 0x000000d1, 0x00004800 },	// VDO_DI_A, MUX_DI_A Select
	{ 0x000000d2, 0x00000305 },	// Internal data MUX selection
	{ 0x000000d9, 0x00000000 },	// EXENC Codec mode			  //************* only 4K
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
	                         
	{ 0x000000c5, 0x000000a0 },	// RPCK_A enable
	{ 0x000000c8, 0xc2224000 }, // PCK_A enable
	{ 0x000000ca, 0x1a500129 },	// EXD_A, EXM_A enable
	{ 0x000000cb, 0x0125012d },	// EXD_B, EXM_B Disable       //************* only 4K
	                         
	{ 0x000000c6, 0x00000000 },	// Encoder Disable
	{ 0x000000c6, 0x0000000f },	// Encoder clock select
	{ 0x000000c6, 0x0000001f },	// Encoder Enable
	                         
	{ 0x00000033, 0x0002191f },	// TSDI_SR_EN 
	{ 0x00000091, 0x00024809 }	// TSDI_DO_OFF

};

// ==================================================================================
// No input (3G-SDI (2.97Gbps) test pattern out)
// ==================================================================================
#define TP_3GSDI_Size	30
const UINT TP_3GSDI[TP_3GSDI_Size][2] =
{
	{ 0x000000c5, 0x00000000 },	// RPCK_A Disable
	{ 0x000000c8, 0x40004000 },	// VD_A Disable
	{ 0x000000ca, 0x00000000 },	// EXD_A, EXM_A Disable
	                           
	{ 0x00000031, 0x0000f705 },	// TSDI_RATE select
	{ 0x00000041, 0x00003800 },	// In-buffer out data select
	{ 0x0000004a, 0x00800050 },	// Internal data MUX selection
	{ 0x00000094, 0x000012b0 },	// OBUF Rate select
	{ 0x000000c2, 0x00000000 },	// TXFCK clock select
	{ 0x000000c3, 0x0000000a },	// TXREFCK clock select
	{ 0x000000c5, 0x00000030 },	// Internal clock selection
	{ 0x000000c8, 0x43434000 },	// PCK_A, Internal clock selection
	{ 0x000000ca, 0x00000000 },	// EXSDI Encoder clock select
	{ 0x000000d1, 0x00004800 },	// VDO_DI_A, MUX_DI_A Select
	{ 0x000000d2, 0x00003000 },	// Internal data MUX selection
	{ 0x000000d9, 0x00000003 },	// EXENC Codec mode
	                           
	{ 0x000000e1, 0x00000897 },	// Horizontal total
	{ 0x000000e2, 0x04640780 },	// Vertical total, Horizontal active
	{ 0x000000e3, 0x04380028 },	// Vertical active, Vertical start point
	{ 0x000000e4, 0x01043060 },	// Test Pattern Generation ON
	                           
	{ 0x00000112, 0x00004c00 },	// EXSDI Version
	{ 0x0000012a, 0x8607080b },	// EX-SDI Voffset
	{ 0x0000012b, 0x0d07080a },	// EX-SDI Voffset
	                           
	{ 0x000000c5, 0x000000b0 },	// RPCK_A enable
	{ 0x000000c8, 0xc3434000 }, // PCK_A enable
	{ 0x000000ca, 0x00000000 },	// EXD_A, EXM_A Disable
	                           
	{ 0x000000c6, 0x00000000 },	// Encoder Disable
	{ 0x000000c6, 0x00000004 },	// Encoder clock select
	{ 0x000000c6, 0x00000014 },	// Encoder Enable 
	                           
	{ 0x00000033, 0x0002191f },	// TSDI_SR_EN 
	{ 0x00000091, 0x00024801 }	// TSDI_DO_OFF
	
};

// ==================================================================================
// No input (EX-SDI TDM (1.485Gbps) Channel A test pattern out)
// ==================================================================================
#define TP_EX1G5_TDM_A_Size	36
const UINT TP_EX1G5_TDM_A[TP_EX1G5_TDM_A_Size][2]  =
{
	{ 0x00000041, 0x00003810 },	// IBUF_IPOL_A
	{ 0x000000e4, 0x01043060 },	// Test Pattern Generation ON
	{ 0x000000d1, 0x00004800 },	// RX_A_OSEL
	{ 0x0000004a, 0x00800050 },	// EXTR_OSEL_A
	{ 0x000000c5, 0x00000000 },
	{ 0x000000c5, 0x00000040 },	// RPCK_A_EN
	{ 0x000000c5, 0x000000c0 },
	{ 0x000000c2, 0x00000054 },
	{ 0x000000c6, 0x00000000 },	// ENC_EN
	{ 0x000000c6, 0x00000005 },	// ENC_PCK_SEL
	{ 0x000000c6, 0x00000015 },	// ENC_EN
	{ 0x000000ca, 0x00000000 },	// 
	{ 0x000000ca, 0x03500039 },	// EXSDI_A
	{ 0x000000ca, 0x13500139 },	// 
	{ 0x000000cc, 0x00000000 },	// 
	{ 0x000000cc, 0x00000405 },	// EXT_EXICK_SEL
	{ 0x000000cc, 0x00001405 },	// 
	{ 0x000000d2, 0x00000007 },	// ENC_VDI_SEL
	{ 0x000000d4, 0x00002000 },	// EXTDM_EX_DI_SEL
	{ 0x000000d9, 0x00000001 },	// CODEC_MODE_A
	{ 0x00000112, 0x20804c00 },	// EXSDI Version
	{ 0x00000113, 0x60018800 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000114, 0x02000080 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000119, 0x00000001 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000125, 0x00c800b4 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000126, 0xe019f201 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000127, 0x81a61d80 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000128, 0x00000000 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x0000012a, 0x8607080b },	// EX-SDI Voffset
	{ 0x0000012b, 0x0d07080a },	// EX-SDI Voffset	
	{ 0x000001a1, 0x80000002 },
	{ 0x000001a2, 0x00120000 },
	{ 0x000001a3, 0x10000000 },
	{ 0x00000091, 0x00024809 },
	{ 0x00000094, 0x000010a0 },
	{ 0x00000033, 0x0002191f }	// TSDI_SR_EN
	
};

// ==================================================================================
// No input (EX-SDI TDM (1.485Gbps) Channel B test pattern out)
// ==================================================================================
#define TP_EX1G5_TDM_B_Size	36
const UINT TP_EX1G5_TDM_B[TP_EX1G5_TDM_B_Size][2]  =
{
	{ 0x00000051, 0x00003810 },	// IBUF_IPOL_B
	{ 0x000000eb, 0x01043060 },	// Test Pattern Generation ON
	{ 0x000000d1, 0x00001800 },	// RX_B_OSEL
	{ 0x0000005a, 0x00800050 },	// EXTR_OSEL_B
	{ 0x000000c5, 0x00000000 },
	{ 0x000000c5, 0x00000004 },	// RPCK_B_EN
	{ 0x000000c5, 0x0000000c },
	{ 0x000000c2, 0x00000054 },
	{ 0x000000c6, 0x00000000 },	// ENC_EN
	{ 0x000000c6, 0x00000005 },	// ENC_PCK_SEL
	{ 0x000000c6, 0x00000015 },	// ENC_EN
	{ 0x000000cb, 0x00000000 },	// 
	{ 0x000000cb, 0x0035003d },	// EXSDI_B
	{ 0x000000cb, 0x0135013d },	// 
	{ 0x000000cc, 0x00000000 },	// 
	{ 0x000000cc, 0x00000405 },	// EXT_EXICK_SEL
	{ 0x000000cc, 0x00001405 },	// 
	{ 0x000000d2, 0x00000007 },	// ENC_VDI_SEL
	{ 0x000000d4, 0x00003000 },	// EXTDM_EX_DI_SEL
	{ 0x000000d9, 0x00000002 },	// CODEC_MODE_B
	{ 0x00000132, 0x20804c00 },	// EXSDI Version
	{ 0x00000133, 0x60018800 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000134, 0x02000080 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000139, 0x00000000 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000145, 0x00c800b4 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000146, 0xe019f201 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000147, 0x81a61d80 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x00000148, 0x00000000 },	// EXSDI Setting for EX-SDI 2.0
	{ 0x0000014a, 0x8607080b },	// EX-SDI Voffset
	{ 0x0000014b, 0x0d07080a },	// EX-SDI Voffset
	{ 0x000001a1, 0x80000002 },
	{ 0x000001a2, 0x00120000 },
	{ 0x000001a3, 0x10000000 },
	{ 0x00000091, 0x00024809 },
	{ 0x00000094, 0x000010a0 },
	{ 0x00000033, 0x0002191f }	// TSDI_SR_EN
	
};
