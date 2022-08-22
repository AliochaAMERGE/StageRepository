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

 L0AppCcdPositionSummaryDispatcher.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0AppCcdPositionSummaryDispatcher.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines ... TO BE COMPLETED ...

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 17 mai 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPCCDPOSITIONSUMMARYDISPATCHER_HPP_
#define L0APPCCDPOSITIONSUMMARYDISPATCHER_HPP_

#include <plato/das/l0app/science/calculator/L0AppCcdPositionSummaryHandler.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace events {
				namespace parameterCalculation {
					/**
					 * @brief This class is used to dispatch the parameter calculation informative event
					 * TM_NCxx_R_EVT_INF_CCD_POS_SUMMARY
					 */
					class CcdPositionSummaryDispatcher : public CcdPositionSummaryHandler {
					 private:
						/// Non functional copy constructor
						CcdPositionSummaryDispatcher(const CcdPositionSummaryDispatcher& other);
						/// Non functional copy constructor
						CcdPositionSummaryDispatcher(const CcdPositionSummaryDispatcher* other);
						/// Non functional assignment operator
						CcdPositionSummaryDispatcher& operator=(const CcdPositionSummaryDispatcher& other);

						/// camA event producer
						CcdPositionSummaryHandler* ncamaCcdPositionSummaryHandler;
						/// camB event producer
						CcdPositionSummaryHandler* ncambCcdPositionSummaryHandler;

					 public:
						/// Default constructor
						CcdPositionSummaryDispatcher();
						/**
						 * @brief Inehrited method from the CcdPositionSummaryHandler class
						 * Forward the call to the destination event producer
						 */
						virtual void processCcdPositionSummary(const cameraIdentifier::CameraIdentifierEnum ncamId, const CcdPositionSummaryParameters& parameters);

						/**
						 * @brief Connect the camA CcdPositionSummaryHandler
						 * @param handler the event handler to connect
						 */
						void setNcamaCcdPositionSummaryHandler(CcdPositionSummaryHandler* handler) {
							this->ncamaCcdPositionSummaryHandler = handler;
						}

						/**
						 * @brief Connect the camB CcdPositionSummaryHandler
						 * @param handler the event handler to connect
						 */
						void setNcambCcdPositionSummaryHandler(CcdPositionSummaryHandler* handler) {
							this->ncambCcdPositionSummaryHandler = handler;
						}
					};
				}
			}
		}
	}
}

using plato::das::l0app::events::parameterCalculation::CcdPositionSummaryDispatcher;

#endif /* L0APPCCDPOSITIONSUMMARYDISPATCHER_HPP_ */
