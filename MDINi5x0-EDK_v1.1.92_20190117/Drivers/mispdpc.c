// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"misp100.h"
#include	"video.h"

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------

	
// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
static MISP_DPD_BUFFER DPD_BUFFER;
static NOR_HDPC_EEPROM NHDPC_DATA;
static SDPC_CTRL_PARAM stSDPCCTRL;

static DDPC_PARAM stDDPC_PARAM;

//case control : auto iris? sensor?
//MCU program : DPD, DPC, EEPROM, IRKEYDISPLAY Control
//--------------------------------------------------------------------------------------------------------------------------
static void dpdpara_set(void)
{
	//save orgin setting

	//set iris close(af) or waiting routine(manual mode is check)

	//set sensor(shut, gain...)
	if(stSDPCCTRL.fwait_cnt==0){
		stSDPCCTRL.fwait_cnt=1;
		UARTprintf("[SDPD]parameter setup wait  ");		
	}
	else if(stSDPCCTRL.fwait_cnt<DPD_SENSOR_WAIT) 	stSDPCCTRL.fwait_cnt++;
	else{

		//at thhd mode, find x :    lower thhd< x < upper thhd		
		#if defined(__USE_DPD_i51X__)	
		MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_upper_thhd, stSDPCCTRL.upper_thhd);	
		MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_lower_thhd, stSDPCCTRL.lower_thhd);			
//		MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 0, 2, 3);							//dpd_pixcnt_en=1;
		MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 1, 1, 1);							//dpd_pixcnt_en=1;

		MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 2, 2, stSDPCCTRL.search_type);		

		UARTprintf("\r\n[SDPD]lower thhd : %d \r\n",  stSDPCCTRL.lower_thhd);
		UARTprintf("\r\n[SDPD]parameter setup end\r\n");
		
		#else
		//set dpd register
		MISP_RegField(MISP_LOCAL_ID, reg_sdpd_pixel_thhd, 0, 8, stSDPCCTRL.hot_thhd);			 //set hot pixel thresholds 	
		MISP_RegField(MISP_LOCAL_ID, reg_sdpd_pixel_thhd, 8, 8, stSDPCCTRL.dead_thhd);		 //set dead pixel thresholds 	

		MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 0, 1, stSDPCCTRL.rp_nfs);		 	//set  dpd parameter
		MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 0, 1, 1);		 					
		#endif

		stSDPCCTRL.mode=MISP_SDPD_CHK;		//overflow
		stSDPCCTRL.fwait_cnt=0;
		UARTprintf("\r\n[SDPD]parameter setup end\r\n");
		
	}

	if(stSDPCCTRL.fwait_cnt>0)	UARTprintf("%d..  ",stSDPCCTRL.fwait_cnt);
	
}

static void dpd_check_para(void)
{
	WORD scope;

	#if defined(__USE_MDIN_i500__) 						// MDIN-i500
	WORD env_size=NH_SIZ;
	#else 												// MDIN-i51X (i510, i540, i550)
	WORD env_size=NH_SIZ*4;
	#endif
	

	if(stSDPCCTRL.scnt<4){
		MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_scope_cnt, &scope); 		//i5xx 0x4BA, i500 0x4B9

		stSDPCCTRL.scope[stSDPCCTRL.scnt]	= scope;
		stSDPCCTRL.scnt++;

		if(scope>env_size){
			stSDPCCTRL.scope[0] = 0;			
			stSDPCCTRL.scope[1] = 0;	
			stSDPCCTRL.scope[2] = 0;						
			stSDPCCTRL.scope[3] = 0;									
			stSDPCCTRL.scnt=0;			

			stSDPCCTRL.mode=MISP_SDPD_DECIDE;						
			stSDPCCTRL.scnt=0;	
			UARTprintf("[DPD]environment setting err. scope : %d \r\n", scope);			
		}
	}
	else{

		//ALL 0? there is no dpc
		stSDPCCTRL.mode=MISP_SDPD_DECIDE;						

		//there is dead pixel
		if(stSDPCCTRL.scope[0]>0)	stSDPCCTRL.mode=MISP_SDPD_RST;			
		if(stSDPCCTRL.scope[1]>0)	stSDPCCTRL.mode=MISP_SDPD_RST;	
		if(stSDPCCTRL.scope[2]>0) stSDPCCTRL.mode=MISP_SDPD_RST;	
		if(stSDPCCTRL.scope[3]>0) stSDPCCTRL.mode=MISP_SDPD_RST;	

		if((stSDPCCTRL.mode==MISP_SDPD_DECIDE)||(stSDPCCTRL.mode==MISP_SDPD_END)||(stSDPCCTRL.mode==MISP_SDPD_OFF)){
			//return agc, ai parameter here
			UARTprintf("[SDPD]0 defect pixel in this mode \r\n");		
		}
		else	 UARTprintf("[SDPD]go to dpc reset mode max_scope = %d, %d, %d, %d \r\n", stSDPCCTRL.scope[0],stSDPCCTRL.scope[1],stSDPCCTRL.scope[2],stSDPCCTRL.scope[3]);

		
	}
	
}
//---------------------------------------------Reset-------------------------------------------------------------------
static void DPD_BuffReset(void)
{

	DPD_BUFFER.dpd_cycleendflag=0;
	DPD_BUFFER.dpd_errcnt_frame=0;
	DPD_BUFFER.dpd_scope_cnt=0;

	#ifndef __USE_DPD_i51X__	
	DPD_BUFFER.dpd_repeat_cnt_chk=0;
	
	DPD_BUFFER.x_pos_buf_0=0;			// static dead pixel x position 0
	DPD_BUFFER.x_pos_buf_1=0;			// static dead pixel x position 1
	DPD_BUFFER.x_pos_buf_2=0;			// static dead pixel x position 2
	DPD_BUFFER.x_pos_buf_3=0;			// static dead pixel x position 3
	DPD_BUFFER.y_pos_buf_0=0;			// static dead pixel x position 0
	DPD_BUFFER.y_pos_buf_1=0;			// static dead pixel x position 1
	DPD_BUFFER.y_pos_buf_2=0;			// static dead pixel x position 2
	DPD_BUFFER.y_pos_buf_3=0;			// static dead pixel x position 3
	#endif
	

}

static void DPC_DataReset(void)
{
	WORD k;

	for(k=0;k<NH_SIZ;k++){
		NHDPC_DATA.x[k]=0;
		NHDPC_DATA.y[k]=0;	
		NHDPC_DATA.byr_dpc_mode[k]=0;		
		#if defined (DPD_COUNT_CUT_USE)			
		NHDPC_DATA.find[k]=0;
		#endif
	}

	NHDPC_DATA.l=0;

	if(stSDPCCTRL.init)	UARTprintf("[SDPD]MCU BUFFER reset before read sequence \r\n");		
	else 				UARTprintf("[SDPD]MCU BUFFER reset for init \r\n");		
}

static void DPD_LogicReset(void)
{
	#if defined(__USE_DPD_i51X__)		
	MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 0, 2, 0);		
	MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 0, 2, 3);					
	#else
	//this is dpd rtl reset
	MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_parameter, 0);							//dpd rtl logic reset - off					
	MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_parameter, stSDPCCTRL.rp_n*16+1);		//dpd rtl logic reset - on
	#endif
	
	stSDPCCTRL.mode=MISP_SDPD_READ;			

	UARTprintf("[DPD]DPD LOGIC reset before read sequence \r\n");	
	UARTprintf("--------------------------------------\r\n"); 		
}

//---------------------------------------MEMORY------------------------------------------------------
#if defined(__USE_MDIN_i500__)
static void SDPC_MemWriteTrig(void)
{
	MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_rd_addr, 0);  // addr
	MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_xpos, 0);	 	
	MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_ypos, 0);	
	MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, 0);	
	
	MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, 0x8000);	 	
	MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, 0);	 		
	Delay_mSec(1);	
}
#endif
static void DPC_RaddrReset(void)
{
	// dpc read addr reset
	MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_rd_addr, 1000);  	
	MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_rd_addr, 0);  		
}

static void DPC_MemoryReset(WORD start, WORD end)
{
	WORD i;
	WORD x,y;
	WORD err_cnt=0;

	for(i=start; i<end;i++){
		MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_xpos, 0);						//dp data write x position : 0x42C [12:0]
		MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_ypos, 0);						//dp data write y position : 0x42D [11:0], dp data normal flag  : 0x42D [12]    normal pixel - 1	 hot pixel - 0 
		MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, i);  // address should be written before write flag
		Delay_uSec(30);		
		MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, (WORD)(i+0x8000));		//1. dp data write address  :  0x42B [10:0]  valid 0-2046 (not 2047)
		Delay_uSec(30);		
		MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, (WORD)i);	 
	 }	

	for(i=start; i<end;i++){
		MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_rd_addr, i);  	//	dp data read address : 0x428 [10:0] valid 0-2046  
		Delay_uSec(30);	//insert wait before reading after writing address
		MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_memory_rd_xpos, &x);  //	dp data read x position : 0x429 [12:0]
		MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_memory_rd_ypos, &y);  // 	dp data read y position : 0x42A [11:0]

		if((x!=0)||(y!=0))
		{
			UARTprintf("\r\n rewrite point = (i,x,y)=(%d,%d,%d) \r\n", i, x, y);				
			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_xpos, 0);
			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_ypos, 0);
			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, (WORD)(i+0x8000));  // wr+addr
			Delay_uSec(30);			
			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, (WORD)i);  // ~wr
			err_cnt++;	
		}			
	 }	

#if defined(__USE_MDIN_i500__)
	//final start reset
	MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_xpos, 0);
	MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_ypos, 0);
	MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, i);  // address should be written before write flag
	MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, 0x8000|i);  // wr+addr
	MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, i);  // ~wr

	MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_rd_addr, 0);  // addr

	SDPC_MemWriteTrig();
#endif

	if(stSDPCCTRL.init) 	UARTprintf("\r\n[DPC]Zero Write to DPC inner memory, zero rewrite count = %d \r\n", err_cnt);		
	else					UARTprintf("\r\n[DPC]Zero Write for init. zero rewrite count = %d\r\n",err_cnt);		


}

static void DPC_MemoryRead(void)
{
#if defined(__USE_DPD_i51X__)
	WORD start=0, end=NH_SIZ;
#else
	WORD start=1, end=NH_SIZ;
#endif

	WORD addr, x, y, mode;

	UARTprintf("\r\n [dpc_buffer_read start] \r\n");

	for(addr=start; addr<end; addr++)
	{
		MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_rd_addr, addr);  // addr
		Delay_uSec(30);	//insert wait before reading after writing address
		MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_memory_rd_xpos, &x);  // x
		MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_memory_rd_ypos, &y);  // y

		mode=(y&&0x1000)>>12;

		UARTprintf("\r\n %04d: %04d,%04d %d \r\n",addr, x, y, mode);

		if((x==0) && (y==0)) break;
	}

	UARTprintf("\r\n [dpc_buffer_read end] \r\n");			


}

static void DPC_MemoryWrite(void)
{
	WORD i=0;
	WORD length;
	WORD x,y;
	WORD cnt_err=0, saddr_err=0, eaddr_err=0;

#if defined(__USE_DPD_i51X__)
	WORD reg_y;

	if(NHDPC_DATA.l>NH_SIZ+1)	{	UARTprintf("[SDPC] return memorywrite for overflow(length=%d) \r\n",NHDPC_DATA.l);	
									UARTprintf("[SDPC] There is a possibility that there is no data for such reasons : erasing flash recently .. etc \r\n");	
									return;	
								}

	// not use SDPC_MemWriteTrig(); at i5xx, because addr start 0 at i5xx
	UARTprintf("[SDPC]Position Write to DPC inner memory \r\n");	
	
	if(stSDPCCTRL.ctrl&SDPC_NORMAL_COND)
	{
		length=NHDPC_DATA.l;

		for(i=0; i<length;i++)
		{
			reg_y=NHDPC_DATA.y[i];
//			reg_y=NHDPC_DATA.y[i]+(NHDPC_DATA.byr_dpc_mode[i]||0x1000);
				
			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_xpos, NHDPC_DATA.x[i]);
			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_ypos, reg_y);			
			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, i);  // address should be written before write flag
			Delay_uSec(30);	//insert wait before reading after writing address				
			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, 0x8000|i);  // wr+addr
			Delay_uSec(30);	//insert wait before reading after writing address				
			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, i);  // ~wr

			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_rd_addr, i);  // addr
			Delay_uSec(30);	//insert wait before reading after writing address
			MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_memory_rd_xpos, &x);  // x		
			MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_memory_rd_ypos, &y);  // y

			if((x!=NHDPC_DATA.x[i])||(y!=reg_y))
			{
				MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_xpos, NHDPC_DATA.x[i]);
				MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_ypos, reg_y);
				MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, i);  // address should be written before write flag
				Delay_uSec(30);	//insert wait before reading after writing address				
				MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, 0x8000|i);  // wr+addr
				Delay_uSec(30);	//insert wait before reading after writing address	
				MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, i);  // ~wr

				if(cnt_err==0){	saddr_err=i;		eaddr_err=i;	}
				else		eaddr_err=i;

				cnt_err++;
			}	

		}

		UARTprintf("[DPC]1st write finish/read check start(nh) \r\n");	
		UARTprintf("[DPC](length,cnt_err,saddr,eaddr)=(%d, %d,%d,%d) \r\n",length, cnt_err, saddr_err, eaddr_err);	

		if(cnt_err>0)	
		{	
			#if 1
			cnt_err=0;
			eaddr_err=MIN(eaddr_err+1, length);
		
			for(i=saddr_err; i<eaddr_err;i++)
			{
				reg_y=NHDPC_DATA.y[i];
//				reg_y=NHDPC_DATA.y[i]+(NHDPC_DATA.byr_dpc_mode[i]||0x1000);			
				
				MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_rd_addr, i);  // addr
				Delay_uSec(30);	//insert wait before reading after writing address			
				MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_memory_rd_xpos, &x);  // x				
				MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_memory_rd_ypos, &y);  // y

				if((x!=NHDPC_DATA.x[i])||(y!=reg_y))
				{

					MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_xpos, NHDPC_DATA.x[i]);
					MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_ypos, reg_y);
					MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, i);  // address should be written before write flag
					Delay_uSec(30);	//insert wait before reading after writing address					
					MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, 0x8000|i);  // wr+addr
					Delay_uSec(30);	//insert wait before reading after writing address						
					MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, i);  // ~wr
					cnt_err++;				
				}	

			}	

			UARTprintf("[DPC]2st write end (cnt_err)=(%d) \r\n",cnt_err);		


			#else
				DPC_MemoryWrite(); 		//replace code recursive style but not tested
			#endif
		}

		
	}
