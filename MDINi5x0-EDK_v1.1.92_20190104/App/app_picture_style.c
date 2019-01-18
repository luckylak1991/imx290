// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"
#include	"osd.h"
// #include	"app_picture_style.h"

#if defined(__USE_MDIN_i510__)||defined(__USE_MDIN_i540__)||defined(__USE_MDIN_i550__)
// ----------------------------------------------------------------------
//
// 	Picture style is APi base preset value of i5x0 series.
//  combination of contrast, saturation, HUE , Edge enhancement
//  Both main and aux Y/C gamma adjust for minor setting
//
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static MDIN_PS_SET_PARAM stPS ;

static ROMDATA BYTE PS_offset[6][4] = 
{	// Con , Sat , Hue , EE
	{11,10,10,5}, // SetApp_PS_OFF
	{11,10,10,0}, // SetApp_PS_STANDARD
	{11,15,10,0}, // SetApp_PS_VIVID
	{12,10,10,1}, // SetApp_PS_CLEAR
	{12,13,10,1}, // SetApp_PS_VIVID_CLEAR
	{10,8,10,0}, // SetApp_PS_NEUTRAL
};

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------
// extern BYTE target_bright ; // From Demo.c FOR reference
// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
static void SetApp_PS_SETUP(PMDIN_PS_PARAM pCtrl) ;
static PMDIN_PS_PARAM SetApp_PS_SELECT(WORD style) ;
static void SetApp_PS_SetHue(WORD nID); // Same function with DEMO_SetHue(WORD nID). NO EEPROM save.

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------
static void SetApp_PS_SETUP(PMDIN_PS_PARAM pCtrl)
{
	BYTE contrast_value ;
	BYTE brightness_level ;
	BYTE saturation_value ;
	BYTE hue_value ;
	BYTE edge_value ;
	
	WORD prestyle ;
	WORD style ;
	
	style = stPS.ss_PS ;
	prestyle = stPS.ss_prePS ;

	if ( style != MDIN_PS_OFF ) 
	{
		if ( GetIN_MainPath() == MISP_MAINPATH_SENSOR || GetIN_AuxPath() == MISP_AUXPATH_SENSOR )
		{
			contrast_value = PS_offset[style][0] ;			
			saturation_value = PS_offset[style][1] ;
			hue_value = PS_offset[style][2] ;
			edge_value = PS_offset[style][3] + pCtrl->EE_level*2 ;
		}
		else
		{
			contrast_value = PS_offset[0][0] ;
			saturation_value = PS_offset[0][1] ;
			hue_value = PS_offset[0][2] ;
			edge_value = PS_offset[0][3] ;		
		}
		
		brightness_level = 11 ;
		SetDM_ColorCC_Brightness(0); // Avoid DEMO_SetBrightness() to preserve EEPROM value
		
		if ( (stPS.ctrl&PSCTRL_EE_UPDATE) !=0 )
		{
			//SetIN_MainHPFLevel(edge_value*4); // Avoid DEMO_SetSharpness() to preserve EEPROM value
			SetIN_MainHPFLevel(edge_value*6); // Avoid DEMO_SetSharpness() to preserve EEPROM value
			SetIN_MainHPFLDslope(edge_value*6) ; // Avoid DEMO_SetSharpness() to preserve EEPROM value		

			SetFC_MainVPFLevel(edge_value*6); // Avoid DEMO_SetSharpness() to preserve EEPROM value
			SetFC_MainVPFLDslope(edge_value*6) ; // Avoid DEMO_SetSharpness() to preserve EEPROM value
			
			// UARTprintf("PSCTRL_EE_UPDATE\r\n");
		}
		
		if ( GetIN_MainPath() == MISP_MAINPATH_SENSOR ) 
		{
			if ( (stPS.ctrl&PSCTRL_CON_UPDATE) !=0 ) 
			{
				SetTONE_YCGAMMA_LUT(1,pCtrl->Contrast_level) ; // Y
					
				// UARTprintf("PSCTRL_CON_UPDATE : M\r\n");
			}
			
			if ( (stPS.ctrl&PSCTRL_SAT_UPDATE) !=0 )
			{
				SetTONE_YCGAMMA_LUT(2,pCtrl->Saturation_level) ; // Cb
				SetTONE_YCGAMMA_LUT(3,pCtrl->Saturation_level) ; // Cr
				
				// UARTprintf("PSCTRL_SAT_UPDATE : M\r\n");
			}
		}
		else
		{
			SetTONE_YCGAMMA_LUT(0,3) ; // YCbCr	: table no.3 is straight line
		}
		
		if ( GetTONE_AYCGAMMA_thermo() == OFF ) // 
		{
			if ( GetIN_AuxPath() == MISP_AUXPATH_SENSOR ) 
			{
				if ( (stPS.ctrl&PSCTRL_CON_UPDATE) !=0 ) 
				{
					SetTONE_AYCGAMMA_LUT(1,pCtrl->Contrast_level) ; // Y
						
					// UARTprintf("PSCTRL_CON_UPDATE : A\r\n");
				}
					
				if ( (stPS.ctrl&PSCTRL_SAT_UPDATE) !=0 )
				{					
					SetTONE_AYCGAMMA_LUT(2,pCtrl->Saturation_level) ; // Cb
					SetTONE_AYCGAMMA_LUT(3,pCtrl->Saturation_level) ; // Cr
					
					// UARTprintf("PSCTRL_SAT_UPDATE : A\r\n");
				}
			}
			else
			{
				SetTONE_AYCGAMMA_LUT(0,3) ; // YCbCr			
			}	
		}	
		
		if ( contrast_value != PS_offset[prestyle][0] ) 
		{
			SetTONE_RGBGAMMA_val(contrast_value) ;
			
			// UARTprintf("contrast_value changed!! : %d \r\n", contrast_value);
		}
	}
	else
	{	// MDIN_PS_OFF : Picture style off

		contrast_value = stPS.stPSoff.Contrast_level;
		brightness_level = stPS.stPSoff.brightness_level;
		saturation_value = stPS.stPSoff.Saturation_level;
		hue_value = stPS.stPSoff.Hue_level;
		edge_value = stPS.stPSoff.EE_level ;
		
		DEMO_SetBrightness(brightness_level); // DEMO_SetBrightness() writes EEPROM
		DEMO_SetSharpness(edge_value) ; // DEMO_SetSharpness() writes EEPROM
		
		SetTONE_YCGAMMA_LUT(0,3) ; // YCbCr
		
		if ( GetTONE_AYCGAMMA_thermo() == OFF ) SetTONE_AYCGAMMA_LUT(0,3) ; // YCbCr		
		
		SetTONE_RGBGAMMA_val(contrast_value) ;
		
		// UARTprintf("Style off!!\r\n");
	}
	
	stPS.ctrl = 0 ; // clear control flag
	stPS.ss_prePS = style ;		

#if defined(__RGBGAMMA_USE_CONTRAST__)		
	SetMenuStatus(CONTRAST_MENU,contrast_value);
#endif

	SetMenuStatus(BRIGHTNESS_MENU,brightness_level);
	
	SetMenuStatus(SATURATION_MENU,saturation_value);	
	
	SetApp_PS_SetHue(hue_value);
	SetMenuStatus(HUE_MENU,hue_value);	

	SetMenuStatus(EDGE_MENU,edge_value);
	
	SetApp_PS_WriteEEPROM() ;
}
// ----------------------------------------------------------------------
static PMDIN_PS_PARAM SetApp_PS_SELECT(WORD style) 
{
	PMDIN_PS_PARAM pstStyle ;
	switch( style )
	{
		case MDIN_PS_STANDARD : pstStyle = (&stPS.stPSstd) ; break ;
		case MDIN_PS_VIVID : pstStyle = (&stPS.stPSvid) ; break ;
		case MDIN_PS_CLEAR : pstStyle = (&stPS.stPSclr) ; break ;
		case MDIN_PS_VIVID_CLEAR : pstStyle = (&stPS.stPSvclr) ; break ;
		case MDIN_PS_NEUTRAL : pstStyle = (&stPS.stPSneu) ; break ;
		default : pstStyle = (&stPS.stPSstd) ; break ; // MDIN_PS_OFF
	}		
	return pstStyle ;
}

