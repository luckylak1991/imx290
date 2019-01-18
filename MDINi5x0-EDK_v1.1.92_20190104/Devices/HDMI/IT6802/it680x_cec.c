///*****************************************
//  Copyright (C) 2009-2016
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <it680x_cec.c>
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2016/05/03
//   @fileversion: ITE_MHLRX_SAMPLE_V1.24
//******************************************/
//#include "it680x_cec.h"
//#include "stdio.h"
//#include "Utility.h"
#include "it680x_inc.h"


#if defined(__USE_IT6802_RX__)


#ifdef Enable_IT6802_CEC

#define	Enable_IT6802_CECB0
#define	ENABLE_LED

#ifdef	DEBUG_IT6802_CEC
#define	IT6802_CEC_PRINT(x) printf x
#else
#define	IT6802_CEC_PRINT(x)
#endif


#define Q_SIZE 3

CEC_FRAME xdata queue[ Q_SIZE ];
P_CEC_FRAME xdata pCECRxBuf;

CECDevice CECList[15];

CEC_TX_StateType xdata CECTxState=sCECOff;
eCEC_CMD OldTxCECcmd=eCECNone;

ARC_StateType ARCState=sARCNone;
LogicAddressing_StateType LAState=sLANone;
OneTouchPlay_StateType OTState=sOTNone;
PollingMessage_StateType PMState=sPMnone;
SystemInfoFeature_StateType SIFState=sSIFnone;
SystemAudioControl_StateType SACState=sSACnone;

//=== Audio System Control ===
//for Spec 13.15.2 and CTS 11.2.15
// 0 Terminating , 1 Initating the system audio control
//need to un-mute Amplifer speaker
BOOL bSystemAudioMode=0;
CECAudioStatus uCECAudioStatus;
CECAudioFormatID uCECAudioFormatID;
BYTE UICommandCode;

u8_t StandbyState;

CEC_FRAME TxCmdBuf;
USHORT	CMDCount=0;
u8_t u8_PowerStatus=0;	// 0: on , 1: standby, 2: in transition standby to on , 3: in transition on to standby

u8_t		head;
u8_t		tail;
u8_t		CECInt;
u8_t 	RxSelf_Dis = 1;	// 1 for check transmiter data by self rx buffer

u8_t		Myself_LogicAdr;
u8_t		Myself_PhyAdr1;
u8_t		Myself_PhyAdr2;


eResultType	RegTxOutState=eResultNone;

#if	0
//FOR IT6802+iT6623
BYTE xdata VSDB_Addr;
BYTE xdata txphyadr[2], txphyA, txphyB, txphyC, txphyD, txphylevel;
BYTE xdata rxphyadr[4][2];
BYTE xdata rxphyA, rxphyB, rxphyC, rxphyD, rxcurport;
#else
extern BYTE xdata VSDB_Addr;
extern BYTE xdata txphyadr[2], txphyA, txphyB, txphyC, txphyD, txphylevel;
extern BYTE xdata rxphyadr[2][2];
extern BYTE xdata rxphyA, rxphyB, rxphyC, rxphyD, rxcurport;
#endif

//for HEC Command
u8_t		FollowerPhyAdr1;
u8_t		FollowerPhyAdr2;
u8_t		FollowerPhyAdr3;
u8_t		FollowerPhyAdr4;

u8_t		FollowerAddr;
u8_t		timeunit;

//xxxxx 2014-0128
CECCallbacks        m_CECcallbacks;
CECCallbacks		*pm_CECcallbacks;
//xxxxx


code CECcmdFormat LookUpTable_CEC[]=
{

    {eFeatureAbort,			eDirectly,		4},			// [Header] + [0x00] + [Feature OpCode] + [Abort Reason]
    {eAbort,					eDirectly,		2},			// [Header] + [0xFF]

    // One Touch Play Feature
    {eActiveSource,			eBroadcast,	4},			// Header + [0x82] + [Physical Address]
    {eImageViewOn,			eDirectly,		2},			// Header + [0x04]
    {eTextViewOn,			eDirectly,		2},			// Header + [0x0D]

    // Routing Control Feature
    {eInactiveSource,			eBroadcast,	2},			// Header + [0x9D]
    {eRequestActiveSource,		eBroadcast,	2},			// Header + [0x85]
    {eRoutingChange,			eBroadcast,	6},			// Header + [0x80] + [Org Address] + [New Address]
    {eRoutingInformation,		eBroadcast,	4},			// Header + [0x80] + [Org Address] + [New Address]
    {eSetStreamPath,			eBroadcast,	4},			// Header + [0x86] + [Physical Address]
    {eStandBy,				eBoth,		2},			// Header + [0x36]

    // System Information Feature
    {eCECVersioin,			eDirectly,		3},			// Header + [0x82] + [CEC Version]
    {eGetCECVersion,			eDirectly,		2},			// Header + [0x9F]
    {eGivePhysicalAddress,		eDirectly,		2},			// Header + [0x83]
    {eGetMenuLanguage,		eDirectly,		2},			// Header + [0x91]
    {eReportPhysicalAddress,	eBroadcast,	5},			// Header + [0x84] + [Physical Address]+ [Device Type]
    {eSetMenuLanguage,		eBroadcast,	3},			// Header + [0x32] + [Language]

    // Vendor Specific Commands Feature
    {eDeviceVendorID,		eBroadcast,	5},			// Header + [0x87] + [Vendor ID]
    {eGiveDeviceVendorID,		eDirectly,		2},			// Header + [0x8C]
    {eVendorCommand,		eDirectly,		3},			// Header + [0x89] + [Vendor Specific Data]
    {eVendorCommandWithID,	eBoth,		6},			// Header + [0xA0] + [Vendor ID]+ [Vendor Specific Data]
    {eRemoteButtonDown,		eBoth,		3},			// Header + [0x8A] + [Vendor Specific RC code]
    {eRemoteButtonUp,		eBoth,		3},			// Header + [0x8B] + [Vendor Specific RC code]

    // Remote Control PassThrough Feature
    {eUserPressed,			eDirectly,		3},			// Header + [0x44] + [UI command]
    {eUserReleased,			eDirectly,		2},			// Header + [0x45]

    // Power Status Feature
    {eGiveDevicePowerStatus,	eDirectly,		2},			// Header + [0x8F]
    {eReportPowerStatus,		eDirectly,		3},			// Header + [0x90] + [Power Status]

    // System Audio Control Feature
    {eGiveAudioStatus,		eDirectly,		2},			// Header + [0x71]
    {eGiveSystemAudIoModeStatus,eDirectly,	2},			// Header + [0x7D]
    {eReportAudioStatus,		eDirectly,		3},			// Header + [0x7A] + [Audio Status]
    {eReportAudioDescriptor,	eDirectly,		4},			// Header + [0xA3] + [Audio Descriptor]
    {eRequestAudioDescriptor,	eDirectly,		3},			// Header + [0xA4] + [Audio Format ID and Code]
    {eSetSystemAudioMode	,	eBoth,		3},			// Header + [0x72] + [System Audio Status]
    {eAudioModeRequest,		eDirectly,		4},			// Header + [0x70] + [Physical Address]
    {eSystemAudIoModeStatus,	eDirectly,		3},			// Header + [0x7E] + [System Audio Status]

    // ACR
    {eInitiateARC,				eDirectly,		2},			// Header + [0xC0]
    {eReportARCInitiated,		eDirectly,		2},			// Header + [0xC1]
    {eReportARCTerminated,	eDirectly,		2},			// Header + [0xC2]
    {eRequestARCInitiation,		eDirectly,		2},			// Header + [0xC3]
    {eRequestARCTermination,	eDirectly,		2},			// Header + [0xC4]
    {eTerminateARC,			eDirectly,		2},			// Header + [0xC5]

    // HEC
    {eCDC,					eDirectly,		3},			// Header + [0xF8]

    // other
    {ePlay,					eDirectly,		3},			// Header + [0x41] + [Play Mode]


    // Polling Mesage
    //ePollingMessage,
    {eCDC+1,				eBoth,		0},			//end of 	lookup table !!!
 };

#define     SizeOfLookUpTable_CEC    (sizeof(LookUpTable_CEC)/sizeof(CECcmdFormat))



char * ARCStr[] = {
"sRequestARCInitiation",
"sInitiateARC",
"sReportARCInitiated",
"sRequestARCTermination",
"sTerminateARC",
"sReportARCTermination",
"sARCNone",
};

void DumpIT6802_CECReg(void)
{


#ifdef DEBUG_IT6802_CEC
    int i,j;
    BYTE ucData;



     IT6802_CEC_PRINT(("       "));
    for( j = 0; j < 16; j++ )
    {
         IT6802_CEC_PRINT((" %02X",(int)j));
        if( (j == 3)||(j==7)||(j==11))
        {
            IT6802_CEC_PRINT(("  "));
        }
    }
     IT6802_CEC_PRINT(("\n        -----------------------------------------------------\n"));



    for(i = 0; i < 0x4F; i+=16 )
    {
         IT6802_CEC_PRINT(("[%02X]  ",(int) i));
        for( j = 0; j < 16; j++ )
        {
            ucData = IT6802_CEC_ReadI2C_Byte((int)((i+j)&0xFF));
              IT6802_CEC_PRINT((" %02X",(int) ucData));
            if( (j == 3)||(j==7)||(j==11))
            {
                 IT6802_CEC_PRINT((" -"));
            }
        }
          IT6802_CEC_PRINT(("\n"));
        if( (i % 0x40) == 0x30)
        {
             IT6802_CEC_PRINT(("        -----------------------------------------------------\n"));
        }
    }

#endif
}


void
initQ()
{
    head = 0;
    tail = 0;
    RegTxOutState=eResultNone;
}


void
EnQueue(void)
{

    UCHAR	i;
    tail += 1;
    i=tail % Q_SIZE;
    queue[i].id.SIZE=IT6802_CEC_ReadI2C_Byte(REG_FOLLOWER_RX_NUM);
    queue[i].id.HEADER=IT6802_CEC_ReadI2C_Byte(REG_RX_HEADER);
    queue[i].id.OPCODE=IT6802_CEC_ReadI2C_Byte(REG_RX_OPCODE);
    queue[i].id.OPERAND1=IT6802_CEC_ReadI2C_Byte(REG_RX_OPERAND1);
    queue[i].id.OPERAND2=IT6802_CEC_ReadI2C_Byte(REG_RX_OPERAND2);
    queue[i].id.OPERAND3=IT6802_CEC_ReadI2C_Byte(REG_RX_OPERAND3);
    queue[i].id.OPERAND4=IT6802_CEC_ReadI2C_Byte(REG_RX_OPERAND4);
    queue[i].id.OPERAND5=IT6802_CEC_ReadI2C_Byte(REG_RX_OPERAND5);
    queue[i].id.OPERAND6=IT6802_CEC_ReadI2C_Byte(REG_RX_OPERAND6);
    queue[i].id.OPERAND7=IT6802_CEC_ReadI2C_Byte(REG_RX_OPERAND7);
    queue[i].id.OPERAND8=IT6802_CEC_ReadI2C_Byte(REG_RX_OPERAND8);
    queue[i].id.OPERAND9=IT6802_CEC_ReadI2C_Byte(REG_RX_OPERAND9);
    queue[i].id.OPERAND10=IT6802_CEC_ReadI2C_Byte(REG_RX_OPERAND10);
    queue[i].id.OPERAND11=IT6802_CEC_ReadI2C_Byte(REG_RX_OPERAND11);
    queue[i].id.OPERAND12=IT6802_CEC_ReadI2C_Byte(REG_RX_OPERAND12);
    queue[i].id.OPERAND13=IT6802_CEC_ReadI2C_Byte(REG_RX_OPERAND13);
    queue[i].id.OPERAND14=IT6802_CEC_ReadI2C_Byte(REG_RX_OPERAND14);
    queue[i].id.OPERAND15=IT6802_CEC_ReadI2C_Byte(REG_RX_OPERAND15);
    queue[i].id.OPERAND16=IT6802_CEC_ReadI2C_Byte(REG_RX_OPERAND16);
//	queue[tail % Q_SIZE]. = dataX;
}




P_CEC_FRAME
DeQueue()
{
    head += 1;
    return &(queue[ head % Q_SIZE ]);
}

UCHAR
isFull()
{
    if( ( head % Q_SIZE ) == ( ( tail+1 ) % Q_SIZE ) )
    {
        return TRUE;
    }
    return FALSE;
}

UCHAR
isEmpty()
{
    if( (head % Q_SIZE) == (tail % Q_SIZE) )
    {
        return TRUE;
    }
    return FALSE;
}

UCHAR
AddQ(void)
{
    // IF buffer is full , can't put data
    if( isFull() == TRUE )
    {
        return FALSE;
    }
    // put in buffer
    EnQueue( );
    return TRUE;
}


void cecclrint( void )
{
        IT6802_CEC_WriteI2C_Byte(REG08,  B_INT_CLR|B_DBGCEC_CLR|B_CEC_SMT); 	//Reset all interrupt
        IT6802_CEC_WriteI2C_Byte(REG08, B_CEC_SMT|B_DBGCEC_CLR);
}


BYTE cec_cnt100ms(void)
{

    BYTE  T100us=0;
    BYTE uc;


#ifdef Enable_iT66121
        iT66121CECchipEnable();
#endif

//#ifdef
    //verify 100ms for IT6802_CEC
    for(uc=0;uc<10;uc++)
    //while(1)
    {
#ifdef ENABLE_LED
        LED2=1;
        LED1=1;
#endif

        IT6802_CEC_WriteI2C_Byte(REG09,B_EN100ms_CNT);
        delay1ms(99);
        IT6802_CEC_WriteI2C_Byte( REG09,0x00);
#ifdef ENABLE_LED
        LED2=~LED2;
        LED1=~LED1;
#endif



    T100us = ( IT6802_CEC_ReadI2C_Byte(REG_MSCOUNT_H)*0x10000 + IT6802_CEC_ReadI2C_Byte(REG_MSCOUNT_M)*0x100 + IT6802_CEC_ReadI2C_Byte(REG_MSCOUNT_L) )/0x3E8;



    IT6802_CEC_PRINT(("\n------------------------------\n"));
    IT6802_CEC_PRINT(("T100us = %X\n", (int) T100us));
    IT6802_CEC_PRINT(("H=%X\n",(int) IT6802_CEC_ReadI2C_Byte(REG_MSCOUNT_H)));
    IT6802_CEC_PRINT(("M=%X\n",(int) IT6802_CEC_ReadI2C_Byte(REG_MSCOUNT_M)));
    IT6802_CEC_PRINT(("L=%X\n",(int) IT6802_CEC_ReadI2C_Byte(REG_MSCOUNT_L)));



    }


    return T100us;
}

