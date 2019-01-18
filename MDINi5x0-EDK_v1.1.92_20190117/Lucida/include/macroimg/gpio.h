/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author	:	Software Team.
 Title	:	GPIO Control
******************************************************************************/

#pragma once

//======================================================================================================================
// Port Direction Register (PxDIR)
#define R_PDIR(N)    ((volatile unsigned int*)(0x80011C00 + (0x40 * N)))
#define R_P0DIR		((volatile unsigned int*)0x80011C00)
#define R_P1DIR		((volatile unsigned int*)0x80011C40)
#define R_P2DIR		((volatile unsigned int*)0x80011C80)
#define R_P3DIR		((volatile unsigned int*)0x80011CC0)

// [ 8] RO Px.yOM : Px.y Output Data Mode Setting Bit
// 0 : Bit Mode
// 1 : Byte Mode
#define F_P0DIR_OM				( 1<< 8)

// [ 1 : 0] RO Px.yDIR : Px.y Direction Bit
// 0 : Input
// 1 : Output
#define F_P0DIR_DIR				( 1<< 0)
//======================================================================================================================

//======================================================================================================================
// Port Direction Output Mode Setting Register (PxODIR)
#define R_PODIR(N)    ((volatile unsigned int*)(0x80011C00 + (0x40 * N)))
#define R_P0ODIR	R_P0DIR
#define R_P1ODIR	R_P1DIR
#define R_P2ODIR	R_P2DIR
#define R_P3ODIR	R_P3DIR

// [ 8] WO Px.y Output Control By Byte Mode Setting Bit
// 0 : No Effect
// 1 : Set
#define F_P0ODIR_OCBM			( 1<< 8)

// [ 1] WO Px.1 Direction Output Mode Setting Bit
// 0 : No Effect
// 1 : Set
#define F_P0ODIR_DOMS_1			( 1<< 1)

// [ 0] WO Px.0 Direction Output Mode Setting Bit
// 0 : No Effect
// 1 : Set
#define F_P0ODIR_DOMS_0			( 1<< 0)
//======================================================================================================================

//======================================================================================================================
// Port Direction Input Mode Setting Register (PxIDIR)
#define R_PIDIR(N)    ((volatile unsigned int*)(0x80011C04 + (0x40 * N)))
#define R_P0IDIR	((volatile unsigned int*)0x80011C04)
#define R_P1IDIR	((volatile unsigned int*)0x80011C44)
#define R_P2IDIR	((volatile unsigned int*)0x80011C84)
#define R_P3IDIR	((volatile unsigned int*)0x80011CC4)

// [ 8] WO Px.y Output Control By Pin Mode Setting Bit
// 0 : No Effect
// 1 : Set
#define F_P0IDIR_OCPM			( 1<< 8)

// [ 1] WO Px.1 Direction Inpu Mode Setting Bit
// 0 : No Effect
// 1 : Set
#define F_P0IDIR_DIMS_1			( 1<< 1)

// [ 0] WO Px.0 Direction Inpu Mode Setting Bit
// 0 : No Effect
// 1 : Set
#define F_P0IDIR_DIMS_0			( 1<< 0)
//======================================================================================================================

//======================================================================================================================
// Port Output Data Level Register (PxOLEV)
#define R_POLEV(N)    ((volatile unsigned int*)(0x80011C08 + (0x40 * N)))
#define R_P0OLEV	((volatile unsigned int*)0x80011C08)
#define R_P1OLEV	((volatile unsigned int*)0x80011C48)
#define R_P2OLEV	((volatile unsigned int*)0x80011C88)
#define R_P3OLEV	((volatile unsigned int*)0x80011CC8)

// [ 1] RO Px.1 Output Level Bit
// 0 : Low Level
// 1 : High Level
#define F_P0OLEV_1				( 1<< 1)

// [ 0] RO Px.0 Output Level Bit
// 0 : Low Level
// 1 : High Level
#define F_P0OLEV_0				( 1<< 0)
//======================================================================================================================

//======================================================================================================================
// Port Output Data High Level Setting Register (PxOHIGH)
#define R_POHIGH(N)    ((volatile unsigned int*)(0x80011C08 + (0x40 * N)))
#define R_P0OHIGH	((volatile unsigned int*)0x80011C08)
#define R_P1OHIGH	((volatile unsigned int*)0x80011C48)
#define R_P2OHIGH	((volatile unsigned int*)0x80011C88)
#define R_P3OHIGH	((volatile unsigned int*)0x80011CC8)