// ----------------------------------------------------------------------
static void SetApp_PS_SetHue(WORD nID)
{
	if(nID==0) 		{SetOUT_MainHue(0); 			SetOUT_AuxHue(0);}
	else			{SetOUT_MainHue(nID*12+8); 		SetOUT_AuxHue(nID*12+8);}
}

// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

void SetApp_initPictureStyle( void )
{
	WORD init_style ;
	PMDIN_PS_PARAM pstStyle ;
	
	memset(&stPS, 0, sizeof(MDIN_PS_SET_PARAM));
	
	init_style = MDIN_PS_OFF ;
	stPS.ss_PS = init_style ;
	
#if defined(__RGBGAMMA_USE_CONTRAST__)				
	stPS.stPSoff.Contrast_level = DEMO_GetContrast();
#else
	stPS.stPSoff.Contrast_level = GetTONE_RGBGAMMA_val();
#endif
	stPS.stPSoff.brightness_level = DEMO_GetBrightness();
	stPS.stPSoff.Saturation_level = DEMO_GetSaturation();
	stPS.stPSoff.Hue_level = DEMO_GetHue();
	stPS.stPSoff.EE_level = DEMO_GetSharpness();	
	
	stPS.stPSstd.Contrast_level = 3 ;	 // 0 1 2 3 4 5 6
	stPS.stPSstd.Saturation_level = 3 ;
	stPS.stPSstd.EE_level = 3 ;

	stPS.stPSvid.Contrast_level = 3 ;	 // 0 1 2 3 4 5 6
	stPS.stPSvid.Saturation_level = 3 ;
	stPS.stPSvid.EE_level = 3 ;
	
	stPS.stPSclr.Contrast_level = 3 ;	 // 0 1 2 3 4 5 6
	stPS.stPSclr.Saturation_level = 3 ;
	stPS.stPSclr.EE_level = 3 ;

	stPS.stPSvclr.Contrast_level = 3 ;	 // 0 1 2 3 4 5 6
	stPS.stPSvclr.Saturation_level = 3 ;
	stPS.stPSvclr.EE_level = 3 ;	
	
	stPS.stPSneu.Contrast_level = 3 ;	 // 0 1 2 3 4 5 6
	stPS.stPSneu.Saturation_level = 3 ;
	stPS.stPSneu.EE_level = 3 ;		
	
	SetApp_PS_ReadEEPROM();	
	
	SetApp_ssPictureStyle(stPS.ss_PS);
	
	// Picture style Menu osd value overide
	pstStyle = SetApp_PS_SELECT(stPS.ss_PS) ;
	
	SetMenuStatus(PICSTYLE_MENU,stPS.ss_PS);

#if defined(__RGBGAMMA_USE_CONTRAST__)			
	SetMenuStatus(PICSTYLE_CON_MENU,pstStyle->Contrast_level);
#endif
	SetMenuStatus(PICSTYLE_SAT_MENU,pstStyle->Saturation_level);
	SetMenuStatus(PICSTYLE_EDE_MENU,pstStyle->EE_level);	
}

