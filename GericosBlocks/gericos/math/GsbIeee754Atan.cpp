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

 GsbIeee754Atan.cpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************
 *
 *  @file GsbIeee754Atan.cpp
 *  @brief Implementation of the GsbIeee754::atan() function.
 *
 *  The algorithm is based on the ONE developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
 *  The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
 *
 *  Changes :
 * 		- All the code has be reworked in order to follow LESIA's Flight Software C++ Coding Standards
 * 		- The algorithm moslty remain the same, but a small change in its structure in order to follow the LESIA's Flight Software C++ Coding Standards
 * 		- All variables has been renamed for better understanding.
 ************************************************************************/

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed ATAN_TAYL SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#include <gericos/blocks/math/GsbIeee754.hpp>
#include <gericos/blocks/util/GsbMath.hpp>

/* atan(x)
 * Method
 *   1. Reduce x to positive by atan(x) = -atan(-x).
 *   2. According to the integer k = 4t + 0.25 chopped, t = x, the argument
 *      is further reduced to ONE of the following intervals and the
 *      arctangent of t is evaluated by the corresponding formula:
 *
 *      [0,7/16]      atan(x) = t-t^3*(a1+t^2*(a2+...(a10+t^2*a11)...)
 *      [7/16,11/16]  atan(x) = atan(1/2) + atan( (t-0.5)/(1+t/2) )
 *      [11/16.19/16] atan(x) = atan( 1 ) + atan( (t-1)/(1+t) )
 *      [19/16,39/16] atan(x) = atan(3/2) + atan( (t-1.5)/(1+1.5t) )
 *      [39/16,INF]   atan(x) = atan(INF) + atan( -1/t )
 *
 * Constants:
 * The hexadecimal values are the intended ones for the following
 * constants. The decimal values may be used, provided that the
 * compiler will convert from decimal to binary accurately enough
 * to produce the hexadecimal values shown.
 */

