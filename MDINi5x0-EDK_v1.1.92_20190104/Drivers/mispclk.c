// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"misp100.h"
#include	"cmos_if.h"		// for sensor dependent option


// ----------------------------------------------------------------------
// jins, board dependent configures about input clock (temp for ISC.WEST 2016)
// ----------------------------------------------------------------------
#if	defined(__MDIN_i5XX_FPGA__)
ROMDATA BYTE clk_inv_m[2]={1, 1};  //main - hdmi.a, hdmi.b
ROMDATA BYTE clk_dly_m[2]={0, 0};  //main - hdmi.a, hdmi.b	
ROMDATA BYTE clk_inv_a[2]={1, 1};  //aux  - hdmi.a, hdmi.b
ROMDATA BYTE clk_dly_a[2]={0, 0};  //aux - hdmi.a, hdmi.b
#else
	#if defined(__USE_MDIN_i5XX_REV__)
		ROMDATA BYTE clk_inv_m[2]={0, 0};  //main - hdmi.a, hdmi.b
		ROMDATA BYTE clk_dly_m[2]={0, 0};  //main - hdmi.a, hdmi.b	
		ROMDATA BYTE clk_inv_a[2]={0, 0};  //aux  - hdmi.a, hdmi.b
		ROMDATA BYTE clk_dly_a[2]={0, 0};  //aux - hdmi.a, hdmi.b
	#else
		ROMDATA BYTE clk_inv_m[2]={0, 1};  //main - hdmi.a, hdmi.b
		ROMDATA BYTE clk_dly_m[2]={0, 0};  //main - hdmi.a, hdmi.b // 1080p
		// ROMDATA BYTE clk_dly_m[2]={0, 2};  //main - hdmi.a, hdmi.b // 2160p
		ROMDATA BYTE clk_inv_a[2]={0, 0};  //aux  - hdmi.a, hdmi.b
		// ROMDATA BYTE clk_dly_a[2]={0, 1};  //aux - hdmi.a, hdmi.b
		ROMDATA BYTE clk_dly_a[2]={0, 2};  //aux - hdmi.a, hdmi.b // 2160p
	#endif	
#endif

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static HostCLK_CTRL_PARAM stHostCLKCTRL;

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
static void Init_HostCLK_PARAM( PHostCLK_CTRL_PARAM pCTRL ) ;
static void Init_HostCLK_REGISTER( PHostCLK_CTRL_PARAM pCTRL ) ;
static void CLKCTRL_ISPClockHandler( PHostCLK_CTRL_PARAM pCTRL ) ;
static void CLKCTRL_AUXClockHandler( PHostCLK_CTRL_PARAM pCTRL ) ;
static void CLKCTRL_VideoClockHandler( PHostCLK_CTRL_PARAM pCTRL ) ;
static void CLKCTRL_MemoryClockHandler( PHostCLK_CTRL_PARAM pCTRL ) ;
static void CLKCTRL_VideoPLLHandler( PHostCLK_CTRL_PARAM pCTRL ) ;
static void CLKCTRL_UserPLLHandler( PHostCLK_CTRL_PARAM pCTRL ) ;
static void CLKCTRL_MemoryPLLHandler( PHostCLK_CTRL_PARAM pCTRL ) ;
static void CLKCTRL_ClockOnOFFHandler( PHostCLK_CTRL_PARAM pCTRL ) ;
// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
static void Init_HostCLK_PARAM( PHostCLK_CTRL_PARAM pCTRL )
{
// ----------------------------------------------------------------------
	pCTRL->ADSTSR_UCOM_EN = 1 ; // When use internal cpu : mpll initialize skip	

#ifndef	SYSTEM_USE_OSC36M	
	pCTRL->SYSTEM_OSC36M = 0 ;	// 0: OSC X_tal is 27MHz
#else
	pCTRL->SYSTEM_OSC36M = 1 ;	// 1 : OSC X_tal is 36MHz	
#endif

	SetCLK_ClkLVDSInOnOff(1) ;
	SetCLK_ClkSensorInOnOff(1) ;

	SetCLK_ClkAEOnOff(1) ;
	SetCLK_ClkWBOnOff(1) ;

#if (defined(__USE_AFZOOM_LENS__)||defined(__USE_2MOTOR_LENS__)) || defined(__USE_AF_SLAVE__) || defined (__USE_AF_DATA_OUT__)
	SetCLK_ClkAFOnOff(1) ;
#else
	SetCLK_ClkAFOnOff(0) ;
#endif	

	SetCLK_ClkWDRRdOnOff(0) ;
	SetCLK_ClkWDROnOff(0) ;
	SetCLK_ClkMGROnOff(0) ;
	SetCLK_ClkTMOnOff(0) ;

#if defined(__MDIN_i550_USB_V100__)
	SetCLK_ClkNROnOff(0) ;
#else
	SetCLK_ClkNROnOff(1) ;
#endif
//	SetCLK_ClkNROnOff(0) ;	// for NR block disable
	SetCLK_ClkNRcOnOff(1) ;

	SetCLK_ClkYCOnOff(1) ;
	SetCLK_ClkYCmOnOff(1) ;

#if 0
	SetCLK_ClkMDOnOff(0) ;
	MISP_RegField(MISP_LOCAL_ID, 0x04A,  0,  1, 1);		// MD block bypass
#else
	SetCLK_ClkMDOnOff(1) ;
#endif	
	
	SetCLK_ClkDISOnOff(1) ;

	SetCLK_ClkInAUXOnOff(1) ;

	SetCLK_ClkAUXInOnOff(1) ;
	SetCLK_ClkAUXMidOnOff(1) ;
	SetCLK_ClkAUXOutOnOff(1) ;

	SetCLK_ClkVideoOnOff(1) ;
	SetCLK_ClkVideoOutOnOff(1) ;
	SetCLK_ClkEncOnOff(1) ;
	
#if (defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)) && \
	(defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__))
	SetCLK_ClkLVDSTxOnOff(1) ;	// Use AUX sync until i5x0 revision for Sensor Slave Mode
#elif defined(__USE_ISP_SCLK_OUT__)
	SetCLK_ClkLVDSTxOnOff(1) ;
#elif defined(__PANORAMA_SENSOR_SETUP__)
	SetCLK_ClkLVDSTxOnOff(1) ;
#else
#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
	SetCLK_ClkLVDSTxOnOff(1) ;
#else
	SetCLK_ClkLVDSTxOnOff(0) ;
#endif
#endif
	SetCLK_ClkAudioOnOff(0) ;

	SetCLK_ClkMemoryOnOff(1) ;
	
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// When use external cpu. mclk_PLL setup.
// See ../common/init.c  "static void SetPMUInit(void)" for internal CPU mclk_PLL setup.
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#if defined (__USE_ISP_SCLK_OUT__)
#if defined (__USE_CMOS_IMX226__) || defined (__USE_CMOS_IMX274__) || defined(__PANORAMA_SENSOR_SETUP__)|| \
	defined (__USE_CMOS_IMX326__) || defined (__USE_CMOS_IMX253__) || defined (__USE_CMOS_IMX273__) || defined (__USE_CMOS_IMX334__) || defined (__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX265__)
//------------------------------------------------------------------------------
	#if defined(__OUT_VCLKPLL_LOCK__) && !(defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__))
		// OUT_VCLKPLL_LOCK : digital pin output
		// system stable should be preceded before lvds tx output with vout_pll lock. 
		#if 1
		//statndard v-clock out such as 37.125, 74.25Mhz...
		SetCLK_UserPLL_LockMode(0);	SetCLK_UserPLL_LockSrcPort(0);		
		pCTRL->stISPCLK.clk_user.clk_src_select = 6 ;			//user pll source : output vclk pll	int
		//pCTRL->stISPCLK.clk_user.clk_src_select = 2 ;		//host clock pin bypass	
		#else
		//use non standard v-clock out, example 36Mhz 	SetCLK_UserPLL(1,4,33,3,1) ;	// 36Mhz -> 37.125Mhz    (30Mhz case     SetCLK_UserPLL(1,4,33,3,1)
		SetCLK_UserPLL_LockMode(2);	SetCLK_UserPLL_LockSrcPort(1);						//pin host_clk
//		SetCLK_UserPLL_LockMode(2);	SetCLK_UserPLL_LockSrcPort(2);						//pin ext_clk(aux clk port)
		pCTRL->stISPCLK.clk_user.clk_src_select = 6 ;	
		#endif
	#else
	SetCLK_UserPLL_LockMode(0);	SetCLK_UserPLL_LockSrcPort(0);	
	pCTRL->stISPCLK.clk_user.clk_src_select = 6 ;		//default
//	pCTRL->stISPCLK.clk_user.clk_src_select = 0 ;		//test xtal
	#endif

#elif defined(__USE_CMOS_EXT3__)
	SetCLK_UserPLL_LockMode(0);	SetCLK_UserPLL_LockSrcPort(0);
	pCTRL->stISPCLK.clk_user.clk_src_select = 0 ;
#elif defined(__USE_CMOS_MN34420__) && defined(__USE_EXT_OSC_148M__)
	pCTRL->stISPCLK.clk_user.clk_src_select = 6 ;
#else
//	pCTRL->stISPCLK.clk_user.clk_src_select = 0 ;
#endif

#if defined(__PANORAMA_SENSOR_SETUP__) 
	MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x0421);	// 
#elif defined (__USE_IMX226_CLK_MATCH_SCLK_)
	MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x0421);	// 	
#elif defined (__MDIN_i550_USB_V100__)
	MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x0421);	// 	use user pll for aux v.out clk
#elif defined(__USE_IMX290_1080P120_MODE__) ||defined( __USE_IMX385_1080P120_MODE__) ||defined( __USE_IMX290_720P120_ISPCROP__)|| \
	defined(__USE_CMOS_IMX273_LVDS_S8__)
	MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x2021);	// user pll clk div 8 
#else
	#if defined(__USE_CMOS_MN34420__) && defined(__USE_EXT_OSC_148M__)
	MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x0421);	// user pll clk div 1
	#else
	MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x1021);	// user pll clk div 4 
	#endif
#endif

	#if defined(__OUT_VCLKPLL_LOCK__)
		#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_MAIN_SINGLE__) || defined(__USE_LVDSTX_EVEN_ODD__)
		MISP_RegField(MISP_HOST_ID,  0x02F, 0, 4, 14);	//from 74.25 x 7 vpll to 37.125 user clock out Mhz
		#else
		MISP_RegField(MISP_HOST_ID,  0x02F, 0, 4, 2);	//from 74.25 vpll to 37.125 user clock out Mhz
		#endif

		if(GetCLK_UserPLL_LockMode()==2)	MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x0421);	// pll source ---> pll --> divider --> user_clock out
	#endif

#else	//__USE_ISP_SCLK_OUT__
	MISP_RegWrite(MISP_HOST_ID, 0x02F, 0x0421);	// 
#endif	//__USE_ISP_SCLK_OUT__


	if (pCTRL->ADSTSR_UCOM_EN == 0)	{		// When use external cpu
		pCTRL->stMPLL.x_tal2_sel = 0 ;		// [¡®0¡¯ : x_tal is selected] [¡®1¡¯ : x_tal2 is selected]	
		
#if defined(__USE_MDIN_i500__) 				// MDIN-i500
		if ( pCTRL->SYSTEM_OSC36M == 1 )	SetCLK_MemoryPLL(4,44,0) ;	// 36M -> 198Mhz
		else								SetCLK_MemoryPLL(3,44,0) ;	// 27M -> 198Mhz
#else										// MDIN-i51X (i510, i540, i550)
		//if ( pCTRL->SYSTEM_OSC36M == 1 )	SetCLK_MemoryPLL(4,88,0) ;	// 36M -> 396Mhz(vertical noise !!!)
		//else								SetCLK_MemoryPLL(3,88,0) ;	// 27M -> 396Mhz(vertical noise !!!)
		if ( pCTRL->SYSTEM_OSC36M == 1 )	SetCLK_MemoryPLL(6,125,0) ;	// 36M -> 375Mhz(o.k)
		else								SetCLK_MemoryPLL(9,250,0) ;	// 27M -> 375Mhz(o.k)
#endif
	}


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// Video PLL setup
// Video PLL output clk setup overide occurs in ../Drivers/mispoutput.c "static void OUTCTRL_SetMainFrmt(BYTE frmt)"	
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

	// select pll input clock
	pCTRL->stVPLL.pll_en_n = 0;			// '0' : enable pll,  '1' : disable pll
#if defined(__USE_MDIN_i5XX_REV__)		// i5xx rev chip
	pCTRL->stVPLL.pll_sel_vclk = 0;		// '0' : pll_in_x_tal_sel,  '1' : pll_in_clk_sel
	pCTRL->stVPLL.pll_in_clk_sel = 0;	// '0' : clk_b,  '1' : clk_a,  '1x' : clk_lvds_p

#if defined(__USE_EXT_PLL_IC__) || defined(__USE_EXT_OSC_37M__) || defined(__USE_EXT_OSC_148M__)	// for 'external pll ic' or 'osc(37.0879Mhz)' ( pll input set to 'host_clk' )
	pCTRL->stVPLL.pll_in_x_tal_sel = 2;	// '0' : x_tal,  '1' : x_tal2,  '1x' : host_clk
#else									// for internal pll ( pll input set to 'x_tal' )
	pCTRL->stVPLL.pll_in_x_tal_sel = 0;	// '0' : x_tal,  '1' : x_tal2,  '1x' : host_clk
#endif
#else									// i5xx org chip
	pCTRL->stVPLL.pll_sel_xtal2 = 0; 	// '0' : pll_sel_xtal,  '1' : x_tal2
	pCTRL->stVPLL.pll_sel_inclk = 1; 	// '0' : clk_b,  '1' : clk_a, '2' : stISPCLK.clk_lvds_sel, '3' : lvds serial clk
	
#if defined(__USE_EXT_PLL_IC__) || defined(__USE_EXT_OSC_37M__) || defined(__USE_EXT_OSC_148M__)	// for 'external pll ic' or 'osc(37.0879Mhz)' ( pll input set to 'host_clk' )
	pCTRL->stVPLL.pll_sel_xtal = 0 ;	// '0' : pll_sel_hclk,  '1' : X_tal
	pCTRL->stVPLL.pll_sel_hclk = 1 ; 	// '0' : pll_sel_inclk,  '1' : host_clk
#else									// for internal pll ( pll input set to 'X_tal' )
	pCTRL->stVPLL.pll_sel_xtal = 1 ;	// '0' : pll_sel_hclk,  '1' : X_tal
	pCTRL->stVPLL.pll_sel_hclk = 0 ; 	// '0' : pll_sel_inclk,  '1' : host_clk
#endif
#endif	// __USE_MDIN_i5XX_REV__
	
	
	// set pll bypass
#if defined(__USE_EXT_PLL_IC__)			// for external pll ic
	pCTRL->stVPLL.pll_bypass = 1;		// '0' : internal_pll_use, '1' : internal_pll_bypass
#else
	pCTRL->stVPLL.pll_bypass = 0;		// '0' : internal_pll_use, '1' : internal_pll_bypass
#endif

	
	// set pll out freq.
#if defined(__USE_MDIN_i500__) 				// MDIN-i500
	if ( pCTRL->SYSTEM_OSC36M == 1 )		SetCLK_VideoPLL(1,33,8);		// 36M -> 148.5Mhz
	else									SetCLK_VideoPLL(4,22,1);		// 27M -> 148.5Mhz
#else										// MDIN-i51X (i510, i540, i550)
	if ( pCTRL->SYSTEM_OSC36M == 1 )		SetCLK_VideoPLL(1,2,33,2,1);	// 36M -> 148.5Mhz
	else									SetCLK_VideoPLL(1,1,22,2,1);	// 27M -> 148.5Mhz
#endif


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// User PLL setup
// Check __USE_ISP_SCLK_OUT__ for using Sensor board OSC or not
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------	

#if defined (__USE_ISP_SCLK_OUT__)
	// If __USE_ISP_SCLK_OUT__ is used : Sensor board does not have OSC
	#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		pCTRL->stUSERPLL.ab_pll_sel = 0 ; // [¡®0¡¯ : CLK_B is used] [¡®1¡¯ : CLK_A is used]
	#endif
	#if defined(__USE_4K60_INOUT__)
		pCTRL->stUSERPLL.pll_in_sel = 3 ; //
	#elif defined(__USE_CMOS_MN34420__) && defined(__USE_EXT_OSC_148M__)
		pCTRL->stUSERPLL.pll_in_sel = 3 ; //
	#else
		if(GetCLK_UserPLL_LockSrcPort()==1)			pCTRL->stUSERPLL.pll_in_sel = 3 ; 
		else if(GetCLK_UserPLL_LockSrcPort()==2)		pCTRL->stUSERPLL.pll_in_sel = 2 ; 		//check also ab_pll_sel = 0
		else											pCTRL->stUSERPLL.pll_in_sel = 1 ; 
	#endif
		// [¡®00¡¯ : LVDS serial clk is used is used] // clk_lvds_p
		// [¡®01¡¯ : X_tal is used] 
		// [¡®10¡¯ : X_tal2 is used] / [ ab_pll_sel is used ]
		// [¡®11¡¯ : HOST_CLK is used]	
	
	#if defined(__USE_CMOS_MN34220__) && defined(__PANORAMA_SENSOR_SETUP__)
		pCTRL->stUSERPLL.pll_bypass = 1 ;
	#endif

	#if defined(__USE_4K60_INOUT__)
//		pCTRL->stUSERPLL.pll_bypass = 1 ;
	#endif
		
	
	#if defined(__USE_CMOS_MN34041__) || defined(__USE_CMOS_MN34220__) || defined(__USE_CMOS_MN34420__)  || defined(__USE_CMOS_PS1400K__) || defined(__USE_CMOS_AR0331_TTL__) || defined(__USE_CMOS_EXT3__)
	#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		SetCLK_UserPLL(4,8,2) ;
	#else 								// MDIN-i51X (i510, i540, i550)
		#if defined (__PANORAMA_SENSOR_SETUP__)
		SetCLK_UserPLL(1,1,24,3,1) ;	// 27Mhz -> 27Mhz
		#elif  defined(__USE_CMOS_PS1400K__)
		SetCLK_UserPLL(1,1,24,3,1) ;	// 27Mhz -> 27Mhz		
