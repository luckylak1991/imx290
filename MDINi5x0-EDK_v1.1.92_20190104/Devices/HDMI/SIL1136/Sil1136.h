/************************************************************
*	ProjectName:	 Sil1136
*	FileName:	     Sil1136.c
*	BuildData:	     2016-06-27
*	Version£º        V1.00
* 	Company:	     Silicon Image
* 	Made:	     	 Tommy Cho
************************************************************/

#ifndef		__SIL1136_H__
#define		__SIL1136_H__


#define   SIL1136_ADR            0x72
#define   SIL1136_ADR_last       0x7e

void SIL1136_Init(void);
BYTE SIL1136_MultiWrite(BYTE sAddr, BYTE rAddr, PBYTE pBuff, WORD bytes);
BYTE SIL1136_MultiRead(BYTE sAddr, BYTE rAddr, PBYTE pBuff, WORD bytes);
void SIL1136_SetOutMode(BYTE mode);


#endif
