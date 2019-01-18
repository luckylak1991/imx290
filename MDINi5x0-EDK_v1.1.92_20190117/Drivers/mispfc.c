//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2014  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:	MISPFC.C
// Description 		:
// Ref. Docment		: 
// Revision History 	:

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"misp100.h"
#include	"cmos_if.h"		// for sensor dependent option

// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
static FC_CTRL_PARAM	stFCCTRL;

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
#if defined(__USE_MDIN_i500__)		// MDIN-i500
// Spline, B=0.00, C=1.00, 4-tap, 64-phase for interpolation filter coefficients
ROMDATA MISP_MFCFILT_COEF MISP_MFCFilter_Spline_1_00		= {
// Spline, B=0.00, C=1.00, 4-tap, lower 32-phase Scale=2048 Sum=2048
	{(WORD) 2048, (WORD)    0,		// phase 00
	 (WORD) 2044, (WORD)  -60,		// phase 02
	 (WORD) 2033, (WORD) -113,		// phase 04
	 (WORD) 2014, (WORD) -158,		// phase 06
	 (WORD) 1988, (WORD) -196,		// phase 08
	 (WORD) 1956, (WORD) -228,		// phase 10
	 (WORD) 1918, (WORD) -254,		// phase 12
	 (WORD) 1873, (WORD) -273,		// phase 14
	 (WORD) 1824, (WORD) -288,		// phase 16
	 (WORD) 1770, (WORD) -298,		// phase 18
	 (WORD) 1711, (WORD) -303,		// phase 20
	 (WORD) 1647, (WORD) -303,		// phase 22
	 (WORD) 1580, (WORD) -300,		// phase 24
	 (WORD) 1509, (WORD) -293,		// phase 26
	 (WORD) 1436, (WORD) -284,		// phase 28
	 (WORD) 1359, (WORD) -271,		// phase 30
	 (WORD) 1280, (WORD) -256,		// phase 32
	 (WORD) 1199, (WORD) -239,		// phase 34
	 (WORD) 1117, (WORD) -221,		// phase 36
	 (WORD) 1033, (WORD) -201,		// phase 38
	 (WORD)  948, (WORD) -180,		// phase 40
	 (WORD)  863, (WORD) -159,		// phase 42
	 (WORD)  778, (WORD) -138,		// phase 44
	 (WORD)  692, (WORD) -116,		// phase 46
	 (WORD)  608, (WORD)  -96,		// phase 48
	 (WORD)  525, (WORD)  -77,		// phase 50
	 (WORD)  443, (WORD)  -59,		// phase 52
	 (WORD)  362, (WORD)  -42,		// phase 54
	 (WORD)  284, (WORD)  -28,		// phase 56
	 (WORD)  208, (WORD)  -16,		// phase 58
	 (WORD)  136, (WORD)   -8,		// phase 60
	 (WORD)   66, (WORD)   -2 },	// phase 62

// Lanczos, B=0.10, C=1.00, 4-tap, upper 32-phase
	{(WORD) 1136, (WORD)  456,		// phase 00
	 (WORD) 1136, (WORD)  427,		// phase 02
	 (WORD) 1133, (WORD)  399,		// phase 04
	 (WORD) 1129, (WORD)  372,		// phase 06
	 (WORD) 1123, (WORD)  346,		// phase 08
	 (WORD) 1115, (WORD)  320,		// phase 10
	 (WORD) 1106, (WORD)  295,		// phase 12
	 (WORD) 1095, (WORD)  271,		// phase 14
	 (WORD) 1082, (WORD)  248,		// phase 16
	 (WORD) 1068, (WORD)  225,		// phase 18
	 (WORD) 1052, (WORD)  204,		// phase 20
	 (WORD) 1034, (WORD)  184,		// phase 22
	 (WORD) 1015, (WORD)  165,		// phase 24
	 (WORD)  994, (WORD)  147,		// phase 26
	 (WORD)  972, (WORD)  130,		// phase 28
	 (WORD)  949, (WORD)  114,		// phase 30
	 (WORD)  925, (WORD)   99,		// phase 32
	 (WORD)  899, (WORD)   86,		// phase 34
	 (WORD)  872, (WORD)   74,		// phase 36
	 (WORD)  845, (WORD)   62,		// phase 38
	 (WORD)  816, (WORD)   52,		// phase 40
	 (WORD)  788, (WORD)   42,		// phase 42
	 (WORD)  758, (WORD)   34,		// phase 44
	 (WORD)  728, (WORD)   27,		// phase 46
	 (WORD)  698, (WORD)   20,		// phase 48
	 (WORD)  667, (WORD)   15,		// phase 50
	 (WORD)  636, (WORD)   11,		// phase 52
	 (WORD)  606, (WORD)    7,		// phase 54
	 (WORD)  575, (WORD)    4,		// phase 56
	 (WORD)  545, (WORD)    2,		// phase 58
	 (WORD)  515, (WORD)    1,		// phase 60
	 (WORD)  485, (WORD)    0 }		// phase 62
};

// Spline, B=0.00, C=0.75, 4-tap, 64-phase for interpolation filter coefficients
ROMDATA MISP_MFCFILT_COEF MISP_MFCFilter_Spline_0_75		= {
// Spline, B=0.00, C=0.75, 4-tap, lower 32-phase Scale=2048 Sum=2048
	{(WORD) 2048, (WORD)    0,		// phase 00
	 (WORD) 2044, (WORD)  -45,		// phase 02
	 (WORD) 2031, (WORD)  -84,		// phase 04
	 (WORD) 2009, (WORD) -118,		// phase 06
	 (WORD) 1981, (WORD) -147,		// phase 08
	 (WORD) 1945, (WORD) -171,		// phase 10
	 (WORD) 1903, (WORD) -190,		// phase 12
	 (WORD) 1854, (WORD) -205,		// phase 14
	 (WORD) 1800, (WORD) -216,		// phase 16
	 (WORD) 1740, (WORD) -223,		// phase 18
	 (WORD) 1676, (WORD) -227,		// phase 20
	 (WORD) 1607, (WORD) -227,		// phase 22
	 (WORD) 1535, (WORD) -225,		// phase 24
	 (WORD) 1459, (WORD) -220,		// phase 26
	 (WORD) 1380, (WORD) -213,		// phase 28
	 (WORD) 1299, (WORD) -203,		// phase 30
	 (WORD) 1216, (WORD) -192,		// phase 32
	 (WORD) 1131, (WORD) -179,		// phase 34
	 (WORD) 1046, (WORD) -165,		// phase 36
	 (WORD)  959, (WORD) -150,		// phase 38
	 (WORD)  873, (WORD) -135,		// phase 40
	 (WORD)  787, (WORD) -119,		// phase 42
	 (WORD)  702, (WORD) -103,		// phase 44
	 (WORD)  618, (WORD)  -87,		// phase 46
	 (WORD)  536, (WORD)  -72,		// phase 48
	 (WORD)  456, (WORD)  -57,		// phase 50
	 (WORD)  379, (WORD)  -44,		// phase 52
	 (WORD)  305, (WORD)  -31,		// phase 54
	 (WORD)  235, (WORD)  -21,		// phase 56
	 (WORD)  169, (WORD)  -12,		// phase 58
	 (WORD)  107, (WORD)   -6,		// phase 60
	 (WORD)   51, (WORD)   -2 },	// phase 62

// Lanczos, B=0.10, C=1.00, 4-tap, upper 32-phase
	{(WORD) 1136, (WORD)  456,		// phase 00
	 (WORD) 1136, (WORD)  427,		// phase 02
	 (WORD) 1133, (WORD)  399,		// phase 04
	 (WORD) 1129, (WORD)  372,		// phase 06
	 (WORD) 1123, (WORD)  346,		// phase 08
	 (WORD) 1115, (WORD)  320,		// phase 10
	 (WORD) 1106, (WORD)  295,		// phase 12
	 (WORD) 1095, (WORD)  271,		// phase 14
	 (WORD) 1082, (WORD)  248,		// phase 16
	 (WORD) 1068, (WORD)  225,		// phase 18
	 (WORD) 1052, (WORD)  204,		// phase 20
	 (WORD) 1034, (WORD)  184,		// phase 22
	 (WORD) 1015, (WORD)  165,		// phase 24
	 (WORD)  994, (WORD)  147,		// phase 26
	 (WORD)  972, (WORD)  130,		// phase 28
	 (WORD)  949, (WORD)  114,		// phase 30
	 (WORD)  925, (WORD)   99,		// phase 32
	 (WORD)  899, (WORD)   86,		// phase 34
	 (WORD)  872, (WORD)   74,		// phase 36
	 (WORD)  845, (WORD)   62,		// phase 38
	 (WORD)  816, (WORD)   52,		// phase 40
	 (WORD)  788, (WORD)   42,		// phase 42
	 (WORD)  758, (WORD)   34,		// phase 44
	 (WORD)  728, (WORD)   27,		// phase 46
	 (WORD)  698, (WORD)   20,		// phase 48
	 (WORD)  667, (WORD)   15,		// phase 50
	 (WORD)  636, (WORD)   11,		// phase 52
	 (WORD)  606, (WORD)    7,		// phase 54
	 (WORD)  575, (WORD)    4,		// phase 56
	 (WORD)  545, (WORD)    2,		// phase 58
	 (WORD)  515, (WORD)    1,		// phase 60
	 (WORD)  485, (WORD)    0 }		// phase 62
};

// Spline, B=0.00, C=0.50, 4-tap, 64-phase for interpolation filter coefficients
ROMDATA MISP_MFCFILT_COEF MISP_MFCFilter_Spline_0_50		= {
// Spline, B=0.00, C=0.50, 4-tap, lower 32-phase Scale=2048 Sum=2048
	{(WORD) 2048, (WORD)    0,		// phase 00
	 (WORD) 2043, (WORD)  -30,		// phase 02
	 (WORD) 2029, (WORD)  -56,		// phase 04
	 (WORD) 2006, (WORD)  -79,		// phase 06
	 (WORD) 1974, (WORD)  -98,		// phase 08
	 (WORD) 1935, (WORD) -114,		// phase 10
	 (WORD) 1888, (WORD) -127,		// phase 12
	 (WORD) 1835, (WORD) -137,		// phase 14
	 (WORD) 1776, (WORD) -144,		// phase 16
	 (WORD) 1711, (WORD) -149,		// phase 18
	 (WORD) 1642, (WORD) -151,		// phase 20
	 (WORD) 1568, (WORD) -152,		// phase 22
	 (WORD) 1490, (WORD) -150,		// phase 24
	 (WORD) 1409, (WORD) -147,		// phase 26
	 (WORD) 1325, (WORD) -142,		// phase 28
	 (WORD) 1239, (WORD) -135,		// phase 30
	 (WORD) 1152, (WORD) -128,		// phase 32
	 (WORD) 1064, (WORD) -120,		// phase 34
	 (WORD)  975, (WORD) -110,		// phase 36
	 (WORD)  886, (WORD) -100,		// phase 38
	 (WORD)  798, (WORD)  -90,		// phase 40
	 (WORD)  711, (WORD)  -79,		// phase 42
	 (WORD)  626, (WORD)  -69,		// phase 44
	 (WORD)  544, (WORD)  -58,		// phase 46
	 (WORD)  464, (WORD)  -48,		// phase 48
	 (WORD)  388, (WORD)  -38,		// phase 50
	 (WORD)  316, (WORD)  -29,		// phase 52
	 (WORD)  248, (WORD)  -21,		// phase 54
	 (WORD)  186, (WORD)  -14,		// phase 56
	 (WORD)  129, (WORD)   -8,		// phase 58
	 (WORD)   79, (WORD)   -4,		// phase 60
	 (WORD)   36, (WORD)   -1 },	// phase 62

// Lanczos, B=0.10, C=1.00, 4-tap, upper 32-phase
	{(WORD) 1136, (WORD)  456,		// phase 00
	 (WORD) 1136, (WORD)  427,		// phase 02
	 (WORD) 1133, (WORD)  399,		// phase 04
	 (WORD) 1129, (WORD)  372,		// phase 06
	 (WORD) 1123, (WORD)  346,		// phase 08
	 (WORD) 1115, (WORD)  320,		// phase 10
	 (WORD) 1106, (WORD)  295,		// phase 12
	 (WORD) 1095, (WORD)  271,		// phase 14
	 (WORD) 1082, (WORD)  248,		// phase 16
	 (WORD) 1068, (WORD)  225,		// phase 18
	 (WORD) 1052, (WORD)  204,		// phase 20
	 (WORD) 1034, (WORD)  184,		// phase 22
	 (WORD) 1015, (WORD)  165,		// phase 24
	 (WORD)  994, (WORD)  147,		// phase 26
	 (WORD)  972, (WORD)  130,		// phase 28
	 (WORD)  949, (WORD)  114,		// phase 30
	 (WORD)  925, (WORD)   99,		// phase 32
	 (WORD)  899, (WORD)   86,		// phase 34
	 (WORD)  872, (WORD)   74,		// phase 36
	 (WORD)  845, (WORD)   62,		// phase 38
	 (WORD)  816, (WORD)   52,		// phase 40
	 (WORD)  788, (WORD)   42,		// phase 42
	 (WORD)  758, (WORD)   34,		// phase 44
	 (WORD)  728, (WORD)   27,		// phase 46
	 (WORD)  698, (WORD)   20,		// phase 48
	 (WORD)  667, (WORD)   15,		// phase 50
	 (WORD)  636, (WORD)   11,		// phase 52
	 (WORD)  606, (WORD)    7,		// phase 54
	 (WORD)  575, (WORD)    4,		// phase 56
	 (WORD)  545, (WORD)    2,		// phase 58
	 (WORD)  515, (WORD)    1,		// phase 60
	 (WORD)  485, (WORD)    0 }		// phase 62
};

ROMDATA MISP_MFCFILT_COEF MISP_MFCFilter_BSpline_Most_Blur	= {
// Spline, B=1.00, C=0.00, 4-tap, lower 32-phase Scale=2048 Sum=2048
	{(WORD) 1365, (WORD)  341,		// phase 00
	 (WORD) 1364, (WORD)  310,		// phase 02
	 (WORD) 1358, (WORD)  281,		// phase 04
	 (WORD) 1348, (WORD)  254,		// phase 06
	 (WORD) 1335, (WORD)  229,		// phase 08
	 (WORD) 1319, (WORD)  205,		// phase 10
	 (WORD) 1300, (WORD)  183,		// phase 12
	 (WORD) 1278, (WORD)  163,		// phase 14
	 (WORD) 1253, (WORD)  144,		// phase 16
	 (WORD) 1226, (WORD)  127,		// phase 18
	 (WORD) 1197, (WORD)  111,		// phase 20
	 (WORD) 1165, (WORD)   96,		// phase 22
	 (WORD) 1131, (WORD)   83,		// phase 24
	 (WORD) 1096, (WORD)   71,		// phase 26
	 (WORD) 1059, (WORD)   61,		// phase 28
	 (WORD) 1021, (WORD)   51,		// phase 30
	 (WORD)  981, (WORD)   43,		// phase 32
	 (WORD)  941, (WORD)   35,		// phase 34
	 (WORD)  900, (WORD)   29,		// phase 36
	 (WORD)  858, (WORD)   23,		// phase 38
	 (WORD)  815, (WORD)   18,		// phase 40
	 (WORD)  773, (WORD)   14,		// phase 42
	 (WORD)  730, (WORD)   10,		// phase 44
	 (WORD)  687, (WORD)    8,		// phase 46
	 (WORD)  645, (WORD)    5,		// phase 48
	 (WORD)  604, (WORD)    3,		// phase 50
	 (WORD)  563, (WORD)    2,		// phase 52
	 (WORD)  523, (WORD)    1,		// phase 54
	 (WORD)  483, (WORD)    1,		// phase 56
	 (WORD)  446, (WORD)    0,		// phase 58
	 (WORD)  409, (WORD)    0,		// phase 60
	 (WORD)  374, (WORD)    0 }, 	// phase 62

// Spline, B=1.00, C=0.00, 2-tap, upper 32-phase Scale=2048 Sum=2048
	{(WORD) 1365, (WORD)  341,		// phase 00
	 (WORD) 1364, (WORD)  310,		// phase 02
	 (WORD) 1358, (WORD)  281,		// phase 04
	 (WORD) 1348, (WORD)  254,		// phase 06
	 (WORD) 1335, (WORD)  229,		// phase 08
	 (WORD) 1319, (WORD)  205,		// phase 10
	 (WORD) 1300, (WORD)  183,		// phase 12
	 (WORD) 1278, (WORD)  163,		// phase 14
	 (WORD) 1253, (WORD)  144,		// phase 16
	 (WORD) 1226, (WORD)  127,		// phase 18
	 (WORD) 1197, (WORD)  111,		// phase 20
	 (WORD) 1165, (WORD)   96,		// phase 22
	 (WORD) 1131, (WORD)   83,		// phase 24
	 (WORD) 1096, (WORD)   71,		// phase 26
	 (WORD) 1059, (WORD)   61,		// phase 28
	 (WORD) 1021, (WORD)   51,		// phase 30
	 (WORD)  981, (WORD)   43,		// phase 32
	 (WORD)  941, (WORD)   35,		// phase 34
	 (WORD)  900, (WORD)   29,		// phase 36
	 (WORD)  858, (WORD)   23,		// phase 38
	 (WORD)  815, (WORD)   18,		// phase 40
	 (WORD)  773, (WORD)   14,		// phase 42
	 (WORD)  730, (WORD)   10,		// phase 44
	 (WORD)  687, (WORD)    8,		// phase 46
	 (WORD)  645, (WORD)    5,		// phase 48
	 (WORD)  604, (WORD)    3,		// phase 50
	 (WORD)  563, (WORD)    2,		// phase 52
	 (WORD)  523, (WORD)    1,		// phase 54
	 (WORD)  483, (WORD)    1,		// phase 56
	 (WORD)  446, (WORD)    0,		// phase 58
	 (WORD)  409, (WORD)    0,		// phase 60
	 (WORD)  374, (WORD)    0 }		// phase 62
};

