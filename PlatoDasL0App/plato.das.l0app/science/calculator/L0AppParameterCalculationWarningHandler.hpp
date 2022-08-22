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

 L0appParameterCalculationWarningHandler.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0appParameterCalculationWarningHandler.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines the handler of the parameter calculation warning event

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 12 mai 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPPARAMETERCALCULATIONWARNINGHANDLER_HPP_
#define L0APPPARAMETERCALCULATIONWARNINGHANDLER_HPP_

#include <stdint.h>

#include <plato/das/idb/common/enumerations/evtErrorCodeEnumerations/IdbParamCalcWarCodeTypeEnum.hpp>
#include <plato/das/idb/common/enumerations/miscEnumerations/IdbCcdNumberTypeEnum.hpp>
#include <plato/das/idb/util/identifier/IdbCameraIdentifierEnum.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace science {
				namespace calculator {

					class ParameterCalculationWarningHandler {
					private:
						/// Non functional copy constructor
						ParameterCalculationWarningHandler(ParameterCalculationWarningHandler const* source);
						/// Non functional copy constructor
						ParameterCalculationWarningHandler(ParameterCalculationWarningHandler const& source);
						/// Non functional assignment operator
						ParameterCalculationWarningHandler& operator=(ParameterCalculationWarningHandler& source);

					public:
						/**
						 * Empty default constructor
						 */
						ParameterCalculationWarningHandler() {}

						/**
						 * @brief Handler method for parameter calculation warnings
						 * @param ncamId camera identifier
						 * @param pCalcWarCode error Code
						 * @param descrId star identifier
						 * @param ccdId CCD identifier (default CCD1)
						 * @param starCcdX position of the star on the CCD (as a floating point value) (default 0.0)
						 * @param starCcdY position of the star on the CCD (as a floating point value) (default 0.0)
						 */
						virtual void processParameterCalculationWarning(const cameraIdentifier::CameraIdentifierEnum ncamId,
								const idbParamCalcWarCodeType::IdbParamCalcWarCodeTypeEnum pCalcWarCode, const uint32_t descrId,
								const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const double starCcdX, const double starCcdY) = 0;
					};

				}
			}
		}
	}
}

using plato::das::l0app::science::calculator::ParameterCalculationWarningHandler;

#endif /* L0APPPARAMETERCALCULATIONWARNINGHANDLER_HPP_ */