void cecConfigure( BYTE LogicAdr, BYTE TimeUnit, BYTE CECRst, BYTE EnNAck )
{

    //TimeUnit=timeunit;

    BYTE uc;
    Myself_LogicAdr=LogicAdr;

    if( RxSelf_Dis==1 )
#if 0
            IT6802_CEC_WriteI2C_Byte(REG09, B_RXSELF_SEL); 	//Reg_RxSelf_Dis[1]
#else
            IT6802_CEC_WriteI2C_Byte(REG09, B_RXSELF_SEL|B_REGION_SEL); 	//Reg_RxSelf_Dis[1]
#endif
        else
            IT6802_CEC_WriteI2C_Byte(REG09, 0x00);

    IT6802_CEC_WriteI2C_Byte(0x0B,0x14); 					//REG_Data_Min[7:0]
//	IT6802_CEC_WriteI2C_Byte(REG_DATA_MIN, 0x00); 		//REG_Data_Min[7:0]
        IT6802_CEC_WriteI2C_Byte(REG_TIMER_UNIT, TimeUnit); 	//REG_Timer_unit[7:0]
        IT6802_CEC_WriteI2C_Byte(REG_CEC_TARGET_ADDR, LogicAdr); 	//REG_Logical_addr[3:0]

        //IT6802_CEC_WriteI2C_Byte(0x07, 0x37); 		//Debug_Reg_Sel

        if( CECRst==1 )
            IT6802_CEC_WriteI2C_Byte(REG08,B_CEC_RST|B_CEC_SMT); 	//REG_CEC_Rst
    else
            IT6802_CEC_WriteI2C_Byte(REG08,B_CEC_SMT); 	//REG_CEC_Rst


    uc=IT6802_CEC_ReadI2C_Byte(REG09);
        if( EnNAck==1 )
            IT6802_CEC_WriteI2C_Byte(REG09, uc|B_NACK_EN); 	//REG_NAck_En

        cecclrint();

    IT6802_CEC_PRINT(("\nSet Configuration : LogicAdr=%X, TimeUnit=%bX, CECRst=%bX, EnNAck=%bX\n\n", (int) LogicAdr, (int) TimeUnit, (int) CECRst,(int)  EnNAck));
}


BYTE cecfirerdy( void )
{
BYTE reg0x44;
BYTE ReadyFire, BusFree;

     reg0x44 = IT6802_CEC_ReadI2C_Byte(REG_SYS_STATUS);
     ReadyFire = (reg0x44&B_READY_FIRE)>>6;
    BusFree = (reg0x44&B_BUS_STATUS)>>1;

        if( ReadyFire==1 && BusFree==1 )  // Ready Fire && Bus Free
            return TRUE;
        else
            {
                //printf("IT6802_CEC[44]SYS_STATUS=%X\n",reg0x44);

                return FALSE;
            }

}


void IT6802_ResetCEC(void)
{

    unsigned char u8_LogicAddress=DEVICE_ID_TV;
    unsigned char i = 0;

    #ifdef Enable_IT6802_CECB0
    IT6802_CEC_WriteI2C_Byte(0xF8,0xC3);
    IT6802_CEC_WriteI2C_Byte(0xF8,0xA5);
    #endif

    // initial Queue for save CEC Rx command
    initQ();

    // Set Logic Address and 100ms timer for IT6802_CEC CEC function	 for IT6802
    cecConfigure(u8_LogicAddress,timeunit,FALSE,FALSE);


    // Reg07[6]='1' for enable CEC interrupt pin to MCU
    IT6802_CEC_WriteI2C_Byte(REG07,IT6802_CEC_ReadI2C_Byte(REG07)|0x40);

    //2013-0509 change it6802 CEC default value for enable ALL interrupt mask
    IT6802_CEC_WriteI2C_Byte(REG06, 0x00);


    #ifdef Enable_IT6802_CECB0
        // Reg0F[4] IO pull up enable
        IT6802_CEC_WriteI2C_Byte(REG0F,0x50);
    #endif
//xxxxx 2014-0128
    //clear value for CEC list
    //for (i = 0; i < 15; i++)
    //{
    //    CECList[i].Active=FALSE;
    //    CECList[i].PhyicalAddr1=0x00;
    //    CECList[i].PhyicalAddr2=0x00;
    //}

    //IT6802_CECPhysicalAddressReset();
    //[HD50_PW780] Hank disable...
    //CEC_RegisterCallBack();
//xxxxx

}


void IT6802_CECInit(void)
{

    unsigned char u8_LogicAddress=DEVICE_ID_TV;
    unsigned char i = 0;

    #ifdef Enable_IT6802_CECB0
    IT6802_CEC_WriteI2C_Byte(0xF8,0xC3);
    IT6802_CEC_WriteI2C_Byte(0xF8,0xA5);
    #endif

    // initial Queue for save CEC Rx command
    initQ();

    // FIX_ID_051 do not use MCU calibrated timeunit
    //// get 100ms timer from 8051 to synchronous IT6802_CEC capture timer
    // timeunit=(unsigned char)cec_cnt100ms();
    //~FIX_ID_051

    // Set Logic Address and 100ms timer for IT6802_CEC CEC function	 for IT6802
    cecConfigure(u8_LogicAddress,timeunit,FALSE,FALSE);


    // Reg07[6]='1' for enable CEC interrupt pin to MCU
    IT6802_CEC_WriteI2C_Byte(REG07,IT6802_CEC_ReadI2C_Byte(REG07)|0x40);

    //2013-0509 change it6802 CEC default value for enable ALL interrupt mask
    IT6802_CEC_WriteI2C_Byte(REG06, 0x00);


    #ifdef Enable_IT6802_CECB0
        // Reg0F[4] IO pull up enable
        IT6802_CEC_WriteI2C_Byte(REG0F,0x50);
    #endif
//xxxxx 2014-0128
    //clear value for CEC list
    for (i = 0; i < 15; i++)
    {
        CECList[i].Active=FALSE;
        CECList[i].PhyicalAddr1=0x00;
        CECList[i].PhyicalAddr2=0x00;
    }

    IT6802_CECPhysicalAddressReset();

    IT6802CEC_RegisterCallBack(&CECReceivePowerOff_Callback, &CECReceivePowerOn_Callback);
//xxxxx

}


void IT6802_CECReceiver(void)
{



#ifdef Enable_IT6802_CECB0
    //	#define	B_CEC_INT				0x40
    //	#define	B_TX_INT				[5]	// transfied  10 bit data per block
    //	#define	B_RXDONE_INT			[4]	// received a cec frame. (header block + data block) --> rx ok
    //	#define	B_TXDONE_INT			[3]	// transfer 1 cec frame. (header block + data block) 	--> tx ok
    //	#define	B_RXFAIL_INT				[2]	// received a cec frame fail !!! --> rx fail
    //	#define	B_RX_INT				[1]	// received 10 bit data per block
    //	#define	B_TXFAIL_INT				[0]	// transfied a cec frame fail !!! --> tx fail


//if(CEC_INT_Pin)
{

    CECInt   = (IT6802_CEC_ReadI2C_Byte(REG_INT_STATUS) & 0x3F);

    //IT6802_CEC_PRINT(("CECint = %X\n",CECInt));

        if(CECInt&B_RXDONE_INT)
        {
            AddQ();
        }

        if( CECInt & B_TXFAIL_INT)
        {
            RegTxOutState=eFail;
            IT6802_CEC_PRINT((" B_TXFAIL_INT => TX cmd %X\n",(int) OldTxCECcmd));
        }

        if( CECInt & B_TXDONE_INT)
        {
            RegTxOutState=eSucessful;
            IT6802_CEC_PRINT((" B_TXDONE_INT => TX cmd %X\n",(int) OldTxCECcmd));
        }

    IT6802_CEC_WriteI2C_Byte(REG_INT_STATUS,CECInt);	// Write '1' clear interrupt
}
#else

    // [0]:TxInt, [1]:RxInt, [2]:RxFail [3]:TxDone [4]:RxDone
    CECInt   = IT6802_CEC_ReadI2C_Byte(REG_INT_STATUS);

    if(CECInt&B_CEC_INT)
    {

        if( CECInt & B_RX_INT )
        {


        }

        if(CECInt&B_RXDONE_INT)
        {
            AddQ();
            #ifdef ENABLE_LED
                LED3=~LED3;
            #endif
        }


        if(CECInt&B_RXFAIL_INT)
        {
            //printf((" Result => RX Fail\n"));
            #ifdef ENABLE_LED
                LED2=~LED2;
            #endif
        }

        if( CECInt & B_TXFAIL_INT)
        {
            RegTxOutState=eFail;
            IT6802_CEC_PRINT((" B_TXFAIL_INT => TX cmd %X\n",(int) OldTxCECcmd));
        }

        if( CECInt & B_TXDONE_INT)
        {
            RegTxOutState=eSucessful;
            IT6802_CEC_PRINT((" B_TXDONE_INT => TX cmd %X\n",(int) OldTxCECcmd));
        }

        #ifdef ENABLE_LED
            LED1=~LED1;
        #endif

        cecclrint();	//Reg08[0]='1'->'0' for clear CEC int
    }

#endif
}

void IT6802_CECCheckFollower(void)	//for double check follower address that is correct address !!
{
    BYTE uc;

    if(Myself_LogicAdr==FollowerAddr)
    {
        FollowerAddr=0;		// default 	FollowerAddr=0 , otherwise is FollowerAddr=5 ==> AVR
        for(uc=1;uc<16;uc++)
        {
            if(CECList[uc].Active==TRUE)
            {
                FollowerAddr=uc;
            }
        }
    }
}

void IT6802_CECTransmitter(void)
{
    BYTE i;
    switch(CECTxState)
    {
        case sCECTransfer:
        {

            //if( cecfirerdy()==TRUE)// for IT6802_CEC old  chip
            if(RegTxOutState!=ePending)		// for IT6802_CECB0 chip only
            {
                IT6802_CEC_WriteI2C_Byte(REG_CEC_OUT_NUM, (TxCmdBuf.array[0]));  	//OutNum

                for(i=0;i<(TxCmdBuf.id.SIZE)+1;i++)
                            IT6802_CEC_WriteI2C_Byte(REG_TX_HEADER+i, TxCmdBuf.array[i+1]);  	//Header



                //FireCmd --> Reg08[7]= 1 -> 0 !!!
                    IT6802_CEC_WriteI2C_Byte(REG08, B_FIRE_FRAME|B_CEC_SMT|B_DBGCEC_CLR);
                    IT6802_CEC_WriteI2C_Byte(REG08, B_CEC_SMT|B_DBGCEC_CLR);

//			IT6802_CEC_WriteI2C_Byte(REG08, B_CEC_SMT);         	//FireCmd
//			IT6802_CEC_WriteI2C_Byte(REG08, B_FIRE_FRAME|B_CEC_SMT);         	//FireCmd




                OldTxCECcmd= IT6802_CEC_ReadI2C_Byte(REG_TX_OPCODE);
                RegTxOutState=ePending;		// for IT6802_CECB0 chip only

                //enable timer out for avoid no interrupt occur
//0503 not yet!!!				CreatTimerTask(eCECTxTimeOut,3000,SysTimerTask_OnceRun);


                CECTxState=sCECCheckResult;
            }
//0503 not yet!!!			else if(TimeOutCheck(eCECTxTimeOut)==TRUE)
//0503 not yet!!!			{
//0503 not yet!!!				CECTxState=sCECReceiverFail;
//0503 not yet!!!				RegTxOutState=eFail;

//0503 not yet!!!				IT6802_CEC_PRINT(("111111111111111111111111111111111111111111111\n"));
//0503 not yet!!!				IT6802_CEC_PRINT((" TimeOutCheck(eCECTxTimeOut)==TRUE\n"));
//0503 not yet!!!				IT6802_CEC_PRINT(("111111111111111111111111111111111111111111111\n"));

//0503 not yet!!!			}

        }
        break;


        case sCECCheckResult:
        {

            if(RegTxOutState==eFail)					// for IT6802_CECB0 chip only
                CECTxState=sCECReceiverFail;
            else if(RegTxOutState==eSucessful)		// for IT6802_CECB0 chip only
                CECTxState=sCECOk;


//0503 not yet!!!			if(TimeOutCheck(eCECTxTimeOut)==TRUE)
//0503 not yet!!!			{
//0503 not yet!!!				CECTxState=sCECReceiverFail;
//0503 not yet!!!				RegTxOutState=eFail;
//0503 not yet!!!				IT6802_CEC_PRINT(("222222222222222222222222222222222222222222222\n"));
//0503 not yet!!!				IT6802_CEC_PRINT((" TimeOutCheck(eCECTxTimeOut)==TRUE\n"));
//0503 not yet!!!				IT6802_CEC_PRINT(("222222222222222222222222222222222222222222222\n"));
//0503 not yet!!!
//0503 not yet!!!			}
#if 0


            u8_t	uc;

            //uc=(IT6802_CEC_ReadI2C_Byte(REG_SYS_STATUS)&B_OUT_STATUS);
            uc=(IT6802_CEC_ReadI2C_Byte(REG_SYS_STATUS));

            if((uc&B_ReceiverFail)==B_ReceiverFail)
            {
                IT6802_CEC_PRINT((" B_ReceiverFail[%X] => TX cmd %bX\n",uc,OldTxCECcmd));
                CECTxState=sCECReceiverFail;
            }
            else if((uc&B_ReceiverNACK)==B_ReceiverNACK)
            {
                IT6802_CEC_PRINT((" B_ReceiverNACK[%X] => TX cmd %bX\n",uc,OldTxCECcmd));
                //IT6802_CEC_PRINT(("Receiver NACK\n");
                //CECTxState=sCECReceiverNack;
            }
            else if((uc&B_ReceiverRetry)==B_ReceiverRetry)
            {
                //IT6802_CEC_PRINT(("Receiver Retry\n");
            }
            else
            {
                //IT6802_CEC_PRINT(("Receiver ACK\n");
                if( IT6802_CEC_ReadI2C_Byte(REG_INITIATOR_TX_NUM)==TxCmdBuf.SIZE)
                {
                    IT6802_CEC_PRINT((" sCECOk [%X]=> TX cmd %bX\n",uc,OldTxCECcmd));
                    CECTxState=sCECOk;
                }
//				else
//					{
//						MHLRX_DEBUG_PRINTF(("TX size error => Reg Tx Num=%X , TxcmdBufSize=%bX\n",
//							IT6802_CEC_ReadI2C_Byte(REG_INITIATOR_TX_NUM),TxCmdBuf.SIZE));
//					}
            }
#endif
        }
        break;


        case sCECReceiverNack:
        case sCECOff:
        case sCECReceiverFail:
        case sCECNone:
        case sCECOk:
            break;

    }
}


HECStateField 	HEC_Status;		// 1 byte
HECSupportField	HEC_Support;	// 2 byte
HECActivationField	HEC_Activation;	// 2 byte