double GsbIeee754::atan(double x) {
	// highWords of atan(0.5), atan(1.0), atan(1.5) and atan(+inf) in this order
	static const double ATAN_HI[] = { 4.63647609000806093515e-01,	 // atan(0.5) highWord : 0x3FDDAC67 0561BB4F
			7.85398163397448278999e-01,	 // atan(1.0) highWord : 0x3FE921FB 54442D18
			9.82793723247329054082e-01,	 // atan(1.5) highWord : 0x3FEF730B D281F69B
			1.57079632679489655800e+00,	 // atan(inf) highWord : 0x3FF921FB 54442D18
			};
	// lowWords of atan(0.5), atan(1.0), atan(1.5) and atan(+inf) in this order
	static const double ATAN_LO[] = { 2.26987774529616870924e-17,	 // atan(0.5) lowWord : 0x3C7A2B7F 222F65E2
			3.06161699786838301793e-17,	 // atan(1.0) lowWord : 0x3C81A626 33145C07
			1.39033110312309984516e-17,	 // atan(1.5) lowWord : 0x3C700788 7AF0CBBD
			6.12323399573676603587e-17,	 // atan(inf) lowWord : 0x3C91A626 33145C07
			};

	// A Taylor series expansion of the function atan
	static const double ATAN_TAYL[] = { 3.33333333333329318027e-01,		// 0x3FD55555 5555550D
			-1.99999999998764832476e-01,	// 0xBFC99999 9998EBC4
			1.42857142725034663711e-01,		// 0x3FC24924 920083FF
			-1.11111104054623557880e-01,	// 0xBFBC71C6 FE231671
			9.09088713343650656196e-02,		// 0x3FB745CD C54C206E
			-7.69187620504482999495e-02,	// 0xBFB3B0F2 AF749A6D
			6.66107313738753120669e-02,		// 0x3FB10D66 A0D03D51
			-5.83357013379057348645e-02,	// 0xBFADDE2D 52DEFD9A
			4.97687799461593236017e-02,		// 0x3FA97B4B 24760DEB
			-3.65315727442169155270e-02,	// 0xBFA2B444 2C6A6C2F
			1.62858201153657823623e-02,		// 0x3F90AD3A E322DA11
			};

	// 1.0
	static const double ONE = 1.0;
	// 2.0
	static const double TWO = 2.0;
	// 1.5
	static const double ONE_HALF = 1.5;
	// A huge value used for trigger
	static const double HUGE = 1.0e300;
	// The result of the computation.
	double result = 0.0;

	// x HighWord
	int32_t highWord = GsbIeee754::getHighWord(x);
	// x HighWord with sign bit removed as atan(x) = -atan(-x)
	int32_t highWordAbs = highWord & 0x7FFFFFFF;

	// if |x| >= 2^66
	if (highWordAbs >= 0x44100000) {
		// x LowWord
		uint32_t lowWord = GsbIeee754::getLowWord(x);
		// Check if the input value is NaN
		if ((highWordAbs > 0x7FF00000) || ((highWordAbs == 0x7FF00000) && (lowWord != 0U))) {
			// Trigger and return NaN
			result = x + x;
		}
		// atan tends toward +-pi/2, if the input is big enough, we do not need to compute the result and will return the limit.
		else if (highWord > 0) {
			// Return pi/2
			result = ATAN_HI[3] + ATAN_LO[3];
		}
		else {
			// return -pi/2
			result = -(ATAN_HI[3] + ATAN_LO[3]);
		}
	}
	// |x| < 2^-27
	if (highWordAbs < 0x3E400000) {
		// The value is too small to be represented, it will be returned and Inexact flag will be triggered
		if (HUGE + x > ONE) {
			// Trigger inexact
			result = x;
		}
	}
	else {
		// The index of the desired value in the atan array.
		int32_t index;
		// |x| < 0.4375
		if (highWordAbs < 0x3FDC0000) {
			// The value is small enough that it will not need the ATAN_HI/ATAN_LO array.
			index = -1;
		}
		else {
			x = GsbMath::fabs(x);
			// |x| < 1.1875
			if (highWordAbs < 0x3FF30000) {
				// 7/16 <=|x|<11/16
				if (highWordAbs < 0x3FE60000) {
					index = 0;
					x = ((TWO * x) - ONE) / (TWO + x);
				}
				else {
					// 11/16<=|x|< 19/16
					index = 1;
					x = (x - ONE) / (x + ONE);
				}
			}
			else {
				if (highWordAbs < 0x40038000) {
					// |x| < 2.4375
					index = 2;
					x = (x - ONE_HALF) / (ONE + (ONE_HALF * x));
				}
				else {
					// 2.4375 <= |x| < 2^66
					index = 3;
					x = -(ONE / x);
				}
			}
		}
		// end of argument reduction
		double xTwo = x * x;
		double xFour = xTwo * xTwo;
		// break sum from i=0 to 10 ATAN_TAYL[i]*(xTwo**(i+1)) into odd and even poly
		double oddPoly = xTwo * (ATAN_TAYL[0] + (xFour * (ATAN_TAYL[2] + (xFour * (ATAN_TAYL[4] + (xFour * (ATAN_TAYL[6] + (xFour * (ATAN_TAYL[8] + (xFour * ATAN_TAYL[10]))))))))));
		double evenPoly = xFour * (ATAN_TAYL[1] + (xFour * (ATAN_TAYL[3] + (xFour * (ATAN_TAYL[5] + (xFour * (ATAN_TAYL[7] + (xFour * ATAN_TAYL[9]))))))));
		if (index < 0) {
			result = x - (x * (oddPoly + evenPoly));
		}
		else {
			xTwo = ATAN_HI[index] - (((x * (oddPoly + evenPoly)) - ATAN_LO[index]) - x);
			result = (highWord < 0) ? (-xTwo) : (xTwo);
		}
	}
	return result;
}
