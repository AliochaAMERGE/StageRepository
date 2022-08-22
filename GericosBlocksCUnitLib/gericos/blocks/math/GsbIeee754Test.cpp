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

 GericosBlocksCUnitLib Component

 GsbIeee754Test.cpp

 $Rev: 000 $

 ************************************************************************/

#include "GsbIeee754Test.hpp"

#include <stdint.h>
#include <stdio.h>

#include <gericos/blocks/math/GsbIeee754.hpp>

#include "Automated.h"
#include "Basic.h"

// class to test

/**
 * The suite initialization function.
 * initializes the variables
 * Returns zero on success, non-zero otherwise.
 */
int initSuite_GsbIeee754Test(void) {
	return 0;
}

/**
 * The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int cleanSuite_GsbIeee754Test(void) {
	return 0;
}

/**
 * @brief  Test of GsbIeee754::getUwordFromFloat()
 * @pre    Initialize a float
 * @result The 32 bit int should represent the float's bit mask
 *
 * @UT_Purpose  Test the getUwordFromFloat function on different data
 * @UT_Tested_function uint32_t getUwordFromFloat(float value);
 * @UT_Type		Functionnality
 * @UT_Description	- Retrieve the bitmask of a float value
 * 					- Retrieve the bitmask of a negative value
 * 					- Retrieve the bitmask of a subnormalized value
 * 					- Retrieve the bitmask of a positive infinite value
 * 					- Retrieve the bitmask of a negative infinite value
 */
void GsbIeee754Test_getUwordFromFloat(void) {
	// arguments of tested function
	float value;
	// results of tested function
	uint32_t res;

	// Test 1 : Basic test
	value = 10.40023568;

	res = GsbIeee754::getUwordFromFloat(value);
	CU_ASSERT_EQUAL(res, 0x4126675E);

	// Test 2 : Basic Test negative value

	value = -10.42345;

	res = GsbIeee754::getUwordFromFloat(value);
	CU_ASSERT_EQUAL(res, 0xC126C674);

	// Test 3 : Basic Test with subnormalized value
	value = 1e-39;

	res = GsbIeee754::getUwordFromFloat(value);
	CU_ASSERT_EQUAL(res, 0x000AE398);

	// Test 4 : positive infinite
	value = 3e+39;

	res = GsbIeee754::getUwordFromFloat(value);
	CU_ASSERT_EQUAL(res, 0x7F800000);

	// Test 5 : negative infinite
	value = -3e+39;

	res = GsbIeee754::getUwordFromFloat(value);
	CU_ASSERT_EQUAL(res, 0xFF800000);
}

/**
 * @brief  Test of GsbIeee754::getFloatFromUword()
 * @pre    Initialize a uint32_t
 * @result The float should have the desired mask
 *
 * @UT_Purpose  Test the getFloatFromUword function on different data
 * @UT_Tested_function uint32_t getFloatFromUword(uint32_t btmsk);
 * @UT_Type		Functionnality
 * @UT_Description	- Return a simple float value
 * 					- Return a simple negative float value
 * 					- Return an infinite value
 * 					- Return a negative infinite value
 * 					- Return a NaN value
 */
void GsbIeee754Test_getFloatFromUword(void) {
	// arguments of tested function
	uint32_t btmsk;
	// results of tested function
	float res;

	// Basic test
	btmsk = 0;

	res = GsbIeee754::getFloatFromUword(btmsk);

	CU_ASSERT_EQUAL(res, 0);

	// Basic test with approximation and exact btmsk
	btmsk = 0x4126675E;

	res = GsbIeee754::getFloatFromUword(btmsk);

	CU_ASSERT_EQUAL(res, 10.4002361297607421875);		 // Exact value stored
	CU_ASSERT_DOUBLE_EQUAL(res, 10.40023568, 1e-6);	 // Approximate value

	// Basic Test negative value

	btmsk = 0xC126C674;

	res = GsbIeee754::getFloatFromUword(btmsk);
	CU_ASSERT_DOUBLE_EQUAL(res, -10.42345047, 1e-6);

	// Positive infinite
	btmsk = 0x7F800000;

	res = GsbIeee754::getFloatFromUword(btmsk);
	// btmsk is cast back to its bitmask in order to check if it has been altered.
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint32_t*>(&res)), 0x7F800000);

	// Negative infinite
	btmsk = 0xFF800000;

	res = GsbIeee754::getFloatFromUword(btmsk);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint32_t*>(&res)), 0xFF800000);

	// NaN
	btmsk = 0x7f800001;

	res = GsbIeee754::getFloatFromUword(btmsk);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint32_t*>(&res)), 0x7f800001);
}

/**
 * @brief  Test of GsbIeee754::floatUwordIsNan(uint32_t btmsk)
 * @pre   Initialize a uint32_t
 * @result True if the float is NaN
 *
 * @UT_Purpose  Test the floatUwordIsNan function on different data
 * @UT_Tested_function uint32_t floatUwordIsNan(uint32_t btmsk);
 * @UT_Type		Functionnality
 * @UT_Description	- Check if a basic positive value is NaN
 * 					- Check if a basic negative value is NaN
 * 					- Check if NaN is NaN
 * 					- Check if +Infinite value is NaN
 * 					- Check if -Infinite value is NaN
 */
void GsbIeee754Test_floatUwordIsNan(void) {
	// arguments of tested function
	uint32_t btmsk;

	// Positive number --> False
	btmsk = 0x4126c674;
	CU_ASSERT_FALSE(GsbIeee754::floatUwordIsNan(btmsk));

	// Negative number --> False
	btmsk = 0xC126C674;
	CU_ASSERT_FALSE(GsbIeee754::floatUwordIsNan(btmsk));

	// NaN --> True
	btmsk = 0x7f800001;
	CU_ASSERT_TRUE(GsbIeee754::floatUwordIsNan(btmsk));

	// +Infinite --> False
	btmsk = 0x7F800000;
	CU_ASSERT_FALSE(GsbIeee754::floatUwordIsNan(btmsk));

	// -Infinite --> False
	btmsk = 0xFF800000;
	CU_ASSERT_FALSE(GsbIeee754::floatUwordIsNan(btmsk));
}

/**
 * @brief  Test of GsbIeee754::floatUwordIsInfinite(uint32_t btmsk)
 * @pre initialize a uint32_t
 * @result True if the float is +Infinite
 *
 * @UT_Purpose  Test the floatUwordIsInfinite function on different data
 * @UT_Tested_function uint32_t floatUwordIsInfinite(uint32_t btmsk);
 * @UT_Type		Functionnality
 * @UT_Description	- Check if a basic positive value is +Infinite
 * 					- Check if a basic negative value is +Infinite
 * 					- Check if NaN is +Infinite
 * 					- Check if +Infinite value is +Infinite
 * 					- Check if -Infinite value is +Infinite
 */
void GsbIeee754Test_floatUwordIsInfinite(void) {
	// arguments of tested function
	uint32_t btmsk;

	// Positive number --> False
	btmsk = 0x4126c674;
	CU_ASSERT_FALSE(GsbIeee754::floatUwordIsInfinite(btmsk));

	// Negative number --> False
	btmsk = 0xC126C674;
	CU_ASSERT_FALSE(GsbIeee754::floatUwordIsInfinite(btmsk));

	// NaN --> False
	btmsk = 0x7f800001;
	CU_ASSERT_FALSE(GsbIeee754::floatUwordIsInfinite(btmsk));

	// +Infinite --> True
	btmsk = 0x7F800000;
	CU_ASSERT_TRUE(GsbIeee754::floatUwordIsInfinite(btmsk));

	// -Infinite --> False
	btmsk = 0xFF800000;
	CU_ASSERT_FALSE(GsbIeee754::floatUwordIsInfinite(btmsk));
}

/**
 * @brief  Test of GsbIeee754::getHighWord(double value)
 * @pre initialize a double
 * @result return the 32 most significant bit of value
 *
 * @UT_Purpose  Test the getHighWord function on different data
 * @UT_Tested_function uint32_t getHighWord(double value);
 * @UT_Type		Functionnality
 * @UT_Description	- Retrieve the High Word of a value encoded on 64 bit (larger than a float)
 * 					- Retrieve the High Word of a basic positive value
 * 					- Retrieve the High Word of a basic negative value
 * 					- Retrieve the High Word of a positive infinite value
 * 					- Retrieve the High Word of a negative infinite value
 */
void GsbIeee754Test_getHighWord(void) {
	// arguments of tested function
	double value;
	// results of tested function
	uint32_t res;

	// Basic test larger than a float
	value = 3e123;

	res = GsbIeee754::getHighWord(value);
	CU_ASSERT_EQUAL(res, 0x5992271C);

	// Basic test
	value = 10.40023568;

	res = GsbIeee754::getHighWord(value);
	CU_ASSERT_EQUAL(res, 0x4024CCEB);

	// Basic Test negative value
	value = -10.42345;

	res = GsbIeee754::getHighWord(value);
	CU_ASSERT_EQUAL(res, 0xC024D8CE);

	// Positive infinite
	uint64_t valBtmsk = 0x7FF0000000000000ULL;
	value = (*(reinterpret_cast<double*>(&valBtmsk)));

	res = GsbIeee754::getHighWord(value);
	CU_ASSERT_EQUAL(res, 0x7FF00000);

	// Negative infinite
	valBtmsk = 0xFFF0000000000000ULL;
	value = (*(reinterpret_cast<double*>(&valBtmsk)));

	res = GsbIeee754::getHighWord(value);
	CU_ASSERT_EQUAL(res, 0xFFF00000);
}

/**
 * @brief  Test of GsbIeee754::getLowWord(double value)
 * @pre initialize a double
 * @result return the 32 least significant bit of value
 *
 * @UT_Purpose  Test the getLowWord function on different data
 * @UT_Tested_function uint32_t getLowWord(double value);
 * @UT_Type		Functionnality
 * @UT_Description	- Retrieve the Low Word of a basic positive value
 * 					- Retrieve the Low Word of a basic negative value
 * 					- Retrieve the Low Word of a positive infinite value
 * 					- Retrieve the Low Word of a negative infinite value
 */
void GsbIeee754Test_getLowWord(void) {
	// arguments of tested function
	double value;
	// results of tested function
	uint32_t res;

	// Basic test
	value = 10.40023568;

	res = GsbIeee754::getLowWord(value);
	CU_ASSERT_EQUAL(res, 0xB0E895AB);
	// Double check in order to make sure that the function do not alter the value.
	res = GsbIeee754::getLowWord(value);
	CU_ASSERT_EQUAL(res, 0xB0E895AB);

	// Basic Test negative value
	value = -10.42345;

	res = GsbIeee754::getLowWord(value);
	CU_ASSERT_EQUAL(res, 0x703AFB7F);

	// Positive infinite
	uint64_t valBtmsk = 0x7FF0000000000000ULL;
	value = (*(reinterpret_cast<double*>(&valBtmsk)));

	res = GsbIeee754::getLowWord(value);
	CU_ASSERT_EQUAL(res, 0);

	// Negative infinite
	valBtmsk = 0xFFF0000000000000ULL;
	value = (*(reinterpret_cast<double*>(&valBtmsk)));

	res = GsbIeee754::getLowWord(value);
	CU_ASSERT_EQUAL(res, 0);
}

/**
 * @brief  Test of GsbIeee754::setHighWord(double& value, uint32_t highWord)
 * @pre initialize a double (value) and a uint32_t (highWord) (high word to set)
 * @result the double value should have its high word set to highWord
 *
 * @UT_Purpose  Test the setHighWord function on different data
 * @UT_Tested_function void setHighWord(double& value, uint32_t highWord);
 * @UT_Type		Functionnality
 * @UT_Description	- Set the high word of a basic positive value
 * 					- Set the high word of a basic negative value
 * 					- Set the high word of an infinite value
 */
void GsbIeee754Test_setHighWord(void) {
	// arguments of tested function
	double value;
	uint32_t highWord;

	// Basic test
	value = 10.40023568;	// 0x 4024CCEB B0E895AB
	highWord = 0xA110C11A;

	GsbIeee754::setHighWord(value, highWord);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint32_t*>((&value))), highWord);

	// Basic test negative value
	value = -10.40023568;
	highWord = 0xA110C11A;

	GsbIeee754::setHighWord(value, highWord);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint32_t*>((&value))), highWord);

	// Positive infinite
	value = 3e129;
	highWord = 0xA110C11A;

	GsbIeee754::setHighWord(value, highWord);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint32_t*>((&value))), highWord);
}

/**
 * @brief  Test of GsbIeee754::setLowWord(double& value, uint32_t lowWord)
 * @pre initialize a double (value) and a uint32_t (lowWord) (low word to set)
 * @result the double value should have its low word set to lowWord
 *
 * @UT_Purpose  Test the setLowWord function on different data
 * @UT_Tested_function void setLowWord(double& value, uint32_t lowWord);
 * @UT_Type		Functionnality
 * @UT_Description	- Set the low word of a basic positive value
 * 					- Set the low word of a basic negative value
 * 					- Set the low word of an infinite value
 */
void GsbIeee754Test_setLowWord(void) {
	// arguments of tested function
	double value;
	uint32_t lowWord;

	// Basic test
	value = 10.40023568;	// 0x 4024CCEB B0E895AB
	lowWord = 0xA110C11A;

	GsbIeee754::setLowWord(value, lowWord);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint32_t*>(&value) + 1), lowWord);

	// Basic test neative value
	value = -10.40023568;
	lowWord = 0xA110C11A;

	GsbIeee754::setLowWord(value, lowWord);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint32_t*>(&value) + 1), lowWord);

	// Positive infinite
	value = 3e129;
	lowWord = 0xA110C11A;

	GsbIeee754::setLowWord(value, lowWord);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint32_t*>(&value) + 1), lowWord);
}

/**
 * @brief  Test of GsbIeee754::insertWords(double& value, uint32_t lsw, uint32_t hsw)
 * @pre initialize a double (value) two uint32_t (hsw, lsw) which will be the low word (resp. high word) of value.
 * @result the double value should have its high word set to v
 *
 * @UT_Purpose  Test the insertWords function on different data
 * @UT_Tested_function void insertWords(double& value, uint32_t lsw, uint32_t hsw);
 * @UT_Type		Functionnality
 * @UT_Description	- Build a new value from a basic positive value
 * 					- Build a new value from a basic negative value
 * 					- Build a new value from a Infinite value
 */
void GsbIeee754Test_insertWords(void) {
	// arguments of tested function
	double value;
	uint32_t lsw;
	uint32_t hsw;
	// Input data
	double in;

	// Basic test
	value = 0;
	in = 10.40023568;	 // 0x 4024CCEB B0E895AB
	hsw = 0x4024CCEB;
	lsw = 0xB0E895AB;

	GsbIeee754::insertWords(value, hsw, lsw);

	CU_ASSERT_EQUAL(value, in);

	// Basic Test negative value
	value = 1e308;
	in = -10.42345;	 // 0x C024D8CE 703AFB7F
	hsw = 0xC024D8CE;
	lsw = 0x703AFB7F;

	GsbIeee754::insertWords(value, hsw, lsw);

	CU_ASSERT_EQUAL(value, in);

	// Positive infinite
	value = -12.102135;
	in = 3e39;	// 0x 4821A1E5 F7753796
	hsw = 0x4821A1E5;
	lsw = 0xF7753796;

	GsbIeee754::insertWords(value, hsw, lsw);

	CU_ASSERT_EQUAL(value, in);
}

/**
 * @brief  Test of GsbIeee754::expf()
 * @pre    Initialize a float value
 * @result  Should return the exponential of value, or NaN, infinite, 0 if desired
 *
 * @UT_Purpose			Test the expf function with severals values.
 * @UT_Tested_function	float expf(float value);
 * @UT_Type				Functionality
 * @UT_Description		Test the expf() function with several values.
 * 						- Tests with basic value (positive, negative or float)
 * 						- Tests with values close to the limits (FLOT_UWORD_LOG_MAX, FLOT_UWORD_LOG_MIN)
 * 						- Tests with +Inf, -Inf, NaN values
 * 						- Tests with max/min float values
 * 						- Tests with values close to 0
 * 						- Tests with result close to 1
 */