#if 0
void HECTransfer(eCDC_CMD CDC_Opcode)
{

     TxCmdBuf.HEADER=((Myself_LogicAdr<<4)+0x0F);				// logice address + 0x0F
     TxCmdBuf.OPCODE=eCDC;					// 0xF8
     TxCmdBuf.OPERAND1=Myself_PhyAdr1;		// Initiator Phy Address
     TxCmdBuf.OPERAND2=Myself_PhyAdr2;		// Initiator Phy Address
     TxCmdBuf.OPERAND3=CDC_Opcode;		// CDC Opcode



     switch(CDC_Opcode)
     {
        case eHEC_InquirState:
            {
                 TxCmdBuf.OPERAND4=FollowerPhyAdr1;
                 TxCmdBuf.OPERAND5=FollowerPhyAdr2;
                 TxCmdBuf.OPERAND6=FollowerPhyAdr3;
                 TxCmdBuf.OPERAND7=FollowerPhyAdr4;
                TxCmdBuf.SIZE=9;
            }
            break;
        case eHEC_ReportState:
            {
                 TxCmdBuf.OPERAND4=FollowerPhyAdr1;
                 TxCmdBuf.OPERAND5=FollowerPhyAdr2;
//			 	TxCmdBuf.OPERAND6=HEC_Status;			// 1 byte
//			 	TxCmdBuf.OPERAND7=HEC_Support;		// 2 byte
//			 	TxCmdBuf.OPERAND8=HEC_Support;
//			 	TxCmdBuf.OPERAND9=HEC_Activation;		// 2 byte
//			 	TxCmdBuf.OPERAND10=HEC_Activation;
                TxCmdBuf.SIZE=12;


            }
            break;
        case eHEC_SetStateAdjacent:
            {

            }
            break;
        case eHEC_SetState:
            {

            }
            break;
        case eHEC_RequestDeactivation:
            {

            }
            break;
        case eHEC_NotifyAlive:
            {

            }
            break;
        case eHEC_Discover:
            {

            }
            break;
        case eHEC_HPDSetState:
            {

            }
            break;
        case eHEC_HPDReportState:
            {

            }
            break;

     }



     CECTxState=sCECTransfer;

}
#endif

// for IT6802+iT6623void CEC_InactiveSource(BYTE port)
// for IT6802+iT6623{
// for IT6802+iT6623
// for IT6802+iT6623#ifdef 	_CEC_DEVICE_AVR_
// for IT6802+iT6623		CECCmdSet(DEVICE_ID_TV,eInactiveSource,txphyadr[0],txphyadr[1]);
// for IT6802+iT6623#else
// for IT6802+iT6623	if(rxcurport==port)
// for IT6802+iT6623		CECCmdSet(DEVICE_ID_TV,eInactiveSource,rxphyadr[rxcurport][0],rxphyadr[rxcurport][1]);
// for IT6802+iT6623#endif
// for IT6802+iT6623}

// for IT6802+iT6623void CEC_RoutingChange(BYTE port)
// for IT6802+iT6623 {
// for IT6802+iT6623	//when Source un-plug(w/o +5v ) then broadcast AVR routing change.
// for IT6802+iT6623
// for IT6802+iT6623	BYTE	adr0,adr1;
// for IT6802+iT6623
// for IT6802+iT6623 	if(Myself_LogicAdr==DEVICE_ID_AUDIO)
// for IT6802+iT6623 	{
// for IT6802+iT6623
// for IT6802+iT6623		if(port<4)
// for IT6802+iT6623		{
// for IT6802+iT6623	 		adr0=rxphyadr[rxcurport][0];
// for IT6802+iT6623	 		adr1=rxphyadr[rxcurport][1];
// for IT6802+iT6623			rxcurport=port;
// for IT6802+iT6623			CECCmdSet(DEVICE_ID_BROADCAST,eRoutingChange,adr0,adr1);
// for IT6802+iT6623		}
// for IT6802+iT6623 	}
// for IT6802+iT6623 }


 void CECCmdSet(u8_t Follower,eCEC_CMD TxCommand,u8_t cOperand1,u8_t cOperand2)
{
#ifdef DEBUG_IT6802_CEC
    BYTE i;
#endif
     //FollowerAddr=Follower;

     IT6802_CEC_PRINT((" follower =%X op1=%bX op2=%bX\n",(int) Follower,(int) cOperand1,(int) cOperand2));


     //TxCmdBuf.HEADER=((Myself_LogicAdr<<4)+FollowerAddr);				// logice address + follower address
     TxCmdBuf.id.HEADER=((Myself_LogicAdr<<4)+Follower);				// logice address + follower address
     TxCmdBuf.id.OPCODE=TxCommand;
     TxCmdBuf.id.OPERAND1=cOperand1;			// Physical Address 1
     TxCmdBuf.id.OPERAND2=cOperand2;			// Physical Address 2
    TxCmdBuf.id.SIZE=2;

     switch(TxCommand)
    {

        case eFeatureAbort:
                     TxCmdBuf.id.SIZE=4;
                break;


        case ePollingMessage:
#ifdef DEBUG_IT6802_CEC
                IT6802_CEC_PRINT(("PollingMessage Header=%X\n",TxCmdBuf.id.HEADER));
#endif
                 TxCmdBuf.id.SIZE=1;
                 break;

        case eTextViewOn:			//			=0x0D,	// follower:TV
        case eImageViewOn:			//			=0x04,	// follower:TV, switch	--> Broadcst
                break;

        case eActiveSource:			//			=0x82,	// follower:TV, switch	--> Broadcst , Directly address
                     TxCmdBuf.id.SIZE=4;
                break;

        // Routing Control Feature
        case eRoutingChange:			//			=0x80,	// follower:TV, switch	--> Broadcst
                     TxCmdBuf.id.OPERAND3=rxphyadr[rxcurport][0];			// Physical Address 1
                     TxCmdBuf.id.OPERAND4=rxphyadr[rxcurport][1];			// Physical Address 2
                     TxCmdBuf.id.SIZE=6;
                    break;
        case eRoutingInformation:		//			=0x81,	// follower:switch		--> Broadcst
                     TxCmdBuf.id.SIZE=4;
                    break;
        case eRequestActiveSource:		//			=0x85,	// follower:switch		--> Broadcst 	, AVR to request active source when Audio ocntrol feature is active
                    break;
        case eSetStreamPath:			//			=0x86,	// follower:switch		--> Broadcst
                     TxCmdBuf.id.SIZE=4;
                    break;
        case eInactiveSource:			//			=0x9D,	//
                    //reply TV that it has no video to be presented !!
                     TxCmdBuf.id.SIZE=4;
                    break;
        case eStandBy:				//			=0x36,	// follower:All			--> Broadcst
                    break;
        // System Information Feature
        case eCECVersioin:			//			=0x9E,
                     TxCmdBuf.id.SIZE=3;
                    break;
        case eGetCECVersion:			//			=0x9F,
        case eGivePhysicalAddress:		//			=0x83,	// follower:All
        case eGetMenuLanguage:		//			=0x91,	// follower:TV
        case eReportPhysicalAddress:	//			=0x84,	// follower:TV
                {
                     TxCmdBuf.id.SIZE=5;

                    switch(Myself_LogicAdr)
                    {
                        case DEVICE_ID_TV:
                                TxCmdBuf.id.OPERAND3=0;
                                break;

                        case DEVICE_ID_RECORDING1:
                        case DEVICE_ID_RECORDING2:
                        case DEVICE_ID_RECORDING3:
                                 TxCmdBuf.id.OPERAND3=1;
                                break;

                        case DEVICE_ID_RESERVED1:
                        case DEVICE_ID_RESERVED2:
                        case DEVICE_ID_FREEUSE:
                        case DEVICE_ID_BROADCAST:
                                 TxCmdBuf.id.OPERAND3=2;
                                break;

                        case DEVICE_ID_TUNER1:
                        case DEVICE_ID_TUNER2:
                        case DEVICE_ID_TUNER3:
                        case DEVICE_ID_TUNER4:
                                 TxCmdBuf.id.OPERAND3=3;
                                break;

                        case DEVICE_ID_PLAYBACK1:
                        case DEVICE_ID_PLAYBACK2:
                        case DEVICE_ID_PLAYBACK3:
                                 TxCmdBuf.id.OPERAND3=4;
                                break;

                        case DEVICE_ID_AUDIO:
                                 TxCmdBuf.id.OPERAND3=5;
                                break;
                        default:

                                 TxCmdBuf.id.OPERAND3=6;	//CEC switch
                                 TxCmdBuf.id.OPERAND3=7;	//Video processor
                                break;
                    }


                }
                break;
        case eSetMenuLanguage:		//			=0x32,	// follower:All,		Initiator:TV

        // Vendor Specific Commands Feature
        case eDeviceVendorID:		//			=0x87,
                TxCmdBuf.id.OPERAND1=0;	//CEC switch
                TxCmdBuf.id.OPERAND2=0;	//CEC switch
                TxCmdBuf.id.OPERAND3=0;	//CEC switch
                 TxCmdBuf.id.SIZE=5;
                break;
        case eGiveDeviceVendorID:		//			=0x8C,
        case eVendorCommand:		//			=0x89,
        case eVendorCommandWithID:	//			 =0xA0,
                break;
        case eReportPowerStatus:
                 TxCmdBuf.id.SIZE=3;
                break;


        // other
        case ePlay:					//			=0x41,
        case eUserPressed:			//			=0x44,
                 TxCmdBuf.id.SIZE=3;
                break;
        case eUserReleased:			//			=0x45,
        case eAudioModeRequest:		//			=0x70,
                break;
        case eGiveAudioStatus:		//			=0x71,
                break;


        case eSetSystemAudioMode:	// <MS> <0x72 Set Audio Mode> <System Audio On / Off >
                                    // 							<--  <MS> <0x70> <Phy Adr>
                 TxCmdBuf.id.SIZE=3;
                break;


        case eReportAudioStatus:		// <MS> <0x7A Report Audio Status> <Volume and Mute status>
                                    //							<--  <MS> <0x71 Give Audio Status>

        case eSystemAudIoModeStatus:	// <MS> <0x7E System AudIo Mode Status> <Audio On / Off>
                                    //							<--  <MS> <0x7D Give System Audio Mode Status>
                 TxCmdBuf.id.SIZE=3;
                break;


        case eRequestAudioDescriptor:{
             TxCmdBuf.id.OPERAND1=0x01;	// PCM
             TxCmdBuf.id.OPERAND2=0x02;	// AC3
             TxCmdBuf.id.OPERAND3=0x07;	// DTS
             TxCmdBuf.id.SIZE=5;
            }
            break;

        case eReportAudioDescriptor:	//			=0xA3
            //0x23,0x0F, 0x7F, 0x07,		//PCM 8CH, 32~192KHz, 16/20/24Bit
            //0x23,0x15, 0x1F, 0x38,		//AC3 6CH, 32~96KHz, 16/20/24Bit
            //0x23,0x3F, 0x07, 0xC0,		//DTS 8CH, 32~ 48K, 1536Kbps
             TxCmdBuf.id.OPERAND1=0x0F;
             TxCmdBuf.id.OPERAND2=0x7F;
             TxCmdBuf.id.OPERAND3=0x07;

             TxCmdBuf.id.OPERAND4=0x15;
             TxCmdBuf.id.OPERAND5=0x1F;
             TxCmdBuf.id.OPERAND6=0x38;

             TxCmdBuf.id.OPERAND7=0x3F;
             TxCmdBuf.id.OPERAND8=0x07;
             TxCmdBuf.id.OPERAND9=0xC0;

             TxCmdBuf.id.SIZE=11;
            break;



        case eGiveSystemAudIoModeStatus:	//			=0x7D,

        case eRemoteButtonDown:		//			=0x8A,
        case eRemoteButtonUp:		//			=0x8B,
            break;

        // ACR
        case eInitiateARC:				//			=0xC0,
        case eReportARCInitiated:		//			=0xC1,
        case eReportARCTerminated:	//			=0xC2,
        case eRequestARCInitiation:		//			=0xC3,
        case eRequestARCTermination:	//			=0xC4,
        case eTerminateARC:			//			=0xC5,
        break;

    }

#ifdef DEBUG_IT6802_CEC

IT6802_CEC_PRINT(("CECCmdSet --> Tx "));

for(i=0;i<((TxCmdBuf.array[0])+1);i++)
{
    IT6802_CEC_PRINT(("TXCmd[%X]=%bX ",i,TxCmdBuf.array[i]));
    if((i%6)==5) IT6802_CEC_PRINT(("\n"));
}
IT6802_CEC_PRINT(("\n\n\n"));

#endif


     CECTxState=sCECTransfer;
}





