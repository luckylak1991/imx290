//---------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//---------------------------------------------------------------------------------------
//
// File Name   		:  
// Description 		:  
// Ref. Docment		:
// Revision History :
//
#ifndef		__CMDTABLE_H__
#define		__CMDTABLE_H__

#include "video.h"

#define BUFSIZE  32
#define CMDLEN   12   

typedef struct command 
{
    char *cmd ;              /* command string (+1 for NULL terminator) */
    int (*cmdfunc)(char *) ; /* pointer to command function */
}CMDTBL;

extern const CMDTBL cmd_tbl[];
extern char cbuf[];
#endif	/* __CMDTABLE_H__ */
