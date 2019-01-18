

#ifndef		__MDIN_PICTURE_H__
#define		__MDIN_PICTURE_H__

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
// Struct/Union Types
// ----------------------------------------------------------------------
typedef	enum {
	MDIN_PS_OFF, 			// Default setting with no adjust ( For external input )
	MDIN_PS_STANDARD, 		// Standard setting
	MDIN_PS_VIVID,	  		// High saturated color
	MDIN_PS_CLEAR,	  		// High contrast High Edge enhancement
	MDIN_PS_VIVID_CLEAR,	// High saturated color High contrast High Edge enhancement
	MDIN_PS_NEUTRAL,  		// Minimum adjust
}	MISP_PS_TYPE_t;

typedef	struct
{								// - <-- * --> +
	WORD Contrast_level ;		// 0 1 2 3 4 5 6
	WORD Saturation_level ;		// 0 1 2 3 4 5 6
	WORD EE_level ;				// 0 1 2 3 4 5 6
} MDIN_PS_PARAM, *PMDIN_PS_PARAM;

typedef	struct
{							
	WORD Contrast_level ;		
	WORD brightness_level ;		
	WORD Saturation_level ;		
	WORD Hue_level ;			
	WORD EE_level ;				
} MDIN_PSOFF_PARAM, *PMDIN_PSOFF_PARAM;

typedef	struct
{
	LONG 			ctrl ;
	WORD			ss_PS ;
	WORD			ss_prePS ;

	MDIN_PSOFF_PARAM	stPSoff ;
	MDIN_PS_PARAM	stPSstd ;
	MDIN_PS_PARAM	stPSvid ;
	MDIN_PS_PARAM	stPSclr ;
	MDIN_PS_PARAM	stPSvclr ;
	MDIN_PS_PARAM	stPSneu ;
} MDIN_PS_SET_PARAM, *PMDIN_PS_SET_PARAM;

#define	PSCTRL_CON_UPDATE			0x00000001		// con update
#define	PSCTRL_SAT_UPDATE			0x00000002		// sat update
#define	PSCTRL_EE_UPDATE			0x00000004		// ee update
// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------
void SetApp_initPictureStyle(void);

void SetApp_ssPictureStyle( WORD style ) ;
void SetApp_ssPictureContrast( WORD level ) ;
void SetApp_ssPictureSaturation( WORD level ) ;
void SetApp_ssPictureEE( WORD level ) ;

// ----------------------------------------------------------------------
void SetApp_PS_WriteEEPROM(void);
void SetApp_PS_ReadEEPROM(void);
void SetApp_PS_EraseEEPROM(void);

// ----------------------------------------------------------------------
WORD GetApp_ssPictureStyle( void ) ;
WORD GetApp_ssPictureContrast( void ) ;
WORD GetApp_ssPictureSaturation( void ) ;
WORD GetApp_ssPictureEE( void ) ;
//--------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
#endif // #if defined(__USE_MDIN_i510__)||defined(__USE_MDIN_i540__)||defined(__USE_MDIN_i550__)
//--------------------------------------------------------------------------------------------------------------------------
#endif 


/*  FILE_END_HERE */