// Bi-linear (2-tap), 32-phase Scale=2048 Sum=2048 for interpolation filter coefficients
ROMDATA MISP_MFCFILT_COEF MISP_MFCFilter_Bilinear		= {
	{(WORD) 2048, (WORD)    0,		// phase 00
	 (WORD) 1984, (WORD)    0,		// phase 02
	 (WORD) 1920, (WORD)    0,		// phase 04
	 (WORD) 1856, (WORD)    0,		// phase 06
	 (WORD) 1792, (WORD)    0,		// phase 08
	 (WORD) 1728, (WORD)    0,		// phase 10
	 (WORD) 1664, (WORD)    0,		// phase 12
	 (WORD) 1600, (WORD)    0,		// phase 14
	 (WORD) 1536, (WORD)    0,		// phase 16
	 (WORD) 1472, (WORD)    0,		// phase 18
	 (WORD) 1408, (WORD)    0,		// phase 20
	 (WORD) 1344, (WORD)    0,		// phase 22
	 (WORD) 1280, (WORD)    0,		// phase 24
	 (WORD) 1216, (WORD)    0,		// phase 26
	 (WORD) 1152, (WORD)    0,		// phase 28
	 (WORD) 1088, (WORD)    0,		// phase 30
	 (WORD) 1024, (WORD)    0,		// phase 32
	 (WORD)  960, (WORD)    0,		// phase 34
	 (WORD)  896, (WORD)    0,		// phase 36
	 (WORD)  832, (WORD)    0,		// phase 38
	 (WORD)  768, (WORD)    0,		// phase 40
	 (WORD)  704, (WORD)    0,		// phase 42
	 (WORD)  640, (WORD)    0,		// phase 44
	 (WORD)  576, (WORD)    0,		// phase 46
	 (WORD)  512, (WORD)    0,		// phase 48
	 (WORD)  448, (WORD)    0,		// phase 50
	 (WORD)  384, (WORD)    0,		// phase 52
	 (WORD)  320, (WORD)    0,		// phase 54
	 (WORD)  256, (WORD)    0,		// phase 56
	 (WORD)  192, (WORD)    0,		// phase 58
	 (WORD)  128, (WORD)    0,		// phase 60
	 (WORD)   64, (WORD)    0 },	// phase 62

// Lanczos, B=0.10, C=1.00, 4-tap, upper 32-phase
	{(WORD) 1136, (WORD)  456,		// phase 00
	 (WORD) 1136, (WORD)  427,		// phase 02
	 (WORD) 1133, (WORD)  399,		// phase 04
	 (WORD) 1129, (WORD)  372,		// phase 06
	 (WORD) 1123, (WORD)  346,		// phase 08
	 (WORD) 1115, (WORD)  320,		// phase 10
	 (WORD) 1106, (WORD)  295,		// phase 12
	 (WORD) 1095, (WORD)  271,		// phase 14
	 (WORD) 1082, (WORD)  248,		// phase 16
	 (WORD) 1068, (WORD)  225,		// phase 18
	 (WORD) 1052, (WORD)  204,		// phase 20
	 (WORD) 1034, (WORD)  184,		// phase 22
	 (WORD) 1015, (WORD)  165,		// phase 24
	 (WORD)  994, (WORD)  147,		// phase 26
	 (WORD)  972, (WORD)  130,		// phase 28
	 (WORD)  949, (WORD)  114,		// phase 30
	 (WORD)  925, (WORD)   99,		// phase 32
	 (WORD)  899, (WORD)   86,		// phase 34
	 (WORD)  872, (WORD)   74,		// phase 36
	 (WORD)  845, (WORD)   62,		// phase 38
	 (WORD)  816, (WORD)   52,		// phase 40
	 (WORD)  788, (WORD)   42,		// phase 42
	 (WORD)  758, (WORD)   34,		// phase 44
	 (WORD)  728, (WORD)   27,		// phase 46
	 (WORD)  698, (WORD)   20,		// phase 48
	 (WORD)  667, (WORD)   15,		// phase 50
	 (WORD)  636, (WORD)   11,		// phase 52
	 (WORD)  606, (WORD)    7,		// phase 54
	 (WORD)  575, (WORD)    4,		// phase 56
	 (WORD)  545, (WORD)    2,		// phase 58
	 (WORD)  515, (WORD)    1,		// phase 60
	 (WORD)  485, (WORD)    0 }		// phase 62
};

// More-Blur (2-tap), 32-phase for interpolation filter coefficients
ROMDATA MISP_MFCFILT_COEF MISP_MFCFilter_BSpline_More_Blur	= {
// Spline, B=0.50, C=0.25, 4-tap, lower 32-phase Scale=2048 Sum=2048
	{(WORD) 1707, (WORD)  171,		// phase 00
	 (WORD) 1703, (WORD)  140,		// phase 02
	 (WORD) 1693, (WORD)  113,		// phase 04
	 (WORD) 1677, (WORD)   88,		// phase 06
	 (WORD) 1655, (WORD)   65,		// phase 08
	 (WORD) 1627, (WORD)   46,		// phase 10
	 (WORD) 1594, (WORD)   28,		// phase 12
	 (WORD) 1556, (WORD)   13,		// phase 14
	 (WORD) 1515, (WORD)    0,		// phase 16
	 (WORD) 1469, (WORD)  -11,		// phase 18
	 (WORD) 1419, (WORD)  -20,		// phase 20
	 (WORD) 1366, (WORD)  -27,		// phase 22
	 (WORD) 1311, (WORD)  -33,		// phase 24
	 (WORD) 1253, (WORD)  -38,		// phase 26
	 (WORD) 1192, (WORD)  -40,		// phase 28
	 (WORD) 1130, (WORD)  -42,		// phase 30
	 (WORD) 1067, (WORD)  -43,		// phase 32
	 (WORD) 1002, (WORD)  -42,		// phase 34
	 (WORD)  937, (WORD)  -41,		// phase 36
	 (WORD)  872, (WORD)  -39,		// phase 38
	 (WORD)  807, (WORD)  -36,		// phase 40
	 (WORD)  742, (WORD)  -33,		// phase 42
	 (WORD)  678, (WORD)  -29,		// phase 44
	 (WORD)  615, (WORD)  -25,		// phase 46
	 (WORD)  555, (WORD)  -21,		// phase 48
	 (WORD)  496, (WORD)  -17,		// phase 50
	 (WORD)  439, (WORD)  -13,		// phase 52
	 (WORD)  385, (WORD)  -10,		// phase 54
	 (WORD)  335, (WORD)   -7,		// phase 56
	 (WORD)  287, (WORD)   -4,		// phase 58
	 (WORD)  244, (WORD)   -2,		// phase 60
	 (WORD)  205, (WORD)    0 },	// phase 62

// Lanczos, B=0.10, C=1.00, 4-tap, upper 32-phase
	{(WORD) 1136, (WORD)  456,		// phase 00
	 (WORD) 1136, (WORD)  427,		// phase 02
	 (WORD) 1133, (WORD)  399,		// phase 04
	 (WORD) 1129, (WORD)  372,		// phase 06
	 (WORD) 1123, (WORD)  346,		// phase 08
	 (WORD) 1115, (WORD)  320,		// phase 10
	 (WORD) 1106, (WORD)  295,		// phase 12
	 (WORD) 1095, (WORD)  271,		// phase 14
	 (WORD) 1082, (WORD)  248,		// phase 16
	 (WORD) 1068, (WORD)  225,		// phase 18
	 (WORD) 1052, (WORD)  204,		// phase 20
	 (WORD) 1034, (WORD)  184,		// phase 22
	 (WORD) 1015, (WORD)  165,		// phase 24
	 (WORD)  994, (WORD)  147,		// phase 26
	 (WORD)  972, (WORD)  130,		// phase 28
	 (WORD)  949, (WORD)  114,		// phase 30
	 (WORD)  925, (WORD)   99,		// phase 32
	 (WORD)  899, (WORD)   86,		// phase 34
	 (WORD)  872, (WORD)   74,		// phase 36
	 (WORD)  845, (WORD)   62,		// phase 38
	 (WORD)  816, (WORD)   52,		// phase 40
	 (WORD)  788, (WORD)   42,		// phase 42
	 (WORD)  758, (WORD)   34,		// phase 44
	 (WORD)  728, (WORD)   27,		// phase 46
	 (WORD)  698, (WORD)   20,		// phase 48
	 (WORD)  667, (WORD)   15,		// phase 50
	 (WORD)  636, (WORD)   11,		// phase 52
	 (WORD)  606, (WORD)    7,		// phase 54
	 (WORD)  575, (WORD)    4,		// phase 56
	 (WORD)  545, (WORD)    2,		// phase 58
	 (WORD)  515, (WORD)    1,		// phase 60
	 (WORD)  485, (WORD)    0 }		// phase 62
};
#endif	//__USE_MDIN_i500__

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
// Spline, B=0.00, C=1.00, 4-tap, 32-phase Scale=2048 Sum=2048 for interpolation filter coefficients
ROMDATA WORD MISP_MFCFilter_Spline_1_00_4T_32P[2*32]		= {
	(WORD) 2048, (WORD)    0,		// phase 00
	 (WORD) 2044, (WORD)  -60,		// phase 02
	 (WORD) 2033, (WORD) -113,		// phase 04
	 (WORD) 2014, (WORD) -158,		// phase 06
	 (WORD) 1988, (WORD) -196,		// phase 08
	 (WORD) 1956, (WORD) -228,		// phase 10
	 (WORD) 1918, (WORD) -254,		// phase 12
	 (WORD) 1873, (WORD) -273,		// phase 14
	 (WORD) 1824, (WORD) -288,		// phase 16
	 (WORD) 1770, (WORD) -298,		// phase 18
	 (WORD) 1711, (WORD) -303,		// phase 20
	 (WORD) 1647, (WORD) -303,		// phase 22
	 (WORD) 1580, (WORD) -300,		// phase 24
	 (WORD) 1509, (WORD) -293,		// phase 26
	 (WORD) 1436, (WORD) -284,		// phase 28
	 (WORD) 1359, (WORD) -271,		// phase 30
	 (WORD) 1280, (WORD) -256,		// phase 32
	 (WORD) 1199, (WORD) -239,		// phase 34
	 (WORD) 1117, (WORD) -221,		// phase 36
	 (WORD) 1033, (WORD) -201,		// phase 38
	 (WORD)  948, (WORD) -180,		// phase 40
	 (WORD)  863, (WORD) -159,		// phase 42
	 (WORD)  778, (WORD) -138,		// phase 44
	 (WORD)  692, (WORD) -116,		// phase 46
	 (WORD)  608, (WORD)  -96,		// phase 48
	 (WORD)  525, (WORD)  -77,		// phase 50
	 (WORD)  443, (WORD)  -59,		// phase 52
	 (WORD)  362, (WORD)  -42,		// phase 54
	 (WORD)  284, (WORD)  -28,		// phase 56
	 (WORD)  208, (WORD)  -16,		// phase 58
	 (WORD)  136, (WORD)   -8,		// phase 60
	 (WORD)   66, (WORD)   -2,	// phase 62
};

// Spline, B=0.00, C=0.75, 4-tap, 32-phase Scale=2048 Sum=2048 for interpolation filter coefficients
ROMDATA WORD MISP_MFCFilter_Spline_0_75_4T_32P[2*32]		= {
	(WORD) 2048, (WORD)    0,		// phase 00
	 (WORD) 2044, (WORD)  -45,		// phase 02
	 (WORD) 2031, (WORD)  -84,		// phase 04
	 (WORD) 2009, (WORD) -118,		// phase 06
	 (WORD) 1981, (WORD) -147,		// phase 08
	 (WORD) 1945, (WORD) -171,		// phase 10
	 (WORD) 1903, (WORD) -190,		// phase 12
	 (WORD) 1854, (WORD) -205,		// phase 14
	 (WORD) 1800, (WORD) -216,		// phase 16
	 (WORD) 1740, (WORD) -223,		// phase 18
	 (WORD) 1676, (WORD) -227,		// phase 20
	 (WORD) 1607, (WORD) -227,		// phase 22
	 (WORD) 1535, (WORD) -225,		// phase 24
	 (WORD) 1459, (WORD) -220,		// phase 26
	 (WORD) 1380, (WORD) -213,		// phase 28
	 (WORD) 1299, (WORD) -203,		// phase 30
	 (WORD) 1216, (WORD) -192,		// phase 32
	 (WORD) 1131, (WORD) -179,		// phase 34
	 (WORD) 1046, (WORD) -165,		// phase 36
	 (WORD)  959, (WORD) -150,		// phase 38
	 (WORD)  873, (WORD) -135,		// phase 40
	 (WORD)  787, (WORD) -119,		// phase 42
	 (WORD)  702, (WORD) -103,		// phase 44
	 (WORD)  618, (WORD)  -87,		// phase 46
	 (WORD)  536, (WORD)  -72,		// phase 48
	 (WORD)  456, (WORD)  -57,		// phase 50
	 (WORD)  379, (WORD)  -44,		// phase 52
	 (WORD)  305, (WORD)  -31,		// phase 54
	 (WORD)  235, (WORD)  -21,		// phase 56
	 (WORD)  169, (WORD)  -12,		// phase 58
	 (WORD)  107, (WORD)   -6,		// phase 60
	 (WORD)   51, (WORD)   -2,	// phase 62
};

