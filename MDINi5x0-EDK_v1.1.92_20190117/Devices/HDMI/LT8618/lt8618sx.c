/******************************************************************************
  * @project: LT8618SX
  * @file: lt8618sx.c
  * @author: xhguo
  * @company: LONTIUM COPYRIGHT and CONFIDENTIAL
  * @date: 2017.07.25
******************************************************************************/

#include   "video.h"

#if defined(__USE_LT8618SX_TX__)

#define     Check_Ycbcr444      0x04
#define     Check_RGB888        0x44
#define     Out_Ycbcr444        0x50
#define     out_RGB888          0x10
#define     Color_Ycbcr444      0x02
#define     Color_RGB888        0x00
#define     Color_Ycbcr422      0x12


#define		bt656				0
#define		HDMI_WriteI2C_Byte(rAddr, wData)	I2C_ByteWrite(TWI_CH0, LT8618SX_ADR, rAddr, wData)
#define		HDMI_ReadI2C_Byte(rAddr)			I2C_ByteRead(TWI_CH0, LT8618SX_ADR, rAddr)
#define		Debug_DispStrNum(str,val)			{UARTprintf(str);	UARTprintf("0x%02x", val);}
#define		Debug_DispNum(val)					UARTprintf("0x%02x", val)
#define		Debug_Printf(str)					UARTprintf(str)

											 // hfp, hs, hbp,hact,htotal,vfp, vs, vbp,vact,vtotal,
ROMDATA video_timing video_640x480_60Hz     ={ 8, 96,  40, 640,   800, 33,  2,  10, 480,   525};
ROMDATA video_timing video_720x480_60Hz     ={16, 62,  60, 720,   858,  9,  6,  30, 480,   525};
ROMDATA video_timing video_720x576_50Hz     ={16, 62,  60, 720,   858,  9,  6,  30, 480,   525};
ROMDATA video_timing video_1280x720_60Hz    ={110,40, 220,1280,  1650,  5,  5,  20, 720,   750};
ROMDATA video_timing video_1920x1080_60Hz   ={88, 44, 148,1920,  2200,  4,  5,  36, 1080, 1125};
ROMDATA video_timing video_3840x2160_30Hz   ={88, 44, 148,1920,  2200,  4,  5,  36, 1080, 1125};


u8 Sink_EDID[256];
u16 hfp,hs_width,hbp,h_act,h_tal,vfp,vs_width,vbp,v_act,v_tal;
bool hs_pol,vs_pol;

bool Tx_HPD=0;
bool Tx_AC_DET=0;
bool Tx_DC_DET=0;
u8 HDMI_VIC=0x00;
u8 HDMI_Y=0x00;
VideoFormat Video_Format;
u8 tmp3;
u8 csc_bt709;

void LT8618SX_Chip_ID(void)
{
	// MCU_I2CMasterSwitchTo(IIC_TO_LT8618SX);
	 HDMI_WriteI2C_Byte(0xFF,0x80);
	 HDMI_WriteI2C_Byte(0xee,0x01);
   Debug_DispStrNum("\r\nLT8618SX Chip ID = ",HDMI_ReadI2C_Byte(0x00));
	 Debug_DispStrNum(" ",HDMI_ReadI2C_Byte(0x01));
	 Debug_DispStrNum(" ",HDMI_ReadI2C_Byte(0x02));
}


void LT8618SX_RST_PD_Init(void)
{
	HDMI_WriteI2C_Byte(0xff,0x80);
	HDMI_WriteI2C_Byte(0x11,0x00); //reset MIPI Rx logic.
}

void LT8618SX_TTL_Input_Analog(void)//xuxi
{
	//TTL mode
	HDMI_WriteI2C_Byte(0xff,0x81);
	HDMI_WriteI2C_Byte(0x02,0x66);
	HDMI_WriteI2C_Byte(0x0a,0x06);
	HDMI_WriteI2C_Byte(0x15,0x06);
}

