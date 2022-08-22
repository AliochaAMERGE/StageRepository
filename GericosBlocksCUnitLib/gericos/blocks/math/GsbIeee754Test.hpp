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

 GsbIeee754Test.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 GsbIeee754Test.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines the prototype of function used in GsbIeee754 tests.

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 2 mars 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef GSBIEEE754TEST_HPP
#define GSBIEEE754TEST_HPP

int initSuite_GsbIeee754Test(void);

int cleanSuite_GsbIeee754Test(void);

void GsbIeee754Test_getUwordFromFloat(void);
void GsbIeee754Test_getFloatFromUword(void);
void GsbIeee754Test_floatUwordIsNan(void);
void GsbIeee754Test_floatUwordIsInfinite(void);
void GsbIeee754Test_getHighWord(void);
void GsbIeee754Test_getLowWord(void);
void GsbIeee754Test_setHighWord(void);
void GsbIeee754Test_setLowWord(void);
void GsbIeee754Test_insertWords(void);
void GsbIeee754Test_expf(void);
void GsbIeee754Test_floor(void);
void GsbIeee754Test_copySign(void);
void GsbIeee754Test_scalbn(void);
void GsbIeee754Test_kernelCos(void);
void GsbIeee754Test_kernelSin(void);
void GsbIeee754Test_getKernelRemainderPio2(void);
void GsbIeee754Test_getRemainderPio2(void);
void GsbIeee754Test_cos(void);
void GsbIeee754Test_sin(void);
void GsbIeee754Test_atan(void);
void GsbIeee754Test_atan2(void);

int executeUnitTests_GsbIeee754Test(void);

#endif /* GSBIEEE754TEST_HPP */