#else
	if(NHDPC_DATA.l>NH_SIZ+1)	{	UARTprintf("[SDPC] return memorywrite for overflow(length=%d) \r\n",NHDPC_DATA.l);	
									UARTprintf("[SDPC] There is a possibility that there is no data for such reasons : erasing flash recently .. etc \r\n");	
									return;	
								}


	UARTprintf("[SDPC]Position Write to DPC inner memory \r\n");	
	if(stSDPCCTRL.ctrl&SDPC_NORMAL_COND)
	{
		length=NHDPC_DATA.l+1;

		for(i=1; i<length;i++)
		{
			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_xpos, NHDPC_DATA.x[i]);
			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_ypos, NHDPC_DATA.y[i]);
			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, 0x8000|i);  // wr+addr
			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, i);  // ~wr

			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_rd_addr, i);  // addr
			Delay_uSec(30);	//insert wait before reading after writing address			
			MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_memory_rd_xpos, &x);  // x
			MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_memory_rd_ypos, &y);  // y

		if((x!=NHDPC_DATA.x[i])||(y!=NHDPC_DATA.y[i]))
		{
			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_xpos, NHDPC_DATA.x[i]);
			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_ypos, NHDPC_DATA.y[i]);
			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, 0x8000|i);  // wr+addr
			MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, i);  // ~wr

			if(cnt_err==0){	saddr_err=i;		eaddr_err=i;	}
			else		eaddr_err=i;

			cnt_err++;
		}	

		}

		UARTprintf("[DPC]1st write finish/read check start(nh) \r\n");	
		UARTprintf("[DPC](length,cnt_err,saddr,eaddr)=(%d, %d,%d,%d) \r\n",length, cnt_err, saddr_err, eaddr_err);	

		if(cnt_err>0)
		{	
			#if 1
			cnt_err=0;
			eaddr_err=MIN(eaddr_err+1, length);
		
			for(i=saddr_err; i<eaddr_err;i++)
			{
				MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_rd_addr, i);  // addr
				Delay_uSec(30);	//insert wait before reading after writing address			
				MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_memory_rd_xpos, &x);  // x
				MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_memory_rd_ypos, &y);  // y

				if((x!=NHDPC_DATA.x[i])||(y!=NHDPC_DATA.y[i]))
				{

					MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_xpos, NHDPC_DATA.x[i]);
					MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_ypos, NHDPC_DATA.y[i]);
					MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, 0x8000|i);  // wr+addr
					MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_memory_wr_addr, i);  // ~wr
					cnt_err++;				
				}	

			}	
			#else
				DPC_MemoryWrite(); 		//replace code recursive style but not tested
			#endif
			
			UARTprintf("[DPC]2st write end (cnt_err)=(%d) \r\n",cnt_err);			
		}

		SDPC_MemWriteTrig();
	}
	//--------------------------------------------------------------------

#endif
}

void DPD_MemPrint(void)
{
	int i;

	UARTprintf("-----------------------------\r\n"); 
	for(i=0; i<NHDPC_DATA.l ;i++) {
		#if defined (DPD_COUNT_CUT_USE)	
		UARTprintf("[DPD]addr:%d   x:%d   y:%d  find:%d   byr_dpc_mode:%d \r\n", i, NHDPC_DATA.x[i], NHDPC_DATA.y[i],  NHDPC_DATA.find[i], NHDPC_DATA.byr_dpc_mode[i]); 
		#else
		UARTprintf("[DPD]addr:%d   x:%d   y:%d  byr_dpc_mode:%d  \r\n", i, NHDPC_DATA.x[i], NHDPC_DATA.y[i], NHDPC_DATA.byr_dpc_mode[i]); 
		#endif
	}				
	UARTprintf("length of dpc_data : %d \r\n",  NHDPC_DATA.l); 	
	UARTprintf("-----------------------------\r\n"); 			

	
}
//--------------------------------------------------------------------------------------------------------------------------
static void DPD_RegisterRead(void)
{
#if defined(__USE_DPD_i51X__)
	BYTE i=0;
	WORD Xcut_start = GetSS_image_boundary_size_h() ;
	WORD Vcut_start = GetSS_image_boundary_size_v() ;
	WORD Xcut_end = GetSS_image_active_size_h() ;
	WORD Vcut_end = GetSS_image_active_size_v() ;
	
	WORD SDPC_YOFFSET=0, SDPC_YOFFSET_n=1;	

	WORD pos01[4];
	WORD pos234[6];
	WORD pos567[6];

	SDPC_YOFFSET=0; 	SDPC_YOFFSET_n=1;		

//	if(bv>0) {	SDPC_YOFFSET=bv-1;	SDPC_YOFFSET_n=0;	}
//	else {	SDPC_YOFFSET=0; SDPC_YOFFSET_n=1;	}

//	UARTprintf("sensor vertical boundary siz = %d  \r\n", bv); 	

	for(i=0 ; i<=stSDPCCTRL.buff_page_use; i++)
	{
		stSDPCCTRL.buff_page_sel=i;	
	
		//select page	
		MISP_RegWrite(MISP_LOCAL_ID,  reg_sdpd_buff_page, (stSDPCCTRL.buff_page_sel<<2)|
													(stSDPCCTRL.buff_page_use)
													);		
		//read pixel data
		MISP_MultiRead(MISP_LOCAL_ID, 0x4BC, (PBYTE)pos01, 8);			//i2c communication : sensor ucom <-> fpga board	
		MISP_MultiRead(MISP_LOCAL_ID, 0x22A, (PBYTE)pos234, 12);		//i2c communication : sensor ucom <-> fpga board		
		MISP_MultiRead(MISP_LOCAL_ID, 0x248, (PBYTE)pos567, 12);		//i2c communication : sensor ucom <-> fpga board			
			//-------------------------------------
			//#4B6#[7:0] dpd_frame_id	
			//#4B6#[14:9] dpd_errcnt_frame	- filtering count in current frame, max value dependent on dpd_buff_page_use. This is searched level
			//#4B6#[15] dpd_cycleendflag	- dpd chip process end signal
			//#4BA#[15:0]  dpd_scope_cnt   - total filtering count in current frame
			//#4BB#[3:2]  dpd_buff_page_sel	 - dpd_buff_page_sel(current page number)
			//#4BB#[1:0]  dpd_buff_page_use   - dpd_buff_page_use(max cnt transfer a frame. 0 : 8, 1: 16, 2:24, 3:32)
	
			//#4BC#[12:0]  dpd_xpos_reg_0   - 0 x buff
			//#4BD#[12:0]  dpd_ypos_reg_0   - 0 y buff		
			//#4BE#[12:0]  dpd_xpos_reg_1   - 1 x buff
			//#4BF#[12:0]  dpd_ypos_reg_1   - 1 y buff	
		
			//#22A#[12:0]  dpd_xpos_reg_2   - 2 x buff
			//#22B#[12:0]  dpd_ypos_reg_2   - 2 y buff		
			//#22C#[12:0]  dpd_xpos_reg_3   - 3 x buff
			//#22D#[12:0]  dpd_ypos_reg_3   - 3 y buff		
			//#22E#[12:0]  dpd_xpos_reg_4   - 4 x buff
			//#22F#[12:0]  dpd_ypos_reg_4   - 4 y buff		

			//#248#[12:0]  dpd_xpos_reg_5   - 5 x buff
			//#249#[12:0]  dpd_ypos_reg_5   - 5 y buff		
			//#24A#[12:0]  dpd_xpos_reg_6   - 6 x buff
			//#24B#[12:0]  dpd_ypos_reg_6   - 6 y buff		
			//#24C#[12:0]  dpd_xpos_reg_7   - 7 x buff
			//#24D#[12:0]  dpd_ypos_reg_7   - 7 y buff					
			//-------------------------------------
			// fpga only
			//-------------------------------------
			//#4BC#[15:13]  dpd_send_flag   - 2 ~0
			//#4BD#[15:13]  dpd_send_flag   - 5 ~3
			//#4BE#[15:13]  dpd_send_flag   - 8 ~6
			//#4BF#[15:13]  dpd_send_flag   - 11 ~9
			//#22A#[15:13]  dpd_send_flag   - 14 ~12
			//#22B#[15:13]  dpd_send_flag   - 17 ~15
			//#22C#[15:13]  dpd_send_flag   - 20 ~18
			//#22D#[15:13]  dpd_send_flag   - 23 ~21
			//#22E#[15:13]  dpd_send_flag   - 26 ~24
			//#22F#[15:13]  dpd_send_flag   - 29 ~27
			//#248#[15:13]  dpd_send_flag   - 31~30	
			//-------------------------------------
//		UARTprintf("dpd_send_flag[2:0] = %d  \r\n",i,  ((pos01[0]&0xE000)>>13)); 	

		//dpd data send flag value dismiss
		pos01[0]=pos01[0]&0x1FFF;		pos01[1]=pos01[1]&0x1FFF;		pos01[2]=pos01[2]&0x1FFF;		pos01[3]=pos01[3]&0x1FFF;					
		pos234[0]=pos234[0]&0x1FFF;	pos234[1]=pos234[1]&0x1FFF;	pos234[2]=pos234[2]&0x1FFF;	pos234[3]=pos234[3]&0x1FFF;	pos234[4]=pos234[4]&0x1FFF;	pos234[5]=pos234[5]&0x1FFF;				
		pos567[0]=pos567[0]&0x1FFF;	pos567[1]=pos567[1]&0x1FFF;	pos567[2]=pos234[2]&0x1FFF;	pos567[3]=pos567[3]&0x1FFF;	pos567[4]=pos567[4]&0x1FFF;	pos567[5]=pos567[5]&0x1FFF;		

//		2017.9.5 (pos_xx[1]) ---> (pos_xx[1]>1). because there is no need to correction y=0 line case


		if((pos01[0]>Xcut_start) && (pos01[1]>Vcut_start))	
		{	
			if((pos01[0]<Xcut_end) && (pos01[1]<Vcut_end))
			{
				NHDPC_DATA.x[NHDPC_DATA.l] = pos01[0] - SDPC_XOFFSET;
				NHDPC_DATA.y[NHDPC_DATA.l] = pos01[1] + SDPC_YOFFSET - SDPC_YOFFSET_n;	
				NHDPC_DATA.byr_dpc_mode[NHDPC_DATA.l] = stSDPCCTRL.dpc_mode;
				NHDPC_DATA.l++;
			}
		}

		if((pos01[2]>Xcut_start) && (pos01[3]>Vcut_start))	
		{	
			if((pos01[2]<Xcut_end) && (pos01[3]<Vcut_end))
			{		
				NHDPC_DATA.x[NHDPC_DATA.l] = pos01[2] - SDPC_XOFFSET;
				NHDPC_DATA.y[NHDPC_DATA.l] = pos01[3] + SDPC_YOFFSET - SDPC_YOFFSET_n;	
				NHDPC_DATA.byr_dpc_mode[NHDPC_DATA.l] = stSDPCCTRL.dpc_mode;	
				NHDPC_DATA.l++;
			}
		}

		if((pos234[0]>Xcut_start) && (pos234[1]>Vcut_start))	
		{
			if((pos234[0]<Xcut_end) && (pos234[1]<Vcut_end))	
			{
				NHDPC_DATA.x[NHDPC_DATA.l] = pos234[0] - SDPC_XOFFSET;
				NHDPC_DATA.y[NHDPC_DATA.l] = pos234[1] + SDPC_YOFFSET - SDPC_YOFFSET_n;	
				NHDPC_DATA.byr_dpc_mode[NHDPC_DATA.l] = stSDPCCTRL.dpc_mode;		
				NHDPC_DATA.l++;
			}
			
		}

		if((pos234[2]>Xcut_start) && (pos234[3]>Vcut_start))	
		{
			if((pos234[2]<Xcut_end) && (pos234[3]<Vcut_end))	
			{		
				NHDPC_DATA.x[NHDPC_DATA.l] = pos234[2] - SDPC_XOFFSET;
				NHDPC_DATA.y[NHDPC_DATA.l] = pos234[3] + SDPC_YOFFSET - SDPC_YOFFSET_n;	
				NHDPC_DATA.byr_dpc_mode[NHDPC_DATA.l] = stSDPCCTRL.dpc_mode;		
				NHDPC_DATA.l++;
			}
		}
	
		if((pos234[4]>Xcut_start) && (pos234[5]>Vcut_start))	
		{
			if((pos234[4]<Xcut_end) && (pos234[5]<Vcut_end))	
			{					
				NHDPC_DATA.x[NHDPC_DATA.l] = pos234[4] - SDPC_XOFFSET;
				NHDPC_DATA.y[NHDPC_DATA.l] = pos234[5] + SDPC_YOFFSET - SDPC_YOFFSET_n;	
				NHDPC_DATA.byr_dpc_mode[NHDPC_DATA.l] = stSDPCCTRL.dpc_mode;		
				NHDPC_DATA.l++;
			}			
		}

		if((pos567[0]>Xcut_start) && (pos567[1]>Vcut_start))	
		{
			if((pos567[0]<Xcut_end) && (pos567[1]<Vcut_end))	
			{				
				NHDPC_DATA.x[NHDPC_DATA.l] = pos567[0] - SDPC_XOFFSET;
				NHDPC_DATA.y[NHDPC_DATA.l] = pos567[1] + SDPC_YOFFSET - SDPC_YOFFSET_n;	
				NHDPC_DATA.byr_dpc_mode[NHDPC_DATA.l] = stSDPCCTRL.dpc_mode;		
				NHDPC_DATA.l++;
			}
		}

		if((pos567[2]>Xcut_start) && (pos567[3]>Vcut_start))	
		{
			if((pos567[2]<Xcut_end) && (pos567[3]<Vcut_end))	
			{				
				NHDPC_DATA.x[NHDPC_DATA.l] = pos567[2] - SDPC_XOFFSET;
				NHDPC_DATA.y[NHDPC_DATA.l] = pos567[3] + SDPC_YOFFSET - SDPC_YOFFSET_n;	
				NHDPC_DATA.byr_dpc_mode[NHDPC_DATA.l] = stSDPCCTRL.dpc_mode;		
				NHDPC_DATA.l++;
			}
		}
	
		if((pos567[4]>Xcut_start) && (pos567[5]>Vcut_start))	
		{
			if((pos567[4]<Xcut_end) && (pos567[5]<Vcut_end))	
			{				
				NHDPC_DATA.x[NHDPC_DATA.l] = pos567[4] - SDPC_XOFFSET;
				NHDPC_DATA.y[NHDPC_DATA.l] = pos567[5] + SDPC_YOFFSET - SDPC_YOFFSET_n;	
				NHDPC_DATA.byr_dpc_mode[NHDPC_DATA.l] = stSDPCCTRL.dpc_mode;	
				NHDPC_DATA.l++;
			}
		}


	}


		stSDPCCTRL.buff_page_sel=0;

#else
	WORD rVal[7];
	//read pixel data
	MISP_MultiRead(MISP_LOCAL_ID, reg_sdpd_scope_cnt, (PBYTE)rVal, 14);		//i2c communication : sensor ucom <-> fpga board	
		//-------------------------------------
			//(16#4B9#, X"0000", X"0000"), --  dpd_scope_cnt(reg_sdpd_scope_cnt)
			//(16#4BA#, X"0000", X"0000"), --  dpd_x&y pos_reg0~3
			//(16#4BB#, X"0000", X"0000"), --  dpd_x&y pos_reg0~3
			//(16#4BC#, X"0000", X"0000"), --  dpd_x&y pos_reg0~3
			//(16#4BD#, X"0000", X"0000"), --  dpd_x&y pos_reg0~3
			//(16#4BE#, X"0000", X"0000"), --  dpd_x&y pos_reg0~3
			//(16#4BF#, X"0000", X"0000"), --  dpd_x&y pos_reg0~3
		//-------------------------------------
	//dpd save process
	DPD_BUFFER.dpd_scope_cnt= rVal[0];
	//UARTprintf("######tot:%d##frame:%d########\r\n",DPD_BUFFER.dpd_scope_cnt, DPD_BUFFER.dpd_errcnt_frame );


	if(DPD_BUFFER.dpd_errcnt_frame){		//case 4,3,2,1
	DPD_BUFFER.x_pos_buf_0 =  (rVal[1] >> 4);												//[15:4]
	DPD_BUFFER.y_pos_buf_0 =  ((WORD)((rVal[1]&0x000f) << 8)) + ((WORD)((rVal[2]&0xff00) >> 8));	 //[3:0]&[15:8]
	//UARTprintf("[DPD]0pos  x:%d   y:%d  \r\n", DPD_BUFFER.x_pos_buf_0, DPD_BUFFER.y_pos_buf_0);	
	}

	if(DPD_BUFFER.dpd_errcnt_frame>1)	{	//case 4,3,2
	DPD_BUFFER.x_pos_buf_1 =  ((WORD)((rVal[2]&0x00ff) << 4)) + ((WORD)((rVal[3]&0xf000) >> 12));	//[8:0]&[15:12]
	DPD_BUFFER.y_pos_buf_1 =  (rVal[3]&0x0fff);
	//UARTprintf("[DPD]0pos  x:%d   y:%d  \r\n", DPD_BUFFER.x_pos_buf_1, DPD_BUFFER.y_pos_buf_1);	
	}

	if(DPD_BUFFER.dpd_errcnt_frame>2)	{	//case 4,3
	DPD_BUFFER.x_pos_buf_2 =  (rVal[4] >> 4);
	DPD_BUFFER.y_pos_buf_2 =  ((WORD)((rVal[4]&0x000f) << 8)) + ((WORD)((rVal[5]&0xff00) >> 8));	 //[3:0]&[15:8]	
	//UARTprintf("[DPD]0pos  x:%d   y:%d  \r\n", DPD_BUFFER.x_pos_buf_2, DPD_BUFFER.y_pos_buf_2);	
	}

	if(DPD_BUFFER.dpd_errcnt_frame>3)	{	//case 4
	DPD_BUFFER.x_pos_buf_3 =  ((WORD)((rVal[5]&0x00ff) << 4)) + ((WORD)((rVal[6]&0xf000) >> 12));	//[8:0]&[15:12]
	DPD_BUFFER.y_pos_buf_3 =  (rVal[6]&0x0fff);
	//UARTprintf("[DPD]0pos  x:%d   y:%d  \r\n", DPD_BUFFER.x_pos_buf_3, DPD_BUFFER.y_pos_buf_3);	

	}
#endif

}