//11.1.2-1 Routing Control  				[AVR: 11.2.2-1 ~ 11.2.2-4]
//11.1.3-1 System Standby				[AVR: 11.2.3-1 ~ 11.2.3-3]
//11.2.9-1 Vendor Specific Commands 	[AVR: 11.2.9-1 ~ 11.2.9-3]
//11.2.12-1 Device Menu Control			[AVR: 11.2.12-1 ~ 11.2.12-7]
//11.2.13-1 Remote Control Pass Through	[AVR: 11.2.13-1]
//11.3.1-1 CEC Switch					[AVR: 11.3.1-1 ~ 11.3.2-1]
//12-1 Invalid Massage
void CECDecoder(void)
{
    u8_t	uc;
    u8_t index;
    BOOL bInvalidCmd=FALSE;
    u8_t i;

    // IF buffer is empty , can't get data
    if( isEmpty() == TRUE )
    {
        return;
    }
    pCECRxBuf = DeQueue();

#ifdef DEBUG_IT6802_CEC
    IT6802_CEC_PRINT(("\n"));
    for(i=0;i<((pCECRxBuf->array[0])+1);i++)
    {
        IT6802_CEC_PRINT(("RxCmd[%X]=%bX ",i,pCECRxBuf->array[i]));
        if((i%6)==5) IT6802_CEC_PRINT(("\n"));
    }
    IT6802_CEC_PRINT(("\n"));
#endif

    if(pCECRxBuf->id.SIZE==1)
    {
            if(Myself_LogicAdr==(pCECRxBuf->id.HEADER &0x0F))
                //switch_PollingMessage(sReportPhyAdr);		<== bug , when TV polling device
                CECCmdSet(DEVICE_ID_BROADCAST, eReportPhysicalAddress,txphyadr[0],txphyadr[1]);
            return;
    }

    // Invalid command Check
    for(index=0;index<SizeOfLookUpTable_CEC;index++)
    {
        if((pCECRxBuf->id.OPCODE)==LookUpTable_CEC[index].cmd)
            break;
    }


    IT6802_CEC_PRINT(("SizeOfLookUpTable_CEC=%X\n", index));


    i=(pCECRxBuf->id.HEADER & 0x0F);	// for Identified who need to deal with the op code

    IT6802_CEC_PRINT(("FollowerAddr=%X\n", i));



    if(index!=(SizeOfLookUpTable_CEC-1))
    {
        switch(LookUpTable_CEC[index].header)
        {
            case eDirectly:
                if(i==DEVICE_ID_BROADCAST)
                {
                    bInvalidCmd=TRUE;
#ifdef DEBUG_IT6802_CEC
                    IT6802_CEC_PRINT(("!!!Invalid Direct Cmd !!!\n"));
#endif
                }
                break;

            case eBroadcast:
                if(i!=DEVICE_ID_BROADCAST)
                {
                    bInvalidCmd=TRUE;
#ifdef DEBUG_IT6802_CEC
                    IT6802_CEC_PRINT(("!!!Invalid Broadcast Cmd !!!\n"));
#endif
                }

                break;

            case eBoth:
                break;
        }

        if((LookUpTable_CEC[index].size)!=(pCECRxBuf->id.SIZE))
        {
            bInvalidCmd=TRUE;
#ifdef DEBUG_IT6802_CEC
            IT6802_CEC_PRINT(("!!!Invalid Size !!!\n"));
#endif
        }

    }


    FollowerAddr=(pCECRxBuf->id.HEADER & 0xF0)>>4;	// Get initator logic address for transfer CEC cmd
    IT6802_CEC_PRINT(("initator addr =%X\n", FollowerAddr));

    // CEC command  decode handler
    switch( pCECRxBuf->id.OPCODE)
    {
//=== 12.3 Feature Abort  ====
        case eFeatureAbort :	// <0x00> ,
#ifdef DEBUG_IT6802_CEC
                IT6802_CEC_PRINT(("Receive !!!Feature Abort !!!\n"));
#endif

                //{
                //	uc=(pCECRxBuf->id.HEADER & 0xf0)>>4;	 //get initator logic address
                //	{
                //		CECCmdSet(uc, pCECRxBuf->id.OPERAND1,0,0);	 //shall follow operand1[op code]  to transfer CEC cmd again
                //	}
                //}
                break;


//=== 12.4  Abort  ====
        case eAbort :		// <MS><0xff> , //example: [0x45 0xff] playback device to AVR
                        //for CEC ATC 8-x , 9-x test only, shall respond with a <Feature Abort> message
                {
                    IT6802_CEC_PRINT(("Receive !!!About !!!\n"));
                    uc=(pCECRxBuf->id.HEADER & 0xf0)>>4;		// get follower logic address
                    //if(Myself_LogicAdr==uc)
                    {
                        CECCmdSet(uc, eFeatureAbort,eAbort,4);  //<MS> <featuer about> <op code> <abort reason>
                    }
                }
            break;



//=== 13.1 One Touch Play ===
        case eImageViewOn :	// <MS><0x04> , //example: [0x40 0x04] , Playback device to TV

            IT6802_CEC_PRINT(("Receive !!!Image View On !!!\n"));

            // if(Myself_LogicAdr==DEVICE_ID_TV)
            //{
            //	uc=(pCECRxBuf->id.HEADER & 0x0f);
            //
            //	if(Myself_LogicAdr==uc)
            //	{
            //		uc=(((pCECRxBuf->id.HEADER) & 0xf0)>>4);
            //		if(CECList[uc].Active==TRUE)
            //		{
            //			CECCmdSet(uc, eSetStreamPath,CECList[uc].PhyicalAddr1,CECList[uc].PhyicalAddr2);
            //		}else
            //			{
            //				IT6802_CEC_PRINT(("CECList[uc].Active -> FALSE\n");
            //			}
            //	}
            //}

            break;


        case eTextViewOn :	// <MS><0x0D>, example: [0x40 0x0D] , Playback device to TV

            IT6802_CEC_PRINT(("Receive !!!Text View On !!!\n"));
            break;


        case eActiveSource :    //<DB> <0X82> <Physical Address of initator> ,example:[0x4f 0x82 AB CD] playback device[AB CD] broadcast acitve source
            {
                IT6802_CEC_PRINT(("Receive !!!Active Source !!!\n"));


//xxxxx 2014-0128
                // disable -> if(t_IIC_Buffer.I2cRegisterMap.CEC_Power_Status==0)
                // disable -> {
                // disable -> // Receiver CEC command Queue for I2C slave protocal
                // disable -> t_IIC_Buffer.I2cRegisterMap.CEC_Power_Status=1;		// power on
                // disable -> I2C_CECAddQ(0x35);	//2011/06/09 for ACR cmd
                // disable -> }

                if(pCECRxBuf->id.SIZE==4)
                {
                    //if( Myself_LogicAdr==DEVICE_ID_AUDIO)
                    //	CECChangePort();

                    uc=(pCECRxBuf->id.HEADER & 0x0f);
                    if(Myself_LogicAdr==uc || uc==0xf)
                    {
                        if((rxphyadr[0][0]==pCECRxBuf->id.OPERAND1) && (rxphyadr[0][1]==pCECRxBuf->id.OPERAND2) )
                            {
                                rxcurport=0;
                            }
                        else
                            {
                                rxcurport=1;
                            }
                        IT6802_CEC_PRINT(("===CEC Active Port = %X ===\n",rxcurport));
                        pm_CECcallbacks->CECReceivePowerOn_Callback(rxcurport);

                        uc=(((pCECRxBuf->id.HEADER) & 0xf0)>>4);	//get initator logice address
                        CECList[uc].Active=TRUE;
                        CECList[uc].PhyicalAddr1=pCECRxBuf->id.OPERAND1;
                        CECList[uc].PhyicalAddr2=pCECRxBuf->id.OPERAND2;
                    }

                }

//xxxxx


            }
            break;

//=== 13.2.1 Routing Control ====
        case eRoutingChange :		//<DB><0x80><Original Adr><New Adr>  , size=6		// send by switch device to manually change port
                                //reply new address <routine information> to indicate its current active route.
            IT6802_CEC_PRINT(("Receive !!!Routing Change> !!!\n"));

            if( txphyadr[0]==pCECRxBuf->id.OPERAND3 && txphyadr[1]==pCECRxBuf->id.OPERAND4 ) {
                    CECCmdSet(DEVICE_ID_BROADCAST, eRoutingInformation,rxphyadr[rxcurport][0],rxphyadr[rxcurport][1]);
            }

//			for(uc=0;uc<4;uc++)
//			{
//				if((rxphyadr[uc][0]==pCECRxBuf->id.OPERAND3) && (rxphyadr[uc][1]==pCECRxBuf->id.OPERAND4) )
//				{
//					if(uc<3)
//					{
//						if(It6633ChangePort(uc)==TRUE)
//						{
//							CECCmdSet(DEVICE_ID_BROADCAST, eRoutingInformation,0,0);
//						}
//						CAT6023InputPortSet(0);
//					}else if(uc==3)
//					{
//						if(CAT6023InputPortSet(1)==TRUE)
//						{
//							CECCmdSet(DEVICE_ID_BROADCAST, eRoutingInformation,0,0);
//						}
//					}
//				}
//
//			}

//			IT6802_CEC_PRINT(("Original Adr = %X,%bX,%bX,%bX\n", (pCECRxBuf->id.OPERAND1 & 0xF0)>>4, pCECRxBuf->id.OPERAND1 & 0x0F,
//			(pCECRxBuf->id.OPERAND2 & 0xF0)>>4, pCECRxBuf->id.OPERAND2 & 0x0F);
//			IT6802_CEC_PRINT(("     New Adr = %X,%bX,%bX,%bX\n", (pCECRxBuf->id.OPERAND3 & 0xF0)>>4, pCECRxBuf->id.OPERAND3 & 0x0F,
//			(pCECRxBuf->id.OPERAND4 & 0xF0)>>4, pCECRxBuf->id.OPERAND4 & 0x0F);
//
//			if( txphyadr[0]==pCECRxBuf->id.OPERAND3 && txphyadr[1]==pCECRxBuf->id.OPERAND4 )
//			{
//				if(It6633ChangePort(rxcurport)==TRUE)
//					CAT6023InputPortSet(0);
//
//				CECCmdSet(DEVICE_ID_BROADCAST, eRoutingInformation,rxphyadr[rxcurport][0],rxphyadr[rxcurport][1]);
//			}
            break;
        case eRoutingInformation :	//<DB> <0x81> <Physical Address> ,example:[0x5F 0x81 AB CD]  AVR get active route below the switch


            IT6802_CEC_PRINT(("Receive !!!Rounting Information !!!\n"));

            if( txphyadr[0]==pCECRxBuf->id.OPERAND1 && txphyadr[1]==pCECRxBuf->id.OPERAND2 )
                CECCmdSet(DEVICE_ID_BROADCAST, eRoutingInformation,rxphyadr[rxcurport][0],rxphyadr[rxcurport][1]);
            else
                IT6802_CEC_PRINT(("Invalid -->eRoutingInformation\n"));

            break;

        case eInactiveSource :    	//<MS> <0x9D> <Physical Address>		example:<0x40> <0x9D> <AB> <CD>
            {
                                        // It has no video to be presented to the user, or is goning into standby state.
                    IT6802_CEC_PRINT(("Receive !!!Inactive Source !!!\n"));

                //xxxxx 2014-0128
                    uc=(pCECRxBuf->id.HEADER & 0x0f);
                    if(Myself_LogicAdr==uc)
                    {
                        uc=(((pCECRxBuf->id.HEADER) & 0xf0)>>4);	//get initator logice address

                        if(CECList[uc].Active==TRUE)
                        {

                            IT6802_CEC_PRINT(("CECList[%X].Active -> FALSE\n",uc));
                            CECList[uc].Active=FALSE;

                            for(uc=1;uc<15;uc++)
                            {
                                if(CECList[uc].Active==TRUE)
                                {
                                    break;
                                }
                            }

                            if(uc==15)
                            {
                                IT6802_CEC_PRINT(("No CEC device active\n"));
                                pm_CECcallbacks->CECReceivePowerOff_Callback();
                            }
                            else
                            {
                                 //Auto change to active source !!!
                                if(CECList[uc].Active==TRUE)
                                 {
                                    if((rxphyadr[0][0]==CECList[uc].PhyicalAddr1) && (rxphyadr[0][1]==CECList[uc].PhyicalAddr2 ))
                                    {
                                        rxcurport=0;
                                    }
                                    else
                                    {
                                        rxcurport=1;
                                    }
                                    IT6802_CEC_PRINT(("===CEC Active Port = %X ===\n",rxcurport));
                                    pm_CECcallbacks->CECReceivePowerOn_Callback(rxcurport);
                                }
                            }
                        }
                    }
                //xxxxx


                    break;
                }


        case eRequestActiveSource :	// <DB> <0x85> , need to reply <active source> by CEC active device
            IT6802_CEC_PRINT(("Receive !!!Request Active Source !!!\n"));
            {
                //uc=(pCECRxBuf->id.HEADER & 0xf0)>>4;


                CECCmdSet(DEVICE_ID_BROADCAST, eActiveSource,txphyadr[0],txphyadr[1]);

//				if(Myself_LogicAdr==DEVICE_ID_AUDIO)
//				{
//					CECCmdSet(DEVICE_ID_BROADCAST, eActiveSource,txphyadr[0],txphyadr[1]);
//				}
//				else if( Myself_LogicAdr==DEVICE_ID_PLAYBACK1 || Myself_LogicAdr==DEVICE_ID_PLAYBACK2
//							|| Myself_LogicAdr==DEVICE_ID_PLAYBACK3)
//				{
//					//uc=(pCECRxBuf->id.HEADER & 0xf0)>>4;
//					CECCmdSet(DEVICE_ID_BROADCAST, eActiveSource,txphyadr[0],txphyadr[1]);
//				}
            }
            break;
        case eSetStreamPath :	// <DB> <0x86> <Physical Address> , example:[0x0F 0x86 AB CD] TV request a streaming path from the specified Physical Address
            {

                IT6802_CEC_PRINT(("Receive !!!Set Stream Path !!!\n"));

                if(Myself_LogicAdr==DEVICE_ID_AUDIO)
                {
#if 0
                    //fail with QD882E test ???

                    //tv send eSetStreamPath then AVR check txphyadr and reply active source by child rxphyadr
                    if((txphyadr[0]==pCECRxBuf->id.OPERAND1) && (txphyadr[1]==pCECRxBuf->id.OPERAND2) )
                    //	CECCmdSet(DEVICE_ID_BROADCAST, eActiveSource,rxphyadr[rxcurport][0],txphyadr[rxcurport][1]);
                    {
                        if(rxcurport<4)
                            //CECCmdSet(DEVICE_ID_BROADCAST, eActiveSource,rxphyadr[rxcurport][0],rxphyadr[rxcurport][1]);
                            CECCmdSet(DEVICE_ID_BROADCAST, eActiveSource,txphyadr[0],txphyadr[1]);
                    }
#else


// for IT6802+iT6623					if(t_IIC_Buffer.I2cRegisterMap.CEC_Power_Status==0)
// for IT6802+iT6623					{
// for IT6802+iT6623						// Receiver CEC command Queue for I2C slave protocal
// for IT6802+iT6623						t_IIC_Buffer.I2cRegisterMap.CEC_Power_Status=1;		// power on
// for IT6802+iT6623						I2C_CECAddQ(0x35);	//2011/06/09 for ACR cmd
// for IT6802+iT6623					}


                    //CECChangePort();

//					// Receiver CEC command Queue for I2C slave protocal
//					t_IIC_Buffer.I2cRegisterMap.CEC_Power_Status=1;		// power on
//					I2C_CECAddQ(0x86);	//2011/06/09 for ACR cmd

#endif

                }else if( Myself_LogicAdr==DEVICE_ID_PLAYBACK1 || Myself_LogicAdr==DEVICE_ID_PLAYBACK2
                            || Myself_LogicAdr==DEVICE_ID_PLAYBACK3)
                    {
                        //if(rxcurport<4)
                        //	CECCmdSet(DEVICE_ID_BROADCAST, eReportPhysicalAddress,txphyadr[0],txphyadr[1]);
                        Switch_OneTouchPlayState(sImageViewOn);
                    }

            }
            break;

//=== 13.3 System Stand by ===
        case eStandBy :	// <MS> <0X36> or <DB> <0x36> ,
            IT6802_CEC_PRINT(("nReceive !!!Standby !!!\n"));

// for IT6802+iT6623			// Receiver CEC command Queue for I2C slave protocal
// for IT6802+iT6623			t_IIC_Buffer.I2cRegisterMap.CEC_Power_Status=0;		// power off
// for IT6802+iT6623			I2C_CECAddQ(0x36);	//2011/06/09 for ACR cmd

//xxxxx 2014-0128
            pm_CECcallbacks->CECReceivePowerOff_Callback();
//xxxxx


            break;

//=== 13.6 System Information ===
        case eCECVersioin :     //<MS> <0x9E> <CEC version>
            IT6802_CEC_PRINT(("Receive !!!CEC Version =%X!!!\n",pCECRxBuf->id.OPERAND1));
            break;
        case eGetCECVersion :   	//<MS> <0x9F> , example: [0x05 0x9F] TV request AVR to reply CEC version
            IT6802_CEC_PRINT(("Receive !!!Report CEC Version !!!\n"));
            uc=(pCECRxBuf->id.HEADER & 0xf0)>>4;
            CECCmdSet(uc, eCECVersioin,0x04,0);	// 5 for Version 1.4a... 4 for Version 1.3A...0~3 for Reserved
            break;

        case eGivePhysicalAddress :    //<MS> <0x83> , example:[0x05 0x83] TV request AVR to return Physical address
            IT6802_CEC_PRINT(("Receive !!!Give Physical Address !!!\n"));

#if 0
                if(Myself_LogicAdr==DEVICE_ID_PLAYBACK1 || Myself_LogicAdr==DEVICE_ID_PLAYBACK2
                    || Myself_LogicAdr==DEVICE_ID_PLAYBACK3)
                {
                    switch_PollingMessage(sPollingMessage1);
                }
                else if(Myself_LogicAdr==DEVICE_ID_AUDIO)
                {
                    CECCmdSet(DEVICE_ID_BROADCAST, eReportPhysicalAddress,txphyadr[0],txphyadr[1]);
                }
#endif


                if(Myself_LogicAdr==DEVICE_ID_TV)
                {
                    Switch_LogicalAddressingState(sReportPhysicalAddressTransfer);
                    //CECCmdSet(DEVICE_ID_BROADCAST, eReportPhysicalAddress,0x00,0x00);	//TV always 0,0,0,0
                }
                else if(Myself_LogicAdr==DEVICE_ID_AUDIO)
                {
                    // when HPD On and EDID change  --> then Device need to polling message and report physical address
                    // when receiver eGivePhysicalAddress cmd --> then direct to response eReportPhysicalAddress !!!
                    CECCmdSet(DEVICE_ID_BROADCAST, eReportPhysicalAddress,txphyadr[0],txphyadr[1]);
                }
                else
                {
                    //switch_PollingMessage(sPollingMessage1);		// need to check !!
                    CECCmdSet(DEVICE_ID_BROADCAST, eReportPhysicalAddress,txphyadr[0],txphyadr[1]);
                }

//				else
//					{
//						//for system infomation 11.1.6-2
//						 //response <report physical addrss>
//						CECCmdSet(DEVICE_ID_BROADCAST, eReportPhysicalAddress,txphyadr[0],txphyadr[1]);
//					}

            break;
        case eReportPhysicalAddress :	// <DB> <0x84> <Physicall Adress> <Device type>, example:[0x5F 0x84 AB CD 0]

                //if(Myself_LogicAdr==DEVICE_ID_TV){
                //	//need to collect all CEC device Physical address and Device type
                //	Switch_LogicalAddressingState(sReportPhysicalAddressReceived);	//2010/12/21
                //}else

                IT6802_CEC_PRINT(("Receive !!!Report Physical Address !!!\n"));

                {
                    if((pCECRxBuf->id.HEADER & 0x0f)==0x0f)
                    {
                        uc=((pCECRxBuf->id.HEADER & 0xf0)>>4);
                        CECList[uc].PhyicalAddr1=pCECRxBuf->id.OPERAND1;
                        CECList[uc].PhyicalAddr2=pCECRxBuf->id.OPERAND2;
                        CECList[uc].Active=TRUE;
                    }

                }

            break;

        case 0x91 :	//Get Menu Language
            uc=(pCECRxBuf->id.HEADER & 0xf0)>>4;		// get follower logic address
            CECCmdSet(uc, eFeatureAbort,0x91,4);  //<MS> <featuer about> <op code> <abort reason>
            IT6802_CEC_PRINT(("Receive !!!Get Menu Language !!!\n"));
            break;

        case 0x32 :	//Set Menu Language
            uc=(pCECRxBuf->id.HEADER & 0xf0)>>4;		// get follower logic address
            CECCmdSet(uc, eFeatureAbort,0x32,4);  //<MS> <featuer about> <op code> <abort reason>
            IT6802_CEC_PRINT(("Receive !!!Set Menu Language !!!\n"));
            break;

//=== 13.7 Deck Control ===
        case 0x41 :	//Play
            uc=(pCECRxBuf->id.HEADER & 0xf0)>>4;		// get follower logic address
            CECCmdSet(uc, eFeatureAbort,0x41,4);  //<MS> <featuer about> <op code> <abort reason>
            IT6802_CEC_PRINT(("Receive !!!Play !!!\n"));
            break;

        case 0x42 :	//Deck control
            uc=(pCECRxBuf->id.HEADER & 0xf0)>>4;		// get follower logic address
            CECCmdSet(uc, eFeatureAbort,0x42,4);  //<MS> <featuer about> <op code> <abort reason>
            IT6802_CEC_PRINT(("Receive !!!Deck control !!!\n"));
            break;

        case 0x1A :	//Give Deck Status
            uc=(pCECRxBuf->id.HEADER & 0xf0)>>4;		// get follower logic address
            CECCmdSet(uc, eFeatureAbort,0x1A,4);  //<MS> <featuer about> <op code> <abort reason>
            IT6802_CEC_PRINT(("Receive !!!Give Deck Status !!!\n"));
            break;

        case 0x1B :	//Deck Status
            uc=(pCECRxBuf->id.HEADER & 0xf0)>>4;		// get follower logic address
            CECCmdSet(uc, eFeatureAbort,0x1B,4);  //<MS> <featuer about> <op code> <abort reason>
            IT6802_CEC_PRINT(("Receive !!!Deck Status !!!\n"));
            break;

//=== 13.9 Vendor Specfic Commands ===
        case 0xA0 :	//Vendor Command With ID			// Vendor Specific
            IT6802_CEC_PRINT(("Receive !!!Vendor Command With ID !!!\n"));
            break;

        case eDeviceVendorID :   	//<DB> <0x87> <Vendor ID>
            IT6802_CEC_PRINT(("Receive !!!Device Vendor ID !!!\n"));
            break;
        case eGiveDeviceVendorID :	//<MS> <0x8C>	//need to return <Device Vendor ID>
            IT6802_CEC_PRINT(("Receive !!!Give Device Vendor ID !!!\n"));
            //uc=(pCECRxBuf->id.HEADER & 0xf0)>>4;
            CECCmdSet(DEVICE_ID_BROADCAST, eDeviceVendorID,0,0);
            break;

        case 0x89 :    	//Vendor Command				// Vendor Specific
            IT6802_CEC_PRINT(("Receive !!!Vendor Command !!!\n"));
            break;
        case 0x8A :    	//Vendor Remote Button Down		// Vendor Specific
            IT6802_CEC_PRINT(("Receive !!!Vendor Remote Button Down !!!\n"));
            break;
        case 0x8B :    	//Vendor Remote Button Up		// Vendor Specific
            IT6802_CEC_PRINT(("Receive !!!Vendor Remote Button Up !!!\n"));
            break;


//=== 13.12 Device Menu Control =========
//MenuRequest
//MenuStatus

//=== 13.13 Remote Control Pass Through ===
        case eUserPressed :	//<MS> <0x44> <UI Command>
            IT6802_CEC_PRINT(("Receive !!!User Control Pressed !!!\n"));
            UICommandCode=pCECRxBuf->id.OPERAND1;		// SAVE UI command code
            break;

        case eUserReleased :	//<MS> <0X45>
            IT6802_CEC_PRINT(("Receive !!!User Control Released !!!\n"));
            CECUICommandCode(UICommandCode);		// executioin UI command code
            break;

//=== 13.14 Device Power Status ===
        case eGiveDevicePowerStatus:{	// <MS> <0x8F Give Device Power Status> --> <MS> <0x90 Report Power Status> < 0~3 >
            IT6802_CEC_PRINT(("Receive !!!Give Device Power Status !!!\n"));
            uc=(pCECRxBuf->id.HEADER & 0xf0)>>4;	//for <MS> Header , get initator logic address then reply cec cmd
            CECCmdSet(uc, eReportPowerStatus,u8_PowerStatus,0);	// 0: on , 1: standby, 2: in transition standby to on , 3: in transition on to standby
            }
            break;
        case eReportPowerStatus:
            IT6802_CEC_PRINT(("Receive !!!Report Power Status !!!\n"));
            break;


//=== 13.15.1 System Audio Control ====
        case eAudioModeRequest :{			//<MS> <0x70 Audio Mode Request > <Phy Adr> ,example:[0x05 0x70 00 00] TV send Audio mode request ON to AVR
                                        //<MS> <0x70 Audio Mode Request >         ,example:[0x05 0x70] TV send Audio mode request OFF to AVR

                IT6802_CEC_PRINT(("Receive !!!System Audio Mode Request !!!\n"));
                //CTS 11.2.15 Audio system control
                //

                uc=(pCECRxBuf->id.HEADER & 0xf0)>>4;	// Get initator logic address

                if(pCECRxBuf->id.SIZE==4)
                {
                    bSystemAudioMode=1;	// Initating the system audio control

                    if(uc==DEVICE_ID_TV)
                        CECCmdSet(DEVICE_ID_BROADCAST,eSetSystemAudioMode,bSystemAudioMode,0);	// AVR reply  [0x05 0x72  and Audio On / Off]
                    else
                        Switch_SystemAudioControl(sSetSystemAudioModeToTV);
                    //need to un-mute Amplifer speaker
                }
                else
                {
                    bSystemAudioMode=0;	//Terminating the system audio control
                    CECCmdSet(DEVICE_ID_BROADCAST,eSetSystemAudioMode,bSystemAudioMode,0);	// AVR reply  [0x05 0x72  and Audio On / Off]
                    //need to mute Amplifer speaker
                }




            }
            break;

        case eGiveAudioStatus:{			//<MS> <0x71>
            IT6802_CEC_PRINT(("Receive !!!Give Audio Status !!!\n"));
            uc=(pCECRxBuf->id.HEADER & 0xf0)>>4;	// Get initator logic address
            CECCmdSet(uc,eReportAudioStatus,uCECAudioStatus.AStatus,0);	//AVR reply  Report Audio Status [0x05 0x7A and Volume and Mute status]
            }
            break;

        case eSetSystemAudioMode :{    	//<MS> <0X72> <Audio on/off>
            IT6802_CEC_PRINT(("Receive Set System Audio Mode !!!\n"));
            if(pCECRxBuf->id.OPERAND1==0)
                bSystemAudioMode=1;	// AVR audio mode ON
            else
                bSystemAudioMode=0;	// AVR audio mode OFF
            }
            break;

        case eReportAudioStatus:{			//<MS> <0x7A> <Volume and Mute Status>
            IT6802_CEC_PRINT(("Receive !!!Report Audio Status !!!\n"));
            uCECAudioStatus.AStatus=pCECRxBuf->id.OPERAND1;
            }
            break;

        case eGiveSystemAudIoModeStatus :{//<MS> <0x7D >  --> <MS> <0x7E > <Audio On / Off>
            IT6802_CEC_PRINT(("Receive !!!Give System Audio Mode Status !!!\n"));
            uc=(pCECRxBuf->id.HEADER & 0xf0)>>4;	// Get initator logic address
            CECCmdSet(uc,eSystemAudIoModeStatus,bSystemAudioMode,0);	//<Audio On / Off>
            }
            break;
        case eSystemAudIoModeStatus :	{ //<MS> <0x7E> <System Audio Status>
            IT6802_CEC_PRINT(("Receive !!!System Audio Mode Status !!!\n"));
            if(pCECRxBuf->id.OPERAND1==0)
                bSystemAudioMode=1;	// AVR audio mode ON
            else
                bSystemAudioMode=0;	// AVR audio mode OFF
            }
            break;

        case eRequestAudioDescriptor :{	//<MS> <0xA4> <Audio Format ID and Code>
            IT6802_CEC_PRINT(("Receive !!!Request Short Audio Descriptor !!!\n"));
            uc=(pCECRxBuf->id.HEADER & 0xf0)>>4;	// Get initator logic address
            CECCmdSet(uc,eReportAudioDescriptor,0,0);	// Report EDID Audio Block !!!
            }
            break;

//=== ARC ====
        case eInitiateARC :
            IT6802_CEC_PRINT(("Receive !!!Initiate ARC !!!\n"));

//			if(CheckUpDownPhyAdr()==TRUE)
            {
                switch_ARCState(sReportARCInitiated);
            }

            break;
        case eReportARCInitiated :
            IT6802_CEC_PRINT(("Receive !!!Report ARC Initiated !!!\n"));
            break;
        case eReportARCTerminated :
            IT6802_CEC_PRINT(("Receive !!!Report ARC Terminated !!!\n"));

//			uc=IT6802_CEC_ReadI2C_Byte(REG_HEAC_CTRL)&(~(B_ARC_TX_EN|B_ARC_RX_EN|B_SPDIFO_EN|B_HEC_EN));
//
//			//if(Myself_LogicAdr==0)
//			//	uc &= ~(B_ARC_TX_EN);
//			//else
//			//	uc &=~(B_ARC_RX_EN|B_SPDIFO_EN);
//			//uc&=0xFC;	//2010/12/21 for test , disable ARC rx and tx  , not yet !!!
//
//			IT6802_CEC_WriteI2C_Byte(REG_HEAC_CTRL,uc);
//			DumpIT6802_CECReg();

            break;
        case eRequestARCInitiation :
            IT6802_CEC_PRINT(("Receive !!!Request ARC Initiation !!!\n"));
//			if(CheckUpDownPhyAdr()==TRUE)
                {
                //FollowerAddr=DEVICE_ID_BROADCAST;
                switch_ARCState(sInitiateARC);
                }

            break;
        case eRequestARCTermination :    //Request ARC Termination
            IT6802_CEC_PRINT(("Receive !!!Request ARC Termination !!!\n"));
            if(CheckUpDownPhyAdr()==TRUE){
                switch_ARCState(sTerminateARC);
                }
            break;
        case eTerminateARC :    //Termiate ARC
            IT6802_CEC_PRINT(("Receive !!!Termiate ARC !!!\n"));
            switch_ARCState(sReportARCTermination);

            break;


//=== HEC ===
        case eCDC:
            switch( pCECRxBuf->id.OPERAND3 )
            {
                case eHEC_InquirState:
                    IT6802_CEC_PRINT(("\nReceive !!!CEC_HEC_InquireState !!!\n"));
//					SwitchHECState(sHEC_InquirState);
                    break;
                case eHEC_ReportState:
                    IT6802_CEC_PRINT(("\nReceive !!!CEC_HEC_Report_State !!!\n"));
                    SwitchHECState(sHEC_ReportState);
                    break;
                case eHEC_SetStateAdjacent:
                    IT6802_CEC_PRINT(("\nReceive !!!CEC_HEC_SetStateAdjacent !!!\n"));
                    SwitchHECState(sHEC_SetStateAdjacent);
                    break;
                case eHEC_SetState:
                    IT6802_CEC_PRINT(("\nReceive !!!CDC_HEC_SetState !!!\n"));
                    SwitchHECState(sHEC_SetState);
                    break;
                case eHEC_RequestDeactivation:
                    IT6802_CEC_PRINT(("\nReceive !!!CEC_HEC_RequestDeactivation !!!\n"));
                    SwitchHECState(sHEC_RequestDeactivation);
                    break;
                case eHEC_NotifyAlive:
                    IT6802_CEC_PRINT(("\nReceive !!!CEC_HEC_NotifyAlive !!!\n"));
                    SwitchHECState(sHEC_NotifyAlive);
                    break;
                case eHEC_Discover:
                    IT6802_CEC_PRINT(("\nReceive !!!CEC_HEC_Discover !!!\n"));
                    SwitchHECState(sHEC_Discover);
                    break;
                case eHEC_HPDSetState:
                    IT6802_CEC_PRINT(("\nReceive !!!CDC_HPD_SetState !!!\n"));
                    SwitchHECState(sHEC_HPDSetState);
                    break;
                case eHEC_HPDReportState:
                    IT6802_CEC_PRINT(("\nReceive CEC_HDP_ReportState !!!\n"));
                    SwitchHECState(sHEC_HPDReportState);
                    break;
                default :
                    IT6802_CEC_PRINT(("\nReceive CDC UnRecongnized OpCode !!!\n"));
                    break;
            }
            break;


        default :
        IT6802_CEC_PRINT(("\nReceive UnRecongnized OpCode !!!\n"));
        break;
    }

      IT6802_CEC_PRINT(("!!***************************** %x **************************************************\n\n",CMDCount++));


}