// [ 1] WO
// if F_P0DIR_OM == 0 then 0 : No Effect 1 : Px.1 Output Data High Level Set
// if F_P0DIR_OM == 1 then Px.1 Output Data Level Setting Bit
#define F_P0OHIGH_OCPM_1		( 1<< 1)
#define F_P0OHIGH_OCBM_1_LOW	( 0<< 1)	// Output Low Level
#define F_P0OHIGH_OCBM_1_HIGH	( 1<< 1)	// Output High Level
#define F_P0OHIGH_OCBM_1(x)		( x<< 1)	// x = 0, 1

// [ 0] WO
// if F_P0DIR_OM == 0 then 0 : No Effect 1 : Px.0 Output Data High Level Set
// if F_P0DIR_OM == 1 then Px.0 Output Data Level Setting Bit
#define F_P0OHIGH_OCPM_0		( 1<< 0)
#define F_P0OHIGH_OCBM_0_LOW	( 0<< 0)	// Output Low Level
#define F_P0OHIGH_OCBM_0_HIGH	( 1<< 0)	// Output High Level
#define F_P0OHIGH_OCBM_0(x)		( x<< 0)	// x = 0, 1
//======================================================================================================================

//======================================================================================================================
// Port Output Data Low Level Setting Register (PxOLOW)
#define R_POLOW(N)    ((volatile unsigned int*)(0x80011C0C + (0x40 * N)))
#define R_P0OLOW	((volatile unsigned int*)0x80011C0C)
#define R_P1OLOW	((volatile unsigned int*)0x80011C4C)
#define R_P2OLOW	((volatile unsigned int*)0x80011C8C)
#define R_P3OLOW	((volatile unsigned int*)0x80011CCC)

// [ 1] WO
// 0 : No Effect
// 1 : Px.1 Output Data Low Level Set
#define F_P0OLOW_1_LOW			( 1<< 1)

// [ 0] WO
// 0 : No Effect
// 1 : Px.0 Output Data Low Level Set
#define F_P0OLOW_0_LOW			( 1<< 0)
//======================================================================================================================

//======================================================================================================================
// Port Input Data Level Register (PxILEV)
#define R_PILEV(N)    ((volatile unsigned int*)(0x80011C10 + (0x40 * N)))
#define R_P0ILEV	((volatile unsigned int*)0x80011C10)
#define R_P1ILEV	((volatile unsigned int*)0x80011C50)
#define R_P2ILEV	((volatile unsigned int*)0x80011C90)
#define R_P3ILEV	((volatile unsigned int*)0x80011CD0)

// [ 1] RO Px.1 Input Level Bit
// 0 : Low Level
// 1 : High Level
#define F_P0ILEV_1				( 1<< 1)

// [ 0] RO Px.0 Input Level Bit
// 0 : Low Level
// 1 : High Level
#define F_P0ILEV_0				( 1<< 0)
//======================================================================================================================

//======================================================================================================================
// Port Pull-up Status Register (PxPUS)
#define R_PPUS(N)    ((volatile unsigned int*)(0x80011C18 + (0x40 * N)))
#define R_P0PUS		((volatile unsigned int*)0x80011C18)
#define R_P1PUS		((volatile unsigned int*)0x80011C58)
#define R_P2PUS		((volatile unsigned int*)0x80011C98)
#define R_P3PUS		((volatile unsigned int*)0x80011CD8)

// [ 1] RO Px.1 Pull-up Status Bit
// 0 : Pull-up Disable
// 1 : Pull-up Enable
#define F_P0PUS_1				( 1<< 1)

// [ 0] RO Px.0 Pull-up Status Bit
// 0 : Pull-up Disable
// 1 : Pull-up Enable
#define F_P0PUS_0				( 1<< 0)
//======================================================================================================================

//======================================================================================================================
// Port Pull-up Enable Register (PxPUEN)
#define R_PPUEN(N)    ((volatile unsigned int*)(0x80011C18 + (0x40 * N)))
#define R_P0PUEN	((volatile unsigned int*)0x80011C18)
#define R_P1PUEN	((volatile unsigned int*)0x80011C58)
#define R_P2PUEN	((volatile unsigned int*)0x80011C98)
#define R_P3PUEN	((volatile unsigned int*)0x80011CD8)

// [ 1] WO Px.1 Pull-up Enable Bit
// 0 : No Effect
// 1 : Pull-up Enable
#define F_P0PUEN_1				( 1<< 1)

