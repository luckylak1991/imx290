//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2017  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  mispcac.c
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:


// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
// #include	"misp100.h"
#include	"video.h"

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
static CAC_CTRL_PARAM stCAC;
static CAC_APP_PARAM stAppCAC;

static ROMDATA WORD MISP_CACCos[]		= {		//degree 0 to 90
	1024,	1024,	1023,	1023,	1022,		//0~4
	1020,	1018,	1016,	1014,	1011,		//5~9
	1008,	1005,	1002,	998	,	994	,		//10~14
	989	,	984	,	979	,	974	,	968	,		//15~19
	962	,	956	,	949	,	943	,	935	,		//20~24
	928	,	920	,	912	,	904	,	896	,		//25~29
	887	,	878	,	868	,	859	,	849	,		//30~34
	839	,	828	,	818	,	807	,	796	,		//35~39
	784	,	773	,	761	,	749	,	737	,		//40~44
	724	,	711	,	698	,	685	,	672	,		//45~49
	658	,	644	,	630	,	616	,	602	,		//50~54
	587	,	573	,	558	,	543	,	527	,		//55~59
	512	,	496	,	481	,	465	,	449	,		//60~64
	433	,	416	,	400	,	384	,	367	,		//65~69
	350	,	333	,	316	,	299	,	282	,		//70~74
	265	,	248	,	230	,	213	,	195	,		//75~79
	178	,	160	,	143	,	125	,	107	,		//80~84
	89	,	71	,	54	,	36	,	18	,		//85~89
	0											//90
};
//sin(90вк-t) = cost

// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Device drive functions
// -----------------------------------------------------------------------------

static void Init_CAC_PARAM( PCAC_CTRL_PARAM pCTRL )
{
	pCTRL->stAREA_Magenta.gain=1024;
	pCTRL->stAREA_Magenta.cos=1024;
	pCTRL->stAREA_Magenta.sin=0;
	
	pCTRL->stAREA_Red.gain=1024;
	pCTRL->stAREA_Red.cos=1024;
	pCTRL->stAREA_Red.sin=0;
	
	pCTRL->stAREA_Yellow.gain=1024;
	pCTRL->stAREA_Yellow.cos=1024;
	pCTRL->stAREA_Yellow.sin=0;
	
	pCTRL->stAREA_Green.gain=1024;
	pCTRL->stAREA_Green.cos=1024;
	pCTRL->stAREA_Green.sin=0;
	
	pCTRL->stAREA_Cyan.gain=1024;		
	pCTRL->stAREA_Cyan.cos=1024;
	pCTRL->stAREA_Cyan.sin=0;
	
	pCTRL->stAREA_Blue.gain=1024;
	pCTRL->stAREA_Blue.cos=1024;	
	pCTRL->stAREA_Blue.sin=0;	

	pCTRL->disp_col_sel=0;
	pCTRL->disp_sel=0;
	pCTRL->disp_en=0;
	pCTRL->enable=1;
	pCTRL->bypass=0;
}

static void Init_CAC_APP_PARAM( PCAC_APP_PARAM pCTRL )
{
	pCTRL->Magenta_gain_menu=32;	
	pCTRL->Red_gain_menu=32;		
	pCTRL->Yellow_gain_menu=32;		
	pCTRL->Green_gain_menu=32;		
	pCTRL->Cyan_gain_menu=32;	
	pCTRL->Blue_gain_menu=32;		

	pCTRL->Magenta_cos_menu=32;	
	pCTRL->Red_cos_menu=32;		
	pCTRL->Yellow_cos_menu=32;		
	pCTRL->Green_cos_menu=32;		
	pCTRL->Cyan_cos_menu=32;	
	pCTRL->Blue_cos_menu=32;	

	pCTRL->Magenta_sin_menu=32;	
	pCTRL->Red_sin_menu=32;		
	pCTRL->Yellow_sin_menu=32;		
	pCTRL->Green_sin_menu=32;		
	pCTRL->Cyan_sin_menu=32;	
	pCTRL->Blue_sin_menu=32;
	
	pCTRL->Magenta_theta_menu=90;	
	pCTRL->Red_theta_menu=90;		
	pCTRL->Yellow_theta_menu=90;		
	pCTRL->Green_theta_menu=90;		
	pCTRL->Cyan_theta_menu=90;	
	pCTRL->Blue_theta_menu=90;			
}

static void Init_CAC_REGISTER( PCAC_CTRL_PARAM pCTRL )
{
	MISP_RegWrite(MISP_LOCAL_ID, 0x440,	(pCTRL->disp_col_sel<<6)|
										(pCTRL->disp_sel<<3)|
										(pCTRL->disp_en<<2)|
										(pCTRL->enable<<1)|
										pCTRL->bypass);	

	MISP_RegWrite(MISP_LOCAL_ID, 0x441,	pCTRL->stAREA_Magenta.cos);
	MISP_RegWrite(MISP_LOCAL_ID, 0x442,	pCTRL->stAREA_Red.cos);
	MISP_RegWrite(MISP_LOCAL_ID, 0x443,	pCTRL->stAREA_Yellow.cos);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x444,	pCTRL->stAREA_Green.cos);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x445,	pCTRL->stAREA_Cyan.cos);		
	MISP_RegWrite(MISP_LOCAL_ID, 0x446,	pCTRL->stAREA_Blue.cos);			

	MISP_RegWrite(MISP_LOCAL_ID, 0x447,	pCTRL->stAREA_Magenta.sin);
	MISP_RegWrite(MISP_LOCAL_ID, 0x448,	pCTRL->stAREA_Red.sin);
	MISP_RegWrite(MISP_LOCAL_ID, 0x449,	pCTRL->stAREA_Yellow.sin);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x44A,	pCTRL->stAREA_Green.sin);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x44B,	pCTRL->stAREA_Cyan.sin);		
	MISP_RegWrite(MISP_LOCAL_ID, 0x44C,	pCTRL->stAREA_Blue.sin);			

	MISP_RegWrite(MISP_LOCAL_ID, 0x44D,	pCTRL->stAREA_Magenta.gain);
	MISP_RegWrite(MISP_LOCAL_ID, 0x44E,	pCTRL->stAREA_Red.gain);
	MISP_RegWrite(MISP_LOCAL_ID, 0x44F,	pCTRL->stAREA_Yellow.gain);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x450,	pCTRL->stAREA_Green.gain);	
	MISP_RegWrite(MISP_LOCAL_ID, 0x451,	pCTRL->stAREA_Cyan.gain);		
	MISP_RegWrite(MISP_LOCAL_ID, 0x452,	pCTRL->stAREA_Blue.gain);		
	
}

