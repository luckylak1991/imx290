// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"misp100.h"
#include	"osd.h"


#if (defined(__USE_CMOS_IMX226__)) || (defined(__USE_CMOS_IMX274__)) || (defined(__USE_CMOS_IMX334__))
	#if defined(__USE_SENS_PANNING_MODE__)
//		#define		__DIS_USE_ONLY_MAIN_EN__			// used main data only mode
//		#define		__DIS_USE_ONLY_AUX_EN__				// used aux data only mode
		#define		__DIS_USE_BOTH_EN__						// used main & aux data mode (default)
	#else 
		#define		__DIS_USE_ONLY_MAIN_EN__			// used main data only mode
//		#define		__DIS_USE_ONLY_AUX_EN__				// used aux data only mode
//		#define		__DIS_USE_BOTH_EN__						// used main & aux data mode (default)
	#endif
#else
	#define		__DIS_USE_ONLY_MAIN_EN__			// used main data only mode
//	#define		__DIS_USE_ONLY_AUX_EN__				// used aux data only mode
//	#define		__DIS_USE_BOTH_EN__					// used main & aux data mode (default)
#endif


#define 	NUM_VEC				64

#define		DIS_CAL_TIME		80		// 100	//bh 2013.12.05 dis (max 100: 15.4ms -> 21.7ms)

#define 	DIS_WAIT_DELAY		30



// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
WORD  DIS_enable;

WORD  DIS_bayer_vect, DIS_wait, pre_DIS_enable;//, DIS_black_screen;
WORD pre_NR_mode ;
int  DIS_Val_ref_x, DIS_Val_ref_y;
int  pre_DIS_Val_ref_x, pre_DIS_Val_ref_y;
BOOL  DIS_delay_en=1;
int  DIS_Vec_for_sum_x[NUM_VEC], DIS_Vec_for_sum_y[NUM_VEC];
BYTE DIS_vsync_flag;			// for one v sync fall, one DIS run.
BYTE DIS_wait_cnt;
WORD DIS_vector_array[4][2];
BYTE DIS_vector_flag_array[4];


BOOL DIS_OSD_en=0;

#if (defined(__USE_CMOS_IMX226__)) || (defined(__USE_CMOS_IMX274__)) || (defined(__USE_CMOS_IMX334__))
	#if defined(__USE_SENS_PANNING_MODE__)
		BYTE DIS_thres=25;
	#else
		BYTE DIS_thres=5;
	#endif
#else 
BYTE DIS_thres=25;
#endif



DIS_ROI stDISCoor;		// DIS coordinate x,y



//extern BOOL  MainSrc;   //bh 2014.09.12

//--------------------------------------------------------------------------------------------------


// vector write!!
void ISPM DIS_VectorWrite() 
{
	WORD DIS_MAIN_FID;
	BYTE start_read_fid ;


	if (DIS_enable==0) 
		return;
	
		
	// read FID
	MISP_RegRead(MISP_LOCAL_ID, 0x05C, &DIS_MAIN_FID);			// start read field ID
	start_read_fid = ((LOBYTE(DIS_MAIN_FID)&0xF0)>>4);


	if (DIS_wait_cnt >= DIS_WAIT_DELAY) {
		MISP_RegWrite(MISP_LOCAL_ID, 0x034, DIS_vector_array[start_read_fid][0]);
		MISP_RegWrite(MISP_LOCAL_ID, 0x035, DIS_vector_array[start_read_fid][1]);
		
//		UARTprintf("frame num, x, y : %d %d %d\n", start_read_fid, DIS_vector_array[start_read_fid][0], DIS_vector_array[start_read_fid][1]);
//		UARTprintf("x, y : %d %d\n", DIS_vector_array[start_read_fid][0], DIS_vector_array[start_read_fid][1]);
	}
}




void DIS_OSD_Display() 
{
	if (DIS_OSD_en==0)		// When DIS mode is changed, do only once. 
		return;
			

	if(DIS_enable==1) {
		if (DIS_wait_cnt >= DIS_WAIT_DELAY) {
			
			if ( (pre_DIS_enable != DIS_enable)||(DIS_delay_en==0) ) {
				OSD_IrkeyTitleDisplay(ON, "DIS ON    ");
				DIS_delay_en = 1;
				
//				SetOUT_AuxFreezeEN(OFF);
//				SetOUT_AuxDispEN(ON);
			}
			
		}else {
			if (DIS_delay_en)
				OSD_IrkeyTitleDisplay(ON, "DIS WAIT  ");
					
			DIS_delay_en = 0;
		}
		
	}else {
		if (pre_DIS_enable != DIS_enable)
			OSD_IrkeyTitleDisplay(OFF, "DIS OFF   ");
		
		DIS_delay_en = 1;
	}
	
	pre_DIS_enable = DIS_enable;
	
	
	DIS_OSD_en = 0;
}


void ISPM DIS_VsyncPulse() 
{
	DIS_vsync_flag = 1;			// for one v sync fall, one DIS run.
}



//--------------------------------------------------------------------------------------------------

void DISCommunication()
{
	//NEED TO KNOW MATRO BLOCK
	BYTE vec_num, vec_num_for_ext, aux_score_thres, aux_min_score_thres;
	WORD rVecNum, rAux_thres;
	WORD DIS_MAX_FRAME_NUM;
	BYTE max_frame_num;
	WORD DIS_EXT_Range, DIS_MAIN_FID;
	BYTE start_write_fid, end_write_fid;
	
#if (defined ( __DIS_USE_ONLY_AUX_EN__ ) || defined ( __DIS_USE_BOTH_EN__ ))
	WORD DIS_AUX_FID;
	BYTE start_write_fid_aux, end_write_fid_aux;
#endif

	BYTE manual_ref_fid, manual_tar_fid;
	BYTE move_x, move_y, valid_en;
	BYTE vector_x_aux, vector_y_aux;
	short stdVector_x_aux, stdVector_y_aux;
	WORD vector_aux_score=0;
	
	
	BYTE vector_x, vector_y;
	short stdVector_x, stdVector_y;
	WORD vector_score=0;
	BYTE ext_mode_en, ext_mode_en_for_next;

	WORD valid_cnt, delay_cnt=10;	//bh 2013.12.20 dis
	
	int aux_vector_x_for_sum[16], aux_vector_y_for_sum[16];
	BYTE aux_vector_cnt[16];
	
		
	BYTE dis_array[32][32], x, y;
	WORD score;
	BYTE score_thres, min_score_thres;
	int	 i,j;
	WORD DIS_Coordinate_X[5], DIS_Coordinate_Y[4];				//bh 2013.12.20 dis
	WORD one_flag1, one_flag2, one_flag3, one_flag4;
	BYTE addr_count;
	WORD DISCalDone;
	WORD rVal[6][44];
	WORD flag_temp[6][4];
	BOOL vector_flag[6][60];
	BYTE flag_count[6];
	
	BYTE x_range, y_range;
	
	BYTE vector_aux_only_en=0;
	BOOL same_vector_en;
	
	int  DIS_Vec_global_x, DIS_Vec_global_y;
	int  DIS_Vec_sum_x, DIS_Vec_sum_y;
	
	float alpha_vector_x, alpha_vector_y;


	
	if (DIS_enable != 1) {
		DIS_wait = 0;
		DIS_wait_cnt = 0;
		return;
	}
	
	
	if (DIS_vsync_flag != 1)		// for one v sync fall, one DIS run.
		return;

	
	
	
	//dis waiting counter
	if(DIS_wait<500) {		//bh 2015.02.12 for dis wait halt..
		DIS_wait++;
		
		DIS_Val_ref_x=stDISCoor.DIS_OFFSET_X;			// DIS vector init.
		DIS_Val_ref_y=stDISCoor.DIS_OFFSET_Y;
	
		pre_DIS_Val_ref_x = DIS_Val_ref_x;	// init..
		pre_DIS_Val_ref_y = DIS_Val_ref_y;

		memset(DIS_Vec_for_sum_x, 0, sizeof(DIS_Vec_for_sum_x));
		memset(DIS_Vec_for_sum_y, 0, sizeof(DIS_Vec_for_sum_y));
		
		for(i=0;i<4;i++)	{
			DIS_vector_array[i][0] = DIS_Val_ref_x;
			DIS_vector_array[i][1] = DIS_Val_ref_y;
		}
	
//		SetOUT_AuxFreezeEN(ON);
//		SetOUT_AuxDispEN(OFF);
		
//		DIS_vsync_flag = 0;		//bh 2015.02.12 for dis wait halt..// for one v sync fall, one DIS run.
			
//		Delay_mSec(10);
		
		return;
	}
	
	
	DIS_vsync_flag = 0;		//bh 2015.02.12 for dis wait halt..// for one v sync fall, one DIS run.
		
			
	
	//write dis ucom code here. you shoud think ucom to fpga communication(ex:spi) time	

	// extend mode(x2) OR normal mode(x1)
	MISP_RegRead(MISP_LOCAL_ID, 0x04C7, &DIS_EXT_Range);
	if (DIS_EXT_Range==0x0010)
		ext_mode_en = 1;
	else ext_mode_en = 0;
	
	
	MISP_RegRead(MISP_LOCAL_ID, 0x04C4, &DIS_MAX_FRAME_NUM);			// start read field ID
	max_frame_num = ((LOBYTE(DIS_MAX_FRAME_NUM)&0xF0)>>4);

#if (defined ( __DIS_USE_ONLY_AUX_EN__ ) || defined ( __DIS_USE_BOTH_EN__ ))
	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// reset value..
	memset(rVal, 0, sizeof(rVal));
	
	
	// DIS AUX First !!! Up Left
	MISP_RegWrite(MISP_LOCAL_ID, 0x04C8, 0x5010);	//dis centering control [14], 8bit mode [15]. AUX data is always 8bit !!!
	
	SetDDRMAP_DIS_mapping(GetDDRMAP_AUX_mapping());		// DIS access AUX data !!!
		
		
	// DIS AUX is not extend mode
	MISP_RegWrite(MISP_LOCAL_ID, 0x04C7, 0x0000);	// not ext mode set !!
	MISP_RegWrite(MISP_LOCAL_ID, 0x04C1, 0x1010);	// SA offset 16
	
	
	// for MB,SA position
	x_range = stDISCoor.aux_x_interval;
	y_range = stDISCoor.aux_y_interval;
	for(i=0;i<5;i++)	DIS_Coordinate_X[i] = stDISCoor.DIS_AUX_Coordinate_XL + (x_range*i);
	for(i=0;i<4;i++)	DIS_Coordinate_Y[i] = stDISCoor.DIS_AUX_Coordinate_YU + (y_range*i);
	

	MISP_MultiWrite(MISP_LOCAL_ID, 0x04CC, (PBYTE)DIS_Coordinate_X, 5*2);
	MISP_MultiWrite(MISP_LOCAL_ID, 0x04D1, (PBYTE)DIS_Coordinate_Y, 4*2);
	
	
	MISP_RegRead(MISP_LOCAL_ID, 0x02FF, &DIS_AUX_FID);			// start read field ID
//		start_read_fid_aux = ((LOBYTE(DIS_AUX_FID)&0xF0)>>4);
	start_write_fid_aux = (LOBYTE(DIS_AUX_FID)&0x0F);
	
	
	// for FID..
	if (max_frame_num==3) {				// 4 frame mode
		if (start_write_fid_aux==3) {
			manual_ref_fid = 1;
			manual_tar_fid = 2;
		}else if (start_write_fid_aux==2) {
			manual_ref_fid = 0;
			manual_tar_fid = 1;
		}else if (start_write_fid_aux==1) {
			manual_ref_fid = 3;
			manual_tar_fid = 0;
		}else if (start_write_fid_aux==0) {
			manual_ref_fid = 2;
			manual_tar_fid = 3;
		}else {
			UARTprintf("Wrong AUX FID !! \n");
			return;
		}
	}else if (max_frame_num==2) {		// 3 frame mode
		if (start_write_fid_aux==2) {
			manual_ref_fid = 0;
			manual_tar_fid = 1;
		}else if (start_write_fid_aux==1) {
			manual_ref_fid = 2;
			manual_tar_fid = 0;
		}else if (start_write_fid_aux==0) {
			manual_ref_fid = 1;
			manual_tar_fid = 2;
		}else {
			UARTprintf("Wrong AUX FID !! \n");
			return;
		}
	}else {
			UARTprintf("Wrong MAX FRAME !! \n");
			return;
	}
	
	DIS_AUX_FID = 0x8000 + (manual_ref_fid<<4) + manual_tar_fid;		// manual FID mode
	MISP_RegWrite(MISP_LOCAL_ID, 0x0505, DIS_AUX_FID);
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x7010);	//[14] - DIS START by CPU

	
	valid_en = 0;	valid_cnt = 0;
	while(valid_en == 0)	//check valid.(maybe 15 loop)
	{
		Delay_uSec(delay_cnt);									//bh 2013.12.20 dis

		MISP_RegRead(MISP_LOCAL_ID, 0x0500, &DISCalDone);
		if((DISCalDone & 0x8000) != 0)	valid_en = 1;

		if(valid_cnt++ > DIS_CAL_TIME)	{UARTprintf("AUX [1]");	break;}	//bh 2013.12.20 dis
	}
	// 60 vectors load..
	MISP_MultiRead(MISP_LOCAL_ID, 0x04D5, (PBYTE)rVal[0], 88);	//read vector(60 set) + flag(60 set)
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x3010);		//[14] - DIS END by CPU
		

	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DIS AUX Second !!! Up Right
	// for MB,SA position
	for(i=0;i<5;i++)	DIS_Coordinate_X[i] = stDISCoor.DIS_AUX_Coordinate_XR + (x_range*i);
	
	MISP_MultiWrite(MISP_LOCAL_ID, 0x04CC, (PBYTE)DIS_Coordinate_X, 5*2);