void GsbIeee754Test_expf(void) {
	// arguments of tested function
	float value;

	// return of tested function
	float res;
	uint32_t valBtmsk;	// input value bitmask, used when input is infinite or NaN value
	uint32_t resBtmsk;	// result bitmask used when expf() return infinite and NaN value

	// Basic test
	value = 2.0f;
	res = GsbIeee754::expf(value);
	// Value obtained with calculator : 7.3890560989
	// Precision calculated with the IEEE-754 Floating Point Converter (source :
	// https://www.h-schmidt.net/FloatConverter/IEEE754.html)
	CU_ASSERT_DOUBLE_EQUAL(res, 7.3890560989f, 2.0574951171875e-7f);

	// exp(0) = 1, the only exact value returned
	value = 0.0f;
	res = GsbIeee754::expf(value);
	// exp(0) = 1
	CU_ASSERT_EQUAL(res, 1.0f);

	// Another basic test
	value = 1.0f;
	res = GsbIeee754::expf(value);
	// Precision calculated with the IEEE-754 Floating Point Converter (source :
	// https://www.h-schmidt.net/FloatConverter/IEEE754.html) value obtained with calculator : 2.7182818284
	CU_ASSERT_DOUBLE_EQUAL(res, 2.7182818284f, -8.248935546875e-8f);

	// Test with a floating point value
	value = 8.171875f;
	res = GsbIeee754::expf(value);
	// Precision calculated with the IEEE-754 Floating Point Converter (source :
	// https://www.h-schmidt.net/FloatConverter/IEEE754.html) value obtained with calculator :3539.97509765625
	CU_ASSERT_EQUAL(res, 3539.97509765625f);

	// Test with value as the largest input that can be passed to exp() without producing overflow.
	valBtmsk = GsbIeee754::FLT_UWORD_LOG_MAX;	 // 0x42B17217 = 88.7228317261
	value = (*reinterpret_cast<float*>(&valBtmsk));
	res = GsbIeee754::expf(value);
	// Precision calculated with the IEEE-754 Floating Point Converter (source :
	// https://www.h-schmidt.net/FloatConverter/IEEE754.html) value obtained with calculator : 3.40279853e38
	CU_ASSERT_DOUBLE_EQUAL(res, 3.40279853e+38f, -1.097852389343757962027391255e+30f);

	// Test with value as the lowest input that can be passed to exp() without producing underflow
	valBtmsk = GsbIeee754::FLT_UWORD_LOG_MIN;									 // 0x42CFF1B5 = 103.972084045
	value = (-1.0f) * (*reinterpret_cast<float*>(&valBtmsk));	 // = -103.972084045 (error : -4.1015625E-10)
	res = GsbIeee754::expf(value);
	// value obtained with calculator : 7.0064435e-46 --> subnormal --> 0
	CU_ASSERT_EQUAL(res, 0.0f);

	// Should trigger an underflow
	value = -1e30f;
	res = GsbIeee754::expf(value);
	resBtmsk = (*(reinterpret_cast<uint32_t*>(&res)));
	CU_ASSERT_EQUAL(res, 0.0f);	 // should be an underflow

	// Small value
	value = -67.897552490234375f;
	res = GsbIeee754::expf(value);
	// Precision calculated with the IEEE-754 Floating Point Converter (source :
	// https://www.h-schmidt.net/FloatConverter/IEEE754.html) value obtained with calculator : 0x0E84035D
	// ~  3.254375106240212e-30
	CU_ASSERT_DOUBLE_EQUAL(res, 3.254375106240212e-30f, 1.0e-38f);

	// Less than FLT_UWORD_LOG_MIN
	value = -105.0f;	/// less than FLT_UWORD_LOG_MIN
	res = GsbIeee754::expf(value);
	resBtmsk = (*(reinterpret_cast<uint32_t*>(&res)));
	CU_ASSERT_EQUAL(res, 0.0f);	 // 2.5065674e-46 --> should be an underflow and return 0

	// check the result
	value = 100.0f;	 /// More than FLT_UWORD_LOG_MAX
	res = GsbIeee754::expf(value);
	resBtmsk = (*(reinterpret_cast<uint32_t*>(&res)));
	CU_ASSERT_EQUAL(resBtmsk, 0x7F800000);	// 2.68811714e43 --> should be an overflow and return infinite

	// check the result
	valBtmsk = 0x7FFF0000;	// NaN mask
	value = *(reinterpret_cast<float*>(&valBtmsk));
	res = GsbIeee754::expf(value);
	resBtmsk = (*(reinterpret_cast<uint32_t*>(&res)));
	CU_ASSERT_EQUAL(resBtmsk, 0x7FFF0000);	// NaN --> should return NaN

	// check the result
	valBtmsk = 0x7F800000;	/// + Infinite mask
	value = *(reinterpret_cast<float*>(&valBtmsk));
	res = GsbIeee754::expf(value);
	resBtmsk = (*(reinterpret_cast<uint32_t*>(&res)));
	CU_ASSERT_EQUAL(resBtmsk, 0x7F800000);	// +Infinite --> should return +Infinite

	// check the result
	valBtmsk = 0xFF800000;	/// -Infinite mask
	value = *(reinterpret_cast<float*>(&valBtmsk));
	res = GsbIeee754::expf(value);
	CU_ASSERT_EQUAL(res, 0.0f);	 // -Infinite --> should return 0

	// check the result
	value = 0.2f;	 // < 0.5 * ln(2)
	res = GsbIeee754::expf(value);
	// Precision calculated with the IEEE-754 Floating Point Converter (source :
	// https://www.h-schmidt.net/FloatConverter/IEEE754.html) value obtained with calculator : 1.2214027581
	CU_ASSERT_DOUBLE_EQUAL(res, 1.2214027581f, 6.22037353515625e-9f);

	// check the result
	value = 3.7252900e-9f;	// 0x317FFFFF
	res = GsbIeee754::expf(value);
	// Precision calculated with the IEEE-754 Floating Point Converter (source :
	// https://www.h-schmidt.net/FloatConverter/IEEE754.html) value obtained with calculator : 1.0000000037253 stored as 1
	// due to maximum float precision (see above)
	CU_ASSERT_EQUAL(res, 1.0f);

	// check the result
	value = -100.0f;	//  k <= -125
	res = GsbIeee754::expf(value);
	// Precision calculated with the IEEE-754 Floating Point Converter (source :
	// https://www.h-schmidt.net/FloatConverter/IEEE754.html) value obtained with calculator : 	3.7200759e-44 -->
	// subnormal
	// --> 0
	CU_ASSERT_EQUAL(res, 0.0f);

	// check the result
	value = -87.33f;	//  k <= -125 and result > MIN_FLOAT (normalized)
	res = GsbIeee754::expf(value);
	// Precision calculated with the IEEE-754 Floating Point Converter (source :
	// https://www.h-schmidt.net/FloatConverter/IEEE754.html) For this test, the error due to conversion
	// is 4.289363503287709153213996486E-46 which result in a floating constant out of range. For this test, the
	// granularity will be increased to the minimum value of a float value obtained with calculator : 1.1832128e-38
	CU_ASSERT_DOUBLE_EQUAL(res, 1.1832128e-38f, 1.17549435082e-38f);

	// Same test as above, but with a increased precision.
	value = -87.336544750555f;
	res = GsbIeee754::expf(value);
	// The granularity will be set to the minimum value of a floating point value.
	// value obtained with calculator : 1.1754943e-38
	CU_ASSERT_DOUBLE_EQUAL(res, 1.1754943e-38f, 1.1754943e-38f);

	// Test a result close to 1, but writable in a float
	value = 1.0430812835693359375e-7f;
	res = GsbIeee754::expf(value);
	// The granularity will be set to the minimum value of a floating point value.
	// value obtained with calculator :1.0000001043081
	CU_ASSERT_EQUAL(res, 1.00000011920928955078125f);

	value = 3.4657356e-1f;
	res = GsbIeee754::expf(value);
	// 0x3FB504F3
	CU_ASSERT_EQUAL(res, 1.41421353816986083984375f);

	value = -3.4657356e-1f;
	res = GsbIeee754::expf(value);
	// 0x3F3504F4
	CU_ASSERT_EQUAL(res, 0.7071068286895751953125f);

	value = 3.4657359e-1f;
	res = GsbIeee754::expf(value);
	// 0x3FB504F3
	CU_ASSERT_EQUAL(res, 1.41421353816986083984375f);

	value = -3.4657359e-1f;
	res = GsbIeee754::expf(value);
	// 0x3F3504F3
	CU_ASSERT_EQUAL(res, 0.707106769085f);

	value = 3.4657362e-1f;
	res = GsbIeee754::expf(value);
	// 0x3FB504F4
	CU_ASSERT_EQUAL(res, 1.41421365738f);

	value = -3.4657362e-1f;
	res = GsbIeee754::expf(value);
	// 0x3F3504F3
	CU_ASSERT_EQUAL(res, 0.707106769085f);

	value = 1.0397206f;
	res = GsbIeee754::expf(value);
	// 0x403504F2
	CU_ASSERT_EQUAL(res, 2.82842683792f);

	value = -1.0397206f;
	res = GsbIeee754::expf(value);
	// 0x3EB504F4
	CU_ASSERT_EQUAL(res, 0.353553414345f);

	value = 1.0397207f;
	res = GsbIeee754::expf(value);
	// 0x403504F3
	CU_ASSERT_EQUAL(res, 2.828426837921143f);

	value = -1.0397207f;
	res = GsbIeee754::expf(value);
	// 0x3EB504F3
	CU_ASSERT_EQUAL(res, 0.353553414344788f);

	value = 1.0397208f;
	res = GsbIeee754::expf(value);
	// 0x403504F4
	CU_ASSERT_EQUAL(res, 2.828427076339722f);

	value = -1.0397208f;
	res = GsbIeee754::expf(value);
	// 0x3EB504F2
	CU_ASSERT_EQUAL(res, 0.353553384542465f);

	value = 3.7252900e-9f;
	res = GsbIeee754::expf(value);
	// 0x3F800000
	CU_ASSERT_EQUAL(res, 1.0f);

	value = -3.7252900e-9f;
	res = GsbIeee754::expf(value);
	// 0x3F800000
	CU_ASSERT_EQUAL(res, 1.0f);

	value = 3.7252902e-9f;
	res = GsbIeee754::expf(value);
	// 0x3F800000
	CU_ASSERT_EQUAL(res, 1.0f);

	value = -3.7252902e-9f;
	res = GsbIeee754::expf(value);
	// 0x3F800000
	CU_ASSERT_EQUAL(res, 1.0f);

	value = 3.7252907e-9f;
	res = GsbIeee754::expf(value);
	// 0x3F800000
	CU_ASSERT_EQUAL(res, 1.0f);

	value = -3.7252907e-9f;
	res = GsbIeee754::expf(value);
	// 0x3F800000
	CU_ASSERT_EQUAL(res, 1.0f);

	value = 0.5f;
	res = GsbIeee754::expf(value);
	// 0x3FD3094C
	CU_ASSERT_EQUAL(res, 1.64872121811f);

	value = -0.5f;
	res = GsbIeee754::expf(value);
	// 0x3F1B4598
	CU_ASSERT_EQUAL(res, 0.606530666351f);

	value = 6.789755e1f;
	res = GsbIeee754::expf(value);
	// 0x707837BC
	CU_ASSERT_EQUAL(res, 3.0727864464268464e29f);

	value = -6.789755e1f;
	res = GsbIeee754::expf(value);
	CU_ASSERT_EQUAL(res, 3.25437510624021225398e-30f);

	value = 3.19015e38f;
	res = GsbIeee754::expf(value);
	// INF
	resBtmsk = (*(reinterpret_cast<uint32_t*>(&res)));
	CU_ASSERT_EQUAL(resBtmsk, 0x7F800000);

	value = -3.19015e38f;
	res = GsbIeee754::expf(value);
	// 0x00000000
	CU_ASSERT_EQUAL(res, 0.0f);
}

/**
 * @brief  	Test of GsbIeee754::floor()
 * @pre		Intialize a double value
 * @result	Should return the value rounded toward - infinite
 *
 * @UT_Purpose			Test the floor function with several inputs.
 * @UT_Tested_function	double floor(double value);
 * @UT_Type				Functionality
 * @UT_Description		- Tests with +Inf, -Inf, NaN values
 * 						- Tests with +-0.0 +-1.0
 * 						- Tests with basic value (positive, negative or floating)
 * 						- Tests with max/min double values
 * 						- Tests with Several exponent for code coverage
 */
void GsbIeee754Test_floor(void) {
	// arguments of tested function
	double value;

	uint64_t valBtmsk;
	uint64_t resBtmsk;

	// return of tested function
	double res;

	// Special cases

	// + Infinite
	valBtmsk = 0x7FF0000000000000ULL;	 /// + Infinite mask
	value = *(reinterpret_cast<double*>(&valBtmsk));
	res = GsbIeee754::floor(value);
	resBtmsk = (*(reinterpret_cast<uint64_t*>(&res)));
	CU_ASSERT_EQUAL(resBtmsk, 0x7FF0000000000000ULL);	 // +Infinite --> should return +Infinite

	// - Infinite
	valBtmsk = 0xFFF0000000000000ULL;	 /// - Infinite mask
	value = *(reinterpret_cast<double*>(&valBtmsk));
	res = GsbIeee754::floor(value);
	resBtmsk = (*(reinterpret_cast<uint64_t*>(&res)));
	CU_ASSERT_EQUAL(resBtmsk, 0xFFF0000000000000ULL);	 // - Infinite --> should return - Infinite

	// NaN
	valBtmsk = 0x7FFE000000000000ULL;	 // NaN mask
	value = *(reinterpret_cast<double*>(&valBtmsk));
	res = GsbIeee754::floor(value);
	resBtmsk = (*(reinterpret_cast<uint64_t*>(&res)));
	CU_ASSERT_EQUAL(resBtmsk, 0x7FFE000000000000ULL);	 // NaN --> should return NaN

	// +0
	value = +0.0;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, +0.0);

	// -0
	value = -0.0;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, -0.0);

	// Nominal cases

	value = 0.1e-12;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, 0.0);

	value = 0.234651e-4;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, 0.0);

	value = 0.5;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, 0.0);

	value = 0.9999999999999;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, 0.0);

	value = -0.1e-12;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, -1.0);
	value = -0.234651e-4;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, -1.0);

	value = -0.5;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, -1.0);

	value = -0.9999999999999;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, -1.0);

	value = 1.0;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, 1.0);

	value = -1.0;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, -1.0);

	value = 4.564319321;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, 4.0);

	value = -4.564319321;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, -5.0);

	value = 3.544694e2;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, 354.0);

	value = -3.544694e2;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, -355.0);

	value = 1e308;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, 1e308);

	value = -1e308;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, -1e308);

	value = 3.141592653589793115997963468544185161590576171875e25;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, 3.1415926535897931159979634e25);

	value = -3.141592653589793115997963468544185161590576171875e25;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, -3.1415926535897931159979635e25);

	value = 3.141592653589793115997963468544185161590576171875e20;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, 3.14159265358979311599e20);

	value = -3.141592653589793115997963468544185161590576171875e20;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, -3.14159265358979311600e20);

	value = 3.141592653589793115997963468544185161590576171875e12;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, 3.141592653589e12);

	value = -3.141592653589793115997963468544185161590576171875e12;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, -3141592653590.0);

	value = 1648639.1981;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, 1648639);

	value = -1648639.1981;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, -1648640.0);

	value = 1648639.0;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, 1648639);

	value = -1648639.0;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, -1648639.0);

	value = -3141594513407.9995;
	res = GsbIeee754::floor(value);
	CU_ASSERT_EQUAL(res, -3141594513408.0);
}

/**
 * @brief  	Test of GsbIeee754::copySign()
 * @pre		Intialize two double values (magnitude and sign)
 * @result	Should return a number with the magnitude of the first parameter, and the sign of the second parameter.
 *
 * @UT_Purpose			Test the copySign function with several inputs.
 * @UT_Tested_function	double copySign(double magnitude, double sign);
 * @UT_Type				Functionality
 * @UT_Description		- Tests with every combination of +-Inf, +-NaN, +-0.0, +-1.0, maxDouble and minDouble
 */
void GsbIeee754Test_copySign(void) {
	// arguments of tested function
	double magnitude;
	double sign;

	// +Infinite double mask
	uint64_t plusInfinityBtmsk = 0x7FF0000000000000ULL;
	// - Infinite double mask
	uint64_t minusInfinityBtmsk = 0xFFF0000000000000ULL;
	// NaN value mask, with sign bit set to 0
	uint64_t plusNaNBtmsk = 0x7FFE000000000000ULL;
	// NaN value mask, with sign bit set to 1
	uint64_t minusNaNBtmsk = 0xFFFE000000000000ULL;

	// return of tested function
	double res;

	// Special cases

	// NaN, +Inf --> NaN
	magnitude = *(reinterpret_cast<double*>(&plusNaNBtmsk));
	sign = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), plusNaNBtmsk);

	// NaN, -Inf --> -NaN
	magnitude = *(reinterpret_cast<double*>(&plusNaNBtmsk));
	sign = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), minusNaNBtmsk);

	// NaN, NaN --> NaN
	magnitude = *(reinterpret_cast<double*>(&plusNaNBtmsk));
	sign = *(reinterpret_cast<double*>(&plusNaNBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), plusNaNBtmsk);

	// -NaN, NaN --> NaN
	magnitude = *(reinterpret_cast<double*>(&plusNaNBtmsk));
	sign = *(reinterpret_cast<double*>(&minusNaNBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), minusNaNBtmsk);

	// NaN, 0 --> NaN
	magnitude = *(reinterpret_cast<double*>(&plusNaNBtmsk));
	sign = 0.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), plusNaNBtmsk);

	// NaN, -0 --> -NaN
	magnitude = *(reinterpret_cast<double*>(&plusNaNBtmsk));
	sign = -0.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), minusNaNBtmsk);

	// NaN, 1 --> NaN
	magnitude = *(reinterpret_cast<double*>(&plusNaNBtmsk));
	sign = 1.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), plusNaNBtmsk);

	// NaN, -1 --> -NaN
	magnitude = *(reinterpret_cast<double*>(&plusNaNBtmsk));
	sign = -1.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), minusNaNBtmsk);

	// Nominal cases

	// +Inf, +Inf --> +Inf
	magnitude = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	sign = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), plusInfinityBtmsk);

	// +Inf, -Inf --> -Inf
	magnitude = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	sign = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), minusInfinityBtmsk);

	// +Inf, NaN --> +Inf
	magnitude = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	sign = *(reinterpret_cast<double*>(&plusNaNBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), plusInfinityBtmsk);

	// +Inf, 0.0 --> +Inf
	magnitude = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	sign = 0.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), plusInfinityBtmsk);

	// +Inf, -0.0 --> -Inf
	magnitude = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	sign = -0.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), minusInfinityBtmsk);

	// -Inf, +Inf --> +Inf
	magnitude = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	sign = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), plusInfinityBtmsk);

	// -Inf, -Inf --> -Inf
	magnitude = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	sign = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), minusInfinityBtmsk);

	// -Inf, NaN --> +Inf
	magnitude = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	sign = *(reinterpret_cast<double*>(&plusNaNBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), plusInfinityBtmsk);

	// -Inf, 0.0 --> +Inf
	magnitude = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	sign = 0.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), plusInfinityBtmsk);

	// -Inf, -0.0 --> -Inf
	magnitude = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	sign = -0.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), minusInfinityBtmsk);

	// 0.0, +Inf --> 0.0
	magnitude = 0.0;
	sign = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 0.0);

	// 0.0, -Inf --> -0.0
	magnitude = 0.0;
	sign = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, -0.0);

	// 0.0, NaN --> 0.0
	magnitude = 0.0;
	sign = *(reinterpret_cast<double*>(&plusNaNBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 0.0);

	// 0.0, -NaN --> -0.0
	magnitude = 0.0;
	sign = *(reinterpret_cast<double*>(&minusNaNBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, -0.0);

	// 0.0, 0.0 --> 0.0
	magnitude = 0.0;
	sign = 0.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 0.0);

	// 0.0, -0.0 --> -0.0
	magnitude = 0.0;
	sign = -0.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, -0.0);

	// 0.0, 1.0 --> 0.0
	magnitude = 0.0;
	sign = 1.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 0.0);

	// 0.0, -1.0 --> -0.0
	magnitude = 0.0;
	sign = -1.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, -0.0);

	// -0.0, +Inf --> 0.0
	magnitude = -0.0;
	sign = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 0.0);

	// -0.0, -Inf --> -0.0
	magnitude = -0.0;
	sign = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, -0.0);

	// -0.0, NaN --> 0.0
	magnitude = -0.0;
	sign = *(reinterpret_cast<double*>(&plusNaNBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 0.0);

	// -0.0, -NaN --> -0.0
	magnitude = -0.0;
	sign = *(reinterpret_cast<double*>(&minusNaNBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, -0.0);

	// -0.0, 0.0 --> 0.0
	magnitude = -0.0;
	sign = 0.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 0.0);

	// -0.0, -0.0 --> -0.0
	magnitude = -0.0;
	sign = -0.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, -0.0);

	// -0.0, 1.0 --> 0.0
	magnitude = -0.0;
	sign = 1.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 0.0);

	// -0.0, -1.0 --> -0.0
	magnitude = -0.0;
	sign = -1.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, -0.0);

	// 1.0, +Inf --> 1.0
	magnitude = 1.0;
	sign = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 1.0);

	// 1.0, -Inf --> -1.0
	magnitude = 1.0;
	sign = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, -1.0);

	// 1.0, NaN --> 1.0
	magnitude = 1.0;
	sign = *(reinterpret_cast<double*>(&plusNaNBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 1.0);

	// 1.0, -NaN --> -1.0
	magnitude = 1.0;
	sign = *(reinterpret_cast<double*>(&minusNaNBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, -1.0);

	// 1.0, 0.0 --> 1.0
	magnitude = 1.0;
	sign = 0.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 1.0);

	// 1.0, -0.0 --> -1.0
	magnitude = 1.0;
	sign = -0.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, -1.0);

	// 1.0, 1.0 --> 1.0
	magnitude = 1.0;
	sign = 1.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 1.0);

	// 1.0, -1.0 --> 1.0
	magnitude = 1.0;
	sign = -1.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, -1.0);

	// -1.0, +Inf --> 1.0
	magnitude = -1.0;
	sign = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 1.0);

	// -1.0, -Inf --> -1.0
	magnitude = -1.0;
	sign = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, -1.0);

	// -1.0, NaN --> 1.0
	magnitude = -1.0;
	sign = *(reinterpret_cast<double*>(&plusNaNBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 1.0);

	// -1.0, -NaN --> -1.0
	magnitude = -1.0;
	sign = *(reinterpret_cast<double*>(&minusNaNBtmsk));
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, -1.0);

	// -1.0, 0.0 --> 1.0
	magnitude = -1.0;
	sign = 0.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 1.0);

	// -1.0 , -0.0 --> -1.0
	magnitude = -1.0;
	sign = -0.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, -1.0);

	// -1.0, 1.0 --> 1.0
	magnitude = -1.0;
	sign = 1.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 1.0);

	// -1.0, -1.0 --> -1.0
	magnitude = -1.0;
	sign = -1.0;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, -1.0);

	// +max Double, +max Double --> +max Double
	magnitude = 1.7e+308;
	sign = 1.7e+308;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 1.7e+308);

	// +max Double, +min Double --> +max Double
	magnitude = 1.7e+308;
	sign = 1.7e-308;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 1.7e+308);

	// +min Double, +min Double --> +min Double
	magnitude = 1.7e-308;
	sign = 1.7e-308;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 1.7e-308);

	// +min Double, +max Double --> +min Double
	magnitude = 1.7e-308;
	sign = 1.7e+308;
	res = GsbIeee754::copySign(magnitude, sign);
	CU_ASSERT_EQUAL(res, 1.7e-308);
}