//		SetCLK_UserPLL(1,1,22,3,2) ;	// 27Mhz -> 37.125Mhz
		#else
			#if defined(__USE_EXT_OSC_148M__)
			SetCLK_UserPLL(1,11,16,3,1) ;	// 148.35 Mhz -> 148.35 / 11 * (16 / 2^3) --> 27Mhz
			#else
			SetCLK_UserPLL(1,1,24,3,1) ;	// 27Mhz -> 27Mhz			
			#endif
		#endif
	#endif
	#endif

	#if defined(__USE_CMOS_OV2710__) 
	#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		SetCLK_UserPLL(3,8,3) ;			//24Mhz
	#else 								// MDIN-i51X (i510, i540, i550)
		SetCLK_UserPLL(1,3,64,3,3) ;	// 27Mhz -> 24Mhz
	#endif
	#endif

	#if defined(__USE_CMOS_IMXx36__) || defined(__USE_CMOS_IMX174__) || defined(__USE_CMOS_IMX249__) || defined(__USE_CMOS_IMX185__) || \
		defined(__USE_CMOS_IMX252__) || defined(__USE_CMOS_AR0331_LVDS_S2__) || \
		defined(__USE_CMOS_IMX253__) || defined(__USE_CMOS_IMX327__)
	#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		SetCLK_UserPLL(4,22,4);		//37.125Mhz(27/4*22/4)
	#else 								// MDIN-i51X (i510, i540, i550)
		SetCLK_UserPLL(1,1,22,3,2) ;	// 27Mhz -> 37.125Mhz
	#endif
	#endif

	#if defined(__USE_CMOS_IMX274__)
		#if defined(__MDIN_i540_REF__) || defined(__MDIN_i550_REF_V121__) || defined(__MDIN_i540_4K60__) || defined(__MDIN_i550_USB_V100__)
			#if defined(__USE_MDIN_i5XX_REV__)
				#if defined(__USE_4K60_INOUT__)
					SetCLK_UserPLL(1,6,24,2,1) ;	// 297M(hclk) -> 297M/4 -> 74.25Mhz
				#else
					SetCLK_UserPLL(1,3,132,2,1) ;	// 27M(xtal) -> 297M/4 -> 74.25Mhz (30Hz)
				//	SetCLK_UserPLL(1,3,128,2,1) ;	// 27M(xtal) -> 288M/4 -> 72Mhz (29.97Hz)
				#endif
			#else
				SetCLK_UserPLL(1,3,107,2,1) ;	// 27M(xtal) -> 240.75M/4 -> 60.1875Mhz (25.05Hz)
			#endif
		#else
			SetCLK_UserPLL(1,3,84,2,1) ;		// 27M(xtal) -> 189M/4 -> 47.25Mhz
		#endif
	#endif

	#if defined(__USE_CMOS_IMX326__)
		#if defined(__USE_IMX326_4K15P_MODE__)	// clk_in -> user_pll_out/div -> usrclk_out
			SetCLK_UserPLL(1,3,66,2,1) ;		// 27M(xtal) -> 148.5M/4 -> 37.125Mhz
		#else
		#if defined(__MDIN_i540_REF__)  || defined(__MDIN_i550_REF_V121__) || defined(__MDIN_i550_USB_V100__) 
			SetCLK_UserPLL(1,3,107,2,1) ;		// 27M(xtal) -> 240.75M/4 -> 60.1875Mhz (25.05Hz)
		#else
			SetCLK_UserPLL(1,3,84,2,1) ;		// 27M(xtal) -> 189M/4 -> 47.25Mhz
		#endif
		#endif
	#endif

	#if defined(__USE_CMOS_IMX226__)
		#if defined(__USE_IMX226_CLK_MATCH_SCLK_)	
			#if defined(__USE_IMX226_4K15P_MODE__)	// clk_in -> user_pll_out/div -> usrclk_out
				SetCLK_UserPLL(1,3,66,2,4) ;		// 27M(xtal) -> 37.125M/1 -> 37.125Mhz
			#elif defined(__USE_IMX226_4K20P_MODE__)
				SetCLK_UserPLL(1,3, 86,2,4);		// 27M(xtal) -> 48.375M/1 -> 48.375Mhz
			#elif defined(__USE_IMX226_4K25P_MODE__)
				SetCLK_UserPLL(1,3,107,2,4);		// 27M(xtal) -> 60.1875M/1 -> 60.1875Mhz
			#else
				SetCLK_UserPLL(1,3*4,128,2,1) ;		// 27M(xtal) -> 72M/1 -> 72Mhz (29.97Hz)
			#endif
		#else
			#if defined(__USE_IMX226_4K15P_MODE__)
				SetCLK_UserPLL(1,3,66,2,1) ;		// 27M(xtal) -> 148.5/4 -> 37.125Mhz
			#elif defined(__USE_IMX226_4K20P_MODE__)
				SetCLK_UserPLL(1,3, 86,2,1);		// 27M(xtal) -> 193.5M/4 -> 48.375Mhz
			#elif defined(__USE_IMX226_4K25P_MODE__)
				SetCLK_UserPLL(1,3,107,2,1);		// 27M(xtal) -> 240.75M/4 -> 60.1875Mhz
			#else
				#if defined(__USE_4K60_INOUT__)
					SetCLK_UserPLL(1,6,24,2,1) ;	// 297M(hclk) -> 297M/4 -> 74.25Mhz
				#else
					#if defined(__USE_IMX226_1080P60_MODE__)
						SetCLK_UserPLL(1,3,128,2,1) ;	// 27M(xtal) -> 297M/4 -> 74.25Mhz
					#else
						SetCLK_UserPLL(1,3,132,2,1) ;	// 27M(xtal) -> 297M/4 -> 74.25Mhz
					#endif
				#endif
			#endif
		#endif
	#endif

	#if defined(__USE_CMOS_IMX253__)  
		SetCLK_UserPLL(1,3,132,2,1) ;	// 27M(xtal) -> 27M
	#endif

	#if defined(__USE_CMOS_IMX273__)  
		#if defined(__USE_CMOS_IMX273_LVDS_S8__)
			SetCLK_UserPLL(1,3,132,2,1) ;	// 27M(xtal) -> 27M
		#elif defined(__USE_CMOS_IMX273_LVDS_S2__) && (defined(__USE_IMX273_480P60_MODE__) || defined(__USE_IMX273_480P30_MODE__))
			#if defined(__USE_OSC_74_25MHZ__)
				SetCLK_UserPLL(1,3,132,2,1) ;	// 74.25/4		
			#else
				SetCLK_UserPLL(1,3,66,2,1) ;	// 27M(xtal) -> 27M
			#endif
		#else
			#if defined(__USE_OSC_74_25MHZ__)
				SetCLK_UserPLL(1,3,132,2,1) ;					
			#else
				#if defined(__OUT_VCLKPLL_LOCK__) 
					if(GetCLK_UserPLL_LockMode()==2)	SetCLK_UserPLL(1,4,33,3,1) ;	// 36Mhz -> 37.125Mhz		
					else									SetCLK_UserPLL(1,3,66,2,1) ;	// 27M(xtal) -> 27M
				#else
					SetCLK_UserPLL(1,3,66,2,1) ;	// 27M(xtal) -> 27M
				#endif
			#endif			
		#endif
	#endif

	#if defined(__USE_CMOS_IMX265__)  
		SetCLK_UserPLL(1,3,66,2,1) ;	
	#endif


	#if defined(__USE_CMOS_IMX334__)
		SetCLK_UserPLL(1,3,66,2,1) ;		// 27M(xtal) -> 148.5/4 -> 37.125Mhz
	#endif	

	#if defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__)
		#if defined(__USE_IMX290_1080P120_MODE__) || defined(__USE_IMX290_720P120_ISPCROP__)		//lvds 8ch only
			SetCLK_UserPLL(1,3,132,2,1) ;		// 27M(user pll source = xtal, 297Mhz = 27*132/3/(2^2) ) -> 297 / 8 -> 37.125Mhz, user pll use 8ch 
		#else
			SetCLK_UserPLL(1,3,66,2,1) ;		// 27M(user pll source = xtal, 297Mhz = 27*132/3/(2^2) ) -> 148.5 / 4 -> 37.125Mhz
		#endif
	#endif		
	
#else
	// If __USE_ISP_SCLK_OUT__ is not used : Sensor board has OSC
	#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		pCTRL->stUSERPLL.ab_pll_sel = 0 ; // [¡®0¡¯ : CLK_B is used] [¡®1¡¯ : CLK_A is used]
	#endif
			
	#if defined(__USE_CMOS_IMX265__)  
		pCTRL->stUSERPLL.pll_in_sel = 1 ; // xtal
	#elif defined(__MDIN_i550_USB_V100__)
		pCTRL->stUSERPLL.pll_in_sel = 1 ; // xtal
	#else	
		pCTRL->stUSERPLL.pll_in_sel = 0 ; // LVDS serial clk
	#endif
		// [¡®00¡¯ : LVDS serial clk is used is used] 
		// [¡®01¡¯ : X_tal is used] 
		// [¡®10¡¯ : X_tal2 is used] / [ ab_pll_sel is used ]
		// [¡®11¡¯ : HOST_CLK is used]			

	#if defined(__USE_CMOS_MN34041__) || defined(__USE_CMOS_MN34220__) || defined(__USE_CMOS_MN34420__) || defined(__USE_CMOS_PS1400K__)
	#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		SetCLK_UserPLL(1,4,6);
	#else 								// MDIN-i51X (i510, i540, i550)
		SetCLK_UserPLL(1,3,64,3,4) ;	// 27M(xtal) -> 18M
	#endif
	#endif

	#if defined(__USE_CMOS_IMX174__) || defined(__USE_CMOS_IMX238__)
	#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		SetCLK_UserPLL(1,4,6);
	#else 								// MDIN-i51X (i510, i540, i550)
		SetCLK_UserPLL(1,3,64,3,4) ;	// 27M(xtal) -> 18M
	#endif
	#endif

	#if defined(__USE_CMOS_IMX249__)
	#if defined(__USE_MDIN_i500__) 		// MDIN-i500
		SetCLK_UserPLL(1,2,6);
	#else 								// MDIN-i51X (i510, i540, i550)
		SetCLK_UserPLL(1,3,56,3,7) ;	// 27M(xtal) -> 9M
	#endif
	#endif

	#if defined(__USE_CMOS_IMX178__)
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			#if defined (__USE_CMOS_IMX178_LVDS_S8__)
				SetCLK_UserPLL(1,8,6);
			#else
				SetCLK_UserPLL(1,4,6);
			#endif
		#else 								// MDIN-i51X (i510, i540, i550)
			#if defined (__USE_CMOS_IMX178_LVDS_S8__)
				SetCLK_UserPLL(1,3,64,3,2) ;			// 27M(xtal) -> 36M
			#else
				SetCLK_UserPLL(1,3,64,3,4) ;			// 27M(xtal) -> 18M
			#endif
		#endif
	#endif

	#if defined(__USE_CMOS_IMX124__)
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			#if defined (__USE_CMOS_IMX123_LVDS_S8__)
				SetCLK_UserPLL(1,8,6);
			#else
				SetCLK_UserPLL(1,4,6);
			#endif
		#else 								// MDIN-i51X (i510, i540, i550)
			#if defined (__USE_CMOS_IMX123_LVDS_S8__)
				SetCLK_UserPLL(1,3,64,3,2) ;			// 27M(xtal) -> 36M
			#else
				SetCLK_UserPLL(1,3,64,3,4) ;			// 27M(xtal) -> 18M
			#endif
		#endif
	#endif

	#if defined(__USE_CMOS_IMX252__)
		SetCLK_UserPLL(1,3,64,3,4) ;		// 27M(xtal) -> 18M
	#endif
	
	#if defined(__USE_CMOS_IMX253__)  
		SetCLK_UserPLL(1,1,24,3,3) ;	// 27M(xtal) -> 27M
	#endif

	#if defined(__USE_CMOS_IMX273__)  
		#if defined(__USE_CMOS_IMX273_LVDS_S8__)
			SetCLK_UserPLL(1,6,32,2,1) ;	// lvds clk-> 36Mhz			pll value restore for 120hz. 180320
		#else
			SetCLK_UserPLL(1,6,16,2,1) ;	// lvds clk-> 36Mhz			pll value restore for 120hz. 180320
		#endif
	#endif
	
	#if defined(__USE_CMOS_IMX265__)  
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,4,6);
		#else 								// MDIN-i51X (i510, i540, i550)
			// SetCLK_UserPLL(1,3,32,3,2) ;	// 27M(xtal) -> 18M -- 'OD' spec limit ??
				SetCLK_UserPLL(1,1,22,2,1) ;	// User pll to drive isp block : userplll source will be 27MHz xtal : 17.12.18
		#endif	
	#endif

	#if defined(__USE_CMOS_IMXx36__)
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			#if defined(__USE_CMOS_IMXx36_LVDS_S4__)		// use sony lvds s4
				SetCLK_UserPLL(1,4,6);
			#endif
			#if defined(__USE_CMOS_IMXx36_LVDS_S2__)		// use sony lvds s2
				SetCLK_UserPLL(1,2,6);
			#endif
			#if defined(__USE_CMOS_IMXx36_LVDS_P__)		// use sony lvds p
				SetCLK_UserPLL(1,4,2);
			#endif
		#else 								// MDIN-i51X (i510, i540, i550)
			#if defined(__USE_CMOS_IMXx36_LVDS_S4__)		// use sony lvds s4
				SetCLK_UserPLL(1,3,64,3,4) ;	// 27M(xtal) -> 18M
			#endif
			#if defined(__USE_CMOS_IMXx36_LVDS_S2__)		// use sony lvds s2
				SetCLK_UserPLL(1,3,56,3,7) ;	// 27M(xtal) -> 9M
			#endif
			#if defined(__USE_CMOS_IMXx36_LVDS_P__)		// use sony lvds p
				SetCLK_UserPLL(1,1,32,3,2) ;	// 27M(xtal) -> 54M
			#endif
		#endif
	#endif

	#if defined(__USE_CMOS_AR0331__)
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			#if defined(__USE_CMOS_AR0331_LVDS_S4__)		// use sony lvds s4
				SetCLK_UserPLL(1,4,6);
			#endif
			#if defined(__USE_CMOS_AR0331_LVDS_S2__)		// use sony lvds s4
				SetCLK_UserPLL(1,2,6);
			#endif
		#else 								// MDIN-i51X (i510, i540, i550)
			#if defined(__USE_CMOS_AR0331_LVDS_S4__)
				SetCLK_UserPLL(1,3,64,3,4) ;	// 27M(xtal) -> 18M
			#endif
			#if defined(__USE_CMOS_AR0331_LVDS_S2__)
				SetCLK_UserPLL(1,3,56,3,7) ;	// 27M(xtal) -> 9M
			#endif
		#endif
	#endif

	#if defined(__USE_CMOS_AR0134_LVDS_S2__)
	#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,2,6);
	#else 								// MDIN-i51X (i510, i540, i550)
		SetCLK_UserPLL(1,3,56,3,7) ;	// 27M(xtal) -> 9M
	#endif
	#endif	

	#if defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__)
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			#if defined(__USE_CMOS_IMX290_LVDS_S2__)
				SetCLK_UserPLL(1,2,6);
			#endif
			#if defined(__USE_CMOS_IMX290_LVDS_S4__)
				SetCLK_UserPLL(1,4,6);
			#endif
			#if defined(__USE_CMOS_IMX290_LVDS_S8__)
				SetCLK_UserPLL(1,8,6);
			#endif
		#else 								// MDIN-i51X (i510, i540, i550)
			#if defined(__USE_CMOS_IMX290_LVDS_S2__)
				SetCLK_UserPLL(1,3,56,3,7) ;	// 27M(xtal) -> 9M
			#endif
			#if defined(__USE_CMOS_IMX290_LVDS_S4__)
				#if defined(__USE_IMX290_720P120_MODE__)
					SetCLK_UserPLL(1,5,32,2,2) ;	// user pll for de-serializer 10bit 4ch
				#else
					// SetCLK_UserPLL(1,3,64,3,4) ;	// 27M(xtal) -> 18M
					SetCLK_UserPLL(1,3,32,3,2) ;	// 27M(xtal) -> 18M
				#endif
			#endif
			#if defined(__USE_CMOS_IMX290_LVDS_S8__)
				SetCLK_UserPLL(1,6,32,2,1) ;	// lvds clk-> 36Mhz			pll value restore for 120hz. 180320
			#endif
		#endif
	#endif

	#if defined(__USE_CMOS_IMX226__)
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			#if defined (__USE_CMOS_IMX226_LVDS_S4__)
				SetCLK_UserPLL(1,4,6);
			#else
				#if defined(__USE_IMX226_CLK_CONV__)
					SetCLK_UserPLL(1,6,6);
				#else
					SetCLK_UserPLL(1,8,6);
				#endif
			#endif
		#else 								// MDIN-i51X (i510, i540, i550)
			#if defined(__USE_CMOS_IMX226_LVDS_S4__)
				SetCLK_UserPLL(1,3,64,3,4) ;		// 27M(xtal) -> 18M
			#else
				#if defined(__USE_IMX226_CLK_CONV__)
					SetCLK_UserPLL(1,1,24,3,3) ;	// 27M(xtal) -> 27M
				#else
					SetCLK_UserPLL(1,3,64,3,2) ;	// 27M(xtal) -> 36M
				#endif
			#endif
		#endif
	#endif

	#if defined(__USE_CMOS_IMX274__)
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,6,6);
		#else 								// MDIN-i51X (i510, i540, i550)
			#if defined(__USE_IMX274_1080P60_MODE__)
			SetCLK_UserPLL(1,3,64,3,4) ;	// 27M(xtal) -> 18M
			#else
			SetCLK_UserPLL(1,1,24,3,3) ;	// 27M(xtal) -> 27M
			#endif

		#endif
	#endif

	#if defined(__USE_CMOS_IMX326__)
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,6,6);
		#else 								// MDIN-i51X (i510, i540, i550)
			SetCLK_UserPLL(1,1,24,3,3) ;	// 27M(xtal) -> 27M
		#endif
	#endif

	#if defined(__USE_CMOS_IMX185__) || defined(__USE_CMOS_IMX385__) 
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,4,6);
		#else 								// MDIN-i51X (i510, i540, i550)
			SetCLK_UserPLL(1,3,64,3,4) ;	// 27M(xtal) -> 18M
		#endif
	#endif
	
	#if defined(__USE_CMOS_EXT2__) 
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,6,6);
		#else 								// MDIN-i51X (i510, i540, i550)
			SetCLK_UserPLL(1,1,24,3,3) ;	// 27M(xtal) -> 27M
		#endif
	#endif
	
	#if defined(__USE_CMOS_EXT1__) 
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,5,6);
		#else 								// MDIN-i51X (i510, i540, i550)
			SetCLK_UserPLL(1,1,20,3,3) ;	// 27M(xtal) -> 22.5M
		#endif
	#endif
	
	#if defined(__USE_CMOS_OV2710__) 
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,4,6);
		#else 								// MDIN-i51X (i510, i540, i550)
			SetCLK_UserPLL(1,3,64,3,4) ;	// 27M(xtal) -> 18M
		#endif
	#endif
	
	#if defined(__USE_CMOS_EXT3__) 
		#if defined(__USE_MDIN_i500__) 		// MDIN-i500
			SetCLK_UserPLL(1,4,6);
		#else 								// MDIN-i51X (i510, i540, i550)
			SetCLK_UserPLL(1,3,64,3,4) ;	// 27M(xtal) -> 18M
		#endif
	#endif		
	
#endif
	
// ----------------------------------------------------------------------	
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ISP clock setup
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
	// clk_lvds_in : Deserializer clk

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	pCTRL->stISPCLK.clk_lvds_in.clk_invert = 0 ; //
	pCTRL->stISPCLK.clk_lvds_in.clk_delay = 0 ; //
#else 												// MDIN-i510,i540,i550
	#if defined(__MDIN_i510_EDK__) && (defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__))
		#if defined(__MDIN_i510_EDK_V10__)					// edk v1.0
			pCTRL->stISPCLK.clk_lvds_in.clk_invert = 1 ;
			pCTRL->stISPCLK.clk_lvds_in.clk_delay = 2 ;
		#else												// edk v1.1
			pCTRL->stISPCLK.clk_lvds_in.clk_invert = 0 ;
			pCTRL->stISPCLK.clk_lvds_in.clk_delay = 1 ;
		#endif
	#elif defined(__MDIN_i540_EDK__) && defined(__MDIN_i540_EDK_V11__)
		pCTRL->stISPCLK.clk_lvds_in.clk_invert = 0 ;
		pCTRL->stISPCLK.clk_lvds_in.clk_delay = 1 ;
	#elif defined(__MDIN_i550_EDK__) && defined(__MDIN_i550_EDK_V11__)
		pCTRL->stISPCLK.clk_lvds_in.clk_invert = 1 ;
		pCTRL->stISPCLK.clk_lvds_in.clk_delay = 1 ;
	#elif defined(__USE_CMOS_IMX226__) && (defined(__MDIN_i550_REF_V121__) || defined(__MDIN_i550_USB_V100__))
		pCTRL->stISPCLK.clk_lvds_in.clk_invert = 0 ;
		pCTRL->stISPCLK.clk_lvds_in.clk_delay = 1 ;
	#else
		pCTRL->stISPCLK.clk_lvds_in.clk_invert = 0 ;
		pCTRL->stISPCLK.clk_lvds_in.clk_delay = 0 ;
	#endif
#endif

