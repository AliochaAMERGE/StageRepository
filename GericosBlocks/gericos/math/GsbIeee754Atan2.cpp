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

 GsbIeee754Atan2.cpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************
 *
 *  @file GsbIeee754Atan2.cpp
 *  @brief Implementation of the GsbIeee754::atan2() function.
 *
 *  The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
 *  The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
 *
 *  Changes :
 * 		- All the code has be reworked in order to follow LESIA's Flight Software C++ Coding Standards
 * 		- The algorithm moslty remain the same, but the function has been split into two parts in order to follow the LESIA's Flight Software C++ Coding Standards
 * 		- All variables has been renamed for better understanding.
 * 		- L.113-114 : The condition in order to check whether x or y is NaN has been replaced in order to comply with the LESIA's Flight Software C++ Coding Standards.
 *
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
#include <gericos/blocks/util/GsbMath.hpp>

/* atan2(y,x)
 * Method :
 *	1. Reduce y to positive by atan2(y,x) = -atan2(-y,x).
 *	2. Reduce x to positive by (if x and y are unexceptional):
 *		ARG (x + yHighWordAbs) = arctan(y/x)   	   ... if x > 0,
 *		ARG (x + yHighWordAbs) = pi - arctan[y/(-x)]   ... if x < 0,
 *
 * Special cases:
 *
 *	atan2((anything), NaN ) is NaN.
 *	atan2(NAN , (anything) ) is NaN.
 *	atan2(+-0, +(anything but NaN)) is +-0.
 *	atan2(+-0, -(anything but NaN)) is +-pi.
 *	atan2(+-(anything but 0 and NaN), 0) is +-pi/2.
 *	atan2(+-(anything but INF and NaN), +INF) is +-0.
 *	atan2(+-(anything but INF and NaN), -INF) is +-pi.
 *	atan2(+-INF,+INF ) is +-pi/4.
 *	atan2(+-INF,-INF ) is +-3pi/4.
 *	atan2(+-INF, (anything but,0,NaN, and INF)) is +-pi/2.
 *
 * Constants:
 * The hexadecimal values are the intended ones for the following
 * constants. The decimal values may be used, provided that the
 * compiler will convert from decimal to binary accurately enough
 * to produce the hexadecimal values shown.
 */

double GsbIeee754::atan2(double y, double x) {
	// Tiny value used for Trigger
	static const double TINY = 1.0e-300;
	// Pi : 0x400921FB 54442D18
	static const double PI = 3.1415926535897931160;
	// Pi/2 : 0x3FF921FB 54442D18
	static const double PI_O_2 = 1.5707963267948965580;

	// Result atan2(y,x)
	double result = 0.0;
	// x High Word
	int32_t xHighWord = GsbIeee754::getHighWord(x);
	// x Low Word
	uint32_t xLowWord = GsbIeee754::getLowWord(x);
	// x High Word with the sign bit set to 0
	int32_t xHighWordAbs = xHighWord & 0x7FFFFFFF;
	// y High Word
	int32_t yHighWord = GsbIeee754::getHighWord(y);
	// y Low Word
	uint32_t yLowWord = GsbIeee754::getLowWord(y);
	// y High Word with the sign bit set to 0
	int32_t yHighWordAbs = yHighWord & 0x7FFFFFFF;

	if ((((static_cast<uint64_t>(xHighWordAbs) << 32U) | (static_cast<uint64_t>(xLowWord))) > 0x7FF0000000000000ULL) 
		|| (((static_cast<uint64_t>(yHighWordAbs) << 32U) | (static_cast<uint64_t>(yLowWord))) > 0x7FF0000000000000ULL)) {
		// x or y is NaN
		result = x + y;
	}
	else if (((xHighWord - 0x3FF00000) | static_cast<int32_t>(xLowWord)) == 0) {
		// x=1.0
		result = GsbIeee754::atan(y);
	}
	else {
		// (2*sign of x + sign of y) --> extract cases depending on the sign of x and y. +y+x : 0, -y+x : 1, +y-x : 2, -y-x : 3
		// cppcheck-suppress[shiftTooManyBitsSigned] In order to retrieve the sign bit of x and y, we need to perform the following operations.
		int32_t signOfXY = ((yHighWord >> 31) & 1) | ((xHighWord >> 30) & 2);

		if ((yHighWordAbs | static_cast<int32_t>(yLowWord)) == 0) {
			// when y = 0
			if ((signOfXY == 0) || (signOfXY == 1)) {
				// x positive, y negative
				// atan(+-0, +anything) = +-0
				result = y;
			}
			else if (signOfXY == 2) {
				// x negative, y negative
				// atan(+0, -anything) = pi
				result = PI + TINY;
			}
			else {
				// Both are negative
				// atan(-0, -anything) = -pi
				result = -(PI + TINY);
			}
		}
		else if ((xHighWordAbs | static_cast<int32_t>(xLowWord)) == 0) {
			// when x = 0
			// atan(..., 0) is +-pi/2.
			result = (yHighWord < 0) ? (-(PI_O_2 + TINY)) : (PI_O_2 + TINY);
		}

		else {
			result = GsbIeee754::atan2Utility(y, x, xHighWordAbs, yHighWordAbs, signOfXY, xHighWord, yHighWord);
		}
	}
	return result;
}