void Init_CAC(void)
{
	Init_CAC_APP_PARAM(&stAppCAC);
	Init_CAC_PARAM(&stCAC);
	Init_CAC_REGISTER(&stCAC);
}
// -----------------------------------------------------------------------------
// api functions
// -----------------------------------------------------------------------------
void SetApp_CACSaturation(WORD level, WORD area)
{
	WORD sat_start = (1024-CAC_gain_pwr*32);
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveCACSaturation(level, area);
#endif
	
	// target saturation update & menu parameter save
	switch(area){
	//area 0 - Magenta		
		case 0 : 	stAppCAC.Magenta_gain_menu=level;
				Set_CAC_Magenta_gain(sat_start+CAC_gain_pwr*level);
				SetApp_AutoCAC_MagentaUpdate();
				break;		
	//area 1 - Red		
		case 1 : stAppCAC.Red_gain_menu=level;		
				Set_CAC_Red_gain(sat_start+CAC_gain_pwr*level);
				break;		
	//area 2 - Yellow		
		case 2 : 	stAppCAC.Yellow_gain_menu=level;	
				Set_CAC_Yellow_gain(sat_start+CAC_gain_pwr*level);
				break;		
	//area 3 - Green 		
		case 3 : 	stAppCAC.Green_gain_menu=level;
				Set_CAC_Green_gain(sat_start+CAC_gain_pwr*level);
				SetApp_AutoCAC_GreenUpdate();
				break;	
	//area 4 - Cyan		
		case 4 : stAppCAC.Cyan_gain_menu=level;	
				Set_CAC_Cyan_gain(sat_start+CAC_gain_pwr*level);
				break;		
	//area 5 - Blue			
		case 5 : 	stAppCAC.Blue_gain_menu=level;	
				Set_CAC_Blue_gain(sat_start+CAC_gain_pwr*level);
				break;		

		default :  break;
	}

	
}
WORD GetApp_CACSaturation(WORD area)
{
	//return menu value
	switch(area){
		case 0 : 	return stAppCAC.Magenta_gain_menu;		//area 0 - Magenta			
		case 1 : return stAppCAC.Red_gain_menu;			//area 1 - Red
		case 2 : return stAppCAC.Yellow_gain_menu;			//area 2 - Yellow	
		case 3 : return stAppCAC.Green_gain_menu;			//area 3 - Green 
		case 4 : return stAppCAC.Cyan_gain_menu;			//area 4 - Cyan			
		case 5 : return stAppCAC.Blue_gain_menu;			//area 5 - Blue	
		default : return 32; break;
	}
}

void SetApp_CACCos(WORD level, WORD area)
{
	//Cb out = (Cb in * cos theta) + (Cr in * sin theta)
	//Cr out = (Cr in * cos theta) - (Cb in * sin theta)
	WORD cos_start = (1024-CAC_cos_pwr*32);

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveCACCos(level, area);
#endif
	
	// target cos update & menu parameter save
	switch(area){
	//area 0 - Magenta		
		case 0 : 	stAppCAC.Magenta_cos_menu=level;
				Set_CAC_Magenta_cos(cos_start+CAC_cos_pwr*level);
				break;		
	//area 1 - Red		
		case 1 : stAppCAC.Red_cos_menu=level;		
				Set_CAC_Red_cos(cos_start+CAC_cos_pwr*level);
				break;		
	//area 2 - Yellow		
		case 2 : 	stAppCAC.Yellow_cos_menu=level;	
				Set_CAC_Yellow_cos(cos_start+CAC_cos_pwr*level);
				break;		
	//area 3 - Green 		
		case 3 : 	stAppCAC.Green_cos_menu=level;
				Set_CAC_Green_cos(cos_start+CAC_cos_pwr*level);
				break;	
	//area 4 - Cyan		
		case 4 : stAppCAC.Cyan_cos_menu=level;	
				Set_CAC_Cyan_cos(cos_start+CAC_cos_pwr*level);
				break;		
	//area 5 - Blue			
		case 5 : 	stAppCAC.Blue_cos_menu=level;	
				Set_CAC_Blue_cos(cos_start+CAC_cos_pwr*level);
				break;		

		default :  break;
	}

	
}
WORD GetApp_CACCos(WORD area)
{
	//return menu value
	switch(area){
		case 0 : 	return stAppCAC.Magenta_cos_menu;		//area 0 - Magenta			
		case 1 : return stAppCAC.Red_cos_menu;			//area 1 - Red
		case 2 : return stAppCAC.Yellow_cos_menu;			//area 2 - Yellow	
		case 3 : return stAppCAC.Green_cos_menu;			//area 3 - Green 
		case 4 : return stAppCAC.Cyan_cos_menu;			//area 4 - Cyan			
		case 5 : return stAppCAC.Blue_cos_menu;			//area 5 - Blue	
		default : return 32; break;
	}
}

