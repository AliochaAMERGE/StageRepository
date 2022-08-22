/*************************************************************************

 Copyright (c) 2022 by CNRS/LESIA

 This software is copyrighted by and is the sole property of CNRS/LESIA.
 All rights, title, ownership, or other interests in the software remain
 the property of CNRS/LESIA.
 This software may only be used in accordance with the corresponding
 license agreement. Any unauthorized use, duplication, transmission,
 distribution, or disclosure of this software is expressly forbidden.

 This Copyright notice may not be removed or modified without prior
 written consent of CNRS/LESIA.


 LESIA
 Observatoire de Meudon
 5 place Jules Janssen
 92195 Meudon
 http://www.lesia.obspm.fr/-Logiciels-embarques-.html
 *************************************************************************/

/************************************************************************

 GericosBlocks Component

 GsbIeee754Sin.cpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************
 *
 *  @file GsbIeee754Sin.cpp
 *  @brief Implementation of the GsbIeee754::sin() function.
 *
 *  The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
 *  The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
 *
 *  Changes :
 * 		- All the code has be reworked in order to follow LESIA's Flight Software C++ Coding Standards
 * 		- All variables has been renamed for better understanding.
 ************************************************************************/

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#include <gericos/blocks/math/GsbIeee754.hpp>

/* sin(x)
 * Return cosine function of x.
 *
 * kernel function:
 *	kernelSin		... sine function on [-pi/4,pi/4]
 *	kernelCos		... cosine function on [-pi/4,pi/4]
 *	getRemainderPio2	... argument reduction routine
 *
 * Method.
 *      Let S,C and T denote the sin, cos and tan respectively on
 *	[-PI/4, +PI/4]. Reduce the argument x to y1+y2 = x-k*pi/2
 *	in [-pi/4 , +pi/4], and let n = k mod 4.
 *	We have
 *
 *          n        sin(x)      cos(x)        tan(x)
 *     ----------------------------------------------------------
 *	    0    	       S    	   C    		 T
 *	    1    	       C    	  -S    		-1/T
 *	    2    	      -S    	  -C    		 T
 *	    3    	      -C    	   S    		-1/T
 *     ----------------------------------------------------------
 *
 * Special cases:
 *      Let trig be any of sin, cos, or tan.
 *      trig(+-INF)  is NaN, with signals.
 *      trig(NaN)    is that NaN.
 *
 * Accuracy:
 *	TRIG(x) returns trig(x) nearly rounded
 */

double GsbIeee754::sin(double x) {
	double rem[2];
	int32_t n;
	int32_t highWord;

	double result = 0.0;

	// High word of x.
	highWord = GsbIeee754::getHighWord(x);
	// Remove sign bit
	highWord &= 0x7FFFFFFF;
	if (highWord <= 0x3FE921FB) {
		// |x| ~< pi/4
		if (highWord < 0x3E500000) {
			// |x| < 2**-26
			if (static_cast<int32_t>(x) == 0) {
				// generate inexact
				result = x;
			}
		}
		else {
			result = GsbIeee754::kernelSin(x, 0.0, 0);
		}
	}

	// sin(Inf or NaN) is NaN
	else if (highWord >= 0x7FF00000) {
		result = x - x;
	}

	// argument reduction needed
	else {
		n = GsbIeee754::getRemainderPio2(x, &rem[0]);
		if ((n & 3) == 0) {
			result = GsbIeee754::kernelSin(rem[0], rem[1], 1);
		}
		else if ((n & 3) == 1) {
			result = GsbIeee754::kernelCos(rem[0], rem[1]);
		}
		else if ((n & 3) == 2) {
			result = -GsbIeee754::kernelSin(rem[0], rem[1], 1);
		}
		else {
			result = -GsbIeee754::kernelCos(rem[0], rem[1]);
		}
	}
	return result;
}
