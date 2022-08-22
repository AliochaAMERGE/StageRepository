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

 L0AppCcdPositionDetailedDispatcher.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0AppCcdPositionDetailedDispatcher.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines ... TO BE COMPLETED ...

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 19 mai 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPCCDPOSITIONDETAILEDDISPATCHER_HPP_
#define L0APPCCDPOSITIONDETAILEDDISPATCHER_HPP_

#include <plato/das/l0app/science/calculator/L0AppCcdPositionDetailedHandler.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace events {
				namespace parameterCalculation {

					class CcdPositionDetailedDispatcher : public CcdPositionDetailedHandler {
					 private:
						/// Non functional copy constructor
						CcdPositionDetailedDispatcher(const CcdPositionDetailedDispatcher& other);
						/// Non functional assignment operator
						CcdPositionDetailedDispatcher(const CcdPositionDetailedDispatcher* other);
						/// Non functional assignment operator
						CcdPositionDetailedDispatcher& operator=(const CcdPositionDetailedDispatcher& other);

						/// camA event producer
						CcdPositionDetailedHandler* ncamaCcdPositionDetailedHandler;
						/// camB event producer
						CcdPositionDetailedHandler* ncambCcdPositionDetailedHandler;

					 public:
						/// Default constructor
						CcdPositionDetailedDispatcher();
						/**
						 * @brief Inehrited method from the CcdPositionDetailedHandler class
						 * Forward the call to the destination event producer
						 */
						virtual void processCcdPositionDetailed(const cameraIdentifier::CameraIdentifierEnum ncamId, const CcdPositionDetailedParameters& parameters);

						/**
						 * @brief connect the cama CcdPositionDetailedHandler
						 * @param handler the event handler to connect
						 */
						void setNcamaCcdPositionDetailedHandler(CcdPositionDetailedHandler* handler) {
							this->ncamaCcdPositionDetailedHandler = handler;
						}

						/**
						 * @brief connect the camb CcdPositionDetailedHandler
						 * @param handler the event handler to connect
						 */
						void setNcambCcdPositionDetailedHandler(CcdPositionDetailedHandler* handler) {
							this->ncambCcdPositionDetailedHandler = handler;
						}
					};
				}
			}
		}
	}
}

using plato::das::l0app::events::parameterCalculation::CcdPositionDetailedDispatcher;

#endif /* L0APPCCDPOSITIONDETAILEDDISPATCHER_HPP_ */
