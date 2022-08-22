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

 GsbIeee754Floor.cpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************
 *
 *  @file GsbIeee754Floor.cpp
 *  @brief Implementation of the GsbIeee754::floor() function.
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

double GsbIeee754::floor(double value) {
	// Large value close to infinity.
	static const double HUGE = 1.0e300;

	// 32 most significant bits of value (HighWord) --> 1 bit of sign 11 bit of exponent 20 bit of mantissa
	int32_t highWord = static_cast<int32_t>(GsbIeee754::getHighWord(value));
	// 32 least significant bits of value (LowWord) --> 32 bits of mantissa
	int32_t lowWord = static_cast<int32_t>(GsbIeee754::getLowWord(value));
	// Retrieve the highest bit of the exponent part of value, sign included
	int32_t exponent = static_cast<int32_t>(((static_cast<uint32_t>(highWord) >> 20U) & 0x7FFU) - 0x3FFU);

	// The value floored
	double result = 0.0;

	// The objective will be to recover the exponent, and apart from special cases (infinity, NaN, +0 or -0), we will use the exponent to remove superfluous numbers from the mantissa.

	// if exponent < 20, only 20 bit of mantissa are relevent (located in the highWord), all the bits of the lowWord will be set to zero.
	if (exponent < 20) {
		// if exponent < 0, |value| < 1, --> result is 0.0 or -1.0
		if (exponent < 0) {
			if (HUGE + value > 0.0) {
				// if value is positive, return 0.0 (0 <= value < 1)
				if (highWord >= 0) {
					// 64 bits float's bitmask for 0
					lowWord = 0;
					highWord = 0;
				}
				// if value is negative, return -1.0 (-1 < value < 0)
				else if (((highWord & 0x7FFFFFFF) | lowWord) != 0) {
					// 64 bits float's bitmask for -1.0
					highWord = 0xBFF00000;
					lowWord = 0;
				}
			}
			// Build the result with both highWord and lowWord changed
			GsbIeee754::insertWords(result, highWord, lowWord);
		}
		// if 0 <= exponent < 20
		else {
			// Temporary variable for holding a modified word for value.
			uint32_t tempWord;
			// Keep "exponent number" of significant bits of the mantissa, the other will be set to zero
			tempWord = static_cast<uint32_t>(0x000FFFFF >> exponent);
			// if the other bit of the mantissa are zero, value is already an integer, no changes needed
			if (((highWord & static_cast<int32_t>(tempWord)) | lowWord) == 0) {
				result = value;	 // value is integral
			}
			// else, we need to set the least significant bit of the mantissa to zero
			// raise inexact flag
			else if (HUGE + value > 0.0) {
				// if value is negative
				if (highWord < 0) {
					// add 1 at the highest exponent bit location in the mantissa (will result to a +1 to the rounded value, as the value is negative, it will be -1)
					highWord += (0x00100000) >> exponent;
				}
				// Remove the least significant bits of the mantissa (sabering the mantissa using the exponent value)
				highWord &= (~tempWord);
				// Since the exponent is less than 20 --> the bit of the mantissa present in the lowWord will not be used (and will be set to 0)
				lowWord = 0;

				GsbIeee754::insertWords(result, highWord, lowWord);
			}
		}
		// if exponent is greater than 51, the value is either Inf, NaN or an integer (all bit of the mantissa will be used for the integral part)
	}
	else if (exponent > 51) {
		// if all bit of the exponent are 1, value is either inf or NaN
		if (exponent == 0x400) {
			result = value + value;
		}
		else {
			// else the value is an integer as mentioned above
			result = value;
		}
	}
	else {
		// if exposant is in [20,51]
		result = floorUtility(value, highWord, lowWord, exponent);
	}
	return result;
}

double GsbIeee754::floorUtility(double value, int32_t highWord, int32_t lowWord, int32_t exponent) {
	// Large value close to infinity.
	static const double HUGE = 1.0e300;

	// Temporary variable for holding a modified word for value.
	uint32_t tempWord;
	// Another temporary variable for holding a modified word.
	uint32_t secondaryTempWord;
	// The value floored
	double result = 0.0;

	// Every bit of the mantissa present in the highWord will be usefull (20 bits)
	// Some bit of the mantissa present in the lowWord may be set to zero (32 bits)
	// Every bits of tempWord will be set to 1 if the associated bit of the lowWord is usefull
	// The "-20" refere to the 20 bits of the mantissa present in the highWord which will be kept.
	tempWord = ((0xFFFFFFFFU) >> static_cast<uint32_t>(exponent - 20));
	// If all the bit has been set to zero, the value is already an integer
	if ((static_cast<uint32_t>(lowWord) & tempWord) == 0U) {
		result = value;	 // value is integral
	}
	// else remove the unused bit of the mantissa
	// raise inexact flag
	else if (HUGE + value > 0.0) {
		// if value is negative
		if (highWord < 0) {
			// add 1 to the value (negative value floored result to a -1 to the integer part)
			// if the exponent is equal to 20, the +1 (resulting in -1) will be added to the highWord (got a carry)
			if (exponent == 20) {
				highWord += 1;
			}
			// else the +1 (resulting in -1) will be added to the lowWord.
			else {
				// tempWord will be set as the lowWord with the integral part + 1 (resulting in -1)
				secondaryTempWord = lowWord + (1 << (52 - exponent));
				// if + 1 is too big to fit in lowWord
				if (secondaryTempWord < static_cast<uint32_t>(lowWord)) {
					// the previous +1 is reported to highWord (got a carry)
					highWord += 1;
				}
				// lowWord + 1 or set to 0 if we got a carry
				lowWord = static_cast<int32_t>(secondaryTempWord);
			}
		}
		// remove the unused bits of the part of the mantissa present in the lowWord.
		lowWord &= (~tempWord);
		GsbIeee754::insertWords(result, highWord, lowWord);
	}
	return result;
}
