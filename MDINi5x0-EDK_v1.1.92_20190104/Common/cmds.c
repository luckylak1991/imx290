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

#include "cmdtable.h"

static int cmd_exit(char *line);
static int cmd_show_help(char *c_ln);

char cbuf[BUFSIZE];

int cmd_test(char *line);
int cmd_lens_correction_test(char *line);
int cmd_dpdbuf_rw_test(char *line);

const CMDTBL cmd_tbl[] =
{
    {"test", cmd_test},
    {"lc", cmd_lens_correction_test},
    {"dprw", cmd_dpdbuf_rw_test},
    {"help", cmd_show_help},
    {"?",    cmd_show_help},
    {"exit", cmd_exit},
	{0, 0}
};

/*** user defined command functions *******************************************/

//extern void AR0331_reg_read(void);

int cmd_test(char *line)
{	
//	AR0331_reg_read();
	return 0;
}

int cmd_lens_correction_test(char *line)
{	
	int x,y;
	debugprintf("\nLens Correction Test\n");
	for(x=100; x<180; x+= 10){
		for(y=30; y<=(x-30); y+= 10){
			MISP_RegWrite(MISP_LOCAL_ID, 0x418, (U8)x*0x100|(U8)y);
			Delay_mSec(500);
		}
	}

	MISP_RegWrite(MISP_LOCAL_ID, 0x418, 120*0x100|60);

	debugprintf("\nChange intensity\n");
	for(x=0; x<=100; x++){
		MISP_RegWrite(MISP_LOCAL_ID, 0x419, (U8)x*0x100);
		Delay_mSec(100);
	}
	
	return 0;
}

int cmd_dpdbuf_rw_test(char *line)
{
	U16 addr, x, y;

	debugprintf("\nwriting...\n");
	for(addr=1; addr<1000; addr++)
	{
		MISP_RegWrite(MISP_LOCAL_ID, 0x42c, addr);  // x
		MISP_RegWrite(MISP_LOCAL_ID, 0x42d, addr);  // y
		MISP_RegWrite(MISP_LOCAL_ID, 0x42b, 0x8000|addr);  // wr+addr
		MISP_RegWrite(MISP_LOCAL_ID, 0x42b, addr);  // ~wr
		debugprintf("%04d: %04d,%04d\r",addr, addr, addr);
	}
	debugprintf("\n");

	MISP_RegWrite(MISP_LOCAL_ID, 0x428, 0);  // addr
	//Delay_mSec(500);
	debugprintf("\nreading...\n");

	for(addr=1; addr<1000; addr++)
	{
		MISP_RegWrite(MISP_LOCAL_ID, 0x428, addr);  // addr
		//Delay_mSec(500);
		MISP_RegRead(MISP_LOCAL_ID, 0x429, &x);  // x
		MISP_RegRead(MISP_LOCAL_ID, 0x42a, &y);  // y

		if((x!=addr)||(y!=addr))
		{
			debugprintf("%04d: %04d,%04d - Error",addr, x, y);
			break;
		}	
		debugprintf("%04d: %04d,%04d\r",addr, x, y);
	}
	debugprintf("\n");

	return 0;
}









/*** static functions *********************************************************/ 

static int cmd_exit(char *line)
{
	return -1;
}

static int cmd_show_help(char *line)
{
	int i=0;

	debugprintf("\n");
	while(cmd_tbl[i].cmd != NULL)
	{
		debugprintf("%s ", cmd_tbl[i].cmd);
		i++;
		if((i%7)==0)
			debugprintf("\n");
	}
	debugprintf("\n");

	return 0;
}

