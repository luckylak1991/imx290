/******************************************************************************
  * @project: LT8619C
  * @file: lt8619.c
  * @author: Yin Chao
  * @company: LONTIUM COPYRIGHT and CONFIDENTIAL
  * @date: 2016.11.10
******************************************************************************/

#include "video.h"
#include "osd.h"


#if defined(__USE_LT8619_RX__)

_LT8619C_RXStatus LT8619C_RXStatus, *pLT8619C_RXStatus;

extern ROMDATA BYTE RX1_EDID_Table[256];

ROMDATA BYTE ONCHIP_EDID[256] ={
#if 1
//TCL 4K2K TV
0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x50,0x6C,0x00,0x00,0x01,0x01,0x01,0x01,
0x18,0x17,0x01,0x03,0x80,0x00,0x00,0x78,0x0A,0xDA,0xFF,0xA3,0x58,0x4A,0xA2,0x29,
0x17,0x49,0x4B,0xA1,0x08,0x00,0x81,0x80,0xD1,0xC0,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x66,0x21,0x50,0xB0,0x51,0x00,0x1B,0x30,0x40,0x70,
0x36,0x00,0x44,0x40,0x21,0x00,0x00,0x18,0x02,0x3A,0x00,0x72,0x51,0xBE,0x1E,0x50,
0x6E,0x28,0x55,0x00,0xBA,0xA8,0x42,0x00,0x00,0x1E,0x00,0x00,0x00,0xFC,0x00,0x54,
0x43,0x4C,0x20,0x4D,0x54,0x33,0x36,0x4B,0x0A,0x20,0x20,0x20,0x00,0x00,0x00,0xFD,
0x00,0x16,0x48,0x0F,0x5A,0x14,0x00,0x0A,0x20,0x20,0x20,0x20,0x20,0x20,0x01,0x37,

0x02,0x03,0x2C,0xC2,0x51,0x1F,0x90,0x14,0x05,0x03,0x13,0x04,0x12,0x03,0x11,0x02,
0x21,0x22,0x15,0x06,0x01,0x20,0x23,0x09,0x07,0x01,0x71,0x03,0x0C,0x00,0x20,0x00,
0x00,0x3C,0x2F,0xC0,0x64,0x01,0x02,0x03,0x01,0x41,0x01,0x00,0x02,0x3A,0x00,0x72,
0x51,0xBE,0x1E,0x50,0x6E,0x28,0x55,0x00,0xBA,0xA8,0x42,0x00,0x00,0x9E,0x02,0x3A,
0x00,0xBC,0x52,0xBE,0x1E,0x50,0xB8,0x28,0x55,0x40,0xBA,0xA8,0x42,0x00,0x00,0x9E,
0x02,0x3A,0x80,0x3E,0x73,0x9D,0x2D,0x80,0x7E,0x2C,0x45,0x80,0xBA,0xA8,0x42,0x00,
0x00,0x1E,0x01,0x1D,0x00,0x72,0x51,0xD0,0x1E,0x20,0x6E,0x28,0x55,0x00,0xC4,0x8E,
0x21,0x00,0x00,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xAF

#elif 0

//4K2K(24/25/30Hz) 1080p 1080i 720p 480p
//RGB only
//no deep color
//LPCM 16-20-24bit 32-44.1-48-88.2-96-176.4-192kHz


0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x32,0x8D,0x02,0x2C,0x01,0x01,0x01,0x01,
0x01,0x15,0x01,0x03,0x80,0x59,0x32,0x78,0x0A,0x0D,0xC9,0xA0,0x57,0x47,0x98,0x27,
0x12,0x48,0x4C,0x20,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x3A,0x80,0x18,0x71,0x38,0x2D,0x40,0x58,0x2C,
0x45,0x00,0xA0,0x5A,0x00,0x00,0x00,0x1E,0x01,0x1D,0x00,0x72,0x51,0xD0,0x1E,0x20,
0x6E,0x28,0x55,0x00,0x76,0xF2,0x31,0x00,0x00,0x1E,0x00,0x00,0x00,0xFD,0x00,0x30,
0x3E,0x1E,0x46,0x0F,0x00,0x0A,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFC,
0x00,0x4C,0x4F,0x4E,0x54,0x49,0x55,0x4D,0x0A,0x20,0x20,0x20,0x20,0x20,0x01,0xF2,

0x02,0x03,0x27,0xC1,0x49,0x90,0x1F,0x05,0x04,0x13,0x14,0x03,0x02,0x01,0x23,0x09,
0x7F,0x07,0x83,0x01,0x00,0x00,0x67,0x03,0x0C,0x00,0x00,0x00,0x00,0x3C,0x20,0x00,
0x60,0x01,0x02,0x03,0xE2,0x00,0x0F,0x01,0x1D,0x80,0x18,0x71,0x1C,0x16,0x20,0x58,
0x2C,0x25,0x00,0xC4,0x8E,0x21,0x00,0x00,0x9E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x53

#else


#endif

};