// Spline, B=0.00, C=0.50, 4-tap, 32-phase Scale=2048 Sum=2048 for interpolation filter coefficients
ROMDATA WORD MISP_MFCFilter_Spline_0_50_4T_32P[2*32]		= {
	(WORD) 2048, (WORD)    0,		// phase 00
	 (WORD) 2043, (WORD)  -30,		// phase 02
	 (WORD) 2029, (WORD)  -56,		// phase 04
	 (WORD) 2006, (WORD)  -79,		// phase 06
	 (WORD) 1974, (WORD)  -98,		// phase 08
	 (WORD) 1935, (WORD) -114,		// phase 10
	 (WORD) 1888, (WORD) -127,		// phase 12
	 (WORD) 1835, (WORD) -137,		// phase 14
	 (WORD) 1776, (WORD) -144,		// phase 16
	 (WORD) 1711, (WORD) -149,		// phase 18
	 (WORD) 1642, (WORD) -151,		// phase 20
	 (WORD) 1568, (WORD) -152,		// phase 22
	 (WORD) 1490, (WORD) -150,		// phase 24
	 (WORD) 1409, (WORD) -147,		// phase 26
	 (WORD) 1325, (WORD) -142,		// phase 28
	 (WORD) 1239, (WORD) -135,		// phase 30
	 (WORD) 1152, (WORD) -128,		// phase 32
	 (WORD) 1064, (WORD) -120,		// phase 34
	 (WORD)  975, (WORD) -110,		// phase 36
	 (WORD)  886, (WORD) -100,		// phase 38
	 (WORD)  798, (WORD)  -90,		// phase 40
	 (WORD)  711, (WORD)  -79,		// phase 42
	 (WORD)  626, (WORD)  -69,		// phase 44
	 (WORD)  544, (WORD)  -58,		// phase 46
	 (WORD)  464, (WORD)  -48,		// phase 48
	 (WORD)  388, (WORD)  -38,		// phase 50
	 (WORD)  316, (WORD)  -29,		// phase 52
	 (WORD)  248, (WORD)  -21,		// phase 54
	 (WORD)  186, (WORD)  -14,		// phase 56
	 (WORD)  129, (WORD)   -8,		// phase 58
	 (WORD)   79, (WORD)   -4,		// phase 60
	 (WORD)   36, (WORD)   -1,	// phase 62
};

// Bi-linear (2-tap), 32-phase for interpolation filter coefficients
ROMDATA WORD MISP_MFCFilter_Bilinear_4T_32P[2*32]		= {
	(WORD) 2048, (WORD)    0,		// phase 00
	 (WORD) 1984, (WORD)    0,		// phase 02
	 (WORD) 1920, (WORD)    0,		// phase 04
	 (WORD) 1856, (WORD)    0,		// phase 06
	 (WORD) 1792, (WORD)    0,		// phase 08
	 (WORD) 1728, (WORD)    0,		// phase 10
	 (WORD) 1664, (WORD)    0,		// phase 12
	 (WORD) 1600, (WORD)    0,		// phase 14
	 (WORD) 1536, (WORD)    0,		// phase 16
	 (WORD) 1472, (WORD)    0,		// phase 18
	 (WORD) 1408, (WORD)    0,		// phase 20
	 (WORD) 1344, (WORD)    0,		// phase 22
	 (WORD) 1280, (WORD)    0,		// phase 24
	 (WORD) 1216, (WORD)    0,		// phase 26
	 (WORD) 1152, (WORD)    0,		// phase 28
	 (WORD) 1088, (WORD)    0,		// phase 30
	 (WORD) 1024, (WORD)    0,		// phase 32
	 (WORD)  960, (WORD)    0,		// phase 34
	 (WORD)  896, (WORD)    0,		// phase 36
	 (WORD)  832, (WORD)    0,		// phase 38
	 (WORD)  768, (WORD)    0,		// phase 40
	 (WORD)  704, (WORD)    0,		// phase 42
	 (WORD)  640, (WORD)    0,		// phase 44
	 (WORD)  576, (WORD)    0,		// phase 46
	 (WORD)  512, (WORD)    0,		// phase 48
	 (WORD)  448, (WORD)    0,		// phase 50
	 (WORD)  384, (WORD)    0,		// phase 52
	 (WORD)  320, (WORD)    0,		// phase 54
	 (WORD)  256, (WORD)    0,		// phase 56
	 (WORD)  192, (WORD)    0,		// phase 58
	 (WORD)  128, (WORD)    0,		// phase 60
	 (WORD)   64, (WORD)    0,	// phase 62
};

// Spline, B=0.50, C=0.00, 4-tap, 32-phase for interpolation filter coefficients
ROMDATA WORD MISP_MFCFilter_BSpline_More_Blur_4T_32P[2*32]	= {
	(WORD) 1707, (WORD)  171,		// phase 00
	 (WORD) 1703, (WORD)  140,		// phase 02
	 (WORD) 1693, (WORD)  113,		// phase 04
	 (WORD) 1677, (WORD)   88,		// phase 06
	 (WORD) 1655, (WORD)   65,		// phase 08
	 (WORD) 1627, (WORD)   46,		// phase 10
	 (WORD) 1594, (WORD)   28,		// phase 12
	 (WORD) 1556, (WORD)   13,		// phase 14
	 (WORD) 1515, (WORD)    0,		// phase 16
	 (WORD) 1469, (WORD)  -11,		// phase 18
	 (WORD) 1419, (WORD)  -20,		// phase 20
	 (WORD) 1366, (WORD)  -27,		// phase 22
	 (WORD) 1311, (WORD)  -33,		// phase 24
	 (WORD) 1253, (WORD)  -38,		// phase 26
	 (WORD) 1192, (WORD)  -40,		// phase 28
	 (WORD) 1130, (WORD)  -42,		// phase 30
	 (WORD) 1067, (WORD)  -43,		// phase 32
	 (WORD) 1002, (WORD)  -42,		// phase 34
	 (WORD)  937, (WORD)  -41,		// phase 36
	 (WORD)  872, (WORD)  -39,		// phase 38
	 (WORD)  807, (WORD)  -36,		// phase 40
	 (WORD)  742, (WORD)  -33,		// phase 42
	 (WORD)  678, (WORD)  -29,		// phase 44
	 (WORD)  615, (WORD)  -25,		// phase 46
	 (WORD)  555, (WORD)  -21,		// phase 48
	 (WORD)  496, (WORD)  -17,		// phase 50
	 (WORD)  439, (WORD)  -13,		// phase 52
	 (WORD)  385, (WORD)  -10,		// phase 54
	 (WORD)  335, (WORD)   -7,		// phase 56
	 (WORD)  287, (WORD)   -4,		// phase 58
	 (WORD)  244, (WORD)   -2,		// phase 60
	 (WORD)  205, (WORD)    0,	// phase 62
};

// Spline, B=1.00, C=0.00, 4-tap, 32-phase for interpolation filter coefficients
ROMDATA WORD MISP_MFCFilter_BSpline_Most_Blur_4T_32P[2*32]	= {
	(WORD) 1365, (WORD)  341,		// phase 00
	 (WORD) 1364, (WORD)  310,		// phase 02
	 (WORD) 1358, (WORD)  281,		// phase 04
	 (WORD) 1348, (WORD)  254,		// phase 06
	 (WORD) 1335, (WORD)  229,		// phase 08
	 (WORD) 1319, (WORD)  205,		// phase 10
	 (WORD) 1300, (WORD)  183,		// phase 12
	 (WORD) 1278, (WORD)  163,		// phase 14
	 (WORD) 1253, (WORD)  144,		// phase 16
	 (WORD) 1226, (WORD)  127,		// phase 18
	 (WORD) 1197, (WORD)  111,		// phase 20
	 (WORD) 1165, (WORD)   96,		// phase 22
	 (WORD) 1131, (WORD)   83,		// phase 24
	 (WORD) 1096, (WORD)   71,		// phase 26
	 (WORD) 1059, (WORD)   61,		// phase 28
	 (WORD) 1021, (WORD)   51,		// phase 30
	 (WORD)  981, (WORD)   43,		// phase 32
	 (WORD)  941, (WORD)   35,		// phase 34
	 (WORD)  900, (WORD)   29,		// phase 36
	 (WORD)  858, (WORD)   23,		// phase 38
	 (WORD)  815, (WORD)   18,		// phase 40
	 (WORD)  773, (WORD)   14,		// phase 42
	 (WORD)  730, (WORD)   10,		// phase 44
	 (WORD)  687, (WORD)    8,		// phase 46
	 (WORD)  645, (WORD)    5,		// phase 48
	 (WORD)  604, (WORD)    3,		// phase 50
	 (WORD)  563, (WORD)    2,		// phase 52
	 (WORD)  523, (WORD)    1,		// phase 54
	 (WORD)  483, (WORD)    1,		// phase 56
	 (WORD)  446, (WORD)    0,		// phase 58
	 (WORD)  409, (WORD)    0,		// phase 60
	 (WORD)  374, (WORD)    0, 	// phase 62
};

// Lanczos, S=0.50, 4-tap, 32-phase for interpolation filter coefficients
ROMDATA WORD MISP_MFCFilter_Lanczos_0_50_4T_32P[2*32]		= {
	(WORD) 1136, (WORD)  456,		// phase 00
	 (WORD) 1136, (WORD)  427,		// phase 02
	 (WORD) 1133, (WORD)  399,		// phase 04
	 (WORD) 1129, (WORD)  372,		// phase 06
	 (WORD) 1123, (WORD)  346,		// phase 08
	 (WORD) 1115, (WORD)  320,		// phase 10
	 (WORD) 1106, (WORD)  295,		// phase 12
	 (WORD) 1095, (WORD)  271,		// phase 14
	 (WORD) 1082, (WORD)  248,		// phase 16
	 (WORD) 1068, (WORD)  225,		// phase 18
	 (WORD) 1052, (WORD)  204,		// phase 20
	 (WORD) 1034, (WORD)  184,		// phase 22
	 (WORD) 1015, (WORD)  165,		// phase 24
	 (WORD)  994, (WORD)  147,		// phase 26
	 (WORD)  972, (WORD)  130,		// phase 28
	 (WORD)  949, (WORD)  114,		// phase 30
	 (WORD)  925, (WORD)   99,		// phase 32
	 (WORD)  899, (WORD)   86,		// phase 34
	 (WORD)  872, (WORD)   74,		// phase 36
	 (WORD)  845, (WORD)   62,		// phase 38
	 (WORD)  816, (WORD)   52,		// phase 40
	 (WORD)  788, (WORD)   42,		// phase 42
	 (WORD)  758, (WORD)   34,		// phase 44
	 (WORD)  728, (WORD)   27,		// phase 46
	 (WORD)  698, (WORD)   20,		// phase 48
	 (WORD)  667, (WORD)   15,		// phase 50
	 (WORD)  636, (WORD)   11,		// phase 52
	 (WORD)  606, (WORD)    7,		// phase 54
	 (WORD)  575, (WORD)    4,		// phase 56
	 (WORD)  545, (WORD)    2,		// phase 58
	 (WORD)  515, (WORD)    1,		// phase 60
	 (WORD)  485, (WORD)    0,		// phase 62
};


// BPF for vertical peaking filter coefficients
ROMDATA WORD MISP_MFCFilter_VPF_BPF50_8T_32P[4*32]	= {
// Lanczos, sharpness = 0.18120, shift=0.50, scale=1024, sum=0, 8-tap, 32-phase	
	  // (WORD)1026, (WORD)     2, (WORD)  -518, (WORD)    2,	// phase 00
      // (WORD)1024,  (WORD)  -41,  (WORD) -506,  (WORD)   10,	// phase 02
      // (WORD)1020,  (WORD)  -83,  (WORD) -492,  (WORD)   17,	// phase 04
      // (WORD)1012,  (WORD) -123,  (WORD) -478,  (WORD)   23,	// phase 06
      // (WORD)1002,  (WORD) -162,  (WORD) -462,  (WORD)   29,	// phase 08
      // (WORD) 990,  (WORD) -200,  (WORD) -446,  (WORD)   34,	// phase 10
      // (WORD) 975,  (WORD) -237,  (WORD) -429,  (WORD)   38,	// phase 12
      // (WORD) 957,  (WORD) -272,  (WORD) -411,  (WORD)   41,	// phase 14
      // (WORD) 937,  (WORD) -305,  (WORD) -393,  (WORD)   44,	// phase 16
      // (WORD) 914,  (WORD) -336,  (WORD) -374,  (WORD)   46,	// phase 18
      // (WORD) 889,  (WORD) -366,  (WORD) -355,  (WORD)   48,	// phase 20
      // (WORD) 862,  (WORD) -394,  (WORD) -336,  (WORD)   48,	// phase 22
      // (WORD) 832,  (WORD) -420,  (WORD) -316,  (WORD)   49,	// phase 24
      // (WORD) 801,  (WORD) -443,  (WORD) -297,  (WORD)   48,	// phase 26
      // (WORD) 767,  (WORD) -465,  (WORD) -277,  (WORD)   48,	// phase 28
      // (WORD) 732,  (WORD) -485,  (WORD) -257,  (WORD)   47,	// phase 30
      // (WORD) 695,  (WORD) -502,  (WORD) -238,  (WORD)   45,	// phase 32
      // (WORD) 657,  (WORD) -518,  (WORD) -219,  (WORD)   43,	// phase 34
      // (WORD) 617,  (WORD) -531,  (WORD) -200,  (WORD)   41,	// phase 36
      // (WORD) 576,  (WORD) -543,  (WORD) -181,  (WORD)   39,	// phase 38
      // (WORD) 534,  (WORD) -552,  (WORD) -163,  (WORD)   36,	// phase 40
      // (WORD) 491,  (WORD) -559,  (WORD) -146,  (WORD)   34,	// phase 42
      // (WORD) 447,  (WORD) -564,  (WORD) -129,  (WORD)   31,	// phase 44
      // (WORD) 403,  (WORD) -568,  (WORD) -112,  (WORD)   27,	// phase 46
      // (WORD) 358,  (WORD) -569,  (WORD)  -97,  (WORD)   25,	// phase 48
      // (WORD) 313,  (WORD) -568,  (WORD)  -82,  (WORD)   22,	// phase 50
      // (WORD) 268,  (WORD) -566,  (WORD)  -67,  (WORD)   18,	// phase 52
      // (WORD) 223,  (WORD) -562,  (WORD)  -54,  (WORD)   15,	// phase 54
      // (WORD) 178,  (WORD) -556,  (WORD)  -41,  (WORD)   12,	// phase 56
      // (WORD) 133,  (WORD) -549,  (WORD)  -29,  (WORD)   10,	// phase 58
      // (WORD)  89,  (WORD) -540,  (WORD)  -18,  (WORD)    7,	// phase 60
      // (WORD)  45,  (WORD) -530,  (WORD)   -7,  (WORD)    5,	// phase 62

// Lanczos, sharpness = 0.18120, shift=0.50, scale=2020, sum=0, 8-tap, 32-phase	
	  (WORD)2024,  (WORD)    6,  (WORD)-1022,  (WORD)    4,	// phase 00
      (WORD)2020,  (WORD)  -80,  (WORD) -998,  (WORD)   20,	// phase 02
      (WORD)2011,  (WORD) -163,  (WORD) -971,  (WORD)   34,	// phase 04
      (WORD)1997,  (WORD) -243,  (WORD) -942,  (WORD)   47,	// phase 06
      (WORD)1978,  (WORD) -320,  (WORD) -912,  (WORD)   57,	// phase 08
      (WORD)1953,  (WORD) -395,  (WORD) -880,  (WORD)   67,	// phase 10
      (WORD)1923,  (WORD) -467,  (WORD) -846,  (WORD)   75,	// phase 12
      (WORD)1888,  (WORD) -536,  (WORD) -811,  (WORD)   82,	// phase 14
      (WORD)1848,  (WORD) -601,  (WORD) -775,  (WORD)   87,	// phase 16
      (WORD)1803,  (WORD) -663,  (WORD) -738,  (WORD)   91,	// phase 18
      (WORD)1753,  (WORD) -722,  (WORD) -701,  (WORD)   94,	// phase 20
      (WORD)1699,  (WORD) -777,  (WORD) -662,  (WORD)   95,	// phase 22
      (WORD)1641,  (WORD) -827,  (WORD) -624,  (WORD)   96,	// phase 24
      (WORD)1579,  (WORD) -874,  (WORD) -585,  (WORD)   96,	// phase 26
      (WORD)1513,  (WORD) -917,  (WORD) -546,  (WORD)   94,	// phase 28
      (WORD)1444,  (WORD) -956,  (WORD) -508,  (WORD)   92,	// phase 30
      (WORD)1371,  (WORD) -991,  (WORD) -469,  (WORD)   89,	// phase 32
      (WORD)1296,  (WORD)-1022,  (WORD) -431,  (WORD)   85,	// phase 34
      (WORD)1217,  (WORD)-1048,  (WORD) -394,  (WORD)   81,	// phase 36
      (WORD)1136,  (WORD)-1071,  (WORD) -358,  (WORD)   77,	// phase 38
      (WORD)1054,  (WORD)-1089,  (WORD) -322,  (WORD)   71,	// phase 40
      (WORD) 969,  (WORD)-1103,  (WORD) -287,  (WORD)   66,	// phase 42
      (WORD) 883,  (WORD)-1113,  (WORD) -254,  (WORD)   60,	// phase 44
      (WORD) 795,  (WORD)-1120,  (WORD) -222,  (WORD)   54,	// phase 46
      (WORD) 707,  (WORD)-1123,  (WORD) -191,  (WORD)   48,	// phase 48
      (WORD) 618,  (WORD)-1122,  (WORD) -161,  (WORD)   42,	// phase 50
      (WORD) 529,  (WORD)-1117,  (WORD) -133,  (WORD)   36,	// phase 52
      (WORD) 439,  (WORD)-1109,  (WORD) -106,  (WORD)   31,	// phase 54
      (WORD) 350,  (WORD)-1097,  (WORD)  -81,  (WORD)   25,	// phase 56
      (WORD) 262,  (WORD)-1083,  (WORD)  -57,  (WORD)   19,	// phase 58
      (WORD) 175,  (WORD)-1065,  (WORD)  -35,  (WORD)   14,	// phase 60
      (WORD)  89,  (WORD)-1045,  (WORD)  -15,  (WORD)   9,	// phase 62	 
};