void LT8618SX_TTL_Input_Digtal(void) //dsren
{
	if(bt656==1)
	{
       	Debug_Printf("\r\nLT8618SX_Input_Mode=LT8618SX_Input_BT656_8BIT");
		HDMI_WriteI2C_Byte(0xff,0x82);
		HDMI_WriteI2C_Byte(0x45,0x00); //RGB channel swap
		HDMI_WriteI2C_Byte(0x4f,0x00); //0x00;  0x40: dclk 
		HDMI_WriteI2C_Byte(0x50,0x00);
		HDMI_WriteI2C_Byte(0x48,0x48); //48Embedded sync mode input enable.
    }
	else{
		//TTL mode
		if (LT8618SX_Input_Mode==LT8618SX_Input_RGB888)
		{
			Debug_Printf("\r\nLT8618SX_Input_Mode=LT8618SX_Input_RGB888");
			HDMI_WriteI2C_Byte(0xff,0x82);
			HDMI_WriteI2C_Byte(0x45,0x70); //RGB channel swap
			HDMI_WriteI2C_Byte(0x4f,0x40); //0x00;  0x40: dclk 
			HDMI_WriteI2C_Byte(0x50,0x00);
		}
		else if (LT8618SX_Input_Mode==LT8618SX_Input_YCbCr444)
		{
			Debug_Printf("\r\nLT8618SX_Input_Mode=LT8618SX_Input_YCbCr444");
			HDMI_WriteI2C_Byte(0xff,0x82);
			HDMI_WriteI2C_Byte(0x45,0x70); //RGB channel swap
			HDMI_WriteI2C_Byte(0x4f,0x40); //0x00;  0x40: dclk 
			HDMI_WriteI2C_Byte(0x50,0x00);
		}
		else if (LT8618SX_Input_Mode==LT8618SX_Input_YCbCr422_16BIT)
		{
			Debug_Printf("\r\nLT8618SX_Input_Mode=LT8618SX_Input_YCbCr422_16BIT");
			HDMI_WriteI2C_Byte(0xff,0x82);
			HDMI_WriteI2C_Byte(0x45,0x00); //RGB channel swap
			HDMI_WriteI2C_Byte(0x4f,0x40); //0x00;  0x40: dclk 
			HDMI_WriteI2C_Byte(0x50,0x00);
		}
		else if (LT8618SX_Input_Mode==LT8618SX_Input_BT1120_16BIT)
		{
			Debug_Printf("\r\nLT8618SX_Input_Mode=LT8618SX_Input_BT1120_16BIT");
			HDMI_WriteI2C_Byte(0xff,0x82);
			HDMI_WriteI2C_Byte(0x45,0x70); //RGB channel swap
			HDMI_WriteI2C_Byte(0x4f,0x40); //0x00;  0x40: dclk 
			HDMI_WriteI2C_Byte(0x50,0x00);
			HDMI_WriteI2C_Byte(0x48,0x08); //Embedded sync mode input enable.
		}
		else if (LT8618SX_Input_Mode==LT8618SX_Input_BT656_8BIT)
		{
			Debug_Printf("\r\nLT8618SX_Input_Mode=LT8618SX_Input_BT656_8BIT");
			HDMI_WriteI2C_Byte(0xff,0x82);
			HDMI_WriteI2C_Byte(0x45,0x00); //RGB channel swap
			HDMI_WriteI2C_Byte(0x4f,0x00); //0x00;  0x40: dclk 
			HDMI_WriteI2C_Byte(0x50,0x00);
			HDMI_WriteI2C_Byte(0x48,0x48); //48Embedded sync mode input enable.
		}	
	}
}