// ----------------------------------------------------------------------
void SetApp_ssPictureContrast( WORD level )
{
	WORD style ;
	PMDIN_PS_PARAM pstStyle ;
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlavePicContrast(level);
#endif
	
	style = stPS.ss_PS ;
	pstStyle = SetApp_PS_SELECT(style) ;
	
	if (level > 6) level = 6 ;
	
	stPS.ctrl |= PSCTRL_CON_UPDATE ;
	pstStyle->Contrast_level = level ;
	SetApp_PS_SETUP(pstStyle) ;

}
// ----------------------------------------------------------------------
void SetApp_ssPictureSaturation( WORD level )
{
	WORD style ;
	PMDIN_PS_PARAM pstStyle ;
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlavePicSaturation(level);
#endif
	
	style = stPS.ss_PS ;
	pstStyle = SetApp_PS_SELECT(style) ;
	
	if (level > 6) level = 6 ;
	
	stPS.ctrl |= PSCTRL_SAT_UPDATE ;
	pstStyle->Saturation_level = level ;
	SetApp_PS_SETUP(pstStyle) ;
}
// ----------------------------------------------------------------------
void SetApp_ssPictureEE( WORD level )
{
	WORD style ;
	PMDIN_PS_PARAM pstStyle ;
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlavePicEdgeEnhance(level);
#endif
	
	style = stPS.ss_PS ;
	pstStyle = SetApp_PS_SELECT(style) ;
	
	if (level > 6) level = 6 ;
	
	stPS.ctrl |= PSCTRL_EE_UPDATE ;
	pstStyle->EE_level = level ;
	SetApp_PS_SETUP(pstStyle) ;	
}
// ----------------------------------------------------------------------
void SetApp_ssPictureStyle( WORD style )
{
	PMDIN_PS_PARAM pstStyle ;
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlavePicStyle(style);
#endif
	
	if ( stPS.ss_PS == MDIN_PS_OFF && style != MDIN_PS_OFF )
	{
		stPS.stPSoff.Contrast_level = GetMenuStatus(CONTRAST_MENU);
		stPS.stPSoff.brightness_level = GetMenuStatus(BRIGHTNESS_MENU);
		stPS.stPSoff.Saturation_level = GetMenuStatus(SATURATION_MENU);
		stPS.stPSoff.Hue_level = GetMenuStatus(HUE_MENU);
		stPS.stPSoff.EE_level = GetMenuStatus(EDGE_MENU);	
		
		// UARTprintf("\r\nMDIN_PS_OFF to setup\r\n");
	}	
	stPS.ss_PS = style ;
	
	// UARTprintf("stPS.ss_PS : %d\r\n",stPS.ss_PS);
	// UARTprintf("stPS.stPSoff.Contrast_level : %d\r\n",stPS.stPSoff.Contrast_level);
	// UARTprintf("stPS.stPSoff.Saturation_level : %d\r\n",stPS.stPSoff.Saturation_level);
	// UARTprintf("stPS.stPSoff.Hue_level : %d\r\n",stPS.stPSoff.Hue_level);
	// UARTprintf("stPS.stPSoff.EE_level : %d\r\n\r\n",stPS.stPSoff.EE_level);	
	
	stPS.ctrl |= (PSCTRL_EE_UPDATE|PSCTRL_SAT_UPDATE|PSCTRL_CON_UPDATE) ;
	pstStyle = SetApp_PS_SELECT(style) ;
	
	SetApp_PS_SETUP(pstStyle) ;		
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void SetApp_PS_WriteEEPROM(void)
{
	#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
    BYTE buff[17];

	buff[0] = EEPROM_SAVE_DONE;
	buff[1] = stPS.ss_PS;                      buff[2] = stPS.stPSstd.Contrast_level;
	buff[3] = stPS.stPSstd.Saturation_level;   buff[4] = stPS.stPSstd.EE_level;
	buff[5] = stPS.stPSvid.Contrast_level;     buff[6] = stPS.stPSvid.Saturation_level;
	buff[7] = stPS.stPSvid.EE_level;           buff[8] = stPS.stPSclr.Contrast_level;
	buff[9] = stPS.stPSclr.Saturation_level;   buff[10] = stPS.stPSclr.EE_level;
	buff[11] = stPS.stPSvclr.Contrast_level;   buff[12] = stPS.stPSvclr.Saturation_level;
	buff[13] = stPS.stPSvclr.EE_level;         buff[14] = stPS.stPSneu.Contrast_level;
	buff[15] = stPS.stPSneu.Saturation_level;  buff[16] = stPS.stPSneu.EE_level;		

	EEPROM_MultiWrite(EEPROM_PICTURE_STYLE, (PBYTE)&buff[0], sizeof(buff));	
	
	#endif
}
// ----------------------------------------------------------------------
void SetApp_PS_ReadEEPROM(void)
{
	#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[17];
	 
	EEPROM_MultiRead(EEPROM_PICTURE_STYLE, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE)  
	{
		stPS.ss_PS = buff[1] ;                     stPS.stPSstd.Contrast_level = buff[2] ;
		stPS.stPSstd.Saturation_level = buff[3] ;  stPS.stPSstd.EE_level = buff[4] ;
		stPS.stPSvid.Contrast_level = buff[5] ;    stPS.stPSvid.Saturation_level = buff[6] ;
		stPS.stPSvid.EE_level = buff[7] ;          stPS.stPSclr.Contrast_level = buff[8] ;
		stPS.stPSclr.Saturation_level = buff[9] ;  stPS.stPSclr.EE_level = buff[10] ;
		stPS.stPSvclr.Contrast_level = buff[11] ;  stPS.stPSvclr.Saturation_level = buff[12] ;
		stPS.stPSvclr.EE_level = buff[13] ;        stPS.stPSneu.Contrast_level = buff[14] ;
		stPS.stPSneu.Saturation_level = buff[15] ; stPS.stPSneu.EE_level = buff[16] ;
	}	
	#endif
}
// ----------------------------------------------------------------------
void SetApp_PS_EraseEEPROM(void)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	WORD i;
	BYTE buff ;
	buff = 0xFF;
	
	for(i=0;i<17;i++) 		EEPROM_MultiWrite(EEPROM_PICTURE_STYLE+i, (PBYTE)&buff, 1);
#endif
}


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
WORD GetApp_ssPictureStyle( void )
{
	return stPS.ss_PS ;
}
// ----------------------------------------------------------------------
WORD GetApp_ssPictureContrast( void )
{
	PMDIN_PS_PARAM pstStyle ;

	pstStyle = SetApp_PS_SELECT(stPS.ss_PS) ;	
	return pstStyle->Contrast_level ;
}
// ----------------------------------------------------------------------
WORD GetApp_ssPictureSaturation( void )
{
	PMDIN_PS_PARAM pstStyle ;

	pstStyle = SetApp_PS_SELECT(stPS.ss_PS) ;	
	return pstStyle->Saturation_level ;
}
// ----------------------------------------------------------------------
WORD GetApp_ssPictureEE( void )
{
	PMDIN_PS_PARAM pstStyle ;

	pstStyle = SetApp_PS_SELECT(stPS.ss_PS) ;	
	return pstStyle->EE_level ;
}
//--------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
#endif // #if defined(__USE_MDIN_i510__)||defined(__USE_MDIN_i540__)||defined(__USE_MDIN_i550__)
//--------------------------------------------------------------------------------------------------------------------------



/*  FILE_END_HERE */