void SetApp_CACSin(WORD level, WORD area)
{
	//Cb out = (Cb in * cos theta) + (Cr in * sin theta)
	//Cr out = (Cr in * cos theta) - (Cb in * sin theta)
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveCACSin(level, area);
#endif
	
	// target cos update & menu parameter save
	switch(area){
	//area 0 - Magenta		
		case 0 : 	stAppCAC.Magenta_sin_menu=level;
				if(level>31)	Set_CAC_Magenta_sin(CAC_sin_pwr*(level-32));
				else			Set_CAC_Magenta_sin(4096-CAC_sin_pwr*(32-level));
				break;		
	//area 1 - Red		
		case 1 : stAppCAC.Red_sin_menu=level;		
				if(level>31)	Set_CAC_Red_sin(CAC_sin_pwr*(level-32));
				else			Set_CAC_Red_sin(4096-CAC_sin_pwr*(32-level));
				break;		
	//area 2 - Yellow		
		case 2 : 	stAppCAC.Yellow_sin_menu=level;	
				if(level>31)	Set_CAC_Yellow_sin(CAC_sin_pwr*(level-32));
				else			Set_CAC_Yellow_sin(4096-CAC_sin_pwr*(32-level));
				break;		
	//area 3 - Green 		
		case 3 : 	stAppCAC.Green_sin_menu=level;
				if(level>31)	Set_CAC_Green_sin(CAC_sin_pwr*(level-32));
				else			Set_CAC_Green_sin(4096-CAC_sin_pwr*(32-level));
				break;	
	//area 4 - Cyan		
		case 4 : stAppCAC.Cyan_sin_menu=level;	
				if(level>31)	Set_CAC_Cyan_sin(CAC_sin_pwr*(level-32));
				else			Set_CAC_Cyan_sin(4096-CAC_sin_pwr*(32-level));
				break;		
	//area 5 - Blue			
		case 5 : 	stAppCAC.Blue_sin_menu=level;	
				if(level>31)	Set_CAC_Blue_sin(CAC_sin_pwr*(level-32));
				else			Set_CAC_Blue_sin(4096-CAC_sin_pwr*(32-level));
				break;		

		default :  break;
	}

	
}

WORD GetApp_CACSin(WORD area)
{
	//return menu value
	switch(area){
		case 0 : 	return stAppCAC.Magenta_sin_menu;		//area 0 - Magenta			
		case 1 : return stAppCAC.Red_sin_menu;			//area 1 - Red
		case 2 : return stAppCAC.Yellow_sin_menu;			//area 2 - Yellow	
		case 3 : return stAppCAC.Green_sin_menu;			//area 3 - Green 
		case 4 : return stAppCAC.Cyan_sin_menu;			//area 4 - Cyan			
		case 5 : return stAppCAC.Blue_sin_menu;			//area 5 - Blue	
		default : return 32; break;
	}
}