void LT8618SX_Video_Check(void)
{
	u8 temp;
	
	Delay_mSec(100);
	#if 0//(LT8618SX_Input_Mode==LT8618SX_Input_BT656_8BIT)
	{
	
	}
	#else
	{
			HDMI_WriteI2C_Byte(0xff,0x82); //video check
			temp=HDMI_ReadI2C_Byte(0x70);  //hs vs polarity
			if(temp&0x02)
			vs_pol=1;
			if(temp&0x01)
			hs_pol=1;
			
			vs_width=HDMI_ReadI2C_Byte(0x71);
			hs_width=HDMI_ReadI2C_Byte(0x72);
			hs_width=(hs_width<<8)+HDMI_ReadI2C_Byte(0x73);
			vbp=HDMI_ReadI2C_Byte(0x74);
			vfp=HDMI_ReadI2C_Byte(0x75);
			hbp=HDMI_ReadI2C_Byte(0x76);
			hbp=(hbp<<8)+HDMI_ReadI2C_Byte(0x77);
			hfp=HDMI_ReadI2C_Byte(0x78);
			hfp=(hfp<<8)+HDMI_ReadI2C_Byte(0x79);
			v_tal=HDMI_ReadI2C_Byte(0x7a);
			v_tal=(v_tal<<8)+HDMI_ReadI2C_Byte(0x7b);
			h_tal=HDMI_ReadI2C_Byte(0x7c);
			h_tal=(h_tal<<8)+HDMI_ReadI2C_Byte(0x7d);
			v_act=HDMI_ReadI2C_Byte(0x7e);
			v_act=(v_act<<8)+HDMI_ReadI2C_Byte(0x7f);
			h_act=HDMI_ReadI2C_Byte(0x80);
			h_act=(h_act<<8)+HDMI_ReadI2C_Byte(0x81);//
  }
	
	#endif

	if((h_act==video_640x480_60Hz.hact)&&
		 (v_act==video_640x480_60Hz.vact)&&
		 (h_tal==video_640x480_60Hz.htotal)&&
		 (v_tal==video_640x480_60Hz.vtotal))
		{
			Debug_Printf("\r\nVideo_Check = video_640x480_60Hz ");
			Video_Format=video_640x480_60Hz_vic1;
			HDMI_VIC=1;
	  }	

	if((h_act==video_720x480_60Hz.hact)&&
		 (v_act==video_720x480_60Hz.vact)&&
		 (h_tal==video_720x480_60Hz.htotal)&&
		 (v_tal==video_720x480_60Hz.vtotal))
		{
			Debug_Printf("\r\nVideo_Check = video_720x480_60Hz ");
			Video_Format=video_720x480_60Hz_vic3;
			HDMI_VIC=3;
	  }	
	
	if((h_act==video_1280x720_60Hz.hact)&&
		 (v_act==video_1280x720_60Hz.vact)&&
		 (h_tal==video_1280x720_60Hz.htotal)&&
		 (v_tal==video_1280x720_60Hz.vtotal))
		{
			Debug_Printf("\r\nVideo_Check = video_1280x720_60Hz ");
			Video_Format=video_1280x720_60Hz_vic4;
			HDMI_VIC=4;
	  }
	if((h_act==video_1920x1080_60Hz.hact)&&
		 (v_act==video_1920x1080_60Hz.vact)&&
		 (h_tal==video_1920x1080_60Hz.htotal)&&
		 (v_tal==video_1920x1080_60Hz.vtotal))
		{
			Debug_Printf("\r\nVideo_Check = video_1920x1080_60Hz ");
			Video_Format=video_1920x1080_60Hz_vic16;
			HDMI_VIC=16;
	  }
}

void LT8618SX_Print_Video_Inf(void)
{
	Debug_Printf("\r\n##########################LT8618SX Input Infor#####################");
	Debug_Printf("\r\nhfp = ");
	Debug_DispNum(hfp); //Hex to Dec
	
	Debug_Printf("\r\nhs_width = ");
	Debug_DispNum(hs_width); //Hex to Dec
	
	Debug_Printf("\r\nhbp = ");
	Debug_DispNum(hbp); //Hex to Dec
	
	Debug_Printf("\r\nh_act = ");
	Debug_DispNum(h_act); //Hex to Dec
	
	Debug_Printf("\r\nh_tal = ");
	Debug_DispNum(h_tal); //Hex to Dec
	
	Debug_Printf("\r\nvfp = ");
	Debug_DispNum(vfp); //Hex to Dec
	
	Debug_Printf("\r\nvs_width = ");
	Debug_DispNum(vs_width); //Hex to Dec
	
	Debug_Printf("\r\nvbp = ");
	Debug_DispNum(vbp); //Hex to Dec
	
	Debug_Printf("\r\nv_act = ");
	Debug_DispNum(v_act); //Hex to Dec
	
	Debug_Printf("\r\nv_tal = ");
	Debug_DispNum(v_tal); //Hex to Dec
	Debug_Printf("\r\n############################end#####################################");
}