//		MISP_MultiWrite(MISP_LOCAL_ID, 0x04D1, (PBYTE)DIS_Coordinate_Y, 4*2);
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x7010);	//[14] - DIS START by CPU			

	valid_en = 0;	valid_cnt = 0;	//move_x = 0;
	while(valid_en == 0)	//check valid.(maybe 15 loop)
	{
		Delay_uSec(delay_cnt);									//bh 2013.12.20 dis

		MISP_RegRead(MISP_LOCAL_ID, 0x0500, &DISCalDone);
		if((DISCalDone & 0x8000) != 0)	valid_en = 1;

		if(valid_cnt++ > DIS_CAL_TIME)	{UARTprintf("AUX [1]");	break;}	//bh 2013.12.20 dis
	}
	
	// 60 vectors load..
	MISP_MultiRead(MISP_LOCAL_ID, 0x04D5, (PBYTE)rVal[1], 88);	//read vector(60 set) + flag(60 set)
		
	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x3010);	//[14] - DIS END by CPU

	
	
	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DIS AUX Third !!! Down Right
	// for MB,SA position
	for(i=0;i<4;i++)	DIS_Coordinate_Y[i] = stDISCoor.DIS_AUX_Coordinate_YD + (y_range*i);

//		MISP_MultiWrite(MISP_LOCAL_ID, 0x04CC, (PBYTE)DIS_Coordinate_X, 5*2);
	MISP_MultiWrite(MISP_LOCAL_ID, 0x04D1, (PBYTE)DIS_Coordinate_Y, 4*2);
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x7010);	//[14] - DIS START by CPU			

	valid_en = 0;	valid_cnt = 0;	//move_x = 0;
	while(valid_en == 0)	//check valid.(maybe 15 loop)
	{
		Delay_uSec(delay_cnt);									//bh 2013.12.20 dis

		MISP_RegRead(MISP_LOCAL_ID, 0x0500, &DISCalDone);
		if((DISCalDone & 0x8000) != 0)	valid_en = 1;

		if(valid_cnt++ > DIS_CAL_TIME)	{UARTprintf("AUX [3]");	break;}	//bh 2013.12.20 dis
	}

	// 60 vectors load..
	MISP_MultiRead(MISP_LOCAL_ID, 0x04D5, (PBYTE)rVal[2], 88);	//read vector(60 set) + flag(60 set)
		
	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x3010);	//[14] - DIS END by CPU
	
	
	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DIS AUX Fourth !!! Down Left
	// for MB,SA position
	for(i=0;i<5;i++)	DIS_Coordinate_X[i] = stDISCoor.DIS_AUX_Coordinate_XL + (x_range*i);

//		for(i=0;i<4;i++)	DIS_Coordinate_Y[i] = DIS_AUX_Coordinate_YD + (y_range*i);
	MISP_MultiWrite(MISP_LOCAL_ID, 0x04CC, (PBYTE)DIS_Coordinate_X, 5*2);
//		MISP_MultiWrite(MISP_LOCAL_ID, 0x04D1, (PBYTE)DIS_Coordinate_Y, 4*2);
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x7010);	//[14] - DIS START by CPU			

	valid_en = 0;	valid_cnt = 0;	//move_x = 0;
	while(valid_en == 0)	//check valid.(maybe 15 loop)
	{
		Delay_uSec(delay_cnt);									//bh 2013.12.20 dis

		MISP_RegRead(MISP_LOCAL_ID, 0x0500, &DISCalDone);
		if((DISCalDone & 0x8000) != 0)	valid_en = 1;

		if(valid_cnt++ > DIS_CAL_TIME)	{UARTprintf("AUX [4]");	break;}	//bh 2013.12.20 dis
	}

	// 60 vectors load..
	MISP_MultiRead(MISP_LOCAL_ID, 0x04D5, (PBYTE)rVal[3], 88);	//read vector(60 set) + flag(60 set)
		
	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x3010);	//[14] - DIS END by CPU

	for(j=0;j<4;j++) {
		flag_temp[j][0] = rVal[j][40];
		flag_temp[j][1] = rVal[j][41];
		flag_temp[j][2] = rVal[j][42];
		flag_temp[j][3] = rVal[j][43];
	}
	for(i=0;i<16;i++) {
		for(j=0;j<4;j++) {
			one_flag1 = (flag_temp[j][0]>>i) & 0x0001;
			one_flag2 = (flag_temp[j][1]>>i) & 0x0001;
			one_flag3 = (flag_temp[j][2]>>i) & 0x0001;
			one_flag4 = (flag_temp[j][3]>>i) & 0x0001;
			vector_flag[j][i] = one_flag1;
			vector_flag[j][i+16] = one_flag2;
			vector_flag[j][i+32] = one_flag3;
			vector_flag[j][i+48] = one_flag4;
		}
	}		
	
	memset(dis_array, 0, sizeof(dis_array));
	addr_count = 0;
	
	//make vector table(32x32)
	for(i=0;i<40;i+=2)
	{
		for(j=0;j<4;j++) {
			x = rVal[j][i]>>10;			y = (rVal[j][i]>>5) & 0x1f;
			if (vector_flag[j][addr_count]==1) {
				dis_array[y][x]++;
			}
		}
		addr_count++;
		
		for(j=0;j<4;j++) {
			x = rVal[j][i] & 0x1f;		y = (rVal[j][i+1]>>10) & 0x1f;
			if (vector_flag[j][addr_count]==1) {
				dis_array[y][x]++;
			}
		}
		addr_count++;
		
		for(j=0;j<4;j++) {
			x = (rVal[j][i+1]>>5) & 0x1f;	y = rVal[j][i+1] & 0x1f;
			if (vector_flag[j][addr_count]==1) {
				dis_array[y][x]++;
			}
		}
		addr_count++;
	}
	
		
		
		
		
	//find max vector score(3x3 sum)
	vector_x_aux=0;	vector_y_aux=0;
	vector_aux_score=0;
