// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"
#include	"osd.h"
// #include	"app_lowlight_control.h"

#if defined(__USE_MDIN_i510__)||defined(__USE_MDIN_i540__)||defined(__USE_MDIN_i550__)
// ----------------------------------------------------------------------
//
// 	Low light control is API base application of i5x0 series.
//  Low light Auto saturation  / Low light Color suppress / Low light Edge enhancement
//
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
MDIN_LL_CONTROL_PARAM stLLControl ;
// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
// extern BYTE target_bright ; // From Demo.c FOR reference
// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
static void LLCTRL_StatusUpdate(PMDIN_LL_CONTROL_PARAM pCtrl) ;

static void LLCTRL_AutoSatTrace(PMDIN_LL_CONTROL_PARAM pCtrl) ;
static void LLCTRL_AutoSatSetCompValue(PMDIN_LL_CONTROL_PARAM pCtrl);

static void LLCTRL_CSPTrace(PMDIN_LL_CONTROL_PARAM pCtrl) ;
static void LLCTRL_CSPSetValue(PMDIN_LL_CONTROL_PARAM pCtrl);

static void LLCTRL_AutoEdge(PMDIN_LL_CONTROL_PARAM pCtrl);
static void LLCTRL_AutoEETrace(PMDIN_LL_CONTROL_PARAM pCtrl);

static void LLCTRL_AutoCAC(PMDIN_LL_CONTROL_PARAM pCtrl);
static void LLCTRL_AutoCACTrace(PMDIN_LL_CONTROL_PARAM pCtrl);