void SetApp_CACTheta(WORD level, WORD area)
{
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveCACTheta(level, area);
#endif
	
	switch(area){
	//area 0 - Magenta		
		case 0 : 	stAppCAC.Magenta_theta_menu=level;
				if(level>=90){	
					Set_CAC_Magenta_cos(MISP_CACCos[level-90]);
					#if defined(CAC_counterclockwise)	
					Set_CAC_Magenta_sin(4096-MISP_CACCos[180-level]);
					#else
					Set_CAC_Magenta_sin(MISP_CACCos[180-level]);
					#endif
				}
				else{	
					Set_CAC_Magenta_cos(MISP_CACCos[90-level]);
					#if defined(CAC_counterclockwise)						
					Set_CAC_Magenta_sin(MISP_CACCos[level]);
					#else
					Set_CAC_Magenta_sin(4096-MISP_CACCos[level]);
					#endif
				}
				break;		
	//area 1 - Red		
		case 1 : stAppCAC.Red_theta_menu=level;		
				if(level>=90){
					Set_CAC_Red_cos(MISP_CACCos[level-90]);
					#if defined(CAC_counterclockwise)						
					Set_CAC_Red_sin(4096-MISP_CACCos[180-level]);
					#else
					Set_CAC_Red_sin(MISP_CACCos[180-level]);
					#endif
					}
				else	{
					Set_CAC_Red_cos(MISP_CACCos[90-level]);		
					#if defined(CAC_counterclockwise)						
					Set_CAC_Red_sin(MISP_CACCos[level]);
					#else
					Set_CAC_Red_sin(4096-MISP_CACCos[level]);
					#endif
				}
				break;		
	//area 2 - Yellow		
		case 2 : 	stAppCAC.Yellow_theta_menu=level;	
				if(level>=90){
					Set_CAC_Yellow_cos(MISP_CACCos[level-90]);
					#if defined(CAC_counterclockwise)						
					Set_CAC_Yellow_sin(4096-MISP_CACCos[180-level]);
					#else
					Set_CAC_Yellow_sin(MISP_CACCos[180-level]);
					#endif
					}
				else	{
					Set_CAC_Yellow_cos(MISP_CACCos[90-level]);					
					#if defined(CAC_counterclockwise)						
					Set_CAC_Yellow_sin(MISP_CACCos[level]);
					#else
					Set_CAC_Yellow_sin(4096-MISP_CACCos[level]);
					#endif
				}
				break;		
	//area 3 - Green 		
		case 3 : 	stAppCAC.Green_theta_menu=level;
				if(level>=90){
					Set_CAC_Green_cos(MISP_CACCos[level-90]);
					#if defined(CAC_counterclockwise)						
					Set_CAC_Green_sin(4096-MISP_CACCos[180-level]);					
					#else
					Set_CAC_Green_sin(MISP_CACCos[180-level]);
					#endif
					}
				else	{
					Set_CAC_Green_cos(MISP_CACCos[90-level]);		
					#if defined(CAC_counterclockwise)						
					Set_CAC_Green_sin(MISP_CACCos[level]);
					#else
					Set_CAC_Green_sin(4096-MISP_CACCos[level]);
					#endif
				}
				break;		
	//area 4 - Cyan		
		case 4 : stAppCAC.Cyan_theta_menu=level;	
				if(level>=90){
					Set_CAC_Cyan_cos(MISP_CACCos[level-90]);
					#if defined(CAC_counterclockwise)						
					Set_CAC_Cyan_sin(4096-MISP_CACCos[180-level]);		
					#else
					Set_CAC_Cyan_sin(MISP_CACCos[180-level]);
					#endif
					}
				else	{
					Set_CAC_Cyan_cos(MISP_CACCos[90-level]);		
					#if defined(CAC_counterclockwise)						
					Set_CAC_Cyan_sin(MISP_CACCos[level]);
					#else
					Set_CAC_Cyan_sin(4096-MISP_CACCos[level]);
					#endif
				}
				break;		
	//area 5 - Blue			
		case 5 : 	stAppCAC.Blue_theta_menu=level;	
				if(level>=90){
					Set_CAC_Blue_cos(MISP_CACCos[level-90]);
					#if defined(CAC_counterclockwise)						
					Set_CAC_Blue_sin(4096-MISP_CACCos[180-level]);						
					#else
					Set_CAC_Blue_sin(MISP_CACCos[180-level]);
					#endif
					}
				else	{
					Set_CAC_Blue_cos(MISP_CACCos[90-level]);			
					#if defined(CAC_counterclockwise)						
					Set_CAC_Blue_sin(MISP_CACCos[level]);
					#else
					Set_CAC_Blue_sin(4096-MISP_CACCos[level]);
					#endif					
				}
				break;	

		default :  break;
	}


}
WORD GetApp_CACTheta(WORD area)
{
	//return menu value
	switch(area){
		case 0 : 	return stAppCAC.Magenta_theta_menu;		//area 0 - Magenta			
		case 1 : return stAppCAC.Red_theta_menu;			//area 1 - Red
		case 2 : return stAppCAC.Yellow_theta_menu;			//area 2 - Yellow	
		case 3 : return stAppCAC.Green_theta_menu;			//area 3 - Green 
		case 4 : return stAppCAC.Cyan_theta_menu;			//area 4 - Cyan			
		case 5 : return stAppCAC.Blue_theta_menu;			//area 5 - Blue	
		default : return 32; break;
	}
}

void SetApp_CAC_WriteEEPROM(void)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[28];
#endif
	
#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveCACWriteEEPROM();
#endif
	
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	EEPROM_MultiRead(EEPROM_CAC, (PBYTE)&buff[0], sizeof(buff));

	#if defined(CAC_ctrl_theta)
	buff[0] = EEPROM_SAVE_DONE;		//gain done
	buff[3] = EEPROM_SAVE_DONE;		//theta done

	buff[4] = (BYTE)stAppCAC.Magenta_gain_menu;                      
	buff[5] = (BYTE)stAppCAC.Red_gain_menu;
	buff[6] = (BYTE)stAppCAC.Yellow_gain_menu;   
	buff[7] = (BYTE)stAppCAC.Green_gain_menu;
	buff[8] = (BYTE)stAppCAC.Cyan_gain_menu;     
	buff[9] = (BYTE)stAppCAC.Blue_gain_menu;	

	buff[22] = (BYTE)stAppCAC.Magenta_theta_menu;           
	buff[23] = (BYTE)stAppCAC.Red_theta_menu;
	buff[24] = (BYTE)stAppCAC.Yellow_theta_menu;   
	buff[25] = (BYTE)stAppCAC.Green_theta_menu;
	buff[26] = (BYTE)stAppCAC.Cyan_theta_menu;   
	buff[27] = (BYTE)stAppCAC.Blue_theta_menu;		
	#else
	buff[0] = EEPROM_SAVE_DONE;		//gain done
	buff[1] = EEPROM_SAVE_DONE;		//cos done
	buff[2] = EEPROM_SAVE_DONE;		//sin done

	buff[4] = (BYTE)stAppCAC.Magenta_gain_menu;                      
	buff[5] = (BYTE)stAppCAC.Red_gain_menu;
	buff[6] = (BYTE)stAppCAC.Yellow_gain_menu;   
	buff[7] = (BYTE)stAppCAC.Green_gain_menu;
	buff[8] = (BYTE)stAppCAC.Cyan_gain_menu;     
	buff[9] = (BYTE)stAppCAC.Blue_gain_menu;	

	buff[10] = (BYTE)stAppCAC.Magenta_cos_menu;           
	buff[11] = (BYTE)stAppCAC.Red_cos_menu;
	buff[12] = (BYTE)stAppCAC.Yellow_cos_menu;   
	buff[13] = (BYTE)stAppCAC.Green_cos_menu;
	buff[14] = (BYTE)stAppCAC.Cyan_cos_menu;   
	buff[15] = (BYTE)stAppCAC.Blue_cos_menu;
	
	buff[16] = (BYTE)stAppCAC.Magenta_sin_menu;           
	buff[17] = (BYTE)stAppCAC.Red_sin_menu;
	buff[18] = (BYTE)stAppCAC.Yellow_sin_menu;   
	buff[19] = (BYTE)stAppCAC.Green_sin_menu;
	buff[20] = (BYTE)stAppCAC.Cyan_sin_menu;   
	buff[21] = (BYTE)stAppCAC.Blue_sin_menu;		
	#endif
		
	EEPROM_MultiWrite(EEPROM_CAC, (PBYTE)&buff[0], sizeof(buff));	
	
