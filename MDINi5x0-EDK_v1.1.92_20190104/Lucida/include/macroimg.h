/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

*****************************************************************************/
#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#define ISPM	__attribute__((section(".ispm")))
#define DSPM	__attribute__((section(".dspm")))

#ifdef __GNUG__ // The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C"{
#endif

#include "lib_config.h"
#include "typedef.h"
#include "macroimg/cache.h"
#include "macroimg/gpio.h"
#include "macroimg/interrupt.h"
#include "macroimg/spi.h"
#include "macroimg/timer.h"
#include "macroimg/twi.h"
#include "macroimg/uart.h"
#include "macroimg/util.h"
#include "macroimg/watchdog.h"
#include "macroimg/sfctrl.h"
#include "macroimg/clockctrl.h"
#include "macroimg/adc.h"

#ifdef __GNUG__
}
#endif