static void DPC_DataSorting(WORD x, WORD y, BYTE dp_mode)
{
	LONG i=0;
	WORD size_short=0;
	WORD size_equal=0;
	WORD mark_i=0;

	//if same postion data exsist? yes is return 
	for(i=0; i<NHDPC_DATA.l;i++){
		if(y==NHDPC_DATA.y[i]){
			if(x==NHDPC_DATA.x[i]){
				#if defined (DPD_COUNT_CUT_USE)	
				NHDPC_DATA.find[i]++;	
				#endif
				
				NHDPC_DATA.byr_dpc_mode[i] |= dp_mode;

				return;		
			}

			if(y==NHDPC_DATA.y[i+1]){
				if(x<NHDPC_DATA.x[i]){
					size_equal++;
					mark_i=i;
					break;
				}
				else{
					size_equal++;
					mark_i=i+1;	//mark_i=size_short+size_equal;
				}
			}
			else{
				if(x<NHDPC_DATA.x[i]){
					size_equal++;
					mark_i=i;		//mark_i=size_short+size_equal;
					break;
				}
				else{	
					size_equal++;
					mark_i=i+1;		//mark_i=size_short+size_equal;
					break;
				}
			}

			
		}
		else if(y<NHDPC_DATA.y[i]) {	
			mark_i=i;
			break;			
		}
		else	 {
			size_short++;			
			mark_i=i+1;

		}

	}

	//make space --> 
	NHDPC_DATA.l++;	

	for(i=NHDPC_DATA.l; i>mark_i;i--){
		NHDPC_DATA.x[i] = NHDPC_DATA.x[i-1];
		NHDPC_DATA.y[i] = NHDPC_DATA.y[i-1];		
		NHDPC_DATA.byr_dpc_mode[i] = NHDPC_DATA.byr_dpc_mode[i-1];
		#if defined (DPD_COUNT_CUT_USE)	
		NHDPC_DATA.find[i] = NHDPC_DATA.find[i-1];
		#endif				
	}

	//then add(write new pixel pos)		
	NHDPC_DATA.x[mark_i] = x;
	NHDPC_DATA.y[mark_i] = y;	
	NHDPC_DATA.byr_dpc_mode[mark_i] = dp_mode;	
	#if defined (DPD_COUNT_CUT_USE)			
	NHDPC_DATA.find[mark_i] = 1;			
	#endif		

	
	
}


#ifndef __USE_DPD_i51X__
static void DPD_DataFilter(BYTE nID)
{
#if defined (DPD_COUNT_CUT_USE)	
	WORD i=0;
	WORD j=0;

	if(nID<2){	UARTprintf("Filtering lv is under 1 = %d\r\n", nID);	return;	}
	
	while(i<NHDPC_DATA.l){
		if(NHDPC_DATA.find[i]<nID){
			for(j=i; j<NHDPC_DATA.l; j++){
			NHDPC_DATA.x[j] = NHDPC_DATA.x[j+1];
			NHDPC_DATA.y[j] = NHDPC_DATA.y[j+1];					
			NHDPC_DATA.find[j] = NHDPC_DATA.find[j+1];
			}

			if(NHDPC_DATA.l)	NHDPC_DATA.l--;

		}
		else i++;
	}
#endif	
}


static void DPC_DataSave(WORD x, WORD y, WORD dp_mode)
{
//	LONG i;
//	WORD mark_i=0;

	if(DPD_BUFFER.dpd_repeat_cnt_chk){
		//if length near overflow, then filtering
		#if defined (DPD_COUNT_CUT_USE)			
//		if(NHDPC_DATA.l>NH_SIZ-100) DPD_DataFilter(2);
		#endif

		#if 1
			DPC_DataSorting(x,y,dp_mode);
		#else
			//if same postion data exsist? yes is return 
			for(i=0; i<NHDPC_DATA.l;i++){
				if(y==NHDPC_DATA.y[i]){
					if(NHDPC_DATA.x[i]<NHDPC_DATA.x[i+1]) mark_i=i+1;

					if(x==NHDPC_DATA.x[i]){
						#if defined (DPD_COUNT_CUT_USE)	
						NHDPC_DATA.find[i]++;	
						#endif
						return;		
					}
				}

			
			}
			//new err pixel find, find pos exceptional case--> 
			if(mark_i==0){
				for(i=0; i<NHDPC_DATA.l;i++){			
					if(NHDPC_DATA.y[i]<y) mark_i=i+1;
				}
			}

			//make space --> 
			NHDPC_DATA.l++;	
			for(i=NHDPC_DATA.l; i>mark_i;i--){
			NHDPC_DATA.x[i] = NHDPC_DATA.x[i-1];
			NHDPC_DATA.y[i] = NHDPC_DATA.y[i-1];		
			#if defined (DPD_COUNT_CUT_USE)	
			NHDPC_DATA.find[i] = NHDPC_DATA.find[i-1];
			#endif				
			}

			//then add(write)		
			NHDPC_DATA.x[mark_i] = x;
			NHDPC_DATA.y[mark_i] = y;	
			#if defined (DPD_COUNT_CUT_USE)			
			NHDPC_DATA.find[mark_i] = 1;			
			#endif		
		#endif

	}
	else{
		NHDPC_DATA.x[NHDPC_DATA.l] = x;
		NHDPC_DATA.y[NHDPC_DATA.l] = y;	
		NHDPC_DATA.byr_dpc_mode[NHDPC_DATA.l] = dp_mode;			
		#if defined (DPD_COUNT_CUT_USE)					
		NHDPC_DATA.find[NHDPC_DATA.l] = 1;	
		#endif		
		
		NHDPC_DATA.l++;	
	}
}
#endif



static void DPD_DataRead(void)
{
#if defined(__USE_DPD_i51X__)
	WORD dpd_status;

	//read rtl parameter data
	MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_rd_status, &dpd_status);

	stSDPCCTRL.frame_id=dpd_status&0x00FF;
	DPD_BUFFER.dpd_errcnt_frame=HIBYTE((dpd_status&0x3F00));		
	
	if(dpd_status&0x8000) DPD_BUFFER.dpd_cycleendflag=1;
	else DPD_BUFFER.dpd_cycleendflag=0;

	if(GetDPC_DebugDisp()==2)	UARTprintf("[SDPD] frame id = %d / errcnt(fr) = %d / cycle end flag = %d \r\n", stSDPCCTRL.frame_id, DPD_BUFFER.dpd_errcnt_frame,DPD_BUFFER.dpd_cycleendflag );

	if(stSDPCCTRL.frame_id>1) DPD_RegisterRead();		//frame id 0 and 1... can't output

	if(NHDPC_DATA.l>NH_SIZ+1)
	{	
		UARTprintf("[SDPC] return memorywrite for overflow(length=%d) \r\n",NHDPC_DATA.l);	
		UARTprintf("[SDPC] There is a possibility that there is no data for such reasons : erasing flash recently .. etc \r\n");	

		MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 0, 1, 0);	
		stSDPCCTRL.mode=MISP_SDPD_T_FLASH;			
		return;	
	}
	
	if(DPD_BUFFER.dpd_cycleendflag==1)
	{
		MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 0, 1, 0);			
		stSDPCCTRL.mode=MISP_SDPD_T_FLASH;	
 		if(GetDPC_DebugDisp()==2)	DPD_MemPrint();
		UARTprintf("[SDPD] position read end\r\n");			
	}		

	DPD_BuffReset();		

		
#else	//-----------------------------------i500 code--------------------------------------------------------------
	WORD dpd_status;
	WORD SDPC_YOFFSET, SDPC_YOFFSET_n;
	BYTE bv = GetSS_image_boundary_size_v();
		//------------------------------------------------------------------------------------------	
			//(16#4B3#, X"FFFF", X"0000"), --dpd_enable, mode,disp_sel,repeat_cnt,
			//(16#4B4#, X"FFFF", X"0000"), --hot thhd, dead_thhd
			//(16#4B5#, X"0000", X"0000"), --dpd_repeat_cnt_chk, dpd_errcnt_frame, dpd_cycleendflag - read register
			//(16#4B6#, X"0000", X"0000"), -- dpd_pixel_val    - read register
			//(16#4B7#, X"FFFF", X"0000"), -- dpd_pixel_xpos  - write position for proof
			//(16#4B8#, X"FFFF", X"0000"), -- dpd_pixel_ypos  - write position for proof
		//------------------------------------------------------------------------------------------

	//read rtl parameter data
	MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_rd_status, &dpd_status);
	DPD_BUFFER.dpd_repeat_cnt_chk=(dpd_status&0x3E0) >> 5;
	DPD_BUFFER.dpd_errcnt_frame=(dpd_status&0x1C) >> 1;
	DPD_BUFFER.dpd_cycleendflag=dpd_status&0x01;

//	UARTprintf("dpd_repeat_cnt_chk = %d\r\n", DPD_BUFFER.dpd_repeat_cnt_chk);	

		//if end... finish dpd logic
	if(DPD_BUFFER.dpd_cycleendflag==1){
		UARTprintf("[SDPD] repeat_cnt_chk = %d\r\n", DPD_BUFFER.dpd_repeat_cnt_chk);	
		stSDPCCTRL.cyl=0;

		if(DPD_BUFFER.dpd_repeat_cnt_chk==stSDPCCTRL.rp_n){
			MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 0, 1, 0);			
			stSDPCCTRL.mode=MISP_SDPD_DECIDE;	
			DPD_BuffReset();					
//			DPD_MemPrint();		//check debug print	
			DPD_DataFilter(stSDPCCTRL.cut);
//			DPD_MemPrint();		//check debug print				
			SetSDPC_MemWrite_Order();			
		}

	}		

	DPD_RegisterRead();

	if(NHDPC_DATA.l>NH_SIZ+1)
	{	
		UARTprintf("[SDPC] return memorywrite for overflow(length=%d) \r\n",NHDPC_DATA.l);	
		UARTprintf("[SDPC] There is a possibility that there is no data for such reasons : erasing flash recently .. etc \r\n");	
		
		MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 0, 1, 0);	
		stSDPCCTRL.mode=MISP_SDPD_DECIDE;			
		SetSDPC_FlashRead_Order();
		return;	
	}
	
	
	if(bv>0) {	SDPC_YOFFSET=bv-1;	SDPC_YOFFSET_n=0;	}
	else {	SDPC_YOFFSET=0; SDPC_YOFFSET_n=1;	}

	//save to pix pos data memory  - mode normal  hot
	if((DPD_BUFFER.x_pos_buf_0>0)&&(DPD_BUFFER.y_pos_buf_0>0)){
		DPC_DataSave(DPD_BUFFER.x_pos_buf_0  - SDPC_XOFFSET, DPD_BUFFER.y_pos_buf_0  + SDPC_YOFFSET - SDPC_YOFFSET_n, stSDPCCTRL.dpc_mode);
		stSDPCCTRL.cyl++;
	}

	if((DPD_BUFFER.x_pos_buf_1>0)&&(DPD_BUFFER.y_pos_buf_1>0)){	
		DPC_DataSave(DPD_BUFFER.x_pos_buf_1  - SDPC_XOFFSET, DPD_BUFFER.y_pos_buf_1  + SDPC_YOFFSET - SDPC_YOFFSET_n, stSDPCCTRL.dpc_mode);
		stSDPCCTRL.cyl++;		
	}

	if((DPD_BUFFER.x_pos_buf_2>0)&&(DPD_BUFFER.y_pos_buf_2>0)){		
		DPC_DataSave(DPD_BUFFER.x_pos_buf_2  - SDPC_XOFFSET, DPD_BUFFER.y_pos_buf_2  + SDPC_YOFFSET - SDPC_YOFFSET_n, stSDPCCTRL.dpc_mode);
		stSDPCCTRL.cyl++;		
		
	}

	if((DPD_BUFFER.x_pos_buf_3>0)&&(DPD_BUFFER.y_pos_buf_3>0)){			
		DPC_DataSave(DPD_BUFFER.x_pos_buf_3  - SDPC_XOFFSET, DPD_BUFFER.y_pos_buf_3  + SDPC_YOFFSET - SDPC_YOFFSET_n, stSDPCCTRL.dpc_mode);		
		stSDPCCTRL.cyl++;		
	}

	DPD_BuffReset();		
