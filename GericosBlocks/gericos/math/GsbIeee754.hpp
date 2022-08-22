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

 GsbIeee754.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 GsbIeee754.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines contains standard mathematical and bitwise functions on IEEE754 double or simple precision floating point numbers.

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 1 mars 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef GSBIEEE754_HPP
#define GSBIEEE754_HPP

#include <stdint.h>

namespace gericos {
	namespace blocks {
		namespace math {
			/**
			 * @author aamerge
			 * @version 1.0
			 * This class contains standard mathematical and bitwise functions on IEEE754 double or simple precision floating point numbers.
			 */
			class GsbIeee754 {
			private:
				/// Non functional copy constructor
				GsbIeee754(const GsbIeee754* other);
				/// Non functional copy constructor
				GsbIeee754(const GsbIeee754& other);
				/// Non functional overload of the assignment operator
				GsbIeee754& operator=(const GsbIeee754& other);

				/**
				 * @brief Utility function for expf(), compute the exponential of value, assuming prior verification of value are already done.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 * 
				 * @param[in] value The value to be exponentiated.
				 * @param[in] signValue The sign of the value.
				 * @param[in] highWordAbs The absolute value of the high word of the value.
				 * @return float The result of the exponentiation of value.
				 */
				static float expfUtility(float value, uint32_t signValue, uint32_t highWordAbs);

				/**
				 * @brief Utility function for floor(), handling values which exponent is in the range [20,51].
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 * 
				 * @param[in] value The value to be floored, which exposant is in [20,51].
				 * @param[in] highWord The 32 most significant bits of the value.
				 * @param[in] lowWord The 32 least significant bits of the value.
				 * @param[in] exponent The exponent part of the value, substract by 1023.
				 * @return double The value floored.
				 */
				static double floorUtility(double value, int32_t highWord, int32_t lowWord, int32_t exponent);

				/**
				 * @brief Utility function for scalbn(), compute x*2^n, assuming prior verification on the exponent has been performed.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 * 
				 * @param[in] x The value to be multiplied by 2^n.
				 * @param[in] n log2 of the value multiplied to x.
				 * @param[in] highWord The 32 most significant bits of x.
				 * @param[in] exponent The exponent part of x.
				 * @return double x * 2^n
				 */
				static double scalbnUtility(double x, int32_t n, uint32_t highWord, int32_t exponent);

				/** 
				 * @brief Utility function for getRemainderPio2(), will perform the computation of medium and large input.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 * 
				 * @param[in] input The input value to be processed.
				 * @param[in/out] output The remainder of input divided pi/2.
				 * @param[in] highWord The 32 most significant bits of input. Will be used to determine the sign of the input.
				 * @param[in] highWordAbs The absolute value of the high word of input. (highWord with sign bit set to 0).
				 * @return int32_t The number of time input can be divided by pi/2.
				 */
				static int32_t getRemainderPio2Utility(double input, double* output, int32_t highWord, int32_t highWordAbs);

				/**
				 * @brief Utility function for atan2(), will perform the computation of the atan2 function if x and/or y is INF, or if x and y are not 0, NaN or INF.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 * 
				 * @param[in] y The input value to be processed.
				 * @param[in] x The input value to be processed.
				 * @param[in] xHighWordAbs The absolute value of the high word of x.
				 * @param[in] yHighWordAbs The absolute value of the high word of y.
				 * @param[in] signOfXY Sign of x and y, can take different values {+y+x : 0, -y+x : 1, +y-x : 2, -y-x : 3}.
				 * @param[in] xHighWord The high word of x.
				 * @param[in] yHighWord The high word of y.
				 * @return double atan2(y,x)
				 */
				static double atan2Utility(double y, double x, int32_t xHighWordAbs, int32_t yHighWordAbs, int32_t signOfXY,
						int32_t xHighWord, int32_t yHighWord);

				/// The bitmask of log(FLT_MAX), rounded down.  This value is the largest input that can be passed to exp() without producing overflow.
				static const uint32_t FLT_UWORD_LOG_MAX = 0x42B17217U;
				/// The bitmask of |log(FLT_MIN)|, rounding down.
				static const uint32_t FLT_UWORD_LOG_MIN = 0x42CFF1B5U;

			public:
				/**
				 * @brief Get a 32 bit int from a float.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 *
				 * @param[in] value a float which will be converted to a 32 bit int bitwise.
				 * @return a 32 bits int, bitmask of value.
				 */
				static inline uint32_t getUwordFromFloat(float value) {
					// Cast a Float in a uint32_t bitwise.
					// cppcheck-suppress[invalidPointerCast] Pointer casting done to extract the binary representation of the float value without changing it.
					return *(reinterpret_cast<uint32_t*>(&value));
				}