//		for(i=0;i<32;i++)	{			//y
//			for(j=0;j<32;j++)	{		//x
	for(i=1;i<31;i++)	{			//y
		for(j=1;j<31;j++)	{		//x
			score = (dis_array[i-1][j-1] + dis_array[i-1][j] + dis_array[i-1][j+1]) +
					(dis_array[i][j-1] + dis_array[i][j]*2 + dis_array[i][j+1]) +
					(dis_array[i+1][j-1] + dis_array[i+1][j] + dis_array[i+1][j+1]);

			if(score > vector_aux_score)	{
				vector_aux_score = score;
				vector_x_aux=j;
				vector_y_aux=i;
			}
		}
	}
	
	
	MISP_RegRead(MISP_LOCAL_ID, 0x02FF, &DIS_AUX_FID);			// start read field ID
//		start_read_fid_aux = ((LOBYTE(DIS_AUX_FID)&0xF0)>>4);
	end_write_fid_aux = (LOBYTE(DIS_AUX_FID)&0x0F);

//		UARTprintf("cur aux write FID !! %d \n", end_write_fid_aux);
	
	if (start_write_fid_aux != end_write_fid_aux)
		UARTprintf("Wrong AUX-FID !! \n\n\n");
	
#else

	// not use aux data..
//		vector_aux_score = 0;
//		vector_x_aux=16;
//		vector_y_aux=16;
	
#endif
	
		
	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// re-write for init. (because AUX !!)
	
	if (ext_mode_en==1)	{
		MISP_RegWrite(MISP_LOCAL_ID, 0x04C7, 0x0010);		// extend mode
		MISP_RegWrite(MISP_LOCAL_ID, 0x04C1, 0x2020);		// SA offset. 32.
	}else {
		MISP_RegWrite(MISP_LOCAL_ID, 0x04C7, 0x0000);		// NOT extend mode
		MISP_RegWrite(MISP_LOCAL_ID, 0x04C1, 0x1010);		// SA offset. 16.
	}

	// MISP_RegWrite(MISP_LOCAL_ID, 0x01C9, 0x0008);	// DIS access MAIN data !!!
	SetDDRMAP_DIS_mapping(GetDDRMAP_IPC_mapping());

//#if defined ( __USE_10BIT_OPERATION__ )
	if (GetSS_sensor_bit_opertion()==MISP_10BIT_OPERATION)
		MISP_RegWrite(MISP_LOCAL_ID, 0x04C8, 0xD010);	//dis centering control [14], 10bit mode [15]
//#endif
		
	// read FID (start)
	MISP_RegRead(MISP_LOCAL_ID, 0x05C, &DIS_MAIN_FID);			// start read field ID
//		start_read_fid = ((LOBYTE(DIS_MAIN_FID)&0xF0)>>4);
	start_write_fid = (LOBYTE(DIS_MAIN_FID)&0x0F);
		
		
		
		
#if (defined ( __DIS_USE_ONLY_MAIN_EN__ ) || defined ( __DIS_USE_BOTH_EN__ ))
		
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DIS FIRST !!! up-side !!! center..
	// for MB,SA position
	DIS_Val_ref_x=stDISCoor.DIS_OFFSET_X;
	DIS_Val_ref_y=stDISCoor.DIS_OFFSET_Y;
	x_range = stDISCoor.x_interval;
	for(i=0;i<5;i++)	DIS_Coordinate_X[i] = stDISCoor.DIS_Coordinate_XC + (x_range*i);	//bh 2013.12.20 dis
		
	y_range = stDISCoor.y_interval;
	for(i=0;i<4;i++)	DIS_Coordinate_Y[i] = stDISCoor.DIS_Coordinate_YU + (y_range*i);


	MISP_MultiWrite(MISP_LOCAL_ID, 0x04CC, (PBYTE)DIS_Coordinate_X, 5*2);
	MISP_MultiWrite(MISP_LOCAL_ID, 0x04D1, (PBYTE)DIS_Coordinate_Y, 4*2);
	
	
	// for FID..
	if (max_frame_num==3) {			// 4 frame mode
		if (start_write_fid==3) {
			manual_ref_fid = 1;
			manual_tar_fid = 2;
		}else if (start_write_fid==2) {
			manual_ref_fid = 0;
			manual_tar_fid = 1;
		}else if (start_write_fid==1) {
			manual_ref_fid = 3;
			manual_tar_fid = 0;
		}else if (start_write_fid==0) {
			manual_ref_fid = 2;
			manual_tar_fid = 3;
		}else {
			UARTprintf("Wrong FID !! \n");
			return;
		}
	}else if (max_frame_num==2) {	// 3 frame mode
		if (start_write_fid==2) {
			manual_ref_fid = 0;
			manual_tar_fid = 1;
		}else if (start_write_fid==1) {
			manual_ref_fid = 2;
			manual_tar_fid = 0;
		}else if (start_write_fid==0) {
			manual_ref_fid = 1;
			manual_tar_fid = 2;
		}else {
			UARTprintf("Wrong FID !! \n");
			return;
		}
	}else {
			UARTprintf("Wrong MAX FRAME !! \n");
			return;
	}

	DIS_MAIN_FID = 0x8000 + (manual_ref_fid<<4) + manual_tar_fid;	// manual FID mode
	MISP_RegWrite(MISP_LOCAL_ID, 0x0505, DIS_MAIN_FID);
	

	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x7010);	//[14] - DIS START by CPU


	
	valid_en = 0;	valid_cnt = 0;
	while(valid_en == 0)	//check valid.(maybe 15 loop)
	{
		Delay_uSec(delay_cnt);									//bh 2013.12.20 dis

		MISP_RegRead(MISP_LOCAL_ID, 0x500, &DISCalDone);
		if((DISCalDone & 0x8000) != 0)	valid_en = 1;

		if(valid_cnt++ > DIS_CAL_TIME)	{UARTprintf("[1]");	break;}	//bh 2013.12.20 dis
	}

	MISP_MultiRead(MISP_LOCAL_ID, 0x4D5, (PBYTE)rVal[0], 88);	//read vector(60 set) + flag(60 set)
		

	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x3010);	//[14] - DIS END by CPU
		
		
	


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DIS SECOND START !!!	left
	// for MB,SA position
	for(i=0;i<5;i++)	DIS_Coordinate_X[i] = stDISCoor.DIS_Coordinate_XL + (x_range*i);	//bh 2013.12.20 dis

//		for(i=0;i<4;i++)	DIS_Coordinate_Y[i] = DIS_Coordinate_YU + (y_range*i);

	MISP_MultiWrite(MISP_LOCAL_ID, 0x04CC, (PBYTE)DIS_Coordinate_X, 5*2);
//		MISP_MultiWrite(MISP_LOCAL_ID, 0x04D1, (PBYTE)DIS_Coordinate_Y, 4*2);

	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x7010);	//[14] - DIS START by CPU

	
	//make vector table(32x32)
//		memset(dis_array, 0, sizeof(dis_array));

	valid_en = 0;	valid_cnt=0;
	while(valid_en == 0)	//check valid.
	{
		Delay_uSec(delay_cnt);									//bh 2013.12.20 dis

		MISP_RegRead(MISP_LOCAL_ID, 0x500, &DISCalDone);
		if((DISCalDone & 0x8000) != 0)	valid_en = 1;

		if(valid_cnt++ > DIS_CAL_TIME)	{UARTprintf("[2]");	break;}	//bh 2013.12.20 dis
	}
//		UARTprintf("2 count : %d\n", valid_cnt);

	MISP_MultiRead(MISP_LOCAL_ID, 0x4D5, (PBYTE)rVal[1], 88);	//read vector(60 set) + flag(60 set)


	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x3010);	//[14] - DIS END by CPU


	
	
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DIS THIRD START !!!	  right
	// for MB,SA position
	for(i=0;i<5;i++)	DIS_Coordinate_X[i] = stDISCoor.DIS_Coordinate_XR + (x_range*i);	//bh 2013.12.20 dis

//		for(i=0;i<4;i++)	DIS_Coordinate_Y[i] = DIS_Coordinate_YU + (y_range*i);

	MISP_MultiWrite(MISP_LOCAL_ID, 0x04CC, (PBYTE)DIS_Coordinate_X, 5*2);
//		MISP_MultiWrite(MISP_LOCAL_ID, 0x04D1, (PBYTE)DIS_Coordinate_Y, 4*2);
		
	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x7010);	//[14] - DIS START by CPU
	
	//make vector table(32x32)
