/*****************************************************************************
*
* Copyright (C) 2014      Advanced Digital Chips, Inc. All Rights Reserved.
*						http://www.adc.co.kr
*
* THIS SOFTWARE IS PROVIDED BY ADCHIPS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ADCHIPS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE
*
*****************************************************************************/
#include "macroimg.h"

int twi_set_freq (U8 ch, U32 freq)
{
    unsigned long long tmp;
    U32 twibr0_cnt, twibr1_cnt;

    tmp = (unsigned long long)get_apb_clock()*700/1000000000;    
    twibr0_cnt = tmp + 3;
    twibr1_cnt = (get_apb_clock()/(2*freq)) - ((twibr0_cnt+7) >> 1);
	
	*R_TWIBR0(ch) = twibr0_cnt;
	*R_TWIBR1(ch) = twibr1_cnt;

    return 0;
}

BOOL twi_trans_complete(U8 ch, U8 devaddr)  //bh 2015.09.23
{
	U32 status;
	int cnt = TWI_RESP_TIME_OUT_COUNT;

	while( cnt-- )
	{
    status = *R_TWISTAT(ch);
    if (status & F_TWISTAT_TWIDT_DONE)   return TRUE;  // Data Transfer Done

		if (status&F_TWISTAT_TWILOST_OCCR)      // Lost Arbitration
		{
			debugprintf("Lost Arbitration!!, ch[%d], devaddr[0x%02x]\r\n", ch, devaddr);
			*R_TWICTRL(ch) = 0;
			*R_TWISTAT(ch) |= F_TWISTAT_TWILOST_OCCR;   // Clear Lost Arbitration(write '1')
			return FALSE;
		}
	}   
	debugprintf("twi_trans_complete() time out!, ch[%d], devaddr[0x%02x]\r\n", ch, devaddr);
	*R_TWISTAT(ch) |= F_TWISTAT_TWIDT_DONE;   // Clear Data Transfer Done(write '1')
	return FALSE;
}

BOOL twi_wait_busy(U8 ch, U8 devaddr) //bh 2015.09.23
{
	U32 status;
	int cnt = TWI_RESP_TIME_OUT_COUNT;

	while( cnt-- )
	{
	  status = *R_TWISTAT(ch);
	  if ((status & F_TWISTAT_TWIBUSY_BUSY)==0)   return TRUE;  // busy check

		if (status&F_TWISTAT_TWILOST_OCCR)      // Lost Arbitration
		{
			debugprintf("Lost Arbitration!!, ch[%d], devaddr[0x%02x]\r\n", ch, devaddr);
			*R_TWICTRL(ch) = 0;
			*R_TWISTAT(ch) |= F_TWISTAT_TWILOST_OCCR;   // Clear Lost Arbitration(write '1')
			return FALSE;
		}
	}
	debugprintf( "twi_wait_busy() time out!, ch[%d], devaddr[0x%02x]\r\n", ch, devaddr);
	*R_TWICTRL(ch) = 0;                            // Stop twi
	*R_TWISTAT(ch) &= (~F_TWISTAT_TWIBUSY_BUSY);   // Clear busy status(write '0')
	return FALSE;

}

/*
 * devaddr : Base of DEVICE ADDRESS
 * waddr : Buffer of Word Address
 * len : length of 'waddr', 1 or 2
 * flags : TWI_TRANSMIT, TWI_RECEIVE
 */
void twi_start (U8 ch, U8 devaddr, U8 *waddr, U32 len, U32 flags)
{
    int i;
    U8 con = 0;
    //U16 status;

    con = F_TWICTRL_TWIEN_EN | F_TWICTRL_TWIMOD_MASTER | F_TWICTRL_TWITR_TX;

	*R_TWIDAT(ch) = devaddr;
	*R_TWICTRL(ch) = con;
    
    if(twi_trans_complete(ch, devaddr) == FALSE)   return; //bh 2015.09.23

    for (i = 0; i < len; i++)
	{
		*R_TWIDAT(ch) = waddr[i];
        if(twi_trans_complete(ch, devaddr) == FALSE)   return; //bh 2015.09.23
    } 

    if (flags == F_TWI_RECEIVE)
	{
        con = F_TWICTRL_TWIEN_EN | F_TWICTRL_TWIMOD_MASTER | F_TWICTRL_REPST;
		*R_TWICTRL(ch) = con;
		*R_TWIDAT(ch) = devaddr+1;
        if(twi_trans_complete(ch, devaddr) == FALSE)   return; //bh 2015.09.23
		
        //status  = *R_TWIDAT(ch);     // Read for SCL Generation -- disabled by hungry 2017.03.24
        //if(twi_trans_complete(ch) == FALSE)   return; //bh 2015.09.23 -- disabled by hungry 2017.03.24
    }
}

/*
 * devaddr : Base of DEVICE ADDRESS
 * waddr : Buffer of Word Address
 * len : length of 'waddr', 1 or 2
 * buffer : Writing buffer
 * length : Size of Writing
 */
int twi_write(U8 ch, U8 devaddr, U8 *waddr, U32 len, U8 *buffer, U32 length)
{
    int write_num, i;

    if(length <= 0)
	{
        return -1;
	}

    write_num = 0;

    twi_start(ch, devaddr, waddr, len, F_TWI_TRANSMIT);

    for (i = 0; i < length; i++)
	{
		*R_TWIDAT(ch) = buffer[i];	// When word is 8bit, use this
        if(twi_trans_complete(ch, devaddr) == FALSE)   return -1; //bh 2015.09.23
        write_num++;
    }

	*R_TWICTRL(ch) = F_TWICTRL_TWIEN_EN;
    if(twi_wait_busy(ch, devaddr) == FALSE)  return -1;  //bh 2015.09.23

    return write_num;
}

/*
 * devaddr : Base of DEVICE ADDRESS
 * addr : Word Address
 * len : length of Word Address (1 or 2)
 * buffer : read buffer
 * length : length of buffer
 */
int twi_read(U8 ch, U8 devaddr, U8 *waddr, U32 len, U8 *buffer, U32 length)
{
    U32 i;
	U8	tmp;
    int read_cnt = 0;

    twi_start (ch, devaddr, waddr, len, F_TWI_RECEIVE);

    for ( i = 0; i < length; i++ )
	{
		if(i==length-1)	{
			*R_TWICTRL(ch) = F_TWICTRL_TWIEN_EN | F_TWICTRL_TWIMOD_MASTER | F_TWICTRL_TWIAK_NACK;	// last byte with 'nack'
		}
		
		if(i==0)	tmp = *R_TWIDAT(ch);							// dummy read for 1st SCL generation
		else		{buffer[i-1] = *R_TWIDAT(ch);	read_cnt++;}
		
        if(twi_trans_complete(ch, devaddr) == FALSE)   return -1; //bh 2015.09.23
		
		if(i==length-1)	{
			buffer[i] = *R_TWIDAT(ch);	read_cnt++;		// get last byte from buffer after transfer terminated..
		}
    }

	*R_TWICTRL(ch) = F_TWICTRL_TWIEN_EN;
    if(twi_wait_busy(ch, devaddr) == FALSE)  return -1;  //bh 2015.09.23
    
	return read_cnt;
}