#endif
}
//--------------------------------------------FLASH---------------------------------------------------
static void dpd_flash_clear(WORD saddr)
{   
       DWORD	FlashAddr[5];

	if(saddr<DPD_FLASH_SECTOR2_ADDR){
		UARTprintf("[SDPC]Wrong flash(%d) sector access", saddr);	 
		return;	
	}

       // set flash addr
       FlashAddr[Start] = saddr * 4096;           // buff1 start addr	1sector is 4K(4096) byte. save sensor status and errcounter
       FlashAddr[ID] = FlashAddr[Start] + 2;     // flash trash check id
       FlashAddr[Xbuf] = FlashAddr[ID] + 2;     // buff2 start addr
       FlashAddr[Ybuf] = FlashAddr[Xbuf] + NH_SIZ*2;       // buff2 end addr
       FlashAddr[Mbuf] = FlashAddr[Ybuf] + NH_SIZ*2;       // buff3 end addr
       

#if defined(__USE_MDIN_i500__)
       // flash erase 4kbyte x 2setctor
       sf_erase_sector(saddr, 2);              // erase 2 sector(125~126 : 0x7D000 ~ 0x7EFFF)
#else
       // flash erase 4kbyte x 3setctor
       sf_erase_sector(saddr, 3);              // erase 3 sector(125~127)
#endif
       //sf_write(FlashAddr[ID], (PBYTE)&(DPC_ID), 2);	   

	if(saddr==DPD_FLASH_SECTOR_ADDR)		UARTprintf("[SDPC]flash clear(result area:1) end \r\n");		
	if(saddr==DPD_FLASH_TEMP_SECTOR_ADDR)		UARTprintf("[SDPC]flash clear(temp area) end \r\n");	
	if(saddr==DPD_FLASH_SECTOR2_ADDR)		UARTprintf("[SDPC]flash clear(result area:2) end \r\n");		
}

static void dpd_flash_write(WORD saddr)
{
/*
	NHDPC_DATA.th;				// count thhd
	NHDPC_DATA.l;				// final data length 
	NHDPC_DATA.x[NH_SIZ];		// static dead pixel x position(normal cond)
	NHDPC_DATA.y[NH_SIZ];		// static dead pixel y position(normal cond)
*/
     
       WORD	DPC_ID = 0x0500;
       DWORD	FlashAddr[5];

	if(saddr<DPD_FLASH_SECTOR2_ADDR){
		UARTprintf("[SDPC]Wrong flash(%d) sector access", saddr);	 
		return;	
	}
	
       // set flash addr
       FlashAddr[Start] = saddr * 4096;           // buff1 start addr	1sector is 4K(4096) byte. save sensor status and errcounter
       FlashAddr[ID] = FlashAddr[Start] + 2;     // flash trash check id
       FlashAddr[Xbuf] = FlashAddr[ID] + 2;     // buff2 start addr
       FlashAddr[Ybuf] = FlashAddr[Xbuf] +  NH_SIZ*2;       // buff2 end addr
       FlashAddr[Mbuf] = FlashAddr[Ybuf] + NH_SIZ*2;       // buff3 end addr       

	// check dpc size
	if(NHDPC_DATA.l==0) 			{	UARTprintf("there is no dp");	return;		}
	if(NHDPC_DATA.l>NH_SIZ+1)	{	UARTprintf("Return spi-flash write (length : %d)\r\n",NHDPC_DATA.l);	
														UARTprintf("[SDPC] There is a possibility that there is no data for such reasons : erasing flash recently .. etc \r \n");
														return;		}

	if(saddr==DPD_FLASH_SECTOR_ADDR)		UARTprintf("[SDPC]FLASH sector access 253,254,255 \r\n");		
	if(saddr==DPD_FLASH_TEMP_SECTOR_ADDR)		UARTprintf("[SDPC]FLASH sector access 250,251,252 \r\n");	
	if(saddr==DPD_FLASH_SECTOR2_ADDR)		UARTprintf("[SDPC]FLASH sector access 247,248,249 \r\n");	
	
       // flash erase 4kbyte x 2setctor
#if defined(__USE_MDIN_i500__)
       // flash erase 4kbyte x 2setctor
       sf_erase_sector(saddr, 2);              // erase 2 sector(125~126 : 0x7D000 ~ 0x7EFFF)
#else
       // flash erase 4kbyte x 3setctor
       sf_erase_sector(saddr, 3);              // erase 3 sector(125~127)
#endif

       sf_write(FlashAddr[Start], (PBYTE)&(NHDPC_DATA.l), 2);
       sf_write(FlashAddr[ID], (PBYTE)&(DPC_ID), 2);	   

      // flash position write
       sf_write(FlashAddr[Xbuf], (PBYTE)NHDPC_DATA.x, sizeof(NHDPC_DATA.x));
	sf_write(FlashAddr[Ybuf], (PBYTE)NHDPC_DATA.y, sizeof(NHDPC_DATA.y));     
	sf_write(FlashAddr[Mbuf], (PBYTE)NHDPC_DATA.byr_dpc_mode, sizeof(NHDPC_DATA.byr_dpc_mode));   	

	if(saddr==DPD_FLASH_SECTOR_ADDR)		UARTprintf("[SDPC]flash write end (result area, length : %d) \r\n",NHDPC_DATA.l);		
	if(saddr==DPD_FLASH_TEMP_SECTOR_ADDR)	UARTprintf("[SDPC]flash write end (temp area, length : %d) \r\n",NHDPC_DATA.l);		
	if(saddr==DPD_FLASH_SECTOR2_ADDR)		UARTprintf("[SDPC]flash write end (result2 area, length : %d) \r\n",NHDPC_DATA.l);			
	
}

static void dpd_flash_read(WORD saddr)
{
	//data flash to cpu memory and copy rtl logic.

       WORD	DPC_ID;
       DWORD  FlashAddr[5];

	if(saddr<DPD_FLASH_SECTOR2_ADDR){
		UARTprintf("[SDPC]Wrong flash(%d) sector access", saddr);	 
		return;	
	}

       // set flash addr
       FlashAddr[Start] = saddr * 4096;           // buff1 start addr	1sector is 4K(4096) byte. save sensor status and errcounter
       FlashAddr[ID] = FlashAddr[Start] + 2;     // flash trash check id       
       FlashAddr[Xbuf] = FlashAddr[ID] + 2;     // buff2 start addr
       FlashAddr[Ybuf] = FlashAddr[Xbuf] +  NH_SIZ*2;       // buff2 end addr
       FlashAddr[Mbuf] = FlashAddr[Ybuf] + NH_SIZ*2;       // buff3 end addr       


	DPC_DataReset();

	sf_read(FlashAddr[Start], (PBYTE)&(NHDPC_DATA.l), 2);
	sf_read(FlashAddr[ID], (PBYTE)&(DPC_ID), 2);

	if(DPC_ID!=0x0500){
		if(saddr==DPD_FLASH_SECTOR_ADDR) 		UARTprintf("[SDPC]init bypass mode(trash data in result1 area  : 0x%x) \r\n", DPC_ID);	 return;	
		if(saddr==DPD_FLASH_TEMP_SECTOR_ADDR)	UARTprintf("[SDPC]temp flash sector trash : 0x%x) \r\n", DPC_ID);	 return;	
		if(saddr==DPD_FLASH_SECTOR2_ADDR)		UARTprintf("[SDPC]init bypass mode(trash data in result2 area : 0x%x) \r\n", DPC_ID);	 return;	
	}
	else{
		if(saddr==DPD_FLASH_SECTOR_ADDR)		UARTprintf("[SDPC]FLASH(result1) read \r\n");		
		if(saddr==DPD_FLASH_TEMP_SECTOR_ADDR)		UARTprintf("[SDPC]FLASH(TEMP_SECTOR) read \r\n");	
		if(saddr==DPD_FLASH_SECTOR2_ADDR)		UARTprintf("[SDPC]FLASH(result2) read \r\n");				
	}
	
	if((NHDPC_DATA.l>0)&&(NHDPC_DATA.l<NH_SIZ+1)){
		sf_read(FlashAddr[Xbuf], (PBYTE)NHDPC_DATA.x, sizeof(NHDPC_DATA.x));
		sf_read(FlashAddr[Ybuf], (PBYTE)NHDPC_DATA.y, sizeof(NHDPC_DATA.y));   
		sf_read(FlashAddr[Mbuf], (PBYTE)NHDPC_DATA.byr_dpc_mode, sizeof(NHDPC_DATA.byr_dpc_mode));   
	
		if(saddr==DPD_FLASH_SECTOR_ADDR)	SetSDPC_MemWrite_Order();	   
		else if(saddr==DPD_FLASH_SECTOR2_ADDR)	SetSDPC_MemWrite_Order();	   		
	}	

	
	if(stSDPCCTRL.init==0){
		if(NHDPC_DATA.l==0) 	UARTprintf("[SDPC]init bypass no dpc in flash\r\n");	
		else if(NHDPC_DATA.l>NH_SIZ+1)	UARTprintf("[SDPC]init bypass. langth over : %d  \r\n", NHDPC_DATA.l);				
		else		 			UARTprintf("[SDPC]init flash load mode. langth : %d  \r\n", NHDPC_DATA.l);			
	}
	else{
		if(NHDPC_DATA.l==0) 	UARTprintf("[SDPC]in flash, no dpc data \r\n");	
		else if(NHDPC_DATA.l>NH_SIZ+1)	UARTprintf("[SDPC]init bypass. langth over : %d  \r\n", NHDPC_DATA.l);			
		else					UARTprintf("[SDPC]flash load end. langth : %d \r\n",NHDPC_DATA.l);		
	}
}

static void dpd_flash_print(WORD saddr)
{
#if 0		//__MDIN_i5XX_FPGA__
	//data flash to cpu memory then print.

       WORD	DPC_ID;
       DWORD  FlashAddr[4];


       // set flash addr
       FlashAddr[Start] = saddr * 4096;           // buff1 start addr	1sector is 4K(4096) byte. save sensor status and errcounter
       FlashAddr[ID] = FlashAddr[Start] + 2;     // flash trash check id       
       FlashAddr[Xbuf] = FlashAddr[ID] + 2;     // buff2 start addr
       FlashAddr[Ybuf] = FlashAddr[Xbuf] +  NH_SIZ*2;       // buff2 end addr


	DPC_DataReset();

	sf_read(FlashAddr[Start], (PBYTE)&(NHDPC_DATA.l), 2);
	sf_read(FlashAddr[ID], (PBYTE)&(DPC_ID), 2);

	if(DPC_ID!=0x0500)
	{
		UARTprintf("[DPC]init bypass mode(flash trash : 0x%x) \r\n", DPC_ID);	 return;	
	}
	
	if((NHDPC_DATA.l>0)&&(NHDPC_DATA.l<NH_SIZ+1)){
	sf_read(FlashAddr[Xbuf], (PBYTE)NHDPC_DATA.x, sizeof(NHDPC_DATA.x));
	sf_read(FlashAddr[Ybuf], (PBYTE)NHDPC_DATA.y, sizeof(NHDPC_DATA.y));   
   
	}	

	
	if(stSDPCCTRL.init==0){
		if(NHDPC_DATA.l==0) 	UARTprintf("[DPC]init bypass no dpc in flash\r\n");	
		else if(NHDPC_DATA.l>NH_SIZ+1)	UARTprintf("[DPC]init bypass. langth over : %d  \r\n", NHDPC_DATA.l);				
		else		 			UARTprintf("[DPC]init flash load mode. langth : %d  \r\n", NHDPC_DATA.l);			
	}
	else{
		if(NHDPC_DATA.l==0) 	UARTprintf("[DPC]in flash, no dpc data \r\n");	
		else if(NHDPC_DATA.l>NH_SIZ+1)	UARTprintf("[DPC]init bypass. langth over : %d  \r\n", NHDPC_DATA.l);			
		else					UARTprintf("[DPC]flash load end. langth : %d \r\n",NHDPC_DATA.l);		
	}

	DPD_MemPrint();
#else
	WORD	FLASHDPC_length=0;				// final data length 
	WORD	FLASHDPC_DATA_x=0;			// static dead pixel x position(normal cond)	//h
	WORD	FLASHDPC_DATA_y=0;			// static dead pixel y position(normal cond)	//h
	BYTE	FLASHDPC_DATA_m=0;
	int 		i=0;

	WORD	DPC_ID;
	DWORD  FlashAddr[5];

	if(saddr<DPD_FLASH_SECTOR2_ADDR){
		UARTprintf("[SDPC]Wrong flash(%d) sector access", saddr);	 
		return;	
	}

	// set flash addr
	FlashAddr[Start] = saddr * 4096;           // buff1 start addr	1sector is 4K(4096) byte. save sensor status and errcounter
	FlashAddr[ID] = FlashAddr[Start] + 2;     // flash trash check id       
	FlashAddr[Xbuf] = FlashAddr[ID] + 2;     // buff2 start addr
	FlashAddr[Ybuf] = FlashAddr[Xbuf] + NH_SIZ*2;       // buff2 end addr
	FlashAddr[Mbuf] = FlashAddr[Ybuf] + NH_SIZ*2;       // buff3 end addr       	

	sf_read(FlashAddr[Start], (PBYTE)&(FLASHDPC_length), 2);
	sf_read(FlashAddr[ID], (PBYTE)&(DPC_ID), 2);

	if(DPC_ID!=0x0500)
	{
		UARTprintf("[SDPC]no data in flash(flash trash : 0x%x) \r\n", DPC_ID);	 
		return;	
	}
	
	if(FLASHDPC_length==0){
		UARTprintf("[SDPC]no data in flash length : 0x%x) \r\n", FLASHDPC_length);	 
		return;	
	}		

	UARTprintf("-----------------------------\r\n"); 	
	for(i=0 ; i<FLASHDPC_length; i++){
		sf_read(FlashAddr[Xbuf]+2*i, (PBYTE)&FLASHDPC_DATA_x, 2);
		sf_read(FlashAddr[Ybuf]+2*i, (PBYTE)&FLASHDPC_DATA_y, 2);  
//		sf_read(FlashAddr[Mbuf]+2*i, (PBYTE)&FLASHDPC_DATA_m, 2);  		
		sf_read(FlashAddr[Mbuf]+i, (PBYTE)&FLASHDPC_DATA_m, 1);  	
		UARTprintf("[SDPC]addr:%d   x:%d   y:%d   mode = %d \r\n", i, FLASHDPC_DATA_x, FLASHDPC_DATA_y, FLASHDPC_DATA_m); 	   
	}	
	UARTprintf("length of sdpc_data form flash : %d \r\n",  FLASHDPC_length); 	
	UARTprintf("-----------------------------\r\n"); 			
#endif
}

