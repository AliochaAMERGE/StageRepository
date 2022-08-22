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

 L0AppCcdPositionSummaryDispatcher.cpp

 $Rev: 000 $

 ************************************************************************/

#include <plato/das/l0app/events/parameterCalculation/dispatcher/L0AppCcdPositionSummaryDispatcher.hpp>

CcdPositionSummaryDispatcher::CcdPositionSummaryDispatcher() {
	// Initialize the pointers to 0
	ncamaCcdPositionSummaryHandler = 0;
	ncambCcdPositionSummaryHandler = 0;
}

void CcdPositionSummaryDispatcher::processCcdPositionSummary(const cameraIdentifier::CameraIdentifierEnum ncamId, const CcdPositionSummaryParameters& parameters) {
	// By default the warning handler depend on the value of the input ncamId parameter
	CcdPositionSummaryHandler* ccdPositionSummaryHandler = 0;

	// Select the warning handler depending on the value of the input ncamId parameter
	if (ncamId == cameraIdentifier::NCAM_A) {
		ccdPositionSummaryHandler = ncamaCcdPositionSummaryHandler;
	}
	else if (ncamId == cameraIdentifier::NCAM_B) {
		ccdPositionSummaryHandler = ncambCcdPositionSummaryHandler;
	}

	if (ccdPositionSummaryHandler != 0) {
		// forward the call to the selected warning handler
		ccdPositionSummaryHandler->processCcdPositionSummary(ncamId, parameters);
	}
}