/**
 * @brief  	Test of GsbIeee754::scalbn()
 * @pre		Initialize a double [x] and an int [n]
 * @result	Should return [x] * 2^[n]
 *
 * @UT_Purpose			Test the scalbn function with several inputs.
 * @UT_Tested_function	double scalbn(double x, int n);
 * @UT_Type				Functionality
 * @UT_Description		- Special cases : +Inf, -Inf, NaN
 * 						- Basic values : positive, negative, float
 * 						- Tests with x = +-0.0 or n = +-0
 * 						- Tests with value close to the Double maximum precisison precision
 * 						- Tests with values close to 0.
 * 						- Tests with overflow, underflow
 * 						- Tests with subnormal values (will result in a 0.0)
 */
void GsbIeee754Test_scalbn(void) {
	// arguments of tested function
	double x;
	int n;

	uint64_t xBtMsk;

	// +Infinite double mask
	uint64_t plusInfinityBtmsk = 0x7FF0000000000000ULL;
	// - Infinite double mask
	uint64_t minusInfinityBtmsk = 0xFFF0000000000000ULL;
	// NaN value mask, with sign bit set to 0
	uint64_t naNBtmsk = 0x7FFE000000000000ULL;

	// return of tested function
	double res;

	// Special Cases

	// +Inf * 2^4 = +Inf
	xBtMsk = plusInfinityBtmsk;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = 4;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), plusInfinityBtmsk);

	// - Inf * 2^4 = -Inf
	xBtMsk = minusInfinityBtmsk;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = 4;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), minusInfinityBtmsk);

	// NaN * 2^4 = NaN
	x = *(reinterpret_cast<double*>(&naNBtmsk));
	n = 4;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), naNBtmsk);

	// -NaN * 2^4 = NaN
	xBtMsk = 0xFFF0000000000030ULL;	 // - NaN
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = 4;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFFE00000000000ULL);	 // + NaN

	// Nominal Cases

	// 123 * 2^4 = 1968
	x = 123;
	n = 4;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 1968.0);

	// 123.4 * 2^5 = 3948.8
	x = 123.4;
	n = 5;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 3948.8);

	// -123456.7 * 2^-8 = -482.252734374999989
	x = -123456.7;
	n = -8;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, -482.252734374999989);

	// 0.0 * 2^8 = 0
	x = 0.0;
	n = 8;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	// -0.0 * 2^8 = 0
	x = -0.0;
	n = 8;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, -0.0);

	//  1 * 2^1024 = +Inf
	x = 1.0;
	n = 1024;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), plusInfinityBtmsk);

	// 1 * 2^1025 = +Inf
	x = 1.0;
	n = 1025;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), plusInfinityBtmsk);

	// -1.0 * 2^1024 = -Inf
	x = -1.0;
	n = 1024;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), minusInfinityBtmsk);

	// -1.0 * 2^1025 = -Inf
	x = -1.0;
	n = 1025;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), minusInfinityBtmsk);

	// +Min Double * 2^0 = +Min Double
	xBtMsk = 0x0010000000000000ULL;	 // + Min Double
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = 0;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x0010000000000000ULL);	 // + Min Double

	// -Min Double * 2^0 = -Min Double
	xBtMsk = 0x8010000000000000ULL;	 // - Min Double
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = 0;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x8010000000000000ULL);	 // - Min Double

	// 1 * 2^1023 = +Max Double
	x = 1.0;
	n = 1023;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FE0000000000000ULL);// + Max Double (exponent part at maximum before infinite)

	// -1 * 2^1023 = -Max Double
	x = -1.0;
	n = 1023;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0xFFE0000000000000ULL);// - Max Double (exponent part at maximum before infinite)

	// +Min Double * 2^(-1) = 0.0
	xBtMsk = 0x0010000000000000ULL;	 // + Min Double
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = -1;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	// +Min Double * 2^(-2) = 0.0
	xBtMsk = 0x0010000000000000ULL;	 // + Min Double
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = -2;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	// +Max Double * 2^(-2046) = 0.0
	xBtMsk = 0x7FE0000000000000ULL;	 // Max Double (exponent part at maximum before infinite)
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = -2046;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	// +Max Double * 2^(-2047) = 0.0
	xBtMsk = 0x7FE0000000000000ULL;	 // + Max Double (exponent part at maximum before infinite)
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = -2047;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	// +Min Double * 2^-54 = 0.0
	xBtMsk = 0x0010000000000000ULL;	 // + Min Double
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = -54;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	// 1 * 2^2147483647 = +Inf
	x = 1.0;
	n = 2147483647;	 // Max Int32_t
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), plusInfinityBtmsk);

	// 1 * 2^2147483647 = +Inf
	x = 1.0;
	n = 49999;	// Max Int32_t
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), plusInfinityBtmsk);

	// 1 * 2^-1200 = 0.0 (underflow)
	x = 1.0;
	n = -1200;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	// 1 * 2^-1077 = 0.0 (underflow)
	x = 1.0;
	n = -1077;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	// 1 * 2^-1076 = 0.0 (underflow)
	x = 1.0;
	n = -1076;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	// 1.0 * 2^-1078 = 0.0 (underflow)
	x = 1.0;
	n = -1078;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	// Subnormal cases
	xBtMsk = 0x00000000000001FFULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = -50001;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	xBtMsk = 0x0000000000000100ULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = -50001;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	xBtMsk = 0x0008000000000000ULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = -50001;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	xBtMsk = 0x000FFFFFFFFFFFFFULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = -50001;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	xBtMsk = 0x00000000000001FFULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = -50000;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	xBtMsk = 0x0000000000000100ULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = -50000;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	xBtMsk = 0x0008000000000000ULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = -50000;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	xBtMsk = 0x000FFFFFFFFFFFFFULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = -50000;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	xBtMsk = 0x00000000000001FFULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = 50000;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	xBtMsk = 0x0000000000000100ULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = 50000;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	xBtMsk = 0x0008000000000000ULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = 50000;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	xBtMsk = 0x000FFFFFFFFFFFFFULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = 50000;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	xBtMsk = 0x00000000000001FFULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = 50001;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	xBtMsk = 0x0000000000000100ULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = 50001;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	xBtMsk = 0x0008000000000000ULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = 50001;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);

	xBtMsk = 0x000FFFFFFFFFFFFFULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	n = 50001;
	res = GsbIeee754::scalbn(x, n);
	CU_ASSERT_EQUAL(res, 0.0);
}

/**
 * @brief  	Test of GsbIeee754::kernelCos()
 * @pre		Initialize two double [x] and [y] with [y] the tail of [x]
 * 			x and y are bounded by [-pi/4, pi/4], pi/4 ~ 0.785398164
 * @result	The cosine of [x] tailed by [y]
 *
 * @UT_Purpose			Test the kernelCos function with several inputs.
 * @UT_Tested_function	double kernelCos(double x, double xTail);
 * @UT_Type				Functionality
 * @UT_Description		- Special cases : Tests with value close to +-2^-27 (below this value, the function will return 1.0)
 * 						- Special cases : Tests with +- 0.0
 * 						- Tests will several values in the range [-pi/4, pi/4] with several y values (tail of x)
 */
void GsbIeee754Test_kernelCos(void) {
	// arguments of tested function
	double x;
	double y;

	// return of tested function
	double res;

	// Special Cases

	// Special test n°1
	// x = 0x3E400000001C5F67
	x = 7.450580599999999e-09;
	// y = 0x0000000000000000;
	y = 0.0;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 1.0
	CU_ASSERT_EQUAL(res, 1.0);

	// Special test n°2
	// x = 0xBE400000001C5F67
	x = -7.450580599999999e-09;
	// y = 0x0000000000000000;
	y = 0.0;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 1.0
	CU_ASSERT_EQUAL(res, 1.0);

	// Special test n°3
	// x = 0x3E400000001C5F68
	x = 7.4505806e-09;
	// y = 0x0000000000000000;
	y = 0.0;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 1.0
	CU_ASSERT_EQUAL(res, 1.0);

	// Special test n°4
	// x = 0xBE400000001C5F68
	x = -7.4505806e-09;
	// y = 0x0000000000000000;
	y = 0.0;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 1.0
	CU_ASSERT_EQUAL(res, 1.0);

	// Special test n°5
	// x = 0x3E400000001C5F69
	x = 7.450580600000002e-09;
	// y = 0x0000000000000000;
	y = 0.0;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 1.0
	CU_ASSERT_EQUAL(res, 1.0);

	// Special test n°6
	// x = 0xBE400000001C5F69
	x = -7.450580600000002e-09;
	// y = 0x0000000000000000;
	y = 0.0;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 1.0
	CU_ASSERT_EQUAL(res, 1.0);

	// Nominal Cases

	// Test n°1
	// x = 0x0000000000000000
	x = 0.0;
	// y = 0x0000000000000000;
	y = 0.0;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 1.0
	CU_ASSERT_EQUAL(res, 1.0);

	// Test n°2
	// x = 0x8000000000000000
	x = -0.0;
	// y = 0x0000000000000000;
	y = 0.0;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 1.0
	CU_ASSERT_EQUAL(res, 1.0);

	// Test n°3
	// x = 0x3FE921FB54442D18
	x = 0.7853981633974483;
	// y = 0x0000000000000000;
	y = 0.0;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 0x3FE6A09E667F3BCD
	CU_ASSERT_EQUAL(res, 0.7071067811865476);

	// Test n°4
	// x = 0x3FE41B2F769CF0E0
	x = 0.6283185307179586;
	// y = 0x0000000000000000;
	y = 0.0;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 0x3FE9E3779B97F4A8
	CU_ASSERT_EQUAL(res, 0.8090169943749475);

	// Test n°5
	// x = 0x3FE0C152382D7365
	x = 0.5235987755982988;
	// y = 0x0000000000000000;
	y = 0.0;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 0x3FEBB67AE8584CAB
	CU_ASSERT_EQUAL(res, 0.8660254037844387);

	// Test n°6
	// x = 0x3FD921FB54442D18
	x = 0.39269908169872414;
	// y = 0x0000000000000000;
	y = 0.0;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 0x3FED906BCF328D46
	CU_ASSERT_EQUAL(res, 0.9238795325112867);

	// Test n°7
	// x = 0x3FD41B2F769CF0E0
	x = 0.3141592653589793;
	// y = 0x0000000000000000;
	y = 0.0;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 0x3FEE6F0E134454FF
	CU_ASSERT_EQUAL(res, 0.9510565162951535);

	// Test n°8
	// x = 0x3FD0C152382D7365
	x = 0.2617993877991494;
	// y = 0x0000000000000000;
	y = 0.0;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 0x3FEEE8DD4748BF15
	CU_ASSERT_EQUAL(res, 0.9659258262890683);

	// Test n°9
	// x = 0xBFE921FB54442D18
	x = -0.7853981633974483;
	// y = 0x0000000000000000;
	y = 0.0;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 0x3FE6A09E667F3B
	CU_ASSERT_EQUAL(res, 0.7071067811865476);

	// Test n°10
	// x = 0xBFE0C152382D7365
	x = -0.5235987755982988;
	// y = 0x0000000000000000;
	y = 0.0;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 0x3FEBB67AE8584CAB
	CU_ASSERT_EQUAL(res, 0.8660254037844387);

	// Test n°11
	// x = 0xBFD921FB54442D18
	x = -0.39269908169872414;
	// y = 0x0000000000000000;
	y = 0.0;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 0x3FED906BCF328D46
	CU_ASSERT_EQUAL(res, 0.9238795325112867);

	// Test n°12
	// x = 0xBFE921FB54442D19
	x = -0.7853981633974484;
	// y = 0xBC6A626331000000;
	y = -1.1442377445176544e-17;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 0x3FE6A09E667F3BCC
	CU_ASSERT_EQUAL(res, 0.7071067811865475);

	// Test n°13
	// x = 0xBFE0C152382D7365
	x = -0.5235987755982988;
	// y = 0xBC6A626331000000;
	y = -1.1442377445176544e-17;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 0x3FEBB67AE8584CAB
	CU_ASSERT_EQUAL(res, 0.8660254037844387);

	// Test n°14
	// x = 0x3FE0C152382D7363
	x = 0.5235987755982986;
	// y = 0xBC7A626331000000;
	y = -2.2884754890353088e-17;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 0x3FEBB67AE8584CAC
	CU_ASSERT_EQUAL(res, 0.8660254037844388);

	// Test n°15
	// x = 0xBFE921FB54442D1A
	x = -0.7853981633974485;
	// y = 0xBC7A626331000000;
	y = -2.2884754890353088e-17;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 0x3FE6A09E667F3BCB
	CU_ASSERT_EQUAL(res, 0.7071067811865474);

	// Test n°16
	// x = 0xBFE0C152382D736A
	x = -0.5235987755982994;
	// y = 0xBC7A626331000000;
	y = -2.2884754890353088e-17;
	res = GsbIeee754::kernelCos(x, y);
	// assert res = 0x3FEBB67AE8584CA8
	CU_ASSERT_EQUAL(res, 0.8660254037844384);
}

/**
 * @brief  	Test of GsbIeee754::kernelSin()
 * @pre		Initialize two double [x] and [y] with [y] the tail of [x]
 * 			x and y are bounded by [-pi/4, pi/4], pi/4 ~ 0.785398164
 * 			and an int value indicating whether y is 0 or not.
 * @result	The sine of [x] tailed by [y]
 *
 * @UT_Purpose			Test the kernelSin function with several inputs.
 * @UT_Tested_function	double kernelSin(double x, double xTail, int32_t xTailIsZero));
 * @UT_Type				Functionality
 * @UT_Description		- Special cases : Tests with value close to +-2^-27 (below this value, the function will return 1.0)
 * 						- Special cases : Tests with +- 0.0
 * 						- Tests will several values in the range [-pi/4, pi/4] with several y values (tail of x)
 */
