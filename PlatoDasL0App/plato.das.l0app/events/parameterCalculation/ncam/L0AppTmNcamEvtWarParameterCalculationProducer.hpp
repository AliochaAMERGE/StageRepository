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

 L0AppTmNcamEvtWarParameterCalculationProducer.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0AppTmNcamEvtWarParameterCalculationProducer.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines ... TO BE COMPLETED ...

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 12 mai 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPTMNCAMEVTWARPARAMETERCALCULATIONPRODUCER_HPP_
#define L0APPTMNCAMEVTWARPARAMETERCALCULATIONPRODUCER_HPP_

#include <plato/das/idb/processes/ncam/packets/telemetry/warningEventReporting/IdbTmNcamREvtWarParameterCalculationProducer.hpp>
#include <plato/das/l0app/science/calculator/L0AppParameterCalculationWarningHandler.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace events {
				namespace parameterCalculation {
					/**
					 * @brief This template class is used to handle a parameter calculation warning event and produce a meta-defined TM_NCxx_R_EVT_WAR_PARAMETER_CALCULATION Packet.
					 */
					template <typename cameraIdentifier::CameraIdentifierEnum cameraId>
					class TmNcamEvtWarParameterCalculationProducer : public ParameterCalculationWarningHandler, public IdbTmNcamREvtWarParameterCalculationProducer<cameraId> {
					 private:
						/// Non functional copy constructor
						TmNcamEvtWarParameterCalculationProducer(const TmNcamEvtWarParameterCalculationProducer* other);
						/// Non functional copy constructor
						TmNcamEvtWarParameterCalculationProducer(const TmNcamEvtWarParameterCalculationProducer& other);
						/// Non functional overload of the assignment operator
						TmNcamEvtWarParameterCalculationProducer& operator=(const TmNcamEvtWarParameterCalculationProducer& other);

					 public:
						/// Default constructor
						TmNcamEvtWarParameterCalculationProducer(){};

						/**
						 * @brief Inherited from ParameterCalculationWarningHandler
						 * Update the data pool element related to the warning event, and produce the event packet.
						 */
						virtual void processParameterCalculationWarning(const cameraIdentifier::CameraIdentifierEnum ncamId, const idbParamCalcWarCodeType::IdbParamCalcWarCodeTypeEnum pCalcWarCode,
								const uint32_t descrId, const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const double starCcdX, const double starCcdY);
					};

				}
			}
		}
	}
}

using plato::das::l0app::events::parameterCalculation::TmNcamEvtWarParameterCalculationProducer;

#endif /* L0APPTMNCAMEVTWARPARAMETERCALCULATIONPRODUCER_HPP_ */