//		memset(dis_array, 0, sizeof(dis_array));
	
	valid_en = 0;	valid_cnt=0;
	while(valid_en == 0)	//check valid.
	{
		Delay_uSec(delay_cnt);									//bh 2013.12.20 dis

		MISP_RegRead(MISP_LOCAL_ID, 0x500, &DISCalDone);
		if((DISCalDone & 0x8000) != 0)	valid_en = 1;

		if(valid_cnt++ > DIS_CAL_TIME)	{UARTprintf("[3]");	break;}	//bh 2013.12.20 dis
	}
//		UARTprintf("3 count : %d\n", valid_cnt);

	MISP_MultiRead(MISP_LOCAL_ID, 0x4D5, (PBYTE)rVal[2], 88);	//read vector(60 set) + flag(60 set)
		
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x3010);	//[14] - DIS END by CPU

		
		
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DIS FOURTH !!!  down side!!!	 middle
	// for MB,SA position
	for(i=0;i<5;i++)	DIS_Coordinate_X[i] = stDISCoor.DIS_Coordinate_XC + (x_range*i);	//bh 2013.12.20 dis
	for(i=0;i<4;i++)	DIS_Coordinate_Y[i] = stDISCoor.DIS_Coordinate_YD + (y_range*i);


	MISP_MultiWrite(MISP_LOCAL_ID, 0x04CC, (PBYTE)DIS_Coordinate_X, 5*2);
	MISP_MultiWrite(MISP_LOCAL_ID, 0x04D1, (PBYTE)DIS_Coordinate_Y, 4*2);

	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x7010);	//[14] - DIS START by CPU
	
	//make vector table(32x32)
//		memset(dis_array, 0, sizeof(dis_array));
	
	valid_en = 0;	valid_cnt=0;
	while(valid_en == 0)	//check valid.
	{
		Delay_uSec(delay_cnt);									//bh 2013.12.20 dis

		MISP_RegRead(MISP_LOCAL_ID, 0x500, &DISCalDone);
		if((DISCalDone & 0x8000) != 0)	valid_en = 1;
		
		if(valid_cnt++ > DIS_CAL_TIME)	{UARTprintf("[4]");	break;}	//bh 2013.12.20 dis
	}
//		UARTprintf("4 count : %d\n", valid_cnt);
	
	MISP_MultiRead(MISP_LOCAL_ID, 0x4D5, (PBYTE)rVal[3], 88);	//read vector(60 set) + flag(60 set)
	

	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x3010);	//[14] - DIS END by CPU

		
	
	
	
		

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DIS 5th !!!   left
	// for MB,SA position
	for(i=0;i<5;i++)	DIS_Coordinate_X[i] = stDISCoor.DIS_Coordinate_XL + (x_range*i);	//bh 2013.12.20 dis
//		for(i=0;i<4;i++)	DIS_Coordinate_Y[i] = stDISCoor.DIS_Coordinate_YD + (y_range*i);

	MISP_MultiWrite(MISP_LOCAL_ID, 0x4CC, (PBYTE)DIS_Coordinate_X, 5*2);
//		MISP_MultiWrite(MISP_LOCAL_ID, 0x04D1, (PBYTE)DIS_Coordinate_Y, 4*2);
		
	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x7010);	//[14] - DIS START by CPU
	
	//make vector table(32x32)
//		memset(dis_array, 0, sizeof(dis_array));
	
	valid_en = 0;	valid_cnt=0;
	while(valid_en == 0)	//check valid.
	{
		Delay_uSec(delay_cnt);									//bh 2013.12.20 dis

		MISP_RegRead(MISP_LOCAL_ID, 0x500, &DISCalDone);
		if((DISCalDone & 0x8000) != 0)	valid_en = 1;

		if(valid_cnt++ > DIS_CAL_TIME)	{UARTprintf("[5]");	break;}	//bh 2013.12.20 dis
	}
//		UARTprintf("5 count : %d\n", valid_cnt);
	
	MISP_MultiRead(MISP_LOCAL_ID, 0x4D5, (PBYTE)rVal[4], 88);	//read vector(60 set) + flag(60 set)
		

	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x3010);	//[14] - DIS END by CPU

		
//		DIS_vsync_flag = 0;	//bh 2015.02.12 for dis wait halt..
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DIS 6th !!!  down side!!!	 right..
	// for MB,SA position
	for(i=0;i<5;i++)	DIS_Coordinate_X[i] = stDISCoor.DIS_Coordinate_XR + (x_range*i);	//bh 2013.12.20 dis

//		for(i=0;i<4;i++)	DIS_Coordinate_Y[i] = stDISCoor.DIS_Coordinate_YD + (y_range*i);

	MISP_MultiWrite(MISP_LOCAL_ID, 0x4CC, (PBYTE)DIS_Coordinate_X, 5*2);
//		MISP_MultiWrite(MISP_LOCAL_ID, 0x04D1, (PBYTE)DIS_Coordinate_Y, 4*2);
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x7010);	//[14] - DIS START by CPU
	
	//make vector table(32x32)
//		memset(dis_array, 0, sizeof(dis_array));
	
	valid_en = 0;	valid_cnt =0;
	while(valid_en == 0)	//check valid.
	{
		Delay_uSec(delay_cnt);									//bh 2013.12.20 dis

		MISP_RegRead(MISP_LOCAL_ID, 0x500, &DISCalDone);
		if((DISCalDone & 0x8000) != 0)	valid_en = 1;

		if(valid_cnt++ > DIS_CAL_TIME)	{UARTprintf("[6]");	break;}	//bh 2013.12.20 dis
	}
//		UARTprintf("6 count : %d\n", valid_cnt);
	
	MISP_MultiRead(MISP_LOCAL_ID, 0x4D5, (PBYTE)rVal[5], 88);	//read vector(60 set) + flag(60 set)
		
	MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x3010);	//[14] - DIS END by CPU
		
		
	for(j=0;j<6;j++) {
		flag_temp[j][0] = rVal[j][40];
		flag_temp[j][1] = rVal[j][41];
		flag_temp[j][2] = rVal[j][42];
		flag_temp[j][3] = rVal[j][43];
	}
	for(i=0;i<16;i++) {
		for(j=0;j<6;j++) {
			one_flag1 = (flag_temp[j][0]>>i) & 0x0001;
			one_flag2 = (flag_temp[j][1]>>i) & 0x0001;
			one_flag3 = (flag_temp[j][2]>>i) & 0x0001;
			one_flag4 = (flag_temp[j][3]>>i) & 0x0001;
			vector_flag[j][i] = one_flag1;
			vector_flag[j][i+16] = one_flag2;
			vector_flag[j][i+32] = one_flag3;
			vector_flag[j][i+48] = one_flag4;
		}
	}		
	
	memset(dis_array, 0, sizeof(dis_array));
	memset(flag_count, 0, sizeof(flag_count));
	addr_count = 0;
	
	//make vector table(32x32)
	for(i=0;i<40;i+=2)
	{
		for(j=0;j<6;j++) {
			x = rVal[j][i]>>10;			y = (rVal[j][i]>>5) & 0x1f;
			if (vector_flag[j][addr_count]==1) {
				dis_array[y][x]++;
				flag_count[j]++;
			}
		}
		
		addr_count++;
		
		for(j=0;j<6;j++) {
			x = rVal[j][i] & 0x1f;		y = (rVal[j][i+1]>>10) & 0x1f;
			if (vector_flag[j][addr_count]==1) {
				dis_array[y][x]++;
				flag_count[j]++;
			}
		}
		addr_count++;
		
		for(j=0;j<6;j++) {
			x = (rVal[j][i+1]>>5) & 0x1f;	y = rVal[j][i+1] & 0x1f;
			if (vector_flag[j][addr_count]==1) {
				dis_array[y][x]++;
				flag_count[j]++;
			}
		}
		addr_count++;
	}

	// for test..
//		UARTprintf("count flag : %d, %d, %d, %d, %d, %d \n", flag_count[0], flag_count[1], flag_count[2], flag_count[3], flag_count[4], flag_count[5] );
	

	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// cal. final vector.    
	
	
	//find max vector score(3x3 sum)
	vector_x=0;	vector_y=0;	vector_score=0;
//		for(i=0;i<32;i++)	{			//y
//			for(j=0;j<32;j++)	{		//x
	for(i=1;i<31;i++)	{			//y
		for(j=1;j<31;j++)	{		//x
			score = (dis_array[i-1][j-1] + dis_array[i-1][j] + dis_array[i-1][j+1]) +
					(dis_array[i][j-1] + dis_array[i][j]*2 + dis_array[i][j+1]) +
					(dis_array[i+1][j-1] + dis_array[i+1][j] + dis_array[i+1][j+1]);

			if(score > vector_score)	{
				vector_score = score;
				vector_x=j;
				vector_y=i;
			}
		}
	}
	
	

		

	#if defined ( __DIS_USE_ONLY_MAIN_EN__ )
		// not use aux data..
		// mX-16 = (aX-16)*4.
		// (mX-16)*2 = (aX-16)*4.
		if (ext_mode_en==1) {
			vector_aux_score = vector_score;
			vector_x_aux=(vector_x+16)/2;
			vector_y_aux=(vector_y+16)/2;
		}else {
			vector_aux_score = vector_score;
			vector_x_aux=(vector_x+48)/4;
			vector_y_aux=(vector_y+48)/4;
		}
		
	#endif
		
		

	
	// read FID (start)
	MISP_RegRead(MISP_LOCAL_ID, 0x05C, &DIS_MAIN_FID);			// start read field ID
	end_write_fid = (LOBYTE(DIS_MAIN_FID)&0x0F);
	
	if (start_write_fid != end_write_fid)
		UARTprintf("Wrong MAIN-FID !! \n");
		