#endif
}
// ----------------------------------------------------------------------
void SetApp_CAC_ReadEEPROM(void)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[28];
	 
	EEPROM_MultiRead(EEPROM_CAC, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE){  //if gain done
		stAppCAC.Magenta_gain_menu = (WORD)buff[4];
		stAppCAC.Red_gain_menu = (WORD)buff[5];
		stAppCAC.Yellow_gain_menu = (WORD)buff[6]; 
		stAppCAC.Green_gain_menu = (WORD)buff[7]; 
		stAppCAC.Cyan_gain_menu = (WORD)buff[8];
		stAppCAC.Blue_gain_menu = (WORD)buff[9];
	}	
	else{
		stAppCAC.Magenta_gain_menu = 32;		stAppCAC.Red_gain_menu = 32;
		stAppCAC.Yellow_gain_menu = 32;		stAppCAC.Green_gain_menu = 32;
		stAppCAC.Cyan_gain_menu = 32;		stAppCAC.Blue_gain_menu = 32;
	}

	if(buff[1] == EEPROM_SAVE_DONE){  //if cos done
		stAppCAC.Magenta_cos_menu = (WORD)buff[10];
		stAppCAC.Red_cos_menu = (WORD)buff[11];
		stAppCAC.Yellow_cos_menu = (WORD)buff[12]; 
		stAppCAC.Green_cos_menu = (WORD)buff[13]; 
		stAppCAC.Cyan_cos_menu = (WORD)buff[14];
		stAppCAC.Blue_cos_menu = (WORD)buff[15];
	}
	else{
		stAppCAC.Magenta_cos_menu = 32;		stAppCAC.Red_cos_menu = 32;
		stAppCAC.Yellow_cos_menu = 32;		stAppCAC.Green_cos_menu = 32;
		stAppCAC.Cyan_cos_menu = 32;			stAppCAC.Blue_cos_menu = 32;
	}
	
	if(buff[2] == EEPROM_SAVE_DONE){  //if sine done
		stAppCAC.Magenta_sin_menu = (WORD)buff[16];
		stAppCAC.Red_sin_menu = (WORD)buff[17];
		stAppCAC.Yellow_sin_menu = (WORD)buff[18]; 
		stAppCAC.Green_sin_menu = (WORD)buff[19]; 
		stAppCAC.Cyan_sin_menu = (WORD)buff[20];
		stAppCAC.Blue_sin_menu = (WORD)buff[21];
	}
	else{
		stAppCAC.Magenta_sin_menu = 32;		stAppCAC.Red_sin_menu = 32;
		stAppCAC.Yellow_sin_menu = 32; 		stAppCAC.Green_sin_menu = 32; 
		stAppCAC.Cyan_sin_menu = 32;			stAppCAC.Blue_sin_menu = 32;
	}

	if(buff[3] == EEPROM_SAVE_DONE){  //if theta done
		stAppCAC.Magenta_theta_menu = (WORD)buff[22];
		stAppCAC.Red_theta_menu = (WORD)buff[23];
		stAppCAC.Yellow_theta_menu = (WORD)buff[24]; 
		stAppCAC.Green_theta_menu = (WORD)buff[25]; 
		stAppCAC.Cyan_theta_menu = (WORD)buff[26];
		stAppCAC.Blue_theta_menu = (WORD)buff[27];
	}		
	else{
		stAppCAC.Magenta_theta_menu = 90;		stAppCAC.Red_theta_menu = 90;
		stAppCAC.Yellow_theta_menu = 90;		stAppCAC.Green_theta_menu = 90;
		stAppCAC.Cyan_theta_menu = 90;		stAppCAC.Blue_theta_menu =90;
	}
