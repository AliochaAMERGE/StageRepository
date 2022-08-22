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

 GsbIeee754KernelSin.cpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************
 *
 *  @file GsbIeee754KernelSin.cpp
 *  @brief Implementation of the GsbIeee754::kernelSin() function
 *
 *  The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
 *  The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
 *
 *  Changes :
 * 		Variable names change.
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

/* kernelSin(x, xTail, xTailIsZero)
 * kernel sin function on [-pi/4, pi/4], pi/4 ~ 0.7854
 * Input x is assumed to be bounded by ~pi/4 in magnitude.
 * Input xTail is the tail of x.
 * Input xTailIsZero indicates whether xTail is 0. (if xTailIsZero=0, xTail assume to be 0).
 *
 * Algorithm
 *	1. Since sin(-x) = -sin(x), we need only to consider positive x.
 *	2. if x < 2^-27 (hx < 0x3e400000 0), return x with inexact if x!=0.
 *	3. sin(x) is approximated by a polynomial of degree 13 on
 *	   [0,pi/4]
 *	   	sin(x) ~ x + S1*x^3 + ... + S6*x^13
 *	   where
 *
 * 	| (sin(x) / x) - ( 1 + S1*x^2 + S2*x^4 + S3*x^6 + S4*x^8 + S5*x^10 + S6*x^12) | <= 2^-58
 *
 *	4. sin(x+xTail) = sin(x) + sin'(x')*xTail
 *		    ~ sin(x) + (1-x*x/2)*xTail
 *	   For better accuracy, let
 *		remez = x^3 *(S2+x^2 *(S3+x^2 *(S4+x^2 *(S5+x^2 *S6))))
 *	   then
 *		sin(x) = x + (S1*x^3 + (x^2 *(remez-xTail/2) + xTail))
 */


double GsbIeee754::kernelSin(double x, double xTail, int32_t xTailIsZero) {
	
	// Remez polynomial approximation of degree 13
	static const double S1 = -1.66666666666666324348e-01;	 // 0xBFC55555 55555549
	static const double S2 = 8.33333333332248946124e-03;	 // 0x3F811111 1110F8A6
	static const double S3 = -1.98412698298579493134e-04;	 // 0xBF2A01A0 19C161D5
	static const double S4 = 2.75573137070700676789e-06;	 // 0x3EC71DE3 57B1FE7D
	static const double S5 = -2.50507602534068634195e-08;	 // 0xBE5AE5E6 8A2B9CEB
	static const double S6 = 1.58969099521155010221e-10;	 // 0x3DE5D93A 5ACFD57C
	
	// 0.5
	static const double HALF = 0.5;

	// sin(x)
	double result = 0.0;

	// xTwo = x*x
	double xTwo = x * x;
	// xThree = (x*x)*x
	double xThree = xTwo * x;
	// Remez polynomial approximation of sin(x)
	double remez = S2 + (xTwo * (S3 + (xTwo * (S4 + (xTwo * (S5 + (xTwo * S6)))))));

	// sin(x) = x + (S1*x^3 + (x^2 *(remez-xTail/2) + xTail))
	if (xTailIsZero == 0) {
		// if xTail = 0
		result = x + (xThree * (S1 + (xTwo * remez)));
	}
	else {
		// if xTail != 0
		result = x - (((xTwo * ((HALF * xTail) - (xThree * remez))) - xTail) - (xThree * S1));
	}

	return result;
}
