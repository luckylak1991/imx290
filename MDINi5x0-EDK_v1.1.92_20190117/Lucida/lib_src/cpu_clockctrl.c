/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/

#include "macroimg.h"

U32 get_ahb_clock()
{
	return OSC_CLOCK;
}

U32 get_apb_clock()
{
    U32 ahbclock = get_ahb_clock()/2;

    return ahbclock;
}