/*		
	#if defined(CAC_ctrl_theta)
	WORD sat_start = (1024-CAC_gain_pwr*32);

	stCAC.stAREA_Magenta.gain=sat_start+CAC_gain_pwr*(stAppCAC.Magenta_gain_menu);
	stCAC.stAREA_Red.gain=sat_start+CAC_gain_pwr*(stAppCAC.Red_gain_menu);
	stCAC.stAREA_Yellow.gain=sat_start+CAC_gain_pwr*(stAppCAC.Yellow_gain_menu);	
	stCAC.stAREA_Green.gain=sat_start+CAC_gain_pwr*(stAppCAC.Green_gain_menu);	
	stCAC.stAREA_Cyan.gain=sat_start+CAC_gain_pwr*(stAppCAC.Cyan_gain_menu);		
	stCAC.stAREA_Blue.gain=sat_start+CAC_gain_pwr*(stAppCAC.Blue_gain_menu);	

	stCAC.stAREA_Magenta.cos=(stAppCAC.Magenta_theta_menu>=90)? (MISP_CACCos[stAppCAC.Magenta_theta_menu-90]) : (MISP_CACCos[90-stAppCAC.Magenta_theta_menu]);
	stCAC.stAREA_Red.cos=(stAppCAC.Red_theta_menu>=90)? (MISP_CACCos[stAppCAC.Red_theta_menu-90]) : (MISP_CACCos[90-stAppCAC.Red_theta_menu]);
	stCAC.stAREA_Yellow.cos=(stAppCAC.Yellow_theta_menu>=90)? (MISP_CACCos[stAppCAC.Yellow_theta_menu-90]) : (MISP_CACCos[90-stAppCAC.Yellow_theta_menu]);
	stCAC.stAREA_Green.cos=(stAppCAC.Green_theta_menu>=90)? (MISP_CACCos[stAppCAC.Green_theta_menu-90]) : (MISP_CACCos[90-stAppCAC.Green_theta_menu]);
	stCAC.stAREA_Cyan.cos=(stAppCAC.Cyan_theta_menu>=90)? (MISP_CACCos[stAppCAC.Cyan_theta_menu-90]) : (MISP_CACCos[90-stAppCAC.Cyan_theta_menu]);
	stCAC.stAREA_Blue.cos=(stAppCAC.Blue_theta_menu>=90)? (MISP_CACCos[stAppCAC.Blue_theta_menu-90]) : (MISP_CACCos[90-stAppCAC.Blue_theta_menu]);

	#if defined(CAC_counterclockwise)			
	stCAC.stAREA_Magenta.sin=(stAppCAC.Magenta_theta_menu>=90)? (4096-MISP_CACCos[180-stAppCAC.Magenta_theta_menu]) : (MISP_CACCos[stAppCAC.Magenta_theta_menu]);
	stCAC.stAREA_Red.sin= (stAppCAC.Red_theta_menu>=90)? (4096-MISP_CACCos[180-stAppCAC.Red_theta_menu]) : (MISP_CACCos[stAppCAC.Red_theta_menu]);
	stCAC.stAREA_Yellow.sin= (stAppCAC.Yellow_theta_menu>=90)? (4096-MISP_CACCos[180-stAppCAC.Yellow_theta_menu]) : (MISP_CACCos[stAppCAC.Yellow_theta_menu]);
	stCAC.stAREA_Green.sin= (stAppCAC.Green_theta_menu>=90)? (4096-MISP_CACCos[180-stAppCAC.Green_theta_menu]) : (MISP_CACCos[stAppCAC.Green_theta_menu]);
	stCAC.stAREA_Cyan.sin= (stAppCAC.Cyan_theta_menu>=90)? (4096-MISP_CACCos[180-stAppCAC.Cyan_theta_menu]) : (MISP_CACCos[stAppCAC.Cyan_theta_menu]);
	stCAC.stAREA_Blue.sin= (stAppCAC.Blue_theta_menu>=90)? (4096-MISP_CACCos[180-stAppCAC.Blue_theta_menu]) : (MISP_CACCos[stAppCAC.Blue_theta_menu]);
	#else
	stCAC.stAREA_Magenta.sin=(stAppCAC.Magenta_theta_menu>=90)? (MISP_CACCos[180-stAppCAC.Magenta_theta_menu]) : (4096-MISP_CACCos[stAppCAC.Magenta_theta_menu]);
	stCAC.stAREA_Red.sin= (stAppCAC.Red_theta_menu>=90)? (MISP_CACCos[180-stAppCAC.Red_theta_menu]) : (4096-MISP_CACCos[stAppCAC.Red_theta_menu]);
	stCAC.stAREA_Yellow.sin= (stAppCAC.Yellow_theta_menu>=90)? (MISP_CACCos[180-stAppCAC.Yellow_theta_menu]) : (4096-MISP_CACCos[stAppCAC.Yellow_theta_menu]);
	stCAC.stAREA_Green.sin= (stAppCAC.Green_theta_menu>=90)? (MISP_CACCos[180-stAppCAC.Green_theta_menu]) : (4096-MISP_CACCos[stAppCAC.Green_theta_menu]);
	stCAC.stAREA_Cyan.sin= (stAppCAC.Cyan_theta_menu>=90)? (MISP_CACCos[180-stAppCAC.Cyan_theta_menu]) : (4096-MISP_CACCos[stAppCAC.Cyan_theta_menu]);
	stCAC.stAREA_Blue.sin= (stAppCAC.Blue_theta_menu>=90)? (MISP_CACCos[180-stAppCAC.Blue_theta_menu]) : (4096-MISP_CACCos[stAppCAC.Blue_theta_menu]);
	#endif

	#else
	WORD sat_start = (1024-CAC_gain_pwr*32);
	WORD cos_start = (1024-CAC_cos_pwr*32);

	stCAC.stAREA_Magenta.gain=sat_start+CAC_gain_pwr*(stAppCAC.Magenta_gain_menu);
	stCAC.stAREA_Magenta.cos=cos_start+CAC_cos_pwr*(stAppCAC.Magenta_cos_menu);
	stCAC.stAREA_Magenta.sin= (stAppCAC.Magenta_sin_menu>31)? (CAC_sin_pwr*(stAppCAC.Magenta_sin_menu-32)) : (4096-CAC_sin_pwr*(32-stAppCAC.Magenta_sin_menu));
	
	stCAC.stAREA_Red.gain=sat_start+CAC_gain_pwr*(stAppCAC.Red_gain_menu);
	stCAC.stAREA_Red.cos=cos_start+CAC_cos_pwr*(stAppCAC.Red_cos_menu);
	stCAC.stAREA_Red.sin= (stAppCAC.Red_sin_menu>31)? (CAC_sin_pwr*(stAppCAC.Red_sin_menu-32)) : (4096-CAC_sin_pwr*(32-stAppCAC.Red_sin_menu));
	
	stCAC.stAREA_Yellow.gain=sat_start+CAC_gain_pwr*(stAppCAC.Yellow_gain_menu);
	stCAC.stAREA_Yellow.cos=cos_start+CAC_cos_pwr*(stAppCAC.Yellow_cos_menu);
	stCAC.stAREA_Yellow.sin= (stAppCAC.Yellow_sin_menu>31)? (CAC_sin_pwr*(stAppCAC.Yellow_sin_menu-32)) : (4096-CAC_sin_pwr*(32-stAppCAC.Yellow_sin_menu));
	
	stCAC.stAREA_Green.gain=sat_start+CAC_gain_pwr*(stAppCAC.Green_gain_menu);
	stCAC.stAREA_Green.cos=cos_start+CAC_cos_pwr*(stAppCAC.Green_cos_menu);
	stCAC.stAREA_Green.sin= (stAppCAC.Green_sin_menu>31)? (CAC_sin_pwr*(stAppCAC.Green_sin_menu-32)) : (4096-CAC_sin_pwr*(32-stAppCAC.Green_sin_menu));
	
	stCAC.stAREA_Cyan.gain=sat_start+CAC_gain_pwr*(stAppCAC.Cyan_gain_menu);	
	stCAC.stAREA_Cyan.cos=cos_start+CAC_cos_pwr*(stAppCAC.Cyan_cos_menu);
	stCAC.stAREA_Cyan.sin= (stAppCAC.Cyan_sin_menu>31)? (CAC_sin_pwr*(stAppCAC.Cyan_sin_menu-32)) : (4096-CAC_sin_pwr*(32-stAppCAC.Cyan_sin_menu));
	
	stCAC.stAREA_Blue.gain=sat_start+CAC_gain_pwr*(stAppCAC.Blue_gain_menu);	
	stCAC.stAREA_Blue.cos=cos_start+CAC_cos_pwr*(stAppCAC.Blue_cos_menu);
	stCAC.stAREA_Blue.sin= (stAppCAC.Blue_sin_menu>31)? (CAC_sin_pwr*(stAppCAC.Blue_sin_menu-32)) : (4096-CAC_sin_pwr*(32-stAppCAC.Blue_sin_menu));
	#endif	
*/
	