ROMDATA WORD MISP_MFCFilter_VPF_BPF40_8T_32P[4*32]	= {
// Lanczos, sharpness = 0.2, shift=0.4, scale=2020, sum=0, 8-tap, 32-phase		
	 // (WORD) 1895,  (WORD) 401,  (WORD) -913,  (WORD)-373, // phase 00
	(WORD)1900,  (WORD) 363,   (WORD)-923,  (WORD)-390,  //phase00
	(WORD)1893,  (WORD) 331,   (WORD)-915,  (WORD)-353,  //phase02
	(WORD)1887,  (WORD) 263,   (WORD)-914,  (WORD)-335,  //phase04
	(WORD)1877,  (WORD) 195,   (WORD)-912,  (WORD)-317,  //phase06
	(WORD)1864,  (WORD) 128,   (WORD)-907,  (WORD)-300,  //phase08
	(WORD)1847,  (WORD)   62,   (WORD)-901,  (WORD)-283,  //phase10
	(WORD)1826,  (WORD)   -2,   (WORD)-893,  (WORD)-268,  //phase12
	(WORD)1802,  (WORD)  -65,  (WORD)-883,  (WORD)-253,  //phase14
	(WORD)1774,  (WORD)-127,  (WORD)-871,  (WORD)-239,  //phase16
	(WORD)1743,  (WORD)-186,  (WORD)-858,  (WORD)-226,  //phase18
	(WORD)1709,  (WORD)-244,  (WORD)-844,  (WORD)-215,  //phase20
	(WORD)1671,  (WORD)-300,  (WORD)-828,  (WORD)-203,  //phase22
	(WORD)1630,  (WORD)-354,  (WORD)-811,  (WORD)-193,  //phase24
	(WORD)1587,  (WORD)-406,  (WORD)-793,  (WORD)-183,  //phase26
	(WORD)1540,  (WORD)-456,  (WORD)-773,  (WORD)-174,  //phase28
	(WORD)1491,  (WORD)-503,  (WORD)-753,  (WORD)-166,  //phase30
	(WORD)1439,  (WORD)-548,  (WORD)-732,  (WORD)-159,  //phase32
	(WORD)1385,  (WORD)-590,  (WORD)-711,  (WORD)-153,  //phase34
	(WORD)1329,  (WORD)-630,  (WORD)-689,  (WORD)-147,  //phase36
	(WORD)1270,  (WORD)-667,  (WORD)-666,  (WORD)-142,  //phase38
	(WORD)1210,  (WORD)-701,  (WORD)-643,  (WORD)-138,  //phase40
	(WORD)1147,  (WORD)-733,  (WORD)-620,  (WORD)-134,  //phase42
	(WORD)1084,  (WORD)-763,  (WORD)-596,  (WORD)-131,  //phase44
	(WORD)1019,  (WORD)-789,  (WORD)-573,  (WORD)-129,  //phase46
	(WORD)  952,  (WORD)-813,  (WORD)-549,  (WORD)-127,  //phase48
	(WORD)  885,  (WORD)-835,  (WORD)-526,  (WORD)-125,  //phase50
	(WORD)  817,  (WORD)-853,  (WORD)-503,  (WORD)-124,  //phase52
	(WORD)  748,  (WORD)-869,  (WORD)-480,  (WORD)-124,  //phase54
	(WORD)  679,  (WORD)-883,  (WORD)-458,  (WORD)-123,  //phase56
	(WORD)  609,  (WORD)-894,  (WORD)-435,  (WORD)-123,  //phase58
	(WORD)  539,  (WORD)-902,  (WORD)-414,  (WORD)-124,  //phase60
	(WORD)  470,  (WORD)-909,  (WORD)-393,  (WORD)-124,  //phase62
 };

ROMDATA WORD MISP_MFCFilter_VPF_BPF_6T_32P[4*32]	= {
	  (WORD)1020,  (WORD)    0,  (WORD) -510,  (WORD)    0,	// phase 00
      (WORD) 988,  (WORD)  -16,  (WORD) -494,  (WORD)    0,	// phase 02
      (WORD) 956,  (WORD)  -32,  (WORD) -478,  (WORD)    0,	// phase 04
      (WORD) 924,  (WORD)  -48,  (WORD) -462,  (WORD)    0,	// phase 06
      (WORD) 893,  (WORD)  -64,  (WORD) -446,  (WORD)    0,	// phase 08
      (WORD) 861,  (WORD)  -80,  (WORD) -430,  (WORD)    0,	// phase 10
      (WORD) 829,  (WORD)  -96,  (WORD) -414,  (WORD)    0,	// phase 12
      (WORD) 797,  (WORD) -112,  (WORD) -398,  (WORD)    0,	// phase 14
      (WORD) 765,  (WORD) -128,  (WORD) -383,  (WORD)    0,	// phase 16
      (WORD) 733,  (WORD) -143,  (WORD) -367,  (WORD)    0,	// phase 18
      (WORD) 701,  (WORD) -159,  (WORD) -351,  (WORD)    0,	// phase 20
      (WORD) 669,  (WORD) -175,  (WORD) -335,  (WORD)    0,	// phase 22
      (WORD) 638,  (WORD) -191,  (WORD) -319,  (WORD)    0,	// phase 24
      (WORD) 606,  (WORD) -207,  (WORD) -303,  (WORD)    0,	// phase 26
      (WORD) 574,  (WORD) -223,  (WORD) -287,  (WORD)    0,	// phase 28
      (WORD) 542,  (WORD) -239,  (WORD) -271,  (WORD)    0,	// phase 30
      (WORD) 510,  (WORD) -255,  (WORD) -255,  (WORD)    0,	// phase 32
      (WORD) 478,  (WORD) -271,  (WORD) -239,  (WORD)    0,	// phase 34
      (WORD) 446,  (WORD) -287,  (WORD) -223,  (WORD)    0,	// phase 36
      (WORD) 414,  (WORD) -303,  (WORD) -207,  (WORD)    0,	// phase 38
      (WORD) 383,  (WORD) -319,  (WORD) -191,  (WORD)    0,	// phase 40
      (WORD) 351,  (WORD) -335,  (WORD) -175,  (WORD)    0,	// phase 42
      (WORD) 319,  (WORD) -351,  (WORD) -159,  (WORD)    0,	// phase 44
      (WORD) 287,  (WORD) -367,  (WORD) -143,  (WORD)    0,	// phase 46
      (WORD) 255,  (WORD) -383,  (WORD) -128,  (WORD)    0,	// phase 48
      (WORD) 223,  (WORD) -398,  (WORD) -112,  (WORD)    0,	// phase 50
      (WORD) 191,  (WORD) -414,  (WORD)  -96,  (WORD)    0,	// phase 52
      (WORD) 159,  (WORD) -430,  (WORD)  -80,  (WORD)    0,	// phase 54
      (WORD) 128,  (WORD) -446,  (WORD)  -64,  (WORD)    0,	// phase 56
      (WORD)  96,  (WORD) -462,  (WORD)  -48,  (WORD)    0,	// phase 58
      (WORD)  64,  (WORD) -478,  (WORD)  -32,  (WORD)    0,	// phase 60
      (WORD)  32,  (WORD) -494,  (WORD)  -16,  (WORD)    0,	// phase 62	  
};

#endif	// MDIN-i51X (i510, i540, i550)

#if defined(__USE_MDIN_i500__)		// MDIN-i500
ROMDATA MISP_AUXFILT_COEF MISP_AuxFilter_Default[]	= {	
	{{0x0068, 0x0042, 0x000a, 0x0000, 0x0000},					// HY[5]
	{0x0080, 0x0042, 0x0000, 0x0000},							// HC[4]
	{0x0046, 0x003c, 0x0021},									// VY[3]
	{0x0082, 0x003f}},											// VC[2]
};
#else								// MDIN-i51X (i510, i540, i550)
ROMDATA MISP_AUXFILT_COEF MISP_AuxFilter_Default[]	= {
	// 3840 to 2048 ~ 3840 ==>  bypass
	{{0x0100, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},	// HY[8] (Y=15-tap, C=7-tap)
	{0x0100, 0x0000, 0x0000, 0x0000},									// HC[4]
	{0x0100, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},	// VY[8] N/A
	{0x0100, 0x0000, 0x0000}},									        // VC[3] N/A

	// 3840 to 1024 ~ 2048 ==> cut0.563
    {{0x0091, 0x0050, 0x03F0, 0x03EA, 0x000B, 0x0004, 0x03FE, 0x0000},	// HY[8] (Y=15-tap, C=7-tap)
	{0x089, 0x003E, 0x03FD, 0x0000},									// HC[4]
	{0x0091, 0x0050, 0x03F0, 0x03EA, 0x000D, 0x0000, 0x0000, 0x0000},	// VY[5] (Y=5-tap, C=3-tap)
	{0x089, 0x003B, 0x0000}},											// VC[2]
	
	// 3840 to 720 ~ 1024  ==> cut0.375
    {{0x0060, 0x0049, 0x001B, 0x03F7, 0x03F2, 0x03FC, 0x0004, 0x0003},	// HY[8] (Y=15-tap, C=7-tap)
	{0x0054, 0x0046, 0x0010, 0x0000},									// HC[4]
	{0x0060, 0x0049, 0x001B, 0x03F7, 0x03F2, 0x03FC, 0x0004, 0x0003},	// VY[8] (Y=15-tap, C=5-tap)
	{0x0054, 0x0046, 0x0010}},											// VC[3]

	// 3840 to 640 ~ 720   ==> cut0.333
    {{0x0054, 0x0047, 0x0023, 0x03FF, 0x03EF, 0x03F4, 0x0001, 0x0009},	// HY[8] (Y=15-tap, C=7-tap)
	{0x054, 0x0046, 0x0010, 0x0000},									// HC[4]
	{0x0054, 0x0047, 0x0023, 0x03FF, 0x03EF, 0x03F4, 0x0001, 0x0009},	// VY[8] (Y=15-tap, C=5-tap)
	{0x054, 0x0046, 0x0010}},											// VC[3]

	// 3840 to 480 ~ 640   ==> cut0.250
    {{0x0040, 0x0038, 0x0025, 0x0010, 0x0000, 0x03FA, 0x03FB, 0x03FE},	// HY[8] (Y=15-tap, C=7-tap)
	{0x003E, 0x0036, 0x0020, 0x00B},									// HC[4]
	{0x0040, 0x0038, 0x0025, 0x0010, 0x0000, 0x03FA, 0x03FB, 0x03FE},	// VY[8] (Y=15-tap, C=5-tap)
	{0x003E, 0x0039, 0x0028}}											// VC[3]
	
	// OLD
	// {{0x006c, 0x004f, 0x0014, 0x0fed, 0x0fee, 0x0004, 0x0008, 0x0000},	// HY[8]
	// {0x0080, 0x0040, 0x0000, 0x0000},									// HC[4]
	// {0x006c, 0x004f, 0x0014, 0x0fed, 0x0fee, 0x0004, 0x0008, 0x0000},	// VY[8]
	// {0x0046, 0x003c, 0x0021}},											// VC[3]	
};
#endif

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
	
// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//------------------------------------------------------------------------------------------
static void Init_FC_PARAM( PFC_CTRL_PARAM pCTRL )
{
	// main
	stFCCTRL.stMFC_m.stSRC.w = GetIN_MainSrcSizeH();
	stFCCTRL.stMFC_m.stSRC.h = GetIN_MainSrcSizeV();
	stFCCTRL.stMFC_m.stDST.w = GetOUT_MainSizeHA();
	stFCCTRL.stMFC_m.stDST.h = GetOUT_MainSizeVA();
	stFCCTRL.stMFC_m.overscan_h = 0;
	stFCCTRL.stMFC_m.overscan_v = 0;
#if defined(__SEAMLESS_FC_CTRL__)
	stFCCTRL.stMFC_m.seamless=1;
#endif

	// aux
	stFCCTRL.stMFC_x.stSRC.w = GetIN_AuxSrcSizeH();
	stFCCTRL.stMFC_x.stSRC.h = GetIN_AuxSrcSizeV();
	stFCCTRL.stMFC_x.stDST.w = GetOUT_AuxSizeHA();
	stFCCTRL.stMFC_x.stDST.h = GetOUT_AuxSizeVA();
	stFCCTRL.stMFC_x.aspect = 0;						// 0(full), 1(4:3)
#if defined(__SEAMLESS_FC_CTRL__)
	stFCCTRL.stMFC_x.seamless=1;
#endif

	// main filter
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	stFCCTRL.stFLT_m.FltSel_HY=MISP_MFCCOEF_SPLN_0_50;
	stFCCTRL.stFLT_m.FltSel_HC=MISP_MFCCOEF_SPLN_0_50;
	stFCCTRL.stFLT_m.FltSel_VY=MISP_MFCCOEF_SPLN_0_50;
	stFCCTRL.stFLT_m.FltSel_VC=MISP_MFCCOEF_SPLN_0_50;
	stFCCTRL.stFLT_m.FltSel_VP=MISP_MFCCOEF_MOST_BLUR;
	stFCCTRL.stFLT_m.VPFcor_value=15;		stFCCTRL.stFLT_m.VPFcor_en=ON;	// 
	stFCCTRL.stFLT_m.VPFsat_value=1;		stFCCTRL.stFLT_m.VPFsat_en=ON;	// 
	stFCCTRL.stFLT_m.VPFlevel=76;			

	stFCCTRL.stFLT_m.v_peak_en=ON; // mfc filter(VPF) level, on/off
#else                           // MDIN-i51X (i510, i540, i550)
	stFCCTRL.stFLT_m.FltSel_HY=MISP_MFCCOEF_SPLN_0_50;
	stFCCTRL.stFLT_m.FltSel_HC=MISP_MFCCOEF_SPLN_0_50;
	stFCCTRL.stFLT_m.FltSel_VY=MISP_MFCCOEF_SPLN_0_50;
	stFCCTRL.stFLT_m.FltSel_VC=MISP_MFCCOEF_SPLN_0_50;
	
	//stFCCTRL.stFLT_m.FltSel_VP=MISP_MFCCOEF_VPF_BPF50_8;
	stFCCTRL.stFLT_m.FltSel_VP=MISP_MFCCOEF_VPF_BPF40_8;
	//stFCCTRL.stFLT_m.FltSel_VP=MISP_MFCCOEF_VPF_BPF_6;
	
	stFCCTRL.stFLT_m.VPFcor_value=4;		stFCCTRL.stFLT_m.VPFcor_en=ON;	// 
	stFCCTRL.stFLT_m.VPFsat_value=3;		stFCCTRL.stFLT_m.VPFsat_en=OFF;	// 
	// stFCCTRL.stFLT_m.VPFlevel=20;			
	stFCCTRL.stFLT_m.VPFlevel=60;			
	stFCCTRL.stFLT_m.VPFinverse=OFF;	// OFF:VPF=BPF or HPF, ON:VPF=LPF
	stFCCTRL.stFLT_m.VPFcor_half_en=OFF;
	stFCCTRL.stFLT_m.level_depend_en=ON;
	stFCCTRL.stFLT_m.level_depend_th=127;
	stFCCTRL.stFLT_m.level_depend_slope=30;
	stFCCTRL.stFLT_m.level_depend_offset=0;
	#if defined(__USE_MDIN_i5XX_REV__)
		stFCCTRL.stFLT_m.v_peak_en=ON; // mfc filter(VPF) level, on/off
	#else
		stFCCTRL.stFLT_m.v_peak_en=OFF; // mfc filter(VPF) level, on/off
	#endif
#endif
	stFCCTRL.stFLT_m.h_peak_en=OFF;	

	//aux filter
	// stFCCTRL.stFLT_x.pCoef=(PMISP_AUXFILT_COEF)&MISP_AuxFilter_Default;
	// stFCCTRL.stFLT_x.Coef=0; // pointer link to table ID
	// stFCCTRL.stFLT_x.hflt_y=ON;
	// stFCCTRL.stFLT_x.hflt_c=ON;
	// stFCCTRL.stFLT_x.vflt_y=ON;
	// stFCCTRL.stFLT_x.vflt_c=ON;
	SetFC_AuxFilterHY(ON);
	SetFC_AuxFilterHC(ON);
	SetFC_AuxFilterVY(ON);
	SetFC_AuxFilterVC(ON);

#if defined(__USE_SENS_PANNING_MODE__) && defined(__USE_CMOS_IMX274__)
	//see also Demo_Set_OutFHD_Crop() in demo.c
	stFCCTRL.stMFC_m.stSRC.x = 64;	// 1920+128=2048--> 0x007 in_size_h_main, 128/2 = 64
	stFCCTRL.stMFC_m.stSRC.y = 64; 	// 1080+128=1208--> 0x008 in_size_v_main,, 128/2 = 64
	stFCCTRL.stMFC_m.stSRC.w = 1920;	  
	stFCCTRL.stMFC_m.stSRC.h = 1080;	  	
	
	stFCCTRL.stMFC_m.stDST.x = 0;
	stFCCTRL.stMFC_m.stDST.y = 0;
	stFCCTRL.stMFC_m.stDST.w = 1920;
	stFCCTRL.stMFC_m.stDST.h = 1080;
#endif	

#if defined(__MN34420_1280x1024_ISPCROP__)
	stFCCTRL.stMFC_m.stSRC.x = 32;	
	stFCCTRL.stMFC_m.stSRC.y = 32; 	
	stFCCTRL.stMFC_m.stSRC.w = 1280;	  
	stFCCTRL.stMFC_m.stSRC.h = 1024;	  

	stFCCTRL.stMFC_m.stDST.x = 0;
	stFCCTRL.stMFC_m.stDST.y = 0;
	stFCCTRL.stMFC_m.stDST.w = 1280;
	stFCCTRL.stMFC_m.stDST.h = 1024;
#endif
	
}