void GsbIeee754Test_kernelSin(void) {
	// arguments of tested function
	double x;
	double y;
	int32_t iy;

	// return of tested function
	double res;

	// Special Cases

	// Special test n°1
	// x = 0x3E3FFFFFFFFFFFFF
	x = 7.450580596923827E-09;
	// y = 0x0000000000000000;
	y = 0.0;
	iy = 0;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0x3E3FFFFFFFFFFFFF
	CU_ASSERT_EQUAL(res, 7.450580596923827e-9);

	// Special test n°2
	// x = 0xBE3FFFFFFFFFFFFF
	x = -7.450580596923827E-09;
	// y = 0x0000000000000000;
	y = 0.0;
	iy = 0;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0xBE3FFFFFFFFFFFFF
	CU_ASSERT_EQUAL(res, -7.450580596923827E-09);

	// Special test n°3
	// x = 0x3E40000000000000
	x = 7.450580596923828E-09;
	// y = 0x0000000000000000;
	y = 0.0;
	iy = 0;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0x3E40000000000000
	CU_ASSERT_EQUAL(res, 7.450580596923828E-09);

	// Special test n°4
	// x = 0xBE40000000000000
	x = -7.450580596923828E-09;
	// y = 0x0000000000000000;
	y = 0.0;
	iy = 0;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0xBE40000000000000
	CU_ASSERT_EQUAL(res, -7.450580596923828E-09);

	// Special test n°5
	// x = 0x3E40000100000000
	x = 7.450587702351186E-09;
	// y = 0x0000000000000000;
	y = 0.0;
	iy = 0;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0x3E40000100000000
	CU_ASSERT_EQUAL(res, 7.450587702351186E-09);

	// Special test n°6
	// x = 0xBE40000100000000
	x = -7.450587702351186E-09;
	// y = 0x0000000000000000;
	y = 0.0;
	iy = 0;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0xBE40000100000000
	CU_ASSERT_EQUAL(res, -7.450587702351186e-9);

	// Nominal Cases

	// Test n°1
	// x = 0x0000000000000000
	x = 0.0;
	// y = 0x0000000000000000;
	y = 0.0;
	iy = 0;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0x0000000000000000
	CU_ASSERT_EQUAL(res, 0.0);

	// Test n°2
	// x = 0x8000000000000000
	x = -0.0;
	// y = 0x0000000000000000;
	y = 0.0;
	iy = 0;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0x8000000000000000
	CU_ASSERT_EQUAL(res, -0.0);

	// Test n°3
	// x = 0x3FE921FB54442D18
	x = 0.7853981633974483;
	// y = 0x0000000000000000;
	y = 0.0;
	iy = 0;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0x3FE6A09E667F3BCC
	CU_ASSERT_EQUAL(res, 0.7071067811865475);

	// Test n°4
	// x = 0x3FE41B2F769CF0E0
	x = 0.6283185307179586;
	// y = 0x0000000000000000;
	y = 0.0;
	iy = 0;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0x3FE2CF2304755A5E
	CU_ASSERT_EQUAL(res, 0.5877852522924731);

	// Test n°5
	// x = 0x3FE0C152382D7365
	x = 0.5235987755982988;
	// y = 0x0000000000000000;
	y = 0.0;
	iy = 0;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0x3FDFFFFFFFFFFFFF
	CU_ASSERT_EQUAL(res, 0.49999999999999994);

	// Test n°6
	// x = 0x3FD921FB54442D18
	x = 0.39269908169872414;
	// y = 0x0000000000000000;
	y = 0.0;
	iy = 0;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0x3FD87DE2A6AEA963
	CU_ASSERT_EQUAL(res, 0.3826834323650898);

	// Test n°7
	// x = 0x3FD41B2F769CF0E0
	x = 0.3141592653589793;
	// y = 0x0000000000000000;
	y = 0.0;
	iy = 0;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0x3FD3C6EF372FE94F
	CU_ASSERT_EQUAL(res, 0.3090169943749474);

	// Test n°8
	// x = 0x3FD0C152382D7365
	x = 0.2617993877991494;
	// y = 0x0000000000000000;
	y = 0.0;
	iy = 0;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0x3FD0907DC1930690
	CU_ASSERT_EQUAL(res, 0.25881904510252074);

	// Test n°9
	// x = 0xBFE921FB54442D18
	x = -0.7853981633974483;
	// y = 0x0000000000000000;
	y = 0.0;
	iy = 0;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0xBFE6A09E667F3BCC
	CU_ASSERT_EQUAL(res, -0.7071067811865475);

	// Test n°10
	// x = 0xBFE0C152382D7365
	x = -0.5235987755982988;
	// y = 0x0000000000000000;
	y = 0.0;
	iy = 0;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0xBFDFFFFFFFFFFFFF
	CU_ASSERT_EQUAL(res, -0.49999999999999994);

	// Test n°11
	// x = 0xBFD921FB54442D18
	x = -0.39269908169872414;
	// y = 0x0000000000000000;
	y = 0.0;
	iy = 0;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0xBFD87DE2A6AEA963
	CU_ASSERT_EQUAL(res, -0.3826834323650898);

	// Test n°12
	// x = 0xBFE921FB54442D19
	x = -0.7853981633974484;
	// y = 0xBC6A626331000000;
	y = -1.1442377445176544E-17;
	iy = 1;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0xBFE6A09E667F3BCD
	CU_ASSERT_EQUAL(res, -0.7071067811865476);

	// Test n°13
	// x = 0xBFE0C152382D736A
	x = -0.5235987755982994;
	// y = 0xBC7A626331000000;
	y = -2.2884754890353088E-17;
	iy = 1;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0xBFE0000000000004
	CU_ASSERT_EQUAL(res, -0.5000000000000004);

	// Test n°14
	// x = 0x3FE0C152382D7363
	x = 0.5235987755982986;
	// y = 0xBC6A626331000000;
	y = -1.1442377445176544E-17;
	iy = 1;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0x3FDFFFFFFFFFFFFB
	CU_ASSERT_EQUAL(res, 0.4999999999999997);

	// Test n°15
	// x = 0xBFE921FB54442D19
	x = -0.7853981633974484;
	// y = 0xBC6A626331000000;
	y = -1.1442377445176544E-17;
	iy = 1;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0xBFE6A09E667F3BCD
	CU_ASSERT_EQUAL(res, -0.7071067811865476);

	// Test n°16
	// x = 0xBFE0C152382D736A
	x = -0.5235987755982994;
	// y = 0xBC7A626331000000;
	y = -2.2884754890353088E-17;
	iy = 1;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0xBFE0000000000004
	CU_ASSERT_EQUAL(res, -0.5000000000000004);

	// Test n°17
	// x = 0xBCB1A62633145C07
	x = -2.4492935982947064E-16;
	// y = 0x393F1976B7ED8FC0;
	y = 5.989539619436682E-33;
	iy = 1;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0xBCB1A62633145C07
	CU_ASSERT_EQUAL(res, -2.4492935982947064e-16);

	// Test n°18
	// x = 0xBCA1A62633145C07
	x = -1.2246467991473532E-16;
	// y = 0x392F1976B7ED8FC0;
	y = 2.994769809718341E-33;
	iy = 1;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0xBCA1A62633145C07
	CU_ASSERT_EQUAL(res, -1.2246467991473532E-16);

	// Test n°19
	// x = 0x3CB1A62633145C07
	x = 2.4492935982947064E-16;
	// y = 0xB93F1976B7ED8FC0;
	y = -5.989539619436682E-33;
	iy = 1;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0x3CB1A62633145C07
	CU_ASSERT_EQUAL(res, 2.4492935982947064E-16);

	// Test n°20
	// x = 0x3CA1A62633145C07
	x = 1.2246467991473532E-16;
	// y = 0xB92F1976B7ED8FC0;
	y = -2.994769809718341E-33;
	iy = 1;
	res = GsbIeee754::kernelSin(x, y, iy);
	// assert res = 0x3CA1A62633145C07
	CU_ASSERT_EQUAL(res, 1.2246467991473532E-16);
}

/**
 * @brief  	Test of GsbIeee754::getKernelRemainderPio2(double* input, double* output, int32_t inputExponent, int32_t inputDim)
 * @pre		input : double[3] : the input value broken into 24 bits positive integer.
 * 			output : double[2] : will store the result
 * 			inputExponent : int32_t : the exponent of input[0] ()
 * 			inputDim : int32_t : the dimension of the input value (number of used elements in the array)
 * @result	Reduce the input in to a multiple of pi/2 with [-pi/4, pi/4] and return the octant.
 *
 * @UT_Purpose			Test the GsbIeee754::getKernelRemainderPio2 function with several inputs.
 * @UT_Tested_function	int32_t GsbIeee754::getKernelRemainderPio2(double* input, double* ouput, int32_t inputExponent, int32_t inputDim);
 * @UT_Type				Functionality
 * @UT_Description		- Special case : input where not all the input array elements are different from 0. (either inputDim < 3, or an element = 0.0)
 * 						- Tests with several values with different range of exponent (negative or positive).
 */
void GsbIeee754Test_getKernelRemainderPio2(void) {
	// arguments of tested function
	double input[3];
	double output[2];
	int32_t inputExponent;
	int32_t inputDim;

	// return of tested function
	int32_t res;

	// Special Cases

	// Special test n°1
	// input[0] = 0x416921FC00000000;
	input[0] = 13176800.0;
	// input[1] = 0x0000000000000000;
	input[1] = 0.0;
	// input[2] = 0x0000000000000000;
	input[2] = 0.0;
	inputExponent = -3;
	inputDim = 1;
	res = GsbIeee754::getKernelRemainderPio2(input, output, inputExponent, inputDim);
	CU_ASSERT_EQUAL(res, 0);
	// output[0] = 0x3FE5777A5CF72CED
	CU_ASSERT_EQUAL(output[0], 0.6708347144825929);
	// output[1] = 0xBC698A2E037076FC
	CU_ASSERT_EQUAL(output[1], -1.1076107309666874e-17);

	// Special test n°2
	// input[0] = 0x416921FC00000000;
	input[0] = 13176800.0;
	// input[1] = 0x0000000000000000;
	input[1] = 0.0;
	// input[2] = 0x4120000000000000;
	input[2] = 524288.0;
	inputExponent = -3;
	inputDim = 3;
	res = GsbIeee754::getKernelRemainderPio2(input, output, inputExponent, inputDim);
	CU_ASSERT_EQUAL(res, 0);
	// output[0] = 0x3FE5777A5D172CED
	CU_ASSERT_EQUAL(output[0], 0.6708347147154236);
	// output[1] = 0xBC698A2E03707704
	CU_ASSERT_EQUAL(output[1], -1.1076107309666886e-17);

	// Special test n°3
	// input[0] = 0x41606E8B60000000;
	input[0] = 8615003.0;
	// input[1] = 0x4169313280000000;
	input[1] = 13207956.0;
	// input[2] = 0x0000000000000000;
	input[2] = 0.0;
	inputExponent = 26;
	inputDim = 2;
	res = GsbIeee754::getKernelRemainderPio2(input, output, inputExponent, inputDim);
	CU_ASSERT_EQUAL(res, 2);
	// output[0] = 0xBFD1C24A9C099DE5
	CU_ASSERT_EQUAL(output[0], -0.2774836086086608);
	// output[1] = 0x3C7499AEB12D8F11
	CU_ASSERT_EQUAL(output[1], 1.7867930954199114e-17);

	// Special test n°4
	// input[0] = 0x4160000000000000;
	input[0] = 8388608.0;
	// input[1] = 0x0000000000000000;
	input[1] = 0.0;
	// input[2] = 0x0000000000000000;
	input[2] = 0.0;
	inputExponent = 72;
	inputDim = 1;
	res = GsbIeee754::getKernelRemainderPio2(input, output, inputExponent, inputDim);
	CU_ASSERT_EQUAL(res, 1);
	// output[0] = 0xBFA0F45204CD2192
	CU_ASSERT_EQUAL(output[0], -0.03311401661500492);
	// output[1] = 0xBC1D6B3C80DE1390
	CU_ASSERT_EQUAL(output[1], -3.9870033216827297e-19);

	// Nominal Cases

	// Test n°1
	// input[0] = 0x416FFFFFE0000000;
	input[0] = 16777215.0;
	// input[1] = 0x416FFFFFE0000000;
	input[1] = 16777215.0;
	// input[2] = 0x416F000000000000;
	input[2] = 16252928.0;
	inputExponent = 1000;
	inputDim = 3;
	res = GsbIeee754::getKernelRemainderPio2(input, output, inputExponent, inputDim);
	CU_ASSERT_EQUAL(res, 2);
	// output[0] = 0xBF7453020FF06B39
	CU_ASSERT_EQUAL(output[0], -0.004961975150787273);
	// output[1] = 0xBC1AFAD10278554C
	CU_ASSERT_EQUAL(output[1], -3.65643818040935e-19);

	// Test n°2
	// input[0] = 0x41601DA220000000;
	input[0] = 8449297.0;
	// input[1] = 0x41666D8C40000000;
	input[1] = 11758690.0;
	// input[2] = 0x416A000000000000;
	input[2] = 13631488.0;
	inputExponent = 25;
	inputDim = 3;
	res = GsbIeee754::getKernelRemainderPio2(input, output, inputExponent, inputDim);
	CU_ASSERT_EQUAL(res, 4);
	// output[0] = 0xBFD5BDCB7068117C
	CU_ASSERT_EQUAL(output[0], -0.33970914818953424);
	// output[1] = 0x3C74E5246FF01848
	CU_ASSERT_EQUAL(output[1], 1.812359918253811e-17);

	// Test n°3
	// input[0] = 0x416921FCE0000000;
	input[0] = 13176807.0;
	// input[1] = 0x416FFFFFE0000000;
	input[1] = 16777215.0;
	// input[2] = 0x416F000000000000;
	input[2] = 16252928.0;
	inputExponent = -3;
	inputDim = 3;
	res = GsbIeee754::getKernelRemainderPio2(input, output, inputExponent, inputDim);
	CU_ASSERT_EQUAL(res, 1);
	// output[0] = 0x3FB99C1DA27695E3
	CU_ASSERT_EQUAL(output[0], 0.10003838745486564);
	// output[1] = 0xBC4AED7E704D789E
	CU_ASSERT_EQUAL(output[1], -2.9195082279649925e-18);

	// Test n°4
	// input[0] = 0x41609F4B00000000;
	input[0] = 8714840.0;
	// input[1] = 0x415E651C80000000;
	input[1] = 7967858.0;
	// input[2] = 0x4144000000000000;
	input[2] = 2621440.0;
	inputExponent = 26;
	inputDim = 3;
	res = GsbIeee754::getKernelRemainderPio2(input, output, inputExponent, inputDim);
	CU_ASSERT_EQUAL(res, 6);
	// output[0] = 0xBE8F55A99D857F25
	CU_ASSERT_EQUAL(output[0], -2.3346110712262172e-7);
	// output[1] = 0x3B0B080591561EB8
	CU_ASSERT_EQUAL(output[1], 2.7949745259480635e-24);

	input[0] = 8714840.0;
	input[1] = 7967858.0;
	input[2] = 2621440.0;
	inputExponent = 400;
	inputDim = 3;
	res = GsbIeee754::getKernelRemainderPio2(input, output, inputExponent, inputDim);
	CU_ASSERT_EQUAL(res, 5);
	// output[0] = 0xBE8F55A99D857F25
	CU_ASSERT_EQUAL(output[0], -2.781952627017652765473165743515e-2);
	// output[1] = 0x3B0B080591561EB8
	CU_ASSERT_EQUAL(output[1], 1.4480322154625608e-18);

}
/**
 * @brief  	Test of GsbIeee754::getRemainderPio2(double input, double* output)
 * @pre		Initialize a double : input and a double array of two elements : output.
 * @result	return the remainder of the input rem pi/2 in output. And return the number of time input can be divided by pi/2.
 *
 * @UT_Purpose			Test the GsbIeee754::getRemainderPio2 function with several inputs.
 * @UT_Tested_function	int32_t GsbIeee754::getRemainderPio2(double input, double* output);
 * @UT_Type				Functionality
 * @UT_Description		- Special case : Test with input as +-INF and NaN
 * 						- Tests with values close to pi/2, pi/4, 3pi/4
 * 						- ~<= 2^19*(pi/2), >2^19*(pi/2) but less than infinite
 */