#if defined (__USE_ISP_SCLK_OUT__)
	// If __USE_ISP_SCLK_OUT__ is used : Sensor board does not have OSC
	#if defined(__USE_CMOS_IMXx36_TTL__) | defined(__USE_CMOS_AR0331_TTL__)	| \
		defined(__USE_CMOS_IMX076__) | defined(__USE_CMOS_PS1210K__) | \
		defined(__USE_CMOS_PO3100K__) | defined(__USE_CMOS_MT9P031__) | \
		defined(__USE_CMOS_IMXx22__) | defined(__USE_CMOS_EXT3__) | defined(__USE_CMOS_EXT2__) | \
		defined(__USE_CMOS_EXT1__) |defined(__USE_CMOS_OV2710__) | defined (__USE_CMOS_IMX323__) |  defined (__USE_CMOS_AR0134_TTL__)
		pCTRL->stISPCLK.clk_sensor_in.clk_src_select = 1 ; // 
			// [¡®00¡¯ : CLK_B is used] : HDMI CLK
			// [¡®01¡¯ : CLK_A is used] : TTL CLK
			// [¡®10¡¯ : refer to stISPCLK.clk_lvds_sel] 
			// [¡®11¡¯ : LVDS serial clk divider clk is used] : See stISPCLK.clk_sensor_in.clk_divider
	#else
		#if defined(__USE_CMOS_IMX290_LVDS_S8__)  ||\
			defined(__USE_IMX273_720P240_MODE__)  || (defined(__USE_IMX273_720P120_MODE__) && defined(__USE_IMX273_ALLSCAN_MODE__))
			pCTRL->stISPCLK.clk_sensor_in.clk_src_select = 2 ; // LVDS 8ch/IMX265 must use pll 
		#else
			pCTRL->stISPCLK.clk_sensor_in.clk_src_select = 3 ; //
		#endif	
			// [¡®00¡¯ : CLK_B is used] : HDMI CLK
			// [¡®01¡¯ : CLK_A is used] : TTL CLK
			// [¡®10¡¯ : refer to stISPCLK.clk_lvds_sel] 
			// [¡®11¡¯ : LVDS serial clk divider clk is used] : See stISPCLK.clk_sensor_in.clk_divider
	#endif 
		pCTRL->stISPCLK.clk_lvds_sel = 0 ; // x49[0]
			// [¡®0¡¯ : User PLL CLK is used] 
			// [¡®1¡¯ : LVDS serial clk is used]
			
	#if defined (__USE_CMOS_IMXx36_LVDS_S2__) || defined (__USE_CMOS_AR0331_LVDS_S2__)	|| defined (__USE_CMOS_AR0134_LVDS_S2__)	
		pCTRL->stISPCLK.clk_sensor_in.clk_divider = 1 ;  // clock divider for clk_sensor_in clk
			// [ ¡®00¡¯ : 1/1 ] 
			// [ ¡®01¡¯ : 1/3 ] 
			// [ ¡®10¡¯ : 1/2 ] 
			// [ ¡®11¡¯ : 2/3 ] 
			
	#elif defined(__USE_CMOS_IMX326__)
		pCTRL->stISPCLK.clk_sensor_in.clk_divider = 0 ;  // clock divider for clk_sensor_in clk	
	#elif defined(__USE_CMOS_IMX226__)
		#if defined(__USE_IMX226_1080P60_MODE__)
		pCTRL->stISPCLK.clk_sensor_in.clk_divider = 3 ;  // clock divider for clk_sensor_in clk
		#else
		pCTRL->stISPCLK.clk_sensor_in.clk_divider = 0 ;  // clock divider for clk_sensor_in clk
		#endif	
	#elif defined(__USE_CMOS_IMX274__)	
		#if defined(__USE_IMX274_1080P60_MODE__)
		pCTRL->stISPCLK.clk_sensor_in.clk_divider = 3 ;  // clock divider for clk_sensor_in clk
		#else
		pCTRL->stISPCLK.clk_sensor_in.clk_divider = 0 ;  // clock divider for clk_sensor_in clk
		#endif
	#elif defined(__USE_CMOS_IMX334__)
		pCTRL->stISPCLK.clk_sensor_in.clk_divider = 0 ;  // clock divider for clk_sensor_in clk		
	#elif defined(__USE_CMOS_IMX253__)
		pCTRL->stISPCLK.clk_sensor_in.clk_divider = 0 ;  // clock divider for clk_sensor_in clk
	#elif defined(__USE_CMOS_IMX273__)
		#if defined(__USE_CMOS_IMX273_LVDS_S2__)
		pCTRL->stISPCLK.clk_sensor_in.clk_divider = 1 ;  // clock divider for clk_sensor_in clk	
		#else
		pCTRL->stISPCLK.clk_sensor_in.clk_divider = 3 ;  // clock divider for clk_sensor_in clk		
		#endif
	#else
		pCTRL->stISPCLK.clk_sensor_in.clk_divider = 3 ;  // clock divider for clk_sensor_in clk
	#endif

	#if defined(__USE_CMOS_EXT2__)
		pCTRL->stISPCLK.clk_sensor_in.clk_invert = 1 ; //
	#else
		pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
	#endif
	
	#if defined(__USE_MDIN_i500__) 						// MDIN-i500
		pCTRL->stISPCLK.clk_sensor_in.clk_delay = 0 ; //
	#else 												// MDIN-i51X (i510, i540, i550)
		#if defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__)
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 2 ; //
		#elif defined(__USE_CMOS_IMX226__) && (defined(__MDIN_i550_REF_V121__) || defined(__MDIN_i540_REF_V121__) || defined(__MDIN_i540_4K60__) || defined(__MDIN_i550_USB_V100__))
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 2 ; //		
		#else
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 1 ; //
		#endif
	#endif
		
#else
	// If __USE_ISP_SCLK_OUT__ is not used : Sensor board has OSC
	#if defined(__USE_CMOS_IMXx36_TTL__) | defined(__USE_CMOS_AR0331_TTL__)	| \
		defined(__USE_CMOS_IMX076__) | defined(__USE_CMOS_PS1210K__) | \
		defined(__USE_CMOS_PO3100K__) | defined(__USE_CMOS_MT9P031__) | \
		defined(__USE_CMOS_IMXx22__) | defined(__USE_CMOS_EXT3__) | defined(__USE_CMOS_EXT2__) | defined(__USE_CMOS_EXT1__) | \
		defined(__USE_CMOS_OV2710__) | defined (__USE_CMOS_IMX323__) | defined(__USE_CMOS_AR0134_TTL__) | \
		defined(__USE_CMOS_IMX385_CSI2TTL__)  // IMX385 CSI2 to TTL
		pCTRL->stISPCLK.clk_sensor_in.clk_src_select = 1 ; //
			// [¡®00¡¯ : CLK_B is used] : HDMI CLK
			// [¡®01¡¯ : CLK_A is used] : TTL CLK
			// [¡®10¡¯ : refer to stISPCLK.clk_lvds_sel] 
			// [¡®11¡¯ : LVDS serial clk divider clk is used] : See stISPCLK.clk_sensor_in.clk_divider
	#else
	#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		#if defined(__USE_CMOS_IMX290_LVDS_S8__)|| defined(__USE_CMOS_IMX265__) || defined(__USE_IMX290_720P120_MODE__) || defined(__USE_IMX273_720P240_MODE__)
			pCTRL->stISPCLK.clk_sensor_in.clk_src_select = 2 ; // LVDS 8ch/IMX265 must use pll 
		#else
			pCTRL->stISPCLK.clk_sensor_in.clk_src_select = 3 ; //
		#endif
	#else
		pCTRL->stISPCLK.clk_sensor_in.clk_src_select = 2 ; //
	#endif
			// [¡®00¡¯ : CLK_B is used] : HDMI CLK
			// [¡®01¡¯ : CLK_A is used] : TTL CLK
			// [¡®10¡¯ : refer to stISPCLK.clk_lvds_sel] 
			// [¡®11¡¯ : LVDS serial clk divider clk is used] : See stISPCLK.clk_sensor_in.clk_divider
	#endif 
		pCTRL->stISPCLK.clk_lvds_sel = 0 ; // // clk_lvds_int
			// [¡®0¡¯ : User PLL CLK is used] 
			// [¡®1¡¯ : LVDS serial clk is used]
			
	#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		#if defined(__USE_CMOS_IMX326__)
			pCTRL->stISPCLK.clk_sensor_in.clk_divider = 0 ;  // clock divider for clk_sensor_in serial clk -- 8ch : use lvds clock, 6ch : use clock divider 1/1
		#elif defined(__USE_CMOS_IMX226__) 
			#if defined(__USE_IMX226_1080P60_MODE__)
				pCTRL->stISPCLK.clk_sensor_in.clk_divider = 3 ;  // clock divider for clk_sensor_in serial clk -- 4ch : imx274 mode2
			#else
				pCTRL->stISPCLK.clk_sensor_in.clk_divider = 0 ;  // clock divider for clk_sensor_in serial clk -- 8ch : use lvds clock, 6ch : use clock divider 1/1
			#endif		
		#elif defined(__USE_CMOS_IMX253__) 		
			pCTRL->stISPCLK.clk_sensor_in.clk_divider = 0 ;  // clock divider for clk_sensor_in serial clk -- 8ch : use lvds clock, 8ch : use clock divider 1/1
		#elif defined(__USE_CMOS_IMX273__) 		
			pCTRL->stISPCLK.clk_sensor_in.clk_divider = 3 ;   // clock divider for clk_sensor_in serial clk -- 4ch : use clock divider 2/3	
		#elif defined(__USE_CMOS_IMX274__) 
			#if defined(__USE_IMX274_1080P60_MODE__)
				pCTRL->stISPCLK.clk_sensor_in.clk_divider = 3 ;  // clock divider for clk_sensor_in serial clk -- 4ch : imx274 mode2
			#else
				pCTRL->stISPCLK.clk_sensor_in.clk_divider = 0 ;  // clock divider for clk_sensor_in serial clk -- 8ch : use lvds clock, 6ch : use clock divider 1/1
			#endif
		#elif defined(__USE_CMOS_IMX249__) || defined(__USE_CMOS_AR0134_LVDS_S2__)
			pCTRL->stISPCLK.clk_sensor_in.clk_divider = 1 ;  // clock divider for clk_sensor_in serial clk -- 2ch : use clock divider 1/3	
		#elif defined(__USE_CMOS_IMX385_CSI2LVDS6CH__) || defined(__USE_CMOS_IMX334__)
			pCTRL->stISPCLK.clk_sensor_in.clk_divider = 0 ;
		#else
			pCTRL->stISPCLK.clk_sensor_in.clk_divider = 3 ;  // clock divider for clk_sensor_in serial clk -- 4ch : use clock divider 2/3
		#endif
	#else		// i500
		pCTRL->stISPCLK.clk_sensor_in.clk_divider = 0 ;  // clock divider for clk_sensor_in serial clk
			// [ ¡®00¡¯ : 1/1 ] 
			// [ ¡®01¡¯ : 1/3 ] 
			// [ ¡®10¡¯ : 1/2 ] 
			// [ ¡®11¡¯ : 2/3 ] 
	#endif
	
	#if defined(__USE_CMOS_EXT2__)	||	defined(__USE_CMOS_EXT1__)
		pCTRL->stISPCLK.clk_sensor_in.clk_invert = 1 ; //
		pCTRL->stISPCLK.clk_sensor_in.clk_delay = 0 ; //		
	#elif defined(__USE_CMOS_IMX322__) || defined(__USE_CMOS_IMX323__) 
		#if defined(__MDIN_i510_REF__) || defined(__MDIN_i540_REF__) || defined(__MDIN_i550_REF__)
		pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
		pCTRL->stISPCLK.clk_sensor_in.clk_delay = 0 ; //		
		#else
		pCTRL->stISPCLK.clk_sensor_in.clk_invert = 1 ; //
		pCTRL->stISPCLK.clk_sensor_in.clk_delay = 3 ; //		
		#endif
	#elif defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__)  // for 4k sensor slave mode
		#if defined(__MDIN_i510_REF__) || defined(__MDIN_i540_REF__) || defined(__MDIN_i550_REF__) || \
			defined(__MDIN_i510_IMX290__) || defined(__MDIN_i540_4K60__)
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 2 ; //
		#else
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 0 ; //
		#endif
	#elif defined(__USE_CMOS_IMX290__) || defined(__USE_CMOS_IMX327__)
		#if defined(__MDIN_i510_EDK__) || defined(__MDIN_i540_EDK__) || defined(__MDIN_i550_EDK__)
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 2 ; // socket b/d
		#elif defined(__MDIN_i510_REF__) || defined(__MDIN_i510_IMX290__)	// hulee added on 20161011
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 1 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 1 ; // jins 20161101, "100" seems good but actually odm data halts for the 60hz(wdr) mode.
		#else
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 2 ; // sachem04 20161101 1-->2 some our chip 1 ok, other ok 0, amost ok 2
		#endif
	#elif defined(__USE_CMOS_IMX185__)
		#if defined(__MDIN_i510_EDK__) || defined(__MDIN_i540_EDK__) || defined(__MDIN_i550_EDK__)
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 2 ; // socket b/d
		#else
			#if defined(__MDIN_i510_REF__)			// test end sample chip
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 2 ; //
			#else
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 1 ; //
			#endif
		#endif
	#elif defined(__USE_CMOS_IMXx36__)
		#if defined(__MDIN_i510_EDK__) || defined(__MDIN_i540_EDK__) || defined(__MDIN_i550_EDK__)
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 3 ; // socket b/d
		#else
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 0 ; //
		#endif
	#elif defined(__USE_CMOS_IMX124__)
		#if defined(__MDIN_i510_EDK__) || defined(__MDIN_i540_EDK__) || defined(__MDIN_i550_EDK__)
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 1 ; // socket b/d
		#else
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 1 ; //
		#endif
	#elif defined(__USE_CMOS_MN34420__)
		#if defined(__MDIN_i540_REF__)
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 2 ; //
		#elif defined(__MDIN_i510_REF__)
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 1 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 1 ; //			
		#else
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 0 ; //
		#endif
	#elif defined(__USE_CMOS_IMX249__)
		#if defined(__MDIN_i510_REF__) || defined(__MDIN_i540_REF__) || defined(__MDIN_i550_REF__)
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 2 ; //
		#else
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 0 ; //
		#endif
	#elif defined(__USE_CMOS_IMX265__) || defined(__USE_CMOS_IMX252__) 
		#if defined(__MDIN_i510_REF_V10__)	//test : samplie chip
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 2 ; //
		#elif defined(__MDIN_i540_REF_V121__)		//test : samplie chip	
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 1 ; //		
		#else
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 0 ; //
		#endif	
	#elif defined(__USE_CMOS_PS1400K__)
		#if defined(__MDIN_i540_REF_V121__)		//test : samplie chip	
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 2 ; //		
		#else
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 0 ; //
		#endif	
	#elif defined(__USE_CMOS_AR0134__) || defined(__USE_CMOS_AR0135__)
			pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
			pCTRL->stISPCLK.clk_sensor_in.clk_delay = 3 ; //	temp for sensor pcb ver 1.0
	#else
		pCTRL->stISPCLK.clk_sensor_in.clk_invert = 0 ; //
		pCTRL->stISPCLK.clk_sensor_in.clk_delay = 0 ; //		
	#endif

#endif	
	
#if defined(__USE_MDIN_i5XX_REV__)
	pCTRL->stISPCLK.clk_bayer.clk_src_select = 3 ; //
		// [ ¡®00¡¯ : clk_sensor_in is used ]
		// [ ¡®01¡¯ : (invert)(clk_sensor_in)/2 is used ]
		// [ ¡®10¡¯ : (invert)(clk_sensor_in)/4 is used ]
		// [ ¡®11¡¯ : (clk_sensor_in)/(clk_bayer.clk_divider) is used ]		
		
	pCTRL->stISPCLK.clk_bayer.clk_divider = 1 ;
		// When stISPCLK.clk_bayer.clk_src_select = ¡®11¡¯
		// [ ¡®000¡¯ : Not allowed ]
		// [ ¡®001¡¯ : clk_sensor_in/1 is used]
		// [ ¡®N¡¯ : clk_sensor_in/N is used]			
#else
	pCTRL->stISPCLK.clk_bayer.clk_src_select = 4 ; //
		// [ ¡®000¡¯ : clk_sensor_in is used ]
		// [ ¡®001¡¯ : (invert)(clk_sensor_in)/2 is used ]
		// [ ¡®010¡¯ : (invert)(clk_sensor_in)/4 is used ]
		// [ ¡®011¡¯ : (invert)(clk_sensor_in/2)/2 is used ]
		// [ ¡®100¡¯ : (clk_sensor_in)/(clk_bayer.clk_divider) is used ]
		
	pCTRL->stISPCLK.clk_bayer.clk_divider = 1 ;
		// When stISPCLK.clk_bayer.clk_src_select = ¡®100¡¯
		// [ ¡®000¡¯ : Not allowed ]
		// [ ¡®001¡¯ : clk_sensor_in/1 is used]
		// [ ¡®N¡¯ : clk_sensor_in/N is used]			
#endif		
	
	
	#if defined(__PANORAMA_SENSOR_SETUP__)
		pCTRL->stISPCLK.clk_bayer.clk_invert = 1 ;
	#else
		pCTRL->stISPCLK.clk_bayer.clk_invert = 0 ;
	#endif
	pCTRL->stISPCLK.clk_bayer.clk_delay = 0 ; 
	
#if defined(__USE_MDIN_i5XX_REV__)
	pCTRL->stISPCLK.clk_odm_int.clk_src_select = 1 ; 
		// [ ¡®00¡¯ : clk_bayer is used ] 		// For Normal or WDR Line mode
		// [ ¡®01¡¯ : clk_sensor_in is used ]	// For WDR Frame mode	
		// [ ¡®10¡¯ : clk_bayer/clk_odm_int.clk_divider is used ]	// For WDR Frame mode	
		// [ ¡®11¡¯ : prohibited ]	
	
	pCTRL->stISPCLK.clk_odm_int.clk_divider = 1 ; 
		//  [ ¡®N¡¯ : clk_odm_int.clk_src_select/N is used]	// 1~7	
#else
	pCTRL->stISPCLK.clk_odm_int.clk_src_select = 1 ; 
		// [ ¡®0¡¯ : clk_bayer is used ] 		// For Normal or WDR Line mode
		// [ ¡®1¡¯ : clk_sensor_in is used ]	// For WDR Frame mode
	pCTRL->stISPCLK.clk_odm_int.clk_divider = 0 ; 
		//  clk_odm_int.clk_divider will not work : only for i5x0rev
#endif	

			 
	pCTRL->stISPCLK.clk_nr.clk_src_select = 0 ; 
		// [ ¡®0¡¯ : clk_bayer is used ]
		// [ ¡®1¡¯ : clk_yc_m is used ]
		// Check "stISPCLK.clk_yc.clk_divider" value when using clk_yc_m clk to clk_nr

	#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) // i510, i540, i550
	#if defined(__USE_MDIN_i5XX_REV__)
		#if defined(__MDIN_i540_4K60__) && defined(__USE_ISP_SCLK_OUT__)
//			pCTRL->stISPCLK.clk_slave.clk_src_select = 2 ; // Clk for sensor slave mode 2017.08.09 hongsi
			pCTRL->stISPCLK.clk_slave.clk_src_select = 6 ; // Clk for sensor slave mode 2017.08.09 hongsi
		#else
			#if (defined(__USE_CMOS_IMX265__) && !defined(__USE_ISP_SCLK_OUT__)) || (defined(__USE_CMOS_IMX334__) && defined(__USE_IMX334_SLAVE_MODE__))
				pCTRL->stISPCLK.clk_slave.clk_src_select = 0 ; // Clk for sensor slave mode 2017.12.18 jhs : xtal
			#else
				pCTRL->stISPCLK.clk_slave.clk_src_select = 6 ; // Clk for sensor slave mode 2017.03.02 jhs
			#endif
		#endif
		// [¡®000¡¯ : X_tal is used]
		// [¡®001¡¯ : X_tal2 is used]
		// [¡®01x¡¯ : host_clk is used]	
		// [¡®100¡¯ : CLK_A is used]
		// [¡®101¡¯ : CLK_B is used]
		// [¡®11x¡¯ : LVDS serial clk is used]	
	#endif // __USE_MDIN_i5XX_REV__
	#endif	
	
	pCTRL->stISPCLK.clk_nr.clk_invert = 0 ; 
	pCTRL->stISPCLK.clk_nr.clk_delay = 0 ; 

#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	pCTRL->stISPCLK.clk_yc.clk_src_select = 2 ; 
		// [¡®00¡¯ : CLK_B is used] : HDMI CLK
		// [¡®01¡¯ : CLK_A is used] : TTL CLK
		// [¡®1x¡¯ : clk_bayer is used]
#else 								// MDIN-i51X (i510, i540, i550)
	pCTRL->stISPCLK.clk_yc_tm.clk_src_select = 0 ;
		// [¡®0¡¯ : CLK_B is used] : HDMI CLK
		// [¡®1¡¯ : CLK_A is used] : TTL CLK	
	
	pCTRL->stISPCLK.clk_yc_tm.clk_invert = 0 ;
	pCTRL->stISPCLK.clk_yc_tm.clk_delay = 0 ;

	pCTRL->stISPCLK.clk_yc.clk_src_select = 2 ; 
		// [¡®00¡¯ : clk_yc_tm is used]
		// [¡®01¡¯ : clk_yc_tm/2 is used]
		// [¡®1x¡¯ : clk_bayer is used] 
#endif
	
	pCTRL->stISPCLK.clk_yc.clk_divider = 0 ; 
		// Valid only when [clk_yc.clk_src_select = ¡®0x¡¯]
		// Need for 656 video input
		// Divider clock does not effect 'clk_yc'.
		// Divider clock effect 'clk_yc_m'.
		// [¡®0¡¯ : clk_yc_m is clk_yc/1 ] 
		// [¡®1¡¯ : clk_yc_m is clk_yc/2 ] 		
	
	// pCTRL->stISPCLK.clk_yc.clk_invert = 0 ; 
