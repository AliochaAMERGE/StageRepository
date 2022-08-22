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

 L0AppParameterCalculationWarningDispatcher.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0AppParameterCalculationWarningDispatcher.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines ... TO BE COMPLETED ...

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 12 mai 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPPARAMETERCALCULATIONWARNINGDISPATCHER_HPP_
#define L0APPPARAMETERCALCULATIONWARNINGDISPATCHER_HPP_

#include <plato/das/l0app/science/calculator/L0AppParameterCalculationWarningHandler.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace events {
				namespace parameterCalculation {

					/**
					 * @brief This class is used to dispatch the parameter calculation warning events to the event producer associated to the given camera Id
					 *
					 */
					class ParameterCalculationWarningDispatcher : public ParameterCalculationWarningHandler {
					 private:
						/// Non functional copy constructor
						ParameterCalculationWarningDispatcher(const ParameterCalculationWarningDispatcher* other);
						/// Non functional copy constructor
						ParameterCalculationWarningDispatcher(const ParameterCalculationWarningDispatcher& other);
						/// Non functional overload of the assignment operator
						ParameterCalculationWarningDispatcher& operator=(const ParameterCalculationWarningDispatcher& other);

						ParameterCalculationWarningHandler* ncamaWarningHandler;	///< camA event producer
						ParameterCalculationWarningHandler* ncambWarningHandler;	///< camB event producer

					 public:
						/// Default constructor
						ParameterCalculationWarningDispatcher();

						/**
						 * @brief Inehrited from the ParameterCalculationWarningHandler class
						 * Forward the call to the designated event producer.
						 */
						virtual void processParameterCalculationWarning(const cameraIdentifier::CameraIdentifierEnum ncamId, const idbParamCalcWarCodeType::IdbParamCalcWarCodeTypeEnum pCalcWarCode,
								const uint32_t descrId, const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const double starCcdX, const double starCcdY);

						/**
						 * @brief Connect the camA parameter calculation warning event handler
						 * @param[in] handler The event handler to connect
						 */
						void setNcamaWarningHandler(ParameterCalculationWarningHandler* handler) {
							this->ncamaWarningHandler = handler;
						}

						/**
						 * @brief Connect the camB parameter calculation warning event handler
						 * @param[in] handler The event handler to connect
						 */
						void setNcambWarningHandler(ParameterCalculationWarningHandler* handler) {
							this->ncambWarningHandler = handler;
						}
					};
				}
			}
		}
	}
}

using plato::das::l0app::events::parameterCalculation::ParameterCalculationWarningDispatcher;

#endif /* L0APPPARAMETERCALCULATIONWARNINGDISPATCHER_HPP_ */
