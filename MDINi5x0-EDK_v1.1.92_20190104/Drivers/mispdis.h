//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  deadpix.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MISP_DIS_H__  
#define		__MISP_DIS_H__  


// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
typedef struct {
	// main..
	WORD DIS_Coordinate_XL;		// left area start X-coordinate
	WORD DIS_Coordinate_XC; 	// center area start X-coordinate
	WORD DIS_Coordinate_XR; 	// right area start X-coordinate
	WORD DIS_Coordinate_YU; 	// up area start Y-coordinate
	WORD DIS_Coordinate_YD; 	// down area start Y-coordinate
	// interval
	WORD x_interval;			// X-coordinate interval
	WORD y_interval;	 		// Y-coordinate interval
	
	// AUX
	WORD DIS_AUX_Coordinate_XL; 	// left area start X-coordinate. AUX
	WORD DIS_AUX_Coordinate_XR; 	// right area start X-coordinate. AUX
	WORD DIS_AUX_Coordinate_YU; 	// up area start Y-coordinate. AUX
	WORD DIS_AUX_Coordinate_YD; 	// down area start Y-coordinate. AUX
	// interval
	WORD aux_x_interval; 			// X-coordinate interval
	WORD aux_y_interval; 			// Y-coordinate interval
	
	// offset(overscan range)
	WORD DIS_OFFSET_X;				// left, right overscan range
	WORD DIS_OFFSET_Y; 				// up, down overscan range
}	DIS_ROI;





// -----------------------------------------------------------------------------
// External Variables declaration
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------

// dis.c
void ISPM DIS_VectorWrite(void);
void DIS_OSD_Display(void);
void ISPM DIS_VsyncPulse(void);
void DISCommunication(void);
//void DISTestVect_Gen(WORD nID);
//void DIS_BayerPosVect_Gen(WORD nID);
//void DIS_BayerVect_Step(WORD nID);
//void DIS_osd(void);
void Init_DIS(void);
void Set_DIS_Interesting_Area(void);

void SetDIS_OnOff(WORD OnOff);
WORD GetDIS_OnOff(void);
void SetDIS_Wait(void);
void SetDIS_bayer_vect(WORD val);
WORD GetDIS_bayer_vect(void);
//void SetDIS_thres(BYTE val);
//BYTE GetDIS_thres(void);
void SetDIS_wait(void);

#endif	/* __MISP_DIS_H__ */