#if defined(__USE_MDIN_i550__)
	pCTRL->stISPCLK.clk_yc.clk_invert = clk_inv_m[1] ; 
	pCTRL->stISPCLK.clk_yc.clk_delay = clk_dly_m[1] ;
#else
	pCTRL->stISPCLK.clk_yc.clk_invert = clk_inv_m[0] ; 
	pCTRL->stISPCLK.clk_yc.clk_delay = clk_dly_m[0] ;	
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	pCTRL->stISPCLK.clk_yc_m.clk_src_select = 0 ; 
		// Valid only when "clk_yc.clk_src_select" is not 2
		// [¡®00¡¯ : clk_yc is used]
		// [¡®01¡¯ : User PLL CLK is used]
		// [¡®1x¡¯ : video PLL CLK is used] 	
#endif
	pCTRL->stISPCLK.clk_yc_m.clk_invert = 0 ; 
	pCTRL->stISPCLK.clk_yc_m.clk_delay = 0 ; 
	
//	pCTRL->stISPCLK.clk_dis.clk_src_select = 0 ; 		// DIS clk. kw5210
	pCTRL->stISPCLK.clk_dis.clk_src_select = 1 ; 
		// [¡®0¡¯ : clk_yc_m is used ] // Check "stISPCLK.clk_yc.clk_divider" value when using clk_yc_m clk to clk_dis
		// [¡®1¡¯ : refer to stISPCLK.clk_dis.clk_divider value ]
		
//	pCTRL->stISPCLK.clk_dis.clk_divider = 1 ; 			// DIS clk. kw5210
	pCTRL->stISPCLK.clk_dis.clk_divider = 12 ; 
		// Valid only when stISPCLK.clk_dis.clk_src_select = ¡®1¡¯
		// [ ¡®0000¡¯ : Not allowed ]
		// [ ¡®0001¡¯ : mclk/1 is used]
		// [ ¡®N¡¯ : mclk/N is used]	

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// Sub lvds mode setup
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------		
#if defined(__USE_EXT2IPP__)
	pCTRL->stISPCLK.clk_sensor_in.clk_src_select = 0;		// [¡®00¡¯ : CLK_B is used] : HDMI CLK
	pCTRL->sub_lvds_disable = 0x1FFF  ; // sub_lvds ports all disabled 
#else
	if ( pCTRL->stISPCLK.clk_sensor_in.clk_src_select > 1 )		pCTRL->sub_lvds_disable = 0x0000 ; // sub_lvds ports all enabled		
	else 												pCTRL->sub_lvds_disable = 0x1FFF  ; // sub_lvds ports all disabled 
#endif	
	
// ----------------------------------------------------------------------	
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// Main video clock setup
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------	
#if defined(__USE_MDIN_i550__) && defined(__USE_MDIN_i5XX_REV__)
	pCTRL->stVIDEOCLK.clk_p0.clk_invert = 0 ;
	pCTRL->stVIDEOCLK.clk_p0.clk_delay = 0 ;

	pCTRL->stVIDEOCLK.clk_p1.clk_invert = 1 ;
	pCTRL->stVIDEOCLK.clk_p1.clk_delay = 0 ;	
#endif	
	
	pCTRL->stVIDEOCLK.vclk_out_diable = 0 ;
		// [¡®0¡¯ : enable vclk_out & vclk_out_b pin of i500]
		// [¡®1¡¯ : disable vclk_out & vclk_out_b pin of i500]
	
#if defined(__USE_MDIN_i5XX_REV__)	
	pCTRL->stVIDEOCLK.vclk.clk_src_select = 0 ; // vclk_bypass_sel
		// [¡®xx0¡¯ : VPLL out clock is used] : VPLL CLK
		// [¡®001¡¯ : CLK_B is used] : HDMI CLK
		// [¡®011¡¯ : CLK_A is used] : TTL CLK
		// [¡®101¡¯ : refer to stISPCLK.clk_lvds_sel value] // clk_lvds_int
		// [¡®111¡¯ : LVDS serial clk is used]		
#else
	pCTRL->stVIDEOCLK.vclk.clk_src_select = 0 ; // vclk_bypass_sel
		// [¡®0¡¯ : from video PLL output]
		// [¡®1¡¯ : refer to stVPLL.pll_sel_inclk value]
		
			// stVPLL.pll_sel_inclk
		// [¡®00¡¯ : CLK_B is used] : HDMI CLK
		// [¡®01¡¯ : CLK_A is used] : TTL CLK
		// [¡®10¡¯ : refer to stISPCLK.clk_lvds_sel value] // clk_lvds_int
		// [¡®11¡¯ : LVDS serial clk is used]
#endif	
		
	pCTRL->stVIDEOCLK.vclk.clk_divider = 0 ;
		// [¡®00¡¯ : video clock (from PLL or port A/B)] : HDMI CLK/TTL CLK
		// [¡®01¡¯ : video clock divided by 2]
		// [¡®10¡¯ : video clock divided by 4] 
		// [¡®11¡¯ : mpll output clock source clock divider output clock is used. 
		//      refer to stAUXCLK.clk_aux_mclk.clk_divide_param.clk_divider value ] ////clk_aux_mclk_int
		
	pCTRL->stVIDEOCLK.vclk.clk_invert = 0 ;
	pCTRL->stVIDEOCLK.vclk.clk_delay = 0 ;

#if defined(__USE_USB_VOUT__)
	pCTRL->stVIDEOCLK.vclk_out.clk_divider = 1 ;	// for main(dual wide out) v.out clk
#else
	pCTRL->stVIDEOCLK.vclk_out.clk_divider = 0 ;
#endif
		// [¡®00¡¯ : video clock (from PLL or port A/B)]
		// [¡®01¡¯ : video clock divided by 2]
		// [¡®10¡¯ : video clock divided by 4]
		// [¡®11¡¯ : video clock divided by 8]	
		
	pCTRL->stVIDEOCLK.vclk_out.clk_src_select = 0 ;
		// [¡®0¡¯ : vclk.clk_divider output clock is used]
		// [¡®1¡¯ : refer to clk_aux_out.clk_src_select value] // clk_aux_out
		
#if defined(__USE_LVDSTX_EVEN_ODD__	) || defined(__USE_USB_VOUT__)
	pCTRL->vclk_out_gen_sync=1;		//Synchronization mode of main video output clock generator (vclk_out)
		// [¡®0¡¯ : free running]
		// [¡®1¡¯ : locked to horizontal sync] 	//need to dual wide mode	
#else
	pCTRL->vclk_out_gen_sync=0;		
#endif
	pCTRL->vclk_out_gen_pload=0;		//Preload value to the main video output clock divider This is valid only when vclk_out_gen_sync is '1'.

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	pCTRL->stVIDEOCLK.vclk_out.clk_invert = 0 ;
	pCTRL->stVIDEOCLK.vclk_out.clk_delay = 0 ;
#else 												// MDIN-i51X (i510, i540, i550)
#if defined(__USE_SIL9136_TX__)
	pCTRL->stVIDEOCLK.vclk_out.clk_invert = 0 ;
	pCTRL->stVIDEOCLK.vclk_out.clk_delay = 2 ;
#else
	pCTRL->stVIDEOCLK.vclk_out.clk_invert = 1 ;	// for 2160p kw5210
	pCTRL->stVIDEOCLK.vclk_out.clk_delay = 0 ;
#endif
#endif

#if defined(__USE_CMOS_AUX_SYNC_OUT__) && (defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__)) // Use AUX sync until i5x0 revision for Sensor Slave Mode, UHD
	pCTRL->stVIDEOCLK.vclk_out_b.clk_src_select = 1 ;	// Use AUX sync until i5x0 revision for Sensor Slave Mode
#elif defined(__MDIN_i550_USB_V100__)
	pCTRL->stVIDEOCLK.vclk_out_b.clk_src_select = 1 ;
#else
 	pCTRL->stVIDEOCLK.vclk_out_b.clk_src_select = 0 ;
#endif
		// [¡®0¡¯ : vclk.clk_divider output clock is used]
		// [¡®1¡¯ : refer to clk_aux_out.clk_src_select value] // clk_aux_out
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	pCTRL->stVIDEOCLK.vclk_out_b.clk_invert = 0 ;
	pCTRL->stVIDEOCLK.vclk_out_b.clk_delay = 0 ;	
#else 								// MDIN-i51X (i510, i540, i550)
#if defined(__USE_SIL9136_TX__)
	pCTRL->stVIDEOCLK.vclk_out_b.clk_invert = 0 ;
	pCTRL->stVIDEOCLK.vclk_out_b.clk_delay = 2 ;
#else
	pCTRL->stVIDEOCLK.vclk_out_b.clk_invert = 1 ;
	pCTRL->stVIDEOCLK.vclk_out_b.clk_delay = 0 ;
#endif
#endif

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	#if defined(__USE_LVDSTX_EVEN_ODD__	) || defined(__USE_USB_VOUT__)
	pCTRL->stVIDEOCLK.vclk_out_i.clk_src_select = 1 ;
	// [¡®00¡¯ : vclk.clk_divider output clock is used]
	// [¡®01¡¯ : vclk.clk_divider output clock/2 is used]
	// [¡®10¡¯ : vclk.clk_divider output clock/4 is used]
	// [¡®11¡¯ : video clock (from PLL or port A/B) is used]
	#else
	pCTRL->stVIDEOCLK.vclk_out_i.clk_src_select = 0 ;
	#endif

#if defined(__USE_EXT_PLL_IC__)
#if defined(__USE_MDIN_i5XX_REV__)
#if defined(__MDIN_i540_4K60__)
	pCTRL->stVIDEOCLK.vclk_out_i.clk_invert = 0 ;	// v.out data delay : ext.pll --> i540_4k60 --> hdtx
	pCTRL->stVIDEOCLK.vclk_out_i.clk_delay = 0 ;
#else
	pCTRL->stVIDEOCLK.vclk_out_i.clk_invert = 1 ;	// v.out data delay : ext.pll --> i5x0 & hdtx
	pCTRL->stVIDEOCLK.vclk_out_i.clk_delay = 2 ;
#endif	// __MDIN_i540_4K60__
#else
	pCTRL->stVIDEOCLK.vclk_out_i.clk_invert = 1 ;	// v.out data delay : ext.pll --> i5x0 & hdtx
	pCTRL->stVIDEOCLK.vclk_out_i.clk_delay = 0 ;
#endif	// __USE_MDIN_i5XX_REV__
#else
	pCTRL->stVIDEOCLK.vclk_out_i.clk_invert = 0 ;
	pCTRL->stVIDEOCLK.vclk_out_i.clk_delay = 0 ;
#endif	// __USE_EXT_PLL_IC__
#endif 	
	
	// clk_m_enc_int is one of the source of clk_enc_int.
	pCTRL->stVIDEOCLK.clk_m_enc_int.clk_src_select = 0 ;	 	
		// [¡®00¡¯ : CLK_B is used] : HDMI CLK
		// [¡®01¡¯ : CLK_A is used] : TTL CLK
		// [¡®1x¡¯ : clk_bayer is used]	
		
	pCTRL->stVIDEOCLK.clk_m_enc_int.clk_divider = 0 ;	 	
		// Valid only when stVIDEOCLK.clk_m_enc_int.clk_src_select = ¡®0x¡¯
		// [¡®0¡¯ : (clk_m_enc_int.clk_src_select source)/1]
		// [¡®1¡¯ : (clk_m_enc_int.clk_src_select source)/2] 
	
	// clk_enc_int is for internal video encoder and DAC. This clock split to encdac_clk&clk_in_enc
 	pCTRL->stVIDEOCLK.clk_enc_int.clk_src_select = 0 ;
		// This value will determine source of encdac_clk&clk_in_enc 
		// [¡®00¡¯ : refer to stVIDEOCLK.vclk.clk_src_select&stVIDEOCLK.vclk.clk_divide value]
		// [¡®01¡¯ : X_tal is used]
		// [¡®10¡¯ : refer to clk_m_enc_int.clk_src_select value]
		// [¡®11¡¯ : X_tal2 is used]		
	
	// DAC video output clk
	pCTRL->stVIDEOCLK.DAC_video_power_down = 0 ;	// Video encoder DAC power-down mode
		// [¡®0¡¯ : normal operation]
		// [¡®1¡¯ : power-down mode] 
	pCTRL->stVIDEOCLK.encdac_clk.clk_invert = 0 ;
	pCTRL->stVIDEOCLK.encdac_clk.clk_delay = 0 ;		
	
	// NTSC encoder clk	
	pCTRL->stVIDEOCLK.clk_in_enc.clk_invert = 0 ;
	pCTRL->stVIDEOCLK.clk_in_enc.clk_delay = 0 ;

// ----------------------------------------------------------------------	
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// AUX video clock setup
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------	
		
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	pCTRL->stAUXCLK.clk_m_aux_int.clk_src_select = 0 ; 		
		// clk_m_aux_int.clk_src_select effects clock source of 'clk_in_aux', 'clk_aux_in', 'clk_aux_mid', 'clk_aux_out' 
		// [¡®00¡¯ : CLK_B is used] : HDMI CLK
		// [¡®01¡¯ : CLK_A is used] : TTL CLK
		// [¡®1x¡¯ : clk_bayer is used ]
#else 								// MDIN-i51X (i510, i540, i550)
	pCTRL->stAUXCLK.clk_in_aux_tm.clk_src_select = 0 ; 	
		// clk_m_aux_int.clk_src_select effects clock source of 'clk_in_aux', 'clk_aux_in', 'clk_aux_mid', 'clk_aux_out' 
		// [¡®0¡¯ : CLK_B is used] : HDMI CLK
		// [¡®1¡¯ : CLK_A is used] : TTL CLK	
	
	pCTRL->stAUXCLK.clk_in_aux_tm.clk_invert = 0 ;
	pCTRL->stAUXCLK.clk_in_aux_tm.clk_delay = 0 ;
	
	pCTRL->stAUXCLK.clk_m_aux_int.clk_src_select = 2 ; 		
		// clk_m_aux_int.clk_src_select effects clock source of 'clk_in_aux', 'clk_aux_in', 'clk_aux_mid', 'clk_aux_out' 
		// [¡®00¡¯ : clk_in_aux_tm is used]
		// [¡®01¡¯ : clk_in_aux_tm/2 is used]
		// [¡®1x¡¯ : clk_bayer is used ]		
#endif	
	
	pCTRL->stAUXCLK.clk_m_aux_int.clk_divider = 0 ; 
		// Valid only when [clk_m_aux_int.clk_src_select = ¡®0x¡¯]
		// Divider clock does not effect 'clk_in_aux'.
		// Divider clock effect 'clk_aux_in', 'clk_aux_mid', 'clk_aux_out'.
		// [¡®0¡¯ : (clk_in_aux)/1 clk is used] 
		// [¡®1¡¯ : (clk_in_aux)/2 clk is used] 
	
	
	// pCTRL->stAUXCLK.clk_in_aux.clk_invert = 0 ; 
#if defined(__USE_MDIN_i550__)	
	pCTRL->stAUXCLK.clk_in_aux.clk_invert = clk_inv_a[1] ; 
	pCTRL->stAUXCLK.clk_in_aux.clk_delay = clk_dly_a[1] ; 
#else
	pCTRL->stAUXCLK.clk_in_aux.clk_invert = clk_inv_a[0] ; 
	pCTRL->stAUXCLK.clk_in_aux.clk_delay = clk_dly_a[0] ; 	
#endif	
		// clk_in_aux is an AUX video clock to video input mux
		// Clock source of clk_in_aux is same as 'clk_m_aux_int.clk_src_select'
	
	// fractional divider by dual modulus	
	pCTRL->stAUXCLK.clk_aux_mclk.clk_divide_param.clk_division_mode = 0 ; 
		// ; division ratio
		// clk_division_mode = '0' : (clk_divider + clk_f/clk_t)
		// clk_division_mode = '1' : (clk_divider + (clk_t-clk_f)/clk_t)	
	pCTRL->stAUXCLK.clk_aux_mclk.clk_divide_param.clk_synchonize = 0 ; 
		// clk_synchonize = '0' : free running
		// clk_synchonize = '1' : synchonized to aux hsync		
	
	pCTRL->stAUXCLK.clk_aux_mclk.clk_divide_param.clk_divider = 13 ; 
	pCTRL->stAUXCLK.clk_aux_mclk.clk_divide_param.clk_t = 1 ; 
	pCTRL->stAUXCLK.clk_aux_mclk.clk_divide_param.clk_f = 1 ; 
#if defined(__USE_CMOS_AUX_SYNC_OUT__) && (defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__)) // Use AUX sync until i5x0 revision for Sensor Slave Mode, UHD
	pCTRL->stAUXCLK.clk_aux_mclk.clk_src_select = 2 ;	// Use AUX sync until i5x0 revision for Sensor Slave Mode
#elif defined(__MDIN_i550_USB_V100__)
	pCTRL->stAUXCLK.clk_aux_mclk.clk_src_select = 2 ;	// use user pll for aux v.out clk
#else
	pCTRL->stAUXCLK.clk_aux_mclk.clk_src_select = 0 ;
#endif
		// Clock source selection to auxiliary video clock 
		// [¡®00¡¯ : mpll output clock source clock divider output clock is used.] : clk_aux_mclk.clk_divide_param setting needed
		// [¡®01¡¯ : refer to stVIDEOCLK.vclk.clk_src_select value]
		// [¡®10¡¯ : User PLL CLK is used]
		// [¡®11¡¯ : X_tal2 is used]
	
			// stVIDEOCLK.vclk.clk_src_select value
		// [¡®0¡¯ : from video PLL output]
		// [¡®1¡¯ : refer to stVPLL.pll_sel_inclk value]		
	
	pCTRL->stAUXCLK.clk_aux_in.clk_src_select = 0 ;	
		// [¡®00¡¯ : refer to stAUXCLK.clk_m_aux_int.clk_src_select & stAUXCLK.clk_m_aux_int.clk_divider value] : clk_m_aux_int
		// [¡®01¡¯ : refer to stVIDEOCLK.vclk.clk_src_select & stVIDEOCLK.vclk.clk_divide value] : vclk_src
		// [¡®10¡¯ : refer to stVIDEOCLK.clk_enc_int.clk_src_select value] : clk_enc_int
		// [¡®11¡¯ : refer to stAUXCLK.clk_aux_mclk.clk_src_select] : clk_aux_mclk
		
	pCTRL->stAUXCLK.clk_aux_in.clk_invert = 0 ;		
	pCTRL->stAUXCLK.clk_aux_in.clk_delay = 0 ;		
	
	pCTRL->stAUXCLK.clk_aux_mid.clk_src_select = 0 ;
		// [¡®00¡¯ : refer to stAUXCLK.clk_m_aux_int.clk_src_select & stAUXCLK.clk_m_aux_int.clk_divider value] : clk_m_aux_int
		// [¡®01¡¯ : refer to stVIDEOCLK.clk_enc_int.clk_src_select value] : clk_enc_int
		// [¡®10¡¯ : refer to stVIDEOCLK.vclk.clk_src_select & stVIDEOCLK.vclk.clk_divide value] : vclk_src		
		// [¡®11¡¯ : refer to stAUXCLK.clk_aux_mclk.clk_src_select] : clk_aux_mclk	
	pCTRL->stAUXCLK.clk_aux_mid.clk_invert = 0 ;		
	pCTRL->stAUXCLK.clk_aux_mid.clk_delay = 0 ;		
	
	pCTRL->stAUXCLK.clk_aux_out.clk_src_select = 1 ;
		// [¡®00¡¯ : refer to stVIDEOCLK.vclk.clk_src_select & stVIDEOCLK.vclk.clk_divide value] : vclk_src
		// [¡®01¡¯ : refer to stVIDEOCLK.clk_enc_int.clk_src_select value] : clk_enc_int
		// [¡®10¡¯ : refer to stAUXCLK.clk_m_aux_int.clk_src_select & stAUXCLK.clk_m_aux_int.clk_divider value] : clk_m_aux_int				
		// [¡®11¡¯ : refer to stAUXCLK.clk_aux_mclk.clk_src_select] : clk_aux_mclk	
		
	pCTRL->stAUXCLK.clk_aux_out.clk_invert = 0 ;		
	pCTRL->stAUXCLK.clk_aux_out.clk_delay = 0 ;	

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// Memory clock setup
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------		
	pCTRL->stMEMCLK.mclk.clk_invert = 0 ;	
	pCTRL->stMEMCLK.mclk.clk_delay = 0 ;	
	
	pCTRL->stMEMCLK.ix2clko.clk_invert = 0 ;	
	pCTRL->stMEMCLK.ix2clko.clk_delay = 0 ;	
	
	pCTRL->stMEMCLK.icoreclk.clk_invert = 0 ;