// ------------------------
static void LLCTRL_CSPCalcMaxTarget(PBYTE x0, PBYTE x1, WORD stdev_int );
static void LLCTRL_CSPCalcMidTarget(PBYTE x0, PBYTE x1, WORD stdev_int );
// ----------------------------------------------------------------------
static void LLCTRL_AutoEECalcMaxTarget(PBYTE comp_value, WORD stdev_int );
static void LLCTRL_AutoEECalcMidTarget(PBYTE comp_value, WORD stdev_int );
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------
#if __LLCNTL_DEBUG_PRINT__
static void LLCTRL_StatusDebug(PMDIN_LL_CONTROL_PARAM pCtrl)
{
	UARTprintf("***LLC STAT****\r\n") ;
	UARTprintf("[Ysum:%d], [Get_AE_Ysum():%d], [Yref:%d]\r\n", (pCtrl->stSTAT.gain_Ysum) ,Get_AE_Ysum(), Get_AE_Yref() ) ;
	UARTprintf("[Gain:%d], [dGain:%d], [Gain_factor:%d] [MaxGain:%d]\r\n", (pCtrl->stSTAT.gain), (pCtrl->stSTAT.dgain), (pCtrl->stSAT.AutoSat_gain_level), (pCtrl->stSTAT.max_gain)) ;
	UARTprintf("[pstdev:%d], [pfrac:%d]\r\n", (pCtrl->stSTAT.pre_stdev_int) ,(pCtrl->stSTAT.pre_stdev_frac)) ;			
	UARTprintf("[stdev:%d],  [frac:%d]\r\n", (pCtrl->stSTAT.stdev_int) ,(pCtrl->stSTAT.stdev_frac)) ;			
	UARTprintf("***AutoSat****\r\n") ;
	UARTprintf("[sat_trace_speed:%d]\r\n", (pCtrl->stSAT.AutoSat_trace_speed)) ;	
	UARTprintf("[active_level:%d],[normal_sat:%d], [trace_sat:%d], [sat_compensation:%d]\r\n", (pCtrl->stSAT.AutoSat_active_level),(pCtrl->stSAT.AutoSat_normal_value),(pCtrl->stSAT.AutoSat_trace_value),(pCtrl->stSAT.AutoSat_comp_value)) ;	
	UARTprintf("***AutoCSP****\r\n") ;
	UARTprintf("[x0_target:%d], [x1_target:%d]\r\n", pCtrl->stCSP.x0_target ,pCtrl->stCSP.x1_target) ;
	UARTprintf("***AutoEDGE****\r\n") ;
	UARTprintf("[EE_trace_speed:%d]\r\n", (pCtrl->stEE.AutoEDGE_trace_speed)) ;	
	UARTprintf("[normal_EE:%d], [trace_EE:%d], [EE_compensation:%d]\r\n", (pCtrl->stEE.AutoEDGE_normal_value),(pCtrl->stEE.AutoEDGE_trace_value),(pCtrl->stEE.AutoEDGE_comp_value)) ;	
	UARTprintf("***AutoCAC****\r\n") ;
	UARTprintf("[g_normal:%d], [m_normal:%d]\r\n", pCtrl->stCAC.AutoCAC_g_normal ,pCtrl->stCAC.AutoCAC_m_normal) ;
	UARTprintf("[g_comp:%d],   [m_compl:%d]\r\n", pCtrl->stCAC.AutoCAC_g_comp_value ,pCtrl->stCAC.AutoCAC_m_comp_value) ;
	UARTprintf("[g_trace:%d],   [m_trace:%d]\r\n", pCtrl->stCAC.AutoCAC_g_trace_value ,pCtrl->stCAC.AutoCAC_m_trace_value) ;	
}
#endif
// ----------------------------------------------------------------------
static void LLCTRL_CSPCalcMaxTarget(PBYTE x0, PBYTE x1, WORD stdev_int)
{ // CSP target calcualtion AT MAX GAIN POINT or DGAIN POINT
	
	BYTE x0_target ;
	BYTE x1_target ;
	
	#if defined(__LL_AUTOCSP_EQ_1__)
		// x0_target = stdev_int + (stdev_int>>1) ; 
		// x1_target = stdev_int + x0_target ; 
		x0_target = (stdev_int>>1) ; 
		x1_target = stdev_int + x0_target ; 		
	#endif		
	
	#if defined(__LL_AUTOCSP_EQ_2__)
		x0_target = stdev_int>>2 ; 
		x1_target = stdev_int>>1 ; 
	#endif	
	
	*x0 = x0_target ;
	*x1 = x1_target ;	
}
// ----------------------------------------------------------------------
static void LLCTRL_CSPCalcMidTarget(PBYTE x0, PBYTE x1, WORD stdev_int )
{ // CSP target calcualtion AT MID GAIN POINT
	BYTE x0_target ;
	BYTE x1_target ;

	#if defined(__LL_AUTOCSP_EQ_1__) // EQ for UHD
		if (stdev_int >= __LL_AUTOCSP_MID_NPOINT_)
		{
			// x0_target = stdev_int ; 
			// x1_target = stdev_int + x0_target ;
			x0_target = stdev_int>>2 ;
			x1_target = stdev_int + x0_target ;			
		}
		else
		{
			x0_target = 0 ; 
			x1_target = stdev_int + x0_target ;		
		}				
	#endif
	
	#if defined(__LL_AUTOCSP_EQ_2__) // // EQ for FHD
		if (stdev_int >= __LL_AUTOCSP_MID_NPOINT_)
		{
			x0_target = stdev_int>>1 ; 
			x1_target = stdev_int + x0_target ;
		}
		else
		{
			x0_target = 0 ;
			x1_target = stdev_int ; 		
		}
	#endif	
	
	*x0 = x0_target ;
	*x1 = x1_target ;
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
static void LLCTRL_AutoEECalcMaxTarget(PBYTE comp_value, WORD stdev_int )
{
	BYTE edge_compensation ;

	#if defined(__LL_AUTOEDGE_EQ_1__) // EQ for UHD
		if ( stdev_int > __LL_AUTOEDGE_EQ_NL__ ) edge_compensation = stdev_int - __LL_AUTOEDGE_EQ_NL__ ;
		else edge_compensation = 0;	
	#endif	
	
	#if defined(__LL_AUTOEDGE_EQ_2__) // EQ for FHD 1/2" sensor
		if( stdev_int > __LL_AUTOEDGE_EQ_NL_1_)	edge_compensation = stdev_int + __LL_AUTOEDGE_EQ_COMP_1__;			
		else if( stdev_int > __LL_AUTOEDGE_EQ_NL_2_)	edge_compensation = stdev_int + __LL_AUTOEDGE_EQ_COMP_2__;			
		else if( stdev_int > __LL_AUTOEDGE_EQ_NL_3_)	edge_compensation = stdev_int + __LL_AUTOEDGE_EQ_COMP_3__;
		else 			edge_compensation = 0;
	#endif	
	
	#if defined(__LL_AUTOEDGE_EQ_3__) // EQ for FHD 1/3" sensor
		if ( stdev_int > __LL_AUTOEDGE_EQ_NL__ ) edge_compensation = stdev_int - __LL_AUTOEDGE_EQ_COMP__ ;
		else edge_compensation = 0;				
	#endif	
	
	*comp_value = edge_compensation ;
}
// ----------------------------------------------------------------------
static void LLCTRL_AutoEECalcMidTarget(PBYTE comp_value, WORD stdev_int )
{
	BYTE edge_compensation ;
	
	#if defined(__LL_AUTOEDGE_EQ_1__) // EQ for UHD
		edge_compensation = 0;
	#endif	
	
	#if defined(__LL_AUTOEDGE_EQ_2__) // EQ for FHD 1/2" sensor
		if( stdev_int > __LL_AUTOEDGE_EQ_NL_1_)	edge_compensation = stdev_int + __LL_AUTOEDGE_EQ_COMP_1__;			
		else if( stdev_int > __LL_AUTOEDGE_EQ_NL_2_)	edge_compensation = stdev_int + __LL_AUTOEDGE_EQ_COMP_2__;			
		else if( stdev_int > __LL_AUTOEDGE_EQ_NL_3_)	edge_compensation = stdev_int + __LL_AUTOEDGE_EQ_COMP_3__;
		else 			edge_compensation = 0;
	#endif	
	
	#if defined(__LL_AUTOEDGE_EQ_3__) // EQ for FHD 1/3" sensor			
		if ( stdev_int > __LL_AUTOEDGE_EQ_NL__ ) edge_compensation = stdev_int - __LL_AUTOEDGE_EQ_COMP__ ;
		else edge_compensation = 0;			
	#endif	
		
	*comp_value = edge_compensation ;
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
static void LLCTRL_StatusUpdate(PMDIN_LL_CONTROL_PARAM pCtrl)
{
	if ( pCtrl->stSTAT.period_counter == 0 )
	{
		pCtrl->stSTAT.gain_Yref =  Get_AE_Yref();	
		pCtrl->stSTAT.gain_Ysum = Get_AE_Ysum() ;
		// pCtrl->stSTAT.gain = AECTRL_GetGAINGain() ; // Total gain
		pCtrl->stSTAT.gain = AECTRL_GetGAINAGain() ; // Sensor gain only 
		pCtrl->stSTAT.dgain = Get_AE_DGain() ; // digital gain only
		pCtrl->stSTAT.max_gain = AECTRL_GetGAINPMax() ;
		
		GetNR_StDEV(&(pCtrl->stSTAT.pre_stdev_int),&(pCtrl->stSTAT.pre_stdev_frac),1); // From struct
		GetNR_StDEV(&(pCtrl->stSTAT.stdev_int),&(pCtrl->stSTAT.stdev_frac),0); // From reg	
		
		if ( pCtrl->stSTAT.gain == pCtrl->stSTAT.max_gain && pCtrl->stSTAT.max_gain == 256 )	
			pCtrl->stSTAT.GainCTLMode = AECTRL_GAINCTL_OFF ;
		else										
			pCtrl->stSTAT.GainCTLMode = AECTRL_GAINCTL_MAX ;			

		if ( ((pCtrl->stSTAT.gain) >= (pCtrl->stSTAT.max_gain)) || (pCtrl->stSTAT.dgain > 256) ) pCtrl->stSTAT.is_gain_max = 1 ;
		else pCtrl->stSTAT.is_gain_max = 0 ;		
		
		#if !(__LL_USE_AUTOSAT_DEFINED_GAIN_LEVEL_)
		switch (pCtrl->stSAT.AutoSat_intensity)
		{		
			case 1 : pCtrl->stSAT.AutoSat_gain_level = (pCtrl->stSTAT.max_gain)/4 ; break ; // intensity low
			case 2 : pCtrl->stSAT.AutoSat_gain_level = (pCtrl->stSTAT.max_gain)/8 ; break ; // intensity mid
			case 3 : pCtrl->stSAT.AutoSat_gain_level = (pCtrl->stSTAT.max_gain)/16 ; break ; // intensity high		
			default : break ;		
		}
		#endif // #if !(__LL_USE_AUTOSAT_DEFINED_GAIN_LEVEL_)
		
		pCtrl->stSTAT.period_counter = __LL_CONTROL_PERIOD__ ; // reset period counter
		
		pCtrl->ctrl |= (LLCTRL_TRIGGER_ISP);

		#if __LLCNTL_DEBUG_PRINT__
			LLCTRL_StatusDebug(pCtrl); //for debugging
		#endif		
	}
	else
	{
		pCtrl->stSTAT.period_counter = (pCtrl->stSTAT.period_counter) - 1 ;		
	}
}

// ------------------------------------------
static void LLCTRL_AutoSatSetCompValue(PMDIN_LL_CONTROL_PARAM pCtrl)
{
	WORD stdev_int ;
	WORD stdev_frac ;
	WORD pre_stdev_int ;
	WORD pre_stdev_frac ;		
	
	BYTE AutoSat_active_level ;
	BYTE AutoSat_compensation ;
	BYTE AutoSat_comp_value ;
	LONG AutoSat_gain_level ;
	
	if ( pCtrl->stSAT.AutoSat_intensity == 0 ) 
	{
		pCtrl->stSAT.AutoSat_comp_value = pCtrl->stSAT.AutoSat_normal_value ;
		return ; // off 
	}
	
	stdev_int = pCtrl->stSTAT.stdev_int ;
	stdev_frac = pCtrl->stSTAT.stdev_frac ;
	
	pre_stdev_int = pCtrl->stSTAT.pre_stdev_int ;
	pre_stdev_frac = pCtrl->stSTAT.pre_stdev_frac ;
	
	AutoSat_active_level = pCtrl->stSAT.AutoSat_active_level ;
	AutoSat_gain_level = pCtrl->stSAT.AutoSat_gain_level ;
	
	if (pCtrl->stSTAT.GainCTLMode == AECTRL_GAINCTL_OFF ) 	
	{
		AutoSat_compensation = 0  ;
	}
	else
	{
		if ( (pCtrl->stSTAT.is_gain_max== 1) || (pCtrl->stSTAT.dgain>256) )
		{ // gain max
			if (stdev_frac >= 4) stdev_int++ ;
			if (pre_stdev_frac >= 4) pre_stdev_int++ ;	
			
			if ((pre_stdev_int >= stdev_int)) 
			{ // when scene is not under exposed
				if( pre_stdev_int > AutoSat_active_level)	AutoSat_compensation = pre_stdev_int - AutoSat_active_level;
				else									AutoSat_compensation = 0;
			}
			else
			{ // when scene is under exposed
				AutoSat_compensation = pCtrl->stSAT.AutoSat_compensation ;
			}
		}
		else if ( (pCtrl->stSTAT.gain) > AutoSat_gain_level ) 
		{
			if( stdev_int > AutoSat_active_level)	AutoSat_compensation = stdev_int - AutoSat_active_level;
			else									AutoSat_compensation = 0;
		}
		else 
		{
			AutoSat_compensation = 0;
		}
	}	
	
	pCtrl->stSAT.AutoSat_compensation = AutoSat_compensation ;
	
	//Set compensated saturation 
	//Auto saturation value cannot go under __LL_AUTOSAT_MINIMUM__ unless
	//AutoSat_normal_value is set to under __LL_AUTOSAT_MINIMUM__
	if ( (pCtrl->stSAT.AutoSat_normal_value) > __LL_AUTOSAT_MINIMUM__ )
	{
		if( (pCtrl->stSAT.AutoSat_normal_value) > (AutoSat_compensation*__LL_AUTOSAT_COMP_TICK__))
		{
			AutoSat_comp_value = (pCtrl->stSAT.AutoSat_normal_value) - (AutoSat_compensation*__LL_AUTOSAT_COMP_TICK__) ;			
		}
		else
		{
			AutoSat_comp_value = __LL_AUTOSAT_MINIMUM__ ;			
		}
		
		if ( AutoSat_comp_value >= __LL_AUTOSAT_MINIMUM__ )
		{
			pCtrl->stSAT.AutoSat_comp_value = AutoSat_comp_value ;
		}
		else
		{
			pCtrl->stSAT.AutoSat_comp_value = __LL_AUTOSAT_MINIMUM__ ;
		}
	}
	else
	{		
		pCtrl->stSAT.AutoSat_comp_value = pCtrl->stSAT.AutoSat_normal_value ;
	}	
}

// ------------------------------------------
static void LLCTRL_AutoSatTrace(PMDIN_LL_CONTROL_PARAM pCtrl)
{
	BYTE AutoSat_comp_value ;
	BYTE AutoSat_trace_value ;
	BYTE diff_trace_value ;
	
	AutoSat_comp_value = pCtrl->stSAT.AutoSat_comp_value ;
	AutoSat_trace_value = pCtrl->stSAT.AutoSat_trace_value ;
	diff_trace_value = DIFF(AutoSat_comp_value,AutoSat_trace_value) ;
	
	if(diff_trace_value <= (pCtrl->stSAT.AutoSat_trace_speed))
	{
		AutoSat_trace_value = AutoSat_comp_value ;
		pCtrl->stSAT.AutoSat_trace_speed = __LL_AUTOSAT_INIT_TRACE_SPEED__ ; // trace speed returns to initial when reach to the goal
	}
	else if ( AutoSat_trace_value < AutoSat_comp_value )
	{		
		AutoSat_trace_value = AutoSat_trace_value + (pCtrl->stSAT.AutoSat_trace_speed) ;
	}
	else if ( AutoSat_trace_value > AutoSat_comp_value )
	{
		AutoSat_trace_value = AutoSat_trace_value - (pCtrl->stSAT.AutoSat_trace_speed) ;
	}
	
	if( AutoSat_trace_value != (pCtrl->stSAT.AutoSat_trace_value))
	{
		SetOUT_MainSaturation(AutoSat_trace_value);	
		SetOUT_AuxSaturation(AutoSat_trace_value);  
// #if defined(__MDIN_i540_4K60__) && defined(__USE_4K60_MD1__)	// 4k60 master only
#if __LLC_SPLIT__==1 // 4k60 master only
		SPImAPP_SetSlaveLLSaturation(AutoSat_trace_value);
#endif
		pCtrl->stSAT.AutoSat_trace_value=AutoSat_trace_value;	
	}
}
// ------------------------------------------
// ------------------------------------------
static void LLCTRL_CSPSetValue(PMDIN_LL_CONTROL_PARAM pCtrl)
{
	WORD stdev_int ;
	WORD stdev_frac ;
	WORD pre_stdev_int ;
	WORD pre_stdev_frac ;
	
	LONG CSP_gain_level ;

	BYTE x0_target ;
	BYTE x1_target ;
	
	if ( pCtrl->stCSP.AutoCSP_intensity == 0 ) 
	{
		pCtrl->stCSP.x0_target = 0 ;
		pCtrl->stCSP.x1_target = 4 ;
		return ; // off 
	}	

	stdev_int = pCtrl->stSTAT.stdev_int ;
	stdev_frac = pCtrl->stSTAT.stdev_frac ;
	
	pre_stdev_int = pCtrl->stSTAT.pre_stdev_int ;
	pre_stdev_frac = pCtrl->stSTAT.pre_stdev_frac ;
	CSP_gain_level =  pCtrl->stCSP.AutoCSP_gain_level;
	
	if (pCtrl->stSTAT.GainCTLMode == AECTRL_GAINCTL_OFF ) 	
	{
		x0_target = 0 ;		
		x1_target = 4 ;		
	}
	else
	{
		if (stdev_frac >= 4) stdev_int++ ;
		if (pre_stdev_frac >= 4) pre_stdev_int++ ;			
		
		if ( (pCtrl->stSTAT.is_gain_max== 1) || (pCtrl->stSTAT.dgain>256) )
		{ // gain max 
			if ((pre_stdev_int >= stdev_int))
			{ // When scene is not under exposed
				LLCTRL_CSPCalcMaxTarget(&x0_target,&x1_target,pre_stdev_int);
			}
			else
			{ // When scene is under exposed
				x0_target = pCtrl->stCSP.x0_target ;		
				x1_target = pCtrl->stCSP.x1_target ;					
			}
		}
		else if ( (pCtrl->stSTAT.gain) > CSP_gain_level ) 
		{	
			LLCTRL_CSPCalcMidTarget(&x0_target,&x1_target,stdev_int);
		}
		else
		{
			x0_target = 0 ;		
			x1_target = stdev_int ;				
		}		
	}
	
	pCtrl->stCSP.x0_target = x0_target ;
	pCtrl->stCSP.x1_target = x1_target ;	
}
// ------------------------------------------
static void LLCTRL_CSPTrace(PMDIN_LL_CONTROL_PARAM pCtrl)
{
	BYTE csp_ls_x0 ;
	BYTE csp_ls_x1 ;

	BYTE x0_target ;
	BYTE x1_target ;
	
	x0_target = pCtrl->stCSP.x0_target ;
	x1_target = pCtrl->stCSP.x1_target ;	

	csp_ls_x0 = pCtrl->stCSP.csp_ls_x0 ;
	csp_ls_x1 = pCtrl->stCSP.csp_ls_x1 ;		

	if ( csp_ls_x0 < x0_target )			csp_ls_x0++ ;
	else if ( csp_ls_x0 > x0_target )		csp_ls_x0-- ;	
	
	if ( csp_ls_x1 < x1_target )			csp_ls_x1++ ;
	else if ( csp_ls_x1 > x1_target )		csp_ls_x1-- ;	

	if ( (csp_ls_x0 != pCtrl->stCSP.csp_ls_x0 ) || (csp_ls_x1 != pCtrl->stCSP.csp_ls_x1) )
	{
		SetCSP_LS_P0(csp_ls_x0);
		SetCSP_LS_P1(csp_ls_x1);
		pCtrl->stCSP.csp_ls_x0 = csp_ls_x0 ;
		pCtrl->stCSP.csp_ls_x1 = csp_ls_x1 ;
// #if defined(__MDIN_i540_4K60__) && defined(__USE_4K60_MD1__)	// 4k60 master only
#if __LLC_SPLIT__==1 // 4k60 master only
		SPImAPP_SetSlaveCSPLS(csp_ls_x0, csp_ls_x1);
#endif
	}		
}
// ------------------------------------------
static void LLCTRL_AutoEdge(PMDIN_LL_CONTROL_PARAM pCtrl)
{
	WORD stdev_int ;
	WORD stdev_frac ;
	WORD pre_stdev_int ;
	WORD pre_stdev_frac ;
	
	LONG EDGE_gain_level ;	

	BYTE edge_compensation ;
	BYTE AutoEDGE_comp_value ;
	
	if (pCtrl->stEE.AutoEDGE_intensity == 0)
	{
		pCtrl->stEE.AutoEDGE_comp_value = pCtrl->stEE.AutoEDGE_normal_value ;
		return ;
	}
	
	stdev_int = pCtrl->stSTAT.stdev_int ;
	stdev_frac = pCtrl->stSTAT.stdev_frac ;
	
	pre_stdev_int = pCtrl->stSTAT.pre_stdev_int ;
	pre_stdev_frac = pCtrl->stSTAT.pre_stdev_frac ;
	EDGE_gain_level = pCtrl->stEE.AutoEDGE_gain_level ;
	
	if (pCtrl->stSTAT.GainCTLMode == AECTRL_GAINCTL_OFF) 
	{
		edge_compensation = 0  ;
	}
	else
	{
		if ( (pCtrl->stSTAT.is_gain_max== 1) || (pCtrl->stSTAT.dgain>256) )
		{
			LLCTRL_AutoEECalcMaxTarget(&edge_compensation,stdev_int);
		}
		else if ( (pCtrl->stSTAT.gain) > EDGE_gain_level ) 
		{	
			LLCTRL_AutoEECalcMidTarget(&edge_compensation,stdev_int);
		}				
		else 
		{
			edge_compensation = 0;	
		}			
	}		
	
	if( (pCtrl->stEE.AutoEDGE_normal_value) > (edge_compensation*__LL_AUTOEDGE_COMP_TICK__))
	{
		AutoEDGE_comp_value = (pCtrl->stEE.AutoEDGE_normal_value) - (edge_compensation*__LL_AUTOEDGE_COMP_TICK__) ;			
	}
	else
	{
		AutoEDGE_comp_value = 0 ;			
	}
	
	pCtrl->stEE.AutoEDGE_comp_value = AutoEDGE_comp_value ;
}
// ------------------------------------------
static void LLCTRL_AutoEETrace(PMDIN_LL_CONTROL_PARAM pCtrl)
{
	BYTE trace_value ;
	BYTE comp_value ;		
	BYTE diff_trace_value ;		
	
	trace_value = pCtrl->stEE.AutoEDGE_trace_value ;
	comp_value = pCtrl->stEE.AutoEDGE_comp_value ;
	diff_trace_value = DIFF(comp_value,trace_value) ;
	
	if(diff_trace_value <= (pCtrl->stEE.AutoEDGE_trace_speed))
	{
		trace_value = comp_value ;
		pCtrl->stEE.AutoEDGE_trace_speed = 1 ; // trace speed returns 1 when reach to the goal
	}
	else if ( trace_value < comp_value )
	{		
		trace_value = trace_value + (pCtrl->stEE.AutoEDGE_trace_speed) ;
	}
	else if ( trace_value > comp_value )
	{
		trace_value = trace_value - (pCtrl->stEE.AutoEDGE_trace_speed) ;
	}	
	
	if( trace_value != (pCtrl->stEE.AutoEDGE_trace_value) )
	{
		// DEMO_SetSharpness(target_edge);	
	
		SetIN_MainHPFLevel(trace_value); // Avoid DEMO_SetSharpness() to preserve EEPROM value
		SetIN_MainHPFLDslope(trace_value) ; // Avoid DEMO_SetSharpness() to preserve EEPROM value		
		
		#if defined(__USE_MDIN_i5XX_REV__)
		SetFC_MainVPFLevel(trace_value); // Avoid DEMO_SetSharpness() to preserve EEPROM value
		SetFC_MainVPFLDslope(trace_value) ; // Avoid DEMO_SetSharpness() to preserve EEPROM value		
		#endif
// #if defined(__MDIN_i540_4K60__) && defined(__USE_4K60_MD1__)	// 4k60 master only
#if __LLC_SPLIT__==1 // 4k60 master only
		SPImAPP_SetSlaveLLSharpness(trace_value);
#endif
		pCtrl->stEE.AutoEDGE_trace_value = trace_value;					
	}	
}
// ------------------------------------------
static void LLCTRL_AutoCAC(PMDIN_LL_CONTROL_PARAM pCtrl)
{
	WORD stdev_int ;
	WORD stdev_frac ;
	WORD pre_stdev_int ;
	WORD pre_stdev_frac ;

	WORD CAC_compensation ;
	WORD AutoCAC_g_comp_value ;
	WORD AutoCAC_m_comp_value ;
	
	WORD Autocac_g_comp_tick ;
	WORD Autocac_m_comp_tick ;
	
	if (pCtrl->stCAC.AutoCAC_en == 0)
	{
		pCtrl->stCAC.AutoCAC_g_comp_value = pCtrl->stCAC.AutoCAC_g_normal ;
		pCtrl->stCAC.AutoCAC_m_comp_value = pCtrl->stCAC.AutoCAC_m_normal ;
		return ;
	}
	
	stdev_int = pCtrl->stSTAT.stdev_int ;
	stdev_frac = pCtrl->stSTAT.stdev_frac ;
	
	pre_stdev_int = pCtrl->stSTAT.pre_stdev_int ;
	pre_stdev_frac = pCtrl->stSTAT.pre_stdev_frac ;	
	
	if (pCtrl->stSTAT.GainCTLMode == AECTRL_GAINCTL_OFF) 
	{
		CAC_compensation = 0  ;
		Autocac_g_comp_tick = 0  ;
		Autocac_m_comp_tick = 0  ;
	}
	else
	{
		if ( (pCtrl->stSTAT.is_gain_max== 1) || (pCtrl->stSTAT.dgain>256) )
		{
			if (pre_stdev_int >= stdev_int)
			{
				if ( pre_stdev_int > __LL_AUTOCAC_NPOINT_0__ )
					CAC_compensation = pre_stdev_int - (__LL_AUTOCAC_NPOINT_0__>>1) ;
				else
					CAC_compensation = 0 ;
				
				if ( pre_stdev_int > __LL_AUTOCAC_NPOINT_1__ )
				{
					Autocac_g_comp_tick = __LL_AUTOCAC_G_COMP_TICK_1__  ;
					Autocac_m_comp_tick = __LL_AUTOCAC_M_COMP_TICK_1__  ;
				}
				else if ( pre_stdev_int > __LL_AUTOCAC_NPOINT_0__ )
				{
					Autocac_g_comp_tick = __LL_AUTOCAC_G_COMP_TICK_0__  ;
					Autocac_m_comp_tick = __LL_AUTOCAC_M_COMP_TICK_0__  ;					
				}
				else
				{
					Autocac_g_comp_tick = 0  ;
					Autocac_m_comp_tick = 0  ;						
				}
			}
			else
			{
				CAC_compensation = pCtrl->stCAC.CAC_compensation ;
				Autocac_g_comp_tick = pCtrl->stCAC.AutoCAC_g_comp_tick  ;
				Autocac_m_comp_tick = pCtrl->stCAC.AutoCAC_m_comp_tick  ;					
			}
		}
		else
		{
			if ( stdev_int > __LL_AUTOCAC_NPOINT_1__ ) 
			{  // CAC active before A-gain MAX : high noise state
				CAC_compensation = stdev_int - (__LL_AUTOCAC_NPOINT_0__>>1) ;
				Autocac_g_comp_tick = __LL_AUTOCAC_G_COMP_TICK_1__  ;
				Autocac_m_comp_tick = __LL_AUTOCAC_M_COMP_TICK_1__  ;
			}
			else
			{
				CAC_compensation = 0  ;
				Autocac_g_comp_tick = 0  ;
				Autocac_m_comp_tick = 0  ;					
			}			
		}
	}
		
	pCtrl->stCAC.CAC_compensation = CAC_compensation ;
	pCtrl->stCAC.AutoCAC_g_comp_tick = Autocac_g_comp_tick ;
	pCtrl->stCAC.AutoCAC_m_comp_tick = Autocac_m_comp_tick ;
	
	if( (pCtrl->stCAC.AutoCAC_g_normal) > (CAC_compensation*Autocac_g_comp_tick))
	{
		AutoCAC_g_comp_value = (pCtrl->stCAC.AutoCAC_g_normal) - (CAC_compensation*Autocac_g_comp_tick) ;			
	}
	else
	{
		AutoCAC_g_comp_value = __LL_AUTOCAC_MINIMUM__ ;
	}
	
	if ( AutoCAC_g_comp_value >= __LL_AUTOCAC_MINIMUM__ )
	{
		pCtrl->stCAC.AutoCAC_g_comp_value = AutoCAC_g_comp_value ;
	}
	else
	{
		pCtrl->stCAC.AutoCAC_g_comp_value = __LL_AUTOCAC_MINIMUM__ ;
	}	
	
	
	if( (pCtrl->stCAC.AutoCAC_m_normal) > (CAC_compensation*Autocac_m_comp_tick))
	{
		AutoCAC_m_comp_value = (pCtrl->stCAC.AutoCAC_m_normal) - (CAC_compensation*Autocac_m_comp_tick) ;			
	}
	else
	{
		AutoCAC_m_comp_value = __LL_AUTOCAC_MINIMUM__ ;
	}
	
	if ( AutoCAC_m_comp_value >= __LL_AUTOCAC_MINIMUM__ )
	{
		pCtrl->stCAC.AutoCAC_m_comp_value = AutoCAC_m_comp_value ;
	}
	else
	{
		pCtrl->stCAC.AutoCAC_m_comp_value = __LL_AUTOCAC_MINIMUM__ ;
	}		
}
// ------------------------------------------
static void LLCTRL_AutoCACTrace(PMDIN_LL_CONTROL_PARAM pCtrl)
{
	WORD AutoCAC_g_comp_value ;
	WORD AutoCAC_g_trace_value ;
	WORD diff_g_trace_value ;

	WORD AutoCAC_m_comp_value ;
	WORD AutoCAC_m_trace_value ;
	WORD diff_m_trace_value ;	
	
	AutoCAC_g_comp_value = pCtrl->stCAC.AutoCAC_g_comp_value ;
	AutoCAC_g_trace_value = pCtrl->stCAC.AutoCAC_g_trace_value ;
	diff_g_trace_value = DIFF(AutoCAC_g_comp_value,AutoCAC_g_trace_value) ;	

	AutoCAC_m_comp_value = pCtrl->stCAC.AutoCAC_m_comp_value ;
	AutoCAC_m_trace_value = pCtrl->stCAC.AutoCAC_m_trace_value ;
	diff_m_trace_value = DIFF(AutoCAC_m_comp_value,AutoCAC_m_trace_value) ;	

	if(diff_g_trace_value <= (pCtrl->stCAC.AutoCAC_g_trace_speed))
	{
		AutoCAC_g_trace_value = AutoCAC_g_comp_value ;
	}
	else if ( AutoCAC_g_trace_value < AutoCAC_g_comp_value )
	{		
		AutoCAC_g_trace_value = AutoCAC_g_trace_value + (pCtrl->stCAC.AutoCAC_g_trace_speed) ;
	}
	else if ( AutoCAC_g_trace_value > AutoCAC_g_comp_value )
	{
		AutoCAC_g_trace_value = AutoCAC_g_trace_value - (pCtrl->stCAC.AutoCAC_g_trace_speed) ;
	}
	
	if( AutoCAC_g_trace_value != (pCtrl->stCAC.AutoCAC_g_trace_value))
	{
		Set_CAC_Green_gain(AutoCAC_g_trace_value);
		pCtrl->stCAC.AutoCAC_g_trace_value = AutoCAC_g_trace_value ;
		
// #if defined(__MDIN_i540_4K60__) && defined(__USE_4K60_MD1__)	// 4k60 master only
#if __LLC_SPLIT__==1 // 4k60 master only
		SPImAPP_SetSlaveCACGgain(AutoCAC_g_trace_value); 
#endif			
	}
		
	if(diff_m_trace_value <= (pCtrl->stCAC.AutoCAC_m_trace_speed))
	{
		AutoCAC_m_trace_value = AutoCAC_m_comp_value ;
	}
	else if ( AutoCAC_m_trace_value < AutoCAC_m_comp_value )
	{		
		AutoCAC_m_trace_value = AutoCAC_m_trace_value + (pCtrl->stCAC.AutoCAC_m_trace_speed) ;
	}
	else if ( AutoCAC_m_trace_value > AutoCAC_m_comp_value )
	{
		AutoCAC_m_trace_value = AutoCAC_m_trace_value - (pCtrl->stCAC.AutoCAC_m_trace_speed) ;
	}
	
	if( AutoCAC_m_trace_value != (pCtrl->stCAC.AutoCAC_m_trace_value))
	{
		Set_CAC_Magenta_gain(AutoCAC_m_trace_value);
		pCtrl->stCAC.AutoCAC_m_trace_value = AutoCAC_m_trace_value ;
		
// #if defined(__MDIN_i540_4K60__) && defined(__USE_4K60_MD1__)	// 4k60 master only
#if __LLC_SPLIT__==1 // 4k60 master only
		SPImAPP_SetSlaveCACMgain(AutoCAC_m_trace_value);		
#endif			
	}	
}
// ------------------------------------------
// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------
void SetApp_initLLcontrol(void)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
	
	EEPROM_MultiRead(EEPROM_EFFECT_AUTOSAT, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] != EEPROM_SAVE_DONE)
	{ // If AutoSat function initial value is not in EEPROM
		SetApp_AutoSat_Intensity(__LL_AUTOSAT_INIT_INTENSITY__); // 0:off ~ Value for menu matching
	}	
#else
	// No USE_EEPROM
	SetApp_AutoSat_Intensity(__LL_AUTOSAT_INIT_INTENSITY__); // 0:off ~ Value for menu matching
#endif	
	
	stLLControl.stSAT.AutoSat_normal_value = GetOUT_MainSaturation() ;
	stLLControl.stSAT.AutoSat_trace_value = GetOUT_MainSaturation() ;
	stLLControl.stSAT.AutoSat_comp_value = GetOUT_MainSaturation() ;
	stLLControl.stSAT.AutoSat_trace_speed = __LL_AUTOSAT_INIT_TRACE_SPEED__ ;
	stLLControl.stSAT.is_initialized = 1 ;
	
	SetApp_AutoCSP_Intensity(__LL_AUTOCSP_INIT_INTENSITY__) ;

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	EEPROM_MultiRead(EEPROM_EFFECT_AUTOEDGE, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] != EEPROM_SAVE_DONE)
	{ // If AutoEE function initial value is not in EEPROM
		SetApp_AutoEDGE_Intensity(__LL_AUTOEDGE_INIT_INTENSITY__); // 0:off ~ Value for menu matching
	}
