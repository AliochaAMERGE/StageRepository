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

 GsbIeee754GetKernelRemainderPio2.cpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************
 *
 *  @file GsbIeee754GetKernelRemainderPio2.cpp
 *  @brief Implementation of the GsbIeee754::getKernelRemainderPio2() function
 *
 *  The algorithm is based on the ONE developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
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

/*
 * getKernelRemPio2 return the last three digits of N with
 *		ouput = input - N*pi/2
 * so that |ouput| < pi/2.
 *
 * The method is to compute the integer (mod 8) and fraction parts of (2/pi)*input without doing the full multiplication.
 * In general we skip the part of the product that are known to be a huge integer (more accurately, = 0 mod 8 ).
 * Thus the number of operations are independent of the exponent of the input.
 *
 * The algorith used here is described in :
 * M. H. Payne and R. N. Hanek, "Radian reduction for trigonometric functions", ACM SIGNUM Newslett., vol. 18, no. 1, pp. 19-24, Jan. 1983.
 *
 * (2/pi) is represented by an array of 24-bit integers in I_PIO2[].
 *
 * Input parameters:
 * 	input[]	The input value (must be positive) is broken into inputDim
 *		pieces of 24-bit integers in double precision format.
 *		input[i] will be the i-th 24 bit of input. The scaled exponent
 *		of input[0] is given in input parameter inputExponent (i.e., input[0] * 2^inputExponent
 *		match input's up to 24 bits.
 *
 *		Example of breaking a double positive preCompute into input[0]+input[1]+input[2]:
 *			inputExponent = ilogb(preCompute)-23
 *			preCompute  = scalbn(preCompute,-inputExponent)
 *		for i = 0,1,2
 *			input[i] = floor(preCompute)
 *			preCompute	 = (preCompute-input[i])*2**24
 *
 *
 *	ouput[]	output result in an array of double precision numbers.
 *		The dimension of ouput[] is 2 which results in 53-bit precision.
 *		The actual value is the sum of them.
 *
 *	inputExponent	The exponent of input[0]
 *
 *	inputDim	dimension of input[]
 *
 * External function:
 *	double scalbn(), floor().
 *
 *
 * Here is the description of some local variables:
 *
 *  I_PIO2[]
 *		integer array, contains the (24*i)-th to (24*i+23)-th
 *		bit of 2/pi after binary point. The corresponding
 *		floating value is
 *
 *			I_PIO2[i] * 2^(-24(i+1)).
 *
 * 	defaultPrecision	defaultPrecision+1 is the initial number of terms of I_PIO2[] needed
 *		in the computation. The recommended value is 2,3,4,
 *		6 for single, double, extended,and quad.
 *
 * 	precision	local integer variable indicating the number of
 *		terms of I_PIO2[] used.
 *
 *	(inputDim - 1)	inputDim - 1
 *
 *	ipioIndex	index for pointing to the suitable I_PIO2[] for the
 *		computation. In general, we want
 *			( 2^inputExponent * input[0] * I_PIO2[ipioIndex-1] * 2^(-24jv) ) / 8
 *		is an integer. Thus
 *			inputExponent - 3 - 24 * ipioIndex >= 0 or (inputExponent-3) / 24 >= ipioIndex
 *		Hence ipioIndex = max(0, (inputExponent-3)/24).
 *
 * 	q[]	double array with integral value, representing the
 *		24-bits chunk of the product of input and 2/pi.
 *
 *	q0Exponent	the corresponding exponent of q[0]. Note that the
 *		exponent for q[i] would be q0Exponent-24*i.
 *
 *	PIO2[]	double precision array, obtained by cutting pi/2
 *		into 24 bits chunks.
 *
 *	doubleIpio2[]	I_PIO2[] in floating point
 *
 *	intQ[]	integer array by breaking up q[] in 24-bits chunk.
 *
 *	finalQ[]	final product of input*(2/pi) in finalQ[0],..,finalQ[defaultPrecision]
 *
 *	qHighBits	integer. If >0 it indicates q[] is >= 0.5, hence
 *		it also indicates the *sign* of the result.
 */