#if defined(__USE_MDIN_i5XX_REV__)	
	pCTRL->stMEMCLK.icoreclk.clk_delay = 1 ;	// CHIP_REV_DDR_TUNE_201703 : ddr_clk_x_1 (400MHz clock to phy) delay control. (default is 0)
#else
	pCTRL->stMEMCLK.icoreclk.clk_delay = 0 ;
#endif
	//jins, temp
#if defined(__USE_2_INPUT_HDMI__)
	pCTRL->stISPCLK.clk_yc_tm.clk_src_select = 1 ;      //0x03E [13]   -- 0 : clk_b, 1:clk_a
	pCTRL->stAUXCLK.clk_in_aux_tm.clk_src_select = 1 ;  //0x03E [12]   -- 0 : clk_b, 1:clk_a
	pCTRL->stISPCLK.clk_sensor_in.clk_src_select = 0 ;  //0x048 [3:0]  -- 0 : clk_b_p hdmi input clock
	pCTRL->stISPCLK.clk_yc.clk_src_select = 0 ;         //0x049 [5:4]  -- 0 : clk_b_p hdmi or clk_a_p 
#endif

// ----------------------------------------------------------------------
// LVDS TX clock setup
// ----------------------------------------------------------------------
	#if defined(__USE_LVDSTX_MAIN_DUAL__) || defined(__USE_LVDSTX_EVEN_ODD__)
	pCTRL->stLVDSTXCLK.lvdstx_clk_sel = 1;		// 1 : use lvds tx , 0 : bypass(digital output)	
	pCTRL->stLVDSTXCLK.lvdstx_pll_clk_sel = 2;			// single = '1', dual = '2' , even_odd = '2' other is prohibit
	#elif defined(__USE_LVDSTX_MAIN_SINGLE__)
	pCTRL->stLVDSTXCLK.lvdstx_clk_sel = 1;		// 1 : use lvds tx , 0 : bypass(digital output)	
	pCTRL->stLVDSTXCLK.lvdstx_pll_clk_sel = 1;			// single = '1', dual = '2' other is prohibit
	#else
	pCTRL->stLVDSTXCLK.lvdstx_clk_sel = 0;		// 1 : use lvds tx , 0 : bypass(digital output)	
	pCTRL->stLVDSTXCLK.lvdstx_pll_clk_sel = 1;			// single = '1', dual = '2' other is prohibit
	#endif

	pCTRL->stLVDSTXCLK.clk_lvdstx_divider = 7;			
	pCTRL->stLVDSTXCLK.lvdstx_pll_clk_dly_sel = 0;	
}

static void Init_HostCLK_REGISTER( PHostCLK_CTRL_PARAM pCTRL ) 
{
	BYTE bypass_vclk_src_sel ;
	bypass_vclk_src_sel = ((pCTRL->stVIDEOCLK.vclk.clk_src_select)>>1) ;
		
	MISP_RegWrite(MISP_HOST_ID, 0x020, 0x0001);		// vclk pll_dis -- for hdmi-tx(lt8618)
	
#if defined(__USE_MDIN_i5XX_REV__)		// i5xx rev chip
	MISP_RegField(MISP_HOST_ID,  0x02a, 0, 1, pCTRL->stVPLL.pll_sel_vclk);
	MISP_RegField(MISP_HOST_ID,  0x02a, 1, 2, pCTRL->stVPLL.pll_in_clk_sel);
	MISP_RegField(MISP_HOST_ID,  0x02a, 3, 2, pCTRL->stVPLL.pll_in_x_tal_sel);
	MISP_RegField(MISP_HOST_ID,  0x02a, 5, 2, bypass_vclk_src_sel);
	
	MISP_RegField(MISP_HOST_ID,  0x026, 8, 6, (pCTRL->stVIDEOCLK.clk_p1.clk_invert<<5)|
											(pCTRL->stVIDEOCLK.clk_p1.clk_delay<<3)|
											(pCTRL->stVIDEOCLK.clk_p0.clk_invert<<2)|
											(pCTRL->stVIDEOCLK.clk_p0.clk_delay));
#else									// i5xx org chip
	MISP_RegField(MISP_HOST_ID,  0x02a, 0, 1, pCTRL->stVPLL.pll_sel_xtal);
	MISP_RegField(MISP_HOST_ID,  0x02a, 1, 2, pCTRL->stVPLL.pll_sel_inclk);
#endif
	
	// Video pll output clk setup overide occurs in ../Drivers/mispoutput.c "static void OUTCTRL_SetMainFrmt(BYTE frmt)"	
	// CLKCTRL_VideoPLLHandler(pCTRL) ;

	//mclk pll(for external cpu)
	if (pCTRL->ADSTSR_UCOM_EN == 0 ) // 0: When use external cpu
	{
		#if defined(__MDIN_i5XX_FPGA__)						// FPGA board
			MISP_RegWrite(MISP_HOST_ID,  0x022, 0x0002);	// only for FPGA
		#endif
		CLKCTRL_MemoryPLLHandler(pCTRL);
	}

												
	CLKCTRL_UserPLLHandler(pCTRL) ;

	MISP_RegWrite(MISP_HOST_ID,  0x037, pCTRL->sub_lvds_disable);	//

	// MISP_RegWrite(MISP_HOST_ID,  0x033, 0x0377);	// clock power save2 1 clk enable
	
	// MISP_RegWrite(MISP_HOST_ID,  0x03c, 0x0000);	// clock power save 0 clk enable

	CLKCTRL_ClockOnOFFHandler(pCTRL) ;
	
	// Delay_mSec(10);									// delay

	MISP_RegWrite(MISP_HOST_ID,  0x018, (pCTRL->stVIDEOCLK.vclk.clk_invert<<2)|
													(pCTRL->stVIDEOCLK.vclk.clk_delay)
													);	
													
	MISP_RegWrite(MISP_HOST_ID,  0x035, (pCTRL->stAUXCLK.clk_aux_mclk.clk_src_select<<12)|
													(pCTRL->stAUXCLK.clk_aux_mclk.clk_divide_param.clk_t)
													);	//
													
	MISP_RegWrite(MISP_HOST_ID,  0x036, (pCTRL->stAUXCLK.clk_aux_mclk.clk_divide_param.clk_synchonize<<13)|
													(pCTRL->stAUXCLK.clk_aux_mclk.clk_divide_param.clk_division_mode<<12)|
													(pCTRL->stAUXCLK.clk_aux_mclk.clk_divide_param.clk_f)
													);	//
	
	MISP_RegWrite(MISP_HOST_ID,  0x038, 	(pCTRL->stLVDSTXCLK.lvdstx_clk_sel<<15)|
													(pCTRL->stLVDSTXCLK.lvdstx_pll_clk_dly_sel<<12)|
													(pCTRL->stLVDSTXCLK.clk_lvdstx_divider<<8)|
													(pCTRL->stLVDSTXCLK.lvdstx_pll_clk_sel<<6)|
													(pCTRL->stISPCLK.clk_dis.clk_divider<<1)|
													(pCTRL->stISPCLK.clk_dis.clk_src_select)
													);	//

#if defined(__USE_MDIN_i510__)|defined(__USE_MDIN_i540__)|defined(__USE_MDIN_i550__)													
	#if defined(__USE_MDIN_i5XX_REV__)
		MISP_RegWrite(MISP_HOST_ID,  0x03f, (pCTRL->stISPCLK.clk_slave.clk_src_select<<10)|
														(pCTRL->stISPCLK.clk_bayer.clk_invert<<9)|
														(pCTRL->stISPCLK.clk_bayer.clk_delay<<7)|
														(pCTRL->stISPCLK.clk_bayer.clk_divider<<3)|
														((pCTRL->stISPCLK.clk_odm_int.clk_src_select>>1)<<2)|
														(pCTRL->stISPCLK.clk_bayer.clk_src_select)
														);	//														
	#else
		MISP_RegWrite(MISP_HOST_ID,  0x03f, (pCTRL->stISPCLK.clk_bayer.clk_invert<<9)|
														(pCTRL->stISPCLK.clk_bayer.clk_delay<<7)|
														(pCTRL->stISPCLK.clk_bayer.clk_divider<<3)|
														(pCTRL->stISPCLK.clk_bayer.clk_src_select)
														);	//
	#endif
													
#if defined(__MDIN_i550_USB_V100__)
	MISP_RegWrite(MISP_HOST_ID,  0x041, (pCTRL->stVIDEOCLK.vclk_out_diable<<8)|0x0000);	// vclk_out_b_dis[9] , vclk_out_dis[8]
#else
	MISP_RegWrite(MISP_HOST_ID,  0x041, (pCTRL->stVIDEOCLK.vclk_out_diable<<8)|0x0200);	// vclk_out_b_dis[9] , vclk_out_dis[8]
#endif
#else
	MISP_RegWrite(MISP_HOST_ID,  0x03f, (pCTRL->stISPCLK.clk_bayer.clk_invert<<9)|
													(pCTRL->stISPCLK.clk_bayer.clk_delay<<7)|
													(pCTRL->stISPCLK.clk_bayer.clk_divider<<3)|
													(pCTRL->stISPCLK.clk_bayer.clk_src_select)
													);	//		
		
	MISP_RegWrite(MISP_HOST_ID,  0x041, (pCTRL->stVIDEOCLK.vclk_out_diable<<9)|(pCTRL->stVIDEOCLK.vclk_out_diable<<8)|0x0000);	// vclk_out_b_dis[9] , vclk_out_dis[8]
#endif
	
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	MISP_RegWrite(MISP_HOST_ID,  0x03a, (pCTRL->stISPCLK.clk_sensor_in.clk_divider<<3)|
													(pCTRL->stUSERPLL.pll_in_sel<<1)|
													(pCTRL->stVPLL.pll_sel_hclk) 
													);	//
													
	MISP_RegWrite(MISP_HOST_ID,  0x03e, (pCTRL->stISPCLK.clk_yc_m.clk_invert<<8)|
													(pCTRL->stISPCLK.clk_yc_m.clk_delay<<6)|
													(pCTRL->stAUXCLK.clk_in_aux.clk_invert<<5)|
													(pCTRL->stAUXCLK.clk_in_aux.clk_delay<<3)|
													0x0000
													);	//

	MISP_RegWrite(MISP_HOST_ID,  0x024, (pCTRL->stVIDEOCLK.vclk_out_b.clk_invert<<5)|
													(pCTRL->stVIDEOCLK.vclk_out_b.clk_delay<<3)|
													(pCTRL->stVIDEOCLK.vclk_out.clk_invert<<2)|
													(pCTRL->stVIDEOCLK.vclk_out.clk_delay)
													);		

	MISP_RegWrite(MISP_HOST_ID,  0x042, (pCTRL->stAUXCLK.clk_aux_mclk.clk_divide_param.clk_divider<<10)|
													(pCTRL->stVIDEOCLK.vclk.clk_src_select<<4)|
													(pCTRL->stVIDEOCLK.vclk.clk_divider<<2)|
													(pCTRL->stVIDEOCLK.vclk_out.clk_divider)
													);											
#else 												// MDIN-i51X (i510, i540, i550)
	MISP_RegField(MISP_HOST_ID,  0x03a, 1, 2, pCTRL->stUSERPLL.pll_in_sel);
	MISP_RegField(MISP_HOST_ID,  0x03a, 3, 2, pCTRL->stISPCLK.clk_sensor_in.clk_divider);
	MISP_RegField(MISP_HOST_ID,  0x03a, 5, 3, pCTRL->stISPCLK.clk_user.clk_src_select);
	MISP_RegField(MISP_HOST_ID,  0x03a, 8, 2, pCTRL->stISPCLK.clk_yc_m.clk_src_select);
	
	#if !defined(__USE_MDIN_i5XX_REV__)		// i5xx org chip
		MISP_RegField(MISP_HOST_ID,  0x03a, 0, 1, pCTRL->stVPLL.pll_sel_hclk);
	#endif
	
	MISP_RegWrite(MISP_HOST_ID,  0x03e, (pCTRL->stISPCLK.clk_yc_tm.clk_src_select<<13)|
													(pCTRL->stAUXCLK.clk_in_aux_tm.clk_src_select<<12)|
													(pCTRL->stISPCLK.clk_yc_tm.clk_invert<<11)|
													(pCTRL->stISPCLK.clk_yc_tm.clk_delay<<9)|
													(pCTRL->stISPCLK.clk_yc_m.clk_invert<<8)|
													(pCTRL->stISPCLK.clk_yc_m.clk_delay<<6)|
													(pCTRL->stAUXCLK.clk_in_aux.clk_invert<<5)|
													(pCTRL->stAUXCLK.clk_in_aux.clk_delay<<3)|
													(pCTRL->stAUXCLK.clk_in_aux_tm.clk_invert<<2)|
													(pCTRL->stAUXCLK.clk_in_aux_tm.clk_delay)
													);
	
	MISP_RegWrite(MISP_HOST_ID,  0x024, (pCTRL->stVIDEOCLK.vclk_out_i.clk_invert<<8)|	
													(pCTRL->stVIDEOCLK.vclk_out_i.clk_delay<<6)|
													(pCTRL->stVIDEOCLK.vclk_out_b.clk_invert<<5)|
													(pCTRL->stVIDEOCLK.vclk_out_b.clk_delay<<3)|
													(pCTRL->stVIDEOCLK.vclk_out.clk_invert<<2)|
													(pCTRL->stVIDEOCLK.vclk_out.clk_delay)
													);	
												
	MISP_RegField(MISP_HOST_ID,  0x042, 10, 6, (pCTRL->stAUXCLK.clk_aux_mclk.clk_divide_param.clk_divider));
	MISP_RegField(MISP_HOST_ID,  0x042, 6, 2, (pCTRL->stVIDEOCLK.vclk_out_i.clk_src_select));	
	
	MISP_RegField(MISP_HOST_ID,  0x042, 0, 5, (pCTRL->stVIDEOCLK.vclk.clk_src_select<<4)|
															(pCTRL->stVIDEOCLK.vclk.clk_divider<<2)|
															(pCTRL->stVIDEOCLK.vclk_out.clk_divider)
															);												
												
#endif
												
	MISP_RegWrite(MISP_HOST_ID,  0x043, (pCTRL->vclk_out_gen_sync<<11)|
										(pCTRL->vclk_out_gen_pload<<8)|
										(pCTRL->stVIDEOCLK.vclk_out_b.clk_src_select<<2)|
										(pCTRL->stVIDEOCLK.vclk_out.clk_src_select<<1)
													);

	MISP_RegWrite(MISP_HOST_ID,  0x045, (pCTRL->stVIDEOCLK.DAC_video_power_down<<13)|
													(pCTRL->stVIDEOCLK.encdac_clk.clk_invert<<12)|
													(pCTRL->stVIDEOCLK.encdac_clk.clk_delay<<10)|
													(pCTRL->stMEMCLK.ix2clko.clk_invert<<9)|
													(pCTRL->stMEMCLK.ix2clko.clk_delay<<7)|
													(pCTRL->stMEMCLK.icoreclk.clk_invert<<6)|
													(pCTRL->stMEMCLK.icoreclk.clk_delay<<4)|
													(pCTRL->stMEMCLK.mclk.clk_invert<<3)|
													(pCTRL->stMEMCLK.mclk.clk_delay)
													);	//
	
	MISP_RegField(MISP_HOST_ID,  0x046,  0,  2,  pCTRL->stAUXCLK.clk_aux_out.clk_delay);
	MISP_RegField(MISP_HOST_ID,  0x046,  2,  1,  pCTRL->stAUXCLK.clk_aux_out.clk_invert);
	MISP_RegField(MISP_HOST_ID,  0x046,  3,  2,  pCTRL->stAUXCLK.clk_aux_out.clk_src_select);
	MISP_RegField(MISP_HOST_ID,  0x046,  5,  2,  pCTRL->stAUXCLK.clk_aux_in.clk_delay);
	MISP_RegField(MISP_HOST_ID,  0x046,  7,  1,  pCTRL->stAUXCLK.clk_aux_in.clk_invert);
	MISP_RegField(MISP_HOST_ID,  0x046, 11,  2,  pCTRL->stVIDEOCLK.clk_enc_int.clk_src_select);
	MISP_RegField(MISP_HOST_ID,  0x046, 13,  2,  pCTRL->stVIDEOCLK.clk_in_enc.clk_delay);
	MISP_RegField(MISP_HOST_ID,  0x046, 15,  1,  pCTRL->stVIDEOCLK.clk_in_enc.clk_invert);
	
	if (pCTRL->ADSTSR_UCOM_EN == 0 )	{	// 0: When use external cpu
		MISP_RegField(MISP_HOST_ID,  0x046, 10,  1, pCTRL->stMPLL.x_tal2_sel);
	}
	
	#if !defined(__USE_MDIN_i5XX_REV__)		// i5xx org chip
		MISP_RegField(MISP_HOST_ID,  0x046,  9,  1,  pCTRL->stVPLL.pll_sel_xtal2);
	#endif

	MISP_RegWrite(MISP_HOST_ID,  0x047, (pCTRL->stAUXCLK.clk_aux_mid.clk_src_select<<14)|
													(pCTRL->stAUXCLK.clk_aux_mid.clk_invert<<13)|
													(pCTRL->stAUXCLK.clk_aux_mid.clk_delay<<11)|
													(pCTRL->stAUXCLK.clk_aux_in.clk_src_select<<9)|
													(pCTRL->stVIDEOCLK.clk_m_enc_int.clk_divider<<6)|
													(pCTRL->stVIDEOCLK.clk_m_enc_int.clk_src_select<<4)|
													(pCTRL->stAUXCLK.clk_m_aux_int.clk_divider<<2)|
													(pCTRL->stAUXCLK.clk_m_aux_int.clk_src_select)
													);

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	MISP_RegWrite(MISP_HOST_ID,  0x048, (pCTRL->stISPCLK.clk_sensor_in.clk_invert<<6)|
													(pCTRL->stISPCLK.clk_sensor_in.clk_delay<<4)|
													(pCTRL->stISPCLK.clk_sensor_in.clk_src_select)
													);												
#else 												// MDIN-i51X (i510, i540, i550)
	MISP_RegWrite(MISP_HOST_ID,  0x048, (pCTRL->stUSERPLL.ab_pll_sel<<9)|
													(pCTRL->stISPCLK.clk_sensor_in.clk_invert<<6)|
													(pCTRL->stISPCLK.clk_sensor_in.clk_delay<<4)|
													(pCTRL->stISPCLK.clk_sensor_in.clk_src_select)
													);
#endif
													
	MISP_RegWrite(MISP_HOST_ID,  0x049, (pCTRL->stISPCLK.clk_yc.clk_divider<<13)|
													(pCTRL->stISPCLK.clk_nr.clk_invert<<12)|
													(pCTRL->stISPCLK.clk_nr.clk_delay<<10)|
													(pCTRL->stISPCLK.clk_nr.clk_src_select<<9)|
													(pCTRL->stISPCLK.clk_yc.clk_invert<<8)|
													(pCTRL->stISPCLK.clk_yc.clk_delay<<6)|
													(pCTRL->stISPCLK.clk_yc.clk_src_select<<4)|
													(pCTRL->stISPCLK.clk_lvds_in.clk_invert<<3)|
													(pCTRL->stISPCLK.clk_lvds_in.clk_delay<<1)|
													(pCTRL->stISPCLK.clk_lvds_sel)
													);	
}

//--------------------------------------------------------------------------------------------------------------------------
void Init_HostCLK(void) {	
	memset(&stHostCLKCTRL, 0, sizeof(HostCLK_CTRL_PARAM));	
	Init_HostCLK_PARAM(&stHostCLKCTRL);
	Init_HostCLK_REGISTER(&stHostCLKCTRL);
	stHostCLKCTRL.ctrl = 0 ;
}