#else
		// not use main data..
		// mX-16 = (aX-16)*4.
		// (mX-16)*2 = (aX-16)*4.
		if (ext_mode_en==1) {				
			vector_score = vector_aux_score;
//				vector_x=(vector_x_aux-8)*2;
//				vector_y=(vector_y_aux-8)*2;
			vector_x=((vector_x_aux-16) * GetIN_AuxSrcSizeH() / GetOUT_AuxSizeHA())/2 + 16;
			vector_y=((vector_y_aux-16) * GetIN_AuxSrcSizeV() / GetOUT_AuxSizeVA())/2 + 16;
		}else {
			vector_score = vector_aux_score;
//				vector_x=(vector_x_aux-12)*4;
//				vector_y=(vector_y_aux-12)*4;
			vector_x=(vector_x_aux-16) * GetIN_AuxSrcSizeH() / GetOUT_AuxSizeHA() + 16;
			vector_y=(vector_y_aux-16) * GetIN_AuxSrcSizeV() / GetOUT_AuxSizeVA() + 16;
		}
#endif
		
//		UARTprintf("aux : (%d, %d) ,   main : (%d, %d)     ext_mode_en : %d\n", vector_x_aux, vector_y_aux, vector_x, vector_y, ext_mode_en);
	
	// vector re-calc. output resolution standard. Standard is main (output hor., input ver.)
	if (ext_mode_en==1) {
		stdVector_x = (vector_x-16)*2;
		stdVector_y = (vector_y-16)*2;
	}else {
		stdVector_x = vector_x-16;
		stdVector_y = vector_y-16;
	}

//		stdVector_x_aux = (vector_x_aux-16) * 4 * GetOUT_MainSizeHA() / GetIN_MainSrcSizeH() + .5;
//		stdVector_y_aux = (vector_y_aux-16) * 4;
	if (GetIN_MainSrcSizeH() < GetOUT_MainSizeHA()) {
		stdVector_x_aux = (vector_x_aux-16) * GetIN_AuxSrcSizeH() / GetOUT_AuxSizeHA();
		stdVector_y_aux = (vector_y_aux-16) * GetIN_AuxSrcSizeV() / GetOUT_AuxSizeVA();
	}else {
		stdVector_x_aux = (vector_x_aux-16) * GetIN_AuxSrcSizeH() / GetOUT_AuxSizeHA() * GetOUT_MainSizeHA() / GetIN_MainSrcSizeH() + .5;
		stdVector_y_aux = (vector_y_aux-16) * GetIN_AuxSrcSizeV() / GetOUT_AuxSizeVA();
	}


	
//		UARTprintf("Std. aux : (%d, %d) ,   main : (%d, %d)     ext_mode_en : %d\n", stdVector_x_aux, stdVector_y_aux, stdVector_x, stdVector_y, ext_mode_en);
	
		
//		UARTprintf("main (%d, %d) -> (%d, %d)	aux (%d, %d) -> (%d, %d)\n", 
//			GetIN_MainSrcSizeH(), GetIN_MainSrcSizeV(), GetOUT_MainSizeHA(), GetOUT_MainSizeVA(), 
//			GetIN_AuxSrcSizeH(), GetIN_AuxSrcSizeV(), GetOUT_AuxSizeHA(), GetOUT_AuxSizeVA());
	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// final vector select

	// Read score threshold
	MISP_RegRead(MISP_LOCAL_ID, 0x050A, &rAux_thres);
	aux_score_thres = (HIBYTE(rAux_thres));
	score_thres = (LOBYTE(rAux_thres));
	
	aux_min_score_thres = DIS_thres;
	min_score_thres = DIS_thres;
	
//		if (ext_mode_en==1) {
//			if ( (ABS((vector_x-16)*2-(vector_x_aux-16)*4)<=4)&&(ABS((vector_y-16)*2-(vector_y_aux-16)*4)<=4) )
//				same_vector_en = 2;
//			else if ( (ABS((vector_x-16)*2-(vector_x_aux-16)*4)<=8)&&(ABS((vector_y-16)*2-(vector_y_aux-16)*4)<=8) )
//				same_vector_en = 1;
//			else same_vector_en = 0;
//		}else {
//			if ( (ABS((vector_x-16)-(vector_x_aux-16)*4)<=4)&&(ABS((vector_y-16)-(vector_y_aux-16)*4)<=4) )
//				same_vector_en = 2;
//			else if ( (ABS((vector_x-16)-(vector_x_aux-16)*4)<=8)&&(ABS((vector_y-16)-(vector_y_aux-16)*4)<=8) )
//				same_vector_en = 1;
//			else same_vector_en = 0;
//		}
	if ( (ABS(stdVector_x-stdVector_x_aux)<=4)&&(ABS(stdVector_y-stdVector_y_aux)<=4) )
		same_vector_en = 2;
	else if ( (ABS(stdVector_x-stdVector_x_aux)<=8)&&(ABS(stdVector_y-stdVector_y_aux)<=8) )
		same_vector_en = 1;
	else same_vector_en = 0;

	// for test..
//		UARTprintf("aux : (%d, %d) %d,   main : (%d, %d) %d    same_vector_en : %d   ext_mode_en : %d\n\n", vector_x_aux, vector_y_aux, vector_aux_score, vector_x, vector_y, vector_score, same_vector_en, ext_mode_en);
	