//---------------------------------------------------------------------------------------------
static void FCCTRL_SetMainFCsize(PMISP_MFCSCALE_INFO pINFO)
{
	PWORD pDstW;
	BYTE ffc_en, out656_en=0;
	MISP_VIDEO_WINDOW fc_src, fc_dst;
	WORD pixel_pack32, in_wr_posi_h, in_wr_posi_v;
	
	// mirror hsize is limited to multiple of 8
	pDstW=(PWORD)&pINFO->stDST.w;
	if(pINFO->mirror&0x01)  *pDstW=pINFO->stDST.w&0xfff8;

	//ffc
	ffc_en=(pINFO->stSRC.w > pINFO->stDST.w)?1:0;
	if(ffc_en) {  // horizontal scale down (use front fc)
		MISP_RegWrite(MISP_LOCAL_ID, 0x03A, pINFO->stSRC.w);		// src_size_h2
		MISP_RegWrite(MISP_LOCAL_ID, 0x03B, pINFO->stSRC.h);		// src_size_v2
		MISP_RegWrite(MISP_LOCAL_ID, 0x03C, pINFO->stDST.w);		// dest_size_h2
		MISP_RegWrite(MISP_LOCAL_ID, 0x03D, GetIN_MainSrcSizeV());	// dest_size_v2 - main video size(v) in frame buffer
		MISP_RegField(MISP_LOCAL_ID, 0x040, 2, 1, 0);				// bypass_main
	}
	else {
		MISP_RegWrite(MISP_LOCAL_ID, 0x03A, pINFO->stSRC.w);		// src_size_h2
		MISP_RegWrite(MISP_LOCAL_ID, 0x03B, pINFO->stSRC.h);		// src_size_v2
		MISP_RegWrite(MISP_LOCAL_ID, 0x03C, pINFO->stSRC.w);		// dest_size_h2
		MISP_RegWrite(MISP_LOCAL_ID, 0x03D, GetIN_MainSrcSizeV());	// dest_size_v2 - main video size(v) in frame buffer
		MISP_RegField(MISP_LOCAL_ID, 0x040, 2, 1, 1);				// bypass_main
	}

	//mfc source
	fc_src.x=pINFO->stSRC.x;
	fc_src.y=pINFO->stSRC.y;
	fc_src.w=(ffc_en)?pINFO->stDST.w:pINFO->stSRC.w;
	fc_src.h=pINFO->stSRC.h;
	fc_src.w -= (fc_src.w > pINFO->overscan_h)? pINFO->overscan_h: 0;
	fc_src.h -= (fc_src.h > pINFO->overscan_v)? pINFO->overscan_v: 0;
	MISP_RegWrite(MISP_LOCAL_ID, 0x032, fc_src.w);	// source_x_size
	MISP_RegWrite(MISP_LOCAL_ID, 0x033, fc_src.h);	// source_y_size
	MISP_RegWrite(MISP_LOCAL_ID, 0x034, fc_src.x);	// source_x_posi
	MISP_RegWrite(MISP_LOCAL_ID, 0x035, fc_src.y);	// source_y_posi
	MISP_RegWrite(MISP_LOCAL_ID, 0x108, fc_src.w);	// source_x_size_cr
	MISP_RegWrite(MISP_LOCAL_ID, 0x109, fc_src.h);	// source_y_size_cr
	MISP_RegWrite(MISP_LOCAL_ID, 0x10C, fc_src.w);	// source_x_size_or
	MISP_RegWrite(MISP_LOCAL_ID, 0x10D, fc_src.h);	// source_y_size_or

	//mfc target
	if(GetOUT_MainMode()==MISP_OUT_MUX656_10) out656_en=1;
	if(GetOUT_MainMode()==MISP_OUT_MUX656_8)  out656_en=1;
	
	fc_dst.x=pINFO->stDST.x;
	fc_dst.y=pINFO->stDST.y;
	fc_dst.w=out656_en?(pINFO->stDST.w*2):pINFO->stDST.w;
	fc_dst.h=pINFO->stDST.h;
	MISP_RegWrite(MISP_LOCAL_ID, 0x10A, fc_dst.w);	// dest_x_size_cr
	MISP_RegWrite(MISP_LOCAL_ID, 0x10B, fc_dst.h);	// dest_y_size_cr
	MISP_RegWrite(MISP_LOCAL_ID, 0x10E, fc_dst.w);	// dest_x_size_or
	MISP_RegWrite(MISP_LOCAL_ID, 0x10F, fc_dst.h);	// dest_y_size_or
	MISP_RegWrite(MISP_LOCAL_ID, 0x036, fc_dst.w);	// dest_x_size_sg
	MISP_RegWrite(MISP_LOCAL_ID, 0x037, fc_dst.h);	// dest_y_size_sg
	MISP_RegWrite(MISP_LOCAL_ID, 0x038, fc_dst.x);	// dest_x_posi_sg
	MISP_RegWrite(MISP_LOCAL_ID, 0x039, fc_dst.y);	// dest_y_posi_sg

	//mirror
	fc_src.w=(ffc_en)?pINFO->stDST.w:pINFO->stSRC.w;
	fc_src.h=pINFO->stSRC.h;
	pixel_pack32 = GetIN_MainBitmode()?(6<<5):(8<<5);
//	in_wr_posi_h = fc_dst.w-pixel_pack32;
	in_wr_posi_h = fc_src.w-pixel_pack32;
	in_wr_posi_v = fc_src.h-1;
	MISP_RegField(MISP_LOCAL_ID, 0x01c, 0, 13, in_wr_posi_h);
 	MISP_RegField(MISP_LOCAL_ID, 0x01d, 0, 12, in_wr_posi_v);
	MISP_RegField(MISP_LOCAL_ID, 0x01c, 15, 1, (pINFO->mirror&0x01)?1:0);  // in_mirror_h_en
	MISP_RegField(MISP_LOCAL_ID, 0x01c, 14, 1, (pINFO->mirror&0x02)?1:0);  // in_mirror_v_en
	//mirror_444			<= input_slv_regs(16#01C#)(13);	-- i51x, added 20151201 by thorn
	
	//load trigger
	if(pINFO->seamless) {
#if defined(__USE_MDIN_i500__) 				// MDIN-i500
		MISP_RegField(MISP_LOCAL_ID, 0x040, 9, 1, 0);	// 'load_size_regs'
		MISP_RegField(MISP_LOCAL_ID, 0x040, 9, 1, 1);	// 'load_size_regs'
#else 										// MDIN-i51X (i510, i540, i550)
	//	MISP_RegField(MISP_LOCAL_ID, 0x03E,14, 2, 3);	// trigger update size (15:input, 14:output)
		MISP_RegField(MISP_LOCAL_ID, 0x03E,15, 1, 1);	// trigger update size (input)
		MISP_RegField(MISP_LOCAL_ID, 0x03E,14, 1, 1);	// trigger update size (output)
#endif
	}
}


//---------------------------------------------------------------------------------------------
static void FCCTRL_SetMainFCctrl(PMISP_MFCSCALE_INFO pINFO)
{
	WORD 	mfc_itp_mode_vy=2, mfc_itp_mode_vc=2, mfc_itp_mode_hy=2, mfc_itp_mode_hc=2;
	BOOL	v_move_down, mfc_prepend_load, mfc_itp_linear, mfc_itp_flt_dis;
	
//NOTE: mfc_itp_mode_vy/vc has no meaning if mfc_itp_linear='1'
// MFC interpolation filters can be up to 4taps both for Y & C on interlaced input
// while they can be up to 8taps for Y & 4taps for C on progressive input.
	if(GetIN_MainScanTypeProg()) {		//progressive
#if defined(__USE_MDIN_i500__) 						// MDIN-i500
		mfc_itp_linear=0;	mfc_prepend_load=0;	/*mfc_itp_mode_vc=2;	mfc_itp_mode_vy=2,*/	v_move_down=0;
#else 	// MDIN-i51X (i510, i540, i550)
	#if defined(__USE_MDIN_i5XX_REV__) 	
		mfc_itp_linear=0;	mfc_prepend_load=0;	/*mfc_itp_mode_vc=2;	mfc_itp_mode_vy=2,*/	v_move_down=0;
	#else
		mfc_itp_linear=1;	mfc_prepend_load=0;	/*mfc_itp_mode_vc=2;	mfc_itp_mode_vy=2,*/	v_move_down=0;
	#endif
#endif
	}
	else	{							//interlace
		mfc_itp_linear=1;	mfc_prepend_load=1;	/*mfc_itp_mode_vc=3;	mfc_itp_mode_vy=3,*/	v_move_down=1;
	}
	mfc_itp_flt_dis=0;
	
	MISP_RegField(MISP_LOCAL_ID, 0x100,  1,  1, mfc_itp_linear);	// 0:use itp flt coff.  1:2D linear itp
	MISP_RegField(MISP_LOCAL_ID, 0x100,  2,  1, mfc_itp_flt_dis);	// 0:filter enable, 1:disable
	MISP_RegField(MISP_LOCAL_ID, 0x100,  6,  1, mfc_prepend_load);	// 0:prog. in  1:interlace in
	MISP_RegField(MISP_LOCAL_ID, 0x100,  8,  8, (mfc_itp_mode_hy<<6)|(mfc_itp_mode_hc<<4)|(mfc_itp_mode_vy<<2)|mfc_itp_mode_vc);	// filter mode sel
	MISP_RegField(MISP_LOCAL_ID, 0x101, 15,  1, v_move_down);		// 1:1-line shift down for prevent i-input bot garbage line

	//seamless
	if(pINFO->seamless) {
#if defined(__USE_MDIN_i500__) 					// MDIN-i500
		MISP_RegField(MISP_LOCAL_ID, 0x107, 1, 1, 1);  // set 'mfc_load_size_en'
		MISP_RegField(MISP_LOCAL_ID, 0x200,15, 1, 1);  // set 'ipc_load_size_en'
#else 											// MDIN-i51X (i510, i540, i550)
		MISP_RegField(MISP_LOCAL_ID, 0x107, 0, 2, 3);  // main fc (0x032~0x033, 0x108~0x10F) : 0-input, 1-output
		MISP_RegField(MISP_LOCAL_ID, 0x03E, 0, 2, 3);  // input sync (0x036~0x039, 0x03A~0x03C) : 0-input, 1-output
		MISP_RegField(MISP_LOCAL_ID, 0x200,15, 1, 1);  // ipc (0x032~0x035, 0x03D)
#endif
	}
	else {
#if defined(__USE_MDIN_i500__) 					// MDIN-i500
		MISP_RegField(MISP_LOCAL_ID, 0x107, 1, 1, 0);  // set 'mfc_load_size_en'
		MISP_RegField(MISP_LOCAL_ID, 0x200,15, 1, 0);  // set 'ipc_load_size_en'
#else 											// MDIN-i51X (i510, i540, i550)
		MISP_RegField(MISP_LOCAL_ID, 0x107, 0, 2, 0);  // main fc (0x032~0x033, 0x108~0x10F) : 0-input, 1-output
		MISP_RegField(MISP_LOCAL_ID, 0x03E, 0, 2, 0);  // input sync (0x036~0x039, 0x03A~0x03C) : 0-input, 1-output
		MISP_RegField(MISP_LOCAL_ID, 0x200,15, 1, 0);  // ipc (0x032~0x035, 0x03D)
#endif
	}
}

//------------------------------------------------------------------------------------
static void FCCTRL_SetMainVPF(PMISP_MFCFILT_INFO pFLT)
{
	//set VPF
	MISP_RegField(MISP_LOCAL_ID, 0x130, 8, 8, pFLT->VPFlevel);		// 0~64(100%)
	// MISP_RegWrite(MISP_LOCAL_ID, 0x12F, (pFLT->VPFcor_value<<12)|(pFLT->VPFsat_value<<8)|(pFLT->VPFcor_en)<<2|(pFLT->VPFsat_en<<1)|(pFLT->VPFen));		// 0:bypass VPF, 1:enable VPF
	MISP_RegField(MISP_LOCAL_ID, 0x12F, 1, 15, (pFLT->VPFcor_value<<11)|(pFLT->VPFsat_value<<7)|(pFLT->VPFcor_en)<<1|(pFLT->VPFsat_en));		// 0:bypass VPF, 1:enable VPF
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	MISP_RegField(MISP_LOCAL_ID, 0x130, 0, 1, pFLT->VPFinverse);		// 0:VPF=non-LPF, 1:VPF=LPF
	MISP_RegField(MISP_LOCAL_ID, 0x12F, 3, 1, pFLT->VPFcor_half_en);
	MISP_RegField(MISP_LOCAL_ID, 0x130, 2, 1, pFLT->level_depend_en);
	
	MISP_RegWrite(MISP_LOCAL_ID, 0x131, (pFLT->level_depend_slope));
	MISP_RegWrite(MISP_LOCAL_ID, 0x132, (pFLT->level_depend_th<<8)|(pFLT->level_depend_offset));
#endif

	//set peaking
#if defined(__USE_EXT2IPP__)
	MISP_RegField(MISP_LOCAL_ID, 0x128, 0, 1, 0);
	MISP_RegField(MISP_LOCAL_ID, 0x12F, 0, 1, 0);
#else
	MISP_RegField(MISP_LOCAL_ID, 0x128, 0, 1, pFLT->h_peak_en);
	MISP_RegField(MISP_LOCAL_ID, 0x12F, 0, 1, pFLT->v_peak_en);
#endif
}

