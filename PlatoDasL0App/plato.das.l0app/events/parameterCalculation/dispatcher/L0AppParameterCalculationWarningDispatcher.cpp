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

 L0AppParameterCalculationWarningDispatcher.cpp

 $Rev: 000 $

 ************************************************************************/

#include <plato/das/l0app/events/parameterCalculation/dispatcher/L0AppParameterCalculationWarningDispatcher.hpp>

ParameterCalculationWarningDispatcher::ParameterCalculationWarningDispatcher() {
	// Initialize the pointers to 0
	ncamaWarningHandler = 0;
	ncambWarningHandler = 0;
}

void ParameterCalculationWarningDispatcher::processParameterCalculationWarning(const cameraIdentifier::CameraIdentifierEnum ncamId,
		const idbParamCalcWarCodeType::IdbParamCalcWarCodeTypeEnum pCalcWarCode, const uint32_t descrId, const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const double starCcdX, const double starCcdY) {
	// By default the warning handler depend on the value of the input ncamId parameter
	ParameterCalculationWarningHandler* warningHandler = 0;

	// Select the warning handler depending on the value of the input ncamId parameter
	if (ncamId == cameraIdentifier::NCAM_A) {
		warningHandler = ncamaWarningHandler;
	}
	else if (ncamId == cameraIdentifier::NCAM_B) {
		warningHandler = ncambWarningHandler;
	}

	if (warningHandler != 0) {
		// forward the call to the selected warning handler
		warningHandler->processParameterCalculationWarning(ncamId, pCalcWarCode, descrId, ccdId, starCcdX, starCcdY);
	}
}