void LT8618SX_BT_Set(void)
{

	if (LT8618SX_Input_Mode==LT8618SX_Input_BT1120_16BIT)
	{
		HDMI_WriteI2C_Byte(0xff,0x82);
		HDMI_WriteI2C_Byte(0x20,(h_act>>8)); 
		HDMI_WriteI2C_Byte(0x21,h_act); 
		HDMI_WriteI2C_Byte(0x22,(hfp>>8)); 
		HDMI_WriteI2C_Byte(0x23,hfp); 
		HDMI_WriteI2C_Byte(0x24,(hs_width>>8)); 
		HDMI_WriteI2C_Byte(0x25,hs_width); 
		HDMI_WriteI2C_Byte(0x26,0x00);
		HDMI_WriteI2C_Byte(0x27,0x00);
		HDMI_WriteI2C_Byte(0x36,(v_act>>8));
		HDMI_WriteI2C_Byte(0x37,v_act);
		HDMI_WriteI2C_Byte(0x38,(vfp>>8));
		HDMI_WriteI2C_Byte(0x39,vfp);
		HDMI_WriteI2C_Byte(0x3a,(vbp>>8));
		HDMI_WriteI2C_Byte(0x3b,vbp);
		HDMI_WriteI2C_Byte(0x3c,(vs_width>>8));
		HDMI_WriteI2C_Byte(0x3d,vs_width);
	}
	else if (LT8618SX_Input_Mode==LT8618SX_Input_BT656_8BIT)
	{
		HDMI_WriteI2C_Byte(0xff,0x82);
		HDMI_WriteI2C_Byte(0x20,((h_act)>>8)); 
		HDMI_WriteI2C_Byte(0x21,(h_act)); 
		HDMI_WriteI2C_Byte(0x22,((hfp)>>8)); 
		HDMI_WriteI2C_Byte(0x23,(hfp)); 
		HDMI_WriteI2C_Byte(0x24,((hs_width)>>8)); 
		HDMI_WriteI2C_Byte(0x25,(hs_width)); 
		HDMI_WriteI2C_Byte(0x26,0x00);
		HDMI_WriteI2C_Byte(0x27,0x00);
		HDMI_WriteI2C_Byte(0x36,(v_act>>8));
		HDMI_WriteI2C_Byte(0x37,v_act);
		HDMI_WriteI2C_Byte(0x38,(vfp>>8));
		HDMI_WriteI2C_Byte(0x39,vfp);
		HDMI_WriteI2C_Byte(0x3a,(vbp>>8));
		HDMI_WriteI2C_Byte(0x3b,vbp);
		HDMI_WriteI2C_Byte(0x3c,(vs_width>>8));
		HDMI_WriteI2C_Byte(0x3d,vs_width);
	}
}