void GsbIeee754Test_getRemainderPio2(void) {
	// arguments of tested function
	double input;
	double output[2];

	// return of tested function
	int32_t res;

	// +Infinite double mask
	uint64_t plusInfinityBtmsk = 0x7FF0000000000000ULL;
	// - Infinite double mask
	uint64_t minusInfinityBtmsk = 0xFFF0000000000000ULL;
	// NaN value mask, with sign bit set to 0
	uint64_t naNBtmsk = 0x7FFE000000000000ULL;

	// Special Cases

	// Special case test n°01
	// input = INF
	// output[0] = NaN output[1] = NaN
	input = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(*(reinterpret_cast<int64_t*>(&output[0])), 0x7FFFE00000000000ULL);
	CU_ASSERT_EQUAL(*(reinterpret_cast<int64_t*>(&output[1])), 0x7FFFE00000000000ULL);

	// Special case test n°02
	// input = -INF
	// output[0] = NaN output[1] = NaN
	input = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(*(reinterpret_cast<int64_t*>(&output[0])), 0x7FFFE00000000000ULL);
	CU_ASSERT_EQUAL(*(reinterpret_cast<int64_t*>(&output[1])), 0x7FFFE00000000000ULL);

	// Special case test n°03
	// input = NaN
	// output[0] = NaN output[1] = NaN
	input = *(reinterpret_cast<double*>(&naNBtmsk));
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(*(reinterpret_cast<int64_t*>(&output[0])), 0x7FFE000000000000ULL);
	CU_ASSERT_EQUAL(*(reinterpret_cast<int64_t*>(&output[1])), 0x7FFE000000000000ULL);

	// Nominal Cases

	// Test n°01
	// input = 0x3FE921FB00000000
	// output[0] = 0x3FE921FB00000000 output[1] = 0x0000000000000000
	input = 0.785398006439209;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(output[0], 0.785398006439209);
	CU_ASSERT_EQUAL(output[1], 0.0);

	// Test n°02
	// input = 0xBFE921FB00000000
	// output[0] = 0xBFE921FB00000000 output[1] = 0x0000000000000000
	input = -0.785398006439209;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(output[0], -0.785398006439209);
	CU_ASSERT_EQUAL(output[1], 0.0);

	// Test n°03
	// input = 0x3A00A55500000000
	// output[0] = 0x3A00A55500000000 output[1] = 0x0000000000000000
	input = 2.626248627827475e-29;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(output[0], 2.626248627827475e-29);
	CU_ASSERT_EQUAL(output[1], 0.0);

	// Test n°04
	// input = 0xBA00A55500000000
	// output[0] = 0xBA00A55500000000 output[1] = 0x0000000000000000
	input = -2.626248627827475e-29;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(output[0], -2.626248627827475e-29);
	CU_ASSERT_EQUAL(output[1], 0.0);

	// Test n°05
	// input = 0x3FE921FB00000001
	// output[0] = 0x3FE921FB00000001 output[1] = 0x0000000000000000
	input = 0.7853980064392091;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(output[0], 0.7853980064392091);
	CU_ASSERT_EQUAL(output[1], 0.0);

	// Test n°06
	// input = 0xBFE921FB00000001
	// output[0] = 0xBFE921FB00000001 output[1] = 0x0000000000000000
	input = -0.7853980064392091;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(output[0], -0.7853980064392091);
	CU_ASSERT_EQUAL(output[1], 0.0);

	// Test n°07
	// input = 0x3A00A55555555555
	// output[0] = 0x3A00A55555555555 output[1] = 0x0000000000000000
	input = 2.626249430298285e-29;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(output[0], 2.626249430298285e-29);
	CU_ASSERT_EQUAL(output[1], 0.0);

	// Test n°08
	// input = 0xBA00A55555555555
	// output[0] = 0xBA00A55555555555 output[1] = 0x0000000000000000
	input = -2.626249430298285e-29;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(output[0], -2.626249430298285e-29);
	CU_ASSERT_EQUAL(output[1], 0.0);

	// Test n°09
	// input = 0x3FE921FC00000000
	// output[0] = 0xBFE921FAA8885A31 output[1] = 0x3C8CB3B399E00000
	input = 0.7853984832763672;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1);
	CU_ASSERT_EQUAL(output[0], -0.7853978435185295);
	CU_ASSERT_EQUAL(output[1], 4.9789962508669555e-17);

	// Test n°10
	// input = 0x3FE921FCAAAAAAAA
	// output[0] = 0xBFE921F9FDDDAF87 output[1] = 0x3C8CB3B399E00000
	input = 0.7853988011678059;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1);
	CU_ASSERT_EQUAL(output[0], -0.7853975256270908);
	CU_ASSERT_EQUAL(output[1], 4.9789962508669555e-17);

	// Test n°11
	// input = 0x3FE921FC00000001
	// output[0] = 0xBFE921FAA8885A30 output[1] = 0x3C8CB3B399E00000
	input = 0.7853984832763673;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1);
	CU_ASSERT_EQUAL(output[0], -0.7853978435185294);
	CU_ASSERT_EQUAL(output[1], 4.9789962508669555e-17);

	// Test n°12
	// input = 0x3FE921FCFFFFFFFF
	// output[0] = 0xBFE921F9A8885A32 output[1] = 0x3C8CB3B399E00000
	input = 0.7853989601135253;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1);
	CU_ASSERT_EQUAL(output[0], -0.7853973666813714);
	CU_ASSERT_EQUAL(output[1], 4.9789962508669555e-17);

	// Test n°13
	// input = 0x3FE921FD00000000
	// output[0] = 0xBFE921F9A8885A31 output[1] = 0x3C8CB3B399E00000
	input = 0.7853989601135254;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1);
	CU_ASSERT_EQUAL(output[0], -0.7853973666813713);
	CU_ASSERT_EQUAL(output[1], 4.9789962508669555e-17);

	// Test n°14
	// input = 0x4002D97BFFFFFFFF
	// output[0] = 0x3FE921F95777A5CB output[1] = 0x3C8CB3B399E00000
	input = 2.3561935424804683;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1);
	CU_ASSERT_EQUAL(output[0], 0.7853972156855716);
	CU_ASSERT_EQUAL(output[1], 4.9789962508669555e-17);

	// Test n°15
	// input = 0x3FF921FB00000000
	// output[0] = 0xBE95110B4611A626 output[1] = 0xBB298A2E03707300
	input = 1.570796012878418;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1);
	CU_ASSERT_EQUAL(output[0], -3.139164786504813e-7);
	CU_ASSERT_EQUAL(output[1], -1.0562999066987326e-23);

	// Test n°16
	// input = 0x3FF921FBFFFFFFFF
	// output[0] = 0x3EA5777A5CD72CED output[1] = 0xBB298A2E03707300
	input = 1.5707969665527342;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1);
	CU_ASSERT_EQUAL(output[0], 6.397578375337241e-7);
	CU_ASSERT_EQUAL(output[1], -1.0562999066987326e-23);

	// Test n°17
	// input = 0xBFF921FB00000000
	// output[0] = 0x3E95110B4611A626 output[1] = 0x3B298A2E03707300
	input = -1.570796012878418;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -1);
	CU_ASSERT_EQUAL(output[0], 3.139164786504813e-7);
	CU_ASSERT_EQUAL(output[1], 1.0562999066987326e-23);

	// Test n°18
	// input = 0xBFF921FB00000001
	// output[0] = 0x3E95110B45D1A626 output[1] = 0x3B298A2E03707300

	input = -1.5707960128784182;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -1);
	CU_ASSERT_EQUAL(output[0], 3.139164784284367e-7);
	CU_ASSERT_EQUAL(output[1], 1.0562999066987326e-23);
	// Test n°19
	// input = 0xBFF921FBFFFFFFFF
	// output[0] = 0xBEA5777A5CD72CED output[1] = 0x3B298A2E03707300

	input = -1.5707969665527342;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -1);
	CU_ASSERT_EQUAL(output[0], -6.397578375337241e-7);
	CU_ASSERT_EQUAL(output[1], 1.0562999066987326e-23);
	// Test n°20
	// input = 0xBFF921FC00000000
	// output[0] = 0xBEA5777A5CF72CED output[1] = 0x3B29880000000000

	input = -1.5707969665527344;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -1);
	CU_ASSERT_EQUAL(output[0], -6.397578377557687e-7);
	CU_ASSERT_EQUAL(output[1], 1.0559477507122244e-23);

	// Test n°21
	// input = 0xBFF921FD00000000
	// output[0] = 0xBEBABBBD2E7B9676 output[1] = 0xBB5CCF0000000000
	input = -1.5707979202270508;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -1);
	CU_ASSERT_EQUAL(output[0], -0.0000015934321541620186);
	CU_ASSERT_EQUAL(output[1], -9.53196408996653e-23);

	// Test n°22
	// input = 0xBFFFFFFC00000000
	// output[0] = 0xBFDB7802AEEF4B9F output[1] = 0x3C5A626331000000
	input = -1.9999961853027344;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -1);
	CU_ASSERT_EQUAL(output[0], -0.42919985850783776);
	CU_ASSERT_EQUAL(output[1], 5.721188722588272e-18);

	// Test n°23
	// input = 0xBFFFFFFFFFFFFFFF
	// output[0] = 0xBFDB7812AEEF4B9B output[1] = 0x3C5A626331000000
	input = -1.9999999999999998;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -1);
	CU_ASSERT_EQUAL(output[0], -0.42920367320510316);
	CU_ASSERT_EQUAL(output[1], 5.721188722588272e-18);
	// Test n°24
	// input = 0x4002D97C00000000
	// output[0] = 0x3FE921F95777A5CF output[1] = 0x3C8CB3B399E00000
	input = 2.3561935424804688;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1);
	CU_ASSERT_EQUAL(output[0], 0.7853972156855721);
	CU_ASSERT_EQUAL(output[1], 4.9789962508669555e-17);

	// Test n°25
	// input = 0x4002D97D00000000
	// output[0] = 0xBFE921F95110B461 output[1] = 0xBC6A626331000000
	input = 2.3561954498291016;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 2);
	CU_ASSERT_EQUAL(output[0], -0.7853972037606917);
	CU_ASSERT_EQUAL(output[1], -1.1442377445176544e-17);

	// Test n°26
	// input = 0x4002D97D00000001
	// output[0] = 0xBFE921F95110B45D output[1] = 0xBC6A626331000000
	input = 2.356195449829102;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 2);
	CU_ASSERT_EQUAL(output[0], -0.7853972037606912);
	CU_ASSERT_EQUAL(output[1], -1.1442377445176544e-17);

	// Test n°27
	// input = 0x4002D97DFFFFFFFF
	// output[0] = 0xBFE921F55110B465 output[1] = 0xBC6A626331000000
	input = 2.356197357177734;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 2);
	CU_ASSERT_EQUAL(output[0], -0.7853952964120593);
	CU_ASSERT_EQUAL(output[1], -1.1442377445176544e-17);

	// Test n°28
	// input = 0x4002D97EFFFFFFFF
	// output[0] = 0xBFE921F15110B465 output[1] = 0xBC6A626331000000
	input = 2.3561992645263667;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 2);
	CU_ASSERT_EQUAL(output[0], -0.7853933890634265);
	CU_ASSERT_EQUAL(output[1], -1.1442377445176544e-17);

	// Test n°29
	// input = 0x400FD97EFFFFFFFF
	// output[0] = 0xBFE765E7F9990E96 output[1] = 0x3C861B1ACD800000
	input = 3.9811992645263667;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 3);
	CU_ASSERT_EQUAL(output[0], -0.7311897158583232);
	CU_ASSERT_EQUAL(output[1], 3.8347585050568314e-17);

	// Test n°30
	// input = 0x403FD97EFFFFFFFF
	// output[0] = 0x3FDBC135AAB1E82A output[1] = 0xBC4F5F7FA0000000
	input = 31.849594116210934;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 20);
	CU_ASSERT_EQUAL(output[0], 0.43366758031300157);
	CU_ASSERT_EQUAL(output[1], -3.401471937550997e-18);

	// Test n°31
	// input = 0x413921FB00000000
	// output[0] = 0xBFD5110B4611A626 output[1] = 0xBC698A2E03707300
	input = 1647099.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1048576);
	CU_ASSERT_EQUAL(output[0], -0.3291652855174071);
	CU_ASSERT_EQUAL(output[1], -1.1076107309665303e-17);

	// Test n°32
	// input = 0x413921FA00000000
	// output[0] = 0x3FCEEDC415FE1C76 output[1] = 0xBC58B1F8D59B2600
	input = 1647098.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1048575);
	CU_ASSERT_EQUAL(output[0], 0.24163104127748952);
	CU_ASSERT_EQUAL(output[1], -5.3549185835555556e-18);

	// Test n°33
	// input = 0x413921F800000000
	// output[0] = 0xBFC8026147E07AC8 output[1] = 0x3C1B06A5BAA9A000
	input = 1647096.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1048574);
	CU_ASSERT_EQUAL(output[0], -0.18757263192761386);
	CU_ASSERT_EQUAL(output[1], 3.662701425541915e-19);

	// Test n°34
	// input = 0x413921F400000000
	// output[0] = 0x3FE0CB4BA7A0EFE0 output[1] = 0xBC85E50D82108480
	input = 1647092.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1048571);
	CU_ASSERT_EQUAL(output[0], 0.524816348457076);
	CU_ASSERT_EQUAL(output[1], -3.7981314910374e-17);

	// Test n°35
	// input = 0x413921F200000000
	// output[0] = 0x3FB87A12814A5082 output[1] = 0xBC54C608DF3E6400
	input = 1647090.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1048570);
	CU_ASSERT_EQUAL(output[0], 0.09561267525197262);
	CU_ASSERT_EQUAL(output[1], -4.504550568635339e-18);

	// Test n°36
	// input = 0x413921F000000000
	// output[0] = 0xBFD5598E0E9CB77E output[1] = 0xBC7E98E96B7E2880
	input = 1647088.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1048569);
	CU_ASSERT_EQUAL(output[0], -0.33359099795313074);
	CU_ASSERT_EQUAL(output[1], -2.653893745815411e-17);

	// Test n°37
	// input = 0x4139210000000000
	// output[0] = 0xBF5CB89BA0E975B6 output[1] = 0x3BFA68B12DD78000
	input = 1646848.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1048416);
	CU_ASSERT_EQUAL(output[0], -0.0017529983339480255);
	CU_ASSERT_EQUAL(output[1], 8.9477014996939e-20);

	// Test n°38
	// input = 0x4139000000000000
	// output[0] = 0xBFD095379AB38989 output[1] = 0xBC7BABA307FD0680
	input = 1638400.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1043038);
	CU_ASSERT_EQUAL(output[0], -0.2591074953799298);
	CU_ASSERT_EQUAL(output[1], -2.4000295161635964e-17);

	// Test n°39
	// input = 0x4100000000000000
	// output[0] = 0x3FA58E8EC12C0AA8 output[1] = 0x3C495C955AFD3E80
	input = 131072.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 83443);
	CU_ASSERT_EQUAL(output[0], 0.04210325344140148);
	CU_ASSERT_EQUAL(output[1], 2.7497160346141488e-18);

	// Test n°40
	// input = 0x404FFFFFFFFFFFFF
	// output[0] = 0xBFD9C501FBACE40D output[1] = 0xBC6CE0F170000000
	input = 63.99999999999999;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 41);
	CU_ASSERT_EQUAL(output[0], -0.4026493985907685);
	CU_ASSERT_EQUAL(output[1], -1.2524132791560722e-17);

	// Test n°41
	// input = 0x4020000000000000
	// output[0] = 0x3FC2B0BAD558F435 output[1] = 0xBC2F5F7FA0000000
	input = 8.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 5);
	CU_ASSERT_EQUAL(output[0], 0.1460183660255169);
	CU_ASSERT_EQUAL(output[1], -8.503679843877493e-19);

	// Test n°42
	// input = 0x4030000000000000
	// output[0] = 0x3FD2B0BAD558F435 output[1] = 0xBC3F5F7FA0000000
	input = 16.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 10);
	CU_ASSERT_EQUAL(output[0], 0.2920367320510338);
	CU_ASSERT_EQUAL(output[1], -1.7007359687754986e-18);

	// Test n°43
	// input = 0x4040000000000000
	// output[0] = 0x3FE2B0BAD558F435 output[1] = 0xBC4F5F7FA0000000
	input = 32.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 20);
	CU_ASSERT_EQUAL(output[0], 0.5840734641020676);
	CU_ASSERT_EQUAL(output[1], -3.401471937550997e-18);

	// Test n°44
	// input = 0x4047FFFF00000000
	// output[0] = 0xBFE63B1E6882EBE1 output[1] = 0xBC68F50170000000
	input = 47.999969482421875;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 31);
	CU_ASSERT_EQUAL(output[0], -0.6947166482199202);
	CU_ASSERT_EQUAL(output[1], -1.0823396512592494e-17);

	// Test n°45
	// input = 0x413921FC00000000
	// output[0] = 0x3FE5777A5CF72CED output[1] = 0xBC698A2E037076FC
	input = 1647100.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(output[0], 0.6708347144825929);
	CU_ASSERT_EQUAL(output[1], -1.1076107309666874e-17);

	// Test n°46
	// input = 0x6FFFFFFF00000000
	// output[0] = 0xBFE82D7FB51E4A8A output[1] = 0xBC8B80B26520A061
	input = 3.1050347040047875e+231;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(output[0], -0.7555540597407611);
	CU_ASSERT_EQUAL(output[1], -4.770961766646426e-17);

	// Test n°47
	// input = 0x413921FC00000001
	// output[0] = 0x3FE5777A5D172CED output[1] = 0xBC698A2E03707704
	input = 1647100.0000000002;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(output[0], 0.6708347147154236);
	CU_ASSERT_EQUAL(output[1], -1.1076107309666886e-17);

	// Test n°48
	// input = 0x413921FCFFFFFFFF
	// output[0] = 0x3FB99C1DA27695E3 output[1] = 0xBC4AED7E704D789E
	input = 1647100.9999999998;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 1);
	CU_ASSERT_EQUAL(output[0], 0.10003838745486564);
	CU_ASSERT_EQUAL(output[1], -2.9195082279649925e-18);

	// Test n°49
	// input = 0x6FFFFFFFFFFFFFFF
	// output[0] = 0x3FE0A9EB2DB1A926 output[1] = 0x3C799488B6E5D29F
	input = 3.1050361846014175e+231;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(output[0], 0.5207420246563614);
	CU_ASSERT_EQUAL(output[1], 2.218729635508935e-17);

	// Test n°50
	// input = 0xC13921FB00000000
	// output[0] = 0x3FD5110B4611A626 output[1] = 0x3C698A2E03707300
	input = -1647099.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -1048576);
	CU_ASSERT_EQUAL(output[0], 0.3291652855174071);
	CU_ASSERT_EQUAL(output[1], 1.1076107309665303e-17);

	// Test n°51
	// input = 0xC13921FA00000000
	// output[0] = 0xBFCEEDC415FE1C76 output[1] = 0x3C58B1F8D59B2600
	input = -1647098.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -1048575);
	CU_ASSERT_EQUAL(output[0], -0.24163104127748952);
	CU_ASSERT_EQUAL(output[1], 5.3549185835555556e-18);

	// Test n°52
	// input = 0xC13921F800000000
	// output[0] = 0x3FC8026147E07AC8 output[1] = 0xBC1B06A5BAA9A000
	input = -1647096.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -1048574);
	CU_ASSERT_EQUAL(output[0], 0.18757263192761386);
	CU_ASSERT_EQUAL(output[1], -3.662701425541915e-19);

	// Test n°53
	// input = 0xC13921F400000000
	// output[0] = 0xBFE0CB4BA7A0EFE0 output[1] = 0x3C85E50D82108480
	input = -1647092.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -1048571);
	CU_ASSERT_EQUAL(output[0], -0.524816348457076);
	CU_ASSERT_EQUAL(output[1], 3.7981314910374e-17);

	// Test n°54
	// input = 0xC13921F200000000
	// output[0] = 0xBFB87A12814A5082 output[1] = 0x3C54C608DF3E6400
	input = -1647090.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -1048570);
	CU_ASSERT_EQUAL(output[0], -0.09561267525197262);
	CU_ASSERT_EQUAL(output[1], 4.504550568635339e-18);

	// Test n°55
	// input = 0xC13921F000000000
	// output[0] = 0x3FD5598E0E9CB77E output[1] = 0x3C7E98E96B7E2880
	input = -1647088.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -1048569);
	CU_ASSERT_EQUAL(output[0], 0.33359099795313074);
	CU_ASSERT_EQUAL(output[1], 2.653893745815411e-17);

	// Test n°56
	// input = 0xC139210000000000
	// output[0] = 0x3F5CB89BA0E975B6 output[1] = 0xBBFA68B12DD78000
	input = -1646848.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -1048416);
	CU_ASSERT_EQUAL(output[0], 0.0017529983339480255);
	CU_ASSERT_EQUAL(output[1], -8.9477014996939e-20);

	// Test n°57
	// input = 0xC139000000000000
	// output[0] = 0x3FD095379AB38989 output[1] = 0x3C7BABA307FD0680
	input = -1638400.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -1043038);
	CU_ASSERT_EQUAL(output[0], 0.2591074953799298);
	CU_ASSERT_EQUAL(output[1], 2.4000295161635964e-17);

	// Test n°58
	// input = 0xC100000000000000
	// output[0] = 0xBFA58E8EC12C0AA8 output[1] = 0xBC495C955AFD3E80
	input = -131072.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -83443);
	CU_ASSERT_EQUAL(output[0], -0.04210325344140148);
	CU_ASSERT_EQUAL(output[1], -2.7497160346141488e-18);

	// Test n°59
	// input = 0xC04FFFFFFFFFFFFF
	// output[0] = 0x3FD9C501FBACE40D output[1] = 0x3C6CE0F170000000
	input = -63.99999999999999;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -41);
	CU_ASSERT_EQUAL(output[0], 0.4026493985907685);
	CU_ASSERT_EQUAL(output[1], 1.2524132791560722e-17);

	// Test n°60
	// input = 0xC13921FC00000000
	// output[0] = 0xBFE5777A5CF72CED output[1] = 0x3C698A2E037076FC
	input = -1647100.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(output[0], -0.6708347144825929);
	CU_ASSERT_EQUAL(output[1], 1.1076107309666874e-17);

	// Test n°61
	// input = 0xEFFFFFFF00000000
	// output[0] = 0x3FE82D7FB51E4A8A output[1] = 0x3C8B80B26520A061
	input = -3.1050347040047875e+231;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(output[0], 0.7555540597407611);
	CU_ASSERT_EQUAL(output[1], 4.770961766646426e-17);

	// Test n°62
	// input = 0xE13921FC00000001
	// output[0] = 0xBFDA4BC0F9426BEA output[1] = 0x3C765059C17A83D0
	input = -2.2084000441408455e+160;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -7);
	CU_ASSERT_EQUAL(output[0], -0.41087364522689873);
	CU_ASSERT_EQUAL(output[1], 1.9354196686666354e-17);

	// Test n°63
	// input = 0xE13921FCFFFFFFFF
	// output[0] = 0x3FD332F4282E01AC output[1] = 0xBC7C7FDDBAB7BEB9
	input = -2.208401384921638e+160;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, -2);
	CU_ASSERT_EQUAL(output[0], 0.29998496936607455);
	CU_ASSERT_EQUAL(output[1], -2.4719355964608102e-17);

	// Test n°64
	// input = 0xEFFFFFFFFFFFFFFF
	// output[0] = 0xBFE0A9EB2DB1A926 output[1] = 0xBC799488B6E5D29F
	input = -3.1050361846014175e+231;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(output[0], -0.5207420246563614);
	CU_ASSERT_EQUAL(output[1], -2.218729635508935e-17);

	// Test n°65
	// input = 0x413000FF00000000
	// output[0] = 0xBFE67EC5EE3B6796 output[1] = 0x3C8319A5A1916E80
	input = 1048831.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 667707);
	CU_ASSERT_EQUAL(output[0], -0.7029752400369371);
	CU_ASSERT_EQUAL(output[1], 3.3133536852393674e-17);

	// Test n°66
	// input = 0x401000FF00000000
	// output[0] = 0xBFE6C3EBF9990E92 output[1] = 0x3C861B1ACD800000
	input = 4.000972747802734;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 3);
	CU_ASSERT_EQUAL(output[0], -0.7114162325819555);
	CU_ASSERT_EQUAL(output[1], 3.8347585050568314e-17);

	// Test n°67
	// input = 0x4140000000000000
	// output[0] = 0x3FE58E8EC12C0AA8 output[1] = 0x3C895C955AFD3D4C
	input = 2097152.0;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(output[0], 0.6736520550624236);
	CU_ASSERT_EQUAL(output[1], 4.399545655382448e-17);

	// Test n°68
	// input = 0x401921FB54442D18
	// output[0] = 0xBCB1A62633145C07 output[1] = 0x393F1976B7ED8FC0
	input = 6.283185307179586;
	res = GsbIeee754::getRemainderPio2(input, output);
	CU_ASSERT_EQUAL(res, 4);
	CU_ASSERT_EQUAL(output[0], -2.4492935982947064e-16);
	CU_ASSERT_EQUAL(output[1], 5.989539619436682e-33);
}