//		UARTprintf("aux score, main score : %d, %d\n", vector_aux_score, vector_score);

	
	// for LOW LIGHT
	if(GetNR_LowLightMode()) {			// If 'LOW LIGHT' mode, just return.
		move_x = 16;
		move_y = 16;
		vector_aux_only_en = 0;
		DIS_wait_cnt = 0;
		
	}else if (same_vector_en==2) {
		// main, aux 벡터 결과 매우 비슷
		if ( (vector_aux_score < (WORD)aux_min_score_thres)||(vector_score < (WORD)min_score_thres) ) {
			// main, aux 점수가 매우 낮으면 포기
			move_x = 16;
			move_y = 16;
			vector_aux_only_en = 0;
			DIS_wait_cnt = 0;
		}else {
			move_x = vector_x;
			move_y = vector_y;
			vector_aux_only_en = 0;
			if (DIS_wait_cnt < DIS_WAIT_DELAY) {
				DIS_wait_cnt++;
			}
		}
	}else if (same_vector_en==1) {
		// main, aux 벡터 결과 비슷
		if ( (vector_aux_score < (WORD)aux_min_score_thres)||(vector_score < (WORD)min_score_thres) ) {
			// main, aux 점수가 매우 낮으면 포기
			move_x = 16;
			move_y = 16;
			vector_aux_only_en = 0;
			DIS_wait_cnt = 0;
//			}else if ( (vector_aux_score >= (WORD)aux_score_thres)&&((ABS(vector_x_aux-16)>=14)||(ABS(vector_y_aux-16)>=14)) ) {		// 14*4=56. 벡터가 너무 크면 포기함.
		}else if ( (ABS(vector_x_aux-16)>14)||(ABS(vector_y_aux-16)>14) ) {		// 14*4=56. 벡터가 너무 크면 포기함.
			move_x = 16;
			move_y = 16;
			vector_aux_only_en = 0;
			DIS_wait_cnt = 0;
		}else if ( (vector_aux_score >= (WORD)aux_score_thres)&&((ABS(vector_x_aux-16)>8)||(ABS(vector_y_aux-16)>8)) ) {		// 8*4=32. 벡터가 크면 aux vector만 사용함.
			move_x = vector_x_aux;
			move_y = vector_y_aux;
			vector_aux_only_en = 1;
			if (DIS_wait_cnt < DIS_WAIT_DELAY) {
				DIS_wait_cnt++;
			}
		}else {
			if (vector_aux_score >= (WORD)aux_score_thres) {
				move_x = vector_x_aux;
				move_y = vector_y_aux;
				vector_aux_only_en = 1;
				if (DIS_wait_cnt < DIS_WAIT_DELAY) {
					DIS_wait_cnt++;
				}
			}else if (vector_score >= (WORD)score_thres) {
				move_x = vector_x;
				move_y = vector_y;
				vector_aux_only_en = 0;
				if (DIS_wait_cnt < DIS_WAIT_DELAY) {
					DIS_wait_cnt++;
				}
			}else {
				move_x = 16;
				move_y = 16;
				vector_aux_only_en = 0;
				DIS_wait_cnt = 0;
			}
		}
	}else {
//			if ( (vector_aux_score >= (WORD)aux_score_thres)&&((ABS(vector_x_aux-16)>=14)||(ABS(vector_y_aux-16)>=14)) ) {		// 14*4=56. 벡터가 너무 크면 포기함.
		if ( (ABS(vector_x_aux-16)>14)||(ABS(vector_y_aux-16)>14) ) {		// 14*4=56. 벡터가 너무 크면 포기
			move_x = 16;
			move_y = 16;
			vector_aux_only_en = 0;
			DIS_wait_cnt = 0;
		}else {
			move_x = vector_x_aux;
			move_y = vector_y_aux;
			vector_aux_only_en = 1;
			if (DIS_wait_cnt < DIS_WAIT_DELAY) {
				DIS_wait_cnt++;
			}
		}
	}
	
	
	// Read vector number..
	MISP_RegRead(MISP_LOCAL_ID, 0x509, &rVecNum);
	vec_num_for_ext = (HIBYTE(rVecNum));
	vec_num = (LOBYTE(rVecNum));
	
	
	
	memset(aux_vector_x_for_sum, 0, sizeof(aux_vector_x_for_sum));
	memset(aux_vector_y_for_sum, 0, sizeof(aux_vector_y_for_sum));
	memset(aux_vector_cnt, 0, sizeof(aux_vector_cnt));
	
	
	// 평균 벡터를 구함. 벡터 array에 저장. int 형식이라 -16 을 해서 넣음. 
	DIS_Vec_sum_x = DIS_Vec_sum_y = 0;
	ext_mode_en_for_next = 0;
	for(i=0;i<NUM_VEC;i++)	{
		
		// for average vector
		if (i!=NUM_VEC-1) {
			DIS_Vec_for_sum_x[NUM_VEC-1-i] = DIS_Vec_for_sum_x[NUM_VEC-1-i-1];
			DIS_Vec_for_sum_y[NUM_VEC-1-i] = DIS_Vec_for_sum_y[NUM_VEC-1-i-1];
			
			if (NUM_VEC-1-i < vec_num) {
				DIS_Vec_sum_x += DIS_Vec_for_sum_x[NUM_VEC-1-i-1];
				DIS_Vec_sum_y += DIS_Vec_for_sum_y[NUM_VEC-1-i-1];
			}
			
			if (NUM_VEC-1-i < vec_num_for_ext) {
				// 벡터가 12를 넘는다면 ext. mode.    넘지 않으면 normal mode..
				if ((ABS(DIS_Vec_for_sum_x[NUM_VEC-1-i-1])>=12)||(ABS(DIS_Vec_for_sum_y[NUM_VEC-1-i-1])>=12)) {
					ext_mode_en_for_next=1;
				}
			}
			
		}else {
			if (vector_aux_only_en == 1) {
				// AUX vector only !!
//					DIS_Vec_for_sum_x[0] = (vector_x_aux-16)*4;
				DIS_Vec_for_sum_x[0] = (vector_x_aux-16)*4 * GetOUT_MainSizeHA() / GetIN_MainSrcSizeH() + .5;
				DIS_Vec_for_sum_y[0] = (vector_y_aux-16)*4;
//					DIS_Vec_sum_x += (vector_x_aux-16)*4;
				DIS_Vec_sum_x += DIS_Vec_for_sum_x[0];
				DIS_Vec_sum_y += (vector_y_aux-16)*4;
				
			}else if (ext_mode_en==1) {
				DIS_Vec_for_sum_x[0] = (move_x-16)*2;
				DIS_Vec_for_sum_y[0] = (move_y-16)*2;
				DIS_Vec_sum_x += (move_x-16)*2;
				DIS_Vec_sum_y += (move_y-16)*2;
			}else {
				DIS_Vec_for_sum_x[0] = move_x-16;
				DIS_Vec_for_sum_y[0] = move_y-16;
//					DIS_Vec_for_sum_x[0] = ((move_x-16)/2)*2;
//					DIS_Vec_for_sum_y[0] = ((move_y-16)/2)*2;
				DIS_Vec_sum_x += move_x-16;
				DIS_Vec_sum_y += move_y-16;
			}
			if ((ABS(DIS_Vec_for_sum_x[0])>=12)||(ABS(DIS_Vec_for_sum_y[0])>=12)) {
				ext_mode_en_for_next=1;
			}
		}
	}
	
	// for average vector
	if (vec_num==0) {
		DIS_Vec_global_x = 0;
		DIS_Vec_global_y = 0;
	}else {
		DIS_Vec_global_x = DIS_Vec_sum_x / vec_num;
		DIS_Vec_global_y = DIS_Vec_sum_y / vec_num;
	}
	
	if (DIS_wait_cnt < DIS_WAIT_DELAY) {		// DIS wait state...
		DIS_Val_ref_x = stDISCoor.DIS_OFFSET_X;
		DIS_Val_ref_y = stDISCoor.DIS_OFFSET_Y;
	}else {
		
		if (pre_DIS_Val_ref_x + DIS_Vec_for_sum_x[0] > stDISCoor.DIS_OFFSET_X) {
			#if defined(__MN34420_1280x1024_ISPCROP__)
			alpha_vector_x = (pre_DIS_Val_ref_x+DIS_Vec_for_sum_x[0]-stDISCoor.DIS_OFFSET_X)/112.0f;
			#else
//			alpha_vector_x = (pre_DIS_Val_ref_x+DIS_Vec_for_sum_x[0]-stDISCoor.DIS_OFFSET_X)/128.0f;
			alpha_vector_x = (pre_DIS_Val_ref_x+DIS_Vec_for_sum_x[0]-stDISCoor.DIS_OFFSET_X)/256.0f;
			#endif
			DIS_Val_ref_x = pre_DIS_Val_ref_x + DIS_Vec_for_sum_x[0] - ((pre_DIS_Val_ref_x+DIS_Vec_for_sum_x[0]-stDISCoor.DIS_OFFSET_X)*alpha_vector_x);

		}else if (pre_DIS_Val_ref_x + DIS_Vec_for_sum_x[0] < stDISCoor.DIS_OFFSET_X) {
			#if defined(__MN34420_1280x1024_ISPCROP__)
			alpha_vector_x = (stDISCoor.DIS_OFFSET_X-pre_DIS_Val_ref_x-DIS_Vec_for_sum_x[0])/112.0f;
			#else
//			alpha_vector_x = (stDISCoor.DIS_OFFSET_X-pre_DIS_Val_ref_x-DIS_Vec_for_sum_x[0])/128.0f;
			alpha_vector_x = (stDISCoor.DIS_OFFSET_X-pre_DIS_Val_ref_x-DIS_Vec_for_sum_x[0])/256.0f;
			#endif
			DIS_Val_ref_x = pre_DIS_Val_ref_x + DIS_Vec_for_sum_x[0] + ((stDISCoor.DIS_OFFSET_X-pre_DIS_Val_ref_x-DIS_Vec_for_sum_x[0])*alpha_vector_x);
			
		}else {
			alpha_vector_x = 0;
			DIS_Val_ref_x = pre_DIS_Val_ref_x + DIS_Vec_for_sum_x[0];
		}
		
		if (pre_DIS_Val_ref_y + DIS_Vec_for_sum_y[0] > stDISCoor.DIS_OFFSET_Y) {
//			alpha_vector_y = (pre_DIS_Val_ref_y+DIS_Vec_for_sum_y[0]-stDISCoor.DIS_OFFSET_Y)/72.0f;
			alpha_vector_y = (pre_DIS_Val_ref_y+DIS_Vec_for_sum_y[0]-stDISCoor.DIS_OFFSET_Y)/144.0f;
			
			DIS_Val_ref_y = pre_DIS_Val_ref_y + DIS_Vec_for_sum_y[0] - ((pre_DIS_Val_ref_y+DIS_Vec_for_sum_y[0]-stDISCoor.DIS_OFFSET_Y)*alpha_vector_y);
			
		}else if (pre_DIS_Val_ref_y + DIS_Vec_for_sum_y[0] < stDISCoor.DIS_OFFSET_Y) {
//			alpha_vector_y = (stDISCoor.DIS_OFFSET_Y-pre_DIS_Val_ref_y-DIS_Vec_for_sum_y[0])/72.0f;
			alpha_vector_y = (stDISCoor.DIS_OFFSET_Y-pre_DIS_Val_ref_y-DIS_Vec_for_sum_y[0])/144.0f;
			
			DIS_Val_ref_y = pre_DIS_Val_ref_y + DIS_Vec_for_sum_y[0] + ((stDISCoor.DIS_OFFSET_Y-pre_DIS_Val_ref_y-DIS_Vec_for_sum_y[0])*alpha_vector_y);
			
		}else {
			alpha_vector_y = 0;
			DIS_Val_ref_y = pre_DIS_Val_ref_y + DIS_Vec_for_sum_y[0];
		}
	}
	