#else	
	SetApp_AutoEDGE_Intensity(__LL_AUTOEDGE_INIT_INTENSITY__) ; // 0:off ~ Value for menu matching
#endif
	stLLControl.stEE.AutoEDGE_gain_level = __LL_AUTOEDGE_GAIN_LEVEL__ ; // 
	stLLControl.stEE.AutoEDGE_trace_speed = __LL_AUTOEDGE_TRACE_SPEED__ ; // 
	stLLControl.stEE.is_initialized = 1 ;
	
	stLLControl.stCAC.AutoCAC_en = __LL_AUTOCAC_EN__ ;
	stLLControl.stCAC.AutoCAC_g_trace_speed = __LL_AUTOCAC_TRACE_SPEED__ ;
	stLLControl.stCAC.AutoCAC_m_trace_speed = __LL_AUTOCAC_TRACE_SPEED__ ;

	SetApp_AutoCAC_GreenUpdate();
	SetApp_AutoCAC_MagentaUpdate();

	stLLControl.stCAC.AutoCAC_g_trace_value = stLLControl.stCAC.AutoCAC_g_normal; 
	stLLControl.stCAC.AutoCAC_m_trace_value = stLLControl.stCAC.AutoCAC_m_normal;
	stLLControl.stCAC.AutoCAC_g_comp_value = stLLControl.stCAC.AutoCAC_g_normal; 
	stLLControl.stCAC.AutoCAC_m_comp_value = stLLControl.stCAC.AutoCAC_m_normal;	
}
//--------------------------------------------------------------------------------------------------------------------------
void LLApp_ProcessHandler(void)
{
#if (__LLC_SPLIT__<=1)	// LLApp_ProcessHandler works only normal or master mode
	LLCTRL_StatusUpdate(&stLLControl) ;
	
	LLCTRL_AutoSatTrace(&stLLControl);
	LLCTRL_CSPTrace(&stLLControl);
	LLCTRL_AutoEETrace(&stLLControl);
	
	LLCTRL_AutoCACTrace(&stLLControl);
	
	if ((stLLControl.ctrl&LLCTRL_TRIGGER_ISP)==0) return;
	stLLControl.ctrl &= (~LLCTRL_TRIGGER_ISP);	
	
	LLCTRL_AutoSatSetCompValue(&stLLControl);
	LLCTRL_CSPSetValue(&stLLControl);
	LLCTRL_AutoEdge(&stLLControl);
	
	LLCTRL_AutoCAC(&stLLControl);
#endif
}
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_AutoSat_Intensity(BYTE intensity) 
{	// Value for menu matching
	stLLControl.stSAT.AutoSat_intensity = intensity ;	
	
	switch (intensity)
	{
		case 1 : stLLControl.stSAT.AutoSat_active_level = __LL_AUTOSAT_1__ ; break ; // intensity low
		case 2 : stLLControl.stSAT.AutoSat_active_level = __LL_AUTOSAT_2__ ; break ; // intensity mid
		case 3 : stLLControl.stSAT.AutoSat_active_level = __LL_AUTOSAT_3__ ; break ; // intensity high		
		default : break ;		
	}
	
	#if !(__LL_USE_AUTOSAT_DEFINED_GAIN_LEVEL_)
	// Auto adjust refer to max_gain setting
	switch (intensity)
	{		
		case 1 : stLLControl.stSAT.AutoSat_gain_level = (stLLControl.stSTAT.max_gain)/4 ; break ; // intensity low
		case 2 : stLLControl.stSAT.AutoSat_gain_level = (stLLControl.stSTAT.max_gain)/8 ; break ; // intensity mid
		case 3 : stLLControl.stSAT.AutoSat_gain_level = (stLLControl.stSTAT.max_gain)/16 ; break ; // intensity high		
		default : break ;		
	}
	#else
	// For direct defined level
	switch (intensity)
	{		
		case 1 : stLLControl.stSAT.AutoSat_gain_level = __LL_AUTOSAT_GAIN_LEVEL_1__ ; break ; // intensity low
		case 2 : stLLControl.stSAT.AutoSat_gain_level = __LL_AUTOSAT_GAIN_LEVEL_2__ ; break ; // intensity mid
		case 3 : stLLControl.stSAT.AutoSat_gain_level = __LL_AUTOSAT_GAIN_LEVEL_3__ ; break ; // intensity high		
		default : break ;		
	}			
	#endif // #if !(__LL_USE_AUTOSAT_DEFINED_GAIN_LEVEL_)
}