static void dpd_flash_update(WORD saddr)
{
	//dp detect result --> update to temp flash

	//read temp flash to cpu mem --> flash read --> sort&merge --> write 

	WORD	DPC_ID,FLASHDPC_length;
	WORD	FLASHDPC_DATA_x=0, FLASHDPC_DATA_y=0;
	BYTE	FLASHDPC_DATA_m=0;
	DWORD	FlashAddr[5];
	int		i=0;

	if(saddr<DPD_FLASH_SECTOR2_ADDR){
		UARTprintf("[SDPC]Wrong flash(%d) sector access", saddr);	 
		return;	
	}

       // set flash addr
       FlashAddr[Start] = saddr * 4096;           // buff1 start addr	1sector is 4K(4096) byte. save sensor status and errcounter
       FlashAddr[ID] = FlashAddr[Start] + 2;     // flash trash check id       
       FlashAddr[Xbuf] = FlashAddr[ID] + 2;     // buff2 start addr
       FlashAddr[Ybuf] = FlashAddr[Xbuf] + NH_SIZ*2;       // buff2 end addr
       FlashAddr[Mbuf] = FlashAddr[Ybuf] + NH_SIZ*2;       // buff3 end addr              

	sf_read(FlashAddr[Start], (PBYTE)&(FLASHDPC_length), 2);
	sf_read(FlashAddr[ID], (PBYTE)&(DPC_ID), 2);

	if(DPC_ID!=0x0500){
		UARTprintf("[SDPC]no data in flash(flash trash, ID : 0x%x) \r\n", DPC_ID);	 
		dpd_flash_write(saddr);
		return;	
	}

	if(FLASHDPC_length==0){
		UARTprintf("[SDPC]no data in flash length : 0x%x) \r\n", FLASHDPC_length);	 
		dpd_flash_write(saddr);
		return;	
	}	

	if(NHDPC_DATA.l>NH_SIZ+1)	{	
		UARTprintf("Please re-try. Environment may have changed during the this detection. Return to the data before saved in flash");	
		dpd_flash_read(saddr);
		return;		

	}

	if(saddr==DPD_FLASH_TEMP_SECTOR_ADDR) UARTprintf("[SDPC]update to tamp flash \r\n");	 	
	if(saddr==DPD_FLASH_SECTOR_ADDR) UARTprintf("[SDPC]update to (result1) flash \r\n");	 	
	if(saddr==DPD_FLASH_SECTOR2_ADDR) UARTprintf("[SDPC]update to (result2) flash \r\n");	 	

	//sort & rewrite to flash(FLASHDPC_DATA --> NHDPC_DATA-->FLASHDPC_DATA)
	for(i=0; i<FLASHDPC_length; i++){
		sf_read(FlashAddr[Xbuf]+2*i, (PBYTE)&FLASHDPC_DATA_x, 2);
		sf_read(FlashAddr[Ybuf]+2*i, (PBYTE)&FLASHDPC_DATA_y, 2);
		sf_read(FlashAddr[Mbuf]+i, (PBYTE)&FLASHDPC_DATA_m, 1);
		DPC_DataSorting(FLASHDPC_DATA_x ,FLASHDPC_DATA_y, FLASHDPC_DATA_m);
//		DPC_DataSorting(FLASHDPC_DATA_x ,FLASHDPC_DATA_y&0x0FFF, (FLASHDPC_DATA_y&0x1000)>>12);		
//		UARTprintf("[DPD]sorting insert addr:%d   x:%d   y:%d  \r\n", i, FLASHDPC_DATA_x, FLASHDPC_DATA_y); 	   
		
	}

	dpd_flash_write(saddr);	//Rewrite Flash

}
//-------------------------------------------------PROCESSING---------------------------------------------------
#if defined(__USE_DPD_i51X__)
static void SetSDPC_Search_para(void)
{
	//median mode
	if(stSDPCCTRL.search_type==white_median){			
		stSDPCCTRL.median_ineqsel=OFF;		//P1+P2+P4+P5 < 4P3 - THHD register   # THHD register is 5*thhd value
		stSDPCCTRL.upper_thhd=4095;			// not used
		stSDPCCTRL.lower_thhd=SDPD_THHD_LV2;		
	}
	//thhd mode	
	else if(stSDPCCTRL.search_type==white_thhd){
		stSDPCCTRL.median_ineqsel=OFF;	
		stSDPCCTRL.upper_thhd=4095;		//use 12bit thhd
		stSDPCCTRL.lower_thhd=SDPD_THHD_LV2;		
	}
}
#endif

static void Init_SDPCDATA(BOOL OnOff)		
{
//	int i;
	
	if(OnOff) return;	


#if defined(__USE_DPD_i51X__)
	stSDPCCTRL.mode=OFF;
	stSDPCCTRL.rp_nfs=10;	

	stSDPCCTRL.search_type=white_thhd;	
//	stSDPCCTRL.search_type=white_median;	
	
	SetSDPC_Search_para();

#if defined (__MDIN_i5XX_FPGA__)
	dpd_buff_page_use(2);
#else
	dpd_buff_page_use(3);
#endif
	stSDPCCTRL.agc_lv=1;		stSDPCCTRL.dpc_mode=0x01;

#else
	MISP_RegWrite(MISP_LOCAL_ID, 0x402, 0x0000);	//init dyanmic dp enable

	if(GetSS_stDESERIALIZER_ext_hactive()){
	MISP_RegWrite(MISP_LOCAL_ID, 0x24E, GetSS_stDESERIALIZER_ext_hactive());	
	MISP_RegWrite(MISP_LOCAL_ID, 0x24F, GetSS_sensor_hactive_read());	
	MISP_RegField(MISP_LOCAL_ID, 0x24F, 15, 1, 1);	
	}

	//set SDPC_CTRL_PARAM stSDPCCTRL;
	stSDPCCTRL.mode=OFF;
	stSDPCCTRL.rp_nfs=10;		stSDPCCTRL.rp_n=3;		//max30
	stSDPCCTRL.cut=2;
	stSDPCCTRL.hot_thhd=3;		stSDPCCTRL.dead_thhd=2;

	stSDPCCTRL.agc_lv=1;		stSDPCCTRL.dpc_mode=0x01;

	stSDPCCTRL.dpc_th_val0 = 100 ;
	stSDPCCTRL.dpc_th_val1 = 170 ;
	stSDPCCTRL.dpc_th_val2 = 170 ;

#endif
	stSDPCCTRL.debug_disp=0;
	SetStaticDPC_init_flag(1);

#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	stSDPCCTRL.ctrl |= (SDPC_NORMAL_COND);			//init : SetMenuSpecialInit in menu.c
#else
	stSDPCCTRL.ctrl = (SDPC_PROCESS_RUN|SDPC_NORMAL_COND);
	stSDPCCTRL.ctrl |= (SDPC_TRIGGER_ISP|SDPC_MEMORY_HOLD|SDPC_FLASH_HOLD);
#endif	

	//loadE2prom pixel pos data
	DPC_RaddrReset();
	DPC_MemoryReset(0,NH_SIZ);

	dpd_flash_clear(DPD_FLASH_TEMP_SECTOR_ADDR);		//read.. if data length == not 0

#if defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__) || defined(__USE_CMOS_IMX226__)
	if(( GetSS_SensorVoutFrmt() == MISP_VIDSRC_1920x1080p50 ) |\
		( GetSS_SensorVoutFrmt() == MISP_VIDSRC_1920x1080p60 ))
			dpd_flash_read(DPD_FLASH_SECTOR2_ADDR);		
	else		dpd_flash_read(DPD_FLASH_SECTOR_ADDR);
	
#else
	dpd_flash_read(DPD_FLASH_SECTOR_ADDR);
#endif



}

static void DPD_END(void)
{
	#if defined(__USE_DPD_i51X__)		
	MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 0, 2, 0);						//dpd rtl logic off		
	#else
	//this is dpd rtl reset
	MISP_RegWrite(MISP_LOCAL_ID, reg_sdpd_parameter, 0);							//dpd rtl logic off					
	#endif

	stSDPCCTRL.scnt=0;	
	stSDPCCTRL.mode=MISP_SDPD_OFF;	
	stSDPCCTRL.end_flag=1;
}

static void DPD_Process(void)
{	

	//decision update mode or pass
	switch(stSDPCCTRL.mode){
		case MISP_SDPD_OFF : 	break;			//status reset and return
		
		case MISP_SDPD_SETUP:	dpdpara_set();		break;
		case MISP_SDPD_CHK: 	dpd_check_para();	break;
		case MISP_SDPD_RST: 	DPC_DataReset();	
								DPD_LogicReset();
								break;
								
		case MISP_SDPD_READ:	DPD_DataRead(); 	break;
		case MISP_SDPD_T_FLASH:	dpd_flash_update(DPD_FLASH_TEMP_SECTOR_ADDR);			
								stSDPCCTRL.mode=MISP_SDPD_DECIDE;																
								break;

		
		case MISP_SDPD_DECIDE:	if(GetSDPD_AGC_Level()==SDPD_AGC_MAX){
									stSDPCCTRL.mode=MISP_SDPD_END;		
									stSDPCCTRL.ae_backflag=ON;
									UARTprintf("[SDPC]last detect end? Yes  \r\n");	
								}
								else	{
									stSDPCCTRL.mode=MISP_SDPD_END;										
									stSDPCCTRL.ae_backflag=OFF;
									UARTprintf("[SDPC]last detect not end. current level =%d  \r\n",stSDPCCTRL.agc_lv);										

									stSDPCCTRL.agc_lv++;	

								}
								break;		//parameter check... ok next check, no? end.
							
		case MISP_SDPD_END:		DPD_END();			break;		

		default : break;
	}	


	//------------------to find offset setting--------------------
//	MISP_RegField(MISP_LOCAL_ID, 0x4B3, 8, 4, 1);	//tp size 2x2
//	MISP_RegField(MISP_LOCAL_ID, 0x4B3, 12, 1, 1);	//dpd tp en
//	MISP_RegWrite(MISP_LOCAL_ID, 0x4B6, 500);	//dpd tp pos x
//	MISP_RegWrite(MISP_LOCAL_ID, 0x4B7, 500);	//dpd tp pos y
	//------------------------------------------------------	

}
static void SDPC_Process_Handler(void)
{	
#if !defined(__USE_4K60_MD2__)		//except 4k60p slave chip 
	
	#if defined(__USE_4K60_MD1__)
		WORD buff[5];
	#endif

	//Checking ISP DPC Memory Read, Write, Reset --> tracking agc

	if ((stSDPCCTRL.ctrl&SDPC_TRIGGER_ISP)==0) return;
	stSDPCCTRL.ctrl &= (~SDPC_TRIGGER_ISP);

//	UARTprintf("SDPC_Process_Handler RUN = %x \r\n", stSDPCCTRL.ctrl);			
	
	//control DPC Register Enable ctrl, run dynamic [1]=en, static [0]=en;
	if ((stSDPCCTRL.ctrl&SDPC_PROCESS_MASK)==SDPC_PROCESS_RUN) 		{	MISP_RegField(MISP_LOCAL_ID, 0x403, 0, 2, 3);	}
	else if ((stSDPCCTRL.ctrl&SDPC_PROCESS_MASK)==SDPC_PROCESS_OFF)		{	MISP_RegField(MISP_LOCAL_ID, 0x403, 0, 2, 0);	}
	else if ((stSDPCCTRL.ctrl&SDPC_PROCESS_MASK)==SDPC_PROCESS_STATIC)	{	MISP_RegField(MISP_LOCAL_ID, 0x403, 0, 2, 1);	}
	else if ((stSDPCCTRL.ctrl&SDPC_PROCESS_MASK)==SDPC_PROCESS_FIXED) 	{
		#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		MISP_RegField(MISP_LOCAL_ID, 0x403, 0, 2, 3);		
		MISP_RegField(MISP_LOCAL_ID, 0x403, 4, 3, 1>>2|stDDPC_PARAM.fixed_selmode);				// white_only, loose_mode = 2
		MISP_RegWrite(MISP_LOCAL_ID, 0x404, stDDPC_PARAM.fixed_threshold1<<8|stDDPC_PARAM.fixed_threshold2);	//threshold0, 1
		#endif
	}

	#if defined(__USE_4K60_MD1__)
		//read (0x403,404,405,406,40c) --> //slave write order to MD2
		MISP_RegRead(MISP_LOCAL_ID, 0x403, &buff[0]);
		MISP_RegRead(MISP_LOCAL_ID, 0x404, &buff[1]);
		MISP_RegRead(MISP_LOCAL_ID, 0x405, &buff[2]);
		MISP_RegRead(MISP_LOCAL_ID, 0x406, &buff[3]);
		MISP_RegRead(MISP_LOCAL_ID, 0x40c, &buff[4]);
		SPImAPP_SetSlaveDDPCReg(&buff[0]);
	#endif
	
#endif
}
static void SDPC_Memory_Handler(void)
{	
	//control DPC Memory Write Controll	
	if (stSDPCCTRL.ctrl&SDPC_MEMORY_MASK){

		if ((stSDPCCTRL.ctrl&SDPC_MEMORY_MASK)==SDPC_MEMORY_RESET) DPC_MemoryReset(0,NH_SIZ);	
		if ((stSDPCCTRL.ctrl&SDPC_MEMORY_MASK)==SDPC_MEMORY_READ) DPC_MemoryRead();	
		if ((stSDPCCTRL.ctrl&SDPC_MEMORY_MASK)==SDPC_MEMORY_WRITE) DPC_MemoryWrite();
		if ((stSDPCCTRL.ctrl&SDPC_MEMORY_MASK)==SDPC_MEMORY_RELOAD){
			#if defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__) || defined(__USE_CMOS_IMX226__)
				if(( GetSS_SensorVoutFrmt() == MISP_VIDSRC_1920x1080p50 ) |\
					( GetSS_SensorVoutFrmt() == MISP_VIDSRC_1920x1080p60 ))
					dpd_flash_read(DPD_FLASH_SECTOR2_ADDR);		
				else		dpd_flash_read(DPD_FLASH_SECTOR_ADDR);
			#else
				dpd_flash_read(DPD_FLASH_SECTOR_ADDR);
			#endif
		}
																		
		//UARTprintf("[DPC]Memory handler call \r\n");		
		stSDPCCTRL.ctrl = ((~SDPC_MEMORY_MASK&stSDPCCTRL.ctrl)|SDPC_MEMORY_HOLD);
	}
	
}
static void SDPC_FLASH_Handler(void)
{
#if defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__) || defined(__USE_CMOS_IMX226__)
	WORD saddr;

	if(( GetSS_SensorVoutFrmt() == MISP_VIDSRC_1920x1080p50 ) |\
		( GetSS_SensorVoutFrmt() == MISP_VIDSRC_1920x1080p60 ))
			saddr = DPD_FLASH_SECTOR2_ADDR;			
	else		saddr = DPD_FLASH_SECTOR_ADDR;		
#else
	WORD saddr = DPD_FLASH_SECTOR_ADDR;
#endif


	if (stSDPCCTRL.ctrl&SDPC_FLASH_MASK){
		if ((stSDPCCTRL.ctrl&SDPC_FLASH_MASK)==SDPC_FLASH_TEMP_READ) dpd_flash_read(DPD_FLASH_TEMP_SECTOR_ADDR);		
		if ((stSDPCCTRL.ctrl&SDPC_FLASH_MASK)==SDPC_FLASH_PRINT) dpd_flash_print(saddr);
		if ((stSDPCCTRL.ctrl&SDPC_FLASH_MASK)==SDPC_FLASH_UPDATE) dpd_flash_update(saddr);
		if ((stSDPCCTRL.ctrl&SDPC_FLASH_MASK)==SDPC_FLASH_CLEAR) {	dpd_flash_clear(saddr);	
																	dpd_flash_clear(DPD_FLASH_TEMP_SECTOR_ADDR);	}

		if ((stSDPCCTRL.ctrl&SDPC_FLASH_MASK)==SDPC_FLASH_READ) dpd_flash_read(saddr);		
		if ((stSDPCCTRL.ctrl&SDPC_FLASH_MASK)==SDPC_FLASH_WRITE) dpd_flash_write(saddr);

		//UARTprintf("[DPC]flash handler call \r\n");		
		stSDPCCTRL.ctrl = ((~SDPC_FLASH_MASK&stSDPCCTRL.ctrl)|SDPC_FLASH_HOLD);
	}

}