void Switch_OneTouchPlayState(OneTouchPlay_StateType uState)
{

    IT6802_CEC_PRINT(("Switch_OneTouchPlayState--> %X\n",uState));

//	if(Myself_LogicAdr!=DEVICE_ID_AUDIO  && Myself_LogicAdr!=DEVICE_ID_TV)
    if(Myself_LogicAdr!=DEVICE_ID_TV)
    {
        OTState=uState;

        switch(uState)
        {

            case sImageViewOn:			//			=0x04,	// follower:TV, switch	--> Broadcst
                    CECCmdSet(DEVICE_ID_TV,eImageViewOn,0,0);
                    break;
            case sActiveSource:			//			=0x82,	// follower:TV, switch	--> Broadcst , Directly address

                    if(Myself_LogicAdr!=DEVICE_ID_TV) //&& Myself_LogicAdr!=DEVICE_ID_AUDIO)
                        CECCmdSet(DEVICE_ID_BROADCAST, eActiveSource,txphyadr[0],txphyadr[1]);
                    break;
            case sTextViewOn:			//			=0x0D,	// follower:TV
                    CECCmdSet(DEVICE_ID_BROADCAST,eTextViewOn,0,0);
                    break;

            default:
                    break;

        }

    }
}

void OneTouchPlayHandler(void)
{


    switch(OTState)
    {

        case sImageViewOn:			//			=0x82,	// follower:TV, switch	--> Broadcst , Directly address
                {

                    if(OldTxCECcmd==eImageViewOn)
                    {

                        switch(CECTxState)
                        {

                            case sCECReceiverFail:

                                    break;

                            case sCECOk:
                            case sCECNone:
                                    Switch_OneTouchPlayState(sActiveSource);
                                    //DumpIT6802_CECReg();
                                    break;

                        }
                    }
                }
                break;
        case sActiveSource:			//			=0x04,	// follower:TV, switch	--> Broadcst
                {

                    if(OldTxCECcmd==eActiveSource)
                    {

                        switch(CECTxState)
                        {

                            case sCECReceiverFail:

                                    break;

                            case sCECOk:
                            case sCECNone:
                                    Switch_OneTouchPlayState(sTextViewOn);
                                    //DumpIT6802_CECReg();
                                    break;

                        }
                    }
                }
                break;

        case sTextViewOn:			//			=0x0D,	// follower:TV
                {

                    if(OldTxCECcmd==sTextViewOn)
                    {

                        switch(CECTxState)
                        {

                            case sCECReceiverFail:

                                    break;

                            case sCECOk:
                            case sCECNone:
                                    Switch_OneTouchPlayState(sOTNone);
                                    //DumpIT6802_CECReg();
                                    break;

                        }
                    }
                }
                break;
        default:
                break;

    }

}