//--------------------------------------------------------------------------------------------------------------------------
void SetApp_AutoSat_TraceSpeed(BYTE trace_speed)
{ // 
	if ( trace_speed > 11 ) trace_speed = 12 ; // limit max speed 
	stLLControl.stSAT.AutoSat_trace_speed = trace_speed ;	
}

//--------------------------------------------------------------------------------------------------------------------------
void SetApp_AutoSat_OutSaturation(BYTE val, BYTE trace_speed)
{ // This function will replace SetOUT_MainSaturation()&SetOUT_AuxSaturation()
	stLLControl.stSAT.AutoSat_normal_value = val ;
	SetApp_AutoSat_TraceSpeed(trace_speed);
	
	stLLControl.ctrl |= (LLCTRL_TRIGGER_ISP); // imadiatly update the AutoSat_comp_value
}
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_AutoSat_ValueUpdate(void) 
{ // Call this function after direct call of SetOUT_MainSaturation() for proper operation
	stLLControl.stSAT.AutoSat_normal_value = GetOUT_MainSaturation() ;
	stLLControl.stSAT.AutoSat_trace_value = GetOUT_MainSaturation() ;
	stLLControl.stSAT.AutoSat_comp_value = GetOUT_MainSaturation() ;
}
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_AutoCSP_Intensity(BYTE intensity) 
{	// Value for menu matching
	stLLControl.stCSP.AutoCSP_intensity = intensity ;	
	
	#if !(__LL_USE_AUTOCSP_DEFINED_GAIN_LEVEL_)
	// Auto adjust refer to max_gain setting
	switch (intensity)
	{		
		case 1 : stLLControl.stCSP.AutoCSP_gain_level = (stLLControl.stSTAT.max_gain)/4 ; break ; // intensity low
		case 2 : stLLControl.stCSP.AutoCSP_gain_level = (stLLControl.stSTAT.max_gain)/8 ; break ; // intensity mid
		case 3 : stLLControl.stCSP.AutoCSP_gain_level = (stLLControl.stSTAT.max_gain)/16 ; break ; // intensity high		
		default : break ;		
	}
	#else
	// For direct defined level
	switch (intensity)
	{		
		case 1 : stLLControl.stCSP.AutoCSP_gain_level = __LL_AUTOCSP_GAIN_LEVEL_1__ ; break ; // intensity low
		case 2 : stLLControl.stCSP.AutoCSP_gain_level = __LL_AUTOCSP_GAIN_LEVEL_2__ ; break ; // intensity mid
		case 3 : stLLControl.stCSP.AutoCSP_gain_level = __LL_AUTOCSP_GAIN_LEVEL_3__ ; break ; // intensity high		
		default : break ;		
	}			
	#endif // #if !(__LL_USE_AUTOSAT_DEFINED_GAIN_LEVEL_)
}
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_AutoEDGE_TraceSpeed(BYTE trace_speed)
{ // 
	if ( trace_speed > 11 ) trace_speed = 12 ; // limit max speed 
	stLLControl.stEE.AutoEDGE_trace_speed = trace_speed ;	
}
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_AutoEDGE_Intensity(BYTE intensity) 
{	// Value for menu matching
	stLLControl.stEE.AutoEDGE_intensity = intensity ;	
}
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_AutoEDGE_HVPFLevel(BYTE val)
{ // This function will replace 
	stLLControl.stEE.AutoEDGE_normal_value = val ;
	SetApp_AutoEDGE_TraceSpeed(__LL_AUTOEDGE_TRACE_SPEED__);
	
	stLLControl.ctrl |= (LLCTRL_TRIGGER_ISP); // imadiatly update the AutoSat_comp_value
}
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_AutoCAC_GreenUpdate(void)
{// Call this function after direct call of Set_CAC_Green_gain() for proper operation
	
	stLLControl.stCAC.AutoCAC_g_normal = Get_CAC_Green_gain() ;
}
//--------------------------------------------------------------------------------------------------------------------------
void SetApp_AutoCAC_MagentaUpdate(void)
{// Call this function after direct call of Set_CAC_Magenta_gain() for proper operation
	stLLControl.stCAC.AutoCAC_m_normal = Get_CAC_Magenta_gain() ;
}
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
BYTE GetApp_AutoSat_Intensity(void)
{ // For menu matching
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
	
	if (stLLControl.stSAT.is_initialized == 0) // When Auto sat is not initialized
	{
		EEPROM_MultiRead(EEPROM_EFFECT_AUTOSAT, (PBYTE)&buff[0], sizeof(buff));
		if(buff[0] == EEPROM_SAVE_DONE)
		{ // If AutoSat function initial value is not in EEPROM
			SetApp_AutoSat_Intensity(buff[1]); // 0:off ~ Value for menu matching
		}
		else
		{
			SetApp_AutoSat_Intensity(__LL_AUTOSAT_INIT_INTENSITY__); // 
		}
	}	
#else
	if (stLLControl.stSAT.is_initialized == 0) // When Auto sat is not initialized
	{
		SetApp_AutoSat_Intensity(__LL_AUTOSAT_INIT_INTENSITY__);
	}	
#endif

	return stLLControl.stSAT.AutoSat_intensity ;
	
}