				/**
				 * @brief True if a positive float with bitmask btmsk is not a number.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 *
				 * @param[in] btmsk the value to check
				 * @return True if btmsk is NaN.
				 */
				static inline bool floatUwordIsNan(uint32_t btmsk) {
					// The value NaN is represented when the exponent field is all ones and a mantissa that is not 0.
					return ((btmsk & 0x7FFFFFFFU) > 0x7F800000U);
				}

				/**
				 * @brief True if a positive float with bitmask btmsk is +infinity.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 *
				 * @param[in] btmsk the value to check
				 * @return True if btmsk is +Infinite
				 */
				static inline bool floatUwordIsInfinite(uint32_t btmsk) {
					// The infinite value is represented when exponent field is all ones
					// and a mantissa that is 0.
					return (btmsk == 0x7F800000U);
				}

				/**
				 * @brief Get a float from a 32 bit int.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 *
				 * @param[in] btmsk a 32 bit int which will be converted to a float bitwise.
				 * @return a float which bitmask is btmsk.
				 * @note Subnormal values will not be handled correctly.
				 */
				static inline float getFloatFromUword(uint32_t btmsk) {
					// Cast an int32_t to a float bitwise.
					// Subnormal values will not be handled correctly.
					// cppcheck-suppress[invalidPointerCast] Pointer casting done to extract the binary representation of the float value embedded in the uint32_t without changing it.
					return *(reinterpret_cast<float*>(&btmsk));
				}

				/**
				 * @brief Get the most significant 32 bit int of a double.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 *
				 * @param[in] value a double which the 32 most significant bits will be extracted
				 * @return the 32 most significant bits of value
				 */
				static inline uint32_t getHighWord(double value) {
					// Truncate the value with a cast (from a 64 bit value to a 32 bit).
					// cppcheck-suppress[invalidPointerCast] Pointer casting done to extract the binary representation of the double value without changing it.
					return *(reinterpret_cast<uint32_t*>(&value));
				}

				/**
				 * @brief Get the least significant 32 bits int of a double.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 *
				 * @param[in] value a double which the 32 least significant bits will be extracted
				 * @return the 32 least significant bits of value.
				 */
				static inline uint32_t getLowWord(double value) {
					// Truncate the value with a cast, and slide the pointer to 32 bit in order to reach the 32 least significant bits.
					// cppcheck-suppress[invalidPointerCast] Pointer casting done to extract the binary representation of the double value without changing it.
					return (*((reinterpret_cast<uint32_t*>(&value)) + 1U));
				}

				/**
				 * @brief Set the most significant 32 bits of a double from an int.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 *
				 * @param[in/out] value a double which its 32 most significant bits will be altered.
				 * @param[in] highWord a 32 bit int which will be set as the 32 most significant bits of value.
				 */
				static inline void setHighWord(double& value, uint32_t highWord) {
					// Cast value to uint64_t in order to retrieve his bitmask.
					// Set the low word of bitmaskX to highWord, then perform a left shift of 32 bit, in order to set highWord.
					// Then set back the low word of bitmaskX to the low word of value.
					// cppcheck-suppress[invalidPointerCast] Pointer casting done to extract the binary representation of the double value without changing it.
					uint64_t newBitmaskX = ((*(reinterpret_cast<uint64_t*>(&value)) & 0x00000000FFFFFFFFULL)
							| (static_cast<uint64_t>(highWord) << 32U));
					// Cast the result to a double, then return the result.
					// cppcheck-suppress[invalidPointerCast] Pointer casting done to extract the binary representation of the double value embedded in the uint64_t without changing it.
					value = *(reinterpret_cast<double*>(&newBitmaskX));
				}

				/**
				 * @brief Set the least significant 32 bits of a double from an int.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 *
				 * @param[in/out] value a double which its 32 least significant bits will be altered.
				 * @param[in] lowWord a 32 bit int which will be set as the 32 least significant bits of value.
				 */
				static inline void setLowWord(double& value, uint32_t lowWord) {
					// Cast value to uint64_t in order to retrieve his bitmask.
					// Set the low word of bitmaskX to 0 then perform a logical OR operation in order to set them to lowWord.
					// cppcheck-suppress[invalidPointerCast] Pointer casting done to extract the binary representation of the double value without changing it.
					uint64_t newBitmaskX = ((*(reinterpret_cast<uint64_t*>(&value)) & 0xFFFFFFFF00000000ULL) | static_cast<uint64_t>(lowWord));
					// Cast the value to a double, then return the result.
					// cppcheck-suppress[invalidPointerCast] Pointer casting done to extract the binary representation of the double value embedded in the uint64_t without changing it.
					value = *(reinterpret_cast<double*>(&newBitmaskX));
				}

