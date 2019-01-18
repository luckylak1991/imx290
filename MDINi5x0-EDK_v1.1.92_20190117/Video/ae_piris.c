#include	"video.h"

#if defined(__ECOM1_UNION_12X__)

typedef	struct
{
	SHORT	min;
	SHORT	max;
	SHORT	pmin;
	SHORT	pmax;

	SHORT	cur_step;
	SHORT	cur_pos;
	SHORT	mov_pos;
	SHORT	enable;
}	PIRIS_PARAM, *PPIRIS_PARAM;

static PIRIS_PARAM stPIRIS = {
	80,
	450,
	0,
	525,

	0,
	0,

	0
};

//--------------------------------------------------------------------------------------------------
static void a3901_step(int out)
{
	if(out==0){			P_IRIS_AP_T(HIGH);	P_IRIS_AM_T(LOW);	P_IRIS_BP_T(HIGH);	P_IRIS_BM_T(LOW);	}
	else if(out==1){		P_IRIS_AP_T(LOW);	P_IRIS_AM_T(LOW);	P_IRIS_BP_T(HIGH);	P_IRIS_BM_T(LOW);	}
	else if(out==2){		P_IRIS_AP_T(LOW);	P_IRIS_AM_T(HIGH);	P_IRIS_BP_T(HIGH);	P_IRIS_BM_T(LOW);	}
	else if(out==3){		P_IRIS_AP_T(LOW);	P_IRIS_AM_T(HIGH);	P_IRIS_BP_T(LOW);	P_IRIS_BM_T(LOW);	}
	else if(out==4){		P_IRIS_AP_T(LOW);	P_IRIS_AM_T(HIGH);	P_IRIS_BP_T(LOW);	P_IRIS_BM_T(HIGH);	}
	else if(out==5){		P_IRIS_AP_T(LOW);	P_IRIS_AM_T(LOW);	P_IRIS_BP_T(LOW);	P_IRIS_BM_T(HIGH);	}
	else if(out==6){		P_IRIS_AP_T(HIGH);	P_IRIS_AM_T(LOW);	P_IRIS_BP_T(LOW);	P_IRIS_BM_T(HIGH);	}
	else if(out==7){		P_IRIS_AP_T(HIGH);	P_IRIS_AM_T(LOW);	P_IRIS_BP_T(LOW);	P_IRIS_BM_T(LOW);	}
}


//--------------------------------------------------------------------------------------------------
static void a3901_Rotation(BOOL init)
{
	int i,k;

	if(init) {

		// 80+370+75 = 525 // go to max position
		// open full to pmax
		for(i=0;i<stPIRIS.pmax;i++){
			a3901_step(stPIRIS.cur_step);
			if(--stPIRIS.cur_step<0) stPIRIS.cur_step=7;
			Delay_uSec(200);//Delay_mSec(1);
		}

		//close to max
		for(i=0;i<(stPIRIS.pmax - stPIRIS.max);i++){
			a3901_step(stPIRIS.cur_step);
			if(++stPIRIS.cur_step>7) stPIRIS.cur_step=0;
			Delay_uSec(200);//Delay_mSec(1);
		}

		stPIRIS.cur_pos = stPIRIS.max;
		stPIRIS.mov_pos = stPIRIS.max;
		//UARTprintf("mov_pos:%d / cur_pos:%d\r\n",stPIRIS.mov_pos,stPIRIS.cur_pos);
	} else {
		//UARTprintf("mov_pos:%d / cur_pos:%d\r\n",stPIRIS.mov_pos,stPIRIS.cur_pos);
		if(stPIRIS.mov_pos == stPIRIS.cur_pos) return;

    #if 1
		if(stPIRIS.mov_pos < stPIRIS.cur_pos) {
			k = stPIRIS.cur_pos - stPIRIS.mov_pos;
			for(i=0;i<k;i++){
				a3901_step(stPIRIS.cur_step);
				if(++stPIRIS.cur_step>7) stPIRIS.cur_step=0;
				Delay_uSec(100);
				stPIRIS.cur_pos--;
			}
		} else {
			k = stPIRIS.mov_pos - stPIRIS.cur_pos;
			for(i=0;i<k;i++){
				a3901_step(stPIRIS.cur_step);
				if(--stPIRIS.cur_step<0) stPIRIS.cur_step=7;
				Delay_uSec(100);
				stPIRIS.cur_pos++;
			}
		}
    #else
		if(stPIRIS.mov_pos < stPIRIS.cur_pos) {
			if(++stPIRIS.cur_step>7) stPIRIS.cur_step=0;
			a3901_step(stPIRIS.cur_step);
			stPIRIS.cur_pos--;
		} else {
			if(--stPIRIS.cur_step<0) stPIRIS.cur_step=7;
			a3901_step(stPIRIS.cur_step);
			stPIRIS.cur_pos++;
		}
    #endif
	}
}

//--------------------------------------------------------------------------------------------------
void AECTRL_PIRIS_Handler(void)
{
	a3901_Rotation(FALSE);
}

//--------------------------------------------------------------------------------------------------
void AECTRL_PIRIS_Init(void)
{
	a3901_Rotation(TRUE);
}

//--------------------------------------------------------------------------------------------------
void AECTRL_PIRIS_ForceSetPosition(SHORT ipos)
{
	int i;

	if(ipos > stPIRIS.cur_pos) {
		for(i=0;i<(ipos-stPIRIS.cur_pos);i++){
			if(--stPIRIS.cur_step<0) stPIRIS.cur_step=7;
			a3901_step(stPIRIS.cur_step);
			Delay_uSec(20);//Delay_mSec(1);
		}
	} else {
		for(i=0;i<(stPIRIS.cur_pos-ipos);i++){
			if(++stPIRIS.cur_step>7) stPIRIS.cur_step=0;
			a3901_step(stPIRIS.cur_step);
			Delay_uSec(20);//Delay_mSec(1);
		}
	}
	stPIRIS.cur_pos = ipos;
	stPIRIS.mov_pos = ipos;
	//UARTprintf("mov_pos:%d / cur_pos:%d\r\n",stPIRIS.mov_pos,stPIRIS.cur_pos);
}

//--------------------------------------------------------------------------------------------------
void AECTRL_PIRIS_SetPosition(SHORT ipos)
{
	//UARTprintf("mov_pos : %d\r\n", stPIRIS.mov_pos);
	stPIRIS.mov_pos = ipos;
}

//--------------------------------------------------------------------------------------------------
SHORT AECTRL_PIRIS_GetCurPosition(void)
{
	return stPIRIS.cur_pos;
}

//--------------------------------------------------------------------------------------------------
SHORT AECTRL_PIRIS_GetMaxPosition(void)
{
	return stPIRIS.max;
}

//--------------------------------------------------------------------------------------------------
SHORT AECTRL_PIRIS_GetMinPosition(void)
{
	return stPIRIS.min;
}

#endif