//		UARTprintf("DIS_Val_ref_x, DIS_Val_ref_y: %d %d\n", DIS_Val_ref_x, DIS_Val_ref_y);
	


	if (max_frame_num==3) {
		// Final vector is saved (for 4 frame mode)
		if (start_write_fid==0) {
			if ((GetIN_MainSrcSizeV()==1080)||(GetIN_MainSrcSizeV()==2160)) {		// must be moved by 2 pixel !!......BUT 1920x1080p is ...
//					DIS_vector_array[3][0] = DIS_Val_ref_x;
//					DIS_vector_array[3][1] = DIS_Val_ref_y;
				DIS_vector_array[3][0] = ((WORD)(DIS_Val_ref_x/2))*2;
				DIS_vector_array[3][1] = ((WORD)(DIS_Val_ref_y/2))*2;
			}else {
				DIS_vector_array[3][0] = ((WORD)(DIS_Val_ref_x/2))*2;
				DIS_vector_array[3][1] = ((WORD)(DIS_Val_ref_y/2))*2;
//					DIS_vector_array[3][0] = DIS_Val_ref_x;
//					DIS_vector_array[3][1] = DIS_Val_ref_y;
			}
			DIS_vector_flag_array[3] = 1;
//			UARTprintf("make vector num. : 3\r\n");
			
		}else {
			if ((GetIN_MainSrcSizeV()==1080)||(GetIN_MainSrcSizeV()==2160)) {		// must be moved by 2 pixel !!......BUT 1920x1080p is ...
//					DIS_vector_array[start_write_fid-1][0] = DIS_Val_ref_x;
//					DIS_vector_array[start_write_fid-1][1] = DIS_Val_ref_y;
				DIS_vector_array[start_write_fid-1][0] = ((WORD)(DIS_Val_ref_x/2))*2;
				DIS_vector_array[start_write_fid-1][1] = ((WORD)(DIS_Val_ref_y/2))*2;
			}else {
				DIS_vector_array[start_write_fid-1][0] = ((WORD)(DIS_Val_ref_x/2))*2;
				DIS_vector_array[start_write_fid-1][1] = ((WORD)(DIS_Val_ref_y/2))*2;
			}
			DIS_vector_flag_array[start_write_fid-1] = 1;
//			UARTprintf("make vector num. : %d\r\n", start_write_fid-1);
		}
	}else if (max_frame_num==2) {
		// Final vector is saved (for 3 frame mode)
		if (start_write_fid==0) {
			if ((GetIN_MainSrcSizeV()==1080)||(GetIN_MainSrcSizeV()==2160)) {		// must be moved by 2 pixel !!......BUT 1920x1080p is ...
//					DIS_vector_array[2][0] = DIS_Val_ref_x;
//					DIS_vector_array[2][1] = DIS_Val_ref_y;
				DIS_vector_array[2][0] = ((WORD)(DIS_Val_ref_x/2))*2;
				DIS_vector_array[2][1] = ((WORD)(DIS_Val_ref_y/2))*2;
			}else {
				DIS_vector_array[2][0] = ((WORD)(DIS_Val_ref_x/2))*2;
				DIS_vector_array[2][1] = ((WORD)(DIS_Val_ref_y/2))*2;
			}
			DIS_vector_flag_array[2] = 1;
//			UARTprintf("make vector num. : 3\r\n");
			
		}else {
			if ((GetIN_MainSrcSizeV()==1080)||(GetIN_MainSrcSizeV()==2160)) {
//					DIS_vector_array[start_write_fid-1][0] = DIS_Val_ref_x;
//					DIS_vector_array[start_write_fid-1][1] = DIS_Val_ref_y;
				DIS_vector_array[start_write_fid-1][0] = ((WORD)(DIS_Val_ref_x/2))*2;
				DIS_vector_array[start_write_fid-1][1] = ((WORD)(DIS_Val_ref_y/2))*2;
			}else {
				DIS_vector_array[start_write_fid-1][0] = ((WORD)(DIS_Val_ref_x/2))*2;
				DIS_vector_array[start_write_fid-1][1] = ((WORD)(DIS_Val_ref_y/2))*2;
			}
			DIS_vector_flag_array[start_write_fid-1] = 1;
//			UARTprintf("make vector num. : %d\r\n", start_write_fid-1);
		}
	}

	// read FID (start)
	MISP_RegRead(MISP_LOCAL_ID, 0x05C, &DIS_MAIN_FID);			// start read field ID
	end_write_fid = (LOBYTE(DIS_MAIN_FID)&0x0F);
	
	if (start_write_fid != end_write_fid)
		UARTprintf("Wrong OUT-FID !! \n");
	
	
//		UARTprintf("make vector num. : %d\n", start_write_fid-1);
//		UARTprintf("make vector num. : %d\n", start_write_fid);
//		UARTprintf("DIS_vector_array X, DIS_vector_array Y: %d %d\n", DIS_vector_array[0][0], DIS_vector_array[0][1]);
//		UARTprintf("DIS_vector_array X, DIS_vector_array Y: %d %d\n", DIS_vector_array[1][0], DIS_vector_array[1][1]);
//		UARTprintf("DIS_vector_array X, DIS_vector_array Y: %d %d\n", DIS_vector_array[2][0], DIS_vector_array[2][1]);
//		UARTprintf("DIS_vector_array X, DIS_vector_array Y: %d %d\n", DIS_vector_array[3][0], DIS_vector_array[3][1]);
	
	
	
//		// read FID (end)
//		MISP_RegRead(MISP_LOCAL_ID, 0x05C, &DIS_MAIN_FID);			// end read field ID
//		last_read_fid = ((LOBYTE(DIS_MAIN_FID)&0xF0)>>4);
//		end_write_fid = (LOBYTE(DIS_MAIN_FID)&0x0F);
	
//		MISP_RegRead(MISP_LOCAL_ID, 0x2FF, &DIS_AUX_FID);			// end read field ID
//		end_read_fid_aux = ((LOBYTE(DIS_AUX_FID)&0xF0)>>4);
//		end_write_fid_aux = (LOBYTE(DIS_AUX_FID)&0x0F);
	
	
		
	// save pre-DATA...
	pre_DIS_Val_ref_x = DIS_Val_ref_x;
	pre_DIS_Val_ref_y = DIS_Val_ref_y;
	
	
	// for next ext. range mode....
	if (ext_mode_en_for_next == 1) {
		DIS_EXT_Range = 0x0010;
	}else {
		DIS_EXT_Range = 0x0000;
	}
	MISP_RegWrite(MISP_LOCAL_ID, 0x04C7, DIS_EXT_Range);

		
		
}


void Init_DIS(void) {
	
		DIS_enable = 0;			// init. DIS enable..
//		DIS_black_screen = 0;	// init. black screen frame counter
		
	#if defined(__USE_CMOS_IMX226__) || defined(__USE_CMOS_IMX274__) || (defined(__USE_CMOS_IMX334__))
		#if defined(__USE_SENS_PANNING_MODE__)
			MISP_RegWrite(MISP_LOCAL_ID, 0x04C4, 0x1630);	// memory data request delay. 22. req. by req....and DIS 4 frame setting
			// memory data request delay. 2K : __USE_MANUAL_LENS__, __USE_DDR_MCLK414__, clk. div. 12  --> 0x8C(140)
			MISP_RegWrite(MISP_LOCAL_ID, 0x0508, 0x008C);
		#else
			MISP_RegWrite(MISP_LOCAL_ID, 0x04C4, 0x1620);	// memory data request delay. 22. req. by req....and DIS 3 frame setting
			// memory data request delay. 4K : __USE_AFZOOM_LENS__, __USE_DDR_MCLK414__, clk. div. 12  --> 0xA0(160)
			MISP_RegWrite(MISP_LOCAL_ID, 0x0508, 0x00A0);
		#endif
	#else 
		MISP_RegWrite(MISP_LOCAL_ID, 0x04C4, 0x1630);	// memory data request delay. 22. req. by req....and DIS 4 frame setting
			
		// memory data request delay. 2K : __USE_MANUAL_LENS__, __USE_DDR_MCLK414__, clk. div. 12  --> 0x8C(140)
		MISP_RegWrite(MISP_LOCAL_ID, 0x0508, 0x008C);
	#endif
		
		// DIS change threshold...for ZOOM mode..(edge threshold..)
		MISP_RegWrite(MISP_LOCAL_ID, 0x04C5, 0x0505);
		MISP_RegWrite(MISP_LOCAL_ID, 0x04C6, 0x0500);
		
		// DIS CPU master setting.!!!
		MISP_RegWrite(MISP_LOCAL_ID, 0x04C0, 0x4128);
//		MISP_RegWrite(MISP_LOCAL_ID, 0x04C9, 0x3010);	//[14] - DIS END by CPU
		// extend mode
		MISP_RegWrite(MISP_LOCAL_ID, 0x04C7, 0x0010);
		
		// interrupt mode enable. h count threshold is 1.
		MISP_RegWrite(MISP_LOCAL_ID, 0x0501, 0x0001);
		
		// for cpu variable..
		MISP_RegWrite(MISP_LOCAL_ID, 0x0503, 0x0000);		// 
		MISP_RegWrite(MISP_LOCAL_ID, 0x0504, 0x1010);		// aux min score thres : 16, min score thres : 16
		MISP_RegWrite(MISP_LOCAL_ID, 0x0509, 0x1008);		// vec_num_for_ext : 16, vec_num : 8
		MISP_RegWrite(MISP_LOCAL_ID, 0x050A, 0x2020);		// aux score thres     : 32, score thres     : 32
		
		// input size
		MISP_RegWrite(MISP_LOCAL_ID, 0x0506, GetIN_MainSrcSizeH());		// H size
		MISP_RegWrite(MISP_LOCAL_ID, 0x0507, GetIN_MainSrcSizeV());		// V size
		
		
		DIS_Val_ref_x=stDISCoor.DIS_OFFSET_X;			// DIS vector init.
		DIS_Val_ref_y=stDISCoor.DIS_OFFSET_Y;
		
		DIS_wait = 0;
		
		DIS_wait_cnt = 0;
}


//--------------------------------------------------------------------------------------------------------------------------
void SetDIS_OnOff(WORD OnOff) {		// used demo.c
	DIS_enable=OnOff;
}

WORD GetDIS_OnOff(void) {			// used ir.c
	return DIS_enable;
}

void SetDIS_Wait(void)	{			// used ir.c
	DIS_wait_cnt = 0;
}

//--------------------------------------------------------------------------------------------------------------------------


//void SetDIS_bayer_vect(WORD val) {
//	DIS_bayer_vect=val;
//}

//WORD GetDIS_bayer_vect(void) {
//	return DIS_bayer_vect;
//}