void LT8618SX_PLL(void) //zhangzhichun
{
	/*
	//4K 300MHz Pixel clk
	HDMI_WriteI2C_Byte(0xff,0x81);	
	HDMI_WriteI2C_Byte(0x23,0x40);
	HDMI_WriteI2C_Byte(0x24,0x64); //icp set
	HDMI_WriteI2C_Byte(0x25,0x0b);
	HDMI_WriteI2C_Byte(0x2c,0xbc);
	HDMI_WriteI2C_Byte(0x2d,0x88);
	HDMI_WriteI2C_Byte(0x2e,0x01);
	HDMI_WriteI2C_Byte(0x2f,0x00);
	HDMI_WriteI2C_Byte(0x26,0x55);
	HDMI_WriteI2C_Byte(0x27,0x60);
	HDMI_WriteI2C_Byte(0x28,0x88);
	*/
	if(bt656==1)
	{
		Debug_Printf("\r\nTx_PLL==LT8618SX_Input_BT656");
		HDMI_WriteI2C_Byte(0xff,0x81);	
		HDMI_WriteI2C_Byte(0x23,0x40);
		HDMI_WriteI2C_Byte(0x24,0x64); //icp set
		HDMI_WriteI2C_Byte(0x25,0x05);
		HDMI_WriteI2C_Byte(0x2c,0x9e);
		HDMI_WriteI2C_Byte(0x2d,0x89); //BT656
		HDMI_WriteI2C_Byte(0x2e,0x01);
		HDMI_WriteI2C_Byte(0x2f,0x00);
		HDMI_WriteI2C_Byte(0x26,0x55);
		HDMI_WriteI2C_Byte(0x27,0x60);
		HDMI_WriteI2C_Byte(0x28,0xa9);//BT656
	}
	else{
		if (LT8618SX_Input_Mode==LT8618SX_Input_RGB888||LT8618SX_Input_Mode==LT8618SX_Input_YCbCr444||LT8618SX_Input_Mode==LT8618SX_Input_YCbCr422_16BIT||LT8618SX_Input_Mode==LT8618SX_Input_BT1120_16BIT)
		{
			Debug_Printf("\r\nTx_PLL==LT8618SX_Input_RGB_SDR");
			HDMI_WriteI2C_Byte(0xff,0x81);	
			HDMI_WriteI2C_Byte(0x23,0x40);
			HDMI_WriteI2C_Byte(0x24,0x64); //icp set
			HDMI_WriteI2C_Byte(0x25,0x05);
			HDMI_WriteI2C_Byte(0x2c,0x9e);
			HDMI_WriteI2C_Byte(0x2d,0x88);
			HDMI_WriteI2C_Byte(0x2e,0x01);
			HDMI_WriteI2C_Byte(0x2f,0x00);
			HDMI_WriteI2C_Byte(0x26,0x55);
			HDMI_WriteI2C_Byte(0x27,0x60);
			HDMI_WriteI2C_Byte(0x28,0x88);
		}
		else if (LT8618SX_Input_Mode==LT8618SX_Input_BT656_8BIT||LT8618SX_Input_Mode==LT8618SX_Input_BT656_10BIT||LT8618SX_Input_Mode==LT8618SX_Input_BT656_12BIT)
		{
			Debug_Printf("\r\nTx_PLL==LT8618SX_Input_BT656");
			HDMI_WriteI2C_Byte(0xff,0x81);	
			HDMI_WriteI2C_Byte(0x23,0x40);
			HDMI_WriteI2C_Byte(0x24,0x64); //icp set
			HDMI_WriteI2C_Byte(0x25,0x05);
			HDMI_WriteI2C_Byte(0x2c,0x9e);
			HDMI_WriteI2C_Byte(0x2d,0x89); //BT656
			HDMI_WriteI2C_Byte(0x2e,0x01);
			HDMI_WriteI2C_Byte(0x2f,0x00);
			HDMI_WriteI2C_Byte(0x26,0x55);
			HDMI_WriteI2C_Byte(0x27,0x60);
			HDMI_WriteI2C_Byte(0x28,0xa9);//BT656
		}
	}
}


void LT8618SX_HDMI_TX_Phy(void) 
{
	HDMI_WriteI2C_Byte(0xff,0x81);
	HDMI_WriteI2C_Byte(0x30,0xea);
	HDMI_WriteI2C_Byte(0x31,0x44);
	HDMI_WriteI2C_Byte(0x32,0x4a);
	HDMI_WriteI2C_Byte(0x33,0x0b);
	HDMI_WriteI2C_Byte(0x34,0x00);
	HDMI_WriteI2C_Byte(0x35,0x00);
	HDMI_WriteI2C_Byte(0x36,0x00);
	HDMI_WriteI2C_Byte(0x37,0x44);
	HDMI_WriteI2C_Byte(0x3f,0x0f);
	HDMI_WriteI2C_Byte(0x40,0xa0);
	HDMI_WriteI2C_Byte(0x41,0xa0);
	HDMI_WriteI2C_Byte(0x42,0xa0);
	HDMI_WriteI2C_Byte(0x43,0xa0);
	HDMI_WriteI2C_Byte(0x44,0x0a);
}

