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

 GsbIeee754CopySign.cpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************
 * 
 * @file GsbIeee754CopySign.cpp
 * @brief Implementation of the GsbIeee754::copySign() function
 * 
 * The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
 * The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
 * 
 * Changes :
 * 		- All the code has be reworked in order to follow LESIA's Flight Software C++ Coding Standards.
 * 		- The algorithm remain the same, only the variables names has been changed.
 ************************************************************************/

#include <gericos/blocks/math/GsbIeee754.hpp>

double GsbIeee754::copySign(double magnitude, double sign) {
	// retrieve magnitude's highWord
	uint32_t magnitudeMsb = GsbIeee754::getHighWord(magnitude);
	// retrieve sign's highWord
	uint32_t signMsb = GsbIeee754::getHighWord(sign);
	// remove magnitude's sign bit, remove every bits of sign but the sign bit, then add sign's sign bit to magnitude.
	GsbIeee754::setHighWord(magnitude, (magnitudeMsb & 0x7FFFFFFFU) | (signMsb & 0x80000000U));
	return magnitude;
}
