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

 GsbIeee754GetRemainderPio2.cpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************
 *
 *  @file GsbIeee754GetRemainderPio2.cpp
 *  @brief Implementation of the GsbIeee754::getRemainderPio2() function.
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
#include <gericos/blocks/util/GsbMath.hpp>

/* getRemainderPio2(input,output)
 *
 * return the remainder of input rem pi/2 in output[0]+output[1]
 * use getKernelRemainderPio2()
 */

int32_t GsbIeee754::getRemainderPio2(double input, double* output) {
	// pio2_1:   first  33 bit of pi/2
	// --- 0x3FF921FB 54400000
	static const double PIO2_1 = 1.57079632673412561417e+00;
	// pio2_1t:  pi/2 - pio2_1
	// --- 0x3DD0B461 1A626331
	static const double PIO2_1T = 6.07710050650619224932e-11;
	// pio2_2:   second 33 bit of pi/2
	// --- 0x3DD0B461 1A600000
	static const double PIO2_2 = 6.07710050630396597660e-11;
	// pio2_2t:  pi/2 - (pio2_1+pio2_2)
	// --- 0x3BA3198A 2E037073
	static const double PIO2_2T = 2.02226624879595063154e-21;

	// remainder of input
	int32_t result = 0;

	// highWordTemp word of input
	int32_t highWord = GsbIeee754::getHighWord(input);
	// HighWord Abs
	int32_t highWordAbs = highWord & 0x7FFFFFFF;
	// |input| ~<= pi/4 , no need for reduction
	if (highWordAbs <= 0x3FE921FB) {
		output[0] = input;
		output[1] = 0;
		result = 0;
	}
	// |input| < 3pi/4, special case with n=+-1
	else if (highWordAbs < 0x4002D97C) {
		// Buffer for holding the result
		double temp = 0.0;
		if (highWord > 0) {
			temp = input - PIO2_1;
			if (highWordAbs != 0x3FF921FB) {
				// 33+53 bit pi is good enough
				output[0] = temp - PIO2_1T;
				output[1] = (temp - output[0]) - PIO2_1T;
			}
			else {
				// near pi/2, use 33+33+53 bit pi
				temp -= PIO2_2;
				output[0] = temp - PIO2_2T;
				output[1] = (temp - output[0]) - PIO2_2T;
			}
			result = 1;
		}
		else {
			// negative input
			temp = input + PIO2_1;
			if (highWordAbs != 0x3FF921FB) {
				// 33+53 bit pi is good enough
				output[0] = temp + PIO2_1T;
				output[1] = (temp - output[0]) + PIO2_1T;
			}
			else {
				// near pi/2, use 33+33+53 bit pi
				temp += PIO2_2;
				output[0] = temp + PIO2_2T;
				output[1] = (temp - output[0]) + PIO2_2T;
			}
			result = -1;
		}
	}
	else {
		result = GsbIeee754::getRemainderPio2Utility(input, output, highWord, highWordAbs);
	}
	return result;
}

