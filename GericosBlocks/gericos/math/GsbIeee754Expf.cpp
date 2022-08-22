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

 GsbIeee754Expf.cpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************
 *
 *  @file GsbIeee754Expf.cpp
 *  @brief Implementation of the GsbIeee754::expf() function
 *
 *  The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
 *  The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
 *
 *  Changes :
 * 		- All the code has be reworked in order to follow LESIA's Flight Software C++ Coding Standards
 * 		- The algorithm moslty remain the same, but the function has been split into two parts in order to follow the LESIA's Flight Software C++ Coding Standards
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

/*
 * Method
 *   1. Argument reduction:
 *      Reduce x to an r so that |r| <= 0.5*ln2 ~ 0.34658.
 *	Given x, find r and integer k such that
 *
 *               x = k*ln2 + r,  |r| <= 0.5*ln2.
 *
 *      Here r will be represented as r = hi-lo for better accuracy.
 *
 *   2. Approximation of exp(r) by a special rational function on
 *	the interval [0,0.34658]:
 *	Write
 *	    R(r**2) = r*(exp(r)+1)/(exp(r)-1) = 2 + r*r/6 - r**4/360 + ...
 *      We use a special Remez algorithm on [0,0.34658] to generate
 * 	a polynomial of degree 5 to approximate R. The maximum error
 *	of this polynomial approximation is bounded by 2**-59.
 * In other words,
 *	    R(z) ~ 2.0 + P1*z + P2*z**2 + P3*z**3 + P4*z**4 + P5*z**5
 *  	(where z=r*r, and the values of P1 to P5 are listed below)
 *	and
 *	    | 2.0+P1*z+...+P5*(z^5) - R(z) | <= 2(^-59)
 *	The computation of exp(r) thus becomes
 *                     2*r
 *		exp(r) = 1 + -------
 *		              R - r
 *                         r*R1(r)
 *		       = 1 + r + ----------- (for better accuracy)
 *		                  2 - R1(r)
 *	where
 *		R1(r) = r - (P1*(r^2) + P2*(r^4)  + ... + P5*(r^10)).
 *
 *   3. Scale back to obtain exp(x):
 *	From step 1, we have
 *	   exp(x) = 2^k * exp(r)
 *
 * Special cases:
 *	exp(INF) is INF, exp(NaN) is NaN.
 *	exp(-INF) is 0, and for finite argument, only exp(0)=1 is exact.
 *
 * Accuracy:
 *	according to an error analysis, the error is always less than 1 ulp (unit in the last place).
 *
 * Misc. info.
 *	For IEEE double
 *	    if x > 88.7228317261  then exp(x) overflow
 *	    if x < -103.972084045 then exp(x) underflow
 *
 * Constants:
 * The hexadecimal values are the intended ones for the following
 * constants. The decimal values may be used, provided that the
 * compiler will convert from decimal to binary accurately enough
 * to produce the hexadecimal values shown.
 */
float GsbIeee754::expf(float value) {
	// Expands to positive float expression that indicates overflow
	static const float HUGE = 1.0e+30F;
	// 2**(-100) = 0x0D800000
	static const float TWO_M_100 = 7.8886090522e-31F;
	// 1.0
	static const float ONE = 1.0F;

	// bitmask of value
	uint32_t btMsk = GsbIeee754::getUwordFromFloat(value);
	// sign bit of value
	uint32_t signValue = (btMsk >> 31U) & 1U;
	// 32 most significant bits of |value| (HighWord)
	uint32_t msbAbs = btMsk & 0x7FFFFFFFU;
	// exp(value), result of the function.
	float result = 0.0F;

	// Filter out non-finite argument
	// if value is NaN
	if (GsbIeee754::floatUwordIsNan(msbAbs)) {
		// Trigger NaN
		result = value + value;
	}
	// if value is infinite (check both +Inf and -Inf because msbAbs is an absolute value)
	else if (GsbIeee754::floatUwordIsInfinite(msbAbs)) {
		// exp(+inf) = inf, exp(-inf) = 0
		result = (signValue == 0U) ? value : 0.0F;
	}
	// if exp(value) cannot be represented with a float (case overflow)
	else if ((signValue == 0U) && (msbAbs > GsbIeee754::FLT_UWORD_LOG_MAX)) {
		// Trigger overflow
		result = (HUGE * HUGE);
	}
	// if exp(value) cannot be represented with a float (case underflow)
	else if ((signValue == 1U) && (msbAbs > GsbIeee754::FLT_UWORD_LOG_MIN)) {
		// Trigger underflow
		result = (TWO_M_100 * TWO_M_100);
	}
	// when |value| < 2**-28
	else if ((msbAbs < 0x31800000U) && ((HUGE + value) > ONE)) {
		// Trigger inexact
		result = ONE + value;
	}
	// Call utility function to compute the result.
	else {
		result = expfUtility(value, signValue, msbAbs);
	}
	return result;
}

