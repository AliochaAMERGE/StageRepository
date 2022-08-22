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

 L0AppTmNcamREvtInfCcdPosDetailedProducer.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0AppTmNcamREvtInfCcdPosDetailedProducer.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines ... TO BE COMPLETED ...

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 19 mai 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPTMNCAMREVTINFCCDPOSDETAILEDPRODUCER_HPP_
#define L0APPTMNCAMREVTINFCCDPOSDETAILEDPRODUCER_HPP_

#include <plato/das/idb/processes/ncam/packets/telemetry/progressEventReporting/IdbTmNcamREvtInfCcdPosDetailedProducer.hpp>
#include <plato/das/l0app/science/calculator/L0AppCcdPositionDetailedHandler.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace events {
				namespace parameterCalculation {
					/**
					 * @brief This template class is used to handle a parameter calculation informative envent and producer a meta defined TM_NCxx_R_EVT_INF_CCD_POS_DETAILED packet.
					 */
					template <typename cameraIdentifier::CameraIdentifierEnum cameraId>
					class TmNcamREvtInfCcdPosDetailedProducer : public CcdPositionDetailedHandler, public IdbTmNcamREvtInfCcdPosDetailedProducer<cameraId> {
					 private:
						/// Non functional copy constructor
						TmNcamREvtInfCcdPosDetailedProducer(const TmNcamREvtInfCcdPosDetailedProducer& other);
						/// Non functional copy constructor
						TmNcamREvtInfCcdPosDetailedProducer(const TmNcamREvtInfCcdPosDetailedProducer* other);
						/// Non functional assignement operator
						TmNcamREvtInfCcdPosDetailedProducer& operator=(const TmNcamREvtInfCcdPosDetailedProducer& other);

					 public:
						/// Default empty constructor
						TmNcamREvtInfCcdPosDetailedProducer(){};
						/**
						 * @brief Inherited from CcdPositionDetailedHandler
						 * Update the data pool element related to the informative event, and produce the event packet
						 */
						virtual void processCcdPositionDetailed(const cameraIdentifier::CameraIdentifierEnum ncamId, const CcdPositionDetailedParameters& parameters);
					};
				}
			}
		}
	}
}

using plato::das::l0app::events::parameterCalculation::TmNcamREvtInfCcdPosDetailedProducer;

#endif /* L0APPTMNCAMREVTINFCCDPOSDETAILEDPRODUCER_HPP_ */