/**
 * @brief  	Test of GsbIeee754::cos(double x)
 * @pre		Initialize a double x, input of the function
 * @result	The cosine of x
 *
 * @UT_Purpose			Test the GsbIeee754::cos function with several inputs.
 * @UT_Tested_function	int32_t GsbIeee754::cos(double x);
 * @UT_Type				Functionality
 * @UT_Description		- Special cases : +-Inf, NaN, +-0.0
 * 						- Tests different division or multiple of pi, positive or negative.
 */
void GsbIeee754Test_cos(void) {
	// arguments of tested function
	double x;

	// return of tested function
	double res;

	// +Infinite double mask
	uint64_t plusInfinityBtmsk = 0x7FF0000000000000ULL;
	// - Infinite double mask
	uint64_t minusInfinityBtmsk = 0xFFF0000000000000ULL;
	// NaN value mask, with sign bit set to 0
	uint64_t naNBtmsk = 0x7FFE000000000000ULL;

	// Special Cases

	// Special test n°01 : +INF
	x = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::cos(x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFFE00000000000ULL);

	// Special test n°02 : -INF
	x = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::cos(x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFFE00000000000ULL);

	// Special test n°03 : NaN
	x = *(reinterpret_cast<double*>(&naNBtmsk));
	res = GsbIeee754::cos(x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000000000ULL);

	// Special test n°04 : 0.0
	x = 0.0;
	res = GsbIeee754::cos(x);
	CU_ASSERT_EQUAL(res, 1.0);

	// Special test n°05 : -0.0
	x = -0.0;
	res = GsbIeee754::cos(x);
	CU_ASSERT_EQUAL(res, 1.0);

	// Nominal Cases

	// Test n°01 : 0x3FF921FB54442D1 (pi/2)
	x = 1.5707963267948966;
	res = GsbIeee754::cos(x);
	// res = 0x3C91A62633145C07
	CU_ASSERT_EQUAL(res, 6.123233995736766e-17);

	// Test n°02 : 0x3FF0C152382D736 (pi/3)
	x = 1.0471975511965976;
	res = GsbIeee754::cos(x);
	// res = 0x3FE0000000000001
	CU_ASSERT_EQUAL(res, 0.5000000000000001);

	// Test n°03 : 0x3FE921FB54442D1 (pi/4)
	x = 0.7853981633974483;
	res = GsbIeee754::cos(x);
	// res = 0x3FE6A09E667F3BCD
	CU_ASSERT_EQUAL(res, 0.7071067811865476);

	// Test n°04 : 0x3FE41B2F769CF0E (pi/5)
	x = 0.6283185307179586;
	res = GsbIeee754::cos(x);
	// res = 0x3FE9E3779B97F4A8
	CU_ASSERT_EQUAL(res, 0.8090169943749475);

	// Test n°05 : 0x3FE0C152382D736 (pi/6)
	x = 0.5235987755982988;
	res = GsbIeee754::cos(x);
	// res = 0x3FEBB67AE8584CAB
	CU_ASSERT_EQUAL(res, 0.8660254037844387);

	// Test n°06 : 0x3FD921FB54442D1 (pi/8)
	x = 0.39269908169872414;
	res = GsbIeee754::cos(x);
	// res = 0x3FED906BCF328D46
	CU_ASSERT_EQUAL(res, 0.9238795325112867);

	// Test n°07 : 0x3FD41B2F769CF0E (pi/10)
	x = 0.3141592653589793;
	res = GsbIeee754::cos(x);
	// res = 0x3FEE6F0E134454FF
	CU_ASSERT_EQUAL(res, 0.9510565162951535);

	// Test n°08 : 0x3FD0C152382D736 (pi/12)
	x = 0.2617993877991494;
	res = GsbIeee754::cos(x);
	// res = 0x3FEEE8DD4748BF15
	CU_ASSERT_EQUAL(res, 0.9659258262890683);

	// Test n°09 : 0x4002D97C7F3321D (3pi/4)
	x = 2.356194490192345;
	res = GsbIeee754::cos(x);
	// res = 0xBFE6A09E667F3BCC
	CU_ASSERT_EQUAL(res, -0.7071067811865475);

	// Test n°10 : 0x4000C152382D736 (2pi/3)
	x = 2.0943951023931953;
	res = GsbIeee754::cos(x);
	// res = 0xBFDFFFFFFFFFFFFC
	CU_ASSERT_EQUAL(res, -0.4999999999999998);

	// Test n°11 : 0x4004F1A6C638D03 (5pi/6)
	x = 2.6179938779914944;
	res = GsbIeee754::cos(x);
	// res = 0xBFEBB67AE8584CAB
	CU_ASSERT_EQUAL(res, -0.8660254037844387);

	// Test n°12 : 0x400F6A7A2955385 (5pi/4)
	x = 3.9269908169872414;
	res = GsbIeee754::cos(x);
	// res = 0xBFE6A09E667F3BCE
	CU_ASSERT_EQUAL(res, -0.7071067811865477);

	// Test n°13 : 0x4010C152382D736 (4pi/3)
	x = 4.1887902047863905;
	res = GsbIeee754::cos(x);
	// res = 0xBFE0000000000004
	CU_ASSERT_EQUAL(res, -0.5000000000000004);

	// Test n°14 : 0x400D524FE24F89F (7pi/6)
	x = 3.665191429188092;
	res = GsbIeee754::cos(x);
	// res = 0xBFEBB67AE8584CAC
	CU_ASSERT_EQUAL(res, -0.8660254037844388);

	// Test n°15 : 0x4015FDBBE9BBA77 (7pi/4)
	x = 5.497787143782138;
	res = GsbIeee754::cos(x);
	// res = 0x3FE6A09E667F3BCB
	CU_ASSERT_EQUAL(res, 0.7071067811865474);

	// Test n°16 : 0x4014F1A6C638D03 (5pi/3)
	x = 5.235987755982989;
	res = GsbIeee754::cos(x);
	// res = 0x3FE0000000000001
	CU_ASSERT_EQUAL(res, 0.5000000000000001);

	// Test n°17 : 0x401709D10D3E7EA (11pi/6)
	x = 5.759586531581287;
	res = GsbIeee754::cos(x);
	// res = 0x3FEBB67AE8584CA8
	CU_ASSERT_EQUAL(res, 0.8660254037844384);

	// Test n°18 : 0x401921FB54442D1 (2pi)
	x = 6.283185307179586;
	res = GsbIeee754::cos(x);
	// res = 0x3FF0000000000000
	CU_ASSERT_EQUAL(res, 1.0);

	// Test n°19 : 0x400921FB54442D1 (pi)
	x = 3.141592653589793;
	res = GsbIeee754::cos(x);
	// res = 0xBFF0000000000000
	CU_ASSERT_EQUAL(res, -1.0);

	// Test n°20 : 0xC01921FB54442D1 (-2pi)
	x = -6.283185307179586;
	res = GsbIeee754::cos(x);
	// res = 0x3FF0000000000000
	CU_ASSERT_EQUAL(res, 1.0);

	// Test n°21 : 0xC00921FB54442D1 (-pi)
	x = -3.141592653589793;
	res = GsbIeee754::cos(x);
	// res = 0xBFF0000000000000
	CU_ASSERT_EQUAL(res, -1.0);

	// Test n°22 : 0xBFF921FB54442D1 (-pi/2)
	x = -1.5707963267948966;
	res = GsbIeee754::cos(x);
	// res = 0x3C91A62633145C07
	CU_ASSERT_EQUAL(res, 6.123233995736766e-17);

	// Test n°23 : 0xBFF0C152382D736 (-pi/3)
	x = -1.0471975511965976;
	res = GsbIeee754::cos(x);
	// res = 0x3FE0000000000001
	CU_ASSERT_EQUAL(res, 0.5000000000000001);

	// Test n°24 : 0xBFE921FB54442D1 (-pi/4)
	x = -0.7853981633974483;
	res = GsbIeee754::cos(x);
	// res = 0x3FE6A09E667F3BCD
	CU_ASSERT_EQUAL(res, 0.7071067811865476);

	// Test n°25 : 0xBFE0C152382D736 (-pi/6)
	x = -0.5235987755982988;
	res = GsbIeee754::cos(x);
	// res = 0x3FEBB67AE8584CAB
	CU_ASSERT_EQUAL(res, 0.8660254037844387);

	// Test n°26 : 0xBFD921FB54442D1 (-pi/8)
	x = -0.39269908169872414;
	res = GsbIeee754::cos(x);
	// res = 0x3FED906BCF328D46
	CU_ASSERT_EQUAL(res, 0.9238795325112867);

	// Test n°27 : 0x4022D97C7F3321D (3pi)
	x = 9.42477796076938;
	res = GsbIeee754::cos(x);
	// res = 0xBFF0000000000000
	CU_ASSERT_EQUAL(res, -1.0);

	// Test n°28 : 0x403F6A7A2955385 (10pi)
	x = 31.41592653589793;
	res = GsbIeee754::cos(x);
	// res = 0x3FF0000000000000
	CU_ASSERT_EQUAL(res, 1.0);

	// Test n°29 : 0xC07ED3AE455B9F4 (-157pi)
	x = -493.2300466135975;
	res = GsbIeee754::cos(x);
	// res = 0xBFF0000000000000
	CU_ASSERT_EQUAL(res, -1.0);

	// Test n°30 : 0xC0B9C55835E7E83 (-2100pi)
	x = -6597.344572538565;
	res = GsbIeee754::cos(x);
	// res = 0x3FF0000000000000
	CU_ASSERT_EQUAL(res, 1.0);

	// Test n°31 : 0x40E2CBBDE9C10C8 (12253pi)
	x = 38493.93478443573;
	res = GsbIeee754::cos(x);
	// res = 0xBFF0000000000000
	CU_ASSERT_EQUAL(res, -1.0);
}

/**
 * @brief  	Test of GsbIeee754::sin(double x)
 * @pre		Initialize a double x, input of the function
 * @result	The sine of x
 *
 * @UT_Purpose			Test the GsbIeee754::sin function with several inputs.
 * @UT_Tested_function	int32_t GsbIeee754::sin(double x); 
 * @UT_Type				Functionality
 * @UT_Description		- Special cases : +-Inf, NaN, +-0.0
 * 						- Tests different division or multiple of pi, positive or negative.
 */
void GsbIeee754Test_sin(void) {
	// arguments of tested function
	double x;

	// return of tested function
	double res;

	// +Infinite double mask
	uint64_t plusInfinityBtmsk = 0x7FF0000000000000ULL;
	// - Infinite double mask
	uint64_t minusInfinityBtmsk = 0xFFF0000000000000ULL;
	// NaN value mask, with sign bit set to 0
	uint64_t naNBtmsk = 0x7FFE000000000000ULL;

	// Special Cases

	// Special test n°01 : +INF
	x = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::sin(x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFFE00000000000ULL);

	// Special test n°02 : -INF
	x = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::sin(x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFFE00000000000ULL);

	// Special test n°03 : NaN
	x = *(reinterpret_cast<double*>(&naNBtmsk));
	res = GsbIeee754::sin(x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000000000ULL);

	// Special test n°04: 0.0
	x = 0.0;
	res = GsbIeee754::sin(x);
	// res = 0.0
	CU_ASSERT_EQUAL(res, 0.0);

	// Special test n°05: -0.0
	x = -0.0;
	res = GsbIeee754::sin(x);
	// res = -0.0
	CU_ASSERT_EQUAL(res, -0.0);

	// Nominal Cases

	// Test n°01: 0x3FF921FB54442D18 (pi/2)
	x = 1.5707963267948966;
	res = GsbIeee754::sin(x);
	// res = 0x3FF0000000000000
	CU_ASSERT_EQUAL(res, 1.0);

	// Test n°02: 0x3FF0C152382D7365 (pi/3)
	x = 1.0471975511965976;
	res = GsbIeee754::sin(x);
	// res = 0x3FEBB67AE8584CAA
	CU_ASSERT_EQUAL(res, 0.8660254037844386);

	// Test n°03: 0x3FE921FB54442D18 (pi/4)
	x = 0.7853981633974483;
	res = GsbIeee754::sin(x);
	// res = 0x3FE6A09E667F3BCC
	CU_ASSERT_EQUAL(res, 0.7071067811865475);

	// Test n°04: 0x3FE41B2F769CF0E0 (pi/5)
	x = 0.6283185307179586;
	res = GsbIeee754::sin(x);
	// res = 0x3FE2CF2304755A5E
	CU_ASSERT_EQUAL(res, 0.5877852522924731);

	// Test n°05: 0x3FE0C152382D7365 (pi/6)
	x = 0.5235987755982988;
	res = GsbIeee754::sin(x);
	// res = 0x3FDFFFFFFFFFFFFF
	CU_ASSERT_EQUAL(res, 0.49999999999999994);

	// Test n°06: 0x3FD921FB54442D18 (pi/8)
	x = 0.39269908169872414;
	res = GsbIeee754::sin(x);
	// res = 0x3FD87DE2A6AEA963
	CU_ASSERT_EQUAL(res, 0.3826834323650898);

	// Test n°07: 0x3FD41B2F769CF0E0 (pi/10)
	x = 0.3141592653589793;
	res = GsbIeee754::sin(x);
	// res = 0x3FD3C6EF372FE94F
	CU_ASSERT_EQUAL(res, 0.3090169943749474);

	// Test n°08: 0x3FD0C152382D7365 (pi/12)
	x = 0.2617993877991494;
	res = GsbIeee754::sin(x);
	// res = 0x3FD0907DC1930690
	CU_ASSERT_EQUAL(res, 0.25881904510252074);

	// Test n°09: 0x4002D97C7F3321D2 (3pi/4)
	x = 2.356194490192345;
	res = GsbIeee754::sin(x);
	// res = 0x3FE6A09E667F3BCD
	CU_ASSERT_EQUAL(res, 0.7071067811865476);

	// Test n°10: 0x4000C152382D7365 (2pi/3)
	x = 2.0943951023931953;
	res = GsbIeee754::sin(x);
	// res = 0x3FEBB67AE8584CAB
	CU_ASSERT_EQUAL(res, 0.8660254037844387);

	// Test n°11: 0x4004F1A6C638D03F (5pi/6)
	x = 2.6179938779914944;
	res = GsbIeee754::sin(x);
	// res = 0x3FDFFFFFFFFFFFFF
	CU_ASSERT_EQUAL(res, 0.49999999999999994);

	// Test n°12: 0x400F6A7A2955385E (5pi/4)
	x = 3.9269908169872414;
	res = GsbIeee754::sin(x);
	// res = 0xBFE6A09E667F3BCC
	CU_ASSERT_EQUAL(res, -0.7071067811865475);

	// Test n°13: 0x4010C152382D7365 (4pi/3)
	x = 4.1887902047863905;
	res = GsbIeee754::sin(x);
	// res = 0xBFEBB67AE8584CA9
	CU_ASSERT_EQUAL(res, -0.8660254037844385);

	// Test n°14: 0x400D524FE24F89F1 (7pi/6)
	x = 3.665191429188092;
	res = GsbIeee754::sin(x);
	// res = 0xBFDFFFFFFFFFFFFB
	CU_ASSERT_EQUAL(res, -0.4999999999999997);

	// Test n°15: 0x4015FDBBE9BBA775 (7pi/4)
	x = 5.497787143782138;
	res = GsbIeee754::sin(x);
	// res = 0xBFE6A09E667F3BCE
	CU_ASSERT_EQUAL(res, -0.7071067811865477);

	// Test n°16: 0x4014F1A6C638D03F (5pi/3)
	x = 5.235987755982989;
	res = GsbIeee754::sin(x);
	// res = 0xBFEBB67AE8584CAA
	CU_ASSERT_EQUAL(res, -0.8660254037844386);

	// Test n°17: 0x401709D10D3E7EAB (11pi/6)
	x = 5.759586531581287;
	res = GsbIeee754::sin(x);
	// res = 0xBFE0000000000004
	CU_ASSERT_EQUAL(res, -0.5000000000000004);

	// Test n°18: 0x401921FB54442D18 (2pi)
	x = 6.283185307179586;
	res = GsbIeee754::sin(x);
	// res = 0xBCB1A62633145C07
	CU_ASSERT_EQUAL(res, -2.4492935982947064e-16);

	// Test n°19: 0x400921FB54442D18 (pi)
	x = 3.141592653589793;
	res = GsbIeee754::sin(x);
	// res = 0x3CA1A62633145C07
	CU_ASSERT_EQUAL(res, 1.2246467991473532e-16);

	// Test n°20: 0xC01921FB54442D18 (-2pi)
	x = -6.283185307179586;
	res = GsbIeee754::sin(x);
	// res = 0x3CB1A62633145C07
	CU_ASSERT_EQUAL(res, 2.4492935982947064e-16);

	// Test n°21: 0xC00921FB54442D18 (-pi)
	x = -3.141592653589793;
	res = GsbIeee754::sin(x);
	// res = 0xBCA1A62633145C07
	CU_ASSERT_EQUAL(res, -1.2246467991473532e-16);

	// Test n°22: 0xBFF921FB54442D18 (-pi/2)
	x = -1.5707963267948966;
	res = GsbIeee754::sin(x);
	// res = 0xBFF0000000000000
	CU_ASSERT_EQUAL(res, -1.0);

	// Test n°23: 0xBFF0C152382D7365 (-pi/3)
	x = -1.0471975511965976;
	res = GsbIeee754::sin(x);
	// res = 0xBFEBB67AE8584CAA
	CU_ASSERT_EQUAL(res, -0.8660254037844386);

	// Test n°24: 0xBFE921FB54442D18 (-pi/4)
	x = -0.7853981633974483;
	res = GsbIeee754::sin(x);
	// res = 0xBFE6A09E667F3BCC
	CU_ASSERT_EQUAL(res, -0.7071067811865475);

	// Test n°25: 0xBFE0C152382D7365 (-pi/6)
	x = -0.5235987755982988;
	res = GsbIeee754::sin(x);
	// res = 0xBFDFFFFFFFFFFFFF
	CU_ASSERT_EQUAL(res, -0.49999999999999994);

	// Test n°26: 0xBFD921FB54442D18 (-pi/8)
	x = -0.39269908169872414;
	res = GsbIeee754::sin(x);
	// res = 0xBFD87DE2A6AEA963
	CU_ASSERT_EQUAL(res, -0.3826834323650898);

	// Test n°27: 0x4022D97C7F3321D2 (3pi)
	x = 9.42477796076938;
	res = GsbIeee754::sin(x);
	// res = 0x3CBA79394C9E8A0A
	CU_ASSERT_EQUAL(res, 3.6739403974420594e-16);

	// Test n°28: 0x403F6A7A2955385E (10pi)
	x = 31.41592653589793;
	res = GsbIeee754::sin(x);
	// res = 0xBCD60FAFBFD97309
	CU_ASSERT_EQUAL(res, -1.2246467991473533e-15);

	// Test n°29: 0xC07ED3AE455B9F4F (-157pi)
	x = -493.2300466135975;
	res = GsbIeee754::sin(x);
	// res = 0xBD28D2E56D537C70
	CU_ASSERT_EQUAL(res, -4.409595049821695e-14);

	// Test n°30: 0xC0B9C55835E7E83D (-2100pi)
	x = -6597.344572538565;
	res = GsbIeee754::sin(x);
	// res = 0x3D5918DE2B60605D
	CU_ASSERT_EQUAL(res, 3.566518108273582e-13);

	// Test n°31: 0x40E2CBBDE9C10C89 (12253pi)
	x = 38493.93478443573;
	res = GsbIeee754::sin(x);
	// res = 0x3D91757AE4B5AAF1
	CU_ASSERT_EQUAL(res, 3.9696957297616e-12);
}