/*
 * Constants:
 * The hexadecimal values are the intended ones for the following constants. The decimal values may be used, provided that the
 * compiler will convert from decimal to binary accurately enough to produce the hexadecimal values shown.
 */
/// @metric_deviation [ct_vg] Checker : Function extracted from the Sun Microsystems libraries. In order to respect this metric, we will need to split the function in several parts which will lead to
/// a decrease in the function readability, and an improper use of arrays as function parameters.
int32_t GsbIeee754::getKernelRemainderPio2(double* input, double* ouput, int32_t inputExponent, int32_t inputDim) {
	// Table of constants for 2/pi, 396 Hex digits (476 decimal) of 2/pi
	// integer array, contains the (24*i)-th to (24*i+23)-th bit of 2/pi after binary point. The corresponding floating value is I_PIO2[i] * 2^(-24(i+1)).
	static const int32_t I_PIO2[] = { 0xA2F983, 0x6E4E44, 0x1529FC, 0x2757D1, 0xF534DD, 0xC0DB62, 0x95993C, 0x439041, 0xFE5163, 0xABDEBB,
			0xC561B7, 0x246E3A, 0x424DD2, 0xE00649, 0x2EEA09, 0xD1921C, 0xFE1DEB, 0x1CB129, 0xA73EE8, 0x8235F5, 0x2EBB44, 0x84E99C,
			0x7026B4, 0x5F7E41, 0x3991D6, 0x398353, 0x39F49C, 0x845F8B, 0xBDF928, 0x3B1FF8, 0x97FFDE, 0x05980F, 0xEF2F11, 0x8B5A0A,
			0x6D1F6D, 0x367ECF, 0x27CB09, 0xB74F46, 0x3F669E, 0x5FEA2D, 0x7527BA, 0xC7EBE5, 0xF17B3D, 0x0739F7, 0x8A5292, 0xEA6BFB,
			0x5FB11F, 0x8D5D08, 0x560330, 0x46FC7B, 0x6BABF0, 0xCFBC20, 0x9AF436, 0x1DA9E3, 0x91615E, 0xE61B08, 0x659985, 0x5F14A0,
			0x68408D, 0xFFD880, 0x4D7327, 0x310606, 0x1556CA, 0x73A8C9, 0x60E27B, 0xC08C6B, };

	// Double precision array, obtained by cutting pi/2 into 24 bits chunks.
	static const double PIO2[] = { 1.57079625129699707031e+00,	 // 0x3FF921FB 40000000
			7.54978941586159635335e-08,	 // 0x3E74442D 00000000
			5.39030252995776476554e-15,	 // 0x3CF84698 80000000
			3.28200341580791294123e-22,	 // 0x3B78CC51 60000000
			1.27065575308067607349e-29,	 // 0x39F01B83 80000000
			1.22933308981111328932e-36,	 // 0x387A2520 40000000
			2.73370053816464559624e-44,	 // 0x36E38222 80000000
			2.16741683877804819444e-51,	 // 0x3569F31D 00000000
			};

	// 0.0
	static const double ZERO = 0.0;
	// 1.0
	static const double ONE = 1.0;
	// 2^24
	static const double TWO_24 = 1.67772160000000000000e+07;	 // 0x41700000 00000000
	// 2^-24
	static const double TWO_N24 = 5.96046447753906250000e-08;	// 0x3E700000 00000000
	// 8.0
	static const double EIGHT = 8.0;
	// 1/8.0
	static const double ONE_O_HEIGHT = 0.125;

	// Indicate if we have a carry
	int32_t carry;
	// N like : ouput = input - N*pi/2
	int32_t n;
	// loop iterator
	int32_t i;
	// loop variable
	int32_t k;
	// If >0, it indicates q[] is >= 0.5, hence it also indicates the *sign* of the result.
	int32_t qHighBits;
	// q[] computed
	double preCompute;
	// utility variable
	double temp;
	// utility variable
	bool recompute;

	// Init arrays.

	// Integer array by breaking up q[] in 24-bits chunk.
	int32_t intQ[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	// I_PIO2[] in floating point.
	double doubleIpio2[20] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

	// Final product of input*(2/pi) in finalQ[0], ..., finalQ[defaultPrecision]
	double finalQ[20] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

	// Double array with integral value, representing the 24-bits chunk of the product of input and 2/pi.
	double q[20] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

	// initialize defaultPrecision
	// defaultPrecision+1 is the initial number of terms of I_PIO2[] needed in the computation.
	int32_t defaultPrecision = 4;
	// useful number of terms used to determine the result
	int32_t precision;

	// determine ipioIndex, q0Exponent, note that 3 > q0Exponent

	// index for pointing to the suitable I_PIO2[] for the computation.
	int32_t ipioIndex = (((inputExponent - 3) / 24) < 0) ? 0 : ((inputExponent - 3) / 24);

	// The corresponding exponent of q[0].
	int32_t q0Exponent = inputExponent - (24 * (ipioIndex + 1));

	// set up doubleIpio2[0] to doubleIpio2[(inputDim-1) + defaultPrecision]
	// where doubleIpio2[(inputDim-1) + defaultPrecision] = I_PIO2[ipioIndex+defaultPrecision]
	int32_t j = ipioIndex - (inputDim - 1);
	for (i = 0; i < (inputDim + defaultPrecision); i++) {
		doubleIpio2[i] = (j < 0) ? ZERO : static_cast<double>(I_PIO2[j]);
		j++;
	}

	// compute q[0], q[1], ..., q[defaultPrecision]
	for (i = 0; i <= defaultPrecision; i++) {
		temp = 0.0;
		for (j = 0; j < inputDim; j++) {
			temp += input[j] * doubleIpio2[(inputDim - 1) + (i - j)];
		}
		q[i] = temp;
	}

	// Useful terms used in the computation.
	precision = defaultPrecision;

	// Computation of preCompute
	do {
		recompute = false;

		// distill q[] into intQ[] reversingly
		preCompute = q[precision];
		i = 0;
		for (j = precision; j > 0; j--) {
			// Retrieve the tailing value of preCompute (same precision as input)
			temp = static_cast<double>(static_cast<int32_t>(TWO_N24 * preCompute));
			// small tmp part of preCompute(cast to int then to double then int and check differences)
			intQ[i] = static_cast<int32_t>(preCompute - (TWO_24 * temp));
			preCompute = q[j - 1] + temp;
			i++;
		}
		// compute n

		// actual value of preCompute
		preCompute = GsbIeee754::scalbn(preCompute, q0Exponent);
		// trim off integer >= 8
		// Prevent the computation of huge number which won't impact the result (mod 8). 
		temp = EIGHT * GsbIeee754::floor(preCompute * ONE_O_HEIGHT);
		preCompute -= temp;
		// remove integer part
		n = static_cast<int32_t>(preCompute);
		preCompute -= static_cast<double>(n);
		qHighBits = 0;
		// if q is > 1
		if (q0Exponent > 0) {
			// need intQ[precision-1] to determine n
			// keep only q0Exponent bits (integer part)
			i = (intQ[precision - 1] >> (24 - q0Exponent));
			// add the integer part to n
			n += i;
			// Remove the integer part
			intQ[precision - 1] -= i << (24 - q0Exponent);
			// Retrieve exponent and sign
			qHighBits = intQ[precision - 1] >> (23 - q0Exponent);
		}
		else if (q0Exponent == 0) {
			// Retrieve sign
			qHighBits = intQ[precision - 1] >> 23;
		}
		else if (preCompute >= 0.5) {
			// Value reduced
			qHighBits = 2;
		}

		// q > 0.5 mean we still can reduce the value.
		if (qHighBits > 0) {
			n += 1;
			carry = 0;
			// compute 1-q and store it in intQ[]
			for (i = 0; i < precision; i++) {
				j = intQ[i];
				if (carry == 0) {
					if (j != 0) {
						carry = 1;
						intQ[i] = 0x1000000 - j;
					}
				}
				else {
					intQ[i] = 0xFFFFFF - j;
				}
			}
			if (q0Exponent > 0) {
				// rare case: chance is 1 in 12
				if (q0Exponent == 2) {
					// remove two highest bit
					intQ[precision - 1] &= 0x3FFFFF;
				}
				else {
					// remove highest bit
					intQ[precision - 1] &= 0x7FFFFF;
				}
			}
			// if preCompute >= 0.5
			if (qHighBits == 2) {
				preCompute = ONE - preCompute;
				if (carry != 0) {
					// Remove the integer part
					preCompute -= GsbIeee754::scalbn(ONE, q0Exponent);
				}
			}
		}

		// check if recomputation is needed, when the result's precision is not enough.
		/*>RELAX<Rule_2040_V3> Exact 0.0 is the only valid value that can trigger recomputation. */
		if (preCompute == ZERO) {
			// We need to reajust the number of digits used in the computation.
			j = 0;
			for (i = (precision - 1); i >= defaultPrecision; i--) {
				// If every element of intQ is ZERO
				j |= intQ[i];
			}
			// need recomputation
			if (j == 0) {
				for (k = 1; intQ[defaultPrecision - k] == 0; k++) {
					// empty loop, only increment k as the number of terms needed
					// skip the trailing zeros
				}

				// add q[precision+1] to q[precision+k]
				for (i = (precision + 1); i <= (precision + k); i++) {
					// complete doubleIpio2 and q arrays
					doubleIpio2[(inputDim - 1) + i] = static_cast<double>(I_PIO2[ipioIndex + i]);
					temp = 0.0;
					for (j = 0; j < inputDim; j++) {
						temp += input[j] * doubleIpio2[(inputDim - 1) + (i - j)];
					}
					q[i] = temp;
				}
				precision += k;
				// recompute with an increase of used terms
				recompute = true;
			}
		}
	} while (recompute);

	// chop off ZERO terms
	/*>RELAX<Rule_2040_V3> Approximated 0.0 or exact 0.0 are both value that we want to check in order to verify if an increase in precision is needed. */
	if (preCompute == ZERO) {
		q0Exponent -= 24;
		for (precision -= 1; ((precision >= 0) && (intQ[precision] == 0)); --precision) {
			q0Exponent -= 24;
		}
	}
	else {
		// break preCompute into 24-bit if necessary
		preCompute = GsbIeee754::scalbn(preCompute, -q0Exponent);
		if (preCompute >= TWO_24) {
			// If the value cannot be represented with 24 bits
			// Retrieve tailing element
			temp = static_cast<double>(static_cast<int32_t>(TWO_N24 * preCompute));
			intQ[precision] = static_cast<int32_t>(preCompute - (TWO_24 * temp));
			// Increment previous value by ONE (carry)
			precision += 1;
			q0Exponent += 24;
			// Add the tailing element
			intQ[precision] = static_cast<int32_t>(temp);
		}
		else {
			intQ[precision] = static_cast<int32_t>(preCompute);
		}
	}

	// convert integer "bit" chunk to floating-point value
	temp = GsbIeee754::scalbn(ONE, q0Exponent);
	for (i = precision; i >= 0; i--) {
		// q[] values multiplied by its corresponding exponent
		q[i] = temp * static_cast<double>(intQ[i]);
		temp *= TWO_N24;
	}

	// compute PIO2[0, ..., defaultPrecision] * q[precision, ..., 0]
	for (i = precision; i >= 0; i--) {
		temp = 0.0;
		for (k = 0; ((k <= defaultPrecision) && (k <= precision - i)); k++) {
			temp += PIO2[k] * q[i + k];
		}
		finalQ[precision - i] = temp;
	}

	// compress finalQ[] into ouput[]
	temp = 0.0;
	for (i = precision; i >= 0; i--) {
		temp += finalQ[i];
	}
	ouput[0] = (qHighBits == 0) ? temp : -temp;
	temp = finalQ[0] - temp;
	for (i = 1; i <= precision; i++) {
		temp += finalQ[i];
	}
	// Keep the small tailing part (same precision as input)
	ouput[1] = (qHighBits == 0) ? temp : -temp;

	// Return N mod 8
	return n & 7;
}