//------------------------------------------------------------------------------------
static void FCCTRL_SetMainFLTctrl(PMISP_MFCFILT_INFO pFLT)
{
#if defined (__USE_MDIN_i510__)||defined (__USE_MDIN_i540__)||defined (__USE_MDIN_i550__)

	WORD main_src_size_h = GetFC_MainFCsrcSizeH() ;
	WORD main_dst_size_h = GetFC_MainFCdstSizeH();
	BYTE hscale_down;
	WORD scale_ratio ; // integer scale_ratio 
	
	DWORD main_dst_size_h_mult ;
	
	BYTE out656_en=0;
	
	if(GetOUT_MainMode()==MISP_OUT_MUX656_10) out656_en=1;
	if(GetOUT_MainMode()==MISP_OUT_MUX656_8)  out656_en=1;
	
	main_dst_size_h /= (out656_en)?  2:  1;
	
	hscale_down = (main_src_size_h > main_dst_size_h )?1:0;
	
	if ( hscale_down == 1 )
	{
		main_dst_size_h_mult = main_dst_size_h<<11 ; // x2048
		scale_ratio = main_dst_size_h_mult/main_src_size_h ;
	}
	else
	{
		main_dst_size_h_mult = 0 ;
		scale_ratio = 2048 ;
	}
	
	if      ( scale_ratio > 1092 ) { SetIn_MainFnrCoef(0); } // 2050~3840 : 1025~1920
	else if ( scale_ratio > 1023 ) { SetIn_MainFnrCoef(1); } // 1919~2049 :  960~1024
	else if ( scale_ratio >  682 ) { SetIn_MainFnrCoef(2); } // 1281~1918 :   641~959
	else if ( scale_ratio >  512 ) { SetIn_MainFnrCoef(3); } //  960~1280 :   481~640
	else                           { SetIn_MainFnrCoef(4); } //     1~961 :     1~480

#endif //#if defined (__USE_MDIN_i510__)||defined (__USE_MDIN_i540__)||defined (__USE_MDIN_i550__)
}
//--------------------------------------------------------------------------------------------------------------------------
static void FCCTRL_WaitMainFilterCoefDone(void)
{
	WORD rVal = 0, count = 100;
	
	Delay_uSec(50);
	
	while (count&&(rVal==0)) {
		MISP_RegRead(MISP_LOCAL_ID, 0x13f, &rVal);
		rVal &= 0x40;	count--;
	}

#if __MISP100_DBGPRT__ == 1
	if (count==0) UARTprintf("FCCTRL_GetMainFilterDone() TimeOut Error!!!\r\n");
#endif
}

//---------------------------------------------------------------------------------------------
static void FCCTRL_SetMainFLTcoef(MISP_MFC_FILTER_t flt_sel, BYTE coef_sel)
{
	PMISP_MFCFILT_COEF pCoef;
	WORD i, buff[5] = {0,0,0,0,0};
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	BYTE num_taps;
#endif
	
#if defined(__USE_MDIN_i500__) 		// MDIN-i500
	switch(coef_sel) {
		case MISP_MFCCOEF_SPLN_1_00:  pCoef=(PMISP_MFCFILT_COEF)&MISP_MFCFilter_Spline_1_00;        break;
		case MISP_MFCCOEF_SPLN_0_75:  pCoef=(PMISP_MFCFILT_COEF)&MISP_MFCFilter_Spline_0_75;        break;
		case MISP_MFCCOEF_SPLN_0_50:  pCoef=(PMISP_MFCFILT_COEF)&MISP_MFCFilter_Spline_0_50;        break;
		case MISP_MFCCOEF_BI_LINEAR:  pCoef=(PMISP_MFCFILT_COEF)&MISP_MFCFilter_Bilinear;           break;
		case MISP_MFCCOEF_MOST_BLUR:  pCoef=(PMISP_MFCFILT_COEF)&MISP_MFCFilter_BSpline_Most_Blur;  break;
		case MISP_MFCCOEF_MORE_BLUR:  pCoef=(PMISP_MFCFILT_COEF)&MISP_MFCFilter_BSpline_More_Blur;  break;
		default:  return;
	}
#else 								// MDIN-i51X (i510, i540, i550)
	switch(coef_sel) {
		case MISP_MFCCOEF_SPLN_1_00:  pCoef=(PMISP_MFCFILT_COEF)&MISP_MFCFilter_Spline_1_00_4T_32P;        num_taps=4;  break;
		case MISP_MFCCOEF_SPLN_0_75:  pCoef=(PMISP_MFCFILT_COEF)&MISP_MFCFilter_Spline_0_75_4T_32P;        num_taps=4;  break;
		case MISP_MFCCOEF_SPLN_0_50:  pCoef=(PMISP_MFCFILT_COEF)&MISP_MFCFilter_Spline_0_50_4T_32P;        num_taps=4;  break;
		case MISP_MFCCOEF_BI_LINEAR:  pCoef=(PMISP_MFCFILT_COEF)&MISP_MFCFilter_Bilinear_4T_32P;           num_taps=4;  break;
		case MISP_MFCCOEF_MOST_BLUR:  pCoef=(PMISP_MFCFILT_COEF)&MISP_MFCFilter_BSpline_Most_Blur_4T_32P;  num_taps=4;  break;
		case MISP_MFCCOEF_MORE_BLUR:  pCoef=(PMISP_MFCFILT_COEF)&MISP_MFCFilter_BSpline_More_Blur_4T_32P;  num_taps=4;  break;
		case MISP_MFCCOEF_LANC_0_50:  pCoef=(PMISP_MFCFILT_COEF)&MISP_MFCFilter_Lanczos_0_50_4T_32P;       num_taps=4;  break;
		case MISP_MFCCOEF_VPF_BPF50_8:  pCoef=(PMISP_MFCFILT_COEF)&MISP_MFCFilter_VPF_BPF50_8T_32P;            num_taps=8;  break;
		case MISP_MFCCOEF_VPF_BPF40_8:  pCoef=(PMISP_MFCFILT_COEF)&MISP_MFCFilter_VPF_BPF40_8T_32P;            num_taps=8;  break;
		case MISP_MFCCOEF_VPF_BPF_6:  pCoef=(PMISP_MFCFILT_COEF)&MISP_MFCFilter_VPF_BPF_6T_32P;            num_taps=8;  break;
		default:  return;
	}
#endif
	
#if defined(__USE_MDIN_i500__)		// MDIN-i500
	for (i=0; i<sizeof(MISP_MFCFILT_COEF)/4; i++) {			// for 2-word write
		memcpy(buff, ((PWORD)pCoef)+i*2, 4);	buff[4] = MAKEWORD(flt_sel,(0x80|i));
		MISP_MultiWrite(MISP_LOCAL_ID, 0x13b, (PBYTE)buff, 10);
		FCCTRL_WaitMainFilterCoefDone();								// check coef r/w done flag
	}
#else                           // MDIN-i51X (i510, i540, i550)
	for (i=0; i<32; i++) {			// 32:lower 32-phase only, 64:lower&upper or 64-phase		
		memcpy(buff, pCoef, num_taps);	buff[4] = MAKEWORD(flt_sel,(0x80|i));
		pCoef += num_taps/sizeof(WORD);	//to next phase		
		MISP_MultiWrite(MISP_LOCAL_ID, 0x13b, (PBYTE)buff, 10);
		FCCTRL_WaitMainFilterCoefDone();								// check coef r/w done flag
	}
#endif
}

//------------------------------------------------------------------------------------
static void FCCTRL_SetAuxFCsize(PMISP_MFCSCALE_INFO pINFO)
{
	BYTE out656_en=0, out_i;
	PWORD pDstW;
	WORD aux_dst_size_h, aux_dst_size_v, mem_hsize, mem_vsize;
	WORD pixel_pack32, in_wr_posi_h, in_wr_posi_v;
	WORD crop_w=0, off_x=0;

	// mirror hsize is limited to multiple of 8
	pDstW=(PWORD)&pINFO->stDST.w;
	if(pINFO->mirror&0x01)  *pDstW=pINFO->stDST.w&0xfff8;

	if((!GetOUT_AuxPipEN()) && (pINFO->aspect==1))	{
		crop_w = pINFO->stSRC.w * 25 / 100;
		off_x = crop_w / 2;
	}
	
	//aux source
	MISP_RegWrite(MISP_LOCAL_ID, 0x160, pINFO->stSRC.x + off_x);	// aux_src_x_posi
	MISP_RegWrite(MISP_LOCAL_ID, 0x161, pINFO->stSRC.y);			// aux_src_y_posi
	MISP_RegWrite(MISP_LOCAL_ID, 0x162, pINFO->stSRC.w - crop_w);	// aux src_x_size
	MISP_RegWrite(MISP_LOCAL_ID, 0x163, pINFO->stSRC.h);			// aux src_y_size

	//aux target
	MISP_RegWrite(MISP_LOCAL_ID, 0x168, pINFO->stDST.w);	// aux dst_x_size
	MISP_RegWrite(MISP_LOCAL_ID, 0x169, pINFO->stDST.h);	// aux dst_y_size
	MISP_RegWrite(MISP_LOCAL_ID, 0x16A, pINFO->stDST.x);	// aux dst_x_posi
	MISP_RegWrite(MISP_LOCAL_ID, 0x16B, pINFO->stDST.y);	// aux dst_y_posi
	MISP_RegWrite(MISP_LOCAL_ID, 0x16C, pINFO->stDST.w);	// aux win_x_size
	MISP_RegWrite(MISP_LOCAL_ID, 0x16D, pINFO->stDST.h);	// aux win_y_size

	//aux mem (use src_size if scale-up, else dst_size)
	if(GetOUT_AuxMode()==MISP_OUT_MUX656_10) out656_en=1;
	if(GetOUT_AuxMode()==MISP_OUT_MUX656_8)  out656_en=1;
	out_i = GetOUT_AuxScanTypeProg()?  0:  1;
	aux_dst_size_h = pINFO->stDST.w / (out656_en?  2:  1);
	aux_dst_size_v = pINFO->stDST.h / (out_i?  2:  1);		// interace out : v/2
	mem_hsize=(pINFO->stSRC.w<aux_dst_size_h)?pINFO->stSRC.w:aux_dst_size_h;
	mem_vsize=(pINFO->stSRC.h<aux_dst_size_v)?pINFO->stSRC.h:pINFO->stDST.h;
//	MISP_RegWrite(MISP_LOCAL_ID, 0x164, 0);					// aux mem_x_posi
//	MISP_RegWrite(MISP_LOCAL_ID, 0x165, 0);					// aux mem_y_posi
	MISP_RegWrite(MISP_LOCAL_ID, 0x166, mem_hsize);			// aux_mem_x_size
	MISP_RegWrite(MISP_LOCAL_ID, 0x167, mem_vsize);			// aux_mem_y_size

	//aux_rd_frm2fld controls
	if((GetIN_AuxScanTypeProg() && out_i) && (pINFO->stSRC.h >= pINFO->stDST.h))	// p -> i && src >= dst
		MISP_RegField(MISP_LOCAL_ID, 0x142, 13, 1, 1);
	else if((!GetIN_AuxScanTypeProg() && out_i) && (pINFO->stSRC.h >= pINFO->stDST.h*2))	// i -> i && src >= dst *2
		MISP_RegField(MISP_LOCAL_ID, 0x142, 13, 1, 1);
	else
		MISP_RegField(MISP_LOCAL_ID, 0x142, 13, 1, 0);

	//mirror
	pixel_pack32 = GetIN_AuxBitmode()?(6<<5):(8<<5);
//	in_wr_posi_h = pINFO->stDST.w-pixel_pack32;
	in_wr_posi_h = mem_hsize-pixel_pack32;
	in_wr_posi_v = mem_vsize-1;
	MISP_RegField(MISP_LOCAL_ID, 0x148, 0, 13, in_wr_posi_h);			// aux_wr_posi_h[12:0]
	MISP_RegField(MISP_LOCAL_ID, 0x149, 0, 12, in_wr_posi_v);			// aux_wr_posi_v[11:0]
	MISP_RegField(MISP_LOCAL_ID, 0x148,13,  1, (in_wr_posi_v>>12)&1);	// aux_wr_posi_v[12]
	if(GetIN_AuxPath() != MISP_AUXPATH_MAIN_O)	{
		MISP_RegField(MISP_LOCAL_ID, 0x148, 15, 1, (pINFO->mirror&0x01)?1:0);  // aux_mirror_h_en
		MISP_RegField(MISP_LOCAL_ID, 0x148, 14, 1, (pINFO->mirror&0x02)?1:0);  // aux_mirror_v_en
	}
	else	{
		MISP_RegField(MISP_LOCAL_ID, 0x148, 15, 1, 0);  // aux_mirror_h_en
		MISP_RegField(MISP_LOCAL_ID, 0x148, 14, 1, 0);  // aux_mirror_v_en
	}
	//aux_mirror_444	<= slv_regs(16#147#)(15);	-- i51x, added 20151201 by thorn

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	//load trigger
	if(pINFO->seamless) {
	//	MISP_RegField(MISP_LOCAL_ID, 0x146,14, 2, 3);	// trigger update size (15:input, 14:output)
		MISP_RegField(MISP_LOCAL_ID, 0x146,15, 1, 1);	// trigger update size (input)
		MISP_RegField(MISP_LOCAL_ID, 0x146,14, 1, 1);	// trigger update size (output)
	}
#endif
}

//------------------------------------------------------------------------------------
static void FCCTRL_SetAuxFCctrl(PMISP_MFCSCALE_INFO pINFO)
{
	BOOL	aux_hfc_bypass, aux_hfc2_bypass, aux_vfc_bypass, aux_vfc2_bypass;
	WORD	aux_dst_size_h, aux_src_size_v, aux_dst_size_v;
	BYTE	aux_hfc_mode, aux_vfc_mode, aux_hfc_load_dly;
	
	BYTE out656_en=0;
	if(GetOUT_AuxMode()==MISP_OUT_MUX656_10) out656_en=1;
	if(GetOUT_AuxMode()==MISP_OUT_MUX656_8)  out656_en=1;

	aux_dst_size_h = pINFO->stDST.w / (out656_en?  2:  1);
	if(pINFO->stSRC.w > aux_dst_size_h)			aux_hfc_mode=0;	//scale down
	else if(pINFO->stSRC.w < aux_dst_size_h)	aux_hfc_mode=1;	//scale up
	else										aux_hfc_mode=2;	//no scale
	
	aux_src_size_v = pINFO->stSRC.h / (GetIN_AuxScanTypeProg()? 1: 2);		//interlace : v/2
	aux_dst_size_v = pINFO->stDST.h / (GetOUT_AuxScanTypeProg()? 1: 2);		//interlace : v/2
	
	if(aux_src_size_v > aux_dst_size_v)			aux_vfc_mode=0;	//scale down
	else if(aux_src_size_v < aux_dst_size_v)	aux_vfc_mode=1;	//scale up
	else										aux_vfc_mode=2;	//no scale

	switch(aux_hfc_mode)	{
		case  0:  aux_hfc_bypass=out656_en?0:1;  aux_hfc2_bypass=0;  break;  //scale down
		case  1:  aux_hfc_bypass=0;              aux_hfc2_bypass=1;  break;  //scale up
		default:  aux_hfc_bypass=out656_en?0:1;	 aux_hfc2_bypass=1;  break;  //no scale
	}
	
	switch(aux_vfc_mode)	{
		case  0:  aux_vfc_bypass=0;  aux_vfc2_bypass=0;  aux_vfc_mode=0;  aux_hfc_load_dly=4;  break;  //scale down
		case  1:  aux_vfc_bypass=0;  aux_vfc2_bypass=1;  aux_vfc_mode=1;  aux_hfc_load_dly=0;  break;  //scale up
		default:  aux_vfc_bypass=1;  aux_vfc2_bypass=1;  aux_vfc_mode=0;  aux_hfc_load_dly=4;  break;  //no scale
	}

	if(aux_vfc_mode==1)	{												// scale up : use 'clk_aux_out'
		switch(GetCLK_SrcAuxOut())	{
			case 0:	SetCLK_SrcAuxlm(2);	break;		// vclk
			case 1:	SetCLK_SrcAuxlm(1);	break;		// venc_clk
			case 2:	SetCLK_SrcAuxlm(0);	break;		// clk_m_aux
			case 3:	SetCLK_SrcAuxlm(3);	break;		// aux_mclk
		}
	}
	else if(GetIN_AuxPath()==MISP_AUXPATH_MAIN_O)	SetCLK_SrcAuxlm(2);	// scale down : vclk(2) -- main_o -> aux
	else											SetCLK_SrcAuxlm(0);	// scale down : clk_m_aux(0) -- sensor -> aux

	MISP_RegField(MISP_LOCAL_ID, 0x142, 11,  1, (aux_vfc_mode==2)?0:1);  // aux_linemem_en
	MISP_RegField(MISP_LOCAL_ID, 0x142,  3,  4, (aux_hfc_bypass<<3)|(aux_vfc_bypass<<2)|(aux_hfc2_bypass<<1)|aux_vfc2_bypass);
	MISP_RegField(MISP_LOCAL_ID, 0x142,  7,  4, (aux_vfc_mode<<3)|aux_hfc_load_dly);

	//seamless
#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
	if(pINFO->seamless)
		MISP_RegField(MISP_LOCAL_ID, 0x146, 0, 3, 3);  // aux (0x160~0x16D) : 0-input, 1-output, 2-vfc
	else
		MISP_RegField(MISP_LOCAL_ID, 0x146, 0, 3, 0);  // aux (0x160~0x16D) : 0-input, 1-output, 2-vfc
#endif
}