/**
 * @brief  	Test of GsbIeee754::atan(double x)
 * @pre		Initialize a double x, input of the function
 * @result	The arc tangent of x
 *
 * @UT_Purpose			Test the GsbIeee754::atan function with several inputs.
 * @UT_Tested_function	int32_t GsbIeee754::atan(double x);
 * @UT_Type				Functionality
 * @UT_Description		- Special cases : +-Inf, NaN, +-0.0
 * 						- Tests with values >2^66, <2^-27 to reach the limits
 * 						- Several values in the following ranges : [0,7/16], [7/16,11/16], [11/16.19/16], [19/16,39/16], [39/16,INF] 
 */
void GsbIeee754Test_atan(void) {
	// arguments of tested function
	double x;

	// return of tested function
	double res;

	// +Infinite double mask
	uint64_t plusInfinityBtmsk = 0x7FF0000000000000ULL;
	// - Infinite double mask
	uint64_t minusInfinityBtmsk = 0xFFF0000000000000ULL;
	// NaN value mask, with sign bit set to 0
	uint64_t naNBtmsk = 0x7FFE000000000000ULL;

	// Special Cases

	// Special test n°01 : +INF
	x = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::atan(x);
	// res = pi/2
	CU_ASSERT_EQUAL(res, 1.5707963267948966);

	// Special test n°02 : -INF
	x = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::atan(x);
	// res = -pi/2
	CU_ASSERT_EQUAL(res, -1.5707963267948966);

	// Special test n°03 : NaN
	x = *(reinterpret_cast<double*>(&naNBtmsk));
	res = GsbIeee754::atan(x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000000000ULL);

	// Special test  n°04 0.0
	x = 0.0;
	res = GsbIeee754::atan(x);
	CU_ASSERT_EQUAL(res, 0.0);

	// Special test  n°05 x = -0.0
	x = -0.0;
	res = GsbIeee754::atan(x);
	CU_ASSERT_EQUAL(res, -0.0);

	// Nominal Cases

	// Test n°01 x = 0x434D02967C31CDB5
	x = 1.633123935319537E+16;
	res = GsbIeee754::atan(x);
	// res = 0x3FF921FB54442D18
	CU_ASSERT_EQUAL(res, 1.5707963267948966);

	// Test n°02 x = 0x3FFBB67AE8584CA8
	x = 1.7320508075688767;
	res = GsbIeee754::atan(x);
	// res = 0x3FF0C152382D7365
	CU_ASSERT_EQUAL(res, 1.0471975511965976);

	// Test n°03 x = 0x3FEFFFFFFFFFFFFF
	x = 0.9999999999999999;
	res = GsbIeee754::atan(x);
	// res = 0x3FE921FB54442D18
	CU_ASSERT_EQUAL(res, 0.7853981633974483);

	// Test n°04 x = 0x3FE73FD61D9DF542
	x = 0.7265425280053608;
	res = GsbIeee754::atan(x);
	// res = 0x3FE41B2F769CF0E0
	CU_ASSERT_EQUAL(res, 0.6283185307179586);

	// Test n°05 x = 0x3FE279A74590331C
	x = 0.5773502691896257;
	res = GsbIeee754::atan(x);
	// res = 0x3FE0C152382D7365
	CU_ASSERT_EQUAL(res, 0.5235987755982988);

	// Test n°06 x = 0x3FDA827999FCEF32
	x = 0.41421356237309503;
	res = GsbIeee754::atan(x);
	// res = 0x3FD921FB54442D18
	CU_ASSERT_EQUAL(res, 0.39269908169872414);

	// Test n°07 x = 0x3FD4CB7BFB4961AE
	x = 0.3249196962329063;
	res = GsbIeee754::atan(x);
	// res = 0x3FD41B2F769CF0E0
	CU_ASSERT_EQUAL(res, 0.3141592653589793);

	// Test n°08 x = 0x3FD126145E9ECD56
	x = 0.2679491924311227;
	res = GsbIeee754::atan(x);
	// res = 0x3FD0C152382D7365
	CU_ASSERT_EQUAL(res, 0.2617993877991494);

	// Test n°09 x = 0xBFF0000000000001
	x = -1.0000000000000002;
	res = GsbIeee754::atan(x);
	// res = 0xBFE921FB54442D19
	CU_ASSERT_EQUAL(res, -0.7853981633974484);

	// Test n°10 x = 0xBFFBB67AE8584CAF
	x = -1.7320508075688783;
	res = GsbIeee754::atan(x);
	// res = 0xBFF0C152382D7367
	CU_ASSERT_EQUAL(res, -1.047197551196598);

	// Test n°11 x = 0xBFE279A74590331C
	x = -0.5773502691896257;
	res = GsbIeee754::atan(x);
	// res = 0xBFE0C152382D7365
	CU_ASSERT_EQUAL(res, -0.5235987755982988);

	// Test n°12 x = 0x3FEFFFFFFFFFFFFD
	x = 0.9999999999999997;
	res = GsbIeee754::atan(x);
	// res = 0x3FE921FB54442D17
	CU_ASSERT_EQUAL(res, 0.7853981633974482);

	// Test n°13 x = 0x3FFBB67AE8584CA2
	x = 1.7320508075688754;
	res = GsbIeee754::atan(x);
	// res = 0x3FF0C152382D7363
	CU_ASSERT_EQUAL(res, 1.0471975511965972);

	// Test n°14 x = 0x3FE279A745903319
	x = 0.5773502691896254;
	res = GsbIeee754::atan(x);
	// res = 0x3FE0C152382D7363
	CU_ASSERT_EQUAL(res, 0.5235987755982986);

	// Test n°15 x = 0xBFF0000000000002
	x = -1.0000000000000004;
	res = GsbIeee754::atan(x);
	// res = 0xBFE921FB54442D1A
	CU_ASSERT_EQUAL(res, -0.7853981633974485);

	// Test n°16 x = 0xBFFBB67AE8584CA9
	x = -1.732050807568877;
	res = GsbIeee754::atan(x);
	// res = 0xBFF0C152382D7365
	CU_ASSERT_EQUAL(res, -1.0471975511965976);

	// Test n°17 x = 0xBFE279A745903322
	x = -0.5773502691896264;
	res = GsbIeee754::atan(x);
	// res = 0xBFE0C152382D736A
	CU_ASSERT_EQUAL(res, -0.5235987755982994);

	// Test n°18 x = 0xBCB1A62633145C07
	x = -2.4492935982947064E-16;
	res = GsbIeee754::atan(x);
	// res = 0xBCB1A62633145C07
	CU_ASSERT_EQUAL(res, -2.4492935982947064e-16);

	// Test n°19 x = 0xBCA1A62633145C07
	x = -1.2246467991473532E-16;
	res = GsbIeee754::atan(x);
	// res = 0xBCA1A62633145C07
	CU_ASSERT_EQUAL(res, -1.2246467991473532e-16);

	// Test n°20 x = 0x3CB1A62633145C07
	x = 2.4492935982947064E-16;
	res = GsbIeee754::atan(x);
	// res = 0x3CB1A62633145C07
	CU_ASSERT_EQUAL(res, 2.4492935982947064e-16);

	// Test n°21 x = 0x3CA1A62633145C07
	x = 1.2246467991473532E-16;
	res = GsbIeee754::atan(x);
	// res = 0x3CA1A62633145C07
	CU_ASSERT_EQUAL(res, 1.2246467991473532e-16);

	// Test n°22 x = 0xC34D02967C31CDB5
	x = -1.633123935319537E+16;
	res = GsbIeee754::atan(x);
	// res = 0xBFF921FB54442D18
	CU_ASSERT_EQUAL(res, -1.5707963267948966);

	// Test n°23 x = 0xBFFBB67AE8584CA8
	x = -1.7320508075688767;
	res = GsbIeee754::atan(x);
	// res = 0xBFF0C152382D7365
	CU_ASSERT_EQUAL(res, -1.0471975511965976);

	// Test n°24 x = 0xBFEFFFFFFFFFFFFF
	x = -0.9999999999999999;
	res = GsbIeee754::atan(x);
	// res = 0xBFE921FB54442D18
	CU_ASSERT_EQUAL(res, -0.7853981633974483);

	// Test n°25 x = 0xBFE279A74590331C
	x = -0.5773502691896257;
	res = GsbIeee754::atan(x);
	// res = 0xBFE0C152382D7365
	CU_ASSERT_EQUAL(res, -0.5235987755982988);

	// Test n°26 x = 0xBFDA827999FCEF32
	x = -0.41421356237309503;
	res = GsbIeee754::atan(x);
	// res = 0xBFD921FB54442D18
	CU_ASSERT_EQUAL(res, -0.39269908169872414);

	// Test n°27 x = 0x7FF000007FF00000
	x = *(reinterpret_cast<double*>(&naNBtmsk));
	res = GsbIeee754::atan(x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000000000ULL);

	// Test n°28 x = 0x7FF000017FF00000
	x = *(reinterpret_cast<double*>(&naNBtmsk));
	res = GsbIeee754::atan(x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000000000ULL);

	// Test n°29 x = 0x7FEF00017FF00000
	x = 1.7415165099989181E+308;
	res = GsbIeee754::atan(x);
	// res = 0x3FF921FB54442D18
	CU_ASSERT_EQUAL(res, 1.5707963267948966);
}

/**
 * @brief  	Test of GsbIeee754::atan2(double x)
 * @pre		Initialize two double, y and x
 * @result	atan2(y,x)
 *
 * @UT_Purpose			Test the GsbIeee754::atan2 function with several inputs.
 * @UT_Tested_function	int32_t GsbIeee754::atan2(double y, double x);
 * @UT_Type				Functionality
 * @UT_Description		- Special cases : every combination of x and y equals to {+-Inf, NaN, +-0.0, +-1.0}
 * 						- Tests with several values of y and x, positive or negative
 * 						- |y/x| >  2**60; 0 > |y|/x > -2**60, or simple value which will result in a call of atan()
 */
