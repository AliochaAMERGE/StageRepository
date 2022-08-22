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

 PlatoDasL0App Component

 L0AppCcdPositionSummaryParameters.cpp

 $Rev: 000 $

 ************************************************************************/

#include <plato/das/l0app/science/calculator/L0AppCcdPositionSummaryParameters.hpp>

CcdPositionSummaryParameters& CcdPositionSummaryParameters::operator=(CcdPositionSummaryParameters const& source) {
	// check that the source and the destination are not the same object
	if (&source != this) {
		// Copy all element from source to this
		evtInfId = source.evtInfId;
		timestamp = source.timestamp;
		numStarPosComp = source.numStarPosComp;
		starNumCcd1Right = source.starNumCcd1Right;
		starNumCcd1Left = source.starNumCcd1Left;
		starNumCcd2Right = source.starNumCcd2Right;
		// copy these elements too
		starNumCcd2Left = source.starNumCcd2Left;
		starNumCcd3Right = source.starNumCcd3Right;
		starNumCcd3Left = source.starNumCcd3Left;
		starNumCcd4Right = source.starNumCcd4Right;
		starNumCcd4Left = source.starNumCcd4Left;
	}
	// return this
	return *this;
}
