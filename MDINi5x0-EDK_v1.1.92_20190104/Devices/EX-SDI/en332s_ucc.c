// ==================================================================================
//		UCC OSD Key Command
// ==================================================================================
//#include "../cpu/cpu_main.h"
//#include "enx_dev.h"
#include  "video.h"
#include  "osd.h"


#if defined(__USE_EN332T__)

const BYTE	UCC_U = 0x80;
const BYTE	UCC_D = 0x40;
const BYTE	UCC_R = 0x20;
const BYTE	UCC_L = 0x10;
const BYTE	UCC_C = 0x08;

UINT UCCPreState;
UINT UCCRxData;

// Camera side for test
void EN332SuccRx(WORD DevAddr)
{
	if((UCCRxData==UCC_L)&(UCCPreState!=UCC_L)){
		TxStrNoIRQ("UCC Rx LEFT",0,2);
		ENX_I2Cw(DevAddr,0xe4,0x0104d478);
		UCCPreState=UCC_L;
	}
	else if((UCCRxData==UCC_R)&(UCCPreState!=UCC_R)){
		TxStrNoIRQ("UCC Rx RIGHT",0,2);
		ENX_I2Cw(DevAddr,0xe4,0x01048080);
		UCCPreState=UCC_R;
	}
	else if((UCCRxData==UCC_U)&(UCCPreState!=UCC_U)){
		TxStrNoIRQ("UCC Rx UP",0,2);
		ENX_I2Cw(DevAddr,0xe4,0x01045e9f);
		UCCPreState=UCC_U;
	}
	else if((UCCRxData==UCC_D)&(UCCPreState!=UCC_D)){
		TxStrNoIRQ("UCC Rx DOWN",0,2);
		ENX_I2Cw(DevAddr,0xe4,0x01046dd4);
		UCCPreState=UCC_D;
	}
	else if((UCCRxData==UCC_C)&(UCCPreState!=UCC_C)){
		TxStrNoIRQ("UCC Rx CENTER",0,2);
		ENX_I2Cw(DevAddr,0xe4,0x01042c88);
		UCCPreState=UCC_C;
	}
}

// DVR side UART UCC
void EN332SKeyComm (WORD DevAddr)
{

	if(KEY_L)
	{			
		TxStrNoIRQ("UCC Tx LEFT",0,2);
		ENX_I2Cw(DevAddr,0x3e1,0x040001ff);
		ENX_I2Cw(DevAddr,0x3e2,0x050000);
		ENX_I2Cw(DevAddr,0x3e7,0x040001ff);
		ENX_I2Cw(DevAddr,0x3e8,0x050000);
		WaitUs(10000);
		ENX_I2Cw(DevAddr,0x306,0x0);
		WaitUs(10000);
		ENX_I2Cw(DevAddr,0x306,0x80000000);
	}
		
	else if(KEY_R)
	{		
		TxStrNoIRQ("UCC Tx RIGHT",0,2);
		ENX_I2Cw(DevAddr,0x3e1,0x020001ff);
		ENX_I2Cw(DevAddr,0x3e2,0x030000);
		ENX_I2Cw(DevAddr,0x3e7,0x020001ff);
		ENX_I2Cw(DevAddr,0x3e8,0x030000);
		WaitUs(10000);
		ENX_I2Cw(DevAddr,0x306,0x0);
		WaitUs(10000);
		ENX_I2Cw(DevAddr,0x306,0x80000000);
	}

	else if(KEY_U)
	{
		TxStrNoIRQ("UCC Tx UP",0,2);
		ENX_I2Cw(DevAddr,0x3e1,0x080001ff);
		ENX_I2Cw(DevAddr,0x3e2,0x090000);
		ENX_I2Cw(DevAddr,0x3e7,0x080001ff);
		ENX_I2Cw(DevAddr,0x3e8,0x090000);
		WaitUs(10000);
		ENX_I2Cw(DevAddr,0x306,0x0);
		WaitUs(10000);
		ENX_I2Cw(DevAddr,0x306,0x80000000);
	}

	else if(KEY_D)
	{
		TxStrNoIRQ("UCC Tx DOWN",0,2);
		ENX_I2Cw(DevAddr,0x3e1,0x100001ff);
		ENX_I2Cw(DevAddr,0x3e2,0x110000);
		ENX_I2Cw(DevAddr,0x3e7,0x100001ff);
		ENX_I2Cw(DevAddr,0x3e8,0x110000);
		WaitUs(10000);
		ENX_I2Cw(DevAddr,0x306,0x0);
		WaitUs(10000);
		ENX_I2Cw(DevAddr,0x306,0x80000000);
	}
		
	else if(KEY_C)
	{ 
		TxStrNoIRQ("UCC Tx CENTER",0,2);
		ENX_I2Cw(DevAddr,0x3e1,0x030001ff);
		ENX_I2Cw(DevAddr,0x3e2,0x040000);
		ENX_I2Cw(DevAddr,0x3e7,0x030001ff);
		ENX_I2Cw(DevAddr,0x3e8,0x040000);
		WaitUs(10000);
		ENX_I2Cw(DevAddr,0x306,0x0);
		WaitUs(10000);
		ENX_I2Cw(DevAddr,0x306,0x80000000);
	}
}

void EN332x_ExeUccRx(PBYTE pBuff)
{
	UARTprintf("[%02x %02x %02x %02x %02x %02x %02x]\r\n", \
				pBuff[0], pBuff[1], pBuff[2], pBuff[3], pBuff[4], pBuff[5], pBuff[6]);
	
	switch(pBuff[3])	{
		case 0x04:	DirLMessageControl();		break;	// left
		case 0x02:	DirRMessageControl();		break;	// right
		case 0x08:	DirUMessageControl();		break;	// up
		case 0x10:	DirDMessageControl();		break;	// down
		case 0x1e:	if(GetOSDMenuID()==0x0000)
						MenuMessageControl();
					else
						EnterMessageControl();
					break;								// center
	}
}

#endif	//#if defined(__USE_EN332T__)