//-------------------------------------------ORDER------------------------------------------------------------
void SetSDPC_OnOff(BYTE nID)
{
	if(nID==1)
	{
		stSDPCCTRL.ctrl = ((~SDPC_PROCESS_MASK&stSDPCCTRL.ctrl)|SDPC_PROCESS_RUN) ;
		stSDPCCTRL.ctrl |= SDPC_TRIGGER_ISP;
		//UARTprintf("stSDPCCTRL.ctrl RUN = %x \r\n", stSDPCCTRL.ctrl);		
	}
	else if(nID==2)
	{
		stSDPCCTRL.ctrl = ((~SDPC_PROCESS_MASK&stSDPCCTRL.ctrl)|SDPC_PROCESS_STATIC) ;
		stSDPCCTRL.ctrl |= SDPC_TRIGGER_ISP;
		//UARTprintf("stSDPCCTRL.ctrl RUN = %x \r\n", stSDPCCTRL.ctrl);		
	}
	else if(nID==3)
	{
		stSDPCCTRL.ctrl = ((~SDPC_PROCESS_MASK&stSDPCCTRL.ctrl)|SDPC_PROCESS_FIXED) ;
		stSDPCCTRL.ctrl |= SDPC_TRIGGER_ISP;
		//UARTprintf("stSDPCCTRL.ctrl RUN = %x \r\n", stSDPCCTRL.ctrl);		
	}	
	else
	{
		stSDPCCTRL.ctrl = ((~SDPC_PROCESS_MASK&stSDPCCTRL.ctrl)|SDPC_PROCESS_OFF) ;
		stSDPCCTRL.ctrl |= SDPC_TRIGGER_ISP;
		//UARTprintf("stSDPCCTRL.ctrl OFF = %x \r\n", stSDPCCTRL.ctrl);				
	}
}

BYTE GetSDPC_OnOff(void)
{
	switch((stSDPCCTRL.ctrl & SDPC_PROCESS_MASK) & (~SDPC_TRIGGER_ISP))	{
		case  SDPC_PROCESS_RUN: 		return  1; 		// on(dynamic+static)
		case  SDPC_PROCESS_STATIC: 		return  2; 		// on(static)
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
		case  SDPC_PROCESS_FIXED: 		return  3; 		// on(low)
#endif
		case  SDPC_PROCESS_OFF: 		return  0; 		// off
		default: 						return  0;
	}
}

void SetSDPC_MemWrite_Order(void)
{
	stSDPCCTRL.ctrl = ((~SDPC_MEMORY_MASK&stSDPCCTRL.ctrl)|SDPC_MEMORY_WRITE);
	stSDPCCTRL.ctrl = ((~SDPC_STATE_MASK&stSDPCCTRL.ctrl)|SDPC_NORMAL_COND);

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveSDPCMemWriteOrder();
#endif
}
void SetSDPC_MemRead_Order(void)
{
	stSDPCCTRL.ctrl = ((~SDPC_MEMORY_MASK&stSDPCCTRL.ctrl)|SDPC_MEMORY_READ);

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveSDPCMemReadOrder();
#endif
}
void SetSDPC_MemReset_Order(void)
{
	stSDPCCTRL.ctrl = ((~SDPC_MEMORY_MASK&stSDPCCTRL.ctrl)|SDPC_MEMORY_RESET);

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveSDPCMemResetOrder();
#endif
}
void SetSDPC_MemReload_Order(void)
{
	stSDPCCTRL.ctrl = ((~SDPC_MEMORY_MASK&stSDPCCTRL.ctrl)|SDPC_MEMORY_RELOAD);

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveSDPCMemReloadOrder();
#endif
}
void SetSDPC_FlashPrint_Order(void)
{
	stSDPCCTRL.ctrl = ((~SDPC_FLASH_MASK&stSDPCCTRL.ctrl)|SDPC_FLASH_PRINT);	

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveSDPCFlashPrintOrder();
#endif
}
void SetSDPC_FlashUpdate_Order(void)
{
	stSDPCCTRL.ctrl = ((~SDPC_FLASH_MASK&stSDPCCTRL.ctrl)|SDPC_FLASH_UPDATE);	

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveSDPCFlashUpdateOrder();
#endif
}
void SetSDPC_FlashWrite_Order(void)
{
	stSDPCCTRL.ctrl = ((~SDPC_FLASH_MASK&stSDPCCTRL.ctrl)|SDPC_FLASH_WRITE);	

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveSDPCFlashWriteOrder();
#endif
}
void SetSDPC_FlashClear_Order(void)
{
	stSDPCCTRL.ctrl = ((~SDPC_FLASH_MASK&stSDPCCTRL.ctrl)|SDPC_FLASH_CLEAR);	

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveSDPCFlashClearOrder();
#endif
}
void SetSDPC_FlashRead_Order(void)
{
	stSDPCCTRL.ctrl = ((~SDPC_FLASH_MASK&stSDPCCTRL.ctrl)|SDPC_FLASH_READ);	

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveSDPCFlashReadOrder();
#endif
}
void SetSDPC_FlashTempRead_Order(void)
{
	stSDPCCTRL.ctrl = ((~SDPC_FLASH_MASK&stSDPCCTRL.ctrl)|SDPC_FLASH_TEMP_READ);	

#if defined(__USE_4K60_INOUT__) && defined(__USE_4K60_MD1__)						// 4k60 master only
	SPImAPP_SetSlaveSDPCFlashTempReadOrder();
#endif
}

//-------------------------------------------PARAMETER-------------------------------------------------------
void SetSDPD_thhd(BYTE nID)
{
#if defined(__USE_DPD_i51X__)

	switch(nID) {
		case 0:	stSDPCCTRL.lower_thhd=SDPD_THHD_LV0;	break;		
		case 1:	stSDPCCTRL.lower_thhd=SDPD_THHD_LV1;	break;
		case 2:	stSDPCCTRL.lower_thhd=SDPD_THHD_LV2;	break;
		case 3:	stSDPCCTRL.lower_thhd=SDPD_THHD_LV3;	break;
		case 4:	stSDPCCTRL.lower_thhd=SDPD_THHD_LV4;	break;
		case 5:	stSDPCCTRL.lower_thhd=SDPD_THHD_LV5;	break;		
		case 6:	stSDPCCTRL.lower_thhd=SDPD_THHD_LV6;	break;		
		case 7:	stSDPCCTRL.lower_thhd=SDPD_THHD_LV7;	break;				
		default:	stSDPCCTRL.lower_thhd=SDPD_THHD_LV0;	break;
	}

	#if (defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__) || defined(__USE_CMOS_IMX226__))
		if(( GetSS_SensorVoutFrmt() == MISP_VIDSRC_1920x1080p50 ) |\
			( GetSS_SensorVoutFrmt() == MISP_VIDSRC_1920x1080p60 ))
			stSDPCCTRL.lower_thhd = stSDPCCTRL.lower_thhd / 6;
	#endif

#else
	switch(nID) {
		case 0:	stSDPCCTRL.hot_thhd=SDPD_THHD_LV0;	break;
		case 1:	stSDPCCTRL.hot_thhd=SDPD_THHD_LV1;	break;
		case 2:	stSDPCCTRL.hot_thhd=SDPD_THHD_LV2;	break;
		case 3:	stSDPCCTRL.hot_thhd=SDPD_THHD_LV3;	break; 
		case 4:	stSDPCCTRL.hot_thhd=SDPD_THHD_LV4;	break; 
		case 5:	stSDPCCTRL.hot_thhd=SDPD_THHD_LV5;	break; 		
		case 6:	stSDPCCTRL.hot_thhd=SDPD_THHD_LV6;	break; 				
		case 7:	stSDPCCTRL.hot_thhd=SDPD_THHD_LV7;	break; 				
		default:	stSDPCCTRL.hot_thhd=SDPD_THHD_LV0;	break;
	}

#endif
}
void SetSDPD_AGC_Level(BYTE nID)
{
	switch(nID) {
		case 0:	stSDPCCTRL.agc_lv=0;	stSDPCCTRL.dpc_mode=0x00; 		break;
		case 1:	stSDPCCTRL.agc_lv=1;	stSDPCCTRL.dpc_mode=0x01; 		break;
		case 2:	stSDPCCTRL.agc_lv=2;	stSDPCCTRL.dpc_mode=0x02;		break;
		case 3:	stSDPCCTRL.agc_lv=3;	stSDPCCTRL.dpc_mode=0x04;		break;
		case 4:	stSDPCCTRL.agc_lv=4;	stSDPCCTRL.dpc_mode=0x08;		break;
		case 5:	stSDPCCTRL.agc_lv=5;	stSDPCCTRL.dpc_mode=0x10;		break;		
	}
	
}
BYTE GetSDPD_AGC_Level(void)
{
	return stSDPCCTRL.agc_lv;
}
void SetSDPD_Shutter_level(BYTE nID)
{
	switch(nID) {
		case 0:	stSDPCCTRL.shut_lv=0;	break;
		case 1:	stSDPCCTRL.shut_lv=1;	break;
		case 2:	stSDPCCTRL.shut_lv=2;	break;	
		default : stSDPCCTRL.shut_lv=0;	break;
	}
	
}
BYTE GetSDPD_Shutter_level(void)
{
	return stSDPCCTRL.shut_lv;
}
BOOL GetSDPD_AE_BackFlag(void)
{
	return stSDPCCTRL.ae_backflag;
}
void SetSDPD_AE_BackFlag(BOOL OnOff)
{
	stSDPCCTRL.ae_backflag = OnOff;
	
}
void SetDynamicDPC_Auto(BOOL OnOff)
{
	stSDPCCTRL.auto_ddpc = OnOff;
	
}
BOOL GetDynamicDPC_Auto(void)
{
	return stSDPCCTRL.auto_ddpc;
}
#if defined(__USE_DPD_i51X__)
void SetSDPC_Search_type(WORD nID)
{
	stSDPCCTRL.search_type=nID;
	SetSDPC_Search_para();
}