//--------------------------------------------------------------------------------------------------------------------------
static void CLKCTRL_VideoClockHandler( PHostCLK_CTRL_PARAM pCTRL )
{
	BYTE bypass_vclk_src_sel = 0 ;
	
	if ((pCTRL->ctrl&HostCLKCTRL_VIDEOCLK_UPDATE)==0) return;
	pCTRL->ctrl &= (~HostCLKCTRL_VIDEOCLK_UPDATE);
	
	MISP_RegWrite(MISP_HOST_ID,  0x018, (pCTRL->stVIDEOCLK.vclk.clk_invert<<2)|
													(pCTRL->stVIDEOCLK.vclk.clk_delay)
													);	
#if defined(__USE_MDIN_i510__)|defined(__USE_MDIN_i540__)|defined(__USE_MDIN_i550__)
	#if defined(__MDIN_i550_USB_V100__)
		MISP_RegWrite(MISP_HOST_ID,  0x041, (pCTRL->stVIDEOCLK.vclk_out_diable<<8)|0x0000);	// vclk_out_b enable
	#else
		MISP_RegWrite(MISP_HOST_ID,  0x041, (pCTRL->stVIDEOCLK.vclk_out_diable<<8)|0x0200);	// vclk_out_b_dis[9] , vclk_out_dis[8]
	#endif
#else													
	MISP_RegWrite(MISP_HOST_ID,  0x041, (pCTRL->stVIDEOCLK.vclk_out_diable<<9)|(pCTRL->stVIDEOCLK.vclk_out_diable<<8)|0x0000);	// vclk_out_b_dis[9] , vclk_out_dis[8]
#endif
	MISP_RegField(MISP_HOST_ID,  0x042, 0, 5, (pCTRL->stVIDEOCLK.vclk.clk_src_select<<4)|
															(pCTRL->stVIDEOCLK.vclk.clk_divider<<2)|
															(pCTRL->stVIDEOCLK.vclk_out.clk_divider)
															);
#if defined(__USE_MDIN_i5XX_REV__)
	bypass_vclk_src_sel = ((pCTRL->stVIDEOCLK.vclk.clk_src_select)>>1) ;												
	MISP_RegField(MISP_HOST_ID,  0x02a, 5, 2, bypass_vclk_src_sel);													
#endif														
														
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	MISP_RegWrite(MISP_HOST_ID,  0x024, (pCTRL->stVIDEOCLK.vclk_out_b.clk_invert<<5)|
													(pCTRL->stVIDEOCLK.vclk_out_b.clk_delay<<3)|
													(pCTRL->stVIDEOCLK.vclk_out.clk_invert<<2)|
													(pCTRL->stVIDEOCLK.vclk_out.clk_delay)
													);	
#else 												// MDIN-i51X (i510, i540, i550)
	MISP_RegWrite(MISP_HOST_ID,  0x024, (pCTRL->stVIDEOCLK.vclk_out_i.clk_invert<<8)|	
													(pCTRL->stVIDEOCLK.vclk_out_i.clk_delay<<6)|
													(pCTRL->stVIDEOCLK.vclk_out_b.clk_invert<<5)|
													(pCTRL->stVIDEOCLK.vclk_out_b.clk_delay<<3)|
													(pCTRL->stVIDEOCLK.vclk_out.clk_invert<<2)|
													(pCTRL->stVIDEOCLK.vclk_out.clk_delay)
													);		
	MISP_RegField(MISP_HOST_ID,  0x042, 6, 2, (pCTRL->stVIDEOCLK.vclk_out_i.clk_src_select));	
#endif
														
	MISP_RegWrite(MISP_HOST_ID,  0x043, (pCTRL->vclk_out_gen_sync<<11)|
													(pCTRL->vclk_out_gen_pload<<8)|
													(pCTRL->stVIDEOCLK.vclk_out_b.clk_src_select<<2)|
													(pCTRL->stVIDEOCLK.vclk_out.clk_src_select<<1)
													);

	MISP_RegField(MISP_HOST_ID,  0x045, 10, 4, (pCTRL->stVIDEOCLK.DAC_video_power_down<<3)|
																(pCTRL->stVIDEOCLK.encdac_clk.clk_invert<<2)|
																(pCTRL->stVIDEOCLK.encdac_clk.clk_delay<<0)
																);	//

	MISP_RegField(MISP_HOST_ID,  0x046, 11, 5, (pCTRL->stVIDEOCLK.clk_in_enc.clk_invert<<4)|
																(pCTRL->stVIDEOCLK.clk_in_enc.clk_delay<<2)|
																(pCTRL->stVIDEOCLK.clk_enc_int.clk_src_select<<0)
																);	

	MISP_RegField(MISP_HOST_ID,  0x047, 4, 3, (pCTRL->stVIDEOCLK.clk_m_enc_int.clk_divider<<2)|
															(pCTRL->stVIDEOCLK.clk_m_enc_int.clk_src_select<<0)
															);

}

//--------------------------------------------------------------------------------------------------------------------------
static void CLKCTRL_MemoryClockHandler( PHostCLK_CTRL_PARAM pCTRL )
{

	if ((pCTRL->ctrl&HostCLKCTRL_MEMCLK_UPDATE)==0) return;
	pCTRL->ctrl &= (~HostCLKCTRL_MEMCLK_UPDATE);

	MISP_RegField(MISP_HOST_ID,  0x045, 1, 9,
													(pCTRL->stMEMCLK.ix2clko.clk_invert<<8)|
													(pCTRL->stMEMCLK.ix2clko.clk_delay<<6)|
													(pCTRL->stMEMCLK.icoreclk.clk_invert<<5)|
													(pCTRL->stMEMCLK.icoreclk.clk_delay<<3)|
													(pCTRL->stMEMCLK.mclk.clk_invert<<2)|
													(pCTRL->stMEMCLK.mclk.clk_delay<<0)
													);	//
}


//--------------------------------------------------------------------------------------------------------------------------
static void CLKCTRL_AUXClockHandler( PHostCLK_CTRL_PARAM pCTRL )
{
	if ((pCTRL->ctrl&HostCLKCTRL_AUXCLK_UPDATE)==0) return;
	pCTRL->ctrl &= (~HostCLKCTRL_AUXCLK_UPDATE);

	MISP_RegWrite(MISP_HOST_ID,  0x035, (pCTRL->stAUXCLK.clk_aux_mclk.clk_src_select<<12)|
													(pCTRL->stAUXCLK.clk_aux_mclk.clk_divide_param.clk_t)
													);	//
													
	MISP_RegWrite(MISP_HOST_ID,  0x036, (pCTRL->stAUXCLK.clk_aux_mclk.clk_divide_param.clk_synchonize<<13)|
													(pCTRL->stAUXCLK.clk_aux_mclk.clk_divide_param.clk_division_mode<<12)|
													(pCTRL->stAUXCLK.clk_aux_mclk.clk_divide_param.clk_f)
													);	//
	
	MISP_RegField(MISP_HOST_ID,  0x03e, 3, 3, (pCTRL->stAUXCLK.clk_in_aux.clk_invert<<2)|
												(pCTRL->stAUXCLK.clk_in_aux.clk_delay<<0)
												);	//
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	MISP_RegField(MISP_HOST_ID,  0x03e, 0, 3, (pCTRL->stAUXCLK.clk_in_aux_tm.clk_invert<<2)|
												(pCTRL->stAUXCLK.clk_in_aux_tm.clk_delay<<0)
												);	//
												
	MISP_RegField(MISP_HOST_ID,  0x03e, 12, 1, (pCTRL->stAUXCLK.clk_in_aux_tm.clk_src_select));	
#endif
												
												
	MISP_RegField(MISP_HOST_ID,  0x042, 10, 6, (pCTRL->stAUXCLK.clk_aux_mclk.clk_divide_param.clk_divider));

	MISP_RegField(MISP_HOST_ID,  0x046, 0, 8, (pCTRL->stAUXCLK.clk_aux_in.clk_invert<<7)|
														(pCTRL->stAUXCLK.clk_aux_in.clk_delay<<5)|
														(pCTRL->stAUXCLK.clk_aux_out.clk_src_select<<3)|
														(pCTRL->stAUXCLK.clk_aux_out.clk_invert<<2)|
														(pCTRL->stAUXCLK.clk_aux_out.clk_delay) 
														);

	MISP_RegField(MISP_HOST_ID,  0x047, 9, 7, (pCTRL->stAUXCLK.clk_aux_mid.clk_src_select<<5)|
														(pCTRL->stAUXCLK.clk_aux_mid.clk_invert<<4)|
														(pCTRL->stAUXCLK.clk_aux_mid.clk_delay<<2)|
														(pCTRL->stAUXCLK.clk_aux_in.clk_src_select<<0)
														);

	MISP_RegField(MISP_HOST_ID,  0x047, 0, 3,	(pCTRL->stAUXCLK.clk_m_aux_int.clk_divider<<2)|
														(pCTRL->stAUXCLK.clk_m_aux_int.clk_src_select)
														);
}

//--------------------------------------------------------------------------------------------------------------------------


static void CLKCTRL_ISPClockHandler( PHostCLK_CTRL_PARAM pCTRL )
{
	if ((pCTRL->ctrl&HostCLKCTRL_ISPCLK_UPDATE)==0) return;
	pCTRL->ctrl &= (~HostCLKCTRL_ISPCLK_UPDATE);

	MISP_RegWrite(MISP_HOST_ID,  0x038, 	(pCTRL->stLVDSTXCLK.lvdstx_clk_sel<<15)|
													(pCTRL->stLVDSTXCLK.lvdstx_pll_clk_dly_sel<<12)|
													(pCTRL->stLVDSTXCLK.clk_lvdstx_divider<<8)|
													(pCTRL->stLVDSTXCLK.lvdstx_pll_clk_sel<<6)|
													(pCTRL->stISPCLK.clk_dis.clk_divider<<1)|
													(pCTRL->stISPCLK.clk_dis.clk_src_select)
													);	//

	MISP_RegField(MISP_HOST_ID,  0x03a, 3, 2, (pCTRL->stISPCLK.clk_sensor_in.clk_divider));	//	
	
	MISP_RegField(MISP_HOST_ID,  0x03e, 6, 3 ,(pCTRL->stISPCLK.clk_yc_m.clk_invert<<2)|(pCTRL->stISPCLK.clk_yc_m.clk_delay));	//	

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	MISP_RegField(MISP_HOST_ID,  0x033, 3, 1, (pCTRL->stISPCLK.clk_odm_int.clk_src_select));	// clock power save2 1 clk enable											
	
	MISP_RegWrite(MISP_HOST_ID,  0x048, (pCTRL->stISPCLK.clk_sensor_in.clk_invert<<6)|
													(pCTRL->stISPCLK.clk_sensor_in.clk_delay<<4)|
													(pCTRL->stISPCLK.clk_sensor_in.clk_src_select)
													);	
												
	MISP_RegWrite(MISP_HOST_ID,  0x03f, (pCTRL->stISPCLK.clk_bayer.clk_invert<<9)|
													(pCTRL->stISPCLK.clk_bayer.clk_delay<<7)|
													(pCTRL->stISPCLK.clk_bayer.clk_divider<<3)|
													(pCTRL->stISPCLK.clk_bayer.clk_src_select)
													);	//								
												
#else 												// MDIN-i51X (i510, i540, i550)
	MISP_RegField(MISP_HOST_ID,  0x033, 9, 1, (pCTRL->stISPCLK.clk_odm_int.clk_src_select));	// clock power save2 1 clk enable
	
	MISP_RegField(MISP_HOST_ID,  0x030, 4, 3, (pCTRL->stISPCLK.clk_odm_int.clk_divider));	
		
	MISP_RegField(MISP_HOST_ID,  0x03e, 13, 1 ,(pCTRL->stISPCLK.clk_yc_tm.clk_src_select));	//	
	MISP_RegField(MISP_HOST_ID,  0x03e, 9, 3 ,(pCTRL->stISPCLK.clk_yc_tm.clk_invert<<2)|(pCTRL->stISPCLK.clk_yc_tm.clk_delay));	//	
	
	#if defined(__USE_MDIN_i5XX_REV__)
		MISP_RegWrite(MISP_HOST_ID,  0x03f, (pCTRL->stISPCLK.clk_slave.clk_src_select<<10)|
														(pCTRL->stISPCLK.clk_bayer.clk_invert<<9)|
														(pCTRL->stISPCLK.clk_bayer.clk_delay<<7)|
														(pCTRL->stISPCLK.clk_bayer.clk_divider<<3)|
														((pCTRL->stISPCLK.clk_odm_int.clk_src_select>>1)<<2)|
														(pCTRL->stISPCLK.clk_bayer.clk_src_select)
														);	//		
	#else
		MISP_RegWrite(MISP_HOST_ID,  0x03f, (pCTRL->stISPCLK.clk_bayer.clk_invert<<9)|
														(pCTRL->stISPCLK.clk_bayer.clk_delay<<7)|
														(pCTRL->stISPCLK.clk_bayer.clk_divider<<3)|
														(pCTRL->stISPCLK.clk_bayer.clk_src_select)
														);	//	
	#endif
	
	MISP_RegWrite(MISP_HOST_ID,  0x048, (pCTRL->stUSERPLL.ab_pll_sel<<9)|
													(pCTRL->stISPCLK.clk_sensor_in.clk_invert<<6)|
													(pCTRL->stISPCLK.clk_sensor_in.clk_delay<<4)|
													(pCTRL->stISPCLK.clk_sensor_in.clk_src_select)
													);
#endif
													
	MISP_RegWrite(MISP_HOST_ID,  0x049, (pCTRL->stISPCLK.clk_yc.clk_divider<<13)|
													(pCTRL->stISPCLK.clk_nr.clk_invert<<12)|
													(pCTRL->stISPCLK.clk_nr.clk_delay<<10)|
													(pCTRL->stISPCLK.clk_nr.clk_src_select<<9)|
													(pCTRL->stISPCLK.clk_yc.clk_invert<<8)|
													(pCTRL->stISPCLK.clk_yc.clk_delay<<6)|
													(pCTRL->stISPCLK.clk_yc.clk_src_select<<4)|
													(pCTRL->stISPCLK.clk_lvds_in.clk_invert<<3)|
													(pCTRL->stISPCLK.clk_lvds_in.clk_delay<<1)|
													(pCTRL->stISPCLK.clk_lvds_sel)
													);													
}


	
//--------------------------------------------------------------------------------------------------------------------------
static void CLKCTRL_ClockOnOFFHandler( PHostCLK_CTRL_PARAM pCTRL )
{
	WORD clk_power_save ;
	
	if ((pCTRL->ctrl&HostCLKCTRL_CLK_ON_OFF_UPDATE)==0) return;
	pCTRL->ctrl &= (~HostCLKCTRL_CLK_ON_OFF_UPDATE);
	
	MISP_RegWrite(MISP_HOST_ID,  0x03C, (pCTRL->clk_power_save_0));	// clock power save2 1 clk enable	
		
#if defined(__USE_MDIN_i500__) 								// MDIN-i500
	clk_power_save = (stHostCLKCTRL.clk_power_save_1 & 0xFFF7 )|(pCTRL->stISPCLK.clk_odm_int.clk_src_select<<3) ;
	MISP_RegWrite(MISP_HOST_ID,  0x033, clk_power_save);	// clock power save2 1 clk enable
#else 														// MDIN-i51X (i510, i540, i550)
	clk_power_save = (stHostCLKCTRL.clk_power_save_1) ;
//	MISP_RegField(MISP_HOST_ID,  0x033, 0, 9, clk_power_save);	// clock power save2 1 clk enable		
	MISP_RegField(MISP_HOST_ID,  0x033, 0, 8, clk_power_save);	// clock power save2 1 clk enable -- exclude '[9]clk_odm_sel'
#endif
}
//--------------------------------------------------------------------------------------------------------------------------
static void CLKCTRL_UserPLLHandler( PHostCLK_CTRL_PARAM pCTRL )
{
	if ((pCTRL->ctrl&HostCLKCTRL_USPLL_UPDATE)==0) return;
	pCTRL->ctrl &= (~HostCLKCTRL_USPLL_UPDATE);
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	MISP_RegWrite(MISP_HOST_ID, 0x039,	(pCTRL->stUSERPLL.post_div<<9)|
										(pCTRL->stUSERPLL.pre_div<<4)|
										(pCTRL->stUSERPLL.post_scale<<1)|
										0x0001);	// disable PLL
	MISP_RegField(MISP_HOST_ID,  0x03a, 1, 2, (pCTRL->stUSERPLL.pll_in_sel));	//										
										
	MISP_RegWrite(MISP_HOST_ID, 0x039,	(pCTRL->stUSERPLL.post_div<<9)|
										(pCTRL->stUSERPLL.pre_div<<4)|
										(pCTRL->stUSERPLL.post_scale<<1)|
										0x0000);	// Enable PLL	
#else 								// MDIN-i51X (i510, i540, i550)
	MISP_RegWrite(MISP_HOST_ID, 0x039,	((pCTRL->stUSERPLL.pre_div&0x0f)<<4)|
										((pCTRL->stUSERPLL.post_scale&0x07)<<1)|
										0x0001);	// disable PLL
	MISP_RegWrite(MISP_HOST_ID, 0x02D,	((pCTRL->stUSERPLL.pll_bypass)<<14)|
										((pCTRL->stUSERPLL.output_div&0x03)<<12)|
										((pCTRL->stUSERPLL.input_div&0x0f)<<8)|
										(pCTRL->stUSERPLL.post_div &0xff));
	
	MISP_RegField(MISP_HOST_ID,  0x03a, 1, 2, (pCTRL->stUSERPLL.pll_in_sel));	//		
	MISP_RegField(MISP_HOST_ID,  0x03a, 5, 3, pCTRL->stISPCLK.clk_user.clk_src_select);	
	
	MISP_RegWrite(MISP_HOST_ID, 0x039,	((pCTRL->stUSERPLL.pre_div&0x0f)<<4)|
										((pCTRL->stUSERPLL.post_scale&0x07)<<1)|
										0x0000);	// Enable PLL
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
static void CLKCTRL_VideoPLLHandler( PHostCLK_CTRL_PARAM pCTRL )
{
	if ((pCTRL->ctrl&HostCLKCTRL_VPLL_UPDATE)==0) return;
	pCTRL->ctrl &= (~HostCLKCTRL_VPLL_UPDATE);

	MISP_RegField(MISP_HOST_ID, 0x020, 0, 1, 1);	// disable PLL

#if defined(__USE_MDIN_i5XX_REV__)		// i5xx rev chip
	MISP_RegField(MISP_HOST_ID,  0x02a, 0, 1, pCTRL->stVPLL.pll_sel_vclk);
	MISP_RegField(MISP_HOST_ID,  0x02a, 1, 2, pCTRL->stVPLL.pll_in_clk_sel);
	MISP_RegField(MISP_HOST_ID,  0x02a, 3, 2, pCTRL->stVPLL.pll_in_x_tal_sel);
#else									// i5xx org chip
	MISP_RegField(MISP_HOST_ID,  0x02a, 0, 1, pCTRL->stVPLL.pll_sel_xtal);
	MISP_RegField(MISP_HOST_ID,  0x02a, 1, 2, pCTRL->stVPLL.pll_sel_inclk);
	
	MISP_RegField(MISP_HOST_ID,  0x03a, 0, 1, (pCTRL->stVPLL.pll_sel_hclk));
	MISP_RegField(MISP_HOST_ID,  0x046, 9, 1, (pCTRL->stVPLL.pll_sel_xtal2));
#endif
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	MISP_RegWrite(MISP_HOST_ID, 0x02c, pCTRL->stVPLL.pre_div);			// pre-divider, Fin/P : 0.0513Mhz ~ 25Mhz
	MISP_RegWrite(MISP_HOST_ID, 0x02e, pCTRL->stVPLL.post_div);			// post-divider, M : 12 ~ 1168
	// MISP_RegWrite(MISP_HOST_ID, 0x030, pCTRL->stVPLL.post_scale);			// post-scaler, Fout*S : 60Mhz ~ 300Mhz
	MISP_RegField(MISP_HOST_ID, 0x030, 0, 3, pCTRL->stVPLL.post_scale);
#else 								// MDIN-i51X (i510, i540, i550)
	MISP_RegWrite(MISP_HOST_ID, 0x02c, MAKEBYTE(pCTRL->stVPLL.pre_div&0x0f, pCTRL->stVPLL.input_div&0x0f));	// [7:4]pre-divider(P) : 1 ~ 15,  [3:0]input-divider(N), Fin/N : 1Mhz ~ 50Mhz
	MISP_RegWrite(MISP_HOST_ID, 0x02e, MAKEWORD((((pCTRL->stVPLL.pll_bypass&0x01)<<2) | (pCTRL->stVPLL.output_div&0x03)), \
												pCTRL->stVPLL.post_div&0xff));	// [10]pll-bypass, [9:8]output-divider(OD) : 0 ~ 2(500Mhz < Fout*2^OD < 1500Mhz),  [7:0]post-divider(M) : 2 ~ 255
	// MISP_RegWrite(MISP_HOST_ID, 0x030, pCTRL->stVPLL.post_scale&0x07);			// [2:0]post-scaler(S)
	MISP_RegField(MISP_HOST_ID, 0x030, 0, 3, pCTRL->stVPLL.post_scale&0x07);
#endif
	MISP_RegField(MISP_HOST_ID, 0x020, 0, 1, pCTRL->stVPLL.pll_en_n);	// 0:enable PLL,  1:disable PLL
}
//--------------------------------------------------------------------------------------------------------------------------
static void CLKCTRL_MemoryPLLHandler( PHostCLK_CTRL_PARAM pCTRL )
{
	if ((pCTRL->ctrl&HostCLKCTRL_MPLL_UPDATE)==0) return;
	pCTRL->ctrl &= (~HostCLKCTRL_MPLL_UPDATE);
	
	MISP_RegField(MISP_HOST_ID, 0x020, 1, 1, 1);	// disable PLL
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	MISP_RegWrite(MISP_HOST_ID, 0x032, pCTRL->stMPLL.pre_div);			// pre-divider
	MISP_RegWrite(MISP_HOST_ID, 0x034, (pCTRL->stMPLL.post_scale<<7)|pCTRL->stMPLL.post_div);	// post-scaler & post-divider
#else 								// MDIN-i51X (i510, i540, i550)
	MISP_RegWrite(MISP_HOST_ID, 0x032, MAKEBYTE(pCTRL->stMPLL.post_scale&0x03, pCTRL->stMPLL.pre_div&0x0f));	// [5:4]post-scaler, [3:0]pre-divider
	MISP_RegField(MISP_HOST_ID, 0x034, 0, 8, pCTRL->stMPLL.post_div&0xff);	// [7:0]post-divider
#endif
	MISP_RegField(MISP_HOST_ID, 0x020, 1, 1, 0);	// enable PLL
}

//--------------------------------------------------------------------------------------------------------------------------	
	
	
//--------------------------------------------------------------------------------------------------------------------------
void HostCLK_ProcessHandler(void)
{
	if ((stHostCLKCTRL.ctrl&HostCLKCTRL_TRIGGER_ISP)==0) return;
	stHostCLKCTRL.ctrl &= (~HostCLKCTRL_TRIGGER_ISP);

	CLKCTRL_ClockOnOFFHandler(&stHostCLKCTRL) ;			
	CLKCTRL_MemoryPLLHandler(&stHostCLKCTRL) ;			
	CLKCTRL_UserPLLHandler(&stHostCLKCTRL) ;	
	CLKCTRL_VideoPLLHandler(&stHostCLKCTRL) ;	
	CLKCTRL_ISPClockHandler(&stHostCLKCTRL) ;	
	CLKCTRL_AUXClockHandler(&stHostCLKCTRL) ;	
	CLKCTRL_VideoClockHandler(&stHostCLKCTRL) ;	
	CLKCTRL_MemoryClockHandler(&stHostCLKCTRL) ;	
}

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
// GET / SET function

//--------------------------------------------------------------------------------------------------------------------------

void SetCLK_SrcAuxTm(WORD clk_aux_tm_sel)
{
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_AUXCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stAUXCLK.clk_in_aux_tm.clk_src_select = clk_aux_tm_sel ;
		// [¡®0¡¯ : CLK_B is used] : HDMI CLK
		// [¡®1¡¯ : CLK_A is used] : TTL CLK		
#endif
}
//--------------------------------------------------------------------------------------------------------------------------
void SetCLK_InvAuxTm(BOOL clk_aux_tm_inv)
{
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_AUXCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stAUXCLK.clk_in_aux_tm.clk_invert = clk_aux_tm_inv ;
#endif	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetCLK_SrcYCTm(WORD clk_yc_tm_sel)
{
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_ISPCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stISPCLK.clk_yc_tm.clk_src_select = clk_yc_tm_sel ;
		// [¡®0¡¯ : CLK_B is used] : HDMI CLK
		// [¡®1¡¯ : CLK_A is used] : TTL CLK	
#if __CLK_DEBUG_PRINT__	
	UARTprintf("SetCLK_SrcYCTm\r\n") ;
#endif
#endif
}
//--------------------------------------------------------------------------------------------------------------------------
void SetCLK_InvYCTm(BOOL clk_tm_yc_inv)
{
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_ISPCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stISPCLK.clk_yc_tm.clk_invert = clk_tm_yc_inv ;
#if __CLK_DEBUG_PRINT__		
	UARTprintf("SetCLK_InvYCTm\r\n") ;
#endif	
#endif	
}
//--------------------------------------------------------------------------------------------------------------------------
void SetCLK_SrcYC(WORD clk_yc_sel)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_ISPCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stISPCLK.clk_yc.clk_src_select = clk_yc_sel ;
		// i51x control value
		// [¡®00¡¯ : clk_yc_tm is used]
		// [¡®01¡¯ : clk_yc_tm/2 is used]
		// [¡®1x¡¯ : clk_bayer is used] 		
	
		// i500 control value
		// [¡®00¡¯ : CLK_B is used] : HDMI CLK
		// [¡®01¡¯ : CLK_A is used] : TTL CLK
		// [¡®1x¡¯ : clk_bayer is used] 
#if __CLK_DEBUG_PRINT__		
	UARTprintf("SetCLK_SrcYC\r\n") ;
#endif
}
//------------------------------------------------------------------------------
void SetCLK_InvYC(BOOL clk_yc_inv)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_ISPCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);

	stHostCLKCTRL.stISPCLK.clk_yc.clk_invert = clk_yc_inv ;