//--------------------------------------------------------------------------------------------------------------------------
BYTE GetApp_AutoEDGE_Intensity(void)
{ // For menu matching
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
	
	if (stLLControl.stEE.is_initialized == 0) // When Auto EE is not initialized
	{
		EEPROM_MultiRead(EEPROM_EFFECT_AUTOEDGE, (PBYTE)&buff[0], sizeof(buff));
		if(buff[0] == EEPROM_SAVE_DONE)
		{ // If AutoSat function initial value is not in EEPROM
			SetApp_AutoEDGE_Intensity(buff[1]); // 0:off ~ Value for menu matching
		}
		else
		{
			SetApp_AutoEDGE_Intensity(__LL_AUTOEDGE_INIT_INTENSITY__); // 
		}
	}	
#else
	if (stLLControl.stEE.is_initialized == 0) // When Auto EE is not initialized
	{
		SetApp_AutoEDGE_Intensity(__LL_AUTOEDGE_INIT_INTENSITY__);
	}	
#endif	

	return stLLControl.stEE.AutoEDGE_intensity ;
}

BYTE GetApp_AutoEDGE_HVPFLevel(void)
{ // For menu matching
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[2];
	
	if (stLLControl.stEE.is_initialized == 0) // When Auto EE is not initialized
	{
		EEPROM_MultiRead(EEPROM_EFFECT_EDGEENH, (PBYTE)&buff[0], sizeof(buff));
		if(buff[0] == EEPROM_SAVE_DONE)
		{ // If AutoSat function initial value is not in EEPROM
			SetApp_AutoEDGE_HVPFLevel((buff[1]*__LL_AUTOEDGE_MENU_TICK__)); // 
		}
		else
		{
			SetApp_AutoEDGE_HVPFLevel(__LL_AUTOEDGE_INIT_HVPFLEVEL__); // 
		}
	}	
#else
	if (stLLControl.stEE.is_initialized == 0) // When Auto EE is not initialized
	{
		SetApp_AutoEDGE_HVPFLevel(__LL_AUTOEDGE_INIT_HVPFLEVEL__);
	}	
#endif	

	return stLLControl.stEE.AutoEDGE_normal_value ;	
}
//--------------------------------------------------------------------------------------------------------------------------
#endif // #if defined(__USE_MDIN_i510__)||defined(__USE_MDIN_i540__)||defined(__USE_MDIN_i550__)
//--------------------------------------------------------------------------------------------------------------------------

/*  FILE_END_HERE */