BYTE GetSDPC_Search_type(void)
{
	return stSDPCCTRL.search_type;
}
#endif
void SetDPC_DebugDisp(BYTE nID)
{
	stSDPCCTRL.debug_disp=nID;

}
BYTE GetDPC_DebugDisp(void)
{
	return stSDPCCTRL.debug_disp;

}
void SetDDPC_Threshold1(WORD pwr, WORD nID)
{
	if(pwr==8)	stDDPC_PARAM.fixed_threshold1=MIN(255,nID);
	else if(pwr==7)	stDDPC_PARAM.pwr7_threshold1=MIN(255,nID);
	else if(pwr==6)	stDDPC_PARAM.pwr6_threshold1=MIN(255,nID);	
	else if(pwr==5)	stDDPC_PARAM.pwr5_threshold1=MIN(255,nID);
	else if(pwr==4)	stDDPC_PARAM.pwr4_threshold1=MIN(255,nID);
	else if(pwr==3)	stDDPC_PARAM.pwr3_threshold1=MIN(255,nID);
	else if(pwr==2)	stDDPC_PARAM.pwr2_threshold1=MIN(255,nID);
	else if(pwr==1)	stDDPC_PARAM.pwr1_threshold1=MIN(255,nID);
	else				stDDPC_PARAM.pwr0_threshold1=MIN(255,nID);

	SetDDPC_WriteEEPROM();	
}
void SetDDPC_Threshold2(WORD pwr, WORD nID)
{
	if(pwr==8)	stDDPC_PARAM.fixed_threshold2=MIN(255,nID);
	else if(pwr==7)	stDDPC_PARAM.pwr7_threshold2=MIN(255,nID);
	else if(pwr==6)	stDDPC_PARAM.pwr6_threshold2=MIN(255,nID);	
	else if(pwr==5)	stDDPC_PARAM.pwr5_threshold2=MIN(255,nID);
	else if(pwr==4)	stDDPC_PARAM.pwr4_threshold2=MIN(255,nID);
	else if(pwr==3)	stDDPC_PARAM.pwr3_threshold2=MIN(255,nID);
	else if(pwr==2)	stDDPC_PARAM.pwr2_threshold2=MIN(255,nID);
	else if(pwr==1)	stDDPC_PARAM.pwr1_threshold2=MIN(255,nID);
	else				stDDPC_PARAM.pwr0_threshold2=MIN(255,nID);

	SetDDPC_WriteEEPROM();		
}
void SetDDPC_SelMode(WORD pwr, WORD nID)
{
	if(pwr==8)	stDDPC_PARAM.fixed_selmode=MIN(3,nID);
	else if(pwr==7)	stDDPC_PARAM.pwr7_selmode=MIN(3,nID);
	else if(pwr==6)	stDDPC_PARAM.pwr6_selmode=MIN(3,nID);	
	else if(pwr==5)	stDDPC_PARAM.pwr5_selmode=MIN(3,nID);
	else if(pwr==4)	stDDPC_PARAM.pwr4_selmode=MIN(3,nID);
	else if(pwr==3)	stDDPC_PARAM.pwr3_selmode=MIN(3,nID);
	else if(pwr==2)	stDDPC_PARAM.pwr2_selmode=MIN(3,nID);
	else if(pwr==1)	stDDPC_PARAM.pwr1_selmode=MIN(3,nID);
	else				stDDPC_PARAM.pwr0_selmode=MIN(3,nID);

	SetDDPC_WriteEEPROM();		
}
BYTE GetDDPC_Threshold1(WORD pwr)
{
	if(pwr==8)	return stDDPC_PARAM.fixed_threshold1;
	else if(pwr==7)	return stDDPC_PARAM.pwr7_threshold1;
	else if(pwr==6)	return stDDPC_PARAM.pwr6_threshold1;	
	else if(pwr==5)	return stDDPC_PARAM.pwr5_threshold1;
	else if(pwr==4)	return stDDPC_PARAM.pwr4_threshold1;
	else if(pwr==3)	return stDDPC_PARAM.pwr3_threshold1;
	else if(pwr==2)	return stDDPC_PARAM.pwr2_threshold1;
	else if(pwr==1)	return stDDPC_PARAM.pwr1_threshold1;
	else				return stDDPC_PARAM.pwr0_threshold1;

}
BYTE GetDDPC_Threshold2(WORD pwr)
{
	if(pwr==8)	return stDDPC_PARAM.fixed_threshold2;
	else if(pwr==7)	return stDDPC_PARAM.pwr7_threshold2;
	else if(pwr==6)	return stDDPC_PARAM.pwr6_threshold2;
	else if(pwr==5)	return stDDPC_PARAM.pwr5_threshold2;
	else if(pwr==4)	return stDDPC_PARAM.pwr4_threshold2;
	else if(pwr==3)	return stDDPC_PARAM.pwr3_threshold2;
	else if(pwr==2)	return stDDPC_PARAM.pwr2_threshold2;
	else if(pwr==1)	return stDDPC_PARAM.pwr1_threshold2;
	else				return stDDPC_PARAM.pwr0_threshold2;

}
BYTE GetDDPC_SelMode(WORD pwr)
{
	if(pwr==8)	return stDDPC_PARAM.fixed_selmode;
	else if(pwr==7)	return stDDPC_PARAM.pwr7_selmode;
	else if(pwr==6)	return stDDPC_PARAM.pwr6_selmode;
	else if(pwr==5)	return stDDPC_PARAM.pwr5_selmode;
	else if(pwr==4)	return stDDPC_PARAM.pwr4_selmode;
	else if(pwr==3)	return stDDPC_PARAM.pwr3_selmode;
	else if(pwr==2)	return stDDPC_PARAM.pwr2_selmode;
	else if(pwr==1)	return stDDPC_PARAM.pwr1_selmode;
	else				return stDDPC_PARAM.pwr0_selmode;

}
void SetDDPC_WriteEEPROM(void)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[28];

	buff[0] = EEPROM_SAVE_DONE;		//gain done
	buff[1] = stDDPC_PARAM.pwr0_threshold1;                      
	buff[2] = stDDPC_PARAM.pwr1_threshold1;
	buff[3] = stDDPC_PARAM.pwr2_threshold1;   
	buff[4] = stDDPC_PARAM.pwr3_threshold1;
	buff[5] = stDDPC_PARAM.pwr4_threshold1;     
	buff[6] = stDDPC_PARAM.pwr5_threshold1;	
	buff[7] = stDDPC_PARAM.pwr6_threshold1;     
	buff[8] = stDDPC_PARAM.pwr7_threshold1;	
	

	buff[9] = stDDPC_PARAM.pwr0_threshold2;           
	buff[10] = stDDPC_PARAM.pwr1_threshold2;
	buff[11] = stDDPC_PARAM.pwr2_threshold2;   
	buff[12] = stDDPC_PARAM.pwr3_threshold2;
	buff[13] = stDDPC_PARAM.pwr4_threshold2;   
	buff[14] = stDDPC_PARAM.pwr5_threshold2;
	buff[15] = stDDPC_PARAM.pwr6_threshold2;   
	buff[16] = stDDPC_PARAM.pwr7_threshold2;	

	buff[17] = stDDPC_PARAM.pwr0_selmode;   
	buff[18] = stDDPC_PARAM.pwr1_selmode;
	buff[19] = stDDPC_PARAM.pwr2_selmode;   
	buff[20] = stDDPC_PARAM.pwr3_selmode;	
	buff[21] = stDDPC_PARAM.pwr4_selmode;   
	buff[22] = stDDPC_PARAM.pwr5_selmode;	
	buff[23] = stDDPC_PARAM.pwr6_selmode;   
	buff[24] = stDDPC_PARAM.pwr7_selmode;		

	buff[25] = stDDPC_PARAM.fixed_threshold1;	
	buff[26] = stDDPC_PARAM.fixed_threshold2;   
	buff[27]= stDDPC_PARAM.fixed_selmode;		
	
	EEPROM_MultiWrite(EEPROM_DDPC_PARAM, (PBYTE)&buff[0], sizeof(buff));	
#endif
}
//----------------------------------------Call by Misp100.c-----------------------------------------------
static void Init_DynamicDPC_PARAM(PDDPC_PARAM pCTRL)
{
#if defined (__USE_EEPROM__) && defined (EEPROM_SIZE_OVER_16KBIT)
	BYTE buff[28];	

	EEPROM_MultiRead(EEPROM_DDPC_PARAM, (PBYTE)&buff[0], sizeof(buff));
	
	if(buff[0] == EEPROM_SAVE_DONE){  //if gain done
		pCTRL->pwr0_threshold1=buff[1];		pCTRL->pwr0_threshold2=buff[9];		pCTRL->pwr0_selmode=buff[17];
		pCTRL->pwr1_threshold1=buff[2];		pCTRL->pwr1_threshold2=buff[10];		pCTRL->pwr1_selmode=buff[18];
		pCTRL->pwr2_threshold1=buff[3];		pCTRL->pwr2_threshold2=buff[11];		pCTRL->pwr2_selmode=buff[19];
		pCTRL->pwr3_threshold1=buff[4];		pCTRL->pwr3_threshold2=buff[12];		pCTRL->pwr3_selmode=buff[20];
		pCTRL->pwr4_threshold1=buff[5];		pCTRL->pwr4_threshold2=buff[13];		pCTRL->pwr4_selmode=buff[21];
		pCTRL->pwr5_threshold1=buff[6];		pCTRL->pwr5_threshold2=buff[14];		pCTRL->pwr5_selmode=buff[22];	
		pCTRL->pwr6_threshold1=buff[7];		pCTRL->pwr6_threshold2=buff[15];		pCTRL->pwr6_selmode=buff[23];	
		pCTRL->pwr7_threshold1=buff[8];		pCTRL->pwr7_threshold2=buff[16];		pCTRL->pwr7_selmode=buff[24];			

		pCTRL->fixed_threshold1=buff[25];		pCTRL->fixed_threshold2=buff[26];		pCTRL->fixed_selmode=buff[27];			
	}
	else{
		#if defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__) || defined(__USE_CMOS_IMX226__)	
		pCTRL->pwr0_threshold1=100;		pCTRL->pwr0_threshold2=170;		pCTRL->pwr0_selmode=0;
//		pCTRL->pwr0_threshold1=15;		pCTRL->pwr0_threshold2=20;		pCTRL->pwr0_selmode=0;		//if pcb so hot
		pCTRL->pwr1_threshold1=15;		pCTRL->pwr1_threshold2=20;		pCTRL->pwr1_selmode=0;		
		pCTRL->pwr2_threshold1=15;		pCTRL->pwr2_threshold2=18;		pCTRL->pwr2_selmode=2;			
		#else
		pCTRL->pwr0_threshold1=100;		pCTRL->pwr0_threshold2=170;		pCTRL->pwr0_selmode=0;
		pCTRL->pwr1_threshold1=15;		pCTRL->pwr1_threshold2=35;		pCTRL->pwr1_selmode=2;		
		pCTRL->pwr2_threshold1=15;		pCTRL->pwr2_threshold2=25;		pCTRL->pwr2_selmode=2;		
		#endif

		pCTRL->pwr3_threshold1=15;		pCTRL->pwr3_threshold2=18;		pCTRL->pwr3_selmode=2;
		pCTRL->pwr4_threshold1=15;		pCTRL->pwr4_threshold2=18;		pCTRL->pwr4_selmode=2;
		pCTRL->pwr5_threshold1=15;		pCTRL->pwr5_threshold2=18;		pCTRL->pwr5_selmode=2;
		pCTRL->pwr6_threshold1=15;		pCTRL->pwr6_threshold2=18;		pCTRL->pwr6_selmode=2;
		pCTRL->pwr7_threshold1=15;		pCTRL->pwr7_threshold2=18;		pCTRL->pwr7_selmode=2;
		
		#if defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__) || defined(__USE_CMOS_IMX226__)
		pCTRL->fixed_threshold1=15;		pCTRL->fixed_threshold2=20;		pCTRL->fixed_selmode=2;	
		#else
		pCTRL->fixed_threshold1=15;		pCTRL->fixed_threshold2=60;		pCTRL->fixed_selmode=3;	
		#endif
	}
		
#else
	pCTRL->pwr0_threshold1=100;		pCTRL->pwr0_threshold2=170;		pCTRL->pwr0_selmode=0;
	pCTRL->pwr1_threshold1=15;		pCTRL->pwr1_threshold2=35;		pCTRL->pwr1_selmode=2;
	pCTRL->pwr2_threshold1=15;		pCTRL->pwr2_threshold2=25;		pCTRL->pwr2_selmode=2;
	pCTRL->pwr3_threshold1=15;		pCTRL->pwr3_threshold2=15;		pCTRL->pwr3_selmode=2;
	pCTRL->pwr4_threshold1=15;		pCTRL->pwr4_threshold2=13;		pCTRL->pwr4_selmode=2;	
	pCTRL->pwr5_threshold1=15;		pCTRL->pwr5_threshold2=10;		pCTRL->pwr5_selmode=2;
	pCTRL->pwr6_threshold1=15;		pCTRL->pwr6_threshold2=10;		pCTRL->pwr6_selmode=2;
	pCTRL->pwr7_threshold1=15;		pCTRL->pwr7_threshold2=10;		pCTRL->pwr7_selmode=2;


	#if defined(__USE_CMOS_IMX274__) || defined(__USE_CMOS_IMX326__) || defined(__USE_CMOS_IMX226__)
	pCTRL->fixed_threshold1=15;		pCTRL->fixed_threshold2=20;		pCTRL->fixed_selmode=2;	
	#else
	pCTRL->fixed_threshold1=15;		pCTRL->fixed_threshold2=60;		pCTRL->fixed_selmode=3;	
	#endif
#endif
}

void Init_DynamicDPC(void)
{
	Init_DynamicDPC_PARAM(&stDDPC_PARAM);
}
//----------------------------------------Call by Video.c -------------------------------------------------
void DPD_RUN(void)
{
	stSDPCCTRL.mode = MISP_SDPD_SETUP;
}

BOOL StaticDPC_Process(void)
{	
	Init_SDPCDATA(stSDPCCTRL.init);							//initial sequence and parameter setting static dpc

	DPD_Process();
	SDPC_Process_Handler();
	SDPC_FLASH_Handler();
	SDPC_Memory_Handler();
	
	//menu, agc return order ctrl
	if(stSDPCCTRL.end_flag){
		stSDPCCTRL.end_flag=0;	
		return 1;
	}
	else		return 0;
}

BOOL StaticDPD_Busy(void)
{
	if(stSDPCCTRL.mode==MISP_SDPD_READ) return ON;
	else if(stSDPCCTRL.mode==MISP_SDPD_DECIDE) return ON;
	else if(stSDPCCTRL.mode==MISP_SDPD_RST) return ON;	
	else return OFF;
}

void SetStaticDPC_init_flag(BOOL en){
	// 1  
	stSDPCCTRL.init=en;
}

