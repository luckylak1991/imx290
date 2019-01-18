//---------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//---------------------------------------------------------------------------------------
//
// File Name   		:  cmdproc.c
// Description 		:  AES32000 version uart command processor
// Ref. Docment		:
// Revision History :
//

#include <stdio.h>
#include <string.h>
#include "cmdtable.h"
int cmd_loop(void);
static int run_cmd(char *cmd_line);


void uart_cmd_proc(void)
{
	char c;

	if(	uart_getch(1, &c) == TRUE)
	{
		if(c==27)
			cmd_loop();
	}
}

int scan(char *chr_ptr)
{
	char cnt = 0;

	*(--chr_ptr) = 1;		
	while (*chr_ptr++) /* return when *chr_ptr == '\0' */
	{
		while(uart_getch(1, chr_ptr)==FALSE);
		if (*chr_ptr == '\r'){ // enter key
			*chr_ptr = '\0';
		}
		else{
			uart_putch(1, *chr_ptr);
			cnt++;
			if(cnt >= BUFSIZE) 	*chr_ptr = '\0';
		}
	}
	return 1;
}

int cmd_loop(void)
{
	int ret = 0;
    char *cbuf_ptr;

	debugprintf("\n");
	for (;;)
    {
	    cbuf_ptr = cbuf;
        debugprintf ("$ ");
        *cbuf_ptr++ = '\0';
        *cbuf_ptr++ = '\0';
		scan(cbuf_ptr);
        ret = run_cmd(cbuf_ptr);
		if(ret == -1){
			debugprintf("\nExit command proc\n");
			break;
		}
	}
	return 0;
}


char *get_token(char *line, char *cmd)
{
    int i;

	/* skip blanks*/
    while ((*line == ' ') || (*line == '\t'))  
        ++line;

    for (i = 0; i < CMDLEN; i++)
    {
		/* break out of the loop when meet the end of line*/
        if ((*line == ' ') || (*line == '\n') || (*line == '\0') || (*line == '\r') )
            break;      
		/* get a byte*/
        *cmd++ = *line++;      
    }
	/* end of the string*/
    *cmd = '\0';

   	/* return pointer to the next char*/
    return(line);   
}

static int run_cmd(char *cmd_line)
{
	CMDTBL *p_tbl;
    char cmd[CMDLEN+1];
    char *token, *cmd_ptr;
    char *rem;

    rem = get_token(cmd_line, cmd);

    if (!cmd[0]) 
	{
		debugprintf ("\n");
		return 0;
	}

	p_tbl = (CMDTBL *)cmd_tbl;
    token = p_tbl->cmd;
	cmd_ptr = cmd;

    while (p_tbl->cmd) 
	{
        if (*token != *cmd_ptr) 
		{
			/* different command string, go to the next */
            if ((*cmd_ptr)) 
			{
                p_tbl++ ;
                token = p_tbl->cmd ;
				cmd_ptr = cmd;
            } 
			else
                return ((*p_tbl->cmdfunc)(rem));
        } 
		else 
		{
            if (!(*cmd_ptr))
                return ((*p_tbl->cmdfunc)(rem));
			/* same char, go to the next char */
            cmd_ptr++ ;
            token++ ;
        }
    }

    if (!p_tbl->cmd) 
        debugprintf ("\nInvalid Command! (See Help.)\n");
	else
        debugprintf ("\n");

	return 0;
}