void LT8618SX_HDMI_TX_Digital(void) 
{
	  //AVI
		HDMI_WriteI2C_Byte(0xff,0x84);
		HDMI_WriteI2C_Byte(0x43,0x56-HDMI_VIC-((HDMI_Y<<5)+0x10));  //AVI_PB0
		HDMI_WriteI2C_Byte(0x44,(HDMI_Y<<5)+0x10); //AVI_PB1
		HDMI_WriteI2C_Byte(0x47,HDMI_VIC); //AVI_PB1
}

void LT8618SX_CSC(void)
{
	if (LT8618SX_Output_Mode == LT8618SX_Output_RGB888)
	{
		HDMI_Y=0;
		HDMI_WriteI2C_Byte(0xff,0x82);
		if(LT8618SX_Input_Mode==LT8618SX_Input_YCbCr444)
		{
			HDMI_WriteI2C_Byte(0xb9,0x08); //YCbCr to RGB
		}
		else if(LT8618SX_Input_Mode==LT8618SX_Input_YCbCr422_16BIT|| \
			 LT8618SX_Input_Mode==LT8618SX_Input_BT1120_16BIT|| \
			 LT8618SX_Input_Mode==LT8618SX_Input_BT1120_20BIT|| \
			 LT8618SX_Input_Mode==LT8618SX_Input_BT1120_24BIT|| \
			 LT8618SX_Input_Mode==LT8618SX_Input_BT656_8BIT || \
			 LT8618SX_Input_Mode==LT8618SX_Input_BT656_10BIT|| \
			 LT8618SX_Input_Mode==LT8618SX_Input_BT656_12BIT)
		{
			HDMI_WriteI2C_Byte(0xb9,0x18); //YCbCr to RGB,YCbCr 422 convert to YCbCr 444
		}
		else
		  HDMI_WriteI2C_Byte(0xb9,0x00); //No csc
	}
	else if (LT8618SX_Output_Mode == LT8618SX_Output_YCbCr444)
	{
		 HDMI_Y=1;
	}
	else if ((LT8618SX_Output_Mode == LT8618SX_Output_YCbCr422_16BIT) || \
			(LT8618SX_Output_Mode == LT8618SX_Output_YCbCr422_20BIT) || \
			(LT8618SX_Output_Mode == LT8618SX_Output_YCbCr422_24BIT))
	{
		 HDMI_Y=2;
	}
}

void LT8618SX_Audio_Init(void)
{
	if(Audio_Input_Mode==I2S_2CH)
	{
		Debug_Printf("\r\nAudio inut I2S 2ch");
		HDMI_WriteI2C_Byte(0xff,0x82);
		HDMI_WriteI2C_Byte(0xd6,0x8c);
		HDMI_WriteI2C_Byte(0xd7,0x04); //sync polarity
		
		HDMI_WriteI2C_Byte(0xff,0x84);
		HDMI_WriteI2C_Byte(0x06,0x08);
		HDMI_WriteI2C_Byte(0x07,0x10);
		
		HDMI_WriteI2C_Byte(0x34,0xd4); //CTS_N
	}
	
	if(Audio_Input_Mode==SPDIF)
	{
		Debug_Printf("\r\nAudio inut I2S 2ch");
		HDMI_WriteI2C_Byte(0xff,0x82);
		HDMI_WriteI2C_Byte(0xd6,0x8c);
		HDMI_WriteI2C_Byte(0xd7,0x04); //sync polarity
		
		HDMI_WriteI2C_Byte(0xff,0x84);
		HDMI_WriteI2C_Byte(0x06,0x0c);
		HDMI_WriteI2C_Byte(0x07,0x10);
	
		HDMI_WriteI2C_Byte(0x34,0xd4); //CTS_N
	}
	
}