#if __CLK_DEBUG_PRINT__		
	UARTprintf("SetCLK_InvYC\r\n") ;
#endif
}
//------------------------------------------------------------------------------
void SetCLK_DivYC(BOOL clk_yc_div2_sel)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_ISPCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stISPCLK.clk_yc.clk_divider = clk_yc_div2_sel ;
		// Valid only when [clk_yc.clk_src_select = ¡®0x¡¯]
		// Need for 656 video input
		// Divider clock does not effect 'clk_yc'.
		// Divider clock effect 'clk_yc_m'.
		// [¡®0¡¯ : clk_yc_m is clk_yc/1 ] : Normal use
		// [¡®1¡¯ : clk_yc_m is clk_yc/2 ] : 656 video input
#if __CLK_DEBUG_PRINT__		
	UARTprintf("SetCLK_DivYC\r\n") ;
#endif
}
//------------------------------------------------------------------------------
void SetCLK_SrcSensor(WORD clk_sensor_in_sel)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_ISPCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stISPCLK.clk_sensor_in.clk_src_select = clk_sensor_in_sel ;
		// [¡®00¡¯ : CLK_B is used] : HDMI CLK
		// [¡®01¡¯ : CLK_A is used] : TTL CLK
		// [¡®10¡¯ : refer to stISPCLK.clk_lvds_sel] // clk_lvds_int
		// [¡®11¡¯ : LVDS serial clk divider clk is used] : See stISPCLK.clk_sensor_in.clk_divider
	
	// UARTprintf("SetCLK_SrcSensor\r\n") ;
}
//------------------------------------------------------------------------------
void SetCLK_SrcAux(WORD clk_m_aux_sel)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_AUXCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stAUXCLK.clk_m_aux_int.clk_src_select = clk_m_aux_sel ;
		// clk_m_aux_int.clk_src_select effects clock source of 'clk_in_aux', 'clk_aux_in', 'clk_aux_mid', 'clk_aux_out'  
		
		// i51x control value
		// [¡®00¡¯ : clk_in_aux_tm is used]
		// [¡®01¡¯ : clk_in_aux_tm/2 is used]
		// [¡®1x¡¯ : clk_bayer is used ]
		
		// i500 control value
		// [¡®00¡¯ : CLK_B is used] : HDMI CLK
		// [¡®01¡¯ : CLK_A is used] : TTL CLK
		// [¡®1x¡¯ : clk_bayer is used ]
}

//------------------------------------------------------------------------------
void SetCLK_InvAux(BOOL clk_aux_inv)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_AUXCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);

	stHostCLKCTRL.stAUXCLK.clk_in_aux.clk_invert = clk_aux_inv ;
}

//------------------------------------------------------------------------------
void SetCLK_SrcAuxOut(WORD clk_aux_out_sel)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_AUXCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);

	stHostCLKCTRL.stAUXCLK.clk_aux_out.clk_src_select = clk_aux_out_sel;	// 0:vclk, 1:venc_clk, 2:axclk
		// [¡®00¡¯ : refer to stVIDEOCLK.vclk.clk_src_select & stVIDEOCLK.vclk.clk_divide value] : vclk_src
		// [¡®01¡¯ : refer to stVIDEOCLK.clk_enc_int.clk_src_select value] : clk_enc_int
		// [¡®10¡¯ : refer to stAUXCLK.clk_m_aux_int.clk_src_select & stAUXCLK.clk_m_aux_int.clk_divider value] : clk_m_aux_int				
		// [¡®11¡¯ : refer to stAUXCLK.clk_aux_mclk.clk_src_select] : clk_aux_mclk		
}

//------------------------------------------------------------------------------
WORD GetCLK_SrcAuxOut(void)
{
	return stHostCLKCTRL.stAUXCLK.clk_aux_out.clk_src_select;	// 0:vclk, 1:venc_clk, 2:axclk, 3:aux_mclk
}

//------------------------------------------------------------------------------
void SetCLK_DivAux(BOOL	clk_m_aux_div2_sel)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_AUXCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	stHostCLKCTRL.stAUXCLK.clk_m_aux_int.clk_divider = clk_m_aux_div2_sel ;
		// Valid only when [clk_m_aux_int.clk_src_select = ¡®0x¡¯]
		// Divider clock does not effect 'clk_in_aux'.
		// Divider clock effect 'clk_aux_in', 'clk_aux_mid', 'clk_aux_out'.
		// [¡®0¡¯ : (clk_m_aux_int)/1 clk is used] 
		// [¡®1¡¯ : (clk_m_aux_int)/2 clk is used] 
}
//------------------------------------------------------------------------------
void SetCLK_SrcAuxlm(WORD clk_aux_lm_sel)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_AUXCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stAUXCLK.clk_aux_mid.clk_src_select = clk_aux_lm_sel ;
		// [¡®00¡¯ : refer to stAUXCLK.clk_m_aux_int.clk_src_select & stAUXCLK.clk_m_aux_int.clk_divider value] : clk_m_aux_int
		// [¡®01¡¯ : refer to stVIDEOCLK.clk_enc_int.clk_src_select value] : clk_enc
		// [¡®10¡¯ : refer to stVIDEOCLK.vclk.clk_src_select & stVIDEOCLK.vclk.clk_divide value] : vclk_src		
		// [¡®11¡¯ : refer to stAUXCLK.clk_aux_mclk.clk_src_select] : clk_aux_mclk		
}
//------------------------------------------------------------------------------
void SetCLK_SrcAuxIn(WORD clk_aux_in_sel)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_AUXCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stAUXCLK.clk_aux_in.clk_src_select = clk_aux_in_sel ;
		// [¡®00¡¯ : refer to stAUXCLK.clk_m_aux_int.clk_src_select & stAUXCLK.clk_m_aux_int.clk_divider value] : clk_m_aux_int
		// [¡®01¡¯ : refer to stVIDEOCLK.vclk.clk_src_select & stVIDEOCLK.vclk.clk_divide value] : vclk_src
		// [¡®10¡¯ : refer to stVIDEOCLK.clk_enc_int.clk_src_select value] : clk_enc_int
		// [¡®11¡¯ : refer to stAUXCLK.clk_aux_mclk.clk_src_select] : clk_aux_mclk
}
//------------------------------------------------------------------------------
void SetCLK_SrcNR(BOOL clk_nr_sel)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_ISPCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stISPCLK.clk_nr.clk_src_select = clk_nr_sel ; 
		// [ ¡®0¡¯ : clk_bayer is used ]
		// [ ¡®1¡¯ : clk_yc_m is used ]
		// Check "stISPCLK.clk_yc.clk_divider" value when using clk_yc_m clk to clk_nr
	
	// UARTprintf("SetCLK_SrcNR\r\n") ;
}

//------------------------------------------------------------------------------
void SetCLK_SrcENC(WORD venc_clk_sel)
{
	stHostCLKCTRL.stVIDEOCLK.clk_enc_int.clk_src_select = venc_clk_sel;
		// This value will determine source of encdac_clk&clk_in_enc 
		// [¡®00¡¯ : refer to stVIDEOCLK.vclk.clk_src_select&stVIDEOCLK.vclk.clk_divide value]
		// [¡®01¡¯ : X_tal is used]
		// [¡®10¡¯ : refer to clk_m_enc_int.clk_src_select value]
		// [¡®11¡¯ : X_tal2 is used]	

	stHostCLKCTRL.ctrl |= (HostCLKCTRL_VIDEOCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------
void SetCLK_SoftReset(void)
{
	MISP_RegField(MISP_HOST_ID, 0x039, 0, 1, 1);	// User pll disable
	MISP_SW_Reset();
	MISP_RegField(MISP_HOST_ID, 0x039, 0, 1, 0);	// User pll enable
}

//------------------------------------------------------------------------------
void SetCLK_ClkSensorInDivider(WORD val)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_ISPCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stISPCLK.clk_sensor_in.clk_divider = val ; // clock divider for LVDS serial clk
		// [ ¡®00¡¯ : 1/1 ] 
		// [ ¡®01¡¯ : 1/3 ] 
		// [ ¡®10¡¯ : 1/2 ] 
		// [ ¡®11¡¯ : 2/3 ]
#if __CLK_DEBUG_PRINT__			
	UARTprintf("SetCLK_ClkSensorInDivider\r\n") ;
#endif
}
//------------------------------------------------------------------------------
void SetCLK_BayerClkDivider(WORD val)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_ISPCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
		
	stHostCLKCTRL.stISPCLK.clk_bayer.clk_divider = val ;
		// When stISPCLK.clk_bayer.clk_src_select = ¡®11¡¯-- i5x0 rev only
		// When stISPCLK.clk_bayer.clk_src_select = ¡®100¡¯-- i500, i5x0
		// [ ¡®000¡¯ : Not allowed ]
		// [ ¡®001¡¯ : clk_sensor_in/1 is used]
		// [ ¡®N¡¯ : clk_sensor_in/N is used]
#if __CLK_DEBUG_PRINT__		
	UARTprintf("SetCLK_BayerClkDivider\r\n") ;
#endif
} 
//------------------------------------------------------------------------------
void SetCLK_OdmFifoClkDivider(WORD val)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_ISPCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stISPCLK.clk_odm_int.clk_divider = val ;
		// When stISPCLK.clk_odm.clk_src_select = ¡®10¡¯
		// [ ¡®000¡¯ : Not allowed ]
		// [ ¡®001¡¯ : clk_sensor_in/1 is used]
		// [ ¡®N¡¯ : clk_sensor_in/clk_odm_int.clk_divider is used]
#if __CLK_DEBUG_PRINT__		
	UARTprintf("SetCLK_OdmFifoClkDivider\r\n") ;
#endif
} 
//------------------------------------------------------------------------------
void SetCLK_ODMClkSelect(BOOL val)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_ISPCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stISPCLK.clk_odm_int.clk_src_select = val ;
	// i500, i5x0
		// [ ¡®0¡¯ : clk_bayer is used ]
		// [ ¡®1¡¯ : clk_sensor_in is used ]
		
	// i5x0 rev
		// [ ¡®00¡¯ : clk_bayer is used ] 		// For Normal or WDR Line mode
		// [ ¡®01¡¯ : clk_sensor_in is used ]	// For WDR Frame mode	
		// [ ¡®10¡¯ : clk_bayer/clk_odm_int.clk_divider is used ]	// For WDR Frame mode	
		// [ ¡®11¡¯ : prohibited ]		
	
#if __CLK_DEBUG_PRINT__		
	UARTprintf("SetCLK_ODMClkSelect\r\n") ;
#endif
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void SetCLK_UserPLLSrc(BYTE pll_in_sel)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_USPLL_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stUSERPLL.pll_in_sel = pll_in_sel ;
		// [¡®00¡¯ : LVDS serial clk is used is used] 
		// [¡®01¡¯ : X_tal is used] 
		// [¡®10¡¯ : clk_b_p or clk_a_p] 
		// [¡®11¡¯ : HOST_CLK is used]	
}
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
//------------------------------------------------------------------------------
void SetCLK_UserPLL(WORD P, WORD M, WORD S)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_USPLL_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stUSERPLL.pre_div = P ;
	stHostCLKCTRL.stUSERPLL.post_div = M ;
	stHostCLKCTRL.stUSERPLL.post_scale = S ;
}
#else 								// MDIN-i51X (i510, i540, i550)
//------------------------------------------------------------------------------
void SetCLK_UserPLL(WORD P, WORD N, WORD M, WORD OD, WORD S)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_USPLL_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stUSERPLL.pre_div = P ;
	stHostCLKCTRL.stUSERPLL.input_div = N ;
	stHostCLKCTRL.stUSERPLL.post_div = M ;
	stHostCLKCTRL.stUSERPLL.output_div = OD ;
	stHostCLKCTRL.stUSERPLL.post_scale = S ;

  #if 0  // debug print target value
	WORD OSC_IN = 27;
	WORD FIN = OSC_IN/P;
	WORD PLL_OUT = (FIN*M)/(N*(1<<OD));
	WORD FOUT = PLL_OUT/S;
	UARTprintf("### USER PLL : (%dMHz -> %dMHz)\r\n", OSC_IN, FOUT);
  #endif
}
//------------------------------------------------------------------------------
void SetCLK_UserPLLSrcSelect(BYTE pll_src_sel)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_USPLL_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stISPCLK.clk_user.clk_src_select = pll_src_sel;

	// [¡®000¡¯ : X_tal is used] 
	// [¡®001¡¯ : X_tal2 is used] 
	// [¡®01x¡¯ : HOST_CLK is used] 
	// [¡®100¡¯ : vclk pll int]	: see also clk divider 0x2F[14:10]
	// [¡®101¡¯ : mclk div4]	: see also clk divider 0x2F[9:5]
	// [¡®110¡¯ : lvds pll]		: see also clk divider 0x2F[4:0]

}
//------------------------------------------------------------------------------
void SetCLK_UserPLL_LockMode(BYTE mode)
{
	stHostCLKCTRL.stUSERPLL.lock_mode = mode;
	
	// [ 0 : free run ] 
	// [ 1 : Locked from standard output vclk ] 
	// [ 2 : Locked from non standard output vclk ] 
}
//------------------------------------------------------------------------------
void SetCLK_UserPLL_LockSrcPort(BYTE src_port)
{
	stHostCLKCTRL.stUSERPLL.lock_src_port = src_port;
	
	// [ 0 : Use MDIN-i5x0 internal path to user_clk out (only for standard case) ] 
	// [ 1 : From host clock pin to user_clk out ] 
	// [ 2 : From aux(ext) port pin to user_clk_out ] 
}
BYTE GetCLK_UserPLL_LockMode(void)
{
	return stHostCLKCTRL.stUSERPLL.lock_mode;
}
//------------------------------------------------------------------------------
BYTE GetCLK_UserPLL_LockSrcPort(void)
{
	return stHostCLKCTRL.stUSERPLL.lock_src_port;
}
//------------------------------------------------------------------------------
void GetCLK_UserPLL(PWORD pP, PWORD pN, PWORD pM, PWORD pOD, PWORD pS)
{
	*pP = stHostCLKCTRL.stUSERPLL.pre_div;
	*pN = stHostCLKCTRL.stUSERPLL.input_div;
	*pM = stHostCLKCTRL.stUSERPLL.post_div;
	*pOD = stHostCLKCTRL.stUSERPLL.output_div;
	*pS = stHostCLKCTRL.stUSERPLL.post_scale;
}
#endif

#if defined(__USE_MDIN_i500__) 		// MDIN-i500
//------------------------------------------------------------------------------
void SetCLK_VideoPLL(WORD P, WORD M, WORD S)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_VPLL_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stVPLL.pre_div = P ;
	stHostCLKCTRL.stVPLL.post_div = M ;
	stHostCLKCTRL.stVPLL.post_scale = S ;
}
#else 								// MDIN-i51X (i510, i540, i550)
//------------------------------------------------------------------------------
void SetCLK_VideoPLL(WORD P, WORD N, WORD M, WORD OD, WORD S)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_VPLL_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	if(stHostCLKCTRL.stVPLL.pll_bypass==1)		{P=1;	S=1;}	// bypass 'pre_div', 'post_scale'
	
	stHostCLKCTRL.stVPLL.pre_div = P ;
	stHostCLKCTRL.stVPLL.input_div = N ;
	stHostCLKCTRL.stVPLL.post_div = M ;
	stHostCLKCTRL.stVPLL.output_div = OD ;
	stHostCLKCTRL.stVPLL.post_scale = S ;
}