				/**
				 * @brief Set a double from two 32 bit ints.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 *
				 * @param[out] value a double which will be set from the two 32 bits int
				 * @param[in] msb a 32 bit int which will be set as the 32 most significant bit of value.
				 * @param[in] lsb a 32 bit int which will be set as the 32 least significant bit of value.
				 */
				static inline void insertWords(double& value, uint32_t msb, uint32_t lsb) {
					// This functions will result in both a setHighWord and a setLowWord.
					uint64_t newBitmaskX = ((static_cast<uint64_t>(msb) << 32U) | lsb);
					// cppcheck-suppress[invalidPointerCast] Pointer casting done to extract the binary representation of the double value embedded in the uint64_t without changing it.
					value = *(reinterpret_cast<double*>(&newBitmaskX));
				}

				/**
				 * @brief Given a floating point value, return the exponential of that value.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 *
				 * @param[in] value The value to be exponentiated.
				 * @return a float, exponential of value.
				 * The input value shall be in the range ]-103.972084045, 88.7228317261[.
				 * Otherwise the result will be 0.0 or +Inf.
				 * If the input is NaN, the output shall be NaN too.
				 */
				static float expf(float value);

				/**
				 * @brief  Return value rounded toward -inf to integral value
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 *
				 * @param[in] value The value to be rounded.
				 * @return a double, floor of value.
				 */
				static double floor(double value);

				/**
				 * @brief Constructs a number with the magnitude (absolute value) of its first argument, <[magnitude]>, and the sign of its second argument, <[sign]>.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 *
				 * @param[in] magnitude magnitude of the result
				 * @param[in] sign sign of the result
				 * @return a number with the magnitude of magnitude, and the sign of sign
				 */
				static double copySign(double magnitude, double sign);

				/**
				 * @brief Return x * 2^n.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 *
				 * @param[in] x The value to be multiplied by 2^n.
				 * @param[in] n log2 of the value multiplied to x.
				 * @return double x * 2^n
				 */
				static double scalbn(double x, int32_t n);

				/**
				 * @brief kernel cos function on [-pi/4, pi/4], pi/4 ~ 0.785398164
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 *
				 * @param[in] x is assumed to be bounded by ~pi/4 in magnitude.
				 * @param[in] xTail is the tail of x.
				 * @return double
				 */
				static double kernelCos(double x, double xTail);

				/**
				 * @brief kernel sin function on [-pi/4, pi/4], pi/4 ~ 0.785398164
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 *
				 * @param[in] x is assumed to be bounded by ~pi/4 in magnitude.
				 * @param[in] xTail is the tail of x.
				 * @param[in] xTailIsZero indicates whether xTail is 0. (if xTailIsZero=0, xTail assume to be 0).
				 * @return double
				 */
				static double kernelSin(double x, double xTail, int32_t xTailIsZero);

				/**
				 * @brief reduce an angle to a multiple of pi/2 with [-pi/4, pi/4].
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 *
				 * @param[in] input the positive angle, broken into 24-bit chunks
				 * @param[out] output an array of 2 doubles modified to hold the remainder of input % pi/2.
				 * @param[in] inputExponent The exponent of input[0]
				 * @param[in] inputDim The last index used in input (<3)
				 * @return the octant of the result, mod 4 :
				 *  0:[-pi/4,   pi/4], 1:[  pi/4,  3pi/4], 2:[ 3pi/4,  5pi/4], 3:[ 5pi/4, 7pi/4],
				 *  4:[7pi/4, -7pi/4], 5:[-7pi/4, -5pi/4], 6:[-5pi/4, -3pi/4], 7:[-3pi/4, -pi/4]
				 */
				static int32_t getKernelRemainderPio2(double* input, double* output, int32_t inputExponent, int32_t inputDim);

				/** 
				 * @brief Return the remainder of input rem pi/2 in output buffer.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 * 
				 * @param[in] input the input angle.
				 * @param[out] output the remainder of input rem pi/2.
				 * @return int32_t The number of time input can be divided by pi/2.
				 */
				static int32_t getRemainderPio2(double input, double* output);

				/**
				 * @brief Return the cosine of x.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 * 
				 * @param[in] x the input value
				 * @return double cos(x)
				 */
				static double cos(double x);

				/**
				 * @brief Return the sin of x.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 * 
				 * @param[in] x the input value
				 * @return double sin(x)
				 */
				static double sin(double x);

				/**
				 * @brief Computes the inverse tangent (arc tangent) of the input value.
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 * 
				 * @param[in] x the input value
				 * @return double atan(x)
				 */
				static double atan(double x);

				/**
				 * @brief Compute the atan2(y,x).
				 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
				 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
				 * 
				 * @param y The input value
				 * @param x The input value
				 * @return double atan2(y,x)
				 */
				static double atan2(double y, double x);
			};
		}
	}
}

using gericos::blocks::math::GsbIeee754;
#endif /* GSBIEEE754_HPP */