double GsbIeee754::atan2Utility(double y, double x, int32_t xHighWordAbs, int32_t yHighWordAbs, int32_t signOfXY, int32_t xHighWord,
		int32_t yHighWord) {
	// Tiny value used for Trigger
	static const double TINY = 1.0e-300;
	// 0.0
	static const double ZERO = 0.0;
	// 0.5
	static const double HALF = 0.5;
	// 3.0
	static const double THREE = 3.0;
	// Pi : 0x400921FB 54442D18
	static const double PI = 3.1415926535897931160E+00;
	// pi(next digit) : 0x3CA1A626 33145C07
	static const double PI_LO = 1.2246467991473531772E-16;
	// Pi/2 : 0x3FF921FB 54442D18
	static const double PI_O_2 = 1.5707963267948965580E+00;
	// Pi/4 : 0x3FE921FB 54442D18
	static const double PI_O_4 = 7.8539816339744827900E-01;

	double result = 0.0;

	if (xHighWordAbs == 0x7FF00000) {
		// when x is INF
		if (yHighWordAbs == 0x7FF00000) {
			// when y is INF
			if (signOfXY == 0) {
				// atan(+INF,+INF)
				result = PI_O_4 + TINY;
			}
			else if (signOfXY == 1) {
				// atan(-INF,+INF)
				result = -(PI_O_4 + TINY);
			}
			else if (signOfXY == 2) {
				// atan(+INF,-INF)
				result = (THREE * PI_O_4) + TINY;
			}
			else {
				// atan(-INF,-INF)
				result = -((THREE * PI_O_4) + TINY);
			}
		}
		else {
			// When y is not INF
			if (signOfXY == 0) {
				// atan(+...,+INF)
				result = ZERO;
			}
			else if (signOfXY == 1) {
				// atan(-...,+INF)
				result = -ZERO;
			}
			else if (signOfXY == 2) {
				// atan(+...,-INF)
				result = PI + TINY;
			}
			else {
				// kf signOfXY == 3
				// atan(-...,-INF)
				result = -(PI + TINY);
			}
		}
	}
	else if (yHighWordAbs == 0x7FF00000) {
		// when y is INF
		// atan(+-INF, ...) is +-pi/2.
		result = (yHighWord < 0) ? (-(PI_O_2 + TINY)) : (PI_O_2 + TINY);
	}
	else {
		// Utility variable for computing the result
		double temp;
		// compute y/x
		// Difference in exponent of y - x
		int32_t k = (yHighWordAbs - xHighWordAbs) >> 20;
		if (k > 60) {
			// |y/x| >  2**60
			temp = PI_O_2 + (HALF * PI_LO);
			// set the sign of y as negative
			signOfXY &= 1;
		}
		else if ((xHighWord < 0) && (k < -60)) {
			// 0 > |y|/x > -2**60
			temp = 0.0;
		}
		else {
			// safe to do y/x
			temp = GsbIeee754::atan(GsbMath::fabs(y / x));
		}
		if (signOfXY == 0) {
			// atan(+,+)
			result = temp;
		}
		else if (signOfXY == 1) {
			// atan(-,+)
			result = -temp;
		}
		else if (signOfXY == 2) {
			// atan(+,-)
			result = PI - (temp - PI_LO);
		}
		else {
			// case 3
			// atan(-,-)
			result = (temp - PI_LO) - PI;
		}
	}
	return result;
}
