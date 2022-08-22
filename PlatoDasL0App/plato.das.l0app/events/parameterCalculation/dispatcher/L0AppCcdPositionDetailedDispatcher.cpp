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

 L0AppCcdPositionDetailedDispatcher.cpp

 $Rev: 000 $

 ************************************************************************/

#include <plato/das/l0app/events/parameterCalculation/dispatcher/L0AppCcdPositionDetailedDispatcher.hpp>

CcdPositionDetailedDispatcher::CcdPositionDetailedDispatcher() {
	// Initialize the pointers to 0
	ncamaCcdPositionDetailedHandler = 0;
	ncambCcdPositionDetailedHandler = 0;
}

void CcdPositionDetailedDispatcher::processCcdPositionDetailed(const cameraIdentifier::CameraIdentifierEnum ncamId, const CcdPositionDetailedParameters& parameters) {
	// By default the warning handler depend on the value of the input ncamId parameter
	CcdPositionDetailedHandler* ccdPositionDetailedHandler = 0;

	// Select the warning handler depending on the value of the input ncamId parameter
	if (ncamId == cameraIdentifier::NCAM_A) {
		ccdPositionDetailedHandler = ncamaCcdPositionDetailedHandler;
	}
	else if (ncamId == cameraIdentifier::NCAM_B) {
		ccdPositionDetailedHandler = ncambCcdPositionDetailedHandler;
	}

	if (ccdPositionDetailedHandler != 0) {
		// forward the call to the selected warning handler
		ccdPositionDetailedHandler->processCcdPositionDetailed(ncamId, parameters);
	}
}