#endif	
}
// ----------------------------------------------------------------------
void SetApp_CAC_DefaultEEPROM(void)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[28];
	
	buff[0] = EEPROM_SAVE_DONE;		//gain done
	buff[1] = EEPROM_SAVE_DONE;		//cos done
	buff[2] = EEPROM_SAVE_DONE;		//sin done	
	buff[3] = EEPROM_SAVE_DONE;		//theta done

	buff[4] = 32;		buff[5] = 32;		buff[6] = 32;		buff[7] = 32;		buff[8] = 32;		buff[9] = 32;			//gain
	buff[10] = 32;		buff[11] = 32;		buff[12] = 32;		buff[13] = 32;		buff[14] = 32;		buff[15] = 32;			//cos
	buff[16] = 32;		buff[17] = 32;		buff[18] = 32;		buff[19] = 32;		buff[20] = 32;		buff[21] = 32;			//sin	
	
	buff[22] = 90;        buff[23] = 90;	buff[24] = 90;		buff[25] = 90;		buff[26] = 90;		buff[27] = 90;			//theta 
		
	EEPROM_MultiWrite(EEPROM_PICTURE_STYLE, (PBYTE)&buff[0], sizeof(buff));	
#endif
}

// -----------------------------------------------------------------------------
// Get & Set functions
// -----------------------------------------------------------------------------

void Set_CAC_Bypass(BOOL OnOff)
{
	stCAC.bypass = OnOff;
	MISP_RegField(MISP_LOCAL_ID, 0x440, 0, 1, stCAC.bypass);	
}

void Set_CAC_Enable(BOOL OnOff)
{
	stCAC.enable = OnOff;
	MISP_RegField(MISP_LOCAL_ID, 0x440, 1, 1, stCAC.enable);		
}

BOOL Get_CAC_Enable(void)
{
	return stCAC.enable;
}
// -----------------------------------------------------------------------------
void Set_CAC_debugdisp_en(BOOL OnOff)
{
	stCAC.disp_en = OnOff;
	MISP_RegField(MISP_LOCAL_ID, 0x440, 2, 1, stCAC.disp_en);			
}

