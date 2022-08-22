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

 L0AppTmNcamREvtInfCcdPosSummaryProducer.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0AppTmNcamREvtInfCcdPosSummaryProducer.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines ... TO BE COMPLETED ...

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 17 mai 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPTMNCAMREVTINFCCDPOSSUMMARYPRODUCER_HPP_
#define L0APPTMNCAMREVTINFCCDPOSSUMMARYPRODUCER_HPP_

#include <plato/das/idb/processes/ncam/packets/telemetry/progressEventReporting/IdbTmNcamREvtInfCcdPosSummaryProducer.hpp>
#include <plato/das/l0app/science/calculator/L0AppCcdPositionSummaryHandler.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace events {
				namespace parameterCalculation {
					/**
					 * @brief This template class is used to handle a parameter calculation informative event and produce a meta defined TM_NCxx_R_EVT_INF_CCD_POS_SUMMARY Packet
					 */
					template <typename cameraIdentifier::CameraIdentifierEnum cameraId>
					class TmNcamREvtInfCcdPosSummaryProducer : public CcdPositionSummaryHandler, public IdbTmNcamREvtInfCcdPosSummaryProducer<cameraId> {
					 private:
						/// Non functional copy constructor
						TmNcamREvtInfCcdPosSummaryProducer(const TmNcamREvtInfCcdPosSummaryProducer* other);
						/// Non functional copy constructor
						TmNcamREvtInfCcdPosSummaryProducer(const TmNcamREvtInfCcdPosSummaryProducer& other);
						/// Non functional overloaded assignment operator
						TmNcamREvtInfCcdPosSummaryProducer& operator=(const TmNcamREvtInfCcdPosSummaryProducer& other);

					 public:
						/// Default constructor
						TmNcamREvtInfCcdPosSummaryProducer(){};

						/**
						 * @brief Inherited from CcdPositionSummaryHandler
						 * Update the data pool element related to the informative event, and produce the event packet
						 */
						virtual void processCcdPositionSummary(const cameraIdentifier::CameraIdentifierEnum ncamId, const CcdPositionSummaryParameters& parameters);
					};
				}
			}
		}
	}
}

using plato::das::l0app::events::parameterCalculation::TmNcamREvtInfCcdPosSummaryProducer;

#endif /* L0APPTMNCAMREVTINFCCDPOSSUMMARYPRODUCER_HPP_ */