// [ 1] WO Px.0 Pull-up Enable Bit
// 0 : No Effect
// 1 : Pull-up Enable
#define F_P0PUEN_0				( 1<< 0)
//======================================================================================================================

//======================================================================================================================
// Port Pull-up Disable Register (PxPUDIS)
#define R_PPUDIS(N)    ((volatile unsigned int*)(0x80011C1C + (0x40 * N)))
#define R_P0PUDIS	((volatile unsigned int*)0x80011C1C)
#define R_P1PUDIS	((volatile unsigned int*)0x80011C5C)
#define R_P2PUDIS	((volatile unsigned int*)0x80011C9C)
#define R_P3PUDIS	((volatile unsigned int*)0x80011CDC)

// [ 1] WO Px.1 Pull-up Disable Bit
// 0 : No Effect
// 1 : Pull-up Disable
#define F_P0PUDIS_1				( 1<< 1)

// [ 1] WO Px.1 Pull-up Disable Bit
// 0 : No Effect
// 1 : Pull-up Disable
#define F_P0PUDIS_0				( 1<< 0)
//======================================================================================================================

//======================================================================================================================
// Port Rising Edge Detect Register (PxRED)
#define R_PRED(N)    ((volatile unsigned int*)(0x80011C20 + (0x40 * N)))
#define R_P0RED		((volatile unsigned int*)0x80011C20)
#define R_P1RED		((volatile unsigned int*)0x80011C60)
#define R_P2RED		((volatile unsigned int*)0x80011CA0)
#define R_P3RED		((volatile unsigned int*)0x80011CE0)

// [ 1] R/W Px.1 Rising Edge Detect Bit
// 0 : Disable
// 1 : Enable
#define F_P0RED_1_DIS			( 0<< 1)
#define F_P0RED_1_EN			( 1<< 1)

// [ 1] R/W Px.0 Rising Edge Detect Bit
// 0 : Disable
// 1 : Enable
#define F_P0RED_0_DIS			( 0<< 0)
#define F_P0RED_0_EN			( 1<< 0)
//======================================================================================================================

//======================================================================================================================
// Port Falling Edge Detect Register (PxFED)
#define R_PFED(N)    ((volatile unsigned int*)(0x80011C24 + (0x40 * N)))
#define R_P0FED		((volatile unsigned int*)0x80011C24)
#define R_P1FED		((volatile unsigned int*)0x80011C64)
#define R_P2FED		((volatile unsigned int*)0x80011CA4)
#define R_P3FED		((volatile unsigned int*)0x80011CE4)

// [ 1] R/W Px.1 Falling Edge Detect Bit
// 0 : Disable
// 1 : Enable
#define F_P0FED_1_DIS			( 0<< 1)
#define F_P0FED_1_EN			( 1<< 1)

// [ 1] R/W Px.0 Falling Edge Detect Bit
// 0 : Disable
// 1 : Enable
#define F_P0FED_0_DIS			( 0<< 0)
#define F_P0FED_0_EN			( 1<< 0)
//======================================================================================================================

//======================================================================================================================
// Port Edge Detect Status Register (PxEDS)
#define R_PEDS(N)    ((volatile unsigned int*)(0x80011C28 + (0x40 * N)))
#define R_P0EDS		((volatile unsigned int*)0x80011C28)
#define R_P1EDS		((volatile unsigned int*)0x80011C68)
#define R_P2EDS		((volatile unsigned int*)0x80011CA8)
#define R_P3EDS		((volatile unsigned int*)0x80011CE8)

// [ 1] R/W Px.1 Edge Detect Satus Bit
// 0 : No Effect
// 1 : Edge Detect Has Occurred On Pin / Clear
#define F_P0EDS_1				( 1<< 1)
#define F_P0EDS_1_CLR			( 1<< 1)


// [ 1] R/W Px.0 Edge Detect Satus Bit
// 0 : No Effect
// 1 : Edge Detect Has Occurred On Pin / Clear
#define F_P0EDS_0				( 1<< 0)
#define F_P0EDS_0_CLR			( 1<< 0)
//======================================================================================================================

//======================================================================================================================
// Port Open Drain Mode Control Register (PxODM)
#define R_PODM(N)    ((volatile unsigned int*)(0x80011C2C + (0x40 * N)))
#define R_P0ODM		((volatile unsigned int*)0x80011C2C)
#define R_P1ODM		((volatile unsigned int*)0x80011C6C)
#define R_P2ODM		((volatile unsigned int*)0x80011CAC)
#define R_P3ODM		((volatile unsigned int*)0x80011CEC)
//======================================================================================================================