void switch_PollingMessage(PollingMessage_StateType state)
{
    if(Myself_LogicAdr==DEVICE_ID_BROADCAST ||Myself_LogicAdr==DEVICE_ID_PLAYBACK1 ||
        Myself_LogicAdr==DEVICE_ID_PLAYBACK2 || Myself_LogicAdr==DEVICE_ID_PLAYBACK3 )
    {


        IT6802_CEC_PRINT(("DVD switch_PollingMessage--> %X\n",state));
        switch(state)
        {
            case sPollingMessage1:

                    #ifdef	_CEC_DEVICE_PLAYBACK_
                        Myself_LogicAdr=DEVICE_ID_PLAYBACK1;
                    #endif

                    #ifdef	_CEC_DEVICE_AVR_
                        Myself_LogicAdr=DEVICE_ID_AUDIO;
                    #endif

                    IT6802_CEC_WriteI2C_Byte(REG_CEC_TARGET_ADDR, Myself_LogicAdr);
                    CECCmdSet(Myself_LogicAdr, ePollingMessage,0,0);
                    break;
            case sPollingMessage2:

                    #ifdef	_CEC_DEVICE_PLAYBACK_
                        Myself_LogicAdr=DEVICE_ID_PLAYBACK2;
                    #endif

                    #ifdef	_CEC_DEVICE_AVR_
                        Myself_LogicAdr=DEVICE_ID_AUDIO;
                    #endif

                    IT6802_CEC_WriteI2C_Byte(REG_CEC_TARGET_ADDR, Myself_LogicAdr);
                    CECCmdSet(Myself_LogicAdr, ePollingMessage,0,0);
                    break;
            case sPollingMessage3:

                    #ifdef	_CEC_DEVICE_PLAYBACK_
                        Myself_LogicAdr=DEVICE_ID_PLAYBACK3;
                    #endif

                    #ifdef	_CEC_DEVICE_AVR_
                        Myself_LogicAdr=DEVICE_ID_AUDIO;
                    #endif

                    IT6802_CEC_WriteI2C_Byte(REG_CEC_TARGET_ADDR, Myself_LogicAdr);
                    CECCmdSet(Myself_LogicAdr, ePollingMessage,0,0);
                    break;
            case sReportPhyAdr:
                    {
//
//						if(PMState==sPollingPlayer1)
//							Myself_LogicAdr=DEVICE_ID_PLAYBACK1;
//						else if(PMState==sPollingPlayer2)
//							Myself_LogicAdr=DEVICE_ID_PLAYBACK2;
//						else if(PMState==sPollingPlayer3)
//							Myself_LogicAdr=DEVICE_ID_PLAYBACK3;
//
//						IT6802_CEC_WriteI2C_Byte(REG_CEC_TARGET_ADDR, Myself_LogicAdr);
                        CECCmdSet(DEVICE_ID_BROADCAST, eReportPhysicalAddress,txphyadr[0],txphyadr[1]);
                    }
                    break;
            case sReportDeviceVendorID:
                        CECCmdSet(DEVICE_ID_BROADCAST, eDeviceVendorID,0,0);
                    break;

        }
        PMState=state;

    }

}
void PollingMessage(void)
{
//	BYTE uc;
    switch(PMState)
    {
        case sPollingMessage1:
                {
                    if(OldTxCECcmd==ePollingMessage)
                    {
//
//						//switch(CECTxState)
//						uc=(IT6802_CEC_ReadI2C_Byte(REG_SYS_STATUS));
//						IT6802_CEC_PRINT(("out status=%X\n",uc);
//						uc&=B_OUT_STATUS;
//						if(uc==B_ReceiverNACK ||uc==B_ReceiverFail )
//						{
//
//							//case sCECReceiverFail:
//									switch_PollingMessage(sReportPhyAdr);
//							//		break;
//
//							//case sCECOk:
//							//case sCECNone:
//							//		switch_PollingMessage(sPollingPlayer2);
//							//		break;
//						}else
//							{
//								switch_PollingMessage(sPollingPlayer2);
//							}

#if 1
                        switch(CECTxState)
                        {
                            //case sCECReceiverNack:
                            case sCECReceiverFail:
                                    switch_PollingMessage(sReportPhyAdr);
                                    break;

                            case sCECOk:
                            case sCECNone:
                                    switch_PollingMessage(sPollingMessage2);
                                    break;
                        }
#else
                    if( IT6802_CECTxOutStatusUpdate() == 0)	// fail
                            switch_PollingMessage(sReportPhyAdr);
                        else if( IT6802_CECTxOutStatusUpdate() == 1)	// sucessful
                            switch_PollingMessage(sPollingMessage2);
#endif

                    }
                }
                break;
        case sPollingMessage2:
                {
                    if(OldTxCECcmd==ePollingMessage)
                    {

                    #if 1
                        switch(CECTxState)
                        {

                            //case sCECReceiverNack:
                            case sCECReceiverFail:
                                    switch_PollingMessage(sReportPhyAdr);
                                    break;

                            case sCECOk:
                            case sCECNone:
                                    switch_PollingMessage(sPollingMessage3);
                                    break;
                        }
                    #else
                        if( IT6802_CECTxOutStatusUpdate() == 0)	// fail
                            switch_PollingMessage(sReportPhyAdr);
                        else if( IT6802_CECTxOutStatusUpdate() == 1)	// sucessful
                            switch_PollingMessage(sPollingMessage3);

                    #endif

                    }
                }
                break;
        case sPollingMessage3:
                {
                    if(OldTxCECcmd==ePollingMessage)
                    {


                    #if 1
                        switch(CECTxState)
                        {
                            //case sCECReceiverNack:
                            case sCECReceiverFail:
                                    switch_PollingMessage(sReportPhyAdr);
                                    break;

                            case sCECOk:
                            case sCECNone:
                                    switch_PollingMessage(sPMnone);
                                    break;
                        }
                    #else
                        if( IT6802_CECTxOutStatusUpdate() == 0)	// fail
                            switch_PollingMessage(sReportPhyAdr);
                        else if( IT6802_CECTxOutStatusUpdate() == 1)	// sucessful
                            switch_PollingMessage(sPMnone);

                    #endif

                    }
                }
                break;

        case sReportPhyAdr:
                    if(OldTxCECcmd==eReportPhysicalAddress)
                    {
                    #if 1
                        switch(CECTxState)
                        {
                            //default:
                            //case sCECReceiverNack:
                            case sCECReceiverFail:
									//CECCmdSet(DEVICE_ID_BROADCAST, eReportPhysicalAddress,txphyadr[0],txphyadr[1]);
                                    //printf("CECTxState=%X ,Reg44=%bX\n",CECTxState,(IT6802_CEC_ReadI2C_Byte(REG_SYS_STATUS)));
                                    //IT6802_CECDeviceStateReset();	// Give up !!!
                                    break;


                            case sCECOk:
                            case sCECNone:
                                    IT6802_CEC_PRINT(("CECTxState=%X\n",CECTxState));
                                    switch_PollingMessage(sReportDeviceVendorID);
                                    break;
                        }
                    #else
						//if( IT6802_CECTxOutStatusUpdate() == 0)	 //fail
						//    switch_PollingMessage(sReportPhyAdr);
						//else
                        if( IT6802_CECTxOutStatusUpdate() == 1)	// sucessful
                            switch_PollingMessage(sReportDeviceVendorID);

                    #endif
                    }


        case sReportDeviceVendorID:
                    if(OldTxCECcmd==eDeviceVendorID)
                    {

                    #if 1
                        switch(CECTxState)
                        {
                            //case sCECReceiverNack:
                            case sCECReceiverFail:
//									CECCmdSet(DEVICE_ID_BROADCAST, eDeviceVendorID,0,0);
                                    break;

                            case sCECOk:
                            case sCECNone:
                                    switch_PollingMessage(sPMnone);
                                    break;
                        }
                    #else
                        if( IT6802_CECTxOutStatusUpdate() == 1)	// sucessful
                            switch_PollingMessage(sPMnone);

                    #endif

                    }

        default:
            break;
    }


}


void Switch_LogicalAddressingState(LogicAddressing_StateType uState)
{

    if(Myself_LogicAdr!=DEVICE_ID_TV)
        return;

    IT6802_CEC_PRINT(("TV Switch_LogicalAddressingState--> %X\n",uState));
    LAState=uState;
}


void TVLogicalAddressingHandler(void)
{
    u8_t		uc;
    static	u8_t		cPollingCount;

    if(Myself_LogicAdr!=DEVICE_ID_TV)
        return;


    switch(LAState)
    {

        case sReportPhysicalAddressTransfer:		// only for TV to create CEC device list
            {
                CECCmdSet(DEVICE_ID_BROADCAST, eReportPhysicalAddress,txphyadr[0],txphyadr[1]);
                cPollingCount=0;
                LAState=sPollingResult;
            }
            break;

        case sPollingMessage:		// polling next CEC device
            {
                cPollingCount++;
                if(cPollingCount<15)
                {
                    uc=(cPollingCount |0xF0);
                    CECCmdSet(uc,ePollingMessage,0,0);
                    LAState=sPollingResult;
                }else
                    {
                        LAState=sLANone;
                        for(uc=1;uc<15;uc++)
                        {
                            IT6802_CEC_PRINT(("CECList[%X]= %bX , PhysicalAddr=[%bX][%bX]\n",
                                    uc,
                                    CECList[uc].Active,
                                    CECList[uc].PhyicalAddr1,
                                    CECList[uc].PhyicalAddr2));

                        }
                    }
            }
            break;

        case sPollingResult:		// Check polling result
            {
                //uc=(IT6802_CEC_ReadI2C_Byte(REG_SYS_STATUS)&B_OUT_STATUS);
                //IT6802_CEC_PRINT(("out status=%X\n",uc);
                //if(uc==B_ReceiverFail)		// not yet !!! need to verify out status

                //printf("CECTxState=%X\n",CECTxState);

                if(CECTxState==sCECReceiverFail)// || CECTxState==sCECReceiverNack)
                {

                    //if(OldTxCECcmd==ePollingMessage)
                    {
                        //clear value for CEC list
                        CECList[cPollingCount].Active=FALSE;
                        CECList[cPollingCount].PhyicalAddr1=0x00;
                        CECList[cPollingCount].PhyicalAddr2=0x00;
                        LAState=sPollingMessage;
                    }
                }
                else
                if(CECTxState==sCECOk)
                {
//					LAState=sReportPhysicalAddressReceived;
//					if(pCECRxBuf->id.OPCODE==eReportPhysicalAddress)
//					{
//						LAState=sPollingMessage;
//					}
                        LAState=sPollingMessage;
                }
            }
            break;

//		case sReportPhysicalAddressReceived:
//			{
//				// stat to polling message and check ACK for allocate Logical Address to CEC device
//				if(pCECRxBuf->id.OPCODE==eReportPhysicalAddress)
//				{
//					if((pCECRxBuf->id.HEADER & 0x0f)==0x0f)
//					{
//
//						uc=((pCECRxBuf->id.HEADER & 0xf0)>>4);
//						cPollingCount=uc;
//						CECList[cPollingCount].PhyicalAddr1=pCECRxBuf->id.OPERAND1;
//						CECList[cPollingCount].PhyicalAddr2=pCECRxBuf->id.OPERAND2;
//						CECList[cPollingCount].Active=TRUE;
//						LAState=sPollingMessage;
//
//					}
//					IT6802_CEC_PRINT(("-----Rx Header=%X, opcode=%bX , operand1=%bX , operand2=%bX ,cPollingCount=%bX-----\n ",
//					pCECRxBuf->id.HEADER,
//					pCECRxBuf->id.OPCODE,
//					pCECRxBuf->id.OPERAND1,
//					pCECRxBuf->id.OPERAND2,
//					cPollingCount));
//				}
//			}
//			break;

        case sLANone:
            break;

    }
}