//------------------------------------------------------------------------------------
static void FCCTRL_SetAuxFLTcoef(PMISP_AUXFILT_INFO pFLT)
{
	PMISP_AUXFILT_COEF pCoef;
	
	pCoef = (PMISP_AUXFILT_COEF)&MISP_AuxFilter_Default[pFLT->Coef]; // pointer link to table ID

#if defined(__USE_MDIN_i500__)		// MDIN-i500
	// MISP_MultiWrite(MISP_LOCAL_ID, 0x151, (PBYTE)pFLT->pCoef, sizeof(MISP_AUXFILT_COEF));
	MISP_MultiWrite(MISP_LOCAL_ID, 0x151, (PBYTE)pCoef, sizeof(MISP_AUXFILT_COEF)); // pointer link to table ID
#else								// MDIN-i51X (i510, i540, i550)
	// MISP_MultiWrite(MISP_LOCAL_ID, 0x151, (PBYTE)pFLT->pCoef, sizeof(MISP_AuxFilter_Default.hy)+sizeof(MISP_AuxFilter_Default.hc));
	// MISP_MultiWrite(MISP_LOCAL_ID, 0x290, (PBYTE)pFLT->pCoef->vy, sizeof(MISP_AuxFilter_Default.vy)+sizeof(MISP_AuxFilter_Default.vc));
	MISP_MultiWrite(MISP_LOCAL_ID, 0x151, (PBYTE)pCoef, sizeof(MISP_AuxFilter_Default[pFLT->Coef].hy)+sizeof(MISP_AuxFilter_Default[pFLT->Coef].hc)); // pointer link to table ID
	MISP_MultiWrite(MISP_LOCAL_ID, 0x290, (PBYTE)pCoef->vy, sizeof(MISP_AuxFilter_Default[pFLT->Coef].vy)+sizeof(MISP_AuxFilter_Default[pFLT->Coef].vc)); // pointer link to table ID
#endif
}

//------------------------------------------------------------------------------------
static void FCCTRL_SetAuxFLTctrl(PMISP_AUXFILT_INFO pFLT)
{
	BYTE vflt_en, aux_linemem_cfg;
	WORD aux_src_size_h = GetFC_AuxFCsrcSizeH() / (GetIN_AuxScanTypeProg()? 1: 2);		//interlace : v/2
	WORD aux_src_size_v = GetFC_AuxFCsrcSizeV() / (GetIN_AuxScanTypeProg()? 1: 2);		//interlace : v/2
	WORD aux_dst_size_h = GetFC_AuxFCdstSizeH();
	BYTE vscale_down = (aux_src_size_v > GetFC_AuxFCdstSizeV())?1:0;
	WORD scale_ratio ; // integer scale_ratio 
	
	DWORD aux_dst_size_h_mult ;
	
	BYTE out656_en=0;
	
	if(GetOUT_AuxMode()==MISP_OUT_MUX656_10) out656_en=1;
	if(GetOUT_AuxMode()==MISP_OUT_MUX656_8)  out656_en=1;
	
	aux_dst_size_h /= (out656_en)?  2:  1;
	
#if defined (__USE_MDIN_i500__) 			// MDIN-i500
	//Aux line memory configuration for vertical format conversion
	//'00': 2048 pixels per line, the vertical filters should be disabled.
	//'01': 1024 pixels per line, the vertical filters (Y=5-tap, C=3-tap) can be enabled.
	//'10': 1024 pixels per line, the vertical filter  (Y=7-tap only) can be enabled.
	vflt_en=((vscale_down)&&(GetFC_AuxFCdstSizeH()<1024))?1:0;
	aux_linemem_cfg=(vflt_en)?1:0;
#else 										// MDIN-i51X (i510, i540, i550)
	//Aux line memory configuration for vertical format conversion
	//'00': 4096 pixels per line, the vertical filters should be disabled.
	//'01': 2048 pixels per line, the vertical filters (Y=5-tap, C=3-tap) can be enabled.
	//'10': 2048 pixels per line, the vertical filter  (Y=7-tap only) can be enabled.
	//'11': 1024 pixels per line, the vertical filters (Y=15-tap, C=5-tap) can be enabled.
	
	// vflt_en=((vscale_down)&&(GetFC_AuxFCdstSizeH()<1024))?1:0;
	// aux_linemem_cfg=(vflt_en)?3:0;
	
	vflt_en=((vscale_down)&&(aux_dst_size_h<=2048))?1:0;

	if ( vscale_down == 1 )
	{	
		aux_dst_size_h_mult = aux_dst_size_h<<11 ;
		scale_ratio = aux_dst_size_h_mult/aux_src_size_h ;
	}
	else
	{
		aux_dst_size_h_mult = 0 ;
		scale_ratio = 2048 ;		
	}
	
	// if ( (aux_dst_size_h > 2048) || (vscale_down == 0) ) { SetFC_AuxFilterCoef(0); aux_linemem_cfg=0; }
	// else if ( aux_dst_size_h > 1024 ) { SetFC_AuxFilterCoef(1); aux_linemem_cfg=1; }
	// else if ( aux_dst_size_h >  720 ) { SetFC_AuxFilterCoef(2); aux_linemem_cfg=3; }
	// else if ( aux_dst_size_h >  640 ) { SetFC_AuxFilterCoef(3); aux_linemem_cfg=3; }
	// else                              { SetFC_AuxFilterCoef(4); aux_linemem_cfg=3; }	

	if ( (aux_dst_size_h > 2048) || (vscale_down == 0) ) { aux_linemem_cfg=0; }
	else if ( aux_dst_size_h > 1024 ) { aux_linemem_cfg=1; }
	// else if ( aux_dst_size_h >  720 ) { aux_linemem_cfg=3; }
	// else if ( aux_dst_size_h >  640 ) { aux_linemem_cfg=3; }
	else                              { aux_linemem_cfg=3; }		
	
	if 		( scale_ratio > 1092 ) { SetFC_AuxFilterCoef(0); } // 2050~3840 : 1025~1920
	else if ( scale_ratio > 1023 ) { SetFC_AuxFilterCoef(1); } // 1919~2049 :  960~1024
	else if ( scale_ratio >  682 ) { SetFC_AuxFilterCoef(2); } // 1281~1918 :   641~959
	else if ( scale_ratio >  512 ) { SetFC_AuxFilterCoef(3); } //  960~1280 :   481~640
	else                           { SetFC_AuxFilterCoef(4); } //     1~961 :     1~480
	
#endif

#if defined (__USE_MDIN_i500__) 			// MDIN-i500
	MISP_RegField(MISP_LOCAL_ID, 0x150, 4, 1, aux_linemem_cfg);       //aux_linemem_cfg
#else 										// MDIN-i51X (i510, i540, i550)
	MISP_RegField(MISP_LOCAL_ID, 0x150, 4, 2, aux_linemem_cfg);       //aux_linemem_cfg
#endif
	MISP_RegField(MISP_LOCAL_ID, 0x150, 3, 1, pFLT->vflt_c&vflt_en);  //aux_vflt_c_on
	MISP_RegField(MISP_LOCAL_ID, 0x150, 2, 1, pFLT->vflt_y&vflt_en);  //aux_vflt_y_on
	MISP_RegField(MISP_LOCAL_ID, 0x150, 1, 1, pFLT->hflt_c);          //aux_hflt_c_on
	MISP_RegField(MISP_LOCAL_ID, 0x150, 0, 1, pFLT->hflt_y);          //aux_hflt_y_on
}

//------------------------------------------------------------------------------------
static void FCCTRL_MainHandler(PFC_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&FCCTRL_UPDATE_MAIN)==0) return;
	pCTRL->ctrl &= (~FCCTRL_UPDATE_MAIN);
	
	FCCTRL_SetMainFCsize(&pCTRL->stMFC_m);
	FCCTRL_SetMainFCctrl(&pCTRL->stMFC_m);
	pCTRL->ctrl |= FCCTRL_UPDATE_MAIN_FLT;  //filter control depends on scale ratio
}

//------------------------------------------------------------------------------------
static void FCCTRL_MainFltHandler(PFC_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&FCCTRL_UPDATE_MAIN_FLT)==0) return;
	pCTRL->ctrl &= (~FCCTRL_UPDATE_MAIN_FLT);
	
	FCCTRL_SetMainFLTctrl(&pCTRL->stFLT_m);
}

//------------------------------------------------------------------------------------
static void FCCTRL_MainVPFHandler(PFC_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&FCCTRL_UPDATE_MAIN_VPF)==0) return;
	pCTRL->ctrl &= (~FCCTRL_UPDATE_MAIN_VPF);
	
	FCCTRL_SetMainVPF(&pCTRL->stFLT_m);
}

//------------------------------------------------------------------------------------
static void FCCTRL_AuxHandler(PFC_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&FCCTRL_UPDATE_AUX)==0) return;
	pCTRL->ctrl &= (~FCCTRL_UPDATE_AUX);
	
	FCCTRL_SetAuxFCsize(&pCTRL->stMFC_x);
	FCCTRL_SetAuxFCctrl(&pCTRL->stMFC_x);
	pCTRL->ctrl |= FCCTRL_UPDATE_AUX_FLT;  //filter control depends on scale ratio
}

//------------------------------------------------------------------------------------
static void FCCTRL_AuxFltHandler(PFC_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&FCCTRL_UPDATE_AUX_FLT)==0) return;
	pCTRL->ctrl &= (~FCCTRL_UPDATE_AUX_FLT);
	
	FCCTRL_SetAuxFLTctrl(&pCTRL->stFLT_x);
}

//------------------------------------------------------------------------------------
static void FCCTRL_MainCoefHandler(PFC_CTRL_PARAM pCTRL)
{
	if (pCTRL->ctrl&FCCTRL_UPDATE_MAIN_COEF_HY) {
		pCTRL->ctrl &= (~FCCTRL_UPDATE_MAIN_COEF_HY);
		FCCTRL_SetMainFLTcoef(MISP_MFCFILT_HY, pCTRL->stFLT_m.FltSel_HY);
	}
	if (pCTRL->ctrl&FCCTRL_UPDATE_MAIN_COEF_HC) {
		pCTRL->ctrl &= (~FCCTRL_UPDATE_MAIN_COEF_HC);
		FCCTRL_SetMainFLTcoef(MISP_MFCFILT_HC, pCTRL->stFLT_m.FltSel_HC);
	}
	if (pCTRL->ctrl&FCCTRL_UPDATE_MAIN_COEF_VY) {
		pCTRL->ctrl &= (~FCCTRL_UPDATE_MAIN_COEF_VY);
		FCCTRL_SetMainFLTcoef(MISP_MFCFILT_VY, pCTRL->stFLT_m.FltSel_VY);
	}
	if (pCTRL->ctrl&FCCTRL_UPDATE_MAIN_COEF_VC) {
		pCTRL->ctrl &= (~FCCTRL_UPDATE_MAIN_COEF_VC);
		FCCTRL_SetMainFLTcoef(MISP_MFCFILT_VC, pCTRL->stFLT_m.FltSel_VC);
	}
	if (pCTRL->ctrl&FCCTRL_UPDATE_MAIN_COEF_VP) {
		pCTRL->ctrl &= (~FCCTRL_UPDATE_MAIN_COEF_VP);
		FCCTRL_SetMainFLTcoef(MISP_MFCFILT_VP, pCTRL->stFLT_m.FltSel_VP);
	}
}

//------------------------------------------------------------------------------------
static void FCCTRL_AuxCoefHandler(PFC_CTRL_PARAM pCTRL)
{
	if ((pCTRL->ctrl&FCCTRL_UPDATE_AUX_COEF)==0) return;
	pCTRL->ctrl &= (~FCCTRL_UPDATE_AUX_COEF);
	
	FCCTRL_SetAuxFLTcoef(&pCTRL->stFLT_x);
}

//--------------------------------------------------------------------------------------
static void Init_FC_REGISTER( PFC_CTRL_PARAM pCTRL ) 
{
	FCCTRL_SetMainFCsize(&pCTRL->stMFC_m);
	FCCTRL_SetMainFCctrl(&pCTRL->stMFC_m);
	FCCTRL_SetMainFLTctrl(&pCTRL->stFLT_m);
	FCCTRL_SetMainVPF(&pCTRL->stFLT_m);
	FCCTRL_SetMainFLTcoef(MISP_MFCFILT_HY, pCTRL->stFLT_m.FltSel_HY);
	FCCTRL_SetMainFLTcoef(MISP_MFCFILT_HC, pCTRL->stFLT_m.FltSel_HC);
	FCCTRL_SetMainFLTcoef(MISP_MFCFILT_VY, pCTRL->stFLT_m.FltSel_VY);
	FCCTRL_SetMainFLTcoef(MISP_MFCFILT_VC, pCTRL->stFLT_m.FltSel_VC);
	FCCTRL_SetMainFLTcoef(MISP_MFCFILT_VP, pCTRL->stFLT_m.FltSel_VP);
	
	FCCTRL_SetAuxFCsize(&pCTRL->stMFC_x);
	FCCTRL_SetAuxFCctrl(&pCTRL->stMFC_x);
	FCCTRL_SetAuxFLTctrl(&pCTRL->stFLT_x);
	FCCTRL_SetAuxFLTcoef(&pCTRL->stFLT_x);

	//convert to api code !!!!
	MISP_RegField(MISP_LOCAL_ID, 0x101, 1, 2, 1);	// mfc_nr_mode (0:NRS only, 1:BNR only, 2:NRS or BNR, 3:NRS and BNR)
	MISP_RegWrite(MISP_LOCAL_ID, 0x147, 0x20b4);	// aux_ptr_ctrl
}


//api
//----------------------------------------------------------------------------------------
void Init_FC(void)
{
	memset(&stFCCTRL, 0, sizeof(FC_CTRL_PARAM));
	Init_FC_PARAM(&stFCCTRL);
	Init_FC_REGISTER(&stFCCTRL);
}