//--------------------------------------------------------------------------------------------------------------------------
BYTE LT8619C_MultiWrite(BYTE rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Write(TWI_CH1, LT8619C_ADR, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes); 	// addr(8bit), data(8bit)
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE LT8619C_MultiRead(BYTE rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Read(TWI_CH1, LT8619C_ADR, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes); 	// addr(8bit), data(8bit)
}

//--------------------------------------------------------------------------------------------------------------------------
void LT8619C_RegWrite(BYTE rAddr, BYTE data)
{
	LT8619C_MultiWrite(rAddr, (PBYTE)&data, 1);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE LT8619C_RegRead(BYTE rAddr)
{
	BYTE data;
	LT8619C_MultiRead(rAddr, (PBYTE)&data, 1);
	return data;
}



void LT8619C_Reset(void)
{
    // GPIO_WriteLow(GPIOC,GPIO_PIN_3);
    // Delay_ms(100);
    // GPIO_WriteHigh(GPIOC,GPIO_PIN_3);
    // Delay_ms(50);
}

void LT8619C_SetHPD(BOOL level)
{
    LT8619C_RegWrite(0xFF,0x80);
    if( level )
    {
        LT8619C_RegWrite(0x06,LT8619C_RegRead(0x06)|0x08);
    }
    else
    {
        LT8619C_RegWrite(0x06,LT8619C_RegRead(0x06)&0xf7);
    }
}


void LT8619C_EDIDSet(void)
{
	LT8619C_RegWrite(0xFF,0x80); //set segment
	LT8619C_RegWrite(0x8E,0x03); //enable EDID shadow operaion
    LT8619C_RegWrite(0x8F, 0x00); //Base EDID offset
//h	LT8619C_MultiWrite(0x90, (PBYTE)&ONCHIP_EDID[0], 256);
	LT8619C_MultiWrite(0x90, (PBYTE)&RX1_EDID_Table[0], 256); //write EDID data
	LT8619C_RegWrite(0x8E,0x02); //disable EDID shadow operaion(?)
}


BOOL LT8619C_LoadHDCPKey(void)
{

    return TRUE;

}

void LT8619C_TXStatus( BOOL status )
{   
    if( status )
    {

    }
    else
    {

    }
}



void LT8619C_RXInit(void)
{
    LT8619C_RegWrite(0xFF,0x80);
    LT8619C_RegWrite(0x2c,LT8619C_RegRead(0x2c)|0x30);  //RGD_CLK_STABLE_OPT[1:0]
    LT8619C_RegWrite(0xFF,0x60); //set segment
    LT8619C_RegWrite(0x80,0x08);  //use xtal_clk as sys_clk. //sys_clk = x-tal (25MHz)
    
    if( LT8619C_OUTPUTMODE ==  OUTPUT_RGB888)
    {
    	LT8619C_RegWrite(0xFF,0x60); //set segment
        LT8619C_RegWrite(0x07,0xff); //enable CSC clock
    	//LT8619C_RegWrite(0xa4,0x08);
        LT8619C_RegWrite(0xa8,0x0f); //enable TTL enhanced driver
        LT8619C_RegWrite(0x52,0x00); 
        LT8619C_RegWrite(0x53,0x00);
        LT8619C_RegWrite(0x60,0x00);
        LT8619C_RegWrite(0x6d,0x07); // R&B data swap : 0 -> 7 //3x8bit output, output[23:0]=RGB
        LT8619C_RegWrite(0x96,0x71);
        LT8619C_RegWrite(0xa0,0x50);
        LT8619C_RegWrite(0xa3,0x44);
        //h LT8619C_RegWrite(0xa2,0x22);	// 0x20[default] -> 0x22[gradation]
        LT8619C_RegWrite(0xa4,0x10);
        LT8619C_RegWrite(0x0e,0xfd);
        LT8619C_RegWrite(0x0e,0xff);
    }
    else if(  LT8619C_OUTPUTMODE ==  OUTPUT_YCBCR422_20BIT )
    {
    	LT8619C_RegWrite(0xFF,0x60);
        LT8619C_RegWrite(0xa8,0x0f);
    	LT8619C_RegWrite(0x07,0xff);
        LT8619C_RegWrite(0x52,0x00);
        LT8619C_RegWrite(0x53,0x40);
        LT8619C_RegWrite(0x60,0x00);
        LT8619C_RegWrite(0x6d,0x40);
        LT8619C_RegWrite(0x96,0x71);
        LT8619C_RegWrite(0xa0,0x50);
        LT8619C_RegWrite(0xa3,0x44);
        LT8619C_RegWrite(0xa2,0x22);
        LT8619C_RegWrite(0xa4,0x10);
    }
    else if(  LT8619C_OUTPUTMODE ==  OUTPUT_BT1120_16BIT )
    {
    	LT8619C_RegWrite(0xFF,0x60);
        LT8619C_RegWrite(0xa8,0x0f);
    	LT8619C_RegWrite(0x07,0xff);
        LT8619C_RegWrite(0x52,0x0a);
        LT8619C_RegWrite(0x60,0x33);
        //timing related start
        //1080P test
        LT8619C_RegWrite(0x61,0x00);
        LT8619C_RegWrite(0x62,0xc0);
        LT8619C_RegWrite(0x63,0x07);
        LT8619C_RegWrite(0x64,0x80);
        LT8619C_RegWrite(0x65,0x08);
        LT8619C_RegWrite(0x66,0x98);
        LT8619C_RegWrite(0x67,0x29);
        LT8619C_RegWrite(0x69,0x04);
        LT8619C_RegWrite(0x6a,0x38);
        LT8619C_RegWrite(0x6b,0x04);
        LT8619C_RegWrite(0x6c,0x65);
        //timing related end.
        LT8619C_RegWrite(0x6d,0x08);//Y/C swap
        LT8619C_RegWrite(0x6e,0x06);
    }
    else if(  LT8619C_OUTPUTMODE ==  OUTPUT_LVDS_2_PORT )
    {
    	LT8619C_RegWrite(0xFF,0x60);
        LT8619C_RegWrite(0x06,0xe7);
    	LT8619C_RegWrite(0x59,0x60);
        LT8619C_RegWrite(0xa0,0x58);
        LT8619C_RegWrite(0xa4,0x01);
        LT8619C_RegWrite(0xa8,0x00);
        LT8619C_RegWrite(0xba,0x18);
        LT8619C_RegWrite(0xc0,0x18);
        LT8619C_RegWrite(0xb0,0x66);
        LT8619C_RegWrite(0xb1,0x66);
        LT8619C_RegWrite(0xb2,0x66);
        LT8619C_RegWrite(0xb3,0x66);
        LT8619C_RegWrite(0xb4,0x66);
        LT8619C_RegWrite(0xb5,0x41);
        LT8619C_RegWrite(0xb6,0x41);
        LT8619C_RegWrite(0xb7,0x41);
        LT8619C_RegWrite(0xb8,0x4c);
        LT8619C_RegWrite(0xb9,0x41);
        LT8619C_RegWrite(0xbb,0x41);
        LT8619C_RegWrite(0xbc,0x41);
        LT8619C_RegWrite(0xbd,0x41);
        LT8619C_RegWrite(0xbe,0x4c);
        LT8619C_RegWrite(0xbf,0x41);
        LT8619C_RegWrite(0xa0,0x18);
        LT8619C_RegWrite(0xa1,0xb0);
        LT8619C_RegWrite(0xa2,0x10);
        LT8619C_RegWrite(0x5c,0x01);
        
    }
    else if(  LT8619C_OUTPUTMODE ==  OUTPUT_LVDS_1_PORT )
    {
    	LT8619C_RegWrite(0xFF,0x60);
        LT8619C_RegWrite(0x06,0xe7);
    	LT8619C_RegWrite(0x59,0x60);
        LT8619C_RegWrite(0xa0,0x58);
        LT8619C_RegWrite(0xa4,0x00);
        LT8619C_RegWrite(0xa8,0x00);
        LT8619C_RegWrite(0xba,0x18);
        LT8619C_RegWrite(0xc0,0x18);
        LT8619C_RegWrite(0xb0,0x66);
        LT8619C_RegWrite(0xb1,0x66);
        LT8619C_RegWrite(0xb2,0x66);
        LT8619C_RegWrite(0xb3,0x66);
        LT8619C_RegWrite(0xb4,0x66);
        LT8619C_RegWrite(0xb5,0x41);
        LT8619C_RegWrite(0xb6,0x41);
        LT8619C_RegWrite(0xb7,0x41);
        LT8619C_RegWrite(0xb8,0x4c);
        LT8619C_RegWrite(0xb9,0x41);
        LT8619C_RegWrite(0xbb,0x41);
        LT8619C_RegWrite(0xbc,0x41);
        LT8619C_RegWrite(0xbd,0x41);
        LT8619C_RegWrite(0xbe,0x4c);
        LT8619C_RegWrite(0xbf,0x41);
        LT8619C_RegWrite(0xa0,0x18);
        LT8619C_RegWrite(0xa1,0xb0);
        LT8619C_RegWrite(0xa2,0x10);
        LT8619C_RegWrite(0x5c,0x00);
        
    }
    

}


void LT8619C_Init(void)
{
    // MCU_I2CMasterSwitchTo(IIC_TO_LT8619C);

    pLT8619C_RXStatus = &LT8619C_RXStatus;
    memset(pLT8619C_RXStatus, 0, sizeof(_LT8619C_RXStatus));
    LT8619C_Reset();
    LT8619C_SetHPD(LOW);
    Delay_mSec(500);
    LT8619C_EDIDSet();
    LT8619C_LoadHDCPKey();
    LT8619C_RXInit();
    LT8619C_SetHPD(HIGH);

    // MCU_I2CMasterSwitchTo(IIC_TO_LT8618EX);
}

BOOL LT8619C_ClkDet(void)
{
/*	
    BYTE read_data_1,read_data_2;
    
    LT8619C_RegWrite(0xFF,0x80);
    if( LT8619C_RegRead(0x44)&0x08 )
    {
        if(!pLT8619C_RXStatus->flag_RXClkStable)
        {
            pLT8619C_RXStatus->flag_RXClkStable = !pLT8619C_RXStatus->flag_RXClkStable;
            LT8619C_RegWrite(0xFF,0x60);
            read_data_1 = LT8619C_RegRead(0x0e);
            read_data_2 = LT8619C_RegRead(0x09);
            LT8619C_RegWrite(0x0e,read_data_1&0x87);
            LT8619C_RegWrite(0x09,read_data_2&0x8f);
            Delay_mSec(5);
            LT8619C_RegWrite(0x0e,read_data_1|0x40);
            Delay_mSec(5);
            LT8619C_RegWrite(0x09,read_data_2|0x70);
            LT8619C_RegWrite(0x0e,read_data_1|0x38);
            LT8619C_RegWrite(0x0e,0xfd);
            LT8619C_RegWrite(0x0e,0xff);
            pLT8619C_RXStatus->flag_RXPLLLocked = TRUE;
            
            return TRUE;
        }
        else
        {
            return TRUE;
        }
        
    }
    else
    {
        //pLT8619C_RXStatus->flag_RXClkStable = FALSE;
        //pLT8619C_RXStatus->flag_RXPLLLocked = FALSE;
        memset(pLT8619C_RXStatus, 0, sizeof(_LT8619C_RXStatus));
        return FALSE;
    }
*/
	
    static BYTE read_data_1 = 0 ;
    static BYTE read_data_2 = 0 ;
    static BYTE detphase = 0 ;
	
	if (detphase == 0)
	{
		LT8619C_RegWrite(0xFF,0x80);
		if( LT8619C_RegRead(0x44)&0x08 )
		{
			if(!pLT8619C_RXStatus->flag_RXClkStable) // RXClk stabilize start
			{
				LT8619C_RegWrite(0xFF,0x60);
				read_data_1 = LT8619C_RegRead(0x0e);
				read_data_2 = LT8619C_RegRead(0x09);
				LT8619C_RegWrite(0x0e,read_data_1&0x87);
				LT8619C_RegWrite(0x09,read_data_2&0x8f);	
				detphase = 1 ;
				return FALSE;
			}
			else
			{
				detphase = 0 ;
				// UARTprintf("flag_RXClkStable : %d / flag_RXPLLLocked : %d \r\n",pLT8619C_RXStatus->flag_RXClkStable,pLT8619C_RXStatus->flag_RXPLLLocked) ;
				return TRUE; // RXClk Stable
			}				
		}
		else
		{
			memset(pLT8619C_RXStatus, 0, sizeof(_LT8619C_RXStatus)); // all param reset	
			pLT8619C_RXStatus->input_videoindex = INDEX_NO_SIGNAL ; // Set no signal
			read_data_1 = 0 ;
			read_data_2 = 0 ;
			detphase = 0 ;
			// UARTprintf("flag_RXClkStable : %d / flag_RXPLLLocked : %d \r\n",pLT8619C_RXStatus->flag_RXClkStable,pLT8619C_RXStatus->flag_RXPLLLocked) ;
			return FALSE; // RXClk unStable			
		}
	}
	else if (detphase == 1) //  replace Delay_mSec(5); LT8619C_RegWrite on next loop
	{ 
		LT8619C_RegWrite(0x0e,read_data_1|0x40);
		detphase = 2 ;
		return FALSE;
	}
	else if (detphase == 2) // replace Delay_mSec(5); LT8619C_RegWrite on next loop
	{ 
		LT8619C_RegWrite(0x09,read_data_2|0x70);
		LT8619C_RegWrite(0x0e,read_data_1|0x38);
		LT8619C_RegWrite(0x0e,0xfd);
		LT8619C_RegWrite(0x0e,0xff);
		pLT8619C_RXStatus->flag_RXClkStable = TRUE;
		pLT8619C_RXStatus->flag_RXPLLLocked = TRUE;
		read_data_1 = 0 ;
		read_data_2 = 0 ;
		detphase = 0 ;	
		// UARTprintf("flag_RXClkStable : %d / flag_RXPLLLocked : %d \r\n",pLT8619C_RXStatus->flag_RXClkStable,pLT8619C_RXStatus->flag_RXPLLLocked) ;
		return TRUE;
	}
	else // detphase [Exception handle]
	{
		memset(pLT8619C_RXStatus, 0, sizeof(_LT8619C_RXStatus));				
		read_data_1 = 0 ;
		read_data_2 = 0 ;
		detphase = 0 ;
		return FALSE;				
	}	
}

int LT8619C_GetRxStat(BYTE *TMDS_clock, BYTE *H_sync, BYTE *HDMI_mode, BYTE *Audio)
{
	BYTE r;
	BYTE buff = 0x80 ;

	if(LT8619C_MultiWrite(0xFF,&buff,1)) return -1;	//set segment
	if(TMDS_clock) {
		r = LT8619C_RegRead(0x43) ;
		*TMDS_clock = r&(1<<7)? ON: OFF;
	}
	if(H_sync || HDMI_mode ||Audio) {
		r = LT8619C_RegRead(0x13) ;
		if(H_sync) *H_sync = r&(1<<0)? ON: OFF;
		if(HDMI_mode) *HDMI_mode = r&(1<<2)? ON: OFF;
		if(Audio) *Audio = r&(1<<4)? ON: OFF;
	}	

	return 0;
}

void LT8619C_GetInputInfo(void)
{
/*	
    BYTE loop_num,read_data;
    
	pLT8619C_RXStatus->flag_GetInputInfo = FALSE;
	
    LT8619C_RegWrite(0xFF,0x80); //set segment
    if( pLT8619C_RXStatus->flag_RXClkStable && pLT8619C_RXStatus->flag_RXPLLLocked)
    {
        if( LT8619C_RegRead(0x13)&0x01 ) // H_sync on/off
        {
            if( !pLT8619C_RXStatus->Flag_HsyncStable )
            {
                // pLT8619C_RXStatus->Flag_HsyncStable = TRUE;
                for(loop_num=0; loop_num<8; loop_num++)
                {
                    Delay_mSec(10);
                    if( !(LT8619C_RegRead(0x13)&0x01))
                    {
                        pLT8619C_RXStatus->Flag_HsyncStable = FALSE;
                        break;
                    }
                }
				
				pLT8619C_RXStatus->Flag_HsyncStable = TRUE;	//
				pLT8619C_RXStatus->flag_GetInputInfo = TRUE;	//
            }
        }
        else
        {
            pLT8619C_RXStatus->Flag_HsyncStable = FALSE;
        }
    }

    if( pLT8619C_RXStatus->flag_GetInputInfo && pLT8619C_RXStatus->Flag_HsyncStable )
    {
        read_data = LT8619C_RegRead(0x13);
        pLT8619C_RXStatus->input_hdmimode = (read_data&0x02)?(TRUE):(FALSE);
        if( pLT8619C_RXStatus->input_hdmimode )
        {
            pLT8619C_RXStatus->input_vic = LT8619C_RegRead(0x74)&0x7f;
            pLT8619C_RXStatus->input_colorspace = LT8619C_RegRead(0x71)&0x60;
            pLT8619C_RXStatus->input_colordepth = LT8619C_RegRead(0x16)&0xf0;
            pLT8619C_RXStatus->input_colorimetry = LT8619C_RegRead(0x72)&0xc0;
            pLT8619C_RXStatus->input_QuantRange = LT8619C_RegRead(0x73)&0x0c;
        }
        else
        {
            pLT8619C_RXStatus->input_vic = 0;
            pLT8619C_RXStatus->input_colorspace = COLOR_RGB;
            pLT8619C_RXStatus->input_colordepth = 0;
            pLT8619C_RXStatus->input_colorimetry = 0;
            pLT8619C_RXStatus->input_QuantRange = 0;
        }
    }
*/

	BYTE TMDS_clock=0, H_sync=0, HDMI_mode=0, Audio=0;
	LT8619C_GetRxStat(&TMDS_clock, &H_sync, &HDMI_mode, &Audio);
	// UARTprintf("TMDS_clock : %d / H_sync : %d / HDMI_mode : %d / Audio : %d \r\n",TMDS_clock,H_sync,HDMI_mode,Audio) ;

    if( TMDS_clock && (!pLT8619C_RXStatus->Flag_HsyncStable) && H_sync)
    {
		// UARTprintf("TMDS_clock : %d / H_sync : %d / HDMI_mode : %d / Audio : %d \r\n",TMDS_clock,H_sync,HDMI_mode,Audio) ;
        if( HDMI_mode )
        {
            pLT8619C_RXStatus->input_vic = LT8619C_RegRead(0x74)&0x7f;
            pLT8619C_RXStatus->input_colorspace = LT8619C_RegRead(0x71)&0x60;
            pLT8619C_RXStatus->input_colordepth = LT8619C_RegRead(0x16)&0xf0;
            pLT8619C_RXStatus->input_colorimetry = LT8619C_RegRead(0x72)&0xc0;
            pLT8619C_RXStatus->input_QuantRange = LT8619C_RegRead(0x73)&0x0c;
        }
        else
        {
            pLT8619C_RXStatus->input_vic = 0;
            pLT8619C_RXStatus->input_colorspace = COLOR_RGB;
            pLT8619C_RXStatus->input_colordepth = 0;
            pLT8619C_RXStatus->input_colorimetry = 0;
            pLT8619C_RXStatus->input_QuantRange = 0;
        }
    }		
	
	pLT8619C_RXStatus->Flag_HsyncStable = H_sync ;	//		
	pLT8619C_RXStatus->input_hdmimode = HDMI_mode ;		
	pLT8619C_RXStatus->flag_GetInputInfo = TMDS_clock;	

}


void LT8619C_CSCConversion(void)
{
	static _INPUT_COLORSPACE input_colorspace = COLOR_NULL;
	BYTE update_colorspace ;
	
    if( (pLT8619C_RXStatus->flag_GetInputInfo && pLT8619C_RXStatus->Flag_HsyncStable)&&(input_colorspace != pLT8619C_RXStatus->input_colorspace) ) 
	{
		update_colorspace = 1;
		input_colorspace = pLT8619C_RXStatus->input_colorspace ;
	}
	else 
	{
		update_colorspace = 0;
	}                                                         
				
    if( pLT8619C_RXStatus->flag_GetInputInfo && pLT8619C_RXStatus->Flag_HsyncStable && (update_colorspace == 1))
    {
        LT8619C_RegWrite(0xFF,0x60);
        LT8619C_RegWrite(0x07,0xfe);
        if( LT8619C_OUTPUTCOLOR == COLOR_RGB )
        {
            if( pLT8619C_RXStatus->input_colorspace == COLOR_RGB )
            {
                LT8619C_RegWrite(0x52,0x00);
                LT8619C_RegWrite(0x53,0x00);
            }
            else if( pLT8619C_RXStatus->input_colorspace == COLOR_YCBCR444 )
            {
                LT8619C_RegWrite(0x52,0x00);
                LT8619C_RegWrite(0x53,0x60);
            }
            else if( pLT8619C_RXStatus->input_colorspace == COLOR_YCBCR422 )
            {
                LT8619C_RegWrite(0x52,0x01);
                LT8619C_RegWrite(0x53,0x60);
            }
        }
        if( LT8619C_OUTPUTCOLOR == COLOR_YCBCR444 )
        {
            if( pLT8619C_RXStatus->input_colorspace == COLOR_RGB )
            {
                LT8619C_RegWrite(0x52,0x28);
                LT8619C_RegWrite(0x53,0x00);
            }
            else if( pLT8619C_RXStatus->input_colorspace == COLOR_YCBCR444 )
            {
                LT8619C_RegWrite(0x52,0x00);
                LT8619C_RegWrite(0x53,0x00);
            }
            else if( pLT8619C_RXStatus->input_colorspace == COLOR_YCBCR422 )
            {
                LT8619C_RegWrite(0x52,0x00);
                LT8619C_RegWrite(0x53,0x01);
            }
        }
        if( LT8619C_OUTPUTCOLOR == COLOR_YCBCR422 )
        {
            if( pLT8619C_RXStatus->input_colorspace == COLOR_RGB )
            {
                LT8619C_RegWrite(0x52,0x2a);
                LT8619C_RegWrite(0x53,0x00);
            }
            else if( pLT8619C_RXStatus->input_colorspace == COLOR_YCBCR444 )
            {
                LT8619C_RegWrite(0x52,0x02);
                LT8619C_RegWrite(0x53,0x00);
            }
            else if( pLT8619C_RXStatus->input_colorspace == COLOR_YCBCR422 )
            {
                LT8619C_RegWrite(0x52,0x00);
                LT8619C_RegWrite(0x53,0x00);
            }
        }
    }
}

//24Hz -- 0xfe546
//25Hz -- 0xf4280
//30Hz -- 0xcb76b
//50Hz -- 0x7a140
//60Hz -- 0x65bb5

#define Margin_24_25HZ    

void LT8619C_VideoCheck(void)
{
    WORD h_active, v_active, F_rate; 
	BYTE input_videoindex ;
    // DWORD frame_counter;
    if( !pLT8619C_RXStatus->Flag_HsyncStable )
    {
		pLT8619C_RXStatus->input_videoindex = INDEX_NO_SIGNAL ;
		// UARTprintf("INDEX_NO_SIGNAL : %d \r\n",pLT8619C_RXStatus->input_videoindex);
        return;
    }
    
    // LT8619C_RegWrite(0xFF,0x60);

    // h_active  = ((WORD)LT8619C_RegRead(0x22))<<8;
    // h_active += LT8619C_RegRead(0x23);
    // v_active  = ((WORD)(LT8619C_RegRead(0x20)&0x0f))<<8;
    // v_active += LT8619C_RegRead(0x21);
    // frame_counter  = ((DWORD)LT8619C_RegRead(0x10))<<16;
    // frame_counter += ((DWORD)LT8619C_RegRead(0x11))<<8;
    // frame_counter += LT8619C_RegRead(0x12);

	LT8619C_GetVideoInfo(&h_active,&v_active,&F_rate) ;

    if( (h_active==1280) && (v_active==720) )
    {
        // if( frame_counter>0x79000 && frame_counter<0x7c000 )
        // {
            // pLT8619C_RXStatus->input_videoindex = INDEX_1280x720_50P;
        // }
        // else if( frame_counter>0x63000 && frame_counter<0x68000 )
        // {
            // pLT8619C_RXStatus->input_videoindex = INDEX_1280x720_60P;
        // }
		
		switch(F_rate) {
			case 50:	input_videoindex = INDEX_1280x720_50P;	break;
			case 60:	input_videoindex = INDEX_1280x720_60P;	break;
			default:	input_videoindex = INDEX_UNSUPPORTED_FORMAT;	break;
		}			
    }
    else if( (h_active==1920) && (v_active==1080) )
    {
        // if( frame_counter>0x79000 && frame_counter<0x7c000 )
        // {
            // pLT8619C_RXStatus->input_videoindex = INDEX_1920x1080_50P;
        // }
        // else if( frame_counter>0x63000 && frame_counter<0x68000 )
        // {
            // pLT8619C_RXStatus->input_videoindex = INDEX_1920x1080_60P;
        // }
		
		switch(F_rate) {
			case 60:	input_videoindex = INDEX_1920x1080_60P;	break;
			case 50:	input_videoindex = INDEX_1920x1080_50P;	break;
			case 30:	input_videoindex = INDEX_1920x1080_30P;	break;
			case 25:	input_videoindex = INDEX_1920x1080_25P;	break;
			case 24:	input_videoindex = INDEX_1920x1080_24P;	break;
			default:	input_videoindex = INDEX_UNSUPPORTED_FORMAT;	break;
		}		
		
    }
    else if( (h_active==3840) && (v_active==2160) )
    {
        // if( frame_counter>0xf8000 && frame_counter<0xfff00 )
        // {
            // pLT8619C_RXStatus->input_videoindex = INDEX_3840x2160_24P;
        // }
        // else if( frame_counter>0xf0000 && frame_counter<0xf8000 )
        // {
            // pLT8619C_RXStatus->input_videoindex = INDEX_3840x2160_25P;
        // }
        // else if( frame_counter>0xc8000 && frame_counter<0xcf000 )
        // {
            // pLT8619C_RXStatus->input_videoindex = INDEX_3840x2160_30P;
        // }
		
		switch(F_rate) {
			case 30:	input_videoindex = INDEX_3840x2160_30P;	break;
			case 25:	input_videoindex = INDEX_3840x2160_25P;	break;
			case 24:	input_videoindex = INDEX_3840x2160_24P;	break;
			default:	input_videoindex = INDEX_UNSUPPORTED_FORMAT;	break;
		}			
    }
    else if( (h_active==4096) && (v_active==2160) )
    {
        // if( frame_counter>0xf8000 && frame_counter<0xfff00 )
        // {
            // pLT8619C_RXStatus->input_videoindex = INDEX_4096x2160_24P;
        // }
		
		switch(F_rate) {
			case 24:	input_videoindex = INDEX_4096x2160_24P;	break;
			default:	input_videoindex = INDEX_UNSUPPORTED_FORMAT;	break;
		}		
    }
    else
    {
        // pLT8619C_RXStatus->input_videoindex = INDEX_UNSUPPORTED_FORMAT;
       input_videoindex = INDEX_UNSUPPORTED_FORMAT;
    }
	
	pLT8619C_RXStatus->input_videoindex = input_videoindex ;
}

BYTE LT8619C_Getvideoindex(void)
{
	return pLT8619C_RXStatus->input_videoindex ;
}

void LT8619C_BT1120Setting(void)
{
    static Video_FormatIndex video_index = INDEX_NO_SIGNAL;
    
    if( !pLT8619C_RXStatus->Flag_HsyncStable )
    {
        video_index = INDEX_NO_SIGNAL;
        return;
    }

    LT8619C_RegWrite(0xFF,0x60);
    if( video_index != pLT8619C_RXStatus->input_videoindex )
    {
        video_index = pLT8619C_RXStatus->input_videoindex;
        if( video_index ==  INDEX_1280x720_50P)
        {
            LT8619C_RegWrite(0x61,0x00);
            LT8619C_RegWrite(0x62,0xc0);
            LT8619C_RegWrite(0x63,0x07);
            LT8619C_RegWrite(0x64,0x80);
            LT8619C_RegWrite(0x65,0x08);
            LT8619C_RegWrite(0x66,0x98);
            LT8619C_RegWrite(0x67,0x29);
            LT8619C_RegWrite(0x69,0x04);
            LT8619C_RegWrite(0x6a,0x38);
            LT8619C_RegWrite(0x6b,0x04);
            LT8619C_RegWrite(0x6c,0x65);
        }
        else if( video_index ==  INDEX_1280x720_60P)
        {
            LT8619C_RegWrite(0x61,0x00);
            LT8619C_RegWrite(0x62,0xc0);
            LT8619C_RegWrite(0x63,0x07);
            LT8619C_RegWrite(0x64,0x80);
            LT8619C_RegWrite(0x65,0x08);
            LT8619C_RegWrite(0x66,0x98);
            LT8619C_RegWrite(0x67,0x29);
            LT8619C_RegWrite(0x69,0x04);
            LT8619C_RegWrite(0x6a,0x38);
            LT8619C_RegWrite(0x6b,0x04);
            LT8619C_RegWrite(0x6c,0x65);
        }
        else if( video_index ==  INDEX_1920x1080_50P)
        {
            LT8619C_RegWrite(0x61,0x00);
            LT8619C_RegWrite(0x62,0xc0);
            LT8619C_RegWrite(0x63,0x07);
            LT8619C_RegWrite(0x64,0x80);
            LT8619C_RegWrite(0x65,0x08);
            LT8619C_RegWrite(0x66,0x98);
            LT8619C_RegWrite(0x67,0x29);
            LT8619C_RegWrite(0x69,0x04);
            LT8619C_RegWrite(0x6a,0x38);
            LT8619C_RegWrite(0x6b,0x04);
            LT8619C_RegWrite(0x6c,0x65);
        }
        else if( video_index ==  INDEX_1920x1080_60P)
        {
            LT8619C_RegWrite(0x61,0x00);
            LT8619C_RegWrite(0x62,0xc0);
            LT8619C_RegWrite(0x63,0x07);
            LT8619C_RegWrite(0x64,0x80);
            LT8619C_RegWrite(0x65,0x08);
            LT8619C_RegWrite(0x66,0x98);
            LT8619C_RegWrite(0x67,0x29);
            LT8619C_RegWrite(0x69,0x04);
            LT8619C_RegWrite(0x6a,0x38);
            LT8619C_RegWrite(0x6b,0x04);
            LT8619C_RegWrite(0x6c,0x65);
        }
        else if( video_index ==  INDEX_3840x2160_24P)
        {
            LT8619C_RegWrite(0x61,0x00);
            LT8619C_RegWrite(0x62,0xc0);
            LT8619C_RegWrite(0x63,0x07);
            LT8619C_RegWrite(0x64,0x80);
            LT8619C_RegWrite(0x65,0x08);
            LT8619C_RegWrite(0x66,0x98);
            LT8619C_RegWrite(0x67,0x29);
            LT8619C_RegWrite(0x69,0x04);
            LT8619C_RegWrite(0x6a,0x38);
            LT8619C_RegWrite(0x6b,0x04);
            LT8619C_RegWrite(0x6c,0x65);
        }
        else if( video_index ==  INDEX_3840x2160_25P)
        {
            LT8619C_RegWrite(0x61,0x00);
            LT8619C_RegWrite(0x62,0xc0);
            LT8619C_RegWrite(0x63,0x07);
            LT8619C_RegWrite(0x64,0x80);
            LT8619C_RegWrite(0x65,0x08);
            LT8619C_RegWrite(0x66,0x98);
            LT8619C_RegWrite(0x67,0x29);
            LT8619C_RegWrite(0x69,0x04);
            LT8619C_RegWrite(0x6a,0x38);
            LT8619C_RegWrite(0x6b,0x04);
            LT8619C_RegWrite(0x6c,0x65);
        }
        else if( video_index ==  INDEX_3840x2160_30P)
        {
            LT8619C_RegWrite(0x61,0x00);
            LT8619C_RegWrite(0x62,0xc0);
            LT8619C_RegWrite(0x63,0x07);
            LT8619C_RegWrite(0x64,0x80);
            LT8619C_RegWrite(0x65,0x08);
            LT8619C_RegWrite(0x66,0x98);
            LT8619C_RegWrite(0x67,0x29);
            LT8619C_RegWrite(0x69,0x04);
            LT8619C_RegWrite(0x6a,0x38);
            LT8619C_RegWrite(0x6b,0x04);
            LT8619C_RegWrite(0x6c,0x65);
        }
        else if( video_index ==  INDEX_4096x2160_24P)
        {
            LT8619C_RegWrite(0x61,0x00);
            LT8619C_RegWrite(0x62,0xc0);
            LT8619C_RegWrite(0x63,0x07);
            LT8619C_RegWrite(0x64,0x80);
            LT8619C_RegWrite(0x65,0x08);
            LT8619C_RegWrite(0x66,0x98);
            LT8619C_RegWrite(0x67,0x29);
            LT8619C_RegWrite(0x69,0x04);
            LT8619C_RegWrite(0x6a,0x38);
            LT8619C_RegWrite(0x6b,0x04);
            LT8619C_RegWrite(0x6c,0x65);
        }
    }

}

int LT8619C_GetVideoInfo(WORD *H_active, WORD *V_active, WORD *F_rate)
{
	BYTE buf[9];
	// BYTE H_adj, V_adj;
	BYTE buff = 0x80 ;

	const int sys_clk = 25000000;
	const int tick_55Hz = sys_clk/55;
	const int tick_40Hz = sys_clk/40;
	const int tick_27_5Hz = sys_clk/27.5;
	const int tick_24_5Hz = sys_clk/24.5;
	int i;	
	
	//read H/V sync adjustment setting
	if(LT8619C_MultiWrite(0xFF,&buff,1)) return -1;	//set segment

	//read video info
	LT8619C_RegWrite(0xFF, 0x60);	//set segment
	LT8619C_MultiRead( 0x20, buf, 4 );
	*V_active = MAKEWORD(buf[0],buf[1]);
	*H_active = MAKEWORD(buf[2],buf[3]);

	LT8619C_MultiRead(0x10, buf, 3);	//frame_time[23:0] in sys_clk
	i = ((DWORD)buf[0]<<16)|((DWORD)buf[1]<<8)|buf[2];
	if(i>tick_24_5Hz)	*F_rate = 24;
	else if(i>tick_27_5Hz)	*F_rate = 25;
	else if(i>tick_40Hz)	*F_rate = 30;
	else if(i>tick_55Hz)	*F_rate = 50;
	else *F_rate = 60;
	
	return 0 ;
}

void LT8619C_MainLoop(void)
{
	if(IsODMVsyncBlank()==TRUE)									return;		// skip func. when odm_vsync interrupted...
	
	if((LT8619C_RXStatus.ctrl & LT8619CCTRL_TRIGGER)==0)		return;		// tick : every 16ms
	LT8619C_RXStatus.ctrl &= (~LT8619CCTRL_TRIGGER);
	
	
    // MCU_I2CMasterSwitchTo(IIC_TO_LT8619C);

    if( LT8619C_ClkDet() )
    {
		LT8619C_GetInputInfo();
		LT8619C_CSCConversion();
		LT8619C_VideoCheck();
		LT8619C_BT1120Setting();
    }

    // LT8619C_GetInputInfo();
    // LT8619C_CSCConversion();
    // LT8619C_VideoCheck();
    // LT8619C_BT1120Setting();

    // MCU_I2CMasterSwitchTo(IIC_TO_LT8618EX);
}


void ISPM SetLT8619C_Tick(void)
{
	LT8619C_RXStatus.ctrl |= LT8619CCTRL_TRIGGER;
}

void ExtInput_Monitor(void)
{
	static BYTE vcount ;
	
	static Video_FormatIndex pre_video_index = INDEX_NO_SIGNAL ;
	Video_FormatIndex video_index = LT8619C_Getvideoindex() ;
	
	if ( pre_video_index != video_index )
	{
		vcount = 0 ;
		pre_video_index = video_index ;
	}
	else
	{
		if( vcount >= 30 )  vcount = 31 ;
		else				vcount++ ;
	}
	
	if ( pre_video_index == INDEX_NO_SIGNAL )
	{
		// if ( GetIN_MainPath() == MISP_MAINPATH_EXT_P1 ) 
		// if ( GetIN_AuxPath() == MISP_MAINPATH_EXT_P1 )
	}
	else
	{
		if ( vcount == 30 )
		{			
			switch(video_index)
			{
				case INDEX_1280x720_60P : SetIN_ExtPort1Frmt(MISP_VIDSRC_1280x720p60) ; SetMenuStatus(INFRMT_MENU,8); SetMenuRedraw(); break ;	
				case INDEX_1280x720_50P : SetIN_ExtPort1Frmt(MISP_VIDSRC_1280x720p50) ; SetMenuStatus(INFRMT_MENU,9); SetMenuRedraw(); break ;
				case INDEX_1920x1080_60P : SetIN_ExtPort1Frmt(MISP_VIDSRC_1920x1080p60) ; SetMenuStatus(INFRMT_MENU,15); SetMenuRedraw(); break ;				
				case INDEX_1920x1080_30P : SetIN_ExtPort1Frmt(MISP_VIDSRC_1920x1080p30) ; SetMenuStatus(INFRMT_MENU,17); SetMenuRedraw(); break ;
				case INDEX_1920x1080_50P : SetIN_ExtPort1Frmt(MISP_VIDSRC_1920x1080p60) ; SetMenuStatus(INFRMT_MENU,16); SetMenuRedraw(); break ;
				case INDEX_1920x1080_25P : SetIN_ExtPort1Frmt(MISP_VIDSRC_1920x1080p30) ; SetMenuStatus(INFRMT_MENU,18); SetMenuRedraw(); break ;
				case INDEX_3840x2160_30P : SetIN_ExtPort1Frmt(MISP_VIDSRC_3840x2160p30) ; SetMenuStatus(INFRMT_MENU,20); SetMenuRedraw(); break ;
				case INDEX_3840x2160_25P : 		
				case INDEX_3840x2160_24P : SetIN_ExtPort1Frmt(MISP_VIDSRC_3840x2160p25) ; SetMenuStatus(INFRMT_MENU,21); SetMenuRedraw(); break ;	
				default : break ;		
			}		
		}
	}
	
}




#endif	//__USE_LT8619_RX__

/*  FILE_END_HERE */
