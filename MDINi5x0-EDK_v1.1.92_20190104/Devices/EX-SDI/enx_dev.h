/* **************************************************************************
 File Name		: 	enx_dev.h
 Description	:	EN332S device function define
 Designer		:	
 Date			:	Sep. 15. 2016
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#ifndef		__ENXDEV_H__
#define		__ENXDEV_H__


//extern	BYTE FixMode;

// ==================================================================================
// SDI mode define
// ==================================================================================
#define RESET			0
#define NO_INPUT		1
#define IN_HDSDI		2
#define IN_3GSDI		3
#define IN_EX270_HD		4
#define IN_EX270_3G		5
#define IN_EX135_HD		6
#define IN_EX1G5_4K		7
#define IN_EX1G5_TDM_RX	8
#define IN_EX1G5_TDM_IC	9


// enx specific define for compile error
#define UINT                DWORD
#define WaitUs(x)           Delay_uSec(x)
#define TxStrNoIRQ(str,a,b)
#define KEY_L   0
#define KEY_R   0
#define KEY_U   0
#define KEY_D   0
#define KEY_C   0

#define ENX_DEV0_ADDR   0x84	// IRQ_A/B/C/D = 0x0
#define ENX_DEV1_ADDR   0x8c	// IRQ_A/B/C/D = 0x1


// ==================================================================================
// external function define
// ==================================================================================
void EN332S_Init(WORD DevAddr);
//void EN332STask(WORD DevAddr);
void EN332STask(WORD DevAddr, BYTE mode, BYTE frmt);
void EN332x_Initialize(void);
void SetEN332T_Frmt(void);
//extern void Mode_chk(BYTE *ModeTrig);

//extern BYTE ENX_I2Cw(WORD DevAddr, WORD RegAddr, UINT RegData);
//extern BYTE ENX_I2Cr(WORD DevAddr, WORD RegAddr, UINT* RegData);
//extern void ENX_MSK_I2Cw(WORD DevAddr, UINT *EN332S_TBL,BYTE size);
//extern void ENX_TBL_I2Cw(WORD DevAddr, UINT *EN332S_TBL,BYTE size);

void EN332SuccRx(WORD DevAddr);
void EN332SKeyComm (WORD DevAddr);
void EN332x_ExeUccRx(PBYTE pBuff);


#endif	/* __ENXDEV_H__ */