void LT8618SX_IRQ_Init(void)
{
		HDMI_WriteI2C_Byte(0xff,0x82);
		//HDMI_WriteI2C_Byte(0x10,0x00); //Output low level active;
		HDMI_WriteI2C_Byte(0x58,0x02); //Det HPD
		//HDMI_WriteI2C_Byte(0x00,0xff); //mask0
		//HDMI_WriteI2C_Byte(0x01,0xff); //mask1
		//HDMI_WriteI2C_Byte(0x02,0xff); //mask2
		HDMI_WriteI2C_Byte(0x03,0x3f); //mask3  //Tx_det
		//HDMI_WriteI2C_Byte(0x04,0xff); //clear0
		//HDMI_WriteI2C_Byte(0x05,0xff); //clear1
		//HDMI_WriteI2C_Byte(0x06,0xff); //clear2
		HDMI_WriteI2C_Byte(0x07,0xff); //clear3
		HDMI_WriteI2C_Byte(0x07,0x3f); //clear3
}


bool LT8918SX_Read_EDID(void)
{
		  u8 i,j;
		  //MCU_I2CMasterSwitchTo(IIC_TO_LT8618SX);
		  	 HDMI_WriteI2C_Byte(0xFF,0x80);
	             HDMI_WriteI2C_Byte(0xee,0x01);
			HDMI_WriteI2C_Byte(0xff,0x85);
			HDMI_WriteI2C_Byte(0x02,0x0a); //I2C 100K
			HDMI_WriteI2C_Byte(0x03,0xc9);
			HDMI_WriteI2C_Byte(0x04,0xa0); //0xA0 is EDID device address
			HDMI_WriteI2C_Byte(0x05,0x00); //0x00 is EDID offset address
			HDMI_WriteI2C_Byte(0x06,0x20); //length for read
			HDMI_WriteI2C_Byte(0x14,0x7f);
			Debug_Printf("\r\nRead EDID Start:");
			for(i=0;i<8;i++)
			{
				HDMI_WriteI2C_Byte(0x05,i*32); //0x00 is EDID offset address
				HDMI_WriteI2C_Byte(0x07,0x36);
				HDMI_WriteI2C_Byte(0x07,0x31);
				HDMI_WriteI2C_Byte(0x07,0x37);
				Delay_mSec(20);
				/*
				while(HDMI_ReadI2C_Byte(0x40)&0x04) //KEY_DDC_PRGRS=1
				{
					Debug_Printf("\r\nRead EDID DDC in progressd...");
					Delay_mSec(10);
				}
				*/
				if(HDMI_ReadI2C_Byte(0x40)&0x02) //KEY_DDC_ACCS_DONE=1
				{
					
					if(HDMI_ReadI2C_Byte(0x40)&0x50) //DDC No Ack or Abitration lost
					{
					 Debug_Printf("\r\nRead EDID Fail...");
					 return FALSE;
				  }
					else
					{
						for(j=0;j<32;j++)
							{
								Sink_EDID[i*32+j]=HDMI_ReadI2C_Byte(0x83);
								Debug_DispStrNum(" ",Sink_EDID[i*32+j]);
							}
					}
				}

			}
	return TRUE;
}


bool LT8618SX_HPD_Status(void)
{

	//MCU_I2CMasterSwitchTo(IIC_TO_LT8618SX);
	 HDMI_WriteI2C_Byte(0xFF,0x80);
	 HDMI_WriteI2C_Byte(0xee,0x01);
	 HDMI_WriteI2C_Byte(0xFF,0x81);
	 HDMI_WriteI2C_Byte(0x49,HDMI_ReadI2C_Byte(0x49)&0xfb);//by marui ,close hpd gpio function
	HDMI_WriteI2C_Byte(0xff,0x82);
	if(HDMI_ReadI2C_Byte(0x5e)&0x04)
	{
	  Tx_HPD=1;
	}
	else
	{
		Tx_HPD=0;
	}
	return Tx_HPD;
}