void DynamicDPC_Process(BYTE pwr)
{	
#if defined(__USE_MDIN_i500__) 						// MDIN-i500

	if ((stSDPCCTRL.ctrl&SDPC_PROCESS_MASK)==SDPC_PROCESS_RUN) 	{	
		if(pwr>0){
			MISP_RegWrite(MISP_LOCAL_ID, 0x403, 3);			
			MISP_RegWrite(MISP_LOCAL_ID, 0x404, (stSDPCCTRL.dpc_th_val0<<8)|(stSDPCCTRL.dpc_th_val1));	
			MISP_RegWrite(MISP_LOCAL_ID, 0x405, (stSDPCCTRL.dpc_th_val2));	
		}
		else{
			MISP_RegWrite(MISP_LOCAL_ID, 0x403, 1);					
			MISP_RegWrite(MISP_LOCAL_ID, 0x404, (stSDPCCTRL.dpc_th_val0<<8)|(stSDPCCTRL.dpc_th_val1));	
			MISP_RegWrite(MISP_LOCAL_ID, 0x405, (stSDPCCTRL.dpc_th_val2));	
		}
	}
#else 												// MDIN-i51X (i510, i540, i550)
#if defined(__USE_4K60_MD2__)						// 4k60p slave chip 

#else												// normal or 4k60p master chip 
	//Dyanmic DPC OnOff setting at SDPC_Process_Handler , SetSDPC_OnOff
	BYTE m2_valid_cnt;
	BOOL m2_lvl_sel;
	BOOL m2_dark_lvl_sel;
	BYTE threshold3;

	#if defined(__USE_4K60_MD1__)
	WORD buff[5];
	#endif	

	//UARTprintf("stSDPCCTRL.ctrl   =  %x \r\n",(stSDPCCTRL.ctrl&SDPC_PROCESS_MASK));	
	//here setting dynamic dpc thhd handler
	if (((stSDPCCTRL.ctrl&SDPC_PROCESS_MASK) & (~SDPC_TRIGGER_ISP))==SDPC_PROCESS_RUN) 	{	
		if(pwr==7){
			MISP_RegField(MISP_LOCAL_ID, 0x403, 4, 3, 1>>2|stDDPC_PARAM.pwr7_selmode);				// white_only, loose_mode = 2
			MISP_RegWrite(MISP_LOCAL_ID, 0x404, stDDPC_PARAM.pwr7_threshold1<<8|stDDPC_PARAM.pwr7_threshold2);	//threshold0, 1

			m2_valid_cnt=3;		m2_lvl_sel=1;			m2_dark_lvl_sel=0;		threshold3=0;					
			MISP_RegWrite(MISP_LOCAL_ID, 0x405, m2_valid_cnt<<14|m2_lvl_sel<<13|m2_dark_lvl_sel<<12|threshold3);	
			
			MISP_RegField(MISP_LOCAL_ID, 0x406, 8, 8, 12);	// m2_offset
			MISP_RegField(MISP_LOCAL_ID, 0x40c, 8, 8, 20);	// m2_dark_offset
			//MISP_RegWrite(MISP_LOCAL_ID, 0x42f,  0x300);	
		}			
		else if(pwr==6){
			MISP_RegField(MISP_LOCAL_ID, 0x403, 4, 3, 1>>2|stDDPC_PARAM.pwr6_selmode);				// white_only, loose_mode = 2
			MISP_RegWrite(MISP_LOCAL_ID, 0x404, stDDPC_PARAM.pwr6_threshold1<<8|stDDPC_PARAM.pwr6_threshold2);	//threshold0, 1

			m2_valid_cnt=3;		m2_lvl_sel=1;			m2_dark_lvl_sel=0;		threshold3=0;					
			MISP_RegWrite(MISP_LOCAL_ID, 0x405, m2_valid_cnt<<14|m2_lvl_sel<<13|m2_dark_lvl_sel<<12|threshold3);	
			
			MISP_RegField(MISP_LOCAL_ID, 0x406, 8, 8, 12);	// m2_offset
			MISP_RegField(MISP_LOCAL_ID, 0x40c, 8, 8, 20);	// m2_dark_offset
			//MISP_RegWrite(MISP_LOCAL_ID, 0x42f,  0x300);	
		}		
		else if(pwr==5){
			MISP_RegField(MISP_LOCAL_ID, 0x403, 4, 3, 1>>2|stDDPC_PARAM.pwr5_selmode);				// white_only, loose_mode = 2
			MISP_RegWrite(MISP_LOCAL_ID, 0x404, stDDPC_PARAM.pwr5_threshold1<<8|stDDPC_PARAM.pwr5_threshold2);	//threshold0, 1

			m2_valid_cnt=3;		m2_lvl_sel=1;			m2_dark_lvl_sel=0;		threshold3=0;					
			MISP_RegWrite(MISP_LOCAL_ID, 0x405, m2_valid_cnt<<14|m2_lvl_sel<<13|m2_dark_lvl_sel<<12|threshold3);	
			
			MISP_RegField(MISP_LOCAL_ID, 0x406, 8, 8, 12);	// m2_offset
			MISP_RegField(MISP_LOCAL_ID, 0x40c, 8, 8, 20);	// m2_dark_offset
			//MISP_RegWrite(MISP_LOCAL_ID, 0x42f,  0x300);	
		}
		else if(pwr==4){
			MISP_RegField(MISP_LOCAL_ID, 0x403, 4, 3, 1>>2|stDDPC_PARAM.pwr4_selmode);				// white_only, loose_mode = 2
			MISP_RegWrite(MISP_LOCAL_ID, 0x404, stDDPC_PARAM.pwr4_threshold1<<8|stDDPC_PARAM.pwr4_threshold2);	//threshold0, 1
		
			m2_valid_cnt=3;		m2_lvl_sel=1;		m2_dark_lvl_sel=0;		threshold3=0;		
			MISP_RegWrite(MISP_LOCAL_ID, 0x405, m2_valid_cnt<<14|m2_lvl_sel<<13|m2_dark_lvl_sel<<12|threshold3);	
			
			MISP_RegField(MISP_LOCAL_ID, 0x406, 8, 8, 12);	// m2_offset
			MISP_RegField(MISP_LOCAL_ID, 0x40c, 8, 8, 25);	// m2_dark_offset
			//MISP_RegWrite(MISP_LOCAL_ID, 0x42f,  0x300);	
			}
		else if(pwr==3){
			MISP_RegField(MISP_LOCAL_ID, 0x403, 4, 3, 1>>2|stDDPC_PARAM.pwr3_selmode);				// white_only, loose_mode = 2
			MISP_RegWrite(MISP_LOCAL_ID, 0x404, stDDPC_PARAM.pwr3_threshold1<<8|stDDPC_PARAM.pwr3_threshold2);	//threshold0, 1
		
			m2_valid_cnt=2;		m2_lvl_sel=1;		m2_dark_lvl_sel=0;		threshold3=0;		
			MISP_RegWrite(MISP_LOCAL_ID, 0x405, m2_valid_cnt<<14|m2_lvl_sel<<13|m2_dark_lvl_sel<<12|threshold3);	
			
			MISP_RegField(MISP_LOCAL_ID, 0x406, 8, 8, 12);	// m2_offset
			MISP_RegField(MISP_LOCAL_ID, 0x40c, 8, 8, 30);	// m2_dark_offset
			//MISP_RegWrite(MISP_LOCAL_ID, 0x42f,  0x300);	
			}
		else if(pwr==2){
			MISP_RegField(MISP_LOCAL_ID, 0x403, 4, 3, 1>>2|stDDPC_PARAM.pwr2_selmode);				// white_only, loose_mode = 2
			MISP_RegWrite(MISP_LOCAL_ID, 0x404, stDDPC_PARAM.pwr2_threshold1<<8|stDDPC_PARAM.pwr2_threshold2);	//threshold0, 1
		
			m2_valid_cnt=2;		m2_lvl_sel=1;		m2_dark_lvl_sel=1;		threshold3=0;		
			MISP_RegWrite(MISP_LOCAL_ID, 0x405, m2_valid_cnt<<14|m2_lvl_sel<<13|m2_dark_lvl_sel<<12|threshold3);	
			
			MISP_RegField(MISP_LOCAL_ID, 0x406, 8, 8, 12);	// m2_offset
			MISP_RegField(MISP_LOCAL_ID, 0x40c, 8, 8, 35);	// m2_dark_offset
			//MISP_RegWrite(MISP_LOCAL_ID, 0x42f,  0x300);	
		}	
		else if(pwr==1){
			MISP_RegField(MISP_LOCAL_ID, 0x403, 4, 3, 1>>2|stDDPC_PARAM.pwr1_selmode);				// white_only, loose_mode = 2
			MISP_RegWrite(MISP_LOCAL_ID, 0x404, stDDPC_PARAM.pwr1_threshold1<<8|stDDPC_PARAM.pwr1_threshold2);	//threshold0, 1
			
			m2_valid_cnt=1;		m2_lvl_sel=1;		m2_dark_lvl_sel=1;		threshold3=0;		
			MISP_RegWrite(MISP_LOCAL_ID, 0x405, m2_valid_cnt<<14|m2_lvl_sel<<13|m2_dark_lvl_sel<<12|threshold3);	
			
			MISP_RegField(MISP_LOCAL_ID, 0x405, 13, 1, 1);	
			MISP_RegWrite(MISP_LOCAL_ID, 0x405, (15*256+35));	//threshold
			
			MISP_RegField(MISP_LOCAL_ID, 0x406, 8, 8, 12);	// m2_offset
			MISP_RegField(MISP_LOCAL_ID, 0x40c, 8, 8, 35);	// m2_dark_offset
			//MISP_RegWrite(MISP_LOCAL_ID, 0x42f,  0x300);	
		}
		else{
			MISP_RegField(MISP_LOCAL_ID, 0x403, 4, 3, 0>>2|stDDPC_PARAM.pwr0_selmode);				
		
			MISP_RegWrite(MISP_LOCAL_ID, 0x404, stDDPC_PARAM.pwr0_threshold1<<8|stDDPC_PARAM.pwr0_threshold2);	//threshold0, 1
			
			m2_valid_cnt=0;		m2_lvl_sel=0;		m2_dark_lvl_sel=0;		threshold3=230;		
			MISP_RegWrite(MISP_LOCAL_ID, 0x405, m2_valid_cnt<<14|m2_lvl_sel<<13|m2_dark_lvl_sel<<12|threshold3);		
			//MISP_RegWrite(MISP_LOCAL_ID, 0x42f, 0);				
		}

		//UARTprintf("dynamic dpc change pwr=%d \r\n",pwr);
	}	
	else if (((stSDPCCTRL.ctrl&SDPC_PROCESS_MASK)&(~SDPC_TRIGGER_ISP))==SDPC_PROCESS_FIXED) 	{	
		MISP_RegField(MISP_LOCAL_ID, 0x403, 4, 3, 1>>2|stDDPC_PARAM.fixed_selmode);				// white_only, loose_mode = 2
		MISP_RegWrite(MISP_LOCAL_ID, 0x404, stDDPC_PARAM.fixed_threshold1<<8|stDDPC_PARAM.fixed_threshold2);	//threshold0, 1
		//MISP_RegWrite(MISP_LOCAL_ID, 0x404, (15*256+15));	//threshold old..


		if(pwr>0){
			m2_valid_cnt=1;		m2_lvl_sel=1;		m2_dark_lvl_sel=1;		threshold3=0;		
			MISP_RegWrite(MISP_LOCAL_ID, 0x405, m2_valid_cnt<<14|m2_lvl_sel<<13|m2_dark_lvl_sel<<12|threshold3);	
			
			MISP_RegField(MISP_LOCAL_ID, 0x405, 13, 1, 1);	
			MISP_RegWrite(MISP_LOCAL_ID, 0x405, (15*256+35));	//threshold
			
			MISP_RegField(MISP_LOCAL_ID, 0x406, 8, 8, 12);	// m2_offset
			MISP_RegField(MISP_LOCAL_ID, 0x40c, 8, 8, 35);	// m2_dark_offset
			//MISP_RegWrite(MISP_LOCAL_ID, 0x42f,  0x300);			
		}
		else{
			m2_valid_cnt=0;		m2_lvl_sel=0;		m2_dark_lvl_sel=0;		threshold3=230;		
			MISP_RegWrite(MISP_LOCAL_ID, 0x405, m2_valid_cnt<<14|m2_lvl_sel<<13|m2_dark_lvl_sel<<12|threshold3);		
		}

	}
	else{		//Mode off and auto off

		//UARTprintf("dynamic dpc change off \r\n");		
		MISP_RegField(MISP_LOCAL_ID, 0x403, 4, 3, 0>>2|stDDPC_PARAM.pwr0_selmode);				
		
		MISP_RegWrite(MISP_LOCAL_ID, 0x404, stDDPC_PARAM.pwr0_threshold1<<8|stDDPC_PARAM.pwr0_threshold2);	//threshold0, 1
		m2_valid_cnt=0;		m2_lvl_sel=0;		m2_dark_lvl_sel=0;		threshold3=230;		
		MISP_RegWrite(MISP_LOCAL_ID, 0x405, m2_valid_cnt<<14|m2_lvl_sel<<13|m2_dark_lvl_sel<<12|threshold3);		
		//MISP_RegWrite(MISP_LOCAL_ID, 0x42f, 0);	
	}


	#if defined(__USE_4K60_MD1__)						// slave write to MD2
		//read (0x403,404,405,406,40c) --> //slave write order to MD2
		MISP_RegRead(MISP_LOCAL_ID, 0x403, &buff[0]);
		MISP_RegRead(MISP_LOCAL_ID, 0x404, &buff[1]);
		MISP_RegRead(MISP_LOCAL_ID, 0x405, &buff[2]);
		MISP_RegRead(MISP_LOCAL_ID, 0x406, &buff[3]);
		MISP_RegRead(MISP_LOCAL_ID, 0x40c, &buff[4]);
		SPImAPP_SetSlaveDDPCReg(&buff[0]);
	#endif

	
#endif
#endif
}
//---------------------------------------for debug-------------------------------------------------------
void dpd_pixel_val_print(void)
{
	WORD scope_img;
	MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 0, 2, 3);	

//	if(stSDPCCTRL.search_type==white_median) 	MISP_RegField(MISP_LOCAL_ID, 0x4B3, 2, 2, 1);			
	
	MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 8, 3, 1);			 	

	MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_scope_cnt, &scope_img); 	
	UARTprintf("[DPC Pixel Val] 0 delay : %d \r\n", scope_img);	
	MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 9, 2, 1);		

	MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_scope_cnt, &scope_img); 	
	UARTprintf("[DPC Pixel Val] 1V delay : %d \r\n", scope_img);	
	MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 9, 2, 2);		

	MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_scope_cnt, &scope_img); 	
	UARTprintf("[DPC Pixel Val] 2V delay : %d \r\n", scope_img);	
	MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 9, 2, 3);		

	MISP_RegRead(MISP_LOCAL_ID, reg_sdpd_scope_cnt, &scope_img); 	
	UARTprintf("[DPC Pixel Val] 3V delay : %d \r\n", scope_img);	

	MISP_RegField(MISP_LOCAL_ID, reg_sdpd_parameter, 8, 3, 0);	

}
#if defined(__USE_DPD_i51X__)
void dpd_buff_page_use(BYTE nID)
{
	stSDPCCTRL.buff_page_use	= nID;		//0 : 8EA, 3=32EA per a frame
 	UARTprintf("[dpd_buff_page_use] nID =%d \r\n", stSDPCCTRL.buff_page_use);	
	MISP_RegField(MISP_LOCAL_ID, reg_sdpd_buff_page, 0, 2, stSDPCCTRL.buff_page_use);		
}
#endif
//--------------------------------------------------------------------------------------------------------------------------
/*  FILE_END _HERE */