void Set_DIS_Interesting_Area(void) {		// coordinate X must be multiple 8 and 6. 8 is for 8bit mode. 6 is for 10bit mode. 
	
	if (GetIN_MainSrcSizeV()==1080) {			// if 1080p
		// 1920x1080 mode
		stDISCoor.DIS_Coordinate_XL	= 288;
		stDISCoor.DIS_Coordinate_XC	= 768;
		stDISCoor.DIS_Coordinate_XR	= 1248;
		stDISCoor.DIS_Coordinate_YU	= 156;
		stDISCoor.DIS_Coordinate_YD	= 540;
		// AUX 480x270
		stDISCoor.DIS_AUX_Coordinate_XL	= 96;
		stDISCoor.DIS_AUX_Coordinate_XR	= 252;
		stDISCoor.DIS_AUX_Coordinate_YU	= 84;
		stDISCoor.DIS_AUX_Coordinate_YD	= 140;
		// range
		stDISCoor.aux_x_interval	= 24;
		stDISCoor.aux_y_interval	= 16;
		stDISCoor.x_interval	= 96;
		stDISCoor.y_interval	= 96;
		stDISCoor.DIS_OFFSET_X = 126;
		stDISCoor.DIS_OFFSET_Y = 72;

//		UARTprintf("Enter 1080p \r\n");		
	}else if (GetIN_MainSrcSizeV()==1536) {		// if 1536p
		// 2048x1536 mode
		stDISCoor.DIS_Coordinate_XL	= 288;
		stDISCoor.DIS_Coordinate_XC	= 768;
		stDISCoor.DIS_Coordinate_XR	= 1248;
		stDISCoor.DIS_Coordinate_YU	= 156;
		stDISCoor.DIS_Coordinate_YD	= 540;
		// AUX 512x384
		stDISCoor.DIS_AUX_Coordinate_XL	= 96;
		stDISCoor.DIS_AUX_Coordinate_XR	= 252;
		stDISCoor.DIS_AUX_Coordinate_YU	= 84;
		stDISCoor.DIS_AUX_Coordinate_YD	= 140;
		// range
		stDISCoor.aux_x_interval	= 48;
		stDISCoor.aux_y_interval	= 16;
		stDISCoor.x_interval	= 96;
		stDISCoor.y_interval	= 96;
		stDISCoor.DIS_OFFSET_X = 126;
		stDISCoor.DIS_OFFSET_Y = 72;

//		UARTprintf("Enter 2048x1536p \r\n");			
	}else if (GetIN_MainSrcSizeV()==2160) {		// if 2160p
		// 4096(or 3840)x2160 mode
/*			DIS_Coordinate_XL	= 720;
		DIS_Coordinate_XC	= 1602;
		DIS_Coordinate_XR	= 2484;
		DIS_Coordinate_YU	= 760;
		DIS_Coordinate_YD	= 1400;
		// AUX 1024(or 960)x540
		DIS_AUX_Coordinate_XL	= 96;
		DIS_AUX_Coordinate_XR	= 256;
		DIS_AUX_Coordinate_YU	= 64;
		DIS_AUX_Coordinate_YD	= 140;
		// range
		aux_x_interval	= 64;
		aux_y_interval	= 32;
		x_interval	= 128;
		y_interval	= 128;
		DIS_OFFSET_X = 128;
		DIS_OFFSET_Y = 72;*/
		stDISCoor.DIS_Coordinate_XL	= 576;
		stDISCoor.DIS_Coordinate_XC	= 1536;
		stDISCoor.DIS_Coordinate_XR	= 2496;
		stDISCoor.DIS_Coordinate_YU	= 312;
		stDISCoor.DIS_Coordinate_YD	= 1080;
		// AUX 1024(or 960)x540
		stDISCoor.DIS_AUX_Coordinate_XL	= 192;
		stDISCoor.DIS_AUX_Coordinate_XR	= 504;
		stDISCoor.DIS_AUX_Coordinate_YU	= 168;
		stDISCoor.DIS_AUX_Coordinate_YD	= 280;
		// range
		stDISCoor.aux_x_interval	= 72;
		stDISCoor.aux_y_interval	= 32;
		stDISCoor.x_interval	= 192;
		stDISCoor.y_interval	= 192;
		stDISCoor.DIS_OFFSET_X = 126;		// 256
		stDISCoor.DIS_OFFSET_Y = 72;		// 144
//		UARTprintf("Enter 2160p \r\n");				
	}else if (GetIN_MainSrcSizeV()==720) {											// if 720p
		// 1280x720 mode
		stDISCoor.DIS_Coordinate_XL	= 288;
		stDISCoor.DIS_Coordinate_XC	= 480;
		stDISCoor.DIS_Coordinate_XR	= 720;
		stDISCoor.DIS_Coordinate_YU	= 168;
		stDISCoor.DIS_Coordinate_YD	= 360;
		// AUX 320x180
		stDISCoor.DIS_AUX_Coordinate_XL = 96;
		stDISCoor.DIS_AUX_Coordinate_XR	= 174;
		stDISCoor.DIS_AUX_Coordinate_YU	= 26;
		stDISCoor.DIS_AUX_Coordinate_YD	= 90;
		// range	
		stDISCoor.aux_x_interval	= 24;
		stDISCoor.aux_y_interval	= 24;
		stDISCoor.x_interval	= 48;
		stDISCoor.y_interval	= 48;
		stDISCoor.DIS_OFFSET_X = 126;
		stDISCoor.DIS_OFFSET_Y = 72;
/*			// 1920x1080 mode
		stDISCoor.DIS_Coordinate_XL	= 288;
		stDISCoor.DIS_Coordinate_XC	= 768;
		stDISCoor.DIS_Coordinate_XR	= 1248;
		stDISCoor.DIS_Coordinate_YU	= 156;
		stDISCoor.DIS_Coordinate_YD	= 540;
		// AUX 480x270
		stDISCoor.DIS_AUX_Coordinate_XL	= 96;
		stDISCoor.DIS_AUX_Coordinate_XR	= 256;
		stDISCoor.DIS_AUX_Coordinate_YU	= 84;
		stDISCoor.DIS_AUX_Coordinate_YD	= 140;
		// range
		stDISCoor.aux_x_interval	= 32;
		stDISCoor.aux_y_interval	= 16;
		stDISCoor.x_interval	= 96;
		stDISCoor.y_interval	= 96;
		stDISCoor.DIS_OFFSET_X = 128;
		stDISCoor.DIS_OFFSET_Y = 72;*/
//		UARTprintf("Enter 720p \r\n");				
	}else if (GetIN_MainSrcSizeV()==1208) {		// from 2048x1208 to 1920x1080 without overscanning
		// 1920x1080 mode
		stDISCoor.DIS_Coordinate_XL	= 288;
		stDISCoor.DIS_Coordinate_XC	= 768;
		stDISCoor.DIS_Coordinate_XR	= 1248;
		stDISCoor.DIS_Coordinate_YU	= 156;
		stDISCoor.DIS_Coordinate_YD	= 540;
		// AUX 480x270
		stDISCoor.DIS_AUX_Coordinate_XL	= 96;
		stDISCoor.DIS_AUX_Coordinate_XR	= 252;
		stDISCoor.DIS_AUX_Coordinate_YU	= 84;
		stDISCoor.DIS_AUX_Coordinate_YD	= 140;
		// range
		stDISCoor.aux_x_interval	= 24;
		stDISCoor.aux_y_interval	= 16;
		stDISCoor.x_interval	= 96;
		stDISCoor.y_interval	= 96;
		stDISCoor.DIS_OFFSET_X = 126;
		stDISCoor.DIS_OFFSET_Y = 72;
//		UARTprintf("Enter 1208p \r\n");	
	}
	else if (GetIN_MainSrcSizeV()==1088) {		// from 1344x1088 to 1280x1024 without overscanning
		// 1344x1088 mode
		stDISCoor.DIS_Coordinate_XL	= 240;
		stDISCoor.DIS_Coordinate_XC	= 480;
		stDISCoor.DIS_Coordinate_XR	= 720;
		stDISCoor.DIS_Coordinate_YU	= 156;
		stDISCoor.DIS_Coordinate_YD	= 540;
		// AUX 480x270
		stDISCoor.DIS_AUX_Coordinate_XL	= 96;
		stDISCoor.DIS_AUX_Coordinate_XR	= 252;
		stDISCoor.DIS_AUX_Coordinate_YU	= 84;
		stDISCoor.DIS_AUX_Coordinate_YD	= 140;
		// range
		stDISCoor.aux_x_interval	= 24;
		stDISCoor.aux_y_interval	= 16;
		stDISCoor.x_interval	= 96;
		stDISCoor.y_interval	= 96;
		stDISCoor.DIS_OFFSET_X = 32;
		stDISCoor.DIS_OFFSET_Y = 32;
		UARTprintf("Enter 1344x1088p \r\n");	
	}else {									// if 480p
			// 720x480 mode
			stDISCoor.DIS_Coordinate_XL	= 144;
			stDISCoor.DIS_Coordinate_XC	= 240;
			stDISCoor.DIS_Coordinate_XR	= 360;
			stDISCoor.DIS_Coordinate_YU	= 84;
			stDISCoor.DIS_Coordinate_YD	= 180;
			// AUX 180x120
			stDISCoor.DIS_AUX_Coordinate_XL = 48;
			stDISCoor.DIS_AUX_Coordinate_XR	= 88;
			stDISCoor.DIS_AUX_Coordinate_YU	= 13;
			stDISCoor.DIS_AUX_Coordinate_YD	= 45;
			// range	
			stDISCoor.aux_x_interval	= 12;
			stDISCoor.aux_y_interval	= 12;
			stDISCoor.x_interval	= 24;
			stDISCoor.y_interval	= 24;
			stDISCoor.DIS_OFFSET_X = 64;
			stDISCoor.DIS_OFFSET_Y = 36;
//		UARTprintf("Enter else-480p \r\n");							
	}
}


/*  FILE_END _HERE */