BOOL CheckUpDownPhyAdr(void)
{
    u8_t i;
    BOOL Result=FALSE;
    BYTE pA,pB,pC,pD,MyLevel,TargetLevel;

    for(i=0;i<4;i++)
    {
        if((rxphyadr[i][0]==CECList[FollowerAddr].PhyicalAddr1) && (rxphyadr[i][1]==CECList[FollowerAddr].PhyicalAddr2) )
        {
            return TRUE;
        }
    }

    pA = ((CECList[FollowerAddr].PhyicalAddr1)&0xF0)>>4;
    pB = ((CECList[FollowerAddr].PhyicalAddr1)&0x0F);
    pC = ((CECList[FollowerAddr].PhyicalAddr2)&0xF0)>>4;
    pD = ((CECList[FollowerAddr].PhyicalAddr2)&0x0F);

    if( pA==0 && pB==0 && pC==0 && pD==0 )
        TargetLevel = 0;
    else if( pB==0 && pC==0 && pD==0 )
        TargetLevel = 1;
    else if( pC==0 && pD==0 )
        TargetLevel = 2;
    else if( pD==0 )
        TargetLevel = 3;

    pA = ((txphyadr[0])&0xF0)>>4;
    pB = ((txphyadr[0])&0x0F);
    pC = ((txphyadr[1])&0xF0)>>4;
    pD = ((txphyadr[1])&0x0F);

    if( pA==0 && pB==0 && pC==0 && pD==0 )
        MyLevel = 0;
    else if( pB==0 && pC==0 && pD==0 )
        MyLevel = 1;
    else if( pC==0 && pD==0 )
        MyLevel = 2;
    else if( pD==0 )
        MyLevel = 3;


    if(MyLevel>TargetLevel)
        Result=TRUE;

    return Result;

}
void switch_ARCState(ARC_StateType uState)
{
    BYTE	uc;

    //if(Myself_LogicAdr==DEVICE_ID_AUDIO||Myself_LogicAdr==DEVICE_ID_TV)
    {
        IT6802_CEC_PRINT(("switch ARCState -> %s  !!!\n",ARCStr[uState]));
        ARCState=uState;


        switch(ARCState)
        {

            case sRequestARCInitiation:
                        IT6802_CECCheckFollower();
                        CECCmdSet(FollowerAddr, eRequestARCInitiation,txphyadr[0],txphyadr[1]);


                    break;
            case sInitiateARC:
                    {


// for IT6802+iT6623						// Receiver CEC command Queue for I2C slave protocal
// for IT6802+iT6623						t_IIC_Buffer.I2cRegisterMap.CEC_ARC_Status=1;
// for IT6802+iT6623						I2C_CECAddQ(0xC0);	//2011/06/09 for ACR cmd
                        IT6802_CECCheckFollower();
                        CECCmdSet(FollowerAddr, eInitiateARC,txphyadr[0],txphyadr[1]);
                        uc=IT6802_CEC_ReadI2C_Byte(REG_HEAC_CTRL)&(~(B_ARC_TX_EN|B_ARC_RX_EN|B_SPDIFO_EN|B_HEC_EN));
                        if(Myself_LogicAdr==0)
                            uc |= (B_ARC_TX_EN);
                        else
                            {
                            uc |= (B_ARC_RX_EN|B_SPDIFO_EN);
                            }
                        IT6802_CEC_WriteI2C_Byte(REG_HEAC_CTRL,uc);
                        ARCState=sARCNone;
                        //DumpIT6802_CECReg();



                        IT6802_CEC_PRINT((" ARC on\n"));

                    }
                    break;
            case sReportARCInitiated:
                    {
                        IT6802_CECCheckFollower();
                        CECCmdSet(FollowerAddr, eReportARCInitiated,txphyadr[0],txphyadr[1]);

                        uc=IT6802_CEC_ReadI2C_Byte(REG_HEAC_CTRL)&(~(B_ARC_TX_EN|B_ARC_RX_EN|B_SPDIFO_EN|B_HEC_EN));
                        if(Myself_LogicAdr==0)
                            uc |= (B_ARC_TX_EN);
                        else
                            {
                            uc |= (B_ARC_RX_EN|B_SPDIFO_EN);
                            }
                        IT6802_CEC_WriteI2C_Byte(REG_HEAC_CTRL,uc);
                        ARCState=sARCNone;
                        //DumpIT6802_CECReg();
                    }
                    break;


            case sRequestARCTermination:
                    {
                        IT6802_CECCheckFollower();
                        CECCmdSet(FollowerAddr, eRequestARCTermination,txphyadr[0],txphyadr[1]);
                    }
                    break;
            case sTerminateARC:
                    {
                        IT6802_CECCheckFollower();
                        CECCmdSet(FollowerAddr, eTerminateARC,txphyadr[0],txphyadr[1]);
                        uc=IT6802_CEC_ReadI2C_Byte(REG_HEAC_CTRL)&(~(B_ARC_TX_EN|B_ARC_RX_EN|B_SPDIFO_EN|B_HEC_EN));

                        //if(Myself_LogicAdr==0)
                        //	uc &= ~(B_ARC_TX_EN);
                        //else
                        //	uc &=~(B_ARC_RX_EN|B_SPDIFO_EN);
                        //uc&=0xFC;	//2010/12/21 for test , disable ARC rx and tx  , not yet !!!

                        IT6802_CEC_WriteI2C_Byte(REG_HEAC_CTRL,uc);

                        ARCState=sARCNone;
                        //DumpIT6802_CECReg();

                    }
                    break;
            case sReportARCTermination:
                    {

// for IT6802+iT6623						// Receiver CEC command Queue for I2C slave protocal
// for IT6802+iT6623						t_IIC_Buffer.I2cRegisterMap.CEC_ARC_Status=0;
// for IT6802+iT6623						I2C_CECAddQ(0xC0);	//2011/06/09 for ACR cmd
                        IT6802_CECCheckFollower();
                        CECCmdSet(FollowerAddr, eReportARCTerminated,txphyadr[0],txphyadr[1]);

                        uc=IT6802_CEC_ReadI2C_Byte(REG_HEAC_CTRL)&(~(B_ARC_TX_EN|B_ARC_RX_EN|B_SPDIFO_EN|B_HEC_EN));

                        //if(Myself_LogicAdr==0)
                        //	uc &= ~(B_ARC_TX_EN);
                        //else
                        //	uc &=~(B_ARC_RX_EN|B_SPDIFO_EN);
                        //uc&=0xFC;	//2010/12/21 for test , disable ARC rx and tx  , not yet !!!

                        IT6802_CEC_WriteI2C_Byte(REG_HEAC_CTRL,uc);

                        ARCState=sARCNone;
                        //DumpIT6802_CECReg();

                        IT6802_CEC_PRINT((" ARC off\n"));
                    }
                    break;
            case sARCNone:
                    break;
        }
    }
}
void ARCHandler(void)
{
//	u8_t		uc;

    switch(ARCState)
    {
        case sRequestARCInitiation:
                {
                    //CECCmdSet(DEVICE_ID_BROADCAST, eRequestARCInitiation,txphyadr[0],txphyadr[1]);
                    //ARCState=sARCNone;
                }
            break;

        case sInitiateARC:
                {
                    //CECCmdSet(DEVICE_ID_BROADCAST, eInitiateARC,txphyadr[0],txphyadr[1]);
                    //ARCState=sARCNone;
                }
            break;

        case sReportARCInitiated:
                {
//					CECCmdSet(FollowerAddr, eReportARCInitiated,txphyadr[0],txphyadr[1]);
//
//					uc=IT6802_CEC_ReadI2C_Byte(REG_HEAC_CTRL)&(~(B_ARC_TX_EN|B_ARC_RX_EN|B_SPDIFO_EN|B_HEC_EN));
//					if(Myself_LogicAdr==0)
//						uc |= (B_ARC_TX_EN);
//					else
//						{
//						uc |= (B_ARC_RX_EN|B_SPDIFO_EN);
//						}
//					IT6802_CEC_WriteI2C_Byte(REG_HEAC_CTRL,uc);
//					ARCState=sARCNone;
//					DumpIT6802_CECReg();
                }
            break;

        case sRequestARCTermination:
                {
                    //CECCmdSet(DEVICE_ID_BROADCAST, eRequestARCTermination,txphyadr[0],txphyadr[1]);
                    //ARCState=sTerminateARC;
                }
            break;

        case sTerminateARC:
                {
//					CECCmdSet(DEVICE_ID_BROADCAST, eTerminateARC,txphyadr[0],txphyadr[1]);
//					ARCState=sARCNone;
                }

            break;

        case sReportARCTermination:
                {
//					CECCmdSet(FollowerAddr, eReportARCTerminated,txphyadr[0],txphyadr[1]);
//
//					uc=IT6802_CEC_ReadI2C_Byte(REG_HEAC_CTRL)&(~(B_ARC_TX_EN|B_ARC_RX_EN|B_SPDIFO_EN|B_HEC_EN));
//
//					//if(Myself_LogicAdr==0)
//					//	uc &= ~(B_ARC_TX_EN);
//					//else
//					//	uc &=~(B_ARC_RX_EN|B_SPDIFO_EN);
//					//uc&=0xFC;	//2010/12/21 for test , disable ARC rx and tx  , not yet !!!
//
//					IT6802_CEC_WriteI2C_Byte(REG_HEAC_CTRL,uc);
//
//					ARCState=sARCNone;
//					DumpIT6802_CECReg();
                }
            break;

        case sARCNone:
        default:
            break;
    }

}



HEC_StateType	HECState=sHEC_None;

void SwitchHECState(HEC_StateType	uState)
{
    HECState=uState;
}