void LT8618SX_HDCP_Init(void)
{
	
}
void video_check(void)
	{
	LT8618SX_Video_Check();

	LT8618SX_Print_Video_Inf();
	LT8618SX_CSC();
	LT8618SX_HDMI_TX_Digital();
	LT8618SX_HDMI_TX_Phy();
	LT8618SX_BT_Set();	
	Debug_Printf("\r\nLT8618SX Initial End...");
}

void LT8618SX_Init(void)
{
	//RESET_Lt8618SX();
	//MCU_I2CMasterSwitchTo(IIC_TO_LT8618SX);
	 HDMI_WriteI2C_Byte(0xFF,0x80);
	 HDMI_WriteI2C_Byte(0xee,0x01);
	LT8618SX_Chip_ID();
	
	LT8618SX_RST_PD_Init();
	
	LT8618SX_TTL_Input_Analog();
	LT8618SX_TTL_Input_Digtal();
	LT8618SX_Video_Check();

	LT8618SX_PLL();
	LT8618SX_Audio_Init();
	LT8618SX_CSC();
	LT8618SX_HDMI_TX_Digital();
	LT8618SX_HDMI_TX_Phy();
	//LT8918SX_Read_EDID();

	//LT8618SX_Print_Video_Inf();
	LT8618SX_BT_Set();
	//video_check();
	Debug_Printf("\r\nLT8618SX Initial End...");
	
}


//--------------------------------------------------------------------------------------------------------------------------
void LT8618SX_SetCSC(WORD mode)
{
	BOOL yc2rgb=0, c422to444=0, bt709=0;
	BYTE rval;
	
	switch(mode)	{
		case 0:	HDMI_Y=0;	yc2rgb = 1;		c422to444 = 1;	bt709 = 1;	break;	// RGB444(bt709)
		case 1:	HDMI_Y=0;	yc2rgb = 1;		c422to444 = 1;	bt709 = 0;	break;	// RGB444(bt601)
		case 2:	HDMI_Y=2;	yc2rgb = 0;		c422to444 = 0;	bt709 = 0;	break;	// YC422(for M400_quad)
	}
	
	csc_bt709 = bt709;
	rval = (yc2rgb<<3) | (c422to444<<4);
	HDMI_WriteI2C_Byte(0xff,0x82);
	HDMI_WriteI2C_Byte(0xb9,rval); // [3]YCbCr -> RGB,  [4]422 -> 444
	
	LT8618SX_HDMI_TX_Digital();
}

//--------------------------------------------------------------------------------------------//
BYTE LT8618SX_GetCSC(void)
{
	BOOL yc2rgb, c422to444, bt709;
	BYTE rval;
	
	rval = HDMI_ReadI2C_Byte(0xb9);		// [3]YCbCr -> RGB,  [4]422 -> 444
	
	yc2rgb = (rval>>3) & 0x01;
	c422to444 = (rval>>4) & 0x01;
	bt709 = csc_bt709;
	
	switch(MAKEWORD(yc2rgb, MAKEBYTE(c422to444, bt709)))	{
		case 0x111:	return 0;	break;	// RGB444(bt709)
		case 0x110:	return 1;	break;	// RGB444(bt601)
		case 0x000:	return 2;	break;	// YC422(for M400_quad)
	}
	
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE LT8618SX_MultiWrite(BYTE sAddr, BYTE rAddr, PBYTE pBuff, WORD bytes)
{
#if defined(__USE_I2C_GPIO__)
	return I2CGPIO_Write(sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes);
#else
	return I2C_Write(TWI_CH0, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes); 	// addr(8bit), data(8bit)
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE LT8618SX_MultiRead(BYTE sAddr, BYTE rAddr, PBYTE pBuff, WORD bytes)
{
#if defined(__USE_I2C_GPIO__)
	return I2CGPIO_Read(sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes);//bh test
#else
	return I2C_Read(TWI_CH0, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes); 	// addr(8bit), data(8bit)
#endif
}

#endif	//__USE_LT8618SX_TX__