float GsbIeee754::expfUtility(float value, uint32_t signValue, uint32_t msbAbs) {
	// 2**(-100) = 0x0d800000
	static const float TWO_M_100 = 7.8886090522e-31F;
	// +- 0.5, used to floor or ceil k
	static const float HALF[2] = { 0.5F, -0.5F };
	// NB : (LN2_HI + LN2_LO) = ln2
	static const float LN2_HI[2] = { 6.9314575195e-01F,		 // 0x3F317200
			-6.9314575195e-01F };	 // 0xBF317200
	static const float LN2_LO[2] = { 1.4286067653e-06F,		 // 0x35BFBE8E
			-1.4286067653e-06F };	 // 0xB5BFBE8E
	// (1 / ln2)
	static const float INV_LN2 = 1.4426950216e+00F;	// 0x3FB8AA3B
	// We use a special Remez algorithm on [0,0.34658] to generate a polynomial
	// of degree 5 to approximate R. With R(r**2) = r*(exp(r)+1)/(exp(r)-1)
	static const float P1 = 1.6666667163e-01F;	 // 0x3E2AAAAB
	static const float P2 = -2.7777778450e-03F;	 // 0xBB360B61
	static const float P3 = 6.6137559770e-05F;	 // 0x388AB355
	static const float P4 = -1.6533901999e-06F;	 // 0xB5DDEA0E
	static const float P5 = 4.1381369442e-08F;	 // 0x3331BB4C
	// 1.0
	static const float ONE = 1.0F;

	// 1. Argument reduction :
	//   Reduce value to an r so that |r| <= (0.5*ln2) ~ 0.34658.
	//	Given value, find r and integer k such that
	//               value = k*ln2 + r,  |r| <= (0.5*ln2).
	//      Here r will be represented as r = hi-lo for better accuracy.

	// k defined as value = k*ln(2) + r with |r| <= (0.5*ln(2))
	int32_t k = 0;
	// r = hi-lo
	float hi = 0.0F;
	// r = hi-lo
	float lo = 0.0F;
	// Result of polynomiale computation
	float rPolynomial;
	// value squared
	float squareValue;
	// exp(value), result of the function.
	float result = 0.0F;

	// if |value| < 0.5 * ln(2) --> value is unchanged
	// if |value| > 0.5 * ln(2)
	if (msbAbs > 0x3EB17218U) {
		// and if |value| < 1.5 * ln(2)
		if (msbAbs < 0x3F851592U) {
			// r = value +- ln(2)
			hi = value - LN2_HI[signValue];
			// reminder : r = hi-lo
			lo = LN2_LO[signValue];
			// k = 1 if value is positive, -1 otherwise
			k = 1 - static_cast<int32_t>(signValue) - static_cast<int32_t>(signValue);
		}
		// if |value| > 1.5 * ln(2)
		else {
			// ceil the value if value is positive, floor it otherwise
			k = (INV_LN2 * value) + HALF[signValue];
			// k * ln2HI is exact here
			hi = value - (static_cast<float>(k) * LN2_HI[0]);
			lo = static_cast<float>(k) * LN2_LO[0];
		}
		// update value by r = hi-lo
		value = hi - lo;
	}

	// value is now in primary range
	// 2. Approximation of exp(r) by a special rational function on the interval [0,0.34658]
	squareValue = value * value;
	rPolynomial = value - (squareValue * (P1 + (squareValue * (P2 + (squareValue * (P3 + (squareValue * (P4 + (squareValue * P5)))))))));

	//   3. Scale back to obtain exp(x):
	// From step 1, we have
	//    exp(x) = 2^k * exp(r)

	// |value| <= 0.5*ln(2) --> k = 0
	if (k == 0) {
		// exp(x) = 2^k * exp(r)  -->  exp(x) = exp(r)
		// exp(r) = 1 + r +  ( r*R1(r)  /  2 - R1(r) )
		// R1(r) = rPolynomial
		result = (ONE - (((value * rPolynomial) / (rPolynomial - 2.0f)) - value));
	}
	// if k != 0
	else {
		// exp(r) = 1 + r +  ( r*R1(r)  /  2 - R1(r) )
		// R1(r) = rPolynomial
		float expR = (ONE - ((lo - ((value * rPolynomial) / (2.0f - rPolynomial))) - hi));
		uint32_t expRbtmsk = GsbIeee754::getUwordFromFloat(expR);

		// exp(x) = 2^k * exp(r)

		// if k can be represented with 8 bits (exponent part of a float reprensentation) add k to expR's exponent (will result in + 2^k)
		if (k >= -125) {
			result = GsbIeee754::getFloatFromUword((expRbtmsk + ((static_cast<uint32_t>(k) << 23U))));
			// else (k cannot be represented with 8 bits)
		}
		// if k < -125
		else {
			// if k cannot be represented with 8 bits, add 100 to k, and then multiply by TWO_M_100 (2^-100) at the end
			// add k to expR's exponent  (will result in + 2^k)
			expR = GsbIeee754::getFloatFromUword((expRbtmsk + ((static_cast<uint32_t>(k + 100) << 23U))));
			// TWO_M_100 = 2^-100 : compensate for "(k+100)"
			result = expR * TWO_M_100;
		}
	}
	return result;
}