int32_t GsbIeee754::getRemainderPio2Utility(double input, double* output, int32_t highWord, int32_t highWordAbs) {
	// 0.0
	// --- 0x00000000 00000000
	static const double ZERO = 0.00000000000000000000e+00;
	// 0.5
	// --- 0x3FE00000 00000000
	static const double HALF = 5.00000000000000000000e-01;
	// 2^24
	// --- 0x41700000 00000000
	static const double TWO24 = 1.67772160000000000000e+07;
	// invpio2:  53 bits of 2/pi
	// --- 0x3FE45F30 6DC9C883
	static const double INV_PIO2 = 6.36619772367581382433e-01;
	// pio2_1:   first  33 bit of pi/2
	// --- 0x3FF921FB 54400000
	static const double PIO2_1 = 1.57079632673412561417e+00;
	// pio2_1t:  pi/2 - pio2_1
	// --- 0x3DD0B461 1A626331
	static const double PIO2_1T = 6.07710050650619224932e-11;
	// pio2_2:   second 33 bit of pi/2
	// --- 0x3DD0B461 1A600000
	static const double PIO2_2 = 6.07710050630396597660e-11;
	// pio2_2t:  pi/2 - (pio2_1+pio2_2)
	// --- 0x3BA3198A 2E037073
	static const double PIO2_2T = 2.02226624879595063154e-21;
	// pio2_3:   third  33 bit of pi/2
	// --- 0x3BA3198A 2E000000
	static const double PIO2_3 = 2.02226624871116645580e-21;
	// pio2_3t:  pi/2 - (pio2_1+pio2_2+pio2_3)
	// --- 0x397B839A 252049C1
	static const double PIO2_3T = 8.47842766036889956997e-32;

	// A temporary variable used to compute the difference between the input's exponent and the result's exponent in order to check if another iteration is needed.
	int32_t exponentDiff;
	// Number of time input can be divided by pi/2
	int32_t n;
	// The number of time input can be divided by pi/2.
	int32_t result = 0;

	// |input| ~<= 2^19*(pi/2), medium size
	if (highWordAbs <= 0x413921FB) {
		// Absolute value of input
		double inputAbs = GsbMath::fabs(input);
		// n as input =  N * pi/2 + remainder
		n = static_cast<int32_t>((inputAbs * INV_PIO2) + HALF);
		// double version of n
		double doubleN = static_cast<double>(n);
		// Remainder of input - N * pi/2
		double rem = inputAbs - (doubleN * PIO2_1);
		// 1st round good to 85 bit
		// tail of input, small precision part of computation
		double tail = doubleN * PIO2_1T;
		// Retrieve the exponent part of the input
		int32_t inputExponent = highWordAbs >> 20;
		// remainder - tail (rest of input - N * pi/2) without the small precision part
		output[0] = rem - tail;
		// High word of the remainder (minus the tail)
		uint32_t highWordTemp = GsbIeee754::getHighWord(output[0]);
		// Difference between exponent of output(remainder of input - N * pi/2) and input
		exponentDiff = inputExponent - static_cast<int32_t>((highWordTemp >> 20U) & 0x7FFU);
		if (exponentDiff > 16) {
			// 2nd iteration needed, good to 118
			inputAbs = rem;
			tail = doubleN * PIO2_2;
			rem = inputAbs - tail;
			tail = (doubleN * PIO2_2T) - ((inputAbs - rem) - tail);
			output[0] = rem - tail;
			highWordTemp = GsbIeee754::getHighWord(output[0]);
			exponentDiff = inputExponent - static_cast<int32_t>((highWordTemp >> 20U) & 0x7FFU);
			if (exponentDiff > 49) {
				// 3rd iteration need, 151 bits acc will cover all possible cases
				inputAbs = rem;
				tail = doubleN * PIO2_3;
				rem = inputAbs - tail;
				tail = (doubleN * PIO2_3T) - ((inputAbs - rem) - tail);
				output[0] = rem - tail;
			}
		}

		// The output has been computed, we will compute its tail (small precision remainder)
		output[1] = (rem - output[0]) - tail;
		if (highWord < 0) {
			// Change the sign if necessary.
			output[0] = -output[0];
			output[1] = -output[1];
			result = -n;
		}
		else {
			result = n;
		}
	}
	// all other (large) arguments (>2^19*(pi/2))
	else if (highWordAbs >= 0x7FF00000) {
		// input is inf or NaN
		output[0] = output[1] = input - input;
		result = 0;
	}
	else {
		// In case of large input, it will be reduced to a value close to pi/2, with trailing zeros removed.
		// Then getKernelRemainderPio2 will be called in order to get an accurate result.
		double buffer[3];
		// (>2^19*(pi/2) but less than infinite)
		double temp = 0.0;
		// set temp = scalbn(|input|,ilogb(input)-23)
		uint32_t low = GsbIeee754::getLowWord(input);
		GsbIeee754::setLowWord(temp, low);
		// exponentReduced = ilogb(temp) -23
		int32_t exponentReduced = (highWordAbs >> 20) - 1046;
		GsbIeee754::setHighWord(temp, highWordAbs - (exponentReduced << 20));
		// Build an array with the decimal part of temp (* 2^(-24^exponentDiff))
		for (exponentDiff = 0; exponentDiff < 2; exponentDiff++) {
			buffer[exponentDiff] = static_cast<double>(static_cast<int32_t>(temp));
			temp = (temp - buffer[exponentDiff]) * TWO24;
		}
		buffer[2] = temp;
		// Utility variable used to iterate through the buffer.
		int32_t bufferDim;
		/*>RELAX<Rule_2040_V3> Exact 0.0 is the only valid value that we want to skip. */
		for (bufferDim = 3; ((bufferDim > 1) && (buffer[bufferDim - 1] == ZERO)); --bufferDim) {
			// Empty loop : skip ZERO term
		}
		// Call getKernelRemainderPio2 on buffer with its exponent reduced to an exploitable range
		n = GsbIeee754::getKernelRemainderPio2(&buffer[0], output, exponentReduced, bufferDim);
		if (highWord < 0) {
			// Change the sign if necessary.
			output[0] = -output[0];
			output[1] = -output[1];
			result = -n;
		}
		else {
			result = n;
		}
	}
	return result;
}