//----------------------------------------------------------------------------------------
void FC_ProcessHandler(void)
{
	if ((stFCCTRL.ctrl&FCCTRL_TRIGGER_ISP)==0) return;
	stFCCTRL.ctrl &= (~FCCTRL_TRIGGER_ISP);
	
	FCCTRL_MainHandler(&stFCCTRL);
	FCCTRL_AuxHandler(&stFCCTRL);
	FCCTRL_MainFltHandler(&stFCCTRL);
	FCCTRL_MainVPFHandler(&stFCCTRL);
	FCCTRL_AuxFltHandler(&stFCCTRL);
	FCCTRL_MainCoefHandler(&stFCCTRL);
	FCCTRL_AuxCoefHandler(&stFCCTRL);
}


//get/set func.

//------------------------------------------------------------------------------------
void SetFC_StopMainFC(BYTE nID)
{ 
	if ( nID == 0 ) // Run Main FC
	{
		MISP_RegField(MISP_LOCAL_ID, 0x200, 2, 1, 1);
		MISP_RegField(MISP_LOCAL_ID, 0x040, 1, 1, 0);	
	}
	else if ( nID == 1 ) // Freeze Main FC only
	{ // Imediately stops Main FC.
		MISP_RegField(MISP_LOCAL_ID, 0x200, 2, 1, 1);
		MISP_RegField(MISP_LOCAL_ID, 0x040, 1, 1, 1);		
	}
	else // Freeze Stop Main FC & IPC
	{ // Imediately stops Main FC & IPC. Display data will turn black
		MISP_RegField(MISP_LOCAL_ID, 0x200, 2, 1, 0);
		MISP_RegField(MISP_LOCAL_ID, 0x040, 1, 1, 1);		
	}	
}

//------------------------------------------------------------------------------------
void SetFC_MainLoadSizeEN(BOOL nID)
{
	stFCCTRL.stMFC_m.seamless = nID;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN | FCCTRL_TRIGGER_ISP);		// main update
}

//------------------------------------------------------------------------------------
void SetFC_MainFCsrcSizeH(WORD val)
{
	stFCCTRL.stMFC_m.stSRC.w = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN | FCCTRL_TRIGGER_ISP);		// main update
}

//------------------------------------------------------------------------------------
void SetFC_MainFCsrcSizeV(WORD val)
{
	stFCCTRL.stMFC_m.stSRC.h = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN | FCCTRL_TRIGGER_ISP);		// main update
}

//------------------------------------------------------------------------------------
void SetFC_MainFCsrcPosiX(WORD val)
{
	stFCCTRL.stMFC_m.stSRC.x = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN | FCCTRL_TRIGGER_ISP);		// main update
}

//------------------------------------------------------------------------------------
void SetFC_MainFCsrcPosiY(WORD val)
{
	stFCCTRL.stMFC_m.stSRC.y = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN | FCCTRL_TRIGGER_ISP);		// main update
}

//------------------------------------------------------------------------------------
void SetFC_MainFCsrcOverscanH(WORD val)
{
	stFCCTRL.stMFC_m.overscan_h = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN | FCCTRL_TRIGGER_ISP);		// main update
}

//------------------------------------------------------------------------------------
void SetFC_MainFCsrcOverscanV(WORD val)
{
	stFCCTRL.stMFC_m.overscan_v = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN | FCCTRL_TRIGGER_ISP);		// main update
}

//------------------------------------------------------------------------------------
void SetFC_MainFCdstSizeH(WORD val)
{
	stFCCTRL.stMFC_m.stDST.w = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN | FCCTRL_TRIGGER_ISP);		// main update
}

//------------------------------------------------------------------------------------
void SetFC_MainFCdstSizeV(WORD val)
{
	stFCCTRL.stMFC_m.stDST.h = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN | FCCTRL_TRIGGER_ISP);		// main update
}

//------------------------------------------------------------------------------------
WORD GetFC_MainFCdstSizeH(void)
{
	return stFCCTRL.stMFC_m.stDST.w;
}

//------------------------------------------------------------------------------------
WORD GetFC_MainFCdstSizeV(void)
{
	return stFCCTRL.stMFC_m.stDST.h;
}

//------------------------------------------------------------------------------------
void SetFC_MainFCdstPosiX(WORD val)
{
	stFCCTRL.stMFC_m.stDST.x = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN | FCCTRL_TRIGGER_ISP);		// main update
}

//------------------------------------------------------------------------------------
void SetFC_MainFCdstPosiY(WORD val)
{
	stFCCTRL.stMFC_m.stDST.y = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN | FCCTRL_TRIGGER_ISP);		// main update
}

//------------------------------------------------------------------------------------
void SetFC_MainVPFLevel(WORD val)
{
	stFCCTRL.stFLT_m.VPFlevel = (BYTE)val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN_VPF | FCCTRL_TRIGGER_ISP);	// filter update
}

//------------------------------------------------------------------------------------
BYTE GetFC_MainVPFLevel(void)
{
	return  stFCCTRL.stFLT_m.VPFlevel;
}

#if defined(__USE_MDIN_i510__) || defined(__USE_MDIN_i540__) || defined(__USE_MDIN_i550__) 	// i510, i540, i550
//------------------------------------------------------------------------------------
void SetFC_MainVPFLDthresh(WORD val)
{
	stFCCTRL.stFLT_m.level_depend_th = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN_VPF | FCCTRL_TRIGGER_ISP);	// filter update
}

void SetFC_MainVPFLDslope(WORD val)
{
	stFCCTRL.stFLT_m.level_depend_slope = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN_VPF | FCCTRL_TRIGGER_ISP);	// filter update
}
#endif

//------------------------------------------------------------------------------------
void SetFC_AuxLoadSizeEN(BOOL nID)
{
	stFCCTRL.stMFC_x.seamless = nID;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_AUX | FCCTRL_TRIGGER_ISP);		// aux update
}

//------------------------------------------------------------------------------------
void SetFC_AuxFCsrcSizeH(WORD val)
{
	stFCCTRL.stMFC_x.stSRC.w = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_AUX | FCCTRL_TRIGGER_ISP);		// aux update
}

//------------------------------------------------------------------------------------
void SetFC_AuxFCsrcSizeV(WORD val)
{
	stFCCTRL.stMFC_x.stSRC.h = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_AUX | FCCTRL_TRIGGER_ISP);		// aux update
}

//------------------------------------------------------------------------------------
void SetFC_AuxFCsrcPosiX(WORD val)
{
	stFCCTRL.stMFC_x.stSRC.x = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_AUX | FCCTRL_TRIGGER_ISP);		// aux update
}

//------------------------------------------------------------------------------------
void SetFC_AuxFCsrcPosiY(WORD val)
{
	stFCCTRL.stMFC_x.stSRC.y = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_AUX | FCCTRL_TRIGGER_ISP);		// aux update
}

//------------------------------------------------------------------------------------
void SetFC_AuxFCdstSizeH(WORD val)
{
	stFCCTRL.stMFC_x.stDST.w = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_AUX | FCCTRL_TRIGGER_ISP);		// aux update
}

//------------------------------------------------------------------------------------
void SetFC_AuxFCdstSizeV(WORD val)
{
	stFCCTRL.stMFC_x.stDST.h = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_AUX | FCCTRL_TRIGGER_ISP);		// aux update
}

//------------------------------------------------------------------------------------
void SetFC_AuxFCdstPosiX(WORD val)
{
	stFCCTRL.stMFC_x.stDST.x = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_AUX | FCCTRL_TRIGGER_ISP);		// aux update
}

//------------------------------------------------------------------------------------
void SetFC_AuxFCdstPosiY(WORD val)
{
	stFCCTRL.stMFC_x.stDST.y = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_AUX | FCCTRL_TRIGGER_ISP);		// aux update
}

//------------------------------------------------------------------------------------
void SetFC_StopAuxFC(WORD nID)
{
	MISP_RegField(MISP_LOCAL_ID, 0x142, 1, 1, (nID)?  1:  0);
}

//------------------------------------------------------------------------------------
WORD GetFC_AuxFCdstSizeH(void)
{
	return stFCCTRL.stMFC_x.stDST.w;
}

//------------------------------------------------------------------------------------
WORD GetFC_AuxFCdstSizeV(void)
{
	return stFCCTRL.stMFC_x.stDST.h;
}


//add by jins 2016.03
//-------------------------------------------------------------------------------------
WORD GetFC_MainFCsrcPosiX(void)
{
	return stFCCTRL.stMFC_m.stSRC.x;
}

//------------------------------------------------------------------------------------
WORD GetFC_MainFCsrcPosiY(void)
{
	return stFCCTRL.stMFC_m.stSRC.y;
}

//------------------------------------------------------------------------------------
WORD GetFC_MainFCsrcSizeH(void)
{
	return stFCCTRL.stMFC_m.stSRC.w;
}

//------------------------------------------------------------------------------------
WORD GetFC_MainFCsrcSizeV(void)
{
	return stFCCTRL.stMFC_m.stSRC.h;
}

//------------------------------------------------------------------------------------
WORD GetFC_MainFCdstPosiX(void)
{
	return stFCCTRL.stMFC_m.stDST.x;
}

//------------------------------------------------------------------------------------
WORD GetFC_MainFCdstPosiY(void)
{
	return stFCCTRL.stMFC_m.stDST.y;
}

//-------------------------------------------------------------------------------------
WORD GetFC_AuxFCsrcPosiX(void)
{
	return stFCCTRL.stMFC_x.stSRC.x;
}

//------------------------------------------------------------------------------------
WORD GetFC_AuxFCsrcPosiY(void)
{
	return stFCCTRL.stMFC_x.stSRC.y;
}

//------------------------------------------------------------------------------------
WORD GetFC_AuxFCsrcSizeH(void)
{
	return stFCCTRL.stMFC_x.stSRC.w;
}

//------------------------------------------------------------------------------------
WORD GetFC_AuxFCsrcSizeV(void)
{
	return stFCCTRL.stMFC_x.stSRC.h;
}

//------------------------------------------------------------------------------------
WORD GetFC_AuxFCdstPosiX(void)
{
	return stFCCTRL.stMFC_x.stDST.x;
}

//------------------------------------------------------------------------------------
WORD GetFC_AuxFCdstPosiY(void)
{
	return stFCCTRL.stMFC_x.stDST.y;
}

//------------------------------------------------------------------------------------
void SetFC_AuxFilterHY(BOOL OnOff)
{
	stFCCTRL.stFLT_x.hflt_y = OnOff;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_AUX_FLT | FCCTRL_TRIGGER_ISP);	// aux_flt update
}

//------------------------------------------------------------------------------------
void SetFC_AuxFilterHC(BOOL OnOff)
{
	stFCCTRL.stFLT_x.hflt_c = OnOff;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_AUX_FLT | FCCTRL_TRIGGER_ISP);	// aux_flt update
}

//------------------------------------------------------------------------------------
void SetFC_AuxFilterVY(BOOL OnOff)
{
	stFCCTRL.stFLT_x.vflt_y = OnOff;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_AUX_FLT | FCCTRL_TRIGGER_ISP);	// aux_flt update
}

//------------------------------------------------------------------------------------
void SetFC_AuxFilterVC(BOOL OnOff)
{
	stFCCTRL.stFLT_x.vflt_c = OnOff;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_AUX_FLT | FCCTRL_TRIGGER_ISP);	// aux_flt update
}

//------------------------------------------------------------------------------------
BOOL GetFC_AuxFilterHY(void)
{
	return stFCCTRL.stFLT_x.hflt_y;
}

//------------------------------------------------------------------------------------
BOOL GetFC_AuxFilterHC(void)
{
	return stFCCTRL.stFLT_x.hflt_c;
}

//------------------------------------------------------------------------------------
BOOL GetFC_AuxFilterVY(void)
{
	return stFCCTRL.stFLT_x.vflt_y;
}

//------------------------------------------------------------------------------------
BOOL GetFC_AuxFilterVC(void)
{
	return stFCCTRL.stFLT_x.vflt_c;
}

//------------------------------------------------------------------------------------
void SetFC_MainFilterCoefHY(BYTE val)
{
	stFCCTRL.stFLT_m.FltSel_HY=val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN_COEF_HY | FCCTRL_TRIGGER_ISP);  //coefficient update
}

//------------------------------------------------------------------------------------
void SetFC_MainFilterCoefHC(BYTE val)
{
	stFCCTRL.stFLT_m.FltSel_HC=val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN_COEF_HC | FCCTRL_TRIGGER_ISP);  //coefficient update
}

//------------------------------------------------------------------------------------
void SetFC_MainFilterCoefVY(BYTE val)
{
	stFCCTRL.stFLT_m.FltSel_VY=val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN_COEF_VY | FCCTRL_TRIGGER_ISP);  //coefficient update
}

//------------------------------------------------------------------------------------
void SetFC_MainFilterCoefVC(BYTE val)
{
	stFCCTRL.stFLT_m.FltSel_VC=val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN_COEF_VC | FCCTRL_TRIGGER_ISP);  //coefficient update
}

//------------------------------------------------------------------------------------
void SetFC_MainFilterCoefVP(BYTE val)
{
	stFCCTRL.stFLT_m.FltSel_VP=val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN_COEF_VP | FCCTRL_TRIGGER_ISP);  //coefficient update
}

//------------------------------------------------------------------------------------
BYTE GetFC_MainFilterCoefHY(void)
{
	return stFCCTRL.stFLT_m.FltSel_HY;
}

//------------------------------------------------------------------------------------
BYTE GetFC_MainFilterCoefHC(void)
{
	return stFCCTRL.stFLT_m.FltSel_HC;
}

//------------------------------------------------------------------------------------
BYTE GetFC_MainFilterCoefVY(void)
{
	return stFCCTRL.stFLT_m.FltSel_VY;
}

//------------------------------------------------------------------------------------
BYTE GetFC_MainFilterCoefVC(void)
{
	return stFCCTRL.stFLT_m.FltSel_VC;
}

//------------------------------------------------------------------------------------
BYTE GetFC_MainFilterCoefVP(void)
{
	return stFCCTRL.stFLT_m.FltSel_VP;
}

//------------------------------------------------------------------------------------
void SetFC_AuxFilterCoef(BYTE val)
{
	stFCCTRL.stFLT_x.Coef = val;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_AUX_COEF | FCCTRL_TRIGGER_ISP);	// filter update
}


//------------------------------------------------------------------------------------
void SetFC_MainHoriPeaking(BOOL OnOff)
{
	stFCCTRL.stFLT_m.h_peak_en = OnOff;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN_VPF | FCCTRL_TRIGGER_ISP);	// filter update
}

//------------------------------------------------------------------------------------
void SetFC_MainVertPeaking(BOOL OnOff)
{
	stFCCTRL.stFLT_m.v_peak_en = OnOff;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN_VPF | FCCTRL_TRIGGER_ISP);	// filter update
}

//------------------------------------------------------------------------------------
BOOL GetFC_MainHoriPeaking(void)
{
	return stFCCTRL.stFLT_m.h_peak_en;
}

//------------------------------------------------------------------------------------
BOOL GetFC_MainVertPeaking(void)
{
	return stFCCTRL.stFLT_m.v_peak_en;
}

//------------------------------------------------------------------------------------
void SetFC_MainMirror(BYTE mode)
{
	stFCCTRL.stMFC_m.mirror = mode;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_MAIN | FCCTRL_TRIGGER_ISP);		// main update
}

//------------------------------------------------------------------------------------
BYTE GetFC_MainMirror(void)
{
	return stFCCTRL.stMFC_m.mirror;
}

//------------------------------------------------------------------------------------
void SetFC_AuxMirror(BYTE mode)
{
	stFCCTRL.stMFC_x.mirror = mode;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_AUX | FCCTRL_TRIGGER_ISP);		// aux update
}

//------------------------------------------------------------------------------------
BYTE GetFC_AuxMirror(void)
{
	return stFCCTRL.stMFC_x.mirror;
}

//------------------------------------------------------------------------------------
BOOL GetFC_MainFFC_OnOff(void)
{
	return	(stFCCTRL.stMFC_m.stSRC.w > stFCCTRL.stMFC_m.stDST.w)?  ON:  OFF;
}

//------------------------------------------------------------------------------------
void SetFC_AuxAspect(BYTE mode)
{
	stFCCTRL.stMFC_x.aspect = mode;
	stFCCTRL.ctrl |= (FCCTRL_UPDATE_AUX | FCCTRL_TRIGGER_ISP);		// aux update
}

/*  FILE_END_HERE */
