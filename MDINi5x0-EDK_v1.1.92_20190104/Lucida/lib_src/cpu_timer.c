/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#include "macroimg.h"

BOOL set_timer_ms(U32 nCh, U32 ms)
{
	static const int timerintnum[] = {INTNUM_TIMER0, INTNUM_TIMER1, INTNUM_TIMER2, INTNUM_TIMER3, INTNUM_TIMER4, INTNUM_TIMER5};
	
	if(nCh > MAX_TIMER_CHANNEL)
		return FALSE;
	
	U32 i;
	U32 nanosecpertick;
	U32 cnt;
	U32 pres;	
	U32 scaler[] = {2, 8, 32, 128, 512, 2048, 8192, 32768, 0};
	U32 usec = ms*1000;
	
	i=0;
	
	while(1)
	{
		pres = scaler[i];
		
		if(pres == 0)
		{
			return FALSE;
		}
		
		nanosecpertick = 1000000000/(get_apb_clock()/pres);
		cnt = (usec*1000) / nanosecpertick;
		
		if(cnt < 0xffff)
		{
			break;
		}
		
		i++;
	}
	
	*R_TPCTRL(nCh) = F_TPCTRL_CNTCLR_SET;
	*R_TPCTRL(nCh) = F_TPCTRL_CNTCLR_CLR;
	
	*R_TMCNT(nCh) = cnt;
	*R_TMCTRL(nCh) = (1<<7 | F_TMCTRL_PFSEL(i) | F_TMCTRL_TMEN_EN);
	
	enable_interrupt(timerintnum[nCh],TRUE);
	
	return TRUE;
}

BOOL stop_timer(U32 nCh)
{
	if(nCh>MAX_TIMER_CHANNEL)
		return FALSE;
	
	// PRINTVAR(*R_TMCTRL(nCh));
	*R_TMCTRL(nCh) &= ~F_TMCTRL_TMEN_EN;
	// PRINTVAR(*R_TMCTRL(nCh));
	
	return TRUE;
}

static void ISPM loopdelayms(U32 ms)
{
	register U32 i;
	register U32 cnt;
	
	cnt = ms*9850;
	
	for(i=0;i < cnt ;i++)
	{
		asm("nop"); 
	}
}

void ISPM delayus(U32 us)
{
	register U32 i;
	register U32 cnt;
	
	cnt = us*100;
	
	for(i=0;i < cnt ;i++)
	{
		asm("nop"); 
	}
}

/*
valid prescale value
2,8,32,128,512,2048,8192,23768
Tick/sec  = (APB clock)/prescale
if APB clock is 48Mhz and prescale is 2048.
	1 tick duration = 1000/(APB clock/prescale) = 0.042ms

if you want to more accuracy, PRESCALE value is should be down
*/
#define MAX_TIMER_PRESCALE 2048
static volatile int delaytimerisrtick=0;
static void delaytimerisr()
{
	delaytimerisrtick++;
}

BOOL delayms(U32 ms)
{
	int nch;

	U32 conval;
	for(nch=0; nch<MAX_TIMER_CHANNEL+1; nch++)
	{
		if( (*R_TMCTRL(nch) & F_TMCTRL_TMEN_EN) == 0)
		{
			break;
		}
	}
	
	if(nch == (MAX_TIMER_CHANNEL+1) )
	{
		debugstring("software delay\r\n");
		loopdelayms(ms);
		return TRUE;
	}
	
#if (MAX_TIMER_PRESCALE==32768)
	conval = 7<<1;
#elif (MAX_TIMER_PRESCALE==8192)
	conval = 6<<1;
#elif (MAX_TIMER_PRESCALE==2048)
	conval = 5<<1;
#elif (MAX_TIMER_PRESCALE==512)
	conval = 4<<1;
#elif (MAX_TIMER_PRESCALE==128)
	conval = 3<<1;
#elif (MAX_TIMER_PRESCALE==32)
	conval = 2<<1;
#elif (MAX_TIMER_PRESCALE==8)
	conval = 1<<1;
#elif (MAX_TIMER_PRESCALE==2)
	conval = 0;
#else
#error invalid MAX_TIMER_PRESCALE
	debugstring("Error MAX_TIMER_PRESCALE\r\n");
	
	return FALSE;
#endif
	
	void* orgisr;
	INTERRUPT_TYPE intnum;
	
	switch(nch)
	{
		case 0:
			intnum = INTNUM_TIMER0;
			break;
		case 1:
			intnum = INTNUM_TIMER1;
			break;
		case 2:
			intnum = INTNUM_TIMER2;
			break;
		case 3:
			intnum = INTNUM_TIMER3;
			break;
		case 4:
			intnum = INTNUM_TIMER4;
			break;
		case 5:
			intnum = INTNUM_TIMER5;
			break;
		default:
			return FALSE;
	}
	
	delaytimerisrtick = 0;
	orgisr = get_interrupt(intnum);
	set_interrupt(intnum, delaytimerisr);
	enable_interrupt(intnum, TRUE);
	
	U32 waitdelaytimerisrtick = 0;
	U32 tickperms = get_apb_clock()/(MAX_TIMER_PRESCALE*1000);
	U32 waittick = tickperms*ms;
	waitdelaytimerisrtick = waittick>>16;
	waittick = waittick&0xffff;

	*R_TPCTRL(nch) = F_TPCTRL_CNTCLR_SET;
	*R_TPCTRL(nch) = F_TPCTRL_CNTCLR_CLR;
	
	*R_TMCNT(nch) = MAX_TIMER_COUNT;
	*R_TMCTRL(nch) = (1<<7 | conval | F_TMCTRL_TMEN_EN);

	while(1)
	{
		if(waitdelaytimerisrtick<delaytimerisrtick)
		{
			break;
		}
		else if(waitdelaytimerisrtick==delaytimerisrtick)
		{
			if(waittick < *R_TMCNT(nch))
			{
				*R_TMCTRL(nch) = 0;
				break;
			}
		}
	}
	
	enable_interrupt(intnum, FALSE);
	set_interrupt(intnum, orgisr);
	
	return TRUE;
}