//------------------------------------------------------------------------------
#if defined(__USE_MDIN_i5XX_REV__)
void SetCLK_SrcSensorSlave(BYTE clk_slave_sel)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_ISPCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stISPCLK.clk_slave.clk_src_select = clk_slave_sel ;
}
#endif // __USE_MDIN_i5XX_REV__

#endif

//------------------------------------------------------------------------------
void SetCLK_MemoryPLL(WORD P, WORD M, WORD S)
{
	if ( stHostCLKCTRL.ADSTSR_UCOM_EN == 0 ) // 0: When use external cpu
	{
		stHostCLKCTRL.ctrl |= (HostCLKCTRL_MPLL_UPDATE | HostCLKCTRL_TRIGGER_ISP);	
		
		stHostCLKCTRL.stMPLL.pre_div = P ;
		stHostCLKCTRL.stMPLL.post_div = M ;
		stHostCLKCTRL.stMPLL.post_scale = S ;
	}
}

//------------------------------------------------------------------------------
void SetCLK_AuxVideoPLL(BYTE S, WORD F, WORD T)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_AUXCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
//	division ratio = (S+F/T) or (S+1-F/T)
	stHostCLKCTRL.stAUXCLK.clk_aux_mclk.clk_divide_param.clk_t = T;
	stHostCLKCTRL.stAUXCLK.clk_aux_mclk.clk_divide_param.clk_f = F;
	stHostCLKCTRL.stAUXCLK.clk_aux_mclk.clk_divide_param.clk_divider = S;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void SetCLK_ClkAFOnOff(BOOL val)
{
	WORD clk_power_save ;

	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_1 & 0xFFFB ) ;
	
	stHostCLKCTRL.clk_power_save_1 = clk_power_save|(val<<2) ;
	
}
//------------------------------------------------------------------------------
void SetCLK_ClkWBOnOff(BOOL val)
{
	WORD clk_power_save ;

	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_1 & 0xFFFD ) ;
	
	stHostCLKCTRL.clk_power_save_1 = clk_power_save|(val<<1) ;
	
}
//------------------------------------------------------------------------------
void SetCLK_ClkAEOnOff(BOOL val)
{
	WORD clk_power_save ;

	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_1 & 0xFFFE ) ;
	
	stHostCLKCTRL.clk_power_save_1 = clk_power_save|(val<<0) ;
	
}
//------------------------------------------------------------------------------
void SetCLK_ClkTMOnOff(BOOL val)
{
	WORD clk_power_save ;

	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_1 & 0xFFBF ) ;
	
	stHostCLKCTRL.clk_power_save_1 = clk_power_save|(val<<6) ;
	
}
//------------------------------------------------------------------------------
void SetCLK_ClkMGROnOff(BOOL val)
{
	WORD clk_power_save ;

	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_1 & 0xFFDF ) ;
	
	stHostCLKCTRL.clk_power_save_1 = clk_power_save|(val<<5) ;
	
}
//------------------------------------------------------------------------------
void SetCLK_ClkWDROnOff(BOOL val)
{
	WORD clk_power_save ;

	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_1 & 0xFFEF ) ;
	
	stHostCLKCTRL.clk_power_save_1 = clk_power_save|(val<<4) ;
	
}
//------------------------------------------------------------------------------
void SetCLK_ClkMDOnOff(BOOL val)
{
	WORD clk_power_save ;

	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	clk_power_save = (stHostCLKCTRL.clk_power_save_1 & 0xFDFF ) ;
	
	stHostCLKCTRL.clk_power_save_1 = clk_power_save|(val<<9) ;
#else 												// MDIN-i51X (i510, i540, i550)
	clk_power_save = (stHostCLKCTRL.clk_power_save_1 & 0xFFF7 ) ;
	
	stHostCLKCTRL.clk_power_save_1 = clk_power_save|(val<<3 ) ;	
#endif	
}
//------------------------------------------------------------------------------
void SetCLK_ClkNRcOnOff(BOOL val)
{
	WORD clk_power_save ;

	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	clk_power_save = (stHostCLKCTRL.clk_power_save_1 & 0xFEFF ) ;
	
	stHostCLKCTRL.clk_power_save_1 = clk_power_save|(val<<8) ;
#else 												// MDIN-i51X (i510, i540, i550)
	clk_power_save = (stHostCLKCTRL.clk_power_save_1 & 0xFF7F ) ;
	
	stHostCLKCTRL.clk_power_save_1 = clk_power_save|(val<<7 ) ;	
#endif	
}
//------------------------------------------------------------------------------
void SetCLK_ClkWDRRdOnOff(BOOL val)
{
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	WORD clk_power_save ;

	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);

	clk_power_save = (stHostCLKCTRL.clk_power_save_1 & 0xFEFF ) ;
	
	stHostCLKCTRL.clk_power_save_1 = clk_power_save|(val<<8 ) ;	
#endif	
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void SetCLK_ClkNROnOff(BOOL val)
{
	WORD clk_power_save ;
	BYTE OnOff ;
	
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	if ( val == 1 ) OnOff = 0 ; else OnOff = 1 ;
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_0 & 0x7FFF ) ;
	
	stHostCLKCTRL.clk_power_save_0 = clk_power_save|(OnOff<<15) ;	
}
//------------------------------------------------------------------------------
void SetCLK_ClkYCmOnOff(BOOL val)
{
	WORD clk_power_save ;
	BYTE OnOff ;
	
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	if ( val == 1 ) OnOff = 0 ; else OnOff = 1 ;
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_0 & 0xBFFF ) ;
	
	stHostCLKCTRL.clk_power_save_0 = clk_power_save|(OnOff<<14) ;	
}
//------------------------------------------------------------------------------
void SetCLK_ClkLVDSTxOnOff(BOOL val)
{
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	WORD clk_power_save ;
	BYTE OnOff ;
	
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	if ( val == 1 ) OnOff = 0 ; else OnOff = 1 ;
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_0 & 0xDFFF ) ;
	
	stHostCLKCTRL.clk_power_save_0 = clk_power_save|(OnOff<<13) ;
#endif
}
//------------------------------------------------------------------------------
void SetCLK_ClkLVDSInOnOff(BOOL val)
{
	WORD clk_power_save ;
	BYTE OnOff ;
	
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	if ( val == 1 ) OnOff = 0 ; else OnOff = 1 ;
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_0 & 0xEFFF ) ;
	
	stHostCLKCTRL.clk_power_save_0 = clk_power_save|(OnOff<<12) ;	
}
//------------------------------------------------------------------------------
void SetCLK_ClkSensorInOnOff(BOOL val)
{
	WORD clk_power_save ;
	BYTE OnOff ;
	
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	if ( val == 1 ) OnOff = 0 ; else OnOff = 1 ;
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_0 & 0xF7FF ) ;
	
	stHostCLKCTRL.clk_power_save_0 = clk_power_save|(OnOff<<11) ;	
}
//------------------------------------------------------------------------------

void SetCLK_ClkAudioOnOff(BOOL val)
{
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	WORD clk_power_save ;
	BYTE OnOff ;
	
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	if ( val == 1 ) OnOff = 0 ; else OnOff = 1 ;
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_0 & 0xFBFF ) ;
	
	stHostCLKCTRL.clk_power_save_0 = clk_power_save|(OnOff<<10) ;	
#endif
}
//------------------------------------------------------------------------------
void SetCLK_ClkYCOnOff(BOOL val)
{
	WORD clk_power_save ;
	BYTE OnOff ;
	
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	if ( val == 1 ) OnOff = 0 ; else OnOff = 1 ;
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_0 & 0xFDFF ) ;
	
	stHostCLKCTRL.clk_power_save_0 = clk_power_save|(OnOff<<9) ;	
}
//------------------------------------------------------------------------------
void SetCLK_ClkAUXMidOnOff(BOOL val)
{
	WORD clk_power_save ;
	BYTE OnOff ;
	
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	if ( val == 1 ) OnOff = 0 ; else OnOff = 1 ;
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_0 & 0xFEFF ) ;
	
	stHostCLKCTRL.clk_power_save_0 = clk_power_save|(OnOff<<8) ;	
}
//------------------------------------------------------------------------------
void SetCLK_ClkAUXInOnOff(BOOL val)
{
#if defined(__USE_MDIN_i500__)	// Only for MDIN i500 	
	WORD clk_power_save ;
	BYTE OnOff ;
	
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	if ( val == 1 ) OnOff = 0 ; else OnOff = 1 ;
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_0 & 0xFF7F ) ;
	
	stHostCLKCTRL.clk_power_save_0 = clk_power_save|(OnOff<<7) ;
#endif
}
//------------------------------------------------------------------------------
void SetCLK_ClkEncOnOff(BOOL val)
{
	WORD clk_power_save ;
	BYTE OnOff ;
	
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	if ( val == 1 ) OnOff = 0 ; else OnOff = 1 ;
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_0 & 0xFFBF ) ;
	
	stHostCLKCTRL.clk_power_save_0 = clk_power_save|(OnOff<<6) ;	
}
//------------------------------------------------------------------------------
void SetCLK_ClkAUXOutOnOff(BOOL val)
{
	WORD clk_power_save ;
	BYTE OnOff ;
	
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	if ( val == 1 ) OnOff = 0 ; else OnOff = 1 ;
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_0 & 0xFFDF ) ;
	
	stHostCLKCTRL.clk_power_save_0 = clk_power_save|(OnOff<<5) ;	
}
//------------------------------------------------------------------------------
void SetCLK_ClkMemoryOnOff(BOOL val)
{
	WORD clk_power_save ;
	BYTE OnOff ;
	
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	if ( val == 1 ) OnOff = 0 ; else OnOff = 1 ;
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_0 & 0xFFEF ) ;
	
	stHostCLKCTRL.clk_power_save_0 = clk_power_save|(OnOff<<4) ;	
}
//------------------------------------------------------------------------------
void SetCLK_ClkDISOnOff(BOOL val)
{
	WORD clk_power_save ;
	BYTE OnOff ;
	
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	if ( val == 1 ) OnOff = 0 ; else OnOff = 1 ;
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_0 & 0xFFF7 ) ;
	
	stHostCLKCTRL.clk_power_save_0 = clk_power_save|(OnOff<<3) ;	
}
//------------------------------------------------------------------------------
void SetCLK_ClkInAUXOnOff(BOOL val)
{
	WORD clk_power_save ;
	BYTE OnOff ;	
		
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	if ( val == 1 ) OnOff = 0 ; else OnOff = 1 ;

#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	clk_power_save = (stHostCLKCTRL.clk_power_save_0 & 0xFFFB ) ;
	
	stHostCLKCTRL.clk_power_save_0 = clk_power_save|(OnOff<<2) ;	
#else 												// MDIN-i51X (i510, i540, i550)
	clk_power_save = (stHostCLKCTRL.clk_power_save_0 & 0xFF7F ) ;
	
	stHostCLKCTRL.clk_power_save_0 = clk_power_save|(OnOff<<7) ;		
#endif	
}

//------------------------------------------------------------------------------
void SetCLK_ClkVideoOutOnOff(BOOL val)
{
	WORD clk_power_save ;
	BYTE OnOff ;
	
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	if ( val == 1 ) OnOff = 0 ; else OnOff = 1 ;
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_0 & 0xFFFD ) ;
	
	stHostCLKCTRL.clk_power_save_0 = clk_power_save|(OnOff<<1) ;	
}
//------------------------------------------------------------------------------
void SetCLK_ClkVideoOnOff(BOOL val)
{
	WORD clk_power_save ;
	BYTE OnOff ;
	
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_CLK_ON_OFF_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	if ( val == 1 ) OnOff = 0 ; else OnOff = 1 ;
	
	clk_power_save = (stHostCLKCTRL.clk_power_save_0 & 0xFFFE ) ;
	
	stHostCLKCTRL.clk_power_save_0 = clk_power_save|(OnOff<<0) ;	
}

//------------------------------------------------------------------------------
void SetCLK_SrcVCLK(BYTE vclk_src_sel)
{
#if defined(__USE_MDIN_i5XX_REV__)		
	stHostCLKCTRL.stVIDEOCLK.vclk.clk_src_select = (vclk_src_sel&0x07);
		// [¡®xx0¡¯ : VPLL out clock is used] : VPLL CLK
		// [¡®001¡¯ : CLK_B is used] : HDMI CLK
		// [¡®011¡¯ : CLK_A is used] : TTL CLK
		// [¡®101¡¯ : refer to stISPCLK.clk_lvds_sel value] // clk_lvds_int
		// [¡®111¡¯ : LVDS serial clk is used]		
#else
	
	stHostCLKCTRL.stVIDEOCLK.vclk.clk_src_select = (vclk_src_sel&0x01);
	stHostCLKCTRL.stVPLL.pll_sel_inclk = ((vclk_src_sel>>1)&0x03) ;
	
		// [¡®0¡¯ : from video PLL output]
		// [¡®1¡¯ : refer to stVPLL.pll_sel_inclk value]
		
			// stVPLL.pll_sel_inclk
		// [¡®00¡¯ : CLK_B is used] : HDMI CLK
		// [¡®01¡¯ : CLK_A is used] : TTL CLK
		// [¡®10¡¯ : refer to stISPCLK.clk_lvds_sel value] // clk_lvds_int
		// [¡®11¡¯ : LVDS serial clk is used]		
	
	stHostCLKCTRL.ctrl |= HostCLKCTRL_VPLL_UPDATE ;
#endif
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_VIDEOCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
}
//------------------------------------------------------------------------------
void SetCLK_VCLKDly(BYTE delay)
{
	stHostCLKCTRL.stVIDEOCLK.vclk.clk_delay = delay;
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_VIDEOCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
}
//------------------------------------------------------------------------------
void SetCLK_VCLKInv(BOOL invert)
{
	stHostCLKCTRL.stVIDEOCLK.vclk.clk_invert = invert;
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_VIDEOCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
}
//------------------------------------------------------------------------------

//add by jins 2016.03

//------------------------------------------------------------------------------
void SetCLK_VCLKOutDly(BYTE delay)
{
	stHostCLKCTRL.stVIDEOCLK.vclk_out.clk_delay = delay;
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_VIDEOCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------
void SetCLK_VCLKOutInv(BOOL invert)
{
	stHostCLKCTRL.stVIDEOCLK.vclk_out.clk_invert = invert;
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_VIDEOCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
}

//-------------------------------------------------------------------------------------
void SetCLK_VCLKOutiDly(BYTE delay)
{
	stHostCLKCTRL.stVIDEOCLK.vclk_out_i.clk_delay = delay;
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_VIDEOCLK_UPDATE| HostCLKCTRL_TRIGGER_ISP);
}

//-------------------------------------------------------------------------------------
void SetCLK_VCLKOutiInv(BOOL invert)
{
	stHostCLKCTRL.stVIDEOCLK.vclk_out_i.clk_invert = invert;
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_VIDEOCLK_UPDATE| HostCLKCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------
void SetCLK_VCLKAuxOutDly(BYTE delay)
{
	stHostCLKCTRL.stVIDEOCLK.vclk_out_b.clk_delay = delay;
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_VIDEOCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------
void SetCLK_VCLKAuxOutInv(BOOL invert)
{
	stHostCLKCTRL.stVIDEOCLK.vclk_out_b.clk_invert = invert;
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_VIDEOCLK_UPDATE | HostCLKCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------
BYTE GetCLK_VCLKOutDly(void)
{
	return stHostCLKCTRL.stVIDEOCLK.vclk_out.clk_delay;
}

//------------------------------------------------------------------------------
BYTE GetCLK_VCLKOutInv(void)
{
	return stHostCLKCTRL.stVIDEOCLK.vclk_out.clk_invert;
}

//------------------------------------------------------------------------------
BYTE GetCLK_VCLKAuxOutDly(void)
{
	return stHostCLKCTRL.stVIDEOCLK.vclk_out_b.clk_delay;
}

//------------------------------------------------------------------------------
BYTE GetCLK_VCLKAuxOutInv(void)
{
	return stHostCLKCTRL.stVIDEOCLK.vclk_out_b.clk_invert;
}

//-------------------------------------------------------------------------------------
void SetCLK_DlyYC(BYTE delay)
{
	stHostCLKCTRL.stISPCLK.clk_yc.clk_delay = delay;
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_ISPCLK_UPDATE| HostCLKCTRL_TRIGGER_ISP);
#if __CLK_DEBUG_PRINT__	
	UARTprintf("SetCLK_DlyYC\r\n") ;
#endif
}

//------------------------------------------------------------------------------
void SetCLK_DlyAux(BYTE delay)
{
	stHostCLKCTRL.stAUXCLK.clk_in_aux.clk_delay = delay;
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_AUXCLK_UPDATE| HostCLKCTRL_TRIGGER_ISP);
}
//-------------------------------------------------------------------------------------
void SetCLK_InvYCm(BOOL clk_yc_m_inv)
{
	stHostCLKCTRL.stISPCLK.clk_yc_m.clk_invert = clk_yc_m_inv;
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_ISPCLK_UPDATE| HostCLKCTRL_TRIGGER_ISP);
#if __CLK_DEBUG_PRINT__		
	UARTprintf("SetCLK_InvYCm\r\n") ;
#endif
}

//-------------------------------------------------------------------------------------
void SetCLK_DlyYCm(BYTE delay)
{
	stHostCLKCTRL.stISPCLK.clk_yc_m.clk_delay = delay;
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_ISPCLK_UPDATE| HostCLKCTRL_TRIGGER_ISP);
#if __CLK_DEBUG_PRINT__		
	UARTprintf("SetCLK_DlyYCm\r\n") ;
#endif	
}

//-------------------------------------------------------------------------------------
BYTE GetCLK_DlyYC(void)
{
	return stHostCLKCTRL.stISPCLK.clk_yc.clk_delay;
}

//------------------------------------------------------------------------------
BYTE GetCLK_DlyAux(void)
{
	return stHostCLKCTRL.stAUXCLK.clk_in_aux.clk_delay;
}

//------------------------------------------------------------------------------
BOOL GetCLK_InvYC(void)
{
	return stHostCLKCTRL.stISPCLK.clk_yc.clk_invert;
}

//------------------------------------------------------------------------------
BOOL GetCLK_InvAux(void)
{
	return stHostCLKCTRL.stAUXCLK.clk_in_aux.clk_invert;
}

//------------------------------------------------------------------------------
BYTE GetCLK_SrcYC(void)
{
	return stHostCLKCTRL.stISPCLK.clk_yc.clk_src_select;
}

//jins, temp (clock invert for FPGA B'd)
//------------------------------------------------------------------------------
BYTE GetCLK_InvYCDef(BYTE nID)
{
	return clk_inv_m[nID];
}

//------------------------------------------------------------------------------
BYTE GetCLK_InvAuxDef(BYTE nID)
{
	return clk_inv_a[nID];
}

//------------------------------------------------------------------------------
void SetCLK_SrcVpllHostCLK(void)
{
#if defined(__USE_MDIN_i5XX_REV__)				// i5xx rev chip
	stHostCLKCTRL.stVPLL.pll_in_x_tal_sel = 2;	// '0' : x_tal,  '1' : x_tal2,  '1x' : host_clk
#else											// i5xx org chip
	stHostCLKCTRL.stVPLL.pll_sel_xtal = 0 ;		// '0' : pll_sel_hclk,  '1' : X_tal
	stHostCLKCTRL.stVPLL.pll_sel_hclk = 1 ; 	// '0' : pll_sel_inclk,  '1' : host_clk
#endif
	
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_VPLL_UPDATE| HostCLKCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------
void SetCLK_SrcVpllXtal(void)
{
#if defined(__USE_MDIN_i5XX_REV__)				// i5xx rev chip
	stHostCLKCTRL.stVPLL.pll_in_x_tal_sel = 0;	// '0' : x_tal,  '1' : x_tal2,  '1x' : host_clk
#else											// i5xx org chip
	stHostCLKCTRL.stVPLL.pll_sel_xtal = 1 ;		// '0' : pll_sel_hclk,  '1' : X_tal
	stHostCLKCTRL.stVPLL.pll_sel_hclk = 0 ; 	// '0' : pll_sel_inclk,  '1' : host_clk
#endif
	
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_VPLL_UPDATE| HostCLKCTRL_TRIGGER_ISP);
}

//------------------------------------------------------------------------------
void SetCLK_SetBypassVideoPLL(void)
{
	stHostCLKCTRL.ctrl |= (HostCLKCTRL_VPLL_UPDATE | HostCLKCTRL_TRIGGER_ISP);
	
	stHostCLKCTRL.stVPLL.pll_bypass = 1;
	stHostCLKCTRL.stVPLL.pll_en_n = 1;			// '0' : enable pll,  '1' : disable pll
}

/*  FILE_END_HERE */
