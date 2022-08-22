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

 GsbIeee754Scalbn.cpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************
 *
 *  @file GsbIeee754Scalbn.cpp
 *  @brief Implementation of the GsbIeee754::scalbn() function
 *
 *  The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS). 
 *  The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
 *
 *  Changes :
 * 		- All the code has be reworked in order to follow LESIA's Flight Software C++ Coding Standards
 * 		- The algorithm moslty remain the same, but the function has been split into two parts in order to follow the LESIA's Flight Software C++ Coding Standards
 * 		- All variables has been renamed for better understanding.
 * 		- The part of the algorithm handling subnormal numbes has be greatly simplified. These values will be considered as 0.0, and treated as such.
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

// returns x * 2^n computed by exponent manipulation rather than by actually performing an exponentiation or a multiplication.
double GsbIeee754::scalbn(double x, int32_t n) {
	// The result : x * 2^n
	double result = 0.0;

	// Extract the 32 most significant bits of x.
	// (1 bit of sign, 11 bits of exponent, 20 bits of mantissa)
	uint32_t highWord = getHighWord(x);

	// Isolate the exponent part of x (NB : exponent will always be positive)
	int32_t exponent = static_cast<int32_t>((highWord & 0x7FF00000U) >> 20);

	// if the exponent part is all zeros, x is either subnormal or +-0
	// We will always defined subnormal value as 0.0, and we will always handle them like so.
	if (exponent == 0) {
		// If every bits (except the sign) are zeros, x is +-0, the result will be +-0.0.
		result = copySign(0.0, x);
	}
	// If exponent's bits are all set to 1, x is either infinte or NaN.
	else if (exponent == 0x7FF) {
		// Trigger Infinite or NaN
		result = x + x;
	}
	else {
		result = scalbnUtility(x, n, highWord, exponent);
	}
	return result;
}

double GsbIeee754::scalbnUtility(double x, int32_t n, uint32_t highWord, int32_t exponent) {
	// 2^(-54)
	static const double TWO_M_54 = 5.55111512312578270212e-17;	// 0x3C900000, 0x00000000
	// Huge value, used to trigger Inexact or Infinite
	static const double HUGE = 1.0e+300;
	// Tiny value, used to trigger Inexact or - Infinite
	static const double TINY = 1.0e-300;

	// The result : x * 2^n
	double result = 0.0;

	// Adding n to the exponent, this amounts to multiplying x by 2^n.
	exponent = exponent + n;
	// If the new exponent is too large, Trigger an overflow
	if (exponent > 0x7FE) {
		// Return +-Infinity
		result = HUGE * copySign(HUGE, x);
		// If the exponent is strictly positive, we have a correct result.
	}
	else if (exponent > 0) {
		// Set the modified exponent bits back in x, then return the result.
		setHighWord(x, (highWord & 0x800FFFFFU) | (exponent << 20));
		result = x;
	}
	else if (exponent <= -54) {
		// Check for integer overflow of the previous operation
		// 50000 is an arbitrary value
		if (n > 50000) {
			// n + exponent overflow toward +Inf, Trigger an overflow.
			// if n is too big, n+exponent will overflow and return a negative value even if both are positive.
			// Return +- Infinity.
			result = HUGE * copySign(HUGE, x);
		}
		else {
			// n + exponent overflow toward -Inf, Trigger an underflow.
			// If n is too small, x * 2^n will underflow and return a value close to zero.
			// Return +-0.0.
			result = TINY * copySign(TINY, x);
		}
	}
	else {
		// The result is subnormal
		// In order to rebuild a subnormal result, we first add 54 to the exponent.
		exponent += 54;
		// Then set back the exponent to x
		setHighWord(x, (highWord & 0x800FFFFFU) | (exponent << 20));
		// And finally retrieve the previously added 54.
		result = x * TWO_M_54;
	}
	return result;
}