void Set_CAC_debugdisp_sel(BYTE sel, BYTE col)
{
	stCAC.disp_sel = sel ;		
	stCAC.disp_col_sel = col ;	
	MISP_RegField(MISP_LOCAL_ID, 0x440, 3, 3, stCAC.disp_sel);		
	MISP_RegField(MISP_LOCAL_ID, 0x440, 6, 2, stCAC.disp_col_sel);		
}
// -----------------------------------------------------------------------------
void Set_CAC_Magenta_gain(WORD gain)
{
	stCAC.stAREA_Magenta.gain = gain;
	MISP_RegWrite(MISP_LOCAL_ID, 0x44D,	stCAC.stAREA_Magenta.gain);
}
void Set_CAC_Magenta_sin(WORD sin)
{
	stCAC.stAREA_Magenta.sin = sin;
	MISP_RegWrite(MISP_LOCAL_ID, 0x447,	stCAC.stAREA_Magenta.sin);	
}
void Set_CAC_Magenta_cos(WORD cos)
{
	stCAC.stAREA_Magenta.cos = cos;
	MISP_RegWrite(MISP_LOCAL_ID, 0x441,	stCAC.stAREA_Magenta.cos);		
}
WORD Get_CAC_Magenta_gain(void)
{
	return stCAC.stAREA_Magenta.gain;
}
WORD Get_CAC_Magenta_sin(void)
{
	return stCAC.stAREA_Magenta.sin;
}
WORD Get_CAC_Magenta_cos(void)
{
	return stCAC.stAREA_Magenta.cos;
}
// -----------------------------------------------------------------------------
void Set_CAC_Red_gain(WORD gain)
{
	stCAC.stAREA_Red.gain = gain;
	MISP_RegWrite(MISP_LOCAL_ID, 0x44E,	stCAC.stAREA_Red.gain);	
}
void Set_CAC_Red_sin(WORD sin)
{
	stCAC.stAREA_Red.sin = sin;
	MISP_RegWrite(MISP_LOCAL_ID, 0x448,	stCAC.stAREA_Red.sin);	
}
void Set_CAC_Red_cos(WORD cos)
{
	stCAC.stAREA_Red.cos = cos;
	MISP_RegWrite(MISP_LOCAL_ID, 0x442,	stCAC.stAREA_Red.cos);	
}
WORD Get_CAC_Red_gain(void)
{
	return stCAC.stAREA_Red.gain;
}
WORD Get_CAC_Red_sin(void)
{
	return stCAC.stAREA_Red.sin;
}
WORD Get_CAC_Red_cos(void)
{
	return stCAC.stAREA_Red.cos;
}
// -----------------------------------------------------------------------------
void Set_CAC_Yellow_gain(WORD gain)
{
	stCAC.stAREA_Yellow.gain = gain;
	MISP_RegWrite(MISP_LOCAL_ID, 0x44F,	stCAC.stAREA_Yellow.gain);		
}
void Set_CAC_Yellow_sin(WORD sin)
{
	stCAC.stAREA_Yellow.sin = sin;
	MISP_RegWrite(MISP_LOCAL_ID, 0x449,	stCAC.stAREA_Yellow.sin);		
}
void Set_CAC_Yellow_cos(WORD cos)
{
	stCAC.stAREA_Yellow.cos = cos;
	MISP_RegWrite(MISP_LOCAL_ID, 0x443,	stCAC.stAREA_Yellow.cos);		
}
WORD Get_CAC_Yellow_gain(void)
{
	return stCAC.stAREA_Yellow.gain;
}
WORD Get_CAC_Yellow_sin(void)
{
	return stCAC.stAREA_Yellow.sin;
}
WORD Get_CAC_Yellow_cos(void)
{
	return stCAC.stAREA_Yellow.cos;
}
// -----------------------------------------------------------------------------
void Set_CAC_Green_gain(WORD gain)
{
	stCAC.stAREA_Green.gain = gain;
	MISP_RegWrite(MISP_LOCAL_ID, 0x450,	stCAC.stAREA_Green.gain);		
}
void Set_CAC_Green_sin(WORD sin)
{
	stCAC.stAREA_Green.sin = sin;
	MISP_RegWrite(MISP_LOCAL_ID, 0x44A,	stCAC.stAREA_Green.sin);		
}
void Set_CAC_Green_cos(WORD cos)
{
	stCAC.stAREA_Green.cos = cos;
	MISP_RegWrite(MISP_LOCAL_ID, 0x444,	stCAC.stAREA_Green.cos);		
}
WORD Get_CAC_Green_gain(void)
{
	return stCAC.stAREA_Green.gain;
}
WORD Get_CAC_Green_sin(void)
{
	return stCAC.stAREA_Green.sin;
}
WORD Get_CAC_Green_cos(void)
{
	return stCAC.stAREA_Green.cos;
}
// -----------------------------------------------------------------------------
void Set_CAC_Cyan_gain(WORD gain)
{
	stCAC.stAREA_Cyan.gain = gain;
	MISP_RegWrite(MISP_LOCAL_ID, 0x451,	stCAC.stAREA_Cyan.gain);			
}
void Set_CAC_Cyan_sin(WORD sin)
{
	stCAC.stAREA_Cyan.sin = sin;
	MISP_RegWrite(MISP_LOCAL_ID, 0x44B,	stCAC.stAREA_Cyan.sin);			
}
void Set_CAC_Cyan_cos(WORD cos)
{
	stCAC.stAREA_Cyan.cos = cos;
	MISP_RegWrite(MISP_LOCAL_ID, 0x445,	stCAC.stAREA_Cyan.cos);	
}
WORD Get_CAC_Cyan_gain(void)
{
	return stCAC.stAREA_Cyan.gain;
}
WORD Get_CAC_Cyan_sin(void)
{
	return stCAC.stAREA_Cyan.sin;
}
WORD Get_CAC_Cyan_cos(void)
{
	return stCAC.stAREA_Cyan.cos;
}
// -----------------------------------------------------------------------------
void Set_CAC_Blue_gain(WORD gain)
{
	stCAC.stAREA_Blue.gain = gain;
	MISP_RegWrite(MISP_LOCAL_ID, 0x452, stCAC.stAREA_Blue.gain);			
}
void Set_CAC_Blue_sin(WORD sin)
{
	stCAC.stAREA_Blue.sin = sin;
	MISP_RegWrite(MISP_LOCAL_ID, 0x44C, stCAC.stAREA_Blue.sin);		
}
void Set_CAC_Blue_cos(WORD cos)
{
	stCAC.stAREA_Blue.cos = cos;
	MISP_RegWrite(MISP_LOCAL_ID, 0x446,	stCAC.stAREA_Blue.cos);			
}
WORD Get_CAC_Blue_gain(void)
{
	return stCAC.stAREA_Blue.gain;
}
WORD Get_CAC_Blue_sin(void)
{
	return stCAC.stAREA_Blue.sin;
}
WORD Get_CAC_Blue_cos(void)
{
	return stCAC.stAREA_Blue.cos;
}
// -----------------------------------------------------------------------------