#if 0
void HECHandler(void)
{

//	u8_t		uc;
    switch(HECState)
        {
            case	sHEC_InquirState:
                {
                    //waiting to receive init ARC cmd
                    if(pCECRxBuf->id.OPERAND3==eHEC_InquirState)
                    {
                        //uc=((pCECRxBuf->id.HEADER & 0xf0)>>4);
                        //FollowerAddr=uc;

                        //pCECRxBuf->id.OPERAND1

                        IT6802_CEC_PRINT(("-----Header=%X, opcode=%bX , operand1=%bX , operand2=%bX ,FollowerAddr=%bX-----\n ",
                        pCECRxBuf->id.HEADER,
                        pCECRxBuf->id.OPCODE,
                        pCECRxBuf->id.OPERAND1,
                        pCECRxBuf->id.OPERAND2,
                        FollowerAddr
                        );

                        ARCState=sReportARCInitiated;
                    }
                }
                break;
            case	sHEC_ReportState:
                {

                }
                break;
            case	sHEC_SetStateAdjacent:
                {
                }
                break;
            case	sHEC_SetState:
                {
                }
                break;
            case	sHEC_RequestDeactivation:
                {
                }
                break;
            case	sHEC_NotifyAlive:
                {
                }
                break;

            case	sHEC_Discover:
                {
                }
                break;

            case	sHEC_HPDSetState:
                {
                }
                break;

            case	sHEC_HPDReportState:
                {
                }
                break;
            case 	sHEC_None:
                {
                }
                break;

        }



}
#endif

#if 0
void Switch_SystemInfoFeature(SystemInfoFeature_StateType State)
{
    IT6802_CEC_PRINT(("!!! SwitchSystemInfoFeature -> %X !!!\n",State);

    SIFState=State;

    switch(SIFState)
    {

        case sReportPhysicalAddress:
                CECCmdSet(DEVICE_ID_BROADCAST,eReportPhysicalAddress,txphyadr[0],txphyadr[1]);
                break;

        case sCECVersioin:
        case sGetCECVersion:
        case sGetMenuLanguage:
        case sGivePhysicalAddress:
        case sSetMenuLanguage:
        case sSIFnone:
                break;
    }

}
#endif
//w/o 11.2.6-3~5
void SystemInfomationFeature(void)
{

#if 0
    switch(SIFState)
    {


        case sReportPhysicalAddress:
                    if(OldTxCECcmd==eReportPhysicalAddress)
                    {
                        switch(CECTxState)
                        {
                            case sCECReceiverNack:
                            case sCECReceiverFail:
                                    break;

                            case sCECOk:
                            case sCECNone:
                                    if(Myself_LogicAdr!=DEVICE_ID_TV && Myself_LogicAdr!=DEVICE_ID_AUDIO)
                                        switch_PollingMessage(sPollingMessage1);

                                    SIFState	=sSIFnone;
                                    break;
                        }
                    }

                break;

        case sCECVersioin:
        case sGetCECVersion:
        case sGetMenuLanguage:
        case sGivePhysicalAddress:
        case sSetMenuLanguage:
        case sSIFnone:
                break;
    }

#endif
}
void CECManager(void)
{
    //Receiver of CEC command
    //#ifndef _Enable_CEC_Interrupt_Pin_
    IT6802_CECReceiver();		// Receive CEC command
    //#endif

    //Decoder of CEC command
    CECDecoder();			// De-Queue for receive CEC command

    //Handler of ARC
    ARCHandler();				//for TV device

    OneTouchPlayHandler();		//for Audio and player device

    TVLogicalAddressingHandler();	//for TV device

    PollingMessage();			//for player device

    SystemInfomationFeature();	//for system information Feature

    SystemAudioControl();		//for Audio control system CTS 11.2.15

    // Tranmitter of CEC command
    IT6802_CECTransmitter();
}
// for IT6802+iT6623
// for IT6802+iT6623//
// for IT6802+iT6623//void switch_Standby(unsigned char state)
// for IT6802+iT6623//{
// for IT6802+iT6623//
// for IT6802+iT6623//	StandbyState=state;
// for IT6802+iT6623//
// for IT6802+iT6623//	switch(StandbyState)
// for IT6802+iT6623//	{
// for IT6802+iT6623//		case 0:
// for IT6802+iT6623//			// TODO: nothing!!!
// for IT6802+iT6623//			break;
// for IT6802+iT6623//
// for IT6802+iT6623//		case 1:
// for IT6802+iT6623//			bSystemAudioMode=0;
// for IT6802+iT6623//			CECCmdSet(DEVICE_ID_TV,eSetSystemAudioMode,bSystemAudioMode,0 );
// for IT6802+iT6623//			break;
// for IT6802+iT6623//
// for IT6802+iT6623//		case 1:
// for IT6802+iT6623//			CECCmdSet(DEVICE_ID_BROADCAST,eSetSystemAudioMode,bSystemAudioMode,0 );
// for IT6802+iT6623//			break;
// for IT6802+iT6623//
// for IT6802+iT6623//		case 2:
// for IT6802+iT6623//			CECCmdSet(DEVICE_ID_BROADCAST, eStandBy,0,0);
// for IT6802+iT6623//			break;
// for IT6802+iT6623//	}
// for IT6802+iT6623//}
// for IT6802+iT6623//void StandbyHandler(void)
// for IT6802+iT6623//{
// for IT6802+iT6623//	switch(StandbyState)
// for IT6802+iT6623//	{
// for IT6802+iT6623//		case 0:
// for IT6802+iT6623//			break;
// for IT6802+iT6623//
// for IT6802+iT6623//		case 1:
// for IT6802+iT6623//			break;
// for IT6802+iT6623//	}
// for IT6802+iT6623//
// for IT6802+iT6623//
// for IT6802+iT6623//}
// for IT6802+iT6623
void CECTest(eCEC_CMD cmd,unsigned char cData)
{
    // unsigned char uc;

    switch(cmd)
    {
        case ePollingMessage:

        case eActiveSource:			//			=0x82,	// follower:TV, switch	--> Broadcst , Directly address,  Opread: Phyical address
                // CECCmdSet(DEVICE_ID_BROADCAST, cmd,txphyadr[0],txphyadr[1]);
                // Switch_OneTouchPlayState(sImageViewOn);
                break;
        case eImageViewOn:			//			=0x04,	// follower:TV, switch	--> Broadcst
        case eTextViewOn:				//			=0x0D,	// follower:TV
                break;
        // Routing Control Feature
        case eRoutingChange:			//			=0x80,	// follower:TV, switch	--> Broadcst
            //CEC_RoutingChange(cData);
                break;

        case eRoutingInformation:		//			=0x81,	// follower:switch		--> Broadcst
        case eRequestActiveSource:		//			=0x85,	// follower:switch		--> Broadcst 	, AVR to request active source when Audio ocntrol feature is active
                break;

        case eSetStreamPath:			//			=0x86,	// follower:switch		--> Broadcst
                {
                    if(cData==0)
                        CECCmdSet(DEVICE_ID_BROADCAST, eSetStreamPath, rxphyadr[0][0], rxphyadr[0][1]);
                    else
                        CECCmdSet(DEVICE_ID_BROADCAST, eSetStreamPath, rxphyadr[1][0], rxphyadr[1][1]);
                    break;
                }
        case eInactiveSource:			//			=0x9D,	//
                {
                    CECCmdSet(DEVICE_ID_TV, eInactiveSource,txphyadr[0],txphyadr[1]);
                    break;
                }
        case eStandBy:				//			=0x36,	// follower:All			--> Broadcst
                {
                    CECCmdSet(DEVICE_ID_BROADCAST, eStandBy,0,0);

                    #if 0
                    bSystemAudioMode=0;

                    if(cData==0)
                        CECCmdSet(DEVICE_ID_TV,eSetSystemAudioMode,bSystemAudioMode,0 );
                    else
                        CECCmdSet(DEVICE_ID_BROADCAST,eSetSystemAudioMode,bSystemAudioMode,0 );
                    #endif

                    break;
                }

        // System Information Feature
        case eCECVersioin:				//			=0x9E,
        case eGetCECVersion:			//			=0x9F,
        case eGivePhysicalAddress:		//			=0x83,	// follower:All
            Switch_LogicalAddressingState(sReportPhysicalAddressTransfer);
            break;
        case eGetMenuLanguage:		//			=0x91,	// follower:TV
        case eReportPhysicalAddress:	//			=0x84,	// follower:TV
        case eSetMenuLanguage:		//			=0x32,	// follower:All,		Initiator:TV

        // Vendor Specific Commands Feature
        case eDeviceVendorID:			//			=0x87,
        case eGiveDeviceVendorID:		//			=0x8C,
        case eVendorCommand:		//			=0x89,
            break;
        case eReportPowerStatus:		//			=0x90,
                u8_PowerStatus=cData;	// 0: on , 1: standby, 2: in transition standby to on , 3: in transition on to standby
            break;
        case eVendorCommandWithID:	//			 =0xA0,

        // other
        case ePlay:					//			=0x41,
            break;

        case eUserPressed:			//		=0x44,
            CECCmdSet(DEVICE_ID_TV, eUserPressed, 0x60,0);
            break;
        case eUserReleased:			//			=0x45,
            CECCmdSet(DEVICE_ID_TV, eUserReleased, 0x60,0);
            break;

        case eAudioModeRequest:		//			=0x70,
        case eGiveAudioStatus:			//			=0x71,
            break;
        case eSetSystemAudioMode:			//			=0x72,

            if(cData==0)
                bSystemAudioMode=0;
            else
                bSystemAudioMode=1;
//
//			if(cData==0)
//				CECCmdSet(DEVICE_ID_TV,eSetSystemAudioMode,bSystemAudioMode,0 );
//			else
//				CECCmdSet(DEVICE_ID_BROADCAST,eSetSystemAudioMode,bSystemAudioMode,0 );

            if(cData==0)
                Switch_SystemAudioControl(sSetSystemAudioModeToTV);
            else
                Switch_SystemAudioControl(sComesOutOfStandby);


            break;
        case eReportAudioStatus:		//			=0x7A,
        case eGiveSystemAudIoModeStatus:		//			=0x7D,
        case eSystemAudIoModeStatus:			//			=0x7E,
        case eRemoteButtonDown:		//			=0x8A,
        case eRemoteButtonUp:			//			=0x8B,
            break;
        case eRequestAudioDescriptor:	//			 =0xA4,
            CECCmdSet(FollowerAddr,eRequestAudioDescriptor,0,0);	// Report EDID Audio Block !!!
            break;
        // ACR
        case eInitiateARC:				//			=0xC0,
        case eReportARCInitiated:		//			=0xC1,
        case eReportARCTerminated:	//			=0xC2,
        case eRequestARCInitiation:		//			=0xC3,
        case eRequestARCTermination:	//			=0xC4,
        case eTerminateARC:			//			=0xC5,
            break;
    }

}




void Switch_SystemAudioControl(SystemAudioControl_StateType State)
{
    IT6802_CEC_PRINT(("!!! SwitchSystemInfoFeature -> %X !!!\n",State));

    SACState=State;

    switch(SACState)
    {
        case sComesOutOfStandby:
                bSystemAudioMode=1;
                CECCmdSet(DEVICE_ID_BROADCAST,eRequestActiveSource,0,0);
            break;

        case sSetSystemAudioModeToTV:

            CECCmdSet(DEVICE_ID_TV,eSetSystemAudioMode,bSystemAudioMode,0 );
//0503 not yet!!! 			CreatTimerTask(eCECTimeOut,200,SysTimerTask_OnceRun);
            break;

        case sWaitingFeatureAbort:
//0503 not yet!!!			CreatTimerTask(eCECTimeOut,1000,SysTimerTask_OnceRun);
            break;

        case sSetSystemAudioModeToAll:
            CECCmdSet(DEVICE_ID_BROADCAST,eSetSystemAudioMode,bSystemAudioMode,0 );
            break;

    }

}



//w/o 11.2.15-10~14 !!!
void SystemAudioControl(void)		//for Audio control system CTS 11.2.15
{

    switch(SACState)
    {
        case sSACnone:
            // TODO: Nothing.
            break;

        case sComesOutOfStandby:{
                //please refer CEC Figure 27 A in HDMI Specification 1.4
                //Request Active Source then Waitting Active Source command
                if(pCECRxBuf->id.OPCODE==eActiveSource)
                    {
                        Switch_SystemAudioControl(sSetSystemAudioModeToTV);
                }
            }
            break;

        case sSetSystemAudioModeToTV:{
                if(OldTxCECcmd==eSetSystemAudioMode)
                {
                    switch(CECTxState)
                    {

                        case sCECReceiverFail:
                                Switch_SystemAudioControl(sSetSystemAudioModeToAll);
                                break;
//						case sCECReceiverNack:
//								IT6802_CEC_PRINT(("!!! Nack !!!\n"));
//								if(TimeOutCheck(eCECTimeOut)==TRUE)
//									Switch_SystemAudioControl(sSetSystemAudioModeToAll);
//
//								break;

                        case sCECOk:
                        case sCECNone:
                                Switch_SystemAudioControl(sWaitingFeatureAbort);
                                break;

                    }
                }
            }
            break;

        case sWaitingFeatureAbort:{

                if(pCECRxBuf->id.OPCODE==eFeatureAbort)
                    {
                        Switch_SystemAudioControl(sSACnone);	//finish < Set system Audio Mode >
                    }
//0503 not yet!!!				else
//0503 not yet!!!					{
//0503 not yet!!!
//0503 not yet!!!						if(TimeOutCheck(eCECTimeOut)==TRUE)
//0503 not yet!!!							Switch_SystemAudioControl(sSetSystemAudioModeToAll);
//0503 not yet!!!					}

            }
            break;

    }
}




void CECUICommandCode(BYTE UIKey)
{
            switch(UIKey)
            {
                case 0x40:	//Power
                    if(u8_PowerStatus==0 || u8_PowerStatus==2)
                        u8_PowerStatus=1;	// standby
                    else
                        u8_PowerStatus=0;	// on
                    break;

                case 0x41:	//Volume Up
                    if((uCECAudioStatus.Map.AudioVolumeStatus) <0x7F)
                        uCECAudioStatus.Map.AudioVolumeStatus++;

                    uCECAudioStatus.Map.AudioMuteStatus=FALSE;



// for IT6802+iT6623					t_IIC_Buffer.I2cRegisterMap.CEC_Audio_Status=(BYTE)uCECAudioStatus.Map.AudioVolumeStatus;
// for IT6802+iT6623					I2C_CECAddQ(0x41);	//2011/06/09 for ACR cmd


                    IT6802_CEC_PRINT(("CEC volume = %X\n",uCECAudioStatus.Map.AudioVolumeStatus));

                    break;

                case 0x42:	//Volume Down
                    if((uCECAudioStatus.Map.AudioVolumeStatus) >0x00)
                    {
                        uCECAudioStatus.Map.AudioVolumeStatus--;
                        uCECAudioStatus.Map.AudioMuteStatus=FALSE;


// for IT6802+iT6623						t_IIC_Buffer.I2cRegisterMap.CEC_Audio_Status=(BYTE)uCECAudioStatus.Map.AudioVolumeStatus;
// for IT6802+iT6623						I2C_CECAddQ(0x42);	//2011/06/09 for ACR cmd
// for IT6802+iT6623						IT6802_CEC_PRINT(("CEC volume = %X\n",uCECAudioStatus.Map.AudioVolumeStatus));

                    }
                    else
                    {
                        if((uCECAudioStatus.Map.AudioVolumeStatus) == 0x00)
                            uCECAudioStatus.Map.AudioMuteStatus=TRUE;
                    }

                    break;

                case 0x43:	//Mute
                    uCECAudioStatus.Map.AudioMuteStatus=~(uCECAudioStatus.Map.AudioMuteStatus);
                    break;

                case 0x65:	//Mute
                    uCECAudioStatus.Map.AudioMuteStatus=TRUE;
                    break;

                case 0x6C:	//Power OFF
                    u8_PowerStatus=1;	// standby
                    break;

                case 0x6D:	//Power ON
                    u8_PowerStatus=0;	// on
                    break;



            }

}

void IT6802_CECDeviceStateReset(void)
{
    if(Myself_LogicAdr==DEVICE_ID_TV)
    {
        LAState=sLANone;
        IT6802_CEC_PRINT(("IT6802_CECDeviceStateReset ==> DEVICE_ID_TV\n"));
    }
    else if( Myself_LogicAdr==DEVICE_ID_PLAYBACK1 || Myself_LogicAdr==DEVICE_ID_PLAYBACK2
        || Myself_LogicAdr==DEVICE_ID_PLAYBACK3)
    {
        PMState=sPMnone;
        IT6802_CEC_PRINT(("IT6802_CECDeviceStateReset ==> DEVICE_ID_PLAYBACK1\n"));
    }
}


//xxxxx 2014-0128
void IT6802_CECPhysicalAddressReset(void)
{
    if(Myself_LogicAdr==DEVICE_ID_TV)
    {
        IT6802_CEC_PRINT(("IT6802_CECPhysicalAddressReset ==> DEVICE_ID_TV\n"));
        txphyadr[0]=0x10;
        txphyadr[1]=0x00;
        rxphyadr[0][0] = 0x10;
        rxphyadr[0][1] = 0x00;
        rxphyadr[1][0] = 0x20;
        rxphyadr[1][1] = 0x00;

    }
    else if( Myself_LogicAdr==DEVICE_ID_PLAYBACK1 || Myself_LogicAdr==DEVICE_ID_PLAYBACK2
        || Myself_LogicAdr==DEVICE_ID_PLAYBACK3)
    {
        IT6802_CEC_PRINT(("IT6802_CECPhysicalAddressReset ==> DEVICE_ID_PLAYBACK1\n"));
        txphyadr[0]=0x11;
        txphyadr[1]=0x00;
    }

    IT6802_CEC_PRINT(("txphyadr[0]=%02X\n",(int) txphyadr[0]));
    IT6802_CEC_PRINT(("txphyadr[1]=%02X\n",(int) txphyadr[1]));
    IT6802_CEC_PRINT(("rxphyadr[0][0]=%02X\n",(int) rxphyadr[0][0]));
    IT6802_CEC_PRINT(("rxphyadr[0][1]=%02X\n",(int) rxphyadr[0][1]));
    IT6802_CEC_PRINT(("rxphyadr[1][0]=%02X\n",(int) rxphyadr[1][0]));
    IT6802_CEC_PRINT(("rxphyadr[1][0]=%02X\n",(int) rxphyadr[1][1]));


}


void IT6802_CECPowerOffByIT680X(unsigned char eCECActivePortNum)
{
    if(eCECActivePortNum == 0)
    {
        IT6802_CEC_PRINT(("IT6802_CECPower Off By IT680X() active port --> HDMI PORT 0\n"));
    }
    else
    {
        IT6802_CEC_PRINT(("IT6802_CECPower Off By IT680X() active port --> HDMI PORT 1\n"));
    }

    CECTest(eStandBy, eCECActivePortNum);
}


void IT6802_CECPowerOnByIT680X(unsigned char eCECActivePortNum)
{
    if(eCECActivePortNum == 0)
    {
        IT6802_CEC_PRINT(("IT6802_CECPowerOn By IT680X() active port --> HDMI PORT 0\n"));
    }
    else
    {
        IT6802_CEC_PRINT(("IT6802_CECPowerOn By IT680X() active port --> HDMI PORT 1\n"));
    }

    CECTest(eSetStreamPath, eCECActivePortNum);
}

void CECReceivePowerOn_Callback(unsigned char eCECActivePortNum)
{
    if(eCECActivePortNum ==0)
    {
        IT6802_CEC_PRINT(("CECReceivePowerOn_Callback() active port --> HDMI PORT 0\n"));
    }
    else
    {
        IT6802_CEC_PRINT(("CECReceivePowerOn_Callback() active port --> HDMI PORT 1\n"));
    }
}

void CECReceivePowerOff_Callback(void)
{
    IT6802_CEC_PRINT(("00000 CECReceivePowerOff_Callback()\n"));
}

void IT6802CEC_RegisterCallBack(void * CECReceivePowerOff_Callback,void * CECReceivePowerOn_Callback)
{
    m_CECcallbacks.CECReceivePowerOff_Callback = CECReceivePowerOff_Callback;
    m_CECcallbacks.CECReceivePowerOn_Callback = CECReceivePowerOn_Callback;
    pm_CECcallbacks = &m_CECcallbacks;
}
//xxxxx
#endif


#endif	// __USE_IT6802_RX__