void GsbIeee754Test_atan2(void) {
	// arguments of tested function
	double x;
	double y;

	// return of tested function
	double res;

	uint64_t xBtMsk;
	uint64_t yBtMsk;
	// +Infinite double mask
	uint64_t plusInfinityBtmsk = 0x7FF0000000000000ULL;
	// - Infinite double mask
	uint64_t minusInfinityBtmsk = 0xFFF0000000000000ULL;
	// NaN value mask, with sign bit set to 0
	uint64_t naNBtmsk = 0x7FFE000000000000ULL;

	// Special Cases

	// Special test n°01 : x = 0xBFF0000000000000, y = 0x8000000000000000
	y = -0.0;
	x = -1.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xC00921FB54442D18
	CU_ASSERT_EQUAL(res, -3.141592653589793);

	// Special test n°02 : x = 0xBFF0000000000000, y = 0x0000000000000000
	y = 0.0;
	x = -1.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x400921FB54442D18
	CU_ASSERT_EQUAL(res, 3.141592653589793);

	// Special test n°03 : x = 0x8000000000000000, y = 0x8000000000000000
	y = -0.0;
	x = -0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xC00921FB54442D18
	CU_ASSERT_EQUAL(res, -3.141592653589793);

	// Special test n°04 : x = 0x8000000000000000, y = 0x0000000000000000
	y = 0.0;
	x = -0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x400921FB54442D18
	CU_ASSERT_EQUAL(res, 3.141592653589793);

	// Special test n°05 : x = 0x0000000000000000, y = 0xBFF0000000000000
	y = -1.0;
	x = 0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xBFF921FB54442D18
	CU_ASSERT_EQUAL(res, -1.5707963267948966);

	// Special test n°06 : x = 0x8000000000000000, y = 0xBFF0000000000000
	y = -1.0;
	x = -0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xBFF921FB54442D18
	CU_ASSERT_EQUAL(res, -1.5707963267948966);

	// Special test n°07 : x = 0x0000000000000000, y = 0x3FF0000000000000
	y = 1.0;
	x = 0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3FF921FB54442D18
	CU_ASSERT_EQUAL(res, 1.5707963267948966);

	// Special test n°08 : x = 0x8000000000000000, y = 0x3FF0000000000000
	y = 1.0;
	x = -0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3FF921FB54442D18
	CU_ASSERT_EQUAL(res, 1.5707963267948966);

	// Special test n°09 : x = 0x0000000000000000, y = 0x8000000000000000
	y = -0.0;
	x = 0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x8000000000000000
	CU_ASSERT_EQUAL(res, -0.0);

	// Special test n°10 : x = 0x0000000000000000, y = 0x0000000000000000
	y = 0.0;
	x = 0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x0000000000000000
	CU_ASSERT_EQUAL(res, 0.0);

	// Special test n°11 : x = 0x3FF0000000000000, y = 0x8000000000000000
	y = -0.0;
	x = 1.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x8000000000000000
	CU_ASSERT_EQUAL(res, -0.0);

	// Special test n°12 : x = 0x3FF0000000000000, y = 0x0000000000000000
	y = 0.0;
	x = 1.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x0000000000000000
	CU_ASSERT_EQUAL(res, 0.0);

	// Special test n°13 : x = INF, y = 0xBFF0000000000000
	y = -1.0;
	x = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0xC00921FB54442D18
	CU_ASSERT_EQUAL(res, -3.141592653589793);

	// Special test n°14 : x = INF, y = 0x3FF0000000000000
	y = 1.0;
	x = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0x400921FB54442D18
	CU_ASSERT_EQUAL(res, 3.141592653589793);

	// Special test n°15 : x = INF, y = 0xBFF0000000000000
	y = -1.0;
	x = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0x8000000000000000
	CU_ASSERT_EQUAL(res, -0.0);

	// Special test n°16 : x = INF, y = 0x3FF0000000000000
	y = 1.0;
	x = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0x0000000000000000
	CU_ASSERT_EQUAL(res, 0.0);

	// Special test n°17 : x = 0x4000000000000000, y = INF
	y = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	x = 2.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xBFF921FB54442D18
	CU_ASSERT_EQUAL(res, -1.5707963267948966);

	// Special test n°18 : x = 0x4000000000000000, y = INF
	y = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	x = 2.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3FF921FB54442D18
	CU_ASSERT_EQUAL(res, 1.5707963267948966);

	// Special test n°19 : x = INF, y = INF
	y = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	x = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0xC002D97C7F3321D2
	CU_ASSERT_EQUAL(res, -2.356194490192345);

	// Special test n°20 : x = INF, y = INF
	y = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	x = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0x4002D97C7F3321D2
	CU_ASSERT_EQUAL(res, 2.356194490192345);

	// Special test n°21 : x = INF, y = INF
	y = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	x = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0xBFE921FB54442D18
	CU_ASSERT_EQUAL(res, -0.7853981633974483);

	// Special test n°22 : x = INF, y = INF
	y = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	x = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0x3FE921FB54442D18
	CU_ASSERT_EQUAL(res, 0.7853981633974483);

	// Special test n°23 : x = NaN, y = 0x4008000000000000
	y = 3.0;
	x = *(reinterpret_cast<double*>(&naNBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000000000ULL);

	// Special test n°24 : x = 0x4008000000000000, y = NaN
	y = *(reinterpret_cast<double*>(&naNBtmsk));
	x = 3.0;
	res = GsbIeee754::atan2(y, x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000000000ULL);

	// Special test : x = NaN, y = 0x4008000000000000
	y = 3.0;
	xBtMsk = 0x7FFE000000000000ULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	res = GsbIeee754::atan2(y, x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000000000ULL);

	// Special test : x = 0x4008000000000000, y = NaN
	yBtMsk = 0x7FFE000000000000ULL;
	y = *(reinterpret_cast<double*>(&yBtMsk));
	x = 3.0;
	res = GsbIeee754::atan2(y, x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000000000ULL);

	// Special test : x = NaN, y = 0x4008000000000000
	y = 3.0;
	xBtMsk = 0x7FFE000000100000ULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	res = GsbIeee754::atan2(y, x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000100000ULL);

	// Special test : x = 0x4008000000000000, y = NaN
	yBtMsk = 0x7FFE000001000000ULL;
	y = *(reinterpret_cast<double*>(&yBtMsk));
	x = 3.0;
	res = GsbIeee754::atan2(y, x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000001000000ULL);

	// Special test : x = NaN, y = 0x4008000000000000
	y = 3.0;
	xBtMsk = 0x7FFE0F0000000000ULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	res = GsbIeee754::atan2(y, x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE0F0000000000ULL);

	// Special test : x = 0x4008000000000000, y = NaN
	yBtMsk = 0x7FFE0F0000000000ULL;
	y = *(reinterpret_cast<double*>(&yBtMsk));
	x = 3.0;
	res = GsbIeee754::atan2(y, x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE0F0000000000ULL);

	// Special test : x = NaN, y = 0x4008000000000000
	y = 3.0;
	xBtMsk = 0x7FFE000000000001ULL;
	x = *(reinterpret_cast<double*>(&xBtMsk));
	res = GsbIeee754::atan2(y, x);
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000000001ULL);
	// res = NaN

	// Special test : x = 0x4008000000000000, y = NaN
	yBtMsk = 0x7FFE000000000001ULL;
	y = *(reinterpret_cast<double*>(&yBtMsk));
	x = 3.0;
	res = GsbIeee754::atan2(y, x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000000001ULL);

	// Nominal Cases

	// Test n°01 : x = 0xC039000000000000, y = 0x8000000000000000
	y = -0.0;
	x = -25.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xC00921FB54442D18
	CU_ASSERT_EQUAL(res, -3.141592653589793);

	// Test n°02 : x = 0xC061200000000000, y = 0x8000000000000000
	y = -0.0;
	x = -137;
	res = GsbIeee754::atan2(y, x);
	// res = 0xC00921FB54442D18
	CU_ASSERT_EQUAL(res, -3.141592653589793);

	// Test n°03 : x = 0xC0A3240000000000, y = 0x8000000000000000
	y = -0.0;
	x = -2450;
	res = GsbIeee754::atan2(y, x);
	// res = 0xC00921FB54442D18
	CU_ASSERT_EQUAL(res, -3.141592653589793);

	// Test n°04 : x = 0xC039000000000000, y = 0x0000000000000000
	y = 0.0;
	x = -25.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x400921FB54442D18
	CU_ASSERT_EQUAL(res, 3.141592653589793);

	// Test n°05 : x = 0xC061200000000000, y = 0x0000000000000000
	y = 0.0;
	x = -137.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x400921FB54442D18
	CU_ASSERT_EQUAL(res, 3.141592653589793);

	// Test n°06 : x = 0xC0A3240000000000, y = 0x0000000000000000
	y = 0.0;
	x = -2450.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x400921FB54442D18
	CU_ASSERT_EQUAL(res, 3.141592653589793);

	// Test n°07 : x = 0x8000000000000000, y = 0xC039000000000000
	y = -25.0;
	x = -0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xBFF921FB54442D18
	CU_ASSERT_EQUAL(res, -1.5707963267948966);

	// Test n°08 : x = 0x8000000000000000, y = 0xC061200000000000
	y = -137.0;
	x = -0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xBFF921FB54442D18
	CU_ASSERT_EQUAL(res, -1.5707963267948966);

	// Test n°09 : x = 0x8000000000000000, y = 0xC0A3240000000000
	y = -2450.0;
	x = -0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xBFF921FB54442D18
	CU_ASSERT_EQUAL(res, -1.5707963267948966);

	// Test n°10 : x = 0x0000000000000000, y = 0xC039000000000000
	y = -25.0;
	x = 0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xBFF921FB54442D18
	CU_ASSERT_EQUAL(res, -1.5707963267948966);

	// Test n°11 : x = 0x0000000000000000, y = 0xC061200000000000
	y = -137.0;
	x = 0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xBFF921FB54442D18
	CU_ASSERT_EQUAL(res, -1.5707963267948966);

	// Test n°12 : x = 0x0000000000000000, y = 0xC0A3240000000000
	y = -2450.0;
	x = 0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xBFF921FB54442D18
	CU_ASSERT_EQUAL(res, -1.5707963267948966);

	// Test n°13 : x = 0x8000000000000000, y = 0x4039000000000000
	y = 25.0;
	x = -0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3FF921FB54442D18
	CU_ASSERT_EQUAL(res, 1.5707963267948966);

	// Test n°14 : x = 0x8000000000000000, y = 0x4061200000000000
	y = 137.0;
	x = -0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3FF921FB54442D18
	CU_ASSERT_EQUAL(res, 1.5707963267948966);

	// Test n°15 : x = 0x8000000000000000, y = 0x40A3240000000000
	y = 2450.0;
	x = -0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3FF921FB54442D18
	CU_ASSERT_EQUAL(res, 1.5707963267948966);

	// Test n°16 : x = 0x0000000000000000, y = 0x4039000000000000
	y = 25.0;
	x = 0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3FF921FB54442D18
	CU_ASSERT_EQUAL(res, 1.5707963267948966);

	// Test n°17 : x = 0x0000000000000000, y = 0x4061200000000000
	y = 137.0;
	x = 0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3FF921FB54442D18
	CU_ASSERT_EQUAL(res, 1.5707963267948966);

	// Test n°18 : x = 0x0000000000000000, y = 0x40A3240000000000
	y = 2450.0;
	x = 0.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3FF921FB54442D18
	CU_ASSERT_EQUAL(res, 1.5707963267948966);

	// Test n°19 : x = 0x4039000000000000, y = 0x8000000000000000
	y = -0.0;
	x = 25.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x8000000000000000
	CU_ASSERT_EQUAL(res, -0.0);

	// Test n°20 : x = 0x4061200000000000, y = 0x8000000000000000
	y = -0.0;
	x = 137.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x8000000000000000
	CU_ASSERT_EQUAL(res, -0.0);

	// Test n°21 : x = 0x40A3240000000000, y = 0x8000000000000000
	y = -0.0;
	x = 2450.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x8000000000000000
	CU_ASSERT_EQUAL(res, -0.0);

	// Test n°22 : x = 0x4039000000000000, y = 0x0000000000000000
	y = 0.0;
	x = 25.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x0000000000000000
	CU_ASSERT_EQUAL(res, 0.0);

	// Test n°23 : x = 0x4061200000000000, y = 0x0000000000000000
	y = 0.0;
	x = 137.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x0000000000000000
	CU_ASSERT_EQUAL(res, 0.0);

	// Test n°24 : x = 0x40A3240000000000, y = 0x0000000000000000
	y = 0.0;
	x = 2450.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x0000000000000000
	CU_ASSERT_EQUAL(res, 0.0);

	// Test n°25 : x = INF, y = 0xC039000000000000
	y = -25.0;
	x = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0xC00921FB54442D18
	CU_ASSERT_EQUAL(res, -3.141592653589793);

	// Test n°26 : x = INF, y = 0xC061200000000000
	y = -137.0;
	x = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0xC00921FB54442D18
	CU_ASSERT_EQUAL(res, -3.141592653589793);

	// Test n°27 : x = INF, y = 0xC0A3240000000000
	y = -2450.0;
	x = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0xC00921FB54442D18
	CU_ASSERT_EQUAL(res, -3.141592653589793);

	// Test n°28 : An error was present in this test, and so it will not be reprensented in the test suite.

	// Test n°29 : x = -INF, y = 0x4061200000000000
	y = 137.0;
	x = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0x400921FB54442D18
	CU_ASSERT_EQUAL(res, 3.141592653589793);

	// Test n°30 : x = -INF, y = 0x40A3240000000000
	y = 2450.0;
	x = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0x400921FB54442D18
	CU_ASSERT_EQUAL(res, 3.141592653589793);

	// Test n°31 : x = INF, y = 0xC039000000000000
	y = -25.0;
	x = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0x8000000000000000
	CU_ASSERT_EQUAL(res, -0.0);

	// Test n°32 : x = INF, y = 0xC061200000000000
	y = -137.0;
	x = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0x8000000000000000
	CU_ASSERT_EQUAL(res, -0.0);

	// Test n°33 : x = INF, y = 0xC0A3240000000000
	y = -2450.0;
	x = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0x8000000000000000
	CU_ASSERT_EQUAL(res, -0.0);

	// Test n°34 : x = INF, y = 0x4039000000000000
	y = 25.0;
	x = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0x0000000000000000
	CU_ASSERT_EQUAL(res, 0.0);

	// Test n°35 : x = INF, y = 0x4061200000000000
	y = 137.0;
	x = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0x0000000000000000
	CU_ASSERT_EQUAL(res, 0.0);

	// Test n°36 : x = INF, y = 0x40A3240000000000
	y = 2450.0;
	x = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = 0x0000000000000000
	CU_ASSERT_EQUAL(res, 0.0);

	// Test n°37 : x = 0x4059800000000000; y = INF
	y = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	x = 102.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xBFF921FB54442D18
	CU_ASSERT_EQUAL(res, -1.5707963267948966);

	// Test n°38 : x = 0xC03B000000000000; y = INF
	y = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	x = -2.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xBFF921FB54442D18
	CU_ASSERT_EQUAL(res, -1.5707963267948966);

	// Test n°39 : x = 0x40AF400000000000; y = INF
	y = *(reinterpret_cast<double*>(&minusInfinityBtmsk));
	x = 4000;
	res = GsbIeee754::atan2(y, x);
	// res = 0xBFF921FB54442D18
	CU_ASSERT_EQUAL(res, -1.5707963267948966);

	// Test n°40 : x = 0x4059800000000000; y = INF
	y = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	x = 102.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3FF921FB54442D18
	CU_ASSERT_EQUAL(res, 1.5707963267948966);

	// Test n°41 : x = 0xC03B000000000000; y = INF
	y = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	x = -27.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3FF921FB54442D18
	CU_ASSERT_EQUAL(res, 1.5707963267948966);

	// Test n°42 : x = 0x40AF400000000000; y = INF
	y = *(reinterpret_cast<double*>(&plusInfinityBtmsk));
	x = 4000.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3FF921FB54442D18
	CU_ASSERT_EQUAL(res, 1.5707963267948966);

	// Test n°43 : x = NaN, y = 0x4059800000000000
	y = 102.0;
	x = *(reinterpret_cast<double*>(&naNBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000000000ULL);

	// Test n°44 : x = NaN, y = 0xC03B000000000000
	y = -27.0;
	x = *(reinterpret_cast<double*>(&naNBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000000000ULL);

	// Test n°45 : x = NaN, y = 0x40AF400000000000
	y = 4000.0;
	x = *(reinterpret_cast<double*>(&naNBtmsk));
	res = GsbIeee754::atan2(y, x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000000000ULL);

	// Test n°46 : x = 0x4059800000000000, y = NaN
	y = *(reinterpret_cast<double*>(&naNBtmsk));
	x = 102.0;
	res = GsbIeee754::atan2(y, x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000000000ULL);

	// Test n°47 : x = 0xC03B000000000000, y = NaN
	y = *(reinterpret_cast<double*>(&naNBtmsk));
	x = -27.0;
	res = GsbIeee754::atan2(y, x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000000000ULL);

	// Test n°48 : x = 0x40AF400000000000, y = NaN
	y = *(reinterpret_cast<double*>(&naNBtmsk));
	x = 4000.0;
	res = GsbIeee754::atan2(y, x);
	// res = NaN
	CU_ASSERT_EQUAL(*(reinterpret_cast<uint64_t*>(&res)), 0x7FFE000000000000ULL);

	// Test n°49 : x = 0xC039000000000000, y = 0xC039000000000000
	y = -25.0;
	x = -25.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xC002D97C7F3321D2
	CU_ASSERT_EQUAL(res, -2.356194490192345);

	// Test n°50 : x = 0xC061200000000000, y = 0xC039000000000000
	y = -25.0;
	x = -137.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xC007B05391622ADC
	CU_ASSERT_EQUAL(res, -2.961096893127758);

	// Test n°51 : x = 0xC0A3240000000000, y = 0xC039000000000000
	y = -25.0;
	x = -2450.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xC0090D15A3251B03
	CU_ASSERT_EQUAL(res, -3.131388926095839);

	// Test n°52 : x = 0x4061200000000000, y = 0x4039000000000000
	y = 25.0;
	x = 137.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3FC71A7C2E2023C6
	CU_ASSERT_EQUAL(res, 0.18049576046203536);

	// Test n°53 : x = 0x40A3240000000000, y = 0x4039000000000000
	y = 25.0;
	x = 2450.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3F84E5B11F121530
	CU_ASSERT_EQUAL(res, 0.010203727493954234);

	// Test n°54 : x = 0x4059800000000000, y = 0x4059800000000000
	y = 102.0;
	x = 102.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3FE921FB54442D18
	CU_ASSERT_EQUAL(res, 0.7853981633974483);

	// Test n°55 : x = 0xC03B000000000000, y = 0x4059800000000000
	y = 102.0;
	x = -27.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3FFD45E846A66DF0
	CU_ASSERT_EQUAL(res, 1.8295672187585943);

	// Test n°56 : x = 0x40AF400000000000, y = 0x4059800000000000
	y = 102.0;
	x = 4000.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3F9A1B3941EFE890
	CU_ASSERT_EQUAL(res, 0.02549447503040675);

	// Test n°57 : x = 0x40AF400000000000, y = 0xC03B000000000000
	y = -27.0;
	x = 4000.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0xBF7BA5C7CF572952
	CU_ASSERT_EQUAL(res, -0.00674989748717743);

	// Test n°58 : x = 0x4059800000000000, y = 0x40AF400000000000
	y = 4000.0;
	x = 102.0;
	res = GsbIeee754::atan2(y, x);
	// res = 0x3FF8B98E6F3C6D76
	CU_ASSERT_EQUAL(res, 1.5453018517644899);

	y = 5.564456;
	x = 1.25247;
	res = GsbIeee754::atan2(y, x);
	CU_ASSERT_EQUAL(res, 1.3494019359229046);

	y = 1981.16198198;
	x = 1.132465798;
	res = GsbIeee754::atan2(y, x);
	CU_ASSERT_EQUAL(res, 1.570224709892817);

	y = 123.46579;
	x = 321654.9879;
	res = GsbIeee754::atan2(y, x);
	CU_ASSERT_EQUAL(res, 0.00038384538894413103423883248944);

	y = 3.14159265;
	x = 56.2951413;
	res = GsbIeee754::atan2(y, x);
	CU_ASSERT_EQUAL(res, 0.05574792761092832);

	y = 0.9999999999999999999999;
	x = 12.0;
	res = GsbIeee754::atan2(y, x);
	CU_ASSERT_EQUAL(res, 0.08314123188844121936647013626498);

	y = -0.000000000000000000001;
	x = 100000000000000.0;
	res = GsbIeee754::atan2(y, x);
	CU_ASSERT_EQUAL(res, -9.999999999999999e-36);

	y = 0.000000000000000000001;
	x = -100000000000000.0;
	res = GsbIeee754::atan2(y, x);
	CU_ASSERT_EQUAL(res, 3.141592653589793);

	y = 0.000000000000000000001;
	x = 100000000000000.0;
	res = GsbIeee754::atan2(y, x);
	CU_ASSERT_EQUAL(res, 9.999999999999999e-36);

	y = -0.000000000000000000001;
	x = -100000000000000.0;
	res = GsbIeee754::atan2(y, x);
	CU_ASSERT_EQUAL(res, -3.141592653589793);

	y = 100000000000000.0;
	x = 0.000000000000000000001;
	res = GsbIeee754::atan2(y, x);
	CU_ASSERT_EQUAL(res, 1.5707963267948966);
	
	y = 100000000000000.0;
	x = -0.000000000000000000001;
	res = GsbIeee754::atan2(y, x);
	CU_ASSERT_EQUAL(res, 1.5707963267948966);

	y = -100000000000000.0;
	x = -0.000000000000000000001;
	res = GsbIeee754::atan2(y, x);
	CU_ASSERT_EQUAL(res, -1.5707963267948966);
}

int executeUnitTests_GsbIeee754Test(void) {
	bool error = false;

	CU_pSuite pSuite = 0;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	// add a suite to the registry
	pSuite = CU_add_suite("GsbIeee754Test", initSuite_GsbIeee754Test, cleanSuite_GsbIeee754Test);

	if (0 == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	// add the tests to the suite
	if ((CU_add_test(pSuite, " Test of GsbIeee754::getFloatFromUword()", GsbIeee754Test_getFloatFromUword) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::getUwordFromFloat()", GsbIeee754Test_getUwordFromFloat) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::floatUwordIsNan()", GsbIeee754Test_floatUwordIsNan) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::floatUwordIsInfinite()", GsbIeee754Test_floatUwordIsInfinite) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::getHighWord()", GsbIeee754Test_getHighWord) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::getLowWord()", GsbIeee754Test_getLowWord) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::setHighWord()", GsbIeee754Test_setHighWord) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::setLowWord()", GsbIeee754Test_setLowWord) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::insertWords()", GsbIeee754Test_insertWords) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::expf()", GsbIeee754Test_expf) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::floor()", GsbIeee754Test_floor) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::copySign()", GsbIeee754Test_copySign) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::scalbn()", GsbIeee754Test_scalbn) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::kernelCos()", GsbIeee754Test_kernelCos) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::kernelSin()", GsbIeee754Test_kernelSin) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::getKernelRemainderPio2()", GsbIeee754Test_getKernelRemainderPio2) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::getRemainderPio2()", GsbIeee754Test_getRemainderPio2) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::cos()", GsbIeee754Test_cos) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::sin()", GsbIeee754Test_sin) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::atan()", GsbIeee754Test_atan) == 0)
			|| (CU_add_test(pSuite, " Test of GsbIeee754::atan2()", GsbIeee754Test_atan2) == 0)) {
		error = true;
	}

	// Error Code
	printf("\n\n The Error Code is %d : %s \n\n", CU_get_error(), CU_get_error_msg());

	if (error) {
		CU_cleanup_registry();
		return CU_get_error();
	}

#ifdef JENKINS
	// Run all tests using the CUnit Automated interface (XML)
	CU_automated_run_tests();
#endif
	// Run all tests using the CUnit Basic interface
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
