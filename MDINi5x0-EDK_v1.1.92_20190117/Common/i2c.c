//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2005  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	I2C.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"video.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
static void I2C_SwapBuff(PBYTE pBuff, WORD bytes)
{
	BYTE	i, temp=0;
	for(i=0; i<bytes/2; i++)		//0<->1, 2<->3,,,
	{
		temp = pBuff[i*2];
		pBuff[i*2] = pBuff[i*2+1];
		pBuff[i*2+1] = temp;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE I2C_Write(BYTE ch, BYTE sAddr, WORD rAddr, BOOL addr_16bit, PBYTE pBuff, BOOL data_16bit, WORD bytes)
{
	BYTE reg_addr[2], len;
	int ret;
	
	// check address bit length(0:8bit, 1:16bit)
	if(addr_16bit) 		{reg_addr[0] = HIBYTE(rAddr);	reg_addr[1] = LOBYTE(rAddr);	len=2;} 	// addr(16bit)
	else 				{reg_addr[0] = LOBYTE(rAddr);	len=1;} 									// addr(8bit)

	// check data bit length(1:8bit, 2:16bit)
	if(data_16bit) 		I2C_SwapBuff(pBuff, bytes); 		// byte swap(for data 16bit)
	
	ret = twi_write(ch, sAddr, &reg_addr[0], len, pBuff, bytes);
	
	twi_wait_busy(ch, sAddr);

	if (ret < 0) { PRINTLINE;	return I2C_TIME_OUT;	}		
	return I2C_OK;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE I2C_Read(BYTE ch, BYTE sAddr, WORD rAddr, BOOL addr_16bit, PBYTE pBuff, BOOL data_16bit, WORD bytes)
{
	BYTE reg_addr[2], len;
	int ret;
	
	// check address bit length(0:8bit, 1:16bit)
	if(addr_16bit) 		{reg_addr[0] = HIBYTE(rAddr);	reg_addr[1] = LOBYTE(rAddr);	len=2;} 	// addr(16bit)
	else 				{reg_addr[0] = LOBYTE(rAddr);	len=1;} 									// addr(8bit)

	ret = twi_read(ch, sAddr&0xfe, &reg_addr[0], len, pBuff, bytes);
	
	// check data bit length(0:8bit, 1:16bit)
	if(data_16bit) 		I2C_SwapBuff(pBuff, bytes); 		// byte swap(for data 16bit)

	twi_wait_busy(ch, sAddr);
	
	if (ret < 0) { PRINTLINE;	return I2C_TIME_OUT;	}	
	return I2C_OK;
}

//--------------------------------------------------------------------------------------------------------------------------
void I2C_ByteWrite(BYTE ch, BYTE sAddr, BYTE rAddr, BYTE wData)
{
	I2C_Write(ch, sAddr, rAddr, I2C_ADDR_8BIT, (PBYTE)&wData, I2C_DATA_8BIT, 1);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE I2C_ByteRead(BYTE ch, BYTE sAddr, BYTE rAddr)
{
	BYTE rData;
	I2C_Read(ch, sAddr, rAddr, I2C_ADDR_8BIT, (PBYTE)&rData, I2C_DATA_8BIT, 1);
	return rData;
}

#ifdef	EEPROM_SIZE_OVER_16KBIT
//--------------------------------------------------------------------------------------------------------------------------
static BYTE AT24C32I2C_Write(WORD rAddr, PBYTE pBuff, WORD bytes)  //addr(16b) + data(8b)
{
	BYTE waddr[2];	BYTE sAddr = I2C_AT24C02_ADDR;
	int ret, len;
	
	waddr[0] = rAddr >> 8;
	waddr[1] = rAddr & 0xff;
	len = 2;										//addr = 16bit

	ret = twi_write(EEPROM_I2C_CH, sAddr, waddr, len, pBuff, bytes);
	Delay_mSec(10);			//tWR : max 10ms
	
	if (ret < 0) { PRINTLINE;	return I2C_TIME_OUT;	}		
	return I2C_OK;
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE AT24C32I2C_Read(WORD rAddr, PBYTE pBuff, WORD bytes)		//addr(16b) + data(8b)
{
	BYTE waddr[2];	BYTE sAddr = I2C_AT24C02_ADDR;
	int ret, len;
	
	waddr[0] = rAddr >> 8;
	waddr[1] = rAddr & 0xff;
	len = 2;								//addr = 16bit
	
	ret = twi_read(EEPROM_I2C_CH, sAddr, waddr, len, pBuff, bytes);
	
	if (ret < 0) { PRINTLINE;	return I2C_TIME_OUT;	}	
	return I2C_OK;
}
#else	//EEPROM_SIZE_OVER_16KBIT
//--------------------------------------------------------------------------------------------------------------------------
static BYTE AT24C16I2C_Write(WORD rAddr, PBYTE pBuff, WORD bytes)  //addr(8/16b) + data(8b)
{
	BYTE waddr[1], sAddr;
	int ret, len;
	
	sAddr = I2C_AT24C02_ADDR | ((rAddr>>8) & 0x07) ;
	waddr[0] = (BYTE)(rAddr & 0xff);
	len = 1;
	
	ret = twi_write (EEPROM_I2C_CH, sAddr, waddr, len, pBuff, bytes);
	Delay_mSec(10);			//tWR : max 10ms
	
	if (ret < 0) { PRINTLINE;	return I2C_TIME_OUT;	}		
	return I2C_OK;
}
//--------------------------------------------------------------------------------------------------------------------------
static BYTE AT24C16I2C_Read(WORD rAddr, PBYTE pBuff, WORD bytes)		//addr(8/16b) + data(8b)
{
	BYTE waddr[2], sAddr;
	int ret, len;
	
	sAddr = I2C_AT24C02_ADDR | ((rAddr>>8) & 0x07) ;
	waddr[0] = (BYTE)(rAddr & 0xff);
	len = 1;
	
	ret = twi_read (EEPROM_I2C_CH, sAddr, waddr, len, pBuff, bytes);
	
	if (ret < 0) { PRINTLINE;	return I2C_TIME_OUT;	}	
	return I2C_OK;
}
#endif	//EEPROM_SIZE_OVER_16KBIT

//--------------------------------------------------------------------------------------------------------------------------
void EEPROM_MultiWrite(WORD rAddr, PBYTE pBuff, WORD bytes)
{
	WORD len;
	BYTE unit = 8;					// page write mode : at24c02(8bytes), at24c04(16bytes), at24c32(32bytes)

	while (bytes>0) {
		len = MIN(unit-(rAddr%unit), bytes);	// calc write byte length(up to page boundary)
		
#ifdef	EEPROM_SIZE_OVER_16KBIT
		AT24C32I2C_Write(rAddr, pBuff, len);
#else
		AT24C16I2C_Write(rAddr, pBuff, len);
#endif
		rAddr += len; pBuff += len;	bytes -= len;
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void EEPROM_MultiRead(WORD rAddr, PBYTE pBuff, WORD bytes)
{
#ifdef	EEPROM_SIZE_OVER_16KBIT
	AT24C32I2C_Read(rAddr, pBuff, bytes);
#else
	AT24C16I2C_Read(rAddr, pBuff, bytes);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void EEPROM_RegWrite(WORD rAddr, WORD wData)
{
	EEPROM_MultiWrite(rAddr, (PBYTE)&wData, 2);
}

//--------------------------------------------------------------------------------------------------------------------------
WORD EEPROM_RegRead(WORD rAddr)
{
	WORD rData;
	EEPROM_MultiRead(rAddr, (PBYTE)&rData, 2);
	return rData;
}

//--------------------------------------------------------------------------------------------------------------------------
void IOExp_Write(BYTE nID, WORD rAddr, BYTE wData)
{
	I2C_ByteWrite(TWI_CH1, I2C_TCA6408_ADDR | ((nID==0)?  0:  2), rAddr, wData);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE IOExp_Read(BYTE nID, WORD rAddr)
{
	return I2C_ByteRead(TWI_CH1, I2C_TCA6408_ADDR | ((nID==0)?  0:  2), rAddr);
}

//--------------------------------------------------------------------------------------------------------------------------
void IOExp_Field(BYTE nID, WORD rAddr, BYTE bPos, BYTE bCnt, BYTE bData)
{
	BYTE m = (~((~0)<<(bCnt)))<<(bPos);
	BYTE r;
	
//	if (bPos>7||bCnt==0||bCnt>7||(bPos+bCnt)>7) return;	// check invalid parameter
	r = IOExp_Read(nID, rAddr);
	r = (r&~m) | (((bData)<<(bPos))&m);
	return IOExp_Write(nID, rAddr, r);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE CDCE913_RegRead(WORD rAddr)
{
	return I2C_ByteRead(TWI_CH1, I2C_CDCE913_ADDR, rAddr|0x80);	// rAddr[7]  0:Block R/W,  1:Byte R/W
}

//--------------------------------------------------------------------------------------------------------------------------
void CDCE913_RegWrite(WORD rAddr, BYTE wData)
{
	I2C_ByteWrite(TWI_CH1, I2C_CDCE913_ADDR, rAddr|0x80, wData);	// rAddr[7]  0:Block R/W,  1:Byte R/W
}

//--------------------------------------------------------------------------------------------------------------------------
void CDCE913_RegField(WORD rAddr, BYTE bPos, BYTE bCnt, BYTE bData)
{
	BYTE m = (~((~0)<<(bCnt)))<<(bPos);
	BYTE r;
	
//	if (bPos>7||bCnt==0||bCnt>7||(bPos+bCnt)>7) return;	// check invalid parameter
	r = CDCE913_RegRead(rAddr);
	r = (r&~m) | (((bData)<<(bPos))&m);
	return CDCE913_RegWrite(rAddr, r);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ENX_I2Cw(WORD sAddr, WORD rAddr, DWORD RegData)
{
	BYTE page_addr, page_data, reg_addr, ch;
	int ret;
	
	ch = 0;							// i2c ch0
	page_addr = 0;
	page_data = HIBYTE(rAddr<<2);
	reg_addr = LOBYTE(rAddr<<2);
	
	// Start |Slave addr|w|ack| Page addr|ack| Page data|ack|Stop|
	ret = twi_write(ch, sAddr, &page_addr, 1, &page_data, 1);		// set page data
	twi_wait_busy(ch, sAddr);
	
	// |Start|Slave addr|w|ack| Reg addr|ack| Reg Data[7:0]|ack| | [15:8]|ack| | [23:16]|ack| | [31:24]|ack| Stop|
	ret = twi_write(ch, sAddr, &reg_addr, 1, (PBYTE)&RegData, 4);	// write reg data(4byte)
	twi_wait_busy(ch, sAddr);

	if (ret < 0) { PRINTLINE;	return I2C_TIME_OUT;	}		
	return I2C_OK;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ENX_I2Cr(WORD sAddr, WORD rAddr, PDWORD pBuff)
{
	BYTE page_addr, page_data, reg_addr, ch;
	int ret;
	
	ch = 0;							// i2c ch0
	page_addr = 0;
	page_data = HIBYTE(rAddr<<2);
	reg_addr = LOBYTE(rAddr<<2);
	
	// Start |Slave addr|w|ack| Page addr|ack| Page data|ack|Stop|
	ret = twi_write(ch, sAddr, &page_addr, 1, &page_data, 1);	// set page data
	twi_wait_busy(ch, sAddr);
	
	// |Start|Slave addr|w|ack| Reg addr|ack| RS| Slave addr|r|ack| Reg Data[7:0]|ack| | [15:8]|ack| | [23:16]|ack| | [31:24]|Nack| Stop|
	ret = twi_read(ch, sAddr&0xfe, &reg_addr, 1, (PBYTE)pBuff, 4);
	twi_wait_busy(ch, sAddr);
	
	if (ret < 0) { PRINTLINE;	return I2C_TIME_OUT;	}	
	return I2C_OK;
}

//--------------------------------------------------------------------------------------------------------------------------
void ENX_MSK_I2Cw(WORD sAddr, PDWORD pTable, BYTE size)
{
 	BYTE i;
	DWORD Buf;
	
	for(i=0;i<size;i++)	{
		ENX_I2Cr(sAddr, pTable[i*3+0], &Buf);		// table(0 : addr, 1 : data, 2 : mask bit)
		Buf &= (~pTable[i*3+2]);					// clear mask bit
		Buf |= (pTable[i*3+1] & (pTable[i*3+2]));	// overwrite new data
		ENX_I2Cw(sAddr, pTable[i*3+0], Buf);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void ENX_TBL_I2Cw(WORD sAddr, PDWORD pTable, BYTE size)
{
 	BYTE i;
	
	for(i=0;i<size;i++)	{
		ENX_I2Cw(sAddr, pTable[i*2+0], pTable[i*2+1]);	// table(0 : addr, 1 : data)
	}
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ENX_MultiRead(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE ch = 0, page_data, reg_addr;
	
	page_data = HIBYTE(rAddr<<2);
	reg_addr = LOBYTE(rAddr<<2);
	
	I2C_Write(ch, sAddr, 0x00, I2C_ADDR_8BIT, &page_data, I2C_DATA_8BIT, 1);		// set page
	return I2C_Read(ch, sAddr, reg_addr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes);	// read reg. data
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE ENX_MultiWrite(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	BYTE ch = 0, page_data, reg_addr;
	
	page_data = HIBYTE(rAddr<<2);
	reg_addr = LOBYTE(rAddr<<2);
	
	I2C_Write(ch, sAddr, 0x00, I2C_ADDR_8BIT, &page_data, I2C_DATA_8BIT, 1);		// set page
	return I2C_Write(ch, sAddr, reg_addr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes);	// write reg. data
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE TVI_MultiRead(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Read(TP2912_I2C_CH, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE TVI_MultiWrite(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Write(TP2912_I2C_CH, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE LCMX03L_RegRead(WORD rAddr)
{
	return I2C_ByteRead(TWI_CH0, I2C_LCMX03L_ADDR, rAddr);
}

//--------------------------------------------------------------------------------------------------------------------------
void LCMX03L_RegWrite(WORD rAddr, BYTE wData)
{
	I2C_ByteWrite(TWI_CH0, I2C_LCMX03L_ADDR, rAddr, wData);
}

//--------------------------------------------------------------------------------------------------------------------------
void LCMX03L_RegField(WORD rAddr, BYTE bPos, BYTE bCnt, BYTE bData)
{
	BYTE m = (~((~0)<<(bCnt)))<<(bPos);
	BYTE r;
	
//	if (bPos>7||bCnt==0||bCnt>7||(bPos+bCnt)>7) return;	// check invalid parameter
	r = LCMX03L_RegRead(rAddr);
	r = (r&~m) | (((bData)<<(bPos))&m);
	return LCMX03L_RegWrite(rAddr, r);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE LCMX03L_MultiRead(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Read(TWI_CH0, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE LCMX03L_MultiWrite(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Write(TWI_CH0, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE LIAMD6K_RegRead(WORD rAddr)
{
	return I2C_ByteRead(TWI_CH1, I2C_LIAMD6K_ADDR, rAddr);
}

//--------------------------------------------------------------------------------------------------------------------------
void  LIAMD6K_RegWrite(WORD rAddr, BYTE wData)
{
	I2C_ByteWrite(TWI_CH1, I2C_LIAMD6K_ADDR, rAddr, wData);
}

//--------------------------------------------------------------------------------------------------------------------------
void LIAMD6K_RegField(WORD rAddr, BYTE bPos, BYTE bCnt, BYTE bData)
{
	BYTE m = (~((~0)<<(bCnt)))<<(bPos);
	BYTE r;
	
//	if (bPos>7||bCnt==0||bCnt>7||(bPos+bCnt)>7) return;	// check invalid parameter
	r = LIAMD6K_RegRead(rAddr);
	r = (r&~m) | (((bData)<<(bPos))&m);
	return LIAMD6K_RegWrite(rAddr, r);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE LIAMD6K_MultiRead(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Read(TWI_CH1, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes); // test
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE LIAMD6K_MultiWrite(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Write(TWI_CH1, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes); // test
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE PS8409_RegRead(WORD rAddr)
{
	return I2C_ByteRead(TWI_CH1, I2C_PS8409_ADDR, rAddr);
}

//--------------------------------------------------------------------------------------------------------------------------
void PS8409_RegWrite(WORD rAddr, BYTE wData)
{
	I2C_ByteWrite(TWI_CH1, I2C_PS8409_ADDR, rAddr, wData);
}

//--------------------------------------------------------------------------------------------------------------------------
void PS8409_RegField(WORD rAddr, BYTE bPos, BYTE bCnt, BYTE bData)
{
	BYTE m = (~((~0)<<(bCnt)))<<(bPos);
	BYTE r;
	
//	if (bPos>7||bCnt==0||bCnt>7||(bPos+bCnt)>7) return;	// check invalid parameter
	r = PS8409_RegRead(rAddr);
	r = (r&~m) | (((bData)<<(bPos))&m);
	return PS8409_RegWrite(rAddr, r);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE PS8409_MultiRead(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Read(TWI_CH1, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes);
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE PS8409_MultiWrite(WORD sAddr, WORD rAddr, PBYTE pBuff, WORD bytes)
{
	return I2C_Write(TWI_CH1, sAddr, rAddr, I2C_ADDR_8BIT, pBuff, I2C_DATA_8BIT, bytes);
}


/***********************************************************
 *
 *  Bitbang mode I2C implementation on GPIO pins
 *
 ***********************************************************/

#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)

#define SetSCL(x)		SET_SCL_GPIO(x)
#define SetSDA(x)		{SET_SDA_GPIO(x);	WR_SDA_GPIO();}
#define GetSCL(x)		GET_SCL_GPIO()
#define GetSDA(x)		GET_SDA_GPIO()
#define	SetInSDA()		{RD_SDA_GPIO();		Delay_uSec(5);}		// gpio mode trans time 'write to read' : min 4us

#define	i2c_delay(x)	Delay_uSec(x)

//I2C bus speed = ~100kHz
#define SH_DELDY	1	//delay for setup or hold time
#define H_PERIOD	1//5	//half period of I2C clock cycle

#if 0
// check if bus is free
//--------------------------------------------------------------------------------------------------------------------------
static int check_bus(void) 
{
	SetInSDA();		// set sda pin 'input mode'
	
	return !GetSDA() || !GetSCL();
}
#endif

// start condition
//--------------------------------------------------------------------------------------------------------------------------
static void i2c_start()
{
	SetSDA(0);
	i2c_delay(H_PERIOD);
	SetSCL(0);
}

// restart condition
//--------------------------------------------------------------------------------------------------------------------------
static void i2c_restart()
{
	SetSDA(1);
	i2c_delay(SH_DELDY);
	SetSCL(1);
	i2c_delay(H_PERIOD);
	i2c_start();
}

//stop condition
//--------------------------------------------------------------------------------------------------------------------------
static void i2c_stop()
{
	SetSDA(0);
	i2c_delay(SH_DELDY);
	SetSCL(1);
	i2c_delay(H_PERIOD);
    SetSDA(1);
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE i2c_abort(int p, WORD rAddr)
{
	i2c_stop();
//	UARTprintf("I2C_GPIO(%04X) %s NACK!!\n", rAddr, p&8?"RD":"WR");
	return I2C_HOST_NACK;
}

//--------------------------------------------------------------------------------------------------------------------------
static char i2c_write_byte(BYTE wrData)
{
	int i;
	BYTE Temp;

	for(i = 0; i < 8; i++) {
		i2c_delay(SH_DELDY);
		Temp = (wrData >> (7 - i)) & 0x1;
		SetSDA(Temp);
		i2c_delay(H_PERIOD);
		SetSCL(1);	//rising
		i2c_delay(H_PERIOD);
		SetSCL(0);	//falling
	}

	//SetSDA(1);	// set 'high' before port read
	if(Temp==0)		i2c_delay(SH_DELDY);	// for remove ripple noise by pull-up before slave device's 'ack'
	SetInSDA();		// set sda pin 'input mode'
	
	i2c_delay(H_PERIOD);
	SetSCL(1);	//rising
	i2c_delay(H_PERIOD);
	// sample ACK.
	Temp = GetSDA();
	SetSDA(Temp);	// for remove dummy bit by pull-up after slave device's 'ack'
	SetSCL(0);	//falling
	i2c_delay(H_PERIOD);

	return Temp;	//ACK
}

//--------------------------------------------------------------------------------------------------------------------------
static BYTE i2c_read_byte(BYTE stop)
{
	int i;
	BYTE rdData;

	rdData = 0;
	//SetSDA(1);	// set 'high' before port read
	SetInSDA();		// set sda pin 'input mode'
	
	// read 8 bits.
	for(i = 0; i < 8; i++) {
		i2c_delay(H_PERIOD);
		SetSCL(1);	//rising
		i2c_delay(H_PERIOD);
		rdData = (rdData << 1) | GetSDA();
		SetSCL(0);	//falling
	}

	i2c_delay(SH_DELDY);
	SetSDA(stop);	//ACK(stop=0) or NACK(stop=1)
	i2c_delay(H_PERIOD);
	SetSCL(1);	//rising
	i2c_delay(H_PERIOD);
	SetSCL(0);	//falling
	i2c_delay(H_PERIOD);

	return rdData;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE I2CGPIO_Write(BYTE sAddr, WORD rAddr, BOOL addr_16bit, PBYTE pBuff, BOOL data_16bit, WORD bytes)
{
	int i;

//	if(check_bus()) return I2C_NOT_FREE;	//bus is not free
	i2c_start();	// start condition
	if(i2c_write_byte(sAddr & 0xFE)) return i2c_abort(0|1, rAddr);	// device ID
	if(addr_16bit)	{if(i2c_write_byte(HIBYTE(rAddr))) return i2c_abort(0|4, rAddr);}	// register address
	if(i2c_write_byte(LOBYTE(rAddr))) return i2c_abort(0|2, rAddr);	// register address

	for(i=0; i<bytes; i++)
		if(i2c_write_byte(pBuff[i])) return i2c_abort(0|3, rAddr);	// send data

	i2c_stop();	// stop condition
	return I2C_OK;
}

//--------------------------------------------------------------------------------------------------------------------------
BYTE I2CGPIO_Read(BYTE sAddr, WORD rAddr, BOOL addr_16bit, PBYTE pBuff, BOOL data_16bit, WORD bytes)
{
	int i;

//	if(check_bus()) return I2C_NOT_FREE;	//bus is not free
	i2c_start();	// start condition
	if(i2c_write_byte(sAddr & 0xFE)) return i2c_abort(8|1, rAddr);	// device ID
	if(addr_16bit)	{if(i2c_write_byte(HIBYTE(rAddr))) return i2c_abort(8|4, rAddr);}	// register address
	if(i2c_write_byte(LOBYTE(rAddr))) return i2c_abort(8|2, rAddr);	// register address

	i2c_restart();	// repeated start condition
	if(i2c_write_byte(sAddr | 1)) return i2c_abort(8|3, rAddr);	// device ID
	for(i=0; i<bytes; i++)
		pBuff[i] = i2c_read_byte((i==bytes-1)?  1:  0 /*ACK(0) or NACK(1)*/);	// receive data

	i2c_stop();	// stop condition
	return I2C_OK;
}
#endif	//#if defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__)


/*  FILE_END_HERE */
